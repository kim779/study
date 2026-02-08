#pragma once

// SStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSStatic window

class CSStatic : public CStatic
{
// Construction
public:
	CSStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};