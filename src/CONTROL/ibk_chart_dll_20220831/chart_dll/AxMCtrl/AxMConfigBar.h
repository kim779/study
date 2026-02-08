#if !defined(AFX_AXMCONFIGBAR_H__A23CE059_3B7B_4CE8_9992_ABE653A1124D__INCLUDED_)
#define AFX_AXMCONFIGBAR_H__A23CE059_3B7B_4CE8_9992_ABE653A1124D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// AxMConfigBar.h : header file
//

#include "../AxMCtrl/AxMCtrl.h"

#include "CodeHelper.h"
#include "ChartHelper.h"

class CTargetItemDlg;

typedef CMap <HTREEITEM, HTREEITEM, int, int> CTreeMap;

#define IDT_AUTOPLAY		0x01
#define	IDT_AUTOSTART		0x02
#define IDT_AUTOSTOP		0x03
#define IDC_BAR		2001

/////////////////////////////////////////////////////////////////////////////
// CAxMConfigBar window

class AFX_EXT_CLASS CAxMConfigBar : public CWnd
{
	friend class CTargetItemDlg;
// Construction
public:
	CAxMConfigBar(CWnd* view, CWnd* parent, CFont* font, LPCTSTR info);

	BOOL	Create(CWnd* parent, LPRECT lpRect, UINT id);
	// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxMConfigBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAxMConfigBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAxMConfigBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnAxUser(WPARAM, LPARAM);
	afx_msg LRESULT OnAxDelKind(WPARAM, LPARAM);
	afx_msg LRESULT OnAxSetCtrl(WPARAM, LPARAM);
	afx_msg LRESULT OnAxReload(WPARAM, LPARAM);
	afx_msg LRESULT OnAxGetCtrl(WPARAM, LPARAM);
	afx_msg LRESULT OnAxDropItem(WPARAM, LPARAM);
	afx_msg LRESULT OnAxAddGraph(WPARAM, LPARAM);
	afx_msg LRESULT OnAxGetUserGraph(WPARAM, LPARAM);
	afx_msg LRESULT OnAxRMouse(WPARAM, LPARAM);
	afx_msg LRESULT OnAxItemClick(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual LRESULT OnAxBarClick(WPARAM, LPARAM);
	virtual LRESULT OnAxTreeKindItemClick(WPARAM, LPARAM);
	virtual LRESULT OnAxListKindItemClick(WPARAM, LPARAM);
	virtual LRESULT OnAxPlayButtonClick(WPARAM, LPARAM);
	virtual LRESULT OnAxReversePlayButtonClick(WPARAM, LPARAM);
	virtual LRESULT OnAxPauseButtonClick(WPARAM, LPARAM);
	virtual LRESULT OnAxFFButtonClick(WPARAM, LPARAM);
	virtual LRESULT OnAxStopButtonClick(WPARAM, LPARAM);
	virtual LRESULT OnAxSpinButtonClick(WPARAM, LPARAM);
	virtual LRESULT OnAxAutoEditChange(WPARAM, LPARAM);
	virtual LRESULT OnAxAutoClick(WPARAM, LPARAM);
	virtual LRESULT OnAxManualClick(WPARAM, LPARAM);
	virtual LRESULT OnAxCheckAllButtonClick(WPARAM, LPARAM);
	virtual LRESULT OnAxTargetItemButtonClick(WPARAM, LPARAM);
	virtual LRESULT OnAxInterItemClick(WPARAM, LPARAM);
	virtual LRESULT	OnAxFeatureItemClick(WPARAM, LPARAM);
	virtual LRESULT OnAxHaveItemClick(WPARAM, LPARAM);
	virtual LRESULT OnAxTreeJPItemClick(WPARAM, LPARAM);
protected:
	CWnd	*m_view, *m_parent;
	CFont	*m_font;
	
	void	DeleteChart(int delKind);
	void	ReloadGraph(int kind);
	void	ReloadGraph(LPSTR pGraph);
	void	AddGraph(int kind);
	LPCTSTR	GetUserGraph();
	virtual	void ChangePalette();
public:
	CAxTargetItem		*m_targetItems;
	CAxMBarCtrl			*m_bar;
	CAxMTreeCtrl		*m_treeJP, *m_treeInter, *m_treeHave, *m_treeKind;
	CAxMListCtrl		*m_listFeature,   *m_listKind;
	CAxMStaticCtrl		*m_panel;
	
	CAxMStaticCtrl*		m_autoLabel;
	CAxMStaticCtrl*		m_autoGroup;
	CAxMRadioButton*	m_manual;
	CAxMRadioButton*	m_auto;
	CAxMEdit*		m_autoInterval;
	CAxMSpinCtrl*		m_spin;
	CAxMSpeedButton		*m_btnPlay, *m_btnReversePlay, *m_btnPause, *m_btnFF, *m_btnStop;
	CAxMSpeedButton		*m_btnTargetItem, *m_btnCheckAll;
	
	HTREEITEM			m_hTreeJP, m_hTreeYH;
protected:
	//** for 지표
	int					m_graphCount;
	int					m_regionCount;
	LPSTR				m_graphInfo;
	BYTE				m_userGraph[MAX_UGRP];
	HTREEITEM			m_hsub[8];
	
	//** for ChartKind (유형)
	int					m_currentChartKind;
	void	InitBar();
	void	InitTreeControls();
	void	InitListControls();
	void	InitListControl(CAxMListCtrl* list, BOOL bCheckStyle);
	void	InitItem_YH();
	void	InitItem_JP();
	int		GetChartKindIndex(int group, int baseIndex);
	
	void	ResizeBar(LPRECT lpRect = NULL);
	BOOL	GetCheckAlreadyChartKind(LPCTSTR ginfo, int kind);
	
	CTreeMap			m_jpList;  // 지표리스트
	CTreeMap			m_ujpList; // 사용자지표리스트
	CTreeMap			m_yhlist;  // 차트유형리스트
	CTypedPtrList<CPtrList, CImageList*> m_imageList;
	
	COLORREF GetColor(UINT index);
	
	void	SetImageList2Tree(CAxMTreeCtrl* tree, LPCTSTR imagePath);
	
	void	EnableChartKindAid(int index);
	void	SelectYH(int index);
	LRESULT	SelectJP(HTREEITEM hItem, int index, CTreeMap& apply);
	void	SetCheckJP(CTreeMap& list, int kind, BOOL bCheck);
	
	void	ResizeListColumn(CAxMListCtrl* list);
	
	int		GetSelectItemCount();
	CString	GetSelectItems();
	void    SaveSelectedItems();
	
	void	SetListKindItemCheck(BOOL check);
	void	SetListKindItemCheck(int index, BOOL check);
	BOOL	GetListKindItemCheck(int index);
	CString GetImageName(LPCTSTR image);
	
	KIND	m_lastKind;
	void	ProcessTargetItem();
	CString GetSelectedItemString(CAxMTreeCtrl* tree);
protected:
	int		m_autoPlayInterval;
	int		m_autoIndex;
	void	SetAutoPlayButtons(CAxMSpeedButton* btn, LPCTSTR imageName, BOOL check = FALSE);
	virtual void	StartAutoPlay();
	virtual void	StopAutoPlay();
	int		GetNextCheckItem(int index);
	int		GetPrevCheckItem(int index);
	void	SelectKindItem(int index);
	void	SelectItem(UINT kind, LPCTSTR code);
	void	OnCfgButtonChange(int index);
protected:
	CChartKindList  m_chartList;

	// abstact virtual functions
	virtual void	GetMenunameBookmark(CString& menuname)  = 0;
	virtual void	GetMenunames(CStringList& strList)  = 0;
	virtual void	GetChartKindList(CChartKindList& chartList)  {}
	virtual CString GetRoot()       = 0;
	virtual CString GetImagePath()  = 0;
	virtual CString GetChartPath()  = 0;
	virtual CString GetTabPath()    = 0;

	virtual HTREEITEM	SetGroupList(CTreeCtrl* tree, LPCTSTR selectKind = "")  = 0;
	virtual void		SetGroupItem(CTreeCtrl* tree, HTREEITEM hItem)  = 0;

	virtual void	LoadInterestItems(CAxMTreeCtrl* tree)  = 0;
	virtual void	LoadHaveItems(CAxMTreeCtrl* tree)  = 0;
	virtual void	LoadFeatureItems(CAxMListCtrl* list)  = 0;

	virtual void	OnSelectItem(LPCTSTR kind_cd)  = 0;
	virtual void	OnTargetItemUserConfig() = 0;

	virtual void	RequestFeatureItems(){}
	virtual void	RequestHaveItem(HTREEITEM hItem){}

	virtual void	RefreshInterestItem() {}
public:
	virtual CString GetCodeName(LPCTSTR code) { return EMPTYSTR; }
	virtual CString GetTargetItemFileName() { return CString(""); }

	CWnd* GetView() const { return m_view; }
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXMCONFIGBAR_H__A23CE059_3B7B_4CE8_9992_ABE653A1124D__INCLUDED_)
