#include "stdafx.h"
#include "C_MultiLine.h"
#include "Grp_Base.h"
#include "Grp_Data.h"
#include "GrpWnd.h"

#include <FLOAT.H>
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define	gapTICK	2

CGrp_Base::CGrp_Base(CWnd* pView, class CGrpWnd *pGrpWnd, char *pInfo)
{
	m_pView = pView;
	m_pGrpWnd = pGrpWnd;

	struct _gInfo *pGInfo = (struct _gInfo*)pInfo;

	m_gKind = pGInfo->gKind;
	m_rKey = pGInfo->rKey;
	m_dKey = pGInfo->dKey;
	m_dKind = pGInfo->dKind;
	m_dIndex = pGInfo->dIndex;

	m_tRGB = pGInfo->tRGB;
	m_pRGB = pGInfo->pRGB;

	m_DrawRect = m_pGrpWnd->m_pRgnInfo[m_rKey]->gpRect;

	m_Max = INT_MIN;
	m_Min = INT_MAX;
	m_pApp = (CC_MultiLineApp*)AfxGetApp();
}

CGrp_Base::~CGrp_Base()
{

}

void CGrp_Base::Resize()
{
	m_RgnRect = m_DrawRect = m_pGrpWnd->m_pRgnInfo[m_rKey]->gpRect;
	m_DrawRect.top += m_pGrpWnd->m_pRgnInfo[m_rKey]->tkHeight + 5;

	if (m_DrawRect.Width() > 50)
	{
		m_DrawRect.right = m_DrawRect.right - 5;
	}
	
	CFont	*oldfont = NULL;
	CDC*	pDC = m_pGrpWnd->GetDC();
	CSize	size;
	oldfont = pDC->SelectObject(m_pGrpWnd->m_pFont);
	size = pDC->GetTextExtent("굳");
	pDC->SelectObject(oldfont);
	m_pGrpWnd->ReleaseDC(pDC);
	m_DrawRect.bottom -= size.cy / 2;
}

CString CGrp_Base::GetDisplayPosHead(CPoint pt)
{
	CString	dispStr = _T("");

	if (m_DrawRect.left > pt.x || m_DrawRect.right <= pt.x)
		return dispStr;

	if (!m_pGrpWnd->m_pDataInfo || m_pGrpWnd->m_pDataInfo->GetDataCount() <= 0)
		return dispStr;

	int	dispPos = m_pGrpWnd->m_dispPos;
	int	dispEnd = m_pGrpWnd->m_dispEnd;
	int	dispDay = m_pGrpWnd->m_dispDay;

	int	xPosition = pt.x - m_DrawRect.left;
	double	szOneDay = double(m_DrawRect.Width()) / double(dispDay);
	int	dataPos = int(double(xPosition) / szOneDay);
	dataPos += dispPos;

	CString	stmp, stmp2, sSign, sSign2;
	struct _multi_line	*mline = 
		 (struct _multi_line *)m_pGrpWnd->m_pDataInfo->GetGraphData(dataPos);
	if (m_dIndex == CDI_MIN || m_dIndex == CDI_TICK)
		dispStr.Format("시간\t%02d:%02d:%02d", mline->index.time.hh, 
		mline->index.time.mm, mline->index.time.ss);
	else
		dispStr.Format("일자\t%04d/%02d/%02d", mline->index.date.yy, 
		mline->index.date.mm, mline->index.date.dd);

	return dispStr;
}

CString CGrp_Base::GetExcelHead(int idx)
{
	CString	dispStr = _T("");
	CString	stmp;
	struct _multi_line	*multi_line = (struct _multi_line*)m_pGrpWnd->m_pDataInfo->GetGraphData(idx);
	if (idx < 0)
	{
		if (m_dIndex == CDI_MIN || m_dIndex == CDI_TICK)
			dispStr = "시간";
		else
			dispStr = "일자";
	}
	else
	{
		if (m_dIndex == CDI_MIN || m_dIndex == CDI_TICK)
			dispStr.Format("%02d:%02d:%02d", multi_line->index.time.hh, 
			multi_line->index.time.mm, multi_line->index.time.ss);
		else
			dispStr.Format("%04d/%02d/%02d", multi_line->index.date.yy, 
			multi_line->index.date.mm, multi_line->index.date.dd);
	}

	return dispStr;
}

