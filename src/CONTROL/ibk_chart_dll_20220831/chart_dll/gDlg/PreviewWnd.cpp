#include "stdafx.h"
#include "axisGDlg.h"
#include "PreviewWnd.h"
#include <math.h>
#include <float.h>
#include <afxtempl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const struct _bongdata g_abongdata[] = {
					{"1500", 9390, 9410, 9385, 9400, 6200},
					{"1501", 9400, 9405, 9390, 9395, 5949},
					{"1502", 9395, 9400, 9390, 9395, 2438},
					{"1503", 9395, 9420, 9395, 9410, 5235},
					{"1504", 9415, 9415, 9395, 9395, 3883},
					{"1505", 9395, 9400, 9380, 9395, 18319},
					{"1506", 9395, 9395, 9385, 9390, 15876},
					{"1507", 9390, 9405, 9390, 9400, 13462},
					{"1508", 9400, 9415, 9400, 9410, 6781},
					{"1509", 9410, 9420, 9405, 9415, 4238},
					{"1510", 9410, 9415, 9400, 9410, 4148},
					{"1511", 9415, 9420, 9410, 9415, 6471},
					{"1512", 9420, 9420, 9395, 9400, 3053},
					{"1513", 9405, 9410, 9400, 9410, 18000},
					{"1514", 9400, 9425, 9380, 9410, 2347},
					{"1515", 9370, 9395, 9365, 9390, 3526},
					{"1516", 9365, 9375, 9355, 9370, 2063},
					{"1517", 9365, 9370, 9360, 9365, 1885},
					{"1518", 9360, 9370, 9355, 9355, 14000},
					{"1519", 9355, 9360, 9345, 9357, 19928},
					{"1520", 9390, 9410, 9385, 9400, 6200},
					{"1521", 9400, 9405, 9390, 9395, 5949},
					{"1522", 9395, 9400, 9390, 9395, 2438},
					{"1523", 9395, 9420, 9395, 9410, 5235},
					{"1524", 9415, 9415, 9395, 9395, 3883},
					{"1525", 9395, 9400, 9380, 9395, 18319},
					{"1526", 9395, 9395, 9385, 9390, 15876},
					{"1527", 9390, 9405, 9390, 9400, 13462},
					{"1528", 9400, 9415, 9400, 9410, 6781},
					{"1529", 9410, 9420, 9405, 9415, 4238},
					{"1530", 9410, 9415, 9400, 9413, 4148},
					{"1531", 9415, 9420, 9410, 9415, 6471},
					{"1532", 9420, 9430, 9395, 9430, 3053},
					{"1533", 9405, 9410, 9400, 9400, 18000},
					{"1534", 9400, 9405, 9380, 9390, 2347},
					{"1535", 9370, 9395, 9365, 9390, 3526},
					{"1536", 9365, 9375, 9355, 9370, 2063},
					{"1537", 9365, 9370, 9360, 9367, 1885},
					{"1538", 9360, 9370, 9355, 9355, 14000},
					{"1539", 9355, 9360, 9345, 9355, 19928},
					};

CPreviewWnd::CPreviewWnd(CWnd* pParent)
{
	m_iSubKind = 0;
	m_pBitmap = NULL;
	m_pen.DeleteObject();
	m_brush.DeleteObject();
	m_pGraph = NULL;

	m_strName = _T("");

	m_dMinValue = 0.0;
	m_dMaxValue = 0.0;
	m_dUnit = 0.0;
	m_iDataCount = 0;

	m_wUnionKind	= 0;

	m_rectTotal = m_rectRegion = m_rectGraph = CRect(0, 0, 0, 0);

	for ( int ii = 0 ; ii < 3 ; ii++ )
	{
		m_arectTick[ii] = CRect(0, 0, 0, 0);
	}
	
}

CPreviewWnd::~CPreviewWnd()
{
	if (m_pBitmap)
		delete m_pBitmap;	
	m_pen.DeleteObject();
	m_brush.DeleteObject();
}

BEGIN_MESSAGE_MAP(CPreviewWnd, CWnd)
	//{{AFX_MSG_MAP(CPreviewWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CPreviewWnd::drawBitmap(CDC* pDC)
{
	CDC		memDC;
	bool		bNew = false;	
	CBitmap*	oldBitmap = NULL;
	CRect		rect;
	CFont		font, *oldfont = NULL;
	
	GetClientRect(rect);
	
	if (m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	if (pDC == NULL)
	{
		bNew = true;
		pDC = GetDC();
	}

	m_pBitmap = new CBitmap;
	memDC.CreateCompatibleDC(pDC);
	
	m_pBitmap->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	oldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap);

	font.CreatePointFont(9 * 10, "±¼¸²Ã¼");
	oldfont = memDC.SelectObject(&font);

	calcuArea(rect);
	drawBack(&memDC);
	DrawGraph(&memDC);	

	memDC.SelectObject(oldfont);
	memDC.SelectObject(oldBitmap);	
	memDC.DeleteDC();
	font.DeleteObject();

	if (bNew)
		ReleaseDC(pDC);
}

void CPreviewWnd::loadBitmap(CDC *pDC)
{
	if (m_pBitmap)
	{
		CDC		memDC;
		CRect		rect;
		CBitmap*	oldBitmap = NULL;
		bool		bNew = false;

		if (pDC == NULL)
		{
			bNew = true;
			pDC = GetDC();
		}
		
		GetClientRect(rect);
		memDC.CreateCompatibleDC(pDC);
		oldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap);
		pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(oldBitmap);
		memDC.DeleteDC();

		if (bNew)
			ReleaseDC(pDC);
	}
}

void CPreviewWnd::OnPaint() 
{
	CPaintDC dc(this);
	loadBitmap(&dc);
}

int CPreviewWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;	

	return 0;
}

CSize CPreviewWnd::calcuWndSize(CDC* pDC, CString strExam)
{
	CSize	csSize;
	csSize = pDC->GetTextExtent(strExam);
	return csSize;
}

void CPreviewWnd::setText(CDC* pDC, CRect rect, CString strInput, COLORREF txColor, UINT fmt)
{
	COLORREF	color = pDC->SetTextColor(txColor);
	UINT		deffmt = DT_SINGLELINE | DT_VCENTER | fmt;
	int	nMode = pDC->SetBkMode(TRANSPARENT);

	pDC->DrawText(strInput, rect, deffmt);
	pDC->SetBkMode(nMode);
	pDC->SetTextColor(color);
}

void CPreviewWnd::setText(CDC* pDC, CPoint pt, CString strInput, COLORREF txColor)
{
	COLORREF	clr;
	int	nMode = pDC->SetBkMode(TRANSPARENT);
	clr = pDC->SetTextColor(txColor);
	
	pDC->TextOut(pt.x, pt.y, strInput);

	pDC->SetBkMode(nMode);
	pDC->SetTextColor(clr);
}

void CPreviewWnd::setText(CDC* pDC, CPoint pt, CString strInput, COLORREF txColor, COLORREF bkColor)
{
	COLORREF	clrtext, clrback;
	
	clrtext = pDC->SetTextColor(txColor);
	clrback = pDC->SetBkColor(bkColor);	
	pDC->TextOut(pt.x, pt.y, strInput);
	pDC->SetTextColor(clrtext);
	pDC->SetBkColor(clrback);
}

void CPreviewWnd::OnDestroy() 
{
	CWnd::OnDestroy();
}

CPen* CPreviewWnd::selectPen(CDC* pDC, struct _line* lineinfo)
{
	CPen	pen(PS_SOLID, 1, RGB(0, 0, 0)), *newpen = NULL;
	pDC->SelectObject(&pen);
	m_pen.DeleteObject();
	int	nLine = PS_SOLID;
	
	m_pen.CreatePen(lineinfo->btLStyle, lineinfo->btLWidth, lineinfo->rgbLColor);
	newpen = pDC->SelectObject(&m_pen);
	pen.DeleteObject();
	return newpen;
}

CPen* CPreviewWnd::selectPen(CDC* pDC, COLORREF clr, int nWidth, int nType)
{
	CPen	pen(PS_SOLID, 1, RGB(0, 0, 0)), *newpen = NULL;

	pDC->SelectObject(&pen);
	m_pen.DeleteObject();	
	m_pen.CreatePen(nType, nWidth, clr);
	newpen = pDC->SelectObject(&m_pen);
	pen.DeleteObject();
	return pDC->SelectObject(&m_pen);
}

CBrush*	CPreviewWnd::selectBrush(CDC* pDC, COLORREF color)
{
	CBrush	brush(RGB(0, 0, 0)), *newbrush = NULL;
	
	pDC->SelectObject(&brush);
	m_brush.DeleteObject();
	m_brush.CreateSolidBrush(color);
	newbrush = pDC->SelectObject(&m_brush);
	brush.DeleteObject();
	return newbrush;
}

void CPreviewWnd::drawWidthLine(CDC* pDC, CPoint pt[2], struct _line* lineinfo)
{
	CPen*	oldpen = selectPen(pDC, lineinfo);
	int	nWidth = lineinfo->btLWidth;
	BOOL	bVertical = FALSE;
	BOOL	bHorizon = FALSE;
	int	nMode = pDC->SetBkMode(TRANSPARENT);
	
	int	ii = 0;

	if (pt[0].x == pt[1].x)
		bVertical = TRUE;
	
	if (pt[0].y == pt[1].y)
		bHorizon = TRUE;

	if ((bVertical && bHorizon) || (!bVertical && !bHorizon))
	{
		pDC->SelectObject(oldpen);
		return;
	}

	
	for ( ii = 0 ; ii < nWidth ; ii++ )
	{
		pDC->Polyline(pt, 2);

		
		if ((ii % 2) == 0)
		{
			if (bVertical)
			{
				pt[0].x += ((ii + 1) * 1);
				pt[1].x += ((ii + 1) * 1);
			}
			else if (bHorizon)
			{
				pt[0].y += ((ii + 1) * 1);
				pt[1].y += ((ii + 1) * 1);
			}
		}
		else
		{
			if (bVertical)
			{
				pt[0].x -= ((ii + 1) * 1);
				pt[1].x -= ((ii + 1) * 1);
			}
			else if (bHorizon)
			{
				pt[0].y -= ((ii + 1) * 1);
				pt[1].y -= ((ii + 1) * 1);
			}
		}
	}

	pDC->SelectObject(oldpen);
	pDC->SetBkMode(nMode);
}

int CPreviewWnd::getYPos(double val)
{
	double	fMinMax = m_dMaxValue - m_dMinValue;
	double	fHeight = m_rectGraph.Height();
	double	yPos = 0;
	double	fGab = m_dMaxValue - val;

	if (fMinMax <= 0.0)
		return m_rectGraph.bottom;

	yPos = fHeight * fGab / fMinMax;
	return (int)((double)m_rectGraph.top + yPos);
}

void CPreviewWnd::ShowGraph(struct _graph* pBong, COLORREF tkCOLOR, struct _envInfo* envinfo, struct _graph* pGraph, int nSubKind)
{
	CAxisGDlgApp*	pApp = (CAxisGDlgApp*)AfxGetApp();

	m_pGraph = pGraph;
	m_pBong = pBong;
	m_pEnv = envinfo;
	m_rgbTick = tkCOLOR;
	m_iSubKind = nSubKind;
	m_strName = pApp->GetTitle(pGraph->wGKind);
	drawBitmap();
	loadBitmap();
}

