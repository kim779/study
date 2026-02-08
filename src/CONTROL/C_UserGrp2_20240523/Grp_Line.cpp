#include "stdafx.h"
#include "C_UserGrp.h"
#include "Grp_Line.h"

#include "GrpWnd.h"
#include "Grp_Data.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CGrp_Line::CGrp_Line(CWnd *pView, class CGrpWnd *pGrpWnd, char *pInfo)
: CGrp_Base(pView, pGrpWnd, pInfo)
{
	CalculateMinMax();
}

CGrp_Line::~CGrp_Line()
{

}

void CGrp_Line::DrawChart(CDC* pDC)
{
	if(m_bHide == FALSE)
	{
		switch (m_gKind)
		{
		case IGK_LINE:
			DrawLineChart(pDC);
			break;
		case IGK_DOT:
			DrawDotChart(pDC);
			break;
		case IGK_YBASELINE:
			DrawPivotChart(pDC);
			break;
		}
	}
}

void CGrp_Line::DrawLineChart(CDC *pDC)
{
	const	int	dispPos = m_pGrpWnd->m_dispPos;
	const	int	dispEnd = m_pGrpWnd->m_dispEnd;
	const	int	dispDay = m_pGrpWnd->m_dispDay;
	const	int	DrawHeight = m_DrawRect.Height();
	const	int	DrawWidth  = m_DrawRect.Width();
	const	int	drawShift = (int)((DrawWidth/dispDay) / 2);	// Center

	CPen	*cPen = m_pApp->GetPen(m_pView, m_LineStyle, m_LineWidth, m_tRGB);
	CPen	*sPen = pDC->SelectObject(cPen);

	double ValueHeight = m_Max - m_Min;
	if (!ValueHeight) ValueHeight = 1;

	ZeroMemory(&m_MaxDayIndex, sz_cgNIndex);
	ZeroMemory(&m_MinDayIndex, sz_cgNIndex);
	m_dispMaxVal = INT_MIN;
	m_dispMinVal = INT_MAX;

	CPoint	pt;
	bool	bInit = true;
	double	val = 0;

	if (dispDay == 1)
	{
		const auto gVal = m_pGrpWnd->m_pDataInfo->GetGraphData(dispPos);

		val = gVal->vData[m_dKey];
		if (val != UNUSED_VALUE)
		{
			pt.x = m_DrawRect.left + drawShift;
			pt.y = m_DrawRect.top;
			pt.y += (int)((DrawHeight * (m_Max - val)) / ValueHeight);
			pDC->SetPixel(pt, m_tRGB);
		}
		
		pDC->SelectObject(sPen);

		return;
	}

	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		const auto gVal = m_pGrpWnd->m_pDataInfo->GetGraphData(ii+dispPos);

		val = gVal->vData[m_dKey];
		if (val == UNUSED_VALUE)
			continue;

		pt.x = m_DrawRect.left + drawShift;			
		pt.x += (int)((DrawWidth * ii) / dispDay);
		pt.y = m_DrawRect.top;
		pt.y += (int)((DrawHeight * (m_Max - val)) / ValueHeight);

		if (val > m_dispMaxVal)
		{
			m_dispMaxVal = val;
			m_MaxDayPos = pt;
			CopyMemory(&m_MaxDayIndex, &gVal->index, sz_cgNIndex);
		}
		if (val < m_dispMinVal)
		{
			m_dispMinVal = val;
			m_MinDayPos = pt;
			CopyMemory(&m_MinDayIndex, &gVal->index, sz_cgNIndex);
		}

		if (bInit)
		{
			pDC->MoveTo(pt);
			bInit = false;
		}
		else
			pDC->LineTo(pt);
	}
	
	pDC->SelectObject(sPen);

	if (m_pGrpWnd->m_bMinMax)	DrawMinMaxDay(pDC);
}

