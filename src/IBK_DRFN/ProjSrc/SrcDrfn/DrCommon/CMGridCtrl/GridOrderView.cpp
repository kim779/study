// GridOrderView.cpp : implementation file
//

#include "stdafx.h"
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

extern HINSTANCE hDllInstance;

//#define GRID_DEF_HEIGHT		34 + 24
#define GRID_DEF_HEIGHT		2 + FOOTER_DEF_HEIGHT
#define GRID_GROUP_GAP				2

/////////////////////////////////////////////////////////////////////////////
// CGridOrderView
IMPLEMENT_DYNCREATE(CGridOrderView, CScrollView)

CGridOrderView::CGridOrderView()
{
	//# System Values ####################
	//m_bAutoGroupAlign = TRUE;
	m_bAutoGroupAlign = FALSE;	//#<= Fix

	m_bInsertPosUp = TRUE;
	//####################################

	m_nResourceIndex = 0;
	m_nLastGroupBottom = 0;

	m_listGroupInfo.RemoveAll();

	m_brushFooter.CreateSolidBrush(COLOR_FOOTER);
	m_brushFooter_Focus.CreateSolidBrush(COLOR_FOOTER_FOCUS);

	m_brushLabel.CreateSolidBrush(COLOR_LABEL);
	m_brushLabelFocus.CreateSolidBrush(COLOR_LABEL_FOCUS);
}

CGridOrderView::~CGridOrderView()
{
	m_brushFooter.DeleteObject();
	m_brushFooter_Focus.DeleteObject();

	m_brushLabel.DeleteObject();
	m_brushLabelFocus.DeleteObject();
}

BEGIN_MESSAGE_MAP(CGridOrderView, CScrollView)
	//{{AFX_MSG_MAP(CGridOrderView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_WND_EVENT, OnNotifyWndEvent)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridOrderView drawing

void CGridOrderView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;

	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CGridOrderView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

	CRect rcClient;
	GetClientRect(rcClient);
	pDC->FillSolidRect(rcClient, RGB(255, 255, 255));
}

/////////////////////////////////////////////////////////////////////////////
// CGridOrderView diagnostics

#ifdef _DEBUG
void CGridOrderView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CGridOrderView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGridOrderView message handlers

int CGridOrderView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	CRect reClient;
	GetClientRect(reClient);

	m_szTotViewSize.cx = reClient.right;
	m_szTotViewSize.cy = reClient.bottom - reClient.top;

	m_nTotHeightView = m_szTotViewSize.cy; 

	m_pParentWnd = this;
	m_pParentWnd = m_pParentWnd->GetParent();	//# CGridSplotterWnd
	m_pParentWnd = m_pParentWnd->GetParent(); //# CGridMainFrameWnd

	return 0;
}

void CGridOrderView::OnDestroy() 
{
	CScrollView::OnDestroy();

	// TODO: Add your message handler code here
	AllDeleteGroupInfo();
}

void CGridOrderView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);

	SetGroupAlign();
}

void CGridOrderView::SetGroupAlign()
{
	//BOOL bLastPos = FALSE;
	BOOL bLastPos = TRUE;

	if (m_nTotHeightView <= 0)
		return;

	CRect reClient;
	GetClientRect(reClient);

	CPoint ptNowPosition;
	ptNowPosition = GetScrollPosition();

	if (m_nTotHeightView > reClient.bottom)
	{
		POINT ptPosition;
		ptPosition.x = 0;
		ptPosition.y = 0;

		ScrollToPosition(ptPosition);
	}

	SetGridMoveWindow(reClient);

	ScrollToPosition(ptNowPosition);	//#<= Scroll Pos 
}

void CGridOrderView::SetGridMoveWindow(CRect reClient)
{
	int nGroupCnt = m_listGroupInfo.GetCount();
	if (m_listGroupInfo.GetCount() <= 0)
		return;

	int nAlignIndex = 1;
	int nMaxGroupNo;

	if (m_bAutoGroupAlign == TRUE)
		nMaxGroupNo = GetMaxGroupNo();
	else
		nMaxGroupNo = GetMaxIndex();

	POSITION pos;
	GroupInfo		stGroupInfo;

	CRect reGroup;
	int nTop = 0;

	BOOL bMoveState;
	int nIndex = 0;
	while (nMaxGroupNo >= nAlignIndex)
	{
		bMoveState = FALSE;

		pos = m_listGroupInfo.FindIndex(nIndex);
		stGroupInfo = m_listGroupInfo.GetAt(pos);

		//# Group Auto Align ######
		if (m_bAutoGroupAlign == TRUE)
		{
			if (nAlignIndex == stGroupInfo.nGroupNo)
				bMoveState = TRUE;
		}
		else if (m_bAutoGroupAlign == FALSE)
		{
			if (nAlignIndex == stGroupInfo.nIndex)
				bMoveState = TRUE;
		}

		if (bMoveState == TRUE)
		{
			stGroupInfo.pOrderGridWnd->m_nGroupNo = stGroupInfo.nGroupNo;

			CString strTemp;
			if (m_bAutoGroupAlign == TRUE)
				strTemp.Format("%d/%d", stGroupInfo.nGroupNo, m_listGroupInfo.GetCount());
			else
				strTemp.Format("%d/%d", stGroupInfo.nIndex, m_listGroupInfo.GetCount());
			stGroupInfo.pOrderGridWnd->m_ctrlGroupNo.SetWindowText(strTemp);

			reClient.top = nTop;
			reClient.bottom = reClient.top + stGroupInfo.nHeight;
			stGroupInfo.pOrderGridWnd->MoveWindow(reClient);

			nTop += stGroupInfo.nHeight + GRID_GROUP_GAP;

			nAlignIndex++;
			nIndex = 0;
		}
		else
			nIndex++;
	}

	Invalidate();
}

