#if !defined(AFX_EXTREECTRL_H__1E2A05A2_2F2A_11D5_941A_00001CD7EE63__INCLUDED_)
#define AFX_EXTREECTRL_H__1E2A05A2_2F2A_11D5_941A_00001CD7EE63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

//#include "resource.h"

class AFX_EXT_CLASS TreeData
{
public:
	TreeData(int depth, int nData);
	TreeData(int depth, CString sData);
	TreeData(CString data, DWORD dw);
	TreeData(int depth, int nData, DWORD dw);
	
	virtual ~TreeData();
public:
	CString m_sData;
	int	m_nData;
	int	m_nDepth;
	DWORD	m_sPoint;
};

////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CExTreeCtrl : public CTreeCtrl
{
public:
	CExTreeCtrl();
	virtual ~CExTreeCtrl();

	//{{AFX_VIRTUAL(CExTreeCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CExTreeCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	/*afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);*/
	DECLARE_MESSAGE_MAP()

public:
	void	InitTreeImage(CBitmap& bm);
	void	FreePtr();
	void	SetEditState(bool bEdit, HTREEITEM item = NULL);
	
	void	SetItemDataEx(HTREEITEM hItem, int depth, CString str);
	void	GetItemDataEx(HTREEITEM hItem, int& depth, CString& str);
	void	SetItemDataEx(HTREEITEM hItem, int depth, int ndata);
	void	GetItemDataEx(HTREEITEM hItem, int& depth, int& ndata);
	void	SetItemDataEx(HTREEITEM hItem, CString data, DWORD dw);
	void	GetItemDataEx(HTREEITEM hItem, CString& data, DWORD& dw);
	void	SetItemDataEx(HTREEITEM hItem, int depth, int ndata, DWORD dw);
	void	GetItemDataEx(HTREEITEM hItem, int& depth, int& ndata, DWORD& dw);


	void	SetDragEnabled(bool bDrag = true);
	void	SetMultiSel(bool bMultiSel = true);
	
public:
	bool		m_bDragging;

protected:
	HTREEITEM	copyBranch(HTREEITEM hitemDrag, HTREEITEM hitemDrop, 
					HTREEITEM htiAfter = TVI_LAST);

	HTREEITEM	copyItem(HTREEITEM hItem, HTREEITEM htiNewParent, 
					HTREEITEM htiAfter = TVI_LAST);
	int		getDepth(HTREEITEM item);

	void		clearSelection();
	BOOL		selectItems(HTREEITEM hItemFrom, HTREEITEM hItemTo);
	HTREEITEM	GetFirstSelectedItem();
	HTREEITEM	GetNextSelectedItem(HTREEITEM hItem);
	HTREEITEM	GetPrevSelectedItem(HTREEITEM hItem);
	HTREEITEM	GetPrevItem(HTREEITEM hItem);

private:
	CImageList*	m_ImageList;
	CPtrArray	m_Ptr;
	bool		m_bDrag;
	
	HTREEITEM	m_hItemDrag;
	HTREEITEM	m_hItemDrop;
	
	bool		m_bMultiSel;
	HTREEITEM	m_hItemFirstSel;

};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_EXTREECTRL_H__1E2A05A2_2F2A_11D5_941A_00001CD7EE63__INCLUDED_)
