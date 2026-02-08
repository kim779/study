// XTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "XTabCtrl.h"

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
	m_clNormalTabStart = 0;
	m_clNormalTabEnd = 0;

	m_bColorSelected  = false;
	m_bColorDisabled  = false;
	m_bColorNormal    = false;
	m_bColorMouseOver = false;

	m_iIndexMouseOver = -1;

	m_bMouseOver = false;

	m_bDown = FALSE;

	m_nType = 0;
	m_nUpperOffset = 0;
	m_nLeftOffset = 0;
}

CXTabCtrl::~CXTabCtrl()
{
	m_arrayStatusTab.RemoveAll();
}


BEGIN_MESSAGE_MAP(CXTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CXTabCtrl)
	ON_NOTIFY_REFLECT_EX(TCN_SELCHANGE, OnSelchange)
	ON_NOTIFY_REFLECT_EX(TCN_SELCHANGING, OnSelchanging)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTabCtrl message handlers

void CXTabCtrl::SetTopLeftCorner(CPoint pt)
{
	m_ptTabs.x = pt.x;
	m_ptTabs.y = pt.y;
}

void CXTabCtrl::SetMouseOverColor(COLORREF cr) 
{
	m_bColorMouseOver = true;
	m_crMouseOver = cr;
}


void CXTabCtrl::SetDisabledColor(COLORREF cr) 
{
	m_bColorDisabled = true;
	m_crDisabled = cr;
}

void CXTabCtrl::SetSelectedColor(COLORREF cr, COLORREF crTabBK)
{
	m_bColorSelected = true;
	m_crSelected= cr;
	m_clSelTabBK = crTabBK;
}


void CXTabCtrl::SetNormalColor(COLORREF cr, COLORREF crTabStart, COLORREF crTabEnd)
{
	m_bColorNormal = true;
	m_crNormal = cr;
	m_clNormalTabStart = crTabStart;
	m_clNormalTabEnd = crTabEnd;
}



void CXTabCtrl::AddTab(CWnd* pWnd, LPTSTR lpszCaption, int iImage)
{
	ASSERT_VALID(pWnd);

	TCITEM item;
	item.mask = TCIF_TEXT|TCIF_PARAM|TCIF_IMAGE;
	item.lParam = (LPARAM) pWnd;
	item.pszText = lpszCaption;
	item.iImage = iImage;

	int iIndex = m_arrayStatusTab.GetSize();
	InsertItem(iIndex, &item);

	CRect rcTab;
	GetClientRect(rcTab);
//	pWnd->SetWindowPos(NULL, m_ptTabs.x, m_ptTabs.y, rcTab.Width(), rcTab.Height(), 
//						SWP_FRAMECHANGED|SWP_NOZORDER);	

	pWnd->SetWindowPos(NULL, rcTab.left + m_ptTabs.x, rcTab.top + m_ptTabs.y, rcTab.Width() - m_ptTabs.x + m_nLeftOffset, rcTab.Height() - m_ptTabs.y + m_nUpperOffset, SWP_SHOWWINDOW);
	pWnd->ShowWindow(iIndex ? SW_HIDE : SW_SHOW);

	//** the initial status is enabled
	m_arrayStatusTab.Add(TRUE); 
}

void CXTabCtrl::EnableTab(int iIndex, BOOL bEnable)
{
	ASSERT(iIndex < m_arrayStatusTab.GetSize());

	//** if it should change the status ----
	if (m_arrayStatusTab[iIndex] != bEnable)
	{
		m_arrayStatusTab[iIndex] = bEnable;

		//** redraw the item -------
		CRect rect;

		GetItemRect(iIndex, &rect);
		InvalidateRect(rect);
	}
}

void CXTabCtrl::DeleteAllTabs()
{
	m_arrayStatusTab.RemoveAll();
	
	DeleteAllItems();

}

void CXTabCtrl::DeleteTab(int iIndex)
{
	ASSERT(iIndex < m_arrayStatusTab.GetSize());

	m_arrayStatusTab.RemoveAt(iIndex);

	DeleteItem(iIndex);
}


