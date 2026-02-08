// LineBase.cpp: implementation of the CLineBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LineBase.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLineBase::CLineBase(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
: CIndcBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	m_iOrgDataKey = m_iDataKey;
	class COrgData *pOrgData;
	if (m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
	{
		m_iTtlDtCnt = pOrgData->GetArraySize();
		if (dispDay == 0)
			m_iDispDtCnt = m_iTtlDtCnt;
		else if (m_iTtlDtCnt < dispDay)
			m_iDispDtCnt = m_iTtlDtCnt;
		else
			m_iDispDtCnt = dispDay;

		EditMemberValue();

		m_iDataKey = pDataMgr->CreateAppData(pOrgData, m_iDataKey, GetGraphKind(),
			&m_pgraph->cInfo, m_pgraph->wCalOption);
	}

	m_iValidDigit = 0;

	//** 2005.11.17 macho
	m_strDspUnit.Empty();
}

CLineBase::~CLineBase()
{
	if (m_iDataKey > 0)
		m_pDataMgr->DetachData(m_iDataKey);
}

CString CLineBase::GetYposValue(CPoint pt, bool bGrid)
{
	double  posValue = 0;
	CString	dispStr = _T("");

	if (pt.x >= m_rectGrp.right)
		pt.x = m_rectGrp.right-1;
	if (pt.x <= m_rectGrp.left)
		pt.x = m_rectGrp.left+1;

	if (!m_rectGrp.PtInRect(pt))
		return dispStr;
	if (m_dYMax == DBL_MIN || m_dYMin == DBL_MAX)
		return dispStr;

	double	Max = m_dYMax;
	double	Min = m_dYMin;

	int DrawHeight = m_rectGrp.Height();
	double  ValueHeight = Max - Min;

	posValue = double(ValueHeight) / double(DrawHeight);
	posValue *= double(m_rectGrp.bottom - double(pt.y));
	posValue = Min + posValue;

	FormatVariableComma(dispStr, posValue, Max, 9);

	return dispStr;
}

CString CLineBase::GetXposValue(CPoint pt)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)
		return _T("");

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iOrgDataKey, pOrgData) || pOrgData->GetArraySize() <= 0)
		return _T("");

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
	if (gBasic->m_iClose == GD_UNUSED)
		return _T("");

	switch (GetDataIndex())
	{
	case GI_MINUTE:
	case GI_TICK:
		unitStr1.Format("%02d/%02d %02d:%02d:%02d", 
			gBasic->m_index.date.mm, gBasic->m_index.date.dd,
			gBasic->m_index.time.hh, gBasic->m_index.time.mm, gBasic->m_index.time.ss);
		break;
	default:
		unitStr1.Format("%04d/%02d/%02d", 
			gBasic->m_index.date.yy, gBasic->m_index.date.mm, gBasic->m_index.date.dd);
		break;
	}

	return unitStr1;
}

void CLineBase::DrawLine(CDC* pDC, double dMax, double dMin, double adPtValue[], int iStyle, int iWidth, COLORREF color, int iIndex, int iOverFillTickIndex)
{
	// skip unusded
	int ii = 0;
	for (ii = 0; ii < m_iDispEPos - m_iDispSPos; ii++)
	{
		if (adPtValue[ii + m_iDispSPos] != GD_UNUSED)
			break;
	}
	
	int iPntCnt = m_iDispEPos - m_iDispSPos - ii;
	if (iPntCnt <= 0)
		return;

	CPoint*	paptLine = new CPoint[iPntCnt * 2];

	CPen* ppenNew = GetPen(iStyle, iWidth, color);
	CPen* ppenOld = pDC->SelectObject(ppenNew);

	int iDrawHeight = m_rectGrp.Height();
	int iDrawWidth  = m_rectGrp.Width();

	double dValueHeight = dMax - dMin;
	if (dValueHeight == 0)
		dValueHeight = 1;

	int iDrawShift = int((iDrawWidth / m_iDispDtCnt) / 2);	// Center

	int iPntIdx = 0;
	CPoint point;
	double	dVal;
	for (; ii < m_iDispEPos - m_iDispSPos; ii++)	// set x, y of point
	{
		dVal = adPtValue[ii + m_iDispSPos];
		
		point.x = m_rectGrp.left + iDrawShift;
		point.x += int((iDrawWidth * ii) / m_iDispDtCnt);

		point.y = m_rectGrp.top;
		point.y += int((iDrawHeight * (dMax - dVal)) / dValueHeight);			
		
		paptLine[iPntIdx++] = point;
	}

///*	// Added by schbang 2006.03.21
	if (iOverFillTickIndex > -1)
	{
		int iY;
		//CRgn rgn;
		CBrush*	pbrushRgn = (CBrush *) 0;
		if (m_pgraph->tInfo.abtFillOver[iOverFillTickIndex] == 1)
		{
			if (m_iOverBuyY <= m_rectGrp.top)
			{
				pDC->SelectObject(ppenOld);
				delete[] paptLine;
				return;
			}

			iY = m_iOverBuyY;
			//rgn.CreateRectRgn(m_rectGrp.left, m_rectGrp.top, m_rectGrp.right, iY);
			pDC->IntersectClipRect(m_rectGrp.left, m_rectGrp.top, m_rectGrp.right, iY);
			pbrushRgn = GetBrush(m_pgraph->tInfo.rgbOverBuy);
		}
		else if (m_pgraph->tInfo.abtFillOver[iOverFillTickIndex] == 2)
		{
			if (m_iOverSellY >= m_rectGrp.bottom)
			{
				pDC->SelectObject(ppenOld);
				delete[] paptLine;
				return;
			}

			iY = m_iOverSellY;
			//rgn.CreateRectRgn(m_rectGrp.left, iY + 1, m_rectGrp.right, m_rectGrp.bottom);
			pDC->IntersectClipRect(m_rectGrp.left, iY + 1, m_rectGrp.right, m_rectGrp.bottom);
			pbrushRgn = GetBrush(m_pgraph->tInfo.rgbOverSell);
		}
		else
		{
			pDC->SelectObject(ppenOld);
			delete[] paptLine;
			return;
		}

		//pDC->SelectClipRgn(&rgn);

		if (paptLine && iPntCnt > 1)
		{
			paptLine[iPntCnt].x = paptLine[iPntCnt - 1].x;
			paptLine[iPntCnt++].y = iY;
			paptLine[iPntCnt].x = paptLine[0].x;
			paptLine[iPntCnt++].y = iY;
		}
		
		CBrush* pbrushOld = pDC->SelectObject(pbrushRgn);
		pDC->Polygon(paptLine, iPntCnt);
		pDC->SelectObject(pbrushOld);
		pDC->SelectClipRgn(NULL);

		delete[] paptLine;
		pDC->SelectObject(ppenOld);

		return;
	}
//*/
	
	pDC->Polyline(paptLine, iPntCnt);
	pDC->SelectObject(ppenOld);

	if (m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn)	// remove prev rgn
	{
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->DeleteObject();
		delete m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn;
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = NULL;
	}

	m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn = new CRgn;
	if (iPntIdx == 0)	// if none point
	{
		paptLine[0] = CPoint(0, 0);
		paptLine[1] = CPoint(1, 1);
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreatePolygonRgn(paptLine, 2, WINDING);
		m_RgnInfo.Rgn[m_RgnInfo.iCount].iIndex = iIndex;
	}
	else	// make line region by double point
	{
		for (int jj = 0; jj < iPntIdx; jj++)
		{
			paptLine[iPntIdx * 2 - jj - 1] = paptLine[jj];
			paptLine[jj].y += rgnLINEGAP;
			paptLine[iPntIdx * 2 - jj - 1].y -= rgnLINEGAP;
		}
		m_RgnInfo.Rgn[m_RgnInfo.iCount].pRgn->CreatePolygonRgn(paptLine, iPntIdx * 2, WINDING);
	}

	if (m_RgnInfo.iSelect >= 0 && m_rectGrp.Height() > 10)	// when selected, display black points
	{
		int iRopOld = pDC->SetROP2(R2_XORPEN);
		CBrush*	pbrushRgn = GetBrush(_L_WHITE);
		CBrush* pbrushOld = pDC->SelectObject(pbrushRgn);
		for (int ii = 0; ii < 4; ii++)
		{
			int iDx = (iPntIdx) * (ii + 1) / 5;

			point.x = paptLine[iDx].x;
			point.y = paptLine[iDx].y - rgnMARKGAP;

			CRect	rc;
			rc.SetRect(point.x - rgnMARKGAP, point.y - rgnMARKGAP, 
				point.x + rgnMARKGAP, point.y + rgnMARKGAP);
			pDC->Rectangle(&rc);
		}
		pDC->SetROP2(iRopOld);
		pDC->SelectObject(pbrushOld);
	}

	delete[] paptLine;
	paptLine = NULL;
}

