#if !defined(AFX_TABCODESEARCHFR_H__E421A4EE_ED1F_46DE_A389_7F206A85B47B__INCLUDED_)
#define AFX_TABCODESEARCHFR_H__E421A4EE_ED1F_46DE_A389_7F206A85B47B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabCodeSearchFR.h : header file
//
#include "./control/XListCtrl.h"
#include "TabDlg.h"
#include "../../inc/ExOCXDLL.h"
#include "./control/XListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTabCodeSearchFR dialog

class CTabCodeSearchFR : public CTabDlg
{
// Construction
	BOOL m_bInsertingInListCode;
	void InitColumns();
	BOOL InsertData();
	void RecalcLayout();
	CRect	GetRectOfCtrl(INT nID,BOOL bInit = FALSE);
	void GetDispInfo(LVITEM* pItem);

public:
	CTabCodeSearchFR(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabCodeSearchFR)
	enum { IDD = IDD_TABCODESEARCH_FR };
	CXListCtrl	m_listCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabCodeSearchFR)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabCodeSearchFR)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickListcode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListcode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABCODESEARCHFR_H__E421A4EE_ED1F_46DE_A389_7F206A85B47B__INCLUDED_)
