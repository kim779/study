// SearchListCtrl.cpp : implementation file
//

#include "stdafx.h"
//#include "NeoInputEx.h"
#include "SearchListCtrl.h"
#include "SearchList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchListCtrl

CSearchListCtrl::CSearchListCtrl()
{
	m_nHighlight = 1;
	m_nCurFocus = -1;
}

CSearchListCtrl::~CSearchListCtrl()
{
}


BEGIN_MESSAGE_MAP(CSearchListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CSearchListCtrl)
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_NOTIFY_REFLECT(NM_KILLFOCUS, OnKillfocus)
	ON_NOTIFY_REFLECT(HDN_ITEMCLICK, OnItemclick)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchListCtrl message handlers
void CSearchListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcItem(lpDrawItemStruct->rcItem);
	int nItem = lpDrawItemStruct->itemID;
	CImageList* pImageList;

	// Save dc state
	int nSavedDC = pDC->SaveDC();

	// Get item image and state info
	LV_ITEM lvi;
	lvi.mask = LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = nItem;
	lvi.iSubItem = 0;
	lvi.stateMask = 0xFFFF;		// get all state flags
	GetItem(&lvi);

	// Should the item be highlighted
/*	BOOL bHighlight =((lvi.state & LVIS_DROPHILITED)
				|| ( (lvi.state & LVIS_SELECTED)
					&& ((GetFocus() == this)
						|| (GetStyle() & LVS_SHOWSELALWAYS)
						)
					)
				);
*/
	BOOL bHighlight =(lvi.state & LVIS_SELECTED);
/*	if(lvi.state & LVIS_SELECTED)
	{
		CRect oldFocusRect;
		GetItemRect(m_nCurFocus, oldFocusRect, LVIR_BOUNDS);
		if(m_nCurFocus != nItem)
			InvalidateRect(oldFocusRect);
		m_nCurFocus = nItem;
	}
*/
	// Get rectangles for drawing
	CRect rcBounds, rcLabel, rcIcon;
	GetItemRect(nItem, rcBounds, LVIR_BOUNDS);
	GetItemRect(nItem, rcLabel, LVIR_LABEL);
	GetItemRect(nItem, rcIcon, LVIR_ICON);
	CRect rcCol( rcBounds ); 


	CString sLabel = GetItemText( nItem, 0 );

	// Labels are offset by a certain amount  
	// This offset is related to the width of a space character
	int offset = pDC->GetTextExtent(_T(" "), 1 ).cx*2;

	CRect rcHighlight;
	CRect rcWnd;
	int nExt;
	switch( m_nHighlight )
	{
	case 0: 
		nExt = pDC->GetOutputTextExtent(sLabel).cx + offset;
		rcHighlight = rcLabel;
		if( rcLabel.left + nExt < rcLabel.right )
			rcHighlight.right = rcLabel.left + nExt;
		break;
	case 1:
		rcHighlight = rcBounds;
		rcHighlight.left = rcLabel.left;
		break;
	case 2:
		GetClientRect(&rcWnd);
		rcHighlight = rcBounds;
		rcHighlight.left = rcLabel.left;
		rcHighlight.right = rcWnd.right;
		break;
	default:
		rcHighlight = rcLabel;
	}

	// Draw the background color
