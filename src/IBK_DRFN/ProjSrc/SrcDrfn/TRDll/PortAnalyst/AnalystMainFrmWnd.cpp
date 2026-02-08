// AnalystMainFrmWnd.cpp : implementation file
//

#include "stdafx.h"
#include "AnalystMainFrmWnd.h"
#include "resource.h"
#include "AnalystResultWnd.h"
#include "ScreenWnd.h"
#include "../../../SrcSite/include/jmcode.h"

#ifdef _DEBUG	
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE g_hInstance;
/////////////////////////////////////////////////////////////////////////////
// CAnalystMainFrmWnd
CAnalystMainFrmWnd::CAnalystMainFrmWnd()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);

	m_clrMainSkin = COLOR_MAIN_CANVAS;
	m_brushStatic.CreateSolidBrush(m_clrMainSkin);
	
	m_ptabAnalystResult = NULL;
	m_pwndAnalystResult = NULL;
	
	m_bInit = FALSE;
	m_bExpFlag = FALSE;
	m_bRun = FALSE;
	
	m_nTrIndex = 0;

	//--> 통신
// 	m_pITrCommManager = NULL;
// 	BDMakeNull(m_pITrComm);
// 	BDMakeNull(m_pITrCommUP);
// 	BDMakeNull(m_pITrCommCon);
// 	m_pDebugMng = NULL;
	m_bRecMST = FALSE;
	//<-- 통신
}

CAnalystMainFrmWnd::~CAnalystMainFrmWnd()
{
	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();
	
	m_brushStatic.DeleteObject();
}

BEGIN_MESSAGE_MAP(CAnalystMainFrmWnd, CFrameWnd)
//{{AFX_MSG_MAP(CAnalystMainFrmWnd)
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_COPYDATA()
	//}}AFX_MSG_MAP

	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_ANANLUSTRESULT, OnNotify_ChangeTab)

	ON_BN_CLICKED(IDC_BTN_SELECTOK, OnBtnSelectOk)
	ON_BN_CLICKED(IDC_BTN_SELECTCANCEL, OnBtnSelectCancel)
	ON_BN_CLICKED(IDC_BTN_SEARCH_CODE, OnBtnSearch_Code)

	ON_BN_CLICKED(IDC_BTN_EXP, OnBtnExp)
	ON_BN_CLICKED(IDC_BTN_LISTALL, OnBtnListAll)
	ON_BN_CLICKED(IDC_BTN_LISTKOSPI, OnBtnKospi)
	ON_BN_CLICKED(IDC_BTN_LISTKOSDAQ, OnBtnKosdaq)

	ON_BN_CLICKED(IDC_BTN_SEARCH_RESULT, OnBtnSearch_Result)

	ON_BN_CLICKED(IDC_BTN_SEND, OnBtnSend)
	ON_COMMAND_RANGE(ID_MENU_RESULTCTRL_START, ID_MENU_RESULTCTRL_END, OnMenuResultCtrl)
	ON_MESSAGE(WM_NOTIFY_GRID, OnNotify_Grid)
	ON_MESSAGE(WM_NOTIFY_TREE, OnNotify_Tree)

	//# 삼성증권
//	ON_MESSAGE(WM_RP_RECEIVED, OnGetData)	-hidden-
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnalystMainFrmWnd message handlers

void CAnalystMainFrmWnd::SetWaitCursor(BOOL bState)
{
	if (bState == TRUE)
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	else
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));	
}

int CAnalystMainFrmWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	m_stcStatusBar.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
	m_stcStatusBar.ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	m_ctrlTitle.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE,
		CRect(0, 0, 0, 0), this, IDC_STATIC_TARGET);
	m_ctrlTitle.SetFont(&m_fontDefault);
	
	m_treeCode.Create(this, IDC_TREE_CODE);
	m_treeCode.SetFont(&m_fontDefault);
 	
	m_gridCode.Create(NULL, _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER,
		CRect(0, 0, 0, 0), this, NULL);
	m_gridCode.SetFont(&m_fontDefault);
	
	m_btnSelectOk.Create(_T("선택"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_ICON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_SELECTOK);
	m_btnSelectOk.SetFont(&m_fontBold);
	
	m_btnSelectCancel.Create(_T("해제"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_ICON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_SELECTCANCEL);
	m_btnSelectCancel.SetFont(&m_fontBold);
	
	m_btnListSearch.Create(_T("검색"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_ICON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_SEARCH_CODE);
	m_btnListSearch.SetFont(&m_fontBold);
	
	m_btnExp.Create(_T(""), CRect(0, 0, 20, 20), this, IDC_BTN_EXP);
	SetBtnImg(&m_btnExp, "LEFT_EX");
	
	m_btnList_All.Create(_T("전체"), CRect(0, 0, 0, 0), this, IDC_BTN_LISTALL);
	SetBtnImg(&m_btnList_All, 6, TRUE);
	m_btnList_All.SetTextColor(RGB(255, 0, 0));
	
	m_btnList_Kospi.Create(_T("거래소"), CRect(0, 0, 0, 0), this, IDC_BTN_LISTKOSPI);
	SetBtnImg(&m_btnList_Kospi, 6, TRUE);
	m_btnList_Kospi.SetTextColor(RGB(255, 0, 0));
	
	m_btnList_Kosdaq.Create(_T("코스닥"), CRect(0, 0, 0, 0), this, IDC_BTN_LISTKOSDAQ);
	SetBtnImg(&m_btnList_Kosdaq, 6, TRUE);
	m_btnList_Kosdaq.SetTextColor(RGB(255, 0, 0));
	
	m_gridResult.Create(NULL, _T(""), WS_CHILD | WS_VISIBLE | WS_BORDER | TVS_NOSCROLL /*| TVS_NOHSCROLL*/, CRect(0, 0, 0, 0), this, NULL);
	m_gridResult.SetFont(&m_fontDefault);
	m_gridResult.NoUseSorting(TRUE);
	
	m_ctrlTarget.Create(_T("지정시간:"),
		WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE,// | WS_BORDER,
		CRect(0, 0, 0, 0), this, IDC_STATIC_TARGET);
	m_ctrlTarget.SetFont(&m_fontDefault);
	
	m_ctrlTarget_Data.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP | DTS_SHORTDATEFORMAT,
		CRect(0, 0, 0, 0), this, IDC_DATA_TARGET);
	m_ctrlTarget_Data.SetFont(&m_fontDefault);

	m_ctrlTargetCombo_Time.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(0, 0, 0, 0), this, IDC_COMBO_TARGETTIME);
	m_ctrlTargetCombo_Time.SetFont(&m_fontDefault);
	m_ctrlTargetCombo_Time.ResetContent();
	
	m_btnResultSearch.Create(_T("검  색"), CRect(0, 0, 0, 0), this, IDC_BTN_SEARCH_RESULT);
	SetBtnImg(&m_btnResultSearch, 4, TRUE);
	//m_btnResultSearch.SetFont(&m_fontBold);

	m_ctrlCalculationTerm.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE,
		CRect(0, 0, 0, 0), this);
	m_ctrlCalculationTerm.SetFont(&m_fontDefault);
	
	m_ptabAnalystResult = new CDrTabCtrl();
	m_ptabAnalystResult->Create(0, this, IDC_TAB_ANANLUSTRESULT);
	m_ptabAnalystResult->SetBKColor(COLOR_MAIN_CANVAS);	
	
	TC_ITEM item;
	item.mask = TCIF_TEXT | TCIF_IMAGE;
	item.pszText = "수익률 차트";
	m_ptabAnalystResult->InsertItem(0, &item);
	
	item.pszText = "수익률 데이터";
	m_ptabAnalystResult->InsertItem(1, &item);
	m_ptabAnalystResult->SetCurSel(0);
	
	m_pwndAnalystResult = new CAnalystResultWnd;
	m_pwndAnalystResult->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 
		CRect(0, 0, 0, 0), this, IDC_WND_ANANLUSTRESULT);
	
	m_ctrlMsg.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE,
		CRect(0, 0, 0, 0), this);
	m_ctrlMsg.SetFont(&m_fontDefault);
	
	
	m_ctrlPass.Create(_T("진행상태"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
		CRect(0, 0, 0, 0), this);
	m_ctrlPass.SetFont(&m_fontDefault);
	
	m_progressPass.Create(WS_CHILD | WS_VISIBLE | PBS_SMOOTH, CRect(0, 0, 0, 0), 
		this, IDC_PROGRESS_PASS);
	m_progressPass.SetFont(&m_fontDefault);
	m_progressPass.SetRange(0, 100);
	
	m_ctrlPassResult.Create(_T("XXX%"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
		CRect(0, 0, 0, 0), this);
	m_ctrlPassResult.SetFont(&m_fontDefault);

	m_btnSend.Create(_T("종목전송"), CRect(0, 0, 0, 0), this, IDC_BTN_SEND);
	SetBtnImg(&m_btnSend, 4);
	m_btnSend.SetFont(&m_fontDefault);

	m_ctrlHelpText.Create(_T("※ 분석기간은 과거 6개월까지 가능합니다."), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
		CRect(0, 0, 0, 0), this);
	m_ctrlHelpText.SetFont(&m_fontDefault);
	m_ctrlHelpText.ShowWindow(SW_HIDE);

	m_ptabAnalystResult->SetWindowPos(&m_ctrlHelpText, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	AfxSetResourceHandle(hInstSave);

	return 0;
}

void CAnalystMainFrmWnd::SetInit(CString strLinkData)
{
 	m_treeCode.SetInit();
 	
	m_gridCode.SetInit("CODE");
	m_gridResult.SetInit("RESULT");
	
	const LPCTSTR BACKTEST_TIME[] = { "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00"};
	int nIndex = 0;
	for (nIndex = 0; nIndex < _countof(BACKTEST_TIME); nIndex++)
		m_ctrlTargetCombo_Time.AddString(BACKTEST_TIME[nIndex]);
	m_ctrlTargetCombo_Time.SetCurSel(0);

	//플랫폼
	for (nIndex = 0; nIndex < _countof(BACKTEST_TIME); nIndex++)
		OnPF_Combo_Event("btnComboTime", PF_EVENT_COMBO_ADDSTRING, BACKTEST_TIME[nIndex]);
	OnPF_Combo_Event("btnComboTime", PF_EVENT_COMBO_SETCURSEL, BACKTEST_TIME[nIndex]);
	
	
	m_pwndAnalystResult->m_btnTarget_Day.SetCheck(TRUE);
	m_pwndAnalystResult->m_btnTarget_Min.SetCheck(FALSE);
	m_ctrlTargetCombo_Time.EnableWindow(FALSE);
	
	m_nAnalystData_TargetType = 0;
	m_gridCode.ClearAll();
	m_gridCode.SetMultiSelectMode(TRUE);
	
	CTime tmItem = CTime::GetCurrentTime();
	/*CTime tmItem;*/
	//m_ctrlTarget_Data.GetTime(tmItem);
	tmItem -= CTimeSpan(30, 0, 0, 0 ); 
	m_ctrlTarget_Data.SetTime(&tmItem);

	//플랫폼
	CString strData = tmItem.Format("%Y%m%d");
	OnPF_Calendar_Event("btnCalendar", PF_EVENT_CALENDAR_SETTIME, strData);
	
	m_nKospiCnt = 0;
	m_nKosdaqCnt = 0;
	SetCodeCnt();
	
	m_bInit = TRUE;
	OnSize();
	
	if(strLinkData == "0")
		m_bLinkData = FALSE;
	else
	{
		m_bLinkData = TRUE;
		int nItemCnt = strLinkData.GetLength() / 7; // Code 번호 + 구분자(|) = 7;
		for(int i = 0; i < nItemCnt; i++)
		{
			CString strItemCode;
			strItemCode = strLinkData.Mid(i * 7, 7);
			strItemCode.Remove('|');
			m_saConditionSearchResultItemCode.Add(strItemCode);
		}
	}
	
	CArray<HTREEITEM, HTREEITEM> saFindItemArray;
	if (m_bRecMST && m_treeCode.GetFindTreeItem(NULL, "검색종목", saFindItemArray) > 0)
		m_treeCode.SetSelectItem(saFindItemArray.GetAt(0));
	
	saFindItemArray.RemoveAll();
	

	OnPF_Contorl_Move();
}

void CAnalystMainFrmWnd::OnDestroy() 
{
	//	CFrameWnd::OnDestroy();

	m_siUserCodeRow.RemoveAll();
	
	if (m_ptabAnalystResult->GetSafeHwnd() != NULL)
	{
		m_ptabAnalystResult->DestroyWindow();
		delete m_ptabAnalystResult;
		m_ptabAnalystResult = NULL;
	} 

	if (m_pwndAnalystResult->GetSafeHwnd() != NULL)
	{
		m_pwndAnalystResult->DestroyWindow();
		delete m_pwndAnalystResult;
		m_pwndAnalystResult = NULL;
	} 

	CFrameWnd::OnDestroy();
}

void CAnalystMainFrmWnd::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	if (m_bInit == TRUE)
		OnSize();
}

void CAnalystMainFrmWnd::OnSize()
{
	CRect reClient, reTemp, reBottom;
	GetClientRect(reClient);

	reClient.top += 3;
	reClient.left += 3;
	reClient.right -= 3;
	reClient.bottom -= 3;

	reTemp = reClient;
	reTemp.top = reTemp.bottom - 22;
	reTemp.bottom -= 2;
	
	reBottom = reTemp;
	reBottom.top -= 2;
	reBottom.bottom += 2;
	m_stcStatusBar.MoveWindow(reBottom);

	reTemp.right -= 5;
	reTemp.left = reTemp.right - 30;
	m_ctrlPassResult.MoveWindow(reTemp);
	
	reTemp.right = reTemp.left - 5;
	reTemp.left = reTemp.right - 150;

	CRect reProgress;
	reProgress = reTemp;
	reProgress.top += 2;
	reProgress.bottom -= 2;
	m_progressPass.MoveWindow(reProgress);
	
	reTemp.right = reTemp.left - 5;
	reTemp.left = reTemp.right - 50;
	m_ctrlPass.MoveWindow(reTemp);
	
	reTemp.right = reTemp.left - 5;
	reTemp.left = reClient.left + 5;
	m_ctrlMsg.MoveWindow(reTemp);

	reClient.bottom -= 26;
	
	if (m_bExpFlag == TRUE)
		OnSize_Right(reClient);
	else
	{
		int nLeft_Width = 280; 
		reTemp = reClient;
		reTemp.left = nLeft_Width + 3;
		OnSize_Right(reTemp);
				
		reTemp = reClient;
		reTemp.right = nLeft_Width;
		OnSize_Left(reTemp);
	}
	
	m_treeCode.ShowWindow((m_bExpFlag == TRUE) ? SW_HIDE : SW_SHOW);
	m_gridCode.ShowWindow((m_bExpFlag == TRUE) ? SW_HIDE : SW_SHOW);
	m_btnSelectOk.ShowWindow((m_bExpFlag == TRUE) ? SW_HIDE : SW_SHOW);
	m_btnSelectCancel.ShowWindow((m_bExpFlag == TRUE) ? SW_HIDE : SW_SHOW);
	m_btnListSearch.ShowWindow((m_bExpFlag == TRUE) ? SW_HIDE : SW_SHOW);

	OnPF_Contorl_Move();
}

void CAnalystMainFrmWnd::OnSize_Left(CRect reClient)
{
	CRect reTop;
	
	reClient.bottom -= 2;
	reTop = reClient;
	
	reTop.bottom = 150;
	m_treeCode.MoveWindow(&reTop);
	
	reTop.top = reTop.bottom + 3;
	reTop.bottom = reClient.bottom;
	m_gridCode.MoveWindow(&reTop);
}

void CAnalystMainFrmWnd::OnSize_Right(CRect reClient)
{
	ST_MOVEITEM mItem;
	CRect reTop, reTemp;
	
	reClient.bottom -= 2;
	
	reTop = reClient;
	
	reTop.bottom = reTop.top + 20;
	reTemp = reTop;
	
	if (m_bExpFlag == TRUE)
		reTemp.left += 3;
	
	reTemp.right = reTemp.left + 20;
	m_btnExp.MoveWindow(&reTemp);
	
	int nPos_Title;
	nPos_Title = reTemp.right + 2;
		
	reTemp.right = reTop.right;
	reTemp.left = reTemp.right - 84;//GetBtnWidth(3 + 3);

	if(reTemp.left < 500)
		reTemp.left = 500;

	m_btnList_Kosdaq.MoveWindow(&reTemp);

	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 84;//GetBtnWidth(3 + 3);
	m_btnList_Kospi.MoveWindow(&reTemp);	

	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 84;//GetBtnWidth(2 + 3);
	m_btnList_All.MoveWindow(&reTemp);

	reTemp.left = nPos_Title;
	reTemp.right = reTemp.left + 200;
	m_ctrlTitle.MoveWindow(&reTemp);
	
	reTop.top = reTop.bottom + 3;
	reTop.bottom = reTop.top + 40;
	m_gridResult.m_GI->m_vScrollWidth = 0;
	m_gridResult.MoveWindow(&reTop);
	
	reTop.top = reTop.bottom + 3;
	reTop.bottom = reTop.top + 20;
	reTemp = reTop;
	
	reTemp.top -= 1;
	reTemp.bottom -= 1;

	//reTemp.left += 3;
	reTemp.right = reTemp.left + 60;
	m_btnResultSearch.MoveWindow(&reTemp);

	reTemp.left = reTemp.right + 3;
	reTemp.right = reTemp.left + 53;
	m_ctrlTarget.MoveWindow(&reTemp);
	
	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 85;
	m_ctrlTarget_Data.MoveWindow(&reTemp);
	
	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 55;
	reTemp.bottom = reTemp.top + 150;
	m_ctrlTargetCombo_Time.MoveWindow(&reTemp);

	reTemp.bottom = reTemp.top + 20;

	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 60;
	m_btnSend.MoveWindow(&reTemp);
	
	reTop.top = reTop.bottom + 1;
	reTop.bottom = reClient.bottom;
	if (m_ptabAnalystResult != NULL)
	{
		m_rtTab = reTop;
		m_rtTab.top += 20;

		CRect reTab = reTop;
		reTab.top += 3;
		reTab.bottom = reTab.top + 22;
		m_ptabAnalystResult->MoveWindow(&reTab);
		
		reTop.top += 29;
 		reTop.bottom -= 3;
 		reTop.left += 2;
 		reTop.right -= 1;
		m_pwndAnalystResult->MoveWindow(&reTop);
	}

	CRect reBottom = reTop;
	reBottom.top -= 26;
	reBottom.bottom = reBottom.top + 20;
	reBottom.left += 217;
	reBottom.right = reClient.right;
	m_ctrlCalculationTerm.MoveWindow(&reBottom);

	CRect reHelp = reBottom;
	reHelp.left = reHelp.right  - 8;
	reHelp.right = reHelp.left + 260;
	m_ctrlHelpText.MoveWindow(&reHelp);
}

void CAnalystMainFrmWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect reClient;
	GetClientRect(reClient);

	dc.FillSolidRect(reClient, m_clrMainSkin);
	
	dc.FillSolidRect(m_rtTab, GetAxColor(66));
	
	HPEN hOldPen;
	hOldPen = (HPEN)dc.SelectObject(GetAxPen(GetAxColor(13), 1, PS_SOLID));
	
	MoveToEx(dc.m_hDC, m_rtTab.right - 1, m_rtTab.top + 2, NULL);		// 오른쪽 테두리
	LineTo(dc.m_hDC, m_rtTab.right - 1,  m_rtTab.bottom - 1);
	
	MoveToEx(dc.m_hDC, m_rtTab.right - 1, m_rtTab.bottom - 1, NULL);	// 아래 테두리
	LineTo(dc.m_hDC, m_rtTab.left - 1, m_rtTab.bottom - 1);
	
	MoveToEx(dc.m_hDC, m_rtTab.left, m_rtTab.bottom - 1, NULL);			// 왼쪽 테두리
	LineTo(dc.m_hDC, m_rtTab.left, m_rtTab.top + 2);
	dc.SelectObject(hOldPen);
}

