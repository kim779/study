#if !defined(AFX_LOGIN_H__D48B9564_5019_479B_8851_69B505F80976__INCLUDED_)
#define AFX_LOGIN_H__D48B9564_5019_479B_8851_69B505F80976__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// login.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogin command target

class CLogin : public CCmdTarget
{
	DECLARE_DYNCREATE(CLogin)

	CLogin();           // protected constructor used by dynamic creation
	virtual ~CLogin();

// Attributes
public:
	CString	m_user;
	CString	m_name;
	CString	m_term;

	CString m_commKind;
	CString m_media;
	CString	m_dept;
	CString	m_deptName;
	CString	m_svrNo;
	CString	m_method;
	CString	m_idNo;
	CString	m_grade;
	CString	m_date;
	CString	m_today;
	CString	m_lmedia;
	CString	m_ldate;
	CString	m_ltime;
	CString	m_check;
	CString	m_popup;

// Operations
public:
	void	SetLogin(char* data);

	LPCTSTR	GetUsid();
	LPCTSTR	GetUserName();
	LPCTSTR	GetTerm();

	LPCTSTR	GetCommKind();
	LPCTSTR	GetMedia();
	LPCTSTR	GetDept();
	LPCTSTR	GetDeptName();
	LPCTSTR	GetSvrNo();
	LPCTSTR	GetMethod();
	LPCTSTR	GetIdNo();
	LPCTSTR	GetGrade();
	LPCTSTR	GetDate();
	LPCTSTR	GetToday();
	LPCTSTR	GetLmedia();
	LPCTSTR	GetLdate();
	LPCTSTR	GetLtime();
	LPCTSTR	GetCheck();
	LPCTSTR	GetPopup();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogin)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	clear();
	void	setDefault(char* data);
	

	// Generated message map functions
	//{{AFX_MSG(CLogin)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CLogin)
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGIN_H__D48B9564_5019_479B_8851_69B505F80976__INCLUDED_)
