#pragma once
// JangoDlg.h : header file
//
#include "./common/accountctrl.h"
#include <afxtempl.h>

#define	TR_DLGREMAINS		151		// 종목잔고조회
#define	TR_DLGREMAINF		152		// 선물잔고조회
#define TR_DLGACCOUNT		255		// 계좌조회
#define	CLRBG		64		// background

struct _Jango
{
	CString	name;
	CString	code;
	CString	price;
	CString	num;
};

/////////////////////////////////////////////////////////////////////////////
// CJangoDlg dialog

class CJangoDlg : public CDialog
{
// Construction
public:
	CJangoDlg(CWnd *pView, CWnd* pParent = NULL);   // standard constructor

	CWnd			*m_pView;
	CWnd			*m_pParent;

	CString			m_sAccn;
	CString			m_sPass;
	char			m_Gubn;
	std::unique_ptr<class CAccountCtrl> m_pAccount;
	CArray <_Jango, _Jango> m_arJango;

	CString		Variant(int cmd, CString data="");
	COLORREF	GetIndexColor(int index);
	CString		Parser(CString &srcstr, CString substr);

	void		sendRemainTR(CString sAccn, CString sPswd);
	CString		ledgerTR(CString sAccn, CString sSvcn, CString sGubn);
	BOOL		sendTR(CString sTR, CString sData, int nKey, int nStat);
	void		parsingRemainData(bool bFuture, char* pData, int len);
	void		SendToAccount(UINT msg, WPARAM wParam, LPARAM lParam);

	BOOL		AddColumn(LPCTSTR string, int item, int width);
	int		AddItem(LPCSTR string, int nItem, int nSubItem);
	CString		editComma(CString sVal);

// Dialog Data
	//{{AFX_DATA(CJangoDlg)
	enum { IDD = IDD_JANGODLG };
	CListCtrl	m_ListJango;
	CStatic	m_AccnName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJangoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJangoDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg long OnAccount(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

