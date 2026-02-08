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

#include "../COMMONTR/ChartRealConverter.h"
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
#include "../CommonTR/SM_fx112.h"					// FX
#include "../CommonTR/SM_CheJanStruct.h"			// 체결 - 주문선에서 사용
#include "../Include_Chart/Dll_Load/IMetaTable.h"	// (2008/9/13 - Seung-Won, Bae) for Multi-Language
#include "../../inc/RealUpdate.h"
#include "../../inc/IAUDataMonitorMng.h"	// UUID_IAUDataMonitorManager : 콜백함수 받는데 사용

#include "../../inc/AZUtil_Ctrl.h"			// OS체크루틴. IsSystemChart()에서 사용
#include "TabSystemTradingEx.h"				// 시스템트레이딩 전략 저장/불러오기

#include "../../inc/RMsgUserInfo.h"

#ifdef __MULTI_LANG__
	#include "../Include_Chart/Dll_Load/IMetaTable.h"	//@Solomon-MultiLang:091117
#endif

//<<20100308_JS.Kim 솔로몬
/*
#include "../../../Common/CommStructDef.h"
// FX차트 주문 연동 - onlyojt (20091204)
#include "../../../TRDll/KTB_CheJanStruct.h"
//<<< 20091211 by kmlee 주문연동
#include "../../../TrDll/TrHeader/signalStruct.h"
#include "../../../common/KHresource.h"
#include "../Common_ST/STConfigEnv.h"
__declspec(dllimport) CString KSLink_GetAccInfo (CString strAccount, int nType);
__declspec(dllimport) long KSLink_GetEnvInfo (int nType);
//>>>
*/
#define CONCLUSION_YES 0   //체결
#define CONCLUSION_NO  1   //미체결

#define ORDERTYPE_MARKET		0 //시장가
#define ORDERTYPE_ENTRY			1 //지정가
#define ORDERTYPE_CHANGE		2 //정정
#define ORDERTYPE_CANCEL		3 //취소
#define ORDERTYPE_STOP			4 //Stop/Limit 적용
#define ORDERTYPE_LIMIT			5 //Stop/Limit 취소
#define ORDERTYPE_CLEAR			6 //주문청산
#define ORDERTYPE_CON_STOP		7 //미청산 Stop/Limit 적용
#define ORDERTYPE_CON_LIMIT		8 //미청산 Stop/Limit 취소

#define ORDERTYPE_STOP_ONLY		9	 //Stop 적용 미체결
#define ORDERTYPE_LIMIT_ONLY	10	//Limit 적용 미체결
#define ORDERTYPE_CON_STOP_ONLY	11	//Stop 적용 미청산
#define ORDERTYPE_CON_LIMIT_ONLY 12	//Limit 적용 미청산
#define ORDERTYPE_CROSS_ENTRY	 13	//역지정가 

#define ORDERTYPE_ENTRY2		14 //지정가(매수/매도 동시에 나오는 주문창을 띄우는 경우)
//>>

//	추가... 솔라리스 환경에서 돌려서 보내야 한다. 고 병훈이가 써놓음
#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")		// htonl, ntohl


#define	WMU_GET_KEY_INFO_ARRAY		WM_USER + 2253	// 해당 AUP에서 입력되어 있는 Key값의 목록을 가져온다. LPARAM은 CStringArray *

#define	WIDTH_SIDEBAR		3

const UINT RMSG_GET_SCREENTYPE = ::RegisterWindowMessage(_T("RMSG_GET_SCREENTYPE"));
const UINT RMSG_GET_MARKETTYPE = ::RegisterWindowMessage(_T("RMSG_GET_MARKETTYPE"));
const UINT RMSG_GET_LINKINFODATA = ::RegisterWindowMessage(_T("RMSG_GET_LINKINFODATA"));

//<<< 20091215 by kmlee for 주문연동
const UINT RMSG_CHANGE_CHARTACCOUNT = ::RegisterWindowMessage(_T("RMSG_CHANGE_CHARTACCOUNT"));
const UINT RMSG_GET_CHARTACCOUNT = ::RegisterWindowMessage(_T("RMSG_GET_CHARTACCOUNT"));
const UINT RMSG_ACCOUNT_INIT = ::RegisterWindowMessage(_T("RMSG_ACCOUNT_INIT"));
//>>>

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

#define WIDTH_SIDEBAR_EXTEND		170
#define WIDTH_SIDEBAR_COLLAPSE		10
#define	REAL_FX_SISE				0xAB

//<<< 20091230 by kmlee  for 주문연동
#define		ORD_TYPE_MARKETORDER		0		// 시장가
#define		ORD_TYPE_ORDER				1		// 지정가
#define		ORD_TYPE_MODIFY				2		// 정정
#define		ORD_TYPE_CANCEL				3		// 취소
#define		ORD_TYPE_STOPLIMIT_APLY1	4		// stop/limit 적용 (미체결)
#define		ORD_TYPE_STOPLIMIT_CANCEL1	5		// stop/limit 취소 (미체결)
#define		ORD_TYPE_CLEAR				6		// 청산
#define		ORD_TYPE_STOPLIMIT_APLY2	7		// stop/limit 적용 (미청산)
#define		ORD_TYPE_STOPLIMIT_CANCEL2	8		// stop/limit 취소 (미청산)
#define		ORD_TYPE_STOP_APPLY3		9		// Stop 적용(미체결)
#define		ORD_TYPE_LIMIT_APPLY3		10		// Limit 적용(미체결)
#define		ORD_TYPE_STOP_APPLY4		11		// Stop 적용(미청산)
#define		ORD_TYPE_LIMIT_APPLY4		12		// Limit 적용(미청산)
#define		ORD_TYPE_ORDER_REVERSE		13		// 역지정가
//>>>

extern HINSTANCE g_hInstance;
//<<< 20091214 by kmlee for 주문연동
extern void dll_GetUnitInfo( CString strCode , CString strMarket, CString& strPrecision, CString& strTickSize );
//

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
// Static CimageList ============================================================>
	int		   CStdDialog::m_static_nChartCnt4ImgList = 0;

	CImageList CStdDialog::m_ImgList_W15H15;				//폭 15에 높이 15인 버튼 이미지리스트
	CImageList CStdDialog::m_ImgList_W20H20;				//폭 20에 높이 19인 버튼 이미지리스트
	CImageList CStdDialog::m_ImgList_W20H20_Tool;			//폭 20에 높이 19인 버튼 이미지리스트
	CImageList CStdDialog::m_ImgList_W46H20;				//폭 46에 높이 19인 버튼 이미지리스트	
	CImageList CStdDialog::m_ImgList_W50H20;				//폭 50에 높이 19인 버튼 이미지리스트

	CImageList CStdDialog::m_ImgList_W151H21;				//폭 151에 높이 21인 버튼 이미지리스트
	CImageList CStdDialog::m_ImgList_W28H22;				//폭 28에 높이 22인 버튼 이미지리스트
	CImageList CStdDialog::m_ImgList_W17H20;				//폭 17에 높이 20인 버튼 이미지리스트
	CImageList CStdDialog::m_ImgList_W43H20;				//폭 43에 높이 20인 버튼 이미지리스트
	CImageList CStdDialog::m_ImgList_W58H20;				//폭 58에 높이 20인 버튼 이미지리스트
	CImageList CStdDialog::m_ImgList_W72H20;				//폭 72에 높이 20인 버튼 이미지리스트
	CImageList CStdDialog::m_ImgList_W46H20_Gwan;			//폭 46에 높이 20인 버튼 이미지리스트
	CImageList CStdDialog::m_ImgList_W8H25;					//폭 8에 높이 25인 버튼 이미지리스트
// <==============================================================================	

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
	m_nLeftBarType		= 0;
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
	m_hViewInMain		= NULL;
	m_lWidthOriginal	= 0;
	m_lHeightOriginal	= 0;
	m_bHideChart		= FALSE;
	m_bInitResize		= FALSE;

	m_hSocket			= NULL;
	m_hMainFrame		= NULL;

	m_bShowDataListWnd	= FALSE;			// 데이터 윈도우를 보일지 여부
	m_hReplayChartLib	= NULL;				// 복기  Lib Handle

	m_nMarketType		= CMasterTypeInfo::FX_CHART;

	m_bActiveDataWindow = false;
	m_pDataWindow		= NULL;
	m_pRqWndFromTab		= NULL;

	m_pWndReplayLib		= NULL;				//복기 관련 Wnd Pointer


	memset(&m_ChartCfgInfo, 0x00, sizeof(CHART_CONFIG_INFO));
	
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

	m_nExtendState			= 0;				// 20061212 정유진 수정, grid column 확장 btn 추가
	m_cMUM					= 'P';
	m_bPreMarket			= FALSE;			// 장전 데이터 처리 by 20070226 by LYH
	m_cUT = '1';								// 틱데이터 없을 경우 클리어 문제 처리
	m_strRecvCode			= "";
	m_strSvcID				= "CP986100";		// 2007.05.02 by LYH 서버 통계 위해 서비스 ID 추가

	m_pDrdsLib				= NULL;				// 실시간..
	m_pDlgGlass				= NULL;				// 돋보기 구현 - ojtaso (20080519)

	g_pDebugMng				= NULL;
	g_pPtUtilMng			= NULL;

	::memset(&m_stTrInfo2, 0x00, sizeof(m_stTrInfo2));

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
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_SPLITTER_MOVED, OnWmSplitterMoved )	
	//ON_MESSAGE( WM_GET_BROD, OnGetBroad )					//20100308_JS.Kim 솔로몬
	ON_REGISTERED_MESSAGE(RMS_ANALYSIS_FUNC			, OnByPass_ExportToolOption ) //2005. 07. 28 added by sy,nam	
	ON_REGISTERED_MESSAGE(RMSG_REPLAY_REAL			, OnReplayDrds )
	ON_REGISTERED_MESSAGE(RMSG_REPLAY_FAIL			, OnReplayFail )
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
	//<<< 20091215 by kmlee
	ON_REGISTERED_MESSAGE(RMSG_CHANGE_CHARTACCOUNT, OnChangeAccount)
	//>>>
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

