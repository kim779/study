// XTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "XTabCtrl.h"
#include "TabDlg.h"
#include "./define/ChartMsgDef.h"
#include "StdDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTabCtrl

CXTabCtrl::CXTabCtrl()
{
	m_iSelectedTab = m_nPrevIndex = 0;
	m_ptTabs.x = 0;
	m_ptTabs.y = 0;

	m_crSelected = 0;
	m_crDisabled = 0;
	m_crNormal = 0;
	m_crMouseOver = 0;
	m_clSelTabStart = 0;
	m_clSelTabEnd = 0;

	m_bColorSelected  = false;
	m_bColorDisabled  = false;
	m_bColorNormal    = false;
	m_bColorMouseOver = false;

	m_iIndexMouseOver = -1;
	m_iLastIndexMouseOver = -1;
	m_bMouseOver = false;

	m_bDown = FALSE;

	m_nType = 0;
	m_nRowCnt = 1;
	m_pTabDlgFocused = NULL;
	m_pTabDlgCodeSearch = NULL;
	m_pCompareChartWnd = NULL;
}

CXTabCtrl::~CXTabCtrl()
{
	
}


BEGIN_MESSAGE_MAP(CXTabCtrl, CExTabCtrl)
	//{{AFX_MSG_MAP(CXTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	ON_NOTIFY_REFLECT(TCN_SELCHANGING, OnSelchanging)
	//ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//ON_WM_LBUTTONDOWN()
	//ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	//ON_WM_ERASEBKGND()
	//ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTabCtrl message handlers

void CXTabCtrl::SetTopLeftCorner(CPoint pt)
{
	m_ptTabs.x = pt.x;
	m_ptTabs.y = pt.y;
}

//void CXTabCtrl::SetDisabledColor(COLORREF cr) 
//{
//	m_bColorDisabled = true;
//	m_crDisabled = cr;
//}
//
//void CXTabCtrl::SetSelectedColor(COLORREF cr, COLORREF crTabStart, COLORREF crTabEnd)
//{
//	m_bColorSelected = true;
//	m_crSelected= cr;
//	m_clSelTabStart = crTabStart;
//	m_clSelTabEnd = crTabEnd;
//}
//
//void CXTabCtrl::SetMouseOverColor(COLORREF cr, COLORREF crTabStart, COLORREF crTabEnd)
//{
//	m_bColorMouseOver = true;
//	m_crMouseOver = cr;
//	m_clOverTabStart = crTabStart;
//	m_clOverTabEnd = crTabEnd;
//}
//
//void CXTabCtrl::SetNormalColor(COLORREF cr)
//{
//	m_bColorNormal = true;
//	m_crNormal = cr;
//}

void CXTabCtrl::Init(CWnd* pCompareChartWnd,CWnd *pChartMng, int nChartType, void* pDBMgr)
{	
	SetpCompareChartWnd(pCompareChartWnd);
	
	m_tabDlgCreator.Init(pCompareChartWnd, (CLeftBarDlg*)GetParent(),this,pChartMng, pDBMgr);
	char szCaption[64];
	//int nCnt = m_tabDlgCreator.GetCntOfTabDlg();
	TCITEM item;
	item.mask = TCIF_TEXT|TCIF_PARAM|TCIF_IMAGE;
	item.lParam = NULL;
	item.pszText = _T("");
	item.iImage = 0;

	int nTabCount = COMPARECHART_TABTOTALCNT;
	if(((CStdDialog*)pCompareChartWnd)->m_nScreenType == CStdDialog::Screen_ELWChart)
		nTabCount = 1;
	else if(((CStdDialog*)pCompareChartWnd)->m_nScreenType == CStdDialog::Screen_ForeignChart)
		nTabCount = 1;
//	20130515 이상신 : 해외지수 삭제.(issue No - 0006748)
//	else if(((CStdDialog*)pCompareChartWnd)->m_nScreenType == CStdDialog::Screen_ForeignChart)
//		nTabCount = 2;

	for(int i=0;i<nTabCount;i++)
	{
		m_tabDlgCreator.GetCaptionOfTabDlg(i,szCaption);
		item.pszText = szCaption;
		//InsertItem(i, &item);
		InsertItem(i, &item, EXTAB_02);
	}	

//	if(!m_pTabDlgCodeSearch)
//	{
//		m_pTabDlgCodeSearch = m_tabDlgCreator.CreateTabDlg(TABNO_CODESEARCH);
//	}

}

