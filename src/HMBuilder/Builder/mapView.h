#pragma once

#include "ViewTree.h"
#include "ttListCtrl.h"
#include "mEdit.h"

#define ID_MAPTREE		0
#define ID_TRLATOUT		1

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

	int	m_maptreegubn;

	DECLARE_MESSAGE_MAP()

private:
	CString	m_dir;
	bool	m_isTree;
	bool	m_isTwoDepth;
	CString m_usid;
	CCriticalSection m_section;

	CMEdit	m_edit;

public:
	void	SetDirectory(CString dir)	{ m_dir = dir; }
	int	GetFiles(CStringArray& files);
	void	OpenMap(CString maps);
	int	IsTwoDepth();
	void	SetTwoDepth(bool twoDepth);
	void	LoadMapTree(CString usid = _T(""), int maptreekind = 0);

	void	SetMapTreeGubn(int treekind);

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
	afx_msg LRESULT OnDownload(WPARAM wp, LPARAM lp);

protected:
	void	adjustLayout();
	void	setFont();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};


