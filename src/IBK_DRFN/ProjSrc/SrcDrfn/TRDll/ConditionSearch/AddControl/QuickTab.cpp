// QuickTab.cpp : implementation file
//

#include "stdafx.h"
#include "QuickTab.h"
#include "../Resource.h"

#include "../MainFrmWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQuickTab

DWORD CQuickTab::s_dwCustomLook = ETC_GRADIENT;
enum { PADDING = 3, EDGE = 20};
COLORREF CQuickTab::Darker(COLORREF crBase, float fFactor)
{
	ASSERT (fFactor < 1.0f && fFactor > 0.0f);

	fFactor = min(fFactor, 1.0f);
	fFactor = max(fFactor, 0.0f);

	BYTE bRed, bBlue, bGreen;
	BYTE bRedShadow, bBlueShadow, bGreenShadow;

	bRed = GetRValue(crBase);
	bBlue = GetBValue(crBase);
	bGreen = GetGValue(crBase);

	bRedShadow = (BYTE)(bRed * fFactor);
	bBlueShadow = (BYTE)(bBlue * fFactor);
	bGreenShadow = (BYTE)(bGreen * fFactor);

	return RGB(bRedShadow, bGreenShadow, bBlueShadow);
}

COLORREF CQuickTab::Lighter(COLORREF crBase, float fFactor) 
{
	ASSERT (fFactor > 1.0f);

	fFactor = max(fFactor, 1.0f);

	BYTE bRed, bBlue, bGreen;
	BYTE bRedHilite, bBlueHilite, bGreenHilite;

	bRed = GetRValue(crBase);
	bBlue = GetBValue(crBase);
	bGreen = GetGValue(crBase);

	bRedHilite = (BYTE)min((int)(bRed * fFactor), 255);
	bBlueHilite = (BYTE)min((int)(bBlue * fFactor), 255);
	bGreenHilite = (BYTE)min((int)(bGreen * fFactor), 255);

	return RGB(bRedHilite, bGreenHilite, bBlueHilite);
}

CSize CQuickTab::FormatText(CString& sText, CDC* pDC, int nWidth)
{
	CRect rect(0, 0, nWidth, 20);
	UINT uFlags = DT_CALCRECT | DT_SINGLELINE | DT_MODIFYSTRING | DT_END_ELLIPSIS;

	::DrawText(pDC->GetSafeHdc(), sText.GetBuffer(sText.GetLength() + 4), -1, rect, uFlags);
	sText.ReleaseBuffer();

	return pDC->GetTextExtent(sText);
}

CQuickTab::CQuickTab()
{
	m_crBack = (COLORREF)-1;

	m_strToolMsg = "";
}

CQuickTab::~CQuickTab()
{

}


BEGIN_MESSAGE_MAP(CQuickTab, CTabCtrl)
	//{{AFX_MSG_MAP(CQuickTab)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchangeQuickTab)
	ON_NOTIFY_REFLECT(NM_CLICK, OnItemClick)
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//###########################################################################
// CQuickTab message handlers
//###########################################################################
BOOL CQuickTab::PreTranslateMessage(MSG* pMsg) 
{
	if(m_ExToolTip.m_hWnd != NULL)
		m_ExToolTip.Update();
 
	return CTabCtrl::PreTranslateMessage(pMsg);
}

int CQuickTab::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_ExToolTip.Create(this);
	m_ExToolTip.AddWindowTool(this, LPSTR_TEXTCALLBACK);


	return 0;
}

void CQuickTab::OnDestroy() 
{
//	CTabCtrl::OnDestroy();
	
	if(m_ExToolTip.m_hWnd != NULL)
		m_ExToolTip.DelTool(this);

	CTabCtrl::OnDestroy();
}

