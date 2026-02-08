#pragma once
// SelJumunDlg.h : header file
//

#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CSelJumunDlg dialog

class CSelJumunDlg : public CDialog
{
// Construction
public:
	CSelJumunDlg(CWnd* pParent = NULL);   // standard constructor

	void	Init(CPoint pos);
	void	SetList(CString juno, CString vol);
	CString	GetJuno() { return m_junos; }
	int	GetJumunCount() { return m_cnt; }
	CString GetJunoString();

// Dialog Data
	//{{AFX_DATA(CSelJumunDlg)
	enum { IDD = IDD_SELJUMUNDLG };
	CListCtrl	m_listJumun;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelJumunDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd*		m_pParent;
	CStringArray	m_vol;
	CStringArray	m_juno;
	CPoint		m_ptStart;
	CString		m_junos;
	int		m_cnt;

	// Generated message map functions
	//{{AFX_MSG(CSelJumunDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPaint();
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

