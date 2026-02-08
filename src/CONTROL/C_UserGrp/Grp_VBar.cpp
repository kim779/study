// Grp_VBar.cpp: implementation of the CGrp_VBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "C_UserGrp.h"
#include "Grp_VBar.h"

#include "GrpWnd.h"
#include "Grp_Data.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrp_VBar::CGrp_VBar(CWnd *pView, class CGrpWnd *pGrpWnd, char *pInfo)
: CGrp_Base(pView, pGrpWnd, pInfo)
{
	m_bAllSameValue = false;
	CalculateMinMax();
}

CGrp_VBar::~CGrp_VBar()
{
}

void CGrp_VBar::DrawChart(CDC* pDC)
{
	switch (m_gKind)
	{
	case IGK_2DVBAR:
	case IGK_2DUDBAR:
	case IGK_2D0UDBAR:
		Draw2DBarChart(pDC);
		break;
	case IGK_XBASEVBAR:
		DrawXBaseBar(pDC);
		break;
	default:
		Draw3DBarChart(pDC);
		break;
	}
}

void CGrp_VBar::Draw2DBarChart(CDC *pDC)
{
	const	int	dispPos = m_pGrpWnd->m_dispPos;
	const	int	dispEnd = m_pGrpWnd->m_dispEnd;
	const	int	dispDay = m_pGrpWnd->m_dispDay;
	COLORREF	upColor = m_clrUp, dnColor = m_clrDn;

	if (m_bAllSameValue)
	{
		upColor = dnColor = m_tRGB;
	}
	
	CPen	*upPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, upColor);
	CPen	*dnPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, dnColor);
	CBrush	*upBrush = m_pApp->GetBrush(m_pView, upColor);
	CBrush	*dnBrush = m_pApp->GetBrush(m_pView, dnColor);
	CPen	*sPen = pDC->SelectObject(upPen);
	CBrush	*sBrush = pDC->SelectObject(upBrush);

	const	int	DrawHeight = m_DrawRect.Height();
	const	int	DrawWidth  = m_DrawRect.Width();
	double	ValueHeight = m_Max - m_Min;

	if (!ValueHeight) ValueHeight = 1;

	double	dValue = 0;
	CRect	rectTRN;
	double	value = 0;
	bool	bRev = false;
	int	nWidth = 0;

	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		const auto gVal = m_pGrpWnd->m_pDataInfo->GetGraphData(ii+dispPos);
		value = gVal->vData[m_dKey];
	
		if (value == 0)
			continue;

		if (m_gKind == IGK_2DVBAR)
		{
			if (value > 0)
			{
				pDC->SelectObject(upPen);
				pDC->SelectObject(upBrush);
			}
			else
			{
				pDC->SelectObject(dnPen);
				pDC->SelectObject(dnBrush);
			}
			value = fabs(value);
		}
		else
		{
			if (value > 0)
			{
				bRev = false;
				pDC->SelectObject(upPen);
				pDC->SelectObject(upBrush);
			}
			else
			{
				bRev = true;
				pDC->SelectObject(dnPen);
				pDC->SelectObject(dnBrush);
			}
		}

		rectTRN.left = m_DrawRect.left;
		rectTRN.left += (int)((DrawWidth * ii) / dispDay+0.5);
		rectTRN.right = m_DrawRect.left;
		rectTRN.right += (int)((DrawWidth * (ii +1)) / dispDay+0.5);
		rectTRN.right -= 1;
		nWidth = rectTRN.Width();
		rectTRN.top = m_DrawRect.top;

		if (nWidth >= 55)
		{
			rectTRN.left += (nWidth-55)/2;
			rectTRN.right -= (nWidth-55)/2;
		}
		else
		{
			rectTRN.left += 0;
			rectTRN.right -= 1;
		}

		nWidth = rectTRN.Width();
		
		dValue = (double)DrawHeight * (m_Max - value);
		rectTRN.top += (int)(dValue / ValueHeight);
		rectTRN.bottom = m_DrawRect.top;

