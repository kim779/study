#pragma once
// DirectInputDlg.h : header file
//

#include "MapForm.h"
#include "AccEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDirectInputDlg dialog

class CDirectInputDlg : public CDialog
{
// Construction
public:
	LRESULT OnAccMaxMessage(WPARAM,LPARAM);
	void InputAccName(CString sAccName);
	CMapForm* m_pParentWnd;
	CDirectInputDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDirectInputDlg)
	enum { IDD = IDD_DIRECTINPUT_DLG };
	CAccEdit	m_accEdit;
	CString	m_sAccName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirectInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDirectInputDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