void CXTabCtrl::DeleteCurrentDlg()
{
	if(m_pTabDlgFocused)
	{
		m_pTabDlgFocused->DestroyWindow();		
		delete m_pTabDlgFocused;
		m_pTabDlgFocused = NULL;	
	}
}

void CXTabCtrl::ShowTab(int nIndex)
{
	DeleteCurrentDlg();
	//SetCurSel(nIndex);
	if(nIndex==TABNO_STOCK) //2005. 06. 09 sy, nam
	{
		if( (m_pTabDlgCodeSearch == NULL) ||
			(m_pTabDlgCodeSearch->GetSafeHwnd() == NULL)
		)
		{	
			m_pTabDlgCodeSearch = m_tabDlgCreator.CreateTabDlg(TABNO_STOCK);
			
			STTabColor tabColor;
			if((COLORREF)((CStdDialog*)m_pCompareChartWnd)->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)1, (LPARAM)&tabColor))
			{
				if(m_pTabDlgCodeSearch)		m_pTabDlgCodeSearch->ChangeSkinColor(tabColor.clrTabBkgrnd);
			}
		}

		if( (m_pTabDlgCodeSearch == NULL) ||(m_pTabDlgCodeSearch->GetSafeHwnd() == NULL))
			return;

		CRect rect;
		GetClientRect(rect);
		int x = m_ptTabs.x;
		int y = m_ptTabs.y+(m_nRowCnt-1)*GetTabHeight();
		//2005. 05. 17 -------------------------------------------------------
		//전체 Row수가 2개 이상이 되면 화면이 보이는 위치를 조절한다.
		//--------------------------------------------------------------------
		if(m_nRowCnt == 3)
			y += 8;
		else if(m_nRowCnt > 3)
			y += 10;
		//--------------------------------------------------------------------
		m_pTabDlgCodeSearch->SetWindowPos(NULL, x, y , rect.right - x, rect.bottom - y,
							SWP_FRAMECHANGED| SWP_NOZORDER);
		m_pTabDlgCodeSearch->ShowWindow(SW_SHOW);

		//JS.Kim_20101005   화면 깜박임 현상 제거
		m_pTabDlgCodeSearch->ModifyStyle(0, WS_CLIPSIBLINGS|WS_CLIPCHILDREN);
	}
	else
	{
		if(m_pTabDlgCodeSearch)
		{
			m_pTabDlgCodeSearch->ShowWindow(SW_HIDE);
		}
		m_pTabDlgFocused = m_tabDlgCreator.CreateTabDlg(nIndex);
		if(m_pTabDlgFocused)
		{
			STTabColor tabColor;
			if((COLORREF)((CStdDialog*)m_pCompareChartWnd)->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)1, (LPARAM)&tabColor))
			{
				if(m_pTabDlgFocused)		m_pTabDlgFocused->ChangeSkinColor(tabColor.clrTabBkgrnd);
			}

			CRect rect;
			GetClientRect(rect);
			int x = m_ptTabs.x;
			int y = m_ptTabs.y+(m_nRowCnt-1)*GetTabHeight();
			//2005. 05. 17 -------------------------------------------------------
			//전체 Row수가 2개 이상이 되면 화면이 보이는 위치를 조절한다.
			//--------------------------------------------------------------------
			if(m_nRowCnt == 3)
				y += 8;
			else if(m_nRowCnt > 3)
				y += 10;
			//--------------------------------------------------------------------
			m_pTabDlgFocused->SetWindowPos(NULL, x, y , rect.right - x, rect.bottom - y,
								SWP_FRAMECHANGED| SWP_NOZORDER);
			m_pTabDlgFocused->ShowWindow(SW_SHOW);
			//JS.Kim_20101005   화면 깜박임 현상 제거
			m_pTabDlgFocused->ModifyStyle(0, WS_CLIPSIBLINGS|WS_CLIPCHILDREN);
		}
	}

}

