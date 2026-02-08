// Grp_Juga.cpp: implementation of the CGrp_Juga class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "C_Avol.h"
#include "Grp_Juga.h"

#include "GrpWnd.h"
#include "Grp_Data.h"

#include <float.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrp_Juga::CGrp_Juga(CWnd* pView, class CGrpWnd *pGrpWnd, char *pInfo)
: CGrp_Base(pView, pGrpWnd, pInfo)
{	
	m_clrNORM = RGB(217, 204,   8);
	m_clrCURR = RGB(217, 204,   8);
	m_clrMAX = RGB(217, 204,   8);
	m_fUnit = 0.0;
	m_iDrawHeight = 0;
	m_iDrawWidth = 0;
	m_nCurr = 0;
	m_nMaxIndex = 0;
	m_MaxVolume = 0;

	CalculateMinMax();
}

CGrp_Juga::~CGrp_Juga()
{
	removeData();
}

void CGrp_Juga::DrawGraph(CDC *pDC)
{
	struct _MinMaxChk *pMinMaxChk;
	
	if (pMinMaxChk = m_pGrpWnd->GetMinMaxChk(m_rKey))
	{
		if (pMinMaxChk->coMinMax)
		{
			m_Max = pMinMaxChk->Max;
			m_Min = pMinMaxChk->Min;

			if (m_Max == INT_MIN || m_Min == INT_MAX)
				return;
		}
	}

	if (m_Max == INT_MIN || m_Min == INT_MAX)
		return;

	if (m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkLEFT].bDraw)	DrawAvolTick(pDC, ctkLEFT);
	if (m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkRIGHT].bDraw)	DrawTickHorizon(pDC, m_Max, m_Min, ctkRIGHT);

	DrawTickBottom(pDC);

	DrawAvol(pDC);

	if (m_pGrpWnd->m_jChart > 0)
	{
		if (m_dIndex == CDI_TICK)
			DrawLine(pDC);
		else
		{
			switch (m_pGrpWnd->m_jChart)
			{
			case CGK_LINE:
				DrawLine(pDC);
				break;
			case CGK_BONG:
				DrawBong(pDC);
				break;
			case CGK_BAR:
				DrawBar(pDC);
				break;
			}
		}		
	}
}

void CGrp_Juga::DrawLine(CDC *pDC)
{
	const int	dispPos = m_pGrpWnd->m_dispPos;
	const int	dispEnd = m_pGrpWnd->m_dispEnd;
	const int	dispDay = m_pGrpWnd->m_dispDay;

	CPen* cPen{};
	const COLORREF color = RGB(255, 0, 0);
	const int lineWidth = 1;
	
	cPen = m_pApp->GetPen(m_pView, PS_SOLID, lineWidth, color);
	CPen	*sPen = pDC->SelectObject(cPen);

	const int DrawHeight = m_DrawRect.Height();
	const int DrawWidth  = m_DrawRect.Width();

	double ValueHeight = m_Max - m_Min;
	if (!ValueHeight) ValueHeight = 1;

	int drawShift = 0;
	drawShift = int((DrawWidth/dispDay) / 2);	// Center

	double epr{};
	
	CPoint pointEPR{};
	const CPoint pointSPR = CPoint(-1, -1);

	int	bInit = true;
	struct _cgBong* cgBong{};
	
	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		cgBong = (struct _cgBong *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(ii+dispPos);
		epr = cgBong->epr;

		if (epr < m_Min || epr > m_Max)
			continue;

		pointEPR.x = m_DrawRect.left + drawShift;
		pointEPR.x += int((DrawWidth * ii) / dispDay);
		pointEPR.y = m_DrawRect.top;
		pointEPR.y += int((DrawHeight * (m_Max - double(epr))) / ValueHeight);

		if (bInit)
		{
			pDC->MoveTo(pointEPR);
			bInit = false;
		}
		else
			pDC->LineTo(pointEPR);
	
	}

	pDC->SelectObject(sPen);	
}

