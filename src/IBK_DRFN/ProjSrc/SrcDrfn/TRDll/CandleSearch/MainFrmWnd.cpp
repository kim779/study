// MainFrmWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrmWnd.h"

#include "Winsock2.h"

#include "../../DrCommon/Dr_Control/DrXLSFile.h"

#include "../../../SrcSite/include/jmcode.h"
#include "../../../SrcSite/include/axis_msg.h"

#include "./CandleChart/DlgChart_Stock.h"

#include "FieldEditDlg.h"
#include "DlgSaveCandle.h"
#include "DlgHtml.h"
#include "ScreenWnd.h"

#include "../../Chart/Include_Chart/Dll_Load/IMetaTable.h"

#define	ITTIMER_SELECTROW0	10
int nCtrlID[] =
{ 
	IDC_BTN_SEARCH,
	IDC_BTN_PRINT,
	IDC_BTN_FIELDEDIT,
	IDC_BTN_RESULTCTRL,
	IDC_BTN_SAVE,
	IDC_BTN_ERASER
};

//extern CString		KSLink_GetFileHeader();
#define MAX_BASEBONG	10
#define MAX_UPDOWNRATE	30

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE g_hInstance;

/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd
// Area Define
#define THUMB_WIDTH		55
#define THUMB_HEIGHT	52

#define DEFAULT_BODYRATE	 3
#define DEFAULT_BODYRATE_STR "3"

IMPLEMENT_DYNCREATE(CMainFrmWnd, CFrameWnd)

const		LPCTSTR		LIST_TARGET_1ST[] = { "전체", "거래소", "코스닥", "관심종목"};
const		LPCTSTR		LIST_TIME[] = { " 1", " 3", " 5", "10", "15", "30", "60" };
const		UINT		LIST_TIME_INDEX[] = { 3, 4, 5, 6, 7, 8, 9 };

CMainFrmWnd::CMainFrmWnd()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);
	GETFONT(logFont, 10, 200, m_fontSmall);

	m_brushBK.CreateSolidBrush(COLOR_MAIN_CANVAS);
	
	m_nTrIndex = 0;
	m_strTargetBaseType = "";

	m_nMarket = 0;

	m_bLink = TRUE;

	m_ptabCondition = NULL;
	
	m_clrMainSkin = RGB(247,248,252);

	m_bExpand = FALSE;

	m_bUpdownState[0] = TRUE;
	m_bUpdownState[1] = TRUE;
	m_bUpdownState[2] = TRUE;
	m_bUpdownState[3] = TRUE;
	m_bUpdownState[4] = TRUE;
	m_strCaption = "캔들패턴 그리기";
	
	m_bSetControlBase = FALSE;

	m_pDlgChart = NULL;

	m_nPeriodCnt = 4;

	m_nPreReceiveLen = -1;
}

CMainFrmWnd::~CMainFrmWnd()
{
}

BEGIN_MESSAGE_MAP(CMainFrmWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrmWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP

	ON_EN_KILLFOCUS(IDC_EDIT_CTRL, OnKillfocusBongCnt)
	ON_EN_KILLFOCUS(IDC_EDIT_RATE1, OnKillfocusRate1)
	ON_EN_KILLFOCUS(IDC_EDIT_RATE2, OnKillfocusRate2)
	ON_EN_KILLFOCUS(IDC_EDIT_RATE3, OnKillfocusRate3)
	ON_EN_KILLFOCUS(IDC_EDIT_RATE4, OnKillfocusRate4)
	ON_EN_KILLFOCUS(IDC_EDIT_RATE5, OnKillfocusRate5)

	ON_CBN_SELCHANGE(IDC_COMBO_TARGET_1ST, OnChgComboTarget1st)
	ON_CBN_SELCHANGE(IDC_COMBO_TARGET_2ND, OnChgComboTarget2nd)

	ON_BN_CLICKED(IDC_BTN_DAY, OnBtnDay)
	ON_BN_CLICKED(IDC_BTN_WEEK, OnBtnWeek)
	ON_BN_CLICKED(IDC_BTN_MONTH,OnBtnMon)
	ON_BN_CLICKED(IDC_BTN_MIN, OnBtnMin)

	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_BN_CLICKED(IDC_BTN_ERASER, OnBtnReset)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	ON_BN_CLICKED(IDC_CHK_BODYRATE, OnBtnChkBodyRate)

	ON_MESSAGE(WM_NOTIFY_GRID, OnNotify_Grid)
	ON_NOTIFY(NM_CLICK, IDC_ITEM_LIST, OnClickListItem)
	ON_NOTIFY(NM_CLICK, IDC_ITEM_USERLIST, OnClickUserListItem)
	ON_MESSAGE(WM_NOTIFY_CANDLEPATTEN, OnNotify_CandlePatten)

	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SEARCH, OnChangeTab)

	ON_BN_CLICKED(IDC_BTN_PRINT, OnBtnPrint)
	ON_BN_CLICKED(IDC_BTN_FIELDEDIT, OnBtnFieldEdit)
	ON_BN_CLICKED(IDC_BTN_RESULTCTRL, OnBtnResultCtrl)
	ON_BN_CLICKED(IDC_BTN_RESULTEXPAND, OnBtnResultExpand)
	ON_BN_CLICKED(IDC_BTN_RESULTEXPAND_DOWN, OnBtnResultExpand)
	ON_COMMAND_RANGE(ID_MENU_RESULTCTRL_START, ID_MENU_RESULTCTRL_END, OnMenuResultCtrl)
	ON_COMMAND_RANGE(IDC_BTN_RATE1, IDC_BTN_RATE5, OnClickBtnRate)

	ON_REGISTERED_MESSAGE(RMSG_DELETE_ITEM, OnDeleteUserCandle)
	ON_REGISTERED_MESSAGE(RMSG_HELPHTML_ITEM, OnHelpHtml)
	ON_REGISTERED_MESSAGE(RMSG_GET_TIPTEXT, OnGetTipText)
	ON_REGISTERED_MESSAGE(RMSG_RECEIVETRANDATA, OnReceiveTranData)
	ON_REGISTERED_MESSAGE(RMSG_RECEIVEDIRECTFID, OnReceiveDirectFID)

	ON_REGISTERED_MESSAGE(RMSG_USERINFO, OnUserInfo)
END_MESSAGE_MAP()
//	ON_BN_CLICKED(IDC_BTN_LOAD, OnBtnLoad)
	//ON_CBN_SELCHANGE(IDC_COMBO_TARGET, OnChgItemCombo)

// 	ON_EN_CHANGE(IDC_EDIT_CTRL, OnEdChangeBongCnt)
// 	ON_EN_CHANGE(IDC_EDIT_RATE1, OnEdChangeRate1)
// 	ON_EN_CHANGE(IDC_EDIT_RATE2, OnEdChangeRate2)
// 	ON_EN_CHANGE(IDC_EDIT_RATE3, OnEdChangeRate3)
// 	ON_EN_CHANGE(IDC_EDIT_RATE4, OnEdChangeRate4)
// 	ON_EN_CHANGE(IDC_EDIT_RATE5, OnEdChangeRate5)



/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd message handlers

int CMainFrmWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CString strData;
	
	GetDir_DrfnRoot(m_strAppPath);

	g_iMetaTable.m_strDataFolderPath.Format(_T("%s\\icss\\"), m_strAppPath);
	g_iMetaTable.LoadMetaTable();

	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	m_strScreenNo = pScreenWnd->GetFormCode();

	CStringArray saPatternFileData;

	m_saPatternItemFile.RemoveAll();
	CreateImageList();
	ReadPresetCandle();
	MakeCandleImageList();

	CheckUserPatternDir();
	ReadUserCandleSet();
	MakeUserCandleImageList();

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	CreateButton();
	CreateStatic();
	CreateCombo();
	CreateMisc();

	AfxSetResourceHandle(hInstSave);

	m_pITrCommManager = (IAUTrCommManager*)AfxGetPctrInterface(UUID_IAUTrCommManager);
	if(m_pITrCommManager)
		m_pITrComm = (ITrComm2*)m_pITrCommManager->AdviseTrComm();

	//tour2k
	Request_PortGroup();

	return 0;
}

void CMainFrmWnd::SetInit()
{	
	GetLoadSet_Def();
	m_comboTail.SetCurSel(m_nDefult_Tail);
	if(m_nDefult_AllColorChk) m_btnIgnoreColorChk.SetCheck(1);
	m_comboPrevTrend.SetCurSel(m_nDefult_PreTrend);
	
	m_gridResult.SetInit("RESULT");
	m_gridResult.ClearAll();
	
	SetLoad_FieldInfo();
	SetGrid_Head(FALSE);
	SetGrid_Head(TRUE);
	m_gridResult.SetMultiSelectMode(TRUE);

	_DoGridWidthSet(0);	//0000207
	
	//////////////////////////////////////////////////////////////////////////
	// 초기값 적용
	if (m_nDefult_TargetBaseType == 0)	// 전체
	{
		m_strTargetBaseType = "0";
	}
	else if (m_nDefult_TargetBaseType == 1) // 코스피-전체
	{
		m_strTargetBaseType = "10";
	}
	else if (m_nDefult_TargetBaseType == 2) // 코스닥-전체
	{
		m_strTargetBaseType = "20";
	}
// 	else if (m_nDefult_TargetBaseType == 3) // 테마
// 	{
// 		m_strTargetBaseType = "30";
// 	}
	else if (m_nDefult_TargetBaseType == 3) // 관심
	{
		m_strTargetBaseType = "40";
	}

	m_comboTarget_1st.SetCurSel(m_nDefult_TargetBaseType);
	
	m_btnExceptionChk.SetCheck(m_nDefult_ExceptPreferred);
 	m_btnReverseChk.EnableWindow(FALSE);
	
	m_strChartTerm.Format("%d", m_nDefult_ChartTerm);

	m_comboTime.SetCurSel(m_nDefult_ChartTermMin);
	m_comboTime.EnableWindow(FALSE);
	m_comboTime.Invalidate();
	
	if (m_nDefult_ChartTerm == 0)		//# 틱
	{
	}
	else if (m_nDefult_ChartTerm == 1)  //# 분
	{
		m_btnMin.SetCheck(TRUE);
		m_comboTime.EnableWindow(TRUE);
		m_comboTime.Invalidate();
	}
	else if (m_nDefult_ChartTerm == 2) //# 일
		m_btnDay.SetCheck(TRUE);
	else if (m_nDefult_ChartTerm == 3)	//# 주
		m_btnWeek.SetCheck(TRUE);
	else if (m_nDefult_ChartTerm == 4)	//# 월
		m_btnMonth.SetCheck(TRUE);
	else if (m_nDefult_ChartTerm == 5)	//# 년
	{
	}
	
	CString strBongPass;
	strBongPass.Format("%d", m_nDefult_BongPass);
	m_editBongCnt.SetWindowText(strBongPass);

	//////////////////////////////////////////////////////////////////////////
	CString strPath;
	GetDir_DrfnRoot(strPath);
// 	m_Chart.SetInit(this->GetSafeHwnd(), (long)m_pBaseDesk, strPath, FALSE);
 	m_pDlgChart->m_ChartItem.LoadAddIn("ChartPeriodSummaryAddIn.dll:DRAG_CANDLE_ANAL");

	m_brushBK.DeleteObject();
	m_brushBK.CreateSolidBrush(m_clrMainSkin);  
	SetTrData_MSTCode();
	SetTrData_ThemeGroup();

	CString strType, strUserFileName;
	if(LookUpSearchCenterModify(strType, strUserFileName))
	{
		if(strType == "1")
			SearchUserItem(strUserFileName);
	}
}

