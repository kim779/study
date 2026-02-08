// CMGrid.cpp : implementation file
//

#include "stdafx.h"
#include "CMGrid.h"

#include "../Dr_Control/DrSplitterCtrl.h"
#include "GridCommonView.h"
#include "GridOrderView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*####################################################################################
##																																									##
##															Condition Make Grid Control													##
##																																									## 
##																																									##
##																작성일 : 2003. 7 ~																##
##																작성자 : 우동우 (TEXEN SYSTEM)										##
##																																									##  
######################################################################################*/
#define DEFULT_COMMVIEW_HEIGHT      50

/////////////////////////////////////////////////////////////////////////////
// CCMGrid
CCMGrid::CCMGrid()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);

	m_pctrlSplitter = NULL;
	m_pViewCommonGrid = NULL;
	m_pViewOrderGrid = NULL;

	m_nGridType = -1;
	m_bDetail = FALSE;
	m_bDel = FALSE;
}

CCMGrid::~CCMGrid()
{
	m_fontDefault.DeleteObject();

	//070423월 m_pctrlSplitter 삭제루틴추가
	if (m_pctrlSplitter != NULL)
	{
		if( IsWindow(m_pctrlSplitter->m_hWnd))
			m_pctrlSplitter->DestroyWindow();

		delete m_pctrlSplitter;
		m_pctrlSplitter = NULL;
	}
}

BEGIN_MESSAGE_MAP(CCMGrid, CFrameWnd)
	//{{AFX_MSG_MAP(CCMGrid)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_MAIN_EVENT, OnNotifyMainEvent)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMGrid message handlers

int CCMGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_pParentWnd = this;
  m_pParentWnd = m_pParentWnd->GetParent();
	
	return 0;
}

void CCMGrid::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	
	if (m_pctrlSplitter != NULL)
	{
		delete m_pctrlSplitter;
		m_pctrlSplitter = NULL;
	}
}

BOOL CCMGrid::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if (m_pctrlSplitter == NULL)
	{
    m_pctrlSplitter = new CDrSplitterCtrl;
		if (!m_pctrlSplitter->CreateStatic(this, 2, 1)) 
			return FALSE;
	}

	m_pctrlSplitter->CreateView(0, 0, RUNTIME_CLASS(CGridCommonView), CSize(0, 0), pContext);
  m_pctrlSplitter->CreateView(1, 0, RUNTIME_CLASS(CGridOrderView), CSize(1, 0), pContext);
  
	m_pViewCommonGrid = (CGridCommonView *)m_pctrlSplitter->GetPane(0, 0);
  m_pViewOrderGrid = (CGridOrderView *)m_pctrlSplitter->GetPane(1, 0);
	
	m_pctrlSplitter->m_nResizeRow = -1;
	m_pctrlSplitter->m_bFirstSizing = true;
	m_pctrlSplitter->m_upBorder = 0;
	m_pctrlSplitter->SetSplitBarCx(3);
	m_pctrlSplitter->RecalcLayout();

	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

/*
void CCMGrid::SetMoveWindow(int nHeight, BOOL bInvalidate)
{
  CRect reClient;
	GetClientRect(reClient);

  int nHeight_Comm;
	nHeight_Comm = (nHeight - 10) / 4;

	if (nHeight_Comm < 40)
		nHeight_Comm = 40;

	m_pctrlSplitter->SetRowInfo(0, nHeight_Comm, 0);
	m_pctrlSplitter->SetRowInfo(1, nHeight - nHeight_Comm, 0);

	if (bInvalidate == TRUE)
		m_pctrlSplitter->RecalcLayout();
}
*/

