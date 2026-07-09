#if !defined(AFX_MGCOLUMNDLG_H__40AF61B8_6424_4C08_9AE1_A68BEA370D21__INCLUDED_)
#define AFX_MGCOLUMNDLG_H__40AF61B8_6424_4C08_9AE1_A68BEA370D21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MGColumnDlg.h : header file
//

#include "resource.h"
#include "SingleProperty.h"
/////////////////////////////////////////////////////////////////////////////
// CMGColumnDlg dialog

class AFX_EXT_CLASS CMGColumnDlg : public CDialog
{
// Construction
public:
	CMGColumnDlg(CWnd* pParent = NULL);   // standard constructor
public:
	CString Parser(CString &srcstr, CString substr);
	void ParseData();
	CString		m_sData;
protected:
	CString		m_sCellInfo;
	CSingleProperty	m_property;
// Dialog Data
	//{{AFX_DATA(CMGColumnDlg)
	enum { IDD = IDD_MGRIDCOLUMN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMGColumnDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMGColumnDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddbtn();
	afx_msg void OnDelbtn();
	afx_msg void OnInsertbtn();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MGCOLUMNDLG_H__40AF61B8_6424_4C08_9AE1_A68BEA370D21__INCLUDED_)
