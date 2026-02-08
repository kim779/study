#if !defined(AFX_TABCODESEARCHELW_H__BF8B23C4_7FBE_4730_A5C1_F8FDB040602B__INCLUDED_)
#define AFX_TABCODESEARCHELW_H__BF8B23C4_7FBE_4730_A5C1_F8FDB040602B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabCodeSearchELW.h : header file
//
#include "../../inc/ExOCXDLL.h"
#include "TabDlg.h"
#include "./control/XListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTabCodeSearchELW dialog

// 콜 및 풋 옵션 월 리스트 
class CELWItem
{
public: 
	CELWItem(){};
	virtual ~CELWItem(){};

	//DATA
	CString m_strCode;
	CString m_strName;

	//FUNCTION
	void SetItem(CString strCode, CString strName)
	{
		m_strCode = strCode;
		m_strName = strName;
	};
};
class CTabCodeSearchELW : public CTabDlg
{
// Construction
	BOOL m_bInsertingInListCode;

	void InitColumns();
	BOOL InsertData();
	void RecalcLayout();
	CRect	GetRectOfCtrl(INT nID,BOOL bInit = FALSE);
	void GetDispInfo(LVITEM* pItem);

public:
	CTabCodeSearchELW(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabCodeSearchELW)
	enum { IDD = IDD_TABCODESEARCH_ELW };
	CXListCtrl	m_listCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabCodeSearchELW)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabCodeSearchELW)
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

#endif // !defined(AFX_TABCODESEARCHELW_H__BF8B23C4_7FBE_4730_A5C1_F8FDB040602B__INCLUDED_)
