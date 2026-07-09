// ToolTipListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ToolTipListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolTipListCtrl

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	SortInformation *p_sort_data = (SortInformation *)lParamSort;
	CListCtrl *p_list_ctrl = p_sort_data->p_list_ctrl;
	char sort_type = p_sort_data->sort_type;
	int first_index = 0, second_index = 0, sort_value[2] = {1, -1};

	// 검색 타입과 정보를 설정하여 FindItem 함수를 통하여 설정된 검색 정보와 
	// 동일 데이터의 위치 값을 얻는다.
	LV_FINDINFO find_data;
	find_data.flags = LVFI_PARAM | LVFI_WRAP;
	find_data.lParam = lParam1;
	first_index = p_list_ctrl->FindItem(&find_data);

	find_data.lParam = lParam2;
	second_index = p_list_ctrl->FindItem(&find_data, first_index);

	// 검색된 위치의 데이터를 얻어 두 개의 데이터를 비교하여 
	// 그에 따른 결과 값을 반환한다.	
	CString string1 = p_list_ctrl->GetItemText(first_index, p_sort_data->column_index);
	CString string2 = p_list_ctrl->GetItemText(second_index, p_sort_data->column_index);

	return string1.Compare(string2)*sort_value[sort_type];
}

CToolTipListCtrl::CToolTipListCtrl()
{
	m_wHitMask = INITIAL_HITMASK;
	m_sort_data.sort_type = -1;
	m_sort_data.p_list_ctrl = this;
}

CToolTipListCtrl::~CToolTipListCtrl()
{
	// Cleanup when destroyed
	
	DeleteAllToolTips();
}


BEGIN_MESSAGE_MAP(CToolTipListCtrl, CListCtrl)
//{{AFX_MSG_MAP(CToolTipListCtrl)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP

//Trap all TTN_NEEDTEXT* Messages
//TTN_NEEDTEXT* messages are sent when a ToolTipCtrl wants a control
//to provide it with text to display as the tooltip.
//Specifically, when the TOOLINFO structure passed back to the ToolTipCtrl
//after ::OnToolHitTest has it's lpszText memeber set to LPSTR_TEXTCALLBACK.

ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
ON_NOTIFY_REFLECT_EX(LVN_COLUMNCLICK, OnColumnClick)
ON_NOTIFY_REFLECT_EX(LVN_ITEMCHANGED, OnItemchangedListFindtext)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolTipListCtrl message handlers

int CToolTipListCtrl::OnToolHitTest(CPoint point, TOOLINFO * pTI) const {
	
	//See if the point falls onto a list item
	//UINT nFlags = 0;
	
	LVHITTESTINFO lvhitTestInfo;
	
	lvhitTestInfo.pt	= point;
	
	int nItem = ListView_SubItemHitTest(
		this->m_hWnd,
		&lvhitTestInfo);
	int nSubItem = lvhitTestInfo.iSubItem;
	
	UINT nFlags =   lvhitTestInfo.flags;
	
	//nFlags is 0 if the SubItemHitTest fails
	//Therefore, 0 & <anything> will equal false
	if (nFlags & m_wHitMask){
		//If it did fall on a list item,
		//and it was also hit one of the
		//item specific sub-areas we wish to show tool tips for
		
		//Get the client (area occupied by this control
		RECT rcClient;
		GetClientRect( &rcClient );
		
		//Fill in the TOOLINFO structure
		pTI->hwnd = m_hWnd;
		pTI->uId = (UINT) (nItem * 100 + nSubItem);
		pTI->lpszText = LPSTR_TEXTCALLBACK;
		pTI->rect = rcClient;
		
		return pTI->uId; //By returning a unique value per listItem,
		//we ensure that when the mouse moves over another list item,
		//the tooltip will change
	}else{
		//Otherwise, we aren't interested, so let the message propagate
		return -1;
	}
}

