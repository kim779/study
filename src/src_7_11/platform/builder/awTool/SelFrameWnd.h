#if !defined(AFX_SELFRAMEWND_H__1653B309_E1A4_4511_ACF7_36B2F6DA2D1D__INCLUDED_)
#define AFX_SELFRAMEWND_H__1653B309_E1A4_4511_ACF7_36B2F6DA2D1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelFrameWnd.h : header file
//
#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CSelFrameWnd window

class CSelFrameWnd : public CWnd
{
// Construction
public:
	CSelFrameWnd();

// Attributes
public:
	CRgn	m_wndRgn;
	CArray	<CRect, CRect> m_rectArr;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelFrameWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	void AddRect(CRect rc);
	void SetTP();
	virtual ~CSelFrameWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSelFrameWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELFRAMEWND_H__1653B309_E1A4_4511_ACF7_36B2F6DA2D1D__INCLUDED_)
