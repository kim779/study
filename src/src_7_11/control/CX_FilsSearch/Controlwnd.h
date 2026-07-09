#if !defined(AFX_CONTROLWND_H__EF28597D_F2E7_4ADF_A840_6B47141678B9__INCLUDED_)
#define AFX_CONTROLWND_H__EF28597D_F2E7_4ADF_A840_6B47141678B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Controlwnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CControlwnd window
class CParam
{
public:
	int		key;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point;			// font point
	int		style;			// font style
	DWORD		tRGB;			// text color index
	DWORD		pRGB;			// paint color index
	CString		options;		// properties
};

class CControlwnd : public CWnd
{
// Construction
public:
	CControlwnd();
	
// Attributes
public:
	CWnd*		m_pParent;
	CParam		m_Param;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlwnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CControlwnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlwnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlwnd)
	afx_msg BSTR GetFilePath();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTROLWND_H__EF28597D_F2E7_4ADF_A840_6B47141678B9__INCLUDED_)
