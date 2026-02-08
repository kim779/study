// Grp_Tick.cpp: implementation of the CGrp_Tick class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "C_TickEx.h"
#include "Grp_Tick.h"
#include "GrpWnd.h"
#include "Grp_Data.h"

#include <float.h>
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	gapTICK	2
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrp_Tick::CGrp_Tick(CWnd *pView, class CGrpWnd *pGrpWnd, int index, char *pInfo)
{
	m_pView = pView;
	m_pGrpWnd = pGrpWnd;

	const struct _gInfo *pGInfo = (struct _gInfo*)pInfo;

	m_gKind = pGInfo->gKind;
	m_dKind = pGInfo->dKind;
	m_dIndex = pGInfo->dIndex;
	m_Index = index;

	m_tRGB = pGInfo->tRGB;
	m_pRGB = pGInfo->pRGB;

	m_DrawRect = m_pGrpWnd->m_pGrpRgn[m_Index]->gpRect;

	m_Max = DBL_MIN;
	m_Min = DBL_MAX;

	m_pApp = (CC_TickExApp *)AfxGetApp();

	CalculateMinMax();
}

CGrp_Tick::~CGrp_Tick()
{

}

void CGrp_Tick::Resize()
{
	m_orgDrawRect = m_DrawRect = m_pGrpWnd->m_pGrpRgn[m_Index]->gpRect;
	m_DrawRect.right -= 5;
	m_DrawRect.top += 12;
}

void CGrp_Tick::DrawTickRight(CDC* pDC, double Max, double Min, int digit)
{
	int	dispCount{};
	double	dispValue[64]{};
	CRect	tickRC = m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkRIGHT].tkRect;
	tickRC.top = m_DrawRect.top;	tickRC.bottom = m_DrawRect.bottom;

	const int	tickHeight = tickRC.Height();
	double	ValueHeight = Max - Min;
	if (ValueHeight <= 0) 	ValueHeight = 1;
	
	double	div{};
	if (ValueHeight < 5)			div = 1;
	else if (ValueHeight < 50)		div = 5;
	else if (ValueHeight < 100)		div = 10;
	else if (ValueHeight < 200)		div = 20;
	else if (ValueHeight < 500)		div = 50;
	else if (ValueHeight < 1000)		div = 100;
	else if (ValueHeight < 2000)		div = 200;
	else if (ValueHeight < 5000)		div = 500;
	else if (ValueHeight < 10000)		div = 1000;
	else if (ValueHeight < 20000)		div = 2000;
	else if (ValueHeight < 50000)		div = 5000;
	else if (ValueHeight < 100000)		div = 10000;
	else if (ValueHeight < 200000)		div = 20000;
	else if (ValueHeight < 500000)		div = 50000;
	else if (ValueHeight < 1000000)		div = 100000;
	else if (ValueHeight < 5000000)		div = 500000;
	else if (ValueHeight < 10000000)	div = 1000000;
	else if (ValueHeight < 50000000)	div = 5000000;
	else if (ValueHeight < 100000000)	div = 10000000;
	else if (ValueHeight < 500000000)	div = 50000000;
	else if (ValueHeight < 1000000000)	div = 100000000;
	else					div = 500000000;

	const int	dispTICK = GetTickCount(ctkRIGHT);
	if (dispTICK <= 0)
		return;

	double	drawMinValue= 0.0;
	//double	dVal = 0.0;

	if (!calculateDisplay(div, Max, Min, drawMinValue, dispTICK, dispValue, dispCount))
		return;

	const int	sMode = pDC->SetBkMode(TRANSPARENT);
	const COLORREF	sColor = pDC->SetTextColor(m_pGrpWnd->m_rtColor);
	const int	style = m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkRIGHT].lstyle;
	const int	width = m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkRIGHT].lwidth;
	const COLORREF	color = m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkRIGHT].lcolor;

	CRect	textRC;
	CFont	*sFont = pDC->SelectObject(m_pGrpWnd->m_pFont);
	CPen	*cPen = m_pApp->GetPen(m_pView, style, width, color);
	CPen	*sPen = pDC->SelectObject(cPen);

	double	dValue{};
	CString	dispStr;
	CPoint	pointS, pointE;
	CRect	tkSaveRC = m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkRIGHT].tkRect;
	tkSaveRC.top = m_orgDrawRect.top;
	tkSaveRC.bottom = m_orgDrawRect.bottom;

	CSize	size;
	int	maxdigit = 7;
	constexpr char const *digitStr[10] = {"2,000,000,000", "200,000,000", "20,000,000",
		"2,000,000", "200,000", "20,000", "2,000", "200", "20", "2"};
	for (int ii = 9; ii >= 0; ii--)
	{
		size = pDC->GetOutputTextExtent(digitStr[ii]);
		if (size.cx >= tkSaveRC.Width())
			break;
		else
			maxdigit = strlen(digitStr[ii]);
	}

	FormatVariableComma(dispStr, Max, digit, maxdigit);
	size = pDC->GetOutputTextExtent(dispStr);
	const int PivotY = tickRC.left + gapTICK + size.cx;

	for (int ii = 0; ii < dispCount; ii++)		// horizon (DOT)
	{
		pointS.x = m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkLEFT].tkRect.right;
		dValue = double(double(tickHeight) * double(Max - dispValue[ii]));
		dValue /= double(ValueHeight);

		pointS.y = tickRC.top + int(dValue);

		if (!tickRC.PtInRect(pointS))
		{
			if (tickRC.bottom < pointS.y)
				continue;
		}

		pointE.x = m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkRIGHT].tkRect.left + 1;
		pointE.y = pointS.y;
		if (pointS.y > m_orgDrawRect.top && pointS.y < m_orgDrawRect.bottom)
		{
			pDC->MoveTo(pointE);
			pDC->LineTo(pointS);
		}

		FormatVariableComma(dispStr, dispValue[ii], digit, maxdigit);
		size = pDC->GetOutputTextExtent(dispStr);

		textRC.SetRect(tickRC.left + gapTICK, pointS.y - size.cy/2,
			PivotY, pointS.y + size.cy/2);

		if (textRC.bottom > m_orgDrawRect.bottom)
		{
			textRC.OffsetRect(0, m_orgDrawRect.bottom - textRC.bottom);
			tkSaveRC.bottom -= size.cy + 1;
		}
		if (textRC.top < tickRC.top)
		{
			textRC.OffsetRect(0, tickRC.top - textRC.top);
		}

		pDC->DrawText(dispStr, &textRC, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	}

	pDC->SelectObject(sFont);
	pDC->SelectObject(sPen);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
}

