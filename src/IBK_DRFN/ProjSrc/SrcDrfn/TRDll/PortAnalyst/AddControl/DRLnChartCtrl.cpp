/************************************************/
/*																		          */
/*							Line Chart Control							*/
/*                                              */
/*																		          */
/*							작성일 : 2006. 07 ~							*/
/*						작성자 : 우동우 (Doori co.)		    */
/*																		          */
/************************************************/

#include "stdafx.h"
#include "DrLnChartCtrl.h"

#include "Windows.h"

#include "math.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif

#define FONT_NAME  "굴림체"
#define FONT_SIZE  11

/////////////////////////////////////////////////////////////////////
// CDrLnChartCtrl class
BEGIN_MESSAGE_MAP(CDrLnChartCtrl, CStatic)
	//{{AFX_MSG_MAP(CDrLnChartCtrl)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDrLnChartCtrl::CDrLnChartCtrl()
{
	m_dYMax = -99999999;
	m_dYMin = 99999999;
	
	m_clrCanvas = RGB(255, 255, 255);
}

CDrLnChartCtrl::~CDrLnChartCtrl()
{
	SetDeleteAll();
}

/////////////////////////////////////////////////////////////////////////////
void CDrLnChartCtrl::OnPaint() 
{
	CRect reClient;
	GetClientRect(reClient);
	
	CPaintDC dc(this);  // device context for painting
	SetDraw_Chart(dc.m_hDC, reClient);
#ifndef _DEBUG
	dc.DeleteDC();
#endif
}

void CDrLnChartCtrl::CopyToClipboard()
{
	CBitmap         bitmap;
	CClientDC       dc(this);
	CDC             memDC;
	CRect           rect;
	
	memDC.CreateCompatibleDC(&dc); 
	
	GetClientRect(rect);
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(),rect.Height() );
	////////////////////////////////////////////////////////////////////
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0, 0, rect.Width(),rect.Height(), &dc, 0, 0, SRCCOPY); 
	
	OpenClipboard();
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, bitmap.GetSafeHandle());
	CloseClipboard();
	
	memDC.SelectObject(pOldBitmap);
	bitmap.Detach();
	
	memDC.DeleteDC();
	bitmap.DeleteObject();	
}

void CDrLnChartCtrl::SetInit()
{
	m_nXMask_OffSet = 50;
	m_nLegend_OffSet = 20;
	
	m_dYMax = -99999999;
	m_dYMin = 99999999;
	
	m_clrCanvas = RGB(255, 255, 255);
	
	SetDeleteAll();
}

BOOL CDrLnChartCtrl::SetAdd_Series(int nSeriesID, CString strLengend, COLORREF clrLine)
{
	//# cheak SeriesID
	BOOL bSeriesIDChk = FALSE;
	
	POSITION pos;
	Series  stSeries;
	for (int nIndex = 0; nIndex < m_listSeries.GetCount(); nIndex++)
	{
		pos = m_listSeries.FindIndex(nIndex);
		stSeries = m_listSeries.GetAt(pos);
		
		if (nSeriesID == stSeries.nSeriesID)
		{
			bSeriesIDChk = TRUE;
			break;
		}
	}
	
	if (bSeriesIDChk == TRUE)
		return FALSE;
	
	stSeries.bView = TRUE;
	stSeries.nSeriesID = nSeriesID;
	stSeries.strLengend = strLengend;
	stSeries.clrLine = clrLine;
	
	stSeries.strDataType = "double";
	stSeries.strMask = "%.2f";
	
	stSeries.psiYIndex = new CUIntArray;
	stSeries.psaYData = new CStringArray;
	
	m_listSeries.AddTail(stSeries);
	
	return TRUE;
}

BOOL CDrLnChartCtrl::SetModify_Series(int nSeriesID, CString strLengend)
{
	//# cheak SeriesID
	BOOL bSeriesIDChk = FALSE;
	
	POSITION pos;
	Series  stSeries;
	for (int nIndex = 0; nIndex < m_listSeries.GetCount(); nIndex++)
	{
		pos = m_listSeries.FindIndex(nIndex);
		stSeries = m_listSeries.GetAt(pos);
		
		if (nSeriesID == stSeries.nSeriesID)
		{
			bSeriesIDChk = TRUE;
			break;
		}
	}
	
	if (bSeriesIDChk == TRUE)
	{
		stSeries.strLengend = strLengend;
		m_listSeries.SetAt(pos, stSeries);
	}
	
	return TRUE;
}