void CCMGrid::SetSelectGroup(int nGroupNo, BOOL bMsgSend)
{
	if (bMsgSend == TRUE)
	{
		if (m_nSelectGroupNo == nGroupNo)
			return;
	}
	
	TRACE("\n# After Group : %d, Now Group : %d", m_nSelectGroupNo ,nGroupNo);

	if (m_nSelectGroupNo > 0)
		m_pViewOrderGrid->SetGroupKillFocus(m_nSelectGroupNo);
	else
		m_pViewCommonGrid->SetGroupKillFocus();
			
	if (nGroupNo > 0)
		m_pViewOrderGrid->SetGroupFocus(nGroupNo);
	else
		m_pViewCommonGrid->SetGroupFocus();
	
	if (bMsgSend == TRUE)
	{
//		int nGroup = GetChange_Group(nGroupNo);
//		m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_SELECT_GROUP, (int)nGroup);

		// 2009.11.09 -hidden-
		int nIndex = GetChange_Group(nGroupNo);
		long nParam = MAKELONG((WORD)nIndex, (WORD)nGroupNo);

		m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_SELECT_GROUP, (LPARAM)nParam);
	}
	m_nSelectGroupNo = nGroupNo;
}

int CCMGrid::GetChange_Group(int nGroupNo)
{
	int nResult = 0; 

	if (nGroupNo > 0)
		nResult = m_pViewOrderGrid->GetChange_GroupNoToGroupIndex(nGroupNo);
	
	return nResult;
}

