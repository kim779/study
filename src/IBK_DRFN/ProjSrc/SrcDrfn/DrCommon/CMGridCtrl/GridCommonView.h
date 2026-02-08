#if !defined(AFX_GRIDCOMMONVIEW_H__E3693578_72C2_43C4_808A_A762F2DB1AA6__INCLUDED_)
#define AFX_GRIDCOMMONVIEW_H__E3693578_72C2_43C4_808A_A762F2DB1AA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridView_Common.h : header file
//

/*##################################################################
##																																##
##								Condition Make Grid Control											##
##																																## 
##																																##
##									작성일 : 2003. 7 ~														##
##									작성자 : 우동우 (TEXEN SYSTEM)								##
##																																##  
###################################################################*/


#include "ConditionGrid.h"
#include "../../../SrcSite/control/fx_misc/fxImgButton.h"
/////////////////////////////////////////////////////////////////////////////
// CGridCommonView view

class CGridCommonView : public CView
{
protected:
	CGridCommonView();           // protected constructor used by dynamic creation
	virtual ~CGridCommonView();
	DECLARE_DYNCREATE(CGridCommonView)

	CFont m_fontDefault, m_fontBold;

public:
	CWnd		*m_pParentWnd;

	CConditionGrid			m_ctrlGrid;

	CStatic		m_ctrlFooter;
	CStatic		m_btnLabel;
	CStatic		m_staticTargetEditIndicate;	

	CBrush		m_brushLabel, m_brushLabelFocus;
	CBrush		m_brushFooter, m_brushFooter_Focus;
	CBrush		m_brushTagetEdit;

	COLORREF	m_clrLabel;	
	COLORREF	m_clrLabelFocus;	
	COLORREF	m_clrFooter;		
	COLORREF	m_clrFooter_Focus;

	CfxImgButton	m_btnChangeSubject;
	CfxImgButton	m_btnChangeSubjectInit;

	CRect		m_reBackGround;
	CRect		m_reBtn;
	CRect		m_reBtnInit;

public:
	void SetInit();

	void SetAddCondition(BOOL		bState,						//# Condition View
		int			nElementIndex,	//# Element Index
		BOOL		bNot,						//# Not State 
		int			nTargetType,		//# Target Type
		int			nTargetIndex,		//# Target Index
		BOOL		bTargetEnable,	//# Target Enable
		CString strItem,				//# 완성 구문 
		BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
		BOOL		bFieldVal);			//# 값보기

	void SetInsertCondition(int nRow,
		BOOL		bState,					//# Condition View
		int			nElementIndex,	//# Element Index
		BOOL		bNot,						//# Not State 
		int			nTargetType,		//# Target Type
		int			nTargetIndex,		//# Target Index
		BOOL		bTargetEnable,	//# Target Enable
		CString strItem,				//# 완성 구문 
		BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
		BOOL		bFieldVal);			//# 값보기

	void SetModifyCondition(int nRow,
		BOOL		bState,					//# Condition View
		CString	strElement,			//# Element Index
		BOOL		bNot,						//# Not State 
		int			nTargetType,		//# Target Type
		int			nTargetIndex,		//# Target Index
		BOOL		bTargetEnable,	//# Target Enable
		CString strItem,				//# 완성 구문 
		BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
		BOOL		bFieldVal);			//# 값보기

	void SetModifyCondition(int nRow, BOOL bNot, CString strItem, BOOL bFieldVal);

	BOOL SetDeleteCondition(int nRow);

	void SetGroupFocus();
	void SetGroupKillFocus();

	void SetSelectRow(int nRow);
	void SetUnSelectRow(int nRow, BOOL bDel = FALSE);

	void SetDetailViewState(BOOL bDetail);

	CString GetCellDate(int nCol, int nRow);
	CString GetElementInfo(int nRow);
	void SetCellDate(int nCol, int nRow, CString strData);

	int SetAlignElement();
	void SetSameTargetBase(int nIndex, int nOutType);

	void SetUserDragDrop(BOOL bState);

	void OnSize_Footer(CRect reClient);
	void ChangeSkinColor(ST_GRIDCOLOR stGridColor);

	void SetChagneTargetEditIndicate(CString strIndicate);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridCommonView)
protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

	// Implementation
protected:


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CGridCommonView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG

	afx_msg LRESULT OnNotifyGridEvent(WPARAM wParam, LPARAM lParam);
	afx_msg void OnFooterClk();
	afx_msg void OnChangeSubject();
	afx_msg void OnChangeSubjectInit();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDCOMMONVIEW_H__E3693578_72C2_43C4_808A_A762F2DB1AA6__INCLUDED_)