BOOL CDrLnChartCtrl::SetSeriesShow(int nSeriesID, BOOL bShow)
{
	BOOL bResult = FALSE;
	
	POSITION posSeries;
	Series stSeries;
	posSeries = GetSeries(nSeriesID, stSeries);
	if (posSeries != NULL)
	{
		stSeries.bView = bShow;
		m_listSeries.SetAt(posSeries, stSeries);
		
		SetMaxMin_YVal();
		
		bResult = TRUE;
		
		CRect reClient;
		GetClientRect(reClient);
		
		CPaintDC dc(this);
		SetDraw_Chart(dc.m_hDC, reClient);
#ifndef _DEBUG
		dc.DeleteDC();
#endif
	}
	
	return bResult;
}

BOOL CDrLnChartCtrl::SetDelete_Series(int nSeriesID)
{
	BOOL bResult = FALSE;
	
	POSITION posSeries;
	Series stSeries;
	posSeries = GetSeries(nSeriesID, stSeries);
	if (posSeries != NULL)
	{
		stSeries.psiYIndex->RemoveAll();
		delete stSeries.psiYIndex;
		stSeries.psiYIndex = NULL;
		
		stSeries.psaYData->RemoveAll();
		delete stSeries.psaYData;
		stSeries.psaYData = NULL;
		
		m_listSeries.RemoveAt(posSeries);
		bResult = TRUE;
	}
	
	return bResult;
}

void CDrLnChartCtrl::SetClear_XData()
{
	m_listXData.RemoveAll();
}

BOOL CDrLnChartCtrl::SetClear_YData(int nSeriesID)
{
	BOOL bResult = FALSE;
	
	POSITION posSeries;
	Series stSeries;
	posSeries = GetSeries(nSeriesID, stSeries);
	if (posSeries != NULL)
	{
		stSeries.psiYIndex->RemoveAll();
		stSeries.psaYData->RemoveAll();
		
		m_listSeries.SetAt(posSeries, stSeries);
		bResult = TRUE;
	}
	
	return bResult;
}

POSITION CDrLnChartCtrl::GetSeries(int nSeriesID, Series &stSeries)
{
	POSITION pos;
	for (int nIndex = 0; nIndex < m_listSeries.GetCount(); nIndex++)
	{
		pos = m_listSeries.FindIndex(nIndex);
		stSeries = m_listSeries.GetAt(pos);
		
		if (nSeriesID == stSeries.nSeriesID)
			break;
	}
	
	return pos;
}

void CDrLnChartCtrl::SetClearAll()
{
	m_dYMax = -99999999;
	m_dYMin = 99999999;
	
	POSITION posSeries;
	Series  stSeries;
	for (int nIndex = 0; nIndex < m_listSeries.GetCount(); nIndex++)
	{
		posSeries = m_listSeries.FindIndex(nIndex);
		stSeries = m_listSeries.GetAt(posSeries);
		
		stSeries.psiYIndex->RemoveAll();
		stSeries.psaYData->RemoveAll();
		
		m_listSeries.SetAt(posSeries, stSeries);
	}
	
	m_listXData.RemoveAll();
	Invalidate();
}

void CDrLnChartCtrl::SetDeleteAll()
{
	POSITION posSeries;
	Series  stSeries;
	while (m_listSeries.GetCount() > 0)
	{
		posSeries = m_listSeries.FindIndex(0);
		stSeries = m_listSeries.GetAt(posSeries);
		
		stSeries.psiYIndex->RemoveAll();
		delete stSeries.psiYIndex;
		stSeries.psiYIndex = NULL;
		
		stSeries.psaYData->RemoveAll();
		delete stSeries.psaYData;
		stSeries.psaYData = NULL;
		
		m_listSeries.RemoveAt(posSeries);
	}
	
	if (m_listXData.GetCount() > 0)
		m_listXData.RemoveAll();
}

void CDrLnChartCtrl::SetAdd_XData(CString strXTitle, double dXData)
{
	XData  stXData;
	
	stXData.strXTitle = strXTitle;
	stXData.dXData = dXData;
	
	m_listXData.AddTail(stXData);
}

