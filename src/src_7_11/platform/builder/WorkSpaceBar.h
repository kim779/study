// WorkSpaceBar.h: interface for the CWorkSpaceBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKSPACEBAR_H__4B500CAD_9C3E_4897_BCDF_23617432189D__INCLUDED_)
#define AFX_WORKSPACEBAR_H__4B500CAD_9C3E_4897_BCDF_23617432189D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdAfx.h"
#include "Components/FileTreeCtrl.h"
#include "Components/ServerTreeCtrl.h"
#include "NFBtn.h"
#include "awUser/SymTbl.h"
#include "Components/CheckComboBox.h"
#include "Components/ToolTipListCtrl.h"
#include "Components/WEdit.h"
#include "Components/symbolTreeCtrl.h"

class CWorkSpaceBar : public CSizingControlBarG  
{
public:
	CWorkSpaceBar();
	virtual ~CWorkSpaceBar();
public:
	CFont		m_font, m_tabFont;
	CString		m_path;
	CImageList*	m_pDragImg, m_tabImg;
	
	CSymTbl		m_SymTbl;
	CServerTreeCtrl	m_TreeServer;
	CServerTreeCtrl	m_TreeTemplate;
	CSymbolTreeCtrl	m_TreeSymbol;

	bool		m_bOffline;
	CWEdit		m_fnames;

	int		m_nSvrDepth;
	int		m_nTmpDepth;

protected:
	CTabCtrl	m_TabCtrl;
	CComboBox	m_groupCBO_sym;
	CComboBox	m_groupCBO_tmp;
	CComboBox	m_groupCBO_svr;
	CComboBox	m_projectCBO;

	CCheckComboBox	m_ioCBO;
	CToolTipListCtrl	m_List_sym;
	CToolTipListCtrl	m_List_tmp;
	CToolTipListCtrl	m_List_svr;
	CTreeFileCtrl	m_TreeFile;
	
	CNFBtn		m_openBtn;
	CNFBtn		m_treeBtn;
	CNFBtn		m_listBtn;
	CNFBtn		m_ReloadBtn;
	CNFBtn		m_MapOnlyBtn;
	CNFBtn		m_depthBtn;

	bool		m_bWait;
	HBITMAP		m_hTreeBitmap;
	HBITMAP		m_hListBitmap;
	HBITMAP		m_hOpenBitmap;
	HBITMAP		m_hReloadBitmap;
	HBITMAP		m_hMapOnlyBitmap;
	HBITMAP		m_hTwoDepthBitmap;

private:
	TCHAR* m_pchTip ;
	WCHAR* m_pwchTip;

public:
	CImageList* createDragImage(CSymbolTreeCtrl *TC, LPPOINT lpPoint);
	CImageList* createDragImage(CListCtrl *LC, LPPOINT lpPoint);

	void	OnMapOnly();
	void	ChangeHangulMode(HWND hWnd, bool bHangle);
	CString	Parser(CString &srcstr, CString substr);
	void	DeleteDragImg();
	int	GetSelectedCount();
	BOOL	addColumn(CListCtrl *LC, LPCTSTR str, int item, int width);
	void	SetMainInfo(CString dir,  CString path, int child, bool request);
	void	initialize();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkSpaceBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

		// Generated message map functions
protected:
	//{{AFX_MSG(CWorkSpaceBar)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnList1BeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnList2BeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnList3BeginDrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnReload();
	afx_msg void OnSymGrpSelChange();
	afx_msg void OnSymPrjSelChange();
	afx_msg void OnOpenMap();
	afx_msg void OnTreeBtnClick();
	afx_msg void OnDepthBtnClick();
	afx_msg void OnServerDblClk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClientDblClk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmplDblClk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTmplSelChange();
	afx_msg void OnSvrSelChange();
	afx_msg void OnSvrDblClk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnListSvrClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnListTmpClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSTreeBeginDrag(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTmpDblClk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_WORKSPACEBAR_H__4B500CAD_9C3E_4897_BCDF_23617432189D__INCLUDED_)