/*
	Function : 멀티 차트 화면 다이알로그 초기화								
	Paramter :																
	Return	:																
	Author	: By DRFN														
	Comments	: FX Margin 하면서 소스 정리 (한국소리치 전용배)				

	ok..
*/
BOOL CStdDialog::OnInitDialog() 
{
	CExOCXDLL::OnInitDialog ();

	AfxEnableControlContainer();

	m_strHwndKey.Format("%d", (long)GetSafeHwnd());

	SetInitInfo ();						// User ID 및 폴더경로 초기화

	InitImageList ();					// 최우선으로 ImageList 로드

	SetBaseInterface ();				// PCTR 의 기본 Interface 셋팅

	m_nChartType = MULTI_CHART;

	CString strScreenType, strMarketType;

	if (m_nScreenID == atoi("9041"))	//@Solomon	FX MultiChart
	{
		m_nMarketType = CMasterTypeInfo::FX_CHART;
		m_bSmallScreenMode = FALSE;
		m_nChartType = MULTI_CHART;
		m_bMultiItemChart = TRUE;
	}
	else if (m_nScreenID == atoi("9043"))	//@Solomon	FX MiniChart
	{
		m_nMarketType = CMasterTypeInfo::FX_CHART;
		m_bSmallScreenMode = FALSE;
		m_nChartType = MINI_MULTI_CHART;
		m_bMultiItemChart = TRUE;
	}


#ifdef _WRITELOG
	CreateLogFile();		// Log - ojtaso (20080107)
#endif

	IPartnerSvrManager* pPtSvrMng = (IPartnerSvrManager*)AfxGetPctrInterface(UUID_IPartnerSvrManager);
	if (pPtSvrMng)
		g_pPtUtilMng = (IPartnerUtilManager*)pPtSvrMng->GetInerInterface(LUID_IPartnerUtilManager);

	FormLoad();				// 차트 폼 로드, 화면에 필요한 툴바 모두 생성하여 올리기..
	
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

/*
	ok
*/
void CStdDialog::ChangeStockMode (int type)
{
	//<< 솔로몬. FX 전용 차트이므로 시장 변경 필요없음
	return;
	//>>

	if(m_nChartType == MINI_MULTI_CHART || m_nChartType == MINI_SINGLE_CHART)
	{
		ChangeMiniStockMode(type);
		return;
	}

	CString strTemp;
	UnLoad_ReplayChart_Lib();		//복기 Lib UnLoad
	
	m_nMarketType =  type;

	//------------------------------------------------------------------------------
	if (m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
	{
		m_pInputBarDlg->SetMarketType (m_nMarketType);
		m_pInputBarDlg->MoveWindow (GetRect (ID_INPUTBAR,TRUE));
		//2005. 06. 22 종목 코드를 받은 상태 에서 생성한다.
		BOOL bRequestAPData = FALSE;
	}

	//------------------------------------------------------------------------------
	// 2005. 05. 16 chart 구분자를 넘긴다.
	//------------------------------------------------------------------------------
	if (m_pExtInputBarDlg)
	{
		m_pExtInputBarDlg->SetMarketType (m_nMarketType);
		m_pExtInputBarDlg->MoveWindow (GetRect(ID_EXTINPUTBAR,TRUE));
	}
	//------------------------------------------------------------------------------
	// 2005. 05. 16 chart 구분자를 넘긴다.
	//------------------------------------------------------------------------------	
	m_ChartMng.SetMarketType (m_nMarketType);
	m_pInputBarDlg->ChangeMarket (m_nMarketType, FALSE, TRUE);
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

	m_strConfig = m_strUserDir + "\\" + m_strScreenNo + "_" + ENVINFOFILE;

	if(m_pInputBarDlg)
	{
		m_pInputBarDlg->m_strCfgFile = m_strConfig;

		m_pInputBarDlg->SetMarketType(m_nMarketType);	
		m_pInputBarDlg->MoveWindow(GetRect(ID_INPUTBAR,TRUE));
	}


	//------------------------------------------------------------------------------
	// 2005. 05. 16 chart 구분자를 넘긴다.
	//------------------------------------------------------------------------------	
	m_ChartMng.SetMarketType(m_nMarketType);
	m_pInputBarDlg->ChangeMarket(m_nMarketType, FALSE, TRUE);
}

/*
	Function : 멀티 차트 폼 로드	
	Paramter : ?					
	Return	:					
	Author	: By DRFN			
	Comments	: FX Margin 하면서 소스 정리 (한국소리치 전용배)
	Date     : 2008/07/10 
*/
void CStdDialog::FormLoad()
{
	m_pMsgBar = new CMessageBar;								// 메세지바 생성
	m_pMsgBar->Create (CMessageBar::IDD, this);

	m_TRComm = (ITrComm2*)GetTrComm ();							// TR 및 DRDS 초기화

	InitDrds ();												// 통신 키 초기화

	m_FavoriteMng.SetPath (m_strUserDir);						// 사용자 환경 path얻어오기
	
	if (_access (m_strUserDir, 0) == -1)						// User Directory 생성
	{															// 참조파일 : IO.h, Direct.h
		 int retval = _mkdir(m_strUserDir);
	}  

	// 설정파일 경로	
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

//		::GetPrivateProfileString("Frame","ShowMigyulche", "1", szTemp, sizeof(szTemp),m_strConfig);
//		m_bMigyulcheDraw = atoi(szTemp);
	}
	else if (m_nChartType == SINGLE_CHART)
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

	// 스플릿바 생성
	m_xSplitter1.CreateInWnd(this, ID_SPLITTER, GetRect(ID_SPLITTER));

	// 차트매니저 생성
	m_ChartMng.SetMarketType(m_nMarketType);
	m_ChartMng.m_strRootPath = m_strRootPath;
	BOOL bRetValue = m_ChartMng.Create(GetRect(ID_CHART),this, m_strUserDir, m_szImageDir,ID_CHART, (long)((CMChartInterface*)this));
	if (!bRetValue) return;
	m_ChartMng.SetBaseDesk ((long)1);            // 데스크가 없어짐으로 양의 정수를 셋팅함으로 기본동작 가능

	if((m_nChartType != MINI_MULTI_CHART && m_nChartType != MINI_SINGLE_CHART) && m_pLeftBarDlg)
		m_pLeftBarDlg->SetConfig();

	if( m_ChartMng.GetpChartItemSelected() != NULL)
		m_ChartMng.GetpChartItemSelected()->LoadTickMinDayInfo(m_strConfig);
#ifdef _DEBUG
	else
	{
		AfxMessageBox("FormLoad : m_ChartMng.GetpChartItemSelected() is NULL !!");
		return;
	}
#endif 

	// (보조)상단바 타입 및 Visible 셋팅
	SetLeftBarType(m_nLeftBarType);
	SetShowExtInputBarDlg(m_bShowExtInputBarDlg);

	BOOL bRequestAPData = TRUE;

	// 종목코드 컨트롤 생성
	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
	{
		m_pInputBarDlg->InitBaseInfo(NULL, bRequestAPData);
	}

	// 스킨색상
	//<<20100308_JS.Kim 솔로몬
	//IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager, 2);
	//if(pKSLibMng)
	//	m_clrBackground = DEFAULT_BKGRND_COLOR;		// 기본값
	//else
	//	m_clrBackground = DEFAULT_BKGRND_COLOR;		// 기본값
	m_clrBackground = DEFAULT_BKGRND_COLOR;		// 기본값
	//>>
	int nTheme = 0;//AfxGetMainWnd()->SendMessage(WMU_GET_THEME);	
	OnChangeSkinColor(nTheme, 0);
//	ChangeSkinColor();
	Invalidate();
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

	if( m_pLeftBarDlg )
		m_pLeftBarDlg->Invalidate();
	m_ChartMng.Invalidate();
	m_xSplitter1.Invalidate();
	m_pInputBarDlg->ShowWindow(SW_HIDE);
	m_pExtInputBarDlg->Invalidate();
	m_pInputBarDlg->ShowWindow(SW_SHOW);


	if(m_ChartMng.m_pFloatingDlg != NULL)				
		m_ChartMng.m_pFloatingDlg->Invalidate();

	
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
		if(m_pLeftBarDlg && m_pLeftBarDlg->GetSafeHwnd()&&!bInit)
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
		//<<< 20091223 by kmlee - 디자인 적용
		rect.top -= 2;
		//>>>
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

			}
		}

		
	
		break;
	case ID_TOOLBAR:
		if(m_pDlgToolbar->GetSafeHwnd())
		{
			rect.left = rect.right;
			rect.right = rect.right + WIDTH_TOOLBAR;
		}
		//<<< 20091223 by kmlee - 디자인 적용
		rect.top -= 2;
		//>>>
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
	}

	if(m_xSplitter1.m_hWnd != NULL )
	{
		m_xSplitter1.GetWindowRect(rect);
		ScreenToClient(rect);
		rectNew = GetRect(ID_SPLITTER,m_bInitResize);
		if(rect!=rectNew)	
		{		
			m_xSplitter1.MoveWindow(rectNew);
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
		}
		//<<20100308_JS.Kim 솔로몬
		/*
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
		//>>
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
		}
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
	CString strTemp;

	m_pPTManager = NULL;

#ifdef _WRITELOG
	WriteToStringLog("좌측바 위치 저장::Begin", _T("CStdDialog"), 10);
#endif
	
	if(!m_bSmallScreenMode)		// DRDS 종료
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
	

#ifdef _WRITELOG
	WriteToStringLog("좌측바 위치 저장::End", _T("CStdDialog"), 10);
#endif
	
#ifdef _WRITELOG
	WriteToStringLog("차트매니저 종료::Begin", _T("CStdDialog"), 10);
#endif

	if(m_ChartMng.GetSafeHwnd())
		m_ChartMng.DestroyWindow();

#ifdef _WRITELOG
	WriteToStringLog("차트매니저 종료::End", _T("CStdDialog"), 10);
#endif
	
#ifdef _WRITELOG
	WriteToStringLog("구분선 종료::Begin", _T("CStdDialog"), 10);
#endif

	if(m_xSplitter1.GetSafeHwnd())
	{
		m_xSplitter1.DetachAllCtrls();
		m_xSplitter1.DestroyWindow();
	}

#ifdef _WRITELOG
	WriteToStringLog("구분선 종료::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	WriteToStringLog("좌측툴바 종료::Begin", _T("CStdDialog"), 10);
#endif

	if(m_pLeftBarDlg)
	{
		m_pLeftBarDlg->DestroyWindow();
		delete m_pLeftBarDlg;
		m_pLeftBarDlg = NULL;
	}

#ifdef _WRITELOG
	WriteToStringLog("좌측툴바 종료::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	WriteToStringLog("인풋바 종료::Begin", _T("CStdDialog"), 10);
#endif

	if(m_pInputBarDlg)
	{
		m_pInputBarDlg->DestroyWindow();
		delete m_pInputBarDlg;
		m_pInputBarDlg = NULL;
	}

#ifdef _WRITELOG
	WriteToStringLog("인풋바 종료::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	WriteToStringLog("인풋바2 종료::Begin", _T("CStdDialog"), 10);
#endif

	if(m_pExtInputBarDlg)
	{
		m_pExtInputBarDlg->DestroyWindow();
		delete m_pExtInputBarDlg;
		m_pExtInputBarDlg = NULL;
	}

#ifdef _WRITELOG
	WriteToStringLog("인풋바2 종료::End", _T("CStdDialog"), 10);
#endif
	
	
#ifdef _WRITELOG
	WriteToStringLog("메세지바 종료::Begin", _T("CStdDialog"), 10);
#endif

	if(m_pMsgBar)
	{
		m_pMsgBar->DestroyWindow();
		delete m_pMsgBar;
		m_pMsgBar = NULL;
	}	

#ifdef _WRITELOG
	WriteToStringLog("메세지바 종료::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	WriteToStringLog("복기 라이브러리 종료::Begin", _T("CStdDialog"), 10);
#endif

	UnLoad_ReplayChart_Lib();	

#ifdef _WRITELOG
	WriteToStringLog("복기 라이브러리 종료::End", _T("CStdDialog"), 10);
#endif

	POSITION pos = NULL;	

#ifdef _WRITELOG
	WriteToStringLog("DRDS 종료::Begin", _T("CStdDialog"), 10);
#endif
	
	//<<20100308_JS.Kim 솔로몬
	//if ( m_pDrdsLib )
	//	m_pDrdsLib->DrdsUnadviseAll(m_hWnd);	// 핸들에 등록되어있는 리얼을 전부 해제시킨다
	//AllCodeUnAdvise ();							// Real 전부 해제..
	//>>

	
#ifdef _WRITELOG
	WriteToStringLog("DRDS 종료::End", _T("CStdDialog"), 10);
#endif
	

#ifdef _WRITELOG
	WriteToStringLog("데이타 윈도우 종료::Begin", _T("CStdDialog"), 10);
#endif
	
	if(m_pDataWindow)
	{
		m_pDataWindow->DestroyWindow();
		delete m_pDataWindow;
		m_pDataWindow = NULL;
	}
	
#ifdef _WRITELOG
	WriteToStringLog("데이타 윈도우 종료::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	WriteToStringLog("툴바 종료::Begin", _T("CStdDialog"), 10);
#endif
	
	if(m_pDlgToolbar)
	{
		m_pDlgToolbar->DestroyWindow();
		delete m_pDlgToolbar;
		m_pDlgToolbar = NULL;
	}
	
#ifdef _WRITELOG
	WriteToStringLog("툴바 종료::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	WriteToStringLog("이미지리스트 종료::Begin", _T("CStdDialog"), 10);
#endif
	
	DeleteAllImageList();
	
#ifdef _WRITELOG
	WriteToStringLog("이미지리스트 종료::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	WriteToStringLog("커서 종료::Begin", _T("CStdDialog"), 10);
#endif
	
	if(m_hCursor)
		::DeleteObject(m_hCursor);
	
#ifdef _WRITELOG
	WriteToStringLog("커서 종료::End", _T("CStdDialog"), 10);
#endif

#ifdef _WRITELOG
	CloseLogFile();
#endif
	
	if(m_pDlgGlass) // 돋보기 구현 - ojtaso (20080519)
	{
		delete m_pDlgGlass;
		m_pDlgGlass = NULL;
	}

	m_memoryDC.DeleteDC();
	m_bufferBitmap.DeleteObject();

	//<<< 20100113 by kmlee 체결잔고 라이브러리 해제..
	//<<20100308_JS.Kim 솔로몬
	//IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager, 2);
	//if (pKSLibMng)
	//	pKSLibMng->Lib_KSLink_SetGridCheJanLibFree (this);
	//>>
	//>>>

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

CString CStdDialog::MakeChartData (STDR_FID* pStdrFid, int& nCount, int dwTRDataLen, int nMarketType, int nType, double dPrePrice, int nUnitType/*=WON_TYPE*/)
{
//<<20100308_JS.Kim 솔로몬
/*
#ifdef _WRITELOG
	WriteToStringLog("서버로부터 받은 Data를 차트 Data로 가공::Begin", m_strRecvCode, m_strRecvCode.GetLength());
#endif

	if(nCount <= 0)
		return "";

	char* szChartData = NULL;
	CString strChartData;
	int nBufferLength = 0;
	if(nMarketType == FX_CHART)
	{
		SM_fx112_OutRec2 eu0602OutRec2;
		nBufferLength = sizeof(SM_fx112_OutRec2) * nCount + 1;
		szChartData = strChartData.GetBuffer(nBufferLength);
		CString strData;
		double dRate = 0.;
		for(int i = 0; i < nCount; ++i)
		{
			if(nUnitType == PERCENT_TYPE)
			{
				memcpy(eu0602OutRec2.date, pStdrFid->vvRpValueList[nCount - i][0], sizeof(eu0602OutRec2.date));
				strData.Format("%10.2f", atof(pStdrFid->vvRpValueList[nCount - i][1]));
				memcpy(eu0602OutRec2.open, strData, sizeof(eu0602OutRec2.open));
				strData.Format("%10.2f", atof(pStdrFid->vvRpValueList[nCount - i][2]));
				memcpy(eu0602OutRec2.high, strData, sizeof(eu0602OutRec2.high));
				strData.Format("%10.2f", atof(pStdrFid->vvRpValueList[nCount - i][3]));
				memcpy(eu0602OutRec2.low, strData, sizeof(eu0602OutRec2.low));
				strData.Format("%10.2f", atof(pStdrFid->vvRpValueList[nCount - i][4]));
				memcpy(eu0602OutRec2.close, strData, sizeof(eu0602OutRec2.close));
				memcpy(eu0602OutRec2.volume, pStdrFid->vvRpValueList[nCount - i][5], sizeof(eu0602OutRec2.volume));
				memcpy(eu0602OutRec2.rights, pStdrFid->vvRpValueList[nCount - i][6], sizeof(eu0602OutRec2.rights));
			}
			else if(nUnitType == DOLLAR_TYPE)
			{
				memcpy(eu0602OutRec2.date, pStdrFid->vvRpValueList[nCount - i][0], sizeof(eu0602OutRec2.date));
				memcpy(eu0602OutRec2.open, pStdrFid->vvRpValueList[nCount - i][1], sizeof(eu0602OutRec2.open));
				memcpy(eu0602OutRec2.high, pStdrFid->vvRpValueList[nCount - i][2], sizeof(eu0602OutRec2.high));
				memcpy(eu0602OutRec2.low, pStdrFid->vvRpValueList[nCount - i][3], sizeof(eu0602OutRec2.low));
				memcpy(eu0602OutRec2.close, pStdrFid->vvRpValueList[nCount - i][4], sizeof(eu0602OutRec2.close));
				memcpy(eu0602OutRec2.volume, pStdrFid->vvRpValueList[nCount - i][5], sizeof(eu0602OutRec2.volume));
				memcpy(eu0602OutRec2.rights, pStdrFid->vvRpValueList[nCount - i][6], sizeof(eu0602OutRec2.rights));
			}
			else	// nUnitType == WON_TYPE
			{
				memcpy(eu0602OutRec2.date, pStdrFid->vvRpValueList[nCount - i][0], sizeof(eu0602OutRec2.date));
				memcpy(eu0602OutRec2.open, pStdrFid->vvRpValueList[nCount - i][1], sizeof(eu0602OutRec2.open));
				memcpy(eu0602OutRec2.high, pStdrFid->vvRpValueList[nCount - i][2], sizeof(eu0602OutRec2.high));
				memcpy(eu0602OutRec2.low, pStdrFid->vvRpValueList[nCount - i][3], sizeof(eu0602OutRec2.low));
				memcpy(eu0602OutRec2.close, pStdrFid->vvRpValueList[nCount - i][4], sizeof(eu0602OutRec2.close));
				memcpy(eu0602OutRec2.volume, pStdrFid->vvRpValueList[nCount - i][5], sizeof(eu0602OutRec2.volume));
				memcpy(eu0602OutRec2.rights, pStdrFid->vvRpValueList[nCount - i][6], sizeof(eu0602OutRec2.rights));
			}

			memcpy(szChartData, (LPCSTR)&eu0602OutRec2, sizeof(SM_fx112_OutRec2));
			szChartData += sizeof(SM_fx112_OutRec2);
		}
	}

	strChartData.ReleaseBuffer();
	return strChartData;
	*/
	return "";
	//>>
}

//aTRData : Recieve Data, nIdx:Start Index, nCount: count, dwTRDataLen: Data Length, nMarketType: Stock, Upjong, Future Type, nType : Tick/Min...
CString CStdDialog::MakeChartData(LPVOID aTRData, int nIdx, int& nCount, int dwTRDataLen, int nMarketType, int nType, double dPrePrice, int nUnitType/*=WON_TYPE*/)
{
//<<20100308_JS.Kim 솔로몬
	/*
#ifdef _WRITELOG
	WriteToStringLog("서버로부터 받은 Data를 차트 Data로 가공::Begin", m_strRecvCode, m_strRecvCode.GetLength());
#endif

	m_bPreMarket = FALSE;

	if(nCount < 0)
		nCount = 400;

	CString strData, strOut, strOut2;
	CString strHigh, strClose;

	double rate;
	int nPriceType = C_DT_ULONG, nVolType = C_DT_ULONG, nDotPos = 0;

	// 상품선물옵션차트 - ojtaso (20071001)
	int nLength=0, nExp=0;

	double dDollar = atof(m_strDollar);
	
	int nRights = 0;
	for(int i=0; i<nCount; i++)
	{
		if(nIdx>=dwTRDataLen)
		{
			nCount = i;
			break;
		}

		strOut = "";
		//날짜
		strData = m_TRComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 8, &nIdx, 0, TRUE);
		// Intra_Ticks 추가 - ojtaso (20071227)
		if(nType == TICK_DATA_CHART)
		{
			strData.Format("%2.2s", strData.Right(2));
			CString strTime = m_TRComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 8, &nIdx, 0, TRUE);
			if(atoi(strTime.Mid(4,6)) != 0)
			{
				int nHour = atoi(strTime.Left(2));
				int nMin  = atoi(strTime.Mid(2,2));
				int nSec  = atoi(strTime.Mid(4,2));

				strTime.Format("%02d%02d%02d", nHour, nMin, nSec);
			}
			strData += strTime.Left(6);

		}
		else if(nType == MIN_DATA_CHART)
		{
			strData.Format("%4.4s", strData.Mid(4, 4));
			CString strTime = m_TRComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 8, &nIdx, 0, TRUE);
			if(atoi(strTime.Mid(4,6)) != 0)
			{
				int nHour = atoi(strTime.Mid(0,2));
				int nMin  = atoi(strTime.Mid(2,2)) + 1;
				if(nMin>=60)
				{
					nHour += 1;
					nMin = 0;
				}
				strTime.Format("%02d%02d", nHour, nMin);
			}
			strData += strTime.Mid(0,4);
		}
		strOut += strData;

		if(nMarketType == FOREIGN_CHART && nType == TICK_DATA_CHART)
		{
			//시가,고가,저가,종가
			strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
			strOut += strData;
			strOut += strData;
			strOut += strData;
			strOut += strData;

			if(i==0)
			{
				strHigh = strData;
				strClose = strData;
			}
			strOut += "          ";
		}
		else
		{
			if(nUnitType == PERCENT_TYPE && dPrePrice != 0)
			{
				//시가
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				rate = (atof(strData)/dPrePrice - 1) * 100.0;
				strData.Format("%10.2f", rate);
				strOut += strData;

				//고가
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				if(i==0)
					strHigh = strData;

				rate = (atof(strData)/dPrePrice - 1) * 100.0;
				strData.Format("%10.2f", rate);
				strOut += strData;

				//저가
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				rate = (atof(strData)/dPrePrice - 1) * 100.0;
				strData.Format("%10.2f", rate);
				strOut += strData;

				//종가
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				if(i==0)
					strClose = strData;

				rate = (atof(strData)/dPrePrice - 1) * 100.0;
				strData.Format("%10.2f", rate);
				strOut += strData;
			}
			else if(nUnitType == DOLLAR_TYPE && dDollar != 0)
			{
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				rate = atof(strData)/dDollar;
				strData.Format("%10.2f", rate);
				strOut += strData;

				//고가
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				if(i==0)
					strHigh = strData;

				rate = atof(strData)/dDollar;
				strData.Format("%10.2f", rate);
				strOut += strData;

				//저가
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				rate = atof(strData)/dDollar;
				strData.Format("%10.2f", rate);
				strOut += strData;

				//종가
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				if(i==0)
					strClose = strData;

				rate = atof(strData)/dDollar;
				strData.Format("%10.2f", rate);
				strOut += strData;
			}
			else
			{
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				strOut += strData;

				//고가
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				if(i==0)
					strHigh = strData;

				strOut += strData;

				//저가
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				strOut += strData;

				//종가
				strData = m_TRComm->ApplyByteOrder(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);
				if(i==0)
					strClose = strData;
				strOut += strData;
			}
			//}
			//거래량
			strData = m_TRComm->ApplyByteOrder(nVolType, (char *)aTRData+nIdx, 10, &nIdx);
			strOut += strData;
		}
		//권리락
		if(nMarketType == FX_CHART)
		{
			if(nType == DAY_DATA_CHART)
			{
				strData = m_TRComm->ApplyByteOrder(C_DT_BYTE, (char *)aTRData+nIdx, 3, &nIdx);
				nRights = atoi(strData);
				if(nRights > 0)
				{
					switch(nRights)
					{
					case 10:
					case 20:
					case 30:
					case 40:
					case 50:
					case 60:
					case 70:
						strData.Format("1%d", nRights);
						strOut += strData;
						break;
					case 61:
						strOut += "260";
						break;
					case 71:
						strOut += "270";
						break;
					case 81:
						strOut += "280";
						break;
					case 91:
						strOut += "110";
						break;
					default:
						strOut += "   ";
						break;
					}
				}
				else
					strOut += "   ";
				
			}
			else
			{
				strOut += "   ";
			}
		}
		strOut2 = strOut + strOut2;
	}
	//20070226 by LYH
	//{장전 데이터 처리
	if( atoi(strHigh) < atoi(strClose) )
		m_bPreMarket = TRUE;
	//}}

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	WriteToStringLog("서버로부터 받은 Data를 차트 Data로 가공::End", m_strRecvCode, m_strRecvCode.GetLength());
#endif

	return strOut2;
	*/
	return "";
	//>>
}