void CGridOrderView::SetGroupKillFocus(int nGroupNo)
{
	POSITION pos; 
	GroupInfo stGroupInfo;
	if (FindIndexToGroupInfo(nGroupNo, pos, stGroupInfo) == TRUE)
	{
		stGroupInfo.pOrderGridWnd->SetKillFocus();
		m_nSelectGroup = -1;
	}
}

void CGridOrderView::SetGroupFocus(int nGroupNo)
{
	POSITION pos; 
	GroupInfo stGroupInfo;

	if (FindIndexToGroupInfo(nGroupNo, pos, stGroupInfo) == TRUE)
	{
		stGroupInfo.pOrderGridWnd->SetFocus(FALSE);
		m_nSelectGroup = nGroupNo;
	}
}

void CGridOrderView::SetSelectRow(int nGroupNo, int nRow)
{
	POSITION pos; 
	GroupInfo stGroupInfo;

	SetUnSelectRow(m_nSelectGroup, m_nSelectRow);
	if (FindIndexToGroupInfo(nGroupNo, pos, stGroupInfo) == TRUE)
	{
		if (m_nSelectGroup != nGroupNo)
		{
			stGroupInfo.pOrderGridWnd->SetFocus(FALSE);
			m_nSelectGroup = nGroupNo;
		}

		stGroupInfo.pOrderGridWnd->m_ctrlGrid.SetSelectRow(nRow);
		m_nSelectRow = nRow;
	}
}

void CGridOrderView::SetUnSelectRow(int nGroupNo, int nRow)
{
	POSITION pos; 
	GroupInfo stGroupInfo;

	if (FindIndexToGroupInfo(nGroupNo, pos, stGroupInfo) == TRUE)
	{
		stGroupInfo.pOrderGridWnd->m_ctrlGrid.SetUnSelectRow(nRow);

		m_nSelectGroup = -1;
		m_nSelectRow = -1;
	}
}

void CGridOrderView::SetDetailViewState(BOOL bDetail)
{
	POSITION pos;
	GroupInfo	stGroupInfo;

	int nGroupCnt = m_listGroupInfo.GetCount();
	for (int nIndex = 0; nIndex < m_listGroupInfo.GetCount(); nIndex++)
	{
		pos = m_listGroupInfo.FindIndex(nIndex);
		stGroupInfo = m_listGroupInfo.GetAt(pos);

		stGroupInfo.pOrderGridWnd->m_ctrlGrid.SetDetailViewState(bDetail);
	}
}

CString CGridOrderView::GetCellDate(int nSelectGroupNo, int nCol, int nRow)
{
	CString strResult = "";

	POSITION pos;
	GroupInfo		stGroupInfo;

	if (FindIndexToGroupInfo(nSelectGroupNo, pos, stGroupInfo) == TRUE)
		strResult = stGroupInfo.pOrderGridWnd->GetCellDate(nCol, nRow);

	return strResult;
}

CString CGridOrderView::GetElementInfo(int nSelectGroupNo, int nRow)
{
	CString strResult = "";

	POSITION pos;
	GroupInfo		stGroupInfo;

	if (m_bAutoGroupAlign == FALSE)
		nSelectGroupNo = GetIndexToGroupNo(nSelectGroupNo);

	if (FindIndexToGroupInfo(nSelectGroupNo, pos, stGroupInfo) == TRUE)
		strResult = stGroupInfo.pOrderGridWnd->GetElementInfo(nRow);

	return strResult;
}

void CGridOrderView::SetCellDate(int nSelectGroupNo, int nCol, int nRow, CString strData)
{
	POSITION pos;
	GroupInfo		stGroupInfo;

	if (FindIndexToGroupInfo(nSelectGroupNo, pos, stGroupInfo) == TRUE)
		stGroupInfo.pOrderGridWnd->SetCellDate(nCol, nRow, strData);
}