LRESULT CCMGrid::OnNotifyMainEvent(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case NM_SELECT_GROUP :	//# Group
			{
				int nGroupNo = (int)lParam;
				SetSelectGroup(nGroupNo);
			}
			break;

		case NM_SELECT_CONDITION :	//#=> (0) : Group, (1) : Row	(2) : Add Falg
			{
				CUIntArray *piaData;
				piaData = (CUIntArray *)lParam;
				
				SetSelectRow(piaData->GetAt(0), piaData->GetAt(1), TRUE);
			}
			break;

		case NM_UPDATE :
			m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_UPDATE, NULL);
			break;
		
		case NM_ALIGN_ELEMENT :	//# (0) : Type, (1) : TotElementIndex
			{
				CUIntArray	*piaData;
				piaData = (CUIntArray*)lParam;		

				int nType = piaData->GetAt(0);
				int nTotElementIndex = piaData->GetAt(1);

				if (nType == 0)	//# Common
				{
					m_nElementIndex = 0;
					m_pViewOrderGrid->SendMessage(WM_WND_EVENT, (WPARAM)NM_ALIGN_ELEMENT, (int)nTotElementIndex);
				}
				else if (nType == 1)	//# Group
					m_pViewCommonGrid->SendMessage(WM_GRID_EVENT, (WPARAM)NM_ALIGN_ELEMENT, (LPARAM)0);
				else if (nType == 2)
				{
					m_nElementIndex = nTotElementIndex;
					m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_ALIGN_ELEMENT, (int)m_nElementIndex);
				}
			}
			break;

		case NM_VALVIEWALL_CHECK :		//# (0) : Group, (1) : Val
			{
				CUIntArray *piaData;
				piaData = (CUIntArray *)lParam;

				int nGroup = GetChange_Group(piaData->GetAt(0));
				piaData->SetAt(0, nGroup);
				
				m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_VALVIEWALL_CHECK, (LPARAM)piaData);
			}
			break;

		case NM_DELETEALL :		//# Group
			{
				int nGroupNo = (int)lParam;
				SetSelectGroup(nGroupNo, TRUE);
				
				//m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_DELETEALL, (LPARAM)GetChange_Group(nGroupNo));
				
				// 2009.11.13 -hidden-
				m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_DELETEALL, (LPARAM)lParam);
			}
			break;

		case NM_CONDITION_CHECK :		//# (0) : Group, (1) : Row, (2) : Val
			{
				CUIntArray	*piaData;
				piaData = (CUIntArray*)lParam;

				SetSelectRow(piaData->GetAt(0), piaData->GetAt(1));

				int nGroup = GetChange_Group(piaData->GetAt(0));
				piaData->SetAt(0, nGroup);

				m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_CONDITION_CHECK, (LPARAM)piaData);
			}
			break;

		case NM_TARGET_LIST :	//# (0) : Group, (1) : Row, (2) : Target Type, (3) : Target
			{
				CUIntArray	*piaData;
				piaData = (CUIntArray*)lParam;

				SetSelectRow(piaData->GetAt(0), piaData->GetAt(1));
				m_nTargetIndex = piaData->GetAt(3);

				int nGroup = GetChange_Group(piaData->GetAt(0));
				piaData->SetAt(0, nGroup);
				
				m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_TARGET_LIST, (LPARAM)piaData);
			}
			break;

		case NM_VALVIEW_CHECK :		//# (0) : Group, (1) : Row, (2) : Val
			{
				CUIntArray	*piaData;
				piaData = (CUIntArray*)lParam;

				SetSelectRow(piaData->GetAt(0), piaData->GetAt(1));
				
				int nGroup = GetChange_Group(piaData->GetAt(0));
				piaData->SetAt(0, nGroup);

				m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_VALVIEW_CHECK, (LPARAM)piaData);
			}
			break;

		case NM_MODIFY_BTN :		//# (0) : Group, (1) : Row, (2) : Element Index
			{
				CUIntArray	*piaData;
				piaData = (CUIntArray*)lParam;

				SetSelectRow(piaData->GetAt(0), piaData->GetAt(1));
				
				int nGroup = GetChange_Group(piaData->GetAt(0));
				piaData->SetAt(0, nGroup);

				m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_MODIFY_BTN, (LPARAM)piaData);
			}
			break;

		case NM_DELETE_BTN :		//# (0) : Group, (1) : Row(After Select), (2) : Element Index
			{
				CUIntArray	*piaData;
				piaData = (CUIntArray*)lParam;

				m_bDel = TRUE;
				SetSelectRow(piaData->GetAt(0), piaData->GetAt(1), TRUE);
				m_bDel = FALSE;

				int nGroup = GetChange_Group(piaData->GetAt(0));
				piaData->SetAt(0, nGroup);

				m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_DELETE_BTN, (LPARAM)piaData);
			}
			break;

		case NM_GROUP_INSERTUP :	//# (0) : Group Index (1) : Group
			{
				CUIntArray	*piaData;
				piaData = (CUIntArray*)lParam;
/*
				if (m_pViewOrderGrid->m_bAutoGroupAlign == TRUE)
					m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_GROUP_INSERTDOWN, (LPARAM)piaData->GetAt(1));
				else
					m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_GROUP_INSERTDOWN, (LPARAM)piaData->GetAt(0));
*/
				// 2009.11.09 -hidden-
				long nParam = MAKELONG((WORD)piaData->GetAt(0), (WORD)piaData->GetAt(1));
				m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_GROUP_INSERTUP, (LPARAM)nParam);

				SetSelectGroup(piaData->GetAt(1), FALSE);
			}
			break;

		case NM_GROUP_INSERTDOWN :	//# (0) : Group Index (1) : Group
			{
				CUIntArray	*piaData;
				piaData = (CUIntArray*)lParam;

/*				if (m_pViewOrderGrid->m_bAutoGroupAlign == TRUE)
					m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_GROUP_INSERTDOWN, (LPARAM)piaData->GetAt(1));
				else
					m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_GROUP_INSERTDOWN, (LPARAM)piaData->GetAt(0));
*/
				// 2009.11.09 -hidden-
				long nParam = MAKELONG((WORD)piaData->GetAt(0), (WORD)piaData->GetAt(1));
				m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_GROUP_INSERTDOWN, (LPARAM)nParam);

				SetSelectGroup(piaData->GetAt(1), FALSE);
			}
			break;

		case NM_GROUP_DELETE :	//# (0) : Group (1) : Focus Group
			{
				CUIntArray	*piaData;
				piaData = (CUIntArray*)lParam;

				int nGroup = piaData->GetAt(0);
				m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_GROUP_DELETE, (LPARAM)nGroup);

				SetSelectGroup(piaData->GetAt(1));
			}
			break;
		case NM_ROW_UP_BTN :
			{
				CUIntArray	*piaData;
				piaData = (CUIntArray*)lParam;

	//			SetSelectRow(piaData->GetAt(0), piaData->GetAt(1), TRUE);

				int nGroup = GetChange_Group(piaData->GetAt(0));
				piaData->SetAt(0, nGroup);
				m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_ROW_UP_BTN, (LPARAM)piaData);
			}
			break;
		case NM_ROW_DOWN_BTN :
			{
				CUIntArray	*piaData;
				piaData = (CUIntArray*)lParam;

	//			SetSelectRow(piaData->GetAt(0), piaData->GetAt(1), TRUE);

				int nGroup = GetChange_Group(piaData->GetAt(0));
				piaData->SetAt(0, nGroup);
				m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_ROW_DOWN_BTN, (LPARAM)piaData);
			}
			break;
		case NM_ROW_MAXUP_BTN :
			{		
				CStringArray* psaData = (CStringArray*)lParam;
				CString strGroupNo;
				int nGroup = GetChange_Group(atoi(psaData->GetAt(0)));
				strGroupNo.Format("%d", nGroup);
				psaData->SetAt(0, strGroupNo);
				m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_ROW_MAXUP_BTN, (LPARAM)psaData);
			}
			break;
		case NM_ROW_MINDOWN_BTN :
			{	
				CStringArray* psaData = (CStringArray*)lParam;
				CString strGroupNo;
				int nGroup = GetChange_Group(atoi(psaData->GetAt(0)));
				strGroupNo.Format("%d", nGroup);
				psaData->SetAt(0, strGroupNo);
				m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_ROW_MINDOWN_BTN, (LPARAM)psaData);
			}
			break;
		case NM_CHANGE_SUBJECT :
			{
				m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_CHANGE_SUBJECT, NULL);
			}
			break;
		case NM_CHANGE_SUBJECT_INIT :
			{
				m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_CHANGE_SUBJECT_INIT, NULL);
			}
      /*
		case NM_TAB_CHANGE:		//Added by cyberahn@KOSCOM 2006.07.24. 일반/순차 선택시 지표선택탭변환이벤트.
			{
				m_pParentWnd->PostMessage(WM_NOTIFY_CMGRIDEVENT, (WPARAM)NM_TAB_CHANGE, (LPARAM)lParam);
			}
			break;
      */

		default :
			break;
	}
	return TRUE;
}

