#if !defined(AFX_GRIDORDERWND_H__2414FA13_F54E_4170_8B8A_BF62D4CE4473__INCLUDED_)
#define AFX_GRIDORDERWND_H__2414FA13_F54E_4170_8B8A_BF62D4CE4473__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridOrderWnd.h : header file
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
#include "ConditionGrid.h"
#include "../../../SrcSite/control/fx_misc/fxImgButton.h"

/////////////////////////////////////////////////////////////////////////////
// CGridOrderWnd window
class CGridOrderWnd : public CWnd
{
public:
	CGridOrderWnd();
	virtual ~CGridOrderWnd();

private:
	CFont m_fontDefault, m_fontBold;

	//# System
public:
	CWnd			*m_pParentWnd;
	int				m_nGroupNo;			//# Just! => Group No

	BOOL			m_bFocus;

	//# System val
public :
	int m_nStartBongVal;
	int m_nEndBongVal;

	// Attributes
public:
	CConditionGrid			m_ctrlGrid;

	CStatic			m_ctrlFooter;
	CStatic			m_btnLabel;

	CStatic			m_ctrlGroupNo;
	CEdit				m_editStartBong;
	CStatic			m_ctrlText1;
	CEdit				m_editEndBong;
	CStatic			m_ctrlText2;

// 	CBitmapButton		m_btnUpInsertGroup;
// 	CBitmapButton		m_btnDownInsertGroup;
// 	CBitmapButton		m_btnDeleteGroup;
// 	CBitmapButton		m_btnAddGroup;

	CfxImgButton		m_btnUpInsertGroup;
	CfxImgButton		m_btnDownInsertGroup;
	CfxImgButton		m_btnDeleteGroup;
	CfxImgButton		m_btnAddGroup;

	BOOL m_bHightlight;

	CRect				m_reBackGround;
	CRect				m_reBtn;

	// Operations
public:
	int SetAddCondition(BOOL		bState,						//# Condition View
		int			nElementIndex,	//# Element Index
		BOOL		bNot,						//# Not State 
		int			nTargetType,		//# Target Type
		int			nTargetIndex,		//# Target Index
		CString strItem,				//# 완성 구문 
		BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
		BOOL		bFieldVal);			//# 값보기

	int SetInsertCondition(int nRow,
		BOOL		bState,					//# Condition View
		int			nElementIndex,	//# Element Index
		BOOL		bNot,						//# Not State 
		int			nTargetType,		//# Target Type
		int			nTargetIndex,		//# Target Index
		CString strItem,				//# 완성 구문 
		BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
		BOOL		bFieldVal);			//# 값보기

	int SetModifyCondition(int nRow,
		BOOL		bState,					//# Condition View
		CString	strElement,			//# Element
		BOOL		bNot,						//# Not State 
		int			nTargetType,		//# Target Type
		int			nTargetIndex,		//# Target Index
		CString strItem,				//# 완성 구문 
		BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
		BOOL		bFieldVal);			//# 값보기

	void SetModifyCondition(int nRow, BOOL bNot, CString strItem, BOOL bFieldVal);

	BOOL SetDeleteCondition(int nRow);

	CString GetCellDate(int nCol, int nRow);
	CString GetElementInfo(int nRow);

	void SetCellDate(int nCol, int nRow, CString strData);

	void SetFocus(BOOL bSendMsg = TRUE);
	void SetKillFocus();

	void SetFooterVal(int nStartBong, int nEndBong);
	void GetFooterVal(int &nStartBong, int &nEndBong);

	int GetWndTotHeight(BOOL bViewState = TRUE);

	void OnSize(CRect reClient);            //# Add : 2006.08
	void OnSize_Footer(CRect reClient);     //# Add : 2006.08
	void OnSize_Footer_Old(CRect reClient); //# Modify : 2006.08
	void ChangeSkinColor(ST_GRIDCOLOR stGridColor);


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridOrderWnd)
	//}}AFX_VIRTUAL

	afx_msg void OnChangeStartBong();
	afx_msg void OnChangeEndBong();

	afx_msg void OnSetFocusStartBong();
	afx_msg void OnSetFocusEndBong();

	afx_msg void OnKillFocusStartBong();
	afx_msg void OnKillFocusEndBong();

	afx_msg void OnClickGrid();

	// Implementation
public:


	// Generated message map functions
protected:
	//{{AFX_MSG(CGridOrderWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG

	afx_msg void OnUpInsertBtn();
	afx_msg void OnDownInsertBtn();
	afx_msg void OnDeleteBtn();
	afx_msg void OnAddBtn();

	afx_msg void OnMenuBtn();					//Added by cyberahn. @KOSCOM 2006.07.14.
	afx_msg void OnFooterClk();	//Added by cyberahn. @KOSCOM 2006.07.14.

	afx_msg LRESULT OnNotifyGridEvent(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDORDERWND_H__2414FA13_F54E_4170_8B8A_BF62D4CE4473__INCLUDED_)
