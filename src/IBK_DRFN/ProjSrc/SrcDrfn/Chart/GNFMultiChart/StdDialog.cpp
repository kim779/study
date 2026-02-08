/****************************************************************************/
/* Project   : Woori Futures Fx Margin System                               */
/* File Name : StdDialog.cpp                                                */
/* Function  : 멀티 차트 화면 다이얼로그                                    */
/* Comment   : DRFN Chart의 소스를 받아 사용자 인터페이스를 수정            */
/* Copyright : 2008 DRFN                                                    */
/*             Modify Korea Sorimachi                                       */
/****************************************************************************/
#include "./define/ChartMsgDef.h"
#include "ChartItem.h"

#include "stdafx.h"
#include "MultiChart.h"
#include "IO.h"
#include "direct.h"
#include "StdDialog.h"
#include "MiniBarDlg.h"
#include "TabCodeSearch.h"
#include "ChartListDlg.h"
#include "FloatingHogaDlg.h"

#include "../COMMONTR/ChartRealConverter_GNF.h"
#include "DataWindowDlg.h"
#include "CodeDataFromInputBar.h"
#include "./define/ReserveShareName.h"
#include "./define/MessageDefine.h"
#include "./define/DragNDropDefine.h"
#include "./include/UChar.h"

#include "../chart_common/StatusMsgDef.h"
#include "../../inc/IMainInfoMng.h"
#include "../../inc/IAUPlatformDispMng.h"
#include "../../inc/IKSLibMng.h"
#include "../../inc/ExCommonHeader.h"
#include "../../inc/RealUpdate.h"
#include "../../inc/IAUDataMonitorMng.h"			// UUID_IAUDataMonitorManager : 콜백함수 받는데 사용
#include "DlgGlass.h"								// 돈보기 구현 - ojtaso (20080519
#include "../CommonTR/EU_p0602.h"	//주식
#include "../Include_Chart/Dll_Load/IMetaTable.h"	// (2008/9/13 - Seung-Won, Bae) for Multi-Language
#include "../../inc/RealUpdate.h"
#include "../CommonTR/SM_CheJanStruct.h"			// 체결 - 주문선에서 사용


//	추가... 솔라리스 환경에서 돌려서 보내야 한다. 고 병훈이가 써놓음
#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")		// htonl, ntohl


#define	WMU_GET_KEY_INFO_ARRAY		WM_USER + 2253	// 해당 AUP에서 입력되어 있는 Key값의 목록을 가져온다. LPARAM은 CStringArray *

#define	WIDTH_SIDEBAR		3

//<<< 20091215 by kmlee for 주문연동
const UINT RMSG_CHANGE_CHARTACCOUNT = ::RegisterWindowMessage(_T("RMSG_CHANGE_CHARTACCOUNT"));
const UINT RMSG_GET_CHARTACCOUNT = ::RegisterWindowMessage(_T("RMSG_GET_CHARTACCOUNT"));
const UINT RMSG_ACCOUNT_INIT = ::RegisterWindowMessage(_T("RMSG_ACCOUNT_INIT"));
//>>>


const UINT RMSG_GET_SCREENTYPE = ::RegisterWindowMessage(_T("RMSG_GET_SCREENTYPE"));
const UINT RMSG_GET_MARKETTYPE = ::RegisterWindowMessage(_T("RMSG_GET_MARKETTYPE"));
const UINT RMSG_GET_LINKINFODATA = ::RegisterWindowMessage(_T("RMSG_GET_LINKINFODATA"));
const UINT RMSG_GET_SCREENGB = ::RegisterWindowMessage(_T("RMSG_GET_SCREENGB"));

const UINT RMSG_USERINFO = ::RegisterWindowMessage(_T("RMSG_USERINFO"));

#define	USERINFO_GET	0x01
#define	USERINFO_SET	0x02

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CStdDialog dialog
#define ID_LEFTBAR			IDD_LEFTBAR
#define ID_INPUTBAR			IDD_INPUTBAR
#define ID_MINIBAR			IDD_MINIBAR
#define ID_EXTINPUTBAR		IDD_EXTINPUTBAR
#define ID_SPLITTER			7000
#define ID_CHART			8000
#define ID_TOOLBAR			IDD_DLG_TOOLBAR

// shared Info Name.. gm0604 2005.8.24
#define MULTICHARTOPEN		"MultiChartOpen"
#define MULTICHARTCLOSE		"MultiChartClose"
#define FROMSTCENTER		"FromSTCenter"

#define DEFAULT_CHARTDATA_COUNT_INT		400
#define DEFAULT_CHARTDATA_COUNT_CHAR	"400"

// --> [Edit]  강지원 2008/08/06		( 왼쪽 사이드바의 넓이를 줄인다. )
//#define WIDTH_SIDEBAR_EXTEND		170
#define WIDTH_SIDEBAR_EXTEND		165
// <-- [Edit]  강지원 2008/08/06
#define WIDTH_SIDEBAR_COLLAPSE		10


extern HINSTANCE g_hInstance;

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

// CMultiChartWnd
#define HEIGHTOFINPUTBAR	(24)
#define WIDTHOFSPLITTER		(3)
#define WIDTH_TOOLBAR		(32)

const UINT RMSG_GETMAINWND	= ::RegisterWindowMessage(_T("RMSG_GETMAINWND"));

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

int		   CStdDialog::m_static_nChartCnt4ImgList = 0;

CImageList CStdDialog::m_ImgList_W15H15; //폭 15에 높이 15인 버튼 이미지리스트
CImageList CStdDialog::m_ImgList_W20H20; //폭 20에 높이 19인 버튼 이미지리스트
CImageList CStdDialog::m_ImgList_W20H20_Tool; //폭 20에 높이 19인 버튼 이미지리스트
CImageList CStdDialog::m_ImgList_W46H20; //폭 46에 높이 19인 버튼 이미지리스트	
CImageList CStdDialog::m_ImgList_W50H20; //폭 50에 높이 19인 버튼 이미지리스트
CImageList CStdDialog::m_ImgList_W151H21; //폭 151에 높이 21인 버튼 이미지리스트
CImageList CStdDialog::m_ImgList_W28H22; //폭 28에 높이 22인 버튼 이미지리스트
CImageList CStdDialog::m_ImgList_W17H20; //폭 17에 높이 20인 버튼 이미지리스트
CImageList CStdDialog::m_ImgList_W43H20; //폭 43에 높이 20인 버튼 이미지리스트
CImageList CStdDialog::m_ImgList_W58H20; //폭 58에 높이 20인 버튼 이미지리스트
CImageList CStdDialog::m_ImgList_W72H20; //폭 72에 높이 20인 버튼 이미지리스트
CImageList CStdDialog::m_ImgList_W46H20_Gwan; //폭 46에 높이 20인 버튼 이미지리스트
CImageList CStdDialog::m_ImgList_W8H25;  //폭 8에 높이 25인 버튼 이미지리스트

CMapStringToString	CStdDialog::m_mapCodeType;

extern CMultiChartApp theApp;
/****************************************************************************/
/* Function : 생성자														*/
/* Paramter :																*/
/* Return	:																*/
/* Author	: By DRFN														*/
/* Comments	: FX Margin 하면서 소스 정리 (한국소리치 전용배)				*/
/****************************************************************************/
CStdDialog::CStdDialog(CWnd* pParent /*=NULL*/)
	: CExOCXDLL(CStdDialog::IDD, pParent),
	m_nDrdsInfoCnt(0)
{
	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	m_nRequestPairTickState = E_RPTS_STOP;
	m_bOrgAddMode = FALSE;

	EnableAutomation();
	//{{AFX_DATA_INIT(CStdDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pLeftBarDlg		= NULL;
	m_pExtInputBarDlg	= NULL;
	m_pInputBarDlg		= NULL;
	m_nLeftBarType		 = 0;
	m_pData				= NULL;
	m_bSmallScreenMode	= FALSE;
	m_nTempLeftBarType	= 0;
	m_pMsgBar			= NULL;
	m_bEnable			= TRUE;
	m_bFirstGShared		= TRUE;
	m_bShowExtInputBarDlg = FALSE;
	m_bShowInputBarDlg	= TRUE;
	m_strMapID			= "33100";
	m_nWidthOfLeftBar	= 0;
	m_nSpliteID			= 0;
	m_nTabID			= 0;
	m_nChartType		= SINGLE_CHART;
	m_nImportedMapID	= 0;
	m_nWidthOfDlg		= 0;
	m_nHeightOfDlg		= 0;
//	m_hViewInMain		= NULL;
	m_lWidthOriginal	= 0;
	m_lHeightOriginal	= 0;
	m_bHideChart		= FALSE;
	m_bInitResize		= FALSE;

	m_hSocket			= NULL;
	m_hMainFrame		= NULL;

	m_bShowDataListWnd	= FALSE;			// 데이터 윈도우를 보일지 여부
	m_hReplayChartLib	= NULL;				// 복기  Lib Handle

	m_nMarketType		= CMasterTypeInfo::STOCK_CHART;

	m_bActiveDataWindow = false;
	m_pDataWindow		= NULL;
	m_pRqWndFromTab		= NULL;

	m_pWndReplayLib		= NULL;				//복기 관련 Wnd Pointer


	memset(&m_ChartCfgInfo, 0x00, sizeof(CHART_CONFIG_INFO));

	// (2008/11/2 - Seung-Won, Bae) Set Default for Imaginary Candle and Gap Revision.
	m_ChartCfgInfo.bImaginaryCandle	= true;		// TRUE 면 허봉
	m_ChartCfgInfo.bUseGapRevision	= false;	// TRUE 면 갭보정
	
	m_bNeedRqstTRForReplay	= FALSE;
	m_bReplayMode			= FALSE;

	m_strConfig				= "";
	m_strRootPath			= "";

	m_TRComm				 = NULL;
	m_bSystemChart			= FALSE;
	m_bSimpleChart			= FALSE;
	m_bMultiItemChart		= FALSE;
	m_hCursor				= NULL;
	m_cUM					= 'E';
	m_strDollar				= "";
	m_pDlgToolbar			= NULL;
	m_bOneClickHide			= FALSE;
	m_bShowToolBar			= FALSE;
	m_nMarketTrCode			= atoi(QUERY_STRMARKET);

	m_nExtendState			= 0;		// 20061212 정유진 수정, grid column 확장 btn 추가
	m_cMUM					= 'P';
	m_bPreMarket			= FALSE;	// 장전 데이터 처리 by 20070226 by LYH
	m_cUT = '1';						// 틱데이터 없을 경우 클리어 문제 처리
	m_strRecvCode			= "";
	m_strSvcID				= "CP986100";// 2007.05.02 by LYH 서버 통계 위해 서비스 ID 추가

	m_pDrdsLib				= NULL;
	m_pDlgGlass				= NULL;		// 돋보기 구현 - ojtaso (20080519)

	g_pPtUtilMng = NULL;

	m_bMigyulcheDraw		= TRUE;

// --> [Edit]  강지원 2008/10/29
//@Sol100618	m_pCommMsg				= NULL;

	m_nCodeInput			= TRUE;
	m_strHwndKey			= "";
	m_strScreenKey			= "";
// <-- [Edit]  강지원 2008/10/29

	m_hWinixWnd				= NULL;
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
//	ON_MESSAGE( WM_GET_BROD, OnGetBroad )
//@Solomon_GNF	ON_MESSAGE( WM_GET_BROD, OnGetBroad )
	ON_REGISTERED_MESSAGE(RMS_ANALYSIS_FUNC		, OnByPass_ExportToolOption ) //2005. 07. 28 added by sy,nam	
	ON_REGISTERED_MESSAGE(RMSG_REPLAY_REAL		, OnReplayDrds )
	ON_REGISTERED_MESSAGE(RMSG_REPLAY_FAIL		, OnReplayFail )
	ON_REGISTERED_MESSAGE(RMSG_REPLAY_CHANGE_DATE	, OnReplayChangeDate)
	ON_REGISTERED_MESSAGE(RMSG_REPLAY_END			, OnReplayEnd)			//복기 정지됨.
	ON_REGISTERED_MESSAGE(RMSG_REPLAY_TOTAL_SEC		, OnReplaySetTotalSec)	//06.01.10
	ON_REGISTERED_MESSAGE(RMSG_REPLAY_INC_SEC		, OnReplayEachSec)		//06.01.10

	ON_REGISTERED_MESSAGE(RMSG_DRAGSTART, OnDragStart)			// Added to drag and drop operation
	ON_REGISTERED_MESSAGE(RMSG_DRAGEND, OnDragEnd)				// 오정택 Date : 2005/2/23

	ON_MESSAGE(UMSG_FORMINITAFTER_FROMFORM, OnFormInitAfter)
	ON_MESSAGE(UMSG_FORMGETPAGESAVE, OnFormGetPageSave)
	ON_MESSAGE(UMSG_FORMSETPAGESAVE, OnFormSetPageSave)
	ON_REGISTERED_MESSAGE(RMSG_CHANGE_SKINCOLOR, OnChangeSkinColor)
	ON_REGISTERED_MESSAGE(RMSG_GET_SKINCOLOR, OnGetSkinColor)
	ON_REGISTERED_MESSAGE(RMSG_USERINFO, OnUserInfo)
	ON_REGISTERED_MESSAGE(RMSG_ACCOUNT_INIT, OnAccountInit)
END_MESSAGE_MAP()

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
	/* solomon
	if(strSharedName=="시간-그리드")
	{
		m_ChartMng.SetTimeMark(lpGSharedData, TRUE);
		return;
	}
	*/
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

/****************************************************************************/
/* Function : 멀티 차트 화면 다이알로그 초기화								*/
/* Paramter :																*/
/* Return	:																*/
/* Author	: By DRFN														*/
/* Comments	: FX Margin 하면서 소스 정리 (한국소리치 전용배)				*/
/****************************************************************************/
BOOL CStdDialog::OnInitDialog() 
{
	CExOCXDLL::OnInitDialog();

	AfxEnableControlContainer();
	
	m_strHwndKey.Format("%d", (long)GetSafeHwnd());

	SetInitInfo();												// User ID 및 폴더경로 초기화

	InitImageList();											// 최우선으로 ImageList 로드

	SetBaseInterface();											// PCTR 의 기본 Interface 셋팅

	m_nChartType = MULTI_CHART;

	CString strScreenType, strMarketType;

	if (m_nScreenID == atoi("8041"))	//@Solomon	8041
	{
		m_nMarketType = CMasterTypeInfo::STOCK_CHART;
		m_bSmallScreenMode = FALSE;
		m_nChartType = MULTI_CHART;
		m_bMultiItemChart = TRUE;
	}
/*	else if (m_nScreenID == atoi(""))	//@Solomon	일주분틱 차트 요구시.
	{
		m_nMarketType = CMasterTypeInfo::STOCK_CHART;
		m_nChartType = SINGLE_CHART;
		m_bSmallScreenMode = FALSE;
		m_bMultiItemChart = TRUE;
	}
*/
#ifdef _WRITELOG
	CreateLogFile();											// Log - ojtaso (20080107)
#endif

#ifdef _WRITELOG_SISE
	CreateLogFile();											// Log - ojtaso (20080107)
#endif

	IPartnerSvrManager* pPtSvrMng = (IPartnerSvrManager*)AfxGetPctrInterface(UUID_IPartnerSvrManager);
	if (pPtSvrMng)
		g_pPtUtilMng = (IPartnerUtilManager*)pPtSvrMng->GetInerInterface(LUID_IPartnerUtilManager);

	// 차트 폼 로드
	FormLoad();

	if(m_nHeightOfDlg && m_nWidthOfDlg)							// ?
		SetWindowPos(&CWnd::wndTop, 0, 0, m_nWidthOfDlg, m_nHeightOfDlg, SWP_SHOWWINDOW);			

	GetOrderInfo();

	if (m_pDrdsLib == NULL)
	{
		//@SolomonGNF IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager, 3);
		IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager);
		if (pDrdsMng)
			m_pDrdsLib = pDrdsMng->AdviseDrdsLib();
	}

	if (m_pDrdsLib)
	{
//@SolomonGNF 		m_pDrdsLib->DrdsAdvise(0, "TIME", "", "01", m_hWnd);
	}

	SetTimer(TIMER_EVENTID_FORMLOAD, 1, NULL);

	return TRUE;
}

void CStdDialog::ChangeStockMode(int type)
{
	if(m_nChartType == MINI_MULTI_CHART || m_nChartType == MINI_SINGLE_CHART)
	{
		ChangeMiniStockMode(type);
		return;
	}
	CString strTemp;
	//복기 Lib UnLoad
	UnLoad_ReplayChart_Lib();	

	m_nMarketType = type;

	//int nPreChartGubun = 
//	if(m_pLeftBarDlg)
//	{
//		m_pLeftBarDlg->SetMarketType(m_nMarketType);				
//		m_pLeftBarDlg->MoveWindow(GetRect(ID_LEFTBAR,TRUE));
//	}
	//------------------------------------------------------------------------------

	if(m_pInputBarDlg)
	{
		m_pInputBarDlg->SetMarketType(m_nMarketType);	

		m_pInputBarDlg->MoveWindow(GetRect(ID_INPUTBAR,TRUE));
		//m_pInputBarDlg->SetMarKetToolType(m_nMarketType);//2005. 05. 16 - chart gubun ¼³A¤

		//2005. 06. 22 종목 코드를 받은 상태 에서 생성한다.
		BOOL bRequestAPData = FALSE;
		if(m_pInputBarDlg->GetSafeHwnd())
		{
//@유지삭제(zz)
//			m_pInputBarDlg->InitBaseInfo(NULL, bRequestAPData);
//@유지삭제
		}
	}

	//------------------------------------------------------------------------------
	// 2005. 05. 16 chart 구분자를 넘긴다.
	//------------------------------------------------------------------------------
	if(m_pExtInputBarDlg)
	{
		m_pExtInputBarDlg->SetMarketType(m_nMarketType);
		m_pExtInputBarDlg->MoveWindow(GetRect(ID_EXTINPUTBAR,TRUE));
	}
	//------------------------------------------------------------------------------
	// 2005. 05. 16 chart 구분자를 넘긴다.
	//------------------------------------------------------------------------------	
	m_ChartMng.SetMarketType(m_nMarketType);
//@유진삭제
//	m_pInputBarDlg->SetMarKetToolType(m_nMarketType);//2005. 05. 16 - chart gubun 설정
//@유진삭제
//@유진추가
	m_pInputBarDlg->ChangeMarket(m_nMarketType, FALSE, TRUE);
//@유진추가
	//------------------------------------------------------------------------------
	// 2005. 09. 01 Tab에 기존에 선택된 항목을 보인다.
	// 순서 바꾸지 않는다.
	//------------------------------------------------------------------------------
	//m_pLeftBarDlg->SetConfig();

//@유진삭제
//	if(m_hCursor == NULL)
//	{
//		if(m_pLeftBarDlg)
//		{
//			if(m_pLeftBarDlg->m_nMarketType != m_nMarketType)
//			{
//				m_pLeftBarDlg->SetMarketType(m_nMarketType);				
//				m_pLeftBarDlg->MoveWindow(GetRect(ID_LEFTBAR,TRUE));
//				if(m_pLeftBarDlg->m_tabctrl.GetCurSel() == 0)
//					m_pLeftBarDlg->m_tabctrl.SetCurSel(0);
//			}
//		}
//	}
//@유진삭제
	//------------------------------------------------------------------------------


//	if( m_ChartMng.GetpChartItemSelected() != NULL)
//		m_ChartMng.GetpChartItemSelected()->LoadTickMinDayInfo(m_strConfig);

	//SetLeftBarType(m_nLeftBarType);
	//SetShowExtInputBarDlg(m_bShowExtInputBarDlg);
}

