#if !defined(AFX_MINISETUNITDLG_H__4453F0D3_1658_492D_8667_F16E6F9C62E6__INCLUDED_)
#define AFX_MINISETUNITDLG_H__4453F0D3_1658_492D_8667_F16E6F9C62E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MiniSetUnitDlg.h : header file
//
#include "RscDlg.h"
#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CMiniSetUnitDlg dialog
class CInputBarDlg;
class CMiniSetUnitDlg : public CRscDlg
{
// Construction
public:
	CMiniSetUnitDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMiniSetUnitDlg)
	enum { IDD = IDD_DLGMINISETUNIT };
	CEdit	m_editCbTwelve;
	CEdit	m_editCbNinth;
	CEdit	m_editCbTenth;
	CEdit	m_editCbEleven;
	CEdit	m_editCbEighth;
	CEdit	m_editCbThird;
	CEdit	m_editCbSixth;
	CEdit	m_editCbSeventh;
	CEdit	m_editCbSecond;
	CEdit	m_editCbFourth;
	CEdit	m_editCbFifth;
	CEdit	m_editCbFirst;
	CButton	m_btApply;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

public:
	CInputBarDlg* m_pInputBarDlg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMiniSetUnitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMiniSetUnitDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEdit();
	afx_msg void OnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MINISETUNITDLG_H__4453F0D3_1658_492D_8667_F16E6F9C62E6__INCLUDED_)
