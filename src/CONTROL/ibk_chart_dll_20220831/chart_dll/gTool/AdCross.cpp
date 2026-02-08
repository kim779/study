// AdCross.cpp: implementation of the CAdCross class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AdCross.h"
#include "../../h/axisgwin.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAdCross::CAdCross(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname)
: CToolBase(pView, pParent, toolRC, drawRC, bgColor, pIndcBase, tkname)
{
	m_hLine[0] = m_hLine[1] = m_vLine[0] = m_vLine[1] = CPoint(0, 0);
	m_dwToolOption = TO_SHOW_POSI;
}

CAdCross::~CAdCross()
{

}

void CAdCross::DrawTool(CDC* pDC)
{
	CPen	*cPen;
	CPoint sp, ep;

	if (!m_bMove)
	{
		m_pIndcBase->GetValueToPoint(sp, m_dPosStart, m_dValueStart);
		m_pointStart = sp;
		cPen = GetPen(m_pwndView, m_btLStyle, m_btLWidth, m_rgbText);
	}
	else
	{
		sp = m_pointStart;
		m_pIndcBase->GetPtToPosValue(sp, m_dPosStart, m_dValueStart);
		cPen = GetPen(m_pwndView, PS_DOT, 1, m_rgbMove);
	}

	int	bkMode = pDC->SetBkMode(TRANSPARENT);
	CPen*	sPen = pDC->SelectObject(cPen);
	int	sROP = pDC->SetROP2(R2_XORPEN);

//	if (m_bClipRegion)
//		pDC->IntersectClipRect(&m_rectGrp);

	if (calculateDrawingPosition(pDC))
	{
		pDC->MoveTo(m_hLine[0].x, m_hLine[0].y);
		pDC->LineTo(m_hLine[1].x, m_hLine[1].y);
		pDC->MoveTo(m_vLine[0].x, m_vLine[0].y);
		pDC->LineTo(m_vLine[1].x, m_vLine[1].y);

		if (!m_bMove)
		{
			CString	str;
			m_pIndcBase->GetValueToStr(m_dValueStart, str);

			if (!str.IsEmpty() && (m_dwToolOption & TO_SHOW_POSI))
			{
				CFont	*cFont;
				cFont = GetFont(m_pwndView, 9, "±¼¸²");
				CFont	*sFont = pDC->SelectObject(cFont);
				int	sColor = pDC->SetTextColor(m_rgbMove);
				CSize	sz = pDC->GetOutputTextExtent(str);

				CRect	rc;
				rc.SetRect(m_rectGrp.left+5, m_hLine[0].y - sz.cy, m_rectGrp.left + (sz.cx + 5), m_hLine[0].y);
				pDC->DrawText(str, &rc, DT_SINGLELINE|DT_VCENTER|DT_LEFT);

				pDC->SetTextColor(sColor);
				pDC->SelectObject(sFont);
			}
		}
	}

	pDC->SetBkMode(bkMode);
	pDC->SetROP2(sROP);
	pDC->SelectObject(sPen);

	InflateHolder();
	if (m_bShowHolder)
		DrawHolder(pDC, true);

//	if (m_bClipRegion)
//		pDC->SelectClipRgn(NULL);
}

void CAdCross::HideTool()
{
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);
}

void CAdCross::DrawHolder(CDC *pDC, bool showHolder)
{
	CClientDC ClientDC(m_pwndParent);

	CBrush	*cBrush = GetBrush(m_pwndView, m_rgbHolder);
	CBrush	*sBrush = pDC->SelectObject(cBrush);
	int	sROP;

	if (showHolder)
	{
		m_bShowHolder = true;
		sROP = pDC->SetROP2(R2_XORPEN);
	}
	else
	{
		m_bShowHolder = false;
		sROP = pDC->SetROP2(R2_XORPEN);
	}

	if (m_cntRcHolder.right < m_rectGrp.right)
		pDC->Rectangle(&m_cntRcHolder);

	pDC->SetROP2(sROP);
	pDC->SelectObject(sBrush);
}

void CAdCross::HideHolder()
{
	CClientDC ClientDC(m_pwndParent);
	if (m_bShowHolder)
		DrawHolder(&ClientDC, false);
}

void CAdCross::InflateHolder()
{
	CPoint holder;
	CPoint	dn;

	m_cntRcHolder.SetRect(m_pointStart.x, m_pointStart.y, m_pointStart.x, m_pointStart.y);
	m_cntRcHolder.InflateRect(szHOLDER, szHOLDER);
}
	
