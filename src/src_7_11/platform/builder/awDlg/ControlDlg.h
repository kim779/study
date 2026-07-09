#if !defined(AFX_CONTROLDLG_H__744888D9_A0EC_45D2_8509_8135ED962076__INCLUDED_)
#define AFX_CONTROLDLG_H__744888D9_A0EC_45D2_8509_8135ED962076__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ControlDlg.h : header file
//

#include "resource.h"
#include "SingleProperty.h"

/////////////////////////////////////////////////////////////////////////////
// CControlDlg dialog

class AFX_EXT_CLASS CControlDlg : public CDialog
{
// Construction
public:
	BOOL	addItem(CListCtrl *LC, int item, int subItem, CString &str);
	BOOL	addColumn(CListCtrl *LC, LPCTSTR str, int item, int width);
	void	LoadFile();
	CString	Parser(CString &srcstr, CString substr);
	void DebugData(char* buf, long bufsize);
	CControlDlg(CWnd* pParent = NULL);   // standard constructor

public:
	CStringArray	m_ctrlArr, m_dataArr;
	CString		m_ctrlParam, m_ctrlName;
protected:
	CSingleProperty	m_singleProperty;
	int		m_beforeIdx;
private:

// Dialog Data
	//{{AFX_DATA(CControlDlg)
	enum { IDD = IDD_CONTROL };
	CListCtrl	m_list;
	CString		m_desc;
	CString		m_input;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CControlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickNamelist(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnItemchangedNamelist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTROLDLG_H__744888D9_A0EC_45D2_8509_8135ED962076__INCLUDED_)