//		if (m_Min == 0)
//			dValue = (double)DrawHeight * (m_Max - m_Min);
//		else
			dValue = (double)DrawHeight * m_Max;
		
		rectTRN.bottom += (int)(dValue / ValueHeight);
		rectTRN.NormalizeRect();

		if (rectTRN.Width() >= 2)
		{
			CPoint	pt[5];

			pt[0].x = rectTRN.left; pt[0].y = rectTRN.top;
			pt[1].x = rectTRN.right - 1; pt[1].y = rectTRN.top;
			pt[2].x = rectTRN.right - 1; pt[2].y = rectTRN.bottom;
			pt[3].x = rectTRN.left; pt[3].y = rectTRN.bottom;
			pt[4].x = rectTRN.left; pt[4].y = rectTRN.top;

			pDC->Polygon(pt, 5);
		}
		else
		{
			pDC->MoveTo(rectTRN.left, rectTRN.top);
			pDC->LineTo(rectTRN.left, rectTRN.bottom);
		}				
	}

	// 0 Line
	if (m_gKind != IGK_2DVBAR)
	{
		CPoint	pointS, pointE;
		pointS.x = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkLEFT].tkRect.right+1;

		dValue = (double)DrawHeight * m_Max;
		pointS.y = m_DrawRect.top + (int)(dValue / ValueHeight);
		pointE.x = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkRIGHT].tkRect.left;
		pointE.y = pointS.y;

		CPen	*cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, m_pGrpWnd->m_TkLineColor);
		pDC->SelectObject(cPen);
		pDC->MoveTo(pointS);
		pDC->LineTo(pointE);
	}

	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);
}

void CGrp_VBar::Draw3DBarChart(CDC *pDC)
{
	const	int	dispPos = m_pGrpWnd->m_dispPos;
	const	int	dispEnd = m_pGrpWnd->m_dispEnd;
	const	int	dispDay = m_pGrpWnd->m_dispDay;
	COLORREF	color = 0, upColor = m_clrUp, dnColor = m_clrDn;

	if (m_bAllSameValue)
	{
		upColor = dnColor = m_tRGB;
	}
	
	CPen	*upPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, upColor);
	CPen	*dnPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, dnColor);
	CPen	*sPen = pDC->SelectObject(upPen);

	const	int	DrawHeight = m_DrawRect.Height();
	const	int	DrawWidth  = m_DrawRect.Width();
	double ValueHeight = m_Max - m_Min;

	if (!ValueHeight) ValueHeight = 1;

	double	dValue = 0;
	CRect	rectTRN;
	double	value = 0;
	bool	bRev = false;
	int	nWidth = 0;

	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		const auto gVal = m_pGrpWnd->m_pDataInfo->GetGraphData(ii+dispPos);
		value = gVal->vData[m_dKey];
	
		if (value == 0)
			continue;

		if (m_gKind == IGK_3DVBAR)
		{
			if (value > 0)
			{
				color = upColor;
				pDC->SelectObject(upPen);
			}
			else
			{
				color = dnColor;
				pDC->SelectObject(dnPen);
			}
			value = fabs(value);
		}
		else
		{
			if (value > 0)
			{
				bRev = false;
				color = upColor;
				pDC->SelectObject(upPen);
			}
			else
			{
				bRev = true;
				color = dnColor;
				pDC->SelectObject(dnPen);
			}
		}

		rectTRN.left = m_DrawRect.left;
		rectTRN.left += (int)((DrawWidth * ii) / dispDay+0.5);
		rectTRN.right = m_DrawRect.left;
		rectTRN.right += (int)((DrawWidth * (ii +1)) / dispDay+0.5);
		nWidth = rectTRN.Width();
		rectTRN.top = m_DrawRect.top;

		if (nWidth >= 50)
		{
			rectTRN.left += (nWidth-50)/2;
			rectTRN.right -= (nWidth-50)/2;
		}
		else
		{
			rectTRN.left += 0;
			rectTRN.right -= 1;
		}
