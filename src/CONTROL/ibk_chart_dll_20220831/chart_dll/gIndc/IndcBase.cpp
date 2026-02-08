#include "stdafx.h"
#include "IndcBase.h"

#include "../gData/DataMgr.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CIndcBase::CIndcBase()
{
	m_pwndView = NULL;
	m_pwndParent = NULL;
	m_pDataMgr = NULL;

	m_pgraph = new _graph;

	ZeroMemory(m_pgraph, SZ_GRAPH);
	ZeroMemory(&m_RgnInfo, SZ_RGNINFO);
	m_RgnInfo.iCount = 0;
	m_RgnInfo.iSelect = -1;

	m_dYMax = DBL_MIN;
	m_dYMin = DBL_MAX;
	m_dXMax = DBL_MIN;
	m_dXMin = DBL_MAX;

	m_rectOrgGrp.SetRectEmpty();
	m_rectGrp.SetRectEmpty();
	m_TkInfo.tkRect[TICK_BOTTOM].SetRectEmpty();
	m_TkInfo.tkRect[TICK_LEFT].SetRectEmpty();
	m_TkInfo.tkRect[TICK_RIGHT].SetRectEmpty();
	m_TkInfo.tkHeight[TICK_BOTTOM] = 0;
	m_TkInfo.tkHeight[TICK_LEFT] = 0;
	m_TkInfo.tkHeight[TICK_RIGHT] = 0;

	m_pCoDraw = NULL;
	m_pCoDraw = NULL;
	m_iUnit = GU_CODE;
	m_iGrpKind = 0;
	m_iDataKey = 1;
	m_crText = m_pgraph->rgbLabel;

	m_iDispDtCnt = 0;

	m_strLegend.Empty();
	m_bInit = true;
}