void CPreviewWnd::drawBox(CDC* pDC, CRect rect, COLORREF clrLine, COLORREF clrBack, UINT nFlag)
{
	CPen*	oldpen = selectPen(pDC, clrLine);
	
	pDC->FillSolidRect(rect, clrBack);	
	
	if (nFlag & BOX_LEFT)
	{
		pDC->MoveTo(rect.TopLeft());
		pDC->LineTo(rect.left, rect.bottom);
	}

	if (nFlag & BOX_TOP)
	{
		pDC->MoveTo(rect.TopLeft());
		pDC->LineTo(rect.right, rect.top);
	}
	
	if (nFlag & BOX_RIGHT)
	{
		pDC->MoveTo(rect.BottomRight());
		pDC->LineTo(rect.right, rect.top);
	}

	if (nFlag & BOX_BOTTOM)
	{
		pDC->MoveTo(rect.BottomRight());
		pDC->LineTo(rect.left, rect.bottom);
	}

	pDC->SelectObject(oldpen);	
}

void CPreviewWnd::calcuArea(CRect rect)
{
	if (rect == NULL)
		GetClientRect(rect);

	CRect	rcTmp;
	int	nWidth = GetSystemMetrics(SM_CYHSCROLL);

	rcTmp = m_rectTotal = rect;

	// bottom tick area
	m_arectTick[2] = rcTmp;
	m_arectTick[2].top = m_arectTick[2].bottom - nWidth;
	
	// left tick area
	m_arectTick[0] = rcTmp;
	m_arectTick[0].right = m_arectTick[0].left + (nWidth * 2);
	m_arectTick[0].bottom = m_arectTick[2].top;

	// right tick area
	m_arectTick[1] = rcTmp;
	m_arectTick[1].left = m_arectTick[1].right - (nWidth * 2);
	m_arectTick[1].bottom = m_arectTick[2].top;

	// region area
	m_rectRegion.top = rcTmp.top;
	m_rectRegion.left = m_arectTick[0].right;
	m_rectRegion.right = m_arectTick[1].left;
	m_rectRegion.bottom = m_arectTick[2].top;

	// graph area
	rcTmp = m_rectRegion;
	rcTmp.top += nWidth;
	rcTmp.DeflateRect(nWidth/2, 2, nWidth/2, 2);
	m_rectGraph = rcTmp;

	m_rectText = m_rectRegion;
	m_rectText.bottom = m_rectGraph.top;
	m_rectText.left += nWidth;

	int	nGraphWidth = m_rectGraph.Width();
	double	tmpMin = DBL_MAX, tmpMax = DBL_MIN;
	double	val1 = 0.0, val2 = 0.0;

	m_iDataCount = sizeof(g_abongdata) / sizeof(struct _bongdata) / 2;
	m_dUnit = (double)nGraphWidth / (double)m_iDataCount;
	

	for ( int ii = 0 ; ii < m_iDataCount ; ii++ )
	{
		val1 = g_abongdata[ii].koga;
		val2 = g_abongdata[ii].jega;

		if (val1 > tmpMax)
			tmpMax = val1;

		if (val2 < tmpMin)
			tmpMin = val2;
	}

	m_dMinValue = tmpMin;
	m_dMaxValue = tmpMax;
}

void CPreviewWnd::DrawGraph(CDC* pDC)
{		
	setText(pDC, m_rectText, m_strName, m_pGraph->rgbLabel, DT_LEFT);

	if (m_pGraph == NULL)
		return;

	switch (m_pGraph->wGKind)
	{
	case GK_JPN:
	case GK_BAR:
	case GK_LIN:
	case GK_BALANCE:
	case GK_POLE:
//	case GK_AVOL:
//	case GK_EQV:
//	case GK_CDV:
		switch (m_wUnionKind)
		{
		case GK_AVOL:
			//DrawUnionAVol(pDC, m_pGraph->cInfo.aline[1].rgbLColor, m_pGraph->cInfo.aline[2].rgbLColor);
			DrawUnionAVol(pDC, RGB(255, 0, 0), RGB(0, 0, 255));
			break;
		case GK_PNF:
			//DrawUnionPnF(pDC, m_pGraph->cInfo.aline[1].rgbLColor, m_pGraph->cInfo.aline[2].rgbLColor);
			DrawUnionPnF(pDC, RGB(255, 0, 0), RGB(0, 0, 255));
			break;
		case GK_SWING:
			//DrawUnionSwing(pDC, m_pGraph->cInfo.aline[1].rgbLColor, m_pGraph->cInfo.aline[2].rgbLColor);
			DrawUnionSwing(pDC, RGB(255, 0, 0), RGB(0, 0, 255));
			break;
		case GK_THREE:
			//DrawUnionSamsun(pDC, m_pGraph->cInfo.aline[1].rgbLColor, m_pGraph->cInfo.aline[2].rgbLColor);
			DrawUnionSamsun(pDC, RGB(255, 0, 0), RGB(0, 0, 255));
			break;
		case GK_KAGI:
			//DrawUnionKagi(pDC, m_pGraph->cInfo.aline[1].rgbLColor, m_pGraph->cInfo.aline[2].rgbLColor);
			DrawUnionKagi(pDC, RGB(255, 0, 0), RGB(0, 0, 255));
			break;
		case GK_RENKO:
			//DrawUnionRenko(pDC, m_pGraph->cInfo.aline[1].rgbLColor, m_pGraph->cInfo.aline[2].rgbLColor);
			DrawUnionRenko(pDC, RGB(255, 0, 0), RGB(0, 0, 255));
			break;
		case GK_GUKKI:
			//DrawUnionGukki(pDC, m_pGraph->cInfo.aline[1].rgbLColor, m_pGraph->cInfo.aline[2].rgbLColor);
			DrawUnionGukki(pDC, RGB(255, 0, 0), RGB(0, 0, 255));
			break;
		}
	}

	switch (m_pGraph->wGKind)
	{
	case GK_JPN:
		if (m_iSubKind == GK_LIN)
			drawLIN(pDC, &m_pGraph->cInfo.aline[0]);
		else
			drawJPN(pDC, m_pGraph->cInfo.aline[1].rgbLColor, m_pGraph->cInfo.aline[2].rgbLColor);
		break;
	case GK_BAR:
		if (m_iSubKind == GK_LIN)
			drawLIN(pDC, &m_pGraph->cInfo.aline[0]);
		else
			drawBAR(pDC, m_pGraph->cInfo.aline[1].rgbLColor, m_pGraph->cInfo.aline[2].rgbLColor);
		break;
	case GK_LIN:
		drawLIN(pDC, &m_pGraph->cInfo.aline[0]);
		break;
	case GK_POLE:
		if (m_iSubKind == GK_LIN)
			drawLIN(pDC, &m_pGraph->cInfo.aline[0]);
		else
			drawPOLE(pDC, m_pGraph->cInfo.aline[1].rgbLColor, m_pGraph->cInfo.aline[2].rgbLColor);
		break;
	case GK_CDV:
		drawCQV(pDC);
		break;
	case GK_EQV:
		drawEQV(pDC);
		break;
	case GK_PNF:
		drawPNF(pDC);
		break;
	case GK_THREE:
		drawTHREE(pDC);
		break;
	case GK_CLOCK:
		drawCLOCK(pDC);
		break;
	case GK_PV:
		drawPV(pDC);
		break;
	case GK_AVOL:
		drawSALE(pDC);
/*
		switch (m_wUnionKind)
		{
		case GK_AVOL:
			//DrawUnionAVol(pDC, m_pGraph->cInfo.aline[1].rgbLColor, m_pGraph->cInfo.aline[2].rgbLColor);
			DrawUnionAVol(pDC, RGB(255, 0, 0), RGB(0, 0, 255));
			break;
		case GK_PNF:
			//DrawUnionPnF(pDC, m_pGraph->cInfo.aline[1].rgbLColor, m_pGraph->cInfo.aline[2].rgbLColor);
			DrawUnionPnF(pDC, RGB(255, 0, 0), RGB(0, 0, 255));
			break;
		case GK_SWING:
			//DrawUnionSwing(pDC, m_pGraph->cInfo.aline[1].rgbLColor, m_pGraph->cInfo.aline[2].rgbLColor);
			DrawUnionSwing(pDC, RGB(255, 0, 0), RGB(0, 0, 255));
			break;
		case GK_THREE:
			//DrawUnionSamsun(pDC, m_pGraph->cInfo.aline[1].rgbLColor, m_pGraph->cInfo.aline[2].rgbLColor);
			DrawUnionSamsun(pDC, RGB(255, 0, 0), RGB(0, 0, 255));
			break;
		case GK_KAGI:
			//DrawUnionKagi(pDC, m_pGraph->cInfo.aline[1].rgbLColor, m_pGraph->cInfo.aline[2].rgbLColor);
			DrawUnionKagi(pDC, RGB(255, 0, 0), RGB(0, 0, 255));
			break;
		case GK_RENKO:
			//DrawUnionRenko(pDC, m_pGraph->cInfo.aline[1].rgbLColor, m_pGraph->cInfo.aline[2].rgbLColor);
			DrawUnionRenko(pDC, RGB(255, 0, 0), RGB(0, 0, 255));
			break;
		case GK_GUKKI:
			//DrawUnionGukki(pDC, m_pGraph->cInfo.aline[1].rgbLColor, m_pGraph->cInfo.aline[2].rgbLColor);
			DrawUnionGukki(pDC, RGB(255, 0, 0), RGB(0, 0, 255));
			break;
		}
*/
		break;
	case GK_BALANCE:
		drawBalance(pDC);
		break;
	case GK_KAGI:
		drawKagi(pDC);
		break;
	case GK_RENKO:
		drawRenko(pDC);
		break;	
	}


	if (m_pGraph->wGKind != GK_CDV && m_pGraph->wGKind != GK_EQV)
	{
		drawTICK(pDC);
	}
}

void CPreviewWnd::drawBalance(CDC* pDC)
{
	int	nCount = m_iDataCount / 2;
	int	ii = 0;
	m_dMinValue = INT_MAX;
	m_dMaxValue = INT_MIN;	

	for ( ii = 0 ; ii < nCount ; ii++ )
	{
		if (m_dMaxValue < g_abongdata[ii].koga)
			m_dMaxValue = g_abongdata[ii].koga;
		if (m_dMinValue > g_abongdata[ii].jega)
			m_dMinValue = g_abongdata[ii].jega;
	}

	double	min = m_dMinValue;
	double	max = m_dMaxValue;
	m_dMinValue -= 40;
	drawJPN(pDC, m_pBong->cInfo.aline[1].rgbLColor, m_pBong->cInfo.aline[2].rgbLColor);
	m_dMinValue = min;

	double*	value[5];
	CPen*	oldpen = selectPen(pDC, &m_pGraph->cInfo.aline[0]);	

	min = m_dMinValue = INT_MAX;
	max = m_dMaxValue = INT_MIN;

	for ( ii = 0 ; ii < 5 ; ii++ )
	{
		value[ii] = LIB_new((double*)NULL, m_iDataCount);

		for ( int jj = 0 ; jj < m_iDataCount ; jj++ )
		{
			if (jj < (m_iDataCount / 2))
			{
				if (ii == 0)
				{
					value[ii][jj] = g_abongdata[jj].koga - 2;
				}
				else if (ii == 1)
				{
					value[ii][jj] = g_abongdata[jj].jega + 5;
				}
				else if (ii == 2)
				{
					if (jj < (m_iDataCount / 4))
						value[ii][jj] = (g_abongdata[jj].jega + g_abongdata[jj].koga) / 2;
					else
						value[ii][jj] = -1;
				}
				else if (ii == 3)
				{
					value[ii][jj] = g_abongdata[0].jega + 5;
				}
				else if (ii == 4)
				{
					if (jj < (m_iDataCount / 4))
						value[ii][jj] = value[ii-1][jj] - 10;
					else
						value[ii][jj] = value[ii-1][jj] - 5;
				}
			}
			else	
			{
				if (ii == 3)
				{
					value[ii][jj] = g_abongdata[0].jega + 5;
				}
				else if (ii == 4)
				{
					if (jj < (m_iDataCount / 4 * 3))
						value[ii][jj] = value[ii-1][jj] + 5;
					else
						value[ii][jj] = value[ii-1][jj] + 10;
				}	
				else
					value[ii][jj] = -1;
			}
				

			if (value[ii][jj] >= 0)
			{
				if (jj < 3)
				{
					if (value[ii][jj] > m_dMaxValue)
						m_dMaxValue = value[ii][jj];

					if (value[ii][jj] < m_dMinValue)
						m_dMinValue = value[ii][jj];
				}
				else
				{
					if (value[ii][jj] > max)
						max = value[ii][jj];

					if (value[ii][jj] < min)
						min = value[ii][jj];
				}
			}
		}
	}

	m_dMinValue -= 5;
	m_dMaxValue += 30;

	CPoint	pt;

	for ( ii = 0 ; ii < 5 ; ii++ )
	{
		selectPen(pDC, &m_pGraph->cInfo.aline[ii]);

		if (ii == 3)
		{
			m_dMaxValue = max;
			m_dMinValue = min - 10;
		}

		for ( int jj = 0 ; jj < m_iDataCount ; jj++ )
		{
			if (value[ii][jj] < 0)
				continue;

			pt.x = getXPosM(jj);
			pt.y = getYPos(value[ii][jj]);

			if (jj == 0)
				pDC->MoveTo(pt);
			else
				pDC->LineTo(pt);
		}
	}

	for ( ii = 0 ; ii < m_iDataCount ; ii++ )
	{
		if (value[3][ii] > value[4][ii])
			selectPen(pDC, m_pGraph->cInfo.aline[5].rgbLColor);
		else
			selectPen(pDC, m_pGraph->cInfo.aline[6].rgbLColor);

		pt.x = getXPosM(ii);
		pt.y = getYPos(value[3][ii]);
		pDC->MoveTo(pt);
		pt.x = getXPosM(ii);
		pt.y = getYPos(value[4][ii]);
		pDC->LineTo(pt);
	}
	
	for ( ii = 0 ; ii < 5 ; ii++ )
	{
		LIB_delete(value[ii]);
	}
	
	pDC->SelectObject(oldpen);

}