void CGrp_Tick::DrawTickLeft(CDC* pDC, double Max, double Min, int digit)
{
	int	dispCount{};
	double	dispValue[64]{};
	CRect	tickRC = m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkLEFT].tkRect;
	tickRC.top = m_DrawRect.top;	tickRC.bottom = m_DrawRect.bottom;

	const int tickHeight = tickRC.Height();
	double	ValueHeight = Max - Min;
	if (ValueHeight <= 0)	ValueHeight = 1;

	double	div{};
	if (ValueHeight < 5)			div = 1;
	else if (ValueHeight < 50)		div = 5;
	else if (ValueHeight < 100)		div = 10;
	else if (ValueHeight < 200)		div = 20;
	else if (ValueHeight < 500)		div = 50;
	else if (ValueHeight < 1000)		div = 100;
	else if (ValueHeight < 2000)		div = 200;
	else if (ValueHeight < 5000)		div = 500;
	else if (ValueHeight < 10000)		div = 1000;
	else if (ValueHeight < 20000)		div = 2000;
	else if (ValueHeight < 50000)		div = 5000;
	else if (ValueHeight < 100000)		div = 10000;
	else if (ValueHeight < 200000)		div = 20000;
	else if (ValueHeight < 500000)		div = 50000;
	else if (ValueHeight < 1000000)		div = 100000;
	else if (ValueHeight < 5000000)		div = 500000;
	else if (ValueHeight < 10000000)	div = 1000000;
	else if (ValueHeight < 50000000)	div = 5000000;
	else if (ValueHeight < 100000000)	div = 10000000;
	else if (ValueHeight < 500000000)	div = 50000000;
	else if (ValueHeight < 1000000000)	div = 100000000;
	else					div = 500000000;

	const int dispTICK = GetTickCount(ctkLEFT);
	if (dispTICK <= 0)
		return;

	double	drawMinValue= 0.0;

	if (!calculateDisplay(div, Max, Min, drawMinValue, dispTICK, dispValue, dispCount))
		return;

	const int	sMode = pDC->SetBkMode(TRANSPARENT);
	const COLORREF	sColor = pDC->SetTextColor(m_pGrpWnd->m_ltColor);
	const int	style = m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkLEFT].lstyle;
	const int	width = m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkLEFT].lwidth;
	const COLORREF	color = m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkLEFT].lcolor;

	CRect	textRC;
	CFont	*sFont = pDC->SelectObject(m_pGrpWnd->m_pFont);
	CPen	*cPen = m_pApp->GetPen(m_pView, style, width, color);
	CPen	*sPen = pDC->SelectObject(cPen);

	double	dValue{};
	CString	dispStr;
	CPoint	pointS, pointE;
	CRect	tkSaveRC = m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkLEFT].tkRect;
	tkSaveRC.top = m_orgDrawRect.top;
	tkSaveRC.bottom = m_orgDrawRect.bottom;

	CSize	size;
	int	maxdigit = 7;
	constexpr char const	*digitStr[10] = {"2,000,000,000", "200,000,000", "20,000,000", 
		"2,000,000", "200,000", "20,000", "2,000", "200", "20", "2"};
	for (int ii = 9; ii >= 0; ii--)
	{
		size = pDC->GetOutputTextExtent(digitStr[ii]);
		if (size.cx >= tkSaveRC.Width())
			break;
		else
			maxdigit = strlen(digitStr[ii]);
	}

	for (int ii = 0; ii < dispCount; ii++)		// horizon (DOT)
	{
		pointS.x = m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkLEFT].tkRect.right + 1;
		dValue = double(double(tickHeight) * double(Max - dispValue[ii]));
		dValue /= double(ValueHeight);

		pointS.y = tickRC.top + int(dValue);

		if (!tickRC.PtInRect(CPoint(pointS.x -1, pointS.y)))
		{
			if (tickRC.bottom < pointS.y)
				continue;
		}

		pointE.x = m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkRIGHT].tkRect.left;
		pointE.y = pointS.y;
		if (pointS.y > m_orgDrawRect.top && pointS.y < m_orgDrawRect.bottom)
		{
			pDC->MoveTo(pointS);
			pDC->LineTo(pointE);
		}

		FormatVariableComma(dispStr, dispValue[ii], digit, maxdigit);

		size = pDC->GetOutputTextExtent(dispStr);

		textRC.SetRect(tickRC.left, pointS.y - size.cy/2,
			tickRC.right - gapTICK, pointS.y + size.cy/2);

		if (textRC.bottom > tkSaveRC.bottom)
		{
			textRC.OffsetRect(0, tkSaveRC.bottom - textRC.bottom);
			tkSaveRC.bottom -= size.cy + 1;
		}
		if (textRC.top < tickRC.top)
		{
			textRC.OffsetRect(0, tickRC.top - textRC.top);
		}

		pDC->DrawText(dispStr, textRC, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
	}

	pDC->SelectObject(sFont);
	pDC->SelectObject(sPen);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
}

