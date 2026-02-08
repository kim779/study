// StdDialog.cpp : implementation file
//
#include "./define/ChartMsgDef.h"
#include "ChartItem.h"

#include "stdafx.h"
#include "MultiChart.h"
#include "IO.h"
#include "direct.h"
#include "StdDialog.h"
#include "MiniBarDlg.h"
#include "MiniTickBarDlg.h"
#include "TabCodeSearch.h"
#include "ChartListDlg.h"
#include "FloatingHogaDlg.h"

#include "../COMMONTR/ChartRealConverter.h"
#include "DataWindowDlg.h"
#include "CodeDataFromInputBar.h"
#include "./define/ReserveShareName.h"
#include "./define/MessageDefine.h"
#include "./define/DragNDropDefine.h"
#include "./include/UChar.h"

#include "../chart_common/StatusMsgDef.h"
#include "../../inc/IAUPlatformDispMng.h"
//#include "../../inc/IKSLibMng.h"
#include "../../inc/ExCommonHeader.h"
#include "../../inc/RealUpdate.h"
#include "../../inc/IAUDataMonitorMng.h"	// UUID_IAUDataMonitorManager : 콜백함수 받는데 사용

#include "../../inc/AZUtil_Ctrl.h"			// OS체크루틴. IsSystemChart()에서 사용
#include "TabSystemTradingEx.h"				// 시스템트레이딩 전략 저장/불러오기

#include "../../inc/RMsgUserInfo.h"

#ifdef __MULTI_LANG__
	#include "../Include_Chart/Dll_Load/IMetaTable.h"	//@Solomon-MultiLang:091117
#endif

//#include "../../../../../include/CommDefine.h"

#include "../CommonTR/KXS3.h"	// 주식
#include "../CommonTR/KXYS.h"	// 주식(예상)
#include "../CommonTR/EWs3.h"	// ELW
#include "../CommonTR/IXIJ.h"	// 업종
#include "../CommonTR/FFC0.h"	// 선물
#include "../CommonTR/OOC0.h"	// 옵션
#include "../CommonTR/JFC0.h"	// 주식선물
#include "../CommonTR/JOC0.h"	// 주식옵션
#include "../CommonTR/HWFJ.h"	// 해외
#include "../CommonTR/GTFJ.h"	// 상품선물
#include "../CommonTR/FOJS.h"	// FO 지수

//#include "../../../../../CommonSrc/CtlColor.h"

// 돋보기 구현 - ojtaso (20080519)
#include "DlgGlass.h"
#define	WMU_GET_KEY_INFO_ARRAY		WM_USER + 2253	// 해당 AUP에서 입력되어 있는 Key값의 목록을 가져온다. LPARAM은 CStringArray *

#define	WIDTH_SIDEBAR		3
//#include "../../inc/RegisterWindowMessage.h"

const UINT RMSG_GET_SCREENTYPE = ::RegisterWindowMessage(_T("RMSG_GET_SCREENTYPE"));
const UINT RMSG_GET_TEMPDATA = ::RegisterWindowMessage(_T("RMSG_GET_TEMPDATA"));
const UINT RMSG_GET_LINKINFODATA = ::RegisterWindowMessage(_T("RMSG_GET_LINKINFODATA"));
const UINT RMSG_GET_ITEMNAMEFORMITEMCODE = ::RegisterWindowMessage(_T("RMSG_GET_ITEMNAMEFORMITEMCODE"));
const UINT RMSG_GET_MARKETTYPEFORMITEMCODE = ::RegisterWindowMessage(_T("RMSG_GET_MARKETTYPEFORMITEMCODE"));
const UINT RMSG_SET_CODEINPUTMARKETTYPE = ::RegisterWindowMessage(_T("RMSG_SET_CODEINPUTMARKETTYPE"));
const UINT RMSG_GET_ACCOUNTLIST = ::RegisterWindowMessage(_T("RMSG_GET_ACCOUNTLIST"));
const UINT RMSG_SET_ITEMCODENAME = ::RegisterWindowMessage(_T("RMSG_SET_ITEMCODENAME"));
//const UINT RMSG_ADDPOPUPMENUITEM = ::RegisterWindowMessage(_T("RMSG_ADDPOPUPMENUITEM"));
//const UINT RMSG_ACTIONPOPUPMENUITEM = ::RegisterWindowMessage(_T("RMSG_ACTIONPOPUPMENUITEM"));
const UINT RMSG_OPENSCREEN = ::RegisterWindowMessage(_T("RMSG_OPENSCREEN"));
const UINT RMSG_GET_HOSTDATE = ::RegisterWindowMessage(_T("RMSG_GET_HOSTDATE"));
const UINT RMSG_GET_ACCTNUM = ::RegisterWindowMessage(_T("RMSG_GET_ACCTNUM"));
const UINT RMSG_CHANGEACCTNUM = ::RegisterWindowMessage(_T("RMSG_CHANGEACCTNUM"));
const UINT RMSG_SET_PASSWORDINFO = ::RegisterWindowMessage(_T("RMSG_SET_PASSWORDINFO"));
const UINT RMSG_FORMLOADSAVE = ::RegisterWindowMessage(_T("RMSG_FORMLOADSAVE"));

const UINT RMSG_RECEIVEREPLAYDATA = ::RegisterWindowMessage(_T("RMSG_RECEIVEREPLAYDATA"));
const UINT RMSG_GET_TRCOMM = ::RegisterWindowMessage(_T("RMSG_GET_TRCOMM"));

//const UINT RMSG_TEST_TR = ::RegisterWindowMessage("RMSG_TEST_TR");
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStdDialog dialog
#define ID_LEFTBAR			IDD_LEFTBAR
#define ID_INPUTBAR			IDD_INPUTBAR
#define ID_MINIBAR			IDD_MINIBAR
#define ID_EXTINPUTBAR		IDD_EXTINPUTBAR
#define ID_SPLITTER			7000
#define ID_CHART			8000
#define ID_TOOLBAR			IDD_DLG_TOOLBAR

// shared Info Name.. gm0604 2005.8.24
#define FROMSTCENTER	"FromSTCenter"

#define DEFAULT_CHARTDATA_COUNT_INT		400
#define DEFAULT_CHARTDATA_COUNT_CHAR	"400"

#define WIDTH_SIDEBAR_EXTEND		170
#define WIDTH_SIDEBAR_COLLAPSE		10


extern HINSTANCE g_hInstance;

// vntsorl:[0001250] [5001] 종합차트 - 일주월분초틱 주기별로 지표 설정 및 설정값을 다르게 할 수 있도록 개선요청
//20110502 이문수 >>
int GetCommonDataInfo(HWND hWnd, int nDataIndex, int nSubInfo)
{
	HWND hParent = NULL;
	while(hParent = ::GetParent(hWnd))
	{
		CStdDialog * pStdDialog = (CStdDialog *)::SendMessage(hParent, UMSG_IS_PARENT_STDDIALOG, 0, 0);
		if(pStdDialog)
		{
			int nResult = ::SendMessage(hParent, UMSG_GET_COMMON_DATA, (WPARAM)nDataIndex, (LPARAM)nSubInfo);
			return nResult;
		}
		hWnd = hParent;
	}
	return 0;
}

int SetCommonDataInfo(HWND hWnd, int nDataIndex, int nData)
{
	HWND hParent = NULL;
	while(hParent = ::GetParent(hWnd))
	{
		CStdDialog * pStdDialog = (CStdDialog *)::SendMessage(hParent, UMSG_IS_PARENT_STDDIALOG, 0, 0);
		if(pStdDialog)
		{
			int nResult = ::SendMessage(hParent, UMSG_SET_COMMON_DATA, (WPARAM)nDataIndex, (LPARAM)nData);
			return nResult;
		}
		hWnd = hParent;
	}
	return 0;
}

CString GetTrCodeFromCode(HWND hWnd, CString strCode)
{
	HWND hParent = NULL;
	while(hParent = ::GetParent(hWnd))
	{
		CStdDialog * pStdDialog = (CStdDialog *)::SendMessage(hParent, UMSG_IS_PARENT_STDDIALOG, 0, 0);
		if(pStdDialog)
		{
			CString strTRCode;
			char szTemp[1024]={0,};
			int nCodeType, nMarketType = pStdDialog->GetMarketTypeFromCode(strCode, nCodeType);
			switch (nMarketType)
			{
			case CMasterTypeInfo::STOCK_CHART:
				strTRCode = QUERY_STRJONGMOK;
				break;
			case CMasterTypeInfo::UPJONG_CHART:
				strTRCode = QUERY_STRUPJONG;
				break;
			case CMasterTypeInfo::FUTOPT_CHART:
				strTRCode = QUERY_STRFUTOPT;
				break;
			case CMasterTypeInfo::STOCKFUTOPT_CHART:
				strTRCode = QUERY_STRSTOCKFUTOPT;
				break;
			case CMasterTypeInfo::FOREIGN_CHART:
				strTRCode = QUERY_STRFOREIGN;
				break;
			case CMasterTypeInfo::FUOPT_JIPYO_CHART:
				strTRCode = QUERY_STRFOJIPYO;
				break;
			}

			return strTRCode;
		}
		
		hWnd = hParent;
	}
	return "";	
}
//20110502 이문수 <<

CString Parser(LPCTSTR lpInputData, int *pNShifter ) 
{
	if(!pNShifter) return "";
	CString strInputData;
	int nPosStart = *pNShifter;
	int nPosEnd = 0;
	strInputData = lpInputData;
	nPosEnd = strInputData.Find(',',nPosStart);
	if(nPosEnd==-1)
	{
		nPosEnd = strInputData.GetLength();
		if(nPosStart>=nPosEnd)
		{
			return "";
		}		
	}
	*pNShifter = nPosEnd + 1;
	return strInputData.Mid(nPosStart,nPosEnd-nPosStart);
}

// hmc skin table id
#define COLORTBL_BKGRND			2			// 배경바탕색
#define COLORTBL_TAB_SEL_TEXT	12			// 탭 활성 텍스트
#define COLORTBL_TAB_NOR_TEXT	14			// 탭 비활성 텍스트
#define COLORTBL_TAB_BKGRND		10			// 텝 바디 배경
#define COLORTBL_TAB_SEL_GDSTT	11			// 탭 활성 헤더 그라데이션 시작
#define COLORTBL_TAB_SEL_GDEND	261			// 탭 활성 헤더 그라데이션 끝
#define COLORTBL_TAB_NOR_GDSTT	13			// 탭 비활성 헤더 그라데이션 시작
#define COLORTBL_TAB_NOR_GDEND	67			// 탭 비활성 헤더 그라데이션 끝

#define WMU_GET_THEME (WM_USER + 520) // 현재 설정된 Theme를 얻는다
#define SKIN_BLUE		0
#define SKIN_SKY		1
#define SKIN_PURPLE		2
#define SKIN_GREEN		3
#define SKIN_GRAY		4

/////////////////////////////////////////////////////////////////////////////
// CMultiChartWnd
#define HEIGHTOFINPUTBAR	(26)
//#define HEIGHTOFINPUTBAR	(20)
//#define WIDTHOFLEFTBAR		(200)
//#define WIDTHOFSPLITTER		(4)
#define WIDTHOFSPLITTER		(3)
#define WIDTH_TOOLBAR		(32)

#define Memset(x) memset(x, 0x00, sizeof(x))

CString SetComma(CString strNumber,BOOL bShowSign=FALSE)
{
	CString szRet=_T("");
	CString szTmp = strNumber;
	szTmp.TrimLeft();
	szTmp.TrimRight();

	char	ch = NULL;
	int nLen = strlen(szTmp);
	for(int i=0;  i<nLen ; i++) {
		ch = szTmp[i];
		if(ch >= '0' && ch <= '9'||ch == '.' || ch == '-' || ch== '+') 
			szRet += ch;
	}
	szTmp = szRet;

	if(szTmp.IsEmpty())	return "" ;
	if(szTmp[0]=='+' ||szTmp[0]=='-') {
		ch = szTmp[0];
		szTmp = szTmp.Mid(1);
	}

	szTmp.MakeReverse();
	nLen = szTmp.GetLength();	
	szRet.Empty();
	BOOL bIsFloat = FALSE;
	if(szTmp.Find('.')!=-1)
	{
		bIsFloat = TRUE;
	}
	int j = 0;
	int i = 0;
 	for(i=0;   i<nLen;   i++) 
	{
		if(szTmp[i]=='.') 	bIsFloat = FALSE;
		else
		{
			if(!bIsFloat)
			{
				if(j!=0 && !(j%3))	szRet += ',';
				j++;
			}
		}
		szRet += szTmp[i];
	}
	if(ch=='-') szRet += ch;
	if(bShowSign&&ch=='+'&&szRet!="0")
	{
		szRet += ch;
	}
	szRet.MakeReverse();
	return szRet;
}
// Static CimageList ============================================================>
	short CStdDialog::m_nChartImgCnt15X15 = 0;
	short CStdDialog::m_nChartImgCnt20X20 = 0;
	short CStdDialog::m_nChartImgCnt20X20Tool = 0;
	short CStdDialog::m_nChartImgCnt46X20 = 0;
	short CStdDialog::m_nChartImgCnt50X20 = 0;
	short CStdDialog::m_nChartImgCnt28X22 = 0;
	short CStdDialog::m_nChartImgCnt17X20 = 0;
	short CStdDialog::m_nChartImgCnt43X20 = 0;
	short CStdDialog::m_nChartImgCnt58X20 = 0;
	short CStdDialog::m_nChartImgCnt46X20Gwan = 0;
	short CStdDialog::m_nChartImgCnt8X25 = 0;
	short CStdDialog::m_nChartImgCnt151X21 = 0;
	short CStdDialog::m_nChartImgCnt72X20 = 0;

	CImageList CStdDialog::m_ImgList_W15H15; //폭 15에 높이 15인 버튼 이미지리스트 -> 차트 확대/종료 이미지

	CImageList CStdDialog::m_ImgList_W20H20; //폭 20에 높이 19인 버튼 이미지리스트 -> ExInputBar, InpuBar 이미지
	CImageList CStdDialog::m_ImgList_W20H20_Tool; //폭 20에 높이 19인 버튼 이미지리스트 -> ExInputBar에 십자선 이미지
	CImageList CStdDialog::m_ImgList_W46H20; //폭 46에 높이 19인 버튼 이미지리스트	-> ExInputBar에 이미지
	CImageList CStdDialog::m_ImgList_W50H20; //폭 50에 높이 19인 버튼 이미지리스트 -> 해외지수 버튼, 0930 재조회 버튼

	CImageList CStdDialog::m_ImgList_W151H21; //폭 151에 높이 21인 버튼 이미지리스트 -> OutlookBar 이미지
	CImageList CStdDialog::m_ImgList_W28H22; //폭 28에 높이 22인 버튼 이미지리스트 -> 분석툴 이미지
	CImageList CStdDialog::m_ImgList_W17H20; //폭 17에 높이 20인 버튼 이미지리스트 -> InputBar에 일주월.. 버튼 이미지
	CImageList CStdDialog::m_ImgList_W43H20; //폭 43에 높이 20인 버튼 이미지리스트 -> 0914/0904 MiniInputBar 사용
	CImageList CStdDialog::m_ImgList_W58H20; //폭 58에 높이 20인 버튼 이미지리스트 -> ExInputBar에 환경설정 이미지
	CImageList CStdDialog::m_ImgList_W72H20; //폭 72에 높이 20인 버튼 이미지리스트
	CImageList CStdDialog::m_ImgList_W46H20_Gwan; //폭 46에 높이 20인 버튼 이미지리스트 -> ExInputBar에 관심이미지
	CImageList CStdDialog::m_ImgList_W8H25;  //폭 8에 높이 25인 버튼 이미지리스트 -> 분석툴 확대/축소 이미지
// <==============================================================================	

extern CMultiChartApp theApp;

CStdDialog::CStdDialog(CWnd* pParent /*=NULL*/)
	: CExOCXDLL(CStdDialog::IDD, pParent),
	m_nDrdsInfoCnt(0),
	m_pPTManager(NULL)
{
	EnableAutomation();
	//{{AFX_DATA_INIT(CStdDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pLeftBarDlg = NULL;
	m_pExtInputBarDlg = NULL;
	m_pInputBarDlg = NULL;
//Han Hwa	m_pDrds = NULL;
	m_nLeftBarType = 0;
	m_pData = NULL;
	m_bSmallScreenMode = FALSE;
	m_nTempLeftBarType = 0;
//	m_pMsgBar = NULL;
	m_bEnable = TRUE;
	m_bShowExtInputBarDlg = FALSE;
	m_bShowInputBarDlg = TRUE;
	m_strMapID = "33100";
	m_nWidthOfLeftBar = 0;
	m_nSpliteID = 0;
	m_nTabID = 0;
	m_nChartType = SINGLE_CHART;
	m_nImportedMapID = 0;
	m_nWidthOfDlg = 0;
	m_nHeightOfDlg = 0;
	m_hViewInMain = NULL;
	m_lWidthOriginal = 0;
	m_lHeightOriginal = 0;
	m_bHideChart = FALSE;
	m_bInitResize = FALSE;

	m_bShowDataListWnd = FALSE; //2005. 04. 25 - 데이터 윈도우를 보일지 여부
	m_hReplayChartLib	= NULL;	//복기  Lib Handle

//	m_nMarketType   = CMasterTypeInfo::STOCK_CHART;
	m_nMarketType   = CMasterTypeInfo::NONE_CHART;

	m_bActiveDataWindow = false;
	m_pDataWindow	= NULL;
	m_pWndReplayLib = NULL; //복기 관련 Wnd Pointer

	memset(&m_ChartCfgInfo, 0x00, sizeof(CHART_CONFIG_INFO));
	
	m_bNeedRqstTRForReplay	= FALSE;
	m_bReplayMode			= FALSE;

	m_strConfig = "";
	m_strRootPath = "";

	m_TRComm = NULL;

	m_bSystemChart = FALSE;
	m_bSimpleChart = FALSE;
	m_bMultiItemChart = FALSE;
	m_hCursor = NULL;
	m_cUM = 'E';
	m_strDollar = "";
	m_pDlgToolbar = NULL;
	m_bOneClickHide = FALSE;
	m_bShowToolBar = FALSE;

//	m_nMarketTrCode = atoi(QUERY_STRMARKET);

	//20061212 정유진 수정
	//grid column 확장 btn 추가
	m_nExtendState = 0;
	//20061212 수정 end
	//20070131 by 정유진
	//{시장지표 다음조회 오류 수정
	m_cMUM = 'P';
	//} 

	//20070226 by LYH
	//{장전 데이터 처리
	m_bPreMarket = FALSE;
	//}}
	//{{틱데이터 없을 경우 클리어 문제 처리
	m_cUT = '1';
	//}}

	m_strRecvCode = "";
	//2007.05.02 by LYH 서버 통계 위해 서비스 ID 추가
	m_strSvcID = "CP986100";

	m_pDrdsLib = NULL;
	// 돋보기 구현 - ojtaso (20080519)
	m_pDlgGlass = NULL;

	g_pDebugMng = NULL;
	g_pPtUtilMng = NULL;

	::memset(&m_stTrInfo2, 0x00, sizeof(m_stTrInfo2));

	m_bChartPopupMenu = FALSE;
	m_nChartImageListFlag = 0;
	m_bIsWaitRequest = FALSE;
	m_bMinimizedStart = FALSE;

	m_pGwanMng = NULL;
	m_bNotifyChangeCode = FALSE;	//TRUE;

	m_nMatrix = 0;
	m_bMetrixScreen = FALSE;
	m_bInitForm = FALSE;

	m_hTabCodeSearch = NULL;

// vntsorl:[0001250] [5001] 종합차트 - 일주월분초틱 주기별로 지표 설정 및 설정값을 다르게 할 수 있도록 개선요청
	m_bUseCycleSave = FALSE;	// ADD PSH 20101117 - 20100730 강부수 주기별 저장 여부 추가 >>
}


void CStdDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStdDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_BTN_EXT_TOOL, m_btnExtTool);
	DDX_Control(pDX, IDC_SIDEBAR, m_btnSideBar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStdDialog, CDialog)
	//{{AFX_MSG_MAP(CStdDialog)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_EXT_TOOL, OnBtnExtTool)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_SPLITTER_MOVED, OnWmSplitterMoved )	
	ON_REGISTERED_MESSAGE(RMS_ANALYSIS_FUNC		, OnByPass_ExportToolOption ) //2005. 07. 28 added by sy,nam	
	ON_REGISTERED_MESSAGE(RMSG_REPLAY_REAL		, OnReplayDrds )
	ON_REGISTERED_MESSAGE(RMSG_REPLAY_FAIL		, OnReplayFail )
	ON_REGISTERED_MESSAGE(RMSG_REPLAY_CHANGE_DATE	, OnReplayChangeDate)
	ON_REGISTERED_MESSAGE(RMSG_REPLAY_END			, OnReplayEnd)//복기 정지됨.
	ON_REGISTERED_MESSAGE(RMSG_REPLAY_TOTAL_SEC		, OnReplaySetTotalSec)	//06.01.10
	ON_REGISTERED_MESSAGE(RMSG_REPLAY_INC_SEC		, OnReplayEachSec)		//06.01.10
	//-----------------------------------------------------------------------------
	// Author		: {{{ 오정택 }}} ojtaso 	Date : 2005/2/23
	// Reason		: Added to drag and drop operation.
	//-----------------------------------------------------------------------------
	// BEGIN
	ON_REGISTERED_MESSAGE(RMSG_DRAGSTART, OnDragStart)
	ON_REGISTERED_MESSAGE(RMSG_DRAGEND, OnDragEnd)
	// END
	ON_REGISTERED_MESSAGE(RMSG_CHANGE_SKINCOLOR, OnChangeSkinColor)
	ON_REGISTERED_MESSAGE(RMSG_GET_SKINCOLOR, OnGetSkinColor)
//	ON_REGISTERED_MESSAGE(RMSG_ADDPOPUPMENUITEM, OnAddPopupMenuItem)
//	ON_REGISTERED_MESSAGE(RMSG_ACTIONPOPUPMENUITEM, OnActionPopupMenuItem)
	//ON_REGISTERED_MESSAGE(RMSG_CHANGEACCTNUM, OnChangeAccount)
	//ON_REGISTERED_MESSAGE(RMSG_SET_PASSWORDINFO, OnVerifyPassword)
//	ON_REGISTERED_MESSAGE(RMSG_FORMLOADSAVE, OnFormLoadSave)
	ON_REGISTERED_MESSAGE(RMSG_USERINFO, OnUserInfo)
	ON_REGISTERED_MESSAGE(RMSG_GET_TRCOMM, OnGetTRComm)
	ON_REGISTERED_MESSAGE(RMSG_DRUSER, OnRmsgDRUser)
END_MESSAGE_MAP()
//@Solomon090819 ON_MESSAGE(WM_BC_DATA_RECEIVED, OnGetBroad)

BEGIN_DISPATCH_MAP(CStdDialog, CExOCXDLL)
	//{{AFX_DISPATCH_MAP(CStdDialog)
	DISP_FUNCTION(CStdDialog, "SetOpenMapData", SetOpenMapData, VT_EMPTY, VTS_BSTR VTS_I2)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IStdDialog to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {506C0A8C-CBC2-4d27-BD67-D4531AB6F5A8}
static const IID IID_IStdDialog =
{ 0x506c0a8c, 0xcbc2, 0x4d27, { 0xbd, 0x67, 0xd4, 0x53, 0x1a, 0xb6, 0xf5, 0xa8 } };

BEGIN_INTERFACE_MAP(CStdDialog, CExOCXDLL)
	INTERFACE_PART(CStdDialog, IID_IStdDialog, Dispatch)
END_INTERFACE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CStdDialog message handlers
//-----------------------------------------------------------------------------
// Author		: ojtaso 	Date : 2005/11/18
// Return		: void
// Paramter
//		LPCTSTR lpSharedName : 공유키
//		LPCTSTR lpGSharedData : 공유값
//		BOOL bWithStart : Start공유 여부
// Comments		: 공유값이 변경되면 호출되는 함수
//				  공유키에 따라 사용할 공유값을 세팅한다.
//				  필요에 따라 CExOCXDLL::OnGSharedDataChange 호출로 대신할 수 있다.
//-----------------------------------------------------------------------------
void CStdDialog::OnGSharedDataChange(LPCTSTR lpSharedName, LPCTSTR lpGSharedData, BOOL bWithStart)
{
	CExOCXDLL::OnGSharedDataChange(lpSharedName, lpGSharedData, bWithStart);
	CString strSharedName = lpSharedName;
	CString strGSharedData = lpGSharedData;
	strGSharedData.TrimRight();
	strSharedName.TrimRight(';');
	if(strGSharedData.IsEmpty()) return;
	if(strSharedName=="시간-그리드")
	{
		m_ChartMng.SetTimeMark(lpGSharedData, TRUE);
		return;
	}
}

//-----------------------------------------------------------------------------
// Author		: ojtaso 	Date : 2005/11/18
// Return		: void
// Paramter
//		long dwMainDllObj : MainDll 인터페이스 (지금은 사용하지 않는다.)
//		long dwBaseDesk : CBaseDesk2 포인터 (데스크의 정보를 담고 있다.)
//		long dwRscMng : 리소스매니저 포인터 (지금은 사용하지 않는다.)
//		CStringList* pAllProperties : DLL.ocx의 모든 속성을 담고 있는 포인터 (문자열 파싱으로 초기정보를 세팅한다.)
// Comments		: CStdDialog에 필요한 모든 정보를 초기화 한다.
//				  실제 초기화 작업은 CExOCXDLL에서 하고 그외 다른 작업이 필요하다면 여기서 구현한다.
//				  반드시 CExOCXDLL::Platform_Initialize를 호출하여야 한다.
//-----------------------------------------------------------------------------
void CStdDialog::Platform_Initialize(long dwMainDllObj, long dwBaseDesk, long dwRscMng, CStringList* pAllProperties)
{
	CExOCXDLL::Platform_Initialize(dwMainDllObj, dwBaseDesk, dwRscMng, pAllProperties);
}

//-----------------------------------------------------------------------------
// Author		: ojtaso 	Date : 2005/11/18
// Return		: 메세지 처리 여부
// Paramter
//		UINT message : DLL.ocx에서 받은 윈도우메세지
//		WPARAM wParam : WPARAM
//		LPARAM lParam : LPARAM
//		int &nContinue : nContinue 0 : 메시지 처리함. 리턴된 값으로 DLL.ocx에서에서 리턴하기 원함.
//								   1 : 처리할 메시지 없음
//								   2 : 메시지 처리함. 리턴값 의미 없음. DLL.ocx에서에서 메시지 계속처리.
// Comments		: CWnd::WindowProc의 호출이외에 DLL.ocx에서 처리하는 메세지를 받는다.
//				  반드시 CExOCXDLL::Platform_WindowProc 호출하여야 한다.
//-----------------------------------------------------------------------------
LRESULT CStdDialog::Platform_WindowProc(UINT message, WPARAM wParam, LPARAM lParam, int &nContinue)
{	
	LRESULT lResult = CExOCXDLL::Platform_WindowProc(message, wParam, lParam, nContinue);

	return lResult;
}

//-----------------------------------------------------------------------------
// Author		: ojtaso 	Date : 2005/11/18
// Return		: 메세지 처리 여부
// Paramter
//		MSG* pMsg : 메세지 구조체
// Comments		: CWnd::PreTranslateMessage의 호출이외에 DLL.ocx에서 처리하는 메세지를 받는다.
//				  반드시 CExOCXDLL::Platform_PreTranslateMessage 호출하여야 한다.
//-----------------------------------------------------------------------------
LRESULT CStdDialog::Platform_PreTranslateMessage(MSG* pMsg)
{
	LRESULT lResult = CExOCXDLL::Platform_PreTranslateMessage(pMsg);

	return 0L;
}

//-----------------------------------------------------------------------------
// Author		: ojtaso 	Date : 2005/11/18
// Return		: 메세지 처리 여부
// Paramter
//		UINT nType : 크기변경 형태
//		int cx : 
//		int cy :
// Comments		: DLL.ocx에서 OnSize 메세지 처리후 호출한다.
//				  CStdDialog의 화면크리 처리를 여기서 구현한다.
//				  필요에 따라 CExOCXDLL::Platform_OnSize 호출로 대신할 수 있다.
//-----------------------------------------------------------------------------
void CStdDialog::Platform_OnSize(UINT nType, int cx, int cy)
{
	CExOCXDLL::Platform_OnSize(nType, cx, cy);
}

BOOL CStdDialog::OnInitDialog() 
{
	AfxEnableControlContainer();	//solomon-090825

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	CreateLogFile();
#endif

#ifdef _WRITELOG			// Log - ojtaso (20080107)
	WriteToStringLog(_T("OnInitDialog::Begin"), _T("CStdDialog"), 10);
#endif			
	CExOCXDLL::OnInitDialog();

	// User ID 및 폴더경로 초기화
	SetInitInfo();

	if(m_nHeightOfDlg && m_nWidthOfDlg)
	{
		SetWindowPos(&CWnd::wndTop, 0, 0, m_nWidthOfDlg, m_nHeightOfDlg, SWP_SHOWWINDOW);			
	}

	// PCTR 의 기본 Interface 셋팅
	SetBaseInterface();
	{
		IPartnerSvrManager* pPtSvrMng = (IPartnerSvrManager*)AfxGetPctrInterface(UUID_IPartnerSvrManager);
		if(pPtSvrMng)
		{
			g_pPtUtilMng = (IPartnerUtilManager*)pPtSvrMng->GetInerInterface(LUID_IPartnerUtilManager);
		}
	}
	if(m_nScreenID == atoi("7111"))	//6001 종합
	{
		m_nMarketType =  CMasterTypeInfo::STOCK_CHART;
		m_bSmallScreenMode = FALSE;
		m_nChartType = MULTI_CHART;
		m_bMultiItemChart = TRUE;
		m_nChartImageListFlag = IMGLIST_15X15 | IMGLIST_20X20 | IMGLIST_20X20TOOL | IMGLIST_46X20 | IMGLIST_28X22 | IMGLIST_17X20 | IMGLIST_58X20 | IMGLIST_46X20GWN | IMGLIST_8X25 | IMGLIST_151X21 | IMGLIST_72X20 | IMGLIST_43X20;
		g_pPtUtilMng->DoWinixVariant(m_hPlatform,  titleCC, "종합차트");
	}
	else if(m_nScreenID == atoi("7125"))	//6301 일주월 
	{
		m_nMarketType =  CMasterTypeInfo::STOCK_CHART;
		m_bSmallScreenMode = TRUE;
		m_nChartType = MINI_SINGLE_CHART;
		m_bSimpleChart = TRUE;
		m_nChartImageListFlag = IMGLIST_15X15 | IMGLIST_20X20 | IMGLIST_28X22 | IMGLIST_17X20 | IMGLIST_43X20 | IMGLIST_8X25;
		g_pPtUtilMng->DoWinixVariant(m_hPlatform,  titleCC, "일주월차트");
	}
	else if(m_nScreenID == atoi("7117"))	//6201 미니차트 
	{
		m_nMarketType =  CMasterTypeInfo::STOCK_CHART;
		m_bSmallScreenMode = TRUE;
		m_nChartType = MINI_SINGLE2_CHART;
		m_bSimpleChart = TRUE;
		m_nChartImageListFlag = IMGLIST_20X20 | IMGLIST_28X22 | IMGLIST_17X20 | IMGLIST_8X25 | IMGLIST_46X20;

		g_pPtUtilMng->DoWinixVariant(m_hPlatform,  titleCC, "주식미니차트");
	}
	else if(m_nScreenID == atoi("7118"))	//6202 멀티미니차트 
	{
		m_nMarketType =  CMasterTypeInfo::STOCK_CHART;
		m_bSmallScreenMode = TRUE;
		m_nChartType = MINI_MULTI_CHART;
		m_nChartImageListFlag = IMGLIST_20X20 | IMGLIST_28X22 | IMGLIST_17X20 | IMGLIST_8X25 | IMGLIST_46X20;

		g_pPtUtilMng->DoWinixVariant(m_hPlatform,  titleCC, "멀티미니차트");
	}
// 	else if(m_nScreenID == )		//6203
// 	{
// 		m_nMarketType =  STOCK_CHART;
// 		m_bSmallScreenMode = TRUE;
// 		m_nChartType = MINI_SINGLE_CHART;
// 		m_bSimpleChart = TRUE;
// 		m_nChartImageListFlag = IMGLIST_20X20 | IMGLIST_28X22 | IMGLIST_17X20 | IMGLIST_8X25 | IMGLIST_46X20;
// 	}
	else if(m_nScreenID == atoi("7119"))	//6204미니틱차트 
	{
		m_nMarketType =  CMasterTypeInfo::STOCK_CHART;
		m_bSmallScreenMode = TRUE;
		m_nChartType = MINI_SINGLE_CHART;
		m_bSimpleChart = TRUE;
		m_nChartImageListFlag = IMGLIST_20X20 | IMGLIST_28X22 | IMGLIST_17X20 | IMGLIST_43X20 | IMGLIST_8X25 | IMGLIST_46X20;
		g_pPtUtilMng->DoWinixVariant(m_hPlatform,  titleCC, "미니틱차트");
	}
	else if(m_nScreenID == atoi("7120"))	//6205 미니업종차트 
	{
		m_nMarketType =  CMasterTypeInfo::UPJONG_CHART;
		m_bSmallScreenMode = TRUE;
		m_nChartType = MINI_SINGLE_CHART;
		m_bSimpleChart = TRUE;
		m_nChartImageListFlag = IMGLIST_20X20 | IMGLIST_28X22 | IMGLIST_17X20 | IMGLIST_8X25 | IMGLIST_46X20;
		g_pPtUtilMng->DoWinixVariant(m_hPlatform,  titleCC, "미니업종차트");
	}
	else if(m_nScreenID == atoi("7121"))	//6206 미니선옵차트 
	{
		m_nMarketType =  CMasterTypeInfo::FUTOPT_CHART;
		m_bSmallScreenMode = TRUE;
		m_nChartType = MINI_SINGLE_CHART;
		m_bSimpleChart = TRUE;
		m_nChartImageListFlag = IMGLIST_20X20 | IMGLIST_28X22 | IMGLIST_17X20 | IMGLIST_8X25 | IMGLIST_46X20;
		g_pPtUtilMng->DoWinixVariant(m_hPlatform,  titleCC, "미니선옵차트");
	}
// 0424는 CompareChart에서 처리하는 부분임.
// 	else if(m_nScreenID == atoi("0424"))	//선물1분차트
// 	{
// 		m_nMarketType =  CMasterTypeInfo::FUTOPT_CHART;
// 		m_bSmallScreenMode = TRUE;
// 		m_nChartType = MINI_SINGLE_CHART;
// 		m_bSimpleChart = TRUE;
// 		m_nChartImageListFlag = IMGLIST_20X20 | IMGLIST_28X22 | IMGLIST_17X20 | IMGLIST_8X25 | IMGLIST_46X20;
// 	}

	else if(m_nScreenID == atoi("0508"))	//nScreenID_6401 선물전일대비차트 : IBK 안씀
	{
		m_nMarketType =  CMasterTypeInfo::FUTOPT_CHART;
		m_bSmallScreenMode = TRUE;
		m_nChartType = MINI_SINGLE_CHART;
		m_bSimpleChart = TRUE;
		m_nChartImageListFlag = IMGLIST_20X20 | IMGLIST_50X20 | IMGLIST_28X22 | IMGLIST_17X20 | IMGLIST_8X25;
	}
	else if(m_nScreenID == atoi("7126"))	//6701 시스템트레이딩차트 
	{
		m_nMarketType =  CMasterTypeInfo::STOCK_CHART;
		m_bSmallScreenMode = FALSE;
		m_nChartType = MULTI_CHART;
		m_bMultiItemChart = TRUE;
		m_bSystemChart = TRUE;
		m_nChartImageListFlag = IMGLIST_15X15 | IMGLIST_20X20 | IMGLIST_20X20TOOL | IMGLIST_46X20 | IMGLIST_28X22 | IMGLIST_17X20 | IMGLIST_58X20 | IMGLIST_46X20GWN | IMGLIST_8X25 | IMGLIST_151X21 | IMGLIST_72X20 | IMGLIST_43X20;
		g_pPtUtilMng->DoWinixVariant(m_hPlatform,  titleCC, "전략차트");
	}
	else if(m_nScreenID == atoi("7122"))	// 상품선물 미니차트 
	{
		m_nMarketType =  CMasterTypeInfo::COMMODITY_CHART;
		m_bSmallScreenMode = TRUE;
		m_nChartType = MINI_SINGLE_CHART;
		m_bSimpleChart = TRUE;
		m_nChartImageListFlag = IMGLIST_20X20 | IMGLIST_28X22 | IMGLIST_17X20 | IMGLIST_8X25 | IMGLIST_46X20;
		g_pPtUtilMng->DoWinixVariant(m_hPlatform,  titleCC, "상품선물미니차트");
	}
	else
	{
		if(m_nScreenID == atoi("7112"))		//주식종합차트 
		{
			m_nChartType = SINGLE_CHART;
			m_nMarketType = CMasterTypeInfo::STOCK_CHART;
			m_nChartImageListFlag = IMGLIST_15X15 | IMGLIST_20X20 | IMGLIST_28X22 | IMGLIST_17X20 | IMGLIST_8X25 | IMGLIST_151X21 | IMGLIST_72X20 | IMGLIST_43X20| IMGLIST_46X20 | IMGLIST_46X20GWN; // 0023618: [7112] 주식종합차트 화면 - 관심종목 등록 버튼 및 관련 기능 추가 요청
			g_pPtUtilMng->DoWinixVariant(m_hPlatform,  titleCC, "주식종합차트");
		}
		else if(m_nScreenID == atoi("7113"))	//업종종합차트
		{
		
			m_nChartType = SINGLE_CHART;
			m_nMarketType = CMasterTypeInfo::UPJONG_CHART;
			m_nChartImageListFlag = IMGLIST_15X15 | IMGLIST_20X20 | IMGLIST_28X22 | IMGLIST_17X20 | IMGLIST_8X25 | IMGLIST_151X21 | IMGLIST_72X20 | IMGLIST_43X20;
			g_pPtUtilMng->DoWinixVariant(m_hPlatform,  titleCC, "업종종합차트");
		}
		//else if(m_nScreenID == atoi("0503"))	//6004
		else if(m_nScreenID == gSTOCKFUTOPT_0503)	//선옵종합차트 
		{
			m_nChartType = SINGLE_CHART;
			m_nMarketType = CMasterTypeInfo::FUTOPT_CHART;
			m_nChartImageListFlag = IMGLIST_15X15 | IMGLIST_20X20 | IMGLIST_28X22 | IMGLIST_17X20 | IMGLIST_8X25 | IMGLIST_151X21 | IMGLIST_72X20 | IMGLIST_43X20| IMGLIST_46X20;
			g_pPtUtilMng->DoWinixVariant(m_hPlatform,  titleCC, "선옵종합차트");
		}
		else if(m_nScreenID == atoi("7138"))	// 주식선옵차트
		{
			m_nChartType = SINGLE_CHART;
			m_nMarketType = CMasterTypeInfo::STOCKFUTOPT_CHART;
			m_nChartImageListFlag = IMGLIST_15X15 | IMGLIST_20X20 | IMGLIST_28X22 | IMGLIST_17X20 | IMGLIST_8X25 | IMGLIST_151X21 | IMGLIST_72X20 | IMGLIST_43X20| IMGLIST_46X20;
			g_pPtUtilMng->DoWinixVariant(m_hPlatform,  titleCC, "주식선물종합차트");
		}
		else if(m_nScreenID == atoi("7116"))	//해외증시차트 
		{
			m_nChartType = SINGLE_CHART;
			m_nMarketType =  CMasterTypeInfo::FOREIGN_CHART;
			m_nChartImageListFlag = IMGLIST_15X15 | IMGLIST_20X20 | IMGLIST_50X20 | IMGLIST_28X22 | IMGLIST_17X20 | IMGLIST_8X25 | IMGLIST_151X21 | IMGLIST_72X20 | IMGLIST_43X20;
			g_pPtUtilMng->DoWinixVariant(m_hPlatform,  titleCC, "해외증시차트");
		}
		//else if(m_nScreenID == atoi("0515"))		//FO차트
// 		else if(m_nScreenID == gFUTOPTJIPYO_0515)	//FO차트 : KHD : 안씀 
// 		{
// 			m_nChartType = SINGLE_CHART;
// //			m_nMarketType = CMasterTypeInfo::FUTOPT_CHART;
// 			m_nMarketType = CMasterTypeInfo::FUOPT_JIPYO_CHART;
// 			m_nChartImageListFlag = IMGLIST_15X15 | IMGLIST_20X20 | IMGLIST_28X22 | IMGLIST_17X20 | IMGLIST_8X25 | IMGLIST_151X21 | IMGLIST_72X20 | IMGLIST_43X20;
// 		}
		//else if(m_nScreenID == atoi("0516"))	//선옵과거종목조회
// 		else if(m_nScreenID == gSTOCKFUTOPT_0516)	//선옵과거종목조회 : KHD : 안씀 
// 		{
// 			m_nChartType = SINGLE_CHART;
// 			m_nMarketType = CMasterTypeInfo::FUTOPT_CHART;
// 			m_nChartImageListFlag = IMGLIST_15X15 | IMGLIST_20X20 | IMGLIST_28X22 | IMGLIST_17X20 | IMGLIST_8X25 | IMGLIST_151X21 | IMGLIST_72X20 | IMGLIST_43X20;
// 		}
// 		else if(m_nScreenID == )	//1115
// 		{
// 			m_nChartType = SINGLE_CHART;
// 			m_nMarketType =  STOCK_CHART;
// 			m_nChartImageListFlag = IMGLIST_15X15 | IMGLIST_20X20 | IMGLIST_28X22 | IMGLIST_17X20 | IMGLIST_8X25 | IMGLIST_151X21 | IMGLIST_72X20 | IMGLIST_43X20;
// 		}
		else if(m_nScreenID == atoi("7115"))	//상품선물 종합차트
		{
			m_nChartType = SINGLE_CHART;
			m_nMarketType =  CMasterTypeInfo::COMMODITY_CHART;
			m_nChartImageListFlag = IMGLIST_15X15 | IMGLIST_20X20 | IMGLIST_50X20 | IMGLIST_28X22 | IMGLIST_17X20 | IMGLIST_8X25 | IMGLIST_151X21 | IMGLIST_72X20 | IMGLIST_43X20;
			g_pPtUtilMng->DoWinixVariant(m_hPlatform,  titleCC, "상품선물종합차트");
		}
		m_bSmallScreenMode = FALSE;
	}

	if(m_nChartImageListFlag & IMGLIST_15X15)	m_nChartImgCnt15X15++;
	if(m_nChartImageListFlag & IMGLIST_20X20)	m_nChartImgCnt20X20++;
	if(m_nChartImageListFlag & IMGLIST_20X20TOOL)	m_nChartImgCnt20X20Tool++;
	if(m_nChartImageListFlag & IMGLIST_46X20)	m_nChartImgCnt46X20++;
	if(m_nChartImageListFlag & IMGLIST_50X20)	m_nChartImgCnt50X20++;
	if(m_nChartImageListFlag & IMGLIST_28X22)	m_nChartImgCnt28X22++;
	if(m_nChartImageListFlag & IMGLIST_17X20)	m_nChartImgCnt17X20++;
	if(m_nChartImageListFlag & IMGLIST_43X20)	m_nChartImgCnt43X20++;
	if(m_nChartImageListFlag & IMGLIST_58X20)	m_nChartImgCnt58X20++;
	if(m_nChartImageListFlag & IMGLIST_46X20GWN)	m_nChartImgCnt46X20Gwan++;
	if(m_nChartImageListFlag & IMGLIST_8X25)	m_nChartImgCnt8X25++;
	if(m_nChartImageListFlag & IMGLIST_151X21)	m_nChartImgCnt151X21++;
	if(m_nChartImageListFlag & IMGLIST_72X20)	m_nChartImgCnt72X20++;

#ifdef _WRITELOG			// Log - ojtaso (20080107)
	WriteToStringLog(_T("InitImageList::Begin"), _T("CStdDialog"), 10);
#endif			
	InitImageList();
#ifdef _WRITELOG			// Log - ojtaso (20080107)
	WriteToStringLog(_T("InitImageList::End"), _T("CStdDialog"), 10);
#endif			

/*
#ifdef _WRITELOG			// Log - ojtaso (20080107)
	WriteToStringLog(_T("FormLoad::Begin"), _T("CStdDialog"), 10);
#endif			
	FormLoad();
#ifdef _WRITELOG			// Log - ojtaso (20080107)
	WriteToStringLog(_T("FormLoad::End"), _T("CStdDialog"), 10);
#endif			
*/

	g_pDebugMng = (IDebugManager*)AfxGetPctrInterface(UUID_IDebugManager);


	FormLoad();
	if( !m_bInitForm )	return FALSE;

	//@Solomon-화면로딩 후 자동조회처리.
	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
	{
		//_DoFirstRequest();	//여기서 직접 조회하면 화면이 Lock이 걸리네.
		// 그래서 Timer를 사용함.
	//	if (m_nChartType == MULTI_CHART)
	//		RequestDollarData();
		SetTimer(2,10,NULL);
	}
#ifdef _WRITELOG			// Log - ojtaso (20080107)
	WriteToStringLog(_T("OnInitDialog::End"), _T("CStdDialog"), 10);
#endif			

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStdDialog::ChangeStockMode(int type)
{
	if (m_nChartType >= MINI_MULTI_CHART)
	{
		ChangeMiniStockMode(type);
		return;
	}

	CString strTemp;
	//복기 Lib UnLoad
	if (m_pInputBarDlg->GetCurrentMode() != REPLAY_ITEM)
		UnLoad_ReplayChart_Lib();	

//@Solomon-->
//	if(m_nMarketType != type)
// 	{
// 		if(m_nChartType == MULTI_CHART || m_nChartType == MINI_MULTI_CHART)
// 			::SendMessage(m_hViewInMain, RMSG_SET_CODEINPUTMARKETTYPE, (WPARAM)type, 0);
// 	}
//@Solomon<--

	if (m_nMarketType == type)
		return;

	m_nMarketType = type;

	if (m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
	{
		m_pInputBarDlg->SetMarketType(m_nMarketType);	
		m_pInputBarDlg->ChangeMarket(m_nMarketType, FALSE, TRUE);
		m_pInputBarDlg->MoveWindow(GetRect(ID_INPUTBAR,TRUE));
	}

	if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
	{
		m_pExtInputBarDlg->SetMarketType(m_nMarketType);
		m_pExtInputBarDlg->MoveWindow(GetRect(ID_EXTINPUTBAR,TRUE));
	}
}

void CStdDialog::ChangeControlMarketType(int nType)
{
	//@100222-alzioyes. N*N일때 현재 선택된 marketType으로 셋팅알도록 수정.
	m_nMarketType = nType;

// 	AfxMessageBox("[ChangeControlMarketType]Solomon수정필요"); //@Solomon-091015
// 
// 	m_nMarketType = nType;
// 
// 	if(m_nChartType == MULTI_CHART || m_nChartType == MINI_MULTI_CHART)
// 	{
// 		if(m_ChartMng.GetJongmokLink())
// 			return;
// 
// 		::SendMessage(m_hViewInMain, RMSG_SET_CODEINPUTMARKETTYPE, (WPARAM)nType, 0);
// 	}
}

void CStdDialog::ChangeMiniStockMode(int type)
{
	m_ChartMng.SaveChart(m_strConfig);
	//===========================================================================
//	AllCodeUnAdvise(); //Real 전부 해제..
	//===========================================================================
	
//	m_ChartMng.SendMessage(UMSG_RESETOCX);
//	m_ChartMng.SendMessage(UMSG_SETMATRIX,0,0);

	//m_nMarketType =  type;

	//====================================================
	//m_strConfig = m_strUserDir + "\\" + GetScreenNo() + "_" + ENVINFOFILE;

	//100530:동일화면 다르게 저장/읽기
	CString szSaveKey;
	GetFileSaveKey(szSaveKey);

	//m_strConfig = m_strUserDir + "\\" + GetScreenNo() + "_" + ENVINFOFILE;
	m_strConfig = m_strUserDir + "\\" + szSaveKey + "_" + ENVINFOFILE;

	{
		CString s;
		s.Format("\r\n[CStdDialog::ChangeMiniStockMode] m_strConfig=%s\r\n", m_strConfig);
		OutputDebugString(s);
	}

	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd() )
	{
		//m_pInputBarDlg->m_strCfgFile = m_strConfig;

		m_pInputBarDlg->SetMarketType(m_nMarketType);	
		m_pInputBarDlg->MoveWindow(GetRect(ID_INPUTBAR,TRUE));
	}

	if(m_nMarketType != type)
	{
		ChangeControlMarketType(type);

	//------------------------------------------------------------------------------
	// 2005. 05. 16 chart 구분자를 넘긴다.
	//------------------------------------------------------------------------------	
		m_ChartMng.SetMarketType(m_nMarketType);
		if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd() )
			m_pInputBarDlg->ChangeMarket(m_nMarketType, FALSE, TRUE);

		m_nMarketType =  type;
	}
}

