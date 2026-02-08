
#include "ColorBtn.h"
#pragma once
// UsefulSetupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUsefulSetupDlg dialog

class CUsefulSetupDlg : public CDialog
{
// Construction
public:
	CUsefulSetupDlg(CWnd* pParent = nullptr);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUsefulSetupDlg)
	enum { IDD = IDD_SETUP };
	CColorBtn	m_clTextColor;
	CColorBtn	m_clBkgColor;
	CColorBtn	m_clKosdaqColor;
	CColorBtn	m_clKospiColor;
	CButton	m_cbClrApply;	
	CButton	m_cbShadow;
	CButton	m_cbMKAllApply;
	CButton	m_cbWaveApply;
	CStatic	m_staticFile;
	CButton	m_cbAdd;
	CButton	m_cbChg;
	CButton	m_cbAllApply;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUsefulSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes
public:
	BOOL		m_bClrApply{};
	COLORREF	m_clrKospi{};
	COLORREF	m_clrKosdaq{};

	COLORREF	m_clrMarkerBKG{};
	COLORREF	m_clrMarkerTXT{};
	BOOL		m_bShadow{};
	BOOL		m_bMKAllApply{};

	CString		m_strFile;
	BOOL		m_bWaveApply{};

	BOOL		m_bAddCnd{};
	BOOL		m_bChgCnd{};
	BOOL		m_bAllApply{};

// Operators
private:
	void EnableConrol(int nType, BOOL bEnable);
	void PlayWave(CString strFile);


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUsefulSetupDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCheckClrApply();
	afx_msg void OnCheckAlertApply();
	afx_msg void OnButtonOpen();
	afx_msg void OnButtonWavlisten();
	afx_msg void OnStaticItemchg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
