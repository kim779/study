// NpnxMgr.h: interface for the CNpnxMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPNXMGR_H__8BB377F1_D300_4A4A_946B_C627CC1EE00A__INCLUDED_)
#define AFX_NPNXMGR_H__8BB377F1_D300_4A4A_946B_C627CC1EE00A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNpnxMgr  
{
public:
	void Check(BOOL bOutMessage = FALSE);
	void Quick(BOOL bOutMessage = FALSE);
	BOOL IsExecute();
	void CloseNetizen();
	void Init(BOOL bOutMessage = FALSE);
	BOOL isAdmin();
	CNpnxMgr();
	~CNpnxMgr();

	HMODULE m_hModule;
	HWND	m_hWnd;
	DWORD	m_dwhwnd;

	BOOL (*lpfnGoNPN)(char *szID);
	BOOL (*lpfnIsExecute)(void);
	BOOL (*lpfnCloseNPN)(void);
	BOOL (*lpfnGoNPNEx)(char *szID,  char *szCurrentPath);
	BOOL (*lpfnGoNPNQuick)(char *szID,  char *szCurrentPath);
	BOOL (*lpfnGoNPNCheck)(char *szID,  char *szCurrentPath);
	BOOL (*lpfnCopyModule)(char *szCurrentPath);
	void (*lpfnSetHwnd)(DWORD dwHwnd);

	CString m_strCustomerID;
	CString m_strCurrentPath;
	CString m_strModulePath;
	CString m_strCheckTime;
	BOOL	m_bOutMessage;
};

#endif // !defined(AFX_NPNXMGR_H__8BB377F1_D300_4A4A_946B_C627CC1EE00A__INCLUDED_)