HBRUSH CAnalystMainFrmWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(m_clrMainSkin);
		hbr = m_brushStatic;
	}
	
	return hbr;
}

void CAnalystMainFrmWnd::OnBtnSelectOk()
{
	//	for (int nRow = 0; nRow < m_pctrlGrid_Code->GetCount(); nRow++)
	//		m_pctrlGrid_Code->SetCellCheck(nRow, 0, TRUE);
	
	//	m_pctrlGrid_Code->SetScroll();
}

void CAnalystMainFrmWnd::OnBtnSelectCancel()
{
	//	for (int nRow = 0; nRow < m_pctrlGrid_Code->GetCount(); nRow++)
	//		m_pctrlGrid_Code->SetCellCheck(nRow, 0, FALSE);
	
	//	m_pctrlGrid_Code->SetScroll();
}

void CAnalystMainFrmWnd::SetSendTr_Code()
{
	CString strMsg;
	SetWaitCursor(TRUE);
	
	SetPassResult(0);
	
	// 2011.11.10 박성경 >>
// 	m_btnList_All.m_bToggle = FALSE;
// 	m_btnList_Kospi.m_bToggle = FALSE;
// 	m_btnList_Kosdaq.m_bToggle = FALSE;
	// 2011.11.10 박성경 <<
	
	if (m_nAnalystData_TargetType == 0)
	{
		//m_btnList_All.m_bToggle = TRUE;
		OnPF_Btn_Event("btnTotal", PF_EVENT_BTN_SETTEXTCOLOR, "", (long)RGB(255, 0, 0));
		OnPF_Btn_Event("btnKospi", PF_EVENT_BTN_SETTEXTCOLOR, "", (long)RGB(0, 0, 0));
		OnPF_Btn_Event("btnKosdaq", PF_EVENT_BTN_SETTEXTCOLOR, "", (long)RGB(0, 0, 0));
	}
	else if (m_nAnalystData_TargetType == 1)
	{
		//m_btnList_Kospi.m_bToggle = TRUE;
		OnPF_Btn_Event("btnTotal", PF_EVENT_BTN_SETTEXTCOLOR, "", (long)RGB(0, 0, 0));
		OnPF_Btn_Event("btnKospi", PF_EVENT_BTN_SETTEXTCOLOR, "", (long)RGB(255, 0, 0));
		OnPF_Btn_Event("btnKosdaq", PF_EVENT_BTN_SETTEXTCOLOR, "", (long)RGB(0, 0, 0));
	}
	else if (m_nAnalystData_TargetType == 2)
	{
		//m_btnList_Kosdaq.m_bToggle = TRUE;
		OnPF_Btn_Event("btnTotal", PF_EVENT_BTN_SETTEXTCOLOR, "", (long)RGB(0, 0, 0));
		OnPF_Btn_Event("btnKospi", PF_EVENT_BTN_SETTEXTCOLOR, "", (long)RGB(0, 0, 0));
		OnPF_Btn_Event("btnKosdaq", PF_EVENT_BTN_SETTEXTCOLOR, "", (long)RGB(255, 0, 0));
	}
	
	m_btnList_All.Invalidate();
	m_btnList_Kospi.Invalidate();
	m_btnList_Kosdaq.Invalidate();
	
	m_nTrIndex++;
	
	CString strData;
	if (m_pwndAnalystResult != NULL)
		m_pwndAnalystResult->SetInit_Val();
	
	GroupToBase_In		stGroupToBase_In;
	memset(&stGroupToBase_In, 0x00, SZ_GROUPTOBASE_IN);
	
	CString strTemp;
	strTemp.Format("%4d", m_nTrIndex);

	memcpy(stGroupToBase_In.sTrIndex, strTemp, 4);
	
	if (m_pwndAnalystResult->m_btnTarget_Day.GetCheck() == TRUE)
		memcpy(&stGroupToBase_In.sType, "1", sizeof(stGroupToBase_In.sType));
	else if (m_pwndAnalystResult->m_btnTarget_Min.GetCheck() == TRUE)
	{
		memcpy(&stGroupToBase_In.sType, "0", sizeof(stGroupToBase_In.sType));
		memcpy(&stGroupToBase_In.sTerm, "60", sizeof(stGroupToBase_In.sTerm));
		
		m_ctrlTargetCombo_Time.GetWindowText(strData);
		strData.Replace(":", "");
		strData += "00";
		memcpy(&stGroupToBase_In.sTime, strData.GetBuffer(0), sizeof(stGroupToBase_In.sTime));
	}

	CTime tmItem;
	m_ctrlTarget_Data.GetTime(tmItem);
	
	CTime tmCur = CTime::GetCurrentTime();
	if(tmItem > tmCur)
	{
		tmCur -= CTimeSpan(30, 0, 0, 0);
		strData = tmCur.Format("%Y%m%d");
		OnPF_Calendar_Event("btnCalendar", PF_EVENT_CALENDAR_SETTIME, strData);
	}
	else
	{
		tmCur -= CTimeSpan(180, 0, 0, 0);
		if(tmCur > tmItem)
			strData = tmCur.Format("%Y%m%d");
		else
			strData = tmItem.Format("%Y%m%d");
	}
		

	memcpy(&stGroupToBase_In.sDate, strData, sizeof(stGroupToBase_In.sDate));
	
	int nCodeCnt = m_gridCode.GetNumberRows();
	memset(stGroupToBase_In.sCodeList, 0x00, sizeof(stGroupToBase_In.sCodeList));
	
	BOOL bSendTr = TRUE;
	
	BOOL bAddState;
	int nAddCnt = 0;
	CodeInfo stCodeInfo;
	
	m_siUserCodeRow.RemoveAll();
	for (int nRow = 0; nRow < nCodeCnt; nRow++)
	{
		if (m_gridCode.GetRowToCodeInfo(nRow, stCodeInfo) == TRUE)
		{
			if (stCodeInfo.bUser == TRUE)
			{
				bAddState = FALSE;
				if (m_nAnalystData_TargetType == 0)   //# All
					bAddState = TRUE;
				else //# Kospi, Kosdaq
				{
					if (stCodeInfo.nMarket == m_nAnalystData_TargetType)
						bAddState = TRUE;
				}
				
				if (bAddState == TRUE)
				{
					if ((int)CODE_MAX_CNT <= nAddCnt)
					{
						bSendTr = FALSE;
						break;
					}

					memcpy(&stGroupToBase_In.sCodeList[nAddCnt], stCodeInfo.strCode.GetBuffer(0), (int)LEN_CODE);
					
					m_siUserCodeRow.Add(nRow);
					nAddCnt++;
				}
			}
		}
	}
	
	if (bSendTr == FALSE)
	{
		strMsg.Format("분석할 종목제한 값(%d)을 초과 하였습니다.", (int)CODE_MAX_CNT);
		SetSendMainMsg(strMsg);

		// 20110222 박찬모 조건검색 성과분석, 50개 종목 제한 UI 추가 보강 >>
		strMsg = "최대 50종목까지 조회하실 수 있으며,\n현재가, 대비, 등락율 데이터는 이미 조회된\n종목 또는 현재 조회된 종목에만 보여집니다.";
		SetMessageBox(strMsg ,PROGRAM_TITLE, MB_YESNO | MB_ICONQUESTION);

		// 2012.2.14 박성경: 50개 이상검색후 그리드 클릭시 종목 차트 안그려지는 오류 수정
		if (m_pwndAnalystResult != NULL)
			m_pwndAnalystResult->SetInit_AnalystInfo();

		return;
	}
	
	sprintf(stGroupToBase_In.sCnt, "%d", nAddCnt);
	
	if (nAddCnt == 0)
	{
		m_pwndAnalystResult->m_gridProfit.ClearAll();
		m_pwndAnalystResult->m_chartProfit.SetClearAll();
		
		SetSendMainMsg("분석할 종목이 없습니다.");
		m_bRun = FALSE;
		SetWaitCursor(FALSE);
		return;
	}
	
	char* lpData = new char[SZ_GROUPTOBASE_IN];
	memcpy(lpData, &stGroupToBase_In, SZ_GROUPTOBASE_IN);
	
	int nState = Modual_SendTr((CString)TRNO_GROUPTOBASE, SZ_GROUPTOBASE_IN, (BYTE*)lpData);
	if (nState < 0)
		SetSendMainMsg("조회실패...");
	else
		SetSendMainMsg("조회중...");
	
	if (lpData)
		delete[] lpData;
	
#ifdef _DEBUG
	m_nTickCnt = GetTickCount();
#endif
	SetWaitCursor(FALSE);
}

