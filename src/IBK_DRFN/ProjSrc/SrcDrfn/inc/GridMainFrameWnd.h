#if !defined(AFX_GRIDMAINFRAMEWND_H__06DB84E6_C6EF_4667_8F76_878EE13E13C9__INCLUDED_)
#define AFX_GRIDMAINFRAMEWND_H__06DB84E6_C6EF_4667_8F76_878EE13E13C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridMainFrameWnd.h : header file
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

//# [기능 정의] ######################################################################
//# 1. 조건검색용 전용 그리드이다.
//# 2. 일반/순차조건을 동시에 구성할수 있는 기능
//# 3. Col은 그룹 확장/축소, 구분(조건선택, Element), 대상, 추가지표, _T("값"), 
//#				수정, 삭제 순서이다.
//# 4. 일반그룹과 순차 그룹은 Splitter로 구분하고 모든 Event는 메세지로 처리한다.
//# 5. 그외 기능은 아래의 주석을 참고하라.

/*# Define Event Message! ############################################################
#define WM_CALLPARENT												(WM_USER + 100)


#define NM_SELECT_GROUP												1		//#=>GroupNo
#define NM_SELECT_CONDITION										2		//# (0) : Group, (1) : Row

#define NM_CONDITION_DRAGDROP									5		//# (0) : Group, (1) : Old Row, (2) : New Row


#define NM_ALIGN_ELEMENT											30	//#=> Last Element Index


#define NM_VALVIEWALL_CHECK										10		//# (0) : Group, (1) : Val
#define NM_DELETEALL													11		//# Group

#define NM_CONDITION_CHECK										20		//# (0) : Group, (1) : Row, (2) : Val
#define NM_TARGET_LIST												21		//# (0) : Group, (1) : Row, (2) : Target Type, (3) : Target
#define NM_VALVIEW_CHECK											22		//# (0) : Group, (1) : Row, (2) : Val
#define NM_MODIFY_BTN													23		//# (0) : Group, (1) : Row, (2) : Display Row
#define NM_DELETE_BTN													24		//# (0) : Group, (1) : Row(After Select), (2) : TotHeight


#define NM_GROUP_ADD													40	//# Group	
#define NM_GROUP_INSERTUP											41	//# Group
#define NM_GROUP_INSERTDOWN										42	//# Group
#define NM_GROUP_DELETE												43	//# Group
//#####################################################################################*/


//# [History]
//# 2005.08.30
//#     [수정/추가] 
//#				: 그룹 및 Element의 자동 Indexing 기능 추가
//#					- void SetOrderUser(BOOL bTypeOrder);
//#					- void SetUserDragDrop(BOOL bState = FALSE);
//#					- void SetAlignGroup(BOOL bAutoGroupAlign = FALSE);






/////////////////////////////////////////////////////////////////////////////
// CGridMainFrameWnd frame
class CGridSplitterWnd;
class CGridCommonView;
class CGridOrderView;
class AFX_EXT_CLASS CGridMainFrameWnd : public CFrameWnd
{
public:
	CGridMainFrameWnd();
	virtual ~CGridMainFrameWnd();

private : 
	CGridSplitterWnd			*m_pGridSplitterWnd;
	CGridCommonView				*m_pViewCommonGrid;
	CGridOrderView				*m_pViewOrderGrid;

	int				m_nElementIndex;		//# 마지막 Element Index
	int				m_nTargetIndex;			//# 입력된 대상시간 Index (순차)

	int				m_nSelectGroupNo;		//# 선택 그룹
	int				m_nSelectRow;
public :
	CWnd			*m_pParentWnd;

public:
	void SetInit();		//# 초기화

	void SetSelectGroup(int nGroupNo, BOOL bMsgSend = TRUE);
	void SetAddGroup();									//# 그룹 추가
	void SetInsertGroup(int nGroupNo, BOOL bInsertPosUp);	//# 그룹 삽입 (Input : 선택그룹)
	void SetDeleteGroup(int nGroupNo);	//# 그룹 삭제 (Input : 선택그룹)

