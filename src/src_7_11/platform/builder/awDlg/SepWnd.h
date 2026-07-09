#if !defined(AFX_SEPWND_H__5E99045A_0822_4A99_8E75_6CAC120036D9__INCLUDED_)
#define AFX_SEPWND_H__5E99045A_0822_4A99_8E75_6CAC120036D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SepWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSepWnd window

class AFX_EXT_CLASS CSepWnd : public CWnd
{
// Construction
public:
	CSepWnd();

// Attributes
public:
	CRgn	m_wndRgn;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSepWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	void MoveSWWnd(int xpos, int ypos);
	void SetSWTP();
	void MoveWnd(int xpos, int ypos);
	void SetTP();
	virtual ~CSepWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSepWnd)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEPWND_H__5E99045A_0822_4A99_8E75_6CAC120036D9__INCLUDED_)
