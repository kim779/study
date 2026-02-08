// TrMainView.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrmWnd.h"
#include "ScreenWnd.h"
#include "../../../SrcSite/include/axis_msg.h"

#include "./SignalChart/DlgChart_Stock.h"
#include "SignalSearch.h"

#include "../../Chart/Include_Chart/Dll_Load/IMetaTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE g_hInstance;

/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd

#define ID_FORM			999

#define TYPE_SEARCH_ALL     0
#define TYPE_SEARCH_NEWS    1
#define TYPE_SEARCH_SYS     2

BOOL g_bSameChk = TRUE; //테스트용 임시변수.

enum
{
	ID_btnSignalSet = IDC_BTN_SIGNALSET,
	ID_btnSearch = IDC_BTN_SEARCH
};

int nCtrlID[] = 
{
	ID_btnSignalSet,
	ID_btnSearch
};

CString sCtrlName[] =
{
	"btnSignalSet",
	"btnSearch"
};

IMPLEMENT_DYNCREATE(CMainFrmWnd, CFrameWnd)

CMainFrmWnd::CMainFrmWnd()
{
	m_nInitFlag     = 0;
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);
	
	m_brushStatic.CreateSolidBrush(COLOR_MAIN_CANVAS);
	
	m_bExp_Left = TRUE;
	
	m_strCode = "";
	m_bIsFnUser = FALSE;

	m_dwAdviseKey = NULL;
	m_nScreenType = 0;
	m_nGridRow = 2;
	
	m_hKSJongName = NULL;
	m_nState = 0; // Default=전체

	m_hSignalMng = NULL;
	m_pModuleMainWnd = NULL;

	m_pWndDummy = NULL;

	m_pDlgChart = NULL;
	m_pCodeInput = NULL;

	m_nPeriodCnt = 5;

	m_hiSymbol	= NULL;
	m_pwndSymbol = NULL;

	m_strChartTerm = "1";
}

CMainFrmWnd::~CMainFrmWnd()
{
	SetDelAll_SignalData();
	
	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();
	
	m_brushStatic.DeleteObject();
}

