#pragma once
// Memo.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CEditX window

class CEditX : public CEdit
{
// Construction
public:
	CEditX();

// Attributes
public:
	CBrush	m_brush;

// Operations
public:
	BOOL	Create(CWnd* parent, UINT nID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditX)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditX();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditX)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMemo window

class CMemo : public CWnd
{
// Construction
public:
	CMemo(CString code, CString jnam, CString root, CString name);
	void ShowMemo(CString code, CString name, CRect rect);

// Attributes
public:
	bool	m_fDone;

	CString	m_root;
	CString	m_name;
	CString	m_code;
	CString	m_jnam;

	CFont*	m_font;
	std::unique_ptr<CEditX>	m_edit;

	CWnd*	m_sendW;
	CWnd*	m_parent;
	CWnd* m_pMainWnd{};
	CString m_memoState{};   //첫바이트가 0 -> 조회중 , 1->서버에 있음 , 2->서버에 없음
	CString m_slog{};
	BOOL m_bMemoFile{};
	void deleteServerMemo(CString skey);
	int m_iLoadCnt{}; //서버종목메모 조회결과가 이상하면 1초에 한번씩 5번만 더 시도해본다
// Operations
public:
	BOOL	createMemo(CWnd* parent, CWnd* sendW, CWnd* mainW, CRect rect);
	void	drawCaption(CDC *pDC);
	void	saveMemo(CString skey);
	//void	loadMemo(CString skey);
	//CString	loadMemoUsingTip(CString skey);
	void	deleteMemo(CString skey);
	void	procedureDeleteMemo(CString code);

	CPen*	getAxPen(COLORREF clr, int width, int style);
	CBrush* getAxBrush(COLORREF clr);
	CFont*	getAxFont(CString fName, int point, int style);
	bool    openMemo(CString sCode);
	BOOL    ShowWindow(int nCmdShow){
		m_fDone = TRUE;
		if (GetSafeHwnd())
			return CWnd::ShowWindow(nCmdShow);
		else
			return FALSE;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMemo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMemo();

private:
	class CCX_INTERGRIDApp* _pApp{};

	// Generated message map functions
protected:
	//{{AFX_MSG(CMemo)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