void CStdDialog::ChangeMiniStockMode(int type)
{
	m_ChartMng.SaveChart(m_strConfig);
	//===========================================================================
	AllCodeUnAdvise(); //Real 전부 해제..
	//===========================================================================
	
	m_ChartMng.SendMessage(UMSG_RESETOCX);
	m_ChartMng.SendMessage(UMSG_SETMATRIX,0,0);

	m_nMarketType =  type;

//	m_ChartMng.m_bLastEnvUpdated = FALSE;
	//====================================================
#ifdef	_LASTINFO_SAVE
	m_strConfig = m_strUserDir + "\\" + m_strHwndKey + "_" + m_strScreenNo + "_" + ENVINFOFILE;
#else
	m_strConfig = m_strUserDir + "\\" + m_strScreenNo + "_" + ENVINFOFILE;
#endif

	if(m_pInputBarDlg)
	{
		m_pInputBarDlg->m_strCfgFile = m_strConfig;

		m_pInputBarDlg->SetMarketType(m_nMarketType);	
		m_pInputBarDlg->MoveWindow(GetRect(ID_INPUTBAR,TRUE));

//@유진삭제
//		m_pInputBarDlg->SetMarKetToolType(m_nMarketType);//2005. 05. 16 - chart gubun 설정
//@유진삭제

//@유진삭제
//		//2005. 06. 22 종목 코드를 받은 상태 에서 생성한다.
//		BOOL bRequestAPData = FALSE;
//		if(m_pInputBarDlg->GetSafeHwnd())
//		{
//			m_pInputBarDlg->InitBaseInfo(NULL, bRequestAPData);
//		}
//@유진삭제
	}


	//------------------------------------------------------------------------------
	// 2005. 05. 16 chart 구분자를 넘긴다.
	//------------------------------------------------------------------------------	
	m_ChartMng.SetMarketType(m_nMarketType);
//@유진추가
	m_pInputBarDlg->ChangeMarket(m_nMarketType, FALSE, TRUE);
//@유진추가
//@유진삭제
//	if( m_ChartMng.GetpChartItemSelected() != NULL)
//		m_ChartMng.GetpChartItemSelected()->LoadTickMinDayInfo(m_strConfig);
//@유진삭제

//@유진삭제
//	//{{2007.07.23 by LYH
//	if(m_pInputBarDlg)
//		m_pInputBarDlg->SetRecentCode();
//	//}}
//@유진삭제
//	PostMessage(UMSG_EDITCODE_INIT, 0L, 0L);
}
/****************************************************************************/
/* Function : 멀티 차트 폼 로드												*/
/* Paramter : ?																*/
/* Return	:																*/
/* Author	: By DRFN														*/
/* Comments	: FX Margin 하면서 소스 정리 (한국소리치 전용배)				*/
/* Date     : 2008/07/10                                                    */
/****************************************************************************/
void CStdDialog::FormLoad()
{
	m_pMsgBar = new CMessageBar;								// 메세지바 생성
	m_pMsgBar->Create(CMessageBar::IDD,this);

// --> [Edit]  강지원 2008/10/29
//@Sol100618	m_pCommMsg = new CRequestMsg;								// 통신조회 메세지바 생성
//@Sol100618	m_pCommMsg->Create(CRequestMsg::IDD,this);
// <-- [Edit]  강지원 2008/10/29

	m_TRComm = (ITrComm2*)GetTrComm();							// TR 및 DRDS 초기화

	InitDrds();													// 통신 키 초기화

	m_FavoriteMng.SetPath(m_strUserDir);						// 사용자 환경 path얻어오기
	
	if (_access(m_strUserDir, 0) == -1)							// User Directory 생성
		 int retval = _mkdir(m_strUserDir);
/*
#ifdef	_LASTINFO_SAVE
	m_strConfig = m_strUserDir + "\\" + m_strHwndKey + "_" + m_strScreenNo + "_" + ENVINFOFILE;

	CString strTemp = "", strSaveKey = "";
	CString strName = m_strConfig;
	CString strScreenNo = m_strScreenNo;

	if ( m_strScreenKey.IsEmpty() )
	{
		strName.Replace(strScreenNo, "Che_" + strScreenNo);
		strName.Replace(".dat", ".mct");
		FileCopyAndSaveAndDelete(DEFAULT_HWND_COPY);
	}
	else
	{
		strName.Replace(strScreenNo, "Che_" + strScreenNo);
		strName.Replace(".dat", ".mct");
		FileCopyAndSaveAndDelete(HWND_LAST_COPY);
	}
#else																// 설정파일 경로
	m_strConfig = m_strUserDir + "\\" + m_strScreenNo + "_" + ENVINFOFILE;
#endif
*/
	m_strConfig = m_strUserDir + "\\" + m_strScreenNo + "_" + ENVINFOFILE;

	char szTemp[64];
	if (m_nChartType == MULTI_CHART)
	{
		m_nWidthOfLeftBar = WIDTH_SIDEBAR_EXTEND;

		if (m_nWidthOfLeftBar)
		{
			::GetPrivateProfileString("Frame","ShowLeftBar","1",szTemp,sizeof(szTemp),m_strConfig);
			m_nLeftBarType = atoi(szTemp);
		}
		else
			m_nWidthOfLeftBar = WIDTH_SIDEBAR_COLLAPSE;

		::GetPrivateProfileString("Frame","ShowExtInputBarDlg","1",szTemp,sizeof(szTemp),m_strConfig);
		m_bShowExtInputBarDlg = atoi(szTemp);

		::GetPrivateProfileString("Frame","ShowToolBar", "1", szTemp, sizeof(szTemp),m_strConfig);
		m_bShowToolBar = atoi(szTemp);

		::GetPrivateProfileString("Frame","ShowMigyulche", "1", szTemp, sizeof(szTemp),m_strConfig);
		m_bMigyulcheDraw = atoi(szTemp);

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

		::GetPrivateProfileString("Frame","ShowToolBar", "1", szTemp, sizeof(szTemp),m_strConfig);
		m_bShowToolBar = atoi(szTemp);
	}

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
				m_pDlgToolbar = new CDlgToolbar(this);
				if(m_pDlgToolbar)
				{
					m_pDlgToolbar->SetChartMng(&m_ChartMng);
					m_pDlgToolbar->Create(IDD_DLG_TOOLBAR,this);
					m_pDlgToolbar->MoveWindow(GetRect(ID_TOOLBAR,TRUE));
					m_pDlgToolbar->ShowWindow(SW_SHOW);

					m_btnExtTool.FP_vSetImage(&m_ImgList_W8H25, 2, 3);
					m_btnExtTool.FP_vSetToolTipText("툴바축소");
					m_btnExtTool.ShowWindow(SW_SHOW);
				}
			}
		}
		break;
	case MULTI_CHART:
	case SINGLE_CHART:
		{
			m_btnSideBar.SetBorderColor(RGB(157,157,161));
			m_btnSideBar.SetBackGroundColor(GetBkgrndColor(), GetBkgrndColor());

			// 상단바 생성
			m_pInputBarDlg = NULL;
			m_pInputBarDlg = new CInputBarDlg();
			if(m_pInputBarDlg)
			{
				if(m_bMultiItemChart == FALSE)
					m_pInputBarDlg->SetReplayChartMode(FALSE);

				m_pInputBarDlg->SetMarketType(m_nMarketType);				
				m_pInputBarDlg->SetpChartMng(&m_ChartMng);
				m_pInputBarDlg->Create(IDD_INPUTBAR,this);
				m_ChartMng.SetpInputBarDlg(m_pInputBarDlg);
				m_pInputBarDlg->MoveWindow(GetRect(ID_INPUTBAR,TRUE));
				m_pInputBarDlg->ShowWindow(SW_SHOW);
				m_pInputBarDlg->ChangeMarket(m_nMarketType, FALSE, TRUE);

				m_pInputBarDlg->SetAllPosition(m_nChartType);

				//분석툴바 생성
				m_pDlgToolbar = new CDlgToolbar(this);
				if(m_pDlgToolbar)
				{
					m_pDlgToolbar->SetChartMng(&m_ChartMng);
					m_pDlgToolbar->Create(IDD_DLG_TOOLBAR,this);
					m_pDlgToolbar->MoveWindow(GetRect(ID_TOOLBAR,TRUE));
					m_pDlgToolbar->ShowWindow(SW_SHOW);

					m_btnExtTool.FP_vSetImage(&m_ImgList_W8H25, 2, 3);
					m_btnExtTool.FP_vSetToolTipText("툴바축소");
					m_btnExtTool.ShowWindow(SW_SHOW);
				}
			}

			// 사이드바 영역 생성
			
			// 좌측바 생성
			m_pLeftBarDlg = NULL;
			m_pLeftBarDlg = new CLeftBarDlg();
			if(m_pLeftBarDlg)
			{
				m_pLeftBarDlg->SetMarketType(m_nMarketType);				
				m_pLeftBarDlg->SetStdDialog(this);
				m_pLeftBarDlg->SetpChartMng(&m_ChartMng);
				m_pLeftBarDlg->SetpFavoriteThingsMng(&m_FavoriteMng);
				m_pLeftBarDlg->Create(IDD_LEFTBAR,this);
				m_btnSideBar.SetSideBar(m_pLeftBarDlg);
				m_pLeftBarDlg->MoveWindow(GetRect(ID_LEFTBAR,TRUE));
				m_pLeftBarDlg->ShowWindow(SW_SHOW);
				m_ChartMng.SetpLeftBarDlg(m_pLeftBarDlg);
			}
		}

		if(m_pInputBarDlg)
			GetIndicatorNameListIntoComboBox(m_pInputBarDlg->m_cmbChartType, "특수지표");
	}

	if(m_pInputBarDlg)
		SetPermanentHandle(m_pInputBarDlg);
	SetPermanentHandle(m_pLeftBarDlg);

	// 보조상단바 생성
	m_pExtInputBarDlg = NULL;

// --> [Edit]  강지원 2008/11/09
	if ( m_nChartType != MULTI_SPREAD_CHART )
	{
		m_pExtInputBarDlg = new CExtInputBarDlg;
		if(m_pExtInputBarDlg)
		{
			m_pExtInputBarDlg->m_nMarketType = m_nMarketType;				

			m_pExtInputBarDlg->SetpChartMng(&m_ChartMng);
			m_pExtInputBarDlg->SetpLeftBar(m_pLeftBarDlg);
			m_pExtInputBarDlg->Create(IDD_EXTINPUTBAR,this);
			m_ChartMng.SetpExtInputBarDlg(m_pExtInputBarDlg);	
			m_pExtInputBarDlg->SetMarketType(m_nMarketType);
			m_pExtInputBarDlg->MoveWindow(GetRect(ID_EXTINPUTBAR,TRUE));
			m_pExtInputBarDlg->ShowWindow(SW_SHOW);
			if(m_pInputBarDlg->GetType() == DAY_DATA_CHART || 
				m_pInputBarDlg->GetType() == WEEK_DATA_CHART || 
				m_pInputBarDlg->GetType() == MONTH_DATA_CHART)
			EnableQueryOpt(FALSE);
		}
	}
// <-- [Edit]  강지원 2008/11/09

	// 스플릿바 생성
	m_xSplitter1.CreateInWnd(this, ID_SPLITTER, GetRect(ID_SPLITTER));

	// 차트매니저 생성
	m_ChartMng.SetMarketType(m_nMarketType);
// --> [Edit]  강지원 2008/08/26	( Root패스를 먼저 알려준다. )
	m_ChartMng.m_strRootPath = m_strRootPath;
// <-- [Edit]  강지원 2008/08/26
	BOOL bRetValue = m_ChartMng.Create(GetRect(ID_CHART),this, m_strUserDir, m_szImageDir,ID_CHART, (long)((CMChartInterface*)this));
	if(!bRetValue) return;
	m_ChartMng.SetBaseDesk((long)1);            // 데스크가 없어짐으로 양의 정수를 셋팅함으로 기본동작 가능

	if((m_nChartType != MINI_MULTI_CHART && m_nChartType != MINI_SINGLE_CHART) && m_pLeftBarDlg)
		m_pLeftBarDlg->SetConfig();

	if( m_ChartMng.GetpChartItemSelected() != NULL)
		m_ChartMng.GetpChartItemSelected()->LoadTickMinDayInfo(m_strConfig);
#ifdef _DEBUG
	else
	{
		AfxMessageBox("FormLoad : m_ChartMng.GetpChartItemSelected() is NULL !!");
//		return;
	}
#endif 

	// (보조)상단바 타입 및 Visible 셋팅
	SetLeftBarType(m_nLeftBarType);
	SetShowExtInputBarDlg(m_bShowExtInputBarDlg);

	BOOL bRequestAPData = TRUE;

	// 종목코드 컨트롤 생성
	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		m_pInputBarDlg->InitBaseInfo(NULL, bRequestAPData);

	// 스킨색상
//@SolomonGNF IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager, 3);
//@SolomonGNF 	if(pKSLibMng)
//@SolomonGNF 		m_clrBackground = pKSLibMng->Lib_KSLink_GetColor(1);	// 바탕화면 배경
//@SolomonGNF 	else
//@SolomonGNF 		m_clrBackground = DEFAULT_BKGRND_COLOR;		// 기본값

	m_clrBackground = DEFAULT_BKGRND_COLOR;		// 기본값

	// 	// 스킨색상
	int nTheme = 0;//AfxGetMainWnd()->SendMessage(WMU_GET_THEME);	
	OnChangeSkinColor(nTheme, 0);
//	ChangeSkinColor();
	Invalidate();

	//달러/원 데이터 요청
//20080714
//	RequestDollarData();
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

	m_pLeftBarDlg->Invalidate();
	m_ChartMng.Invalidate();
	m_xSplitter1.Invalidate();
	//m_pInputBarDlg->Invalidate();
	m_pInputBarDlg->ShowWindow(SW_HIDE);
	m_pExtInputBarDlg->Invalidate();
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

	if(!m_nLeftBarType && m_nChartType != MINI_MULTI_CHART && m_nChartType != MINI_SINGLE_CHART)
		rect.left = WIDTH_SIDEBAR;

	switch(nID)
	{
	case IDC_SIDEBAR:
		{
			if(!m_nLeftBarType && m_nChartType != MINI_MULTI_CHART && m_nChartType != MINI_SINGLE_CHART)
			{
				rect.left = 0;
				rect.right = WIDTH_SIDEBAR;
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
		if(m_pLeftBarDlg->GetSafeHwnd()&&!bInit)
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
//		if(m_pLeftBarDlg->GetSafeHwnd()&&!bInit)
//		{
//			CRect rectLeftBar;
//			m_pLeftBarDlg->GetWindowRect(rectLeftBar);
//			ScreenToClient(rectLeftBar);
//			if(m_nLeftBarType==1)
//			{
//				rect.left = WIDTH_SIDEBAR;
//				rect.right = rectLeftBar.right;
//			}
//			else if(m_nLeftBarType==2)
//			{
//				rect.right -= WIDTH_SIDEBAR;
//				rect.left = rect.right - rectLeftBar.Width();
//			}
//		}
//		else	// initial
//		{
//			if(m_nLeftBarType==1)
//			{
//				rect.left = WIDTH_SIDEBAR;
//				rect.right = rect.left + m_nWidthOfLeftBar;
//			}
//			else if(m_nLeftBarType==2)
//			{
//				rect.right -= WIDTH_SIDEBAR;
//				rect.left = rect.right - m_nWidthOfLeftBar;
//			}
//		}		
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
				if(m_pLeftBarDlg)
				{
					m_pLeftBarDlg->GetWindowRect(rectLeftBar);
				}
				
				if(m_pInputBarDlg)
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
				else if(m_nLeftBarType==2)	rect.right = rect.right - (m_nWidthOfLeftBar + WIDTHOFSPLITTER);			
			}	
			if(m_bShowExtInputBarDlg)
				rect.bottom = rect.top + HEIGHTOFINPUTBAR + 1;
			else
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
		if(m_pExtInputBarDlg->GetSafeHwnd()&&!bInit)
		{
			CRect rectInputBar;
			CRect rectLeftBar;
			if(m_pLeftBarDlg)
			{
				m_pLeftBarDlg->GetWindowRect(rectLeftBar);
			}		

			if(m_pInputBarDlg)
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
		rect.top = rect.top + HEIGHTOFINPUTBAR + 1;
		rect.bottom = rect.top + HEIGHTOFINPUTBAR - 1;
		break;
	case ID_SPLITTER:
		if(!m_nLeftBarType||!m_pLeftBarDlg)	
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
			if(m_pLeftBarDlg)
			{
				m_pLeftBarDlg->GetWindowRect(rectLeftBar);
			}
			
			ScreenToClient(rectChart);
			if(m_nLeftBarType==1)		rect.left = rectChart.left;
			else if(m_nLeftBarType==2)	rect.right = rect.right - rectLeftBar.Width() - 5;
		}
		else
		{
			if(m_pLeftBarDlg)
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
		}				
		if(m_bShowExtInputBarDlg)
		{
			rect.top = rect.top + HEIGHTOFINPUTBAR*2;
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
		if(m_pDlgToolbar->GetSafeHwnd())
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
	
	if(m_pLeftBarDlg)
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

	if(m_pInputBarDlg)
	{
		m_pInputBarDlg->GetWindowRect(rect);
		ScreenToClient(rect);
		rectNew = GetRect(ID_INPUTBAR,m_bInitResize);
		if(rect!=rectNew)	
		{
			m_pInputBarDlg->MoveWindow(rectNew);
			//m_pInputBarDlg->ShowWindow(SW_SHOW);
		}

/*@sini
		if(m_pInputBarDlg->m_ctrlCodeInput.GetSafeHwnd())
		{
			m_pInputBarDlg->m_ctrlCodeInput.SetWindowPos(NULL, 
				m_pInputBarDlg->m_rectCmb.left, m_pInputBarDlg->m_rectCmb.top,
				m_pInputBarDlg->m_rectCmb.Width(), m_pInputBarDlg->m_rectCmb.Height(),
				SWP_NOZORDER);

			if(m_nChartType != MINI_MULTI_CHART && m_nChartType != MINI_SINGLE_CHART)
			{
				if(m_pInputBarDlg->m_hKSJongName && IsWindow(m_pInputBarDlg->m_hKSJongName))
				{
					CRect rcJN = m_pInputBarDlg->m_rectJongName;
					CWnd::FromHandle(m_pInputBarDlg->m_hKSJongName)->SetWindowPos(NULL, 
						rcJN.left, rcJN.top, rcJN.Width(), rcJN.Height(),
						SWP_NOZORDER);
				}
			}
		}
*/
		//m_pInputBarDlg->Invalidate();
	}
	
	if(m_pExtInputBarDlg)
	{
		m_pExtInputBarDlg->GetWindowRect(rect);
		ScreenToClient(rect);
		rectNew = GetRect(ID_EXTINPUTBAR,m_bInitResize);
		if(rect!=rectNew)	
		{
			m_pExtInputBarDlg->MoveWindow(rectNew);
			//m_pExtInputBarDlg->ShowWindow(SW_SHOW);
		}

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

	if(m_pDlgToolbar)
	{
		m_pDlgToolbar->GetWindowRect(rect);
		ScreenToClient(rect);
		rectNew = GetRect(ID_TOOLBAR,m_bInitResize);
		if(rect!=rectNew)	
		{
			m_pDlgToolbar->MoveWindow(rectNew);

			rectNew.right = rectNew.left;
			rectNew.top += 2;
			rectNew.left = rectNew.right - 8;
			rectNew.bottom = rectNew.top + 20;
			m_btnExtTool.MoveWindow(rectNew);
		}
		m_btnExtTool.Invalidate();
	}

	if(m_bOneClickHide)
	{		
		//20080914 이문수 >>
		//윈클릭 숨기기 셋팅 후 리사이징시 숨겼던 컨트롤들이 다시 보이는 버그때문에 수정
		//false로 셋팅하여 다시 모두 보이게 된다.
		//false를 m_bOneClickHide로 변경 셋팅
		//SetOneClickHide(FALSE);
		//m_pDlgToolbar->SetToggleButton(50066, 0);
		SetOneClickHide(m_bOneClickHide);
		m_pDlgToolbar->SetToggleButton(50066, m_bOneClickHide);
		//20080914 이문수 <<
	}
	m_bInitResize = TRUE;
}

void CStdDialog::OnDestroy() 
{	
	if ( m_pDrdsLib )
	{
		// [Edit] 2009.05.27 김현수
		// 복수 화면을 띄운 상태에서 화면을 종료 시 TIME 실시간을 해제하는 경우가 발생하여
		// 아래의 DrdsUnadviseAll()에서 한 번만 삭제하게 막음
		//m_pDrdsLib->DrdsUnadvise(STOCK_30SEC_PID, "TIME", "", "01", m_hWnd);
//@SolomonGNF 		m_pDrdsLib->DrdsUnadvise(MIGYULJE_PID, "A", "", "MIGYULCHE", m_hWnd);
	}

	CString strTemp;
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("좌측바 위치 저장::Begin", _T("CStdDialog"), 10);
#endif
	strTemp.Format("%d",m_bMigyulcheDraw);
	::WritePrivateProfileString("Frame","ShowMigyulche", strTemp,m_strConfig);

	CChartItem* pChartItem = m_ChartMng.GetpChartItemSelected();
	if( pChartItem) if( pChartItem->GetCheHogaChange() != 1 )
		SaveChart(FALSE);

// <-- [Edit]  강지원 2008/10/30
#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("좌측바 위치 저장::End", _T("CStdDialog"), 10);
#endif

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
	if(m_pMsgBar)
	{
		m_pMsgBar->DestroyWindow();
		delete m_pMsgBar;
		m_pMsgBar = NULL;
	}	
// --> [Edit]  강지원 2008/10/29
//@Sol100618-->
// 	if(m_pCommMsg)
// 	{
// 		m_pCommMsg->DestroyWindow();
// 		delete m_pCommMsg;
// 		m_pCommMsg = NULL;
// 	}
//@Sol100618 <--

// <-- [Edit]  강지원 2008/10/29
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
// --> [Edit]  강지원 2008/11/18	( 리얼해제 막기 )
// --> [Edit]  강지원 2008/11/23	( 핸들에 등록되어있는 리얼을 전부 해제시킨다. )
	if ( m_pDrdsLib )
//@SolomonGNF 		m_pDrdsLib->DrdsUnadviseAll(m_hWnd);
// <-- [Edit]  강지원 2008/11/23

	AllCodeUnAdvise( FALSE ); //Real 전부 해제..
// <-- [Edit]  강지원 2008/11/18
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

#ifdef _WRITELOG_SISE
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

// --> [Edit]  강지원 2008/12/17
#ifdef	_LASTINFO_SAVE
//	LastInfoSave();
	if ( !m_strSaveKey.IsEmpty() )
		FileCopyAndSaveAndDelete(LAST_HWND_COPY);	//	사용했던 파일 지우기

	FileCopyAndSaveAndDelete(HWND_DEFAULT_COPY);
	FileCopyAndSaveAndDelete(DELETE_FILE);	//	사용했던 파일 지우기
#endif
// <-- [Edit]  강지원 2008/12/17

	CExOCXDLL::OnDestroy();
}

// --> [Edit]  강지원 2008/07/25		( 데이터를 넘길때 벡터가 아닌 그냥 Char로 넘김 )
HRESULT CStdDialog::ReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	{
		CString szDebug;
		szDebug.Format("TR[%s](%s)(%s) Len=%d\n", szTR, szMsgCode, szMessage, dwTRDateLen);
		OutputDebugString(szDebug);
	}

	CWnd *pWndRequested = NULL;
	CString strTRCode = szTR;
	strTRCode.Remove(' ');

	BOOL bSendGuideLine = FALSE;
	CString strMsgCode = szMsgCode;
	if (strMsgCode.GetLength() <= 0)
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
		if (strMsgCode.GetLength() > 0 || dwTRDateLen == 0)
		{
			if (!m_bEnable)
				EnableWindowAll(TRUE);
		}

		return 1L;
	}

	STTRINFO trInfo;
	long lCnt = m_arryQuery.GetSize();
	CString sTemp;
	for (long lPos=0; lPos<lCnt; lPos++)
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

			lCnt = m_arryQuery.GetSize();
			break;
		}
	}

	if (strTRCode == QUERY_STRFOFOREIGN_HOGA)
	{
		if (pWndRequested->GetSafeHwnd())
		{
			stReceiveFromServer stR;
			stR.m_strTRCode = strTRCode;
			stR.m_pBuffer = aTRData;
			stR.m_dwLength = dwTRDateLen;
			pWndRequested->SendMessage(UMSG_RECEIVEFROMSERVER,0,(long)&stR);
		}

		return 0L;
	}

	//<<20100308_JS.Kim 솔로몬
	if (strTRCode == QUERY_STRFOFOREIGN_JAN)		// 잔고 내역
	{
		CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrOpenPosition;
		
		FX_CHE_STRUCT* pCheSt = NULL;
		int nIdxData = 0;
		while(nIdxData < dwTRDateLen)
		{
			pCheSt = (FX_CHE_STRUCT *)((char *)aTRData+nIdxData);
			arrOpenPosition.Add(pCheSt);

			nIdxData += sizeof(FX_CHE_STRUCT);
		}

		CChartItem* pChartItem = (CChartItem *)pWndRequested;
		if(pChartItem)
		{
			pChartItem->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 1, (long)&arrOpenPosition);
			//pChartItem->InvokeAddIn("ChartKoreaSorimachiAddIn.dll:USER_COMMENT", 1, (long)&arrOpenPosition);
		}
		return 0L;
	}
	else if (strTRCode == QUERY_STRFOFOREIGN_CHE2)	// 체결내역
	{
		CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrPendingOrder;
		
		FX_CHE_STRUCT* pCheSt = NULL;
		int nIdxData = 0;
		while(nIdxData < dwTRDateLen)
		{
			pCheSt = (FX_CHE_STRUCT *)((char *)aTRData+nIdxData);
			arrPendingOrder.Add(pCheSt);
			nIdxData += sizeof(FX_CHE_STRUCT);
		}

		CChartItem* pChartItem = (CChartItem *)pWndRequested;
		if(pChartItem)
		{
			// 체결내역은 진입/청산 표시 쪽에 보낸다.
			pChartItem->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 6, (long)&arrPendingOrder);
		}
		return 0L;
	}
	//>>


	if (pWndRequested)
	{
		//2007.04.25 by LYH 받은 코드(현재 CodeCombo 코드와 비교해서 다를 경우 재조회)
		if (m_strRecvCode == "" || m_pInputBarDlg->m_pCodeInput->GetCode() == "" || m_ChartMng.IsInLoop() || m_pInputBarDlg->m_pCodeInput->GetCode() == m_strRecvCode)
		{
			if (pWndRequested->GetSafeHwnd())
			{
				ConvertKoscomData(strTRCode, (char*)aTRData, dwTRDateLen);
				stReceiveFromServer stR;
				stR.m_strTRCode = strTRCode;
				stR.m_pBuffer = (LPVOID)(LPCTSTR)m_strOut;
				stR.m_dwLength = m_strOut.GetLength();

				pWndRequested->SendMessage(UMSG_RECEIVEFROMSERVER,0,(long)&stR);
			}
			m_strRecvCode = "";
		}
		else
			m_pInputBarDlg->SetKeyCodeChange(NID_CODEINPUT, (LPARAM)(LPCTSTR)m_pInputBarDlg->m_pCodeInput->GetCode());
	}
	else
	{
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
			ConvertKoscomData(szTR, (char*)aTRData, dwTRDateLen);
			stReceiveFromServer stR;
			stR.m_strTRCode = szTR;
			stR.m_pBuffer = (LPVOID)(LPCTSTR)m_strOut;
			stR.m_dwLength = m_strOut.GetLength();

			if (dwTRDateLen>=12)
			{
				CChartItem *pWndChartItem = NULL;
				char szWndChartItem[18];
				memcpy(szWndChartItem,aTRData,12);
				szWndChartItem[12] = NULL;
				pWndChartItem = (CChartItem *)atol(szWndChartItem);
				if (pWndChartItem->GetSafeHwnd())
					pWndChartItem->SendMessageToInvoke(INVOKEMSG_RECEIVEDATA,(long)&stR, sizeof(stReceiveFromServer));					
			}
		}
	}

	
	BOOL bRet = m_ChartMng.IsToBeReceivedMore();
	if (!m_arryQuery.GetSize()&&!bRet)
		EnableWindowAll();

	SendMessage2StatusBar(MSG_SEARCH_END, (LPTSTR)(LPCTSTR)szMessage);

	return S_OK;
}