CIndcBase::CIndcBase(CWnd* pwndView, CWnd* pwndParent, class CDataMgr* pDataMgr, char* pcInfo, char* pcGraph, int iDispDay)
{
	m_pwndView = pwndView;
	m_pwndParent = pwndParent;
	m_pDataMgr = pDataMgr;
	m_iDataKey = 1;

	m_pgraph = new _graph;
	CopyMemory(m_pgraph, pcGraph, SZ_GRAPH);

	_envInfo *info = (_envInfo *)pcInfo;
	m_wEnvOpt = info->wEnvOption;
	m_dwDspOpt = info->display.dwDspOption;
	m_crBG = info->display.rgbChartBG;
	m_crFG = info->display.rgbChartEdge;
	CopyMemory(&m_lineH, &info->display.hline, SZ_LINE);
	CopyMemory(&m_lineV, &info->display.vline, SZ_LINE);	
	CopyMemory(&m_lineDate, &info->display.dline, SZ_LINE);	
	CopyMemory(m_atick, info->display.atick, sizeof(info->display.atick));

	m_dYMax = DBL_MIN;
	m_dYMin = DBL_MAX;
	m_dXMax = DBL_MIN;
	m_dXMin = DBL_MAX;

	m_rectOrgGrp.SetRectEmpty();
	m_rectGrp.SetRectEmpty();
	m_TkInfo.tkRect[TICK_BOTTOM].SetRectEmpty();
	m_TkInfo.tkRect[TICK_LEFT].SetRectEmpty();
	m_TkInfo.tkRect[TICK_RIGHT].SetRectEmpty();
	m_TkInfo.tkHeight[TICK_BOTTOM] = 0;
	m_TkInfo.tkHeight[TICK_LEFT] = 0;
	m_TkInfo.tkHeight[TICK_RIGHT] = 0;

	m_pCoDraw = NULL;
	m_iUnit = GU_CODE;
	class COrgData *pOrgData;
	if (m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
		 m_iUnit = pOrgData->GetUnit();

	m_iGrpKind = m_pgraph->wGKind;
	m_iDataKey = m_pgraph->btDataKey;
	m_crText = m_pgraph->rgbLabel;

	// 20090918 : ADD
	//m_iValidDigit = (m_iUnit == GU_CODE || m_iUnit == GU_COFUT || m_iUnit == GU_COOPT)? 0 : 2;
	m_iValidDigit = (m_iUnit == GU_CODE || m_iUnit == GU_COFUT || m_iUnit == GU_COOPT || m_iUnit == GU_FFUTURE)? 0 : 2;

	//if (pDataMgr->m_dDollar > 1)
	if (pDataMgr->GetDollar() > 1)
		m_iValidDigit = 2;
	
	m_iUTickNum = 0;
	DWORD	chkbit = 0x00000001;
	for (int ii = 0; ii < MAX_LINE; ii++, chkbit <<= 1)
	{
		if (m_pgraph->tInfo.wCOption & chkbit)
			m_iUTickNum++;
	}

	m_iDispDtCnt = 0;
	m_strLegend = _T("");
	m_bInit = true;

	ZeroMemory(&m_RgnInfo, SZ_RGNINFO);
	m_RgnInfo.iCount = 0;
	m_RgnInfo.iSelect = -1;

	CalculateTickInfo();
	m_rectGrp.SetRectEmpty();

	m_crUpSise = GetColor(94);
	m_crDnSise = GetColor(95);
	m_crEqSise = GetColor(97);
}

CIndcBase::~CIndcBase()
{
	delete m_pgraph;

	for (int ii = 0; ii < DISP_LINEMAX; ii++)
	{
		if (m_RgnInfo.Rgn[ii].pRgn)
		{
			delete m_RgnInfo.Rgn[ii].pRgn;
			m_RgnInfo.Rgn[ii].pRgn = NULL;
		}
		if (m_RgnInfo.Rgn[ii].pRect)
		{
			delete m_RgnInfo.Rgn[ii].pRect;
			m_RgnInfo.Rgn[ii].pRect = NULL;
		}
	}
}

void CIndcBase::ResizeGraph(CRect ObjRect, CRect GrpRect, CRect DrawRect, CRect TickRect[3], int lgap, int rgap, int BalanceGap)
{
	m_rectObj = ObjRect;
	m_rectChart = GrpRect;

	m_rectOrgGrp = DrawRect;
	m_rectGrp = DrawRect;
	m_rectGrp.left += lgap;
	m_rectGrp.right -= rgap;
	
	if (m_rectGrp.left == m_rectOrgGrp.left)
		m_rectGrp.left += 1;
	if (m_rectGrp.right == m_rectOrgGrp.right)
		m_rectGrp.left -= 1;
	if (m_rectGrp.Height() > 2)
	{
		m_rectGrp.DeflateRect(0, 1);
		TickRect[TICK_LEFT].DeflateRect(0, 1);
		TickRect[TICK_RIGHT].DeflateRect(0, 1);
	}
	
	if (BalanceGap > 0)
	{
		if (m_iGrpKind != GK_BALANCE)
			m_rectGrp.right -= int(double(m_rectGrp.Width()*BalanceGap) / 
					double(m_iDispDtCnt + BalanceGap));
	}

	m_TkInfo.tkRect[TICK_BOTTOM] = TickRect[TICK_BOTTOM];
	m_TkInfo.tkRect[TICK_LEFT] = TickRect[TICK_LEFT];
	m_TkInfo.tkRect[TICK_RIGHT] = TickRect[TICK_RIGHT];

	int	maxGap = m_TkInfo.tkHeight[TICK_BOTTOM] + 3;
	switch (m_iGrpKind)
	{
	case GK_JPN:
	case GK_BAR:
	case GK_LIN:
	case GK_POLE:
	case GK_CDV:
	case GK_EQV:
	case GK_PMA:
	case GK_NET:
	case GK_BOLB:
	case GK_ENVL:
	case GK_PARB:
	case GK_BALANCE:
	case GK_PIVOT:
	case GK_DEMARK:
	case GK_AVOL:
//		maxGap += 5;	
		break;
	default:
		break;
	}

	if (m_rectGrp.Height() > int(maxGap*1.5))
	{
		m_rectGrp.top += maxGap;
		m_TkInfo.tkRect[TICK_LEFT].top += maxGap;
		m_TkInfo.tkRect[TICK_RIGHT].top += maxGap;
	}

	CalculateMinMax(m_bInit);
	if (m_bInit)
		m_bInit = false;
}

void CIndcBase::ResizeGraph(CRect ObjRect, CRect GrpRect, CRect DrawRect, CRect TickRect[3], int lgap, int rgap, int tgap, int bgap, int BalanceGap)
{
	m_rectObj = ObjRect;
	m_rectChart = GrpRect;
	
	m_rectOrgGrp = DrawRect;
	m_rectGrp = DrawRect;
	m_rectGrp.left += lgap;
	m_rectGrp.right -= rgap;
	
	if (m_rectGrp.left == m_rectOrgGrp.left)
		m_rectGrp.left += 1;
	if (m_rectGrp.right == m_rectOrgGrp.right)
		m_rectGrp.left -= 1;
	if (m_rectGrp.Height() > 2)
	{
		m_rectGrp.DeflateRect(0, 1);
		TickRect[TICK_LEFT].DeflateRect(0, 1);
		TickRect[TICK_RIGHT].DeflateRect(0, 1);
	}
	
	if (BalanceGap > 0)
	{
		if (m_iGrpKind != GK_BALANCE)
			m_rectGrp.right -= int(double(m_rectGrp.Width()*BalanceGap) / 
			double(m_iDispDtCnt + BalanceGap));
	}
	
	m_TkInfo.tkRect[TICK_BOTTOM] = TickRect[TICK_BOTTOM];
	m_TkInfo.tkRect[TICK_LEFT] = TickRect[TICK_LEFT];
	m_TkInfo.tkRect[TICK_RIGHT] = TickRect[TICK_RIGHT];
	
	int	maxGap = m_TkInfo.tkHeight[TICK_BOTTOM] + 3;
	switch (m_iGrpKind)
	{
	case GK_JPN:
	case GK_BAR:
	case GK_LIN:
	case GK_POLE:
	case GK_CDV:
	case GK_EQV:
	case GK_PMA:
	case GK_NET:
	case GK_BOLB:
	case GK_ENVL:
	case GK_PARB:
	case GK_BALANCE:
	case GK_PIVOT:
	case GK_DEMARK:
	case GK_AVOL:
		//		maxGap += 5;	
		break;
	default:
		break;
	}
	
	if (m_rectGrp.Height() > int(maxGap*1.5))
	{
		m_rectGrp.top += maxGap;
		m_TkInfo.tkRect[TICK_LEFT].top += maxGap;
		m_TkInfo.tkRect[TICK_RIGHT].top += maxGap;
	}

	m_rectGrp.top += tgap;
	m_TkInfo.tkRect[TICK_LEFT].top += tgap;
	m_TkInfo.tkRect[TICK_RIGHT].top += tgap;
	m_rectGrp.bottom -= bgap;
	m_TkInfo.tkRect[TICK_LEFT].bottom -= bgap;
	m_TkInfo.tkRect[TICK_RIGHT].bottom -= bgap;
	
	CalculateMinMax(m_bInit);
	if (m_bInit)
		m_bInit = false;
}


void CIndcBase::DrawTickBox(CDC* pDC, COLORREF ef, COLORREF eb, COLORREF gf, COLORREF gb)
{
	int sMode = pDC->SetBkMode(TRANSPARENT);
	CBrush	*efBrush = GetBrush(ef);
	CBrush	*ebBrush = GetBrush(eb);
	CBrush	*gfBrush = GetBrush(gf);
	CBrush	*gbBrush = GetBrush(gb);
	CPen	*ebPen = GetPen(PS_SOLID, 1, eb);

	CBrush	*sBrush = pDC->SelectObject(ebBrush);
	CPen	*sPen = pDC->SelectObject(ebPen);
//	pDC->Rectangle(&m_rectObj);

	pDC->SelectObject(gbBrush);
	pDC->Rectangle(&m_rectChart);

	CRect	fRect = m_rectChart;
	fRect.bottom += 1;
	pDC->FrameRect(&fRect, gfBrush); 

	pDC->SelectObject(sBrush);
	pDC->SelectObject(sPen);
	pDC->SetBkMode(sMode);
}

void CIndcBase::DrawCurPrice(CDC *pDC)
{
	if (!(m_dwDspOpt & DO_CURRENT))
		return;

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
		return;
	
	double	Max = m_dYMax;
	double	Min = m_dYMin;

	if (Max == DBL_MIN || Min == DBL_MAX)
		return;

	double	curr = pOrgData->GetCurr();
	double	udyl = pOrgData->GetUdly();
	int	sign = pOrgData->GetSign();

	CSize	size;
	int	dispTICK;
	if (!(dispTICK = GetTickCount(TICK_RIGHT)))
		return;

	CRect	tickRC = m_TkInfo.tkRect[TICK_RIGHT];
	int	maxdigit = 7;
	char	*digitStr[10] = {"2,000,000,000", "200,000,000", "20,000,000", 
		"2,000,000", "200,000", "20,000", "2,000", "200", "20", "2"};
	for (int ii = 9; ii >= 0; ii--)
	{
		size = pDC->GetOutputTextExtent(digitStr[ii]);
		if (size.cx >= tickRC.Width())
			break;
		else
			maxdigit = strlen(digitStr[ii]);
	}

	bool	bReverse = IsReverseChart();
	double	gap = Max - Min;

	CFont	*tkFont = GetFont(TICK_RIGHT);
	CFont	*sFont = pDC->SelectObject(tkFont);
	
	int	tickHeight = tickRC.Height();
	double	ValueHeight = Max - Min;

	CPoint	pointS, pointE;
	CString	dispStr;
	CRect	textRC;

	COLORREF color;
	switch (sign)
	{
	case 1:
		color = m_crUpSise;//RGB(255,0,0);
		break;
	case -1:
		color = m_crDnSise;//RGB(0,0,255);
		break;
	case 0:
		color = m_crEqSise;//RGB(10,142,0);
		break;
	}

	if (CIndcBase::m_iValidDigit == 2)
		curr *= 100.;

	// 20090918 : CHANGE :
	//if (CIndcBase::m_iValidDigit == 0)
	//{
	//	if (m_iUnit == GU_FFUTURE)
	//	{
	//		// 데이터가 100배로 정수형으로 수신
	//		curr /= 100;
	//	}
	//}

	GetValueToYPoint(pointS, curr);
	pointE.x = m_rectOrgGrp.left;
	pointE.y = pointS.y;

	FormatVariableComma(dispStr, curr, curr);

	CString	dispStr2;
	dispStr2.Format("%.2f%%", udyl);
	size = pDC->GetOutputTextExtent(dispStr);
	CSize	size2 = pDC->GetOutputTextExtent(dispStr2);
	if (bReverse)
	{
		textRC.SetRect(tickRC.left + gapTICK, pointS.y - size.cy - 1, 
			tickRC.right, pointS.y + size.cy + 1);
	}
	else
	{
		textRC.SetRect(tickRC.left + gapTICK, pointS.y - size.cy - 1,
			tickRC.right, pointS.y + size.cy + 1);
	}

	if (size.cx > size2.cx)
	{
		if (textRC.Width() > size.cx)
			textRC.right = textRC.left + size.cx + 1;
	}
	else
	{
		if (textRC.Width() > size2.cx)
			textRC.right = textRC.left + size2.cx + 1;
	}

	COLORREF sbkColor = pDC->SetBkColor(color);
	int	sbkMode = pDC->SetBkMode(OPAQUE);
	COLORREF sColor = pDC->SetTextColor(RGB(255,255,255));

	// 주차트 tick 영역안에 한정시킴
	CRect	cmpRC;
	cmpRC.SetRect(m_TkInfo.tkRect[TICK_RIGHT].left, m_rectOrgGrp.top, 
		m_TkInfo.tkRect[TICK_RIGHT].right, m_rectOrgGrp.bottom);
	textRC.IntersectRect(&textRC, &cmpRC);

	CBrush	*lBrush = GetBrush(color);
	pDC->FillRect(&textRC, lBrush);

	if (textRC.Height() >= size.cy*2)
	{
		CRect	DrawRC = textRC;
		DrawRC.top += 1;
		DrawRC.bottom = DrawRC.top + size.cy;
		pDC->DrawText(dispStr, &DrawRC, 
			DT_CENTER|DT_SINGLELINE|DT_BOTTOM|DT_VCENTER);
		DrawRC.top = DrawRC.bottom;
		DrawRC.bottom = textRC.bottom;
		pDC->DrawText(dispStr2, &DrawRC, DT_CENTER|DT_SINGLELINE|DT_BOTTOM|DT_VCENTER);
	}
	else if (textRC.Height() >= size.cy)
	{
		CRect	DrawRC = textRC;
		DrawRC.top += 1;
		DrawRC.bottom = DrawRC.top + size.cy;
		pDC->DrawText(dispStr, &DrawRC, DT_CENTER|DT_SINGLELINE|DT_BOTTOM|DT_VCENTER);
	}

	CBrush	*fBrush = GetBrush(color);
	pDC->FrameRect(&textRC, fBrush);

	pDC->SetBkColor(sbkColor);
	pDC->SetBkMode(sbkMode);
	pDC->SetTextColor(sColor);
	pDC->SelectObject(sFont);
}

void CIndcBase::DrawPrePrice(CDC *pDC)
{
	if (!(m_dwDspOpt & DO_PCURRENT))
		return;

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
		return;

	if (m_dYMax == DBL_MIN || m_dYMin == DBL_MAX)
		return;

	double	PCurr = pOrgData->GetPCurr();
	if (CIndcBase::m_iValidDigit == 2)
		PCurr *= 100.;


	// 20090918 : CHANGE :
	//if (CIndcBase::m_iValidDigit == 0)
	//{
	//	if (m_iUnit == GU_FFUTURE)
	//	{
	//		// 데이터가 100배로 정수형으로 수신
	//		PCurr /= 100;
	//	}
	//}

	if (PCurr < m_dYMin || m_dYMax < PCurr)
		return;

	CPoint	pt;
	GetValueToYPoint(pt, PCurr);

	CPen	*cPen = GetPen(PS_SOLID, 1, m_crEqSise);//RGB(0,128,0));
	CPen	*sPen = pDC->SelectObject(cPen);

	pDC->MoveTo(m_rectOrgGrp.left, pt.y);
	pDC->LineTo(m_rectOrgGrp.right, pt.y);

	pDC->SelectObject(sPen);
}

void CIndcBase::DrawExtPrice(CDC *pDC, bool bOrderMode)
{
	if (!(m_dwDspOpt & DO_ECURRENT))
		return;

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
		return;

	double	Max = m_dYMax;
	double	Min = m_dYMin;

	if (Max == DBL_MIN || Min == DBL_MAX || m_iUnit != GU_CODE)
		return;

	double	sahg = pOrgData->GetSahg();
	double	hahg = pOrgData->GetHahg();
	double	PCurr = pOrgData->GetPCurr();
	if (sahg <= 0.0 || hahg <= 0.0)
		return;

	if (CIndcBase::m_iValidDigit == 2)
	{
		sahg *= 100.;
		hahg *= 100.;
		PCurr *= 100.;
	}

	// 20090918 : CHANGE :
	//if (CIndcBase::m_iValidDigit == 0)
	//{
	//	if (m_iUnit == GU_FFUTURE)
	//	{
	//		// 데이터가 100배로 정수형으로 수신
	//		sahg /= 100;
	//		hahg /= 100;
	//		PCurr /= 100;
	//	}
	//}

	int	dIndex = pOrgData->GetIndex();
	if (dIndex >= GI_MINUTE || bOrderMode)
	{
		CPoint	pt;
		if (m_dYMin <= sahg && sahg <= m_dYMax)
		{
			GetValueToYPoint(pt, sahg);

			CPen	*cPen = GetPen(PS_SOLID, 1, m_crUpSise);//RGB(255,0,0));
			CPen	*sPen = pDC->SelectObject(cPen);

			pDC->MoveTo(m_rectOrgGrp.left, pt.y);
			pDC->LineTo(m_rectOrgGrp.right, pt.y);

			pDC->SelectObject(sPen);
		}

		if (m_dYMin <= hahg && hahg <= m_dYMax)
		{
			GetValueToYPoint(pt, hahg);

			CPen	*cPen = GetPen(PS_SOLID, 1, m_crDnSise);//RGB(0,0,255));
			CPen	*sPen = pDC->SelectObject(cPen);

			pDC->MoveTo(m_rectOrgGrp.left, pt.y);
			pDC->LineTo(m_rectOrgGrp.right, pt.y);

			pDC->SelectObject(sPen);
		}
	}

	if (PCurr < m_dYMax)
	{
		double	upVal = (sahg < m_dYMax)? sahg:m_dYMax;
		double	dnVal = (PCurr > m_dYMin)? PCurr:m_dYMin;

		CPoint	upPt, dnPt;
		GetValueToYPoint(upPt, upVal);
		GetValueToYPoint(dnPt, dnVal);

		pDC->FillSolidRect(CRect(m_rectOrgGrp.right-3, upPt.y, m_rectOrgGrp.right, dnPt.y), m_crUpSise);//RGB(255,0,0));
	}
	if (PCurr > m_dYMin)
	{
		double	dnVal = (hahg > m_dYMin)? hahg:m_dYMin;
		double	upVal = (PCurr < m_dYMax)? PCurr:m_dYMax;
		
		CPoint	upPt, dnPt;
		GetValueToYPoint(upPt, upVal);
		GetValueToYPoint(dnPt, dnVal);

		pDC->FillSolidRect(CRect(m_rectOrgGrp.right-3, upPt.y, m_rectOrgGrp.right, dnPt.y), m_crDnSise);//RGB(0,0,255));
	}
}

// in Fixed 
int CIndcBase::DrawTickIndex(CDC* pDC, CString dispText, bool bLeft)
{
	if (dispText.IsEmpty())
		return 0;

	int sMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF sColor = pDC->SetTextColor(m_crText);

	CFont	*gFont = GetFont(TICK_BOTTOM);
	CFont	*sFont = pDC->SelectObject(gFont);

	CSize	size = pDC->GetOutputTextExtent(dispText);
	if (m_rectOrgGrp.Width() < size.cx + 5)
	{
		pDC->SelectObject(sFont);
		pDC->SetTextColor(sColor);
		pDC->SetBkMode(sMode);
		return 1;
	}

	CRect	dRect;
	if (bLeft)
	{
		dRect.left = m_rectOrgGrp.left + 5;
		dRect.top = m_rectOrgGrp.top + 3;
		dRect.right = dRect.left + size.cx;
		dRect.bottom = dRect.top + size.cy;
		pDC->DrawText(dispText, &dRect, DT_SINGLELINE|DT_TOP|DT_LEFT);
	}
	else
	{
		dRect.right = m_rectOrgGrp.right - int(m_TkInfo.tkRect[TICK_BOTTOM].Width());
		dRect.left  = dRect.right - size.cx;
		dRect.top   = m_rectOrgGrp.top + 3;
		dRect.bottom= dRect.top + size.cy;
		pDC->DrawText(dispText, &dRect, DT_SINGLELINE|DT_TOP|DT_RIGHT);
	}

	if (m_RgnInfo.Rgn[m_RgnInfo.iCount].pRect)
	{
		delete m_RgnInfo.Rgn[m_RgnInfo.iCount].pRect;
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRect = NULL;
	}
	
	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRect = new CRect;
	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRect->left = dRect.left;
	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRect->top = dRect.top;
	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRect->right = dRect.right;
	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRect->bottom = dRect.bottom;
	m_RgnInfo.iCount++;

	pDC->SelectObject(sFont);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);

	if (m_RgnInfo.iSelect >= 0)
	{
		int	sROP = pDC->SetROP2(R2_XORPEN);
		CPen	*cPen = GetPen(PS_SOLID, 1, _L_WHITE);
		CPen	*sPen = pDC->SelectObject(cPen);
		
		dRect.InflateRect(0, 1);
		pDC->MoveTo(dRect.left, dRect.top);
		pDC->LineTo(dRect.right, dRect.top);
		pDC->LineTo(dRect.right, dRect.bottom);
		pDC->LineTo(dRect.left, dRect.bottom);
		pDC->LineTo(dRect.left, dRect.top);

		pDC->SetROP2(sROP);
		pDC->SelectObject(sPen);
	}

	return 1;
}