LRESULT CGridOrderView::OnNotifyWndEvent(WPARAM wParam, LPARAM lParam)
{
	//# WPARAM wParam  ==>  Event
	//# LPARAM lParam  ==>  DATA
	CString strTemp;

	CWnd *pMainWnd = m_pParentWnd->GetParent(); //# OUT Wnd

	switch(wParam)
	{
		case NM_SELECT_GROUP :	
			{
				m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_SELECT_GROUP, (int)lParam);
			}
			break;

		case NM_SELECT_CONDITION :	//#=> (0) : Group, (1) : Row
			{
				CUIntArray *piaData;
				piaData = (CUIntArray *)lParam;
				m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_SELECT_CONDITION, (LPARAM)lParam);
			}
			break;

		case NM_CONDITION_VIEW :	//# (0) : Group, (1) : TotHeight
			{
				CUIntArray *piaData;
				piaData = (CUIntArray *)lParam;

				POSITION pos; 
				GroupInfo stGroupInfo;
				if (FindIndexToGroupInfo(piaData->GetAt(0), pos, stGroupInfo) == TRUE)
				{
					stGroupInfo.nHeight = piaData->GetAt(1);
					m_listGroupInfo.SetAt(pos, stGroupInfo);

					SetViewSize();
				}
			}
			break;

		case  NM_STARTBONG_VAL : //#=> (0) : Group, (1) : val
			{
				CUIntArray *piaData;
				piaData = (CUIntArray *)lParam;
						
				int nGroup = GetChange_GroupNoToGroupIndex(piaData->GetAt(0));
				piaData->SetAt(0, nGroup);

				pMainWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_STARTBONG_VAL, (LPARAM)piaData);
			}
			break;

		case NM_ENDBONG_VAL :	//#=> (0) : Group, (1) : Val
			{
				CUIntArray *piaData;
				piaData = (CUIntArray *)lParam;
				
				int nGroup = GetChange_GroupNoToGroupIndex(piaData->GetAt(0));
				piaData->SetAt(0, nGroup);

				pMainWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_ENDBONG_VAL, (LPARAM)piaData);
			}
			break;

		case NM_CONDITION_DRAGDROP :	//# (0) : Group, (1) : Old Row, (2) : New Row
			{
				CUIntArray *piaData;
				piaData = (CUIntArray *)lParam;

				int nGroup = GetChange_GroupNoToGroupIndex(piaData->GetAt(0));
				piaData->SetAt(0, nGroup);

				pMainWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_CONDITION_DRAGDROP, (LPARAM)piaData);
			}
			break;

		case NM_VALVIEWALL_CHECK :	//# (0) : Group, (1) : Val
			{
				CUIntArray *piaData;
				piaData = (CUIntArray *)lParam;
				
				m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_VALVIEWALL_CHECK, (LPARAM)piaData);
			}
			break;

		case NM_DELETEALL :	//# Group
			{
				POSITION pos; 
				GroupInfo stGroupInfo;
				if (FindIndexToGroupInfo((int)lParam, pos, stGroupInfo) == TRUE)
				{
					stGroupInfo.nHeight = (int)GRID_DEF_HEIGHT;
					m_listGroupInfo.SetAt(pos, stGroupInfo);

					SetViewSize();
				}
				
				m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_DELETEALL, (LPARAM)lParam);
			}
			break;

		case NM_CONDITION_CHECK :		//# (0) : Group, (1) : Row, (2) : Val
			{
				CUIntArray *piaData;
				piaData = (CUIntArray *)lParam;

				m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_CONDITION_CHECK, (LPARAM)piaData);
			}
			break;

		case NM_TARGET_LIST :		//# (0) : Group, (1) : Row, (2) : Target Type, (3) : Target
			{
				CUIntArray *piaData;
				piaData = (CUIntArray *)lParam;
				
				//# Target All Same!
				SetTargetAllSame(piaData->GetAt(2), piaData->GetAt(3));
				
				//# Message : Focus Color
				m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_TARGET_LIST, (LPARAM)piaData);
			}
			break;

		case NM_VALVIEW_CHECK : //# (0) : Group, (1) : Row, (1) : Val
			{
				CUIntArray *piaData;
				piaData = (CUIntArray *)lParam;
				
				m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_VALVIEW_CHECK, (LPARAM)piaData);
			}
			break;

		case NM_MODIFY_BTN :		//# (0) : Group, (1) : Row, (2) : Display Row, (3) : Display Row => Top Screen Pos
			{
				CUIntArray *piaData;
				piaData = (CUIntArray *)lParam;

				m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_MODIFY_BTN, (LPARAM)piaData);
			}
			break;

		case NM_DELETE_BTN :	//# (0) : Group, (1) : Row(After Select), (2) : TotHeight, (3) : Element Index
			{
				CUIntArray *piaData;
				piaData = (CUIntArray *)lParam;

				POSITION pos; 
				GroupInfo stGroupInfo;
				if (FindIndexToGroupInfo(piaData->GetAt(0), pos, stGroupInfo) == TRUE)
				{
					if (stGroupInfo.pOrderGridWnd->m_ctrlGrid.GetNumberRows() == 0)
						stGroupInfo.nHeight = GRID_DEF_HEIGHT;
					else
						stGroupInfo.nHeight = (int)piaData->GetAt(2);
					m_listGroupInfo.SetAt(pos, stGroupInfo);

					SetViewSize();
				}

				CUIntArray iaData;
				iaData.Add(piaData->GetAt(0));
				iaData.Add(piaData->GetAt(1));
				iaData.Add(piaData->GetAt(3));

				m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_DELETE_BTN, (LPARAM)&iaData);
			}
			break;


		case NM_ALIGN_ELEMENT :
			{
				int nTotElementCnt = (int)lParam;
				CUIntArray iaData;

				if (nTotElementCnt == -1)
					iaData.Add(1);
				else
				{
					nTotElementCnt = SetAlignElement(nTotElementCnt);
					iaData.Add(2);
				}
				iaData.Add(nTotElementCnt);
				
				m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_ALIGN_ELEMENT, (LPARAM)&iaData);
			}
			break;

		case NM_GROUP_ADD	:
				SetAddGroup();
			break;

		case NM_GROUP_INSERTUP :		//# Group
			{
				int nSelectGroup = (int)lParam;
				SetInsertGroup(nSelectGroup, TRUE);				
			}
			break;

		case NM_GROUP_INSERTDOWN :		//# Group
			{
				int nSelectGroup = (int)lParam;
				SetInsertGroup(nSelectGroup, FALSE);
			}
			break;

		case NM_GROUP_DELETE :		//# Group
			{
				int nSelectGroup = (int)lParam;			
				SetDeleteGroup(nSelectGroup);
			}
			break;
		case NM_ROW_UP_BTN:
		case NM_ROW_DOWN_BTN:
			{
				m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_ROW_UP_BTN, (LPARAM)(CUIntArray*)lParam);		
			}
			break;
		case NM_ROW_MAXUP_BTN:
			{
				m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_ROW_MAXUP_BTN, (LPARAM)(CStringArray*)lParam);		
			}
			break;
		case NM_ROW_MINDOWN_BTN:
			{
				m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_ROW_MINDOWN_BTN, (LPARAM)(CStringArray*)lParam);		
			}
			break;
						
		default :
      break;
	}
	return TRUE;
}

