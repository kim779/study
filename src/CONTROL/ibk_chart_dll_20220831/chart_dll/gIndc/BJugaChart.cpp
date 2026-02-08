#include "stdafx.h"
#include "BJugaChart.h"
#include "../gData/DataMgr.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CBJugaChart::CBJugaChart(CWnd* pwndView, CWnd* pwndParent, class CDataMgr* pDataMgr, char* pcInfo, char* pcGraph, int iDispDay)
: CBongBase(pwndView, pwndParent, pDataMgr, pcInfo, pcGraph, iDispDay)
{
}

CBJugaChart::~CBJugaChart()
{
}

void CBJugaChart::DrawGraph(CDC* pDC)
{
	m_RgnInfo.iCount = 0;

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
	{
		TRACE("Invalid JugaChart Data\n");
		return;
	}

	if (pOrgData->GetPattern() != dpBASIC)
		return;

	if (CIndcBase::m_pCoDraw && CIndcBase::m_pCoDraw->coWork == MC_PRC)
	{
		m_dYMax = m_pCoDraw->PMax;
		m_dYMin = m_pCoDraw->PMin;
	}
	if (m_dYMax == DBL_MIN || m_dYMin == DBL_MAX)
		return;

//	CBongBase::DrawTick(pDC);

	DrawBong(pDC);
}

void CBJugaChart::DrawBong(CDC* pDC)
{
	switch(m_iGrpKind)
	{
	case GK_JPN:
	case GK_BAR:
	case GK_LIN:
	case GK_POLE:
		{
			class COrgData *pOrgData;
			if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
				return;
			
			int index = pOrgData->GetIndex();
			int gap = pOrgData->GetDataGap();
			if (index == GI_TICK && gap <= 1)
				DrawBongLIN(pDC);	// 1틱
			else
			{
				switch (m_iGrpKind)
				{
				case GK_JPN:
					DrawBongJPN(pDC);
					break;
				case GK_BAR:
					DrawBongUSA(pDC);
					break;
				case GK_LIN:
					DrawBongLIN(pDC);
					break;
				case GK_POLE:
					DrawBongPole(pDC);
					break;
				}
			}

			struct _EXTDRAWINFO {
				CDC* pdc;
				COrgData* pOrgData;
				LPRECT rcChart;
				double max;
				double min;
				BOOL   log;
			} di;
			di.pdc = pDC;
			di.pOrgData = pOrgData;

			di.rcChart = m_rectGrp;
			di.max = m_dYMax;
			di.min = m_dYMin;
			di.log = IsLogChart();
			
			m_pwndParent->SendMessage(WM_USER + 999, (WPARAM)_T("EXTM_DRAWBONG"), (LPARAM)&di);
		}
		break;		
	case GK_EQV:
	case GK_CDV:
		DrawBongJPN(pDC);
		break;
	default:
		return;
	}

	return;
}