void CStdDialog::FormLoad()
{
// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CStdDialog::FormLoad: Begin\n", __LINE__);
// 		OutputDebugString(szDebug);
// 	}
	// 메세지바 생성
//	m_pMsgBar = new CMessageBar;
//	m_pMsgBar->Create(CMessageBar::IDD,this);

#ifdef _WRITELOG			// Log - ojtaso (20080107)
	WriteToStringLog(_T("FormLoad::Step1"), _T("CStdDialog"), 10);
#endif			
	// TR 및 DRDS 초기화
	m_TRComm = (ITrComm2*)GetTrComm();
	InitDrds();

	m_FavoriteMng.SetPath(m_strUserDir);
	
	// User Directory 생성
	// 참조파일 : IO.h, Direct.h
	if (_access(m_strUserDir, 0) == -1)
	{	
		 int retval = _mkdir(m_strUserDir);
	}  

	// 설정파일 경로
	//100530:동일화면 다르게 저장/읽기
	CString szSaveKey;
	GetFileSaveKey(szSaveKey);
	//KHD : 차트를 로딩 하는데 있어서 화면에서 로딩시엔 종목코드를 읽어온다. 그리고, 메뉴에서 로딩했을땐 화면번호를 로딩한다.
	// 메뉴로딩때 개별 저장된 파일을 로딩한다. 따라서, 화면에서도 로딩할때도 개별저장이 된걸로 로딩하도록 요청이 있어서 수정한다. 
//	m_strConfig = m_strUserDir + "\\" + GetScreenNo() + "_" + ENVINFOFILE;
	m_strConfig = m_strUserDir + "\\" + szSaveKey + "_" + ENVINFOFILE; 

	{
		CString s;
		s.Format("\r\n[CStdDialog::FormLoad] m_strConfig=%s\r\n", m_strConfig);
		OutputDebugString(s);
	}

//	OutputDebugString("KHD : Config =" + m_strConfig);
	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	
	if(pMasterDataManager!=NULL)
		pMasterDataManager->InitData(m_strUserDir);
//@신영삭제
//	// 스킨색상
//	IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager);
//	if(pKSLibMng)
//		m_clrBackground = pKSLibMng->Lib_KSLink_GetColor(1);	// 바탕화면 배경
//	else
		m_clrBackground = DEFAULT_BKGRND_COLOR;		// 기본값

#ifdef _WRITELOG			// Log - ojtaso (20080107)
	WriteToStringLog(_T("FormLoad::Step2"), _T("CStdDialog"), 10);
#endif			
	char szTemp[64];

// vntsorl:[0001250] [5001] 종합차트 - 일주월분초틱 주기별로 지표 설정 및 설정값을 다르게 할 수 있도록 개선요청
	// ADD PSH 20101117 - 20100730 강부수 주기별 저장 여부 추가 >>
	::GetPrivateProfileString("Frame", "UseCycleSave", "0", szTemp, sizeof(szTemp), m_strConfig);
	m_bUseCycleSave = atoi(szTemp);
	// END ADD

	if(m_nChartType == MULTI_CHART)
	{
		m_nWidthOfLeftBar = WIDTH_SIDEBAR_EXTEND;

		if(m_nWidthOfLeftBar)
		{
			::GetPrivateProfileString("Frame","ShowLeftBar","1",szTemp,sizeof(szTemp),m_strConfig);
			m_nLeftBarType = atoi(szTemp);
		}
		else
		{
			m_nWidthOfLeftBar = WIDTH_SIDEBAR_COLLAPSE;
		}
		::GetPrivateProfileString("Frame","ShowExtInputBarDlg","1",szTemp,sizeof(szTemp),m_strConfig);
		m_bShowExtInputBarDlg = atoi(szTemp);

		::GetPrivateProfileString("Frame","ShowToolBar", "1", szTemp, sizeof(szTemp),m_strConfig);
		m_bShowToolBar = atoi(szTemp);
	}
	else if(m_nChartType == SINGLE_CHART)
	{
		m_nWidthOfLeftBar = WIDTH_SIDEBAR_EXTEND;

		if(m_nWidthOfLeftBar)
		{
			::GetPrivateProfileString("Frame","ShowLeftBar","0",szTemp,sizeof(szTemp),m_strConfig);
			m_nLeftBarType = atoi(szTemp);
	
			::GetPrivateProfileString("Frame","ShowToolBar", "1", szTemp, sizeof(szTemp),m_strConfig);
			m_bShowToolBar = atoi(szTemp);
		}
		else
		{
			m_nWidthOfLeftBar = WIDTH_SIDEBAR_COLLAPSE;
		}
		m_bShowExtInputBarDlg = 0;
	}
	else
	{
		m_nWidthOfLeftBar = WIDTH_SIDEBAR_EXTEND;
		m_nLeftBarType = 0;
		m_bShowExtInputBarDlg = 0;

		::GetPrivateProfileString("Frame","ShowToolBar", "0", szTemp, sizeof(szTemp),m_strConfig);
		m_bShowToolBar = atoi(szTemp);
	}

#ifdef _WRITELOG			// Log - ojtaso (20080107)
	WriteToStringLog(_T("FormLoad::Step3"), _T("CStdDialog"), 10);
#endif
	switch(m_nChartType)
	{
	case MINI_MULTI_CHART:
	case MINI_SINGLE_CHART:
		{
			m_pInputBarDlg = new CMiniBarDlg;
			if(m_pInputBarDlg)
			{
				m_pInputBarDlg->SetMarketType(m_nMarketType);
				m_pInputBarDlg->SetpChartMng(&m_ChartMng);
				m_pInputBarDlg->SetBaseDesk((long)m_pBaseDesk);
				m_pInputBarDlg->Create(IDD_MINIBAR,this);
				m_ChartMng.SetpInputBarDlg(m_pInputBarDlg);
				m_pInputBarDlg->MoveWindow(GetRect(IDD_MINIBAR,TRUE));
				m_pInputBarDlg->ShowWindow(SW_SHOW);
				m_pInputBarDlg->ChangeMarket(m_nMarketType, FALSE, TRUE);

				//분석툴바 생성
				if(m_nScreenID != nScreenID_6401)
				{
					m_pDlgToolbar = new CDlgToolbar(this);
					if(m_pDlgToolbar)
					{
						CRect rectNew = GetRect(ID_TOOLBAR,TRUE);
						m_pDlgToolbar->SetChartMng(&m_ChartMng);
						m_pDlgToolbar->Create(IDD_DLG_TOOLBAR,this);
						if( m_pDlgToolbar->GetSafeHwnd() )
						{
							m_pDlgToolbar->MoveWindow(rectNew);
							m_pDlgToolbar->ShowWindow(SW_SHOW);
						
							m_btnExtTool.FP_vSetImage(&m_ImgList_W8H25, 2, 3);
							m_btnExtTool.FP_vSetToolTipText("툴바확대");
							m_btnExtTool.ShowWindow(SW_SHOW);

							rectNew.right = rectNew.left;
							rectNew.top += 3;
							rectNew.left = rectNew.right - 8;
							rectNew.bottom = rectNew.top + 20;
							m_btnExtTool.MoveWindow(rectNew);
						}

					}
				}
			}
		}
		break;
	case MINI_SINGLE2_CHART:
		{
			m_pInputBarDlg = new CMiniTickBarDlg;
			if(m_pInputBarDlg)
			{
				m_pInputBarDlg->SetMarketType(m_nMarketType);
				m_pInputBarDlg->SetpChartMng(&m_ChartMng);
				m_pInputBarDlg->SetBaseDesk((long)m_pBaseDesk);
				m_pInputBarDlg->Create(IDD_MINITICKBAR,this);
				m_ChartMng.SetpInputBarDlg(m_pInputBarDlg);
				m_pInputBarDlg->MoveWindow(GetRect(IDD_MINITICKBAR,TRUE));
				m_pInputBarDlg->ShowWindow(SW_SHOW);
				m_pInputBarDlg->ChangeMarket(m_nMarketType, FALSE, TRUE);

				//분석툴바 생성
				m_pDlgToolbar = new CDlgToolbar(this);
				if(m_pDlgToolbar)
				{
					CRect rectNew = GetRect(ID_TOOLBAR,TRUE);
					m_pDlgToolbar->SetChartMng(&m_ChartMng);
					m_pDlgToolbar->Create(IDD_DLG_TOOLBAR,this);
					if( m_pDlgToolbar->GetSafeHwnd() )
					{
						m_pDlgToolbar->MoveWindow(rectNew);
						m_pDlgToolbar->ShowWindow(SW_SHOW);

						m_btnExtTool.FP_vSetImage(&m_ImgList_W8H25, 2, 3);
						m_btnExtTool.FP_vSetToolTipText("툴바확대");
						m_btnExtTool.ShowWindow(SW_SHOW);

						rectNew.right = rectNew.left;
						rectNew.top += 3;
						rectNew.left = rectNew.right - 8;
						rectNew.bottom = rectNew.top + 20;
						m_btnExtTool.MoveWindow(rectNew);
					}
				}
			}
		}
		break;
	case MULTI_CHART:
	case SINGLE_CHART:
		{
			if( m_btnSideBar.GetSafeHwnd() && ::IsWindow(m_btnSideBar.GetSafeHwnd()) )
			{
				m_btnSideBar.SetBorderColor(RGB(157,157,161));
				m_btnSideBar.SetBackGroundColor(GetBkgrndColor(), GetBkgrndColor());
			}
			else
			{
				AfxMessageBox("시스템리소스가 부족하여 화면을 띄울 수 없습니다. 다른 화면을 종료하고 실행하여 주십시오.");
				GetParent()->PostMessage(WM_DESTROY, 0, 0);
				return;
			}
			if( !m_btnExtTool.GetSafeHwnd() || !::IsWindow(m_btnExtTool.GetSafeHwnd()) )
			{
				AfxMessageBox("시스템리소스가 부족하여 화면을 띄울 수 없습니다. 다른 화면을 종료하고 실행하여 주십시오.");
				GetParent()->PostMessage(WM_DESTROY, 0, 0);
				return;
			}
	
			// 상단바 생성
			m_pInputBarDlg = NULL;
			m_pInputBarDlg = new CInputBarDlg();
			if( !m_pInputBarDlg )
			{
				AfxMessageBox("시스템리소스가 부족하여 화면을 띄울 수 없습니다. 다른 화면을 종료하고 실행하여 주십시오.");
				GetParent()->PostMessage(WM_DESTROY, 0, 0);
				return;
			}
			else
			{
				if(m_bMultiItemChart == FALSE)
					m_pInputBarDlg->SetReplayChartMode(FALSE);
	
//@Solomon	20091228	속도 문제.
				m_pInputBarDlg->SetMarketType(m_nMarketType);
			
				m_pInputBarDlg->SetpChartMng(&m_ChartMng);
				
				m_pInputBarDlg->Create(IDD_INPUTBAR,this);
				
				if( m_pInputBarDlg->GetSafeHwnd() && ::IsWindow(m_pInputBarDlg->GetSafeHwnd()) )
				/* KHD	&& m_pInputBarDlg->m_pCodeInput->GetSafeHwnd() && ::IsWindow(m_pInputBarDlg->m_pCodeInput->GetSafeHwnd())) */	
				{
					m_ChartMng.SetpInputBarDlg(m_pInputBarDlg);
					m_pInputBarDlg->MoveWindow(GetRect(ID_INPUTBAR,TRUE));
					m_pInputBarDlg->ShowWindow(SW_SHOW);
	//@Solomon	20091228	속도 문제.
	//				m_pInputBarDlg->ChangeMarket(m_nMarketType, FALSE, TRUE);

					m_pInputBarDlg->SetAllPosition(m_nChartType);
				
				}
				else
				{
					AfxMessageBox("시스템리소스가 부족하여 화면을 띄울 수 없습니다. 다른 화면을 종료하고 실행하여 주십시오.");
					GetParent()->PostMessage(WM_DESTROY, 0, 0);
					return;
				}

				//분석툴바 생성
				m_pDlgToolbar = new CDlgToolbar(this);
				if( !m_pDlgToolbar )
				{
					AfxMessageBox("시스템리소스가 부족하여 화면을 띄울 수 없습니다. 다른 화면을 종료하고 실행하여 주십시오.");
					GetParent()->PostMessage(WM_DESTROY, 0, 0);
					return;
				}
				else
				{
					CRect rectNew = GetRect(ID_TOOLBAR,TRUE);
					m_pDlgToolbar->SetChartMng(&m_ChartMng);
					m_pDlgToolbar->Create(IDD_DLG_TOOLBAR,this);
					if( m_pDlgToolbar->GetSafeHwnd() && ::IsWindow(m_pDlgToolbar->GetSafeHwnd()) )
					{
						m_pDlgToolbar->MoveWindow(rectNew);
						m_pDlgToolbar->ShowWindow(SW_SHOW);

						m_btnExtTool.FP_vSetImage(&m_ImgList_W8H25, 2, 3);
						m_btnExtTool.FP_vSetToolTipText("툴바축소");
						m_btnExtTool.ShowWindow(SW_SHOW);

						rectNew.right = rectNew.left;
						rectNew.top += 3;
						rectNew.left = rectNew.right - 8;
						rectNew.bottom = rectNew.top + 20;
						m_btnExtTool.MoveWindow(rectNew);
					}
					else
					{
						AfxMessageBox("시스템리소스가 부족하여 화면을 띄울 수 없습니다. 다른 화면을 종료하고 실행하여 주십시오.");
						GetParent()->PostMessage(WM_DESTROY, 0, 0);
						return;
					}

				}
			}
			// 사이드바 영역 생성
			
			// 좌측바 생성
			m_pLeftBarDlg = NULL;
			m_pLeftBarDlg = new CLeftBarDlg();
			if( !m_pLeftBarDlg )
			{
				AfxMessageBox("시스템리소스가 부족하여 화면을 띄울 수 없습니다. 다른 화면을 종료하고 실행하여 주십시오.");
				GetParent()->PostMessage(WM_DESTROY, 0, 0);
				return;
			}
			else
			{
				m_pLeftBarDlg->SetMarketType(m_nMarketType);				
				m_pLeftBarDlg->SetStdDialog(this);
				m_pLeftBarDlg->SetpChartMng(&m_ChartMng);
				m_pLeftBarDlg->SetpFavoriteThingsMng(&m_FavoriteMng);
				m_pLeftBarDlg->Create(IDD_LEFTBAR,this);
				if( m_pLeftBarDlg->GetSafeHwnd() && ::IsWindow(m_pLeftBarDlg->GetSafeHwnd()) )
				{
					m_btnSideBar.SetSideBar(m_pLeftBarDlg);
					m_pLeftBarDlg->MoveWindow(GetRect(ID_LEFTBAR,TRUE));
					m_pLeftBarDlg->ShowWindow(SW_SHOW);
					m_ChartMng.SetpLeftBarDlg(m_pLeftBarDlg);
				}
				else
				{
					AfxMessageBox("시스템리소스가 부족하여 화면을 띄울 수 없습니다. 다른 화면을 종료하고 실행하여 주십시오.");
					GetParent()->PostMessage(WM_DESTROY, 0, 0);
					return;
				}

			}
		}

		if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
			GetIndicatorNameListIntoComboBox(m_pInputBarDlg->m_cmbChartType, "특수지표");
	}

#ifdef _WRITELOG			// Log - ojtaso (20080107)
	WriteToStringLog(_T("FormLoad::Step4"), _T("CStdDialog"), 10);
#endif			
	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		SetPermanentHandle(m_pInputBarDlg);
	if( m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd())
		SetPermanentHandle(m_pLeftBarDlg);

#ifdef _WRITELOG			// Log - ojtaso (20080107)
	WriteToStringLog(_T("FormLoad::Step5"), _T("CStdDialog"), 10);
#endif			
	// 보조상단바 생성
	m_pExtInputBarDlg = NULL;
	m_pExtInputBarDlg = new CExtInputBarDlg;
	if( !m_pExtInputBarDlg )
	{
		AfxMessageBox("시스템리소스가 부족하여 화면을 띄울 수 없습니다. 다른 화면을 종료하고 실행하여 주십시오.");
		GetParent()->PostMessage(WM_DESTROY, 0, 0);
		return;
	}
	else
	{
		m_pExtInputBarDlg->m_nMarketType = m_nMarketType;				

		m_pExtInputBarDlg->SetpChartMng(&m_ChartMng);
		m_pExtInputBarDlg->SetpLeftBar(m_pLeftBarDlg);
		m_pExtInputBarDlg->Create(IDD_EXTINPUTBAR,this);
		if( m_pExtInputBarDlg->GetSafeHwnd() && ::IsWindow(m_pExtInputBarDlg->GetSafeHwnd()) )
		{
			m_ChartMng.SetpExtInputBarDlg(m_pExtInputBarDlg);	
			m_pExtInputBarDlg->SetMarketType(m_nMarketType);
			m_pExtInputBarDlg->MoveWindow(GetRect(ID_EXTINPUTBAR,TRUE));
			m_pExtInputBarDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			AfxMessageBox("시스템리소스가 부족하여 화면을 띄울 수 없습니다. 다른 화면을 종료하고 실행하여 주십시오.");
			GetParent()->PostMessage(WM_DESTROY, 0, 0);
			return;
		}

		if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		{
			char cType = m_pInputBarDlg->GetType();
			if(cType == DAY_DATA_CHART || 
				cType == WEEK_DATA_CHART || 
				cType == MONTH_DATA_CHART)
				EnableQueryOpt(FALSE);
			
			if(cType == TICK_DATA_CHART || 
				cType == SEC_DATA_CHART || 
				cType == WEEK_DATA_CHART || 
				cType == MONTH_DATA_CHART)
				EnableAccount(FALSE);
		}
	}

#ifdef _WRITELOG			// Log - ojtaso (20080107)
	WriteToStringLog(_T("FormLoad::Step6"), _T("CStdDialog"), 10);
#endif			
	// 스플릿바 생성
	m_xSplitter1.CreateInWnd(this, ID_SPLITTER, GetRect(ID_SPLITTER));
	if( !m_xSplitter1.GetSafeHwnd() || !::IsWindow(m_xSplitter1.GetSafeHwnd()) )
	{
		AfxMessageBox("000");
		return;
	}

#ifdef _WRITELOG			// Log - ojtaso (20080107)
	WriteToStringLog(_T("FormLoad::Step7"), _T("CStdDialog"), 10);
#endif			
	// 차트매니저 생성
	m_ChartMng.SetMarketType(m_nMarketType);
	BOOL bRetValue = m_ChartMng.Create(GetRect(ID_CHART),this, m_strUserDir, m_szImageDir,ID_CHART, (long)((CMChartInterface*)this));
	if(!bRetValue) 
	{
		AfxMessageBox("시스템리소스가 부족하여 화면을 띄울 수 없습니다. 다른 화면을 종료하고 실행하여 주십시오.");
		GetParent()->PostMessage(WM_DESTROY, 0, 0);
		return;
	}
	m_ChartMng.m_strRootPath = m_strRootPath;
	m_ChartMng.SetBaseDesk((long)1);            // 데스크가 없어짐으로 양의 정수를 셋팅함으로 기본동작 가능

	if((m_nChartType < MINI_MULTI_CHART) && m_pLeftBarDlg)
		m_pLeftBarDlg->SetConfig();

	if( m_ChartMng.GetpChartItemSelected() != NULL)
		m_ChartMng.GetpChartItemSelected()->LoadTickMinDayInfo(m_strConfig);
#ifdef _DEBUG
	else
	{
		AfxMessageBox("FormLoad : m_ChartMng.GetpChartItemSelected() is NULL !!");
		return FALSE;
	}
#endif 

	// (보조)상단바 타입 및 Visible 셋팅
	if(m_nChartType == MULTI_CHART || m_nChartType == SINGLE_CHART)
		SetLeftBarType(m_nLeftBarType);

	SetShowExtInputBarDlg(m_bShowExtInputBarDlg);

	BOOL bRequestAPData = TRUE;

#ifdef _WRITELOG			// Log - ojtaso (20080107)
	WriteToStringLog(_T("FormLoad::Step8"), _T("CStdDialog"), 10);
#endif			
	// 종목코드 컨트롤 생성
	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
	{
		m_pInputBarDlg->InitBaseInfo(NULL, bRequestAPData);
	}

// 	// 스킨색상
	int nTheme = 0;//AfxGetMainWnd()->SendMessage(WMU_GET_THEME);	
	OnChangeSkinColor(nTheme, 0);

#ifdef _WRITELOG			// Log - ojtaso (20080107)
	WriteToStringLog(_T("FormLoad::Step9"), _T("CStdDialog"), 10);
#endif			
	Invalidate();

#ifdef _WRITELOG			// Log - ojtaso (20080107)
	WriteToStringLog(_T("FormLoad::Step10"), _T("CStdDialog"), 10);
#endif			
	//달러/원 데이터 요청
//@Solomon 모래시계.
//	if (m_nChartType == MULTI_CHART)		RequestDollarData();
	
/*
	if(m_pInputBarDlg)
	{
		STINPUTCODEDATA stInputCodeData;
		stInputCodeData.m_strCode = m_pInputBarDlg->GetCurrentCode();
		stInputCodeData.m_strTime = "";
		stInputCodeData.m_nDefault = MODE_DEFAULT;
		stInputCodeData.m_nMode = CHANGE_ITEM;
		m_ChartMng.SendMessage(UMSG_INPUTCODETOCHARTMNG, MODE_DEFAULT, (LPARAM)&stInputCodeData);
	}
*/
	m_bInitForm = TRUE;

// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CStdDialog::FormLoad: End\n", __LINE__);
// 		OutputDebugString(szDebug);
// 	}

	//ClearSaveKey();	
}

LONG CStdDialog::OnWmSplitterMoved( UINT wParam, LONG lParam )
{

	if(ID_SPLITTER == lParam)
	{		
		if(m_nLeftBarType == 1)		
		{
			m_nWidthOfLeftBar += wParam;
		}
		else if(m_nLeftBarType == 2)	
		{
			m_nWidthOfLeftBar -= wParam;
		}
	}

	if(	(m_bShowDataListWnd				) &&
		(m_pDataWindow != NULL			) &&
		(m_pDataWindow->GetSafeHwnd()	) )
	{
		if(m_ChartMng.GetSafeHwnd())
		{	
			CRect rect;
			m_ChartMng.GetWindowRect(rect);	
			ScreenToClient(rect);
			m_ViewRect.left = rect.left;
			m_pDataWindow->MoveWindow(m_ViewRect);	
		}
	}

	if( m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd() )
		m_pLeftBarDlg->Invalidate();
	m_ChartMng.Invalidate();
	m_xSplitter1.Invalidate();
	//m_pInputBarDlg->Invalidate();
	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		m_pInputBarDlg->ShowWindow(SW_HIDE);
	if( m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd() )	
		m_pExtInputBarDlg->Invalidate();
	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		m_pInputBarDlg->ShowWindow(SW_SHOW);


	if(m_ChartMng.m_pFloatingDlg != NULL)				
		m_ChartMng.m_pFloatingDlg->Invalidate();

//	if(m_pInputBarDlg)
//		m_pInputBarDlg->SendMessage(UMSG_EDIT_FOCUS);
	
    return 1L;
}

CRect CStdDialog::GetRect(UINT nID, BOOL bInit /*=FALSE*/)
{
	CRect rect;
	GetClientRect(rect);

	if(m_bShowToolBar)
		rect.right -= WIDTH_TOOLBAR;

	if(!m_nLeftBarType && m_nChartType < MINI_MULTI_CHART)
		rect.left = WIDTH_SIDEBAR;

	switch(nID)
	{
	case IDC_SIDEBAR:
		{
			if(!m_nLeftBarType && m_nChartType < MINI_MULTI_CHART)
			{
				rect.left = 0;
				rect.right = WIDTH_SIDEBAR;
				rect.top += 2;
// 				rect.bottom -= 2;
			}
			else
			{
				rect.SetRectEmpty();
			}
			return rect;
		}
		break;
	case ID_LEFTBAR:
		if(!m_nLeftBarType)	
		{
			rect.SetRectEmpty();		
			return rect;
		}
		if(m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd() && !bInit)
		{
			CRect rectLeftBar;
			m_pLeftBarDlg->GetWindowRect(rectLeftBar);
			ScreenToClient(rectLeftBar);
			if(m_nLeftBarType==1)		rect.right = rectLeftBar.right;
			else if(m_nLeftBarType==2)	rect.left = rect.right - rectLeftBar.Width();
		}
		else	// initial
		{
			if(m_nLeftBarType==1)		rect.right = rect.left + m_nWidthOfLeftBar;
			else if(m_nLeftBarType==2)	rect.left = rect.right - m_nWidthOfLeftBar;
		}		
		break;
	case ID_INPUTBAR:
	case ID_MINIBAR:
		if(!m_bShowInputBarDlg)	
		{
//			rect.left = WIDTH_SIDEBAR;
			rect.bottom = rect.top;
			return rect;
		}
		if(m_pInputBarDlg && m_pInputBarDlg->IsMiniBar())
		{
			rect.bottom = rect.top + HEIGHTOFINPUTBAR;
			if(m_nLeftBarType==1)		rect.left = rect.left + m_nWidthOfLeftBar + WIDTHOFSPLITTER;
			else if(m_nLeftBarType==2)	rect.right = rect.right - (m_nWidthOfLeftBar + WIDTHOFSPLITTER);			

			if(m_pDlgToolbar)
				rect.right = rect.right - 8;
		}
		else
		{
			if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd()&&!bInit)
			{
				CRect rectInputBar;
				CRect rectLeftBar;
				if(m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd())
				{
					m_pLeftBarDlg->GetWindowRect(rectLeftBar);
				}
				
				m_pInputBarDlg->GetWindowRect(rectInputBar);
				ScreenToClient(rectInputBar);
				if(m_nLeftBarType==1)		rect.left = rectInputBar.left;
				else if(m_nLeftBarType==2)	rect.right = rect.right - rectLeftBar.Width() - WIDTHOFSPLITTER;
			}
			else
			{			
				if(m_nLeftBarType==1)		rect.left = rect.left + m_nWidthOfLeftBar + WIDTHOFSPLITTER;
				else if(m_nLeftBarType==2)	rect.right = rect.right - (m_nWidthOfLeftBar + WIDTHOFSPLITTER);			
			}	
//			if(m_bShowExtInputBarDlg)
//				rect.bottom = rect.top + HEIGHTOFINPUTBAR + 1;
//			else
				rect.bottom = rect.top + HEIGHTOFINPUTBAR;

			if(m_pDlgToolbar)
				rect.right = rect.right - 8;
		}
		break;
	case ID_EXTINPUTBAR:
		if(!m_bShowExtInputBarDlg)	
		{
			rect.bottom = rect.top;
			return rect;
		}
		if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd()&&!bInit)
		{
			CRect rectInputBar;
			CRect rectLeftBar;
			if(m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd())
			{
				m_pLeftBarDlg->GetWindowRect(rectLeftBar);
			}		

			if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
			{
				m_pInputBarDlg->GetWindowRect(rectInputBar);
				ScreenToClient(rectInputBar);
				if(m_nLeftBarType==1)		rect.left = rectInputBar.left;
				else if(m_nLeftBarType==2)	rect.right = rect.right - rectLeftBar.Width() - WIDTHOFSPLITTER;
			}
		}
		else
		{			
			if(m_nLeftBarType==1)		rect.left = rect.left + m_nWidthOfLeftBar + WIDTHOFSPLITTER;
			else if(m_nLeftBarType==2)	rect.right = rect.right - (m_nWidthOfLeftBar +WIDTHOFSPLITTER);			
		}		
		rect.top = rect.top + HEIGHTOFINPUTBAR;
		rect.bottom = rect.top + HEIGHTOFINPUTBAR - 3;
		break;
	case ID_SPLITTER:
		if(!m_nLeftBarType||!m_pLeftBarDlg||!m_pLeftBarDlg->GetSafeHwnd())	
		{
			rect.SetRectEmpty();		
			return rect;
		}
		if(m_xSplitter1.GetSafeHwnd()&&!bInit)
		{
			CRect rectSplitter1;
			CRect rectLeftBar;
			m_pLeftBarDlg->GetWindowRect(rectLeftBar);
			m_xSplitter1.GetWindowRect(rectSplitter1);
			ScreenToClient(rectSplitter1);
			if(m_nLeftBarType==1)
			{
				rect.left = rectSplitter1.left;
				rect.right = rectSplitter1.right;			
			}
			else if(m_nLeftBarType==2)
			{
				rect.right = rect.right - rectLeftBar.Width();
				rect.left = rect.right - WIDTHOFSPLITTER;							
			}
		}
		else
		{
			if(m_nLeftBarType==1)
			{
				rect.left = rect.left + m_nWidthOfLeftBar;
				rect.right = rect.left + WIDTHOFSPLITTER;
			}
			else if(m_nLeftBarType==2)
			{
				rect.right = rect.right - m_nWidthOfLeftBar;
				rect.left = rect.right - WIDTHOFSPLITTER;				
			}
		}
		break;
	case ID_CHART:
		if(m_ChartMng.GetSafeHwnd()&&!bInit)
		{
			CRect rectChart;
			CRect rectLeftBar;
			m_ChartMng.GetWindowRect(rectChart);			
			if(m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd())
			{
				m_pLeftBarDlg->GetWindowRect(rectLeftBar);
			}
			
			ScreenToClient(rectChart);
			if(m_nLeftBarType==1)		rect.left = rectChart.left;
			else if(m_nLeftBarType==2)	rect.right = rect.right - rectLeftBar.Width() - 5;

			rect.bottom -= 3;
		}
		else
		{
			if(m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd())
			{
				if(m_nLeftBarType==1)
				{
					rect.left = rect.left + (m_nWidthOfLeftBar + WIDTHOFSPLITTER);
				}
				else if(m_nLeftBarType==2)
				{
					rect.right = rect.right - (m_nWidthOfLeftBar + WIDTHOFSPLITTER);
				}			
			}

			rect.bottom -= 3;
		}				
		if(m_bShowExtInputBarDlg)
		{
			rect.top = rect.top + HEIGHTOFINPUTBAR*2 - 3;
		}
		else
		{
			if(m_bShowInputBarDlg)
			{
				if(m_pInputBarDlg && m_pInputBarDlg->IsMiniBar())
				{
					rect.top = rect.top + HEIGHTOFINPUTBAR;
				}
				else
				{
					rect.top = rect.top + HEIGHTOFINPUTBAR;
				}
			}
		}

		if(m_bShowDataListWnd)
		{
			m_ViewRect.left		= 0;
			m_ViewRect.top		= 0;
			m_ViewRect.right	= 0;
			m_ViewRect.bottom	= 0;

			m_ViewRect.left		= rect.left;		//ChartMng
			m_ViewRect.right	= rect.right;	//ChartMng
			m_ViewRect.bottom	= rect.bottom;		
			m_ViewRect.top		= rect.bottom = rect.bottom - 90;
			if(m_ChartMng.GetSafeHwnd())
			{
				m_ChartMng.MoveWindow(rect);

			//	m_ChartMng.Invalidate();
			//	if(m_ChartMng.m_pFloatingDlg)
			//	m_ChartMng.m_pFloatingDlg->SendMessage(WM_LBUTTONUP, 0, 0);
			}

			if(	(m_pDataWindow != NULL			) &&
				(m_pDataWindow->GetSafeHwnd()	) 
			)
			{
				m_pDataWindow->MoveWindow(m_ViewRect);
			}
		}
		else
		{
			m_ViewRect.left		= 0;
			m_ViewRect.top		= 0;
			m_ViewRect.right	= 0;
			m_ViewRect.bottom	= 0;
			
			if(m_ChartMng.GetSafeHwnd())
			{
				CRect rectChartMng;
				m_ChartMng.GetWindowRect(rectChartMng);
				ScreenToClient(rectChartMng);
				rectChartMng.bottom = rect.bottom;
				m_ChartMng.MoveWindow(rectChartMng);

				//m_ChartMng.Invalidate();
				//if(m_ChartMng.m_pFloatingDlg)
				//	m_ChartMng.m_pFloatingDlg->SendMessage(WM_LBUTTONUP, 0, 0);
			}
		}

		
	
		break;
	case ID_TOOLBAR:
		if(m_pDlgToolbar && m_pDlgToolbar->GetSafeHwnd())
		{
			//if(m_nLeftBarType==1)
			//{
				rect.left = rect.right;
				rect.right = rect.right + WIDTH_TOOLBAR;
			//}
		}
		break;
	default:
		break;
	}
	return rect;
}


