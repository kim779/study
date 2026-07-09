#if !defined(AFX_STOCKNEWDLG_H__4EFF3BDC_33BC_4649_BB1E_7C8478053CD9__INCLUDED_)
#define AFX_STOCKNEWDLG_H__4EFF3BDC_33BC_4649_BB1E_7C8478053CD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StockNewDlg.h : header file
//
#include "fxButton.h"
/////////////////////////////////////////////////////////////////////////////
// CStockNewDlg dialog



class CStockNewDlg : public CDialog
{
// Construction
public:
	void SetButton();
	void SetButton(CButtonST *pButton);

	CStockNewDlg(CPoint pt = CPoint(-1, -1), int nType = 0, CWnd* pParent = NULL);   // standard constructor
	BOOL	CountData();
	CString GetName() { return m_szName; }
	CString GetCode() { return m_szCode; }
	void	AddItem(int row, CString szCode, CString szName, CString szTcode, CString kind);
	BOOL	loadJCode();
	CString	GetName(char* code);
	void	SetDlgPos(int x, int y);

// Dialog Data
	//{{AFX_DATA(CStockNewDlg)
	enum { IDD = IDD_DIALOG_STOCKNEW };
	CfxButton	m_suikBtn;
	CfxButton	m_newStockBtn;
	CfxButton	m_elwBtn;
	CfxButton	m_allCodeBtn;
	CSkinListCtrl	m_ListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStockNewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	int		m_nType, m_nStep;
	CTool		m_Tool;

	CString		m_szCode;
	CString		m_szName;

	CPoint		m_Point;

	HBRUSH		m_hBrush;

	CArray<NCODE, NCODE>m_arrayNCode;

	// Generated message map functions
	//{{AFX_MSG(CStockNewDlg)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListNew(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCancelMode();
	afx_msg void OnAllcodeBtn();
	afx_msg void OnElwBtn();
	afx_msg void OnNewstockBtn();
	afx_msg void OnSuikBtn();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOCKNEWDLG_H__4EFF3BDC_33BC_4649_BB1E_7C8478053CD9__INCLUDED_)
