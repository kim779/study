#pragma once
// treeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CtreeCtrl window

class CtreeCtrl : public CTreeCtrl
{
// Construction
public:
	CtreeCtrl();

// Attributes
private:
	HTREEITEM	m_item{};


// Operations
public:
	HTREEITEM GetPrevItem(HTREEITEM hItem);
	HTREEITEM GetPrevSelectedItem(HTREEITEM hItem);
	HTREEITEM GetNextSelectedItem(HTREEITEM hItem);
	HTREEITEM GetFirstSelectedItem();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CtreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CtreeCtrl();

	// Generated message map functions
protected:
	void	clearSelection();
	BOOL	selectItems(HTREEITEM hItemFrom, HTREEITEM hItemTo);

	//{{AFX_MSG(CtreeCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

