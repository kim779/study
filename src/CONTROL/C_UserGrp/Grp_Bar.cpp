#include "stdafx.h"
#include "C_UserGrp.h"
#include "Grp_Bar.h"

#include "GrpWnd.h"
#include "Grp_Data.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CGrp_Bar::CGrp_Bar(CWnd *pView, class CGrpWnd *pGrpWnd, char *pInfo)
: CGrp_Base(pView, pGrpWnd, pInfo)
{
	CalculateMinMax();

	m_pColor[0] = m_clrUp;		// 상승
	m_pColor[1] = m_clrDn;		// 하락
	m_pColor[2] = RGB(30,150,0);	// 보합

	CString	sName = m_pGrpWnd->m_sExtraName;
	int	index = -1;
	for (int ii = 0; ii < 3; ii++)
	{
		index = sName.Find("\t");
		if (index >= 0)
		{
			m_pName[ii] = sName.Left(index);
			sName = sName.Mid(index+1);
		}
		else if (!sName.IsEmpty())
		{
			m_pName[ii] = sName;
			sName.Empty();
		}
	}
}

CGrp_Bar::~CGrp_Bar()
{
	m_arNInfo.clear();
}

void CGrp_Bar::DrawGraph(CDC *pDC)
{
	switch (m_gKind)
	{
	case IGK_2D1NBAR:
		Draw2D1NBar(pDC);
		if (m_TickInfo & CDT_LEFT)	DrawTickHorizon2DNBar(pDC, m_Max, m_Min, ctkLEFT);
		if (m_TickInfo & CDT_RIGHT)	DrawTickHorizon2DNBar(pDC, m_Max, m_Min, ctkRIGHT);
		Draw2D1NBar(pDC);
		break;
	case IGK_2D3NBAR:
		Draw2D3NBar(pDC);
		if (m_TickInfo & CDT_LEFT)	DrawTickHorizon2DNBar(pDC, m_Max, m_Min, ctkLEFT);
		if (m_TickInfo & CDT_RIGHT)	DrawTickHorizon2DNBar(pDC, m_Max, m_Min, ctkRIGHT);
		Draw2D3NBar(pDC);
		break;
	default:
		break;
	}
}

void CGrp_Bar::Draw2D1NBar(CDC *pDC)
{
	const size_t count = m_arNInfo.size();
	if (count < 1)
		return;

	CRect	DrawRect = m_DrawRect;

	double	dPixel = (double)DrawRect.Height() / (double)count;
	const	int	iPixel = (int)dPixel;
	if (iPixel > 1)
	{
		DrawRect.DeflateRect(0, (DrawRect.Height()-iPixel*count)/2);
		dPixel = iPixel;
	}

	const	int	iDrawWidth	= DrawRect.Width();
	CPen	*pPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, m_tRGB);
	CBrush	*pBrush = m_pApp->GetBrush(m_pView, m_tRGB);
	CPen	*sPen = pDC->SelectObject(pPen);
	CBrush	*sBrush = pDC->SelectObject(pBrush);

	int	rGap = 0;
	CRect	rect;
//	struct	_3NInfo	*pInfo = nullptr;

	for (size_t ii = 0; ii < count; ii++)
	{
		const auto& pInfo = m_arNInfo.at(ii);

		rect.top = DrawRect.top + (int)(dPixel * ii);
		rect.bottom = DrawRect.top + (int)(dPixel * (ii+1));
		rGap = rect.bottom - rect.top;
		pInfo->top = rect.top;
		pInfo->bottom = rect.bottom;
		if (rGap >= 3)
		{
			rect.top += 1;	rect.bottom -= 1;
		}
		else if (rGap > 1)
		{
			rect.bottom -= 1;
		}

		if (rGap > 30)
			rect.DeflateRect(0, (rGap-30)/2);

		rect.left = DrawRect.left;
		rect.right = rect.left + (int)((pInfo->val[0]*iDrawWidth)/m_Max);
		pDC->FillSolidRect(rect, m_tRGB);
	}

	pDC->SelectObject(sBrush);
	pDC->SelectObject(sPen);
}

