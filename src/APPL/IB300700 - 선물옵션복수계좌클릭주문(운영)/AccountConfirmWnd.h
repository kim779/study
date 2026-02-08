
#pragma once
// AccountConfirmWnd.h : header file
//
#include "AxisExt.h"
#include "afxtempl.h"
#include "Common.h"

/////////////////////////////////////////////////////////////////////////////
// CAccountConfirmWnd dialog
class CAccountInfo
{
public:
	CAccountInfo(){}
	~CAccountInfo(){}
public:
	CString m_Account;
	CString m_AcctName;

};

class CAccountConfirmWnd : public CDialog, public CAxisExt
{
// Construction
public:
	CAccountConfirmWnd(CWnd* pParent = NULL,AccountMap *pAccount = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAccountConfirmWnd)
	enum { IDD = IDD_ACCTCONFIRM_DLG };
	CListCtrl	m_listNOwn;
	CListCtrl	m_listOwn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccountConfirmWnd)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void ParseAccntInfo(CString data);
	CString SplitString(CString &strData, CString strToken);
public:
	AccountMap *m_pAccount;
	CMapStringToString m_mapAccnt;
	CArray <CAccountInfo,CAccountInfo> m_accntInfo;
protected:

	// Generated message map functions
	//{{AFX_MSG(CAccountConfirmWnd)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

