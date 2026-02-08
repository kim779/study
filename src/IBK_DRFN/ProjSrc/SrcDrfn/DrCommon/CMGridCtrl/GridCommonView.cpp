// GridCommonView.cpp : implementation file
//

#include "stdafx.h"
#include "GridCommonView.h"

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

#define IDC_GRID						100
#define IDC_FOOTER						200
#define	IDC_BTN_LABEL					300
#define IDC_TARGET_STATIC_INDICATE		400
#define IDC_CHANGE_SUBJECT				500
#define IDC_CHANGE_SUBJECT_INIT			600

/////////////////////////////////////////////////////////////////////////////
// CGridCommonView
IMPLEMENT_DYNCREATE(CGridCommonView, CView)

CGridCommonView::CGridCommonView()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);

	// 	m_brushLabel.CreateSolidBrush(COLOR_LABEL);
	// 	m_brushLabelFocus.CreateSolidBrush(COLOR_LABEL_FOCUS);
	// 	
	// 	m_brushFooter.CreateSolidBrush(COLOR_FOOTER);
	// 	m_brushFooter_Focus.CreateSolidBrush(COLOR_FOOTER_FOCUS);

	m_ctrlGrid.m_nDefRowHeadHeight = 0;

	m_clrLabel			= COLOR_LABEL;
	m_clrLabelFocus		= COLOR_LABEL_FOCUS;
	m_clrFooter			= COLOR_FOOTER;
	m_clrFooter_Focus	= COLOR_FOOTER_FOCUS;

	m_brushLabel.CreateSolidBrush(m_clrLabel);
	m_brushLabelFocus.CreateSolidBrush(m_clrLabelFocus);

	m_brushFooter.CreateSolidBrush(m_clrFooter);
	m_brushFooter_Focus.CreateSolidBrush(m_clrFooter_Focus);
	m_brushTagetEdit.CreateSolidBrush(RGB(255, 255, 255));
}

CGridCommonView::~CGridCommonView()
{
	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();

	m_brushLabel.DeleteObject();
	m_brushLabelFocus.DeleteObject();

	m_brushFooter.DeleteObject();
	m_brushFooter_Focus.DeleteObject();
	m_brushTagetEdit.DeleteObject();
}


BEGIN_MESSAGE_MAP(CGridCommonView, CView)
	//{{AFX_MSG_MAP(CGridCommonView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_GRID_EVENT, OnNotifyGridEvent)
	ON_BN_CLICKED(IDC_FOOTER, OnFooterClk)		//Added by cyberahn@KOSCOM 2006.07.21
	ON_BN_CLICKED(IDC_CHANGE_SUBJECT, OnChangeSubject)
	ON_BN_CLICKED(IDC_CHANGE_SUBJECT_INIT, OnChangeSubjectInit)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridCommonView drawing

void CGridCommonView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here	
}

/////////////////////////////////////////////////////////////////////////////
// CGridCommonView diagnostics

#ifdef _DEBUG
void CGridCommonView::AssertValid() const
{
	CView::AssertValid();
}

void CGridCommonView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGridCommonView message handlers

void CGridCommonView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	CRect reClient, reTop;
	GetClientRect(reClient);

	reTop = reClient;
	reTop.bottom = reTop.top + FOOTER_DEF_HEIGHT;
	OnSize_Footer(reTop);

	reTop.top = reTop.bottom + 1;
	reTop.bottom = reClient.bottom;
	m_ctrlGrid.MoveWindow(reTop);
}

void CGridCommonView::OnSize_Footer(CRect reClient)
{
	m_ctrlFooter.MoveWindow(reClient);
	m_reBackGround = reClient;
	/*m_ctrlFooter.ShowWindow(SW_HIDE);*/

	CRect reLeft;
	reLeft = reClient;

	reLeft.top += 3;
	reLeft.bottom -= 3;

	reLeft.left += 2;
	reLeft.right = reLeft.left + 80;
	m_btnLabel.MoveWindow(reLeft);

	reLeft.top += 1;
	reLeft.bottom -= 1;

	reLeft.right = reClient.right - 2;
	reLeft.left = reLeft.right - 20;
	m_btnChangeSubjectInit.MoveWindow(reLeft);
	m_reBtnInit = reLeft;
	
	reLeft.right = reLeft.left - 2;
	reLeft.left = reClient.left + 147;
	m_staticTargetEditIndicate.MoveWindow(reLeft);

	reLeft.right = reLeft.left - 2;
	reLeft.left = reLeft.right - 60;
	m_btnChangeSubject.MoveWindow(reLeft);
	m_reBtn = reLeft;
}