void CGrp_Bar::Draw2D3NBar(CDC *pDC)
{
	const size_t count = m_arNInfo.size();
	if (count < 1)
		return;

	CRect	DrawRect = m_DrawRect;
	double	dPixel = (double)DrawRect.Height() / (double)count;
	const	int	iPixel = (int)dPixel;

	if (iPixel > 1)
	{
		DrawRect.DeflateRect(0, (DrawRect.Height()-iPixel*count)/2);
		dPixel = iPixel;
	}

	const	int	iDrawHeight	= DrawRect.Height();
	const	int	iDrawWidth	= DrawRect.Width();

	CPen	*pPen[3]{}, *sPen = nullptr;
	CBrush	*pBrush[3]{}, *sBrush = nullptr;
	for (int ii = 0 ; ii < 3 ; ii++ )
	{
		pPen[ii] = m_pApp->GetPen(m_pView, PS_SOLID, 1, m_pColor[ii]);
		pBrush[ii] = m_pApp->GetBrush(m_pView, m_pColor[ii]);
	}
	sPen = pDC->SelectObject(pPen[0]);
	sBrush = pDC->SelectObject(pBrush[0]);

	CRect	rect;
	int	rGap = 0;
//	struct	_3NInfo	*pInfo = nullptr;

	for (size_t ii = 0; ii < count; ii++)
	{
		const auto& pInfo = m_arNInfo.at(ii);

		rect.top = DrawRect.top + (int)(dPixel * ii);
		rect.bottom = DrawRect.top + (int)(dPixel * (ii+1));
		rGap = rect.bottom - rect.top;
		pInfo->top = rect.top;
		pInfo->bottom = rect.bottom;
		if (rGap >= 3)
		{
			rect.top += 1;	rect.bottom -= 1;
		}
		else if (rGap > 1)
		{
			rect.bottom -= 1;
		}

		if (rGap > 30)
			rect.DeflateRect(0, (rGap-30)/2);

		rect.left = DrawRect.left;
		rect.right = rect.left + (int)((pInfo->val[0]*iDrawWidth)/m_Max);
		pDC->FillSolidRect(rect, m_pColor[0]);

		rect.left = rect.right;
		rect.right = rect.left + (int)((pInfo->val[1]*iDrawWidth)/m_Max);
		pDC->FillSolidRect(rect, m_pColor[1]);

		rect.left = rect.right;
		rect.right = rect.left + (int)((pInfo->val[2]*iDrawWidth)/m_Max);
		pDC->FillSolidRect(rect, m_pColor[2]);
	}

	pDC->SelectObject(sBrush);
	pDC->SelectObject(sPen);
}