void CGrp_Tick::DrawTickBottom(CDC *pDC)
{
	if (!m_pGrpWnd->m_pGrpData)
		return;

	CRect	tickRC = m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkBOTTOM].tkRect;
	const CRect	bottomRC = m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkBOTTOM].tkRect;
	tickRC.right = m_DrawRect.right;
	tickRC.left = m_DrawRect.left;
	const int tickWidth = tickRC.Width();
	const int tickHeight = tickRC.Height();

	const int	sMode = pDC->SetBkMode(TRANSPARENT);
	const COLORREF	sColor = pDC->SetTextColor(m_pGrpWnd->m_btColor);
	const int	style = m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkBOTTOM].lstyle;
	const int	width = m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkBOTTOM].lwidth;
	const COLORREF	color = m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkBOTTOM].lcolor;

	CFont	*sFont = pDC->SelectObject(m_pGrpWnd->m_pFont);
	CPen	*cPen = m_pApp->GetPen(m_pView, style, width, color);
	CPen	*sPen = pDC->SelectObject(cPen);

	CPoint	pointDAY;
	//int svYear = 0; int svMon = 0; int svDAY = 0;
	int svHH = 0; 
//	int svMM = 0;
	CRect	tkSaveRC(0, 0, 0, 0);
	CRect	drawRC;
	
	CSize	size;

	CString	dispStr;
	int	tPos = 0;
	bool	bDispDay = false;
	const int	dispPos = m_pGrpWnd->m_dispPos;
	const int	dispEnd = m_pGrpWnd->m_dispEnd;
	const int	dispDay = m_pGrpWnd->m_dispDay;
	struct _cgTickEx* cgTickEx{};
	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		cgTickEx = (struct _cgTickEx *)m_pGrpWnd->m_pGrpData->GetGraphData(ii+dispPos);
		pointDAY.x = tickRC.left;
		pointDAY.x += int((tickWidth * ii) / dispDay);
		pointDAY.y = tickRC.top + int(tickHeight / 5);

		if (cgTickEx->time.hh == 0)
			continue;

		if (svHH != cgTickEx->time.hh)
		{
			dispStr.Format("%02d:%02d", cgTickEx->time.hh, cgTickEx->time.mm);
			tPos = pointDAY.x;
		}
		else
		{
			continue;
		}
		svHH = cgTickEx->time.hh;
		
		if (pointDAY.x >= tickRC.left
			&& pointDAY.x <= tickRC.right)
		{
			pDC->MoveTo(pointDAY.x, m_orgDrawRect.top+1);	// vertic
			pDC->LineTo(pointDAY.x, m_orgDrawRect.bottom);
		}

		size = pDC->GetOutputTextExtent(dispStr);
		if (ii == 0)
			drawRC.SetRect(bottomRC.left, pointDAY.y, 
				bottomRC.left + size.cx+1, pointDAY.y + size.cy);
		else
			drawRC.SetRect(pointDAY.x - size.cx/2, pointDAY.y, 
				pointDAY.x + size.cx/2, pointDAY.y + size.cy);
		if (bDispDay)
		{
			bDispDay = false;
			if (drawRC.right > bottomRC.right)
				drawRC.OffsetRect(bottomRC.right - drawRC.right, 0);
			if (tkSaveRC.right > drawRC.left - 4)
				continue;
			pDC->DrawText(dispStr, &drawRC, DT_SINGLELINE|DT_TOP|DT_CENTER);
			tkSaveRC = drawRC;
		}
		else
		{
			if (drawRC.right > bottomRC.right)
				drawRC.OffsetRect(bottomRC.right - drawRC.right, 0);
			if (tkSaveRC.right > drawRC.left - 4)
				continue;
			pDC->DrawText(dispStr, &drawRC, DT_SINGLELINE|DT_TOP|DT_CENTER);
			tkSaveRC = drawRC;
		}
	}
		
	pDC->SelectObject(sFont);
	pDC->SelectObject(sPen);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
}