BOOL CQuickTab::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rClient, rTab, rTotalTab, rBkgnd, rEdge;
	COLORREF crBack;
	int nTab, nTabHeight = 0;

	CTabCtrl::OnEraseBkgnd(pDC);

	// calc total tab width
	GetClientRect(rClient);
	nTab = GetItemCount();
	rTotalTab.SetRectEmpty();

	while(nTab--)
	{
		GetItemRect(nTab, rTab);
		rTotalTab.UnionRect(rTab, rTotalTab);
	}

	nTabHeight = rTotalTab.Height();

	// add a bit
	rTotalTab.InflateRect(2, 3);
	rEdge = rTotalTab;

	// then if background color is set, paint the visible background
	// area of the tabs in the bkgnd color
	// note: the mfc code for drawing the tabs makes all sorts of assumptions
	// about the background color of the tab control being the same as the page
	// color - in some places the background color shows thru' the pages!!
	// so we must only paint the background color where we need to, which is that
	// portion of the tab area not excluded by the tabs themselves
	crBack = (m_crBack == -1) ? ::GetSysColor(COLOR_3DFACE) : m_crBack;

	// full width of tab ctrl above top of tabs
	rBkgnd = rClient;
	if (GetStyle() & TCS_BOTTOM)
		rBkgnd.top = rTotalTab.bottom - 3;
	else
		rBkgnd.bottom = rTotalTab.top + 3;
	pDC->SetBkColor(crBack);
	pDC->ExtTextOut(rBkgnd.left, rBkgnd.top, ETO_CLIPPED | ETO_OPAQUE, rBkgnd, "", NULL);
	
	// width of tab ctrl visible bkgnd including bottom pixel of tabs to left of tabs
	rBkgnd = rClient;
	rBkgnd.right = 2;
	if (GetStyle() & TCS_BOTTOM)
		rBkgnd.bottom = rBkgnd.bottom - (nTabHeight + 2);
	else
		rBkgnd.bottom = rBkgnd.top + (nTabHeight + 2);

	pDC->ExtTextOut(rBkgnd.left, rBkgnd.top, ETO_CLIPPED | ETO_OPAQUE, rBkgnd, "", NULL);
	
	// to right of tabs
	rBkgnd = rClient;
	rBkgnd.left += rTotalTab.Width() - 2;

	if (GetStyle() & TCS_BOTTOM)
		rBkgnd.top = rBkgnd.bottom - (nTabHeight + 2);
	else
		rBkgnd.bottom = rBkgnd.top + (nTabHeight + 2);

	pDC->ExtTextOut(rBkgnd.left, rBkgnd.top, ETO_CLIPPED | ETO_OPAQUE, rBkgnd, "", NULL);

	return TRUE;
	//return CTabCtrl::OnEraseBkgnd(pDC);
}

void CQuickTab::OnItemClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	//Item click시 Display된 Tooltip을 제거한다.
	m_ExToolTip.Pop();
	
	*pResult = 0;
}

#if _MSC_VER >= 1400
	LRESULT CQuickTab::OnNcHitTest(CPoint point)
#else
	UINT CQuickTab::OnNcHitTest(CPoint point)
#endif
//UINT CQuickTab::OnNcHitTest(CPoint point) 
{
	TCHITTESTINFO pHitTestInfo; 
	ScreenToClient(&point);
	pHitTestInfo.pt = point;
	pHitTestInfo.flags = TCHT_ONITEM;
	int nIndex = HitTest(&pHitTestInfo);
	
	if (nIndex < 0 )
		return FALSE;
	
	POSITION pos;
	QuickList stQuickList;
	pos = m_plistQuick->FindIndex(nIndex);
	if (pos != NULL)
	{
		stQuickList = m_plistQuick->GetAt(pos);

		if (stQuickList.bCheck == TRUE)
			m_strToolMsg = stQuickList.strFileName;
	}

	return CTabCtrl::OnNcHitTest(point);
}

void CQuickTab::OnSelchangeQuickTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	/*
	TC_ITEM item;
  GetItem(GetCurSel(), &item);
	*/

	POSITION pos;
	QuickList stQuickList;
	//pos = m_plistQuick->FindIndex((int)item.lParam);
	pos = m_plistQuick->FindIndex(m_iaTabPosData.GetAt(GetCurSel()));
	
	if (pos != NULL)
	{
		stQuickList = m_plistQuick->GetAt(pos);
		CWnd* pParent = GetParent();
		pParent->SendMessage(WM_NOTIFY_QUICKTAB, (WPARAM)NM_QUICK_CLICK, (LPARAM)&stQuickList);
	}

	SetCurSel(-1);

	*pResult = 0;
}

BOOL CQuickTab::SetAdd_Item(CString strItem, int nIndex, int nImage, int nPos)
{
	BOOL bInsert = FALSE;
	ShowWindow(SW_SHOW);

	TC_ITEM item;

	item.mask = TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM;
	item.pszText = (LPTSTR)(LPCSTR)strItem;
	item.cchTextMax = strItem.GetLength();
	item.iImage = nImage;
	item.lParam = (int)nPos;
	
	bInsert = InsertItem(nIndex, &item);

	SetMinTabWidth(70);
	SetCurSel(-1);

	return bInsert;
}

void CQuickTab::SetDeleteAll()
{
	DeleteAllItems();
}

