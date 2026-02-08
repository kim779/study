/* Standard Disclaimer: 
Copyright (C) 2000  Dennis Howard
This file is free software; you can redistribute it and/or
modify it without any conditions. There is no warranty,
implied or expressed, as to validity or fitness for a particular purpose.
*/

// TreeCombo.cpp : implementation file
//

#include "stdafx.h"
#include "TreeCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//non top level child control ids
#define IDC_COMBOTREE_BUTTON 4317         
#define IDC_COMBOTREE_EDIT   4318 
        

/////////////////////////////////////////////////////////////////////////////
// CTreeCombo

CTreeCombo::CTreeCombo()
{
	m_Tree.SetCombo (this);
	m_ComboWidth = 200;
	m_ComboHeight = 200;
	m_bDroppedState = FALSE;
}

CTreeCombo::~CTreeCombo()
{
}


BEGIN_MESSAGE_MAP(CTreeCombo, CWnd)
	//{{AFX_MSG_MAP(CTreeCombo)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED (IDC_COMBOTREE_BUTTON, OnDropdownButton )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeCombo message handlers

LRESULT CTreeCombo::SendParentComboMessage (UINT Action)
{
	CWnd* pParent = GetParent ();
	if (pParent && pParent->GetSafeHwnd ())
	{
		return pParent->SendMessage (WM_COMMAND, MAKEWPARAM( GetDlgCtrlID(), Action),(LPARAM) GetSafeHwnd ());
	}

	return 0L;
}

void CTreeCombo::SetWindowText (LPCTSTR Text)
{
	m_Edit.SetWindowText (Text);
	Invalidate ();
}


CString CTreeCombo::GetWindowText ()
{
	CString Text;
	m_Edit.GetWindowText(Text);
	return Text;
}

int CTreeCombo::GetLBText (HTREEITEM hItem, CString& rText)
{
  rText = _T("");
  int retval = CB_ERR;
  rText = m_Tree.GetItemText (hItem);
  if (rText.GetLength () > 0)
  {
	retval = 0;
  }
  return retval;
}

TCHAR CTreeCombo::GetPathDelimiter ()
{
	return m_Tree.GetPathDelimiter ();
}

void CTreeCombo::SetPathDelimiter (TCHAR Delimiter)
{
	m_Tree.SetPathDelimiter (Delimiter);
}

void CTreeCombo::OnDropdownButton()
{

    if (GetDroppedState ())
	{
		//If the drop down button was clicked on while the tree was open,
		//the tree's OnKillFocus () will close it. Only need to clear the 
		//flag here.
	    SetDroppedState (FALSE);
	}
	else
	{
		DisplayTree ();
		SendParentComboMessage (CBN_DROPDOWN);
	}
	m_Edit.Invalidate ();
}


void CTreeCombo::CalculateDroppedRect(LPRECT lpDroppedRect)
{
	_ASSERTE (lpDroppedRect);

	if (!lpDroppedRect)
	{
		return;
	}

	CRect rectCombo;
	GetWindowRect(&rectCombo);

	//adjust to either the top or bottom
	int DropTop = rectCombo.bottom;
	int ScreenHeight = GetSystemMetrics (SM_CYSCREEN);
	if ((DropTop + m_ComboHeight) > ScreenHeight)
	{
		DropTop = rectCombo.top - m_ComboHeight;
	}

	//adjust to either the right or left
	int DropLeft = rectCombo.left;
	int ScreenWidth = GetSystemMetrics (SM_CXSCREEN);
	if ((DropLeft + m_ComboWidth) > ScreenWidth)
	{
		DropLeft = rectCombo.right - m_ComboWidth;
	}

	lpDroppedRect->left  = DropLeft;
	lpDroppedRect->top   = DropTop;
	lpDroppedRect->bottom = DropTop + m_ComboHeight;
	lpDroppedRect->right  = DropLeft + m_ComboWidth;

}

void CTreeCombo::DisplayTree()
{
	CRect rect(0,0,200,200);

	CalculateDroppedRect (&rect);

	m_Tree.InitDisplay ();

	if(&wndNoTopMost != NULL) {
		m_Tree.SetWindowPos (&wndNoTopMost, rect.left, rect.top,
			                 rect.Width (), rect.Height (), SWP_SHOWWINDOW );
	}

	m_BeginPath = m_Tree.GetCurrentTreePath ();

	SetDroppedState (TRUE);
}


void CTreeCombo::OnCancel ()
{
	//restore inital path
	m_Tree.SelectString (m_BeginPath);
	SendParentComboMessage (CBN_SELENDCANCEL);
}

