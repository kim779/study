#if !defined(AFX_XTREECTRL_H__E1A22161_6F9B_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_XTREECTRL_H__E1A22161_6F9B_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// xTreeCtrl.h : header file
//

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CXTreeCtrl window

#define	R_NONE		-1

class AFX_EXT_CLASS CXTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CXTreeCtrl();

// Attributes
public:
	CArray	< HTREEITEM, HTREEITEM & > m_arItem;

protected:
	HTREEITEM	m_firstItem;

// Operations
public:
	BOOL	Attach(const UINT nID, CWnd* parent);
	int	GetSelectedItemCount();
	void	ClearSelection();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXTreeCtrl();

	// Generated message map functions
public:
	bool	selectItems(HTREEITEM hitemFROM, HTREEITEM hitemTO);
	HTREEITEM getFirstSelectedItem();
	HTREEITEM getNextSelectedItem(HTREEITEM hitem);
	HTREEITEM getPrevSelectedItem(HTREEITEM hitem);

	//{{AFX_MSG(CXTreeCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XTREECTRL_H__E1A22161_6F9B_11D4_A024_00001CD7F9BE__INCLUDED_)
