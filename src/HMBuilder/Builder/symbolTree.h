#pragma once


// CSymbolTree

class CSymbolTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(CSymbolTree)

public:
	CSymbolTree();
	virtual ~CSymbolTree();

public:
	CArray	< HTREEITEM, HTREEITEM & > m_itemArr;

protected:
	HTREEITEM	m_firstItem;
	HTREEITEM	m_singleItem;
	CRect		m_dragRect;

	CImageList*	m_dragImg;

	void	clearSelection();
	bool	selectItems(HTREEITEM hitemFROM, HTREEITEM hitemTO);
	HTREEITEM getFirstSelectedItem();
	HTREEITEM getNextSelectedItem(HTREEITEM hitem);

	CImageList* createDragImage(CPoint& point);

	DECLARE_MESSAGE_MAP()

public:
	HTREEITEM GetSelectedItem(int idx);
	int	GetSelectedItemCount();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult);
};


