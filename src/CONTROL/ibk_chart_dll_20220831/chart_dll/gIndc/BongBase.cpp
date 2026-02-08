// BongBase.cpp: implementation of the CBongBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BongBase.h"
#include "../gData/DataMgr.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBongBase::CBongBase(CWnd* pwndView, CWnd* pwndParent, class CDataMgr* pDataMgr, char* pcInfo, char* pcGraph, int iDispDay)
: CIndcBase(pwndView, pwndParent, pDataMgr, pcInfo, pcGraph, iDispDay)
{
	if (m_iGrpKind == GK_EQV ||  m_iGrpKind == GK_CDV || m_iGrpKind == GK_EQVV || m_iGrpKind == GK_CDVV)
		m_bEqui = true;
	else
		m_bEqui = false;

	m_dMaxVolValue = m_dMaxVolMav = DBL_MIN;
	m_dMinVolValue = m_dMinVolMav = DBL_MAX;
	
	m_dTotal = 0.0;
	m_dUnit = 0.0;
//	m_nLine = 0;
	m_piVol = NULL;

	class COrgData *pOrgData;
	if (m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
	{
		m_iTtlDtCnt = pOrgData->GetArraySize();
		if (iDispDay == 0)
			m_iDispDtCnt = m_iTtlDtCnt;
		else if (m_iTtlDtCnt < iDispDay)
			m_iDispDtCnt = m_iTtlDtCnt;
		else
			m_iDispDtCnt = iDispDay;

		EditMemberValue();
	}
}

CBongBase::~CBongBase()
{
	if (m_piVol)
	{
		delete[] m_piVol;	m_piVol = NULL;
	}
}

CString CBongBase::GetYposValue(CPoint pt, bool bGrid)
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
	if (IsLogChart())
	{
		Max = GetLogData(Max);
		Min = GetLogData(Min);
	}

	int DrawHeight = m_rectGrp.Height();
	double  ValueHeight = Max - Min;

	if (IsReverseChart())
	{
		posValue = double(ValueHeight) / double(DrawHeight);
		posValue *= double(m_rectGrp.bottom - double(pt.y));
		posValue = Max - posValue;
	}
	else
	{
		posValue = double(ValueHeight) / double(DrawHeight);
		posValue *= double(m_rectGrp.bottom - double(pt.y));
		posValue = Min + posValue;
	}

	if (IsLogChart())
	{
		if (posValue > 0)
			posValue = exp(posValue / 1000.);
		else if (posValue < 0)
			posValue = exp(posValue*(-1) / 1000)*(-1);
		else
			posValue = 0;
	}

	FormatVariableComma(dispStr, posValue, Max, 9);

	if (!bGrid)
		return dispStr;

	if (!dispStr.IsEmpty())
	{
		switch (m_iUnit)
		{
		case GU_CODE:
		case GU_COFUT:
		case GU_COOPT:
			if (m_iValidDigit > 0)
				dispStr = _T("@@달러:") + dispStr;
			else
				dispStr = _T("@@가격:") + dispStr;
			break;
		case GU_CFUTURE: // 20090917 : ADD
		case GU_FFUTURE: // 20090917 : ADD
			dispStr = _T("@@수평값:") + dispStr;
			break;
		case GU_INDEX:
		case GU_FUTURE:
		case GU_OPTION:
			dispStr = _T("@@지수:") + dispStr;
			break;
		default:
			return _T("@@수평값:") + dispStr;
		}
	}

	return dispStr;
}

double CBongBase::GetYposOrgValue(CPoint pt)
{
	double  posValue = 0;
	if (pt.x >= m_rectGrp.right)
		pt.x = m_rectGrp.right-1;
	if (pt.x <= m_rectGrp.left)
		pt.x = m_rectGrp.left+1;

	if (!m_rectGrp.PtInRect(pt))
		return posValue;
	if (m_dYMax == DBL_MIN || m_dYMin == DBL_MAX)
		return posValue;

	double	Max = m_dYMax;
	double	Min = m_dYMin;
	if (IsLogChart())
	{
		Max = GetLogData(Max);
		Min = GetLogData(Min);
	}

	int DrawHeight = m_rectGrp.Height();
	double  ValueHeight = Max - Min;

	if (IsReverseChart())
	{
		posValue = double(ValueHeight) / double(DrawHeight);
		posValue *= double(m_rectGrp.bottom - double(pt.y));
		posValue = Max - posValue;
	}
	else
	{
		posValue = double(ValueHeight) / double(DrawHeight);
		posValue *= double(m_rectGrp.bottom - double(pt.y));
		posValue = Min + posValue;
	}

	if (IsLogChart())
	{
		if (posValue > 0)
			posValue = exp(posValue / 1000.);
		else if (posValue < 0)
			posValue = exp(posValue*(-1) / 1000)*(-1);
		else
			posValue = 0;
	}

	return posValue;
}

CString CBongBase::GetXposValue(CPoint pt)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)
		return _T("");

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData) || pOrgData->GetArraySize() <= 0)
		return _T("");

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
		if (gBasic->m_iClose == GD_UNUSED)	return _T("");

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
	else
	{
		int DrawWidth  = m_rectGrp.Width();
		if (m_dTotal == 0.0 || !m_piVol)
			return _T("");
		double VolWidth = (double)DrawWidth / m_dTotal;
		CString	sTrn;
		CRect	rc;

		double	right = m_rectGrp.left;
		CString	unitStr1 = _T("");
		for (int ii = 0; ii < m_iDispEPos - m_iDispSPos; ii++)
		{
			rc.left = int(right);
			right += m_piVol[ii] * VolWidth;
			rc.right = int(right) - DISTANCE_GAP;

			if (pt.x < rc.left || pt.x > rc.right)
				continue;
			CGrpBasic* gBasic = pOrgData->GetGraphData(ii + m_iDispSPos);
			if (gBasic->m_iClose == GD_UNUSED)	return _T("");

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
	}

	return _T("");
}

CPoint CBongBase::GetAdjustPoint(CPoint pt, CString &sPosVal)
{
	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return CPoint(-1, -1);

	CPoint	retPt(-1, -1);
	if (!m_bEqui)
	{
		int DrawHeight = m_rectGrp.Height();
		int DrawWidth = m_rectGrp.Width();
		int xPosition = pt.x - m_rectGrp.left;

		double	szOneDay = double(DrawWidth) / double(m_iDispDtCnt);
		int	dataPos = int(double(xPosition) / szOneDay);
		dataPos += m_iDispSPos;
		if (dataPos < 0 || dataPos >= m_iTtlDtCnt)
			return retPt;

		CGrpBasic* gBasic = pOrgData->GetGraphData(dataPos);

		double epr = (double)gBasic->m_iClose;
		double	Max = m_dYMax;
		double	Min = m_dYMin;
		if (IsLogChart())
		{
			epr = GetLogData(epr);
			Max = GetLogData(Max);
			Min = GetLogData(Min);
		}

		double ValueHeight = Max - Min;
		if (ValueHeight == 0)
			ValueHeight = 1;		

		if (gBasic->m_iClose > 0.0)
		{
			if (IsReverseChart())
			{
				retPt.y = m_rectGrp.bottom;
				retPt.y -= int((DrawHeight * (Max - double(epr))) / ValueHeight);
			}
			else
			{
				retPt.y = m_rectGrp.top;
				retPt.y += int((DrawHeight * (Max - double(epr))) / ValueHeight);
			}

			FormatVariableComma(sPosVal, epr, Max, 9);
		}
	}
	else
	{
		int DrawHeight = m_rectGrp.Height();
		int DrawWidth  = m_rectGrp.Width();
		if (m_dTotal == 0.0 || !m_piVol)
			return retPt;
		double VolWidth = (double)DrawWidth / m_dTotal;

		double	dValue;
		CGrpBasic* gBasic;

		CString	sTrn;
		CRect	rc;
		int cnt = 0;

		for (int ii = 0; ii < m_iDispEPos - m_iDispSPos; ii++)
		{
			gBasic = pOrgData->GetGraphData(ii+m_iDispSPos);
			rc.left = m_rectGrp.left;
			dValue = VolWidth * cnt;
			rc.left += int(dValue+0.5);

			rc.right = rc.left;
			dValue = m_piVol[ii] * VolWidth;
			rc.right += int(dValue+0.5);

			cnt += m_piVol[ii];
			//if (rc.left == rc.right)
			//	continue;
			if (pt.x < rc.left || pt.x > rc.right)
				continue;

			double epr = (double)gBasic->m_iClose;
			double	Max = m_dYMax;
			double	Min = m_dYMin;
			if (IsLogChart())
			{
				epr = GetLogData(epr);
				Max = GetLogData(Max);
				Min = GetLogData(Min);
			}

			double ValueHeight = Max - Min;
			if (ValueHeight == 0) 
				ValueHeight = 1;

			if (gBasic->m_iClose > 0.0)
			{
				if (IsReverseChart())
				{
					retPt.y = m_rectGrp.bottom;
					retPt.y -= int((DrawHeight * (Max - double(epr))) / ValueHeight);
				}
				else
				{
					retPt.y = m_rectGrp.top;
					retPt.y += int((DrawHeight * (Max - double(epr))) / ValueHeight);
				}

				FormatVariableComma(sPosVal, epr, Max, 9);
			}
			break;
		}
	}
	return retPt;
}

