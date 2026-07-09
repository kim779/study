#if !defined(AFX_ATTRDLG_H__5CAD0A00_7CC6_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_ATTRDLG_H__5CAD0A00_7CC6_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// attrDlg.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CAttrDlg dialog

class AFX_EXT_CLASS CAttrDlg : public CDialog
{
// Construction
public:
	CAttrDlg(CWnd* pParent, BYTE kind, BYTE type, BYTE iok, DWORD attr, DWORD attr2);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAttrDlg)
	enum { IDD = IDD_ATTR };
	BOOL	m_at01;
	BOOL	m_at02;
	BOOL	m_at03;
	BOOL	m_at04;
	BOOL	m_at05;
	BOOL	m_at06;
	BOOL	m_at07;
	BOOL	m_at08;
	BOOL	m_at09;
	BOOL	m_at10;
	BOOL	m_at11;
	BOOL	m_at12;
	BOOL	m_at13;
	//BOOL	m_at14;
	BOOL	m_at15;
	BOOL	m_at16;
	BOOL	m_at17;
	BOOL	m_at18;
	BOOL	m_at19;
	BOOL	m_at20;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAttrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes
public:
	void OnContrast(UINT cmdID);
	void OnCheckRight();
	void OnCheckComma();
	DWORD	m_attr;
	DWORD	m_attr2;

protected:
	BYTE	m_kind;
	BYTE	m_type;
	BYTE	m_iok;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAttrDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckZerosup();
	afx_msg void OnCheckCoRgb();
	afx_msg void OnCheckSend();
	virtual void OnOK();
	afx_msg void OnCheckContrast();
	//}}AFX_MSG
	afx_msg	void	OnZeroSup(UINT cmdID);
	afx_msg	void	OnCoRgb(UINT cmdID);
	afx_msg	void	OnSend(UINT cmdID);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATTRDLG_H__5CAD0A00_7CC6_11D4_A024_00001CD7F9BE__INCLUDED_)
