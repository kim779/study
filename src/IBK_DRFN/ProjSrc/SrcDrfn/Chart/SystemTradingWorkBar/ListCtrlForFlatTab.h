// ListCtrlForFlatTab.h : header file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// ©2004 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO 
// BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED 
// WRITTEN CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS 
// OUTLINED IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT.  CODEJOCK SOFTWARE 
// GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE 
// THIS SOFTWARE ON A SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTCTRLFORFLATTAB_H__065C7848_BDB9_4599_A81B_81F15E9A23E6__INCLUDED_)
#define AFX_LISTCTRLFORFLATTAB_H__065C7848_BDB9_4599_A81B_81F15E9A23E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CListCtrlForFlatTab window

class CListCtrlForFlatTab : public CListCtrl
{
// Construction
public:
	CListCtrlForFlatTab();

// Attributes
public:
	CMenu	m_mnuPopup;

// Operations
public:
	void	GotoError(int nItem);
	void	GotoTag(int nItem);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlForFlatTab)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListCtrlForFlatTab();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListCtrlForFlatTab)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnGoto();
	afx_msg void OnCopytext();
	afx_msg void OnClear();
	afx_msg void OnUpdateGoto(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCopytext(CCmdUI* pCmdUI);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLFORFLATTAB_H__065C7848_BDB9_4599_A81B_81F15E9A23E6__INCLUDED_)
