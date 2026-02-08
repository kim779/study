#if !defined(AFX_SSTATIC_H__BAD7C915_88A3_47F7_A6A6_6461B2C413DF__INCLUDED_)
#define AFX_SSTATIC_H__BAD7C915_88A3_47F7_A6A6_6461B2C413DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
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

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSTATIC_H__BAD7C915_88A3_47F7_A6A6_6461B2C413DF__INCLUDED_)
