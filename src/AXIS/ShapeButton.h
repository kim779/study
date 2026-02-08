#if !defined(AFX_ShapeButton_H__62F88637_FBBC_4595_BDB7_3A8F2A739730__INCLUDED_)
#define AFX_ShapeButton_H__62F88637_FBBC_4595_BDB7_3A8F2A739730__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ShapeButton.h : header file
//

#define BMBS_HOVER	1
#define BMBS_DOWN	2
/////////////////////////////////////////////////////////////////////////////
// CShapeButton window

class CShapeButton : public CWnd
{
// Construction
public:
	CShapeButton(CFont* pFont = NULL);

// Attributes
public:	
	DWORD		m_dwState{};
	bool		m_capture, m_bChecked, m_bCheck, m_bStretch, m_bFocus, m_bDrawFocus	;
	BOOL		m_bHover, m_bTracking;
	HBITMAP		m_bmp;

protected:
	CFont		*m_pFont;
	//CToolTipCtrl	*m_pTipCtrl;
	std::unique_ptr <CToolTipCtrl> m_pTipCtrl;

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
	void	SetFont(CFont* pFont, BOOL bRedraw = true);
	void	SetCheck(bool bCheck = true);
	void	SetDrawFocus(bool bDraw = true);

	bool	IsChecked();

protected:
	CString	Parser(CString &srcstr, CString substr);
	CSize	BitmapSize(HBITMAP hBitmap);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShapeButton)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void CreateTooltip();
	void FitImageSize();
	virtual ~CShapeButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShapeButton)
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
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
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

#endif // !defined(AFX_ShapeButton_H__62F88637_FBBC_4595_BDB7_3A8F2A739730__INCLUDED_)
