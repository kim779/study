
#pragma once

#include "h/workio.h"

/////////////////////////////////////////////////////////////////////////////
// CViewTree 창입니다.
class CMapTree 
{
public:
	CString m_mgub;
	CString m_desc;
	CString m_fname;
	CString m_ckgb;
	CString m_ckid;
};

class CViewTree : public CTreeCtrl
{
// 생성입니다.
public:
	CViewTree();
	virtual ~CViewTree();

// 재정의입니다.
public:
	CStringArray	m_files;
//	CArray < _mapTree, _mapTree >	m_treeArr;
	CArray <CMapTree*, CMapTree* >	m_treeArr;

	int	m_maptreekind;

protected:
	CString	m_dir;

	CArray	< HTREEITEM, HTREEITEM & > m_itemArr;

	CImageList*	m_pilDrag;

	HTREEITEM	m_firstItem;
	HTREEITEM	m_dragItem;
	HTREEITEM	m_dropItem;

private:
	bool	m_open;
	int	m_depth;

	CString	m_name;

	HCURSOR	m_cDropCopy;
	HCURSOR	m_cNoDropCopy;
	HCURSOR	m_cDropMove;
	HCURSOR	m_cNoDropMove;

	CMapStringToPtr	m_itemMap1;
	CMapStringToPtr m_itemMap2;

// 구현입니다.
public:
	void	Initialize(CString dir);
	void	InitTree();
	void	LoadFile(CString path);
	void	OpenMap(int idx = -1);
	void	OpenMap(CString mapName);

	void	SetDepth(int depth, bool init = true);
	int	GetSelectedItemCount();
	CString	GetNameOfIdx(int idx);

	void	RemoveAll();
	int	GetFiles(CStringArray& files);
	CString	Parser(CString &srcstr, CString substr);

protected:
	void	clearSelection();
	bool	selectItems(HTREEITEM hitemFROM, HTREEITEM hitemTO);
	HTREEITEM getFirstSelectedItem();
	HTREEITEM getNextSelectedItem(HTREEITEM hitem);
	void	addFiles(CString name);
	void	insert(CString maps, CString desc, int idx);
	CString	setMapPath(CString maps);

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