void CStdDialog::Resize()
{	
	CRect rect;	
	CRect rectNew;	
	
	if(m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd())
	{
		if(m_bHideChart)
		{
			CRect rect;
			GetClientRect(rect);
			rect.DeflateRect(BOUND_GAP,BOUND_GAP,BOUND_GAP,BOUND_GAP);
			m_pLeftBarDlg->MoveWindow(rect);
			m_nWidthOfLeftBar = rect.Width();
		}
		else
		{
			m_pLeftBarDlg->GetWindowRect(rect);
			ScreenToClient(rect);
			rectNew = GetRect(ID_LEFTBAR,m_bInitResize);
			if(rect!=rectNew)
			{
				m_pLeftBarDlg->MoveWindow(rectNew);
			}

			if(m_btnSideBar.GetSafeHwnd())
			{
				m_btnSideBar.GetWindowRect(rect);
				ScreenToClient(rect);
				rectNew = GetRect(IDC_SIDEBAR, m_bInitResize);
				if(rect!=rectNew)
				{
					m_btnSideBar.MoveWindow(rectNew);
				}
			}
		}

		//m_pLeftBarDlg->Invalidate();
	}

	if(m_xSplitter1.m_hWnd != NULL )
	{
		m_xSplitter1.GetWindowRect(rect);
		ScreenToClient(rect);
		rectNew = GetRect(ID_SPLITTER,m_bInitResize);
		if(rect!=rectNew)	
		{		
			m_xSplitter1.MoveWindow(rectNew);
			//m_xSplitter1.ShowWindow(SW_SHOW);
		}
	}

	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
	{
		m_pInputBarDlg->GetWindowRect(rect);
		ScreenToClient(rect);
		rectNew = GetRect(ID_INPUTBAR,m_bInitResize);
		if(rect!=rectNew)	
		{
			m_pInputBarDlg->MoveWindow(rectNew);
			//m_pInputBarDlg->ShowWindow(SW_SHOW);
		}

		if(m_pInputBarDlg->m_pCodeInput && m_pInputBarDlg->m_pCodeInput->GetSafeHwnd())
		{
			m_pInputBarDlg->m_pCodeInput->SetWindowPos(NULL, 
				m_pInputBarDlg->m_rectCmb.left, m_pInputBarDlg->m_rectCmb.top,
				m_pInputBarDlg->m_rectCmb.Width(), m_pInputBarDlg->m_rectCmb.Height(),
				SWP_NOZORDER);

			if(m_nChartType < MINI_MULTI_CHART)
			{
//@Solomon 				if(m_pInputBarDlg->m_hKSJongName && IsWindow(m_pInputBarDlg->m_hKSJongName))
				{
					CRect rcJN = m_pInputBarDlg->m_rectJongName;
//@Solomon 					CWnd::FromHandle(m_pInputBarDlg->m_hKSJongName)->SetWindowPos(NULL, 
//@Solomon 						rcJN.left, rcJN.top, rcJN.Width(), rcJN.Height(),
//@Solomon 						SWP_NOZORDER);
				}
			}
		}
		//m_pInputBarDlg->Invalidate();
		m_pInputBarDlg->MoveCodeInputCtrl();
	}
	
	if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
	{
		m_pExtInputBarDlg->GetWindowRect(rect);
		ScreenToClient(rect);
		rectNew = GetRect(ID_EXTINPUTBAR,m_bInitResize);
		if(rect!=rectNew)	
		{
			m_pExtInputBarDlg->MoveWindow(rectNew);
			//m_pExtInputBarDlg->ShowWindow(SW_SHOW);
		}

		m_pExtInputBarDlg->MoveAccountCtrl();
		//m_pExtInputBarDlg->Invalidate();
	}

	if(m_ChartMng.GetSafeHwnd())
	{
		m_ChartMng.GetWindowRect(rect);	
		ScreenToClient(rect);
		rectNew = GetRect(ID_CHART,m_bInitResize);
		if(rect!=rectNew)	
		{	
			m_ChartMng.MoveWindow(rectNew);
		}
	}

	if(m_pDlgToolbar && m_pDlgToolbar->GetSafeHwnd())
	{
		m_pDlgToolbar->GetWindowRect(rect);
		ScreenToClient(rect);
		rectNew = GetRect(ID_TOOLBAR,m_bInitResize);
		if(rect!=rectNew)	
		{
			m_pDlgToolbar->MoveWindow(rectNew);

			rectNew.right = rectNew.left;
			rectNew.top += 3;
			rectNew.left = rectNew.right - 8;
			rectNew.bottom = rectNew.top + 20;
			m_btnExtTool.MoveWindow(rectNew);
		}
		m_btnExtTool.Invalidate();
	}

	if(m_bOneClickHide)
	{
		SetOneClickHide(FALSE);
		if(m_pDlgToolbar && m_pDlgToolbar->GetSafeHwnd())
			m_pDlgToolbar->SetToggleButton(50066, 0);
	}
	m_bInitResize = TRUE;
}


