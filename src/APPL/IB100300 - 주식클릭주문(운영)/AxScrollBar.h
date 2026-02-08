#pragma once
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

