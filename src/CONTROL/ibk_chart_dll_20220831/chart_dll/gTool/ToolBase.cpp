// ToolBase.cpp: implementation of the CToolBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToolBase.h"
#include "../../h/axisgenv.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToolBase::CToolBase(CWnd* pwndView, CWnd* pwndParent, CRect rectChart, CRect rectGrp, COLORREF rgbBG, CIndcBase* pIndcBase, int iToolKind)
{
	m_pwndView = pwndView;
	m_pwndParent = pwndParent;
	m_pIndcBase = pIndcBase;

	m_rectChart = rectChart;
	m_rectGrp = rectGrp;
	m_eToolKind = (enumTK)iToolKind;

	m_rgbBG = rgbBG;
	m_rgbLColor = RGB(0,144,0);
	m_rgbText =  m_rgbLColor ^ rgbBG;
	m_rgbMove = m_rgbLColor ^ rgbBG;
	m_rgbHolder = m_rgbBG;

	m_pointCur.x = m_pointCur.y = 0;
	m_pointStart.x = m_pointStart.y = 0;
	m_pointEnd.x = m_pointEnd.y = 0;

	m_IndexPoint1.x = m_IndexPoint1.y = 0;
	m_IndexPoint2.x = m_IndexPoint3.y = 0;
	m_IndexPoint3.x = m_IndexPoint2.y = 0;

	m_eChgUpDn = enumUD::chgNO;

	m_bShowHolder = false;
	m_rectUpHolder.SetRectEmpty();
	m_rectDownHolder.SetRectEmpty();

	m_FisrtHoder.SetRectEmpty();
	m_SecondtHoder.SetRectEmpty();
	m_ThirdHoder.SetRectEmpty();
		
	m_bMove = FALSE;
	m_bClipRegion = FALSE;

	m_strFontName = "굴림";
	m_btFontSize = 9;
	m_wFontWeight = FW_NORMAL;
	m_dwToolOption = NULL;		// added by schang 2006.03.29

	m_rgbLColor = 0;
	m_btLStyle = PS_SOLID;
	m_btLWidth = 1;

	m_PointCount = 0;
}

CToolBase::CToolBase(const CToolBase* rhs)
{
	m_pwndView = rhs->m_pwndView;
	m_pwndParent = rhs->m_pwndParent; 
	m_pIndcBase = rhs->m_pIndcBase;

	m_rectChart = rhs->m_rectChart;
	m_rectGrp = rhs->m_rectGrp;
	m_eToolKind = rhs->m_eToolKind;

	m_rgbBG = rhs->m_rgbBG;
	m_rgbHolder = rhs->m_rgbHolder;
	m_rgbText = rhs->m_rgbText;
	m_rgbMove = rhs->m_rgbMove;

	m_bShowHolder = rhs->m_bShowHolder;
	m_rectUpHolder = rhs->m_rectUpHolder;
	m_rectDownHolder = rhs->m_rectDownHolder;

	m_FisrtHoder = rhs->m_FisrtHoder;
	m_SecondtHoder = rhs->m_SecondtHoder;
	m_ThirdHoder = rhs->m_ThirdHoder;

	m_eChgUpDn   = rhs->m_eChgUpDn;
	
	m_pointCur  = rhs->m_pointCur;
	m_pointStart = rhs->m_pointStart;
	m_pointEnd  = rhs->m_pointEnd;

	m_IndexPoint1 = rhs->m_IndexPoint1;
	m_IndexPoint2 = rhs->m_IndexPoint2;
	m_IndexPoint3 = rhs->m_IndexPoint3;

	m_bMove = FALSE;
	m_bClipRegion = true;

	m_strFontName.Format("%s", rhs->m_strFontName);
	m_btFontSize = rhs->m_btFontSize;
	m_wFontWeight = rhs->m_wFontWeight;
	m_dwToolOption = rhs->m_dwToolOption;	// added by schang 2006.03.29

	m_rgbLColor = rhs->m_rgbLColor;
	m_btLStyle = rhs->m_btLStyle;
	m_btLWidth = rhs->m_btLWidth;
}

CToolBase::~CToolBase()
{

}

void CToolBase::ResizeTool(CRect rectGrp, CRect rectChart)
{
	m_rectGrp = rectGrp;
	m_rectChart = rectChart;
}