BOOL CDrLnChartCtrl::SetAdd_YData(int nSeriesID, int nDataIndex, double dYData)
{
	BOOL bResult = FALSE;
	
	POSITION posSeries;
	Series stSeries;
	posSeries = GetSeries(nSeriesID, stSeries);
	if (posSeries != NULL)
	{
		stSeries.psiYIndex->Add(nDataIndex);
		
		CString strData;
		strData.Format("%.2f", dYData);
		stSeries.psaYData->Add(strData);
		//SetMaxMin_YVal(dYData);
		
		m_listSeries.SetAt(posSeries, stSeries);
		bResult = TRUE;
	}
	
	return bResult;
}

void CDrLnChartCtrl::SetMaxMin_YVal(double dYData)
{
	if (m_dYMax < dYData)
		m_dYMax = dYData;
	
	if (m_dYMin > dYData)
		m_dYMin = dYData;
}

void CDrLnChartCtrl::SetMaxMin_YVal()
{
	m_dYMax = -99999999;
	m_dYMin = 99999999;
	
	POSITION pos;
	Series stSeries;
	int nIndex, nItem;
	double dYData;
	for (nIndex = 0; nIndex < m_listSeries.GetCount(); nIndex++)
	{
		pos = m_listSeries.FindIndex(nIndex);
		stSeries = m_listSeries.GetAt(pos);
		
		if (stSeries.bView == TRUE)
		{
		/*
		if (stSeries.psaYData != NULL)
		continue;
			*/
			
			for (nItem = 0; nItem < stSeries.psaYData->GetSize(); nItem++)
			{
				dYData = atof(stSeries.psaYData->GetAt(nItem));
				SetMaxMin_YVal(dYData);
			}
		}
	}
}

void CDrLnChartCtrl::Draw3dRect(HDC hDC, int x, int y, int cx, int cy, int nLH,
								COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	FillSolidRect(hDC, x, y, cx - nLH, nLH, clrTopLeft);
	FillSolidRect(hDC, x, y, nLH, cy - nLH, clrTopLeft);
	FillSolidRect(hDC, x + cx, y, -nLH, cy, clrBottomRight);
	FillSolidRect(hDC, x, y + cy, cx, -nLH, clrBottomRight);
}

void CDrLnChartCtrl::FillSolidRect(HDC hDC, int x, int y, int cx, int cy, COLORREF clr)
{
	ASSERT (hDC != NULL);
	SetBkColor(hDC, clr);
	CRect rect(x, y, x + cx, y + cy);
	ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
}

void CDrLnChartCtrl::SetDrawBox(HDC hDC, int x, int y, int cx, int cy, 
								COLORREF clr/*=RGB(0xFF, 0xFF, 0xFF)*/, 
								int nWidth/*=1*/)
{
	ASSERT (hDC != NULL);
	
	HPEN hOldPen, hPen = CreatePen(PS_SOLID, nWidth, clr);
	hOldPen = (HPEN)SelectObject (hDC, hPen);
	
	MoveToEx(hDC, x, y, NULL);
	LineTo (hDC, x, cy);
	
	MoveToEx(hDC, x, cy, NULL);
	LineTo (hDC, cx, cy);
	
	/*
	MoveToEx(hDC, x, y, NULL);
	LineTo (hDC, x + cx, y);
	LineTo (hDC, x + cx, y + cy);
	LineTo (hDC, x, y + cy);
	LineTo (hDC, x, y);
	*/
	
	SelectObject (hDC, hOldPen);
	::DeleteObject(hPen);
}