void CBJugaChart::DrawBongJPN(CDC* pDC)
{
	if (m_bEqui && !m_piVol)
		return;

	int	iMaxPrc = INT_MIN,  iMinPrc = INT_MAX;
	bool	bMaxUpdate = false, bMinUpdate = false;

	int ii = 0, iRgnCnt = 0;
	CPoint* ppointRgn;

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return;

	CGrpBasic* gBasic;
	CGrpBasic* rBasic;
	for (ii = 0; ii < m_iDispEPos - m_iDispSPos; ii++)
	{
		gBasic = pOrgData->GetGraphData(ii+m_iDispSPos);
		if (gBasic->m_iClose != GD_UNUSED)
			break;
	}

	int iPointCnt = m_iDispEPos - m_iDispSPos - ii;
	if (iPointCnt <= 0)
		return;

	ppointRgn = new CPoint[iPointCnt * 2];

	COLORREF rgbUp = m_pgraph->cInfo.aline[1].rgbLColor;// ^ m_gbColor;
	COLORREF rgbDown = m_pgraph->cInfo.aline[2].rgbLColor;// ^ m_gbColor;

	CPen* ppenUp = GetPen(PS_SOLID, 1, rgbUp);
	CPen* ppenDown = GetPen(PS_SOLID, 1, rgbDown);
	CPen* ppenOld = pDC->SelectObject(ppenUp);
	 
	CBrush* pbrushUp = GetBrush(rgbUp);
	CBrush* pbrushDown = GetBrush(rgbDown);
	CBrush* pbrushOld = pDC->SelectObject(pbrushUp);
//	int	iRopOld = pDC->SetROP2(R2_XORPEN);

	int iDrawHeight = m_rectGrp.Height();
	int iDrawWidth  = m_rectGrp.Width();

	bool bLog = IsLogChart();
	bool bReverse = IsReverseChart();
	bool bUpFill = (m_pgraph->dwDraOption & GO_FIILUPBONG)? true:false;
	bool bDnFill = (m_pgraph->dwDraOption & GO_FIILDNBONG)? true:false;
	
	double dMax = m_dYMax;
	double dMin = m_dYMin;
	if (bLog)
	{
		dMax = GetLogData(dMax);
		dMin = GetLogData(dMin);
	}
	double dValueHeight = (double)iDrawHeight / (double)(dMax - dMin);
	if (dValueHeight == 0.0) 
		dValueHeight = 1.0;

	double dOpen, dHigh, dLow, dClose;
	CPoint pointOpen, pointHigh, pointLow, pointClose;

	double dValue;
	CPoint apointBong[11];
	int iGKind = GetGraphKind();

	ZeroMemory(&m_gindexMaxDay, sz_gIndex);
	ZeroMemory(&m_gindexMinDay, sz_gIndex);

	bool bUp;
	double dEquiWidth = 0.0;
	if (m_dTotal)
		dEquiWidth = (double)iDrawWidth / (double)m_dTotal;

	double dLeft;
	double dRight = m_rectGrp.left;
	for (; ii < m_iDispEPos - m_iDispSPos; ii++)
	{
		gBasic = pOrgData->GetGraphData(ii+m_iDispSPos);

		dOpen = (double)gBasic->m_iOpen; 
		dHigh = (double)gBasic->m_iHigh;
		dLow = (double)gBasic->m_iLow; 
		dClose = (double)gBasic->m_iClose;
		if (bLog)
		{
			dOpen = GetLogData(dOpen);
			dHigh = GetLogData(dHigh);
			dLow = GetLogData(dLow);
			dClose = GetLogData(dClose);
		}

		if (gBasic->m_iHigh > iMaxPrc)
		{
			bMaxUpdate = true;
			m_iDispMaxVal = iMaxPrc = gBasic->m_iHigh;
		}
		if (gBasic->m_iLow < iMinPrc)
		{
			bMinUpdate = true;
			m_iDispMinVal = iMinPrc = gBasic->m_iLow;
		}
	
		if (!m_bEqui)
		{
			pointOpen.x = m_rectGrp.left;
			dValue = double(iDrawWidth * ii) / double(m_iDispDtCnt);
			pointOpen.x += int(dValue);

			pointClose.x = m_rectGrp.left;
			dValue = double(iDrawWidth * (ii+1)) / double(m_iDispDtCnt);
			pointClose.x += int(dValue);
			pointClose.x -= DISTANCE_GAP;	// 그래프간 간격

			// 2006.06.08 schbang : 날씬한 봉
//			if (pointClose.x - pointOpen.x < 3)
//				pointClose.x = pointOpen.x + 1;

			pointHigh.x = int(double(pointOpen.x + pointClose.x - DISTANCE_GAP) / 2.);
			pointLow.x = pointHigh.x;
		}
		else
		{
			dLeft = dRight;
			dRight += m_piVol[ii] * dEquiWidth;
			pointOpen.x = int(dLeft);
			pointClose.x = int(dRight) - DISTANCE_GAP;
			pointHigh.x = int(double(pointOpen.x + pointClose.x - DISTANCE_GAP) / 2.);
			pointLow.x = pointHigh.x;
		}

		if (bReverse)
		{
			pointOpen.y = m_rectGrp.bottom;
			dValue = dValueHeight * (dMax - dOpen);
			pointOpen.y -= int(dValue);

			pointClose.y = m_rectGrp.bottom;
			dValue = dValueHeight * (dMax - dClose);
			pointClose.y -= int(dValue);

			pointHigh.y = m_rectGrp.bottom;
			dValue = dValueHeight * (dMax - dHigh);
			pointHigh.y -= int(dValue);

			pointLow.y = m_rectGrp.bottom;
			dValue = dValueHeight * (dMax - dLow);
			pointLow.y -= int(dValue);

		}
		else
		{
			pointOpen.y = m_rectGrp.top;
			dValue = dValueHeight * (dMax - double(dOpen));
			pointOpen.y += int(dValue);

			pointClose.y = m_rectGrp.top;
			dValue = dValueHeight * (dMax - double(dClose));
			pointClose.y += int(dValue);

			pointHigh.y = m_rectGrp.top;
			dValue = dValueHeight * (dMax - double(dHigh));
			pointHigh.y += int(dValue);

			pointLow.y = m_rectGrp.top;
			dValue = dValueHeight * (dMax - double(dLow));
			pointLow.y += int(dValue);
		}

		if (iGKind == GK_EQV)
		{
			if (dOpen > dClose)
			{
				pointOpen.y = pointHigh.y;
				pointClose.y = pointLow.y;
			}
			else
			{
				pointClose.y = pointHigh.y;
				pointOpen.y = pointLow.y;
			}
		}

		if (!m_bEqui)
			pointClose.x -= DISTANCE_GAP;
		else
		{
			if (pointClose.x - pointOpen.x > DISTANCE_GAP)
				pointClose.x -= DISTANCE_GAP;
		}
		bUp = true;
		if (dOpen == dClose)
		{
			for (int rr = ii+m_iDispSPos; rr >= m_iDispSPos; rr--)
			{
				rBasic = pOrgData->GetGraphData(rr);
				if (rBasic->m_iClose == dClose)
					continue;
				if (rBasic->m_iClose > dClose)
					bUp = false;
				else
					bUp = true;
				break;
			}
		}
		
		if (dOpen > dClose || !bUp)	// 시 > 종
		{
			if ((pointHigh == pointLow) && (pointOpen == pointClose))
			{
				pDC->SetPixel(pointClose, rgbDown);
			}
			else
			{
				pDC->SelectObject(ppenDown);
				pDC->SelectObject(pbrushDown);

				apointBong[0].x = pointHigh.x;	apointBong[0].y = pointHigh.y;
				apointBong[1].x = pointHigh.x;	apointBong[1].y = pointOpen.y;
				apointBong[2].x = pointOpen.x;	apointBong[2].y = pointOpen.y;
				apointBong[3].x = pointOpen.x;	apointBong[3].y = pointClose.y;
				apointBong[4].x = pointLow.x;	apointBong[4].y = pointClose.y;
				apointBong[5].x = pointLow.x;	apointBong[5].y = pointLow.y;
				apointBong[6].x = pointLow.x;	apointBong[6].y = pointClose.y;
				apointBong[7].x = pointClose.x;	apointBong[7].y = pointClose.y;
				apointBong[8].x = pointClose.x;	apointBong[8].y = pointOpen.y;
				apointBong[9].x = pointHigh.x;	apointBong[9].y = pointOpen.y;
				apointBong[10].x = pointHigh.x;	apointBong[10].y = pointHigh.y;

				if (bDnFill)
					pDC->Polygon(apointBong, 11);
				else
					pDC->Polyline(apointBong, 11);
			}
		}
		else		// 시 <= 종
		{
			if ((pointHigh == pointLow) && (pointOpen == pointClose))
			{
				pDC->SetPixel(pointClose, rgbUp);
			}
			else
			{
				pDC->SelectObject(ppenUp);
				pDC->SelectObject(pbrushUp);

				apointBong[0].x = pointHigh.x;	apointBong[0].y = pointHigh.y;
				apointBong[1].x = pointHigh.x;	apointBong[1].y = pointClose.y;
				apointBong[2].x = pointClose.x;	apointBong[2].y = pointClose.y;
				apointBong[3].x = pointClose.x;	apointBong[3].y = pointOpen.y;
				apointBong[4].x = pointLow.x;	apointBong[4].y = pointOpen.y;
				apointBong[5].x = pointLow.x;	apointBong[5].y = pointLow.y;
				apointBong[6].x = pointLow.x;	apointBong[6].y = pointOpen.y;
				apointBong[7].x = pointOpen.x;	apointBong[7].y = pointOpen.y;
				apointBong[8].x = pointOpen.x;	apointBong[8].y = pointClose.y;
				apointBong[9].x = pointHigh.x;	apointBong[9].y = pointClose.y;
				apointBong[10].x = pointHigh.x;	apointBong[10].y = pointHigh.y;

				if (bUpFill)
					pDC->Polygon(apointBong, 11);
				else
					pDC->Polyline(apointBong, 11);
			}
		}

		ppointRgn[iRgnCnt] = pointLow;
		ppointRgn[(iPointCnt) * 2 - iRgnCnt - 1] = pointHigh;
		iRgnCnt++;

		if (bMaxUpdate)
		{
			bMaxUpdate = false;
			m_ptMaxDayPos.x = pointHigh.x;
			m_ptMaxDayPos.y = pointHigh.y;
			CopyMemory(&m_gindexMaxDay, &gBasic->m_index, sz_gIndex);
		}
		if (bMinUpdate)
		{
			bMinUpdate = false;
			m_ptMinDayPos.x = pointLow.x;
			m_ptMinDayPos.y = pointLow.y;
			CopyMemory(&m_gindexMinDay, &gBasic->m_index, sz_gIndex);
		}

		if (gBasic->m_dwCheck)
			DisplayCheckBits(pDC, CPoint(pointOpen.x, pointHigh.y), gBasic->m_dwCheck, gBasic->m_wRate);
	}
	pDC->SelectObject(ppenOld);
	pDC->SelectObject(pbrushOld);
//	pDC->SetROP2(iRopOld);

	if (m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn)
	{
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->DeleteObject();
		delete m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn;
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = NULL;
	}

	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = new CRgn;
	m_RgnInfo.Rgn[m_RgnInfo.iCount].iIndex = 0;
	if (iRgnCnt == 0)
	{
		ppointRgn[0] = CPoint(0,0);
		ppointRgn[1] = CPoint(1,1);
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreatePolygonRgn(ppointRgn, 2, WINDING);
	}
	else
	{
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreatePolygonRgn(ppointRgn, iRgnCnt * 2, WINDING);
	}

	if (m_RgnInfo.iSelect >= 0 && m_rectGrp.Height() > 10)
	{
		int	iRopOld = pDC->SetROP2(R2_XORPEN);
		CBrush	*rgnBrush = GetBrush(_L_WHITE);
		CBrush	*srgnBrush = pDC->SelectObject(rgnBrush);
		for (int ii = 0; ii < 4; ii++)
		{
			int idx = iPointCnt * (ii+1)/5;

			pointClose.x = ppointRgn[idx].x;
			pointClose.y = ppointRgn[idx].y - rgnMARKGAP;

			CRect	rc;
			rc.SetRect(pointClose.x - rgnMARKGAP, pointClose.y - rgnMARKGAP, 
				pointClose.x + rgnMARKGAP, pointClose.y + rgnMARKGAP);
			rc.IntersectRect(&rc, &m_rectGrp);
			pDC->Rectangle(&rc);
		}
		pDC->SetROP2(iRopOld);
		pDC->SelectObject(srgnBrush);
	}

//	DrawTickIndex(pDC, "가격차트", m_pgraph->rgbLabel, true);
	m_RgnInfo.iCount++;

	delete[] ppointRgn;
	ppointRgn = NULL;

	DrawMinMaxDay(pDC);
}

