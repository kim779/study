// SkinListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SkinListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*****************************************************************
** credits for the custom highlight color bar
** using custom draw technique goes to Michael Dunn
**
** Neat Stuff to do in List Controls Using Custom Draw
** By Michael Dunn 
** http://www.codeproject.com/listctrl/lvcustomdraw.asp?print=true
******************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CSkinListCtrl

CSkinListCtrl::CSkinListCtrl()
{ 
//	m_cBGColor = RGB(115,123,165);
//	m_cFGColor = RGB(229,229,229);
	m_cBGColor = RGB(199,203,221);
	m_cFGColor = RGB(0,0,0);
	m_bUse     = FALSE;
}

CSkinListCtrl::~CSkinListCtrl()
{
}


BEGIN_MESSAGE_MAP(CSkinListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CSkinListCtrl)
	ON_WM_NCCALCSIZE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT ( NM_CUSTOMDRAW, OnCustomDrawList )
//	ON_MESSAGE(WM_SETFONT, OnSetFont)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinListCtrl message handlers
/*
LRESULT CSkinListCtrl::OnSetFont(WPARAM wParam, LPARAM)
{
	LRESULT res =  Default();

	CRect rc;
	GetWindowRect( &rc );

	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rc.Width();
	wp.cy = rc.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage( WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp );

	return res;
}
*/

void CSkinListCtrl::PreSubclassWindow() 
{
	//use our custom CHeaderCtrl
	
	if (m_bUse)
		m_SkinHeaderCtrl.SubclassWindow(GetHeaderCtrl()->m_hWnd);
	
	SetExtendedStyle(GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_OWNERDRAWFIXED);
	CListCtrl::PreSubclassWindow();
}


void CSkinListCtrl::OnCustomDrawList ( NMHDR* pNMHDR, LRESULT* pResult )
{
	return;
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	static bool bHighlighted = false;
	

    // Take the default processing unless we set this to something else below.
    *pResult = CDRF_DODEFAULT;
    // First thing - check the draw stage. If it's the control's prepaint
    // stage, then tell Windows we want messages for every item.
	
    if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        *pResult = CDRF_NOTIFYITEMDRAW;
	}
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
	        const int iRow = (int)pLVCD->nmcd.dwItemSpec;
		
		bHighlighted = IsRowHighlighted(m_hWnd, iRow);
		if (bHighlighted)
		{
			pLVCD->clrText   = m_cFGColor; // Use my foreground hilite color
			pLVCD->clrTextBk = m_cBGColor; // Use my background hilite color
	
			// Turn off listview highlight otherwise it uses the system colors!
			EnableHighlighting(m_hWnd, iRow, false);
		}
		
		// We want item post-paint notifications, so...
		*pResult = CDRF_DODEFAULT | CDRF_NOTIFYPOSTPAINT;
		
	}
	else if(CDDS_ITEMPOSTPAINT == pLVCD->nmcd.dwDrawStage)
	{
	if (bHighlighted)
	{

		const int  iRow = (int)pLVCD->nmcd.dwItemSpec;

        // Turn listview control's highlighting back on now that we have
        // drawn the row in the colors we want.
        EnableHighlighting(m_hWnd, iRow, true);
		
	}

		*pResult = CDRF_DODEFAULT;
  
	}
	
}

void CSkinListCtrl::EnableHighlighting(HWND hWnd, int row, bool bHighlight)
{
	ListView_SetItemState(hWnd, row, bHighlight? 0xff: 0, LVIS_SELECTED);
}

bool CSkinListCtrl::IsRowSelected(HWND hWnd, int row)
{
	return ListView_GetItemState(hWnd, row, LVIS_SELECTED) != 0;
}

bool CSkinListCtrl::IsRowHighlighted(HWND hWnd, int row)
{
  // We check if row is selected.
  // We also check if window has focus. This was because the original listview
  //  control I created did not have style LVS_SHOWSELALWAYS. So if the listview
  //  does not have focus, then there is no highlighting.	

  return IsRowSelected(hWnd, row) /*&& (::GetFocus() == hWnd)*/;
}

void CSkinListCtrl::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	/*****************************************************************
	** credits for this method of removing horz and vert
	** scrollbars goes to Filbert Fox. He posted this solution
	** on the www.codeguru.com discussion boards
	******************************************************************/

	//ensure the scrollbars are hidden

//	ModifyStyle( WS_HSCROLL | WS_VSCROLL, 0);
//	ModifyStyle(WS_HSCROLL, 0);
	UpdateWindow();
	CListCtrl::OnNcCalcSize(bCalcValidRects, lpncsp);
}

BOOL CSkinListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
/*
	switch (((NMHDR*)lParam)->code)
        {
		case HDN_BEGINTRACKW:
		case HDN_BEGINTRACKA:
			*pResult = TRUE;                // disable tracking
			return TRUE;                    // Processed message
        }
*/

	return CListCtrl::OnNotify(wParam, lParam, pResult);
}

void CSkinListCtrl::Selected(int nItem, BOOL scroll /* = TRUE */)
{
	CRect	rc;
	CSize	size;

	SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	if (scroll)
	{
		GetItemRect(0, rc, LVIR_LABEL);
		size.cx = 0;
		size.cy = rc.Height() * (nItem - GetTopIndex());
		
		Scroll(size);
	}
}