struct _klocal_data
{
	double	val[2];
};

void CPreviewWnd::drawKagi(CDC* pDC)
{
	int	nSize = sizeof(g_abongdata)/sizeof(struct _bongdata);
	int	ii = 0;
	double	pVal = 0, cVal = 0;
	double	val = 0, diff = 0;
	bool	bUp = false, bPrev = false, bTurn = true, bStart = false, bPaning = false;	
	
	struct _klocal_data*	pObject = NULL;           
	CArray<struct _klocal_data*, struct _klocal_data*>	arData;	
	bPrev = bUp;

	double	Value = 0;

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		cVal = g_abongdata[ii].curr;

		if (ii == 0)
		{
			pVal = (double)cVal;
			continue;
		}

		if (cVal > pVal)
			bUp = true;
		else if (cVal < pVal)
			bUp = false;
		else
			continue;

		if (bStart && bPrev != bUp)
		{
			diff = fabs(pVal - cVal);

			if (diff >= Value)
				bTurn = true;
			else
			{
				bUp = bPrev;
				continue;
			}
		}
		
		bStart = true;
		
		if (bTurn)
		{
			if (pObject)
				pVal = pObject->val[1];

			pObject = new _klocal_data;
			pObject->val[0] = pVal;
			pObject->val[1] = cVal;
			bTurn = false;	
			arData.Add(pObject);
		}
		else
		{
			if (bUp)
			{
				if ( cVal > pObject->val[1] )
					pObject->val[1] = cVal;
			}
			else
			{
				if ( cVal < pObject->val[1] )
					pObject->val[1] = cVal;
			}			
		}

		bPrev = bUp;
		pVal = cVal;

		if (pObject->val[0] > m_dMaxValue)
			m_dMaxValue = pObject->val[0];
		if (pObject->val[1] > m_dMaxValue)
			m_dMaxValue = pObject->val[1];
		if (pObject->val[0] < m_dMinValue)
			m_dMinValue = pObject->val[0];
		if (pObject->val[1] < m_dMinValue)
			m_dMinValue = pObject->val[1];
	}

	int	nCount = arData.GetSize();

	if (nCount == 0)
	{
		m_dUnit = 0.0;
		return;
	}

	int	nWidth = m_rectGraph.Width();
	int	nWidthCount = nWidth / 7;

	if (nWidthCount <= 0)
		return;

	int	nShow = nWidthCount;
	m_dUnit = (double)nWidth / (nShow + 2);

	CPen	upPen(m_pGraph->cInfo.aline[0].btLStyle, m_pGraph->cInfo.aline[0].btLWidth, m_pGraph->cInfo.aline[0].rgbLColor);
	CPen	dnPen(m_pGraph->cInfo.aline[1].btLStyle, m_pGraph->cInfo.aline[1].btLWidth, m_pGraph->cInfo.aline[1].rgbLColor);
	CPen	*SavePen = pDC->SelectObject(&upPen);	

	CPoint	pt;
	CPoint*	ppt = NULL;
	CString	dispText;
	nSize = arData.GetSize();
	int	nIndex = 0;

	if (nShow < nSize)
		nIndex = nSize - nShow;

	double	nXpos = (double)m_rectGraph.left + m_dUnit / 2.0, fMid = 0.0, fStandard = 0.0;
	int	nMid = 0;
	struct	_klocal_data*	ppObject = NULL;
	bool	bThick = false;
	bUp = bThick;	// true : red false : blue
	CArray<CPoint, CPoint>		arPt;
	CMap<int, int, int, int>	mapColor;
	int	nPt = 0, nColor = 0;	
	CRect	rect;

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		pObject = arData.GetAt(ii);

		if (ii)
			ppObject = arData.GetAt(ii - 1);
		
		if (pObject->val[1] > pObject->val[0])
			bUp = true;
		else
			bUp = false;

		fMid = (pObject->val[1] + pObject->val[0]) / 2.0;	

		if (!ppObject)
		{
			if (bUp)
			{
				bThick = true;
			}
			else
			{
				bThick = false;
			}
			
			if (bThick)
				pDC->SelectObject(&upPen);
			else
				pDC->SelectObject(&dnPen);

			arPt.RemoveAll();

			nMid = (int)(nXpos + m_dUnit / 2.0);
			pt.x = (int)nXpos;
			pt.y = getYPos(pObject->val[0]);
			arPt.Add(pt);
			
			pt.x = nMid;
			arPt.Add(pt);

			pt.y = getYPos(fMid);
			arPt.Add(pt);

			pt.x = (int)(nXpos + m_dUnit);
			arPt.Add(pt);

			pt.x = nMid;
			arPt.Add(pt);

			pt.y = getYPos(pObject->val[1]);
			arPt.Add(pt);

			pt.x = (int)(nXpos + m_dUnit);
			arPt.Add(pt);

			nPt = arPt.GetSize();

			if (ii < nIndex)
				continue;	

			for ( int jj = 0 ; jj < nPt ; jj++ )
			{
				pt = arPt.GetAt(jj);

				if (jj == 0)
					pDC->MoveTo(pt);
				else
				{
					if (ii == (nSize - 1))
					{
						if (jj != (nPt - 1))
							pDC->LineTo(pt);
					}
					else
						pDC->LineTo(pt);
				}
			}
		}
		else
		{	
			
			arPt.RemoveAll();
			mapColor.RemoveAll();
			fStandard = ppObject->val[0];	
			nMid = (int)(nXpos + m_dUnit / 2.0);
			
			pt.x = (int)nXpos;
			pt.y = getYPos(pObject->val[0]);
			arPt.Add(pt);		
			
			pt.x = nMid;
			arPt.Add(pt);

			if (bThick)
				nColor = 1;
			else
				nColor = 0;
				
			mapColor.SetAt(arPt.GetSize() - 1, nColor);

			if (bUp)
			{				
				if (!bThick && pObject->val[1] > fStandard)
				{
					if (fMid > fStandard)
					{
						pt.y = getYPos(fStandard);
						arPt.Add(pt);					
						mapColor.SetAt(arPt.GetSize() - 1, nColor);						
						
						pt.y = getYPos(fMid);
						arPt.Add(pt);

						bThick = !bThick;

						if (bThick)
							nColor = 1;						
						else
							nColor = 0;

						mapColor.SetAt(arPt.GetSize() - 1, nColor);
						
						pt.x = (int)(nXpos + m_dUnit);
						arPt.Add(pt);
						mapColor.SetAt(arPt.GetSize() - 1, nColor);

						pt.x = nMid;
						arPt.Add(pt);
						mapColor.SetAt(arPt.GetSize() - 1, nColor);
					}
					else
					{
						pt.y = getYPos(fMid);
						arPt.Add(pt);						
						mapColor.SetAt(arPt.GetSize() - 1, nColor);

						pt.x = (int)(nXpos + m_dUnit);
						arPt.Add(pt);						
						mapColor.SetAt(arPt.GetSize() - 1, nColor);

						pt.x = nMid;
						arPt.Add(pt);				
						mapColor.SetAt(arPt.GetSize() - 1, nColor);

						pt.y = getYPos(fStandard);
						arPt.Add(pt);					
						mapColor.SetAt(arPt.GetSize() - 1, nColor);

						bThick = !bThick;

						if (bThick)
							nColor = 1;						
						else
							nColor = 0;
					}

					
				}
				else
				{
					pt.y = getYPos(fMid);
					arPt.Add(pt);						
					mapColor.SetAt(arPt.GetSize() - 1, nColor);

					pt.x = (int)(nXpos + m_dUnit);
					arPt.Add(pt);						
					mapColor.SetAt(arPt.GetSize() - 1, nColor);

					pt.x = nMid;
					arPt.Add(pt);				
					mapColor.SetAt(arPt.GetSize() - 1, nColor);									
				}
			}
			else
			{
				if (bThick && pObject->val[1] < fStandard)
				{
					if (fMid < fStandard)
					{
						pt.y = getYPos(fStandard);
						arPt.Add(pt);					
						mapColor.SetAt(arPt.GetSize() - 1, nColor);						
						
						pt.y = getYPos(fMid);
						arPt.Add(pt);

						bThick = !bThick;

						if (bThick)
							nColor = 1;						
						else
							nColor = 0;

						mapColor.SetAt(arPt.GetSize() - 1, nColor);
						
						pt.x = (int)(nXpos + m_dUnit);
						arPt.Add(pt);
						mapColor.SetAt(arPt.GetSize() - 1, nColor);

						pt.x = nMid;
						arPt.Add(pt);
						mapColor.SetAt(arPt.GetSize() - 1, nColor);
					}
					else
					{
						pt.y = getYPos(fMid);
						arPt.Add(pt);						
						mapColor.SetAt(arPt.GetSize() - 1, nColor);

						pt.x = (int)(nXpos + m_dUnit);
						arPt.Add(pt);						
						mapColor.SetAt(arPt.GetSize() - 1, nColor);

						pt.x = nMid;
						arPt.Add(pt);				
						mapColor.SetAt(arPt.GetSize() - 1, nColor);

						pt.y = getYPos(fStandard);
						arPt.Add(pt);					
						mapColor.SetAt(arPt.GetSize() - 1, nColor);

						bThick = !bThick;

						if (bThick)
							nColor = 1;						
						else
							nColor = 0;
					}					
				}
				else
				{
					pt.y = getYPos(fMid);
					arPt.Add(pt);						
					mapColor.SetAt(arPt.GetSize() - 1, nColor);

					pt.x = (int)(nXpos + m_dUnit);
					arPt.Add(pt);						
					mapColor.SetAt(arPt.GetSize() - 1, nColor);

					pt.x = nMid;
					arPt.Add(pt);				
					mapColor.SetAt(arPt.GetSize() - 1, nColor);									
				}
			}

			pt.y = getYPos(pObject->val[1]);
			arPt.Add(pt);
			mapColor.SetAt(arPt.GetSize() - 1, nColor);

			pt.x = (int)(nXpos + m_dUnit);
			arPt.Add(pt);
			mapColor.SetAt(arPt.GetSize() - 1, nColor);

			nPt = arPt.GetSize();
			int	nPrev = -1;

			if (ii < nIndex)
				continue;

			for ( int jj = 0 ; jj < nPt ; jj++ )
			{
				pt = arPt.GetAt(jj);

				if (jj == 0)
					pDC->MoveTo(pt);
				else
				{
					mapColor.Lookup(jj, nColor);

					if (nPrev != nColor)
					{
						if (nColor)
							pDC->SelectObject(&upPen);
						else
							pDC->SelectObject(&dnPen);
					}
					
					nPrev = nColor;

					if (ii == (nSize - 1))
					{
						if (jj != (nPt - 1))
							pDC->LineTo(pt);
					}
					else
						pDC->LineTo(pt);
				}
			}		
		}

		CPoint	ptTmp[2];

		ptTmp[0] = arPt.GetAt(0);
		ptTmp[1] = arPt.GetAt(nPt - 1);
		
		if (bUp)
			rect.SetRect(CPoint(ptTmp[0].x, ptTmp[1].y), CPoint(ptTmp[1].x, ptTmp[0].y));
		else
			rect.SetRect(ptTmp[0], ptTmp[1]);
		
		nXpos += m_dUnit;
	}

	pDC->SelectObject(SavePen);
	upPen.DeleteObject();
	dnPen.DeleteObject();
	arPt.RemoveAll();
	mapColor.RemoveAll();
	nSize = arData.GetSize();

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		pObject = arData.GetAt(ii);
		delete pObject;
		pObject = NULL;
	}

	arData.RemoveAll();	
}

