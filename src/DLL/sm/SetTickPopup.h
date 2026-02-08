#if !defined(AFX_SETTICKPOPUP_H__1A103CB5_8A15_4BCC_8900_361B0986485D__INCLUDED_)
#define AFX_SETTICKPOPUP_H__1A103CB5_8A15_4BCC_8900_361B0986485D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetTickPopup.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CSetTickPopup dialog
#define	CTRL_SETTICKPOPUP	100

class AFX_EXT_CLASS CSetTickPopup : public CDialog
{
// Construction
public:
	CWnd		*m_pParent;
	CPoint		m_point;
public:
	BOOL GetCheck(int nIdx);
	void SetCheck(int nIdx, BOOL bCheck);
	CSetTickPopup(CWnd* pParent = NULL);   // standard constructor
	DWORD GetIDByIdx(int nIdx);
// Dialog Data
	//{{AFX_DATA(CSetTickPopup)
	enum { IDD = IDD_SETTICK_POPUP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetTickPopup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetTickPopup)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnPaint();
	afx_msg void OnAllCheck();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSetupBtn();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTICKPOPUP_H__1A103CB5_8A15_4BCC_8900_361B0986485D__INCLUDED_)
