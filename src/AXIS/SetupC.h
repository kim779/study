#if !defined(AFX_SETUPC_H__033987E8_1D8C_44A7_BF04_FD2158F0F71C__INCLUDED_)
#define AFX_SETUPC_H__033987E8_1D8C_44A7_BF04_FD2158F0F71C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetupC.h : header file
//
#include "BtnST.h"
#include "../dll/sm/NButton.h"
/////////////////////////////////////////////////////////////////////////////
// CSetupC dialog

class CSetupC : public CDialog
{
// Construction
public:
	LONG OnXMSG(WPARAM wParam, LPARAM lParam);
	CSetupC(CWnd* pParent = NULL);   // standard constructor
	UINT	GetEmergency() { return m_nEmerge; }
// Dialog Data
	//{{AFX_DATA(CSetupC)
	enum { IDD = IDD_SETUP_CUST };
	CButtonST	m_ckProxy;
	CButtonST	m_ckFirewall;
	CButtonST	m_ckServerTime;
	CButtonST	m_ckResource;
	CStatic	m_static;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetupC)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetupC)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnEmergency();
	DECLARE_MESSAGE_MAP()

	BOOL	m_nEmerge;
	HBITMAP m_bitmap;
#ifdef DF_USE_CPLUS17
	std::unique_ptr<CNButton> m_okBtn;
#else
	CNButton	*m_okBtn;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUPC_H__033987E8_1D8C_44A7_BF04_FD2158F0F71C__INCLUDED_)
