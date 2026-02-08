// GridOrderWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "GridOrderWnd.h"

#include "GridOrderView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*##############################################################
##																														##
##							Condition Make Grid Control										##
##																														## 
##																														##
##								작성일 : 2003. 7 ~													##
##								작성자 : 우동우 (TEXEN SYSTEM)							##
##																														##  
################################################################*/
extern HINSTANCE hDllInstance;

#define IDC_GRID								100
#define IDC_FOOTER							200
#define	IDC_BTN_LABEL					  300

#define IDC_STARTBONG_EDIT			1000
#define IDC_ENDBONG_EDIT				1100

#define IDC_BTN_UPINSERT				1200
#define IDC_BTN_DOWNINSERT			1300
#define IDC_BTN_DELETE					1400
#define IDC_BTN_ADD						  1500
#define	IDC_BTN_MENU					  1600

/////////////////////////////////////////////////////////////////////////////
// CGridOrderWnd

CGridOrderWnd::CGridOrderWnd()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);
	
	m_nStartBongVal = 1;
	m_nEndBongVal = 1;
	
	m_bFocus = FALSE;
	
	m_bHightlight = FALSE;
	
	m_ctrlGrid.m_nDefRowHeadHeight = 0;
}

CGridOrderWnd::~CGridOrderWnd()
{
	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();
}

BEGIN_MESSAGE_MAP(CGridOrderWnd, CWnd)
//{{AFX_MSG_MAP(CGridOrderWnd)
ON_WM_CREATE()
ON_WM_SIZE()
ON_WM_LBUTTONDOWN()
ON_WM_CTLCOLOR()
ON_WM_DESTROY()
ON_WM_PAINT()
//}}AFX_MSG_MAP

ON_EN_CHANGE(IDC_STARTBONG_EDIT, OnChangeStartBong)
ON_EN_CHANGE(IDC_ENDBONG_EDIT, OnChangeEndBong)

ON_EN_SETFOCUS(IDC_STARTBONG_EDIT, OnSetFocusStartBong)
ON_EN_SETFOCUS(IDC_ENDBONG_EDIT, OnSetFocusEndBong)

ON_EN_KILLFOCUS(IDC_STARTBONG_EDIT, OnKillFocusStartBong)
ON_EN_KILLFOCUS(IDC_ENDBONG_EDIT, OnKillFocusEndBong)

ON_STN_CLICKED(IDC_GRID, OnClickGrid)

ON_BN_CLICKED(IDC_BTN_UPINSERT, OnUpInsertBtn)
ON_BN_CLICKED(IDC_BTN_DOWNINSERT, OnDownInsertBtn)
ON_BN_CLICKED(IDC_BTN_DELETE, OnDeleteBtn)
ON_BN_CLICKED(IDC_BTN_ADD, OnAddBtn)

ON_MESSAGE(WM_GRID_EVENT, OnNotifyGridEvent)

ON_BN_CLICKED(IDC_BTN_MENU, OnMenuBtn)
/*ON_BN_CLICKED(IDC_FOOTER, OnFooterClk)*/
ON_BN_CLICKED(IDC_BTN_LABEL, OnFooterClk)

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGridOrderWnd message handlers

int CGridOrderWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pParentWnd = this;
	m_pParentWnd = m_pParentWnd->GetParent();
	
	m_ctrlGrid.m_bGridType = TRUE;
	m_ctrlGrid.CreateGrid(
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SS_SIMPLE,//|WS_BORDER,
		CRect(0, 0, 0, 0),
		this,
		IDC_GRID);
	m_ctrlGrid.SetDefFont(&m_fontDefault);
	m_ctrlGrid.m_bSearchMode = FALSE;		//#<= 순차 검색
	
	CWnd* pWndConditionMake = m_pParentWnd->GetParent();
	pWndConditionMake = pWndConditionMake->GetParent();
	pWndConditionMake = pWndConditionMake->GetParent();
	
	CString strImgPath;
	m_btnAddGroup.Create(_T(""), CRect(0, 0, 0, 0), this, IDC_BTN_MENU);
	m_btnAddGroup.SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | WS_EX_TOPMOST);
	strImgPath = "CS_Group_Add";
	pWndConditionMake->SendMessage(NM_SETBTNIMG, (WPARAM)&m_btnAddGroup, (LPARAM)(LPCTSTR)strImgPath);
	
	m_ctrlFooter.Create(_T(""), 
		WS_CHILD | WS_VISIBLE | SS_CENTER | SS_NOTIFY,  // | WS_BORDER
		CRect(0, 0, 0, 0), this, IDC_FOOTER);
	m_ctrlFooter.SetFont(&m_fontDefault);
	m_ctrlFooter.SetWindowPos(&m_btnAddGroup, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	
	CString strTemp;
	strTemp.Format("순차검색 %2d", m_nGroupNo);
	m_btnLabel.Create(strTemp, 
		WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE | SS_NOTIFY ,// | WS_BORDER
		CRect(0, 0, 0, 0), this, IDC_BTN_LABEL);
	m_btnLabel.SetFont(&m_fontBold);
	
	m_editStartBong.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
		CRect(0, 0, 0, 0), 
		this, 
		IDC_STARTBONG_EDIT);
	m_editStartBong.SetFont(&m_fontDefault);
	
	m_ctrlText1.Create("~",
		WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlText1.SetFont(&m_fontDefault);
	
	m_editEndBong.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
		CRect(0, 0, 0, 0), 
		this, 
		IDC_ENDBONG_EDIT);
	m_editEndBong.SetFont(&m_fontDefault);
	
	m_ctrlText2.Create("봉안에 위의 조건이 만족한 후에",
		WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlText2.SetFont(&m_fontDefault);
	
	//# Set Def Bong Val
	SetFooterVal(m_nStartBongVal, m_nEndBongVal);

	m_btnUpInsertGroup.Create(_T(""), CRect(0, 0, 0, 0), this, IDC_BTN_UPINSERT);
	m_btnUpInsertGroup.SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | WS_EX_TOPMOST);
	strImgPath = "CS_Group_Up";
	pWndConditionMake->SendMessage(NM_SETBTNIMG, (WPARAM)&m_btnUpInsertGroup, (LPARAM)(LPCTSTR)strImgPath);
		
	m_btnDownInsertGroup.Create(_T(""),	CRect(0, 0, 0, 0), this, IDC_BTN_DOWNINSERT);
	m_btnDownInsertGroup.SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | WS_EX_TOPMOST);
	strImgPath = "CS_Group_Down";
	pWndConditionMake->SendMessage(NM_SETBTNIMG, (WPARAM)&m_btnDownInsertGroup, (LPARAM)(LPCTSTR)strImgPath);
		
	m_btnDeleteGroup.Create(_T(""), CRect(0, 0, 0, 0), this, IDC_BTN_DELETE);
	m_btnDeleteGroup.SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | WS_EX_TOPMOST);
	strImgPath = "CS_Group_Del";
	pWndConditionMake->SendMessage(NM_SETBTNIMG, (WPARAM)&m_btnDeleteGroup, (LPARAM)(LPCTSTR)strImgPath);
		
	strTemp.Format("%d/%d", m_nGroupNo, m_nGroupNo);
	m_ctrlGroupNo.Create(strTemp,
		WS_CHILD | WS_VISIBLE | SS_RIGHT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlGroupNo.SetFont(&m_fontDefault);
	
	return 0;
}

void CGridOrderWnd::OnPaint()
{
	CPaintDC dc(this);

	CRgn rgnBackGround;
	rgnBackGround.CreateRectRgnIndirect(m_reBackGround);

	CRgn rgnBtn;
	rgnBtn.CreateRectRgnIndirect(m_reBtn);

	rgnBackGround.CombineRgn(&rgnBackGround, &rgnBtn, RGN_DIFF);

	CBrush brush;
	if (m_bFocus == TRUE)
	{
		brush.CreateSolidBrush(COLOR_LABEL_FOCUS);
		dc.FillRgn(&rgnBackGround, &brush);
	}
	else if (m_bFocus == FALSE)
	{
		brush.CreateSolidBrush(COLOR_LABEL);
		dc.FillRgn(&rgnBackGround, &brush);
	}
}

void CGridOrderWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CGridOrderWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect reClient;
	GetClientRect(reClient);
	OnSize(reClient);
}

void CGridOrderWnd::OnSize(CRect reClient)
{
	CRect reTop;
	
	reTop = reClient;
	reTop.bottom = reTop.top + FOOTER_DEF_HEIGHT;
	OnSize_Footer(reTop);
	
	reTop.top = reTop.bottom + 1;
	reTop.bottom = reClient.bottom;
	m_ctrlGrid.MoveWindow(reTop);
}