HRESULT	CStdDialog::ReceiveMessage(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage)
{
	CString szTmp;
	szTmp.Format("[%s]%s\n", szMsgCode, szMessage);
	
	if(strcmp(szMsgCode, "e")==0)		// 타임아웃
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
	else
		EnableWindowAll(TRUE);	//@유진 임시코드임. 이 루틴이 남아있으면 삭제바람.

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
		if(m_pLeftBarDlg->GetSafeHwnd())
		{
			m_pLeftBarDlg->EnableWindow();
		}
		if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		{
			m_pInputBarDlg->EnableWindow();
		}
		if(m_pExtInputBarDlg->GetSafeHwnd())
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
		
		KillTimer(1);
// --> [Edit]  강지원 2008/10/29
//@Sol100618		m_pCommMsg->HideMessage();
// <-- [Edit]  강지원 2008/10/29
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}
	else
	{
		if(m_pLeftBarDlg->GetSafeHwnd())
		{
			m_pLeftBarDlg->EnableWindow(FALSE);
		}
		if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())	
		{
			m_pInputBarDlg->EnableWindow(FALSE);
		}

		if(m_pExtInputBarDlg->GetSafeHwnd())	
		{
			m_pExtInputBarDlg->EnableWindow(FALSE);
		}

		if(m_ChartMng.GetSafeHwnd())
		{
			m_ChartMng.EnableWindow(FALSE);
		}

		CWnd *pFocus = GetFocus();
		CWnd *pParent = GetParent();
		if (pParent->IsChild(pFocus))
		{
// --> [Edit]  강지원 2008/10/29
			CString		strTemp = "조회중입니다...";
//@Sol100618			m_pCommMsg->ShowMessage((LPCTSTR)strTemp,(CWnd*)this);
// <-- [Edit]  강지원 2008/10/29
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		}

// --> [Edit]  강지원 2008/07/23	( 임시 : 타임아웃 일단 막음 )
//		AfxMessageBox("SetTimer(1)");
//		SetTimer(1,20000,NULL); // 20sec
// <-- [Edit]  강지원 2008/07/23
	}
}

// 테스트모드이면 서버에서 받은 데이터 말고 서버에서 받은 데이터로 처리한다.
int CStdDialog::Send2Server(LPCTSTR lpTrCode,LPCTSTR lpData,int nSize,BOOL bWait, int nArrayIndex)
{
// 	{
// 		CString szDebug;
// 		szDebug.Format("->SVR TR[%s]Len=%d [%-*.*s]\n", lpTrCode, nSize, nSize, nSize, lpData);
// 		OutputDebugString(szDebug);
// 	}
// 
// 	int nTestMode = AfxGetApp()->GetProfileInt("TESTMODE", "HW", 0);

	SendMessage2StatusBar(MSG_GOING);	
	EnableWindowAll(FALSE);	

	if (m_TRComm)
	{
		if (CString(lpTrCode) == QUERY_STRFOFOREIGN_HOGA)
		{
			m_TRComm->SetBaseData("SCREENCODE", m_strScreenNo);
			m_TRComm->SetBaseData("PtManager", m_szPTManager);
			m_TRComm->SetTR(lpTrCode);
			m_TRComm->Send2Server((LPVOID)lpData, nSize, TRKEY_STRFOFOREIGN_HOGA);
		}
		else if(CString(lpTrCode) == QUERY_STRFOFOREIGN_JAN || CString(lpTrCode) == QUERY_STRFOFOREIGN_CHE2)
		{
			m_TRComm->SetBaseData("SCREENCODE", m_strScreenNo);
			m_TRComm->SetTR(lpTrCode);
			m_TRComm->Send2Server((LPVOID)lpData, nSize, TRKEY_FID_END);
		}
		// 종목TR
		else
		{
			// (2008/9/14 - Seung-Won, Bae) for Pair Tick
			if (GetRequestPairTickState() != E_RPTS_PAIR)
			{
				m_TRComm->SetTR(lpTrCode);
				m_TRComm->SetBaseData("PtManager", m_szPTManager);
				m_TRComm->SetBaseData("SCREENCODE", m_strScreenNo);

				return m_TRComm->Send2Server((LPVOID)lpData, nSize, TRKEY_FID_END);
			}
			else
				return 0;
		}

		return 0;
	}
	else
		return 0;
}

void CStdDialog::SetShowExtInputBarDlg(BOOL bShowExtInputBarDlg)
{
	m_bShowExtInputBarDlg = bShowExtInputBarDlg;
	if(m_pInputBarDlg)
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

	if(!m_pInputBarDlg||!m_pExtInputBarDlg||!m_pLeftBarDlg||!m_ChartMng.GetSafeHwnd()) return;
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
	m_pExtInputBarDlg->MoveWindow(rect);
	m_pExtInputBarDlg->ShowWindow(SW_SHOW);

	rect = GetRect(ID_LEFTBAR,TRUE);
	if(rect.IsRectNull())
	{
		m_pLeftBarDlg->ShowWindow(SW_HIDE);
		m_pLeftBarDlg->ModifyStyle(WS_CHILD, WS_POPUP|WS_DLGFRAME|WS_CLIPSIBLINGS);
		m_pLeftBarDlg->SetParent(this);

		if(m_btnSideBar.GetSafeHwnd())
		{
			m_btnSideBar.MoveWindow(GetRect(IDC_SIDEBAR, TRUE));
			m_btnSideBar.ShowWindow(SW_SHOW);
			m_btnSideBar.Invalidate();
		}
	}
	else
	{
		m_pLeftBarDlg->ModifyStyle(WS_POPUP|WS_DLGFRAME|WS_CLIPSIBLINGS, WS_CHILD);
		m_pLeftBarDlg->SetParent(this);
		
		m_pLeftBarDlg->MoveWindow(rect);
		m_pLeftBarDlg->ShowWindow(SW_SHOW);

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

	m_pLeftBarDlg->Invalidate();
	m_xSplitter1.Invalidate();
}

LRESULT CStdDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case UMSG_SEND2SERVER:
		{
			stSend2Server *pSt = (stSend2Server *)lParam;

			if ((CString(pSt->m_strTRCode) != QUERY_STRFOFOREIGN) &&		//	해외선옵
				(CString(pSt->m_strTRCode) != QUERY_STRFOFOREIGN_HOGA) &&	//	호가창조회
				(CString(pSt->m_strTRCode) != QUERY_STRFOFOREIGN_HOGA2) &&	//	매수/매도 호가조회
				(CString(pSt->m_strTRCode) != QUERY_STRFOFOREIGN_CHE2)		&&		// 체결
				(CString(pSt->m_strTRCode) != QUERY_STRFOFOREIGN_JAN) )		// 잔고
				break;

			if ((CString(pSt->m_strTRCode) != QUERY_STRFOFOREIGN))
				memcpy(&m_st2Server, pSt, sizeof(stSend2Server));				

			int lCnt = m_arryQuery.GetSize();

			STTRINFO trInfo, trCheckInfo;
			trInfo.m_pWndRequested = pSt->m_pWnd;
			trInfo.m_strTrCode = pSt->m_strTRCode;

			for (long lPos=0;lPos<lCnt;lPos++)
				trCheckInfo = m_arryQuery.GetAt(lPos);
			
			if (pSt->m_nType==0)
			{
				trInfo.m_nType = 0;
				m_arryQuery.Add(trInfo);
			}
			else
			{
				trInfo.m_nType = pSt->m_nType;
				m_arryQueryInvoke.Add(trInfo);
			}

			Send2Server(pSt->m_strTRCode,(char*)pSt->m_pBuffer,pSt->m_lLength, (BOOL)wParam);
		}
		break;
	case UMSG_SENDSHAREDCODE:
		{
			if (!m_bEnable)
				break;

// --> [Edit]  강지원 2008/11/07	( 종목조회 중이면 종목이 새로 들어와도 뱉어버린다. )
			if (m_ChartMng.IsInLoop())
				break;
// <-- [Edit]  강지원 2008/11/07

			// 종목컨트롤을 통한 공유처리
			if (m_pInputBarDlg)
			{
				STINPUTCODEDATA stInputCodeData;
				stInputCodeData.m_strCode = CString((LPCTSTR)wParam);
				stInputCodeData.m_strTime = "";
				stInputCodeData.m_nDefault = MODE_DEFAULT;
				stInputCodeData.m_nMode = CHANGE_ITEM;

				if (m_nCodeInput)
					m_ChartMng.SendMessage(UMSG_INPUTCODETOCHARTMNG, MODE_DEFAULT, (LPARAM)&stInputCodeData);

// --> [Edit]  강지원 2008/10/28
				//	현재 RQ의 선택되어 있는 시간대의 Index 구하기
				CChartItem* pSelectedChartItem = m_ChartMng.GetpChartItemSelected();
				if (pSelectedChartItem)
				{
					// lParam 이 1이면 Get
					long lResult = pSelectedChartItem->SendMessage(UDM_CHANGE_TIMEDIFF, 0, 1);
					// lResult => 1 Based 시간대 Index, -1 : 원래 시간대인 상태

					SetTabTimeIndex(lResult);
					ChangeTimeSet(lResult);
				}
// <-- [Edit]  강지원 2008/10/28
			}
		}		
		break;
	case UMSG_SENDSYNCCODE:
		break;
	case UMSG_SENDSHARETIME:
		m_ChartMng.SetSharedTime((LPCTSTR)wParam);
		break;
	case UMSG_SENDTOMSGBOX:
		m_pMsgBar->ShowMessage((LPCTSTR)wParam,(CWnd*)lParam);
		break;
	case UMSG_EDITCODE_INIT:
		if (m_pInputBarDlg)
			m_pInputBarDlg->SetCodeInit();
//		break;
	case UMSG_UPDATECODE:
		{
			//코스콤 종목공유 세팅(재정의)
			if(m_pInputBarDlg)
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
					// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
					CString strCodeInputCode = m_pInputBarDlg->GetTypedItemCode();
					CString strSetCode = (LPCTSTR)wParam;
					if(strSetCode.GetLength() && strCodeInputCode != strSetCode)
					{
						m_pInputBarDlg->SetShareCodeChanged(TRUE);
						// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
						m_pInputBarDlg->m_pCodeInput->SetCode(CStdDialog::GetRealItemCode( ( LPCTSTR)wParam));
						m_pInputBarDlg->SetShareCodeChanged(FALSE);
					}
				}
			}
		}
		break;
	case UMSG_DROPCODE:
		{
			//코스콤 종목공유 세팅(재정의)
			if(m_nChartType == MULTI_CHART && m_pInputBarDlg)
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
				if(m_pInputBarDlg)
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
			if(m_pInputBarDlg)
				m_pInputBarDlg->SetMode(nMode);
		}
		break;
	case UMSG_SETHIGHLIGHTINFO:
		{
			if(m_pExtInputBarDlg)
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
	case USMG_PLEASE_RESIZE:			break;
	case UMSG_SETMATRIX:				break;
	case UMSG_FORCE_SHOW_HOGAWND:		break;
	case WM_DRDS_DATA:					break;
	case UMSG_SENDTR_HOGA_DATA:
		{
			CString* pStrCode, sCode;
			pStrCode = (CString*)wParam;
			sCode.Format("%s", pStrCode->GetBuffer(pStrCode->GetLength()));
			Request_StockHogaData(sCode);
		}
		break;
	case UMSG_DISCONNECT_HOGA_REAL:
		Request_Disconnect_StockRealHogaData(_T(""));
		break;
	case UMSG_GET_CHARTMODE:
		{
			// 복기차트 사용여부 : 복수종목 - ojtaso (20070530)
			if(m_pInputBarDlg)
			{
				if(wParam == 1 && !m_pInputBarDlg->GetReplayChartMode())
					return -1;

				return m_pInputBarDlg->m_nCurrentMode;
			}
		}
		break;
	case UMSG_GET_REPLAY_STARTTIME:
		{
			if(m_pInputBarDlg)
				return m_pInputBarDlg->GetReplayStartTime();
		}
		break;
	case UMSG_GET_REPLAY_DAYBEFORE:
		{
			if(m_pInputBarDlg)
				return m_pInputBarDlg->GetReplayDayBefore();
		}
		break;
	case UMSG_STOP_JIPYOCYCLE:
		{
			if(m_pLeftBarDlg)
			{
				m_pLeftBarDlg->SendMessage(message, 0, 0);
			}			
		}
		break;
	case UMSG_SHOW_HOGADLG:
		if(m_ChartMng)
			m_ChartMng.m_pFloatingDlg->Invalidate();
		break;
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
					OutputDebugString(strTemp);
#endif
					return 0l;
				}				
			}
			return 1l;
		}
		break;
	case UMSG_GETJONGMOKLINK:
		{
			if(m_pExtInputBarDlg)
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
			if(m_pInputBarDlg)
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
			if(m_pDlgToolbar)
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
		if(m_pDlgToolbar)
		{
			if(lParam == 50000)
				m_pDlgToolbar->ResetAllButton();
			else
				m_pDlgToolbar->SetToggleButton(wParam, lParam);
		}
		break;
	case WMU_GET_KEY_INFO_ARRAY:
		if(m_pInputBarDlg)
		{
			//m_pInputBarDlg->m_ctrlCodeInput.OnGetKeyInfoArray(wParam, lParam);
		}
		break;
	case UMSG_CREATESCREEN_LINK:
		if(m_pInputBarDlg)
		{
// --> [Edit]  강지원 2008/10/30	( 호가테이블에서 주문화면 연동을 위한 메시지 )
//			::SendMessage(m_hViewInMain, RMSG_GET_LINKINFODATA, wParam, lParam);
// <-- [Edit]  강지원 2008/10/30
		}
		break;

	case UMSG_CHART_TICKCHART:
		{
			//	연속월물이 들어왔을경우 호가차트나 틱차트일경우 조회를 하지 않는다. 
			if ( ((CInputBarDlg*)m_pInputBarDlg)->GetCheHogaType() == 1 )
				return 0;

			if ( ((CInputBarDlg*)m_pInputBarDlg)->GetType() == TICK_DATA_CHART )
				return 0;

			return 1;
		}		
		break;
		
	}
	return CExOCXDLL::WindowProc(message, wParam, lParam);
}
/****************************************************************************/
/* Function : 통신 키 초기화												*/
/* Paramter : ?																*/
/* Return	:																*/
/* Author	: By DRFN														*/
/* Comments	: FX Margin 하면서 소스 정리 (한국소리치 전용배)				*/
/****************************************************************************/
void CStdDialog::InitDrds()
{
	m_nDrdsInfoCnt = 1;
	
	m_DrdsInfo[0].DataName = "FOFR";								// 해외선물
	m_DrdsInfo[0].FieldName = "*";
	m_DrdsInfo[0].KeyName = "";
	m_DrdsInfo[0].nID = 0;
	m_DrdsInfo[0].bAdvise = FALSE;
}

BOOL CStdDialog::MakeDrdsConnectAdviseFromCode(int nI, BOOL bAddMod)
{	
	char* DataName = (LPSTR)(LPCTSTR)m_DrdsInfo [nI].DataName;
	char* FieldName = (LPSTR)(LPCTSTR)m_DrdsInfo [nI].FieldName;

	// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
	CString KeyName = GetRealItemCode (m_DrdsInfo[nI].KeyName);

	UINT	nID = m_DrdsInfo[nI].nID;
	IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager);

	if(!pDrdsMng) return FALSE;

	// 실시간 라이브러리 로드
	if (m_pDrdsLib == NULL)		m_pDrdsLib = pDrdsMng->AdviseDrdsLib();
	if (!m_pDrdsLib) return FALSE;

	/*
	CString strType = "1";

	ST_CODEDATA oneCode;

	oneCode.m_strCode = m_DrdsInfo[nI].KeyName;
	
	if (m_pInputBarDlg->m_pDataManager)
		oneCode.m_bType = GetMarketTypeFromCode(oneCode.m_strCode);
	else
		oneCode.m_bType = atoi(strType);
	
	if(nI == 0)
		m_ListStockRealCodes.AddTail(oneCode);

	if (m_RealTrCounter.AddTr (KeyName) == 1)
	{
		m_pDrdsLib->DrdsInit (m_hWnd);
		m_pDrdsLib->DrdsAdvise (nID, DataName, FieldName, ( char *)( const char *)KeyName);
		m_DrdsInfo[nI].bAdvise = TRUE;
	}
	*/
	CString strType("1");
	ST_CODEDATA oneCode;
	int nType;
	oneCode.m_strCode = m_DrdsInfo[nI].KeyName;
	if(m_pInputBarDlg->m_pDataManager)
		oneCode.m_bType = GetMarketTypeFromCode(oneCode.m_strCode, nType);// m_pDataManager->GetCodeInfo("", 2, oneCode.m_strCode);
	else
		oneCode.m_bType = atoi(strType);

	// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
//	if(bAddMod)
	if(m_RealTrCounter.AddTr(oneCode.m_strCode) == 1)
	{
		//@Solomon-->
		//m_pDrdsLib->DrdsInit(m_hWnd);
		//m_pDrdsLib->DrdsAdvise(nID, DataName, FieldName, KeyName);
		if(m_pPTManager) m_pPTManager->RequestMChartReal('A', DataName, KeyName);
		//@Solomon<--
		m_DrdsInfo[nI].bAdvise = TRUE;
	}


	return TRUE;
}

// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
void CStdDialog::AddRealCodes()
{
}

// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
void CStdDialog::AddRealCodeList(LPCTSTR lpszCode)
{
	ST_CODEDATA oneCode;
	oneCode.m_strCode = lpszCode;

	int nType;
	int nMarketType = GetMarketTypeFromCode(lpszCode, nType);
	BOOL bRet = FALSE;
	if(nMarketType == CMasterTypeInfo::STOCK_CHART)
	{
		m_ListStockRealCodes.AddTail(oneCode);
	}
	else if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
	{
		m_ListUpjongRealCodes.AddTail(oneCode);
	}
	else if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
	{
		m_ListFutOptRealCodes.AddTail(oneCode);
	}
	else if(nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
	{
		m_ListStockFutOptRealCodes.AddTail(oneCode);
	}
	else if(nMarketType == CMasterTypeInfo::FOREIGN_CHART)
	{
		m_ListForeignRealCodes.AddTail(oneCode);
	}
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
//@SolomonGNF 					m_pDrdsLib->DrdsUnadvise(0, "", "", (LPSTR)(LPCSTR)strCode, m_hWnd);
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
//@SolomonGNF 				m_pDrdsLib->DrdsUnadvise(0, "", "", (LPSTR)(LPCSTR)pListRealCodes->GetAt(pos).m_strCode, m_hWnd);
				pListRealCodes->RemoveAt(pos);
			}
		}
		pListRealCodes->RemoveAll();
	}
}

BOOL CStdDialog::MakeDrdsUnConnectUnAdviseFromCode(int nI, BOOL bAddMod)
{
	if(!m_pDrdsLib) return FALSE;

	IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager);
	if(!pDrdsMng) return FALSE;

	if(m_pDrdsLib == NULL)
		return TRUE;

	char* DataName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].DataName;
	char* FieldName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].FieldName;
	// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
	CString KeyName = GetRealItemCode( m_DrdsInfo[nI].KeyName);
	UINT nID = m_DrdsInfo[nI].nID;