void CBJugaChart::DrawBongUSA(CDC* pDC)
{
	int	MaxPrc = INT_MIN;
	int	MinPrc = INT_MAX;
	bool	bMaxUpdate = false;
	bool	bMinUpdate = false;

	int	ii = 0;
	int	cnt = 0, rgncnt = 0;
	CPoint	*pRgnPoint;

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return;

	CGrpBasic* gBasic;
	CGrpBasic* rBasic;
	for (ii = 0; ii < m_iDispEPos - m_iDispSPos; ii++)
	{
		gBasic = pOrgData->GetGraphData(ii+m_iDispSPos);
		if (gBasic->m_iClose != GD_UNUSED)
			break;
	}
	int nPoint = m_iDispEPos - m_iDispSPos - ii;
	if (nPoint <= 0)
		return;

	pRgnPoint = new CPoint[nPoint * 2];

	COLORREF upColor = m_pgraph->cInfo.aline[1].rgbLColor;
	COLORREF dnColor = m_pgraph->cInfo.aline[2].rgbLColor;
	CPen	*upPen = GetPen(PS_SOLID, 1, upColor);
	CPen	*dnPen = GetPen(PS_SOLID, 1, dnColor);
	CPen	*sPen = pDC->SelectObject(upPen);

	int DrawHeight = m_rectGrp.Height();
	int DrawWidth  = m_rectGrp.Width();

	bool	bLog = IsLogChart();
	bool	bReverse = IsReverseChart();
	
	double	Max = m_dYMax;
	double	Min = m_dYMin;
	if (bLog)
	{
		Max = GetLogData(Max);
		Min = GetLogData(Min);
	}

	double ValueHeight = Max - Min;
	if (ValueHeight == 0) 
		ValueHeight = 1;

	double fpr, hpr, lpr, epr;
	CPoint pointHPR, pointLPR, pointEPR;

	double	dValue;
	ZeroMemory(&m_gindexMaxDay, sz_gIndex);
	ZeroMemory(&m_gindexMinDay, sz_gIndex);

	bool	bUp;
	COLORREF	PixClr = upColor;
	for (; ii < m_iDispEPos - m_iDispSPos; ii++)
	{
		gBasic = pOrgData->GetGraphData(ii+m_iDispSPos);

		fpr = gBasic->m_iOpen; hpr = gBasic->m_iHigh; lpr = gBasic->m_iLow; epr = gBasic->m_iClose;

		bUp = true;
		if (fpr == epr)
		{
			for (int rr = ii+m_iDispSPos; rr >= m_iDispSPos; rr--)
			{
				rBasic = pOrgData->GetGraphData(rr);
				if (rBasic->m_iClose == epr)
					continue;
				if (rBasic->m_iClose > epr)
					bUp = false;
				else
					bUp = true;
				break;
			}
		}

		if (fpr > epr || !bUp)	// 시 > 종
		{
			PixClr = dnColor;
			pDC->SelectObject(dnPen);
		}
		else // 시 <= 종
		{
			PixClr = upColor;
			pDC->SelectObject(upPen);
		}

		if (bLog)
		{
			hpr = GetLogData(hpr);	lpr = GetLogData(lpr);	epr = GetLogData(epr);
		}

		if (gBasic->m_iHigh > MaxPrc)
		{
			bMaxUpdate = true;
			m_iDispMaxVal = MaxPrc = gBasic->m_iHigh;
		}
		if (gBasic->m_iLow < MinPrc)
		{
			bMinUpdate = true;
			m_iDispMinVal = MinPrc = gBasic->m_iLow;
		}

		pointHPR.x = m_rectGrp.left;
		pointHPR.x += int(double(DrawWidth * ii) / double(m_iDispDtCnt));
		pointLPR.x = pointHPR.x;

		pointEPR.x = m_rectGrp.left;
		pointEPR.x += int(double(DrawWidth * (ii +1)) / double(m_iDispDtCnt));
		pointEPR.x -= DISTANCE_GAP;	// 그래프간 간격

		if (bReverse)
		{
			pointHPR.y = m_rectGrp.bottom;
			dValue = double(DrawHeight * (Max - hpr)) / double(ValueHeight);
			pointHPR.y -= int(dValue);

			pointLPR.y = m_rectGrp.bottom;
			dValue = double(DrawHeight * (Max - lpr)) / double(ValueHeight);
			pointLPR.y -= int(dValue);

			pointEPR.y = m_rectGrp.bottom;
			dValue = double(DrawHeight * (Max - epr)) / double(ValueHeight);
			pointEPR.y -= int(dValue);
		}
		else
		{
			pointHPR.y = m_rectGrp.top;
			dValue = double(DrawHeight * (Max - hpr)) / double(ValueHeight);
			pointHPR.y += int(dValue);

			pointLPR.y = m_rectGrp.top;
			dValue = double(DrawHeight * (Max - lpr)) / double(ValueHeight);
			pointLPR.y += int(dValue);

			pointEPR.y = m_rectGrp.top;
			dValue = double(DrawHeight * (Max - epr)) / double(ValueHeight);
			pointEPR.y += int(dValue);
		}

		if ((pointHPR == pointLPR) && (pointLPR == pointEPR))
		{
			pDC->SetPixel(pointEPR, PixClr);
		}
		else
		{
			pDC->MoveTo(pointHPR);
			pDC->LineTo(pointLPR);
			pDC->MoveTo(pointEPR);
			pDC->LineTo(pointHPR.x, pointEPR.y);
		}

		pRgnPoint[rgncnt] = pointLPR;
		pRgnPoint[nPoint*2 - rgncnt - 1] = pointHPR;
		rgncnt++;

		if (bMaxUpdate)
		{
			bMaxUpdate = false;
			m_ptMaxDayPos = pointHPR;
			CopyMemory(&m_gindexMaxDay, &gBasic->m_index, sz_gIndex);
		}
		if (bMinUpdate)
		{
			bMinUpdate = false;
			m_ptMinDayPos = pointLPR;
			CopyMemory(&m_gindexMinDay, &gBasic->m_index, sz_gIndex);
		}

		if (gBasic->m_dwCheck)
			DisplayCheckBits(pDC, pointHPR, gBasic->m_dwCheck, gBasic->m_wRate);

	}
	pDC->SelectObject(sPen);

	if (m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn)
	{
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->DeleteObject();
		delete m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn;
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = NULL;
	}

	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = new CRgn;
	if (rgncnt == 0)
	{
		pRgnPoint[0] = CPoint(0,0);
		pRgnPoint[1] = CPoint(1,1);
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreatePolygonRgn(pRgnPoint, 2, WINDING);
	}
	else
	{
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreatePolygonRgn(pRgnPoint, rgncnt * 2, WINDING);
	}

	if (m_RgnInfo.iSelect >= 0 && m_rectGrp.Height() > 10)
	{
		int	sROP = pDC->SetROP2(R2_XORPEN);
		CBrush	*rgnBrush = GetBrush(_L_WHITE);
		CBrush	*srgnBrush = pDC->SelectObject(rgnBrush);
		for (int ii = 0; ii < 4; ii++)
		{
			int idx = nPoint * (ii+1)/5;

			pointEPR.x = pRgnPoint[idx].x;
			pointEPR.y = pRgnPoint[idx].y - rgnMARKGAP;

			CRect	rc;
			rc.SetRect(pointEPR.x - rgnMARKGAP, pointEPR.y - rgnMARKGAP, 
				pointEPR.x + rgnMARKGAP, pointEPR.y + rgnMARKGAP);
			rc.IntersectRect(&rc, &m_rectGrp);
			pDC->Rectangle(&rc);
		}
		pDC->SetROP2(sROP);
		pDC->SelectObject(srgnBrush);
	}

//	DrawTickIndex(pDC, "가격차트", m_pgraph->cInfo.aline[0].rgbLColor, true);
	m_RgnInfo.iCount++;

	delete[] pRgnPoint;
	pRgnPoint = NULL;

	DrawMinMaxDay(pDC);
}