void CMainFrmWnd::CreateImageList()
{
	CString strImgPath;
	strImgPath = m_strAppPath + (CString)FOLDER_IMGDATA + "\\W58_H20.bmp";

	//////////////////////////////////////////////////////////////////////////
	strImgPath = m_strAppPath + (CString)FOLDER_IMGDATA + "\\W20_H20.bmp";

	if ( !m_ImageListBtn.m_hImageList)
	{
		if ( !m_ImageListBtn.Create( 20, 20, ILC_MASK | ILC_COLOR24, 1, 1 ) )
		{
			SetMessageBox("Can not Find W20_H20.bmp file");			
		}
		CBitmap bitmap;
		bitmap.Attach ((HBITMAP)::LoadImage(NULL, strImgPath, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImageListBtn.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}

	strImgPath = m_strAppPath + (CString)FOLDER_IMGPATTERN + "\\";
	if ( !m_ImageBK.m_hImageList)
	{
		if ( !m_ImageBK.Create( THUMB_WIDTH, THUMB_HEIGHT, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			SetMessageBox("Can not Find bmp file");			
		}	
		
		CBitmap bitmap;
		BOOL bOK = bitmap.Attach((HBITMAP)::LoadImage(NULL, strImgPath + "CandlePattern.bmp", IMAGE_BITMAP, 0, 0,
												LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE));
		m_ImageBK.Add(&bitmap, RGB(255,0,0));

		bitmap.DeleteObject();
	}
}

void CMainFrmWnd::CreateButton()
{
	m_btnReverseChk.Create(_T("지정그룹제외"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_REVERSECHK);
	m_btnReverseChk.SetFont(&m_fontDefault);
	
	m_btnBodyRateChk.Create(_T("몸통비율적용"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_CHK_BODYRATE);
	m_btnBodyRateChk.SetFont(&m_fontDefault);

	m_btnExceptionChk.Create(_T("우선주제외"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_EXCEPTIONCHK);
	m_btnExceptionChk.SetFont(&m_fontDefault);

	m_btnIgnoreColorChk.Create(_T("양음봉무시"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_IGNORECOLORCHK);
	m_btnIgnoreColorChk.SetFont(&m_fontDefault);
	
	m_btnDay.Create(_T("일"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
		CRect(0, 0, 0, 0), this, IDC_BTN_DAY);
	m_btnDay.SetFont(&m_fontDefault);
	
	m_btnWeek.Create(_T("주"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
		CRect(0, 0, 0, 0), this, IDC_BTN_WEEK);
	m_btnWeek.SetFont(&m_fontDefault);
	
	m_btnMonth.Create(_T("월"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
		CRect(0, 0, 0, 0), this, IDC_BTN_MONTH);
	m_btnMonth.SetFont(&m_fontDefault);
	
	m_btnMin.Create(_T("분"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
		CRect(0, 0, 0, 0), this, IDC_BTN_MIN);
	m_btnMin.SetFont(&m_fontDefault);

	m_btnDay.FP_vSetImage(&m_ImageListBtn, /*m_clrMainSkin,*/ 81, 82, TRUE, "일", TRUE);
	m_btnWeek.FP_vSetImage(&m_ImageListBtn, /*m_clrMainSkin,*/ 81, 82, TRUE, "주", TRUE);
	m_btnMonth.FP_vSetImage(&m_ImageListBtn, /*m_clrMainSkin,*/ 81, 82, TRUE, "월", TRUE);
	m_btnMin.FP_vSetImage(&m_ImageListBtn, /*m_clrMainSkin,*/ 81, 82, TRUE, "분", TRUE);

	for(int i = 0; i < MAX_CANDLE; i++)
	{
		m_btnUpdown[i].Create(_T("↑"), CRect(-20, -20, 0, 0), this, IDC_BTN_RATE1 + i);
		SetBtnImg(&m_btnUpdown[i], 1, TRUE);
		CString	strTip = "이상";
	}
	
	m_btnSave.Create(_T("저장"), CRect(0, 0, 0, 0), this, IDC_BTN_SAVE);
	SetBtnImg(&m_btnSave, 2);
		
	m_btnReset.Create(_T("초기화"), CRect(0, 0, 0, 0), this, IDC_BTN_ERASER);
	SetBtnImg(&m_btnReset, 4);
	
	m_btnSearch.Create(_T("검  색"), CRect(0, 0, 0, 0), this, IDC_BTN_SEARCH);
	SetBtnImg(&m_btnSearch, 4, TRUE);
	
	m_btnPrint.Create(_T("인쇄"), CRect(0, 0, 0, 0), this, IDC_BTN_PRINT);
	SetBtnImg(&m_btnPrint, 2);

	m_btnFieldEdit.Create(_T("항목편집"), CRect(0, 0, 0, 0), this, IDC_BTN_FIELDEDIT);
	SetBtnImg(&m_btnFieldEdit, 4);
    
	m_btnResultCtrl.Create(_T("종목전송"), CRect(0, 0, 0, 0), this, IDC_BTN_RESULTCTRL);
	SetBtnImg(&m_btnResultCtrl, 4);
	
	m_btnExpand.Create(_T(""), CRect(0, 0, 20, 20), this, IDC_BTN_RESULTEXPAND);
	SetBtnImg(&m_btnExpand, "UP_EX");
	

	//2009.03.20 by LYH >>
}

void CMainFrmWnd::CreateStatic()
{
	m_stcStatusBar.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
	m_stcStatusBar.ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	m_ctrlCandleName.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlCandleName.SetFont(&m_fontDefault);

	m_ctrlJongName.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlJongName.SetFont(&m_fontDefault);

	m_ctrlTarget.Create(_T("대상"), WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlTarget.SetFont(&m_fontDefault);

	m_ctrlState.Create(_T("진행상태"), WS_CHILD | WS_VISIBLE | SS_RIGHT |SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlState.SetFont(&m_fontDefault);

	m_ctrlTime.Create(_T("검색시간 :     "), WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlTime.SetFont(&m_fontDefault);

	m_ctrlNotice.Create(_T("※ 본 내용은 투자판단의 참고사항이며 투자 판단의 최종 책임은 본 서비스를 사용하시는 이용자에게 있습니다."), 
		WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, CRect(0, 0, 0, 0), this);
	m_ctrlNotice.SetFont(&m_fontDefault);

	m_ctrlCodeCntText.Create(_T("검색결과 "), WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlCodeCntText.SetFont(&m_fontDefault);
	m_ctrlCodeCntText.ShowWindow(SW_HIDE);

	m_ctrlCodeCnt.Create(_T("    "), WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlCodeCnt.SetFont(&m_fontBold);

	m_ctrlBongCntText1.Create(_T("기준"), WS_CHILD | WS_VISIBLE | SS_LEFT |SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlBongCntText1.SetFont(&m_fontDefault);

	m_ctrlBongCntText2.Create(_T("봉전(0은 현재봉, 최대10)"), WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlBongCntText2.SetFont(&m_fontDefault);

	for(int i = 0; i < MAX_CANDLE; i++)
	{
		m_ctrlPersent[i].Create(_T("%"), WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, 
			CRect(0, 0, 0, 0), this);
		m_ctrlPersent[i].SetFont(&m_fontDefault);
	}

}

void CMainFrmWnd::CreateCombo()
{
	m_comboTarget_1st.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(0, 0, 0, 0), this, IDC_COMBO_TARGET_1ST);
	m_comboTarget_1st.SetFont(&m_fontDefault);
	int nIndex;
	for (nIndex = 0; nIndex < _countof(LIST_TARGET_1ST); nIndex++)
		m_comboTarget_1st.AddString(LIST_TARGET_1ST[nIndex]);
	
	m_comboTarget_2nd.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(0, 0, 0, 0), this, IDC_COMBO_TARGET_2ND);
	m_comboTarget_2nd.SetFont(&m_fontDefault);
	m_comboTarget_2nd.EnableWindow(FALSE);
	//////////////////////////////////////////////////////////////////////////
	m_comboTime.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(0, 0, 0, 0), this, IDC_COMBO_TIME);
	m_comboTime.SetFont(&m_fontDefault);
	
	CString srtTerm;
	for (nIndex = 0; nIndex < _countof(LIST_TIME); nIndex++)
	{
		srtTerm = (CString)LIST_TIME[nIndex] + "분";
		m_comboTime.AddString(srtTerm);
	}
	
	m_comboTail.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(0, 0, 0, 0), this, IDC_COMBO_TAIL);
	m_comboTail.SetFont(&m_fontDefault);
	char* szTailText[] = {"꼬리길이적용", "꼬리길이무시", "꼬리유무무시"};
	for (nIndex = 0; nIndex < 3; nIndex++)
		m_comboTail.AddString(szTailText[nIndex]);
	m_comboTail.SetCurSel(0);

	// 추세 : 20090618 수정, 컨트롤 숨김
	m_comboPrevTrend.Create(WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST, CRect(0, 0, 0, 0), this, IDC_COMBO_TREND);
	m_comboPrevTrend.SetFont(&m_fontDefault);

	char* szPrevText[] = {"추세무관", "상승추세", "하락추세"};
	for (nIndex = 0; nIndex < 3; nIndex++)
		m_comboPrevTrend.AddString(szPrevText[nIndex]);
	m_comboPrevTrend.SetCurSel(0);
}

void CMainFrmWnd::CreateMisc()
{
	// Edit Control
	m_editBongCnt.CreateEx(WS_EX_CLIENTEDGE, "edit", "0", WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, IDC_EDIT_CTRL);
	m_editBongCnt.SetFont(&m_fontDefault);

	for(int i = 0; i < MAX_CANDLE; i++)
	{
		m_editUpdownRate[i].CreateEx(WS_EX_CLIENTEDGE, "edit", "3", WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, IDC_EDIT_RATE1 + i);
		m_editUpdownRate[i].SetFont(&m_fontDefault);
	}

	// List Control
	m_ctrlListItem.Create(CRect(0, 0, 100, 100), this, WS_CHILD|WS_VISIBLE|WS_TABSTOP, IDC_ITEM_LIST);
	m_ctrlListItem.SetDefaultFont(&m_fontDefault);
	m_ctrlListItem.CreateTooltip();
	m_ctrlListItem.SetMargin(4, 9);
	m_ctrlListItem.SetImageSpace(2, 6);
//	m_ctrlListItem.SetUseHelpMenu(TRUE);  // 패턴설명을 보고싶을 때, 메뉴표시
	m_ctrlListItem.SetImageData(m_ImageListItem, 3, THUMB_WIDTH, THUMB_HEIGHT);

	m_ctrlUserListItem.Create(CRect(0, 0, 100, 100), this, WS_CHILD|WS_VISIBLE|WS_TABSTOP, IDC_ITEM_USERLIST);
	m_ctrlUserListItem.SetDefaultFont(&m_fontDefault);
	m_ctrlUserListItem.CreateTooltip();
	m_ctrlUserListItem.SetMargin(4, 9);
	m_ctrlUserListItem.SetImageSpace(2, 6);
	m_ctrlUserListItem.SetUseDeleteMenu(TRUE); // 패턴을 지우고 싶을 때, 메뉴표시
	m_ctrlUserListItem.SetImageData(m_ImageUserListItem, 3, THUMB_WIDTH, THUMB_HEIGHT);
	m_ctrlUserListItem.ShowWindow(SW_HIDE);

	// Candle-Maker control
	m_CandleMgr.Create(200, CPoint(10,10), CSize(70,150), this, "C\\");
	m_CandleMgr.SetFocusCandle(4);
	m_CandleMgr.SetFont(&m_fontDefault);
//	m_CandleMgr.SetYScale(TRUE, 25);

	// grid
	m_gridResult.CreateGrid(WS_CHILD | WS_VISIBLE| WS_BORDER,
		CRect(0, 0, 0, 0), this, IDC_GRID_CTRL);
	
	//////////////////////////////////////////////////////////////////////////
	// create tab control
	m_ptabCondition = new CDrTabCtrl();
	m_ptabCondition->Create(0, this, IDC_TAB_SEARCH);
	m_ptabCondition->SetBKColor(COLOR_MAIN_CANVAS);
	m_ptabCondition->SetCurSel(0);
		
	TC_ITEM item;
	item.mask = TCIF_TEXT | TCIF_IMAGE;
	item.pszText = "기본";
	m_ptabCondition->InsertItem(0, &item);

	item.pszText = "사용자";
	m_ptabCondition->InsertItem(1, &item);
	//////////////////////////////////////////////////////////////////////////
	// Create chart
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	m_pDlgChart		  =	new CDlgChart_Stock;
	m_pDlgChart->Create(IDD_DLG_CHART, this );
	m_pDlgChart->ModifyStyle(0, WS_CLIPCHILDREN);
	m_pDlgChart->m_ChartItem.SetMultiItemMode(TRUE);
	m_pDlgChart->ShowWindow(SW_SHOW);

	AfxSetResourceHandle(hInstSave);
}

void CMainFrmWnd::SetTrData_MSTCode()
{
	int nType = 2;

	char* lpData = new char[sizeof(int) + 1];
	nType = ntohl(nType);
	::memset(lpData, 0x00, sizeof(int) + 1);
	memcpy(lpData, &nType, sizeof(int) + 1);

	int nRet = Module_SendTr((CString)TRNO_MSTCODE, sizeof(int), (BYTE *)lpData);

	//SetStateTitleBar("[조회중]");
		
	if (lpData)
		delete[] lpData;
}

void CMainFrmWnd::OnDestroy() 
{
	_DoGridWidthSet(1);	//0000207

	SaveUserCandleSet();

	if (m_ptabCondition != NULL)
	{
		delete m_ptabCondition;
		m_ptabCondition = NULL;
	}
	
	if(m_pITrCommManager)
	{
		if(m_pITrComm)
		{
			m_pITrCommManager->UnAdviseTrComm(m_pITrComm);
			m_pITrComm = NULL;
		}
	}
	
	if(m_pDlgChart)			// 차트 Dlg
	{
		if(m_pDlgChart->m_hWnd && IsWindow(m_pDlgChart->m_hWnd))
			m_pDlgChart->DestroyWindow();
		delete m_pDlgChart;
		m_pDlgChart = NULL;
	}

	int nSize = m_CandleSetArray.GetSize();
	int i;
	for(i = 0; i < nSize; i++)
		delete m_CandleSetArray.GetAt(i);
	m_CandleSetArray.RemoveAll();

	nSize = m_UserCandleSetArray.GetSize();
	for(i = 0; i < nSize; i++)
		delete m_UserCandleSetArray.GetAt(i);
	m_UserCandleSetArray.RemoveAll();

	m_ImageListItem.DeleteImageList();
	m_ImageUserListItem.DeleteImageList();
	m_ImageListBtn.DeleteImageList();
	m_ImageBK.DeleteImageList();

	m_brushBK.DeleteObject();
	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();
	m_fontSmall.DeleteObject();

// 	if(m_pctrlDrawPatten)
// 	{
// 		if(m_pctrlDrawPatten->GetSafeHwnd())
// 			m_pctrlDrawPatten->SendMessage(WM_DESTROY);
// 		SafeDelete(m_pctrlDrawPatten);
// 	}
	CFrameWnd::OnDestroy();
	
	
	/*	POSITION pos;
	UniversGroup	stUniversGroup;
	for (int nIndex = 0; nIndex < m_listUnivers.GetCount(); nIndex++)
	{
	pos = m_listUnivers.FindIndex(nIndex);
	stUniversGroup = m_listUnivers.GetAt(pos);
	
	  stUniversGroup.psaItem->RemoveAll();
	  delete stUniversGroup.psaItem;
	  stUniversGroup.psaItem = NULL;
	  }
	  m_listUnivers.RemoveAll();
	*/
}

LRESULT CMainFrmWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
	{
	case MF_PFCONTORL_MOVE:	
		{	
			CWnd *pWnd = GetParent();
			if(!pWnd) return FALSE;
			GetParent()->GetParent()->SendMessage(RMSG_KSMOVECTRL, wParam, lParam);
			return TRUE;
		}
	case MF_PFCONTORL_EVENT:
		{
			CWnd *pWnd = GetParent();
			if(!pWnd) return FALSE;
			GetParent()->GetParent()->SendMessage(RMSG_PF_CONTROL_EVENT, wParam, lParam);
			return TRUE;
		}
	}
	
	return CFrameWnd::WindowProc(message, wParam, lParam);
}

void CMainFrmWnd::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	MoveControls();
	OnPF_Contorl_Move();
}

void CMainFrmWnd::MoveControls()
{
	CRect reClient, reTop, reBottom, reStatusBar;
	GetClientRect(reClient);
	
	reClient.top += 6;
	reClient.bottom -= 6;
	reClient.left += 6;
	reClient.right -= 6;
	
	reTop = reClient;
	reBottom = reClient;
	//# Menu
	reTop.bottom = reTop.top + 20;
	OnSize_Menu(reTop);
	
	reBottom.top = reBottom.bottom - 20;
	reStatusBar = reBottom;

	reBottom.left += 2;
	reBottom.right = reBottom.left + 650;
	//m_ctrlTime.MoveWindow(&reBottom);
	m_ctrlNotice.MoveWindow(&reBottom);

	reBottom.right = reClient.right - 5;
	reBottom.left = reBottom.right - 170;
	m_ctrlState.MoveWindow(&reBottom);

	reStatusBar.top -= 2;
	reStatusBar.bottom += 3;
	reStatusBar.left -= 3;
	reStatusBar.right += 3;
	m_stcStatusBar.MoveWindow(&reStatusBar);

	int nWidth_Left = 197;
	reTop.top = reTop.bottom + 7;
	reTop.bottom = reBottom.top - 8;
	
	reTop.right = reTop.left + nWidth_Left;
	OnSize_Left(reTop);
	
	reTop.left = reTop.right + 5;
	reTop.right = reClient.right;
	reTop.top += 22;
	OnSize_Right(reTop);
}

void CMainFrmWnd::OnSize_Menu(CRect reClient)
{
	CRect reLeft, reRight;
	
	//////////////////////////////////////////////////////////////////////////
	// 첫번째 줄
	reLeft = reClient;
	reLeft.left += 197 + 5;

	reLeft.right = reLeft.left + 27;
	m_ctrlTarget.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 1;
	reLeft.right = reLeft.left + 75;
	reLeft.bottom += 80;
	m_comboTarget_1st.MoveWindow(&reLeft);
	
	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + 150;
	reLeft.bottom += 100;
	m_comboTarget_2nd.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 10;
	reLeft.right = reLeft.left + 80;
	reLeft.bottom -= 180;
	m_btnExceptionChk.MoveWindow(&reLeft); // 우선주 제외
	
	reLeft.left = reLeft.right + 5;
	reLeft.right = reLeft.left + 90;
	m_btnReverseChk.MoveWindow(&reLeft); // 지정그룹제외


	//////////////////////////////////////////////////////////////////////////
	// 두번째 줄
	reLeft = reClient;
	reLeft.OffsetRect(0, 24);
	reLeft.left += 197 + 5;

	reLeft.top -= 1;
	reLeft.bottom -= 1;

	reLeft.right = reLeft.left + 20;
	m_btnDay.MoveWindow(&reLeft);;  // 일

	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + 20;
	m_btnWeek.MoveWindow(&reLeft);;  // 주

	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + 20;
	m_btnMonth.MoveWindow(&reLeft);;  // 월

	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + 20;
	m_btnMin.MoveWindow(&reLeft);  // 분

	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + 49;
	reLeft.bottom += 100;
	m_comboTime.MoveWindow(&reLeft);
	reLeft.bottom -= 100;

	reLeft.left = reLeft.right + 20;
	reLeft.right = reLeft.left + 80;
	m_btnIgnoreColorChk.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 10;
	reLeft.right = reLeft.left + 98;
	reLeft.bottom = reLeft.top + 70;
	m_comboTail.MoveWindow(&reLeft);

	reLeft.left = reLeft.right + 2;
	reLeft.right = reLeft.left + 75;
	m_comboPrevTrend.MoveWindow(&reLeft);

	//////////////////////////////////////////////////////////////////////////
	reRight = reClient;
	reRight.OffsetRect(0, 24);
	reRight.bottom = reRight.top + 20;

	reRight.left = reRight.right - 145;
	m_ctrlBongCntText2.MoveWindow(&reRight);	

	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - 20;
	m_editBongCnt.MoveWindow(&reRight);	

	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - 26;
	m_ctrlBongCntText1.MoveWindow(&reRight);	

	//////////////////////////////////////////////////////////////////////////
}

void CMainFrmWnd::OnSize_Left(CRect reClient)
{
	CRect reTop, reBtn;
	
	reTop = reClient;

	reTop.right = 200;
	reTop.top -= 25;
	reTop.bottom++;

	m_rtTab = reTop;
	m_rtTab.top += 18;
	m_rtTab.bottom += 1;
	m_rtTab.left -= 5;
	m_rtTab.right += 1;

	CRect rtTemp;
	rtTemp = reTop;
	rtTemp.top -= 4;
	rtTemp.left -= 3;
	rtTemp.bottom = reTop.bottom + 3;

	if(m_ptabCondition)
		m_ptabCondition->MoveWindow(&rtTemp);
	CRect rcTab;
	m_ptabCondition->GetItemRect(0, &rcTab);

	reTop.top += (rcTab.bottom-4);
 	reTop.left -= 2;
	reTop.DeflateRect(0,2,1,0);
	m_ctrlListItem.MoveWindow(&reTop);
	if (m_ptabCondition->GetCurSel() == 0) m_ctrlListItem.BringWindowToTop();

	m_ctrlUserListItem.MoveWindow(&reTop);
	if (m_ptabCondition->GetCurSel() == 1) m_ctrlUserListItem.BringWindowToTop();
}

void CMainFrmWnd::OnSize_Right(CRect reClient)
{
	CRect reTop, reBottom, reLine, reRight;
	
	int nBasePos;
	nBasePos = (reClient.bottom - reClient.top) / 8;
	
	//////////////////////////////////////////////////////////////////////////
	// Candle Control
	reBottom = reClient;
//	reBottom.top += 20;
	reBottom.bottom = reBottom.top + nBasePos*4 + 15;
	reBottom.right = reBottom.left + reClient.Width()/2 + 20;
	if(m_CandleMgr.m_bCreated)
	{
		reTop = reBottom;
		reTop.top -= 1;
		reTop.bottom = reTop.top + 28;

		m_reTopChart = m_reTopCandle = reTop;
		m_reTopChart.left = reBottom.right + 2;
		m_reTopChart.right = reClient.right + 3;

		reTop.DeflateRect(0, 4);
		reTop.right -= 3;
		reTop.left = reTop.right - 36;
		m_btnSave.MoveWindow(&reTop);
		
		reTop.right = reTop.left - 2;
		reTop.left = reTop.right - 60;
		m_btnReset.MoveWindow(&reTop);

		reTop.right = reTop.left - 2;
		reTop.left = reTop.right - 92;
		reTop.OffsetRect(0, 2);
		m_btnBodyRateChk.MoveWindow(&reTop);

		reTop = m_reTopCandle;
		reTop.DeflateRect(0, 4);
		reTop.right -= 200;
		reTop.left += 2;
		m_ctrlCandleName.MoveWindow(&reTop);	// 캔들패턴 이름

		reBottom.top += 30;
		if(m_btnBodyRateChk.GetCheck()) reBottom.bottom -= 23;
		m_CandleMgr.MoveWindow(reBottom.left, reBottom.top, reBottom.Width(), reBottom.Height());
		if(m_btnBodyRateChk.GetCheck())
		{
			reBottom.bottom += 23;

			CRect rcRate;
			for(int i = 0; i < MAX_CANDLE; i++)
			{
				m_CandleMgr.GetWindowRect(m_rcBodyRate[i], i);
				ScreenToClient(	m_rcBodyRate[i] );

				m_rcBodyRate[i].top = m_rcBodyRate[i].bottom - 1;
				m_rcBodyRate[i].bottom = m_rcBodyRate[i].top + 24;

				rcRate = m_rcBodyRate[i];

				rcRate.OffsetRect(6,3);
				rcRate.right = rcRate.left + 22;
				rcRate.bottom = rcRate.top + 18;
				m_editUpdownRate[i].MoveWindow(&rcRate);

				rcRate.left = rcRate.right + 1;
				rcRate.right = rcRate.left + 12;
				m_ctrlPersent[i].MoveWindow(&rcRate);

				rcRate.left = rcRate.right;
				rcRate.right = rcRate.left + 20;
				m_btnUpdown[i].MoveWindow(&rcRate);
			}
		}
	
	}
	
	reTop = m_reTopChart;
	reTop.top += 2;
	reTop.bottom -= 2;
	reTop.left += 10;
	reTop.right = reTop.left + 100;
	m_ctrlJongName.MoveWindow(&reTop);	// 종목명

	// 챠트
	reBottom.left = reBottom.right + 2;
	reBottom.right = reClient.right + 3;
	m_pDlgChart->MoveWindow(&reBottom);

	//////////////////////////////////////////////////////////////////////////
	reBottom = reClient;

	if(!m_bExpand)
	{
		reBottom.top = reBottom.top + nBasePos*4 + 17;
	}
	else
		reBottom.top -= 24;
	
	reBottom.bottom = reBottom.top + 28;

	m_rcMiddle = reBottom;
	m_rcMiddle.right += 3;
	
	reBottom.DeflateRect(3, 4);
	reLine = reBottom; 
	reRight = reBottom;

	reLine.left += 1;
	reLine.right = reLine.left + 60;
	m_btnSearch.MoveWindow(&reLine); // 검색버튼

	reLine.OffsetRect(0, 4);
	reLine.bottom = reLine.top + 16;
	reLine.left = reLine.right + 10;
	reLine.right = reLine.left + 55;
	m_ctrlCodeCntText.MoveWindow(&reLine);

	reLine.left = reLine.right + 5;
	reLine.right = reLine.left + 60;
	m_ctrlCodeCnt.MoveWindow(&reLine);

	reRight.right += 2;
	reRight.left = reRight.right - 20;
	m_btnExpand.MoveWindow(reRight);
	
	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - 60;
	m_btnResultCtrl.MoveWindow(reRight);

	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - 60;
	m_btnFieldEdit.MoveWindow(reRight);

	reRight.right = reRight.left - 2;
	reRight.left = reRight.right - 36;
	m_btnPrint.MoveWindow(reRight);

	reBottom = reClient;
	reBottom.right += 3;
	reBottom.bottom += 3;

	if(m_bExpand)
	{
		reBottom.top += 7;
	}
	else
	{
		reBottom.top = reBottom.top + nBasePos*4 + 48;
	}

	m_gridResult.MoveWindow(&reBottom);
}

void CMainFrmWnd::GetRgn_ExeptButton(CRect rect, CRgn* pRgn)
{
	CRect rectBtn;
	CRgn rgnBtn;
	pRgn->CreateRectRgnIndirect(rect);	
	
	for(int nIndex = 0; nIndex < _countof(nCtrlID); nIndex++)
	{
		GetDlgItem(nCtrlID[nIndex])->GetWindowRect(&rectBtn);
		ScreenToClient(&rectBtn);
		rgnBtn.CreateRectRgnIndirect(rectBtn);
		pRgn->CombineRgn(pRgn, &rgnBtn, RGN_DIFF);
		rgnBtn.DeleteObject();
	}
	
}

BOOL CMainFrmWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CMainFrmWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

 	CRect rcClient, rcText;
 	GetClientRect(&rcClient);

	dc.FillSolidRect(rcClient, m_clrMainSkin);
	// 2012.2.20 박성경: IBK 탭 변경으로인한 테두리 삭제 >>
// 	dc.FillSolidRect(m_rtTab, GetAxColor(66));
// 	
// 	HPEN hOldPen;
// 	hOldPen = (HPEN)dc.SelectObject(GetAxPen(GetAxColor(13), 1, PS_SOLID));
// 	
// 	MoveToEx(dc.m_hDC, m_rtTab.right, m_rtTab.top + 3, NULL);		// 오른쪽 테두리
// 	LineTo(dc.m_hDC, m_rtTab.right,  m_rtTab.bottom);
// 	
// 	MoveToEx(dc.m_hDC, m_rtTab.right, m_rtTab.bottom, NULL);	// 아래 테두리
// 	LineTo(dc.m_hDC, m_rtTab.left - 2, m_rtTab.bottom);
// 	
// 	MoveToEx(dc.m_hDC, m_rtTab.left, m_rtTab.bottom, NULL);			// 왼쪽 테두리
// 	LineTo(dc.m_hDC, m_rtTab.left, m_rtTab.top + 2);
// 	dc.SelectObject(hOldPen);
	// 2012.2.20 박성경: IBK 탭 변경으로인한 테두리 삭제 <<

// 	CRgn rgn;
// 	GetRgn_ExeptButton(rcClient, &rgn);	
//	CBrush BackBrush(m_clrMainSkin);
// 	dc.FillRgn(&rgn, &BackBrush);
// 	BackBrush.DeleteObject();
// 	rgn.DeleteObject();


	CPen *pOldPen, penGray(PS_SOLID, 1, RGB(166,166,166));
	pOldPen = dc.SelectObject(&penGray);

	CBrush*pBrush = dc.SelectObject(&m_brushBK);
	
	if(!m_bExpand)
	{
		dc.Rectangle(&m_reTopCandle);
		dc.Rectangle(&m_reTopChart);

//		CFont* pFont = dc.SelectObject(&m_fontBold);

// 		rcText = m_reTopCandle;
// 		rcText.DeflateRect(5, 2);
// 		dc.DrawText(m_strCaption, rcText, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
		m_ctrlCandleName.SetWindowText(m_strCaption);
		
		if(!m_strItemName.IsEmpty())
		{
			m_ctrlJongName.SetWindowText(m_strItemName);
//			rcText = m_reTopChart;
//			rcText.DeflateRect(5, 2);
// 			dc.DrawText(m_strItemName, rcText, DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
// 			dc.DrawText(m_strItemPrice, rcText, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);

		}

//		dc.SelectObject(pFont);
	}
	
	dc.Rectangle(&m_rcMiddle);

	if(m_btnBodyRateChk.GetCheck())
	{
		CRect rcWnd, rcArea;
		if(m_CandleMgr.m_bCreated)
		{
			ScreenToClient(&rcWnd);

			rcArea = m_rcBodyRate[0];
			rcArea.right = rcArea.left + rcArea.Width() * 5;
			dc.FillSolidRect(&rcArea, RGB(255,255,255));

			dc.SelectStockObject(BLACK_PEN);
			
			for(int i = 0; i < MAX_CANDLE; i++)
			{
				dc.Rectangle(&m_rcBodyRate[i]);
			}
		}
	}

	dc.SelectObject(pBrush);
	dc.SelectObject(pOldPen);
}

void CMainFrmWnd::SendTr_ChartData(CString strCode, CString strDate)
{
	KB_p0602_InRec1 stOutput;
	
	CTime timer;
	timer = CTime::GetCurrentTime();
	
	memset(&stOutput, 0x20, sizeof(stOutput));
	
	memset(stOutput.shcode, 0x20, sizeof(stOutput.shcode));
	memcpy(stOutput.shcode, strCode, strCode.GetLength());
	
	CString strCount;
	CString strDate_Start, strDate_End;
	strDate_Start.Format("%04d%02d%02d",timer.GetYear(), timer.GetMonth(), timer.GetDay());
	if (atoi(m_strChartTerm) > 3)
		strDate_Start = strDate_Start.Left(6) + "01";
	memcpy(stOutput.date, strDate_Start.GetBuffer(0), strDate_Start.GetLength());						// 검색일
	strDate_Start.ReleaseBuffer();
	
	memcpy(stOutput.num,		    "0080",				 4);		// 검색할 데이터 갯수s
	
	if (atoi(m_strChartTerm) == 1)
 	{
 		CString strTerm;
 		strTerm.Format("%03d", atoi(LIST_TIME[m_comboTime.GetCurSel()]));
 		memcpy(stOutput.unit, strTerm.GetBuffer(0),	strTerm.GetLength());
 		strTerm.ReleaseBuffer();
	}
	else
		memcpy(stOutput.unit,	    "001",			 3);		// 틱/분/일 조회간격단위
	
	memcpy(stOutput.button,		m_strChartTerm,		 1);	
	memcpy(stOutput.modGb,		"1",			 	 1);		// 수정주가 구분 1:수정주가반영
	memcpy(stOutput.nflag,		"0",				 1);
	memset(stOutput.nkey,		0x20,	sizeof(stOutput.nkey));
	memcpy(stOutput.nDataGubun,	"0",				 1);		// '0': 가격, '1':비율
	memcpy(stOutput.ImgRgb,		"1",				 1);
	
	memset(stOutput.Itime	 ,  0x00			,   sizeof(stOutput.Itime));
	memcpy(stOutput.Irecordgb, 	"0"	, 	1);
	memcpy(stOutput.nDayBefore, "00"	,	2);
	memcpy(stOutput.Itime	 , 	"000000"	,	6);	
	memcpy(stOutput.nowData	 ,  "0"	,	1);
	
	CString strKB0602Info;
	char* szKB0602Info = strKB0602Info.GetBuffer(sizeof(stOutput));
	memcpy(szKB0602Info, (char*)&stOutput, sizeof(stOutput));
	strKB0602Info.ReleaseBuffer();
	CString strKB0602InRec1;
	strKB0602InRec1 = strKB0602Info;
	
	CString szTrNo(QUERY_STRJONGMOK);
	
	m_pITrComm->SetBaseData("PtManager", m_szPTManager);	//For Solomon
	m_pITrComm->SetTR(szTrNo);
	m_pITrComm->Send2Server((LPVOID)(LPCTSTR)strKB0602InRec1, strKB0602InRec1.GetLength(), TRKEY_STRJONGMOK);
}

HRESULT CMainFrmWnd::StatusChange(int nState)
{
	return S_OK;
}

HRESULT CMainFrmWnd::ReceiveData(LPCSTR szTR, LPVOID aTRData, long dwTRDateLen , int nKey )
{
	HRESULT dwResult = 0;
	
	char *pBuff;
	CString strData;
	CString strTRCode(szTR);
	
	if (dwTRDateLen == 0)		//# 0 : err
	{
		m_WaitCursor.EWaitCursor();
		SetSearchResultState(0, -1);
		return S_OK;
	}
	else if (dwTRDateLen > 0)
	{
		if(!strTRCode.CompareNoCase(QUERY_STRJONGMOK_MIN) && m_strChartTerm.CompareNoCase("1") == 0)
			strTRCode = (LPCSTR)QUERY_STRJONGMOK;	//분데이터 조회시 TR번호 변경	

		//BeginWaitCursor();
		SetStateTitleBar("[조회 완료]");

		if(!strTRCode.CompareNoCase(TRNO_MSTCODE))	// 종목 마스터
		{
			// 받은 마스터 데이터의 크기가 작을 경우는 오류로 판단하고, 재조회.
			if (dwTRDateLen < MINLEN_MSTCODE_TR)
			{
				CString strFile, strDataDir;
				strDataDir.Format("%s%s\\%s", Variant(homeCC), FOLDER_FIND, gszCacheDir);
				strFile.Format("%s\\%s", strDataDir, "RequestToday.ini");
				::WritePrivateProfileString(gszDataKey_MstCode, "Today", "", strFile);
				strFile.Format("%s%s", strDataDir, "\\MstCode.dat");
				::DeleteFile(strFile);

				if (m_nPreReceiveLen == dwTRDateLen)
					AfxMessageBox(_T("\"MstCode.dat\" 파일을 정상적으로 생성할 수 없습니다.\n 화면을 다시 실행하여 주십시요."));
				else
				{
					m_nPreReceiveLen = dwTRDateLen;

					SetTrData_MSTCode();
				}
				return S_OK;
			}

			m_WaitCursor.EWaitCursor();

			m_saCodeMST.RemoveAll();

			MSTCode stMSTCode;
			int nDataCnt = dwTRDateLen / sizeof(MSTCode);
			
			CString strAddData;

			char* szTrData = new char[dwTRDateLen + 1];
			memcpy(szTrData, aTRData, dwTRDateLen);
			for (int nIndex = 0; nIndex < nDataCnt; nIndex++)
			{
				int nTest = sizeof(MSTCode);
				memset(&stMSTCode, 0x00, sizeof(MSTCode));
				memcpy(&stMSTCode, szTrData + sizeof(MSTCode) * nIndex + 4, sizeof(MSTCode));

				strAddData = "";
				STR_CONVERT(pBuff, stMSTCode.sMarket, sizeof(stMSTCode.sMarket), strData);
				strAddData = strData;

				STR_CONVERT(pBuff, stMSTCode.sCode, sizeof(stMSTCode.sCode), strData);
				strData.TrimLeft(" ");
				strData.TrimRight(" ");

				strAddData += strData;
				m_saCodeMST.Add(strAddData);
			}
    
			delete [] szTrData;
		}
		else if(!strTRCode.CompareNoCase(TRNO_CANDLE_RESULT)) // 캔들 패턴 검색 결과
		{
			SetEnable(TRUE);
			SetReceiveData_Result((char*)aTRData, dwTRDateLen);
			m_WaitCursor.EWaitCursor();

			SetResultTemp();
		}
		else if(!strTRCode.CompareNoCase(QUERY_STRJONGMOK))//차트 TR
		{
			m_pDlgChart->m_ChartItem.ClearChartData(0);
			m_WaitCursor.EWaitCursor();

			char *pszBuff=NULL;
			pszBuff = new char[dwTRDateLen+1];
			memset(pszBuff, 0x00, sizeof(pszBuff));
			memcpy(pszBuff, aTRData, dwTRDateLen);

			KB_p0602_OutRec1_EX *pStOutput = (KB_p0602_OutRec1_EX *)pszBuff;
			long lPosForChart = dwTRDateLen - (&pStOutput->fcnt[0] - &pStOutput->name[0]);
			
			if (m_pDlgChart && m_pDlgChart->m_ChartItem)
			{
				m_strItemName = CString(pStOutput->name, sizeof(pStOutput->name));
				m_strItemName.TrimLeft(); m_strItemName.TrimRight();
				m_strItemName.Replace("&", "&&");

				m_pDlgChart->m_ChartItem.RemoveRealItem();
				m_pDlgChart->m_ChartItem.SetItemCode("DEFAULT", m_strCode, m_strItemName);
				m_pDlgChart->m_ChartItem.AddPacketAndGraph();
				m_pDlgChart->m_ChartItem.SetPacketData(m_pDlgChart->m_ChartItem.GetPacketName(),(long)pStOutput->fcnt, lPosForChart,TRUE);
				m_pDlgChart->m_ChartItem.SetDrdsAdvise(FALSE);
				
// 				m_pDlgChart->m_ChartItem.AddRealItem(strCode);		// 실시간등록할 종목코드
// 				m_pDlgChart->m_ChartItem.SetDrdsAdvise(TRUE);
				
				CString strDateTime;
				//strDateTime = m_strEndTime + ":" + m_strStartTime;
				strDateTime = GetDateTime();
				BOOL bLoad = m_pDlgChart->m_ChartItem.InvokeAddInStr("ChartPeriodSummaryAddIn.dll:DRAG_CANDLE_ANAL", "0", strDateTime);

				int nData;
				char*pBuff;
				INT_CONVERT(pBuff, pStOutput->fcnt, sizeof(pStOutput->fcnt), nData);
				
				m_pDlgChart->m_ChartItem.SetOnePageDataCount(min(20, nData));
				
				m_gridResult.SetFocus();
			}

			delete []pszBuff;
			pszBuff = NULL;

			Invalidate();
		}
                else if((CString)strTRCode == TRNO_FAVORITE) //tour2k 관심종목 그룹
		{
			char* pReceiveBuff = new char[dwTRDateLen + 1];
			memset(pReceiveBuff, 0x00, dwTRDateLen + 1);
			memcpy(pReceiveBuff, aTRData, dwTRDateLen);
			if (nKey == TRKEY_GWANSIMGROUP)//관심종목
			{
				CStringArray arrName, arrKey;
				ParseGroup(pReceiveBuff, arrKey, arrName);
			}
			else if (nKey == TRKEY_GWANSIMITEM)//종목 리스트 
			{
				ParseList(pReceiveBuff, m_saCodeGroup);
			}

			delete[] pReceiveBuff; pReceiveBuff = nullptr;
		}
		else if(!strTRCode.CompareNoCase("13600"))
		{
// 						CHARVECTOR2* pData = (CHARVECTOR2*)aTRData;
// 			
// 						CString strCount, strOut;
// 						strCount.Format("%d", dwTRDateLen);
// 			
// 						m_saCodeGroup.RemoveAll();
// 			
// 						//데이터
// 						for(int i=0; i < dwTRDateLen; i++)
// 						{
// 							//strData = (*pData)[i][0]; //순번
// 							strData = (*pData)[i][1]; // 코드
// 							m_saCodeGroup.Add(strData); // 종목명
// 							//strData = (*pData)[i][2];
// 						}
// 			
// 						m_WaitCursor.EWaitCursor();
		}
	}
	return S_OK;
}

int CMainFrmWnd::ParseGroup(char* pReceiveBuff, CStringArray& arrKey, CStringArray& arrName)
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

		arrKey.Add(pSeq);
		m_GwanSimGroup.strKey.Add(pSeq);

		memset(pgame, 0x00, sizeof(stgr->gname) + 1);
		memcpy(pgame, stgr->gname, sizeof(stgr->gname));

		arrName.Add(pgame);
		m_GwanSimGroup.strGroupName.Add(pgame);

		parseL += sizeof(struct _grSearch);

	}

	delete[] pSeq; pSeq = nullptr;
	delete[] pgame; pgame = nullptr;

	return 0;
}

int CMainFrmWnd::ParseList(char* pReceiveBuff, CStringArray& arrName)
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

CString CMainFrmWnd::GetDateTime()
{
	CString strResult = "";
	strResult = m_strDateTime_Select.Left(14);
	strResult += ":";
	strResult += m_strDateTime_Select.Right(14);
	
	return strResult;
}

BOOL CMainFrmWnd::SetReceiveData_Result(LPTSTR pData, DWORD dwDataLen)
{	
	CString strData;	

	HKST03660000_OUT1 stHKST03660000_OUT1;
	memset(&stHKST03660000_OUT1, 0x00,  sizeof(HKST03660000_OUT1));
	memcpy(&stHKST03660000_OUT1, pData, sizeof(HKST03660000_OUT1));	

	CANDLE_OUT_HEAD	stCANDLE_OUT_HEAD;

	//구조체 값 세팅.
	
	strData = CString(stHKST03660000_OUT1.nTrIndex, sizeof(stHKST03660000_OUT1.nTrIndex));
	stCANDLE_OUT_HEAD.nTrIndex = atoi(strData);
	strData = CString(stHKST03660000_OUT1.lTotalJongCnt, sizeof(stHKST03660000_OUT1.lTotalJongCnt));
	stCANDLE_OUT_HEAD.lTotalCnt = atol(strData);
	strData = CString(stHKST03660000_OUT1.lOutFieldCnt, sizeof(stHKST03660000_OUT1.lOutFieldCnt));
	stCANDLE_OUT_HEAD.lOutFieldCnt = atol(strData);
	strData = CString( stHKST03660000_OUT1.lOutListPacketSize, sizeof(stHKST03660000_OUT1.lOutListPacketSize));
	stCANDLE_OUT_HEAD.lOutListPacketSize = atol(strData);
	strData = CString(stHKST03660000_OUT1.lFindTime, sizeof(stHKST03660000_OUT1.lFindTime));
	stCANDLE_OUT_HEAD.lFindTime = atol(strData);

	if (m_nTrIndex != stCANDLE_OUT_HEAD.nTrIndex)
		return FALSE;

//	m_strStartTime = CString(stHKST03660000_OUT1.sStartDateTime, sizeof(stHKST03660000_OUT1.sStartDateTime));
//	m_strEndTime = CString(stHKST03660000_OUT1.sEndDateTime, sizeof(stHKST03660000_OUT1.sEndDateTime));

	m_gridResult.ClearAll();
	m_gridResult.SetNumberRows(stCANDLE_OUT_HEAD.lTotalCnt);
	m_gridResult.m_strDate = m_strEndTime;

 	m_pDlgChart->m_ChartItem.InvokeAddInStr("ChartPeriodSummaryAddIn.dll:DRAG_CANDLE_ANAL", "0", "");
 	m_pDlgChart->m_ChartItem.ClearChartData(0);

	if (stCANDLE_OUT_HEAD.lTotalCnt == 0)
	{
		m_pDlgChart->m_ChartItem.ClearChartData(0);
		SetSearchResultState(0, -1);
		SetStateTitleBar("[조회 결과 없음.]");
		m_strItemName.Empty();
		m_strItemPrice.Empty();
		Invalidate();
		return FALSE;
	}

	int nTotalCount = stCANDLE_OUT_HEAD.lTotalCnt;

	HKST03660000_OUT2 stHKST03660000_OUT2;

	CANDLE_OUT_LIST stCANDLE_OUT_LIST;
	int nIndex, nIndex_Field;
	
	int nRow = 0;
	for (nIndex = 0; nIndex < stCANDLE_OUT_HEAD.lTotalCnt; nIndex++)
	{

		memset(&stHKST03660000_OUT2, 0x00, stCANDLE_OUT_HEAD.lOutListPacketSize);
		memcpy(&stHKST03660000_OUT2, pData + sizeof(HKST03660000_OUT1) + sizeof(HKST03660000_OUT2)*nIndex, stCANDLE_OUT_HEAD.lOutListPacketSize);

//		memset(&stCANDLE_OUT_LIST, 0x00, stCANDLE_OUT_HEAD.lOutListPacketSize);
		memcpy(&stCANDLE_OUT_LIST.sJmcode,  stHKST03660000_OUT2.sJmcode, sizeof(stHKST03660000_OUT2.sJmcode));
		memcpy(&stCANDLE_OUT_LIST.sHname,   stHKST03660000_OUT2.sHname,  sizeof(stHKST03660000_OUT2.sHname));
		memcpy(&stCANDLE_OUT_LIST.sSign,    stHKST03660000_OUT2.sSign,   sizeof(stHKST03660000_OUT2.sSign));
		memcpy(&stCANDLE_OUT_LIST.sMarket,  stHKST03660000_OUT2.sMarket, sizeof(stHKST03660000_OUT2.sMarket));
		memcpy(&stCANDLE_OUT_LIST.sS_Time,  stHKST03660000_OUT2.sS_Time, sizeof(stHKST03660000_OUT2.sJmcode));
		memcpy(&stCANDLE_OUT_LIST.sDateTime_Start,  stHKST03660000_OUT2.sStartDateTime, sizeof(stHKST03660000_OUT2.sStartDateTime));
		memcpy(&stCANDLE_OUT_LIST.sDateTime_End,  stHKST03660000_OUT2.sEndDateTime, sizeof(stHKST03660000_OUT2.sEndDateTime));


		for(nIndex_Field = 0; nIndex_Field < stCANDLE_OUT_HEAD.lOutFieldCnt; nIndex_Field++)
		{
			strData = CString(stHKST03660000_OUT2.dFieldData[nIndex_Field], sizeof(stHKST03660000_OUT2.dFieldData[nIndex_Field]));
			stCANDLE_OUT_LIST.dFieldData[nIndex_Field] = atof(strData);
		}
// 		for (nIndex_Field = 0; nIndex_Field < stCANDLE_OUT_HEAD.lOutFieldCnt; nIndex_Field++)
// 			SetByteOrderRevDouble((char far *)&(stCANDLE_OUT_LIST.dFieldData[nIndex_Field]));
		
		for(int i = 0; i < 5; i++) // 종, 전일 종, 시, 고, 저
		{
			char szTemp[16];
			memset(szTemp, 0x00, sizeof(szTemp));
			memcpy(szTemp, stHKST03660000_OUT2.dFieldData[55 + i], 16);
			m_arCandleData[i] = atoi(szTemp);
		}
		
		//# 우선주 제외
		if (m_btnExceptionChk.GetCheck() == TRUE)
		{
			strData = CString(stCANDLE_OUT_LIST.sJmcode, sizeof(stCANDLE_OUT_LIST.sJmcode));
			strData.TrimLeft(" ");
			strData.TrimRight(" ");

			if (atoi(strData.Right(1)) > 0)
			{
				nTotalCount--; // 우선주 제외시 검색결과 수량 조정
				continue;
			}
		}

		// 그리드의 삭제버튼으로 제외된 종목
		if(!m_saDelItemCode.GetSize())
		{
			BOOL bChk = FALSE;
			strData = CString(stCANDLE_OUT_LIST.sJmcode, sizeof(stCANDLE_OUT_LIST.sJmcode));
			strData.TrimLeft(" ");
			strData.TrimRight(" ");
			for(int i = 0; i < m_saDelItemCode.GetSize(); i++)
			{
				if(m_saDelItemCode.GetAt(i) == strData)
				{
					bChk = TRUE;
					break;
				}
			}
			
			if(bChk)
			{
				nTotalCount--;
				continue;
			}

		}

	
		m_gridResult.SetAddRow(nRow, stCANDLE_OUT_HEAD.lOutFieldCnt, stCANDLE_OUT_LIST, m_arCandleData);
		nRow++;
	}

	SetSearchResultState(nTotalCount, stCANDLE_OUT_HEAD.lFindTime);

	if (nTotalCount > 0)
		m_gridResult.SetSelectRow(0);

	m_gridResult.SetNumberRows(nRow);
	CUGHScroll* pGridHScroll = m_gridResult.m_CUGHScroll;
	pGridHScroll->SetScrollPos(0, FALSE);
	m_gridResult.RedrawAll();

	return TRUE;
}


void CMainFrmWnd::SetSearchResultState(LONG lCnt/* = 0*/, LONG lTime/* = -1*/)
{
	CString strResult;
	CString strTime, strTemp;
	strResult.Format(" %d건", lCnt);
	
	/*
	if (lCnt == 0)
	m_gridResult.ClearData(TRUE);
	*/
	
	if (lTime == -1)
	{
		m_ctrlCodeCnt.SetWindowText("0");		
		strTime = "";

		CTime timer;
		timer = CTime::GetCurrentTime();
	
		strTime.Format("%02d:%02d:%02d", timer.GetHour(), timer.GetMinute(), timer.GetSecond());		
	}
	else
	{
		int nLength;
		strTemp.Format("%06d", lTime);
		nLength = strTemp.GetLength();
		
		if (nLength == 3)
		{
			strTime = "0" + strTemp.Left(1) + ":";
			strTime += strTemp.Right(2);
		}
		else if (nLength == 4)
		{
			strTime = strTemp.Left(2) + ":";
			strTime += strTemp.Right(2);
		}
		else if (nLength == 6)
		{
			strTime = strTemp.Left(2) + ":";
			strTime += strTemp.Mid(2, 2) + ":";
			strTime += strTemp.Right(2);
		}
	}
	m_ctrlCodeCntText.ShowWindow(SW_SHOW);
	m_ctrlCodeCnt.SetWindowText(strResult);
	m_ctrlTime.SetWindowText("검색시간:" + strTime);
	SetStateTitleBar("조회완료");
	
}

void CMainFrmWnd::SetByteOrderRevDouble(LPSTR pDouble)
{
	int nCount, nLength = 4;
	
	for (nCount = 0; nCount < nLength; nCount++)
	{
		unsigned char chTemp;
		chTemp = *(pDouble + nCount);
		*(pDouble + nCount) = *(pDouble + sizeof(double) - 1 - nCount);
		*(pDouble + sizeof(double) - 1 - nCount) = chTemp;
	}
}

HBRUSH CMainFrmWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFrameWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC || nCtlColor ==CTLCOLOR_BTN)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(m_clrMainSkin);
		//2009.10.08 by LYH >> window7 테마 지원(고대비검정) <<
		pDC->SetTextColor(RGB(0,0,0));
		//	pDC->SetBkColor(COLOR_MAIN_CANVAS);		
		//hbr = m_brushBK;
		hbr = GetAxBrush(m_clrMainSkin);
	}	
	return hbr;
}

BOOL CMainFrmWnd::SetLoad_TargetBase_2nd(CString strTargetBaseType)
{
	BOOL bResult = TRUE;

	//# 0 : 전체, 1:Kospi, 2:Kosdak

	int nTargetBaseType_0st, nTargetBaseType_1st;
	GetTargetBaseType(nTargetBaseType_0st, nTargetBaseType_1st);

	if (nTargetBaseType_0st == 1)// kospi
		SetLoad_CUMaster();
	else if (nTargetBaseType_0st == 2)// kosdaq
		SetLoad_CKUMaster();
// 	else if (nTargetBaseType_0st == 3)	//# 테마
// 		SetLoad_CTHMaster();
	else if (nTargetBaseType_0st == 3)		//# 관심
		SetLoad_PortGroup();

	return bResult;
}

int CMainFrmWnd::SetLoad_CUMaster()
{
	CStringArray* arCode;
	CStringArray* arName;
	arCode = new CStringArray;
	arName = new CStringArray;
	int nIndex = 0;
	
	m_comboTarget_2nd.ResetContent();

	CList<CUMaster, CUMaster&> list;
	
	GetUpMasterData(arCode, arName, GETMARKET_KOSPI);
	for(nIndex=0; nIndex<arCode->GetSize(); nIndex++)
	{
		CUMaster	clsUMaster;
		clsUMaster.m_szCode = arCode->GetAt(nIndex);
		clsUMaster.m_szName = arName->GetAt(nIndex);
		clsUMaster.m_szName.TrimRight();
		clsUMaster.m_szGubun = _T("");
		list.AddTail(clsUMaster);
	}
	arCode->RemoveAll();
	arName->RemoveAll();

	GetUpMasterData(arCode, arName, GETMARKET_KOSPI200);
	for(nIndex=0; nIndex<arCode->GetSize(); nIndex++)
	{
		if(arCode->GetAt(nIndex) == "1")
		{
			CUMaster	clsUMaster;
			clsUMaster.m_szCode = arCode->GetAt(nIndex);
			clsUMaster.m_szName = arName->GetAt(nIndex);
			clsUMaster.m_szName.TrimRight();
			clsUMaster.m_szGubun = _T("");
			list.AddTail(clsUMaster);
		}
	}	
	delete arCode;
	delete arName;
		
	int nCode;
	char* pBuff;
	char aKeyB[5+1];
	for (POSITION pos=list.GetHeadPosition(); pos;)
	{
		CUMaster data = list.GetNext(pos);
		sprintf(aKeyB, "B%04.4s", data.m_szCode);// data.m_szCode.Mid(0));
		if(strcmp(aKeyB, "B1001")==0) 
			continue;
		
		nIndex = m_comboTarget_2nd.AddString(data.m_szName);
		INT_CONVERT(pBuff, data.m_szCode, sizeof(data.m_szCode), nCode);
		m_comboTarget_2nd.SetItemData(nIndex, (DWORD)nCode);
	//	m_comboTarget_2nd.SetItemData(nIndex, (DWORD&)data.m_szCode); 
	}
	
	m_comboTarget_2nd.SetCurSel(0);	

	return m_comboTarget_2nd.GetCount();
}

int CMainFrmWnd::SetLoad_CKUMaster()
{
	CStringArray* arCode;
	CStringArray* arName;
	arCode = new CStringArray;
	arName = new CStringArray;
	int nIndex = 0;
	
	m_comboTarget_2nd.ResetContent();
	
	CList<CUMaster, CUMaster&> list;
	
	GetUpMasterData(arCode, arName, GETMARKET_KOSDAQ);
	for(nIndex=0; nIndex<arCode->GetSize(); nIndex++)
	{
		CUMaster	clsUMaster;
		clsUMaster.m_szCode = arCode->GetAt(nIndex);
		clsUMaster.m_szName = arName->GetAt(nIndex);
		clsUMaster.m_szName.TrimRight();
		clsUMaster.m_szGubun = _T("");
		list.AddTail(clsUMaster);
	}		
	delete arCode;
	delete arName;
	
	int nCode;
	char* pBuff;
	char aKeyB[5+1];
	for (POSITION pos=list.GetHeadPosition(); pos;)
	{
		CUMaster data = list.GetNext(pos);
		sprintf(aKeyB, "B%04.4s", data.m_szCode);// data.m_szCode.Mid(0));
		if(strcmp(aKeyB, "B2001")==0) 
			continue;
		
		nIndex = m_comboTarget_2nd.AddString(data.m_szName);
		INT_CONVERT(pBuff, data.m_szCode, sizeof(data.m_szCode), nCode);
		m_comboTarget_2nd.SetItemData(nIndex, (DWORD)nCode);
		//	m_comboTarget_2nd.SetItemData(nIndex, (DWORD&)data.m_szCode); 
	}
	
	m_comboTarget_2nd.SetCurSel(0);	

	return m_comboTarget_2nd.GetCount();
}

BOOL CMainFrmWnd::GetData_UpJongToCode(UINT nCode, CString strGroupName)
{
	CStringArray arrCode, arrName;

	if(!strGroupName.CompareNoCase("KOSPI200") && nCode == 1)
		GetStockMasterData(arrCode, arrName, GETMARKET_KOSPI200, nCode);
	else
		GetStockMasterData(arrCode, arrName, m_nMarket, nCode);

	m_saCodeGroup.RemoveAll();
	for(int i=0; i < arrCode.GetSize(); i++)
	{
		m_saCodeGroup.Add(arrCode.GetAt(i));
	}

//20100217 SJ_KIM
// 	CString strTR, strInputData;
// 	
// 	if(m_nMarket == 1)  //코스피
// 	{
// 		strTR = "13600";
// 		strInputData.Format("A1100B%04d", atoi(strCode));
// 	}
// 	else if(m_nMarket == 2) //코스닥
// 	{
// 		strTR = "13600";
// 		strInputData.Format("A2100B%04d", atoi(strCode));
// 	}
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	TR_88803_I inData;
// 	MakeSpaceTR_88803_I(inData);
// 	memcpy(inData.reserved, "        ", 8);
// 	memcpy(inData.mainkey,strInputData, 10);
// 	inData.sort[0] = '1';
// 	inData.datatype[0]= 'B';
// 
// 	int nLen = TR_88803_I_SIZE;
// 	char* szData = (char*)&inData;
// 	m_pTRComm->SetTR(strTR);
// 
// 	strCode.Format("%5.5s", inData.subkey);
// 
// 	CString strQuery="";
// 	CString strKeyList = "";
// 	CString strSaveBuf = "";
// 	CStringArray arFidList;
// 		
// 	m_pTRComm->FidQueryKeyPartMake ( strKeyList, "3", (LPSTR)(LPCSTR)strCode.Mid(2));   // 종목코드FID
// 	m_pTRComm->FidQueryKeyPartMake ( strKeyList, "9104", "U");		// 시장구분FID
// 	m_pTRComm->FidQueryKeyPartMake ( strKeyList, "9103", "4");		// 시장구분FID
// 	if(strCode.GetLength()>2)
// 	{
// 		if( strCode.GetAt(2) == '0')
// 		{
// 			m_pTRComm->FidQueryKeyPartMake ( strKeyList, "242", "2");		// 거래소
// 			m_pTRComm->FidQueryKeyPartMake ( strKeyList, "9210", "0534");	// 시장구분FID
// 			m_pTRComm->FidQueryKeyPartMake ( strKeyList, "GID", "3053");	// 체결GID
// 		}
// 		else if( strCode.GetAt(2) == '1')
// 		{
// 			m_pTRComm->FidQueryKeyPartMake ( strKeyList, "242", "3");		// KOSPI200
// 			m_pTRComm->FidQueryKeyPartMake ( strKeyList, "9221", "1");		// 시장구분FID
// 			m_pTRComm->FidQueryKeyPartMake ( strKeyList, "9210", "0535");	// 시장구분FID
// 			m_pTRComm->FidQueryKeyPartMake ( strKeyList, "GID", "1007");	// 체결GID
// 		}
// 		else if( strCode.GetAt(2) == '2')
// 		{
// 			m_pTRComm->FidQueryKeyPartMake ( strKeyList, "242", "4");		// 코스닥
// 			m_pTRComm->FidQueryKeyPartMake ( strKeyList, "9210", "0534");	// 시장구분FID
// 			m_pTRComm->FidQueryKeyPartMake ( strKeyList, "GID", "3053");	// 체결GID
// 		}
// 		else if( strCode.GetAt(2) == '3')
// 		{
// 			m_pTRComm->FidQueryKeyPartMake ( strKeyList, "242", "5");		// 코스닥50
// 			m_pTRComm->FidQueryKeyPartMake ( strKeyList, "9221", "1");		// 시장구분FID
// 			m_pTRComm->FidQueryKeyPartMake ( strKeyList, "9210", "0535");	// 시장구분FID
// 			m_pTRComm->FidQueryKeyPartMake ( strKeyList, "GID", "1007");	// 체결GID
// 		}
// 		else if( strCode.GetAt(2) == '4')
// 		{
// 			m_pTRComm->FidQueryKeyPartMake ( strKeyList, "242", "10");		// KRX
// 			m_pTRComm->FidQueryKeyPartMake ( strKeyList, "9210", "0534");	// 시장구분FID
// 			m_pTRComm->FidQueryKeyPartMake ( strKeyList, "GID", "3053");	// 체결GID
// 		}
// 	}
// 
// 	arFidList.Add ( "902");							// 순위번호
// 	arFidList.Add ( "3");							// 종목명
// 	arFidList.Add ( "1");							// 종목코드 
// 
// 	int nQueryCnt = MAX_MASTERQUERY_CNT;
// 	nLen = m_pTRComm->FidArrayQueryMake ( strQuery, strKeyList,  strSaveBuf , arFidList, 0, nQueryCnt);
// 
// 	m_WaitCursor.BWaitCursorIntelli();
// 
// 	//화면번호를 세팅한다.
// 	m_pTRComm->SetBaseData("SCREENCODE", m_strScreenNo);
// 	m_pTRComm->Send2Server(szData, nLen);

	return 0;
}

int CMainFrmWnd::SetLoad_CTHMaster()
{
	m_comboTarget_2nd.ResetContent();
	
	int nIndex;
	CString strOut, strData, strName;
	
	for(int nThemeIndex = 0; nThemeIndex < m_saCodeTheme.GetSize(); nThemeIndex++)
	{
		strOut = m_saCodeTheme.GetAt(nThemeIndex);
		strName = strOut.Mid(4, strOut.GetLength() - 4);
		strName.TrimRight();
		strData = strOut.Mid(0, 4);
		
		char strCode[5+1]={0,};
		memset(strCode, 0x00, 5);
		memcpy(strCode, (LPCTSTR)strData, 5);
		
		nIndex = m_comboTarget_2nd.AddString(strName);
		m_comboTarget_2nd.SetItemData(nIndex, (DWORD)atoi(strCode));
	}
	
	m_comboTarget_2nd.SetCurSel(0);
	
	return m_comboTarget_2nd.GetCount();
}

void CMainFrmWnd::GetData_ThemaToCode(CString strThemeCode)
{
	m_saCodeGroup.RemoveAll();

	CStringArray arrCode, arrName;
	CString strCode;
	
	int nIndex;
	for(nIndex = 0; nIndex < arrCode.GetSize(); nIndex++)
	{
		strCode.Format("%6.6s", arrCode.GetAt(nIndex));
		m_saCodeGroup.Add(strCode);	
	}

// 	m_saCodeGroup.RemoveAll();
// 
// 	BYTE*	lpInputData;
// 	lpInputData = (BYTE*)calloc(sizeof(fn_HFIBOA076210_InRec1), sizeof(BYTE));
// 	memset(lpInputData, ' ', sizeof(fn_HFIBOA076210_InRec1));
// 
// 	fn_HFIBOA076210_InRec1	lpInput;
// 	memset(&lpInput, ' ', sizeof(fn_HFIBOA076210_InRec1));
// 
// 	CString strTemp;
// 	strTemp = "30";
// 	memcpy(lpInput.InItem1,	 (LPCSTR)strTemp, strTemp.GetLength() );
// 
// 	memcpy( lpInput.ThemaCd, (LPCSTR)strThemeCode, strThemeCode.GetLength() );
// 
// 	m_pTRComm->SetDestination('X');
// 	m_pTRComm->SetTR("HFIBOA076210");
// 
// 	m_WaitCursor.BWaitCursorIntelli();
// 
// 	//화면번호를 세팅한다.
//  	m_pTRComm->SetBaseData("SCREENCODE", m_strScreenNo);
// 	m_pTRComm->Send2Server(&lpInput, sizeof(fn_HFIBOA076210_InRec1), FALSE);
}

int	CMainFrmWnd::SetLoad_PortGroup()
{
	m_comboTarget_2nd.ResetContent();

	int nType = 0;
	LPCSTR szGroup=NULL;
	CStringArray arrKey, arrName;
	
	GetData_PortGroup(arrKey, arrName);

	int nCnt = arrName.GetSize();
	int nIdx;
	for(int i=0; i<nCnt; i++)
	{
		nIdx = m_comboTarget_2nd.AddString(arrName.GetAt(i));
		m_comboTarget_2nd.SetItemData(nIdx, atoi(arrKey.GetAt(i)));
	}
	if(nCnt>0)
		m_comboTarget_2nd.SetCurSel(0);

	return nCnt;

//20100217 SJ_KIM
// 	m_comboTarget_2nd.ResetContent();
// 	
// 	IAUGwanManager* pGwanMng = (IAUGwanManager*)m_pIGateManager->GetInterface(UUID_IAUGwanManager);
// 	if(!pGwanMng)	return 0;
// 	
// 	int nType = 0;
// 	LPCSTR szGroup=NULL;
// 	CStringArray arrKey, arrName;
// 	pGwanMng->GetGwansimGroupName(arrName);
// 
// 	int nCnt = arrName.GetSize();
// 	int nIdx;
// 	for(int i=0; i<nCnt; i++)
// 	{
// 		nIdx = m_comboTarget_2nd.AddString(arrName.GetAt(i));
// 		m_comboTarget_2nd.SetItemData(nIdx, nIdx);//atol(arrName.GetAt(i)));
// 	}
// 	if(nCnt>0)
// 		m_comboTarget_2nd.SetCurSel(0);
//	
//	return nCnt;
	return 0;
}

void CMainFrmWnd::GetData_PortGroupToCode(CString szGroupKey, CStringArray *psaData)
{
	int nTargetBaseType_0st, nTargetBaseType_1st;
	GetTargetBaseType(nTargetBaseType_0st, nTargetBaseType_1st);

	psaData->RemoveAll();

	CStringArray strItemArray, strNameArray;
	
	GetData_PortItem(strItemArray, strNameArray, atoi(szGroupKey));

	CString strItem, strCode;
	int nSize = strItemArray.GetSize();
	for(int nIndex = 0; nIndex < nSize; nIndex++)
	{
		strItem = strItemArray.GetAt(nIndex);
		psaData->Add(strItem);
	}
}

BOOL CMainFrmWnd::GetLoadXml(CString strFileRoot, MSXML::IXMLDOMDocumentPtr &pXmlDom)
{
	char msg[500];
	CString strDebug;
	
	pXmlDom->async = false;
	if (!pXmlDom->load (strFileRoot.AllocSysString())) 
	{	
		MSXML::IXMLDOMParseErrorPtr pParsingErr = pXmlDom->parseError;			
		sprintf(msg, "Error 0x%.8X on line %d, position %d\r\n", 
			pParsingErr->errorCode,pParsingErr->line, pParsingErr->linepos);
		
		CString strTemp(BSTR(pParsingErr->reason));	
		strDebug = strTemp + CString(msg) + "\n" + strFileRoot;
		
		//SetMessageBox(strDebug, "[XML Files Error]", MB_OK);		
		return FALSE;
	}
	
	return TRUE;
}

void CMainFrmWnd::SetLoad_FieldInfo()
{
	MSXML::IXMLDOMDocumentPtr pXmlDom ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	
	CString strFileRoot;
	strFileRoot = m_strAppPath + FOLDER_FIND + "\\" + FIXFIELD_SYS_FILE;
	
	if (GetLoadXml(strFileRoot, pXmlDom) == TRUE)
	{
		GetLoadFixSysFieldData(pXmlDom->selectSingleNode("INDEX"), m_listFixHeadSys);
		m_strFixFieldVersion = GetVersion_FieldFile(pXmlDom->selectSingleNode("INDEX"));
	}
	
	BOOL bSetInit = TRUE;
	strFileRoot = m_strAppPath + USRDIR + Variant(nameCC) + FOLDER_FINDDATA + "\\" + FIXFIELD_USER_CANDLE_FILE;
	if (GetLoadXml(strFileRoot, pXmlDom) == TRUE)
	{
		//# Fix Field User Version Check!
		if (m_strFixFieldVersion == GetVersion_FieldFile(pXmlDom->selectSingleNode("INDEX")))
		{
			GetLoadFixUserFieldData(pXmlDom->selectSingleNode("INDEX"), m_listFixHeadUser);
			
			if (m_listFixHeadUser.GetCount() == 0)
				bSetInit = FALSE;
			else
				SetUpdateUserFieldData(&m_listFixHeadSys, &m_listFixHeadUser);		//# Info UpDate!
			
			bSetInit = FALSE;
		}
	}
	
	if (bSetInit == TRUE)
	{
		m_nFixCol = 2;
		m_listFixHeadUser.RemoveAll();
		
		CStringArray saFieldId;
		if (GetLoadFixField_Def(saFieldId) == TRUE)
		{
			POSITION pos;
			HeadFixInfo		stHeadFixInfo;
			HeadInfoType	stHeadInfoType;
			for (int nIndex = 0; nIndex < m_listFixHeadSys.GetCount(); nIndex++)
			{
				pos = m_listFixHeadSys.FindIndex(nIndex);
				stHeadInfoType = m_listFixHeadSys.GetAt(pos);
				
				CString strFieldId;
				for (int nItem = 0; nItem < saFieldId.GetSize(); nItem++)
				{
					strFieldId = saFieldId.GetAt(nItem);
					if (stHeadInfoType.lHeadID == atol(strFieldId))
					{
						stHeadFixInfo.stHeadType = stHeadInfoType;
						stHeadFixInfo.nColNum = m_listFixHeadUser.GetCount();
						stHeadFixInfo.bView = TRUE;
						
						m_listFixHeadUser.AddTail(stHeadFixInfo);
						break;
					}
				}
			}
		}
	}
	
	SetSaveFixUserFieldData(&m_listFixHeadUser, m_strFixFieldVersion);
}

void CMainFrmWnd::SetGrid_Head(BOOL bView /*= FALSE*/)
{
	int nCol;
	POSITION pos;
	
	if (bView == TRUE)
	{
		m_gridResult.SetHeader(&m_listField);
		m_gridResult.m_plistField = &m_listField;
		
		m_gridResult.SetLockColumns(m_nFixCol, TRUE);	
	}
	else if (bView == FALSE)
	{
		m_listField.RemoveAll();
		
		HeadFixInfo	stHeadFixInfo;
		for (nCol = 0; nCol < m_listFixHeadUser.GetCount(); nCol++)
		{
			pos = m_listFixHeadUser.FindIndex(nCol);
			stHeadFixInfo = m_listFixHeadUser.GetAt(pos);
			
			if (stHeadFixInfo.bView == TRUE)
				m_listField.AddTail(stHeadFixInfo.stHeadType);
		}
	}
}

BOOL CMainFrmWnd::GetLoadSet_Def()
{
	CFileFind cFind;
	BOOL bRead = TRUE;
	
	m_nDefult_TargetBaseType = 0;
	m_nDefult_ChartTerm = 2;
	m_nDefult_ChartTermMin = 0;
	m_nDefult_Tail = 0;
	m_nDefult_AllColorChk = 0;
	m_nDefult_PreTrend = 0;
	m_nDefult_ExceptPreferred = 1;
	
	CString strFileRoot = m_strAppPath + FOLDER_DATA + "\\" + CANDLEINFO_INIFILE;
	bRead = cFind.FindFile(strFileRoot);
	if (!bRead)
	{
		CString strMsg = (CString)CANDLEINFO_INIFILE + "을 찾지 못했습니다.";
		SetStateTitleBar(strMsg);
	}
	else
	{
		CString strKey;
		strKey = "TargetBaseType";
		m_nDefult_TargetBaseType = GetPrivateProfileInt("DefaultSet", strKey, 0, strFileRoot);

		strKey = "ChartTerm"; // 분:1 일:2 주:3 월:4
		m_nDefult_ChartTerm = GetPrivateProfileInt("DefaultSet", strKey, 2, strFileRoot);

		strKey = "ChartTermMin"; //분선택(1분:0, 3분:1, 5분:2, 10분:3, 15분:4, 30분:5, 60분:6)
		m_nDefult_ChartTermMin = GetPrivateProfileInt("DefaultSet", strKey, 0, strFileRoot);

		strKey = "BongPassCnt";
		m_nDefult_BongPass = GetPrivateProfileInt("DefaultSet", strKey, 0, strFileRoot);

		strKey = "ExceptPreferred";
		m_nDefult_ExceptPreferred = GetPrivateProfileInt("DefaultSet", strKey, 1, strFileRoot);

		strKey = "IgnoreColor";
		m_nDefult_AllColorChk = GetPrivateProfileInt("DefaultSet", strKey, 0, strFileRoot);

		strKey = "IgnoreTail";
		m_nDefult_Tail = GetPrivateProfileInt("DefaultSet", strKey, 0, strFileRoot);

		strKey = "PreTrend";
		m_nDefult_PreTrend = GetPrivateProfileInt("DefaultSet", strKey, 0, strFileRoot);

		if(m_nDefult_TargetBaseType < 0 || m_nDefult_TargetBaseType > 4) m_nDefult_TargetBaseType = 0;
		if(m_nDefult_ChartTerm < 1 || m_nDefult_ChartTerm > 4) m_nDefult_ChartTerm = 2;
		if(m_nDefult_ExceptPreferred < 0 || m_nDefult_ExceptPreferred > 1) m_nDefult_ExceptPreferred = 0;
		if(m_nDefult_ChartTermMin < 0 || m_nDefult_ChartTermMin > 6) m_nDefult_ChartTermMin = 0;
		if(m_nDefult_BongPass < 0 || m_nDefult_BongPass > 99) m_nDefult_BongPass = 1;
		if(m_nDefult_AllColorChk < 0 || m_nDefult_AllColorChk > 1) m_nDefult_AllColorChk = 0;
		if(m_nDefult_Tail < 0 || m_nDefult_Tail > 2) m_nDefult_Tail = 0;
		if(m_nDefult_PreTrend < 0 || m_nDefult_PreTrend > 2) m_nDefult_PreTrend = 0;
	}
	
	return bRead;
}

BOOL CMainFrmWnd::GetLoadFixField_Def(CStringArray &saFieldId)
{
	CFileFind cFind;
	BOOL bRead = TRUE;

	CString strFilePath = GetFilePath("FINDERINFO");

	bRead = cFind.FindFile(strFilePath);
	if (!bRead)
	{
		CString strMsg = (CString)FINDERINFO_INIFILE + "을 찾지 못했습니다.";
		SetSendMainMsg(strMsg);
	}
	else
	{
		char chColID[10];
		memset(chColID, ' ', sizeof(chColID));

		int nCount = GetPrivateProfileInt("DefaultColumn", "Line Count", 0, strFilePath);
		CString strTemp;
		for (int nIndex = 0; nIndex < nCount; nIndex++)
		{
			strTemp.Format("%d", nIndex + 1);
			GetPrivateProfileString("DefaultColumn", strTemp, "", chColID, sizeof(chColID), strFilePath);
			saFieldId.Add(chColID);
		}
	}

	return bRead;
}

CString CMainFrmWnd::GetVersion_FieldFile(MSXML::IXMLDOMNodePtr pNIndex)
{
	CString strResult = XML_VERSION_FIXFIELDFILE;
	
	MSXML::IXMLDOMNamedNodeMapPtr	pNAIndex;
	try 
	{
		MSXML::IXMLDOMNodePtr	pNAttributeTemp;
		pNAIndex = pNIndex->attributes;
		
		pNAttributeTemp = pNAIndex->getNamedItem("VERSION");
		strResult = CString((PCHAR)pNAttributeTemp->text);
	}
	catch(_com_error e) 
	{
		strResult = "";
	}
	
	return strResult;
}

BOOL CMainFrmWnd::SetSaveFixUserFieldData(CList<HeadFixInfo, HeadFixInfo&> *plistFixUser,
										  CString strVersion)
{
	BOOL bResult = TRUE;
	
	CString strFileRoot = m_strAppPath + USRDIR + Variant(nameCC) + FOLDER_FINDDATA + "\\" + FIXFIELD_USER_CANDLE_FILE;
	
	MSXML::IXMLDOMNodePtr						pNIndex;
	MSXML::IXMLDOMNamedNodeMapPtr		pNAIndex;
	
	MSXML::IXMLDOMNodePtr						pN_FItem;
	MSXML::IXMLDOMNamedNodeMapPtr		pNA_FItem;
	
	try 
	{
		CString strTemp;
		
		MSXML::IXMLDOMDocumentPtr pDOMIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
		
		MSXML::IXMLDOMProcessingInstructionPtr pPI;
		MSXML::IXMLDOMNamedNodeMapPtr					pNAPI;
		MSXML::IXMLDOMNodePtr									pNEncoding;
		
		pPI = pDOMIndex->createProcessingInstruction("xml", "version=\"1.0\"");
		
		pNEncoding = pDOMIndex->createAttribute("encoding");
		pNEncoding->text = "KSC_5601";
		
		pNAPI = pPI->attributes;
		pNAPI->setNamedItem(pNEncoding);
		
		pDOMIndex->appendChild(pPI);
		
		pNIndex = pDOMIndex->createElement("INDEX");
		pNAIndex = pNIndex->attributes;
		
		MSXML::IXMLDOMNodePtr pNAttributeTemp;
		pNAttributeTemp = pDOMIndex->createAttribute("VERSION");
		pNAttributeTemp->text = (LPCTSTR)strVersion;
		pNAIndex->setNamedItem(pNAttributeTemp);
		
		pNAttributeTemp = pDOMIndex->createAttribute("FIX_COL");
		strTemp.Format("%d", m_nFixCol);
		pNAttributeTemp->text = (LPCTSTR)strTemp;
		pNAIndex->setNamedItem(pNAttributeTemp);
		
		pDOMIndex->appendChild(pNIndex);
		
		POSITION pos;
		HeadFixInfo	stHeadFixInfo;
		int nFieldCount = plistFixUser->GetCount();
		for (int nIndex = 0;  nIndex < nFieldCount; nIndex++)	
		{
			pos = plistFixUser->FindIndex(nIndex);
			stHeadFixInfo = plistFixUser->GetAt(pos);
			
			MSXML::IXMLDOMNodePtr pNAttributeTemp;
			
			pN_FItem = pDOMIndex->createElement("FITEM");
			pNIndex->appendChild(pN_FItem);
			pNA_FItem = pN_FItem->attributes;
			
			//ID
			pNAttributeTemp = pDOMIndex->createAttribute("ID");
			strTemp.Format("%d", stHeadFixInfo.stHeadType.lHeadID);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//COL_NO
			pNAttributeTemp = pDOMIndex->createAttribute("COL_NO");
			strTemp.Format("%d", stHeadFixInfo.nColNum);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//FIELD_NAME
			pNAttributeTemp = pDOMIndex->createAttribute("FIELD_NAME");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strHeadName;;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//UNIT
			pNAttributeTemp = pDOMIndex->createAttribute("UNIT");
			pNAttributeTemp->text = (LPCTSTR)"1";
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//ALIGN
			pNAttributeTemp = pDOMIndex->createAttribute("ALIGN");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strAlignment;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//COL_WIDTH
			pNAttributeTemp = pDOMIndex->createAttribute("COL_WIDTH");
			strTemp.Format("%d", stHeadFixInfo.stHeadType.nColWidth);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//DATA_TYPE
			pNAttributeTemp = pDOMIndex->createAttribute("DATA_TYPE");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strDataType;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//VIEW_NAME
			pNAttributeTemp = pDOMIndex->createAttribute("VIEW_NAME");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strHeadName;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//VISIBLE
			pNAttributeTemp = pDOMIndex->createAttribute("VISIBLE");
			strTemp = (stHeadFixInfo.bView == TRUE)?"TRUE":"FALSE";
			pNAttributeTemp->text =	(LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//UNIT_NUM
			pNAttributeTemp = pDOMIndex->createAttribute("UNIT_NUM");
			strTemp.Format("%d", stHeadFixInfo.stHeadType.nUnitNum);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//UNIT_TEXT
			pNAttributeTemp = pDOMIndex->createAttribute("UNIT_TEXT");
			pNAttributeTemp->text = (LPCTSTR)stHeadFixInfo.stHeadType.strUnitText;
			pNA_FItem->setNamedItem(pNAttributeTemp);
			
			//DATA_LENGTH
			pNAttributeTemp = pDOMIndex->createAttribute("DATA_LENGTH");
			strTemp.Format("%d", stHeadFixInfo.stHeadType.nDataLength);
			pNAttributeTemp->text = (LPCTSTR)strTemp;
			pNA_FItem->setNamedItem(pNAttributeTemp);
		}
		
		pDOMIndex->save(_bstr_t(strFileRoot));
	}	
	catch(_com_error e)
	{
		bResult = FALSE;
	}
	
	return bResult;
}

void CMainFrmWnd::SetUpdateUserFieldData(CList<HeadInfoType, HeadInfoType&> *plistFixSys, 
										 CList<HeadFixInfo, HeadFixInfo&> *plistFixUser)
{
	POSITION	posSys, posUser;
	HeadInfoType stHeadInfoType;
	HeadFixInfo	 stHeadFixInfo;
	
	int nSysCnt = plistFixSys->GetCount();
	int nUserCnt = plistFixUser->GetCount();
	for (int nSysIndex = 0; nSysIndex < nSysCnt; nSysIndex++)
	{
		posSys = plistFixSys->FindIndex(nSysIndex);
		stHeadInfoType = plistFixSys->GetAt(posSys);
		
		for (int nUserIndex = 0; nUserIndex < nUserCnt; nUserIndex++)
		{
			posUser = plistFixUser->FindIndex(nUserIndex);
			stHeadFixInfo = plistFixUser->GetAt(posUser);
			
			if (stHeadInfoType.lHeadID == stHeadFixInfo.stHeadType.lHeadID)
			{
				if (stHeadFixInfo.stHeadType.strHeadName.Compare(stHeadInfoType.strHeadName) == -1)
					stHeadFixInfo.stHeadType.strHeadName = stHeadInfoType.strHeadName;
				
				stHeadFixInfo.stHeadType.strDataType = stHeadInfoType.strDataType;
				stHeadFixInfo.stHeadType.nUnitNum	= stHeadInfoType.nUnitNum;
				stHeadFixInfo.stHeadType.strUnitText = stHeadInfoType.strUnitText;
				stHeadFixInfo.stHeadType.nDataLength = stHeadInfoType.nDataLength;
				stHeadFixInfo.stHeadType.lHeadID = stHeadInfoType.lHeadID;
				stHeadFixInfo.stHeadType.strAlignment	= stHeadInfoType.strAlignment;
				stHeadFixInfo.stHeadType.nColWidth = stHeadInfoType.nColWidth;
				//				//stHeadFixInfo.strUNIT						= stHeadInfoType.strUNIT;
				//				//stHeadFixInfo.strVISIBLE					= stHeadInfoType.strVISIBLE;
				
				plistFixUser->SetAt(posUser, stHeadFixInfo);
			}
		}
	}
}

BOOL CMainFrmWnd::GetLoadFixSysFieldData(MSXML::IXMLDOMNodePtr pNIndex,
										 CList<HeadInfoType, HeadInfoType&> &listFixList)
{
	BOOL bResult = TRUE;
	
	MSXML::IXMLDOMNodeListPtr		pNS_FItem;	// Item List
	MSXML::IXMLDOMNodePtr				pN_FItem;	// Item
	MSXML::IXMLDOMNamedNodeMapPtr	pNA_FItem;	// Item Attribute 	
	
	try
	{
		pNS_FItem  = pNIndex->selectNodes("FITEM");
		
		//System의 version은 읽어올 필요없다
		//해당 모듈이 로드되기 전에 이미 version처리 되어진다.
		int nCount, nLength;
		nLength = pNS_FItem->length;
		for (nCount = 0; nCount < nLength; nCount++)
		{
			HeadInfoType stHeadInfoType;
			
			//node를 얻어온다.
			pN_FItem = pNS_FItem->item[nCount];	
			
			//속성을 얻어온다.
			MSXML::IXMLDOMNodePtr	pNAttributeTemp;
			pNA_FItem= pN_FItem->attributes;
			
			//ID
			pNAttributeTemp = pNA_FItem->getNamedItem("ID");
			stHeadInfoType.lHeadID = atol(CString((PCHAR)pNAttributeTemp->text));
			
			// 분, 신 필드 삭제
			if(stHeadInfoType.lHeadID == 8000 || stHeadInfoType.lHeadID == 8001)
				continue;
			
			//FIELD_NAME
			pNAttributeTemp = pNA_FItem->getNamedItem("FIELD_NAME");
			stHeadInfoType.strHeadName = CString((PCHAR)pNAttributeTemp->text);
			
			//ALIGN
			pNAttributeTemp = pNA_FItem->getNamedItem("ALIGN");
			stHeadInfoType.strAlignment = CString((PCHAR)pNAttributeTemp->text);
			
			//COL_WIDTH
			pNAttributeTemp = pNA_FItem->getNamedItem("COL_WIDTH");
			stHeadInfoType.nColWidth = atoi(CString((PCHAR)pNAttributeTemp->text));
			
			//DATA_TYPE
			pNAttributeTemp = pNA_FItem->getNamedItem("DATA_TYPE");
			stHeadInfoType.strDataType = CString((PCHAR)pNAttributeTemp->text);
			
			//UNIT_NUM
			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_NUM");
			stHeadInfoType.nUnitNum = atoi(CString((PCHAR)pNAttributeTemp->text));
			
			//UNIT_TEXT
			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_TEXT");
			stHeadInfoType.strUnitText = CString((PCHAR)pNAttributeTemp->text);
			
			//DATA_LENGTH
			pNAttributeTemp = pNA_FItem->getNamedItem("DATA_LENGTH");
			stHeadInfoType.nDataLength = atoi(CString((PCHAR)pNAttributeTemp->text));
			
			//			//UNIT
			//			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT");
			//			stHeadInfoType.nDataLength = atoi(CString((PCHAR)pNAttributeTemp->text));
			
			// 시스템용 리스트에 추가
			listFixList.AddTail(stHeadInfoType);
		}
	}
	catch(_com_error e)
	{
		CString strMsg = "시스템 파일을 읽어오는 중 에러가 발생하였습니다. 고객지원센터로 연락 바랍니다.";
		//SetSendMainMsg(strMsg);
		bResult = FALSE;
	}
	
	return bResult;
}

BOOL CMainFrmWnd::GetLoadFixUserFieldData(MSXML::IXMLDOMNodePtr pNIndex,
										  CList<HeadFixInfo, HeadFixInfo&> &listFixList)
{
	BOOL bResult = TRUE;
	
	MSXML::IXMLDOMNamedNodeMapPtr	pNAIndex;
	
	MSXML::IXMLDOMNodeListPtr			pNS_FItem;		// Item List
	MSXML::IXMLDOMNodePtr					pN_FItem;	// Item
	MSXML::IXMLDOMNamedNodeMapPtr	pNA_FItem;		// Item Attribute
	
	CString							strView;
	CString							strViewName;
	try 
	{
		MSXML::IXMLDOMNodePtr	pNAttributeTemp;
		pNAIndex = pNIndex->attributes;
		
		pNAttributeTemp = pNAIndex->getNamedItem("VERSION");
		//	m_strFileVersion = CString((PCHAR)pNAttributeTemp->text);			
		
		pNAttributeTemp = pNAIndex->getNamedItem("FIX_COL");
		m_nFixCol = atoi(CString((PCHAR)pNAttributeTemp->text));
		
		pNS_FItem  = pNIndex->selectNodes("FITEM");
		
		//Attribute를 얻어온다
		int nCount, nLength;
		nLength = pNS_FItem->length;
		for (nCount = 0; nCount < nLength; nCount++)
		{
			HeadFixInfo stHeadFixInfo;
			
			//node를 얻어온다.
			pN_FItem = pNS_FItem->item[nCount];	
			
			//속성을 얻어온다.
			MSXML::IXMLDOMNodePtr	pNAttributeTemp;
			pNA_FItem = pN_FItem->attributes;
			
			//ID
			pNAttributeTemp = pNA_FItem->getNamedItem("ID");
			stHeadFixInfo.stHeadType.lHeadID = atol(CString((PCHAR)pNAttributeTemp->text));
			
			//COL_NO
			pNAttributeTemp = pNA_FItem->getNamedItem("COL_NO");
			stHeadFixInfo.nColNum = atoi(CString((PCHAR)pNAttributeTemp->text));
			
			//FIELD_NAME
			pNAttributeTemp = pNA_FItem->getNamedItem("FIELD_NAME");
			stHeadFixInfo.stHeadType.strHeadName = CString((PCHAR)pNAttributeTemp->text);
			
			//UNIT_NUM
			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_NUM");
			stHeadFixInfo.stHeadType.nUnitNum = atoi(CString((PCHAR)pNAttributeTemp->text));
			
			//UNIT_TEXT
			pNAttributeTemp = pNA_FItem->getNamedItem("UNIT_TEXT");
			stHeadFixInfo.stHeadType.strUnitText = CString((PCHAR)pNAttributeTemp->text);
			
			//ALIGN
			pNAttributeTemp = pNA_FItem->getNamedItem("ALIGN");
			stHeadFixInfo.stHeadType.strAlignment = CString((PCHAR)pNAttributeTemp->text);
			
			//COL_WIDTH
			pNAttributeTemp = pNA_FItem->getNamedItem("COL_WIDTH");
			stHeadFixInfo.stHeadType.nColWidth = atoi(CString((PCHAR)pNAttributeTemp->text));
			
			//DATA_TYPE
			pNAttributeTemp = pNA_FItem->getNamedItem("DATA_TYPE");
			stHeadFixInfo.stHeadType.strDataType = CString((PCHAR)pNAttributeTemp->text);
			
			//DATA_LENGTH
			pNAttributeTemp = pNA_FItem->getNamedItem("DATA_LENGTH");
			stHeadFixInfo.stHeadType.nDataLength = atoi(CString((PCHAR)pNAttributeTemp->text));
			
			
			//VIEW_NAME
			pNAttributeTemp = pNA_FItem->getNamedItem("VIEW_NAME");
			strViewName = CString((PCHAR)pNAttributeTemp->text);
			
			//VISIBLE
			pNAttributeTemp = pNA_FItem->getNamedItem("VISIBLE");
			stHeadFixInfo.bView = (CString((PCHAR)pNAttributeTemp->text) == "TRUE")?TRUE:FALSE;
			
			listFixList.AddTail(stHeadFixInfo);
		}
	}
	catch(_com_error e) 
	{
		CString strMsg = "사용자 파일을 읽어오는 중 에러가 발생하였습니다. 고객지원센터로 연락 바랍니다.";
		//SetSendMainMsg(strMsg);
		bResult = FALSE;
	}
	
	return bResult;
}

void CMainFrmWnd::SetSharedDataToCode(CString strCode)
{
	if (strCode.IsEmpty() == FALSE)
	{
		CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
		pScreenWnd->ShareCode(strCode);
	}
}

void CMainFrmWnd::SetStateTitleBar(CString strMsg)
{
	CString strData = "진행상태 : ";
	strData += strMsg;
	m_ctrlState.SetWindowText(strData);
	m_ctrlState.Invalidate();
}

void CMainFrmWnd::SetEnable(BOOL bEnable)
{
	m_comboTarget_1st.EnableWindow(bEnable);
	
	m_btnReverseChk.EnableWindow(bEnable);
	if (bEnable == TRUE && m_comboTarget_1st.GetCurSel() == 0)
	{
		m_comboTarget_2nd.EnableWindow(FALSE);
		m_btnReverseChk.EnableWindow(FALSE);
	}
	m_btnExceptionChk.EnableWindow(bEnable);
	
	m_btnDay.EnableWindow(bEnable);
	m_btnWeek.EnableWindow(bEnable);
	m_btnMonth.EnableWindow(bEnable);
	m_btnMin.EnableWindow(bEnable);
	
	m_comboTime.EnableWindow(bEnable);
	if (bEnable == TRUE)
		m_comboTime.EnableWindow(m_btnMin.GetCheck());
	
	m_gridResult.EnableWindow(bEnable);
	
// 	m_btnSave.EnableWindow(bEnable);
// 	m_btnReset.EnableWindow(bEnable);
// 	m_btnSearch.EnableWindow(bEnable);
	
	m_editBongCnt.EnableWindow(bEnable);
	m_btnIgnoreColorChk.EnableWindow(bEnable);
	m_comboTail.EnableWindow(bEnable);
	m_comboPrevTrend.EnableWindow(bEnable);

	m_btnBodyRateChk.EnableWindow(bEnable);
	if(m_btnBodyRateChk.GetCheck()) // 몸통비율 적용이 체크되어 있을 때만 하면됨
	{
		for(int i = 0; i < MAX_CANDLE; i++)
		{
			m_btnUpdown[i].EnableWindow(bEnable);
			m_editUpdownRate[i].EnableWindow(bEnable);
		}
	}


	m_CandleMgr.EnableWindow(bEnable);
	
//	m_btnSearch.SetFocus();
}

void CMainFrmWnd::ConvertMstData(LPCSTR szTR, LPVOID	aTRData, int dwTRDataLen)
{
	m_saCodeMST.RemoveAll();
	
	CString strAddData, strData;
	
	strAddData = "";
	char *pBuff;
	
//	MSTCode** pstMSTCode = (MSTCode**)aTRData;
	MSTCode stMSTCode;
	char* pstData = (char*)aTRData;
	
	int nDataCnt = dwTRDataLen / sizeof(MSTCode);
	CString strCode;
	int nPos = 0;
	
	for (int nIndex = 0; nIndex < nDataCnt; nIndex++)
	{
		memset(&stMSTCode, 0x00, sizeof(MSTCode));	
		memcpy(&stMSTCode, &pstData[nPos], sizeof(MSTCode));
		nPos += sizeof(MSTCode);
	//	memcpy(&stMSTCode, &pstMSTCode[0][nIndex], sizeof(MSTCode));
		
		STR_CONVERT(pBuff, stMSTCode.sMarket, sizeof(stMSTCode.sMarket), strData);
		strAddData = strData;
		
		STR_CONVERT(pBuff, stMSTCode.sCode, sizeof(stMSTCode.sCode), strData);
		strData.TrimLeft(" ");
		strData.TrimRight(" ");
		
		strAddData += strData;
		m_saCodeMST.Add(strAddData); //NULL로 끝나므로 이렇게 해도 됨.
	}
	int size = m_saCodeMST.GetSize();
}

void CMainFrmWnd::SetTrData_ThemeGroup()
{
	m_saCodeTheme.RemoveAll();
	CStringArray arrCode, arrName;
	CString strCode, strName;

	int nIndex;
	for(nIndex = 0; nIndex < arrCode.GetSize(); nIndex++)
	{
		strCode.Format("%4.4s", arrCode.GetAt(nIndex));
		strName.Format("%-36.36s",	arrName.GetAt(nIndex));
		m_saCodeTheme.Add(strCode + strName);	
	}
}

BOOL CMainFrmWnd::PreTranslateMessage(MSG* pMsg) 
{
	UINT nChar = (UINT)pMsg->wParam;
	if(pMsg->message == WM_KEYDOWN)  
	{		
		switch(nChar)
		{
		case VK_RETURN :
			{
				if (m_editBongCnt.GetSafeHwnd() == pMsg->hwnd)
				{
					OnKillfocusBongCnt();
					return TRUE;
				}
				else if (m_editUpdownRate[0].GetSafeHwnd() == pMsg->hwnd)
				{
					OnKillfocusRate1();
					return TRUE;
				}
				else if (m_editUpdownRate[1].GetSafeHwnd() == pMsg->hwnd)
				{
					OnKillfocusRate2();
					return TRUE;
				}
				else if (m_editUpdownRate[2].GetSafeHwnd() == pMsg->hwnd)
				{
					OnKillfocusRate3();
					return TRUE;
				}
				else if (m_editUpdownRate[3].GetSafeHwnd() == pMsg->hwnd)
				{
					OnKillfocusRate4();
					return TRUE;
				}
				else if (m_editUpdownRate[4].GetSafeHwnd() == pMsg->hwnd)
				{
					OnKillfocusRate5();
					return TRUE;
				}

				return FALSE;
			}
		case VK_TAB:
			if(::GetKeyState(VK_CONTROL) < 0) // 삼성: ctrl-tab 이 안먹는 현상 때문에
			{
				m_btnSearch.SetFocus();
				GetParent()->GetParent()->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
				m_btnSearch.SetFocus();
				return FALSE;
			}

		case VK_BACK :
			break;

		default:
			if(m_editUpdownRate[0].GetSafeHwnd() == pMsg->hwnd || m_editUpdownRate[1].GetSafeHwnd() == pMsg->hwnd ||
				m_editUpdownRate[2].GetSafeHwnd() == pMsg->hwnd || m_editUpdownRate[3].GetSafeHwnd() == pMsg->hwnd ||
				m_editUpdownRate[4].GetSafeHwnd() == pMsg->hwnd)
			{
				if(nChar < '0' || nChar > '9')		
					return TRUE;
			}
		}
	}
	else if(pMsg->message == WM_LBUTTONDOWN)
	{
		if(pMsg->hwnd == m_editBongCnt.GetSafeHwnd())
		{
			if(GetFocus() != &m_editBongCnt)
			{
				m_editBongCnt.SetFocus();
				m_editBongCnt.SetSel(0, -1);
				return TRUE;
			}
		}
		else 
		{
			for(int i = 0; i < MAX_CANDLE; i++)
			{
				if(pMsg->hwnd == m_editUpdownRate[i].GetSafeHwnd() && GetFocus() != &m_editUpdownRate[i])
				{
					m_editUpdownRate[i].SetFocus();
					m_editUpdownRate[i].SetSel(0, -1);
					return TRUE;
				}
			}
		}
	}
	else if(pMsg->message == WM_SYSKEYDOWN)
	{
		if( ::GetKeyState(VK_MENU) & 0x8000)
			return FALSE;
	}

	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrmWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	SetCommMenu(&point);

	CFrameWnd::OnRButtonUp(nFlags, point);
}

extern void KSLink_SetFormLinkTagData( CString sTag, CString sValue );
//extern void KSLink_SetLinkTagData( LPCTSTR sTag, LPCTSTR sValue);
void CMainFrmWnd::SetCommMenu(POINT *point)
{
//	HWND hChildFrame = (HWND)m_pKSGateMng->GetChildFrame();
//	if(!hChildFrame) return;
//
//	long lMenu = ::SendMessage(hChildFrame, UM_GET_SCREENLINKMENU, 0, 3 );
//	if ( lMenu > 0 )
//	{
//		CMenu* pMenu = (CMenu*)lMenu;
//		ASSERT ( pMenu );
//
//		GetCursorPos(point);
//		UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
//		int nRetValue = pMenu->TrackPopupMenu(nFlagsForMenu, point->x, point->y, this);
//		
//		if( nRetValue>0 )
//		{
//			::PostMessage(hChildFrame, WM_COMMAND, nRetValue, 0);
//			return;
//		}
//		pMenu->DestroyMenu();
//		delete pMenu;
//	}
}

void CMainFrmWnd::OnClickListItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMITEMACTIVATE* pIA = (LPNMITEMACTIVATE) pNMHDR;

	int nSelIndex = pIA->iItem;
	if(nSelIndex == -1) return;

	CANDLE_SET* pSet = m_CandleSetArray.GetAt(nSelIndex);
	CString strText;

	m_CandleMgr.SetCandleSet(pSet);
	m_CandleMgr.SetFocusCandle(4);

	// >> 저장된 옵션 적용안되도록 함 20090618
	//m_btnIgnoreColorChk.SetCheck(pSet->nIgnoreColor);
	//m_comboTail.SetCurSel(pSet->nIgnoreTail);
	//m_comboPrevTrend.SetCurSel(pSet->nPreTrend); 

	m_btnBodyRateChk.SetCheck(pSet->nApplyBodyLength);
	OnBtnChkBodyRate();

	if(pSet->nApplyBodyLength == 0)
	{
		pSet->nBodyRate[0] = pSet->nBodyRate[1]= pSet->nBodyRate[2]= pSet->nBodyRate[3]= pSet->nBodyRate[4] = DEFAULT_BODYRATE;
		pSet->nBodyRateDir[0] = pSet->nBodyRateDir[1]= pSet->nBodyRateDir[2]= pSet->nBodyRateDir[3]= pSet->nBodyRateDir[4] = 1;
	}
	
	for(int i = 0; i < MAX_CANDLE; i++)
	{
		if(!pSet->cd[i].bEnable) pSet->nBodyRate[i] = DEFAULT_BODYRATE;

		strText.Format("%d", pSet->nBodyRate[i]);
		m_editUpdownRate[i].SetWindowText(strText);

		if(pSet->nBodyRateDir[i] == 1)
		{
			m_btnUpdown[i].SetWindowText("↑");
			strText = "이상";
		}
		else
		{
			m_btnUpdown[i].SetWindowText("↓");
			strText = "이하";
		}
		
		m_bUpdownState[i] = pSet->nBodyRateDir[i];
	}

	m_strCaption = pSet->szCandleName;
	CRect rcText = m_reTopCandle;
	rcText.DeflateRect(5, 2);
	InvalidateRect(rcText);

	OnBtnSearch();

	*pResult = 0;
}

void CMainFrmWnd::OnClickUserListItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMITEMACTIVATE* pIA = (LPNMITEMACTIVATE) pNMHDR;

	int nSelIndex = pIA->iItem;
	if(nSelIndex == -1) return;

	CANDLE_SET* pSet = m_UserCandleSetArray.GetAt(nSelIndex);
	CString strText;

	m_CandleMgr.SetCandleSet(pSet);
	m_CandleMgr.SetFocusCandle(4);

	// >> 저장된 옵션 적용안되도록 함 20090618
	//m_btnIgnoreColorChk.SetCheck(pSet->nIgnoreColor);
	//m_comboTail.SetCurSel(pSet->nIgnoreTail);
	//m_comboPrevTrend.SetCurSel(pSet->nPreTrend);

	m_btnBodyRateChk.SetCheck(pSet->nApplyBodyLength);

	if(pSet->nApplyBodyLength == 0)
	{
		pSet->nBodyRate[0] = pSet->nBodyRate[1]= pSet->nBodyRate[2]= pSet->nBodyRate[3]= pSet->nBodyRate[4] = DEFAULT_BODYRATE;
		pSet->nBodyRateDir[0] = pSet->nBodyRateDir[1]= pSet->nBodyRateDir[2]= pSet->nBodyRateDir[3]= pSet->nBodyRateDir[4] = 1;
	}


	for(int i = 0; i < MAX_CANDLE; i++)
	{
		if(!pSet->cd[i].bEnable) pSet->nBodyRate[i] = DEFAULT_BODYRATE;
		
		strText.Format("%d", pSet->nBodyRate[i]);
		m_editUpdownRate[i].SetWindowText(strText);

		if(pSet->nBodyRateDir[i] == 1)
		{
			m_btnUpdown[i].SetWindowText("↑");
			strText = "이상";
		}
		else
		{
			m_btnUpdown[i].SetWindowText("↓");
			strText = "이하";
		}

		m_bUpdownState[i] = pSet->nBodyRateDir[i];
	}
	OnBtnChkBodyRate();

	m_strCaption = pSet->szCandleName;
	CRect rcText = m_reTopCandle;
	rcText.DeflateRect(5, 2);
	InvalidateRect(rcText);

	OnBtnSearch();

	*pResult = 0;
}

void CMainFrmWnd::OnChangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
  int nTabIndex = m_ptabCondition->GetCurSel();

  BOOL bTabChange = TRUE;
  if(nTabIndex == 1)
  {
 	  m_ctrlListItem.ShowWindow(SW_HIDE);

	  m_ctrlUserListItem.ShowWindow(SW_SHOW);
	  m_ctrlUserListItem.BringWindowToTop();
	  m_ctrlUserListItem.Invalidate();
  }
  else
  {
 	  m_ctrlUserListItem.ShowWindow(SW_HIDE);

	  m_ctrlListItem.ShowWindow(SW_SHOW);
	  m_ctrlListItem.BringWindowToTop();
	  m_ctrlListItem.Invalidate();
  }
}

void CMainFrmWnd::OnBtnPrint()
{
	SetResultPrint();
}

void CMainFrmWnd::SetResultPrint()
{
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	CPrintDialog printDlg(FALSE);
	if(printDlg.DoModal() != IDOK) return;

	CDC dc;
	dc.Attach(printDlg.GetPrinterDC());
	dc.m_bPrinting = TRUE;


	//인쇄될 도큐먼트에 관한 정보 설정
	CString strTitle;
	strTitle = "조건검색";
	DOCINFO di;
	::ZeroMemory(&di, sizeof(DOCINFO));
	di.cbSize = sizeof(di);
	di.lpszDocName = strTitle;


	//도큐먼트 인쇄 시작
	BOOL bPrintingOK = dc.StartDoc(&di);

	//CPrintInfo에 인쇄 관련 정보설정
	CPrintInfo Info;
	Info.m_rectDraw.SetRect(0, 0, dc.GetDeviceCaps(HORZRES),
		dc.GetDeviceCaps(VERTRES));
	
	
	//프린트시 셀 스타일
	m_gridResult.SetBorderStyle4Print(UG_BDR_LTHIN | UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	
	//프린트시 셀 선색 선택
	m_gridResult.SetCellBorderColor4Print(RGB(0, 0, 0));


	//top헤더 출력 셋팅
	m_gridResult.PrintingSetOption(UG_PRINT_TOPHEADING, TRUE);
	

	//좌우측 여백.
	m_gridResult.PrintingSetOption(UG_PRINT_LEFTMARGIN, 5);
	m_gridResult.PrintingSetOption(UG_PRINT_RIGHTMARGIN, 5);

	
	//페이지 셋팅
	Info.SetMaxPage(
		m_gridResult.PrintingInit(&dc, &printDlg, 0, 0, m_gridResult.GetNumberCols()-1, m_gridResult.GetNumberRows()-1)
		);

	
	//페이지별 프린트
	for(UINT page = Info.GetMinPage(); page <= Info.GetMaxPage() && bPrintingOK; page++)
	{
		dc.StartPage();
		Info.m_nCurPage = page;

		m_gridResult.PrintingPage(&dc, Info.m_nCurPage);

		m_gridResult.PrintingTail(&dc, &Info);

		bPrintingOK = (dc.EndPage() > 0);
	}


	if(bPrintingOK) dc.EndDoc();
	else			dc.AbortDoc();

	
	dc.Detach();

	AfxSetResourceHandle(hInstSave);
}
//
HeadInfoType CMainFrmWnd::GetMoveFieldPosInfo()
{
	HeadInfoType	stHeadInfoType;

	//# 값보기 위치!
//	stHeadInfoType.lHeadID = (long)GRID_MOVEFIELD_POS_INDEX;
//	stHeadInfoType.strHeadName = (CString)GRID_MOVEFIELD_POS_NAME;
//	stHeadInfoType.nUnitNum = 0;
//	stHeadInfoType.strUnitText = "";
//	stHeadInfoType.strDataType = "";
//	stHeadInfoType.nDataLength = 0;
//	stHeadInfoType.nColWidth = 0;
//	stHeadInfoType.strAlignment = "";

	return stHeadInfoType;
}

// CString CMainFrmWnd::GetFilePath(CString strLoadType)
// {
// 	CString strFile, strFolder;
// 	CString strFilePath = "";
// 	CString strAppPath = "";
// 	
// 	BOOL bUserFolder = FALSE;
// 	BOOL bSetIndustry = TRUE;
// 	
// 	strFile = (CString)FIXFIELD_USER_FILE;;
// 	
// 	GETPATH_APP(strAppPath);
// 	strFilePath = strAppPath + (CString)FOLDER_DATA + "\\" + strFile;
// 	
// 	return strFilePath;
// }

int CMainFrmWnd::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	return ::MessageBox(m_hWnd, lpszText, lpszCaption, nType);
}

void CMainFrmWnd::SetFieldEditApply()
{
	SetSaveFixUserFieldData(&m_listFixHeadUser, m_strFixFieldVersion);

	SetGrid_Head();
	SetGrid_Head(TRUE);
	OnBtnSearch();
}

void CMainFrmWnd::OnBtnFieldEdit()
{
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	CFieldEditDlg dlg(this, 0);
	if(dlg.DoModal() == IDOK)
	{

	}
	
	AfxSetResourceHandle(hInstSave);
}

void CMainFrmWnd::OnBtnResultCtrl()
{
	CRect reClient;
	m_btnResultCtrl.GetWindowRect(reClient);

	CPoint ptMenu;
	ptMenu.x = reClient.left;
	ptMenu.y = reClient.bottom;
	SetPopupMenuResultCtrl(ptMenu);
}

void CMainFrmWnd::OnBtnResultExpand()
{
	m_bExpand = !m_bExpand;

	if(m_bExpand)
	{
		m_ctrlBongCntText1.ShowWindow(SW_HIDE);
		m_ctrlBongCntText2.ShowWindow(SW_HIDE);
		m_editBongCnt.ShowWindow(SW_HIDE);
		m_btnIgnoreColorChk.ShowWindow(SW_HIDE);
		m_comboTail.ShowWindow(SW_HIDE);
		//m_comboPrevTrend.ShowWindow(SW_HIDE);
		//m_btnSearch.ShowWindow(SW_HIDE);
		m_btnReset.ShowWindow(SW_HIDE);
		m_btnSave.ShowWindow(SW_HIDE);
		m_CandleMgr.ShowWindow(SW_HIDE);

		m_btnMin.ShowWindow(SW_HIDE);
		m_btnMonth.ShowWindow(SW_HIDE);
		m_btnWeek.ShowWindow(SW_HIDE);
		m_btnDay.ShowWindow(SW_HIDE);
		m_comboTime.ShowWindow(SW_HIDE);

		/*m_Chart.ShowWindow(SW_HIDE);*/

		m_ctrlCandleName.ShowWindow(SW_HIDE);
		m_ctrlJongName.ShowWindow(SW_HIDE);

		m_pDlgChart->ShowWindow(SW_HIDE);

		m_btnBodyRateChk.ShowWindow(SW_HIDE);
		SetBtnImg(&m_btnExpand, "DOWN_EX");

	}
	else
	{
		m_ctrlBongCntText1.ShowWindow(SW_SHOW);
		m_ctrlBongCntText2.ShowWindow(SW_SHOW);
		m_editBongCnt.ShowWindow(SW_SHOW);
		m_btnIgnoreColorChk.ShowWindow(SW_SHOW);
		m_comboTail.ShowWindow(SW_SHOW);
		//m_comboPrevTrend.ShowWindow(SW_SHOW);
		//m_btnSearch.ShowWindow(SW_SHOW);
		m_btnReset.ShowWindow(SW_SHOW);
		m_btnSave.ShowWindow(SW_SHOW);
		m_CandleMgr.ShowWindow(SW_SHOW);

		m_btnMin.ShowWindow(SW_SHOW);
		m_btnMonth.ShowWindow(SW_SHOW);
		m_btnWeek.ShowWindow(SW_SHOW);
		m_btnDay.ShowWindow(SW_SHOW);
		m_comboTime.ShowWindow(SW_SHOW);

		/*m_Chart.ShowWindow(SW_SHOW);*/

		m_ctrlCandleName.ShowWindow(SW_SHOW);
		m_ctrlJongName.ShowWindow(SW_SHOW);

		m_pDlgChart->ShowWindow(SW_SHOW);

		m_btnBodyRateChk.ShowWindow(SW_SHOW);
		SetBtnImg(&m_btnExpand, "UP_EX");
	}

	if(m_btnBodyRateChk.GetCheck()) // 몸통비율 적용이 체크되어 있을 때만 하면됨
	{
		for(int i = 0; i < MAX_CANDLE; i++)
		{
			m_ctrlPersent[i].ShowWindow(m_bExpand?SW_HIDE:SW_SHOW);
			m_btnUpdown[i].ShowWindow(m_bExpand?SW_HIDE:SW_SHOW);
			m_editUpdownRate[i].ShowWindow(m_bExpand?SW_HIDE:SW_SHOW);
		}
	}

	MoveControls();

// 	int nX;
// 	int nY;
// 	CRect rt;
// 
// 	m_btnExpand.GetWindowRect(rt);
// 	nX = rt.left + rt.Width()/2;
// 	nY = rt.top + rt.Height()/2;
// 
// 	SetCursorPos(nX, nY);

	Invalidate();
}

void CMainFrmWnd::SetPopupMenuResultCtrl(CPoint ptMenu)
{
	int bIsHTS = 1;
	
	CMenu menu;
	menu.CreatePopupMenu();

	CMenu *pMenuChild = NULL;
	pMenuChild = new CMenu;
	pMenuChild->CreatePopupMenu();
	
	CString strScreenInfo;
	int nIndex, nMenuType, nMenuCnt;

	nMenuCnt = _countof(MENU_RESULTCTRL_NAME);
	for (nIndex = 0; nIndex < nMenuCnt; nIndex++)
	{
		if (MENU_RESULTCTRL_NAME[nIndex] == "")
			nMenuType = MF_SEPARATOR;
		else
			nMenuType = MF_STRING;
		
		if(bIsHTS)
		{
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
		}
		else
		{
			strScreenInfo = (CString)MENU_RESULTCTRL_SCREEN_NOEMP[nIndex];
			if (atoi(strScreenInfo) <= 0)
				strScreenInfo = "";
			
			if (strScreenInfo.IsEmpty() == FALSE)
			{
				strScreenInfo = strScreenInfo.Left(5);
				strScreenInfo = "[" + strScreenInfo + "] " + (CString)MENU_RESULTCTRL_NAME[nIndex];
			}
			else
				strScreenInfo = "          " + (CString)MENU_RESULTCTRL_NAME[nIndex];
		}			
		if(nIndex == 1)
		{
			menu.AppendMenu(MF_POPUP, (UINT)pMenuChild->GetSafeHmenu(), (LPCTSTR)strScreenInfo); 
			
			CStringArray arrKey, arrGroupName;
			GetData_PortGroup(arrKey, arrGroupName);
			
			int nMenuIndex = 11200;
			for(int i = 0; i < arrKey.GetSize(); i++)
				pMenuChild->AppendMenu(MF_STRING, nMenuIndex + atoi(arrKey.GetAt(i)), arrGroupName.GetAt(i));
			
			pMenuChild->AppendMenu(MF_SEPARATOR);
			
			pMenuChild->AppendMenu(MF_STRING, nMenuIndex, "새 관심그룹 추가");
		}
		else
			menu.AppendMenu(nMenuType, MENU_RESULTCTRL_INDEX[nIndex], (LPCTSTR)strScreenInfo); 
	}
	GetCursorPos(&ptMenu);
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptMenu.x, ptMenu.y, this);

	pMenuChild = menu.GetSubMenu(0);
	if (pMenuChild)
	{
		pMenuChild->DestroyMenu();
		delete pMenuChild;
	}

	menu.DestroyMenu();		
}

#define SCREEN_NO_ALLBUY				"NOT USER"
#define SCREEN_NO_ROLL					"NOT USER"
void CMainFrmWnd::SetSharedDataToSend(ShardData *pstShardData)
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

// 검색한 정보를 파일(excel 또는 text)로 저장
void CMainFrmWnd::SetResultSave()
{
	CFileFind FileFinder;

	BOOL bExist = FALSE;
	BOOL bSave = TRUE;

	int nReturn = 0;
	CString strMsg = "";
	CString strFileName, strFileType;
	CString strDefaultName = "임시저장";

	static char BASED_CODE szFilter[] = "WorkSheet Files (*.xls)|*.xls|EZ-Finder Data Files (*.txt)|*.txt||";
	CFileDialog saveDlg(FALSE, _T("xls"), strDefaultName + _T(".xls"), NULL, szFilter, NULL);
	
	while (bSave)
	{
		if (saveDlg.DoModal() == IDOK)
		{
			strFileType = saveDlg.GetFileExt();
			strFileName = saveDlg.GetPathName();
			
			bExist = FileFinder.FindFile(strFileName);
			if (bExist == TRUE)
			{
				strMsg.Format("%s 은(는) 이미 있습니다.\n바꾸시겠습니까?", strFileName);
				nReturn = SetMessageBox(strMsg, PROGRAM_TITLE, MB_YESNO | MB_ICONQUESTION);
				if (nReturn == IDYES)
				{
					if (!strFileName.IsEmpty())
					{
						if (!strFileType.CompareNoCase("xls"))				
							SetSaveGridToExcel(strFileName);
						else if (!strFileType.CompareNoCase("txt"))				
							SetSaveGridToText(strFileName);
					}
					bSave = FALSE;
					break;
				}
				else
					bSave = TRUE;
			}
			else
			{
				if(!strFileName.IsEmpty())
				{
					if (!strFileType.CompareNoCase("xls"))				
						SetSaveGridToExcel(strFileName);
					else if (!strFileType.CompareNoCase("txt"))				
						SetSaveGridToText(strFileName); //txt file save 
				}
				bSave = FALSE;
				break;
			}
		}
		else
		{
			bSave = FALSE;
			break;
		}

		if (bSave == FALSE)
			break;
	}
}

// Excel 파일로 저장
void CMainFrmWnd::SetSaveGridToExcel(CString strFileName)
{
	int nRow, nCol;
	CDrXLSFile xlsfile;
	
	CString strColName = "";
	CString strData = "";
			
	if (!xlsfile.OpenFile(strFileName))
	{
		CString strMsg = "지정한 파일이 다른 프로그램에서 사용중입니다";
		SetMessageBox(strMsg, (CString)PROGRAM_TITLE, MB_OK);
		return;
	}
	else
	{
		nRow = -1;
		for (nCol = 0; nCol < m_gridResult.GetNumberCols(); nCol++)
		{
			strData = m_gridResult.GetCellData(nRow, nCol);
			xlsfile.SetTextAlign(EXCEL_ALIGN_CENTER);
			xlsfile.WriteLabel(0, nCol, strData);
		}

		int nWriteRow=0;
		for (nRow = 0; nRow < m_gridResult.GetNumberRows(); nRow++)
		{
			//[20110825alzioyes]0000135: 조건검색결과리스트에체크박스및기능추가, 저장관련.
			if(!m_gridResult.IsRowCodeChecked(nRow)) continue;

			for (nCol = 0; nCol < m_gridResult.GetNumberCols(); nCol++)
			{
				strData = m_gridResult.GetCellData(nRow, nCol);
				strData.Replace("&&", "&");

				strData.TrimRight(" ");
				strData.TrimLeft(" ");
						
				strColName.Format("%s", m_gridResult.GetCellData(-1, nCol));
				if ((strColName.Compare("등락율") == 0) ||
					(strColName.Compare("전일대비") == 0))
				{
					COLORREF clrText, clrBack;
					m_gridResult.GetCellColor(nRow, nCol, clrText, clrBack);

					if (clrText == COLOR_BLUE)
						strData = "-" + strData;
					else if (clrText == COLOR_RED)
						strData = "+" + strData;
				}
        						
				//if (strColName.Compare("종목코드") == 0)
				if (strColName.Find("종목코드") >= 0)
					xlsfile.SetTextAlign(EXCEL_ALIGN_CENTER);
				//else if (strColName.Compare("종목명") == 0)
				else if (strColName.Find("종목명") >= 0)
					xlsfile.SetTextAlign(EXCEL_ALIGN_LEFT);
				else
					xlsfile.SetTextAlign(EXCEL_ALIGN_RIGHT);
        
				xlsfile.WriteLabel(nWriteRow + 1, nCol, (LPCSTR)strData);
			}
			nWriteRow++;
		}
	}
	xlsfile.CloseFile();
}

// Text 파이로 저장
void CMainFrmWnd::SetSaveGridToText(CString strFileName)
{
	CFile SaveFile(strFileName,CFile::modeCreate | CFile::modeWrite);
	
	int nRowCnt = m_gridResult.GetNumberRows();

	CString strLineData, strCell;
	int nRow, nCol;
	BOOL bCodeCheck=TRUE;

	for (nRow = 0; nRow < nRowCnt; nRow++)
	{
		// 타이틀 저장용
		if (nRow == 0)
		{
			strCell = m_gridResult.GetCellData(0, 0);
			strLineData = strCell;	// 종목명
			strLineData += " ";
			
			for (nCol = 1; nCol < m_gridResult.GetNumberCols(); nCol++)
			{
				strCell = m_gridResult.GetCellData(0, nCol);
				strLineData += strCell;	// 각 Item 정보
				strLineData += " ";
			}
			
			strLineData += "\r\n";
			SaveFile.Write( strLineData, strLineData.GetLength() );			
		}

		//[20110825alzioyes]0000135: 조건검색결과리스트에체크박스및기능추가, 저장관련.
		if(!m_gridResult.IsRowCodeChecked(nRow)) continue;

		strCell = m_gridResult.GetCellData(nRow, 0);
		strLineData = strCell;	// 종목명
		strLineData += " ";
		
		for (nCol = 1; nCol < m_gridResult.GetNumberCols();  nCol++)
		{
			strCell = m_gridResult.GetCellData(nRow, nCol);
			strLineData += strCell;	// 각 Item 정보
			strLineData += " ";
		}
		
		strLineData += "\r\n";
		SaveFile.Write(strLineData, strLineData.GetLength());
	}
	
	SaveFile.Close();
}

void CMainFrmWnd::SetSendMainMsg(CString strMsg)
{
	GetParent()->SendMessage(UMSG_DISP_MSG, (WPARAM)(LPCSTR)"", (LPARAM)(LPCSTR)strMsg);
}

void CMainFrmWnd::OnClickBtnRate(UINT nID)
{
	int nIndex = nID - IDC_BTN_RATE1;
	CString strTip;

	m_bUpdownState[nIndex] = !m_bUpdownState[nIndex];
	
	if(m_bUpdownState[nIndex])
	{
		m_btnUpdown[nIndex].SetWindowText("↑");
		strTip = "이상";
	}
	else
	{
		m_btnUpdown[nIndex].SetWindowText("↓");
		strTip = "이하";
	}
}

void CMainFrmWnd::OnMenuResultCtrl(UINT nIndex)
{
	int bIsHTS = 1;
	
	CString strCode;
	CString strName;
	int nMarkType;
	ShardData		stShardData;
	
	CString strPortGroup;
	strPortGroup.Format("%d", nIndex);
	
	if(nIndex >= 11200)
		nIndex = 11020;

	int nDataCnt;
	nDataCnt = m_gridResult.GetNumberRows();

	if(nDataCnt <= 0)
	{
		SetSendMainMsg("전송할 데이터가 없음.");
		return;
	}

	int nItem;
	for(nItem = 0; nItem < _countof(MENU_RESULTCTRL_INDEX); nItem++)
	{
		if(nIndex == MENU_RESULTCTRL_INDEX[nItem])
			break;
	}

	if (nIndex == (int)SCREEN_INDEX_EXCEL)
	{
		SetResultSave();
	}
	else if(nIndex == (int)SCREEN_INDEX_RUSULTPRINT)
	{
		SetResultPrint();
	}
	else
	{
		BOOL bCodeCheck = FALSE;
		if(atoi(strPortGroup) >= 11200)
		{
			stShardData.saCode.Add(strPortGroup.Right(2));
			stShardData.saName.Add("");
			stShardData.iaMarkType.Add(0);
		}

		for (int nRow = 0; nRow < nDataCnt; nRow++)
		{
			bCodeCheck = m_gridResult.IsRowCodeChecked(nRow);
			if(bCodeCheck)
			{
				/*m_gridResult.GetRowCode(nRow, strCode, nMarkType);*/
				m_gridResult.GetRowToCodeName(nRow, strCode, strName, nMarkType);

				if (!strCode.IsEmpty())
				{
					stShardData.saCode.Add(strCode);
					stShardData.saName.Add(strName);
					stShardData.iaMarkType.Add(nMarkType);
				}
			}
		}

		if (stShardData.saCode.GetSize() <= 0)
		{
			AfxMessageBox("전송할 데이터가 없습니다.");
			return;
		}
		
		stShardData.bOpen = MENU_RESULTCTRL_SCREEN_OPEN[nItem];
		
// 		if(bIsHTS)
// 			stShardData.strScreenNo = MENU_RESULTCTRL_SCREEN_NOMAX[nItem];
// 		else
// 			stShardData.strScreenNo = MENU_RESULTCTRL_SCREEN_NOEMP[nItem];

		stShardData.strScreenNo = MENU_RESULTCTRL_SCREEN_NOMAX[nItem];
		stShardData.strMapNo = MENU_RESULTCTRL_MAP_NO[nItem];
		
		ShardData *pstShardData = (ShardData*)&stShardData;
		SetSharedDataToSend(pstShardData);
	}
}

CANDLE_SET* CMainFrmWnd::ParseToCandleSet(CString strLine)
{
	if(strLine.Left(7) == "VERSION")
	{
		strLine.Remove(_T(' '));
		m_strVerion = strLine.Mid(8);
		return NULL;
	}

	int nCandleDataMinLen = 13*MAX_CANDLE;
	if(strLine.GetLength() < nCandleDataMinLen)
		return NULL;

	CANDLE_SET* pSet = new CANDLE_SET;
	memset(pSet, 0, sizeof(CANDLE_SET));
	
	int nFirst = 0;
	for(int nCandle = 0; nCandle < MAX_CANDLE; nCandle++)
	{
		pSet->cd[nCandle].bEnable = atoi(strLine.Mid(nFirst, 1)); nFirst++;
		pSet->cd[nCandle].nOpen = atoi(strLine.Mid(nFirst, 3)); nFirst += 3;
		pSet->cd[nCandle].nHigh = atoi(strLine.Mid(nFirst, 3)); nFirst += 3;
		pSet->cd[nCandle].nLow = atoi(strLine.Mid(nFirst, 3)); nFirst += 3;
		pSet->cd[nCandle].nClose = atoi(strLine.Mid(nFirst, 3)); nFirst += 3;
	}
	
	pSet->nIgnoreTail = atoi(strLine.Mid(nFirst, 1)); nFirst++;
	pSet->nIgnoreColor = atoi(strLine.Mid(nFirst, 1)); nFirst++;
	pSet->nPreTrend = atoi(strLine.Mid(nFirst, 2)); nFirst += 2;
	if(m_strVerion == "1.0")
	{
		pSet->nApplyBodyLength = atoi(strLine.Mid(nFirst, 1)); nFirst++;
		pSet->nBodyRate[0] = atoi(strLine.Mid(nFirst, 2)); nFirst += 2;
		pSet->nBodyRate[1] = atoi(strLine.Mid(nFirst, 2)); nFirst += 2;
		pSet->nBodyRate[2] = atoi(strLine.Mid(nFirst, 2)); nFirst += 2;
		pSet->nBodyRate[3] = atoi(strLine.Mid(nFirst, 2)); nFirst += 2;
		pSet->nBodyRate[4] = atoi(strLine.Mid(nFirst, 2)); nFirst += 2;

		pSet->nBodyRateDir[0] = atoi(strLine.Mid(nFirst, 1)); nFirst++;
		pSet->nBodyRateDir[1] = atoi(strLine.Mid(nFirst, 1)); nFirst++;
		pSet->nBodyRateDir[2] = atoi(strLine.Mid(nFirst, 1)); nFirst++;
		pSet->nBodyRateDir[3] = atoi(strLine.Mid(nFirst, 1)); nFirst++;
		pSet->nBodyRateDir[4] = atoi(strLine.Mid(nFirst, 1)); nFirst++;
	}

	CString strTitle = strLine.Mid(nFirst);
	memcpy(pSet->szCandleName, strTitle, strTitle.GetLength());

	return pSet;
}

void CMainFrmWnd::ReadPresetCandle()
{
	CString strFilePath;
	strFilePath = m_strAppPath + (CString)FOLDER_DATA + "\\PresetCndl.dat";

	CStdioFile file;
	if( !file.Open( strFilePath, CFile::modeRead | CFile::typeText ) ) {
		return;
	}

	CString strLine;
	m_strVerion.Empty();
	while(file.ReadString(strLine))
	{
		CANDLE_SET* pSet = ParseToCandleSet(strLine);
		if(pSet == NULL)
			continue;
		
		m_CandleSetArray.Add(pSet);
	}

	file.Close();
}

// 사용자가 저장한 캔들 파일을 읽어온다.
// 각각의 파일로 저장하지 않고 하나의 텍스트 파일에 저장(UserCndl.dat)
void CMainFrmWnd::ReadUserCandleSet()
{
	CString strFilePath;
	strFilePath = GetFilePath("USERCONDFILE") + "\\finddata\\UserPtrn\\UserCndl.dat";

	CStdioFile file;
	if( !file.Open( strFilePath, CFile::modeRead | CFile::typeText ) ) {
		return;
	}

	CString strLine;
	m_strVerion.Empty();
	while(file.ReadString(strLine))
	{
		CANDLE_SET* pSet = ParseToCandleSet(strLine);
		if(pSet == NULL) continue;
		
		m_UserCandleSetArray.Add(pSet);
	}

	file.Close();
}

void CMainFrmWnd::SaveUserCandleSet()
{
	CString strFilePath;	
	strFilePath = GetFilePath("USERCONDFILE") + "\\finddata\\UserPtrn";
	CreateDirectory(strFilePath, NULL);
	strFilePath += "\\UserCndl.dat";

	CStdioFile file;
	if( !file.Open( strFilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeText ) ) {
//<<!>> 메시지 표시
		DWORD dw = ::GetLastError();
		
		return;
	}

	int nSize = m_UserCandleSetArray.GetSize();

	CString strData, strSetup;

	strData = "VERSION=1.0\n";
	file.WriteString(strData);

	for(int nIndex = 0; nIndex < nSize; nIndex++)
	{
		CANDLE_SET* pSet = m_UserCandleSetArray.GetAt(nIndex);
		
		strData.Empty();
		for(int nCandle = 0; nCandle < MAX_CANDLE; nCandle++)
		{
			CString strCANDLE;

			strCANDLE.Format("%1d%3d%3d%3d%3d", pSet->cd[nCandle].bEnable,
												pSet->cd[nCandle].nOpen,
												pSet->cd[nCandle].nHigh,
												pSet->cd[nCandle].nLow,
												pSet->cd[nCandle].nClose);
			strData += strCANDLE;
		}

		if(pSet->nApplyBodyLength == 0)
		{
			pSet->nBodyRate[0] = pSet->nBodyRate[1]= pSet->nBodyRate[2]= pSet->nBodyRate[3]= pSet->nBodyRate[4] = 5;
			pSet->nBodyRateDir[0] = pSet->nBodyRateDir[1]= pSet->nBodyRateDir[2]= pSet->nBodyRateDir[3]= pSet->nBodyRateDir[4] = 1;
		}

		// 설정사항
		strSetup.Format("%1d%1d%02d%d", pSet->nIgnoreTail, pSet->nIgnoreColor, pSet->nPreTrend, pSet->nApplyBodyLength);
		strData += strSetup;
		strSetup.Format("%02d%02d%02d%02d%02d", pSet->nBodyRate[0], pSet->nBodyRate[1], pSet->nBodyRate[2], pSet->nBodyRate[3], pSet->nBodyRate[4]);
		strData += strSetup;
		strSetup.Format("%1d%1d%1d%1d%1d", pSet->nBodyRateDir[0], pSet->nBodyRateDir[1], pSet->nBodyRateDir[2], pSet->nBodyRateDir[3], pSet->nBodyRateDir[4]);
		strData += strSetup;

		strData += pSet->szCandleName;
		strData.TrimRight();
		strData += "\n";

		file.WriteString(strData);
	}
	
	file.Close();
}

void CMainFrmWnd::MakeCandleImageList()
{
	CRect rcBorder(0,0,THUMB_WIDTH,THUMB_HEIGHT);

	int nCount = m_CandleSetArray.GetSize();

	if( !m_ImageListItem.m_hImageList)
	{
		if ( !m_ImageListItem.Create(THUMB_WIDTH, THUMB_HEIGHT, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			SetMessageBox("Can not Find bmp file");			
		}		
	}

	CClientDC cdc(this);
	CDC	memDC;
	memDC.CreateCompatibleDC(&cdc);


	for(int i = 0; i < nCount; i++)
	{
		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(&cdc, THUMB_WIDTH, THUMB_HEIGHT);


		CANDLE_SET* pSet = m_CandleSetArray.GetAt(i);
		MakeCandleBitmap(&memDC, &bitmap, rcBorder, pSet);
		
		// Attach to Bitmap and Replace image in CImageList
		//m_ImageListItem.Replace(i, &bitmap, NULL);
		m_ImageListItem.Add(&bitmap, RGB(255,0,255));
		
		// Release Object
		bitmap.DeleteObject();
		memDC.SelectClipRgn(NULL);
	}

	//Release used DC
	memDC.DeleteDC();
}

void CMainFrmWnd::MakeUserCandleImageList()
{
	CRect rcBorder(0,0,THUMB_WIDTH,THUMB_HEIGHT);

	int nCount = m_UserCandleSetArray.GetSize();

	if( !m_ImageUserListItem.m_hImageList)
	{
		if ( !m_ImageUserListItem.Create(THUMB_WIDTH, THUMB_HEIGHT, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			SetMessageBox("Can not Find bmp file");			
		}		
	}

	CClientDC cdc(this);
	CDC	memDC;
	memDC.CreateCompatibleDC(&cdc);

	// make Candle bitmaps
	for(int i = 0; i < nCount; i++)
	{
		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(&cdc, THUMB_WIDTH, THUMB_HEIGHT);


		CANDLE_SET* pSet = m_UserCandleSetArray.GetAt(i);
		MakeCandleBitmap(&memDC, &bitmap, rcBorder, pSet);
		
		// Attach to Bitmap and Replace image in CImageList
		//m_ImageUserListItem.Replace(i, &bitmap, NULL);
		m_ImageUserListItem.Add(&bitmap, RGB(255,0,255));

		// Release Object
		bitmap.DeleteObject();
		memDC.SelectClipRgn(NULL);
	}

	//Release used DC
	memDC.DeleteDC();
}

void CMainFrmWnd::MakeCandleBitmap(CDC* pDC, CBitmap* bitmap, CRect rcBorder, CANDLE_SET* CandleSet)
{
	CBitmap *pOldBitmap;
	pOldBitmap = (CBitmap*)pDC->SelectObject(bitmap);

	// fill background
	pDC->FillSolidRect(rcBorder, RGB(255,255,255));

// 	// create roundrect region
// 	CRgn rgn;
// 	rgn.CreateRoundRectRgn(0,0, rcBorder.Width(), rcBorder.Height(), 4, 4);
// 	pDC->SelectClipRgn(&rgn);
// 	rgn.DeleteObject();
// 
// 	// draw gradient pattern backgnd
// 	m_CandleMgr.DrawGradientRect(pDC, rcBorder, RGB(63,132,169), RGB(120,186,218));//, RGB(94,172,210));

	if(!m_ImageBK.m_hImageList)
		pDC->Draw3dRect(&rcBorder, RGB(153,153,153), RGB(153,153,153));
	else
		m_ImageBK.Draw(pDC, 0, CPoint(0,0), ILD_NORMAL);

	// Draw Image
	DrawCandle(pDC, THUMB_WIDTH, THUMB_HEIGHT, CandleSet);
	
	// Draw Border
	pDC->SelectObject(pOldBitmap);
}

void CMainFrmWnd::DrawCandle(CDC* pDC, int cx, int cy, CANDLE_SET* CandleSet)
{
	CRect rcClient(0, 0, cx, cy);

	// Min, Max값 결정
	int nMax = INT_MIN, nMin = INT_MAX;
	int nCnt = 0;
	BOOL bFindEnable = FALSE;
	for(int i = 0; i < MAX_CANDLE; i++)
	{
		if( ! CandleSet->cd[i].bEnable)
		{
			if(!bFindEnable)
				continue;
		}
			
		nCnt++;
		bFindEnable = TRUE;

		//if( ! CandleSet->cd[i].bEnable) continue;
		
		if(CandleSet->cd[i].nHigh > nMax) nMax = CandleSet->cd[i].nHigh;	
		if(CandleSet->cd[i].nLow < nMin) nMin = CandleSet->cd[i].nLow;	
	}

	rcClient.DeflateRect(2,3,2,18);
	double dUnitPerPercent = (double)rcClient.Height()/ (double)(nMax - nMin);

	int nSpace = 2;
	int	nRate[5] = { 3,4,4,4,5};

	int nHigh, nLow, nClose, nOpen ;
	int nBongWidth = rcClient.Width() / nRate[nCnt-1] - 3;
	int nStartX;
	nStartX = rcClient.left + (rcClient.Width() - (nBongWidth + 3)*nCnt) / 2 + 1;

	bFindEnable = FALSE;
	int nBongIndex = 0;
	for(int nIndex = 0; nIndex < MAX_CANDLE; nIndex++)
	{
		if( ! CandleSet->cd[nIndex].bEnable && !bFindEnable) continue;
		
		bFindEnable = TRUE;

		nOpen = CandleSet->cd[nIndex].nOpen;
		nHigh = CandleSet->cd[nIndex].nHigh;
		nLow = CandleSet->cd[nIndex].nLow;
		nClose = CandleSet->cd[nIndex].nClose;

		int nAdd = 2; // 보정치
		if(nOpen < nClose && nOpen == nLow) nAdd = 0;
		else if(nOpen > nClose && nClose == nLow) nAdd = 0;
		
		COLORREF clrBong = RGB(255,0,0);

		if(nOpen > nClose) 
		{
			int nTmp = nOpen;
			nOpen = nClose;
			nClose = nTmp;
			clrBong = RGB(0,0,255);
		}
		else if(nOpen == nClose)
			clrBong = RGB(50,50,50);

		if(! CandleSet->cd[nIndex].bEnable)
			clrBong = RGB(180,180,180);

		int nHighPos = rcClient.bottom - static_cast<int>((nHigh - nMin) * dUnitPerPercent);
		int nLowPos = rcClient.bottom - static_cast<int>((nLow - nMin) * dUnitPerPercent);

		CRect rcBong(nStartX + nBongIndex*(nBongWidth + 3), rcClient.bottom - static_cast<int>((nClose - nMin)*dUnitPerPercent),
					nStartX + nBongWidth + nBongIndex*(nBongWidth + 3), rcClient.bottom - static_cast<int>((nOpen - nMin)*dUnitPerPercent));

		pDC->FillSolidRect(rcBong, clrBong);

		CPen penLine(PS_SOLID, 1, clrBong), *pOldPen;
		pOldPen = pDC->SelectObject(&penLine);
		
		pDC->MoveTo(rcBong.TopLeft());
		pDC->LineTo(rcBong.right, rcBong.top);
		pDC->LineTo(rcBong.BottomRight());
		pDC->LineTo(rcBong.left, rcBong.bottom);
		pDC->LineTo(rcBong.TopLeft());

		pDC->MoveTo(rcBong.CenterPoint().x, nHighPos);

		pDC->LineTo(rcBong.CenterPoint().x, nLowPos+nAdd);
		pDC->SelectObject(pOldPen);

		CRect rcText(0,THUMB_HEIGHT-16,THUMB_WIDTH,THUMB_HEIGHT);
		rcText.DeflateRect(4,1,3,1);
		rcText.OffsetRect(0, 1);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(80,80,80));//

		int nLen = strlen(CandleSet->szCandleName);
		CFont* pOldFont = pDC->SelectObject(&m_fontDefault);//(nLen>8)? &m_fontSmall:
		DWORD dwFormat = DT_SINGLELINE;
		dwFormat |= (nLen <= 8) ? DT_CENTER:DT_LEFT;
		CString strTitle = GetThumbTitle(CandleSet->szCandleName);
		pDC->DrawText(strTitle, rcText, dwFormat);

		pDC->SelectObject(pOldFont);

		nBongIndex++;
	}
}

// 사용자 캔들을 이미지리스트와 사용자캔들리스트()에서 삭제
LONG CMainFrmWnd::OnDeleteUserCandle(WPARAM wParam, LPARAM lParam)
{
	m_ctrlUserListItem.m_ImageList.Remove(wParam);
	m_ctrlUserListItem.Refresh();

	CANDLE_SET* pSet = m_UserCandleSetArray.GetAt(wParam);
	delete pSet;
	pSet = NULL;
	m_UserCandleSetArray.RemoveAt(wParam);
	
	return 0;
}

// 캔들패턴 설명 대화상자를 띄운다.
LONG CMainFrmWnd::OnHelpHtml(WPARAM wParam, LPARAM lParam)
{
	int nIndex = (int)wParam;

	if(nIndex >= 0)
	{
		HINSTANCE hInstResourceClient = AfxGetResourceHandle();
		AfxSetResourceHandle(g_hInstance);

		CDlgHtml dlghtml;
		CString strURL = "", strPatternMane = GetCandlePatternName(nIndex);

		if(!strPatternMane.IsEmpty())
		{
			strURL.Format("%s\\Chart\\data\\Candle\\%s.html", m_strAppPath, strPatternMane);

			dlghtml.SetURL(strURL);

			if(dlghtml.DoModal() == IDOK)
			{
			}
		}

		AfxSetResourceHandle(g_hInstance);
	}

	return 0;
}

CString CMainFrmWnd::GetCandlePatternName(int nIndex)
{
	CString strCandlePatternName;
	strCandlePatternName =_T("");

	// Get Pattern Name
	switch (nIndex)
	{
	case 0:		strCandlePatternName = "교수형";		break;
	case 1:		strCandlePatternName = "유성형";		break;
	case 2:		strCandlePatternName = "상승샅바형";		break;
	case 3:		strCandlePatternName = "하락샅바형";		break;
	case 4:		strCandlePatternName = "장대양봉";		break;
	case 5:		strCandlePatternName = "장대음봉";		break;
// 	case 6:		strCandlePatternName = "교수형";		break;
// 	case 7:		strCandlePatternName = "교수형";		break;
// 	case 8:		strCandlePatternName = "교수형";		break;
// 	case 9:		strCandlePatternName = "교수형";		break;
// 	case 10:	strCandlePatternName = "교수형";		break;
// 	case 11:	strCandlePatternName = "교수형";		break;
// 	case 12:	strCandlePatternName = "교수형";		break;
// 	case 13:	strCandlePatternName = "교수형";		break;
// 	case 14:	strCandlePatternName = "교수형";		break;
// 	case 15:	strCandlePatternName = "교수형";		break;
// 	case 16:	strCandlePatternName = "교수형";		break;
// 	case 17:	strCandlePatternName = "교수형";		break;
// 
	}

	return	strCandlePatternName;
}

// 이미지 리스트컨트롤에서 툴팁 텍스트 요청 메시지 처리
LONG CMainFrmWnd::OnGetTipText(WPARAM wParam, LPARAM lParam)
{
	int nImageIndex = HIWORD(wParam);
	int nID = LOWORD(wParam);
	char* szTipText = (char*)lParam;
	memset(szTipText, 0x00, 256);
	CString strTipText;

	if(nID == IDC_ITEM_LIST)
	{
		CANDLE_SET* pSet = m_CandleSetArray.GetAt(nImageIndex);
		strTipText = pSet->szCandleName;
		int nSize = min(255, strTipText.GetLength());
		sprintf(szTipText, "%*.*s", nSize, nSize, strTipText);

		return 1;
	}
	else if(nID == IDC_ITEM_USERLIST)
	{
		CANDLE_SET* pSet = m_UserCandleSetArray.GetAt(nImageIndex);
		strTipText = pSet->szCandleName;
		int nSize = min(255, strTipText.GetLength());
		sprintf(szTipText, "%*.*s", nSize, nSize, strTipText);

		return 1;
	}


	return 0;
}

// 사용자 패턴을 저장하는 디렉토리가 존재하는지 파악하고
// 디렉토리가 없을 경우 미리 생성한다.
void CMainFrmWnd::CheckUserPatternDir()
{
	CFileFind cFind;
	BOOL bRead = TRUE;

	CString strUserFile;
	//20100217 SJ_KIM
	CString strUserID;// = KSLink_GetFileHeader();
	//m_strUserPatternPath.Format("%s\\User\\%s\\UserPtrn", m_strAppPath, strUserID);
	m_strUserPatternPath = GetFilePath("USERCONDFILE") + "\\finddata\\UserPtrn";

	BOOL bFind = FALSE;
	bRead = cFind.FindFile(m_strUserPatternPath);
	while(bRead)
	{
		bRead = cFind.FindNextFile();

		if(cFind.IsDots())  continue;
		if (cFind.IsDirectory())
		{
			bFind = TRUE;
			break;
		}
	}

	if(bFind == FALSE)
	{
		::CreateDirectory(m_strUserPatternPath, NULL);
	}
}

void CMainFrmWnd::OnEdChangeBongCnt()
{
	CString strCount;
	m_editBongCnt.GetWindowText(strCount);

	int nGap = atoi(strCount);

	if(nGap > MAX_BASEBONG)
	{
		strCount.Format("%d", (int)MAX_BASEBONG);
		m_editBongCnt.SetWindowText(strCount);
	}
	else
	{
		strCount.Format("%d", nGap);
		m_editBongCnt.SetWindowText(strCount);
	}

	m_editBongCnt.SetSel(strCount.GetLength(), -1);
}

void CMainFrmWnd::OnEdChangeRate1()
{
	EditChangeRate(0);
}

void CMainFrmWnd::OnEdChangeRate2()
{
	EditChangeRate(1);
}

void CMainFrmWnd::OnEdChangeRate3()
{
	EditChangeRate(2);
}

void CMainFrmWnd::OnEdChangeRate4()
{
	EditChangeRate(3);
}

void CMainFrmWnd::OnEdChangeRate5()
{
	EditChangeRate(4);
}

void CMainFrmWnd::OnKillfocusBongCnt()
{
	CString strCount;
	m_editBongCnt.GetWindowText(strCount);

	int nGap = atoi(strCount);

	if(nGap > MAX_BASEBONG)
	{
		strCount.Format("%d", (int)MAX_BASEBONG);
		m_editBongCnt.SetWindowText(strCount);
	}
	else
	{
		strCount.Format("%d", nGap);
		m_editBongCnt.SetWindowText(strCount);
	}

	m_editBongCnt.SetSel(strCount.GetLength(), -1);
}

void CMainFrmWnd::OnKillfocusRate1()
{
	EditChangeRate(0);
}

void CMainFrmWnd::OnKillfocusRate2()
{
	EditChangeRate(1);
}

void CMainFrmWnd::OnKillfocusRate3()
{
	EditChangeRate(2);
}

void CMainFrmWnd::OnKillfocusRate4()
{
	EditChangeRate(3);
}

void CMainFrmWnd::OnKillfocusRate5()
{
	EditChangeRate(4);
}

void CMainFrmWnd::EditChangeRate(int nIndex)
{
	CString strRate;
	m_editUpdownRate[nIndex].GetWindowText(strRate);

	int nGap = atoi(strRate);

	if(nGap > MAX_UPDOWNRATE)
	{
		strRate.Format("%d", (int)MAX_UPDOWNRATE);
		m_editUpdownRate[nIndex].SetWindowText(strRate);
	}
	else
	{
		strRate.Format("%d", nGap);
		m_editUpdownRate[nIndex].SetWindowText(strRate);
	}

	m_editUpdownRate[nIndex].SetSel(strRate.GetLength(), -1);
}



LRESULT CMainFrmWnd::OnNotify_CandlePatten(WPARAM wParam, LPARAM lParam)
{
	CString strData;
	if (wParam == NM_CLICK)
	{
		m_strCaption = "캔들패턴 그리기";
		CRect rcText = m_reTopCandle;
		rcText.DeflateRect(5, 2);
		InvalidateRect(&rcText);
	}
	return 1L;
}

// 
CString CMainFrmWnd::GetThumbTitle(CString strTitle)
{
	if(strTitle.GetLength() < 8) return strTitle;

	if(IsDBCSLeadByte(strTitle[7]))
		return strTitle.Left(8);

	return strTitle;
}

void CMainFrmWnd::GetTargetBaseType(int &nTargetBaseType_0st, int &nTargetBaseType_1st)
{
	if (m_strTargetBaseType.GetLength() == 0)
	{
		nTargetBaseType_0st = -1;
		nTargetBaseType_1st = -1;
	}
	else
	{
		// 0:전체, 1:코스피, 2:코스닥, 3:관심
		nTargetBaseType_0st = atoi(m_strTargetBaseType.Left(1));
		
		if (m_strTargetBaseType.GetLength() == 2)
			nTargetBaseType_1st = atoi(m_strTargetBaseType.Right(1));
		else
			nTargetBaseType_1st = -1;
	}
}

CString CMainFrmWnd::SetTargetBaseType(int nTargetBaseType_0st, int nTargetBaseType_1st)
{
	m_strTargetBaseType.Format("%d%d", nTargetBaseType_0st, nTargetBaseType_1st);
	
	if (nTargetBaseType_1st == -1)
		m_saCodeGroup.RemoveAll();
	
	return m_strTargetBaseType;
}

void CMainFrmWnd::OnChgComboTarget1st()
{
	int nTargetBaseType_0st, nTargetBaseType_1st;
	GetTargetBaseType(nTargetBaseType_0st, nTargetBaseType_1st);
	
	int nIndex = m_comboTarget_1st.GetCurSel();

	if (nIndex == 0)
	{
		m_btnReverseChk.EnableWindow(FALSE);
		m_btnReverseChk.SetCheck(FALSE);

		m_comboTarget_2nd.ResetContent();
		m_comboTarget_2nd.EnableWindow(FALSE);
		SetTargetBaseType(0);

		m_btnReverseChk.EnableWindow(FALSE);
		m_btnReverseChk.SetCheck(FALSE);
	}
	else
	{
		m_btnReverseChk.EnableWindow(TRUE);
		
		m_comboTarget_2nd.EnableWindow(TRUE);

		CString strTargetBaseType = SetTargetBaseType(nIndex, 0);

		if (SetLoad_TargetBase_2nd(strTargetBaseType) == TRUE)
			OnChgComboTarget2nd();
	}

}

void CMainFrmWnd::OnChgComboTarget2nd()
{
	CString strCode, strName;
	int nTargetBase_2nd = m_comboTarget_2nd.GetCurSel();
	
	if(nTargetBase_2nd < 0)
		return;
	
	DWORD lCode;
	lCode = m_comboTarget_2nd.GetItemData(nTargetBase_2nd);
	//strCode = (LPCTSTR)(DWORD)m_comboTarget_2nd.GetItemData(nTargetBase_2nd);
	m_comboTarget_2nd.GetLBText(m_comboTarget_2nd.GetCurSel(), strName);
	
	int nTargetBaseType_0st, nTargetBaseType_1st;
	GetTargetBaseType(nTargetBaseType_0st, nTargetBaseType_1st);
	
	//	m_saCodeGroup.RemoveAll();
	if (nTargetBaseType_0st == 0)		//# 전체
	{
		m_comboTarget_2nd.ResetContent();
		m_comboTarget_2nd.EnableWindow(FALSE);
	}
	else if (nTargetBaseType_0st == 1)		//# KOSPI
	{
		m_nMarket = GETMARKET_KOSPI;
		//strCode.Format("%03ld", lCode);
		GetData_UpJongToCode(lCode, strName);
	}
	else if (nTargetBaseType_0st == 2)		//# KOSDAQ
	{
		m_nMarket = GETMARKET_KOSDAQ;
		//strCode.Format("%03ld", lCode);
		GetData_UpJongToCode(lCode, strName);
	}
// 	else if (nTargetBaseType_0st == 3)	//# 테마
// 	{
// 		strCode.Format("%04ld", lCode);
// 		GetData_ThemaToCode(strCode);
// 	}
	else if (nTargetBaseType_0st == 3)		//# 관심
	{
		strCode.Format("%02ld", lCode);
		GetData_PortGroupToCode(strCode, &m_saCodeGroup);
	}
	
}

void CMainFrmWnd::OnBtnDay()
{
	m_strChartTerm = "2";
	m_comboTime.EnableWindow(FALSE);
	m_comboTime.Invalidate();

	m_btnDay.SetCheck(TRUE);
	m_btnWeek.SetCheck(FALSE);
	m_btnMonth.SetCheck(FALSE);
	m_btnMin.SetCheck(FALSE);

	OnBtnSearch();
}

void CMainFrmWnd::OnBtnWeek()
{
	m_strChartTerm = "3";
	m_comboTime.EnableWindow(FALSE);
	m_comboTime.Invalidate();

	m_btnDay.SetCheck(FALSE);
	m_btnWeek.SetCheck(TRUE);
	m_btnMonth.SetCheck(FALSE);
	m_btnMin.SetCheck(FALSE);

	OnBtnSearch();
}

void CMainFrmWnd::OnBtnMon()
{
	m_strChartTerm = "4";
	m_comboTime.EnableWindow(FALSE);
	m_comboTime.Invalidate();

	m_btnDay.SetCheck(FALSE);
	m_btnWeek.SetCheck(FALSE);
	m_btnMonth.SetCheck(TRUE);
	m_btnMin.SetCheck(FALSE);

	OnBtnSearch();
}

void CMainFrmWnd::OnBtnMin()
{
	m_strChartTerm = "1";
	m_comboTime.EnableWindow(TRUE);
	m_comboTime.Invalidate();

	m_btnDay.SetCheck(FALSE);
	m_btnWeek.SetCheck(FALSE);
	m_btnMonth.SetCheck(FALSE);
	m_btnMin.SetCheck(TRUE);

	OnBtnSearch();
}

void CMainFrmWnd::OnChgTimeCombo()
{
	
}

void CMainFrmWnd::OnBtnChkBodyRate()
{
	BOOL bShow = m_btnBodyRateChk.GetCheck() == 1;
	for(int i = 0; i < MAX_CANDLE; i++)
	{
		m_editUpdownRate[i].ShowWindow(bShow);
		m_ctrlPersent[i].ShowWindow(bShow);
		m_btnUpdown[i].ShowWindow(bShow);
	}


	MoveControls();
}

void CMainFrmWnd::OnBtnSave()
{
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	CDlgSaveCandle dlg(this);

	if(dlg.DoModal() == IDOK)
	{
		CANDLE_SET* pSet = NULL;
		CString strCandleName = dlg.m_strCandleName;
		BOOL bSameNameFind = FALSE;
		int nImageIndex = -1;
		
		// 같은 이름 검색
		for(int i = 0; i < m_UserCandleSetArray.GetSize(); i++)
		{
			CANDLE_SET* pCandleSet = m_UserCandleSetArray.GetAt(i);

			if(strCandleName == pCandleSet->szCandleName)
			{
				bSameNameFind = TRUE;
				pSet = pCandleSet;
				nImageIndex = i;
				break;
			}
		}

		int nRes = IDYES;
		if(bSameNameFind)
		{
			SetMessageBox("같은 캔들패턴 이름이 존재합니다. \n\n계속 진행 하시겠습니까?", PROGRAM_TITLE, MB_YESNO);
		}
		else
		{
			pSet = new CANDLE_SET;
			memset(pSet, 0, sizeof(CANDLE_SET));
			memcpy(pSet->szCandleName, strCandleName, min(strCandleName.GetLength(), 255));
		}

		if(nRes == IDYES)
		{
			m_CandleMgr.GetCandleSet(pSet);
			pSet->nIgnoreColor = m_btnIgnoreColorChk.GetCheck();
			pSet->nIgnoreTail = m_comboTail.GetCurSel();
			pSet->nPreTrend = 0; //m_comboPrevTrend.GetCurSel();  추세무관으로 고정
			pSet->nApplyBodyLength = m_btnBodyRateChk.GetCheck();
			if(!bSameNameFind) m_UserCandleSetArray.Add(pSet);
			if(pSet->nApplyBodyLength)
			{
				CString strVal;

				for(int i = 0; i < MAX_CANDLE; i++)
				{
					m_editUpdownRate[i].GetWindowText(strVal);
					pSet->nBodyRate[i] = atoi(strVal);
					pSet->nBodyRateDir[i] = m_bUpdownState[i];
				}
			}

			// Make bitmap
			CRect rcBorder(0,0,THUMB_WIDTH,THUMB_HEIGHT);

			CClientDC cdc(this);
			CDC	memDC;
			memDC.CreateCompatibleDC(&cdc);

			CBitmap bitmap;
 			bitmap.CreateCompatibleBitmap(&cdc, THUMB_WIDTH, THUMB_HEIGHT);

			MakeCandleBitmap(&memDC, &bitmap, rcBorder, pSet);
			
			// add bitmap to imagelist
			if(!bSameNameFind)
				m_ctrlUserListItem.m_ImageList.Add(&bitmap, RGB(255,0,255));
			else
				m_ctrlUserListItem.m_ImageList.Replace(nImageIndex, &bitmap, NULL);

			bitmap.DeleteObject();

			// refresh list window
			m_ctrlUserListItem.Refresh();

			m_ptabCondition->SetCurSel(1);
			m_ctrlListItem.ShowWindow(SW_HIDE);

			m_ctrlUserListItem.ShowWindow(SW_SHOW);
			m_ctrlUserListItem.BringWindowToTop();
			m_ctrlUserListItem.Invalidate();

			// refresh pattern title
			m_strCaption = strCandleName;
			CRect rcText = m_reTopCandle;
			rcText.DeflateRect(5, 2);
			InvalidateRect(rcText);
		}
	}
	
	AfxSetResourceHandle(hInstSave);

}

void CMainFrmWnd::OnBtnLoad()
{
// 	TCHAR szFilters[] = _T("Patten Draw Data Files (*.pdd)|*.pdd||");
// 	
// 	CFileDialog FileDlg (TRUE, _T("pdd"), _T("*.pdd"),   OFN_FILEMUSTEXIST, szFilters);
// 	if (FileDlg.DoModal() == IDOK)
// 	{
// 		CString strFilePath;
// 		strFilePath = FileDlg.GetPathName();
// 		m_pctrlDrawPatten->GetLoad_DrawData(strFilePath);
// 		
// 		OnBtnSearch();
// 	}
}

void CMainFrmWnd::OnBtnReset()
{
	m_CandleMgr.Reset();

	CString strTip;
	for(int i = 0; i < MAX_CANDLE; i++)
	{
		m_editUpdownRate[i].SetWindowText(DEFAULT_BODYRATE_STR);
		m_btnUpdown[i].SetWindowText("↑");
		strTip = "이상";
	}

	m_btnBodyRateChk.SetCheck(FALSE);
	OnBtnChkBodyRate();

	// refresh pattern title
	m_strCaption = "캔들패턴 그리기";
	CRect rcText = m_reTopCandle;
	rcText.DeflateRect(5, 2);
	InvalidateRect(&rcText);

}

void CMainFrmWnd::OnBtnSearch()
{
	CANDLE_IN	stCandleIn;

	if (SetMakePacket(stCandleIn) == TRUE)
	{
	//	SetBtyeOrdering_Packet(stCandleIn);

 		HKST03660000_IN1* pstHKST03660000_IN;
		pstHKST03660000_IN = new HKST03660000_IN1;
		SetConvert_SendPatternPacket(stCandleIn, pstHKST03660000_IN);

		char szData[sizeof(HKST03660000_IN1) + 1];
		memset(szData, 0x00, sizeof(szData));
		memcpy((void *)szData, (void *)pstHKST03660000_IN, sizeof(HKST03660000_IN1));
	
		int nRet = Module_SendTr((CString)TRNO_CANDLE_RESULT, sizeof(HKST03660000_IN1), (BYTE *)szData);

		delete pstHKST03660000_IN;
		
		SetStateTitleBar("[조회중]");

		SetEnable(FALSE);
	}

	return;
}

long CMainFrmWnd::GetSearchTimeTerm()
{
	long nResult = 0;
	if (atoi(m_strChartTerm) == 0)		//# 틱
	{
		
	}
	else if (atoi(m_strChartTerm) == 1)  //# 분
	{
		//   30초", "1분", "3분","5분","10분","30분" ,"60분"};
		//# 3:1분, 4:3분, 5:5분, 6:10분, 7:15분, 8:30분, 9:60분
		nResult = LIST_TIME_INDEX[m_comboTime.GetCurSel()];
	}
	else if (atoi(m_strChartTerm) == 2) //# 일
		nResult = 0;
	else if (atoi(m_strChartTerm) == 3)	//# 주
		nResult = 1;
	else if (atoi(m_strChartTerm) == 4)	//# 월
		nResult = 2;
	else if (atoi(m_strChartTerm) == 5)	//# 년
		nResult = -1;
	
	return nResult;
}

int CMainFrmWnd::GetBongGap()
{
	CString strCount;
	m_editBongCnt.GetWindowText(strCount);

	int nGap = atoi(strCount);

	int nSkipBong = m_CandleMgr.GetFirstEnableCandle();
	nGap = nGap + MAX_CANDLE - 1 - nSkipBong;

	return nGap;
}

int CMainFrmWnd::GetPreTrand()	// 이전 추세 구분 
{
	// 추세무시로 고정(20090618)
	return 0;
	//return m_comboPrevTrend.GetCurSel();
}

int CMainFrmWnd::GetIgnoreColor()	// 양음봉 무시
{
	return m_btnIgnoreColorChk.GetCheck();
}

int CMainFrmWnd::GetIgnoreTail()	// 꼬리 무시
{
	return m_comboTail.GetCurSel();
}

BOOL CMainFrmWnd::SetMakePacket(CANDLE_IN	&stCandleIn)
{
	BOOL bResult = FALSE;

	m_nTrIndex++;

	stCandleIn.nTrIndex = m_nTrIndex;

	stCandleIn.iS_Time = GetSearchTimeTerm();
	stCandleIn.iS_Range = m_CandleMgr.GetRange();

	if(stCandleIn.iS_Range == 0)
	{
		SetMessageBox("선택된 봉이 없습니다.", PROGRAM_TITLE, MB_ICONWARNING | MB_OK);
		return FALSE;
	}

	//# 유사도
	stCandleIn.iSimilar = 0; // not used yet

    stCandleIn.iPreGap = GetBongGap();		// 기준 봉( X봉전 Data 조회)
    stCandleIn.iPreTrand = GetPreTrand();	// 이전 추세 구분 

    stCandleIn.iIgnoreColor = GetIgnoreColor();		// 양음봉 구분 무시
	stCandleIn.iIgnoreTail = GetIgnoreTail();		// 꼬리 무시

	//# 필드설정
	stCandleIn.FieldEditerCnt = m_listField.GetCount();

	POSITION pos;
	int nIndex;
	HeadInfoType		stHeadInfoType;
	for (nIndex = 0; nIndex < m_listField.GetCount(); nIndex++)
	{
		pos = m_listField.FindIndex(nIndex);
		stHeadInfoType = m_listField.GetAt(pos);

		stCandleIn.FieldList[nIndex] = stHeadInfoType.lHeadID;
	}

	for(int nCndl = 0; nCndl < MAX_CANDLE; nCndl++)
		memset(&stCandleIn.tCandleData[nCndl], -1, sizeof(CANDLE_IN_DATA));
	m_CandleMgr.GetCandlePacketData(&stCandleIn.tCandleData[0]);

	stCandleIn.iApplyBodyRate = m_btnBodyRateChk.GetCheck();
	if(stCandleIn.iApplyBodyRate)
	{
		// 첫봉이 enable 상태가 아닐 때, 봉전 입력값을 바꾸고,
		// 캔들데이터를 첫봉이 enable인 봉이 오도록 shift한다.
		CString strRate;
		CANDLE_SET TmpSet;
		m_CandleMgr.GetCandleSet(&TmpSet);
		int nIndex = 0;
		BOOL bStartFill = FALSE;
		for(int i = MAX_CANDLE-1; i >= 0; i--)
		{
			if(TmpSet.cd[i].bEnable)
			{
				m_editUpdownRate[i].GetWindowText(strRate);
				stCandleIn.tCandleData[nIndex].iBodyRate = atoi(strRate) * (m_bUpdownState[i]?1:-1);

				bStartFill = TRUE;
				nIndex++;
			}
			else if(bStartFill && !TmpSet.cd[i].bEnable)
			{
				stCandleIn.tCandleData[nIndex].iBodyRate = -1;
				nIndex++;
			}
		}
	}

	//# 0:전체, 1:Kospi, 2:Kosdak
	//# 1st -> 0:전체, 1:업종, 2:테마, 3:관심, 4:유니버스
	int nTargetBaseType_0st, nTargetBaseType_1st;
	GetTargetBaseType(nTargetBaseType_0st, nTargetBaseType_1st);

	BOOL bReverse = FALSE;
	if (m_btnReverseChk.IsWindowEnabled() == TRUE)
		bReverse = m_btnReverseChk.GetCheck();

	CString strCode, strCodeCompare;
	if (nTargetBaseType_0st == 0) // 전체
	{
		memset(&stCandleIn.sTargetBit, 0x31, sizeof(stCandleIn.sTargetBit));
	}
	else if (nTargetBaseType_0st == 1 || nTargetBaseType_0st == 2) // 코스피, 코스닥
	{
		if (bReverse == TRUE)
			memset(&stCandleIn.sTargetBit, 0x31, sizeof(stCandleIn.sTargetBit));
		else
			memset(&stCandleIn.sTargetBit, 0x30, sizeof(stCandleIn.sTargetBit));

		for (int nIndex_Code = 0; nIndex_Code < m_saCodeGroup.GetSize(); nIndex_Code++)
		{
			strCodeCompare = m_saCodeGroup.GetAt(nIndex_Code);
			for (nIndex = 0; nIndex < m_saCodeMST.GetSize(); nIndex++)
			{
				strCode = m_saCodeMST.GetAt(nIndex);
				if (strCodeCompare == strCode.Mid(1, 6))		//# Kospi
				{
					stCandleIn.sTargetBit[nIndex + (int)RESULT_MAX_OFFSET] = (m_btnReverseChk.GetCheck() == TRUE) ? '0':'1';
					break;
				}
			}
		}
	}
	else if (nTargetBaseType_0st == 3 || nTargetBaseType_0st == 4)// 테마, 관심
	{
		if (bReverse == TRUE)
			memset(&stCandleIn.sTargetBit, 0x31, sizeof(stCandleIn.sTargetBit));
		else
			memset(&stCandleIn.sTargetBit, 0x30, sizeof(stCandleIn.sTargetBit));

		for (int nIndex_Code = 0; nIndex_Code < m_saCodeGroup.GetSize(); nIndex_Code++)
		{
			strCodeCompare = m_saCodeGroup.GetAt(nIndex_Code);
			for (nIndex = 0; nIndex < m_saCodeMST.GetSize(); nIndex++)
			{
				strCode = m_saCodeMST.GetAt(nIndex);
				if (strCodeCompare == strCode.Mid(1, 6))
				{
					stCandleIn.sTargetBit[nIndex + (int)RESULT_MAX_OFFSET] = (m_btnReverseChk.GetCheck() == TRUE) ? '0':'1';
					break;
				}
			}
		}
	}
	return TRUE;
}

void CMainFrmWnd::SetBtyeOrdering_Packet(CANDLE_IN	&stCandleIn)
{
	int nCnt = stCandleIn.iS_Range;

	stCandleIn.nTrIndex = ntohl(stCandleIn.nTrIndex);
    
	stCandleIn.iS_Time = ntohl(stCandleIn.iS_Time);
	stCandleIn.iS_Range = ntohl(stCandleIn.iS_Range);           // 검색 범위
	stCandleIn.iSimilar = ntohl(stCandleIn.iSimilar);           // 유사도
    stCandleIn.iPreGap = ntohl(stCandleIn.iPreGap);				// 기준 봉( X봉전 Data 조회)
    stCandleIn.iPreTrand = ntohl(stCandleIn.iPreTrand);			// 이전 추세 구분 
    stCandleIn.iIgnoreColor = ntohl(stCandleIn.iIgnoreColor);	// 양음봉 무시 
    stCandleIn.iIgnoreTail = ntohl(stCandleIn.iIgnoreTail);		// 꼬리무시
    stCandleIn.iApplyBodyRate = ntohl(stCandleIn.iApplyBodyRate);// 몸통길이 적용
	
	int nIndex;
	for (nIndex = 0; nIndex < stCandleIn.FieldEditerCnt; nIndex++)
		stCandleIn.FieldList[nIndex] = ntohl(stCandleIn.FieldList[nIndex]);
	stCandleIn.FieldEditerCnt = ntohl(stCandleIn.FieldEditerCnt);		// 필드편집 개수   [Fix]
	
	
	for (nIndex = 0; nIndex < nCnt; nIndex++)
	{
		stCandleIn.tCandleData[nIndex].iHigh = ntohl(stCandleIn.tCandleData[nIndex].iHigh);
		stCandleIn.tCandleData[nIndex].iClose = ntohl(stCandleIn.tCandleData[nIndex].iClose);
		stCandleIn.tCandleData[nIndex].iOpen = ntohl(stCandleIn.tCandleData[nIndex].iOpen);
		stCandleIn.tCandleData[nIndex].iLow = ntohl(stCandleIn.tCandleData[nIndex].iLow);
		stCandleIn.tCandleData[nIndex].iBodyRate = ntohl(stCandleIn.tCandleData[nIndex].iBodyRate);
	}
}

// void CMainFrmWnd::OnBtnChkBong()
// {
// 	m_comboBongTerm.EnableWindow(m_btnBongChk.GetCheck());
// 	m_comboBongTerm.Invalidate();
// }
// 
// void CMainFrmWnd::OnChgBongCombo()
// {
// 	
// }
// 
// void CMainFrmWnd::OnChgSimCombo()
// {
// 	
// }

LRESULT CMainFrmWnd::OnNotify_Grid(WPARAM wParam, LPARAM lParam)
{
	CString strData;
	if (wParam == NM_GRID_CLK)
	{
		strData = *(CString*)lParam;		
		
		if (strData.IsEmpty() == FALSE)
		{
			int nPos_Find;
			nPos_Find = strData.Find("/");
			
			m_strCode = strData.Left(nPos_Find);
			m_strDateTime_Select = strData.Right(strData.GetLength() - (nPos_Find + 1));	
			//m_strDateTime_Select = m_strEndTime + m_strStartTime;
			
			//if(m_strCode_Select.GetLength()>0)
			{
				SendTr_ChartData(m_strCode, m_strDateTime_Select);
	
				if(m_bLink)
					SetSharedDataToCode(m_strCode);
				else
					m_bLink = TRUE;
			}
		}	
	}
	else if (wParam == NM_DBCLICK_L)
	{
		CString strCode = *(CString*)lParam;

		if (strCode.IsEmpty() == FALSE)
		{
			/*pView->OnDBClick(strCode);*/
		}
	}
	return 1L;
}

int CMainFrmWnd::Module_SendTr(CString szTrNo, int nTrLen, BYTE *TrData)
{
	CString strMsg;
	char szSaveBuf[8192] = { 0 };
	int nPos = 0;

	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	if(szTrNo.CompareNoCase(TRNO_CANDLE_RESULT)==0)
	{	
		pScreenWnd->SendTR(szTrNo, (char*)TrData, nTrLen, TRKEY_CANDLE_RESULT, US_KEY);
	}
	if(szTrNo.CompareNoCase(TRNO_MSTCODE)==0)
	{
		if(CheckSavedMstcode()) return 1;

		InMakeFid_Mstcode(szTrNo, nTrLen, TrData, szSaveBuf, nPos);
		pScreenWnd->SendTR(TRNO_MSTCODE, (char*)TrData, nTrLen, TRKEY_MSTCODE, US_KEY);
	}
	if(szTrNo.CompareNoCase(QUERY_STRJONGMOK)==0)
	{			
// 		CString strTrInfo;
// 		strTrInfo.Format("%s", TrData);
// 
// 		CString stInput;
// 		stInput = strTrInfo;
// 
// 		if (m_pPartnerMng)
// 		{
// 			CString szReqTr = szTrNo;			
// 			return m_pPartnerMng->SendConvData((LPCTSTR)szReqTr, (LPBYTE)(LPCTSTR)stInput, stInput.GetLength());
// 		}
	}

	return 1;
}

BOOL CMainFrmWnd::CheckSavedMstcode()
{
	// 기존에 저장된 데이터가 있는지 확인한다.
	// ::CTrCommSite::ReceiveData에서 저장을 했을 경우 다시 읽어온다.
	//#include "../../Contents/inc/IMainExInfoMng.h" 추가해야한다.
	long  rValue = NULL;
	int   rLen = 0;
// 	if(pMainExMng)
// 	{
// 		BOOL bSaved = (BOOL)pMainExMng->GetUserData("MSTCODE", rValue, rLen);
// 		if(!bSaved) return FALSE;
// 
// 		LPCSTR szTR = TRNO_MSTCODE;
// 		LPCSTR szMsgCode="00000";
// 		LPCSTR szMessage="조회완료";
// 
// 		ConvertMstData(szTR, (BYTE*)rValue, rLen);
// 		//ReceiveData(1, szTR, szMsgCode, szMessage, (BYTE*)rValue, rLen);
// 		return TRUE;
// 	}
	return FALSE;
}

void CMainFrmWnd::InMakeFid_Mstcode(LPCSTR szTrNo, int nTrLen, BYTE *pTrData, char* szSaveBuf, int &nPos)
{
	//1. 연속패킷, DRFN 연속키값 쓸필요있나???, 그련 space(80)
	memset(&szSaveBuf[nPos], 0x20, 80);
	nPos += 80;

	//2. TR값 문자열로 만든다.
	//	EU_p0621_InAll inAll;
	//	memset(&inAll, 0x20, sizeof(EU_p0621_InAll));
	//	int nSendTrDataSize = InMakeTr_Mstcode(pTrData, (BYTE*)&inAll);
	//	memcpy(&szSaveBuf[nPos], &inAll, nSendTrDataSize);
	//	nPos += nSendTrDataSize;

	//3. 레코드 번호 1부터 차례로 
	CString szOutRecord;

	// 1st Record
	szOutRecord.Format("%-16d", 1);
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	GetFillString(szOutRecord, 0, 0, 3);	// 0 ~ 4
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	memcpy(&szSaveBuf[nPos], "|", 1);	//구분자.
	nPos++;

	// 2st Record
	szOutRecord.Format("%-16d", 2);
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	GetFillString(szOutRecord, 0, 3, 3);	// 0 ~ 5
	memcpy(&szSaveBuf[nPos], szOutRecord, szOutRecord.GetLength());
	nPos += szOutRecord.GetLength();

	memcpy(&szSaveBuf[nPos], ";", 1);	//구분자.
	nPos++;
}

LRESULT CMainFrmWnd::OnReceiveTranData(WPARAM wParam, LPARAM lParam)
{
// 	CString strData = (LPCSTR)wParam;
// 	int nBuffSize = m_pPartnerMng->GetRecvLen(strData);
// 	if (nBuffSize <= 0)
// 		return 0L;
// 
// 	char *pszRecvData = new char[nBuffSize+1];
// 	memset(pszRecvData, 0x00, nBuffSize+1);
// 
// 	if (m_pPartnerMng->RecvConvData((LPCSTR)wParam, (LPBYTE)pszRecvData, nBuffSize))
// 		ReceiveData(0, (LPCSTR)wParam, "", "", (LPVOID)pszRecvData, (long)nBuffSize);
// 
// 	delete []pszRecvData;

	return 0L;
}

LRESULT CMainFrmWnd::OnReceiveDirectFID(WPARAM wParam, LPARAM lParam)
{
// 	LPITGBASE_HEADER lpITGBaseHead =(LPITGBASE_HEADER)wParam;
// 	LPITGUSER_HEADER lpITGUserHead =(LPITGUSER_HEADER)lParam;
// 
// 	CString strTR;
// 	strTR.Format(_T("%.*s"), sizeof(lpITGBaseHead->cTrcode), lpITGBaseHead->cTrcode);
// 
// 	if (!m_pPartnerMng)
// 		return 0L;
// 
// 	int nBuffSize = lpITGUserHead->nDataSize;
// 	LPVOID lpData = (LPVOID)lpITGUserHead->pData;
// 
// 	char *pszRecvData = NULL;
// 	long lSize = m_pPartnerMng->Receivedata(strTR, lpData, nBuffSize, &pszRecvData,	lpITGBaseHead->nRqid);
// 
// 	if (lSize > 0)
// 		ReceiveData(0, (LPCSTR)strTR, "", "", (LPVOID)pszRecvData, lSize);
// 
// 	if (pszRecvData)
// 		delete[] pszRecvData;
	return 1L;
}

#include <math.h>
BOOL CMainFrmWnd::SetConvert_SendPatternPacket(CANDLE_IN &stCandle_In, HKST03660000_IN1* pstHKST03660000_IN)
{

	memset(pstHKST03660000_IN, 0x20, sizeof(HKST03660000_IN1));

	char tmpVal[2600]={0,};
	CString szVal;

	CString strBuff;
	LONG2STR_BUF(tmpVal, 4, stCandle_In.nTrIndex, strBuff);
	memcpy(pstHKST03660000_IN->nTrIndex, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stCandle_In.iS_Time, strBuff);
	memcpy(pstHKST03660000_IN->iS_Time, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stCandle_In.iS_Range, strBuff);
	memcpy(pstHKST03660000_IN->iS_Range, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stCandle_In.iSimilar, strBuff);
	memcpy(pstHKST03660000_IN->iSimilar, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stCandle_In.FieldEditerCnt, strBuff);
 	memcpy(pstHKST03660000_IN->FieldEditerCnt, tmpVal, 4);


	int fieldIndex=0;
	int nIndex;
	memset(tmpVal, 0x20, RESULT_FIELD_MAX_EUGENE);
	for(nIndex = 0; fieldIndex < stCandle_In.FieldEditerCnt; nIndex+=4)
	{
		strBuff.Format("%ld", stCandle_In.FieldList[fieldIndex++]);
		memcpy(&tmpVal[nIndex], strBuff, strBuff.GetLength());
	}
 	memcpy(pstHKST03660000_IN->FieldList, tmpVal, RESULT_FIELD_MAX_EUGENE);	

// 	LONG2STR_BUF(tmpVal, 4, stCandle_In.FieldEditerCnt, strBuff);
// 	memcpy(pstHKST03660000_IN->FieldEditerCnt, "0000", 4);
// 	CString str;
// 	str.Format("%080d", 0);
// 	memcpy(pstHKST03660000_IN->FieldList, str, RESULT_FIELD_MAX_EUGENE);	





	LONG2STR_BUF(tmpVal, 4, stCandle_In.iPreGap, strBuff);
	memcpy(pstHKST03660000_IN->iPreGap, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stCandle_In.iPreTrand, strBuff);
	memcpy(pstHKST03660000_IN->iPreTrend, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stCandle_In.iIgnoreColor, strBuff);
	memcpy(pstHKST03660000_IN->iIgnoreColor, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stCandle_In.iIgnoreTail, strBuff);
	memcpy(pstHKST03660000_IN->iIgnoreTail, tmpVal, 4);

	/*LONG2STR_BUF(tmpVal, 4, stCandle_In.iApplyBodyRate, strBuff);*/
	char ch[4];
	itoa(stCandle_In.iApplyBodyRate, ch, 10);
	memcpy(&pstHKST03660000_IN->iApplyBodyRate, ch, 1);

	Safe_CheckCopy2(pstHKST03660000_IN->sTargetBit, stCandle_In.sTargetBit, RESULT_MAX, szVal);

	for(nIndex = 0; nIndex < COUNT_CANDLE_DATA; nIndex++)
	{	
		INT2STR_BUF(tmpVal, 8, stCandle_In.tCandleData[nIndex].iHigh, strBuff);
		memcpy(pstHKST03660000_IN->data[nIndex].iHigh, tmpVal, sizeof(pstHKST03660000_IN->data[nIndex].iHigh));

		INT2STR_BUF(tmpVal, 8, stCandle_In.tCandleData[nIndex].iClose, strBuff);
		memcpy(pstHKST03660000_IN->data[nIndex].iClose, tmpVal, sizeof(pstHKST03660000_IN->data[nIndex].iClose));

		INT2STR_BUF(tmpVal, 8, stCandle_In.tCandleData[nIndex].iOpen, strBuff);
		memcpy(pstHKST03660000_IN->data[nIndex].iOpen, tmpVal, sizeof(pstHKST03660000_IN->data[nIndex].iOpen));

		INT2STR_BUF(tmpVal, 8, stCandle_In.tCandleData[nIndex].iLow, strBuff);
		memcpy(pstHKST03660000_IN->data[nIndex].iLow, tmpVal, sizeof(pstHKST03660000_IN->data[nIndex].iLow));

		INT2STR_BUF(tmpVal, 8, stCandle_In.tCandleData[nIndex].iBodyRate, strBuff);
		memcpy(pstHKST03660000_IN->data[nIndex].iBodyRate, tmpVal, sizeof(pstHKST03660000_IN->data[nIndex].iBodyRate));
	}

	return TRUE;
}


void CMainFrmWnd::OnPF_Contorl_Move()
{
// 	ST_MOVEITEM stItem;	
// 	stItem.nType = PF_ITEM_BTN;
// 
// 	SET_ST_MOVEITEM(stItem, "btnSearch", &m_btnSearch);	
// 	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);
// 
// 	SET_ST_MOVEITEM(stItem, "btnPrint", &m_btnPrint);
// 	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);
// 
// 	SET_ST_MOVEITEM(stItem, "btnItemEdit", &m_btnFieldEdit);
// 	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);
// 
// 	SET_ST_MOVEITEM(stItem, "btnItemSend", &m_btnResultCtrl);
// 	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);	
// 
// 	SET_ST_MOVEITEM(stItem, "btnSave", &m_btnSave);
// 	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);	
// 
// 	SET_ST_MOVEITEM(stItem, "btnInit", &m_btnReset);
// 	SendMessage(MF_PFCONTORL_MOVE, PF_CONTROL_SHOW, (LPARAM)&stItem);

}

void CMainFrmWnd::PlatFormBtn_OnSearh()
{
	OnBtnSearch();
}

void CMainFrmWnd::PlatFormBtn_OnPrint()
{
	OnBtnPrint();
}

void CMainFrmWnd::PlatFormBtn_OnItemEdit()
{
	OnBtnFieldEdit();
}

void CMainFrmWnd::PlatFormBtn_OnItemSend()
{
	OnBtnResultCtrl();
}

void CMainFrmWnd::PlatFormBtn_OnSave()
{	
	OnBtnSave();
}

void CMainFrmWnd::PlatFormBtn_OnInit()
{
	OnBtnReset();
}


void CMainFrmWnd::ChangeSkinColor(COLORREF clrBkgnd, ST_TABCOLOR stTabColor)
{
	if(!clrBkgnd)
		return;

	m_clrMainSkin = clrBkgnd;

	m_stTabColor = stTabColor;

	if(m_brushBK.DeleteObject())
		m_brushBK.CreateSolidBrush(m_clrMainSkin);

	m_ptabCondition->SetTabColor(m_clrMainSkin, m_clrMainSkin, stTabColor.clrB, NULL, NULL, m_clrMainSkin);
	m_ptabCondition->SetTextColor(stTabColor.clrSF, stTabColor.clrSF);
	m_ptabCondition->Invalidate();
	
	ST_GRIDCOLOR stGridColor;
	stGridColor.clrGridHeader = GetAxColor(74);
	stGridColor.clrGridHilight = GetAxColor(78);
	
	ChangeGridColor(stGridColor);

	m_ctrlTarget.Invalidate();
	m_ctrlBongCntText1.Invalidate();
	m_ctrlBongCntText2.Invalidate();
	m_ctrlCodeCntText.Invalidate();
	m_ctrlCodeCnt.Invalidate();
	m_ctrlJongName.Invalidate();
	
	m_btnExceptionChk.Invalidate();
	m_btnReverseChk.Invalidate();
	m_btnIgnoreColorChk.Invalidate();
	m_btnBodyRateChk.Invalidate();
	
	m_comboTarget_2nd.Invalidate();
	m_comboTime.Invalidate();
	
	m_stcStatusBar.Invalidate();
	m_ctrlTime.Invalidate();
	m_ctrlNotice.Invalidate();
	m_ctrlState.Invalidate();

	Invalidate();
}

void CMainFrmWnd::ChangeGridColor(ST_GRIDCOLOR stGridColor)
{
	m_gridResult.ChangeGridColor(stGridColor);
	m_gridResult.Invalidate();
}

void CMainFrmWnd::GetTabClr(COLORREF* clrBkgnd, ST_TABCOLOR* stTabColor)
{
	*clrBkgnd = m_clrMainSkin;
	*stTabColor = m_stTabColor;
}

CString CMainFrmWnd::GetUserFilePath(CString strOnlyFileName)
{
	CString strFilePath = "";
	CString strUserPath;

	strUserPath.Format("%s%s%s", Variant(homeCC), USRDIR, Variant(nameCC));

	strFilePath = (CString)strUserPath + (CString)FOLDER_FINDDATA + "\\" + strOnlyFileName;
	return strFilePath;
}

CString CMainFrmWnd::GetFilePath(CString strLoadType)
{
	CString strFile, strFolder;
	CString strFilePath = "";
	CString strAppPath;

	BOOL bUserFolder = FALSE;
	BOOL bSetIndustry = TRUE;

	strFile = (CString)FINDERINFO_INIFILE;

	if ("USERCONDFILE" == strLoadType)
	{
		strFilePath.Format("%s%s%s", Variant(homeCC), USRDIR, Variant(nameCC));
	}
	else
	{
		GetDir_DrfnRoot(strAppPath);
		strFilePath = strAppPath + (CString)FOLDER_FIND + "\\" + strFile;
	}

	return strFilePath;
}

BOOL CMainFrmWnd::LookUpSearchCenterModify(CString& strType, CString& strUserFileName)
{
	CString strFileName = GetFilePath("USERCONDFILE") + "\\SearchCenterModify.xml";

	BOOL bResult = TRUE;
	VARIANT_BOOL bLoadedXMLFile;

	DOCUMENT pDOMConditionIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	pDOMConditionIndex->async = false;
	bLoadedXMLFile = pDOMConditionIndex->load(_bstr_t(strFileName));

	if(bLoadedXMLFile == VARIANT_FALSE) 
	{
		bResult = FALSE;
		return bResult;
	}
	try
	{
		XMLELEMENT pParent = pDOMConditionIndex->documentElement;

		NODE pUserItem = pParent->firstChild;
		ATTRIBUTE pAttr = pUserItem->attributes;

		int i;
		for(i = 0; i < pAttr->length; ++i)
		{
			if(_stricmp(pAttr->item[i]->nodeName, "TYPE") == 0)
				strType = (char*)pAttr->item[i]->text;
			else if(_stricmp(pAttr->item[i]->nodeName, "FULLNAME") == 0)
			{
				strUserFileName = (char*)pAttr->item[i]->text;
				if(strUserFileName.Left(1) == "\\")
					strUserFileName = strUserFileName.Mid(1);
			}
		}
	}
	catch(_com_error e)
	{
		bResult = FALSE;
	}

	return bResult;
}

void CMainFrmWnd::SearchUserItem(CString& strFullName)
{
	CANDLE_SET* pSet = NULL;
	
	int i;
	for(i = 0; i < m_UserCandleSetArray.GetSize(); ++i)
	{
		pSet = m_UserCandleSetArray.GetAt(i);
		if(pSet->szCandleName == strFullName)
			break;
	}

	if(!pSet)
		return;

	CString strText;

	m_CandleMgr.SetCandleSet(pSet);
	m_CandleMgr.SetFocusCandle(4);

	m_btnBodyRateChk.SetCheck(pSet->nApplyBodyLength);

	if(pSet->nApplyBodyLength == 0)
	{
		pSet->nBodyRate[0] = pSet->nBodyRate[1]= pSet->nBodyRate[2]= pSet->nBodyRate[3]= pSet->nBodyRate[4] = DEFAULT_BODYRATE;
		pSet->nBodyRateDir[0] = pSet->nBodyRateDir[1]= pSet->nBodyRateDir[2]= pSet->nBodyRateDir[3]= pSet->nBodyRateDir[4] = 1;
	}


	for(i = 0; i < MAX_CANDLE; i++)
	{
		if(!pSet->cd[i].bEnable) pSet->nBodyRate[i] = DEFAULT_BODYRATE;

		strText.Format("%d", pSet->nBodyRate[i]);
		m_editUpdownRate[i].SetWindowText(strText);

		if(pSet->nBodyRateDir[i] == 1)
		{
			m_btnUpdown[i].SetWindowText("↑");
			strText = "이상";
		}
		else
		{
			m_btnUpdown[i].SetWindowText("↓");
			strText = "이하";
		}
		
		m_bUpdownState[i] = pSet->nBodyRateDir[i];
	}
	OnBtnChkBodyRate();

	m_strCaption = pSet->szCandleName;
	CRect rcText = m_reTopCandle;
	rcText.DeflateRect(5, 2);
	InvalidateRect(rcText);

	OnBtnSearch();

	m_ptabCondition->SetCurSel(1);
	OnChangeTab(NULL, NULL);
}

BOOL CMainFrmWnd::SetTempGroupReg(CStringArray *psaCode, CUIntArray *iaMarkType)
{
	if (psaCode->GetSize() <= 0)
		return FALSE;

	CString strCode;

	return TRUE;
}

BOOL CMainFrmWnd::SetPortfolioReg(CString strOpenDlg, CStringArray *psaCode)
{
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
// 		//20100202 SJ_KIM
// 		pGwanMng->ShowGwansimDlgWithList(this, arrCode);	
// 	}
	return TRUE;
}

void CMainFrmWnd::SetSendSurfing(CStringArray *psaCode)
{
	if (SetPortfolioReg("0", psaCode) == TRUE)
	{
		SetSaveTempGroup(psaCode);
		SetSendMainMsg("돌려보기의 임시그룹으로 종목을 보냈습니다.");
	}
}

void CMainFrmWnd::SetSaveTempGroup(CStringArray *psaCode)
{
	CString strAppPath;
	GetDir_DrfnRoot(strAppPath);
	CString strFileName = strAppPath + "\\Usr\\1807_Result.ini";//20090615 SJ_KIM Fix the path

	CFile SaveFile(strFileName, CFile::modeCreate | CFile::modeWrite);
	CString strLineData;

	strLineData = "[REG_GROUP_LIST]\r\n";
	SaveFile.Write(strLineData, strLineData.GetLength());

	strLineData.Format("REG_CNT=%d\r\n", psaCode->GetSize());
	SaveFile.Write(strLineData, strLineData.GetLength());
	for (int nIndex = 0; nIndex < psaCode->GetSize(); nIndex++)
	{
		strLineData.Format("%d=%s\r\n", nIndex, psaCode->GetAt(nIndex));
		SaveFile.Write(strLineData, strLineData.GetLength());
	}

	SaveFile.Close();
}

//>>0000207: 조건검색 실시간, 사용자의 Column 구성과 배치(크기)등의 최종상태 저장 및 복원 기능 추가
#define	szDoGridWidthSetSectKey "UserGridWidth"
void CMainFrmWnd::_DoGridWidthSet(int nOption)
{
	CUGCtrl*pGridCtrl = (CUGCtrl*)&m_gridResult;
	CMapStringToString UserColWidthsMap;
	LPCSTR szSectKey = szDoGridWidthSetSectKey;
	CString			   szUserMapFile;

	//CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)m_pwndMain;
	CString szScreenCFG;
	szScreenCFG.Format("%s.cfg", GetScreenKey());
	szUserMapFile = GetUserFilePath(szScreenCFG);
	//szUserMapFile = pwndMainFrm->GetFilePath("FINDERINFO");

	if(nOption==0)
	{
		CFindData_GridUtil::LoadFromUserEnvFile(szUserMapFile, szSectKey, &UserColWidthsMap);
		if(UserColWidthsMap.GetCount()>0)
			CFindData_GridUtil::SetGridAllWidth(pGridCtrl, &UserColWidthsMap);
	}
	else if(nOption==1)
	{
		CFindData_GridUtil::GetGridAllWidth(pGridCtrl, &UserColWidthsMap);
		CFindData_GridUtil::SaveToUserEnvFile(szUserMapFile, szSectKey, &UserColWidthsMap);
	}
}
//<<

int CMainFrmWnd::GetFillString(CString& _szGetStr, int nStart, int nEnd, int nOneWordSize)
{
	int nLen = nEnd-nStart+1;
	if(nLen<=0) return 0;
	
	_szGetStr.Empty();
	
	char *szGetStr = (char*)malloc((nLen*nOneWordSize)+1);
	char *szOneWord = (char*)malloc(nOneWordSize+1);
	memset(szOneWord, 0, nOneWordSize+1);
	
	for(int i=nStart; i<=nEnd; i++)
	{
		sprintf(szOneWord, "%*.*d", nOneWordSize, nOneWordSize, i);
		memcpy(&szGetStr[nOneWordSize*i], szOneWord, nOneWordSize);
	}
	
	szGetStr[nLen*nOneWordSize] = 0;
	_szGetStr = szGetStr;
	free(szOneWord);
	free(szGetStr);
	
	return _szGetStr.GetLength();
}

CFont* CMainFrmWnd::GetFont(BOOL bBold)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	
	CFont* pFont;
	
	if(!bBold)
		pFont = pScreenWnd->GetDefFont();
	else
		pFont = pScreenWnd->GetBoldFont();
	
	return pFont;
}

COLORREF CMainFrmWnd::GetColor(int nColor)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	
	return pScreenWnd->GetAxColor(nColor);
}

CString CMainFrmWnd::Variant(int nComm, CString strData)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	
	return pScreenWnd->Variant(nComm, strData);
}

void CMainFrmWnd::SetBtnImg(CfxImgButton *pBtn, CString strFileName)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	pScreenWnd->SetBtnImg(pBtn, strFileName);
}