/*
	if(m_RealTrCounter.RemoveTr( KeyName) == 0)
	{
		//<<20100316_JS.Kim 솔로몬
		//m_pDrdsLib->DrdsUnadvise(nID, DataName, FieldName, ( char *)( const char *)KeyName);
		//>>
		m_DrdsInfo[nI].bAdvise = FALSE;
	}

	//{{2007.01.02 by LYH 리얼등록
	if(nI == 0)
		RemoveRealCodes(&m_ListStockRealCodes, m_DrdsInfo[nI].KeyName);
	//}}
*/
	if(m_RealTrCounter.RemoveTr(m_DrdsInfo[nI].KeyName) == 0)
	{
		//@Solomon
		//m_pDrdsLib->DrdsUnadvise(nID, DataName, FieldName, KeyName);
		if(m_pPTManager) m_pPTManager->RequestMChartReal('U', DataName, KeyName);
		m_DrdsInfo[nI].bAdvise = FALSE;
	}

	return TRUE;
}

// 복수종목 실시간 처리 : 복수종목 - ojtaso (20070109)
void CStdDialog::OnCodeUnAdvise(CString strCode, BOOL bAddMode)
{
	// 기존 코드가 리스트에 있으면 삭제
	// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
	int nType;
	int nMarketType = GetMarketTypeFromCode(strCode, nType);
	if(nMarketType == CMasterTypeInfo::STOCK_CHART)
	{
// --> [Edit]  강지원 2008/10/29
//		m_DrdsInfo[0].KeyName = strCode;
//		MakeDrdsUnConnectUnAdviseFromCode(0, bAddMode);								
		CChartItem*	pChartItem = m_ChartMng.GetpChartItemSelected();
		if ( pChartItem )
		{
			if ( m_ChartMng.GetpChartItemSelected()->GetCheHogaChange() == 0 )
			{
				m_DrdsInfo[0].KeyName = strCode;
				MakeDrdsUnConnectUnAdviseFromCode(0, bAddMode);								
			}
			else
			{
				m_DrdsInfo[1].KeyName = strCode;
				MakeDrdsUnConnectUnAdviseFromCode(1, bAddMode);								
			}
		}
		else
		{
			m_DrdsInfo[0].KeyName = strCode;
			MakeDrdsUnConnectUnAdviseFromCode(0, bAddMode);								
			m_DrdsInfo[1].KeyName = strCode;
			MakeDrdsUnConnectUnAdviseFromCode(1, bAddMode);								
		}

		// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
// <-- [Edit]  강지원 2008/10/29
	}
	else if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
	{
		m_DrdsInfo[1].KeyName = strCode;
		MakeDrdsUnConnectUnAdviseFromCode(1, bAddMode);
		// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
	}
	else if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
	{
		if(strCode[0]=='1'||strCode[0]=='4')
		{
			m_DrdsInfo[2].KeyName = strCode;
			MakeDrdsUnConnectUnAdviseFromCode(2, bAddMode);								
			// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
		}
		else
		{
			m_DrdsInfo[3].KeyName = strCode;
			MakeDrdsUnConnectUnAdviseFromCode(3, bAddMode);								
			// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
		}
	}
	else if(nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
	{
		MakeDrdsUnConnectUnAdviseFromCode(4, bAddMode);								
	}
	else if(nMarketType == CMasterTypeInfo::FOREIGN_CHART)
	{
		m_DrdsInfo[6].KeyName = strCode;
		MakeDrdsUnConnectUnAdviseFromCode(6, bAddMode);								
		// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
	}
}

// 복수종목 실시간 처리 : 복수종목 - ojtaso (20070109)
void CStdDialog::OnCodeAdvise(CString strCode, BOOL bAddMode)
{
	// Drds 연결 
	// 기존 코드가 리스트에 없으면 추가
	// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
	int nType;
	int nMarketType = GetMarketTypeFromCode(strCode, nType);
	if(nMarketType == CMasterTypeInfo::STOCK_CHART)
	{
// --> [Edit]  강지원 2008/10/29
//		m_DrdsInfo[0].KeyName = strCode;
//		MakeDrdsConnectAdviseFromCode(0, bAddMode);
		if ( m_ChartMng.GetpChartItemSelected()->GetCheHogaChange() == 0 )
		{
			m_DrdsInfo[0].KeyName = strCode;
			MakeDrdsConnectAdviseFromCode(0, bAddMode);
		}
		else
		{
			m_DrdsInfo[1].KeyName = strCode;
			MakeDrdsConnectAdviseFromCode(1, bAddMode);
		}
// <-- [Edit]  강지원 2008/10/29
	}
	else if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
	{
		m_DrdsInfo[1].KeyName = strCode;
		MakeDrdsConnectAdviseFromCode(1, bAddMode);
	}
	else if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
	{
		if(strCode[0]=='1'||strCode[0]=='4')
		{
			m_DrdsInfo[2].KeyName = strCode;
			MakeDrdsConnectAdviseFromCode(2, bAddMode);
		}
		else
		{
			m_DrdsInfo[3].KeyName = strCode;
			MakeDrdsConnectAdviseFromCode(3, bAddMode);
		}
	}
	else if(nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
	{
		MakeDrdsConnectAdviseFromCode(4, bAddMode);
	}
	else if(nMarketType == CMasterTypeInfo::FOREIGN_CHART)
	{
		m_DrdsInfo[6].KeyName = strCode;
		MakeDrdsConnectAdviseFromCode(6, bAddMode);
	}

}

BOOL CStdDialog::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)  
	{		
		UINT nChar = (UINT)pMsg->wParam;
		switch(nChar)
		{
		case VK_RETURN :
		case VK_ESCAPE :
			{
				return FALSE;
				break;					
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
	
	if(GetSafeHwnd())
	{
		Resize();			
	}	
}

void CStdDialog::SetSmallView(BOOL bSmall /*= TRUE*/)
{
	if(bSmall)
	{
		if(m_pInputBarDlg)
			m_pInputBarDlg->HideLeftBarBtn(SW_HIDE);
		m_nTempLeftBarType = GetLeftBarType();
		SetLeftBarType(0);
		m_bSmallScreenMode = TRUE;
	}
	else
	{
		if(m_pInputBarDlg)
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
	m_pLeftBarDlg->SetPacketFromOutSide(strPacketFromOutSide);
}

void CStdDialog::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TIMER_EVENTID_FORMLOAD)
	{
		KillTimer(nIDEvent);
		_DoFirstRequest();
	}
	else
		KillTimer(nIDEvent);

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
	CTabCodeSearch *pTabCodeSearch = (CTabCodeSearch*)m_pLeftBarDlg->m_tabctrl.GetWndCodeSearch();
	if(pTabCodeSearch)
	{
		pTabCodeSearch->OnBtprev();
	}	
}

void	CStdDialog::SetJongListNext()
{
	CTabCodeSearch *pTabCodeSearch = (CTabCodeSearch*)m_pLeftBarDlg->m_tabctrl.GetWndCodeSearch();
	if(pTabCodeSearch)
	{
		pTabCodeSearch->OnBtnext();
	}	
}

CString CStdDialog::GetAccountInfo() 
{ 
	return m_strAccountInfo;
}

BOOL CStdDialog::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(!m_bEnable)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

/****************************************************************************/
/* Function : 초기 정보 초기화												*/
/* Paramter :																*/
/* Return	:																*/
/* Author	: By DRFN														*/
/* Comments	: FX Margin 하면서 소스 정리 (한국소리치 전용배)				*/
/****************************************************************************/
void CStdDialog::SetInitInfo()
{
	int nID = 21;

	IGateManager* pGateMng;
	AfxGetIGateManager(pGateMng);
	if(pGateMng == NULL)
		return;

	IMainInfoManager* pManager = (IMainInfoManager*)pGateMng->GetInterface(nID);

	m_strUserID = pManager->GetEncUserID();					// 사용자 ID 얻어 오기
	if(m_strUserID.GetLength() == 0)
	{
		AfxMessageBox("UserID를 얻을 수 없습니다.");
	}

	m_strRootPath = pManager->GetRootDir();					// Root Path얻어 오기
	if(m_strRootPath.GetLength()==0)
	{
		AfxMessageBox("RootPath를 얻을 수 없습니다.");
	}

	m_strUserDir.Format("%s%s", pManager->GetEncUserDir(), "\\Chart");

// --> [Edit]  강지원 2008/10/14	( 이미지 경로 변경 )
//	m_szImageDir = m_strRootPath + "\\system\\Image\\Chart";		// 이미지 Path 설정
	m_szImageDir = m_strRootPath + "\\Icss\\Image\\drchart";		// 이미지 Path 설정
// <-- [Edit]  강지원 2008/10/14

	// (2008/9/13 - Seung-Won, Bae) for Multi-Language
	g_iMetaTable.m_strDataFolderPath.Format(_T("%s\\"), pManager->GetDataDir());
	g_iMetaTable.LoadMetaTable();
}

//#============================== TR 전송 Test ==========================
//* 호가조회
#define TR_HOGA_FUTURE "HWS10010" 
void CStdDialog::SendTR_TEST(CString strRecvCode)
{	
	strRecvCode = "10156000";
	if(m_TRComm)
	{
		m_TRComm->SetTR(TR_HOGA_FUTURE);
		m_TRComm->Send2Server((LPVOID)(LPCSTR)strRecvCode, strRecvCode.GetLength(), FALSE);
	}
}
//#============================== TR 전송 Test ==========================



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

void CStdDialog::SetRealInfo(
									void* pDrds		,   // Drds포인터인데 NULL로 보냄.
									HWND hWnd		,   // Chart OCX의 윈도우 핸들
									UINT id			, 	// CPacket* 
									LPSTR DataName	,	// ex: "S31"
									LPSTR FieldName	, 	// ex: "time"
									LPSTR KeyName	)	// ex: "000660"
{	
	if(!CString(DataName).GetLength()) return;
	if(!CString(FieldName).GetLength()) return;
	if(!CString(FieldName).GetLength()) return;
}

void CStdDialog::UnSetRealInfo(
									void* pDrds		,   // Drds포인터인데 NULL로 보냄.
									HWND hWnd		,   // Chart OCX의 윈도우 핸들
									UINT id			, 	// CPacket* 
									LPSTR DataName	,	// ex: "S31"
									LPSTR FieldName	, 	// ex: "time"
									LPSTR KeyName	)	// ex: "000660"
{
}


void CStdDialog::SendStockReal(char* pData)
{
}

void CStdDialog::SendUpjongReal(char* pData)
{
}

void CStdDialog::SendFutureReal(char* pData)
{
}

void CStdDialog::SendOptionReal(char* pData)
{
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
	CString strFidInfo("0                                                                               ");
	CString strTrInfo;
	strTrInfo.Format("%-10s", "FX@KRW");
	CString strRecordInfo("1               000;");
	CString strOutput;
	strOutput = strFidInfo + strTrInfo + strRecordInfo;

	stSend2Server st;
	st.m_nType = 0;
	st.m_pWnd = (CWnd*)this;
	st.m_pBuffer = (void*)(LPCTSTR)strOutput;
	st.m_lLength= strOutput.GetLength();
	st.m_strTRCode = QUERY_STRDOLLAR;	
	m_TRComm->SetTR(QUERY_STRDOLLAR);
	m_TRComm->SetBaseData("PtManager", m_szPTManager);
	m_TRComm->SetBaseData("SCREENCODE", m_strScreenNo);
	m_TRComm->Send2Server(st.m_pBuffer, st.m_lLength, FALSE);	
}

void CStdDialog::Request_StockHogaData(CString strRecvCode)
{
	if ((m_ChartMng.m_pFloatingDlg == NULL) ||
		(m_ChartMng.m_pFloatingDlg->GetSafeHwnd() == NULL) ||
		(m_ChartMng.m_pFloatingDlg->m_bIconState) ||			//축소 상태면 TR조회 안한다.
		(m_pInputBarDlg == NULL) ||
		(m_pInputBarDlg->GetSafeHwnd() == NULL))
	{
		return;
	}

	CString strCode;
	if (strRecvCode.GetLength() == CODE_STOCK_LENGTH)
		strCode = strRecvCode;
	else
		strCode = m_pInputBarDlg->GetTypedItemCode();

	if (!strCode.GetLength())	// 호가창 Data Clear & Real 해제
	{
		Request_Disconnect_StockRealHogaData(m_pInputBarDlg->m_strOldCode);
		return;
	}

	if (m_ChartMng.m_pFloatingDlg != NULL)
	{
		if (m_ChartMng.m_pFloatingDlg->GetSafeHwnd())
		{	
			if (m_pInputBarDlg->m_strOldCode != strCode)
				Request_Disconnect_StockRealHogaData(m_pInputBarDlg->m_strOldCode);

			strCode = strCode.Mid(3);
			strCode = CStdDialog::GetRealItemCode( strCode, TRUE);
			if (strCode.GetLength() < SIMBOL_CODE_MIN_LENGTH)
				return;

			char stJMCode[HCQ01112_IN_SZ];
			memset(&stJMCode, 0x00, HCQ01112_IN_SZ);			
			memcpy(stJMCode, strCode, strCode.GetLength());		

			CString strOutput;
			strOutput.Format("%-*s", HCQ01112_IN_SZ, strCode);

			stSend2Server st;
			st.m_nType = 0;
			st.m_pWnd = (CWnd*)m_ChartMng.m_pFloatingDlg;
			st.m_pBuffer = (void*)(LPCTSTR)strOutput;
			st.m_lLength= HCQ01112_IN_SZ;
			st.m_strTRCode = QUERY_STRFOFOREIGN_HOGA;
			SendMessage(UMSG_SEND2SERVER,0,(long)&st);

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
			m_DrdsInfo[2].KeyName = strCode;
			MakeDrdsConnectAdviseFromCode(2, FALSE);		// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
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
				m_DrdsInfo[2].KeyName = m_ChartMng.m_pFloatingDlg->m_strCode;
				MakeDrdsUnConnectUnAdviseFromCode(2, FALSE);		// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
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

BOOL CStdDialog::ChangedChartItem()
{
	return FALSE;	
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

void CStdDialog::Send2HanHwa_09501(void* pData, int nLen, CWnd* pRequestWnd, CString strTRCode)
{
	EnableWindowAll(FALSE);	

	SendMessage2StatusBar(MSG_GOING);
	m_pRqWndFromTab = NULL;
	m_pRqWndFromTab = pRequestWnd;
}

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
		|| wParam == CToolOptionInfo::T_REDO_WORK)
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
					if(m_pInputBarDlg)
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


				pWnd->SendMessage(RMS_ANALYSIS_FUNC, wParam, lParam);	
			}
		}
	}
	return 0L;
}

void CStdDialog::SendMessage2StatusBar(int nMsgType, const char* pMsg, int nNum /*=0*/)
{
//100618-alzioyes: 이 루틴은 뭐하는 거야?
// 	CString strMessage;
// 	switch(nMsgType)
// 	{
// 		case	MSG_GOING:
// 			strMessage.Format("처리중...");
// 			break;
// 		default:
// 			if( lstrlen(pMsg) )
// 				strMessage.Format("%s", pMsg);
// 			break;
// 	}
}


void CStdDialog::Send2HanHwa_IFCond(void* pData, int nLen, CWnd* pRequestWnd, CString strTRCode)
{
	EnableWindowAll(FALSE);	

	SendMessage2StatusBar(MSG_GOING);
	m_pRqWndFromTab = NULL;
	m_pRqWndFromTab = pRequestWnd;
}


BOOL CStdDialog::Load_ReplayChart_Lib()
{	
	//100618-alzioyes : 해외는 복기 안하잖아.
	return FALSE;

	BOOL bResult  = FALSE;

	CString strFileRoot;
	strFileRoot = m_strRootPath + "\\dev\\GNF_ReplayChart.dll";

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
		AfxMessageBox("\n# GNF_ReplayChart.dll이 없습니다.");
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
					m_pExtInputBarDlg->EnableWindow(FALSE);
				
					if(pReplayInfo != NULL)
					{
						pReplayInfo->hCallHwnd		= this->m_hWnd;
						pReplayInfo->hRecvMainFrame = m_hMainFrame;
						m_strSpeed.Format("%d",		pReplayInfo->nSpeed);
						m_ChartMng.GetChartAllCode(pReplayInfo->arrStrCode);
						if( pReplayInfo->arrStrCode.GetSize() <= 0)
						{
							AfxMessageBox("복기를 시작할 종목이 없습니다.\n선물옵션 차트에서 복기가 가능한 종목은\n지수선물옵션 종목입니다.");
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
	m_strSpeed.Format("%d",					  nSpeed);
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

// 허수봉설정.
// (2008/11/2 - Seung-Won, Bae) for Gap Revision.
bool CStdDialog::SetImaginaryCandleAndGapRevisionToOutside( const bool bImaginaryCandle, const bool bUseGapRevision)
{ 
// (2008/11/2 - Seung-Won, Bae) Do not support sync for all ChartItem.
//	m_ChartCfgInfo.bImaginaryCandle	= bImaginaryCandle; // TRUE 면 허봉
//	m_ChartCfgInfo.bUseGapRevision	= bUseGapRevision;	// TRUE 면 갭보정
//	m_pExtInputBarDlg->SendMessage( UMSG_SETNORMALIZEDMINUTETIMERULE, bImaginaryCandle, bUseGapRevision);	

	// (2008/11/2 - Seung-Won, Bae) Set Minute data for Normalized Time or Gap Revision.
	m_ChartMng.SendMessage( UMSG_SET_NORMALIZEDMINUTETIMERULE_AND_GAPREVISION, bImaginaryCandle, bUseGapRevision);

	return TRUE;
}	

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


bool CStdDialog::GetImaginaryCandleToOutside(bool& bImaginaryCandle		)	// 허수봉설정.
{
	return m_ChartMng.GetImaginaryCandleToOutside(bImaginaryCandle);
}
// (2008/11/2 - Seung-Won, Bae) for Gap Revision.
bool CStdDialog::GetUseGapRevisionToOutside(bool& bUseGapRevision)	// 갭보정
{
	return m_ChartMng.GetUseGapRevisionToOutside(bUseGapRevision);
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
	
	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.bottom = rcClient.top + 2;
	dc.FillSolidRect(rcClient, RGB(255,255,255));

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
/****************************************************************************/
/* Function : 차트 이미지 초기화											*/
/* Paramter :																*/
/* Return	:																*/
/* Author	: By DRFN														*/
/* Comments	: FX Margin 하면서 소스 정리 (한국소리치 전용배)				*/
/****************************************************************************/
void CStdDialog::InitImageList()
{
	BOOL bOK = FALSE;											// 버튼 이미지 로드 플래그

	CBitmap		bitmap;
	CString		strBitmapFile;	
	
	m_static_nChartCnt4ImgList++;								// 로드된 차트 갯수


	if ( !m_ImgList_W15H15.m_hImageList)						// Width 15, Height 15 이미지 파일 로드
	{
		if ( !m_ImgList_W15H15.Create( 15, 14, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W15_H15.bmp file" );			
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W15_H15.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W15H15.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}
	
	if ( !m_ImgList_W20H20.m_hImageList)						// Width 20, Height 20 이미지 파일 로드
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

	if ( !m_ImgList_W20H20_Tool.m_hImageList)					// Width 15, Height 15 이미지 파일 로드(TOOL)
	{
		if ( !m_ImgList_W20H20_Tool.Create( 19, 19, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not Find W20_H20_Tool.bmp file" );			
		}
		strBitmapFile.Format("%s\\%s", m_szImageDir,  "W20_H20_Tool.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W20H20_Tool.Add(&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject();
	}

	if ( !m_ImgList_W46H20.m_hImageList)						// Width 46, Height 20 이미지 파일 로드
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
	
	if ( !m_ImgList_W50H20.m_hImageList)						// Width 50, Height 20 이미지 파일 로드
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

	if ( !m_ImgList_W151H21.m_hImageList)						// Width 151, Height 21 이미지 파일 로드
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

	if ( !m_ImgList_W28H22.m_hImageList)						// Width 28, Height 24 이미지 파일 로드
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

	if ( !m_ImgList_W17H20.m_hImageList)						// Width 17, Height 20 이미지 파일 로드
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

	if ( !m_ImgList_W43H20.m_hImageList)						// Width 43, Height 20 이미지 파일 로드
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

	if ( !m_ImgList_W58H20.m_hImageList)						// Width 58, Height 20 이미지 파일 로드
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

	if ( !m_ImgList_W72H20.m_hImageList)						// Width 72, Height 19 이미지 파일 로드
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
	
	if ( !m_ImgList_W46H20_Gwan.m_hImageList)					// Width 46, Height 20 이미지 파일 로드
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

	if ( !m_ImgList_W8H25.m_hImageList)							// Width 8, Height 25 이미지 파일 로드
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
	m_static_nChartCnt4ImgList--;
	
	if ( m_static_nChartCnt4ImgList == 0)
	{
		m_ImgList_W15H15.DeleteImageList();

		m_ImgList_W20H20.DeleteImageList();
		m_ImgList_W20H20_Tool.DeleteImageList();
		m_ImgList_W46H20.DeleteImageList();
		m_ImgList_W50H20.DeleteImageList();

		m_ImgList_W151H21.DeleteImageList();
		m_ImgList_W28H22.DeleteImageList();
		m_ImgList_W17H20.DeleteImageList();
		m_ImgList_W43H20.DeleteImageList();	
		m_ImgList_W58H20.DeleteImageList();	
		m_ImgList_W72H20.DeleteImageList();	
		m_ImgList_W46H20_Gwan.DeleteImageList();
		m_ImgList_W8H25.DeleteImageList();
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

void CStdDialog::SendHogaNCodeToSpeedJumun(CString strCode, CString strPrice)
{
	m_xGSharedCli.SetData("S51210-호가;", strPrice);
}

void CStdDialog::AllCodeUnAdvise(BOOL bFlag /* = TRUE */)
{
	if ( bFlag )
	{
		for(int i = 0; i < m_nDrdsInfoCnt; ++i)
			MakeDrdsUnConnectUnAdviseFromCode(i, FALSE);
	}

//@SolomonGNF 	IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager, 3);
	IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager);
	if(!pDrdsMng) return;

	if(m_pDrdsLib)
	{
		RemoveDrdsFromCode(&m_ListStockRealCodes);
		RemoveDrdsFromCode(&m_ListUpjongRealCodes);
		RemoveDrdsFromCode(&m_ListFutOptRealCodes);
		RemoveDrdsFromCode(&m_ListStockFutOptRealCodes);
		RemoveDrdsFromCode(&m_ListForeignRealCodes);

		m_pDrdsLib->DrdsClose("");

		pDrdsMng->UnadviseDrdsLib(m_pDrdsLib);
		m_pDrdsLib = NULL;
	}

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
	
	if(m_pInputBarDlg)
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
	return m_pExtInputBarDlg->GetTimeSyncFlag();
}

void CStdDialog::SetTimeSyncFlag(BOOL bCheck)
{
	SetBReplayMode(bCheck);

	m_ChartMng.SetBTypeLink(bCheck);
	m_ChartMng.SetBReplayChartMode(bCheck); 
	m_pExtInputBarDlg->SetTimeSyncFlag(bCheck);
	m_pExtInputBarDlg->EnableWindow(!bCheck);
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
	m_pExtInputBarDlg->EnableWindow(!bReplay);
}

BOOL CStdDialog::CheckDataTypeForReplay()
{
	return m_ChartMng.CheckDataTypeForReplay();
}

LONG CStdDialog::OnReplayEnd(WPARAM wp, LPARAM lp)
{
	if ((m_nMarketType != CMasterTypeInfo::STOCK_CHART) && (m_nMarketType != CMasterTypeInfo::FUTOPT_CHART))
		return 1L;

	return 1;
}

LONG CStdDialog::OnReplaySetTotalSec(WPARAM wp, LPARAM lp)
{
	if ((m_nMarketType != CMasterTypeInfo::STOCK_CHART) && (m_nMarketType != CMasterTypeInfo::FUTOPT_CHART))
		return 1L;
	
	if(m_pInputBarDlg)
		m_pInputBarDlg->SetReplayTotalSec(wp);

	return 1;
}

LONG CStdDialog::OnReplayEachSec(WPARAM wp, LPARAM lp)
{
	if ((m_nMarketType != CMasterTypeInfo::STOCK_CHART) && (m_nMarketType != CMasterTypeInfo::FUTOPT_CHART))
		return 1L;

	CString sTime((LPTSTR)(LPCTSTR)wp);

	if(m_pInputBarDlg)
		m_pInputBarDlg->SetReplayEachSec(sTime);

	return 1;
}


void CStdDialog::SetOpenMapData(LPCTSTR szOpenData, short nDataLen) 
{
	if(m_pInputBarDlg)
	{
		// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
		CString strCodeInputCode = m_pInputBarDlg->GetTypedItemCode();
		CString strSetCode = (LPCTSTR)szOpenData;
		if(strSetCode.GetLength() && strCodeInputCode != strSetCode)
		{
			m_pInputBarDlg->SetShareCodeChanged(TRUE);
			// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
			m_pInputBarDlg->m_pCodeInput->SetCode(CStdDialog::GetRealItemCode( ( LPCTSTR)szOpenData));
			m_pInputBarDlg->SetShareCodeChanged(FALSE);
		}
	}
}

//@유진추가
void CStdDialog::ChangeCode(LPCTSTR szCode, int nDefault/* = MODE_DEFAULT*/, int nMode/* = CHANGE_ITEM*/) 
{
	// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
	CString strSBCode( szCode);
	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	if( m_pInputBarDlg) if( m_pInputBarDlg->GetSafeHwnd())
		strSBCode = m_pInputBarDlg->GetTypedItemCode( szCode, GetRequestPairTickState());

	int nType;
	ChangeStockMode(GetMarketTypeFromCode(szCode, nType));

	STINPUTCODEDATA stInputCodeData;
	stInputCodeData.m_strCode = strSBCode;
	stInputCodeData.m_strTime = "";
	stInputCodeData.m_nDefault = nDefault;
	stInputCodeData.m_nMode = nMode;
	// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)
	m_ChartMng.SendMessage(UMSG_INPUTCODETOCHARTMNG, nDefault, (LPARAM)&stInputCodeData);

}

//@유진추가
void CStdDialog::ChangeTimeSet(int nIndex, BOOL	bSetFlag /* = FALSE */) 
{
	m_ChartMng.SetTimeGab(nIndex, bSetFlag);
}

void CStdDialog::ChangeMarket(UINT nMarketType, BOOL bUpdateCbMarket/* = TRUE*/ , BOOL bRequestToServer/* = FALSE*/) 
{
	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		m_pInputBarDlg->ChangeMarket(nMarketType, bUpdateCbMarket, bRequestToServer);
}

void CStdDialog::EnableQueryOpt(BOOL bEnable)
{
	if(m_pExtInputBarDlg && m_pExtInputBarDlg->GetSafeHwnd())
		m_pExtInputBarDlg->EnableQueryOpt(bEnable);
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
				if(m_pInputBarDlg)
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

					ChangeCode(strCode, MODE_USER, nSelectMode - 1000);
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
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		//HINSTANCE hInstResourceClient = AfxGetResourceHandle();
		//AfxSetResourceHandle(g_hInstance);

		m_hCursor = AfxGetApp()->LoadCursor(IDC_HANDCUR);
		//AfxSetResourceHandle(hInstResourceClient);
		::SetCursor(m_hCursor);
	}

	return 0L;
}

//003 {{ 호가TR_AddDataKey		
void CStdDialog::AddDataKey_Hoga()
{
//@SolomonGNF -->
// 	m_TRComm->AddDataKey(15007);	//기준가
// 	m_TRComm->AddDataKey(15313);	//예상체결가격
// 	m_TRComm->AddDataKey(15315);	//예상체결수량	//-> 예상거래량으로 임시받음
// 	//}}
// 
// 	m_TRComm->AddDataKey(DI_NAME);	//종목명
// 	m_TRComm->AddDataKey(DI_PRICE);	//현재가
// 	//m_TRComm->AddDataKey(DI_SIGN);	//등락부호
// 	m_TRComm->AddDataKey(15317);	//등락부호
// 	m_TRComm->AddDataKey(15004);	//등락률
// 	m_TRComm->AddDataKey(15472);	//대비율
// 	m_TRComm->AddDataKey(DI_VOLUME);	//누적거래량	//-> 거래량으로 임시받음
// 	m_TRComm->AddDataKey(DI_AMOUNT);	//누적거래대금	//-> 거래대금으로 임시받음
// 	//m_TRComm->AddDataKey(14659);	//호가잔량기준시간
// 	
// 	// 매도 8 + 40 + 
// 	int nIdx;
// 	int nDataKey;
// 
// 	nDataKey = DI_OFFER_HO1;		// 매도호가1
// 	for(nIdx=0; nIdx < 5; nIdx++)
// 	{
// 		m_TRComm->AddDataKey(nDataKey+nIdx);		
// 	}
// 
// 	nDataKey = DI_OFFER_REM1;		// 매도호가잔량1
// 	for(nIdx=0; nIdx < 5; nIdx++)
// 	{
// 		m_TRComm->AddDataKey(nDataKey+nIdx);		
// 	}
// 
// 	nDataKey = DI_BID_HO1;		// 매수호가1
// 	for(nIdx=0; nIdx < 5; nIdx++)
// 	{
// 		m_TRComm->AddDataKey(nDataKey+nIdx);		
// 	}
// 
// 	nDataKey = DI_BID_REM1;		// 매수호가잔량1
// 	for(nIdx=0; nIdx < 5; nIdx++)
// 	{
// 		m_TRComm->AddDataKey(nDataKey+nIdx);		
// 	}
// 
// 	//{{ 2007.06.27 by LYH 총잔량을 10호가잔량으로 수정
// 	//m_TRComm->AddDataKey(DI_OFFER_TOTREM);	//총매도잔량
// 	//m_TRComm->AddDataKey(DI_BID_TOTREM);	//총매수잔량
// 	m_TRComm->AddDataKey(14565);	//10호가매도잔량
// 	m_TRComm->AddDataKey(14567);	//10호가매수잔량
// 	//}}
// 	m_TRComm->AddDataKey(DI_OFFER_OUT_TOT);	//시간외매도잔량
// 	m_TRComm->AddDataKey(DI_BID_OUT_TOT);	//시간외 매수잔량
// 
// 	m_TRComm->AddDataKey(16117);	//상한가
// 	m_TRComm->AddDataKey(16118);	//하한가
// 	//{{ 2007.06.27 by LYH 시가대신 예상등락율 받음
// 	m_TRComm->AddDataKey(15326);	//시가->사용안함 -> 예상등락율로 사용 
// 	//}}
// 	m_TRComm->AddDataKey(15010);	//고가
// 	m_TRComm->AddDataKey(15011);	//저가
// 
// 	m_TRComm->AddDataKey(3007);		//전일거래량  //-> 전일거래량_스프레드제외 데이터로 임시사용
// 	m_TRComm->AddDataKey(16143);	//상장kospi발행kosdaq 주식수
//@SolomonGNF <--
}

//003 }}

//008 {{
void CStdDialog::ChangeUpjongMode(UINT type)
{
	if(m_pInputBarDlg)
		m_pInputBarDlg->InitUpjongCodeCombo(type);
}
//008 }}

// END

//Real process
HRESULT CStdDialog::ReceiveRealData(WORD wFID, LPCSTR szMainKey, void* pRealData)
{
	return 1L;
}

void CStdDialog::ConvertHogaData(LPVOID	aTRData, long dwTRDateLen)
{
//@SolomonGNF -->
// 	STDR_FID* pStdrFid_Rec1 = (STDR_FID*)aTRData;
// 	m_strOut = "";
// 
// 	CString strOut;
// 	// 메인키
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][0];
// 	// 기준가
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][1];
// 	// 예상체결가격
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][2];
// 	// 예상체결수량
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][3];
// 	// 종목명
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][4];
// 	// 현재가
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][5];
// 	// 등락부호
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][6];
// 	// 등락폭
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][7];
// 	// 대비율
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][8];
// 	// 누적거래량
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][9];
// 	// 누적거래대금
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][10];
// 	// 매도호가1
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][11];
// 	// 매도호가2
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][12];
// 	// 매도호가3
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][13];
// 	// 매도호가4
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][14];
// 	// 매도호가5
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][15];
// 	// 매도잔량1
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][16];
// 	// 매도잔량2
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][17];
// 	// 매도잔량3
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][18];
// 	// 매도잔량4
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][19];
// 	// 매도잔량5
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][20];
// 	// 매수호가1
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][21];
// 	// 매수호가2
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][22];
// 	// 매수호가3
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][23];
// 	// 매수호가4
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][24];
// 	// 매수호가5
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][25];
// 	// 매수잔량1
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][26];
// 	// 매수잔량2
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][27];
// 	// 매수잔량3
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][28];
// 	// 매수잔량4
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][29];
// 	// 매수잔량5
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][30];
// 	// 총매도잔량
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][31];
// 	// 총매수잔량
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][32];
// 	// 시간외매도잔량
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][33];
// 	// 시간외 매수잔량
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][34];
// 	// 상한가
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][35];
// 	// 하한가
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][36];
// 	// 예상등락율
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][37];
// 	// 고가
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][38];
// 	// 저가
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][39];
// 	// 전일거래량
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][40];
// 	// 상장kospi발행kosdaq 주식수
// 	strOut += pStdrFid_Rec1->vvRpValueList[1][41];
// 	//strData = "          ";
// 	m_strOut = strOut;

