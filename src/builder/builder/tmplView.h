#pragma once

#include "ViewTree.h"
#include "ttListCtrl.h"
#include "mEdit.h"

class CTmplViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};


// CTmplView

class CTmplView : public CDockablePane
{
	DECLARE_DYNAMIC(CTmplView)

public:
	CTmplView();
	virtual ~CTmplView();

protected:
	CTmplViewToolBar m_wndToolBarTree;
	CTmplViewToolBar m_wndToolBarList;

	CComboBox	m_group;
	CViewTree	m_tree;
	CTTListCtrl	m_list;
	CMEdit		m_edit;

	CFont	m_font;

	DECLARE_MESSAGE_MAP()

private:
	CString	m_dir;
	bool	m_isTree;
	bool	m_isTwoDepth;
	CCriticalSection m_section;

public:
	void	SetDirectory(CString dir)	{ m_dir = dir; }
	int	GetFiles(CStringArray& files);
	void	OpenMap(CString maps);
	int	IsTwoDepth();
	void	SetTwoDepth(bool twoDepth);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRefresh();
	afx_msg void OnTree();
	afx_msg void OnOpen();
	afx_msg void OnTwoDepth();
	afx_msg void OnList();
	afx_msg void OnSelChange();
	afx_msg void OnUpdateCommand(CCmdUI *pCmdUI);
	afx_msg LRESULT OnMessage(WPARAM wp, LPARAM lp);

protected:
	void	adjustLayout();
	void	setFont();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


