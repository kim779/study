#if !defined(AFX_FPRINTER_H__F7E40005_B675_4882_857E_F72EABCE8702__INCLUDED_)
#define AFX_FPRINTER_H__F7E40005_B675_4882_857E_F72EABCE8702__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FPrinter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFPrinter window

class CFPrinter : public CWnd
{
// Construction
public:
	CFPrinter();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFPrinter)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFPrinter();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFPrinter)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CFPrinter)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FPRINTER_H__F7E40005_B675_4882_857E_F72EABCE8702__INCLUDED_)
