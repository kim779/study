// Grp_Juga.cpp: implementation of the CGrp_Juga class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "C_Bong.h"
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

CGrp_Juga::CGrp_Juga(CWnd *pView, class CGrpWnd *pGrpWnd, char *pInfo)
: CGrp_Base(pView, pGrpWnd, pInfo)
{
	CalculateMinMax();

	m_clrUp = m_pApp->GetColor(pView, CLR_UP);
	m_clrDn = m_pApp->GetColor(pView, CLR_DOWN);
}

CGrp_Juga::~CGrp_Juga()
{

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
			if (m_Max == DBL_MIN || m_Min == DBL_MAX)
				return;
		}
	}

	if (m_Max == DBL_MIN || m_Min == DBL_MAX)
		return;

	if (m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkLEFT].bDraw)	DrawTickHorizon(pDC, m_Max, m_Min, ctkLEFT);
	if (m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkRIGHT].bDraw)	DrawTickHorizon(pDC, m_Max, m_Min, ctkRIGHT);
	DrawTickBottom(pDC);

	switch (m_gKind)
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

void CGrp_Juga::DrawBong(CDC *pDC)
{
	int	dispPos = m_pGrpWnd->m_dispPos;
	int	dispEnd = m_pGrpWnd->m_dispEnd;
	int	dispDay = m_pGrpWnd->m_dispDay;

	COLORREF upColor = m_clrUp;
	COLORREF dnColor = m_clrDn;

	CPen *upPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, upColor);
	CPen *dnPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, dnColor);
	CPen *sPen;
	 
	CBrush *upBrush =  m_pApp->GetBrush(m_pView, upColor);
	CBrush *dnBrush =  m_pApp->GetBrush(m_pView, dnColor);
	CBrush *sBrush;

	int DrawHeight = m_DrawRect.Height();
	int DrawWidth  = m_DrawRect.Width();

	double ValueHeight = (double)DrawHeight / (double)(m_Max - m_Min);
	if (ValueHeight == 0.0) 
		ValueHeight = 1.0;

	double fpr, hpr, lpr, epr;
	CPoint pointFPR, pointHPR, pointLPR, pointEPR;

	double	dValue;
	CPoint	pt[11];

	ZeroMemory(&m_MaxDayIndex, sz_cgIndex);
	ZeroMemory(&m_MinDayIndex, sz_cgIndex);
	m_dispMinVal = INT_MAX;
	m_dispMaxVal = INT_MIN;

	bool	bUp;
	struct _cgBong	*gBong, *rBong;
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

		int	gap = pointEPR.x - pointFPR.x;
		if (gap > 20)
		{
			pointFPR.x += (gap-20)/2;
			pointEPR.x -= (gap-20)/2;
		}

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

void CGrp_Juga::DrawBar(CDC *pDC)
{
	int	dispPos = m_pGrpWnd->m_dispPos;
	int	dispEnd = m_pGrpWnd->m_dispEnd;
	int	dispDay = m_pGrpWnd->m_dispDay;

	CPen	*cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, m_tRGB);
	CPen	*sPen = pDC->SelectObject(cPen);

	int DrawHeight = m_DrawRect.Height();
	int DrawWidth  = m_DrawRect.Width();

	double ValueHeight = m_Max - m_Min;
	if (ValueHeight == 0) 
		ValueHeight = 1;

	double hpr, lpr, epr;
	CPoint pointHPR, pointLPR, pointEPR;

	double	dValue;
	ZeroMemory(&m_MaxDayIndex, sz_cgIndex);
	ZeroMemory(&m_MinDayIndex, sz_cgIndex);
	m_dispMinVal = INT_MAX;
	m_dispMaxVal = INT_MIN;

	struct _cgBong	*gBong;
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