void CToolBase::DrawHolder(bool showHolder)
{
	CClientDC ClientDC(m_pwndParent);

	CBrush	*cBrush = GetBrush(m_pwndView, m_rgbHolder);
	CBrush	*sBrush = ClientDC.SelectObject(cBrush);
	int	sROP;

	//if (showHolder)
	//	m_bShowHolder = true;
	//else
	//	m_bShowHolder = false;
	m_bShowHolder = showHolder;
	
	sROP = ClientDC.SetROP2(R2_XORPEN);

	if (CheckValidPoint(m_rectDownHolder.CenterPoint()))
		ClientDC.Rectangle(&m_rectDownHolder);

	if (CheckValidPoint(m_rectUpHolder.CenterPoint()))
		ClientDC.Rectangle(&m_rectUpHolder);

	if (CheckValidPoint(m_FisrtHoder.CenterPoint()))
		ClientDC.Rectangle(&m_FisrtHoder);

	if (CheckValidPoint(m_SecondtHoder.CenterPoint()))
		ClientDC.Rectangle(&m_SecondtHoder);

	if (CheckValidPoint(m_ThirdHoder.CenterPoint()))
		ClientDC.Rectangle(&m_ThirdHoder);

	ClientDC.SetROP2(sROP);
	ClientDC.SelectObject(sBrush);
}

void CToolBase::DrawHolder(CDC *pDC, bool showHolder)
{
	CBrush	*cBrush = GetBrush(m_pwndView, m_rgbHolder);
	CBrush	*sBrush = pDC->SelectObject(cBrush);
	int	sROP;

	if (showHolder)
		m_bShowHolder = true;
	else
		m_bShowHolder = false;

	sROP = pDC->SetROP2(R2_XORPEN);

	if (CheckValidPoint(m_rectDownHolder.CenterPoint()))
		pDC->Rectangle(&m_rectDownHolder);

	if (CheckValidPoint(m_rectUpHolder.CenterPoint()))
		pDC->Rectangle(&m_rectUpHolder);

	if (CheckValidPoint(m_FisrtHoder.CenterPoint()))
		pDC->Rectangle(&m_FisrtHoder);

	if (CheckValidPoint(m_SecondtHoder.CenterPoint()))
		pDC->Rectangle(&m_SecondtHoder);

	if (CheckValidPoint(m_ThirdHoder.CenterPoint()))
		pDC->Rectangle(&m_ThirdHoder);

	pDC->SetROP2(sROP);
	pDC->SelectObject(sBrush);
}

void CToolBase::HideHolder()
{
	if (m_bShowHolder)
		DrawHolder(false);
}

void CToolBase::InflateHolder()
{
	CPoint sp = m_pointStart;
	CPoint ep = m_pointEnd;
	
	CPoint fp = m_IndexPoint1;
	CPoint dp = m_IndexPoint2;
	CPoint tp = m_IndexPoint3;

	m_rectDownHolder.SetRect(sp.x, sp.y, sp.x, sp.y);
	m_rectUpHolder.SetRect(ep.x, ep.y, ep.x, ep.y);

	m_FisrtHoder.SetRect(fp.x, fp.y, fp.x, fp.y);
	m_SecondtHoder.SetRect(dp.x, dp.y, dp.x, dp.y);
	m_ThirdHoder.SetRect(tp.x, tp.y, tp.x, tp.y);

	m_rectDownHolder.InflateRect(szHOLDER, szHOLDER);
	m_rectUpHolder.InflateRect(szHOLDER, szHOLDER);

	m_FisrtHoder.InflateRect(szHOLDER, szHOLDER);
	m_SecondtHoder.InflateRect(szHOLDER, szHOLDER);
	m_ThirdHoder.InflateRect(szHOLDER, szHOLDER);
}

bool CToolBase::PtInHolderRect(CPoint pt)
{
	if (!m_rectGrp.PtInRect(pt))	
		return false;

	if (m_rectUpHolder.PtInRect(pt) || m_rectDownHolder.PtInRect(pt)
		|| m_FisrtHoder.PtInRect(pt) || m_SecondtHoder.PtInRect(pt) || m_ThirdHoder.PtInRect(pt))	
		return true;

	return false;
}

bool CToolBase::CheckValidPoint(CPoint pt)
{
	CRect	drawRECT = m_rectGrp;
	drawRECT.DeflateRect(1, 1);
	if (drawRECT.PtInRect(pt))
		return true;

	return false;
}

void CToolBase::IndexShift()
{
	if(!m_pIndcBase->IsNoTime())//kangmc추세선변경
	{
		m_dPosStart -= 1.0;
		m_dPosEnd -= 1.0;
	}
}

void CToolBase::ChangeBackGround(int rgb)
{
	m_rgbBG = rgb;
	m_rgbHolder = m_rgbBG;
	m_rgbMove = RGB(0,144,0) ^ rgb;
	m_rgbText =  m_rgbLColor ^ m_rgbBG;
}

void CToolBase::SetFontStyle(struct _font *font)
{
	m_strFontName.Format("%s", font->acFName);
	m_btFontSize = font->btFPoint;
	m_wFontWeight = font->btFStyle;
}

