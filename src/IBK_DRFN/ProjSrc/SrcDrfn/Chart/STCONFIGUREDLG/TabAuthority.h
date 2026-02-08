#if !defined(AFX_TABAUTHORITY_H__40F8C1EB_2188_4EBC_86AD_E1BD18EAD89B__INCLUDED_)
#define AFX_TABAUTHORITY_H__40F8C1EB_2188_4EBC_86AD_E1BD18EAD89B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabAuthority.h : header file
//
#include "Resource.h"
#include "RscDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CTabAuthority dialog

class CTabAuthority : public CRscDlg
{
// Construction
public:
	CTabAuthority(CWnd* pParent = NULL);   // standard constructor
	void SetColor(COLORREF& rCrBk) { m_crBk = rCrBk;}
	void SetBrush(CBrush*	pBrBkColor) { m_pBrBkColor = pBrBkColor;}

// Dialog Data
	//{{AFX_DATA(CTabAuthority)
	enum { IDD = IDD_DLG_TAB_AUTHORITY };
	CStatic	m_stPassword2;
	CDateTimeCtrl	m_datepickerExpiredDate;
	CEdit	m_editUserID;
	CEdit	m_editPassword2;
	CEdit	m_editPassword;
	CButton	m_chkUserID;
	CButton	m_chkPassword;
	CButton	m_chkExpiredDate;
	BOOL	m_bChkPassword;
	CString	m_strPassword;
	CString	m_strPassword2;
	CString	m_strUserID;
	CTime	m_timeExpiredDate;
	BOOL	m_bChkExpiredDate;
	BOOL	m_bChkUserID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabAuthority)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
private:
	void LoadInfo();
	BOOL SaveInfo();
	COLORREF m_crBk;
	CBrush*	m_pBrBkColor;
	CWnd*	m_pDlgSTOption;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabAuthority)
	afx_msg void OnChkexpireddate();
	afx_msg void OnChkpasswd();
	afx_msg void OnChkuserid();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABAUTHORITY_H__40F8C1EB_2188_4EBC_86AD_E1BD18EAD89B__INCLUDED_)
