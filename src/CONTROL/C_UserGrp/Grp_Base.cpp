// Grp_Base.cpp: implementation of the CGrp_Base class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "C_UserGrp.h"
#include "Grp_Base.h"
#include "Grp_Data.h"
#include "GrpWnd.h"

#include <math.h>
//#include <fstream.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define	gapTICK	2

CGrp_Base::CGrp_Base(CWnd *pView, class CGrpWnd *pGrpWnd, char *pInfo)
{
	m_pView = pView;
	m_pGrpWnd = pGrpWnd;

	const	struct _gNInfo *pGInfo = (struct _gNInfo *)pInfo;

	m_gKind = pGInfo->gKind;
	m_rKey = pGInfo->rKey;
	m_dKey = pGInfo->dKey;
	m_dIndex = pGInfo->dIndex;
	m_mIndex = pGInfo->mIndex;
	m_lIndex = pGInfo->lIndex;
	m_TickInfo = pGInfo->Tick;
	m_LineWidth = pGInfo->lwidth;
	m_LineStyle = pGInfo->lstyle;

	m_tRGB = pGInfo->tRGB;
	m_pRGB = pGInfo->pRGB;
	m_clrUp = m_pApp->GetColor(pView, CLR_UP);
	m_clrDn = m_pApp->GetColor(pView, CLR_DOWN);

	m_DrawRect = m_pGrpWnd->m_pRgnInfo[m_rKey]->gpRect;

	m_Max = INT_MIN;
	m_Min = INT_MAX;
	m_pApp = (CC_UserGrpApp*)AfxGetApp();

	m_bHide = FALSE;
}

CGrp_Base::~CGrp_Base()
{

}

void CGrp_Base::Resize()
{
	m_RgnRect = m_DrawRect = m_pGrpWnd->m_pRgnInfo[m_rKey]->gpRect;

	CFont	*oldfont = nullptr;
	CDC*	pDC = m_pGrpWnd->GetDC();
	CSize	size;
	oldfont = pDC->SelectObject(m_pGrpWnd->m_pFont);
	size = pDC->GetTextExtent("굳");
	pDC->SelectObject(oldfont);
	m_pGrpWnd->ReleaseDC(pDC);
	
	const	int	gap = size.cy/2;
	if (m_gKind == IGK_2D3NBAR)
	{
		m_DrawRect.top += 12;
	}
	else
	{
		if (m_pGrpWnd->m_totalDay > 0)
		{
			m_DrawRect.top += gap*3;
			m_DrawRect.left += 3;
			m_DrawRect.right -= 3;
		}
	}
}

void CGrp_Base::DrawTickHorizon(CDC* pDC, double Max, double Min, int tickinfo)
{
//ofstream dFile ("D:\\dataG.txt", ios::app);
//dFile	<<endl<<"index="<<m_dKey<<"rgn="<<m_rKey<<endl;	

	int	dispCount=0;
	double	dispValue[64]{};
	CRect	tickRC = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[tickinfo].tkRect;
	const	CRect	bottomRC = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].tkRect;

	tickRC.top = m_DrawRect.top;
	tickRC.bottom = m_DrawRect.bottom;

	const	int	tickHeight = tickRC.Height();
	double	ValueHeight = Max - Min;
	if (ValueHeight <= 0) 	ValueHeight = 1;
	
	double	div = 0;
	if (ValueHeight < 5)			div = 1;
	else if (ValueHeight < 50)		div = 5;
	else if (ValueHeight < 200)		div = 10;
	else
	{
		CString	strTmp, strDiv;
		strTmp.Format("%.0f", ValueHeight);
		if (strTmp.GetAt(0) < '2')	// if ( < 2xxx) ---> 1xx
		{
			strDiv.Format("1%0*d", strTmp.GetLength()-2, 0);
			div = atof(strDiv);
		}
		else			// if ( < 5xxx) ---> 5xx
		{
			strDiv.Format("5%0*d", strTmp.GetLength()-2, 0);
			div = atof(strDiv);
		}
	}

	const	int	dispTICK = GetTickCount(tickinfo);
	if (dispTICK <= 0)
		return;

	double	drawMinValue= 0.0;

	if (!calculateDisplay(div, Max, Min, drawMinValue, dispTICK, dispValue, dispCount))
		return;

	const	int		sMode = pDC->SetBkMode(TRANSPARENT);
	const	COLORREF	sColor = pDC->SetTextColor(m_pGrpWnd->m_TkTextColor);
	const	int		style = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[tickinfo].lstyle;
	const	int		width = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[tickinfo].lwidth;
	const	COLORREF	color = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[tickinfo].lcolor;

	CRect	textRC;
	CFont	*sFont = pDC->SelectObject(m_pGrpWnd->m_pFont);
	CPen	*cPen = m_pApp->GetPen(m_pView, style, width, color);
	CPen	*sPen = pDC->SelectObject(cPen);

	double	dValue = 0;
	CString	dispStr;
	CPoint	pointS, pointE;
	CRect	tkSaveRC = CRect(10000,10000,10000,10000);
	CSize	size;
	int	maxdigit = 7;
	const char* const digitStr[10] = {"2,000,000,000", "200,000,000", "20,000,000", 
		"2,000,000", "200,000", "20,000", "2,000", "200", "20", "2"};
	for (int ii = 9; ii >= 0; ii--)
	{
		size = pDC->GetOutputTextExtent(digitStr[ii]);
		if (size.cx >= tkSaveRC.Width())
			break;
		else
			maxdigit = strlen(digitStr[ii]);
	}

	int	nMaxLen = 0;

	for (int ii = 0 ; ii < dispCount; ii++ )
	{
		FormatVariableComma(dispStr, dispValue[ii], m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->digit);
		size = pDC->GetOutputTextExtent(dispStr);

		if (nMaxLen < size.cx)
			nMaxLen = size.cx;
	}

	size = pDC->GetOutputTextExtent(" ");
	const	int	nGap = size.cx;
	CRect	rcSave;

	for (int ii = 0; ii < dispCount; ii++)		// 가격별가로 (DOT)
	{
		pointS.x = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkLEFT].tkRect.right+1;
		dValue  = (double)tickHeight * (Max - dispValue[ii]);
		dValue /= ValueHeight;
		pointS.y = tickRC.top + (int)dValue;

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

		FormatVariableComma(dispStr, dispValue[ii], m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->digit);
		size = pDC->GetOutputTextExtent(dispStr);
		textRC.SetRect(tickRC.left + gapTICK, pointS.y - size.cy/2,
			tickRC.right, pointS.y + size.cy/2);

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
		pDC->DrawText(dispStr, &textRC, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
		rcSave = textRC;
	}

	textRC.SetRect(tickRC.left, bottomRC.top, tickRC.right, bottomRC.bottom);
	CString sDegree;
	sDegree = m_pGrpWnd->m_sTickUnit[tickinfo];
	if (!sDegree.IsEmpty())
	{
		pDC->DrawText(sDegree, textRC, DT_CENTER|DT_SINGLELINE|DT_BOTTOM);
	}

	pDC->SelectObject(sFont);
	pDC->SelectObject(sPen);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
}