void CLineBase::DrawTickRight(CDC* pDC, double dMax, double dMin)
{
	if (!(m_pgraph->dwDraOption & GO_TKRIGHT))
	{
		TRACE("DrawTickRight Function Error\n");
		return;
	}

	int iDispTick = GetTickCount(TICK_RIGHT);
	if (iDispTick <= 0)
		return;

	CRect rectTick = m_TkInfo.tkRect[TICK_RIGHT];
	CRect rectOrgTick = rectTick;
	rectTick.left += 10;
	rectOrgTick.top = m_rectOrgGrp.top;

	int iTickHeight = rectTick.Height();
	double	dValueHeight = dMax - dMin;
	if (dValueHeight <= 0)
		dValueHeight = 1;
	
	double dDiv;
	if (dValueHeight < 5)			dDiv = 1;
	else if (dValueHeight < 20)		dDiv = 2;
	else if (dValueHeight < 50)		dDiv = 5;
	else if (dValueHeight < 100)		dDiv = 10;
	else if (dValueHeight < 200)		dDiv = 20;
	else if (dValueHeight < 500)		dDiv = 50;
	else if (dValueHeight < 1000)		dDiv = 100;
	else if (dValueHeight < 2000)		dDiv = 200;
	else if (dValueHeight < 5000)		dDiv = 500;
	else if (dValueHeight < 10000)		dDiv = 1000;
	else if (dValueHeight < 20000)		dDiv = 2000;
	else if (dValueHeight < 50000)		dDiv = 5000;
	else if (dValueHeight < 100000)		dDiv = 10000;
	else if (dValueHeight < 200000)		dDiv = 20000;
	else if (dValueHeight < 500000)		dDiv = 50000;
	else if (dValueHeight < 1000000)	dDiv = 100000;
	else if (dValueHeight < 2000000)	dDiv = 200000;
	else if (dValueHeight < 5000000)	dDiv = 500000;
	else if (dValueHeight < 10000000)	dDiv = 1000000;
	else if (dValueHeight < 20000000)	dDiv = 2000000;
	else if (dValueHeight < 50000000)	dDiv = 5000000;
	else if (dValueHeight < 100000000)	dDiv = 10000000;
	else if (dValueHeight < 200000000)	dDiv = 20000000;
	else if (dValueHeight < 500000000)	dDiv = 50000000;
	else if (dValueHeight < 1000000000)	dDiv = 100000000;
	else					dDiv = 500000000;

	// y축 설정값 적용 2006.07.03
	if (m_pgraph->yainfo.cMode == Y_FIT)
	{
		dDiv = dValueHeight / m_pgraph->yainfo.iFit;

		double dHogaDanWi = 0;
		if (dMin < 5000)
			dHogaDanWi = 5;
		else if (dMin < 10000)
			dHogaDanWi = 10;
		else if (dMin < 50000)
			dHogaDanWi = 50;
		else if (dMin < 100000)
			dHogaDanWi = 100;
		else if (dMin < 500000)
			dHogaDanWi = 500;
		else 
			dHogaDanWi = 1000;

		if (dDiv < dHogaDanWi)
			dDiv = dHogaDanWi;
		else if (dDiv > dHogaDanWi)
		{
			dDiv += dHogaDanWi / 2;
			dDiv /= dHogaDanWi;
			dDiv = (double)((int)(dDiv)) * dHogaDanWi;
		}
	}
	else if (m_pgraph->yainfo.cMode == Y_FIX)
		dDiv = m_pgraph->yainfo.dFix * 100;


	int iDispCount;
	double dDrawMinValue = 0.0;
	double adDispValue[64];
	double adLogValue[64];
	if (!CalculateDisplay(dDiv, dMax, dMin, dDrawMinValue, iDispTick, adDispValue, adLogValue, iDispCount))
		return;

	int iOldeMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF rgbOldText = pDC->SetTextColor(m_atick[TICK_RIGHT].dwTColor);
	int iLineStyle = m_lineH.btLStyle;
	int iLineWidth = m_lineH.btLWidth;
	COLORREF rgbLineColor = m_lineH.rgbLColor;

	
	CFont* pfontTick = GetFont(TICK_RIGHT);
	CFont* pfontOld = pDC->SelectObject(pfontTick);
	CPen* ppenTick = GetPen(iLineStyle, iLineWidth, rgbLineColor);
	CPen* ppenLine = GetPen(PS_SOLID, iLineWidth, m_crFG);
	CPen* ppenOld = pDC->SelectObject(ppenTick);

	CRect rectTickSave = rectTick;
	CSize size;
	int iMaxDigit = 7;
	char* apcDigitStr[10] = {	"2,000,000,000", "200,000,000", "20,000,000", 
					"2,000,000", "200,000", "20,000", "2,000", "200", "20", "2"};
	for (int ii = 9; ii >= 0; ii--)
	{
		size = pDC->GetOutputTextExtent(apcDigitStr[ii]);
		if (size.cx >= rectTickSave.Width())
			break;
		else
			iMaxDigit = strlen(apcDigitStr[ii]);
	}
	
	//bool bDrawLine = (m_dwDspOpt & DO_TKHGRID) ? true : false;
	bool bDrawLine = ((m_dwDspOpt & DO_TKHGRID) && !(m_pgraph->dwDraOption & GO_TICK_SKIP_HLINE)) ? true : false;

	rectTickSave.top = rectTick.bottom;

	int iMaxLen = 0;
	double dValue;
	CRect rectText;
	CString	strDisp;
	CPoint pointStart;
	CPoint pointEnd;
	CArray<CRect, CRect> arRect;
	CStringArray arString;
	for (int ii = 0; ii < iDispCount; ii++)		// 가격별가로 (DOT)
	{
		pointStart.x = m_rectOrgGrp.right;
		dValue = adDispValue[ii];
		dValue = double(double(iTickHeight) * double(dMax - dValue));
		dValue /= double(dValueHeight);

		pointStart.y = rectTick.top + int(dValue);

		if (!rectTick.PtInRect(pointStart))
		{
			if (rectTick.bottom < pointStart.y)
				continue;
		}

		pointEnd.x = m_rectOrgGrp.left+1;
		pointEnd.y = pointStart.y;
		if (bDrawLine && pointStart.y > m_rectChart.top && pointStart.y < m_rectChart.bottom)
		{
			// 수평선
			pDC->SelectObject(ppenTick);
			pDC->MoveTo(pointEnd);
			pDC->LineTo(pointStart);
		}

		// 눈금
		pDC->SelectObject(ppenLine);
		pDC->MoveTo(m_rectOrgGrp.right, pointStart.y);
		pDC->LineTo(m_rectOrgGrp.right + lenTICK, pointStart.y);

		if (ii == 0)
		{
			strDisp = GetEachUnit(dMin, dMax);

			if (!strDisp.IsEmpty())
			{
				size = pDC->GetOutputTextExtent(strDisp);
				rectText.SetRect(m_rectOrgGrp.right + lenTICK, rectTick.bottom - size.cy,
						rectTick.right, rectTick.bottom);
				rectText.IntersectRect(&rectText, &rectOrgTick);
				pDC->DrawText(strDisp, &rectText, DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_NOCLIP);
				rectTickSave = rectText;
				continue;
			}				
		}

		FormatVariableComma(strDisp, adDispValue[ii], dMax, iMaxDigit);

		size = pDC->GetOutputTextExtent(strDisp);
			
		rectText.SetRect(rectTick.left + gapTICK, pointStart.y - size.cy/2,
			rectTick.left + gapTICK + size.cx, pointStart.y + size.cy/2);

		if (rectText.bottom > rectTickSave.top)
			rectText.OffsetRect(0, rectTickSave.top - rectText.bottom);
		rectText.IntersectRect(&rectText, &rectOrgTick);
		arRect.Add(rectText);
		arString.Add(strDisp);

		if (size.cx > iMaxLen)
			iMaxLen = size.cx;
		
		rectTickSave = rectText;
	}

	for (int ii = 0; ii < arRect.GetSize(); ii++)
	{
		rectText = arRect.GetAt(ii);
		rectText.right = rectText.left + iMaxLen;
		pDC->DrawText(arString[ii], &rectText, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
	}
	
	m_aiRight[TICK_RIGHT] = rectText.right;

	pDC->SelectObject(pfontOld);
	pDC->SelectObject(ppenOld);
	pDC->SetTextColor(rgbOldText);
	pDC->SetBkMode(iOldeMode);
}


void CLineBase::DrawTickLeft(CDC* pDC, double dMax, double dMin)
{
	if (!(m_pgraph->dwDraOption & GO_TKLEFT))
	{
		TRACE("DrawTickLeft Function Error\n");
		return;
	}

	int iDispTick = GetTickCount(TICK_LEFT);
	if (iDispTick <= 0)
		return;

	CRect rectTick = m_TkInfo.tkRect[TICK_LEFT];
	CRect rectOrgTick = rectTick;
	rectTick.right -= 10;
	rectOrgTick.top = m_rectOrgGrp.top;

	int iTickHeight = rectTick.Height();
	double dValueHeight = dMax - dMin;
	if (dValueHeight <= 0)
		dValueHeight = 1;

	double dDiv;
	if (dValueHeight < 5)			dDiv = 1;
	else if (dValueHeight < 20)		dDiv = 2;
	else if (dValueHeight < 50)		dDiv = 5;
	else if (dValueHeight < 100)		dDiv = 10;
	else if (dValueHeight < 200)		dDiv = 20;
	else if (dValueHeight < 500)		dDiv = 50;
	else if (dValueHeight < 1000)		dDiv = 100;
	else if (dValueHeight < 2000)		dDiv = 200;
	else if (dValueHeight < 5000)		dDiv = 500;
	else if (dValueHeight < 10000)		dDiv = 1000;
	else if (dValueHeight < 20000)		dDiv = 2000;
	else if (dValueHeight < 50000)		dDiv = 5000;
	else if (dValueHeight < 100000)		dDiv = 10000;
	else if (dValueHeight < 200000)		dDiv = 20000;
	else if (dValueHeight < 500000)		dDiv = 50000;
	else if (dValueHeight < 1000000)	dDiv = 100000;
	else if (dValueHeight < 2000000)	dDiv = 200000;
	else if (dValueHeight < 5000000)	dDiv = 500000;
	else if (dValueHeight < 10000000)	dDiv = 1000000;
	else if (dValueHeight < 20000000)	dDiv = 2000000;
	else if (dValueHeight < 50000000)	dDiv = 5000000;
	else if (dValueHeight < 100000000)	dDiv = 10000000;
	else if (dValueHeight < 200000000)	dDiv = 20000000;
	else if (dValueHeight < 500000000)	dDiv = 50000000;
	else if (dValueHeight < 1000000000)	dDiv = 100000000;
	else					dDiv = 500000000;

	// y축 설정값 적용 2006.07.03
	if (m_pgraph->yainfo.cMode == Y_FIT)
	{
		dDiv = dValueHeight / m_pgraph->yainfo.iFit;

		double dHogaDanWi = 0;
		if (dMin < 5000)
			dHogaDanWi = 5;
		else if (dMin < 10000)
			dHogaDanWi = 10;
		else if (dMin < 50000)
			dHogaDanWi = 50;
		else if (dMin < 100000)
			dHogaDanWi = 100;
		else if (dMin < 500000)
			dHogaDanWi = 500;
		else 
			dHogaDanWi = 1000;

		if (dDiv < dHogaDanWi)
			dDiv = dHogaDanWi;
		else if (dDiv > dHogaDanWi)
		{
			dDiv += dHogaDanWi / 2;
			dDiv /= dHogaDanWi;
			dDiv = (double)((int)(dDiv)) * dHogaDanWi;
		}
	}
	else if (m_pgraph->yainfo.cMode == Y_FIX)
		dDiv = m_pgraph->yainfo.dFix * 100;

	int iDispCount;
	double adDispValue[64];
	double adLogValue[64];
	double drawMinValue= 0.0;
	if (!CalculateDisplay(dDiv, dMax, dMin, drawMinValue, iDispTick, adDispValue, adLogValue, iDispCount))
		return;

	int iOldMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF rgbOldText = pDC->SetTextColor(m_atick[TICK_LEFT].dwTColor);
	int iLineStyle = m_lineH.btLStyle;
	int iLineWidth = m_lineH.btLWidth;
	COLORREF rgbLineColor = m_lineH.rgbLColor;

	CRect rectText;
	CFont* pfontTick = GetFont(TICK_LEFT);
	CFont* pfontOld = pDC->SelectObject(pfontTick);
	CPen* ppenTick = GetPen(iLineStyle, iLineWidth, rgbLineColor);
	CPen* ppenLine = GetPen(PS_SOLID, iLineWidth, m_crFG);
	CPen* ppenOld = pDC->SelectObject(ppenTick);

	double	dValue;
	CString	strDisp;
	CPoint pointStart, pointEnd;
	CRect rectTickSave = rectTick;

	CSize size;
	int iMaxDigit = 7;
	char* apcDigitStr[10] = {	"2,000,000,000", "200,000,000", "20,000,000", 
					"2,000,000", "200,000", "20,000", "2,000", "200", "20", "2"};
	for (int ii = 9; ii >= 0; ii--)
	{
		size = pDC->GetOutputTextExtent(apcDigitStr[ii]);
		if (size.cx >= rectTickSave.Width())
			break;
		else
			iMaxDigit = strlen(apcDigitStr[ii]);
	}

	bool	bDrawLine = (m_dwDspOpt & DO_TKHGRID)? true:false;
	
	rectTickSave.top = rectTick.bottom;
	


	for (int ii = 0; ii < iDispCount; ii++)		// 가격별가로 (DOT)
	{
		pointStart.x = m_rectOrgGrp.left+1;
		dValue = adDispValue[ii];
		dValue = double(double(iTickHeight) * double(dMax - dValue));
		dValue /= double(dValueHeight);

		pointStart.y = rectTick.top + int(dValue);

		if (!rectTick.PtInRect(CPoint(pointStart.x -1, pointStart.y)))
		{
			if (rectTick.bottom < pointStart.y)
				continue;
		}

		pointEnd.x = m_rectOrgGrp.right;
		pointEnd.y = pointStart.y;
		if (bDrawLine && pointStart.y > m_rectChart.top && pointStart.y < m_rectChart.bottom)
		{
			pDC->SelectObject(ppenTick);
			pDC->MoveTo(pointStart);
			pDC->LineTo(pointEnd);
		}
		
		pDC->SelectObject(ppenLine);
		pDC->MoveTo(m_rectOrgGrp.left, pointStart.y);
		pDC->LineTo(m_rectOrgGrp.left - lenTICK, pointStart.y);

		if (ii == 0)
		{
			strDisp = GetEachUnit(dMin, dMax);

			if (!strDisp.IsEmpty())
			{
			
				size = pDC->GetOutputTextExtent(strDisp);
				rectText.SetRect(rectTick.left, rectTick.bottom - size.cy,
						m_rectOrgGrp.left - lenTICK, rectTick.bottom);
				rectText.IntersectRect(&rectText, &rectOrgTick);
				pDC->DrawText(strDisp, rectText, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
				rectTickSave = rectText;
				continue;
			}		
		}

		FormatVariableComma(strDisp, adDispValue[ii], dMax, iMaxDigit);

//		CString szRep;
//		szRep.Format(".%0*d", m_iValidDigit, 0);
//		strDisp.Replace(szRep, "");

		size = pDC->GetOutputTextExtent(strDisp);

		rectText.SetRect(rectTick.left, pointStart.y - size.cy/2,
			rectTick.right - gapTICK, pointStart.y + size.cy/2);

		if (rectText.bottom > rectTickSave.top)
			rectText.OffsetRect(0, rectTickSave.top - rectText.bottom);

		rectText.IntersectRect(&rectText, &rectOrgTick);
		pDC->DrawText(strDisp, rectText, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
		rectTickSave = rectText;
	}

	m_aiRight[TICK_LEFT] = rectText.right;
	pDC->SelectObject(pfontOld);
	pDC->SelectObject(ppenOld);
	pDC->SetTextColor(rgbOldText);
	pDC->SetBkMode(iOldMode);
}

bool CLineBase::DrawUserDefinedTick(CDC* pDC, double dMaxValue, double dMinValue)
{
	CRect rectTick;
	CRect rectTickSave;
	CRect rectText;
	int iTickHeight;
	double	dValueHeight;

	rectTick = rectTickSave = m_TkInfo.tkRect[TICK_RIGHT];
	iTickHeight = rectTick.Height();
	dValueHeight = dMaxValue - dMinValue;
	if (dValueHeight <= 0)
		dValueHeight = 1;

	m_iOverBuyY = rectTickSave.top;
	m_iOverSellY = rectTickSave.bottom;

	if (m_iUTickNum <= 0)
		return false;

	struct _userTick
	{
		double		value;
		unsigned char	style;	// PS_SOLID,PS_DASH,PS_DOT,PS_DASHDOT,PS_DASHDOTDOT
		unsigned char	width;	// 1,2,3....
		int		color;	// if color is negative value, color is undefined
		byte		btFillOver;

	};

	_userTick* putLine = new _userTick[m_iUTickNum];

	int	iTickCnt = 0;
	DWORD	dwChkBit = 0x00000001;
	for (int ii = 0; ii < MAX_ULINE; ii++, dwChkBit <<= 1)
	{
		if (!(m_pgraph->tInfo.wCOption & dwChkBit))
			continue;

		if (iTickCnt >= m_iUTickNum)
			break;

		float fVal1 = (float)pow(10.0, m_iValidDigit);
		float fVal = m_pgraph->tInfo.afValue[ii] * fVal1;
		if (fVal < dMinValue || fVal > dMaxValue)
			continue;

		putLine[iTickCnt].value = fVal;
		putLine[iTickCnt].style = m_pgraph->tInfo.aline[ii].btLStyle;
		putLine[iTickCnt].width = m_pgraph->tInfo.aline[ii].btLWidth;
		putLine[iTickCnt].color = m_pgraph->tInfo.aline[ii].rgbLColor;
		putLine[iTickCnt].btFillOver = m_pgraph->tInfo.abtFillOver[ii];	// added by schbang 2006.03.23
		iTickCnt++;
	}

	if (iTickCnt <= 0)
	{
		delete [] putLine;
		putLine = NULL;
		return false;
	}
/*
	if (!(m_pgraph->dwDraOption & GO_TKLEFT) && !(m_pgraph->dwDraOption & GO_TKRIGHT))
	{
		delete [] putLine;
		putLine = NULL;
		return false;
	}
*/
	CFont* pfontTick = GetFont(TICK_RIGHT);
	CFont* pfontOld = pDC->SelectObject(pfontTick);
	COLORREF clrBG = pDC->SetBkColor(m_crBG);
	COLORREF clrTxt = pDC->SetTextColor(RGB(255, 255, 255));
	int iOldBkMode = pDC->SetBkMode(TRANSPARENT);

	int iStyle = putLine[0].style;
	int iWidth = putLine[0].width;
	int iColor = putLine[0].color;

	CPen* ppenColor = GetPen(iStyle, iWidth, iColor);
	CPen* ppenOld = pDC->SelectObject(ppenColor);


	double	dValue;
	CString	strDisp;
	CPoint pointS;
	CPoint pointE;
	CSize size;
	bool bDrawLine = (m_dwDspOpt & DO_TKHGRID)? true:false;

	// right tick
	rectTick = rectTickSave = m_TkInfo.tkRect[TICK_RIGHT];
	iTickHeight = rectTick.Height();
	dValueHeight = dMaxValue - dMinValue;
	if (dValueHeight <= 0)
		dValueHeight = 1;

	m_iOverBuyY = rectTickSave.top;
	m_iOverSellY = rectTickSave.bottom;

	CRgn rgn;
	rgn.CreateRectRgnIndirect(&m_rectChart);
	pDC->SelectClipRgn(&rgn);

	for (int ii = 0; ii < iTickCnt; ii++)
	{
		iStyle = putLine[ii].style;
		if (putLine[ii].width > 0) {
			iWidth = putLine[ii].width;
		}
		else {
			iWidth = 1;
		}
		iColor = putLine[ii].color;

		CPen	*tPen = GetPen(iStyle, iWidth, iColor);
		pDC->SelectObject(tPen);

		pointS.x = m_rectOrgGrp.right;
		dValue = double(double(iTickHeight) * double(dMaxValue - putLine[ii].value));
		dValue /= double(dValueHeight);
		pointS.y = rectTick.top + int(dValue);

		pointE.x = m_rectOrgGrp.left+1;
		pointE.y = pointS.y;

		if (bDrawLine && pointS.y > m_rectChart.top && pointS.y < m_rectChart.bottom)	// 기준선
		{
			pDC->MoveTo(pointE);
			pDC->LineTo(pointS);

			if (putLine[ii].btFillOver == 1 && m_iOverBuyY < pointS.y)	// Added by schbang 2006.03.24
				m_iOverBuyY = pointS.y;
			if (putLine[ii].btFillOver == 2 && m_iOverSellY > pointS.y)
				m_iOverSellY = pointS.y;
		}

		FormatVariableComma(strDisp, putLine[ii].value, dMaxValue);

		CString szRep;
		szRep.Format(".%0*d", m_iValidDigit, 0);
		strDisp.Replace(szRep, "");

		size = pDC->GetOutputTextExtent(strDisp);

		rectText.SetRect(m_rectOrgGrp.left+1, pointS.y - size.cy, 
			m_rectOrgGrp.left+1 + size.cx, pointS.y);

		CRect	rc;
		if (ii > 0 && rc.IntersectRect(&rectText, &rectTickSave))
			continue;

		pDC->SetTextColor(iColor);
		pDC->DrawText(strDisp, &rectText, DT_BOTTOM|DT_SINGLELINE|DT_LEFT);
		pDC->SelectObject(ppenColor);
		rectTickSave = rectText;
	}

	pDC->SelectClipRgn(NULL);

	pDC->SetBkMode(iOldBkMode);
	pDC->SelectObject(ppenOld);
	pDC->SetBkColor(clrBG);
	pDC->SetTextColor(clrTxt);
	pDC->SelectObject(pfontOld);

	delete [] putLine;
	putLine = NULL;

	return true;
}

bool CLineBase::CalculateDisplay(double& rdDiv, double dMax, double dMin, double& rdDrawMinValue, 
				int iDispTick, double* pdDispValue, double* pdLogValue, int& riDispCount)
{
	CString	strTemp;
	double dMok = dMin / rdDiv;
	strTemp.Format("%f", dMok);
	int iIndex = strTemp.Find('.'); 
	dMok = atof(strTemp.Left(iIndex));
	rdDrawMinValue = dMok * rdDiv;
	if (rdDrawMinValue < dMin)		
		rdDrawMinValue += rdDiv;

	int iDrawNum = int ((dMax - rdDrawMinValue) / rdDiv);
	// 2006.07.03
	if (m_pgraph->yainfo.cMode == Y_FIX)
	{
		if ((dMax - dMin) / iDispTick > rdDiv)
		{
			double dAdd = rdDiv;
			while ((dMax - dMin) / iDispTick > rdDiv)
			{
				rdDiv += dAdd;
			}

			dMok = dMin / rdDiv;
			strTemp.Format("%f", dMok);
			iIndex = strTemp.Find('.'); 
			dMok = atof(strTemp.Left(iIndex));
			rdDrawMinValue = dMok * rdDiv;

			
		}
	}
	//if (iDrawNum >= iDispTick)
	else if (iDrawNum >= iDispTick)
	{
		if (iDrawNum <= iDispTick * 2)
			rdDiv *= 2;
		else if (iDrawNum <= iDispTick * 4)
			rdDiv *= 4;
		else if (iDrawNum <= iDispTick * 5)
			rdDiv *= 5;
		else if (iDrawNum <= iDispTick * 10)
			rdDiv *= 10;
		else if (iDrawNum <= iDispTick * 50)
			rdDiv *= 50;
		else if (iDrawNum <= iDispTick * 100)
			rdDiv *= 100;
		else if (iDrawNum <= iDispTick * 500)
			rdDiv *= 500;
		else if (iDrawNum <= iDispTick * 1000)
			rdDiv *= 1000;
		else if (iDrawNum <= iDispTick * 5000)
			rdDiv *= 5000;
		else if (iDrawNum <= iDispTick * 10000)
			rdDiv *= 10000;
		else if (iDrawNum <= iDispTick * 50000)
			rdDiv *= 50000;
		else
			rdDiv *= 100000;

		dMok = dMin / rdDiv;
		strTemp.Format("%f", dMok);
		iIndex = strTemp.Find('.'); 
		dMok = atof(strTemp.Left(iIndex));
		rdDrawMinValue = dMok * rdDiv;
		if (rdDrawMinValue < dMin)		
			rdDrawMinValue += rdDiv;
	}

	riDispCount = 0;
	pdDispValue[riDispCount] = rdDrawMinValue;
	double dVal = 0.0;
	while(1)
	{
		if (pdDispValue[riDispCount] > dMax + 0.01)
			break;

		pdDispValue[riDispCount+1] = pdDispValue[riDispCount] + rdDiv;
		riDispCount++;
	}
	if (riDispCount <= 0)
	{
		if (dMax == dMin)
		{
			pdDispValue[0] = dMax;
			riDispCount = 1;
		}
		else
			return false;
	}
	return true;
}

void CLineBase::DrawTick(CDC* pDC)
{
	m_RgnInfo.iCount = 0;

	class CAppData *pAppData;
	if (!m_pDataMgr->GetData(m_iDataKey, pAppData))
		return;

	if (m_dYMax == DBL_MIN || m_dYMin == DBL_MAX)
	{
// 2007.01.04 schbang : 데이터 없을시 범례2번 그리기 막기
/*
		CString dispText;
		dispText = MakeLegend(0);
		DrawTickIndex(pDC, dispText, m_pgraph->rgbLabel, true);
*/
		m_RgnInfo.iCount++;
		return;
	}

	// 2006.10.11 schbang
	switch (m_iGrpKind)
	{
	case GK_MACD:
	case GK_MACDOS:
		if (CIndcBase::m_pCoDraw && CIndcBase::m_pCoDraw->coWork == MC_MACD)
		{
			m_dYMax = m_pCoDraw->dMacdMax;
			m_dYMin = m_pCoDraw->dMacdMin;
		}
		break;
	}
	
	if (m_pgraph->dwDraOption & GO_TKBOTTOM)
		CIndcBase::DrawTickVertical(pDC);

	if (m_pgraph->dwDraOption & GO_TKLEFT)
	{
		DrawTickLeft(pDC, m_dYMax, m_dYMin);
	}
	
	if (m_pgraph->dwDraOption & GO_TKRIGHT)
	{
		DrawTickRight(pDC, m_dYMax, m_dYMin);
	}

//	if (!(m_pgraph->dwDraOption & GO_USERLINE))
//	{
//		DrawUserDefinedTick(pDC,m_dYMax, m_dYMin);
//	}
}

void CLineBase::DrawGraph(CDC* pDC)
{

	m_RgnInfo.iCount = 0;

	class CAppData *pAppData;
	if (!m_pDataMgr->GetData(m_iDataKey, pAppData))
		return;

	if (m_dYMax == DBL_MIN || m_dYMin == DBL_MAX)
	{
		CString dispText;
		dispText = MakeLegend(0);
		DrawTickIndex(pDC, dispText, m_pgraph->rgbLabel, true);
		m_RgnInfo.iCount++;
		return;
	}

//	DrawTick(pDC);

	if (!(m_pgraph->dwDraOption & GO_USERLINE))
	{
		DrawUserDefinedTick(pDC,m_dYMax, m_dYMin);
	}

	DrawAid(pDC);
}

bool CLineBase::CalculateMinMax(bool bForce)
{
	class CAppData *pAppData;
	if (!m_pDataMgr->GetData(m_iDataKey, pAppData))
		return false;

	bool	bRet = false;
	m_dYMin = DBL_MAX;
	m_dYMax = DBL_MIN;
	if (m_dwDspOpt & DO_SCREEN)
		bRet = pAppData->CalculateMinMax(m_iDispSPos, m_iDispEPos, &m_dYMin, &m_dYMax, bForce);
	else
		bRet = pAppData->CalculateMinMax(0, m_iTtlDtCnt, &m_dYMin, &m_dYMax, bForce);

	ReviseMinMax(m_dYMax, m_dYMin);
	return bRet;
}

bool CLineBase::IsChangeMinMaxRTM(bool bShift, bool bIncrease)
{
	double	svMax = m_dYMax;
	double  svMin = m_dYMin;

	class CAppData *pAppData;
	if (!m_pDataMgr->GetData(m_iDataKey, pAppData))
		return false;

	pAppData->CalculateLastIndex(bShift, bIncrease);
/*	if (!pAppData->CalculateLastIndex(bShift))
		return false;
*/
	if (!CalculateMinMax(false))
		return false;

	if ((svMax != m_dYMax) || (svMin != m_dYMin))
		return true;

	return false;
	
}

void CLineBase::DrawAid(CDC* pDC)
{
	class CAppData *pAppData;
	if (!m_pDataMgr->GetData(m_iDataKey, pAppData))
		return;

	CString	dispText;
	double	*pValue = new double[m_iTtlDtCnt];

	int btLStyle = 0;
	int btLWidth = 0;
	int lcolor = 0;

	int nCount = pAppData->GetDataCount();

	// added by schbang 2006.03.23
	if ((m_pgraph->dwDraOption & GO_FILLOVER) == GO_FILLOVER)
	{
		for (int ii = 0 ; ii < nCount ; ii++)
		{
			for ( int jj = m_iDispSPos ; jj < m_iDispEPos ; jj++ )
				pValue[jj] = pAppData->GetVal(ii, jj);

			DWORD dwChkBit = 0x00000001;
			for (int kk = 0; kk < MAX_ULINE; kk++, dwChkBit <<= 1)
			{
				if (!(m_pgraph->tInfo.wCOption & dwChkBit))	// 사용여부
					continue;

				if (m_pgraph->tInfo.abtFillOver[kk] != 1 && m_pgraph->tInfo.abtFillOver[kk] != 2)	// 과매도, 과매수 여부
					continue;

				btLStyle = m_pgraph->cInfo.aline[kk].btLStyle;
				btLWidth = m_pgraph->cInfo.aline[kk].btLWidth;
				lcolor = m_pgraph->cInfo.aline[kk].rgbLColor;

				DrawLine(pDC, m_dYMax, m_dYMin, pValue, btLStyle, btLWidth, lcolor, ii, kk);
			}
		}
	}

	for (int ii = 0 ; ii < nCount ; ii++)
	{
		for ( int jj = m_iDispSPos ; jj < m_iDispEPos ; jj++ )
			pValue[jj] = pAppData->GetVal(ii, jj);

		btLStyle = m_pgraph->cInfo.aline[ii].btLStyle;
		btLWidth = m_pgraph->cInfo.aline[ii].btLWidth;
		lcolor = m_pgraph->cInfo.aline[ii].rgbLColor;

		DrawLine(pDC, m_dYMax, m_dYMin, pValue, btLStyle, btLWidth, lcolor, ii);
		dispText = MakeLegend(ii);
		DrawTickIndex(pDC, dispText, lcolor, true);
		m_RgnInfo.iCount++;
	}

	delete[] pValue;
}

CString CLineBase::GetDisplayPosData(CPoint pt, bool bGrid)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right) {
		return _T("");
	}

	class CAppData *pAppData;
	if (!m_pDataMgr->GetData(m_iDataKey, pAppData) || pAppData->GetDataCount() <= 0) {
		return "";
	}

	int DrawWidth = m_rectGrp.Width();
	int xPosition = pt.x - m_rectGrp.left;

	double szOneDay = double(DrawWidth) / double(m_iDispDtCnt);
	int dataPos = int(double(xPosition) / szOneDay);
	dataPos += m_iDispSPos;

	if (dataPos < 0 || dataPos >= m_iTtlDtCnt) {
		return _T("");
	}

	CString	unitStr = _T(""), tmpS, tmpS2;
	double	val = 0.0;
	CStringArray	arTitle;

	arTitle.RemoveAll();
	MakeTitle(arTitle);	

	int	nCount = pAppData->GetDataCount();
	
	char	split = ':';
	if (!bGrid)	split = '\t';
	for ( int ii = 0 ; ii < nCount ; ii++ )
	{
		val = pAppData->GetVal(ii, dataPos);

		if (val == GD_UNUSED)
			continue;

		CIndcBase::FormatVariableComma(tmpS, val, m_dYMax, 100);
		tmpS2.Format("%s%c%s", LPCTSTR(arTitle.GetAt(ii)), split, LPCTSTR(tmpS));

		if (!unitStr.IsEmpty())
			unitStr += "\t" + tmpS2;
		else
			unitStr = tmpS2;
	}

	return unitStr;
}

CString CLineBase::ExcelData(int idx)
{
	CString	dispStr = _T("");
	CString	tmpS = _T("");
	CStringArray	arString;

	MakeTitle(arString);

	class CAppData *pAppData;

	if (!m_pDataMgr->GetData(m_iDataKey, pAppData))
		return dispStr;

	int nCount = pAppData->GetDataCount();

	if (idx < 0)
	{
		for ( int ii = 0 ; ii < nCount ; ii++ )
		{
			if (ii != 0)
				dispStr += _T("\t");
			tmpS = arString.GetAt(ii);
			dispStr += tmpS;			
		}		
	}
	else
	{
		for ( int ii = 0 ; ii < nCount ; ii++ )
		{
			if (ii != 0)
				dispStr += _T("\t");

			double	val = pAppData->GetVal(ii, idx);			
			if (val == GD_UNUSED)
				tmpS = _T("0");
			else
				CIndcBase::FormatVariableComma(tmpS, val, 0, 0);
			
			dispStr += tmpS;
		}
	}

	return dispStr;
}

CString CLineBase::GetEachUnit(double Min, double Max)
{
	CString dispStr = _T("");
	
	double value = pow(10.0, m_iValidDigit);

	Max = Max / value;
	Min = Min / value;

	switch (m_iUnit)
	{
	case GU_CODE:
		switch (m_pgraph->wGKind)
		{
		/** remarking by macho@2006.12.12
		case GK_FEXRATE: case GK_FOBRATE: **/
		case GK_FRGNRATE:
			dispStr = _T("[%]");
			break;
		case GK_FRGNHAVE:	// 외국인보유(수량)
			if (Max > (castDVAL(dispUNIT) * 1000.) || Min < (castDVAL(dispUNIT) * -1 * 100.))
			{
				dispStr = _T("[십억]");
			}
			else
			{
				if (Max > dispUNIT || Min < (-1 * (castDVAL(dispUNIT) / 10)))
					dispStr = _T("[백만]");
				else
					dispStr = _T("[x1000]");
			}
			break;
		default:
			if (Max > (castDVAL(dispUNIT) * 1000) || Min < (-1 * castDVAL(dispUNIT) * 100))
			{
				dispStr = _T("[백만]");
			}
			else
			{
				if (Max > dispUNIT || Min < (-1 * (castDVAL(dispUNIT) / 10)))
					dispStr = _T("[x1000]");
				else
					dispStr = _T("");
			}
			break;
		}
		break;
	case GU_INDEX:
		switch (m_pgraph->wGKind)
		{
			case GK_FRGNRATE:						// 외국인보유(비율)
				dispStr = "[%]";
				break;
			case GK_FRGNHAVE:						// 외국인보유(수량)
			case GK_PSNLTBUY: case GK_FRGNTBUY: case GK_ORGNTBUY:	// 누적순매수(수량) - 개인,외인,기관
			case GK_PSNLPBUY: case GK_FRGNPBUY: case GK_ORGNPBUY:	// 순매수(수량) - 개인,외인,기관
				if (Max > (castDVAL(dispUNIT) * 1000) || Min < (-1 * castDVAL(dispUNIT) * 100))
				{
					dispStr = _T("[십억]");
				}
				else
				{
					if (Max > castDVAL(dispUNIT) || Min < (-1 * (castDVAL(dispUNIT) / 10)))
						dispStr = _T("[백만]");
					else
						dispStr = _T("[x1000]");
				}
				break;
		}
		break;
	default:
		if (Max > (castDVAL(dispUNIT) * 1000) || Min < (-1 * castDVAL(dispUNIT) * 100))
		{
			dispStr = _T("[백만]");
		}
		else
		{
			if (Max > castDVAL(dispUNIT) || Min < (-1 * (castDVAL(dispUNIT) / 10)))
				dispStr = _T("[x1000]");
			else
				dispStr = _T("");
		}
		break;
	}
		
	return dispStr;
}

void CLineBase::FormatSubComma(CString &dispTxt, double dVal, double Min, double Max, int maxDigit)
{
	CString	edit1, edit2;
	double dotL = 0, dotR = 0;
	bool	bSign = false;

	if (dVal < 0)
	{
		bSign = true;
		dVal = dVal * (-1);
	}

	dotL = dVal;
	double value = pow(10.0, m_iValidDigit);

	Max = Max / value;
	Min = Min / value;

	if (Max > (static_cast<double>(dispUNIT) * 1000) || Min < (-1 * static_cast<double>(dispUNIT) * 100))
	{
		dotL = int(dotL / 1000000.0);
	}
	else
	{
		if (Max > dispUNIT || Min < (-1 * (static_cast<double>(dispUNIT) / 10)))
		{
			dotL = int(dotL / 1000.0);
		}
		else
		{
			if (bSign)
				dVal = dVal * (-1.0);
			CIndcBase::FormatVariableComma(dispTxt, dVal, Max, maxDigit);
			return;
		}
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
	if (bSign) edit2 += "-";
	edit2.MakeReverse();

	edit1= _T("");


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
	{
		dispTxt = edit2 + edit1;
	}
}