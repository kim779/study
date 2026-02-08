#if !defined(AFX_DLGGLOBALLANGUAGETABLE_H__18C3CFF5_E0B0_407A_9223_F9BFD40F7C1A__INCLUDED_)
#define AFX_DLGGLOBALLANGUAGETABLE_H__18C3CFF5_E0B0_407A_9223_F9BFD40F7C1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGlobalLanguageTable.h : header file
//

#include "EditableListCtrl.h"		// for CEditableListCtrl

/////////////////////////////////////////////////////////////////////////////
// CDlgGlobalLanguageTable dialog

class CDlgGlobalLanguageTable : public CDialog
{
// Construction
public:
	CDlgGlobalLanguageTable(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGlobalLanguageTable)
	enum { IDD = IDD_GLOBAL_LANGUAGE_TABLE };
	CComboBox	m_cmbTableType;
	CListCtrl	m_lstMetaTable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGlobalLanguageTable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGlobalLanguageTable)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnUpdateEdtSearchText();
	afx_msg void OnSelchangeCmbTableType();
	afx_msg void OnItemchangedLstMetaTable(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnItemclickLstMetaTable(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

// (2008/2/24 - Seung-Won, Bae) Limit Size
protected:
	CPoint			m_ptMinSize;

// (2008/2/25 - Seung-Won, Bae) for Column Reverse Sort
protected:
	int				m_nSortColumn;
	BOOL			m_bSortASC;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGLOBALLANGUAGETABLE_H__18C3CFF5_E0B0_407A_9223_F9BFD40F7C1A__INCLUDED_)
