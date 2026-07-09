#if !defined(AFX_BASEINFOPAGE_H__80ADC881_783A_4D4E_993F_3CD78F679888__INCLUDED_)
#define AFX_BASEINFOPAGE_H__80ADC881_783A_4D4E_993F_3CD78F679888__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseInfoPage.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CBaseInfoPage dialog

class CBaseInfoPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CBaseInfoPage)

// Construction
public:
	CBaseInfoPage();
	~CBaseInfoPage();
protected:
private:
	void getAddress(BYTE &field0, BYTE &field1, BYTE &field2, BYTE &field3);
public:
	CString	m_ips;
protected:
private:
	CString	m_currentPath;

public:

// Dialog Data
	//{{AFX_DATA(CBaseInfoPage)
	enum { IDD = IDD_PROPPAGE_BASEINFO };
	CIPAddressCtrl	m_addr;
	int		m_nGap;
	CString	m_mapPath;
	CString	m_path;
	int		m_nDisplayMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CBaseInfoPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CBaseInfoPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDisplayMode();
	afx_msg void OnBrowser();
	afx_msg void OnBrowser2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASEINFOPAGE_H__80ADC881_783A_4D4E_993F_3CD78F679888__INCLUDED_)
