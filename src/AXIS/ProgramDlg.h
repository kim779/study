#if !defined(AFX_PROGRAMDLG_H__FD782225_B83F_4768_8A5D_4E5B0F6429CA__INCLUDED_)
#define AFX_PROGRAMDLG_H__FD782225_B83F_4768_8A5D_4E5B0F6429CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgramDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgramDlg dialog
#include "useMaindefine.h"
class CProgramDlg : public CDialog
{
// Construction
public:
	CProgramDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProgramDlg();

// Dialog Data
	//{{AFX_DATA(CProgramDlg)
	enum { IDD = IDD_PROGRAM };
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgramDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CStringArray	m_arF;

	void	init_List();
	void	fill_List();
	void	fit();
	BOOL	addColumn(LPCTSTR columns, int col, int width);
	BOOL	addItem(CString items, int item, int subi = 0);
	// Generated message map functions
	//{{AFX_MSG(CProgramDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnSearchp();
	afx_msg void OnDblclkPlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString Parser(CString &srcstr, CString substr);

#ifdef DF_USE_CPLUS17
	std::unique_ptr<CControlChanger> m_changer;
#else
	CControlChanger* m_changer{};
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRAMDLG_H__FD782225_B83F_4768_8A5D_4E5B0F6429CA__INCLUDED_)