void CGrp_Line::DrawDotChart(CDC *pDC)
{
	const	int	dispPos = m_pGrpWnd->m_dispPos;
	const	int	dispEnd = m_pGrpWnd->m_dispEnd;
	const	int	dispDay = m_pGrpWnd->m_dispDay;
	const	int	DrawHeight = m_DrawRect.Height();
	const	int	DrawWidth  = m_DrawRect.Width();
	const	int	drawShift = (int)((DrawWidth/dispDay) / 2);	// Center

	CPen	*cPen = m_pApp->GetPen(m_pView, m_LineStyle, m_LineWidth, m_tRGB);
	CPen	*sPen = pDC->SelectObject(cPen);
	CBrush	*cBrush = m_pApp->GetBrush(m_pView, m_tRGB);
	CBrush	*sBrush = pDC->SelectObject(cBrush);

	double ValueHeight = m_Max - m_Min;
	if (!ValueHeight) ValueHeight = 1;

	ZeroMemory(&m_MaxDayIndex, sz_cgNIndex);
	ZeroMemory(&m_MinDayIndex, sz_cgNIndex);
	m_dispMaxVal = INT_MIN;
	m_dispMinVal = INT_MAX;

	CPoint	pt;
	double	val = 0;

	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		const auto gVal = m_pGrpWnd->m_pDataInfo->GetGraphData(ii+dispPos);

		val = gVal->vData[m_dKey];
		if (val == UNUSED_VALUE)
			continue;

		pt.x = m_DrawRect.left + drawShift;			
		pt.x += (int)((DrawWidth * ii) / dispDay);
		pt.y = m_DrawRect.top;
		pt.y += (int)((DrawHeight * (m_Max - val)) / ValueHeight);

		if (val > m_dispMaxVal)
		{
			m_dispMaxVal = val;
			m_MaxDayPos = pt;
			CopyMemory(&m_MaxDayIndex, &gVal->index, sz_cgNIndex);
		}
		if (val < m_dispMinVal)
		{
			m_dispMinVal = val;
			m_MinDayPos = pt;
			CopyMemory(&m_MinDayIndex, &gVal->index, sz_cgNIndex);
		}

		pDC->Ellipse(pt.x-2, pt.y-2, pt.x+2, pt.y+2);
	}
	
	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);

	if (m_pGrpWnd->m_bMinMax)	DrawMinMaxDay(pDC);
}

void CGrp_Line::DrawPivotChart(CDC *pDC)
{
	const	int	dispPos = m_pGrpWnd->m_dispPos;
	const	int	dispEnd = m_pGrpWnd->m_dispEnd;
	const	int	dispDay = m_pGrpWnd->m_dispDay;
	const	int	DrawHeight = m_DrawRect.Height();
	const	int	DrawWidth  = m_DrawRect.Width();
	const	int	drawShift = (int)((DrawWidth/dispDay) / 2);	// Center

	CPen	*uPen = m_pApp->GetPen(m_pView, m_LineStyle, m_LineWidth, m_clrUp);
	CPen	*dPen = m_pApp->GetPen(m_pView, m_LineStyle, m_LineWidth, m_clrDn);
	CPen	*sPen = pDC->SelectObject(uPen);

	double ValueHeight = m_Max - m_Min;
	if (!ValueHeight) ValueHeight = 1;

	double epr = 0;
	CPoint ptEPR, ptPRE;

	ZeroMemory(&m_MaxDayIndex, sz_cgNIndex);
	ZeroMemory(&m_MinDayIndex, sz_cgNIndex);
	m_dispMaxVal = INT_MIN;
	m_dispMinVal = INT_MAX;

	int	xPivot = 0;
	const	int	iPivot = m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->pivot;
	const	int	yPivot = m_DrawRect.top + (int)((DrawHeight * (m_Max - (double)iPivot)) / ValueHeight);
	const	int	bPivot = (iPivot <= m_Max && iPivot >= m_Min)? true:false;

	if (bPivot)
		DrawPivot(pDC, yPivot);

	const auto gVal = m_pGrpWnd->m_pDataInfo->GetGraphData(dispPos);
	ptEPR.x = m_DrawRect.left + drawShift;
	ptEPR.y = m_DrawRect.top;
	ptEPR.y += (int)((DrawHeight * (m_Max - gVal->vData[m_dKey])) / ValueHeight);

	if (yPivot < ptEPR.y)	pDC->SelectObject(dPen);
	pDC->MoveTo(ptEPR);
	ptPRE = ptEPR;

	if (bPivot)
	{
		for (int ii = 1; ii < dispEnd - dispPos; ii++)
		{
			const auto gVal = m_pGrpWnd->m_pDataInfo->GetGraphData(ii+dispPos);
			epr = gVal->vData[m_dKey];

			if (epr < m_Min || epr > m_Max)
				continue;

			ptEPR.x = m_DrawRect.left + drawShift;
			ptEPR.x += (int)((DrawWidth * ii) / dispDay);
			ptEPR.y = m_DrawRect.top;
			ptEPR.y += (int)((DrawHeight * (m_Max - epr)) / ValueHeight);

			if (yPivot < ptPRE.y && yPivot < ptEPR.y)		// down->down
			{
				pDC->SelectObject(dPen);
				pDC->LineTo(ptEPR);
			}
			else if (yPivot >= ptPRE.y && yPivot >= ptEPR.y)	// up->up
			{
				pDC->SelectObject(uPen);
				pDC->LineTo(ptEPR);
			}
			else if (yPivot >= ptPRE.y && yPivot < ptEPR.y)		// up->down
			{
				xPivot = ptPRE.x + (yPivot-ptPRE.y)*(ptEPR.x-ptPRE.x)/(ptEPR.y-ptPRE.y);
				pDC->LineTo(xPivot, yPivot);
				pDC->SelectObject(dPen);
				pDC->LineTo(ptEPR);
			}
			else							// down->up
			{
				xPivot = ptPRE.x + (yPivot-ptPRE.y)*(ptEPR.x-ptPRE.x)/(ptEPR.y-ptPRE.y);
				pDC->LineTo(xPivot, yPivot);
				pDC->SelectObject(uPen);
				pDC->LineTo(ptEPR);
			}

			ptPRE = ptEPR;
		}
	}
	else
	{
		for (int ii = 1; ii < dispEnd - dispPos; ii++)
		{
			const auto gVal = m_pGrpWnd->m_pDataInfo->GetGraphData(ii+dispPos);
			epr = gVal->vData[m_dKey];

			if (epr < m_Min || epr > m_Max)
				continue;

			ptEPR.x = m_DrawRect.left + drawShift;
			ptEPR.x += (int)((DrawWidth * ii) / dispDay);
			ptEPR.y = m_DrawRect.top;
			ptEPR.y += (int)((DrawHeight * (m_Max - epr)) / ValueHeight);
			pDC->LineTo(ptEPR);
		}
	}

	pDC->SelectObject(sPen);

	if (m_pGrpWnd->m_bMinMax)	DrawMinMaxDay(pDC);
}