BOOL CXTabCtrl::IsTabEnabled(int iIndex)
{
	ASSERT(iIndex < m_arrayStatusTab.GetSize());
	
	return m_arrayStatusTab[iIndex];
}


BOOL CXTabCtrl::SelectTab(int iIndex)
{
	ASSERT(iIndex < m_arrayStatusTab.GetSize());

	if (GetCurSel() == iIndex)
		return TRUE;

	if (m_arrayStatusTab[iIndex])
	{
		TCITEM item;
		CWnd* pWnd;
		item.mask = TCIF_PARAM;

		GetItem(GetCurSel(), &item);
		pWnd = reinterpret_cast<CWnd*> (item.lParam);

		ASSERT_VALID(pWnd);
		pWnd->ShowWindow(SW_HIDE);

		SetCurSel(iIndex);
		GetItem(iIndex, &item);
		pWnd = reinterpret_cast<CWnd*> (item.lParam);

		ASSERT_VALID(pWnd);
//		pWnd->ShowWindow(SW_SHOW);
		CRect rcTab;
		GetClientRect(rcTab);

		pWnd->SetWindowPos(NULL, m_ptTabs.x, m_ptTabs.y, rcTab.Width() - m_ptTabs.x + m_nLeftOffset, rcTab.Height() - m_ptTabs.y + m_nUpperOffset, 
							SWP_FRAMECHANGED|SWP_NOZORDER|SWP_SHOWWINDOW);	

//		pWnd->SetWindowPos(NULL, m_ptTabs.x, m_ptTabs.y, 0, 0,SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER);

//		pWnd->SetWindowPos(NULL, 0, 0, 0, 0, SWP_SHOWWINDOW|SWP_FRAMECHANGED|SWP_NOSIZE|SWP_NOMOVE);

		return TRUE;
	}

	return FALSE;
}

BOOL CXTabCtrl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int iNewTab = GetCurSel();

	if (!IsTabEnabled(iNewTab))
	{
		SetCurSel(m_iSelectedTab);
	}
	else
	{
		TCITEM item;
		CWnd* pWnd;

		item.mask = TCIF_PARAM;
		
		//** hide the current tab ---------
		GetItem(m_iSelectedTab, &item);
		pWnd = reinterpret_cast<CWnd*> (item.lParam);

		ASSERT_VALID(pWnd);
		pWnd->ShowWindow(SW_HIDE);

		//** show the selected tab --------
		GetItem(iNewTab, &item);
		pWnd = reinterpret_cast<CWnd*> (item.lParam);

		ASSERT_VALID(pWnd);

		CRect rc;
		GetClientRect(&rc);

		CRect rcTab;
		GetItemRect(iNewTab, &rcTab);

		pWnd->SetWindowPos(NULL, rc.left + m_ptTabs.x, rc.top + m_ptTabs.y, rc.Width() - m_ptTabs.x + m_nLeftOffset, rc.Height() - m_ptTabs.y + m_nUpperOffset, SWP_SHOWWINDOW);
		//		pWnd->SetWindowPos(NULL,  rc.left + m_ptTabs.x, rc.top +  m_ptTabs.y, 0, 0,SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
	}

	*pResult = 0;
	return FALSE;
}

BOOL CXTabCtrl::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_nPrevIndex = m_iSelectedTab;
	m_iSelectedTab = GetCurSel();

	*pResult = 0;
	return FALSE;
}

void CXTabCtrl::SetPrevSelect()
{
	int nEnableIndex = GetCurSel();

	if(nEnableIndex < 0) nEnableIndex = 0;

	nEnableIndex ? nEnableIndex-- : nEnableIndex++;

	if(GetItemCount() == 1) nEnableIndex = 0;

	SelectTab(nEnableIndex);
}

void CXTabCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CTabCtrl::PreSubclassWindow();
	ModifyStyle(0, TCS_OWNERDRAWFIXED);
}

void CXTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{	
	CRect rect = lpDrawItemStruct->rcItem;
	rect.top += ::GetSystemMetrics(SM_CYEDGE);
	rect.bottom += 1;

	int nTabIndex = lpDrawItemStruct->itemID;
	
	if (nTabIndex < 0) return;

	BOOL bSelected = (nTabIndex == GetCurSel());

	COLORREF crSelected = m_bColorSelected ? m_crSelected : GetSysColor(COLOR_BTNTEXT);
	COLORREF crNormal  = m_bColorNormal   ? m_crNormal   : GetSysColor(COLOR_BTNTEXT);
	COLORREF crDisabled = m_bColorDisabled ? m_crDisabled : GetSysColor(COLOR_GRAYTEXT);

	char label[64];
	TC_ITEM item;
	item.mask = TCIF_TEXT|TCIF_IMAGE;
	item.pszText = label;     
	item.cchTextMax = 63;    	
	if (!GetItem(nTabIndex, &item))
		return;

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	if (!pDC)
		return;

	int nSavedDC = pDC->SaveDC();

	CRect rectItem;
	POINT pt;

	GetItemRect(nTabIndex, &rectItem);
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	if (rectItem.PtInRect(pt))
		m_iIndexMouseOver = nTabIndex;

	pDC->SetBkMode(TRANSPARENT);

	//**  Draw the image
	CImageList* pImageList = GetImageList();
	if (pImageList && item.iImage >= 0) 
	{
		rect.left += pDC->GetTextExtent(_T(" ")).cx;
		IMAGEINFO info;
		pImageList->GetImageInfo(item.iImage, &info);
		CRect ImageRect(info.rcImage);
		int nYpos = rect.top;

		pImageList->Draw(pDC, item.iImage, CPoint(rect.left, nYpos), ILD_TRANSPARENT);
		rect.left += ImageRect.Width();
	}

	if (!IsTabEnabled(nTabIndex))
	{
		pDC->SetTextColor(crDisabled);
	//	rect.top -= ::GetSystemMetrics(SM_CYEDGE);
		pDC->DrawText(label, rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	}
	else
	{
		//** selected item -----
		if (bSelected)
		{
			rect.bottom -= 2;
			pDC->FillSolidRect(rect, m_clSelTabBK);
			pDC->SetTextColor(crSelected);
		}
		else //** other item ---
		{
			DrawGradientTab(pDC, rect);			
			if (m_bColorMouseOver && nTabIndex == m_iIndexMouseOver) 
			{
				pDC->SetTextColor(m_crMouseOver);	
			}
			else
			{
				pDC->SetTextColor(crNormal);
			}
		}

	//	rect.top -= ::GetSystemMetrics(SM_CYEDGE);
		pDC->DrawText(label, rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	}
	pDC->RestoreDC(nSavedDC);
}

void CXTabCtrl::DrawGradientTab(CDC *pDC, CRect rect)
{
	COLORREF Start, End;
	Start = m_clNormalTabStart;
	End   = m_clNormalTabEnd;

	CRect backup = rect;
		
	int r,g,b;//각 3가지 색상의 값의 차이..
	float rStep, gStep, bStep;//각 3가지 값들 각각의 색상값의 차이..
	float fStep;//최대 색상값 차이로 나누어서 한번에 이동할 옵셋..

	//각 생상 차이를 구한다..
	r = (GetRValue(End) - GetRValue(Start));
	g = (GetGValue(End) - GetGValue(Start));
	b = (GetBValue(End) - GetBValue(Start));

	//가장 큰차이가 나는것을 구한다..
	int Max = max( abs(r), max(abs(g), abs(b)) );

	//그릴 사각형을 차이가 나는것 만큼.. 분할할 길이를 구한다..
	fStep = (float)rect.Height()/(float)Max;     // 상-하 Gradation 

	//각 색상별 단계 차이값..
	rStep = r/(float)Max;
	gStep = g/(float)Max;
	bStep = b/(float)Max;

	//r,g,b를 시작 색으로 재설정...
	r = GetRValue(Start);
	g = GetGValue(Start);
	b = GetBValue(Start);

	//색깔이 변해야 하는 횟수만큼.. 반복 그린다.
	for(int iOnBand=0 ; iOnBand <Max; iOnBand++)  // 상-하 Gradation 
	{
		::SetRect( &rect, rect.left, int(fStep*iOnBand) + backup.top ,
		    rect.right, backup.bottom );  
		pDC->FillSolidRect( &rect,  RGB( r + rStep*iOnBand, g + gStep*iOnBand, b + bStep*iOnBand ) );
	}
}

void CXTabCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	CTabCtrl::OnMouseMove(nFlags, point);
	return;

	if(m_bDown)
	{
		CDC ScreenDC;
		ScreenDC.CreateDC("DISPLAY", NULL, NULL, NULL);
		ScreenDC.SetROP2(R2_XORPEN);
		CPen pen2, *pOldPen;
		pen2.CreatePen(PS_SOLID, 2, RGB(127, 127, 127));

//		CBaseFrameBar* pParent = (CBaseFrameBar*)GetParent();

		int nTopPoint = 20;
//		if(!pParent->IsFloating())
//			nTopPoint = 1;

		pOldPen = ScreenDC.SelectObject(&pen2);
		ScreenDC.MoveTo(m_rcRound.left, m_rcRound.top - nTopPoint);
		ScreenDC.LineTo(m_rcRound.right, m_rcRound.top - nTopPoint);
		ScreenDC.LineTo(m_rcRound.right, m_rcRound.bottom - 1);
		ScreenDC.LineTo(m_rcRound.left, m_rcRound.bottom - 1);
		ScreenDC.LineTo(m_rcRound.left, m_rcRound.top - nTopPoint);
		
		m_rcRound -= m_ptPoint - point;
		ScreenDC.MoveTo(m_rcRound.left, m_rcRound.top - nTopPoint);
		ScreenDC.LineTo(m_rcRound.right, m_rcRound.top - nTopPoint);
		ScreenDC.LineTo(m_rcRound.right, m_rcRound.bottom - 1);
		ScreenDC.LineTo(m_rcRound.left, m_rcRound.bottom - 1);
		ScreenDC.LineTo(m_rcRound.left, m_rcRound.top - nTopPoint);

		ScreenDC.SelectObject(pOldPen);

		m_ptPoint = point;
	}

	//** if we should change the color of the tab ctrl ---
	if (m_bColorMouseOver)
	{
		SetTimer(1,10,NULL);

		if (m_iIndexMouseOver != -1)
		{
			CRect rectItem;
			GetItemRect(m_iIndexMouseOver, rectItem);
			if (!rectItem.PtInRect(point))
			{
				CRect rectOldItem;
				GetItemRect(m_iIndexMouseOver, rectOldItem);
				m_iIndexMouseOver = -1;
				InvalidateRect(rectOldItem);
				return;
			}
		}
	

		if (!m_bMouseOver)
		{
			TCHITTESTINFO hitTest;

			m_bMouseOver = true;
			hitTest.pt = point;

			int iItem = HitTest(&hitTest);
			if (iItem != -1 && m_arrayStatusTab[iItem])
			{
				RECT rectItem;
				GetItemRect(iItem, &rectItem);

				InvalidateRect(&rectItem);
			}
		}
	}
	
	CTabCtrl::OnMouseMove(nFlags, point);
}

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
	if (!rectScreen.PtInRect(pt)) 
	{
		KillTimer (1);
		m_bMouseOver = false;
		m_iIndexMouseOver = -1;
		InvalidateRect(rectItem);
	}
	CTabCtrl::OnTimer(nIDEvent);
}