void CXTabCtrl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//int iNewTab = GetCurSel();
	//ShowTab(iNewTab);
	//*pResult = 0;

	CExTabCtrl::OnSelchange(pNMHDR, pResult);
	int iNewTab = GetCurSel();	
	ShowTab(iNewTab);
	*pResult = 0;
}

void CXTabCtrl::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	// TODO: Add your control notification handler code here
//	m_nPrevIndex = m_iSelectedTab;
//	m_iSelectedTab = GetCurSel();
//	*pResult = 0;
	CExTabCtrl::OnSelchange(pNMHDR, pResult);
	m_nPrevIndex = m_iSelectedTab;
	m_iSelectedTab = GetCurSel();
	*pResult = 0;
}

void CXTabCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CExTabCtrl::PreSubclassWindow();
	ModifyStyle(0, TCS_OWNERDRAWFIXED);
}

//void CXTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
//{
//
//	CRect rect = lpDrawItemStruct->rcItem;
//	rect.top += ::GetSystemMetrics(SM_CYEDGE);
//	rect.bottom += 1;
//
//	int nTabIndex = lpDrawItemStruct->itemID;
//	
//	if (nTabIndex < 0) return;
//
//	BOOL bSelected = (nTabIndex == GetCurSel());
//
//	COLORREF crSelected = m_bColorSelected ? m_crSelected : GetSysColor(COLOR_BTNTEXT);
//	COLORREF crNormal  = m_bColorNormal   ? m_crNormal   : GetSysColor(COLOR_BTNTEXT);
//	COLORREF crDisabled = m_bColorDisabled ? m_crDisabled : GetSysColor(COLOR_GRAYTEXT);
//
//	char label[64];
//	TC_ITEM item;
//	item.mask = TCIF_TEXT|TCIF_IMAGE;
//	item.pszText = label;     
//	item.cchTextMax = 63;    	
//	if (!GetItem(nTabIndex, &item))
//		return;
//
//	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
//	if (!pDC)
//		return;
//
//	int nSavedDC = pDC->SaveDC();
//
//	CRect rectItem;
//	POINT pt;
//
//	GetItemRect(nTabIndex, &rectItem);
//	GetCursorPos(&pt);
//	ScreenToClient(&pt);
//
//	if (rectItem.PtInRect(pt))
//		m_iIndexMouseOver = nTabIndex;
//
//	pDC->SetBkMode(TRANSPARENT);
//
//	//**  Draw the image
//	CImageList* pImageList = GetImageList();
//	if (pImageList && item.iImage >= 0) 
//	{
//		rect.left += pDC->GetTextExtent(_T(" ")).cx;
//		IMAGEINFO info;
//		pImageList->GetImageInfo(item.iImage, &info);
//		CRect ImageRect(info.rcImage);
//		int nYpos = rect.top;
//
//		pImageList->Draw(pDC, item.iImage, CPoint(rect.left, nYpos), ILD_TRANSPARENT);
//		rect.left += ImageRect.Width();
//	}
//
//	//** selected item -----
//	if (bSelected)
//	{
//		rect.bottom -= 2;
//		DrawGradientTab(pDC, rect);
//		pDC->SetTextColor(RGB(255,255,255));
////		pDC->SetTextColor(crSelected);
//	}
//	else //** other item ---
//	{
////		pDC->FillSolidRect(rect, RGB(231, 231, 231));
//		rect.right -= 2;
////		pDC->FillSolidRect(rect, RGB(231, 231,231));
//		pDC->FillSolidRect(rect, RGB(231, 231,231));
//		if (m_bColorMouseOver && nTabIndex == m_iIndexMouseOver) 
//		{
//			DrawGradientTab(pDC, rect,FALSE);
//			pDC->SetTextColor(m_crMouseOver);
//		}
//		else
//		{
//			pDC->SetTextColor(crNormal);
//		}
//	}
//
////	rect.top -= ::GetSystemMetrics(SM_CYEDGE);
//	pDC->DrawText(label, rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
//	pDC->RestoreDC(nSavedDC);
//}
//
//void CXTabCtrl::DrawGradientTab(CDC *pDC, CRect rectIn, BOOL bSelectedItem/*=TRUE*/)
//{
//	CRect rect;
//	rect = rectIn;
//	rect.bottom = rect.top + rect.Height();///4;
//	COLORREF Start, End;
//	if(bSelectedItem)
//	{
//		Start = m_clSelTabStart;
//		End   = m_clSelTabEnd;
//	}
//	else
//	{
//		if(m_bColorMouseOver)
//		{
//			Start = m_clOverTabStart;
//			End   = m_clOverTabEnd;
//		}
//	}
//
//	CRect backup = rect;
//		
//	int r,g,b;//각 3가지 색상의 값의 차이..
//	float rStep, gStep, bStep;//각 3가지 값들 각각의 색상값의 차이..
//	float fStep;//최대 색상값 차이로 나누어서 한번에 이동할 옵셋..
//
//	//각 생상 차이를 구한다..
//	r = (GetRValue(End) - GetRValue(Start));
//	g = (GetGValue(End) - GetGValue(Start));
//	b = (GetBValue(End) - GetBValue(Start));
//
//	//가장 큰차이가 나는것을 구한다..
//	int Max = max( abs(r), max(abs(g), abs(b)) );
//
//	//그릴 사각형을 차이가 나는것 만큼.. 분할할 길이를 구한다..
//	fStep = (float)rect.Height()/(float)Max;     // 상-하 Gradation 
//
//	//각 색상별 단계 차이값..
//	rStep = r/(float)Max;
//	gStep = g/(float)Max;
//	bStep = b/(float)Max;
//
//	//r,g,b를 시작 색으로 재설정...
//	r = GetRValue(Start);
//	g = GetGValue(Start);
//	b = GetBValue(Start);
//
//	//색깔이 변해야 하는 횟수만큼.. 반복 그린다.
//	for(int iOnBand=0 ; iOnBand <Max; iOnBand++)  // 상-하 Gradation 
//	{
//		::SetRect( &rect, rect.left, int(fStep*iOnBand) + backup.top ,
//		    rect.right, backup.bottom );  
//		pDC->FillSolidRect( &rect,  RGB( r + rStep*iOnBand, g + gStep*iOnBand, b + bStep*iOnBand ) );
//	}
//}

