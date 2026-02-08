#pragma once
// Execute.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CExecute command target

class CExecute : public CCmdTarget
{
	DECLARE_DYNCREATE(CExecute)

	CExecute();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	void Open(CString AppName, CString Param);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExecute)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CExecute();

	// Generated message map functions
	//{{AFX_MSG(CExecute)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CExecute)
	afx_msg void _Open(LPCTSTR AppName, LPCTSTR Param);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};