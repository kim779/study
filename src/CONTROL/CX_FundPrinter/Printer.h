#if !defined(AFX_PRINTER_H__F9D147D0_54F0_4C6D_86A9_A674CE25A5B8__INCLUDED_)
#define AFX_PRINTER_H__F9D147D0_54F0_4C6D_86A9_A674CE25A5B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Printer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrinter window

class CPrinter : public CWnd
{
// Construction
public:
	CPrinter();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrinter)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPrinter();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPrinter)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CPrinter)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTER_H__F9D147D0_54F0_4C6D_86A9_A674CE25A5B8__INCLUDED_)
