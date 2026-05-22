#if !defined(AFX_MAINWND_H__94B299AB_7B42_4614_8B58_1D6A7E5C2889__INCLUDED_)
#define AFX_MAINWND_H__94B299AB_7B42_4614_8B58_1D6A7E5C2889__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window
#include "include/EAPIInterface.h"
#include "include/EAPIInterfaceDefine.h"

#define WM_APP_LOG      (WM_APP + 1)

class CParam
{
public:
	int		key = 0;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point = 0;		// font point
	int		style = 0;		// font style
	DWORD		tRGB = 0;			// text color index
	DWORD		pRGB = 0;			// paint color index
	CString		options;		// properties
};

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd(CWnd* pParent, _param* pParam);

// Attributes
public:
	enum {
		LOGOUT = 0, LOGINED = 1, LOGING = 2};

	int m_iLoginState{};   
	int m_nPort{};
	CString m_strIP{};
	CString	 m_strID{};
	CString	 m_strDevice{};
	CString m_strUnique{};
	CString m_sAccPass{};
	CString m_sState{};
	CString m_sMsgID{};
	// PhonePad
	CString	m_strPPDialNum{};
	CString	m_strPPCustData{};

	CParam m_Param{};
	CWnd* m_pWizard{};
	void	SetParam(_param* pParam);
	void parseOptions();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)
	afx_msg BOOL DoSomething(short type, BSTR sval);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	enum
	{
		dispidsMSGID = 12,
		dispidblogin = 11,
		dispidsState = 10,
		dispidSendMSgToMain = 8L,
		dispidsRes = 7,
		dispidEAPIPhonePad = 6L,
		dispidEAPILogout = 5L,
		dispidEAPILogin = 4L,
		dispidEAPIDisconnect = 3L,
		dispidEAPIConnect = 2L
	};
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnAppLog(WPARAM wParam, LPARAM lParam);


public:
	void SetStatus(const char* pszStatus);
	void PostLogFromCallback(const char* pszMsg);
	void SendPassToMap();

	static DWORD CB_Resp(int nMessageID, char* pszUniqueNO, char* pszDeviceType,
		char* pszResult, char* pszCause, char* pszCauseStr, HWND hWnd, HANDLE hHnd);
	static DWORD CB_Event(int nMessageID, char* pszCallID1, char* pszCallID2,
		char* pszANI, char* pszDNIS, char* pszCallType, char* pszCallDirection,
		char* pszPrevStatus, char* pszCallStatus, char* pszDeviceID,
		HWND hWnd, HANDLE hHnd);
	static DWORD CB_UUI(int nMessageID, char* pszCallID, char* pszUUIData,
		char* pszResult, char* pszCause, char* pszCauseStr, HWND hWnd, HANDLE hHnd);

	static CMainWnd* s_pDlg;
protected:
	VARIANT_BOOL _EAPIConnect(BSTR ip, SHORT port);
	VARIANT_BOOL _EAPIDisconnect();
	VARIANT_BOOL _EAPILogin(BSTR sID, BSTR sDvcID, BSTR sUnqID, BSTR sOtpion);
	VARIANT_BOOL _EAPILogout();
	VARIANT_BOOL _EAPIPhonePad(BSTR sCallID, BSTR sSvcCode, BSTR sCustData, BSTR sDialNumber);
public:
	afx_msg void OnDestroy();
protected:
	CString m_sRes{};
	void _SendMSgToMain(BSTR sMsg);
	BSTR GetsRes();
	void SetsRes(BSTR newVal);
	BSTR GetsMSGID();
	void SetsMSGID(BSTR newVal);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	BSTR GetsState();
	void SetsState(BSTR newVal);
	SHORT Getblogin();
	void Setblogin(SHORT newVal);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINWND_H__94B299AB_7B42_4614_8B58_1D6A7E5C2889__INCLUDED_)
