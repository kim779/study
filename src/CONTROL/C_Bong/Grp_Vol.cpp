// Grp_Vol.cpp: implementation of the CGrp_Vol class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "C_Bong.h"
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

	if (pGrpWnd->m_bSameRegion && pGrpWnd->m_VolColor > 0)
	{
		m_clrUp = m_clrDn = m_pApp->GetColor(pView, pGrpWnd->m_VolColor);
	}

	m_brUp   = m_pApp->GetBrush(pView, m_clrUp);
	m_brDown = m_pApp->GetBrush(pView, m_clrDn);

	m_penUp  = m_pApp->GetPen(pView, PS_SOLID, 1, m_clrUp);
	m_penDown= m_pApp->GetPen(pView, PS_SOLID, 1, m_clrDn);
}

CGrp_Vol::~CGrp_Vol()
{

}

void CGrp_Vol::DrawGraph(CDC *pDC)
{
	if (!m_pGrpWnd->m_bSameRegion)
	{
		struct _MinMaxChk *pMinMaxChk;
		if (pMinMaxChk = m_pGrpWnd->GetMinMaxChk(m_rKey))
		{
			if (pMinMaxChk->coMinMax)
			{
				m_Max = pMinMaxChk->Max;
				m_Min = pMinMaxChk->Min;
				if (m_Max == DBL_MIN || m_Min == DBL_MAX)
					return;
			}
		}
	}

	if (m_Max == DBL_MIN || m_Min == DBL_MAX)
		return;

	if (!m_pGrpWnd->m_bSameRegion)
	{
		if (m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkLEFT].bDraw)	DrawTickHorizon(pDC, m_Max, m_Min, ctkLEFT);
		if (m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkRIGHT].bDraw)	DrawTickHorizon(pDC, m_Max, m_Min, ctkRIGHT);
		DrawTickBottom(pDC);
	}

	DrawVolume(pDC);
}

void CGrp_Vol::DrawVolume(CDC *pDC)
{
	int	dispPos = m_pGrpWnd->m_dispPos;
	int	dispEnd = m_pGrpWnd->m_dispEnd;
	int	dispDay = m_pGrpWnd->m_dispDay;

	CPen	*sPen = pDC->SelectObject(m_penUp);
	CBrush	*sBrush = pDC->SelectObject(m_brUp);

	int DrawHeight = m_DrawRect.Height();
	int DrawWidth  = m_DrawRect.Width();

	double ValueHeight = m_Max - m_Min;
	if (ValueHeight == 0) 
		ValueHeight = 1;

	double	trn;
	double	dValue, dPrvValue = 0;
	CRect	rectTRN;

	struct _cgBong	*gBong;
	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		gBong = (struct _cgBong *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(ii+dispPos);

		if ((trn = gBong->trn) <= 0)
			continue;
	
		rectTRN.left = m_DrawRect.left;
		rectTRN.left += int((DrawWidth * ii) / dispDay+0.5);
		rectTRN.right = m_DrawRect.left;
		rectTRN.right += int((DrawWidth * (ii +1)) / dispDay+0.5);
		rectTRN.right -= 1;

		int	gap = rectTRN.right - rectTRN.left;
		if (gap > 20)
		{
			rectTRN.left += (gap-20)/2;
			rectTRN.right -= (gap-20)/2;
		}

		rectTRN.top = m_DrawRect.top;
		dValue = double(DrawHeight) * double(m_Max - trn);
		if (dValue < dPrvValue)
		{
			pDC->SelectObject(m_brUp);
			pDC->SelectObject(m_penUp);
		}
		else
		{
			pDC->SelectObject(m_brDown);
			pDC->SelectObject(m_penDown);
		}
		rectTRN.top += int(dValue / ValueHeight);
		rectTRN.bottom = m_DrawRect.bottom;// - gapBOTTOM;

		if (rectTRN.Width() > 2)
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

		dPrvValue = dValue;
	}

	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);
}

bool CGrp_Vol::CalculateMinMax()
{
	m_Max = DBL_MIN;
	m_Min = DBL_MAX;

	if (!m_pGrpWnd->m_pDataInfo[m_dKey] || 
		m_pGrpWnd->m_pDataInfo[m_dKey]->GetDataCount() <= 0)
		return false;

	int	dispPos = m_pGrpWnd->m_dispPos;
	int	dispEnd = m_pGrpWnd->m_dispEnd;
	int	dispDay = m_pGrpWnd->m_dispDay;

	struct _cgBong	*gBong;
	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		gBong = (struct _cgBong *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(ii+dispPos);
		if (m_Max < gBong->trn)	m_Max = gBong->trn;
		if (m_Min > gBong->trn)	m_Min = gBong->trn;
	}

	return true;
}

bool CGrp_Vol::IsChangeMinMax(bool bShift)
{
	double	m_sMax = m_Max;
	double	m_sMin = m_Min;

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

	int	dispPos = m_pGrpWnd->m_dispPos;
	int	dispEnd = m_pGrpWnd->m_dispEnd;
	int	dispDay = m_pGrpWnd->m_dispDay;

	int	xPosition = pt.x - m_DrawRect.left;
	double	szOneDay = double(m_DrawRect.Width()) / double(dispDay);
	int	dataPos = int(double(xPosition) / szOneDay);
	dataPos += dispPos;

	CString	stmp, stmp2, sSign, sSign2;
	struct _cgBong	*gBong = 
		 (struct _cgBong *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(dataPos);
	if (m_dIndex == CDI_MIN || m_dIndex == CDI_TICK)
	{
		FormatVariableComma(stmp, gBong->trn, 0);
		stmp2.Format("체결량\t%s", stmp);
		dispStr = dispStr + stmp2;
	}
	else
	{
		FormatVariableComma(stmp, gBong->trn, 0);
		stmp2.Format("거래량\t%s", stmp);
		dispStr = dispStr + stmp2;
	}
	
	return dispStr;
}

CString CGrp_Vol::GetExcelData(int idx)
{
	CString	dispStr = __T("");
	if (!m_pGrpWnd->m_pDataInfo[m_dKey] || 
		m_pGrpWnd->m_pDataInfo[m_dKey]->GetDataCount() <= 0)
		return dispStr;

	if (idx < 0)
	{
		dispStr = "거래량";
	}
	else
	{
		struct _cgBong	*gBong = (struct _cgBong *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(idx);
		FormatVariableComma(dispStr, gBong->trn, 0);
	}

	return dispStr;
}