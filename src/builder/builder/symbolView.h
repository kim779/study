#pragma once

#include "symbolTree.h"
#include "ttListCtrl.h"
#include "checkComboBox.h"
#include "symTbl.h"

class CSymbolViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

// CSymbolView

class CSymbolView : public CDockablePane
{
	DECLARE_DYNAMIC(CSymbolView)

public:
	CSymbolView();
	virtual ~CSymbolView();

	CSymTbl	m_symTbl;

	bool	GetSelectedSymbol(CPtrArray* arr);

protected:
	CSymbolViewToolBar m_wndToolBarTree;
	CSymbolViewToolBar m_wndToolBarList;

	CComboBox	m_projectCB;
	CComboBox	m_groupCB;
	CCheckComboBox	m_ioCB;

	CSymbolTree	m_tree;
	CTTListCtrl	m_list;

	CFont	m_font;

private:
	CString	m_root;
	bool	m_isTree;

protected:
	void	adjustLayout();
	void	setFont();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRefresh();
	afx_msg void OnTree();
	afx_msg void OnList();
	afx_msg void OnProjectSelChange();
	afx_msg void OnGroupSelChange();
	afx_msg void OnUpdateCommand(CCmdUI *pCmdUI);

	DECLARE_MESSAGE_MAP()
};


