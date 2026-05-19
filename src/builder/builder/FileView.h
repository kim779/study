
#pragma once

#include "filetree.h"

class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileView : public CDockablePane
{
// 생성입니다.
public:
	CFileView();

// 특성입니다.
protected:
	CFileTree	m_tree;
	CFileViewToolBar m_wndToolBar;

// 구현입니다.
public:
	virtual ~CFileView();

protected:
	void	adjustLayout();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnRefresh();
	afx_msg void OnMapOnly();
	afx_msg void OnUpdateCommand(CCmdUI *pCmdUI);


	DECLARE_MESSAGE_MAP()
};