//void CXTabCtrl::OnMouseMove(UINT nFlags, CPoint point) 
//{
//	CExTabCtrl::OnMouseMove(nFlags, point);	
//
//	if(m_bDown)
//	{
//		CDC ScreenDC;
//		ScreenDC.CreateDC("DISPLAY", NULL, NULL, NULL);
//		ScreenDC.SetROP2(R2_XORPEN);
//		CPen pen2, *pOldPen;
//		pen2.CreatePen(PS_SOLID, 2, RGB(127, 127, 127));
//
//
//		int nTopPoint = 20;
//
//		pOldPen = ScreenDC.SelectObject(&pen2);
//		ScreenDC.MoveTo(m_rcRound.left, m_rcRound.top - nTopPoint);
//		ScreenDC.LineTo(m_rcRound.right, m_rcRound.top - nTopPoint);
//		ScreenDC.LineTo(m_rcRound.right, m_rcRound.bottom - 1);
//		ScreenDC.LineTo(m_rcRound.left, m_rcRound.bottom - 1);
//		ScreenDC.LineTo(m_rcRound.left, m_rcRound.top - nTopPoint);
//		
//		m_rcRound -= m_ptPoint - point;
//		ScreenDC.MoveTo(m_rcRound.left, m_rcRound.top - nTopPoint);
//		ScreenDC.LineTo(m_rcRound.right, m_rcRound.top - nTopPoint);
//		ScreenDC.LineTo(m_rcRound.right, m_rcRound.bottom - 1);
//		ScreenDC.LineTo(m_rcRound.left, m_rcRound.bottom - 1);
//		ScreenDC.LineTo(m_rcRound.left, m_rcRound.top - nTopPoint);
//
//		ScreenDC.SelectObject(pOldPen);
//
//		m_ptPoint = point;
//	}
//
//	//** if we should change the color of the tab ctrl ---
//	if (m_bColorMouseOver)
//	{
//		if(m_iIndexMouseOver!=-1&&!m_bMouseOver)	m_iLastIndexMouseOver = m_iIndexMouseOver;
//		SetTimer(1,5,NULL);
//		if (m_iIndexMouseOver != -1)
//		{
//			CRect rectItem;
//			GetItemRect(m_iIndexMouseOver, rectItem);
//			if (!rectItem.PtInRect(point))
//			{
//				CRect rectOldItem;
//				GetItemRect(m_iIndexMouseOver, rectOldItem);
//				m_iIndexMouseOver = -1;
//				InvalidateRect(rectOldItem);
//				return;
//			}
//		}
//		if (!m_bMouseOver)
//		{
//			TCHITTESTINFO hitTest;
//			m_bMouseOver = true;
//			hitTest.pt = point;
//
//			int iItem = HitTest(&hitTest);
//			if (iItem != -1 )
//			{
//				RECT rectItem;
//				GetItemRect(iItem, &rectItem);
//				InvalidateRect(&rectItem);
//			}
//		}
//	}
//	
//	CExTabCtrl::OnMouseMove(nFlags, point);
//}

