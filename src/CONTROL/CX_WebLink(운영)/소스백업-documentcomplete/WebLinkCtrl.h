/************************************************************************/
/* FILENAME:	WebLinkCtrl.h                                           */
/* DESCRIPTION: IBK투자증권 MAC: 웹링크용 Control인 WebLinkCtrl 정의헤더*/
/* WRITE:		Macho @2008-06-04                                       */
/* UPDATE:                                                              */
/*                                                                      */
/*                                                                      */
/************************************************************************/

#if !defined(AFX_WEBLINKCTRL_H__F3EF4086_21E0_4BCE_B736_1AE3527769B1__INCLUDED_)
#define AFX_WEBLINKCTRL_H__F3EF4086_21E0_4BCE_B736_1AE3527769B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WebBrowserCtrl.h"
// WebLinkCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// WebLinkCtrl window

class WebLinkCtrl : public CWnd
{
	DECLARE_DYNCREATE(WebLinkCtrl)
// Construction
public:
	WebLinkCtrl();

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
	//{{AFX_MSG(WebLinkCtrl)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	LRESULT OnMessage(WPARAM wParam, LPARAM lParam);

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
	//}}AFX_DISPATCH
	DECLARE_EVENTSINK_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
private:
	CString m_className;
	void	RegisterClass();
private:
	// member variables..
	CString m_userID, m_baseURL, m_itemCode, m_home, m_menu;	// web parameters & information
	CString m_type,	m_description, m_url;						// web parameters & information
	int		m_width, m_height;									// web page's size information
	void*	m_param;											// parameter from map
	CWnd*	m_parent;											// parent window pointer
	CWebBrowserCtrl* m_ctrl;									// wrapped web browser control

	// member functions..
	CString GetURL();											// generate URL to navigate
	CString GetUserPassword();									// getting user password
	CString GetCertPassword();									// getting user certify password
	void	GetInformation();									// getting web information
	CString FindMap(LPCTSTR map);								// getting real map name by screen number

	CString	URLEncode(const char* lpszURL);						// URL Encoding for Control-characters
	CString GetAuthParam();										// generating & getting 'Auth' parameter for IBK Web-page
	CString HTSEncode(const char* lpszSource, const char* key); // encoding data by IBK encoding rule.
public:
	int kill_count;
	// window creation function
	BOOL Create(CWnd* parent, void* param);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBLINKCTRL_H__F3EF4086_21E0_4BCE_B736_1AE3527769B1__INCLUDED_)