void CGrp_Bar::DrawTickHorizon2DNBar(CDC* pDC, double Max, double Min, int tickinfo)
{
	CRect	tickRC = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[tickinfo].tkRect;
	tickRC.top = m_DrawRect.top;
	tickRC.bottom = m_DrawRect.bottom;

	const	int		sMode = pDC->SetBkMode(TRANSPARENT);
	const	COLORREF	sColor = pDC->SetTextColor(m_pGrpWnd->m_TkTextColor);
	const	int		style = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[tickinfo].lstyle;
	const	int		width = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[tickinfo].lwidth;
	const	COLORREF	color = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[tickinfo].lcolor;

	CRect	textRC;
	CFont	*sFont = pDC->SelectObject(m_pGrpWnd->m_pFont);
	CPen	*cPen = m_pApp->GetPen(m_pView, style, width, color);
	CPen	*sPen = pDC->SelectObject(cPen);
	const size_t count = m_arNInfo.size();
	CString	dispStr;
	CPoint	pointS, pointE;
	CRect	tkSaveRC = CRect(10000,10000,10000,10000);
	struct	_3NInfo	*pInfo = nullptr;
	CSize	size;
	int	nMaxLen = 0;

	for (size_t ii = 0 ; ii < count; ii++ )
	{
		const auto& pInfo = m_arNInfo.at(ii);
		FormatVariableComma(dispStr, pInfo->dUpTick, m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->digit);
		size = pDC->GetOutputTextExtent(dispStr);

		if (nMaxLen < size.cx)
			nMaxLen = size.cx;
	}

	size = pDC->GetOutputTextExtent(" ");
	const	int	nGap = size.cx;

	CRect	rcSave;
	rcSave.SetRect(0,0,0,0);	
	for (size_t ii = 0; ii < count; ii++)		// 가격별가로 (DOT)
	{
		const auto& pInfo = m_arNInfo.at(ii);
		pointS.x = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkLEFT].tkRect.right+1;
		pointS.y = pInfo->top;

		if (!tickRC.PtInRect(pointS))
		{
			if (tickRC.bottom < pointS.y)
				continue;
		}

		pointE.x = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkRIGHT].tkRect.left;
		pointE.y = pointS.y;
		if (pointS.y >= m_DrawRect.top && pointS.y <= m_DrawRect.bottom && m_pGrpWnd->m_bHDraw)
		{
			pDC->MoveTo(pointS);
			pDC->LineTo(pointE);
		}

		FormatVariableComma(dispStr, pInfo->dUpTick, m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->digit);
		size = pDC->GetOutputTextExtent(dispStr);
		textRC.SetRect(tickRC.left, pointS.y - size.cy/2, tickRC.right, pointS.y + size.cy/2);

		if (textRC.bottom > tkSaveRC.top)
		{
			textRC.OffsetRect(0, tkSaveRC.top - textRC.bottom);
			tkSaveRC.top += size.cy + 1;
		}
		
		if (tickinfo == ctkLEFT)
		{
			textRC.right -= nGap;
		}
		else
		{
			textRC.left += nGap;
			textRC.right = textRC.left + nMaxLen;
		}
		if (rcSave.bottom + size.cx/4 > textRC.top)
			continue;

		pDC->DrawText(dispStr, &textRC, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
		rcSave = textRC;
	}

	if (count > 0)
	{
		pointS.x = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkLEFT].tkRect.right+1;
		if (pInfo)
			pointS.y = pInfo->bottom;

		if (!tickRC.PtInRect(pointS))
		{
			if (tickRC.bottom < pointS.y)
			{
				pDC->SelectObject(sFont);
				pDC->SelectObject(sPen);
				pDC->SetTextColor(sColor);
				pDC->SetBkMode(sMode);
				return;
			}				
		}

		pointE.x = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkRIGHT].tkRect.left;
		pointE.y = pointS.y;
		if (pointS.y >= m_DrawRect.top && pointS.y <= m_DrawRect.bottom && m_pGrpWnd->m_bHDraw)
		{
			pDC->MoveTo(pointS);
			pDC->LineTo(pointE);
		}

		if (pInfo)
			FormatVariableComma(dispStr, pInfo->dDnTick, m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->digit);
		size = pDC->GetOutputTextExtent(dispStr);
		textRC.SetRect(tickRC.left, pointS.y - size.cy/2, tickRC.right, pointS.y + size.cy/2);

		if (textRC.bottom > tkSaveRC.top)
		{
			textRC.OffsetRect(0, tkSaveRC.top - textRC.bottom);
			tkSaveRC.top += size.cy + 1;
		}
		
		if (tickinfo == ctkLEFT)
		{
			textRC.right -= nGap;
		}
		else
		{
			textRC.left += nGap;
			textRC.right = textRC.left + nMaxLen;
		}
		if (rcSave.bottom + size.cx/4 > textRC.top)
		{
			pDC->SelectObject(sFont);
			pDC->SelectObject(sPen);
			pDC->SetTextColor(sColor);
			pDC->SetBkMode(sMode);
			return;
		}

		pDC->DrawText(dispStr, &textRC, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
		rcSave = textRC;
	}

	pDC->SelectObject(sFont);
	pDC->SelectObject(sPen);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
}