void CGrp_Base::DrawTickHorizon(CDC* pDC, double Max, double Min, int tickinfo)
{
	int	dispCount;
	double	dispValue[64];
	CRect	tickRC = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[tickinfo].tkRect;
	CRect	bottomRC = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].tkRect;
	tickRC.top = m_DrawRect.top;
	tickRC.bottom = m_DrawRect.bottom;

	int	tickHeight = tickRC.Height();
	double	ValueHeight = Max - Min;
	if (ValueHeight <= 0)	ValueHeight = 1;

	double	div;
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
	else if (ValueHeight < 2000000)		div = 200000;
	else if (ValueHeight < 5000000)		div = 500000;
	else if (ValueHeight < 10000000)	div = 1000000;
	else if (ValueHeight < 20000000)	div = 2000000;
	else if (ValueHeight < 50000000)	div = 5000000;
	else if (ValueHeight < 100000000)	div = 10000000;
	else if (ValueHeight < 200000000)	div = 20000000;
	else if (ValueHeight < 500000000)	div = 50000000;
	else if (ValueHeight < 1000000000)	div = 100000000;
	else					div = 500000000;

	int dispTICK = GetTickCount(tickinfo);
	if (dispTICK <= 0)
		return;

	double	drawMinValue= 0.0;

	if (!calculateDisplay(div, Max, Min, drawMinValue, dispTICK, dispValue, dispCount))
	{
		return;
	}

	int		sMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF	sColor = pDC->SetTextColor(m_pGrpWnd->m_ltColor);
	int		style = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[tickinfo].lstyle;
	int		width = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[tickinfo].lwidth;
	COLORREF	color = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[tickinfo].lcolor;

	CPen	*cPen, *sPen;
	CRect	textRC;
	CFont	*sFont = pDC->SelectObject(m_pGrpWnd->m_pFont);
	cPen = m_pApp->GetPen(m_pView, style, width, color);
	sPen = pDC->SelectObject(cPen);

	double	dValue;
	CString	dispStr;
	CPoint	pointS, pointE;
	CRect	tkSaveRC = CRect(10000,10000,10000,10000);

	CSize	size;
	int	maxdigit = 7;
	char	*digitStr[10] = {"2,000,000,000", "200,000,000", "20,000,000", 
		"2,000,000", "200,000", "20,000", "2,000", "200", "20", "2"};
	for (int ii = 9; ii >= 0; ii--)
	{
		size = pDC->GetOutputTextExtent(digitStr[ii]);
		if (size.cx >= tickRC.Width())
			break;
		else
			maxdigit = strlen(digitStr[ii]);
	}

	CString	sUnit = _T("");
	int	nMaxLen = 0;

	if (m_Max > VOLUNIT)
	{
		sUnit = "x1000";
		size = pDC->GetOutputTextExtent(sUnit);
		nMaxLen = size.cx;
	}

	for (int ii = 0 ; ii < dispCount ; ii++ )
	{
		FormatVariableComma(dispStr, dispValue[ii], m_pGrpWnd->m_pRgnInfo[m_rKey]->digit, maxdigit);
		size = pDC->GetOutputTextExtent(dispStr);

		if (nMaxLen < size.cx)
			nMaxLen = size.cx;
	}

	size = pDC->GetOutputTextExtent(" ");
	int	nGap = size.cx;

	for (int ii = 0; ii < dispCount; ii++)		// 가격별가로 (DOT)
	{
		pointS.x = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkLEFT].tkRect.right+1;
		dValue = double(double(tickHeight) * double(Max - dispValue[ii]));
		dValue /= double(ValueHeight);

		pointS.y = tickRC.top + int(dValue);

		if (!tickRC.PtInRect(CPoint(pointS.x -1, pointS.y)))
		{
			if (tickRC.bottom < pointS.y)
				continue;
		}

		pointE.x = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkRIGHT].tkRect.left;
		pointE.y = pointS.y;
		if (pointS.y >= m_DrawRect.top && pointS.y <= m_DrawRect.bottom)
		{
			if (ii != 0 || m_Max <= VOLUNIT)
			{
				pDC->MoveTo(pointS);
				pDC->LineTo(pointE);
			}
		}

		FormatVariableComma(dispStr, dispValue[ii], m_pGrpWnd->m_pRgnInfo[m_rKey]->digit, maxdigit);
		if (ii == 0 && !sUnit.IsEmpty())
			dispStr = sUnit;

		size = pDC->GetOutputTextExtent(dispStr);

		textRC.SetRect(tickRC.left, pointS.y - size.cy/2,
			tickRC.right - gapTICK, pointS.y + size.cy/2);

		if (textRC.bottom > tkSaveRC.top)
		{
			textRC.OffsetRect(0, tkSaveRC.top - textRC.bottom);
			tkSaveRC.top += size.cy + 1;
		}

		if (tickinfo == ctkLEFT)
			textRC.right -= nGap;
		else
		{
			textRC.left += nGap;
			textRC.right = textRC.left + nMaxLen;
		}

		pDC->DrawText(dispStr, textRC, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
		tkSaveRC = textRC;
	}
	
	textRC.SetRect(tickRC.left, bottomRC.top, tickRC.right, bottomRC.bottom);
	int	nDegree = m_pGrpWnd->m_arDegree.GetSize();
	CString sDegree;
	if (m_rKey < nDegree)
	{
		sDegree = m_pGrpWnd->m_arDegree.GetAt(m_rKey);

		if (!sDegree.IsEmpty())
		{
			pDC->DrawText(sDegree, textRC, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
		}
	}

	pDC->SelectObject(sFont);
	pDC->SelectObject(sPen);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
}