void CBJugaChart::DrawBongLIN(CDC* pDC)
{
	int	MaxPrc = INT_MIN;
	int	MinPrc = INT_MAX;
	bool	bMaxUpdate = false;
	bool	bMinUpdate = false;

	int	rgncnt = 0;
	CPoint	*pRgnPoint;

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return;

	int	ii = 0;
	CGrpBasic* gBasic;
	for (ii = 0; ii < m_iDispEPos - m_iDispSPos; ii++)
	{
		gBasic = pOrgData->GetGraphData(ii+m_iDispSPos);
		if (gBasic->m_iClose != GD_UNUSED)
			break;
	}
	int nPoint = m_iDispEPos - m_iDispSPos - ii;
	if (nPoint <= 0)
		return;

	pRgnPoint = new CPoint[nPoint * 2];

	COLORREF lcolor = m_pgraph->cInfo.aline[0].rgbLColor;
	int	btLWidth = m_pgraph->cInfo.aline[0].btLWidth;
	int	btLStyle = m_pgraph->cInfo.aline[0].btLStyle;
	
	CPen	*cPen = GetPen(btLStyle, btLWidth, lcolor);
	CPen	*sPen = pDC->SelectObject(cPen);

	int DrawHeight = m_rectGrp.Height();
	int DrawWidth  = m_rectGrp.Width();

	bool	bLog = IsLogChart();
	bool	bReverse = IsReverseChart();
	
	double	Max = m_dYMax;
	double	Min = m_dYMin;
	if (bLog)
	{
		Max = GetLogData(Max);
		Min = GetLogData(Min);
	}
	double ValueHeight = Max - Min;
	if (!ValueHeight) ValueHeight = 1;

	double	epr;
	int drawShift = int((DrawWidth/m_iDispDtCnt) / 2);	// Center

	CPoint pointEPR;
	CPoint pointFPR;
	CPoint pointSPR = CPoint(-1, -1);

	ZeroMemory(&m_gindexMaxDay, sz_gIndex);
	ZeroMemory(&m_gindexMinDay, sz_gIndex);

	int	bInit = true;
	for (; ii < (m_iDispEPos-m_iDispSPos); ii++)
	{
		gBasic = pOrgData->GetGraphData(ii+m_iDispSPos);
		epr = gBasic->m_iClose;

		if (epr < m_dYMin || epr > m_dYMax)
			continue;

		if (bLog)
			epr = GetLogData(epr);

		if (gBasic->m_iClose > MaxPrc)
		{
			bMaxUpdate = true;
			m_iDispMaxVal = MaxPrc = gBasic->m_iClose;
		}
		if (gBasic->m_iClose < MinPrc)
		{
			bMinUpdate = true;
			m_iDispMinVal = MinPrc = gBasic->m_iClose;
		}

		pointEPR.x = m_rectGrp.left + drawShift;
		pointEPR.x += int((DrawWidth * ii) / m_iDispDtCnt);

		if (bReverse)
		{
			pointEPR.y = m_rectGrp.bottom;
			pointEPR.y -= int((DrawHeight * (Max - epr)) / ValueHeight);
			
		}
		else
		{
			pointEPR.y = m_rectGrp.top;
			pointEPR.y += int((DrawHeight * (Max - epr)) / ValueHeight);
			
		}

		pRgnPoint[rgncnt++] = pointEPR;
		if (bInit)
		{
			pDC->MoveTo(pointEPR);
			bInit = false;
		}
		else
			pDC->LineTo(pointEPR);

		if (bMaxUpdate)
		{
			bMaxUpdate = false;
			m_ptMaxDayPos = pointEPR;
			CopyMemory(&m_gindexMaxDay, &gBasic->m_index, sz_gIndex);
		}
		if (bMinUpdate)
		{
			bMinUpdate = false;
			m_ptMinDayPos = pointEPR;
			CopyMemory(&m_gindexMinDay, &gBasic->m_index, sz_gIndex);
		}

		if (gBasic->m_dwCheck)
			DisplayCheckBits(pDC, pointEPR, gBasic->m_dwCheck, gBasic->m_wRate);
	}
	pDC->SelectObject(sPen);

	if (m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn)
	{
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->DeleteObject();
		delete m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn;
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = NULL;
	}

	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = new CRgn;
	if (rgncnt == 0)
	{
		pRgnPoint[0] = CPoint(0,0);
		pRgnPoint[1] = CPoint(1,1);
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreatePolygonRgn(pRgnPoint, 2, WINDING);
	}
	else
	{
		for (int jj = 0; jj < rgncnt; jj++)
		{
			pRgnPoint[rgncnt*2 - jj - 1] = pRgnPoint[jj];
			pRgnPoint[jj].y += rgnLINEGAP;
			pRgnPoint[rgncnt*2 - jj - 1].y -= rgnLINEGAP;
		}
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreatePolygonRgn(pRgnPoint, rgncnt * 2, WINDING);
	}

	if (m_RgnInfo.iSelect >= 0 && m_rectGrp.Height() > 10)
	{
		int	sROP = pDC->SetROP2(R2_XORPEN);
		CBrush	*rgnBrush = GetBrush(_L_WHITE);
		CBrush	*srgnBrush = pDC->SelectObject(rgnBrush);
		for (int ii = 0; ii < 4; ii++)
		{
			int idx = (rgncnt) * (ii+1)/5;

			pointEPR.x = pRgnPoint[idx].x;
			pointEPR.y = pRgnPoint[idx].y - rgnMARKGAP;

			CRect	rc;
			rc.SetRect(pointEPR.x - rgnMARKGAP, pointEPR.y - rgnMARKGAP, 
				pointEPR.x + rgnMARKGAP, pointEPR.y + rgnMARKGAP);
			pDC->Rectangle(&rc);
		}
		pDC->SetROP2(sROP);
		pDC->SelectObject(srgnBrush);
	}

//	DrawTickIndex(pDC, "가격차트", lcolor, true);
	m_RgnInfo.iCount++;

	delete[] pRgnPoint;
	pRgnPoint = NULL;

	DrawMinMaxDay(pDC);
}

void CBJugaChart::DrawBongPole(CDC* pDC)
{
	int	MaxPrc = INT_MIN;
	int	MinPrc = INT_MAX;
	bool	bMaxUpdate = false;
	bool	bMinUpdate = false;

	int	rgncnt = 0;
	CPoint	*pRgnPoint;

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return;

	int ii = 0;
	CGrpBasic* gBasic, *rBasic;
	for (ii = 0; ii < m_iDispEPos - m_iDispSPos; ii++)
	{
		gBasic = pOrgData->GetGraphData(ii+m_iDispSPos);
		if (gBasic->m_iClose != GD_UNUSED)
			break;
	}
	int	nPoint = m_iDispEPos - m_iDispSPos - ii;
	if (nPoint <= 0)
		return;

	pRgnPoint = new CPoint[nPoint * 2];

	COLORREF	upColor = m_pgraph->cInfo.aline[1].rgbLColor;
	COLORREF	dnColor = m_pgraph->cInfo.aline[2].rgbLColor;
	
	CPen	*upPen = GetPen(PS_SOLID, 1, upColor);
	CPen	*dnPen = GetPen(PS_SOLID, 1, dnColor);
	CPen	*sPen = pDC->SelectObject(upPen);

	int DrawHeight = m_rectGrp.Height();
	int DrawWidth  = m_rectGrp.Width();

	bool	bLog = IsLogChart();
	bool	bReverse = IsReverseChart();
	
	double	Max = m_dYMax;
	double	Min = m_dYMin;
	if (bLog)
	{
		Max = GetLogData(Max);
		Min = GetLogData(Min);
	}
	double ValueHeight = Max - Min;
	if (!ValueHeight) ValueHeight = 1;

	double hpr, lpr, epr, fpr, pepr = GD_UNUSED;
	CPoint pointHPR, pointLPR, pointEPR;
	int drawShift = int((DrawWidth/m_iDispDtCnt) / 2);	// Center

	ZeroMemory(&m_gindexMaxDay, sz_gIndex);
	ZeroMemory(&m_gindexMinDay, sz_gIndex);

	double	dValue;
	bool	bUp = false;
	for (; ii < m_iDispEPos - m_iDispSPos; ii++)
	{
		gBasic = pOrgData->GetGraphData(ii+m_iDispSPos);
		hpr = gBasic->m_iHigh; lpr = gBasic->m_iLow; epr = gBasic->m_iClose; fpr = gBasic->m_iOpen;

		if (fpr > epr)
			bUp = false;
		else if (fpr < epr)
			bUp = true;
		else
		{
			bool	bDecided = false;

			for (int jj = ii+m_iDispSPos - 1; jj >= 0 ; jj-- )
			{
				rBasic = pOrgData->GetGraphData(jj);

				if (rBasic->m_iClose == GD_UNUSED)
				{
					bDecided = false;
					break;
				}
				else
				{
					if (rBasic->m_iClose == epr)
					{
						bDecided = false;
					}
					else
					{
						bDecided = true;
						if (rBasic->m_iClose > epr)
							bUp = false;
						else
							bUp = true;
					}
				}
			}

			if (!bDecided)
				bUp = true;
		}

		if (bLog)
		{
			fpr = GetLogData(fpr);
			hpr = GetLogData(hpr);
			lpr = GetLogData(lpr);
			epr = GetLogData(epr);			
		}

		if (bUp)
			pDC->SelectObject(upPen);
		else
			pDC->SelectObject(dnPen);

		if (gBasic->m_iHigh > MaxPrc)
		{
			bMaxUpdate = true;
			m_iDispMaxVal = MaxPrc = gBasic->m_iHigh;
		}
		if (gBasic->m_iLow < MinPrc)
		{
			bMinUpdate = true;
			m_iDispMinVal = MinPrc = gBasic->m_iLow;
		}

		pointHPR.x = m_rectGrp.left + drawShift;
		pointHPR.x += int(double(DrawWidth * ii) / double(m_iDispDtCnt));
		pointLPR.x = pointHPR.x;

		pointEPR.x = m_rectGrp.left + drawShift;
		pointEPR.x += int(double(DrawWidth * (ii)) / double(m_iDispDtCnt));
		pointEPR.x -= DISTANCE_GAP;

		if (bReverse)
		{
			pointHPR.y = m_rectGrp.bottom;
			dValue = double(DrawHeight * (Max - hpr)) / double(ValueHeight);
			pointHPR.y -= int(dValue);

			pointLPR.y = m_rectGrp.bottom;
			dValue = double(DrawHeight * (Max - lpr)) / double(ValueHeight);
			pointLPR.y -= int(dValue);

			pointEPR.y = m_rectGrp.bottom;
			dValue = double(DrawHeight * (Max - epr)) / double(ValueHeight);
			pointEPR.y -= int(dValue);
		}
		else
		{
			pointHPR.y = m_rectGrp.top;
			dValue = double(DrawHeight * (Max - hpr)) / double(ValueHeight);
			pointHPR.y += int(dValue);

			pointLPR.y = m_rectGrp.top;
			dValue = double(DrawHeight * (Max - lpr)) / double(ValueHeight);
			pointLPR.y += int(dValue);

			pointEPR.y = m_rectGrp.top;
			dValue = double(DrawHeight * (Max - epr)) / double(ValueHeight);
			pointEPR.y += int(dValue);
		}

		if (hpr == lpr)
		{
			if (bUp)
				pDC->SetPixel(pointHPR, upColor);
			else
				pDC->SetPixel(pointHPR, dnColor);
		}
		else
		{
			pDC->MoveTo(pointHPR);
			pDC->LineTo(pointLPR);
		}

		pRgnPoint[rgncnt] = pointLPR;
		pRgnPoint[nPoint*2 - rgncnt - 1] = pointHPR;
		rgncnt++;

		if (bMaxUpdate)
		{
			bMaxUpdate = false;
			m_ptMaxDayPos = pointHPR;
			CopyMemory(&m_gindexMaxDay, &gBasic->m_index, sz_gIndex);
		}
		if (bMinUpdate)
		{
			bMinUpdate = false;
			m_ptMinDayPos = pointLPR;
			CopyMemory(&m_gindexMinDay, &gBasic->m_index, sz_gIndex);
		}

		if (gBasic->m_dwCheck)
			DisplayCheckBits(pDC, pointEPR, gBasic->m_dwCheck, gBasic->m_wRate);
	}
	pDC->SelectObject(sPen);

	if (m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn)
	{
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->DeleteObject();
		delete m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn;
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = NULL;
	}

	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = new CRgn;
	if (rgncnt == 0)
	{
		pRgnPoint[0] = CPoint(0,0);
		pRgnPoint[1] = CPoint(1,1);
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreatePolygonRgn(pRgnPoint, 2, WINDING);
	}
	else
	{
		for (int jj = 0; jj < rgncnt; jj++)
		{
			pRgnPoint[rgncnt*2 - jj - 1] = pRgnPoint[jj];
			pRgnPoint[jj].y += rgnLINEGAP;
			pRgnPoint[rgncnt*2 - jj - 1].y -= rgnLINEGAP;
		}
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreatePolygonRgn(pRgnPoint, rgncnt * 2, WINDING);
	}
	
	if (m_RgnInfo.iSelect >= 0 && m_rectGrp.Height() > 10)
	{
		int	sROP = pDC->SetROP2(R2_XORPEN);
		CBrush	*rgnBrush = GetBrush(_L_WHITE);
		CBrush	*srgnBrush = pDC->SelectObject(rgnBrush);
		for (int ii = 0; ii < 4; ii++)
		{
			int idx = (rgncnt) * (ii+1)/5;

			pointEPR.x = pRgnPoint[idx].x;
			pointEPR.y = pRgnPoint[idx].y - rgnMARKGAP;

			CRect	rc;
			rc.SetRect(pointEPR.x - rgnMARKGAP, pointEPR.y - rgnMARKGAP, 
				pointEPR.x + rgnMARKGAP, pointEPR.y + rgnMARKGAP);
			pDC->Rectangle(&rc);
		}
		pDC->SetROP2(sROP);
		pDC->SelectObject(srgnBrush);
	}

//	DrawTickIndex(pDC, "가격차트", m_pgraph->rgbLabel, true);
	m_RgnInfo.iCount++;

	delete[] pRgnPoint;
	pRgnPoint = NULL;

	DrawMinMaxDay(pDC);
}