void CGrp_Bar::DrawTickBottom(CDC* pDC) 
{
	int	dispCount = 0;
	double	dispValue[64]{};
	const	CRect	tickRC = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].tkRect;
	const	CRect	orgTickRC = tickRC;
	double	ValueHeight = m_Max - m_Min;
	double	div = 0;

	if (ValueHeight <= 0) 	ValueHeight = 1;
	
	if (ValueHeight < 5)			div = 1;
	else if (ValueHeight < 50)		div = 5;
	else if (ValueHeight < 200)		div = 10;
	else if (ValueHeight < 500)		div = 50;
	else if (ValueHeight < 2000)		div = 100;
	else if (ValueHeight < 5000)		div = 500;
	else if (ValueHeight < 20000)		div = 1000;
	else if (ValueHeight < 50000)		div = 5000;
	else if (ValueHeight < 200000)		div = 10000;
	else if (ValueHeight < 500000)		div = 50000;
	else if (ValueHeight < 2000000)		div = 100000;
	else if (ValueHeight < 5000000)		div = 500000;
	else if (ValueHeight < 20000000)	div = 1000000;
	else if (ValueHeight < 50000000)	div = 5000000;
	else if (ValueHeight < 200000000)	div = 10000000;
	else if (ValueHeight < 500000000)	div = 50000000;
	else if (ValueHeight < 2000000000)	div = 100000000;
	else					div = 500000000;

	double	drawMinValue= 0.0;

	if (!calculateDisplay(div, m_Max, m_Min, drawMinValue, 10, dispValue, dispCount))
		return;

	const	int		sMode = pDC->SetBkMode(TRANSPARENT);
	const	COLORREF	sColor = pDC->SetTextColor(m_pGrpWnd->m_TkTextColor);
	const	int		style = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].lstyle;
	const	int		width = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].lwidth;
	const	COLORREF	color = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].lcolor;

	CFont	*sFont = pDC->SelectObject(m_pGrpWnd->m_pFont);
	CPen	*cPen = m_pApp->GetPen(m_pView, style, width, color);
	CPen	*sPen = pDC->SelectObject(cPen);

	CRect	textRC;
	CString	dispStr;
	CPoint	pointS, pointE;
	CRect	tkSaveRC;
	CSize	size;

	tkSaveRC = tickRC;
	for (int ii = 0, jj = 0 ; ii < dispCount ; ii++ )		// 가격별가로 (DOT)
	{
		if (dispValue[ii] < drawMinValue)
			continue;
		
		pointS.x = GetXPos(dispValue[ii]);
		pointS.y = m_pGrpWnd->m_GrpRect.top;

		if (!tickRC.PtInRect(pointS))
		{
			if (tickRC.bottom < pointS.y)
				continue;
		}

		pointE.x = pointS.x;
		pointE.y = m_pGrpWnd->m_GrpRect.bottom - 1;

		if (m_pGrpWnd->m_bVDraw)
		{
			pDC->SelectObject(cPen);
			pDC->MoveTo(pointE);
			pDC->LineTo(pointS);
		}

		
		pointS.y = tickRC.top;

		if (!tickRC.PtInRect(pointS))
		{
			if (tickRC.bottom < pointS.y)
				continue;
		}

		FormatVariableComma(dispStr, dispValue[ii], 0);

		size = pDC->GetOutputTextExtent(dispStr);
		textRC = DisplayRect(pDC, 0, dispStr, pointS);
		textRC.top += 4;

		textRC.left -= size.cx / 2;
		textRC.right = textRC.left + size.cx;		

		if (jj != 0)
		{
			if (textRC.left - tkSaveRC.right <= 5)	
				continue;
		}

		if (textRC.right > tickRC.right)
		{
			textRC.OffsetRect(tickRC.right - textRC.right, 0);

			if (textRC.left < tkSaveRC.right)
				continue;
		}
		
		pDC->DrawText(dispStr, &textRC, DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_NOCLIP);
		tkSaveRC = textRC;
		jj++;
	}

	pDC->SelectObject(sFont);
	pDC->SelectObject(sPen);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
}