CPoint CBongBase::GetOrderPoint(CPoint pt, CString &sPosVal)
{
	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return CPoint(-1, -1);

	CPoint	retPt(-1, -1);
	{
		int DrawHeight = m_rectGrp.Height();
		int DrawWidth = m_rectGrp.Width();
		int xPosition = pt.x - m_rectGrp.left;

		double	szOneDay = double(DrawWidth) / double(m_iDispDtCnt);
		int	dataPos = int(double(xPosition) / szOneDay);
		dataPos += m_iDispSPos;
		if (dataPos < 0 || dataPos >= m_iTtlDtCnt)
			return retPt;

		CGrpBasic* gBasic = pOrgData->GetGraphData(dataPos);

		double	epr = GetOrderPrice(pOrgData, pt);
		double	Max = m_dYMax;
		double	Min = m_dYMin;
		if (IsLogChart())
		{
			epr = GetLogData(epr);
			Max = GetLogData(Max);
			Min = GetLogData(Min);
		}

		double ValueHeight = Max - Min;
		if (ValueHeight == 0)
			ValueHeight = 1;		

		if (gBasic->m_iClose > 0.0)
		{
			if (IsReverseChart())
			{
				retPt.y = m_rectGrp.bottom;
				retPt.y -= int((DrawHeight * (Max - double(epr))) / ValueHeight);
			}
			else
			{
				retPt.y = m_rectGrp.top;
				retPt.y += int((DrawHeight * (Max - double(epr))) / ValueHeight);
			}

			FormatVariableComma(sPosVal, epr, Max, 9);
		}
	}

	return retPt;
}

double CBongBase::GetOrderPrice(COrgData *pOrgData, CPoint pt)
{
	double	upVal = pOrgData->GetSahg();
	double	dnVal = pOrgData->GetHahg();
	char	cKind = pOrgData->GetStockKind();

	double	dVal = GetYposOrgValue(pt);
	double	delta = 0;

	if (cKind == 1 || cKind == 11)
	{
		if (dVal >= 500000)
			delta = 1000;
		else if (dVal >= 100000)
			delta = 500;
		else if (dVal >= 50000)
			delta = 100;
		else if (dVal >= 10000)
			delta = 50;
		else if (dVal >= 5000)
			delta = 10;
		else
			delta = 5;
	}
	else if (cKind = 2)
	{
		if (dVal >= 50000)
			delta = 100;
		else if (dVal >= 10000)
			delta = 50;
		else if (dVal >= 5000)
			delta = 10;
		else
			delta = 5;
	}

	double	retVal = 0;
	if (delta != 0)
		retVal = int(dVal / delta) * delta;

	if (dVal == 0)
		retVal = 0;
	else if (dVal > upVal)
		retVal = upVal;
	else if (dVal < dnVal)
		retVal = dnVal;

	return retVal;
}

bool CBongBase::CalculateMinMax(bool bForce)
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

bool CBongBase::IsChangeMinMaxRTM(bool bShift, bool bIncrease)
{
	double	svMax = m_dYMax;
	double  svMin = m_dYMin;

	class CAppData *pAppData;
	if (!m_pDataMgr->GetData(m_iDataKey, pAppData))
		return false;

	if (!pAppData->CalculateLastIndex(bShift, bIncrease))
		return false;

	if (!CalculateMinMax(false))
		return false;

	if ((svMax != m_dYMax) || (svMin != m_dYMin))
		return true;

	return false;
}

void CBongBase::DrawTickRight(CDC* pDC, double dMax, double dMin, bool bDrawMin)
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
	double dValueHeight = dMax - dMin;
	if (dValueHeight <= 0)
		dValueHeight = 1;
	
	double dDiv;
	if (dValueHeight < 5)			dDiv = 1;
	else if (dValueHeight < 50)		dDiv = 5;
	else if (dValueHeight < 200)		dDiv = 10;
	else if (dValueHeight < 500)		dDiv = 50;
	else if (dValueHeight < 2000)		dDiv = 100;
	else if (dValueHeight < 5000)		dDiv = 500;
	else if (dValueHeight < 20000)		dDiv = 1000;
	else if (dValueHeight < 50000)		dDiv = 5000;
	else if (dValueHeight < 200000)		dDiv = 10000;
	else if (dValueHeight < 500000)		dDiv = 50000;
	else if (dValueHeight < 2000000)	dDiv = 100000;
	else if (dValueHeight < 5000000)	dDiv = 500000;
	else if (dValueHeight < 20000000)	dDiv = 1000000;
	else if (dValueHeight < 50000000)	dDiv = 5000000;
	else if (dValueHeight < 200000000)	dDiv = 10000000;
	else if (dValueHeight < 500000000)	dDiv = 50000000;
	else if (dValueHeight < 2000000000)	dDiv = 100000000;
	//else					dDiv = 500000000;
	else
	{
		CString	strTmp, strDiv;
		strTmp.Format("%.0f", dValueHeight);
		strDiv.Format("1%0*d", strTmp.GetLength()-1, 0);
		dDiv = atof(strDiv);
	}


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
	{
		switch (m_iUnit)
		{
		case GU_CODE:
			dDiv = m_pgraph->yainfo.dFix;
			break;
		default:
			dDiv = m_pgraph->yainfo.dFix * 100;
			break;
		}
	}