int CGridCommonView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pParentWnd = this;
	m_pParentWnd = m_pParentWnd->GetParent();	//# CGridSplotterWnd
	m_pParentWnd = m_pParentWnd->GetParent(); //# CGridMainFrameWnd
	
	// TODO: Add your specialized creation code here
	m_btnChangeSubject.Create(_T("대상변경"), CRect(0, 0, 0, 0), this, IDC_CHANGE_SUBJECT);
	m_btnChangeSubject.SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | WS_EX_TOPMOST);
	m_pParentWnd->GetParent()->SendMessage(NM_SETBTNIMG, (WPARAM)&m_btnChangeSubject, (LPARAM)"4");

	m_btnChangeSubjectInit.Create("\n초기화", CRect(0, 0, 20, 20), this, IDC_CHANGE_SUBJECT_INIT);
	m_btnChangeSubjectInit.SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | WS_EX_TOPMOST);
	m_pParentWnd->GetParent()->SendMessage(NM_SETBTNIMG, (WPARAM)&m_btnChangeSubjectInit, (LPARAM)"지우개");

	CString strTemp;
	strTemp.Format("일반검색");
	m_btnLabel.Create(strTemp, 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | SS_CENTER | SS_CENTERIMAGE, // | WS_BORDER
		CRect(0, 0, 0, 0), this, IDC_BTN_LABEL);
	m_btnLabel.SetFont(&m_fontBold);
	
	m_staticTargetEditIndicate.Create(_T(""), WS_CLIPSIBLINGS | WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTERIMAGE,
		CRect(0, 0, 0, 0), this, IDC_TARGET_STATIC_INDICATE);
	m_staticTargetEditIndicate.SetFont(&m_fontDefault);	

	m_ctrlGrid.m_bGridType = FALSE;
	m_ctrlGrid.CreateGrid(
		WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|SS_SIMPLE,//|WS_BORDER,
		CRect(0, 0, 0, 0),
		this,
		IDC_GRID);
	m_ctrlGrid.SetDefFont(&m_fontDefault);
	
	m_ctrlFooter.Create(_T(""), 
		WS_CHILD | WS_VISIBLE | SS_CENTER | SS_NOTIFY | WS_CLIPSIBLINGS, //| WS_BORDER
		CRect(0, 0, 0, 0), this, IDC_FOOTER);
	m_ctrlFooter.SetFont(&m_fontDefault);
	m_ctrlFooter.SetWindowPos(&m_staticTargetEditIndicate, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
// 	m_ctrlFooter.SetWindowPos(&m_btnChangeSubject, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	return 0;
}


