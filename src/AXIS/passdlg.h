#if !defined(AFX_PASSDLG_H__4C798E81_36F0_4A75_9980_CF5E8B91D080__INCLUDED_)
#define AFX_PASSDLG_H__4C798E81_36F0_4A75_9980_CF5E8B91D080__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// passdlg.h : header file
//

class	Caccount
{
public:
	CString	m_name;
	CString	m_account;
	CString	m_pass;
};
/////////////////////////////////////////////////////////////////////////////
// CPassDlg dialog
#include "useMaindefine.h"
class CPassDlg : public CDialog
{
// Construction
public:
	CPassDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPassDlg();

// Dialog Data
	//{{AFX_DATA(CPassDlg)
	enum { IDD = IDD_SAVEPASS };
	CListCtrl	m_list;
	//}}AFX_DATA
	CArray	< Caccount *, Caccount * > m_acs;

protected:
	int	m_select;

public:
	void	SetData(CString data);

protected:
	void	fit();
	bool	setAccountData(int sel);
	void	setStatus(CString status);
	CString	editAccount(CString account);

	BOOL	addColumn(LPCTSTR columns, int col, int width);
	BOOL	addItem(CString items, int item, int subi, bool press = false);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPassDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPassDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSave();
	afx_msg void OnListClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUp();
	afx_msg void OnDown();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
#ifdef DF_USE_CPLUS17
	std::unique_ptr<CControlChanger> m_changer;
#else
	CControlChanger* m_changer{};
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSDLG_H__4C798E81_36F0_4A75_9980_CF5E8B91D080__INCLUDED_)