//@SolomonGNF <--
}
// END


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

	if(!m_pInputBarDlg||!m_pExtInputBarDlg||!m_pLeftBarDlg||!m_ChartMng.GetSafeHwnd()) return;
	rect = GetRect(ID_INPUTBAR,TRUE);
	m_pInputBarDlg->MoveWindow(rect);
	m_pInputBarDlg->SetLeftBarType(m_nLeftBarType);

	rect = GetRect(ID_EXTINPUTBAR,TRUE);
	m_pExtInputBarDlg->MoveWindow(rect);

	if(m_bOneClickHide)
	{		
		m_pInputBarDlg->ShowWindow(SW_HIDE);
		m_pExtInputBarDlg->ShowWindow(SW_HIDE);
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
		m_pExtInputBarDlg->ShowWindow(SW_SHOW);

		rect = GetRect(ID_LEFTBAR,TRUE);
		if(rect.IsRectNull())
		{
			m_pLeftBarDlg->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pLeftBarDlg->MoveWindow(rect);
			m_pLeftBarDlg->ShowWindow(SW_SHOW);
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
//	if(m_pInputBarDlg) // 2005. 04. 22 by Nam
//		m_pInputBarDlg->SetViewCount(p_nDataCountInView);

	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		m_pInputBarDlg->SendMessage(UMSG_SETZOOMVALUE, p_nDataCountInView);

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
	m_pLeftBarDlg->Invalidate();
	m_ChartMng.Invalidate();
	m_xSplitter1.Invalidate();

	if(m_pInputBarDlg)
		m_pInputBarDlg->Invalidate();
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

//차트 종목들 리얼 등록
void CStdDialog::AddRealCodes(CString strTrCode, LIST_CODEDATA *pListRealCodes) 
{
//@유진삭제
//	m_TRComm->ResetRealTR(strTrCode);
//@유진삭제
	if(pListRealCodes->GetCount() <= 0)
		return;

//@유진삭제
//	m_TRComm->SetTRCondition(strTrCode, strTrCode);
//@유진삭제
	ST_CODEDATA oneCode;
	int nOffset = 0;
	char cCode[6+1]={0,}; 
	int nIndex = 0;
	UINT nCodeLength=0;
	CByteArray bArr;
	for(POSITION pos=pListRealCodes->GetHeadPosition(); pos; )
	{
		oneCode = pListRealCodes->GetNext(pos);
		if(oneCode.m_bType == 79 || oneCode.m_bType == 80 || oneCode.m_bType == 81 || oneCode.m_bType == 82 || oneCode.m_bType == 83 || oneCode.m_bType == 84 || oneCode.m_bType == 85 || oneCode.m_bType == 86)
		{
			nCodeLength = 16;
		}
		else
		{
			nCodeLength = oneCode.m_strCode.GetLength();
			if(nCodeLength != CODE_STOCK_LENGTH && nCodeLength != FUTOPT_LENGTH && nCodeLength != (UPJONG_LENGTH-1))
				continue;
		}

		if(nIndex == 0)
		{
//@SolomonGNF -->
// 			if(oneCode.m_bType == 2 || oneCode.m_bType == 4)
// 				m_TRComm->AddMainKey(DI_CODE, oneCode.m_strCode, C_DT_USHORT, 2);
// 			else
// 				m_TRComm->AddMainKey(DI_CODE, oneCode.m_strCode, C_DT_STRING, nCodeLength);
// 			
// 			m_TRComm->AddOptionKey(1, NULL, 5, 0, 0);
// 			m_TRComm->SetRealUpdateType(2);			//  RT_UPDATE	2	// 업데이트 타입
// 			//{{2007.06.26 by LYH 주식리얼 등록(예상체결 포함으로 수정)
// 			if(strTrCode == "20000")
// 			{
// 				m_TRComm->AddDataKey(15313);//현재가
// 				m_TRComm->AddDataKey(15009);//시가
// 				m_TRComm->AddDataKey(15010);//고가
// 				m_TRComm->AddDataKey(15011);//저가
// 				m_TRComm->AddDataKey(15317);//등락부호
// 				m_TRComm->AddDataKey(15316);//전일비
// 				m_TRComm->AddDataKey(15326);//등락률
// 				m_TRComm->AddDataKey(15315);//거래량
// 				m_TRComm->AddDataKey(DI_TIME);//체결시간
// 			}
// 			else
// 				//}}
// 			{
// 				m_TRComm->AddDataKey(15001);//현재가
// 				m_TRComm->AddDataKey(15009);//시가
// 				m_TRComm->AddDataKey(15010);//고가
// 				m_TRComm->AddDataKey(15011);//저가
// 				m_TRComm->AddDataKey(15006);//등락부호
// 				m_TRComm->AddDataKey(15472);//전일비
// 				m_TRComm->AddDataKey(15004);//등락률
// 				m_TRComm->AddDataKey(15015);//거래량
// 				m_TRComm->AddDataKey(DI_TIME);//체결시간
// 			}
//@SolomonGNF <--
			nIndex++;
		}
		else
		{
		}
		bArr.Add(oneCode.m_bType);
	}
}

//차트 종목들 리얼 등록
void CStdDialog::RemoveRealCodes(LIST_CODEDATA *pListRealCodes, CString strKey) 
{
	ST_CODEDATA oneCode;
	for(POSITION pos=pListRealCodes->GetHeadPosition(); pos; )
	{
		oneCode = pListRealCodes->GetAt(pos);

		{
			if(strKey == oneCode.m_strCode)
			{
				pListRealCodes->RemoveAt(pos);
				break;
			}
		}
		pListRealCodes->GetNext(pos);
	}
}

int CStdDialog::GetWidthOfLeftBar()
{
	return m_nWidthOfLeftBar;
}

// 복수종목 리얼 해제 : 복수종목 - ojtaso (20070209)
void CStdDialog::OnDeleteChartItem(LPCSTR lpszItemCode)
{
	OnCodeUnAdvise(lpszItemCode, TRUE);

	int nType;
	int nMarketType = GetMarketTypeFromCode(lpszItemCode, nType);
	if(nMarketType == CMasterTypeInfo::STOCK_CHART)
	{
		RemoveRealCodes(&m_ListStockRealCodes, lpszItemCode);
		AddRealCodes("20000", &m_ListStockRealCodes);
	}
	else if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
	{
		RemoveRealCodes(&m_ListUpjongRealCodes, lpszItemCode);
		AddRealCodes("20001", &m_ListUpjongRealCodes);
	}
	else if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
	{
		RemoveRealCodes(&m_ListFutOptRealCodes, lpszItemCode);
		AddRealCodes("20002", &m_ListFutOptRealCodes);
	}
	else if(nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
	{
		RemoveRealCodes(&m_ListStockFutOptRealCodes, lpszItemCode);
		AddRealCodes("20002", &m_ListStockFutOptRealCodes);
	}
	else if(nMarketType == CMasterTypeInfo::FOREIGN_CHART)
	{
		RemoveRealCodes(&m_ListForeignRealCodes, lpszItemCode);
		AddRealCodes("20003", &m_ListForeignRealCodes);
	}
}

// 차트 초기화때 모든 리얼 해제 : 복수종목 - ojtaso (20070621)
void CStdDialog::ResetChartAll()
{
	if(!m_ListStockRealCodes.IsEmpty())
	{
		m_ListStockRealCodes.RemoveAll();
		AddRealCodes("20000", &m_ListStockRealCodes);
	}
	if(!m_ListUpjongRealCodes.IsEmpty())
	{
		m_ListUpjongRealCodes.RemoveAll();
		AddRealCodes("20001", &m_ListUpjongRealCodes);
	}
	if(!m_ListFutOptRealCodes.IsEmpty())
	{
		m_ListFutOptRealCodes.RemoveAll();
		AddRealCodes("20002", &m_ListFutOptRealCodes);
	}
	if(!m_ListStockFutOptRealCodes.IsEmpty())
	{
		m_ListStockFutOptRealCodes.RemoveAll();
		AddRealCodes("20002", &m_ListStockFutOptRealCodes);
	}
	if(!m_ListForeignRealCodes.IsEmpty())
	{
		m_ListForeignRealCodes.RemoveAll();
		AddRealCodes("20003", &m_ListForeignRealCodes);
	}
}

// 종목명 얻기 : 복수종목 - ojtaso (20080213)
CString CStdDialog::GetJongMokNameByCode(LPCSTR lpszItemCode, LPCTSTR lpszTRCode/* = NULL*/)
{
	if(!m_pInputBarDlg)
		return _T("");

	return m_pInputBarDlg->GetJongMokNameByCode(lpszItemCode, lpszTRCode);
}

// 복수종목 선택 : 복수종목 - ojtaso (20080215)
void CStdDialog::OnSelectChartItem(ST_SELCHART* pSelChart)
{
	m_pInputBarDlg->InitChartMulitItem(pSelChart);
// --> [Edit]  강지원 2008/12/04	( 주기구분을 설정해준다. )
	m_pExtInputBarDlg->m_chType = m_pInputBarDlg->GetType();
// <-- [Edit]  강지원 2008/12/04
	m_pExtInputBarDlg->InitChartMulitItem(pSelChart);
}

// 현재 종목과 선택된 종목이 같은지 : 복수종목 - ojtaso (20070223)
BOOL CStdDialog::IsSameItemCodeName(LPCTSTR lpszItemName)
{
	return !::lstrcmp(lpszItemName, m_pInputBarDlg->GetCurCodeNameFromControl());
}

// 상품선물옵션차트 - ojtaso (20071001)
USHORT CStdDialog::GetDotPosition(CString& strItemCode)
{
	if(strItemCode.GetAt(0) == _T('2') || strItemCode.GetAt(0) == _T('3'))
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
	if(m_pInputBarDlg)
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
		// (2005/9/1 - Seung-Won, Bae) Line Skip for Only Section.
		strCurTime.Format("\r\n\r\n\r\n[%02d:%02d:%02d:%03d  ", sTime.wHour, sTime.wMinute, sTime.wSecond, 
			sTime.wMilliseconds);
		strBuffer = strCurTime + p_szSection + "]\r\n";
		_hwrite( m_hLogFile, ( LPCTSTR)strBuffer, strBuffer.GetLength());
	}
	// (2005/9/2 - Seung-Won, Bae) in No Section Log, Insert Time in front of Line
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
	strFileName.Format( "%s\\MultiChart%s_%04d%02d%02d.log", szLogFolderPath, m_strScreenNo, sTime.wYear, sTime.wMonth, sTime.wDay);
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


#ifdef _WRITELOG_SISE
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
		// (2005/9/1 - Seung-Won, Bae) Line Skip for Only Section.
		strCurTime.Format("\r\n[%02d:%02d:%02d:%03d  ", sTime.wHour, sTime.wMinute, sTime.wSecond, 
			sTime.wMilliseconds);
		strBuffer = strCurTime + p_szSection + "]\r\n";
		_hwrite( m_hLogFile, ( LPCTSTR)strBuffer, strBuffer.GetLength());
	}
	// (2005/9/2 - Seung-Won, Bae) in No Section Log, Insert Time in front of Line
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
	strFileName.Format( "%s\\MultiChart%s_%04d%02d%02d_%02d%02d%02d.log", szLogFolderPath, m_strScreenNo, sTime.wYear, sTime.wMonth, sTime.wDay, sTime.wHour, sTime.wMinute, sTime.wSecond);
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

	WriteToStringLog("로그 기록 시작 =========================================================================", _T("CStdDialog"), 10);
}

// Log - ojtaso (20080107)
void CStdDialog::CloseLogFile()
{
	WriteToStringLog("로그 기록 종료 =========================================================================", _T("CStdDialog"), 10);

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

//@Solomon_GNF-->
/*
long CStdDialog::OnGetBroad(WPARAM wParam, LPARAM lParam)
{
	if(!m_pDrdsLib || !m_ChartMng)
		return 0;

	int pRealType = -1;
	char* pRealStruct = NULL;
	m_pDrdsLib->GetRealStructData(wParam, lParam, pRealStruct, pRealType);

	if(pRealType == STOCK_SISE_PID)
	{
		CRealData_Common realData_Common;
		CChartRealConverter::Convert_FX_STOCK_SISE(pRealStruct, realData_Common);
		m_ChartMng.SetRealJPriceData((LPVOID)&realData_Common, pRealType);
		return 1L;
	}
	else if(pRealType == STOCK_30SEC_PID)
	{
	CRealData_Common realData_Common;
	CChartRealConverter::Convert_FX_STOCK_30SEC(pRealStruct, realData_Common);
	m_ChartMng.SetRealJPriceData((LPVOID)&realData_Common, pRealType);
	return 1L;
	}
	else if(pRealType == STOCK_HOGA0_PID)	
	{
		CRealData_Common realData_Common;
		CChartRealConverter::Convert_FX_STOCK_HOGA(pRealStruct, realData_Common);
		m_ChartMng.SetRealJPriceData((LPVOID)&realData_Common, pRealType);
		return 1L;
	}
	else if(pRealType == MIGYULJE_PID)
	{
		CRealData_Common realData_Common;
		CChartRealConverter::Convert_ORDERINFO(pRealStruct, realData_Common);
		m_ChartMng.SetRealJPriceData((LPVOID)&realData_Common, pRealType);
		return 1L;
	}
	
	return 0L;
}
*/
//@Solomon_GNF<--

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

LRESULT CStdDialog::OnFormInitAfter(WPARAM wParam, LPARAM lParam)
{
	if(m_pInputBarDlg)
		m_pInputBarDlg->SetLastHistoryCode();
	PostMessage(UMSG_EDITCODE_INIT);
	
	return 0L;
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

//@SolomonGNF 	IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager, 3);
//@SolomonGNF 	if(!pKSLibMng) return 0;

//@SolomonGNF 	m_clrBackground = pKSLibMng->Lib_KSLink_GetColor(1);	// 바탕화면 배경
//@SolomonGNF 	ChangeSkinColor();
	return 0;
}

COLORREF CStdDialog::GetBkgrndColor()
{
	return winix_GetColor(clridx_BackGround);
}

void CStdDialog::ChangeSkinColor()
{
	if((m_nChartType != MINI_MULTI_CHART && m_nChartType != MINI_SINGLE_CHART) && m_pLeftBarDlg)
		m_pLeftBarDlg->ChangeSkinColor();

	m_pInputBarDlg->ChangeSkinColor();
	m_pExtInputBarDlg->ChangeSkinColor();
	m_xSplitter1.Invalidate();
	m_pDlgToolbar->Invalidate();

	m_pInputBarDlg->ShowWindow(SW_HIDE);
	m_pInputBarDlg->ShowWindow(SW_SHOW);
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
			pOLColor->clrOLBkgrnd = m_clrOLBkgrnd;
			pOLColor->clrOLOutLine = m_clrOLOutLine;
			return TRUE;
		}
		return FALSE;
	}
	
	return DEFAULT_BKGRND_COLOR;
}

