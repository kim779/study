#pragma once

class CImgBtn : public CButton
{
public:
	CImgBtn();

public:
	DWORD		m_dwState{};
	bool		m_capture, m_bChecked, m_bCheck, m_bToolTip;
	BOOL		m_bHover, m_bTracking;
	HBITMAP		m_hBitmap, m_hBitmap_dn, m_hBitmap_hv;
protected:
	CFont		*m_pFont, m_font;
	std::unique_ptr<CToolTipCtrl> m_pTipCtrl;
private:
	COLORREF	m_clText;
	bool		m_bFocus;

public:

// Overrides
	//{{AFX_VIRTUAL(CImgBtn)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

public:
	void	FitImageSize();
	void	CreateTooltip();
	void	setCheck(bool bCheck = true);
	void	setTextColor(COLORREF clText);
	long	OnGetCheck(WPARAM wParam, LPARAM lParam);
	
	LOGFONT setFont(int fsize, CString fname, BOOL bBold, BOOL bItalic);
	CString	Parser(CString &srcstr, CString substr);
	
	bool	IsChecked();
	CSize	BitmapSize(HBITMAP hBitmap);

	virtual ~CImgBtn();

protected:
	afx_msg LRESULT OnSetCheck(WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CImgBtn)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg LRESULT HandleSetFont(WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