LRESULT CGridCommonView::OnNotifyGridEvent(WPARAM wParam, LPARAM lParam)
{
	//# WPARAM wParam  ==>  Event
	//# LPARAM lParam  ==>  DATA
	CString strTemp;

	CWnd *pMainWnd = m_pParentWnd->GetParent(); //# OUT Wnd

	switch(wParam)
	{
	case NM_GRID_SETFOCUS :
		m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_SELECT_GROUP, (int)0);
		break;

	case NM_ROW_SELECT :
		{
			int nSelectRow = (int)lParam;

			CUIntArray iaData;
			iaData.Add(0);
			iaData.Add(nSelectRow);
			iaData.Add(0);

			m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_SELECT_CONDITION, (LPARAM)&iaData);
		}
		break;

	case NM_UPDATE :
		{
			m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_UPDATE, NULL);
			break;
		}

	case NM_ROW_MAXMIN :
		{
			//AfxMessageBox("Set Grid Min/Max!");

		}
		break;

		//####################################################################################

	case NM_ROW_DRAGDROP :		//# (0) : Old Row, (1) : New Row
		{
			CUIntArray* piaData;
			piaData = (CUIntArray*)lParam;

			CUIntArray iaData;
			iaData.Add(0);
			iaData.Add(piaData->GetAt(0));
			iaData.Add(piaData->GetAt(1));

			pMainWnd->SendMessage(WM_CALLPARENT, (WPARAM)NM_CONDITION_DRAGDROP, (LPARAM)&iaData);
		}
		break;

	case NM_HEAD_VALVIEWALL_CHECK	:		//# 값 All Check
		{
			CUIntArray iaData;
			iaData.Add(0);
			iaData.Add((int)lParam);

			m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_HEAD_VALVIEWALL_CHECK, (LPARAM)&iaData);
		}
		break;

	case NM_HEAD_DELETEALL :	//# Group
		m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_DELETEALL, (LPARAM)0);
		break;

	case NM_ROW_CONDITION_CHECK :		//# (0) : Element, (1) : Row, (2) : Val
		{
			CStringArray *psaData;
			psaData = (CStringArray *)lParam;

			CString strElement = psaData->GetAt(0);

			CUIntArray iaData;
			iaData.Add(0);
			iaData.Add(atoi(psaData->GetAt(1)));
			iaData.Add(atoi(psaData->GetAt(2)));

			m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_CONDITION_CHECK, (LPARAM)&iaData);
		}
		break;

	case NM_ROW_TARGET_LIST :		//# (0) : Element, (1) : Row, (2) : Target Type, (3) : Target
		{
			CStringArray *psaData;
			psaData = (CStringArray *)lParam;

			CString strElement = psaData->GetAt(0);

			CUIntArray iaData;
			iaData.Add(0);
			iaData.Add(atoi(psaData->GetAt(1)));
			iaData.Add(atoi(psaData->GetAt(2)));
			iaData.Add(atoi(psaData->GetAt(3)));

			m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_TARGET_LIST, (LPARAM)&iaData);
		}
		break;

	case NM_ROW_VALVIEW_CHECK :		//# (0) : Element, (1) : Row, (2) : Val
		{
			CStringArray *psaData;
			psaData = (CStringArray *)lParam;

			CString strElement = psaData->GetAt(0);

			CUIntArray iaData;
			iaData.Add(0);
			iaData.Add(atoi(psaData->GetAt(1)));
			iaData.Add(atoi(psaData->GetAt(2)));

			m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_VALVIEW_CHECK, (LPARAM)&iaData);
		}
		break;

	case NM_ROW_MODIFY_BTN :	//# (0) : Element, (1) : Row, (2) : Display Row, (3) : Display Row => Top Screen Pos
		{
			CStringArray *psaData;
			psaData = (CStringArray *)lParam;

			CString strElement = psaData->GetAt(0);

			CUIntArray iaData;
			iaData.Add(0);
			iaData.Add(atoi(psaData->GetAt(1)));
			iaData.Add(atoi(psaData->GetAt(2)));
			iaData.Add(atoi(psaData->GetAt(3)));

			m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_MODIFY_BTN, (LPARAM)&iaData);
			m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_SELECT_CONDITION, (LPARAM)&iaData);	//Added by cyberahn @KOSCOM. 2006.08.03.
		}
		break;

	case NM_ROW_DELETE_BTN :	//# (0) : Element, (1) : Row(After Select) 
		{
			CStringArray *psaData;
			psaData = (CStringArray *)lParam;

			CString strElement = psaData->GetAt(0);

			CUIntArray iaData;
			iaData.Add(0);
			iaData.Add(atoi(psaData->GetAt(1)));

			int nElementIndex = m_ctrlGrid.GetElementIndex(strElement);
			iaData.Add(nElementIndex);

			m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_DELETE_BTN, (LPARAM)&iaData);
		}
		break;

	case NM_ALIGN_ELEMENT :
		{
			CUIntArray	iaData;

			//# Set Element Align!
			int nTotElementIndex = 0;
			nTotElementIndex = m_ctrlGrid.SetAlignElement(nTotElementIndex);

			iaData.Add(0);
			iaData.Add(nTotElementIndex);

			m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_ALIGN_ELEMENT, (LPARAM)&iaData);
		}
		break;
	case NM_ROW_UP_BTN :
		{			
			CUIntArray	*piaData;
			piaData = (CUIntArray*)lParam;
			m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_ROW_UP_BTN, (LPARAM)piaData);
		}
		break;
	case NM_ROW_DOWN_BTN :
		{
			CUIntArray	*piaData;
			piaData = (CUIntArray*)lParam;
			m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_ROW_DOWN_BTN, (LPARAM)piaData);
		}
		break;
	case NM_ROW_MAXUP_BTN :
		{	
			m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_ROW_MAXUP_BTN, (LPARAM)(CStringArray *)lParam);
		}
		break;
	case NM_ROW_MINDOWN_BTN :
		{				
			m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_ROW_MINDOWN_BTN, (LPARAM)(CStringArray *)lParam);
		}
		break;

	default :
		break;
	}
	return TRUE;
}