double CPreviewWnd::getDefVal()
{
	double	minValue = DBL_MAX;
	double	maxValue = DBL_MIN;
	double	val = 0.0;

	int	nSize = sizeof(g_abongdata) / sizeof(struct _bongdata);
	int	ii = 0;

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		if (g_abongdata[ii].koga > maxValue)
			maxValue = g_abongdata[ii].koga;

		if (g_abongdata[ii].jega < minValue)
			minValue = g_abongdata[ii].jega;
	}

	double	gap = maxValue - minValue;
	double	div;
	if (gap < 5)			div = 1;
	else if (gap < 50)		div = 5;
	else if (gap < 100)		div = 10;
	else if (gap < 200)		div = 20;
	else if (gap < 500)		div = 50;
	else if (gap < 1000)		div = 100;
	else if (gap < 2000)		div = 200;
	else if (gap < 5000)		div = 500;
	else if (gap < 10000)		div = 1000;
	else if (gap < 20000)		div = 2000;
	else if (gap < 50000)		div = 5000;
	else if (gap < 100000)		div = 10000;
	else if (gap < 200000)		div = 20000;
	else if (gap < 500000)		div = 50000;
	else if (gap < 1000000)		div = 100000;
	else if (gap < 2000000)		div = 200000;
	else if (gap < 5000000)		div = 500000;
	else if (gap < 10000000)	div = 1000000;
	else if (gap < 50000000)	div = 5000000;
	else if (gap < 100000000)	div = 10000000;
	else if (gap < 500000000)	div = 50000000;
	else if (gap < 1000000000)	div = 100000000;
	else				div = 500000000;

	if (div >= 5)
		div /= 4.0;

	return div;
}

struct _fhle_
{
	double	fpr;
	double	hpr;
	double	lpr;
	double	epr;
};

struct _local_data
{		
	double	hpr;
	double	lpr;
	bool	bUp;
	struct _fhle_	info;
};

void CPreviewWnd::drawRenko(CDC* pDC)
{
	// make data
	int	nSize = sizeof(g_abongdata) / sizeof(struct _bongdata) / 2;
	int	ii = 0, jj = 0;

	double	JWValue = 5;
	
	double	pHpr = 0, pLpr = 0;
	double	val = 0, diff1 = 0, diff2 = 0;
	bool	bUp = false;
	int	nBox = 0;

	CArray<struct _local_data*, struct _local_data*>	arData;
	struct _local_data*	pObject = NULL;	

	m_dMinValue = DBL_MAX;
	m_dMaxValue = DBL_MIN;
	
	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		val = g_abongdata[ii].curr;

		if (ii == 0)
		{
			pHpr = pLpr = val;
			continue;
		}

		diff1 = val - pHpr;
		diff2 = pLpr - val;

		if (diff1 > 0)
			bUp = true;
		else if (diff2 > 0)
		{
			diff1 = diff2;
			bUp = false;
		}
		else 
			continue;

		nBox = (int)(diff1 / JWValue);

		if (!nBox)
			continue;


		for ( int jj = 0 ; jj < nBox ; jj++ )
		{
			pObject = new struct _local_data;
			arData.Add(pObject);			
			pObject->bUp = bUp;
						
			if (bUp)
			{
				pObject->hpr = pHpr + JWValue;
				pObject->lpr = pHpr;					
			}
			else
			{
				pObject->hpr = pLpr;
				pObject->lpr = pLpr - JWValue;
			}	

			pHpr = pObject->hpr;
			pLpr = pObject->lpr;
			
			if (pHpr > m_dMaxValue)
				m_dMaxValue = pHpr;
			if (pLpr < m_dMinValue)
				m_dMinValue = pLpr;
		}
	}	

	int	nCount = arData.GetSize();
	int	nShowCount = 0;

	if (nCount == 0)
	{
		m_dUnit = 0.0;
		return;
	}

	int	nWidth = m_rectGraph.Width();
	int	nWidthCount = nWidth / 7;

	if (nWidthCount <= 0)
		return;

	nShowCount = nWidthCount;
	m_dUnit = (double)nWidth / (nShowCount + 2);

	// draw
	CPen	UpPen(m_pGraph->cInfo.aline[0].btLStyle, m_pGraph->cInfo.aline[0].btLWidth, m_pGraph->cInfo.aline[0].rgbLColor);
	CPen	DnPen(m_pGraph->cInfo.aline[1].btLStyle, m_pGraph->cInfo.aline[1].btLWidth, m_pGraph->cInfo.aline[1].rgbLColor);
	
	CBrush	DnBrush(m_pGraph->cInfo.aline[1].rgbLColor);
	CBrush	UpBrush(m_pGraph->cInfo.aline[0].rgbLColor);
	CPen	*SavePen = pDC->SelectObject(&UpPen);
	CBrush	*SaveBrush = pDC->SelectObject(&UpBrush);

	CPoint	pt;
	CPoint	ptAr[5];
	CString	dispText;
	int	nIndex = 0;

	if (nShowCount < nSize)
		nIndex = nSize - nShowCount;
	
	double	nXpos = (double)m_rectGraph.left + m_dUnit;
	
	for ( ii = nIndex, jj = 0; ii < nCount ; ii++, jj++ )
	{
		pObject = arData.GetAt(ii);
		ptAr[1].x = ptAr[0].x = (int)nXpos;
		ptAr[3].y = ptAr[0].y = getYPos(pObject->lpr);

		ptAr[1].x = ptAr[0].x;
		ptAr[2].y = ptAr[1].y = getYPos(pObject->hpr);
		
		ptAr[3].x = ptAr[2].x = (int)(nXpos + m_dUnit);

		ptAr[4] = ptAr[0];
		nXpos += m_dUnit;

		if (pObject->bUp)
		{
			pDC->SelectObject(&UpPen);
			pDC->SelectObject(&UpBrush);			
			if (m_pGraph->dwDraOption & GO_FIILUPBONG)
				pDC->Polygon(ptAr, 5);
			else
				pDC->Polyline(ptAr, 5);
		}
		else
		{
			pDC->SelectObject(&DnPen);
			pDC->SelectObject(&DnBrush);
			if (m_pGraph->dwDraOption & GO_FIILDNBONG)
				pDC->Polygon(ptAr, 5);
			else
				pDC->Polyline(ptAr, 5);
		}
		
		
	}

	pDC->SelectObject(SavePen);
	pDC->SelectObject(SaveBrush);

	UpPen.DeleteObject();
	DnBrush.DeleteObject();
	DnPen.DeleteObject();
	UpBrush.DeleteObject();

	nSize = arData.GetSize();

	for ( ii = 0 ; ii < nSize ; ii++ )
	{
		pObject = arData.GetAt(ii);
		delete pObject;
		pObject = NULL;
	}

	arData.RemoveAll();
}

void CPreviewWnd::drawCLOCK(CDC* pDC)
{
	CPen*	oldpen = selectPen(pDC, &m_pGraph->cInfo.aline[0]);
	
	CPoint	pt[16];
	int	nPen = -1, ii = 0;
	double	value[16];
	int	nIndex[16] = {10, 10, 14, 14, 14, 10, 10, 2, 10, 2, 2, 10, 10, 18, 10, 18};
	char*	date[5] = {"20020310", "20020320", "20020330", "20020410", "20020420"};
	CFont	font, *oldfont = NULL;
	COLORREF	textColor = pDC->SetTextColor(m_pGraph->cInfo.aline[0].rgbLColor);
	int	nMode = pDC->SetBkMode(TRANSPARENT);

	font.CreatePointFont(6* 10, "±¼¸²Ã¼");
	oldfont = pDC->SelectObject(&font);

	value[15] = value[14] = value[11] = value[9] = value[8] = value[2] = value[0] = (m_dMaxValue + m_dMinValue) / 2.0;	 
	value[5] = value[3] = value[1] = (m_dMaxValue + value[0]) / 2.0;
	value[7] = value[6] = value[4] = m_dMaxValue;
	value[13] = value[12] = value[10] = (m_dMinValue + value[0]) / 2.0;

	for ( ii = 0 ; ii < 16 ; ii++ )
	{		
		pt[ii] = CPoint(getXPosM(nIndex[ii]), getYPos(value[ii]));
	}

	BYTE	opt = PT_BEZIERTO;	

	for ( ii = 0 ; ii < 5 ; ii++ )
	{
		selectPen(pDC, &m_pGraph->cInfo.aline[ii % 5]);

		if (ii == 0)
			pDC->TextOut(pt[0].x, pt[0].y, date[ii]);
		if (ii == 4)
			pDC->TextOut(pt[15].x, pt[15].y, date[ii]);

		pDC->PolyBezier(&pt[ii * 3], 4);
	}
	
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldfont);
	pDC->SetTextColor(textColor);
	pDC->SetBkMode(nMode);
	font.DeleteObject();
}

void CPreviewWnd::drawPV(CDC* pDC)
{
	CPen*	oldpen = selectPen(pDC, &m_pGraph->cInfo.aline[0]);	
	CPoint	pt, pt2;
	int	nPen = -1;
	char*	date[5] = {"20020310", "20020320", "20020330", "20020410", "20020420"};
	CFont	font, *oldfont = NULL;
	COLORREF	textColor = pDC->SetTextColor(RGB(0, 0, 0));
	int	nMode = pDC->SetBkMode(TRANSPARENT);

	font.CreatePointFont(6* 10, "±¼¸²Ã¼");
	oldfont = pDC->SelectObject(&font);
	
	for ( int ii = 0 ; ii < m_iDataCount ; ii++ )
	{
		pt= CPoint(getXPosM(ii), getYPos(g_abongdata[ii].curr));

		if (ii == 0)
			pt2 = pt;
		
		if ((ii % 4) == 0)
		{
			nPen++;
			nPen = nPen % 5;
			selectPen(pDC, &m_pGraph->cInfo.aline[nPen]);
			pDC->TextOut(pt2.x, pt2.y, date[nPen]);
		}		

		if (ii == 0)
			pDC->MoveTo(pt);
		else
			pDC->LineTo(pt);
		
		pt2 = pt;
	}

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldfont);
	pDC->SetTextColor(textColor);
	pDC->SetBkMode(nMode);
	font.DeleteObject();
}

