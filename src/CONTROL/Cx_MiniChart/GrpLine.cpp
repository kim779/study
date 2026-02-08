// GrpLine.cpp: implementation of the CGrpLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Cx_MiniChart.h"
#include "GrpLine.h"
#include "GrpData.h"
#include "GrpWnd.h"
#include "MainWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define	gapTICK	2

CGrpLine::CGrpLine(CWnd *pView, CWnd* pMainWnd, class CGrpWnd *pGrpWnd, int order)
{
	m_pView = pView;
	m_pGrpWnd = pGrpWnd;
	m_pMainWnd = pMainWnd;

	m_Order = order;
	m_DrawRect = m_pGrpWnd->m_GrpRect;

	m_Max = INT_MIN;
	m_Min = INT_MAX;

	m_pApp = (CCx_MiniChartApp *)AfxGetApp();

	CalculateMinMax();
}

CGrpLine::~CGrpLine()
{

}

void CGrpLine::DrawGraph(CDC *pDC)
{
	m_Max = m_pGrpWnd->m_Max;
	m_Min = m_pGrpWnd->m_Min;

	if (m_Max == INT_MIN || m_Min == INT_MAX)
		return;

	DrawLine(pDC);

	DrawTickIndex(pDC, m_pGrpWnd->m_pGrpData->m_pName[m_Order], 
		m_pGrpWnd->m_mj.jisu[m_Order].color, true, false, false);
}

void CGrpLine::DrawLine(CDC *pDC)
{
	const int	dispPos = 0;
	const int	dispEnd = m_pGrpWnd->m_pGrpData->GetDataCount();
	const int	dispDay = m_pGrpWnd->m_pGrpData->GetDataCount();

	if (dispDay < 1)
		return;

	CPen	*cPen = ((CMainWnd*)m_pMainWnd)->GetPen(m_pGrpWnd->m_mj.jisu[m_Order].style, 
		m_pGrpWnd->m_mj.jisu[m_Order].width, m_pGrpWnd->m_mj.jisu[m_Order].color);
	CPen	*sPen = pDC->SelectObject(cPen);

	const int DrawHeight = m_DrawRect.Height();
	const int DrawWidth  = m_DrawRect.Width();

	double ValueHeight = m_Max - m_Min;
	if (!ValueHeight) ValueHeight = 1;
	const int drawShift = int((DrawWidth/dispDay) / 2);	// Center

	CPoint	pt;
	double epr{};
		
	struct _cgMJ	*gMJ;
	gMJ = (struct _cgMJ *)m_pGrpWnd->m_pGrpData->GetGraphData(dispPos);
	pt.x = m_DrawRect.left + drawShift;
	pt.y = m_DrawRect.top;
	pt.y += int((DrawHeight * (m_Max - double(gMJ->ratio[m_Order]))) / ValueHeight);

	bool	bFirst = true;
	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		gMJ = (struct _cgMJ *)m_pGrpWnd->m_pGrpData->GetGraphData(ii+dispPos);
		epr = gMJ->ratio[m_Order];

		if (gMJ->val[m_Order] <= 0 || epr < m_Min || epr > m_Max)
			continue;

		pt.x = m_DrawRect.left + drawShift;
		pt.x += int((DrawWidth * ii) / dispDay);
		pt.y = m_DrawRect.top;
		pt.y += int((DrawHeight * (m_Max - double(epr))) / ValueHeight);

		if (bFirst)
		{
			bFirst = false;
			pDC->MoveTo(pt);
		}
		else
		{
			pDC->LineTo(pt);
		}
	}

	pDC->SelectObject(sPen);
}

bool CGrpLine::CalculateMinMax()
{
	m_Max = INT_MIN;
	m_Min = INT_MAX;

	if (!m_pGrpWnd->m_pGrpData || 
		m_pGrpWnd->m_pGrpData->GetDataCount() <= 0)
		return false;

	const int	dispPos = 0;
	const int	dispEnd = m_pGrpWnd->m_pGrpData->GetDataCount();
	const int	dispDay = m_pGrpWnd->m_pGrpData->GetDataCount();

	struct _cgMJ* cgMJ{};
	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		cgMJ = (struct _cgMJ *)m_pGrpWnd->m_pGrpData->GetGraphData(ii+dispPos);
		if (cgMJ->val[m_Order] <= 0)	continue;

		if (m_Max < cgMJ->ratio[m_Order])	m_Max = cgMJ->ratio[m_Order];
		if (m_Min > cgMJ->ratio[m_Order])	m_Min = cgMJ->ratio[m_Order];
	}

	if (m_Max == INT_MIN || m_Min == INT_MAX)
		return false;

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
	else
	{
		m_Min -= gap / 10;
	}

	return true;
}

