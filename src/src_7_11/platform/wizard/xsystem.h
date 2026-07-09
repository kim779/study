#if !defined(AFX_XSYSTEM_H__8ACB2E08_6C37_4E14_8E4C_DC74C540E280__INCLUDED_)
#define AFX_XSYSTEM_H__8ACB2E08_6C37_4E14_8E4C_DC74C540E280__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// xsystem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CxSystem command target

class CxSystem : public CCmdTarget
{
	DECLARE_DYNCREATE(CxSystem)

public:
	CxSystem()	{}           // protected constructor used by dynamic creation
	CxSystem(class CGuard* guard);
	virtual ~CxSystem();

// Attributes
public:
	CString	m_host;

protected:
	CGuard*	m_guard;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CxSystem)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CxSystem)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CxSystem)
	afx_msg BOOL _getTriggerOn();
	afx_msg long _getConnectBy();
	afx_msg BSTR _getUserID();
	afx_msg BSTR _getTermID();
	afx_msg BOOL _getOrderConfirm();
	afx_msg BOOL _getOrderPopup();
	afx_msg BSTR _getHome();
	afx_msg BSTR _getHost();
	afx_msg BSTR _getSystemID();
	afx_msg BSTR _getOtp();
	afx_msg BSTR _Convert(long type, LPCTSTR src, LPCTSTR file);
	afx_msg BOOL _IsPopup(LPCTSTR mapN);
	afx_msg void _Help(LPCTSTR file);
	afx_msg void _Navigate(LPCTSTR url);
	afx_msg void _Trigger(LPCTSTR procs, LPCTSTR params, LPCTSTR maps);
	afx_msg BSTR _GetAccountName(LPCTSTR account);
	afx_msg BSTR _GetAccountPass(LPCTSTR account);
	afx_msg BSTR _GetHistory(BOOL historical, LPCTSTR symbol);
	afx_msg void _SetHistory(LPCTSTR symbol, LPCTSTR code);
	afx_msg void _Trace(LPCTSTR str);
	afx_msg void _Push(LPCTSTR name, LPCTSTR data);
	afx_msg BSTR _Pop(LPCTSTR name);
	afx_msg BOOL _Open(LPCTSTR file);
	afx_msg void _SetAutoCode(LPCTSTR data, long count);
	afx_msg BOOL _ExcelToText(LPCTSTR files);
	afx_msg BOOL _TextToExcel(LPCTSTR files);
	afx_msg void _Exit(BOOL reboot);
	afx_msg BOOL _Menu(LPCTSTR menus);
	afx_msg BOOL _Print(LPCTSTR data, LPCTSTR fName, long fPoint);
	afx_msg BOOL _PlaySound(LPCTSTR fileN);
	afx_msg BOOL _CheckPasswd(LPCTSTR passwd);
	afx_msg void _SetPasswd(LPCTSTR passwd);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XSYSTEM_H__8ACB2E08_6C37_4E14_8E4C_DC74C540E280__INCLUDED_)
