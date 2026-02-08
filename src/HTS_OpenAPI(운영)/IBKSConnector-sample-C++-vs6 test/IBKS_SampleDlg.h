// IBKS_SampleDlg.h : header file
//
//{{AFX_INCLUDES()
#include "ibksconnector.h"
//}}AFX_INCLUDES

#if !defined(AFX_IBKS_SAMPLEDLG_H__254E8314_84A3_414C_824D_ECD76353BF0E__INCLUDED_)
#define AFX_IBKS_SAMPLEDLG_H__254E8314_84A3_414C_824D_ECD76353BF0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Common.h"

class CIBKS_SampleDlg : public CDialog
{
public:
	CIBKS_SampleDlg(CWnd* pParent = NULL);	// standard constructor

protected:
	void ParseString(LPCSTR st, LPCSTR ed, CHAR del, vector<StringProxy> &retval);
	void ParseString(StringProxy &sp, CHAR del, vector<StringProxy> &retval);

	#define STR(field)	GetString(field, sizeof(field))
	LPCSTR GetString(LPCSTR dat, int len);
	void CreateHorizontalScroll();

public:
// Dialog Data
	//{{AFX_DATA(CIBKS_SampleDlg)
	enum { IDD = IDD_IBKS_SAMPLE_DIALOG };
	CComboBox	m_cbJType;
	CButton	m_rdMptest;
	CButton	m_rdKhtest;
	CComboBox	m_cbAccn2;
	CListBox	m_lbxJngo;
	CListBox	m_cbCheg;
	CEdit	m_edRdResult;
	CEdit	m_edResult;
	CComboBox	m_cbMmgb;
	CComboBox	m_cbHogb;
	CComboBox	m_cbAccn;
	CString	m_CertPW;
	CString	m_UserID;
	CString	m_UserPW;
	CIBKSConnector	m_axIBKS;
	CString	m_odrAccn;
	CString	m_odrPswd;
	CString	m_odrCode;
	CString	m_odrHogb;
	CString	m_odrJprc;
	CString	m_odrJqty;
	CString	m_odrMmgb;
	CString	m_odrOgno;
	CString	m_siseCode;
	CString	m_jngoAccn;
	CString	m_jngoPswd;
	CString	m_ServerIP;
	//}}AFX_DATA

	BOOL m_bGreeks1;
	BOOL m_bGreeks2;

	CString m_strGreeks1;
	CString m_strGreeks2;
	CString m_strGreeksCode;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIBKS_SampleDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CIBKS_SampleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnConnect();
	afx_msg void OnEvtLogin(BOOL bLogin, LPCTSTR msg);
	afx_msg void OnEvtContract(LPCTSTR data);
	afx_msg void OnEvtRealData(LPCTSTR data);
	afx_msg void OnEvtRecvData(long key, long dptr, long size, BOOL bNext, LPCTSTR nkey);
	afx_msg void OnEvtError(LPCTSTR msg);
	afx_msg void OnEvtClose();
	afx_msg void OnEvtAlert(long code, LPCTSTR msg);
	afx_msg void OnOdrSend();
	afx_msg void OnSiseSend();
	afx_msg void OnJngoSend();
	afx_msg void OnBtnDisconnect();
	afx_msg void OnEvtVerUpdate();
	afx_msg void OnEvtUpdateStart();
	afx_msg void OnEvtUpdating(LPCTSTR sData, long nProgress);
	afx_msg void OnEvtUpdateEnd();
	afx_msg void OnEvtGuideMsg(long key, LPCTSTR sMsg);
	afx_msg void OnEvtFBalance(LPCTSTR data);
	afx_msg void OnEvtSBalance(LPCTSTR data);
	afx_msg void OnJngoSend2();
	afx_msg void OnSiseSend2();
	afx_msg void OnSiseSend3();
	afx_msg void OnSiseSend4();
	afx_msg void OnSiseSend5();
	afx_msg void OnGreeks();
	afx_msg void OnMonSise();
	afx_msg void OnSonIk();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IBKS_SAMPLEDLG_H__254E8314_84A3_414C_824D_ECD76353BF0E__INCLUDED_)