BEGIN_MESSAGE_MAP(CMainFrmWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrmWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_INITCODE, OnBtnInit_Code)
	ON_BN_CLICKED(IDC_BTN_EXP_LEFT, OnBtnExp_Left)
	ON_CBN_SELCHANGE(IDC_COMBO_SIGNALTYPE, OnChangeSignalType)

	ON_BN_CLICKED(IDC_BTN_SIGNALSET, OnBtnSignalSet)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	ON_BN_CLICKED(IDC_BTN_ITEMEDIT, OnBtnFieldEdit)
	ON_COMMAND_RANGE(IDC_RADIO_TOTAL, IDC_RADIO_CODE, OnRadioBtnClk)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHK_MARKETCONDITION, IDC_CHK_SYS, OnChkBtnClk)
	ON_MESSAGE(WM_GET_BROD, OnGetBrod)

	ON_BN_CLICKED(IDC_BTN_DAY, OnBtnDay)
	ON_BN_CLICKED(IDC_BTN_WEEK, OnBtnWeek)
	ON_BN_CLICKED(IDC_BTN_MONTH, OnBtnMonth)
	ON_BN_CLICKED(IDC_BTN_MIN, OnBtnMinute)
	ON_BN_CLICKED(IDC_BTN_TICK, OnBtnTick)
	ON_CBN_SELCHANGE(IDC_COMBO_SEL_PERIOD, OnChangeInterval)

	ON_REGISTERED_MESSAGE(RMSG_USERINFO, OnUserInfo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd diagnostics

HRESULT CMainFrmWnd::ReceiveData(LPCSTR szTR, LPVOID aTRData, long dwTRDateLen)
{
	HRESULT dwResult = 0;
	
	CString strData;
	if (dwTRDateLen == 0)		//# 0 : err
	{
		return S_OK;
	}
	else if(!strcmp(szTR, TRNO_SET_REAL))
	{
		/*SetAdvData_Alert(TRUE);*/
	}
	else if(strcmp(szTR, QUERY_STRJONGMOK) == 0)//차트 TR
	{
		m_pDlgChart->m_ChartItem.ClearChartData(0);

		char *pszBuff=NULL;
		pszBuff = new char[dwTRDateLen+1];
		memset(pszBuff, 0x00, sizeof(pszBuff));
		memcpy(pszBuff, aTRData, dwTRDateLen);
		
		Receive_ChartData(pszBuff, dwTRDateLen);
		
		delete []pszBuff;
		pszBuff = NULL;
	}
		return S_OK;
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

void CMainFrmWnd::OnCodeEnter()
{
	CString strCode = m_pCodeInput->GetCode();
	strCode.TrimLeft();		strCode.TrimRight();

	if (strCode.GetLength())
	{
 		//m_ctrlCodeName.SetWindowText(strData);
		
		SendTr_ChartData(strCode, "");
		//SetSearchData_Chart(strCode);
		
		if(m_nState==1)		SetSearch();
	}
// 	else
// 		m_ctrlCodeName.SetWindowText("");

}

void CMainFrmWnd::OnCodeEnter_Chart()
{

}


void CMainFrmWnd::OnReceiveDataAfter(LPCTSTR strTrCode, VARIANT FAR* strVarOutInfo, VARIANT FAR* nVarOutInfo, VARIANT FAR* nVarRetValue, VARIANT FAR* nVarRetOption, long nDataCount)
{
	SetSearchData_TimeSet();
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd message handlers
int CMainFrmWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	GetDir_DrfnRoot(m_strAppPath);

	g_iMetaTable.m_strDataFolderPath.Format(_T("%s\\icss\\"), m_strAppPath);
	g_iMetaTable.LoadMetaTable();

	m_crBackColor = COLOR_MAIN_CANVAS;
	
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	m_comboSignalType.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(0, 0, 0, 0), this, IDC_COMBO_SIGNALTYPE);
	m_comboSignalType.SetFont(&m_fontDefault);
	
	m_btnExp_Left.Create(_T(""), CRect(0, 0, 0, 0), this, IDC_BTN_EXP_LEFT);
	SetBtnImg(&m_btnExp_Left, "RIGHT_EX");
	m_btnExp_Left.ShowWindow(SW_SHOW);

	m_btnSignalSet.Create(_T("신호설정"), CRect(0, 0, 0, 0), this, IDC_BTN_SIGNALSET);
	SetBtnImg(&m_btnSignalSet, 4);

	m_gridResult.CreateGrid(WS_CHILD | WS_VISIBLE | WS_BORDER,
		CRect(0, 0, 0, 0), this, IDC_GRID_RESULT);

//유진 UI변경
	m_radioTotal.Create(_T("전체"), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON |WS_GROUP , 
		CRect(0, 0, 0, 0), this, IDC_RADIO_TOTAL);
	m_radioTotal.SetFont(&m_fontDefault);
	m_radioTotal.SetCheck(TRUE);

	m_radioCode.Create(_T(""), WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON, 
		CRect(0, 0, 0, 0), this, IDC_RADIO_CODE);
	m_radioCode.SetFont(&m_fontDefault);

	m_chkMarketCondition.Create(_T("시세"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX , 
		CRect(0, 0, 0, 0), this, IDC_CHK_MARKETCONDITION);
	m_chkMarketCondition.SetFont(&m_fontDefault);

	m_chkTechIndex.Create(_T("기술적 지표"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_CHK_TECHINDEX);
	m_chkTechIndex.SetFont(&m_fontDefault);

	m_chkDealAgency.Create(_T("거래원"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_CHK_DEALAGENCY);
	m_chkDealAgency.SetFont(&m_fontDefault);

	m_chkNews.Create(_T("뉴스"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_CHK_NEWS);	
	m_chkNews.SetFont(&m_fontDefault);
	m_chkNews.SetCheck(FALSE);//20100506 김한두 >>
	m_chkNews.ShowWindow(SW_HIDE);//20100506 김한두 >>

	m_chkSys.Create(_T("시스템"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 
		CRect(0, 0, 0, 0), this, IDC_CHK_SYS);	
	m_chkSys.SetFont(&m_fontDefault);
	m_chkSys.SetCheck(TRUE);
	m_chkSys.ShowWindow(SW_HIDE);//20100506 김한두 >>

	m_btnItemEdit.Create(_T("항목편집"), CRect(0, 0, 0, 0), this, IDC_BTN_ITEMEDIT);
	SetBtnImg(&m_btnItemEdit, 4);

	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	m_pModuleMainWnd = pScreenWnd->GetModuleMainWnd();

	if (m_pCodeInput == NULL)
	{
		CRect rtNULL(0, 0, 0, 0);

		m_pCodeInput = new CCodeCtrl(pScreenWnd->GetModuleMainWnd());
		m_pCodeInput->SetRootPath(Variant(homeCC));
		m_pCodeInput->UseCodeDlg(TRUE);
		m_pCodeInput->CreateCodeCtrl(this, pScreenWnd->GetModuleMainWnd(), pScreenWnd, rtNULL, IDC_CODEINPUT);
		m_pCodeInput->SetColor(RGB(0,0,0), RGB(255,255,255));
		m_pCodeInput->SetUnit("주식");
	}

	SetInitSymbol();

	// 차트 DLG 생성
	m_pDlgChart		  =	new CDlgChart_Stock;
	m_pDlgChart->Create(IDD_DLG_CHART, this );
	m_pDlgChart->ModifyStyle(0, WS_CLIPCHILDREN);
	m_pDlgChart->m_ChartItem.SetMultiItemMode(TRUE);
	m_pDlgChart->ShowWindow(SW_SHOW);
	
	m_ctrlCodeName_Chart.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(0, 0, 0, 0), this);
	m_ctrlCodeName_Chart.SetFont(&m_fontBold);
	
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
	
	m_btnTick.Create(_T("틱"), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		CRect(0, 0, 0, 0), this, IDC_BTN_TICK);
	m_btnTick.SetFont(&m_fontDefault);

	m_comboSelInterval.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(0, 0, 0, 0), this, IDC_COMBO_SEL_PERIOD);
	m_comboSelInterval.SetFont(&m_fontDefault);
	
	for(int i = 0; i < _countof(SEL_PERIOD); i++)
		m_comboSelInterval.AddString(SEL_PERIOD[i]);
	m_comboSelInterval.SetCurSel(0);

	SetInitControl();

	CreateImageList();
	m_btnDay.FP_vSetImage(&m_ImageListBtn, /*m_crBackColor,*/ 81, 82, TRUE, "일", TRUE);
	m_btnWeek.FP_vSetImage(&m_ImageListBtn, /*m_crBackColor,*/ 81, 82, TRUE, "주", TRUE);
	m_btnMonth.FP_vSetImage(&m_ImageListBtn, /*m_crBackColor,*/ 81, 82, TRUE, "월", TRUE);
	m_btnMin.FP_vSetImage(&m_ImageListBtn, /*m_crBackColor,*/ 81, 82, TRUE, "분", TRUE);
	m_btnTick.FP_vSetImage(&m_ImageListBtn, /*m_crBackColor,*/ 81, 82, TRUE, "틱", TRUE);

	{
		CString szIni;
		szIni.Format("%s\\drdata\\test.ini", m_strAppPath);
		g_bSameChk = ::GetPrivateProfileInt("test", "same", 1, szIni);
	}

	AfxSetResourceHandle(hInstSave);

	m_pITrCommManager = (IAUTrCommManager*)AfxGetPctrInterface(UUID_IAUTrCommManager);
	if(m_pITrCommManager)
		m_pITrComm = (ITrComm2*)m_pITrCommManager->AdviseTrComm();

	return 0;
}

void CMainFrmWnd::CreateImageList()
{
	CString strImgPath;
	
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
}

const		LPCTSTR		LIST_TIME[] = { " 1", " 3", " 5", "10", "15", "30", "60" };
void CMainFrmWnd::SendTr_ChartData(CString strCode, CString strDate)
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
	strDate_Start.Format("%04d%02d%02d",timer.GetYear(), timer.GetMonth(), timer.GetDay());
	memcpy(stOutput.date, strDate_Start, strDate_Start.GetLength());						// 검색일
	
	memcpy(stOutput.num,		    "0050",				 4);		// 검색할 데이터 갯수s
	
	CString strTerm;
	if (atoi(m_strChartTerm) == 1 || atoi(m_strChartTerm) == 0)
		strTerm.Format("%03d", atoi(SEL_PERIOD[m_comboSelInterval.GetCurSel()]));

	if(m_strChartTerm == "1" || m_strChartTerm == "0")
		memcpy(stOutput.unit,	    strTerm,			 min(strTerm.GetLength(), 3));		// 틱/분/일 조회간격단위
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

	if (m_pwndSymbol)
		SendSymbol(strCode);
}
void CMainFrmWnd::OnDestroy() 
{
	if(m_hSignalMng)
	{
		Lib_Close(this);
		FreeLibrary(m_hSignalMng);
		m_hSignalMng = NULL;
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

	m_ImageListBtn.DeleteImageList();

	CFrameWnd::OnDestroy();
}

void CMainFrmWnd::OnSize(UINT nType, int cx, int cy) 
{	

	CFrameWnd::OnSize(nType, cx, cy);	

	if (m_nInitFlag > 0)		OnSize();
}

void CMainFrmWnd::OnSize()
{	
	CRect reClient, reTemp;
	GetClientRect(reClient);
	
	reClient.top += 3;
	reClient.bottom -= 3;
	reClient.left += 3;
	reClient.right -= 3;
	
	int nWidth;
	nWidth = (reClient.right - reClient.left) / 2;
	reTemp = reClient;
	if (m_bExp_Left == TRUE)
	{
		reTemp.right = reClient.right;
		reTemp.left = reTemp.right - nWidth;
		OnSize_Right(reTemp);
		
		reTemp.right = reTemp.left - 3;
		reTemp.left = reClient.left;
	}

	OnSize_Left(reTemp);
}

void CMainFrmWnd::OnSize_Left(CRect reClient)
{
	CRect reTop, reTemp, reBottom;
	
	reTop = reClient;
	reTop.bottom = reTop.top + 20;
	
	reTemp = reTop;
	//reTemp.right -= 2;
	
 	reTemp.left = reTemp.right - 20;
 	m_btnExp_Left.MoveWindow(&reTemp);
	
	// 20111103 박성경 >>
	reBottom = reTemp;

	// 항목편집 상단에 컨트롤 있을때
// 	reTemp.right = reTemp.left - 1;
// 	reTemp.left = reTemp.right - 60;

	// 항목편집 하단에 컨트롤 있을때
	reBottom.top = reBottom.bottom + 3;
	reBottom.bottom = reBottom.top + 20;
	
	reBottom.left = reTemp.right - 60;
	m_btnItemEdit.MoveWindow(&reBottom);	// 항목편집
	// 20111103 박성경 <<

	// 신호설정 상단에 컨트롤 있을때
//	reTemp.right = reTemp.left - 2; 
// 	reTemp.left = reTemp.right - 60;
// 	reTemp.bottom = reTemp.top + 20;

	// 신호설정 하단에 컨트롤 있을때
	reBottom.right = reBottom.left - 2;
	reBottom.left = reBottom.right - 60;
	m_btnSignalSet.MoveWindow(&reBottom);	// 신호설정
	

	reTemp.left = reTemp.right + GetBtnWidth(4) + 10;
	reTemp.left = reTemp.right - 20;

//유진 추가< 
	reTemp = reTop;
	reTemp.left = reTemp.left;
	reTemp.right = reTemp.left + 50;
	m_radioTotal.MoveWindow(&reTemp);

	// 종목컨트롤버튼 상단 컨트롤 있을때
// 	reTemp.left = reTemp.right;
// 	reTemp.right = reTemp.left + 13;

	// 종목컨트롤버튼 하단 컨트롤 있을때
	reBottom = reTemp;
	reBottom.top = reBottom.bottom + 3;
	reBottom.bottom = reBottom.top + 20;

	reBottom.right = reBottom.left + 12;
	m_radioCode.MoveWindow(&reBottom);

//유진 추가>	
//	reTemp = reTop;

	// 종목컨트롤 상단에 컨트롤 있을때
// 	reTemp.left = reTemp.right + 5;
// 	reTemp.right = reTemp.left + 90;

	// 종목컨트롤 하단에 컨트롤 있을때
	reBottom.left = reBottom.right + 5;
	reBottom.right = reBottom.left + 84;
	m_pCodeInput->MoveWindow(reBottom);

	reBottom.left = reBottom.right + 2;
	reBottom.right = reBottom.left + 130;
	m_pwndSymbol->MoveWindow(reBottom);
	
	reTemp.left = reTemp.right + 1;
	//reTemp.right = reTemp.left + 100;
	reTemp.right = reTemp.left + 90;
	//reTemp.top += 4;
	//reTemp.bottom = reTemp.top + 14;
	reTemp.bottom = reTemp.top + 20;
	//m_ctrlCodeName.MoveWindow(&reTemp);

	if(m_hKSJongName && IsWindow(m_hKSJongName))
	{
		CWnd::FromHandle(m_hKSJongName)->MoveWindow(reTemp.left, reTemp.top, reTemp.Width(), reTemp.Height());
		m_rectJongName = reTemp;
	}
/*	
	reTemp = reTop;
	reTemp.top = reTemp.top + 22;
	reTemp.bottom = reTemp.top + 20;
	reTemp.right = reTemp.left + 50;
	m_chkMarketCondition.MoveWindow(&reTemp);

	reTemp.left = reTemp.right + 5;
	reTemp.right = reTemp.left + 90;
	m_chkTechIndex.MoveWindow(&reTemp);

	reTemp.left = reTemp.right + 5;
	reTemp.right = reTemp.left + 60;
	m_chkDealAgency.MoveWindow(&reTemp);

	reTemp.left = reTemp.right + 5;
	reTemp.right = reTemp.left + 50;
	m_chkNews.MoveWindow(&reTemp);
*/
	reTemp = reTop;
	reTemp.top = reTemp.top + 22;
	reTemp.bottom = reTemp.top + 20;
	reTemp.right = reTemp.left + 60;
	m_chkSys.MoveWindow(&reTemp);

	reTemp.left = reTemp.right + 5;
	reTemp.right = reTemp.left + 60;
	m_chkNews.MoveWindow(&reTemp);


	reTop.top = reTop.bottom + 2;
	reTop.bottom = reTop.top + 20;	
	reTemp = reTop;

	reTemp.right -= 4;
	reTemp.left = reTemp.right - GetBtnWidth(2);
//유진		m_btnSearch.MoveWindow(&reTemp);   //20100224 김한두 >>
	
	reTemp.right = reTemp.left - 4;
	reTemp.left = reTemp.right - GetBtnWidth(4);
//유진		m_btnSignalSet.MoveWindow(&reTemp);//20100224 김한두 >>
	
//@유진삭제			reTemp.right = reTemp.left - 4;
//@유진삭제			reTemp.left = reTemp.right - GetBtnWidth(2);
//@유진삭제			m_btnSell.MoveWindow(&reTemp);
	
//@유진삭제		reTemp.right = reTemp.left - 2;
//@유진삭제		reTemp.left = reTemp.right - GetBtnWidth(2);
//@유진삭제		m_btnBuy.MoveWindow(&reTemp);	

	// 20111107 박성경 >>
	// 항목편집 상단에 있을때
// 	reTop.top = reTop.bottom - 13;
// 	reTop.bottom = reClient.bottom;

	// 항목편집 하단에 있을때
	reTop.top = reBottom.bottom + 3;
	reTop.bottom = reClient.bottom;
	// 20111107 박성경 <<
	if(m_gridResult.GetSafeHwnd())
		m_gridResult.MoveWindow(&reTop);

//20100224 김한두 >>
//	DoKSControlMove(sCtrlName[0], &m_btnSignalSet );
//	DoKSControlMove(sCtrlName[1], &m_btnSearch );
//20100224 김한두 <<
}

void CMainFrmWnd::OnSize_Right(CRect reClient)
{
// 	if (m_Chart)
// 	{
// 		CRect reTop;
// 		reTop = reClient;
// 		
// 		reTop.top += 4;
// 		reTop.bottom = reTop.top + 14;
// 	//	m_ctrlCodeName_Chart.MoveWindow(&reTop);
// 		
// 		reTop.top = reTop.bottom + 4;
// 		reTop.bottom = reClient.bottom;
// 		m_Chart.MoveWindow(&reTop);
// 
// 		int nChartNameLeft = reTop.left;
// 
// 		reTop.top -= 15;
// 		reTop.bottom = reTop.top + 15;
// 		reTop.left = reTop.right - 45;//20100514 김한두 >>
// 		m_ctrlUnitText.MoveWindow(&reTop);
// 
// 		reTop.left = nChartNameLeft;
// 		reTop.right = reTop.left + 200;
// 		m_chartCodeName.MoveWindow(&reTop);
// 	}

 	if (m_pDlgChart)
 	{
		CRect reTop;
		reTop = reClient;
		
		CRect reTemp = reTop;
 		reTemp.right = reTemp.left;//+ 100;
 		reTemp.bottom = reTemp.top + 20;
// 		m_ctrlCodeName_Chart.MoveWindow(&reTemp);
		
		reTemp.left = reTemp.right + 2;
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
		reTemp.right = reTemp.left + 20;
		m_btnTick.MoveWindow(&reTemp);
		
		reTemp.left = reTemp.right + 2;
		reTemp.right = reTemp.left + 45;
		reTemp.bottom += 160;
		m_comboSelInterval.MoveWindow(&reTemp);

		reTop.top += 5;
		reTop.bottom = reTop.top + 14;
		
		reTop.top = reTop.bottom + 4;
		reTop.bottom = reClient.bottom;
 		m_pDlgChart->MoveWindow(&reTop);
 
// 		int nChartNameLeft = reTop.left;
// 
// 		reTop.top -= 15;
// 		reTop.bottom = reTop.top + 15;
// 		reTop.left = reTop.right - 45;//20100514 김한두 >>
// 		m_ctrlUnitText.MoveWindow(&reTop);
// 
// 		reTop.left = nChartNameLeft;
// 		reTop.right = reTop.left + 200;
// 		m_chartCodeName.MoveWindow(&reTop);
	}
}

void CMainFrmWnd::SetInitControl() 
{	
//	CFormView::OnInitialUpdate();
	
	if (m_nInitFlag == 0)
	{
		SetInit_MainWnd();
		if(m_nScreenType==1)
		{
//@유진삭제	m_btnSell.ShowWindow(SW_HIDE);
			m_btnSignalSet.ShowWindow(SW_HIDE);
			m_gridResult.ShowWindow(SW_HIDE);
			m_comboSignalType.ShowWindow(SW_HIDE);
			m_btnExp_Left.ShowWindow(SW_HIDE);

			//m_ctrlCodeName.ShowWindow(SW_HIDE);
			if(m_hKSJongName) CWnd::FromHandle(m_hKSJongName)->ShowWindow(SW_SHOW);

			//m_btnInit_Code.ShowWindow(SW_HIDE);
			m_ctrlCodeName_Chart.ShowWindow(SW_HIDE);

			
			m_radioTotal.ShowWindow(SW_HIDE);
			m_radioCode.ShowWindow(SW_HIDE);
			m_chkMarketCondition.ShowWindow(SW_HIDE);
			m_chkTechIndex.ShowWindow(SW_HIDE);
			m_chkDealAgency.ShowWindow(SW_HIDE);
			m_chkNews.ShowWindow(SW_HIDE);
			m_chkSys.ShowWindow(SW_HIDE);
		}
		else
		{
			//if(m_hKSJongName) CWnd::FromHandle(m_hKSJongName)->ShowWindow(SW_HIDE);
			// 종목코드 도움말을 Disable되게 한다.
			
			//DoDllSizeEx();
		}
	//	m_pDllGate->SetControlDispatch();

		m_hChildFrame = GetFrameWnd(this)->GetSafeHwnd();
	}
	m_nInitFlag++;
}

void CMainFrmWnd::SetInit_MainWnd()
{
//	IDispatch* pDispatch = NULL;

	
// 	pDispatch = m_pDllGate->GetControlDispatch("cmbJCode");
// 	if(pDispatch)
// 	{
// 		m_pctrlCodeInput =  CWnd::FromHandle(m_pDllGate->GetControlHwnd("cmbJCode"));
// 		m_ctrlJMCombo.AttachDispatch(pDispatch, FALSE);
// 		m_ctrlJMCombo.SetEnabled(FALSE); // 신호조회화면 처음 뜰때 종목콤보 비활성화 - HONGKYU KIM
// 		
// 		m_pctrlCodeInput->SetParent(this);
// 	}
	
// 	{
// 		m_hKSJongName = m_pDllGate->GetControlHwnd("jmname");
// 
// 		if(m_hKSJongName)
// 		{
// 			CRect rect;
// 			m_ctrlCodeName.GetWindowRect(rect);
// 		//	m_ctrlCodeName.ShowWindow(SW_SHOW);
//  			ScreenToClient(rect);
//  			CWnd::FromHandle(m_hKSJongName)->SetParent(this);
//  			CWnd::FromHandle(m_hKSJongName)->MoveWindow(rect);
// 			CWnd::FromHandle(m_hKSJongName)->ShowWindow(SW_HIDE); // 신호조회화면 처음 뜰때 레이블 숨김 - HONGKYU KIM
// 			m_rectJongName = rect;	
// 		}
// 	}
	

//20100218 김한두 <<

	SetInit();
}

void CMainFrmWnd::OnFormInitAfter()
{
	
}

void CMainFrmWnd::SetInit()
{
	Load_LibSignalMng();
	
	CString strData;
	//# 1:HTS  0:FRONT(지점)
//--> HTS구분없앰. 사용하면 추후 다시
//	strData = KSLink_GetLoginInfo(ISHTS);
//	BOOL bIsHTS = FALSE;
//	if (strData.Left(1) == "1")
//		bIsHTS = TRUE;
	
	//strData = KSLink_GetLoginInfo(SUPER_USERLEVEL);
//@유진변경	strData = AfxGetApp()->GetProfileString("STBasic", "UserL",  "");
//@유진변경	if(strData.GetLength()==0)
//@유진변경		strData = KSLink_GetLoginInfo(SUPER_USERLEVEL);
//	strData = "S";

//	if (bIsHTS == TRUE)
//	{
//		//# "S", "A", "B", "C", "D"...
//		strData = strData.Left(1);
//		if ((strData == "S") || (strData == "A") || (strData == "B"))
			m_bIsFnUser = TRUE;
//	}
//	else if (bIsHTS == FALSE)
//	{
//		strData = strData.Left(3);
//		if ((atoi(strData) == 5) || 
//			(atoi(strData) == 6) ||
//			(atoi(strData) == 7) ||
//			(atoi(strData) == 9) ||
//			(atoi(strData) == 10) ||
//			(atoi(strData) == 11) ||
//			(atoi(strData) == 12) ||
//			(atoi(strData) == 13))
//			m_bIsFnUser = TRUE;
//	}
//<--
			
	if(m_gridResult.GetSafeHwnd())
		m_gridResult.SetInit("RESULT");
	
	m_comboSignalType.ResetContent();
	m_comboSignalType.AddString("전체");
	m_comboSignalType.AddString("뉴스");
	if (m_bIsFnUser == TRUE)
		m_comboSignalType.AddString("시스템");
	m_comboSignalType.SetCurSel(0);

	if(m_gridResult.GetSafeHwnd())
		m_gridResult.SetHideCol(1, TRUE);
	
	//OnCodeEnter();
	
	SetLoad_FieldInfo();
	SetGrid_Head(FALSE);
	SetGrid_Head(TRUE);

	//Module_SendTr(TRNO_SET_REAL);
	SetAdvData_Alert(TRUE);
	OnRMSGColorChange();

	//OnBtnMinute();
}



void CMainFrmWnd::SetAdvData_Alert(BOOL bReg)
{
	CString strRealType;
	if(m_hSignalMng && m_nScreenType == 0)
	{
		strRealType.Format("%d", (int)TYPE_SIGNAL_SYS);
		Lib_SetSignal(bReg, this, strRealType, "","*");//2022.07.26
	}
}
 
BOOL CMainFrmWnd::SetAdvData_Alert_CurrentPrice(BOOL bReg)
{
	BOOL bResult = TRUE;

	
// 	INewsSignalManager* pNSMng = (INewsSignalManager*)AfxGetPctrInterface(UUID_INewsSignalManager, 1);
// 
// 	if(pNSMng)
// 	{
// 		if(bReg && !m_dwAdviseKey)
// 		{
// 			m_dwAdviseKey = pNSMng->AdviseNewsSignal('A', (LONG)this);
// 		}
// 		if(!m_dwAdviseKey) return FALSE; //생성실패
// 	}
// 
// 	CString strData, strkey;
// 	for(int nIndex = 2; nIndex <= 66; nIndex++)
// 	{
// 		strData.Format("%.4d", nIndex);		
// 		strkey = strData;		
// 
// 		CString strRealType;					
// 		strRealType.Format("%d", (int)TYPE_SIGNAL_SYS);
// 		pNSMng->SetNewsSignal(bReg, this, strRealType, strkey);	
// 	}
	

	return bResult;
}

void CMainFrmWnd::OnBtnInit_Code()
{
	DoBtnInit_Code(FALSE);
}

void CMainFrmWnd::DoBtnInit_Code(BOOL bClear)
{
	CString strData = "";
	strData = m_pCodeInput->GetCode();
	if (strData.IsEmpty() == FALSE || m_nState == 0)
	{
		if(bClear)
		{
			strData = "";
			m_pCodeInput->SetCode(strData);
		}
		
		strData.Replace("&", "&&");
		//m_ctrlCodeName.SetWindowText(strData);
		
		//# All Search!
		SetSearch();
	}
// 	else
// 		m_ctrlCodeName.SetWindowText("");
}

void CMainFrmWnd::OnBtnExp_Left()
{
	m_bExp_Left = !m_bExp_Left;
	
	if (m_bExp_Left == TRUE)
		//m_btnExp_Left.SetLoadImage(IDB_EXP_RIGHT);
		SetBtnImg(&m_btnExp_Left, "RIGHT_EX");
	else
		//m_btnExp_Left.SetLoadImage(IDB_EXP_LEFT);
		SetBtnImg(&m_btnExp_Left, "LEFT_EX");
	
	if(m_pDlgChart)
	{
		m_ctrlCodeName_Chart.ShowWindow(m_bExp_Left);
		m_pDlgChart->ShowWindow(m_bExp_Left);

		m_btnDay.ShowWindow(m_bExp_Left);
		m_btnWeek.ShowWindow(m_bExp_Left);
		m_btnMonth.ShowWindow(m_bExp_Left);
		m_btnMin.ShowWindow(m_bExp_Left);
		m_btnTick.ShowWindow(m_bExp_Left);

		m_comboSelInterval.ShowWindow(m_bExp_Left);
// 		m_ctrlUnitText.ShowWindow(m_bExp_Left);		
// 		m_chartCodeName.ShowWindow(m_bExp_Left);
	}

	CRect rect;
	m_pDlgChart->GetWindowRect(rect);
 	ScreenToClient(rect);

	//20100506 김한두 >>
// 	VARIANT nWidth, nHeight;
// 	m_lRunForm.GetCurrentScreenSize(&nWidth, &nHeight);
// 
// 	int nOffset = ::GetSystemMetrics(SM_CXBORDER) + 3;
// 	if (m_bExp_Left == TRUE)
// 	{
// 		m_lRunForm.SetFormResize(rect.right+nOffset, nHeight.iVal);
// 	}
// 	else
// 	{
// 		m_lRunForm.SetFormResize(rect.left, nHeight.iVal);
// 	}
//20100506 김한두 <<
	OnSize();
	
	//m_ctrlCodeName.Invalidate();
	//m_btnInit_Code.Invalidate();
	
	m_comboSignalType.Invalidate();
	m_btnExp_Left.Invalidate();
	
//@유진삭제	m_btnSell.Invalidate();
//@유진삭제	m_btnBuy.Invalidate();
	
	m_btnSignalSet.Invalidate();
/*	m_btnSearch.Invalidate();*/
	
	if(m_gridResult.GetSafeHwnd())
		m_gridResult.Invalidate();
	
	m_ctrlCodeName_Chart.Invalidate();

// 	int nX;
// 	int nY;
// 	CRect rt;
// 	
// 	m_btnExp_Left.GetWindowRect(rt);
// 	nX = rt.left + rt.Width()/2;
// 	nY = rt.top + rt.Height()/2;
// 	
// 	SetCursorPos(nX, nY);
}

//@유진삭제	
//-->
//void CMainFrmWnd::OnBtnSell()
//{	
//	CString strCode;
//	strCode = m_gridResult.GetRowToCode(m_gridResult.m_nLastSelRow);
//	
//	CString strData;
//	//# 1:HTS  0:FRONT(지점)
//	strData = KSLink_GetLoginInfo(ISHTS);
//
//	if(strData[0]=='1')	
//		SetOpenScreen(0, "1003", strCode);	//# 주식주문화면(2101)
////	else	SetOpenScreen(0, "211600", strCode);	//# 주식 주문화면 14101)	
//}
//<--

void CMainFrmWnd::OnChangeSignalType()
{
	int nCurSel = m_comboSignalType.GetCurSel();
	
	SetSearch();
}

//@유진삭제
/*
void CMainFrmWnd::OnBtnSell()
{
	CString strCode;
	strCode = m_gridResult.GetRowToCode(m_gridResult.m_nLastSelRow);
	
	CString strData;
	//# 1:HTS  0:FRONT(지점)
	strData = KSLink_GetLoginInfo(ISHTS);

	if(MAX_VERSION==200703)
	{
		// 기존 루틴 현재 지워놓음.
	}
	else
	{
		if(strData[0]=='1')		SetOpenScreen("2101", strCode);	//# 주식주문화면(2101)
		else	SetOpenScreen("14101", strCode);	//# 주식 주문화면 14101)
	}
}
*/

/*
void CMainFrmWnd::OnBtnBuy()
{
CString strCode;
strCode = m_gridResult.GetRowToCode(m_gridResult.m_nLastSelRow);

  //# 주식 매수(2102)
  SetOpenScreen("2102", strCode);
  }
*/

void CMainFrmWnd::OnBtnSignalSet()
{
	
	//# 신호 설정(1508)
	//SetOpenScreen( 0 , "2116"); //다이얼로그 6자리 
	/*m_pDllGate->DoRegOpen(10001,"2187");*/
//	SetOpenScreen( 0 , "2034"); //화면번호 벼경 2116->2034
	

	SetOpenScreen(0, "7134", "");
}

void CMainFrmWnd::OnBtnSearch()
{
	
// 	CString strCode;
// 	strCode = m_ctrlJMCombo.GetCaption();
// 	if (strCode.IsEmpty() == TRUE)
// 		SetSearch();
// 	else
// 	{
// 		if (m_ctrlJMCombo.CheckCaption() == TRUE)
// 		{
// 			CString strData;
// 			strData = m_ctrlJMCombo.GetCurName();
// 			
// 			strData.Replace("&","&&");
// 			m_ctrlCodeName.SetWindowText(strData);
// 			
// 			SetSearch();
// 		}
// 		else
// 			SetSendMainMsg("정확하지 않은 종목코드입니다.");
// 	}
	
}

void CMainFrmWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	
	CBrush BackBrush(m_crBackColor);
	
	CRect rect;
	dc.GetClipBox(&rect);
	dc.FillRect(rect, &BackBrush);	

	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.bottom = rcClient.top;
	dc.FillSolidRect(rcClient, RGB(255,255,255));
	
	BackBrush.DeleteObject();
	// Do not call CFormView::OnPaint() for painting messages
}

long CMainFrmWnd::OnGetBrod(WPARAM wParam, LPARAM lParam)
{
	ST_REALDATA* pstRealData = (ST_REALDATA*)lParam;
	//pRealData = (REALDATA_st*)malloc(pstRealData->nSize);
	char* pRealData = (char*)malloc(pstRealData->nSize);
	//stRealData = *(REALDATA_st*)lParam;
	memcpy(pRealData, pstRealData->pDataBuf, pstRealData->nSize);
	SetAdd_SignalData((int)pstRealData->chRealType, pRealData, pstRealData->nSize);
	free(pRealData);
	return 0;
}

//유진 : Client To Main 신호 define
#define SIGNAL_JONGMOKSIG		30	//		종목시그널
#define SIGNAL_CONDSEARCH		31	//		조건검색
#define SIGNAL_CONDSYSTEM		32	//		시스템트레이딩

BOOL CMainFrmWnd::SendToEugeneMain(SignalData *pstSignalData)
{
	
// 	if(m_nScreenType == 1)
// 		return FALSE;
// 	
// 	EUGENE_SIGNALMSGINFO	stEUGENE_SIGNALMSGINFO;
// 	memset(&stEUGENE_SIGNALMSGINFO, 0x00, sizeof(EUGENE_SIGNALMSGINFO));
// 	ConvertForEugeneSignalManager(pstSignalData, &stEUGENE_SIGNALMSGINFO);	
// 	
// 	HWND hKSWnd;
// 	hKSWnd = (HWND)KSLink_GetEnvInfo(2);
// 	
// 	CWnd* pWndKSLink = NULL;
// 	pWndKSLink = (CWnd*)FromHandle(hKSWnd);
// 	
// 				
// 	if(pWndKSLink && ::IsWindow(pWndKSLink->m_hWnd))
// 		pWndKSLink->SendMessage(UM_SM_SIGNAL_MSG, SIGNAL_JONGMOKSIG, (LPARAM)&stEUGENE_SIGNALMSGINFO);
	
	
	return TRUE;
}



BOOL CMainFrmWnd::ConvertForEugeneSignalManager(SignalData *pstSignalData, EUGENE_SIGNALMSGINFO *pstEUGENE_SIGNALMSGINFO)
{
	CString strData, strNews;

	memcpy(pstEUGENE_SIGNALMSGINFO->m_sgType,		_T("신호"), 2); 

	strData = pstSignalData->strTime;
	memcpy(pstEUGENE_SIGNALMSGINFO->m_sgTime,	strData, strData.GetLength());	

	if(pstSignalData->nRealType == (int)TYPE_SIGNAL_SYS || pstSignalData->nRealType == (int)TYPE_SIGNAL_ALERT)
		strData.Format("%26s", " ");
	else
	{
		strData.Format("%8s", pstSignalData->strDate);
		strNews += strData;
		strData.Format("%6s", pstSignalData->strTime); 
		strNews += strData;
		strData.Format("%8s", pstSignalData->strSqNo);
		strNews += strData;
		strData.Format("%2s", pstSignalData->strProfferID);
		strNews += strData;
		strData.Format("%2s", pstSignalData->strSubProfferID); 
		strNews += strData;
		
		strData = strNews;
	}
	memcpy(pstEUGENE_SIGNALMSGINFO->m_sgnskey, strData, strData.GetLength());
	
	if(pstSignalData->nRealType == (int)TYPE_SIGNAL_SYS || pstSignalData->nRealType == (int)TYPE_SIGNAL_ALERT)
		strData.Format("%8s", " ");
	else
		strData.Format("%d", pstSignalData->strSqNo);
	
	memcpy(pstEUGENE_SIGNALMSGINFO->m_sgSeqno,	strData, strData.GetLength());//일련번호

	if(pstSignalData->nMarket < 0 || pstSignalData->nMarket > 4)
		strData.Format("%s", " ");
	else
		strData.Format("%d", pstSignalData->nMarket);
	memcpy(pstEUGENE_SIGNALMSGINFO->m_sgCodeGb,	strData, strData.GetLength());
	strData = pstSignalData->strCode;
	memcpy(pstEUGENE_SIGNALMSGINFO->m_sgCode,	strData, strData.GetLength());
	//# 신규:'N', 재진입:'R', 탈퇴:'O'
	CString strContext;

	strContext = pstSignalData->strName + " : ";
	strContext += pstSignalData->strTitle;
/*	STR_CONVERT(pBuff, pstSignalData->strType, 1, strData);
	CString strType;
	if(strData == "N")
		strType = "신규";
	else if(strData == "R")
		strType = "재진입";
	else if(strData == "O")
		strType = "탈퇴";
		
	strContext =  strContext + "!" + strType;
*/	
	
	memcpy(pstEUGENE_SIGNALMSGINFO->m_sgContext,	strContext, strContext.GetLength());

	int nPoint = pstSignalData->nPoint;
	
	if(nPoint > 0)
		memcpy(pstEUGENE_SIGNALMSGINFO->m_sgGubun,	"2",	1);
	else if(nPoint < 0)
		memcpy(pstEUGENE_SIGNALMSGINFO->m_sgGubun,	"3",	1);
	else
		memcpy(pstEUGENE_SIGNALMSGINFO->m_sgGubun,	"1",	1);
/*	memcpy(stEUGENE_SIGNALMSGINFO->m_sgBlink,	stSIGNAL_CONDITION
	memcpy(stEUGENE_SIGNALMSGINFO->m_sgContinue, stSIGNAL_CONDITION
	memcpy(stEUGENE_SIGNALMSGINFO->m_sgPriority, stSIGNAL_CONDITION
	memcpy(stEUGENE_SIGNALMSGINFO->m_sgIcon,		stSIGNAL_CONDITION
	memcpy(stEUGENE_SIGNALMSGINFO->m_sgBugaInfo, stSIGNAL_CONDITION
*/	
	return TRUE;

}

void CMainFrmWnd::SetAdd_SignalData(int nRealType, LPSTR sRealData, int nLen)
{
	BOOL bAddData = FALSE;
	
	CString strData;
	SignalData stSignalData;
	
	if ((int)TYPE_SIGNAL_SYS == nRealType)
	{
		if (nLen != sizeof(SIGNAL_CONDITION))
			return;
	}
	else if ((int)TYPE_SIGNAL_ALERT == nRealType)
	{
		if (nLen != sizeof(SIGNAL_CONDITION))
			return;
	}
	
	bAddData = GetConvert_RealToSignalData(nRealType, sRealData, stSignalData);

	if (bAddData == TRUE)
	{
// 		if(g_bSameChk && m_oldSigData.CompData(stSignalData)==TRUE)
// 			return;
// 
// 		m_oldSigData.DoCopy(stSignalData);

		//# Add Data!
		SetAdd_SignalData(stSignalData);

		int nCurSel = GetCurSearchType();

		bAddData = FALSE;
		if ((int)TYPE_SEARCH_ALL == nCurSel)   //# 전체
			bAddData = TRUE;
		else if ((int)TYPE_SEARCH_SYS == nCurSel)    //# 시스템
		{
			if ((int)TYPE_SIGNAL_SYS == nRealType)
				bAddData = TRUE;
		}
	
		if (bAddData == TRUE)
		{
			CString strCode;

			if(m_nState==1)
 				strCode = m_pCodeInput->GetCode();
			
			if (strCode.IsEmpty() == FALSE)
			{
				if (stSignalData.strCode != strCode)
					bAddData = FALSE;
			}
			
	
			if (bAddData == TRUE)
				if(m_gridResult.GetSafeHwnd())
				{
					int ntotCol = m_listField.GetCount();
					
					SIGNAL_CONDITION stSignal_Condition;
					memset(&stSignal_Condition, 0x00, SZ_SIGNAL_CONDITION);
					memcpy(&stSignal_Condition, sRealData, SZ_SIGNAL_CONDITION);
 					m_gridResult.SetAddRow(TRUE, &stSignalData, stSignal_Condition, ntotCol);
					PlaySound();
 					//SendToEugeneMain(&stSignalData);
				}
			
		}
	}
}

void CMainFrmWnd::SetAdd_SignalNowPrice(STSIGNAL_NOWPRICE stSignalNowPrice)
{
	int nRow_Prev;
	int nLastRow = 0;// m_gridSignal.GetRowCount();
//	if(m_nGridRow == nLastRow)
//				// m_gridSignal.InsertRows(++nLastRow, 1);

	if(m_nGridRow == 102)
	{
		// m_gridSignal.RemoveRows(m_nGridRow - 1, m_nGridRow -1 );
		// m_gridSignal.InsertRows(2, 1);
				
		nRow_Prev = m_nGridRow - 1;  //다음 호출시 다시 마지막 라인을 가리키기 위해
		m_nGridRow = 2;				//첫줄에 Insert
	}
	else
	{
		// m_gridSignal.InsertRows(2, 1);
		nRow_Prev = m_nGridRow;
		m_nGridRow = 2;
//		// m_gridSignal.RemoveRows(nLastRow , nLastRow);
	}


	CString strTime, strDisplayTime;
	strTime = stSignalNowPrice.strTime;

	strDisplayTime = strTime.Left(2) + ":";
	strDisplayTime += strTime.Mid(2, 2) + ":";
	strDisplayTime += strTime.Right(2);	
//#시간
	// m_gridSignal.SetCellString(m_nGridRow, 1, strDisplayTime, 0);
//#내용
//	CString strContens;
//	strContens = stSignalNowPrice.strName + " " + stSignalNowPrice.strTitle;
	// m_gridSignal.SetCellString(m_nGridRow, 2, stSignalNowPrice.strTitle, 0);
//#진입, 재진입
	CString strJobFlag;
	if(stSignalNowPrice.strJobFlag == "N")
		strJobFlag = "신규";
	else if(stSignalNowPrice.strJobFlag == "R")
		strJobFlag = "재진입";

	// m_gridSignal.SetCellString(m_nGridRow, 3, strJobFlag, 0);
//#포착가격
	// m_gridSignal.SetCellString(m_nGridRow, 4, stSignalNowPrice.strPrice, 0);

//	m_nGridRow++;
	m_nGridRow = nRow_Prev + 1;
}

BOOL CMainFrmWnd::GetConvert_RealToSignalData(int nRealType, LPSTR sRealData, SignalData &stSignalData)
{
	BOOL bResult = TRUE;

//20100419 김한두 >> 뉴스신호없음	
// 	if (((int)TYPEREAL_SIGNAL_NEWS == nRealType) || ((int)TYPEREAL_REAL_NEWS == nRealType))
// 	{
// 		SIGANL_NEWS stSIGANL_NEWS;
// 		memset(&stSIGANL_NEWS, 0x00, sizeof(SIGANL_NEWS));
// 		memcpy(&stSIGANL_NEWS, sRealData, sizeof(SIGANL_NEWS));
// 		
// 		CSIGANL_NEWSDATA  csData;
// 		csData.Convert(&stSIGANL_NEWS);
// 		
// 		stSignalData.nRealType = nRealType;
// 		stSignalData.strType = csData.m_strType;
// 		stSignalData.strSqNo = csData.m_strSqNo;
// 		
// 		CString strData;
// 		strData.Format("%0.2d",csData.m_nProfferID);
// 		stSignalData.strProfferID = strData;
// 		stSignalData.strSubProfferID = csData.m_strProfferTypeID;
// 
// 	
// 		stSignalData.nMarket = csData.m_nMarket;//유진
// 
// 		CString sRecvCode;
// //20100218 김한두 >> NH 포팅
// 		//		IMasterDataManager090803* pMasterDataManager = (IMasterDataManager090803*)AfxGetPctrInterface(UUID_IMasterDataManager);
// //		pMasterDataManager->GetShortCode(csData.m_strCode, sRecvCode);
// //20100218 김한두 <<		
// 		stSignalData.strCode = sRecvCode;
// 
// 		stSignalData.strName = csData.m_strName;
// 		
// 		stSignalData.strDate = csData.m_strDate;
// 		stSignalData.strTime = csData.m_strTime;
// 		
// 		stSignalData.strTitle = csData.m_strTitle;
// 		stSignalData.nPoint = csData.m_nSignalDegree;
// 		stSignalData.clrTitle = GetPointToColor(csData.m_nSignalDegree);
// 	}
//20100419 김한두 <<
	
	if (((int)TYPE_SIGNAL_SYS == nRealType) || ((int)TYPE_SIGNAL_ALERT == nRealType))
	{
		SIGNAL_CONDITION stSIGNAL_CONDITION;
		memset(&stSIGNAL_CONDITION, 0x00, sizeof(SIGNAL_CONDITION));
		memcpy(&stSIGNAL_CONDITION, sRealData, sizeof(SIGNAL_CONDITION));		

		char *pBuff;
		CString strData;
		STR_CONVERT(pBuff, stSIGNAL_CONDITION.sJobFlag, sizeof(stSIGNAL_CONDITION.sJobFlag), strData);
			
		//# 신규:'N', 재진입:'R', 탈퇴:'O'
		if (strData != "O")
		{
			CSIGNAL_CONDITIONDATA  csData;
			csData.Convert(&stSIGNAL_CONDITION);

			if(csData.m_strCode.GetLength()<6)
				return FALSE;
			
			stSignalData.nRealType = nRealType;
			stSignalData.strType = "";
			
			stSignalData.nMarket = csData.m_nMarket;//유진

			stSignalData.strCode = csData.m_strCode;
			stSignalData.strName = csData.m_strName;
			
			stSignalData.strDate = csData.m_strDate;
			stSignalData.strTime = csData.m_strTime;
			
			stSignalData.strTitle = csData.m_strSignalName;
			stSignalData.nPoint = csData.m_nSignalDegree;
			stSignalData.clrTitle = GetPointToColor(csData.m_nSignalDegree);

			if(m_nScreenType == 1)
				GetSignal_NowPrice(stSIGNAL_CONDITION);	//20080522 김성조 <101>	현재가 화면 Grid	
		}
		else
			bResult = FALSE;
	}
	else
		bResult = FALSE;
	
	return bResult;
}
//20080522 김성조 <101
void CMainFrmWnd::GetSignal_NowPrice(SIGNAL_CONDITION  stSIGNAL_CONDITION)
{

	STSIGNAL_NOWPRICE	stSTSIGNAL_NOWPRICE;
//	memset(&stSTSIGNAL_NOWPRICE, 0x00, sizeof(STSIGNAL_NOWPRICE));
	
	char *pBuff;
	CString strData;
	STR_CONVERT(pBuff, stSIGNAL_CONDITION.sJobFlag, sizeof(stSIGNAL_CONDITION.sJobFlag), strData);
	
	CSIGNAL_CONDITIONDATA	csData;
	csData.Convert(&stSIGNAL_CONDITION);

	stSTSIGNAL_NOWPRICE.strJobFlag = strData;
	stSTSIGNAL_NOWPRICE.strPrice = csData.m_strPrice;
	stSTSIGNAL_NOWPRICE.strCode = csData.m_strCode;	
	stSTSIGNAL_NOWPRICE.strName = csData.m_strName;
	stSTSIGNAL_NOWPRICE.strTime = csData.m_strTime;
	stSTSIGNAL_NOWPRICE.strTitle = csData.m_strSignalName;

	//if(strcmp(csData.m_strType, "NEWS") == 0 )	return;

	CString strCode = m_pCodeInput->GetCode();
	if(strCode == stSTSIGNAL_NOWPRICE.strCode)
		SetAdd_SignalNowPrice(stSTSIGNAL_NOWPRICE);
}

//20080522 김성조 101>
COLORREF CMainFrmWnd::GetPointToColor(int nPoint)
{
	COLORREF clrPointText = RGB(255, 255, 255);
	
	if (nPoint <= -3)
		clrPointText = RGB(52, 70, 133);
	else if (nPoint == -2)
		clrPointText = RGB(0, 114, 188);
	else if (nPoint == -1)
		clrPointText = RGB(0, 174, 239);
	else if (nPoint == 0)
		clrPointText = RGB(135, 135, 135);
	else if (nPoint == 1)
		clrPointText = RGB(250, 166, 26);
	else if (nPoint == 2)
		clrPointText = RGB(245, 130, 32);
	else if (nPoint >= 3)
		clrPointText = RGB(237, 28, 36);
	
	return clrPointText;
}

void CMainFrmWnd::SetAdd_SignalData(SignalData stSignalData)
{
	CString strCode, strType;
	strCode = stSignalData.strCode;
	strType.Format("%d", stSignalData.nRealType);
	
	if (strCode.IsEmpty() == TRUE)
		return;
	
	CList <SignalData, SignalData&> *plistSignalData;
	//# ALL
	if (m_mapSignalData_Type.Lookup("ALL", (void*&)plistSignalData) == TRUE)
		plistSignalData->AddTail(stSignalData);
	else
	{
		plistSignalData = new CList <SignalData, SignalData&>;
		plistSignalData->AddHead(stSignalData);
	}
	m_mapSignalData_Type.SetAt("ALL", (void*&)plistSignalData);
	
	//# Type
	if (m_mapSignalData_Type.Lookup(strType, (void*&)plistSignalData) == TRUE)
		plistSignalData->AddTail(stSignalData);
	else
	{
		plistSignalData = new CList <SignalData, SignalData&>;
		plistSignalData->AddHead(stSignalData);
	}
	m_mapSignalData_Type.SetAt(strType, (void*&)plistSignalData);
	
	//# Code
	if (m_mapSignalData_Code.Lookup(strCode, (void*&)plistSignalData) == TRUE)
		plistSignalData->AddTail(stSignalData);
	else
	{
		plistSignalData = new CList <SignalData, SignalData&>;
		plistSignalData->AddHead(stSignalData);
	}
	m_mapSignalData_Code.SetAt(strCode, (void*&)plistSignalData);
}

void CMainFrmWnd::SetSearch()
{
//유진수정	int nCurSel = m_comboSignalType.GetCurSel();
	int nCurSel = GetCurSearchType();
	
	CString strData;
	if(m_nState==1)
	{
		strData = m_pCodeInput->GetCode();
		if(strData == "" && m_gridResult.GetNumberRows() > 0)
		{
			strData = m_gridResult.GetRowToCode(0);
			// 박성경
			//m_pCodeInput->SetListCode(strData);
		}
			

	}
	
	if (strData.IsEmpty() == TRUE)
	{
		if(m_gridResult.GetSafeHwnd())
			m_gridResult.SetHideCol(1, TRUE);
		
		if ((int)TYPE_SEARCH_ALL == nCurSel)
			strData = "ALL";
//20100419 김한두 >>뉴스신호없음
// 		else if ((int)TYPE_SEARCH_NEWS == nCurSel)
// 			strData.Format("%d", (int)TYPEREAL_SIGNAL_NEWS);//20080522 김성조 <101>//TYPEREAL_SIGNAL_NEWS);
//20100419 김한두 <<
		else if ((int)TYPE_SEARCH_SYS == nCurSel)
			strData.Format("%d", (int)TYPE_SIGNAL_SYS);//20080522 김성조 <101>//TYPEREAL_SIGNAL_SYS);
		
		//(int)TYPEREAL_SIGNAL_ALERT
		if (SetFind_SignalData(FALSE, strData) > 0)
		{
			if(m_gridResult.GetSafeHwnd())
			{
				m_gridResult.SetSelectRow(-1, 0);
				strData = m_gridResult.GetRowToCode(0);
			}
		}
		else
			strData = "";
		//SetSearchData_Chart(strData);
		strData = m_pCodeInput->GetCode();
		SendTr_ChartData(strData, "");
	}
	else
	{
		//if (m_pfxCodeCtrl->IsValidCode(strData) == TRUE)	// 20111107 박성경 확인해야함
		{
			if(m_gridResult.GetSafeHwnd())
				m_gridResult.SetHideCol(1, FALSE);
			
			//SetSearchData_Chart(strData);
			SendTr_ChartData(strData, "");
			SetFind_SignalData(TRUE, strData);
		}
		// 박성경
		//else
			SetSendMainMsg("잘못된 종목 코드입니다.");
	}
}

int CMainFrmWnd::SetFind_SignalData(BOOL bCode, CString strKey)
{
	int nResult = 0;
	if(m_gridResult.GetSafeHwnd())
		m_gridResult.ClearAll();
	
	CMapStringToPtr *pmapData;
	if (bCode == TRUE)
		pmapData = &m_mapSignalData_Code;
	else
		pmapData = &m_mapSignalData_Type;
	
	CList <SignalData, SignalData&> *plistSignalData;
	if (pmapData->Lookup(strKey, (void*&)plistSignalData) == TRUE)
	{
		POSITION pos;
		SignalData stSignalData;
		if(m_gridResult.GetSafeHwnd())
			m_gridResult.SetNumberRows(plistSignalData->GetCount());
		
		BOOL bAdd;
		int nCurSel = GetCurSearchType();

		CString strCode = "";

 		if(m_nState != 0)
 			 strCode = m_pCodeInput->GetCode();
		
		for (int nIndex = plistSignalData->GetCount() - 1; nIndex >= 0; nIndex--)
		{
			pos = plistSignalData->FindIndex(nIndex);
			stSignalData = plistSignalData->GetAt(pos);
			
			bAdd = FALSE;
			if (bCode == TRUE)
			{
				if ((int)TYPE_SEARCH_ALL == nCurSel)   //# All
					bAdd = TRUE;
				else if ((int)TYPE_SEARCH_SYS == nCurSel)    //# Sys
				{
					if ((int)TYPE_SIGNAL_SYS == stSignalData.nRealType)//20080522 김성조<101>
						bAdd = TRUE;
				}
//20100419 김한두 >>뉴스신호없음
// 				else if ((int)TYPE_SEARCH_NEWS == nCurSel)    //# News
// 				{
// 					if ((int)TYPEREAL_SIGNAL_NEWS == stSignalData.nRealType) 
// 					//20080522 김성조<101>
// 						bAdd = TRUE;
// 				}
//20100419 김한두 <<
			}
			else
			{
				if (strCode.IsEmpty() == TRUE)
					bAdd = TRUE;
				else
				{
					if (strCode == stSignalData.strCode)
						bAdd = TRUE;
				}
			}
			
			if (bAdd == TRUE)
			{
				
// 				if(m_gridResult.GetSafeHwnd())
// 					m_gridResult.SetAddRow(FALSE, &stSignalData, nResult);
				
				nResult++;
			}
		}
		SetSendMainMsg("검색완료");
	}
	else
		SetSendMainMsg("입력된 종목의 데이터가 없습니다.");
	
	if(m_gridResult.GetSafeHwnd())
		m_gridResult.ClearAll();
		//m_gridResult.RedrawAll();
	return nResult;
}

void CMainFrmWnd::SetDelAll_SignalData()
{
	POSITION pos;
	CString strKey;
	CList <SignalData, SignalData&> *plistSignalData;
	for (pos = m_mapSignalData_Type.GetStartPosition(); pos != NULL;)
	{
		m_mapSignalData_Type.GetNextAssoc(pos, strKey, (void *&)plistSignalData);
		
		plistSignalData->RemoveAll();
		delete plistSignalData;
		m_mapSignalData_Type.RemoveKey(strKey);
	}
	m_mapSignalData_Type.RemoveAll();
	
	for (pos = m_mapSignalData_Code.GetStartPosition(); pos != NULL;)
	{
		m_mapSignalData_Code.GetNextAssoc(pos, strKey, (void *&)plistSignalData);
		
		plistSignalData->RemoveAll();
		delete plistSignalData;
		m_mapSignalData_Code.RemoveKey(strKey);
	}
	m_mapSignalData_Code.RemoveAll();
}

HBRUSH CMainFrmWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFrameWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(m_crBackColor/*COLOR_MAIN_CANVAS*/);
		hbr = m_brushStatic;
	}

	return hbr;
}

void CMainFrmWnd::SetOpenScreen(int nType, CString strMapNo, CString strSendData)
{
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	CString strScreenNo(strMapNo);

	pScreenWnd->OpenScreen(strScreenNo);
}

void CMainFrmWnd::SetLinkCode(CString strCode, BOOL bTagData)
{
	if (strCode.IsEmpty() == TRUE)
		return;

	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	pScreenWnd->ShareCode(strCode);
}

void CMainFrmWnd::SetLinkTime(CString strTime)
{
	//m_lTimeEdit.SetCaptionCtrl(strTime);
}

void CMainFrmWnd::SetSendMainMsg(CString strMsg)
{
//20100506 김한두 >>	m_lRunForm.SetFrameMsgText(strMsg);
}

void CMainFrmWnd::SetSearchData_Chart(CString strCode)
{
	
// 	m_ctrlJMCombo.SetCaptionCtrlEx(strCode, FALSE);//SetCaption(strCode);
// 	m_pDllGate->SetShareCode(-1, strCode);
// 	SendTr_ChartData(strCode, "");
	
}

void CMainFrmWnd::SetSearchData_Time(CString strTime)
{
	m_strTime = strTime;
}

void CMainFrmWnd::SetSearchData_TimeSet()
{
	CString strPropName;
	strPropName = "SetTimeMark(" + m_strTime + ", 1)";
//20100218 김한두 >> NH 포팅 
	//	m_ctrlChart.GetChartProperty(strPropName);
//20100218 김한두 <<
}

// 메인의 저장화면관련.
void CMainFrmWnd::OnFormGetPageSave(LPCTSTR strFileName)
{
}

// 메인의 저장화면관련.
void CMainFrmWnd::OnFormSetPageSave(LPCTSTR strFileName)
{
}

//메인의 스킨변경시 불린다.
LRESULT CMainFrmWnd::OnRMSGColorChange()
{
	
// 	IKSLibManager*	pKSMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager, 1);	
// 	if(!pKSMng) return 0L;
// 	
// 	COLORREF clrBkgnd = pKSMng->Lib_KSLink_GetColor(1);
// 	
// 	ST_TABCOLOR stTabColor;
// 	
// 	stTabColor.clrTabSelText		= pKSMng->Lib_KSLink_GetColor(101);
// 	stTabColor.clrTabUnSelText		= pKSMng->Lib_KSLink_GetColor(105);
// 	stTabColor.clrTabBody			= pKSMng->Lib_KSLink_GetColor(100);
// 	
// 	stTabColor.clrTabSelGraUp		=  pKSMng->Lib_KSLink_GetColor(100); 
// 	stTabColor.clrTabSelGraDown		=  pKSMng->Lib_KSLink_GetColor(101);
// 	stTabColor.clrTabUnSelGraUp		=  pKSMng->Lib_KSLink_GetColor(104); 
// 	stTabColor.clrTabUnSelGraDown	=  pKSMng->Lib_KSLink_GetColor(101);
// 	
// 	m_crBackColor = clrBkgnd;
// 	
// 	if(m_brushStatic.DeleteObject())
// 		m_brushStatic.CreateSolidBrush(m_crBackColor);
// 		
// 	ST_GRIDCOLOR stGridColor;
// 	stGridColor.clrGridHeader = pKSMng->Lib_KSLink_GetColor(64);
// 	stGridColor.clrGridHText = pKSMng->Lib_KSLink_GetColor(65);
// 	stGridColor.clrGridSBk = pKSMng->Lib_KSLink_GetColor(70);
// 	stGridColor.clrGridHilight = RGB(220,251,253); // 박성경 셀 선택 색
// 	
// 	ChangeGridColor(stGridColor); // 박성경 그리드 색
// 	
// 	Invalidate();
	

 	return TRUE;
}

void CMainFrmWnd::ChangeSkinColor(COLORREF clrBkgnd, ST_TABCOLOR stTabColor)
{
	if(!clrBkgnd)
		return;
	
	m_crBackColor = clrBkgnd;
	
	m_stTabColor = stTabColor;
	m_clrBkgnd = clrBkgnd;
	
	if(m_brushStatic.DeleteObject())
		m_brushStatic.CreateSolidBrush(m_crBackColor);
	
	ST_GRIDCOLOR stGridColor;
	stGridColor.clrGridHeader = GetAxColor(74);
	stGridColor.clrGridHilight = GetAxColor(78);
	
	ChangeGridColor(stGridColor);

	m_radioTotal.Invalidate();
	m_radioCode.Invalidate();
	m_comboSelInterval.Invalidate();

	m_pwndSymbol->SendMessage(WM_USER, MAKEWPARAM(DLL_SETPAL, 0));
	m_pwndSymbol->Invalidate();

	Invalidate();	
}

void CMainFrmWnd::ChangeGridColor(ST_GRIDCOLOR stGridColor)
{
	m_gridResult.ChangeGridColor(stGridColor);
	m_gridResult.Invalidate();
}

void CMainFrmWnd::OnRadioBtnClk(UINT nID)
{	
	if(m_radioTotal.GetDlgCtrlID() == nID)
	{	
		m_nState=0;
		CString strData;
		m_radioTotal.SetCheck(TRUE);
		m_radioCode.SetCheck(FALSE);
		strData = m_pCodeInput->GetCode();
		DoBtnInit_Code(FALSE);			
	
		m_pCodeInput->EnableWindow(FALSE);
//		CWnd::FromHandle(m_hKSJongName)->ShowWindow(SW_HIDE);
	}
	else
	{
		m_radioTotal.SetCheck(FALSE);
		m_radioCode.SetCheck(TRUE);
		
		m_pCodeInput->EnableWindow(TRUE);
// 		CWnd::FromHandle(m_hKSJongName)->ShowWindow(SW_SHOW);
		

		int nOldState = m_nState;
		m_nState=1;
		if(nOldState==0)	SetSearch();
		
	}
	//CWnd::FromHandle(m_hKSJongName)->ShowWindow(m_nState);
}

void CMainFrmWnd::OnChkBtnClk(UINT nID)
{
	switch(nID)
	{
	case IDC_CHK_MARKETCONDITION:
		break;
	case IDC_CHK_TECHINDEX:
		break;
	case IDC_CHK_DEALAGENCY:
		break;
	case IDC_CHK_NEWS:
		{
			if(!m_chkSys.GetCheck())
				m_chkNews.SetCheck(TRUE);
	//		else if(!m_chkSys.GetCheck())
	//			m_chkSys.SetCheck(FALSE);
			OnChangeSignalType();
			break;
		}		
	case IDC_CHK_SYS:
		{
			if(!m_chkNews.GetCheck())
				m_chkSys.SetCheck(TRUE);
	//		else
	//			m_chkNews.SetCheck(FALSE);
			OnChangeSignalType();
			break;
		}		
	default:
		break;
	}

}

short CMainFrmWnd::GetCurSearchType()
{
	short nResult = 0;
/*
	if(m_chkNews.GetCheck())
		nResult = 0;
	else if(!m_chkNews.GetCheck())
		nResult = 2;
*/
	if(m_chkNews.GetCheck())
	{
		if(m_chkSys.GetCheck())
			nResult = 0;
		else
			nResult = 1;
	}
	else if(m_chkSys.GetCheck())
		nResult = 2;
	
	return nResult;
}

void CMainFrmWnd::SetControlDispatch()
{
	
// 	CSCNode* pSCNode = NULL;
// 	for(int nIndex = 0; nIndex < sizeof(nCtrlID)/sizeof(int); nIndex++)
// 	{
// 		if(m_MapControlName->Lookup(sCtrlName[nIndex], (CObject*&)pSCNode) )
// 		{
// 			pSCNode->GetControlWnd()->m_pWnd->SetDlgCtrlID(nCtrlID[nIndex]);
// 			pSCNode->GetControlWnd()->SetCtrlID(nCtrlID[nIndex]);
// 		}
// 	}	
	
}

void CMainFrmWnd::DoKSControlMove(LPCSTR szCtrlName, CWnd* pWnd)
{
	if(!pWnd)
		return;

	pWnd->ShowWindow(SW_HIDE);

	CRect ctlrect;
	pWnd->GetWindowRect(&ctlrect);
	ScreenToClient(&ctlrect);

	VARIANT nLeft, nTop, nWidth, nHeight;
	nLeft.vt = VT_I2;
	nTop.vt = VT_I2;
	nWidth.vt = VT_I2;
	nHeight.vt = VT_I2;

	nLeft.iVal = ctlrect.left;
	nTop.iVal = ctlrect.top;
	nWidth.iVal = ctlrect.Width();
	nHeight.iVal = ctlrect.Height();	


//20100506 김한두 >>	m_lRunForm.SetControlPos(szCtrlName, nLeft.iVal, nTop.iVal, nWidth.iVal, nHeight.iVal);
}

void CMainFrmWnd::OnEuBtnClk(UINT nID)
{
	switch(nID)
	{
	case ID_btnSignalSet:
		OnBtnSignalSet();
		break;
	case ID_btnSearch:
		OnBtnSearch();
		break;
	default:
		break;
	}
}

CWnd* CMainFrmWnd::GetFrameWnd(CWnd* pWnd)
{
	CWnd* pParentWnd = pWnd->GetParent();  // start with one parent up
	while (pParentWnd != NULL)
	{
		DWORD dwExStyle = GetWindowLong(pParentWnd->GetSafeHwnd(), GWL_STYLE);
		if ( dwExStyle & 0x8000)
		{
			return pParentWnd;
		}
		pParentWnd = pParentWnd->GetParent();
	}
	return NULL;
}

void CMainFrmWnd::DoDllSizeEx(HWND _hChildFrame)
{
	HWND hChildFrame = _hChildFrame;

	if(hChildFrame==NULL)
	{
		CWnd* pChildFrame = GetFrameWnd(this);
		hChildFrame = pChildFrame->GetSafeHwnd();
	}

	
// 	if(hChildFrame)
// 		::SendMessage(hChildFrame, UM_SET_FORMMAXSIZE, 34, MAKELPARAM(0, 0));
	
}

void CMainFrmWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	
	/*m_pDllGate->DoPopupMainPlatformMenu(point, this);*/
	

	CFrameWnd::OnRButtonUp(nFlags, point);
}