//void CToolBase::GetLineStyle(struct _line* pLine)
void CToolBase::GetToolInfo(struct _toolInfo* pToolInfo)
{
	pToolInfo->rgbLine = m_rgbLColor;
	pToolInfo->btLineStyle = m_btLStyle;
	pToolInfo->btLineWidth = m_btLWidth;
	pToolInfo->dwOption = m_dwToolOption;	// added by schbang 2006.03.29

	switch (m_eToolKind)
	{
	case enumTK::tkTENDENCY:
		pToolInfo->dwOption2 = TO_START_EXT | TO_END_EXT | TO_SHOW_ANGLE | TO_SHOW_POSI;
		break;
	case enumTK::tkTENDENCYEX:
		pToolInfo->dwOption2 = TO_START_EXT | TO_SHOW_ANGLE | TO_SHOW_POSI;
		break;
	case enumTK::tkHLINE:
	case enumTK::tkCROSS:
	case enumTK::tkRECT:
	case enumTK::tkTRI:
	//case tkANDREWPICHFORK:
	//	pToolInfo->dwOption2 = TO_SHOW_POSI;
		break;
	default:
		pToolInfo->dwOption2 = NULL;
		break;
	}
}

//void CToolBase::SetLineStyle(struct _line* pLine, bool bColor, bool bStyle, bool bWidth, bool bOption)
void CToolBase::SetToolInfo(struct _toolInfo* pToolInfo, bool bOption)
{
	m_rgbLColor = pToolInfo->rgbLine;
	m_rgbText = m_rgbLColor^m_rgbBG;
	m_btLStyle = pToolInfo->btLineStyle;
	m_btLWidth = pToolInfo->btLineWidth;

	if (bOption)
		m_dwToolOption = pToolInfo->dwOption;
}

void CToolBase::SetToolColor(COLORREF rgb)
{
	m_rgbLColor = rgb;
	m_rgbText = m_rgbLColor^m_rgbBG;
}

void CToolBase::SetToolWidth(BYTE btWidth)
{
	m_btLWidth = btWidth;
}


#include "../../h/axisfire.h"
CFont* CToolBase::GetFont(CWnd *pView, int point, CString name, int bold, bool italic)
{
	_fontR rFont;
	rFont.name = (char *)name.operator LPCTSTR();
	rFont.point = point;
	rFont.italic = italic;
	rFont.bold = bold;
	return (CFont*) pView->SendMessage(WM_USER, getFONT, long(&rFont));
}

CPen* CToolBase::GetPen(CWnd *pView, int style, int width, COLORREF clr)
{
	_penR rPen;
	rPen.clr = clr;
	rPen.width = width;
	rPen.style = style;
	return (CPen*) pView->SendMessage(WM_USER, getPEN, long(&rPen));
}

CBrush* CToolBase::GetBrush(CWnd *pView, COLORREF rColor)
{
	return (CBrush*) pView->SendMessage(WM_USER, getBRUSH, rColor);
}

/*
void CToolBase::DrawPosiAngle(CDC* pDC, BOOL bPosi, BOOL bAngle)
{
	CString strTextS;
	CString strTextE;
	CRect rectS;
	CRect rectE;

	if (bPosi)
	{
		strTextS.Format("%s (%s)", m_pIndcBase->GetYposValue(m_pointStart, false), m_pIndcBase->GetXposValue(m_pointStart));
		CSize size = pDC->GetTextExtent(strTextS);
		rectS.SetRect(m_pointStart.x, m_pointStart.y, m_pointStart.x + size.cx, m_pointStart.y + size.cy);

		strTextE.Format("%s (%s)", m_pIndcBase->GetYposValue(m_pointEnd, false), m_pIndcBase->GetXposValue(m_pointEnd));
		size = pDC->GetTextExtent(strTextE);
		rectE.SetRect(m_pointEnd.x, m_pointEnd.y, m_pointEnd.x + size.cx, m_pointEnd.y + size.cy);
	}


	
	if (bAngle)
	{
		double dX = abs(m_pointEnd.x - m_pointStart.x);
		double dY = abs(m_pointEnd.y - m_pointStart.y);

		if (dX > 0)
		{
			CString strAngle;
			strAngle.Format(" %0.2f°", atan2(dY, dX) * 180 / 3.141592); 

			strTextS += strAngle;
			CSize size = pDC->GetTextExtent(strTextS);
			rectS.SetRect(m_pointStart.x, m_pointStart.y, m_pointStart.x + size.cx, m_pointStart.y + size.cy);
		}
	}

	if (strTextS.IsEmpty() && strTextE.IsEmpty())
		return;

	int iOldMode = pDC->SetBkMode(TRANSPARENT);

	COLORREF rgbOld;
	rgbOld = pDC->SetTextColor(m_rgbLColor);

	pDC->DrawText(strTextS, rectS, DT_SINGLELINE |  DT_LEFT | DT_VCENTER);
	pDC->DrawText(strTextE, rectE, DT_SINGLELINE |  DT_LEFT | DT_VCENTER);

	pDC->SetTextColor(rgbOld);
	pDC->SetBkMode(iOldMode);
}
*/