BOOL CToolTipListCtrl::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult ){
	//Handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	
	//Ignore messages from the built in tooltip, we are processing them internally
	if( (pNMHDR->idFrom == (UINT)m_hWnd) &&
		( ((pNMHDR->code == TTN_NEEDTEXTA) && (pTTTA->uFlags & TTF_IDISHWND)) ||
		((pNMHDR->code == TTN_NEEDTEXTW) && (pTTTW->uFlags & TTF_IDISHWND)) ) ){
		return FALSE;
	}
	
	
	*pResult = 0;
	
	CString strTipText;
	
	//Get the mouse position
	const MSG* pMessage;
	pMessage = GetCurrentMessage();
	ASSERT ( pMessage );
	CPoint pt;
	pt = pMessage->pt; //Get the point from the message
	ScreenToClient( &pt ); //Convert the point's coords to be relative to this control
	
	//See if the point falls onto a list item
	
	LVHITTESTINFO lvhitTestInfo;
	
	lvhitTestInfo.pt	= pt;
	
	int nItem = SubItemHitTest(&lvhitTestInfo);
	int nSubItem = lvhitTestInfo.iSubItem;
	
	UINT nFlags =   lvhitTestInfo.flags;
	
	//nFlags is 0 if the SubItemHitTest fails
	//Therefore, 0 & <anything> will equal false
	if( nFlags & m_wHitMask ){
		//If it did fall on a list item,
		//and it was also hit one of the
		//item specific sub-areas we wish to show tool tips for
		
		//Lookup the list item's text in the ToolTip Map
		
		CString strKey;
		
		strKey.Format(_T("%d"),  nItem * 100 + nSubItem);
		
		if( m_ToolTipMap.Lookup(strKey, strTipText ) ){
			//If there was a CString associated with the list item,
			//copy it's text (up to 80 characters worth, limitation of the TOOLTIPTEXT structure)
			//into the TOOLTIPTEXT structure's szText member
			
			//Deal with UNICODE
#ifndef _UNICODE
			if (pNMHDR->code == TTN_NEEDTEXTA)
				lstrcpyn(pTTTA->szText, strTipText, 80);
			else
				_mbstowcsz(pTTTW->szText, strTipText, 80);
#else
			if (pNMHDR->code == TTN_NEEDTEXTA)
				_wcstombsz(pTTTA->szText, strTipText, 80);
			else
				lstrcpyn(pTTTW->szText, strTipText, 80);
#endif
			return FALSE;    //We found a tool tip,
			//tell the framework this message has been handled
			
			////////////////////////////////////////////////////////////////////////////////
			// ****** Special note *****
			//
			// Still don't understand why the function must return FALSE for CListCtrl
			// so as not to cause flickering, as opposed to Nate Maynard's derivation
			// from CTreeCtrl.
			// I have experimented with disabling Tooltips for the control
			// and found out that a "ghost" tooltip appears for a fraction of a second...
			//
			// I am completely at a loss...
			// Seems to work, though...
			//
			////////////////////////////////////////////////////////////////////////////////
			
		}
	}
	
	return FALSE; //We didn't handle the message,
	//let the framework continue propagating the message
}

//Sets the tooltip text for a specific item
BOOL CToolTipListCtrl::SetItemToolTipText( int nItem, int nSubItem, LPCTSTR lpszToolTipText ){
	
	CString strKey;
	
	strKey.Format(_T("%d"),  nItem * 100 + nSubItem);
	
	m_ToolTipMap.SetAt( strKey, lpszToolTipText );
	
	return TRUE;
	
}

//Retrieve the tooltip text for a specific list item
CString CToolTipListCtrl::GetItemToolTipText( int nItem, int nSubItem){
	
	CString itemToolTipText;
	
	CString strKey;
	
	strKey.Format(_T("%d"),  nItem * 100 + nSubItem);
	
	
	if( !m_ToolTipMap.Lookup( strKey, itemToolTipText ) ){
		itemToolTipText = "";
	}
	
	return itemToolTipText;
}


WORD CToolTipListCtrl::SetToolTipHitMask( WORD wHitMask ){
	WORD oldHitMask = m_wHitMask;
	
	m_wHitMask = wHitMask;
	
	return oldHitMask;
}

void CToolTipListCtrl::DeleteAllToolTips()
{
	m_ToolTipMap.RemoveAll();
}

BOOL CToolTipListCtrl::DeleteAllItems( ){
	//Call the base class method
	BOOL retVal = CListCtrl::DeleteAllItems();
	
	if( retVal ){
		//If it succeeded, remove all tooltips
		DeleteAllToolTips();
	}
	
	return retVal;
}

BOOL CToolTipListCtrl::DeleteItem( int nItem ){
	
	//Call the base class method
	BOOL retVal = CListCtrl::DeleteItem( nItem );
	
	if( retVal ){
		//If it succeeded, remove it's tooltip from the map
		
		LVCOLUMN *lvColumn = (LVCOLUMN*) malloc (sizeof(LVCOLUMN));
		int i = 0;
		for (i = 0; GetColumn(i, lvColumn) != 0; i++);
		{
			CString strKey;
			
			strKey.Format(_T("%d"),  nItem * 100 + i);
			
			m_ToolTipMap.RemoveKey( strKey );
		}
	}
	
	return retVal;
}


BOOL CToolTipListCtrl::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW* p_view = (NMLISTVIEW*)pNMHDR;

	if(p_view->iSubItem == m_sort_data.column_index) 
		m_sort_data.sort_type = !m_sort_data.sort_type;
	else 
		m_sort_data.sort_type = 1;
	
	m_sort_data.column_index = p_view->iSubItem;
	SortItems(CompareFunc, (DWORD)&m_sort_data);

	*pResult = 0;
	return TRUE;
}

BOOL CToolTipListCtrl::OnItemchangedListFindtext(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	if (!(pNMLV->uChanged == LVIF_STATE && pNMLV->uNewState == (LVIS_SELECTED | LVIS_FOCUSED)))
		return FALSE;

	int index = pNMLV->iItem; 

	return TRUE;
}