//extern void KSLink_SetFormLinkTagData( CString sTag, CString sValue );
//extern void KSLink_SetLinkTagData( LPCTSTR sTag, LPCTSTR sValue);
// 	void CMainFrmWnd::SetCommMenu(POINT *point)
// 	{
// 		HWND hChildFrame = m_hChildFrame;
// 		if(!hChildFrame) return;
// 
// 		long lMenu = ::SendMessage(hChildFrame, UM_GET_SCREENLINKMENU, 0, 3 );
// 		if ( lMenu > 0 )
// 		{
// 			CMenu* pMenu = (CMenu*)lMenu;
// 			ASSERT ( pMenu );
// 
// 			GetCursorPos(point);
// 			UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
// 			int nRetValue = pMenu->TrackPopupMenu(nFlagsForMenu, point->x, point->y, this);
// 			pMenu->DestroyMenu();
// 			delete pMenu;
// 			
// 			if( nRetValue>0 )
// 			{
// 				::PostMessage(hChildFrame, WM_COMMAND, nRetValue, 0);
// 				return;
// 			}
// 		}
//	}

void CMainFrmWnd::SetCommMenu(POINT *point)
{

}

void CMainFrmWnd::SetPopupMenuResultCtrl(CPoint &ptMenu)
{
	CString strCode, strCodeName; 
//	int nMarkType;
	
	int nRow = m_gridResult.GetCurrentRow();
	if(nRow<0) return;

	strCode = m_gridResult.GetRowToCode(nRow);

	//메뉴선택시 조건검색임시정보에 셋팅.
	SetResultSearchTemp();

	
// 	int nRetValue = m_pDllGate->DoPopupDllMenu(ptMenu , 2 , strCode);
// 	if(!nRetValue) return;
//	
//
//	OnMenuResultCtrl(nRetValue);
	
}

