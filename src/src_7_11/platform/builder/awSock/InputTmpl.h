#if !defined(AFX_INPUTTMPL_H__CF20A5B2_2196_43D1_9746_BFEC4EBBFDC6__INCLUDED_)
#define AFX_INPUTTMPL_H__CF20A5B2_2196_43D1_9746_BFEC4EBBFDC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputTmpl.h : header file
//

#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CInputTmpl dialog

class CInputTmpl : public CDialog
{
// Construction
public:
	void loadFile();
	CInputTmpl(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputTmpl)
	enum { IDD = IDD_INPUTTMPL };
	CListCtrl	m_listctrl;
	CComboBox	m_combo;
	CString	m_groupName;
	CString	m_tmplName;
	BOOL	m_bExport;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputTmpl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL addItem(CListCtrl *LC, int item, int subItem, CString &str);
	void fillList(CListCtrl *LC, int idx);
	void fillCombo(CComboBox *CBO);
	CString Parser(CString &srcstr, CString substr);
	CStringArray	m_groupArr, m_nameArr;

	// Generated message map functions
	//{{AFX_MSG(CInputTmpl)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelendokGroupcbo();
	afx_msg void OnClickTmpllist(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTTMPL_H__CF20A5B2_2196_43D1_9746_BFEC4EBBFDC6__INCLUDED_)