void CMainFrmWnd::SetBtnImg(CfxImgButton *pBtn, int nTextLen, BOOL bBold)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	pScreenWnd->SetBtnImg(pBtn, nTextLen, bBold);
}

COLORREF CMainFrmWnd::GetAxColor(int nColor)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetAxColor(nColor);
}

HBRUSH CMainFrmWnd::GetAxBrush(COLORREF clr)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return (HBRUSH)(*pScreenWnd->GetAxBrush(clr));
}

CPen* CMainFrmWnd::GetAxPen(COLORREF clr, int nWidth, int nStyle)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetAxPen(clr, nWidth, nStyle);
}

CWnd* CMainFrmWnd::GetModuleMainWnd()
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetModuleMainWnd();
}


BOOL CMainFrmWnd::GetStockMasterData(CStringArray &arrCode, CStringArray &arrName, UINT Jgub, UINT Upgub)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetStockMasterData(arrCode, arrName, Jgub, Upgub);
}

BOOL CMainFrmWnd::GetUpMasterData(CStringArray *&psaCode, CStringArray *&psaName, int jgub)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetUpMasterData(psaCode, psaName, jgub);
}

CString CMainFrmWnd::GetMarketNameToCode(CString strMaketCode)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	return pScreenWnd->GetMarketNameToCode(strMaketCode);
}