int CGridOrderView::GetChange_GroupNoToGroupIndex(int nGroupNo)
{
	int nGroupIndex;
	if (m_bAutoGroupAlign == TRUE)
		nGroupIndex = nGroupNo;
	// 20100630 박찬모 >>
	// 순차검색 그룹 삽입(추가No)시 선택한 그룹을 다르게 리턴
	else
		/*nGroupIndex = GetGroupNoToIndex((int)nGroupNo);*/
		nGroupIndex = nGroupNo;

	return nGroupIndex;
}


int CGridOrderView::SetAlignElement(int nTotElementCnt)
{
	//# Set Group Element Align!
	POSITION pos;
	GroupInfo	stGroupInfo;
	
	int nGroupNoIndex = 1;
	int nTotElementIndex = nTotElementCnt;

	int nGroupCnt = m_listGroupInfo.GetCount();
	int nIndex = 0;
	while(nGroupCnt >= nGroupNoIndex)
	{
		BOOL bAlignState = FALSE;
							
		pos = m_listGroupInfo.FindIndex(nIndex);
		stGroupInfo = m_listGroupInfo.GetAt(pos);
		
		if (m_bAutoGroupAlign == TRUE)
		{
			if (stGroupInfo.nGroupNo == nGroupNoIndex)
				bAlignState = TRUE;
		}
		else if (m_bAutoGroupAlign == FALSE)
		{
			if (stGroupInfo.nIndex == nGroupNoIndex)
				bAlignState = TRUE;
		}
		
		if (bAlignState == TRUE)
		{
			nTotElementIndex = stGroupInfo.pOrderGridWnd->m_ctrlGrid.SetAlignElement(nTotElementIndex);
			nGroupNoIndex++;
			nIndex = 0;
		}
		else
			nIndex++;
	}

	return nTotElementIndex;
}


void CGridOrderView::SetTargetAllSame(int nTargetType, int nTargetIndex)
{
	//## int nTargetType   <=  Not User!


	POSITION pos;
	GroupInfo	stGroupInfo;

	int nGroupCnt = m_listGroupInfo.GetCount();
	for (int nIndex = 0; nIndex < nGroupCnt; nIndex++)
	{
		pos = m_listGroupInfo.FindIndex(nIndex);
		stGroupInfo = m_listGroupInfo.GetAt(pos);

		//# Inex => String change
		stGroupInfo.pOrderGridWnd->m_ctrlGrid.SetModifyAllTargetCol(nTargetIndex);
	}

	Invalidate();
}


BOOL CGridOrderView::SetAddCondition(int nSelectGroupNo, BOOL bState,	//# Condition View
															int	nElementIndex,	//# Element Index
															BOOL bNot,						//# Not State 
															int	nTargetType,		//# Target Type
															int	nTargetIndex,		//# Target Index
															CString strItem,				//# 완성 구문 
															BOOL bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
															BOOL bFieldVal)			//# 값보기

{
	BOOL bResult = FALSE;

	POSITION pos;
	GroupInfo		stGroupInfo;

//	if (m_bAutoGroupAlign == FALSE)
//		nSelectGroupNo = GetIndexToGroupNo(nSelectGroupNo);
	
	if (FindIndexToGroupInfo(nSelectGroupNo, pos, stGroupInfo) == TRUE)
	{
		stGroupInfo.nHeight = stGroupInfo.pOrderGridWnd->SetAddCondition(bState, nElementIndex, bNot, nTargetType, 
			nTargetIndex, strItem, bFieldState, bFieldVal);

		m_listGroupInfo.SetAt(pos, stGroupInfo);

		if (stGroupInfo.pOrderGridWnd->m_ctrlGrid.m_bViewState == TRUE)
		{
			if (stGroupInfo.pOrderGridWnd->m_ctrlGrid.SetMakeRowVisible(TRUE) == TRUE)
				stGroupInfo.pOrderGridWnd->m_ctrlGrid.m_bViewState = FALSE;
		}
		SetViewSize();

		bResult = TRUE;
	}

	return bResult;
}