void CQuickTab::SetDelete_Item()
{
	int nCurSel = GetCurSel();

	if(nCurSel < 0)
		return;

	DeleteItem(nCurSel);
}

void CQuickTab::DrawItem(LPDRAWITEMSTRUCT lpdis)
{
	int nDebugPoint = 0;
	TC_ITEM     tci;
	CDC* pDC = CDC::FromHandle(lpdis->hDC);
	HIMAGELIST hilTabs = (HIMAGELIST)TabCtrl_GetImageList(GetSafeHwnd());

	BOOL bSelected = (lpdis->itemID == (UINT)GetCurSel());
	BOOL bColor = (s_dwCustomLook & ETC_COLOR);

	CRect rItem(lpdis->rcItem);

	if (bSelected)
		rItem.bottom -= 1;
	else
		rItem.bottom += 2;

	// tab
	// blend from back color to COLOR_3DFACE if 16 bit mode or better
	COLORREF crFrom = GetTabColor(bSelected);
	crFrom = COLOR_WHITE;
	
	if (s_dwCustomLook & ETC_GRADIENT && pDC->GetDeviceCaps(BITSPIXEL) >= 16)
	{
		COLORREF crTo = bSelected ? ::GetSysColor(COLOR_3DFACE) : Darker(!bColor || m_crBack == -1 ? ::GetSysColor(COLOR_3DFACE) : m_crBack, 0.9f);

		int nROrg = GetRValue(crFrom);
		int nGOrg = GetGValue(crFrom);
		int nBOrg = GetBValue(crFrom);
		int nRDiff = GetRValue(crTo) - nROrg;
		int nGDiff = GetGValue(crTo) - nGOrg;
		int nBDiff = GetBValue(crTo) - nBOrg;
		
		int nHeight = rItem.Height();

		for (int nLine = 0; nLine < nHeight; nLine += 2)
		{
			int nRed = nROrg + (nLine * nRDiff) / nHeight;
			int nGreen = nGOrg + (nLine * nGDiff) / nHeight;
			int nBlue = nBOrg + (nLine * nBDiff) / nHeight;
			
			pDC->FillSolidRect(CRect(rItem.left, rItem.top + nLine, rItem.right, rItem.top + nLine + 2),
							   RGB(nRed, nGreen, nBlue));
		}
	}
	else // simple solid fill
		pDC->FillSolidRect(rItem, crFrom);

	// text & icon
	rItem.left += PADDING;
	rItem.top += PADDING + (bSelected ? 1 : 0);

	pDC->SetBkMode(TRANSPARENT);
	
	CString sTemp;
	tci.mask        = TCIF_TEXT | TCIF_IMAGE;
	tci.pszText     = sTemp.GetBuffer(100);
	tci.cchTextMax  = 99;
	GetItem(lpdis->itemID, &tci);
	sTemp.ReleaseBuffer();

	// icon
	if (hilTabs)
	{
		ImageList_Draw(hilTabs, tci.iImage, *pDC, rItem.left, rItem.top, ILD_TRANSPARENT);
		rItem.left += 16 + PADDING;
	}

	// text
	rItem.right -= PADDING;
	FormatText(sTemp, pDC, rItem.Width());

	//pDC->SetTextColor(GetTabTextColor(bSelected));
	pDC->SetTextColor(COLOR_BLACK);
	pDC->DrawText(sTemp, rItem, DT_NOPREFIX | DT_CENTER);
}

void CQuickTab::DrawItemBorder(LPDRAWITEMSTRUCT lpdis)
{
	ASSERT (s_dwCustomLook & ETC_FLAT);

	BOOL bSelected = (lpdis->itemID == (UINT)GetCurSel());
	BOOL bBackTabs = (s_dwCustomLook & ETC_BACKTABS);

	CRect rItem(lpdis->rcItem);
	CDC* pDC = CDC::FromHandle(lpdis->hDC); 

	COLORREF crTab = GetTabColor(bSelected);
	COLORREF crHighlight = Lighter(crTab, 1.5f);
	COLORREF crShadow = Darker(crTab, 0.75f);

	if (bSelected || bBackTabs)
	{
		rItem.bottom += bSelected ? -1 : 1;

		// edges
		pDC->FillSolidRect(CRect(rItem.left, rItem.top, rItem.left + 1, rItem.bottom), crHighlight);
		pDC->FillSolidRect(CRect(rItem.left, rItem.top, rItem.right, rItem.top + 1), crHighlight);
		pDC->FillSolidRect(CRect(rItem.right - 1, rItem.top, rItem.right, rItem.bottom), crShadow);
	}
	else // draw simple dividers
	{
		pDC->FillSolidRect(CRect(rItem.left - 1, rItem.top, rItem.left, rItem.bottom), crShadow);
		pDC->FillSolidRect(CRect(rItem.right - 1, rItem.top, rItem.right, rItem.bottom), crShadow);
	}
}

