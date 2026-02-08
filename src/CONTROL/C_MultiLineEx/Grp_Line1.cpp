#include "stdafx.h"
#include "C_MultiLineEx.h"
#include "Grp_Line1.h"

#include "GrpWnd.h"
#include "Grp_Data.h"
#include "float.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CGrp_Line1::CGrp_Line1(CWnd *pView, class CGrpWnd *pGrpWnd, char *pInfo, char* dInfo)
: CGrp_Base(pView, pGrpWnd, pInfo, dInfo)
{
	CalculateMinMax();

	m_arColor.Add(RGB(255, 0, 0));
	m_arColor.Add(RGB(0, 0, 255));	
	m_arColor.Add(RGB(255, 128, 64));	
	m_arColor.Add(RGB(128, 64, 64));
	m_arColor.Add(RGB(0, 128, 128));
	m_arColor.Add(RGB(255, 0, 255));
	m_arColor.Add(RGB(0, 128, 64));
	m_arColor.Add(RGB(0, 0, 128));
	m_arColor.Add(RGB(128, 128, 0));
}

CGrp_Line1::~CGrp_Line1()
{
	m_arColor.RemoveAll();
}

void CGrp_Line1::DrawGraph(CDC *pDC)
{	
	if (m_Max == INT_MIN || m_Min == INT_MAX)
		return;

	switch (m_indexInRGN)
	{
	case 0:
		DrawTickHorizon(pDC, m_Max, m_Min, ctkLEFT);
		if (m_pGrpWnd->m_GrpCnt == 1)
			DrawTickHorizon(pDC, m_Max, m_Min, ctkRIGHT);
		break;
	case 1:
		DrawTickHorizon(pDC, m_Max, m_Min, ctkRIGHT);
		break;
	}

	if (m_indexInRGN == 0)
		DrawTickBottom(pDC);
	
	DrawLine(pDC);	
	DrawLegend(pDC);
}

void CGrp_Line1::DrawLine(CDC *pDC)
{	
	const int	dispPos = m_pGrpWnd->m_dispPos;
	const int	dispEnd = m_pGrpWnd->m_dispEnd;
	const int	dispDay = m_pGrpWnd->m_dispDay;

	if (dispEnd - dispPos <= 0)
		return;

	CPen* pen{};
	COLORREF color = 0;
	int	ii = 0;
	const int	nMod = m_arColor.GetSize();
	
	color = m_arColor.GetAt(m_indexInRGN % nMod);
	pen = m_pApp->GetPen(m_pView, PS_SOLID, 2, color);
	
	CPen	*sPen = pDC->SelectObject(pen);

	const int DrawHeight = m_DrawRect.Height();
	const int DrawWidth  = m_DrawRect.Width();

	double ValueHeight = m_Max - m_Min;
	if (!ValueHeight) ValueHeight = 1;


	int drawShift = 0;
	drawShift = int((DrawWidth/dispDay) / 2);	// Center

	double val{};
	CPoint pointEPR;
	struct _multi_line*	mline = nullptr;
	bool	bInit = true;
	bInit = true;

	const int	sMode = 0;
	for ( ii = 0 ; ii < dispEnd - dispPos ; ii++ )
	{
		mline = (struct _multi_line*)m_pGrpWnd->m_pDataInfo->GetGraphData(ii+dispPos);

		val = mline->data[m_index];

		if (val == UNUSED_VALUE)
			continue;

		pointEPR.x = m_DrawRect.left + drawShift;			
		pointEPR.x += int((DrawWidth * ii) / dispDay);
		pointEPR.y = m_DrawRect.top;
		pointEPR.y += int((DrawHeight * (m_Max - double(val))) / ValueHeight);

		if (bInit)
		{
			pDC->MoveTo(pointEPR);
			bInit = false;
		}
		else
			pDC->LineTo(pointEPR);	
	}

	if (m_jgga != 0.0)
	{
		CPoint	pt[2]{};
		COLORREF	rColor{};

		pt[0].y = m_DrawRect.top;
		pt[1].y = pt[0].y += int((DrawHeight * (m_Max - double(m_jgga))) / ValueHeight);
		pt[0].x = m_DrawRect.left;
		pt[1].x = m_DrawRect.right;

		rColor = GetColorReverse(color);
		pDC->MoveTo(pt[0]);
		DrawColorLine(pDC, pt[1], rColor, PS_SOLID, 1);		
	}

	pDC->SelectObject(sPen);
}

