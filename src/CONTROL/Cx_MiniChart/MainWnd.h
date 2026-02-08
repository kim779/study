#pragma once
// MainWnd.h : header file
//

class CParam
{
public:
	int		key{};			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point{};			// font point
	int		style{};			// font style
	DWORD		tRGB{};			// text color index
	DWORD		pRGB{};			// paint color index
	CString		options;		// properties
};



/////////////////////////////////////////////////////////////////////////////
// CMainWnd window

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd(CWnd *pParent, _param* pInfo);

// Attributes
public:

// Operations
	CFont*	GetFont(int point, CString name, int bold = 0, bool italic = false);
	CPen*	GetPen(int style, int width, COLORREF clr);
	CBrush*	 GetBrush(COLORREF rColor);
	CBitmap* GetBitmap(CString path);	
	
	void	ReadCondition(struct _minijisu *mj);
	void	SaveCondition(struct _minijisu *mj);
	CString GetImgPath() { return m_imgPath; }

	
	CString	m_imgPath;

	CWnd	*m_pView;
	CString	m_infoPath;
	
	
	void	MakeBasicInfo(struct _minijisu *mj);
	bool	readFile(CString path, char *pBytes, int nBytes);
	bool	writeFile(CString path, char *pBytes, int nBytes);

	void	SendTR(CString name, CString data, BYTE type, int key, CString keyName);

	void	SetParam(_param *pParam);
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
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)
	BOOL m_show;
	afx_msg BSTR GetProperties();
	afx_msg void SetProperties(LPCTSTR str);
	afx_msg BOOL GetVisible();
	afx_msg void SetVisible(BOOL bNewValue);
	afx_msg void Send();
	CString m_strMarket;
	afx_msg void OnsMarketChanged();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	std::unique_ptr<class CGrpWnd>	m_pGrpWnd;

	CWnd* m_pParent{};

//	struct _param	m_param;
	CParam m_param;

	bool		m_bCtrl{};
	
	void	Resize();
	enum
	{
		dispidSetMarket = 2L,
		dispidsMarket = 1
	};
	void SetMarket(SHORT sMarket);
};