void CGrp_Base::DrawTickBottom(CDC *pDC)
{
	if (!m_pGrpWnd->m_pDataInfo)
		return;

	if (m_pGrpWnd->m_bTickVertic)
	{
		DrawVerticNameBottom(pDC);
	}
	else
	{
		if (m_pGrpWnd->m_bStringIndex)	// 문자열틱
			DrawNameBottom(pDC);
		else				// 일자틱
			DrawDateBottom(pDC);
	}
}

int CGrp_Base::DrawTickIndex(CDC* pDC, CString dispText, COLORREF tColor)
{
	if (dispText.IsEmpty())
		return 0;

	int	ret = 0;
	const	int	sMode = pDC->SetBkMode(TRANSPARENT);
	const	COLORREF sColor = pDC->SetTextColor(tColor);
	CFont	*sFont = pDC->SelectObject(m_pGrpWnd->m_pBoldFont);

	CString	realSizeStr = dispText;
	CSize	size = pDC->GetOutputTextExtent(realSizeStr);
	CSize	size2 = pDC->GetOutputTextExtent("D");
	size.cx = size.cx + size2.cx/2;
	size2.cx = size2.cx + size2.cx/2;

	CPoint pt;
	struct _NMinMaxChk	*pMinMaxChk = m_pGrpWnd->GetMinMaxChk(m_rKey);
	int	antiX=0, antiY=0;
	const	int	startX = m_RgnRect.left + 5;
	const	int	startX2 = m_RgnRect.right - 5;
	const	int	startY = m_RgnRect.top + 3;
	if (m_lIndex == 0)
	{
		antiX = pMinMaxChk->tickx[1];
		antiY = pMinMaxChk->ticky[1];
		if (antiX == 0)
		{
			antiX = startX2;
			antiY = startY;
		}

		if (pMinMaxChk->tickx[0] == 0)
		{
			pMinMaxChk->tickx[0] = startX;
			pMinMaxChk->ticky[0] = startY;
		}

		if (pMinMaxChk->tickx[0] + size.cx > antiX)
		{
			pt.x = m_RgnRect.left + 5;
			pt.y = antiY + pMinMaxChk->ticky[0] - startY + size.cy;
		}
		else
		{
			pt.x = pMinMaxChk->tickx[0];
			pt.y = pMinMaxChk->ticky[0];
		}

		if (pt.y + size.cy > m_RgnRect.bottom)
		{
			pDC->SelectObject(sFont);
			pDC->SetTextColor(sColor);
			pDC->SetBkMode(sMode);

			return 0;
		}

		CRect	dispRC;
		dispRC.SetRect(pt.x, pt.y, pt.x + size.cx, pt.y + size.cy);
		dispRC.NormalizeRect();
		pDC->DrawText(dispText, &dispRC, DT_SINGLELINE|DT_TOP|DT_LEFT);

		pMinMaxChk->tickx[0] = pt.x + size.cx;
		pMinMaxChk->ticky[0] = pt.y;

		ret = pt.x + size.cx;
	}
	else
	{
		antiX = pMinMaxChk->tickx[0];
		antiY = pMinMaxChk->ticky[0];
		if (antiX == 0)
		{
			antiX = startX;
			antiY = startY;
		}

		if (pMinMaxChk->tickx[1] == 0)
		{
			pMinMaxChk->tickx[1] = startX2;
			pMinMaxChk->ticky[1] = startY;
		}

		if (pMinMaxChk->tickx[1] - size.cx < antiX)
		{
			pt.x = startX2;
			pt.y = antiY + pMinMaxChk->ticky[1] - startY + size.cy;
		}
		else
		{
			pt.x = pMinMaxChk->tickx[1];
			pt.y = pMinMaxChk->ticky[1];
		}

		if (pt.y + size.cy > m_RgnRect.bottom)
		{
			pDC->SelectObject(sFont);
			pDC->SetTextColor(sColor);
			pDC->SetBkMode(sMode);

			return 0;
		}

		CRect	dispRC;
		dispRC.SetRect(pt.x - size.cx, pt.y, pt.x, pt.y + size.cy);
		dispRC.NormalizeRect();
		pDC->DrawText(dispText, &dispRC, DT_SINGLELINE|DT_TOP|DT_LEFT);

		pMinMaxChk->tickx[1] = pt.x - size.cx;
		pMinMaxChk->ticky[1] = pt.y;

		ret = pt.x - size.cx;
	}

	pDC->SelectObject(sFont);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);

	return ret;	
}