bool CBJugaChart::CalculateMinMax(bool bForce)
{
	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return false;

	int	cKind = cPRC;
	int	gKind = GetGraphKind();
	switch(gKind)
	{
	case GK_JPN:
	case GK_BAR:
	case GK_LIN:
	case GK_POLE:
		{
			int index = pOrgData->GetIndex();
			int gap = pOrgData->GetDataGap();
			if (index == GI_TICK && gap <= 1)
				cKind = cLIN;
			else if (gKind == GK_LIN)
				cKind = cLIN;
		}
	}

	bool	bRet = false;
	m_dYMin = DBL_MAX;	m_dYMax = DBL_MIN;
	if (m_dwDspOpt & DO_SCREEN) {
		bRet = pOrgData->CalculateMinMax(m_iDispSPos, m_iDispEPos, &m_dYMin, &m_dYMax, 
		m_pgraph->wCalOption, cKind, bForce);
	}
	else {
		//Debug("Count = %d, %d", pOrgData->GetArraySize(), m_iTtlDtCnt);
		bRet = pOrgData->CalculateMinMax(0, m_iTtlDtCnt, &m_dYMin, &m_dYMax,
		m_pgraph->wCalOption, cKind, bForce);
	}

	if (m_bEqui)
	{
		if (!CalculateVolMinMax())
			return false;

		CalculateVol();
	}

	ReviseMinMax(m_dYMax, m_dYMin);

	return bRet;
}

bool CBJugaChart::IsChangeMinMaxRTM(bool bShift, bool bIncrease)
{
	double	svMax = m_dYMax;
	double  svMin = m_dYMin;

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return false;

	pOrgData->CalculateLastIndex(m_pgraph->wCalOption, cPRC);

	if (!CalculateMinMax(false))
		return false;

	if ((svMax != m_dYMax) || (svMin != m_dYMin))
		return true;

	return false;
}

int CBJugaChart::GetDisplayPosIndex(CPoint pt)
{
	int	retIdx = -1;
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)
		return retIdx;

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData) || pOrgData->GetArraySize() <= 0)
		return retIdx;

	if (!m_bEqui)
	{		int DrawWidth = m_rectGrp.Width();
		int xPosition = pt.x - m_rectGrp.left;

		double szOneDay = double(DrawWidth) / double(m_iDispDtCnt);
		int dataPos = int(double(xPosition) / szOneDay);
		dataPos += m_iDispSPos;
		if (dataPos < 0)	return -1;
		if (dataPos >= m_iTtlDtCnt)	dataPos = m_iTtlDtCnt-1;

		CGrpBasic* gBasic = pOrgData->GetGraphData(dataPos);
		
		if (gBasic->m_iClose == GD_UNUSED)	return -1;
		retIdx = m_iTtlDtCnt - dataPos;
	}
	else
	{
		int DrawWidth  = m_rectGrp.Width();
		if (m_dTotal == 0.0 || !m_piVol)	return -1;

		double VolWidth = (double)DrawWidth / m_dTotal;
		CRect	rc;

		double	right = m_rectGrp.left;
		for (int ii = 0; ii < m_iDispEPos - m_iDispSPos; ii++)
		{
			rc.left = int(right);
			right += m_piVol[ii] * VolWidth;
			rc.right = int(right) - DISTANCE_GAP;

			if (pt.x < rc.left || pt.x > rc.right)
				continue;

			CGrpBasic *gBasic = pOrgData->GetGraphData(ii + m_iDispSPos);
			
			if (gBasic->m_iClose == GD_UNUSED)	return -1;
			retIdx = m_iTtlDtCnt - (ii + m_iDispSPos);
		}
	}

	return retIdx;
}