// 팝업 메뉴 command 처리
void CMainFrmWnd::OnMenuResultCtrl(UINT nIndex)
{
	CString strCode;
//	int nMarkType;
	
	int nDataCnt;
	nDataCnt = m_gridResult.GetNumberRows();
	
	if(nDataCnt <= 0)
	{
		SetSendMainMsg("전송할 데이터가 없음.");
		return;
	}

	
// 	if(nIndex == SCREEN_GWAN_DLG )
// 	{
// 		CString szCodeList;
// 		CString strCode;
// 		if(nDataCnt<=0) return;
// 		
// 		CUGCell cellItemName, cellItemCode;
// 		CString szFindKey;
// 		int nSendCnt = 0;	//50개까지만 관심리스트로 보내기.
// 		for (int nRow = 0; nRow < nDataCnt && nSendCnt<=50; nRow++)
// 		{
// 			strCode = m_gridResult.GetRowToCode(nRow);
// 			szFindKey.Format("%s%s", strCode, "!");
// 
// 			if(!strCode.IsEmpty() && szCodeList.Find(szFindKey)<0)
// 			{
// 				szCodeList	= szCodeList + strCode + "!";
// 				nSendCnt++;
// 			}
// 		}
// 
// 		m_pDllGate->DoRegOpen(10000, szCodeList);
// 		
// 	}
	
}

