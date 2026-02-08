#pragma once
// WebLinkCtrl.h : header file
//
#include "../../h/axisfire.h"
#include "WebBrowserCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// WebLinkCtrl window
#define		WM_BROWSER	WM_USER + 9955

class WebLinkCtrl : public CWnd
{
// Construction
public:
	WebLinkCtrl();
	
// Attributes
public:
	CWnd*	m_parent;
	_param*	m_param;
	_param  m_Param;
	std::unique_ptr<CWebBrowserCtrl> m_ctrl;		
	IWebBrowserApp*  m_pIEApp;
	IWebBrowser2*    m_pBrowser2;

	int		m_igubn;
	int		m_width, m_height;
	enum {IE = 1, EDGE, CHROME};

	CString m_className;
	CString m_url;
	CString Parser(CString &srcstr, CString substr);
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
	BOOL	create(CWnd* parent, _param* param);
	BOOL	SetParam(CWnd* parent, _param* param);
	void	SetParam(_param *pParam);
	void	ResizeToFitWindow();
	// Generated message map functions
protected:
	//{{AFX_MSG(WebLinkCtrl)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	LRESULT OnBrowser(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(WebLinkCtrl)
	afx_msg void Navigate(LPCTSTR url);
	afx_msg short PopIE(LPCTSTR surl);
	afx_msg short PopUrl(LPCTSTR surl);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};