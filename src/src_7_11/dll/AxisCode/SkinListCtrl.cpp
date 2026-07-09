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
** using cushttp://sstatic.naver.com/keypage/html/seoul_alln3.htmltom draw technique goes to Michael Dunn
**
** Neat Stuff to do in List Controls Using Custom Draw
** By Michael Dunn 
** http://www.codeproject.com/listctrl/lvcustomdraw.asp?print=true
******************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CSkinListCtrl

CSkinListCtrl::CSkinListCtrl()
{
	m_cBGColor = RGB(199,203,221);
	m_cFGColor = RGB(0,0,0);
	m_bUse     = FALSE;
	m_nAtmCallIdx  = -1;
	m_nAtmPutIdx  = -1;

	m_bCustomDrawList = TRUE;
}

CSkinListCtrl::~CSkinListCtrl()
{
}


BEGIN_MESSAGE_MAP(CSkinListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CSkinListCtrl)
	ON_WM_NCCALCSIZE()
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT ( NM_CUSTOMDRAW, OnCustomDrawList )
//	ON_MESSAGE(WM_SETFONT, OnSetFont)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinListCtrl message handlers

void CSkinListCtrl::PreSubclassWindow() 
{
	//use our custom CHeaderCtrl
	
	if (m_bUse)
		m_SkinHeaderCtrl.SubclassWindow(GetHeaderCtrl()->m_hWnd);
	
	SetExtendedStyle(GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_OWNERDRAWFIXED);
	CListCtrl::PreSubclassWindow();
}


void CSkinListCtrl::OnCustomDrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	static bool bHighlighted = false;

    // Take the default processing unless we set this to something else below.
    *pResult = CDRF_DODEFAULT;
    // First thing - check the draw stage. If it's the control's prepaint
    // stage, then tell Windows we want messages for every item.
	
	if (!m_bCustomDrawList)
		return;

    if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        *pResult = CDRF_NOTIFYITEMDRAW;
	}
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        int iRow = (int)pLVCD->nmcd.dwItemSpec;
		
		bHighlighted = IsRowHighlighted(m_hWnd, iRow);
		if (bHighlighted)
		{
			pLVCD->clrText   = m_cFGColor; // Use my foreground hilite color
			pLVCD->clrTextBk = m_cBGColor; // Use my background hilite color
	
			// Turn off listview highlight otherwise it uses the system colors!
			EnableHighlighting(m_hWnd, iRow, false);
		}

		if (m_nAtmCallIdx == iRow || m_nAtmPutIdx == iRow)
		{
			pLVCD->clrText   = RGB(255, 255, 255);
			pLVCD->clrTextBk = RGB(148, 148, 210);
		}
		
		// We want item post-paint notifications, so...
		*pResult = CDRF_DODEFAULT | CDRF_NOTIFYPOSTPAINT;
		
	}
	else if(CDDS_ITEMPOSTPAINT == pLVCD->nmcd.dwDrawStage)
	{
		if (bHighlighted)
		{
			int  iRow = (int)pLVCD->nmcd.dwItemSpec;

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
	UpdateWindow();
	CListCtrl::OnNcCalcSize(bCalcValidRects, lpncsp);
}

BOOL CSkinListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{

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

void CSkinListCtrl::SetAtmCallIndex(int nIdx)
{
	m_nAtmCallIdx = nIdx;
}

void CSkinListCtrl::SetAtmPutIndex(int nIdx)
{
	m_nAtmPutIdx = nIdx;
}

void CSkinListCtrl::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	LVITEM& item = pDispInfo->item;

	if (item.mask & LVIF_TEXT)
	{
		if (m_arrayCustomItem.GetSize() <= item.iItem)
		{
			*pResult = 0;
			return;
		}
		
		LISTITEM* pitem =(LISTITEM*)m_arrayCustomItem.GetAt(item.iItem);
		switch(item.iSubItem)
		{
		case 0:
			item.pszText = (char*)pitem->Code;
			break;
		case 1:
			item.pszText = (char*)pitem->Name;
			break;
		}
	}
	*pResult = 0;
}

void CSkinListCtrl::AddCustomItem(DWORD dwData)
{
	m_arrayCustomItem.Add(dwData);
}


void CSkinListCtrl::SortCustomItem(LPARAM lParamSort, BOOL bAscending)
{
	QuickSortRe(0, (int)m_arrayCustomItem.GetSize()-1, lParamSort, bAscending);
	Invalidate();
}

void CSkinListCtrl::RemoveAllCustomItem()
{
	int count = (int)m_arrayCustomItem.GetSize();
	LISTITEM* pitem;
	for (int ii = count - 1; ii >= 0; ii--)
	{
		pitem =(LISTITEM*)m_arrayCustomItem.GetAt(ii);
		SAFE_DELETE(pitem);
	}
	m_arrayCustomItem.RemoveAll();
	Invalidate();
}

void CSkinListCtrl::QuickSortRe(int low, int high, LPARAM lParamSort, BOOL bAscending)
{
	LISTITEM* pitem1 = NULL;
	LISTITEM* pitem2 = NULL;
	LISTITEM* ptemp = NULL;

	int ii, jj;
	ii = high;
	jj = low;
	pitem1 = (LISTITEM*)m_arrayCustomItem[((int)((low+high) / 2))];

	do {
		if ((int)lParamSort == 0)
		{
			if (!bAscending) 
			{
				while (strcmp((char*)((LISTITEM*)m_arrayCustomItem[jj])->Code, (char*)pitem1->Code) < 0) jj++;
				while (strcmp((char*)((LISTITEM*)m_arrayCustomItem[ii])->Code, (char*)pitem1->Code) > 0) ii--;
			}
			else
			{
				while (strcmp((char*)((LISTITEM*)m_arrayCustomItem[jj])->Code, (char*)pitem1->Code) > 0) jj++;
				while (strcmp((char*)((LISTITEM*)m_arrayCustomItem[ii])->Code, (char*)pitem1->Code) < 0) ii--;
			}
		}
		else if ((int)lParamSort == 1)
		{
			if (!bAscending) 
			{
				
				while (strcmp((char*)((LISTITEM*)m_arrayCustomItem[jj])->Name, (char*)pitem1->Name) < 0) jj++;
				while (strcmp((char*)((LISTITEM*)m_arrayCustomItem[ii])->Name, (char*)pitem1->Name) > 0) ii--;
			}
			else
			{
				while (strcmp((char*)((LISTITEM*)m_arrayCustomItem[jj])->Name, (char*)pitem1->Name) > 0) jj++;
				while (strcmp((char*)((LISTITEM*)m_arrayCustomItem[ii])->Name, (char*)pitem1->Name) < 0) ii--;
			}
		}

		if (ii >= jj) 
		{
			if (ii != jj) 
			{
				ptemp = (LISTITEM*)m_arrayCustomItem[ii];
				m_arrayCustomItem[ii] = m_arrayCustomItem[jj];
				m_arrayCustomItem[jj] = (DWORD)ptemp;
			}
			ii--;
			jj++;
		}
	} while (jj <= ii);

	if (low < ii) 
		QuickSortRe(low, ii, lParamSort);
	if (jj < high) 
		QuickSortRe(jj, high, lParamSort);
}