void CPreviewWnd::drawCQV(CDC* pDC)
{
	CRect	rcAll = m_rectGraph;
	CRect	rcCandle, rcVol;
	int	nWidth = rcAll.Height() / 3;
	double*	width = LIB_new((double*)NULL, m_iDataCount);
	calcuVolWidth(width);

	rcVol = rcCandle = rcAll;
	rcVol.top = rcVol.bottom - nWidth;
	rcCandle.bottom = rcVol.top - 2;	

	CPen*	oldpen = selectPen(pDC, &m_pGraph->cInfo.aline[1]);
	CBrush*	oldbrush = selectBrush(pDC, m_pGraph->cInfo.aline[1].rgbLColor);
	CPoint	pt[11];
	double	nStart = m_rectGraph.left;
	bool	bUp = false;
	m_rectGraph = rcCandle;
	
	for ( int ii = 0 ; ii < m_iDataCount ; ii++ )
	{
		pt[0] = pt[10] = CPoint(int(nStart + (width[ii] / 2.0)), getYPos(g_abongdata[ii].koga));
				
		if (g_abongdata[ii].curr > g_abongdata[ii].siga)
		{
			bUp = true;
			selectPen(pDC, &m_pGraph->cInfo.aline[1]);
			selectBrush(pDC, m_pGraph->cInfo.aline[1].rgbLColor);
			pt[1] = CPoint(pt[0].x, getYPos(g_abongdata[ii].curr));
			pt[4] = CPoint(pt[0].x, getYPos(g_abongdata[ii].siga));
		}
		else
		{
			bUp = false;
			selectPen(pDC, &m_pGraph->cInfo.aline[2]);
			selectBrush(pDC, m_pGraph->cInfo.aline[2].rgbLColor);
			pt[4] = CPoint(pt[0].x, getYPos(g_abongdata[ii].curr));
			pt[1] = CPoint(pt[0].x, getYPos(g_abongdata[ii].siga));
		}

		pt[2] = CPoint((int)(nStart + 1.0), pt[1].y);
		pt[3] = CPoint(pt[2].x, pt[4].y);
		pt[5] = CPoint(pt[0].x, getYPos(g_abongdata[ii].jega));
		pt[6] = pt[4];
		pt[7] = CPoint(int(nStart + width[ii] - 1.0), pt[4].y);
		pt[8] = CPoint(pt[7].x, pt[1].y);
		pt[9] = pt[1];
		
		if (bUp)
		{
			if (m_pGraph->dwDraOption & GO_FIILUPBONG)
				pDC->Polygon(pt, 11);
			else
				pDC->Polyline(pt, 11);
		}
		else
		{
			if (m_pGraph->dwDraOption & GO_FIILDNBONG)
				pDC->Polygon(pt, 11);
			else
				pDC->Polyline(pt, 11);
		}


		nStart += width[ii];
	}

	pt[0] = CPoint(m_rectRegion.left, rcCandle.bottom + 2);
	pt[1] = CPoint(m_rectRegion.right, rcCandle.bottom + 2);
	struct _line line;
	line.rgbLColor = RGB(0, 0, 0);
	line.btLStyle = PS_SOLID;
	line.btLWidth = 1;
	drawWidthLine(pDC, pt, &line);
	drawTICK(pDC);

	m_dMinValue = 0;
	double	tmpMin = DBL_MAX, tmpMax = DBL_MIN;
	double	val = 0.0;

	selectPen(pDC, RGB(84,164,5));
	selectBrush(pDC, RGB(84,164,5));
	
	for ( int ii = 0 ; ii < m_iDataCount ; ii++ )
	{
		val = g_abongdata[ii].vol;
		if (val > tmpMax)
			tmpMax = val;
	}

	m_dMaxValue = tmpMax;
	rcVol.DeflateRect(0, 2, 0, 2);
	

	m_rectGraph = rcVol;

	nStart = rcVol.left;
	for ( int ii = 0 ; ii < m_iDataCount ; ii++ )
	{
		pt[0] = CPoint(int(nStart + 1.0), getYPos(g_abongdata[ii].vol));
		pt[1] = CPoint(int((double)pt[0].x + width[ii] - 2.0), pt[0].y);
		pt[2] = CPoint(pt[1].x, rcVol.bottom);
		pt[3] = CPoint(pt[0].x, pt[2].y);		
		pt[4] = pt[0];
		pDC->Polygon(pt, 5);
		nStart += width[ii];
	}
	
	LIB_delete(width);
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
	drawTICK(pDC, true);
}

void CPreviewWnd::drawEQV(CDC* pDC)
{
	CRect	rcAll = m_rectGraph;
	CRect	rcCandle, rcVol;
	int	nWidth = rcAll.Height() / 3;
	double*	width = LIB_new((double*)NULL, m_iDataCount);
	calcuVolWidth(width);

	rcVol = rcCandle = rcAll;
	rcVol.top = rcVol.bottom - nWidth;
	rcCandle.bottom = rcVol.top - 2;
		
	CPen*	oldpen = selectPen(pDC, &m_pGraph->cInfo.aline[1]);
	CBrush*	oldbrush = selectBrush(pDC, m_pGraph->cInfo.aline[1].rgbLColor);
	CPoint	pt[5];
	double	nStart = m_rectGraph.left;
	bool	bUp = false;

	m_rectGraph = rcCandle;
	
	
	for ( int ii = 0 ; ii < m_iDataCount ; ii++ )
	{
		pt[0] = pt[4] = CPoint(int(nStart + 1.0), getYPos(g_abongdata[ii].koga));
		pt[1] = CPoint(int((double)pt[0].x + width[ii] - 2.0), pt[0].y);
		pt[2] = CPoint(pt[1].x, getYPos(g_abongdata[ii].jega));
		pt[3] = CPoint(pt[0].x, pt[2].y);
						
		if (g_abongdata[ii].curr > g_abongdata[ii].siga)
		{
			bUp = true;
			selectPen(pDC, &m_pGraph->cInfo.aline[1]);
			selectBrush(pDC, m_pGraph->cInfo.aline[1].rgbLColor);
		}
		else
		{
			bUp = false;
			selectPen(pDC, &m_pGraph->cInfo.aline[2]);
			selectBrush(pDC, m_pGraph->cInfo.aline[2].rgbLColor);
		}
		
		if (bUp)
		{
			if (m_pGraph->dwDraOption & GO_FIILUPBONG)
				pDC->Polygon(pt, 5);
			else
				pDC->Polyline(pt, 5);
		}
		else
		{
			if (m_pGraph->dwDraOption & GO_FIILDNBONG)
				pDC->Polygon(pt, 5);
			else
				pDC->Polyline(pt, 5);
		}

		nStart += width[ii];
	}

	pt[0] = CPoint(m_rectRegion.left, rcCandle.bottom + 2);
	pt[1] = CPoint(m_rectRegion.right, rcCandle.bottom + 2);
	struct _line line;
	line.rgbLColor = RGB(0, 0, 0);
	line.btLStyle = PS_SOLID;
	line.btLWidth = 1;
	drawWidthLine(pDC, pt, &line);
	drawTICK(pDC);

	m_dMinValue = 0;
	double	tmpMin = DBL_MAX, tmpMax = DBL_MIN;
	double	val = 0.0;

	selectPen(pDC, RGB(84,164,5));
	selectBrush(pDC, RGB(84,164,5));
	
	for ( int ii = 0 ; ii < m_iDataCount ; ii++ )
	{
		val = g_abongdata[ii].vol;

		if (val > tmpMax)
			tmpMax = val;
	}

	m_dMaxValue = tmpMax;
	rcVol.DeflateRect(0, 2, 0, 2);

	m_rectGraph = rcVol;

	nStart = rcVol.left;
	for ( int ii = 0 ; ii < m_iDataCount ; ii++ )
	{
		pt[0] = CPoint(int(nStart + 1.0), getYPos(g_abongdata[ii].vol));
		pt[1] = CPoint(int((double)pt[0].x + width[ii] - 2.0), pt[0].y);
		pt[2] = CPoint(pt[1].x, rcVol.bottom);
		pt[3] = CPoint(pt[0].x, pt[2].y);		
		pt[4] = pt[0];
		pDC->Polygon(pt, 5);
		nStart += width[ii];
	}
	
	LIB_delete(width);
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
	drawTICK(pDC, true);
}

void CPreviewWnd::drawJPN(CDC* pDC, COLORREF clrUp, COLORREF clrDown)
{
	CPen*	oldpen = selectPen(pDC, clrUp);
	CBrush*	oldbrush = selectBrush(pDC, clrUp);
	CPoint	pt[11];
	bool	bUp = false;
	int	nCount = m_iDataCount;
	DWORD	option = m_pGraph->dwDraOption;

	if (m_pGraph->wGKind == GK_BALANCE)
	{
		option = m_pBong->dwDraOption;
		nCount = m_iDataCount / 2;
	}
	
	for ( int ii = 0 ; ii < nCount ; ii++ )
	{
		pt[0] = pt[10] = CPoint(getXPosM(ii), getYPos(g_abongdata[ii].koga));
		
		if (g_abongdata[ii].curr > g_abongdata[ii].siga)
		{
			bUp = true;
			selectPen(pDC, clrUp);
			selectBrush(pDC, clrUp);
			pt[1] = CPoint(pt[0].x, getYPos(g_abongdata[ii].curr));
			pt[4] = CPoint(pt[0].x, getYPos(g_abongdata[ii].siga));
		}
		else
		{
			bUp = false;
			selectPen(pDC, clrDown);
			selectBrush(pDC, clrDown);
			pt[4] = CPoint(pt[0].x, getYPos(g_abongdata[ii].curr));
			pt[1] = CPoint(pt[0].x, getYPos(g_abongdata[ii].siga));
		}

		pt[2] = CPoint(getXPosL(ii) + 1, pt[1].y);
		pt[3] = CPoint(pt[2].x, pt[4].y);
		pt[5] = CPoint(pt[0].x, getYPos(g_abongdata[ii].jega));
		pt[6] = pt[4];
		pt[7] = CPoint(getXPosL(ii + 1) - 1, pt[4].y);
		pt[8] = CPoint(pt[7].x, pt[1].y);
		pt[9] = pt[1];
		
		if (bUp)
		{
			if (option & GO_FIILUPBONG)
				pDC->Polygon(pt, 11);
			else
				pDC->Polyline(pt, 11);
		}
		else
		{
			if (option & GO_FIILDNBONG)
				pDC->Polygon(pt, 11);
			else
				pDC->Polyline(pt, 11);
		}
		
	}

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}
void CPreviewWnd::drawPOLE(CDC* pDC, COLORREF clrUp, COLORREF clrDown)
{
	CPen*	oldpen = selectPen(pDC, clrUp);	
	CPoint	pt[6];
	
	for ( int ii = 0 ; ii < m_iDataCount ; ii++ )
	{
		pt[0] = CPoint(getXPosM(ii), getYPos(g_abongdata[ii].koga));
		pt[1] = CPoint(pt[0].x, getYPos(g_abongdata[ii].jega));
		
		if (g_abongdata[ii].curr > g_abongdata[ii].siga)
			selectPen(pDC, clrUp);			
		else
			selectPen(pDC, clrDown);		
		
		pDC->MoveTo(pt[0]);
		pDC->LineTo(pt[1]);	
	}

	pDC->SelectObject(oldpen);
}