//	if( nItem == m_nCurFocus )
	if(bHighlight)
	{
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));

		pDC->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_HIGHLIGHT)));
	}
	else
		pDC->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_WINDOW)));

	

	// Set clip region
	rcCol.right = rcCol.left + GetColumnWidth(0);
	CRgn rgn;
	rgn.CreateRectRgnIndirect(&rcCol);
	pDC->SelectClipRgn(&rgn);
	rgn.DeleteObject();

	// Draw state icon
	if (lvi.state & LVIS_STATEIMAGEMASK)
	{
		int nImage = ((lvi.state & LVIS_STATEIMAGEMASK)>>12) - 1;
		pImageList = GetImageList(LVSIL_STATE);
		if (pImageList)
		{
			pImageList->Draw(pDC, nImage,
				CPoint(rcCol.left, rcCol.top), ILD_TRANSPARENT);
		}
	}
	
	// Draw normal and overlay icon
	pImageList = GetImageList(LVSIL_SMALL);
	if (pImageList)
	{
		UINT nOvlImageMask=lvi.state & LVIS_OVERLAYMASK;
		pImageList->Draw(pDC, lvi.iImage, 
			CPoint(rcIcon.left, rcIcon.top),
			(bHighlight?ILD_BLEND50:0) | ILD_TRANSPARENT | nOvlImageMask );
	}

	
	
	// Draw item label - Column 0
	rcLabel.left += offset/2;
	rcLabel.right -= offset;

	pDC->DrawText(sLabel,-1,rcLabel,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP 
				| DT_VCENTER | DT_END_ELLIPSIS);


	// Draw labels for remaining columns
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;

	if( m_nHighlight == 0 )		// Highlight only first column
	{
		pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
		pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
	}
	
	rcBounds.right = rcHighlight.right > rcBounds.right ? rcHighlight.right :
							rcBounds.right;
	rgn.CreateRectRgnIndirect(&rcBounds);
	pDC->SelectClipRgn(&rgn);
				   
	for(int nColumn = 1; GetColumn(nColumn, &lvc); nColumn++)
	{
		rcCol.left = rcCol.right;
		rcCol.right += lvc.cx;

		// Draw the background if needed
		if( m_nHighlight == 0 )
			pDC->FillRect(rcCol, &CBrush(::GetSysColor(COLOR_WINDOW)));

		sLabel = GetItemText(nItem, nColumn);
		if (sLabel.GetLength() == 0)
			continue;

		// Get the text justification
		UINT nJustify = DT_LEFT;
		switch(lvc.fmt & LVCFMT_JUSTIFYMASK)
		{
		case LVCFMT_RIGHT:
			nJustify = DT_RIGHT;
			break;
		case LVCFMT_CENTER:
			nJustify = DT_CENTER;
			break;
		default:
			break;
		}

		rcLabel = rcCol;
		rcLabel.left += offset;
		rcLabel.right -= offset;

		pDC->DrawText(sLabel, -1, rcLabel, nJustify | DT_SINGLELINE | 
					DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
	}

	// Draw focus rectangle if item has focus
//	if (lvi.state & LVIS_FOCUSED && (GetFocus() == this))
//		pDC->DrawFocusRect(rcHighlight);

	
	// Restore dc
	pDC->RestoreDC( nSavedDC );
}

BOOL CSearchListCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style &= ~LVS_TYPEMASK;
	cs.style &= ~LVS_SHOWSELALWAYS;
	cs.style |= LVS_SINGLESEL;
	cs.style |= LVS_REPORT | LVS_OWNERDRAWFIXED;	
	return CListCtrl::PreCreateWindow(cs);	
}

void CSearchListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSearchListCtrl::FP_vEventUpDown(UINT nKey)
{
	CListCtrl::OnKeyDown(nKey, 0, 0);
}

void CSearchListCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CListCtrl::OnKillFocus(pNewWnd);
}

void CSearchListCtrl::OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult) 
{		
	*pResult = 0;
}

void CSearchListCtrl::OnItemclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	
	*pResult = 0;
}

CString CSearchListCtrl::FP_csEditEnter()
{
	POSITION pos = GetFirstSelectedItemPosition();
	if(pos)
	{
		int nSel = GetNextSelectedItem(pos);
		CString csData = GetItemText(nSel , 1);
		return csData;
	}
	return "";
}

void CSearchListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{		
	CListCtrl::OnLButtonDblClk(nFlags, point);
	POSITION pos = GetFirstSelectedItemPosition();
	if(pos)
	{
		int nSel = GetNextSelectedItem(pos);
		CString csData = GetItemText(nSel , 1);
		GetParent()->ShowWindow(SW_HIDE);
		((CSearchList*)GetParent())->FP_vEventSeachList(csData);
	}
}

void CSearchListCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	
	CListCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
	if(nChar == VK_RETURN)
	{
		POSITION pos = GetFirstSelectedItemPosition();
		if(pos)
		{
			int nSel = GetNextSelectedItem(pos);
			CString csData = GetItemText(nSel , 1);
			GetParent()->ShowWindow(SW_HIDE);
			((CSearchList*)GetParent())->FP_vEventSeachList(csData);
		}
	}
}

LRESULT CSearchListCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
//	TRACE("message = %d , wParam = %d , lParam = %d\n",message,wParam,lParam);
	if(message == 641)
	{
		((CSearchList*)GetParent())->FP_vShowList(FALSE);
	}
	return CListCtrl::WindowProc(message, wParam, lParam);
}



// Sort the item in reverse alphabetical order.
static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
   // lParamSort contains a pointer to the list view control.
   // The lParam of an item is just its index.
   CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
   CString    strItem1 = pListCtrl->GetItemText(lParam1, 0);
   CString    strItem2 = pListCtrl->GetItemText(lParam2, 0);

   return strcmp(strItem2, strItem1);
}
void CSearchListCtrl::FP_vSortItem()
{
//	VERIFY( SortItems( CompareFunction, reinterpret_cast<DWORD>( this ) ) );
	SortItems(MyCompareProc, (LPARAM) this);
}