void CCMGrid::SetAddGroup(BOOL bGroupForcus)
{
	m_pViewOrderGrid->SetAddGroup(bGroupForcus);
}

void CCMGrid::SetInsertGroup(int nGroupNo, BOOL bInsertPosUp)
{
	if (nGroupNo > 0)
		m_pViewOrderGrid->SetInsertGroup(nGroupNo, bInsertPosUp);
}

void CCMGrid::SetDeleteGroup(int nGroupNo)
{
	if (nGroupNo > 0)
		m_pViewOrderGrid->SetDeleteGroup(nGroupNo);
}

int CCMGrid::GetGroupCnt()
{
	int nResult = -1;

	nResult = m_pViewOrderGrid->m_listGroupInfo.GetCount();

	return nResult;
}

int CCMGrid::GetMaxGroupNo()
{
	int nResult = -1;

	nResult = m_pViewOrderGrid->GetMaxGroupNo();

	return nResult;
}

int CCMGrid::GetSelectGroup()
{
	return GetChange_Group(m_nSelectGroupNo);
}

int CCMGrid::GetGroupRowCnt(int nGroupNo)
{
	int nResult = 0;

	if (nGroupNo == 0)
		nResult = m_pViewCommonGrid->m_ctrlGrid.GetNumberRows();
	else
	{
		GroupInfo stGroupInfo;

		if (m_pViewOrderGrid->FindIndexToGroupInfo(nGroupNo, stGroupInfo) == TRUE)
			nResult = stGroupInfo.pOrderGridWnd->m_ctrlGrid.GetNumberRows();
	}

	return nResult;
}