void CGridCommonView::SetGroupFocus()
{
	m_ctrlGrid.SetHeadColor(TRUE);
	Invalidate();
}

void CGridCommonView::SetGroupKillFocus()
{
	m_ctrlGrid.SetHeadColor(FALSE);
	m_ctrlGrid.SetUnSelectRow();
	Invalidate();
}

void CGridCommonView::SetSelectRow(int nRow)
{
	m_ctrlGrid.SetSelectRow(nRow);
}

void CGridCommonView::SetUnSelectRow(int nRow, BOOL bDel)
{
	m_ctrlGrid.SetUnSelectRow(nRow, bDel);
}

void CGridCommonView::SetDetailViewState(BOOL bDetail)
{
	m_ctrlGrid.SetDetailViewState(bDetail);
}

void CGridCommonView::SetAddCondition(BOOL		bState,					//# Condition View
									  int			nElementIndex,	//# Element Index
									  BOOL		bNot,						//# Not State 
									  int			nTargetType,		//# Target Type
									  int			nTargetIndex,		//# Target Index
									  BOOL		bTargetEnable,	//# Target Enable
									  CString strItem,				//# 완성 구문 
									  BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
									  BOOL		bFieldVal)			//# 값보기
{
	if (nTargetType == 2)		//# 일간 고정 
		bTargetEnable = TRUE;

	m_ctrlGrid.SetAddRow(bState,					//# Condition View
		nElementIndex,	//# Element Index
		bNot,						//# Not State 
		nTargetType,		//# Target Type
		nTargetIndex,		//# Target Index
		bTargetEnable,	//# Target Enable
		strItem,				//# 완성 구문 
		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
		bFieldVal);			//# 값보기
}

void CGridCommonView::SetInsertCondition(int nRow,
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
	if (nTargetType == 2)		//# 일간 고정 
		bTargetEnable = TRUE;

	m_ctrlGrid.SetInsertRow(nRow,
		bState,					//# Condition View
		nElementIndex,	//# Element Index
		bNot,						//# Not State 
		nTargetType,		//# Target Type
		nTargetIndex,		//# Target Index
		bTargetEnable,	//# Target Enable
		strItem,				//# 완성 구문 
		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
		bFieldVal);			//# 값보기
}


void CGridCommonView::SetModifyCondition(int nRow,
										 BOOL		bState,					//# Condition View
										 CString	strElement,			//# Element
										 BOOL		bNot,						//# Not State 
										 int			nTargetType,		//# Target Type
										 int			nTargetIndex,		//# Target Index
										 BOOL		bTargetEnable,	//# Target Enable
										 CString strItem,				//# 완성 구문 
										 BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
										 BOOL		bFieldVal)			//# 값보기
{
	if (nTargetType == 2)		//# 일간 고정 
		bTargetEnable = TRUE;

	m_ctrlGrid.SetModifyRow(nRow,
		bState,					//# Condition View
		strElement,			//# Element
		bNot,						//# Not State 
		nTargetType,		//# Target Type
		nTargetIndex,		//# Target Index
		bTargetEnable,	//# Target Enable
		strItem,				//# 완성 구문 
		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
		bFieldVal);			//# 값보기
}