void CStdDialog::OnDestroy() 
{
	_DoCheckMetrixScreen();

	m_pPTManager = NULL;

	CString strTemp;
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("좌측바 위치 저장::Begin", _T("CStdDialog"), 10);
#endif
	// DRDS 종료

	SaveShowInfo();
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("좌측바 위치 저장::End", _T("CStdDialog"), 10);
#endif

// vntsorl:[0001250] [5001] 종합차트 - 일주월분초틱 주기별로 지표 설정 및 설정값을 다르게 할 수 있도록 개선요청
	// ADD PSH - 20100730 강부수 주기별 저장 여부 추가 >>
	strTemp.Format("%d", m_bUseCycleSave);
	::WritePrivateProfileString("Frame","UseCycleSave", strTemp, m_strConfig);
	// END ADD

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("차트매니저 종료::Begin", _T("CStdDialog"), 10);
#endif
	if(m_ChartMng.GetSafeHwnd())
		m_ChartMng.DestroyWindow();
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("차트매니저 종료::End", _T("CStdDialog"), 10);
#endif
	
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("구분선 종료::Begin", _T("CStdDialog"), 10);
#endif
	if(m_xSplitter1.GetSafeHwnd())
	{
		m_xSplitter1.DetachAllCtrls();
		m_xSplitter1.DestroyWindow();
	}
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("구분선 종료::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("좌측툴바 종료::Begin", _T("CStdDialog"), 10);
#endif
	if(m_pLeftBarDlg)
	{
		if( m_pLeftBarDlg->GetSafeHwnd() )
			m_pLeftBarDlg->DestroyWindow();
		delete m_pLeftBarDlg;
		m_pLeftBarDlg = NULL;
	}
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("좌측툴바 종료::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("인풋바 종료::Begin", _T("CStdDialog"), 10);
#endif
	if(m_pInputBarDlg)
	{
		if( m_pInputBarDlg->GetSafeHwnd() )
			m_pInputBarDlg->DestroyWindow();
		delete m_pInputBarDlg;
		m_pInputBarDlg = NULL;
	}
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("인풋바 종료::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("인풋바2 종료::Begin", _T("CStdDialog"), 10);
#endif
	if(m_pExtInputBarDlg)
	{
		if( m_pExtInputBarDlg->GetSafeHwnd() )
			m_pExtInputBarDlg->DestroyWindow();
		delete m_pExtInputBarDlg;
		m_pExtInputBarDlg = NULL;
	}
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("인풋바2 종료::End", _T("CStdDialog"), 10);
#endif
	
	

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("메세지바 종료::Begin", _T("CStdDialog"), 10);
#endif
/*
	if(m_pMsgBar)
	{
		m_pMsgBar->DestroyWindow();
		delete m_pMsgBar;
		m_pMsgBar = NULL;
	}	
*/
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("메세지바 종료::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("복기 라이브러리 종료::Begin", _T("CStdDialog"), 10);
#endif
	//복기 Lib UnLoad
	UnLoad_ReplayChart_Lib();	
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("복기 라이브러리 종료::End", _T("CStdDialog"), 10);
#endif

	//===========================================================================
	POSITION pos = NULL;	
//	CRealChartData *pRealChart = NULL;
//	while(m_lstRegRealChart.GetHeadPosition())
//	{
//		pRealChart = NULL;		
//		pRealChart = (CRealChartData*)m_lstRegRealChart.GetAt(pos);
//		if(pRealChart)
//		{	delete pRealChart;
//			pRealChart = NULL;
//			m_lstRegRealChart.RemoveAt(pos);							
//		}		
//	}

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("DRDS 종료::Begin", _T("CStdDialog"), 10);
#endif
	AllCodeUnAdvise(); //Real 전부 해제..
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("DRDS 종료::End", _T("CStdDialog"), 10);
#endif	//===========================================================================
	

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("데이타 윈도우 종료::Begin", _T("CStdDialog"), 10);
#endif
	if(m_pDataWindow)
	{
		if( m_pDataWindow->GetSafeHwnd() )
			m_pDataWindow->DestroyWindow();
		delete m_pDataWindow;
		m_pDataWindow = NULL;
	}
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("데이타 윈도우 종료::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("툴바 종료::Begin", _T("CStdDialog"), 10);
#endif
	if(m_pDlgToolbar)
	{
		if( m_pDlgToolbar->GetSafeHwnd() )
			m_pDlgToolbar->DestroyWindow();
		delete m_pDlgToolbar;
		m_pDlgToolbar = NULL;
	}
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("툴바 종료::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("이미지리스트 종료::Begin", _T("CStdDialog"), 10);
#endif
	//ImageList들을 삭제한다. 05.09.01
	DeleteAllImageList();
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("이미지리스트 종료::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("커서 종료::Begin", _T("CStdDialog"), 10);
#endif
	if(m_hCursor)
		::DeleteObject(m_hCursor);
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("커서 종료::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	CloseLogFile();
#endif

	// 돋보기 구현 - ojtaso (20080519)
	if(m_pDlgGlass) 
	{
		delete m_pDlgGlass;
		m_pDlgGlass = NULL;
	}

	m_memoryDC.DeleteDC();
	m_bufferBitmap.DeleteObject();

	pos = m_lstSendData.GetHeadPosition();
	while(pos)
	{
		STSENDDATAQUE* pSendData = m_lstSendData.GetNext(pos);
		::free(pSendData->lpData);
		delete pSendData;
	}

	CExOCXDLL::OnDestroy();
}

CString CStdDialog::MakeChartData(LPVOID aTRData, int& nCount, int dwTRDataLen, int nMarketType, int nType, double dPrePrice, int nUnitType/*=WON_TYPE*/)
{
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("서버로부터 받은 Data를 차트 Data로 가공::Begin", m_strRecvCode, m_strRecvCode.GetLength());
#endif

	//20070226 by LYH
	//{장전 데이터 처리
//	m_bPreMarket = FALSE;
	//}}
	if(nCount <= 0)
		return "";

	char* szChartData = NULL;
	CString strChartData, strData;
	int nBufferLength = 0;

	BOOL bPercent = FALSE;
	int nRecSize = 0;
	switch (nMarketType)
	{
	case CMasterTypeInfo::STOCK_CHART:
		if (nUnitType == PERCENT_TYPE)		bPercent = TRUE;
		nRecSize = sizeof(KB_p0602_OutRec2);			break;
		break;
	case CMasterTypeInfo::UPJONG_CHART:
		nRecSize = sizeof(KB_p0603_OutRec2);			break;
	case CMasterTypeInfo::FUTOPT_CHART:
		nRecSize = sizeof(KB_p0604_OutRec2);			break;
	case CMasterTypeInfo::STOCKFUTOPT_CHART:
		nRecSize = sizeof(KB_d1013_OutRec2);			break;
	case CMasterTypeInfo::FOREIGN_CHART:
		nRecSize = sizeof(KB_p0606_OutRec2);			break;
	case CMasterTypeInfo::FUOPT_JIPYO_CHART:
		nRecSize = sizeof(KB_p0515_OutRecArr);			break;
	default:
		nRecSize = 0;									break;
	}

	if (nRecSize > 0)
	{
		char* szTRData = (char*)aTRData + (nRecSize * (nCount - 1));
		nBufferLength = nRecSize * nCount + 1;
		
		if (dwTRDataLen >= nBufferLength-1)
		{
			szChartData = strChartData.GetBuffer(nBufferLength);

			char szTemp[10+1];
			KB_p0602_OutRec2* pChartData = NULL;

			for (int i = 0; i < nCount; ++i)
			{
				if (bPercent)
				{
					pChartData = (KB_p0602_OutRec2*)szTRData;

					::memset(szTemp, 0x00, 10 + 1);
					::memcpy(szTemp, pChartData->open, sizeof(pChartData->open));
					strData.Format("%10.2f", atof(szTemp));
					memcpy(pChartData->open, strData, sizeof(pChartData->open));
					
					::memset(szTemp, 0x00, 10 + 1);
					::memcpy(szTemp, pChartData->high, sizeof(pChartData->high));
					strData.Format("%10.2f", atof(szTemp));
					memcpy(pChartData->high, strData, sizeof(pChartData->high));
					
					::memset(szTemp, 0x00, 10 + 1);
					::memcpy(szTemp, pChartData->low, sizeof(pChartData->low));
					strData.Format("%10.2f", atof(szTemp));
					memcpy(pChartData->low, strData, sizeof(pChartData->low));
					
					::memset(szTemp, 0x00, 10 + 1);
					::memcpy(szTemp, pChartData->close, sizeof(pChartData->close));
					strData.Format("%10.2f", atof(szTemp));
					memcpy(pChartData->close, strData, sizeof(pChartData->close));
				}

				memcpy(szChartData, szTRData, nRecSize);
				szChartData += nRecSize;
				szTRData -= nRecSize;
			}
		}
	}

	strChartData.ReleaseBuffer();

//	if( atoi(strHigh) < atoi(strClose) )
//		m_bPreMarket = TRUE;

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("서버로부터 받은 Data를 차트 Data로 가공::End", m_strRecvCode, m_strRecvCode.GetLength());
#endif

	return strChartData;
}

CString CStdDialog::CharToString(char *pszData, int nLen, BOOL bABS)
{
	CString strData;

	if(nLen>0)
	{
		strData.GetBufferSetLength(nLen);
		memcpy((LPSTR)(LPCSTR)strData, pszData, nLen);
		strData.ReleaseBuffer();
		//strData.Format(_T("%.*s"), nLen, pszData);
		if (bABS)
		{
			strData.Replace(_T("-"), _T(" "));
		}
	}

	return strData;
}

// Function name	: CStdDialog::RecieveData
// Author			: Sang-Yun, Nam
// Description	    : TR Data를 받는다.
// Return type		: HRESULT
// Argument         : HWND		hTrComm		--	TRComm Handle
// Argument         : LPCSTR	szTR		--	TR No.
// Argument         : LPCSTR	szMsgCode	--	Result Code
// Argument         : LPCSTR	szMessage	--	Result Message
// Argument         : LPVOID	aTRData		--	조회 데이타 Pointer
// Argument         : long		dwTRDateLen	--	조회 데이타 길이
void CStdDialog::ConvertKoscomData(LPCSTR szTR, LPVOID	aTRData, long dwTRDateLen)
{
	CString strTRCode = szTR;
	strTRCode.Remove(' ');
	if (strTRCode == QUERY_STRDOLLAR)
	{
		KB_d1018_OutRec* pOut = (KB_d1018_OutRec*)aTRData;

		//@Solomon	091123SM072	-->
		m_strDollar = CharToString((char*)pOut->price, sizeof(pOut->price));
		m_strDollar.TrimLeft();	m_strDollar.TrimRight();
		m_strDollar.Remove('-');
// 		if (atoi(m_strDollar) <= 0)
// 			m_strDollar.Format(_T("%d"), 1);
		//@Solomon	091123SM072	<--
	}
	else if (strTRCode == QUERY_STOCK_HOGA)
		ConvertHogaData(aTRData, dwTRDateLen);
	else if (strTRCode == QUERY_STRMARKET)
	{
		m_strMarketOut = "";
		KB_d1015_OutRec1* pOut1 = (KB_d1015_OutRec1*)aTRData;
		CString strOut, strBojoMsg, strData;

		strOut += CharToString(pOut1->winid, sizeof(pOut1->winid));		// 요청 윈도우 ID
		strOut += CharToString(pOut1->dataNm, sizeof(pOut1->dataNm));		// 데이타번호
		strOut += CharToString(pOut1->inNumGb, sizeof(pOut1->inNumGb));		// 구분값
		strOut += CharToString(pOut1->shcode, sizeof(pOut1->shcode));		// 종목코드
		strOut += CharToString(pOut1->nxtKey, sizeof(pOut1->nxtKey));		// 검색키
		strData = CharToString(pOut1->chtCnt, sizeof(pOut1->chtCnt));		// 조회건수
		strOut += strData;
		m_stTrInfo2.m_nChartCount = atoi(strData);

		strData = CharToString(pOut1->chtBjM, sizeof(pOut1->chtBjM));		// 보조메세지
		strBojoMsg.Format("%-400s", strData);

		strOut += strBojoMsg;

		m_strMarketOut = strOut;
	}
}

// 소리마치 플랫폼으로 커서 변경 메시지를 보낸다.
// CommApi에서 Form Wnd에 있는 BeginWaitCursor를 기동하게 한다.
#define WM_BEGIN_WAIT_CURSOR	WM_USER + 610
#define WM_END_WAIT_CURSOR		WM_USER + 611
HRESULT	CStdDialog::StatusChange(int nState)
{
//@.Solomon	if(nState==WM_BEGIN_WAIT_CURSOR)	GetParent()->SendMessage(WM_USER+105, 0, 0);
//@.Solomon	else if(nState==WM_END_WAIT_CURSOR)	GetParent()->SendMessage(WM_USER+106, 0, 0);

	return S_OK;
}

HRESULT CStdDialog::ReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CStdDialog::ReceiveData: Entry TR(%s) DataLen(%ld)\n", __LINE__, szTR, dwTRDateLen);
// 		OutputDebugString(szDebug);
// 	}
	DoNotifyChangeCode(TRUE);

	g_logMan.AddLog("CStdDialog::ReceiveData()");
	g_logMan.AddLog(CString(szTR));

	CWnd *pWndRequested = NULL;
	CString strTRCode = szTR;
	strTRCode.Remove(' ');
	if (strTRCode.CompareNoCase(QUERY_STRPREFUTOPT) == 0)
		strTRCode = QUERY_STRFUTOPT;

	if (strTRCode.IsEmpty() && g_pDebugMng) 
	{
			g_pDebugMng->Trace("%s", "Error CStdDialog::ReceiveData 에서 알수 없는 TR");
			//OutputDebugString("ReceiveData End\n");
			return 1L;
	}
	if(strTRCode.Compare("SYMBOLDATA") == 0)
	{
		m_pInputBarDlg->ParsingSymbol((char*)aTRData);
		return 1L;
	}
	BOOL bSendGuideLine = FALSE;
	CString strMsgCode = szMsgCode;
	strMsgCode.Remove(' ');
	if(strMsgCode.GetLength() <= 0)
	{
		EnableWindowAll(TRUE);	
	}
	//@Solomon:091123SM068	-->
	else if (strMsgCode.CompareNoCase(QUERY_STRGUIDELINE) == 0)
	{
		bSendGuideLine = TRUE;
	}
	//@Solomon:091123SM068	<--
	else
	{
		if(strMsgCode.GetLength() > 0 || dwTRDateLen == 0)
		{
			if(!m_bEnable)
			{
				EnableWindowAll(TRUE);
			}
		}
		//OutputDebugString("ReceiveData End\n");
		return 1L;
	}

	STTRINFO trInfo;
	long lCnt = m_arryQuery.GetSize();
	CString sTemp;
	
	for (long lPos=0;lPos<lCnt;lPos++)
	{
		trInfo = m_arryQuery.GetAt(lPos);
		if (trInfo.m_strTrCode==strTRCode)
		{
			pWndRequested = trInfo.m_pWndRequested;

			//@Solomon:091123SM068	-->
			if (bSendGuideLine)
			{
				stReceiveFromServer stR;
				stR.m_strTRCode = szMsgCode;
				stR.m_pBuffer = (LPVOID)(LPCTSTR)aTRData; 
				stR.m_dwLength = dwTRDateLen;
				if (((CChartMng*)pWndRequested) == ((CChartMng*)&m_ChartMng))
				{
					CChartItem* pItem = m_ChartMng.GetpChartItemSelected();
					if (pItem)
						pItem->SendMessageToInvoke(INVOKEMSG_RECEIVEDATA, (long)&stR, sizeof(stReceiveFromServer));
				}

				//OutputDebugString("ReceiveData End\n");
				return 0L;
			}
			//@Solomon:091123SM068	<--

			m_arryQuery.RemoveAt(lPos);
			::memset(&m_stTrInfo2, 0x00, sizeof(m_stTrInfo2));

			break;
		}
	}
	//OutputDebugString("ReceiveData 1\n");
	if (strTRCode.Compare(QUERY_STRREPLAY) == 0)
	{
		//OutputDebugString("ReceiveData 2\n");
		m_pWndReplayLib->SendMessage(RMSG_RECEIVEREPLAYDATA, (WPARAM)aTRData, (LPARAM)dwTRDateLen);
		//OutputDebugString("ReceiveData End\n");
		return 0L;
	}
	else if (strTRCode.Compare(TRNO_WORKDATE) == 0)
	{
		return 0L; //KHD : 사용 안함 
		//OutputDebugString("ReceiveData 3\n");
		if (4 != dwTRDateLen)
		{
			char szTmp[8+1] = {0,};
			m_saWorkDate.RemoveAll();
			int nDataCnt;
			char* pTrData = (char*)aTRData;

			memcpy(szTmp, pTrData, 4); // 데이터카운트
			nDataCnt = atoi(szTmp);

			for (int nIndex=0; nIndex<nDataCnt; nIndex++)
			{
				memcpy(szTmp, &pTrData[nIndex*8 + 4], 8);
				m_saWorkDate.Add(szTmp);
			}
		}

		//OutputDebugString("ReceiveData End\n");

	}
	else if (strTRCode.Compare(TRNO_MSTCODE) == 0)
	{
		CTabCodeSearch *pTabCodeSearch = (CTabCodeSearch*)m_pLeftBarDlg->m_tabctrl.GetWndCodeSearch();
		if (pTabCodeSearch != NULL)
		{
			stReceiveFromServer stR;
			stR.m_strTRCode = strTRCode;
			stR.m_pBuffer = aTRData;
			stR.m_dwLength = dwTRDateLen;

			pTabCodeSearch->SendMessage(UMSG_RECEIVEFROMSERVER, 0, (long)&stR);
		}

		return 0L;
	}
	else if(strTRCode == QUERY_STRDOLLAR)
	{
		//OutputDebugString("ReceiveData 4\n");
		ConvertKoscomData(strTRCode, aTRData, dwTRDateLen);
		//OutputDebugString("ReceiveData End\n");
		return 0L;
	}
	else if(strTRCode == QUERY_ACCOUNTCODE)
	{
		//OutputDebugString("ReceiveData 5\n");
		AddAccountCodeItem(aTRData, dwTRDateLen);
		//OutputDebugString("ReceiveData End\n");
		return 0L;
	}
	else if(strTRCode == QUERY_ACCOUNTCONC)
	{
		//OutputDebugString("ReceiveData 6\n");
		MakeAccountConcData(aTRData, dwTRDateLen);
		//OutputDebugString("ReceiveData End\n");
		return 0L;
	}

	if (!m_stTrInfo2.m_pWndRequested)
	{
		//OutputDebugString("ReceiveData 7\n");
		if(!m_strOut.IsEmpty())
		{
			m_strOut = "";
			::memset(&m_stTrInfo2, 0x00, sizeof(m_stTrInfo2));
			m_bIsWaitRequest = FALSE;

			SendLastData();
			//OutputDebugString("ReceiveData End\n");
			return 0L;
		}

		if(strTRCode == QUERY_STRMARKET)
			m_stTrInfo2.m_pWndRequested = &m_ChartMng;
		else
			m_stTrInfo2.m_pWndRequested = pWndRequested;

		//OutputDebugString("ReceiveData 7-1\n");
		ConvertKoscomData(strTRCode, aTRData, dwTRDateLen);
		//OutputDebugString("ReceiveData 7-2\n");

		//if(m_stTrInfo2.m_nChartCount || strTRCode == QUERY_STOCK_HOGA)
		if(strTRCode == QUERY_STOCK_HOGA)
		{
			//OutputDebugString("ReceiveData End\n");
			return 0L;
		}
	}
	else if(!pWndRequested)
	{
		//OutputDebugString("ReceiveData 8\n");
		pWndRequested = m_stTrInfo2.m_pWndRequested;
	}

	if(strTRCode == QUERY_STRMARKET)
	{
		//OutputDebugString("ReceiveData 9\n");
		CString strChartData = MakeMarketChartData(aTRData, m_stTrInfo2.m_nChartCount, dwTRDateLen, m_stTrInfo2.m_dPrevPrice);
//		char *pszRcv = (char*)aTRData;
//		CString strChartData;
//		strChartData.Format(_T("%s"), (char*)(pszRcv+sizeof(KB_d1015_OutRec1)));
		m_strMarketOut += strChartData;
	
		m_ChartMng.SendMessage( UMSG_RECV_MARKET_TR, (long)m_strMarketOut.GetLength(), (long)(LPCTSTR)m_strMarketOut);

//		SendMessage2StatusBar(MSG_SEARCH_END, (LPTSTR)(LPCTSTR)szMessage);
		EnableWindowAll(TRUE);	

		::memset(&m_stTrInfo2, 0x00, sizeof(m_stTrInfo2));
		m_bIsWaitRequest = FALSE;

		SendNextData();
		//OutputDebugString("ReceiveData End\n");
		return 0L;
	}

	if(pWndRequested && m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd() && m_pInputBarDlg->m_pCodeInput )
	{
		//OutputDebugString("ReceiveData 10\n");
		//2007.04.25 by LYH 받은 코드(현재 CodeCombo 코드와 비교해서 다를 경우 재조회)
		if(m_strRecvCode.GetLength() == 0 || m_pInputBarDlg->m_pCodeInput->GetCode().GetLength() == 0
			|| m_ChartMng.IsInLoop() || m_pInputBarDlg->m_pCodeInput->GetCode() == m_strRecvCode)
		{
			if(pWndRequested->GetSafeHwnd())
			{
				int nUnitType = WON_TYPE;
				char chDataType = DAY_DATA_CHART;
				CChartItem* pChartItem = m_ChartMng.GetpChartItemSelected();
				if(pChartItem)
				{
					nUnitType = pChartItem->m_nChartNumericValueType;
					chDataType = pChartItem->GetType();
				}

				//OutputDebugString("ReceiveData 10-1\n");
				int nMarket = CMasterTypeInfo::STOCK_CHART;
				if(strTRCode == QUERY_STRJONGMOK)
					nMarket = CMasterTypeInfo::STOCK_CHART;
				else if(strTRCode == QUERY_STRUPJONG)
					nMarket = CMasterTypeInfo::UPJONG_CHART;
				else if((strTRCode == QUERY_STRFUTOPT) || (strTRCode == QUERY_STRPREFUTOPT))
					nMarket = CMasterTypeInfo::FUTOPT_CHART;
				else if(strTRCode == QUERY_STRSTOCKFUTOPT)
					nMarket = CMasterTypeInfo::STOCKFUTOPT_CHART;
				else if(strTRCode == QUERY_STRFOREIGN)
					nMarket = CMasterTypeInfo::FOREIGN_CHART;
				else if(strTRCode == QUERY_STRFOJIPYO)
					nMarket = CMasterTypeInfo::FUOPT_JIPYO_CHART;
				//OutputDebugString("ReceiveData 10-2\n");

//@solomon 코스콤데이터로 바꾸는 루틴이 필요하지 않다.
//				CString strChartData = MakeChartData(aTRData, m_stTrInfo2.m_nChartCount, dwTRDateLen, nMarket, chDataType, m_stTrInfo2.m_dPrevPrice, nUnitType);
//				m_strOut += strChartData;

				stReceiveFromServer stR;
				stR.m_strTRCode = strTRCode;
//@solomon 코스콤데이터로 바꾸는 루틴이 필요하지 않은것 같아서 수정  20091007 정용호
//				stR.m_pBuffer = (LPVOID)(LPCTSTR)m_strOut;
				stR.m_pBuffer = aTRData; 
				stR.m_dwLength = dwTRDateLen; //m_strOut.GetLength();

				//OutputDebugString("ReceiveData 10-3\n");
				pWndRequested->SendMessage(UMSG_RECEIVEFROMSERVER, 0, (long)&stR);
				//OutputDebugString("ReceiveData 10-4\n");
				m_strOut = "";

				::memset(&m_stTrInfo2, 0x00, sizeof(m_stTrInfo2));
				m_bIsWaitRequest = FALSE;
				//OutputDebugString("ReceiveData 10-5\n");
				SendLastData();
				//OutputDebugString("ReceiveData 10-6\n");
			}
			m_strRecvCode = "";
		}
		else
		{
			if( m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd() )
				m_pInputBarDlg->SetKeyCodeChange(NID_CODEINPUT, (LPARAM)(LPCTSTR)m_pInputBarDlg->m_pCodeInput->GetCode());
		}
	}
	else
	{
		//OutputDebugString("ReceiveData 11\n");
		::memset(&m_stTrInfo2, 0x00, sizeof(m_stTrInfo2));
		m_bIsWaitRequest = FALSE;

		long lCnt = m_arryQueryInvoke.GetSize();
		for(long lPos=0;lPos<lCnt;lPos++)
		{
			trInfo = m_arryQueryInvoke.GetAt(lPos);
			if(trInfo.m_strTrCode==szTR)
			{
				pWndRequested = trInfo.m_pWndRequested;
				m_arryQueryInvoke.RemoveAt(lPos);
				break;
			}
		}
		if (pWndRequested)
		{
			stReceiveFromServer stR;

			stR.m_strTRCode = szTR;
			stR.m_pBuffer = aTRData;
			stR.m_dwLength = dwTRDateLen;
			if (dwTRDateLen>=12)
			{
				CChartItem *pWndChartItem = NULL;
				char szWndChartItem[18];
				memcpy(szWndChartItem,aTRData,12);
				szWndChartItem[12] = NULL;
				pWndChartItem = (CChartItem *)atol(szWndChartItem);
				if(pWndChartItem->GetSafeHwnd())
				{
					pWndChartItem->SendMessageToInvoke(INVOKEMSG_RECEIVEDATA,(long)&stR, sizeof(stReceiveFromServer));					
				}
			}
		}
	}

// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CStdDialog::ReceiveData: Middle\n", __LINE__);
// 		OutputDebugString(szDebug);
// 	}

	//OutputDebugString("ReceiveData 12\n");
	SendLastData();

	BOOL bRet = m_ChartMng.IsToBeReceivedMore();
	if(!m_arryQuery.GetSize()&&!bRet)
	{
		ClearSaveKey();	//100530:동일화면 다르게 저장/읽기
		EnableWindowAll();
	}

	//대기중인 TR 호출
	CallStandByTr();		
		
 	//if(g_pDebugMng) 
 	//{
 	//	g_pDebugMng->Trace("%s TR[%s]", "CStdDialog::ReceiveData", "End");
 	//}
	return S_OK;
}

HRESULT	CStdDialog::ReceiveMessage(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage)
{
	CString szTmp;
	szTmp.Format("[%s]%s\n", szMsgCode, szMessage);
	
	if(strcmp(szMsgCode, "80001")==0)		// 타임아웃
	{
		AfxMessageBox(szMessage);
		EnableWindowAll(TRUE);	//@유진 임시코드임. 이 루틴이 남아있으면 삭제바람.
	}
	else if(strcmp(szMsgCode, "99999")==0)	// 해당 자료가 없습니다.
	{
		EnableWindowAll(TRUE);	//@유진 임시코드임. 이 루틴이 남아있으면 삭제바람.
		
		AfxMessageBox("해당 자료가 없습니다.");
		m_ChartMng.ReplaceOldCode();
	}
	return S_OK;
}

BOOL CStdDialog::GetStatusEnable()
{
	return m_bEnable;
}

void CStdDialog::EnableWindowAll(BOOL bEnable /*= TRUE*/)
{
	m_bEnable = bEnable;
	if(bEnable)
	{
		if(m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd())
		{
			m_pLeftBarDlg->EnableWindow();
		}
		if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		{
			m_pInputBarDlg->EnableWindow();
		}
		if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
		{
			if(m_hReplayChartLib == NULL)
				m_pExtInputBarDlg->EnableWindow();
			else
				m_pExtInputBarDlg->EnableWindow(FALSE);
		}
		if(m_ChartMng.GetSafeHwnd())
		{
			m_ChartMng.EnableWindow();
		}

		g_logMan.SetLogging(FALSE);

		KillTimer(1);
		//@solomon 		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}
	else
	{
		if(m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd())
		{
			m_pLeftBarDlg->EnableWindow(FALSE);
		}
		if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())	
		{
			m_pInputBarDlg->EnableWindow(FALSE);
		}

		if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())	
		{
			m_pExtInputBarDlg->EnableWindow(FALSE);
		}

		if(m_ChartMng.GetSafeHwnd())
		{
			m_ChartMng.EnableWindow(FALSE);
		}

		//@Solomon 		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

		//SetTimer(1,30000,NULL); // 30sec
		SetTimer(1,10000,NULL); // 10sec
		g_logMan.ResetLog();
		g_logMan.SetLogging(TRUE);
	}

	if( m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd() )
		m_pInputBarDlg->MoveCodeInputCtrl();

	if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
		m_pExtInputBarDlg->MoveAccountCtrl();
}

// 테스트모드이면 서버에서 받은 데이터 말고 서버에서 받은 데이터로 처리한다.
int CStdDialog::Send2Server(LPCTSTR lpTrCode,LPCTSTR lpData,int nSize,BOOL bWait, int nArrayIndex)
{
// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CStdDialog::Send2Server: Start TR(%s) DataLen(%d)\n", __LINE__, lpTrCode, nSize);
// 		OutputDebugString(szDebug);
// 	}

	//int nTestMode = AfxGetApp()->GetProfileInt("TESTMODE", "HW", 0);
//	SendMessage2StatusBar(MSG_GOING);	
	EnableWindowAll(FALSE);
	if(m_TRComm)
	{
		if (m_bReplayMode)
		{
			if (CString(lpTrCode) == QUERY_STRJONGMOK)
			{
				KB_p0602_InRec1 *pInRec;
				pInRec = (KB_p0602_InRec1 *)lpData;
				char szDay[5];
				memset(szDay, 0x00, sizeof(szDay));
				memcpy(szDay, pInRec->nDayBefore, sizeof(pInRec->nDayBefore));
				
				if (atoi(szDay) > 0)
				{
					if (m_saWorkDate.GetSize() > 1)
					{
						CString szDate = m_saWorkDate.GetAt(1);
						memcpy(pInRec->date, szDate, sizeof(pInRec->date));
					}
				}
			}
			else if (CString(lpTrCode) == QUERY_STRFUTOPT)
			{
				KB_p0604_InRec1 *pInRec;
				pInRec = (KB_p0604_InRec1 *)lpData;
				char szDay[5];
				memset(szDay, 0x00, sizeof(szDay));
				memcpy(szDay, pInRec->Sredate, sizeof(pInRec->Sredate));

				if (atoi(szDay) > 0)
				{
					if (m_saWorkDate.GetSize() > 1)
					{
						CString szDate = m_saWorkDate.GetAt(1);
						memcpy(pInRec->Sdate, szDate, sizeof(pInRec->Sdate));
					}
				}
			}
		}

		if(CString(lpTrCode) == QUERY_STRMARKET)
		{
			KB_d1015_InRec1* pMarket = (KB_d1015_InRec1*)lpData;

			char szCode[9] = {NULL, };
			::memcpy(szCode, pMarket->shcode, sizeof(pMarket->shcode));
			CString strCode = szCode;
			strCode.TrimRight();

			CString strMarketName, strNum;
			strMarketName.Format("%s", pMarket->dataNm);

			int nCodeType = -1;
			GetMarketTypeFromCode(strCode, nCodeType);
			if(nCodeType == CDRCodeTypeInfo::CODE_ELW && !IsInvestMarket(strMarketName))
			{
				EnableWindowAll(TRUE);	
				return 0;
			}

			if(strMarketName.Find("기관누적거래추이") >= 0)
			{
				strNum.Format("%d", m_pInputBarDlg->GetAllCount());
				memcpy(pMarket->num,strNum,sizeof(pMarket->num));
				memcpy(pMarket->nxtGb ,"0",sizeof(pMarket->nxtGb));
//				memcpy(pMarket->nxtKey ,"              ",sizeof(pMarket->nxtKey));
				memset(pMarket->nxtKey, 0x20, sizeof(pMarket->nxtKey));
			}
		}


		g_logMan.AddLog("CStdDialog::Send2Server()");
		g_logMan.AddLog(CString(lpTrCode));
		g_logMan.AddLog(CString(lpData));

		if(CString(lpTrCode) == QUERY_STOCK_HOGA)
		{
			m_TRComm->SetTR(lpTrCode);
			m_TRComm->SetBaseData("PtManager", m_szPTManager);
			m_TRComm->SetBaseData("SCREENCODE", GetScreenNo());

			return m_TRComm->Send2Server((LPVOID)lpData, nSize, TRKEY_STOCK_HOGA);
		}
		else if	(CString(lpTrCode) == QUERY_STRDOLLAR || CString(lpTrCode) == QUERY_ACCOUNTCODE ||
				 CString(lpTrCode) == QUERY_ACCOUNTCONC)
		{
			m_TRComm->SetTR(lpTrCode);
			m_TRComm->SetBaseData("PtManager", m_szPTManager);
			m_TRComm->SetBaseData("SCREENCODE", GetScreenNo());
//			m_TRComm->ChangeCallback(&m_xTLMCallback);

			return m_TRComm->Send2Server((LPVOID)lpData, nSize, TRKEY_STRDOLLAR);
		}
		else if	(CString(lpTrCode) == TRNO_WORKDATE)
		{
			m_TRComm->SetTR(lpTrCode);
			m_TRComm->SetBaseData("PtManager", m_szPTManager);
			m_TRComm->SetBaseData("SCREENCODE", GetScreenNo());
//			m_TRComm->ChangeCallback(&m_xTLMCallback);
// 			{
// 				CString szDebug;
// 				szDebug.Format("[DRCS][LOAD](%8d) CStdDialog::Send2Server: End(%d)\n", __LINE__);
// 				OutputDebugString(szDebug);
// 			}
			return 0;//KHD
			return m_TRComm->Send2Server((LPVOID)lpData, nSize, TRKEY_WORKDATE);
		}
		else if	(CString(lpTrCode) == TRNO_MSTCODE)
		{
			m_TRComm->SetTR(lpTrCode);
			m_TRComm->SetBaseData("PtManager", m_szPTManager);
			m_TRComm->SetBaseData("SCREENCODE", GetScreenNo());
			m_TRComm->SetBaseDataLong("IPartnerSite", (long)&m_xPartnerSite);
			return m_TRComm->Send2Server((LPVOID)lpData, nSize, TRKEY_MSTCODE);
		}
		// 종목TR
		else
		{
			if(m_stTrInfo2.m_pWndRequested || m_bIsWaitRequest)
			{
				//[A00000380]n*n형태 구성에서 시장지표 적용 후 시장지표가 마지막 차트에서만 보여지는 현상
// 				if(CString(lpTrCode) != QUERY_STRMARKET)
// 				{
// 					POSITION pos = m_lstSendData.GetHeadPosition();
// 					while(pos)
// 					{
// 						STSENDDATAQUE* pSendData = m_lstSendData.GetNext(pos);
// 						::free(pSendData->lpData);
// 						delete pSendData;
// 					}
// 
// 					m_lstSendData.RemoveAll();
// 				}

				STSENDDATAQUE* pSendData = new STSENDDATAQUE;

				pSendData->strTRCode = lpTrCode;
				pSendData->lpData = ::strdup(lpData);
				pSendData->nSize = nSize;
				pSendData->bWait = bWait;

				m_lstSendData.AddTail(pSendData);
// 				{
// 					CString szDebug;
// 					szDebug.Format("[DRCS][LOAD](%8d) CStdDialog::Send2Server: End(%d)\n", __LINE__);
// 					OutputDebugString(szDebug);
// 				}
				return 0;
			}

			if (CString(lpTrCode) == QUERY_STRFUTOPT)
			{
				KB_p0604_InRec1* pData = (KB_p0604_InRec1*)lpData;
				CString strCode;
				strCode.Format(_T("%.*s"), sizeof(pData->Scode), pData->Scode);
				strCode.TrimLeft();	strCode.TrimRight();
				if (m_pInputBarDlg && m_pInputBarDlg->m_pFOHisBtn)
				{
					int nType = m_pInputBarDlg->m_pFOHisBtn->GetCodeType(strCode);
					switch (nType)
					{
					case futureType:	// 선물
//KHD					case spreadType:	// 스프레드
					case callType:		// 콜옵션
					case putType:		// 풋옵션
					case sfutureType:	// 주식선물
					case koptionType:	// 주식옵션
						lpTrCode = QUERY_STRPREFUTOPT;
					}
				}
			}

			m_TRComm->SetTR(lpTrCode);
			m_TRComm->SetBaseData("PtManager", m_szPTManager);
			m_TRComm->SetBaseData("SCREENCODE", GetScreenNo());

#ifdef _DEBUG
			CTime tm = CTime::GetCurrentTime();
			CString strTime;
			strTime.Format("\n\n### Send2Server - %d:%d:%d\n", tm.GetHour(), tm.GetMinute(), tm.GetSecond());
			AfxTrace(strTime);
#endif
			m_bIsWaitRequest = TRUE;

// 			{
// 				CString szDebug;
// 				szDebug.Format("[DRCS][LOAD](%8d) CStdDialog::Send2Server: End(%d)\n", __LINE__);
// 				OutputDebugString(szDebug);
// 			}
			return m_TRComm->Send2Server((LPVOID)lpData, nSize, TRKEY_FID_END);
		}

// 		{
// 			CString szDebug;
// 			szDebug.Format("[DRCS][LOAD](%8d) CStdDialog::Send2Server: End(%d)\n", __LINE__);
// 			OutputDebugString(szDebug);
// 		}
		return 0;
	}
	else
	{
// 		{
// 				CString szDebug;
// 				szDebug.Format("[DRCS][LOAD](%8d) CStdDialog::Send2Server: End(%d)\n", __LINE__);
// 				OutputDebugString(szDebug);
// 		}
		return 0;
	}
}

void CStdDialog::SetShowExtInputBarDlg(BOOL bShowExtInputBarDlg)
{
	m_bShowExtInputBarDlg = bShowExtInputBarDlg;
	if( m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd() )
	{
		m_pInputBarDlg->SetShowExtInputBarDlg(bShowExtInputBarDlg);
	}	
	Resize();
}

void CStdDialog::SetLeftBarType(UINT nLeftBarType)
{
	m_nLeftBarType = nLeftBarType;
	CRect rect;	
	CRect chartMngRect;

	if(!m_pInputBarDlg||!m_pInputBarDlg->GetSafeHwnd()||!m_pExtInputBarDlg||!m_pLeftBarDlg||!m_ChartMng.GetSafeHwnd()) return;

	rect = GetRect(ID_INPUTBAR,TRUE);
	m_pInputBarDlg->MoveWindow(rect);
	m_pInputBarDlg->ShowWindow(SW_SHOW);
	m_pInputBarDlg->SetLeftBarType(m_nLeftBarType);
	
	if(m_ChartMng.GetSafeHwnd())
	{
		m_ChartMng.GetWindowRect(chartMngRect);
		ScreenToClient(chartMngRect);
		CRect rectNew = GetRect(ID_CHART,TRUE);
		if(chartMngRect!=rectNew)	
		{
			chartMngRect.left	= rectNew.left;		//ChartMng
			chartMngRect.right	= rectNew.right;	//ChartMng
			chartMngRect.bottom	= rectNew.bottom;		
			chartMngRect.top	= rectNew.top;				
		}

		m_ChartMng.MoveWindow(chartMngRect);
	}

	rect = GetRect(ID_EXTINPUTBAR,TRUE);
	if( m_pExtInputBarDlg->GetSafeHwnd() )
	{
		m_pExtInputBarDlg->MoveWindow(rect);
		m_pExtInputBarDlg->ShowWindow(SW_SHOW);
	}

	rect = GetRect(ID_LEFTBAR,TRUE);
	if(rect.IsRectNull())
	{
		if( m_pLeftBarDlg->GetSafeHwnd() )
		{
			//<<20100412_JS.Kim 솔로몬 요청사항. 무조건 지표탭을 보여준다
			m_pLeftBarDlg->m_tabctrl.SetCurSel(1);
			//>>
			m_pLeftBarDlg->ShowWindow(SW_HIDE);
	//		m_pLeftBarDlg->SetParent(this);
			//JS.Kim_20101005   화면 깜박임 현상 제거
			//m_pLeftBarDlg->ModifyStyle(0, WS_DLGFRAME|WS_CLIPSIBLINGS);
			m_pLeftBarDlg->ModifyStyle(0, WS_DLGFRAME|WS_CLIPSIBLINGS|WS_CLIPCHILDREN);
		}
		//JS.Kim_20101005   화면 깜박임 현상 제거
		//m_pInputBarDlg->ModifyStyle(0, WS_CLIPSIBLINGS);
		m_pInputBarDlg->ModifyStyle(0, WS_CLIPSIBLINGS|WS_CLIPCHILDREN);
		if( m_pExtInputBarDlg->GetSafeHwnd() )
			//JS.Kim_20101005   화면 깜박임 현상 제거
			//m_pExtInputBarDlg->ModifyStyle(0, WS_CLIPSIBLINGS);
			m_pExtInputBarDlg->ModifyStyle(0, WS_CLIPSIBLINGS|WS_CLIPCHILDREN);
		if(m_btnSideBar.GetSafeHwnd())
		{
			m_btnSideBar.MoveWindow(GetRect(IDC_SIDEBAR, TRUE));
			m_btnSideBar.ShowWindow(SW_SHOW);
			m_btnSideBar.Invalidate();
		}
	}
	else
	{
		if( m_pLeftBarDlg->GetSafeHwnd() )
		{
			//JS.Kim_20101005   화면 깜박임 현상 제거
			//m_pLeftBarDlg->ModifyStyle(WS_DLGFRAME|WS_CLIPSIBLINGS, 0);
			m_pLeftBarDlg->ModifyStyle(WS_DLGFRAME, WS_CLIPSIBLINGS|WS_CLIPCHILDREN);
	//		m_pLeftBarDlg->SetParent(this);
		}
		//JS.Kim_20101005   화면 깜박임 현상 제거
		//m_pInputBarDlg->ModifyStyle(WS_CLIPSIBLINGS, 0);
		m_pInputBarDlg->ModifyStyle(0, WS_CLIPSIBLINGS|WS_CLIPCHILDREN);
		if( m_pExtInputBarDlg->GetSafeHwnd() )
			//JS.Kim_20101005   화면 깜박임 현상 제거
			//m_pExtInputBarDlg->ModifyStyle(WS_CLIPSIBLINGS, 0);
			m_pExtInputBarDlg->ModifyStyle(0, WS_CLIPSIBLINGS|WS_CLIPCHILDREN);
	
		if( m_pLeftBarDlg->GetSafeHwnd() )
		{
			//<<20100412_JS.Kim 솔로몬 요청사항. 무조건 지표탭을 보여준다
			m_pLeftBarDlg->m_tabctrl.SetCurSel(1);
			//>>	
			m_pLeftBarDlg->MoveWindow(rect);
			m_pLeftBarDlg->ShowWindow(SW_SHOW);
		}

		if(m_btnSideBar.GetSafeHwnd())
		{
			m_btnSideBar.MoveWindow(CRect(0,0,0,0));
			m_btnSideBar.ShowWindow(SW_HIDE);
			m_btnSideBar.Invalidate();
		}
	}

	rect = GetRect(ID_SPLITTER,TRUE);
	if(rect.IsRectNull())
	{
		m_xSplitter1.ShowWindow(SW_HIDE);
	}
	else
	{
		m_xSplitter1.MoveWindow(rect);
		m_xSplitter1.ShowWindow(SW_SHOW);
	}	
	switch(m_nLeftBarType)
	{
	case 0:
		break;
	case 1:
		m_xSplitter1.DetachAllCtrls();
		m_xSplitter1.AttachAsLeftCtrl(m_pLeftBarDlg);
		m_xSplitter1.AttachAsRightCtrl(ID_CHART);
		m_xSplitter1.AttachAsRightCtrl(m_pInputBarDlg);
		m_xSplitter1.AttachAsRightCtrl(m_pExtInputBarDlg);	
		m_xSplitter1.RecalcLayout();
		break;
	case 2:
		m_xSplitter1.DetachAllCtrls();
		m_xSplitter1.AttachAsLeftCtrl(ID_CHART);
		m_xSplitter1.AttachAsLeftCtrl(m_pInputBarDlg);
		m_xSplitter1.AttachAsLeftCtrl(m_pExtInputBarDlg);
		m_xSplitter1.AttachAsRightCtrl(m_pLeftBarDlg);	
		if(	(m_bShowDataListWnd				) &&
			(m_pDataWindow != NULL			) &&
			(m_pDataWindow->GetSafeHwnd()	) 
		)
		{
			m_xSplitter1.AttachAsLeftCtrl(m_pDataWindow);
		}
		m_xSplitter1.RecalcLayout();
		break;
	}

	if(m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd())
		m_pLeftBarDlg->Invalidate();
	m_xSplitter1.Invalidate();
}

LRESULT CStdDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
	{
	case UMSG_SEND2SERVER:
		{
			BOOL bWait = wParam;
			stSend2Server *pSt = (stSend2Server *)lParam;

/*
			if( (CString(pSt->m_strTRCode) != QUERY_STRJONGMOK) && //주식		차트 데이타
				(CString(pSt->m_strTRCode) != QUERY_STRFUTOPT) && //선물옵션 차트 데이타
				(CString(pSt->m_strTRCode) != QUERY_STRSTOCKFUTOPT) && //주식선물옵션 차트 데이타
				(CString(pSt->m_strTRCode) != QUERY_STRUPJONG) && //업종		차트 데이타
				(CString(pSt->m_strTRCode) != QUERY_STRFOREIGN) && //해외지수 차트 데이타
				(CString(pSt->m_strTRCode) != QUERY_STRTHEME_GROUP) && //테마 그룹 데이타
				(CString(pSt->m_strTRCode) != QUERY_STRTHEME_ITEM) && //테마 그룹내 종목  데이타
				(CString(pSt->m_strTRCode) != QUERY_STRMARKET) && //시장지표
				(CString(pSt->m_strTRCode) != QUERY_STOCK_HOGA)		 && //주식 호가
				(CString(pSt->m_strTRCode) != QUERY_STR_AVGMP) &&      //이평 조회 
				(CString(pSt->m_strTRCode) != QUERY_STRDOLLAR) )
				break;			
*/

			if(pSt->m_strTRCode == QUERY_STRMARKET)
			{
				Send2Server(pSt->m_strTRCode,(char*)pSt->m_pBuffer,pSt->m_lLength, FALSE);
				return 1;
			}

			int lCnt = m_arryQuery.GetSize();
			
			STTRINFO trInfo, trCheckInfo;
			trInfo.m_pWndRequested = pSt->m_pWnd;
			trInfo.m_strTrCode = pSt->m_strTRCode;

			for(long lPos=0;lPos<lCnt;lPos++)
			{
				trCheckInfo = m_arryQuery.GetAt(lPos);
			}
			
			if(pSt->m_nType==0)
			{				
				trInfo.m_nType = 0;
				m_arryQuery.Add(trInfo);
			}
			else
			{
				trInfo.m_nType = pSt->m_nType;
				m_arryQueryInvoke.Add(trInfo);
			}

			if (CString(pSt->m_strTRCode) == QUERY_STRFUTOPT)
				Send2Server(pSt->m_strTRCode,(char*)pSt->m_pBuffer,pSt->m_lLength, bWait, m_arryQueryInvoke.GetSize());
			else
				Send2Server(pSt->m_strTRCode,(char*)pSt->m_pBuffer,pSt->m_lLength, bWait);
		}
		break;
	case UMSG_SENDSHAREDCODE:
		{
			// 종목컨트롤을 통한 공유처리
			if( m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd() )
			{
				if(!m_pInputBarDlg->GetShareCodeChanged())
				{
//					if((m_nChartType == SINGLE_CHART || m_nChartType == MINI_SINGLE_CHART) && (m_nMarketType == UPJONG_CHART || m_nMarketType == FOREIGN_CHART))
						if(m_nMarketType == lParam && m_ChartMng.GetOldCode() == CString((LPCTSTR)wParam))
							break;
#ifdef _DEBUG
					CTime tm = CTime::GetCurrentTime();
					CString strTime;
					strTime.Format("\n\n### SENDSHAREDCODE이후 - %d:%d:%d\n", tm.GetHour(), tm.GetMinute(), tm.GetSecond());
					AfxTrace(strTime);
#endif
					
					STINPUTCODEDATA stInputCodeData;
					stInputCodeData.m_strCode = CString((LPCTSTR)wParam);
//					stInputCodeData.m_strCode = CString("101D3000");
					stInputCodeData.m_strTime = "";
					stInputCodeData.m_nDefault = MODE_DEFAULT;
					stInputCodeData.m_nMode = CHANGE_ITEM;
					
					m_nMarketType = lParam;
					m_ChartMng.SetMarketType(m_nMarketType);

// 					if(m_nScreenID == gSTOCKFUTOPT_0503)	//KB903
// 					{
// 						m_nMarketType = lParam;
// 						m_ChartMng.SetMarketType(m_nMarketType);
// 					}
// 					if(m_nScreenID == gFUTOPTJIPYO_0515)
// 					{
// 						m_nMarketType = lParam;
// 						m_ChartMng.SetMarketType(m_nMarketType);
// 					}
// 					if(m_nScreenID == gSTOCKFUTOPT_0516)
// 					{
// 						m_nMarketType = lParam;
// 						m_ChartMng.SetMarketType(m_nMarketType);
// 					}

//					if(m_nChartType != MULTI_CHART)
//						m_ChartMng.m_strCodeForLoadChart = stInputCodeData.m_strCode;

					m_ChartMng.SendMessage(UMSG_INPUTCODETOCHARTMNG, MODE_DEFAULT, (LPARAM)&stInputCodeData);
					
#ifdef _DEBUG
					tm = CTime::GetCurrentTime();
					strTime.Format("\n\n### INPUTCODETOCHARTMNG 메세지 처리후 - %d:%d:%d\n", tm.GetHour(), tm.GetMinute(), tm.GetSecond());
					AfxTrace(strTime);
#endif			
				}
				else if( m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd() )
					m_pInputBarDlg->SetShareCodeChanged(FALSE);
			}
		}		
		break;
	case UMSG_SENDSYNCCODE:
		{
			//m_xGSharedCli.SetData("코드-동기화",(LPCTSTR)wParam);
		}		
		break;
	case UMSG_SENDSHARETIME:
		{
			m_ChartMng.SetSharedTime((LPCTSTR)wParam);
		}		
		break;
	case UMSG_SENDTOMSGBOX:
		{
//			m_pMsgBar->ShowMessage((LPCTSTR)wParam,(CWnd*)lParam);
		}
		break;
	case UMSG_EDITCODE_INIT:
		{
			if( m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd() )
				m_pInputBarDlg->SetCodeInit();
		}
//		break;
	case UMSG_UPDATECODE:
		{
			//코스콤 종목공유 세팅(재정의)
			if( m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd() )
			{
				switch(lParam) {
				case 1:
					if(m_pInputBarDlg->m_nMarketType != CMasterTypeInfo::STOCK_CHART)
						return 1L;
					break;
				case 8:
				case 12:
					if(m_pInputBarDlg->m_nMarketType != CMasterTypeInfo::FUTOPT_CHART)
						return 1L;
					break;
				case 3:
						return 1L;
				case 100:
					return 1L;
				default:
					return 1L;
				}

				// 종목코드로 RQ를 찾고 선택 : 복수종목 - ojtaso (20070705)
				if(!m_ChartMng.FindSelectChartByItemCode((LPCTSTR)wParam))
				{
					m_pInputBarDlg->SetMode(0);
					CString strCodeInputCode = m_pInputBarDlg->m_pCodeInput->GetCode();
					CString strSetCode = (LPCTSTR)wParam;
					if(strSetCode.GetLength() && strCodeInputCode != strSetCode)
					{
						m_pInputBarDlg->SetShareCodeChanged(TRUE);
						m_pInputBarDlg->m_pCodeInput->SetCode((LPCTSTR)wParam);
					}
				}
			}
		}
		break;
	case UMSG_DROPCODE:
		{
			//코스콤 종목공유 세팅(재정의)
			if(m_nChartType == MULTI_CHART && m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
			{
				m_pInputBarDlg->SetCodeDragAndDrop(CString((LPCTSTR)wParam), lParam);
								STINPUTCODEDATA stInputCodeData;
				stInputCodeData.m_strCode = CString((LPCTSTR)wParam);
				stInputCodeData.m_strTime = "";
				stInputCodeData.m_nDefault = MODE_DEFAULT;
				stInputCodeData.m_nMode = CHANGE_ITEM;
				m_ChartMng.SendMessage(UMSG_INPUTCODETOCHARTMNG, MODE_DEFAULT, (LPARAM)&stInputCodeData);
			}
			//코스콤 종목공유 세팅(재정의)
			if(m_nChartType == SINGLE_CHART)
			{
				if( m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd() )
				{
					switch(lParam) {
					case 1:
						if(m_pInputBarDlg->m_nMarketType != CMasterTypeInfo::STOCK_CHART)
							return 1L;
						break;
					case 8:
					case 12:
						if(m_pInputBarDlg->m_nMarketType != CMasterTypeInfo::FUTOPT_CHART)
							return 1L;
						break;
					case 3:
							return 1L;
					case 100:
						return 1L;
					default:
						return 1L;
					}

					STINPUTCODEDATA stInputCodeData;
					stInputCodeData.m_strCode = CString((LPCTSTR)wParam);
					stInputCodeData.m_strTime = "";
					stInputCodeData.m_nDefault = MODE_DEFAULT;
					stInputCodeData.m_nMode = CHANGE_ITEM;
					m_ChartMng.SendMessage(UMSG_INPUTCODETOCHARTMNG, MODE_DEFAULT, (LPARAM)&stInputCodeData);
				}
			}
		}
		break;
	case UMSG_GETSTATUSENABLE:
		{
			return GetStatusEnable();
		}
		break;
	case UMSG_CHANGEMODEBYINPUTBAR:
		{
			int nMode = wParam;
			if( m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd() )
				m_pInputBarDlg->SetMode(nMode);
		}
		break;
	case UMSG_SETHIGHLIGHTINFO:
		{
			if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
			{
				m_pExtInputBarDlg->SendMessage(UMSG_SETHIGHLIGHTINFO,wParam,lParam);
			}			
		}
		break;
	case USMG_SHOW_DATAWND:
		{
			m_bShowDataListWnd = (BOOL)lParam;
			Resize();
			CRect ViewRect(0,0,0,0);
			ViewRect = GetRect(ID_CHART);
			ShowDataListView(m_bShowDataListWnd, ViewRect);
		}
		break;
	case USMG_PLEASE_RESIZE:
		{
		}
		break;	
	case UMSG_SETMATRIX:
		{
		}
		break;
	case UMSG_FORCE_SHOW_HOGAWND:
		{
		}
		break;
	case WM_DRDS_DATA:
		{
		}
		break;
	case UMSG_SENDTR_HOGA_DATA:
		{		
			CString* pStrCode, sCode;
			pStrCode = (CString*)wParam;
			sCode.Format("%s", pStrCode->GetBuffer(pStrCode->GetLength()));
			Request_StockHogaData(sCode);

			//100204-alzioyes 조회가는할 때는 다시 "호"가 보이게 처리.
			if(m_ChartMng.m_pFloatingDlg && m_ChartMng.m_pFloatingDlg->GetSafeHwnd())
				m_ChartMng.m_pFloatingDlg->ShowWindow(SW_SHOW);
		}
		break;
	case UMSG_DISCONNECT_HOGA_REAL:
		{
			Request_Disconnect_StockRealHogaData(_T(""));
		}
		break;
	case UMSG_GET_CHARTMODE:
		{
			// 복기차트 사용여부 : 복수종목 - ojtaso (20070530)
			if( m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd() )
			{
				if(wParam == 1 && !m_pInputBarDlg->GetReplayChartMode())
					return -1;

				return m_pInputBarDlg->GetCurrentMode();
			}
		}
		break;
	case UMSG_GET_REPLAY_STARTTIME:
		{
			if( m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd() )
				return m_pInputBarDlg->GetReplayStartTime();
		}
		break;
	case UMSG_GET_REPLAY_DAYBEFORE:
		{
			if( m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd() )
				return m_pInputBarDlg->GetReplayDayBefore();
		}
		break;
	case UMSG_STOP_JIPYOCYCLE:
		{
			if(m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd())
			{
				m_pLeftBarDlg->SendMessage(message, 0, 0);
			}			
		}
		break;
	case UMSG_SHOW_HOGADLG:
		{
			// for Invalidate Floating Hoga Dialog - SeoJeong - 2005/08/26
			if(m_ChartMng)
				m_ChartMng.m_pFloatingDlg->Invalidate();
			break;
		}
	case UMSG_CHECK_BE_COMPLETE_QUERYDATA:
		{
			STTRINFO trCheckInfo;
			
			CString strTRCode;
			strTRCode.Format("%s",(LPTSTR)(LPCTSTR)wParam);

			int lCnt = m_arryQuery.GetSize();
			for(long lPos=0;lPos<lCnt;lPos++)
			{
				trCheckInfo = m_arryQuery.GetAt(lPos);
				if(trCheckInfo.m_strTrCode == strTRCode)
				{
#ifdef _DEBUG
					CString strTemp;
					strTemp.Format("Not Send 2 TR No. : %s", (LPTSTR)(LPCTSTR)strTRCode);
					//OutputDebugString(strTemp);
#endif
					return 0l;
				}				
			}
			return 1l;
		}
		break;
	case UMSG_GETJONGMOKLINK:
		{
			if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
				return m_pExtInputBarDlg->SendMessage(UMSG_GETJONGMOKLINK);
		}
		break;
	case UMSG_DESTROY_FLOATINGHOGA:
		{
			if( m_ChartMng.m_pFloatingDlg == NULL ) return 0;
			
			if(lParam)
				m_ChartMng.m_pFloatingDlg->ShowWindow(SW_SHOW);
			else
				m_ChartMng.m_pFloatingDlg->ShowWindow(SW_HIDE);
		}
		break;
	case UMSG_EXTINPUT_CHANGEDATE:
		{
			if( m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd() )
				m_pInputBarDlg->SendMessage(UMSG_EXTINPUT_CHANGEDATE, wParam, lParam);
		}
		break;
	case UMSG_ONECLICK_HIDE:
		{
			SetOneClickHide(lParam);
		}
		break;
	case UMSG_RESETCHART:
		{
			ResetChartData();

			if(m_pDlgToolbar && m_pDlgToolbar->GetSafeHwnd())
			{
				m_pDlgToolbar->ResetAllButton();
				m_bShowToolBar = TRUE;
				if(m_bShowDataListWnd)
				{
					m_bShowDataListWnd = FALSE;
					Resize();
					CRect ViewRect(0,0,0,0);
					ViewRect = GetRect(ID_CHART);
					ShowDataListView(m_bShowDataListWnd, ViewRect);
				}
			}
		}
		break;
		//2007.01.17 add by LYH  툴바버튼 동기화
	case  UMSG_SYNC_TOOLBAR:
		if(m_pDlgToolbar && m_pDlgToolbar->GetSafeHwnd())
		{
			if(lParam == 50000)
				m_pDlgToolbar->ResetAllButton();
			else
				m_pDlgToolbar->SetToggleButton(wParam, lParam);
		}

		break;
	case WMU_GET_KEY_INFO_ARRAY:
		if( m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd() )
		{
			//m_pInputBarDlg->m_ctrlCodeInput.OnGetKeyInfoArray(wParam, lParam);
		}
		break;
// vntsorl:[0001250] [5001] 종합차트 - 일주월분초틱 주기별로 지표 설정 및 설정값을 다르게 할 수 있도록 개선요청
	//201105002 이문수 >>
	case UMSG_GET_COMMON_DATA:
		{
			return m_CommonData.GetCommonDataInfo(wParam, lParam);
		}
		break;
	case UMSG_SET_COMMON_DATA:
		{
			return m_CommonData.SetCommonDataInfo(wParam, lParam);
		}
		break;
	case UMSG_IS_PARENT_STDDIALOG:
		{
			return (LONG)this;
		}
		break;
	//201105002 이문수 <<
	}

	//20220117 이문수 >>
	if (RMSG_GET_STDDLG_PTR == message)
	{
		return (LRESULT)this;
	}
	//20220117 이문수 <<
	return CExOCXDLL::WindowProc(message, wParam, lParam);
}

void CStdDialog::InitDrds()
{
	m_nDrdsInfoCnt = 8;

	m_DrdsInfo[0].DataName = NAME_KXS3;	// 주식 (KXS3)
	m_DrdsInfo[0].FieldName = "*";
	m_DrdsInfo[0].KeyName = "";
	m_DrdsInfo[0].nID = 0;
	m_DrdsInfo[0].bAdvise = FALSE;

	m_DrdsInfo[1].DataName = NAME_EWs3;	// ELW (EWs3)
	m_DrdsInfo[1].FieldName = "*";
	m_DrdsInfo[1].KeyName = "";
	m_DrdsInfo[1].nID = 0;
	m_DrdsInfo[1].bAdvise = FALSE;

	m_DrdsInfo[2].DataName = NAME_IXIJ;	// 업종 (IXIJ)
	m_DrdsInfo[2].FieldName = "*";
	m_DrdsInfo[2].KeyName = "";
	m_DrdsInfo[2].nID = 0;
	m_DrdsInfo[2].bAdvise = FALSE;

	m_DrdsInfo[3].DataName = NAME_FFC0;	// 선물 (FFC0)
	m_DrdsInfo[3].FieldName = "*";
	m_DrdsInfo[3].KeyName = "";
	m_DrdsInfo[3].nID = 0;
	m_DrdsInfo[3].bAdvise = FALSE;

	m_DrdsInfo[4].DataName = NAME_OOC0;	// 옵션 (OOC0)
	m_DrdsInfo[4].FieldName = "*";
	m_DrdsInfo[4].KeyName = "";
	m_DrdsInfo[4].nID = 0;
	m_DrdsInfo[4].bAdvise = FALSE;

	m_DrdsInfo[5].DataName = NAME_JFC0;	// 주식선물 (JFC0)
	m_DrdsInfo[5].FieldName = "*";
	m_DrdsInfo[5].KeyName = "";
	m_DrdsInfo[5].nID = 0;
	m_DrdsInfo[5].bAdvise = FALSE;

	m_DrdsInfo[6].DataName = NAME_JOC0;	// 주식옵션 (JOC0)
	m_DrdsInfo[6].FieldName = "*";
	m_DrdsInfo[6].KeyName = "";
	m_DrdsInfo[6].nID = 0;
	m_DrdsInfo[6].bAdvise = FALSE;

	m_DrdsInfo[7].DataName = NAME_HWFJ;	// 해외 (HWFJ)
	m_DrdsInfo[7].FieldName = "*";
	m_DrdsInfo[7].KeyName = "";
	m_DrdsInfo[7].nID = 0;
	m_DrdsInfo[7].bAdvise = FALSE;

	m_DrdsInfo[8].DataName = NAME_GTFJ;	// 상품선물 (GTFJ)
	m_DrdsInfo[8].FieldName = "*";
	m_DrdsInfo[8].KeyName = "";
	m_DrdsInfo[8].nID = 0;
	m_DrdsInfo[8].bAdvise = FALSE;

	m_DrdsInfo[9].DataName = NAME_FOJS;	// FO 지수 (FOJS)
	m_DrdsInfo[9].FieldName = "*";
	m_DrdsInfo[9].KeyName = "";
	m_DrdsInfo[9].nID = 0;
	m_DrdsInfo[9].bAdvise = FALSE;

// 	m_DrdsInfo[8].DataName = NAME_KXYS;	// 주식예상 (KXYS)
// 	m_DrdsInfo[8].FieldName = "*";
// 	m_DrdsInfo[8].KeyName = "";
// 	m_DrdsInfo[8].nID = 0;
// 	m_DrdsInfo[8].bAdvise = FALSE;

/*
	m_DrdsInfo[9].DataName = NAME_EWs3;	// ELW (EWs3)
	m_DrdsInfo[9].FieldName = "*";
	m_DrdsInfo[9].KeyName = "";
	m_DrdsInfo[9].nID = 0;
	m_DrdsInfo[9].bAdvise = FALSE;
*/
}

BOOL CStdDialog::MakeDrdsConnectAdviseFromCode(int nI, BOOL bAddMod)
{	
	char* DataName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].DataName;
	char* FieldName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].FieldName;
	char* KeyName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].KeyName;

	UINT	nID = m_DrdsInfo[nI].nID;
	IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager);
	if(!pDrdsMng) return FALSE;

	if(m_pDrdsLib == NULL)
		m_pDrdsLib = pDrdsMng->AdviseDrdsLib();

	if(!m_pDrdsLib) return FALSE;

	CString strType("1");
	ST_CODEDATA oneCode;
	if(nI == 1)//업종이면
	{
		if(m_DrdsInfo[nI].KeyName.GetLength() == UPJONG_LENGTH)
		{
			oneCode.m_bType = atoi(m_DrdsInfo[nI].KeyName.Left(1));
			oneCode.m_strCode = m_DrdsInfo[nI].KeyName;
		}
	}
	else
	{
		oneCode.m_strCode = m_DrdsInfo[nI].KeyName;
		if(m_pInputBarDlg && m_pInputBarDlg->m_pDataManager)
		{
			int nCodeType;
			oneCode.m_bType = GetMarketTypeFromCode(oneCode.m_strCode, nCodeType);// m_pDataManager->GetCodeInfo("", 2, oneCode.m_strCode);
		}
		else
			oneCode.m_bType = atoi(strType);
	}
	
/*
	if(nI == 0)
		m_ListStockRealCodes.AddTail(oneCode);
	else if(nI == 1)
		m_ListUpjongRealCodes.AddTail(oneCode);
	else if(nI == 2 || nI == 3)
		m_ListFutOptRealCodes.AddTail(oneCode);
	else if(nI == 4)
		m_ListStockFutOptRealCodes.AddTail(oneCode);
	else if(nI == 6)
		m_ListForeignRealCodes.AddTail(oneCode);
*/

	// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
//	if(bAddMod)
	CString strRealKey;
	strRealKey.Format("%s:%s", DataName, KeyName);
//		m_RealTrCounter.AddTr(oneCode.m_strCode);
	if(m_RealTrCounter.AddTr(strRealKey) == 1)
	{
		//@Solomon-->
		//m_pDrdsLib->DrdsInit(m_hWnd);
		m_pDrdsLib->DrdsAdvise(nID, DataName, FieldName, KeyName);
		if(m_pPTManager) m_pPTManager->RequestMChartReal('A', DataName, KeyName);
		//@Solomon<--
		m_DrdsInfo[nI].bAdvise = TRUE;
	}

	return TRUE;
}

// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
void CStdDialog::AddRealCodeList(LPCTSTR lpszCode)
{
/*
	ST_CODEDATA oneCode;
	oneCode.m_strCode = lpszCode;

	int nCodeType;
	int nMarketType = GetMarketTypeFromCode(lpszCode, nCodeType);
	BOOL bRet = FALSE;
	if(nMarketType == STOCK_CHART)
	{
		m_ListStockRealCodes.AddTail(oneCode);
	}
	else if(nMarketType == UPJONG_CHART)
	{
		m_ListUpjongRealCodes.AddTail(oneCode);
	}
	else if(nMarketType == FUTOPT_CHART)
	{
		m_ListFutOptRealCodes.AddTail(oneCode);
	}
	else if(nMarketType == STOCKFUTOPT_CHART)
	{
		m_ListStockFutOptRealCodes.AddTail(oneCode);
	}
	else if(nMarketType == FOREIGN_CHART)
	{
		m_ListForeignRealCodes.AddTail(oneCode);
	}
*/
	m_RealTrCounter.AddTr(lpszCode);
}

void CStdDialog::RemoveDrdsFromCode(LIST_CODEDATA *pListRealCodes, CString strCode/*=""*/)
{
	POSITION pos = NULL;
	int nCount = pListRealCodes->GetCount();
	if(strCode.GetLength() > 0)
	{
		for(int nIndx = 0; nIndx < nCount; ++nIndx)
		{
			pos = pListRealCodes->FindIndex(nIndx);
			if(strCode == pListRealCodes->GetAt(pos).m_strCode)
			{
				if(m_pDrdsLib && pos)
				{
					//@Solomon
					m_pDrdsLib->DrdsUnadvise(0, "", "", (LPSTR)(LPCSTR)strCode);
					if(m_pPTManager) m_pPTManager->RequestMChartReal('U', "", strCode);
					pListRealCodes->RemoveAt(pos);
				}
				break;
			}
		}
	}
	else
	{
		for(int nIndx = 0; nIndx < nCount; ++nIndx)
		{
			pos = pListRealCodes->FindIndex(nIndx);
			if(m_pDrdsLib && pos)
			{
				//@Solomon
				m_pDrdsLib->DrdsUnadvise(0, "", "", (LPSTR)(LPCSTR)pListRealCodes->GetAt(pos).m_strCode);
				if(m_pPTManager) m_pPTManager->RequestMChartReal('U', "", pListRealCodes->GetAt(pos).m_strCode);
				pListRealCodes->RemoveAt(pos);
			}
		}
		pListRealCodes->RemoveAll();
	}
}

BOOL CStdDialog::MakeDrdsUnConnectUnAdviseFromCode(int nI, BOOL bAddMod)
{
	IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager);
	if (!pDrdsMng) return FALSE;

	if (m_pDrdsLib == NULL)
		return TRUE;

	char* DataName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].DataName;
	char* FieldName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].FieldName;
	char* KeyName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].KeyName;
	UINT nID = m_DrdsInfo[nI].nID;

	CString strRealKey;
	strRealKey.Format("%s:%s", DataName, KeyName);
//	if(m_RealTrCounter.RemoveTr(m_DrdsInfo[nI].KeyName) == 0)
	if(m_RealTrCounter.RemoveTr(strRealKey) == 0)
	{
		//@Solomon
		m_pDrdsLib->DrdsUnadvise(nID, DataName, FieldName, KeyName);
		if(m_pPTManager) m_pPTManager->RequestMChartReal('U', DataName, KeyName);
		m_DrdsInfo[nI].bAdvise = FALSE;
	}

	//{{2007.01.02 by LYH 리얼등록
/*
	if(nI == 0)
		RemoveRealCodes(&m_ListStockRealCodes, m_DrdsInfo[nI].KeyName);
	else if(nI == 1)
		RemoveRealCodes(&m_ListUpjongRealCodes, m_DrdsInfo[nI].KeyName);
	else if(nI == 2 || nI == 3)
		RemoveRealCodes(&m_ListFutOptRealCodes, m_DrdsInfo[nI].KeyName);
	else if(nI == 4)
		RemoveRealCodes(&m_ListStockFutOptRealCodes, m_DrdsInfo[nI].KeyName);
	else if(nI == 6)
		RemoveRealCodes(&m_ListForeignRealCodes, m_DrdsInfo[nI].KeyName);
*/
	//}}

	// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
