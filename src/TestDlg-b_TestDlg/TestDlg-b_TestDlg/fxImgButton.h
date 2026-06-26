#pragma once
// fxImgButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CfxImgButton window

class AFX_EXT_CLASS CfxImgButton : public CWnd
{
// Construction
public:
	CfxImgButton(CFont* pFont = nullptr);

// Attributes
public:	
	DWORD		m_dwState{};
	bool		m_capture{}, m_bChecked{}, m_bCheck{}, m_bStretch{}, m_bFocus{}, m_bDrawFocus{};
	BOOL		m_bHover{}, m_bTracking{};
	HBITMAP		m_hBitmap{}, m_hBitmap_dn{}, m_hBitmap_hv{};

protected:
	CFont		*m_pFont{};
	std::unique_ptr<CToolTipCtrl>	m_pTipCtrl{};

private:
	COLORREF	m_clText{};
	BOOL		m_bDisableDraw{};

// Operations
public:
	BOOL	Create(LPCTSTR caption, const RECT& rect, CWnd* parent, UINT nID, bool bCheck = FALSE, bool bStretch = FALSE);

	void	SetTextColor(COLORREF clText);
	void	SetImgBitmap(HBITMAP hBitmap, HBITMAP hBitmap_dn, HBITMAP hBitmap_hv);
	void	SetFont(CFont* pFont, BOOL bRedraw = true);
	void	SetCheck(bool bCheck = true);
	void	SetDrawFocus(bool bDraw = true);

	bool	IsChecked();

protected:
	CString	Parser(CString &srcstr, CString substr);
	CSize	BitmapSize(HBITMAP hBitmap);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfxImgButton)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetDisableDraw(BOOL bDraw);
	void CreateTooltip();
	void FitImageSize();
	virtual ~CfxImgButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CfxImgButton)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);

	afx_msg void OnSetCheck(WPARAM wParam, LPARAM lParam);
	afx_msg long OnGetCheck(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

