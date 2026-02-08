#if !defined(AFX_BMPBUTTON_H__C450698F_CFF1_4CE2_B739_07F5FB6AF016__INCLUDED_)
#define AFX_BMPBUTTON_H__C450698F_CFF1_4CE2_B739_07F5FB6AF016__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BmpButton.h : header file
//

#define BMBS_HOVER	1
#define BMBS_DOWN	2
/////////////////////////////////////////////////////////////////////////////
// CBmpButton window

class CBmpButton : public CWnd
{
// Construction
public:
	CBmpButton(CFont* pFont = NULL);

// Attributes
public:	
	DWORD		m_dwState{};
	bool		m_capture, m_bChecked, m_bCheck, m_bStretch, m_bFocus, m_bDrawFocus	;
	BOOL		m_bHover, m_bTracking, m_bMouseIn{};
	BOOL		m_bImageSelfLoaded{};
	HBITMAP		m_bmp;

protected:
	CFont		*m_pFont;
	std::unique_ptr< CToolTipCtrl> m_pTipCtrl;
	int			m_nID{};
	int			m_statusCount;
	CSize		m_bmpSize;
	void		ReleaseBitmap();
	int			GetBitmapOffset(int status);
private:
	COLORREF	m_clText;

// Operations
public:
	BOOL	Create(LPCTSTR caption, DWORD style, const RECT& rect, CWnd* parent, UINT nID, bool bCheck = FALSE, bool bStretch = FALSE);

	void	SetTextColor(COLORREF clText);
	void	SetImgBitmap(const char* bmpName, int statusCount = 3);
	void	SetImgBitmap(CBitmap* bmp, int statusCount = 3);
	void	SetFont(CFont* pFont, BOOL bRedraw = true);
	void	SetCheck(bool bCheck = true);
	void	SetDrawFocus(bool bDraw = true);

	bool	IsChecked();

protected:
	CString	Parser(CString &srcstr, CString substr);
	CSize	BitmapSize(HBITMAP hBitmap);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBmpButton)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void CreateTooltip();
	void FitImageSize();
	virtual ~CBmpButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBmpButton)
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
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);  //vc2019 void->LRESULT
	afx_msg LRESULT OnBmSetCheck(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBmGetCheck(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	BOOL IsEnable();
	void SetEnable(BOOL enable);

	void SetCheckButton(BOOL check);
	BOOL IsCheckButton();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMPBUTTON_H__C450698F_CFF1_4CE2_B739_07F5FB6AF016__INCLUDED_)