void CAdCross::CreateToolLBDN(CPoint pt, bool &bMouseCapture)
{	
	m_pointStart = m_pointEnd = pt;

	m_bMove = TRUE;
	bMouseCapture = TRUE;
	m_bClipRegion = FALSE;

	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);
	InflateHolder();
}

void CAdCross::CreateToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	//DrawTool(&ClientDC);
	m_pointStart = m_pointEnd = pt;

	m_pwndParent->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, chartUpdate), 0);
	DrawTool(&ClientDC);
	InflateHolder();
}

void CAdCross::CreateToolLBUP(CPoint pt, bool &bMouseCapture)
{
	m_pwndParent->SendMessage(GEV_CHART, MAKEWPARAM(chartToolTip, FALSE), 0);
	CClientDC ClientDC(m_pwndParent);
	DrawHolder(&ClientDC, true);
	//DrawTool(&ClientDC);

	bMouseCapture = FALSE;
	m_bMove = FALSE;
	m_bClipRegion = TRUE;

	m_pwndParent->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, chartUpdate), 0);
	DrawTool(&ClientDC);
}

void CAdCross::MoveToolLBDN(CPoint pt, bool &bMouseCapture)
{
	m_pointCur = pt;
	
	::SetCapture(m_pwndParent->m_hWnd);
	bMouseCapture = TRUE;

	CClientDC ClientDC(m_pwndParent);
	if (!m_bShowHolder)
		DrawHolder(&ClientDC, false);
	m_bShowHolder = false;
	m_bClipRegion = FALSE;
	m_bMove = TRUE;

	m_pwndParent->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, chartUpdate), 0);
	DrawTool(&ClientDC);
	m_bShowHolder = true;
}

void CAdCross::MoveToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	if (m_bShowHolder)
		DrawHolder(&ClientDC, false);

	//DrawTool(&ClientDC);
	m_pointStart.x += pt.x - m_pointCur.x;
	m_pointStart.y += pt.y - m_pointCur.y;

	m_pwndParent->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, chartUpdate), 0);
	DrawTool(&ClientDC);

	m_pointCur = pt;
	InflateHolder();
}

void CAdCross::MoveToolLBUP(CPoint pt, bool &bMouseCapture)
{
	m_pwndParent->SendMessage(GEV_CHART, MAKEWPARAM(chartToolTip, FALSE), 0);

	CClientDC ClientDC(m_pwndParent);

	::ReleaseCapture();
	bMouseCapture = FALSE;
	m_bMove = FALSE;
	m_bClipRegion = TRUE;
	m_bShowHolder = true;
	m_pwndParent->Invalidate();
}

bool CAdCross::PtInToolHolderRect(CPoint pt)
{
	if (m_cntRcHolder.PtInRect(pt))
		return true;

	return false;
}

bool CAdCross::PtInToolRegion(CPoint pt)
{
	if (!m_rectChart.PtInRect(pt))	return false;

	CPoint	point[5];
	
	CPoint curPt;
	curPt.x = m_vLine[0].x;
	curPt.y = m_hLine[0].y;

	point[0].x = point[4].x = curPt.x - szREGION;
	point[0].y = point[4].y = curPt.y - szREGION;
	point[1].x = curPt.x - szREGION;
	point[1].y = curPt.y + szREGION;
	point[2].x = curPt.x + szREGION;
	point[2].y = curPt.y + szREGION;
	point[3].x = curPt.x + szREGION;
	point[3].y = curPt.y - szREGION;

	CRgn GannRGN;
	GannRGN.CreatePolygonRgn(point, 5, WINDING);
	BOOL bInRgn = GannRGN.PtInRegion(pt);
	GannRGN.DeleteObject();

	return (bInRgn ? true : false);
}

bool CAdCross::calculateDrawingPosition(CDC* pDC)
{
	m_hLine[0].x = m_rectGrp.left;
	m_hLine[1].x = m_rectGrp.right;
	m_hLine[0].y = m_hLine[1].y = m_pointStart.y;
	
	m_vLine[0].y = m_rectChart.top;
	m_vLine[1].y = m_rectChart.bottom;
	
	m_vLine[0].x = m_vLine[1].x = m_pointStart.x;

	return true;
}