bool CGrp_Line1::CalculateMinMax()
{
	m_Max = INT_MIN;
	m_Min = INT_MAX;

	if (!m_pGrpWnd->m_pDataInfo || 
		m_pGrpWnd->m_pDataInfo[m_dKey].GetDataCount() <= 0)
		return false;

	const int	dispPos = m_pGrpWnd->m_dispPos;
	const int	dispEnd = m_pGrpWnd->m_dispEnd;
	const int	dispDay = m_pGrpWnd->m_dispDay;

	struct _multi_line*	mline = nullptr;
	double	val = 0.0;

	if (m_pGrpWnd->m_bEachTick)
	{
		for ( int ii = 0 ; ii < dispEnd - dispPos ; ii++ )
		{
			mline = (struct _multi_line*)m_pGrpWnd->m_pDataInfo->GetGraphData(ii + dispPos);
			val = mline->data[m_index];

			if (val == UNUSED_VALUE)
				continue;

			if (m_Max < val)
				m_Max = val;
			if (m_Min > val)
				m_Min = val;		
		}
	}
	else
	{
		for ( int ii = 0 ; ii < dispEnd - dispPos ; ii++ )
		{
			mline = (struct _multi_line*)m_pGrpWnd->m_pDataInfo->GetGraphData(ii + dispPos);
			for ( int jj = 0 ; jj < m_pGrpWnd->m_lcnt; jj++ )
			{
				val = mline->data[jj];

				if (val == UNUSED_VALUE)
					continue;

				if (m_Max < val)
					m_Max = val;
				if (m_Min > val)
					m_Min = val;
			}
		}
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

	return true;
}

bool CGrp_Line1::IsChangeMinMax(bool bShift)
{
	const double	m_sMax = m_Max;
	const double	m_sMin = m_Min;

	if (!CalculateMinMax())
		return false;

	if (m_sMax != m_Max || m_sMin != m_Min)
		return true;

	return false;
}

CString CGrp_Line1::GetDisplayPosData(CPoint pt)
{
	CString	dispStr = _T("");

	if (m_DrawRect.left > pt.x || m_DrawRect.right <= pt.x)
		return dispStr;

	if (!m_pGrpWnd->m_pDataInfo || 
		m_pGrpWnd->m_pDataInfo->GetDataCount() <= 0)
		return dispStr;

	const int	dispPos = m_pGrpWnd->m_dispPos;
	const int	dispEnd = m_pGrpWnd->m_dispEnd;
	const int	dispDay = m_pGrpWnd->m_dispDay;
	const int	xPosition = pt.x - m_DrawRect.left;
	const double	szOneDay = double(m_DrawRect.Width()) / double(dispDay);
	int	dataPos = int(double(xPosition) / szOneDay);
	dataPos += dispPos;

	CString	sVal, stmp, szrslt = _T("");
	const struct _multi_line*	mline = (struct _multi_line*)m_pGrpWnd->m_pDataInfo->GetGraphData(dataPos);
	double	val = 0;

	val = mline->data[m_index];

	if (m_Max > VOLUNIT)
		val *= 1000;

	if (val == UNUSED_VALUE)
		sVal = _T("N/A");
	else
		FormatVariableComma(sVal, val, m_digit);

	szrslt = m_szName + "\t" + sVal;
	
	szrslt += stmp;		 
	return szrslt;
}

void CGrp_Line1::DrawLegend(CDC* pDC)
{
	CSize		size;
	CString		title;
	const int		nMod = m_arColor.GetSize();
	CPoint		pt;	
	
	const int	LWIDTH = 10;
	CRect		lRc;
	
	if (m_pGrpWnd->m_pFont)
	{
		const int	bkMode = pDC->SetBkMode(TRANSPARENT);
		CFont	*oldfont = pDC->SelectObject(m_pGrpWnd->m_pFont);

		const COLORREF	color = m_arColor.GetAt(m_indexInRGN % nMod);
		const COLORREF	sColor = pDC->SetTextColor(color);

		if (m_indexInRGN == 0)
			pt = m_pGrpWnd->m_pRgnInfo[m_rKey]->pt = CPoint(m_RgnRect.left + 5, m_RgnRect.top + 3);
		else
			pt = m_pGrpWnd->m_pRgnInfo[m_rKey]->pt;
		
		pDC->TextOut(pt.x, pt.y, m_szName);
		size = pDC->GetTextExtent(m_szName + " ");
		pt.x += size.cx;
		lRc.SetRect(pt.x, pt.y + (size.cy/4), pt.x + LWIDTH, pt.y + size.cy - (size.cy/4));
		pDC->FillSolidRect(&lRc, color);
		pt.x += LWIDTH*2;

		m_pGrpWnd->m_pRgnInfo[m_rKey]->pt = pt;
		pDC->SetTextColor(sColor);
		pDC->SelectObject(oldfont);
		pDC->SetBkMode(bkMode);
	}
}

void CGrp_Line1::DrawColorLine(CDC* pDC, CPoint ptTo, COLORREF color, UINT nStyle, int nWidth)
{
	CPen* pen{}, * oldpen = nullptr;

	if (nStyle <= 0)
		nStyle = PS_SOLID;
	if (nWidth <= 0)
		nWidth = 1;

	pen = m_pApp->GetPen(m_pView, nStyle, nWidth, color);
	oldpen = pDC->SelectObject(pen);
	pDC->LineTo(ptTo);
	pDC->SelectObject(oldpen);
}

COLORREF CGrp_Line1::GetColorReverse(COLORREF color)
{
	int red = GetRValue(color);
	int green = GetGValue(color);
	int  blue = GetBValue(color);
	if ((red == green && green == blue)
		&& (red > 120 && red < 140) 
		&& (green > 120 && green < 140)
		&& (blue > 120 && blue < 140))
	{
		red = red - 20;
		green = green - 20;
		blue = blue - 20;
	}
	return RGB(255-red, 255-green, 255-blue);
}

CString CGrp_Line1::GetExcelData(int idx, int idx1)
{
	CString	dispStr = __T("");

	if (!m_pGrpWnd->m_pDataInfo || m_pGrpWnd->m_pDataInfo->GetDataCount() <= 0)
		return dispStr;
	
	if (idx < 0)
	{
		dispStr = m_szName;
	}
	else
	{
		CString	stmp;
		const struct _multi_line	*multi_line = (struct _multi_line*)m_pGrpWnd->m_pDataInfo->GetGraphData(idx);

		FormatVariableComma(stmp, multi_line->data[idx1], 0);
		dispStr += stmp;
	}

	return dispStr;
}