#if !defined(AFX_BUTTON_H__8163AE22_69FB_496C_86EE_836EEE0AC652__INCLUDED_)
#define AFX_BUTTON_H__8163AE22_69FB_496C_86EE_836EEE0AC652__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// button.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Cbutton window

class Cbutton : public CButton
{
// Construction
public:
	Cbutton();

// Attributes
protected:
	CRect	m_rect;
	CString	m_text;
	HBITMAP	m_bitmap;
	HBITMAP	m_bitmap_;
	HBITMAP	m_bitmap__;

	bool	m_down;
	bool	m_enable;
	bool	m_bsubClass;
	bool	m_toggle;

	UINT	m_align;
	COLORREF	m_tcolor;
	COLORREF	m_EBcolor;
	bool	m_bHover;

// Operations
public:
	BOOL	Attach(const UINT nID, CWnd* pParent, CRect rect, CString file,
				CString file_, CString file__, CString text = _T(""));

	BOOL	Attach(const UINT nID, CWnd* pParent, CRect rect, UINT cool,
				UINT down = 0, UINT disable = 0, CString text = _T(""));
	CString	GetText() { return m_text; }
	void	SetText(CString text, bool draw = true);
	void	SetTextColor(COLORREF color) { m_tcolor = color; }
	void	SetAlign(UINT align) { m_align = align; }
	void	SetState(bool down = false);
	void	SetEnable(bool enable);
	bool	IsEnable()		{ return m_enable;   }
	void	SetToggle()	{ m_toggle = true; }
	void	SetEBcolor(COLORREF color) { m_EBcolor = color; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Cbutton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~Cbutton();

	// Generated message map functions
protected:
	void	drawBitmap(CDC* dc, CRect rect);
	//{{AFX_MSG(Cbutton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUTTON_H__8163AE22_69FB_496C_86EE_836EEE0AC652__INCLUDED_)
