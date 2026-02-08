// Grp_Vol.cpp: implementation of the CGrp_Vol class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "C_VolLine1.h"
#include "Grp_Vol.h"
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

CGrp_Vol::CGrp_Vol(CWnd *pView, class CGrpWnd *pGrpWnd, char *pInfo)
: CGrp_Base(pView, pGrpWnd, pInfo)
{
	CalculateMinMax();
}

CGrp_Vol::~CGrp_Vol()
{

}

void CGrp_Vol::DrawGraph(CDC *pDC)
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

	DrawTickBottom(pDC);
	DrawVolume(pDC);
	DrawLegend(pDC);
}

void CGrp_Vol::DrawVolume(CDC *pDC)
{
	const int	dispPos = m_pGrpWnd->m_dispPos;
	const int	dispEnd = m_pGrpWnd->m_dispEnd;
	const int	dispDay = m_pGrpWnd->m_dispDay;
	if (dispEnd - dispPos <= 0)
		return;

	const COLORREF VolColor = RGB(84,164,5);
	CPen	*cPen;
	cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, VolColor);
	CPen	*sPen = pDC->SelectObject(cPen);

	CBrush	*cBrush;
	cBrush = m_pApp->GetBrush(m_pView, VolColor);
	CBrush	*sBrush = pDC->SelectObject(cBrush);

	const int DrawHeight = m_DrawRect.Height();
	const int DrawWidth  = m_DrawRect.Width();

	double	trn{};
	double	dValue{};
	int	nWidth{};
	CRect	rectTRN;

	struct _cgVal2* gVal2{};
	double	dAvg = 0;
	int	count = 0;
	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		gVal2 = (struct _cgVal2 *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(ii+dispPos);
		if(gVal2->val[1] <= 0)	continue;
		
		dAvg += gVal2->val[1];
		count++;
	}
	if (count > 0)	dAvg /= count;

	const double	dMax = m_Max, dMin = m_Min;
//	if (dMax > dAvg*2)	dMax = dAvg*2;

	if (m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkLEFT].bDraw)	DrawTickHorizon(pDC, dMax, dMin, ctkLEFT);
	if (m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkRIGHT].bDraw)DrawTickHorizon(pDC, dMax, dMin, ctkRIGHT);

	double ValueHeight = dMax - dMin;
	if (ValueHeight == 0) 
		ValueHeight = 1;

	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		gVal2 = (struct _cgVal2 *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(ii+dispPos);

		if ((trn = gVal2->val[1]) <= 0)	continue;

		rectTRN.left = m_DrawRect.left;
		rectTRN.left += int((DrawWidth * ii) / dispDay+0.5);
		rectTRN.right = m_DrawRect.left;
		rectTRN.right += int((DrawWidth * (ii +1)) / dispDay+0.5);
		rectTRN.right -= 1;

		rectTRN.top = m_DrawRect.top;
		dValue = double(DrawHeight) * double(dMax - trn);
		rectTRN.top += int(dValue / ValueHeight);
		rectTRN.bottom = m_DrawRect.bottom;// - gapBOTTOM;

		if (trn > dMax)
		{
			if (rectTRN.top < m_pGrpWnd->m_GrpRect.top)	rectTRN.top = m_pGrpWnd->m_GrpRect.top;
			if (rectTRN.top > rectTRN.bottom)	rectTRN.top = m_pGrpWnd->m_GrpRect.top;
		}

		nWidth = rectTRN.Width();
		if (nWidth > 40)
		{
			rectTRN.left += (nWidth-40)/2;
			rectTRN.right -= (nWidth-40)/2;
		}
		else if (nWidth > 2)
		{
			CPoint	pt[5];

			pt[0].x = rectTRN.left; pt[0].y = rectTRN.top;
			pt[1].x = rectTRN.right - 1; pt[1].y = rectTRN.top;
			pt[2].x = rectTRN.right - 1; pt[2].y = rectTRN.bottom - 1;
			pt[3].x = rectTRN.left; pt[3].y = rectTRN.bottom - 1;
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

bool CGrp_Vol::CalculateMinMax()
{
	m_Max = INT_MIN;
	m_Min = INT_MAX;

	if (!m_pGrpWnd->m_pDataInfo[m_dKey] || 
		m_pGrpWnd->m_pDataInfo[m_dKey]->GetDataCount() <= 0)
		return false;

	const int	dispPos = m_pGrpWnd->m_dispPos;
	const int	dispEnd = m_pGrpWnd->m_dispEnd;
	const int	dispDay = m_pGrpWnd->m_dispDay;

	struct _cgVal2* gVal2{};
	double	dMax = INT_MIN;
	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		gVal2 = (struct _cgVal2 *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(ii+dispPos);
		if (dMax < gVal2->val[1])	dMax = gVal2->val[1];
	}

	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		gVal2 = (struct _cgVal2 *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(ii+dispPos);

		if (m_Max < gVal2->val[1] && gVal2->val[1] != dMax)	m_Max = gVal2->val[1];
		if (m_Min > gVal2->val[1])	m_Min = gVal2->val[1];
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

bool CGrp_Vol::IsChangeMinMax(bool bShift)
{
	const double	m_sMax = m_Max;
	const double	m_sMin = m_Min;

	if (!CalculateMinMax())
		return false;

	if (m_sMax != m_Max || m_sMin != m_Min)
		return true;

	return false;
}

CString CGrp_Vol::GetDisplayPosData(CPoint pt)
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
	const struct _cgVal2	*gVal2 =
		 (struct _cgVal2*)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(dataPos);

	FormatVariableComma(stmp, gVal2->val[1], 0);
	stmp2.Format("%s\t%s", m_pGrpWnd->m_title[1], stmp);
	dispStr = dispStr + stmp2;
	
	return dispStr;
}

void CGrp_Vol::DrawLegend(CDC* pDC)
{
	if (m_pGrpWnd->m_pFont)
	{
		CFont	*oldfont = pDC->SelectObject(m_pGrpWnd->m_pFont);
		const int	bkMode = pDC->SetBkMode(TRANSPARENT);

		pDC->TextOut(m_RgnRect.left + 5, m_RgnRect.top + 3, m_pGrpWnd->m_title[1]);

		pDC->SetBkMode(bkMode);
		pDC->SelectObject(oldfont);
	}
}

CString CGrp_Vol::GetExcelData(int idx)
{
	CString	dispStr = __T("");
	if (!m_pGrpWnd->m_pDataInfo[m_dKey] || 
		m_pGrpWnd->m_pDataInfo[m_dKey]->GetDataCount() <= 0)
		return dispStr;

	if (idx < 0)
	{
		dispStr = m_pGrpWnd->m_title[1];
	}
	else
	{
		const struct _cgVal2* gVal2 = (struct _cgVal2 *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(idx);
		FormatVariableComma(dispStr, gVal2->val[1], 0);
	}

	return dispStr;
}