//dDiv = dValueHeight / 8;
//if (dDiv == 5000)
//dDiv = 10000;
	
	int	iDispCount;
	double	adDispValue[64];
	double	adLogValue[64];
	double	dDrawMinValue = 0.0;
	if (!CalculateDisplay(dDiv, dMax, dMin, dDrawMinValue, iDispTick, adDispValue, adLogValue, iDispCount))
		return;

	int iOldMode = pDC->SetBkMode(TRANSPARENT);
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

	CString	strDisp;
	FormatVariableComma(strDisp, dMax, dMax, iMaxDigit);
	size = pDC->GetOutputTextExtent(strDisp);
	FormatVariableComma(strDisp, dMin, dMax, iMaxDigit);
	CSize size2 = pDC->GetOutputTextExtent(strDisp);
	
	int iPivotY = rectTick.left + gapTICK + size.cx;
	if (size.cx < size2.cx)
		iPivotY = rectTick.left + gapTICK + size2.cx;

	bool bLog = IsLogChart();
	bool bReverse = IsReverseChart();
	//bool bDrawLine = (m_dwDspOpt & DO_TKHGRID)? true : false;
	bool bDrawLine = ((m_dwDspOpt & DO_TKHGRID) && !(m_pgraph->dwDraOption & GO_TICK_SKIP_HLINE)) ? true : false;

	if (bReverse)
		rectTickSave.bottom = rectTick.top;
	else
		rectTickSave.top = rectTick.bottom;

	if (bLog)
	{
		if (dMax > 0)
			dMax = double(1000. * double(log(dMax)));
		else if (dMax < 0)
			dMax = (-1) * double(1000. * double(log(dMax * (-1))));
		else
			dMax = 0;

		if (dMin > 0)
			dMin = double(1000. * double(log(dMin)));
		else if (dMin < 0)
			dMin = (-1) * double(1000. * double(log(dMin * (-1))));
		else
			dMin = 0;

		dValueHeight = dMax - dMin;
		if (dValueHeight <= 0)
			dValueHeight = 1;
	}
	
	
	int	ii = 0;
	double	dValue = 0.;
	CRect	rectText(0,0,0,0);
	CPoint	pointStart, pointEnd;

	for (ii = 0; ii < iDispCount; ii++)		// 가격별가로 (DOT)
	{
		pointStart.x = m_rectOrgGrp.right;
		dValue = (bLog)? adLogValue[ii] : adDispValue[ii];
		dValue = double(double(iTickHeight) * double(dMax - dValue));
		dValue /= double(dValueHeight);

		if (bReverse)
			pointStart.y = rectTick.bottom - int(dValue);
		else
			pointStart.y = rectTick.top + int(dValue);

		if (!rectTick.PtInRect(pointStart))
		{
			if (rectTick.bottom < pointStart.y)
				continue;
		}

		pointEnd.x = m_rectOrgGrp.left + 1;
		pointEnd.y = pointStart.y;
		if (bDrawLine && pointStart.y > m_rectChart.top && pointStart.y < m_rectChart.bottom)
		{
			if (ii != 0)
			{
				pDC->SelectObject(ppenTick);
				pDC->MoveTo(pointEnd);
				pDC->LineTo(pointStart);
			}
		}

		if (ii != 0)
		{
			pDC->SelectObject(ppenLine);
			pDC->MoveTo(m_rectOrgGrp.right, pointStart.y);
			pDC->LineTo(m_rectOrgGrp.right + lenTICK, pointStart.y);
		}

		FormatVariableComma(strDisp, adDispValue[ii], dMax, iMaxDigit);

		if (ii == 0)
		{
			CString	str = GetTickUnit(dMax);
			if (str.IsEmpty())
			{
				size = pDC->GetOutputTextExtent(strDisp);
				if (bReverse)
				{
					rectText.SetRect(rectTick.left + gapTICK, pointStart.y - size.cy/2, 
						iPivotY, pointStart.y + size.cy/2);

					if (rectTickSave.bottom - rectText.top >= 5)
						continue;
					if (rectText.top < rectTickSave.bottom)
						rectText.OffsetRect(0, rectTickSave.bottom - rectText.top);
					if (rectText.bottom > rectTick.bottom)
						rectText.OffsetRect(0, rectTick.bottom - rectText.bottom);
				}
				else
				{
					rectText.SetRect(rectTick.left + gapTICK, pointStart.y - size.cy/2,
						iPivotY, pointStart.y + size.cy/2);

					if (rectText.bottom - rectTickSave.top >= 5)
						continue;
					if (rectText.bottom > rectTickSave.top)
						rectText.OffsetRect(0, rectTickSave.top - rectText.bottom);
				}
				rectText.IntersectRect(&rectText, &rectOrgTick);
				pDC->DrawText(strDisp, &rectText, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
			}
			else
			{
				strDisp = str;
				size = pDC->GetOutputTextExtent(strDisp);
				if (bReverse)
					rectText.SetRect(m_rectOrgGrp.right + lenTICK, rectTick.top, 
						rectTick.right, rectTick.top + size.cy);
				else
					rectText.SetRect(m_rectOrgGrp.right + lenTICK, rectTick.bottom - size.cy,
						rectTick.right, rectTick.bottom);
				rectText.IntersectRect(&rectText, &rectOrgTick);
				pDC->DrawText(strDisp, &rectText, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
			}
		}
		else
		{
			size = pDC->GetOutputTextExtent(strDisp);
			if (bReverse)
			{
				rectText.SetRect(rectTick.left + gapTICK, pointStart.y - size.cy/2, 
					iPivotY, pointStart.y + size.cy/2);

				if (rectTickSave.bottom - rectText.top >= 5)
					continue;
				if (rectText.top < rectTickSave.bottom)
					rectText.OffsetRect(0, rectTickSave.bottom - rectText.top);
				if (rectText.bottom > rectTick.bottom)
					rectText.OffsetRect(0, rectTick.bottom - rectText.bottom);
			}
			else
			{
				rectText.SetRect(rectTick.left + gapTICK, pointStart.y - size.cy/2,
					iPivotY, pointStart.y + size.cy/2);

				if (rectText.bottom - rectTickSave.top >= 5)	// 0번째거는 이거때문에 안그려짐
					continue;
				if (rectText.bottom > rectTickSave.top)
					rectText.OffsetRect(0, rectTickSave.top - rectText.bottom);
			}
			rectText.IntersectRect(&rectText, &rectOrgTick);
			pDC->DrawText(strDisp, &rectText, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
		}
		rectTickSave = rectText;
	}

	if (m_iGrpKind == GK_VOL || m_iGrpKind == GK_VMA || m_iGrpKind == GK_AMT || m_iGrpKind == GK_AMA)
	{
		pDC->SelectObject(pfontOld);
		pDC->SelectObject(ppenOld);
		pDC->SetTextColor(rgbOldText);
		pDC->SetBkMode(iOldMode);
		return;
	}

	rectTick = m_TkInfo.tkRect[TICK_RIGHT];

	if (!bLog && dDiv != 1)
	{
		pDC->SelectObject(ppenLine);
		strDisp.Format("%f", dDiv);
		double dDivSave = dDiv;
		if (strDisp.GetAt(0) == '5')
			dDivSave = dDiv/5.;
		else
			dDivSave = dDiv/2.;

		GetMinMaxValue(dMin, dMax);
		dValueHeight = dMax - dMin;

		CString	strTemp;
		double dMok = dMin / dDivSave;
		strTemp.Format("%f", dMok);
		int iIndex = strTemp.Find('.'); 
		dMok = atof(strTemp.Left(iIndex));
		dDrawMinValue = dMok * dDivSave;
		if (dDrawMinValue < dMin)		
			dDrawMinValue += dDivSave;
		for (ii = 0;;ii++)
		{
			dValue = dDrawMinValue + dDivSave*ii;
			if (dValue >= dMax)
				break;

			dValue = double(iTickHeight) * double(dMax - dValue);
			dValue /= double(dValueHeight);
			if (bReverse)
				pointStart.y = rectTick.bottom - int(dValue);
			else
				pointStart.y = rectTick.top + int(dValue);
			
			pDC->MoveTo(m_rectOrgGrp.right, pointStart.y);
			pDC->LineTo(m_rectOrgGrp.right+5, pointStart.y);
		}
	}

	pDC->SelectObject(pfontOld);
	pDC->SelectObject(ppenOld);
	pDC->SetTextColor(rgbOldText);
	pDC->SetBkMode(iOldMode);
}

void CBongBase::DrawTickLeft(CDC* pDC, double dMax, double dMin, bool bDrawMin)
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
	double	dValueHeight = dMax - dMin;
	if (dValueHeight <= 0)
		dValueHeight = 1;

	double	dDiv;
/*	CString	strTmp, strDiv;
	strTmp.Format("%.0f", dValueHeight);
	char	cFirst = strTmp.GetAt(0);
	if (dValueHeight < 5)			dDiv = 1;
	else if (dValueHeight < 50)		dDiv = 5;
	else if (cFirst < '2')
	{
		strDiv.Format("1%0*d", strTmp.GetLength()-2, 0);
		dDiv = atof(strDiv);
	}
	else
	{
		strDiv.Format("5%0*d", strTmp.GetLength()-2, 0);
		dDiv = atof(strDiv);
	}
*/
	if (dValueHeight < 5)			dDiv = 1;
	else if (dValueHeight < 50)		dDiv = 5;
	else if (dValueHeight < 200)		dDiv = 10;
	else if (dValueHeight < 500)		dDiv = 50;
	else if (dValueHeight < 2000)		dDiv = 100;
	else if (dValueHeight < 5000)		dDiv = 500;
	else if (dValueHeight < 20000)		dDiv = 1000;
	else if (dValueHeight < 50000)		dDiv = 5000;
	else if (dValueHeight < 200000)		dDiv = 10000;
	else if (dValueHeight < 500000)		dDiv = 50000;
	else if (dValueHeight < 2000000)	dDiv = 100000;
	else if (dValueHeight < 5000000)	dDiv = 500000;
	else if (dValueHeight < 20000000)	dDiv = 1000000;
	else if (dValueHeight < 50000000)	dDiv = 5000000;
	else if (dValueHeight < 200000000)	dDiv = 10000000;
	else if (dValueHeight < 500000000)	dDiv = 50000000;
	else if (dValueHeight < 2000000000)	dDiv = 100000000;
	//else					dDiv = 500000000;
	else
	{
		CString	strTmp, strDiv;
		strTmp.Format("%.0f", dValueHeight);
		strDiv.Format("1%0*d", strTmp.GetLength()-1, 0);
		dDiv = atof(strDiv);
	}

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
		dDiv = m_pgraph->yainfo.dFix;

	int iDispCount;
	double adDispValue[64];
	double adLogValue[64];
	double dDrawMinValue = 0.0;
	
	if (!CalculateDisplay(dDiv, dMax, dMin, dDrawMinValue, iDispTick, adDispValue, adLogValue, iDispCount))
		return;

	int iOldMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF rgbOldText = pDC->SetTextColor(m_atick[TICK_LEFT].dwTColor);
	int iLineStyle = m_lineH.btLStyle;
	int iLineWidth = m_lineH.btLWidth;
	COLORREF rgbLineColor = m_lineH.rgbLColor;

	CFont* pfontTick = GetFont(TICK_LEFT);
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

	bool bLog = IsLogChart();
	bool bReverse = IsReverseChart();
	bool bDrawLine = (m_dwDspOpt & DO_TKHGRID)? true : false;

	if (bReverse)
		rectTickSave.bottom = rectTick.top;
	else
		rectTickSave.top = rectTick.bottom;

	if (bLog)
	{
		if (dMax > 0)
			dMax = double(1000. * double(log(dMax)));
		else if (dMax < 0)
			dMax = (-1) * double(1000. * double(log(dMax*(-1))));
		else
			dMax = 0;

		if (dMin > 0)
			dMin = double(1000. * double(log(dMin)));
		else if (dMin < 0)
			dMin = (-1) * double(1000. * double(log(dMin*(-1))));
		else
			dMin = 0;

		dValueHeight = dMax - dMin;
		if (dValueHeight <= 0)
			dValueHeight = 1;
	}

	int ii = 0;
	double dValue;
	CRect rectText;
	CString	strDisp;
	CPoint pointStart;
	CPoint pointEnd;
	for (ii = 0; ii < iDispCount; ii++)		// 가격별가로 (DOT)
	{
		pointStart.x = m_rectOrgGrp.left+1;
		dValue = (bLog)? adLogValue[ii]:adDispValue[ii];
		dValue = double(double(iTickHeight) * double(dMax - dValue));
		dValue /= double(dValueHeight);

		if (bReverse)
			pointStart.y = rectTick.bottom - int(dValue);
		else
			pointStart.y = rectTick.top + int(dValue);

		if (!rectTick.PtInRect(CPoint(pointStart.x - 1, pointStart.y)))
		{
			if (rectTick.bottom < pointStart.y)
				continue;
		}

		pointEnd.x = m_rectOrgGrp.right;
		pointEnd.y = pointStart.y;
		if (bDrawLine && pointStart.y > m_rectChart.top && pointStart.y < m_rectChart.bottom)
		{
			if (ii != 0)
			{
				pDC->SelectObject(ppenTick);
				pDC->MoveTo(pointStart);
				pDC->LineTo(pointEnd);
			}
		}
		
		if (ii != 0)
		{
			pDC->SelectObject(ppenLine);
			pDC->MoveTo(m_rectOrgGrp.left, pointStart.y);
			pDC->LineTo(m_rectOrgGrp.left - lenTICK, pointStart.y);
		}

		FormatVariableComma(strDisp, adDispValue[ii], dMax, iMaxDigit);

		if (ii == 0)
		{
			CString	str = GetTickUnit(dMax);
			if (str.IsEmpty())
			{
				size = pDC->GetOutputTextExtent(strDisp);
				if (bReverse)
				{
					rectText.SetRect(rectTick.left, pointStart.y - size.cy/2, 
						rectTick.right - gapTICK, pointStart.y + size.cy/2);

					if (rectTickSave.bottom - rectText.top >= 5)	continue;
					if (rectText.top < rectTickSave.bottom)
						rectText.OffsetRect(0, rectTickSave.bottom - rectText.top);
					if (rectText.bottom > rectTick.bottom)
						rectText.OffsetRect(0, rectTick.bottom - rectText.bottom);
				}
				else
				{
					rectText.SetRect(rectTick.left, pointStart.y - size.cy/2,
						rectTick.right - gapTICK, pointStart.y + size.cy/2);

					if (rectText.bottom - rectTickSave.top >= 5)	continue;
					if (rectText.bottom > rectTickSave.top)
						rectText.OffsetRect(0, rectTickSave.top - rectText.bottom);
				}
				rectText.IntersectRect(&rectText, &rectOrgTick);
				pDC->DrawText(strDisp, rectText, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
			}
			else
			{
				strDisp = str;
				size = pDC->GetOutputTextExtent(strDisp);
				if (bReverse)
					rectText.SetRect(rectTick.left, rectTick.top, 
						m_rectOrgGrp.left - lenTICK, rectTick.top + size.cy);
				else
					rectText.SetRect(rectTick.left, rectTick.bottom - size.cy,
						m_rectOrgGrp.left - lenTICK, rectTick.bottom);
				rectText.IntersectRect(&rectText, &rectOrgTick);
				pDC->DrawText(strDisp, rectText, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
			}
		}
		else
		{
			size = pDC->GetOutputTextExtent(strDisp);
			if (bReverse)
			{
				rectText.SetRect(rectTick.left, pointStart.y - size.cy/2, 
					rectTick.right - gapTICK, pointStart.y + size.cy/2);

				if (rectTickSave.bottom - rectText.top >= 5)	continue;
				if (rectText.top < rectTickSave.bottom)
					rectText.OffsetRect(0, rectTickSave.bottom - rectText.top);
				if (rectText.bottom > rectTick.bottom)
					rectText.OffsetRect(0, rectTick.bottom - rectText.bottom);
			}
			else
			{
				rectText.SetRect(rectTick.left, pointStart.y - size.cy/2,
					rectTick.right - gapTICK, pointStart.y + size.cy/2);

				if (rectText.bottom - rectTickSave.top >= 5)	continue;
				if (rectText.bottom > rectTickSave.top)
					rectText.OffsetRect(0, rectTickSave.top - rectText.bottom);
			}
			rectText.IntersectRect(&rectText, &rectOrgTick);
			pDC->DrawText(strDisp, rectText, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
		}
		rectTickSave = rectText;
	}

	if (m_iGrpKind == GK_VOL || m_iGrpKind == GK_VMA || m_iGrpKind == GK_AMT || m_iGrpKind == GK_AMA)
	{
		pDC->SelectObject(pfontOld);
		pDC->SelectObject(ppenOld);
		pDC->SetTextColor(rgbOldText);
		pDC->SetBkMode(iOldMode);
		return;
	}

	rectTick = m_TkInfo.tkRect[TICK_LEFT];
	if (!bLog && dDiv != 1)
	{
		pDC->SelectObject(ppenLine);
		strDisp.Format("%f", dDiv);
		double	divS = dDiv;
		if (strDisp.GetAt(0) == '5')
			divS = dDiv/5.;
		else
			divS = dDiv/2.;

		GetMinMaxValue(dMin, dMax);
		dValueHeight = dMax - dMin;

		CString	temp;
		double	mok = dMin / divS;
		temp.Format("%f", mok);
		int index = temp.Find('.'); 
		mok = atof(temp.Left(index));
		dDrawMinValue = mok * divS;
		if (dDrawMinValue < dMin)		
			dDrawMinValue += divS;
		for (ii = 0;;ii++)
		{
			dValue = dDrawMinValue + divS*ii;
			if (dValue >= dMax)	break;

			dValue = double(iTickHeight) * double(dMax - dValue);
			dValue /= double(dValueHeight);
			if (bReverse)	pointStart.y = rectTick.bottom - int(dValue);
			else		pointStart.y = rectTick.top + int(dValue);
			
			pDC->MoveTo(m_rectOrgGrp.left, pointStart.y);
			pDC->LineTo(m_rectOrgGrp.left-5, pointStart.y);
		}
	}

	pDC->SelectObject(pfontOld);
	pDC->SelectObject(ppenOld);
	pDC->SetTextColor(rgbOldText);
	pDC->SetBkMode(iOldMode);
}

void CBongBase::DrawTickVertical(CDC *pDC)
{
	class COrgData *pOrgData;
	if (!m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
		return;

	if (pOrgData->GetPattern() != dpBASIC)
		return;

	if (m_bEqui && !m_piVol)
		return;

//if (pOrgData->GetArraySize() < 1)
//	return;

	CRect	tickRC = m_TkInfo.tkRect[TICK_BOTTOM];
	tickRC.right = m_rectGrp.right;
	tickRC.left = m_rectGrp.left;

	int tickWidth = tickRC.Width();
	int tickHeight = tickRC.Height();

	int sMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF sColor = pDC->SetTextColor(m_atick[TICK_BOTTOM].dwTColor);

	int	style = m_lineV.btLStyle;
	int	width = m_lineV.btLWidth;
	COLORREF color = m_lineV.rgbLColor;

	CPen	*cPen = GetPen(style, width, color);
	CPen	*cPen2 = GetPen(m_lineDate.btLStyle, m_lineDate.btLWidth, m_lineDate.rgbLColor);
	CPen	*sPen = pDC->SelectObject(cPen);

	CFont	*boldFont = GetFont(TICK_BOTTOM, FW_BOLD);
	CFont	*tkFont = GetFont(TICK_BOTTOM);
	CFont	*sFont = pDC->SelectObject(boldFont);

	CArray <_dispInfo, _dispInfo> arDispInfo;
	_dispInfo	dispInfo;
	arDispInfo.RemoveAll();

	int index = GetDataIndex();
	CPoint	pointDAY;
	CString	dispStr;
	CRect	drawRC;
	CRect	saveRC(0,0,0,0);
	int	svYear = 0, svMonth = 0, svDay = 0;
	int	svHH = 0, svMM = 0;
	int	savePos = 0;
	int	gap = 50;
	pointDAY.y = tickRC.top + int(tickHeight / 5);

	int	endIdx = m_iDispEPos - m_iDispSPos - 1;
	CGrpBasic* gBasic;

	double	equiWidth = 0.0;
	double	left = tickRC.left;

	if (m_dTotal)
		equiWidth = (double)tickWidth / (double)m_dTotal;

	int ii = 0;
	for (ii = 0; ii < m_iDispEPos - m_iDispSPos; ii++)
	{
		gBasic = pOrgData->GetGraphData(ii+m_iDispSPos);
		if (gBasic->m_index.date.yy || gBasic->m_index.time.hh)
			break;
	}

	int first = ii;
	gBasic = pOrgData->GetGraphData(ii+m_iDispSPos);
if (!gBasic)
	return;

	svDay = gBasic->m_index.date.dd;
	for (; ii < m_iDispEPos - m_iDispSPos; ii++)
	{
		gBasic = pOrgData->GetGraphData(ii+m_iDispSPos);
		if (!m_bEqui)
		{
			pointDAY.x = tickRC.left;
			pointDAY.x += int((tickWidth * ii) / m_iDispDtCnt);
			pointDAY.x -= DISTANCE_GAP;	// 그래프간 간격
		}	
		else
		{
			pointDAY.x = int(left) - DISTANCE_GAP;
			left += m_piVol[ii] * equiWidth;
		}
		pointDAY.y = tickRC.top + int(tickHeight / 5);

		switch(index)
		{
		case GI_MONTH:			// 년도
		case GI_WEEK:			// 3달 
			if (svYear == gBasic->m_index.date.yy)
				continue;
			dispStr.Format("%04d", gBasic->m_index.date.yy);
			svYear = gBasic->m_index.date.yy;
			break;
		case GI_MINUTE:	case GI_TICK:			// 시간
			if (svHH == gBasic->m_index.time.hh)
				continue;

			if (ii != 0)
				gap = max(pointDAY.x - savePos, gap);
			savePos = pointDAY.x;
			svHH = gBasic->m_index.time.hh;

			if (ii != first && gBasic->m_index.date.dd == svDay)
				continue;
			
			if (ii == first)
				dispStr.Format("%02d:%02d", gBasic->m_index.time.hh,
					gBasic->m_index.time.mm);
			else
			{
				dispStr.Format("%02d/%02d", 
					gBasic->m_index.date.mm, gBasic->m_index.date.dd);
				svDay = gBasic->m_index.date.dd;
			}

			break;
		case GI_DAY:			// 달
		default:
			if (svYear == gBasic->m_index.date.yy && svMonth == gBasic->m_index.date.mm)
				continue;

			svMonth  = gBasic->m_index.date.mm;
			gap = max(pointDAY.x - savePos, gap);
			savePos = pointDAY.x;

			if (svYear == gBasic->m_index.date.yy)
				continue;

			dispStr.Format("%04d/%02d", gBasic->m_index.date.yy, gBasic->m_index.date.mm);
			svYear = gBasic->m_index.date.yy;
			
			break;
		}

		drawRC = DisplayRect(pDC, ii, dispStr, pointDAY);

		dispInfo.dispRC = drawRC;
		memset(&dispInfo.dispStr, 0x00, sizeof(dispInfo.dispStr));
		memcpy(&dispInfo.dispStr, dispStr, dispStr.GetLength());
		dispInfo.dispPt = pointDAY;
		arDispInfo.Add(dispInfo);
	}
		
	int view = dispONEDAY;
	if (index == GI_DAY)
	{
		if (gap < 13)
			view = dispNONE;
		else if (gap >= 13 && gap < 20)
			view = dispSIXDAY;
		else if (gap >= 20 && gap < 30)
			view = dispTHREEDAY;
	}
	else if (index == GI_MINUTE || index == GI_TICK)
	{
		if (gap > 55)
			view = dispONEHOUR;
		else if (gap >= 15 && gap < 55)
			view = dispTHREEHOUR;
		else
			view = dispNONE;
	}
	else
		view = dispNONE;

	bool bText = true;
	CRect unionRC;
	CRect cmpRC;

	saveRC.SetRectEmpty();
	int arSize = arDispInfo.GetSize();
	for (ii = 0; ii < arSize; ii++)
	{
		bText = true;
		dispInfo = arDispInfo.GetAt(ii);

		if ((index == GI_MINUTE || index == GI_TICK) && (view != dispNONE && ii != 0))
			pDC->SelectObject(cPen2);
		else
			pDC->SelectObject(cPen);

		if (m_dwDspOpt & DO_TKVGRID && dispInfo.dispPt.x > tickRC.left && dispInfo.dispPt.x < tickRC.right)
		{
			pDC->MoveTo(dispInfo.dispPt.x, m_rectChart.top);
			pDC->LineTo(dispInfo.dispPt.x, m_rectChart.bottom);
		}
		
		if (arSize > 1)
		{
			if (index != GI_MINUTE && index != GI_TICK)
			{
				
				if (ii == 0)
				{
					cmpRC = arDispInfo.GetAt(1).dispRC;
					unionRC = cmpRC & dispInfo.dispRC;
					if (!unionRC.IsRectEmpty())
						bText = false;
					else
						saveRC = unionRC;
				}
				else
				{
					unionRC = saveRC & dispInfo.dispRC;
					if (unionRC.IsRectEmpty())
						saveRC = dispInfo.dispRC;
					else
						bText = false;
				}
			}
			else
			{
				unionRC = saveRC & dispInfo.dispRC;
				if (!unionRC.IsRectEmpty())
					continue;
				saveRC = dispInfo.dispRC;
			}
		}

//		pDC->MoveTo(dispInfo.dispPt.x, m_rectChart.bottom);
//		pDC->LineTo(dispInfo.dispPt.x, m_rectChart.bottom+4);
		
		if (bText)
		{
			if (index == GI_MINUTE || index == GI_TICK)
			{
				if (view != dispNONE && ii != 0)
					pDC->SelectObject(boldFont);
				else
					pDC->SelectObject(tkFont);
				
			}
			if (dispInfo.dispRC.right < tickRC.right)
				pDC->DrawText(dispInfo.dispStr, &dispInfo.dispRC, DT_SINGLELINE|DT_TOP|DT_CENTER);
		}
	}

	pDC->SelectObject(tkFont);
	pDC->SelectObject(cPen);

	left = tickRC.left;
	if (view > 0)
	{
		int jj = 0;
		bool bDraw = true;
		saveRC.SetRectEmpty();
		for (ii = 0; ii <= endIdx; ii++)
		{
			gBasic = pOrgData->GetGraphData(ii+m_iDispSPos);
			if (gBasic->m_index.date.yy || gBasic->m_index.time.hh)
				break;
		}
		
		int	prePoint = 0;
		for (; ii <= endIdx; ii++)
		{
			gBasic = pOrgData->GetGraphData(ii+m_iDispSPos);
			
			if (m_bEqui)
			{
				pointDAY.x = int(left) - DISTANCE_GAP;
				left += m_piVol[ii] * equiWidth;
			}

			switch (index)
			{
			case GI_DAY:
				if (svYear == gBasic->m_index.date.yy 
					&& svMonth == gBasic->m_index.date.mm)
					continue;
				switch (view)
				{
				case dispTHREEDAY:
					if (gBasic->m_index.date.mm % 3 != 1)
						continue;
					break;
				case dispSIXDAY:
					if (gBasic->m_index.date.mm % 6 != 1)
						continue;
					break;
				}
				dispStr.Format("%02d", gBasic->m_index.date.mm);
				svYear = gBasic->m_index.date.yy;
				svMonth  = gBasic->m_index.date.mm;
				break;
			case GI_MINUTE:	case GI_TICK:				
				if (svHH == gBasic->m_index.time.hh && ii != endIdx)
					continue;
				switch (view)
				{
				case dispTHREEHOUR:
					if (ii != endIdx && gBasic->m_index.time.hh % 12 != 0)
					{
						svHH = gBasic->m_index.time.hh;
						continue;
					}
					break;
				}
				
				if (ii != endIdx)
					dispStr.Format("%02d", gBasic->m_index.time.hh);
				else
					dispStr.Format("%02d:%02d", 
						gBasic->m_index.time.hh, gBasic->m_index.time.mm);

				svHH = gBasic->m_index.time.hh;
				break;
			}

			// 그래프간 간격
			if (!m_bEqui)
			{
				pointDAY.x = tickRC.left;
				pointDAY.x += int((tickWidth * ii) / m_iDispDtCnt);
				pointDAY.x -= DISTANCE_GAP;
			}
			pointDAY.y = tickRC.top + int(tickHeight / 5);

			drawRC = DisplayRect(pDC, ii, dispStr, pointDAY);
			unionRC = saveRC & drawRC;
			if (!unionRC.IsRectEmpty())
				continue;
			
			bDraw = true;
			for (jj = 0; jj < arDispInfo.GetSize(); jj++)
			{
				if (arDispInfo.GetAt(jj).dispPt.x == pointDAY.x)
				{
					bDraw = false;
					break;
				}
			}
			if (!bDraw)	continue;

			for (jj = 0; jj < arDispInfo.GetSize(); jj++)
			{
				cmpRC = arDispInfo.GetAt(jj).dispRC;
				unionRC = cmpRC & drawRC;
				if (!unionRC.IsRectEmpty())
				{
					bDraw = false;
					break;
				}
			}

			if (bDraw)
			{
				int Pixgap = drawRC.right - m_TkInfo.tkRect[TICK_RIGHT].right;
				if (Pixgap > 0)	drawRC.OffsetRect(Pixgap*(-1), 0);
				for (jj = 0; jj < arDispInfo.GetSize(); jj++)
				{
					cmpRC = arDispInfo.GetAt(jj).dispRC;
					unionRC = cmpRC & drawRC;
					if (!unionRC.IsRectEmpty())
					{
						bDraw = false;
						break;
					}
				}

				if (m_dwDspOpt & DO_TKVGRID)
				{
					pDC->MoveTo(pointDAY.x, m_rectChart.top);
					pDC->LineTo(pointDAY.x, m_rectChart.bottom);
					pDC->MoveTo(pointDAY.x, m_rectChart.bottom);
					pDC->LineTo(pointDAY.x, m_rectChart.bottom+3);

					prePoint = pointDAY.x;
				}

				if (bDraw)
				{
					pDC->DrawText(dispStr, &drawRC, DT_SINGLELINE|DT_TOP|DT_CENTER);
					saveRC = drawRC;
				}
			}
			else if (index == GI_DAY && pointDAY.x - prePoint > 50)
			{
				if (m_dwDspOpt & DO_TKVGRID)
				{
					pDC->MoveTo(pointDAY.x, m_rectChart.top);
					pDC->LineTo(pointDAY.x, m_rectChart.bottom);
					pDC->MoveTo(pointDAY.x, m_rectChart.bottom);
					pDC->LineTo(pointDAY.x, m_rectChart.bottom+3);

					prePoint = pointDAY.x;
				}
			}
		}
	}

	arDispInfo.RemoveAll();
	pDC->SelectObject(sFont);
	pDC->SelectObject(sPen);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
}

bool CBongBase::CalculateDisplay(double &rdDiv, double dMax, double dMin, double& rdDrawMinValue, 
				int iDispTick, double* pdDispValue, double* pdLogValue, int& riDispCount)
{
	double	dMok = dMin / rdDiv;
	CString	strTemp;
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
		if (m_iGrpKind == GK_VOL || m_iGrpKind == GK_VMA || m_iGrpKind == GK_AMT || m_iGrpKind == GK_AMA)
		{
			if (iDrawNum <= iDispTick * 2)
				rdDiv *= 2;
			else if (iDrawNum <= iDispTick * 4)
				rdDiv *= 4;
			else if (iDrawNum <= iDispTick * 5)
				rdDiv *= 5;
			else
				rdDiv *= 10;
		}
		else
		{
			CString	tStr;
			tStr.Format("%f", rdDiv);
			if (tStr.GetAt(0) == '5')	// 첫글자가 5?
			{
				if (iDrawNum <= iDispTick * 2)
					rdDiv *= 2;
				else if (iDrawNum <= iDispTick * 10)
					rdDiv *= 10;
				else if (iDrawNum <= iDispTick * 20)
					rdDiv *= 20;
				else
					rdDiv *= 100;
			}
			else
			{

				if (iDrawNum <= iDispTick * 5)
					rdDiv *= 5;
				else if (iDrawNum <= iDispTick * 10)
					rdDiv *= 10;
				else if (iDrawNum <= iDispTick * 50)
					rdDiv *= 50;
				else 
					rdDiv *= 100;

/*
				if (iDrawNum <= iDispTick * 5)
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
*/
			}
		}

		dMok = dMin / rdDiv;
		strTemp.Format("%f", dMok);
		iIndex = strTemp.Find('.'); 
		dMok = atof(strTemp.Left(iIndex));
		rdDrawMinValue = dMok * rdDiv;
		if (rdDrawMinValue < dMin)		
			rdDrawMinValue += rdDiv;
	}



	riDispCount = 0;
	pdDispValue[riDispCount++] = dMin;
	pdDispValue[riDispCount] = rdDrawMinValue;
	bool bLog = IsLogChart();
	if (bLog)
	{
		if (dMin > 0)
			pdLogValue[0] = double(1000. * double(log(dMin)));
		else if (dMin < 0)
			pdLogValue[0] = (-1) * double(1000. * double(log(dMin * (-1))));
		else
			pdLogValue[0] = 0;

		if (rdDrawMinValue > 0)
			pdLogValue[riDispCount] = double(1000. * double(log(rdDrawMinValue)));
		else if (rdDrawMinValue< 0)
			pdLogValue[riDispCount] = (-1) * double(1000. * double(log(rdDrawMinValue * (-1))));
		else
			pdLogValue[riDispCount] = 0;
	}

	double dVal = 0.0;
	while(1)
	{
		if (pdDispValue[riDispCount] > dMax + 0.01)
			break;

		pdDispValue[riDispCount + 1] = pdDispValue[riDispCount] + rdDiv;
		if (bLog)
		{
			dVal = pdDispValue[riDispCount + 1];
			if (dVal > 0)
				pdLogValue[riDispCount + 1] = double(1000. * double(log(dVal)));
			else if (dVal < 0)
				pdLogValue[riDispCount + 1] = (-1) * double(1000. * double(log(dVal * (-1))));
			else
				pdLogValue[riDispCount + 1] = 0;
		}
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

void CBongBase::DrawTick(CDC* pDC)
{
	bool	bDrawMin = true;

	switch (m_iGrpKind)
	{
	case GK_VOL:
	case GK_VMA:
		if (CIndcBase::m_pCoDraw && CIndcBase::m_pCoDraw->coWork == MC_VOL)
		{
			m_dYMax = m_pCoDraw->VMax;
			m_dYMin = m_pCoDraw->VMin;
		}
		break;
	case GK_AMT:
	case GK_AMA:
		if (CIndcBase::m_pCoDraw && CIndcBase::m_pCoDraw->coWork == MC_AMT)
		{
			m_dYMax = m_pCoDraw->AMax;
			m_dYMin = m_pCoDraw->AMin;
		}
		break;
	default:
		if (CIndcBase::m_pCoDraw && CIndcBase::m_pCoDraw->coWork == MC_PRC)
		{
			m_dYMax = m_pCoDraw->PMax;
			m_dYMin = m_pCoDraw->PMin;
		}
		break;
	}

	if (m_dYMax == DBL_MIN || m_dYMin == DBL_MAX)
		return;


//	if (m_pgraph->dwDraOption & GO_TKBOTTOM)
//		DrawTickVertical(pDC);
	if (m_pgraph->dwDraOption & GO_TKLEFT)
		DrawTickLeft(pDC, m_dYMax, m_dYMin, bDrawMin);
	if (m_pgraph->dwDraOption & GO_TKRIGHT)
		DrawTickRight(pDC, m_dYMax, m_dYMin, bDrawMin);
}

void CBongBase::DrawLine(CDC *pDC, double Max, double Min, double pValue[], int style, int width, COLORREF color, int index)
{
	int ii = 0;

	for (ii = 0; ii < m_iDispEPos - m_iDispSPos; ii++)
	{
		if (pValue[ii+m_iDispSPos] != GD_UNUSED)
			break;
	}
	
	int	nPoint = m_iDispEPos - m_iDispSPos - ii;
	if (nPoint <= 0)
		return;

	CPoint	*pRgnPoint = new CPoint[nPoint * 2];

	CPen	*cPen = GetPen(style, width, color);
	CPen	*sPen = pDC->SelectObject(cPen);

	int DrawHeight = m_rectGrp.Height();
	int DrawWidth  = m_rectGrp.Width();

	bool	bLog = IsLogChart();
	bool	bReverse = IsReverseChart();
	if (bLog)
	{
		Max = GetLogData(Max);
		Min = GetLogData(Min);
	}
	double ValueHeight = double(DrawHeight) / double(Max - Min);
	if (ValueHeight == 0.0)
		ValueHeight = 1.0;

	int drawShift = 0;
	drawShift = int((DrawWidth/m_iDispDtCnt) / 2);	// Center

	int	rgncnt = 0;
	CPoint point;
	double	dVal;
	for (; ii < m_iDispEPos - m_iDispSPos; ii++)
	{
		dVal = pValue[ii + m_iDispSPos];
		if (bLog)
			dVal = GetLogData(dVal);

		point.x = m_rectGrp.left + drawShift;
		point.x += int((DrawWidth * ii) / m_iDispDtCnt);

		if (bReverse)
		{
			point.y = m_rectGrp.bottom;
			point.y -= int(ValueHeight * (Max - dVal));
		}
		else
		{
			point.y = m_rectGrp.top;
			point.y += int(ValueHeight * (Max - dVal));
		}

		pRgnPoint[rgncnt++] = point;
	}

	pDC->Polyline(pRgnPoint, nPoint);
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
		m_RgnInfo.Rgn[m_RgnInfo.iCount].iIndex = index;
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

			point.x = pRgnPoint[idx].x;
			point.y = pRgnPoint[idx].y - rgnMARKGAP;

			CRect	rc;
			rc.SetRect(point.x - rgnMARKGAP, point.y - rgnMARKGAP, 
				point.x + rgnMARKGAP, point.y + rgnMARKGAP);
			pDC->Rectangle(&rc);
		}
		pDC->SetROP2(sROP);
		pDC->SelectObject(srgnBrush);
	}

	delete[] pRgnPoint;
	pRgnPoint = NULL;
}

bool CBongBase::CalculateVolMinMax()
{
	double tmpMax, tmpMin;

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return false;

	if (pOrgData->GetPattern() != dpBASIC)
		return false;

	int ii = 0;
	CGrpBasic* gBasic;
	for (ii = 0; ii < m_iTtlDtCnt; ii++)
	{
		gBasic = pOrgData->GetGraphData(ii);
		if (gBasic->m_iClose != GD_UNUSED)
			break;
	}

	if (m_iTtlDtCnt < 5)
	//if (m_iTtlDtCnt < 5 || ii + 5 >= m_iTtlDtCnt)
		return false;

	if (ii + 5 >= m_iTtlDtCnt)
		return true;

	m_dMaxVolValue = m_dMaxVolMav = DBL_MIN;
	m_dMinVolValue = m_dMinVolMav = DBL_MAX;

	int	start = m_iDispSPos;
	int	end = m_iDispEPos;
	if (!(m_dwDspOpt & DO_SCREEN))
	{
		start = 0;
		end = m_iTtlDtCnt;
	}
	if (start < ii)
		start = ii;

	double	*pMAV = new double[m_iTtlDtCnt];
	for (int jj = 0; jj < ii + 5; jj++)
		pMAV[jj] = 0;

	double	dMA;
	for (ii = ii + 4; ii < m_iTtlDtCnt; ii++)
	{
		dMA = 0;
		for (int jj = ii; jj >= ii - 4; jj--)
		{
			gBasic = pOrgData->GetGraphData(jj);
			dMA += gBasic->m_dVolume;
		}
		pMAV[ii] = dMA / 5.;
	}

	for (ii = start; ii < end; ii++)
	{
		gBasic = pOrgData->GetGraphData(ii);
		tmpMax = tmpMin = gBasic->m_dVolume;

		if (gBasic->m_dVolume > 0)
		{
			if (gBasic->m_dVolume > m_dMaxVolValue)
				m_dMaxVolValue = gBasic->m_dVolume;
			if (gBasic->m_dVolume < m_dMinVolValue)
				m_dMinVolValue = gBasic->m_dVolume;
		}

		if (pMAV[ii] > 0 && pMAV[ii] > m_dMaxVolMav)
			m_dMaxVolMav = pMAV[ii];
		if (pMAV[ii] > 0 && pMAV[ii] < m_dMinVolMav)
			m_dMinVolMav = pMAV[ii];
	}
	if (m_dMaxVolValue == DBL_MIN || m_dMinVolValue == DBL_MAX)
		return false;

	if (m_dMaxVolMav != DBL_MIN && m_dMinVolMav != DBL_MAX)
	{
		if (m_dMaxVolValue < m_dMaxVolMav)
			m_dMaxVolValue = m_dMaxVolMav;
		if (m_dMinVolValue > m_dMinVolMav)
			m_dMinVolValue = m_dMinVolMav;
	}

	m_dMinVolValue = 0;
	delete pMAV;

	return true;
}

bool CBongBase::CalculateVol()
{
	int DrawWidth  = m_rectGrp.Width();

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))
		return false;

	if (pOrgData->GetPattern() != dpBASIC)
		return false;

	int cnt = m_iDispEPos - m_iDispSPos;
	
	if (m_piVol)
	{
		delete m_piVol;	m_piVol = NULL;
	}

	m_piVol = new int[cnt + 1];
	m_dUnit = (m_dMaxVolValue - m_dMinVolValue) / 40;
	m_dTotal = 0.0;
	CGrpBasic* gBasic;
	for (int ii = 0; ii < cnt; ii++)
	{
		gBasic = pOrgData->GetGraphData(ii+m_iDispSPos);
		if (	gBasic->m_iOpen == GD_UNUSED
			|| gBasic->m_iHigh == GD_UNUSED
			|| gBasic->m_iLow == GD_UNUSED
			|| gBasic->m_iClose == GD_UNUSED)
			continue;
		m_piVol[ii] = rangeValue(gBasic->m_dVolume);
		m_dTotal += m_piVol[ii];
	}

	if (cnt <= 0)
		return false;

	return true;
}

int CBongBase::rangeValue(double trn)
{
	int mok = int(trn / m_dUnit + 0.5);
	if (mok == 0)	mok = 1;

	return mok;
}

void CBongBase::SetGrpInfo(struct _graph *pGrp, struct _envInfo* pInfo, struct _CoDraw* pCoDraw)
{
	bool bChangeVar = false;
	if (pGrp)
	{
		CopyMemory(m_pgraph, pGrp, SZ_GRAPH);
		m_crText = m_pgraph->rgbLabel;

		m_iUTickNum = 0;
		DWORD dwChkBit = 0x00000001;
		for (int ii = 0; ii < MAX_LINE; ii++, dwChkBit <<= 1)
		{
			if (m_pgraph->tInfo.wCOption & dwChkBit)
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
		int iGKind = GetGraphKind();
		switch (iGKind)
		{
		case GK_JPN:
		case GK_BAR:
		case GK_LIN:
		case GK_POLE:
		case GK_VOL:
		case GK_AMT:
		case GK_CDV:
		case GK_CDVV:
		case GK_EQV:
		case GK_EQVV:
		case GK_BALANCE:
		case GK_AVOL:
			CalculateMinMax(true);
			break;
		default:
			{
				class CAppData *pAppData;
				if (m_pDataMgr->GetData(m_iDataKey, pAppData))
				{
					pAppData->SetVariable(&m_pgraph->cInfo, m_pgraph->wCalOption);
					CalculateMinMax(true);
				}
			}
			break;
		}

	}

	if (pCoDraw)
		m_pCoDraw = pCoDraw;
}

void CBongBase::GetPtToEndvalPt(CPoint pt, CPoint &ept)
{
	ept = pt;
	double  posValue = 0;

	double	Max = m_dYMax;
	double	Min = m_dYMin;
/*	CString	sVal;
	FormatVariableComma(sVal, Max, Max, 1);
	Max = atof(sVal);
	FormatVariableComma(sVal, Min, Max, 1);
	Min = atof(sVal);
*/
	int dispCount = GetDisplayEnd() - GetDisplayPos();
	CRect	DrawRect = m_rectGrp;
	if (m_iGrpKind == GK_BALANCE)
	{
		int	BalanceGap = GetValue();
		if (BalanceGap > 0)
			DrawRect.right -= int(double(m_rectGrp.Width()*BalanceGap) / double(m_iDispDtCnt + BalanceGap));
	}

	double	pos = double((pt.x - DrawRect.left) * dispCount) / double(DrawRect.Width());
	pos += GetDisplayPos();

	int DrawHeight = DrawRect.Height();

	if (IsLogChart())
	{
		Max = GetLogData(Max);
		Min = GetLogData(Min);
	}
	double  ValueHeight = Max - Min;
	if (!ValueHeight) ValueHeight = 1;

	class COrgData *pOrgData;
	if (!m_pDataMgr->GetData(m_iDataKey, pOrgData))	return;

	CGrpBasic* gBasic = pOrgData->GetGraphData(int(pos));
	if (!gBasic)	return;

	double	epr = gBasic->m_iClose;
	if (IsLogChart())
		epr = GetLogData(epr);

	if (IsReverseChart())
	{
		ept.y = DrawRect.bottom;
		ept.y -= int((DrawHeight * (Max - epr)) / ValueHeight);	
	}
	else
	{
		ept.y = DrawRect.top;
		ept.y += int((DrawHeight * (Max - epr)) / ValueHeight);	
	}
}