void CGridOrderWnd::OnSize_Footer(CRect reClient)
{
	m_ctrlFooter.MoveWindow(reClient);

	m_reBackGround = reClient;
	m_ctrlFooter.ShowWindow(SW_HIDE);
	
	CRect reLeft, reTemp;
	reLeft = reClient;
	
	reLeft.top += 3;
	reLeft.bottom -= 3;
	
	reLeft.left += 2;
	reLeft.right = reLeft.left + 80;
	m_btnLabel.MoveWindow(reLeft);
	
	reLeft.top += 1;
	reLeft.bottom -= 1;

	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + 20;
	m_btnAddGroup.MoveWindow(reLeft);

	m_reBtn = reLeft;
	
	reTemp = reLeft;
	reTemp.bottom = reTemp.top + 20;
	reTemp.left = reTemp.right + 40;
	reTemp.right = reTemp.left + 30;
	m_editStartBong.MoveWindow(reTemp);
	
	reTemp.top += 3;
	reTemp.bottom = reTemp.top + 12;
	reTemp.left = reTemp.right + 4;
	reTemp.right = reTemp.left + 10;
	m_ctrlText1.MoveWindow(reTemp);
	
	reTemp.top -= 2;
	reTemp.bottom = reTemp.top + 20;
	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 30;
	m_editEndBong.MoveWindow(reTemp);
	
	reTemp.top += 3;
	reTemp.bottom = reTemp.top + 15;
	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 200;
	m_ctrlText2.MoveWindow(reTemp);
}

void CGridOrderWnd::OnSize_Footer_Old(CRect reClient)
{
	CRect reFooter;
	reFooter = reClient;
	reFooter.bottom = reClient.bottom;
	reFooter.top = reFooter.bottom - FOOTER_DEF_HEIGHT;
	
	if (m_ctrlFooter)
		m_ctrlFooter.MoveWindow(reFooter);
	
	CRect reEdit, reText;
	if ((m_editStartBong) && (m_editEndBong))
	{
		reEdit.top = reFooter.top + 4;
		reEdit.bottom = reFooter.bottom - 4;
		
		reText.top = reFooter.top + 7;
		reText.bottom = reFooter.bottom - 2;
		
		reEdit.left = reFooter.left + 5;
		reEdit.right = reEdit.left + 30;
		m_editStartBong.MoveWindow(reEdit);
		
		reText.left = reEdit.right + 3;
		reText.right = reText.left + 10;
		m_ctrlText1.MoveWindow(reText);
		reEdit.right += reText.right - reText.left;
		
		reEdit.left = reEdit.right + 2;
		reEdit.right = reEdit.left + 30;
		m_editEndBong.MoveWindow(reEdit);	
		
		reText.left = reEdit.right + 3;
		reText.right = reText.left + 200;
		m_ctrlText2.MoveWindow(reText);
	}
	
	reText.top = reFooter.top + 7;
	reText.bottom = reFooter.bottom - 2;
	
	reText.right = reFooter.right - 5;
	reText.left = reText.right - 20;
	m_ctrlGroupNo.MoveWindow(reText);
	reFooter.left = reText.left;
	
	reFooter.top += 2;
	reFooter.bottom -= 2;
	
	int nBtnWidth = 20;
	
	reFooter.right = reFooter.left - 2;
	reFooter.left = reFooter.right - nBtnWidth;
	m_btnAddGroup.MoveWindow(reFooter);
	
	reFooter.right = reFooter.left - 2;
	reFooter.left = reFooter.right - nBtnWidth;
	m_btnDeleteGroup.MoveWindow(reFooter);
	
	reFooter.right = reFooter.left - 2;
	reFooter.left = reFooter.right - nBtnWidth;
	m_btnDownInsertGroup.MoveWindow(reFooter);
	
	reFooter.right = reFooter.left - 2;
	reFooter.left = reFooter.right - nBtnWidth;
	m_btnUpInsertGroup.MoveWindow(reFooter);
	
	if (m_nGroupNo == 1)
	{
		m_btnUpInsertGroup.ShowWindow(SW_HIDE);
		m_btnDownInsertGroup.ShowWindow(SW_HIDE);
		m_btnDeleteGroup.ShowWindow(SW_HIDE);
	}
	
	reClient.bottom -= FOOTER_DEF_HEIGHT;
	if (m_ctrlGrid)
		m_ctrlGrid.MoveWindow(reClient);
}


