#if !defined(AFX_XSCREEN_H__111F6C01_25F9_46C3_8478_8E76BFAD455B__INCLUDED_)
#define AFX_XSCREEN_H__111F6C01_25F9_46C3_8478_8E76BFAD455B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// xscreen.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CxScreen command target

class CxScreen : public CCmdTarget
{
	DECLARE_DYNCREATE(CxScreen)

public:
	CxScreen()	{}           // protected constructor used by dynamic creation
	CxScreen(class CScreen* screen);
	virtual ~CxScreen();

// Attributes
public:
	BOOL		m_return;
	CString		m_multiUse;

protected:
	CScreen*	m_screen;
	class	CGuard*	m_guard;

	char*		m_pBytes;
	int		m_nBytes;
	enum	{svREADY, svWAIT, svTIMEOUT, svDONE}	m_service;

// Operations
public:
	bool	OnService(char* pBytes, int nBytes);
	void	LuaSend(long target, CString strTR);

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CxScreen)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	_Send(long target, CString strTR);

	// Generated message map functions
	//{{AFX_MSG(CxScreen)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CxScreen)
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XSCREEN_H__111F6C01_25F9_46C3_8478_8E76BFAD455B__INCLUDED_)
