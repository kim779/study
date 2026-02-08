#pragma once
// Mainwnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainwnd window

class CMainwnd : public CWnd
{
// Construction
public:
	CMainwnd();
	CString	Variant(int nComm, CString strData /* = _T("") */);
	bool	sendTR(CString trN, char* datB, int datL, BYTE stat, int key);
	void	setParam(const struct _param* pParam);
	LPCTSTR	Variant( int cmd, LPCTSTR data );

// Attributes
public:
	CWnd* m_pWizard;

protected:
	_param	m_param;
	CWnd*	m_pParentWnd;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainwnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainwnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainwnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainwnd)
	afx_msg void send4013(LPCTSTR sdata);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};