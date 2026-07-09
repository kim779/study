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

	CLedger(void* data = NULL);           // protected constructor used by dynamic creation

// Attributes
protected:
	char*	m_ledger;
	CString	m_text;
	int		m_iInputlen;
// Operations
public:
	CString	GetLedger(int pos, int length,int inputhlen = 0);
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
	virtual ~CLedger();

	// Generated message map functions
	//{{AFX_MSG(CLedger)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CLedger)
	afx_msg BSTR _getanam();
	afx_msg void _setanam(LPCTSTR lpszNewValue);
	afx_msg BSTR _getsnam();
	afx_msg void _setsnam(LPCTSTR lpszNewValue);
	afx_msg BSTR _getfnam();
	afx_msg void _setfnam(LPCTSTR lpszNewValue);
	afx_msg BSTR _getfncd();
	afx_msg void _setfncd(LPCTSTR lpszNewValue);
	afx_msg BSTR _getguid();
	afx_msg void _setguid(LPCTSTR lpszNewValue);
	afx_msg BSTR _getctyp();
	afx_msg void _setctyp(LPCTSTR lpszNewValue);
	afx_msg BSTR _geteflg();
	afx_msg void _seteflg(LPCTSTR lpszNewValue);
	afx_msg BSTR _gettflg();
	afx_msg void _settflg(LPCTSTR lpszNewValue);
	afx_msg BSTR _gettdat();
	afx_msg void _settdat(LPCTSTR lpszNewValue);
	afx_msg BSTR _Getttim();
	afx_msg void _Setttim(LPCTSTR lpszNewValue);
	afx_msg BSTR _Getclip();
	afx_msg void _Setclip(LPCTSTR lpszNewValue);
	afx_msg BSTR _Getrtyp();
	afx_msg void _Setrtyp(LPCTSTR lpszNewValue);
	afx_msg BSTR _Getrcod();
	afx_msg void _Setrcod(LPCTSTR lpszNewValue);
	afx_msg BSTR _Getrlcd();
	afx_msg void _Setrlcd(LPCTSTR lpszNewValue);
	afx_msg BSTR _Getrtit();
	afx_msg void _Setrtit(LPCTSTR lpszNewValue);
	afx_msg BSTR _Getrbmg();
	afx_msg void _Setrbmg(LPCTSTR lpszNewValue);
	afx_msg BSTR _Getrdmg();
	afx_msg void _Setrdmg(LPCTSTR lpszNewValue);
	afx_msg BSTR _Getusid();
	afx_msg void _Setusid(LPCTSTR lpszNewValue);
	afx_msg BSTR _Getcrst();
	afx_msg void _Setcrst(LPCTSTR lpszNewValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
	BSTR Getfill(void);
	void Setfill(LPCTSTR newVal);
	void SetLedgerLen();
	enum 
	{
		dispidmenu = 29,
		dispidsusid = 28,
		dispidsfiller = 27,
		dispidagnc = 26,
		dispidsale = 25,
		dispidbrnc = 24,
		dispiddept = 23,
		dispidtmps = 22,
		dispidtmno = 21,
		dispidfill = 20
	};
	BSTR Gettmno(void);
	void Settmno(LPCTSTR newVal);
	BSTR Gettmps(void);
	void Settmps(LPCTSTR newVal);
	BSTR Getdept(void);
	void Setdept(LPCTSTR newVal);
	BSTR Getbrnc(void);
	void Setbrnc(LPCTSTR newVal);
	BSTR Getsale(void);
	void Setsale(LPCTSTR newVal);
	BSTR Getagnc(void);
	void Setagnc(LPCTSTR newVal);
	BSTR Getsfiller(void);
	void Setsfiller(LPCTSTR newVal);
	BSTR Getsusid(void);
	void Setsusid(LPCTSTR newVal);
	BSTR Getmenu(void);
	void Setmenu(LPCTSTR newVal);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEDGERX_H__4E5AE5B4_B3E1_4281_B082_162F1083BC6F__INCLUDED_)
