#pragma once
// MainWnd.h : header file

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window
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

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd(CWnd* pParent, _param* pParam);
	virtual ~CMainWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	public:
	virtual void OnFinalRelease();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	CString	m_sUserID;
	CString m_sRoot;
	CParam	m_Param;
	CWnd*	m_pParent;
	CString	m_caption;

	void	SetParam(_param *pParam);
	void	SetAccountInfo(CString acnt_info);
	void	Initialize(BOOL tp);

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LONG OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnDataTrans(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)
	CString m_data;
	afx_msg void OnDataChanged();
	afx_msg BOOL GetVisible2();
	afx_msg void SetVisible2(BOOL bNewValue);
	afx_msg BOOL GetVisible();
	afx_msg void SetVisible(BOOL bNewValue);
	afx_msg BSTR GetStrTemp();
	afx_msg void SetStrTemp(LPCTSTR lpszNewValue);
	afx_msg void SetProperties();
	afx_msg BSTR GetProperties();
	afx_msg BOOL RequestTR();
	afx_msg BOOL Config();
	afx_msg long GetTotalDay();
	afx_msg long GetDisplayDay();
	afx_msg void OnVisibleChanged();
	afx_msg BSTR GetTypeData(short typ, LPCTSTR str1);
	afx_msg BSTR GetMemoData(short typ, LPCTSTR idata, LPCTSTR mdata);
	afx_msg void Refresh813(long tp);
	afx_msg long GetWidth();
	afx_msg long GetHeight();
	afx_msg BSTR _Get813Group(LPCTSTR groupid);
	afx_msg BSTR _GetGet813Account(LPCTSTR grp);
	//}}AFX_DISPATCH
	
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};