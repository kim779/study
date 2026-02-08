#if !defined(AFX_MYLISTCTRL_H__9A417DA5_31C4_11D2_A335_0060088E1AE4__INCLUDED_)
#define AFX_MYLISTCTRL_H__9A417DA5_31C4_11D2_A335_0060088E1AE4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MyListCtrl.h : header file
//

#include "ChartDrop.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl window

class CMyListCtrl : public CListCtrl
{
// Construction
public:
	CMyListCtrl();

// Attributes
public:
	CChartDrop		m_dropTarget;
	COleDataSource	m_dragSource;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void InitDropTarget();
	virtual ~CMyListCtrl();

	// Generated message map functions
protected:
	CImageList m_ImageList;
	//{{AFX_MSG(CMyListCtrl)
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLISTCTRL_H__9A417DA5_31C4_11D2_A335_0060088E1AE4__INCLUDED_)
