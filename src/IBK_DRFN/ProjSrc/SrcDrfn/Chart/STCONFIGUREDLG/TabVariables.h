#if !defined(AFX_TABVARIABLES_H__E05EFB9A_E963_408C_99ED_F2225E52E92B__INCLUDED_)
#define AFX_TABVARIABLES_H__E05EFB9A_E963_408C_99ED_F2225E52E92B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabVariables.h : header file
//
#include "Resource.h"
#include "RscDlg.h"
#include "Control/XListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CTabVariables dialog

class CTabVariables : public CRscDlg
{
// Construction
public:
	CTabVariables(CWnd* pParent = NULL);   // standard constructor
	void SetColor(COLORREF& rCrBk) { m_crBk = rCrBk;}
	void SetBrush(CBrush*	pBrBkColor) { m_pBrBkColor = pBrBkColor;}

// Dialog Data
	//{{AFX_DATA(CTabVariables)
	enum { IDD = IDD_DLG_TAB_VARIABLES };
	CButton	m_chkSaveAsDefault;
	CButton	m_btnDelete;
	CButton	m_btnAdd;
	CXListCtrl	m_listVariables;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabVariables)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitColumn();
	void LoadInfo();
	BOOL SaveInfo();
	void RemoveInfo();
	BOOL CheckWhetherHasSameName(LPCTSTR lpNewNameForInsertion);
	COLORREF m_crBk;
	CBrush*	m_pBrBkColor;
	CWnd*	m_pDlgSTOption;

	// Generated message map functions
	//{{AFX_MSG(CTabVariables)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDelete();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABVARIABLES_H__E05EFB9A_E963_408C_99ED_F2225E52E92B__INCLUDED_)
