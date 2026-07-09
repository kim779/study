#if !defined(AFX_PUBLICSETDLG_H__B40AF88A_715A_4A04_B342_BB48395A819F__INCLUDED_)
#define AFX_PUBLICSETDLG_H__B40AF88A_715A_4A04_B342_BB48395A819F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PublicSetDlg.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CPublicSetDlg dialog

class CPublicSetDlg : public CDialog
{
// Construction
public:
	CString		m_publicR;
	CStringArray	m_HelpArr;
	CStringArray	m_proArr;
protected:
private:


public:
	BOOL addItem(CListCtrl *LC, int item, int subItem, CString &str);
	void OnPublicInfo(CString data);
	CString Parser(CString &srcstr, CString substr);
	BOOL addColumn(CListCtrl *LC, LPCTSTR str, int item, int width);
	void LoadFile();
	CPublicSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPublicSetDlg)
	enum { IDD = IDD_PUBLIC };
	CListCtrl	m_List_pro;
	CListBox	m_newList;
	CListBox	m_oldList;
	CString	m_sHelpStatic;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPublicSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPublicSetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnInbtn();
	afx_msg void OnOutbtn();
	afx_msg void OnSelchangeOldlist();
	afx_msg void OnSelchangeNewlist();
	virtual void OnOK();
	afx_msg void OnDblclkOldlist();
	afx_msg void OnDblclkNewlist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PUBLICSETDLG_H__B40AF88A_715A_4A04_B342_BB48395A819F__INCLUDED_)
