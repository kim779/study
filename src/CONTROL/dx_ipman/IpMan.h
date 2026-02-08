#pragma once
// IpMan.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIpMan command target

class CIpMan : public CCmdTarget
{
	DECLARE_DYNCREATE(CIpMan)

	CIpMan();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIpMan)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CIpMan();

	// Generated message map functions
	//{{AFX_MSG(CIpMan)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CIpMan)
	afx_msg BSTR GetLocalIP();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};