void CGridOrderWnd::SetFooterVal(int nStartBong, int nEndBong)
{
	CString strTemp;
	
	strTemp.Format("%d", nStartBong);
	m_editStartBong.SetWindowText(strTemp);
	
	strTemp.Format("%d", nEndBong);
	m_editEndBong.SetWindowText(strTemp);
}

void CGridOrderWnd::GetFooterVal(int &nStartBong, int &nEndBong)
{
	CString strTemp;
	
	m_editStartBong.GetWindowText(strTemp);
	nStartBong = atoi(strTemp);
	
	m_editEndBong.GetWindowText(strTemp);
	nEndBong = atoi(strTemp);
}


void CGridOrderWnd::OnChangeStartBong()
{
	CString strTemp;
	strTemp.Format("\n# Group : %d => OnChangeStartBong", m_nGroupNo);
	TRACE(strTemp);
}

void CGridOrderWnd::OnChangeEndBong()
{
	CString strTemp;
	strTemp.Format("\n# Group : %d => OnChangeEndBong", m_nGroupNo);
	TRACE(strTemp);
}

void CGridOrderWnd::OnSetFocusStartBong()
{
	CString strTemp;
	strTemp.Format("\n# Group : %d => OnSetFocusStartBong", m_nGroupNo);
	TRACE(strTemp);
	
	m_editStartBong.GetWindowText(strTemp);
	m_nStartBongVal = atoi(strTemp);
	
	m_editStartBong.SetSel(0, strTemp.GetLength());
}

void CGridOrderWnd::OnSetFocusEndBong()
{
	CString strTemp;
	strTemp.Format("\n# Group : %d => OnSetFocusEndBong", m_nGroupNo);
	TRACE(strTemp);
	
	m_editEndBong.GetWindowText(strTemp);
	m_nEndBongVal = atoi(strTemp);
	
	m_editEndBong.SetSel(0, strTemp.GetLength());
}

void CGridOrderWnd::OnKillFocusStartBong()
{
	CString strTemp;
	strTemp.Format("\n# Group : %d => OnKillFocusStartBong", m_nGroupNo);
	TRACE(strTemp);
	
	CString strSBong, strEBong;
	CUIntArray iaData;
	iaData.Add(m_nGroupNo);
	
	m_editStartBong.GetWindowText(strSBong);
	m_editEndBong.GetWindowText(strEBong);
	if (atoi(strSBong) > atoi(strEBong))
	{
		iaData.Add(atoi(strSBong));
		m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_ENDBONG_VAL, (LPARAM)&iaData);
		
		m_editEndBong.SetWindowText(strSBong);
		
		/*
		strTemp.Format("[Group : %d] => '봉전 기준'은 '봉이내'보다 같거나작아야합니다.", m_nGroupNo);
		AfxMessageBox(strTemp);
		
		  strTemp.Format("%d", m_nStartBongVal);
		  m_editStartBong.SetWindowText(strTemp);
		  m_editStartBong.SetFocus();
		*/
	}
	
	iaData.Add(atoi(strSBong));
	m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_STARTBONG_VAL, (LPARAM)&iaData);
}

void CGridOrderWnd::OnKillFocusEndBong()
{
	CString strTemp;
	strTemp.Format("\n# Group : %d => OnKillFocusEndBong", m_nGroupNo);
	TRACE(strTemp);
	
	CString strSBong, strEBong;
	CUIntArray iaData;
	iaData.Add(m_nGroupNo);
	
	m_editStartBong.GetWindowText(strSBong);
	m_editEndBong.GetWindowText(strEBong);
	if (atoi(strSBong) > atoi(strEBong))
	{
	/*
	//# 2005.08.31
	strTemp.Format("[Group : %d] => '봉전 기준'은 '봉이내'보다 같거나 커야합니다.", m_nGroupNo);
	MessageBox(strTemp, MB_OK);
		*/
		
		strTemp.Format("%d", m_nEndBongVal);
		m_editEndBong.SetWindowText(strTemp);
		m_editEndBong.SetFocus();
	}
	else
	{
		iaData.Add(atoi(strEBong));
		m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_ENDBONG_VAL, (LPARAM)&iaData);
	}
}

void CGridOrderWnd::OnUpInsertBtn()
{
	m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_GROUP_INSERTUP, (LPARAM)m_nGroupNo);		
}

void CGridOrderWnd::OnDownInsertBtn()
{
	m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_GROUP_INSERTDOWN, (LPARAM)m_nGroupNo);		
}