int CMainFrmWnd::SetResultTemp()
{
	CString strCode, strCodeName; 
	CStringArray strArrayCode, strArrayCodeName;
	
	int nDataCnt = m_gridResult.GetNumberRows();
	if(nDataCnt>0)
	{
		int nRow;
		int nMarkType;
		for (nRow = 0; nRow < m_gridResult.GetNumberRows(); nRow++)
		{
			m_gridResult.GetRowCode(nRow, strCode, nMarkType);
			strCodeName = m_gridResult.GetCellData(nRow, 0);
			
			strArrayCode.Add(strCode);
			strArrayCodeName.Add(strCodeName);
		}
		
		CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
		pScreenWnd->SetUserMaster(usrMasterCondition, strArrayCode.GetSize(), &strArrayCode, &strArrayCodeName);
	}
	return 0;
}

void CMainFrmWnd::Request_PortGroup()
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
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

	pScreenWnd->SendTR(TRNO_FAVORITE, sendB, sendL, TRKEY_GWANSIMGROUP, US_PASS);
}

void CMainFrmWnd::GetData_PortGroup(CStringArray& saKey, CStringArray& saGroupName)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();

#if (0)//tour2k
	pScreenWnd->GetData_PortGroup(saKey, saGroupName);
#else
	saKey.Copy(m_GwanSimGroup.strKey);
	saGroupName.Copy(m_GwanSimGroup.strGroupName);