void CXTabCtrl::ChangeTab(int iIndex, CWnd *pNewTab, LPTSTR lpszCaption, int iImage)
{
	ASSERT_VALID(pNewTab);

	TCITEM item;
	item.mask = TCIF_TEXT|TCIF_PARAM|TCIF_IMAGE;
	item.lParam = (LPARAM) pNewTab;
	item.pszText = lpszCaption;
	item.iImage = iImage;

	//** update item 
	VERIFY(SetItem(iIndex, &item));

	pNewTab->SetWindowPos(NULL, m_ptTabs.x, m_ptTabs.y , 0, 0,
						SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER);
	
	pNewTab->ShowWindow(SW_HIDE);

	//** the initial status is enabled
	m_arrayStatusTab[iIndex] = TRUE; 

	//** updates the screen
	GetItem(GetCurSel(), &item);
	((CWnd*)item.lParam)->ShowWindow(SW_SHOW);
}

BOOL CXTabCtrl::SelectTabByName(LPCTSTR lpszTabCaption)
{
	TCHAR szName[64];
	TCITEM item;
	
	memset(&item, 0, sizeof(item));

	item.mask       = TCIF_TEXT;
	item.cchTextMax = sizeof(szName);
	item.pszText    = szName;

	for (int i=0; i < GetItemCount(); i++)
	{
		GetItem(i, &item);

		if (strcmp(lpszTabCaption, item.pszText) == 0)
			return SelectTab(i);
	}

	return FALSE;
}

