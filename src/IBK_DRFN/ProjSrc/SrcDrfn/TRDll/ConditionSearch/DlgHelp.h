#if !defined(AFX_DLGHELP_H__C168893C_6D6C_44F4_928A_E96D606CF7BF__INCLUDED_)
#define AFX_DLGHELP_H__C168893C_6D6C_44F4_928A_E96D606CF7BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHelp.h : header file
//
#include "ConditionMakeWnd.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// DlgHelp dialog

class DlgHelp : public CDialog
{
// Construction
public:
	DlgHelp(CWnd* pParent = NULL, CString strHelpText = "");   // standard constructor
// Dialog Data
	//{{AFX_DATA(DlgHelp)
	enum { IDD = IDD_HELP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgHelp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	COLORREF		m_clrMainSkin;

	CfxImgButton	m_btnOk;

	CWnd*			m_pWndMainFrm;
	CEdit			m_ctrlHelpText;
	CString			m_strHelpText;

	void OnSize();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgHelp)
	virtual BOOL OnInitDialog();
	void OnBtnOk();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHELP_H__C168893C_6D6C_44F4_928A_E96D606CF7BF__INCLUDED_)