bool CGrpLine::IsChangeMinMax(bool bShift)
{
	const double	m_sMax = m_Max;
	const double	m_sMin = m_Min;

	if (!CalculateMinMax())
		return false;

	if (m_sMax != m_Max || m_sMin != m_Min)
		return true;

	return false;
}

void CGrpLine::Resize()
{
	m_DrawRect = m_pGrpWnd->m_GrpRect;
	m_DrawRect.top += 15;

	if (m_DrawRect.Width() > 50)
	{
		m_DrawRect.left += 1;
		m_DrawRect.right = m_DrawRect.right - 10;
	}
}

CString CGrpLine::GetDisplayPosHead(CPoint pt)
{
	CString	dispStr = _T("");

	if (m_DrawRect.left > pt.x || m_DrawRect.right <= pt.x)
		return dispStr;

	if (!m_pGrpWnd->m_pGrpData|| 
		m_pGrpWnd->m_pGrpData->GetDataCount() <= 0)
		return dispStr;

	const int	dispPos = 0;
	const int	dispEnd = m_pGrpWnd->m_pGrpData->GetDataCount();
	const int	dispDay = m_pGrpWnd->m_pGrpData->GetDataCount();

	const int	xPosition = pt.x - m_DrawRect.left;
	const double	szOneDay = double(m_DrawRect.Width()) / double(dispDay);
	int	dataPos = int(double(xPosition) / szOneDay);
	dataPos += dispPos;

	struct _cgMJ	*cgMJ = 
		 (struct _cgMJ *)m_pGrpWnd->m_pGrpData->GetGraphData(dataPos);
	if (cgMJ)
		dispStr.Format("시간\t%02d:%02d:%02d", cgMJ->time.hh, 
			cgMJ->time.mm, cgMJ->time.ss);

	return dispStr;
}

CString CGrpLine::GetDisplayPosData(CPoint pt)
{
	CString	dispStr = _T("");

	if (m_DrawRect.left > pt.x || m_DrawRect.right <= pt.x)
		return dispStr;

	if (!m_pGrpWnd->m_pGrpData || 
		m_pGrpWnd->m_pGrpData->GetDataCount() <= 0)
		return dispStr;

	const int	dispPos = 0;
	const int	dispEnd = m_pGrpWnd->m_pGrpData->GetDataCount();
	const int	dispDay = m_pGrpWnd->m_pGrpData->GetDataCount();

	const int	xPosition = pt.x - m_DrawRect.left;
	const double	szOneDay = double(m_DrawRect.Width()) / double(dispDay);
	int	dataPos = int(double(xPosition) / szOneDay);
	dataPos += dispPos;

	CString	stmp, stmp2;
	char	sign = ' ';
	struct _cgMJ	*cgMJ = (struct _cgMJ *)m_pGrpWnd->m_pGrpData->GetGraphData(dataPos);

	if (!cgMJ)	return dispStr;

	if (cgMJ->ratio[m_Order] > 0)		sign = '+';
	else if (cgMJ->ratio[m_Order] == 0)	sign = ' ';

	FormatVariableComma(stmp, cgMJ->val[m_Order], m_pGrpWnd->m_pGrpData->m_pDigit[m_Order], 10);
	if (cgMJ->ratio[m_Order] < 0)
		stmp2.Format("%s\t%s %.2f%%", m_pGrpWnd->m_pGrpData->m_pName[m_Order], 
			stmp, cgMJ->ratio[m_Order]/100.);
	else
		stmp2.Format("%s\t%s %c%.2f%%", m_pGrpWnd->m_pGrpData->m_pName[m_Order], 
			stmp, sign, cgMJ->ratio[m_Order]/100.);
	dispStr = dispStr + stmp2;

	return dispStr;
}

