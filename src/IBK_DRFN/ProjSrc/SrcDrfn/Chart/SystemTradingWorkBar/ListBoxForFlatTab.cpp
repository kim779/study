// ListBoxForFlatTab.cpp : implementation file
//

#include "stdafx.h"
#include "systemtradingworkbar.h"
#include "ListBoxForFlatTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListBoxForFlatTab

CListBoxForFlatTab::CListBoxForFlatTab()
{
	LOGFONT lf;
	memset(&lf, 0x00, sizeof(LOGFONT));       
	lf.lfHeight = 12;
	::lstrcpy(lf.lfFaceName, "±¼¸²Ã¼");
	VERIFY(m_font.CreateFontIndirect(&lf));
}

CListBoxForFlatTab::~CListBoxForFlatTab()
{
	m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CListBoxForFlatTab, CListBox)
	//{{AFX_MSG_MAP(CListBoxForFlatTab)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DRAWITEM_REFLECT()
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDM_CLEAR, OnClear)
	ON_COMMAND(IDM_COPYTEXT, OnCopytext)
	ON_UPDATE_COMMAND_UI(IDM_COPYTEXT, OnUpdateCopytext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListBoxForFlatTab message handlers

void CListBoxForFlatTab::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CListBox::OnWindowPosChanged(lpwndpos);
	
    ShowScrollBar(SB_HORZ, FALSE);
    ModifyStyle(WS_HSCROLL, 0, SWP_DRAWFRAME);
}

void CListBoxForFlatTab::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if ((int)lpDrawItemStruct->itemID < 0)
		return; 

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	COLORREF crText;
	CString sText;
	
	COLORREF crNorm = lpDrawItemStruct->itemData;		// Color information is in item data.
	COLORREF crHilite = RGB(255-GetRValue(crNorm), 255-GetGValue(crNorm), 255-GetBValue(crNorm));

	// If item has been selected, draw the highlight rectangle using the item's color.
	if ((lpDrawItemStruct->itemState & ODS_SELECTED) &&
		 (lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		if(!crNorm)
		{
			CBrush brush(::GetSysColor(COLOR_HIGHLIGHT));
			pDC->FillRect(&lpDrawItemStruct->rcItem, &brush);
		}
		else
		{
			CBrush brush(crNorm);
			pDC->FillRect(&lpDrawItemStruct->rcItem, &brush);
		}
	}

	// If item has been deselected, draw the rectangle using the window color.
	if (!(lpDrawItemStruct->itemState & ODS_SELECTED) &&	(lpDrawItemStruct->itemAction & ODA_SELECT))
	{
		CBrush brush(::GetSysColor(COLOR_WINDOW));
		pDC->FillRect(&lpDrawItemStruct->rcItem, &brush);
	}	 	

	// If item has focus, draw the focus rect.
	if ((lpDrawItemStruct->itemAction & ODA_FOCUS) && (lpDrawItemStruct->itemState & ODS_FOCUS))
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem); 

	// If item does not have focus, redraw (erase) the focus rect.
	if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&	!(lpDrawItemStruct->itemState & ODS_FOCUS))
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem); 

	// Set the background mode to TRANSPARENT to draw the text.
	int nBkMode = pDC->SetBkMode(TRANSPARENT);

	// If the item's color information is set, use the highlight color
	// gray text color, or normal color for the text.
	if (lpDrawItemStruct->itemData)		
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			crText = pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		else if (lpDrawItemStruct->itemState & ODS_DISABLED)
			crText = pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
		else
			crText = pDC->SetTextColor(crNorm);
	}
	// Else the item's color information is not set, so use the
	// system colors for the text.
	else
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			crText = pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		else if (lpDrawItemStruct->itemState & ODS_DISABLED)
			crText = pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
		else
			crText = pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	}


	// Get and display item text.
	GetText(lpDrawItemStruct->itemID, sText);
	CRect rect = lpDrawItemStruct->rcItem;

	// Setup the text format.
	UINT nFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	if (GetStyle() & LBS_USETABSTOPS)
		nFormat |= DT_EXPANDTABS;
	
	CFont* pFontOld = pDC->SelectObject(&m_font);

	// Calculate the rectangle size before drawing the text.
	pDC->DrawText(sText, -1, &rect, nFormat | DT_CALCRECT);
	pDC->DrawText(sText, -1, &rect, nFormat);

	pDC->SelectObject(pFontOld);
	pDC->SetTextColor(crText); 
	pDC->SetBkMode(nBkMode);	
}


void CListBoxForFlatTab::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here
	
}

int CListBoxForFlatTab::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_mnuPopup.LoadMenu(IDR_MENU_POPUP);
	
	return 0;
}

void CListBoxForFlatTab::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CListBox::OnRButtonDown(nFlags, point);

	CMenu* pPopupMenu = m_mnuPopup.GetSubMenu(1);
	ClientToScreen(&point);

	CXTPCommandBars::TrackPopupMenu(pPopupMenu, TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CListBoxForFlatTab::OnClear() 
{
	int nCount = GetCount();
	for(int i = nCount - 1; i >= 0; i--)
		DeleteString(i);
}

void CListBoxForFlatTab::OnCopytext() 
{
	CString strText(_T(""));
	GetText(GetCurSel(), strText);

	if(strText.IsEmpty()) 
		return;

	CWaitCursor wc;

	if(OpenClipboard())
	{
		EmptyClipboard();
		HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, strText.GetLength() + 1);
		if(hData)
		{
			LPSTR pszData = (LPSTR)::GlobalLock(hData);
			USES_CONVERSION;
			::lstrcpy(pszData, T2A((LPTSTR)(LPCTSTR)strText));
			GlobalUnlock(hData);
			SetClipboardData(CF_TEXT, hData);
		}
		CloseClipboard();
	}
}

void CListBoxForFlatTab::OnUpdateCopytext(CCmdUI* pCmdUI) 
{
	int nSel = GetCurSel();
	if(nSel == LB_ERR)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
}