LRESULT CStdDialog::OnFormGetPageSave(WPARAM wParam, LPARAM lParam)
{
	CString strKey = (LPCSTR)lParam;
	strKey.TrimRight();
	m_strSaveKey.Format(_T("%s"), strKey);

	// 설정파일 경로
//	m_strConfig = m_strUserDir + "\\" + m_strSaveKey + m_strScreenNo + "_" + ENVINFOFILE;
	//20080915 이문수 주석처리 >>
	//m_strConfig = m_strUserDir + "\\" + m_strScreenNo + m_strSaveKey +  ENVINFOFILE;
	//20080915 이문수 <<

	// Save 처리
//	SaveChart();

// --> [Edit]  강지원 2008/12/19
#ifdef	_LASTINFO_SAVE
//	FileCopyAndSaveAndDelete(LAST_HWND_COPY);
#endif
// <-- [Edit]  강지원 2008/12/19

	return 0L;
}

LRESULT CStdDialog::OnFormSetPageSave(WPARAM wParam, LPARAM lParam)
{
	CString strKey = (LPCSTR)lParam;
	strKey.TrimRight();
	m_strSaveKey.Format(_T("%s"), strKey);

	// 설정파일 경로
//	m_strConfig = m_strUserDir + "\\" + m_strSaveKey + m_strScreenNo + "_" + ENVINFOFILE;
	//20080915 이문수 주석처리>>
	//m_strConfig = m_strUserDir + "\\" + m_strScreenNo + m_strSaveKey +  ENVINFOFILE;
	//20080915 이문수 <<
//	m_strSaveKey.Empty();
	return 0L;
}

CString CStdDialog::GetLinkInfoData()
{
	CString strCode;
//	if(m_hViewInMain)
//		::SendMessage(m_hViewInMain, RMSG_GET_LINKINFODATA, (WPARAM)0, (LPARAM)&strCode);

	return strCode;
}