void CGrp_Base::FormatVariableComma(CString &dispTxt, double dVal, int point, int maxDigit)
{
	CString	edit1, edit2;
	double	dotL = 0, dotR = 0;

	bool bSign = false;
	if (dVal < 0)
	{
		bSign = true;
		dVal = dVal * (-1);
	}
	if (point > 0)
	{
		const	double	div = pow(10.0, point);
		dotL = (int)(dVal / div);
		dotR = (int)((int)(dVal) % (int)div);
	}
	else
		dotL = dVal;

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

	if (point > 0)
		edit1.Format(".%0*d", point, (int)(dotR+0.5));
	else
		edit1 = _T("");

	const	int	trimComma = edit2.GetLength() + edit1.GetLength() - maxDigit;
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

int CGrp_Base::GetTickCount(int dir)
{
	int	dispTICK = 5;
	int	tkHeight = 0;

	CRect	RC;
	switch (dir)
	{
	case ctkLEFT:
		RC = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkLEFT].tkRect;
		RC.top = m_DrawRect.top;
		RC.bottom = m_DrawRect.bottom;
		break;
	case ctkRIGHT:
		RC = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkRIGHT].tkRect;
		RC.top = m_DrawRect.top;
		RC.bottom = m_DrawRect.bottom;
		break;
	case ctkBOTTOM:
		RC = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].tkRect;
		break;
	}

	tkHeight = RC.Height();
	dispTICK = (int)(tkHeight / (m_pGrpWnd->m_pRgnInfo[m_rKey]->tkHeight * 2));

	if (dispTICK > 10)
		dispTICK = 10;

	return dispTICK;
}

bool CGrp_Base::calculateDisplay(double div, double Max, double Min, double& drawMinValue, 
		int dispTICK, double* dispValue, int& dispCount)
{
	int	index = -1;
	CString	temp;
	double	mok = Min / div;

	temp.Format("%f", mok);
	index = temp.Find('.'); 
	mok = atof(temp.Left(index));
	drawMinValue = mok * div;
	if (drawMinValue < Min)		
		drawMinValue += div;

	const	int	drawNum = int ((Max - drawMinValue) / div);
	if (drawNum >= dispTICK)
	{
		CString	tStr;
		tStr.Format("%f", div);
		if (tStr.GetAt(0) == '5')
		{
			if (drawNum <= dispTICK*1)
				div *= 1;
			else if (drawNum <= dispTICK*2)
				div *= 2;
			else if (drawNum <= dispTICK*10)
				div *= 10;
			else if (drawNum <= dispTICK*20)
				div *= 20;
			else
				div *= 100;
		}
		else
		{
			if (drawNum <= dispTICK*1)
				div *= 1;
			else if (drawNum <= dispTICK*2)
				div *= 2;
			else if (drawNum <= dispTICK*2)
				div *= 3;
			else if (drawNum <= dispTICK*5)
				div *= 5;
			else if (drawNum <= dispTICK*10)
				div *= 10;
			else if (drawNum <= dispTICK*50)
				div *= 50;
			else
				div *= 100;
		}

		mok = Min / div;
		temp.Format("%f", mok);
		index = temp.Find('.'); 
		mok = atof(temp.Left(index));
		drawMinValue = mok * div;
		if (drawMinValue < Min)		
			drawMinValue += div;
	}
	dispCount = 0;

	if (false)//m_pGrpWnd->m_gKind == GTYPE_VERTICAL)
		dispValue[dispCount] = 0.0;
	else
		dispValue[dispCount] = drawMinValue;
	
	while (1)
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

void CGrp_Base::GetMinMaxValue(double& dMin, double& dMax)
{
	dMin = m_Min;
	dMax = m_Max;
}

void CGrp_Base::DrawNameBottom(CDC* pDC)
{
	double ValueHeight = m_Max - m_Min;
	if (!ValueHeight) ValueHeight = 1;
	
	CRect	tickRC = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].tkRect;
	const	CRect	bottomRC = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].tkRect;

	tickRC.right = m_DrawRect.right;
	tickRC.left = m_DrawRect.left;
	const	int	tickWidth = tickRC.Width();
	const	int	tickHeight = tickRC.Height();
	const	int	sMode = pDC->SetBkMode(TRANSPARENT);
	const	COLORREF sColor = pDC->SetTextColor(m_pGrpWnd->m_TkTextColor);
	const	int	style = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].lstyle;
	const	int	width = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].lwidth;
	const	COLORREF color = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].lcolor;

	CFont	*sFont = pDC->SelectObject(m_pGrpWnd->m_pFont);
	CPen	*cPen = m_pApp->GetPen(m_pView, style, width, color);
	CPen	*sPen = pDC->SelectObject(cPen);

	CPoint	pointDAY;
	CRect	tkSaveRC(0, 0, 0, 0);
	CRect	drawRC;
	
	CSize	size;
	CString	dispStr;
	const	int	dispPos = m_pGrpWnd->m_dispPos;
	const	int	dispEnd = m_pGrpWnd->m_dispEnd;
	const	int	dispDay = m_pGrpWnd->m_dispDay;
	int	drawShift = 0;
	if (dispDay)
		drawShift = (int)((tickWidth/dispDay) / 2);	// Center

	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		const auto gVal = m_pGrpWnd->m_pDataInfo->GetGraphData(ii+dispPos);
		pointDAY.x = tickRC.left + drawShift;
		pointDAY.x += (int)((tickWidth * ii) / dispDay);
		pointDAY.y = tickRC.top + (int)(tickHeight / 5);

		dispStr.Format("%s", gVal->index.name.str);
		size = pDC->GetOutputTextExtent(dispStr);

		drawRC.SetRect(pointDAY.x - size.cx/2,    pointDAY.y, pointDAY.x + size.cx/2,    pointDAY.y + size.cy);
		if (drawRC.left < bottomRC.left)
			drawRC.OffsetRect(bottomRC.left - drawRC.left+2, 0);
		if (drawRC.right > bottomRC.right)
			drawRC.OffsetRect(bottomRC.right - drawRC.right+2, 0);
		
		if (drawRC.left < tkSaveRC.right + 10)
			continue;

		if (m_pGrpWnd->m_bVDraw)
		{
			pDC->MoveTo(pointDAY.x, bottomRC.top);
			pDC->LineTo(pointDAY.x, m_pGrpWnd->m_GrpRect.top);
		}
		pDC->DrawText(dispStr, &drawRC, DT_SINGLELINE|DT_VCENTER|DT_CENTER);	
		tkSaveRC = drawRC;
	}
		
	pDC->SelectObject(sFont);
	pDC->SelectObject(sPen);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
}