int CGrp_Base::DrawTickIndex(CDC* pDC, CString dispText, COLORREF tColor, bool bColor, bool bIndex, CPoint pt)
{
	if (dispText.IsEmpty())
		return 0;

	int sMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF sColor;
	if (bColor)	sColor = pDC->SetTextColor(tColor);
	else		sColor = pDC->SetTextColor(m_tRGB);
	CFont *sFont = pDC->SelectObject(m_pGrpWnd->m_pFont);

	CString	realSizeStr = dispText;
	CSize	size = pDC->GetOutputTextExtent(realSizeStr);
	CSize	size2 = pDC->GetOutputTextExtent("D");
	size.cx = size.cx + size2.cx/2;
	size2.cx = size2.cx + size2.cx/2;

	if (bIndex)
	{
		if (pt.x < m_DrawRect.left)
			pt.x = m_DrawRect.left;
		else if ((pt.x + size.cx) > m_DrawRect.right)
			pt.x = m_DrawRect.right - size.cx;
		if (pt.y < m_DrawRect.top)
			pt.y = m_DrawRect.top;
		else if ((pt.y + size.cy) > m_DrawRect.bottom)
			pt.y = m_DrawRect.bottom - size.cy;

		CRect	dispRC;
		dispRC.SetRect(pt.x, pt.y, pt.x + size.cx, pt.y + size.cy);
		pDC->DrawText(dispText, &dispRC, DT_SINGLELINE|DT_TOP|DT_LEFT);

		pDC->SelectObject(sFont);
		pDC->SetTextColor(sColor);
		pDC->SetBkMode(sMode);

		return pt.x + size.cx;
	}

	struct _MinMaxChk	*pMinMaxChk = m_pGrpWnd->GetMinMaxChk(m_rKey);
	if (pMinMaxChk->tickx == 0)
	{
		pt.x = m_DrawRect.left + 5;
		pt.y = m_DrawRect.top + 3;
	}
	else
	{
		if (pMinMaxChk->tickx + size.cx > m_DrawRect.right)
		{
			pt.x = m_DrawRect.left + 5;
			pt.y = pMinMaxChk->ticky + size.cy;
		}
		else
		{
			pt.x = pMinMaxChk->tickx;
			pt.y = pMinMaxChk->ticky;
		}
	}

	if (pt.y + size.cy > m_DrawRect.bottom)
	{
		pDC->SelectObject(sFont);
		pDC->SetTextColor(sColor);
		pDC->SetBkMode(sMode);

		return 0;
	}

	CRect	dispRC;
	CRect	legendRC;
	dispRC.SetRect(pt.x, pt.y, pt.x + size.cx, pt.y + size.cy);
	dispRC.NormalizeRect();
	pDC->DrawText(dispText, &dispRC, DT_SINGLELINE|DT_TOP|DT_LEFT);
	legendRC = dispRC;

	pDC->SelectObject(sFont);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);

	pMinMaxChk->tickx = pt.x + size.cx;
	pMinMaxChk->ticky = pt.y;

	return pt.x + size.cx;
}