void CQuickTab::DrawMainBorder(LPDRAWITEMSTRUCT lpdis)
{
	CRect rBorder(lpdis->rcItem);
	CDC* pDC = CDC::FromHandle(lpdis->hDC); 

	COLORREF crTab = GetTabColor();
	COLORREF crHighlight = Lighter(crTab, 1.5f);
	COLORREF crShadow = Darker(crTab, 0.75f);

	pDC->Draw3dRect(rBorder, crHighlight, crShadow);
}

void CQuickTab::SetEnableCustomLook(BOOL bEnable, DWORD dwStyle)
{
	if (!bEnable)
		dwStyle = 0;

	s_dwCustomLook = dwStyle;
}

BOOL CQuickTab::SetEnableDraw(int nType)
{
	ASSERT (nType >= BTC_NONE && nType <= BTC_ALL);

	if (nType < BTC_NONE || nType > BTC_ALL)
		return FALSE;

	m_nDrawType = nType;

	if (GetSafeHwnd())
	{
		if (m_nDrawType != BTC_NONE) 
			ModifyStyle(0, TCS_OWNERDRAWFIXED);
		else
			ModifyStyle(TCS_OWNERDRAWFIXED, 0);

		Invalidate();
	}

	return TRUE;
}

COLORREF CQuickTab::GetTabColor(BOOL bSelected)
{
	BOOL bColor = (s_dwCustomLook & ETC_COLOR);
	BOOL bHiliteSel = (s_dwCustomLook & ETC_SELECTION);
	BOOL bBackTabs = (s_dwCustomLook & ETC_BACKTABS);
	BOOL bFlat = (s_dwCustomLook & ETC_FLAT);

	if (bSelected && bHiliteSel)
	{
		if (bColor)
			return Lighter((m_crBack == -1) ? ::GetSysColor(COLOR_3DFACE) : m_crBack, 1.4f);
		else
			return Lighter(::GetSysColor(COLOR_3DFACE), 1.4f);
	}
	else if (!bSelected)
	{
		if (bBackTabs || !bFlat)
		{
			if (bColor)
				return Darker((m_crBack == -1) ? ::GetSysColor(COLOR_3DFACE) : m_crBack, 0.9f);
			else
				return Darker(::GetSysColor(COLOR_3DFACE), 0.9f);
		}
		else
			return (m_crBack == -1) ? ::GetSysColor(COLOR_3DFACE) : m_crBack;
	}

	// else
	return ::GetSysColor(COLOR_3DFACE);
}

COLORREF CQuickTab::GetTabTextColor(BOOL bSelected)
{
	BOOL bColor = (s_dwCustomLook & ETC_COLOR);
	BOOL bFlat = (s_dwCustomLook & ETC_FLAT);

	if (bSelected)
	{
		return ::GetSysColor(COLOR_WINDOWTEXT);
	}
	else
	{
		if (bColor || bFlat)
			return Darker((m_crBack == -1) ? ::GetSysColor(COLOR_3DFACE) : m_crBack, 0.5f);
		else
			return Darker(::GetSysColor(COLOR_3DFACE), 0.5f);
	}

	// else
	return Darker(::GetSysColor(COLOR_3DFACE), 0.5f);
}

void CQuickTab::SetInitTab(CList<QuickList, QuickList> *plistQuick)
{
	DeleteAllItems();
	m_iaTabPosData.RemoveAll();

	int nAddIndex = 0;
	POSITION pos;
	QuickList stQuickList;
	for (int nIndex = 0; nIndex < plistQuick->GetCount(); nIndex++)
	{
		pos = plistQuick->FindIndex(nIndex);
		stQuickList = plistQuick->GetAt(pos);
		if (stQuickList.bCheck == TRUE)
		{
			SetAdd_Item(stQuickList.strQuickName, nAddIndex, 0, nIndex);
			nAddIndex++;

			m_iaTabPosData.Add(nIndex);
		}
	}
	
	m_plistQuick = plistQuick;
	//Set QuickBtn Item color
	SetEnableDraw(BTC_TABS);

	Invalidate();
}

void CQuickTab::SetBkgndColor(COLORREF color)
{
	m_crBack = color;
	if (GetSafeHwnd())	Invalidate();
}
