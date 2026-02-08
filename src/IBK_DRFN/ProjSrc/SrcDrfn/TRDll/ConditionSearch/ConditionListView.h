#if !defined(AFX_CONDITIONLISTVIEW_H__3F4E9214_A1F6_40BB_9677_D44DFBC9D337__INCLUDED_)
#define AFX_CONDITIONLISTVIEW_H__3F4E9214_A1F6_40BB_9677_D44DFBC9D337__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConditionListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConditionListView window
#include "DefineFiles/client_typedef.h"

//Control
#include "./AddControl/XmlTreeCtrl.h"
#include "./AddControl/HistoryComboCtrl.h"
#include "./AddControl/HelpEditCtrl.h"

#include "../../../SrcSite/control/fx_misc/fxImgButton.h"
#include "../../../SrcSite/control/fx_misc/misctype.h"
#include "../../DrCommon/Dr_Control/DrTabCtrl.h"

class CFolderEditDlg;

class CConditionListView : public CView
{
public:
	CConditionListView();
	virtual ~CConditionListView();
	DECLARE_DYNCREATE(CConditionListView)
		
public:
	BOOL m_bIndustry;
	
	int m_nViewType_Tree;   //# 0 : Comm, 1 : Order
	BOOL m_bTreeHelpExp;

	int m_nSeparateForm;
	
private:
	COLORREF	m_clrMainSkin, m_clrStaic;
	CWnd    *m_pwndMain;
	CFont m_fontDefault, m_fontBold;
	CBrush m_brushBK;
	CBrush m_brushBKTab;
	
	
	//# Search
	int m_nSearchPos;
	int m_nSearchCnt;
	CArray<HTREEITEM, HTREEITEM>		m_saFindItem;
	
	ExplainInfo					m_stExplainInfo;
	MSXML::IXMLDOMDocumentPtr	m_pDomXmlExp;
	CString m_strAfterExpTag;
	
	BOOL m_bActiveTreeComboLoad;
	
	CFolderEditDlg	*m_pdlgFolderEdit;
	
	CImageList m_imglistTree;

	int m_nLastSelectTab; // 20110628 박찬모 조건이 바뀌었을시 무조건 changeTab을 하는 오류 >>

	CRect	m_reTree;

	
public:
	CDrTabCtrl*			m_ptabCondition;
	
	CStatic				m_ctrlSearchBar;
	CHistoryComboCtrl	m_comboSearch;
    
	CfxImgButton		m_btnSearch;
	CfxImgButton		m_btnNext;
	CfxImgButton		m_btnPre;
	CfxImgButton		m_btnCollaspe;
	
	CStatic				m_ctrlMenuBar;
	CButton				m_btnTypeSort;
	CButton				m_btnTypeNormal;
	CfxImgButton		m_btnFolderEdit;
	CfxImgButton		m_btnFileAdd;
	
	CXmlTreeCtrl*		m_ptreeActive;
	CXmlTreeCtrl		m_treeCommonCtrl;
	CXmlTreeCtrl		m_treeCommonCtrl_Sort;
	CXmlTreeCtrl		m_treeOrderCtrl;
	CXmlTreeCtrl		m_treeOrderCtrl_Sort;
	CXmlTreeCtrl		m_treeExcavation;
	
	CHelpEditCtrl		m_editHelp;
	CfxImgButton		m_btnHelpView;

	CRect				m_rtTab;
	
public:
	void SetInit(int nSeparateForm);
	
	void SetItme_User(BOOL bModify, XMLTreeInfoFile	stXMLTreeInfoFile);
	void SerTreeReload();
	
	void SetSelectTab(int nViewType, int nTabIndex = -1);	
	void SetHelpExp(BOOL bHelpExp);
	
	void SetExplainView(CXmlTreeCtrl	*pTreeCtrl);
	void SetFileExplain(XMLTreeInfoFile	stXMLTreeInfoFile);
	void SetTextExplain(CString strExplain);
	void SetItemExplain(XMLTreeInfo stXMLTreeInfo);
	void SetItemExplain(CString strTag);
	BOOL GetExplainItem(MSXML::IXMLDOMNodeList *pNodes, CString strTag,
		ExplainInfo &stExplainInfo);
	
	void SetAddTreeMark(XMLTreeInfo stXMLTreeInfo);
	void SetDelTreeMark(XMLTreeInfo stXMLTreeInfo);
	
	void SetControlEnable(BOOL bState);
	
	void SetTreeView();
	void SetTreeView(CXmlTreeCtrl* pTreeCtrl);
	
	void SetCallBulletin(CString strURL);
	LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);
	
	void SetFindBar_Init();
//	void ChangeSkinColor(COLORREF clrSkin);
	void ChangeSkinColor(COLORREF clrSkin, ST_TABCOLOR stTabColor);

	void CallBtnItemHelp()					{	OnBtnItemHelp(); }

	BOOL	GetSaveFlag();
	CString GetSaveFileName();

	void SetShowTree(BOOL bShow);
	void SetSelectItem();
	
private:
	void OnSize();
	void OnSizeSearchBar(CRect reClient);
	void OnSizeMenuBar(CRect reClient);
	
	CRect OnSizeHelpView(BOOL bView);
	void SetShowSearchBar(BOOL bState);
	
	void SetSendTitleBar(CString strMsg);
	int SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK);
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConditionListView)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL
	
	
	// Implementation
protected:
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
	
	// Implementation
public:
	
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CConditionListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnChangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	
	afx_msg void OnBtnSearch();
	afx_msg void OnBtnNext();
	afx_msg void OnBtnPrevious();
	afx_msg void OnBtnExp();
	
	afx_msg void OnBtnItemHelp();
	
	afx_msg void OnBtnNormalType();
	afx_msg void OnBtnSortType();
	
	afx_msg void OnBtnFolderEdit();
	afx_msg void OnBtnFileAdd();
	
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);//20100827 이문수 >>
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONDITIONLISTVIEW_H__3F4E9214_A1F6_40BB_9677_D44DFBC9D337__INCLUDED_)
