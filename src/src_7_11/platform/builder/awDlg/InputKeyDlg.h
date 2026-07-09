#if !defined(AFX_INPUTKEYDLG_H__9348106A_4CE3_477A_977D_5371646A3783__INCLUDED_)
#define AFX_INPUTKEYDLG_H__9348106A_4CE3_477A_977D_5371646A3783__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputKeyDlg.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CInputKeyDlg dialog

class AFX_EXT_CLASS CInputKeyDlg : public CDialog
{
// Construction
public:
	CInputKeyDlg(CWnd* pParent = NULL, CRect voidRect = CRect(0));   // standard constructor

public:
	DWORD		m_keyVal;
	CRect		m_voidRect;

// Dialog Data
	//{{AFX_DATA(CInputKeyDlg)
	enum { IDD = IDD_INPUTKEYDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputKeyDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputKeyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnNonebtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTKEYDLG_H__9348106A_4CE3_477A_977D_5371646A3783__INCLUDED_)
