// VarsBase.cpp: implementation of the CVarsBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VarsBase.h"
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

CVarsBase::CVarsBase(CWnd* pwndView, CWnd* pwndParent, class CDataMgr* pDataMgr, char* pcInfo, char* pcGraph, int iDispDay)
: CIndcBase(pwndView, pwndParent, pDataMgr, pcInfo, pcGraph, iDispDay)
{
	class COrgData *pOrgData;
	if (m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
	{
		m_iTtlDtCnt = pOrgData->GetArraySize();

		//m_iDispDtCnt = m_iTtlDtCnt;
		//m_iDispSPos = 0;
		//m_iDispEPos = m_iTtlDtCnt;
		if (m_pgraph->dwDraOption & GO_UNION)
		{
			if (iDispDay == 0)
				m_iDispDtCnt = m_iTtlDtCnt;
			else if (m_iTtlDtCnt < iDispDay)
				m_iDispDtCnt = m_iTtlDtCnt;
			else
				m_iDispDtCnt = iDispDay;

			EditMemberValue();
		}
		else
		{
			m_iDispDtCnt = m_iTtlDtCnt;
			m_iDispSPos = 0;
			m_iDispEPos = m_iTtlDtCnt;
		}
	}

	m_arColorX.RemoveAll();
	m_arPointX.RemoveAll();
}

CVarsBase::~CVarsBase()
{

}

CString CVarsBase::GetYposValue(CPoint pt, bool bGrid)
{
	if (bGrid)
		return _T("");

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

//void CVarsBase::DrawTick(CDC* pDC, bool bBottom)
void CVarsBase::DrawTick(CDC* pDC)
{
	if (m_pgraph->dwDraOption & GO_TKLEFT)
		DrawTickLeft(pDC, m_dYMax, m_dYMin, false);
	if (m_pgraph->dwDraOption & GO_TKRIGHT)
		DrawTickRight(pDC, m_dYMax, m_dYMin, false);

	//if ((m_pgraph->dwDraOption & GO_TKBOTTOM) && bBottom)
	//	DrawTickVertical(pDC);
}

void CVarsBase::DrawTickRight(CDC* pDC, double dMax, double dMin, bool bDrawMin)
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
	
	double	dDiv;
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
	double dVal = 0.0;
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
	int iPivotY = rectTick.left + gapTICK + size.cx;

	bool bDrawLine = (m_dwDspOpt & DO_TKHGRID)? true : false;

	rectTickSave.top = rectTick.bottom;

	double dValue;
	CRect rectText;
	CPoint pointStart;
	CPoint pointEnd;
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
			pDC->SelectObject(ppenTick);
			pDC->MoveTo(pointEnd);
			pDC->LineTo(pointStart);
		}

		pDC->SelectObject(ppenLine);
		pDC->MoveTo(m_rectOrgGrp.right, pointStart.y);
		pDC->LineTo(m_rectOrgGrp.right + lenTICK, pointStart.y);

		FormatVariableComma(strDisp, adDispValue[ii], dMax, iMaxDigit);

		if (ii == 0)
		{
			CString	str = GetTickUnit(dMax);
			if (str.IsEmpty())
			{
				size = pDC->GetOutputTextExtent(strDisp);
				rectText.SetRect(rectTick.left + gapTICK, pointStart.y - size.cy/2,
					iPivotY, pointStart.y + size.cy/2);

				if (rectText.bottom - rectTickSave.top >= 5)	continue;

				if (rectText.bottom > rectTickSave.top)
					rectText.OffsetRect(0, rectTickSave.top - rectText.bottom);
				rectText.IntersectRect(&rectText, &rectOrgTick);
				pDC->DrawText(strDisp, &rectText, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
			}
			else
			{
				strDisp = str;
				size = pDC->GetOutputTextExtent(strDisp);
				rectText.SetRect(m_rectOrgGrp.right + lenTICK, rectTick.bottom - size.cy,
					rectTick.right, rectTick.bottom);
				rectText.IntersectRect(&rectText, &rectOrgTick);
				pDC->DrawText(strDisp, &rectText, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
			}
		}
		else
		{
			size = pDC->GetOutputTextExtent(strDisp);
			rectText.SetRect(rectTick.left + gapTICK, pointStart.y - size.cy/2,
				iPivotY, pointStart.y + size.cy/2);

			if (rectText.bottom - rectTickSave.top >= 5)	continue;

			if (rectText.bottom > rectTickSave.top)
				rectText.OffsetRect(0, rectTickSave.top - rectText.bottom);
			rectText.IntersectRect(&rectText, &rectOrgTick);
			pDC->DrawText(strDisp, &rectText, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
		}
		rectTickSave = rectText;
	}

	if (dDiv != 1)
	{
		rectTick = m_TkInfo.tkRect[TICK_RIGHT];
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
		for (int ii = 0;;ii++)
		{
			dValue = dDrawMinValue + divS*ii;
			if (dValue >= dMax)	break;

			dValue = double(iTickHeight) * double(dMax - dValue);
			dValue /= double(dValueHeight);
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

void CVarsBase::DrawTickLeft(CDC* pDC, double dMax, double dMin, bool bDrawMin)
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
	double drawMinValue= 0.0;
	if (!CalculateDisplay(dDiv, dMax, dMin, drawMinValue, iDispTick, adDispValue, adLogValue, iDispCount))
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

	bool bDrawLine = (m_dwDspOpt & DO_TKHGRID)? true:false;

	rectTickSave.top = rectTick.bottom;

	double dValue;
	CString	strDisp;
	CPoint pointStr;
	CPoint pointEnd;
	CRect rectText;
	for (int ii = 0; ii < iDispCount; ii++)		// 가격별가로 (DOT)
	{
		pointStr.x = m_rectOrgGrp.left+1;
		dValue = adDispValue[ii];
		dValue = double(double(iTickHeight) * double(dMax - dValue));
		dValue /= double(dValueHeight);

		pointStr.y = rectTick.top + int(dValue);

		if (!rectTick.PtInRect(CPoint(pointStr.x -1, pointStr.y)))
		{
			if (rectTick.bottom < pointStr.y)
				continue;
		}

		pointEnd.x = m_rectOrgGrp.right;
		pointEnd.y = pointStr.y;
		if (bDrawLine && pointStr.y > m_rectChart.top && pointStr.y < m_rectChart.bottom)
		{
			pDC->SelectObject(ppenTick);
			pDC->MoveTo(pointStr);
			pDC->LineTo(pointEnd);
		}
		
		pDC->SelectObject(ppenLine);
		pDC->MoveTo(m_rectOrgGrp.left, pointStr.y);
		pDC->LineTo(m_rectOrgGrp.left - lenTICK, pointStr.y);

		FormatVariableComma(strDisp, adDispValue[ii], dMax, iMaxDigit);

		if (ii == 0)
		{
			CString	str = GetTickUnit(dMax);
			if (str.IsEmpty())
			{
				size = pDC->GetOutputTextExtent(strDisp);
				rectText.SetRect(rectTick.left, pointStr.y - size.cy/2,
					rectTick.right - gapTICK, pointStr.y + size.cy/2);

				if (rectText.bottom - rectTickSave.top >= 5)	continue;
				if (rectText.bottom > rectTickSave.top)
					rectText.OffsetRect(0, rectTickSave.top - rectText.bottom);
				rectText.IntersectRect(&rectText, &rectOrgTick);
				pDC->DrawText(strDisp, rectText, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
			}
			else
			{
				strDisp = str;
				size = pDC->GetOutputTextExtent(strDisp);
				rectText.SetRect(rectTick.left, rectTick.bottom - size.cy,
					m_rectOrgGrp.left - lenTICK, rectTick.bottom);
				rectText.IntersectRect(&rectText, &rectOrgTick);
				pDC->DrawText(strDisp, rectText, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
			}
		}
		else
		{
			size = pDC->GetOutputTextExtent(strDisp);
			rectText.SetRect(rectTick.left, pointStr.y - size.cy/2,
				rectTick.right - gapTICK, pointStr.y + size.cy/2);

			if (rectText.bottom - rectTickSave.top >= 5)	continue;
			if (rectText.bottom > rectTickSave.top)
				rectText.OffsetRect(0, rectTickSave.top - rectText.bottom);
			rectText.IntersectRect(&rectText, &rectOrgTick);
			pDC->DrawText(strDisp, rectText, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
		}
		rectTickSave = rectText;
	}

	if (dDiv != 1)
	{
		rectTick = m_TkInfo.tkRect[TICK_LEFT];

		pDC->SelectObject(ppenLine);
		strDisp.Format("%f", dDiv);
		double	dDivSave = dDiv;
		if (strDisp.GetAt(0) == '5')
			dDivSave = dDiv / 5.;
		else
			dDivSave = dDiv / 2.;

		GetMinMaxValue(dMin, dMax);
		dValueHeight = dMax - dMin;

		CString	strTemp;
		double	dMok = dMin / dDivSave;
		strTemp.Format("%f", dMok);
		int iIndex = strTemp.Find('.'); 
		dMok = atof(strTemp.Left(iIndex));
		drawMinValue = dMok * dDivSave;
		if (drawMinValue < dMin)		
			drawMinValue += dDivSave;
		for (int ii = 0;;ii++)
		{
			dValue = drawMinValue + dDivSave*ii;
			if (dValue >= dMax)
				break;

			dValue = double(iTickHeight) * double(dMax - dValue);
			dValue /= double(dValueHeight);
			pointStr.y = rectTick.top + int(dValue);
			
			pDC->MoveTo(m_rectOrgGrp.left, pointStr.y);
			pDC->LineTo(m_rectOrgGrp.left-5, pointStr.y);
		}
	}

	pDC->SelectObject(pfontOld);
	pDC->SelectObject(ppenOld);
	pDC->SetTextColor(rgbOldText);
	pDC->SetBkMode(iOldMode);
}

bool CVarsBase::DrawLineTickBottom(CDC* pDC, double MaxV, double MinV, int dispTICK, bool bTRN)
{
	if (!(m_pgraph->dwDraOption & GO_TKBOTTOM))
	{
		TRACE("DrawLineTickBottom Function Error\n");
		return false;
	}

	int	dispCount;
	int	dispValue[32];

	int	MaxValue = int(MaxV);
	int	MinValue = int(MinV);

	if (MaxV > double(dispUNIT))
	{
		MaxValue = int(MaxV / double(1000));
		MinValue = int(MinV / double(1000));
	}

	CRect	tickRC = m_TkInfo.tkRect[TICK_BOTTOM];

	int	tickHeight = tickRC.Height();
	int	tickWidth  = tickRC.Width();

	double ValueWidth = MaxValue - MinValue;
	if (ValueWidth <= 0) ValueWidth = 1;

	int nextTick = int(ValueWidth / double(dispTICK));

	dispCount = 0;
	dispValue[dispCount] = int(MinValue);
	while(1)
	{
		if (dispValue[dispCount] > int(MaxValue))
			break;
		dispValue[dispCount+1] = dispValue[dispCount] + nextTick;
		dispCount++;
	}
	if (dispCount <= 0)
		return false;

	int sMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF sColor = pDC->SetTextColor(m_atick[TICK_BOTTOM].dwTColor);

	int		style = m_lineH.btLStyle;
	int		width = m_lineH.btLWidth;
	COLORREF	color = m_lineH.rgbLColor;

	CString dispStr;
	CFont	*tkFont = GetFont(TICK_BOTTOM);
	CFont	*sFont = pDC->SelectObject(tkFont);
	CPen	*cPen = GetPen(style, width, color);
	CPen	*sPen = pDC->SelectObject(cPen);

	double	dValue;
	CPoint	pointPOS;
	CRect	tkSaveRC(0, 0, 0, 0);
	CRect	drawRC;
	CSize	size;

	bool	bDrawLine = (m_dwDspOpt & DO_TKHGRID)? true:false;
	for (int ii = 0; ii < dispCount; ii++)
	{
		pointPOS.x = tickRC.right;
		dValue = double(double(tickWidth) * double(MaxValue - dispValue[ii]));
		dValue /= double(ValueWidth);
		pointPOS.x -= int(dValue);
		pointPOS.y = tickRC.top + int(tickHeight / 5);

		if (bDrawLine && pointPOS.x > tickRC.left && pointPOS.x < tickRC.right)
		{
			pDC->MoveTo(pointPOS.x, m_rectChart.top+1);// 세로선(점선)
			pDC->LineTo(pointPOS.x, m_rectChart.bottom);
		}

		CIndcBase::FormatVariableComma(dispStr, dispValue[ii], int(MaxValue));
		size = pDC->GetOutputTextExtent(dispStr);

		drawRC.SetRect(pointPOS.x - size.cx/2, pointPOS.y, 
			pointPOS.x + (size.cx + 1)/2, pointPOS.y + size.cy);

		if (drawRC.left < m_TkInfo.tkRect[TICK_BOTTOM].left)
			drawRC.OffsetRect(m_TkInfo.tkRect[TICK_BOTTOM].left - drawRC.left, 0);
		if (drawRC.right > m_TkInfo.tkRect[TICK_BOTTOM].right)
			drawRC.OffsetRect(m_TkInfo.tkRect[TICK_BOTTOM].right - drawRC.right, 0);
		if (tkSaveRC.PtInRect(CPoint(drawRC.left, drawRC.top)))
			continue;

		pDC->DrawText(dispStr, &drawRC, DT_SINGLELINE|DT_TOP|DT_LEFT);
		tkSaveRC = drawRC;
	}

	CRect	tkUnit;
	tkUnit.SetRect(tickRC.right +2, tickRC.top + (tickHeight / 5),
			m_rectObj.right, tickRC.bottom);

	if (bTRN)
	{
		switch (GetDataUnit())
		{
		case GU_CODE:
			dispStr = _T("[ 주 ]");
			if (MaxV > double(dispUNIT))
				dispStr = _T("[천주]");
			break;
		case GU_INDEX:
			dispStr = _T("[천주]");
			if (MaxV > double(dispUNIT))
				dispStr = _T("[백만]");
			break;
		case GU_FUTURE:
		case GU_OPTION:
		case GU_COFUT:
		case GU_COOPT:
		case GU_CFUTURE: // 20090917 : ADD
		case GU_FFUTURE: // 20090917 : ADD
			dispStr = _T("[계약]");
			break;
		}
	}
	else
	{
		dispStr = _T("[ 원 ]");
		if (MaxV > double(dispUNIT))
			dispStr = _T("[천원]");
	}
	int Split = m_strLegend.Find("\\n");
	if (Split != -1)
	{
		dispStr = m_strLegend.Mid(Split + 2);
	}

	pDC->DrawText(dispStr, &tkUnit, DT_LEFT|DT_SINGLELINE|DT_TOP);
	
	pDC->SelectObject(sFont);
	pDC->SelectObject(sPen);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);

	return true;
}

bool CVarsBase::CalculateDisplay(double &rdDiv, double dMax, double dMin, double& rdDrawMinValue, 
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
		CString	strText;
		strText.Format("%f", rdDiv);
		if (strText.GetAt(0) == '5')
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

void CVarsBase::DrawBottomStick(CDC* pDC)
{
	if (m_arPointX.GetSize() < 1)
		return;

	CRgn rgnDraw;
	rgnDraw.CreateRectRgnIndirect(m_rectGrp);
	pDC->SelectClipRgn(&rgnDraw);

	CRect rect;
	for (int ii = 0; ii < m_arPointX.GetSize() - 1; ii++)
	{
		rect.SetRect(m_arPointX.GetAt(ii), m_rectGrp.bottom - 10, m_arPointX.GetAt(ii + 1), m_rectGrp.bottom);
		pDC->FillSolidRect(rect, m_arColorX.GetAt(ii));
	}

	pDC->SelectClipRgn(NULL);
	rgnDraw.DeleteObject();


	m_arPointX.RemoveAll();
	m_arColorX.RemoveAll();
}