void CGrp_Tick::FormatVariableComma(CString &dispTxt, double dVal, int point, int maxDigit)
{
	CString	edit1, edit2;
	double dotL = 0, dotR = 0;

	bool bSign = false;
	if (dVal < 0)
	{
		bSign = true;
		dVal = dVal * (-1);
	}
	switch (point)
	{
	case 2:
		dotL = int(dVal / 100.0);
		dotR = int(int(dVal) % 100);
		break;
	default:
		dotL = dVal;
		break;
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
	switch (point)
	{
	case 2:
		edit1.Format(".%02d", int(dotR+0.5));
		break;
	default:
		break;
	}

	const int trimComma = edit2.GetLength() + edit1.GetLength() - maxDigit;
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

int CGrp_Tick::GetTickCount(int dir)
{
	int	dispTICK = 5;
	int	tkHeight{};

	CRect	RC;
	switch (dir)
	{
	case ctkLEFT:
		RC = m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkLEFT].tkRect;
		break;
	case ctkRIGHT:
		RC = m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkRIGHT].tkRect;
		break;
	case ctkBOTTOM:
		RC = m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkBOTTOM].tkRect;
		break;
	}

	tkHeight = RC.Height();
	dispTICK = int(tkHeight / (m_pGrpWnd->m_pGrpRgn[m_Index]->tkHeight * 1.5));

	if (dispTICK > 10)
		dispTICK = 10;

	return dispTICK;
}

