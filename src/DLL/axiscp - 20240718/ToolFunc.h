#if !defined(AFX_TOOLFUNC_H__C0E75974_BC35_4187_AA7A_56A370BD6E84__INCLUDED_)
#define AFX_TOOLFUNC_H__C0E75974_BC35_4187_AA7A_56A370BD6E84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolFunc.h : header file
//
#include "SetupDialog.h"
#include "reportctrl.h"

#define MAX_FUNC	9
/////////////////////////////////////////////////////////////////////////////
// CToolFunc dialog

class CToolFunc : public CSetupDialog
{
// Construction
public:
	CToolFunc(CWnd* pParent = NULL);   // standard constructor

	virtual void		ApplySetup();
	virtual void		DefaultSetup();

protected:
	CImageList	m_imageList;

	void		loadSetup();
	void		init();
	CString		Parser(CString &srcstr, CString substr);

public:
// Dialog Data
	//{{AFX_DATA(CToolFunc)
	enum { IDD = IDD_FUNCTIONS };
	CReportCtrl	m_toollist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolFunc)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolFunc)
	virtual BOOL OnInitDialog();
	afx_msg void OnMoveup();
	afx_msg void OnMovedown();
	afx_msg void OnMovetop();
	afx_msg void OnMovebottom();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLFUNC_H__C0E75974_BC35_4187_AA7A_56A370BD6E84__INCLUDED_)