void CGrp_Base::DrawVerticNameBottom(CDC* pDC)
{
	double	ValueHeight = m_Max - m_Min;
	if (!ValueHeight) ValueHeight = 1;

	CRect	tickRC = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].tkRect;
	const	CRect	bottomRC = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].tkRect;
	tickRC.right = m_DrawRect.right;
	tickRC.left = m_DrawRect.left;
	const	int	tickWidth = tickRC.Width();
	const	int	sMode = pDC->SetBkMode(TRANSPARENT);
	const	COLORREF sColor = pDC->SetTextColor(m_pGrpWnd->m_TkTextColor);
	const	int	style = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].lstyle;
	const	int	width = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].lwidth;
	const	COLORREF color = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].lcolor;

	CFont	*sFont = pDC->SelectObject(m_pGrpWnd->m_pFont);
	CPen	*cPen = m_pApp->GetPen(m_pView, style, width, color);
	CPen	*sPen = pDC->SelectObject(cPen);

	const	CSize	size = pDC->GetTextExtent("굳");
	const	int	hsize = size.cx/2;
	CPoint	pointDAY;
	CRect	tkSaveRC(0, 0, 0, 0);
	CRect	drawRC;
	
	CString	dispStr;
	const	int	dispPos = m_pGrpWnd->m_dispPos;
	const	int	dispEnd = m_pGrpWnd->m_dispEnd;
	const	int	dispDay = m_pGrpWnd->m_dispDay;
	int	drawShift = 0;
	if (dispDay)
		drawShift = (int)((tickWidth/dispDay) / 2);	// Center

	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		const auto gVal = m_pGrpWnd->m_pDataInfo->GetGraphData(ii+dispPos);
		pointDAY.x = tickRC.left + drawShift;
		pointDAY.x += (int)((tickWidth * ii) / dispDay);
		pointDAY.y = tickRC.top + 5;

		dispStr.Format("%s", gVal->index.name.str);
		drawRC.SetRect(pointDAY.x - hsize, pointDAY.y, pointDAY.x + hsize, pointDAY.y +  tickRC.bottom);
		if (drawRC.left < tkSaveRC.right + 2)
			continue;

		DrawVerticalString(pDC, dispStr, drawRC);
		if (m_pGrpWnd->m_bVDraw)
		{
			pDC->MoveTo(pointDAY.x, bottomRC.top);
			pDC->LineTo(pointDAY.x, m_pGrpWnd->m_GrpRect.top);
		}
		tkSaveRC = drawRC;
	}
		
	pDC->SelectObject(sFont);
	pDC->SelectObject(sPen);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
}

