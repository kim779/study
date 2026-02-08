#if !defined(AFX_PHONEPAD_H__C8EBCC32_9E0F_4608_8AF9_ADEDEEA2D814__INCLUDED_)
#define AFX_PHONEPAD_H__C8EBCC32_9E0F_4608_8AF9_ADEDEEA2D814__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PhonePad.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPhonePad dialog

class CPhonePad : public CDialog
{
// Construction
public:
	HWND main_hwnd;
	CPhonePad(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPhonePad)
	enum { IDD = IDD_PHONEPAD_DIALOG };
	CString	m_msg;
	CString	m_pwd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhonePad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPhonePad)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHONEPAD_H__C8EBCC32_9E0F_4608_8AF9_ADEDEEA2D814__INCLUDED_)