void CGridOrderWnd::OnDeleteBtn()
{
	m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_GROUP_DELETE, (LPARAM)m_nGroupNo);		
}

void CGridOrderWnd::OnAddBtn()
{
	m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_GROUP_ADD, (LPARAM)m_nGroupNo);		
}

void CGridOrderWnd::OnClickGrid()
{
/*
CString strTemp;
strTemp.Format("\n# Select Group : %d", m_nGroupNo);
AfxMessageBox(strTemp);
	*/
	
	//# Select Grid Focus!
	
	m_bHightlight = !m_bHightlight;
	Invalidate();
}

void CGridOrderWnd::OnMenuBtn()			//Added by cyberahn. @KOSCOM 2006.07.14.
{
	CMenu	pPopupMenu;
	CRect	rectButton;
	pPopupMenu.CreatePopupMenu(); 
	pPopupMenu.AppendMenu( MF_STRING | MF_ENABLED, IDC_BTN_DOWNINSERT, "순차조건그룹 추가");
	pPopupMenu.AppendMenu( MF_STRING | MF_ENABLED, IDC_BTN_UPINSERT,	"순차조건그룹 삽입");
	pPopupMenu.AppendMenu( MF_STRING | MF_ENABLED, IDC_BTN_DELETE, "순차조건그룹 삭제");
	m_btnAddGroup.GetWindowRect(&rectButton);
	pPopupMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
		rectButton.left, rectButton.bottom, this);
	pPopupMenu.DestroyMenu();
}

//Footer 마우스 클릭시 메뉴랑 포커스 조정.
void CGridOrderWnd::OnFooterClk()		//Added by cyberahn. @KOSCOM 2006.07.14.
{
	POINT pt;
	CRect reBtnAdd, reStartBong, reEndBong;
	
	GetCursorPos(&pt);
	m_btnAddGroup.GetWindowRect(&reBtnAdd);
	m_editStartBong.GetWindowRect(&reStartBong);
	m_editEndBong.GetWindowRect(&reEndBong);
	
	if (PtInRect(reBtnAdd, pt))
		OnMenuBtn();
	else if (PtInRect(reStartBong, pt))
	{
		SetFocus(TRUE);
		m_editStartBong.SetFocus();
	}
	else if (PtInRect(reEndBong, pt))
	{
		SetFocus(TRUE);
		m_editEndBong.SetFocus();
	}
	else
		SetFocus(TRUE);
}

void CGridOrderWnd::SetFocus(BOOL bSendMsg)
{
	if (bSendMsg == TRUE)
		m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_SELECT_GROUP, (LPARAM)m_nGroupNo);
	
	m_bFocus = TRUE;
	m_ctrlGrid.SetHeadColor(m_bFocus);
	Invalidate();
}

void CGridOrderWnd::SetKillFocus()
{
	m_bFocus = FALSE;
	m_ctrlGrid.SetHeadColor(m_bFocus);
	m_ctrlGrid.SetUnSelectRow();
	Invalidate();
}

