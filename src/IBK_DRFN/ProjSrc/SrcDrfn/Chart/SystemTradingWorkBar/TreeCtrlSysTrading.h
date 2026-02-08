// TreeCtrlSysTrading.h: interface for the CTreeCtrlSysTrading class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREECTRLSYSTRADING_H__B89BE636_69BA_4776_BDAB_2DBB408DB122__INCLUDED_)
#define AFX_TREECTRLSYSTRADING_H__B89BE636_69BA_4776_BDAB_2DBB408DB122__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../common_ST/DropSource.h"
#include "../common_ST/DropTarget.h"

#include "TreeDropTarget.h"

class CTreeCtrlSysTrading : public CXTTreeCtrl
{
	DECLARE_DYNAMIC(CTreeCtrlSysTrading)
public:
	CTreeCtrlSysTrading(UINT nTreeType);
	void	OpenSelectedItem(HTREEITEM hItem);
	void	DelSelectedItem(HTREEITEM hItem);
	void	MultiCompileSelectedItem(HTREEITEM hItem, BOOL bMakeDll);
	void	SingleCompileSelectedItem(HTREEITEM hItem, BOOL bMakeDll);
	void	StartDrag(NM_TREEVIEW* pNMTreeView);
	void	SetMenuStatement(HTREEITEM hItem, CMenu* pPopupMenu);
	void	SetMenuStatementGroup(CCmdUI* pCmdUI);
	void	SetMenuStatementScript(CCmdUI* pCmdUI);
	void	SaveTreeGroup();
	void	LoadTreeGroup();
	HTREEITEM	AddNewGroup(LPCSTR szName);
	virtual HTREEITEM	FindItem(LPCTSTR lpszSearch, HTREEITEM hItem, BOOL bOnlyRoot);

private:
	UINT	m_nTreeType;
	CDropTarget m_DropTaget;
	CTreeDropTarget	m_DropTaget2;

	CMenu	m_mnuPopup;
	int		m_nGroupCount;
	UINT	m_nMenuCopyID;
	UINT	m_nMenuMoveID;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeCtrlSysTrading)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTreeCtrlSysTrading();

	// Generated message map functions

protected:
	//{{AFX_MSG(CTreeCtrlSysTrading)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnNewgroup();
	afx_msg void OnOpenfile();
	afx_msg void OnDelscript();
	afx_msg void OnUpdateDelscript(CCmdUI* pCmdUI);
	afx_msg void OnSinglecompile();
	afx_msg void OnProperty();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDelgroup();
	afx_msg void OnRenamegroup();
	afx_msg void OnUpdateDelgroup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRenamegroup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOpenfile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateProperty(CCmdUI* pCmdUI);
	afx_msg void OnBuildSingleCompileDll();
	afx_msg void OnUpdateNewgroup(CCmdUI* pCmdUI);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdatSinglecompile(CCmdUI* pCmdUI);
	afx_msg void OnUpdatBuildSingleCompileDll(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnGroupCopy(UINT nID);
	afx_msg void OnGroupMove(UINT nID);
	afx_msg void OnUpdateGroupCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGroupMove(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_TREECTRLSYSTRADING_H__B89BE636_69BA_4776_BDAB_2DBB408DB122__INCLUDED_)