bool CGrp_Tick::calculateDisplay(double div, double Max, double Min, double& drawMinValue, 
		int dispTICK, double* dispValue, int& dispCount)
{
	int	index{};
	CString	temp;
	double	mok = Min / div;
	temp.Format("%f", mok);
	index = temp.Find('.'); 
	mok = atof(temp.Left(index));
	drawMinValue = mok * div;
	if (drawMinValue < Min)		
		drawMinValue += div;
	const int	drawNum = int ((Max - drawMinValue) / div);
	if (drawNum >= dispTICK)
	{
		if (drawNum <= dispTICK*2)
			div *= 2;
		else if (drawNum <= dispTICK*4)
			div *= 4;
		else if (drawNum <= dispTICK*5)
			div *= 5;
		else
			div *= 6;

		mok = Min / div;
		temp.Format("%f", mok);
		index = temp.Find('.'); 
		mok = atof(temp.Left(index));
		drawMinValue = mok * div;
		if (drawMinValue < Min)		
			drawMinValue += div;
	}
	dispCount = 0;
	dispValue[dispCount] = drawMinValue;
//	double dVal = 0.0;

	while(1)
	{
		if (dispValue[dispCount] > Max + 0.01)
			break;

		dispValue[dispCount+1] = dispValue[dispCount] + div;
		dispCount++;
	}
	if (dispCount <= 0)
	{
		if (Max == Min)
		{
			dispValue[0] = Max;
			dispCount = 1;
		}
		else
			return false;
	}
	return true;
}

void CGrp_Tick::DrawGraph(CDC *pDC)
{
	if (m_Max == DBL_MIN || m_Min == DBL_MAX)
		return;

	if (m_Index != 0)
	{
		if (m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkLEFT].bDraw)		DrawTickLeft(pDC, m_Max, m_Min, 0);
		if (m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkRIGHT].bDraw)	DrawTickRight(pDC, m_Max, m_Min, 0);
		DrawTickBottom(pDC);
		DrawVol(pDC);
	}
	else
	{
		if (m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkLEFT].bDraw)		DrawTickLeft(pDC, m_Max, m_Min, m_pGrpWnd->m_digit);
		if (m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkRIGHT].bDraw)	DrawTickRight(pDC, m_Max, m_Min, m_pGrpWnd->m_digit);
		DrawTickBottom(pDC);
		DrawLine(pDC);
	}
	DrawName(pDC);
}