int CAnalystMainFrmWnd::Modual_SendTr(CString szTrNo, int nTrLen, BYTE *TrData)
{	
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();

	CString strScreenCode = "";
	if (szTrNo.CompareNoCase(TRNO_GROUPTOBASE)==0)
	{
		pScreenWnd->SendTR(TRNO_GROUPTOBASE, (char*)TrData, nTrLen, TRKEY_GROUPTOBASE, US_KEY);
	}
	else if(szTrNo.CompareNoCase(TRNO_GROUPTOBASE_UP)==0)
	{
		pScreenWnd->SendTR(TRNO_GROUPTOBASE_UP, (char*)TrData, nTrLen, TRKEY_GROUPTOBASE_UP, US_KEY);
	}
	else if (szTrNo.CompareNoCase(TRNO_CONDITION)==0)
	{
		pScreenWnd->SendTR(TRNO_CONDITION, (char*)TrData, nTrLen, TRKEY_CONDITION, US_KEY);
	}
	else if(szTrNo.CompareNoCase(TRNO_MSTCODE)==0)
	{
		pScreenWnd->SendTR(TRNO_MSTCODE, (char*)TrData, nTrLen, TRKEY_MSTCODE, US_KEY);
	}

	return 1;
}

void CAnalystMainFrmWnd::OnBtnSearch_Code()
{
	SetSendTr_Code();
}

long CAnalystMainFrmWnd::OnGetData(WPARAM wParam, LPARAM lParam)
{
	int nResult = -1;
/* -hidden-
	CRpDataParser rpParser;
	rpParser.OnGetData (wParam, lParam);
	
	if (!rpParser.IsError())
	{
		CString strTrCode;
		strTrCode = rpParser.GetTranCode();
		strTrCode.TrimRight(" ");
		
		nResult = ReceiveData(strTrCode, rpParser.GetErrCode(), rpParser.GetErrMsg(), 
			rpParser.GetData(), rpParser.GetDataSize());
	}

	CString strMsgCode = rpParser.GetReciveCode();
	CString strMsg = CommGetMsgString(strMsgCode);
	SetSendMainMsg(strMsg);
*/	
	return nResult;
}

HRESULT CAnalystMainFrmWnd::ReceiveData(LPCSTR szTR, LPVOID aTRData, long dwTRDateLen , int nKey)
{
	char *pchData;
	CString strData;
	
	CString strMsg;
	if (dwTRDateLen == 0)		//# 0 : err
	{
		SetWaitCursor(FALSE);
		// 2011.11.10 박성경
		//SetSendMainMsg(szMessage);
	}
	else if (dwTRDateLen > 0)
	{
		SetSendMainMsg("조회 완료...");
		
		if ((CString)szTR == TRNO_GROUPTOBASE)
		{
			char* pReceiveBuff = new char[dwTRDateLen + 1];
			memset(pReceiveBuff, 0x00,		dwTRDateLen + 1);
			memcpy(pReceiveBuff, aTRData,	dwTRDateLen);

			CommonGroupToBase_Out stCommonGroupToBase_Out;
			memset(&stCommonGroupToBase_Out, 0x00, (int)SZ_COMMON_GROUPTOBASE_OUT);	
			memcpy(&stCommonGroupToBase_Out, pReceiveBuff, (int)SZ_COMMON_GROUPTOBASE_OUT);

			CString strTemp;
			strTemp.Format("%4.4s", stCommonGroupToBase_Out.sTrIndex);
			if (atoi(strTemp) != m_nTrIndex)
				return FALSE;
			
			STR_CONVERT(pchData, stCommonGroupToBase_Out.sCnt, sizeof(stCommonGroupToBase_Out.sCnt), strData);
			int nCodeCnt = atoi(strData);
			
			if (nCodeCnt <= 0)
			{
				m_bRun = FALSE;
				
				SetSendMainMsg("분석할 종목이 없습니다.");
				SetWaitCursor(FALSE);
				
				return FALSE;
			}
			
			int nPos = 0;
			GroupToBase_Out stGroupToBase_Out;
			memset(&stGroupToBase_Out, 0x00, (int)SZ_GROUPTOBASE_OUT);
			memcpy(&stGroupToBase_Out, &stCommonGroupToBase_Out, (int)SZ_COMMON_GROUPTOBASE_OUT);
			nPos += (int)SZ_COMMON_GROUPTOBASE_OUT;

			memcpy(stGroupToBase_Out.stPrice, pReceiveBuff + nPos, /*1620*/900/*3600*/ * nCodeCnt);
			nPos += /*1620*/900/*3600*/ * nCodeCnt;

			memcpy(stGroupToBase_Out.stBase, pReceiveBuff + nPos, SZ_BASEDATA_CODE * nCodeCnt);

			m_pwndAnalystResult->SetData(&stGroupToBase_Out);
			int nRow;
			BaseData_Code stBaseData_Code;
			for (int nIndex = 0; nIndex < nCodeCnt; nIndex++)
			{
				memset(&stBaseData_Code, 0x00, (int)SZ_BASEDATA_CODE);
				memcpy(&stBaseData_Code, &stGroupToBase_Out.stBase[nIndex], (int)SZ_BASEDATA_CODE);
				
				nRow = m_siUserCodeRow.GetAt(nIndex);
				m_gridCode.SetModifyRow(nRow, stBaseData_Code);
			}
			m_gridCode.RedrawAll();

			SetCodeCnt();
			SetSendTr_UpJong();

			delete pReceiveBuff;
		}
		else if ((CString)szTR == TRNO_GROUPTOBASE_UP)
		{
			char* pReceiveBuff = new char[dwTRDateLen + 1];
			memset(pReceiveBuff, 0x00,		dwTRDateLen + 1);
			memcpy(pReceiveBuff, aTRData,	dwTRDateLen);

			CommonGroupToBase_Out stCommonGroupToBase_Out;
			memset(&stCommonGroupToBase_Out, 0x00, (int)SZ_COMMON_GROUPTOBASE_OUT);	
			memcpy(&stCommonGroupToBase_Out, pReceiveBuff, (int)SZ_COMMON_GROUPTOBASE_OUT);

			CString strTemp;
			strTemp.Format("%4.4s", stCommonGroupToBase_Out.sTrIndex);
			if (atoi(strTemp) != m_nTrIndex)
				return FALSE;

			STR_CONVERT(pchData, stCommonGroupToBase_Out.sCnt, sizeof(stCommonGroupToBase_Out.sCnt), strData);
			int nCodeCnt = atoi(strData);
			if (nCodeCnt <= 0)
			{
				m_bRun = FALSE;
				SetWaitCursor(FALSE);
				delete pReceiveBuff;				
				return FALSE;
			}

			int nPos = 0;
			GroupToBase_UPOut stGroupToBase_UPOut;
			memset(&stGroupToBase_UPOut, 0x00, SZ_GROUPTOBASE_UPOUT);
			memcpy(&stGroupToBase_UPOut, &stCommonGroupToBase_Out, (int)SZ_COMMON_GROUPTOBASE_OUT);
			nPos += (int)SZ_COMMON_GROUPTOBASE_OUT;

			memcpy(stGroupToBase_UPOut.stPrice, pReceiveBuff + nPos, sizeof(PriceData) * RESULT_MAX_CNT2 * nCodeCnt);
			nPos += sizeof(PriceData) * RESULT_MAX_CNT2 * nCodeCnt;
			
			memcpy(stGroupToBase_UPOut.stBase, pReceiveBuff + nPos, SZ_BASEDATA_UPCODE * nCodeCnt);

			m_pwndAnalystResult->SetData(&stGroupToBase_UPOut);
			m_pwndAnalystResult->SetDataCalculation(&m_siUserCodeRow);
			
			m_bRun = FALSE;
			delete pReceiveBuff;

			m_gridCode.ReSelectRow();
		}
		else if ((CString)szTR == TRNO_CONDITION || szTR == (CString)TRNO_MSTCODE)
		{
			m_treeCode.ReceiveData(szTR, aTRData, dwTRDateLen);
			m_bRecMST = TRUE;
			
			if(m_bLinkData)
			{
				CArray<HTREEITEM, HTREEITEM> saFindItemArray;
				if (m_treeCode.GetFindTreeItem(NULL, "조건검색결과", saFindItemArray) > 0)
					m_treeCode.SetSelectItem(saFindItemArray.GetAt(0));
			}
		}else if ( (CString)szTR == TRNO_FAVORITE ) //tour2k 관심종목 그룹
		{
			char* pReceiveBuff = new char[dwTRDateLen + 1];
			memset(pReceiveBuff, 0x00, dwTRDateLen + 1);
			memcpy(pReceiveBuff, aTRData, dwTRDateLen);
			
			if (nKey == TRKEY_GWANSIMGROUP )//관심종목
			{
				CStringArray arrName, arrKey;
				ParseGroup(pReceiveBuff, arrKey, arrName);

				m_treeCode.SetInitTree_PortData(NULL, m_treeCode.GetPortIndex(), arrName);
			}
			else if ( nKey == TRKEY_GWANSIMITEM)//종목 리스트 
			{
				CStringArray saCode;
				ParseList(pReceiveBuff, saCode);

				if (saCode.GetSize() > 0)
					SendMessage(WM_NOTIFY_TREE, (WPARAM)NM_TREE_CLK, (LPARAM)&saCode);

			}

			delete[] pReceiveBuff; pReceiveBuff = nullptr;
		}
	
		SetWaitCursor(FALSE);
	}
	OnPF_Contorl_Move();
	return S_OK;
}