int CIndcBase::DrawTickIndex(CDC* pDC, CString strDispText, COLORREF rgbText, bool bColor, bool bIndex, CPoint point)
{
	if (strDispText.IsEmpty())
		return 0;

	int iBkModeOld = pDC->SetBkMode(TRANSPARENT);
	COLORREF rgbTextOld;
	if (bColor)
		rgbTextOld = pDC->SetTextColor(rgbText);
	else
		rgbTextOld = pDC->SetTextColor(m_crText);

	CFont* pfontBottom = GetFont(TICK_BOTTOM);
	CFont* pfontOld = pDC->SelectObject(pfontBottom);

	CString	strRealSize = strDispText;
	CSize size = pDC->GetOutputTextExtent(strRealSize);
	CSize size2 = pDC->GetOutputTextExtent("D");
	size.cx = size.cx + size2.cx/2;
	size2.cx = size2.cx + size2.cx/2;

	if (bIndex)
	{
		if (point.x < m_rectOrgGrp.left)
			point.x = m_rectOrgGrp.left;
		else if ((point.x + size.cx) > m_rectOrgGrp.right)
			point.x = m_rectOrgGrp.right - size.cx;
		if (point.y < m_rectOrgGrp.top)
			point.y = m_rectOrgGrp.top;
		else if ((point.y + size.cy) > m_rectOrgGrp.bottom)
			point.y = m_rectOrgGrp.bottom - size.cy;

		CRect rectDisp;
		rectDisp.SetRect(point.x, point.y, point.x + size.cx, point.y + size.cy);
		pDC->DrawText(strDispText, &rectDisp, DT_SINGLELINE|DT_TOP|DT_LEFT);

		pDC->SelectObject(pfontOld);
		pDC->SetTextColor(rgbTextOld);
		pDC->SetBkMode(iBkModeOld);
		return point.x + size.cx;
	}

	if (m_pCoDraw->tickx == 0)	// 처음
	{
		point.x = m_rectOrgGrp.left + 5;
		point.y = m_rectOrgGrp.top + 3;
	}
	else
	{
		if (m_pCoDraw->tickx + size.cx > m_rectOrgGrp.right)	// 오른쪽 끝을 넘으면 한줄 밑으로
		{
			point.x = m_rectOrgGrp.left + 5;
			point.y = m_pCoDraw->ticky + int(m_TkInfo.tkHeight[TICK_BOTTOM]);
		}
		else
		{
			point.x = m_pCoDraw->tickx;
			point.y = m_pCoDraw->ticky;
		}
	}

	if (point.y + int(m_TkInfo.tkHeight[TICK_BOTTOM]) > m_rectOrgGrp.bottom)	// 하단을 넘으면 리턴
	{
		pDC->SelectObject(pfontOld);
		pDC->SetTextColor(rgbTextOld);
		pDC->SetBkMode(iBkModeOld);

		return 0;
	}

	CRect rectDisp;
	CRect rectLegend;
	rectDisp.SetRect(point.x, point.y, point.x + size.cx, point.y + size.cy);
	rectDisp.NormalizeRect();
	pDC->DrawText(strDispText, &rectDisp, DT_SINGLELINE|DT_TOP|DT_LEFT);
	rectLegend = rectDisp;

	pDC->SelectObject(pfontOld);
	pDC->SetTextColor(rgbTextOld);
	pDC->SetBkMode(iBkModeOld);

	m_pCoDraw->tickx = point.x + size.cx;
	m_pCoDraw->ticky = point.y;

	if (m_RgnInfo.Rgn[m_RgnInfo.iCount].pRect)
	{
		delete m_RgnInfo.Rgn[m_RgnInfo.iCount].pRect;
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRect = NULL;
	}
	
	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRect = new CRect;
	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRect->left = rectLegend.left;
	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRect->top = rectLegend.top;
	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRect->right = rectLegend.right;
	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRect->bottom = rectLegend.bottom;

	if (m_RgnInfo.iSelect >= 0)
	{
		int iRopOld = pDC->SetROP2(R2_XORPEN);
		CPen* ppenWhite = GetPen(PS_SOLID, 1, _L_WHITE);
		CPen* ppenOld = pDC->SelectObject(ppenWhite);
		
		rectLegend.InflateRect(0, 1);
		pDC->MoveTo(rectLegend.left, rectLegend.top);
		pDC->LineTo(rectLegend.right, rectLegend.top);
		pDC->LineTo(rectLegend.right, rectLegend.bottom);
		pDC->LineTo(rectLegend.left, rectLegend.bottom);
		pDC->LineTo(rectLegend.left, rectLegend.top);

		pDC->SetROP2(iRopOld);
		pDC->SelectObject(ppenOld);
	}

	return point.x + size.cx;
}