void CGrp_Base::DrawVerticalNumber(CDC* pDC, CString dispStr, CRect drawRC)
{
	const	CSize	size = pDC->GetTextExtent(dispStr);
	CPoint	pt = drawRC.TopLeft();
	CString tmpstr;
	const	int	nCount = dispStr.GetLength();

	if (nCount == 0)
		return;

	for (int ii = 0 ; ii < nCount ; ii++ )
	{
		tmpstr = dispStr.GetAt(ii);
		drawRC.top = pt.y;
		drawRC.bottom = drawRC.top + size.cy;
		pDC->DrawText(tmpstr, drawRC, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		pt.y += size.cy;
	}
}

bool CGrp_Base::CalculateMinMax()
{
	m_Max = INT_MIN;
	m_Min = INT_MAX;

	if (!m_pGrpWnd->m_pDataInfo || m_pGrpWnd->m_pDataInfo->GetDataCount() <= 0)
		return false;

	const	int	dispPos = m_pGrpWnd->m_dispPos;
	const	int	dispEnd = m_pGrpWnd->m_dispEnd;
	for (int ii = 0 ; ii < dispEnd - dispPos ; ii++ )
	{
		const auto gVal = m_pGrpWnd->m_pDataInfo->GetGraphData(ii+dispPos);
		if (m_Max < gVal->vData[m_dKey])	m_Max = gVal->vData[m_dKey];
		if (m_Min > gVal->vData[m_dKey])	m_Min = gVal->vData[m_dKey];
	}

	return true;
}

bool CGrp_Base::IsChangeMinMax(bool bShift)
{
	const	double	sMax = m_Max;
	const	double	sMin = m_Min;

	if (!CalculateMinMax())
		return false;

	if (sMax != m_Max || sMin != m_Min)
		return true;

	return false;
}

void CGrp_Base::DrawGraph(CDC *pDC)
{
	struct _NMinMaxChk *pMinMaxChk;
	if (pMinMaxChk = m_pGrpWnd->GetMinMaxChk(m_rKey))
	{
		if (m_mIndex >= 0 && pMinMaxChk->coMinMax[m_mIndex])
		{
			m_Max = pMinMaxChk->Max[m_mIndex];
			m_Min = pMinMaxChk->Min[m_mIndex];
			if (m_Max == INT_MIN || m_Min == INT_MAX)
			{
		//		DrawTickIndex(pDC, m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->name, m_tRGB);
				return;
			}
		}
	}

	if (m_Max == INT_MIN || m_Min == INT_MAX)
	{
	//	DrawTickIndex(pDC, m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->name, m_tRGB);
		return;
	}


	if (m_TickInfo & CDT_LEFT)	DrawTickHorizon(pDC, m_Max, m_Min, ctkLEFT);
	if (m_TickInfo & CDT_RIGHT)	DrawTickHorizon(pDC, m_Max, m_Min, ctkRIGHT);

	DrawChart(pDC);
}

void CGrp_Base::DrawName(CDC *pDC)
{
	DrawTickIndex(pDC, m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->name, m_tRGB);
}

bool CGrp_Base::Draw3DPole(CDC* pDC, CRect rc, int xGap, int yGap, COLORREF orgColor)
{
	COLORREF	color[3];
	color[0] = orgColor;
	Get3DRGB(color);

	CPen	*cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, color[0]);
	CPen	*sPen = pDC->SelectObject(cPen);
	CBrush	*cBrush = m_pApp->GetBrush(m_pView, color[0]);
	CBrush	*sBrush = pDC->SelectObject(cBrush);

	CPoint	pointPOLE[4];				// 정면
	pointPOLE[0].x = rc.left;
	pointPOLE[0].y = rc.top;
	pointPOLE[1].x = rc.left;
	pointPOLE[1].y = rc.bottom;
	pointPOLE[2].x = rc.right;
	pointPOLE[2].y = rc.bottom;
	pointPOLE[3].x = rc.right;
	pointPOLE[3].y = rc.top;
	pDC->Polygon(pointPOLE, sizeof(pointPOLE)/sizeof(CPoint));
	
	pDC->SelectObject(sBrush);
	pDC->SelectObject(sPen);

	cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, color[1]);
	sPen = pDC->SelectObject(cPen);
	cBrush = m_pApp->GetBrush(m_pView, color[1]);
	sBrush = pDC->SelectObject(cBrush);

	pointPOLE[0].x = rc.left;			// 상단
	pointPOLE[0].y = rc.top;
	pointPOLE[1].x = rc.right;
	pointPOLE[1].y = rc.top;
	pointPOLE[2].x = rc.right + xGap;
	pointPOLE[2].y = rc.top - yGap;
	pointPOLE[3].x = rc.left + xGap;
	pointPOLE[3].y = rc.top - yGap;
	pDC->Polygon(pointPOLE, sizeof(pointPOLE)/sizeof(CPoint));

	pDC->SelectObject(sBrush);
	pDC->SelectObject(sPen);

	cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, color[2]);
	sPen = pDC->SelectObject(cPen);
	cBrush = m_pApp->GetBrush(m_pView, color[2]);
	sBrush = pDC->SelectObject(cBrush);

	pointPOLE[0].x = rc.right;			// 우측
	pointPOLE[0].y = rc.top;
	pointPOLE[1].x = rc.right;
	pointPOLE[1].y = rc.bottom;
	pointPOLE[2].x = rc.right + xGap;
	pointPOLE[2].y = rc.bottom - yGap;
	pointPOLE[3].x = rc.right + xGap;
	pointPOLE[3].y = rc.top - yGap;
	pDC->Polygon(pointPOLE, sizeof(pointPOLE)/sizeof(CPoint));

	pDC->SelectObject(sBrush);
	pDC->SelectObject(sPen);
	return true;
}

bool CGrp_Base::Draw3DRevPole(CDC* pDC, CRect rc, int xGap, int yGap, COLORREF orgColor)
{
	COLORREF	color[3];
	color[0] = orgColor;
	Get3DRGB(color);

	CPen	*cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, color[0]);
	CPen	*sPen = pDC->SelectObject(cPen);
	CBrush	*cBrush = m_pApp->GetBrush(m_pView, color[0]);
	CBrush	*sBrush = pDC->SelectObject(cBrush);

	CPoint	pointPOLE[4];				// 정면
	pointPOLE[0].x = rc.left;
	pointPOLE[0].y = rc.top;
	pointPOLE[1].x = rc.left;
	pointPOLE[1].y = rc.bottom;
	pointPOLE[2].x = rc.right;
	pointPOLE[2].y = rc.bottom;
	pointPOLE[3].x = rc.right;
	pointPOLE[3].y = rc.top;
	pDC->Polygon(pointPOLE, sizeof(pointPOLE)/sizeof(CPoint));
	
	pDC->SelectObject(sBrush);
	pDC->SelectObject(sPen);

	cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, color[2]);
	sPen = pDC->SelectObject(cPen);
	cBrush = m_pApp->GetBrush(m_pView, color[2]);
	sBrush = pDC->SelectObject(cBrush);

	pointPOLE[0].x = rc.right;			// 우측
	pointPOLE[0].y = rc.top;
	pointPOLE[1].x = rc.right;
	pointPOLE[1].y = rc.bottom;
	pointPOLE[2].x = rc.right + xGap;
	pointPOLE[2].y = rc.bottom - yGap;
	pointPOLE[3].x = rc.right + xGap;
	pointPOLE[3].y = rc.top - yGap;
	pDC->Polygon(pointPOLE, sizeof(pointPOLE)/sizeof(CPoint));

	pDC->SelectObject(sBrush);
	pDC->SelectObject(sPen);

	color[0] = color[2];
	Get3DRGB(color);
	cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, color[2]);
	sPen = pDC->SelectObject(cPen);
	cBrush = m_pApp->GetBrush(m_pView, color[2]);
	sBrush = pDC->SelectObject(cBrush);

	pointPOLE[0].x = rc.left;			// 상단
	pointPOLE[0].y = rc.top;
	pointPOLE[1].x = rc.right;
	pointPOLE[1].y = rc.top;
	pointPOLE[2].x = rc.right + xGap;
	pointPOLE[2].y = rc.top - yGap;
	pointPOLE[3].x = rc.left + xGap;
	pointPOLE[3].y = rc.top - yGap;
	pDC->Polygon(pointPOLE, sizeof(pointPOLE)/sizeof(CPoint));

	pDC->SelectObject(sBrush);
	pDC->SelectObject(sPen);
	return true;
}

