#if !defined(AFX_TITLERECEIVERDLG_H__DE5286E0_F2CB_4A18_9672_570D2E942BD1__INCLUDED_)
#define AFX_TITLERECEIVERDLG_H__DE5286E0_F2CB_4A18_9672_570D2E942BD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TitleReceiverDlg.h : header file
//
#include "RscDlg.h"
#include "Resource.h"
/////////////////////////////////////////////////////////////////////////////
// CTitleReceiverDlg dialog

class CTitleReceiverDlg : public CRscDlg
{
// Construction
public:
	CTitleReceiverDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTitleReceiverDlg)
	enum { IDD = IDD_DIGTITLERECEIVER };
	CString	m_strNewTitle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTitleReceiverDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTitleReceiverDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TITLERECEIVERDLG_H__DE5286E0_F2CB_4A18_9672_570D2E942BD1__INCLUDED_)
