// ComboInListView.cpp : implementation file
//

// The code contained in this file is based on the original
// CInPlaceList from http://www.codeguru.com/listview


#include "stdafx.h"
#include "ComboInListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboInListView
CComboInListView::CComboInListView(int iItem, int iSubItem, CStringArray *plstItems, CDWordArray *parrIDs)
{	
	m_iItem = iItem;
	m_iSubItem = iSubItem;
	m_lstItems.Copy(*plstItems);
	if (parrIDs) m_arrIDs.Copy(*parrIDs);
	m_bVK_ESCAPE = 0;
}

CComboInListView::~CComboInListView()
{
}

BEGIN_MESSAGE_MAP(CComboInListView, CComboBox)	
//{{AFX_MSG_MAP(CComboInListView)
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_WM_NCDESTROY()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// 


int CComboInListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)		
		return -1;	
	CFont* font = GetParent()->GetFont();	
	SetFont(font);
	//add the items from CStringlist
	for (int i = 0; i < m_lstItems.GetSize(); i++) 
		AddString((LPCTSTR)(m_lstItems[i]));	
	for (int i = 0; i < m_arrIDs.GetSize(); i++) {
		if (i < m_lstItems.GetSize())
			SetItemData(i, m_arrIDs[i]);
	}
	SetFocus();	
	return 0;
}

BOOL CComboInListView::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )	
	{		
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)	
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);			
			return 1;
		}	
	}	
	return CComboBox::PreTranslateMessage(pMsg);
}


void CComboInListView::OnKillFocus(CWnd* pNewWnd) 
{	
	int nIndex = GetCurSel();

	CComboBox::OnKillFocus(pNewWnd);
 
	CString str;	
	GetWindowText(str);
	// Send Notification to parent of ListView ctrl	
	LV_DISPINFO lvDispinfo;
	lvDispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	lvDispinfo.hdr.idFrom = GetDlgCtrlID();//that's us
	lvDispinfo.hdr.code = LVN_ENDLABELEDIT;
	lvDispinfo.item.mask = LVIF_TEXT | LVIF_PARAM;	
	lvDispinfo.item.iItem = m_iItem;
	lvDispinfo.item.iSubItem = m_iSubItem;
	lvDispinfo.item.pszText = m_bVK_ESCAPE ? NULL : LPTSTR((LPCTSTR)str);
	lvDispinfo.item.cchTextMax = str.GetLength();
	lvDispinfo.item.lParam = GetItemData(GetCurSel());
	if(nIndex!=CB_ERR)
		GetParent()->GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&lvDispinfo);
	PostMessage(WM_CLOSE);
	
}

//need to catch the VK_ESCAPE for the notification msg
void CComboInListView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == VK_ESCAPE || nChar == VK_RETURN)	
	{		
		if( nChar == VK_ESCAPE)
			m_bVK_ESCAPE = 1;
		GetParent()->SetFocus();		
		return;	
	}	
	CComboBox::OnChar(nChar, nRepCnt, nFlags);
}

//doing this hence we are "modaless" and need to clean up me self
void CComboInListView::OnNcDestroy() 
{
	CComboBox::OnNcDestroy();		
	delete this;
}

void CComboInListView::OnCloseup() 
{
	GetParent()->SetFocus();
}

void CComboInListView::OnSize(UINT nType, int cx, int cy) 
{
	CComboBox::OnSize(nType, cx, cy);
}

void CComboInListView::OnSelchange()
{
	int nIndex = GetCurSel();

	CString str;	
	GetWindowText(str);

	// Send Notification to parent of ListView ctrl	
	LV_DISPINFO lvDispinfo;
	lvDispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	lvDispinfo.hdr.idFrom = GetDlgCtrlID();//that's us
	lvDispinfo.hdr.code = LVN_ITEMCHANGED;
	lvDispinfo.item.mask = LVIF_TEXT | LVIF_PARAM;	
	lvDispinfo.item.iItem = m_iItem;
	lvDispinfo.item.iSubItem = m_iSubItem;
	lvDispinfo.item.pszText = m_bVK_ESCAPE ? NULL : LPTSTR((LPCTSTR)str);
	lvDispinfo.item.cchTextMax = str.GetLength();
	lvDispinfo.item.lParam = GetItemData(GetCurSel());
	if(nIndex!=CB_ERR)
		GetParent()->GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&lvDispinfo);
}