// 0:origin color, [1]:light color, [2]:dark color
void CGrp_Base::Get3DRGB(COLORREF color[3])
{
	const	int	rColor = GetRValue(color[0]);
	const	int	gColor = GetGValue(color[0]);
	const	int	bColor = GetBValue(color[0]);
	const	int	rRate  = rColor / 4;
	const	int	gRate  = gColor / 4;
	const	int	bRate  = bColor / 4;

	int	r = (rColor + rRate > 255)? 255:rColor + rRate;
	int	g = (gColor + gRate > 255)? 255:gColor + gRate;
	int	b = (bColor + bRate > 255)? 255:bColor + bRate;
	if (r == rColor && g == gColor && b == bColor)
	{
		r = (r + 10 > 255)? 255: r + 10;
		g = (g + 10 > 255)? 255: g + 10;
		b = (b + 10 > 255)? 255: b + 10;
	}
	color[1] = RGB(r, g, b);

	r = (rColor - rRate < 0)? 0:rColor - rRate;
	g = (gColor - gRate < 0)? 0:gColor - gRate;
	b = (bColor - bRate < 0)? 0:bColor - bRate;
	if (r == rColor && g == gColor && b == bColor)
	{
		r = (r - 10 < 0)? 0: r - 10;
		g = (g - 10 < 0)? 0: g - 10;
		b = (b - 10 < 0)? 0: b - 10;
	}

	color[2] = RGB(r, g, b);
}