void CGrpLine::DrawZeroLine(CDC *pDC, double Max, double Min)
{
	if (Max < 0 || Min > 0)	return;

	CPen	*cPen = ((CMainWnd*)m_pMainWnd)->GetPen(PS_SOLID, 1, RGB(128,128,128));
	CPen	*sPen = pDC->SelectObject(cPen);

	const int	tickHeight = m_DrawRect.Height();
	double	ValueHeight = Max - Min;
	if (ValueHeight <= 0)	ValueHeight = 1;
	const double	dValue = double(double(tickHeight) * Max) / ValueHeight;
	const int	y = m_DrawRect.top + int(dValue);

	pDC->MoveTo(m_pGrpWnd->m_tkInfo.tkRect[ctkLEFT].right, y);
	pDC->LineTo(m_pGrpWnd->m_tkInfo.tkRect[ctkRIGHT].left, y);

	pDC->SelectObject(sPen);
}

void CGrpLine::DrawTickHorizon(CDC* pDC, double Max, double Min, int tickinfo)
{
	int	dispCount{};
	double	dispValue[64]{};
	CRect	tickRC = m_pGrpWnd->m_tkInfo.tkRect[tickinfo];
	tickRC.top = m_DrawRect.top;

	const int	tickHeight = tickRC.Height();
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
	else if (ValueHeight < 2000000)		div = 200000;
	else if (ValueHeight < 5000000)		div = 500000;
	else if (ValueHeight < 10000000)	div = 1000000;
	else if (ValueHeight < 50000000)	div = 5000000;
	else	div = 10000000;

	const int dispTICK = GetTickCount(tickinfo);
	if (dispTICK <= 0)
		return;

	double	drawMinValue= 0.0;

	if (!calculateDisplay(div, Max, Min, drawMinValue, dispTICK, dispValue, dispCount))
	{
		return;
	}

	const int		sMode = pDC->SetBkMode(TRANSPARENT);
	const COLORREF	sColor = pDC->SetTextColor(RGB(64,64,64));
	const int		style = PS_SOLID;
	const int		width = 1;
	const COLORREF	color = m_pGrpWnd->m_tkInfo.tkColor[tickinfo];

	CRect	textRC;
	CFont	*sFont = pDC->SelectObject(m_pGrpWnd->m_pFont);
	CPen	*cPen = ((CMainWnd*)m_pMainWnd)->GetPen(style, width, color);
	CPen	*sPen = pDC->SelectObject(cPen);

	double	dValue{};
	CString	dispStr;
	CPoint	pointS, pointE;
	CRect	tkSaveRC = tickRC;

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

	size = pDC->GetOutputTextExtent(" ");
	const int	nGap = size.cx;

	FormatVariableComma(dispStr, Max, m_pGrpWnd->m_pGrpData->m_pDigit[m_Order], maxdigit);
	size = pDC->GetOutputTextExtent(dispStr);
	int	PivotY = tickRC.left + gapTICK + size.cx + nGap*2;
	if (tickinfo == ctkLEFT)
		PivotY = tickRC.right - gapTICK;

	for (int ii = 0; ii < dispCount; ii++)		// 가격별가로 (DOT)
	{
		pointS.x = m_pGrpWnd->m_tkInfo.tkRect[ctkLEFT].right+1;
		dValue = double(double(tickHeight) * double(Max - dispValue[ii]));
		dValue /= double(ValueHeight);

		pointS.y = tickRC.top + int(dValue);

		if (tickRC.top > pointS.y || tickRC.bottom < pointS.y)
				continue;

		pointE.x = m_pGrpWnd->m_tkInfo.tkRect[ctkRIGHT].left;
		pointE.y = pointS.y;
		if (pointS.y >= m_DrawRect.top && pointS.y <= m_DrawRect.bottom)
		{
			pDC->MoveTo(pointS);
			pDC->LineTo(pointE);
		}

		FormatVariableComma(dispStr, dispValue[ii], m_pGrpWnd->m_pGrpData->m_pDigit[m_Order], maxdigit);

		size = pDC->GetOutputTextExtent(dispStr);

		textRC.SetRect(tickRC.left, pointS.y - size.cy/2,
			PivotY, pointS.y + size.cy/2);

		if (textRC.bottom > tkSaveRC.bottom)
		{
			textRC.OffsetRect(0, tkSaveRC.bottom - textRC.bottom);
			tkSaveRC.bottom -= size.cy + 1;
		}
		if (textRC.top < tkSaveRC.top)
		{
			textRC.OffsetRect(0, tkSaveRC.top - textRC.top);
			tkSaveRC.top += size.cy + 1;
		}
		if (textRC.top < tickRC.top)
		{
			textRC.OffsetRect(0, tickRC.top - textRC.top);
		}

		textRC.right -= nGap;
		pDC->DrawText(dispStr, textRC, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
	}

	pDC->SelectObject(sFont);
	pDC->SelectObject(sPen);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
}

struct _dispInfo {
	char	dispStr[10]{};
	CRect	dispRC;
	CPoint	dispPt;
};

enum { dispNONE, dispONEHOUR, dispTHREEHOUR, dispONEDAY, dispTHREEDAY, dispSIXDAY};
void CGrpLine::DrawTickBottom(CDC *pDC)
{
	if (!m_pGrpWnd->m_pGrpData)
		return;

	const CRect	orgRC = m_pGrpWnd->m_ObjRect;
	CRect	tickRC = m_pGrpWnd->m_tkInfo.tkRect[ctkBOTTOM];
	const CRect	bottomRC = m_pGrpWnd->m_tkInfo.tkRect[ctkBOTTOM];
	tickRC.right = m_DrawRect.right;
	tickRC.left = m_DrawRect.left;
	const int tickWidth = tickRC.Width();
	const int tickHeight = tickRC.Height();

	const int		sMode = pDC->SetBkMode(TRANSPARENT);
	const COLORREF	sColor = pDC->SetTextColor(RGB(64,64,64));
	const int		style = PS_SOLID;
	const int		width = 1;
	const COLORREF	color = m_pGrpWnd->m_tkInfo.tkColor[ctkBOTTOM];

	CFont	*sFont = pDC->SelectObject(m_pGrpWnd->m_pFont);
	CPen	*cPen = ((CMainWnd*)m_pMainWnd)->GetPen(style, width, color);
	CPen	*sPen = pDC->SelectObject(cPen);

	CArray <_dispInfo, _dispInfo> arDispInfo;
	_dispInfo	dispInfo;
	arDispInfo.RemoveAll();

	CPoint	pointDAY;
	CString	dispStr;
	CRect	drawRC;
	CRect	saveRC(0,0,0,0);
	int	svHH = 0;
	//int     svMM = 0;
	int	savePos = -20;
	int	gap = 0;
	pointDAY.y = tickRC.top + int(tickHeight / 5);

	const int	dispPos = 0;
	const int	dispEnd = m_pGrpWnd->m_pGrpData->GetDataCount();
	const int	dispDay = m_pGrpWnd->m_pGrpData->GetDataCount();
	const int	endIdx = dispEnd - dispPos - 1;
	struct _cgMJ* cgMJ{};

	for (int ii = 0; ii < dispEnd - dispPos; ii++)
	{
		cgMJ = (struct _cgMJ *)m_pGrpWnd->m_pGrpData->GetGraphData(ii+dispPos);
		pointDAY.x = tickRC.left;
		pointDAY.x += int((tickWidth * ii) / dispDay);
		pointDAY.y = tickRC.top + int(tickHeight / 5);

		if (ii != dispEnd - dispPos - 1 && svHH == cgMJ->time.hh)
		{
			if ((cgMJ->time.mm % 10) == 0)
			{
				pDC->MoveTo(pointDAY.x, m_pGrpWnd->m_GrpRect.bottom);
				pDC->LineTo(pointDAY.x, m_pGrpWnd->m_GrpRect.bottom+4);
			}

			continue;
		}

		gap = max(pointDAY.x - savePos, gap);
		savePos = pointDAY.x;
		svHH = cgMJ->time.hh;

		if (ii == dispEnd - dispPos - 1)
			dispStr.Format("%02d:%02d", cgMJ->time.hh, cgMJ->time.mm);
		else
			dispStr.Format("%02d", cgMJ->time.hh);
		drawRC = DisplayRect(pDC, ii, dispStr, pointDAY);

		dispInfo.dispRC = drawRC;
		memset(&dispInfo.dispStr, 0x00, sizeof(dispInfo.dispStr));
		memcpy(&dispInfo.dispStr, dispStr, dispStr.GetLength());
		dispInfo.dispPt = pointDAY;
		arDispInfo.Add(dispInfo);
	}
		
	int view = dispONEDAY;
	if (gap > 55)
		view = dispONEHOUR;
	else if (gap >= 15 && gap < 55)
		view = dispTHREEHOUR;
	else
		view = dispNONE;

	bool bText = true;
	CRect unionRC;
	//CRect cmpRC;

	saveRC.SetRectEmpty();
	const int arSize = arDispInfo.GetSize();
	for (int ii = 0; ii < arSize; ii++)
	{
		bText = true;
		dispInfo = arDispInfo.GetAt(ii);
		// 최초값
		if (dispInfo.dispPt.x > tickRC.left && dispInfo.dispPt.x < tickRC.right)
		{
			pDC->MoveTo(dispInfo.dispPt.x, orgRC.top);
			pDC->LineTo(dispInfo.dispPt.x, m_pGrpWnd->m_GrpRect.bottom);
		}
		
		unionRC = saveRC & dispInfo.dispRC;
		if (!unionRC.IsRectEmpty())
			continue;
		saveRC = dispInfo.dispRC;

		pDC->MoveTo(dispInfo.dispPt.x, m_pGrpWnd->m_GrpRect.bottom);
		pDC->LineTo(dispInfo.dispPt.x, m_pGrpWnd->m_GrpRect.bottom+3);
		
		if (bText)
		{
			if (dispInfo.dispRC.right < orgRC.right)
				pDC->DrawText(dispInfo.dispStr, &dispInfo.dispRC, DT_SINGLELINE|DT_TOP|DT_CENTER);
		}
	}

	arDispInfo.RemoveAll();

	pDC->SelectObject(sFont);
	pDC->SelectObject(sPen);
	pDC->SetTextColor(sColor);
	pDC->SetBkMode(sMode);
}

CRect CGrpLine::DisplayRect(CDC *pDC, int idx, CString dispStr, CPoint dispPt)
{
	CRect dispRC = CRect(0, 0, 0, 0);
	const CSize size = pDC->GetOutputTextExtent(dispStr);
	if (idx == 0)
		dispRC.SetRect(dispPt.x, dispPt.y+2, 
				dispPt.x + size.cx, dispPt.y + size.cy);
	else
		dispRC.SetRect(dispPt.x - size.cx/2, dispPt.y+2, 
				dispPt.x + size.cx/2+1, dispPt.y + size.cy+2);

	return dispRC;
}

int CGrpLine::DrawTickIndex(CDC* pDC, CString dispText, COLORREF tColor, bool bColor, bool bIndex, CPoint pt)
{
	if (dispText.IsEmpty())
		return 0;

	const int sMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF sColor;
	sColor = pDC->SetTextColor(tColor);
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

	if (m_pGrpWnd->m_tkPoint.x == 0)
	{
		pt.x = m_pGrpWnd->m_GrpRect.left + 5;
		pt.y = m_pGrpWnd->m_GrpRect.top + 3;
	}
	else
	{
		if (m_pGrpWnd->m_tkPoint.x + size.cx > m_DrawRect.right)
		{
			pt.x = m_pGrpWnd->m_GrpRect.left + 5;
			pt.y = m_pGrpWnd->m_tkPoint.y + size.cy;
		}
		else
		{
			pt.x = m_pGrpWnd->m_tkPoint.x;
			pt.y = m_pGrpWnd->m_tkPoint.y;
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

	m_pGrpWnd->m_tkPoint.x = pt.x + size.cx + 5;
	m_pGrpWnd->m_tkPoint.y = pt.y;

	return pt.x + size.cx + 5;
}

void CGrpLine::FormatVariableComma(CString &dispTxt, double dVal, int point, int maxDigit)
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

int CGrpLine::GetTickCount(int dir)
{
	int	dispTICK = 5;
	int	tkHeight{};

	CRect	RC;
	switch (dir)
	{
	case ctkLEFT:	case ctkRIGHT:
		RC = m_DrawRect;
		break;
	case ctkBOTTOM:
		RC = m_pGrpWnd->m_tkInfo.tkRect[ctkBOTTOM];
		break;
	}

	tkHeight = RC.Height();
	dispTICK = int(tkHeight / (15));

	if (dispTICK > 10)
		dispTICK = 10;

	return dispTICK;
}

bool CGrpLine::calculateDisplay(double div, double Max, double Min, double& drawMinValue, 
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
	//double dVal = 0.0;

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

void CGrpLine::GetMinMaxValue(double& dMin, double& dMax)
{
	dMin = m_Min;
	dMax = m_Max;
}