void CGrp_Base::FormatVariableComma(CString &dispTxt, double dVal, int point, int maxDigit)
{
	CString	edit1, edit2;
	double dotL = 0, dotR = 0;

	bool bSign = false;
	if (dVal < 0)
	{
		bSign = true;
		dVal = dVal * (-1);
	}

	if (point > 0)
	{
		double	div = pow(10.0, point);
		dotL = int(dVal / div);
		dotR = int(int(dVal) % (int)div);
	}
	else
		dotL = dVal;
	
	if (m_Max > VOLUNIT)
		dotL /= 1000.;

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
		edit1.Format(".%0*d", point, int(dotR+0.5));
	else
		edit1 = _T("");
		
	int trimComma = edit2.GetLength() + edit1.GetLength() - maxDigit;
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
	int	tkHeight;

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

	dispTICK = int(tkHeight / (m_pGrpWnd->m_pRgnInfo[m_rKey]->tkHeight * 1.5));

	if (dispTICK > 10)
		dispTICK = 10;

	return dispTICK;
}

bool CGrp_Base::calculateDisplay(double div, double Max, double Min, double& drawMinValue, 
		int dispTICK, double* dispValue, int& dispCount)
{
	int	index;
	CString	temp;
	double	mok = Min / div;
	temp.Format("%f", mok);
	index = temp.Find('.'); 
	mok = atof(temp.Left(index));
	drawMinValue = mok * div;
	if (drawMinValue < Min)		
		drawMinValue += div;
	int	drawNum = int ((Max - drawMinValue) / div);
	if (drawNum >= dispTICK)
	{
		if (drawNum <= dispTICK*2)
			div *= 2;
		else if (drawNum <= dispTICK*5)
			div *= 5;
		else
			div *= 10;

		mok = Min / div;
		temp.Format("%f", mok);
		index = temp.Find('.'); 
		mok = atof(temp.Left(index));
		drawMinValue = mok * div;
		if (drawMinValue < Min)		
			drawMinValue += div;
	}

	dispCount = 0;
	if (Max > VOLUNIT)
	{
		dispValue[dispCount] = Min;
		dispCount++;
	}

	if (drawMinValue > Max)
	{
		mok = (Max + Min) / 2.0;
		temp.Format("%f", mok);
		index = temp.Find('.'); 
		temp = temp.Left(index);
		mok = atof(temp.Left(index));
		index = temp.GetLength();

		if (index > 1)
		{
			double	value = pow(10.0, index - 1);
			temp = temp.Left(1);
			mok = atof(temp);
			mok = mok * value;
		}

		drawMinValue = mok;
		dispValue[dispCount] = drawMinValue;
		dispCount = 1;
		return true;
	}

	dispValue[dispCount] = drawMinValue;
	double dVal = 0.0;

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

void CGrp_Base::GetMinMaxValue(double& dMin, double& dMax)
{
	dMin = m_Min;
	dMax = m_Max;
}

struct _dispInfo {
	char	dispStr[10];
	CRect	dispRC;
	CPoint	dispPt;
};
enum { dispNONE, dispONEHOUR, dispTHREEHOUR, dispONEDAY, dispTHREEDAY, dispSIXDAY};
void CGrp_Base::DrawTickBottom(CDC *pDC)
{
	if (!m_pGrpWnd->m_pDataInfo)
		return;

	CRect	orgRC = m_pGrpWnd->m_pRgnInfo[m_rKey]->gpRect;
	orgRC.right = m_pGrpWnd->m_ObjRect.right;
	CRect	tickRC = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].tkRect;
	CRect	bottomRC = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].tkRect;
	tickRC.right = m_DrawRect.right;
	tickRC.left = m_DrawRect.left;
	int tickWidth = tickRC.Width();
	int tickHeight = tickRC.Height();

	int		sMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF	sColor = pDC->SetTextColor(m_pGrpWnd->m_btColor);
	int		style = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].lstyle;
	int		width = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].lwidth;
	COLORREF	color = m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkBOTTOM].lcolor;

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
	int	svHH = 0, svMM = 0;
	int	savePos = -20;
	int	gap = 0;
	pointDAY.y = tickRC.top + int(tickHeight / 5);

	int	dispPos = m_pGrpWnd->m_dispPos;
	int	dispEnd = m_pGrpWnd->m_dispEnd;
	int	dispDay = m_pGrpWnd->m_dispDay;
	int	endIdx = dispEnd - dispPos - 1;
	struct _cgBong	*cgBong;

	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		cgBong = (struct _cgBong *)m_pGrpWnd->m_pDataInfo->GetGraphData(ii+dispPos);
		pointDAY.x = tickRC.left;
		pointDAY.x += int((tickWidth * ii) / dispDay);
		pointDAY.x -= 1;	// 그래프간 간격
		pointDAY.y = tickRC.top + int(tickHeight / 5);

		switch(m_dIndex)
		{
		case CDI_MON:			// 년도
		case CDI_WEEK:			// 3달 
			if (svYear == cgBong->index.date.yy)
				continue;
			dispStr.Format("%04d", cgBong->index.date.yy);
			svYear = cgBong->index.date.yy;
			break;
		case CDI_MIN:	case CDI_TICK:			// 시간
			if (svHH == cgBong->index.time.hh)
				continue;

			gap = max(pointDAY.x - savePos, gap);
			savePos = pointDAY.x;
			svHH = cgBong->index.time.hh;

			if (ii != 0 && cgBong->index.date.dd == svDay)
				continue;
			
			if (ii == 0)
				dispStr.Format("%02d:%02d", cgBong->index.time.hh,
					cgBong->index.time.mm);
			else
				dispStr.Format("%02d/%02d", 
					cgBong->index.date.mm, cgBong->index.date.dd);

			svDay = cgBong->index.date.dd;

			break;
		case CDI_DAY:			// 달
		default:
			if (svYear == cgBong->index.date.yy && svMonth == cgBong->index.date.mm)
				continue;

			svMonth  = cgBong->index.date.mm;
			gap = max(pointDAY.x - savePos, gap);
			savePos = pointDAY.x;

			if (svYear == cgBong->index.date.yy)
				continue;

			dispStr.Format("%04d/%02d", cgBong->index.date.yy, cgBong->index.date.mm);
			svYear = cgBong->index.date.yy;
			
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
	int arSize = arDispInfo.GetSize();
	for (int ii = 0; ii < arSize; ii++)
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

		if (dispInfo.dispPt.x > tickRC.left && dispInfo.dispPt.x < tickRC.right)
		{
			pDC->MoveTo(dispInfo.dispPt.x, orgRC.top);
			pDC->LineTo(dispInfo.dispPt.x, orgRC.bottom);
		}
		
		if (m_dIndex == CDI_MIN || m_dIndex == CDI_TICK)
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
			cgBong = (struct _cgBong *)m_pGrpWnd->m_pDataInfo->GetGraphData(ii+dispPos);
			
			switch (m_dIndex)
			{
			case CDI_DAY:
				if (svYear == cgBong->index.date.yy 
					&& svMonth == cgBong->index.date.mm)
					continue;
				switch (view)
				{
				case dispTHREEDAY:
					if (cgBong->index.date.mm % 3 != 1)
						continue;
					break;
				case dispSIXDAY:
					if (cgBong->index.date.mm % 6 != 1)
						continue;
					break;
				}
				dispStr.Format("%02d", cgBong->index.date.mm);
				svYear = cgBong->index.date.yy;
				svMonth  = cgBong->index.date.mm;
				break;
			case CDI_MIN:	case CDI_TICK:
				if (svHH == cgBong->index.time.hh && ii != endIdx)
					continue;
				switch (view)
				{
				case dispTHREEHOUR:
					if (ii != endIdx && cgBong->index.time.hh % 12 != 0)
					{
						svHH = cgBong->index.time.hh;
						continue;
					}
					break;
				}
				
				if (ii != endIdx)
					dispStr.Format("%02d", cgBong->index.time.hh);
				else
					dispStr.Format("%02d:%02d", 
						cgBong->index.time.hh, cgBong->index.time.mm);

				svHH = cgBong->index.time.hh;
				break;
			}

			// 그래프간 간격
			pointDAY.x = tickRC.left;
			pointDAY.x += int((tickWidth * ii) / dispDay);
			pointDAY.x -= 1;
			pointDAY.y = tickRC.top + int(tickHeight / 5);

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
				int Pixgap = drawRC.right - m_pGrpWnd->m_pRgnInfo[m_rKey]->tick[ctkRIGHT].tkRect.right;
				if (Pixgap > 0)	drawRC.OffsetRect(Pixgap*(-1), 0);
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
				pDC->MoveTo(pointDAY.x, orgRC.top);
				pDC->LineTo(pointDAY.x, orgRC.bottom);
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
	CRect dispRC = CRect(0, 0, 0, 0);
	CSize size = pDC->GetOutputTextExtent(dispStr);
	if (idx == 0)
		dispRC.SetRect(dispPt.x, dispPt.y+2, 
				dispPt.x + size.cx, dispPt.y + size.cy);
	else
		dispRC.SetRect(dispPt.x - size.cx/2, dispPt.y+2, 
				dispPt.x + size.cx/2+1, dispPt.y + size.cy+2);

	return dispRC;
}