CString CBJugaChart::GetDisplayPosHeader(CPoint pt, bool bGrid)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)
		return _T("");

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData) || pOrgData->GetArraySize() <= 0)
		return "";

	if (!m_bEqui)
	{
		int DrawWidth = m_rectGrp.Width();
		int xPosition = pt.x - m_rectGrp.left;

		double szOneDay = double(DrawWidth) / double(m_iDispDtCnt);
		int dataPos = int(double(xPosition) / szOneDay);
		dataPos += m_iDispSPos;
		if (dataPos < 0)
			return _T("");
		if (dataPos >= m_iTtlDtCnt)
			dataPos = m_iTtlDtCnt-1;

		CString	unitStr1 = _T("");

		CGrpBasic* gBasic = pOrgData->GetGraphData(dataPos);
		if (bGrid)
		{
			switch (GetDataIndex())
			{
			case GI_MINUTE:
			case GI_TICK:
				if (gBasic->m_index.date.dd == 0)
					unitStr1.Format("~~시간:%02d:%02d:%02d\t", gBasic->m_index.time.hh,
									gBasic->m_index.time.mm, gBasic->m_index.time.ss);
				else
					unitStr1.Format("~~시간:%02d/%02d-%02d:%02d:%02d\t", 
						gBasic->m_index.date.mm, gBasic->m_index.date.dd,
						gBasic->m_index.time.hh, gBasic->m_index.time.mm, gBasic->m_index.time.ss);
				break;
			default:
				unitStr1.Format("~~일자:%04d/%02d/%02d\t", gBasic->m_index.date.yy,
					gBasic->m_index.date.mm, gBasic->m_index.date.dd);
				break;
			}
		}
		else
		{
			switch (GetDataIndex())
			{
			case GI_MINUTE:
			case GI_TICK:
				unitStr1.Format("시간\t%02d:%02d:%02d\t", gBasic->m_index.time.hh,
					gBasic->m_index.time.mm, gBasic->m_index.time.ss);
				break;
			default:
				unitStr1.Format("일자\t%04d/%02d/%02d\t", gBasic->m_index.date.yy,
					gBasic->m_index.date.mm, gBasic->m_index.date.dd);
				break;
			}
		}

		CString	sFpr, sHpr, sLpr, sEpr;

		if (gBasic->m_iClose == GD_UNUSED)
		{
			return _T("");
		}

		return (unitStr1);
	}
	else
	{
		int DrawWidth  = m_rectGrp.Width();
		if (m_dTotal == 0.0 || !m_piVol)
			return _T("");
		double VolWidth = (double)DrawWidth / m_dTotal;
		CString	sTrn;
		CRect	rc;

		int ii = 0;
		double	right = m_rectGrp.left;
		CString	unitStr1 = _T("");
		CString	sFpr, sHpr, sLpr, sEpr;
		for (ii = 0; ii < m_iDispEPos - m_iDispSPos; ii++)
		{
			CGrpBasic* gBasic = pOrgData->GetGraphData(ii + m_iDispSPos);
			if (gBasic->m_iClose != GD_UNUSED)
				break;
		}

		for (; ii < m_iDispEPos - m_iDispSPos; ii++)
		{
			rc.left = int(right);
			right += m_piVol[ii] * VolWidth;
			rc.right = int(right) - DISTANCE_GAP;

			if (pt.x < rc.left || pt.x > rc.right)
				continue;

			CGrpBasic* gBasic = pOrgData->GetGraphData(ii + m_iDispSPos);
			if (bGrid)
			{
				switch (GetDataIndex())
				{
				case GI_MINUTE:
				case GI_TICK:
					if (gBasic->m_index.date.dd == 0)
						unitStr1.Format("~~시간:%02d:%02d:%02d\t", gBasic->m_index.time.hh,
							gBasic->m_index.time.mm, gBasic->m_index.time.ss);
					else
						unitStr1.Format("~~시간:%02d/%02d-%02d:%02d:%02d\t", 
							gBasic->m_index.date.mm, gBasic->m_index.date.dd,
							gBasic->m_index.time.hh, gBasic->m_index.time.mm, gBasic->m_index.time.ss);

					break;
				default:
					unitStr1.Format("~~일자:%04d/%02d/%02d\t", gBasic->m_index.date.yy,
						gBasic->m_index.date.mm, gBasic->m_index.date.dd);
					break;
				}
			}
			else
			{
				switch (GetDataIndex())
				{
				case GI_MINUTE:
				case GI_TICK:
					unitStr1.Format("시간\t%02d:%02d:%02d\t", gBasic->m_index.time.hh,
									gBasic->m_index.time.mm, gBasic->m_index.time.ss);
					break;
				default:
					unitStr1.Format("일자\t%04d/%02d/%02d\t", gBasic->m_index.date.yy,
						gBasic->m_index.date.mm, gBasic->m_index.date.dd);
					break;
				}
			}

			if (gBasic->m_iClose == GD_UNUSED)
			{
				return _T("");
			}

			return unitStr1;
		}
	}

	return _T("");
}

