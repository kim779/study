#if !defined(AFX_SCREENDLG_H__DC2100CE_9B14_41E4_8B06_1B585B7AEB72__INCLUDED_)
#define AFX_SCREENDLG_H__DC2100CE_9B14_41E4_8B06_1B585B7AEB72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// screenDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreenDlg dialog

class CScreenDlg : public CDialog
{
// Construction
public:
	CScreenDlg(CWnd* pParent, CString root);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScreenDlg)
	enum { IDD = IDD_SCREEN };
	CListCtrl	m_list;
	//}}AFX_DATA
	CWnd*	m_parent;
	CString	m_root;
	bool	m_add;

	CString	m_user;
	CString	m_colName;
	int	m_select;
	bool	m_changed;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	SaveOrder();
	void	fillList();
	BOOL	addColumn(LPCTSTR columns, int col, int width);
	BOOL	addItem(CString items, int item, int subi);
	// Generated message map functions
	//{{AFX_MSG(CScreenDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnADD();
	afx_msg void OnDelete();
	virtual void OnOK();
	afx_msg void OnClickAlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUp();
	afx_msg void OnDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREENDLG_H__DC2100CE_9B14_41E4_8B06_1B585B7AEB72__INCLUDED_)
