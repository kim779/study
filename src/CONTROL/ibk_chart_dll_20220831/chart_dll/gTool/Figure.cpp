// Figure.cpp: implementation of the CFigure class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Figure.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CFigure::CFigure(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname)
: CToolBase(pView, pParent, toolRC, drawRC, bgColor, pIndcBase, tkname)
{
	m_bDrawPosi = true;
}

CFigure::~CFigure()
{

}

///////////////////////////////////////////////////////////////////////////
// class member function

void CFigure::DrawTool(CDC* pDC)
{
	if (m_pointStart.x == m_pointEnd.x && m_pointStart.y == m_pointEnd.y)
		return;

	CPen* ppenCur;
	CPoint pointStart;
	CPoint pointEnd;
	if (!m_bMove)
	{
		m_pIndcBase->GetValueToPoint(pointStart, m_dPosStart, m_dValueStart);
		m_pIndcBase->GetValueToPoint(pointEnd, m_dPosEnd, m_dValueEnd);
		m_pointStart = pointStart;
		m_pointEnd = pointEnd;

		ppenCur = GetPen(m_pwndView, m_btLStyle, m_btLWidth, m_rgbText);
	}
	else
	{
		pointStart = m_pointStart;
		pointEnd = m_pointEnd;
		m_pIndcBase->GetPtToPosValue(pointStart, m_dPosStart, m_dValueStart);
		m_pIndcBase->GetPtToPosValue(pointEnd, m_dPosEnd, m_dValueEnd);

		ppenCur = GetPen(m_pwndView, PS_DOT, 1, m_rgbMove);
	}

	if (m_bClipRegion)
		pDC->IntersectClipRect(&m_rectGrp);

	int iOldMode = pDC->SetBkMode(TRANSPARENT);
	CPen* ppenOld = pDC->SelectObject(ppenCur);
	int iOldRop = pDC->SetROP2(R2_XORPEN);

	CalculateDrawingPosition();
	pDC->SelectObject(GetStockObject(NULL_BRUSH));
	switch(m_eToolKind)
	{
	case enumTK::tkRECT:		// 사각형 
		pDC->Rectangle(m_pointStart.x, m_pointStart.y, m_pointEnd.x, m_pointEnd.y);

		if ((m_dwToolOption & TO_SHOW_POSI) && m_bDrawPosi)
		{
			CString strTextS;
			CString strTextE;
			strTextS.Format("%s (%s)", m_pIndcBase->GetYposValue(m_pointStart, false), m_pIndcBase->GetXposValue(m_pointStart));
			strTextE.Format("%s (%s)", m_pIndcBase->GetYposValue(m_pointEnd, false), m_pIndcBase->GetXposValue(m_pointEnd));

			CFont* pfontCur = GetFont(m_pwndView, 9, "굴림");
			CFont* pfontOld = pDC->SelectObject(pfontCur);

			CSize sizeS = pDC->GetTextExtent(strTextS);
			CSize sizeE = pDC->GetTextExtent(strTextE);

			CRect rectTextS;
			CRect rectTextE;
			rectTextS.SetRect(m_pointStart.x + 4, m_pointStart.y + 4, m_pointStart.x + sizeS.cx + 4, m_pointStart.y + sizeS.cy + 4);
			rectTextE.SetRect(m_pointEnd.x - sizeS.cx - 4, m_pointEnd.y - sizeS.cy - 4, m_pointEnd.x - 4, m_pointEnd.y - 4);

			COLORREF rgbOld = pDC->SetTextColor(m_rgbMove);
			pDC->DrawText(strTextS, rectTextS, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			pDC->DrawText(strTextE, rectTextE, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

			pDC->SetTextColor(rgbOld);
			pDC->SelectObject(pfontOld);
			
		}
		break;
	case enumTK::tkARC:		// 타원 
		if (m_bMove)
			pDC->Rectangle(m_pointStart.x, m_pointStart.y, m_pointEnd.x, m_pointEnd.y);
		pDC->Ellipse(m_pointStart.x, m_pointStart.y, m_pointEnd.x, m_pointEnd.y);
		break;
	case enumTK::tkTRI:		// 직각삼각형
		pDC->MoveTo(m_pointStart);
		pDC->LineTo(m_pointEnd);
		pDC->LineTo(m_pointEnd.x, m_pointStart.y);
		pDC->LineTo(m_pointStart);

		if ((m_dwToolOption & TO_SHOW_POSI) && m_bDrawPosi)
		{
			CString strTextS;
			CString strTextE;
			strTextS.Format("%s (%s)", m_pIndcBase->GetYposValue(m_pointStart, false), m_pIndcBase->GetXposValue(m_pointStart));
			strTextE.Format("%s (%s)", m_pIndcBase->GetYposValue(m_pointEnd, false), m_pIndcBase->GetXposValue(m_pointEnd));

			CFont* pfontCur = GetFont(m_pwndView, 9, "굴림");
			CFont* pfontOld = pDC->SelectObject(pfontCur);

			CSize sizeS = pDC->GetTextExtent(strTextS);
			CSize sizeE = pDC->GetTextExtent(strTextE);

			CRect rectTextS;
			CRect rectTextE;
			rectTextS.SetRect(m_pointStart.x + 4, m_pointStart.y + 4, m_pointStart.x + sizeS.cx + 4, m_pointStart.y + sizeS.cy + 4);
			rectTextE.SetRect(m_pointEnd.x + sizeS.cx + 4, m_pointEnd.y + sizeS.cy + 4, m_pointEnd.x + 4, m_pointEnd.y + 4);

			COLORREF rgbOld = pDC->SetTextColor(m_rgbMove);
			pDC->DrawText(strTextS, rectTextS, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			pDC->DrawText(strTextE, rectTextE, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

			pDC->SetTextColor(rgbOld);
			pDC->SelectObject(pfontOld);
			
		}

		break;
/*	case enumTK::tkTRI:			// 삼각형
		pDC->MoveTo(m_pointStart);
		pDC->LineTo(m_pointEnd);
		pDC->LineTo(m_pointStart.x + (m_pointEnd.x - m_pointStart.x) * 2, m_pointStart.y);
		pDC->LineTo(m_pointStart);
		break;
*/	}

	pDC->SetROP2(iOldRop);
	pDC->SelectObject(ppenOld);
	pDC->SetBkMode(iOldMode);

	CToolBase::InflateHolder();
	if (m_bShowHolder)
		CToolBase::DrawHolder(pDC, true);

	if (m_bClipRegion)
		pDC->SelectClipRgn(NULL);
}

void CFigure::HideTool()
{
	CClientDC ClientDC(m_pwndParent);
	DrawTool(&ClientDC);
}

void CFigure::CreateToolLBDN(CPoint pt, bool &bMouseCapture)
{
	m_pointStart = pt;
	m_pointEnd = pt;

	bMouseCapture = TRUE;
	m_bMove = TRUE;
	m_bClipRegion = FALSE;

	CToolBase::InflateHolder();
}

void CFigure::CreateToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	m_bDrawPosi = false;
	DrawTool(&ClientDC);
	m_pointEnd = pt;
	CToolBase::InflateHolder();
	DrawTool(&ClientDC);
	m_bDrawPosi = true;
}

void CFigure::CreateToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	CToolBase::DrawHolder(&ClientDC, true);
	DrawTool(&ClientDC);

	bMouseCapture = FALSE;

	m_bMove = FALSE;
	m_bClipRegion = TRUE;
	DrawTool(&ClientDC);
}

void CFigure::ChangeToolLBDN(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	CToolBase::DrawHolder(&ClientDC, false);

	DrawTool(&ClientDC);
	m_bClipRegion = FALSE;
	DrawTool(&ClientDC);

	if (m_rectDownHolder.PtInRect(pt))
	{
		m_eChgUpDn = enumUD::chgDN;
		m_pointStart = pt;
	}
	else if (m_rectUpHolder.PtInRect(pt))
	{
		m_eChgUpDn = enumUD::chgUP;
		m_pointEnd = pt;
	}

	m_bMove = TRUE;
	DrawTool(&ClientDC);

	::SetCapture(m_pwndParent->m_hWnd);
	bMouseCapture = TRUE;

	CToolBase::InflateHolder();
}

void CFigure::ChangeToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	if (m_bShowHolder)
		CToolBase::DrawHolder(&ClientDC, false);

	m_bDrawPosi = false;
	DrawTool(&ClientDC);
	switch (m_eChgUpDn)
	{
	case enumUD::chgDN:
		m_pointStart = pt;
		break;
	case enumUD::chgUP:
		m_pointEnd = pt;
		break;
	case enumUD::chgNO:
	default:
		return;
	}
	DrawTool(&ClientDC);
	m_bDrawPosi = true;
	CToolBase::InflateHolder();
}

void CFigure::ChangeToolLBUP(CPoint pt, bool &bMouseCapture)
{
	MoveToolLBUP(pt, bMouseCapture);
}

void CFigure::MoveToolLBDN(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	m_pointCur = pt;

	::SetCapture(m_pwndParent->m_hWnd);
	bMouseCapture = TRUE;

	if (!m_bShowHolder)
		CToolBase::DrawHolder(&ClientDC, true);
	m_bShowHolder = false;
	DrawTool(&ClientDC);

	m_bClipRegion = FALSE;
	DrawTool(&ClientDC);

	m_bMove = TRUE;
	DrawTool(&ClientDC);
	m_bShowHolder = true;
}

void CFigure::MoveToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	if (m_bShowHolder)
		CToolBase::DrawHolder(&ClientDC, false);

	m_bDrawPosi = false;
	DrawTool(&ClientDC);

	m_pointStart.x += pt.x - m_pointCur.x;
	m_pointStart.y += pt.y - m_pointCur.y;
	m_pointEnd.x += pt.x - m_pointCur.x;
	m_pointEnd.y += pt.y - m_pointCur.y;
	DrawTool(&ClientDC);
	m_bDrawPosi = true;

	m_pointCur = pt;
	CToolBase::InflateHolder();
}