BOOL CGridOrderView::SetInsertCondition(int nRow, int nSelectGroupNo, 
										BOOL		bState,					//# Condition View
										int nElementIndex,	//# Element Index
										BOOL		bNot,						//# Not State 
										int			nTargetType,		//# Target Type
										int			nTargetIndex,		//# Target Index
										CString strItem,				//# 완성 구문 
										BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
										BOOL		bFieldVal)			//# 값보기

{
	BOOL bResult = FALSE;

	POSITION pos;
	GroupInfo		stGroupInfo;

	if (FindIndexToGroupInfo(nSelectGroupNo, pos, stGroupInfo) == TRUE)
	{
		stGroupInfo.nHeight = stGroupInfo.pOrderGridWnd->SetInsertCondition(nRow, bState, nElementIndex, bNot, nTargetType,	
			nTargetIndex, strItem, bFieldState, bFieldVal);

		m_listGroupInfo.SetAt(pos, stGroupInfo);

		if (stGroupInfo.pOrderGridWnd->m_ctrlGrid.m_bViewState == TRUE)
		{
			if (stGroupInfo.pOrderGridWnd->m_ctrlGrid.SetMakeRowVisible(TRUE) == TRUE)
				stGroupInfo.pOrderGridWnd->m_ctrlGrid.m_bViewState = FALSE;
		}
		SetViewSize();

		bResult = TRUE;
	}

	return bResult;
}

BOOL CGridOrderView::SetModifyCondition(int nRow, int nSelectGroupNo, BOOL	bState,					//# Condition View
										CString	strElement,			//# Element Index
										BOOL		bNot,						//# Not State 
										int			nTargetType,		//# Target Type
										int			nTargetIndex,		//# Target Index
										CString strItem,				//# 완성 구문 
										BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
										BOOL		bFieldVal)			//# 값보기
{
	BOOL bResult = FALSE;

	POSITION pos;
	GroupInfo		stGroupInfo;

	if (FindIndexToGroupInfo(nSelectGroupNo, pos, stGroupInfo) == TRUE)
	{
		stGroupInfo.pOrderGridWnd->SetModifyCondition(nRow, bState, strElement,	bNot, nTargetType, 
			nTargetIndex, strItem, bFieldState, bFieldVal);
		bResult = TRUE;
	}

	return bResult;
}


BOOL CGridOrderView::SetModifyCondition(int nRow, int nSelectGroupNo, 
										BOOL bNot, CString strItem, BOOL bFieldVal)
{
	BOOL bResult = FALSE;

	POSITION pos;
	GroupInfo		stGroupInfo;
	if (FindIndexToGroupInfo(nSelectGroupNo, pos, stGroupInfo) == TRUE)
	{
		stGroupInfo.pOrderGridWnd->SetModifyCondition(nRow, bNot, strItem, bFieldVal);	//#<= Input Struct
		bResult = TRUE;

		SetViewSize();
	}

	return bResult;
}

BOOL CGridOrderView::SetDeleteCondition(int nRow, int nSelectGroupNo)
{
	BOOL bResult = FALSE;

	POSITION pos;
	GroupInfo		stGroupInfo;
	if (FindIndexToGroupInfo(nSelectGroupNo, pos, stGroupInfo) == TRUE)
		bResult = stGroupInfo.pOrderGridWnd->SetDeleteCondition(nRow);

	return bResult;
}

void CGridOrderView::SetAddGroup(BOOL bGroupForcus /*= TRUE*/)
{
	CRect reClient;
	GetClientRect(reClient);

	int nIndex;
	if (m_nResourceIndex == 0)
	{
		reClient.top = m_nLastGroupBottom;
		nIndex = 1;
	}
	else
	{
		reClient.top = m_nLastGroupBottom + (int)GRID_GROUP_GAP;
		nIndex = GetMaxIndex() + 1;
	}

	reClient.bottom = reClient.top + GRID_DEF_HEIGHT;

	m_nLastGroupBottom = reClient.bottom;

	CGridOrderWnd	*pOrderGridWnd = new CGridOrderWnd;
	int nGroupNo = GetMaxGroupNo() + 1;
	pOrderGridWnd->m_nGroupNo = nGroupNo;

	BOOL bState = pOrderGridWnd->Create(_T("STATIC"), 
		"", 
		WS_CHILD | WS_VISIBLE,
		reClient, this, m_nResourceIndex);

	pOrderGridWnd->ChangeSkinColor(m_stGridColor);

	if (bState == FALSE)
		AfxMessageBox("Order Condition Group Faile!");

	GroupInfo	stGroupInfo;

	stGroupInfo.nIndex = nIndex;
	stGroupInfo.nGroupNo = nGroupNo;
	stGroupInfo.pOrderGridWnd = pOrderGridWnd;
	stGroupInfo.nHeight = GRID_DEF_HEIGHT;

	m_listGroupInfo.AddTail(stGroupInfo);

	if (bGroupForcus == TRUE)
		m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_SELECT_GROUP, (int)nGroupNo);

	m_nResourceIndex++;
	SetViewSize();
}

int CGridOrderView::GetMaxGroupNo()
{
	int nResult = 0;

	POSITION pos;
	GroupInfo		stGroupInfo;

	int nGroupCnt = m_listGroupInfo.GetCount();
	for (int nIndex = 0; nIndex < nGroupCnt; nIndex++)
	{
		pos = m_listGroupInfo.FindIndex(nIndex);
		stGroupInfo = m_listGroupInfo.GetAt(pos);

		if (nResult < stGroupInfo.nGroupNo)
			nResult = stGroupInfo.nGroupNo;
	}

	return nResult;
}