BOOL CCMGrid::SetAddCondition(int nGroupNo, 
																				BOOL		bState,					//# Condition View
																				int			nElementIndex,	//# Element Index
																				BOOL		bNot,						//# Not State 
																				int			nTargetType,		//# Target Type
																				int			nTargetIndex,		//# Target Index
																				BOOL		bTargetEnable,	//# Target Enable
																				CString strItem,				//# 완성 구문 
																				BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
																				BOOL		bFieldVal)			//# 값보기
{
	BOOL bResult = FALSE;

	/*
	BOOL		bState = TRUE;					//# Condition View
	int			nElementIndex = 1;	//# Element Index
	BOOL		bNot = FALSE;						//# Not State 
	int			nTargetType = 1;		//# Target Type
	int			nTargetIndex = 0;		//# Target Index
	CString strItem = "123123";				//# 완성 구문 
	BOOL		bFieldState = TRUE;		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
	BOOL		bFieldVal = FALSE;			//# 값보기
	*/

	if (nGroupNo == 0)
	{
		m_pViewCommonGrid->SetAddCondition(bState,					//# Condition View
																				nElementIndex,	//# Element Index
																				bNot,						//# Not State 
																				nTargetType,		//# Target Type
																				nTargetIndex,		//# Target Index
																				bTargetEnable,	//# Target Enable
																				strItem,				//# 완성 구문 
																				bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
																				bFieldVal);
		bResult = TRUE;
	}
	else
	{
		//# Target Index <= Order Group : TargetIndex All Same
		if (m_pViewOrderGrid->SetAddCondition(nGroupNo, 
																					bState,					//# Condition View
																					nElementIndex,	//# Element Index
																					bNot,						//# Not State 
																					nTargetType,		//# Target Type
																					m_nTargetIndex,	//#<= Target Index
																					strItem,				//# 완성 구문 
																					bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
																					bFieldVal) == TRUE)
			bResult = TRUE;
	}

	return bResult;
}

BOOL CCMGrid::SetInsertCondition(int nRow,
																						int nGroupNo, 
																						BOOL		bState,					//# Condition View
																						int			nElementIndex,	//# Element Index
																						BOOL		bNot,						//# Not State 
																						int			nTargetType,		//# Target Type
																						int			nTargetIndex,		//# Target Index
																						BOOL		bTargetEnable,	//# Target Enable
																						CString strItem,				//# 완성 구문 
																						BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
																						BOOL		bFieldVal)			//# 값보기
{
	BOOL bResult = FALSE;

	/*
	BOOL		bState = TRUE;					//# Condition View
	int			nElementIndex = 1;	//# Element Index
	BOOL		bNot = FALSE;						//# Not State 
	int			nTargetType = 1;		//# Target Type
	int			nTargetIndex = 0;		//# Target Index
	CString strItem = "123123";				//# 완성 구문 
	BOOL		bFieldState = TRUE;		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
	BOOL		bFieldVal = FALSE;			//# 값보기
	*/

	if (nGroupNo == 0)
	{
		m_pViewCommonGrid->SetInsertCondition(nRow,
																				bState,					//# Condition View
																				nElementIndex,	//# Element Index
																				bNot,						//# Not State 
																				nTargetType,		//# Target Type
																				nTargetIndex,		//# Target Index
																				bTargetEnable,	//# Target Enable
																				strItem,				//# 완성 구문 
																				bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
																				bFieldVal);
		bResult = TRUE;
	}
	else
	{
		//# Target Index <= Order Group : TargetIndex All Same
		if (m_pViewOrderGrid->SetInsertCondition(nRow,
																					nGroupNo, 
																					bState,					//# Condition View
																					nElementIndex,	//# Element Index
																					bNot,						//# Not State 
																					nTargetType,		//# Target Type
																					m_nTargetIndex,	//#<= Target Index
																					strItem,				//# 완성 구문 
																					bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
																					bFieldVal) == TRUE)
			bResult = TRUE;
	}

	return bResult;
}


