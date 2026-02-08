#pragma once
// MainWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd(CWnd* parent);
	virtual ~CMainWnd();

// Attributes
public:
	HINSTANCE m_instance;
	CString	m_PhonePadPath;
	CWnd*	m_pWizard;
	CString Variant(int comm, CString data);

// Operations
protected:
	int	GetState();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)
	afx_msg BSTR Request();
	afx_msg BOOL Load();
	afx_msg void ExitPhonePad();
	afx_msg void StopRequest();
	afx_msg short GetStatus();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	void SendMSgToMain(BSTR strPass);

	enum
	{
		dispidKillProcess = 7L,
		dispidSendMSgToMain = 6L
	};
	void KillProcess(BSTR strProcess);
};