void CGrp_Tick::DrawLine(CDC *pDC)
{
	const int	dispPos = m_pGrpWnd->m_dispPos;
	const int	dispEnd = m_pGrpWnd->m_dispEnd;
	const int	dispDay = m_pGrpWnd->m_dispDay;

	if (dispDay < 1)
		return;

	const COLORREF color = m_tRGB;
	const COLORREF clrUP = m_pApp->GetColor(m_pView, 94);
	const COLORREF clrDN = m_pApp->GetColor(m_pView, 95);
	
	CPen	*dPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, clrDN);
	CPen	*uPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, clrUP);
	CPen	*sPen = pDC->SelectObject(uPen);

	const int DrawHeight = m_DrawRect.Height();
	const int DrawWidth  = m_DrawRect.Width();

	double ValueHeight = m_Max - m_Min;
	if (!ValueHeight) ValueHeight = 1;
	const int drawShift = int((DrawWidth/dispDay) / 2);	// Center

	double epr{};
		
	CPoint ptEPR, ptPRE;

	struct _cgTickEx* gTickEx{};
	int	xPivot{};
	const int yPivot = m_DrawRect.top + int((DrawHeight * (m_Max - double(m_pGrpWnd->m_pJgga))) / ValueHeight);
	const int bPivot = (m_pGrpWnd->m_pJgga <= m_Max && m_pGrpWnd->m_pJgga >= m_Min)? true:false;
	if (bPivot)
		DrawPrePrc(pDC);

	gTickEx = (struct _cgTickEx *)m_pGrpWnd->m_pGrpData->GetGraphData(dispPos);
	ptEPR.x = m_DrawRect.left + drawShift;
	ptEPR.y = m_DrawRect.top;
	ptEPR.y += int((DrawHeight * (m_Max - double(gTickEx->epr))) / ValueHeight);

	if (yPivot < ptEPR.y)	pDC->SelectObject(dPen);
	pDC->MoveTo(ptEPR);
	ptPRE = ptEPR;

	if (bPivot)
	{
		for (int ii = 1; ii < dispEnd - dispPos; ii++)
		{
			gTickEx = (struct _cgTickEx *)m_pGrpWnd->m_pGrpData->GetGraphData(ii+dispPos);
			epr = gTickEx->epr;

			if (epr < m_Min || epr > m_Max)
				continue;

			ptEPR.x = m_DrawRect.left + drawShift;
			ptEPR.x += int((DrawWidth * ii) / dispDay);
			ptEPR.y = m_DrawRect.top;
			ptEPR.y += int((DrawHeight * (m_Max - double(epr))) / ValueHeight);

			if (yPivot < ptPRE.y && yPivot < ptEPR.y)
			{
				pDC->SelectObject(dPen);
				pDC->LineTo(ptEPR);
			}
			else if (yPivot >= ptPRE.y && yPivot >= ptEPR.y)
			{
				pDC->SelectObject(uPen);
				pDC->LineTo(ptEPR);
			}
			else if (yPivot >= ptPRE.y && yPivot < ptEPR.y)	// up, down
			{
				xPivot = ptPRE.x + (yPivot-ptPRE.y)*(ptEPR.x-ptPRE.x)/(ptEPR.y-ptPRE.y);
				pDC->LineTo(xPivot, yPivot);
				pDC->SelectObject(dPen);
				pDC->LineTo(ptEPR);
			}
			else	// down, up
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
			gTickEx = (struct _cgTickEx *)m_pGrpWnd->m_pGrpData->GetGraphData(ii+dispPos);
			epr = gTickEx->epr;

			if (epr < m_Min || epr > m_Max)
				continue;

			ptEPR.x = m_DrawRect.left + drawShift;
			ptEPR.x += int((DrawWidth * ii) / dispDay);
			ptEPR.y = m_DrawRect.top;
			ptEPR.y += int((DrawHeight * (m_Max - double(epr))) / ValueHeight);
			pDC->LineTo(ptEPR);
		}
	}

	pDC->SelectObject(sPen);
}

void CGrp_Tick::DrawPrePrc(CDC *pDC)
{
	const int	pjgga = m_pGrpWnd->m_pJgga;

	CPen	*cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, RGB(1, 196, 1));
	CPen	*sPen = pDC->SelectObject(cPen);

	const int y = m_DrawRect.top + int((m_DrawRect.Height() * (m_Max - double(pjgga))) / (m_Max - m_Min));
	pDC->MoveTo(m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkLEFT].tkRect.right, y);
	pDC->LineTo(m_pGrpWnd->m_pGrpRgn[m_Index]->tick[ctkRIGHT].tkRect.left, y);

	pDC->SelectObject(sPen);
}