/*
	데이타를 전달 받아서 변경. ok

	LPCSTR szTR : TR 코드
	char* pData	: TR 결과 buffer
	long dwTRDateLen : TR 결과 buffer length

	m_strOut에 다음과 같이 변환된 데이타가 저장되게 된다.
	챠트count[5];보조메시지길이[4];보조메시지[296];챠트데이타-count 만큼

*/
void CStdDialog::ConvertKTBFXData (LPCSTR szTR, char* pData, long dwTRDateLen)
{
	TRACE("dwTRDateLen=%d\n", dwTRDateLen);

	if (pData == NULL || dwTRDateLen <= 0)
		return;

	CString strTRCode = szTR;
	strTRCode.Remove (' ');

	if (strTRCode == QUERY_STRFX)		// TR = x56000#
	{
		CString		strData			= "";
		CString		strTemp			= "";
		CString		strPipLowest	= "";

		m_strOut = "";
/*
		//	데이터의 갯수를 구한다.
		int	nDataCnt = (dwTRDateLen - EU_X56000_OUT1_SZ - EU_X56000_OUT_KEY_SZ) / EU_X56000_OUT2_SZ;

		//	받은 데이터가 레코드의 길이보다 작을경우 오류이므로 return
		if (nDataCnt <= 0)		
		{
			//<<< 20091109 by kmlee 데이타가 조회되지 않아 메시지 출력
			AfxMessageBox ("데이타가 없습니다!! 다시 조회하십시오!!");
			//>>>
			return;
		}

		EU_x56000_OutRec1		singOutput;	
		EU_x56000_OutRec2		output;
		CString					strOut;
		int						nPipLowest(0);

		//	싱글데이터에서 소수점 데이터를 받아서 처리하는부분이 필요할거 같음... ㅠ.ㅠ
		//	( 소수점 자릿수를 마스터파일에서 읽음 => TR로 받아서 사용 )
		memset (&singOutput, 0x00,  EU_X56000_OUT1_SZ);
		memcpy (&singOutput, pData, EU_X56000_OUT1_SZ);	//	KeyValue와 데이터 갯수를 짤라낸다.

		CChartItem* pChartItem = m_ChartMng.GetpChartItemSelected();
		char chTypeRequested = m_ChartMng.GetpChartItemSelected()->m_lpSelChart ? 
							m_ChartMng.GetpChartItemSelected()->m_lpSelChart->m_cType : pChartItem->GetTypeRequested();

		CString strBojoyMsg = "";
		strTemp.Format("%05d", nDataCnt);
		strOut += strTemp;

		//	소수점 자리수
		nPipLowest = ntohl (singOutput.nPipLowest);
		strPipLowest.Format("%.*f", nPipLowest, pow(10 , nPipLowest*(-1)));

		nPipLowest += 1;

		if (chTypeRequested == DAY_DATA_CHART)
		{
			strBojoyMsg.Format ("UM=E@UD=1@RDATET=YYYYMMDD@TDC=%s:시가,|%s:고가,|%s:저가,|%s:종가,|1:기본거래량,|1:락유무,@RDC=%s:%d:시가,|%s:%d:고가,|%s:%d:저가,|%s:%d:종가,|1:기본거래량,|1:락유무,@REALTIMET=YYYYMMDDHHMMSS@", 
								strPipLowest, strPipLowest, strPipLowest, strPipLowest, 
								strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest);
		}
		else if (chTypeRequested == WEEK_DATA_CHART)
		{
			strBojoyMsg.Format ("UM=E@UD=2@RDATET=YYYYMMDD@TDC=%s:시가,|%s:고가,|%s:저가,|%s:종가,|1:기본거래량,|1:락유무,@RDC=%s:%d:시가,|%s:%d:고가,|%s:%d:저가,|%s:%d:종가,|1:기본거래량,|1:락유무,@REALTIMET=YYYYMMDDHHMMSS@", 
								strPipLowest, strPipLowest, strPipLowest, strPipLowest, 
								strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest);
		}
		else if (chTypeRequested == MONTH_DATA_CHART)
		{
			strBojoyMsg.Format ("UM=E@UD=3@RDATET=YYYYMM@TDC=%s:시가,|%s:고가,|%s:저가,|%s:종가,|1:기본거래량,|1:락유무,@RDC=%s:%d:시가,|%s:%d:고가,|%s:%d:저가,|%s:%d:종가,|1:기본거래량,|1:락유무,@REALTIMET=YYYYMMDDHHMMSS@", 
								strPipLowest, strPipLowest, strPipLowest, strPipLowest, 
								strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest);
		}
		else if (chTypeRequested == MIN_DATA_CHART)
		{
			CString strTerm = pChartItem->GetTickMinDayNumberRequested ();
			strBojoyMsg.Format ("UM=E@UD=4@UT=%d@RDATET=MMDDHHMM@TDC=%s:시가,|%s:고가,|%s:저가,|%s:종가,|1:기본거래량,|1:락유무,@RDC=%s:%d:시가,|%s:%d:고가,|%s:%d:저가,|%s:%d:종가,|1:기본거래량,|1:락유무,@REALTIMET=YYYYMMDDHHMMSS@", atoi(strTerm),
								strPipLowest, strPipLowest, strPipLowest, strPipLowest, 
								strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest);
		}
		else if (chTypeRequested == TICK_DATA_CHART)
		{
			strBojoyMsg.Format ("UM=E@UD=5@RDATET=HHMMSS@TDC=%s:시가,|%s:고가,|%s:저가,|%s:종가,|1:기본거래량,|1:락유무,@RDC=%s:%d:시가,|%s:%d:고가,|%s:%d:저가,|%s:%d:종가,|1:기본거래량,|1:락유무,@REALTIMET=YYYYMMDDHHMMSS@", 
								strPipLowest, strPipLowest, strPipLowest, strPipLowest, 
								strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest);
		}
		else if (chTypeRequested == HOUR_DATA_CHART)
		{
			//	분일경우 분 단위설정값을 변경해달라는 요청 ( 배승원부장님요청...  144분 => 0224로 변경해서 달라고함 )
			CString strTerm = pChartItem->GetTickMinDayNumberRequested ();
			long	nTime = atol(strTerm) * 100;
			strTerm.Format ("%02d00", nTime);

			strBojoyMsg.Format ("UM=E@UD=4@UT=%d@RDATET=MMDDHHMM@TDC=%s:시가,|%s:고가,|%s:저가,|%s:종가,|1:기본거래량,|1:락유무,@RDC=%s:%d:시가,|%s:%d:고가,|%s:%d:저가,|%s:%d:종가,|1:기본거래량,|1:락유무,@REALTIMET=YYYYMMDDHHMMSS@", 
								nTime, strPipLowest, strPipLowest, strPipLowest, strPipLowest, 
								strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest, strPipLowest, nPipLowest);
		}
		else
		{
			return;
		}

		nPipLowest -= 1;

		CString strBojoMsg;
		strBojoMsg.Format("0296%-296s", strBojoyMsg);
		strOut += strBojoMsg;

		// (2008/9/14 - Seung-Won, Bae) for Pair Tick
		char *szPrice = NULL;
		CString strItemCode;
		
		if (m_ChartMng.GetpChartItemSelected ()) 
			if (m_ChartMng.GetpChartItemSelected ()->m_lpSelChart)
				strItemCode = m_ChartMng.GetpChartItemSelected ()->m_lpSelChart->m_strItemCode;

		BOOL bAskItem = FALSE;
		if (!strItemCode.IsEmpty ()) 
			if (strItemCode.GetAt (0) == '|') 
				if (strItemCode.GetAt (1) == 'S') 
					bAskItem = TRUE;

		int nTemp (0);
		char cTemp [128];
 		double	fTemp = pow (10, nPipLowest);


		for (int i = nDataCnt - 1; 0 <= i; i--)
		{
			memset (&output, 0x00, EU_X56000_OUT2_SZ);
			memcpy (&output,							//	KeyValue와 데이터 갯수를 짤라낸다.
				pData + ( i * EU_X56000_OUT2_SZ ) + EU_X56000_OUT1_SZ + EU_X56000_OUT_KEY_SZ, 
				EU_X56000_OUT2_SZ);	

			memset(cTemp, 0x00, 128);

			//<<< date & time 처리 ==========================================
			if (chTypeRequested == TICK_DATA_CHART)			// HHMMSS
			{
				memcpy (cTemp, &output.szDate [8], 6);
				strTemp = cTemp;
				strTemp.Remove ('.');
				strData.Format ("%6s00", strTemp);
				strOut += strData;
			}
			else if (chTypeRequested == MIN_DATA_CHART)		// MMDDHHMM
			{
				memcpy (cTemp, &output.szDate [4], 8 );
				strTemp = cTemp;
				strTemp.Remove ('.');
				strData.Format ("%8s", strTemp);
				strOut += strData;
			}
			else if (chTypeRequested == HOUR_DATA_CHART)	// MMDDHHMM
			{
				memcpy (cTemp, &output.szDate [4], 8);
				strTemp = cTemp;
				strTemp.Remove ('.');
				strData.Format ("%8s", strTemp);
				strOut += strData;
			}
			else if (chTypeRequested == MONTH_DATA_CHART)	// YYYYMM
			{
				memcpy (cTemp, output.szDate, 6);
				strTemp = cTemp;
				strTemp.Remove ('.');
				strData.Format ("%6s01", strTemp);
				strOut += strData;
			}
			else											// YYYYMMDD
			{
				memcpy (cTemp, output.szDate, 8);
				strTemp = cTemp;
				strTemp.Remove ('.');
				strData.Format ("%8s", strTemp);
				strOut += strData;
			}
			//>>> date & time 처리 ==========================================

			//<<< 데이타 처리 ===============================================
			if (chTypeRequested == TICK_DATA_CHART)
			{
				// (2008/9/14 - Seung-Won, Bae) for Pair Tick
				if (bAskItem)	szPrice = output.szAsk;
				else			szPrice = output.szBid;

				//	시가
				memset (cTemp, 0x00, 128);
				memcpy (cTemp, szPrice, sizeof (output.szBid));
				strTemp.Format ("%.0f", fTemp * atof (cTemp));
				strData.Format ("%10s", strTemp);
				strOut += strData;

				//	고가
				memset (cTemp, 0x00, 128);
				memcpy (cTemp, szPrice, sizeof (output.szBid));
				strTemp.Format ("%.0f", fTemp * atof (cTemp));
				strData.Format ("%10s", strTemp);
				strOut += strData;

				//	저가
				memset (cTemp, 0x00, 128);
				memcpy (cTemp, szPrice, sizeof (output.szBid));
				strTemp.Format ("%.0f", fTemp * atof (cTemp));
				strData.Format ("%10s", strTemp);
				strOut += strData;

				//	종가
				memset (cTemp, 0x00, 128);
				memcpy (cTemp, szPrice, sizeof (output.szBid));
				strTemp.Format ("%.0f", fTemp * atof (cTemp));
				strData.Format ("%10s", strTemp);
				strOut += strData;

				//	체결량
				strTemp.Format ("%.0f", fTemp * 0);
				strData.Format ("%10s", strTemp);
				strOut += strData;
				
				//	권리락 ( 일단 우리 서버데이터에는 없는데.. 여기에서 차트 넘겨주는 데이터에서 필요함... 나중에 조정해야할거 같음 )
				strOut += "0";
			}
			else
			{
				//	시가
				memset (cTemp, 0x00, 128);
				memcpy (cTemp, output.szBidOpen, sizeof (output.szBidOpen));
				strTemp.Format ("%.0f", fTemp * atof (cTemp));
				strData.Format ("%10s", strTemp);
				strOut += strData;

				//	고가
				memset (cTemp, 0x00, 128);
				memcpy (cTemp, output.szBidHigh, sizeof (output.szBidHigh));
				strTemp.Format ("%.0f", fTemp * atof (cTemp));
				strData.Format ("%10s", strTemp);
				strOut += strData;

				//	저가
				memset (cTemp, 0x00, 128);
				memcpy (cTemp, output.szBidLow, sizeof (output.szBidLow));
				strTemp.Format ("%.0f", fTemp * atof (cTemp));
				strData.Format ("%10s", strTemp);
				strOut += strData;

				//	종가
				memset (cTemp, 0x00, 128);
				memcpy (cTemp, output.szBidClose, sizeof (output.szBidClose));
				strTemp.Format ("%.0f", fTemp * atof (cTemp));
				strData.Format ("%10s", strTemp);
				strOut += strData;
				
				//	체결량
				strTemp.Format ("%.0f", fTemp * 0);
				strData.Format ("%10s", strTemp);
				strOut += strData;
	
				//	권리락 ( 일단 우리 서버데이터에는 없는데.. 여기에서 차트 넘겨주는 데이터에서 필요함... 나중에 조정해야할거 같음 )
				strOut += "0";
			}
			//>>> 데이타 처리 ===============================================
		
		} // end of for
		m_strOut = strOut;

#ifdef _WRITELOG
		WriteToStringLog("주식종목 서버로부터 받음_현재가::End", strCode, strCode.GetLength());
#endif
		*/
	}

}

HRESULT CStdDialog::DemoReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
/*
	CWnd *pWndRequested = NULL;
	CString strTRCode = szTR;
	strTRCode.Remove(' ');

	STTRINFO trInfo;
	long lCnt = m_arryQuery.GetSize();
	CString sTemp;

	for(long lPos=0;lPos<lCnt;lPos++)
	{
		trInfo = m_arryQuery.GetAt(lPos);
		if(trInfo.m_strTrCode==strTRCode)
		{

			pWndRequested = trInfo.m_pWndRequested;
			m_arryQuery.RemoveAt(lPos);

			lCnt = m_arryQuery.GetSize();
			break;
		}
	}

	//============================================================================
	if(strTRCode=="HWS09501")
	{	
		if(m_pRqWndFromTab)
		{			
			m_pRqWndFromTab->SendMessage(UMSG_RECEIVE_TR09501, 0, (long)aTRData);			
			SendMessage2StatusBar(MSG_SEARCH_END, (LPTSTR)(LPCTSTR)szMessage);
		}
		else
			SendMessage2StatusBar(MSG_SEARCH_FAIL, (LPTSTR)(LPCTSTR)szMessage);

		EnableWindowAll(TRUE);	

		return 0L;
	}
	else if(strTRCode == QUERY_STRMARKET)
	{								

		m_ChartMng.SendMessage( UMSG_RECV_MARKET_TR, (long)dwTRDateLen, (long)aTRData);

		SendMessage2StatusBar(MSG_SEARCH_END, (LPTSTR)(LPCTSTR)szMessage);

		EnableWindowAll(TRUE);	

		return 0L;
	}

	if(pWndRequested)
	{
		if(strTRCode==QUERY_STR_AVGMP)
		{
			CStringList strlistSetAll;
			char szWndChartItem[18];
			CChartItem *pWndChartItem = NULL;
			HWND hWndChartItem = NULL;
			D10418_O st10418o;
			memset(&st10418o, 0x00, sizeof(D10418_O));
			memcpy(&st10418o,aTRData,dwTRDateLen);
			st10418o.chNull = NULL;
			MEMCPYWITHNULL(szWndChartItem,st10418o.sBuffer);
			hWndChartItem = (HWND)atol(szWndChartItem);

			pWndChartItem = (CChartItem *)CWnd::FromHandle(hWndChartItem);
			if(pWndChartItem->GetSafeHwnd())
			{
				strlistSetAll.AddTail(GetAccountInfo());
				strlistSetAll.AddTail((char*)&st10418o.sShcode[0]);
				pWndChartItem->InvokeAddIn("FX_ChartGuideLineAddin.dll",INVOKEMSG_SETACCOUNTNQUOTECODE,(long)&strlistSetAll);
			}

			EnableWindowAll(TRUE);

		}	
		else
		{
			if(pWndRequested->GetSafeHwnd())
			{
				stReceiveFromServer stR;
				stR.m_strTRCode = strTRCode;
				stR.m_pBuffer = aTRData;
				stR.m_dwLength = dwTRDateLen;
				pWndRequested->SendMessage(UMSG_RECEIVEFROMSERVER,0,(long)&stR);
			}
		}
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
		if(pWndRequested)
		{
			stReceiveFromServer stR;	

			stR.m_strTRCode = szTR;
			stR.m_pBuffer = aTRData;
			stR.m_dwLength = dwTRDateLen;
			if(dwTRDateLen>=12)
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


	BOOL bRet = m_ChartMng.IsToBeReceivedMore();
	if(!m_arryQuery.GetSize()&&!bRet)
	{
		EnableWindowAll();
	}	

	SendMessage2StatusBar(MSG_SEARCH_END, (LPTSTR)(LPCTSTR)szMessage);
*/
	return S_OK;
}


