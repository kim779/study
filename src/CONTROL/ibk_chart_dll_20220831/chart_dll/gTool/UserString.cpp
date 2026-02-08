// UserString.cpp: implementation of the CUserString class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserString.h"
#include "UserInputDlg.h"
#include "../../h/axisgwin.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserString::CUserString(CWnd *pView, CWnd *pParent, CRect toolRC, CRect drawRC, int bgColor, CIndcBase* pIndcBase, int tkname)
: CToolBase(pView, pParent, toolRC, drawRC, bgColor, pIndcBase, tkname)
{
	m_dispRECT.SetRectEmpty();
	m_dispSTR= _T("");
}

CUserString::~CUserString()
{
//	HideTool();
}

void CUserString::DrawTool(CDC* pDC)
{
	if (m_pointStart.x == m_pointEnd.x && m_pointStart.y == m_pointEnd.y)
		return;

	if (m_dispSTR.IsEmpty())
		return;

	CFont	*cFont = GetFont(m_pwndView, m_btFontSize, m_strFontName);
	CFont	*sFont = pDC->SelectObject(cFont);

	int sMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF sColor = pDC->SetTextColor(m_rgbLColor);

	CPoint sp, ep;
	if (!m_bMove)
	{
		m_pIndcBase->GetValueToPoint(sp, m_dPosStart, m_dValueStart);

		m_pointStart = sp;
		m_pointEnd = m_pointStart;
		m_pointEnd.x += m_fWidth;
		m_pointEnd.y += m_fHeight;
		InflateHolder();
	}
	else
	{
		sp = m_pointStart;
		m_pIndcBase->GetPtToPosValue(sp, m_dPosStart, m_dValueStart);
	}

	if (m_bClipRegion)
		pDC->IntersectClipRect(&m_rectGrp);

	pDC->DrawText(m_dispSTR, &m_dispRECT, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	
	pDC->SelectObject(sFont);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);

	if (m_bShowHolder)
		DrawHolder(pDC, true);

	if (m_bClipRegion)	pDC->SelectClipRgn(NULL);
}

void CUserString::HideTool()
{
	m_pwndParent->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, chartUpdate), 1);
}

void CUserString::DrawHolder(CDC *pDC, bool showHolder)
{
	m_bShowHolder = showHolder;

	CPen	*cPen = GetPen(m_pwndView, PS_SOLID, 1, m_rgbMove);
	CPen	*sPen = pDC->SelectObject(cPen);
	pDC->SelectStockObject(NULL_BRUSH);

	int	sROP = pDC->SetROP2(R2_XORPEN);

	pDC->Rectangle(m_dispRECT);
	
	pDC->SetROP2(sROP);
	pDC->SelectObject(sPen);
}

void CUserString::HideHolder()
{
	CClientDC ClientDC(m_pwndParent);
	if (m_bShowHolder)
		DrawHolder(&ClientDC, false);
}

void CUserString::InflateHolder()
{
	m_dispRECT.SetRect(m_pointStart.x - 1, m_pointStart.y - 1,
		m_pointEnd.x + 1, m_pointEnd.y + 1);
}

void CUserString::CreateToolLBUP(CPoint pt, bool &bMouseCapture)
{
	m_pointStart = pt;
	m_pointEnd = pt;
//	m_bMove = true;
	m_bClipRegion = true;

	GetFontSize();
	m_pointEnd.x += m_fWidth;
	m_pointEnd.y += m_fHeight;

	m_pIndcBase->GetPtToPosValue(m_pointStart, m_dPosStart, m_dValueStart);
	InflateHolder();

	CClientDC ClientDC(m_pwndParent);
//	m_bShowHolder = true;
	DrawTool(&ClientDC);
//	m_bMove = false;
}

void CUserString::ChangeToolLBDN(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	if (m_bShowHolder)
		DrawHolder(&ClientDC, false);

	CString	InputStr = m_dispSTR;
	CUserInputDlg	dlg(InputStr);
	if (dlg.DoModal() == IDOK)
	{
		InputStr = dlg.GetDisplayString();
		SetDisplayString(InputStr);

		GetFontSize();
		m_pointEnd = m_pointStart;
		m_pointEnd.x += m_fWidth;
		m_pointEnd.y += m_fHeight;
		
		m_pIndcBase->GetPtToPosValue(m_pointStart, m_dPosStart, m_dValueStart);
		InflateHolder();
		m_pwndParent->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, chartUpdate), 1);
	}
}

void CUserString::MoveToolLBDN(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	m_pointCur = pt;
	m_saveRECT = m_dispRECT;
	m_saveRECT.InflateRect(1,1,1,1);
	m_bClipRegion = true;
	if (m_bShowHolder)
		DrawHolder(&ClientDC, false);
}

void CUserString::MoveToolLBMV(CPoint pt, bool &bMouseCapture)
{
	CClientDC ClientDC(m_pwndParent);
	if (m_bShowHolder)
		DrawHolder(&ClientDC, false);
	
	m_pointStart.x += pt.x - m_pointCur.x;
	m_pointStart.y += pt.y - m_pointCur.y;
	m_pointEnd.x += pt.x - m_pointCur.x;
	m_pointEnd.y += pt.y - m_pointCur.y;

	m_pIndcBase->GetPtToPosValue(m_pointStart, m_dPosStart, m_dValueStart);

	InflateHolder();
	if (!m_bShowHolder)
		DrawHolder(&ClientDC, true);
	m_pointCur = pt;
}

void CUserString::MoveToolLBUP(CPoint pt, bool &bMouseCapture)
{
	m_pwndParent->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, chartUpdate), 1);

	CClientDC ClientDC(m_pwndParent);
	if (m_bShowHolder)
		DrawHolder(&ClientDC, false);
	m_bShowHolder = true;
	m_pwndParent->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, chartUpdate), 1);
}

bool CUserString::PtInToolRegion(CPoint pt)
{
	if (!m_rectGrp.PtInRect(pt))	return false;

	CRect	rect;
	rect.SetRect(m_pointStart.x, m_pointStart.y, m_pointEnd.x, m_pointEnd.y);
	CRgn	region;
	region.CreateRectRgnIndirect(&rect);
	if (region.PtInRegion(pt))
		return true;
	return false;
}

bool CUserString::PtInHolderRect(CPoint pt)
{
	if (!m_rectGrp.PtInRect(pt))	return false;

	if (PtInToolRegion(pt))
		return false;

	if (m_dispRECT.PtInRect(pt))
		return true;

	return false;
}

void CUserString::GetFontSize()
{
	CClientDC ClientDC(m_pwndParent);

	CFont	*cFont = GetFont(m_pwndView, m_btFontSize, m_strFontName);
	CFont	*sFont = ClientDC.SelectObject(cFont);

	CSize	sz = ClientDC.GetOutputTextExtent(m_dispSTR);
	m_fWidth = sz.cx;
	m_fHeight = sz.cy;

	ClientDC.SelectObject(sFont);
}