void CFigure::MoveToolLBUP(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);

	::ReleaseCapture();
	bMouseCapture = FALSE;
	m_bMove = FALSE;
	m_bClipRegion = TRUE;
	m_bShowHolder = true;
	m_pwndParent->Invalidate();
}

bool CFigure::PtInToolRegion(CPoint pt)
{
	if (!m_rectGrp.PtInRect(pt))	return false;

	CRect	Rect;
	CRgn	rgn;
	CPoint	ptRgn[4];

	switch (m_eToolKind)
	{
	case enumTK::tkTRI:		// 직각삼각형 
		ptRgn[0] = m_pointStart;
		ptRgn[1] = m_pointEnd;
		ptRgn[2].x = m_pointEnd.x;
		ptRgn[2].y = m_pointStart.y;
		ptRgn[3] = m_pointStart;
		rgn.CreatePolygonRgn(ptRgn, 4, WINDING);
		break;
	case enumTK::tkRECT:		// 사각형 
	case enumTK::tkARC:		// 타원
		Rect.SetRect(m_pointStart, m_pointEnd);
		rgn.CreateRectRgnIndirect(&Rect);
		break;
	}

	BOOL bInRgn = rgn.PtInRegion(pt);
	rgn.DeleteObject();

	return (bInRgn ? true : false);
}

///////////////////////////////////////////////////////////////////////
void CFigure::CalculateDrawingPosition()
{

}
