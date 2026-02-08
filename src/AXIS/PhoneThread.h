#if !defined(AFX_PHONETHREAD_H__0DAD261E_CFD4_4B44_83B1_39BF839FA51A__INCLUDED_)
#define AFX_PHONETHREAD_H__0DAD261E_CFD4_4B44_83B1_39BF839FA51A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PhoneThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CPhoneThread thread

class CPhoneThread : public CWinThread
{
	DECLARE_DYNCREATE(CPhoneThread)
protected:
	CPhoneThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhoneThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPhoneThread();

	// Generated message map functions
	//{{AFX_MSG(CPhoneThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHONETHREAD_H__0DAD261E_CFD4_4B44_83B1_39BF839FA51A__INCLUDED_)