void CDrLnChartCtrl::SetDrawYGrid(HDC hDC)
{
	int nItemCnt = m_listXData.GetCount();
	if (nItemCnt < 1) 
		return;	
	
	HFONT hFont = SetMakeFont ((CString)FONT_NAME, (int)FONT_SIZE, FALSE);
	HFONT hOldFont = (HFONT)SelectObject (hDC, hFont);
	SetBkMode (hDC, TRANSPARENT);
	
	HPEN hOldPen, hPen = CreatePen(PS_SOLID, 1, RGB(192, 192, 192));
	hOldPen = (HPEN)SelectObject (hDC, hPen);
	
	div_t div_result;
	
	CString strTitle;
	int nX = m_ptBase.x, nY = m_ptBase.y;
	
	double dOffset_Title, dData_Offset, dData;
	dData_Offset = (m_dYMax - m_dYMin) * 0.2; 
	dOffset_Title = (m_dYMax - m_dYMin + dData_Offset * 2) / nItemCnt;
	
	m_nZeroPos = 0;
	
	BOOL bState = FALSE;
	double dBasePointVal = 0;
	int nBasePointY =0;
	
	for (int nIndex = 0; nIndex < nItemCnt; nIndex++) 
	{
		div_result = div(nIndex + 1, m_nTitleInterval_Y);
		
		if (div_result.rem > 0 && nItemCnt > m_nTitleInterval_Y*2)
			continue;
		
		nY = m_ptBase.y - (int)(nIndex * m_dYOffset);
		if (nIndex > 0)
		{
			MoveToEx(hDC, nX, nY, NULL);
			LineTo (hDC, m_reCanvas.right, nY);
		}
		
		dData = (m_dYMin - dData_Offset) + dOffset_Title * nIndex;
		if( bState == FALSE)
		{
			nBasePointY = nY;
			dBasePointVal = dData;
			
			bState = TRUE;
		}
		
		strTitle.Format("%.2f", (m_dYMin - dData_Offset) + dOffset_Title * nIndex);
		TextOut (hDC, nX - (int)(strTitle.GetLength() * (int)FONT_SIZE * 0.7), nY - 5, strTitle, strTitle.GetLength());
	}
	
	double dValToPixcel =0;
	dValToPixcel = m_dYOffset / dOffset_Title;
	m_nZeroPos = (int)((double)nBasePointY + dBasePointVal * dValToPixcel);
	
	SelectObject (hDC, hOldFont);
	SelectObject (hDC, hOldPen);
	
	::DeleteObject(hFont);
	::DeleteObject(hPen);
}

void CDrLnChartCtrl::SetDrawXGrid(HDC hDC)
{
	int nItemCnt = m_listXData.GetCount();
	if (nItemCnt < 1) 
		return;	
	
	HFONT hFont = SetMakeFont ((CString)FONT_NAME, (int)FONT_SIZE, TRUE);
	HFONT hOldFont = (HFONT)SelectObject (hDC, hFont);
	SetBkMode (hDC, TRANSPARENT);
	
	HPEN hOldPen, hPen = CreatePen(PS_SOLID, 1, RGB(192, 192, 192));
	hOldPen = (HPEN)SelectObject (hDC, hPen);
	
	div_t div_result;
	
	CString strTitle;
	int nX = m_ptBase.x, nY = m_ptBase.y;
	
	CRect reClient;
	GetClientRect(reClient);
	
	XData stXData;
	POSITION posXData;
	for (int nIndex = 0; nIndex < nItemCnt; nIndex++) 
	{
		posXData = m_listXData.FindIndex(nIndex);
		stXData = m_listXData.GetAt(posXData);
		
		div_result = div(nIndex + 1, m_nTitleInterval_X);
		
		if (div_result.rem > 0 && nItemCnt > m_nTitleInterval_X * 2)
			continue;
		
		nX = m_ptBase.x + (int)(nIndex * m_dXOffset);
		if (nIndex > 0)
		{
			MoveToEx(hDC, nX, m_reCanvas.top, NULL);
			LineTo (hDC, nX, nY);
		}
		
		strTitle = stXData.strXTitle;
		TextOut(hDC, nX - 5, reClient.bottom - 10, strTitle, strTitle.GetLength());
	}
	
	SelectObject (hDC, hOldFont);
	SelectObject (hDC, hOldPen);
	
	::DeleteObject(hFont);
	::DeleteObject(hPen);
}