//	if(bAddMod)
//		m_RealTrCounter.RemoveTr(m_DrdsInfo[nI].KeyName);

	return TRUE;
}

// 복수종목 실시간 처리 : 복수종목 - ojtaso (20070109)
void CStdDialog::OnCodeUnAdvise(CString strCode, BOOL bAddMode)
{
	if(IsChartScreen("OldFuture")==TRUE) return;	//과거종목은  skip

	// 기존 코드가 리스트에 있으면 삭제
	// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
	int nCodeType;
	int nMarketType = GetMarketTypeFromCode(strCode, nCodeType);
	if(nMarketType == CMasterTypeInfo::STOCK_CHART)
	{
		if(nCodeType == CDRCodeTypeInfo::CODE_KOSPI_STOCK || nCodeType == CDRCodeTypeInfo::CODE_KOSDAQ_STOCK)
		{
			m_DrdsInfo[0].KeyName = strCode;
			//@Solomon m_DrdsInfo[8].KeyName = strCode;
			MakeDrdsUnConnectUnAdviseFromCode(0, bAddMode);
			//@Solomon MakeDrdsUnConnectUnAdviseFromCode(8, bAddMode);
		}
		else if(nCodeType == CDRCodeTypeInfo::CODE_ELW)
		{
			m_DrdsInfo[1].KeyName = strCode;
			MakeDrdsUnConnectUnAdviseFromCode(1, bAddMode);
		}
	}
	else if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
	{
		m_DrdsInfo[2].KeyName = strCode;
		MakeDrdsUnConnectUnAdviseFromCode(2, bAddMode);
		// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
	}
	else if(nMarketType == CMasterTypeInfo::FUTOPT_CHART || nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
	{
		if(nCodeType == CDRCodeTypeInfo::CODE_KFUTURE)
		{
			// 연결선물일 경우 최근월물로 변경
			if(strCode.CompareNoCase(DEF_CONTINUE_FUT_CODE) == 0 || strCode.CompareNoCase(DEF_CONTINUE_MINI_FUT_CODE) == 0 )
				IsContinueFutCode(strCode);
			m_DrdsInfo[3].KeyName = strCode;
			MakeDrdsUnConnectUnAdviseFromCode(3, bAddMode);								
		}
		else if(nCodeType == CDRCodeTypeInfo::CODE_KOPTION)
		{
			m_DrdsInfo[4].KeyName = strCode;
			MakeDrdsUnConnectUnAdviseFromCode(4, bAddMode);								
		}
		else if(nCodeType == CDRCodeTypeInfo::CODE_STOCKFUT)
		{
			// 연결선물일 경우 최근월물로 변경
//			IsContinueFutCode(strCode);
			m_DrdsInfo[5].KeyName = strCode;
			MakeDrdsUnConnectUnAdviseFromCode(5, bAddMode);
		}
		else if(nCodeType == CDRCodeTypeInfo::CODE_STOCKOPT)
		{
			m_DrdsInfo[6].KeyName = strCode;
			MakeDrdsUnConnectUnAdviseFromCode(6, bAddMode);
 		}
	}
	else if(nMarketType == CMasterTypeInfo::FOREIGN_CHART)
	{
		m_DrdsInfo[7].KeyName = strCode;
		MakeDrdsUnConnectUnAdviseFromCode(7, bAddMode);							
		// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
	}
	//20110124_alzioyes:종목해지시의 비교루틴 오류수정.
	//else if(nCodeType == CMasterTypeInfo::FUOPT_JIPYO_CHART)
	else if(nMarketType == CMasterTypeInfo::FUOPT_JIPYO_CHART)
	{
		m_DrdsInfo[9].KeyName = strCode;
		MakeDrdsUnConnectUnAdviseFromCode(9, bAddMode);
	}
	//20110124_alzioyes:종목해지시의 비교루틴 오류수정.
	//else if(nCodeType == CMasterTypeInfo::COMMODITY_CHART)
	else if(nMarketType == CMasterTypeInfo::COMMODITY_CHART)
	{
		//[A00000546]연결선물 리얼안되는 부분 수정.
		// 연결선물일 경우 최근월물로 변경
//		IsContinueFutCode(strCode);
		m_DrdsInfo[8].KeyName = strCode;
		MakeDrdsUnConnectUnAdviseFromCode(8, bAddMode);
	}
}

// 복수종목 실시간 처리 : 복수종목 - ojtaso (20070109)
void CStdDialog::OnCodeAdvise(CString strCode, BOOL bAddMode)
{
	if(IsChartScreen("OldFuture")==TRUE) return;	//과거종목은  skip

	// Drds 연결 
	// 기존 코드가 리스트에 없으면 추가
	// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
	int nCodeType;
	int nMarketType = GetMarketTypeFromCode(strCode, nCodeType);
	if(nMarketType == CMasterTypeInfo::STOCK_CHART)
	{
		if(nCodeType == CDRCodeTypeInfo::CODE_KOSPI_STOCK || nCodeType == CDRCodeTypeInfo::CODE_KOSDAQ_STOCK)
		{
			m_DrdsInfo[0].KeyName = strCode;
			//@Solomon m_DrdsInfo[8].KeyName = strCode;
			MakeDrdsConnectAdviseFromCode(0, bAddMode);
			//@Solomon MakeDrdsConnectAdviseFromCode(8, bAddMode);
		}
		else if(nCodeType == CDRCodeTypeInfo::CODE_ELW)
		{
			m_DrdsInfo[1].KeyName = strCode;
			MakeDrdsConnectAdviseFromCode(1, bAddMode);
		}
	}
	else if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
	{
		m_DrdsInfo[2].KeyName = strCode;
		MakeDrdsConnectAdviseFromCode(2, bAddMode);
	}
	else if(nMarketType == CMasterTypeInfo::FUTOPT_CHART || nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
	{
		if(nCodeType == CDRCodeTypeInfo::CODE_KFUTURE)
		{
			// 연결선물일 경우 최근월물로 변경
			if(strCode.CompareNoCase(DEF_CONTINUE_FUT_CODE) == 0 || strCode.CompareNoCase(DEF_CONTINUE_MINI_FUT_CODE) == 0 )
				IsContinueFutCode(strCode);
			m_DrdsInfo[3].KeyName = strCode;
			MakeDrdsConnectAdviseFromCode(3, bAddMode);
		}
		else if(nCodeType == CDRCodeTypeInfo::CODE_KOPTION)
		{
			m_DrdsInfo[4].KeyName = strCode;
			MakeDrdsConnectAdviseFromCode(4, bAddMode);
		}
		else if(nCodeType == CDRCodeTypeInfo::CODE_STOCKFUT)
		{
			// 연결선물일 경우 최근월물로 변경
//			IsContinueFutCode(strCode);
			m_DrdsInfo[5].KeyName = strCode;
			MakeDrdsConnectAdviseFromCode(5, bAddMode);
		}
		else if(nCodeType == CDRCodeTypeInfo::CODE_STOCKOPT)
		{
			m_DrdsInfo[6].KeyName = strCode;
			MakeDrdsConnectAdviseFromCode(6, bAddMode);
		}
	}
	else if(nMarketType == CMasterTypeInfo::FOREIGN_CHART)
	{
		m_DrdsInfo[7].KeyName = strCode;
		MakeDrdsConnectAdviseFromCode(7, bAddMode);
	}
 	else if(nMarketType == CMasterTypeInfo::COMMODITY_CHART)
 	{
		//[A00000546]연결선물 리얼안되는 부분 수정.
		// 연결선물일 경우 최근월물로 변경
//		IsContinueFutCode(strCode);
		m_DrdsInfo[8].KeyName = strCode;
		MakeDrdsConnectAdviseFromCode(8, bAddMode);
 	}
	else if(nMarketType == CMasterTypeInfo::FUOPT_JIPYO_CHART)
	{
		m_DrdsInfo[9].KeyName = strCode;
		MakeDrdsConnectAdviseFromCode(9, bAddMode);
	}
}

BOOL CStdDialog::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)  
	{		
		if( ::GetKeyState(VK_CONTROL) & 0x8000)
			return FALSE;

		UINT nChar = (UINT)pMsg->wParam;
		switch(nChar)
		{
		case VK_RETURN :
		case VK_ESCAPE :
		case VK_TAB:
		case VK_HOME:
		case VK_ADD:
			{
				return FALSE;
			}
		}
	}
	else if(pMsg->message == WM_SYSKEYDOWN)
	{
		if( ::GetKeyState(VK_MENU) & 0x8000)
			return FALSE;
	}
	else if(pMsg->message == WM_LBUTTONDOWN ||
			pMsg->message == WM_LBUTTONUP ||
			pMsg->message == WM_LBUTTONDBLCLK||
			pMsg->message == WM_RBUTTONDOWN ||
			pMsg->message == WM_RBUTTONUP ||
			pMsg->message == WM_RBUTTONDBLCLK ||
			pMsg->message == WM_MOUSEMOVE)
	{
		if(m_pDlgGlass && m_pDlgGlass->IsWindowVisible())
		{
			ScreenToClient( &pMsg->pt);

			CRect rcGlass;
			m_pDlgGlass->GetWindowRect( &rcGlass);
			ScreenToClient( &rcGlass);
			if(!rcGlass.PtInRect(pMsg->pt))
				return TRUE;
		}
	}

	return CExOCXDLL::PreTranslateMessage(pMsg);
}

void CStdDialog::OnSize(UINT nType, int cx, int cy) 
{
	CExOCXDLL::OnSize(nType, cx, cy);
	
	if(m_bMinimizedStart && cx > 0 && cy > 0)
	{
		m_bMinimizedStart = FALSE;

		FormLoad();
		if( !m_bInitForm )	return;

		m_ChartMng.LoadChart(m_strConfig);

		//100530:동일화면 다르게 저장/읽기
		CString szSaveKey;
		GetFileSaveKey(szSaveKey);
		//	m_strConfig = m_strUserDir + "\\" + GetScreenNo() + "_" + ENVINFOFILE;
		m_strConfig = m_strUserDir + "\\" + szSaveKey + "_" + ENVINFOFILE;

		{
			CString s;
			s.Format("\r\n[CStdDialog::OnSize] m_strConfig=%s\r\n", m_strConfig);
			OutputDebugString(s);
		}
	}

	if(GetSafeHwnd())
	{
		Resize();			
	}	
}

void CStdDialog::SetSmallView(BOOL bSmall /*= TRUE*/)
{
	if(bSmall)
	{
		if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
			m_pInputBarDlg->HideLeftBarBtn(SW_HIDE);
		m_nTempLeftBarType = GetLeftBarType();
		SetLeftBarType(0);
		m_bSmallScreenMode = TRUE;
	}
	else
	{
		if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
			m_pInputBarDlg->HideLeftBarBtn(SW_SHOW);
		SetLeftBarType(m_nTempLeftBarType);
		m_bSmallScreenMode = FALSE;
	}
}


void CStdDialog::InputCodeFromOutSide(CString strPacketFromOutSide)
{
	if(GetLeftBarType()==0)
	{
		SetLeftBarType(1);
	}
	if(m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd())
		m_pLeftBarDlg->SetPacketFromOutSide(strPacketFromOutSide);
}

void CStdDialog::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent==2)	
	{	
		KillTimer(nIDEvent);
	//KHD	if (m_nChartType == MULTI_CHART) // 요청할 필요 없음 
	//KHD		RequestDollarData();
		_DoFirstRequest(); // StdDialogPartnerSite.cpp에서 구현함.
	}
	else if (nIDEvent == 3)
	{
		// 처음 로딩속도관련 하여 Timer로 작성.
		KillTimer(nIDEvent);

	//	if (m_nChartType == MULTI_CHART)
	//		RequestDollarData();

		if(m_nScreenID == atoi("7111") || m_nScreenID == atoi("7112"))	// 종합, 주식종합차트
			RequestMstCode();
	}
	if(nIDEvent==1)	
	{
		KillTimer(nIDEvent);

		ClearSaveKey();	//100530:동일화면 다르게 저장/읽기
		m_arryQuery.RemoveAll();

		CString strMessage;
		if(m_lstSendData.IsEmpty())
		{
//			strMessage.Format("조회처리에 실패하였습니다.(TimeOut)");
		}
		else
		{
			CString strItemCode("종목아님");

			STSENDDATAQUE* pSendData = m_lstSendData.GetTail();
			if(pSendData)
			{
				if(pSendData->strTRCode == QUERY_STRJONGMOK)
				{
					KB_p0602_InRec1* pData = (KB_p0602_InRec1*)pSendData->lpData;
					strItemCode.Format(_T("%.*s"), sizeof(pData->shcode), pData->shcode);
				}
				else if(pSendData->strTRCode == QUERY_STRUPJONG)
				{
					KB_p0603_InRec1* pData = (KB_p0603_InRec1*)pSendData->lpData;
					strItemCode.Format(_T("%.*s"), sizeof(pData->Supcode), pData->Supcode);
				}
				else if((pSendData->strTRCode == QUERY_STRFUTOPT) ||
						(pSendData->strTRCode == QUERY_STRPREFUTOPT))
				{
					KB_p0604_InRec1* pData = (KB_p0604_InRec1*)pSendData->lpData;
					strItemCode.Format(_T("%.*s"), sizeof(pData->Scode), pData->Scode);
				}
				else if(pSendData->strTRCode == QUERY_STRSTOCKFUTOPT)
				{
					KB_d1013_InRec1* pData = (KB_d1013_InRec1*)pSendData->lpData;
					strItemCode.Format(_T("%.*s"), sizeof(pData->Scode), pData->Scode);
				}
				else if(pSendData->strTRCode == QUERY_STRFOREIGN)
				{
					KB_p0606_InRec1* pData = (KB_p0606_InRec1*)pSendData->lpData;
					strItemCode.Format(_T("%.*s"), sizeof(pData->Shcode), pData->Shcode);
				}
				else if(pSendData->strTRCode == QUERY_STRMARKET)
				{
					KB_d1015_InRec1* pData = (KB_d1015_InRec1*)pSendData->lpData;
					strItemCode.Format(_T("%.*s"), sizeof(pData->shcode), pData->shcode);
				}
				else if(pSendData->strTRCode == QUERY_STRFOJIPYO)
				{
					KB_d1015_InRec1* pData = (KB_d1015_InRec1*)pSendData->lpData;
					strItemCode.Format(_T("%.*s"), sizeof(pData->shcode), pData->shcode);
				}

				strItemCode.TrimLeft(); strItemCode.TrimRight();
			}
			
//			strMessage.Format("조회처리에 실패하였습니다.(TimeOut) TR:%s Code:%s", pSendData->strTRCode, strItemCode);

			m_bIsWaitRequest = FALSE;
			::memset(&m_stTrInfo2, 0x00, sizeof(m_stTrInfo2));

			POSITION pos = m_lstSendData.GetHeadPosition();
			while(pos)
			{
				STSENDDATAQUE* pSendData = m_lstSendData.GetNext(pos);
				::free(pSendData->lpData);
				delete pSendData;
			}

			m_lstSendData.RemoveAll();

			m_ChartMng.ReplaceOldCode();
		}

		g_logMan.SaveLogFile(m_strUserDir);

		EnableWindowAll();
		strMessage.Format("데이타 수신이 지연되고 있습니다.화면을 재실행해 주세요.");
		AfxMessageBox(strMessage);
	}
	
	CDialog::OnTimer(nIDEvent);
}


HRESULT CStdDialog::RecieveError(int nError, LPCSTR szMessage)
{
	MessageBox(szMessage);
	return S_OK;
}

CString CStdDialog::GetMapID()
{
	return m_strMapID;
}


BOOL CStdDialog::IsChartHide()
{
	return m_bHideChart;
}

BOOL CStdDialog::HideChart()
{
	m_bHideChart=!m_bHideChart;
	
	CString strSize;
	CRect rect;
	CRect rectOriginal;
	if(m_bHideChart)
	{
		GetClientRect(rectOriginal);
		m_lWidthOriginal = rectOriginal.Width()+1;
		m_lHeightOriginal = rectOriginal.Height()+1;
		if( m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd() )
			m_pLeftBarDlg->GetClientRect(rect);
		strSize.Format("%d,%d,",rect.Width(),rect.Height());
	}
	else
	{
		if(!m_lWidthOriginal||!m_lHeightOriginal)
		{
			m_lWidthOriginal = m_nWidthOfDlg;
			m_lHeightOriginal = m_nHeightOfDlg;
		}
		strSize.Format("%d,%d,",m_lWidthOriginal,m_lHeightOriginal);
	}	
	CWnd* pWnd = GetParent();	
	if (pWnd)// && m_pBaseDesk)
	{
		//화면에 Data를 보내준다.
		CString  csData;
		csData.Format("Dim szText, szValue\n" "szText=\"%s\"\n" "szValue=\"%s\"\n", "LOCAL_RESIZE", strSize);
	}
	return m_bHideChart;
}

void	CStdDialog::SetJongListPrev()
{
	if( m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd() )
	{
		CTabCodeSearch *pTabCodeSearch = (CTabCodeSearch*)m_pLeftBarDlg->m_tabctrl.GetWndCodeSearch();
		if(pTabCodeSearch)
		{
			pTabCodeSearch->OnBtprev();
		}	
	}
}

void	CStdDialog::SetJongListNext()
{
	if( m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd() )
	{
		CTabCodeSearch *pTabCodeSearch = (CTabCodeSearch*)m_pLeftBarDlg->m_tabctrl.GetWndCodeSearch();
		if(pTabCodeSearch)
		{
			pTabCodeSearch->OnBtnext();
		}	
	}
}

CString CStdDialog::GetAccountInfo() 
{ 
	return m_strAccountInfo;
}

BOOL CStdDialog::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
//	if(!m_bEnable)
//	{
//		//@Solomon 
//		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
//		return TRUE;
//	}
//	else
//	{
//		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW)); //@.Solomon
//	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CStdDialog::SetInitInfo()
{
	int nID = 21;
	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
	if(pGateMng == NULL)
	{
		return;
	}
	IMainInfoManager* pManager = (IMainInfoManager*)pGateMng->GetInterface(nID);

	// UserID
	m_strUserID = pManager->GetEncUserID();
	// KB 포팅중 수정 - ojtaso (20080901)
/*
	if(m_strUserID.GetLength() == 0)
	{
		AfxMessageBox("UserID를 얻을 수 없습니다.");
	}
*/

	// Root Path
	m_strRootPath = pManager->GetRootDir();
	if(m_strRootPath.GetLength()==0)
	{
		AfxMessageBox("RootPath를 얻을 수 없습니다.");
	}

	// User Path 
	m_strUserDir.Format("%s%s", pManager->GetEncUserDir() , "\\Chart");
	_DoMakeUserExtraDir(m_strUserDir);

	_DoVersionCheck_ChartDef();
	_DoCheckDaultDataFile();

	// Image Path
	//solomon-090825
	m_szImageDir = m_strRootPath + "\\Icss\\Image\\drchart";

#ifdef __MULTI_LANG__
	//@Solomon-MultiLang:091117
	g_iMetaTable.m_strDataFolderPath.Format(_T("%s\\"), pManager->GetDataDir());
	//KHD : 경로 재설정 : OCX가 기존에 등록되어 있고, 다른 경로의 HTS를 로딩하고 ,Data 폴더를 읽어올때 기존 경로로 읽어오기에
	 // 메타테이블을 제대로 읽어오지 못할때가 있다. 따라서, 명시적으로 경로를 재설정함 
	GetDir_DrfnBin(g_iMetaTable.m_strHtsFolderPath); 
	g_iMetaTable.m_strHtsFolderPath+="\\";
	g_iMetaTable.LoadMetaTable();

#endif
}

void CStdDialog::GetIndicatorNameListIntoComboBox(CComboBox& rComboBox,LPCTSTR p_strGroup)
{
#ifdef _DEBUG
	VERIFY(m_pLeftBarDlg);
	VERIFY(m_pLeftBarDlg->GetSafeHwnd());
	VERIFY(rComboBox.GetSafeHwnd());
#endif
	if(m_pLeftBarDlg)
	{
		if(m_pLeftBarDlg->GetSafeHwnd())
		{
			m_pLeftBarDlg->GetIndicatorNameListIntoComboBox(rComboBox, p_strGroup);
		}		
	}	
}

#include <atlconv.h>
void CStdDialog::ShowDataListView(BOOL bShowDataListView, CRect ViewRect)
{
	if(bShowDataListView)
	{	
		//혹시 Window가 있다면 지운다.
		DestroyDataWindow();		

		if(m_pDataWindow != NULL)				
			m_pDataWindow = NULL;	

		if(m_pDataWindow == NULL)
		{
			m_pDataWindow = new CDataWindowDlg();
			m_pDataWindow->CreateDataWindow();
			m_pDataWindow->SetParent((CStdDialog*)this);
			m_pDataWindow->MoveWindow(&m_ViewRect);
			m_pDataWindow->ShowWindow(SW_SHOW);
		}
		m_bActiveDataWindow = true;
	}
	else
	{
		DestroyDataWindow();
	}
}

static int ConvertToChartSign(char* pBuff)
{	
	CString sTemp;
	sTemp.Format("%1.1s", pBuff);
	int nSign = 0;
	nSign = atoi((LPTSTR)(LPCTSTR)sTemp);

	int nResult = 0;
	switch(nSign)
	{
		case 1:		//상한
			nResult =  7; break;
		case 2:		//상승
			nResult =  6; break;
		case 3:		//보합
			nResult =  5; break;
		case 4:		//하한
			nResult =  3; break;
		case 5:		//하락
			nResult =  4; break;
	}
	char buff[1];
	itoa(nResult, buff, 10);
	memcpy(pBuff, buff, 1);	

	return 0;
}

void CStdDialog::RequestDollarData()
{
	CString strTrInfo;
	strTrInfo.Format("%-10s", "USDKRWCOMP");

	stSend2Server st;
	st.m_nType = 0;
	st.m_pWnd = (CWnd*)this;
	st.m_pBuffer = (void*)(LPCTSTR)strTrInfo;
	st.m_lLength= strTrInfo.GetLength();
	st.m_strTRCode = QUERY_STRDOLLAR;	
	m_TRComm->SetTR(QUERY_STRDOLLAR);
	m_TRComm->SetBaseData("PtManager", m_szPTManager);
	m_TRComm->SetBaseData("SCREENCODE", GetScreenNo());
	m_TRComm->Send2Server(st.m_pBuffer, st.m_lLength, TRKEY_FID_END);	
}

void CStdDialog::RequestWorkDate()
{	return ;//KHD
	CString strTrInfo;
	strTrInfo.Format("%04d", 42);	//조건검색에서 42일조회.

	Send2Server(TRNO_WORKDATE, (LPCSTR)(LPCTSTR)strTrInfo, strTrInfo.GetLength(), FALSE);
}

void CStdDialog::RequestMstCode()
{
	CTabCodeSearch *pTabCodeSearch = (CTabCodeSearch*)m_pLeftBarDlg->m_tabctrl.GetWndCodeSearch();
	if (pTabCodeSearch != NULL)
	{
		int nPos = 0;
		char szSaveBuf[24576] = { 0 };
		//1. 연속패킷, DRFN 연속키값 쓸필요있나???, 그련 space(80)
		memset(szSaveBuf+nPos, 0x20, 80);
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

//		CString strTrInfo;
//		strTrInfo.Format("%04d", 0);
//		strTrInfo = "    ";
		pTabCodeSearch->RequestMstCode(szSaveBuf, sizeof(int));
	}
}

int CStdDialog::GetFillString(CString& _szGetStr, int nStart, int nEnd, int nOneWordSize)
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

void CStdDialog::Request_StockHogaData(CString strRecvCode)
{
	if (m_ChartMng.m_pFloatingDlg == NULL) return;
	if (m_ChartMng.m_pFloatingDlg->GetSafeHwnd() == NULL ) return;
	if (m_ChartMng.m_pFloatingDlg->m_bIconState) return; //축소 상태면 TR조회 안한다.

	if (m_pInputBarDlg == NULL) return;
	if (m_pInputBarDlg->GetSafeHwnd() == NULL) return;


	CString strCode;
	if (strRecvCode.GetLength() == CODE_STOCK_LENGTH)
		strCode = strRecvCode;
	else
		strCode = m_pInputBarDlg->m_pCodeInput->GetCode();

	if (!strCode.GetLength())
	{
		//호가창 Data Clear & Real 해제 ===================================
		Request_Disconnect_StockRealHogaData(m_pInputBarDlg->m_strOldCode);
		return;
		//================================================================
	}

	if(m_ChartMng.m_pFloatingDlg != NULL)
	{
		if(m_ChartMng.m_pFloatingDlg->GetSafeHwnd())
		{	
			// Real 해제 ====================================================================
			if(m_pInputBarDlg->m_strOldCode != strCode)
				Request_Disconnect_StockRealHogaData(m_pInputBarDlg->m_strOldCode);
			// Real 해제 ====================================================================

			if(strCode.GetLength() != CODE_STOCK_LENGTH)
				return;

//@유진변경
			stSend2Server st;
			st.m_nType = 0;
			st.m_pWnd = (CWnd*)m_ChartMng.m_pFloatingDlg;
			st.m_pBuffer = (void*)(LPCTSTR)strCode;
			st.m_lLength= strCode.GetLength();
			st.m_strTRCode = QUERY_STOCK_HOGA;	
			SendMessage(UMSG_SEND2SERVER,0,(long)&st);
//@유진추가
			//2005. 07. 15
			Request_StockRealHogaData(strCode);
		}
	}
}

void CStdDialog::Request_StockRealHogaData(CString strCode)
{
	// 주식 Real Hoga 요청 ====================================================================
	if(strCode.GetLength())
	{
		//if(m_pDBMgr)
		{				
			strCode.TrimLeft();
			strCode.TrimRight();
			m_DrdsInfo[4].KeyName = strCode;
			MakeDrdsConnectAdviseFromCode(4, FALSE);		// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
		}
	}
}

void CStdDialog::Request_Disconnect_StockRealHogaData(CString strCode)
{
#ifdef _WRITELOG
	WriteToStringLog("CStdDialog::Request_Disconnect_StockRealHogaData::Begin", _T("멀티차트 종료"), 13);
#endif
	if(m_ChartMng.m_pFloatingDlg != NULL)
	{
		if(m_ChartMng.m_pFloatingDlg->GetSafeHwnd())
		{	
			// Real 해제 ====================================================================
			if(strCode.GetLength())
			{
				m_ChartMng.m_pFloatingDlg->m_strCode.TrimLeft();
				m_ChartMng.m_pFloatingDlg->m_strCode.TrimRight();
				m_DrdsInfo[4].KeyName = m_ChartMng.m_pFloatingDlg->m_strCode;
				MakeDrdsUnConnectUnAdviseFromCode(4, FALSE);		// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
			}
			// Real 해제 ====================================================================
		}
	}
#ifdef _WRITELOG
	WriteToStringLog("CStdDialog::Request_Disconnect_StockRealHogaData::End", _T("멀티차트 종료"), 13);
#endif
}

void CStdDialog::CreateDataWindow()
{
	if(m_pDataWindow == NULL)
	{
		m_pDataWindow = new CDataWindowDlg();
		m_pDataWindow->CreateDataWindow();
	}
	m_bActiveDataWindow = true;
}

void CStdDialog::DestroyDataWindow()
{
	if(m_pDataWindow)
	{
		
		m_pDataWindow->DestroyWindow();
		delete m_pDataWindow;
	}
	m_pDataWindow = NULL;
	m_bActiveDataWindow = false;

}

bool CStdDialog::IsActiveDataWindow()
{
	return m_bActiveDataWindow;
}

void CStdDialog::SetDataWindowData(CList<CString, CString&>* pListData)
{
	m_pDataWindow->SetDataWindowData(pListData);
}

/// gm0604 Start 
void CStdDialog::InitChartBySystemTrading(LPCTSTR lpMessage)
{
	CString strMessage = lpMessage;
	CString strInfo;
	CString strTimeType;
	CString strDataCount;
	long lDataCount = 0;

	int nRow = 0;
	int nCol = 0;
	CString strInputCode;
	CString strEndDate;
	CString strPtrST;
	CString strName;


	// col, row, inputcode, enddate, timetype, datacnt,....
	int nShifter = 0;
	m_strInputCode = Parser(strMessage,&nShifter);
	m_strEndDate = Parser(strMessage,&nShifter);

	while(TRUE)
	{
		strTimeType = Parser(strMessage,&nShifter);
		strDataCount = Parser(strMessage,&nShifter);
		strPtrST = Parser(strMessage,&nShifter);
		strName = Parser(strMessage,&nShifter);
		if(strTimeType!=""&&strDataCount!=""&&strPtrST!=""&&strName!="")
		{
			m_stringListTimeType.AddTail(strTimeType);
			m_longListDataCount.AddTail(atol(strDataCount));
			m_ptrListSystemTrading.AddTail(atol(strPtrST));
			m_stringListSTName.AddTail(strName);
		}
		else
		{
			break;
		}
	}		
	m_ChartMng.SetSystemTradingMode(TRUE);
	SetChartBySystemTrading(TRUE);
}


void CStdDialog::SetChartBySystemTrading(BOOL bIsFirst)
{
	if(m_stringListTimeType.GetCount()==0)
	{		
		m_ChartMng.Invalidate();
		return;
	}

	if(!m_ChartMng.SelectEmptyPos())	
	{
		if(!m_ChartMng.InsertChart())
		{
			return ;
		}
		m_ChartMng.SelectEmptyPos();		
	}
	
	CString strTimeType = m_stringListTimeType.GetHead();
	m_stringListTimeType.RemoveHead();
	long lDataCount = m_longListDataCount.GetHead();
	m_longListDataCount.RemoveHead();
	long lPtrST = m_ptrListSystemTrading.GetHead();
	m_ptrListSystemTrading.RemoveHead();
	CString strSTName = m_stringListSTName.GetHead();
	m_stringListSTName.RemoveHead();


	// 틱(0)/분(1)/일(2)/주(3)/월(4)
	CString strNum;
	CString strType;
	char chTimeType = NULL;
	int nLength = strTimeType.GetLength();
	if(nLength==2)
	{
		if(strTimeType=="월")
		{
			chTimeType = MONTH_DATA_CHART;
		}
		else if(strTimeType=="주")
		{
			chTimeType = WEEK_DATA_CHART;
		}
		else if(strTimeType=="일")
		{
			chTimeType = DAY_DATA_CHART;
		}
	}
	else
	{
		strType = strTimeType.Right(2);
		strNum = strTimeType.Left(nLength-2);
		if(strType=="분")
		{
			chTimeType = MIN_DATA_CHART;
		}
		else if(strType=="틱")
		{
			chTimeType = TICK_DATA_CHART;
		}
	}
	CCodeDataFromInputBar CodeData;
	CodeData.m_strCode = m_strInputCode;
	CodeData.m_lEndTime = atol(m_strEndDate);
	CodeData.m_chType = chTimeType;
	CodeData.m_lTypeNum = atol(strNum);
	CodeData.m_lDataCnt = lDataCount;
	CodeData.m_lPtrST = lPtrST;
	CodeData.m_strSTName = strSTName;
	m_ChartMng.SendMessage(UMSG_INPUTCODEFROMINPUTBAR,(WPARAM)&CodeData);
	m_ChartMng.MoveCharts();
}

long CStdDialog::ChangeSharedData(CString sShareName, BYTE* pData, int nLen, int nStock/*=0*/)
{
	if(sShareName==FROMSTCENTER)
	{
		CString strData = (LPCTSTR)pData;
		if(strData=="#") return 0;
		CString strMessage = strData.Mid(3);
		int nCode = atol(strData.Left(3));

		switch(nCode)
		{
		case 2:	// Update SystemTrading Info
			{
				m_ChartMng.OnSTDataManagerMessaggeAll();
			}
			break;
		case 3:// Close MultiChart
			{
				m_ChartMng.SetSystemTradingMode(FALSE,strMessage);
			}
			break;
		}
	}
	else if(sShareName=="종목코드")
	{
		AfxMessageBox("종목코드 공유");
	}
	return 0;
}
/// gm0604 end

/*
void CStdDialog::Send2HanHwa_09501(void* pData, int nLen, CWnd* pRequestWnd, CString strTRCode)
{
	EnableWindowAll(FALSE);	

	SendMessage2StatusBar(MSG_GOING);
	m_pRqWndFromTab = NULL;
	m_pRqWndFromTab = pRequestWnd;
}
*/

#undef OBJECT_SELECT
#include "../chart_common/BlockBaseEnum.h"


LONG CStdDialog::OnByPass_ExportToolOption(UINT wParam, LPARAM lParam)
{	
	if(wParam == CToolOptionInfo::T_RESTORE
		|| wParam == CToolOptionInfo::T_DATALIST_REPORT
		|| wParam == CToolOptionInfo::T_DELETE_INORDER
		|| wParam == CToolOptionInfo::T_DELETE_ALL
		|| wParam == CToolOptionInfo::T_PRICE_YSCALE_SETUP
		|| wParam == CToolOptionInfo::T_UNDO_WORK
		|| wParam == CToolOptionInfo::T_REDO_WORK
		|| wParam == CToolOptionInfo::TOOLOPTION::T_ORDER_LINE				//주문선
		|| wParam == CToolOptionInfo::TOOLOPTION::T_ORDER_LINE_MANAGER)		//주문선관리
	{
		CWnd* pWnd = (CWnd*)m_ChartMng.GetpChartItemSelected();
		if(pWnd)
		{
			//========================================================================
			// 초기화 버튼이 눌렸을 경우 해당 차트의 종목이 없는 경우는 
			// 초기화 메세지를 OCX에 던지지 않는다.
			// 05.11.24
			//========================================================================
			if(wParam == CToolOptionInfo::T_RESTORE)
			{
				if(m_ChartMng.GetSafeHwnd())
					if( !m_ChartMng.SendMessage(UMSG_EXT_TOOL_RESTORE)) //종목이 없는 경우는 
						return 1;										//초기화를 시키지 않는다.
			}
			//========================================================================


			pWnd->SendMessage(RMS_ANALYSIS_FUNC, wParam, 1);	
		}
		return 0L;
	}

	for(int nRowIndex=(m_ChartMng.GetRows()-1);nRowIndex>=0;nRowIndex--)
	{
		for(int nColIndex=(m_ChartMng.GetCols()-1);nColIndex>=0;nColIndex--)
		{
			CWnd* pWnd = (CWnd*)m_ChartMng.GetChartItem(nRowIndex, nColIndex);
			if(pWnd)
			{
				//========================================================================
				// 외부툴바 가 생성되면서... 
				// 종목코드의 포커스를 가져가는 문제가 생겨서 
				// lParam에 9999가 들어오면
				// 종목 입력창에 포커스가 가도록 한다. 05.11.15
				//========================================================================
				if(lParam == 9999)
					if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
					{
						m_pInputBarDlg->SendMessage(UMSG_EDIT_FOCUS);
						return 1;
					}
				//========================================================================

				//========================================================================
				// 초기화 버튼이 눌렸을 경우 해당 차트의 종목이 없는 경우는 
				// 초기화 메세지를 OCX에 던지지 않는다.
				// 05.11.24
				//========================================================================
				if(wParam == CToolOptionInfo::T_RESTORE)
				{
					if(m_ChartMng.GetSafeHwnd())
						if( !m_ChartMng.SendMessage(UMSG_EXT_TOOL_RESTORE)) //종목이 없는 경우는 
							return 1;										//초기화를 시키지 않는다.
				}
				//========================================================================

// 				// 1*1일때만 허용한다.
// 				if( wParam == CToolOptionInfo::TOOLOPTION::T_ORDER_LINE ||			//주문선
// 					wParam == CToolOptionInfo::TOOLOPTION::T_ORDER_LINE_MANAGER)	//주문선관리
// 				{
// 					if(m_pExtInputBarDlg)
// 					{
// 						long lRow, lCols;
// 						m_pExtInputBarDlg->GetMatrix(lRow, lCols);
// 						if(lRow>1 || lCols>1)
// 						{
// 							AfxMessageBox("주문선 기능은 1*1 형태에서만 가능합니다.");
// 							return 1L;
// 						}
// 					}
// 				}
				pWnd->SendMessage(RMS_ANALYSIS_FUNC, wParam, lParam);	
			}
		}
	}
	return 0L;
}

BOOL CStdDialog::Load_ReplayChart_Lib()
{	
	BOOL bResult  = FALSE;

	CString strFileRoot;
	strFileRoot = m_strRootPath + "\\Dev\\ReplayChart.dll";

	m_hReplayChartLib = NULL;
	m_hReplayChartLib = LoadLibrary(strFileRoot);
	
	if (m_hReplayChartLib)
	{
		_fnReplayDll_Load			= (_DLL_fnReplayDll_Load		)::GetProcAddress(m_hReplayChartLib, "fnReplayDll_Load"			);
		_fnReplayDll_UnLoad			= (_DLL_fnReplayDll_Unload		)::GetProcAddress(m_hReplayChartLib, "fnReplayDll_Unload"		);
		_fnReplayDll_SetInitInfo	= (_DLL_fnReplayDll_SetInitInfo	)::GetProcAddress(m_hReplayChartLib, "fnReplayDll_SetInitInfo"	);
		_fnReplayDll_Run			= (_DLL_fnReplayDll_Run			)::GetProcAddress(m_hReplayChartLib, "fnReplayDll_Run"			);				
		_fnReplayDll_ChangeSpeed	= (_DLL_fnReplayDll_ChangeSpeed	)::GetProcAddress(m_hReplayChartLib, "fnReplayDll_ChangeSpeed"	);

		m_pWndReplayLib = _fnReplayDll_Load(GetSafeHwnd());
		bResult = TRUE;
	}
	else
	{	
#ifdef _DEBUG
		AfxMessageBox("\n# ReplayChart.dll이 없습니다.");
#endif
		m_pWndReplayLib = NULL;
		bResult = FALSE;
	}

	return bResult;
}

BOOL CStdDialog::UnLoad_ReplayChart_Lib()
{		
	if(m_hReplayChartLib != NULL)
	{
		if(m_pWndReplayLib)
			_fnReplayDll_UnLoad(m_pWndReplayLib);
		
		if(FreeLibrary(m_hReplayChartLib))
			m_hReplayChartLib = NULL;

		return TRUE;
	}

	return FALSE;
}

BOOL CStdDialog::RunReplay(int nRun, REPLAY_INFO* pReplayInfo) 
{	
	if(m_pWndReplayLib && m_hReplayChartLib)
	{
		switch(nRun)
		{			
			case PLAY:
				{	
					if( m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd() )
						m_pExtInputBarDlg->EnableWindow(FALSE);
				
					if(pReplayInfo != NULL)
					{
						pReplayInfo->hCallHwnd		= this->m_hWnd;
//						pReplayInfo->hRecvMainFrame = m_hMainFrame;
						m_strSpeed.Format("%d",		pReplayInfo->nSpeed);
//						m_ChartMng.GetChartAllCode(pReplayInfo->arrStrCode);
						CString strCodeInputCode = m_pInputBarDlg->m_pCodeInput->GetCode();
						pReplayInfo->arrStrCode.RemoveAll();
						pReplayInfo->arrStrCode.Add(strCodeInputCode);
						if( pReplayInfo->arrStrCode.GetSize() <= 0)
						{
							AfxMessageBox("복기를 시작할 종목이 없습니다.");
							return FALSE;
						}	
						
						_fnReplayDll_SetInitInfo(m_pWndReplayLib, (void*)pReplayInfo);
						return TRUE;
					}				
				}
				break;
			case STOP:
				{
					m_strSpeed = "1";
					SetBNeedRqstTRForReplay(TRUE);	//Stop 버튼이 눌리는 것은.. 플레이를 적어도 한번은 실행하고 
													//눌렸다고 가정 
				}
				break;
		}

		_fnReplayDll_Run(m_pWndReplayLib, nRun);

		return TRUE;
	}
	return FALSE;
}

