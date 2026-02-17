#if !defined(AFX_XSYSTEM_H__E45252DC_943C_4CBB_B652_731B037D652F__INCLUDED_)
#define AFX_XSYSTEM_H__E45252DC_943C_4CBB_B652_731B037D652F__INCLUDED_

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
	BOOL	m_userKey;

	int	m_certL;
	CString	m_certB;

protected:
	CGuard*	m_guard;

// Operations
public:
	int	ConvertHEX(char* datB, int datL);

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
	afx_msg BSTR _getSound();
	afx_msg BOOL _getUserKey();
	afx_msg void _setUserKey(BOOL bNewValue);
	afx_msg BSTR _getSystemID();
	afx_msg BSTR _getUserPass();
	afx_msg BSTR _getCertifyPass();
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
	afx_msg BOOL _PlaySound(LPCTSTR soundFile);
	afx_msg BOOL _CheckPasswd(LPCTSTR passwd);
	afx_msg BSTR _Encrypt(LPCTSTR data, LPCTSTR keys);
	afx_msg BSTR _Decrypt(LPCTSTR data, LPCTSTR keys);
	afx_msg BSTR _EncryptSHA(LPCTSTR data);
	afx_msg long CertifyFull(LPCTSTR srcB, long srcL, VARIANT FAR* desB);
	afx_msg BSTR _GetUserEncID();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XSYSTEM_H__E45252DC_943C_4CBB_B652_731B037D652F__INCLUDED_)