void CGrp_Juga::DrawBong(CDC *pDC)
{
	const int	dispPos = m_pGrpWnd->m_dispPos;
	const int	dispEnd = m_pGrpWnd->m_dispEnd;
	const int	dispDay = m_pGrpWnd->m_dispDay;

	const COLORREF upColor = RGB(223,1,0);
	const COLORREF dnColor = RGB(1,0,198);

	CPen* upPen{};
	CPen* dnPen{};
	CPen* sPen{};

	upPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, upColor);
	dnPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, dnColor);
	 
	CBrush* upBrush{};
	CBrush* dnBrush{};
	CBrush* sBrush{};

	upBrush = m_pApp->GetBrush(m_pView, upColor);
	dnBrush = m_pApp->GetBrush(m_pView, dnColor);

	const int DrawHeight = m_DrawRect.Height();
	const int DrawWidth  = m_DrawRect.Width();

	double ValueHeight = (double)(m_Max - m_Min);
	if (ValueHeight == 0.0) 
		ValueHeight = 1.0;

	ValueHeight = (double)DrawHeight / ValueHeight;

	double fpr{}, hpr{}, lpr{}, epr{};
	CPoint pointFPR, pointHPR, pointLPR, pointEPR;

	double	dValue{};
	CPoint	pt[11]{};

	ZeroMemory(&m_MaxDayIndex, sz_cgIndex);
	ZeroMemory(&m_MinDayIndex, sz_cgIndex);
	m_dispMinVal = INT_MAX;
	m_dispMaxVal = INT_MIN;

	bool	bUp{};
	struct _cgBong* gBong{}, * rBong{};
	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		gBong = (struct _cgBong *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(ii+dispPos);

		fpr = (double)gBong->fpr; 
		hpr = (double)gBong->hpr;
		lpr = (double)gBong->lpr; 
		epr = (double)gBong->epr;

		pointFPR.x = m_DrawRect.left;
		dValue = double(DrawWidth * ii) / double(dispDay);
		pointFPR.x += int(dValue);

		pointEPR.x = m_DrawRect.left;
		dValue = double(DrawWidth * (ii+1)) / double(dispDay);
		pointEPR.x += int(dValue);
		pointEPR.x -= 1;	// 그래프간 간격

		pointHPR.x = int(double(pointFPR.x + pointEPR.x - 1) / 2.);
		pointLPR.x = pointHPR.x;

		pointFPR.y = m_DrawRect.top;
		dValue = ValueHeight * (m_Max - double(fpr));
		pointFPR.y += int(dValue);

		pointEPR.y = m_DrawRect.top;
		dValue = ValueHeight * (m_Max - double(epr));
		pointEPR.y += int(dValue);

		pointHPR.y = m_DrawRect.top;
		dValue = ValueHeight * (m_Max - double(hpr));
		pointHPR.y += int(dValue);

		pointLPR.y = m_DrawRect.top;
		dValue = ValueHeight * (m_Max - double(lpr));
		pointLPR.y += int(dValue);
		pointEPR.x -= 1;

		bUp = true;
		if (fpr == epr)
		{
			for (int rr = ii + dispPos; rr >= dispPos; rr--)
			{
				rBong = (struct _cgBong *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(rr);
				if (rBong->epr == epr)
					continue;
				if (rBong->epr > epr)
					bUp = false;
				else
					bUp = true;
				break;
			}
		}
		
		if (fpr > epr || !bUp)	// 시 > 종
		{
			sPen = pDC->SelectObject(dnPen);
			sBrush = pDC->SelectObject(dnBrush);

			pt[0].x = pointHPR.x;	pt[0].y = pointHPR.y;
			pt[1].x = pointHPR.x;	pt[1].y = pointFPR.y;
			pt[2].x = pointFPR.x;	pt[2].y = pointFPR.y;
			pt[3].x = pointFPR.x;	pt[3].y = pointEPR.y;
			pt[4].x = pointLPR.x;	pt[4].y = pointEPR.y;
			pt[5].x = pointLPR.x;	pt[5].y = pointLPR.y;
			pt[6].x = pointLPR.x;	pt[6].y = pointEPR.y;
			pt[7].x = pointEPR.x;	pt[7].y = pointEPR.y;
			pt[8].x = pointEPR.x;	pt[8].y = pointFPR.y;
			pt[9].x = pointHPR.x;	pt[9].y = pointFPR.y;
			pt[10].x = pointHPR.x;	pt[10].y = pointHPR.y;
			pDC->Polygon(pt, 11);
		}
		else		// 시 <= 종
		{
			sPen = pDC->SelectObject(upPen);
			sBrush = pDC->SelectObject(upBrush);

			pt[0].x = pointHPR.x;	pt[0].y = pointHPR.y;
			pt[1].x = pointHPR.x;	pt[1].y = pointEPR.y;
			pt[2].x = pointEPR.x;	pt[2].y = pointEPR.y;
			pt[3].x = pointEPR.x;	pt[3].y = pointFPR.y;
			pt[4].x = pointLPR.x;	pt[4].y = pointFPR.y;
			pt[5].x = pointLPR.x;	pt[5].y = pointLPR.y;
			pt[6].x = pointLPR.x;	pt[6].y = pointFPR.y;
			pt[7].x = pointFPR.x;	pt[7].y = pointFPR.y;
			pt[8].x = pointFPR.x;	pt[8].y = pointEPR.y;
			pt[9].x = pointHPR.x;	pt[9].y = pointEPR.y;
			pt[10].x = pointHPR.x;	pt[10].y = pointHPR.y;
			pDC->Polygon(pt, 11);
		}

		if (gBong->hpr > m_dispMaxVal)
		{
			m_dispMaxVal = gBong->hpr;
			m_MaxDayPos.x = pointHPR.x;
			m_MaxDayPos.y = pointHPR.y;
			CopyMemory(&m_MaxDayIndex, &gBong->index, sz_cgIndex);

		}
		if (gBong->lpr < m_dispMinVal)
		{
			m_dispMinVal = gBong->lpr;
			m_MinDayPos.x = pointLPR.x;
			m_MinDayPos.y = pointLPR.y;
			CopyMemory(&m_MinDayIndex, &gBong->index, sz_cgIndex);
		}

		pDC->SelectObject(sPen);
		pDC->SelectObject(sBrush);
	}

	DrawMinMaxDay(pDC);
}