BOOL CTreeCombo::CreateTree ()
{	
	CWnd* pParent = GetParent ();
	if (!pParent && pParent->GetSafeHwnd ())
	{
		return FALSE;
	}

	CRect treeRect (0,0, m_ComboWidth, m_ComboHeight);

	//can't have a control Id with WS_POPUP style
   /* if (!m_Tree.CreateEx(0,
		                   WC_TREEVIEW, NULL,
						   	WS_POPUP | WS_BORDER | TVS_SINGLEEXPAND | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
							treeRect, pParent, 0, NULL))*/
		if (!m_Tree.CreateEx(NULL,WS_POPUP | WS_BORDER | TVS_SINGLEEXPAND | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
			treeRect, pParent, 0)) //tour2k 2022.7.27
	{
		return FALSE;
	}


	return TRUE;
}


BOOL CTreeCombo::CreateButton ()
{	
	CRect btnRect;
	GetClientRect (&btnRect);

	btnRect.left = btnRect.right - btnRect.Height();
	btnRect.NormalizeRect();

	m_DropDownButton.Create(_T("..."), 
							WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 
							btnRect, 
							this, 
							IDC_COMBOTREE_BUTTON);

	m_DropDownButton.MoveWindow (&btnRect);

	return TRUE;
}


//Requires that dropdown button be created first, to size it properly
BOOL CTreeCombo::CreateEdit ()
{	
	if (!m_DropDownButton.GetSafeHwnd ())
	{
		return FALSE;
	}

	CRect clientRect;
	GetClientRect (&clientRect);

	CRect btnRect;
	m_DropDownButton.GetWindowRect (&btnRect);
	ScreenToClient (&btnRect);

	CRect editRect;
	editRect.left = clientRect.left;
	editRect.top = clientRect.top;
	editRect.bottom = clientRect.bottom;
	editRect.right = btnRect.left;

	BOOL bCreate = m_Edit.Create(WS_CHILD |  WS_TABSTOP | WS_VISIBLE ,  editRect, this, IDC_COMBOTREE_EDIT);


	if (bCreate)
	{
		CWnd* pParent = GetParent ();
		CFont* pFont = pParent->GetFont ();
		m_Edit.SetFont (pFont);
	}

	return bCreate;
}

//Removes the original ComboBox control instead of subclassing it
BOOL CTreeCombo::SubclassDlgItem (UINT nID, CWnd* pParent)
{
	CComboBox* pCombo = static_cast<CComboBox*>(pParent->GetDlgItem (nID));
	if (!pCombo)
	{
		return FALSE;
	}

	CRect DroppedRect;
	pCombo->GetDroppedControlRect (&DroppedRect);
	m_ComboWidth = DroppedRect.Width ();
	m_ComboHeight = DroppedRect.Height ();


 	CRect rect;
	pCombo->GetWindowRect (&rect);
	pParent->ScreenToClient (&rect);

	CString className = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS,
											::LoadCursor(NULL, IDC_ARROW),
											(HBRUSH) ::GetStockObject(WHITE_BRUSH));


   BOOL bCreate =  CreateEx(WS_EX_CLIENTEDGE, // 3D  client edge 
							className, 
							NULL,
							WS_CHILD | WS_VISIBLE | WS_TABSTOP,
							rect, 
							pParent, 
							nID);

	_ASSERTE (bCreate);

	if (!bCreate)
	{
		return FALSE;
	}

	//Set Z Order to follow after original combo
	SetWindowPos (pCombo, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);

	UpdateWindow ();

	pCombo->DestroyWindow (); //remove original window

	return TRUE; 
}

int CTreeCombo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!CreateTree())
	{
		return -1;
	}

	if (!CreateButton())
	{
		return -1;
	}

	if (!CreateEdit())
	{
		return -1;
	}

	return 0;
}


CString CTreeCombo::GetTreePath (HTREEITEM hItem)
{
	return m_Tree.GetTreePath (hItem);
}

CString CTreeCombo::GetCurrentTreePath ()
{
	return m_Tree.GetCurrentTreePath ();
}

/*
HTREEITEM CTreeCombo::AddString ( LPCTSTR lpszString)
{
	return m_Tree.AddString (lpszString);
}
*/

HTREEITEM CTreeCombo::FindString ( LPCTSTR lpszString, HTREEITEM hParent /*=NULL*/)
{
	return m_Tree.FindString (lpszString, hParent);
}


int CTreeCombo::DeleteString( HTREEITEM hItem )
{
	HTREEITEM hSelected = m_Tree.GetSelectedItem ();
	if (m_Tree.DeleteItem (hItem))
	{
		if (hItem == hSelected)
		{
			//get the new item
			hItem = m_Tree.GetSelectedItem ();
			if (hItem)
			{
				CString NodeText = m_Tree.GetItemText (hItem);
				SetWindowText (NodeText);
				SendParentComboMessage (CBN_SELCHANGE);
			}
			else
			{
				SetWindowText (_T(""));
			}
		}

		return 0;
	}
	else
	{
		return CB_ERR;
	}
}