BOOL CCMGrid::SetModifyCondition(int nRow,
																						int nGroupNo, 
																						BOOL		bState,					//# Condition View
																						CString	strElement,			//# Element Index
																						BOOL		bNot,						//# Not State 
																						int			nTargetType,		//# Target Type
																						int			nTargetIndex,		//# Target Index
																						BOOL		bTargetEnable,	//# Target Enable
																						CString strItem,				//# 완성 구문 
																						BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
																						BOOL		bFieldVal)			//# 값보기
{
	BOOL bResult = FALSE;

	/*
	BOOL		bState = TRUE;					//# Condition View
	int			nElementIndex = 1;	//# Element Index
	BOOL		bNot = FALSE;						//# Not State 
	int			nTargetType = 1;		//# Target Type
	int			nTargetIndex = 0;		//# Target Index
	CString strItem = "123123";				//# 완성 구문 
	BOOL		bFieldState = TRUE;		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
	BOOL		bFieldVal = FALSE;			//# 값보기
	*/

	if (nGroupNo == 0)
	{
		m_pViewCommonGrid->SetModifyCondition(nRow,
																				bState,					//# Condition View
																				strElement,			//# Element
																				bNot,						//# Not State 
																				nTargetType,		//# Target Type
																				nTargetIndex,		//# Target Index
																				bTargetEnable,	//# Target Enable
																				strItem,				//# 완성 구문 
																				bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
																				bFieldVal);
		bResult = TRUE;
	}
	else
	{
		//# Target Index <= Order Group : TargetIndex All Same
		if (m_pViewOrderGrid->SetModifyCondition(nRow,
																					nGroupNo, 
																					bState,					//# Condition View
																					strElement,			//# Element Index
																					bNot,						//# Not State 
																					nTargetType,		//# Target Type
																					m_nTargetIndex,	//#<= Target Index
																					strItem,				//# 완성 구문 
																					bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
																					bFieldVal) == TRUE)
			bResult = TRUE;
	}

	return bResult;
}


BOOL CCMGrid::SetModifyCondition(int nRow, int nGroupNo,
																						BOOL		bNot,						//# Not State 
																						CString strItem,				//# 완성 구문
																						BOOL bFieldVal)
{
	BOOL bResult = FALSE;

	if (nGroupNo == 0)
	{
		m_pViewCommonGrid->SetModifyCondition(nRow, bNot,	strItem, bFieldVal);
		bResult = TRUE;
	}
	else
	{
		if (m_pViewOrderGrid->SetModifyCondition(nRow, nGroupNo, bNot, strItem, bFieldVal) == TRUE)
			bResult = TRUE;
	}

	return bResult;
}

BOOL CCMGrid::SetDeleteCondition(int nRow, int nGroupNo)
{
	BOOL bResult = FALSE;

	if (nGroupNo == 0)
		bResult = m_pViewCommonGrid->SetDeleteCondition(nRow);
	else
		bResult = m_pViewOrderGrid->SetDeleteCondition(nGroupNo, nRow);

	return bResult;
}


CString CCMGrid::GetCellData(int nGroupNo, int nCol, int nRow)
{
	CString strResult = "";

	if (nGroupNo == 0)
		strResult = m_pViewCommonGrid->GetCellDate(nCol, nRow);
	else
		strResult = m_pViewOrderGrid->GetCellDate(nGroupNo, nCol, nRow);

	return strResult;
}


CString CCMGrid::GetElementInfo(int nGroupNo, int nRow)
{
	CString strResult = "";

	if (nGroupNo == 0)
		strResult = m_pViewCommonGrid->GetElementInfo(nRow);
	else
		strResult = m_pViewOrderGrid->GetElementInfo(nGroupNo, nRow);

	return strResult;
}

void CCMGrid::SetCellData(int nGroupNo, int nCol, int nRow, CString strData)
{
	if (nGroupNo == 0)
		m_pViewCommonGrid->SetCellDate(nCol, nRow, strData);
	else
		m_pViewOrderGrid->SetCellDate(nGroupNo, nCol, nRow, strData);
}

BOOL CCMGrid::SetFooterData(int nGroupNo, int nStartBong, int nEndBong)
{
	BOOL bResult = FALSE;

	if (nGroupNo > 0)
	{
		GroupInfo stGroupInfo;

		if (m_pViewOrderGrid->FindIndexToGroupInfo(nGroupNo, stGroupInfo) == TRUE)
		{
			stGroupInfo.pOrderGridWnd->SetFooterVal(nStartBong, nEndBong);
			bResult = TRUE;
		}
	}

	return bResult;
}