void CGrp_Juga::DrawMinMaxDay(CDC* pDC)
{
	if (!m_MaxDayIndex.date.yy && !m_MaxDayIndex.time.hh)
		return;

	CString	valstr, dispStr;
	CRect	tkRect;
	CPen* uPen{}, * dPen{}, * sPen{};
	CFont* sFont{};
	CSize	size;

	const COLORREF	upColor = RGB(255, 0, 0);
	const COLORREF	dnColor = RGB(0, 0, 255);

	uPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, upColor);
	dPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, dnColor);

	sFont = pDC->SelectObject(m_pGrpWnd->m_pFont);
	size = pDC->GetTextExtent("틱의높이");
	sPen = pDC->SelectObject(uPen);
	const int	sMode = pDC->SetBkMode(TRANSPARENT);
	const COLORREF sColor = pDC->SetTextColor(upColor);
	
	CString	MaxDayStr, MinDayStr;
	if (m_dIndex == CDI_DAY)
	{
		MaxDayStr.Format("(%d/%d)", 
			m_MaxDayIndex.date.mm, m_MaxDayIndex.date.dd);
		MinDayStr.Format("(%d/%d)", 
			m_MinDayIndex.date.mm, m_MinDayIndex.date.dd);		
	}
	else
	{
		MaxDayStr.Format("(%02d:%02d:%02d)",
			m_MaxDayIndex.time.hh, m_MaxDayIndex.time.mm, m_MaxDayIndex.time.ss);
		MinDayStr.Format("(%02d:%02d:%02d)", 
			m_MinDayIndex.time.hh, m_MinDayIndex.time.mm, m_MinDayIndex.time.ss);
	}

	const CRect	rect = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].tkRect;
	const int	nWidth = rect.Width();
	const int	nHeight = size.cy;

	if (nHeight * 2 < m_DrawRect.Height() + 2)
	{
		FormatVariableComma(valstr, (int)m_dispMaxVal, m_pGrpWnd->m_digit);
		dispStr = " 최고:" + valstr + MaxDayStr;

		size = pDC->GetOutputTextExtent(dispStr);
		tkRect.SetRect(m_MaxDayPos.x + 20, m_MaxDayPos.y - size.cy/2,
			m_MaxDayPos.x + 20 + size.cx, m_MaxDayPos.y + size.cy/2);

		if (tkRect.right > m_DrawRect.right)
		{
			tkRect.SetRect(m_MaxDayPos.x - 20 - size.cx, m_MaxDayPos.y - size.cy/2,
				m_MaxDayPos.x - 20, m_MaxDayPos.y + size.cy/2);

			pDC->MoveTo(m_MaxDayPos.x, m_MaxDayPos.y);
			pDC->LineTo(m_MaxDayPos.x - 4, m_MaxDayPos.y - 4);
			pDC->MoveTo(m_MaxDayPos.x - 3, m_MaxDayPos.y + 3);
			pDC->LineTo(m_MaxDayPos.x, m_MaxDayPos.y);
			pDC->LineTo(m_MaxDayPos.x - 20, m_MaxDayPos.y);
		}
		else
		{
			pDC->MoveTo(m_MaxDayPos.x, m_MaxDayPos.y);
			pDC->LineTo(m_MaxDayPos.x + 4, m_MaxDayPos.y - 4);
			pDC->MoveTo(m_MaxDayPos.x + 3, m_MaxDayPos.y + 3);
			pDC->LineTo(m_MaxDayPos.x, m_MaxDayPos.y);
			pDC->LineTo(m_MaxDayPos.x + 20, m_MaxDayPos.y);
		}

		if (tkRect.top < m_DrawRect.top)
			tkRect.OffsetRect(0, m_DrawRect.top - tkRect.top);
		if (tkRect.bottom > m_DrawRect.bottom)
			tkRect.OffsetRect(0, m_DrawRect.bottom - tkRect.bottom);

		pDC->DrawText(dispStr, &tkRect, DT_CENTER|DT_BOTTOM);

		pDC->SelectObject(dPen);
		pDC->SetTextColor(dnColor);

		FormatVariableComma(valstr, (int)m_dispMinVal, m_pGrpWnd->m_digit);
		dispStr = " 최저:" + valstr + MinDayStr;

		size = pDC->GetOutputTextExtent(dispStr);
		tkRect.SetRect(m_MinDayPos.x + 20, m_MinDayPos.y - size.cy/2,
			m_MinDayPos.x + 20 + size.cx, m_MinDayPos.y + size.cy/2);

		if (tkRect.right > m_DrawRect.right)
		{
			tkRect.SetRect(m_MinDayPos.x - 20 - size.cx, m_MinDayPos.y - size.cy/2,
				m_MinDayPos.x - 20, m_MinDayPos.y + size.cy/2);

			pDC->MoveTo(m_MinDayPos.x, m_MinDayPos.y);
			pDC->LineTo(m_MinDayPos.x - 4, m_MinDayPos.y - 4);
			pDC->MoveTo(m_MinDayPos.x - 3, m_MinDayPos.y + 3);
			pDC->LineTo(m_MinDayPos.x, m_MinDayPos.y);
			pDC->LineTo(m_MinDayPos.x - 20, m_MinDayPos.y);
		}
		else
		{
			pDC->MoveTo(m_MinDayPos.x, m_MinDayPos.y);
			pDC->LineTo(m_MinDayPos.x + 4, m_MinDayPos.y - 4);
			pDC->MoveTo(m_MinDayPos.x + 3, m_MinDayPos.y + 3);
			pDC->LineTo(m_MinDayPos.x, m_MinDayPos.y);
			pDC->LineTo(m_MinDayPos.x + 20, m_MinDayPos.y);
		}

		if (tkRect.top < m_DrawRect.top)
			tkRect.OffsetRect(0, m_DrawRect.top - tkRect.top);
		if (tkRect.bottom > m_DrawRect.bottom)
			tkRect.OffsetRect(0, m_DrawRect.bottom - tkRect.bottom);

		pDC->DrawText(dispStr, &tkRect, DT_CENTER|DT_TOP);
	}

	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
	pDC->SelectObject(sFont);
	pDC->SelectObject(sPen);
}