CString CBJugaChart::GetDisplayPosData(CPoint pt, bool bGrid)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)
		return _T("");

	class COrgData* pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData) || pOrgData->GetArraySize() <= 0)
		return "";

	CString	strUnit1 = _T("");
	CString	strUnit2 = _T("");
	CString strOpenP;
	CString strHighP;
	CString strLowP;
	CString strCloseP;
	CString strVolP;

	if (!m_bEqui)
	{
		int iDrawWidth = m_rectGrp.Width();
		int iXPosition = pt.x - m_rectGrp.left;

		double dOneDayWidth = double(iDrawWidth) / double(m_iDispDtCnt);
		int iDataPos = int(double(iXPosition) / dOneDayWidth);
		iDataPos += m_iDispSPos;
		if (iDataPos < 0)
			return _T("");
		if (iDataPos >= m_iTtlDtCnt)
			iDataPos = m_iTtlDtCnt - 1;


		CGrpBasic* gBasic = pOrgData->GetGraphData(iDataPos);

		// added by schbang 2006.03.16, calculate percent
		if (iDataPos - 1 >= 0)
		{
			CGrpBasic* pgbPrev = pOrgData->GetGraphData(iDataPos - 1);
			if (pgbPrev->m_iClose != GD_UNUSED)
			{
				if (pgbPrev->m_iClose == 0)
				{
					strOpenP = "(0.00%)";
					strHighP = "(0.00%)";
					strLowP = "(0.00%)";
					strCloseP = "(0.00%)";
				}
				else
				{
					double dPcnt = (double)(gBasic->m_iOpen - pgbPrev->m_iClose) / (double)pgbPrev->m_iClose * 100;
					dPcnt < 0 ? strOpenP.Format(" (%0.2f%%)", dPcnt) : strOpenP.Format(" (+%0.2f%%)", dPcnt);

					dPcnt = (double)(gBasic->m_iHigh - pgbPrev->m_iClose) / (double)pgbPrev->m_iClose * 100;
					dPcnt < 0 ? strHighP.Format(" (%0.2f%%)", dPcnt) : strHighP.Format(" (+%0.2f%%)", dPcnt);

					dPcnt = (double)(gBasic->m_iLow - pgbPrev->m_iClose) / (double)pgbPrev->m_iClose * 100;
					dPcnt < 0 ? strLowP.Format(" (%0.2f%%)", dPcnt) : strLowP.Format(" (+%0.2f%%)", dPcnt);

					dPcnt = (double)(gBasic->m_iClose - pgbPrev->m_iClose) / (double)pgbPrev->m_iClose * 100;
					dPcnt < 0 ? strCloseP.Format(" (%0.2f%%)", dPcnt) : strCloseP.Format(" (+%0.2f%%)", dPcnt);
				}

				if (pgbPrev->m_dVolume  == 0)
				{
					strVolP = "(0.00%)";
				}
				else
				{
					double dPcnt = (double)(gBasic->m_dVolume - pgbPrev->m_dVolume) / (double)pgbPrev->m_dVolume * 100;
					dPcnt < 0 ? strVolP.Format(" (%0.2f%%)", dPcnt) : strVolP.Format(" (+%0.2f%%)", dPcnt);
				}
				

				int iMaxLen = strOpenP.GetLength();
				if (iMaxLen < strHighP.GetLength())
					iMaxLen = strHighP.GetLength();
				if (iMaxLen < strLowP.GetLength())
					iMaxLen = strLowP.GetLength();
				if (iMaxLen < strCloseP.GetLength())
					iMaxLen = strCloseP.GetLength();

				CString strSpace = "          ";
				strOpenP = strSpace.Left(iMaxLen - strOpenP.GetLength()) + strOpenP;
				strHighP = strSpace.Left(iMaxLen - strHighP.GetLength()) + strHighP;
				strLowP = strSpace.Left(iMaxLen - strLowP.GetLength()) + strLowP;
				strCloseP = strSpace.Left(iMaxLen - strCloseP.GetLength()) + strCloseP;

			}
		}

		CString	sFpr, sHpr, sLpr, sEpr;
		CString	strVol;	// Added by 2006.03.14 schbang
		if (gBasic->m_iClose == GD_UNUSED)
		{
			return _T("");
		}
		else
		{
			FormatVariableComma(sFpr, gBasic->m_iOpen, gBasic->m_iOpen);
			FormatVariableComma(sHpr, gBasic->m_iHigh, gBasic->m_iHigh);
			FormatVariableComma(sLpr, gBasic->m_iLow, gBasic->m_iLow);
			FormatVariableComma(sEpr, gBasic->m_iClose, gBasic->m_iClose);
			int iValidDigit = m_iValidDigit;
			m_iValidDigit = 0;

			//if (m_iUnit == GU_INDEX || (m_iUnit != GU_INDEX && gBasic->m_dVolume < 1000000))
			if (m_iUnit == GU_INDEX)
				FormatVariableComma(strVol, gBasic->m_dVolume, 1, 14);	// Added by 2006.03.14 schbang
			else
			{
				double dVol = gBasic->m_dVolume;
				if (gBasic->m_dVolume >= dispMUNIT)
				{
					dVol = (double)(int)(dVol / 1000000);
					FormatVariableComma(strVol, dVol, 1, 14);
					strVol += "백만";
				}
				else if (gBasic->m_dVolume >= dispUNIT)
				{
					dVol = (double)(int)(dVol / 1000);
					FormatVariableComma(strVol, dVol, 1, 14);
					strVol += "천";
				}
				else
					FormatVariableComma(strVol, gBasic->m_dVolume, 1, 14);

			}
			m_iValidDigit = iValidDigit;

			if (m_iUnit == GU_INDEX)
			{
				//strVol += 'K';
				strVol += "천";
			}
			
			if (bGrid)
			{
				if (GetDataIndex() == GI_TICK || iDataPos == 0)
				{
					strUnit1.Format("시가:%s\t고가:%s\t저가:%s\t$$종가:%s\t$$거래량:%s", sFpr + strOpenP, sHpr + strHighP, sLpr + strLowP, sEpr + strCloseP, strVol + strVolP);
				}
				else
				{
					CGrpBasic* pBasic = pOrgData->GetGraphData(iDataPos - 1);
					if (pBasic->m_iClose == GD_UNUSED)
					{
						strUnit1.Format("시가:%s\t고가:%s\t저가:%s\t$$종가:%s\t$$거래량:%s", sFpr + strOpenP, sHpr + strHighP, sLpr + strLowP, sEpr + strCloseP, strVol + strVolP);
					}
					else
					{
						CString	sUdyl;
						double	dUdyl = double(gBasic->m_iClose - pBasic->m_iClose)/double(pBasic->m_iClose)*100.0;
						if (dUdyl > 0.0)
							sUdyl.Format("+%.2f%%", dUdyl);
						else if (dUdyl < 0.0)
							sUdyl.Format("%.2f%%", dUdyl);
						else
							sUdyl.Format(" 0.00%%");

						strUnit1.Format("시가:%s\t고가:%s\t저가:%s\t$$종가:%s\t##등락율:%s\t$$거래량:%s", sFpr + strOpenP, sHpr + strHighP, sLpr + strLowP, sEpr + strCloseP, sUdyl, strVol + strVolP);
					}
				}
			}
			else
			{
				strUnit1.Format("시가\t%s\t고가\t%s\t저가\t%s\t종가\t%s\t거래량\t%s", sFpr + strOpenP, sHpr + strHighP, sLpr + strLowP, sEpr + strCloseP, strVol + strVolP);
			}
		}

		if (gBasic->m_dwCheck > 0 && bGrid)
		{
			strUnit2 = GetExtraInfo(gBasic->m_dwCheck, gBasic->m_wRate, true);
		}

	}
	else
	{
		int DrawWidth  = m_rectGrp.Width();
		if (m_dTotal == 0.0 || !m_piVol)
			return _T("");
		double VolWidth = (double)DrawWidth / m_dTotal;
		CString	sTrn;
		CRect	rc;

		int ii = 0;
		double	right = m_rectGrp.left;
		CString	sFpr, sHpr, sLpr, sEpr;
		CString strVol;		// Added by 2006.03.14 schbang
		for (ii = 0; ii < m_iDispEPos - m_iDispSPos; ii++)
		{
			CGrpBasic	*gBasic = pOrgData->GetGraphData(ii + m_iDispSPos);
			if (gBasic->m_iClose != GD_UNUSED)
				break;
		}

		for (; ii < m_iDispEPos - m_iDispSPos; ii++)
		{
			rc.left = int(right);
			right += m_piVol[ii] * VolWidth;
			rc.right = int(right) - DISTANCE_GAP;

			if (pt.x < rc.left || pt.x > rc.right)
				continue;

			CGrpBasic* gBasic = pOrgData->GetGraphData(ii + m_iDispSPos);
			if (gBasic->m_iClose == GD_UNUSED)
			{
				return _T("");
			}
			else
			{
				// added by schbang 2006.03.17, calculate percent
				if (ii + m_iDispSPos - 1 >= 0)
				{
					CGrpBasic* pgbPrev = pOrgData->GetGraphData(ii + m_iDispSPos - 1);
					if (pgbPrev->m_iClose != GD_UNUSED)
					{
						double dPcnt = (double)(pgbPrev->m_iOpen - gBasic->m_iOpen) / (double)pgbPrev->m_iOpen * 100;
						dPcnt < 0 ? strOpenP.Format("  (%0.2f%%)", dPcnt) : strOpenP.Format("  (+%0.2f%%)", dPcnt);

						dPcnt = (double)(pgbPrev->m_iHigh - gBasic->m_iHigh) / (double)pgbPrev->m_iHigh * 100;
						dPcnt < 0 ? strHighP.Format("  (%0.2f%%)", dPcnt) : strHighP.Format("  (+%0.2f%%)", dPcnt);

						dPcnt = (double)(pgbPrev->m_iLow - gBasic->m_iLow) / (double)pgbPrev->m_iLow * 100;
						dPcnt < 0 ? strLowP.Format("  (%0.2f%%)", dPcnt) : strLowP.Format("  (+%0.2f%%)", dPcnt);

						dPcnt = (double)(pgbPrev->m_iClose - gBasic->m_iClose) / (double)pgbPrev->m_iClose * 100;
						dPcnt < 0 ? strCloseP.Format("  (%0.2f%%)", dPcnt) : strCloseP.Format("  (+%0.2f%%)", dPcnt);

						dPcnt = (double)(pgbPrev->m_dVolume - gBasic->m_dVolume) / (double)pgbPrev->m_dVolume * 100;
						dPcnt < 0 ? strVolP.Format("  (%0.2f%%)", dPcnt) : strVolP.Format("  (+%0.2f%%)", dPcnt);
					}			
				}

				FormatVariableComma(sFpr, gBasic->m_iOpen, gBasic->m_iOpen);
				FormatVariableComma(sHpr, gBasic->m_iHigh, gBasic->m_iHigh);
				FormatVariableComma(sLpr, gBasic->m_iLow, gBasic->m_iLow);
				FormatVariableComma(sEpr, gBasic->m_iClose, gBasic->m_iClose);
				FormatVariableComma(strVol, gBasic->m_dVolume, gBasic->m_dVolume);	// Added by 2006.03.14 schbang

				if (bGrid)
				{
					if (GetDataIndex() == GI_TICK || ii + m_iDispSPos == 0)
					{
						strUnit1.Format("시가:%s\t고가:%s\t저가:%s\t$$종가:%s\t거래량:%s", sFpr + strOpenP, sHpr + strHighP, sLpr + strLowP, sEpr + strCloseP, strVol + strVolP);
					}
					else
					{
						CGrpBasic *pBasic = pOrgData->GetGraphData(ii + m_iDispSPos - 1);
						if (pBasic->m_iClose == GD_UNUSED)
						{
							strUnit1.Format("시가:%s\t고가:%s\t저가:%s\t$$종가:%s\t거래량:%s", sFpr + strOpenP, sHpr + strHighP, sLpr + strLowP, sEpr + strCloseP, strVol + strVolP);
						}
						else
						{
							CString	sUdyl;
							double	dUdyl = double(gBasic->m_iClose - pBasic->m_iClose)/double(pBasic->m_iClose)*100.0;
							if (dUdyl > 0.0)
								sUdyl.Format("+%.2f%%", dUdyl);
							else if (dUdyl < 0.0)
								sUdyl.Format("%.2f%%", dUdyl);
							else
								sUdyl.Format(" 0.00%%");

							strUnit1.Format("시가:%s\t고가:%s\t저가:%s\t$$종가:%s\t##등락율:%s\t거래량:%s", sFpr + strOpenP, sHpr + strHighP, sLpr + strLowP, sEpr + strCloseP, sUdyl, strVol + strVolP);
						}
					}
				}
				else
				{
					strUnit1.Format("시가\t%s\t고가\t%s\t저가\t%s\t종가\t%s\t거래량\t%s", sFpr + strOpenP, sHpr + strHighP, sLpr + strLowP, sEpr + strCloseP, strVol + strVolP);
				}
			}
			if (gBasic->m_dwCheck > 0 && bGrid)
			{
				strUnit2 = GetExtraInfo(gBasic->m_dwCheck, gBasic->m_wRate, true);
			}
		}
	}

	return (strUnit1 + strUnit2);
}

