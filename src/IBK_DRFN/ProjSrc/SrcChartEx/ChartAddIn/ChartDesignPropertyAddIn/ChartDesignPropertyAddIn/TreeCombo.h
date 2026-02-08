/* Standard Disclaimer: 
Copyright (C) 2000  Dennis Howard
This file is free software; you can redistribute it and/or
modify it without any conditions. There is no warranty,
implied or expressed, as to validity or fitness for a particular purpose.
*/

#if !defined(AFX_TREECOMBO_H__7A73B177_AC87_41F0_BAE5_734A041659EE__INCLUDED_)
#define AFX_TREECOMBO_H__7A73B177_AC87_41F0_BAE5_734A041659EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTreeCombo window
#include "TreeDropList.h"
#include "ComboEdit.h"

class CTreeCombo : public CWnd
{
  friend class CTreeDropList;
  friend class CComboEdit;

// Construction
public:
	CTreeCombo();

// Attributes
public:
//Child control access functions
	CTreeDropList& GetTree ()           { return m_Tree; }
	CComboEdit& GetEdit ()              { return m_Edit; }
	CButton& GetDropDownButton () { return m_DropDownButton; }

//Tree specific extensions 
	CString GetTreePath (HTREEITEM hItem);
	CString GetCurrentTreePath ();
	int  SetDroppedHeight (UINT nHeight);
	int  GetDroppedHeight ();
	TCHAR GetPathDelimiter ();
	void SetPathDelimiter (TCHAR Delimiter);

//CComboBox emulation functions
	void SetWindowText (LPCTSTR Text);
	CString GetWindowText ();
    int GetLBText (HTREEITEM hItem, CString& rText);
//	HTREEITEM AddString ( LPCTSTR lpszString);
	HTREEITEM FindString ( LPCTSTR lpszString, HTREEITEM hParent = NULL);
	HTREEITEM SelectString( LPCTSTR lpszString, HTREEITEM hParent = NULL);
	HTREEITEM GetCurSel ();
	int   SetItemData (HTREEITEM hItem, DWORD dwItemData);
	DWORD GetItemData (HTREEITEM hItem);
	void ShowDropDown( BOOL bShowIt = TRUE );
    void ResetContent ();
	int  SetDroppedWidth( UINT nWidth );
	int  GetDroppedWidth( );
	BOOL GetDroppedState( );
	int  DeleteString( HTREEITEM hItem );
	void GetDroppedControlRect (LPRECT pRect);
  
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeCombo)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	public:
	virtual BOOL SubclassDlgItem (UINT nID, CWnd* pParent);

// Implementation
public:
	BOOL LoadTableInfo(CString szConfigFile);
	HTREEITEM AddData(HTREEITEM hParent, LPSTR Descption, BOOL IsKey);
	virtual ~CTreeCombo();

protected:
	void DisplayTree();
	BOOL CreateTree ();
	BOOL CreateButton ();
	BOOL CreateEdit ();
	void SetDroppedState (BOOL bState)
	{
		m_bDroppedState = bState;
	}

	LRESULT SendParentComboMessage ( UINT Action);
	void CalculateDroppedRect(LPRECT lpDroppedRect);
    void OnCancel ();

	BOOL IsMouseOnEdit(); 
	BOOL IsMouseOnButton(); 

	BOOL IsEditHighlightOn ();


	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeCombo)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	void OnDropdownButton ();

	DECLARE_MESSAGE_MAP()

protected:
	CTreeDropList m_Tree;
	CComboEdit    m_Edit;
	CButton m_DropDownButton;
	CString       m_BeginPath;
	int           m_ComboWidth;
	int           m_ComboHeight;
	BOOL          m_bDroppedState;

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
public:
	void	SetOcxHwnd( HWND p_hOcxWnd)		{	m_Tree.SetOcxHwnd( p_hOcxWnd);	}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREECOMBO_H__7A73B177_AC87_41F0_BAE5_734A041659EE__INCLUDED_)
