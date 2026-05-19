#pragma once


// CNFButton window

class CNFButton : public CWnd
{
public:
	CNFButton();
	virtual ~CNFButton();

	DWORD		m_dwState;
	bool		m_capture, m_bCheck;
	HBITMAP		m_hBitmap, m_hBitmap2;
	CFont		*m_pFont, m_font;
	CToolTipCtrl	*m_pTipCtrl;
	CImageList	m_ImageList;

public:

public:
	LOGFONT	setFont(int fsize, CString fname, BOOL bBold, BOOL bItalic);
	CString	Parser(CString &srcstr, CString substr);
	bool	GetCheck();
	void	SetCheck(bool bCheck);
	DWORD	GetState();

public:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT	HandleSetFont(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};