int CGridOrderView::GetMaxIndex()
{
	int nResult = 0;

	POSITION pos;
	GroupInfo		stGroupInfo;

	int nGroupCnt = m_listGroupInfo.GetCount();
	for (int nIndex = 0; nIndex < nGroupCnt; nIndex++)
	{
		pos = m_listGroupInfo.FindIndex(nIndex);
		stGroupInfo = m_listGroupInfo.GetAt(pos);

		if (nResult < stGroupInfo.nIndex)
			nResult = stGroupInfo.nIndex;
	}

	return nResult;
}


BOOL CGridOrderView::SetInsertGroup(int nSelectGroupNo, BOOL bInsertPosUp)
{
	if (nSelectGroupNo <= 0)
		return FALSE;

	POSITION pos;
	GroupInfo stGroupInfo;

	//# Get Select Group Rect!
	CRect reClient;
	GetClientRect(reClient);

	CRect reGroup;
	int nTempGroupNo;

	int nInsertGroupNo;
	int nInsertIndex;
	if (m_bAutoGroupAlign == TRUE)
	{
		if ((bInsertPosUp == TRUE) && (nSelectGroupNo > 1))
			nTempGroupNo = nSelectGroupNo - 1;
		else
			nTempGroupNo = nSelectGroupNo;

		nInsertIndex = m_nResourceIndex;
		nInsertGroupNo = nTempGroupNo;
	}
	else
	{
		if (FindIndexToGroupInfo(nSelectGroupNo, pos, stGroupInfo) == TRUE)
		{
			if (bInsertPosUp == TRUE)
			{
				nTempGroupNo = GetIndexToGroupNo(stGroupInfo.nIndex);
				nInsertIndex = stGroupInfo.nIndex;
			}
			else
			{
				nTempGroupNo = GetIndexToGroupNo(stGroupInfo.nIndex + 1);
				nInsertIndex = stGroupInfo.nIndex + 1;
			}
		}

		nInsertGroupNo = GetMaxGroupNo() + 1;
	}

	reGroup = GetSelectGroupRect(nTempGroupNo);

	reClient.top = reGroup.bottom + GRID_GROUP_GAP;
	reClient.bottom = reClient.top + GRID_DEF_HEIGHT;

	CGridOrderWnd	*pOrderGridWnd = new CGridOrderWnd;

	if (m_bAutoGroupAlign == TRUE)
		SetModifyGroupInfo(nTempGroupNo, FALSE);
	else
		SetModifyGroupInfo(nInsertIndex, FALSE);

	pOrderGridWnd->m_nGroupNo = nInsertGroupNo;

	pOrderGridWnd->Create(_T("STATIC"), 
		"", 
		WS_CHILD | WS_VISIBLE,
		reClient, this, m_nResourceIndex);


	memset(&stGroupInfo, 0x00, sizeof(stGroupInfo));
	stGroupInfo.nIndex = nInsertIndex;
	stGroupInfo.nGroupNo = nInsertGroupNo;
	stGroupInfo.pOrderGridWnd = pOrderGridWnd;
	stGroupInfo.nHeight = GRID_DEF_HEIGHT;

	m_listGroupInfo.AddTail(stGroupInfo);
	m_nResourceIndex++;

	SetViewSize();

	CUIntArray iaData;
	iaData.Add(nInsertIndex);
	iaData.Add(nInsertGroupNo);
	if (bInsertPosUp == TRUE)
		m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_GROUP_INSERTUP, (LPARAM)&iaData);
	else
		m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_GROUP_INSERTDOWN, (LPARAM)&iaData);

	//	m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_SELECT_GROUP, (int)nInsertGroupNo);

	return TRUE;
}


int CGridOrderView::GetIndexToGroupNo(int nGroupIndex)
{
	/*
	int nResult = -1;

	POSITION pos;
	GroupInfo stGroupInfo;

	int nGroupCnt = m_listGroupInfo.GetCount();
	for (int nIndex = 0; nIndex < nGroupCnt; nIndex++)
	{
	pos = m_listGroupInfo.FindIndex(nIndex);
	stGroupInfo = m_listGroupInfo.GetAt(pos);

	if (nGroupIndex == stGroupInfo.nIndex)
	nResult = stGroupInfo.nGroupNo;
	}
	*/

	// 2009.11.12 -hidden-
	int nResult = nGroupIndex;

	return nResult;
}

int CGridOrderView::GetGroupNoToIndex(int nGroupNo)
{
	int nResult = -1;

	POSITION pos;
	GroupInfo stGroupInfo;

	int nGroupCnt = m_listGroupInfo.GetCount();
	for (int nIndex = 0; nIndex < nGroupCnt; nIndex++)
	{
		pos = m_listGroupInfo.FindIndex(nIndex);
		stGroupInfo = m_listGroupInfo.GetAt(pos);

		if (nGroupNo == stGroupInfo.nGroupNo)
			nResult = stGroupInfo.nIndex;
	}

	return nResult;
}


