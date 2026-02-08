#if !defined(AFX_MAINWND_H__79F491B7_F6A1_4445_82CA_E672611EF4C1__INCLUDED_)
#define AFX_MAINWND_H__79F491B7_F6A1_4445_82CA_E672611EF4C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainWnd.h : header file
//
#include "BrowserWindowEdge.h"

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window


class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd();
	virtual ~CMainWnd();
	void	SetParam(struct _param* pParam);


	//Edge window
	std::unique_ptr<BrowserWindowEdge> browserEdge{};
	CString URLEncode(const char* lpszURL);
	std::string UrlEncode(const std::string& value);
protected:

	
// Attributes
public:

// Operations
public:
	CWnd* m_pParent{};
	CParam m_Param;
	CString m_strurl;
	CString m_slog;
private:
	CString _sURL;
	bool	_bScroll{};

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	enum
	{
		dispidBrowserPopup = 7L,
		dispidSetScreenZoomRate = 6L,
		dispidGoForward = 5L,
		dispidGoBack = 4L,
		dispidcloseEdge = 3L,
		dispidNavigatesrc = 2L,
		dispidNavigate = 1L,
	};

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:

	void Navigate(BSTR url);
	void Navigatesrc(BSTR src);
	void Navigate_strUrl();
	void closeEdge();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	void GoBack();
	void GoForward();
	void SetScreenZoomRate(DOUBLE dRate);
	void BrowserPopup(BSTR sUrl);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINWND_H__79F491B7_F6A1_4445_82CA_E672611EF4C1__INCLUDED_)
