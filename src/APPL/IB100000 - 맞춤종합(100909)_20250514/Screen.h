#pragma once
// Screen.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen window

class CScreen : public CWnd
{
// Construction
public:
	CScreen();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CScreen();

	// Generated message map functions
protected:
	//{{AFX_MSG(CScreen)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

