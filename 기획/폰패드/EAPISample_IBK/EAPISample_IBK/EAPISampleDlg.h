#pragma once

#include "EAPIInterface.h"
#include "EAPIInterfaceDefine.h"

#define WM_APP_LOG      (WM_APP + 1)

class CEAPISampleDlg : public CDialog
{
public:
	CEAPISampleDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_EAPISAMPLE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	// Connect / Login
	CString	m_strIP;
	int		m_nPort;
	CString	m_strID;
	CString	m_strDevice;
	CString	m_strUnique;

	// PhonePad
	CString	m_strPPDialNum;
	CString	m_strPPCustData;

	// Log
	CEdit	m_editLog;

	// Status
	CStatic	m_stStatus;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();

	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedBtnDisconnect();
	afx_msg void OnBnClickedBtnLogin();
	afx_msg void OnBnClickedBtnLogout();
	afx_msg void OnBnClickedBtnPhonepad();
	afx_msg void OnBnClickedBtnPhonepadCheck();
	afx_msg void OnBnClickedBtnClearLog();

	afx_msg LRESULT OnAppLog(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	void AppendLog(const CString& strMsg);
	void PostLogFromCallback(const char* pszMsg);
	void SetStatus(const char* pszStatus);

	// DLL Callback (static)
	static DWORD CB_Resp(int nMessageID, char* pszUniqueNO, char* pszDeviceType,
		char* pszResult, char* pszCause, char* pszCauseStr, HWND hWnd, HANDLE hHnd);
	static DWORD CB_Event(int nMessageID, char* pszCallID1, char* pszCallID2,
		char* pszANI, char* pszDNIS, char* pszCallType, char* pszCallDirection,
		char* pszPrevStatus, char* pszCallStatus, char* pszDeviceID,
		HWND hWnd, HANDLE hHnd);
	static DWORD CB_UUI(int nMessageID, char* pszCallID, char* pszUUIData,
		char* pszResult, char* pszCause, char* pszCauseStr, HWND hWnd, HANDLE hHnd);

	static CEAPISampleDlg* s_pDlg;
};
