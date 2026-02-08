#if !defined(AFX_LogicEditCtrlCTRL_H__E1A1B90F_5BE6_4872_8C2C_E47BB818D034__INCLUDED_)
#define AFX_LogicEditCtrlCTRL_H__E1A1B90F_5BE6_4872_8C2C_E47BB818D034__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogicEditCtrl.h : header file
/////////////////////////////////////////////////////////////////////////////
// CLogicEditCtrl window
#include "Imm.h"
#include <afxtempl.h>   //<= CList

#include "../DefineFiles/client_typedef.h"

/*
typedef struct tagElement 
{
	CString   strOperation;
	CString   strNot;
	CString   strElement;
	
	BOOL      bView;
	
	CString   strBracket;
	LONG      lBracketCount;
} OneElementInfo;

typedef struct tagGroupElement
{
	int nGroup;
	OneElementInfo stOneElementInfo;
} GroupElementInfo;
*/
class CLogicEditCtrl : public CRichEditCtrl
{
private:
  CFont m_fontDefault;
	
public:
	CLogicEditCtrl();
	virtual ~CLogicEditCtrl();
	
public:
	//## system values ##
	BOOL m_bAutoElemet;  //# TRUE : 자동
	//# FALSE : 고정
	
	BOOL      m_bUseGroup;   //# TRUE : Group  <= 순차
	//# FALSE : not Group <= 일반
	
	CList<OneElementInfo, OneElementInfo&> m_listOneElement;
	CList<GroupElementInfo, GroupElementInfo&> m_listGroupElement;

	CList<OneElementInfo, OneElementInfo&> m_listOneElementUser;
	
// 2009.11.10 -hidden-
	ArrGroupElements		m_arrGroupElement;

	//## Bracket Editing ##
	BOOL m_bCaptureEnabled;
	
	BOOL m_bSelectDuring;
	BOOL m_bMouseOver;
	BOOL m_bSelectState;
	
	int m_nSelectStartPosition;
	int m_nSelectEndPosition;
	
	BOOL m_bHaveBracket;
	BOOL m_bHaveCaret;
	
	//## Cursor ##
	HCURSOR m_hOperationSelectCursor;
	HCURSOR m_hSelectDuringCursor;
	HCURSOR m_hCursor;
	
	
	// Operations
public:
	BOOL BracketSelectMake();
	BOOL BracketSystemMake();
	BOOL BracketDelete();
	void BrecketSelectCreate();
	void AllAndOrChange(BOOL bAnd);
	
	CSize GetFontSize();
	BOOL SetErrorMessageBox(CString strErrorMsg, BOOL bFocus = FALSE);
	
	//## Set Text Color ##
	int m_noldStartOperation;
	int m_noldEndOperation;
	void SetCStringColor();
  CHARFORMAT GetFontFormat(COLORREF clrColor);
  void DeleteAllBracket();
		
	
	//## 2002. 10 ~
	
	void AddOneElement(int nElementIndex, int nOperation = 0, BOOL bNot = FALSE);
	void AddGroupElement(int nGroup, int nElementIndex, int nOperation = 0, BOOL bNot = FALSE);
	// 2009.12.04 -hidden-
	void AddGroupElement(GroupElementInfo& stNewInfo);

	void InsertOneElement(CString strElement, int nElementIndex, int nOperation = 0, BOOL bNot = FALSE);
	void InsertGroupElement(CString strElement, int nGroup, int nElementIndex, int nOperation = 0, BOOL bNot = FALSE);
	
	BOOL DeleteElement(CString strElement, BOOL bDelete = TRUE);
	BOOL DeleteOneElement(CString strElement, BOOL bDelete);
	BOOL DeleteGroupElement(CString strElement, BOOL bDelete = TRUE);
	
	BOOL ModifyElementView(CString strElement, BOOL bView);
	BOOL ModifyOneElementView(CString strElement, BOOL bView);
	BOOL ModifyGroupElementView(CString strElement, BOOL bView);
	
	BOOL ModifyElementNot(CString strElement, BOOL bNot);
	BOOL ModifyOneElementNot(CString strElement, BOOL bNot);
	BOOL ModifyGroupElementNot(CString strElement, BOOL bNot);
	
	BOOL ModifyElementInsertGroup(int nGroup);
	BOOL ModifyElementDeleteGroup(int nGroup);

	// 2009.11.10 -hidden-	
	BOOL ModifyElementInsertUpGroup(int nIndex, int nGroup);
	BOOL ModifyElementInsertDownGroup(int nIndex, int nGroup);
	void MakeGroupStruct(int nIndex, int nGroup);
	
	void ElementPositionMove(int FrontIndex, int AfterIndex); 
	
	void ReLoadAll();
	void RollBack();
	void DeleteAll();
	void DeleteGroupAll(int nGroup);
	
	POSITION ListSearchPosition(CList<OneElementInfo, OneElementInfo> &beforList, 
								 CString strElement);
	
	int PointToStrPosition(CPoint point);
	void SelectFieldCheck();
	
	int GetMaxGroup();
	int GetGroupToConditionCnt(int nGroup);
	
	int GetElementCount();   //## Input Data Count 
	BOOL GetNowElement(CStringArray &saElement);
	POSITION GetOneElementToPosition(CString strElement);
//	POSITION GetGroupElementToPosition(CString strElement);
// 2009.11.13 -hidden-
	BOOL GetGroupElementToPosition(CString strElement, BOOL bView);	

	//## Grammar Check ##
	BOOL CheckMakeBracketOK(CString strSelect);
	
	
	void GetLogicString(CString &strLogic, int nStepCnt, 
						 CStringArray &strarrayOperater);
	void GetLogicPacketOneString(CString &strLogic);
	void GetLogicPacketGroupString(CString &strGroupLogic);
	void GetLogicElement(CStringArray& arrElement, CStringArray& arrOpt);
	
	
	BOOL GetHaveBracket();
	BOOL GetElementOverlap();
	
	int SetOneElementStandLine();
	int SetGroupElementStandLine(int nElementIndex);
		
	//## Debug ##
	void DebugViewElement(OneElementInfo  &stElement);
	void AddElementOnMinDownBtn(CString strElement, CString strElementAfter);
	void InsertElementOnMaxUpBtn(CString strElement, CString strElementAfter);
	void SwitchElement(CString strElement, CString strElementAfter);

	void AddGroupElementOnMinDownBtn(CString strElement, CString strElementAfter);
	void InsertGroupElementOnMaxUpBtn(CString strElement, CString strElementAfter);
	void SwitchGroupElement(CString strElement, CString strElementAfter);
	
	// 2009.11.13 -hidden-
	GROUP_ELEMINFO* FindGroupInfoWithGroupNumber(int nGroup);
	BOOL  CheckIsValidLogic(CString strLogic, int& nIndex);

	CString GetElementOperation(CString strElement);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogicEditCtrl)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// Generated message map functions
protected:
	//{{AFX_MSG(CLogicEditCtrl)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnDestroy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	//}}AFX_MSG
	
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LogicEditCtrl_H__E1A1B90F_5BE6_4872_8C2C_E47BB818D034__INCLUDED_)

