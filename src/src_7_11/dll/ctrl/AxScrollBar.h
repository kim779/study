#if !defined(AFX_AXSCROLLBAR_H__FFF072E9_8771_11D5_A3CB_004F4E0844B2__INCLUDED_)
#define AFX_AXSCROLLBAR_H__FFF072E9_8771_11D5_A3CB_004F4E0844B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxScrollBar.h : header file
//
#define	WM_AXIS_VSCROLL		WM_USER + 100
/////////////////////////////////////////////////////////////////////////////
// CAxScrollBar window

class CAxScrollBar : public CScrollBar
{
// Construction
public:
	CAxScrollBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxScrollBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual ~CAxScrollBar();

	// Generated message map functions
protected:
	void	LineUp();
	void	LineDown();
	void	PageUp();
	void	PageDown();
	void	ThumbTrack(UINT nPos);

	//{{AFX_MSG(CAxScrollBar)
	afx_msg void VScroll(UINT nSBCode, UINT nPos);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXSCROLLBAR_H__FFF072E9_8771_11D5_A3CB_004F4E0844B2__INCLUDED_)
