#pragma once
// ControlWnd.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

class CParam
{
public:
	int		key=0;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point=0;		// font point
	int		style=0;		// font style
	DWORD		tRGB=0;			// text color index
	DWORD		pRGB=0;			// paint color index
	CString		options;		// properties
};

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CControlWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnUser(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	CString m_strLog;
	afx_msg void OnStrLogChanged();
	afx_msg BSTR GetProperties();
	afx_msg void SetProperties(LPCTSTR sParam);
	afx_msg void viewURL(LPCTSTR url);
	afx_msg void _Seturl(LPCTSTR url);
	afx_msg void SetVisible(BOOL bVisible);
	afx_msg void Seturl2(LPCTSTR url);
	afx_msg void GoBack();
	afx_msg void GoForward();
	afx_msg void PopUpUrl(LPCTSTR url);
	afx_msg void PopUpChrome(LPCTSTR url);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

public:
	BOOL	IsDev();
	CWnd	*m_pParent;
	class CAxBrowser	*m_pHtml;
	class Ccx_THtmlExApp	*m_pApp;

	void	SetParam(struct _param *pParam);

	CParam	m_Param;
	void	SetBrowserReg();
	void	CheckOS();
	CString	m_sos;
	CString	m_sprocess;

private:
	CFont	*m_pNFont, *m_pBFont;
	CRect	m_Htmlrc;
	CString	m_sURL;
	bool	m_bScroll;
	int	m_scrGap;
protected:
	void Seturl3(BSTR strUri);

	enum
	{
		dispidSeturl3 = 13L
	};
};