const UINT RMSG_EVENT_CODEINPUT = ::RegisterWindowMessage(_T("RMSG_EVENT_CODEINPUT"));

LRESULT CMainFrmWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CFrameWnd::WindowProc(message, wParam, lParam);
}

int CMainFrmWnd::SetResultSearchTemp()
{
	CString strCode, strCodeName; 
	CStringArray strArrayCode, strArrayCodeName;

//	for (int nRow = 0; nRow < nDataCnt && nSendCnt<=50; nRow++)
//	{
//		strCode = m_gridResult.GetRowToCode(nRow);
//	}
	int nDataCnt = m_gridResult.GetNumberRows();
// 	if(nDataCnt>0)
// 	{
// 		for (int nRow = 0; nRow < nDataCnt; nRow++)
// 		{
// 			strCode = m_gridResult.GetRowToCode(nRow);
// 			strArrayCode.Add(strCode);
// 
// 			strCodeName = m_gridResult.QuickGetText(1, nRow);
// 			strCodeName.TrimRight();
// 			strArrayCodeName.Add(strCodeName);
// 		}
// 		IMasterDataManagerLast* pDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
// 		if(pDataManager == NULL)
// 			return 0;
// 		pDataManager->SetUserMaster(usrMasterCondition,strArrayCode.GetSize(),&strArrayCode,&strArrayCodeName);
// 	}
	return nDataCnt;
}

