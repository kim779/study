#if !defined(AFX_TABCODESEARCHJISU_H__CABE69B0_C626_4F07_8629_B49CE1EBF652__INCLUDED_)
#define AFX_TABCODESEARCHJISU_H__CABE69B0_C626_4F07_8629_B49CE1EBF652__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabCodeSearchJisu.h : header file
//
#include "resource.h"
#include "TabDlg.h"
#include "./control/XListCtrl.h"
#include "XListCtrlJongData.h"
#include "JongStatusInTab.h"
/////////////////////////////////////////////////////////////////////////////
// CTabCodeSearchJisu dialog

class CTabCodeSearchJisu : public CTabDlg
{
// Construction
public:
	CTabCodeSearchJisu(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabCodeSearchJisu)
	enum { IDD = IDD_TABCODESEARCH_UP };
	CXListCtrlJongData	m_listCode;
	CILButton	m_BtnKospi;
	CILButton	m_BtnK200;
	CILButton	m_BtnKosdaq;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CJongStatusInTab m_JongStatusInTab;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabCodeSearchJisu)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
public:
	void SetRealPriceData(char *pData, int nSize);
private:
	void	AdviseMain(int nAdivise);
	CRect	GetRectOfCtrl(INT nID,BOOL bInit = FALSE);
	void	RecalcLayout();
	void	RequestCurrentPrice(LPCTSTR lpBuffer,long lLength);
	void	ReceiveData(LPCSTR szTR, LPVOID aTRData, long dwTRDateLen);
	void	SetConfig(BOOL bLoad = TRUE);

	BOOL		m_bInsertingInListCode;
	BOOL		m_bLBtnDown;
	BOOL		m_bDragNDrop;
	int			m_nPosGroup;
	long		m_lTitleCode;
	BOOL		m_bCheckError;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabCodeSearchJisu)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDblclkListcode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindragListcode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnItemchangedListcode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListcode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListcode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChkKospi();
	afx_msg void OnChkKosdaq();
	afx_msg void OnChkK200();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABCODESEARCHJISU_H__CABE69B0_C626_4F07_8629_B49CE1EBF652__INCLUDED_)
