#pragma once
// MainWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MainWnd window
#include <afxmt.h>

class CParam
{
public:
	int		key=0;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point=0;		// font point
	int		style=0;		// font style
	DWORD		tRGB=0;			// text color index
	DWORD		pRGB=0;			// paint color index
	CString		options;		// properties
};

class MainWnd : public CWnd
{
// Construction
public:
	MainWnd();

// Attributes
public:
	CWnd*		m_pParent;
	CParam		m_param;
	HMODULE		m_hSignalMng;
	CWnd*		m_pWndDummy;
	CStringArray	m_arSymbol;

// Operations
public:
	void	SetParam(struct _param *pParam);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MainWnd)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~MainWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(MainWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam,LPARAM lParam);
	afx_msg long OnGetBroad(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(MainWnd)
	CString m_dataList;
	afx_msg void OnDataListChanged();
	afx_msg void SetSignal(LPCTSTR symbol);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};