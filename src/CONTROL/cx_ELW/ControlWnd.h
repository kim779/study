#pragma once
// ControlWnd.h : header file
//

#include "afxtempl.h"
#include "../../h/jmcode.h"

class CParam
{
public:
	int		key=0;			// screen key
	CString		name;			// control CX_Symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point=0;		// font point
	int		style=0;		// font style
	DWORD		tRGB=0;			// text color index
	DWORD		pRGB=0;			// paint color index
	CString		options;		// properties
};

class _xelw
{
public:
	CString	code;
	CString	hnam;
	CString	enam;
	CString	symb;
	CString	lphn;
	CString	lpse;
	CString	hbge;
	CString	ebge;
	CString	bgce;
	CString	gcjs;
	CString	grat;
	CString	gcge;
	CString	hsge;
	CString	krye;
	CString	khbe;
	CString	mjmt;
};
/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd();
	virtual ~CControlWnd();

// Attributes
public:
	CWnd*	m_parent;
	CParam	m_param;

	CString	m_root;
	CString	m_name;
	CArray	<_xelw, _xelw> m_xelws;

// Operations
public:
	void	SetParam(struct _param *param);
	bool	loadingELW();

	CString	parseX(CString &srcstr, CString substr);
	CString Variant(int comm, CString data);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)

public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	afx_msg BSTR _GetName(LPCTSTR code);
	afx_msg BSTR _GetBgce(LPCTSTR code);
	afx_msg BSTR _GetGcjs(LPCTSTR code);
	afx_msg BSTR _GetHbge(LPCTSTR code);
	afx_msg BOOL _IsElwCode(LPCTSTR code);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};