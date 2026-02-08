#pragma once
// MainWnd.h : header file
//

#include <wrl.h>
#include <wil/com.h>
#include "WebView2.h"

using namespace Microsoft::WRL;

class CParam
{
public:
	int		key = 0;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point = 0;		// font point
	int		style = 0;		// font style
	DWORD		tRGB = 0;			// text color index
	DWORD		pRGB = 0;			// paint color index
	CString		options;		// properties
};

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd();
	virtual ~CMainWnd();

	void	SetParam(struct _param* pParam);
	void    InitEdge();
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	CWnd* m_pParent{};
	CParam	  m_Param;



private:
	CString _sURL;
	bool	_bScroll{};

	wil::com_ptr<ICoreWebView2Controller> _edgeControl;
	wil::com_ptr<ICoreWebView2>	      _edgeWnd;
	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	enum
	{
		dispidcloseEdge = 3L,
		dispidNavigatesrc = 2L,
		dispidNavigate = 1L
	};
	void Navigate(BSTR url);
	void Navigatesrc(BSTR src);
	void closeEdge();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