BOOL CMainFrmWnd::Load_LibSignalMng()
{
	m_hSignalMng = NULL;
	
	CString strDllPath;
	strDllPath.Format("%s%s", m_strAppPath, "\\dev\\lib_signalmng.dll");

	CFileFind ff;
	if(!ff.FindFile(strDllPath))
		return FALSE;

	m_hSignalMng = LoadLibrary(strDllPath);
	if(m_hSignalMng)
	{
		Lib_Open		= (_DLL_SetLibOpen)GetProcAddress(m_hSignalMng, "_DLL_SetLibOpen");
		Lib_Close		= (_DLL_SetLibClose)GetProcAddress(m_hSignalMng, "_DLL_SetLibClose");
		Lib_SetSignal	= (_DLL_SetSiganl)GetProcAddress(m_hSignalMng,"_DLL_SetSiganl");

		m_pWndDummy = Lib_Open(this, NULL, m_pModuleMainWnd);
	}

	return TRUE;
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

int CMainFrmWnd::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	return ::MessageBox(m_hWnd, lpszText, lpszCaption, nType);
}

void CMainFrmWnd::PutLinkData(CString strType, CString strCode)
{
	if(m_pCodeInput)
		m_pCodeInput->SetCode(strCode);

	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	pScreenWnd->ShareCode(strCode);
}

