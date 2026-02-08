#if !defined(AFX_TITLETIP_H__58D72159_6F44_499F_B255_058F2B53458F__INCLUDED_)
#define AFX_TITLETIP_H__58D72159_6F44_499F_B255_058F2B53458F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TitleTip.h : header file
//

#define TITLETIP_CLASSNAME _T("ZTitleTip")

/////////////////////////////////////////////////////////////////////////////
// CTitleTip window

class CTitleTip : public CWnd
{
// Construction
public:
	CTitleTip();
	virtual ~CTitleTip();
	virtual BOOL Create(CWnd *parentWnd);

// Attributes
public:

// Operations
public:
	void Show(CRect rectTitle, LPCTSTR lpszTitleText, 
			int xoffset = 0, LPRECT lpHoverRect = NULL, LPLOGFONT lpLogFont = NULL);
	void Hide();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTitleTip)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:

protected:
	CWnd*	m_parentWnd{};
	CRect	m_rectTitle;
	CRect	m_rectHover;

	// Generated message map functions
protected:
	//{{AFX_MSG(CTitleTip)
	/*  //vc2019
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	*/
	 void OnMouseMove(UINT nFlags, CPoint point);
	 void OnLButtonDown(UINT nFlags, CPoint point);
	 void OnLButtonUp(UINT nFlags, CPoint point);
	 void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TITLETIP_H__58D72159_6F44_499F_B255_058F2B53458F__INCLUDED_)
