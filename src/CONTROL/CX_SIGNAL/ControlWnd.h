#if !defined(AFX_CONTROLWND_H__9AFC10B9_2592_44B9_A4FE_BC8320017346__INCLUDED_)
#define AFX_CONTROLWND_H__9AFC10B9_2592_44B9_A4FE_BC8320017346__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ControlWnd.h : header file
//
#define WM_GET_BROD		WM_USER + 703

#include <afxmt.h>

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

/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

class CControlWnd : public CWnd
{

public:
	CControlWnd();
	virtual ~CControlWnd();

public:
	CWnd*		m_pParent;
	CParam		m_Param;
	HMODULE		m_hSignalMng;
	CWnd*		m_pWndDummy;

private:

public:
	void	SetParam(struct _param *pParam);

private:

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CControlWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTROLWND_H__9AFC10B9_2592_44B9_A4FE_BC8320017346__INCLUDED_)