void CIndcBase::DrawTickVertical(CDC *pDC)
{
}

CString CIndcBase::GetTickUnit(double Max)
{
	CString dispStr = _T("");
	int graphKind = m_pgraph->wGKind;

	switch (m_iUnit)
	{
	case GU_CODE:
		switch (graphKind)
		{
		case GK_AMT:	case GK_AMA:
			if (Max > dispUNIT)
				dispStr = _T("[10억]");
			else
				dispStr = _T("[백만]");
			break;
		case GK_VOL:	case GK_VMA:
		case GK_OBV:
			if (Max > dispMUNIT)
				dispStr = _T("[백만]");
			else if (Max > dispUNIT)
				dispStr = _T("[x1000]");
			else
				dispStr = _T("[ 주 ]");
			break;
		default:
			dispStr = _T("");
/*			if (m_iValidDigit > 0)
				dispStr = _T("[달러]");
			else
				dispStr = _T("[ 원 ]");
*/			break;
		}
		break;
	case GU_COFUT:	case GU_COOPT:
		switch (graphKind)
		{
		case GK_AMT:
		case GK_AMA:
			if (Max > dispUNIT)
				dispStr = _T("[백만]");
			else
				dispStr = _T("[x1000]");
			break;
		case GK_VOL:
		case GK_VMA:
		case GK_OBV:
			if (Max > dispMUNIT)
				dispStr = _T("[백만]");
			else if (Max > dispUNIT)
				dispStr = _T("[x1000]");
			else
				dispStr = _T("[계약]");
			break;
		default:
			dispStr = _T("");
//			dispStr = _T("[ 원 ]");
			break;
		}
		break;
	case GU_INDEX:
		switch (graphKind)
		{
		case GK_AMT:
		case GK_AMA:
			if (Max > dispUNIT)
				dispStr = _T("[10억]");
			else
				dispStr = _T("[백만]");
			break;
		case GK_VOL:
		case GK_VMA:
		case GK_OBV:
			if (Max > dispUNIT)
				dispStr = _T("[백만]");
			else
				dispStr = _T("[x1000]");
			break;
		default:
			dispStr = _T("");
//			dispStr = _T("[지수]");
			break;
		}
		break;
	case GU_FUTURE:
		switch (graphKind)
		{
		case GK_AMT:
		case GK_AMA:
			if (Max > dispUNIT)
				dispStr = _T("[10억]");
			else
				dispStr = _T("[백만]");
			break;
		case GK_VOL:
		case GK_VMA:
		case GK_OBV:
			if (Max > dispUNIT)
				dispStr = _T("[x1000]");
			else
				dispStr = _T("[계약]");
			break;
		default:
			dispStr = _T("");
//			dispStr = _T("[지수]");
			break;
		}
		break;
	case GU_OPTION:
		switch (graphKind)
		{
		case GK_AMT:
		case GK_AMA:
			if (Max > dispUNIT)
				dispStr = _T("[10억]");
			else
				dispStr = _T("[백만]");
			break;
		case GK_VOL:
		case GK_VMA:
		case GK_OBV:
			if (Max > dispUNIT)
				dispStr = _T("[x1000]");
			else	
				dispStr = _T("[계약]");
			break;
		default:
			dispStr = _T("");
//			dispStr = _T("[지수]");
			break;
		}
		break;
	//case GU_CFUTURE: // 20090917 : ADD
	//	switch (graphKind)
	//	{
	//	case GK_AMT:
	//	case GK_AMA:
	//		if (Max > dispUNIT)
	//			dispStr = _T("[10억]");
	//		else
	//			dispStr = _T("[백만]");
	//		break;
	//	case GK_VOL:
	//	case GK_VMA:
	//	case GK_OBV:
	//		if (Max > dispUNIT)
	//			dispStr = _T("[x1000]");
	//		else	
	//			dispStr = _T("[계약]");
	//		break;
	//	default:
	//		dispStr = _T("");
//			dispStr = _T("[지수]");
	//		break;
	//	}
	//	break;
	case GU_FOREIGN:
		switch (graphKind)
		{
		case GK_AMT:
		case GK_AMA:
			if (Max > dispUNIT)
				dispStr = _T("[10억]");
			else
				dispStr = _T("[백만]");
			break;
		case GK_VOL:
		case GK_VMA:
		case GK_OBV:
			if (Max > dispMUNIT)
				dispStr = _T("[백만]");
			else if (Max > dispUNIT)
				dispStr = _T("[x1000]");
			else
				dispStr = _T("");
			break;
		default:
			dispStr = _T("");
//			dispStr = _T("[달러]");
			break;
		}
		break;
	}

	return dispStr;
}