void CPreviewWnd::drawBAR(CDC* pDC, COLORREF clrUp, COLORREF clrDown)
{
	CPen*	oldpen = selectPen(pDC, clrUp);	
	CPoint	pt[6];
	
	for ( int ii = 0 ; ii < m_iDataCount ; ii++ )
	{
		pt[0] = CPoint(getXPosM(ii), getYPos(g_abongdata[ii].koga));
		pt[1] = CPoint(pt[0].x, getYPos(g_abongdata[ii].jega));
		pt[2] = CPoint(getXPosL(ii) + 1, getYPos(g_abongdata[ii].siga));
		pt[3] = CPoint(pt[0].x, pt[2].y);
		pt[4] = CPoint(pt[0].x, getYPos(g_abongdata[ii].curr));
		pt[5] = CPoint(getXPosL(ii + 1) - 1, pt[4].y);
		
		if (g_abongdata[ii].curr > g_abongdata[ii].siga)
		{
			selectPen(pDC, clrUp);			
		}
		else
		{
			selectPen(pDC, clrDown);		
		}

		pDC->MoveTo(pt[0]);
		pDC->LineTo(pt[1]);

		pDC->MoveTo(pt[2]);
		pDC->LineTo(pt[3]);

		pDC->MoveTo(pt[4]);
		pDC->LineTo(pt[5]);
	}

	pDC->SelectObject(oldpen);
}

void CPreviewWnd::drawLIN(CDC* pDC, struct _line* lineinfo)
{
	CPen*	oldpen = selectPen(pDC, lineinfo);	
	CPoint*	pt = LIB_new((CPoint*)NULL, m_iDataCount + 1);
	
	for ( int ii = 0 ; ii < m_iDataCount ; ii++ )
	{
		pt[ii] = CPoint(getXPosM(ii), getYPos(g_abongdata[ii].curr));
	}

	pDC->Polyline(pt, m_iDataCount);
	LIB_delete(pt);
	pDC->SelectObject(oldpen);
}

/*************************************************************************************************/
// fill rgn
/*************************************************************************************************	
	CPen*	oldpen = selectPen(pDC, lineinfo);	
	CPoint*	pt = LIB_new((CPoint*)NULL, m_iDataCount + 3);
	
	for ( int ii = 0 ; ii < m_iDataCount ; ii++ )
	{
		pt[ii] = CPoint(getXPosM(ii), getYPos(g_abongdata[ii].curr));
	}

	int	xPos[2];
	xPos[0] = getXPosM(ii - 1);
	pt[m_iDataCount] = CPoint(xPos[0], m_rectGraph.bottom);
	xPos[1]  = getXPosM(0);
	pt[m_iDataCount + 1] = CPoint(xPos[1], m_rectGraph.bottom);
	pt[m_iDataCount + 2] = pt[0];
	pDC->Polyline(pt, m_iDataCount);


	CRgn	rgn1, rgn2, rgnDraw;
	
	rgn1.CreatePolygonRgn(pt, m_iDataCount + 3, WINDING);

	CRect	rect;
	rect.SetRect(xPos[1], m_rectGraph.top, xPos[0], (m_rectGraph.top + m_rectGraph.bottom) / 3 );
	rgn2.CreateRectRgnIndirect(rect);

	rgn1.CombineRgn(&rgn1, &rgn2, RGN_AND);

	CBrush	brush(RGB(255, 255, 0));
	pDC->FillRgn(&rgn1, &brush);

	rgn1.DeleteObject();
	rgn2.DeleteObject();

	pt[m_iDataCount] = CPoint(xPos[0], m_rectGraph.top);
	pt[m_iDataCount + 1] = CPoint(xPos[1], m_rectGraph.top);

	rgn1.CreatePolygonRgn(pt, m_iDataCount + 3, WINDING);
	rect.SetRect(xPos[1], (m_rectGraph.top + m_rectGraph.bottom) / 3 * 2, xPos[0], m_rectGraph.bottom);
	rgn2.CreateRectRgnIndirect(rect);
	rgn1.CombineRgn(&rgn1, &rgn2, RGN_AND);
	
	brush.DeleteObject();
	brush.CreateSolidBrush(RGB(0, 255, 255));
	pDC->FillRgn(&rgn1, &brush);
	rgn1.DeleteObject();
	rgn2.DeleteObject();
	brush.DeleteObject();
	LIB_delete(pt);
	pDC->SelectObject(oldpen);
*************************************************************************************************/

void CPreviewWnd::drawSALE(CDC* pDC)
{
	CRect	rcAll = m_rectGraph;
	CRect	rcCandle, rcVol;
	int	nWidth = rcAll.Height() / 3, ii = 0;
	
	rcCandle = rcAll;	
	
	CPen*	oldpen = selectPen(pDC, &m_pGraph->cInfo.aline[0]);
	CBrush*	oldbrush = selectBrush(pDC, m_pGraph->cInfo.aline[0].rgbLColor);

	CPoint	pt[5];
	double	nStart = m_rectGraph.left;

	drawMaemul(pDC, rcCandle);
	
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}

void CPreviewWnd::drawPNF(CDC* pDC)
{
	CPen*	oldpen = selectPen(pDC, &m_pGraph->cInfo.aline[0]);
	CBrush*	oldbrush = selectBrush(pDC, m_pGraph->cInfo.aline[0].rgbLColor);
	CPoint	pt;
	CRect	rect;
	int	jj = 0;
	int	nPos = m_rectGraph.bottom;
	int	nCount[20] = {3, 7, 4, 8, 5, 8, 2, 3, 4, 6,
				10, 5, 3, 4, 8, 7, 2, 3, 8, 9};

	
	for ( int ii = 0 ; ii < m_iDataCount ; ii++ )
	{
		pt.y = getYPos(g_abongdata[ii].koga);
		pt.x = getXPosL(ii);

		if ((ii % 2) == 0)
		{
			selectPen(pDC, &m_pGraph->cInfo.aline[0]);
			selectBrush(pDC, m_pGraph->cInfo.aline[0].rgbLColor);
			rect.left = pt.x;
			rect.top = pt.y;

			for ( jj = 0 ; jj < nCount[ii] ; jj++ )
			{
				rect.bottom = rect.top + (int)m_dUnit;
				rect.right = rect.left + (int)m_dUnit;

				if (rect.bottom < nPos)
				{
					pDC->MoveTo(rect.TopLeft());
					pDC->LineTo(rect.BottomRight());
					pDC->MoveTo(rect.right, rect.top);
					pDC->LineTo(rect.left, rect.bottom);
				}
				else
					break;
				rect.top = rect.bottom;
			}
			
		}
		else
		{
			selectPen(pDC, &m_pGraph->cInfo.aline[1]);
			selectBrush(pDC, m_pGraph->cInfo.aline[1].rgbLColor);

			rect.left = pt.x;
			rect.top = pt.y;

			for ( jj = 0 ; jj < nCount[ii] ; jj++ )
			{
				rect.bottom = rect.top + (int)m_dUnit;
				rect.right = rect.left + (int)m_dUnit;

				if (rect.bottom < nPos)
				{
					pDC->Ellipse(rect);
				}
				else
					break;
				rect.top = rect.bottom;
			}
			
		}
	}

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}

void CPreviewWnd::drawTHREE(CDC* pDC)
{
	CPen*	pen = selectPen(pDC, m_pGraph->cInfo.aline[1].rgbLColor);
	CBrush*	brush = selectBrush(pDC, m_pGraph->cInfo.aline[1].rgbLColor);
	CRect	rect[13]{};
	int	nUpDown[13] = {0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1};

	int	nWidth = m_rectGraph.Height();
	int	nDWidth[5];
	
	for ( int ii = 0 ; ii < 5 ; ii++ )
	{
		nDWidth[ii] = nWidth / ((ii + 1) * 2);
	}

	for ( int ii = 0 ; ii < 13 ; ii++ )
	{
		rect[ii].left = getXPosL(ii);
		rect[ii].right = rect[ii].left + (int)m_dUnit;
	}

	rect[0].bottom = m_rectGraph.bottom - nDWidth[4];
	rect[0].top = m_rectGraph.bottom - nDWidth[0];

	rect[1].bottom = rect[0].top;
	rect[1].top = rect[1].bottom - nDWidth[4];

	rect[2].bottom = rect[1].top;
	rect[2].top = rect[2].bottom - nDWidth[3];

	rect[3].top = rect[2].bottom;
	rect[3].bottom = rect[3].top + nDWidth[2];

	rect[4].bottom = rect[3].top;
	rect[4].top = rect[4].bottom - nDWidth[4];
	
	rect[5].bottom = rect[4].top;
	rect[5].top = rect[5].bottom - nDWidth[1];

	rect[6].top = rect[5].bottom;
	rect[6].bottom = rect[6].top + nDWidth[3];

	rect[7].top = rect[6].bottom;
	rect[7].bottom = rect[7].top + nDWidth[2];

	rect[8].top = rect[7].bottom;
	rect[8].bottom = rect[8].top + nDWidth[4];

	rect[9].top = rect[8].bottom;
	rect[9].bottom = rect[9].top + nDWidth[2];

	rect[10].bottom = rect[9].top;
	rect[10].top = rect[10].bottom - nDWidth[1];

	rect[11].bottom = rect[10].top;
	rect[11].top = rect[11].bottom - nDWidth[2];

	rect[12].bottom = rect[11].top;
	rect[12].top = rect[12].bottom - nDWidth[2];


	CPoint	pt[5];
	for ( int ii = 0 ; ii < 13 ; ii++ )
	{
		selectBrush(pDC, RGB(0, 0, 0));
		
		pt[4] = pt[0] = CPoint(rect[ii].left, rect[ii].bottom);
		pt[1] = CPoint(rect[ii].right, rect[ii].bottom);
		pt[2] = CPoint(rect[ii].right, rect[ii].top); 
		pt[3] = CPoint(rect[ii].left, rect[ii].top);

		if (nUpDown[ii] == 0)
		{
			selectPen(pDC, m_pGraph->cInfo.aline[1].rgbLColor);
			selectBrush(pDC, m_pGraph->cInfo.aline[1].rgbLColor);

			if (m_pGraph->dwDraOption & GO_FIILDNBONG)
				pDC->Polygon(pt, 5);
			else
				pDC->Polyline(pt, 5);
		}
		else
		{
			selectPen(pDC, m_pGraph->cInfo.aline[0].rgbLColor);
			selectBrush(pDC, m_pGraph->cInfo.aline[0].rgbLColor);

			if (m_pGraph->dwDraOption & GO_FIILUPBONG)
				pDC->Polygon(pt, 5);
			else
				pDC->Polyline(pt, 5);
		}
	}

	pDC->SelectObject(pen);
	pDC->SelectObject(brush);
}

int CPreviewWnd::getXPosL(int nIndex)
{
	return (int)(m_dUnit * (double)nIndex + (double)m_rectGraph.left);
}

int CPreviewWnd::getXPosM(int nIndex)
{
	double	xPos = getXPosL(nIndex);
	double	xPos1 = getXPosR(nIndex);

	xPos = (xPos + xPos1) / 2.0;
	return (int)xPos;
}

int CPreviewWnd::getXPosR(int nIndex)
{
	double	xPos = getXPosL(nIndex);
	xPos += m_dUnit;
	return (int)xPos;
}

void CPreviewWnd::calcuVolWidth(double* width)
{
	int	ii = 0;
	double	nVolTotal = 0;
	int	nWidth = m_rectGraph.Width();

	for ( ii = 0 ; ii < m_iDataCount ; ii++ )
	{
		nVolTotal += g_abongdata[ii].vol;
	}

	for ( ii = 0 ; ii < m_iDataCount ; ii++ )
	{
		width[ii] = (int)((double)g_abongdata[ii].vol * (double)nWidth/ (double)nVolTotal);
	}
}

