#if !defined(AFX_TABBASIC_H__CCA90CF5_1761_4ECC_BCE8_2144C1AF6118__INCLUDED_)
#define AFX_TABBASIC_H__CCA90CF5_1761_4ECC_BCE8_2144C1AF6118__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabBasic.h : header file
//
#include "Resource.h"
#include "RscDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CTabBasic dialog

class CTabBasic : public CRscDlg
{
// Construction
public:
	CTabBasic(CWnd* pParent = NULL);   // standard constructor
	void SetColor(COLORREF& rCrBk) { m_crBk = rCrBk;}
	void SetBrush(CBrush*	pBrBkColor) { m_pBrBkColor = pBrBkColor;}
// Dialog Data
	//{{AFX_DATA(CTabBasic)
	enum { IDD = IDD_DLG_TAB_BASIC };
	CEdit	m_editLangExplanation;
	CEdit	m_editExplanation;
	CEdit	m_editName;
	CButton	m_ctrlLangType0;
	CButton	m_ctrlLangType1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabBasic)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void LoadInfo();
	BOOL SaveInfo();

	COLORREF m_crBk;
	CBrush*	m_pBrBkColor;
	BOOL	m_bInitEvent;
	CWnd*	m_pDlgSTOption;

	// Generated message map functions
	//{{AFX_MSG(CTabBasic)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioLangtype0();
	afx_msg void OnRadioLangtype1();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABBASIC_H__CCA90CF5_1761_4ECC_BCE8_2144C1AF6118__INCLUDED_)