void CXTabCtrl::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt;
	GetCursorPos(&pt);
	CRect rectItem, rectScreen;

	GetItemRect(m_iIndexMouseOver, rectItem);
	rectScreen = rectItem;
	ClientToScreen(rectScreen);

	
	// If mouse leaves, show normal
	if (!rectScreen.PtInRect(pt)||m_iLastIndexMouseOver!=m_iIndexMouseOver) 
	{
		KillTimer (1);
		m_bMouseOver = false;
		m_iIndexMouseOver = -1;
		InvalidateRect(rectItem);
	}


	CExTabCtrl::OnTimer(nIDEvent);
}


//void CXTabCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
//{
//	CExTabCtrl::OnLButtonDown(nFlags, point);
//	return;
//
//	CRect rcTitle;
//	GetClientRect(&rcTitle);
//
//	if(rcTitle.PtInRect(point))
//	{
//		SetCapture();
//		GetWindowRect(&m_rcRound);
//
//		CDC ScreenDC;
//		ScreenDC.CreateDC("DISPLAY", NULL, NULL, NULL);
//		ScreenDC.SetROP2(R2_XORPEN);
//
//
//		int nTopPoint = 20;
//
//		CPen pen, *pOldPen;
//		pen.CreatePen(PS_SOLID, 2, RGB(127, 127, 127));
//		pOldPen = ScreenDC.SelectObject(&pen);
//		ScreenDC.MoveTo(m_rcRound.left, m_rcRound.top - nTopPoint);
//		ScreenDC.LineTo(m_rcRound.right, m_rcRound.top - nTopPoint);
//		ScreenDC.LineTo(m_rcRound.right, m_rcRound.bottom - 1);
//		ScreenDC.LineTo(m_rcRound.left, m_rcRound.bottom - 1);
//		ScreenDC.LineTo(m_rcRound.left, m_rcRound.top - nTopPoint);
//		
//		ScreenDC.SelectObject(pOldPen);
//
//		m_ptPoint = point;
//		m_bDown = TRUE;
//	}
//
//	CExTabCtrl::OnLButtonDown(nFlags, point);
//}
//
//void CXTabCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
//{
//	CExTabCtrl::OnLButtonUp(nFlags, point);
//	return;
//	
//	if(m_bDown)
//	{
//		ReleaseCapture();
//
//		m_bDown = FALSE;
//
//		CDC ScreenDC;
//		ScreenDC.CreateDC("DISPLAY", NULL, NULL, NULL);
//		ScreenDC.SetROP2(R2_XORPEN);
//
//
//		int nTopPoint = 20;
//
//		CPen pen, *pOldPen;
//		pen.CreatePen(PS_SOLID, 2, RGB(127, 127, 127));
//		pOldPen = ScreenDC.SelectObject(&pen);
//		ScreenDC.MoveTo(m_rcRound.left, m_rcRound.top - nTopPoint);
//		ScreenDC.LineTo(m_rcRound.right, m_rcRound.top - nTopPoint);
//		ScreenDC.LineTo(m_rcRound.right, m_rcRound.bottom - 1);
//		ScreenDC.LineTo(m_rcRound.left, m_rcRound.bottom - 1);
//		ScreenDC.LineTo(m_rcRound.left, m_rcRound.top - nTopPoint);
//		ScreenDC.SelectObject(pOldPen);
//
//		CRect rc;
//		GetWindowRect(&rc);
//		ClientToScreen(&point);
//
//
//	}
//
//	CExTabCtrl::OnLButtonUp(nFlags, point);
//}

void CXTabCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CExTabCtrl::OnSize(nType, cx, cy);
	
	m_nRowCnt = GetRowCount();

	int x = m_ptTabs.x;

	int y = m_ptTabs.y+(m_nRowCnt-1)*GetTabHeight();
	//2005. 05. 17 -------------------------------------------------------
	//전체 Row수가 2개 이상이 되면 화면이 보이는 위치를 조절한다.
	//--------------------------------------------------------------------
	if(m_nRowCnt == 3)
		y += 8;
	else if(m_nRowCnt > 3)
		y += 10;
	//--------------------------------------------------------------------
		
	cx = cx - x;
	cy = cy - y;
	if(m_pTabDlgFocused->GetSafeHwnd())
	{
		m_pTabDlgFocused->SetWindowPos(NULL,x, y,cx,cy,SWP_FRAMECHANGED | SWP_NOZORDER);
	}		
	if(m_pTabDlgCodeSearch)
	{
		if(m_pTabDlgCodeSearch->GetSafeHwnd())
		{
			m_pTabDlgCodeSearch->SetWindowPos(NULL,x, y,cx,cy,SWP_FRAMECHANGED | SWP_NOZORDER);
		}		
	}
}

//BOOL CXTabCtrl::OnEraseBkgnd(CDC* pDC) 
//{
//	CRect rc;
//	GetClientRect(&rc);
//	pDC->FillSolidRect(rc, m_clTabBk);
//
//	rc.top += 20;
//
//	pDC->FillSolidRect(rc, RGB(233,233,232));
//
//	rc.bottom = rc.top + 19;
//	pDC->FillSolidRect(rc, RGB(243,243,243));
//	
//	return TRUE;
//}

//void CXTabCtrl::OnPaint() 
//{
//	CPaintDC dc(this); // device context for painting
//
//	dc.SelectObject(GetFont());
//
//	DRAWITEMSTRUCT dis;
//	dis.CtlType = ODT_TAB;
//	dis.CtlID = GetDlgCtrlID();
//	dis.hwndItem = GetSafeHwnd();
//	dis.hDC = dc.GetSafeHdc();
//	dis.itemAction = ODA_DRAWENTIRE;
//
//	CRect rClient, rPage;
///*	GetClientRect(&dis.rcItem);
//	rPage = dis.rcItem;
//	AdjustRect(FALSE, rPage);
//	dis.rcItem.top = rPage.top - 2;
//*/
//	COLORREF crHighlight = RGB(255, 255, 255);
//	COLORREF crHighlight2 = RGB(68, 68, 68);
//	COLORREF crShadow = RGB(255, 255, 255);
//	COLORREF crShadow2 = RGB(68, 68, 68);
//	COLORREF crBack = RGB(233,233,232);
//
//	CRect rcTemp;
//	rcTemp.CopyRect(&dis.rcItem);
//	dc.Draw3dRect(rcTemp, crHighlight, crShadow);
//
//	DrawLine(CPoint(rcTemp.left, rcTemp.top),
//		CPoint(rcTemp.left, rcTemp.bottom - 1),
//		&dc, crShadow2);
//
//	rcTemp.DeflateRect(1, 1);
//	dc.Draw3dRect(rcTemp, crHighlight2, crShadow2);
//
//	DrawLine(CPoint(rcTemp.left, rcTemp.top),
//		CPoint(rcTemp.left, rcTemp.bottom - 1),
//		&dc, crShadow);
//
//	int nTab = GetItemCount();
//	int nSel = GetCurSel();
//
//	if (!nTab)
//		return;	
//
//	while (nTab--)
//	{
//		if (nTab != nSel)
//		{
//			dis.itemID = nTab;
//			dis.itemState = 0;
//
//			VERIFY(GetItemRect(nTab, &dis.rcItem));
//
//			dis.rcItem.bottom -= 2;
//			DrawItem(&dis);
//
//			rcTemp.CopyRect(&dis.rcItem);
//			rcTemp.left -= 2;
//			rcTemp.bottom += 4;
//			dc.Draw3dRect(rcTemp, crHighlight2, crShadow2);
//		}
//	}
//
//	dis.itemID = nSel;
//	dis.itemState = ODS_SELECTED;
//	//if(nSel < 0 ) nSel = 0;
//
//	VERIFY(GetItemRect(nSel, &dis.rcItem));
//
//	//dis.rcItem.bottom += 2;
//	//dis.rcItem.top -= 2;
//	DrawItem(&dis);
//
//	rcTemp.CopyRect(&dis.rcItem);
//	//rcTemp.InflateRect(2, 0);
//	dc.Draw3dRect(rcTemp, crHighlight2, crShadow2);
//
//	DrawLine(CPoint(rcTemp.left + 1, rcTemp.bottom - 1),
//		CPoint(rcTemp.right - 1, rcTemp.bottom - 1),
//		&dc, crBack);
//
//	rcTemp.DeflateRect(1, 0);
//	dc.Draw3dRect(rcTemp, crHighlight, crShadow);
//
//	DrawLine(CPoint(rcTemp.left, rcTemp.top),
//		CPoint(rcTemp.right, rcTemp.top),
//		&dc, crShadow2);
//	
//}
//
//void CXTabCtrl::DrawLine(CPoint ptMove, CPoint ptTo, CDC* pDC, COLORREF cr)
//{
//	CPen pen, *pOldPen;
//	pen.CreatePen(PS_SOLID, 1, cr);
//	pOldPen = pDC->SelectObject(&pen);
//
//	pDC->MoveTo(ptMove);
//	pDC->LineTo(ptTo);
//
//	pDC->SelectObject(pOldPen);
//	pen.DeleteObject();
//}