bool CGrp_Juga::CalculateMinMax()
{
	m_Max = INT_MIN;
	m_Min = INT_MAX;
	m_maxCURR = INT_MIN;
	m_minCURR = INT_MAX;

	if (!m_pGrpWnd->m_pDataInfo[m_dKey] || 
		m_pGrpWnd->m_pDataInfo[m_dKey]->GetDataCount() <= 0)
		return false;

	const int	dispPos = m_pGrpWnd->m_dispPos;
	const int	dispEnd = m_pGrpWnd->m_dispEnd;
	const int	dispDay = m_pGrpWnd->m_dispDay;
	struct _cgBong* gBong{};

	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		gBong = (struct _cgBong *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(ii+dispPos);
		if (m_Max < gBong->hpr)	m_Max = gBong->hpr;
		if (m_Min > gBong->lpr)	m_Min = gBong->lpr;

		if (m_maxCURR < gBong->epr) m_maxCURR = gBong->epr;
		if (m_minCURR > gBong->epr) m_minCURR = gBong->epr;
	}

	double gap = m_Max - m_Min;
	if (gap == 0)
	{
		if (m_Max != DBL_MIN)
		{
			gap = m_Max;
			m_Max += gap / 10;
			m_Min -= gap / 10;
		}
	}
	
	if (m_pGrpWnd->m_nPoll)
		m_fUnit = (m_maxCURR - m_minCURR) / (double)m_pGrpWnd->m_nPoll;
	else
		m_fUnit = 0.0;

	if (m_fUnit == 0.0)
		return false;

	if (!CreateData())
		return false;

	return true;
}