/*
		if (nWidth >= 40)
		{
			rectTRN.left += 5;
			rectTRN.right -= 5;
		}
		else if (nWidth > 7)
		{
			rectTRN.left += 2;
			rectTRN.right -= 3;
		}
		else if (nWidth > 3)
		{
			rectTRN.left += 1;
			rectTRN.right -= 1;
		}
*/
		nWidth = rectTRN.Width();
		
		dValue = (double)DrawHeight * (m_Max - value);
		rectTRN.top += (int)(dValue / ValueHeight);
		rectTRN.bottom = m_DrawRect.top;

//		if (m_Min == 0)
//			dValue = (double)DrawHeight) * (m_Max - m_Min);
//		else
		dValue = (double)DrawHeight * m_Max;
		
		rectTRN.bottom += (int)(dValue / ValueHeight);
		rectTRN.NormalizeRect();

		if (rectTRN.Width() > 2)
		{
			if (bRev)
			{
				rectTRN.OffsetRect(0, nWidth / 10);
				Draw3DPole(pDC, rectTRN, nWidth / 10, nWidth / 10, color);
			}
			else
			{
				Draw3DPole(pDC, rectTRN, nWidth / 10, nWidth / 10, color);
			}
		}
		else
		{
			pDC->MoveTo(rectTRN.left, rectTRN.top);
			pDC->LineTo(rectTRN.left, rectTRN.bottom);
		}				
	}

	// 0 Line
	if (m_gKind != IGK_3DVBAR)
	{
		CPoint	pointS, pointE;

		pointS.x = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkLEFT].tkRect.right+1;
		dValue = (double)DrawHeight * m_Max;
		pointS.y = m_DrawRect.top + (int)(dValue / ValueHeight);
		pointE.x = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkRIGHT].tkRect.left;
		pointE.y = pointS.y;

		CPen	*cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, m_pGrpWnd->m_TkLineColor);
		pDC->SelectObject(cPen);
		pDC->MoveTo(pointS);
		pDC->LineTo(pointE);
	}

	pDC->SelectObject(sPen);
}

void CGrp_VBar::DrawXBaseBar(CDC *pDC)
{
	const	int	dispPos = m_pGrpWnd->m_dispPos;
	const	int	dispEnd = m_pGrpWnd->m_dispEnd;
	const	int	dispDay = m_pGrpWnd->m_dispDay;
	const	COLORREF	upColor = m_clrUp, dnColor = m_clrDn;
	CPen	*upPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, upColor);
	CPen	*dnPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, dnColor);
	CBrush	*upBrush = m_pApp->GetBrush(m_pView, upColor);
	CBrush	*dnBrush = m_pApp->GetBrush(m_pView, dnColor);
	CPen	*sPen = pDC->SelectObject(upPen);
	CBrush	*sBrush = pDC->SelectObject(upBrush);

	const	int	DrawHeight = m_DrawRect.Height();
	const	int	DrawWidth  = m_DrawRect.Width();
	double ValueHeight = m_Max - m_Min;

	if (!ValueHeight) ValueHeight = 1;

	double	dValue = 0;
	CRect	rectTRN;
	double	value = 0;
	int	nWidth = 0;

	CPoint	pt[5];
	double	dIndex = 0;
	const	double	dPivot = (double)m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->pivot;
	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		const auto gVal = m_pGrpWnd->m_pDataInfo->GetGraphData(ii+dispPos);
		value = gVal->vData[m_dKey];
		if (value < 0)
			value = 0;
		dIndex = atof(gVal->index.name.str);

		if (dIndex >= dPivot)
		{
			pDC->SelectObject(upPen);
			pDC->SelectObject(upBrush);
		}
		else
		{
			pDC->SelectObject(dnPen);
			pDC->SelectObject(dnBrush);
		}

		rectTRN.left = m_DrawRect.left;
		rectTRN.left += (int)((DrawWidth * ii) / dispDay+0.5);
		rectTRN.right = m_DrawRect.left;
		rectTRN.right += (int)((DrawWidth * (ii +1)) / dispDay+0.5);
		rectTRN.right -= 1;
		nWidth = rectTRN.Width();
		rectTRN.top = m_DrawRect.top;

		if (nWidth >= 55)
		{
			rectTRN.left += (nWidth-55)/2;
			rectTRN.right -= (nWidth-55)/2;
		}
		else
		{
			rectTRN.left += 0;
			rectTRN.right -= 1;
		}

		nWidth = rectTRN.Width();
		
		dValue = (double)DrawHeight * (m_Max - value);
		rectTRN.top += (int)(dValue / ValueHeight);
		rectTRN.bottom = m_DrawRect.top;