BOOL CXTabCtrl::SelectNextTab(BOOL bForward)
{
	int iSum = bForward ? 1 : -1;

	int iCurSel = GetCurSel();
	int iCurSelSave(iCurSel);

	do
	{
		iCurSel += iSum;

		if (iCurSel < 0 || iCurSel == GetItemCount())
			return FALSE;

		if (IsTabEnabled(iCurSel))
		{
			if (iCurSel < GetItemCount())
			{
				SelectTab(iCurSel);

				return TRUE;
			}

			return FALSE;
		}

	}
	while (iCurSel != iCurSelSave);
	
	return FALSE;
}

void CXTabCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CTabCtrl::OnLButtonDown(nFlags, point);
	return;

	CRect rcTitle;
	GetClientRect(&rcTitle);

	if(rcTitle.PtInRect(point))
	{
		SetCapture();
		GetWindowRect(&m_rcRound);

		CDC ScreenDC;
		ScreenDC.CreateDC("DISPLAY", NULL, NULL, NULL);
		ScreenDC.SetROP2(R2_XORPEN);

//		CBaseFrameBar* pParent = (CBaseFrameBar*)GetParent();

		int nTopPoint = 20;
//		if(!pParent->IsFloating())
//			nTopPoint = 1;

		CPen pen, *pOldPen;
		pen.CreatePen(PS_SOLID, 2, RGB(127, 127, 127));
		pOldPen = ScreenDC.SelectObject(&pen);
		ScreenDC.MoveTo(m_rcRound.left, m_rcRound.top - nTopPoint);
		ScreenDC.LineTo(m_rcRound.right, m_rcRound.top - nTopPoint);
		ScreenDC.LineTo(m_rcRound.right, m_rcRound.bottom - 1);
		ScreenDC.LineTo(m_rcRound.left, m_rcRound.bottom - 1);
		ScreenDC.LineTo(m_rcRound.left, m_rcRound.top - nTopPoint);
		
		ScreenDC.SelectObject(pOldPen);

		m_ptPoint = point;
		m_bDown = TRUE;
	}

	CTabCtrl::OnLButtonDown(nFlags, point);
}

void CXTabCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CTabCtrl::OnLButtonUp(nFlags, point);
	return;
	
	if(m_bDown)
	{
		ReleaseCapture();

		m_bDown = FALSE;

		CDC ScreenDC;
		ScreenDC.CreateDC("DISPLAY", NULL, NULL, NULL);
		ScreenDC.SetROP2(R2_XORPEN);

//		CBaseFrameBar* pParent = (CBaseFrameBar*)GetParent();

		int nTopPoint = 20;
//		if(!pParent->IsFloating())
//			nTopPoint = 1;

		CPen pen, *pOldPen;
		pen.CreatePen(PS_SOLID, 2, RGB(127, 127, 127));
		pOldPen = ScreenDC.SelectObject(&pen);
		ScreenDC.MoveTo(m_rcRound.left, m_rcRound.top - nTopPoint);
		ScreenDC.LineTo(m_rcRound.right, m_rcRound.top - nTopPoint);
		ScreenDC.LineTo(m_rcRound.right, m_rcRound.bottom - 1);
		ScreenDC.LineTo(m_rcRound.left, m_rcRound.bottom - 1);
		ScreenDC.LineTo(m_rcRound.left, m_rcRound.top - nTopPoint);
		ScreenDC.SelectObject(pOldPen);

		CRect rc;
		GetWindowRect(&rc);
		ClientToScreen(&point);

/*
		if(!rc.PtInRect(point))
		{
			int nIndex = GetCurSel();

			if(m_nType == 0)
			{
				CDlgFrame* pParent = (CDlgFrame*)GetParent();
				CPoint pt = m_rcRound.TopLeft();
				pt.y -= nTopPoint;

				if(!pParent->OnTabLButtonUp(nIndex, pt))
					return;
			}
			else if(m_nType == 1)
			{
				CPoint pt = m_rcRound.TopLeft();
				pt.y -= nTopPoint;

				if(!pParent->OnTabLButtonUp(nIndex, pt))
					return;
			}		
		}
*/
	}

	CTabCtrl::OnLButtonUp(nFlags, point);
}

void CXTabCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CTabCtrl::OnSize(nType, cx, cy);
	
	TCITEM item;
	CWnd* pWnd;

	item.mask = TCIF_PARAM;
	
	int nCurSel = GetCurSel();
	GetItem(nCurSel, &item);
	pWnd = reinterpret_cast<CWnd*> (item.lParam);

	if(pWnd->GetSafeHwnd())
		pWnd->SetWindowPos(NULL, m_ptTabs.x, m_ptTabs.y, cx - m_ptTabs.x + m_nLeftOffset, cy - m_ptTabs.y + m_nUpperOffset, SWP_NOACTIVATE);	
}

BOOL CXTabCtrl::OnEraseBkgnd(CDC* pDC) 
{
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(rc, m_clTabBk);

//	rc.top += 20;

//	pDC->FillSolidRect(rc, RGB(233,233,232));

//	rc.bottom = rc.top + 19;
//	pDC->FillSolidRect(rc, RGB(243,243,243));
	
	return TRUE;
}

void CXTabCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	dc.SelectObject(GetFont());

	DRAWITEMSTRUCT dis;
	dis.CtlType = ODT_TAB;
	dis.CtlID = GetDlgCtrlID();
	dis.hwndItem = GetSafeHwnd();
	dis.hDC = dc.GetSafeHdc();
	dis.itemAction = ODA_DRAWENTIRE;

	CRect rClient, rPage;
	GetClientRect(&dis.rcItem);
	rPage = dis.rcItem;
	AdjustRect(FALSE, rPage);
	dis.rcItem.top = rPage.top - 2;

	COLORREF crHighlight = RGB(255, 255, 255);
	COLORREF crHighlight2 = RGB(185, 185, 185);
	COLORREF crShadow = m_clrHilight;
	COLORREF crShadow1 = m_clrShadow;
	COLORREF crShadow2 = m_clrBorder;

	CRect rcTemp;
	rcTemp.CopyRect(&dis.rcItem);
	dc.Draw3dRect(rcTemp, crHighlight, crHighlight);

	rcTemp.DeflateRect(1, 1);
	dc.Draw3dRect(rcTemp, crShadow2, crShadow2);

	int nTab = GetItemCount();
	int nSel = GetCurSel();

	if (!nTab)
		return;

	while (nTab--)
	{
		if (nTab != nSel)
		{
			dis.itemID = nTab;
			dis.itemState = 0;

			VERIFY(GetItemRect(nTab, &dis.rcItem));

			dis.rcItem.bottom -= 2;
			DrawItem(&dis);

			rcTemp.CopyRect(&dis.rcItem);
			rcTemp.left -= 2;
			rcTemp.bottom += 4;
			dc.Draw3dRect(rcTemp, crHighlight2, crShadow2);
		}
	}

	dis.itemID = nSel;
	dis.itemState = ODS_SELECTED;

	VERIFY(GetItemRect(nSel, &dis.rcItem));

	dis.rcItem.bottom += 2;
	dis.rcItem.top -= 2;
	DrawItem(&dis);

	rcTemp.CopyRect(&dis.rcItem);
	rcTemp.InflateRect(2, 0);
	dc.Draw3dRect(rcTemp, crShadow2, crShadow2);

	rcTemp.DeflateRect(1, 1, 1, 0);
	dc.Draw3dRect(rcTemp, crShadow, crShadow1);
}

void CXTabCtrl::DrawLine(CPoint ptMove, CPoint ptTo, CDC* pDC, COLORREF cr)
{
	CPen pen, *pOldPen;
	pen.CreatePen(PS_SOLID, 1, cr);
	pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(ptMove);
	pDC->LineTo(ptTo);

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}