void CStdDialog::SaveChart(BOOL bType)
{
	CString strTemp;
	if(!m_bSmallScreenMode)
	{
		CRect rect;
		if(m_pLeftBarDlg)
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

	if ( bType )
	{
		m_ChartMng.SaveChart(m_strConfig);
		if(m_nChartType != MINI_MULTI_CHART && m_nChartType != MINI_SINGLE_CHART)
			m_pLeftBarDlg->SetConfig(FALSE);
		m_pInputBarDlg->SaveInputBar();
		if(m_nChartType == MULTI_CHART)
			m_pExtInputBarDlg->SaveExtInputBar();
	}
}

// --> [Edit]  이도영 2008/09/11
// 
void CStdDialog::GetOrderInfo()
{
	//20091103 SJ_KIM KTB
	return;


// 	IKSLibManager* pKSLibManager = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager, 3);
// 
// 	CString	m_strUserID		= pKSLibManager->Lib_KSLink_GetLoginInfo("USER_ID");		/*ID(17)		: 사용자 IB_No */
// 
// 	OAQ69901_INREC Inrec;	
// 	memset(&Inrec, 0x00, sizeof(Inrec));
// 	memcpy(Inrec.userid, (LPSTR)(LPCTSTR)m_strUserID, m_strUserID.GetLength());
// 
// 	Send2Server(TROAQ69901, (char*)&Inrec, sizeof(Inrec), FALSE, 0);
}
// <-- [Edit]  이도영 2008/09/11

// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
CString	CStdDialog::GetRealItemCode( const char *p_szItemCode, BOOL	bOrgCode /* = FALSE */)
{
	if (!p_szItemCode) return "";
	if (!*p_szItemCode) return "";
	CString strItemCode(p_szItemCode);
	if (strItemCode.GetAt(0) == '|') strItemCode = strItemCode.Mid( 3);

	int nFind = strItemCode.Find('.');
	if (nFind != -1 && bOrgCode)
	{
		CString strName, strMarket, strLimitSize, strActCode;
		BOOL	bCalcInfo(0);
//		short	nPrecision;
		strItemCode = strItemCode.Left(nFind);
//@SolomonGNF	IMasterDataManager* pDataManager = (IMasterDataManager*)AfxGetPctrInterface(UUID_IMasterDataManager, 3);
//@SolomonGNF	IMasterDataManager* pDataManager = (IMasterDataManager*)AfxGetPctrInterface(UUID_IMasterDataManager);
//@SolomonGNF	if( pDataManager->GetGItemInfo( strItemCode, strName, strMarket, strLimitSize, bCalcInfo, nPrecision, strActCode) )
//@SolomonGNF		strItemCode = strActCode;
	}

	return strItemCode;
}

// (2008/9/14 - Seung-Won, Bae) for Pair Tick
void CStdDialog::StartRequestPairTick( const char *p_strPairTickCode, int p_nRequestPairTickState)
{
	// (2008/11/8 - Seung-Won, Bae) for Sync Drawing within Pair Tick.
	m_ChartMng.GetpChartItemSelected()->ClearChartData( 0);

	m_nRequestPairTickState = p_nRequestPairTickState;
	m_strPairTickCode = "|B_" + GetRealItemCode( p_strPairTickCode);
	m_ChartMng.GetpChartItemSelected()->SetCodeForQuery( m_strPairTickCode, QUERY_STRFOFOREIGN_HOGA2, TRUE);

	// (2008/9/24 - Seung-Won, Bae) delete C2_OBVIOUSLY_BALANCE_CHART in Tick
	CChartItem *pChartItem = m_ChartMng.GetpChartItemSelected();
	if( pChartItem)
	{
		pChartItem->ResetChart();
		pChartItem->RemoveChart( "거래량차트");
		pChartItem->RemoveChart( "가격 이동평균");
		pChartItem->ChangePriceGraphDrawingData( TRUE, TRUE, TRUE, FALSE, 3, 0);
	}
}
void CStdDialog::EndRequestPairTick( void)
{
	m_nRequestPairTickState = E_RPTS_STOP;
}
void CStdDialog::RequestPairTick( const char *p_strPairTickCode, int p_nRequestPairTickState)
{
	m_nRequestPairTickState = p_nRequestPairTickState;
	m_bOrgAddMode = m_ChartMng.GetAddModeFromTab();
	m_ChartMng.SetAddModeFromTab( TRUE);
	m_ChartMng.RequestChartPairTickData( p_strPairTickCode, p_nRequestPairTickState != E_PRTS_ROTATE_PAIR);
}
void CStdDialog::ClearPairTick( const char *p_strRQ)
{
// --> [Edit]  강지원 2008/11/03	( 체결/호가일경우 TR코드를 변경해준다. 아마 여기서는 체결쪽으로만 흘러야 할거 같다. )
//	if ( m_ChartMng.GetpChartItemSelected()->m_nCheHoga == 1 )
//		m_ChartMng.GetpChartItemSelected()->SetCodeForQuery( m_strPairTickCode, QUERY_STRFOFOREIGN_HOGA2, TRUE);
//	else
//		m_ChartMng.GetpChartItemSelected()->SetCodeForQuery( m_strPairTickCode, QUERY_STRFOFOREIGN, TRUE);
// <-- [Edit]  강지원 2008/11/03

	EndRequestPairTick();
	CString strPairRQ( p_strRQ);
	if( strPairRQ.Right( 5) != "_PAIR") strPairRQ = strPairRQ + "_PAIR";
	m_ChartMng.GetpChartItemSelected()->RemoveAllGraphOfRQ( strPairRQ);
}
int CStdDialog::GetRequestPairTickState( void)
{
	return m_nRequestPairTickState;
}
void CStdDialog::SelectMainRQ( void)
{
	CString strMainRQ = m_ChartMng.GetpChartItemSelected()->GetSelectedRQ();
	if( strMainRQ.Right( 5) == "_PAIR")
	{
		strMainRQ = strMainRQ.Left( strMainRQ.GetLength() - 5);
		m_ChartMng.GetpChartItemSelected()->SetCurrentRQ( strMainRQ);
	}
}
void CStdDialog::SetRequestPairTickState( int p_nRequestPairTickState)
{
	m_nRequestPairTickState = p_nRequestPairTickState;
}


//	받은 데이터에서 OffSet 정보를 이용하여 데이터 값을 추출하는 함수
long CStdDialog::DataSeparation(CString strData, long nMask, long nTickSize, int nLog, int nPrecision) 
{
	int nVpoint = MAX_VPOINT;
	if(strData.GetLength() == 10 )//&& nLog == 1)
		nVpoint = MAX_VPOINT_PRICE;

	CString strInt = "", strFloat = "";
	long nResult(0);

	strData.TrimLeft();		strData.TrimRight();
	if ( strData.IsEmpty() )
		return 99999999;

	strFloat = strData.Right(nVpoint);
	strInt = strData.Left(strData.GetLength() - nVpoint );

//	strData.Format("%0.*f", nPrecision, atof(strData) / pow(10 , MAX_VPOINT) );

//	strData.Format("%s.%s", strInt, strFloat);
	if ( nMask <= 0 )
		nMask = 1;

	if ( nLog == 8 || nLog == 32 )
 	{
 		strData.Format("%s.%s", strInt, strFloat );
 		nResult = (long)(atof(strData) * (float)(nMask))/nTickSize;
 	}
	else
	{
/*
				if(strData == "") return 0;		
				if(strData.Left(1).CompareNoCase("0") == 0)
				{
					strData = strData.Mid(1, strData.GetLength() - 5);
				}
				return atof(strData);*/
		
		strData.Format("%0.*f", nPrecision, atof(strData) / pow(10 , nVpoint) );
//		nResult = (long)(atof(strData) * nMask);
		nResult = pow(10, nPrecision);
		double dValue = atof(strData) * (double)nResult;
		dValue += dValue * DBL_EPSILON;
		nResult = (long)(dValue);
		
//		if(nMask == 1)		
//		{
//			strFloat.Format("%f", atof(strData));
//			nResult = (long)(atof(strFloat));
//		}
//		else
//		{
////			m_nTickSize = 1;
//			strFloat.Format("%f", atof(strData));
//			float fGab = pow(10 , nPrecision*(-1))*5;
//			nResult = (long)((atof(strData) * (float)(nMask))+fGab);
//		}
	}

	return nResult;
}


//*******************************************************************/
/*! Function Name : SetFXMasterData
/*! Function      : FX 종목코드를 리스트에 세팅한다. 
/*! Param         : 
/*! Return        : void 
/*! Create        : 강지원 , 2008/08/06
/*! Comment       : 
//******************************************************************/
CString CStdDialog::GetTimeIndexInfo(int nIndex, int nType)
{
// --> [Edit]  강지원 2008/10/13
	CStringArray arrCode, arrName;

	CString strFileName = m_strRootPath + "\\Icss\\timesetup.ini";

	CString	strTemp, strSection, strKey, strData;
	CString	strTitle, strGmt, strSummerTime, strCityName;

	char	szItem[MAX_PATH];
	int nCount;

	strSection = "TIME";
	nCount = GetPrivateProfileInt (strSection, "COUNT", 0, strFileName); 

	if ( nCount < nIndex )
		return "";

	strKey.Format("%d", nIndex);
	memset(szItem, 0, MAX_PATH);
	GetPrivateProfileString(strSection, strKey, "", szItem, MAX_PATH, strFileName); 

	strTemp = szItem;
	int nCnt = strTemp.Find(",");
	strTitle= strTemp.Left(nCnt);	
	strTemp = strTemp.Mid(nCnt+1);	
	nCnt	= strTemp.Find(",");
	strGmt	= strTemp.Left(nCnt);

	// (2008/11/6 - Seung-Won, Bae) Check Index.
	if( strTemp.GetLength() < nCnt + 1) 
	{
		if ( nIndex == -1 && (nType != 1 || nType != 2) )
			return "원래시간대";
		
		return "";
		
	}

	strSummerTime	= strTemp.Mid(nCnt+1);	
	
	// 도시명만 취득한다.
	nCnt = strTitle.Find('(');	
	strCityName = strTitle.Left(nCnt);	

	switch(nType)
	{
		case 1 :		//	시간차
			{
				if ( strSummerTime == "1" )
					strData.Format( "%d", atoi(strGmt) + 1 );
				else
					strData.Format( "%d", atoi(strGmt) );
			}
			break;
		case 2 :		//	섬머타임 여부
			strData = strData;
			break;
		default :		//	타이틀
			{
				if ( nIndex == -1 && (nType != 1 || nType != 2) )
					return "원래시간대";
				else
					strData = strTitle;
			}
			break;
	}

	return strData;
}

//*******************************************************************/
/*! Function Name : TbondPrcLToF
/*! Function      : 미국채 표시변환 TB Price long -> float decimal
/*! Param         : double inprice, 
/*!                 int scale, 
/*!                 int outdec
/*! Return        : long  
/*! Create        : 안경환 , 2006/09/14
/*! Comment       : 
//******************************************************************/
CString CStdDialog::TbondPrcLToF( CString inprice, int scale, int outdec, int ValidPoint, BOOL bFlag)
{
	double	dTemp(0.0);

	CString strReturn = "";
	CString strFloat = "", strInt = "";

	//	실수부분 짤라내기	(예 : 102'31.5 => 315 )
	strFloat = inprice.Right(outdec);
	//	정수부분 짤라내기	(예 : 102'31.5 => 102 )
	strInt = inprice.Left(inprice.GetLength() - outdec);
	
	if ( scale == 8 )
	{
		dTemp = atof(strFloat);
		strReturn.Format("%f", (double)atoi(strInt) + (dTemp / scale));

//		strReturn.Remove('.');
	}
	else if ( scale == 32 )
	{
//		CString strTemp = "";
//		strTemp.Format("0.%s", strFloat);

		dTemp = atof(strFloat);
		if ( outdec == 4 )
			strReturn.Format("%f", (double)atoi(strInt) + (dTemp / scale / 100.0 ));
// --> [Edit]  김현수 2009/09/03
// 미국채 30년물 틱사이즈 변경에 따라 추가
		else if ( outdec == 2 )
			strReturn.Format("%f", (double)atoi(strInt) + (dTemp / scale ));
// <-- [Edit]  김현수 2009/09/03
		else
			strReturn.Format("%f", (double)atoi(strInt) + (dTemp / scale / 10.0 ));

//		strReturn.Remove('.');
	}
	else
	{
		if ( bFlag )
		{
			dTemp = (double)(atol(inprice) / (double)pow(10 , ValidPoint));
			strReturn.Format("%f", dTemp);
		}
		else
			strReturn = inprice;

		
		return strReturn;
	}
	
	return strReturn;
}

long CStdDialog::DataSeparation2(CString strData, long nMask, long nTickSize, int nLog, int nPrecision) 
{
	CString strInt = "", strFloat = "";
	long nResult(0);

	strData.TrimLeft();		strData.TrimRight();
	if ( strData.IsEmpty() )
		return 99999999;

	if ( nMask <= 0 )
		nMask = 1;

	if ( nLog == 8 )
	{
		nResult = (long)((atof(strData) * (float)(nMask))/nTickSize);
	}
	else if ( nLog == 32 )
	{
		nResult = (long)((atof(strData) * (float)(nMask))/nTickSize);
	}
	else
	{
//		strData.Format("%0.*f", nPrecision, atof(strData) / pow(10 , MAX_VPOINT) );
//		nResult = (long)((atof(strData) * pow(10, nPrecision)));
		nResult = atol(strData);
	}

	return nResult;
}

void CStdDialog::ConvertKoscomData(LPCSTR szTR, char* pData, long dwTRDateLen)
{
	CString strTRCode = szTR;
	strTRCode.Remove(' ');
	if (strTRCode != QUERY_STRFOFOREIGN)
		return;

	m_strOut = "";
	if (pData == NULL)
		return;

	EU_p0602_OutRec1	singOutput;
	memcpy(&singOutput, pData, sizeof(EU_p0602_OutRec1));

	CChartItem* pChartItem = m_ChartMng.GetpChartItemSelected();
	ST_SELCHART *pSelChart = pChartItem->m_lpSelChart;

	if (!pSelChart)
		pChartItem->GetSelChart(pChartItem->GetSelectedRQ(), pSelChart);

	CString strTerm = pChartItem->GetTickMinDayNumberRequested();
	char chTypeRequested = pChartItem->GetTypeRequested();
	if (pSelChart)
	{
		pSelChart->m_strKey = CString(singOutput.nkey, sizeof(singOutput.nkey));
		pSelChart->m_nOrgTimeIndex = atoi("22");
		if (chTypeRequested != pSelChart->m_cType)
			chTypeRequested = pSelChart->m_cType;

		CString strCodeType;
		char cRet = singOutput.codetype[0];
		strCodeType.Format(_T("%c"), cRet);
		if (cRet >= '0' && cRet <= '9')
		{
			int nType = atoi(strCodeType) + 1;
			strCodeType.Format(_T("%d"), nType);
		}
		SetCodeType(pSelChart->m_strItemCode, strCodeType);
	}

	ST_CHARTITEM_REALDATA* pChartItemRealData = pChartItem->GetChartItemRealData();
	if (pChartItemRealData)
	{
		//	부호
		CString	strSign = "", strChange = "", strDrate = "", strOpen = "", strHigh = "";
		CString	strLow = "", strPrice = "", strName = "", strStartDate= "", strEndDate = "";
		strSign = CString(singOutput.sign, sizeof(singOutput.sign));
		strChange = CString(singOutput.change, sizeof(singOutput.change));	//	전일대비
		strDrate = CString(singOutput.chrate, sizeof(singOutput.chrate));	//	등락율
		strOpen = CString(singOutput.open, sizeof(singOutput.open));		//	시가
		strHigh = CString(singOutput.high, sizeof(singOutput.high));		//	고가
		strLow = CString(singOutput.low, sizeof(singOutput.low));			//	저가
		strPrice = CString(singOutput.price, sizeof(singOutput.price));		//	종가
		strName = CString(singOutput.name, sizeof(singOutput.name));
		strName.TrimRight();
		strStartDate = CString(singOutput.startDate, sizeof(singOutput.startDate));
		strEndDate = CString(singOutput.endDate, sizeof(singOutput.endDate));

		//	부호
		//if (!strSign.IsEmpty())		pChartItemRealData->m_chSign = strSign.GetAt(0);
		//else						pChartItemRealData->m_chSign = ' ';
		strSign.TrimLeft();		strSign.TrimRight();
		if( strSign.IsEmpty() )
		{
			char cSign = strChange.GetAt(0);
			switch(cSign)
			{
			case ' ': strSign = "3"; break;
			case '+': strSign = "2"; break;
			case '-': strSign = "5"; break;
			default : strSign = "3"; break;
			}
		}
		pChartItemRealData->m_chSign = strSign.GetAt(0);


//		pChartItemRealData->m_strTime = strCodeTime;	//	거래소 현재시간
		pChartItemRealData->m_strChange = strChange;	//	전일대비
		pChartItemRealData->m_strChrate = strDrate;		//	등락율
		pChartItemRealData->m_strOpen = strOpen;		//	시가
		pChartItemRealData->m_strHigh = strHigh;		//	고가
		pChartItemRealData->m_strLow = strLow;			//	저가
		pChartItemRealData->m_strPrice = strPrice;		//	종가
		pChartItemRealData->m_strItemName = strName;	//	품목명
		pChartItemRealData->m_strTimeIndex = "22";

		pChartItemRealData->m_nRealType = STOCK_SISE_PID;

		pChartItemRealData->m_nStartDate = atol(strStartDate);
		pChartItemRealData->m_nEndDate = atol(strEndDate);
	}

	m_strOut.Format(_T("%s"), (LPTSTR)(char*)(pData+sizeof(EU_p0602_OutRec1)));
}

/*
void CStdDialog::ConvertKoscomData(LPCSTR szTR, char* pData, long dwTRDateLen)
{
	TRACE("dwTRDateLen=%d\n", dwTRDateLen);

	CString strTRCode = szTR;
	strTRCode.Remove(' ');

	if(strTRCode == QUERY_STRFOFOREIGN_HOGA2)
	{
		m_strOut = "";
		int nDataCnt(0);

		if ( pData == NULL )
		{
			m_strOut = "";
			nDataCnt = 0;
		}

		CString		strData = "", strTemp = "", strPipLowest = "", strOut = "";
		char		szData[1024];

		int nTemp(0);
		char cTemp[128];

		HCQ03011_OUT_KEY		singOutput;
		HCQ03011_OUT			output;
		o56501_OUT_HEADER		sto56051_Out_Head;	
		memset(&sto56051_Out_Head, 0x00, o56501_OUT_SZ);
		memcpy(&sto56051_Out_Head, pData + HCQ03011_OUT_KEY_SZ, o56501_OUT_SZ);

		memset(cTemp, 0x00, 128);
		memcpy(cTemp, sto56051_Out_Head.count, sizeof(sto56051_Out_Head.count));
		nDataCnt = atoi(cTemp);

//		nDataCnt = (dwTRDateLen - HCQ03011_OUT_KEY_SZ) / HCQ03011_OUT_SZ;		//	데이터의 갯수를 구한다.

		if ( dwTRDateLen < HCQ03011_OUT_KEY_SZ )
		{
			m_strOut = "";
			nDataCnt = 0;
		}

		if ( nDataCnt <= 0 || dwTRDateLen <= 0 )			//	받은 데이터가 레코드의 길이보다 작을경우 오류이므로 return
		{
			m_strOut = "";
			nDataCnt = 0;
		}

		memset(&singOutput, 0x00, HCQ03011_OUT_KEY_SZ);
		if ( dwTRDateLen >= HCQ03011_OUT_KEY_SZ )
			memcpy(&singOutput, pData, HCQ03011_OUT_KEY_SZ);	//	KeyValue와 데이터 갯수를 짤라낸다.

		memset(szData, 0x00, sizeof(szData));
		memset(&output, 0x00, HCQ03011_OUT_SZ);

		if ( nDataCnt > 0 )
		{
			memcpy(szData, pData + HCQ03011_OUT_KEY_SZ, HCQ03011_OUT_SZ);	//	KeyValue와 데이터 갯수를 짤라낸다.
			memcpy(&output, pData + HCQ03011_OUT_KEY_SZ, HCQ03011_OUT_SZ);	//	KeyValue와 데이터 갯수를 짤라낸다.
		}

		strData = szData;

		CString			strTickSize = "1";
		CString			strTickValue, strPriceInfo;

		int				nLog(1);
		int				nLogDispSize(0);
		int				nPrecision(0);
		int				ValidPoint(0);
		int				nMask(1);
		long			nTickSize(1);

		// (2008/9/14 - Seung-Won, Bae) for Pair Tick
		char *szPrice = NULL;
		CString strItemCode;
		CChartItem* pChartItem = m_ChartMng.GetpChartItemSelected();
		if( pChartItem ) if( pChartItem->m_lpSelChart)
			strItemCode = pChartItem->m_lpSelChart->m_strItemCode;
		BOOL bAskItem = FALSE;
		if( !strItemCode.IsEmpty()) if( strItemCode.GetAt( 0) == '|') if( strItemCode.GetAt( 1) == 'S') bAskItem = TRUE;


		//	넥스트 키값
		ST_SELCHART *pSelChart = pChartItem->m_lpSelChart;

		if ( !pSelChart )
			pChartItem->GetSelChart(pChartItem->GetSelectedRQ(), pSelChart);

		if ( pSelChart )
			pSelChart->m_strKey = CString(sto56051_Out_Head.keyvalue, sizeof(sto56051_Out_Head.keyvalue));

		pChartItem->m_strNextKey = CString(sto56051_Out_Head.keyvalue, sizeof(sto56051_Out_Head.keyvalue));
		char chTypeRequested = pChartItem->GetTypeRequested();

		strItemCode.TrimLeft();		strItemCode.TrimRight();
		//@SolomonGNF IMasterDataManager* pDataManager = (IMasterDataManager*)AfxGetPctrInterface(UUID_IMasterDataManager, 3);
		IMasterDataManager* pDataManager = (IMasterDataManager*)AfxGetPctrInterface(UUID_IMasterDataManager);
		//@SolomonGNF if ( !pDataManager->GetGJongmokInfo( CStdDialog::GetRealItemCode( ( LPCTSTR)strItemCode, TRUE), nLog, nLogDispSize, nPrecision, strTickSize, strTickValue, strPriceInfo ) )
		if(TRUE)
		{
			nLog			= 1;
			nLogDispSize	= 0;
			nPrecision	= 0;

			ValidPoint	= 0;
			nMask			= 0;

			nTickSize		= 1;
		}
		else
		{
			strTickSize.TrimLeft(); strTickSize.TrimRight();
			CString strTmp = strTickSize.Right(1);

			while( strTmp == "0" )
			{
				strTickSize = strTickSize.Left(strTickSize.GetLength()-1);
				strTmp = strTickSize.Right(1);
			};

			nMask = strTickSize.GetLength() - 2;

			if ( nMask < 0 )
				nMask = 0;

			nMask = pow(10, nMask);
			ValidPoint = nPrecision;

			nTickSize = (long)(atof(strTickSize)*nMask);

		}

		CString	strTickSize2 = "";
		int nPoint(0);
		if ( nLog == 1 )	//	10진법일경우 기존 로직대로 태워달라는 요청때문에 8,32 진법과는 구분해준다.
		{
			nPoint = nPrecision;
			strTickSize.Format("%0.*f", nPoint, 1 / pow(10, nPoint));
			strTickSize2.Format("%0.*f", nPoint, 1 / pow(10, nPoint - 1));		//	한틱의 사이즈... 
		}
		else if ( nLog == 8 )
		{
			//	차트에서 100을 곱해서 달라고 함... 8진법일경우...
			strTickSize2.Format("%0.8f", atof(strTickSize) * 100 );
		}
		else if ( nLog == 32 )
		{
			strTickSize2.Format("%0.8f", atof(strTickSize)  );
		}		


		for(int i = nDataCnt - 1; i >= 0 ; i--)
		{
			memset(&output, 0x00, o56501_OUT_SZ);
			memcpy(&output, pData + ( i * HCQ03011_OUT_SZ + o56501_OUT_SZ ) + HCQ03011_OUT_KEY_SZ, HCQ03011_OUT_SZ);	//	KeyValue와 데이터 갯수를 짤라낸다.

			if ( chTypeRequested == TICK_DATA_CHART )
			{
				//	시간
				memset(cTemp, 0x00, 128);
				memcpy(cTemp, output.date + 6, 8 );
				strTemp = cTemp;
				strTemp.Remove('.');
				strData.Format("%8s", strTemp);
				strOut += strData;
			}


			if ( chTypeRequested == TICK_DATA_CHART )		//	틱차트에서는 시고저종을 같은값으로 설정
			{
				if ( !bAskItem )
				{
					//	매수호가
					memset(cTemp, 0x00, 128);
					memcpy(cTemp, output.mshoga_d, sizeof(output.mshoga_d));
					strTemp = cTemp;
					strTemp.Format("%d", DataSeparation(strTemp, nMask, nTickSize, nLog, nPrecision));
					strData.Format("%10s", strTemp);

					strOut += strData;
					strOut += strData;
					strOut += strData;
					strOut += strData;
				}
				else
				{
					//	매도호가
					memset(cTemp, 0x00, 128);
					memcpy(cTemp, output.mdhoga_d, sizeof(output.mdhoga_d));
					strTemp = cTemp;
					strTemp.Format("%d", DataSeparation(strTemp, nMask, nTickSize, nLog, nPrecision));
					strData.Format("%10s", strTemp);

					strOut += strData;
					strOut += strData;
					strOut += strData;
					strOut += strData;
				}

			}

			
			if ( chTypeRequested == TICK_DATA_CHART )
			{
				if ( !bAskItem )
				{
					//	매수잔량
					memset(cTemp, 0x00, 128);
					memcpy(cTemp, output.mshorem, sizeof(output.mshorem));
					strTemp.Format("%.0f", atof(cTemp));
					strData.Format("%10s", strTemp);
					strOut += strData;
				}
				else
				{
					//	매도잔량
					memset(cTemp, 0x00, 128);
					memcpy(cTemp, output.mdhorem, sizeof(output.mdhorem));
					strTemp.Format("%.0f", atof(cTemp));
					strData.Format("%10s", strTemp);
					strOut += strData;
				}
			}

			//	권리락 ( 일단 우리 서버데이터에는 없는데.. 여기에서 차트 넘겨주는 데이터에서 필요함... 나중에 조정해야할거 같음 )
//			strOut += "0";
		}

		//  진법 적용되는 품목에서는 리얼데이터의 소수점 자리수를 -1로 세팅해달라는 요청
//		if ( nLog != 1 )
			ValidPoint = -1;

		CString strBojoyMsg = "";

		//	기준가
		CString strPrePrice = "";
		strPrePrice = CString(singOutput.jsprice, sizeof(singOutput.jsprice));
		strPrePrice = TbondPrcLToF(strPrePrice, nLog, nLogDispSize, ValidPoint, TRUE);
//		strPrePrice.Format("%ld", DataSeparation2(strPrePrice, nMask, nTickSize, nLog, nLogDispSize));
		strPrePrice.TrimLeft();
		//	시간대
		CString strTimeIndex = "", strTimeName = "";
		//20091117 SJ_KIM 01로 세팅 차후 수정.
	//	strTimeName = strTemp = CString(singOutput.timeindex, sizeof(singOutput.timeindex));
		strTimeName = strTemp = "01";
		strTimeIndex.Format("%s", GetTimeIndexInfo(atoi(strTemp), 1));
		//	장시작
		CString	strMarketStart = "";
		strMarketStart = CString(singOutput.estime, sizeof(singOutput.estime));
		//	장마감
		CString	strMarketEnd = "";
		strMarketEnd = CString(singOutput.eetime, sizeof(singOutput.eetime));
		//	품목의 현재시간
		CString	strCodeTime = "";
		strCodeTime = CString(singOutput.date, sizeof(singOutput.date) + sizeof(singOutput.time));
		//	영업일 추가
		CString	strBDay = "";
		strBDay = CString(singOutput.bday, sizeof(singOutput.bday));

		CString strName = "";
		ST_CHARTITEM_REALDATA* pChartItemRealData = pChartItem->GetChartItemRealData();
		if( pChartItemRealData )
		{
			//	부호
			CString	strSign = "";
			strSign = CString(singOutput.sign, sizeof(singOutput.sign));
			//	전일대비
			CString	strChange = "";
			strChange = CString(singOutput.change, sizeof(singOutput.change));
			strChange = TbondPrcLToF(strChange, nLog, nLogDispSize, ValidPoint, FALSE);
			strChange.Format("%ld", DataSeparation2(strChange, nMask, nTickSize, nLog, nLogDispSize));
			strChange.TrimLeft();
			//	등락율
			CString	strDrate = "";
			strDrate = CString(singOutput.drate, sizeof(singOutput.drate));
			strDrate.TrimLeft();
			//	시가
			CString	strOpen = "";
			strOpen = CString(singOutput.open, sizeof(singOutput.open));
			strOpen = TbondPrcLToF(strOpen, nLog, nLogDispSize, ValidPoint, FALSE);
			strOpen.Format("%ld", DataSeparation2(strOpen, nMask, nTickSize, nLog, nLogDispSize));
			strOpen.TrimLeft();
			//	고가
			CString	strHigh = "";
			strHigh = CString(singOutput.high, sizeof(singOutput.high));
			strHigh = TbondPrcLToF(strHigh, nLog, nLogDispSize, ValidPoint, FALSE);
			strHigh.Format("%ld", DataSeparation2(strHigh, nMask, nTickSize, nLog, nLogDispSize));
			strHigh.TrimLeft();
			//	저가
			CString	strLow = "";
			strLow = CString(singOutput.low, sizeof(singOutput.low));
			strLow = TbondPrcLToF(strLow, nLog, nLogDispSize, ValidPoint, FALSE);
			strLow.Format("%ld", DataSeparation2(strLow, nMask, nTickSize, nLog, nLogDispSize));
			strLow.TrimLeft();
			//	종가
			CString	strPrice = "";
			strPrice = CString(singOutput.price, sizeof(singOutput.price));
			strPrice = TbondPrcLToF(strPrice, nLog, nLogDispSize, ValidPoint, FALSE);
			strPrice.Format("%ld", DataSeparation2(strPrice, nMask, nTickSize, nLog, nLogDispSize));
			strPrice.TrimLeft();

			strName = CString(singOutput.jmname, sizeof(singOutput.jmname));
			strName.TrimLeft();	strName.TrimRight();

			//	부호
			if ( !strSign.IsEmpty() )
				pChartItemRealData->m_chSign = strSign.GetAt(0);
			else
				pChartItemRealData->m_chSign = ' ';
			//	시간
//			int nLen = strCodeTime.GetLength();
//			if ( nLen >= 6 )
//				pChartItemRealData->m_strTime.Format("%s:%s:%s", strCodeTime.Mid(nLen-6, 2), strCodeTime.Mid(nLen-4, 2), strCodeTime.Mid(nLen-2, 2));
// --> [Edit]  강지원 2008/11/01
			//	일단 거래소 시간으로 보여준다.. 체결시간은 TR 수정해야함
			pChartItemRealData->m_strTime = strCodeTime;		//	거래소 현재시간
//			pChartItemRealData->m_strTime = strCheTime;			//	체결시간
// <-- [Edit]  강지원 2008/11/01
//			pChartItemRealData->m_strTimeGab = strTimeIndex;
			//	전일대비
			pChartItemRealData->m_strChange = strChange;
			//	등락율
			pChartItemRealData->m_strChrate = strDrate;
			//	시가
			pChartItemRealData->m_strOpen = strOpen;
			//	고가
			pChartItemRealData->m_strHigh = strHigh;
			//	저가
			pChartItemRealData->m_strLow = strLow;
			//	종가
			pChartItemRealData->m_strPrice = strPrice;
			//	품목명
			pChartItemRealData->m_strItemName = strName;
		//	pChartItemRealData->m_strItemName = strName = pDataManager->GetCodeName( CStdDialog::GetRealItemCode( ( LPCTSTR)strItemCode ) );

//			long lResult(0);
//			CTabCodeSearch *pTabCodeSearch = (CTabCodeSearch*)m_pLeftBarDlg->m_tabctrl.GetWndCodeSearch();
//			if(pTabCodeSearch)
//			{
//				lResult = pTabCodeSearch->GetCurListIndex();
//			}
//
//			if (lResult > 0 )
//			{
//////				long lResult = pChartItem->SendMessage(UDM_CHANGE_TIMEDIFF, 0, 1);
//////				if ( lResult < 1 )
//////				{
//////					SetTabTimeIndex(atol(strTimeName));
//////				}
////				strTimeName.TrimLeft();
////				pChartItemRealData->m_strTimeName = GetTimeIndexInfo(lResult, 0);
////				SetTabTimeIndex(lResult);
////				ChangeTimeSet(lResult);
//				pChartItemRealData->m_strTimeIndex = strTimeName;
//////				m_ChartMng.m_strTimeSet = strTimeName;
//			}
////			else
			{
//				long lResult = pChartItem->SendMessage(UDM_CHANGE_TIMEDIFF, 0, 1);
//				if ( lResult < 1 )
//				{
//					SetTabTimeIndex(atol(m_ChartMng.m_strTimeSet));
//				}
//				pChartItemRealData->m_strTimeName = GetTimeIndexInfo(atoi(m_ChartMng.m_strTimeSet), 0);
				pChartItemRealData->m_strTimeIndex = strTimeName;
			}

			pChartItemRealData->m_nRealType = 0; //@SolomonGNF STOCK_HOGA0_PID;

			//	처음과 마지막 시간을 오늘 날짜로 설정한다. 
			CString strDateToday;
			CTime t = CTime::GetCurrentTime();
			strDateToday = t.Format("%Y%m%d");

			pChartItemRealData->m_nStartDate = atol(strDateToday);
			pChartItemRealData->m_nEndDate = atol(strDateToday);

		}
		else
		{
#ifdef _DEBUG
			AfxMessageBox("이리로 들어오는건 뭘까??");
#endif
		}

		CString strNext = "E";
		BOOL bNext = pChartItem->GetNext();
		if ( bNext )
			strNext = "A";
		else
			strNext = "E";

		if ( chTypeRequested == TICK_DATA_CHART )
		{
			if ( bNext )
			{
				strBojoyMsg.Format("UM=%s@UD=5@RDATET=DDHHMMSS@TDC=%s:시가,|%s:고가,|%s:저가,|%s:종가,|1:기본거래량,@RDC=%s:%d:시가,|%s:%d:고가,|%s:%d:저가,|%s:%d:종가,|1:기본거래량,@REALTIMET=HHMMSS@TIMEDIFF=%s@@MARKETTIME=%s|%s@DATATIME=%s@CODEANDNAME=%s:%s@MARKETDAY=%s@RESETUSEREAL=%d@HBPRICE=%s@", 
									strNext, strTickSize, strTickSize, strTickSize, strTickSize, 
									strTickSize, ValidPoint, strTickSize, ValidPoint, strTickSize, ValidPoint, strTickSize, ValidPoint,
									strTimeIndex, strMarketStart, strMarketEnd, strCodeTime, strItemCode, strName, strBDay, GetRealCheck(), strTickSize2);
			}
			else
			{
				strBojoyMsg.Format("UM=%s@UD=5@RDATET=DDHHMMSS@TDC=%s:시가,|%s:고가,|%s:저가,|%s:종가,|1:기본거래량,@RDC=%s:%d:시가,|%s:%d:고가,|%s:%d:저가,|%s:%d:종가,|1:기본거래량,@REALTIMET=HHMMSS@TIMEDIFF=%s@PREVPRICE=종가:%s@MARKETTIME=%s|%s@DATATIME=%s@CODEANDNAME=%s:%s@MARKETDAY=%s@RESETUSEREAL=%d@HBPRICE=%s@", 
								strNext, strTickSize, strTickSize, strTickSize, strTickSize, 
								strTickSize, ValidPoint, strTickSize, ValidPoint, strTickSize, ValidPoint, strTickSize, ValidPoint,
								strTimeIndex, strPrePrice, strMarketStart, strMarketEnd, strCodeTime, strItemCode, strName, strBDay, GetRealCheck(), strTickSize2);
			}
//			strBojoyMsg = "UM=E@UD=5@RDATET=DDHHMMSS@REALTIMET=YYYYMMDDHHMMSS@";
		}
		else
		{
			return;
		}

//		int n = strBojoyMsg.GetLength();

		CString strBojoMsg;
		strBojoMsg.Format("0496%-496s", strBojoyMsg);
		strTemp.Format("%05d", nDataCnt);

//		strBojoyMsg = strTemp + strBojoMsg;

		m_strOut = strTemp + strBojoMsg + strOut;


#ifdef _WRITELOG
		// Log - ojtaso (20080107)
		WriteToStringLog("해외품목 호가데이터 서버에서 받음::End", strCode, strCode.GetLength());
#endif


	}
	else if(strTRCode == QUERY_STOCK_HOGA)
	{
		if ( pData == NULL )
		{
			return;
		}

		if ( dwTRDateLen < HCQ01112_OUT_SZ )
		{
			return;
		}

		char		szData[1024];
		memset(szData, 0x00, sizeof(szData));
		memcpy(szData, pData, HCQ01112_OUT_SZ);	//	KeyValue와 데이터 갯수를 짤라낸다.

		m_strOut = szData;

		int n = m_strOut.GetLength();

#ifdef _WRITELOG
		// Log - ojtaso (20080107)
		WriteToStringLog("해외품목 호가데이터 서버에서 받음::End", strCode, strCode.GetLength());
#endif


	}
}
*/

BOOL CStdDialog::GetRealCheck()
{
	return m_pExtInputBarDlg->GetRealUpdateCheck();
}

void CStdDialog::SetTabTimeIndex(int nIndex)
{
	CTabCodeSearch *pTabCodeSearch = (CTabCodeSearch*)m_pLeftBarDlg->m_tabctrl.GetWndCodeSearch();
	if(pTabCodeSearch)
	{
		pTabCodeSearch->SetCurListIndex( nIndex );
	}	
}

int CStdDialog::GetSummerTimeInfo(int nStart, int nEnd) 
{
	//	시카고 시간은 nStart 1 nEnd 7로 입력해야 한다고 해서... 일단 고정으로 박는다. 
	nStart	= 1;
	nEnd	= 7;

	CTime	tToday = CTime::GetCurrentTime();
	CTime	tStart, tEnd;
	CString strStart, strEnd;

	tToday.GetCurrentTime();
	int nYear = tToday.GetYear();

	//	섬머타임 시작 날짜
	switch(nStart)
	{
// --> [Edit]  박정일 2007/2/5 
// 	case 1 :	//	4월 첫번째 일요일을 구한다.
// 		{
// 			CTime	t(tToday.GetYear(), 4, 1, 1, 0, 0);
// 			int nWeek = 8 - t.GetDayOfWeek();
// 
// 			CTimeSpan cts = CTimeSpan(nWeek, 0, 0, 0);     // 1일
// 			tStart = t + cts;
// 		}
// 		break;
	case 1:		//	3월 둘째주 일요일을 구한다.
		{
			
			//	( 예외처리... 3월보다 현재월이 클경우 1년후 3월의 날짜를 구한다)
			CTime	t(nYear, 3, 1+7, 1, 0, 0);
			int nWeek = 8 - t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek, 0, 0, 0);     // 1일
			tStart = t + cts;
		}
		break;

// <-- [Edit]  박정일 2007/2/5 
	case 2 :	//	3월 마지막 일요일을 구한다.
		{
			CTime	t(tToday.GetYear(), 3, 31, 1, 0, 0);
			int nWeek = t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek-1, 0, 0, 0);     // 1일
			tStart = t - cts;
		}
		break;
	case 3 :	//	4월 마지막 일요일을 구한다.
		{
			CTime	t(tToday.GetYear(), 4, 30, 1, 0, 0);
			int nWeek = t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek-1, 0, 0, 0);     // 1일
			tStart = t - cts;
		}
		break;
	case 4 :	//	10월 둘째주 일요일을 구한다. 
		{
			//	( 예외처리... 10월보다 현재월이 작을경우 1년전 10월의 날짜를 구한다)
			if ( tToday.GetMonth() < 10 )
			{
				nYear -= 1;
			}
			CTime	t(nYear, 10, 1+7, 1, 0, 0);
			int nWeek = 8 - t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek, 0, 0, 0);     // 1일
			tStart = t + cts;
		}
		break;
	case 5 :	//	4월 1일을 구한다.
		{
			CTime	t(tToday.GetYear(), 4, 1, 1, 0, 0);
			tStart = t;
		}
		break;
	case 6 :	//	5월 1일을 구한다.
		{
			CTime	t(tToday.GetYear(), 5, 1, 1, 0, 0);
			tStart = t;
		}
		break;
	case 7 :	//	10월 마지막주 일요일을 구한다.
		{
			//	( 예외처리... 10월보다 현재월이 작을경우 1년전 10월의 날짜를 구한다)
 			if ( tToday.GetMonth() < 10 )
 			{
 				nYear -= 1;
 			}
			CTime	t(nYear, 10, 31, 1, 0, 0);
			int nWeek = t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek-1, 0, 0, 0);     // 1일
			tStart = t - cts;	
		}
		break;
	case 8 :	//	10월 첫번째 일요일을 구한다.
		{
			CTime	t(tToday.GetYear(), 10, 1, 1, 0, 0);
			int nWeek = 8 - t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek, 0, 0, 0);     // 1일
			tStart = t + cts;
		}
		break;
	default :
		{
			return 0;
		}
		break;
	}

	//	섬머타임 끝나는 날짜
	switch(nEnd)
	{
	case 1 :	//	10월 마지막 일요일을 구한다.
		{
			CTime	t(tToday.GetYear(), 10, 31, 1, 0, 0);
			int nWeek = t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek-1, 0, 0, 0);     // 1일
			tEnd = t - cts;
		}
		break;
	case 2 :	//	2월 둘째주 일요일을 구한다.
		{
			//	( 예외처리... 2월보다 현재월이 클경우 1년후 2월의 날짜를 구한다)
			nYear += 1;
			CTime	t(nYear, 2, 1+7, 1, 0, 0);
			int nWeek = 8 - t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek, 0, 0, 0);     // 1일
			tEnd = t + cts;
		}
		break;
	case 3 :	//	3월 둘째주 일요일을 구한다.
		{
			//	( 예외처리... 3월보다 현재월이 클경우 1년후 3월의 날짜를 구한다)
				nYear += 1;
			CTime	t(nYear, 3, 1+7, 1, 0, 0);
			int nWeek = 8 - t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek, 0, 0, 0);     // 1일
			tEnd = t + cts;
		}
		break;
	case 4 :	//	10월 1일을 구한다. 
		{
			CTime	t(tToday.GetYear(), 10, 1, 1, 0, 0);
			tEnd = t;
		}
		break;
	case 5 :	//	3월 마지막주 일요일을 구한다.
		{
			//	( 예외처리... 3월보다 현재월이 클경우 1년후 3월의 날짜를 구한다)
			nYear += 1;
			CTime	t(nYear, 3, 31, 1, 0, 0);
			int nWeek = t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek-1, 0, 0, 0);     // 1일
			tEnd = t - cts;
		}
		break;
	case 6 :	//	3월 셋째주 일요일을 구한다.
		{
			//	( 예외처리... 3월보다 현재월이 클경우 1년후 3월의 날짜를 구한다)

			nYear += 1;
			CTime	t(nYear, 3, 1+7+7, 1, 0, 0);
			int nWeek = 8 - t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek, 0, 0, 0);     // 1일
			tEnd = t + cts;
		}
		break;