void CGrp_Juga::DrawAvolTick(CDC* pDC, int tkInfo)
{
	const CRect	tickRC = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[tkInfo].tkRect;
	const int	tickHeight = tickRC.Height();
	CFont* tkFont{}, * sFont{};
	tkFont = m_pApp->GetFont(m_pView, m_pGrpWnd->m_fPoint, m_pGrpWnd->m_fName, FW_NORMAL);
	sFont = pDC->SelectObject(tkFont);

	const COLORREF	tkSCALE = RGB(126, 126, 126);

	CPen* tPen{}, * lPen{}, * sPen{};
	tPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, tkSCALE);
	lPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, tkSCALE);

	sPen = pDC->SelectObject(tPen);
	CRect	textRC;
	const CSize	size = pDC->GetTextExtent("0");

	double	yPos = m_minCURR;
	int	nYpos = 0;
	const int sMode = pDC->SetBkMode(TRANSPARENT);
	const COLORREF sColor = pDC->SetTextColor(RGB(0, 0, 0));
	CString	dispStr;

	textRC = tickRC;
	textRC.right -= size.cx;

	for ( int ii = 0 ; ii < m_pGrpWnd->m_nPoll + 1; ii++ )
	{
		nYpos = getYPos(yPos);		
		FormatVariableComma(dispStr, yPos, m_pGrpWnd->m_digit, 20);
		yPos += m_fUnit;
		textRC.bottom = nYpos + size.cy;
		textRC.top = nYpos - size.cy;
		pDC->DrawText(dispStr, textRC, DT_VCENTER | DT_SINGLELINE | DT_RIGHT);
	}

	pDC->SelectObject(sFont);
	pDC->SelectObject(sPen);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);	
}

void CGrp_Juga::removeData()
{
	_vData.clear();
	//const int nSize = m_arData.GetSize();
	//struct	_local_data*	pObject = nullptr;

	//for (int  ii = 0 ; ii < nSize ; ii++ )
	//{
	//	pObject = m_arData.GetAt(ii);
	//	delete pObject;		
	//}

	//m_arData.RemoveAll();
}