void CBJugaChart::DrawMinMaxDay(CDC* pDC)
{
	if (!( m_dwDspOpt & DO_MINMAX))
		return;

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return;

	if (m_rectGrp.Height() < int(m_TkInfo.tkRect[TICK_BOTTOM].Height()))
		return;

	if ((!m_gindexMaxDay.date.yy && !m_gindexMaxDay.date.mm && !m_gindexMaxDay.date.dd) &&
		!(m_gindexMaxDay.time.hh))
		return;

	if (IsReverseChart())
	{
		m_ptMinDayPos.y -= 5;
		m_ptMaxDayPos.y += 5;
	}
	else
	{
		m_ptMinDayPos.y += 5;
		m_ptMaxDayPos.y -= 5;
	}

	CString	valstr, dispStr;
	CRect	tkRect;

	COLORREF	upColor = m_crUpSise;//_UPCOLOR;
	COLORREF	dnColor = m_crDnSise;//_DNCOLOR;

	CPen	*uPen = GetPen(PS_SOLID, 1, upColor);
	CPen	*dPen = GetPen(PS_SOLID, 1, dnColor);
	CPen	*sPen = pDC->SelectObject(uPen);
	
	CFont	*tkFont = GetFont(TICK_BOTTOM);
	CFont	*sFont = pDC->SelectObject(tkFont);
	
	int	sMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF sColor = pDC->SetTextColor(upColor);

	double	dCurr = pOrgData->GetCurr() * pow(10.0, m_iValidDigit);
	CString	strMaxRate = _T("");
	CString	strMinRate = _T("");

	if (dCurr > 0.0 || dCurr < 0.0)
		strMaxRate.Format("%.2f%% ", double(m_iDispMaxVal - dCurr)*100 / dCurr);
	if (dCurr > 0.0 || dCurr < 0.0)
		strMinRate.Format("%.2f%% ", double(m_iDispMinVal - dCurr)*100 / dCurr);

	CSize	size;
	CString	MaxDayStr, MinDayStr;
	int	rtmGap = pOrgData->GetDataGap();
	int	dIndex = pOrgData->GetIndex();
	bool	bReverse = IsReverseChart();
	if (dIndex == GI_TICK)
	{
		MaxDayStr.Format("(%s%02d:%02d:%02d)", strMaxRate,
			m_gindexMaxDay.time.hh, m_gindexMaxDay.time.mm, m_gindexMaxDay.time.ss);
		MinDayStr.Format("(%s%02d:%02d:%02d)", strMinRate,
			m_gindexMinDay.time.hh, m_gindexMinDay.time.mm, m_gindexMinDay.time.ss);
	}
	else if (dIndex == GI_MINUTE)
	{
		MaxDayStr.Format("(%s%02d:%02d)", strMaxRate,
			m_gindexMaxDay.time.hh, m_gindexMaxDay.time.mm);
		MinDayStr.Format("(%s%02d:%02d)", strMinRate,
			m_gindexMinDay.time.hh, m_gindexMinDay.time.mm);
	}
	else 
	{
		MaxDayStr.Format("(%s%d/%d)", strMaxRate,
			m_gindexMaxDay.date.mm, m_gindexMaxDay.date.dd);
		MinDayStr.Format("(%s%d/%d)", strMinRate,
			m_gindexMinDay.date.mm, m_gindexMinDay.date.dd);
	}

	if (int(m_TkInfo.tkHeight[TICK_BOTTOM]) * 2 < m_rectGrp.Height() + 2 &&
		int(m_TkInfo.tkHeight[TICK_BOTTOM])*2 < m_rectGrp.Width())
	{
		FormatVariableComma(valstr, m_iDispMaxVal, m_iDispMinVal, 10);
		dispStr = " 최고:" + valstr + MaxDayStr;

		size = pDC->GetOutputTextExtent(dispStr);
		if (m_ptMaxDayPos.y - size.cy/2 < m_rectGrp.top)
			m_ptMaxDayPos.y = m_rectGrp.top + size.cy/2;
		if (m_ptMinDayPos.y + size.cy/2 > m_rectGrp.bottom)
			m_ptMinDayPos.y = m_rectGrp.bottom - size.cy/2;

		tkRect.SetRect(m_ptMaxDayPos.x + 20, m_ptMaxDayPos.y - size.cy/2,
			m_ptMaxDayPos.x + 20 + size.cx, m_ptMaxDayPos.y + size.cy/2);

		if (tkRect.right > m_rectGrp.right)
		{
			tkRect.SetRect(m_ptMaxDayPos.x - 20 - size.cx, m_ptMaxDayPos.y - size.cy/2,
				m_ptMaxDayPos.x - 20, m_ptMaxDayPos.y + size.cy/2);

			pDC->MoveTo(m_ptMaxDayPos.x, m_ptMaxDayPos.y);
			pDC->LineTo(m_ptMaxDayPos.x - 4, m_ptMaxDayPos.y - 4);
			pDC->MoveTo(m_ptMaxDayPos.x - 3, m_ptMaxDayPos.y + 3);
			pDC->LineTo(m_ptMaxDayPos.x, m_ptMaxDayPos.y);
			pDC->LineTo(m_ptMaxDayPos.x - 20, m_ptMaxDayPos.y);
		}
		else
		{
			pDC->MoveTo(m_ptMaxDayPos.x, m_ptMaxDayPos.y);
			pDC->LineTo(m_ptMaxDayPos.x + 4, m_ptMaxDayPos.y - 4);
			pDC->MoveTo(m_ptMaxDayPos.x + 3, m_ptMaxDayPos.y + 3);
			pDC->LineTo(m_ptMaxDayPos.x, m_ptMaxDayPos.y);
			pDC->LineTo(m_ptMaxDayPos.x + 20, m_ptMaxDayPos.y);
		}

		if (tkRect.top < m_rectOrgGrp.top)
			tkRect.OffsetRect(0, m_rectOrgGrp.top - tkRect.top);
		if (tkRect.bottom > m_rectOrgGrp.bottom)
			tkRect.OffsetRect(0, m_rectOrgGrp.bottom - tkRect.bottom);

		pDC->DrawText(dispStr, &tkRect, DT_CENTER|DT_BOTTOM);

		pDC->SelectObject(dPen);
		pDC->SetTextColor(dnColor);

		FormatVariableComma(valstr, m_iDispMinVal, m_iDispMinVal, 10);
		dispStr = " 최저:" + valstr + MinDayStr;

		size = pDC->GetOutputTextExtent(dispStr);
		tkRect.SetRect(m_ptMinDayPos.x + 20, m_ptMinDayPos.y - size.cy/2,
			m_ptMinDayPos.x + 20 + size.cx, m_ptMinDayPos.y + size.cy/2);

		if (tkRect.right > m_rectGrp.right)
		{
			tkRect.SetRect(m_ptMinDayPos.x - 20 - size.cx, m_ptMinDayPos.y - size.cy/2,
				m_ptMinDayPos.x - 20, m_ptMinDayPos.y + size.cy/2);

			pDC->MoveTo(m_ptMinDayPos.x, m_ptMinDayPos.y);
			pDC->LineTo(m_ptMinDayPos.x - 4, m_ptMinDayPos.y - 4);
			pDC->MoveTo(m_ptMinDayPos.x - 3, m_ptMinDayPos.y + 3);
			pDC->LineTo(m_ptMinDayPos.x, m_ptMinDayPos.y);
			pDC->LineTo(m_ptMinDayPos.x - 20, m_ptMinDayPos.y);
		}
		else
		{
			pDC->MoveTo(m_ptMinDayPos.x, m_ptMinDayPos.y);
			pDC->LineTo(m_ptMinDayPos.x + 4, m_ptMinDayPos.y - 4);
			pDC->MoveTo(m_ptMinDayPos.x + 3, m_ptMinDayPos.y + 3);
			pDC->LineTo(m_ptMinDayPos.x, m_ptMinDayPos.y);
			pDC->LineTo(m_ptMinDayPos.x + 20, m_ptMinDayPos.y);
		}

		if (tkRect.top < m_rectOrgGrp.top)
			tkRect.OffsetRect(0, m_rectOrgGrp.top - tkRect.top);
		if (tkRect.bottom > m_rectOrgGrp.bottom)
			tkRect.OffsetRect(0, m_rectOrgGrp.bottom - tkRect.bottom);

		pDC->DrawText(dispStr, &tkRect, DT_CENTER|DT_TOP);
	}

	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
	pDC->SelectObject(sFont);
	pDC->SelectObject(sPen);
}

void CBJugaChart::DrawLegend(CDC* pDC)
{
	if (IsUnion())
		return;

	m_RgnInfo.iCount = 0;

	switch(m_iGrpKind)
	{
	case GK_JPN:
	case GK_EQV:
	case GK_CDV:
		DrawTickIndex(pDC, "가격차트", m_pgraph->rgbLabel, true);
		break;
	case GK_BAR:
		DrawTickIndex(pDC, "가격차트", m_pgraph->cInfo.aline[0].rgbLColor, true);
		break;
	case GK_LIN:
		DrawTickIndex(pDC, "가격차트", m_pgraph->cInfo.aline[0].rgbLColor, true);
		break;
	case GK_POLE:
		DrawTickIndex(pDC, "가격차트", m_pgraph->rgbLabel, true);
		break;
	}

	m_RgnInfo.iCount++;
	
}