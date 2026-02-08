/************************************************************************/
/* FILENAME:	WebLinkCtrl.h                                           */
/* DESCRIPTION: IBK투자증권 MAC: 웹링크용 Control인 WebLinkCtrl 정의헤더*/
/* WRITE:		Macho @2008-06-04                                       */
/* UPDATE:                                                              */
/*                                                                      */
/*                                                                      */
/************************************************************************/
#pragma once

#include "WebBrowserCtrl.h"
#include "../../h/axisfire.h"
// WebLinkCtrl.h : header file
//
#define		WM_BROWSER	WM_USER + 9955
/////////////////////////////////////////////////////////////////////////////
// WebLinkCtrl window

class WebLinkCtrl : public CWnd
{
	DECLARE_DYNCREATE(WebLinkCtrl)
// Construction
public:
	WebLinkCtrl();
	CWnd* m_pMainFrame{};
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(WebLinkCtrl)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~WebLinkCtrl();

	// Generated message map functions
protected:
	CString SetNoParam();
	//{{AFX_MSG(WebLinkCtrl)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	LRESULT OnUser(WPARAM wParam, LPARAM lParam);
	LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	LRESULT OnBrowser(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(WebLinkCtrl)
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(WebLinkCtrl)
	afx_msg BSTR GetBaseURL();
	afx_msg void SetBaseURL(LPCTSTR lpszNewValue);
	afx_msg BSTR GetItemCode();
	afx_msg void SetItemCode(LPCTSTR lpszNewValue);
	afx_msg BSTR GetUserID();
	afx_msg void SetUserID(LPCTSTR lpszNewValue);
	afx_msg BSTR GetHome();
	afx_msg void SetHome(LPCTSTR lpszNewValue);
	afx_msg BSTR GetMenu();
	afx_msg void SetMenu(LPCTSTR lpszNewValue);
	afx_msg short GetWidth();
	afx_msg short GetHeight();
	afx_msg void ResizeToFitWindow();
	afx_msg void Navigate();
	afx_msg BSTR GetCaption();
	afx_msg void OnBeforeNavigate2WebCtrl(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel);
	afx_msg void OnDocumentComplete(LPDISPATCH lpDisp, VARIANT FAR* URL);
	afx_msg void OnNavigateError(LPDISPATCH lpDisp, VARIANT FAR* URL, VARIANT FAR* TargetFrameName, VARIANT FAR* StatusCode, BOOL FAR* Cancel);
	afx_msg void Navigate2(LPCTSTR sUrl, bool bLogin);
	afx_msg void NavigateFRmap(LPCTSTR sUrl, bool bLogin);
	//}}AFX_DISPATCH
	DECLARE_EVENTSINK_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
private:
	CString m_className;
	void	RegisterClass();
	HINSTANCE m_hInst;
	BOOL	m_bCertLogin;
	CString m_sCert;
	CString m_sUrl;

private:

	void SetParam(_param *pParam);
	// member variables..
	CString m_userID, m_baseURL, m_itemCode, m_home, m_menu;	// web parameters & information
	CString m_type,	m_description, m_url, m_finalurl;		// web parameters & information
	int	m_width, m_height;					// web page's size information
	_param*	m_param;						// parameter from map
	_param  m_Param;
	CWnd*	m_parent;						// parent window pointer
	std::unique_ptr<CWebBrowserCtrl> m_ctrl;			// wrapped web browser control
	CString m_sRoot;
	CString m_sPath;
	CString m_sMapName;


	CString m_time{};
	bool m_bNavigate2;
	bool m_bBrowser;
	IWebBrowserApp* m_pIEApp{};
	IWebBrowser2* pBrowser2{};
	// member functions..
	CString GetURL();						// generate URL to navigate
	CString GetUserPassword();					// getting user password
	CString GetCertPassword();					// getting user certify password
	void	GetInformation();					// getting web information
	CString FindMap(LPCTSTR map);					// getting real map name by screen number

	CString	URLEncode(const char* lpszURL);				// URL Encoding for Control-characters
	CString GetAuthParam();						// generating & getting 'Auth' parameter for IBK Web-page
	CString HTSEncode(const char* lpszSource, const char* key);	// encoding data by IBK encoding rule.
	void GetLocalIP();
	void GetMAC();
	int GetProcssNameByPID(DWORD dwPID, char *buff, int size);
	BOOL IsNumber(CString str);
	BOOL CheckBrowserVersion();

	CString m_ipAddr;
	CString m_MacAddr;

	CString m_strUrl;
	void openhtml();
	int GetOSversion();
	void WriteLog(LPCSTR log, ...);
public:
	LRESULT SendTR(CString strName, BYTE type, CString strData, BYTE key);
	BOOL IsDev();
	CString Variant(int comm, CString data);
	CWnd* m_pMainWnd;
	// window creation function
	BOOL Create(CWnd* parent, _param* param);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	

	
	void SetWidth(LONG width);

	enum
	{
		dispidSetHeight = 17L,
		dispidSetWidth = 16L
	};
	void SetHeight(LONG height);
};