struct _dispInfo {
	char	dispStr[10]{};
	CRect	dispRC;
	CPoint	dispPt;
};
enum { dispNONE, dispONEHOUR, dispTHREEHOUR, dispONEDAY, dispTHREEDAY, dispSIXDAY};
void CGrp_Base::DrawDateBottom(CDC *pDC)
{
	if (!m_pGrpWnd->m_pDataInfo)
		return;

	CRect	orgRC = m_pGrpWnd->m_GrpRect;//m_pRgnInfo[m_rKey]->gpRect;
	orgRC.right = m_pGrpWnd->m_ObjRect.right;
	CRect	tickRC = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].tkRect;
	tickRC.right = m_DrawRect.right;
	tickRC.left = m_DrawRect.left;

	const	int	tickWidth = tickRC.Width();
	const	int	tickHeight = tickRC.Height();
	const	int	sMode = pDC->SetBkMode(TRANSPARENT);
	const	COLORREF sColor = pDC->SetTextColor(m_pGrpWnd->m_TkTextColor);
	const	int	style = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].lstyle;
	const	int	width = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].lwidth;
	const	COLORREF color = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].lcolor;

	CFont	*boldFont = m_pApp->GetFont(m_pView, m_pGrpWnd->m_fPoint, m_pGrpWnd->m_fName, FW_BOLD);
	CFont	*sFont = pDC->SelectObject(boldFont);
	CPen	*cPen = m_pApp->GetPen(m_pView, style, width, color);
	CPen	*sPen = pDC->SelectObject(cPen);

	CArray <_dispInfo, _dispInfo> arDispInfo;
	_dispInfo	dispInfo;
	arDispInfo.RemoveAll();

	CPoint	pointDAY;
	CString	dispStr;
	CRect	drawRC;
	CRect	saveRC(0,0,0,0);
	int	svYear = 0, svMonth = 0, svDay = 0;
	int	svHH = 0;
	int	savePos = -20;
	int	gap = 0;
	pointDAY.y = tickRC.top + (int)(tickHeight / 5);

	const	int	dispPos = m_pGrpWnd->m_dispPos;
	const	int	dispEnd = m_pGrpWnd->m_dispEnd;
	const	int	dispDay = m_pGrpWnd->m_dispDay;
	const	int	endIdx = dispEnd - dispPos - 1;

	int	drawShift = 0;
	if (dispDay)	drawShift = (int)((tickWidth/dispDay) / 2);	// Center

	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		const auto gVal = m_pGrpWnd->m_pDataInfo->GetGraphData(ii+dispPos);
		pointDAY.x = tickRC.left + drawShift;
		pointDAY.x += (int)((tickWidth * ii) / dispDay);
		pointDAY.x -= 1;	// 그래프간 간격
		pointDAY.y = tickRC.top + (int)(tickHeight / 5);

		switch(m_dIndex)
		{
		case CDI_MON:			// 년도
		case CDI_WEEK:			// 3달 
			if (svYear == gVal->index.year.date.yy)
				continue;
			dispStr.Format("%04d", gVal->index.year.date.yy);
			svYear = gVal->index.year.date.yy;
			break;
		case CDI_MIN:	case CDI_TICK:			// 시간
			if (svHH == gVal->index.year.time.hh)
				continue;

			gap = max(pointDAY.x - savePos, gap);
			savePos = pointDAY.x;
			svHH = gVal->index.year.time.hh;

			if (ii != 0 && gVal->index.year.date.dd == svDay)
				continue;
			
			//if (ii == 0)
				dispStr.Format("%02d:%02d", gVal->index.year.time.hh,
					gVal->index.year.time.mm);
			//else
			//	dispStr.Format("%02d/%02d", 
			//		gVal->index.year.date.mm, gVal->index.year.date.dd);

			svDay = gVal->index.year.date.dd;

			break;
		case CDI_DAY:			// 달
		default:
			if (svYear == gVal->index.year.date.yy && svMonth == gVal->index.year.date.mm)
				continue;

			svMonth  = gVal->index.year.date.mm;
			gap = max(pointDAY.x - savePos, gap);
			savePos = pointDAY.x;

			if (svYear == gVal->index.year.date.yy)
				continue;

			dispStr.Format("%04d/%02d", gVal->index.year.date.yy, gVal->index.year.date.mm);
			svYear = gVal->index.year.date.yy;
			
			break;
		}

		drawRC = DisplayRect(pDC, ii, dispStr, pointDAY);
		drawRC.left -= 4;
		drawRC.right += 4;

		dispInfo.dispRC = drawRC;
		memset(&dispInfo.dispStr, 0x00, sizeof(dispInfo.dispStr));
		memcpy(&dispInfo.dispStr, dispStr, dispStr.GetLength());
		dispInfo.dispPt = pointDAY;
		arDispInfo.Add(dispInfo);
	}
		
	int view = dispONEDAY;
	if (m_dIndex == CDI_DAY)
	{
		if (gap < 13)
			view = dispNONE;
		else if (gap >= 13 && gap < 20)
			view = dispSIXDAY;
		else if (gap >= 20 && gap < 30)
			view = dispTHREEDAY;
	}
	else if (m_dIndex == CDI_MIN || m_dIndex == CDI_TICK)
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
	const	int	arSize = arDispInfo.GetSize();
	//for (ii = 0; ii < arSize; ii++)
	for (int ii = arSize-1; ii >= 0; ii--)	// 최근일자부터 뿌리도록 변경
	{
		bText = true;
		dispInfo = arDispInfo.GetAt(ii);
		// 최초값
		if ((m_dIndex != CDI_MIN && m_dIndex != CDI_TICK) && ii == 0 && arSize > 1)
		{
			cmpRC = arDispInfo.GetAt(1).dispRC;
			unionRC = cmpRC & dispInfo.dispRC;
			if (!unionRC.IsRectEmpty())
				bText = false;
		}

		if (dispInfo.dispPt.x > tickRC.left && dispInfo.dispPt.x < tickRC.right && m_pGrpWnd->m_bVDraw)
		{
			pDC->MoveTo(dispInfo.dispPt.x, orgRC.top);
			pDC->LineTo(dispInfo.dispPt.x, orgRC.bottom);
		}
		
		//if (m_dIndex == CDI_MIN || m_dIndex == CDI_TICK)
		{
			unionRC = saveRC & dispInfo.dispRC;
			if (!unionRC.IsRectEmpty())
				continue;
			saveRC = dispInfo.dispRC;
		}
		pDC->MoveTo(dispInfo.dispPt.x, m_DrawRect.bottom);
		pDC->LineTo(dispInfo.dispPt.x, m_DrawRect.bottom+6);
		
		if (bText)
		{
			if (m_dIndex == CDI_MIN || m_dIndex == CDI_TICK)
			{
				if (view != dispNONE && ii != 0)
					pDC->SelectObject(&boldFont);
				else
					pDC->SelectObject(m_pGrpWnd->m_pFont);
				
			}
			if (dispInfo.dispRC.right < orgRC.right)
				pDC->DrawText(dispInfo.dispStr, &dispInfo.dispRC, DT_SINGLELINE|DT_TOP|DT_CENTER);
		}
	}

	pDC->SelectObject(m_pGrpWnd->m_pFont);
	if (view > 0)
	{
		int jj = 0;
		bool bDraw = true;
		saveRC.SetRectEmpty();
		
		for (int ii = 0; ii <= endIdx; ii++)
		{
			const auto gVal = m_pGrpWnd->m_pDataInfo->GetGraphData(ii+dispPos);
			
			switch (m_dIndex)
			{
			case CDI_DAY:
				if (svYear == gVal->index.year.date.yy 
					&& svMonth == gVal->index.year.date.mm)
					continue;
				switch (view)
				{
				case dispTHREEDAY:
					if (gVal->index.year.date.mm % 3 != 1)
						continue;
					break;
				case dispSIXDAY:
					if (gVal->index.year.date.mm % 6 != 1)
						continue;
					break;
				}
				dispStr.Format("%02d", gVal->index.year.date.mm);
				svYear = gVal->index.year.date.yy;
				svMonth  = gVal->index.year.date.mm;
				break;
			case CDI_MIN:	case CDI_TICK:
				if (svHH == gVal->index.year.time.hh && ii != endIdx)
					continue;
				switch (view)
				{
				case dispTHREEHOUR:
					if (ii != endIdx && gVal->index.year.time.hh % 12 != 0)
					{
						svHH = gVal->index.year.time.hh;
						continue;
					}
					break;
				}
				
				if (ii != endIdx)
					dispStr.Format("%02d", gVal->index.year.time.hh);
				else
					dispStr.Format("%02d:%02d", 
						gVal->index.year.time.hh, gVal->index.year.time.mm);

				svHH = gVal->index.year.time.hh;
				break;
			}

			// 그래프간 간격
			pointDAY.x = tickRC.left + drawShift;
			pointDAY.x += (int)((tickWidth * ii) / dispDay);
			pointDAY.x -= 1;
			pointDAY.y = tickRC.top + (int)(tickHeight / 5);

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
				const	int	Pixgap = drawRC.right - m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkRIGHT].tkRect.right;
				if (Pixgap > 0)	drawRC.OffsetRect(-Pixgap, 0);

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
				if (m_pGrpWnd->m_bVDraw)
				{
					pDC->MoveTo(pointDAY.x, orgRC.top);
					pDC->LineTo(pointDAY.x, orgRC.bottom);
				}

				pDC->MoveTo(pointDAY.x, orgRC.bottom);
				pDC->LineTo(pointDAY.x, orgRC.bottom+3);

				if (bDraw)
				{
					pDC->DrawText(dispStr, &drawRC, DT_SINGLELINE|DT_TOP|DT_CENTER);
					saveRC = drawRC;
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

CRect CGrp_Base::DisplayRect(CDC *pDC, int idx, CString dispStr, CPoint dispPt)
{
	CRect	dispRC = CRect(0, 0, 0, 0);
	const	CSize	size = pDC->GetOutputTextExtent(dispStr);

	if (idx == 0)
		dispRC.SetRect(dispPt.x, dispPt.y+2, 
				dispPt.x + size.cx, dispPt.y + size.cy);
	else
		dispRC.SetRect(dispPt.x - size.cx/2, dispPt.y+2, 
				dispPt.x + size.cx/2+1, dispPt.y + size.cy+2);

	return dispRC;
}

int CGrp_Base::DrawVerticalString(CDC* pDC, CString dispStr, CRect drawRC, bool bDraw)
{
	CPoint	pt = drawRC.TopLeft();
	int	nSum = 0;
	CString tmpstr;
	const	CSize	size = pDC->GetTextExtent(dispStr);
	const	int	nCount = dispStr.GetLength();
	char	ch = 0;
	bool	bSpace = false;

	if (nCount == 0)
		return 0;

	for (int ii = 0 ; ii < nCount ; ii++)
	{
		tmpstr = dispStr.Mid(ii, 2);
		ch = tmpstr.GetAt(0);
		
		if (ch >= 0 && ch <= 128)
		{			
			tmpstr = tmpstr.Left(1);
			if (ch == ' ')
				bSpace = true;
		}
		else
			ii++;

		drawRC.top = pt.y;
		drawRC.bottom = drawRC.top + size.cy;

		if (bDraw)
			pDC->DrawText(tmpstr, drawRC, DT_SINGLELINE|DT_VCENTER|DT_CENTER);

		if (bSpace)
			nSum = pt.y += size.cy / 2;
		else
			nSum = pt.y += size.cy;	

		bSpace = false;
	}

	return nSum + size.cy;
}

CString CGrp_Base::GetDisplayPosHead(CPoint pt)
{
	CString	dispStr = _T("");

	if (m_DrawRect.left > pt.x || m_DrawRect.right <= pt.x)
		return dispStr;

	if (!m_pGrpWnd->m_pDataInfo || m_pGrpWnd->m_pDataInfo->GetDataCount() <= 0)
		return dispStr;

	const	int	dispPos = m_pGrpWnd->m_dispPos;
	const	int	dispDay = m_pGrpWnd->m_dispDay;
	const	int	xPosition = pt.x - m_DrawRect.left;
	const	double	szOneDay = (double)m_DrawRect.Width() / (double)dispDay;
	int	dataPos = (int)((double)xPosition / szOneDay);
	dataPos += dispPos;
	
	const auto gVal = m_pGrpWnd->m_pDataInfo->GetGraphData(dataPos);
	if (m_pGrpWnd->m_bStringIndex)
	{
		dispStr.Format("%s\t%s", m_pGrpWnd->m_sTipName, gVal->index.name.str);
	}
	else
	{
		
		if (m_dIndex == CDI_MIN || m_dIndex == CDI_TICK)
			dispStr.Format("시간\t%02d:%02d:%02d", gVal->index.year.time.hh, 
					gVal->index.year.time.mm, gVal->index.year.time.ss);
		else
			dispStr.Format("날짜\t%04d/%02d/%02d", gVal->index.year.date.yy, 
				gVal->index.year.date.mm, gVal->index.year.date.dd);
	}

	return dispStr;
}

CString CGrp_Base::GetDisplayPosData(CPoint pt)
{
	CString	dispStr = _T("");

	if (m_DrawRect.left > pt.x || m_DrawRect.right <= pt.x)
		return dispStr;

	if (!m_pGrpWnd->m_pDataInfo || m_pGrpWnd->m_pDataInfo->GetDataCount() <= 0)
		return dispStr;

	const	int	dispPos = m_pGrpWnd->m_dispPos;
	const	int	dispDay = m_pGrpWnd->m_dispDay;
	const	int	xPosition = pt.x - m_DrawRect.left;
	const	double	szOneDay = (double)m_DrawRect.Width() / (double)dispDay;
	int	dataPos = (int)((double)xPosition / szOneDay);
	dataPos += dispPos;

	CString	stmp, stmp2, sSign, sSign2;
	const auto gVal = m_pGrpWnd->m_pDataInfo->GetGraphData(dataPos);

	FormatVariableComma(stmp, gVal->vData[m_dKey], m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->digit);
	dispStr.Format("%s\t%s", m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->name, stmp);			
	return dispStr;
}

CString CGrp_Base::GetExcelHead(int idx)
{
	CString	dispStr = _T("");
	const auto gVal = m_pGrpWnd->m_pDataInfo->GetGraphData(idx);

	if (idx < 0)
	{
		if (m_pGrpWnd->m_bStringIndex)
		{
			dispStr = m_pGrpWnd->m_sTipName;
		}
		else
		{
			if (m_dIndex == CDI_MIN || m_dIndex == CDI_TICK)
				dispStr = "시간";
			else
				dispStr = "일자";
		}
	}
	else
	{
		if (m_pGrpWnd->m_bStringIndex)
		{
			dispStr.Format("%s", gVal->index.name.str);
		}
		else
		{
			if (m_dIndex == CDI_MIN || m_dIndex == CDI_TICK)
				dispStr.Format("%02d:%02d:%02d", gVal->index.year.time.hh, 
				gVal->index.year.time.mm, gVal->index.year.time.ss);
			else
				dispStr.Format("%04d/%02d/%02d", gVal->index.year.date.yy, 
				gVal->index.year.date.mm, gVal->index.year.date.dd);
		}
	}

	return dispStr;
}

CString CGrp_Base::GetExcelData(int idx)
{
	CString	dispStr = __T("");

	if (!m_pGrpWnd->m_pDataInfo || m_pGrpWnd->m_pDataInfo->GetDataCount() <= 0)
		return dispStr;

	if (idx < 0)
	{
		dispStr = m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->name;
	}
	else
	{
		const auto gVal = m_pGrpWnd->m_pDataInfo->GetGraphData(idx);
		FormatVariableComma(dispStr, gVal->vData[m_dKey], m_pGrpWnd->m_pDataInfo->m_pInfo[m_dKey]->digit);
	}

	return dispStr;
}