// MainFrmWnd.cpp : implementation file
//
#include "stdafx.h"
#include "MainFrmWnd.h"
#include "Winsock2.h"

#include "FieldEditDlg.h"
//#include "DlgSaveCandle.h"
#include "SaveDlg.h"
#include "XMLWriter.h"
#include "ScreenWnd.h"

#include "./SectionChart/DlgChart_Stock.h"
#include "ChartSectionSearch.h"
#include "../../DrCommon/Dr_Control/DrXLSFile.h"
#include "../../../SrcSite/include/jmcode.h"

//#include "../../../SrcSite/include/axisvar.h"
#include "../../../SrcSite/include/axis_msg.h"
//#include "../../../SrcSite/include/Symbol.h"

#include "../../inc/MasterDefine.h"

#include "../../Chart/Include_Chart/Dll_Load/IMetaTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMainFrmWnd, CFrameWnd)
const  LPCTSTR  LIST_TARGET_1ST[] = { "전체", "거래소", "코스닥", /*"테마",*/ "관심종목"};
const	UINT	LIST_BONGTERM[] = { 100, 120, 150, 170, 200, 250, 300 };
const	UINT	LIST_SIMILAR[] = { 90, 80, 70, 60, 50 };

const LPCTSTR	LIST_MINTERM[] = { _T(" 1"),  _T(" 3"),  _T(" 5"), _T("10"), _T("15"), _T("30"), _T("60") };
const	UINT	LIST_TIME_INDEX[] = { 3, 4, 5, 6, 7, 8, 9 };

#define _CHARTMSG_EVENTMESSAGE	"CHARTMSG_EVENTMESSAGE"
const UINT RMSG_CHARTMSG_EVENTMESSAGE	= ::RegisterWindowMessage( _CHARTMSG_EVENTMESSAGE);

extern HINSTANCE g_hInstance;

#define TAB_PATTERN	0
#define TAB_CANDLE	1
#define MAX_BASEBONG	10

#define UWM_SEARCHCENTERDATE		( WM_USER + 0x2D49)

/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd
CMainFrmWnd::CMainFrmWnd()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);
	
//	m_bRequestReal = FALSE;
 	m_chartActivation = 0;

	m_nTrIndex = 0;
	m_nLastTrIndex = 0;
	m_interval = "001";

	m_nMarket = 0;
	m_strSectionDate = "";
	m_bLink = TRUE;

	m_nTabIndex = TAB_PATTERN;
	m_bExpand = FALSE;

	m_nCntInView	= 120;
	m_nAllCntInView	= 120;
	m_nRestrictedMaxCnt = 0;
	m_strChartTerm = "2";

	m_bSetControlBase = FALSE;

	m_bModifyFlag = FALSE;

	m_pCodeInput = NULL;

	m_pDlgChart = NULL;
	m_pDlgChartSecond = NULL;

	m_nPeriodCnt = 4;

	m_nSeparateChart = 0;

	m_pScreenWnd = NULL;

	m_hiSymbol	= NULL;
	m_pwndSymbol = NULL;

	m_nPreReceiveLen = -1;
}

CMainFrmWnd::~CMainFrmWnd()
{
//	m_brushStatic.DeleteObject();
	
}

void CMainFrmWnd::DoDataExchange(CDataExchange* pDX)
{
	CFrameWnd::DoDataExchange(pDX);	

	//{{AFX_DATA_MAP(CMainFrmWnd)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainFrmWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrmWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP

	ON_EN_KILLFOCUS(IDC_EDIT_START, OnEdKillfocusBongCnt)

	ON_CBN_SELCHANGE(IDC_COMBO_TARGET_1ST, OnChgComboTarget1st)
	ON_CBN_SELCHANGE(IDC_COMBO_TARGET_2ND, OnChgComboTarget2nd)
	ON_BN_CLICKED(IDC_RADIO_PATTERN, OnRadioPattern)
	ON_BN_CLICKED(IDC_RADIO_CANDLE, OnRadioCandle)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)

	ON_BN_CLICKED(IDC_CHK_BONG, OnBtnChkBong)
	ON_CBN_SELCHANGE(IDC_COMBO_BONG, OnChgBongCombo)
	ON_CBN_SELCHANGE(IDC_COMBO_SIM, OnChgSimCombo)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ZOOM, OnDeltaposSpinzoomvalue)

	ON_BN_CLICKED(IDC_BTN_DAY, OnBtnDay)
	ON_BN_CLICKED(IDC_BTN_WEEK, OnBtnWeek)
	ON_BN_CLICKED(IDC_BTN_MONTH, OnBtnMonth)
	ON_BN_CLICKED(IDC_BTN_MINUTE, OnBtnMinute)
	ON_CBN_SELCHANGE(IDC_COMBO_SELECTTIME, OnPeriodSelection)

	ON_BN_CLICKED(IDC_BTN_DATAMORE, OnBtnDataMore)
	ON_BN_CLICKED(IDC_BTN_PRINT, OnBtnPrint)
	ON_BN_CLICKED(IDC_BTN_FIELDEDIT, OnBtnFieldEdit)
	ON_BN_CLICKED(IDC_BTN_RESULTCTRL, OnBtnResultCtrl)
	ON_BN_CLICKED(IDC_BTN_RESULTEXPAND, OnBtnResultExpand)
	ON_BN_CLICKED(IDC_BTN_RESULTEXPAND_DOWN, OnBtnResultExpand)
	ON_COMMAND_RANGE(ID_MENU_RESULTCTRL_START, ID_MENU_RESULTCTRL_END, OnMenuResultCtrl)
	ON_COMMAND_RANGE(ID_MENU_CONDITIONSAVE_START, ID_MENU_CONDITIONSAVE_END, OnMenuConditionSave)
	ON_MESSAGE(WM_NOTIFY_GRID, OnNotify_Grid)
	ON_REGISTERED_MESSAGE( RMSG_CHARTMSG_EVENTMESSAGE,	OnEventMessage)
	ON_REGISTERED_MESSAGE(RMSG_RECEIVETRANDATA, OnReceiveTranData)
	ON_REGISTERED_MESSAGE(RMSG_RECEIVEDIRECTFID, OnReceiveDirectFID)

	ON_REGISTERED_MESSAGE(RMSG_USERINFO, OnUserInfo)

END_MESSAGE_MAP()
//	ON_MESSAGE(UMSG_SETZOOMVALUE, OnSetZoomValue)
	//ON_EN_CHANGE(IDC_EDIT_START, OnEdChangeBongCnt)

/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd diagnostics

LRESULT CMainFrmWnd::OnEventMessage(WPARAM wParam, LPARAM lParam)
{
	MSG *pMsgFromChart = (MSG *)lParam;

	switch (pMsgFromChart->message)
	{
	case UWM_ZOOMVALUE:
		{
			if(pMsgFromChart->hwnd == m_pDlgChart->m_ChartItem.GetSafeHwnd())
				OnSetZoomValue(pMsgFromChart->lParam, 0);
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		{
			int kkk = 0;
		}
		break;
	case UWM_SEARCHCENTERDATE:
		{
			int nStartDate = (int)pMsgFromChart->wParam;
			m_strSectionStartDate.Format("%d", nStartDate);

			int nEndDate = (int)pMsgFromChart->lParam;
			m_strSectionEndDate.Format("%d", nEndDate);
		}
	}

	return 1;
}

#ifdef _DEBUG
void CMainFrmWnd::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrmWnd::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd message handlers

LRESULT CMainFrmWnd::OnNotify_Grid(WPARAM wParam, LPARAM lParam)
{
	CString strData, strDate;

	if (wParam == NM_GRID_CLK)
	{
		if( 1/*bFirstClk*/)
		{
			strData = *(CString*)lParam;

			if (strData.IsEmpty() == FALSE)
			{
				int nPos_Find;
				nPos_Find = strData.Find("/");

				CString strCode_Select;
				strCode_Select = strData.Left(nPos_Find);
				m_strCode = strCode_Select;
				strDate = strData.Right(strData.GetLength() - (nPos_Find + 1));	

				if(strCode_Select.GetLength()>0)
				{
					SetChartActivation(RESULT_CHART);
					Set_SectionDate(strDate);			//검색된 구간의 시간설정
					SendTr_ChartData(strCode_Select);

					if(m_bLink)
						SetSharedDataToCode(strCode_Select);
					else
						m_bLink = TRUE;
				}
			}
		}
		else
			bFirstClk = TRUE;

		m_nSeparateChart = 1;

	}
	else if (wParam == NM_DBCLICK_L)
	{
		CString strCode = *(CString*)lParam;

		if (strCode.IsEmpty() == FALSE)
		{
// 			CStdDialog  *pdlgStd = (CStdDialog*)GetParent();
// 			//pdlgStd->GetParent()->SendMessage(UMSG_LINK_CODE, 0, (LPARAM)(LPCSTR)strCode);
// 			CSectionSearchView* pView = (CSectionSearchView*)pdlgStd->GetParent();
// 			pView->OnDBClick(strCode);
		}
	}

	return 0L;
}

LRESULT CMainFrmWnd::OnSetZoomValue(WPARAM wParam, LPARAM lParam)
{
	int nCnt = (int)wParam;

// 	if(nCnt < m_nCntInView)
// 		nCnt = m_nCntInView;
	if(m_nTabIndex == 0)
	{

	}
	else
	{
	}
	
	CString strZoomValue;
	strZoomValue.Format("%d", nCnt);
	m_edtZoomValue.SetWindowText(strZoomValue);

	return 0L;
}

void CMainFrmWnd::SetSharedDataToCode(CString strCode)
{
	m_pScreenWnd->ShareCode(strCode);
}

int CMainFrmWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	GetDir_DrfnRoot(m_strAppPath);

	g_iMetaTable.m_strDataFolderPath.Format(_T("%s\\icss\\"), m_strAppPath);
	g_iMetaTable.LoadMetaTable();

	m_pScreenWnd = (CScreenWnd*)GetParent();
	m_strScreenNo = m_pScreenWnd->GetFormCode();

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
		
	if (m_pCodeInput == NULL)
	{
		CRect rtNULL(0, 0, 0, 0);

		m_pCodeInput = new CCodeCtrl(m_pScreenWnd->GetModuleMainWnd());
		m_pCodeInput->SetRootPath(Variant(homeCC));
		m_pCodeInput->UseCodeDlg(TRUE);
		m_pCodeInput->CreateCodeCtrl(this, m_pScreenWnd->GetModuleMainWnd(), m_pScreenWnd, rtNULL, IDC_CODEINPUT);
		m_pCodeInput->SetColor(RGB(0,0,0), RGB(255,255,255));
		m_pCodeInput->SetUnit("주식");
	}

	SetInitSymbol();

	CreateImageList();
	CreateButton();
	CreateCombo();
	CreateStatic();
	CreateEdit();
	CreateMisc();
	CreateDlgChart();

	AfxSetResourceHandle(hInstSave);

	m_pITrCommManager = (IAUTrCommManager*)AfxGetPctrInterface(UUID_IAUTrCommManager);
	if(m_pITrCommManager)
		m_pITrComm = (ITrComm2*)m_pITrCommManager->AdviseTrComm();

	m_WaitCursor.SetParentWnd(this);
	//tour2k
	Request_PortGroup();

	return 0;
}