void CGrp_Line::DrawMinMaxDay(CDC* pDC)
{
	if (!m_MaxDayIndex.date.yy && !m_MaxDayIndex.time.hh)
		return;

	CString	valstr, dispStr;
	CRect	tkRect;

	const	COLORREF	upColor = RGB(255, 0, 0);
	const	COLORREF	dnColor = RGB(0, 0, 255);

	CPen	*usPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, upColor);
	CPen	*udPen = m_pApp->GetPen(m_pView, PS_DOT, 1, upColor);
	CPen	*dsPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, dnColor);
	CPen	*ddPen = m_pApp->GetPen(m_pView, PS_DOT, 1, dnColor);

	CSize	size = pDC->GetTextExtent("Æ½ÀÇ³ôÀÌ");
	CPen	*sPen = pDC->SelectObject(udPen);
	CONST	int	sMode = pDC->SetBkMode(TRANSPARENT);
	CONST	COLORREF sColor = pDC->SetTextColor(upColor);
	CString	MaxDayStr, MinDayStr;

	if (m_dIndex == CDI_TICK || m_dIndex == CDI_MIN)
	{
		MaxDayStr.Format("(%02d:%02d:%02d)",
			m_MaxDayIndex.time.hh, m_MaxDayIndex.time.mm, m_MaxDayIndex.time.ss);
		MinDayStr.Format("(%02d:%02d:%02d)", 
			m_MinDayIndex.time.hh, m_MinDayIndex.time.mm, m_MinDayIndex.time.ss);
	}
	else
	{
		MaxDayStr.Format("(%d/%d)", 
			m_MaxDayIndex.date.mm, m_MaxDayIndex.date.dd);
		MinDayStr.Format("(%d/%d)", 
			m_MinDayIndex.date.mm, m_MinDayIndex.date.dd);		
	}

	CRect	rect = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].tkRect;
	rect.top = m_DrawRect.top;
	rect.bottom = m_DrawRect.bottom;
	const	int	nHeight = size.cy;

	if (nHeight * 2 < m_DrawRect.Height() + 2)
	{
		FormatVariableComma(valstr, (int)m_dispMaxVal, m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->digit);
		dispStr = valstr + MaxDayStr;

		size = pDC->GetOutputTextExtent(dispStr);
		tkRect.SetRect(m_MaxDayPos.x + 5, m_DrawRect.bottom - (size.cy*3)/2,
			m_MaxDayPos.x + 5 + size.cx, m_DrawRect.bottom - size.cy/2);

		if (tkRect.right > m_DrawRect.right)
			tkRect.OffsetRect(m_DrawRect.right - tkRect.right + 2, 0);

		pDC->MoveTo(m_MaxDayPos.x, m_MaxDayPos.y);
		pDC->LineTo(m_MaxDayPos.x, tkRect.top - 10);

		pDC->SelectObject(usPen);
		pDC->MoveTo(m_MaxDayPos.x - 3,tkRect.top - 7);
		pDC->LineTo(m_MaxDayPos.x    ,tkRect.top - 10);
		pDC->LineTo(m_MaxDayPos.x + 3,tkRect.top - 7);
		pDC->LineTo(m_MaxDayPos.x    ,tkRect.top - 10);
		pDC->LineTo(m_MaxDayPos.x    ,tkRect.top);

		if (tkRect.top < m_DrawRect.top)
			tkRect.OffsetRect(0, m_DrawRect.top - tkRect.top);
		if (tkRect.bottom > m_DrawRect.bottom)
			tkRect.OffsetRect(0, m_DrawRect.bottom - tkRect.bottom);

		pDC->DrawText(dispStr, &tkRect, DT_CENTER|DT_BOTTOM);

		pDC->SelectObject(ddPen);
		pDC->SetTextColor(dnColor);

		FormatVariableComma(valstr, (int)m_dispMinVal, m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->digit);
		dispStr = valstr + MinDayStr;

		size = pDC->GetOutputTextExtent(dispStr);
		tkRect.SetRect(m_MinDayPos.x + 5, m_DrawRect.top + size.cy/2,
			m_MinDayPos.x + 5 + size.cx, m_DrawRect.top + (size.cy*3)/2);

		if (tkRect.right > m_DrawRect.right)
			tkRect.OffsetRect(m_DrawRect.right - tkRect.right + 2, 0);

		pDC->MoveTo(m_MinDayPos.x, m_MinDayPos.y);
		pDC->LineTo(m_MinDayPos.x, tkRect.bottom + 10);

		pDC->SelectObject(dsPen);
		pDC->MoveTo(m_MinDayPos.x - 3,tkRect.bottom + 7);
		pDC->LineTo(m_MinDayPos.x    ,tkRect.bottom + 10);
		pDC->LineTo(m_MinDayPos.x + 3,tkRect.bottom + 7);
		pDC->LineTo(m_MinDayPos.x    ,tkRect.bottom + 10);
		pDC->LineTo(m_MinDayPos.x    ,tkRect.bottom);

		if (tkRect.top < m_DrawRect.top)
			tkRect.OffsetRect(0, m_DrawRect.top - tkRect.top);
		if (tkRect.bottom > m_DrawRect.bottom)
			tkRect.OffsetRect(0, m_DrawRect.bottom - tkRect.bottom);

		pDC->DrawText(dispStr, &tkRect, DT_CENTER|DT_TOP);
	}

	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
	pDC->SelectObject(sPen);
}

void CGrp_Line::DrawPivot(CDC *pDC, int yPivot)
{
	CPen	*cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, RGB(1, 196, 1));
	CPen	*sPen = pDC->SelectObject(cPen);

	pDC->MoveTo(m_DrawRect.left, yPivot);
	pDC->LineTo(m_DrawRect.right, yPivot);

	pDC->SelectObject(sPen);
}


bool CGrp_Line::CalculateMinMax()
{
	CGrp_Base::CalculateMinMax();

	double gap = m_Max - m_Min;
	if (gap != 0)
	{
		if (m_Max != INT_MIN)
		{
			m_Max += gap / 10;
			m_Min -= gap / 10;
		}
	}
	else
	{
		if (m_Max != INT_MIN)
		{
			gap = m_Max;
			m_Max += gap / 10;
			m_Min -= gap / 10;
		}
	}

	return true;
}