void CDrLnChartCtrl::SetDrawLine(HDC hDC)
{
	int nItemCnt;
	nItemCnt = m_listXData.GetCount();
	
	int nOffset_Y, nOffset_X;
	if (nItemCnt <= 1) 
		return;	
	nOffset_Y = (int)(m_reCanvas.Height() / nItemCnt);		
	nOffset_X = (int)(m_reCanvas.Width() / (nItemCnt - 1));
	
	double dData_Offset;
	dData_Offset = (m_dYMax - m_dYMin) * 0.2;
	
	double dValToPixcel;
	double dOffset_Title = (m_dYMax - m_dYMin + dData_Offset * 2) / nItemCnt;
	dValToPixcel = nOffset_Y / dOffset_Title;
	
	int nIndex, nIndex_Data;
	Series  stSeries;
	POSITION posSeries;
	for (nIndex = 0; nIndex < m_listSeries.GetCount(); nIndex++)
	{
		posSeries = m_listSeries.FindIndex(nIndex);
		stSeries = m_listSeries.GetAt(posSeries);
		
		if (stSeries.bView == FALSE)
			continue;
		
		HPEN hOldPen, hPen = CreatePen(PS_SOLID, 1, stSeries.clrLine);
		hOldPen = (HPEN)SelectObject (hDC, hPen);
		SetBkMode (hDC, TRANSPARENT);
		
		double dData, dYVal;
		int nX = m_ptBase.x, nY = m_ptBase.y;
		MoveToEx(hDC, nX, nY, NULL);
		
		for (nIndex_Data = 0; nIndex_Data < nItemCnt; nIndex_Data++)
		{
			if (nIndex_Data > stSeries.psaYData->GetSize() - 1)
				break;
			
			if (stSeries.strDataType == "int")
				dData = atoi(stSeries.psaYData->GetAt(nIndex_Data));
			else if (stSeries.strDataType == "double")
				dData = atof(stSeries.psaYData->GetAt(nIndex_Data));
			
			dYVal = dData * dValToPixcel;
			nY = m_nZeroPos - (int)dYVal;
			nX = m_ptBase.x + nIndex_Data * nOffset_X;
			
			if (nIndex_Data > 0) 
				LineTo (hDC, nX, nY);
			MoveToEx(hDC, nX, nY, NULL);
		}
		
		SelectObject (hDC, hOldPen);
		::DeleteObject(hPen);
	}
}

void CDrLnChartCtrl::SetDrawCanvers(HDC hDC, CRect reClient, COLORREF crColor)
{
	CRect reBorder(reClient);
	FillSolidRect(hDC, reBorder.left, reBorder.top, reBorder.Width(), reBorder.Height(), crColor);
	Draw3dRect(hDC, reBorder.left, reBorder.top, reBorder.Width(), reBorder.Height(), 1,
		GetSysColor(COLOR_BTNHIGHLIGHT), GetSysColor(COLOR_BTNSHADOW)); 
	reBorder.InflateRect(1, 1);
	Draw3dRect(hDC, reBorder.left, reBorder.top, reBorder.Width(), reBorder.Height(), 1,
		GetSysColor (COLOR_BTNSHADOW), GetSysColor (COLOR_BTNHIGHLIGHT));
	
	int nX_Gap = (int)(reClient.Width() / 20),
		nY_Gap = (int)(reClient.Height() / 20);
	
	m_reLegend.top = reClient.top + 3;
	m_reLegend.bottom = m_reLegend.top + m_nLegend_OffSet - 3;
	m_reLegend.left = reClient.left + 3;
	m_reLegend.right = reClient.right - 3;
	
	m_reCanvas.top = reClient.top + nY_Gap + m_nLegend_OffSet; 
	m_reCanvas.bottom = reClient.bottom - m_nXMask_OffSet;
	m_reCanvas.left = reClient.left + nX_Gap * 2;
	m_reCanvas.right = reClient.right - nX_Gap;
	
	int nItemCnt;
	nItemCnt = m_listXData.GetCount();
	if (nItemCnt > 0)
	{
		m_dXOffset = (int)(m_reCanvas.Width() / (double)(nItemCnt - 1));
		m_reCanvas.right = m_reCanvas.left + (int)(m_dXOffset * (nItemCnt - 1));
		
		m_dYOffset = m_reCanvas.Height() / (double)nItemCnt;
		m_reCanvas.top = m_reCanvas.bottom - (int)(m_dYOffset * nItemCnt);
	}
	
	FillSolidRect(hDC, m_reCanvas.left, m_reCanvas.top, 
		m_reCanvas.right - m_reCanvas.left, m_reCanvas.bottom - m_reCanvas.top,
		crColor);
	
	SetDrawBox(hDC, m_reCanvas.left, m_reCanvas.top, 
		m_reCanvas.right, m_reCanvas.bottom,
		RGB(0x00, 0x00, 0x00), 2);
	
	m_ptBase.x = m_reCanvas.left + 1;
	m_ptBase.y = m_reCanvas.bottom - 1;
	
	m_nTitleInterval_Y = 5;
	m_nTitleInterval_X = 5;
	
	
	/*
	// Put Title
	m_rectTitle.SetRect (rc.left+nX_Gap, rc.top, rc.right-2*nX_Gap, rc.top + nY_Gap);
	
	  
		m_rectLegend.SetRect(
		rc.right-2*nX_Gap,
		rc.top+10,
		rc.right-10, 
		m_rectCanvas.bottom - 10);
	*/
}

