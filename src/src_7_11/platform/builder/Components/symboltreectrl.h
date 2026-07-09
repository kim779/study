#if !defined(AFX_SYMBOLTREECTRL_H__90523571_272A_44C3_8F7A_B21A7B93BB90__INCLUDED_)
#define AFX_SYMBOLTREECTRL_H__90523571_272A_44C3_8F7A_B21A7B93BB90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// symboltreectrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSymbolTreeCtrl window

class CSymbolTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CSymbolTreeCtrl();

// Attributes
public:
	CArray	< HTREEITEM, HTREEITEM & > m_arItem;
protected:
	HTREEITEM	m_firstItem;
	HTREEITEM	m_singleItem;
	CRect		m_dragRect;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSymbolTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	bool	selectItems(HTREEITEM hitemFROM, HTREEITEM hitemTO);
	void	ClearSelection();
	HTREEITEM getNextSelectedItem(HTREEITEM hitem);
	HTREEITEM getFirstSelectedItem();
	int	GetSelectedItemCount();
	virtual ~CSymbolTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSymbolTreeCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYMBOLTREECTRL_H__90523571_272A_44C3_8F7A_B21A7B93BB90__INCLUDED_)