int CAnalystMainFrmWnd::ParseGroup(char* pReceiveBuff,CStringArray& arrKey, CStringArray& arrName)
{
	int cnt = atoi(CString(pReceiveBuff, 4));
	int parseL = 4;

	struct _grSearch
	{
		char ngrs[2];
		char grseq[2];
		char gname[30];
	};

	struct _grSearch* stgr;
	char* pSeq, * pgame;
	pSeq = new char[sizeof(stgr->grseq) + 1];
	pgame = new char[sizeof(stgr->gname) + 1];

	m_GwanSimGroup.strKey.RemoveAll();
	m_GwanSimGroup.strGroupName.RemoveAll();

	for (int ii = 0; ii < cnt; ii++)
	{
		stgr = (struct _grSearch*)&pReceiveBuff[parseL];
		memset(pSeq, 0x00, sizeof(stgr->grseq) + 1);
		memcpy(pSeq, stgr->grseq, sizeof(stgr->grseq));
		m_GwanSimGroup.strKey.Add(pSeq);
		arrKey.Add(pSeq);

		memset(pgame, 0x00, sizeof(stgr->gname) + 1);
		memcpy(pgame, stgr->gname, sizeof(stgr->gname));

		m_GwanSimGroup.strGroupName.Add(pgame);
		arrName.Add(pgame);

		parseL += sizeof(struct _grSearch);

	}

	delete[] pSeq; pSeq = nullptr;
	delete[] pgame; pgame = nullptr;

	return 0;
}

int CAnalystMainFrmWnd::ParseList(char* pReceiveBuff, CStringArray& arrName)
{
	CString strseq = CString(pReceiveBuff, 2);
	CString strgname = CString(&pReceiveBuff[2], 20);
	int cnt = atoi(CString(&pReceiveBuff[22], 4));
	int parseL = 26;

	struct _jinfo {
		char gubn[1]; // 종목구분 0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
		char code[12]; // 종목코드
		char xprc[10]; // 보유단가
		char xnum[10]; // 보유수량
	};

	struct _jinfo* jinfo;
	char* pgubn, * pCode, * pxprc, * pxnum;
	pgubn = new char[sizeof(jinfo->gubn) + 1];
	pCode = new char[sizeof(jinfo->code) + 1];

	for (int ii = 0; ii < cnt; ii++)
	{
		jinfo = (struct _jinfo*)&pReceiveBuff[parseL];
		memset(pgubn, 0x00, sizeof(jinfo->gubn) + 1);
		memcpy(pgubn, jinfo->gubn, sizeof(jinfo->gubn));  // 종목구분 0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
		memset(pCode, 0x00, sizeof(jinfo->code) + 1);
		memcpy(pCode, jinfo->code, sizeof(jinfo->code));  // 종목코드[12]

		arrName.Add(CString(pCode).Trim());

		parseL += sizeof(struct _jinfo);
	}

	delete[] pgubn; pgubn = nullptr;
	delete[] pCode; pCode = nullptr;

	return 0;
}

void CAnalystMainFrmWnd::OnBtnExp()
{
	m_bExpFlag = !m_bExpFlag;
	
	if (m_bExpFlag == TRUE)
		SetBtnImg(&m_btnExp, "RIGHT_EX");
	else
		SetBtnImg(&m_btnExp, "LEFT_EX");
	
	OnSize();

// 	int nX;
// 	int nY;
// 	CRect rt;
// 	
// 	m_btnExp.GetWindowRect(rt);
// 	nX = rt.left + rt.Width()/2;
// 	nY = rt.top + rt.Height()/2;
// 	
// 	SetCursorPos(nX, nY);

	m_ctrlCalculationTerm.Invalidate();
	m_btnSend.Invalidate();

	Invalidate();
}

void CAnalystMainFrmWnd::OnBtnListAll()
{
	m_nAnalystData_TargetType = 0;
	SetSendTr_Code();
}

void CAnalystMainFrmWnd::OnBtnKospi()
{
	m_nAnalystData_TargetType = 1;
	SetSendTr_Code();
}

void CAnalystMainFrmWnd::OnBtnKosdaq()
{
	m_nAnalystData_TargetType = 2;
	SetSendTr_Code();
}

void CAnalystMainFrmWnd::SetSendTr_UpJong()
{
	SetPassResult(0);
	
	CString strData;
	
	GroupToBase_UPIn		stGroupToBase_UPIn;
	memset(&stGroupToBase_UPIn, 0x00, SZ_GROUPTOBASE_UPIN);
	
	strData.Format("%4d", m_nTrIndex);

	memcpy(stGroupToBase_UPIn.sTrIndex, strData, strData.GetLength());
	
	if (m_pwndAnalystResult->m_btnTarget_Day.GetCheck() == TRUE)
	{
		memcpy(&stGroupToBase_UPIn.sType, "1", sizeof(stGroupToBase_UPIn.sType));
		memcpy(&stGroupToBase_UPIn.sTerm, "00", sizeof(stGroupToBase_UPIn.sTerm));
	}
	else if (m_pwndAnalystResult->m_btnTarget_Min.GetCheck() == TRUE)
	{
		memcpy(&stGroupToBase_UPIn.sType, "0", sizeof(stGroupToBase_UPIn.sType));
		memcpy(&stGroupToBase_UPIn.sTerm, "60", sizeof(stGroupToBase_UPIn.sTerm));
		
		m_ctrlTargetCombo_Time.GetWindowText(strData);
		strData.Replace(":", "");
		strData += "00";
		memcpy(&stGroupToBase_UPIn.sTime, strData.GetBuffer(0), sizeof(stGroupToBase_UPIn.sTime));
	}
	
// 	CTime tmItem;
// 	m_ctrlTarget_Data.GetTime(tmItem);
// 	strData = tmItem.Format("%Y%m%d");

	CTime tmItem;
	m_ctrlTarget_Data.GetTime(tmItem);

	CTime tmCur = CTime::GetCurrentTime();
	if(tmItem > tmCur)
	{
		tmCur -= CTimeSpan(30, 0, 0, 0);
		strData = tmCur.Format("%Y%m%d");
		OnPF_Calendar_Event("btnCalendar", PF_EVENT_CALENDAR_SETTIME, strData);
	}
	else
	{
		tmCur -= CTimeSpan(180, 0, 0, 0);
		if(tmCur > tmItem)
			strData = tmCur.Format("%Y%m%d");
		else
			strData = tmItem.Format("%Y%m%d");
	}

	memcpy(&stGroupToBase_UPIn.sDate, strData.GetBuffer(0), sizeof(stGroupToBase_UPIn.sDate));
	
	memcpy(&stGroupToBase_UPIn.sCnt, "2", sizeof(stGroupToBase_UPIn.sCnt));
	memcpy(&stGroupToBase_UPIn.sCodeList[0], "001", (int)LEN_UPCODE);
	memcpy(&stGroupToBase_UPIn.sCodeList[1], "201", (int)LEN_UPCODE);
	
	char* lpData = new char[SZ_GROUPTOBASE_UPIN];
	memcpy(lpData, &stGroupToBase_UPIn, SZ_GROUPTOBASE_UPIN);
	
	int nState = Modual_SendTr((CString)TRNO_GROUPTOBASE_UP, SZ_GROUPTOBASE_UPIN, (BYTE*)lpData);
	if (nState < 0)
		SetSendMainMsg("조회실패...");
	else
		SetSendMainMsg("조회중...");
	
	if (lpData)
		delete[] lpData;
}

void CAnalystMainFrmWnd::OnBtnSearch_Result()
{	
	SetSendTr_Code();
}

void CAnalystMainFrmWnd::SetSendMainMsg(CString strMsg)
{
	m_ctrlMsg.SetWindowText(strMsg);
	
	
	//SetMessageBox(strMsg, (CString)PROGRAM_TITLE, MB_ICONINFORMATION | MB_OK);
	
	//  CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();
	//  pwndMainFrm->SetSendMainMsg(strMsg);
}

void CAnalystMainFrmWnd::SetSendTitleBar(CString strMsg)
{
	
}

void CAnalystMainFrmWnd::SetCodeCnt()
{
	m_gridCode.GetMarketTypeCnt(m_nKospiCnt, m_nKosdaqCnt);
	
	CString strDataCnt;
	strDataCnt.Format("전체:%4d", m_nKospiCnt + m_nKosdaqCnt);
	m_btnList_All.SetWindowText(strDataCnt);
	OnPF_Btn_Event("btnTotal", PF_EVENT_BTN_SETWINDOWTEXT, strDataCnt);
	
	strDataCnt.Format("거래소:%3d", m_nKospiCnt);
	m_btnList_Kospi.SetWindowText(strDataCnt);
	OnPF_Btn_Event("btnKospi", PF_EVENT_BTN_SETWINDOWTEXT, strDataCnt);
	
	strDataCnt.Format("코스닥:%3d", m_nKosdaqCnt);
	m_btnList_Kosdaq.SetWindowText(strDataCnt);
	OnPF_Btn_Event("btnKosdaq", PF_EVENT_BTN_SETWINDOWTEXT, strDataCnt);

	m_btnList_All.Invalidate();
	m_btnList_Kospi.Invalidate();
	m_btnList_Kosdaq.Invalidate();
}

void CAnalystMainFrmWnd::OnNotify_ChangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nCurSel = m_ptabAnalystResult->GetCurSel();
	m_pwndAnalystResult->SetSelectType(nCurSel);
}

