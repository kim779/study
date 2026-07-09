#if !defined(AFX_NFBTN_H__71AB1CE5_2357_41B0_9AE3_513B0EF144D1__INCLUDED_)
#define AFX_NFBTN_H__71AB1CE5_2357_41B0_9AE3_513B0EF144D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NFBtn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNFBtn window

class CNFBtn : public CWnd
{
// Construction
public:
	CNFBtn();

// Attributes
public:
	DWORD		m_dwState;
	bool		m_capture, m_bCheck;
	HBITMAP		m_hBitmap, m_hBitmap2;
	CFont		*m_pFont, m_font;
	CToolTipCtrl	*m_pTipCtrl;
	CImageList	m_ImageList;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNFBtn)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	LOGFONT	setFont(int fsize, CString fname, BOOL bBold, BOOL bItalic);
	CString	Parser(CString &srcstr, CString substr);
	bool	GetCheck();
	void	SetCheck(bool bCheck);
	DWORD	GetState();
	virtual ~CNFBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNFBtn)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT	HandleSetFont(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NFBTN_H__71AB1CE5_2357_41B0_9AE3_513B0EF144D1__INCLUDED_)