	int GetGroupCnt();									//# 그룹 개수
	int GetMaxGroupNo();								//# 마지막 그룹 번호
	int GetGroupRowCnt(int nGroupNo);		//# 선택그룹의 Row 개수
	int GetSelectGroup();

	//# 선택 그룹의 조건 데이터  삽입
	BOOL SetAddCondition(int nGroupNo, 
												BOOL		bState,					//# Condition View
												int			nElementIndex,	//# Element Index
												BOOL		bNot,						//# Not State 
												int			nTargetType,		//# Target Type
												int			nTargetIndex,		//# Target Index
												BOOL		bTargetEnable,	//# Target Enable
												CString strItem,				//# 완성 구문 
												BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
												BOOL		bFieldVal);			//# 값보기

	BOOL SetInsertCondition(int nRow,
												int nGroupNo, 
												BOOL		bState,					//# Condition View
												int			nElementIndex,	//# Element Index
												BOOL		bNot,						//# Not State 
												int			nTargetType,		//# Target Type
												int			nTargetIndex,		//# Target Index
												BOOL		bTargetEnable,	//# Target Enable
												CString strItem,				//# 완성 구문 
												BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
												BOOL		bFieldVal);			//# 값보기

	//# 선택 그룹/조건의 데이터 수정
	BOOL SetModifyCondition(int nRow,
												int nGroupNo, 
												BOOL		bState,					//# Condition View
												CString	strElement,			//# Element
												BOOL		bNot,						//# Not State 
												int			nTargetType,		//# Target Type
												int			nTargetIndex,		//# Target Index
												BOOL		bTargetEnable,	//# Target Enable
												CString strItem,				//# 완성 구문 
												BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
												BOOL		bFieldVal);			//# 값보기

	BOOL SetModifyCondition(int nRow, 
													int nGroupNo, 
													BOOL bNot,						//# Not State 
													CString strItem,			//# 완성 구문
													BOOL bFieldVal);

	BOOL SetDeleteCondition(int nRow, int nGroupNo);


	CString GetCellData(int nGroupNo, int nCol, int nRow);
	CString GetElementInfo(int nGroupNo, int nRow);
	void SetCellData(int nGroupNo, int nCol, int nRow, CString strData);

	BOOL GetFooterData(int nGroupNo, int &nStartBong, int &nEndBong);		//# 순차그룹의 Footer의 데이터 받기
	BOOL SetFooterData(int nGroupNo, int nStartBong, int nEndBong);			//# 순차그룹의 Footer의 데이터 설정
	
	int SetAlignElement();
	int GetTargetBase(int nGroupNo);
	void SetTargetBase(int nGroupNo, int nTargetIndex);
	void SetSameTargetBase(int nIndex, int nOutType = -1);

	void SetSelectRow(int nGroupNo, int nRow, BOOL bMsgSend = FALSE);
	void SetUnSelectRow(int nGroupNo, int nRow, BOOL bMsgSend = FALSE);
	

	void SetGroupViewState(BOOL bMinMax);		//# 그룹 최대/최소화 (TRUE : MIN, FALSE : MAX)
	void SetMoveWindow(int nHeight, BOOL bInvalidate = TRUE);


	//# Grid Setting Function ####################
	void SetOrderUser(BOOL bTypeOrder);
	void SetUserDragDrop(BOOL bState = FALSE);

	void SetAlignGroup(BOOL bAutoGroupAlign = FALSE);

private:
	int GetChange_Group(int nGroupNo);



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridMainFrameWnd)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGridMainFrameWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG

//	afx_msg void OnNotifyMainEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotifyMainEvent(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDMAINFRAMEWND_H__06DB84E6_C6EF_4667_8F76_878EE13E13C9__INCLUDED_)