LRESULT CAnalystMainFrmWnd::OnNotify_Grid(WPARAM wParam, LPARAM lParam)
{

	if (wParam == NM_GRID_CLK)
	{
		CodeInfo stCodeInfo = *(CodeInfo*)lParam;
		m_pwndAnalystResult->SetDrewIndexToLine(stCodeInfo);

 		PutLinkData("&STOCK_CODE", stCodeInfo.strCode);
		m_gridCode.SetFocus();
	}
	else if (wParam == NM_GRID_DBCLK)
	{
		CodeInfo stCodeInfo = *(CodeInfo*)lParam;
		
// -hidden-
//		CTrMainView *pviewTrMain = (CTrMainView*)GetParent();
//		pviewTrMain->SetLinkCode(stCodeInfo.strCode);

		// 2011.11.10 박성경: 더블클릭으로 화면오픈>>
// 		CPortAnalystView* pView = (CPortAnalystView*)GetParent();
// 		pView->OnDBClick(stCodeInfo.strCode);
		// 2011.11.10 박성경 <<
	}
	else if (wParam == NM_GRID_HEADERCLK)
		SetCodeCnt();
	else if (wParam == NM_GRID_CELLDATA)
	{
		AnalystData	stData = *(AnalystData*)lParam;
		if (stData.nType == 1)
		{
			if (stData.bUser == TRUE)
				m_nKospiCnt++;
			else
				m_nKospiCnt--;
		}
		else if (stData.nType == 2)
		{
			if (stData.bUser == TRUE)
				m_nKosdaqCnt++;
			else
				m_nKosdaqCnt--;
		}
		
		CString strDataCnt;
		strDataCnt.Format("전체:%4d", m_nKospiCnt + m_nKosdaqCnt);
		m_btnList_All.SetWindowText(strDataCnt);
		m_btnList_All.Invalidate();
		OnPF_Btn_Event("btnTotal", PF_EVENT_BTN_SETWINDOWTEXT, strDataCnt);
		
		strDataCnt.Format("거래소:%3d", m_nKospiCnt);
		m_btnList_Kospi.SetWindowText(strDataCnt);
		m_btnList_Kospi.Invalidate();
		OnPF_Btn_Event("btnKospi", PF_EVENT_BTN_SETWINDOWTEXT, strDataCnt);
		
		strDataCnt.Format("코스닥:%3d", m_nKosdaqCnt);
		m_btnList_Kosdaq.SetWindowText(strDataCnt);
		m_btnList_Kosdaq.Invalidate();
		OnPF_Btn_Event("btnKosdaq", PF_EVENT_BTN_SETWINDOWTEXT, strDataCnt);
	}
	else if(wParam == NM_GRID_RCLK)
	{
		RButtonInfo stRInfo = *(RButtonInfo*)lParam;
		CPoint point;

		point.x = stRInfo.xPoint;
		point.y = stRInfo.yPoint;

		OnRButtonClick(&point, stRInfo.strCode);
	}
	return TRUE;
}

//extern LPCTSTR dll_GetJongmokName(CString szType, LPCTSTR szCode, BOOL bFullName=TRUE);
//extern BYTE dll_GetJongMarket(LPCTSTR szCode);	-hidden-
LRESULT CAnalystMainFrmWnd::OnNotify_Tree(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NM_TREE_CLK)
	{
		CStringArray *psaCode = (CStringArray *)lParam;
		
		m_gridCode.ClearAll();
		if (psaCode->GetSize() == 0)
		{
			SetSendMainMsg("분석할 종목이 없습니다.");
			return FALSE;
		}
		
		CString strMsg;
		int nKospiCnt, nKosdaqCnt;
		nKospiCnt = 0;
		nKosdaqCnt = 0;
		m_gridCode.SetColResize(FALSE);
		int nCodeCnt = 0;
		
		SetWaitCursor(TRUE);
		SetSendMainMsg("분석 종목 구성중...");
		SetPassResult(0);
		
		BOOL bChk = TRUE;
		int nTotCnt, nRunVal;
		nRunVal = 0;
		nTotCnt = psaCode->GetSize();

		int nMarket;
		BaseData_Code  stBaseData;
		CString strCode, strName, strMarket;
		m_gridCode.SetNumberRows(nTotCnt);//psaCode->GetSize());
		for (int nIndex = 0; nIndex < nTotCnt/*psaCode->GetSize()*/; nIndex++)
		{
			bChk = (nCodeCnt < (int)CODE_MAX_CNT) ? TRUE : FALSE;
			memset(&stBaseData, 0x00, SZ_BASEDATA_CODE);

			strCode = psaCode->GetAt(nIndex);
			if(!GetItemNameAndMarket(strCode, strName, nMarket))
				continue;
			strMarket.Format("%d", nMarket);

			memcpy(&stBaseData.sMarket, strMarket.GetBuffer(0), strMarket.GetLength());
			memcpy(&stBaseData.sCode, strCode.GetBuffer(0), strCode.GetLength());
			memcpy(&stBaseData.sName, strName.GetBuffer(0), strName.GetLength());
			
			m_gridCode.SetAddRow(bChk, nCodeCnt/*nIndex*/, stBaseData);
			nCodeCnt++;

			nRunVal = ((nIndex + 1) * 100 / nTotCnt);
			SetPassResult(nRunVal);
		}
		strMsg.Format("분석 종목 %d 구성 완료.", nCodeCnt);
		SetSendMainMsg(strMsg);

		{
			CString szMsg;
			szMsg.Format("After Count = %d\n", nCodeCnt);
			OutputDebugString(szMsg);
		}
		
		m_gridCode.SetColResize(TRUE);
		m_gridCode.RedrawAll();
		SetWaitCursor(FALSE);
		
		OnBtnListAll();
	}
	else if (wParam == NM_TREE_ITEMTITLE)
	{
		CString strData = *(CString*)lParam;
		m_ctrlTitle.SetWindowText(strData);
	}
	
	return TRUE;
}

void CAnalystMainFrmWnd::GetKwansimGroupName(CStringArray& arrGroupName)
{
	m_GwanSimGroup.strGroupName.RemoveAll();
	m_GwanSimGroup.strKey.RemoveAll();

	GetData_PortGroup(m_GwanSimGroup.strKey, arrGroupName);

	for(int i=0; i < arrGroupName.GetSize(); i++)
		m_GwanSimGroup.strGroupName.Add(arrGroupName.GetAt(i));
}

void CAnalystMainFrmWnd::GetItemsWithGroupName(CString strIndex, CStringArray& arrItems)
{
	CStringArray arrGroupName;
	if(m_GwanSimGroup.strGroupName.GetSize() != NULL)
	{
		for(int i=0; i < m_GwanSimGroup.strGroupName.GetSize(); i++)
			if(strIndex.CompareNoCase(m_GwanSimGroup.strGroupName.GetAt(i)) == 0)
				GetData_PortItem(arrItems, arrGroupName, atoi(m_GwanSimGroup.strKey.GetAt(i)));
	}
}

BOOL CAnalystMainFrmWnd::GetItemNameAndMarket(CString& strCode, CString& strName, int& nMarket)
{
	return m_treeCode.GetItemNameAndMarket(strCode, strName, nMarket);
}

void CAnalystMainFrmWnd::SetPassResult(int nData)
{
	CString strData;
	
	m_progressPass.SetPos(nData);
	
	strData.Format("%d", nData);
	m_ctrlPassResult.SetWindowText(strData + "%");
}

BOOL CAnalystMainFrmWnd::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_RBUTTONUP == pMsg->message)
	{
		CRect rcGridCode;
		m_gridCode.GetWindowRect(rcGridCode);
		ScreenToClient(rcGridCode);

		if(!rcGridCode.PtInRect(pMsg->pt))
			::PostMessage(GetParent()->m_hWnd, WM_CONTEXTMENU, 0, 0);
	}
	
	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CAnalystMainFrmWnd::ChangeSkinColor(COLORREF clrBkgnd, ST_TABCOLOR stTabColor)
{
	if(!clrBkgnd)
		return;
	
	m_clrMainSkin = clrBkgnd;
	
	if(m_brushStatic.DeleteObject())
		m_brushStatic.CreateSolidBrush(m_clrMainSkin);
	
	m_ptabAnalystResult->SetTabColor(m_clrMainSkin, m_clrMainSkin, stTabColor.clrB, NULL, NULL, m_clrMainSkin);
	m_ptabAnalystResult->SetTextColor(stTabColor.clrSF, stTabColor.clrSF);
	
	if(m_pwndAnalystResult)
		m_pwndAnalystResult->ChangeSkinColor(m_clrMainSkin, stTabColor);

	ST_GRIDCOLOR stGridColor;
	stGridColor.clrGridHeader = GetAxColor(74);
	stGridColor.clrGridHilight = GetAxColor(78);

	ChangeGridColor(stGridColor);

	m_ctrlTitle.Invalidate();
	m_ctrlTarget.Invalidate();
	m_ctrlTargetCombo_Time.Invalidate();
	m_ctrlCalculationTerm.Invalidate();
	
	m_stcStatusBar.Invalidate();
	m_ctrlMsg.Invalidate();
	m_ctrlPass.Invalidate();
	m_progressPass.Invalidate();
	m_ctrlPassResult.Invalidate();

	Invalidate();	
}

void CAnalystMainFrmWnd::ChangeGridColor(ST_GRIDCOLOR stGridColor)
{
	//우측 상단 그리드
	m_gridCode.ChangeGridColor(stGridColor);
	m_gridCode.Invalidate();
	m_gridResult.ChangeGridColor(stGridColor);
	m_gridResult.Invalidate();

	if(m_pwndAnalystResult)
		m_pwndAnalystResult->ChangeGridColor(stGridColor);
}

void CAnalystMainFrmWnd::OnPF_Btn_Event(CString szCtrlName, int nEventID, CString strData, long nData)
{
	ST_PF_CONTROL_EVENT pfEvent;
	SET_PF_CONTROL_EVENT(pfEvent, szCtrlName, PF_ITEM_BTN, TRUE);
	pfEvent.nControlType = PF_ITEM_BTN;

	if(nEventID == PF_EVENT_BTN_SETWINDOWTEXT)
	{		
		pfEvent.nEventID = PF_EVENT_BTN_SETWINDOWTEXT;
		pfEvent.szData = strData;	
	}
	else if(nEventID == PF_EVENT_BTN_SETTEXTCOLOR)
	{
		pfEvent.nEventID = PF_EVENT_BTN_SETTEXTCOLOR;
		pfEvent.nData = nData;
	}

	SendMessage(MF_PFCONTORL_EVENT, 0, (LPARAM)&pfEvent);
}