void CDrLnChartCtrl::SetDrawTitle(HDC hDC)
{
	HFONT hFont = SetMakeFont((CString)FONT_NAME, (int)FONT_SIZE, FALSE);
	HFONT hOldFont;
	hOldFont = (HFONT)SelectObject(hDC, hFont);
	SetBkMode (hDC, TRANSPARENT);
	DrawText(hDC, (LPSTR)(LPCSTR)m_strTitle, m_strTitle.GetLength(), m_reTitle, DT_CENTER|DT_VCENTER);
	
	SelectObject (hDC, hOldFont);
	::DeleteObject(hFont);
}

void CDrLnChartCtrl::SetDrawLegend(HDC hDC)
{
	HFONT hFont = SetMakeFont((CString)FONT_NAME, (int)FONT_SIZE, FALSE);
	HFONT hOldFont;
	hOldFont = (HFONT)SelectObject(hDC, hFont);
	
	FillSolidRect (hDC, 
		m_reLegend.left, m_reLegend.top, 
		m_reLegend.Width(), m_reLegend.Height(),
		RGB(255, 255, 255));
	
	int nOffSet_Legend = 3;
	int nOffSet_Text = (int)FONT_SIZE;
	int nOffSet_Mark = 13;
	int nMovePos = nOffSet_Legend;
	
	CRect reLegend = m_reLegend;
	reLegend.top += 2;
	reLegend.bottom -= 2;
	
	POSITION pos;
	Series  stSeries;
	for (int nIndex = 0; nIndex < m_listSeries.GetCount(); nIndex++)
	{
		pos = m_listSeries.FindIndex(nIndex);
		stSeries = m_listSeries.GetAt(pos);
		
		if (stSeries.bView == TRUE)
		{
			//# Mark!
			reLegend.left = nMovePos + 3;
			reLegend.right = reLegend.left + nOffSet_Mark;
			
			FillSolidRect (hDC, 
				reLegend.left, reLegend.top, 
				reLegend.Width(), reLegend.Height(),
				stSeries.clrLine);
			
			reLegend.left = reLegend.right + 1;
			reLegend.right = reLegend.left + (int)(nOffSet_Text * stSeries.strLengend.GetLength() * 0.6);
			SetBkMode (hDC, TRANSPARENT);
			TextOut(hDC, reLegend.left, reLegend.bottom - nOffSet_Text, stSeries.strLengend, stSeries.strLengend.GetLength());
			
			nMovePos = reLegend.right;
		}
	}
	
	SelectObject (hDC, hOldFont);
	::DeleteObject(hFont);
}

void CDrLnChartCtrl::SetDraw_Chart(HDC hDC)
{
	CRect reClient;
	GetClientRect(reClient);
	
	SetMaxMin_YVal();
	SetDraw_Chart(hDC, reClient);
	
	Invalidate();
}

void CDrLnChartCtrl::SetDraw_Chart(HDC hDC, CRect reClient)
{
	if (hDC == NULL) 
	{
		CClientDC dc(this);
		hDC = dc.m_hDC;
	}
	
	SetDrawCanvers(hDC, reClient, m_clrCanvas);
	
	SetDrawLegend(hDC);
	
	SetDrawYGrid(hDC);
	SetDrawXGrid(hDC);
	SetDrawLine(hDC);
}

HFONT CDrLnChartCtrl::SetMakeFont(CString strFontName, int nFontSize, BOOL bRotate/*=FALSE*/)
{
	if (bRotate) 
	{
		return CreateFont(nFontSize, 0, 900, 900, 400, 
			FALSE, FALSE, 0, 
			ANSI_CHARSET, 
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, 
			DEFAULT_PITCH|FF_MODERN,
			strFontName);
	}
	else 
	{
		return CreateFont(nFontSize, 0, 0, 0, 400, 
			FALSE, FALSE, 0, 
			ANSI_CHARSET, 
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH|FF_MODERN,
			strFontName);
	}
}
