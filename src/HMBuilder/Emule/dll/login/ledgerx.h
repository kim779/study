#if !defined(AFX_LEDGERX_H__4E5AE5B4_B3E1_4281_B082_162F1083BC6F__INCLUDED_)
#define AFX_LEDGERX_H__4E5AE5B4_B3E1_4281_B082_162F1083BC6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ledgerx.h : header file
//

#include "ledger.h"

/////////////////////////////////////////////////////////////////////////////
// CLedger command target

class CLedger : public CCmdTarget
{
	DECLARE_DYNCREATE(CLedger)

	CLedger();           // protected constructor used by dynamic creation

// Attributes
protected:
	char*	m_ledger;

// Operations
public:
	LPCTSTR	GetTime();
	void	SetTime(LPCTSTR lpszNewValue);

	LPCTSTR	GetFlag();
	void	SetFlag(LPCTSTR lpszNewValue);

	LPCTSTR	GetSvcn();
	void	SetSvcn(LPCTSTR lpszNewValue);

	LPCTSTR GetSvid();
	void	SetSvid(LPCTSTR lpszNewValue);

	LPCTSTR GetPgid();
	void	SetPgid(LPCTSTR lpszNewValue);

	LPCTSTR GetUsid();
	LPCTSTR GetDept();
	LPCTSTR GetTerm();
	LPCTSTR GetUsnm();

	LPCTSTR GetTrcd();
	void	SetTrcd(LPCTSTR lpszNewValue);

	LPCTSTR GetScrn();
	void	SetScrn(LPCTSTR lpszNewValue);

	LPCTSTR GetKEnd();
	void	SetKEnd(LPCTSTR lpszNewValue);

	LPCTSTR GetFxcp();
	void	SetFxcp(LPCTSTR lpszNewValue);

	LPCTSTR GetIkey();
	void	SetIkey(LPCTSTR lpszNewValue);

	LPCTSTR GetEofd();
	void	SetEofd(LPCTSTR lpszNewValue);

	LPCTSTR GetReqn();
	void	SetReqn(LPCTSTR lpszNewValue);

	LPCTSTR GetECod();
	void	SetECod(LPCTSTR lpszNewValue);

	LPCTSTR GetEMsg();
	void	SetEMsg(LPCTSTR lpszNewValue);

	LPCTSTR GetAccn();
	void	SetAccn(LPCTSTR lpszNewValue);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLedger)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	parseParam();
	virtual ~CLedger();

	// Generated message map functions
	//{{AFX_MSG(CLedger)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CLedger)
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEDGERX_H__4E5AE5B4_B3E1_4281_B082_162F1083BC6F__INCLUDED_)
