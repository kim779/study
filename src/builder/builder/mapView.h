#pragma once

#include "ViewTree.h"
#include "ttListCtrl.h"
#include "mEdit.h"

class CMapViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};


// CMapView

class CMapView : public CDockablePane
{
	DECLARE_DYNAMIC(CMapView)

public:
	CMapView();
	virtual ~CMapView();

protected:
	CMapViewToolBar m_wndToolBarTree;
	CMapViewToolBar m_wndToolBarList;

	CComboBox	m_group;
	CViewTree	m_tree;
	CTTListCtrl	m_list;

	CFont	m_font;
	int	m_kind;

	DECLARE_MESSAGE_MAP()

private:
	CString	m_dir;
	bool	m_isTree;
	bool	m_isTwoDepth;
	CCriticalSection m_section;

	CMEdit	m_edit;

public:
	void	SetDirectory(CString dir)	{ m_dir = dir; }
	int	GetFiles(CStringArray& files);
	void	OpenMap(CString maps);
	int	IsTwoDepth();
	void	SetTwoDepth(bool twoDepth);
	CString	LoadForm(CString mapName);
	void	SetMapTreeKind(int kind, bool bReflash);

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
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};