#endif
}

BOOL CMainFrmWnd::GetData_PortItem(CStringArray& saCode, CStringArray& saName, int nKey)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
#if (0)//tour2k
	return pScreenWnd->GetData_PortItem(saCode, saName, nKey);
#else
	//sprintf(sSend, "%s%80s%s%40s%02d", "MYUj", " ", "00001U", " ", nKey);

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
	stmp.Format("%02d", nKey);
	memcpy((char*)&sendB[sz_uinfo], stmp, 2);

	pScreenWnd->SendTR(TRNO_FAVORITE, sendB, sendL , TRKEY_GWANSIMITEM, US_PASS);
	return true;
#endif
}

void CMainFrmWnd::DoSetPartnerSite(IPartnerManager* pPtManager)
{
	m_pPTManager = pPtManager;
	m_szPTManager.Format("%08X", pPtManager);
	if(pPtManager) pPtManager->SetSite(&m_xPartnerSite);
}

LRESULT CMainFrmWnd::OnUserInfo(WPARAM wp, LPARAM lp)
{
	BYTE bt = LOBYTE(LOWORD(wp));
	switch (bt)
	{
	case USERINFO_SET:
		{
			WORD dwKey = HIWORD(wp);
			if(dwKey==0)
			{
				IPartnerManager* pPtManager = (IPartnerManager*)lp;
				DoSetPartnerSite(pPtManager);
			}
		}
		break;
	}
	
	return 0L;
}

