#if !defined(AFX_AXMTOOLTIP_H__330540D0_9F19_4F57_B83B_5F8E4C7F589C__INCLUDED_)
#define AFX_AXMTOOLTIP_H__330540D0_9F19_4F57_B83B_5F8E4C7F589C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxMToolTip.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAxMToolTip window

class CAxMToolTip : public CWnd
{
// Construction
public:
	CAxMToolTip();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxMToolTip)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAxMToolTip();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAxMToolTip)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CFont m_font;
	COLORREF m_crBack;
	CString m_text;
	void DrawTip(CDC* dc);
public:
	int Create();
	CSize SetText(LPCTSTR text);
	void WindowMove(int x, int y, int cx, int cy);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXMTOOLTIP_H__330540D0_9F19_4F57_B83B_5F8E4C7F589C__INCLUDED_)