void CGrp_Bar::DrawName(CDC *pDC) 
{
	if (m_gKind != IGK_2D3NBAR)
	{
		DrawTickIndex(pDC, m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->name, m_tRGB);
		return;
	}

	for (int ii = 0; ii < 3; ii++)
	{
		if (!m_pName[ii].IsEmpty())
			DrawTickIndex(pDC, m_pName[ii], m_pColor[ii]);
	}
}

bool CGrp_Bar::CalculateMinMax()
{
	switch (m_gKind)
	{
	case IGK_2D1NBAR:
		return Calculate2D1N();
	case IGK_2D3NBAR:
		return Calculate2D3N();
	}

	return true;
}

bool CGrp_Bar::Calculate2D1N()
{
	if (m_arNInfo.size() > 0)
		return true;

	m_Max = INT_MIN;
	m_Min = INT_MAX;

	if (!m_pGrpWnd->m_pDataInfo || m_pGrpWnd->m_pDataInfo->GetDataCount() <= 0)
		return false;

	for (int ii = 0 ; ii < m_pGrpWnd->m_totalDay ; ii++ )
	{
		const auto gVal = m_pGrpWnd->m_pDataInfo->GetGraphData(ii);
		if (m_Max < gVal->vData[m_dKey])
			m_Max = gVal->vData[m_dKey];
	}

	if (m_Max > 0)
		m_Max += m_Max/20;

	int	index= 0;
	CString		str, sVal[2];
	for (int ii = m_pGrpWnd->m_totalDay - 1 ; ii >= 0 ; ii-- )
	{
		const auto gVal = m_pGrpWnd->m_pDataInfo->GetGraphData(ii);
		const auto& pInfo = m_arNInfo.emplace_back(std::make_unique<_3NInfo>());
		pInfo->val[0] = gVal->vData[m_dKey];

		str = gVal->index.name.str;
		index = str.Find("\t");
		if (index > 0)
		{
			sVal[0] = str.Left(index++);
			sVal[1] = str.Mid(index);
		}
		else
		{
			sVal[0] = str;
			sVal[1] = "";
		}

		pInfo->dDnTick = atof(sVal[0]) * m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->power;
		pInfo->dUpTick = atof(sVal[1]) * m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->power;
		
	}

	m_Min = 0;

	return true;
}

bool CGrp_Bar::Calculate2D3N()
{
	if (m_arNInfo.size() > 0)
		return true;

	m_Max = INT_MIN;
	m_Min = INT_MAX;

	if (!m_pGrpWnd->m_pDataInfo || m_pGrpWnd->m_pDataInfo->GetDataCount() <= 0)
		return false;

	for (int ii = 0 ; ii < m_pGrpWnd->m_totalDay ; ii++ )
	{
		const auto gVal = m_pGrpWnd->m_pDataInfo->GetGraphData(ii);
		if (m_Max < gVal->vData[m_dKey] + gVal->vData[m_dKey+1] + gVal->vData[m_dKey+2])
			m_Max = gVal->vData[m_dKey] + gVal->vData[m_dKey+1] + gVal->vData[m_dKey+2];
	}

	if (m_Max > 0)
		m_Max += m_Max/20;

	int	index = 0;
	CString	str, sVal[2];
	struct	_3NInfo	*pInfo = nullptr;

	for (int ii = m_pGrpWnd->m_totalDay - 1 ; ii >= 0 ; ii-- )
	{
		const auto gVal = m_pGrpWnd->m_pDataInfo->GetGraphData(ii);
		const auto& pInfo = m_arNInfo.emplace_back(std::make_unique<_3NInfo>());
		pInfo->val[0] = gVal->vData[m_dKey];
		pInfo->val[1] = gVal->vData[m_dKey+1];
		pInfo->val[2] = gVal->vData[m_dKey+2];

		str = gVal->index.name.str;
		index = str.Find("\t");
		if (index > 0)
		{
			sVal[0] = str.Left(index++);
			sVal[1] = str.Mid(index);
		}
		else
		{
			sVal[0] = str;
			sVal[1] = "";
		}

		pInfo->dDnTick = atof(sVal[0]) * m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->power;
		pInfo->dUpTick = atof(sVal[1]) * m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->power;
	}

	m_Min = 0;

	return true;
}