HRESULT CMainFrmWnd::CPartnerSite::RecieveFidData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	METHOD_PROLOGUE(CMainFrmWnd, PartnerSite)
		
		return S_OK;
}

HRESULT CMainFrmWnd::CPartnerSite::RecieveTRData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	METHOD_PROLOGUE(CMainFrmWnd, PartnerSite)
		
		if(strcmp(QUERY_STRJONGMOK, szTR)==0 && strcmp(QUERY_STRGUIDELINE, szMsgCode) != 0)
			pThis->ReceiveData(szTR, aTRData, dwTRDateLen);
		
		return S_OK;
}

HRESULT CMainFrmWnd::CPartnerSite::RecieveRealData(long dwKey, LONG dwData)
{
	return S_OK;
}

HRESULT CMainFrmWnd::CPartnerSite::RecieveKeyData(int nKey, LPCSTR _szData)
{
	METHOD_PROLOGUE(CMainFrmWnd, PartnerSite)
		
		switch(nKey)
	{
		case IPartnerSite::IPS_RK_gwansim:
			{
				//관심그룹이 변경되었다는 메시지를 받아서 관심그룹 갱신을 요청함.
				// 박성경
				// 				if(pThis->m_pGwanMng)
				// 					pThis->m_pGwanMng->RequestGwansimGroup();
			}		
			break;
	}
	
	return S_OK;
}

HRESULT CMainFrmWnd::CPartnerSite::ReceiveOnUser(WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}