void CAnalystMainFrmWnd::OnPF_Combo_Event(CString szCtrlName, int nEventID, CString strData, int nData)
{
	ST_PF_CONTROL_EVENT pfEvent;
	SET_PF_CONTROL_EVENT(pfEvent, szCtrlName, PF_ITEM_COMBOBOX, TRUE);
	pfEvent.nControlType = PF_ITEM_COMBOBOX;
	
	if(nEventID == PF_EVENT_COMBO_SETCURSEL)
	{		
		pfEvent.nEventID = PF_EVENT_COMBO_SETCURSEL;
		pfEvent.nData = nData;	
	}
	else if(nEventID == PF_EVENT_COMBO_RESETCONTENTS)
	{
		pfEvent.nEventID = PF_EVENT_COMBO_RESETCONTENTS;
	}
	else if(nEventID == PF_EVENT_COMBO_ADDSTRING)
	{
		pfEvent.nEventID = PF_EVENT_COMBO_ADDSTRING;
		pfEvent.szData = strData;
	}
	SendMessage(MF_PFCONTORL_EVENT, 0, (LPARAM)&pfEvent);
}

void CAnalystMainFrmWnd::OnPF_Check_Event(CString szCtrlName, int nEventID, int nData,  CString strData)
{
	ST_PF_CONTROL_EVENT pfEvent;
	SET_PF_CONTROL_EVENT(pfEvent, szCtrlName, PF_ITEM_CHECKBTN, TRUE);
	pfEvent.nControlType = PF_ITEM_CHECKBTN;
	
	if(nEventID == PF_EVENT_CHK_SETCHECK)
	{		
		pfEvent.nEventID = PF_EVENT_CHK_SETCHECK;
		pfEvent.bState = nData;	
	}
	SendMessage(MF_PFCONTORL_EVENT, 0, (LPARAM)&pfEvent);
}

void CAnalystMainFrmWnd::OnPF_Calendar_Event(CString szCtrlName, int nEventID, CString strData, long nData)
{
	ST_PF_CONTROL_EVENT pfEvent;
	SET_PF_CONTROL_EVENT(pfEvent, szCtrlName, PF_ITEM_CALENDAR, TRUE);
	pfEvent.nControlType = PF_ITEM_CALENDAR;
	
	if(nEventID == PF_EVENT_CALENDAR_SETTIME)
	{		
		pfEvent.nEventID = PF_EVENT_CALENDAR_SETTIME;
		pfEvent.szData = strData;	
	}
	SendMessage(MF_PFCONTORL_EVENT, 0, (LPARAM)&pfEvent);
}

void CAnalystMainFrmWnd::OnPF_Contorl_Move()
{
	return;

	ST_MOVEITEM stItem;	
	stItem.nType = PF_ITEM_BTN;
	
	SET_ST_MOVEITEM(stItem, "btnKospi", &m_btnList_Kosdaq);	
	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);
	
	SET_ST_MOVEITEM(stItem, "btnKosdaq", &m_btnList_Kospi);
	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);

	SET_ST_MOVEITEM(stItem, "btnTotal", &m_btnList_All);
	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);

	SET_ST_MOVEITEM(stItem, "btnSearch", &m_btnResultSearch);
	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);	

	SET_ST_MOVEITEM(stItem, "btnCalendar", &m_ctrlTarget_Data);
	stItem.nType = PF_ITEM_CALENDAR;
 	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);	

	SET_ST_MOVEITEM(stItem, "btnComboTime", &m_ctrlTargetCombo_Time);
	stItem.nType = PF_ITEM_COMBOBOX;
	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);

	SET_ST_MOVEITEM(stItem, "btnSend", &m_btnSend);
	stItem.nType = PF_ITEM_BTN;
	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);
	
}

LRESULT CAnalystMainFrmWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{	
	switch(message)
	{
	case MF_PFCONTORL_MOVE:	
		{	
			CWnd *pWnd = GetParent();
			if(!pWnd) return FALSE;
			GetParent()->SendMessage(RMSG_MOVECTRL, wParam, lParam);
			return TRUE;
		}
	case MF_PFCONTORL_EVENT:
		{
			CWnd *pWnd = GetParent();
			if(!pWnd) return FALSE;
			GetParent()->SendMessage(RMSG_PF_CONTROL_EVENT, wParam, lParam);
			return TRUE;
		}
	}
	return CFrameWnd::WindowProc(message, wParam, lParam);
}

void CAnalystMainFrmWnd::PlatFormBtnTotal()
{
	OnBtnListAll();
}
void CAnalystMainFrmWnd::PlatFormBtnKospi()
{
	OnBtnKospi();
}
void CAnalystMainFrmWnd::PlatFormBtnKosdaq()
{
	OnBtnKosdaq();
}
void CAnalystMainFrmWnd::PlatFormBtnSearch()
{
	OnBtnSearch_Result();
}
void CAnalystMainFrmWnd::PlatFormBtnSend()
{
	OnBtnSend();
}

void CAnalystMainFrmWnd::PlatFormBtnComboTime(int nIndex)
{
	m_ctrlTargetCombo_Time.SetCurSel(nIndex);
}

void CAnalystMainFrmWnd::PlatFormBtnCalendar(LPCTSTR szData)
{
	CString strDate = szData;

	if(strDate == "" || strDate.GetLength() != 8)
	{
		CString strMsg = "달력 컨트롤 I/O가 변경 되었습니다. 상담원에게 문의 바랍니다.";
		SetMessageBox(strMsg, PROGRAM_TITLE, MB_YESNO | MB_ICONQUESTION);
		return;
	}

	int nYear = atoi( strDate.Mid(0, 4));
	int nMon  = atoi( strDate.Mid(4, 2));
	int nDay  = atoi( strDate.Mid(6, 2));
    
	CTime time(nYear, nMon, nDay, 0, 0, 0);
	m_ctrlTarget_Data.SetTime(&time);
}


	
void CAnalystMainFrmWnd::OnBtnSend()
{
	CRect reClient;
	m_btnSend.GetWindowRect(reClient);
	
	CPoint ptMenu;
	ptMenu.x = reClient.left;
	ptMenu.y = reClient.bottom;
	SetCommMenu(&ptMenu);
}

void CAnalystMainFrmWnd::OnMenuResultCtrl(UINT nIndex)
{
	CString strCode;

	ShardData		stShardData;
	CodeInfo stCodeInfo;
	CString strPortGroup;
	strPortGroup.Format("%d", nIndex);
	
	if(nIndex >= 11200)
		nIndex = 11010;

	int nDataCnt;
	nDataCnt = m_gridCode.GetNumberRows();
	
	if (nDataCnt <= 0)
	{	
		return;
	}
	
	int nItem;
	for (nItem = 0; nItem < _countof(MENU_RESULTCTRL_INDEX); nItem++)
	{
		if (nIndex == MENU_RESULTCTRL_INDEX[nItem])
			break;
	}
	
	if (nIndex == (int)SCREEN_INDEX_EXCEL)
	{
		if(m_pwndAnalystResult) 
			m_pwndAnalystResult->SetResultSave();
	}
	else
	{
		if(atoi(strPortGroup) >= 11200)
		{
			stShardData.saCode.Add(strPortGroup.Right(2));
			stShardData.saName.Add("");
			stShardData.iaMarkType.Add(0);
		}

		for (int nRow = 0; nRow < nDataCnt; nRow++)
		{
			m_gridCode.GetRowToCodeInfo(nRow, stCodeInfo);
			if (stCodeInfo.bUser && !stCodeInfo.strCode.IsEmpty())
			{
				stShardData.saCode.Add(stCodeInfo.strCode);
				stShardData.iaMarkType.Add(stCodeInfo.nMarket);
				stShardData.saName.Add(stCodeInfo.strName);
			}
		}		
		stShardData.bOpen = MENU_RESULTCTRL_SCREEN_OPEN[nItem];
		stShardData.strScreenNo = MENU_RESULTCTRL_SCREEN_NOMAX[nItem];		
		stShardData.strMapNo = MENU_RESULTCTRL_SCREEN_NOMAX[nItem];
		
		SetSharedDataToSend(&stShardData);
	}
}

void CAnalystMainFrmWnd::SetSharedDataToSend(ShardData *pstShardData)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	CString strScreenNo(pstShardData->strScreenNo);

	if(pstShardData->saCode.GetSize() > 0)
	{
		pScreenWnd->OpenScreen(strScreenNo, &pstShardData->saCode, &pstShardData->saName);
	}
	else
	{
		pScreenWnd->OpenScreen(strScreenNo);
	}
}

BOOL CAnalystMainFrmWnd::SetPortfolioReg(CString strOpenDlg, CStringArray *psaCode)
{
	// 2011.11.10 박성경: 관심등록 >>
// 	CPortAnalystView* pView = (CPortAnalystView*)GetParent();
// 	
// 	CString strCode = psaCode->GetAt(0);
// 	if(pView)
// 	{
// 		IAUGwanManager* pGwanMng = (IAUGwanManager*)AfxGetPctrInterface(UUID_IAUGwanManager, 1);
// 		if(!pGwanMng)
// 			return FALSE;
// 		
// 		CStringArray arrCode;
// 		for(int nIndex = 0; nIndex < psaCode->GetSize(); nIndex++)
// 		{	
// 			strCode = psaCode->GetAt(nIndex);
// 			strCode = strCode.Left(6);
// 			arrCode.Add(strCode);							
// 		}	
// 		pGwanMng->ShowGwansimDlgWithList(this, arrCode);	
// 	}
// 	
	// 2011.11.10 박성경 <<
	return TRUE;
}