CRect CGridOrderView::GetSelectGroupRect(int nSelectGroupNo)
{
	CRect reResult;

	POSITION pos;
	GroupInfo stGroupInfo;

	if (FindIndexToGroupInfo(nSelectGroupNo, pos, stGroupInfo) == TRUE)
		stGroupInfo.pOrderGridWnd->GetClientRect(reResult);

	return reResult;
}


void CGridOrderView::SetDeleteGroup(int nSelectGroupNo)
{
	if (nSelectGroupNo < 0)
		return;

	if(m_listGroupInfo.GetCount() == 1)
		return;

	POSITION pos;
	GroupInfo stGroupInfo;

	int nFocusGroupNo;
	if (FindIndexToGroupInfo(nSelectGroupNo, pos, stGroupInfo) == TRUE)
	{
		if (::IsWindow(stGroupInfo.pOrderGridWnd->m_hWnd) == TRUE)
			stGroupInfo.pOrderGridWnd->DestroyWindow();
		delete stGroupInfo.pOrderGridWnd;

		m_listGroupInfo.RemoveAt(pos);

		if (m_bAutoGroupAlign == TRUE)
		{
			SetModifyGroupInfo(nSelectGroupNo, TRUE);
			nFocusGroupNo = nSelectGroupNo - 1;
		}
		else
		{
			SetModifyGroupInfo(stGroupInfo.nIndex, TRUE);
			nFocusGroupNo = GetIndexToGroupNo(stGroupInfo.nIndex - 1);
		}
	}
	SetViewSize();

	if (m_listGroupInfo.GetCount() == 0)
		SetInit();

	CUIntArray iaData;
	iaData.Add(nSelectGroupNo);
	iaData.Add(nFocusGroupNo);
	m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_GROUP_DELETE, (LPARAM)&iaData);
}


void CGridOrderView::SetModifyGroupInfo(int nGroupNo, BOOL bDelete)
{
	POSITION pos;
	GroupInfo		stGroupInfo;

	int nGroupCnt = m_listGroupInfo.GetCount();
	for (int nIndex = 0; nIndex < nGroupCnt; nIndex++)
	{
		pos = m_listGroupInfo.FindIndex(nIndex);
		stGroupInfo = m_listGroupInfo.GetAt(pos);

		if (bDelete == TRUE)
		{
			if ((m_bAutoGroupAlign == TRUE) && (stGroupInfo.nGroupNo > nGroupNo))
			{
				stGroupInfo.nGroupNo--;
				m_listGroupInfo.SetAt(pos, stGroupInfo);
			}
			else if ((m_bAutoGroupAlign == FALSE) && (stGroupInfo.nIndex > nGroupNo))
			{
				stGroupInfo.nIndex--;
				m_listGroupInfo.SetAt(pos, stGroupInfo);
			}
		}
		else
		{
			if ((m_bAutoGroupAlign == TRUE) && (stGroupInfo.nGroupNo >= nGroupNo))
			{
				stGroupInfo.nGroupNo++;
				m_listGroupInfo.SetAt(pos, stGroupInfo);
			}
			else if ((m_bAutoGroupAlign == FALSE) && (stGroupInfo.nIndex >= nGroupNo))
			{
				stGroupInfo.nIndex++;
				m_listGroupInfo.SetAt(pos, stGroupInfo);
			}
		}
	}
}


BOOL CGridOrderView::FindIndexToGroupInfo(int nGroupNo, POSITION &pos, 
										  GroupInfo &stGroupInfo)
{
	BOOL bResult = FALSE;

	int nGroupCnt = m_listGroupInfo.GetCount();
	for (int nIndex = 0; nIndex < nGroupCnt; nIndex++)
	{
		pos = m_listGroupInfo.FindIndex(nIndex);
		stGroupInfo = m_listGroupInfo.GetAt(pos);

		if (stGroupInfo.nGroupNo == nGroupNo)
		{
			bResult = TRUE;
			break;
		}
	}

	return bResult;
}


BOOL CGridOrderView::FindIndexToGroupInfo(int nGroupNo, GroupInfo &stGroupInfo)
{
	BOOL bResult = FALSE;

	POSITION pos;
	int nGroupCnt = m_listGroupInfo.GetCount();
	for (int nIndex = 0; nIndex < nGroupCnt; nIndex++)
	{
		pos = m_listGroupInfo.FindIndex(nIndex);
		stGroupInfo = m_listGroupInfo.GetAt(pos);

		if (stGroupInfo.nGroupNo == nGroupNo)
		{
			bResult = TRUE;
			break;
		}
	}

	return bResult;
}


void CGridOrderView::AllDeleteGroupInfo()
{
	POSITION pos;
	GroupInfo stGroupInfo;

	int nGroupCnt = m_listGroupInfo.GetCount();
	for (int nIndex = 0; nIndex < nGroupCnt; nIndex++)
	{
		pos = m_listGroupInfo.FindIndex(nIndex);
		stGroupInfo = m_listGroupInfo.GetAt(pos);

		if (::IsWindow(stGroupInfo.pOrderGridWnd->m_hWnd) == TRUE)
			stGroupInfo.pOrderGridWnd->DestroyWindow();

		delete stGroupInfo.pOrderGridWnd;
	}

	m_listGroupInfo.RemoveAll();
}