void CMainFrmWnd::Module_SendTr(CString szTRNo)
{
	m_CommCritical.Lock();
	
	CScreenWnd* pScreenWnd = (CScreenWnd*)GetParent();
	
	int nType = 2;

	char* lpData = new char[sizeof(int) + 1];
	::memset(lpData, 0x00, sizeof(int) + 1);
	memcpy(lpData, &nType, sizeof(int) + 1);
	
	if(szTRNo == TRNO_SET_REAL)
	{
		pScreenWnd->SendTR(TRNO_SET_REAL, lpData, sizeof(int), TRKEY_SET_REAL, US_KEY);
	}
// 	else if(szTRNo == TRNO_UNSET_REAL)
// 	{
// 		pScreenWnd->SendTR(TRNO_UNSET_REAL, lpData, sizeof(int), TRKEY_UNSET_REAL, US_KEY);
// 	}
	m_CommCritical.Unlock();
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
	
	if (dlg.DoModal() == IDOK)
	{
		
	}

	AfxSetResourceHandle(hInstSave);
}


BOOL CMainFrmWnd::SetSaveFixUserFieldData(CList<HeadFixInfo, HeadFixInfo&> *plistFixUser,
										  CString strVersion)
{
	BOOL bResult = TRUE;

	CString strRoot;
	strRoot = Variant(homeCC);
	CString strFileRoot = strRoot + USRDIR + Variant(nameCC) + FOLDER_FINDDATA + "\\" + FIXFIELD_USER_SIGNAL_FILE;

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

BOOL CMainFrmWnd::GetLoadFixUserFieldData(MSXML::IXMLDOMNodePtr pNIndex,
										  CList<HeadFixInfo, HeadFixInfo&> &listFixList)
{
	BOOL bResult = TRUE;

	MSXML::IXMLDOMNamedNodeMapPtr	pNAIndex;

	MSXML::IXMLDOMNodeListPtr		pNS_FItem;		// Item List
	MSXML::IXMLDOMNodePtr			pN_FItem;		// Item
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


void CMainFrmWnd::SetLoad_FieldInfo()
{
	MSXML::IXMLDOMDocumentPtr pXmlDom ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);

	CString strRoot;
	strRoot =  Variant(homeCC);
	
	CString strFileRoot = strRoot + FOLDER_DATA + "\\" + FIXFIELD_SYS_FILE;

	
	if (GetLoadXml(strFileRoot, pXmlDom) == TRUE)
	{
		GetLoadFixSysFieldData(pXmlDom->selectSingleNode("INDEX"), m_listFixHeadSys);
		m_strFixFieldVersion = GetVersion_FieldFile(pXmlDom->selectSingleNode("INDEX"));
	}

	BOOL bSetInit = TRUE;
	strFileRoot = strRoot + USRDIR + Variant(nameCC) + FOLDER_FINDDATA + "\\" + FIXFIELD_USER_SIGNAL_FILE;

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

BOOL CMainFrmWnd::GetLoadFixSysFieldData(MSXML::IXMLDOMNodePtr pNIndex,
										 CList<HeadInfoType, HeadInfoType&> &listFixList)
{
	BOOL bResult = TRUE;

	MSXML::IXMLDOMNodeListPtr		pNS_FItem;	// Item List
	MSXML::IXMLDOMNodePtr			pN_FItem;	// Item
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
			CString szIndex;
			szIndex.Format("%d", nCount);
			m_mapResultFieldIndex.SetAt(stHeadInfoType.strHeadName, szIndex);
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

int CMainFrmWnd::GetCount_FixHead(BOOL &bHaveCodeFiled, CUIntArray &iaFixHeadId)
{
	int nResult = 0;

	bHaveCodeFiled = FALSE;
	iaFixHeadId.RemoveAll();

	POSITION pos;
	HeadFixInfo		stHeadFixInfo;
	for (int nIndex = 0; nIndex < m_listFixHeadUser.GetCount(); nIndex++)
	{
		pos = m_listFixHeadUser.FindIndex(nIndex);
		stHeadFixInfo = m_listFixHeadUser.GetAt(pos);

		if (stHeadFixInfo.bView == TRUE)
		{
			if (stHeadFixInfo.stHeadType.strDataType == "code")
				bHaveCodeFiled = TRUE;
			else
				iaFixHeadId.Add(stHeadFixInfo.stHeadType.lHeadID);

			nResult++;
		}
		else
		{
			if (stHeadFixInfo.stHeadType.strHeadName == (CString)GRID_MOVEFIELD_POS_NAME)
				iaFixHeadId.Add(stHeadFixInfo.stHeadType.lHeadID);
		}
	}

	return nResult;
}

void CMainFrmWnd::GetTabClr(COLORREF* clrBkgnd, ST_TABCOLOR* stTabColor)
{
	*clrBkgnd = m_clrBkgnd;
	*stTabColor = m_stTabColor;
}

CString CMainFrmWnd::GetFilePath(CString strLoadType)
{
	CString strFile, strFolder;
	CString strFilePath = "";

	BOOL bUserFolder = FALSE;
	BOOL bSetIndustry = TRUE;

	strFile = (CString)FINDERINFO_INIFILE;

	if (bUserFolder == TRUE)
	{
		CString strUserPath;
		strUserPath.Format("%s%s%s", Variant(homeCC), USRDIR, Variant(nameCC));
		strFilePath = (CString)strUserPath + (CString)FOLDER_FINDDATA + "\\" + strFile;
	}
	else
	{
		CString strAppPath;
		strAppPath = Variant(homeCC);
		
		strFilePath = strAppPath + (CString)FOLDER_DATA + "\\" + strFile;
	}

	return strFilePath;
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
		m_pDlgChart->m_ChartItem.SetDrdsAdvise(FALSE);
		
		m_pDlgChart->m_ChartItem.AddRealItem(strCode);		// 실시간등록할 종목코드
		m_pDlgChart->m_ChartItem.SetDrdsAdvise(TRUE);
		
		int nData;
		char*pBuff;
		INT_CONVERT(pBuff, pStOutput->fcnt, sizeof(pStOutput->fcnt), nData);
		
		m_pDlgChart->m_ChartItem.SetOnePageDataCount(min(40, nData));
	}
}

void CMainFrmWnd::OnBtnDay()   
{	
	m_strChartTerm = "2";
	m_btnDay.SetCheck(TRUE);
	m_btnWeek.SetCheck(FALSE);
	m_btnMonth.SetCheck(FALSE);
	m_btnMin.SetCheck(FALSE);
	m_btnTick.SetCheck(FALSE);

	m_comboSelInterval.EnableWindow(FALSE);

	CString szCode = m_pCodeInput->GetCode();
	if(szCode != _T(""))
	{
		SendTr_ChartData(szCode, "");
	}
}

void CMainFrmWnd::OnBtnWeek()
{
	m_strChartTerm = "3";
	m_btnDay.SetCheck(FALSE);
	m_btnWeek.SetCheck(TRUE);
	m_btnMonth.SetCheck(FALSE);
	m_btnMin.SetCheck(FALSE);
	m_btnTick.SetCheck(FALSE);

	m_comboSelInterval.EnableWindow(FALSE);

	CString szCode = m_pCodeInput->GetCode();
	if(szCode != _T(""))
	{
		SendTr_ChartData(szCode, "");
	}
}

void CMainFrmWnd::OnBtnMonth()
{
	m_strChartTerm = "4";
	m_btnDay.SetCheck(FALSE);
	m_btnWeek.SetCheck(FALSE);
	m_btnMonth.SetCheck(TRUE);
	m_btnMin.SetCheck(FALSE);
	m_btnTick.SetCheck(FALSE);

	m_comboSelInterval.EnableWindow(FALSE);

	CString szCode = m_pCodeInput->GetCode();
	if(szCode != _T(""))
	{
		SendTr_ChartData(szCode, "");
	}
}

void CMainFrmWnd::OnBtnMinute()
{
	m_strChartTerm = "1";
	m_btnDay.SetCheck(FALSE);
	m_btnWeek.SetCheck(FALSE);
	m_btnMonth.SetCheck(FALSE);
	m_btnMin.SetCheck(TRUE);
	m_btnTick.SetCheck(FALSE);

	m_comboSelInterval.EnableWindow(TRUE);

	CString szCode = m_pCodeInput->GetCode();
	if(szCode != _T(""))
	{
		SendTr_ChartData(szCode, "");
	}
}

void CMainFrmWnd::OnBtnTick()
{
	m_strChartTerm = "0";
	m_btnDay.SetCheck(FALSE);
	m_btnWeek.SetCheck(FALSE);
	m_btnMonth.SetCheck(FALSE);
	m_btnMin.SetCheck(FALSE);
	m_btnTick.SetCheck(TRUE);

	m_comboSelInterval.EnableWindow(TRUE);

	CString szCode = m_pCodeInput->GetCode();
	if(szCode != _T(""))
	{
		SendTr_ChartData(szCode, "");
	}
}

void CMainFrmWnd::OnChangeInterval()
{
	CString szCode = m_pCodeInput->GetCode();
	if(szCode != _T(""))
	{
		SendTr_ChartData(szCode, "");
	}
}
// 박성경 >>
// void CMainFrmWnd::ReceiveChartReal(LPCTSTR pszData, UINT nLength, WPARAM wParam)
// {
// 	/*m_pwndChartCtrl->m_pTransaction->OnGetDataReal(pszData, nLength, wParam);*/
// }
// 박성경 <<

void CMainFrmWnd::SetChangeCode(CString strCode)
{
	if(!m_pCodeInput)
		return;
	
	m_pCodeInput->SetCode(strCode);
	OnCodeEnter();
}

void CMainFrmWnd::DoSetPartnerSite(IPartnerManager* pPtManager)
{
	m_pPTManager = pPtManager;
	m_szPTManager.Format("%08X", pPtManager);
	if(pPtManager) pPtManager->SetSite(&m_xPartnerSite);

	m_pCodeInput->EnableWindow(FALSE);
	
	m_pCodeInput->SetHistory(Variant(historyCC, "1301"));
	CString strHistoryCode, strCode;
	strHistoryCode = Variant(historyCC, "1301");
	int nFind = strHistoryCode.Find(" ");
	if(nFind > -1)
		strCode = strHistoryCode.Left(nFind);
	
	if(strCode.IsEmpty())
		strCode = "024110";
	
	m_pCodeInput->SetCode(strCode);

	OnBtnMinute();
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
			
			m_pwndSymbol = (*axCreate)(m_pModuleMainWnd, &param);
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
	
	m_pModuleMainWnd->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strData.GetLength()), (LPARAM)wb);

	delete[] wb;
}

void CMainFrmWnd::PlaySound()
{	
	CString strFilePath, strAppPath, strFileName;
	GetDir_DrfnRoot(strAppPath);
	strFileName = strAppPath + "\\user\\" + "soundSet.ini";
	char sztemp[128];
	GetPrivateProfileString(_T("CHKSOUND"), _T("CHECKSTATE"), "0", sztemp, sizeof(sztemp), strFileName);
	if (atoi(sztemp) == 0)
	{
		return;
	}
	GetPrivateProfileString(_T("ALARMSET"), _T("alarm"), "1", sztemp, sizeof(sztemp), strFileName);
	strFilePath = sztemp;

	if(strFilePath.GetLength() > 0)
		sndPlaySound(strFilePath, SND_ASYNC );	
}