void CXTabCtrl::OnDestroy() 
{
	DeleteCurrentDlg();
	if(m_pTabDlgCodeSearch)
	{
		m_pTabDlgCodeSearch->DestroyWindow();
		delete m_pTabDlgCodeSearch;
		m_pTabDlgCodeSearch = NULL;
	}
	CExTabCtrl::OnDestroy();
}

LRESULT CXTabCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	//===================================================================
	//2005. 06. 14 add by sy,nam
	//===================================================================
	switch(message)
	{
		case UMSG_ADDITEM_BTNCLICK:
		case UMSG_DELITEM_BTNCLICK:
			{
				if(m_pTabDlgFocused->GetSafeHwnd())
					::SendMessage(m_pTabDlgFocused->GetSafeHwnd(), message, 0, 0);
				//주식탭일 경우만 보냄
				m_iSelectedTab = GetCurSel();
				if(m_iSelectedTab == 0 && m_pTabDlgCodeSearch->GetSafeHwnd())
					::SendMessage(m_pTabDlgCodeSearch->GetSafeHwnd(), message, 0, 0);
			}
			break;
		case UMSG_INITIAL_BTNCLICK:
			//Drds끊기.
			//차트 내용없애기 		
			break;
		//===================================================================

	}
	return CExTabCtrl::WindowProc(message, wParam, lParam);
}

#include "../../inc/IKSLibMng.h"
void CXTabCtrl::ChangeSkinColor()
{
//	COLORREF clrBkgrnd = (COLORREF)GetParent()->GetParent()->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0);
//
//	COLORREF clrTabActive = RGB(191,222,255);
//	COLORREF clrTabInactive = RGB(226,226,226);
//	IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager);
//	if(pKSLibMng)
//	{
//		clrTabActive = pKSLibMng->Lib_KSLink_GetColor(135);		// 탭 색상 Active
//		clrTabInactive = pKSLibMng->Lib_KSLink_GetColor(98);	// 탭 색상 Inactive
//	}
//	SetTabColor(clrBkgrnd, clrTabActive, clrTabInactive);
	if(m_pCompareChartWnd == NULL)
		return;
	
	STTabColor tabColor;
	if((COLORREF)m_pCompareChartWnd->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)1, (LPARAM)&tabColor))
	{
		if(m_pTabDlgCodeSearch)		m_pTabDlgCodeSearch->ChangeSkinColor(tabColor.clrTabBkgrnd);

		SetSTTabColor((long)&tabColor);
	}

	Invalidate();
}