void CStdDialog::ChangeSpeed(int nSpeed)
{
	m_strSpeed.Format("%d",	nSpeed);
	_fnReplayDll_ChangeSpeed(m_pWndReplayLib, nSpeed);
}

LPCTSTR CStdDialog::GetMainPath()
{
	return m_strRootPath;
}
LPCTSTR CStdDialog::GetUserID()
{
	return m_strUserID;
}

//>> 갭보정 - vntsorl(20091008)
// 허수봉설정.
//bool CStdDialog::SetImaginaryCandleToOutside(const bool bImaginaryCandle)
//{ 
//	m_ChartCfgInfo.bRealCandle	= bImaginaryCandle; // TRUE 면 허봉
////	m_ChartMng.UpdateShowingFacked_JustOneChart(bImaginaryCandle, FALSE);
//
//	//향후 환경 동기화 버튼이 토글 형태로 바뀔 경우 환경동기화 On일 경우 적용한다.
//	//{
//		m_pExtInputBarDlg->SendMessage(UMSG_SETNORMALIZEDMINUTETIMERULE,bImaginaryCandle);	
//		m_ChartMng.SendMessage(UMSG_SETNORMALIZEDMINUTETIMERULE,bImaginaryCandle);	
//	//}
//	return TRUE;
//}	
bool CStdDialog::SetImaginaryCandleAndGapRevisionToOutside( const bool bImaginaryCandle, const bool bUseGapRevision)
{
//<< 20130530 이상신 : 월물간 갭보정 추가(IBK) (Issue No : 0006572)
	BOOL bDontRequestData = TRUE;
	CChartItem* pChartItem = m_ChartMng.GetpChartItemSelected();
	if (pChartItem && pChartItem->SyncMonthGapRev())
		bDontRequestData = FALSE;
//>> (Issue No : 0006572)

	m_ChartCfgInfo.bRealCandle	= bImaginaryCandle; // TRUE 면 허봉
	//	m_ChartMng.UpdateShowingFacked_JustOneChart(bImaginaryCandle, FALSE);
	
	//향후 환경 동기화 버튼이 토글 형태로 바뀔 경우 환경동기화 On일 경우 적용한다.
	//{
	if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
		m_pExtInputBarDlg->SendMessage(UMSG_SETNORMALIZEDMINUTETIMERULE,bImaginaryCandle);	
	m_ChartMng.SendMessage(UMSG_SETNORMALIZEDMINUTETIMERULE, bImaginaryCandle, bDontRequestData);
	//}
	return TRUE;
	
	/*
	// (2008/11/2 - Seung-Won, Bae) Do not support sync for all ChartItem.
	//	m_ChartCfgInfo.bImaginaryCandle	= bImaginaryCandle; // TRUE 면 허봉
	//	m_ChartCfgInfo.bUseGapRevision	= bUseGapRevision;	// TRUE 면 갭보정
	//	m_pExtInputBarDlg->SendMessage( UMSG_SETNORMALIZEDMINUTETIMERULE, bImaginaryCandle, bUseGapRevision);	
	
	  // (2008/11/2 - Seung-Won, Bae) Set Minute data for Normalized Time or Gap Revision.
	  m_ChartMng.SendMessage( UMSG_SET_NORMALIZEDMINUTETIMERULE_AND_GAPREVISION, bImaginaryCandle, bUseGapRevision);
	*/	
	return TRUE;
}	
//<< 갭보정 - vntsorl(20091008)

// 상/하한가 표시
bool CStdDialog::SetShowHighLowBoundToOutside(const bool bShow)	
{ 
	m_ChartCfgInfo.bShowHLBar		= bShow				;
	m_ChartMng.SetEnableBoundMarkOnlyOne(bShow);
	return TRUE;
}	

// 권리락/배당락 표시
bool CStdDialog::SetShowExRightDividendToOutside(const bool bShow)
{ 
	m_ChartCfgInfo.bShowRight		= bShow;
	
	if(m_ChartMng.GetpChartItemSelected()->GetType()=='2')
	{
		m_ChartMng.SetShowExRightOnlyOne(bShow);
	}
	else
	{
		m_ChartMng.SetShowExRightOnlyOne(FALSE);
	}
	
	return TRUE;
}	

// 시간연동 수신
bool CStdDialog::SetTimeLinkageToOutside(const bool bTimeLinkage)
{ 
	m_ChartCfgInfo.bRecvTimeShare	= bTimeLinkage;		
	m_ChartMng.SetTimeShareOnlyOne(bTimeLinkage);
	return TRUE;
}	

// 차트종료시 자동 저장 유무
bool CStdDialog::SetAutoSaveToOutside(const bool bAutoSave)
{	
	m_ChartCfgInfo.bAutoSave		= bAutoSave;
	m_ChartMng.SetAutoSaveToOutside(bAutoSave);
	return TRUE;
}	

// 가격차트 log 셋팅.
bool CStdDialog::SetPriceLog(const bool bLog)
{
	m_ChartCfgInfo.bShowLog		= bLog;
	m_ChartMng.SetPriceLog(bLog);
	return TRUE;
}

bool CStdDialog::OnChartEnvSetupOk()
{
	m_ChartMng.OnChartEnvSetupOK();
	return TRUE;
}


bool CStdDialog::GetImaginaryCandleToOutside		(bool& bImaginaryCandle		)	// 허수봉설정.
{
	return m_ChartMng.GetImaginaryCandleToOutside(bImaginaryCandle);
}
bool CStdDialog::GetShowHighLowBoundToOutside	(bool& bShow				)	// 상/하한가 표시
{
	return m_ChartMng.GetShowHighLowBoundToOutside(bShow);
}

bool CStdDialog::GetShowExRightDividendToOutside	(bool& bShow				)	// 권리락/배당락 표시
{
	return m_ChartMng.GetShowExRightDividendToOutside(bShow);
}

bool CStdDialog::GetTimeLinkageToOutside			(bool& 	bTimeLinkage		)	// 시간연동 수신
{
	return m_ChartMng.GetTimeLinkageToOutside(bTimeLinkage);
}

bool CStdDialog::GetAutoSaveToOutside			(bool& bAutoSave			)	// 차트종료시 자동 저장 유무
{
	return m_ChartMng.GetAutoSaveToOutside(bAutoSave);
}

void CStdDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonDown(nFlags, point);
}

void CStdDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rcExtTool;
	dc.GetClipBox(rcExtTool);
	dc.FillSolidRect(&rcExtTool, GetBkgrndColor());
	
	// 돋보기 구현 - ojtaso (20080519)
//	if(m_pDlgGlass && m_pDlgGlass->IsWindowVisible())
//	{
//		CRect rcBounds;
//		GetWindowRect(rcBounds);
//		ScreenToClient(rcBounds);
//		
//		m_memoryDC.CreateCompatibleDC(&dc);
//		CreateBufferBitmap(&dc, rcBounds);
//		CBitmap* pOldbm = m_memoryDC.SelectObject(&m_bufferBitmap);
//		
//		CRect rcMain = rcBounds;
//		dc.StretchBlt(rcMain.left, rcMain.top, rcMain.Width(), rcMain.Height(), &m_memoryDC, 
//			0, 0, rcMain.Width(), rcMain.Height(), SRCCOPY) ;
//		
//		m_pDlgGlass->OnDraw(&m_memoryDC, m_bufferBitmap);
//		
//		m_memoryDC.SelectObject(pOldbm) ;
//		m_memoryDC.DeleteDC();
//	}
}