LRESULT CGridOrderWnd::OnNotifyGridEvent(WPARAM wParam, LPARAM lParam)
{
	//# WPARAM wParam  ==>  Event Name
	//# LPARAM lParam  ==>  NULL
	
	switch(wParam)
	{
	case NM_GRID_SETFOCUS :
		SetFocus();
		break;
		
		/*
		case NM_GRID_KILLSETFOCUS :
		SetKillFocus();
		break;
		*/
		
	case NM_ROW_SELECT :
		{
			int nSelectRow = (int)lParam;
			
			CUIntArray iaData;
			iaData.Add(m_nGroupNo);
			iaData.Add(nSelectRow);
			iaData.Add(0);
			
			m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_SELECT_CONDITION, (LPARAM)&iaData);
		}
		break;

	case NM_UPDATE :
		{
			m_pParentWnd->SendMessage(WM_MAIN_EVENT, (WPARAM)NM_UPDATE, NULL);
			break;
		}
		
	case NM_ROW_MAXMIN :	//# View (MAX/MIN)
		{
			CUIntArray iaData;
			iaData.Add(m_nGroupNo);
			iaData.Add(GetWndTotHeight((BOOL)lParam));
			
			CRect reClient;
			GetClientRect(reClient);
			
			reClient.bottom = GetWndTotHeight((BOOL)lParam);
			OnSize(reClient);
			Invalidate();
			
			m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_CONDITION_VIEW, (LPARAM)&iaData);		
		}
		break;
		
	case NM_ROW_DRAGDROP :		//# (0) : Old Row, (1) : New Row
		{
			CUIntArray* piaData;
			piaData = (CUIntArray*)lParam;
			
			CUIntArray iaData;
			iaData.Add(m_nGroupNo);
			iaData.Add(piaData->GetAt(0));
			iaData.Add(piaData->GetAt(1));
			
			m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_CONDITION_DRAGDROP, (LPARAM)&iaData);
		}
		break;
		
	case NM_HEAD_VALVIEWALL_CHECK	:
		{
			CUIntArray iaData;
			iaData.Add(m_nGroupNo);
			iaData.Add((int)lParam);
			
			m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_VALVIEWALL_CHECK, (LPARAM)&iaData);
		}
		break;
		
	case NM_HEAD_DELETEALL :
		m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_DELETEALL, (LPARAM)m_nGroupNo);
		break;
		
	case NM_ROW_CONDITION_CHECK :		//# (0) : Element, (1) : Row, (2) : Val
		{
			CStringArray *psaData;
			psaData = (CStringArray *)lParam;
			
			CString strElement = psaData->GetAt(0);
			
			CUIntArray iaData;
			iaData.Add(m_nGroupNo);
			iaData.Add(atoi(psaData->GetAt(1)));
			iaData.Add(atoi(psaData->GetAt(2)));
			
			m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_CONDITION_CHECK, (LPARAM)&iaData);
		}
		break;
		
	case NM_ROW_TARGET_LIST_START :
		{
			
		}
		break;
		
	case NM_ROW_TARGET_LIST :		//# (0) : Element, (1) : Row, (2) : Target Type, (3) : Target
		{
			CStringArray *psaData;
			psaData = (CStringArray *)lParam;
			
			CString strElement = psaData->GetAt(0);
			
			CUIntArray iaData;
			iaData.Add(m_nGroupNo);
			iaData.Add(atoi(psaData->GetAt(1)));
			iaData.Add(atoi(psaData->GetAt(2)));
			iaData.Add(atoi(psaData->GetAt(3)));
			
			m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_TARGET_LIST, (LPARAM)&iaData);
		}
		break;
		
	case NM_ROW_VALVIEW_CHECK :		//# (0) : Element, (1) : Row, (2) : Val
		{
			CStringArray *psaData;
			psaData = (CStringArray *)lParam;
			
			CString strElement = psaData->GetAt(0);
			
			CUIntArray iaData;
			iaData.Add(m_nGroupNo);
			iaData.Add(atoi(psaData->GetAt(1)));
			iaData.Add(atoi(psaData->GetAt(2)));
			
			m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_VALVIEW_CHECK, (LPARAM)&iaData);
		}
		break;
		
	case NM_ROW_MODIFY_BTN :	//# (0) : Element, (1) : Row, (2) : Display Row, (3) : Display Row => Top Screen Pos
		{
			CStringArray *psaData;
			psaData = (CStringArray *)lParam;
			
			CString strElement = psaData->GetAt(0);
			
			CUIntArray iaData;
			iaData.Add(m_nGroupNo);
			iaData.Add(atoi(psaData->GetAt(1)));
			iaData.Add(atoi(psaData->GetAt(2)));
			iaData.Add(atoi(psaData->GetAt(3)));
			
			m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_MODIFY_BTN, (LPARAM)&iaData);
			m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_SELECT_CONDITION, (LPARAM)&iaData);	//Added by cyberahn@KOSCOM 2006.08.03.
		}
		break;
		
	case NM_ROW_DELETE_BTN :	//# (0) : Element, (1) : Row(After Select) 
		{
			CStringArray *psaData;
			psaData = (CStringArray *)lParam;
			
			CString strElement = psaData->GetAt(0);
			int nSelectRow = atoi(psaData->GetAt(1));
			
			CUIntArray iaData;
			iaData.Add(m_nGroupNo);
			iaData.Add(nSelectRow);
			iaData.Add(GetWndTotHeight());
			
			int nElementIndex = m_ctrlGrid.GetElementIndex(strElement);
			iaData.Add(nElementIndex);
			
			m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_DELETE_BTN, (LPARAM)&iaData);
		}
		break;
	case NM_ROW_UP_BTN:
	case NM_ROW_DOWN_BTN:
		{
			CUIntArray* iaData = (CUIntArray*)lParam;
			iaData->SetAt(0, m_nGroupNo);
			m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_ROW_UP_BTN, (LPARAM)iaData);				
		}
		break;
	case NM_ROW_MAXUP_BTN:
		{
			CStringArray* psaData = (CStringArray*)lParam;
			CString strGroupNo;
			strGroupNo.Format("%d", m_nGroupNo);
			psaData->SetAt(0, strGroupNo);
			m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_ROW_MAXUP_BTN, (LPARAM)psaData);			
		}
		break;
	case NM_ROW_MINDOWN_BTN:
		{
			CStringArray* psaData = (CStringArray*)lParam;
			CString strGroupNo;
			strGroupNo.Format("%d", m_nGroupNo);
			psaData->SetAt(0, strGroupNo);
			m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_ROW_MINDOWN_BTN, (LPARAM)psaData);			
		}			
		break;
		
		//#############################################################################################
		
	case NM_ALIGN_ELEMENT :
		m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_ALIGN_ELEMENT, (LPARAM)-1);
		break;
		
	case NM_GROUP_ADD	:
		m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_GROUP_ADD, (LPARAM)m_nGroupNo);
		break;
		
	case NM_GROUP_INSERTUP : 
		m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_GROUP_INSERTUP, (LPARAM)m_nGroupNo);
		break;
		
	case NM_GROUP_INSERTDOWN : 
		m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_GROUP_INSERTDOWN, (LPARAM)m_nGroupNo);
		break;
		
	case NM_GROUP_DELETE :
		m_pParentWnd->SendMessage(WM_WND_EVENT, (WPARAM)NM_GROUP_DELETE, (LPARAM)m_nGroupNo);
		break;
		
	default :
		break;
	}
	return TRUE;
}

void CGridOrderWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnLButtonDown(nFlags, point);
}

HBRUSH CGridOrderWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	CGridOrderView	*pviewGridOrder = (CGridOrderView*)GetParent();
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(COLOR_BLACK);
		
		if (pWnd->GetDlgCtrlID() == IDC_BTN_LABEL)
		{
			if (m_bFocus == TRUE)
			{
				pDC->SetBkColor(COLOR_LABEL_FOCUS);
				hbr = pviewGridOrder->m_brushLabelFocus;
			}
			else if (m_bFocus == FALSE)
			{
				pDC->SetBkColor(COLOR_LABEL);
				hbr = pviewGridOrder->m_brushLabel;
			}
		}
		else
		{
			if (m_bFocus == TRUE)
			{
				pDC->SetBkColor(COLOR_FOOTER_FOCUS);
				hbr = pviewGridOrder->m_brushFooter_Focus;
			}
			else if (m_bFocus == FALSE)
			{
				pDC->SetBkColor(COLOR_FOOTER);
				hbr = pviewGridOrder->m_brushFooter;
			}
		}
	}
	
	/*
	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC :
	pDC->SetTextColor(COLOR_BLACK);
	
	  switch (pWnd->GetDlgCtrlID()) 
	  {
	  case IDC_BTN_LABEL:									//Added by cyberahn @KOSCOM 2006.07.14.
	  if (m_bFocus == TRUE)
	  {
	  pDC->SetBkColor(COLOR_LABEL_FOCUS);
	  hbr = m_brushLabelFocus;
	  }
	  else if (m_bFocus == FALSE)
	  {
	  pDC->SetBkColor(COLOR_LABEL);
	  hbr = m_brushLabel;
	  }
	  break;
	  default:
	  if (m_bFocus == TRUE)
	  {
	  pDC->SetBkColor(COLOR_FOOTER_FOCUS);
	  hbr = pviewGridOrder->m_brushFooter_Focus;
	  }
	  else if (m_bFocus == FALSE)
	  {
	  pDC->SetBkColor(COLOR_FOOTER);
	  hbr = pviewGridOrder->m_brushFooter;
	  }
	  break;
	  }
	  }
	*/
	return hbr;
}

int CGridOrderWnd::SetAddCondition(BOOL		bState,					//# Condition View
								   int			nElementIndex,	//# Element Index
								   BOOL		bNot,						//# Not State 
								   int			nTargetType,		//# Target Type
								   int			nTargetIndex,		//# Target Index
								   CString strItem,				//# 완성 구문 
								   BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
								   BOOL		bFieldVal)			//# 값보기
{
	BOOL bTargetEnable = FALSE;	//# Target Enable
	if (nTargetType == 2)		//# 일간 고정
		bTargetEnable = TRUE;
	
	//# return => Grid Total Height
	m_ctrlGrid.SetAddRow(bState,					//# Condition View
		nElementIndex,	//# Element Index
		bNot,						//# Not State 
		nTargetType,		//# Target Type
		nTargetIndex,		//# Target Index
		bTargetEnable,
		strItem,				//# 완성 구문 
		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
		bFieldVal);			//# 값보기
	
	return GetWndTotHeight();
}

