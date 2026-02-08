#pragma once
// JisuConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJisuConfigDlg dialog
#include "controls.h"
#include "colorbutton.h"

class CJisuConfigDlg : public CDialog
{
// Construction
public:
	CJisuConfigDlg(struct _minijisu *mj, CRect rcBtn);
	CColorButton	m_pColorBtn[MAX_JISU];

// Dialog Data
	//{{AFX_DATA(CJisuConfigDlg)
	enum { IDD = IDD_CONFIG_DLG };
	
	CWidthCtrl	m_jwidth6;
	CWidthCtrl	m_jwidth5;
	CWidthCtrl	m_jwidth4;
	CWidthCtrl	m_jwidth3;
	CWidthCtrl	m_jwidth2;
	CWidthCtrl	m_jwidth1;
	CKindCtrl	m_jstyle6;
	CKindCtrl	m_jstyle5;
	CKindCtrl	m_jstyle4;
	CKindCtrl	m_jstyle3;
	CKindCtrl	m_jstyle2;
	CKindCtrl	m_jstyle1;
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJisuConfigDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJisuConfigDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnJcolor1();
	afx_msg void OnJcolor2();
	afx_msg void OnJcolor3();
	afx_msg void OnJcolor4();
	afx_msg void OnJcolor5();
	afx_msg void OnJcode1();
	afx_msg void OnJcode2();
	afx_msg void OnJcode3();
	afx_msg void OnJcode4();
	afx_msg void OnJcode5();
	afx_msg void OnBasic();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void	InitColorBtn();
	void	GetCode(CString &code);
private:
	struct _minijisu	*m_pMJ, m_mj;
	CRect	m_rcBtn;
	HINSTANCE	m_hReg;
};