//		if (m_Min == 0)
//			dValue = (double)DrawHeight * (m_Max - m_Min);
//		else
		dValue = (double)DrawHeight * m_Max;
		
		rectTRN.bottom += (int)(dValue / ValueHeight);
		rectTRN.NormalizeRect();

		if (rectTRN.Width() >= 2)
		{
			pt[0].x = rectTRN.left; pt[0].y = rectTRN.top;
			pt[1].x = rectTRN.right - 1; pt[1].y = rectTRN.top;
			pt[2].x = rectTRN.right - 1; pt[2].y = rectTRN.bottom;
			pt[3].x = rectTRN.left; pt[3].y = rectTRN.bottom;
			pt[4].x = rectTRN.left; pt[4].y = rectTRN.top;

			pDC->Polygon(pt, 5);
		}
		else
		{
			pDC->MoveTo(rectTRN.left, rectTRN.top);
			pDC->LineTo(rectTRN.left, rectTRN.bottom);
		}				
	}

	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);
}

bool CGrp_VBar::CalculateMinMax()
{
	CGrp_Base::CalculateMinMax();

	double	minmax[2]{};
	switch(m_gKind)
	{
	case IGK_2DVBAR:
	case IGK_3DVBAR:
	case IGK_XBASEVBAR:
		if ((m_Max >= 0 && m_Min >= 0) || (m_Max <= 0 && m_Min <= 0))
			m_bAllSameValue = true;
		m_Min = 0.0;
		break;
	case IGK_2DUDBAR:
		if (m_Max >= 0 && m_Min >= 0)
		{
			m_Min = 0;
		}
		else if (m_Max <= 0 && m_Min <= 0)
		{
			m_Max = 0;
		}
		break;
	case IGK_3DUDBAR:
		if (m_Max >= 0 && m_Min >= 0)
		{
			m_Min = 0;
		}
		else if (m_Max <= 0 && m_Min <= 0)
		{
			m_Max = 0;
		}
		
		if (m_Min != m_Max && m_Max != INT_MIN)
			m_Min -= (m_Max - m_Min ) / m_pGrpWnd->m_dispDay;
		break;
	case IGK_2D0UDBAR:
		minmax[0] = fabs(m_Min);
		minmax[1] = fabs(m_Max);

		m_Max = m_Min = (minmax[0] > minmax[1]) ? minmax[0] : minmax[1];
		m_Min *= -1.0;
		break;
	case IGK_3D0UDBAR:
		if (m_Min != m_Max && m_Max != INT_MIN)
		{
			const	double	gap = m_Max - m_Min;
			m_Max += gap / (m_pGrpWnd->m_dispDay*2);
			m_Min -= gap / (m_pGrpWnd->m_dispDay*2);
		}

		minmax[0] = fabs(m_Min);
		minmax[1] = fabs(m_Max);
		m_Max = m_Min = (minmax[0] > minmax[1]) ? minmax[0] : minmax[1];
		m_Min *= -1.0;
		break;
	default:
		break;
	}

	double gap = m_Max - m_Min;
	if (gap == 0)
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

void CGrp_VBar::DrawName(CDC *pDC)
{
	if (m_gKind != IGK_XBASEVBAR)
	{
		DrawTickIndex(pDC, m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->name, m_tRGB);
		return;
	}

	// X기준 대비 수직막대 예외처리(받은데이타중 tipName의 2,3번째 이름을 출력
	CString	sName = m_pGrpWnd->m_sExtraName;
	int	index = sName.Find("\t");
	if (index > 0)
	{
		CString	sName1 = sName.Left(index++);
		CString sName2 = sName.Mid(index);

		DrawTickIndex(pDC, sName1, m_clrUp);
		DrawTickIndex(pDC, sName2, m_clrDn);
	}
	else
	{
		return;
	}
}