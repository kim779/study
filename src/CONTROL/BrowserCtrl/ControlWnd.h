#pragma once
// ControlWnd.h : header file
//

class CParam
{
public:
	int		key;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point;			// font point
	int		style;			// font style
	DWORD		tRGB;			// text color index
	DWORD		pRGB;			// paint color index
	CString		options;		// properties
};
/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd();

	//BOOL	Create(CWnd* pParent, void* param);
	void	SetParam(_param *pParam);
	void	ReCalcLayout();

// Attributes
public:
	CWnd	*m_pParent;
	std::unique_ptr<class SimpleBrowser>	m_pWebBrowser;
	std::unique_ptr<class CEdgeBrowser>	m_pEdgeBrowser;

	CParam	m_Param{};
	CString	m_strURL;
	CString	m_strMap1;
	CString	m_strMap2;

	CString	m_strClassName;

	CString m_SData;
	CString m_sTime;

	bool	m_bEdgeBrowser{};
	bool	m_bParentWnd{};

	BOOL m_visible;
	int m_nLeft;
	int m_nTop;
	int m_nRight;
	int m_nBottom;

//	HRESULT InitializeWebView();
//	static CString GetInstallPath();
//	static CString GetInstallPathFromRegistry(bool const searchWebView = true);
//	static CString GetInstallPathFromDisk(bool const searchWebView = true);
//	static CString GetUserDataFolder();

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
	//}}AFX_MSG
	afx_msg long OnUser(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	afx_msg void viewURL(LPCTSTR url);
	afx_msg void Print();
	afx_msg void Save(LPCTSTR strType, LPCTSTR strTitle, LPCTSTR strCodeName, LPCTSTR strDate, LPCTSTR strTime, LPCTSTR strSubType, LPCTSTR strCode, LPCTSTR strKey);
	afx_msg void _PrintData(LPCTSTR sData, LPCTSTR sTime);
	afx_msg LRESULT OnOpenScreen(WPARAM wParam, LPARAM lParam);
	afx_msg void navigate(LPCTSTR url);
	afx_msg void Refresh();
	afx_msg void SetRect(long Left, long top, long right, long bottom);
	afx_msg void OnVisibleChanged();
	afx_msg void OnTextChanged();
	afx_msg void OnLeftChanged();
	afx_msg void OnTopChanged();
	afx_msg void OnRightChanged();
	afx_msg void OnBottomChanged();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

