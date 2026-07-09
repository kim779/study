#if !defined(AFX_SETTINGDLG_H__BE5613BE_86AE_4D5B_802F_A865B72489B9__INCLUDED_)
#define AFX_SETTINGDLG_H__BE5613BE_86AE_4D5B_802F_A865B72489B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingDlg.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CSettingDlg dialog

class AFX_EXT_CLASS CSettingDlg : public CDialog
{
// Construction
public:
	CString	m_ips, m_currentPath;
protected:
	CBrush		m_ctlBrushUp, m_ctlBrushDn;
	COLORREF	m_ctlClrUp, m_ctlClrDn;
public:
	void getAddress(BYTE& field0, BYTE& field1, BYTE& field2, BYTE& field3);
	CSettingDlg(CWnd* pParent = NULL, CString path = "");   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSettingDlg)
	enum { IDD = IDD_SETTING };
	CSpinButtonCtrl	m_upRGBSpin;
	CSpinButtonCtrl	m_dnRGBSpin;
	CString	m_path;
	int		m_nGap;
	CIPAddressCtrl	m_addr;
	CString	m_fontname;
	CString	m_fontpnt;
	CString	m_mapPath;
	int		m_dnRGB;
	int		m_upRGB;
	int		m_clrTabPos;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSettingDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowser();
	afx_msg void OnSetfontbtn();
	afx_msg void OnBrowser2();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGDLG_H__BE5613BE_86AE_4D5B_802F_A865B72489B9__INCLUDED_)