void CPreviewWnd::drawMaemul(CDC* pDC, CRect rect)
{
	int	ii = 0;
	double	nTmpMax = DBL_MIN;
	int	nMaxIndex = 0, nCurIndex = 0, nIndex = 0;
	double	nTotVol = 0, fGab = 0.0, sum = 0.0;
	int	nSAR = (int)m_pGraph->cInfo.awValue[0];
	int	nViewType = (int)m_pGraph->cInfo.awValue[1];

	if (nSAR <= 0)
		return;

	double*	pTerm = LIB_new((double*)NULL, nSAR + 1); 
	double*	pMarket = LIB_new((double*)NULL, nSAR);
	double*	pVMarket = LIB_new((double*)NULL, nSAR);
	double* pCount = LIB_new((double*)NULL, nSAR);
	double	valPRICE = 0.0;

	LIB_memset(pCount, nSAR, 0.0);
	LIB_memset(pMarket, nSAR, 0.0);
	LIB_memset(pVMarket, nSAR, 0.0);
	LIB_memset(pTerm, nSAR + 1, 0.0);

	for ( ii = 0 ; ii < m_iDataCount ; ii++ )
	{
		if (g_abongdata[ii].vol > nTmpMax)
		{
			nTmpMax = g_abongdata[ii].vol;
		}

		nTotVol += g_abongdata[ii].vol;
	}

	if (nSAR >= 0)
		fGab = (m_dMaxValue - m_dMinValue) / (double)nSAR;
	else
		fGab = 0;

	for ( ii = 0 ; ii < nSAR + 1 ; ii++ )
	{
		pTerm[ii] = m_dMinValue + fGab * (double)ii;
	}



	for ( ii = 0 ; ii < m_iDataCount ; ii++ )
	{
		valPRICE = g_abongdata[ii].curr;
		nIndex = 0;

		for ( int jj = 0 ; jj < nSAR ; jj++ ) 
		{
			if (valPRICE >= pTerm[jj] && valPRICE <= pTerm[jj + 1])
				break;
			nIndex++;
		}

		pCount[nIndex] += g_abongdata[ii].vol;

	}		

	for ( ii = 0 ; ii < nSAR; ii++ )
	{
		sum += pCount[ii];
	}

	if (sum != 0.0)
	{
		nTmpMax = DBL_MIN;

		for ( ii = 0 ; ii < nSAR ; ii++ )
		{
			pVMarket[ii] = pCount[ii];
			pMarket[ii] = (pCount[ii] / sum) * 100.0;
			if (pVMarket[ii] >= nTmpMax)
			{
				nTmpMax = pVMarket[ii];
				nMaxIndex = ii;
			}
		}		
	}

	CRect	rcTmp;
	CString	dispStr;

	CPen	penSVR(PS_SOLID, 1, m_pGraph->cInfo.aline[0].rgbLColor);
	CPen	penCLOSE(PS_SOLID, 1, m_pGraph->cInfo.aline[0].rgbLColor);	
	CPen	penBIGSVR(PS_SOLID, 1, m_pGraph->cInfo.aline[0].rgbLColor);
	CPen	*oldpen = NULL;
	CBrush	brSVR(m_pGraph->cInfo.aline[0].rgbLColor);
	CBrush	brCLOSE(m_pGraph->cInfo.aline[0].rgbLColor);
	CBrush	brBIGSVR(m_pGraph->cInfo.aline[0].rgbLColor);
	CBrush	*oldbrush = NULL;

	oldpen = pDC->SelectObject(&penSVR);
	oldbrush = pDC->SelectObject(&brSVR);
	double	val0, val0_1, val1, val2;
	CPoint	pt;
	int	nMode = pDC->SetBkMode(TRANSPARENT);
	CFont	tkFont, *sFont = NULL;
	int	fHeight=0, fWidth=0;

	tkFont.CreatePointFont(6 * 10, "±¼¸²Ã¼");
	sFont = pDC->SelectObject(&tkFont);

	COLORREF	OrgtextCOLOR = pDC->SetTextColor(RGB(0, 0, 0));
	CSize	size, tmpSize;
	int	nType = (int)m_pGraph->cInfo.awValue[1];

	tmpSize = pDC->GetTextExtent("0");
	CString		tmpstr;

	for ( ii = 0 ; ii < nSAR ; ii++ )
	{
		val0 = pMarket[ii];
		val0_1 = pVMarket[ii];

		val1 = pTerm[ii];
		val2 = pTerm[ii + 1];

		if (val0 < 0 || val0_1 < 0)
			break;

		if (nType == 0)
		{
			dispStr.Format("%.f", val0_1);
			dispStr = LIB_MakeComma(dispStr);
		}
		else if (nType == 1)
		{
			dispStr.Format("%.2f %%", val0);
		}		
		else if (nType == 2)
		{
			dispStr.Format("%.f", val0_1);
			dispStr = LIB_MakeComma(dispStr);
			tmpstr = dispStr;
			dispStr.Format("%.2f %%", val0);
			dispStr = dispStr + "(" + tmpstr + ")";
		}
			
		size = pDC->GetTextExtent(dispStr);

		rcTmp.left = rect.left;
		rcTmp.bottom = getYPos(val1) - 1;
		rcTmp.top = getYPos(val2) + 1;
		rcTmp.right = getmarket(val0_1, rect, nTmpMax) + rect.left;

		pDC->SelectObject(&penSVR);
		pDC->SelectObject(&brSVR);
		pt.x = rcTmp.right + tmpSize.cx;
		pt.y = (int)((double)rcTmp.top + (double)(rcTmp.Height() / 2) - (double)(size.cy / 2));

		pDC->Rectangle(rcTmp);
		pDC->TextOut(pt.x, pt.y, dispStr);
	}

	pDC->SetTextColor(OrgtextCOLOR);
	pDC->SetBkMode(nMode);
	pDC->SelectObject(sFont);
	pDC->SelectObject(oldbrush);
	pDC->SelectObject(oldpen);
	brSVR.DeleteObject();
	brBIGSVR.DeleteObject();
	brCLOSE.DeleteObject();
	penSVR.DeleteObject();
	penCLOSE.DeleteObject();
	penBIGSVR.DeleteObject();
	tkFont.DeleteObject();
	

	LIB_delete(pTerm);
	LIB_delete(pMarket);
	LIB_delete(pVMarket);
	LIB_delete(pCount);
}

int CPreviewWnd::getmarket(double val, CRect rect, double nMax)
{
	int	nWidth = rect.Width();
	nWidth = nWidth * 4 / 5;
	
	double	xWidth = 0;

	if (val == 0 || nMax == 0)
		return 1;

	xWidth = (double)nWidth * val / nMax;
	return (int)xWidth;
}

int CPreviewWnd::getClock(double val, double nMax)
{
	int	nXpos = 0;
	int	nWidth = m_rectGraph.Width();

	nXpos = (int)(((double)nWidth * val) / nMax);
	return	nXpos + m_rectGraph.left;
}

void CPreviewWnd::drawBack(CDC* pDC)
{
	pDC->FillSolidRect(m_rectTotal, m_rgbTick);
	drawBox(pDC, m_rectRegion, m_pEnv->display.rgbChartEdge, m_pEnv->display.rgbChartBG, BOX_ALL);
}