CString CGrp_Juga::GetDisplayPosData(CPoint pt)
{
	CString	dispStr = _T("");

	if (m_DrawRect.left > pt.x || m_DrawRect.right <= pt.x)
		return dispStr;


	if (!m_pGrpWnd->m_pDataInfo[m_dKey] || 
		m_pGrpWnd->m_pDataInfo[m_dKey]->GetDataCount() <= 0)
		return dispStr;

	const int	dispPos = m_pGrpWnd->m_dispPos;
	const int	dispEnd = m_pGrpWnd->m_dispEnd;
	const int	dispDay = m_pGrpWnd->m_dispDay;

	const int	xPosition = pt.x - m_DrawRect.left;
	const double	szOneDay = double(m_DrawRect.Width()) / double(dispDay);
	int	dataPos = int(double(xPosition) / szOneDay);
	dataPos += dispPos;

	CString	stmp, stmp2, sSign, sSign2;
	const struct _cgBong	*gBong =
		 (struct _cgBong *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(dataPos);

	if (m_dIndex == CDI_TICK)
	{
		FormatVariableComma(stmp, gBong->epr, m_pGrpWnd->m_digit);
		dispStr = "체결가\t" + stmp;
	}
	else
	{
		FormatVariableComma(stmp, gBong->fpr, m_pGrpWnd->m_digit);
		dispStr = dispStr + "시가\t" + stmp;
		FormatVariableComma(stmp, gBong->hpr, m_pGrpWnd->m_digit);
		dispStr = dispStr + "\n고가\t" + stmp;
		FormatVariableComma(stmp, gBong->lpr, m_pGrpWnd->m_digit);
		dispStr = dispStr + "\n저가\t" + stmp;
		FormatVariableComma(stmp, gBong->epr, m_pGrpWnd->m_digit);
		dispStr = dispStr + "\n종가\t" + stmp;
	}
	
	return dispStr;
}

CString CGrp_Juga::GetExcelData(int idx)
{
	CString	dispStr = __T("");

	if (!m_pGrpWnd->m_pDataInfo[m_dKey] || 
		m_pGrpWnd->m_pDataInfo[m_dKey]->GetDataCount() <= 0)
		return dispStr;
	
	if (idx < 0)
	{
		if (m_dIndex == CDI_MIN || m_dIndex == CDI_TICK)
			dispStr.Format("시가\t고가\t저가\t종가");
		else
			dispStr.Format("시가\t고가\t저가\t종가");
	}
	else
	{
		CString	stmp;
		const struct _cgBong	*gBong = (struct _cgBong *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(idx);

		FormatVariableComma(stmp, gBong->fpr, m_pGrpWnd->m_digit);
		dispStr += stmp + "\t";
		FormatVariableComma(stmp, gBong->hpr, m_pGrpWnd->m_digit);
		dispStr += stmp + "\t";
		FormatVariableComma(stmp, gBong->lpr, m_pGrpWnd->m_digit);
		dispStr += stmp + "\t";
		FormatVariableComma(stmp, gBong->epr, m_pGrpWnd->m_digit);
		dispStr += stmp;
	}

	return dispStr;
}
void CGrp_Juga::DrawBar(CDC *pDC)
{
	const int	dispPos = m_pGrpWnd->m_dispPos;
	const int	dispEnd = m_pGrpWnd->m_dispEnd;
	const int	dispDay = m_pGrpWnd->m_dispDay;

	CPen* cPen{};
	cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, m_tRGB);
	CPen	*sPen = pDC->SelectObject(cPen);

	const int DrawHeight = m_DrawRect.Height();
	const int DrawWidth  = m_DrawRect.Width();

	double ValueHeight = m_Max - m_Min;
	if (ValueHeight == 0) 
		ValueHeight = 1;

	double hpr{}, lpr{}, epr{};
	CPoint pointHPR, pointLPR, pointEPR;

	double	dValue{};
	ZeroMemory(&m_MaxDayIndex, sz_cgIndex);
	ZeroMemory(&m_MinDayIndex, sz_cgIndex);
	m_dispMinVal = INT_MAX;
	m_dispMaxVal = INT_MIN;

	struct _cgBong* gBong{};
	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		gBong = (struct _cgBong*)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(ii+dispPos);
		hpr = gBong->hpr; lpr = gBong->lpr; epr = gBong->epr;

		pointHPR.x = m_DrawRect.left;
		pointHPR.x += int(double(DrawWidth * ii) / double(dispDay));
		pointLPR.x = pointHPR.x;

		pointEPR.x = m_DrawRect.left;
		pointEPR.x += int(double(DrawWidth * (ii +1)) / double(dispDay));
		pointEPR.x -= 1;	// 그래프간 간격

		pointHPR.y = m_DrawRect.top;
		dValue = double(DrawHeight * (m_Max - double(hpr))) / double(ValueHeight);
		pointHPR.y += int(dValue);

		pointLPR.y = m_DrawRect.top;
		dValue = double(DrawHeight * (m_Max - double(lpr))) / double(ValueHeight);
		pointLPR.y += int(dValue);

		pointEPR.y = m_DrawRect.top;
		dValue = double(DrawHeight * (m_Max - double(epr))) / double(ValueHeight);
		pointEPR.y += int(dValue);

		pDC->MoveTo(pointHPR);
		pDC->LineTo(pointLPR);
		pDC->MoveTo(pointEPR);
		pDC->LineTo(pointHPR.x, pointEPR.y);

		if (gBong->hpr > m_dispMaxVal)
		{
			m_dispMaxVal = gBong->hpr;
			m_MaxDayPos = pointHPR;
			CopyMemory(&m_MaxDayIndex, &gBong->index, sz_cgIndex);
		}
		if (gBong->lpr < m_dispMinVal)
		{
			m_dispMinVal = gBong->lpr;
			m_MinDayPos = pointLPR;
			CopyMemory(&m_MinDayIndex, &gBong->index, sz_cgIndex);
		}
	}

	pDC->SelectObject(sPen);
	DrawMinMaxDay(pDC);
}

bool CGrp_Juga::CreateData()		
{	
	CMap<int, int, double, double>	mapValue;
	const int	nPoll = m_pGrpWnd->m_nPoll;
	
	if (nPoll == 0)
		return false;

	removeData();
//	struct _local_data*	pObject = nullptr;           
	int	ii = 0;
	const int	dispEnd = m_pGrpWnd->m_dispEnd;
	const int	dispPos = m_pGrpWnd->m_dispPos;
	const int	dispDay = m_pGrpWnd->m_dispDay;
	const int	nSize =  dispEnd - dispPos;
	int	key = 0;
	double	val = 0.0, sum = 0.0;

	mapValue.RemoveAll();

	for ( ii = 0 ; ii < nPoll ; ii++ )
	{
		mapValue.SetAt(ii, 0);	
	}

	m_nMaxIndex = -1;
	m_nCurr = -1;
	m_MaxVolume = DBL_MIN;

	struct _cgBong*	cgBong = nullptr;
		
	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		cgBong = (struct _cgBong *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(ii+dispPos);

		if (cgBong->epr == UNUSED_VALUE)
			continue;		

		sum += (double)cgBong->trn;
		key = (int)(((double)cgBong->epr - m_minCURR) / m_fUnit);

		if (cgBong->epr == m_maxCURR)
			key = nPoll - 1;

		if (ii == (nSize - 1))
			m_nCurr = key;

		val = 0.0;
		mapValue.Lookup(key, val);
		val += (double)cgBong->trn;		

		if (val > m_MaxVolume)
		{
			m_MaxVolume = val;
			m_nMaxIndex = key;
		}

		mapValue.SetAt(key, val);
	}	

	for ( ii = 0 ; ii < nPoll ; ii++ )
	{
		auto& item = _vData.emplace_back(std::make_unique<struct _local_data>());

		//pObject = new struct _local_data;
		mapValue.Lookup(ii, val);
		item->amount = val;
		item->percent = val / sum * 100.0;
		//m_arData.Add(pObject);
	}

	return true;
}