/*
	TR결과를 받아서 처리하는 함수 - ok
*/
//<<20100308_JS.Kim 솔로몬
/*
HRESULT CStdDialog::ReceiveData (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, 
								 LPCSTR szMessage, char* pData, long dwTRDateLen)
{
	int nRequestPairTickState = GetRequestPairTickState ();

	CWnd		*pWndRequested	= NULL;
	CString		strTRCode		= szTR;
	strTRCode.Remove (' ');

	CString strMsgCode = szMsgCode;

	if (strMsgCode.GetLength () <= 0)	// There is no error
	{
		EnableWindowAll(TRUE);	
	}
	else								// error
	{
		if (strMsgCode.GetLength () > 0 || dwTRDateLen == 0)
		{
			if(!m_bEnable)
				EnableWindowAll(TRUE);	
		}
		return 1L;
	}

	if (strcmp(szTR, TR3930) == 0)
	{
		if (m_pDrdsLib == NULL)		MakeDrdsConnectAdviseFromCode (0, 0);
		EnableWindowAll (TRUE);
		m_ChartMng.SetOrderData (pData);
		m_pDrdsLib->DrdsAdvise (CHEGEUL_PID, "@", "", "EXECUTED");
		m_pDrdsLib->DrdsAdvise (MICHEGEUL_PID, "A", "", "PENDING");
	}
	else	// #560000
	{
		STTRINFO trInfo;
		long lCnt = m_arryQuery.GetSize ();
		CString sTemp;
		
		for (long lPos = 0; lPos < lCnt; lPos++)
		{
			trInfo = m_arryQuery.GetAt (lPos);
			if (trInfo.m_strTrCode == strTRCode)
			{
				pWndRequested = trInfo.m_pWndRequested;
				m_arryQuery.RemoveAt (lPos);
				lCnt = m_arryQuery.GetSize ();
				break;
			}
		}

		if (pWndRequested)
		{
			// 2007.04.25 by LYH 받은 코드(현재 CodeCombo 코드와 비교해서 다를 경우 재조회)
			if (m_strRecvCode == "" || m_pInputBarDlg->m_ctrlCodeInput.GetDataText () == "" || 
				m_ChartMng.IsInLoop () || m_pInputBarDlg->m_ctrlCodeInput.GetDataText () == m_strRecvCode)
			{
				if (pWndRequested->GetSafeHwnd ())
				{
	#ifdef _DEBUG
					CTime tm = CTime::GetCurrentTime ();
					CString strTime;
					strTime.Format ("\n\n### ReceiveData이후 - %d:%d:%d\n", tm.GetHour(), tm.GetMinute(), tm.GetSecond());
					AfxTrace (strTime);
	#endif
					// TR (x56000) 결과를 받아서 데이타 변환.
					ConvertKTBFXData (strTRCode, pData, dwTRDateLen);

					stReceiveFromServer stR;
					stR.m_strTRCode = strTRCode;
					stR.m_pBuffer = (LPVOID)(LPCTSTR)m_strOut;
					stR.m_dwLength = m_strOut.GetLength();



	#ifdef _DEBUG
					tm = CTime::GetCurrentTime ();
					strTime.Format ("\n\n### ConvertKTBFXData이후 - %d:%d:%d\n", tm.GetHour(), tm.GetMinute(), tm.GetSecond());
					AfxTrace (strTime);
	#endif
					//<<< 20091221 by kmlee
					UpdateCheJanOption ();
					//>>>
					pWndRequested->SendMessage (UMSG_RECEIVEFROMSERVER,0,(long)&stR);

	#ifdef _DEBUG
					tm = CTime::GetCurrentTime ();
					strTime.Format ("\n\n### RECEIVEFROMSERVER 메세지 처리후 - %d:%d:%d\n", tm.GetHour(), tm.GetMinute(), tm.GetSecond());
					AfxTrace (strTime);
	#endif
				}
				m_strRecvCode = "";
			}
			else
			{
				// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
				m_pInputBarDlg->SetKeyCodeChange (NID_CODEINPUT, (LPARAM)(LPCTSTR)m_pInputBarDlg->GetTypedItemCode());
			}
		}
		
		
		BOOL bRet = m_ChartMng.IsToBeReceivedMore ();

		if (!m_arryQuery.GetSize () && !bRet)
		{
			EnableWindowAll ();
		}	
	}
	
	// ******************************************************************
	SendMessage2StatusBar (MSG_SEARCH_END, (LPTSTR)(LPCTSTR)szMessage);
	// ******************************************************************

	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	switch (nRequestPairTickState)
	{
		case E_RPTS_START:	RequestPairTick (m_strPairTickCode);
							ReceiveData (dwKey,  szTR,  szMsgCode, szMessage, pData, dwTRDateLen);
							break;
		case E_RPTS_PAIR:	EndRequestPairTick();
							m_ChartMng.SetAddModeFromTab (m_bOrgAddMode);
							break;
		case E_RPTS_STOP:	EndRequestPairTick ();
							break;
	}
	
	return S_OK;
}
// <-- [Edit]  강지원 2008/07/25
*/
// --> [Edit]  강지원 2008/07/25		( 데이터를 넘길때 벡터가 아닌 그냥 Char로 넘김 )
HRESULT CStdDialog::ReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	if( strcmp(szTR, QUERY_STRFX) && strcmp(szTR, QUERY_STRFX_CHE) && strcmp(szTR, QUERY_STRFX_JAN) && strcmp(szTR, QUERY_STRFX_CHE2) )		
		return 0L;

	int nRequestPairTickState = GetRequestPairTickState ();

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
	//<<20100308_JS.Kim 솔로몬
	/*
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
	*/
	/*
	if (strTRCode == QUERY_STRFX_CHE || strTRCode == QUERY_STRFX_JAN)
	{
		CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrOpenPosition;
		CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrPendingOrder;
		
		FX_CHE_STRUCT* pCheSt = NULL;
		int nIdxData = 0;
		while(nIdxData < dwTRDateLen)
		{
			pCheSt = (FX_CHE_STRUCT *)((char *)aTRData+nIdxData);

			if( memcmp(pCheSt->szKeyType, "CH01", 4) == 0 )
				arrOpenPosition.Add(pCheSt);
			else
				arrPendingOrder.Add(pCheSt);
			nIdxData += sizeof(FX_CHE_STRUCT);
		}

		CChartItem* pChartItem = (CChartItem *)pWndRequested;
		if(pChartItem)
		{
			pChartItem->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 1, (long)&arrOpenPosition);
			pChartItem->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 2, (long)&arrPendingOrder);
		}
		return 0L;
	}
	*/
	if (strTRCode == QUERY_STRFX_JAN)		// 잔고 내역
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
	else if (strTRCode == QUERY_STRFX_CHE || strTRCode == QUERY_STRFX_CHE2)	// 미체결내역/체결내역
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
			if( strTRCode == QUERY_STRFX_CHE ) // 미체결내역은 OrderLine 에 보낸다.
				pChartItem->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 2, (long)&arrPendingOrder);
			else								// 체결내역은 진입/청산 표시 쪽에 보낸다.
				pChartItem->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 6, (long)&arrPendingOrder);
				//pChartItem->InvokeAddIn("ChartKoreaSorimachiAddIn.dll:USER_COMMENT", 2, (long)&arrPendingOrder);
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

				//<<< 20091221 by kmlee
				UpdateCheJanOption ();
				//>>>

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

	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	switch (nRequestPairTickState)
	{
		case E_RPTS_START:	RequestPairTick (m_strPairTickCode);
							ReceiveData (dwKey,  szTR,  szMsgCode, szMessage, aTRData, dwTRDateLen);
							break;
		case E_RPTS_PAIR:	EndRequestPairTick();
							m_ChartMng.SetAddModeFromTab (m_bOrgAddMode);
							break;
		case E_RPTS_STOP:	EndRequestPairTick ();
							break;
	}

	return S_OK;
}
//>>

HRESULT	CStdDialog::ReceiveMessage(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage)
{
	CString szTmp;
	szTmp.Format("[%s]%s\n", szMsgCode, szMessage);
	
	//<<20100308_JS.Kim 솔로몬
	/*
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
	*/
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
	//>>
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
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		}
	}
}