void CStdDialog::InitImageList()
{
	// 버튼 이미지 로드
	BOOL bOK = FALSE;
	CBitmap bitmap;
	CString strBitmapFile;	
	
	//W15_H15===========================================================================
	if ( !m_ImgList_W15H15.m_hImageList && (m_nChartImageListFlag & IMGLIST_15X15))
	{
		if ( !m_ImgList_W15H15.Create( 15, 15, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W15_H15.bmp file" );			
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W15_H15.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W15H15.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}
	
	//W20_H20===========================================================================
	if ( !m_ImgList_W20H20.m_hImageList && (m_nChartImageListFlag & IMGLIST_20X20))
	{
		if ( !m_ImgList_W20H20.Create( 20, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W20_H20.bmp file" );			
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W20_H20.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W20H20.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}

	//W20_H20_Tool===========================================================================
	if ( !m_ImgList_W20H20_Tool.m_hImageList && (m_nChartImageListFlag & IMGLIST_20X20TOOL))
	{
		if ( !m_ImgList_W20H20_Tool.Create( 20, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W20_H20_Tool.bmp file" );			
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W20_H20_Tool.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W20H20_Tool.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}

	//W46_H20===========================================================================
	if ( !m_ImgList_W46H20.m_hImageList && (m_nChartImageListFlag & IMGLIST_46X20))
	{
		if ( !m_ImgList_W46H20.Create( 46, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W46_H20.bmp file" );
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W46_H20.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));	
		m_ImgList_W46H20.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();						

	}
	
	//W50_H20===========================================================================
	if ( !m_ImgList_W50H20.m_hImageList && (m_nChartImageListFlag & IMGLIST_50X20))
	{
		if ( !m_ImgList_W50H20.Create( 50, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W50_H20.bmp file" );
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W50_H20.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));	

		m_ImgList_W50H20.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}

	//W151_H21===========================================================================
	if ( !m_ImgList_W151H21.m_hImageList && (m_nChartImageListFlag & IMGLIST_151X21))
	{		
		if ( !m_ImgList_W151H21.Create( 151, 21, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W151_H21.bmp file" );
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W151_H21.bmp");		
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W151H21.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}

	//W28_H22===========================================================================
	if ( !m_ImgList_W28H22.m_hImageList && (m_nChartImageListFlag & IMGLIST_28X22))
	{
		if ( !m_ImgList_W28H22.Create( 28, 22, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W28_H22.bmp file" );
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W28_H22.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));	
		m_ImgList_W28H22.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();

	}

	//W17_H20===========================================================================

	if ( !m_ImgList_W17H20.m_hImageList && (m_nChartImageListFlag & IMGLIST_17X20))
	{
		if ( !m_ImgList_W17H20.Create( 17, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W17_H20.bmp file" );
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W17_H20.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));	
		m_ImgList_W17H20.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();

	}

	//W43_H20===========================================================================
	if ( !m_ImgList_W43H20.m_hImageList && (m_nChartImageListFlag & IMGLIST_43X20))
	{
		if ( !m_ImgList_W43H20.Create( 43, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W43_H19.bmp file" );			
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W43_H20.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W43H20.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}

	//W58_H20===========================================================================
	if ( !m_ImgList_W58H20.m_hImageList && (m_nChartImageListFlag & IMGLIST_58X20))
	{
		if ( !m_ImgList_W58H20.Create( 58, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W58_H19.bmp file" );			
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W58_H20.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W58H20.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}

	//W72_H19===========================================================================
	if ( !m_ImgList_W72H20.m_hImageList && (m_nChartImageListFlag & IMGLIST_72X20))
	{
		if ( !m_ImgList_W72H20.Create( 72, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W72_H20.bmp file" );			
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W72_H20.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W72H20.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}
	
	//W46_H20_Gwan===========================================================================
	if ( !m_ImgList_W46H20_Gwan.m_hImageList && (m_nChartImageListFlag & IMGLIST_46X20GWN))
	{
		if ( !m_ImgList_W46H20_Gwan.Create( 46, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W46_H20_Gwan.bmp file" );			
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W46_H20_Gwan.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W46H20_Gwan.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}

	//W8_H25===========================================================================
	if ( !m_ImgList_W8H25.m_hImageList && (m_nChartImageListFlag & IMGLIST_8X25))
	{
		if ( !m_ImgList_W8H25.Create( 8, 20, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W8_H25.bmp file" );
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W8_H25.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));	
		m_ImgList_W8H25.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
		
	}
}

void CStdDialog::DeleteAllImageList()
{
	// 로드된 차트 갯수
	if(m_nChartImageListFlag & IMGLIST_15X15)
	{
		m_nChartImgCnt15X15--;
		if(m_nChartImgCnt15X15 <= 0)
			m_ImgList_W15H15.DeleteImageList();
	}
	if(m_nChartImageListFlag & IMGLIST_20X20)
	{
		m_nChartImgCnt20X20--;
		if(m_nChartImgCnt20X20 <= 0)
			m_ImgList_W20H20.DeleteImageList();
	}
	if(m_nChartImageListFlag & IMGLIST_20X20TOOL)
	{
		m_nChartImgCnt20X20Tool--;
		if(m_nChartImgCnt20X20Tool <= 0)
			m_ImgList_W20H20_Tool.DeleteImageList();
	}
	if(m_nChartImageListFlag & IMGLIST_46X20)
	{
		m_nChartImgCnt46X20--;
		if(m_nChartImgCnt46X20 <= 0)
			m_ImgList_W46H20.DeleteImageList();
	}
	if(m_nChartImageListFlag & IMGLIST_50X20)
	{
		m_nChartImgCnt50X20--;
		if(m_nChartImgCnt50X20 <= 0)
			m_ImgList_W50H20.DeleteImageList();
	}
	if(m_nChartImageListFlag & IMGLIST_28X22)
	{
		m_nChartImgCnt28X22--;
		if(m_nChartImgCnt28X22 <= 0)
			m_ImgList_W28H22.DeleteImageList();
	}
	if(m_nChartImageListFlag & IMGLIST_17X20)
	{
		m_nChartImgCnt17X20--;
		if(m_nChartImgCnt17X20 <= 0)
			m_ImgList_W17H20.DeleteImageList();
	}
	if(m_nChartImageListFlag & IMGLIST_43X20)
	{
		m_nChartImgCnt43X20--;
		if(m_nChartImgCnt43X20 <= 0)
			m_ImgList_W43H20.DeleteImageList();
	}
	if(m_nChartImageListFlag & IMGLIST_58X20)
	{
		m_nChartImgCnt58X20--;
		if(m_nChartImgCnt58X20 <= 0)
			m_ImgList_W58H20.DeleteImageList();
	}
	if(m_nChartImageListFlag & IMGLIST_46X20GWN)
	{
		m_nChartImgCnt46X20Gwan--;
		if(m_nChartImgCnt46X20Gwan <= 0)
			m_ImgList_W46H20_Gwan.DeleteImageList();
	}
	if(m_nChartImageListFlag & IMGLIST_8X25)
	{
		m_nChartImgCnt8X25--;
		if(m_nChartImgCnt8X25 <= 0)
			m_ImgList_W8H25.DeleteImageList();
	}
	if(m_nChartImageListFlag & IMGLIST_151X21)
	{
		m_nChartImgCnt151X21--;
		if(m_nChartImgCnt151X21 <= 0)
			m_ImgList_W151H21.DeleteImageList();
	}
	if(m_nChartImageListFlag & IMGLIST_72X20)
	{
		m_nChartImgCnt72X20--;
		if(m_nChartImgCnt72X20 <= 0)
			m_ImgList_W72H20.DeleteImageList();
	}
}

void CStdDialog::GetStartEndDateAtStdDialog(double& dStartDate, double& dEndDate)
{
	if(m_ChartMng.GetSafeHwnd())
		m_ChartMng.GetStartEndDateAtChartMng(dStartDate, dEndDate);	
}


LRESULT CStdDialog::OnDropData(WPARAM wp, LPARAM lp)
{	
	return 0;
}

void CStdDialog::AllCodeUnAdvise()
{
//	for(int i = 0; i < m_nDrdsInfoCnt; ++i)
//		MakeDrdsUnConnectUnAdviseFromCode(i, FALSE);

	CStringList strList;
	m_RealTrCounter.GetAllKey(strList);

	CString strKey;
	int nPos1 = 0;
	int nPos2 = 0;
	int nCount = 0;
	CString strCode;
	POSITION pos = strList.GetHeadPosition();
	while(pos)
	{
		strKey = strList.GetNext(pos);
		nPos1 = strKey.Find(":");
		nPos2 = strKey.Find(",");
		strCode = strKey.Mid(nPos1 + 1, nPos2 - nPos1 - 1);
		nCount = atoi(strKey.Mid(nPos2 + 1));
		for(int nIndx = 0; nIndx < nCount; ++nIndx)
		{
			OnCodeUnAdvise(strCode, TRUE);
		}
	}


/*
	IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager);
	if(!pDrdsMng) return;

	if(m_pDrdsLib)
	{
		RemoveDrdsFromCode(&m_ListStockRealCodes);
		RemoveDrdsFromCode(&m_ListUpjongRealCodes);
		RemoveDrdsFromCode(&m_ListFutOptRealCodes);
		RemoveDrdsFromCode(&m_ListStockFutOptRealCodes);
		RemoveDrdsFromCode(&m_ListForeignRealCodes);

		pDrdsMng->UnadviseDrdsLib(m_pDrdsLib);
		m_pDrdsLib = NULL;
	}
*/

	m_RealTrCounter.RemoveAll();
}

void CStdDialog::Recv_ReplayRealData(void *pData, int nLen)
{
}

LONG CStdDialog::OnReplayDrds(WPARAM wp, LPARAM lp)
{
	if( (m_nMarketType != CMasterTypeInfo::STOCK_CHART) && (m_nMarketType != CMasterTypeInfo::FUTOPT_CHART))
		return 1L;

	CString strCode, strSpeed;
	strCode.Format("%s", (LPCTSTR)wp);

	m_ChartMng.SetReplayRealTitle(strCode, m_strSpeed, (TICK_DATA*)lp);
	return 1;
}

LONG CStdDialog::OnReplayFail(WPARAM wp, LPARAM lp)
{
	if( (m_nMarketType != CMasterTypeInfo::STOCK_CHART) && (m_nMarketType != CMasterTypeInfo::FUTOPT_CHART))
		return 1L;
	
	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		m_pInputBarDlg->ReplayFail();

	return 1;
}

LONG CStdDialog::OnReplayChangeDate(WPARAM wp, LPARAM lp)
{
	if( (m_nMarketType != CMasterTypeInfo::STOCK_CHART) && (m_nMarketType != CMasterTypeInfo::FUTOPT_CHART))
		return 1L;

	CString strCode, strBojoMsg;
	strCode.Format("%s", (LPCTSTR)wp);
	strBojoMsg.Format("%s", (LPCTSTR)lp);
	
	m_ChartMng.SetReplayChangedDate(strCode, strBojoMsg);

	return 1;
}

BOOL CStdDialog::GetTimeSyncFlag()
{
	if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
		return m_pExtInputBarDlg->GetTimeSyncFlag();
	return FALSE;
}

void CStdDialog::SetTimeSyncFlag(BOOL bCheck)
{
	SetBReplayMode(bCheck);

	m_ChartMng.SetBTypeLink(bCheck);
	m_ChartMng.SetBReplayChartMode(bCheck); 
	if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
	{
		m_pExtInputBarDlg->SetTimeSyncFlag(bCheck);
		m_pExtInputBarDlg->EnableWindow(!bCheck);
	}
}

void CStdDialog::RqstTRForReplay()
{
	CStringArray arrStrCode;
	m_ChartMng.GetChartAllCode(arrStrCode);

	int nSize = arrStrCode.GetSize();
	if(nSize <= 0) return;

	

	CString strMultiCodes = _T("");

	for(int i = 0; i < nSize; i++) 
	{
		strMultiCodes += arrStrCode.GetAt(i);
		strMultiCodes += ",;";
	}	

	m_ChartMng.SendMessage(UMSG_MULTICODES, NULL, (LPARAM)(LPCTSTR)strMultiCodes);	
}

// 시장지표등의 TR Data요청
void CStdDialog::RequestTRPacket(const short nID, const LPCTSTR pcszPacketNames, const short nCount, const bool bIsNew)
{		
	m_ChartMng.MarketTRSend_By_OCXSignal(nID, pcszPacketNames, nCount, bIsNew ? TRUE : FALSE);
}

bool CStdDialog::IsLoad_MarketChart() const
{ 
	if(m_nMarketType == CMasterTypeInfo::STOCK_CHART)
		return true;

	return false;
}

void CStdDialog::SetBReplayMode(BOOL bReplay)
{ 
	m_bReplayMode = bReplay; 	
	if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
		m_pExtInputBarDlg->EnableWindow(!bReplay);
}

BOOL CStdDialog::CheckDataTypeForReplay()
{
	return m_ChartMng.CheckDataTypeForReplay();
}

LONG CStdDialog::OnReplayEnd(WPARAM wp, LPARAM lp)
{
	if( (m_nMarketType != CMasterTypeInfo::STOCK_CHART) && (m_nMarketType != CMasterTypeInfo::FUTOPT_CHART))
		return 1L;

	return 1;
}

LONG CStdDialog::OnReplaySetTotalSec(WPARAM wp, LPARAM lp)
{
	if( (m_nMarketType != CMasterTypeInfo::STOCK_CHART) && (m_nMarketType != CMasterTypeInfo::FUTOPT_CHART))
		return 1L;
	
	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		m_pInputBarDlg->SetReplayTotalSec(wp);

	return 1;
}

LONG CStdDialog::OnReplayEachSec(WPARAM wp, LPARAM lp)
{
	if( (m_nMarketType != CMasterTypeInfo::STOCK_CHART) && (m_nMarketType != CMasterTypeInfo::FUTOPT_CHART))
		return 1L;
	CString sTime((LPTSTR)(LPCTSTR)wp);

	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		m_pInputBarDlg->SetReplayEachSec(sTime);

	return 1;
}


void CStdDialog::SetOpenMapData(LPCTSTR szOpenData, short nDataLen) 
{
	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd() && m_pInputBarDlg->m_pCodeInput)
	{
		CString strCodeInputCode = m_pInputBarDlg->m_pCodeInput->GetCode();
		CString strSetCode = (LPCTSTR)szOpenData;
		if(strSetCode.GetLength() && strCodeInputCode != strSetCode)
		{
			m_pInputBarDlg->SetShareCodeChanged(TRUE);
			m_pInputBarDlg->m_pCodeInput->SetCode((LPCTSTR)szOpenData);
		}
	}
}

//@유진추가
void CStdDialog::ChangeCode(LPCTSTR szCode, int nDefault/* = MODE_DEFAULT*/, int nMode/* = CHANGE_ITEM*/) 
{
	int nEnumMarketType = GetMarketTypeFromItemCode(szCode);

	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd() && m_pInputBarDlg->m_bUseReplayChartMode)
	{
		if(IsAvailableReplayRunnig(nEnumMarketType, nMode) == FALSE)
			return;
	}
	else
	{
		nDefault = MODE_DEFAULT;
		nMode = CHANGE_ITEM;
	}

	if(nDefault == MODE_USER)
	{
		if(nMode != REPLAY_ITEM)
			m_ChartMng.SendMessage(UMSG_CHANGEMODE, 0, nMode);
	}
	
	// 20130518 이상신 : 종목코드가 동일할때는 조회하지 않기 때문에 실시간을 해제 하지 않는다.
	if (m_nChartType >= MINI_MULTI_CHART)
	{
		CString strCode = m_pInputBarDlg->GetCurrentCode();

		if (strCode.Compare(szCode) != 0)
			AllCodeUnAdvise(); //Real 전부 해제..
	}

	ChangeStockMode(nEnumMarketType);
	
	STINPUTCODEDATA stInputCodeData;
	stInputCodeData.m_strCode = szCode;
	stInputCodeData.m_strTime = "";
	stInputCodeData.m_nDefault = nDefault;
	stInputCodeData.m_nMode = nMode;
	// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)
	m_ChartMng.SendMessage(UMSG_INPUTCODETOCHARTMNG, nDefault, (LPARAM)&stInputCodeData);
	//@alzioyes-A001 DoShareCode(szCode);	//@Solomon
}

void CStdDialog::ChangeMarket(UINT nMarketType, BOOL bUpdateCbMarket/* = TRUE*/ , BOOL bRequestToServer/* = FALSE*/) 
{
	m_nMarketType = nMarketType;
	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		m_pInputBarDlg->ChangeMarket(nMarketType, bUpdateCbMarket, bRequestToServer);
}

void CStdDialog::EnableQueryOpt(BOOL bEnable)
{
	if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
		m_pExtInputBarDlg->EnableQueryOpt(bEnable);

	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		m_pInputBarDlg->EnableQueryOpt(bEnable);
}

void CStdDialog::EnableAccount(BOOL bEnable)
{
	if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
		m_pExtInputBarDlg->EnableAccount(bEnable);
}

//@유진추가

//-----------------------------------------------------------------------------
// Author		: {{{ 오정택 }}} ojtaso 	Date : 2005/2/23
// Reason		: Added to drag and drop operation.
// modify		: {{{ 이영희 }}} Date : 2006/7/7
// Reason		: 메인 모듈 옮겨옴
//-----------------------------------------------------------------------------
// BEGIN
LRESULT CStdDialog::OnDragEnd(WPARAM wParam, LPARAM lParam)
{
	CDragNDrop* pDnD = (CDragNDrop*)lParam;	

	//@Solomon:100226(alzioyes) m_ChartMng영역일때만 처리하도록 제한한다.
	{
		CRect rectChartMng;
		m_ChartMng.GetWindowRect(&rectChartMng);
		ScreenToClient(&rectChartMng);

		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);
		if(rectChartMng.PtInRect(pt)==FALSE)
			return 0L;
	}

	if(strcmp(pDnD->GetKey(), "지표-드래그") == 0)
	{
		if(!pDnD)
			return 0L;

		HWND hWnd = ::WindowFromPoint(*pDnD->GetCursorPoint());
		HWND hStart = pDnD->GetHWndStart();

		if(m_hCursor)
		{
			::DeleteObject(m_hCursor);
			m_hCursor = NULL;
		}

		if(::IsWindow(hWnd) && hWnd != hStart && !::IsChild(hWnd, hStart))
		{
			if(m_ChartMng.GetSafeHwnd()) // 2005. 04. 22 by Nam
				m_ChartMng.SendMessage(UMSG_ADDCHARTBLOCK,(WPARAM)(LPCTSTR)pDnD->GetValue());
		}

		if(pDnD->IsDelete())
			delete pDnD;
	}
	else
	{
		if(!pDnD)
			return 0L;

		HWND hWnd = ::WindowFromPoint(*pDnD->GetCursorPoint());
		HWND hStart = pDnD->GetHWndStart();

		if(m_hCursor)
		{
			::DeleteObject(m_hCursor);
			m_hCursor = NULL;
		}

		if(::IsWindow(hWnd) && hWnd != hStart && !::IsChild(hWnd, hStart))
		{
			CString strParam = pDnD->GetValue();
			int nPosCode = strParam.Find(";");
			CString strCode = strParam.Left(nPosCode);
			strCode.Remove(';');
			CString strOption = strParam.Mid(nPosCode + 1, 1);
			if(strOption == "L")
				ChangeCode(strCode);
			else if(strOption == "R")
			{
				CStringList listViewType;
				if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
				{
					m_pInputBarDlg->GetChartViewType(listViewType);

					CMenu menu;
					menu.CreatePopupMenu();

					int nIndx = 0;
					POSITION pos = listViewType.GetHeadPosition();
					while(pos)
					{
						menu.AppendMenu(MF_STRING, (UINT)1000 + nIndx, listViewType.GetNext(pos));
						nIndx++;
					}

					UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
					int nXPos = (*pDnD->GetCursorPoint()).x;
					int nYPos = (*pDnD->GetCursorPoint()).y;
					int nSelectMode = menu.TrackPopupMenu(nFlagsForMenu, nXPos, nYPos, this);
					menu.DestroyMenu();
					if(nSelectMode <= 0)
					{
						
						if(pDnD->IsDelete())
						delete pDnD;

						return 0L;
					}

					if(nSelectMode - 1000 == 4)
					{
						m_pInputBarDlg->RequestReplayMode();
						ChangeCode(strCode);
					}
					else
					{
						ChangeCode(strCode, MODE_USER, nSelectMode - 1000);
					}
				}				
			}
		}

		if(pDnD->IsDelete())
			delete pDnD;
	}
	return 0L;
}

LRESULT CStdDialog::OnDragStart(WPARAM wParam, LPARAM lParam)
{
	CPoint* pPoint = (CPoint*)wParam;

	HWND hWnd = ::WindowFromPoint((POINT)(*pPoint));
	HWND hStart = (HWND)lParam;

	::SetFocus(hWnd);

	if(hWnd != hStart)
	{
		//AFX_MANAGE_STATE(AfxGetStaticModuleState());
		//HINSTANCE hInstResourceClient = AfxGetResourceHandle();
		//AfxSetResourceHandle(g_hInstance);

		m_hCursor = AfxGetApp()->LoadCursor(IDC_HANDCUR);
		//AfxSetResourceHandle(hInstResourceClient);
		::SetCursor(m_hCursor);
	}

	return 0L;
}

void CStdDialog::ChangeUpjongMode(UINT type)
{
	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		m_pInputBarDlg->InitUpjongCodeCombo(type);
}

HRESULT CStdDialog::ReceiveRealData(WORD wFID, LPCSTR szMainKey, void* pRealData)
{
	return 1L;
}

void CStdDialog::ConvertHogaData(LPVOID	aTRData, long dwTRDateLen)
{
// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CStdDialog::ConvertHogaData: Begin\n", __LINE__);
// 		OutputDebugString(szDebug);
// 	}

	CString strOut;
	LPk01230Out1 pOut1 = (LPk01230Out1)aTRData;
	m_strOut = "";

	strOut += CharToString(pOut1->recprice, sizeof(pOut1->recprice), TRUE);			// 기준가
	strOut += CharToString(pOut1->yeprice, sizeof(pOut1->yeprice), TRUE);			// 예상체결가격
	strOut += CharToString(pOut1->yevolume, sizeof(pOut1->yevolume), TRUE);			// 예상체결수량
	strOut += CharToString(pOut1->hname, sizeof(pOut1->hname));						// 종목명
	strOut += CharToString(pOut1->price, sizeof(pOut1->price), TRUE);				// 현재가
	strOut += CharToString(pOut1->sign, sizeof(pOut1->sign));						// 등락부호
	strOut += CharToString(pOut1->change, sizeof(pOut1->change));					// 등락폭
	strOut += CharToString(pOut1->rate, sizeof(pOut1->rate));						// 대비율
	strOut += CharToString(pOut1->volume, sizeof(pOut1->volume), TRUE);				// 누적거래량
	strOut += CharToString(pOut1->trade_cost, sizeof(pOut1->trade_cost), TRUE);		// 누적거래대금
	strOut += CharToString(pOut1->offerho1, sizeof(pOut1->offerho1), TRUE);			// 매도호가1
	strOut += CharToString(pOut1->offerho2, sizeof(pOut1->offerho2), TRUE);			// 매도호가2
	strOut += CharToString(pOut1->offerho3, sizeof(pOut1->offerho3), TRUE);			// 매도호가3
	strOut += CharToString(pOut1->offerho4, sizeof(pOut1->offerho4), TRUE);			// 매도호가4
	strOut += CharToString(pOut1->offerho5, sizeof(pOut1->offerho5), TRUE);			// 매도호가5
	strOut += CharToString(pOut1->offerrem1, sizeof(pOut1->offerrem1), TRUE);		// 매도잔량1
	strOut += CharToString(pOut1->offerrem2, sizeof(pOut1->offerrem2), TRUE);		// 매도잔량2
	strOut += CharToString(pOut1->offerrem3, sizeof(pOut1->offerrem3), TRUE);		// 매도잔량3
	strOut += CharToString(pOut1->offerrem4, sizeof(pOut1->offerrem4), TRUE);		// 매도잔량4
	strOut += CharToString(pOut1->offerrem5, sizeof(pOut1->offerrem5), TRUE);		// 매도잔량5
	strOut += CharToString(pOut1->bidho1, sizeof(pOut1->bidho1), TRUE);				// 매수호가1
	strOut += CharToString(pOut1->bidho2, sizeof(pOut1->bidho2), TRUE);				// 매수호가2
	strOut += CharToString(pOut1->bidho3, sizeof(pOut1->bidho3), TRUE);				// 매수호가3
	strOut += CharToString(pOut1->bidho4, sizeof(pOut1->bidho4), TRUE);				// 매수호가4
	strOut += CharToString(pOut1->bidho5, sizeof(pOut1->bidho5), TRUE);				// 매수호가5
	strOut += CharToString(pOut1->bidrem1, sizeof(pOut1->bidrem1), TRUE);			// 매수잔량1
	strOut += CharToString(pOut1->bidrem2, sizeof(pOut1->bidrem2), TRUE);			// 매수잔량2
	strOut += CharToString(pOut1->bidrem3, sizeof(pOut1->bidrem3), TRUE);			// 매수잔량3
	strOut += CharToString(pOut1->bidrem4, sizeof(pOut1->bidrem4), TRUE);			// 매수잔량4
	strOut += CharToString(pOut1->bidrem5, sizeof(pOut1->bidrem5), TRUE);			// 매수잔량5
	strOut += CharToString(pOut1->totofferrem, sizeof(pOut1->totofferrem), TRUE);	// 총매도잔량
	strOut += CharToString(pOut1->totbidrem, sizeof(pOut1->totbidrem), TRUE);		// 총매수잔량
	strOut += CharToString(pOut1->tmofferrem, sizeof(pOut1->tmofferrem), TRUE);		// 시간외매도잔량
	strOut += CharToString(pOut1->tmbidrem, sizeof(pOut1->tmbidrem), TRUE);			// 시간외 매수잔량
	strOut += CharToString(pOut1->uplmtprice, sizeof(pOut1->uplmtprice), TRUE);		// 상한가
	strOut += CharToString(pOut1->dnlmtprice, sizeof(pOut1->dnlmtprice), TRUE);		// 하한가
	strOut += CharToString(pOut1->yerate, sizeof(pOut1->yerate));					// 예상등락율
	strOut += CharToString(pOut1->high, sizeof(pOut1->high), TRUE);					// 고가
	strOut += CharToString(pOut1->low, sizeof(pOut1->low), TRUE);					// 저가
	strOut += CharToString(pOut1->volrate, sizeof(pOut1->volrate), TRUE);			// 전일거래량
	strOut += CharToString(pOut1->jnilysign, sizeof(pOut1->jnilysign));				// 예상체결가전일종가대비구분
	strOut += "          ";															// 상장kospi발행kosdaq 주식수

	m_strOut = strOut;

	stReceiveFromServer stR;
	stR.m_strTRCode = QUERY_STOCK_HOGA;
	stR.m_pBuffer = (LPVOID)(LPCTSTR)m_strOut;
	stR.m_dwLength = m_strOut.GetLength();

	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd() && m_pInputBarDlg->m_pCodeInput)
	{
		CString strRecvCode = m_pInputBarDlg->m_pCodeInput->GetCode();
		m_stTrInfo2.m_pWndRequested->SendMessage(UMSG_RECEIVEFROMSERVER, (WPARAM)&strRecvCode, (long)&stR);
	}

	::memset(&m_stTrInfo2, 0x00, sizeof(m_stTrInfo2));
	m_strOut = "";

// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CStdDialog::ConvertHogaData: End\n", __LINE__);
// 		OutputDebugString(szDebug);
// 	}
}

// 종목을 변경하거나 등등의 경우 데이터를 새로 받게 된다. 이때 초기화처리를 해줘야하는 루틴이있다.
// 시스템트레이딩의 전략이 실행되고 있을 때 이러한 변경에 대한 처리를 해줘야 한다.
void CStdDialog::NofityNewDataReceive()
{
	// 시스템전략이 실행되고 있을 경우 전략을 클리어한다.
	// m_ChartMng.GetpChartItemSelected()->InvokeAddIn("ChartSTIndexAddIn.dll", 7, (long)this);

	// 전략리스트가 변경되었다는 내용을 알린다.
	//int nID = UUID_IAUDataMonitorManager;
	//IAUDataMonitorManager* pMng = (IAUDataMonitorManager*)AfxGetPctrInterface(nID);
	//pMng->NotifyData("MultiChart_ReceiveData", (long)this, NULL);
}

void CStdDialog::SetOneClickHide(BOOL bOneClickHide)
{
	m_bOneClickHide = bOneClickHide;
	CRect rect;	
	CRect chartMngRect;

	//@m_pLeftBarDlg가 생성되지 않은 경우에도 OneClick Show/Hide동작해야 함.
	//if(!m_pInputBarDlg||!m_pExtInputBarDlg||!m_pLeftBarDlg||!m_ChartMng.GetSafeHwnd()) return;
	if(!m_pInputBarDlg||!m_pInputBarDlg->GetSafeHwnd()||!m_pExtInputBarDlg||!m_ChartMng.GetSafeHwnd()) return;
	rect = GetRect(ID_INPUTBAR,TRUE);
	m_pInputBarDlg->MoveWindow(rect);
	m_pInputBarDlg->SetLeftBarType(m_nLeftBarType);

	rect = GetRect(ID_EXTINPUTBAR,TRUE);
	if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
		m_pExtInputBarDlg->MoveWindow(rect);

	if(m_bOneClickHide)
	{		
		m_pInputBarDlg->ShowWindow(SW_HIDE);
		if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
			m_pExtInputBarDlg->ShowWindow(SW_HIDE);
		if( m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd() )
			m_pLeftBarDlg->ShowWindow(SW_HIDE);
		m_xSplitter1.ShowWindow(SW_HIDE);
		if(m_ChartMng.GetSafeHwnd())
		{
			m_ChartMng.GetWindowRect(chartMngRect);
			ScreenToClient(chartMngRect);
			CRect rectNew = GetRect(ID_CHART,TRUE);
			rect = GetRect(ID_LEFTBAR,TRUE);
			chartMngRect.left	= rect.left;		//ChartMng
			chartMngRect.right	= rectNew.right;	//ChartMng
			chartMngRect.bottom	= rectNew.bottom;		
			chartMngRect.top	= rect.top;				

			m_ChartMng.MoveWindow(chartMngRect);
		}
	}
	else
	{
		m_pInputBarDlg->ShowWindow(SW_SHOW);
		if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
			m_pExtInputBarDlg->ShowWindow(SW_SHOW);

		rect = GetRect(ID_LEFTBAR,TRUE);
		if(rect.IsRectNull())
		{
			if( m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd() )
				m_pLeftBarDlg->ShowWindow(SW_HIDE);
		}
		else
		{
			if(m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd())
			{
				m_pLeftBarDlg->MoveWindow(rect);
				m_pLeftBarDlg->ShowWindow(SW_SHOW);
			}
		}

		rect = GetRect(ID_SPLITTER,TRUE);
		if(rect.IsRectNull())
		{
			m_xSplitter1.ShowWindow(SW_HIDE);
		}
		else
		{
			m_xSplitter1.MoveWindow(rect);
			m_xSplitter1.ShowWindow(SW_SHOW);
		}	

		if(m_ChartMng.GetSafeHwnd())
		{
			m_ChartMng.GetWindowRect(chartMngRect);
			ScreenToClient(chartMngRect);
			CRect rectNew = GetRect(ID_CHART,TRUE);
			if(chartMngRect!=rectNew)	
			{
				chartMngRect.left	= rectNew.left;		//ChartMng
				chartMngRect.right	= rectNew.right;	//ChartMng
				chartMngRect.bottom	= rectNew.bottom;		
				chartMngRect.top	= rectNew.top;				
			}

			m_ChartMng.MoveWindow(chartMngRect);
		}
	}
}

BOOL	CStdDialog::GetRevisedPriceFlag( void)	//ocx 수정주가사용여부 받아가기
{
	return m_ChartMng.RevisedPriceFlag();
}
BOOL	CStdDialog::SetRevisedPriceFlag( BOOL p_bRevisedFlag)	//ocx 수정주가사용여부 설정
{
	m_ChartMng.SendMessage(UMSG_UPDATEREVISED, p_bRevisedFlag);
	return TRUE;
}
BOOL	CStdDialog::OnIndicatorAddOrRemoved( const char *p_szGraphName,	const BOOL p_bAdded)
{
	return FALSE;
}
BOOL	CStdDialog::OnUserZoomChanged( const int p_nDataCountInView)
{
//@Solomon	100104SM015	-->
//	if(m_pInputBarDlg) // 2005. 04. 22 by Nam
//		m_pInputBarDlg->SetViewCount(p_nDataCountInView);
	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		m_pInputBarDlg->SendMessage(UMSG_SETZOOMVALUE, p_nDataCountInView, TRUE);
//@Solomon	100104SM015	<--

	return FALSE;
}
BOOL	CStdDialog::OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn)
{
	return FALSE;
}

void	CStdDialog::GridColumnExtend()
{
	
	CRect rect;
	GetClientRect(rect);
	rect.DeflateRect(BOUND_GAP,BOUND_GAP,BOUND_GAP,BOUND_GAP);

	CRect rectSplitter1;
	CRect rectLeftBar;
	if( m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd() )
		m_pLeftBarDlg->GetWindowRect(rectLeftBar);
	m_xSplitter1.GetWindowRect(rectSplitter1);
	ScreenToClient(rectSplitter1);	

	if (m_nExtendState == 0)
	{
		rect.left = 245;
		rect.right = 250;    

		m_nWidthOfLeftBar += 75;
		m_nExtendState = 1;
	}
	else
	{
		rect.left = 162;
		rect.right = 167; 

		m_nWidthOfLeftBar -= 75;
		m_nExtendState = 0;
	}
	
	m_xSplitter1.MoveWindow(rect);
	m_xSplitter1.RecalcLayout();
	if( m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd() )
		m_pLeftBarDlg->Invalidate();
	m_ChartMng.Invalidate();
	m_xSplitter1.Invalidate();

	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		m_pInputBarDlg->Invalidate();
	if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
		m_pExtInputBarDlg->Invalidate();

	if(	(m_bShowDataListWnd				) &&
		(m_pDataWindow != NULL			) &&
		(m_pDataWindow->GetSafeHwnd()	) 
	)
	{
		if(m_ChartMng.GetSafeHwnd())
		{	

			CRect rect2;
			m_ChartMng.GetWindowRect(rect2);	
			ScreenToClient(rect2);
			m_ViewRect.left = rect2.left;
			m_pDataWindow->MoveWindow(m_ViewRect);	
		}
	}

	if(m_ChartMng.m_pFloatingDlg != NULL)				
		m_ChartMng.m_pFloatingDlg->Invalidate();

	return;
}

BOOL CStdDialog::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

BOOL CStdDialog::IsSystemChart()
{
	//시스템화면으로 열어볼 수 있는 키값 설정.
	{
		CString szIniFile = m_strRootPath + "\\Icss\\CHARTDEF.CFG";
		int nSysChart = ::GetPrivateProfileInt("STBasic", "SystemTradingMode", 0, szIniFile);
		if(nSysChart==999) m_bSystemChart=TRUE;
	}

	if(!m_bSystemChart) return m_bSystemChart;

	// 이루틴은 헤더에서 옮길예정임. 07.01.20 JunokLee
	// 98,Me,95에서는 시스템트레이딩을 제공하지 않는다.
	if(CAZUtilCtrl::GetOSPlatformID()==VER_PLATFORM_WIN32_WINDOWS)
		return FALSE;
	
/*
	IMainInfoManager* pMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
	if(pMng)
	{
		int nUserType = atoi(pMng->GetLoginInfo("ISHTS"));	//1(HTS), 0(지점)
		if(nUserType==0) return m_bSystemChart; // TRUE리턴
	}
*/

// 여기에서 시스템트레이딩 교육여부를 체크한다.	
// 	int nSysEdu=-1;
// 	gGetSamsungSysEdu(nSysEdu);
// 
// 	// 시스템탭은 항상 보이도록 한다. 2007.01.07
// 	if(nSysEdu>=0)
// 		return m_bSystemChart;
// 	else
// 	{
// 		ICompanyManager_SS* pMng = (ICompanyManager_SS*)AfxGetPctrInterface(UUID_ICompanyManager);
// 		if(pMng)
// 		{
// 			LPCSTR szMainKey = "SYSEDUE";
// 			nSysEdu = pMng->GetSysEdu(szMainKey, (long)this, (long)NULL, NULL);
// 		}
// 	}
	return m_bSystemChart;
}


// 시스템트레이딩 전략 저장/불러오기
void CStdDialog::LoadSaveSystemTrading(CString& szSTPath, int nRow, int nCol, BOOL bSave)
{
	if(bSave && nRow >= 0 && nCol >= 0)
		szSTPath.Empty();
	else
	{
		if(szSTPath.IsEmpty() || szSTPath.Find(".ST") < 0)
			return;
		
		if(m_pLeftBarDlg == NULL || !m_pLeftBarDlg->GetSafeHwnd())
			return;
		
		// 시스템탭 로딩
		m_pLeftBarDlg->m_tabctrl.SetWndSystemTabEx();
		CTabSystemTradingEx *pTabSystemTabEx = (CTabSystemTradingEx*)m_pLeftBarDlg->m_tabctrl.GetWndSystemTabEx();
		if(pTabSystemTabEx)
		{
			if(bSave)
			{
				CChartItem* pCItem = m_ChartMng.GetChartItem(nRow, nCol);
				if(pCItem && m_ChartMng.GetSTRunState() == CChartMng::ST_STSAVING)
					szSTPath = pCItem->GetSTRunSTPath();
			}
			else
			{
				pTabSystemTabEx->DirectRunST(szSTPath);
			}
		}
	}
}

int CStdDialog::GetWidthOfLeftBar()
{
	return m_nWidthOfLeftBar;
}

// 복수종목 리얼 해제 : 복수종목 - ojtaso (20070209)
void CStdDialog::OnDeleteChartItem(LPCSTR lpszItemCode)
{
//@유진추가
	OnCodeUnAdvise(lpszItemCode, TRUE);
//@유진추가
}

// 종목명 얻기 : 복수종목 - ojtaso (20080213)
CString CStdDialog::GetJongMokNameByCode(LPCSTR lpszItemCode, LPCTSTR lpszTRCode/* = NULL*/)
{
	if(!m_pInputBarDlg||!m_pInputBarDlg->GetSafeHwnd())
		return _T("");

	return m_pInputBarDlg->GetJongMokNameByCode(lpszItemCode, lpszTRCode);
}

// 복수종목 선택 : 복수종목 - ojtaso (20080215)
void CStdDialog::OnSelectChartItem(ST_SELCHART* pSelChart)
{
	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		m_pInputBarDlg->InitChartMulitItem(pSelChart);
	if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
		m_pExtInputBarDlg->InitChartMulitItem(pSelChart);

	char cType = m_pInputBarDlg->GetType();
	if(cType == MIN_DATA_CHART || cType == SEC_DATA_CHART || cType == TICK_DATA_CHART)
		EnableQueryOpt(TRUE);
	else
		EnableQueryOpt(FALSE);

	if(cType == TICK_DATA_CHART || 
		cType == SEC_DATA_CHART || 
		cType == WEEK_DATA_CHART || 
		cType == MONTH_DATA_CHART)
		EnableAccount(FALSE);
	else
		EnableAccount(TRUE);

	//SetItemCodeName(pSelChart->m_strItemCode);
}

// 현재 종목과 선택된 종목이 같은지 : 복수종목 - ojtaso (20070223)
BOOL CStdDialog::IsSameItemCodeName(LPCTSTR lpszItemName)
{
	return !::lstrcmp(lpszItemName, m_pInputBarDlg->GetCurCodeNameFromControl());
}

// 상품선물옵션차트 - ojtaso (20071001)
USHORT CStdDialog::GetDotPosition(CString& strItemCode)
{
	if(IS_CALL_CODE_1(strItemCode.GetAt(0)) || IS_PUT_CODE_1(strItemCode.GetAt(0))) // 0024696: 파생상품 코드체계 변경 제도개선
		return 1;

	USHORT nDotPos = 2;

	if(strItemCode.GetAt(1) == _T('7') || strItemCode.GetAt(1) == _T('U') || strItemCode.GetAt(1) == _T('Y') || strItemCode.GetAt(1) == _T('E'))
		nDotPos = 1;
	else if(strItemCode.GetAt(1) == _T('8') || strItemCode.GetAt(1) == _T('G'))
		nDotPos = 0;

	return nDotPos;
}

// 분차트 마지막 키인 경우 - ojtaso (20071226)
void CStdDialog::EnableRequestMoreButton(BOOL bEnable)
{
	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		m_pInputBarDlg->EnableRequestMoreButton(bEnable);
}

#ifdef _WRITELOG
// Log - ojtaso (20080107)
void CStdDialog::WriteToStringLog( const char *p_szSection, const char *p_szData, int p_nDataLen)
{
	if( m_hLogFile == HFILE_ERROR) return;

	// 1. 먼저 Logging하는 시각을 확인한다.
	SYSTEMTIME sTime;
	GetLocalTime(&sTime);

	// 4. 주어진 Section Value을 확인하여 Section String을 구성한다.
	CString strBuffer;
	if( p_szSection && *p_szSection != '\0')
	{
		CString strCurTime;
		// (2005/9/1 - Ssyng-Won, Bae) Line Skip for Only Section.
		strCurTime.Format("\r\n\r\n\r\n[%02d:%02d:%02d:%03d  ", sTime.wHour, sTime.wMinute, sTime.wSecond, 
			sTime.wMilliseconds);
		strBuffer = strCurTime + p_szSection + "]\r\n";
		_hwrite( m_hLogFile, ( LPCTSTR)strBuffer, strBuffer.GetLength());
	}
	// (2005/9/2 - Ssyng-Won, Bae) in No Section Log, Insert Time in front of Line
	else
	{
		CString strCurTime;
		strCurTime.Format("%02d:%02d:%02d:%03d\t", sTime.wHour, sTime.wMinute, sTime.wSecond, 
			sTime.wMilliseconds);
		_hwrite( m_hLogFile, strCurTime, strCurTime.GetLength());
	}

	// 5. Memory를 Dump한다.
	_hwrite( m_hLogFile, p_szData, p_nDataLen);
}

// Log - ojtaso (20080107)
void CStdDialog::CreateLogFile()
{
	// 0. Parameter 값을 조정하기 위하여, 보조 Variable을 이용한다.
	const char *szLogFolderPath	 = "C:\\Logs";
	// 1. 먼저 Logging하는 시각을 확인한다.
	SYSTEMTIME sTime;
	GetLocalTime(&sTime);

	// 2.3 Log File Name을 구성한다.
	CString strFileName;
	strFileName.Format( "%s\\MultiChart%04d_%04d%02d%02d.log", szLogFolderPath, m_nScreenID, sTime.wYear, sTime.wMonth, sTime.wDay);
	// 3. szLogFolderPath를 관리할 필요가 없도록 미리 처리한다.
	::CreateDirectory( szLogFolderPath, NULL);

	// 3. Log File을 Open한다.
	m_hLogFile = _lopen( ( LPCTSTR)strFileName, OF_READWRITE);
	if( m_hLogFile != HFILE_ERROR)
	{
		// 3.1 이전 File이 있으면 File 끝으로 이동한다.
		if( _llseek( m_hLogFile, 0L, FILE_END) == HFILE_ERROR) return;
	}
	// 3.2 이전 File이 없으면 새로이 생성한다.
	else m_hLogFile = _lcreat( ( LPCTSTR)strFileName, 0);
}

// Log - ojtaso (20080107)
void CStdDialog::CloseLogFile()
{
	// 6. Dump File을 닫는다.
	if( m_hLogFile != HFILE_ERROR) _lclose( m_hLogFile);
}
#endif

void CStdDialog::OnBtnExtTool() 
{
	m_bShowToolBar = !m_bShowToolBar;

	if(m_bShowToolBar)
	{
		m_btnExtTool.FP_vSetImage( &m_ImgList_W8H25, 2, 3);
		m_btnExtTool.FP_vSetToolTipText("툴바축소");
	}
	else
	{
		m_btnExtTool.FP_vSetImage( &m_ImgList_W8H25, 0, 1);
		m_btnExtTool.FP_vSetToolTipText("툴바확대");
	}
	m_btnExtTool.Invalidate();
	Resize();
}


//-->@Solomon090819 
// 	long CStdDialog::OnGetBroad(WPARAM wParam, LPARAM lParam)
// 	{
// 		if(m_pDrdsLib == NULL)
// 			return 0;
// 
// 
// 		BC_ITEM_INFO *pRealItem = (BC_ITEM_INFO*)lParam;
// 
// 	#ifdef _WRITELOG
// 			// Log - ojtaso (20080107)
// 			WriteToStringLog(_T("CStdDialog::OnGetBroad"), pRealItem->szBcCode, strlen(pRealItem->szBcCode));
// 	#endif
// 
// 		if(strncmp(pRealItem->szBcCode, NAME_KXS3, strlen(NAME_KXS3)) == 0)			// 주식현재가
// 		{
// 			CRealData_Common realData_Common;
// 			CChartRealConverter::Convert_KXS3(pRealItem->szBcData, realData_Common);
// 			m_ChartMng.SetRealJPriceData((LPVOID)&realData_Common);
// 			return 1L;
// 		}
// 		else if(strncmp(pRealItem->szBcCode, NAME_KXYS, strlen(NAME_KXYS)) == 0)	// 주식 예상
// 		{
// 			CRealData_Common realData_Common;
// 			CChartRealConverter::Convert_KXYS(pRealItem->szBcData, realData_Common);
// 			m_ChartMng.SetRealYPriceData((LPVOID)&realData_Common);
// 			return 1L;
// 		}
// 		else if(strncmp(pRealItem->szBcCode, NAME_EWs3, strlen(NAME_EWs3)) == 0)	// ELW현재가
// 		{	
// 			CRealData_Common realData_Common;
// 			CChartRealConverter::Convert_EWs3(pRealItem->szBcData, realData_Common);
// 			m_ChartMng.SetRealJPriceData((LPVOID)&realData_Common);
// 			return 1L;
// 		}
// 		else if(strncmp(pRealItem->szBcCode, NAME_IXIJ, strlen(NAME_IXIJ)) == 0)	// 업종현재가
// 		{	
// 			CRealData_Common realData_Common;
// 			CChartRealConverter::Convert_IXIJ(pRealItem->szBcData, realData_Common);
// 			m_ChartMng.SetRealJPriceData((LPVOID)&realData_Common);
// 			return 1L;
// 		}
// 		else if(strncmp(pRealItem->szBcCode, NAME_FFC0, strlen(NAME_FFC0)) == 0)	// 선물현재가
// 		{	
// 			CRealData_Common realData_Common;
// 			CChartRealConverter::Convert_FFC0(pRealItem->szBcData, realData_Common);
// 			m_ChartMng.SetRealJPriceData((LPVOID)&realData_Common);
// 
// 	/*
// 	#ifdef _WRITELOG			// Log - ojtaso (20080107)
// 			CString strData;
// 			strData.Format("KR4101D60000|%s000|%s|%s|%s|%s|0|%s|0|0|0|%s|0|%s|0|%s0|0|0|0|0|0|0|0|0|0|0|0|0|0|0|1|0|0|0|0|0|0|0|0|0|0|000000000|0|000000000|0|000000000|0|000000000|0|000000000|0|000000000|0|000000000|0|000000000|", 
// 								realData_Common.m_strTime,
// 								realData_Common.m_strPrice,
// 								realData_Common.m_strSign,
// 								realData_Common.m_strChange,
// 								realData_Common.m_strChrate,
// 								realData_Common.m_strVolume,
// 								realData_Common.m_strOpen,
// 								realData_Common.m_strHigh,
// 								realData_Common.m_strLow);
// 
// 		WriteToStringLog(_T("FurturRealData"), strData, strData.GetLength());
// 	#endif			
// 	*/
// 			return 1L;
// 		}
// 		else if(strncmp(pRealItem->szBcCode, NAME_OOC0, strlen(NAME_OOC0)) == 0)	// 옵션현재가
// 		{	
// 			CRealData_Common realData_Common;
// 			CChartRealConverter::Convert_OOC0(pRealItem->szBcData, realData_Common);
// 			m_ChartMng.SetRealJPriceData((LPVOID)&realData_Common);
// 			return 1L;
// 		}
// 		else if(strncmp(pRealItem->szBcCode, NAME_JFC0, strlen(NAME_JFC0)) == 0)	// 주식선물
// 		{	
// 			CRealData_Common realData_Common;
// 			CChartRealConverter::Convert_JFC0(pRealItem->szBcData, realData_Common);
// 			m_ChartMng.SetRealJPriceData((LPVOID)&realData_Common);
// 			return 1L;
// 		}
// 		else if(strncmp(pRealItem->szBcCode, NAME_JOC0, strlen(NAME_JOC0)) == 0)	// 주식옵션
// 		{	
// 			CRealData_Common realData_Common;
// 			CChartRealConverter::Convert_JOC0(pRealItem->szBcData, realData_Common);
// 			m_ChartMng.SetRealJPriceData((LPVOID)&realData_Common);
// 			return 1L;
// 		}
// 		else if(strncmp(pRealItem->szBcCode, NAME_HWFJ, strlen(NAME_HWFJ)) == 0)	// 해외
// 		{
// 			CRealData_Common realData_Common;
// 			CChartRealConverter::Convert_HWFJ(pRealItem->szBcData, realData_Common);
// 			m_ChartMng.SetRealJPriceData((LPVOID)&realData_Common);
// 			return 1L;
// 		}
// 		return 0L;
// 	}
// 	//<--@Solomon090819 

int CStdDialog::GetMarketTypeFromCode(CString strItemCode, int& nCodeType) 
{
//@solomon	FO지수 차트의 종목공유시 다른 종목이어도 공유되는 에러 수정.
//  	if(IsChartScreen("FUTOPT_CHART")==TRUE)
//  	{
//  		nCodeType = CDRCodeTypeInfo::CODE_KFUTURE;
// 		return CMasterTypeInfo::FUOPT_JIPYO_CHART;
//  	}
	int nRetType;
	nCodeType = -1;
	if(strItemCode.IsEmpty())
		return -1;

	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	if (pMasterDataManager)
	{
		nRetType = pMasterDataManager->GetMarketTypeFromCode(strItemCode);
		nCodeType = pMasterDataManager->GetCodeTypeFromCode(strItemCode);

		if (nRetType == CMasterTypeInfo::NONE_CHART)
		{
			if( strlen(strItemCode)==8 && IS_PUT_CODE_1(strItemCode[0])) // 0024696: 파생상품 코드체계 변경 제도개선
			{
				nCodeType = CDRCodeTypeInfo::CODE_KOPTION;
				nRetType = CMasterTypeInfo::FUTOPT_CHART;	
				return nRetType;
			}

			if (!m_pInputBarDlg || !m_pInputBarDlg->m_pFOHisBtn)
				return nRetType;

			nCodeType = m_pInputBarDlg->m_pFOHisBtn->GetCodeType(strItemCode);
			nRetType = CMasterTypeInfo::FUTOPT_CHART;
			switch (nCodeType)
			{
			case futureType:	// 선물
				nCodeType = CDRCodeTypeInfo::CODE_KFUTURE;
//				nRetType = CMasterTypeInfo::FUTOPT_CHART;
				break;
			case spreadType:	// 스프레드
				nCodeType = CDRCodeTypeInfo::CODE_FUTURE_SPREAD;
//				nRetType = CMasterTypeInfo::FUTOPT_CHART;
				break;
			case sfutureType:	// 주식선물
				nCodeType = CDRCodeTypeInfo::CODE_STOCKFUT;
				nRetType = CMasterTypeInfo::STOCKFUTOPT_CHART;
				break;
			case koptionType:	// 주식옵션
				nCodeType = CDRCodeTypeInfo::CODE_STOCKOPT;
				nRetType = CMasterTypeInfo::STOCKFUTOPT_CHART;
				break;
			case callType:		// 콜옵션
			case putType:		// 풋옵션
				nCodeType = CDRCodeTypeInfo::CODE_KOPTION;
//				nRetType = CMasterTypeInfo::FUTOPT_CHART;
				break;
			default:
				nRetType = CMasterTypeInfo::NONE_CHART;
			}
		}
	}

	return nRetType;
}

// 돋보기 구현 - ojtaso (20080519)
void CStdDialog::OnMagnifyingglass(BOOL bShow)
{
	if(bShow)
	{
		if(!m_pDlgGlass)	
		{
			m_pDlgGlass = new CDlgGlass(&m_ChartMng);
			
			AFX_MANAGE_STATE(AfxGetStaticModuleState());
			//HINSTANCE hInstResourceClient = AfxGetResourceHandle();
			//AfxSetResourceHandle(g_hInstance);

			m_pDlgGlass->Create(IDD_MAGNIFYINGGLASS, &m_ChartMng);
	
			//AfxSetResourceHandle(hInstResourceClient);
		}
		//hide만 시켜놓은 경우는 다시 비트맵을 만들어 주지 않으면.
		//hide 이전의 값을 가지고 있는다.
		else 
			m_pDlgGlass->MakeBitmap();
		
		m_pDlgGlass->ShowWindow(SW_SHOW);
		m_pDlgGlass->BringWindowToTop();
	}
	else
	{
		if(m_pDlgGlass)
			m_pDlgGlass->ShowWindow(SW_HIDE);

		if(m_pDlgToolbar && m_pDlgToolbar->GetSafeHwnd())
			m_pDlgToolbar->SetToggleButton(50073, 0);

		SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION, TRUE);
	}
}

void CStdDialog::CreateBufferBitmap(CDC* pdc, const CRect& rcBounds)
{
	if (m_bufferBitmap.GetSafeHandle() == NULL)
	{
		m_bufferBitmap.CreateCompatibleBitmap(pdc, rcBounds.Width(), rcBounds.Height());
	}
	else
	{
		CSize bitmapSize = m_bufferBitmap.GetBitmapDimension();
		CSize boundsSize = rcBounds.Size();
		if (bitmapSize.cx < boundsSize.cx || bitmapSize.cy < boundsSize.cy)
		{
			m_bufferBitmap.DeleteObject();
			m_bufferBitmap.CreateCompatibleBitmap(pdc, boundsSize.cx, boundsSize.cy);
		}
	}
}

void CStdDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_pDlgGlass && m_pDlgGlass->IsWindowVisible())
	{
		m_pDlgGlass->SetWindowPos( NULL, point.x, point.y, 0, 0, 
				SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
	}
	CExOCXDLL::OnMouseMove(nFlags, point);
}

BOOL CStdDialog::GetDlgToolBtnToggle(CArray<int, int>& arAnal, CArray<int, int>& arAnalCheck, CArray<int, int>& arAddition, CArray<int, int>& arAdditionCheck, CArray<int, int>& arUser, CArray<int, int>& arUserCheck, CMap<int, int, CString, CString>& mapToolName)
{
	if(m_pDlgToolbar && m_pDlgToolbar->GetSafeHwnd())
	{
		m_pDlgToolbar->GetIndexArray(0, arUser, arUserCheck);
		m_pDlgToolbar->GetIndexArray(1, arAnal, arAnalCheck);
		m_pDlgToolbar->GetIndexArray(2, arAddition, arAdditionCheck);
		m_pDlgToolbar->GetToolNameMap(mapToolName);
	}

	return TRUE;
}

void CStdDialog::SetDlgToolBtnToggle(int nToolBtnID, BOOL bToggle)
{
	if(m_pDlgToolbar && m_pDlgToolbar->GetSafeHwnd())
		m_pDlgToolbar->SetToggleBtnFromOutside(nToolBtnID);
}

#define getPALETTE 0x01 
#define winix_GetColor(nIdx)	(COLORREF)::SendMessage(m_hPlatform, WM_USER, MAKEWPARAM(getPALETTE, 0), nIdx);

LRESULT CStdDialog::OnChangeSkinColor(WPARAM wParam, LPARAM lParam)
{
	m_clrTabBkgrnd = m_clrBackground;
	m_clrOLBkgrnd = winix_GetColor(9);
	m_clrOLOutLine = winix_GetColor(10);

	g_pPtUtilMng->GetCurrentPalette();

	ChangeSkinColor();
	return 0;
}

void CStdDialog::ChangeSkinColor()
{
	if((m_nChartType < MINI_MULTI_CHART) && m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd())
		m_pLeftBarDlg->ChangeSkinColor();

	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		m_pInputBarDlg->ChangeSkinColor();
	if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
		m_pExtInputBarDlg->ChangeSkinColor();
	m_xSplitter1.Invalidate();
	if(m_pDlgToolbar && m_pDlgToolbar->GetSafeHwnd())
		m_pDlgToolbar->Invalidate();

	m_btnSideBar.SetBorderColor(RGB(157,157,161));
	m_btnSideBar.SetBackGroundColor(GetBkgrndColor(), GetBkgrndColor());
	m_btnSideBar.Invalidate();

	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
	{
		m_pInputBarDlg->ShowWindow(SW_HIDE);
		m_pInputBarDlg->ShowWindow(SW_SHOW);
	}
}

COLORREF CStdDialog::GetBkgrndColor()
{
	return winix_GetColor(clridx_BackGround);
}

LRESULT CStdDialog::OnGetSkinColor(WPARAM wParam, LPARAM lParam)
{
	int nColorType = (int)wParam;
	if(nColorType == 0)				// 배경색
		return (LRESULT)GetBkgrndColor();
	else if(nColorType == 1)		// Tab
	{
		STTabColor* pTabColor = (STTabColor*)lParam;
		if(pTabColor)
		{
			pTabColor->clrTabSelText = winix_GetColor(1);
			pTabColor->clrTabNorText = winix_GetColor(2);
			pTabColor->clrTabBkgrnd  = winix_GetColor(clridx_BackGround);

			g_pPtUtilMng->GetTabColor("", 0, pTabColor->clrTabSelGDStt, pTabColor->clrTabSelGDEnd);
			g_pPtUtilMng->GetTabColor("", 1, pTabColor->clrTabNorGDStt, pTabColor->clrTabNorGDEnd);

			return TRUE;
		}
		return FALSE;
	}
	else if(nColorType == 2)
	{
		STOutBarColor* pOLColor = (STOutBarColor*)lParam;
		if(pOLColor)
		{
			pOLColor->clrOLBkgrnd =RGB(237,237,237);//KHD m_clrOLBkgrnd;
			pOLColor->clrOLOutLine = m_clrOLOutLine;
			pOLColor->clrSelecBkgrnd = RGB(255,246,182);//KHD IBK 요청 : 김진아대리
			return TRUE;
		}
		return FALSE;
	}

	return DEFAULT_BKGRND_COLOR;
}

CString CStdDialog::GetLinkInfoData()
{
	AfxMessageBox("[GetLinkInfoData]Solomon수정필요"); //@Solomon-091015

	CString strCode;
	if(m_hViewInMain)
		::SendMessage(m_hViewInMain, RMSG_GET_LINKINFODATA, (WPARAM)0, (LPARAM)&strCode);

	return strCode;
}

void CStdDialog::SaveChart()
{
	SaveShowInfo();

	m_ChartMng.SaveChart(m_strConfig);
	if(m_nChartType < MINI_MULTI_CHART)
		m_pLeftBarDlg->SetConfig(FALSE);
	m_pInputBarDlg->SaveInputBar();
	if(m_nChartType == MULTI_CHART)
		if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
			m_pExtInputBarDlg->SaveExtInputBar();
}

//유진: 당일데이터는 뉴스서버에 존재함 그래서 p0602 대신에 p0612로 수정. 나머지도 마찬가지임.
void CStdDialog::DoCheckMin(int nType, LPCTSTR lpData, CString& szReqTr)
{
	if(nType==2)
	{
		KB_p0602_InRec1* pInRec1 = (KB_p0602_InRec1*)&lpData[80];
		if(pInRec1->nowData[0]=='0' && pInRec1->button[0]=='1') szReqTr="p0612";
		return;
	}
	if(nType==3)
	{
		KB_p0603_InRec1* pInRec1 = (KB_p0603_InRec1*)&lpData[80];
		if(pInRec1->nowData[0]=='0' && pInRec1->Sbtngb[0]=='1') szReqTr="p0613";
		return;
	}
	if(nType==4)
	{
		KB_p0604_InRec1* pInRec1 = (KB_p0604_InRec1*)&lpData[80];
		if(pInRec1->nowData[0]=='0' && pInRec1->Sbtngb[0]=='1') szReqTr="p0614";
		return;
	}
	if(nType==5)
	{
		KB_d1013_InRec1* pInRec1 = (KB_d1013_InRec1*)&lpData[80];
		if(pInRec1->nowData[0]=='0' && pInRec1->Sbtngb[0]=='1') szReqTr="p0615";
		return;
	}
}

CString CStdDialog::GetItemNameFromItemCode(CString strItemCode, BOOL bPreFix)
{
	CString strCodeName = _T("");
	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	if(pMasterDataManager)
	{
		strCodeName = pMasterDataManager->GetCodeName(strItemCode);

		if (!bPreFix)
			strCodeName.Replace("&&", "&");
	}

	return strCodeName;
}

BOOL CStdDialog::GetOldFututureCodeName(LPCSTR szCode, CString& szCodeName) 
{
	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
	{
		return m_pInputBarDlg->GetOldFututureCodeName(szCode, szCodeName);
	}
	else
	{
		return FALSE;
	}
}

int CStdDialog::GetMarketTypeFromItemCode(CString strItemCode)
{
	if(strItemCode.IsEmpty())
		return -1;

	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	if(pMasterDataManager)
	{
		int nRetType = pMasterDataManager->GetMarketTypeFromCode(strItemCode);
		
		if (nRetType == CMasterTypeInfo::NONE_CHART)
		{
			if (!m_pInputBarDlg || !m_pInputBarDlg->m_pFOHisBtn)
				return nRetType;
			
			nRetType = m_pInputBarDlg->m_pFOHisBtn->GetCodeType(strItemCode);
			switch (nRetType)
			{
			case futureType:	// 선물
			case callType:		// 콜옵션
			case putType:		// 풋옵션
				return CMasterTypeInfo::FUTOPT_CHART;
			case sfutureType:	// 주식선물
			case koptionType:	// 주식옵션
				return CMasterTypeInfo::STOCKFUTOPT_CHART;
			}
		}

		return nRetType;
	}

	return CMasterTypeInfo::NONE_CHART;

// 	int nEnumMarketType = 0;
// 	if(m_hViewInMain)
// 		nEnumMarketType = ::SendMessage(m_hViewInMain, RMSG_GET_MARKETTYPEFORMITEMCODE, (WPARAM)&strItemCode, (LPARAM)&nEnumMarketType);
// 
// 	return nEnumMarketType;
}

BOOL CStdDialog::IsAvailableReplayRunnig(int nEnumMarketType, int nEnumItemAddType)
{
	// 복기모드시 주식, 선옵만 가능
	if(m_pInputBarDlg->GetCurrentMode() == REPLAY_ITEM)
	{
		if(nEnumMarketType == CMasterTypeInfo::STOCK_CHART || nEnumMarketType == CMasterTypeInfo::FUTOPT_CHART)
		{
			if(nEnumItemAddType == NEW_CHART)
			{
				AfxMessageBox("복기모드시 종목추가할 수 없습니다.");
				return FALSE;
			}
			else if(nEnumItemAddType == OVERLAPP_ITEM)
			{
				AfxMessageBox("복기모드시 종목중첩할 수 없습니다.");
				return FALSE;
			}
		}
		else
		{
			AfxMessageBox("복기모드는 주식과 선물/옵션 종목만 가능합니다.");
			return FALSE;
		}
	}
	
	return TRUE;
}

CString CStdDialog::MakeMarketChartData(LPVOID aTRData, int& nCount, int dwTRDataLen, double dFlag)
{
	char* szChartData = NULL;
	char* pszRcv = NULL;
	CString strChartData;
	int nBufferLength = 0;

	pszRcv = (char*)aTRData;
	KB_d1015_OutRec1* pOutRec = (KB_d1015_OutRec1*)aTRData;

	if(pOutRec->inNumGb[0] == '2')
	{
		KB_d1015_OutRec2_1* pChartData = NULL;

		nBufferLength = sizeof(KB_d1015_OutRec2_1) * nCount + 1;
		szChartData = strChartData.GetBuffer(nBufferLength);

		pszRcv += sizeof(KB_d1015_OutRec1);
		for(int i = 0; i < nCount; ++i)
		{
			pChartData = (KB_d1015_OutRec2_1*)((char*)pszRcv + (sizeof(KB_d1015_OutRec2_1) * (nCount-i-1)));

			memcpy(szChartData, (LPCSTR)pChartData, sizeof(KB_d1015_OutRec2_1));
			szChartData += sizeof(KB_d1015_OutRec2_1);
		}
		strChartData.ReleaseBuffer();
	}
	else if(pOutRec->inNumGb[0] == '4')
	{
		KB_d1015_OutRec2_3* pChartData = NULL;

		nBufferLength = sizeof(KB_d1015_OutRec2_3) * nCount + 1;
		szChartData = strChartData.GetBuffer(nBufferLength);

		pszRcv += sizeof(KB_d1015_OutRec1);
		for(int i = 0; i < nCount; ++i)
		{
			pChartData = (KB_d1015_OutRec2_3*)((char*)pszRcv + (sizeof(KB_d1015_OutRec2_3) * (nCount-i-1)));

			memcpy(szChartData, (LPCSTR)pChartData, sizeof(KB_d1015_OutRec2_3));
			szChartData += sizeof(KB_d1015_OutRec2_3);
		}
		strChartData.ReleaseBuffer();
	}

	return strChartData;
}

int CStdDialog::GetAccountList(int nType, CStringArray* pAccountArray)
{
	AfxMessageBox("[GetAccountList]Solomon수정필요"); //@Solomon-091015
	return ::SendMessage(m_hViewInMain, RMSG_GET_ACCOUNTLIST, (WPARAM)nType, (LPARAM)pAccountArray);
} 

//IBK : 사용안함 : KHD
void CStdDialog::RequestAccountData(LPCTSTR lpszQueryTR, LPCSTR lpszData, int nLen)
{
// 	m_TRComm->SetTR(lpszQueryTR);
// 	m_TRComm->SetBaseData("PtManager", m_szPTManager);
// 	m_TRComm->SetBaseData("SCREENCODE", GetScreenNo());
// 
// 	m_TRComm->Send2Server((LPVOID)lpszData, nLen, TRKEY_FID_END);
}

void CStdDialog::RequestAvgUnitPrice()
{
	AfxMessageBox("[RequestAvgUnitPrice]Solomon수정필요"); //@Solomon-091015

	char szAccount[12] = {NULL, };
	::SendMessage(m_hViewInMain, RMSG_GET_ACCTNUM, (WPARAM)szAccount, 0);

	CString strCode = GetItemExpCode(m_pInputBarDlg->GetCurrentCode());

	a4017In sta4017In;
	::memset(&sta4017In, 0x20, sizeof(sta4017In));
	::memcpy(sta4017In.accno, szAccount, sizeof(sta4017In.accno));
	::memcpy(sta4017In.expcode, (LPCTSTR)strCode, sizeof(sta4017In.expcode));

	m_TRComm->SetTR(NAME_a4017);
	m_TRComm->SetBaseData("PtManager", m_szPTManager);
	m_TRComm->SetBaseData("SCREENCODE", GetScreenNo());
	m_TRComm->Send2Server((char*)&sta4017In, sizeof(sta4017In), TRKEY_FID_END);
}

void CStdDialog::RequestCondUnitPrice()
{
	AfxMessageBox("[RequestCondUnitPrice]Solomon수정필요"); //@Solomon-091015

	char szAccount[12] = {NULL, };
	::SendMessage(m_hViewInMain, RMSG_GET_ACCTNUM, (WPARAM)szAccount, 0);

	CString strCode = GetItemExpCode(m_pInputBarDlg->GetCurrentCode());

	a4018In sta4018In;
	::memset(&sta4018In, 0x20, sizeof(sta4018In));

	::memcpy(sta4018In.accno, szAccount, sizeof(sta4018In.accno));
	::memcpy(sta4018In.expcode, strCode, sizeof(sta4018In.expcode));
	
	CString strNumber, strDate, strTime;
	long lCount = 0;
	
	char cTemp;
	m_ChartMng.GetCurStatusInfo(cTemp, strNumber, strDate, strTime, lCount);
	
	CString strTemp;
	if(cTemp == '1')		// 분
	{
		sta4018In.gubun[0] = '2';

		strTime.Replace(":", "");
		strTemp.Format("%s00", strTime.Right(4));
		::memcpy(sta4018In.basetm, strTemp, sizeof(sta4018In.basetm));
	}
	else if(cTemp == '2')	// 일
	{
		sta4018In.gubun[0] = '1';
		if(atoi(strNumber) != 1)
		{
			m_ChartMng.RemoveConcPacketData();
			return;
		}
	}
	else 
		return;

	::memcpy(sta4018In.term, strNumber, sizeof(sta4018In.term));
	::memcpy(sta4018In.basedt, strDate, sizeof(sta4018In.basedt));
	
	strTemp.Format("%ld", lCount);
	::memcpy(sta4018In.cnt, strTemp, sizeof(sta4018In.cnt));
	
	m_ChartMng.AddConcPacketGraph();

	m_TRComm->SetTR(NAME_a4018);
	m_TRComm->SetBaseData("PtManager", m_szPTManager);
	m_TRComm->SetBaseData("SCREENCODE", GetScreenNo());
	m_TRComm->Send2Server((char*)&sta4018In, sizeof(sta4018In), TRKEY_FID_END);
}

void CStdDialog::AddAccountCodeItem(LPVOID aTRData, long dwTRDateLen)
{
	// 종목별 조회는 보유종목 1개
	LPa4017Out2 pCodeData = (LPa4017Out2)((char*)aTRData + 5);

	CString strCode, strPrice;
	strCode.Format("%6.6s", pCodeData->expcode + 3);
	strPrice.Format("%9.9s", pCodeData->pamt);
	strPrice.Replace(",", "");

	m_ChartMng.ShowAvgLine(strCode, strPrice);	
/*
	if(m_pLeftBarDlg)
		m_pLeftBarDlg->AddAccountCodeItem(aTRData, dwTRDateLen);
*/
}

void CStdDialog::MakeAccountConcData(LPVOID aTRData, long dwTRDateLen)
{
	if(m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd())
		m_pLeftBarDlg->MakeAccountConcData(aTRData, dwTRDateLen);
}

void CStdDialog::ResetChartData()
{
	if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
		m_pExtInputBarDlg->ResetChartData();
}

//void CStdDialog::SetItemCodeName(CString strItemCode)
//{
//@Solomon-091015	::SendMessage(m_hViewInMain, RMSG_SET_ITEMCODENAME, (WPARAM)&strItemCode, 0);
//}

BOOL CStdDialog::TrackPopupMenu() 
{
// 	AfxMessageBox("[TrackPopupMenu]Solomon수정필요"); //@Solomon-091015
// 
 	m_bChartPopupMenu = TRUE;
// 	return ::PostMessage(m_hViewInMain, WM_CONTEXTMENU, 0, 0);
	return FALSE;
}

//@Solomon100225:솔로몬에서는 사용안함.
// LRESULT CStdDialog::OnAddPopupMenuItem(WPARAM wParam, LPARAM lParam)
// {
// 	if(!m_bChartPopupMenu)
// 		return 0L;
// 
// 	m_bChartPopupMenu = FALSE;
// 
// 	m_ChartMng.AddPopupMenuItem((CMenu*)wParam, (CPoint*)lParam);
// 	if(m_pExtInputBarDlg)
// 		m_pExtInputBarDlg->AddPopupMenuItem((CMenu*)wParam, (CPoint*)lParam);
// 
// 	return 0L;
// }
// 
// LRESULT CStdDialog::OnActionPopupMenuItem(WPARAM wParam, LPARAM lParam)
// {
// 	m_ChartMng.ActionPopupMenuItem((UINT)wParam);
// 	if(m_pExtInputBarDlg)
// 		m_pExtInputBarDlg->ActionPopupMenuItem((UINT)wParam);
// 
// 	return 0L;
// }

BOOL CStdDialog::IsSystemTradingUser()
{
	AfxMessageBox("[IsSystemTradingUser]Solomon수정필요"); //@Solomon-091015

//	return TRUE;
	CString strSystemTradingUser("N");
	::SendMessage(m_hViewInMain, RMSG_GET_TEMPDATA, (WPARAM)&strSystemTradingUser, 0);

	return (strSystemTradingUser == "Y");
}

void CStdDialog::OpenScreen(LPSTR lpszScreenNumber)
{
	AfxMessageBox("[OpenScreen]Solomon수정필요"); //@Solomon-091015

	::SendMessage(m_hViewInMain, RMSG_OPENSCREEN, (WPARAM)lpszScreenNumber, 0);
}

CString CStdDialog::GetItemExpCode(LPCTSTR lpszCode)
{
	CString strExpCode(_T(""));

	if(m_pInputBarDlg && m_pInputBarDlg->m_pDataManager)
		strExpCode = m_pInputBarDlg->m_pDataManager->GetItemExpCode(lpszCode);

	return strExpCode;
}

CString CStdDialog::GetScreenNo()
{
	CString strScreenNo;
	strScreenNo.Format("%04d", m_nScreenID);
	return strScreenNo;
}

CString CStdDialog::GetBizDate()
{
	return DoGetWorkDate(0);	//영업일리턴.
// 	//@Solomon-091015
// 		return "";
// 		CString strDate;
// 		::SendMessage(m_hViewInMain, RMSG_GET_HOSTDATE, (WPARAM)strDate.GetBuffer(10), 0);
// 		strDate.ReleaseBuffer();
// 
// 		return strDate;
}

// LRESULT CStdDialog::OnChangeAccount(WPARAM wParam, LPARAM lParam)
// {
// 	if(m_pExtInputBarDlg)
// 		m_pExtInputBarDlg->SendMessage(UMSG_END_REQUEST_CHARTDATA, 0, 0);
// 
// 	return 0L;
// }
// 
// 
// LRESULT CStdDialog::OnVerifyPassword(WPARAM wParam, LPARAM lParam)
// {
// 	int nID = UUID_IAUDataMonitorManager; 
// 	IAUDataMonitorManager* pIAUDataMonitorManager = (IAUDataMonitorManager*)AfxGetPctrInterface(nID);
// 	pIAUDataMonitorManager->NotifyData("CHECK_VALIDPWD", wParam, 0);
// 
// 	return 0L;
// }

// 	LRESULT CStdDialog::OnFormLoadSave(WPARAM wParam, LPARAM lParam)
// 	{
// 		CString strFile = (LPCTSTR)lParam;
// 		if(wParam == 1)
// 		{
// 			if(!strFile.IsEmpty())
// 			{
// 				int nIndex = strFile.ReverseFind('\\');
// 				strFile.Delete(0, nIndex + 1);
// 				
// 				m_strConfig = m_strUserDir + "\\" + strFile + "_" + ENVINFOFILE;
// 			}
// 			else
// 			{
// 				m_strConfig = m_strUserDir + "\\" + GetScreenNo() + "_" + ENVINFOFILE;
// 			}
// 
// 			CRect rcDialog;
// 			GetClientRect(rcDialog);
// 
// 			if(rcDialog.IsRectEmpty())
// 			{
// 				m_bMinimizedStart = TRUE;
// 				return 0L;
// 			}
// 
// 			FormLoad();
// 			if( !m_bInitForm )	return FALSE;
// 
// 			if(!strFile.IsEmpty())
// 				m_ChartMng.LoadChart(m_strConfig);
// 
// 			m_strConfig = m_strUserDir + "\\" + GetScreenNo() + "_" + ENVINFOFILE;
// 		}
// 		else if(wParam == 2)
// 		{
// 			if(!strFile.IsEmpty())
// 			{
// 				int nIndex = strFile.ReverseFind('\\');
// 				strFile.Delete(0, nIndex + 1);
// 				
// 				CString strConfig = m_strConfig;
// 
// 				m_strConfig = m_strUserDir + "\\" + strFile + "_" + ENVINFOFILE;
// 
// 				SaveShowInfo();
// 				m_ChartMng.SaveChart(m_strConfig);
// 
// 				if(m_pInputBarDlg)
// 					m_pInputBarDlg->SaveValue();
// 
// 				if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
// 				{
// 					m_pExtInputBarDlg->m_strCfgFile = m_strConfig;
// 					m_pExtInputBarDlg->SaveExtInputBar();
// 					m_pExtInputBarDlg->m_strCfgFile = strConfig;
// 				}
// 
// 				m_strConfig = strConfig;
// 			}
// 		}
// 		return 0L;
// 	}

void CStdDialog::SaveShowInfo()
{
	CString strTemp;
	if(!m_bSmallScreenMode)
	{
		CRect rect;
		if(m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd())
		{
			if(m_nWidthOfLeftBar>0)
			{
				strTemp.Format("%d",m_nWidthOfLeftBar);
				::WritePrivateProfileString("Frame","WidthOfLeftBar",strTemp,m_strConfig);
			}
			strTemp.Format("%d",GetLeftBarType());
			::WritePrivateProfileString("Frame","ShowLeftBar",strTemp,m_strConfig);
			strTemp.Format("%d",m_bShowExtInputBarDlg);
			::WritePrivateProfileString("Frame","ShowExtInputBarDlg",strTemp,m_strConfig);
			strTemp.Format("%d",m_bShowToolBar);
			::WritePrivateProfileString("Frame","ShowToolBar",strTemp,m_strConfig);
		}
	}
	else
	{
		strTemp.Format("%d",m_bShowToolBar);
		::WritePrivateProfileString("Frame","ShowToolBar",strTemp,m_strConfig);
	}

// vntsorl:[0001250] [5001] 종합차트 - 일주월분초틱 주기별로 지표 설정 및 설정값을 다르게 할 수 있도록 개선요청
	// ADD PSH - 20100730 강부수 주기별 저장 여부 추가 >>
	strTemp.Format("%d", m_bUseCycleSave);
	::WritePrivateProfileString("Frame","UseCycleSave", strTemp, m_strConfig);
	// END ADD
}

void CStdDialog::ShareInitCode()
{
//@Solomon-091015	if(m_hViewInMain)
//@Solomon-091015	::PostMessage(m_hViewInMain, RMSG_FORMLOADSAVE, 0, 0);
}

void CStdDialog::SendNextData()
{
	if(m_lstSendData.IsEmpty() || m_bIsWaitRequest)
		return;

	STSENDDATAQUE* pSendData = m_lstSendData.GetHead();

	m_TRComm->SetTR(pSendData->strTRCode);
	m_TRComm->SetBaseData("PtManager", m_szPTManager);
	m_TRComm->SetBaseData("SCREENCODE", GetScreenNo());

	m_lstSendData.RemoveHead();

	m_bIsWaitRequest = TRUE;
	m_TRComm->Send2Server(pSendData->lpData, pSendData->nSize, TRKEY_FID_END);	

	::free(pSendData->lpData);
	delete pSendData;

}

void CStdDialog::SendLastData()
{
	if(m_lstSendData.IsEmpty() || m_bIsWaitRequest)
		return;

	STSENDDATAQUE* pSendData = m_lstSendData.GetHead();
	if(pSendData->strTRCode == QUERY_STRMARKET)
	{
		SendNextData();
		return;
	}

	pSendData = m_lstSendData.GetTail();

	m_TRComm->SetTR(pSendData->strTRCode);
	m_TRComm->SetBaseData("PtManager", m_szPTManager);
	m_TRComm->SetBaseData("SCREENCODE", GetScreenNo());

	LPSTR lpszData = ::strdup(pSendData->lpData);
	int nSize = pSendData->nSize;
	BOOL bWait = pSendData->bWait;

	POSITION pos = m_lstSendData.GetHeadPosition();
	while(pos)
	{
		pSendData = m_lstSendData.GetNext(pos);
		::free(pSendData->lpData);
		delete pSendData;
	}

	m_lstSendData.RemoveAll();

	::memset(&m_stTrInfo2, 0x00, sizeof(m_stTrInfo2));

	m_bIsWaitRequest = TRUE;
	m_TRComm->Send2Server(lpszData, nSize, TRKEY_FID_END);
	::free(lpszData);
} 

BOOL CStdDialog::IsInvestMarket(CString& strMarketName)
{
	if(strMarketName.Find("거래소순매수") >= 0)
		return TRUE;
	if(strMarketName.Find("코스닥순매수") >= 0)
		return TRUE;
	if(strMarketName.Find("K200순매수") >= 0)
		return TRUE;
	if(strMarketName.Find("선물순매수") >= 0)
		return TRUE;
	if(strMarketName.Find("콜옵션순매수") >= 0)
		return TRUE;
	if(strMarketName.Find("풋옵션순매수") >= 0)
		return TRUE;

	return FALSE;
}

// 멀티차트의 십자선 동기화 - ojtaso (20090609)
bool CStdDialog::GetCrossLine()
{
	if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
		return m_pExtInputBarDlg->SendMessage(RMSG_CHANGE_CROSSLINE) == 1L ? true : false;
	
	return false;
}

// 멀티차트의 십자선 동기화 - ojtaso (20090609)
void CStdDialog::SetCrossLine(bool bUseCrossLine)
{
	if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
		m_pExtInputBarDlg->SendMessage(RMSG_CHANGE_CROSSLINE, 1, (bUseCrossLine == true ? TRUE : FALSE));
}

//>> [QC No]090811HMC104
BOOL CStdDialog::IsContinueFutCode(CString& strCode)
{
	CStringArray arCode, arName;

	//>>[A00000546] 연결선물일 때 종목코드 가져오기.
// 	if(strCode == DEF_CONTINUE_FUT_CODE)		// 연결선물
// 	{
// 		IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
// 		
// 		if(m_strLastFutureCode.IsEmpty())
// 		{
// 			CString strFOCode;
// 			pMasterDataManager->GetRecentCode(strCode, strFOCode, 10);
// 			strCode = strFOCode;
// 			
// 			m_strLastFutureCode = strCode;
// 		}
// 		else
// 		{
// 			strCode = m_strLastFutureCode;
// 		}
// 		return TRUE;
// 	}

	if(strCode.GetLength()<8) return FALSE;

	CString szTmpCode = strCode.Mid(3);
	if(szTmpCode.Compare("99999") != 0) return FALSE;

	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);		
	if(pMasterDataManager)
	{
		CString strFOCode;
		BOOL bRet = pMasterDataManager->GetRecentCode(strCode, strFOCode, 10);
		strCode = strFOCode;
		return bRet;
	}
	//<<[A00000546]

	return FALSE;
}
//<< [QC No]090811HMC104

BOOL CStdDialog::HasKey(CString strCode)
{
	return m_RealTrCounter.HasKey(strCode);
}

LRESULT CStdDialog::OnGetTRComm(WPARAM wp, LPARAM lp)
{
	ITrComm2 **pTrComm = NULL;
	pTrComm = (ITrComm2 **)wp;

	*pTrComm = (ITrComm2*)m_TRComm;

	CString* pStrCode, sCode;
	pStrCode = (CString*)lp;
	*pStrCode = m_szPTManager;

	return (LRESULT)(LPTSTR)(LPCTSTR)m_szPTManager;
}

LRESULT CStdDialog::OnUserInfo(WPARAM wp, LPARAM lp)
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
					SetTimer(3, 1, NULL);

/*
					RequestWorkDate();
					if (m_nChartType == MULTI_CHART)
						RequestDollarData();
*/
				}
			}
			break;
	}

	return 0L;
}


void CStdDialog::DoShareCode(LPCSTR _szCode)
{
	//@100304 공유(도미노)를 받지는 않지만 보내기는 한다.
	//	if(m_nMatrix>=1) return;
	if(!m_bNotifyChangeCode) return;
	if(g_pPtUtilMng)
	{
		//@Solomon	과거종목일 경우 종목 공유 없음.	-->
		if (m_pInputBarDlg && m_pInputBarDlg->m_pFOHisBtn)
		{
			CString strCode;
			strCode.Format(_T("%s"), _szCode);
			int nCodeType = m_pInputBarDlg->m_pFOHisBtn->GetCodeType(strCode);
			switch (nCodeType)
			{
			case futureType:	// 선물
//KHD			case spreadType:	// 스프레드
			case callType:		// 콜옵션
			case putType:		// 풋옵션
			case sfutureType:	// 주식선물
			case koptionType:	// 주식옵션
				return;
			}
		}
		//@Solomon	과거종목일 경우 종목 공유 없음.	<--
		g_pPtUtilMng->ShareCode(m_hPlatform, _szCode);
	}
}

//---------------------------------------------------------------------------
// 첫 조회에 대한 룰
// step-1. 자동조회하도록 설정된 종목이 있는지 체크.
// step-2. 1번에 해당하는 종목이 없으면 최근 조회종목으로 조회.
// step-3. 2번까지 조회가 안될경우 -> 디폴트 코드로 조회.
//			주식:솔로몬증권으로 조회.
//			업종:001 코스피종합으로 조회.
//			선옵:최근 선물코드로 조회.
//			해외:
BOOL CStdDialog::GetFirstCode(CString &rString, int nMarketType, int &nSetp)
{
	BOOL bRet = TRUE;
	if (IsChartScreen("OldFuture") == TRUE)
	{
		//ClearSaveKey();	//100530:동일화면 다르게 저장/읽기
		return FALSE;
	}

	rString.Empty();
	nSetp=0;
	
	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);

//@Solomon	DominoData 적용. -->
	BOOL bDomino = FALSE;
	int nCount = m_mapDominoCode.GetCount();
	if ((m_nMatrix == 0) && nCount > 0)
		bDomino = TRUE;

	char szTemp[32];
	memset(szTemp, 0x00, sizeof(szTemp));
	::GetPrivateProfileString("0000", "Code00", "", szTemp, sizeof(szTemp), m_strConfig);

	rString.Format(_T("%s"), szTemp);
	// FO지수차트에서의 히스토리는 별도관리.
	if(GetScreenNo()=="0515")
	{
		//ClearSaveKey();	//100530:동일화면 다르게 저장/읽기
		return TRUE;
	}

// 	else
// 	{
// 		m_pInputBarDlg->m_bChkLoadSaveCode = FALSE;
// 		rString = m_mapDominoCode.;
// 		m_strDominoCode = _T("");
// 		bDomino = TRUE;
// 	}
//@Solomon	DominoData 적용. <--

	if (rString.GetLength() > 0)
	{
		if (nMarketType < 0)
		{
			nMarketType = pMasterDataManager->GetMarketTypeFromCode(rString);
			if (nMarketType == CMasterTypeInfo::NONE_CHART)
			{
				nMarketType = m_nMarketType;
				rString = "";
			}
			else
				m_nMarketType = nMarketType;
		}
	}

	if (nMarketType < 0)
		nMarketType = m_nMarketType;
	

	//100530:동일화면 다르게 저장/읽기 -->
	//ClearSaveKey();
	if(m_szOpenInfo.GetLength()>0 && rString.GetLength()>0)
	{
		m_szOpenInfo.Empty();	//[A00000356]alzioyes_20110330 선택한 시장과 다르게 종목창 열리는 현상수정.
		return TRUE;
	}
	//100530:동일화면 다르게 저장/읽기 <--

	CStringArray arrCode, arrName;
	CString rCode;
	CString strStandardCode;

	switch (nMarketType)
	{
	case CMasterTypeInfo::STOCK_CHART:
		{
			//if(m_nMatrix>=1)
			{
				CString szInitKey;
				szInitKey.Format("OnLoadHistory_Init_%d", m_nMatrix);
				if(m_mapDominoCode.Lookup(szInitKey, rCode)==TRUE)
				{
					m_mapDominoCode.RemoveKey(szInitKey);
					rString = rCode;
				}
				if(rCode.GetLength()>0)
					break;
			}
			if(m_nMatrix>=1)
			{
				rString.Empty();
				break;
			}

			//Domino가 있으면 먼저.
			if (bDomino && m_mapDominoCode.Lookup(_T("1301"), rCode))
			{
				nSetp = 0;
				rString = rCode;
				m_mapDominoCode.RemoveKey(_T("1301"));
				break;
			}

			//Step 1.
			IMainExInfoManagerLast* pMainExMng = (IMainExInfoManagerLast*)AfxGetPctrInterface(UUID_IMainExInfoManager);
			if(pMainExMng)
			{
				BOOL bSaved = (BOOL)pMainExMng->GetUserStringData("SET_STOCKCODE", rCode);
				if(rCode.GetLength()>0)
				{
					nSetp = 1;
					rString = rCode;
					break;
				}
			}

			//Step 2.
			//GetRecentFutureCode()
			pMasterDataManager->GetRecentCodeNameArray(nMarketType, arrCode, arrName);
			if(arrCode.GetSize()>0)
			{
				rCode = arrCode.GetAt(0);
				nSetp = 2;
				rString = rCode;
				break;
			}

			//Step 3.
			rCode = _T("024110");
			nSetp = 3;
			rString = rCode;
			break;
		}
		break;

	case CMasterTypeInfo::UPJONG_CHART:
		{
			CString szInitKey;
			szInitKey.Format("OnLoadHistory_Init_%d", m_nMatrix);
			if(m_mapDominoCode.Lookup(szInitKey, rCode)==TRUE)
			{
				rString = rCode;
				m_mapDominoCode.RemoveKey(szInitKey);
			}
			if(rCode.GetLength()>0)
				break;
		}
		if(m_nMatrix>=1)
		{
			rString.Empty();
			break;
		}

		//Domino가 있으면 먼저.
		if (bDomino && m_mapDominoCode.Lookup(_T("20301"), rCode))
		{
			m_mapDominoCode.RemoveKey(_T("20301"));
			nSetp = 0;
			rString = rCode;
			break;
		}

		//Step 1. 조회하도록 셋팅된 종목찾기
		
		//Step 2. 최근조회종목.
		pMasterDataManager->GetRecentCodeNameArray(nMarketType, arrCode, arrName);
		if(arrCode.GetSize()>0)
		{
			rCode = arrCode.GetAt(0);
			nSetp = 2;
			rString = rCode;
			break;
		}
		
		//Step 3. 디폴트 코드로 조회.
		rCode = _T("001");	//거래소 종합
		nSetp = 3;
		rString = rCode;
		break;

	case CMasterTypeInfo::FUTOPT_CHART:
		{
			CString szInitKey;
			szInitKey.Format("OnLoadHistory_Init_%d", m_nMatrix);
			if(m_mapDominoCode.Lookup(szInitKey, rCode)==TRUE)
			{
				m_mapDominoCode.RemoveKey(szInitKey);
				rString = rCode;
			}
			if(rCode.GetLength()>0)
				break;
		}
		if(m_nMatrix>=1)
		{
			rString.Empty();
			break;
		}

		//Domino가 있으면 먼저.
		if (bDomino && m_mapDominoCode.Lookup(_T("30301"), rCode))
		{
			m_mapDominoCode.RemoveKey(_T("30301"));
			nSetp = 0;
			rString = rCode;
			break;
		}

		//Step 1. 조회하도록 셋팅된 종목찾기
		
		//Step 2. 최근조회종목.
		pMasterDataManager->GetRecentCodeNameArray(nMarketType, arrCode, arrName);
		if(arrCode.GetSize()>0)
		{
			rCode = arrCode.GetAt(0);
			nSetp = 2;
			rString = rCode;
			break;
		}
		
		//Step 3. 디폴트 코드로 조회.
		{
			CStringArray arCode, arName, arEngName;
			CStringArray *parCode = &arCode, *parName = &arName, *parEngName = &arEngName;
			pMasterDataManager->GetFutureJongMst(parCode, parName, parEngName);
			if(arCode.GetSize()>0)
			{
				rCode = arCode.GetAt(0);
				nSetp = 3;
				rString = rCode;
				break;
			}
		}

		rString = "";
		bRet = FALSE;
		break;
		
	case CMasterTypeInfo::STOCKFUTOPT_CHART:
		{
		//Step 1. 조회하도록 셋팅된 종목찾기		
		//Step 2. 최근조회종목.
		//메인에서 제공이 안된다고해서 파일에서 직접 읽어서 처리
		CString strRString;
		CString strHistoryFile;
		CString strFirstCode;
		IMainInfoManager* pManager = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
		if(pManager)
		{
			CString strUserDir = pManager->GetEncUserDir();
			strHistoryFile.Format("%s\\sfcode_history.dat", strUserDir);
			
			CStdioFile file;
			if(file.Open(strHistoryFile, CFile::modeRead|CFile::typeText))
			{			
				file.ReadString(strRString);
				file.Close();

				int nFind = strRString.Find(' ');
				if(nFind > -1 && nFind == FUTOPT_LENGTH)
				{			
					strFirstCode = strRString.Left(nFind);
					if(!strFirstCode.IsEmpty())
					{
						rString = strFirstCode;
						nSetp = 2;
						break;
					}	
				}
			}
		}	
		
		//Step 3. 디폴트 코드로 조회.		
		pMasterDataManager->GetJFMaster(arrCode, arrName);
		if(arrCode.GetSize() <= 0)
		{
			bRet = FALSE;
			break;
		}
		else 
			strStandardCode = arrCode.GetAt(0);

		arrCode.RemoveAll();
		arrName.RemoveAll();
		pMasterDataManager->GetMasterDataInUpDetails("JFMASTER", arrCode, arrName, strStandardCode);
		if (arrCode.GetSize() > 0)
		{
			rCode = arrCode.GetAt(0);
			nSetp = 3;
			rString = rCode;
			break;
		}

		rString = "";
		bRet = FALSE;
		}
		break;
		
	case CMasterTypeInfo::COMMODITY_CHART:
		//100208-alziyes. 상품선물 domino확인할 것.
		if (bDomino && m_mapDominoCode.Lookup(_T("cfcod"), rCode))
		{
			m_mapDominoCode.RemoveKey(_T("cfcod"));
			nSetp = 0;
			rString = rCode;
			break;
		}
		//Step 1. 조회하도록 셋팅된 종목찾기
		
		//Step 2. 최근조회종목.
		pMasterDataManager->GetRecentCodeNameArray(nMarketType, arrCode, arrName);
		if(arrCode.GetSize()>0)
		{
			rCode = arrCode.GetAt(0);
			nSetp = 2;
			rString = rCode;
			break;
		}

		//Step 3. 디폴트 코드로 조회.
		pMasterDataManager->GetMasterDataInUpDetails("COMMODITYMASTER", arrCode, arrName, "6665");
		if (arrCode.GetSize() > 0)
		{
			rCode = arrCode.GetAt(0);
			nSetp = 3;
			rString = rCode;
			break;
		}

		rString = "";
		bRet = FALSE;
		break;
		
	case CMasterTypeInfo::FOREIGN_CHART:
		if (bDomino && m_mapDominoCode.Lookup(_T("50301"), rCode))
		{
			m_mapDominoCode.RemoveKey(_T("50301"));
			nSetp = 0;
			rString = rCode;
			break;
		}
		//Step 2. 최근조회종목.
		pMasterDataManager->GetRecentCodeNameArray(nMarketType, arrCode, arrName);
		if(arrCode.GetSize()>0)
		{
			rCode = arrCode.GetAt(0);
			nSetp = 2;
			rString = rCode;
			break;
		}
		
		//Step 3. 디폴트 코드로 조회.
		{
			rString = "DJI@DJC";
			nSetp = 3;
			break;
		}
		break;
	case CMasterTypeInfo::FUOPT_JIPYO_CHART:
		if (bDomino && m_mapDominoCode.Lookup(_T("109301"), rCode))
		{
			m_mapDominoCode.RemoveKey(_T("109301"));
			nSetp = 0;
			rString = rCode;
			break;
		}
		//Step 2. 최근조회종목.
		pMasterDataManager->GetRecentCodeNameArray(nMarketType, arrCode, arrName);
		if(arrCode.GetSize()>0)
		{
			rCode = arrCode.GetAt(0);
			nSetp = 2;
			rString = rCode;
			break;
		}

		//Step 3. 디폴트 코드로 조회.
		rCode = _T("PGKOSPIXPT");	
		nSetp = 3;
		rString = rCode;
		break;
	default:	bRet = FALSE;	break;
	}

	m_mapDominoCode.RemoveAll();

	return bRet;
}

void CStdDialog::_DoFirstRequest()
{
	int nStep;
	CString rCode;
//	GetFirstCode(rCode, m_nMarketType, nStep);

// 	//@solomon	미니멀티차트인경우 빈페이지로 띄움.
// 	if (m_nChartType == MINI_SINGLE2_CHART)
// 		return ;

//@100303 매트릭스 화면에서도 처리.
//	if (m_nMatrix != 0)		return;
	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	int nDomenoMarket = pMasterDataManager->GetMarketTypeFromCode(m_szOpenInfo);
	if(nDomenoMarket != CMasterTypeInfo::NONE_CHART)
	{
		rCode = m_szOpenInfo;
	}
	else
		GetFirstCode(rCode, -1, nStep);

	WPARAM wp = (WPARAM)(LPCSTR)rCode;
	LPARAM lp = m_nMarketType;
	SendMessage(UMSG_SENDSHAREDCODE, wp, lp);

	RequestDollarData();

//	if(GetFirstCode(rCode, nStep))
//		m_pInputBarDlg->m_strCodeFromOutside  = rCode;
//	m_pInputBarDlg->SetCodeInit();
	return;


// 	switch(m_nMarketType)
// 	{
// 		case STOCK_CHART:
// 			//if(nStep==1)	pMainExMng->SetUserStringData("SET_STOCKCODE", "");
// 			
// 			m_pInputBarDlg->m_pCodeInput->SetCode(rCode);
// 			m_pInputBarDlg->DoSelchangeCodeInput();
// 			return;
// 
// 		case UPJONG_CHART:
// 			m_pInputBarDlg->m_pCodeInput->SetCode(rCode);
// 			m_pInputBarDlg->DoSelchangeCodeInput();
// 			return;
// 
// 		case FUTOPT_CHART:
// 		case STOCKFUTOPT_CHART:
// 			m_pInputBarDlg->m_pCodeInput->SetCode(rCode);
// 			m_pInputBarDlg->DoSelchangeCodeInput();
// 			return;
// 
// 		case FOREIGN_CHART:
// 			break;
// 		default: break;
// 	}
}

BOOL CStdDialog::IsChartScreen(LPCSTR szKey)
{
	//m_nMarketType = CMasterTypeInfo::FUOPT_JIPYO_CHART;
	if(strcmp(szKey, "FUTOPT_CHART")==0)
	{
		if (GetScreenNo().Compare("0515")==0) //|| GetScreenNo().Compare("0516")==0)
			return TRUE;
	}

	if(strcmp(szKey, "OldFuture")==0)
	{
		if(GetScreenNo().Compare("0516")==0)
			return TRUE;
	}
// 		else if(strcmp(szKey, "FOChart")==0)
// 		{
// 			if(m_nMarketType == CMasterTypeInfo::FUOPT_JIPYO_CHART)
// 				return TRUE;
// 	//		if(GetScreenNo().Compare("0515")==0)
// 	//			return TRUE;
// 		}
	return FALSE;
}

#include "../../inc/AZUtilFileMng.h"
void CStdDialog::_DoVersionCheck_ChartDef()
{
// 	{
// 		CString szDelFile;
// 		szDelFile.Format("%s\\0508_ChartInfo.dat", m_strUserDir);
// 		::DeleteFile(szDelFile);
// 	}

	//100204-alzioyes. 설정값을 전체적으로 적용하기 위해 사용함.
	//return;// 이 기능이 필요한 시점에만 사용한다.

	CString szChartCfg, szUserCfg;
	szChartCfg.Format("%s\\Icss\\ChartDef.cfg", m_strRootPath);
	szUserCfg.Format("%s\\ChartDef.cfg", m_strUserDir);

	char aDataVer[20]={0,}, aUserVer[20]={0,};
	::GetPrivateProfileString("Ver", "Current", "000000", aDataVer, 20, szChartCfg);
	::GetPrivateProfileString("Ver", "Current", "000000", aUserVer, 20, szUserCfg);

	if(strcmp(aDataVer, aUserVer)==0) return;

	DoClearUserChartData();
	::WritePrivateProfileString("Ver", "Current", aDataVer, szUserCfg);
}

//alzioyes 사용자의 차트정보 모두지우기
void CStdDialog::DoClearUserChartData()
{
//--> Method 1.
	CString strUserPath(m_strUserDir);
	CString strExt;

	LPTSTR arExt[] = {	"*.mct", "*.cht", "*IndiCond*.*", "*ChartInfo*.dat", "*AnalTool*.*", 
		"*JipyoSync*.dat"};
	
	int nCount = sizeof(arExt)/sizeof(LPTSTR);
	for(int i=0; i<nCount; i++)
	{
		strExt = arExt[i];
		CAZUtil_FileManager::DoDirFileRemove(strUserPath, strExt, FALSE);
	}
//<--

//--> Method 2.
// 	CString szChartCfg;
// 	szChartCfg.Format("%s\\Hanaro\\ChartDef.cfg", m_strRootPath);
// 
// 	int nCount = ::GetPrivateProfileInt("ClearListA", "Count", 0, szChartCfg);
// 
// 	CString strUserPath(m_strUserDir);
// 	CString strExt;
// 	char aExt[100+1], aKey[4];
// 	for(int i=0; i<nCount; i++)
// 	{
// 		sprintf(aKey, "%d", i);
// 		if(::GetPrivateProfileString("ClearListA", aKey, "", aExt, 100, szChartCfg)>0)
// 		{
// 			strExt = aExt;
// 			CAZUtil_FileManager::DoDirFileRemove(strUserPath, strExt, FALSE);
// 		}
// 	}
//<--
	
//--> Method 3.
// 	CString strUserPath(m_strUserDir);
// 	CAZUtil_FileManager::DoDirFileRemove(strUserPath, "*.*", FALSE);
// 	
// 	CString szChartCfg, szUserCfg;
// 	szChartCfg.Format("%s\\Hanaro\\ChartDef.cfg", m_strRootPath);
// 	szUserCfg.Format("%s\\ChartDef.cfg", m_strUserDir);
// 	::CopyFile(szChartCfg, szUserCfg, FALSE);
//<--
}

void CStdDialog::_DoCheckDaultDataFile()
{
	//100530:동일화면 다르게 저장/읽기
	CString szSaveKey = GetSaveKey();
	if(szSaveKey.GetLength()>0) return;

	//if(m_nScreenID==nScreenID_6301)
	{
		CString szScreenNo = GetScreenNo();

		CString szUserFile;
		szUserFile.Format("%s\\%s%s", m_strUserDir, szScreenNo, "_ChartInfo.dat");

//	임시루틴 삭제. 
// 		if(szScreenNo.CompareNoCase("0515")==0)
//		{
// 			CString szChartCfg;
// 			szChartCfg.Format("%s\\Hanaro\\ChartDef.cfg", m_strRootPath);
// 	
// 			int nVer = GetPrivateProfileInt(szScreenNo, "SvrVer", 0, szChartCfg);
// 			if(nVer==0)
// 			{
// 				::DeleteFile(szUserFile);
// 			}
// 			::WritePrivateProfileString(szScreenNo, "svrVer", "1", szChartCfg);
// 		}
 		CFileFind file;
 		if(file.FindFile(szUserFile)==FALSE)
 		{
 			_DoCopyDefaultToUser(GetScreenNo());
  		}
	}
}

void CStdDialog::_DoCopyDefaultToUser(LPCSTR szSect)
{
	CString szChartCfg;
	szChartCfg.Format("%s\\Icss\\ChartDef.cfg", m_strRootPath);

	int nCnt = ::GetPrivateProfileInt(szSect, "Count", 0, szChartCfg);
	
	char aKey[4], aBuff[MAX_PATH];
	CString szFullName, szFrom, szTo, szDefFile, szUserFile;
	for(int i=0; i<nCnt; i++)
	{
		sprintf(aKey, "%d", i);
		if(::GetPrivateProfileString(szSect, aKey, "", aBuff, MAX_PATH, szChartCfg)>0)
		{			
			szFullName = aBuff;
			//파일명이 32바이트를 넘어가는 경우가 있어서 수정함.
			AfxExtractSubString(szFrom, szFullName, 0, ';');
			if(szFrom.GetLength() < szFullName.GetLength())
			{
				szTo = szFullName.Mid(szFrom.GetLength()+1);
			}
			else
			{
				szFrom = szFullName;
				szTo = szFrom;
			}
			//szDefFile.Format("%s\\Hanaro\\%s", m_strRootPath, aBuff);
			//szUserFile.Format("%s\\%s", m_strUserDir, aBuff);
			szDefFile.Format("%s\\Icss\\%s", m_strRootPath, szFrom);
			szUserFile.Format("%s\\%s", m_strUserDir, szTo);
			::CopyFile(szDefFile, szUserFile, FALSE);
		}
	}
}

LPCSTR CStdDialog::DoGetWorkDate(int nIndex)
{
	int nSize = m_saWorkDate.GetSize();
	if(nSize<=nIndex) return "";

	return m_saWorkDate.GetAt(nIndex);
}

void CStdDialog::_DoMakeUserExtraDir(LPCSTR szBaseDir)
{
	if (_access(szBaseDir, 0) == -1)
		::mkdir(szBaseDir);
	
	LPTSTR arExt[] = { "\\Guide", "\\Saved"};	//"\\AnalTool", 
	
	int nCount = sizeof(arExt)/sizeof(LPTSTR);
	CString szExtraDir;
	for(int i=0; i<nCount; i++)
	{
		szExtraDir.Format("%s%s", szBaseDir, arExt[i]);
		if (_access(szExtraDir, 0) == -1)
			::mkdir(szExtraDir);
	}
}

//100530:동일화면 다르게 저장/읽기-->
//  저장화면을 위한 키값셋팅.
//	m_strSaveKey값은 저장된 화면을 열때와 닫을 때만 셋팅된 후 clear된다.
//  Open이의 정보는 m_szOpenInfo를 참조한다.
void CStdDialog::GetFileSaveKey(CString &rString) 
{
//KHD : 화면이든 연결화면이든 디폴트 파일을 읽는다.( IBK 요청으로 분석툴 저장은 안하고, 지표로딩 때문에 수정함)
//	if(m_strSaveKey.GetLength()==0)
//		rString = GetScreenNo();
//	else
//		rString.Format("%s_%s", GetScreenNo(), m_strSaveKey);

	if( (m_strSaveKey.GetLength() > 0) && m_strSaveKey.Compare("LASTSTAT") > 0 )
		rString.Format("%s_%s", GetScreenNo(), m_strSaveKey);
	else
		rString = GetScreenNo();
}

void CStdDialog::SetSaveKey(LPCSTR szNewKey)
{
	m_strSaveKey=szNewKey;

	CString szSaveKey;
	GetFileSaveKey(szSaveKey);
	m_strConfig = m_strUserDir + "\\" + szSaveKey + "_" + ENVINFOFILE;

	{
		CString s;
		s.Format("\r\n[CStdDialog::SetSaveKey] m_strConfig=%s\r\n", m_strConfig);
		OutputDebugString(s);
	}
}

void CStdDialog::UpdateSaveKey(CString szOpenInfo)
{
	m_strSaveKey = szOpenInfo;
	//m_strSaveKey = "test";  //test.
}

void CStdDialog::ClearSaveKey()
{
	m_strSaveKey.Empty();

	CString szSaveKey;
	GetFileSaveKey(szSaveKey);
	m_strConfig = m_strUserDir + "\\" + szSaveKey + "_" + ENVINFOFILE;

	{
		CString s;
		s.Format("\r\n[CStdDialog::ClearSaveKey] m_strConfig=%s\r\n", m_strConfig);
		OutputDebugString(s);
	}
}
//100530:동일화면 다르게 저장/읽기 <--

//100611-alzioyes: 좌측탭이 자동으로 열리도록 처리.
//RMSG_TABCODESEARCH : TabCodeSearch.cpp에 동일선언.
const UINT RMSG_TABCODESEARCH	=	::RegisterWindowMessage(_T("RMSG_TABCODESEARCH"));
void CStdDialog::SetSelFolder(LPCSTR szFolder)
{
	if(m_hTabCodeSearch)
	{
		::SendMessage(m_hTabCodeSearch, RMSG_TABCODESEARCH, 10, (LPARAM)szFolder);
	}
}


BOOL CStdDialog::IsAvailableBlockCount(BOOL bItemCode, int& nUserBlockCount)
{
	if(m_ChartMng.GetSafeHwnd() == NULL || m_pInputBarDlg == NULL)
		return FALSE;

	if(bItemCode)
	{
		if(m_pInputBarDlg->GetCurrentMode() != NEW_ITEM)
			return TRUE;
	}

	CChartItem* pItem = m_ChartMng.GetpChartItemSelected();
	if(!pItem)
		return FALSE;

	nUserBlockCount = pItem->GetNUserBlockAddCount();
	short nBlockCount = pItem->GetRowBlockCount();

	return nUserBlockCount > nBlockCount;
}

void CStdDialog::AddStandByTrCall( CString strCode )
{
	CString strWatingCode = "";
	if( m_saStandByTr.GetSize() > 0 )
	{
		strWatingCode = m_saStandByTr.GetAt( 0 );
		m_saStandByTr.RemoveAll();
	}
	
	if( strCode.GetLength() > 0 && strCode.CompareNoCase( strWatingCode ) == 0 )
		return;
	
	m_saStandByTr.Add( strCode );
}

void CStdDialog::CallStandByTr()
{
	if( m_saStandByTr.GetSize() == 0 )
		return;

	CString strCode = m_saStandByTr.GetAt( 0 );
	m_saStandByTr.RemoveAll();

	STINPUTCODEDATA stInputCodeData;
	stInputCodeData.m_strCode = strCode;
	stInputCodeData.m_nDefault = MODE_DEFAULT;
	stInputCodeData.m_nMode = CHANGE_ITEM;
	if(stInputCodeData.m_strCode.GetLength()>0)
		m_ChartMng.SendMessage(UMSG_INPUTCODETOCHARTMNG, MODE_DEFAULT, (LPARAM)&stInputCodeData);
}
