#pragma once

#include "resource.h"
#include "SetupDialog.h"
// CAlarmDlg dialog

class AFX_EXT_CLASS CAlarmDlg : public CSetupDialog
{
//	DECLARE_DYNAMIC(CAlarmDlg)

public:
	CAlarmDlg(CWnd* pParent = NULL);   // standard constructor
	CAlarmDlg(class CTMenu* menu, CString home, CString user, CWnd* pParent = NULL);  
	CString m_home;
	CString m_user;

	void Init();
	void CheckAlarm();
	void save_info();
	void	ApplySetup();
	virtual ~CAlarmDlg();

// Dialog Data
	enum { IDD = IDD_DALMDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnThemechangedSpinPop(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinPop(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedChkGong();
	afx_msg void OnBnClickedChkSu();
	afx_msg void OnBnClickedChkBal();
};