int CIndcBase::GetDataIndex()
{
	class COrgData *pOrgData;
	if (m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
	{
		return pOrgData->GetIndex();
	}

	return GI_DAY;
}

int CIndcBase::GetGraphID()
{
	return m_pgraph->wKey;
}

int CIndcBase::GetGraphType()
{
	return m_pgraph->btGType;
}

int CIndcBase::GetGraphRegionID()
{
	return m_pgraph->btRgnKey;
}

bool CIndcBase::SetDisplay(int iDisDay)
{
	m_iDispDtCnt = iDisDay;
	m_iDispSPos = m_iDispEPos - iDisDay;

	if (m_iDispSPos < 0)
	{
		int gap = 0 - m_iDispSPos;
		m_iDispSPos += gap;
		m_iDispEPos += gap;
	}

	CalculateMinMax();

	return true;
}

int CIndcBase::SetDisplayScale(int& scaleUpDn)
{
	if (scaleUpDn > 0)	// dispDay 증가, scale 축소
	{
		if (m_iDispDtCnt >= m_iTtlDtCnt)
			return 0;

		if (m_iDispSPos == 0)
		{
			scaleUpDn = (m_iTtlDtCnt - m_iDispDtCnt >= scaleUpDn) ?	scaleUpDn : m_iTtlDtCnt - m_iDispDtCnt;
			m_iDispDtCnt += scaleUpDn;
			m_iDispEPos += scaleUpDn;
			CalculateMinMax();

			return m_iDispDtCnt;
		}
		else if (m_iDispSPos - abs(scaleUpDn) < 0)
		{
			scaleUpDn = m_iDispSPos;
		}
	}
	else if (scaleUpDn < 0)	// dispDay 감소, scale 확대
	{
		if (m_iDispDtCnt <= minDisplayDay)
			return 0;
		if ((m_iDispDtCnt - abs(scaleUpDn)) <= minDisplayDay)
			scaleUpDn = minDisplayDay - m_iDispDtCnt;

		if (scaleUpDn == 0)
			return 0;
	}
	else
		return 0;

	m_iDispDtCnt += scaleUpDn;
	m_iDispSPos -= scaleUpDn;
	CalculateMinMax();

	return m_iDispDtCnt;
}

bool CIndcBase::SetDisplayShift(int& shiftUpDn)
{
	if (shiftUpDn > 0)	// right scroll
	{
		if (m_iDispEPos >= m_iTtlDtCnt)
			return false;
		if ((m_iDispEPos + shiftUpDn) > m_iTtlDtCnt)
			shiftUpDn = m_iTtlDtCnt - m_iDispEPos;
	}
	else if (shiftUpDn < 0)	// left scroll
	{
		if (m_iDispSPos <= 0)
			return false;
		if ((m_iDispSPos - abs(shiftUpDn)) <  0)
			shiftUpDn = m_iDispSPos * (-1);
	}

	if (shiftUpDn == 0)
		return false;

	m_iDispEPos += shiftUpDn;
	m_iDispSPos += shiftUpDn;
	CalculateMinMax();

	return true;
}

void CIndcBase::ChangeDisplayPos(int iDispPos, int iDispEnd)
{
	m_iDispSPos = iDispPos;
	m_iDispEPos = iDispEnd;
	CalculateMinMax();
}

void CIndcBase::SetGrpInfo(struct _graph *pGrp, struct _envInfo *pInfo, struct _CoDraw *pCoDraw)
{
	bool bChangeVar = false;
	if (pGrp)
	{
		CopyMemory(m_pgraph, pGrp, SZ_GRAPH);
		m_crText = m_pgraph->rgbLabel;

		m_iUTickNum = 0;
		DWORD	chkbit = 0x00000001;
		for (int ii = 0; ii < MAX_LINE; ii++, chkbit <<= 1)
		{
			if (m_pgraph->tInfo.wCOption & chkbit)
				m_iUTickNum++;
		}

		bChangeVar = true;
	}

	bool bChangeOption = false;
	if (pInfo)
	{
		m_wEnvOpt = pInfo->wEnvOption;
		m_dwDspOpt = pInfo->display.dwDspOption;
		m_crBG = pInfo->display.rgbChartBG;
		m_crFG = pInfo->display.rgbChartEdge;
		CopyMemory(&m_lineH, &pInfo->display.hline, SZ_LINE);
		CopyMemory(&m_lineV, &pInfo->display.vline, SZ_LINE);	
		CopyMemory(&m_lineDate, &pInfo->display.dline, SZ_LINE);	
		CopyMemory(m_atick, pInfo->display.atick, sizeof(pInfo->display.atick));

		CalculateTickInfo();
		bChangeOption = true;
	}

	if (bChangeVar || bChangeOption)
	{
		class CAppData *pAppData;
		if (m_pDataMgr->GetData(m_iDataKey, pAppData))
		{
			pAppData->SetVariable(&m_pgraph->cInfo, m_pgraph->wCalOption);
			CalculateMinMax(true);
		}
		else
			CalculateMinMax(true);
	}

	if (pCoDraw)
		m_pCoDraw = pCoDraw;
}

void CIndcBase::SetGraphRegionID(int id)
{
	m_pgraph->btRgnKey = id;
}

//CString CIndcBase::GetExtraInfo(unsigned char check, int val, bool bGrid)
CString CIndcBase::GetExtraInfo(DWORD dwCheck, int val, bool bGrid)
{
	CString	dispText = _T("");

	if (dwCheck <= 0)
		return dispText;

	if (bGrid)
	{
		if (dwCheck & CBITS_KR)
		{
			dispText = _T("권리락");
		}
		if (dwCheck & CBITS_BR)
		{
			if (dispText.IsEmpty())
				dispText = _T("배당락");
			else
				dispText += _T(",배당락");
		}
		if (dwCheck & CBITS_KJ)
		{
			if (dispText.IsEmpty())
				dispText = _T("감자");
			else
				dispText += _T(",감자");
		}
		if (dwCheck & CBITS_DI)
		{
			if (dispText.IsEmpty())
				dispText = _T("액분");
			else
				dispText += _T(",액분");
		}
		if (dwCheck & CBITS_YP)
		{
			if (dispText.IsEmpty())
				dispText = _T("액병");
			else
				dispText += _T(",액병");
		}
		if (dwCheck & CBITS_KS)
		{
			if (dispText.IsEmpty())
				dispText = _T("공시");
			else
				dispText += _T(",공시");
		}
	}
	else
	{
		if (dwCheck & CBITS_KR)
		{
			dispText = _T("권");
		}
		if (dwCheck & CBITS_BR)
		{
			if (dispText.IsEmpty())
				dispText = _T("배");
			else
				dispText += _T(",배");
		}
		if (dwCheck & CBITS_KJ)
		{
			if (dispText.IsEmpty())
				dispText = _T("감");
			else
				dispText += _T(",감");
		}
		if (dwCheck & CBITS_DI)
		{
			if (dispText.IsEmpty())
				dispText = _T("액분");
			else
				dispText += _T(",액분");
		}
		if (dwCheck & CBITS_YP)
		{
			if (dispText.IsEmpty())
				dispText = _T("액병");
			else
				dispText += _T(",액병");
		}
		if (dwCheck & CBITS_KS)
		{
			if (dispText.IsEmpty())
				dispText = _T("공시");
			else
				dispText += _T(",공시");
		}
	}

	CString	dispVal;
	dispVal.Format("%d%%", val);
	if (bGrid)
		dispText = _T("\t&&") + dispText + _T(":") + dispVal;
	else
		dispText = dispText + _T(":") + dispVal;

	return dispText;
}

//void CIndcBase::DisplayCheckBits(CDC* pDC, CPoint point, unsigned char check, int val)
void CIndcBase::DisplayCheckBits(CDC* pDC, CPoint point, DWORD dwCheck, int val)
{
	if (!(m_dwDspOpt & DO_LOCK))
		return;

	CString	dispText = GetExtraInfo(dwCheck, val);
	if (dispText.IsEmpty())
		return;

	CFont	*gFont = GetFont(TICK_BOTTOM);
	CFont	*sFont = pDC->SelectObject(gFont);

	int sMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF sColor = pDC->SetTextColor(_L_MAGENTA);

	CSize	sz = pDC->GetOutputTextExtent(dispText);
	CPoint	dispPoint;

	dispPoint.x = point.x - sz.cx/2;
	dispPoint.y = point.y - sz.cy;

	if (dispPoint.x < m_rectOrgGrp.left)
		dispPoint.x = m_rectOrgGrp.left + 2;
	if (dispPoint.x + sz.cx > m_rectOrgGrp.right)
		dispPoint.x = m_rectOrgGrp.right - sz.cx;
	if (dispPoint.y < m_rectGrp.top)
		dispPoint.y = m_rectGrp.top + 2;

	pDC->TextOut(dispPoint.x, dispPoint.y, dispText);

	pDC->SelectObject(sFont);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
}

void CIndcBase::ReviseMinMax(double& Max, double& Min)
{
	if ((m_wEnvOpt & EO_MAP) && !(m_dwDspOpt & DO_MINMAX))
		return;

	int	graphKind = GetGraphKind();
	switch (graphKind)
	{
	case GK_VOL:
	case GK_AMT:	
	case GK_VMA:
	case GK_AMA:
	case GK_EQVV:
	case GK_CDVV:
		Min = 0;
		break;
	default:
		break;
	}

	double	dMax = Max;
	double	dMin = Min;
	
	if ((Max == DBL_MIN || Min == DBL_MAX) || (Min == 0 && Max == 0))
		return;

	double gap = fabs(dMax - dMin);
	if (gap == 0)
	{
		if (dMax > 100)
		{
			dMax += dMax / 20;
			dMin -= dMin / 20;
		}
		else
		{
			dMax += 5;
			dMin -= 5;
		}
	}
	else
	{
		if (IsLogChart())
			dMin -= min(int(dMin * 0.05), int(gap * 0.05));
		else
			dMin -= int(gap * 0.05);

		dMax += int(gap * 0.05);

		// 2007.01.03 schbang : 로그차트 아래공백 제거를 위해
		if (IsLogChart() && dMin < 0)
		{
			dMin = Min - (max(5, Min * 0.1));
		}
	}

	int	index;
	CString	temp;
	temp.Format("%f", dMax + 1);
	index = temp.Find('.'); 
	dMax = atof(temp.Left(index));
	temp.Format("%f", dMin - 1);
	index = temp.Find('.'); 
	dMin = atof(temp.Left(index));

	Max = dMax;
	Min = dMin;

	switch (graphKind)
	{
	case GK_VOL:
	case GK_AMT:	
	case GK_VMA:
	case GK_AMA:
	case GK_EQVV:
	case GK_CDVV:
		Min = 0;
		break;
	default:
		break;
	}

	// 2007.01.02 schbang : 로그차트시 min이 음수가 들어와서 공백이 크게 보이는 현상
	if (IsLogChart() && Min < 0)
	{
		Min = 0;
	}
}

int CIndcBase::GetTickCount(int dir)
{
	int	dispTICK = 5;
	int	tkHeight;

	switch (dir)
	{
	case 1:
	case 2:
		tkHeight = m_TkInfo.tkRect[dir].Height();
		dispTICK = int(tkHeight / (m_TkInfo.tkHeight[dir] * 1.1));
		break;
	case 0:
		if (m_TkInfo.tkHeight[TICK_LEFT] > 0)
		{
			tkHeight = m_TkInfo.tkRect[TICK_LEFT].Height();
			dispTICK = int(tkHeight / (m_TkInfo.tkHeight[TICK_LEFT] * 1.1));
		}
		else if (m_TkInfo.tkHeight[TICK_RIGHT] > 0)
		{			
			tkHeight = m_TkInfo.tkRect[TICK_RIGHT].Height();
			dispTICK = int(tkHeight / (m_TkInfo.tkHeight[TICK_RIGHT] * 1.1));
		}
		else
			dispTICK = 5;
		break;
	default:
		dispTICK = 5;
		break;
	}

	if (dispTICK > 15)
		dispTICK = 15;

	return dispTICK;
}

void CIndcBase::FormatNormalComma(CString &dispTxt, double dVal, bool bComma, int idigit)
{
	char Str[32], Str2[32];
	int Val = int(dVal);

	CString	Minus = _T("");
	if (Val < 0)
	{
		Minus = "-";
		Val = -Val;
	}

	int Digit;
	if (idigit >= 0)
		Digit = idigit;
	else
		Digit = m_iValidDigit;

	if (Digit < 0 || Digit > 10)
		Digit = 0;

	int Divide = int( pow(10.0, double(Digit)) );
	int Mod = Val % Divide;

	for (int ii = 0; ii < Digit; ii++)
	{
		Str2[ii] = Mod % 10 + 0x30;
		Mod = Mod / 10;
	}
	
	Str2[Digit] = 0x00;
	CString PointStr = Str2;
	PointStr.MakeReverse();
	
	CString tmp, IntegerStr;
	tmp.Format("%d", Val / Divide);

	if (bComma)
	{
		int idx = 0, jj = 1;
		for (int ii = tmp.GetLength() - 1; ii >= 0 ;ii--, jj++)
		{
			Str[idx++] = tmp.GetAt(ii);		

			if (jj % 3 == 0 && ii > 0)
				Str[idx++] = ',';
		}

		Str[idx] = 0x00;
		IntegerStr = Str + Minus;
		IntegerStr.MakeReverse();
	}
	else
	{
		IntegerStr = Minus + tmp;
	}

	if (Digit > 0)
		dispTxt = IntegerStr + "." + PointStr; 
	else
		dispTxt = IntegerStr;
}

void CIndcBase::FormatVariableComma(CString &dispTxt, double dVal, double Max, int maxDigit)
{
	CString	edit1, edit2;
	double	dotL = 0;
	int	dotR = 0;

	int graphKind = m_pgraph->wGKind;

	bool bSign = false;
	if (dVal < 0)
	{
		bSign = true;
		dVal = dVal * (-1);
	}
	switch (m_iValidDigit)
	{
	case 2:
		dotL = int(dVal / 100.0);
		dotR = int(dVal - dotL*100.0 + 0.5);
		if (dotR == 100)
		{
			dotL++;
			dotR = 00;
		}
		break;
	case 4:
		dotL = int(dVal / 10000.0);
		dotR = int(dVal - dotL*10000.0 + 0.5);
		break;
	default:
		switch (graphKind)
		{
		case GK_EQVV:
		case GK_CDVV:
		case GK_VOL:
		case GK_AMT:
		case GK_VMA:
		case GK_AMA:

		/** remarking by macho@2006.12.12 <begin>
		//** 2005.11.08 add & modify by mach0
		case GK_FRGNVOL:
		case GK_ORGNVOL:
			dotL = dVal;
			if (Max > dispMUNIT)
				dotL = int(dotL / 1000000.0);
			else if (Max > dispUNIT)
				dotL = int(dotL / 1000.0);
			break;
		case GK_FRGNAMT:
		case GK_ORGNAMT:
		//** 
		remarking by macho@2006.12.12 <end> **/
		case GK_FRGNHAVE:case GK_PSNLPBUY: case GK_FRGNPBUY: case GK_ORGNPBUY:
		case GK_PSNLTBUY:case GK_FRGNTBUY: case GK_ORGNTBUY:
			dotL = dVal;
			if (Max > dispMUNIT)
				dotL = int(dotL / 1000000.0);
			else if (Max > dispUNIT)
				dotL = int(dotL / 1000.0);
			break;
		default:
			dotL = dVal;
			break;
		}
		break;
	}

	dispTxt = _T("");
	edit1.Format("%.f", dotL);
	edit1.MakeReverse();
	for (int ii = 0; ii < edit1.GetLength(); ii++)
	{
		if (ii && !(ii % 3))
			edit2 += ',';
		edit2 += edit1[ii];
	}
	if (bSign)
		edit2 += "-";
	edit2.MakeReverse();

	edit1= _T("");
	switch (m_iValidDigit)
	{
	case 2:
		edit1.Format(".%02d", dotR);
		break;
	case 4:
		edit1.Format(".%04d", dotR);
		break;
	default:
		break;
	}

	/**  주식외에도 , 를 찍어달라는 요구에 의해 아래 루틴 제거.
	int trimComma = edit2.GetLength() + edit1.GetLength() - maxDigit;
	if (trimComma > 0)
	{
		int trim = 0;
		for (int ii = 0; ii < edit2.GetLength(); ii++)
		{
			if (edit2[ii] == ',' && trim < trimComma)
			{
				trim++;
				continue;
			}
			dispTxt += edit2[ii];
		}
		dispTxt += edit1;
	}
	else
	{**/
		dispTxt = edit2 + edit1;
	//}
}

CString CIndcBase::GetIndexStrTool(int index, int &fpr, int &hpr, int &lpr, int &epr)
{
	if (index < 0)
		return _T("");

	if (index >= m_iTtlDtCnt)
		index = m_iTtlDtCnt - 1;
	
	class COrgData *pOrgData;
	if (!m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
		return _T("");

	CGrpBasic* gBasic;
	gBasic = pOrgData->GetGraphData(index);

	fpr = gBasic->m_iOpen;
	hpr = gBasic->m_iHigh;
	lpr = gBasic->m_iLow;
	epr = gBasic->m_iClose;

	return _T("");
}

CString CIndcBase::GetIndexValueStr(CPoint pt, int &fpr, int &hpr, int &lpr, int &epr)
{
	CString	IndexStr = _T("");

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
		return IndexStr;

	CRect	DrawRect = m_rectGrp;
	if (m_iGrpKind == GK_BALANCE)
	{
		int	BalanceGap = GetValue();
		if (BalanceGap > 0)
			DrawRect.right -= int(double(m_rectGrp.Width()*BalanceGap) / double(m_iDispDtCnt + BalanceGap));
	}

	int DrawWidth = DrawRect.Width();
	int xPosition = pt.x - DrawRect.left;

	double szOneDay = double(DrawWidth) / double(m_iDispDtCnt);
	int dataPos = int(double(xPosition) / szOneDay);
	dataPos += m_iDispSPos;
	if (dataPos < 0)
		return _T("");
	if (dataPos >= m_iTtlDtCnt)
		dataPos = m_iTtlDtCnt - 1;

	CString	unitStr = _T("");
	CGrpBasic* gBasic;
	gBasic = pOrgData->GetGraphData(dataPos);

	switch (GetDataIndex())
	{
	case GI_MINUTE:
	case GI_TICK:
		unitStr.Format("t%04d%02d%02d%02d%02d%02d", 
			gBasic->m_index.date.yy, gBasic->m_index.date.mm, gBasic->m_index.date.dd,
			gBasic->m_index.time.hh, gBasic->m_index.time.mm, gBasic->m_index.time.ss);
		break;
	default:
		unitStr.Format("d%04d%02d%02d", 
			gBasic->m_index.date.yy, gBasic->m_index.date.mm, gBasic->m_index.date.dd);
		break;
	}

	fpr = gBasic->m_iOpen;
	hpr = gBasic->m_iHigh;
	lpr = gBasic->m_iLow;
	epr = gBasic->m_iClose;

	return unitStr;
}

bool CIndcBase::IsPointInGrpRect(CPoint pt)
{
	if (m_rectOrgGrp.PtInRect(pt))
		return true;
	return false;
}

int CIndcBase::IsPointInGrpRgn(CPoint pt, bool &bLabel, bool bSet)
{
	// 합성된건 선택 안되게
	if (m_pgraph->dwDraOption & GO_UNION)
		return false;

	BOOL	bInRgn = 0;
	if (bSet)
		m_RgnInfo.iSelect = -1;

	bLabel = false;
	for (int ii = 0; ii < m_RgnInfo.iCount; ii++)
	{
		if (!m_RgnInfo.Rgn[ii].pRgn || m_RgnInfo.Rgn[ii].pRgn->m_hObject == NULL)
			break;


		if ((bInRgn = m_RgnInfo.Rgn[ii].pRgn->PtInRegion(pt)))
		{
			if (bSet)
				m_RgnInfo.iSelect = ii;
			break;
		}
	}

	if (bInRgn)
		return true;

	bLabel = true;
	for (int ii = 0; ii < m_RgnInfo.iCount; ii++)
	{
		if (!m_RgnInfo.Rgn[ii].pRect)
			break;;

		if ((bInRgn = m_RgnInfo.Rgn[ii].pRect->PtInRect(pt)))
		{
			if (bSet)
				m_RgnInfo.iSelect = ii;
			break;
		}
	}

	return (bInRgn ? true : false);
}

bool CIndcBase::IsRgnSelect()
{
	if (m_RgnInfo.iCount <= 0)
		return false;

	if (m_RgnInfo.iSelect>= 0)
		return true;
	else
		return false;
}

void CIndcBase::ResetSelRgn()
{
	m_RgnInfo.iSelect = -1;
}

void CIndcBase::GetPtToEndvalPt(CPoint pt, CPoint &ept)
{
	ept = CPoint(pt.x, pt.y+1);
}

void CIndcBase::GetValueToStr(double value, CString &str)
{
	double	Max = m_dYMax;
	double	Min = m_dYMin;
	if (IsLogChart())
	{
		Max = GetLogData(Max);
		Min = GetLogData(Min);
	}

	FormatVariableComma(str, value, Max, 9);
}

void CIndcBase::GetPtToPosValue(CPoint pt, double& dPos, double& dValue)
{
//	int	graphKind = GetGraphKind();
//	switch (graphKind)
//	{
//	case GK_RENKO:
//		if (!(m_pgraph->dwDraOption & GO_UNION))
//		{
//			VGetPtToPosValue(pt, dPos, dValue);
//			return;
//		}
//		break;
//	default:
//		break;
//	}

	
	
	double dMax = m_dYMax;
	double dMin = m_dYMin;
/*	CString	sVal;
	FormatVariableComma(sVal, dMax, dMax, 1);
	dMax = atof(sVal);
	FormatVariableComma(sVal, dMin, dMax, 1);
	dMin = atof(sVal);
*/
	int iDispCount = GetDisplayEnd() - GetDisplayPos();
	CRect rectDraw = m_rectGrp;
	if (m_iGrpKind == GK_BALANCE)
	{
		int iBalanceGap = GetValue();
		if (iBalanceGap > 0)
			rectDraw.right -= int(double(m_rectGrp.Width()*iBalanceGap) / double(m_iDispDtCnt + iBalanceGap));
	}
	
	dPos = double((pt.x - rectDraw.left) * iDispCount) / double(rectDraw.Width());
	dPos += GetDisplayPos();

	int iDrawHeight = rectDraw.Height();

	if (IsLogChart())
	{
		dMax = GetLogData(dMax);
		dMin = GetLogData(dMin);
	}

	double dValueHeight = dMax - dMin;
	double dPosValue = double(dValueHeight) / double(iDrawHeight);
	dPosValue *= double(rectDraw.bottom - double(pt.y));
	
	if (IsReverseChart())
		dPosValue = dMax - dPosValue;
	else
		dPosValue = dMin + dPosValue;

	if (IsLogChart())
		dPosValue = exp(dPosValue / 1000.);

	dValue = dPosValue;
}

void CIndcBase::GetValueToPoint(CPoint& pt, double dXValue, double dYValue)
{
	pt = CPoint(-1, -1);
//
//	int	graphKind = GetGraphKind();
//	switch (graphKind)
//	{
//	case GK_RENKO:
//		if (!(m_pgraph->dwDraOption & GO_UNION))
//		{
//			VGetValueToPoint(pt, dXValue, dYValue);
//			return;
//		}
//		break;
//	default:
//		break;
//	}


	double dMax = m_dYMax;
	double dMin = m_dYMin;
/*	CString	sVal;
	FormatVariableComma(sVal, dMax, dMax, 1);
	dMax = atof(sVal);
	FormatVariableComma(sVal, dMin, dMax, 1);
	dMin = atof(sVal);
*/
	if (IsLogChart())
	{
		dMax = GetLogData(dMax);
		dMin = GetLogData(dMin);
		dYValue = GetLogData(dYValue);
	}

	CRect rectDraw = m_rectGrp;
	if (m_iGrpKind == GK_BALANCE)
	{
		int iBalanceGap = GetValue();
		if (iBalanceGap > 0)
			rectDraw.right -= int(double(m_rectGrp.Width()*iBalanceGap) / double(m_iDispDtCnt + iBalanceGap));
	}

	double dGap = m_iDispEPos - m_iDispSPos;
	if (dGap == 0)
		dGap = 1;

	double dX = (dXValue - m_iDispSPos) * rectDraw.Width() / dGap;

	dGap = dMax - dMin;
	if (dGap == 0)
		dGap = 1;	

	double dY;
	if (IsReverseChart())
		dY = rectDraw.top + (dYValue - dMin) * rectDraw.Height() / dGap;
	else
		dY = rectDraw.bottom - (dYValue - dMin) * rectDraw.Height() / dGap;		

	pt.x = int(dX + rectDraw.left);
	pt.y = int(dY);
}

void CIndcBase::GetValueToYPoint(CPoint& pt, double yValue)
{
	pt = CPoint(-1, -1);

	double  Min, Max, y;
	GetMinMaxValue(Min, Max);

	if (IsLogChart())
	{
		Max = GetLogData(Max);
		Min = GetLogData(Min);
		yValue = GetLogData(yValue);
	}

	double	gap = Max - Min;
	if (gap == 0)
		gap = 1;
	bool	bReverse = IsReverseChart();
	if (!bReverse)
		y = m_rectGrp.bottom - (yValue - Min) * m_rectGrp.Height() / gap;
	else
		y = m_rectGrp.top + (yValue - Min) * m_rectGrp.Height() / gap;

	pt.y = int(y);
}

void CIndcBase::EditMemberValue()
{
	m_iDispSPos = m_iTtlDtCnt - m_iDispDtCnt;
	m_iDispEPos = m_iDispSPos + m_iDispDtCnt;
}

void CIndcBase::CalculateTickInfo()
{
	CClientDC	cltDC(m_pwndParent);

	for (int ii = 0; ii < 3; ii++)
	{
		CFont	*tkFont = GetFont(ii);
		CFont	*sFont = cltDC.SelectObject(tkFont);
		CSize sz = cltDC.GetOutputTextExtent("9");
		m_TkInfo.tkHeight[ii] = sz.cy;

		cltDC.SelectObject(sFont);
	}
}

bool CIndcBase::IsLogChart()
{
	bool	bLog = false;

	switch (m_iGrpKind)
	{
	case GK_JPN:
	case GK_LIN:
	case GK_BAR:
	case GK_POLE:
	case GK_PMA:
	case GK_NET:
	case GK_EQV:
	case GK_CDV:
	case GK_ENVL:
	case GK_PARB:
	case GK_BOLB:
	case GK_DEMARK:
	case GK_PIVOT:
	case GK_BALANCE:
	case GK_AVOL:
		if (m_dwDspOpt & DO_LOG)
			return true;

	case GK_PNF:
	case GK_SWING:
	case GK_THREE:
	case GK_PV:
	case GK_KAGI:
	case GK_RENKO:
	case GK_GUKKI:
		if ((m_dwDspOpt & DO_LOG) && (m_pgraph->dwDraOption & GO_UNION))
			return true;
	}
	
	return false;
}

bool CIndcBase::IsReverseChart()
{
	bool	bLog = false;

	switch (m_iGrpKind)
	{
	case GK_JPN:
	case GK_LIN:
	case GK_BAR:
	case GK_POLE:
	case GK_PMA:
	case GK_NET:
	case GK_EQV:
	case GK_CDV:
	case GK_ENVL:
	case GK_PARB:
	case GK_BOLB:
	case GK_DEMARK:
	case GK_PIVOT:
	case GK_BALANCE:
	case GK_AVOL:
		if (m_dwDspOpt & DO_REVERSE)
			return true;

	case GK_PNF:
	case GK_SWING:
	case GK_THREE:
	case GK_PV:
	case GK_KAGI:
	case GK_RENKO:
	case GK_GUKKI:
		if ((m_dwDspOpt & DO_REVERSE) && (m_pgraph->dwDraOption & GO_UNION))
			return true;

	}
	
	return false;
}

CRect CIndcBase::DisplayRect(CDC *pDC, int idx, CString dispStr, CPoint dispPt)
{
	CRect dispRC = CRect(0, 0, 0, 0);
	CSize size = pDC->GetOutputTextExtent(dispStr);
	if (idx == 0)
		dispRC.SetRect(dispPt.x, dispPt.y+2, 
				dispPt.x + size.cx, dispPt.y + size.cy);
	else
		dispRC.SetRect(dispPt.x - size.cx/2, dispPt.y+2, 
				dispPt.x + size.cx/2+1, dispPt.y + size.cy+2);

	return dispRC;
}

double CIndcBase::GetLogData(double val, bool bLog)
{
	double gVal = val;
	if (bLog)
	{
		if (gVal > 0)	
			gVal = 1000. * double(log(gVal));
		else if (gVal < 0)
			gVal = (-1) * (1000. * double(log(gVal*(-1))));
		else
			gVal = 0.0;
	}
	return gVal;
}

#include "../../h/axisfire.h"
CFont* CIndcBase::GetFont(int tkID, int weight)
{
	bool	bItalic = false;
	if (m_atick[tkID].tfont.btFWeight)
		weight = FW_BOLD;

	_fontR rFont;
	rFont.name = m_atick[tkID].tfont.acFName;
	rFont.point = m_atick[tkID].tfont.btFPoint;
	rFont.italic = 0;
	rFont.bold = weight;
	return (CFont*) m_pwndView->SendMessage(WM_USER, getFONT, long(&rFont));
}

CPen* CIndcBase::GetPen(int style, int width, COLORREF clr)
{
	_penR rPen;
	rPen.clr = clr;
	rPen.width = width;
	rPen.style = style;
	return (CPen*) m_pwndView->SendMessage(WM_USER, getPEN, long(&rPen));
}

CBrush* CIndcBase::GetBrush(COLORREF rColor)
{
	return (CBrush*) m_pwndView->SendMessage(WM_USER, getBRUSH, rColor);
}

COLORREF CIndcBase::GetColor(int index)
{
	return m_pwndView->SendMessage(WM_USER, getPALETTE, index);
}

bool CIndcBase::IsPointInTickRect(CPoint point)
{
	if (m_pgraph->dwDraOption & GO_TKLEFT)
		if (m_TkInfo.tkRect[TICK_LEFT].PtInRect(point))
			return true;
	if (m_pgraph->dwDraOption & GO_TKRIGHT)
		if (m_TkInfo.tkRect[TICK_RIGHT].PtInRect(point))
			return true;
	if (m_pgraph->dwDraOption & GO_TKBOTTOM)
		if (m_TkInfo.tkRect[TICK_BOTTOM].PtInRect(point))
			return true;

	return false;
}

int CIndcBase::IncreaseCount()
{
	if (m_iTtlDtCnt == m_iDispDtCnt)
	{
		m_iTtlDtCnt++;
		m_iDispDtCnt++;
		m_iDispEPos++;
		return m_iTtlDtCnt;
	}
	else if (m_iDispEPos == m_iTtlDtCnt)
	{
		m_iTtlDtCnt++;
		m_iDispSPos++;
		m_iDispEPos++;
	}
	else
		m_iTtlDtCnt++;

	return 0;
}

// 2006.11.20 lsh (start)
CString	CIndcBase::GetDisplayPosDateTime(CPoint pt)
{
	int index = GetDisplayPosIndex(pt);
	if (index < 0)
		return _T("");
	
	class COrgData *pOrgData;
	if (!m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
		return _T("");
	
	CGrpBasic* gBasic;
	gBasic = pOrgData->GetGraphData(index);
	
	CString unitStr;
	unitStr.Format("%04d%02d%02d:%02d%02d%02d", 
		gBasic->m_index.date.yy, gBasic->m_index.date.mm, gBasic->m_index.date.dd,
		gBasic->m_index.time.hh, gBasic->m_index.time.mm, gBasic->m_index.time.ss);
	
	return unitStr;
}
// 2006.11.20 lsh (end)