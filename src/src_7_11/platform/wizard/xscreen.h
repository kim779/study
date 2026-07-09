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

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CxScreen)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL	dialogAction(bool isKOSPI, CString data, CString code, CString name);
	bool	findForm(int parent, CString name, class CfmBase*& form);

	// Generated message map functions
	//{{AFX_MSG(CxScreen)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CxScreen)
	afx_msg long _getKey();
	afx_msg BOOL _getOnSend();
	afx_msg void _setOnSend(BOOL bNewValue);
	afx_msg BOOL _getWait();
	afx_msg void _setWait(BOOL bNewValue);
	afx_msg long _getMajor();
	afx_msg long _getMinor();
	afx_msg BOOL _getCertify();
	afx_msg void _setCertify(BOOL bNewValue);
	afx_msg void _setTrigger(BOOL bNewValue);
	afx_msg void _ClearAll(long dest, long type);
	afx_msg void _ChangeTR(LPCTSTR trN);
	afx_msg void _ChangePath(LPCTSTR path);
	afx_msg void _SetLedger(long pos, long length, LPCTSTR data);
	afx_msg BSTR _GetLedger(long pos, long length);
	afx_msg void _ChangeMap(LPCTSTR mapN);
	afx_msg void _SetMessage(LPCTSTR msg);
	afx_msg void _SetMessage2(long level, LPCTSTR msg);
	afx_msg void _CreateWindow(long type, LPCTSTR param, LPCTSTR data);
	afx_msg void _Send(long target);
	afx_msg void _RSend(long target);
	afx_msg void _CloseWindow();
	afx_msg BSTR _GetMapName(long target);
	afx_msg void _IgnoreChange(BOOL ignore);
	afx_msg LPDISPATCH _Object(LPCTSTR forms);
	afx_msg BOOL _GetCode(long kind, LPCTSTR codeSymbol, LPCTSTR nameSymbol, long type);
	afx_msg BSTR _GetName(long kind, LPCTSTR code, long type);
	afx_msg BOOL _Service(LPCTSTR trN, LPCTSTR data, long length, long mode);
	afx_msg void _Proc(LPCTSTR procs, LPCTSTR data);
	afx_msg void _SetCaption(LPCTSTR caption);
	afx_msg BOOL _Approve(long key, LPCTSTR ip, LPCTSTR map, LPCTSTR data, long length);
	afx_msg BSTR _PopupMenu(LPCTSTR menus, LPCTSTR symbol);
	afx_msg void _SetFont(long point, LPCTSTR name);
	afx_msg BOOL _CreateObject(LPCTSTR name);
	afx_msg void _SetSize(long width, long height);
	afx_msg void _Print(LPCTSTR text);
	afx_msg void _SetTimer(long interval, BOOL main);
	afx_msg long _GetCodeType(LPCTSTR code);
	afx_msg long _UploadFile(LPCTSTR trN, LPCTSTR data, long length, long mode, LPCTSTR fileN, long offset);
	afx_msg BOOL _DownloadFile(LPCTSTR trN, LPCTSTR data, long length, long mode, LPCTSTR fileN);
	afx_msg BOOL _IsCodeType(LPCTSTR code, long type);
	afx_msg long _GetOrderType(LPCTSTR code);
	afx_msg BSTR _GetCaption();
	afx_msg BSTR _ServiceEx(LPCTSTR trN, LPCTSTR data, long length, long mode, long timeout);
	afx_msg void _ProcEx(LPCTSTR procs, LPCTSTR data, long count);
	afx_msg void _ReloadAccn(LPCTSTR name);
	afx_msg void _RegisterRTM(LPCTSTR codes);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XSCREEN_H__111F6C01_25F9_46C3_8478_8E76BFAD455B__INCLUDED_)
