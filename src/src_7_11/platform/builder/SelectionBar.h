// SelectionBar.h: interface for the CSelectionBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELECTIONBAR_H__6CE463BC_F09D_4823_975E_B4695A834349__INCLUDED_)
#define AFX_SELECTIONBAR_H__6CE463BC_F09D_4823_975E_B4695A834349__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mapform.h"
#include "awCommon/FormItem.h"
#include "Components/MultiSelTree.h"

class CSelectionBar : public CSizingControlBarG  
{
	// operations
public:
	CSelectionBar();
	virtual ~CSelectionBar();
protected:
private:
	// attributes
public:
	void UpdateEventItem(int nEvent, int nIndex, bool bAdd = true);
	void AddTreeEventItem(int nIndex, int nEvent, HTREEITEM hParent);
	void AddEventItem(int nIndex, HTREEITEM hMyItem = NULL);
	void	ReplaceMapName(struct _mapH *mapH);
	void	RemoveMap(struct _mapH *mapH);
	HTREEITEM GetParentItem(HTREEITEM hItem);
	void	ClearSelection();
	HTREEITEM FindItem(DWORD data, HTREEITEM hBaseItem);
	void	SelectControl(int selctl);
	CString	GetMapName(struct _mapH *mapH = NULL);
	HTREEITEM GetMapItem(struct _mapH *mapH);
	void	ResetTree(int type = 1);
	void	ReplaceTreeItem(CString sStr, DWORD data);
	HTREEITEM AddTreeItem(CString sStr, DWORD data, int nDepth);
	void	Initialize(struct _mapH *mapH);
protected:
	struct	_mapH*		m_mapH;
	CFormItem	*m_pFormItem;
private:
	CMultiSelTree	m_treeCtrl;
	HTREEITEM	m_hMapItem;
protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectionBar)
	public:
	//}}AFX_VIRTUAL

		// Generated message map functions
protected:
	//{{AFX_MSG(CSelectionBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	long OnSelectMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SELECTIONBAR_H__6CE463BC_F09D_4823_975E_B4695A834349__INCLUDED_)
