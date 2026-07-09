#if !defined(AFX_BONDDLG_H__5940AE5F_F33E_49BA_BF78_7454388C77DA__INCLUDED_)
#define AFX_BONDDLG_H__5940AE5F_F33E_49BA_BF78_7454388C77DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BondDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBondDlg dialog

static BOOL m_gsort;

class CBondDlg : public CDialog
{
// Construction
public:
	void	Set_KS_HanMode();
	void	SetDlgPos(int x, int y);
	BOOL	FindCode(CString szCode);

	CBondDlg(CPoint pt, CWnd* pParent = NULL);   // standard constructor
	void	AddItem(int row, CString szCode, CString szName);
	void	AddRow(int sort);
	CString	GetCode(){ return m_szCode; }
	CString GetName(){ return m_szName; }
	void	SetName(CString szName){ m_szName = szName; }

// Dialog Data
	//{{AFX_DATA(CBondDlg)
	enum { IDD = IDD_DIALOG_BOND };
	CSkinListCtrl	m_ListCtrl;
	int		m_nRadio;
	CString	m_szEditSearch;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBondDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	CFont				m_Font;
	CPoint				m_Point;
	CString				m_szCode;
	CString				m_szName;
	HBRUSH				m_hBrush;
	// Generated message map functions
	//{{AFX_MSG(CBondDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio();
	afx_msg void OnDestroy();
	afx_msg void OnDblclkListBond(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListBond(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditSearch();
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BONDDLG_H__5940AE5F_F33E_49BA_BF78_7454388C77DA__INCLUDED_)