void CGrp_Tick::DrawVol(CDC *pDC)
{
	const int	dispPos = m_pGrpWnd->m_dispPos;
	const int	dispEnd = m_pGrpWnd->m_dispEnd;
	const int	dispDay = m_pGrpWnd->m_dispDay;

	const COLORREF clrUP = m_pApp->GetColor(m_pView, 94);
	const COLORREF clrDN = m_pApp->GetColor(m_pView, 95);
	
	CPen	*uPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, clrUP);
	CPen	*dPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, clrDN);
	CPen	*sPen = pDC->SelectObject(uPen);

	CBrush	*uBrush =  m_pApp->GetBrush(m_pView, clrUP);
	CBrush	*dBrush =  m_pApp->GetBrush(m_pView, clrDN);
	CBrush	*sBrush = pDC->SelectObject(uBrush);

	const int DrawHeight = m_DrawRect.Height();
	const int DrawWidth  = m_DrawRect.Width();

	double ValueHeight = m_Max - m_Min;
	if (ValueHeight == 0) 
		ValueHeight = 1;

	double	trn{};
	double	dValue{};
	double	preVal = 0;
	CRect	rectTRN;

	if (dispPos != 0 && dispDay > 0)
		preVal = ((struct _cgTickEx *)m_pGrpWnd->m_pGrpData->GetGraphData(dispPos - 1))->trn;

	struct _cgTickEx* gTickEx{};
	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		gTickEx = (struct _cgTickEx *)m_pGrpWnd->m_pGrpData->GetGraphData(ii+dispPos);

		if ((trn = gTickEx->trn) <= 0)
			continue;
	
		rectTRN.left = m_DrawRect.left;
		rectTRN.left += int((DrawWidth * ii) / dispDay+0.5);
		rectTRN.right = m_DrawRect.left;
		rectTRN.right += int((DrawWidth * (ii +1)) / dispDay+0.5);
		rectTRN.right -= 1;

		const int gap = rectTRN.right - rectTRN.left;
		if (gap > 20)
		{
			rectTRN.left += (gap-20)/2;
			rectTRN.right -= (gap-20)/2;
		}

		rectTRN.top = m_DrawRect.top;
		dValue = double(DrawHeight) * double(m_Max - trn);
		rectTRN.top += int(dValue / ValueHeight);
		rectTRN.bottom = m_DrawRect.bottom;// - gapBOTTOM;

		if (trn >= preVal)
		{
			pDC->SelectObject(uPen);
			pDC->SelectObject(uBrush);
		}
		else
		{
			pDC->SelectObject(dPen);
			pDC->SelectObject(dBrush);
		}
		preVal = trn;

		if (rectTRN.Width() > 2)
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

	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);
}

void CGrp_Tick::DrawName(CDC *pDC)
{
	CString	str = (m_Index == 0)? "종가":"거래량";

	const COLORREF	sColor = pDC->SetTextColor(RGB(0,128,0));
	CFont	*sFont = pDC->SelectObject(m_pGrpWnd->m_pFont);
	const int	sMode = pDC->SetBkMode(TRANSPARENT);

	CRect	rc = m_orgDrawRect;
	rc.DeflateRect(2, 2);

	pDC->DrawText(str, &rc, DT_LEFT|DT_SINGLELINE|DT_TOP);

	pDC->SetTextColor(sColor);
	pDC->SelectObject(sFont);
	pDC->SetBkMode(sMode);
}

bool CGrp_Tick::CalculateMinMax()
{
	m_Max = INT_MIN;
	m_Min = INT_MAX;

	if (!m_pGrpWnd->m_pGrpData || 
		m_pGrpWnd->m_pGrpData->GetDataCount() <= 0)
		return false;

	const int	dispPos = m_pGrpWnd->m_dispPos;
	const int	dispEnd = m_pGrpWnd->m_dispEnd;
	const int	dispDay = m_pGrpWnd->m_dispDay;

	struct _cgTickEx* gTickEx{};
	if (m_gKind == CGK_VOL)
	{
		for (int ii = 0; ii < dispEnd - dispPos; ii++)
		{
			gTickEx = (struct _cgTickEx *)m_pGrpWnd->m_pGrpData->GetGraphData(ii+dispPos);
			if (m_Max < gTickEx->trn)	m_Max = gTickEx->trn;
			if (m_Min > gTickEx->trn)	m_Min = gTickEx->trn;
		}
	}
	else
	{
		for (int ii = 0; ii < dispEnd - dispPos; ii++)
		{
			gTickEx = (struct _cgTickEx *)m_pGrpWnd->m_pGrpData->GetGraphData(ii+dispPos);
			if (m_Max < gTickEx->epr)	m_Max = gTickEx->epr;
			if (m_Min > gTickEx->epr)	m_Min = gTickEx->epr;
		}

		double	dMax = m_Max;
		double	dMin = m_Min;

		const double gap = fabs(dMax - dMin);
		if (gap == 0)
		{
			if (dMax > 100)
			{
				dMax += dMax / 20;
				dMin -= dMin / 20;
			}
			else
			{
				dMax += 5;
				dMin -= 5;
			}
		}

		int	index;
		CString	temp;
		temp.Format("%f", dMax + 1);
		index = temp.Find('.'); 
		dMax = atof(temp.Left(index));
		temp.Format("%f", dMin - 1);
		index = temp.Find('.'); 
		dMin = atof(temp.Left(index));

		m_Max = dMax;
		m_Min = dMin;
	}

	return true;
}

