#if !defined(AFX_INDEXSEARCHDLG_H__1400E2CD_F6C6_47BE_BBE0_2E5CA3275257__INCLUDED_)
#define AFX_INDEXSEARCHDLG_H__1400E2CD_F6C6_47BE_BBE0_2E5CA3275257__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IndexSearchDlg.h : header file
//

#include "resource.h"
#include "AxMTreeCtrl.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CIndexSearchDlg dialog

class CIndexSearchDlg : public CDialog
{
// Construction
public:
	CIndexSearchDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIndexSearchDlg)
	enum { IDD = IDD_INDEXSEARCH };
	CListCtrl	m_ResultList;
	CEdit	m_IndexEdit;
	CButton	m_SearchButton;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIndexSearchDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIndexSearchDlg)
	afx_msg void OnButtonSearch();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListResult(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CAxMTreeCtrl	 *m_pParentTree;
	CMapStringToPtr  m_TextMapItemList;

private:
	void SearchIndexName();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDEXSEARCHDLG_H__1400E2CD_F6C6_47BE_BBE0_2E5CA3275257__INCLUDED_)