void CGrp_Bar::GetMinMaxValue(double& dMin, double& dMax)
{
	if (m_arNInfo.size() > 0)
	{
		dMax = m_arNInfo.at(0)->dUpTick;
		dMin = (*m_arNInfo.rbegin())->dDnTick;
		return;
	}

	dMin = m_Min;
	dMax = m_Max;
}

int CGrp_Bar::GetXPos(double val)
{
	const	double	fMinMax = m_Max - m_Min;
	const	double	fHeight = m_DrawRect.Width();
	const	double	fGab = val - m_Min;
	double	yPos = 0;

	if (fMinMax <= 0.0)
		return m_DrawRect.left;

	yPos = fHeight * fGab / fMinMax;
	return (int)((double)m_DrawRect.left + yPos);
}

CString CGrp_Bar::GetDisplayPosHead(CPoint pt)
{
	switch (m_gKind)
	{
	case IGK_2D1NBAR:
	case IGK_2D3NBAR:
		break;
	default:
		return CGrp_Base::GetDisplayPosHead(pt);
	}

	CString	dispStr = _T("");
	if (m_DrawRect.left > pt.x || m_DrawRect.right <= pt.x || m_arNInfo.size() < 1)
		return dispStr;

	for (size_t ii = 0; ii < m_arNInfo.size(); ii++)
	{
		const auto& pInfo = m_arNInfo.at(ii);
		if (pInfo->top <= pt.y && pt.y <= pInfo->bottom)
		{
			CString	str[2];
			FormatVariableComma(str[0], pInfo->dDnTick, m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->digit);
			FormatVariableComma(str[1], pInfo->dUpTick, m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->digit);
			dispStr.Format("%s\t%s-%s", m_pGrpWnd->m_sTipName, str[0], str[1]);
			break;
		}
	}

	return dispStr;
}

CString CGrp_Bar::GetDisplayPosData(CPoint pt)
{
	switch (m_gKind)
	{
	case IGK_2D1NBAR:
	case IGK_2D3NBAR:
		break;
	default:
		return CGrp_Base::GetDisplayPosData(pt);
	}

	CString	dispStr = _T("");
	if (m_DrawRect.left > pt.x || m_DrawRect.right <= pt.x || m_arNInfo.size() < 1)
		return dispStr;

	if (m_gKind == IGK_2D1NBAR)
	{
		for (size_t ii = 0; ii < m_arNInfo.size(); ii++)
		{
			const auto& pInfo = m_arNInfo.at(ii);
			if (pInfo->top <= pt.y && pt.y <= pInfo->bottom)
			{
				CString	str[3];
				FormatVariableComma(str[0], pInfo->val[0], m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->digit);

				dispStr.Format("%s\t%s", 
					m_pName[0], str[0]);
				break;
			}
		}
	}
	else if (m_gKind == IGK_2D3NBAR)
	{
		for (size_t ii = 0; ii < m_arNInfo.size(); ii++)
		{
			const auto& pInfo = m_arNInfo.at(ii);
			if (pInfo->top <= pt.y && pt.y <= pInfo->bottom)
			{
				CString	str[3];
				FormatVariableComma(str[0], pInfo->val[0], m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->digit);
				FormatVariableComma(str[1], pInfo->val[1], m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->digit);
				FormatVariableComma(str[2], pInfo->val[2], m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->digit);

				dispStr.Format("%s\t%s\n%s\t%s\n%s\t%s", 
					m_pName[0], str[0], m_pName[1], str[1], m_pName[2], str[2]);
				break;
			}
		}
	}

	return dispStr;
}

CString CGrp_Bar::GetExcelHead(int idx)
{
	return "";
}

CString CGrp_Bar::GetExcelData(int idx)
{
	return "";
}