void CGridCommonView::SetModifyCondition(int nRow, 
										 BOOL bNot,				//# Not State 
										 CString strItem,	//# 완성 구문
										 BOOL bFieldVal)
{
	m_ctrlGrid.SetModifyRow(nRow, bNot, strItem, bFieldVal);
}

BOOL CGridCommonView::SetDeleteCondition(int nRow)
{
	BOOL bResult;

	bResult = m_ctrlGrid.SetDeleteRow(nRow);

	return bResult;
}

CString CGridCommonView::GetCellDate(int nCol, int nRow)
{
	CString strResult = "";
	strResult = m_ctrlGrid.GetCellDate(nCol, nRow);
	return strResult;
}

CString CGridCommonView::GetElementInfo(int nRow)
{
	CString strResult = "";
	strResult = m_ctrlGrid.GetRowToElement(nRow);
	return strResult;
}


void CGridCommonView::SetCellDate(int nCol, int nRow, CString strData)
{
	m_ctrlGrid.SetCellDate(nCol, nRow, strData);
}


void CGridCommonView::SetInit()
{
	m_ctrlGrid.ClearAll();
}


int CGridCommonView::SetAlignElement()
{
	int nResult = 0;
	nResult = m_ctrlGrid.SetAlignElement(0);

	return nResult;
}

void CGridCommonView::SetSameTargetBase(int nIndex, int nOutType)
{
	BOOL bEnable;
	bEnable = (nIndex < 0) ? FALSE : TRUE;

	m_ctrlGrid.SetModifyAllTargetCol(nIndex, nOutType, bEnable);
}

void CGridCommonView::SetUserDragDrop(BOOL bState)
{
	m_ctrlGrid.m_bUserDragDrop = bState;
}

HBRUSH CGridCommonView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(COLOR_BLACK);

		if (pWnd->GetDlgCtrlID() == IDC_BTN_LABEL)
		{
			if (m_ctrlGrid.m_bSetFocus == TRUE)
			{
				pDC->SetBkColor(m_clrLabelFocus);
				hbr = m_brushLabelFocus;
			}
			else if (m_ctrlGrid.m_bSetFocus == FALSE)
			{
				pDC->SetBkColor(m_clrLabel);
				hbr = m_brushLabel;
			}
		}
		else if (pWnd->GetDlgCtrlID() == IDC_FOOTER)
		{
			if (m_ctrlGrid.m_bSetFocus == TRUE)
			{
				pDC->SetBkColor(m_clrFooter_Focus);
				hbr = m_brushFooter_Focus;
			}
			else if (m_ctrlGrid.m_bSetFocus == FALSE)
			{
				pDC->SetBkColor(m_clrFooter);
				hbr = m_brushFooter;
			}
		}
		else if(pWnd->GetDlgCtrlID() == IDC_TARGET_STATIC_INDICATE)
		{
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)m_brushTagetEdit;
		}		
	}

	return hbr;
}

void CGridCommonView::OnFooterClk()
{
	m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_SELECT_GROUP, (int)0);
}

void CGridCommonView::ChangeSkinColor(ST_GRIDCOLOR stGridColor)
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

	m_ctrlGrid.ChangeGridColor(stGridColor);
	m_ctrlGrid.RedrawAll();
}

void CGridCommonView::SetChagneTargetEditIndicate(CString strIndicate)
{
	m_staticTargetEditIndicate.SetWindowText(strIndicate);
	m_staticTargetEditIndicate.Invalidate();
} 

void CGridCommonView::OnChangeSubject()
{
	m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_CHANGE_SUBJECT, NULL);
}

void CGridCommonView::OnChangeSubjectInit()
{
	m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_CHANGE_SUBJECT_INIT, NULL);
}