BOOL CCMGrid::GetFooterData(int nGroupNo, int &nStartBong, int &nEndBong)
{
	BOOL bResult = FALSE;

	if (nGroupNo > 0)
	{
		GroupInfo stGroupInfo;

		if (m_pViewOrderGrid->FindIndexToGroupInfo(nGroupNo, stGroupInfo) == TRUE)
		{
			stGroupInfo.pOrderGridWnd->GetFooterVal(nStartBong, nEndBong);
			bResult = TRUE;
		}
	}

	return bResult;
}


int CCMGrid::GetTargetBase(int nGroupNo)
{
	int nResult = 0;

	if (nGroupNo > 0)		//# 순차
		nResult = m_nTargetIndex;

	return nResult;
}


void CCMGrid::SetTargetBase(int nGroupNo, int nTargetIndex)
{
	if (nGroupNo > 0)		//# 순차
		m_nTargetIndex = nTargetIndex;
}

void CCMGrid::SetInit(int nType, BOOL bFocus)
{
	//# System Values Init
	m_nElementIndex = -1;
	m_nTargetIndex = -1;

	m_pViewCommonGrid->SetInit();
	m_pViewOrderGrid->SetInit();
	
	m_nSelectGroupNo = 0;
	m_nSelectRow = -1;

//  m_bDetail = FALSE;
  SetDetailViewState(m_bDetail);
    
  if ((nType == (int)TYPE_GRID_COMMON) && (m_nGridType != (int)TYPE_GRID_COMMON))
    SetGridType((int)TYPE_GRID_COMMON); 
  else if ((nType == (int)TYPE_GRID_ORDER) && (m_nGridType != (int)TYPE_GRID_ORDER))
  {
    SetGridType((int)TYPE_GRID_ORDER); 
    m_nSelectGroupNo = 1;
  }

  if (bFocus == TRUE)
    SetSelectGroup(m_nSelectGroupNo, FALSE);

  if (m_pctrlSplitter != NULL)
  {
    CRect reClient;
	  GetClientRect(&reClient);

    int nHeight;
	  nHeight = (reClient.Height() - 10) / 4;
	  if (nHeight < (int)DEFULT_COMMVIEW_HEIGHT)
		  nHeight = (int)DEFULT_COMMVIEW_HEIGHT;

	  m_pctrlSplitter->SetRowInfo(0, nHeight, 0);
    if (reClient.Height() <= 0)
      m_pctrlSplitter->SetRowInfo(1, nHeight, 0);
    else
	{
		int cyIDeal = max(reClient.Height() - nHeight, 0);
		m_pctrlSplitter->SetRowInfo(1, cyIDeal, 0);
		m_pctrlSplitter->RecalcLayout();
	}
  }
}

void CCMGrid::SetRedrewAll()
{
	m_pViewCommonGrid->m_ctrlGrid.RedrawAll();
	m_pViewOrderGrid->SetRedrewAll();
}

void CCMGrid::SetGroupViewState(BOOL bMinMax)
{
	if (bMinMax == TRUE)	//# MIN
	{

	}
	else if (bMinMax == FALSE)	//# MAX
	{

	}
}

void CCMGrid::SetDetailViewState(BOOL bDetail)
{
  if (m_bDetail == bDetail)
    return;

	m_pViewCommonGrid->SetDetailViewState(bDetail);
	m_pViewOrderGrid->SetDetailViewState(bDetail);

  m_bDetail = bDetail;
}

void CCMGrid::SetGridType(int nType)   //# TYPE_GRID_COMMON, TYPE_GRID_ORDER
{
  if (m_nGridType == nType)
    return;

  if ((int)TYPE_GRID_COMMON == nType)
    m_pctrlSplitter->SetShow_Row(TRUE, 1);
  else if ((int)TYPE_GRID_ORDER == nType)
    m_pctrlSplitter->SetShow_Row(FALSE, 1);

  m_nGridType = nType;
}