void CGridOrderView::SetViewSize()
{
	POSITION pos;
	GroupInfo		stGroupInfo;

	int nGroupCnt = m_listGroupInfo.GetCount();
	m_nTotHeightView = 0;
	for (int nIndex = 0; nIndex < nGroupCnt; nIndex++)
	{
		pos = m_listGroupInfo.FindIndex(nIndex);
		stGroupInfo = m_listGroupInfo.GetAt(pos);

		m_nTotHeightView += stGroupInfo.nHeight;
	}

	m_nTotHeightView += (int)GRID_GROUP_GAP * nGroupCnt;

	CSize sizeTotal;
	if (m_szTotViewSize.cy < m_nTotHeightView)
	{
		sizeTotal.cx = 0;
		sizeTotal.cy = m_nTotHeightView;
		SetScrollSizes(MM_TEXT, sizeTotal);
	}

	SetGroupAlign();
}

void CGridOrderView::SetRedrewAll()
{
	POSITION pos;
	GroupInfo		stGroupInfo;
	int nGroupCnt = m_listGroupInfo.GetCount();
	for (int nIndex = 0; nIndex < nGroupCnt; nIndex++)
	{
		pos = m_listGroupInfo.FindIndex(nIndex);
		stGroupInfo = m_listGroupInfo.GetAt(pos);

		stGroupInfo.pOrderGridWnd->m_ctrlGrid.RedrawAll();
	}
}

void CGridOrderView::DeleteAll()
{
	m_nSelectGroup = -1;
	m_nSelectRow = -1;

	m_nResourceIndex = 0;
	m_nLastGroupBottom = 0;

	AllDeleteGroupInfo();
}


void CGridOrderView::SetInit()
{
	DeleteAll();

	SetAddGroup(FALSE);
}


void CGridOrderView::SetUserDragDrop(BOOL bState)
{
	POSITION pos;
	GroupInfo		stGroupInfo;

	int nGroupCnt = m_listGroupInfo.GetCount();
	for (int nIndex = 0; nIndex < nGroupCnt; nIndex++)
	{
		pos = m_listGroupInfo.FindIndex(nIndex);
		stGroupInfo = m_listGroupInfo.GetAt(pos);

		stGroupInfo.pOrderGridWnd->m_ctrlGrid.m_bUserDragDrop = bState; 
	}
}


void CGridOrderView::DebugDataShow()
{
	POSITION pos;
	GroupInfo		stGroupInfo;

	TRACE("\n### DebugDataShow ###");

	int nGroupCnt = m_listGroupInfo.GetCount();
	for (int nIndex = 0; nIndex < nGroupCnt; nIndex++)
	{
		pos = m_listGroupInfo.FindIndex(nIndex);
		stGroupInfo = m_listGroupInfo.GetAt(pos);

		CString strTemp;
		strTemp.Format("\n\n# Index : %d", stGroupInfo.nIndex);
		TRACE(strTemp);
		strTemp.Format("\n# GroupNo : %d", stGroupInfo.nGroupNo);
		TRACE(strTemp);
		strTemp.Format("\n# Height : %d", stGroupInfo.nHeight);
		TRACE(strTemp);
	}
}

void CGridOrderView::ChangeSkinColor(ST_GRIDCOLOR stGridColor)
{	
	if(!stGridColor.clrGridHeader)
		return;	

	m_clrLabel			=	stGridColor.clrGridHeader;
	m_clrLabelFocus		=	stGridColor.clrGridHeader;
	m_clrFooter			=	stGridColor.clrGridHeader;
	m_clrFooter_Focus	=	stGridColor.clrGridHeader;


	if(m_brushLabel.DeleteObject())
		m_brushLabel.CreateSolidBrush(m_clrLabel);
	if(m_brushLabelFocus.DeleteObject())
		m_brushLabelFocus.CreateSolidBrush(m_clrLabelFocus);
	if(m_brushFooter.DeleteObject())
		m_brushFooter.CreateSolidBrush(m_clrFooter);
	if(m_brushFooter_Focus.DeleteObject())
		m_brushFooter_Focus.CreateSolidBrush(m_clrFooter_Focus);

	GroupInfo		stGroupInfo;
	POSITION pos;

	int nGroupCnt = m_listGroupInfo.GetCount();
	for (int nIndex = 0; nIndex < nGroupCnt; nIndex++)
	{
		pos = m_listGroupInfo.FindIndex(nIndex);
		stGroupInfo = m_listGroupInfo.GetAt(pos);
		stGroupInfo.pOrderGridWnd->ChangeSkinColor(stGridColor);	
	} 

	m_stGridColor.clrGridDownText = stGridColor.clrGridDownText;
	m_stGridColor.clrGridHeader   = stGridColor.clrGridHeader;
	m_stGridColor.clrGridHText    = stGridColor.clrGridHText;  
	m_stGridColor.clrGridHilight  = stGridColor.clrGridHilight;
	m_stGridColor.clrGridSBk      = stGridColor.clrGridSBk;
	m_stGridColor.clrGridSText	  = stGridColor.clrGridSText; 
	m_stGridColor.clrGridUpText	  = stGridColor.clrGridUpText;  
	m_stGridColor.clrGridDownText = stGridColor.clrGridDownText;
	m_stGridColor.clrGridVolText  = stGridColor.clrGridVolText; 
}