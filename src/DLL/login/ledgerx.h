#if !defined(AFX_LEDGERX_H__25AC7C24_9FE7_4A03_8394_E490DFCAF3B7__INCLUDED_)
#define AFX_LEDGERX_H__25AC7C24_9FE7_4A03_8394_E490DFCAF3B7__INCLUDED_

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

	CLedger(void* data = NULL);           // protected constructor used by dynamic creation
	virtual ~CLedger();

// Attributes
public:protected:
	char*	m_ledger;
	CString	m_text;

// Operations
public:
	void	LedgerEx(void* data);
	CString	GetLedger(int pos, int length);
	CString	GetLedgerEx(int id, void* data);
	void	SetLedger(int pos, int length, void* data);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLedger)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	parseParam(void* param);

	// Generated message map functions
	//{{AFX_MSG(CLedger)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CLedger)
	afx_msg BSTR _getType();
	afx_msg BSTR _getSvcd();
	afx_msg void _setSvcd(LPCTSTR lpszNewValue);
	afx_msg BSTR _getCono();
	afx_msg BSTR _getUsid();
	afx_msg BSTR _getCogb();
	afx_msg BSTR _getPuip();
	afx_msg BSTR _getPcip();
	afx_msg BSTR _getDept();
	afx_msg BSTR _getBrno();
	afx_msg void _setBrno(LPCTSTR lpszNewValue);
	afx_msg BSTR _getLang();
	afx_msg BSTR _getTime();
	afx_msg BSTR _getEcod();
	afx_msg BSTR _getEknd();
	afx_msg BSTR _getFkey();
	afx_msg void _setFkey(LPCTSTR lpszNewValue);
	afx_msg BSTR _getRcnt();
	afx_msg void _setRcnt(LPCTSTR lpszNewValue);
	afx_msg BSTR _getNext();
	afx_msg void _setNext(LPCTSTR lpszNewValue);
	afx_msg BSTR _getNkey();
	afx_msg void _setNkey(LPCTSTR lpszNewValue);
	afx_msg BSTR _getOdrf();
	afx_msg void _setOdrf(LPCTSTR lpszNewValue);
	afx_msg BSTR _getMkty();
	afx_msg void _setMkty(LPCTSTR lpszNewValue);
	afx_msg BSTR _getSvrm();
	afx_msg BSTR _getEact();
	afx_msg BSTR _getEmsg();
	afx_msg BSTR _getSkip();
	afx_msg void _setSkip(LPCTSTR lpszNewValue);
	afx_msg BSTR _getTerm();
	afx_msg BSTR _getHsiz();
	afx_msg void _setHsiz(LPCTSTR lpszNewValue);
	afx_msg BSTR _getEpwd();
	afx_msg void _setEpwd(LPCTSTR lpszNewValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEDGERX_H__25AC7C24_9FE7_4A03_8394_E490DFCAF3B7__INCLUDED_)