// --> [Edit]  박정일 2007/2/5 	
	case 7 :	//	11월 첫째주 일요일을 구한다.
		{
			CTime	t(tToday.GetYear(), 11, 1, 1, 0, 0);
			int nWeek = 8 - t.GetDayOfWeek();

			CTimeSpan cts = CTimeSpan(nWeek, 0, 0, 0);     // 1일
			tEnd = t + cts;
		}
		break;
// <-- [Edit]  박정일 2007/2/5 

	default :
		{
			return 0;
		}
		break;
	}

//	strStart.Format("%d년 %d월 %d일", tStart.GetYear(), tStart.GetMonth(), tStart.GetDay());
//	strEnd.Format("%d년 %d월 %d일", tEnd.GetYear(), tEnd.GetMonth(), tEnd.GetDay());
//	ASSERT (tStart < tEnd);

	if ( tToday > tStart && tToday < tEnd )
		return 1;
	else
		return 0;

}

CString CStdDialog::Gl_Format32String(long nTBValue, int scale, int outdec, int indec, UINT uMask) 
{

	CString strValue = "", strInt = "", strMid = "", strFloat = "", strResult = "";
	long	nValue(0);

	if ( scale < 0 || outdec < 0 || indec < 0 )
	{
		strValue.Format("%d", nTBValue);
		return strValue;
	}

	//	값이 0일경우에는 Format에 따라 계산하지 않고 리턴한다.
	if ( nTBValue == 0 )
	{
		CString strZero = "00000000000000000000";
		if ( scale == 1 )
			return "0";
		else if ( scale == 8 || scale == 32 )
		{
			if ( indec == 0 )
				strValue.Format("0'%s", strZero.Left(outdec));
			else
				strValue.Format("0'%s.%s", strZero.Left(outdec - indec), strZero.Left(indec) );
		}
	}

	int nMask = uMask;
	if( nMask == 0 )
		nMask = 1;

	BOOL	bFlag = FALSE;
	if ( nTBValue < 0 )
	{
		nTBValue = abs(nTBValue);
		bFlag = TRUE;
	}

	double dValue = (double)nTBValue/(double)nMask;

	nValue = FtoTbondPrcL( dValue, scale, outdec );

//	if ( scale == 1 )		//	10진법일경우
//	{
//		strValue.Format("%0.*f", indec, dValue);
//		return strValue;
//	}

//	nTBValue = nTBValue/nMask;

	//	32진법일경우 포맷처리
	if ( scale == 32 || scale == 8 ) 
	{

		int nMidSize = 2;
		strValue.Format("%d", nValue);

		if ( strValue.GetLength() < outdec )
		{
			int nCnt = strValue.GetLength();
			for ( int i = 0 ; i < outdec - nCnt; i++ )
				strValue = "0" + strValue;
		}
			
		//	실수부분 짤라내기	(예 : 102'31.5 => 5 )
		strFloat = strValue.Right(indec);
		//	중간부분 짤라내기	(예 : 102'31.5 => 31 )
		strMid = strValue.Mid(strValue.GetLength() - outdec, outdec - indec);
		//	정수부분 짤라내기	(예 : 102'31.5 => 102 )
		strInt = strValue.Left(strValue.GetLength() - outdec);

		//	정수부분이 없을경우 0으로 대체한다.
		if ( strInt.IsEmpty() )
			strInt = "0";

		//	실수부분이 없을경우 . 이하는 표시 안한다.
		if ( strFloat.IsEmpty() )
			strValue.Format("%s'%s", strInt, strMid);
		else
			strValue.Format("%s'%s.%s", strInt, strMid, strFloat);

//		return strValue;
	}
	else	//10
	{
		strValue.Format("%d", nTBValue);

		if ( strValue.GetLength() < indec/*outdec*/ )
		{
			int nCnt = strValue.GetLength();
			for ( int i = 0 ; i < indec/*outdec*/ - nCnt; i++ )
				strValue = "0" + strValue;
		}
			
		//	실수부분 짤라내기	(예 : 102'31.5 => 5 )
		strFloat = strValue.Right(indec);
		//	정수부분 짤라내기	(예 : 102'31.5 => 102 )
		strInt = strValue.Left(strValue.GetLength() - indec/*outdec*/);

		//	정수부분이 없을경우 0으로 대체한다.
		if ( strInt.IsEmpty() )
			strInt = "0";
		//	실수부분이 없을경우 . 이하는 표시 안한다.
		if ( strFloat.IsEmpty() )
			strValue.Format("%s", strInt);
		else
			strValue.Format("%s.%s", strInt, strFloat/*strMid*/);

//		return strValue;
	}

//	strValue.Format("%d", nTBValue);
	if ( bFlag )
		strResult.Format("-%s", strValue);
	else
		strResult.Format("%s", strValue);

	return strResult;
}

long CStdDialog::FtoTbondPrcL( double inprice, int scale, int outdec )
{
	int		i_intnum;
	char	s_float[10], s_temp[20];
	
	if( scale == 8 )
	{
		i_intnum = (int)floor( inprice );
		sprintf( s_float, "%3.1f", (inprice-i_intnum)*scale );
	}
	else if ( scale == 32 )
	{
		i_intnum = (int)floor( inprice );
		if ( outdec == 4 )
			sprintf( s_float, "%06.1f", (inprice-i_intnum)*scale*100.0 );
// --> [Edit]  김현수 2009/09/03
// 미국채 30년물 틱사이즈 변경에 따라 추가
		else if ( outdec == 2 )
			sprintf( s_float, "%04.1f", (inprice-i_intnum)*scale );
// <-- [Edit]  김현수 2009/09/03
		else
			sprintf( s_float, "%05.1f", (inprice-i_intnum)*scale*10.0 );
	}
	else
	{
		i_intnum = (int)floor( inprice );
		sprintf( s_float, "%05.1f", (inprice-i_intnum)*scale*10.0 );
	}

	
	sprintf( s_temp, "%d%.*s", i_intnum, outdec, s_float );
	return atol( s_temp );
}


void CStdDialog::LastInfoSave()
{
#ifdef	_LASTINFO_SAVE
//	// 마지막 저장파일일경우 다른 화면번호로 저장하기위해서 추가
//	CString strScreenNo = m_strScreenNo;
//
//	long	nHwnd(0);
//	CString strTempName = "";
//	CString strOrgName = "";
//	CString strTemp = m_strConfig;
//	CString strUserPath = "";
//
//	int pos = m_strConfig.ReverseFind('\\'); 
//	if(pos > -1)
//		strUserPath = m_strConfig.Left(pos); 
//	else
//		strUserPath = m_strUserDir;
//
//	strTemp.Replace(strScreenNo, "Che_" + strScreenNo);
//	strTemp.Replace(".dat", "");
//	strOrgName = strTemp;
//	nHwnd = (long)GetSafeHwnd();
//
////	int pos = strExePath.ReverseFind('\\'); 
////	if(pos > -1)
////	{
////		strExePath = strExePath.Left(pos); 
////	}
//
//	strTempName.Format("%s%s.tmp", strTemp, m_strHwndKey);
//	strOrgName.Format("%s%s.mct", strTemp, m_strSaveKey);
//
//	if ( !m_strSaveKey.IsEmpty() )
//	{
//		CFileFind ff;
//		BOOL bResult = ff.FindFile(strTempName);
//		if(bResult)
//		{
//			::CopyFile(strTempName, strOrgName, TRUE);
//		}
//	}
//
//	::DeleteFile(strTempName);
	
#endif
}

void CStdDialog::FileCopyAndSaveAndDelete(int nType)
{
#ifdef	_LASTINFO_SAVE
	CString strUserPath = "";

	if ( m_strConfig.IsEmpty() )
		return; 

	int pos = m_strConfig.ReverseFind('\\'); 
	if(pos > -1)
		strUserPath = m_strConfig.Left(pos); 
	else
		return;

	if ( nType == DELETE_FILE )		//	사용했던 파일 지우기
	{
// [Edit]  김현수 2009/08/26
//		CString sAllFileName = strUserPath + "\\" + "*" + m_strHwndKey + "*";
		CString sAllFileName = strUserPath + "\\" + "*" + m_strHwndKey + "*" + m_strScreenNo + "*";
		CFileFind ff;
		CString sFileName;
		CString sNewFileName;
		BOOL bFindNextFile = FALSE;
		if(ff.FindFile(sAllFileName,0))
		{
			while(1)
			{
				bFindNextFile = ff.FindNextFile();
				sFileName = ff.GetFilePath();
				::DeleteFile(sFileName);
				if(!bFindNextFile)	break;
			}
		}
	}
	else if ( nType == HWND_DEFAULT_COPY )	//	핸들키 파일 => Default 파일 복사
	{
// [Edit]  김현수 2009/08/26
//		CString sAllFileName = strUserPath + "\\" + "*" + m_strHwndKey + "*";
		CString sAllFileName = strUserPath + "\\" + "*" + m_strHwndKey + "*" + m_strScreenNo + "*";
		CFileFind ff;
		CString sFileName;
		CString sNewFileName;
		BOOL bFindNextFile = FALSE;
		if(ff.FindFile(sAllFileName,0))
		{
			while(1)
			{
				bFindNextFile = ff.FindNextFile();
				sNewFileName = sFileName = ff.GetFilePath();

				sNewFileName.Replace(m_strHwndKey, "DEFAULT");
				::CopyFile(sFileName, sNewFileName, FALSE);
				if(!bFindNextFile)	break;
			}
		}
	}
	else if ( nType == DEFAULT_HWND_COPY )	//	Default 파일 복사 => 핸들키 파일
	{
// [Edit]  김현수 2009/08/26
//		CString sAllFileName = strUserPath + "\\" + "*" + "DEFAULT" + "*";
		CString sAllFileName = strUserPath + "\\" + "*" + "DEFAULT" + "*" + m_strScreenNo + "*";
		CFileFind ff;
		CString sFileName;
		CString sNewFileName;
		BOOL bFindNextFile = FALSE;
		if(ff.FindFile(sAllFileName,0))
		{
			while(1)
			{
				bFindNextFile = ff.FindNextFile();
				sNewFileName = sFileName = ff.GetFilePath();

				sNewFileName.Replace("DEFAULT", m_strHwndKey);
				::CopyFile(sFileName, sNewFileName, FALSE);
				if(!bFindNextFile)	break;
			}
		}
	}
	else if ( nType == LAST_HWND_COPY )	//	메인이 종료될때 화면 키값을 변경시켜준다. 
	{
// [Edit]  김현수 2009/08/26
//		CString sAllFileName = strUserPath + "\\" + "*" + m_strHwndKey + "*";
		CString sAllFileName = strUserPath + "\\" + "*" + m_strHwndKey + "*" + m_strScreenNo + "*";
		CFileFind ff;
		CString sFileName;
		CString sNewFileName;
		BOOL bFindNextFile = FALSE;
		if(ff.FindFile(sAllFileName,0))
		{
			while(1)
			{
				bFindNextFile = ff.FindNextFile();
				sNewFileName = sFileName = ff.GetFilePath();

				sNewFileName.Replace(m_strHwndKey, m_strSaveKey);
				::CopyFile(sFileName, sNewFileName, FALSE);
				if(!bFindNextFile)	break;
			}
		}
	}
	else if ( nType == HWND_LAST_COPY )	//	저장화면을 핸들값을 추가한 파일로 카피한다.
	{
// [Edit]  김현수 2009/08/26
//		CString sAllFileName = strUserPath + "\\" + "*" + m_strScreenKey + "*";
		CString sAllFileName = strUserPath + "\\" + "*" + m_strScreenKey + "*" + m_strScreenNo + "*";
		CFileFind ff;
		CString sFileName;
		CString sNewFileName;
		BOOL bFindNextFile = FALSE;
		if(ff.FindFile(sAllFileName,0))
		{
			while(1)
			{
				bFindNextFile = ff.FindNextFile();
				sNewFileName = sFileName = ff.GetFilePath();

				sNewFileName.Replace(m_strScreenKey, m_strHwndKey);
				::CopyFile(sFileName, sNewFileName, FALSE);
				::DeleteFile(sFileName);
				if(!bFindNextFile)	break;
			}
		}
	}

#endif
}

LRESULT CStdDialog::OnUserInfo(WPARAM wp, LPARAM lp)
{
	BYTE bt = LOBYTE(LOWORD(wp));
	switch (bt)
	{
	case USERINFO_SET:
		{
			WORD dwKey = HIWORD(wp);
			if (dwKey==0)
			{
				IPartnerManager* pPtManager = (IPartnerManager*)lp;
				DoSetPartnerSite(pPtManager);
			}
		}
		break;
	}
	
	return 0L;
}

void CStdDialog::_DoFirstRequest()
{
	int nStep;
	CString rCode;

	GetFirstCode(rCode, CMasterTypeInfo::FO_FOREIGN_CHART, nStep);

	if(rCode.GetLength()>0)
		SendMessage(UMSG_SENDSHAREDCODE, (WPARAM)(LPCSTR)rCode, (LPARAM)m_nMarketType);

	return;
}

BOOL CStdDialog::GetFirstCode(CString &rString, int nMarketType, int &nSetp)
{
	BOOL bRet = TRUE;
	rString.Empty();
	nSetp=0;

	char szTemp[32];
	memset(szTemp, 0x00, sizeof(szTemp));
	::GetPrivateProfileString("0000", "Code00", "", szTemp, sizeof(szTemp), m_strConfig);

	rString.Format(_T("%s"), szTemp);

	int nCodeType;
	if(rString.IsEmpty()==FALSE)
	{
		nMarketType = GetMarketTypeFromCode(rString, nCodeType);
		if (nMarketType != CMasterTypeInfo::FO_FOREIGN_CHART)
			rString = "";
	}

	CStringArray arrCode, arrName;
	CString rCode;

//	if (nMarketType == CMasterTypeInfo::STOCK_CHART)
	{
/*
		//Step 1. 조회하도록 셋팅된 종목찾기
		//Step 2. 최근조회종목.
		pMasterDataManager->GetRecentCodeNameArray(nMarketType, arrCode, arrName);
		if (arrCode.GetSize()>0)
		{
			rCode = arrCode.GetAt(0);
			nSetp = 2;
			rString = rCode;
		}
		else		//Step 3. 디폴트 코드로 조회.
		{
			CStringArray arCode, arName, arEngName;
			CStringArray *parCode = &arCode, *parName = &arName, *parEngName = &arEngName;
			pMasterDataManager->GetFutureJongMst(parCode, parName, parEngName);
			if(arCode.GetSize()>0)
			{
				rCode = arCode.GetAt(0);
				nSetp = 3;
				rString = rCode;
			}
			else
			{
				rString = "";
				bRet = FALSE;
			}
		}
*/
		IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
		pMasterDataManager->GetRecentCodeNameArray(nMarketType, arrCode, arrName);
		if(arrCode.GetSize()>0)
		{
			rCode = arrCode.GetAt(0);
			nSetp = 2;
			rString = rCode;
		};
	}

	return bRet;
}

CString CStdDialog::GetCodeType(CString strCode)
{
	CString strType = "";
	m_mapCodeType.Lookup(strCode, strType);

	return strType;
}



// 차트 주문 연동 - onlyojt (20091204)
void CStdDialog::SetOrderLineInfo(BOOL bSet)
{
	if(bSet)
	{
		// 종목
		CString strCodeInputCode = m_pInputBarDlg->m_pCodeInput->GetCode();
		if (strCodeInputCode.IsEmpty ())
			return;

		// 계좌번호
		CString strAccount = "", strAccName, strAccPW = "";
		m_pExtInputBarDlg->GetAccountInfo(strAccount, strAccName, strAccPW);
		if (strAccount.IsEmpty ())
			return;


		m_ChartMng.GetpChartItemSelected()->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 5, 0);

		//매도/매수 구분	'0' : 매도, '1' : 매수
		CString strOrderType = ((CInputBarDlg*)m_pInputBarDlg)->GetOrderType();

		FX_CHE_RQ_INPUT Input;
		memset(&Input, ' ', sizeof(Input));
		memcpy(Input.szAccNo, (LPCTSTR)strAccount, strAccount.GetLength());
		memcpy(Input.szAccNoPW, (LPCTSTR)strAccPW, strAccPW.GetLength());
		memcpy(Input.szItemCode, (LPCTSTR)strCodeInputCode, strCodeInputCode.GetLength());
		if( strOrderType.GetLength() > 0 )
			Input.szMMGB[0] = strOrderType.GetAt(0);

		stSend2Server st;
		st.m_nType = 0;
		st.m_pWnd = m_ChartMng.GetpChartItemSelected();
		st.m_pBuffer = (void*)(LPCTSTR)&Input;
		st.m_lLength= sizeof(Input);
		//// 미체결조회
		//Input.szChGB[0] = '2';
		//st.m_strTRCode = QUERY_STRFX_CHE;	
		//SendMessage(UMSG_SEND2SERVER,0,(long)&st);

		//<<해외선물은 잔고조회 안하고 당일치만 체결내역으로 조회하기로 함
		//// 잔고조회
		//st.m_strTRCode = QUERY_STRFOFOREIGN_JAN;	
		//SendMessage(UMSG_SEND2SERVER,0,(long)&st);
		//>>

		// 체결조회
		Input.szChGB[0] = '1';
		st.m_strTRCode = QUERY_STRFOFOREIGN_CHE2;	
		SendMessage(UMSG_SEND2SERVER,0,(long)&st);
	}
	else
	{
		CChartItem* pChartItem = m_ChartMng.GetpChartItemSelected();;
		if(pChartItem)
		{
			pChartItem->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 5, 0);
			//pChartItem->InvokeAddIn("ChartKoreaSorimachiAddIn.dll:USER_COMMENT", 5, 0);
		}
	}
}


//<<< 22091215 by kmlee for 주문연동
LRESULT CStdDialog::OnChangeAccount(WPARAM wParam, LPARAM lParam)
{
	UpdateCheJanOption ();
	return 0;
}

void CStdDialog::UpdateCheJanOption()
{
	if (m_pExtInputBarDlg->GetChkOrder ())
	{
		// 기존 리스트 삭제..
		SetOrderLineInfo (FALSE);
		// FX차트 주문 연동 - onlyojt (20091204)
		SetOrderLineInfo (TRUE);
	}
}
//>>>
LRESULT CStdDialog::OnAccountInit(WPARAM wp, LPARAM lp)
{
	if(m_pExtInputBarDlg && m_pExtInputBarDlg->m_hWnd)
		m_pExtInputBarDlg->InitAccountCtrl(wp, lp);

	return 0L;
}