bool CGrp_Tick::IsChangeMinMax()
{
	const double	m_sMax = m_Max;
	const double	m_sMin = m_Min;

	if (!CalculateMinMax())
		return false;

	if (m_sMax != m_Max || m_sMin != m_Min)
		return true;

	return false;
}

CString CGrp_Tick::GetDisplayPosData(CPoint pt)
{
	CString	dispStr = _T("");

	if (m_DrawRect.left > pt.x || m_DrawRect.right <= pt.x)
		return dispStr;

	if (!m_pGrpWnd->m_pGrpData || 
		m_pGrpWnd->m_pGrpData->GetDataCount() <= 0)
		return dispStr;

	const int	dispPos = m_pGrpWnd->m_dispPos;
	const int	dispEnd = m_pGrpWnd->m_dispEnd;
	const int	dispDay = m_pGrpWnd->m_dispDay;

	const int	xPosition = pt.x - m_DrawRect.left;
	const double	szOneDay = double(m_DrawRect.Width()) / double(dispDay);
	int	dataPos = int(double(xPosition) / szOneDay);
	dataPos += dispPos;

	CString	stmp, stmp2, sSign, sSign2;
	if (m_dKind == CDK_TICK)
	{
		const struct _cgTickEx	*gTickEx =
			(struct _cgTickEx *)m_pGrpWnd->m_pGrpData->GetGraphData(dataPos);

		if (gTickEx == nullptr)
			return dispStr;

		dispStr.Format("%s\t%02d:%02d:%02d", "시간", gTickEx->time.hh, gTickEx->time.mm, gTickEx->time.ss);
		FormatVariableComma(stmp, gTickEx->epr, m_pGrpWnd->m_digit);
		dispStr = dispStr + "\n종가\t" + stmp;
		FormatVariableComma(stmp, gTickEx->trn, 0);
		dispStr = dispStr + "\n거래량\t" + stmp;
	}
	
	return dispStr;
}

void CGrp_Tick::GetMaxStr(CString &sMax)
{
	sMax = _T("");
	FormatVariableComma(sMax, m_Max, m_pGrpWnd->m_digit);
}

void CGrp_Tick::GetMinStr(CString &sMin)
{
	sMin = _T("");
	FormatVariableComma(sMin, m_Min, m_pGrpWnd->m_digit);
}

CString CGrp_Tick::GetExcelHead(int idx)
{
	CString	dispStr = _T("");
	CString	stmp;
	const struct _cgTickEx	*gTickEx = (struct _cgTickEx *)m_pGrpWnd->m_pGrpData->GetGraphData(idx);

	if (idx < 0)
		dispStr = "시간";
	else
		dispStr.Format("%02d:%02d:%02d", gTickEx->time.hh, gTickEx->time.mm, gTickEx->time.ss);

	return dispStr;
}

CString CGrp_Tick::GetExcelData(int idx)
{
	CString	dispStr = __T("");

	if (!m_pGrpWnd->m_pGrpData || 
		m_pGrpWnd->m_pGrpData->GetDataCount() <= 0)
		return dispStr;
	
	if (idx < 0)
	{
		dispStr.Format("종가\t거래량");
	}
	else
	{
		CString	stmp, sSign, sSign2;
		const struct _cgTickEx	*gTickEx = (struct _cgTickEx *)m_pGrpWnd->m_pGrpData->GetGraphData(idx);

		FormatVariableComma(stmp, gTickEx->epr, m_pGrpWnd->m_digit);
		dispStr += stmp;
		FormatVariableComma(stmp, gTickEx->trn, 0);
		dispStr += "\t" + stmp;
	}

	return dispStr;
}