int CGridOrderWnd::SetInsertCondition(int nRow,
									  BOOL		bState,					//# Condition View
									  int			nElementIndex,	//# Element Index
									  BOOL		bNot,						//# Not State 
									  int			nTargetType,		//# Target Type
									  int			nTargetIndex,		//# Target Index
									  CString strItem,				//# 완성 구문 
									  BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
									  BOOL		bFieldVal)			//# 값보기
{
	BOOL bTargetEnable = FALSE;	//# Target Enable
	if (nTargetType == 2)		//# 일간 고정
		bTargetEnable = TRUE;
	
	//# return => Grid Total Height
	m_ctrlGrid.SetInsertRow(nRow,
		bState,					//# Condition View
		nElementIndex,	//# Element Index
		bNot,						//# Not State 
		nTargetType,		//# Target Type
		nTargetIndex,		//# Target Index
		bTargetEnable,
		strItem,				//# 완성 구문 
		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
		bFieldVal);			//# 값보기
	
	return GetWndTotHeight();
}


int CGridOrderWnd::SetModifyCondition(int nRow,
									  BOOL		bState,					//# Condition View
									  CString	strElement,			//# Element
									  BOOL		bNot,						//# Not State 
									  int			nTargetType,		//# Target Type
									  int			nTargetIndex,		//# Target Index
									  CString strItem,				//# 완성 구문 
									  BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
									  BOOL		bFieldVal)			//# 값보기
{
	BOOL bTargetEnable = FALSE;	//# Target Enable
	if (nTargetType == 2)		//# 일간 고정
		bTargetEnable = TRUE;
	
	//# return => Grid Total Height
	m_ctrlGrid.SetModifyRow(nRow,
		bState,					//# Condition View
		strElement,			//# Element
		bNot,						//# Not State 
		nTargetType,		//# Target Type
		nTargetIndex,		//# Target Index
		bTargetEnable,
		strItem,				//# 완성 구문 
		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
		bFieldVal);			//# 값보기
	
	return GetWndTotHeight();
}


void CGridOrderWnd::SetModifyCondition(int nRow, 
									   BOOL		bNot,			//# Not State 
									   CString strItem,	//# 완성 구문
									   BOOL bFieldVal)				
{
	m_ctrlGrid.SetModifyRow(nRow, bNot, strItem, bFieldVal);
}

BOOL CGridOrderWnd::SetDeleteCondition(int nRow)
{
	BOOL bResult;
	
	bResult = m_ctrlGrid.SetDeleteRow(nRow);
	
	return bResult;
}

CString CGridOrderWnd::GetCellDate(int nCol, int nRow)
{
	CString strResult = "";
	strResult = m_ctrlGrid.GetCellDate(nCol, nRow);
	return strResult;
}

CString CGridOrderWnd::GetElementInfo(int nRow)
{
	CString strResult = "";
	strResult = m_ctrlGrid.GetRowToElement(nRow);
	return strResult;
}

void CGridOrderWnd::SetCellDate(int nCol, int nRow, CString strData)
{
	m_ctrlGrid.SetCellDate(nCol, nRow, strData);
}

int CGridOrderWnd::GetWndTotHeight(BOOL bViewState)
{
	int nResult;
	
	//# Grid Client Size!
	int nHeadHeight = m_ctrlGrid.m_nDefRowHeadHeight;
	int nRowCnt = m_ctrlGrid.GetNumberRows();
	
	int nRowHeight = 0;
	if (bViewState == TRUE)
		nRowHeight= m_ctrlGrid.m_nDefRowHeight;
	
	nResult = nHeadHeight + nRowCnt * nRowHeight + (int)FOOTER_DEF_HEIGHT + 2;
	
	//## Set Resize Default Size ?
	
	return nResult;
}

void CGridOrderWnd::ChangeSkinColor(ST_GRIDCOLOR stGridColor)
{	
	if(!stGridColor.clrGridHeader)
		return;
	
	m_ctrlGrid.ChangeGridColor(stGridColor);
	m_ctrlGrid.RedrawAll();
	
}