void CGrp_Juga::DrawAvol(CDC* pDC)
{
	if (m_pGrpWnd->m_nPoll <= 0 || m_MaxVolume <= 0)
		return;

	m_iDrawHeight	= m_DrawRect.Height();
	m_iDrawWidth	= m_DrawRect.Width() - 2;
	
	CPen* pen[3]{}, * sPen = nullptr;
	CBrush* brush[3]{}, * sBrush = nullptr;
	const COLORREF	color[3] = {m_clrNORM, m_clrCURR, m_clrMAX};
	COLORREF	selcolor{};
	int		ii = 0, nWidth = 0;
	CRect		rcDraw, rcDisplay, rect;
	CPoint		pt[5];
	double		yPos = 0.0, val = 0.0;	
	struct	_local_data*	pObject = nullptr;
	CString		dispText;
	const bool	bPercent = true;
	CString		tmpstr;
	CSize		size1, size2;
	const int	nPoll = m_pGrpWnd->m_nPoll;

	FormatVariableComma(tmpstr, m_MaxVolume, 0, 20);
	dispText.Format(" %s (000.00%%) ", tmpstr);
	size1 = pDC->GetTextExtent(dispText);
	size2 = pDC->GetTextExtent("0");

	for ( ii = 0 ; ii < 3 ; ii++ )
	{
		pen[ii] = m_pApp->GetPen(m_pView, PS_SOLID, 1, color[ii]);
		brush[ii] = m_pApp->GetBrush(m_pView, color[ii]);
	}

	sPen = pDC->SelectObject(pen[0]);
	sBrush = pDC->SelectObject(brush[0]);
	rcDisplay = rcDraw = m_DrawRect;
	rcDisplay.left = rcDraw.left = m_DrawRect.left + 2;
	rcDraw.right = rcDraw.left + m_iDrawWidth - size1.cx;
	rcDisplay.left = rcDraw.right;	
	nWidth = rcDraw.Width();	
	yPos = m_minCURR;	
	int	nMid = 0;
	const int nHalf = size1.cy / 2;	
	const int nHeight = m_DrawRect.Height();

	for ( ii = 0 ; ii < nPoll ; ii++ )
	{
		if (ii == m_nMaxIndex)
		{
			pDC->SelectObject(pen[2]);
			pDC->SelectObject(brush[2]);
			selcolor = color[2];
		}
		else
		{
			pDC->SelectObject(pen[0]);
			pDC->SelectObject(brush[0]);
			selcolor = color[0];
		}

		if (ii == m_nCurr)
		{
			pDC->SelectObject(pen[1]);
			pDC->SelectObject(brush[1]);
			selcolor = color[1];
		}

		pObject = _vData.at(ii).get();
		val = pObject->amount;

		pt[1].y = pt[0].y = getYPos(yPos) - 3;
		pt[3].x = pt[0].x = rcDraw.left;
		pt[4] = pt[0];
		pt[2].x = pt[1].x = (int)((double)rcDraw.left + val * nWidth / m_MaxVolume);
		yPos += m_fUnit;

		if (yPos > m_maxCURR)
			yPos = m_maxCURR;		

		pt[3].y = pt[2].y = getYPos(yPos) + 3;	

		rect.SetRect(pt[3], pt[1]);

		if (m_pGrpWnd->m_poleType == PT_3D)
		{
			pDC->Polyline(pt, 5);
			Draw3DBar(pDC, rect, 1, selcolor);
		}
		else
		{	
			pDC->Polygon(pt, 5);
		}
		
		rcDisplay.left = pt[2].x + size2.cx;
		rcDisplay.bottom = pt[0].y;
		rcDisplay.top = pt[3].y;
		nMid = (pt[0].y + pt[2].y) / 2;		
		rcDisplay.right = m_DrawRect.right;
		rcDisplay.bottom = nMid + size1.cy;
		rcDisplay.top = nMid - size1.cy;		

		dispText.Format("%.2lf%%", pObject->percent);
		pDC->DrawText(dispText, rcDisplay, DT_SINGLELINE|DT_VCENTER|DT_LEFT);				
	}

	pDC->SelectObject(sBrush);
	pDC->SelectObject(sPen);
}

