#if !defined(AFX_MULTISELTREE_H__E0907B16_03AE_4144_9808_4FA57E629589__INCLUDED_)
#define AFX_MULTISELTREE_H__E0907B16_03AE_4144_9808_4FA57E629589__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MultiSelTree.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMultiSelTree window

class CMultiSelTree : public CTreeCtrl
{
// Construction
public:
	CMultiSelTree();

// Attributes
public:
	HTREEITEM	m_firstItem;
	CArray	< HTREEITEM, HTREEITEM & > m_arItem;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiSelTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	HTREEITEM getNextSelectedItem(HTREEITEM hitem);
	HTREEITEM getFirstSelectedItem();
	int GetSelectedItemCount();
	bool selectItems(HTREEITEM hitemFROM, HTREEITEM hitemTO);
	void ClearSelection();
	virtual ~CMultiSelTree();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMultiSelTree)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTISELTREE_H__E0907B16_03AE_4144_9808_4FA57E629589__INCLUDED_)
