#if !defined(AFX_CHOICEDLG_H__C75A4F34_D8A5_417D_AE01_9A15C675EEAD__INCLUDED_)
#define AFX_CHOICEDLG_H__C75A4F34_D8A5_417D_AE01_9A15C675EEAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// choiceDlg.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CChoiceDlg dialog

class AFX_EXT_CLASS CChoiceDlg : public CDialog
{
// Construction
public:
	CChoiceDlg(CWnd* pParent = NULL, bool userScreen = false);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChoiceDlg)
	enum { IDD = IDD_CHOOSE };
	CListCtrl	m_list;
	//}}AFX_DATA

// Attributes
public:
	enum	NTYPE { N_NORM=0,  N_SPLIT01, N_SPLIT02, N_SPLIT03, N_SPLIT04,
			N_SPLIT05, N_SPLIT06, N_SPLIT07, N_SPLIT08, N_SPLIT09,
			N_SPLIT10, N_SPLIT11, N_SPLIT12, N_ENUM,    N_TAB } m_type;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChoiceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool	m_userScreen;

protected:
	void	addLVItem(int nItem, CString& strItem);

	// Generated message map functions
	//{{AFX_MSG(CChoiceDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnListDblClk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListReturn(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHOICEDLG_H__C75A4F34_D8A5_417D_AE01_9A15C675EEAD__INCLUDED_)