void CMainFrmWnd::CreateStatic()
{
	m_stcStatusBar.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this);
	m_stcStatusBar.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	
	m_ctrlTarget.Create(_T("대상"), WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlTarget.SetFont(&m_fontDefault);

	m_ctrlBongCntText1.Create(_T("기준"), WS_CHILD | WS_VISIBLE | SS_LEFT |SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlBongCntText1.SetFont(&m_fontDefault);
	m_ctrlBongCntText1.ShowWindow(SW_HIDE);

	m_ctrlBongCntText2.Create(_T("봉전(0은 현재봉, 최대10)"), WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlBongCntText2.SetFont(&m_fontDefault);
	m_ctrlBongCntText2.ShowWindow(SW_HIDE);

	m_ctrlCodeName.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, CRect(0, 0, 0, 0), this, IDC_STATIC_NAME);
	m_ctrlCodeName.SetFont(&m_fontDefault);
	m_ctrlCodeName.ModifyStyleEx(0,WS_EX_STATICEDGE,SWP_DRAWFRAME);

	m_ctrlSimilarity.Create(_T("유사도"), WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlSimilarity.SetFont(&m_fontDefault);

// 	m_Similarity.Create(_T("유사도"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, CRect(0, 0, 0, 0), this, IDC_STATIC);
// 	m_Similarity.SetFont(&m_fontDefault);

	m_ctrlSlash.Create(_T("/"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, CRect(0, 0, 0, 0), this);
	m_ctrlSlash.SetFont(&m_fontDefault);	

	m_ctrlState.Create(_T("진행상태"), WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_CENTERIMAGE, CRect(0, 0, 0, 0), this);
	m_ctrlState.SetFont(&m_fontDefault);	

	m_ctrlTime.Create(_T("검색시간 : "), WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, CRect(0, 0, 0, 0), this);
	m_ctrlTime.SetFont(&m_fontDefault);

	m_ctrlNotice.Create(_T("※ 본 내용은 투자판단의 참고사항이며 투자 판단의 최종 책임은 본 서비스를 사용하시는 이용자에게 있습니다."), 
		WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, CRect(0, 0, 0, 0), this);
	m_ctrlNotice.SetFont(&m_fontDefault);

	m_ctrlCodeCntText.Create(_T("검색결과 "), WS_CHILD | WS_VISIBLE | SS_LEFT |SS_CENTERIMAGE , 
		CRect(0, 0, 0, 0), this);
	m_ctrlCodeCntText.SetFont(&m_fontDefault);
	m_ctrlCodeCntText.ShowWindow(SW_HIDE);

	m_ctrlCodeCnt.Create(_T("    "), WS_CHILD | WS_VISIBLE | SS_LEFT|SS_CENTERIMAGE, 
		CRect(0, 0, 0, 0), this);
	m_ctrlCodeCnt.SetFont(&m_fontBold);
}

void CMainFrmWnd::CreateButton()
{
	// 라디오 버튼
	m_RadioPattern.Create("추세패턴", WS_CHILD|BS_AUTORADIOBUTTON|WS_GROUP|WS_VISIBLE, CRect(0,0,0,0), this, IDC_RADIO_PATTERN);
	m_RadioPattern.SetFont(&m_fontDefault);	
	m_RadioCandle.Create("캔들패턴", WS_CHILD|BS_AUTORADIOBUTTON|WS_GROUP|WS_VISIBLE, CRect(0,0,0,0), this, IDC_RADIO_CANDLE);
	m_RadioCandle.SetFont(&m_fontDefault);	
	CheckRadioButton(IDC_RADIO_PATTERN, IDC_RADIO_CANDLE, IDC_RADIO_PATTERN);

	// 체크박스
	m_btnReverseChk.Create(_T("지정그룹제외"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_SPECIALIZEDITEMCHK);
	m_btnReverseChk.SetFont(&m_fontDefault);

	m_btnExceptionChk.Create(_T("우선주제외"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_PREFFERDITEMCHK);
	m_btnExceptionChk.SetFont(&m_fontDefault);

	m_btnIgnoreColorChk.Create(_T("양음봉무시"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_IGNORECOLORCHK);
	m_btnIgnoreColorChk.SetFont(&m_fontDefault);
	if(m_RadioPattern.GetState())
		m_btnIgnoreColorChk.ShowWindow(SW_HIDE);

	m_btnDay.Create(_T("일"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_DAY);
	m_btnDay.SetFont(&m_fontDefault);

	m_btnWeek.Create(_T("주"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_WEEK);
	m_btnWeek.SetFont(&m_fontDefault);

	m_btnMonth.Create(_T("월"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_MONTH);
	m_btnMonth.SetFont(&m_fontDefault);
	
	m_btnMin.Create(_T("분"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_MINUTE);
	m_btnMin.SetFont(&m_fontDefault);

	m_btnDay.FP_vSetImage(&m_ImageListBtn, /*m_clrMainSkin,*/ 81, 82, TRUE, "일", TRUE);
	m_btnWeek.FP_vSetImage(&m_ImageListBtn, /*m_clrMainSkin,*/ 81, 82, TRUE, "주", TRUE);
	m_btnMonth.FP_vSetImage(&m_ImageListBtn, /*m_clrMainSkin,*/ 81, 82, TRUE, "월", TRUE);
	m_btnMin.FP_vSetImage(&m_ImageListBtn, /*m_clrMainSkin,*/ 81, 82, TRUE, "분", TRUE);

	m_btnSearch.Create(_T("검  색"), CRect(0, 0, 0, 0), this, IDC_BTN_SEARCH);
	SetBtnImg(&m_btnSearch, 4, TRUE);
	m_btnSearch.SetFont(&m_fontBold);

	m_btnBongChk.Create(_T("과거시점"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_CHK_BONG);
	m_btnBongChk.SetFont(&m_fontDefault);

	m_btnRequestMore.Create(_T(""), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_BTN_DATAMORE);
	m_btnRequestMore.SetFont(&m_fontDefault);
	m_btnRequestMore.FP_vSetImage(&m_ImageListBtn, /*m_clrMainSkin,*/ 40, 42, FALSE);
	m_btnRequestMore.FP_vSetToolTipText("데이타추가부르기");
	m_btnRequestMore.ShowWindow(SW_HIDE);

	//////////////////////////////////////////////////////////////////////////
	m_btnPrint.Create(_T("인쇄"), CRect(0, 0, 0, 0), this, IDC_BTN_PRINT);
	SetBtnImg(&m_btnPrint, 2);
	m_btnPrint.SetFont(&m_fontDefault);

	m_btnFieldEdit.Create(_T("항목편집"), CRect(0, 0, 0, 0), this, IDC_BTN_FIELDEDIT);
	SetBtnImg(&m_btnFieldEdit, 4);
	m_btnFieldEdit.SetFont(&m_fontDefault);
    
	m_btnResultCtrl.Create(_T("종목전송"), CRect(0, 0, 0, 0), this, IDC_BTN_RESULTCTRL);
	SetBtnImg(&m_btnResultCtrl, 4);
	m_btnResultCtrl.SetFont(&m_fontDefault);
  
	m_btnExpand.Create(_T(""), CRect(0, 0, 25, 20), this, IDC_BTN_RESULTEXPAND);
	SetBtnImg(&m_btnExpand, "RIGHT_EX");
}

void CMainFrmWnd::CreateCombo()
{
	m_comboTarget_1st.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 
		CRect(0, 0, 0, 0), this, IDC_COMBO_TARGET_1ST);
	int nIndex = 0;
	for (nIndex = 0; nIndex < _countof(LIST_TARGET_1ST); nIndex++)
		m_comboTarget_1st.AddString(LIST_TARGET_1ST[nIndex]);
	m_comboTarget_1st.SetCurSel(0);
	m_comboTarget_1st.SetFont(&m_fontDefault);	

	m_comboTarget_2nd.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 
		CRect(0, 0, 0, 0), this, IDC_COMBO_TARGET_2ND);
	m_comboTarget_2nd.SetFont(&m_fontDefault);	
	m_comboTarget_2nd.EnableWindow(FALSE);

	m_comboTime.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 
		CRect(0, 0, 0, 0), this, IDC_COMBO_SELECTTIME);
	m_comboTime.SetFont(&m_fontDefault);
	m_comboTime.ResetContent();
	for(int index=0; index < _countof(LIST_MINTERM); index++)
		m_comboTime.AddString((CString)LIST_MINTERM[index] + "분");

	m_comboTail.Create(WS_CHILD | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(0, 0, 0, 0), this, IDC_COMBO_TAIL);
	m_comboTail.SetFont(&m_fontDefault);
	char* szTailText[] = {"꼬리길이적용", "꼬리길이무시", "꼬리유무무시"};
	for (nIndex = 0; nIndex < 3; nIndex++)
		m_comboTail.AddString(szTailText[nIndex]);
	m_comboTail.SetCurSel(0);

	m_comboBongTerm.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(0, 0, 0, 0), this, IDC_COMBO_BONG);
	m_comboBongTerm.SetFont(&m_fontDefault);

	CString strData;
	for (nIndex = 0; nIndex < _countof(LIST_BONGTERM); nIndex++)
	{
		strData.Format("%d개 이내", (int)LIST_BONGTERM[nIndex]);
		m_comboBongTerm.AddString(strData);
	}
	m_comboBongTerm.SetCurSel(0);
	
	
	m_comboSimilar.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(0, 0, 0, 0), this, IDC_COMBO_SIM);
	m_comboSimilar.SetFont(&m_fontDefault);
	for (nIndex = 0; nIndex < _countof(LIST_SIMILAR); nIndex++)
	{
		strData.Format("%d%% 이상", (int)LIST_SIMILAR[nIndex]);
		m_comboSimilar.AddString(strData);
	}
	m_comboSimilar.SetCurSel(0);
}

void CMainFrmWnd::CreateEdit()
{
	m_editBongCnt.CreateEx(WS_EX_CLIENTEDGE, "edit", "0", WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, IDC_EDIT_START);
	m_editBongCnt.SetFont(&m_fontDefault);
	m_editBongCnt.ShowWindow(SW_HIDE);
	
	// 유사도 에디트박스
// 	m_editSimilarity.Create(WS_CHILD | WS_VISIBLE | ES_CENTER, CRect(0,0,0,0), this, IDC_EDIT_SIMILARITY);
// 	m_editSimilarity.ModifyStyleEx(0, WS_EX_CLIENTEDGE);	
// 	m_editSimilarity.SetFont(&m_fontDefault);
// 	m_editSimilarity.SetWindowText("60");

	m_edtZoomValue.CreateEx(WS_EX_CLIENTEDGE, "edit", "0", WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_AUTOHSCROLL,CRect(0,0,0,0), this, IDC_EDITZOOMVALUE);
	m_edtZoomValue.SetFont(&m_fontDefault);
	m_edtZoomValue.SetWindowText("120");

	m_spinZoom.Create(WS_CHILD | WS_VISIBLE | UDS_AUTOBUDDY | UDS_ARROWKEYS, CRect(0,0,10,10), this, IDC_SPIN_ZOOM);
	m_spinZoom.SetBuddy(&m_edtZoomValue);
	
	m_editCntOfDatAll.CreateEx(WS_EX_CLIENTEDGE, "edit", "0", WS_CHILD | WS_VISIBLE | ES_RIGHT  |ES_MULTILINE,CRect(0,0,0,0), this, IDC_EDITCNTOFDATALL);
	m_editCntOfDatAll.SetFont(&m_fontDefault);
	m_editCntOfDatAll.SetWindowText("120");
	m_editCntOfDatAll.SetLimitText(3);
}

void CMainFrmWnd::CreateMisc()
{
	// 그리드
	m_gridResult.CreateGrid(WS_CHILD | WS_VISIBLE | WS_BORDER,
		CRect(0, 0, 0, 0), this, IDC_GRID_RESULT);

	//챠트
// 	m_ctrlChartCenter.Create(_T(""), WS_CHILD | WS_VISIBLE,
// 		CRect(0, 0, 0, 0), this, IDC_STATIC_CHARTCENTER);
// 
// 	m_ctrlChartZoomIn.Create(_T(""), WS_CHILD | WS_VISIBLE,
// 		CRect(0, 0, 0, 0), this, IDC_STATIC_CHARTZOOMIN);
}

void CMainFrmWnd::CreateImageList()
{
	CString strImgPath, strData;

	strImgPath = m_strAppPath + (CString)FOLDER_IMAGE + "\\W20_H20.bmp";

	if ( !m_ImageListBtn.m_hImageList)
	{
		if ( !m_ImageListBtn.Create( 20, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			SetMessageBox("Can not Find W20_H20.bmp file" );			
		}

		CBitmap bitmap;
		bitmap.Attach ((HBITMAP)::LoadImage(NULL, strImgPath, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImageListBtn.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}
}

void CMainFrmWnd::OnDestroy() 
{
	_DoGridWidthSet(1);	//0000207

	if(m_pCodeInput)
	{
		m_pCodeInput->DestroyWindow();
		delete m_pCodeInput;
		m_pCodeInput = NULL;
	}

	if (m_pwndSymbol)
	{
		if (IsWindow(m_pwndSymbol->GetSafeHwnd()))
			m_pwndSymbol->SendMessage(WM_CLOSE);
		m_pwndSymbol->Detach();
		delete m_pwndSymbol;
	}

	CFrameWnd::OnDestroy();
	
 	m_saCodeTheme.RemoveAll();
	m_saCodeMST.RemoveAll();
	m_saCodeGroup.RemoveAll();

	m_ImageListBtn.DeleteImageList();

	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();	
	m_brushBK.DeleteObject();

    DestroyChart();
	ClearUserData();

	if(m_pITrCommManager)
	{
		if(m_pITrComm)
		{
			m_pITrCommManager->UnAdviseTrComm(m_pITrComm);
			m_pITrComm = NULL;
		}
	}
}


void CMainFrmWnd::OnSize(UINT nType, int cx, int cy) 
{	
	CFrameWnd::OnSize(nType, cx, cy);
	OnSize(); 
}

void CMainFrmWnd::OnSize()
{
	CRect reClient, reTop, reBottom, reTemp;
	GetClientRect(reClient);

	reClient.top += 3;
	reClient.bottom -= 3;

	reTop = reClient;
	reTop.DeflateRect(5, 2, 0, 2);
	reTop.bottom = reTop.top + 20;

	m_rcRdFrame = reTop;
	m_rcRdFrame.right =  m_rcRdFrame.left + 150;

	reTemp = reTop;
	reTemp.left += 4;
	reTemp.right = reTemp.left + 68;
	reTemp.top += 3;
	reTemp.bottom = reTemp.top + 15;
	m_RadioPattern.MoveWindow(&reTemp); // 추세패턴 라디오

	reTemp.left = reTemp.right + 4;
	reTemp.right = reTemp.left + 68;
	m_RadioCandle.MoveWindow(&reTemp); // 캔들패턴 라디오

//	reTemp = reTop;
	reTemp.top = reTop.top;
	reTemp.bottom = reTop.bottom;
	reTemp.left = reTemp.right + 15;
	reTemp.right = reTemp.left + 27;
	m_ctrlTarget.MoveWindow(&reTemp);

	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 75;
	reTemp.bottom = reTemp.top + 100;
	m_comboTarget_1st.MoveWindow(&reTemp);

	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 150;
	reTemp.bottom = reTemp.top + 180;
	m_comboTarget_2nd.MoveWindow(&reTemp);
	reTemp.bottom -= 160;

	reTemp.left = reTemp.right + 5;
	reTemp.right = reTemp.left + 80;
	m_btnExceptionChk.MoveWindow(&reTemp); //우선주제외

	reTemp.left = reTemp.right + 9;
	reTemp.right = reTemp.left + 90;
	m_btnReverseChk.MoveWindow(&reTemp);   //지정종목

	// 캔들용 (기준봉)
	reTemp = reTop;

	reTemp.right -= 5;
	reTemp.left = reTemp.right - 145;
	m_ctrlBongCntText2.MoveWindow(&reTemp);

	reTemp.right = reTemp.left  - 2;
	reTemp.left = reTemp.right - 20;
	m_editBongCnt.MoveWindow(&reTemp);

	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 26;
	m_ctrlBongCntText1.MoveWindow(&reTemp);	

// 	reTemp.right = reTemp.left - 8;
// 	reTemp.left = reTemp.right - 80;
// 	reTemp.OffsetRect(0, 1);
// 	m_btnIgnoreColorChk.MoveWindow(&reTemp);

	//////////////////////////////////////////////////////////////////////////
	reTop.OffsetRect(0, 24);

	//==========================================
	reTemp = reTop;

	reTemp.right = reTemp.left + 84;
	if(m_pCodeInput) m_pCodeInput->MoveWindow(&reTemp);

	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 142;
	/*m_ctrlCodeName.MoveWindow(&reTemp);*/
	m_pwndSymbol->MoveWindow(reTemp);

	reTemp.left = reTemp.right + 5;
	reTemp.right = reTemp.left + 20;
	m_btnDay.MoveWindow(&reTemp);

	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 20;
	m_btnWeek.MoveWindow(&reTemp);

	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 20;
	m_btnMonth.MoveWindow(&reTemp);

	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 20;
	m_btnMin.MoveWindow(&reTemp);

	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 49;
	reTemp.bottom += 100;
	m_comboTime.MoveWindow(&reTemp);
	reTemp.bottom -= 100;

	reTemp.left = reTemp.right + 5;
	reTemp.right = reTemp.left + 30;
	m_edtZoomValue.MoveWindow(&reTemp);

	reTemp.left = reTemp.right + 0;
	reTemp.right = reTemp.left + 16;
	m_spinZoom.MoveWindow(&reTemp);

	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 12;
	m_ctrlSlash.MoveWindow(&reTemp);

	reTemp.left = reTemp.right + 1;
	reTemp.right = reTemp.left + 30;
	m_editCntOfDatAll.MoveWindow(&reTemp);

	reTemp.left = reTemp.right + 3;
	reTemp.right = reTemp.left + 20;
	m_btnRequestMore.MoveWindow(&reTemp);

	// 추세용 (과거시점 갯수, 유사도)
	reTemp = reTop;
	reTemp.right -= 5;

	reTemp.left = reTemp.right - 90;
	reTemp.bottom += 100;
	m_comboBongTerm.MoveWindow(&reTemp);
	reTemp.bottom -= 100;

	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 68;
	m_btnBongChk.MoveWindow(&reTemp);

	reTemp.right = reTemp.left - 7;
	reTemp.left = reTemp.right - 80;
	reTemp.bottom += 100;
	m_comboSimilar.MoveWindow(&reTemp);
	reTemp.bottom -= 100;

	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 40;
	m_ctrlSimilarity.MoveWindow(&reTemp);  // "유사도"

	// 캔들용 (기준봉)
	reTemp = reTop;

	reTemp.right -= 5;
	reTemp.left = reTemp.right - 98;
	reTemp.bottom = reTemp.top + 70;
	m_comboTail.MoveWindow(&reTemp);

// 	reTemp.right -= 5;
// 	reTemp.left = reTemp.right - 110;
// 	m_ctrlBongCntText2.MoveWindow(&reTemp);
// 
// 	reTemp.right = reTemp.left  - 2;
// 	reTemp.left = reTemp.right - 20;
// 	m_editBongCnt.MoveWindow(&reTemp);
// 
// 	reTemp.right = reTemp.left - 3;
// 	reTemp.left = reTemp.right - 28;
// 	m_ctrlBongCntText1.MoveWindow(&reTemp);	

	reTemp.right = reTemp.left - 8;
	reTemp.left = reTemp.right - 80;
	reTemp.bottom = reTemp.top + 20;
	reTemp.OffsetRect(0, 1);
	m_btnIgnoreColorChk.MoveWindow(&reTemp);
	//////////////////////////////////////////////////////////////////////////
	reClient.DeflateRect(5, 50, 5, 25);
	int nHalf = reClient.Height() / 2;
	
	reTop = reClient;
	reTop.bottom = reTop.top + nHalf;

	reBottom = reClient;
	reBottom.top += nHalf + 3;

 	if(m_pDlgChart)
 		m_pDlgChart->MoveWindow(&reTop);

	//////////////////////////////////////////////////////////////////////////
	int nWidth_Grid;

	reTemp = reBottom;
	reTemp.bottom = reTemp.top + 20;
	
	reTemp.right = reTemp.left + 60;
	m_btnSearch.MoveWindow(&reTemp);   // 검색
	
	reTemp.left = reTemp.right + 5;
	reTemp.right = reTemp.left + 52;
	m_ctrlCodeCntText.MoveWindow(&reTemp);

	reTemp.left = reTemp.right + 0;
	reTemp.right = reTemp.left + 60;
	m_ctrlCodeCnt.MoveWindow(&reTemp);


	reTemp = reBottom;
	if(m_bExpand)
		nWidth_Grid = reTemp.Width();
	else
		nWidth_Grid = (reTemp.right - reTemp.left + 5) / 2 + 3 + 69;	
	reTemp.right = reTemp.left + nWidth_Grid;
	reTemp.bottom = reTemp.top + 20;	

	reTemp.left = reTemp.right - 20;
	m_btnExpand.MoveWindow(&reTemp);

	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 60;
	m_btnResultCtrl.MoveWindow(&reTemp);
	
	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 60;
	m_btnFieldEdit.MoveWindow(&reTemp);
	
	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 36;
	m_btnPrint.MoveWindow(&reTemp);	

	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 67;

	reTemp = reBottom;
	
	reTemp.top += 1;
	
	if(m_bExpand)
		nWidth_Grid = reTemp.Width();
	else
		nWidth_Grid = (reTemp.right - reTemp.left + 5) / 2 + 3 + 69;	
	reTemp.right = reTemp.left + nWidth_Grid;
	reTemp.top += 22;	

	m_gridResult.MoveWindow(&reTemp);  //그리드 

	//////////////////////////////////////////////////////////////////////////
	reTemp = reBottom;
	reTemp.left += nWidth_Grid + 4;

  	if(m_pDlgChartSecond)
  		m_pDlgChartSecond->MoveWindow(&reTemp);

	reTemp = reBottom;
	reTemp.top = reTemp.bottom + 2;
	reTemp.bottom = reTemp.bottom + 25;
	m_stcStatusBar.MoveWindow(&reTemp);


	reTemp = reBottom;
	reTemp.left += 2;
	reTemp.top = reTemp.bottom + 4;
	reTemp.right = reTemp.left + 650;
	reTemp.bottom = reTemp.top + 18;
	//m_ctrlTime.MoveWindow(&reTemp);
	m_ctrlNotice.MoveWindow(&reTemp);


	reTemp.right = reClient.right - 5;
	reTemp.left = reTemp.right - 150;
	m_ctrlState.MoveWindow(&reTemp);

	OnPF_Contorl_Move();
}

void CMainFrmWnd::SetInit()
{
	GetLoadSet_Def();

	m_gridResult.SetInit("RESULT");
	m_gridResult.ClearAll();

	SetLoad_FieldInfo();
	SetGrid_Head(FALSE);
	SetGrid_Head(TRUE);
	LoadUserData();
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
	
///////////////////////////////////////////////////////////
// 20100414 박찬모 테마 삭제 >>
// 	else if (m_nDefult_TargetBaseType == 3) // 테마
// 	{
// 		m_strTargetBaseType = "30";
// 	}
// 	else if (m_nDefult_TargetBaseType == 4) // 관심
// 	{
// 		m_strTargetBaseType = "40";
// 	}

	else if (m_nDefult_TargetBaseType == 3) // 관심
	{
		m_strTargetBaseType = "30";
	}
// 20100414 박찬모 테마 삭제 <<
///////////////////////////////////////////////////////////
	
	m_comboTarget_1st.SetCurSel(m_nDefult_TargetBaseType);
	m_btnBongChk.SetCheck(m_nDefult_BongTerm_User);
	if(m_nDefult_BongTerm_User == 0)
		m_comboBongTerm.EnableWindow(FALSE);
	
	m_btnIgnoreColorChk.SetCheck(m_nDefult_AllColorChk);
	m_btnExceptionChk.SetCheck(m_nDefult_ExceptPreferred);
	m_btnReverseChk.EnableWindow(FALSE);

	m_comboTail.SetCurSel(m_nDefult_Tail);

	m_comboTime.EnableWindow(FALSE);
	m_comboTime.SetCurSel(m_nDefult_ChartTermMin);
	m_interval.Format("%03d", atoi(LIST_MINTERM [m_nDefult_ChartTermMin]));
	m_comboTime.Invalidate();

	m_strChartTerm.Format("%d", m_nDefult_ChartTerm);

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
	{
		m_btnDay.SetCheck(TRUE);
	}
	else if (m_nDefult_ChartTerm == 3)	//# 주
	{
		m_btnWeek.SetCheck(TRUE);
	}
	else if (m_nDefult_ChartTerm == 4)	//# 월
	{
		m_btnMonth.SetCheck(TRUE);
	}
	else if (m_nDefult_ChartTerm == 5)	//# 년
	{
	}
	
	m_comboBongTerm.SetCurSel(m_nDefult_BongTerm);
	m_comboSimilar.SetCurSel(m_nDefult_Similar);
	
	CString strBongPass;
	strBongPass.Format("%d", m_nDefult_BongPass);
	m_editBongCnt.SetWindowText(strBongPass);

	//////////////////////////////////////////////////////////////////////////
	m_pDlgChart->m_ChartItem.SetInit(this->GetSafeHwnd(), 0, m_strAppPath, FALSE);
	m_pDlgChart->m_ChartItem.LoadAddIn("ChartPeriodSummaryAddIn.dll:DRAG_ZOOM_ANAL");//DRAG_ZOOM_ANAL

	//m_pDlgChartSecond->m_ChartItem.SetInit(this->GetSafeHwnd(), 0, m_strAppPath, TRUE);
	m_pDlgChartSecond->m_ChartItem.LoadAddIn("ChartPeriodSummaryAddIn.dll:DRAG_ZOOM_ANAL");

	m_brushBK.DeleteObject();
	m_brushBK.CreateSolidBrush(m_clrMainSkin);  

	SetTrData_MSTCode();
}

void CMainFrmWnd::SetTrData_MSTCode()
{
	int nType = 2;

	char* lpData = new char[sizeof(int) + 1];
	nType = ntohl(nType);
	::memset(lpData, 0x00, sizeof(int) + 1);
	memcpy(lpData, &nType, sizeof(int) + 1);

	m_WaitCursor.BWaitCursorIntelli();

	char szSaveBuf[8192] = { 0 };
	int nPos = 0;
	InMakeFid_Mstcode(TRNO_MSTCODE, sizeof(int), (BYTE*)lpData, szSaveBuf, nPos);
	
	m_pScreenWnd->SendTR(TRNO_MSTCODE, (char*)lpData, sizeof(int), TRKEY_MSTCODE, US_KEY);
	
	if (lpData)
		delete[] lpData;

	m_pCodeInput->SetHistory(Variant(historyCC, "1301"));
	CString strHistoryCode, strCode;
	strHistoryCode = Variant(historyCC, "1301");
	int nFind = strHistoryCode.Find(" ");
	if(nFind > -1)
		strCode = strHistoryCode.Left(nFind);

	if(strCode.IsEmpty())
		strCode = "024110";

	SetChangeCode(strCode);
}

// 테마 그룹/코드 정보를 조회한다.
void CMainFrmWnd::SetTrData_ThemeGroup()
{
// 	BYTE*	lpInputData;
// 	lpInputData = (BYTE*)calloc(sizeof(fn_HFIBOA076220_InRec1), sizeof(BYTE));
// 	memset(lpInputData, ' ', sizeof(fn_HFIBOA076220_InRec1));
// 
// 	fn_HFIBOA076220_InRec1	lpInput;
// 	memset(&lpInput, ' ', sizeof(fn_HFIBOA076220_InRec1));
// 
// 	CString strTemp;
// 	strTemp = "1";
// 	memcpy(lpInput.Sect2,		(LPCSTR)strTemp, strTemp.GetLength() );
// 	strTemp = "W";
// 	memcpy( lpInput.WMonthSect,	(LPCSTR)strTemp, strTemp.GetLength() );
// 	
// 	m_pTRComm->SetTR("HFIBOA076220");
// 
// 	m_WaitCursor.BWaitCursorIntelli();
// 
// 	//화면번호를 세팅한다.
// 	m_pTRComm->SetBaseData("SCREENCODE", m_strScreenNo);
// 	
// 	if(!m_pDataUtilMng)
// 	{
// 		m_pDataUtilMng = (IDataUtilManager2*)AfxGetPctrInterface(UUID_IDataUtilManager2, 1);
// 	}
// 	m_pTRComm->Send2Server(&lpInput, sizeof(fn_HFIBOA076220_InRec1), FALSE);
}

void CMainFrmWnd::SendTr_ChartData(CString strCode)
{
	CString strDateTime;
	int nPos_Find = strCode.Find("/");
	if (nPos_Find > 0)
	{
		CString strTemp = strCode.Left(nPos_Find);
		strDateTime = strCode.Right(strCode.GetLength() - (nPos_Find + 1));
		strCode = strTemp;
	}

	KB_p0602_InRec1 stOutput;

	CTime timer;
	timer = CTime::GetCurrentTime();
	
	memset(&stOutput, 0x20, sizeof(stOutput));
	
	memset(stOutput.shcode, 0x20, sizeof(stOutput.shcode));
	memcpy(stOutput.shcode, strCode, strCode.GetLength());

	CString strCount;
	CString strDate_Start, strDate_End;
	strDate_Start = m_strSectionDate.Left(8);
	strDate_End = m_strSectionDate.Mid(15, 8);
	if (atoi(m_strChartTerm) > 3) //월
		strDate_End = strDate_End.Left(6) + "01";
	
	if(m_nTabIndex == TAB_PATTERN && m_chartActivation == RESULT_CHART)
	{
		//2009.12.01 by LYH >> 60일 이평까지 보이도록 개선
		//strCount.Format("%04d", m_SectionDataList.GetCount() + 5);
		strCount.Format("%04d", m_SectionDataList.GetCount() + 5 + 60);
		//2009.12.01 by LYH <<
		memcpy(stOutput.date, strDate_End.GetBuffer(0), strDate_End.GetLength());		// 검색일
		strDate_End.ReleaseBuffer();
	}
	else
	{
		if(m_bModifyFlag)
			strDate_Start = m_strSectionDate.Mid(15, 8);
		else
			strDate_Start.Format("%04d%02d%02d",timer.GetYear(), timer.GetMonth(), timer.GetDay());
		memcpy(stOutput.date, strDate_Start, strDate_Start.GetLength());						// 검색일
		strCount.Format("%04d", m_nAllCntInView);
	}

	if(m_nTabIndex == TAB_CANDLE && m_chartActivation == RESULT_CHART)
		memcpy(stOutput.num,		    "0100",				 4);		// 검색할 데이터 갯수s
	else
		memcpy(stOutput.num,		    strCount,			 4);
	
	if(m_strChartTerm == "1")
		memcpy(stOutput.unit,	    m_interval,			 min(m_interval.GetLength(), 3));		// 틱/분/일 조회간격단위
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

	if (m_pwndSymbol && m_chartActivation == CENTER_CHART)
		SendSymbol(strCode);
}


HRESULT CMainFrmWnd::ReceiveData(LPCSTR szTR, LPVOID aTRData, long dwTRDataLen , int nKey )
{
    char *pBuff;
	CString strMsg, strData;

	if (dwTRDataLen == 0)		//# 0 : err
	{
		m_WaitCursor.EWaitCursor();
		m_ctrlState.SetWindowText("진행상태 : [패킷 데이터 오류]");  
		SetEnable(TRUE);
	}
	else if (dwTRDataLen > 0)
	{
		m_ctrlState.SetWindowText("진행상태 : [조회 완료]");

		if((CString)szTR == TRNO_DRAW_RESULT)		// 패턴 구간 검색  결과
		{	
			bFirstClk = FALSE;

			if(SetReceiveData_Pattern((LPTSTR)aTRData, dwTRDataLen) == FALSE)
			{
				m_ctrlState.SetWindowText("진행상태 : [조회 결과 없음]");
	
				m_gridResult.ClearAll();
				m_gridResult.m_nPatternType = 0;

				m_pDlgChartSecond->m_ChartItem.ClearChartData(0);
				m_pDlgChartSecond->m_ChartItem.InvokeAddInStr("ChartPeriodSummaryAddIn.dll:DRAG_ZOOM_ANAL", "0","");
			}
            SetEnable(TRUE);
		
			m_WaitCursor.EWaitCursor();

			SetResultTemp();
		}
 		else if(strcmp(szTR, TRNO_CANDLE_RESULT) == 0) // 캔들 검색 결과
		{
			SetEnable(TRUE);

			if(SetReceiveData_Candle((char*)aTRData, dwTRDataLen) == FALSE)
			{
				m_ctrlState.SetWindowText("진행상태 : [조회 결과 없음]");
	
				m_gridResult.ClearAll();
				m_gridResult.m_nPatternType = 1;

				m_pDlgChartSecond->m_ChartItem.ClearChartData(0);
				m_pDlgChartSecond->m_ChartItem.InvokeAddInStr("ChartPeriodSummaryAddIn.dll:DRAG_CANDLE_ANAL", "0","");
			}

			m_WaitCursor.EWaitCursor();

			SetResultTemp();
		}
		else if((CString)szTR == QUERY_STRJONGMOK) //차트 TR
		{
			char *pszBuff=NULL;
			pszBuff = new char[dwTRDataLen+1];
			memset(pszBuff, 0x00, sizeof(pszBuff));
			memcpy(pszBuff, aTRData, dwTRDataLen);
			
			if (m_pDlgChart && m_chartActivation == CENTER_CHART)
				Receive_ChartData(pszBuff, dwTRDataLen);
			
			if (m_pDlgChartSecond && m_chartActivation == RESULT_CHART)
				Receive_ChartData_Second(pszBuff, dwTRDataLen);
			
			delete []pszBuff;
			pszBuff = NULL;
		}	
		else if (szTR == (CString)TRNO_MSTCODE) // 전체종목 정보(검색서버와 동기화)
		{
			// 받은 마스터 데이터의 크기가 작을 경우는 오류로 판단하고, 재조회.
			if (dwTRDataLen < MINLEN_MSTCODE_TR)
			{
				CString strFile, strDataDir;
				strDataDir.Format("%s%s\\%s", Variant(homeCC), FOLDER_FIND, gszCacheDir);
				strFile.Format("%s\\%s", strDataDir, "RequestToday.ini");
				::WritePrivateProfileString(gszDataKey_MstCode, "Today", "", strFile);
				strFile.Format("%s%s", strDataDir, "\\MstCode.dat");
				::DeleteFile(strFile);

				if (m_nPreReceiveLen == dwTRDataLen)
					AfxMessageBox(_T("\"MstCode.dat\" 파일을 정상적으로 생성할 수 없습니다.\n 화면을 다시 실행하여 주십시요."));
				else
				{
					m_nPreReceiveLen = dwTRDataLen;

					SetTrData_MSTCode();
				}
				return S_OK;
			}

			m_WaitCursor.EWaitCursor();

			m_saCodeMST.RemoveAll();

			MSTCode stMSTCode;
			//	int nDataCnt = dwTRDateLen / sizeof(MSTCode);
			char szCount[5] = {0,};
			memcpy(szCount, aTRData, 4);
			int nDataCnt = atoi(szCount);	
			int nSizeOfCount = 4;

			CString strAddData;
			for (int nIndex = 0; nIndex < nDataCnt; nIndex++)
			{
				memset(&stMSTCode, 0x00, sizeof(MSTCode));
				memcpy(&stMSTCode, (char*)aTRData + sizeof(MSTCode) * nIndex + 4, sizeof(MSTCode));

				strAddData = "";
				STR_CONVERT(pBuff, stMSTCode.sMarket, sizeof(stMSTCode.sMarket), strData);
				strAddData = strData;
				
				STR_CONVERT(pBuff, stMSTCode.sCode, sizeof(stMSTCode.sCode), strData);
				strData.TrimLeft(" ");
				strData.TrimRight(" ");

				strAddData += strData;
				m_saCodeMST.Add(strAddData);
			}

			m_WaitCursor.EWaitCursor();
		}
		else if(strcmp(szTR, TRNO_THEMEGROUP) == 0) // 테마그룹 및 코드
		{
// 			m_WaitCursor.EWaitCursor();
// 
// 			m_saCodeTheme.RemoveAll();
// 
// 			fn_HFIBOA076220_OutRec1* pOutRec1 = (fn_HFIBOA076220_OutRec1*)aTRData;
// 
// 			CString szTmp;
// 			szTmp.Format("%4.4s", pOutRec1->PrntCnt3);
// 			int nDataCnt = atoi(szTmp);
// 
// 			fn_HFIBOA076220_OutRec2* pOutRec2 = (fn_HFIBOA076220_OutRec2*)((char*)aTRData+sizeof(fn_HFIBOA076220_OutRec1));
// 			CString	strCode, strName;
// 			LIST_CCodeTypeA ListTypeA;
// 			for(int i=0; i<nDataCnt; i++)
// 			{
// 				fn_HFIBOA076220_OutRec2 *pTmpRec = (fn_HFIBOA076220_OutRec2*)(pOutRec2 + i);
// 				CString strCode, strName;
// 				strCode.Format("%5.5s", pTmpRec->ThemaGrpCd); strCode.TrimRight();
// 				strName.Format("%50.50s", pTmpRec->ThemaGrpNm);	strName.TrimRight();			
// 				m_saCodeTheme.Add(strCode + strName);
// 			}
		}
		else if(strcmp(szTR, TRNO_THEMEITEM) == 0)	// 특정테마 구성종목
		{
			m_WaitCursor.EWaitCursor();

			m_saCodeGroup.RemoveAll();

			fn_HFIBOA076210_OutRec1* pOutRec1 = (fn_HFIBOA076210_OutRec1*)aTRData;

			CString strCnt;
			strCnt.Format("%04.04s", pOutRec1->PrntCnt3);	/* 출력건수 */
			int nDataCnt = atoi(strCnt);

			fn_HFIBOA076210_OutRec2* pOutRec2 = (fn_HFIBOA076210_OutRec2 *)((char*)aTRData+sizeof(fn_HFIBOA076210_OutRec1));
			CString	strCode;

			for(int i=0; i<nDataCnt; i++)
			{
				fn_HFIBOA076210_OutRec2* pTmpRec = (fn_HFIBOA076210_OutRec2*)(pOutRec2 + i);

				strCode.Format("%6.6s", pTmpRec->ItemCd13);

				m_saCodeGroup.Add(strCode);		
			}
		}
		else if(strcmp(szTR, "13600") == 0) // 코스피/코스닥 업종 구성종목
		{
// 			CHARVECTOR2* pData = (CHARVECTOR2*)aTRData;
// 
// 			CString strCount, strOut;
// 			strCount.Format("%d", dwTRDataLen);
// 
// 			m_saCodeGroup.RemoveAll();
// 
// 			//데이터
// 			for(int i=0; i < dwTRDataLen; i++)
// 			{
// 				//strData = (*pData)[i][0]; //순번
// 				strData = (*pData)[i][1]; // 코드
// 				m_saCodeGroup.Add(strData); // 종목명
// 				//strData = (*pData)[i][2];
// 			}
// 			m_WaitCursor.EWaitCursor();
		}else
		if ((CString)szTR == TRNO_FAVORITE) //tour2k 관심종목 그룹
		{
			char* pReceiveBuff = new char[dwTRDataLen + 1];
			memset(pReceiveBuff, 0x00, dwTRDataLen + 1);
			memcpy(pReceiveBuff, aTRData, dwTRDataLen);

			if (nKey == TRKEY_GWANSIMGROUP)//관심종목
			{
				CStringArray arrName, arrKey;
				ParseGroup(pReceiveBuff, arrKey, arrName);

			}
			delete[] pReceiveBuff; pReceiveBuff = nullptr;
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

void CMainFrmWnd::Receive_ChartData(char* pszBuff, long nDataLength)
{
	CString strCode, strName;
	KB_p0602_OutRec1_EX *pStOutput = (KB_p0602_OutRec1_EX *)pszBuff;
	long lPosForChart = nDataLength - (&pStOutput->fcnt[0] - &pStOutput->name[0]);

	if (m_pDlgChart && m_pDlgChart->m_ChartItem)
	{
		strCode = m_pCodeInput->GetCode();
		strName = CString(pStOutput->name, sizeof(pStOutput->name));
		strName.TrimRight();

		m_pDlgChart->m_ChartItem.RemoveRealItem();
		m_pDlgChart->m_ChartItem.SetItemCode("DEFAULT", strCode, strName);
		m_pDlgChart->m_ChartItem.AddPacketAndGraph();
		m_pDlgChart->m_ChartItem.SetPacketData(m_pDlgChart->m_ChartItem.GetPacketName(),(long)pStOutput->fcnt, lPosForChart,TRUE);

//		m_pDlgChart->m_ChartItem.SetDrdsAdvise(FALSE);
// 		m_pDlgChart->m_ChartItem.AddRealItem(strCode);		// 실시간등록할 종목코드
// 		m_pDlgChart->m_ChartItem.SetDrdsAdvise(TRUE);

		int nData;
		char*pBuff;
		INT_CONVERT(pBuff, pStOutput->fcnt, sizeof(pStOutput->fcnt), nData);
		
		CString strCnt;
		if(nData < m_nCntInView)
		{
			m_nCntInView = nData;
			strCnt.Format("%d", m_nCntInView);
			m_edtZoomValue.SetWindowText(strCnt);
		}
		
		m_nRestrictedMaxCnt = 0;
		
		if(nData < m_nAllCntInView)
		{
			m_nRestrictedMaxCnt = m_nAllCntInView = nData;
			strCnt.Format("%d", m_nAllCntInView);
			m_editCntOfDatAll.SetWindowText(strCnt);
		}
		
		//m_pDlgChart->m_ChartItem.SetNOnePageDataCount(6);
		if(m_nTabIndex == TAB_PATTERN)
		{
			//	m_pDlgChart->m_ChartItem.SetNOnePageDataCount(6);
			m_pDlgChart->m_ChartItem.SetOnePageDataCount(min(120, nData));
		}
		else
		{
			// nData에 상관없이 60개만 보여주자
			m_pDlgChart->m_ChartItem.SetOnePageDataCount(min(60, nData));
			//m_pDlgChart->m_ChartItem.SetBWholeView(TRUE);
		}
	}
}

void CMainFrmWnd::Receive_ChartData_Second(char* pszBuff, long nDataLength)
{
	CString strCode, strName;
	KB_p0602_OutRec1_EX *pStOutput = (KB_p0602_OutRec1_EX *)pszBuff;
	long lPosForChart = nDataLength - (&pStOutput->fcnt[0] - &pStOutput->name[0]);

	if (m_pDlgChartSecond && m_pDlgChartSecond->m_ChartItem)
	{
		strName = CString(pStOutput->name, sizeof(pStOutput->name));
		strName.TrimRight();

		m_pDlgChartSecond->m_ChartItem.RemoveRealItem();
		m_pDlgChartSecond->m_ChartItem.SetItemCode("DEFAULT", m_strCode, strName);
		m_pDlgChartSecond->m_ChartItem.AddPacketAndGraph();
		m_pDlgChartSecond->m_ChartItem.SetPacketData(m_pDlgChart->m_ChartItem.GetPacketName(),(long)pStOutput->fcnt, lPosForChart,TRUE);
		
		//m_pDlgChartSecond->m_ChartItem.SetDrdsAdvise(FALSE);
		//m_pDlgChartSecond->m_ChartItem.AddRealItem(strCode);		// 실시간등록할 종목코드
		//m_pDlgChartSecond->m_ChartItem.SetDrdsAdvise(TRUE);

		if(m_nTabIndex == TAB_PATTERN)
		{
			BOOL bLoad = m_pDlgChartSecond->m_ChartItem.InvokeAddInStr("ChartPeriodSummaryAddIn.dll:DRAG_ZOOM_ANAL", "0", m_strSectionDate);
			
			char*pBuff;
			int nData;
			INT_CONVERT(pBuff, pStOutput->fcnt, sizeof(pStOutput->fcnt), nData);
			//m_pDlgChartSecond->m_ChartItem.SetNOnePageDataCount(6);
			//2009.12.01 by LYH >> 60일 이평까지 보이도록 개선
			//m_DlgChartSecond.SetOnePageDataCount(nData);
			int nCount = m_SectionDataList.GetCount() + 5;
			m_pDlgChartSecond->m_ChartItem.SetOnePageDataCount(min(nCount, nData));
			//2009.12.01 by LYH <<
		}
		else
		{
			//m_strSectionDate = m_strEndTime + ":" + m_strStartTime;
			m_pDlgChartSecond->m_ChartItem.InvokeAddInStr("ChartPeriodSummaryAddIn.dll:DRAG_CANDLE_ANAL", "0", m_strSectionDate);
			
			char*pBuff;
			int nData;
			INT_CONVERT(pBuff, pStOutput->fcnt, sizeof(pStOutput->fcnt), nData);
			//2009.12.01 by LYH >> 60일 이평까지 보이도록 개선
			//m_DlgChartSecond.SetOnePageDataCount(nData);
			m_pDlgChartSecond->m_ChartItem.SetOnePageDataCount(min(40, nData));
			//2009.12.01 by LYH <<
		}
		//구간 표시
	}
	
	m_gridResult.SetFocus();
}

HBRUSH CMainFrmWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFrameWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC) 
	{
		pDC->SetBkMode(TRANSPARENT);		
		pDC->SetBkColor(m_clrMainSkin);
		//2009.10.08 by LYH >> window7 테마 지원(고대비검정) <<
		pDC->SetTextColor(RGB(0,0,0));
		hbr = m_brushBK;//m_brushStatic
	}
	return hbr;
}

void CMainFrmWnd::ChangeSkinColor(COLORREF clrBkgnd, ST_TABCOLOR stTabColor)
{
	if(!clrBkgnd)
		return;
	
	m_clrMainSkin = clrBkgnd;
	
	m_stTabColor = stTabColor;
	
	if(m_brushBK.DeleteObject())
		m_brushBK.CreateSolidBrush(m_clrMainSkin);
	
	// 	m_ptabCondition->SetBKColor(m_clrMainSkin);	
	// 	m_ptabCondition->SetTabColor(m_clrMainSkin, stTabColor.clrTabSelGraUp, stTabColor.clrTabUnSelGraUp, stTabColor.clrTabSelGraDown, stTabColor.clrTabUnSelGraDown, stTabColor.clrTabBody);
	// 	m_ptabCondition->SetTextColor(stTabColor.clrTabSelText, stTabColor.clrTabUnSelText);
	
	ST_GRIDCOLOR stGridColor;
	stGridColor.clrGridHeader = GetAxColor(74);
	stGridColor.clrGridHilight = GetAxColor(78);
	
	ChangeGridColor(stGridColor);

	m_RadioPattern.Invalidate();
	m_RadioCandle.Invalidate();
	m_btnReverseChk.Invalidate();
	m_btnExceptionChk.Invalidate();
	m_btnIgnoreColorChk.Invalidate();
	m_btnBongChk.Invalidate();
	
	m_stcStatusBar.Invalidate();
	m_ctrlTime.Invalidate();
	m_ctrlNotice.Invalidate();
	m_ctrlTarget.Invalidate();
	m_ctrlBongCntText1.Invalidate();
	m_ctrlBongCntText2.Invalidate();
	m_ctrlSimilarity.Invalidate();
	m_ctrlSlash.Invalidate();
	m_ctrlCodeCntText.Invalidate();
	m_ctrlCodeCnt.Invalidate();
	
	m_comboTarget_2nd.Invalidate();
	m_comboTime.Invalidate();
	m_comboBongTerm.Invalidate();
	m_comboSimilar.Invalidate();

	m_pwndSymbol->SendMessage(WM_USER, MAKEWPARAM(DLL_SETPAL, 0));
	m_pwndSymbol->Invalidate();

	Invalidate();	
}

void CMainFrmWnd::ChangeGridColor(ST_GRIDCOLOR stGridColor)
{
	m_gridResult.ChangeGridColor(stGridColor);
	m_gridResult.Invalidate();
}


void CMainFrmWnd::SetOpenScreen(CString strMapNo, CString strSendData)
{
//	m_pIAUFormOpenDispMng->OpenMap(m_dwFormOpenManagerKey, strMapNo, strSendData, strSendData.GetLength());
}

void CMainFrmWnd::SetLinkCode(CString strCode, BOOL bTagData)
{
	if (strCode.IsEmpty() == TRUE)
		return;
	
}

CString CMainFrmWnd::GetCodeToName(CString strCode)
{
	char szBuf[16];
	sprintf(szBuf, "%s ", strCode);
	CString strName = (char*)m_pScreenWnd->GetModuleMainWnd()->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)szBuf);
	strName.TrimLeft();
	strName.TrimRight();
	
	return strName;
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

void CMainFrmWnd::CreateDlgChart()
{
	DestroyChart();
	// 차트 DLG 생성
	HINSTANCE hInstResourceClient = AfxGetResourceHandle();
	AfxSetResourceHandle(theApp.m_hInstance);
	m_pDlgChart		  =	new CDlgChart_Stock;
	m_pDlgChart->Create(IDD_DLG_CHART, this );// m_pParent);
	m_pDlgChart->ModifyStyle(0, WS_CLIPCHILDREN);
	m_pDlgChart->m_ChartItem.SetMultiItemMode(TRUE);
	
	//차트의 기본 메뉴 보이지 않게 처리
	m_pDlgChart->m_ChartItem.SetBAddGraphMenuShow(FALSE);
	
	if(m_pDlgChart)
		m_pDlgChart->m_ChartItem.SetOutsideCommonInterface((long)((CMChartInterface*)this));
	
	m_pDlgChartSecond = new CDlgChart_Stock;
	m_pDlgChartSecond->Create(IDD_DLG_CHART, this);
	m_pDlgChartSecond->ModifyStyle(0, WS_CLIPCHILDREN);
	m_pDlgChartSecond->m_ChartItem.SetMultiItemMode(TRUE);
	AfxSetResourceHandle(hInstResourceClient);
	
	m_pDlgChart->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE ); //그룹 박스 위로
	
	m_pDlgChart->ShowWindow(SW_SHOW);
	m_pDlgChartSecond->ShowWindow(SW_SHOW);
	
	m_pDlgChartSecond->m_ChartItem.InvokeAddInStr("ChartPeriodSummaryAddIn.dll:DRAG_ZOOM_ANAL", "0", m_strSectionDate);
}

void CMainFrmWnd::DestroyChart()
{
	if(m_pDlgChart)			// 차트 Dlg
	{
		if(m_pDlgChart->m_hWnd && IsWindow(m_pDlgChart->m_hWnd))
			m_pDlgChart->DestroyWindow();
		delete m_pDlgChart;
		m_pDlgChart = NULL;
	}
	if(m_pDlgChartSecond)
	{
		if(m_pDlgChartSecond->m_hWnd && IsWindow(m_pDlgChartSecond->m_hWnd))
			m_pDlgChartSecond->DestroyWindow();
		delete m_pDlgChartSecond;
		m_pDlgChartSecond = NULL;
	}
}

void CMainFrmWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CBrush BackBrush(m_clrMainSkin);

 	CRect rcClient;
 	GetClientRect(&rcClient);
  	dc.FillSolidRect(rcClient, m_clrMainSkin);

	CRect rcFrame = m_rcRdFrame;

// 	dc.Draw3dRect(rcFrame, RGB(190,190,190),RGB(190,190,190));
//  	rcFrame.DeflateRect(1,1);
// 	dc.Draw3dRect(rcFrame, RGB(255,255,255),RGB(255,255,255));
}


void CMainFrmWnd::SendTr_PatternData(CList<CString, CString&>* pListData)  //구간 검색 TR전송
{
	m_ctrlState.SetWindowText("진행상태 : [조회중]");  

	POSITION pos = pListData->GetHeadPosition();	

	DRAW_IN stDraw_IN;
	memset((void*)&stDraw_IN, 0x20, SZ_DRAW_IN);

	if(SetMakePatternPacket(stDraw_IN, pListData) == TRUE)	//Packet Data 구성
	{
// 		if(!pos || pListData->GetCount() == 0)
// 		return;
// 
// 		stDraw_IN.nTrIndex = GetTrIndex();  //Tr Index ++
// 
// 		SetByteOrdering_PatternIn(&stDraw_IN);  //Byte Ordering
// 
// 		char szData[SZ_DRAW_IN+1];
// 		memset(szData, 0x00, sizeof(szData));
// 		memcpy((void *)&szData, (void *)&stDraw_IN, SZ_DRAW_IN);
// 		szData[SZ_DRAW_IN] = '\0';

		p0624InBlock* pst0624;
		pst0624 = new p0624InBlock;
		SetConvert_SendPatternPacket(stDraw_IN, pst0624);

		m_WaitCursor.BWaitCursorIntelli();

		m_pScreenWnd->SendTR(TRNO_DRAW_RESULT, (char*)pst0624, SZ_DRAW_IN, TRKEY_DRAW_RESULT, US_KEY);
	}
	else 
	{
		m_WaitCursor.EWaitCursor(TRUE);
		m_gridResult.ClearAll();
		m_ctrlState.SetWindowText("진행상태 : [검색할 패턴이 존재하지 않습니다.]"); 
		SetEnable(TRUE);
	}
		

}

void CMainFrmWnd::SendTr_CandleData(CList<CString, CString&>* pListData)  //구간 검색 TR전송
{
	m_ctrlState.SetWindowText("진행상태 : [조회중]");  

	CANDLE_IN stCandle_IN;
	memset((void*)&stCandle_IN, 0x20, sizeof(CANDLE_IN));

	if(SetMakeCandlePacket(stCandle_IN, pListData) == TRUE)	//Packet Data 구성
	{
// 		stCandle_IN.nTrIndex = GetTrIndex();  //Tr Index ++
// 
// 		SetBtyeOrdering_CandleIn(stCandle_IN);  //Byte Ordering
// 
// 		char szData[sizeof(CANDLE_IN)+1];
// 		memset(szData, 0x00, sizeof(szData));
// 		memcpy((void *)&szData, (void *)&stCandle_IN, sizeof(CANDLE_IN));
// 		szData[sizeof(CANDLE_IN)] = '\0';
// 
// 		m_WaitCursor.BWaitCursorIntelli();
// 
// 		m_pTRComm->SetDestination('V');
// 		m_pTRComm->SetTR(TRNO_CANDLE_RESULT);
// 		m_pTRComm->SetBaseData("SCREENCODE", m_strScreenNo);
// 		
// 		if(!m_pDataUtilMng)
// 		{
// 			m_pDataUtilMng = (IDataUtilManager2*)AfxGetPctrInterface(UUID_IDataUtilManager2, 1);
// 		}
// 		
// 		m_pTRComm->Send2Server(szData, sizeof(CANDLE_IN), FALSE);
		HKST03660000_IN1* pstHKST03660000_IN;
		pstHKST03660000_IN = new HKST03660000_IN1;
		SetConvert_SendPatternPacket(stCandle_IN, pstHKST03660000_IN);

		int nBufSize = sizeof(HKST03660000_IN1);
		char szData[sizeof(HKST03660000_IN1) + 1];
		memset(szData, 0x20, sizeof(szData));
		szData[nBufSize] = NULL;
		memcpy((void *)szData, (void *)pstHKST03660000_IN, sizeof(HKST03660000_IN1));

		m_WaitCursor.BWaitCursorIntelli();

		m_pScreenWnd->SendTR(TRNO_CANDLE_RESULT, (char*)szData, SZ_DRAW_IN, TRKEY_CANDLE_RESULT, US_KEY);

		delete pstHKST03660000_IN;

		SetEnable(FALSE);

	}
	else 
		SetEnable(TRUE);
}

int CMainFrmWnd::GetTrIndex()
{
	m_nLastTrIndex = m_nTrIndex++;
	return m_nLastTrIndex;
}

int CMainFrmWnd::GetBongGap()
{
	CString strCount;
	m_editBongCnt.GetWindowText(strCount);

	int nGap = atoi(strCount);

	return nGap;
}

int CMainFrmWnd::GetIgnoreColor()	// 양음봉 무시
{
	return m_btnIgnoreColorChk.GetCheck();
}

int CMainFrmWnd::GetIgnoreTail()	// 꼬리 무시
{
	return m_comboTail.GetCurSel();
}

void CMainFrmWnd::SetByteOrdering_PatternIn(DRAW_IN *pstDraw_IN)
{
	pstDraw_IN->nTrIndex = ntohl(pstDraw_IN->nTrIndex);
	pstDraw_IN->nS_Time  = ntohl(pstDraw_IN->nS_Time);
	pstDraw_IN->lTarget  = ntohl(pstDraw_IN->lTarget);
	pstDraw_IN->lMoveAvg = ntohl(pstDraw_IN->lMoveAvg);
	pstDraw_IN->lS_Pos   = ntohl(pstDraw_IN->lS_Pos);
	pstDraw_IN->lS_Range = ntohl(pstDraw_IN->lS_Range);
	pstDraw_IN->lSimilar = ntohl(pstDraw_IN->lSimilar);
	
	int index = 0;
	for(index=0; index < pstDraw_IN->nFieldEditerCnt; index++)
	{
		pstDraw_IN->nFieldList[index] = ntohl(pstDraw_IN->nFieldList[index]);
	}
	for(index=0; index < pstDraw_IN->stDrawData.lDraw_Cnt; index++)
	{
		pstDraw_IN->stDrawData.lDraw_Value[index] = ntohl(pstDraw_IN->stDrawData.lDraw_Value[index]);
	}
	for(index=pstDraw_IN->stDrawData.lDraw_Cnt; index < 120; index++)
	{
		pstDraw_IN->stDrawData.lDraw_Value[index] = 0;
	}
	pstDraw_IN->nFieldEditerCnt	       = ntohl(pstDraw_IN->nFieldEditerCnt);
	pstDraw_IN->stDrawData.lAccuVal    = ntohl(pstDraw_IN->stDrawData.lAccuVal);
	pstDraw_IN->stDrawData.lDraw_Cnt   = ntohl(pstDraw_IN->stDrawData.lDraw_Cnt);
	pstDraw_IN->stDrawData.lErrRate    = ntohl(pstDraw_IN->stDrawData.lErrRate);
	pstDraw_IN->stDrawData.lGapRate    = ntohl(pstDraw_IN->stDrawData.lGapRate);

}

void CMainFrmWnd::SetBtyeOrdering_CandleIn(CANDLE_IN	&stCandleIn)
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
	}
}

BOOL CMainFrmWnd::SetReceiveData_Pattern(LPTSTR pData, DWORD dwDataLen)
{
	///////////////////////////////////////////////////////////////
	// 20100412 박찬모 한투 >> 
	/*{
// 		DRAW_OUT_HEAD	stDraw_OUT_HEAD;
// 		memset(&stDraw_OUT_HEAD, 0x00, SZ_DRAW_OUT_HEAD);
// 		memcpy(&stDraw_OUT_HEAD, pData, SZ_DRAW_OUT_HEAD);
// 
// 		stDraw_OUT_HEAD.nTrIndex		    = ntohl(stDraw_OUT_HEAD.nTrIndex);
// 		stDraw_OUT_HEAD.lTotalCnt   	    = ntohl(stDraw_OUT_HEAD.lTotalCnt); //검색된 종목 수
// 		stDraw_OUT_HEAD.lOutFieldCnt    	= ntohl(stDraw_OUT_HEAD.lOutFieldCnt);  //Out Filed Count
// 		stDraw_OUT_HEAD.lOutListPacketSize  = ntohl(stDraw_OUT_HEAD.lOutListPacketSize); //한 종목 사이즈
// 		stDraw_OUT_HEAD.lFindTime		    = ntohl(stDraw_OUT_HEAD.lFindTime); //검색 시간

		p0624OutBlock1 st0624Out;
		memset(&st0624Out, 0x00, SZ_p0624OutBlock1);
		memcpy(&st0624Out, pData, SZ_p0624OutBlock1);

		char* pBuff;
		DRAW_OUT_HEAD	stDraw_OUT_HEAD;
		memset(&stDraw_OUT_HEAD, 0x00, sizeof(DRAW_OUT_HEAD));

		LONG_CONVERT(pBuff, st0624Out.nTrIndex,			 4, stDraw_OUT_HEAD.nTrIndex);
		LONG_CONVERT(pBuff, st0624Out.lTotalJongCnt,      4, stDraw_OUT_HEAD.lTotalCnt);
		LONG_CONVERT(pBuff, st0624Out.lOutFieldCnt,	 	 4, stDraw_OUT_HEAD.lOutFieldCnt);
		LONG_CONVERT(pBuff, st0624Out.lOutListPacketSize, 4, stDraw_OUT_HEAD.lOutListPacketSize);
		LONG_CONVERT(pBuff, st0624Out.lFindTime,			 6, stDraw_OUT_HEAD.lFindTime);


		if(stDraw_OUT_HEAD.lTotalCnt == 0)
		{
			SetSearchResultState(0, -1);
			return FALSE;
		}


		if(m_nLastTrIndex != (int)stDraw_OUT_HEAD.nTrIndex)
			return TRUE;

		m_gridResult.ClearAll();
		m_gridResult.SetNumberRows(stDraw_OUT_HEAD.lTotalCnt);

		pData = &pData[SZ_DRAW_OUT_HEAD];
		dwDataLen = dwDataLen - SZ_DRAW_OUT_HEAD;

		int nRow = 0, nIndex_Field, nTotalCount = stDraw_OUT_HEAD.lTotalCnt;
		//	int nPrefferdItem_Count = 0; //우선주 카운트
		CString strData;

		DRAW_OUT_LIST	stDraw_OUT_LIST;
		for(int nIndex = 0; nIndex < stDraw_OUT_HEAD.lTotalCnt; nIndex++)
		{
			memset(&stDraw_OUT_LIST, 0x00, SZ_DRAW_OUT_LIST);
			memcpy(&stDraw_OUT_LIST, pData, stDraw_OUT_HEAD.lOutListPacketSize);

			stDraw_OUT_LIST.iTarget = ntohl(stDraw_OUT_LIST.iTarget);
			for (nIndex_Field = 0; nIndex_Field < stDraw_OUT_HEAD.lOutFieldCnt; nIndex_Field++)
				SetByteOrderRevDouble((char far *)&(stDraw_OUT_LIST.dFieldData[nIndex_Field]));

			//# 우선주 제외
			if (m_btnExceptionChk.GetCheck() == TRUE)
			{
				STR_CONVERT(pBuff, stDraw_OUT_LIST.sJmcode, sizeof(stDraw_OUT_LIST.sJmcode), strData);
				strData.TrimLeft(" ");
				strData.TrimRight(" ");

				if (atoi(strData.Right(1)) > 0)
				{
					pData = &pData[stDraw_OUT_HEAD.lOutListPacketSize];
					nTotalCount--;
					continue;
				}
			}

			m_gridResult.SetAddRow(nRow, stDraw_OUT_HEAD.lOutFieldCnt, stDraw_OUT_LIST);
			pData = &pData[stDraw_OUT_HEAD.lOutListPacketSize];
			nRow++;
		}
		m_gridResult.SetNumberRows(nRow);
		m_gridResult.RedrawAll();

		SetSearchResultState(nTotalCount, stDraw_OUT_HEAD.lFindTime);

		if (nTotalCount > 0)
			m_gridResult.SetSelectRow(0);

		return TRUE;
	}*/
	// 20100412 한투 <<
	///////////////////////////////////////////////////////////////
	//20100211 SJ_KIM
	///////////////////////////////////////////////////////
	
	p0624OutBlock1 st0624Out;
	memset(&st0624Out, 0x00, SZ_p0624OutBlock1);
	memcpy(&st0624Out, pData, SZ_p0624OutBlock1);

	char* pBuff;
	DRAW_OUT_HEAD	stDRAW_OUT_HEAD;
	memset(&stDRAW_OUT_HEAD, 0x00, sizeof(DRAW_OUT_HEAD));

	LONG_CONVERT(pBuff, st0624Out.nTrIndex,			 4, stDRAW_OUT_HEAD.nTrIndex);
	LONG_CONVERT(pBuff, st0624Out.lTotalJongCnt,      4, stDRAW_OUT_HEAD.lTotalCnt);
	LONG_CONVERT(pBuff, st0624Out.lOutFieldCnt,	 	 4, stDRAW_OUT_HEAD.lOutFieldCnt);
	LONG_CONVERT(pBuff, st0624Out.lOutListPacketSize, 4, stDRAW_OUT_HEAD.lOutListPacketSize);
	LONG_CONVERT(pBuff, st0624Out.lFindTime,			 6, stDRAW_OUT_HEAD.lFindTime);

	m_gridResult.ClearAll();
	m_gridResult.SetNumberRows(stDRAW_OUT_HEAD.lTotalCnt);

// 	m_Chart.InvokeAddInStr("ChartPeriodSummaryAddIn.dll:DRAG_PATTERN_ANAL", "0", "");
// 	m_Chart.ClearChartData(0);

	if (stDRAW_OUT_HEAD.lTotalCnt == 0)
	{
		SetSearchResultState(0, -1);
		Invalidate();
		return FALSE;
	}

	pData = &pData[SZ_p0624OutBlock1];	

	DRAW_OUT_LIST stDRAW_OUT_LIST;
	int nSize = sizeof(DRAW_OUT_LIST);
	int nIndex;

	CString strData;
	int nRow = 0, nTotalCount = stDRAW_OUT_HEAD.lTotalCnt;
	p0624OutBlock2 st0624List;

	for (nIndex = 0; nIndex < stDRAW_OUT_HEAD.lTotalCnt; nIndex++)
	{
		memset(&st0624List, 0x00, SZ_p0624OutBlock2);
		memcpy(&st0624List, pData, SZ_p0624OutBlock2);

		memset(&stDRAW_OUT_LIST, 0x00, SZ_DRAW_OUT_LIST);
		Safe_CheckCopy2(stDRAW_OUT_LIST.sJmcode,    st0624List.sJmcode, sizeof(stDRAW_OUT_LIST.sJmcode), strData);
		Safe_CheckCopy2(stDRAW_OUT_LIST.sHname,		st0624List.sHname, sizeof(stDRAW_OUT_LIST.sHname), strData);
		Safe_CheckCopy2(stDRAW_OUT_LIST.sSign,		st0624List.sSign, sizeof(stDRAW_OUT_LIST.sSign), strData);
		Safe_CheckCopy2(stDRAW_OUT_LIST.sMarket,	st0624List.sMarket, sizeof(stDRAW_OUT_LIST.sMarket), strData);
		LONG_CONVERT(pBuff, st0624List.iTarget,			 4, stDRAW_OUT_LIST.iTarget	);
		Safe_CheckCopy2(stDRAW_OUT_LIST.sDateTime_Start, st0624List.sDateTime_Start, sizeof(stDRAW_OUT_LIST.sDateTime_Start), strData);
		Safe_CheckCopy2(stDRAW_OUT_LIST.sDateTime_End,   st0624List.sDateTime_End, sizeof(stDRAW_OUT_LIST.sDateTime_End), strData);

		CString strFieldData="";
		for(int nFieldIndex = 0; nFieldIndex < stDRAW_OUT_HEAD.lOutFieldCnt; nFieldIndex++)
		{		
			STR_CONVERT(pBuff, &st0624List.dFieldData[nFieldIndex*16], 16, strFieldData);
			stDRAW_OUT_LIST.dFieldData[nFieldIndex] = atof(strFieldData);
		}

		for(int i = 0; i < 5; i++) // 종, 전일 종, 시, 고, 저
		{
			char szTemp[16];
			memset(szTemp, 0x00, sizeof(szTemp));
			memcpy(szTemp, &st0624List.dFieldData[(55 + i) * 16], 16);
			m_arCandleData[i] = atoi(szTemp);
		}
		
		//# 우선주 제외
		if (m_btnExceptionChk.GetCheck() == TRUE)
		{
			STR_CONVERT(pBuff, stDRAW_OUT_LIST.sJmcode, sizeof(stDRAW_OUT_LIST.sJmcode), strData);
			strData.TrimLeft(" ");
			strData.TrimRight(" ");

			if (atoi(strData.Right(1)) > 0)
			{
				pData = &pData[SZ_p0624OutBlock2];
				nTotalCount--;
				continue;
			}
		}

		m_gridResult.SetAddRow(nRow, stDRAW_OUT_HEAD.lOutFieldCnt, stDRAW_OUT_LIST, m_arCandleData);
		pData = &pData[SZ_p0624OutBlock2];
		nRow++;
	}
	m_gridResult.SetNumberRows(nRow);
	CUGHScroll* pGridHScroll = m_gridResult.m_CUGHScroll;
	pGridHScroll->SetScrollPos(0, FALSE);
	m_gridResult.RedrawAll();

	SetSearchResultState(nTotalCount, stDRAW_OUT_HEAD.lFindTime);

	if (nTotalCount > 0)
		m_gridResult.SetSelectRow(0);

	return TRUE;

}

// 캔들 패턴 검색 결과
BOOL CMainFrmWnd::SetReceiveData_Candle(LPTSTR pData, DWORD dwDataLen)
{
	/*{
		CANDLE_OUT_HEAD	stCANDLE_OUT_HEAD;
		::memcpy(&stCANDLE_OUT_HEAD, pData, sizeof(CANDLE_OUT_HEAD));

		stCANDLE_OUT_HEAD.nTrIndex = ntohl(stCANDLE_OUT_HEAD.nTrIndex);

		if (m_nLastTrIndex != stCANDLE_OUT_HEAD.nTrIndex)
			return FALSE;

		stCANDLE_OUT_HEAD.lTotalCnt = ntohl(stCANDLE_OUT_HEAD.lTotalCnt);
		stCANDLE_OUT_HEAD.lOutFieldCnt = ntohl(stCANDLE_OUT_HEAD.lOutFieldCnt);
		stCANDLE_OUT_HEAD.lOutListPacketSize = ntohl(stCANDLE_OUT_HEAD.lOutListPacketSize);
		stCANDLE_OUT_HEAD.lFindTime = ntohl(stCANDLE_OUT_HEAD.lFindTime);

		m_strStartTime = CString(stCANDLE_OUT_HEAD.sDateTime_Start, sizeof(stCANDLE_OUT_HEAD.sDateTime_Start));
		m_strEndTime = CString(stCANDLE_OUT_HEAD.sDateTime_End, sizeof(stCANDLE_OUT_HEAD.sDateTime_End));

		m_gridResult.ClearAll();
		m_gridResult.SetNumberRows(stCANDLE_OUT_HEAD.lTotalCnt);
		m_gridResult.m_strDate = m_strEndTime;

		// 	m_pDlgChartSecond->m_ChartItem.InvokeAddInStr("ChartPeriodSummaryAddIn.dll:DRAG_CANDLE_ANAL", "0", "");
		// 	m_pDlgChartSecond->m_ChartItem.ClearChartData(0);


		if (stCANDLE_OUT_HEAD.lTotalCnt == 0)
		{
			SetSearchResultState(0, -1);
			//		SetStateTitleBar("[조회 결과 없음.]");
			return FALSE;
		}

		pData = &pData[sizeof(CANDLE_OUT_HEAD)];

		CANDLE_OUT_LIST stCANDLE_OUT_LIST;
		int nSize = sizeof(CANDLE_OUT_LIST);
		int nIndex, nIndex_Field;

		char *pBuff;
		CString strData;
		int nRow = 0, nTotalCount = stCANDLE_OUT_HEAD.lTotalCnt;
		for (nIndex = 0; nIndex < stCANDLE_OUT_HEAD.lTotalCnt; nIndex++)
		{
			memset(&stCANDLE_OUT_LIST, 0x00, stCANDLE_OUT_HEAD.lOutListPacketSize);
			memcpy(&stCANDLE_OUT_LIST, pData, stCANDLE_OUT_HEAD.lOutListPacketSize);

			//		stCANDLE_OUT_LIST.iTarget = ntohl(stCANDLE_OUT_LIST.iTarget);
			for (nIndex_Field = 0; nIndex_Field < stCANDLE_OUT_HEAD.lOutFieldCnt; nIndex_Field++)
				SetByteOrderRevDouble((char far *)&(stCANDLE_OUT_LIST.dFieldData[nIndex_Field]));

			//# 우선주 제외
			if (m_btnExceptionChk.GetCheck() == TRUE)
			{
				STR_CONVERT(pBuff, stCANDLE_OUT_LIST.sJmcode, sizeof(stCANDLE_OUT_LIST.sJmcode), strData);
				strData.TrimLeft(" ");
				strData.TrimRight(" ");

				if (atoi(strData.Right(1)) > 0)
				{
					pData = &pData[stCANDLE_OUT_HEAD.lOutListPacketSize];
					nTotalCount--; // 우선주 제외시 검색결과 수량 조정
					continue;
				}
			}

			m_gridResult.SetAddRow(nRow, stCANDLE_OUT_HEAD.lOutFieldCnt, stCANDLE_OUT_LIST);
			pData = &pData[stCANDLE_OUT_HEAD.lOutListPacketSize];
			nRow++;
		}
		m_gridResult.SetNumberRows(nRow);
		m_gridResult.RedrawAll();

		SetSearchResultState(nTotalCount, stCANDLE_OUT_HEAD.lFindTime);

		if (nTotalCount > 0)
			m_gridResult.SetSelectRow(0);
	}*/

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

// 	m_strStartTime = CString(stHKST03660000_OUT1.sStartDateTime, sizeof(stHKST03660000_OUT1.sStartDateTime));
// 	m_strEndTime = CString(stHKST03660000_OUT1.sEndDateTime, sizeof(stHKST03660000_OUT1.sEndDateTime));

	m_gridResult.ClearAll();
	m_gridResult.SetNumberRows(stCANDLE_OUT_HEAD.lTotalCnt);
	m_gridResult.m_strDate = m_strEndTime;

	//m_pDlgChart->m_ChartItem.InvokeAddInStr("ChartPeriodSummaryAddIn.dll:DRAG_CANDLE_ANAL", "0", "");
	//m_pDlgChart->m_ChartItem.ClearChartData(0);

	if (stCANDLE_OUT_HEAD.lTotalCnt == 0)
	{
		SetSearchResultState(0, -1);
// 		m_strItemName.Empty();
// 		m_strItemPrice.Empty();
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

		m_gridResult.SetAddRow(nRow, stCANDLE_OUT_HEAD.lOutFieldCnt, stCANDLE_OUT_LIST, m_arCandleData);
		nRow++;
	}

	m_gridResult.SetNumberRows(nRow);
	CUGHScroll* pGridHScroll = m_gridResult.m_CUGHScroll;
	pGridHScroll->SetScrollPos(0, FALSE);
	m_gridResult.RedrawAll();

	SetSearchResultState(nTotalCount, stCANDLE_OUT_HEAD.lFindTime);

	if (nTotalCount > 0)
		m_gridResult.SetSelectRow(0);


	return TRUE;
}

void CMainFrmWnd::SetByteOrderRevDouble(LPSTR pDouble)
{
	int nCount, nLength = 4;

	for(nCount=0; nCount < nLength; nCount++)
	{
		unsigned char chTemp;
		chTemp = *(pDouble + nCount);
		*(pDouble + nCount) = *(pDouble + sizeof(double) - 1 - nCount);
		*(pDouble + sizeof(double) - 1 - nCount) = chTemp;
	}
}

void CMainFrmWnd::SetSearchResultState(LONG lCnt/* = 0*/, LONG lTime/* = -1*/)
{
	CString strResult;
	CString strTime, strTemp;
	strResult.Format(" %d건", lCnt);
	
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

	m_ctrlState.SetWindowText("진행상태 : [조회 완료]");
}

void CMainFrmWnd::ResendTR()
{
	if(m_SectionDataList.GetCount() == 0)
	{
		SetMessageBox("검색할 구간이 지정되지 않았습니다.", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	if(m_nTabIndex == TAB_PATTERN)
		SendTr_PatternData(&m_SectionDataList); 		
	else
		SendTr_CandleData(&m_SectionDataList);

	SetEnable(FALSE);
}

void CMainFrmWnd::OnBtnChkBong()
{ 
	m_comboBongTerm.EnableWindow(m_btnBongChk.GetCheck());
	m_comboBongTerm.Invalidate();

//     ResendTR();
}

void CMainFrmWnd::OnChgBongCombo()
{
//    ResendTR();
}

void CMainFrmWnd::OnChgSimCombo()
{
 //   ResendTR();
}

void CMainFrmWnd::OnBtnSearch()
{
	ResendTR();
}

void CMainFrmWnd::OnBtnDay()   
{	
	if(m_strChartTerm != "2")
		m_SectionDataList.RemoveAll();

	m_strChartTerm = "2"; //일
	m_comboTime.EnableWindow(FALSE);
	m_comboTime.Invalidate();

	m_btnDay.SetCheck(TRUE);
	m_btnWeek.SetCheck(FALSE);
	m_btnMonth.SetCheck(FALSE);
	m_btnMin.SetCheck(FALSE);
	
	m_nCntInView = m_nAllCntInView = (m_nTabIndex == TAB_PATTERN)?120:60;
	CString strCnt;
	strCnt.Format("%d", m_nCntInView);
	m_edtZoomValue.SetWindowText(strCnt);

	strCnt.Format("%d", m_nAllCntInView);
	m_editCntOfDatAll.SetWindowText(strCnt);

	CString strCode = m_pCodeInput->GetCode();
	if(strCode != "")
	{
		SetChartActivation(CENTER_CHART);
		SendTr_ChartData(strCode);
	}
}

void CMainFrmWnd::OnBtnWeek()
{
	if(m_strChartTerm != "3")
		m_SectionDataList.RemoveAll();

	m_strChartTerm = "3"; //주
	m_comboTime.EnableWindow(FALSE);
	m_comboTime.Invalidate();

	m_btnDay.SetCheck(FALSE);
	m_btnWeek.SetCheck(TRUE);
	m_btnMonth.SetCheck(FALSE);
	m_btnMin.SetCheck(FALSE);

	m_nCntInView = m_nAllCntInView = (m_nTabIndex == TAB_PATTERN)?120:60;
	CString strCnt;
	strCnt.Format("%d", m_nCntInView);
	m_edtZoomValue.SetWindowText(strCnt);

	strCnt.Format("%d", m_nAllCntInView);
	m_editCntOfDatAll.SetWindowText(strCnt);
	
	CString strCode = m_pCodeInput->GetCode();
	if(strCode != "")
	{
		SetChartActivation(CENTER_CHART);
		SendTr_ChartData(strCode);
	}
}

void CMainFrmWnd::OnBtnMonth()
{
	if(m_strChartTerm != "4")
		m_SectionDataList.RemoveAll();

	m_strChartTerm = "4"; //월
	m_comboTime.EnableWindow(FALSE);
	m_comboTime.Invalidate();

	m_btnDay.SetCheck(FALSE);
	m_btnWeek.SetCheck(FALSE);
	m_btnMonth.SetCheck(TRUE);
	m_btnMin.SetCheck(FALSE);
	
	m_nCntInView = m_nAllCntInView = (m_nTabIndex == TAB_PATTERN)?120:60;
	CString strCnt;
	strCnt.Format("%d", m_nCntInView);
	m_edtZoomValue.SetWindowText(strCnt);

	strCnt.Format("%d", m_nAllCntInView);
	m_editCntOfDatAll.SetWindowText(strCnt);

	CString strCode = m_pCodeInput->GetCode();
	if(strCode != "")
	{
		SetChartActivation(CENTER_CHART);
		SendTr_ChartData(strCode);
	}
}

void CMainFrmWnd::OnBtnMinute()
{
	if(m_strChartTerm != "1")
		m_SectionDataList.RemoveAll();

	m_strChartTerm = "1";  //분
	m_comboTime.EnableWindow(TRUE);
	m_comboTime.Invalidate();

	m_btnDay.SetCheck(FALSE);
	m_btnWeek.SetCheck(FALSE);
	m_btnMonth.SetCheck(FALSE);
	m_btnMin.SetCheck(TRUE);
	
	m_nCntInView = m_nAllCntInView = (m_nTabIndex == TAB_PATTERN)?120:60;
	CString strCnt;
	strCnt.Format("%d", m_nCntInView);
	m_edtZoomValue.SetWindowText(strCnt);

	strCnt.Format("%d", m_nAllCntInView);
	m_editCntOfDatAll.SetWindowText(strCnt);
	
	CString strCode = m_pCodeInput->GetCode();
	if(strCode != "")
	{
		SetChartActivation(CENTER_CHART);
		SendTr_ChartData(strCode);
	}
}

void CMainFrmWnd::SetChartActivation(int chart)
{
	m_chartActivation = chart;
}

void CMainFrmWnd::Set_SectionDate(CString strDate)
{
	m_strSectionDate = strDate;
	m_strSectionDate = strDate.Left(14);
	m_strSectionDate += ":";
	m_strSectionDate += strDate.Right(14);
}

LPCTSTR CMainFrmWnd::GetDateTime(WPARAM wParam, LPARAM lParam)
{
	if(m_nTabIndex != TAB_PATTERN)
		m_strSectionDate = m_strEndTime + ":" + m_strStartTime;

	return (LPCTSTR)m_strSectionDate;
}

void CMainFrmWnd::OnPeriodSelection() //분 콤보
{
	CString minTerm = "";
	CString strCode = m_pCodeInput->GetCode();

	minTerm.Format("%03d", atoi(LIST_MINTERM [m_comboTime.GetCurSel()]));
	m_interval = minTerm;

	if(strCode != "")
	{			
		SetChartActivation(CENTER_CHART);
		SendTr_ChartData(strCode);
	}	
}


BOOL CMainFrmWnd::SetLoad_TargetBase_2nd(CString strTargetBaseType)
{
	BOOL bResult = TRUE;
	//# 0 : 전체, 1:Kospi, 2:Kosdak, 3:테마, 4:관심
	
	int nTargetBaseType_0st, nTargetBaseType_1st;
	GetTargetBaseType(nTargetBaseType_0st, nTargetBaseType_1st);

	if (nTargetBaseType_0st == 1)// kospi
		SetLoad_CUMaster();
	else if (nTargetBaseType_0st == 2)// kosdaq
		SetLoad_CKUMaster();

///////////////////////////////////////////////////////////
// 20100414 박찬모 테마 삭제 >>
// 	else if (nTargetBaseType_0st == 3)	//# 테마
// 		SetLoad_CTHMaster();
// 20100414 박찬모 테마 삭제 <<
//	else if (nTargetBaseType_0st == 4)	//# 관심
//		SetLoad_PortGroup();

	else if (nTargetBaseType_0st == 3)	//# 관심
		SetLoad_PortGroup();
///////////////////////////////////////////////////////////
	
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
	
///////////////////////////////////////////////////////////
// 20100414 박찬모 테마 삭제 >>
// 	else if (nTargetBaseType_0st == 3)	//# 테마
// 	{
// 		strCode.Format("%05ld", lCode);
// 		GetData_ThemaToCode(strCode);
// 	}
// 	else if (nTargetBaseType_0st == 4)		//# 관심
// 	{
// 		//strCode.Format("%02ld", lCode);
// 		GetData_PortGroupToCode(strName, &m_saCodeGroup);
// 	}

	else if (nTargetBaseType_0st == 3)		//# 관심
	{
		strCode.Format("%02ld", lCode);
		GetData_PortGroupToCode(strCode, &m_saCodeGroup);
	}	
// 20100414 박찬모 테마 삭제 <<
///////////////////////////////////////////////////////////
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

BOOL CMainFrmWnd::SetMakeCandlePacket(CANDLE_IN	&stCandleIn, CList<CString, CString&>* pListChartData)
{
	int nTot = pListChartData->GetCount();
	int nCandleCnt = min(5, nTot/4);
	BOOL bResult = FALSE;

// 	int	nS_Time = atoi(m_strChartTerm) - 2 ;
// 
// 	if(nS_Time == -1)
// 		nS_Time = m_comboTime.GetCurSel() + 3; 

	m_nTrIndex++;

	stCandleIn.nTrIndex = m_nTrIndex;
	
	stCandleIn.iS_Time = GetSearchTimeTerm();
	stCandleIn.iS_Range = nCandleCnt;

// 	if(stCandleIn.iS_Range == 0)
// 	{
// //		MessageBox("선택된 봉이 없습니다.", "캔들패턴", MB_ICONWARNING | MB_OK);
// 		return FALSE;
// 	}

	//////////////////////////////////////////////////////////////////////////
	//  캔들 패턴 검색 데이터로 변환
	POSITION pos;
	int nIndex = 0, nCandle;
	int nPrice, nMax = INT_MIN, nMin = INT_MAX;
	CString strData;

	for(int nCndl = 0; nCndl < MAX_CANDLE; nCndl++)
		memset(&stCandleIn.tCandleData[nCndl], 0x20, sizeof(CANDLE_IN_DATA));
	
	// Min, Max 얻기
	for(pos = pListChartData->GetTailPosition(); pos != NULL;)
	{
		/*strData = pListChartData->GetPrev(pos);*/
		strData.Format("%s", pListChartData->GetPrev(pos));

		//종가->저->고->시->종->저->고->시 ....
		nPrice = atoi(strData);
		if(nPrice > nMax) nMax = nPrice;
		if(nPrice < nMin) nMin = nPrice;

		nCandle = nIndex / 4;

		if(nIndex % 4 == 0) //종
			stCandleIn.tCandleData[nCandle].iClose = nPrice;
		if(nIndex % 4 == 1) //저
			stCandleIn.tCandleData[nCandle].iLow = nPrice;
		if(nIndex % 4 == 2) //고
			stCandleIn.tCandleData[nCandle].iHigh = nPrice;
		if(nIndex % 4 == 3) //시
			stCandleIn.tCandleData[nCandle].iOpen = nPrice;


		nIndex++;
		if(nIndex >= nCandleCnt * 4)
			break;
	}

	double dUnit = ((double)nMax - (double)nMin) / 100.;
	
	// 0 ~ 100% 값으로 변환
	for(int i = 0; i < nCandleCnt; i++)
	{
		stCandleIn.tCandleData[i].iClose -= nMin;
		stCandleIn.tCandleData[i].iLow -= nMin;
		stCandleIn.tCandleData[i].iHigh -= nMin;
		stCandleIn.tCandleData[i].iOpen -= nMin;
		
		stCandleIn.tCandleData[i].iClose /= dUnit;
		stCandleIn.tCandleData[i].iLow /= dUnit;
		stCandleIn.tCandleData[i].iHigh /= dUnit;
		stCandleIn.tCandleData[i].iOpen /= dUnit;
	}
	//////////////////////////////////////////////////////////////////////////

	//# 유사도
	stCandleIn.iSimilar = 0; // not used yet

    stCandleIn.iPreGap = GetBongGap();		// 기준 봉( X봉전 Data 조회)
    stCandleIn.iPreTrand = m_nDefult_PreTrend;	// 이전 추세 구분 

    stCandleIn.iIgnoreColor = GetIgnoreColor();	// 양음봉 구분 무시
	stCandleIn.iIgnoreTail = GetIgnoreTail();	// 꼬리 무시 : 0-꼬리적용, 1-꼬리길이무시, 2-꼬리무시

	stCandleIn.iApplyBodyRate = 0;	// 몸통길이 적용안함
	for(int k = 0; k < MAX_CANDLE; k++)
		stCandleIn.tCandleData[k].iBodyRate = 0;

	//# 필드설정
	stCandleIn.FieldEditerCnt = m_listField.GetCount();

	HeadInfoType		stHeadInfoType;
	for (nIndex = 0; nIndex < m_listField.GetCount(); nIndex++)
	{
		pos = m_listField.FindIndex(nIndex);
		stHeadInfoType = m_listField.GetAt(pos);

		stCandleIn.FieldList[nIndex] = stHeadInfoType.lHeadID;
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

BOOL CMainFrmWnd::SetMakePatternPacket(DRAW_IN	&stDrawIn, CList<CString, CString&>* pListChartData)
{
	int nTot = pListChartData->GetCount();
	int nS_Time=0;
	BOOL bResult = FALSE;

// 	nS_Time = atoi(m_strChartTerm) - 2 ;
// 	if(nS_Time == -1)
// 		nS_Time = m_comboTime.GetCurSel() + 3; 

	stDrawIn.nS_Time  = GetSearchTimeTerm();     // 0:일간, 1:주간, 2:월간
	stDrawIn.lTarget  = 4;           // 대상데이터 => 4 고정
	stDrawIn.lMoveAvg = 1;           // 이평기간 => 1고정
    stDrawIn.lS_Pos   = 0;           // 검색 시작위치

	if (m_btnBongChk.GetCheck() == TRUE)
		stDrawIn.lS_Range = LIST_BONGTERM[m_comboBongTerm.GetCurSel()];        // 검색 범위
	else
		stDrawIn.lS_Range = nTot;


    stDrawIn.lSimilar = LIST_SIMILAR[m_comboSimilar.GetCurSel()];         // 유사도
    stDrawIn.cBoxFlag = '0';					// 횡보 체크 여부

	//# 필드설정
    stDrawIn.nFieldEditerCnt = m_listField.GetCount();

	memset(&stDrawIn.sTargetBit, '1', sizeof(stDrawIn.sTargetBit));

	POSITION pos;
	int nIndex;
	HeadInfoType		stHeadInfoType;
	for (nIndex = 0; nIndex < m_listField.GetCount(); nIndex++)
	{
		pos = m_listField.FindIndex(nIndex);
		stHeadInfoType = m_listField.GetAt(pos);
		stDrawIn.nFieldList[nIndex] = stHeadInfoType.lHeadID;
	}

	stDrawIn.stDrawData.lAccuVal = 2;   // 최근 가중치 적용여부
                                     //  : 1~ 10, 미적용시 0으로.. (0.005)
    stDrawIn.stDrawData.lDraw_Cnt = nTot;// m_SliderCtrl.GetPos();    // 그리기 기간범위(20~120)  // ->차트 데이터 건수
    stDrawIn.stDrawData.lErrRate = 10;				// 오차율
    stDrawIn.stDrawData.lGapRate = 40;        // 필터 범위
	
	long *pDrawData;
	pDrawData = new long[120];	

	int lTest;
	POSITION posChart = pListChartData->GetHeadPosition();
	for(nIndex = 0; nIndex < nTot; nIndex++)
	{
		pDrawData[nIndex] = atol((LPSTR)(LPCTSTR)pListChartData->GetNext(posChart));
		lTest = pDrawData[nIndex];
	}

	stDrawIn.stDrawData.lDraw_Cnt = GetData_DrawPacket(nTot, pDrawData);//m_pctrlDrawPatten->GetData_DrawPacket(pDrawData);  // ->차트 데이터 건수
	
// 	if (stDrawIn.stDrawData.lDraw_Cnt == 0)
// 	{
// // 		m_ctrlState.SetWindowText("[검색할 패턴이 존재하지 않습니다.]");
// // 		g_iMetaTable.ShowMessageBox(m_hWnd, "검색할 패턴이 존재하지 않습니다.");
// 
// 		return FALSE;
// 	}
	
	for (nIndex = 0; nIndex < nTot; nIndex++) //120
		stDrawIn.stDrawData.lDraw_Value[nTot-1-nIndex] = pDrawData[nIndex];

	for (nIndex = stDrawIn.stDrawData.lDraw_Cnt; nIndex < (int)CNT_DRAW_VAL; nIndex++)
		stDrawIn.stDrawData.lDraw_Value[nIndex] = -1;

	delete [] pDrawData;

	int nTargetBaseType_0st, nTargetBaseType_1st;
	GetTargetBaseType(nTargetBaseType_0st, nTargetBaseType_1st);

	CString strData = "";
	int nTargetBase_2nd = m_comboTarget_2nd.GetCurSel();
	if(nTargetBase_2nd > 0)
		m_comboTarget_2nd.GetLBText(nTargetBase_2nd, strData);
		/*strData = (LPCTSTR)(DWORD)m_comboTarget_2nd.GetItemData(nTargetBase_2nd);*/

	BOOL bReverse = FALSE;
	if (m_btnReverseChk.IsWindowEnabled() == TRUE)
		bReverse = m_btnReverseChk.GetCheck();

	CString strCode, strCodeCompare;
	if (nTargetBaseType_0st == 0) // 전체
	{
		memset(&stDrawIn.sTargetBit, 0x31, sizeof(stDrawIn.sTargetBit));
	}
	else if (nTargetBaseType_0st == 1 || nTargetBaseType_0st == 2) // 코스피, 코스닥
	{
		if (bReverse == TRUE)
			memset(&stDrawIn.sTargetBit, 0x31, sizeof(stDrawIn.sTargetBit));
		else
			memset(&stDrawIn.sTargetBit, 0x30, sizeof(stDrawIn.sTargetBit));

		for (int nIndex_Code = 0; nIndex_Code < m_saCodeGroup.GetSize(); nIndex_Code++)
		{
			strCodeCompare = m_saCodeGroup.GetAt(nIndex_Code);
			for (nIndex = 0; nIndex < m_saCodeMST.GetSize(); nIndex++)
			{
				strCode = m_saCodeMST.GetAt(nIndex);
				strCode = strCode.Mid(1, 6);
				if (strCodeCompare == strCode)		//# Kospi
				{
					stDrawIn.sTargetBit[nIndex + (int)RESULT_MAX_OFFSET] = (m_btnReverseChk.GetCheck() == TRUE) ? '0':'1';
					break;
				}
			}
		}
	}
	else if (nTargetBaseType_0st == 3 || nTargetBaseType_0st == 4)// 테마, 관심
	{
		if (bReverse == TRUE)
			memset(&stDrawIn.sTargetBit, 0x31, sizeof(stDrawIn.sTargetBit));
		else
			memset(&stDrawIn.sTargetBit, 0x30, sizeof(stDrawIn.sTargetBit));

		for (int nIndex_Code = 0; nIndex_Code < m_saCodeGroup.GetSize(); nIndex_Code++)
		{
			strCodeCompare = m_saCodeGroup.GetAt(nIndex_Code);
			for (nIndex = 0; nIndex < m_saCodeMST.GetSize(); nIndex++)
			{
				strCode = m_saCodeMST.GetAt(nIndex);
				if (strCodeCompare == strCode.Mid(1, 6))
				{
					stDrawIn.sTargetBit[nIndex + (int)RESULT_MAX_OFFSET] = (m_btnReverseChk.GetCheck() == TRUE) ? '0':'1';
					break;
				}
			}
		}
	}

	return TRUE;
}

long CMainFrmWnd::GetData_DrawPacket(int nTot, long* pDrawData)
{
//	dData = (dData - dData_YMin) * 100 / (dData_YMax - dData_YMin);
	long dData_YMax = pDrawData[0];
	long dData_YMin = pDrawData[0];

	int index = 0;
	for(index=0; index < nTot; index++)
	{
		if(dData_YMin > pDrawData[index])
			dData_YMin = pDrawData[index];

		if(dData_YMax < pDrawData[index])
			dData_YMax = pDrawData[index];
	}

	if( !dData_YMax || !dData_YMin || dData_YMin>=dData_YMax )
		return FALSE;

	for(index=0; index < nTot ; index++)
	{
		pDrawData[index] = (pDrawData[index] - dData_YMin) * 100 / (dData_YMax - dData_YMin);
	}
	return nTot;
}

// 챠트의 Addin 에서 직접 호출하는 함수임
// CMainFrmWnd 클래스가 CMChartInterface를 상속, 
// 챠트의 SetOutsideCommonInterface 함수로 자신을 Addin에 등록한다.
void CMainFrmWnd::SetDataWindowData(CList<CString, CString&>* pListData)
{
	//차트 종가 데이터 Receive 
	if(pListData)
	{
		m_SectionDataList.RemoveAll();

		CString szTemp;
		for(POSITION posChart = pListData->GetHeadPosition(); posChart!=NULL;)
		{
			szTemp = (LPSTR)(LPCTSTR)pListData->GetNext(posChart);
			m_SectionDataList.AddTail(szTemp);
		}
		
        
		if(m_nTabIndex == TAB_PATTERN)
			SendTr_PatternData(pListData); 		
		else
			SendTr_CandleData(pListData);

		SetEnable(FALSE);
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
	strFileRoot = m_strAppPath + "\\" + USRDIR + "\\" + Variant(nameCC) + FOLDER_FINDDATA + "\\" + FIXFIELD_USER_CHART_SECTION_FILE;
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
		m_nFixCol = 0;
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
			
			if(m_nTabIndex == TAB_CANDLE && stHeadFixInfo.stHeadType.strHeadName == "유사도")
				continue;

			if (stHeadFixInfo.bView == TRUE)
				m_listField.AddTail(stHeadFixInfo.stHeadType);
		}
	}
}

BOOL CMainFrmWnd::GetLoadSet_Def()
{
	CFileFind cFind;
	BOOL bRead = TRUE;
	
	m_nDefult_BongTerm = 2;
	m_nDefult_BongTerm_User = 0;
	m_nDefult_Similar = 2;
	m_nDefult_ChartTerm = 2;
	m_nDefult_ChartTermMin = 2;
	m_nDefult_Tail = 0;
	m_nDefult_AllColorChk = 0;
	m_nDefult_PreTrend = 0;
	m_nDefult_ExceptPreferred = 1;

	CString strFileRoot = m_strAppPath + FOLDER_DATA + "\\" + PATTERNINFO_INIFILE;
	bRead = cFind.FindFile(strFileRoot);
	if (bRead)
	{
		CString strKey;
		strKey = "TargetBaseType";
		m_nDefult_TargetBaseType = GetPrivateProfileInt("DefaultSet", strKey, 0, strFileRoot);

		strKey = "ExceptPreferred";
		m_nDefult_ExceptPreferred = GetPrivateProfileInt("DefaultSet", strKey, 1, strFileRoot);
		
		strKey = "SS_ChartTerm";//차트의 대상시간(분:1, 일:2, 주:3, 월:4)
		m_nDefult_ChartTerm = GetPrivateProfileInt("DefaultSet", strKey, 2, strFileRoot);

		strKey = "SS_ChartTermMin"; //분선택(1분:0, 3분:1, 5분:2, 10분:3, 15분:4, 30분:5, 60분:6)
		m_nDefult_ChartTermMin = GetPrivateProfileInt("DefaultSet", strKey, 2, strFileRoot);
		
		strKey = "SS_BongTerm";//분석 범위(100, 120, 150, 170, 200, 250, 300)의 Index
		m_nDefult_BongTerm = GetPrivateProfileInt("DefaultSet", strKey, 2, strFileRoot);
		
		strKey = "SS_BongTerm_User";//분석 범위 적용 유무
		m_nDefult_BongTerm_User = GetPrivateProfileInt("DefaultSet", strKey, 0, strFileRoot);
		
		strKey = "SS_Similar";
		m_nDefult_Similar = GetPrivateProfileInt("DefaultSet", strKey, 2, strFileRoot);

		if(m_nDefult_TargetBaseType < 0 || m_nDefult_TargetBaseType > 4) m_nDefult_TargetBaseType = 0;
		if(m_nDefult_ExceptPreferred < 0 || m_nDefult_ExceptPreferred > 1) m_nDefult_ExceptPreferred = 0;
		if(m_nDefult_ChartTerm < 1 || m_nDefult_ChartTerm > 4) m_nDefult_ChartTerm = 2;
		if(m_nDefult_BongTerm < 0 || m_nDefult_BongTerm > 6) m_nDefult_BongTerm = 2;
		if(m_nDefult_BongTerm_User < 0 || m_nDefult_BongTerm_User > 1) m_nDefult_BongTerm_User = 0;
		if(m_nDefult_Similar < 0 || m_nDefult_Similar > 3) m_nDefult_Similar = 2;
	}

	strFileRoot = m_strAppPath + FOLDER_DATA + "\\" + CANDLEINFO_INIFILE;
	bRead = cFind.FindFile(strFileRoot);
	if (bRead)
	{
		CString strKey;

		strKey = "BongPassCnt";
		m_nDefult_BongPass = GetPrivateProfileInt("DefaultSet", strKey, 1, strFileRoot);
		
		strKey = "SS_IgnoreColor";
		m_nDefult_AllColorChk = GetPrivateProfileInt("DefaultSet", strKey, 0, strFileRoot);

		strKey = "SS_IgnoreTail";
		m_nDefult_Tail = GetPrivateProfileInt("DefaultSet", strKey, 1, strFileRoot);

		strKey = "SS_PreTrend";
		m_nDefult_PreTrend = GetPrivateProfileInt("DefaultSet", strKey, 0, strFileRoot);

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

	CString strFileRoot = m_strAppPath + "\\" + USRDIR + "\\"+ Variant(nameCC) + FOLDER_FINDDATA + "\\" + FIXFIELD_USER_CHART_SECTION_FILE;

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

	int nSysCnt = plistFixSys->GetCount();		//보존 필드 갯수
	int nUserCnt = plistFixUser->GetCount();	//사용자 지정 필드 갯수
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

void CMainFrmWnd::SetEnable(BOOL bEnable)
{
	m_RadioCandle.EnableWindow(bEnable);
	m_RadioPattern.EnableWindow(bEnable);
	
	m_comboTarget_1st.EnableWindow(bEnable);

	m_btnReverseChk.EnableWindow(bEnable);
	if (bEnable == TRUE && m_comboTarget_1st.GetCurSel() == 0)
	{
		m_comboTarget_2nd.EnableWindow(FALSE);
		m_btnReverseChk.EnableWindow(FALSE);
	}
	m_btnExceptionChk.EnableWindow(bEnable);

	m_btnIgnoreColorChk.EnableWindow(bEnable);
	m_editBongCnt.EnableWindow(bEnable);

	m_btnBongChk.EnableWindow(bEnable);
	m_comboBongTerm.EnableWindow(m_btnBongChk.GetCheck()? bEnable:FALSE);
	m_comboSimilar.EnableWindow(bEnable);
	m_comboTail.EnableWindow(bEnable);
	
	m_btnDay.EnableWindow(bEnable);
	m_btnWeek.EnableWindow(bEnable);
	m_btnMonth.EnableWindow(bEnable);
	m_btnMin.EnableWindow(bEnable);

	m_comboTime.EnableWindow(bEnable);
 	if (bEnable == TRUE)
 		m_comboTime.EnableWindow(m_btnMin.GetCheck());

	m_edtZoomValue.EnableWindow(bEnable);
	m_editCntOfDatAll.EnableWindow(bEnable);

	m_pDlgChart->EnableWindow(bEnable);
	m_pDlgChartSecond->EnableWindow(bEnable);
	
	m_gridResult.EnableWindow(bEnable);
//	m_editSimilarity.EnableWindow(bEnable);
}


LRESULT CMainFrmWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
// 	if (message == RMSG_EVENT_CODEINPUT)
// 	{
// 		CString strEvent = (char*)lParam;
// 
// 		if(strEvent == "LengthFull")
// 		{
// 			CString strCode = (LPCTSTR)wParam;
// 	
// 			if(!m_bModifyFlag && FALSE)
// 				m_SectionDataList.RemoveAll();
// 
// 			m_nCntInView = m_nAllCntInView = (m_nTabIndex == TAB_PATTERN)?120:60;
// 			CString strCnt;
// 			strCnt.Format("%d", m_nAllCntInView);
// 			m_editCntOfDatAll.SetWindowText(strCnt);
// 			m_edtZoomValue.SetWindowText(strCnt);
// 			
// 			SetChartActivation(CENTER_CHART); //Center Chart 활성화
// 			SendTr_ChartData(strCode);
// 
// 			CString strName;
// 			strName = GetCodeToName(strCode);
// 			strName.Replace("&", "&&");
// 			
// 			if (strName != "ⓚ")
// 			{
// 				m_ctrlCodeName.SetWindowText(strName);				
// 			}			
// 		}		
// 
// 	}

	return CFrameWnd::WindowProc(message, wParam, lParam);
}

BOOL CMainFrmWnd::PreTranslateMessage(MSG* pMsg)
{
	if(m_WaitCursor.WaitCursorProc(pMsg)) return TRUE;

	if(pMsg->message == WM_KEYDOWN)  
	{		
		UINT nChar = (UINT)pMsg->wParam;
		switch(nChar)
		{
		case VK_RETURN :
			{
				if (m_edtZoomValue.GetSafeHwnd() == pMsg->hwnd)
				{
					MsgFunc_KeyDownZoomValue();
					return TRUE;
				}
				else if (m_editCntOfDatAll.GetSafeHwnd() == pMsg->hwnd)
				{
					MsgFunc_KeyDownCntOfDateAll();
					return TRUE;
				}
				else if (m_editBongCnt.GetSafeHwnd() == pMsg->hwnd)
				{
					OnEdKillfocusBongCnt();
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
	}

	if (pMsg->wParam == VK_HOME)
		return FALSE;

	return CFrameWnd::PreTranslateMessage(pMsg);
}

BOOL CMainFrmWnd::MsgFunc_KeyDownZoomValue()
{
	CString strZoomValue;
	m_edtZoomValue.GetWindowText(strZoomValue);

	int nCnt = atoi(strZoomValue);
	if(nCnt > m_nAllCntInView)
	{
		strZoomValue.Format("%d", m_nAllCntInView);
		m_edtZoomValue.SetWindowText(strZoomValue);
		nCnt = m_nAllCntInView;
	}

	if (m_nCntInView <= 0)
		return FALSE;


	m_nCntInView = nCnt;

	m_pDlgChart->m_ChartItem.SetOnePageDataCount(m_nCntInView);
	return TRUE;
}

BOOL CMainFrmWnd::MsgFunc_KeyDownCntOfDateAll()
{
	CString strCntOfDataAll;
	m_editCntOfDatAll.GetWindowText(strCntOfDataAll);

	int nAllCnt = atol(strCntOfDataAll);
	int nMaxCnt;
	if(m_nTabIndex == TAB_PATTERN)
		nMaxCnt = 120;
	else
		nMaxCnt = 60;

	if(nAllCnt < nMaxCnt)
	{
		strCntOfDataAll.Format("%d", m_nAllCntInView);
		m_editCntOfDatAll.SetWindowText(strCntOfDataAll);
		return FALSE;
	}

	if(m_nRestrictedMaxCnt!=0 && nAllCnt > m_nRestrictedMaxCnt)
	{
		strCntOfDataAll.Format("%d", m_nRestrictedMaxCnt);
		m_editCntOfDatAll.SetWindowText(strCntOfDataAll);
		nAllCnt = m_nAllCntInView = m_nRestrictedMaxCnt;
	}

	m_nAllCntInView = nAllCnt;

	CString strCode = m_pCodeInput->GetCode();
	if(strCode != "")
	{
		SetChartActivation(CENTER_CHART);
		SendTr_ChartData(strCode);
	}

	return TRUE;
}

void CMainFrmWnd::OnDeltaposSpinzoomvalue(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	*pResult = 0;

	CString strZoomValue;
	m_edtZoomValue.GetWindowText(strZoomValue);

	if (!strZoomValue.IsEmpty())
	{
		int nCnt = atoi(strZoomValue);
		//if(nCnt < 60 && pNMUpDown->iDelta > 0) return;

		if (pNMUpDown->iDelta > 0)
		{
			if (nCnt <= 5) return;

			nCnt--;
		}
		else
			nCnt++;

		if(nCnt > m_nAllCntInView) return;
		m_nCntInView = nCnt;
		strZoomValue.Format("%d",m_nCntInView);
		m_edtZoomValue.SetWindowText(strZoomValue);

		if(m_pDlgChart->GetSafeHwnd())
		{
			m_pDlgChart->m_ChartItem.SetOnePageDataCount(m_nCntInView);
		}
	}

}

void CMainFrmWnd::OnBtnDataMore() 
{
	CString strCntOfDatAll;
	m_editCntOfDatAll.GetWindowText(strCntOfDatAll);
	m_nAllCntInView = atoi(strCntOfDatAll);

	if (m_nAllCntInView > 0)
	{
// 		if (m_DlgChart.GetSafeHwnd())
// 		{
// 			m_SectionDataList.RemoveAll();
// 			//요청할 갯수를 Reset한다.
// 			m_nAllCntInView += 100;
// 			strCntOfDatAll.Format("%d", m_nAllCntInView);
// 			m_editCntOfDatAll.SetWindowText(strCntOfDatAll);
// 
// 			CString strCode = m_pCodeInput->GetCode();
// 			if(strCode != "")
// 			{
// 				SetChartActivation(CENTER_CHART);
// 				SendTr_ChartData(strCode);
// 			}
// 		}
	}
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

	return 0;
}

void CMainFrmWnd::GetData_ThemaToCode(CString strThemeCode)
{
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
// 	m_pTRComm->SetTR("HFIBOA076210");
// 
// 	m_WaitCursor.BWaitCursorIntelli();
// 
// 	//화면번호를 세팅한다.
//  	m_pTRComm->SetBaseData("SCREENCODE", m_strScreenNo);
// 	
// 	if(!m_pDataUtilMng)
// 	{
// 		m_pDataUtilMng = (IDataUtilManager2*)AfxGetPctrInterface(UUID_IDataUtilManager2, 1);
// 	}
// 	
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

void CMainFrmWnd::OnRadioPattern()
{
	CheckRadioButton(IDC_RADIO_PATTERN, IDC_RADIO_CANDLE, IDC_RADIO_PATTERN);

//	if(m_nTabIndex != 0)
	{
		m_nTabIndex = TAB_PATTERN;

		m_btnBongChk.ShowWindow(SW_SHOW);
		m_comboBongTerm.ShowWindow(SW_SHOW);
		m_comboSimilar.ShowWindow(SW_SHOW);
		m_ctrlSimilarity.ShowWindow(SW_SHOW);

		m_ctrlBongCntText1.ShowWindow(SW_HIDE);
		m_editBongCnt.ShowWindow(SW_HIDE);
		m_ctrlBongCntText2.ShowWindow(SW_HIDE);
		m_btnIgnoreColorChk.ShowWindow(SW_HIDE);
		m_comboTail.ShowWindow(SW_HIDE);

		m_SectionDataList.RemoveAll();

		m_nCntInView = m_nAllCntInView = (m_nTabIndex == TAB_PATTERN)?120:60;
		CString strCnt;
		strCnt.Format("%d", m_nAllCntInView);
		m_editCntOfDatAll.SetWindowText(strCnt);
		m_edtZoomValue.SetWindowText(strCnt);
	
		m_gridResult.m_nPatternType = 0;
		SetGrid_Head();
		SetGrid_Head(TRUE);

		CString strCode = m_pCodeInput->GetCode();
		
		SetChartActivation(CENTER_CHART); //Center Chart 활성화

 		m_pDlgChart->m_ChartItem.LoadAddIn("ChartPeriodSummaryAddIn.dll:DRAG_ZOOM_ANAL");//DRAG_ZOOM_ANAL
 		m_pDlgChartSecond->m_ChartItem.LoadAddIn("ChartPeriodSummaryAddIn.dll:DRAG_ZOOM_ANAL");

		SendTr_ChartData(strCode);
	}
}

void CMainFrmWnd::OnRadioCandle()
{
	CheckRadioButton(IDC_RADIO_PATTERN, IDC_RADIO_CANDLE, IDC_RADIO_CANDLE);

//	if(m_nTabIndex != 1)
	{
		m_nTabIndex = TAB_CANDLE;

		m_btnBongChk.ShowWindow(SW_HIDE);
		m_comboBongTerm.ShowWindow(SW_HIDE);
		m_comboSimilar.ShowWindow(SW_HIDE);
		m_ctrlSimilarity.ShowWindow(SW_HIDE);

		m_ctrlBongCntText1.ShowWindow(SW_SHOW);
		m_editBongCnt.ShowWindow(SW_SHOW);
		m_ctrlBongCntText2.ShowWindow(SW_SHOW);
		m_btnIgnoreColorChk.ShowWindow(SW_SHOW);
		m_comboTail.ShowWindow(SW_SHOW);

		m_SectionDataList.RemoveAll();

		m_nCntInView = m_nAllCntInView = (m_nTabIndex == TAB_PATTERN)?120:60;
		CString strCnt;
		strCnt.Format("%d", m_nAllCntInView);
		m_editCntOfDatAll.SetWindowText(strCnt);
		m_edtZoomValue.SetWindowText(strCnt);
		
		m_gridResult.m_nPatternType = 1;
		SetGrid_Head();
		SetGrid_Head(TRUE);

		CString strCode = m_pCodeInput->GetCode();
		
		SetChartActivation(CENTER_CHART); //Center Chart 활성화

		m_pDlgChart->m_ChartItem.LoadAddIn("ChartPeriodSummaryAddIn.dll:DRAG_CANDLE_ANAL");//DRAG_ZOOM_ANAL
 		m_pDlgChartSecond->m_ChartItem.LoadAddIn("ChartPeriodSummaryAddIn.dll:DRAG_CANDLE_ANAL");
		
		SendTr_ChartData(strCode);
	}
}


void CMainFrmWnd::OnBtnPrint()
{
	SetResultPrint();
}
void CMainFrmWnd::SetResultPrint()
{
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

	if (m_bExpand)
		SetBtnImg(&m_btnExpand, "LEFT_EX");
	else
		SetBtnImg(&m_btnExpand, "RIGHT_EX");

	OnSize();

	
// 	int nX;
// 	int nY;
// 	CRect rt;
// 	
// 	m_btnExpand.GetWindowRect(rt);
// 	nX = rt.left + rt.Width()/2;
// 	nY = rt.top + rt.Height()/2;
// 	
// 	SetCursorPos(nX, nY);
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

// 팝업 메뉴 command 처리
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
// 	else if(nIndex == (int)SCREEN_INDEX_RUSULTPRINT)
// 	{
// 		SetResultPrint();
// 	}
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
				//m_gridResult.GetRowCode(nRow, strCode, nMarkType);
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

void CMainFrmWnd::OnMenuConditionSave(UINT nIndex)
{
	if(ID_MENU_CONDITIONSAVE_START == nIndex)
		SaveUserCondition();
	else
	{
		int i = nIndex - ID_MENU_CONDITIONSAVE_START - 1;

		if(i < 0 || i > m_UserData.GetSize() - 1)
			return;
		
		CHARTSECTIONINFO* pInfo = m_UserData.GetAt(i);
		SearchUserItem(pInfo);
	}
}

#define SCREEN_NO_ALLBUY				"NOT USER"
#define SCREEN_NO_ROLL					"NOT USER"
void CMainFrmWnd::SetSharedDataToSend(ShardData *pstShardData)
{
	CString strScreenNo(pstShardData->strScreenNo);

	if(pstShardData->saCode.GetSize() > 0)
	{
		m_pScreenWnd->OpenScreen(strScreenNo, &pstShardData->saCode, &pstShardData->saName);
	}
	else
	{
		m_pScreenWnd->OpenScreen(strScreenNo);
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
				nReturn = SetMessageBox(strMsg, MB_YESNO | MB_ICONQUESTION);
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
		SetMessageBox(strMsg, MB_OK);
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


int CMainFrmWnd::SetMessageBox(LPCTSTR lpszText, UINT nType, LPCTSTR lpszCaption)
{
	return ::MessageBox(m_hWnd, lpszText, lpszCaption, nType);
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

void CMainFrmWnd::OnEdKillfocusBongCnt()
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

BOOL CMainFrmWnd::SetConvert_SendPatternPacket(DRAW_IN &stDrawIn, p0624InBlock* pst0624)
{
	memset(pst0624, 0x20, sizeof(p0624InBlock));

	char tmpVal[2600]={0,};
	CString szVal;

	CString strBuff;
	LONG2STR_BUF(tmpVal, 4, stDrawIn.nTrIndex, strBuff);
	memcpy(pst0624->nTrIndex, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stDrawIn.nS_Time, strBuff);
	memcpy(pst0624->iS_Time, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stDrawIn.lTarget, strBuff);
	memcpy(pst0624->iTarget, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stDrawIn.lMoveAvg, strBuff);
	memcpy(pst0624->iMoveAvg, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stDrawIn.lS_Pos, strBuff);
	memcpy(pst0624->iS_Pos, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stDrawIn.lS_Range, strBuff);
	memcpy(pst0624->iS_Range, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stDrawIn.lSimilar, strBuff);
	memcpy(pst0624->iSimilar, tmpVal, 4);

	memcpy(pst0624->sBoxFlag, &stDrawIn.cBoxFlag, 1);

	LONG2STR_BUF(tmpVal, 4, stDrawIn.nFieldEditerCnt, strBuff);
	memcpy(pst0624->FieldEditerCnt, tmpVal, 4);


	int fieldIndex=0;
	int nIndex = 0;
	memset(tmpVal, 0x20, RESULT_FIELD_MAX_EUGENE);
	for( nIndex = 0; fieldIndex < stDrawIn.nFieldEditerCnt; nIndex+=4)
	{
		strBuff.Format("%ld", stDrawIn.nFieldList[fieldIndex++]);
		memcpy(&tmpVal[nIndex], strBuff, strBuff.GetLength());
	}
	memcpy(pst0624->FieldList, tmpVal, RESULT_FIELD_MAX_EUGENE);	
	//	LONG2STR_BUF(tmpVal, RESULT_FIELD_MAX, stDrawIn.nFieldList, strBuff);
	//	memcpy(pst0624->FieldList, tmpVal, RESULT_FIELD_MAX);


	//	INT2STR_BUF(tmpVal, RESULT_MAX, stDrawIn.sTargetBit);
	//memcpy(pst0624->sTargetBit, stDrawIn.sTargetBit, RESULT_MAX);
	Safe_CheckCopy2(pst0624->sTargetBit, stDrawIn.sTargetBit, RESULT_MAX, szVal);

	LONG2STR_BUF(tmpVal, 4, stDrawIn.stDrawData.lAccuVal, strBuff);
	memcpy(pst0624->iAccuVal, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stDrawIn.stDrawData.lDraw_Cnt, strBuff);
	memcpy(pst0624->iDraw_Cnt, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stDrawIn.stDrawData.lErrRate, strBuff);
	memcpy(pst0624->iErrRate, tmpVal, 4);

	LONG2STR_BUF(tmpVal, 4, stDrawIn.stDrawData.lGapRate, strBuff);
	memcpy(pst0624->iGapRate, tmpVal, 4);

	fieldIndex = 0;
	for(nIndex = 0; nIndex < CNT_DRAW_VAL_EUGENE; nIndex += 4)
	{
		strBuff.Format("%04ld", stDrawIn.stDrawData.lDraw_Value[fieldIndex++]);
		memcpy(&tmpVal[nIndex], strBuff, strBuff.GetLength());
	}
	memcpy(pst0624->iDraw_Value, tmpVal, CNT_DRAW_VAL_EUGENE);	
	//	LONG2STR_BUF(tmpVal, CNT_DRAW_VAL, stDrawIn.stDrawData.lDraw_Value, strBuff);
	//	memcpy(pst0624->iDraw_Value, tmpVal, CNT_DRAW_VAL);	

	return TRUE;
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

	LONG2STR_BUF(tmpVal, 4, stCandle_In.iApplyBodyRate, strBuff);
	memcpy(&pstHKST03660000_IN->iApplyBodyRate, tmpVal, 1);

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

LRESULT CMainFrmWnd::OnReceiveTranData(WPARAM wParam, LPARAM lParam)
{
	CString strData = (LPCSTR)wParam;
// 	int nBuffSize = m_pPartnerMng->GetRecvLen(strData);
// 
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

BOOL CMainFrmWnd::OnPF_Contorl_Move()
{
// 	CWnd* pWnd = NULL;
// 	if(!(pWnd = GetParent()->GetParent()))
// 	{
// 		return FALSE;
// 	}
// 	
// 	ST_MOVEITEM stItem;	
// 	
// 	SET_ST_MOVEITEM(stItem, "ItemCode1", m_pJCodeWnd);
// 	stItem.nType = PF_ITEM_CODEEDIT;
// 	pWnd->SendMessage(RMSG_MOVECTRL, PF_CONTROL_SHOW, (LPARAM)&stItem);

// 	stItem.nType = PF_ITEM_BTN;
// 	SET_ST_MOVEITEM(stItem, "btnSearch", &m_btnSearch);
// 	pWnd->SendMessage(RMSG_MOVECTRL, PF_CONTROL_SHOW, (LPARAM)&stItem);
// 
// 	SET_ST_MOVEITEM(stItem, "btnPrint", &m_btnPrint);
// 	pWnd->SendMessage(RMSG_MOVECTRL, PF_CONTROL_SHOW, (LPARAM)&stItem);
// 
// 	SET_ST_MOVEITEM(stItem, "btnFieldEdit", &m_btnFieldEdit);
// 	pWnd->SendMessage(RMSG_MOVECTRL, PF_CONTROL_SHOW, (LPARAM)&stItem);
// 
// 	SET_ST_MOVEITEM(stItem, "btnTargetSend", &m_btnResultCtrl);
// 	pWnd->SendMessage(RMSG_MOVECTRL, PF_CONTROL_SHOW, (LPARAM)&stItem);
	
	return TRUE;
}

void CMainFrmWnd::OnBtnClickSearch()
{
	OnBtnSearch();
}

void CMainFrmWnd::OnBtnClickPrint()
{
	OnBtnPrint();
}

void CMainFrmWnd::OnBtnClickFieldEdit()
{
	OnBtnFieldEdit();
}

void CMainFrmWnd::OnBtnClickTargetSend()
{
	OnBtnResultCtrl();
}

void CMainFrmWnd::OnBtnClickSave()
{
	CMenu menu;
	menu.CreatePopupMenu();
	
	CString strScreenInfo;
	int nMenuID = ID_MENU_CONDITIONSAVE_START;

	menu.AppendMenu(MF_STRING, nMenuID, _T("조건저장"));
	menu.AppendMenu(MF_SEPARATOR); 

	int nIndex;
	for (nIndex = 0; nIndex < m_UserData.GetSize(); nIndex++)
	{
		++nMenuID;
		CHARTSECTIONINFO* pInfo = m_UserData.GetAt(nIndex);
		menu.AppendMenu(MF_STRING, nMenuID, pInfo->strName); 
	}

	CPoint ptMenu;
	GetCursorPos(&ptMenu);
	
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptMenu.x, ptMenu.y, this);
	menu.DestroyMenu();
}

void CMainFrmWnd::SaveUserCondition()
{
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	CSaveDlg  dlgSave(this);

	if(dlgSave.DoModal() == IDOK)
	{
		CHARTSECTIONINFO* pSet = NULL;
		CString strCandleName = dlgSave.m_strLoadedFileName;

		BOOL bSameNameFind = FALSE;
		int nImageIndex = -1;

		// 같은 이름 검색
		for(int i = 0; i < m_UserData.GetSize(); i++)
		{
			if(strCandleName == m_UserData.GetAt(i)->strName)
			{
				bSameNameFind = TRUE;
				pSet = m_UserData.GetAt(i);
				nImageIndex = i;
				break;
			}
		}

		int nRes = IDYES;
		if(bSameNameFind)
		{
			nRes = SetMessageBox(_T("같은 구간검색조건 이름이 존재합니다. \n\n계속 진행 하시겠습니까?"), MB_YESNO);
		}
		else
		{
			pSet = new CHARTSECTIONINFO;
			pSet->strName = strCandleName;
			m_UserData.Add(pSet);
		}

		if(nRes == IDYES)
		{
			pSet->bType = m_nTabIndex;
			pSet->strCode = m_pCodeInput->GetCode();
			pSet->strChartDate = m_strSectionDate.Mid(15, 8);
			pSet->strStartDate = m_strSectionStartDate;
			pSet->strEndDate = m_strSectionEndDate;
			pSet->arrDrawData.RemoveAll();

			POSITION posChart = m_SectionDataList.GetHeadPosition();
			for(int i = 0; i < m_SectionDataList.GetCount(); i++)
				pSet->arrDrawData.Add(m_SectionDataList.GetNext(posChart));

			SaveUserData(pSet);
		}
	}

	AfxSetResourceHandle(hInstSave);
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
	CString strFilePath = "";
	CString strAppPath;

	CString strUserPath;
	strUserPath.Format("%s%s%s", Variant(homeCC), USRDIR, Variant(nameCC));

	if ("CHART_SECTION" == strLoadType)
		strFilePath = (CString)strUserPath + "\\UserChartSection";
	else if(strLoadType =="SEARCHCENTER")
		strFilePath = strUserPath;
	else
	{
		GetDir_DrfnRoot(strAppPath);
		strFilePath = strAppPath + (CString)FOLDER_FIND + "\\" + (CString)FINDERINFO_INIFILE;
	}

	return strFilePath;
}

void CMainFrmWnd::LoadUserData()
{
	ClearUserData();
	CString strPath = GetFilePath("CHART_SECTION");

	CFileFind cFind;
	CString strUserFile;
	BOOL bRead = TRUE;
	BOOL bFind = FALSE;

	bRead = cFind.FindFile(strPath);
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
		::CreateDirectory(strPath, NULL);
	}

	// find user pattern files
	CString strFileFind = strPath + "\\*.xml";
	bRead = cFind.FindFile(strFileFind);
	if (!bRead)
	{
		CString strMsg = _T("사용자 구간검색 파일을 찾지 못했습니다.");
		//		SetStateTitleBar(strMsg);
	}
	else
	{
		while(bRead)
		{
			bRead = cFind.FindNextFile();

			if(cFind.IsDots())  continue;
			if (!cFind.IsDirectory())
			{
				strUserFile = cFind.GetFileName();
				if(strUserFile.Right(4) != ".xml")
					continue;

				strUserFile = strPath + "\\" + strUserFile;
				CHARTSECTIONINFO* pUserInfo = new CHARTSECTIONINFO;
				if(ReadXML(strUserFile, pUserInfo))
				{
					m_UserData.Add(pUserInfo);
				}
				else
					delete pUserInfo;
			}
		}
	}
}

void CMainFrmWnd::SaveUserData(CHARTSECTIONINFO* pUserInfo)
{
	CString strFileName = GetFilePath("CHART_SECTION");
	strFileName += "\\" + pUserInfo->strName + ".xml";

	CFile cf;
	if (cf.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) == FALSE)
		return;
	
	CXMLWriter XmlWriter;

	CString strHeader = XML_HEADING;
	XmlWriter.WriteString(cf, strHeader);

	CString strType = pUserInfo->bType ? "1" : "0";
	XmlWriter.WriteHeader(cf, 0, TRUE, "CHARTSECTION", "NAME", pUserInfo->strName, 
													   "TYPE", strType, 
													   "CODE", pUserInfo->strCode,
													   "CHARTDATE", m_strSectionDate,
													   "START_DATE", m_strSectionStartDate,
													   "END_DATE", m_strSectionEndDate, NULL);
	int i;
	for(i = 0; i < pUserInfo->arrDrawData.GetSize(); i++)
	{
		if(!pUserInfo->bType)
			XmlWriter.WriteNodeOnlyAttribute(cf, 1, "DRAWDATA", "VALUE", pUserInfo->arrDrawData.GetAt(i), NULL);
		else
		{
			if( i % 4 > 0)
				continue;

			XmlWriter.WriteNodeOnlyAttribute(cf, 1, "DRAWDATA", "OPEN", pUserInfo->arrDrawData.GetAt(i), 
																"HIGH",  pUserInfo->arrDrawData.GetAt(i+1), 
																"LOW",  pUserInfo->arrDrawData.GetAt(i+2), 
																"CLOSE",pUserInfo->arrDrawData.GetAt(i+3), NULL);
		}
	}

	XmlWriter.WriteNodeEnd(cf, 0, "CHARTSECTION");

	cf.Close();
}

BOOL CMainFrmWnd::ReadXML(CString& strUserFile, CHARTSECTIONINFO* pUserInfo)
{
	BOOL bResult = TRUE;
	VARIANT_BOOL bLoadedXMLFile;

	DOCUMENT pDOMConditionIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	pDOMConditionIndex->async = false;
	bLoadedXMLFile = pDOMConditionIndex->load(_bstr_t(strUserFile));

	// XML 파일을 로드하는데 실패하였다면
	if(bLoadedXMLFile == VARIANT_FALSE) 
	{
		bResult = FALSE;
		return bResult;
	}
	try
	{
		XMLELEMENT pParent = pDOMConditionIndex->documentElement;

		NODE pDrawItem = NULL;
		ATTRIBUTE pAttr = NULL;
		CString strDrawItem;

		int i = 0;
		pAttr = pParent->attributes;
		for(i; i < pAttr->length; ++i)
		{
			if(_stricmp(pAttr->item[i]->nodeName, "NAME") == 0)
				pUserInfo->strName = (char*)pAttr->item[i]->text;
			else if(_stricmp(pAttr->item[i]->nodeName, "TYPE") == 0)
				pUserInfo->bType = ((char*)pAttr->item[i]->text == "1") ? 1:0;
			else if(_stricmp(pAttr->item[i]->nodeName, "CODE") == 0)
				pUserInfo->strCode = (char*)pAttr->item[i]->text;
			else if(_stricmp(pAttr->item[i]->nodeName, "CHARTDATE") == 0)
				m_strSectionDate = (char*)pAttr->item[i]->text;
			else if(_stricmp(pAttr->item[i]->nodeName, "START_DATE") == 0)
				pUserInfo->strStartDate = (char*)pAttr->item[i]->text;
			else if(_stricmp(pAttr->item[i]->nodeName, "END_DATE") == 0)
				pUserInfo->strEndDate = (char*)pAttr->item[i]->text;
		}

		for(pDrawItem = pParent->firstChild; pDrawItem != NULL; pDrawItem = pDrawItem->nextSibling)
		{
			pAttr = pDrawItem->attributes;
			for(i = 0; i < pAttr->length; ++i)
			{
				if(_stricmp(pAttr->item[i]->nodeName, "VALUE") == 0 ||
				   _stricmp(pAttr->item[i]->nodeName, "CLOSE") == 0 ||	
				   _stricmp(pAttr->item[i]->nodeName, "LOW")   == 0	||
				   _stricmp(pAttr->item[i]->nodeName, "HIGH")  == 0	||
				   _stricmp(pAttr->item[i]->nodeName, "OPEN")  == 0)
				{
					strDrawItem = (char*)pAttr->item[i]->text;
					pUserInfo->arrDrawData.Add(strDrawItem);
				}
			}
		}
	}
	catch(_com_error e)
	{
		bResult = FALSE;
	}

	return bResult;
}

void CMainFrmWnd::ClearUserData()
{
	int i = 0;
	for(i; i < m_UserData.GetSize(); ++i)
		delete m_UserData.GetAt(i);
	m_UserData.RemoveAll();
}

void CMainFrmWnd::DeleteUserData(CString& strItemName)
{
	CString strPath = GetFilePath("CHART_SECTION");
	int i;
	for(i = 0; i < m_UserData.GetSize(); ++i)
	{
		CHARTSECTIONINFO* pInfo = m_UserData.GetAt(i);
		if(pInfo->strName + ".xml" == strItemName)
		{
			CFile::Remove(strPath + "\\" + pInfo->strName + ".xml");
			delete pInfo;
			m_UserData.RemoveAt(i);
			break;
		}
	}
}

void CMainFrmWnd::DeleteAllUserData()
{
	CString strPath = GetFilePath("CHART_SECTION");

	int i;
	for(i = 0; i < m_UserData.GetSize(); ++i)
	{
		CHARTSECTIONINFO* pInfo = m_UserData.GetAt(i);
		CFile::Remove(strPath + "\\" + pInfo->strName + ".xml");
	}

	ClearUserData();
}

BOOL CMainFrmWnd::GetFindFile(CString& strFileName)
{
	BOOL bReturn = FALSE;

	int i;
	for(i = 0; i < m_UserData.GetSize(); ++i)
	{
		CHARTSECTIONINFO* pInfo = m_UserData.GetAt(i);
		if(pInfo->strName == strFileName)
		{
			bReturn = TRUE;
			break;
		}
	}	

	return bReturn;
}

BOOL CMainFrmWnd::SetTempGroupReg(CStringArray *psaCode, CUIntArray *iaMarkType)
{
	if (psaCode->GetSize() <= 0)
		return FALSE;

	//  >>
// 	CString strCode;
// 	pGwanMng->RegiestToImsiGroup(*psaCode, NULL);
	//  <<

	return TRUE;
}

BOOL CMainFrmWnd::SetPortfolioReg(CString strOpenDlg, CStringArray *psaCode)
{
	//  >>
// 	CSectionSearchView* pView = (CSectionSearchView*)GetParent();
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
// 		//20100202 SJ_KIM
// 		pGwanMng->ShowGwansimDlgWithList(this, arrCode);	
// 	}
	//  <<

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

void CMainFrmWnd::LoadModifyFile(CString& strCode)
{
	CString strType, strUserFileName;
	if(LookUpSearchCenterModify(strType, strUserFileName))
	{
		if(strType == "4")
			SearchUserItem(strCode, strUserFileName);
	}
}

BOOL CMainFrmWnd::LookUpSearchCenterModify(CString& strType, CString& strUserFileName)
{
	CString strFileName = GetFilePath("SEARCHCENTER") + "\\SearchCenterModify.xml";

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

void CMainFrmWnd::SearchUserItem(CString& strCode, CString& strFullName)
{
	CString strPath = GetFilePath("CHART_SECTION") + "\\";
	strPath += strFullName;

	CHARTSECTIONINFO UserInfo;
	if(!ReadXML(strPath, &UserInfo))
		return;

	m_SectionDataList.RemoveAll();
	
	CString strValue;
	int i;
	for(i = 0; i < UserInfo.arrDrawData.GetSize(); ++i)
	{
		strValue = UserInfo.arrDrawData.GetAt(i);
		m_SectionDataList.AddTail(strValue);
	}

	m_nTabIndex = UserInfo.bType;
	m_strSectionStartDate = UserInfo.strStartDate;
	m_strSectionEndDate = 	UserInfo.strEndDate;

	m_bModifyFlag = TRUE;

	strCode = UserInfo.strCode;
}

void CMainFrmWnd::SearchUserItem(CHARTSECTIONINFO* pInfo)
{
	m_SectionDataList.RemoveAll();
	
	CString strValue;
	int i;
	for(i = 0; i < pInfo->arrDrawData.GetSize(); ++i)
	{
		strValue = pInfo->arrDrawData.GetAt(i);
		m_SectionDataList.AddTail(strValue);
	}

	m_nTabIndex = pInfo->bType;
	m_strSectionStartDate = pInfo->strStartDate;
	m_strSectionEndDate = 	pInfo->strEndDate;

	m_bModifyFlag = TRUE;

	//  >>
// 	CStdDialog  *pdlgStd = (CStdDialog*)GetParent();
// 	CSectionSearchView* pView = (CSectionSearchView*)pdlgStd->GetParent();
// 
// 	CWrpItemCode* pItemCode = pView->GetCodeItemPtr();
// 	if(pItemCode->GetCaption() == pInfo->strCode)
// 		ResendTR();
// 	else
// 		pItemCode->EditFullCaption(pInfo->strCode);
	//  <<
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
	CFont* pFont;
	
	if(!bBold)
		pFont = m_pScreenWnd->GetDefFont();
	else
		pFont = m_pScreenWnd->GetBoldFont();
	
	return pFont;
}

COLORREF CMainFrmWnd::GetColor(int nColor)
{
	return m_pScreenWnd->GetAxColor(nColor);
}

CString CMainFrmWnd::Variant(int nComm, CString strData)
{
	return m_pScreenWnd->Variant(nComm, strData);
}

void CMainFrmWnd::SetBtnImg(CfxImgButton *pBtn, CString strFileName)
{
	m_pScreenWnd->SetBtnImg(pBtn, strFileName);
}

void CMainFrmWnd::SetBtnImg(CfxImgButton *pBtn, int nTextLen, BOOL bBold)
{
	m_pScreenWnd->SetBtnImg(pBtn, nTextLen, bBold);
}

COLORREF CMainFrmWnd::GetAxColor(int nColor)
{
	return m_pScreenWnd->GetAxColor(nColor);
}

HBRUSH CMainFrmWnd::GetAxBrush(COLORREF clr)
{
	return (HBRUSH)(*m_pScreenWnd->GetAxBrush(clr));
}

BOOL CMainFrmWnd::GetStockMasterData(CStringArray &arrCode, CStringArray &arrName, UINT Jgub, UINT Upgub)
{
	return m_pScreenWnd->GetStockMasterData(arrCode, arrName, Jgub, Upgub);
}

BOOL CMainFrmWnd::GetUpMasterData(CStringArray *&psaCode, CStringArray *&psaName, int jgub)
{
	return m_pScreenWnd->GetUpMasterData(psaCode, psaName, jgub);
}

CString CMainFrmWnd::GetMarketNameToCode(CString strMaketCode)
{
	return m_pScreenWnd->GetMarketNameToCode(strMaketCode);
}

CPen* CMainFrmWnd::GetAxPen(COLORREF clr, int nWidth, int nStyle)
{
	return m_pScreenWnd->GetAxPen(clr, nWidth, nStyle);
}

CWnd* CMainFrmWnd::GetModuleMainWnd()
{
	return m_pScreenWnd->GetModuleMainWnd();
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
		
		m_pScreenWnd->SetUserMaster(usrMasterCondition, strArrayCode.GetSize(), &strArrayCode, &strArrayCodeName);
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
#if (0)//tour2k
	m_pScreenWnd->GetData_PortGroup(saKey, saGroupName);
#else
	saKey.Copy(m_GwanSimGroup.strKey);
	saGroupName.Copy(m_GwanSimGroup.strGroupName);
#endif
}

BOOL CMainFrmWnd::GetData_PortItem(CStringArray& saCode, CStringArray& saName, int nKey)
{
	return m_pScreenWnd->GetData_PortItem(saCode, saName, nKey);
}

void CMainFrmWnd::SetChangeCode(CString strCode)
{
	if(!m_pCodeInput)
		return;

	SetSharedDataToCode(strCode);

	m_pCodeInput->SetCode(strCode);
	
	if(!m_bModifyFlag && FALSE)
		m_SectionDataList.RemoveAll();

	m_nCntInView = m_nAllCntInView = (m_nTabIndex == TAB_PATTERN)?120:60;
	CString strCnt;
	strCnt.Format("%d", m_nAllCntInView);
	m_editCntOfDatAll.SetWindowText(strCnt);
 	m_edtZoomValue.SetWindowText(strCnt);

	SetChartActivation(CENTER_CHART); //Center Chart 활성화
	SendTr_ChartData(strCode);
	
	CString strName;
	strName = GetCodeToName(strCode);
	strName.Replace("&", "&&");
	
	if (strName != "ⓚ")
		m_ctrlCodeName.SetWindowText(strName);			
}

//기존 데이터 clear
void CMainFrmWnd::_DoClearOldData()
{
	m_gridResult.ClearAll();
	SetGrid_Head(FALSE);
	SetGrid_Head(TRUE);
	
	m_ctrlCodeCnt.SetWindowText("0건");
	if(m_pDlgChart)
	{
		m_pDlgChartSecond->m_ChartItem.ClearChartData(0);
		m_pDlgChartSecond->m_ChartItem.InvokeAddInStr("ChartPeriodSummaryAddIn.dll:DRAG_ZOOM_ANAL", "0","");
	}
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

void CMainFrmWnd::SetInitSymbol()
{
	// 종목 symbol
	CString	strPath;
	CWnd*	(APIENTRY* axCreate)(CWnd*, void*);
	
	strPath.Format("%s\\dev\\cx_symbol.dll", Variant(homeCC));
	m_hiSymbol = AfxLoadLibrary(strPath);
	if (m_hiSymbol != NULL)
	{
		axCreate = (CWnd* (APIENTRY*)(CWnd*, void*))GetProcAddress(m_hiSymbol, _T("axCreate"));
		if (axCreate != NULL)
		{
			struct	_param	param;
			
			int nkey = TRKEY_SYMBOL;
			param.key   = nkey;
			param.name  = _T("17413");
			param.rect  = CRect(0, 0, 130, 20);
			param.fonts = "굴림체";
			param.point = 9;
			param.style = 1;
			param.tRGB  = 63;
			param.pRGB  = 181;
			param.options = _T("/a89/b91/c92/d69/i99");
			
			m_pwndSymbol = (*axCreate)(m_pScreenWnd->GetModuleMainWnd(), &param);
			m_pwndSymbol->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
			
			//내렸다 올라오면 심볼 사라지는 현상 해결
			m_pwndSymbol->SetParent(this);
			
			if (m_pwndSymbol == NULL)
			{
				AfxFreeLibrary(m_hiSymbol);
				m_hiSymbol = NULL;
			}
		}
	}
}

void CMainFrmWnd::ParsingSymbol(char* datb)
{
	int	value;
	CString	strCode;
	
	strCode  = CString(datb);
	value = strCode.Find('\t');
	if (value != -1)
	{
		WPARAM	wParam;
		CString tempCode;
		tempCode  = strCode.Left(value++);
		
		if( tempCode.GetLength() ==7 )
		{
			strCode   = strCode.Mid(value);
			wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, 20), (strCode.GetLength()));
			m_pwndSymbol->SendMessage(WM_USER, wParam, (LPARAM)strCode.operator LPCTSTR());
		}
		else
		{
			wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, 20), strCode.GetLength());
			m_pwndSymbol->SendMessage(WM_USER, wParam, (LPARAM)strCode.operator LPCTSTR());
		}
	}
}

void CMainFrmWnd::SendSymbol(CString strCode)
{
	CString strData;
	strData.Format("1301%c%s\t1021\t17413\t", 0x7f, strCode);
	
	char*	wb;
	struct	_userTH* userth;
	
	wb = new char[L_userTH + strData.GetLength()];
	FillMemory(wb, L_userTH + strData.GetLength(), ' ');
	userth = (struct _userTH *)wb;
	CopyMemory(userth->trc, "POOPPOOP", sizeof(userth->trc));
	userth->key  = TRKEY_SYMBOL;
	userth->stat = US_OOP ;
	CopyMemory(&wb[L_userTH], (char *)strData.operator LPCTSTR(), strData.GetLength());
	
	m_pScreenWnd->GetModuleMainWnd()->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strData.GetLength()), (LPARAM)wb);
	
	delete[] wb;
}