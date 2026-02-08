#if !defined(AFX_GRIDORDERVIEW_H__63CE744A_4871_4FE1_9001_37D0C59AE49B__INCLUDED_)
#define AFX_GRIDORDERVIEW_H__63CE744A_4871_4FE1_9001_37D0C59AE49B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridView_Order.h : header file
//

/*####################################################################################
##																																									##
##															Condition Make Grid Control													##
##																																									## 
##																																									##
##																작성일 : 2003. 7 ~																##
##																작성자 : 우동우 (TEXEN SYSTEM)										##
##																																									##  
######################################################################################*/


#include <afxtempl.h>				//<= CList
#include "GridOrderWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CGridOrderView view

typedef struct tagOrderGroupInfo
{
	int						nIndex;

	int						nGroupNo;
	CGridOrderWnd *pOrderGridWnd;
	int						nHeight;
} GroupInfo;


class CGridOrderView : public CScrollView
{
protected:
	CGridOrderView();           // protected constructor used by dynamic creation
	virtual ~CGridOrderView();
	DECLARE_DYNCREATE(CGridOrderView)

	//# System Values ###########################
private :
	CWnd			*m_pParentWnd;

	BOOL m_bInsertPosUp;

	CPoint m_cpScrollPos;

	int	m_nResourceIndex;

	CSize		m_szTotViewSize;
	int			m_nTotHeightView;
	int			m_nLastGroupBottom;

	int m_nSelectGroup, m_nSelectRow;

public:
	CBrush    m_brushFooter, m_brushFooter_Focus;
	CBrush		m_brushLabel, m_brushLabelFocus;

	CList <GroupInfo, GroupInfo&> m_listGroupInfo;
	BOOL m_bAutoGroupAlign;		//#<= Group Auto Align

	COLORREF	m_clrLabel;
	COLORREF	m_clrLabelFocus;
	COLORREF	m_clrFooter;
	COLORREF	m_clrFooter_Focus;
	ST_GRIDCOLOR m_stGridColor;

public:
	void SetInit();

	void SetAddGroup(BOOL bGroupForcus = TRUE);
	void SetDeleteGroup(int nSelectGroupNo);
	BOOL SetInsertGroup(int nSelectGroupNo, BOOL bInsertPosUp);

	void SetModifyGroupInfo(int nSelectGroupNo, BOOL bDelete);

	BOOL SetAddCondition(int nSelectGroupNo, 
	BOOL		bState,					//# Condition View
	int			nElementIndex,	//# Element Index
	BOOL		bNot,						//# Not State 
	int			nTargetType,		//# Target Type
	int			nTargetIndex,		//# Target Index
	CString strItem,				//# 완성 구문 
	BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
	BOOL		bFieldVal);			//# 값보기

	BOOL SetInsertCondition(int nRow,
	int nSelectGroupNo, 
	BOOL		bState,					//# Condition View
	int			nElementIndex,	//# Element Index
	BOOL		bNot,						//# Not State 
	int			nTargetType,		//# Target Type
	int			nTargetIndex,		//# Target Index
	CString strItem,				//# 완성 구문 
	BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
	BOOL		bFieldVal);			//# 값보기

	BOOL SetModifyCondition(int nRow,
	int nSelectGroupNo, 
	BOOL		bState,					//# Condition View
	CString	strElement,			//# Element Index
	BOOL		bNot,						//# Not State 
	int			nTargetType,		//# Target Type
	int			nTargetIndex,		//# Target Index
	CString strItem,				//# 완성 구문 
	BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
	BOOL		bFieldVal);			//# 값보기

	BOOL SetModifyCondition(int nRow, int nSelectGroupNo,  
		BOOL bNot, CString strItem, BOOL bFieldVal);

	BOOL SetDeleteCondition(int nRow, int nSelectGroupNo);

	int GetMaxGroupNo();
	int GetMaxIndex();

	CRect GetSelectGroupRect(int nSelectGroupNo);
	int GetIndexToGroupNo(int nGroupIndex);
	int GetGroupNoToIndex(int nGroupNo);

	int GetChange_GroupNoToGroupIndex(int nGroupNo);


	BOOL FindIndexToGroupInfo(int nGroupNo, POSITION &pos, GroupInfo &stGroupInfo);
	BOOL FindIndexToGroupInfo(int nGroupNo, GroupInfo &stGroupInfo);

	void SetTargetAllSame(int nTargetType, int nTargetIndex);

	void SetGroupAlign();
	void SetGridMoveWindow(CRect reClient);
	void SetViewSize();

	void SetRedrewAll();

	int SetAlignElement(int nTotElementCnt);

	void SetGroupFocus(int nGroupNo);
	void SetGroupKillFocus(int nGroupNo);

	void SetSelectRow(int nGroupNo, int nRow);
	void SetUnSelectRow(int nGroupNo, int nRow);

	void SetDetailViewState(BOOL bDetail);

	CString GetCellDate(int nSelectGroupNo, int nCol, int nRow);
	CString GetElementInfo(int nSelectGroupNo, int nRow);

	void SetCellDate(int nSelectGroupNo, int nCol, int nRow, CString strData);

	void AllDeleteGroupInfo();
	void DeleteAll();

	void SetUserDragDrop(BOOL bState);

	//# Debug
	void DebugDataShow();
	void ChangeSkinColor(ST_GRIDCOLOR stGridColor);


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridOrderView)
protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

	// Implementation
protected:

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CGridOrderView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	afx_msg LRESULT OnNotifyWndEvent(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDORDERVIEW_H__63CE744A_4871_4FE1_9001_37D0C59AE49B__INCLUDED_)
