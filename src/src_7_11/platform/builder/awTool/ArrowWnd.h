#if !defined(AFX_ARROWWND_H__23BDCA79_48EC_40B3_A3D4_15E8E055208B__INCLUDED_)
#define AFX_ARROWWND_H__23BDCA79_48EC_40B3_A3D4_15E8E055208B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ArrowWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CArrowWnd window

class CArrowWnd : public CWnd
{
// Construction
public:
	CArrowWnd();

// Attributes
public:
	CRgn	m_wndRgn;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CArrowWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetTP(int type);
	virtual ~CArrowWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CArrowWnd)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ARROWWND_H__23BDCA79_48EC_40B3_A3D4_15E8E055208B__INCLUDED_)