// 테스트모드이면 서버에서 받은 데이터 말고 서버에서 받은 데이터로 처리한다.
int CStdDialog::Send2Server(LPCTSTR lpTrCode,LPCTSTR lpData,int nSize,BOOL bWait, int nArrayIndex)
{
	int nTestMode = AfxGetApp()->GetProfileInt("TESTMODE", "HW", 0);

	SendMessage2StatusBar(MSG_GOING);	

	//<<< 20090904 by kmlee for test...
	// EnableWindowAll(FALSE);	
	//>>>
	
	if(m_TRComm)
	{
		if(CString(lpTrCode) == QUERY_STRMARKET)
		{
			p0607InBlock* pMarket = (p0607InBlock*)lpData;

			CString strMarketName, strNum;
			strMarketName.Format("%s", pMarket->pkName);
			if(strMarketName.Find("기관누적거래추이") >= 0)
			{
				strNum.Format("%d", m_pInputBarDlg->GetAllCount());
				memcpy(pMarket->num,strNum,sizeof(pMarket->num));
				memcpy(pMarket->nxtGb ,"0",sizeof(pMarket->nxtGb));
				memcpy(pMarket->nxtKey ,"              ",sizeof(pMarket->nxtKey));
			}
		}


		if(CString(lpTrCode) == QUERY_STRFX_JAN || CString(lpTrCode) == QUERY_STRFX_CHE || CString(lpTrCode) == QUERY_STRFX_CHE2)
		{
			m_TRComm->SetBaseData("SCREENCODE", m_strScreenNo);
			m_TRComm->SetTR(lpTrCode);
			m_TRComm->Send2Server((LPVOID)lpData, nSize, TRKEY_FID_END);
		}
		// 종목TR
		else
		{
			// (2008/9/14 - Seung-Won, Bae) for Pair Tick
			if( GetRequestPairTickState() != E_RPTS_PAIR)
			{
				m_TRComm->SetTR(lpTrCode);
				m_TRComm->SetBaseData("PtManager", m_szPTManager);
				m_TRComm->SetBaseData("SCREENCODE", m_strScreenNo);

	#ifdef _DEBUG
				CTime tm = CTime::GetCurrentTime();
				CString strTime;
				strTime.Format("\n\n### Send2Server - %d:%d:%d\n", tm.GetHour(), tm.GetMinute(), tm.GetSecond());
				AfxTrace(strTime);
	#endif
				return m_TRComm->Send2Server((LPVOID)lpData, nSize, TRKEY_FID_END);	
			}
//ㅁㅁㅁㅁㅁㅁㅁㅁㅁㅁ			else return 0;
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
	switch(message)
	{
	case UMSG_SEND2SERVER:
		{
			BOOL bWait = wParam;
			stSend2Server *pSt = (stSend2Server *)lParam;

			if( (CString(pSt->m_strTRCode) != QUERY_STRFX)			&&		//
				(CString(pSt->m_strTRCode) != QUERY_STRFX_CHE)		&&		// 미체결
				(CString(pSt->m_strTRCode) != QUERY_STRFX_CHE2)		&&		// 체결
				(CString(pSt->m_strTRCode) != QUERY_STRFX_JAN)		&&		// 잔고
				(CString(pSt->m_strTRCode) != QUERY_STR_AVGMP)			    // 이평 조회 
				)
				break;			

			if(pSt->m_strTRCode != QUERY_STRFX)
				memcpy(&m_st2Server, pSt, sizeof(stSend2Server));				

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
			if(m_pInputBarDlg)
			{
#ifdef _DEBUG
				CTime tm = CTime::GetCurrentTime();
				CString strTime;
				strTime.Format("\n\n### SENDSHAREDCODE이후 - %d:%d:%d\n", tm.GetHour(), tm.GetMinute(), tm.GetSecond());
				AfxTrace(strTime);
#endif

				STINPUTCODEDATA stInputCodeData;
				stInputCodeData.m_strCode = CString((LPCTSTR)wParam);
				stInputCodeData.m_strTime = "";
				stInputCodeData.m_nDefault = MODE_DEFAULT;
				stInputCodeData.m_nMode = CHANGE_ITEM;
				m_ChartMng.SendMessage(UMSG_INPUTCODETOCHARTMNG, MODE_DEFAULT, (LPARAM)&stInputCodeData);
				//	종목콤보에서 코드가 변경되었을경우 탭의 종목선택도 변경해준다.
				if( m_pLeftBarDlg )
				{
					CTabCodeSearch *pTabCodeSearch = (CTabCodeSearch*)m_pLeftBarDlg->m_tabctrl.GetWndCodeSearch();
					if(pTabCodeSearch)
					{
						pTabCodeSearch->SetCurListIndex(stInputCodeData.m_strCode);
					}
				}

#ifdef _DEBUG
				tm = CTime::GetCurrentTime();
				strTime.Format("\n\n### INPUTCODETOCHARTMNG 메세지 처리후 - %d:%d:%d\n", tm.GetHour(), tm.GetMinute(), tm.GetSecond());
				AfxTrace(strTime);
#endif
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
			m_pMsgBar->ShowMessage((LPCTSTR)wParam,(CWnd*)lParam);
		}
		break;
	case UMSG_EDITCODE_INIT:
		{
			if(m_pInputBarDlg)
				m_pInputBarDlg->SetCodeInit ();
		}
//		break;
	case UMSG_UPDATECODE:
		{
			//코스콤 종목공유 세팅(재정의)
			if(m_pInputBarDlg)
			{
				switch(lParam) {
				case 1:
					if(m_pInputBarDlg->m_nMarketType != CMasterTypeInfo::FX_CHART)
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
						//<<20100308_JS.Kim 솔로몬
						//m_pInputBarDlg->m_ctrlCodeInput.SetDataText( CStdDialog::GetRealItemCode( ( LPCTSTR)wParam));
						m_pInputBarDlg->m_pCodeInput->SetCode(CStdDialog::GetRealItemCode( ( LPCTSTR)wParam));
						m_pInputBarDlg->SetShareCodeChanged(FALSE);
						//>>

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
						if(m_pInputBarDlg->m_nMarketType != FX_CHART)
							return 1L;
						break;
					case 8:
					case 12:
						if(m_pInputBarDlg->m_nMarketType != FUTOPT_CHART)
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
			//::SendMessage(m_hViewInMain, RMSG_GET_LINKINFODATA, wParam, lParam);
			CString strItemCode = "";
			int	nOrderType = 2;
			if (!m_ChartMng.GetpChartItemSelected ()) 
				break;
			
			if (m_ChartMng.GetpChartItemSelected()->m_lpSelChart)
				strItemCode = GetRealItemCode(m_ChartMng.GetpChartItemSelected()->m_lpSelChart->m_strItemCode);
			else
				strItemCode = GetRealItemCode(m_ChartMng.GetpChartItemSelected()->GetDataCode());
			nOrderType = m_ChartMng.GetpChartItemSelected()->m_strOrderType == "0" ? 1 : 2;

			if( strItemCode.IsEmpty() )		break;

			CString strAccNo, strAccName, strAccPW;
			m_pExtInputBarDlg->GetAccountInfo(strAccNo, strAccName, strAccPW);
			ConfirmMap(strAccNo, strAccName, strAccPW,
						strItemCode, "", "1", "", "", "", nOrderType, FALSE);

// <-- [Edit]  강지원 2008/10/30
		}
		break;

	// FX차트 주문 연동 - onlyojt (20091204)
	//<<js.kim
	/*
	case UM_GETBROAD:
		{
			if(wParam != 4)		// wParam이 4인것만..
				return 0;
			
			FX_CHE_STRUCT* pOrderInfomation = (FX_CHE_STRUCT*)lParam;
			if(!pOrderInfomation)
				return 0;
			
			CString strCodeInputCode = m_pInputBarDlg->m_ctrlCodeInput.GetDataText();

			if(strCodeInputCode.IsEmpty())
				return 0;

			CString strAccount = "";
			if (m_hViewInMain)
				::SendMessage (m_hViewInMain, RMSG_GET_CHARTACCOUNT, 0L, (LPARAM)&strAccount);
			
			CString strItemCode (pOrderInfomation->SHCODE,	sizeof(pOrderInfomation->SHCODE));
			CString szAccNo (pOrderInfomation->szAccNo,		sizeof(pOrderInfomation->szAccNo));

			strItemCode.TrimRight ();
			strCodeInputCode.TrimRight ();
			strAccount.TrimRight ();
			szAccNo.TrimRight ();
			

			if (strCodeInputCode == strItemCode && strAccount == szAccNo)
			{
				CString strKeyType(pOrderInfomation->szKeyType, sizeof(pOrderInfomation->szKeyType));

				CChartItem* pChartItem = m_ChartMng.GetpChartItemSelected();
				if(pChartItem)
				{
					if(strKeyType == "CH01" || strKeyType == "CLSL")		// 체결
						pChartItem->InvokeAddIn("ChartCommonAddIn.dll:ORDER_LINE", 3, (long)pOrderInfomation);
					else if(strKeyType == "OR01" )	// 미체결
						pChartItem->InvokeAddIn("ChartCommonAddIn.dll:ORDER_LINE", 4, (long)pOrderInfomation);
				}
			}
		}
		break;
	// FX차트 주문 연동 - onlyojt (20091204)
	case UM_RESET_CHEJANINFO:
		{
			IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager, 2);
			if(!pKSLibMng)
				return 0;
			
			CString strCodeInputCode = m_pInputBarDlg->m_ctrlCodeInput.GetDataText();
			if(strCodeInputCode.IsEmpty())
				return 0;

			// 계좌번호
			CString strAccount = "";
			if (m_hViewInMain)
				::SendMessage (m_hViewInMain, RMSG_GET_CHARTACCOUNT, 0L, (LPARAM)&strAccount);

			if (strAccount.IsEmpty ())
				return 0;

			CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrOpenPosition;
			CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrPendingOrder;

			strAccount.TrimRight ();
			strCodeInputCode.TrimRight ();

			pKSLibMng->Lib_KSLink_GetOrderLineInfo(strAccount, strCodeInputCode, (CObArray*)&arrOpenPosition, (CObArray*)&arrPendingOrder, this);

			CChartItem* pChartItem = m_ChartMng.GetpChartItemSelected();
			if(pChartItem)
			{
				pChartItem->InvokeAddIn("ChartCommonAddIn.dll:ORDER_LINE", 1, (long)&arrOpenPosition);
				pChartItem->InvokeAddIn("ChartCommonAddIn.dll:ORDER_LINE", 2, (long)&arrPendingOrder);
			}
		}
		break;
		*/

	}
	return CExOCXDLL::WindowProc(message, wParam, lParam);
}

/*
	Function : 통신 키 초기화
	Paramter :
	Return	:
	Author	: By DRFN
	Comments	: FX Margin 하면서 소스 정리 (한국소리치 전용배)

	ok
*/
void CStdDialog::InitDrds()
{
	m_nDrdsInfoCnt = 1;

	m_DrdsInfo[0].DataName = "FX";								// FX
	m_DrdsInfo[0].FieldName = "*";
	m_DrdsInfo[0].KeyName = "";
	m_DrdsInfo[0].nID = FX_SISE_PID;
	m_DrdsInfo[0].bAdvise = FALSE;

	/*
	m_DrdsInfo[0].DataName = "a";								// 주식 (KXS3)
	m_DrdsInfo[0].FieldName = "*";
	m_DrdsInfo[0].KeyName = "";
	m_DrdsInfo[0].nID = STOCK_SISE_PID;
	m_DrdsInfo[0].bAdvise = FALSE;

	m_DrdsInfo[1].DataName = "JS0";								// 업종 (IXIJ)
	m_DrdsInfo[1].FieldName = "*";
	m_DrdsInfo[1].KeyName = "";
	m_DrdsInfo[1].nID = UPJONG_SISE_PID;
	m_DrdsInfo[1].bAdvise = FALSE;

	m_DrdsInfo[2].DataName = "SC0";								// 선물 (FFC0)
	m_DrdsInfo[2].FieldName = "*";
	m_DrdsInfo[2].KeyName = "";
	m_DrdsInfo[2].nID = FUTURE_SISE_PID;
	m_DrdsInfo[2].bAdvise = FALSE;

	m_DrdsInfo[3].DataName = "OC0";								// 옵션 (OOC0)
	m_DrdsInfo[3].FieldName = "*";
	m_DrdsInfo[3].KeyName = "";
	m_DrdsInfo[3].nID = OPTION_SISE_PID;
	m_DrdsInfo[3].bAdvise = FALSE;

	m_DrdsInfo[4].DataName = "X11";								// 호가잔량
	m_DrdsInfo[4].FieldName = "*";
	m_DrdsInfo[4].KeyName = "";
	m_DrdsInfo[4].nID = 3;										// 삼104;
	m_DrdsInfo[4].bAdvise = FALSE;

	m_DrdsInfo[5].DataName = "ELW";
	m_DrdsInfo[5].FieldName = "*";
	m_DrdsInfo[5].KeyName = "";
	m_DrdsInfo[5].nID = 105;
	m_DrdsInfo[5].bAdvise = FALSE;

	m_DrdsInfo[6].DataName = "FOR";								// 해외 (HWFJ)
	m_DrdsInfo[6].FieldName = "*";
	m_DrdsInfo[6].KeyName = "";
	m_DrdsInfo[6].nID = FOREIGN_SISE_PID;
	m_DrdsInfo[6].bAdvise = FALSE;
	*/
}

/*
	실시간 Advise하는 함수..
*/
BOOL CStdDialog::MakeDrdsConnectAdviseFromCode (int nI, BOOL bAddMod)
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
		// vntsorl_20110520:[A00000485] 리얼등록/해제 처리오류 수정
		if( m_RealTrCounter.FindCode(KeyName) == 1)
		{
			//@Solomon-->
			//m_pDrdsLib->DrdsInit(m_hWnd);
			//m_pDrdsLib->DrdsAdvise(nID, DataName, FieldName, KeyName);
			if(m_pPTManager) m_pPTManager->RequestMChartReal('A', DataName, KeyName);
			//@Solomon<--
			m_DrdsInfo[nI].bAdvise = TRUE;
		}
	}


	return TRUE;
}

// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
void CStdDialog::AddRealCodes()
{
}

// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
void CStdDialog::AddRealCodeList (LPCTSTR lpszCode)
{
	/*
	ST_CODEDATA oneCode;
	oneCode.m_strCode = lpszCode;

	int nMarketType = GetMarketTypeFromCode (lpszCode);
	BOOL bRet = FALSE;

	if(nMarketType == FX_CHART)
	{
		m_ListStockRealCodes.AddTail(oneCode);
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
		// vntsorl_20110520:[A00000485] 리얼등록/해제 처리오류 수정
		if( m_RealTrCounter.FindCode(KeyName) == 0)
		{
			//@Solomon
			//m_pDrdsLib->DrdsUnadvise(nID, DataName, FieldName, KeyName);
			if(m_pPTManager) m_pPTManager->RequestMChartReal('U', DataName, KeyName);
			m_DrdsInfo[nI].bAdvise = FALSE;
		}
	}

	return TRUE;
}

// 복수종목 실시간 처리 : 복수종목 - ojtaso (20070109)
void CStdDialog::OnCodeUnAdvise(CString strCode, BOOL bAddMode)
{
	// 기존 코드가 리스트에 있으면 삭제
	// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
	int nMarketType = GetMarketTypeFromCode(strCode);
	if(nMarketType == FX_CHART)
	{
		m_DrdsInfo[0].KeyName = strCode;
		MakeDrdsUnConnectUnAdviseFromCode(0, bAddMode);								
		// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
	}
}

// 복수종목 실시간 처리 : 복수종목 - ojtaso (20070109)
void CStdDialog::OnCodeAdvise(CString strCode, BOOL bAddMode)
{
	// Drds 연결 - 기존 코드가 리스트에 없으면 추가
	// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
	int nMarketType = GetMarketTypeFromCode(strCode);

	if(nMarketType == FX_CHART)
	{
		m_DrdsInfo[0].KeyName = strCode;
		MakeDrdsConnectAdviseFromCode(0, bAddMode);
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
	if( m_pLeftBarDlg )	m_pLeftBarDlg->SetPacketFromOutSide(strPacketFromOutSide);
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
	CRect rect(0, 0, 0, 0);
	CRect rectOriginal;
	if(m_bHideChart)
	{
		GetClientRect(rectOriginal);
		m_lWidthOriginal = rectOriginal.Width()+1;
		m_lHeightOriginal = rectOriginal.Height()+1;
		if( m_pLeftBarDlg )		m_pLeftBarDlg->GetClientRect(rect);
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
	if( m_pLeftBarDlg )
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
	if( m_pLeftBarDlg )
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
	if(!m_bEnable)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

/*
	Function : 초기 정보 초기화	
	Paramter :					
	Return	:					
	Author	: By DRFN			
	Comments	: FX Margin 하면서 소스 정리 (한국소리치 전용배)
*/
void CStdDialog::SetInitInfo ()
{
	int nID = 21;
	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
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

	// User Path 
	m_strUserDir.Format("%s%s", pManager->GetEncUserDir(), "\\Chart");
	if (_access(m_strUserDir, 0) == -1)
		::mkdir(m_strUserDir);

	_DoVersionCheck_ChartDef();
	_DoCheckDaultDataFile();

	// Image Path
	//solomon-090825
	m_szImageDir = m_strRootPath + "\\Icss\\Image\\drchart";

#ifdef __MULTI_LANG__
	//@Solomon-MultiLang:091117
	g_iMetaTable.m_strDataFolderPath.Format(_T("%s\\"), pManager->GetDataDir());
	g_iMetaTable.LoadMetaTable();
#endif
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
	m_TRComm->SetBaseData("SCREENCODE", m_strScreenNo);
	m_TRComm->Send2Server(st.m_pBuffer, st.m_lLength, FALSE);	
}

void CStdDialog::Request_StockHogaData(CString strRecvCode)
{
	if(m_ChartMng.m_pFloatingDlg == NULL) return;
	if(m_ChartMng.m_pFloatingDlg->GetSafeHwnd() == NULL ) return;
	if(m_ChartMng.m_pFloatingDlg->m_bIconState) return; //축소 상태면 TR조회 안한다.

	if(m_pInputBarDlg == NULL) return;
	if(m_pInputBarDlg->GetSafeHwnd() == NULL) return;

	CString strCode;

	if(strRecvCode.GetLength() == CODE_STOCK_LENGTH)
		strCode = strRecvCode;
	else
		// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
		strCode = m_pInputBarDlg->GetTypedItemCode();

	CString	strKmlee;
	strKmlee.Format ("Request_StockHogaData - %s", strCode);
	AfxMessageBox (strKmlee);

	if(!strCode.GetLength())
	{
		//호가창 Data Clear & Real 해제 ===================================
		Request_Disconnect_StockRealHogaData(m_pInputBarDlg->m_strOldCode);
		return;
	}

	strKmlee.Format ("Request_StockHogaData2 - %s", strCode);
	AfxMessageBox (strKmlee);

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

			char stJMCode[7];
			memset(&stJMCode,0x00,sizeof(stJMCode));			
			memcpy(stJMCode, strCode, strCode.GetLength());		
			
			CString strFidInfo("0                                                                               ");
			CString strTrInfo;
			strTrInfo.Format("%s20001", strCode);
			CString strRecordInfo("1               000060220221001002003004005006008122126130134138124128132136140162166170174178164168172176180202204207209061062222070075010092;");
			CString strOutput;
			strOutput = strFidInfo + strTrInfo + strRecordInfo;

			stSend2Server st;
			st.m_nType = 0;
			st.m_pWnd = (CWnd*)m_ChartMng.m_pFloatingDlg;
			st.m_pBuffer = (void*)(LPCTSTR)strOutput;
			st.m_lLength= strOutput.GetLength();
			st.m_strTRCode = QUERY_STOCK_HOGA;	
			SendMessage(UMSG_SEND2SERVER,0,(long)&st);

			//2005. 07. 15
			Request_StockRealHogaData(strCode);
		}
	}
}

/*
	필요한 부분인지 검토해 보자... by kmlee 20090921
*/
void CStdDialog::Request_StockRealHogaData(CString strCode)
{
	// 주식 Real Hoga 요청 
	if(strCode.GetLength())
	{
		strCode.TrimLeft();
		strCode.TrimRight();
		m_DrdsInfo[4].KeyName = strCode;
		// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
		MakeDrdsConnectAdviseFromCode(4, FALSE);		
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
				// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
				MakeDrdsUnConnectUnAdviseFromCode(4, FALSE);		
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
	CString strMessage;
	switch(nMsgType)
	{
		case	MSG_GOING:
			strMessage.Format("처리중...");
			break;
		default:
			if( lstrlen(pMsg) )
				strMessage.Format("%s", pMsg);
			break;
	}
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

/*
	ok
*/
BOOL CStdDialog::UnLoad_ReplayChart_Lib ()
{		
	if (m_hReplayChartLib != NULL)
	{
		if (m_pWndReplayLib)
			_fnReplayDll_UnLoad (m_pWndReplayLib);
		
		if (FreeLibrary (m_hReplayChartLib))
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
bool CStdDialog::SetImaginaryCandleToOutside(const bool bImaginaryCandle)
{ 
	m_ChartCfgInfo.bRealCandle	= bImaginaryCandle; // TRUE 면 허봉
	m_ChartMng.UpdateShowingFacked_JustOneChart(bImaginaryCandle, FALSE);

	//향후 환경 동기화 버튼이 토글 형태로 바뀔 경우 환경동기화 On일 경우 적용한다.
	//{
		m_pExtInputBarDlg->SendMessage(UMSG_SETNORMALIZEDMINUTETIMERULE,bImaginaryCandle);	
		m_ChartMng.SendMessage(UMSG_SETNORMALIZEDMINUTETIMERULE,bImaginaryCandle);	
	//}
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
	
	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.bottom = rcClient.top + 2;
	dc.FillSolidRect(rcClient, GetBkgrndColor());
}

/*
	Function : 차트 이미지 초기화
	Paramter :					
	Return	:					
	Author	: By DRFN			
	Comments	: FX Margin 하면서 소스 정리 (한국소리치 전용배)
*/
void CStdDialog::InitImageList ()
{
	BOOL bOK = FALSE;											// 버튼 이미지 로드 플래그

	CBitmap		bitmap;
	CString		strBitmapFile;	
	
	m_static_nChartCnt4ImgList++;								// 로드된 차트 갯수


	if (!m_ImgList_W15H15.m_hImageList)							// Width 15, Height 15 이미지 파일 로드
	{
		if (!m_ImgList_W15H15.Create (15, 14, ILC_MASK | ILC_COLORDDB, 1, 1))
		{
			AfxMessageBox ("Can not Find W15_H15.bmp file");			
		}
		strBitmapFile.Format ("%s\\%s", m_szImageDir, "W15_H15.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage (NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
										LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W15H15.Add (&bitmap, RGB (255, 0, 255));
		bitmap.DeleteObject ();
	}
	
	if (!m_ImgList_W20H20.m_hImageList)						// Width 20, Height 20 이미지 파일 로드
	{
		if (!m_ImgList_W20H20.Create (20, 20, ILC_MASK | ILC_COLORDDB, 1, 1))
		{
			AfxMessageBox ("Can not Find W20_H20.bmp file");
		}

		strBitmapFile.Format ("%s\\%s", m_szImageDir,  "W20_H20.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
									LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W20H20.Add (&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject ();
	}

	if (!m_ImgList_W20H20_Tool.m_hImageList)				// Width 15, Height 15 이미지 파일 로드(TOOL)
	{
		//<<< 20091111 by kmlee
		//if (!m_ImgList_W20H20_Tool.Create (19, 19, ILC_MASK | ILC_COLORDDB, 1, 1))
		if (!m_ImgList_W20H20_Tool.Create (20, 20, ILC_MASK | ILC_COLORDDB, 1, 1))
		//>>>
		{
			AfxMessageBox ("Can not Find W20_H20_Tool.bmp file");			
		}
		strBitmapFile.Format ("%s\\%s", m_szImageDir,  "W20_H20_Tool.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
									LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
		m_ImgList_W20H20_Tool.Add (&bitmap, RGB (255, 0, 255));
		bitmap.DeleteObject ();
	}

	if (!m_ImgList_W46H20.m_hImageList)						// Width 46, Height 20 이미지 파일 로드
	{
		if (!m_ImgList_W46H20.Create (46, 20, ILC_MASK | ILC_COLORDDB, 1, 1))
		{
			AfxMessageBox ("Can not Find W46_H20.bmp file");
		}
		strBitmapFile.Format ("%s\\%s", m_szImageDir,  "W46_H20.bmp");
		bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile, IMAGE_BITMAP, 0, 0, 
									LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));	
		m_ImgList_W46H20.Add (&bitmap, RGB(255, 0, 255));
		bitmap.DeleteObject ();						
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

	if ( !m_ImgList_W28H22.m_hImageList)						// Width 28, Height 22 이미지 파일 로드
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

/*
*/
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

void CStdDialog::AllCodeUnAdvise()
{
	for(int i = 0; i < m_nDrdsInfoCnt; ++i)
		MakeDrdsUnConnectUnAdviseFromCode (i, FALSE);

	IAUDrdsManager* pDrdsMng = (IAUDrdsManager*)AfxGetPctrInterface(UUID_IAUDrdsManager);
	if(!pDrdsMng) return;

	if(m_pDrdsLib)
	{
		RemoveDrdsFromCode (&m_ListStockRealCodes);
		m_pDrdsLib->DrdsClose ("");
		pDrdsMng->UnadviseDrdsLib (m_pDrdsLib);
		m_pDrdsLib = NULL;
	}

	m_RealTrCounter.RemoveAll();
}

void CStdDialog::Recv_ReplayRealData(void *pData, int nLen)
{
}

LONG CStdDialog::OnReplayDrds(WPARAM wp, LPARAM lp)
{
	if( (m_nMarketType != FX_CHART) && (m_nMarketType != FUTOPT_CHART))
		return 1L;

	CString strCode, strSpeed;
	strCode.Format("%s", (LPCTSTR)wp);

	m_ChartMng.SetReplayRealTitle(strCode, m_strSpeed, (TICK_DATA*)lp);
	return 1;
}

LONG CStdDialog::OnReplayFail(WPARAM wp, LPARAM lp)
{
	if( (m_nMarketType != FX_CHART) && (m_nMarketType != FUTOPT_CHART))
		return 1L;
	
	if(m_pInputBarDlg)
		m_pInputBarDlg->ReplayFail();

	return 1;
}

LONG CStdDialog::OnReplayChangeDate(WPARAM wp, LPARAM lp)
{
	if( (m_nMarketType != FX_CHART) && (m_nMarketType != FUTOPT_CHART))
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
	if(m_nMarketType == FX_CHART)
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
	if( (m_nMarketType != FX_CHART) && (m_nMarketType != FUTOPT_CHART))
		return 1L;

	return 1;
}

LONG CStdDialog::OnReplaySetTotalSec(WPARAM wp, LPARAM lp)
{
	if( (m_nMarketType != FX_CHART) && (m_nMarketType != FUTOPT_CHART))
		return 1L;
	
	if(m_pInputBarDlg)
		m_pInputBarDlg->SetReplayTotalSec(wp);

	return 1;
}

LONG CStdDialog::OnReplayEachSec(WPARAM wp, LPARAM lp)
{
	if( (m_nMarketType != FX_CHART) && (m_nMarketType != FUTOPT_CHART))
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
			//<<20100308_JS.Kim 솔로몬
			//m_pInputBarDlg->m_ctrlCodeInput.SetDataText( CStdDialog::GetRealItemCode( ( LPCTSTR)szOpenData));
			m_pInputBarDlg->m_pCodeInput->SetCode(CStdDialog::GetRealItemCode( ( LPCTSTR)szOpenData));
			m_pInputBarDlg->SetShareCodeChanged(FALSE);
			//>>
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

	//<< 솔로몬. FX 전용 차트이므로 시장 변경 필요없음
	//ChangeStockMode( GetMarketTypeFromCode( szCode));
	//>>

	STINPUTCODEDATA stInputCodeData;
	stInputCodeData.m_strCode = strSBCode;
	stInputCodeData.m_strTime = "";
	stInputCodeData.m_nDefault = nDefault;
	stInputCodeData.m_nMode = nMode;
	// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)
	m_ChartMng.SendMessage(UMSG_INPUTCODETOCHARTMNG, nDefault, (LPARAM)&stInputCodeData);
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
		HINSTANCE hInstResourceClient = AfxGetResourceHandle();
		AfxSetResourceHandle(g_hInstance);

		m_hCursor = AfxGetApp()->LoadCursor(IDC_HANDCUR);
		AfxSetResourceHandle(hInstResourceClient);
		::SetCursor(m_hCursor);
	}

	return 0L;
}

// 종목을 변경하거나 등등의 경우 데이터를 새로 받게 된다. 이때 초기화처리를 해줘야하는 루틴이있다.
// 시스템트레이딩의 전략이 실행되고 있을 때 이러한 변경에 대한 처리를 해줘야 한다.
void CStdDialog::NofityNewDataReceive()
{
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
	if(m_pInputBarDlg) // 2005. 04. 22 by Nam
		m_pInputBarDlg->SetViewCount(p_nDataCountInView);
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
	CRect rectLeftBar(0, 0, 0, 0);
	if( m_pLeftBarDlg )
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
	if( m_pLeftBarDlg )		m_pLeftBarDlg->Invalidate();
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
	if(pListRealCodes->GetCount() <= 0)
		return;

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
			//<<20100308_JS.Kim 솔로몬
			/*
			if(oneCode.m_bType == 2 || oneCode.m_bType == 4)
				m_TRComm->AddMainKey(DI_CODE, oneCode.m_strCode, C_DT_USHORT, 2);
			else
				m_TRComm->AddMainKey(DI_CODE, oneCode.m_strCode, C_DT_STRING, nCodeLength);
			
			m_TRComm->AddOptionKey(1, NULL, 5, 0, 0);
			m_TRComm->SetRealUpdateType(2);			//  RT_UPDATE	2	// 업데이트 타입
			//{{2007.06.26 by LYH 주식리얼 등록(예상체결 포함으로 수정)
			if(strTrCode == "20000")
			{
				m_TRComm->AddDataKey(15313);//현재가
				m_TRComm->AddDataKey(15009);//시가
				m_TRComm->AddDataKey(15010);//고가
				m_TRComm->AddDataKey(15011);//저가
				m_TRComm->AddDataKey(15317);//등락부호
				m_TRComm->AddDataKey(15316);//전일비
				m_TRComm->AddDataKey(15326);//등락률
				m_TRComm->AddDataKey(15315);//거래량
				m_TRComm->AddDataKey(DI_TIME);//체결시간
			}
			else
				//}}
			{
				m_TRComm->AddDataKey(15001);//현재가
				m_TRComm->AddDataKey(15009);//시가
				m_TRComm->AddDataKey(15010);//고가
				m_TRComm->AddDataKey(15011);//저가
				m_TRComm->AddDataKey(15006);//등락부호
				m_TRComm->AddDataKey(15472);//전일비
				m_TRComm->AddDataKey(15004);//등락률
				m_TRComm->AddDataKey(15015);//거래량
				m_TRComm->AddDataKey(DI_TIME);//체결시간
			}
			*/
			//>>
			nIndex++;
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

		if(strKey == oneCode.m_strCode)
		{
			pListRealCodes->RemoveAt(pos);
			break;
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

	int nMarketType = GetMarketTypeFromCode(lpszItemCode);

	if(nMarketType == FX_CHART)
	{
		RemoveRealCodes(&m_ListStockRealCodes, lpszItemCode);
		AddRealCodes("20000", &m_ListStockRealCodes);
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

/*
*/
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


/*
	실시간 데이타 수신 ok
*/
//<<20100308_JS.Kim 솔로몬
/*
long CStdDialog::OnGetBroad(WPARAM wParam, LPARAM lParam)
{
	REALDATA_st* lpSbData = (REALDATA_st*)lParam;
	LPREALDATA lpRealData = NULL;

	switch( lpSbData->chRealType )
	{
	case REAL_FX_SISE:
		{
			if ( !m_ChartMng )
				return 0L;
			BYTE *tmpBuff = NULL;
			tmpBuff = new BYTE [lpSbData->nSize + 1];
			memset (tmpBuff, 0x00, lpSbData->nSize + 1);
			memcpy (tmpBuff, lpSbData->pDataBuf, lpSbData->nSize);
			m_ChartMng.SetRealJPriceData ((LPVOID)tmpBuff);
			delete []tmpBuff;
			return 1L;
		}
		break;
	}
	return 0L;
}
*/

/*
	ok
*/
int CStdDialog::GetMarketTypeFromCode (CString strCode) 
{
	//<<20100308_JS.Kim 솔로몬
	int nType;
	return GetMarketTypeFromCode(strCode, nType);
	/*
	IMasterDataManager080331* pDataManager = (IMasterDataManager080331*)AfxGetPctrInterface(UUID_IMasterDataManager, 2);

	BYTE ucMarketType = pDataManager->GetMarketTypeFromCode (strCode);

	BOOL bRet = FALSE;
	int nLength = strCode.GetLength();

	if(		ucMarketType == 1	||		// 상장 종목
			ucMarketType == 2	||		// 코스닥 종목
			ucMarketType == 3	||		// 제3시장(OTCBB)
			ucMarketType == 4	||		// HIGHYIELD 펀드(수익증권)
			ucMarketType == 5	||		// 주식워런트 ELW
			ucMarketType == 6 )			// KDR
			return FX_CHART;
	else if( ucMarketType == 26 ||		// 장내업종
			 ucMarketType == 27 ||		// KOSPI 200 업종
			 ucMarketType == 28 ||		// KOSPI 100 업종
			 ucMarketType == 33 ||		// 코스닥 업종
			 ucMarketType == 34 ||		// 코스닥50 업종
			 ucMarketType == 36 ||		// 코스닥STAR 업종
			 ucMarketType == 59 ||		// 스타지수업종
			 ucMarketType == 60	||		// KRX
			 ucMarketType == 61	||		// KRX 섹터지수
			 ucMarketType == 62	)		// 프리보드업종
		return UPJONG_CHART;
	else if( ucMarketType == 21	||		// KOFEX 복합종목
			 ucMarketType == 24 ||		// 선물
			 ucMarketType == 25 ||		// 옵션
			 ucMarketType == 65 )		// 스프레드선물
		return FUTOPT_CHART;
	else if( ucMarketType == 64 ||		// 주식선물
			 ucMarketType == 38 )		// 주식옵션
		return STOCKFUTOPT_CHART;
	else if( ucMarketType == 42	||		// 미국지수
			 ucMarketType == 43	||		// 미국종가(미국종목)
			 ucMarketType == 44	||		// 미국상장 국내기업(ADR)
			 ucMarketType == 45	||		// 상품선물(원자재)
			 ucMarketType == 46	||		// CME선물
			 ucMarketType == 47	||		// 유럽상장 국내기업(GDR)
			 ucMarketType == 48	||		// 환율
			 ucMarketType == 49	||		// 금리
			 ucMarketType == 50	||		// 리보금리
			 ucMarketType == 51	||		// 주요국정부채
			 ucMarketType == 52	||		// 국내금리
			 ucMarketType == 53	||		// 반도체
			 ucMarketType == 54	||		// 세계주요종목
			 ucMarketType == 55	||		// ECN
			 ucMarketType == 56	)		// 세계주가지수(국가별)
		return FOREIGN_CHART;
	return FX_CHART;
	*/
	//>>
}

// 돋보기 구현 - ojtaso (20080519)
void CStdDialog::OnMagnifyingglass(BOOL bShow)
{
	if(bShow)
	{
		if(!m_pDlgGlass)	
		{
			m_pDlgGlass = new CDlgGlass(&m_ChartMng);
			
			HINSTANCE hInstResourceClient = AfxGetResourceHandle();
			AfxSetResourceHandle(g_hInstance);

			m_pDlgGlass->Create(IDD_MAGNIFYINGGLASS, &m_ChartMng);
	
			AfxSetResourceHandle(hInstResourceClient);
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

/*
	Form이 load 되면.. 현재 View에서도 보내고 있다..
*/
LRESULT CStdDialog::OnFormInitAfter(WPARAM wParam, LPARAM lParam)
{
	if (m_pInputBarDlg)
		m_pInputBarDlg->SetLastHistoryCode ();

	PostMessage (UMSG_EDITCODE_INIT);
	
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
	//<<20100308_JS.Kim 솔로몬
	/*
	IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager, 2);
	if(!pKSLibMng) return 0;

	m_clrBackground = pKSLibMng->Lib_KSLink_GetColor(1);	// 바탕화면 배경
	ChangeSkinColor();
	*/
	m_clrTabBkgrnd = m_clrBackground;
	m_clrOLBkgrnd = winix_GetColor(9);
	m_clrOLOutLine = winix_GetColor(10);
	
	g_pPtUtilMng->GetCurrentPalette();
	
	ChangeSkinColor();
	//>>

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
	SaveChart();
	return 0L;
}

LRESULT CStdDialog::OnFormSetPageSave(WPARAM wParam, LPARAM lParam)
{
	CString strKey = (LPCSTR)lParam;
	strKey.TrimRight();
	return 0L;
}

CString CStdDialog::GetLinkInfoData()
{
	CString strCode;
	if(m_hViewInMain)
		::SendMessage(m_hViewInMain, RMSG_GET_LINKINFODATA, (WPARAM)0, (LPARAM)&strCode);

	return strCode;
}

void CStdDialog::SaveChart()
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

	m_ChartMng.SaveChart(m_strConfig);
	if(m_nChartType != MINI_MULTI_CHART && m_nChartType != MINI_SINGLE_CHART)
		if( m_pLeftBarDlg )		m_pLeftBarDlg->SetConfig(FALSE);
	m_pInputBarDlg->SaveInputBar();
	if(m_nChartType == MULTI_CHART)
		m_pExtInputBarDlg->SaveExtInputBar();
}

/*
	ok
*/
void CStdDialog::GetOrderInfo()
{
	//<<20100308_JS.Kim 솔로몬
	/*
	IKSLibManager* pKSLibManager = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager, 2);

	CString	m_strIBNo		= pKSLibManager->Lib_KSLink_GetLoginInfo("IB_NO");			// 회원사 IB_NO(3)		: 회원사 IB_No
	CString m_strTradeType	= "00";														// 종목 구분 - 거래단위(00 : 10만, 01:1만)
	CString	m_strUserID		= pKSLibManager->Lib_KSLink_GetLoginInfo("USER_ID");		// ID(17)		: 사용자 IB_No

	EU_T3930_INREC Inrec;	

	memset(&Inrec, 0x00, sizeof(Inrec));
	memcpy(Inrec.szMemberNo, (LPSTR)(LPCTSTR)m_strIBNo, m_strIBNo.GetLength());
	memcpy(Inrec.szTradeType, (LPSTR)(LPCTSTR)m_strTradeType, m_strTradeType.GetLength());
	memcpy(Inrec.szCustNo, (LPSTR)(LPCTSTR)m_strUserID, m_strUserID.GetLength());

	Send2Server(TR3930, (char*)&Inrec, sizeof(Inrec), FALSE, 0);
	*/
	//>>
}

// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
CString	CStdDialog::GetRealItemCode( const char *p_szItemCode)
{
	if( !p_szItemCode) return "";
	if( !*p_szItemCode) return "";
	CString strItemCode( p_szItemCode);
	if( strItemCode.GetAt( 0) == '|') strItemCode = strItemCode.Mid( 3);
	return strItemCode;
}

// (2008/9/14 - Seung-Won, Bae) for Pair Tick
void CStdDialog::StartRequestPairTick( const char *p_strPairTickCode, int p_nRequestPairTickState)
{

	m_nRequestPairTickState = p_nRequestPairTickState;
	m_strPairTickCode = "|B_" + GetRealItemCode( p_strPairTickCode);
	m_ChartMng.GetpChartItemSelected()->SetCodeForQuery( m_strPairTickCode, QUERY_STRFX, TRUE);

	// (2008/9/24 - Seung-Won, Bae) delete C2_OBVIOUSLY_BALANCE_CHART in Tick
	m_ChartMng.GetpChartItemSelected()->RemoveChart( "일목균형표");

	//>> (2008/12/22 - Seung-Lyong, Park) 매수/도 데이터를 수신하는 각종 경우, 매수/도가 동시에 보이도록 개선
	m_ChartMng.GetpChartItemSelected()->LockWindowUpdate();
	//<< (2008/12/22 - Seung-Lyong, Park) 매수/도 데이터를 수신하는 각종 경우, 매수/도가 동시에 보이도록 개선
}
void CStdDialog::EndRequestPairTick( void)
{
	m_nRequestPairTickState = E_RPTS_STOP;
	//>> (2008/12/22 - Seung-Lyong, Park) 매수/도 데이터를 수신하는 각종 경우, 매수/도가 동시에 보이도록 개선
	if(m_ChartMng.GetpChartItemSelected() && m_ChartMng.GetpChartItemSelected()->GetSafeHwnd())
		m_ChartMng.GetpChartItemSelected()->UnlockWindowUpdate();
	//<< (2008/12/22 - Seung-Lyong, Park) 매수/도 데이터를 수신하는 각종 경우, 매수/도가 동시에 보이도록 개선
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

CString CStdDialog::GetOrderType(){
	return m_pInputBarDlg->GetOrderType();
}

// FX차트 주문 연동 - onlyojt (20091204)
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
		strAccount.TrimRight();
		if (strAccount.IsEmpty ())
		{
			MessageBox("계좌번호를 입력해 주십시오", "차트주문");
			return;
		}
		if( strAccPW.GetLength() < 4 )
		{
			MessageBox("비밀번호를 입력해 주십시오", "차트주문");
			m_pExtInputBarDlg->m_edAccPW.SetFocus();
			return;
		}


		m_ChartMng.GetpChartItemSelected()->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 5, 0);
		//m_ChartMng.GetpChartItemSelected()->InvokeAddIn("ChartKoreaSorimachiAddIn.dll:USER_COMMENT", 5, 0);

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
		// 미체결조회
		Input.szChGB[0] = '2';
		st.m_strTRCode = QUERY_STRFX_CHE;	
		SendMessage(UMSG_SEND2SERVER,0,(long)&st);
		// 잔고조회
		st.m_strTRCode = QUERY_STRFX_JAN;	
		SendMessage(UMSG_SEND2SERVER,0,(long)&st);

		// 체결조회
		Input.szChGB[0] = '1';
		st.m_strTRCode = QUERY_STRFX_CHE2;	
		SendMessage(UMSG_SEND2SERVER,0,(long)&st);

		//CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrOpenPosition;
		//CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrPendingOrder;
		//pKSLibMng->Lib_KSLink_GetOrderLineInfo(strAccount, strCodeInputCode, (CObArray*)&arrOpenPosition, (CObArray*)&arrPendingOrder, this);

		//<<20100308_JS.Kim 솔로몬
		/*
		IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface (UUID_IKSLibManager, 2);
		if(!pKSLibMng)
			return;
		
		// 종목
		CString strCodeInputCode = m_pInputBarDlg->m_ctrlCodeInput.GetDataText ();
		if (strCodeInputCode.IsEmpty ())
			return;

		// 계좌번호
		CString strAccount = "";
		if (m_hViewInMain)
			::SendMessage (m_hViewInMain, RMSG_GET_CHARTACCOUNT, 0L, (LPARAM)&strAccount);

		if (strAccount.IsEmpty ())
			return;
		
		CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrOpenPosition;
		CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrPendingOrder;
		pKSLibMng->Lib_KSLink_GetOrderLineInfo(strAccount, strCodeInputCode, (CObArray*)&arrOpenPosition, (CObArray*)&arrPendingOrder, this);
		
		CChartItem* pChartItem = m_ChartMng.GetpChartItemSelected();
		if(pChartItem)
		{
			pChartItem->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 1, (long)&arrOpenPosition);
			pChartItem->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 2, (long)&arrPendingOrder);
		}
		*/
		////////////////////>>
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


// FX차트 주문 연동 - onlyojt (20091204)
// 매수/매도/정정/취소 -- 주문유형[nOrderType]
/*
	nOrderType
		[0] = 시장가 주문
		[1] = 지정가 주문
		[2] = 정정
		[3] = 취소
		[4] = stop/limit 적용 (미체결)
		[5] = stop/limit 취소 (미체결)
		[6] = 청산
		[7] = stop/limit 적용 (미청산)
		[8] = stop/limit 취소 (미청산)
		[9] = Stop 적용 미체결
		[10] = Limit 적용 미체결
		[11] = Stop 적용 미청산
		[12] = Limit 적용 미청산
		[13] = 역지정가 주문

	szOrgOrderID = 신규주문일 경우엔 값이 없고, 나머지는 원주문번호가 있다.
*/
BOOL CStdDialog::OnOrderInfomationChanged(const double dPirce, const int Qty, const int nOrderType, LPCTSTR szOrgOrderID,

							LPCTSTR	szCustItem1,			/*  [회원처리항목 1] or 매도/매수 구분(0:매도,1:매수)	*/
							LPCTSTR	szCustItem2,			/*  [회원처리항목 2]  주문번호							*/
							LPCTSTR	szCustItem3				/*  [회원처리항목 3]									*/
						)
{
	if (!m_ChartMng.GetpChartItemSelected ()) 
		return FALSE;


	CString strItemCode = "";
	CString strAccNo = "", strAccName = "", strAccPW = "", strFDM = "", strOrderType = "시장가", strPipLowest = "";
	CString strPrice = "", strQty = "", strStopPrice = "";
	int	nMMGB = 2;
	int nDecPos(0);

	m_pExtInputBarDlg->GetAccountInfo(strAccNo, strAccName, strAccPW);

	strAccNo.TrimRight();
	if (strAccNo.IsEmpty ())
	{
		MessageBox("계좌번호를 입력해 주십시오", "차트주문");
		return FALSE;
	}

	if( strAccPW.GetLength() < 4 )
	{
		MessageBox("비밀번호를 입력해 주십시오", "차트주문");
		m_pExtInputBarDlg->m_edAccPW.SetFocus();
		return FALSE;
	}

	if (m_ChartMng.GetpChartItemSelected()->m_lpSelChart)
		strItemCode = m_ChartMng.GetpChartItemSelected()->m_lpSelChart->m_strItemCode;
	else
		strItemCode = m_ChartMng.GetpChartItemSelected()->GetDataCode();
	
	ST_CHARTITEM_REALDATA* pChartItemRealData = m_ChartMng.GetpChartItemSelected()->GetChartItemRealData(strItemCode);
	if( pChartItemRealData )
	{
		strPipLowest = pChartItemRealData->m_strPipLowest;
		if ( strPipLowest.Find('.') >= 0 )
			nDecPos = strPipLowest.GetLength() - strPipLowest.Find(".") - 1;
	}

	strItemCode = GetRealItemCode(strItemCode);
	nMMGB = m_ChartMng.GetpChartItemSelected()->m_strOrderType == "0" ? 0 : 1;

	if( strItemCode.IsEmpty() )		return FALSE;

/*
	if( strItemCode.GetLength() > 2 )
	{
		if( strItemCode.Right(2) == ".I" )
		{
			strFDM = "IBFX";
			strItemCode = strItemCode.Left(strItemCode.GetLength()-2);
		}
		else if( strItemCode.Right(2) == ".H" )
		{
			strFDM = "HMWR";
			strItemCode = strItemCode.Left(strItemCode.GetLength()-2);
		}
		strItemCode.Replace('/', '-');
	}
*/
	strItemCode.Replace('/', '-');
	
	strQty.Format("%d", Qty);

	switch( nOrderType )
	{
	case ORDERTYPE_MARKET:
		strOrderType = "시장가";
		if( szCustItem1 )		nMMGB = atoi(szCustItem1);
		break;
	case ORDERTYPE_ENTRY:			// 지정가
	case ORDERTYPE_CROSS_ENTRY:		// 역지정가
		strOrderType = "지정가";
		if( szCustItem1 )		nMMGB = atoi(szCustItem1);
		if( nDecPos > 0 )	strPrice.Format("%.*f", nDecPos, dPirce);
		else				strPrice.Format("%f", dPirce);
		break;
	case ORDERTYPE_ENTRY2:			// double click (지정가에 매도/매수 동시 주문화면)
		strOrderType = "지정가";
		if( nDecPos > 0 )	strPrice.Format("%.*f", nDecPos, dPirce);
		else				strPrice.Format("%f", dPirce);
		break;

	case ORDERTYPE_STOP:		
	case ORDERTYPE_CON_STOP:		//미청산 Stop/Limit 적용
		strOrderType = "Stop";
		nMMGB = 6;
		if( nDecPos > 0 )	strStopPrice.Format("%.*f", nDecPos, dPirce);
		else				strStopPrice.Format("%f", dPirce);
		break;
	case ORDERTYPE_CHANGE:
		nMMGB = 2;
		strOrderType = "지정가";
		if( nDecPos > 0 )	strPrice.Format("%.*f", nDecPos, dPirce);
		else				strPrice.Format("%f", dPirce);
		break;
	case ORDERTYPE_STOP_ONLY:		//Stop 적용 미체결
	case ORDERTYPE_LIMIT_ONLY:		//Limit 적용 미체결
		nMMGB = 2;
		strOrderType = "Stop";
		if( nDecPos > 0 )	strStopPrice.Format("%.*f", nDecPos, dPirce);
		else				strStopPrice.Format("%f", dPirce);
		strPrice = strStopPrice;
		break;
	case ORDERTYPE_CANCEL:
	case ORDERTYPE_LIMIT:			//Stop/Limit 취소
		nMMGB = 3;
		break;
	case ORDERTYPE_CLEAR:		//주문청산
		nMMGB = 5;
		break;

	case ORDERTYPE_CON_LIMIT :		//미청산 Stop/Limit 취소
		nMMGB = 6;
		break;
	}

	CString strCustItem2 = szCustItem2;
	strCustItem2.TrimLeft();		strCustItem2.TrimRight();
	ConfirmMap(	strAccNo,		//	CString strAccount		: 계좌번호
				strAccName,		//  CString strAccountName	: 계좌명
				strAccPW,		//	CString strPassword		: 비밀번호
				strItemCode,	//	CString strCodeName		: 종목
				strFDM,			//	CString strFDM 			: FDM 
				strOrderType,	//	CString strOrderType	: 주문구분(보통/시장가 등...)
				strQty,			//	CString strQuantity		: 수량
				strPrice,		//	CString strPrice		: 가격
				strCustItem2,	//	CString strJmNum		: 주문번호
				strStopPrice,	//	CString strStopPrice	:
				nMMGB,			//	int nOrderType			: (0-매도 1-매수 2-정정 3-취소 4-T.STOP 5-청산 6-STOP/LIMIT)
				nOrderType == ORDERTYPE_ENTRY2 ? 1 : 0 );	// int nMapType : 주문화면 종류


//<<20100308_JS.Kim 솔로몬
	/*
	CString strMsg;

	CString		strItem1 = szCustItem1;
	CString		strItem2 = szCustItem2;
	CString		strItem3 = szCustItem3;

	//<<< for test...
	strMsg.Format("kmlee : OnOrderInfomationChanged %f, %d, %d, %s", dPirce, Qty, nOrderType, szOrgOrderID);
	// AfxMessageBox(strMsg);
	OutputDebugString (strMsg);
	//>>>

	CString		strOrderNo = szOrgOrderID;
	FX_CHE_STRUCT	*pInfo = NULL;					// 주문 정보 찾기..

	// 종목코드
	CString strCodeInputCode = m_pInputBarDlg->m_ctrlCodeInput.GetDataText();
	strCodeInputCode.TrimRight ();
	

	CString strCode = CStdDialog::GetRealItemCode(m_pInputBarDlg->GetCurrentCode ());
	CString	strCodeName = m_pInputBarDlg->GetJongMokNameByCode (strCode);
	
	CString strAccount = "";
	if (m_hViewInMain)
		::SendMessage (m_hViewInMain, RMSG_GET_CHARTACCOUNT, 0L, (LPARAM)&strAccount);
	strAccount.TrimRight ();

	if (strCodeInputCode.IsEmpty () || strAccount.IsEmpty ())
	{
		OutputDebugString ("계좌번호가 없거나, 종목코드가 없습니다.!!");
		return FALSE;

	}

//************************************************************
// [ST]체결/미체결 리스트에서 데이타 검색하기
//************************************************************
	IKSLibManager* pKSLibMng = NULL;
	CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrOpenPosition;
	CArray<FX_CHE_STRUCT*, FX_CHE_STRUCT*> arrPendingOrder;

	// 신규주문이 아니면.. 기존 체결/
	if (nOrderType != ORD_TYPE_MARKETORDER && nOrderType != ORD_TYPE_ORDER && nOrderType != ORD_TYPE_ORDER_REVERSE)
	{
		pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager, 2);
		if (pKSLibMng)
		{
			pKSLibMng->Lib_KSLink_GetOrderLineInfo(strAccount, strCodeInputCode, (CObArray*)&arrOpenPosition, (CObArray*)&arrPendingOrder, this);
		}
		else
		{
			OutputDebugString ("pKSLibMng == NULL!!");
			return FALSE;
		}

	}

	if (nOrderType == ORD_TYPE_MODIFY ||						// 정정
		nOrderType == ORD_TYPE_CANCEL ||						// 취소
		nOrderType == ORD_TYPE_STOPLIMIT_APLY1 ||				// stop/limit 적용 (미체결)
		nOrderType == ORD_TYPE_STOPLIMIT_CANCEL1 ||				// stop/limit 취소 (미체결)
		nOrderType == ORD_TYPE_STOP_APPLY3 ||					// stop 적용 (미체결)
		nOrderType == ORD_TYPE_LIMIT_APPLY3)					// limit 적용 (미체결)
	{
		// 미체결내역에서 데이타 찾기..
		for (int i = 0; i < arrPendingOrder.GetSize (); i++)
		{
			FX_CHE_STRUCT	*pInfoTemp = NULL;
			pInfoTemp = (FX_CHE_STRUCT *)arrPendingOrder.GetAt (i);

			if (pInfoTemp)
			{
				if (!memcmp (szCustItem1, pInfoTemp->szCustItem1, sizeof (pInfoTemp->szCustItem1)) &&
					!memcmp (szCustItem2, pInfoTemp->szCustItem2, sizeof (pInfoTemp->szCustItem2)) &&
					!memcmp (szCustItem3, pInfoTemp->szCustItem3, sizeof (pInfoTemp->szCustItem3)))
				{
					pInfo = pInfoTemp;
					break;
				}
			}
		}
	}
	else if (nOrderType == ORD_TYPE_CLEAR ||					// 청산
			 nOrderType == ORD_TYPE_STOPLIMIT_APLY2 ||			// stop/limit 적용 (미청산)
			 nOrderType == ORD_TYPE_STOPLIMIT_CANCEL2 ||		// stop/limit 취소 (미청산)
			 nOrderType == ORD_TYPE_STOP_APPLY4 ||				// stop 적용 (미청산)
			 nOrderType == ORD_TYPE_LIMIT_APPLY4 )				// limit 적용 (미청산)
	{
		// 체결내역에서 데이타 찾기..
		for (int i = 0; i < arrOpenPosition.GetSize (); i++)
		{
			FX_CHE_STRUCT	*pInfoTemp = NULL;
			pInfoTemp = (FX_CHE_STRUCT *)arrOpenPosition.GetAt (i);
			if (pInfoTemp)
			{
				if (!memcmp (szCustItem1, pInfoTemp->szCustItem1, sizeof (pInfoTemp->szCustItem1)) &&
				    !memcmp (szCustItem2, pInfoTemp->szCustItem2, sizeof (pInfoTemp->szCustItem2)) &&
				    !memcmp (szCustItem3, pInfoTemp->szCustItem3, sizeof (pInfoTemp->szCustItem3)))
				{
					pInfo = pInfoTemp;
					break;
				}
			}
		}
	
	}

	if( !pInfo && nOrderType != ORD_TYPE_MARKETORDER && nOrderType != ORD_TYPE_ORDER && nOrderType != ORD_TYPE_ORDER_REVERSE)
	{
		AfxMessageBox("주문정보를 불러올 수 없습니다. 미체결 및 미청산 내역을 확인하여 주시기 바랍니다.");
		return 0;
	}

	//<<< for test...
	if (pInfo)
	{
		strMsg.Format ("주문타입[%d]", nOrderType); 
		OutputDebugString (strMsg);
	}
	//>>>
//************************************************************
// [END]체결/미체결 리스트에서 데이타 검색하기
//************************************************************
			
	// 주문화면 로딩
	//<<< 20091212 by kmlee
	HWND		hSignalWnd = (HWND)KSLink_GetEnvInfo (2);

	if (hSignalWnd == NULL)
		return 0;

	// main으로 전달할 주문정보 자료구조..
	SIGNALORDERSINFO	signalOrdersInfo;
	memset (&signalOrdersInfo, 0, sizeof (SIGNALORDERSINFO));

	// 가격 취득
	CString strPrecision, strTickSize;
	dll_GetUnitInfo (strCode, "", strPrecision, strTickSize);
	strPrecision.Format ("%%.%df", atoi (strPrecision));

	signalOrdersInfo.hDLLWnd = (long)GetSafeHwnd ();			// 호출한 컨트롤 핸들

	strcpy (signalOrdersInfo.szIndexNo,		"20");
	strcpy (signalOrdersInfo.szWindName,	"FX챠트");		 
	strcpy (signalOrdersInfo.szSignalgb,	"감"); 
	strcpy (signalOrdersInfo.szPopgb,		"2");
	strcpy (signalOrdersInfo.szActionGb,	"1");

	strcpy (signalOrdersInfo.szAcctno,		strAccount);
	strcpy (signalOrdersInfo.szCode,		strCode);

	// 시장가[0], 지정가[1], 정정[2], 취소[3], stop/limit 적용 (미체결)[4]
	// stop/limit 취소 (미체결)[5], 청산[6], stop/limit 적용 (미청산)[7], 
	// stop/limit 취소 (미청산) [8]
	switch (nOrderType)
	{
	case ORD_TYPE_MARKETORDER :							// 신규시장가
		
		strcpy (signalOrdersInfo.szMemegb, "0");
		strcpy (signalOrdersInfo.szJanGogb, "0");		// 매수
		if (dPirce == 0)
			strcpy (signalOrdersInfo.szOrdgb, "2");		// 시장가
		else
			strcpy (signalOrdersInfo.szOrdgb, "0");		// 지정가
		break;						

	case ORD_TYPE_ORDER :								// 신규지정가

		strcpy (signalOrdersInfo.szMemegb, "0");
		strcpy (signalOrdersInfo.szJanGogb, "0");		// 매수
		if (dPirce == 0)
			strcpy (signalOrdersInfo.szOrdgb, "2");		// 시장가
		else
			strcpy (signalOrdersInfo.szOrdgb, "0");		// 지정가
		break;

	case ORD_TYPE_ORDER_REVERSE:
		strcpy (signalOrdersInfo.szMemegb, "0");
		strcpy (signalOrdersInfo.szJanGogb, "0");		// 매수
		strcpy (signalOrdersInfo.szOrdgb, "1");			// 역지정가

		break;

	case ORD_TYPE_MODIFY :										// 정정

		strcpy (signalOrdersInfo.szMemegb,	"4");
		strcpy (signalOrdersInfo.szOrdgb,	"1");				// 정정
		if (memcmp (pInfo->szSide, "079", 3) == 0)
			strcpy (signalOrdersInfo.szJanGogb,	"0");			// 매수 정정
		else
			strcpy (signalOrdersInfo.szJanGogb,	"1");			// 매도 정정
		strcpy (signalOrdersInfo.szMsg, szOrgOrderID);			// 원주문번호
		break;

	case ORD_TYPE_CANCEL :										// 취소

		strcpy (signalOrdersInfo.szMemegb,	"4");
		strcpy (signalOrdersInfo.szOrdgb,	"0");				// 취소
		if (memcmp (pInfo->szSide, "079", 3) == 0)
			strcpy (signalOrdersInfo.szJanGogb,	"0");			// 매수 취소
		else
			strcpy (signalOrdersInfo.szJanGogb,	"1");			// 매도 취소
		strcpy (signalOrdersInfo.szMsg, szOrgOrderID);			// 원주문번호
		break;

	case ORD_TYPE_STOPLIMIT_APLY1 :								// stop/limit 적용 (미체결)
	case ORD_TYPE_STOPLIMIT_APLY2 :								// stop/limit 적용 (미청산)
	case ORD_TYPE_STOP_APPLY3:
	case ORD_TYPE_LIMIT_APPLY3:
	case ORD_TYPE_STOP_APPLY4:
	case ORD_TYPE_LIMIT_APPLY4:

		strcpy (signalOrdersInfo.szMemegb,	"3");
		strcpy (signalOrdersInfo.szOrdgb, "0");					// 적용 (추가, 신규)
		if (memcmp (pInfo->szKeyType, "OR01", 4) == 0)
		{
			strcpy (signalOrdersInfo.szJanGogb,	"0");			// 미체결
			strcpy (signalOrdersInfo.szMsg, szOrgOrderID);		// 원주문번호
		}
		else
		{
			strcpy (signalOrdersInfo.szJanGogb,	"1");			// 미청산
			memcpy (signalOrdersInfo.szMsg, pInfo->szFXCMPosID, sizeof (pInfo->szFXCMPosID));	// 체결번호
		}
		if (memcmp (pInfo->szSide, "079", 3) == 0)
			strcpy (signalOrdersInfo.szSygb,	"0");			// 매수 취소
		else
			strcpy (signalOrdersInfo.szSygb,	"1");			// 매도 취소

		break;

	case ORD_TYPE_STOPLIMIT_CANCEL1 :							// stop/limit 취소 (미체결)
	case ORD_TYPE_STOPLIMIT_CANCEL2 :							// stop/limit 취소 (미청산)

		strcpy (signalOrdersInfo.szMemegb,	"3");
		strcpy (signalOrdersInfo.szOrdgb,	"2");				// 취소
		if (memcmp (pInfo->szKeyType, "OR01", 4) == 0)
		{
			strcpy (signalOrdersInfo.szJanGogb,	"0");			// 미체결
			strcpy (signalOrdersInfo.szMsg, szOrgOrderID);		// 원주문번호
		}
		else
		{
			strcpy (signalOrdersInfo.szJanGogb,	"1");			// 미청산
			memcpy (signalOrdersInfo.szMsg, pInfo->szFXCMPosID, sizeof (pInfo->szFXCMPosID));	// 체결번호
		}
		break;

	case ORD_TYPE_CLEAR :										// 청산

		strcpy (signalOrdersInfo.szMemegb,	"2");
		strcpy (signalOrdersInfo.szJanGogb,	"1");				// 청산일 경우는 항상 '0'

		if (memcmp (pInfo->szSide, "079", 3) == 0)
			strcpy (signalOrdersInfo.szOrdgb,	"0");			// 매수 청산
		else
			strcpy (signalOrdersInfo.szOrdgb,	"1");			// 매도 청산
		memcpy (signalOrdersInfo.szMsg, pInfo->szFXCMPosID, sizeof (pInfo->szFXCMPosID));	// 체결번호
		break;

	}

	// 주문번호 Trim..
	CString szTempOrdNo = signalOrdersInfo.szMsg;
	szTempOrdNo.TrimRight ();
	strcpy (signalOrdersInfo.szMsg, szTempOrdNo.operator LPCTSTR  ());

	sprintf (signalOrdersInfo.szOrdqty, "%d", Qty);
	// 주문가격
	if( nOrderType == ORD_TYPE_MARKETORDER || nOrderType == ORD_TYPE_ORDER || nOrderType == ORD_TYPE_ORDER_REVERSE || 
		nOrderType == ORD_TYPE_MODIFY || nOrderType == ORD_TYPE_CANCEL || nOrderType == ORD_TYPE_CLEAR)
	{
		// 시장가, 지정가, 정정, 취소, 청산일때는 szOrdprc에 이동가격을 넣어준다
		sprintf (signalOrdersInfo.szOrdprc,	strPrecision, dPirce);
	}
	else if( nOrderType == ORD_TYPE_MODIFY || nOrderType == ORD_TYPE_STOPLIMIT_CANCEL1 || 
			nOrderType == ORD_TYPE_STOPLIMIT_APLY2 || nOrderType == ORD_TYPE_STOPLIMIT_CANCEL2 ||
			nOrderType == ORD_TYPE_STOPLIMIT_APLY1 || nOrderType == ORD_TYPE_STOPLIMIT_CANCEL1 )
	{
		// Stop/Limit 정정/취소일때는 이동가격은 상관없고 주문환율을 넣어주어야함
		memcpy (signalOrdersInfo.szOrdprc,	pInfo->szRate,  sizeof (pInfo->szRate));

		memcpy (signalOrdersInfo.szOrdStop,		pInfo->szStop,  sizeof (pInfo->szStop));		// stop
		memcpy (signalOrdersInfo.szOrdLimit,	pInfo->szLimit, sizeof (pInfo->szLimit));		// limit
	}
	else if( nOrderType == ORD_TYPE_STOP_APPLY3 || nOrderType == ORD_TYPE_STOP_APPLY4 )
	{
		// Stop 적용일때는 주문환율 입력
		memcpy (signalOrdersInfo.szOrdprc,	pInfo->szRate,  sizeof (pInfo->szRate));
		sprintf (signalOrdersInfo.szOrdStop,	strPrecision, dPirce);
	}
	else if( nOrderType == ORD_TYPE_LIMIT_APPLY3 || nOrderType == ORD_TYPE_LIMIT_APPLY4 )
	{
		// Stop 적용일때는 주문환율 입력
		memcpy (signalOrdersInfo.szOrdprc,	pInfo->szRate,  sizeof (pInfo->szRate));
		sprintf (signalOrdersInfo.szOrdLimit,	strPrecision, dPirce);
	}

// 	if (nOrderType != ORD_TYPE_MARKETORDER && nOrderType != ORD_TYPE_ORDER)
// 	{
// 		memcpy (signalOrdersInfo.szOrdStop,		pInfo->szStop,  sizeof (pInfo->szStop));		// stop
// 		memcpy (signalOrdersInfo.szOrdLimit,	pInfo->szLimit, sizeof (pInfo->szLimit));		// limit
// 		sprintf (signalOrdersInfo.szOrdTSPip, "%ld", pInfo->nTrailingStop);						// Trailing Stop Pip
// 	}

	strcpy (signalOrdersInfo.szExecRangeGb,	"0");			// 주문유효기간(체결범위, AtBest : 0, Range : 1)
	strcpy (signalOrdersInfo.szExecRange,	"0");			// 주문유효기간(체결범위 Spin값)

	::SendMessage(hSignalWnd, UM_SM_SIGNALORDER_MSG, (WPARAM)0L, (long)&signalOrdersInfo);
*/
//>>
	return TRUE;
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

//<<< 20100107 by kmlee
BOOL CStdDialog::DoOrderFromSTSignal (WPARAM wp, LPARAM lp)
{
//<<20100308_JS.Kim 솔로몬  
	/*
	STORDER_INDATA2* lpOrderData = (STORDER_INDATA2*)wp;
	CSTConfigEnv* lpConfigEnv = (CSTConfigEnv*)lp;

	if (lpOrderData == NULL || lpConfigEnv == NULL )
		return 0;

	HWND		hSignalWnd = (HWND)KSLink_GetEnvInfo (2);

	if (hSignalWnd == NULL)
		return 0;
	
	CString strCode = lpConfigEnv->m_szCode;
	strCode.Right (7);

	CString	strCodeName = m_pInputBarDlg->GetJongMokNameByCode (strCode);
	CString strAccount = lpOrderData->aAcctNo;

	SIGNALORDERSINFO	signalOrdersInfo;
	memset (&signalOrdersInfo, 0, sizeof (SIGNALORDERSINFO));


	signalOrdersInfo.hDLLWnd = (long)GetSafeHwnd ();			// 호출한 컨트롤 핸들

	strcpy (signalOrdersInfo.szIndexNo,		"20");
	strcpy (signalOrdersInfo.szWindName,	"FX챠트");		 
	strcpy (signalOrdersInfo.szSignalgb,	"감"); 
	strcpy (signalOrdersInfo.szPopgb,		"2");
	strcpy (signalOrdersInfo.szActionGb,	"1");

	strcpy (signalOrdersInfo.szAcctno,		strAccount);
	strcpy (signalOrdersInfo.szCode,		strCode);

	switch (lpConfigEnv->m_nOrderType)
	{
	case 0 :		// 매수 신규
		strcpy (signalOrdersInfo.szMemegb, "0");
		strcpy (signalOrdersInfo.szJanGogb, "0");		// 매수
		strcpy (signalOrdersInfo.szOrdgb, "2");			// 시장가
		break;

	case 1 :		// 매수 청산
		strcpy (signalOrdersInfo.szMemegb,	"2");
		strcpy (signalOrdersInfo.szJanGogb,	"1");		// 청산일 경우는 항상 '0'
		strcpy (signalOrdersInfo.szOrdgb,	"0");		// 매수 청산
		break;

	case 2 :		// 매도 청산
		strcpy (signalOrdersInfo.szMemegb,	"2");
		strcpy (signalOrdersInfo.szJanGogb,	"1");		// 청산일 경우는 항상 '0'
		strcpy (signalOrdersInfo.szOrdgb,	"1");		// 매수 청산
		break;

	case 3 :		// 매도 신규
		strcpy (signalOrdersInfo.szMemegb, "1");
		strcpy (signalOrdersInfo.szJanGogb, "1");		// 매수
		strcpy (signalOrdersInfo.szOrdgb, "2");			// 시장가
		break;
	}

	sprintf (signalOrdersInfo.szOrdqty, "%d", atoi (lpOrderData->aOrderAmt));
	strcpy (signalOrdersInfo.szOrdprc,	"0");			// 시장가 적용


	strcpy (signalOrdersInfo.szExecRangeGb,	"0");			// 주문유효기간(체결범위, AtBest : 0, Range : 1)
	strcpy (signalOrdersInfo.szExecRange,	"0");			// 주문유효기간(체결범위 Spin값)

	::SendMessage(hSignalWnd, UM_SM_SIGNALORDER_MSG, (WPARAM)0L, (long)&signalOrdersInfo);
	//>>>
	*/
	//>>
	return FALSE;
}
//>>>

//<<20100308_JS.Kim 솔로몬
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
	if (strTRCode != QUERY_STRFX)
		return;

	m_strOut = "";
	if (pData == NULL)
		return;

	SM_fx112_OutRec1	singOutput;
	memcpy(&singOutput, pData, sizeof(SM_fx112_OutRec1));

	CChartItem* pChartItem = m_ChartMng.GetpChartItemSelected();
	ST_SELCHART *pSelChart = pChartItem->m_lpSelChart;

	if (!pSelChart)
		pChartItem->GetSelChart(pChartItem->GetSelectedRQ(), pSelChart);

	CString strTerm = pChartItem->GetTickMinDayNumberRequested();
	char chTypeRequested = pChartItem->GetTypeRequested();
	bool	bSellChart = false;
	if (pSelChart)
	{
		pSelChart->m_strKey = CString(singOutput.nkey, sizeof(singOutput.nkey));
		//<<20100308_JS.Kim 솔로몬
		//pSelChart->m_nOrgTimeIndex = atoi("22");
		//>>
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

		if( singOutput.mgubun[0] == '2' && pSelChart->m_strItemCode.Left(3) == "|S_" )	// 매도
			bSellChart = true;
	}
/*
//JS.Kim
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
		strStartDate = CString(singOutput.startDate, sizeof(singOutput.startDate));
		strEndDate = CString(singOutput.endDate, sizeof(singOutput.endDate));

		//	부호
		if (!strSign.IsEmpty())		pChartItemRealData->m_chSign = strSign.GetAt(0);
		else						pChartItemRealData->m_chSign = ' ';

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
	m_strOut.Format(_T("%s"), (LPTSTR)(char*)(pData+sizeof(SM_fx112_OutRec1)));
*/
	SM_fx112_CHART* pChart = (SM_fx112_CHART *)((char*)pData + sizeof(SM_fx112_OutRec1));
	char pCnt[12];
	sprintf(pCnt, "%*.*s", sizeof(pChart->fcnt), sizeof(pChart->fcnt), pChart->fcnt);
	int nCount = atoi(pCnt);

	if( E_RPTS_PAIR == GetRequestPairTickState() || bSellChart )
	{
		/*
		int nLen = (sizeof(SM_fx112_OutRec1) + sizeof(SM_fx112_CHART) - 1);
		if( nLen > dwTRDateLen )	return;
		int nLen2 = nCount * sizeof(SM_fx112_OutRec2);
		if( nLen + nLen2 > dwTRDateLen )	return;
		nLen2 = min(nLen2, dwTRDateLen-nLen-nLen2);
		m_strOut.Format(_T("%*.*s%*.*s"), 
							nLen, nLen, (LPTSTR)(char*)(pData), 
							nLen2, nLen2, (LPTSTR)((char*)pData + nLen + nLen2));
		*/
		/*
		int nLen = sizeof(SM_fx112_OutRec1);
		if( nLen > dwTRDateLen )	return;
		int nLen2 = sizeof(SM_fx112_CHART) - 1 + nCount * sizeof(SM_fx112_OutRec2);
		if( nLen + nLen2 > dwTRDateLen )	return;
		nLen2 = min(nLen2, dwTRDateLen-nLen-nLen2);

		m_strOut.Format(_T("%*.*s%*.*s"), 
							nLen, nLen, (LPTSTR)(char*)(pData), 
							nLen2, nLen2, (LPTSTR)((char*)pData + nLen + nLen2));
		*/
		int nLen = (sizeof(SM_fx112_OutRec1) + sizeof(SM_fx112_CHART) - 1) + nCount * sizeof(SM_fx112_OutRec2);
		if( nLen > dwTRDateLen )	return;

		int nLen2 = (sizeof(SM_fx112_CHART) - 1) + nCount * sizeof(SM_fx112_OutRec2);
		nLen2 = min(nLen2, dwTRDateLen);


		m_strOut.Format(_T("%*.*s%*.*s"), sizeof(SM_fx112_OutRec1), sizeof(SM_fx112_OutRec1), (LPTSTR)(char*)(pData),
							nLen2, nLen2, (LPTSTR)((char*)pData + nLen));
	}
	else
	{
		int nLen = (sizeof(SM_fx112_OutRec1) + sizeof(SM_fx112_CHART) - 1) + nCount * sizeof(SM_fx112_OutRec2);
		nLen = min(nLen, dwTRDateLen);
		m_strOut.Format(_T("%*.*s"), nLen, nLen, (LPTSTR)(char*)(pData));
	}

}

void CStdDialog::DoShareCode(LPCSTR _szCode)
{
	//@100304 공유(도미노)를 받지는 않지만 보내기는 한다.
	//	if(m_nMatrix>=1) return;

	if(!m_bNotifyChangeCode) return;
	if(g_pPtUtilMng)
	{
		//<<20100312_JS.Kim FX 
		/*
		//@Solomon	과거종목일 경우 종목 공유 없음.	-->
		if (m_pInputBarDlg && m_pInputBarDlg->m_pFOHisBtn)
		{
			CString strCode;
			strCode.Format(_T("%s"), _szCode);
			int nCodeType = m_pInputBarDlg->m_pFOHisBtn->GetCodeType(strCode);
			switch (nCodeType)
			{
			case futureType:	// 선물
			case spreadType:	// 스프레드
			case callType:		// 콜옵션
			case putType:		// 풋옵션
			case sfutureType:	// 주식선물
			case koptionType:	// 주식옵션
				return;
			}
		}
		*/
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
		return FALSE;

	rString.Empty();
	nSetp=0;

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
	if(GetScreenNo()=="0515") return TRUE;

// 	else
// 	{
// 		m_pInputBarDlg->m_bChkLoadSaveCode = FALSE;
// 		rString = m_mapDominoCode.;
// 		m_strDominoCode = _T("");
// 		bDomino = TRUE;
// 	}
//@Solomon	DominoData 적용. <--

	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);

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

	CStringArray arrCode, arrName;
	CString rCode;

	switch (nMarketType)
	{
	case CMasterTypeInfo::STOCK_CHART:
		{
			//if(m_nMatrix>=1)
			{
				CString szInitKey;
				szInitKey.Format("OnLoadHistory_Init_%d", m_nMatrix);
				if(m_mapDominoCode.Lookup(szInitKey, rCode)==TRUE)
					rString = rCode;
				if(rCode.GetLength()>0)
					break;
			}
			if(m_nMatrix>=1)
			{
				rString.Empty();
				break;
			}

// 			if (bDomino && m_mapDominoCode.Lookup(_T("1301"), rCode))
// 			{
// 				nSetp = 0;
// 				rString = rCode;
// 				break;
// 			}

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
			rCode = _T("007800");	//솔로몬저축은행.
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
				rString = rCode;
			if(rCode.GetLength()>0)
				break;
		}
		if(m_nMatrix>=1)
		{
			rString.Empty();
			break;
		}

// 		if (bDomino && m_mapDominoCode.Lookup(_T("20301"), rCode))
// 		{
// 			nSetp = 0;
// 			rString = rCode;
// 			break;
// 		}
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
				rString = rCode;
			if(rCode.GetLength()>0)
				break;
		}
		if(m_nMatrix>=1)
		{
			rString.Empty();
			break;
		}
// 		if (bDomino && m_mapDominoCode.Lookup(_T("30301"), rCode))
// 		{
// 			nSetp = 0;
// 			rString = rCode;
// 			break;
// 		}
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
		if (bDomino && m_mapDominoCode.Lookup(_T("30301"), rCode))
		{
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

		rString = "";
		bRet = FALSE;
		break;
		
	case CMasterTypeInfo::COMMODITY_CHART:
		//100208-alziyes. 상품선물 domino확인할 것.
		if (bDomino && m_mapDominoCode.Lookup(_T("cfcod"), rCode))
		{
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
		
		pMasterDataManager->GetMasterDataInUpDetails("COMMODITYMASTER", arrCode, arrName, "NONE");

		if (arrCode.GetSize() > 0)
		{
			rCode = arrCode.GetAt(0);
			nSetp = 3;
			rString = rCode;
			break;
		}

		//Step 3. 디폴트 코드로 조회.
		rString = "";
		bRet = FALSE;
		break;
		
	case CMasterTypeInfo::FOREIGN_CHART:
		if (bDomino && m_mapDominoCode.Lookup(_T("50301"), rCode))
		{
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
//	GetFirstCode(rCode, m_nMarketType, nStep);

// 	//@solomon	미니멀티차트인경우 빈페이지로 띄움.
// 	if (m_nChartType == MINI_SINGLE2_CHART)
// 		return ;

//@100303 매트릭스 화면에서도 처리.
//	if (m_nMatrix != 0)		return;

	GetFirstCode(rCode, -1, nStep);

	WPARAM wp = (WPARAM)(LPCSTR)rCode;
	LPARAM lp = m_nMarketType;
	SendMessage(UMSG_SENDSHAREDCODE, wp, lp);

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

CString CStdDialog::GetScreenNo()
{
	CString strScreenNo;
	strScreenNo.Format("%04d", m_nScreenID);
	return strScreenNo;
}

CString CStdDialog::GetCodeType(CString strCode)
{
	CString strType = "";
	m_mapCodeType.Lookup(strCode, strType);

	return strType;
}

LRESULT CStdDialog::OnAccountInit(WPARAM wp, LPARAM lp)
{
	if(m_pExtInputBarDlg && m_pExtInputBarDlg->m_hWnd)
		m_pExtInputBarDlg->InitAccountCtrl(wp, lp);

	return 0L;
}


//>>