int CCMGrid::SetAlignElement()
{
	int nResult = 0;

	int nGroupCnt = GetGroupCnt();
// 	for (int nGroupNo = 0; nGroupNo < nGroupCnt; nGroupNo++)
// 	{
// 		if (nGroupNo == 0)
// 			nResult = m_pViewCommonGrid->SetAlignElement();
// 		else
// 			nResult = m_pViewOrderGrid->SetAlignElement(nResult);
// 	}

	if(nGroupCnt == 1)
		nResult = m_pViewCommonGrid->SetAlignElement();
	else
		nResult = m_pViewOrderGrid->SetAlignElement(nResult);

	return nResult;
}

void CCMGrid::SetSameTargetBase(int nIndex, int nOutType)
{
	m_pViewCommonGrid->SetSameTargetBase(nIndex, nOutType);
}

void CCMGrid::SetSelectRow(int nGroupNo, int nRow, BOOL bMsgSend /* = FALSE*/)
{
	if (bMsgSend == FALSE)
	{
		if ((m_nSelectGroupNo == nGroupNo) && (m_nSelectRow == nRow))
			return;
	}
	
	SetUnSelectRow(m_nSelectGroupNo, m_nSelectRow);
	
	if (nGroupNo == 0)
	{
		m_pViewCommonGrid->SetGroupFocus();
		m_pViewCommonGrid->SetSelectRow(nRow);
	}
	else
	{
		m_pViewOrderGrid->SetGroupFocus(nGroupNo);
		m_pViewOrderGrid->SetSelectRow(nGroupNo, nRow);
	}

	m_nSelectGroupNo = nGroupNo;
	m_nSelectRow = nRow;

	if (bMsgSend == TRUE)
	{
		CUIntArray iaData;

		int nGroup = GetChange_Group(nGroupNo);
		iaData.Add(nGroup);
		iaData.Add(nRow);
		iaData.Add(0);
		
		m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_SELECT_CONDITION, (LPARAM)&iaData);
	}
}

void CCMGrid::SetUnSelectRow(int nGroupNo, int nRow, BOOL bMsgSend)
{
	if (m_nSelectGroupNo == 0)
	{
		m_pViewCommonGrid->SetGroupKillFocus();
		m_pViewCommonGrid->SetUnSelectRow(nRow, m_bDel);
	}
	else
	{
		m_pViewOrderGrid->SetGroupKillFocus(nGroupNo);
		m_pViewOrderGrid->SetUnSelectRow(nGroupNo, nRow);
	}

	m_nSelectGroupNo = -1;
	m_nSelectRow = -1;

	if (bMsgSend == TRUE)
	{
		CUIntArray iaData;

		int nGroup = GetChange_Group(nGroupNo);
		iaData.Add(nGroup);
		iaData.Add(nRow);
		iaData.Add(-1);
		
		//m_pParentWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_SELECT_CONDITION, (LPARAM)&iaData);
	}


	m_pViewCommonGrid->Invalidate();
}

void CCMGrid::SetUserDragDrop(BOOL bState)
{
	m_pViewCommonGrid->SetUserDragDrop(bState);
	m_pViewOrderGrid->SetUserDragDrop(bState);
}

void CCMGrid::SetAlignGroup(BOOL bAutoGroupAlign)
{
	m_pViewOrderGrid->m_bAutoGroupAlign = bAutoGroupAlign;
}

void CCMGrid::ChangeSkinColor(ST_GRIDCOLOR stGridColor)
{	
	if(!stGridColor.clrGridHeader)
		return;

	m_pViewCommonGrid->ChangeSkinColor(stGridColor);
	m_pViewOrderGrid->ChangeSkinColor(stGridColor);
}	

CRect CCMGrid::GetLabelRect()
{
	CRect reLabel;
	m_pViewCommonGrid->m_btnLabel.GetClientRect(&reLabel);
	return reLabel;
}

void CCMGrid::SetChagneTargetEditIndicate(CString strIndicate)
{
	m_pViewCommonGrid->SetChagneTargetEditIndicate(strIndicate);
}