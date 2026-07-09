#if !defined(AFX_AUTOCMPLDLG_H__FAE9EDB7_7074_4F0D_B3E0_1514F7AF7156__INCLUDED_)
#define AFX_AUTOCMPLDLG_H__FAE9EDB7_7074_4F0D_B3E0_1514F7AF7156__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoCmplDlg.h : header file
//
#include "resource.h"
#include "Components/ListBoxST.h"
/////////////////////////////////////////////////////////////////////////////
// CAutoCmplDlg dialog

class CAutoCmplDlg : public CDialog
{
// Construction
public:
	void	Reset();
	void	AddString(CString str, int type = -1);
	int	getWidth();
	LOGFONT	setFont(int fsize, CString fname = "", BOOL bBold = FALSE, BOOL bItalic = FALSE);
	void	FindString(CString str);
	CString	getSelString();
	void	CreateImageList();
	CAutoCmplDlg(CWnd* pParent = NULL);   // standard constructor

protected:
	
public:
	BOOL IsEmpty();
	CWnd*	m_pParent;
	CImageList	m_ImageList;
// Dialog Data
	//{{AFX_DATA(CAutoCmplDlg)
	enum { IDD = IDD_AUTOCOMPLETE };
	CListBoxST	m_aclist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoCmplDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoCmplDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT	OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOCMPLDLG_H__FAE9EDB7_7074_4F0D_B3E0_1514F7AF7156__INCLUDED_)