void CAnalystMainFrmWnd::SetCommMenu(POINT *point)
{
	CMenu	menu;
	menu.CreatePopupMenu();
	
	CMenu *pMenuChild = NULL;
	pMenuChild = new CMenu;
	pMenuChild->CreatePopupMenu();

	CString strScreenInfo;
	int nMenuType;
	int nMenuCnt = _countof(MENU_RESULTCTRL_NAME);
	for (int nIndex = 0; nIndex < nMenuCnt; nIndex++)
	{
		if (MENU_RESULTCTRL_NAME[nIndex] == "")
			nMenuType = MF_SEPARATOR;
		else
			nMenuType = MF_STRING;		
		
		strScreenInfo = (CString)MENU_RESULTCTRL_SCREEN_NOMAX[nIndex];
		if (atoi(strScreenInfo) <= 0)
			strScreenInfo = "";
		
		if (strScreenInfo.IsEmpty() == FALSE)
		{
			strScreenInfo = strScreenInfo.Left(4);
			strScreenInfo = "[" + strScreenInfo + "] " + (CString)MENU_RESULTCTRL_NAME[nIndex];
		}
		else
			strScreenInfo = "          " + (CString)MENU_RESULTCTRL_NAME[nIndex];
		
		if(nIndex == 0)
		{
			menu.AppendMenu(MF_POPUP, (UINT)pMenuChild->GetSafeHmenu(), (LPCTSTR)strScreenInfo); 
			
			CStringArray arrKey, arrGroupName;
#if(0)//tour2k
			GetData_PortGroup(arrKey, arrGroupName);
#else
			arrKey.Copy(m_GwanSimGroup.strKey);
			arrGroupName.Copy(m_GwanSimGroup.strGroupName);
#endif						
			int nMenuIndex = 11200;
			for(int i = 0; i < arrKey.GetSize(); i++)
				pMenuChild->AppendMenu(MF_STRING, nMenuIndex + atoi(arrKey.GetAt(i)), arrGroupName.GetAt(i));
			
			pMenuChild->AppendMenu(MF_SEPARATOR);
			
			pMenuChild->AppendMenu(MF_STRING, nMenuIndex, "새 관심그룹 추가");
			
		}
		else
			menu.AppendMenu(nMenuType, MENU_RESULTCTRL_INDEX[nIndex], (LPCTSTR)strScreenInfo); 
	}
	
	GetCursorPos(point);
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point->x, point->y, this);

	pMenuChild = menu.GetSubMenu(0);
	if (pMenuChild)
	{
		pMenuChild->DestroyMenu();
		delete pMenuChild;
	}

	menu.DestroyMenu();

}

BOOL CAnalystMainFrmWnd::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// 2011.11.10 박성경 >>
// 	if(!m_pDebugMng || !pCopyDataStruct)
// 		return FALSE;
// 
// 	CString strTr;
// 	STIDM2Info_TranData *pData = (STIDM2Info_TranData*)pCopyDataStruct->lpData;
// 
// 	int nSize = (int)(pCopyDataStruct->cbData);
// 	CString strTrCopy(pData->aDataKey2,sizeof(pData->aDataKey2));
// 
// 
// 	if(strTrCopy.CompareNoCase("piso0350") == 0)
// 		strTr = TRNO_CONDITION;
// 	else if(strTrCopy.CompareNoCase("so035010") == 0)
// 		strTr = TRNO_WORKDATE;
// 	else if(strTrCopy.CompareNoCase("so035020") == 0)
// 		strTr = TRNO_MSTCODE;
// 	else if(strTrCopy.CompareNoCase("piso0352") == 0)
// 		strTr = TRNO_GROUPTOBASE;
// 	else if(strTrCopy.CompareNoCase("so035210") == 0)
// 		strTr = TRNO_GROUPTOBASE_UP;
// 
// 
// 	//	else if(strTrCopy.CompareNoCase("d1010") == 0)
// 	//		strTr = QUERY_STRJONGMOK;
// 
// 	if (strTr.CompareNoCase("") != 0)
// 	{
// 		ReceiveData(strTr, "", "", (LPSTR)(LPCTSTR)pData->pData, nSize-(sizeof(pData)-1));		
// 	}
	// 2011.11.10 박성경 <<

	return CWnd::OnCopyData(pWnd, pCopyDataStruct);
}

int CAnalystMainFrmWnd::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	return ::MessageBox(m_hWnd, lpszText, lpszCaption, nType);
}

BOOL CAnalystMainFrmWnd::OnEraseBkgnd(CDC* pDC)
{
// 	CRect reClient;
// 	GetClientRect(reClient);
// 	pDC->FillSolidRect(reClient, m_clrMainSkin);

	return FALSE;
}

CFont* CAnalystMainFrmWnd::GetFont(BOOL bBold)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	
	CFont* pFont;
	
	if(!bBold)
		pFont = pScreenWnd->GetDefFont();
	else
		pFont = pScreenWnd->GetBoldFont();
	
	return pFont;
}

COLORREF CAnalystMainFrmWnd::GetColor(int nColor)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	
	return pScreenWnd->GetAxColor(nColor);
}

CString CAnalystMainFrmWnd::Variant(int nComm, CString strData)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	
	return pScreenWnd->Variant(nComm, strData);
}

void CAnalystMainFrmWnd::SetBtnImg(CfxImgButton *pBtn, CString strFileName)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	pScreenWnd->SetBtnImg(pBtn, strFileName);
}

void CAnalystMainFrmWnd::SetBtnImg(CfxImgButton *pBtn, int nTextLen, BOOL bBold)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	pScreenWnd->SetBtnImg(pBtn, nTextLen, bBold);
}

COLORREF CAnalystMainFrmWnd::GetAxColor(int nColor)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetAxColor(nColor);
}

HBRUSH CAnalystMainFrmWnd::GetAxBrush(COLORREF clr)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return (HBRUSH)(*pScreenWnd->GetAxBrush(clr));
}

void CAnalystMainFrmWnd::PutLinkData(CString strType, CString strCode)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	pScreenWnd->ShareCode(strCode);
}

void CAnalystMainFrmWnd::GetData_PortGroup(CStringArray& saKey, CStringArray& saGroupName)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
#if (0)//tour2k
	pScreenWnd->GetData_PortGroup(saKey, saGroupName);
#else
//	sprintf( sSend,"%s%80s%s", "MYUg", " ", "00001U");

	int sendL = 0;
	char sendB[16 * 1024] = { 0, }, tempB[32];
	int isz_updn = sz_uinfo;
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;
	FillMemory((char*)updn, isz_updn, ' ');
	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'g';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'U';

	pScreenWnd->SendTR(TRNO_FAVORITE, sendB, sendL, TRKEY_GWANSIMGROUP, US_PASS );
#endif
}

BOOL CAnalystMainFrmWnd::GetData_PortItem(CStringArray& saCode, CStringArray& saName, int nKey)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
#if (0)//tour2k
	return pScreenWnd->GetData_PortItem(saCode, saName, nKey);
#else
//	sprintf(sSend, "%s%80s%s%40s%02d", "MYUj", " ", "00001U", " ", nKey);

	int sendL = 0;
	CString stmp;
	char sendB[16 * 1024] = { 0, }, tempB[32];
	int isz_updn = sz_uinfo + 2;
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;
	FillMemory((char*)updn, isz_updn, ' ');
	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'j';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'U';
	stmp.Format("%02d", nKey );
	memcpy((char*)&sendB[sz_uinfo], stmp, 2);

	pScreenWnd->SendTR(TRNO_FAVORITE, sendB, sendL , TRKEY_GWANSIMITEM, US_PASS );
	return true;
#endif
}

void CAnalystMainFrmWnd::RegPortItem(CStringArray* pSaItemCode)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	pScreenWnd->RegPortItem(pSaItemCode);
}

CWnd* CAnalystMainFrmWnd::GetModuleMain()
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetModuleMainWnd();
}

BOOL CAnalystMainFrmWnd::GetStockMasterData(CStringArray &arrCode, CStringArray &arrName, UINT Jgub, UINT Upgub)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetStockMasterData(arrCode, arrName, Jgub, Upgub);
}

BOOL CAnalystMainFrmWnd::GetUpMasterData(CStringArray *&psaCode, CStringArray *&psaName, int jgub)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetUpMasterData(psaCode, psaName, jgub);
}

int CAnalystMainFrmWnd::GetUserMaster(LPCSTR szKey, CStringArray &arrCode, CStringArray &arrName)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetUserMaster(szKey, arrCode, arrName);
}

CPen* CAnalystMainFrmWnd::GetAxPen(COLORREF clr, int nWidth, int nStyle)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetAxPen(clr, nWidth, nStyle);
}
// 2012.4.5 박성경: winix플랫폼 R메뉴 호출 >>
void CAnalystMainFrmWnd::OnRButtonClick(CPoint *point, CString strCode /*= ""*/)
{
	CWnd*		pModuleMainWnd;
	pModuleMainWnd = GetModuleMain();
	GetCursorPos(point);
	ScreenToClient(point);
	ClientToScreen(point);
	
	CString strLPARM;
	strLPARM.Format("%s\n%d\t%d\n1301\t%s\n", "IB712900", point->x, point->y, strCode);
	// mapN \n pt.x \t pt.y \n SYM \t DATA \n
	pModuleMainWnd->SendMessage(WM_USER, MAKEWPARAM(mouseDLL, FALSE), (LPARAM)strLPARM.operator LPCTSTR());
}
// 2012.4.5 박성경: winix플랫폼 R메뉴 호출 <<