void CGrp_Juga::DrawLine(CDC *pDC)
{
	int	dispPos = m_pGrpWnd->m_dispPos;
	int	dispEnd = m_pGrpWnd->m_dispEnd;
	int	dispDay = m_pGrpWnd->m_dispDay;
	
	COLORREF color = m_tRGB;
	int lineWidth = 1;
	
	CPen	*cPen = m_pApp->GetPen(m_pView, PS_SOLID, lineWidth, color);
	CPen	*sPen = pDC->SelectObject(cPen);

	int DrawHeight = m_DrawRect.Height();
	int DrawWidth  = m_DrawRect.Width();

	double ValueHeight = m_Max - m_Min;
	if (!ValueHeight) ValueHeight = 1;

	int drawShift = 0;
	drawShift = int((DrawWidth/dispDay) / 2);	// Center

	double epr;
	
	CPoint pointEPR;
	CPoint pointFPR;
	CPoint pointSPR = CPoint(-1, -1);

	ZeroMemory(&m_MaxDayIndex, sz_cgIndex);
	ZeroMemory(&m_MinDayIndex, sz_cgIndex);
	m_dispMaxVal = INT_MIN;
	m_dispMinVal = INT_MAX;

	int	bInit = true;
	struct _cgBong	*gBong;
	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		gBong = (struct _cgBong *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(ii+dispPos);
		epr = gBong->epr;

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

		if (epr > m_dispMaxVal)
		{
			m_dispMaxVal = epr;
			m_MaxDayPos = pointEPR;
			CopyMemory(&m_MaxDayIndex, &gBong->index, sz_cgIndex);
		}
		if (epr < m_dispMinVal)
		{
			m_dispMinVal = epr;
			m_MinDayPos = pointEPR;
			CopyMemory(&m_MinDayIndex, &gBong->index, sz_cgIndex);
		}
	}

	pDC->SelectObject(sPen);

	DrawMinMaxDay(pDC);
}

void CGrp_Juga::DrawMinMaxDay(CDC* pDC)
{
	if (!m_MaxDayIndex.date.yy && !m_MaxDayIndex.time.hh)
		return;

	CString	valstr, dispStr;
	CRect	tkRect;
	CPen	*uPen, *dPen, *sPen;
	CFont	*sFont;
	CSize	size;

	COLORREF	upColor = RGB(255, 0, 0);
	COLORREF	dnColor = RGB(0, 0, 255);

	uPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, upColor);
	dPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, dnColor);

	sFont = pDC->SelectObject(m_pGrpWnd->m_pFont);
	size = pDC->GetTextExtent("틱의높이");
	sPen = pDC->SelectObject(uPen);
	int	sMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF sColor = pDC->SetTextColor(upColor);
	
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
	int	nWidth = rect.Width();
	int	nHeight = size.cy;

	m_MaxDayPos.y -= 2;
	m_MinDayPos.y += 2;
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
	pDC->SelectObject(sPen);
}

bool CGrp_Juga::CalculateMinMax()
{
	m_Max = DBL_MIN;
	m_Min = DBL_MAX;

	if (!m_pGrpWnd->m_pDataInfo[m_dKey] || 
		m_pGrpWnd->m_pDataInfo[m_dKey]->GetDataCount() <= 0)
		return false;

	int	dispPos = m_pGrpWnd->m_dispPos;
	int	dispEnd = m_pGrpWnd->m_dispEnd;
	int	dispDay = m_pGrpWnd->m_dispDay;

	if (m_gKind == CGK_LINE)
	{
		struct _cgBong	*gBong;
		for (int ii = 0; ii < dispEnd - dispPos; ii++)
		{
			gBong = (struct _cgBong *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(ii+dispPos);
			if (m_Max < gBong->epr)	m_Max = gBong->epr;
			if (m_Min > gBong->epr)	m_Min = gBong->epr;
		}
	}
	else
	{
		struct _cgBong	*gBong;
		for (int ii = 0; ii < dispEnd - dispPos; ii++)
		{
			gBong = (struct _cgBong *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(ii+dispPos);
			if (m_Max < gBong->hpr)	m_Max = gBong->hpr;
			if (m_Min > gBong->lpr)	m_Min = gBong->lpr;
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
	else
	{
		m_Min -= gap / 10;
	}

	return true;
}

bool CGrp_Juga::IsChangeMinMax(bool bShift)
{
	double	m_sMax = m_Max;
	double	m_sMin = m_Min;

	if (!CalculateMinMax())
		return false;

	if (m_sMax != m_Max || m_sMin != m_Min)
		return true;

	return false;
}

CString CGrp_Juga::GetDisplayPosData(CPoint pt)
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

	CString	stmp;
	struct _cgBong	*gBong = 
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
		struct _cgBong	*gBong = (struct _cgBong *)m_pGrpWnd->m_pDataInfo[m_dKey]->GetGraphData(idx);

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