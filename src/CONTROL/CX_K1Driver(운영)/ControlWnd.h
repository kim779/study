#if !defined(AFX_CONTROLWND_H__5E5B31B4_9BD1_43F8_BBE0_85F953AC0802__INCLUDED_)
#define AFX_CONTROLWND_H__5E5B31B4_9BD1_43F8_BBE0_85F953AC0802__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ControlWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd();
	CControlWnd(CWnd *parent);


// Attributes
public:
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CControlWnd();

private:
	CWnd* m_pWizard;

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	afx_msg void start(short nindex);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTROLWND_H__5E5B31B4_9BD1_43F8_BBE0_85F953AC0802__INCLUDED_)