HTREEITEM CTreeCombo::SelectString( LPCTSTR lpszString, HTREEITEM hParent /*=NULL*/)
{
	HTREEITEM hMatch = m_Tree.SelectString (lpszString, hParent);
	CString NodeText = m_Tree.GetItemText (hMatch);
	SetWindowText (NodeText);

	SendParentComboMessage (CBN_SELCHANGE);

	return hMatch;
}

HTREEITEM CTreeCombo::GetCurSel ()
{
	return m_Tree.GetSelectedItem ();
}

int CTreeCombo::SetItemData (HTREEITEM hItem, DWORD dwItemData)
{
	if (!m_Tree.SetItemData (hItem, dwItemData))
	{
		return CB_ERR;
	}
	else 
	{
		return 0;
	}
}

DWORD CTreeCombo::GetItemData (HTREEITEM hItem)
{
	return m_Tree.GetItemData (hItem);
}

void CTreeCombo::ShowDropDown( BOOL bShowIt)
{
	if (bShowIt)
	{
		if (!GetDroppedState ())
		{
			DisplayTree ();
		}
	}
	else if (!GetDroppedState ())
	{
		m_Tree.ShowWindow (SW_HIDE);
		SetDroppedState (FALSE);
	}
}

void CTreeCombo::ResetContent ()
{
	m_Tree.DeleteAllItems( );
}


int CTreeCombo::SetDroppedWidth( UINT nWidth )
{
	int retval = 0;
	if (nWidth >= 0)
	{
		m_ComboWidth = nWidth;
		if (GetDroppedState ())
		{
			DisplayTree ();
		}
	}
	else
	{
		retval = CB_ERR;
	}
	return retval;
}

int CTreeCombo::GetDroppedWidth( )
{
	return m_ComboWidth;
}

int CTreeCombo::SetDroppedHeight (UINT nHeight)
{
	int retval = 0;
	if (nHeight >= 0)
	{
		m_ComboHeight = nHeight;
		if (GetDroppedState ())
		{
			DisplayTree ();
		}
	}
	else
	{
		retval = CB_ERR;
	}
	return retval;
}

int CTreeCombo::GetDroppedHeight ()
{
	return m_ComboHeight;
}

BOOL CTreeCombo::GetDroppedState( )
{
	return m_bDroppedState;
}

void CTreeCombo::GetDroppedControlRect (LPRECT pRect)
{
	_ASSERTE (pRect);
	if (pRect)
	{
		CalculateDroppedRect (pRect);
	}
}

void CTreeCombo::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	if (::GetFocus() != m_DropDownButton.GetSafeHwnd())
	{
		m_Edit.SetFocus ();
	}
}

BOOL CTreeCombo::IsMouseOnButton() 
{
	BOOL bMouseOn = FALSE;
	CPoint point;
	if (GetCursorPos (&point))
	{
		CRect rect;
		m_DropDownButton.GetWindowRect (&rect);
		if (rect.PtInRect (point))
		{
			bMouseOn = TRUE;
		}
	}
	return bMouseOn;
}

BOOL CTreeCombo::IsMouseOnEdit() 
{
	BOOL bMouseOn = FALSE;
	CPoint point;
	if (GetCursorPos (&point))
	{
		CRect rect;
		m_Edit.GetWindowRect (&rect);
		if (rect.PtInRect (point))
		{
			bMouseOn = TRUE;
		}
	}
	return bMouseOn;
}

BOOL CTreeCombo::IsEditHighlightOn () 
{
	BOOL bHighlightOn = FALSE;
	HWND hFocus = ::GetFocus ();

	if (hFocus == GetSafeHwnd () || 
		hFocus == m_Edit.GetSafeHwnd  () ||
		hFocus == m_DropDownButton.GetSafeHwnd  ())
	{
		bHighlightOn = TRUE;
	}

	return bHighlightOn;
}


void CTreeCombo::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	
	m_Edit.Invalidate ();	
}


BOOL CTreeCombo::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (HIWORD(wParam) == BN_KILLFOCUS	)
	{
		m_Edit.Invalidate ();
	}

	return CWnd::OnCommand(wParam, lParam);
}

HTREEITEM CTreeCombo::AddData(HTREEITEM hParent, LPSTR Descption, BOOL IsKey)
{
	return m_Tree.AddData(hParent, Descption,IsKey);
}

BOOL CTreeCombo::LoadTableInfo(CString szConfigFile)
{
	return m_Tree.LoadTableInfo(szConfigFile);
}
