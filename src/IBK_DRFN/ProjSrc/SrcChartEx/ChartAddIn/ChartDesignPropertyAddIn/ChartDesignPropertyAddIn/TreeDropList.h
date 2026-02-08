/* Standard Disclaimer: 
Copyright (C) 2000  Dennis Howard
This file is free software; you can redistribute it and/or
modify it without any conditions. There is no warranty,
implied or expressed, as to validity or fitness for a particular purpose.
*/


#if !defined(AFX_TREEDROPLIST_H__0F2E2150_2342_4F38_AC2F_92A6C718C28F__INCLUDED_)
#define AFX_TREEDROPLIST_H__0F2E2150_2342_4F38_AC2F_92A6C718C28F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// TreeDropList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTreeDropList window
class CTreeCombo;

class CTreeDropList : public CTreeCtrl
{

friend class CTreeCombo;

enum CloseReason { e_Cancel, e_Select, e_KillFocus };

// Construction
public:
	CTreeDropList();

// Attributes
protected:
	void SetCombo (CTreeCombo* pCombo)
	{
		m_pCombo = pCombo;
	}

	CTreeCombo* GetCombo ()
	{
		return m_pCombo;
	}

	void SplitPath (const CString& strTreeBranch, CString& strRoot, CString& strRemainder);
	HTREEITEM MatchSibling (HTREEITEM hItem, CString& strMatch);
	HTREEITEM FindString (HTREEITEM hParent, CString strTreeBranch,
			 		     HTREEITEM& hPreviousMatch, CString& strRoot, CString& strRemainder );

	HTREEITEM DropListAddItem (HTREEITEM hParent, CString strTreeBranch);
	BOOL GetDroppedState () { return m_bDropped; }
	void InitDisplay () { m_CloseReason = e_KillFocus; }

//	HTREEITEM AddString ( LPCTSTR lpszString);

	HTREEITEM FindString (CString strTreeBranch, HTREEITEM hParent = NULL );

	HTREEITEM SelectString( LPCTSTR lpszString, HTREEITEM hParent = NULL);

	CString GetTreePath (HTREEITEM hItem);

	CString GetCurrentTreePath ();

	TCHAR GetPathDelimiter () { return m_Delimiter; }

	void SetPathDelimiter (TCHAR Delimiter) { m_Delimiter = Delimiter; }


	//Added by kay Seo..	
	LPSTR CutString(LPSTR str, LPSTR buff);
	BOOL Parsing();
	int DataRegular(unsigned char* tmp, unsigned char* buff);
	BOOL LoadTableInfo(CString szConfigFile);
	CStringList m_stringList;



protected:
	CTreeCombo* m_pCombo;
	TCHAR       m_Delimiter;
	CloseReason m_CloseReason;
	BOOL        m_bDropped;



protected:
	void OnSelection ();
	void OnCancel ();


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeDropList)
	//}}AFX_VIRTUAL

// Implementation
public:
	HTREEITEM AddData(HTREEITEM hParent, LPSTR Descption, BOOL IsKey);
	HTREEITEM AddPacket(LPSTR szPacketName, LPSTR szDescrip);
	HTREEITEM AddItem(HTREEITEM hParent, LPSTR Description, LPSTR Name,LPSTR type, LPSTR szLength);
	virtual ~CTreeDropList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeDropList)
	afx_msg void OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSetfocus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd)		{	m_hOcxWnd = p_hOcxWnd;	}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREEDROPLIST_H__0F2E2150_2342_4F38_AC2F_92A6C718C28F__INCLUDED_)