int CGrp_Juga::getYPos(double val)
{
	const double	MaxValue = m_Max;
	const double	MinValue = m_Min;
	const double	yVal = val;
	

	const double	fMinMax = MaxValue - MinValue;
	const double	fHeight = m_DrawRect.Height();
	double	yPos = 0;
	const double	fGab = MaxValue - yVal;

	if (fMinMax <= 0.0)
		return m_DrawRect.bottom;

	yPos = fHeight * fGab / fMinMax;
	yPos = (double)m_DrawRect.top + yPos;

	return int(yPos);
}

bool CGrp_Juga::Draw3DBar(CDC* pDC, CRect rc, int Gap, COLORREF orgColor)
{
	COLORREF	color[3];
	color[0] = orgColor;
	Get3DRGB(color);

	CPen* cPen{}, * sPen{};
	CBrush* cBrush{}, * sBrush{};

	cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, color[0]);
	sPen = pDC->SelectObject(cPen);
	cBrush = m_pApp->GetBrush(m_pView, color[0]);
	sBrush = pDC->SelectObject(cBrush);

	CPoint	pointBAR[6];						// 정면

	pointBAR[0].x = rc.left;
	pointBAR[0].y = rc.top;
	pointBAR[1].x = rc.left;
	pointBAR[1].y = rc.bottom;
	pointBAR[2].x = rc.right;
	pointBAR[2].y = rc.bottom;
	pointBAR[3].x = rc.right;
	pointBAR[3].y = rc.top;
	pDC->Polygon(pointBAR, 4);

	pDC->SelectObject(sBrush);
	pDC->SelectObject(sPen);

	cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, color[1]);
	sPen = pDC->SelectObject(cPen);
	cBrush = m_pApp->GetBrush(m_pView, color[1]);
	sBrush = pDC->SelectObject(cBrush);

	pointBAR[0].x = rc.left;
	pointBAR[0].y = rc.bottom;
	pointBAR[1].x = rc.left;
	pointBAR[1].y = rc.top;
	pointBAR[2].x = rc.right;
	pointBAR[2].y = rc.top;
	pointBAR[3].x = rc.right + Gap;
	pointBAR[3].y = rc.top - Gap;
	pointBAR[4].x = rc.left - Gap;
	pointBAR[4].y = rc.top - Gap;
	pointBAR[5].x = rc.left - Gap;
	pointBAR[5].y = rc.bottom + Gap;
	pDC->Polygon(pointBAR, sizeof(pointBAR)/sizeof(CPoint));

	pDC->SelectObject(sBrush);
	pDC->SelectObject(sPen);

	cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, color[2]);
	sPen = pDC->SelectObject(cPen);
	cBrush = m_pApp->GetBrush(m_pView, color[2]);
	sBrush = pDC->SelectObject(cBrush);

	pointBAR[0].x = rc.left;
	pointBAR[0].y = rc.bottom;
	pointBAR[1].x = rc.right;
	pointBAR[1].y = rc.bottom;
	pointBAR[2].x = rc.right;
	pointBAR[2].y = rc.top;
	pointBAR[3].x = rc.right + Gap;
	pointBAR[3].y = rc.top - Gap;
	pointBAR[4].x = rc.right + Gap;
	pointBAR[4].y = rc.bottom + Gap;
	pointBAR[5].x = rc.left - Gap;
	pointBAR[5].y = rc.bottom + Gap;
	pDC->Polygon(pointBAR, sizeof(pointBAR)/sizeof(CPoint));

	pDC->SelectObject(sBrush);
	pDC->SelectObject(sPen);
	return true;
}

// 0:origin color, [1]:light color, [2]:dark color
void CGrp_Juga::Get3DRGB(COLORREF color[3])
{
	const int	rColor = GetRValue(color[0]);
	const int	gColor = GetGValue(color[0]);
	const int	bColor = GetBValue(color[0]);
	const int	rRate  = rColor / 5;
	const int	gRate  = gColor / 5;
	const int	bRate  = bColor / 5;

	int	r = (rColor + rRate > 255)? 255:rColor + rRate;
	int	g = (gColor + gRate > 255)? 255:gColor + gRate;
	int	b = (bColor + bRate > 255)? 255:bColor + bRate;
	color[1] = RGB(r, g, b);
	r = (rColor - rRate < 0)? 0:rColor - rRate;
	g = (gColor - gRate < 0)? 0:gColor - gRate;
	b = (bColor - bRate < 0)? 0:bColor - bRate;
	color[2] = RGB(r, g, b);
}