void CPreviewWnd::drawTICK(CDC* pDC, bool bVol)
{
	double	gap = m_dMaxValue - m_dMinValue;
	int	val[] = {9300, 9400, 9500};
	int	vval[] = {0, 15000};	
	int	*pVal = NULL;
	int	nSize = 0;
	CPoint	pt;
	CString	tmpstr;	
	CFont	font, *oldfont = NULL;
	int	nMode = pDC->SetBkMode(TRANSPARENT);
	CSize	size;
	
	
	font.CreatePointFont(8 * 10, m_pEnv->display.atick[TICK_LEFT].tfont.acFName);
	oldfont = pDC->SelectObject(&font);
	CRect	rect;	

	if (bVol)
	{
		pVal = vval;
		nSize = sizeof(vval) / sizeof(int);
	}
	else
	{
		pVal = val;
		nSize = sizeof(val) / sizeof(int);
	}

	for ( int ii = 0 ; ii < nSize ; ii++ )
	{
		
		tmpstr.Format("%d ", pVal[ii]);
		size = pDC->GetTextExtent(tmpstr);
		rect.SetRect(m_arectTick[0].right - size.cx, 0, m_arectTick[0].right,	0);

		if (ii == 0)
		{
			rect.bottom = m_rectGraph.bottom;
			rect.top = rect.bottom - size.cy;
		}
		else if (ii == 1)
		{
			pt.y = (m_rectGraph.bottom + m_rectGraph.top) / 2;
			rect.bottom = pt.y;
			rect.top = rect.bottom - size.cy;
		}
		else if (ii == 2)
		{			
			rect.top = m_rectGraph.top;
			rect.bottom = rect.top - size.cy;
		}

		pDC->DrawText(tmpstr, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				
	}

	for ( int ii = 0 ; ii < nSize ; ii++ )
	{		
		tmpstr.Format(" %d", pVal[ii]);
		size = pDC->GetTextExtent(tmpstr);
		rect = CRect(m_arectTick[1].left, 0, m_arectTick[1].left + size.cx, 0);

		if (ii == 0)
		{
			rect.bottom = m_rectGraph.bottom;
			rect.top = rect.bottom - size.cy;
		}
		else if (ii == 1)
		{
			pt.y = (m_rectGraph.bottom + m_rectGraph.top) / 2;
			rect.bottom = pt.y;
			rect.top = rect.bottom - size.cy;
		}
		else if (ii == 2)
		{			
			rect.top = m_rectGraph.top;
			rect.bottom = rect.top - size.cy;
		}

		pDC->DrawText(tmpstr, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				
	}

	if (!bVol)
	{
		CStringArray	arData;

		if (m_pGraph->wGKind != GK_PV && m_pGraph->wGKind != GK_CLOCK)
		{
			arData.Add("2003/03");
			arData.Add("04");
			arData.Add("05");
			arData.Add("06");
			arData.Add("07");
		}
		else
		{
			arData.Add("1000");
			arData.Add("2000");
			arData.Add("3000");
			arData.Add("4000");		
		}

		nSize = arData.GetSize();
		int	nWidth = m_rectGraph.Width();

		nWidth /= nSize;
		CRect	pRect;

		for ( int ii = 0 ; ii < nSize ; ii++ )
		{
			tmpstr = arData.GetAt(ii);
			size = pDC->GetTextExtent(tmpstr);		

			rect.SetRect(m_rectGraph.left + (ii * nWidth),
					m_arectTick[2].top + 2, 
					m_rectGraph.left + (ii * nWidth) + size.cx,
					m_arectTick[2].top + 2 + size.cy);

			if (ii > 0)
			{
				if (rect.left < pRect.right)
					continue;
			}
			
			pDC->DrawText(tmpstr, rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
			pRect = rect;
		}
	}

	pDC->SelectObject(oldfont);
	font.DeleteObject();
	pDC->SetBkMode(nMode);
}

void CPreviewWnd::DrawUnionKagi(CDC* pDC, COLORREF clrUp, COLORREF clrDown)
{
	CPen*	oldpen = selectPen(pDC, clrUp);
	CPoint	pt[11];
	bool	bUp = false;
	int	nCount = m_iDataCount;
	DWORD	option = m_pGraph->dwDraOption;

	if (m_pGraph->wGKind == GK_BALANCE)
	{
		option = m_pBong->dwDraOption;
		nCount = m_iDataCount / 2;
	}
	
	pDC->MoveTo(CPoint(getXPosM(0), getYPos(g_abongdata[0].curr)));
	pDC->LineTo(CPoint(getXPosM(0), getYPos(g_abongdata[3].koga)));
	pDC->LineTo(CPoint(getXPosM(5), getYPos(g_abongdata[3].koga)));
	selectPen(pDC, clrDown);
	pDC->LineTo(CPoint(getXPosM(5), getYPos(g_abongdata[5].jega)));
	selectPen(pDC, clrUp);
	pDC->LineTo(CPoint(getXPosM(8), getYPos(g_abongdata[5].jega)));
	pDC->LineTo(CPoint(getXPosM(8), getYPos(g_abongdata[12].koga)));
	pDC->LineTo(CPoint(getXPosM(8), getYPos(g_abongdata[12].koga)));
	pDC->LineTo(CPoint(getXPosM(13), getYPos(g_abongdata[12].koga)));
	selectPen(pDC, clrDown);
	pDC->LineTo(CPoint(getXPosM(13), getYPos(g_abongdata[18].jega)));
	pDC->LineTo(CPoint(getXPosM(19), getYPos(g_abongdata[18].jega)));
/*
	for ( int ii = 1 ; ii < nCount ; ii++ )
	{
		

		pt[0] = pt[10] = CPoint(getXPosM(ii), getYPos(g_abongdata[ii].koga));
		
		if (g_abongdata[ii].curr > g_abongdata[ii].siga)
		{
			bUp = true;
			selectPen(pDC, clrUp);
			selectBrush(pDC, clrUp);
			pt[1] = CPoint(pt[0].x, getYPos(g_abongdata[ii].curr));
			pt[4] = CPoint(pt[0].x, getYPos(g_abongdata[ii].siga));
		}
		else
		{
			bUp = false;
			selectPen(pDC, clrDown);
			selectBrush(pDC, clrDown);
			pt[4] = CPoint(pt[0].x, getYPos(g_abongdata[ii].curr));
			pt[1] = CPoint(pt[0].x, getYPos(g_abongdata[ii].siga));
		}

		pt[2] = CPoint(getXPosL(ii) + 1, pt[1].y);
		pt[3] = CPoint(pt[2].x, pt[4].y);
		pt[5] = CPoint(pt[0].x, getYPos(g_abongdata[ii].jega));
		pt[6] = pt[4];
		pt[7] = CPoint(getXPosL(ii + 1) - 1, pt[4].y);
		pt[8] = CPoint(pt[7].x, pt[1].y);
		pt[9] = pt[1];
		
		if (bUp)
		{
			if (option & GO_FIILUPBONG)
				pDC->Polygon(pt, 11);
			else
				pDC->Polyline(pt, 11);
		}
		else
		{
			if (option & GO_FIILDNBONG)
				pDC->Polygon(pt, 11);
			else
				pDC->Polyline(pt, 11);
		}
	}
*/	

	pDC->SelectObject(oldpen);
}

void CPreviewWnd::DrawUnionPnF(CDC* pDC, COLORREF clrUp, COLORREF clrDown)
{
	CPoint	tpoint;
	double	dOnePoint = 7.0;
	bool	bDir = true;
	bool	dDir = true;
	int	sidx, eidx;
	CRect	DrawR;

	double ePoint;
	double sPoint;
	double pnt;

	CPen*	oldpen = selectPen(pDC, clrUp);
	selectPen(pDC, clrUp);

	for (int ii = 0; ii < 4; ii++)
	{
		switch (ii)
		{
		case 0:
			sidx = 0;
			eidx = 4;
			break;
		case 1:
			sidx = 4;
			eidx = 7;
			break;
		case 2:
			sidx = 7;
			eidx = 12;
			break;
		case 3:
			sidx = 12;
			eidx = 19;
			break;
		}

		sPoint = getYPos(g_abongdata[sidx].jega);
		ePoint = getYPos(g_abongdata[eidx].koga);

		if (dDir)
		{
			selectPen(pDC, clrUp);
			pnt = sPoint - dOnePoint;
			
			tpoint.x = int(getXPosM(sidx));
			tpoint.y = int(sPoint);
			
			pDC->MoveTo(tpoint);
			
			for (; int(pnt) >= int(ePoint);)
			{
				if (bDir)
				{
					tpoint.x = int(getXPosM(eidx));
					tpoint.y = int(pnt);
					pDC->LineTo(tpoint);
				}
				else
				{
					tpoint.x = int(getXPosM(sidx));
					tpoint.y = int(pnt);
					pDC->LineTo(tpoint);
				}
				bDir = !bDir;
				pnt -= dOnePoint;
			}
			
			tpoint.x = int(getXPosM(eidx));
			tpoint.y = int(sPoint);
			
			pDC->MoveTo(tpoint);			
			
			bDir = true;
			pnt = sPoint - dOnePoint;
			
			for (; int(pnt) >= int(ePoint);)
			{
				if (bDir)
				{
					tpoint.x = int(getXPosM(sidx));
					tpoint.y = int(pnt);
					pDC->LineTo(tpoint);
				}
				else
				{
					tpoint.x = int(getXPosM(eidx));
					tpoint.y = int(pnt);
					pDC->LineTo(tpoint);
				}
				bDir = !bDir;
				pnt -= dOnePoint;
			}
		}
		else
		{
			selectPen(pDC, clrDown);
			pnt = sPoint + dOnePoint;

			if (ii == 1)
			{
				pnt = 35.;
				ePoint = 60.;
			}

			for (; int(pnt) <= int(ePoint);)
			{
				DrawR = CRect(getXPosM(sidx), int(pnt - dOnePoint) + 1,
					int(getXPosM(eidx)), int(pnt));

				pDC->SelectObject((HBRUSH)GetStockObject(NULL_BRUSH));
				pDC->Ellipse(&DrawR);
				pnt += dOnePoint;
			}
		}

		dDir = !dDir;
	}
}

void CPreviewWnd::DrawUnionSwing(CDC* pDC, COLORREF clrUp, COLORREF clrDown)
{
	CPen*	oldpen = selectPen(pDC, clrUp);
	selectPen(pDC, clrUp);
	pDC->MoveTo(getXPosM(0),getYPos(g_abongdata[0].curr));
	pDC->LineTo(getXPosM(0),getYPos(g_abongdata[3].koga));
	pDC->LineTo(getXPosM(5),getYPos(g_abongdata[3].koga));
	pDC->LineTo(getXPosM(5),getYPos(g_abongdata[5].jega));
	pDC->LineTo(getXPosM(8),getYPos(g_abongdata[5].jega));
	pDC->LineTo(getXPosM(8),getYPos(g_abongdata[14].koga));
	pDC->LineTo(getXPosM(15),getYPos(g_abongdata[14].koga));
	pDC->LineTo(getXPosM(15),getYPos(g_abongdata[19].jega));
	pDC->LineTo(getXPosM(19),getYPos(g_abongdata[19].jega));
}

void CPreviewWnd::DrawUnionSamsun(CDC* pDC, COLORREF clrUp, COLORREF clrDown)
{
	CPen*	oldpen = selectPen(pDC, clrUp);
	selectPen(pDC, clrUp);
	DrowRect(pDC, getXPosM(0), getXPosM(5), getYPos(g_abongdata[0].curr), getYPos(g_abongdata[3].koga));
	selectPen(pDC, clrDown);
	DrowRect(pDC, getXPosM(5),getXPosM(8), getYPos(g_abongdata[0].curr),getYPos(g_abongdata[5].jega));
	selectPen(pDC, clrUp);
	DrowRect(pDC, getXPosM(8),getXPosM(14),	getYPos(g_abongdata[0].curr),getYPos(g_abongdata[14].koga));
	selectPen(pDC, clrDown);
	DrowRect(pDC, getXPosM(14),getXPosM(14), getYPos(g_abongdata[0].curr),getYPos(g_abongdata[15].koga));
	DrowRect(pDC, getXPosM(14),getXPosM(19), getYPos(g_abongdata[15].koga),getYPos(g_abongdata[19].jega));
}

void CPreviewWnd::DrowRect(CDC* pDC, int x1,int x2,int y1,int y2)
{
	pDC->MoveTo(x1, y1);
	pDC->LineTo(x1, y2);
	pDC->LineTo(x2, y2);
	pDC->LineTo(x2, y1);
	pDC->LineTo(x1, y1);
}

void CPreviewWnd::DrawUnionRenko(CDC* pDC, COLORREF clrUp, COLORREF clrDown)
{
	CPen* oldpen = selectPen(pDC, clrUp);
	selectPen(pDC, clrUp);
	DrowRect(pDC, getXPosM(0), getXPosM(2), getYPos(g_abongdata[0].curr), getYPos(g_abongdata[0].koga));
	DrowRect(pDC, getXPosM(2), getXPosM(5), getYPos(g_abongdata[0].koga), getYPos(g_abongdata[3].koga));
	selectPen(pDC, clrDown);
	DrowRect(pDC, getXPosM(5),getXPosM(6), getYPos(g_abongdata[6].koga),getYPos(g_abongdata[5].jega));
	DrowRect(pDC, getXPosM(6),getXPosM(8), getYPos(g_abongdata[0].koga),getYPos(g_abongdata[6].koga));
	selectPen(pDC, clrUp);
	DrowRect(pDC, getXPosM(8),getXPosM(14), getYPos(g_abongdata[0].koga),getYPos(g_abongdata[14].koga));
	selectPen(pDC, clrDown);
	DrowRect(pDC, getXPosM(14),getXPosM(14), getYPos(g_abongdata[0].curr),getYPos(g_abongdata[15].koga));
	DrowRect(pDC, getXPosM(14),getXPosM(15), getYPos(g_abongdata[15].koga),getYPos(g_abongdata[15].jega));
	DrowRect(pDC, getXPosM(15),getXPosM(19), getYPos(g_abongdata[15].jega),getYPos(g_abongdata[19].jega));
}

void CPreviewWnd::DrawUnionGukki(CDC* pDC, COLORREF clrUp, COLORREF clrDown)
{
	CPen*	oldpen = selectPen(pDC, clrUp);

	selectPen(pDC, clrUp);
	pDC->MoveTo(getXPosM(0),getYPos(g_abongdata[0].jega));
	selectPen(pDC, clrUp);
	pDC->LineTo(getXPosM(4),getYPos(g_abongdata[4].koga));
	selectPen(pDC, clrDown);
	pDC->LineTo(getXPosM(5),getYPos(g_abongdata[5].jega));
	selectPen(pDC, clrUp);
	pDC->LineTo(getXPosM(12),getYPos(g_abongdata[12].koga));
	selectPen(pDC, clrDown);
	pDC->LineTo(getXPosM(18),getYPos(g_abongdata[19].koga));
	selectPen(pDC, clrUp);
}

void CPreviewWnd::DrawUnionAVol(CDC* pDC, COLORREF clrUp, COLORREF clrDown)
{
	CRect rt = m_rectRegion;
	CRect trt;

	CFont	font, *oldfont = NULL;

	int	nMode = pDC->SetBkMode(TRANSPARENT);

	font.CreatePointFont(6* 10, "±¼¸²Ã¼");
	oldfont = pDC->SelectObject(&font);

	pDC->FillSolidRect(rt.left, rt.top + 17, rt.left + 100, rt.top + 17, RGB(210, 210 ,210));
	trt.SetRect(rt.left, rt.top + 17, rt.left + 130, rt.top + 37);
	pDC->DrawText("21,303", trt, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);

	pDC->FillSolidRect(rt.left, rt.top + 37, rt.left + 80, rt.top + 17, RGB(210, 210 ,210));
	trt.SetRect(rt.left, rt.top + 37, rt.left + 110, rt.top + 57);
	pDC->DrawText("12,125", trt, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);

	pDC->FillSolidRect(rt.left, rt.top + 57, rt.left + 130, rt.top + 17, RGB(210, 210 ,210));
	trt.SetRect(rt.left, rt.top + 57, rt.left + 160, rt.top + 77);
	pDC->DrawText("33,954", trt, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);

	pDC->FillSolidRect(rt.left, rt.top + 77, rt.left + 60, rt.top + 17, RGB(210, 210 ,210));
	trt.SetRect(rt.left, rt.top + 77, rt.left + 90, rt.top + 97);
	pDC->DrawText("9,543", trt, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
	
	pDC->SetBkMode(nMode);
	font.DeleteObject();
}

void CPreviewWnd::SetUnion(WORD wUnion)
{
	m_wUnionKind = wUnion;
	drawBitmap();
	Invalidate();
}