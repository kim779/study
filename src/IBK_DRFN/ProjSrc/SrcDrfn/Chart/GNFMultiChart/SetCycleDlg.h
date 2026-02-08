#if !defined(AFX_SETCYCLEDLG_H__0E20B6B8_B8CF_4129_BD60_E60182478717__INCLUDED_)
#define AFX_SETCYCLEDLG_H__0E20B6B8_B8CF_4129_BD60_E60182478717__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetCycleDlg.h : header file
//
#include "RscDlg.h"
#include "Resource.h"
/////////////////////////////////////////////////////////////////////////////
// CSetCycleDlg dialog
class CInputBarDlg;
class CSetCycleDlg : public CRscDlg
{
// Construction
public:
	CSetCycleDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL VerifySettingData(CString& strData);
	void VerifySettingData(CEdit& ctrlEdit, LONG& lData);

// Dialog Data
	//{{AFX_DATA(CSetCycleDlg)
	enum { IDD = IDD_DLGSETCYCLE };
	CButton	m_btApply;
	CEdit	m_editDayFirst;
	CEdit	m_editDaySecond;
	CEdit	m_editDayThird;
	CEdit	m_editDayFourth;
	CEdit	m_editMinFirst;
	CEdit	m_editMinSecond;
	CEdit	m_editMinThird;
	CEdit	m_editMinFourth;	
	CEdit	m_editSecFirst;
	CEdit	m_editSecSecond;
	CEdit	m_editSecThird;
	CEdit	m_editSecFourth;
	CEdit	m_editTickFirst;
	CEdit	m_editTickSecond;
	CEdit	m_editTickThird;
	CEdit	m_editTickFourth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetCycleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CInputBarDlg* m_pInputBarDlg;
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetCycleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	virtual void OnOK();
	afx_msg void OnChangeEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETCYCLEDLG_H__0E20B6B8_B8CF_4129_BD60_E60182478717__INCLUDED_)
