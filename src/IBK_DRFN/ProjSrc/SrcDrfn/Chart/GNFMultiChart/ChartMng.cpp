// ChartMng.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "ChartMng.h"
#include "StdDialog.h"
#include "ChartListDlg.h"
#include "CodeDataFromInputBar.h"
#include "FloatingHogaDlg.h"
#include "./define/ChartMsgDef.h"
#include "./define/DragNDropDefine.h"
#include "./define/MessageDefine.h"
#include "../COMMONTR/ChartRealConverter_GNF.h"
#include "../Chart_Common/BCMenu.h"
//@SolomonGNF #include "../../../Common/KHResource.h"
#include "../../inc/RealUpdate.h"

//20080919 이문수 >>
#include "Winprint.h"
//20080919 이문수 <<

//#include "../../COMMON_BASE/drds/DRDSDEF.h"
//#include "../CommonTR/DrdsS31.h"
//#include "../CommonTR/DrdsJS0.h"
//#include "../CommonTR/DrdsSC0.h"
//#include "../CommonTR/DrdsOC0.h"
//#include "../../Inc/RealUpdate.h"

//#include "../../../DataMgr/DBMgr.h" //05.09.27
//#include "../../../DrMLib/DrMiddleLib.h"//05.09.27

//#include "../../INCLUDE_BASE/XfmMapInfo.h"
//#include "../../ToolKit/MainDll_Interface/Plat/form.h"	// 플랫폼 관련 부분 모음.

#include "../Include_Chart/Dll_Load/IMetaTable.h"	// (2008/9/13 - Seung-Won, Bae) for Multi-Language


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE g_hInstance;

const UINT	RMSG_OPENMAPWITHDATA	= ::RegisterWindowMessage(_T("RMSG_OPENMAPWITHDATA"));
const UINT  RMSG_SAVE_FOCUS_CONTROL = ::RegisterWindowMessage("RMSG_SAVE_FOCUS_CONTROL"); //05.09.30
const UINT  RMSG_MAGNIFYING_GLASS = ::RegisterWindowMessage("RMSG_MAGNIFYING_GLASS"); //05.09.30


// (2005/2/17 - Seung-Won, Bae) Term Analysis Infomation Notify I (Drag Start Position in Time Serial)
//		LParam : Time Value (long) of Mouse Position
#define UWM_DRAGSTART				( WM_USER + 0x2D3C)
// (2005/2/17 - Seung-Won, Bae) Term Analysis Infomation Notify II (Drag End Position in Time Serial)
//		LParam : Time Value (long) of Mouse Position
#define UWM_DRAGEND					( WM_USER + 0x2D3D)

#define UWM_ZOOMVALUE			( WM_USER + 0x2D36)
#define	UWM_VIEWENDTIME			( WM_USER + 0x2D37)
#define UWM_FIRECODESHARE		( WM_USER + 0x2D38)
// (2004.10.08, 배승원) 십자선 이용시의 수치조회창 이용여부 변경시 Multi-Chart에게 알려,
//		다른 분할차트에도 이 설정이 적용되도록 한다.
//		이때, WParam의 값이 새로이 설정된 수치조회창 이용여부 설정값이다.
#define UWM_USEINQUIRY_CROSSLINE	( WM_USER + 0x2D39)

// (2004.10.26, 배승원) 차트의 호가 공유 (호가-차트)가 발생됨을 알리는 Message이다.
//		이때 LParam의 값으로 공유 Data String의 Pointer (const char *)가 전달된다.
#define UWM_FIREASKINGPRICESHARE	( WM_USER + 0x2D3A)
// (2004.11.04, 배승원) 차트의 Global 공유 Data 발생을 DLL Container에게 알리는 Message이다.
//		이때 LParam으로 "공유명;공유Data"의 형태로 Data가 전달된다.
//		기본적으로 Chart는 시간공유, 일반호가공유, 특수호가공유(화면별제한불가)를 발생시킨다.
#define UWM_FIREGSHAREDATA			( WM_USER + 0x2D3B)

// (2005/10/25 - Sun-Young, Kim) 차트 전체초기화
#define UWM_ORGRETURN				( WM_USER + 0x2D40)

// 복수종목 차트 선택 : 복수종목 - ojtaso (20070122)
#define UWM_SELECTEDCHART			( WM_USER + 0x2D46)
// 복수종목의 Total Count를 멀티차트에 알려줌 : 복수종목 - ojtaso (20070704)
#define UWM_MULTITOTALCOUNT			( WM_USER + 0x2D47)

// 2008.10.15 by LYH >> 갭보정
#define UWM_USE_GAPREVISION			( WM_USER + 0x2D50)

#define UDM_DRAW_FOR_PRINT      WM_USER + 617 // wParam - 0, lParam - HDC //20080919 이문수 >>


#define _CHARTMSG_EVENTMESSAGE	"CHARTMSG_EVENTMESSAGE"
const UINT RMSG_CHARTMSG_EVENTMESSAGE	= ::RegisterWindowMessage( _CHARTMSG_EVENTMESSAGE);

const UINT RMSG_GETOPENEDOWNER = ::RegisterWindowMessage(_T("RMSG_GETOPENEDOWNER"));

//extern IMainDllObj	* g_pMainDll;
const UINT RMSG_CREATEOPENSHAREDATA	= ::RegisterWindowMessage(_T("RMSG_CREATEOPENSHAREDATA"));

#define EACH_CHARTJOB_START()	\
		{	CChartItem *pChartItem = NULL;\
			for(int nRowIndex=0;nRowIndex<m_lRows;nRowIndex++){\
				for(int nColIndex=0;nColIndex<m_lCols;nColIndex++){\
					pChartItem = m_pChartItems[nRowIndex][nColIndex];\
					if(pChartItem){
#define EACH_CHARTJOB_END()		\
		}	}	}	}

#define EACH_CHART_MAKENULL() m_pChartItems[nRowIndex][nColIndex] = NULL;

#define MEMCOPYINTOBUFFER(lpBuf,lpData,lDataSize, lOffset)		memcpy(&lpBuf[lOffset],lpData,lDataSize); lOffset+=lDataSize;



const UINT RMSG_STDATAMANAGER_RESULTDATA = ::RegisterWindowMessage("RMSG_STDATAMANAGER_RESULTDATA");

// 메뉴 추가
#define ID_PLM_ADD_MENU					41500		// 메뉴 아이디 시작
#define ID_PLM_ADD_MENU_STARTANALTOOL	41500		// 분석도구 메뉴 아이디 Begin
#define ID_PLM_ADD_MENU_ENDANALTOOL		41699		// 분석도구 메뉴 아이디 End
#define ID_PLM_ADD_MENU_BEGINMAIN		41700		// 메인 메뉴 아이디 Begin
#define ID_PLM_ADD_MENU_ENDMAIN			41719		// 메인 메뉴 아이디 End
#define ID_PLM_ADD_MENU_BEGINMAINCONN	41720		// 메인연결화면 메뉴 아이디 Begin
#define ID_PLM_ADD_MENU_ENDMAINCONN		42699		// 메인연결화면 메뉴 아이디 End
#define ID_PLM_ADD_MENU_END				42699		// 메뉴 아이디 끝

#define ID_TOOLMENU_BASE				2000
#define ID_MAINMENU_BASE				4310		// 4310 ~ 4320
#define ID_MAINMENU_BASE_END			4320		// 4310 ~ 4320
#define ID_MAINCONNMENU_BASE			33000		// 33000 ~ 34000
#define ID_MAINCONNMENU_BASE_END		34000		// 33000 ~ 34000
//#define UDM_ADD_PLATFORM_MENU		( WM_USER + 615 )
//#define UDM_CHECK_PLATFORM_MENU		( WM_USER + 616 )
const UINT RMSG_ADD_PLATFORM_MENU = ::RegisterWindowMessage(_T("RMSG_ADD_PLATFORM_MENU"));
//const UINT RMSG_CHECK_PLATFORM_MENU = ::RegisterWindowMessage(_T("RMSG_CHECK_PLATFORM_MENU"));

// --> [Edit]  강지원 2008/11/05
const UINT RMSG_INQUIRY_DATA_EXPORT = ::RegisterWindowMessage( "RMSG_INQUIRY_DATA_EXPORT");
// <-- [Edit]  강지원 2008/11/05
/////////////////////////////////////////////////////////////////////////////
// CChartMng

int		 CChartMng::m_nStatic_RecvCntShareCode = 0;
CString  CChartMng::m_strStatic_ShareCode = _T(""); //차트 로드시 공유로 들어온 종목을 한번만 적용하기 위해 
CString  CChartMng::m_strCodeForMulti = _T(""); //차트 로드시 공유로 들어온 종목을 한번만 적용하기 위해 


CChartMng::CChartMng()
{
	m_bNeedForceCode    = TRUE;
	m_bAddingMarketData = FALSE;
	m_bLastEnvUpdated = FALSE;
	m_lColSelectedInLoop = 0;
	m_lRowSelectedInLoop = 0;
	m_bIsInLoop = FALSE;
	m_lTypeInLoop = LOOPTYPE_NULL/*NULL*/;
	
	m_pChartItemInLoop = NULL;
	m_nRowIndexInLoop = 0;
	m_nColIndexInLoop = 0;

	m_bUpdatingRevisedValue = FALSE;
	m_bHideScale = FALSE;
	m_bHideInnerToolBar = FALSE;
	m_bUpdatedInChartExtended = FALSE;
	m_bUpdatingShowingFackedCandle = FALSE;
	m_bTypeLink = FALSE;
	m_bJongmokLink = FALSE;
	m_bClicked = FALSE;
	m_bDragNDrop = FALSE;
	m_lKey = NULL;
	m_pInputBarDlg = NULL;
	m_pExtInputBarDlg = NULL;
	m_pLeftBarDlg = NULL;
	m_bChartExtended = FALSE;
//	m_clStartSelected = RGB(201,207,229);
//	m_clEndSelected = RGB(202,208,230);
//	m_clStartUnselected = RGB(135,135,135);
//	m_clEndUnselected = RGB(165,165,165);

//	m_clStartSelected = RGB(246,242,252);
//	m_clEndSelected = RGB(242,242,255);
//	m_clStartUnselected = RGB(181,187,209);
//	m_clEndUnselected = RGB(182,188,210);

//	m_clStartSelected = RGB(226,236,251);
//	m_clEndSelected = RGB(226,236,251);
//	m_clStartUnselected = RGB(225,227,233);
//	m_clEndUnselected = RGB(182,188,210);

	//m_clStartSelected = RGB(219,229,247);
	//m_clStartSelected = RGB(255,243,207);
//	m_clStartSelected =	RGB(222,232,255);	//RGB(236,241,255);// RGB(255,255,239);	//RGB(255,233,160);
	m_clStartSelected =	RGB(252,214,210);
	//m_clEndSelected = RGB(219,229,247);
	m_clStartUnselected = RGB(242,242,242);	//RGB(225,227,233);
	//m_clEndUnselected = RGB(225,227,233);
	m_nMode = CHANGE_ITEM;
	m_nOldMode = -1;
	m_lCols = 0;
	m_lRows = 0;
	m_lColSelected = 0;
	m_lRowSelected = 0;
	m_lColDragFrom = -1;
	m_lRowDragFrom = -1;
	m_lDragStart = 0;
	m_lDragEnd = 0;
	::memset(m_pChartItems,0x00,sizeof(m_pChartItems));	

	m_lMltchrtDlgParentClsPointer = 0;

	//m_pDBMgr = NULL;
	// gm0604 added 2005/8/23
	m_bIsSystemTradingMode = FALSE;

	m_pFloatingDlg = NULL;
	m_pRectFloatingHoga = NULL;

	m_bLBDnFromChart = FALSE;

	m_bEmptyDataChart		= FALSE;
	m_bEmptyChartForSinkCfg	= FALSE;


	//=======================================================================
	// 2005.10.12
	// 틱/초/일/주/월/년 별 지표 저장
	//=======================================================================
	m_pTickCfgChart		= NULL;
	m_pSecCfgChart		= NULL;
	m_pMinCfgChart		= NULL;
	m_pDayCfgChart		= NULL;
	m_pHourCfgChart		= NULL;
	m_pWeekCfgChart		= NULL;
	m_pMonthCfgChart	= NULL;
	m_pYearCfgChart		= NULL;

	m_bExistSyncChart	= FALSE;
	//=======================================================================
	
	m_bReplaychartMode  = FALSE;

	m_bMarketJipyoAdding = FALSE;

	m_bNoExistLoadCnts = FALSE;

	m_bInitFromExtToolBar = FALSE; //05.11.24 초기화 상태의 모드는 아니다.

	m_strCodeForLoadChart = "";

	// 복수종목 추가 : 복수종목 - ojtaso (20070118)
	m_bAddMode = FALSE;
	m_bChangeTypeMode = FALSE;

	// 멀티차트의 선택여부 : 복수종목 - ojtaso (20070525)
	m_bSelChangeChart = FALSE;
	m_bAddModeFromTab = FALSE;

	// 차트 초기화인지 : 복수종목 - ojtaso (20070621)
	m_bResetChartAll = FALSE;
	m_bOnEventMessage = FALSE;
// --> [Edit]  이도영 2008/09/13
// 
	m_bOrderLink	= FALSE;
// <-- [Edit]  이도영 2008/09/13

// --> [Edit]  강지원 2008/10/21
	m_strTimeSet		= "";
// <-- [Edit]  강지원 2008/10/21

// --> [Edit]  강지원 2008/11/04
	m_pInquiryDlg	= NULL;
	m_pChartItemForRequest = NULL;
// <-- [Edit]  강지원 2008/11/04

// --> [Edit]  강지원 2008/11/19
	m_bInitFlag		= FALSE;		//	전체 초기화시 파일을 저장하지 않는다.
// <-- [Edit]  강지원 2008/11/19

	// 2011.01.12 by SYS >> 분차트 YYYY(년) 추가 및 분석툴 공유 기능 추가
	m_bShareAnalTool	= FALSE;
	// 2011.01.12 by SYS
}

CChartMng::~CChartMng()
{
// --> [Edit]  이도영 2008/09/11
// 
	POSITION pos;
	CString strKey;
	ORDER_INFO * pOrerInfo;
	pos = m_mapOrder.GetStartPosition();
	while (pos)
	{
		m_mapOrder.GetNextAssoc(pos, strKey, (CObject*&)pOrerInfo);
		if( pOrerInfo )
		{
			delete pOrerInfo;
		}
	}
	m_mapOrder.RemoveAll();
// <-- [Edit]  이도영 2008/09/11

// --> [Edit]  강지원 2008/11/06
	//	수치조회창
	if(m_pInquiryDlg)
	{
		m_pInquiryDlg->DestroyWindow();
		delete m_pInquiryDlg;
		m_pInquiryDlg = NULL;
	}
// <-- [Edit]  강지원 2008/11/06

}


BEGIN_MESSAGE_MAP(CChartMng, CWnd)
	//{{AFX_MSG_MAP(CChartMng)	
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE( RMSG_CHARTMSG_EVENTMESSAGE,	OnEventMessage)
	ON_REGISTERED_MESSAGE( RMSG_GET_USERID,	OnGetUserID)
	ON_REGISTERED_MESSAGE( RMSG_MAGNIFYING_GLASS, OnMagnifyingGlass)
	ON_REGISTERED_MESSAGE( RMSG_ADD_PLATFORM_MENU, OnAddPlatformMenu)
// --> [Edit]  강지원 2008/11/05	( 수치조회 메시지 )
	ON_REGISTERED_MESSAGE( RMSG_INQUIRY_DATA_EXPORT, OnInquiryDataExport)
// <-- [Edit]  강지원 2008/11/05
//	ON_REGISTERED_MESSAGE( RMSG_CHECK_PLATFORM_MENU, OnCheckPlatformMenu)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChartMng message handlers

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	Input	:	lKey	데스크기능을 위한 class의 Pointer
//	목적	:	데스크 Class와 연결하고 Chart들을 Setting한다.
///////////////////////////////////////////////////////////////////////////////
void CChartMng::SetBaseDesk(long lKey)
{
	BOOL bNoToolBar = FALSE;
	if(m_pExtInputBarDlg == NULL)
	{
#ifdef _DEBUG
		AfxMessageBox("CChartMng::SetBaseDesk m_pExtInputBarDlg ERRROR!");
#endif
		return;
		//VERIFY(m_pExtInputBarDlg);
	}
	m_lKey = lKey;
	VERIFY(m_pExtInputBarDlg);
	//BOOL bRevised = m_pExtInputBarDlg->SendMessage(UMSG_GETREVISED);
	//BOOL bNormalizedMinuteTimeRule = m_pExtInputBarDlg->SendMessage(UMSG_GETNORMALIZEDMINUTETIMERULE);
	BOOL bTimeShareUseage = m_pExtInputBarDlg->SendMessage(UMSG_GETTIMESHAREUSEAGE);
//	if( ((CStdDialog*)GetParent())->m_nChartType==MINI_MULTI_CHART ||
//		((CStdDialog*)GetParent())->m_nChartType==MINI_SINGLE_CHART ) 
//		bNoToolBar = TRUE;

	//BOOL bUpperLowPriceMarkUseage = m_pExtInputBarDlg->SendMessage(UMSG_GETUPPERLOWPRICEMARKUSEAGE);
	EACH_CHARTJOB_START()
	//pChartItem->SetInit((long)this->GetSafeHwnd(), m_lKey,bNoToolBar);
	pChartItem->SetInit((long)this->GetSafeHwnd(), m_lKey, m_strRootPath, bNoToolBar);
	if(m_lMltchrtDlgParentClsPointer > 0)
		pChartItem->SetOutsideCommonInterface(m_lMltchrtDlgParentClsPointer);
	//pChartItem->SetUseRevisedValue(bRevised);
	//pChartItem->SetBNormalizedMinuteTimeRule(bNormalizedMinuteTimeRule);
	pChartItem->EnableReceivingTimeShare(bTimeShareUseage);
	//pChartItem->EnableBoundMark(bUpperLowPriceMarkUseage);
	EACH_CHARTJOB_END()
//	BOOL bNoToolBar = FALSE;
//	if(((CStdDialog*)GetParent())->m_nChartType==1)
//	{
//		bNoToolBar = TRUE;
//	}
//	m_lKey = lKey;
//	if(m_pExtInputBarDlg == NULL)
//	{
//#ifdef _DEBUG
//		AfxMessageBox("CChartMng::SetBaseDesk m_pExtInputBarDlg ERRROR!");
//#endif
//		return;
//		//VERIFY(m_pExtInputBarDlg);
//	}
//
//	
//	
//	//BOOL bRevised		= m_pInputBarDlg->SendMessage(UMSG_GETREVISED); // 수정 주가 사용여부
//	//BOOL bLogPrice	= m_pInputBarDlg->SendMessage(UMSG_GETLOG);		// 로그값    사용여부
//	//BOOL bRevised		= m_pExtInputBarDlg->SendMessage(UMSG_GETREVISED); //==? 수정 주가 사용여부
//
//	//체결이 없어도 해당 시간간격으로 차트를 그림=====================================================
//	//어떤 종목을 1분주기로 조회를 했을때 
//	//10시부터 11시사이에  체결이 없다면 허수봉 적용 안했을때는
//	//10-11시사이를 건너뛰면서 그리고 허수봉적용하면 
//	//10시-11시 체결이 없어도 최근 체결가격을 이어서 그려줌
//	//허수봉 여부
//	//BOOL bNormalizedMinuteTimeRule = m_pExtInputBarDlg->SendMessage(UMSG_GETNORMALIZEDMINUTETIMERULE);	
//	//상하한바 표시 여부.
//	//BOOL bUpperLowPriceMarkUseage  = m_pExtInputBarDlg->SendMessage(UMSG_GETUPPERLOWPRICEMARKUSEAGE);	
//	//시간 공유 여부
//	//BOOL bTimeShareUseage		   = m_pExtInputBarDlg->SendMessage(UMSG_GETTIMESHAREUSEAGE);
//	//락 표시 여부.
//	//차트 종료시 저장 여부.
//
//	//==============================================================================================
//	// 차트에 반복 적용을 한다.
//	//==============================================================================================
//	EACH_CHARTJOB_START()			
//			pChartItem->SetInit((long)this->GetSafeHwnd(), m_lKey, m_strRootPath, bNoToolBar);
//
//			if(m_lMltchrtDlgParentClsPointer > 0)
//				pChartItem->SetOutsideCommonInterface(m_lMltchrtDlgParentClsPointer);
//			
//			/*
//			pChartItem->SetUseRevisedValue(TRUE);
//			pChartItem->SetUseLogValue(FALSE); // 2005. 05. 27 add by sy, nam
//			pChartItem->SetBNormalizedMinuteTimeRule(TRUE);
//			pChartItem->EnableReceivingTimeShare(TRUE);
//			pChartItem->EnableBoundMark(TRUE);
//			*/
//
//
//	EACH_CHARTJOB_END()
//	//===============================================================================================
}

BOOL CChartMng::Create(const RECT& rect, CWnd *pParentWnd, LPCTSTR lpUserPath, LPCTSTR lpImagePath, UINT nID, long clsPointer)
{
	m_strUserPath = lpUserPath;
	m_strImagePath = lpImagePath;
	m_nID = nID;
	m_lMltchrtDlgParentClsPointer = clsPointer;
//	if(((CStdDialog*)pParentWnd)->m_nChartType==1)
//	{
//		m_lHeightOfCaption = 0;
//	}

	
	//BOOL bResult = CWnd::Create(NULL,"Chart Mng",WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE|WS_BORDER,rect,pParentWnd,nID);
	BOOL bResult = CWnd::Create(NULL,"Chart Mng",WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE,rect,pParentWnd,nID);
	//==============================================================================================
	//2005. 06. 23 현재 분할된 차트의 갯수 1 * 1 상태일 경우 Floating Hoga 창을 생성한다.
	//==============================================================================================
	CreateFloatingHogaWnd();
	//==============================================================================================


	// 분석툴 개별 저장 - ojtaso (20080723)
	if(((CStdDialog*)GetParent())->m_nChartType==SINGLE_CHART)
	{
		m_bSaveJipyoOneFile = m_pInputBarDlg->SendMessage(UMSG_GETSAVEJIPYOONEFILE);
		m_bSaveAnalTool = !m_bSaveJipyoOneFile;
	}
	else if(m_pExtInputBarDlg)
	{
		m_bSaveJipyoOneFile = m_pExtInputBarDlg->SendMessage(UMSG_GETSAVEJIPYOONEFILE);
		m_bSaveAnalTool = m_pExtInputBarDlg->SendMessage(UMSG_GETSAVEJIPYOONEFILE, 100);
	}
	
	// 2011.01.12 by SYS >> 분차트 YYYY(년) 추가 및 분석툴 공유 기능 추가
	if(m_pExtInputBarDlg)
	{
		m_bShareAnalTool = m_pExtInputBarDlg->SendMessage(UMSG_GET_ALALTOOL_SHAREFILE, 0);
	}
	// 2011.01.12 by SYS 

	return bResult;
}


int CChartMng::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;	
	// TODO: Add your specialized creation code here
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));       // Zero out the structure.
	lf.lfHeight = 12;                      // Request a 12-pixel-height font.
/*	lf.lfWeight = 700;					   JSR-20051129 STOP  FW_BOLD(=700) */
	lf.lfWeight = 700;					   //LYH-20061009 FW_BOLD(=700)
	lstrcpy(lf.lfFaceName, _T("굴림체"));   // Request a face name "굴림".
	lf.lfCharSet = HANGUL_CHARSET;
	m_ftForTitle.CreateFontIndirect(&lf);

	lf.lfWeight	= 500;
	m_ftNormal.CreateFontIndirect(&lf);
	Rearrange(1,RESIZEBOTH);
	SetChartSelected(CPoint(0,0));	

	return 0;
}

void CChartMng::OnDestroy() 
{
#ifdef _WRITELOG
	((CStdDialog*)GetParent())->WriteToStringLog("CChartMng::OnDestroy::Begin", _T("멀티차트 종료"), 13);
#endif

	//Floating Hoga Destroy......
	DestroyFloatingHogaWnd();

#ifdef _DEBUG
		EACH_CHARTJOB_START()
			pChartItem->InvokeAddInSTIndex("ChartSTIndexAddIn.dll",2,0);
		EACH_CHARTJOB_END()		
#endif
	if(m_bIsSystemTradingMode)// clear System strading info. 2005.10.10
	{
		EACH_CHARTJOB_START()
			pChartItem->ClearPtrST();
		EACH_CHARTJOB_END()		
	}

	BOOL bSavedFromExtInputBarGlg = FALSE;
	CString sJipyoFileName1, sJipyoSaveFileName;

	if(m_bReplaychartMode)
		goto ReplayMode;

	
	if(m_pExtInputBarDlg)
	{
		bSavedFromExtInputBarGlg = m_pExtInputBarDlg->SendMessage(UMSG_GETSAVED); //종료시 저장은 무조건 전부 
		if(bSavedFromExtInputBarGlg)//모두 저장시 푼다.
		{
// --> [Edit]  강지원 2008/11/02	( 체결 버튼일경우 저장만 한다. )
			if ( GetCheHogaType() == 0 )
			{
				CheHoga_Save(1, TRUE, FALSE);
			}
			else
			{
				CheHoga_Save(0, TRUE, FALSE);
			}
			//SaveChart(((CStdDialog*)GetParent())->m_strConfig);
// <-- [Edit]  강지원 2008/11/02
		}
	}

/*	else //저장 안 하면 하나만 저장한다. 05.12.20 막음
	{
		//무조건 단순 저장 : 현재 선택된 ChartItem 하나만 저장한다.
		ReplaceChart(0,0,m_lRowSelected,m_lColSelected);
		SetChartSelected(CPoint(0,0));
		if(GetpChartItemSelected())
		{
			GetpChartItemSelected()->SaveChartInfo(((CStdDialog*)GetParent())->m_strConfig,TRUE);
		}			
	}*/

	//각 차트 별 저장 여부에 대한 판단을 한다.
	//SaveChart(((CStdDialog*)GetParent())->m_strConfig);

	//if(!((CStdDialog*)GetParent())->IsSimpleChart())
	{
		//sJipyoFileName1 = ((CStdDialog*)GetParent())->m_strScreenNo;
		//if(m_nMarketType == STOCK_CHART)
		//	sJipyoFileName1 += "_Stock";
		//else if(m_nMarketType == FUTOPT_CHART)
		//	sJipyoFileName1 += "_FutOpt";
		//else if(m_nMarketType == UPJONG_CHART)
		//	sJipyoFileName1 += "_Upjong";
		//else if(m_nMarketType == FOREIGN_CHART)
		//	sJipyoFileName1 += "_Foreign";	
		//else if(m_nMarketType == ELW_CHART)
		//	sJipyoFileName1 += "_ELW";	

#ifdef _WRITELOG
		((CStdDialog*)GetParent())->WriteToStringLog("m_pSecCfgChart::SaveandOpenFromFile::Begin", _T("멀티차트 종료"), 13);
#endif
		if(m_pSecCfgChart)
		{		
			if(::IsWindow(m_pSecCfgChart->GetSafeHwnd()))
			{
//				if( ( ((CChartItem*)m_pSecCfgChart)->GetType() == SEC10_DATA_CHART ) ||
//					( ((CChartItem*)m_pSecCfgChart)->GetType() == SEC10_DATA_CHART ) )
				if( ( ((CChartItem*)m_pSecCfgChart)->GetType() == SEC_DATA_CHART ))
				{
					//if(m_bSaveJipyoOneFile)
					//	sJipyoSaveFileName = sJipyoFileName1 + "_" + JIPYOSYNC_FILE;
					//else
					//	sJipyoSaveFileName = sJipyoFileName1 + "_Sec_" + JIPYOSYNC_FILE	;
					sJipyoSaveFileName = ((CChartItem*)m_pSecCfgChart)->GetOpenAndSaveFileName(FALSE, TRUE);
					((CChartItem*)m_pSecCfgChart)->SaveandOpenFromFile("", sJipyoSaveFileName, FALSE,TRUE);
				}
			}
		}
#ifdef _WRITELOG
		((CStdDialog*)GetParent())->WriteToStringLog("m_pSecCfgChart::SaveandOpenFromFile::End", _T("멀티차트 종료"), 13);
		((CStdDialog*)GetParent())->WriteToStringLog("m_pTickCfgChart::SaveandOpenFromFile::Begin", _T("멀티차트 종료"), 13);
#endif
		if(m_pTickCfgChart)
		{		
			if(::IsWindow(m_pTickCfgChart->GetSafeHwnd()))
			{
				if( ((CChartItem*)m_pTickCfgChart)->GetType() == TICK_DATA_CHART ) 
				{
					//if(m_bSaveJipyoOneFile)
					//	sJipyoSaveFileName = sJipyoFileName1 + "_" + JIPYOSYNC_FILE;
					//else
					//	sJipyoSaveFileName = sJipyoFileName1 + "_Tick_" + JIPYOSYNC_FILE	;
					sJipyoSaveFileName = ((CChartItem*)m_pTickCfgChart)->GetOpenAndSaveFileName(FALSE, TRUE);
					((CChartItem*)m_pTickCfgChart)->SaveandOpenFromFile("", sJipyoSaveFileName, FALSE,TRUE);
				}
			}
		}
#ifdef _WRITELOG
		((CStdDialog*)GetParent())->WriteToStringLog("m_pTickCfgChart::SaveandOpenFromFile::End", _T("멀티차트 종료"), 13);
		((CStdDialog*)GetParent())->WriteToStringLog("m_pMinCfgChart::SaveandOpenFromFile::Begin", _T("멀티차트 종료"), 13);
#endif

		if(m_pMinCfgChart)
		{		
			if(::IsWindow(m_pMinCfgChart->GetSafeHwnd()))
			{
				if( ((CChartItem*)m_pMinCfgChart)->GetType() == MIN_DATA_CHART ) 
				{
					//if(m_bSaveJipyoOneFile)
					//	sJipyoSaveFileName = sJipyoFileName1 + "_" + JIPYOSYNC_FILE;
					//else
					//	sJipyoSaveFileName = sJipyoFileName1 + "_Min_" + JIPYOSYNC_FILE	;
					sJipyoSaveFileName = ((CChartItem*)m_pMinCfgChart)->GetOpenAndSaveFileName(FALSE, TRUE);
					((CChartItem*)m_pMinCfgChart)->SaveandOpenFromFile("", sJipyoSaveFileName, FALSE,TRUE);
				}
			}
		}
#ifdef _WRITELOG
		((CStdDialog*)GetParent())->WriteToStringLog("m_pMinCfgChart::SaveandOpenFromFile::End", _T("멀티차트 종료"), 13);
		((CStdDialog*)GetParent())->WriteToStringLog("m_pDayCfgChart::SaveandOpenFromFile::Begin", _T("멀티차트 종료"), 13);
#endif

		if(m_pDayCfgChart)
		{				
			if(::IsWindow(m_pDayCfgChart->GetSafeHwnd()))
			{
				if( ((CChartItem*)m_pDayCfgChart)->GetType() == DAY_DATA_CHART ) 
				{
					//if(m_bSaveJipyoOneFile)
					//	sJipyoSaveFileName = sJipyoFileName1 + "_" + JIPYOSYNC_FILE;
					//else
					//	sJipyoSaveFileName = sJipyoFileName1 + "_Day_" + JIPYOSYNC_FILE	;
					sJipyoSaveFileName = ((CChartItem*)m_pDayCfgChart)->GetOpenAndSaveFileName(FALSE, TRUE);
					((CChartItem*)m_pDayCfgChart)->SaveandOpenFromFile("", sJipyoSaveFileName, FALSE,TRUE);
				}
			}
		}
#ifdef _WRITELOG
		((CStdDialog*)GetParent())->WriteToStringLog("m_pDayCfgChart::SaveandOpenFromFile::End", _T("멀티차트 종료"), 13);
		((CStdDialog*)GetParent())->WriteToStringLog("m_pHourCfgChart::SaveandOpenFromFile::Begin", _T("멀티차트 종료"), 13);
#endif

		if(m_pHourCfgChart)
		{				
			if(::IsWindow(m_pHourCfgChart->GetSafeHwnd()))
			{
				if( ((CChartItem*)m_pHourCfgChart)->GetType() == HOUR_DATA_CHART ) 
				{
					//if(m_bSaveJipyoOneFile)
					//	sJipyoSaveFileName = sJipyoFileName1 + "_" + JIPYOSYNC_FILE;
					//else
					//	sJipyoSaveFileName = sJipyoFileName1 + "_Hour_" + JIPYOSYNC_FILE	;
					sJipyoSaveFileName = ((CChartItem*)m_pHourCfgChart)->GetOpenAndSaveFileName(FALSE, TRUE);
					((CChartItem*)m_pHourCfgChart)->SaveandOpenFromFile("", sJipyoSaveFileName, FALSE,TRUE);
				}
			}
		}
#ifdef _WRITELOG
		((CStdDialog*)GetParent())->WriteToStringLog("m_pHourCfgChart::SaveandOpenFromFile::End", _T("멀티차트 종료"), 13);
		((CStdDialog*)GetParent())->WriteToStringLog("m_pWeekCfgChart::SaveandOpenFromFile::Begin", _T("멀티차트 종료"), 13);
#endif

		if(m_pWeekCfgChart)
		{		
			if(::IsWindow(m_pWeekCfgChart->GetSafeHwnd()))
			{
				if( ((CChartItem*)m_pWeekCfgChart)->GetType() == WEEK_DATA_CHART ) 
				{
				//	if(m_bSaveJipyoOneFile)
				//		sJipyoSaveFileName = sJipyoFileName1 + "_" + JIPYOSYNC_FILE;
				//	else
				//		sJipyoSaveFileName = sJipyoFileName1 + "_Week_" + JIPYOSYNC_FILE	;
					sJipyoSaveFileName = ((CChartItem*)m_pWeekCfgChart)->GetOpenAndSaveFileName(FALSE, TRUE);
					((CChartItem*)m_pWeekCfgChart)->SaveandOpenFromFile("", sJipyoSaveFileName, FALSE,TRUE);
				}
			}
		}
#ifdef _WRITELOG
		((CStdDialog*)GetParent())->WriteToStringLog("m_pWeekCfgChart::SaveandOpenFromFile::End", _T("멀티차트 종료"), 13);
		((CStdDialog*)GetParent())->WriteToStringLog("m_pMonthCfgChart::SaveandOpenFromFile::Begin", _T("멀티차트 종료"), 13);
#endif

		if(m_pMonthCfgChart)
		{		
			if(::IsWindow(m_pMonthCfgChart->GetSafeHwnd()))
			{
				if( ((CChartItem*)m_pMonthCfgChart)->GetType() == MONTH_DATA_CHART ) 
				{
					//if(m_bSaveJipyoOneFile)
					//	sJipyoSaveFileName = sJipyoFileName1 + "_" + JIPYOSYNC_FILE;
					//else
					//	sJipyoSaveFileName = sJipyoFileName1 + "_Month_" + JIPYOSYNC_FILE	;
					sJipyoSaveFileName = ((CChartItem*)m_pMonthCfgChart)->GetOpenAndSaveFileName(FALSE, TRUE);
					((CChartItem*)m_pMonthCfgChart)->SaveandOpenFromFile("", sJipyoSaveFileName, FALSE,TRUE);
				}
			}
		}
#ifdef _WRITELOG
		((CStdDialog*)GetParent())->WriteToStringLog("m_pMonthCfgChart::SaveandOpenFromFile::End", _T("멀티차트 종료"), 13);
		((CStdDialog*)GetParent())->WriteToStringLog("m_pYearCfgChart::SaveandOpenFromFile::Begin", _T("멀티차트 종료"), 13);
#endif

		if(m_pYearCfgChart)
		{		
			if(::IsWindow(m_pYearCfgChart->GetSafeHwnd()))
			{
				if( ((CChartItem*)m_pYearCfgChart)->GetType() == YEAR_DATA_CHART ) 
				{
					//if(m_bSaveJipyoOneFile)
					//	sJipyoSaveFileName = sJipyoFileName1 + "_" + JIPYOSYNC_FILE;
					//else
					//	sJipyoSaveFileName = sJipyoFileName1 + "_Year_" + JIPYOSYNC_FILE	;
					sJipyoSaveFileName = ((CChartItem*)m_pYearCfgChart)->GetOpenAndSaveFileName(FALSE, TRUE);
					((CChartItem*)m_pYearCfgChart)->SaveandOpenFromFile("", sJipyoSaveFileName, FALSE,TRUE);
				}
			}
		}
	}
#ifdef _WRITELOG
	((CStdDialog*)GetParent())->WriteToStringLog("m_pYearCfgChart::SaveandOpenFromFile::End", _T("멀티차트 종료"), 13);
//	((CStdDialog*)GetParent())->WriteToStringLog("m_pHogaCfgChart::SaveandOpenFromFile::Begin", _T("멀티차트 시작"), 13);
#endif
//
//		if(m_pHogaCfgChart)
//		{		
//			if(::IsWindow(m_pHogaCfgChart->GetSafeHwnd()))
//			{
//				if( ((CChartItem*)m_pHogaCfgChart)->GetType() == TICK_DATA_CHART_HOGA ) 
//				{
//					sJipyoSaveFileName = ((CChartItem*)m_pHogaCfgChart)->GetOpenAndSaveFileName(FALSE, TRUE);
//					((CChartItem*)m_pHogaCfgChart)->SaveandOpenFromFile("", sJipyoSaveFileName, FALSE,TRUE);
//				}
//			}
//		}
//#ifdef _WRITELOG
//	((CStdDialog*)GetParent())->WriteToStringLog("m_pHogaCfgChart::SaveandOpenFromFile::End", _T("멀티차트 종료"), 13);
//#endif
ReplayMode:

	m_ftForTitle.DeleteObject();		
	m_ftNormal.DeleteObject();
	EACH_CHARTJOB_START()
		pChartItem->DestroyWindow();
		delete pChartItem;
		EACH_CHART_MAKENULL()
	EACH_CHARTJOB_END()

	m_lCols = 0;
	m_lRows = 0;

	// 분석도구 메뉴추가
	m_arrayAnalIdx.RemoveAll();
	m_arrayAdditionIdx.RemoveAll();
	m_arrayUserIdx.RemoveAll();
	m_mapToolName.RemoveAll();

#ifdef _WRITELOG
	((CStdDialog*)GetParent())->WriteToStringLog("CChartMng::OnDestroy::End", _T("멀티차트 종료"), 13);
#endif
	m_menuMain.RemoveAll();
	menuChart.RemoveAll();
	
	CWnd::OnDestroy();
}

void CChartMng::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

//	OutputDebugString("CChartMng Size");

	// TODO: Add your message handler code here
	if(GetSafeHwnd())
	{
		MoveCharts();
	}
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	Input	:	nRowIndex, nColIndex  차트의 위치
//				nTitle 0=chart 1=title 그외=Chart+Title
//	목적	:	차트와 차트타이틀의 위치를 보내준다.
///////////////////////////////////////////////////////////////////////////////
CRect CChartMng::GetRectOfChart(int nRowIndex,int nColIndex, int nTitle /*=0*/)
{
	CRect rectOfChart = CRect(0,0,0,0);
	CRect rectClient;
	GetClientRect(rectClient);
	rectClient.DeflateRect(1,1,1,1);
	BOOL bIsSystemTradingMode = FALSE;
	if(m_pChartItems[nRowIndex][nColIndex])// added by gm0604 2005.10.10
	{
		bIsSystemTradingMode = m_pChartItems[nRowIndex][nColIndex]->GetPtrST();
	}	
	if(!m_bChartExtended)
	{
		if(!m_lCols||!m_lRows)	return rectOfChart;
		int nWidthOfChart =  (rectClient.Width()  - (m_lCols)*GAPOFCHARTS)/m_lCols;
		int nHeightOfChart = (rectClient.Height() - (m_lRows)*GAPOFCHARTS)/m_lRows;

		if(!nColIndex)
		{
			rectOfChart.left = rectClient.left;
		}
		else
		{
			rectOfChart.left = 1 + nWidthOfChart*nColIndex + nColIndex*GAPOFCHARTS;
		}
		if(nColIndex==(m_lCols-1))
		{
			rectOfChart.right = rectClient.right;
		}
		else
		{
			rectOfChart.right = rectOfChart.left + nWidthOfChart;
		}
		if(!nRowIndex)
		{
			rectOfChart.top = rectClient.top;
		}
		else
		{
			rectOfChart.top = 1 + nHeightOfChart*nRowIndex + nRowIndex*GAPOFCHARTS;
		}
		if(nRowIndex==(m_lRows-1))
		{
			rectOfChart.bottom = rectClient.bottom;
		}
		else
		{
			rectOfChart.bottom = rectOfChart.top + nHeightOfChart;
		}		
	}
	else
	{
		if(m_pChartItems[nRowIndex][nColIndex] == GetpChartItemSelected())
		{
			rectOfChart = rectClient;
		}
		else
		{
			return CRect(0,0,0,0);
		}
	}
	if(nTitle==1) // Title
	{
//		rectOfChart.bottom = rectOfChart.top + m_lHeightOfCaption;
		if(bIsSystemTradingMode)
		{
			rectOfChart.bottom = rectOfChart.top + HEIGHTOFCAPTION*2;
		}
		else
		{
			rectOfChart.bottom = rectOfChart.top + HEIGHTOFCAPTION;
		}
	}
	else if(nTitle==0) // Chart
	{
//		rectOfChart.top += m_lHeightOfCaption;
		if(!((CStdDialog*)GetParent())->IsSimpleChart())
		{
			if(bIsSystemTradingMode)
			{
				rectOfChart.top += HEIGHTOFCAPTION*2;
			}
			else
			{
				rectOfChart.top += HEIGHTOFCAPTION;
			}
		}

	}
	return rectOfChart;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	Input	:	szNewPos  x, y 차트의 위치
//	목적	:	특정 차트를 Active한다.
///////////////////////////////////////////////////////////////////////////////
void CChartMng::SetChartSelected(CPoint szNewPos, BOOL bInvalidateEvenIfPosisequal /*=FALSE*/, BOOL bQueryData/*=FALSE*/)
{
static BOOL bProcessed = FALSE;
	if (bProcessed)
		return;
	bProcessed = TRUE;
	//지표 돌려보기를 멈춘다. ===================================================================
	GetParent()->PostMessage(UMSG_STOP_JIPYOCYCLE, 0, 0);
	//===========================================================================================
	
	CPoint ptOutput(m_lColSelected,m_lRowSelected);
	if(szNewPos.y>=m_lRows||szNewPos.x>=m_lCols
		||((szNewPos.x ==m_lColSelected)&&(szNewPos.y==m_lRowSelected))) 
	{
		if(!bInvalidateEvenIfPosisequal)
		{
			bProcessed = FALSE;
			return;
		}
	}
	ptOutput.x = m_lColSelected;
	ptOutput.y = m_lRowSelected;
	m_lRowSelected = szNewPos.y;
	m_lColSelected = szNewPos.x;
	InvalidateRect(GetRectOfChart(ptOutput.y,ptOutput.x,TRUE));
	InvalidateRect(GetRectOfChart(m_lRowSelected,m_lColSelected,TRUE));
	//2007.01.29 by LYH
	if( m_bLastEnvUpdated && m_pChartItems[m_lRowSelected][m_lColSelected]->GetSafeHwnd())
		m_pChartItems[m_lRowSelected][m_lColSelected]->SetFocus();
	//}}

	//===============================================================================
	::SendMessage( GetParent()->GetParent()->GetSafeHwnd(), RMSG_SAVE_FOCUS_CONTROL, 0, 0);
	//===============================================================================

// --> [Edit]  강지원 2008/10/31	( 종목변경일때문 종목코드 컨트롤에 종목을 변경해주는것 같아서 막음... )
//	int nMode = ((CInputBarDlg*)m_pInputBarDlg)->GetCurrentMode();
//	if(nMode == CHANGE_ITEM)
// <-- [Edit]  강지원 2008/10/31
	{
		if(bQueryData)
			SendInitChartData(TRUE, TRUE);
		else
			SendInitChartData();
	}
	
	
	//2005. 09. 05 added by sy, nam ==================================
	//CString strCode = GetpChartItemSelected()->GetDataCode();
	//m_pInputBarDlg->SendMessage(UMSG_SETTEXT_CODE_INPUTBAR, 0, (LPARAM)(LPTSTR)(LPCTSTR)strCode);
	//================================================================
	bProcessed = FALSE;
	return;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	Input	:	szNewPos  x, y 차트의 위치
//	목적	:	조회후 결과 Data를 툴바로 보낸다.
///////////////////////////////////////////////////////////////////////////////
void CChartMng::SendInitChartData(BOOL bNotCodeInputInit/*= TRUE*/, BOOL bFromTRData /*= FALSE */)
{
	stChartInitData _stData;
	double dStartDate = 0, dEndDate = 0;
	CChartItem* pChartItem = NULL;
	pChartItem = GetpChartItemSelected();
	if(pChartItem == NULL) return;
	// (2008/11/7 - Seung-Won, Bae) Check usefull control.
	if( !pChartItem->GetSafeHwnd()) return;
	if( !IsWindow( pChartItem->GetSafeHwnd())) return;

// --> [Edit]  강지원 2008/12/15	( 데이터 추가조회 갯수가 2000을 넘으면 2000개로 고정하고 그 수량을 저장한다. )
	_stData.m_lTotalCnt = pChartItem->GetAllCnt();
//	_stData.m_lTotalCnt = pChartItem->m_nSaveAllCount;
	if ( _stData.m_lTotalCnt > 2000 )
		_stData.m_lTotalCnt = 2000;

//	_stData.m_lTotalCnt = pChartItem->m_nSaveAllCount;
//	pChartItem->SetSaveAllCount(_stData.m_lTotalCnt);
// <-- [Edit]  강지원 2008/12/15
// --> [Edit]  강지원 2008/12/12
//	long nDataCnt = pChartItem->GetNFullDataCount();
//	if ( nDataCnt > 2000 )
//		_stData.m_lTotalCnt = 2000;
//	else
//		_stData.m_lTotalCnt = nDataCnt;
// <-- [Edit]  강지원 2008/12/12

	pChartItem->GetStartEndDateOnView(&dStartDate,&dEndDate);
	_stData.m_lStartDate = (long)dStartDate;
	_stData.m_lEndDate = (long)dEndDate;
	if(pChartItem->GetBWholeView())
		_stData.m_lZoomValue = _stData.m_lTotalCnt;
	else	
		_stData.m_lZoomValue = pChartItem->GetNOnePageDataCount();
	// 멀티차트의 선택여부 : 복수종목 - ojtaso (20070525)
	ST_SELCHART* lpSelChart = NULL;
	if(pChartItem->GetSelChart(NULL, lpSelChart))
		_stData.m_strCode = lpSelChart->m_strItemCode;
	else
		_stData.m_strCode = pChartItem->GetDataCode();
	_stData.m_chType = pChartItem->GetType();
	
	_stData.m_strTick	= pChartItem->GetTickNumber();
	_stData.m_strSec	= pChartItem->GetSecNumber();
	_stData.m_strMin	= pChartItem->GetMinNumber();	
	_stData.m_strDay	= pChartItem->GetDayNumber();
	_stData.m_strHour	= pChartItem->GetHourNumber();

	//_stData.m_bRateValue = pChartItem->GetUseRateValue();
	_stData.m_nChartNumericValue = pChartItem->GetChart_NumericValueType();
	_stData.m_bFackedCandle = pChartItem->GetBNormalizedMinuteTimeRule();

	//=============================================================
	//2005.05.24 add by sy, nam
	//현재 선택된 Row,col을 넘긴다.
	_stData.m_nSelCol = m_lColSelected;
	_stData.m_nSelRow = m_lRowSelected;
	//=============================================================


	//=============================================================
	//2005.08.22 add by sy, nam
	_stData.m_bRevised	= pChartItem->m_CfgInfo.bRevisedValue;
	_stData.m_bLog		= pChartItem->m_CfgInfo.bShowLog;	

	//=============================================================
	//2005.08.23 add by sy, nam
	_stData.m_strChartLineType	= pChartItem->m_strChartLineType;
	
	//=============================================================
	//2005.08.25 add by sy, nam
	//시작일 선택 여부
	_stData.m_bStartDateState	= pChartItem->m_CfgInfo.bStartDate;

	//============================================================
	//2005. 09. 05 한화 뷁
	if(bNotCodeInputInit == FALSE) // bNotCodeInputInit이 False로 오는 경우는 단 한번 불린다.
	{
		_stData.m_strCode = "NOCHANGE";
	}
	//==============================================================

// --> [Edit]  이도영 2008/09/11
// 
	_stData.m_strOrderType		= pChartItem->m_strOrderType;
// <-- [Edit]  이도영 2008/09/11

// --> [Edit]  강지원 2008/10/15	//	체결/호가 버튼 값 저장
	_stData.m_nCheHoga			= pChartItem->m_nCheHoga;
// <-- [Edit]  강지원 2008/10/15

// --> [Edit]  강지원 2008/11/23	( 다음키값에 따라 다음조회 버튼 활성화 )
	if(m_pInputBarDlg)
	{
		if(lpSelChart)
		{
			CString sKey = lpSelChart->m_strKey;
			sKey.TrimLeft();	sKey.TrimRight();

			//	전체 데이터갯수에 추가 데이터 조회갯수를 더한값이 최대 조회 갯수를 넘으면 데이터 추가조회 버튼을 막는다. 
			if( sKey.IsEmpty() || atol(sKey) == 0 || pChartItem->GetNFullDataCount() >= CHARTDATA_MAX_CNT || ((CInputBarDlg*)m_pInputBarDlg)->GetCheHogaType() == 1 )
				((CInputBarDlg*)m_pInputBarDlg)->EnableRequestMoreButton(FALSE);
			else
				((CInputBarDlg*)m_pInputBarDlg)->EnableRequestMoreButton(TRUE);
		}
	}
// <-- [Edit]  강지원 2008/11/23
//	if(((CStdDialog*)GetParent())->GetpMainBaseInfo()) // gm0604 added ..
//	{
		//============================================================
		//2005. 09. 05 한화 뷁
		long lFromTRdata = 0;
		if(bFromTRData)
			lFromTRdata  = 1;
		//============================================================

// --> [Edit]  강지원 2008/11/24	( 데이터 전체갯수를 설정한다. )
		if(m_pInputBarDlg)
		{
			((CInputBarDlg*)m_pInputBarDlg)->SetMultiTotalCount(pChartItem->GetNFullDataCount());
			m_pInputBarDlg->SendMessage(UMSG_SETINITIALCHARTDATA,lFromTRdata,(LPARAM)&_stData);
		}
// <-- [Edit]  강지원 2008/11/24
//	}
	
	if(m_pLeftBarDlg)
	{
		m_pLeftBarDlg->SendMessage(UMSG_SETINITIALCHARTDATA,0,(LPARAM)&_stData);
	}	

	//=================================================================================
	// 2005. 05. 24 add by sy, nam
	// %, \, $ 표시가 기존의 CInputBarDlg에서 CExtInputBarDlg로 옮겨지면서...추가한다.
	//=================================================================================
	if(m_pExtInputBarDlg)
	{
		m_pExtInputBarDlg->SendMessage(UMSG_SETINITIALCHARTDATA,0,(LPARAM)&_stData);
	}	
	//=================================================================================

//	if(bFromTRData)
//	{
//		//종목 코드를 셋한
//		m_pInputBarDlg->PostMessage(UMSG_SETTEXT_CODE_INPUTBAR, 0, (LPARAM)(LPTSTR)(LPCTSTR)_stData.m_strCode);
//	}
//	OutputDebugString("Send Init..........................");

/*	//차트를 옮긴후 
	MoveFloatingHogaDlg();
	//호가 조회를 날린다.
	//2005. 07. 15 == 주식 종목에서는 Hoga를 요청한다.		
	if(m_nMarketType == STOCK_CHART)
	{
		CString strCode = pChartItem->GetDataCode(); strCode.TrimLeft(); strCode.TrimRight();
		if(strCode == CODE_STOCK_LENGTH)
			GetParent()->SendMessage(UMSG_SENDTR_HOGA_DATA, (WPARAM)&strCode, 0);
	}
*/
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	Input	:	szNewPos  x, y 차트의 위치
//	목적	:	차트에 사용될 차트, 지표 List를 가져온다.
///////////////////////////////////////////////////////////////////////////////
CString CChartMng::GetIndicatorNameList(LPCTSTR p_strGroup)
{
	CChartItem* pChartItem = NULL;
	pChartItem = GetpChartItemSelected();
	if(pChartItem == NULL) return _T("");

	return GetpChartItemSelected()->GetIndicatorNameList(p_strGroup);
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	Input	:	p_strGroup  지표 그룹명
//				pStrArray   결과값
//	목적	:	차트에 사용될 차트, 지표 List를 가져온다.
///////////////////////////////////////////////////////////////////////////////
void CChartMng::GetIndicatorNameListIntoArray(CStringArray *pStrArray, LPCTSTR p_strGroup)
{
	pStrArray->RemoveAll();
	if(p_strGroup[0]==NULL || p_strGroup == "전체지표")
	{
		CString strList = GetIndicatorNameList("");
		CString strALine;
		CString strALineTitle;
		CString strAItem;
		int nPos = 0;
		int nLength = 0;
// (2008/9/13 - Seung-Won, Bae) Remove Volume Index
		pStrArray->Add("거래량차트");
		if(strList.GetLength())
		{
			while(1)
			{
				nPos = strList.Find(':');
				if(nPos==-1) break;		
				if(nPos==0) 
				{
					nPos = strList.Find('\n');
					strList.Delete(0,nPos+1);
					continue;
				}			
				strALineTitle = strList.Left(nPos);
				if(strALineTitle=="기본지표"
					||strALineTitle=="시장지표"
					||strALineTitle=="투자자동향"
					||strALineTitle=="특수지표")
				{
					nPos = strList.Find('\n');
					strList.Delete(0,nPos+1);
					continue;
				}

				strList.Delete(0,nPos+1);
				nPos = strList.Find('\n');
				strALine = strList.Left(nPos-1);		
				strList.Delete(0,nPos+1);

				while(1)
				{
					nLength = strALine.GetLength();
					if(!nLength) break;
					nPos = strALine.Find(',');
					if(nPos==-1) 
					{
						strAItem = strALine;
						strALine.Empty();
					}
					else
					{
						strAItem = strALine.Left(nPos);
						strALine.Delete(0,nPos+1);
					}

					// (2008/9/13 - Seung-Won, Bae) for Multi-Language
					_MHWND( NULL);
					strAItem = _MTOL2( strAItem);

					pStrArray->Add(strAItem);
				}
			}		
		}		
	}
	else
	{
		CString strGroup = p_strGroup;
		if(strGroup=="특수지표")
		{
			pStrArray->Add("캔들차트");
			pStrArray->Add("바차트");
			pStrArray->Add("선차트");
			pStrArray->Add("FLOW차트");
			//pStrArray->Add("점차트");
			//pStrArray->Add("렌코차트");
			//pStrArray->Add("삼선전환도");
// (2008/9/13 - Seung-Won, Bae) Remove Volume Index
//			pStrArray->Add("캔들볼륨차트");
//			pStrArray->Add("이큐볼륨차트");
		}

		CString strALine = GetIndicatorNameList(p_strGroup);
		CString strAItem;
		int nPos = 0;
		int nLength = 0;

		if(strALine.GetLength())
		{
			while(1)
			{
				nLength = strALine.GetLength();
				if(!nLength) break;
				nPos = strALine.Find(',');
				if(nPos==-1) 
				{
					strAItem = strALine;
					strALine.Empty();
				}
				else
				{
					strAItem = strALine.Left(nPos);
					strALine.Delete(0,nPos+1);
				}
				//if(strAItem=="P&F")
				//{
				//	strAItem = "P&&F";
				//}
				if(strAItem=="가격차트")
				{
					continue;
				}

				// (2008/9/13 - Seung-Won, Bae) for Multi-Language
				if ( strAItem.Left(1) == "_" )
				{
					_MHWND( NULL);
					strAItem = _MTOL2( strAItem);
				}

				pStrArray->Add(strAItem);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	Input	:	p_strGroup  지표 그룹명
//				rxlistCtrl  결과값이 들어갈 ListCtrl
//	목적	:	차트에 사용될 차트, 지표 List를 가져온다.
///////////////////////////////////////////////////////////////////////////////
void CChartMng::GetIndicatorNameListIntoListCtrl(CXListCtrl& rxlistCtrl, LPCTSTR p_strGroup)
{
	if(p_strGroup[0]!=NULL)
	{
		CString strALine = GetIndicatorNameList(p_strGroup);
		CString strAItem;
		int nPos = 0;
		int nItem = 0;
		int nLength = 0;
		rxlistCtrl.DeleteAllItems();
		if(!strcmp(p_strGroup,"특수지표"))
		{
			rxlistCtrl.InsertItem(0, "");
			rxlistCtrl.SetItemText(0, 0, "캔들차트",RGB(0,0,0),CRGRID);

			rxlistCtrl.InsertItem(1, "");
			rxlistCtrl.SetItemText(1, 0, "바차트");

			rxlistCtrl.InsertItem(2, "");
			rxlistCtrl.SetItemText(2, 0, "선차트",RGB(0,0,0),CRGRID);

			//rxlistCtrl.InsertItem(3, "");
			//rxlistCtrl.SetItemText(3, 0, "점차트");

			//rxlistCtrl.InsertItem(4, "");
			//rxlistCtrl.SetItemText(4, 0, "렌코차트");

			//rxlistCtrl.InsertItem(4, "");
			//rxlistCtrl.SetItemText(4, 0, "삼선전환도",RGB(0,0,0),CRGRID);

			//rxlistCtrl.InsertItem(6, "");
			//rxlistCtrl.SetItemText(6, 0, "이큐볼륨차트");
			
			//rxlistCtrl.InsertItem(7, "");
			//rxlistCtrl.SetItemText(7, 0, "캔들볼륨차트",RGB(0,0,0),CRGRID);
			
			nItem = 3;
		}

		if(strALine.GetLength())
		{
			while(1)
			{
				nLength = strALine.GetLength();
				if(!nLength) break;
				nPos = strALine.Find(',');
				if(nPos==-1) 
				{
					strAItem = strALine;
					strALine.Empty();
				}
				else
				{
					strAItem = strALine.Left(nPos);
					strALine.Delete(0,nPos+1);
				}
				//if(strAItem=="P&F")
				//{
				//	strAItem = "P&&F";
				//}
				rxlistCtrl.InsertItem(nItem, "");
				if(nItem%2) // odd
				{
					rxlistCtrl.SetItemText(nItem, 0, strAItem);
				}
				else		// even
				{
					rxlistCtrl.SetItemText(nItem, 0, strAItem,RGB(0,0,0),CRGRID);					
				}				
				nItem++;
			}
		}
	}
	else
	{
		CString strList = GetIndicatorNameList("");
		CString strALine;
		CString strALineTitle;
		CString strAItem;
		int nPos = 0;
		int nItem = 0;
		int nLength = 0;
		rxlistCtrl.DeleteAllItems();
		rxlistCtrl.InsertItem(nItem, "");
		rxlistCtrl.SetItemText(nItem, 0, "거래량차트",RGB(0,0,0),CRGRID);
		nItem++;

		if(strList.GetLength())
		{
			while(1)
			{
				nPos = strList.Find(':');
				if(nPos==-1) break;		
				if(nPos==0) 
				{
					nPos = strList.Find('\n');
					strList.Delete(0,nPos+1);
					continue;
				}			
				strALineTitle = strList.Left(nPos);
				if(strALineTitle=="기본지표"
					||strALineTitle=="시장지표"
					||strALineTitle=="투자자동향"
					||strALineTitle=="특수지표")
				{
					nPos = strList.Find('\n');
					strList.Delete(0,nPos+1);
					continue;
				}

				strList.Delete(0,nPos+1);
				nPos = strList.Find('\n');
				strALine = strList.Left(nPos-1);		
				strList.Delete(0,nPos+1);
				
				while(1)
				{
					nLength = strALine.GetLength();
					if(!nLength) break;
					nPos = strALine.Find(',');
					if(nPos==-1) 
					{
						strAItem = strALine;
						strALine.Empty();
					}
					else
					{
						strAItem = strALine.Left(nPos);
						strALine.Delete(0,nPos+1);
					}
					rxlistCtrl.InsertItem(nItem, "");
					if(nItem%2) // odd
					{
						rxlistCtrl.SetItemText(nItem, 0, strAItem);
					}
					else		// even
					{
						rxlistCtrl.SetItemText(nItem, 0, strAItem,RGB(0,0,0),CRGRID);					
					}				
					nItem++;
				}
			}		
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	Input	:	
//				WPARAM	: MESSAGE
//				LPARAM	: MSG Structure Address
//	목적	:	차트에서 오는 Message 들을 처리한다.
///////////////////////////////////////////////////////////////////////////////
LRESULT CChartMng::OnEventMessage( WPARAM wParam, LPARAM lParam)
{	
	MSG *pMsgFromChart = (MSG *)lParam;
	//TRACE1("OnEventMessage %d\n", pMsgFromChart->message);
	switch(pMsgFromChart->message)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_SETFOCUS:	// 종목 drag&grop시 해당 차트에 focus가 가도록 함..	
		{	
			m_bOnEventMessage = TRUE;
			CDRChartOcx *pChart = (CDRChartOcx *)GetDlgItem(wParam);		
			if(pChart)
			{				
				EACH_CHARTJOB_START()
					if(pChart==pChartItem)
					{
						//CString strTemp;
						//strTemp.Format("SETFOCUS %d::%d\r\n", nColIndex, nRowIndex);
						//OutputDebugString(strTemp);
						//if( pMsgFromChart->message == WM_LBUTTONDOWN )
						//2007.01.17 add by LYH  툴바버튼 동기화(이전 선택 세팅)
						int m_lColSelectedOld = m_lColSelected;
						int m_lRowSelectedOld = m_lRowSelected;
						//}}

						BOOL bOld = ((CStdDialog*)GetParent())->m_nCodeInput;
						((CStdDialog*)GetParent())->m_nCodeInput = FALSE;
						SetChartSelected(CPoint(nColIndex,nRowIndex));
						((CStdDialog*)GetParent())->m_nCodeInput = bOld;

						//2007.01.17 add by LYH  툴바버튼 동기화(정보를 얻어온다)
						if(m_lColSelectedOld!=nColIndex || m_lRowSelectedOld != nRowIndex)
						{
							// 멀티차트의 선택여부 : 복수종목 - ojtaso (20070525)
							SetSelChangeChart(TRUE);

							GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 0, 50000);
							GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50031, pChartItem->m_bDrawAutoTrendLine );
							GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50032, pChartItem->m_bDrawAutoPattern );
							GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50033, pChartItem->m_bDrawCandlePattern );
							GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50026, pChartItem->m_bDrawNews );

							if(m_pExtInputBarDlg)
							{
								m_pExtInputBarDlg->SendMessage(UMSG_SYNC_TOOLBAR, CToolOptionInfo::TOOLOPTION::T_CROSSHAIRS_TOOL, 0);
							}
							pChartItem->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::T_ALL_TOOL_TRUE_QUERY, 0);
						}
						//}}
						break;
					}	
				EACH_CHARTJOB_END()	
			}	
			m_bOnEventMessage = FALSE;
		}
		break;
	case WM_KEYUP:
		{
			if(pMsgFromChart->wParam==VK_TAB)
			{
				SHORT wShiftKeyDown = ::GetAsyncKeyState(VK_SHIFT);
				CDRChartOcx *pChart = (CDRChartOcx *)GetDlgItem(wParam);
				if(pChart)
				{
					int nColIndexCurrent = 0;
					int nRowIndexCurrent = 0;
					EACH_CHARTJOB_START()
					if(pChart==pChartItem)
					{
						nColIndexCurrent = nColIndex;
						nRowIndexCurrent = nRowIndex;
					}
					EACH_CHARTJOB_END()
					if(wShiftKeyDown)
					{
						if(nColIndexCurrent>0)
						{
							nColIndexCurrent--;
						}
						else if(nRowIndexCurrent>0)
						{
							nRowIndexCurrent--;
							nColIndexCurrent = GetCols() - 1;
						}
						else
						{
							nColIndexCurrent = GetCols() - 1;
							nRowIndexCurrent = GetRows() - 1;
						}
					}
					else
					{
						if((nColIndexCurrent+1)<GetCols())
						{
							nColIndexCurrent++;
						}
						else if((nRowIndexCurrent+1)<GetRows())
						{
							nRowIndexCurrent++;
							nColIndexCurrent = 0;
						}
						else
						{
							nColIndexCurrent = 0;
							nRowIndexCurrent = 0;
						}
					}
					SetChartSelected(CPoint(nColIndexCurrent,nRowIndexCurrent));
				}
			}
		}
		break;
// 		case UWM_ZOOMVALUE:
// 			{
// 				CChartItem* pChartItem = NULL;
// 				pChartItem = GetpChartItemSelected();
// 				if(pChartItem == NULL) 
// 					break;
// 
// 				CDRChartOcx *pChart = (CDRChartOcx *)GetDlgItem(wParam);		
// 				if(GetpChartItemSelected()==pChart&&m_bLastEnvUpdated)
// 				{
// 					//R버튼으로 발생하는 전체 초기화 라는 의미 05.12.05 선영대리 요청=================================
// 					if(pMsgFromChart->wParam == 0) 
// 					//=============================================================================
// 					if(m_pInputBarDlg)
// 					{
// 						//>> 기간 초기화시 주기 초기화 처리 - vntsorl(20090217)
// 						((CInputBarDlg*)m_pInputBarDlg)->ResetZoomValue(pMsgFromChart->lParam);
// 						//<< 기간 초기화시 주기 초기화 처리 - vntsorl(20090217)
// 						m_pInputBarDlg->SendMessage(UMSG_SETZOOMVALUE,pMsgFromChart->lParam);
// 					}
// 				}
// 			}		
// 			break;
	case UWM_VIEWENDTIME:
		return 0l;

	case UWM_USEINQUIRY_CROSSLINE:
		{
			CChartItem *pChart = (CChartItem *)GetDlgItem(wParam);
			EACH_CHARTJOB_START()
				if(pChartItem!=pChart)
				{
					pChartItem->SetNUseInquiryWithCrossline(pMsgFromChart->wParam);
				}				
			EACH_CHARTJOB_END()
		}
		break;
	case UWM_FIREGSHAREDATA: // Global 공유가 없어져 므로... 안써버린다.
		{
			CString strCodeShare = (LPCTSTR)pMsgFromChart->lParam;			
			int nSize = strCodeShare.GetLength();
			int nPos = strCodeShare.Find(';');
			if(nPos<1) return 1L;
			CString strCodeHead = strCodeShare.Left(nPos);
			CString strCodeTail = strCodeShare.Right(nSize-(nPos+1));
			if (strCodeHead=="시간-차트")
			{
				/* solomon
				CChartItem *pChart = (CChartItem *)GetDlgItem(wParam);

				SetTimeMark((LPCTSTR)strCodeTail,TRUE);
				*/
			}
			else
			{
				STSHAREDDATA stData;
				stData.m_szKey = (LPCTSTR)strCodeHead;
				stData.m_szValue = (LPCTSTR)strCodeTail;
				if(m_pLeftBarDlg)
				{
					m_pLeftBarDlg->SendMessage(UMSG_SETCHARTHOGA,NULL,(LPARAM)&stData);
				}	
			}

		}
		break;
	case UWM_DRAGSTART:
		{
			m_lDragStart = pMsgFromChart->lParam;
		}
		break;
	case UWM_DRAGEND:
		{
			m_lDragEnd = pMsgFromChart->lParam;
			CChartItem *pChart = (CChartItem *)GetDlgItem(wParam);
			if(pChart->GetDataTRCode()==QUERY_STRFOFOREIGN)
			{
				SetSendJongData(pChart->GetDataCode(),pChart->GetType(),pChart->GetTickMinDayNumber());
			}			
		}
		break;
	case UWM_ORGRETURN:
		{
			// 선택된 차트에 새로 요청할 데이터 default 갯수 셋팅
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem == NULL) 
				return 0;

			if( ((CStdDialog*)GetParent())->m_nChartType==MINI_MULTI_CHART ||
				((CStdDialog*)GetParent())->m_nChartType==MINI_SINGLE_CHART )
				pChartItem->SetCntForRequest(90);
			else
				pChartItem->SetCntForRequest(DEFAULTCNTATONCE);
//			pChartItem->SetSaveAllCount(0);

			if(!m_bReplaychartMode) //06.02.03  복기 시 초기화로 인한 TR 전송하는 것을 막는다.
			{
				pChartItem->m_bDrawAutoTrendLine = 0;
				pChartItem->m_bDrawAutoPattern = 0;
				pChartItem->m_bDrawCandlePattern = 0;
				pChartItem->m_bDrawNews = 0;
				GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50031, pChartItem->m_bDrawAutoTrendLine );
				GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50032, pChartItem->m_bDrawAutoPattern );
				GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50033, pChartItem->m_bDrawCandlePattern );
				GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50026, pChartItem->m_bDrawNews );

				m_bHideScale = FALSE;
				ShowScale();
				m_bHideInnerToolBar = FALSE;
				ShowInnerToolbar();
				GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50040, pChartItem->m_bDrawCandlePattern );
				GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50041, pChartItem->m_bDrawNews );
				//초기화 시키도록 정보 제공.
				if(m_pExtInputBarDlg->GetSafeHwnd())
					m_pExtInputBarDlg->SendMessage(UMSG_CHART_INIT_FROMOCX);

				//초기화 후 다시 TR재전송
				if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
					m_pInputBarDlg->SendMessage(UMSG_CHART_INIT_FROMOCX);		

				pChartItem->InvokeAddIn("ChartPatternAddin.dll",0,200);
				pChartItem->InvokeAddInSTIndex("ChartSTIndexAddIn.dll",200,0);	// 시스템트레이딩 전체초기화

				// (2008/9/14 - Seung-Won, Bae) for Pair Tick
				if( pChartItem->GetType() == TICK_DATA_CHART && atoi( pChartItem->GetTickMinDayNumberRequested()) == 1
					&& pChartItem->m_nCheHoga == 1)	// (2008/11/4 - Seung-Won, Bae) Do not support Buy/Sell for Che Chart.
				{
					( ( CStdDialog *)GetParent())->StartRequestPairTick( pChartItem->GetSelectedItemCode());
					RequestChartData();
				}
				else	( ( CStdDialog *)GetParent())->ClearPairTick( pChartItem->GetSelectedRQ());
			}		
		}
		break;
		// 복수종목 차트 선택 : 복수종목 - ojtaso (20070122)
	case UWM_SELECTEDCHART:
		{
			if( pMsgFromChart->lParam)
			{
				// 선택된 차트 정보는 외부dll에서 관리토록 수정 : 복수종목 - ojtaso (20070504)
				CChartItem* pSelItem = GetpChartItemSelected();
				if(pSelItem)
				{
					LPCTSTR pSelRQ = (LPCTSTR)pMsgFromChart->lParam;
					ST_SELCHART* pSelChart = NULL;

					// 삭제된 RQ는 저장하지 않도록 수정 - ojtaso (20080509)
					if(pSelItem->GetSelChart(pSelRQ, pSelChart, pMsgFromChart->wParam))
					{
						if(!pMsgFromChart->wParam)	// 차트 삭제
						{
							((CStdDialog*)GetParent())->OnDeleteChartItem(pSelChart->m_strItemCode);
							pSelItem->OnDeleteChartItem(pSelRQ, pSelChart);
						}
						else						// 선택됨
						{
							((CStdDialog*)GetParent())->OnSelectChartItem(pSelChart);
							pSelItem->OnSelectChartItem(pSelChart);
// --> [Edit]  강지원 2008/11/08
//							SendInitChartData();
// <-- [Edit]  강지원 2008/11/08
						}
					}
				}

			}
		}
		break;
		// 복수종목의 Total Count를 멀티차트에 알려줌 : 복수종목 - ojtaso (20070704)
	case UWM_MULTITOTALCOUNT:
		if(m_pInputBarDlg)
		{
// --> [Edit]  강지원 2008/11/03
			//	선택된 ChartItem의 핸들이 같을경우에만 리얼 업데이트를 해준다. 
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem == NULL) 
				return 0;

			CDRChartOcx *pChart = (CDRChartOcx *)GetDlgItem(wParam);		
			if(pChart)
			{				
				if(pChart == pChartItem)
				{
					((CInputBarDlg*)m_pInputBarDlg)->SetMultiTotalCount(pMsgFromChart->lParam);
				}
			}
// <-- [Edit]  강지원 2008/11/03
		}
		break;
		
	// 2008.10.15 by LYH >> 갭보정		
	case UWM_USE_GAPREVISION:
		{
			CChartItem* pSelChartItem = NULL;
			pSelChartItem = GetpChartItemSelected();
			if(!pSelChartItem) return FALSE;
			pSelChartItem->ClearData();
			RequestChartData();
		}
		break;
// --> [Edit]  강지원 2008/10/31
	case UDM_NOTIFY_TIMEDIFF :
		{
			//	현재 RQ의 선택되어 있는 시간대의 Index 구하기
			int index = pMsgFromChart->wParam;
//			if ( index < 1 )
//			{
//				CChartItem* pSelectedChartItem = GetpChartItemSelected();
//				if( pSelectedChartItem)
//				{
//					ST_CHARTITEM_REALDATA* pChartItemRealData = NULL;
//					if(pSelectedChartItem->m_mapChartItemRealData.Lookup(CStdDialog::GetRealItemCode((LPCTSTR)pSelectedChartItem->GetSelectedItemCode()), (LPVOID&)pChartItemRealData))
//					{
//						index = atoi(pChartItemRealData->m_strTimeIndex);
//					}
//				}				
//			}

			((CStdDialog*)GetParent())->SetTabTimeIndex(index);
			SetTimeGab(index, FALSE);
		}
		break;
// <-- [Edit]  강지원 2008/10/31
	// 2008.10.15 by LYH <<
	}
	//2007.01.17 add by LYH  툴바버튼 동기화
	if(pMsgFromChart->message == RMS_ANALYSIS_FUNC)
	{
		//툴바에 없는 메시지 리턴
		if(pMsgFromChart->wParam == CToolOptionInfo::TOOLOPTION::T_DRAG_X_ZOOM 
			|| pMsgFromChart->wParam == CToolOptionInfo::TOOLOPTION::T_WHOLEVIEW
			|| pMsgFromChart->wParam == CToolOptionInfo::TOOLOPTION::T_TOOL_SELECTION)
			return 0;
		//end

		if(pMsgFromChart->wParam == CToolOptionInfo::TOOLOPTION::T_CROSSHAIRS_TOOL
			|| pMsgFromChart->wParam == CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_LEFT
			|| pMsgFromChart->wParam == CToolOptionInfo::TOOLOPTION::T_ADDIN_AUTOSCROLL_PLAY_RIGHT)
		{
			m_pExtInputBarDlg->SendMessage(UMSG_SYNC_TOOLBAR,pMsgFromChart->wParam, pMsgFromChart->lParam);
			return 0;
		}

		CChartItem* pChartItem = NULL;
		pChartItem = GetpChartItemSelected();
		if(pChartItem == NULL) 
			return 0;

		CDRChartOcx *pChart = (CDRChartOcx *)GetDlgItem(wParam);		
		if(pChartItem==pChart)
		{
			int nID = (int)pMsgFromChart->wParam;
			CString strData;
			strData.Format("%d | %d",nID , pMsgFromChart->lParam);
			//AfxMessageBox(strData);
			GetParent()->SendMessage(UMSG_SYNC_TOOLBAR,pMsgFromChart->wParam, pMsgFromChart->lParam);				
		}
	}		
	//end
	return 0;
}

LRESULT CChartMng::OnGetUserID( WPARAM wParam, LPARAM lParam)
{
//	return (long)(LPCSTR)((CStdDialog*)GetParent())->m_strUserID;
	
	//@Solomon-091106 차트파일이 너무 많이 생성되어 Chart폴더를 별도로 처리하기위해.
	//OCX에서 이것을 기준으로 처리를 하고 있는 듯함.
	CString szID;
	szID.Format("%s%s", ((CStdDialog*)GetParent())->m_strUserID, "\\Chart");
	return (long)(LPCSTR)szID;
}

LRESULT CChartMng::OnMagnifyingGlass( WPARAM wParam, LPARAM lParam)
{
	((CStdDialog*)GetParent())->OnMagnifyingglass((BOOL)wParam);
	return 0L;
}

void CChartMng::SetTimeMark(LPCTSTR p_szMarkTime, BOOL p_bWithScroll)
{
	/* solomon
	EACH_CHARTJOB_START()
		pChartItem->SetTimeMark(p_szMarkTime,p_bWithScroll);
	EACH_CHARTJOB_END()	
	*/
}

//20080914 이문수 >>
void CChartMng::Paint(CDC * pDC, BOOL bPrint/*=FALSE*/) 
{
	CRect clientRc;
	GetClientRect(&clientRc);	

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, clientRc.Width(), clientRc.Height());
	CBitmap * obmp = memDC.SelectObject(&bmp);

	
	CRect rect;
	GetClientRect(rect);
	memDC.FillSolidRect(rect,RGB(128,135,145));
	rect.DeflateRect(1,1,1,1);
	memDC.FillSolidRect(rect, RGB(237,243,249));
	EACH_CHARTJOB_START()
		PaintTitle(nRowIndex,nColIndex,pChartItem,&memDC);	
		
		//20080919 이문수 >>
		if(bPrint)
		{
			CRect rectChart		= GetRectOfChart(nRowIndex,nColIndex, 0);// 챠트=0, 타이틀=1
			
			CRect rectChartClient;
			pChartItem->GetClientRect(rectChartClient);

			CDC * pChartDC		= pChartItem->GetDC();
			CDC memChartDC;
			memChartDC.CreateCompatibleDC(pChartDC);			

			CBitmap bmpChart, * pOldBmpChart;
			bmpChart.CreateCompatibleBitmap(pChartDC, rectChartClient.Width(), rectChartClient.Height());
			pChartItem->ReleaseDC(pChartDC);

			pOldBmpChart = (CBitmap *)memChartDC.SelectObject(&bmpChart);
			pChartItem->SendMessage( UDM_DRAW_FOR_PRINT, 0, ( LPARAM)memChartDC.GetSafeHdc());
			
			memDC.StretchBlt(rectChart.left, rectChart.top, rectChart.Width(), rectChart.Height(), 
				&memChartDC, 0, 0, rectChartClient.Width(), rectChartClient.Height(), SRCCOPY);

			memChartDC.SelectObject(pOldBmpChart);


			CBrush * pOldBrush	= (CBrush *)memDC.SelectStockObject(NULL_BRUSH);
			memDC.Rectangle(rectChart);
			CRect rectChartTitle= GetRectOfChart(nRowIndex,nColIndex, 1);// 챠트=0, 타이틀=1		
			memDC.Rectangle(rectChartTitle);


			memDC.SelectObject(pOldBrush);
		}
		//20080919 이문수 <<

	EACH_CHARTJOB_END()

	pDC->BitBlt(clientRc.left, clientRc.top, clientRc.Width(), clientRc.Height(), &memDC, 0,0, SRCCOPY);


	memDC.SelectObject(obmp);
}
//20080914 이문수 <<

void CChartMng::OnPaint() 
{
	//20080914 이문수 >>
	CPaintDC dc(this);
	Paint(&dc);
	
//	CRect clientRc;
//	GetClientRect(&clientRc);
//	CPaintDC dc(this);
//
//	CDC memDC;
//	memDC.CreateCompatibleDC(&dc);
//	CBitmap bmp;
//	bmp.CreateCompatibleBitmap(&dc, clientRc.Width(), clientRc.Height());
//	CBitmap * obmp = memDC.SelectObject(&bmp);
//
//	CDC * pDC = &memDC;
//
//	//	CPaintDC dc(this); // device context for painting
//	CRect rect;
//	GetClientRect(rect);
//	pDC->FillSolidRect(rect,RGB(128,135,145));
//	rect.DeflateRect(1,1,1,1);
//	pDC->FillSolidRect(rect, RGB(237,243,249));	//RGB(222,222,222));
//	EACH_CHARTJOB_START()
//		PaintTitle(nRowIndex,nColIndex,pChartItem,pDC);			
//	//		pChartItem->Invalidate();
//	EACH_CHARTJOB_END()
//
//		dc.BitBlt(clientRc.left, clientRc.top, clientRc.Width(), clientRc.Height(), &memDC, 0,0, SRCCOPY);
//
//	memDC.SelectObject(obmp);
	//20080914 이문수 <<
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	Input	:	
//	목적	:	차트의 Title들을 Draw한다.
///////////////////////////////////////////////////////////////////////////////
void CChartMng::PaintTitle(int nRowIndex,int nColIndex,CChartItem* pChartItem,CDC *pDC)
{
	CRect rect;
	rect = GetRectOfChart(nRowIndex,nColIndex,TRUE);
	pChartItem->SetButton(rect);	
	if(rect.IsRectEmpty()) return;	
	COLORREF Start, End;

	CChartItem* pChartItem2 = NULL;
	pChartItem2 = GetpChartItemSelected();
	if(pChartItem2 == NULL) 
		return;

	if(pChartItem==GetpChartItemSelected())
	{
		Start = m_clStartSelected;
		End   = m_clEndSelected;
	}
	else
	{
		Start = m_clStartUnselected;
		End   = m_clEndUnselected;
	}

	pDC->FillSolidRect( &rect,  Start);
	CRect backup = rect;
//		
//	int r,g,b;//각 3가지 색상의 값의 차이..
//	float rStep, gStep, bStep;//각 3가지 값들 각각의 색상값의 차이..
//	float fStep;//최대 색상값 차이로 나누어서 한번에 이동할 옵셋..
//
//	//각 생상 차이를 구한다..
//	r = (GetRValue(End) - GetRValue(Start));
//	g = (GetGValue(End) - GetGValue(Start));
//	b = (GetBValue(End) - GetBValue(Start));
//
//	//가장 큰차이가 나는것을 구한다..
//	int Max = max( abs(r), max(abs(g), abs(b)) );
//
//	//그릴 사각형을 차이가 나는것 만큼.. 분할할 길이를 구한다..
//	fStep = (float)rect.Height()/(float)Max;     // 상-하 Gradation 
//
//	//각 색상별 단계 차이값..
//	rStep = r/(float)Max;
//	gStep = g/(float)Max;
//	bStep = b/(float)Max;
//
//	//r,g,b를 시작 색으로 재설정...
//	r = GetRValue(Start);
//	g = GetGValue(Start);
//	b = GetBValue(Start);
//
//	//색깔이 변해야 하는 횟수만큼.. 반복 그린다.
//	for(int iOnBand=0 ; iOnBand <Max; iOnBand++)  // 상-하 Gradation 
//	{
//		::SetRect( &rect, rect.left, int(fStep*iOnBand) + backup.top ,
//		    rect.right, backup.bottom );  
//		pDC->FillSolidRect( &rect,  RGB( r + rStep*iOnBand, g + gStep*iOnBand, b + bStep*iOnBand ) );
//	}	
//	pChartItem->DrawTitle(backup, pDC,&m_ftForTitle);

	if(pChartItem==GetpChartItemSelected())
	{
		if(pChartItem->GetBReplayChartMode())
		{

			//	OutputDebugString("Paint Tilte" + pChartItem->GetDataCode());
			pChartItem->DrawReplayChartTitle(backup, pDC,&m_ftForTitle,&m_ftNormal,m_clStartSelected);
		}
		else
			pChartItem->DrawTitle(backup, pDC,&m_ftForTitle,&m_ftNormal,m_clStartSelected);			
	}
	else
	{
		if(pChartItem->GetBReplayChartMode())
		{
			//	OutputDebugString("Paint Tilte" + pChartItem->GetDataCode());
			pChartItem->DrawReplayChartTitle(backup, pDC,&m_ftForTitle,&m_ftNormal,m_clStartUnselected);
		}
		else
			pChartItem->DrawTitle(backup, pDC,&m_ftForTitle,&m_ftNormal,m_clStartUnselected);
	}

	
}


void CChartMng::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	EACH_CHARTJOB_START()
	rect = GetRectOfChart(nRowIndex,nColIndex,TRUE);
	if(rect.PtInRect(point))
	{
		SetChartSelected(CPoint(nColIndex,nRowIndex));
		ExtendChart(!m_bChartExtended);

		// 호가 창 Show ---------------------------------------------------
		// 2005. 04. 28 by Nam
		//-----------------------------------------------------------------
		//HWND hParent = GetParent()->GetSafeHwnd();		
		//::SendMessage(hParent, UMSG_FORCE_SHOW_HOGAWND, m_bChartExtended, 0 );		
		//-----------------------------------------------------------------
		break;

	}
	EACH_CHARTJOB_END()		

	


	CWnd::OnLButtonDblClk(nFlags, point);
}

void CChartMng::MoveCharts(int nRowIndexOutside/*= -1*/,int nColIndexOutside/* = -1*/)
{
	if(nRowIndexOutside==-1&&nColIndexOutside==-1)
	{
		EACH_CHARTJOB_START()
		CRect rect = GetRectOfChart(nRowIndex,nColIndex);
		//2005. 06. 07 =======================================================================
		//새로이 생성되는 ChartOCX(chartItem)에게 Drds를 요청 가능한 ClassPointer를 넘겨준다.
		//added by sang yun, nam
		if(m_lMltchrtDlgParentClsPointer > 0)
		{
			pChartItem->SetOutsideCommonInterface(m_lMltchrtDlgParentClsPointer); 
			pChartItem->m_CfgInfo.bShowLog = pChartItem->GetLogScale(_T("")) ? true : false;
			//pChartItem->SetRootPath(m_strRootPath);
		}
		//====================================================================================
		if( pChartItem->GetSafeHwnd()) pChartItem->MoveWindow(rect);	
		EACH_CHARTJOB_END()

	//	if(m_pFloatingDlg && m_pFloatingDlg->GetSafeHwnd())
	//		m_pFloatingDlg->SetWindowPos(	&CWnd::wndTopMost,
	//										m_pRectFloatingHoga->left,
	//										m_pRectFloatingHoga->top,
	//										m_pRectFloatingHoga->Width(),
	//										m_pRectFloatingHoga->Height(),
	//										SWP_NOMOVE | SWP_NOSIZE |SWP_SHOWWINDOW);
	}
	else// gm0604 added 2005.10.10
	{
		CRect rect = GetRectOfChart(nRowIndexOutside,nColIndexOutside);
		CChartItem *pChartItem = m_pChartItems[nRowIndexOutside][nColIndexOutside];
		//2005. 06. 07 =======================================================================
		//새로이 생성되는 ChartOCX(chartItem)에게 Drds를 요청 가능한 ClassPointer를 넘겨준다.
		//added by sang yun, nam
		if(m_lMltchrtDlgParentClsPointer > 0)
		{
			pChartItem->SetOutsideCommonInterface(m_lMltchrtDlgParentClsPointer); 
			pChartItem->m_CfgInfo.bShowLog = pChartItem->GetLogScale(_T("")) ? true : false;
			//pChartItem->SetRootPath(m_strRootPath);
		}
		//====================================================================================
		pChartItem->MoveWindow(rect);	

	}
	
	Invalidate(TRUE);
}


///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	Input	:	lCnt 변화량
//				nType	RESIZEBOTH = 행,열 각각 증가
//						RESIZECOL  = 열 증가
//						RESIZEROW  = 행 증가
//	목적	:	차트의 수를 증가시키거나 감소시킨다.
///////////////////////////////////////////////////////////////////////////////
BOOL CChartMng::Rearrange(long lCnt, int nType)
{
	CChartItem *pChartItem = NULL;
	m_bChartExtended = FALSE;
	long lColsForFuture = m_lCols;
	long lRowsForFuture = m_lRows;
	CString strEndDateFromInputBar;
	if(nType == RESIZEBOTH)
	{
		lColsForFuture += lCnt;
		lRowsForFuture += lCnt;
	}
	else if(nType == RESIZECOL)
	{
		lColsForFuture += lCnt;
	}
	else if(nType == RESIZEROW)
	{
		lRowsForFuture += lCnt;
	}
	if(lCnt>0)
	{
		if(lColsForFuture>CNTOFMAXCOL)	return FALSE;
		if(lRowsForFuture>CNTOFMAXROW)	return FALSE;
		for(int nRowIndex=0;nRowIndex<lRowsForFuture;nRowIndex++)
		{
			for(int nColIndex=0;nColIndex<lColsForFuture;nColIndex++)
			{
				pChartItem = NULL;
				pChartItem = m_pChartItems[nRowIndex][nColIndex];
				if(pChartItem == NULL)
				{
					pChartItem = new CChartItem;
					m_nID += 10;
					if( ((CStdDialog*)GetParent())->m_nChartType==MINI_MULTI_CHART ||
						((CStdDialog*)GetParent())->m_nChartType==MINI_SINGLE_CHART )
					{
						pChartItem->Create(this,GetRectOfChart(nRowIndex,nColIndex),m_nID,nRowIndex,nColIndex, m_strImagePath, FALSE);
						pChartItem->SetSmallChartMode(2);
					}
					else
					{
						pChartItem->Create(this,GetRectOfChart(nRowIndex,nColIndex),m_nID,nRowIndex,nColIndex, m_strImagePath, TRUE);
					}					
//					else if(((CStdDialog*)GetParent())->GetMapID()=="10010")
//					{
//						pChartItem->Create(this,GetRectOfChart(nRowIndex,nColIndex),m_nID,nRowIndex,nColIndex, "");
//						pChartItem->SetSmallChartMode(1);
//					}
					
					pChartItem->ShowInnerToolBar(FALSE);
					
					//================================================================
					// 2005. 07. 28 add by sy, nam
					// InputBar에서 Extension Bar로 
					//================================================================
					if(!m_pExtInputBarDlg)
					{
						MessageBox("m_pExtInputBarDlg ERROR!");
					}
					else
					{
						long lEndDate = m_pExtInputBarDlg->SendMessage(UMSG_GETENDDATEFROMINPUTBAR);
						//================================================================
						if(lEndDate)
						{
							strEndDateFromInputBar.Format("%d",lEndDate);
							pChartItem->SetEndDateRequested(strEndDateFromInputBar);
							pChartItem->SetEndDate(strEndDateFromInputBar); //05.09.06 added by sy, nam
						}
					}
					if(m_bTypeLink) //설정연동
					{							
						char chType = m_pInputBarDlg ? (char)m_pInputBarDlg->SendMessage(UMSG_GETTYPEFROMINPUTBAR) : TICK_DATA_CHART;
						long lNumber = m_pInputBarDlg ? m_pInputBarDlg->SendMessage(UMSG_GETTYPENUMFROMINPUTBAR) : 0;
						pChartItem->SetType(chType);
						pChartItem->SetTickMinDayNumber(lNumber);
					}
					if(m_lKey)
					{
						pChartItem->SetInit((long)this->GetSafeHwnd(), m_lKey, m_strRootPath, FALSE);
//@유잔삭제
//						if( ((CStdDialog*)GetParent())->m_nChartType==MINI_MULTI_CHART ||
//							((CStdDialog*)GetParent())->m_nChartType==MINI_SINGLE_CHART )
//						{
//							pChartItem->SetInit((long)this->GetSafeHwnd(), m_lKey, m_strRootPath, TRUE);
//						}
//						else
//						{
//							pChartItem->SetInit((long)this->GetSafeHwnd(), m_lKey, m_strRootPath, FALSE);						
//						}
//@유잔삭제
						if(m_pChartItems[0][0])
						{
							pChartItem->SetNUseInquiryWithCrossline(m_pChartItems[0][0]->GetNUseInquiryWithCrossline());							
						}
					}	
					if(!m_pExtInputBarDlg)
					{
						MessageBox("m_pExtInputBarDlg ERROR!");
						return FALSE;
					}
					

					BOOL bRevised = m_pExtInputBarDlg->SendMessage(UMSG_GETREVISED);
					BOOL bLogPrice	= m_pInputBarDlg ? m_pInputBarDlg->SendMessage(UMSG_GETLOG) : FALSE;		// 로그값    사용여부
					
					
					//BOOL bNormalizedMinuteTimeRule	= m_pExtInputBarDlg->SendMessage(UMSG_GETNORMALIZEDMINUTETIMERULE);
					//BOOL bTimeShareUseage				= m_pExtInputBarDlg->SendMessage(UMSG_GETTIMESHAREUSEAGE);
					//BOOL bUpperLowPriceMarkUseage		= m_pExtInputBarDlg->SendMessage(UMSG_GETUPPERLOWPRICEMARKUSEAGE);

					INT  nAnalysisType = m_pExtInputBarDlg->SendMessage(UMSG_GETDRAGMODETYPE);
					
					pChartItem->SetUseRevisedValue			(bRevised);
					pChartItem->SetUseLogValue				(bLogPrice);
					
					pChartItem->EnableReceivingTimeShare	(pChartItem->m_CfgInfo.bRecvTimeShare	);
					pChartItem->EnableBoundMark				(pChartItem->m_CfgInfo.bShowHLBar		);

					pChartItem->SetBNormalizedMinuteTimeRule(	pChartItem->m_CfgInfo.bImaginaryCandle	);
					pChartItem->SetBUseGapRevision(				pChartItem->m_CfgInfo.bUseGapRevision	);

					pChartItem->SetDragModeType				(nAnalysisType);

					m_pChartItems[nRowIndex][nColIndex] = pChartItem;
					SetChartSelected(CPoint(nColIndex,nRowIndex));					
				}
			}			
		}
		m_lCols = lColsForFuture;
		m_lRows = lRowsForFuture;
	}
	else if(lCnt<0)
	{
		if(lColsForFuture<1)	return FALSE;
		if(lRowsForFuture<1)	return FALSE;
		// delete Col First..
		for(int nRowIndex=(m_lRows-1);nRowIndex>=0;nRowIndex--)
		{
			for(int nColIndex=(m_lCols-1);nColIndex>=(lColsForFuture);nColIndex--)
			{
				pChartItem = m_pChartItems[nRowIndex][nColIndex];
				if(pChartItem)
				{					
					pChartItem->DestroyWindow();
					delete pChartItem;
					pChartItem = NULL;
					m_pChartItems[nRowIndex][nColIndex] = pChartItem;
					if(nColIndex<=m_lColSelected)
					{
						m_lColSelected = nColIndex - 1;
					}
				}
			}			
		}
		m_lCols = lColsForFuture;
		
		// delete Row Second..
		for(int nColIndex=(m_lCols-1);nColIndex>=0;nColIndex--)
		{
			for(int nRowIndex=(m_lRows-1);nRowIndex>=(lRowsForFuture);nRowIndex--)
			{
				pChartItem = m_pChartItems[nRowIndex][nColIndex];
				if(pChartItem)
				{
					pChartItem->DestroyWindow();
					delete pChartItem;
					pChartItem = NULL;
					m_pChartItems[nRowIndex][nColIndex] = pChartItem;
					if(nRowIndex<=m_lRowSelected)
					{
						m_lRowSelected = nRowIndex - 1;
					}
				}
			}			
		}
		m_lRows = lRowsForFuture;
	}
	if(m_pExtInputBarDlg)
	{
		m_pExtInputBarDlg->SendMessage(UMSG_SETMATRIX,m_lRows,m_lCols);
	}
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	차트의 행, 열의 수를 제어한다.
///////////////////////////////////////////////////////////////////////////////
BOOL CChartMng::SetMatrix(long lRows,long lCols)
{
	long lColSelectedOld = m_lColSelected;
	long lRowSelectedOld = m_lRowSelected;

	long lColsDif = lCols - m_lCols;
	long lRowsDif = lRows - m_lRows;
	if(lColsDif==lRowsDif)
	{
		Rearrange(lColsDif,RESIZEBOTH);
	}
	else
	{
		if(lRowsDif>lColsDif)
		{
			Rearrange(lColsDif,RESIZECOL);
			Rearrange(lRowsDif,RESIZEROW);
		}
		else
		{
			Rearrange(lRowsDif,RESIZEROW);
			Rearrange(lColsDif,RESIZECOL);			
		}
	}
	MoveCharts();
	if(lColSelectedOld>=m_lCols||lRowSelectedOld>=m_lRows)
	{
		SetChartSelected(CPoint(0,0),TRUE);
	}

/*	// 호가 창 Show ---------------------------------------------------
	// 2005. 04. 28 by Nam
	//-----------------------------------------------------------------
	if(lRows == 1 && lCols == 1)
	{
		HWND hParent = GetParent()->GetSafeHwnd();		
		::SendMessage(hParent, UMSG_FORCE_SHOW_HOGAWND, 1, 0 );		
	}
	//-----------------------------------------------------------------
*/
	return TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	선택된 차트의 Pointer을 보낸다.
///////////////////////////////////////////////////////////////////////////////
CChartItem* CChartMng::GetpChartItemSelected()
{
	// (2008/9/28 - Seung-Won, Bae) for Processing Error of Loop
	if( m_pChartItemForRequest) return m_pChartItemForRequest;

	// (2008/9/28 - Seung-Won, Bae) for Processing Error of Loop
	if( m_bIsInLoop) return m_pChartItemInLoop;

	if(m_lRowSelected < 0) return NULL;
	if(m_lColSelected < 0) return NULL;

	if(m_pChartItems[m_lRowSelected][m_lColSelected]->GetSafeHwnd())
		return m_pChartItems[m_lRowSelected][m_lColSelected];
	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	차트의 수를 증가시킨다.
///////////////////////////////////////////////////////////////////////////////
BOOL CChartMng::InsertChart()
{
	BOOL betValue = FALSE;
	if(m_lRows*m_lCols<20)
	{
		if(m_lRows<m_lCols)
		{
			betValue = Rearrange(1,RESIZEROW);
		}
		else
		{
			betValue = Rearrange(1,RESIZECOL);
		}
		MoveCharts();
	}
	return betValue;
}


void CChartMng::OnLButtonDown(UINT nFlags, CPoint point) 
{
	((CInputBarDlg*)m_pInputBarDlg)->m_pCodeInput->SetFocus();
	// TODO: Add your message handler code here and/or call default
//	CRect rect;
//	EACH_CHARTJOB_START()
//		rect = GetRectOfChart(nRowIndex,nColIndex,TRUE);
//		if(rect.PtInRect(point))
//		{
//			//======================================================================================
//			// 2005. 07. 18 by sy, nam
//			// DataListView에 데이터를 뿌리기 위해 호출하면 
//			// CStdDialog의 SetOutsideCommonInterface로 해당 ocx의 정보를 가져갈수 있는 
//			// 포인터가 넘어오면 그 안에서 DataListView내의 내용을 가져간다.
//			//======================================================================================
//			pChartItem->SetOutsideCommonInterface(m_lMltchrtDlgParentClsPointer);
//
//
//			SetChartSelected(CPoint(nColIndex,nRowIndex));
//			//======================================================================================
//			//2005. 05. 24 by sy, nam
//			// 지존에서는 주식전용이었으나 한화에서 선물/업종/주식 모두 다 쓰므로 수정한다.
//			//======================================================================================
//			if( (pChartItem->GetDataTRCode()==QUERY_STRFOFOREIGN	) ||
//				(pChartItem->GetDataTRCode()==QUERY_STRFUTOPT	) ||
//				(pChartItem->GetDataTRCode()==QUERY_STRKQFUTOPT ) ||
//				(pChartItem->GetDataTRCode()==QUERY_STRSTOCKOPT ) ||
//				(pChartItem->GetDataTRCode()==QUERY_STRUPJONG	) ||
//				(pChartItem->GetDataTRCode()==QUERY_STRFOREIGN	) || 
//				(pChartItem->GetDataTRCode()==QUERY_STR_FSTAR	) 
//
//			)
//			{
//				GetParent()->SendMessage(UMSG_SENDSHAREDCODE,(WPARAM)(LPCTSTR)pChartItem->GetDataCode());				
//			}
//			//======================================================================================
//			//if(pChartItem->GetDataTRCode()==QUERY_STRFOFOREIGN)
//			//{
//			//	GetParent()->SendMessage(UMSG_SENDSHAREDCODE,(WPARAM)(LPCTSTR)pChartItem->GetDataCode());				
//			//}
//			break;
//		}
//	EACH_CHARTJOB_END()	
//	m_bClicked = TRUE;
//	CWnd::OnLButtonDown(nFlags, point);
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	EACH_CHARTJOB_START()
		rect = GetRectOfChart(nRowIndex,nColIndex,TRUE);
		if(rect.PtInRect(point))
		{
			pChartItem->SetOutsideCommonInterface(m_lMltchrtDlgParentClsPointer);
			//2007.01.18 by LYH ToolBar 정보를 얻어온다. 이전 선택 세팅
			int m_lColSelectedOld = m_lColSelected;
			int m_lRowSelectedOld = m_lRowSelected;
			//}}

			SetChartSelected(CPoint(nColIndex,nRowIndex));

			//2007.01.18 by LYH ToolBar 정보를 얻어온다.
			if(m_lColSelectedOld!=nColIndex || m_lRowSelectedOld != nRowIndex)
			{
				GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 0, 50000);
				GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50031, pChartItem->m_bDrawAutoTrendLine );
				GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50032, pChartItem->m_bDrawAutoPattern );
				GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50033, pChartItem->m_bDrawCandlePattern );
				GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 50026, pChartItem->m_bDrawNews );

				if(m_pExtInputBarDlg)
					m_pExtInputBarDlg->SendMessage(UMSG_SYNC_TOOLBAR, CToolOptionInfo::TOOLOPTION::T_CROSSHAIRS_TOOL, 0);
				pChartItem->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::T_ALL_TOOL_TRUE_QUERY, 0);
			}
			//}}
//			if(pChartItem->GetDataTRCode()==QUERY_STRFOFOREIGN)
//			{
//공유 두번 날라가서 막음 by LYH 2007.01.22
//				GetParent()->SendMessage(UMSG_SENDSHAREDCODE,(WPARAM)(LPCTSTR)pChartItem->GetDataCode());				
//			}
//			((CMultiChartDlg*)GetParent())->SendMessage(RMSG_SETFOCUSTOCODE);
			break;
		}
	EACH_CHARTJOB_END()	
	m_bClicked = TRUE;
	CWnd::OnLButtonDown(nFlags, point);
}

void CChartMng::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bClicked)
	{
		m_bClicked = FALSE;
		ReleaseCapture();
		if(m_bDragNDrop)	// 드래그 중이었으면
		{
			//CChartItem* pLastChartItem = NULL;

			CRect rect;
			EACH_CHARTJOB_START()
				rect = GetRectOfChart(nRowIndex,nColIndex,2);
				if(rect.PtInRect(point))
				{
					ReplaceChart(m_lRowDragFrom,m_lColDragFrom, nRowIndex, nColIndex);
					SetChartSelected(CPoint(nColIndex,nRowIndex),TRUE);
					//2005. 07. 18  드래그 종료 마지막 선택된 ChartItem을 저장한다.==========
					//테스트 후 적용 필요..  
					//pLastChartItem = pChartItem; 
					//=======================================================================
					break;
				}
			EACH_CHARTJOB_END()
		}
		m_bDragNDrop = FALSE;
	}		
	CWnd::OnLButtonUp(nFlags, point);
}

void CChartMng::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bClicked)
	{
		// 드래그임을 표시
		if(!m_bDragNDrop)
		{
			m_lColDragFrom = m_lColSelected;
			m_lRowDragFrom = m_lRowSelected;
			SetCapture();
		}
		m_bDragNDrop = TRUE;
		CPoint ptDrag(point);
		ClientToScreen(&ptDrag);
		GetParent()->GetParent()->GetParent()->SendMessage(RMSG_DRAGSTART, (WPARAM)&ptDrag, (LPARAM)m_hWnd);
	}	
	else
	{
//		m_tooltip.ShowToolTip( point );
//		int  nCmdShow = SW_HIDE;
//		CRect rect;
//		EACH_CHARTJOB_START()			
//			if(pChartItem->PtInRectJongmokTitle(point))
//			{
//				nCmdShow = SW_SHOW;
//				break;
//			}
//		EACH_CHARTJOB_END()
//		m_tooltipDlg.ShowWindow(nCmdShow);
	}

	
/*
	if(!m_bClicked)
	{
		BOOL bWithinRows = FALSE;
		BOOL bWithinCols = FALSE;
		CChartItem *pChartItem = NULL;
		CRect rectClient;
		CRect rectOrigin;
		GetClientRect(rectClient);	
		rectOrigin = GetRectOfChart(0,0,2);

		CRect rectOld;
		CRect rectCurrent;
		CRect rectWithinRows;
		CRect rectWithinCols;		

		rectOld = rectOrigin;
		rectWithinRows.left = rectClient.left;
		rectWithinRows.right = rectClient.right;
		for(int nRowIndex=1;nRowIndex<m_lRows;nRowIndex++)
		{
			rectCurrent = GetRectOfChart(nRowIndex,0,2);
			rectWithinRows.top = rectOld.bottom;
			rectWithinRows.bottom = rectCurrent.top;
			if(rectWithinRows.PtInRect(point))
			{
				bWithinRows = TRUE;
			}
			rectOld = rectCurrent;
		}
		
		rectOld = rectOrigin;
		rectWithinCols.top = rectClient.top;
		rectWithinCols.bottom = rectClient.bottom;
		for(int nColIndex=1;nColIndex<m_lCols;nColIndex++)
		{
			rectCurrent = GetRectOfChart(0,nColIndex,2);
			rectWithinCols.left = rectOld.right;
			rectWithinCols.right = rectCurrent.left;
			if(rectWithinCols.PtInRect(point))
			{
				bWithinCols = TRUE;
			}
			rectOld = rectCurrent;
		}
		if(bWithinRows&&bWithinCols)
		{
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));	
			m_nWhereIsCursor = 1;
		}
		else if(bWithinRows)
		{
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
			m_nWhereIsCursor = 1;
		}
		else if(bWithinCols)
		{
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
			m_nWhereIsCursor = 1;
		}
		else
		{
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			m_nWhereIsCursor = 0;
		}
	}
	else
	{
		CString strPos;
		strPos.Format("Dragging %d %d\n",point.x, point.y);
		TRACE(strPos);
	}
	*/

	CWnd::OnMouseMove(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	차트를 초기값으로 바꾼다.
//////////////////////////////////////////////////////////////////////////////
long CChartMng::ResetChart()
{
	if(m_bJongmokLink)
	{
		EACH_CHARTJOB_START()
			pChartItem->ResetChart();
		EACH_CHARTJOB_END()
	}
	else
	{
		CChartItem* pChartItem = NULL;
		pChartItem = GetpChartItemSelected();
		if(pChartItem == NULL) 
			return 1L;

		GetpChartItemSelected()->ResetChart();
	}
	return 1L;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	차트를 초기화한다.
//////////////////////////////////////////////////////////////////////////////
void CChartMng::ResetOCX()
{
	CChartItem* pChartItem = NULL;
	pChartItem = GetpChartItemSelected();
	if(pChartItem == NULL) 
		return;

/*	if(m_bChartExtended)
	{
		// 호가 창 Show ---------------------------------------------------
		// 2005. 04. 28 by Nam
		//-----------------------------------------------------------------
		HWND hParent = GetParent()->GetSafeHwnd();		
		::SendMessage(hParent, UMSG_FORCE_SHOW_HOGAWND, m_bChartExtended, (LPARAM)1);		
		//-----------------------------------------------------------------
	}
*/
	GetpChartItemSelected()->ResetOCX();
//	SendJongmokCodesToLeftBar();
}

void CChartMng::InitInLoop()
{	
	m_bDontRequestDataInLoop = NULL;
	m_bUseRevisedInLoop = NULL;

// (2008/11/2 - Seung-Won, Bae) Do not support sync for all ChartItem.
//	m_bNormalizedMinuteTimeRuleInLoop = NULL;

	//m_bUseRateValueInLoop = NULL;
	m_nChartMngs_ChartDataNumericValue = WON_TYPE;
	m_pChartItemInLoop = NULL;
	m_strChartSelectedInLoop.Empty();
	m_strCodeInLoop.Empty();
	m_strEndDateInLoop.Empty();
	m_chTypeInLoop = NULL;
	m_lNumberInLoop = NULL;
	m_lZoomValueInLoop = NULL;
	m_dEndDateInLoop = NULL;
	m_lTypeInLoop = LOOPTYPE_NULL/*0*/;
}

BOOL CChartMng::PostInLoop()
{
	SetChartSelected(CPoint(m_lColSelectedInLoop,m_lRowSelectedInLoop));
	if(m_lTypeInLoop==LOOPTYPE_UPDATE_BY_EXTEND/*5*/)
	{
		m_bUpdatedInChartExtended = FALSE;
	}

	return TRUE;
}

BOOL CChartMng::TailInLoop()
{
	if(!m_bIsInLoop) return FALSE;
	if(!HeadInLoop())
	{
		PostInLoop();
	}
	return TRUE;
}

BOOL CChartMng::HeadInLoop()
{
	if(!m_bIsInLoop)
	{
		m_lColSelectedInLoop = m_lColSelected;
		m_lRowSelectedInLoop = m_lRowSelected;
		m_nRowIndexInLoop = 0;
		m_nColIndexInLoop = 0;
		m_bIsInLoop = TRUE;
	}
	else
	{
		if(m_pChartItemInLoop) 
		{
			switch(m_lTypeInLoop)
			{
			case LOOPTYPE_LOAD/*1*/: // Load >>> m_strChartSelectedInLoop : 파일명.				
				m_pChartItemInLoop->LoadChartSetting(m_strChartSelectedInLoop);
				if(m_pInputBarDlg)
				{
					if(m_pChartItemInLoop->GetBWholeView())
						m_pInputBarDlg->SendMessage(UMSG_SETZOOMVALUE,m_pChartItemInLoop->GetAllCnt());
					else
					{
						//m_pInputBarDlg->SendMessage(UMSG_SETZOOMVALUE,m_pChartItemInLoop->GetNOnePageDataCount());
						long lZoomValue = m_pInputBarDlg ? m_pInputBarDlg->SendMessage(UMSG_GETZOOMVALUE,m_pChartItemInLoop->GetType()) : 0;	
						if(lZoomValue > m_pChartItemInLoop->GetAllCnt())
							m_pChartItemInLoop->SetNew_OnePageDataCount(m_pChartItemInLoop->GetAllCnt());
						else
							m_pChartItemInLoop->SetNew_OnePageDataCount(lZoomValue);
						//2007.02.06 by LYH 불러오기 후 다음종목 veiwcount 안 맞는 버그 수정
						//((CInputBarDlg*)m_pInputBarDlg)->SetViewCount(m_pChartItemInLoop->GetNOnePageDataCount());
						//}}
					}
				}
				break;
			case LOOPTYPE_FIXDATE/*2*/:// fixdate
				m_pChartItemInLoop->ZoomToWithRightFix((short)m_lZoomValueInLoop,TRUE);
				break;
			case LOOPTYPE_UPDATE/*4*/:
				if(m_pChartItemInLoop!=m_pChartItemSelectedInLoop)
				{
// --> [Edit]  강지원 2008/10/31	( 주기별 지표 동기화가 안된다는 애기가 있어서 일단 풀어보고 문제가 생기면 RollBack)
//					if(m_pChartItemInLoop->GetAllCnt()&&m_pChartItemInLoop->GetCntOfItemData()&&m_pChartItemInLoop->GetType()==m_chTypeInLoop) 
					if(m_pChartItemInLoop->GetAllCnt()&&
						m_pChartItemInLoop->GetCntOfItemData())
						 
// <-- [Edit]  강지원 2008/10/31
					{
						m_pChartItemInLoop->SaveandOpenFromFile("temp.tmp","",TRUE,FALSE);
						m_pChartItemInLoop->InvalidateType();
					}				
				}
				break;
			default:
				break;
			}
		}
		// (2008/9/14 - Seung-Won, Bae) for Pair Tick
		int nRequestPairTickState = ( ( CStdDialog *)GetParent())->GetRequestPairTickState();
		if( nRequestPairTickState != E_RPTS_LOOP_START)
		{
			if( nRequestPairTickState == E_RPTS_LOOP_PAIR) ( ( CStdDialog *)GetParent())->EndRequestPairTick();

			m_nColIndexInLoop++;
			if(m_nColIndexInLoop>=m_lCols)
			{
				m_nColIndexInLoop = 0;
				m_nRowIndexInLoop++;
				if(m_nRowIndexInLoop>=m_lRows)			
				{
					m_bIsInLoop = FALSE;

					//========================================================================
					if(m_lTypeInLoop == LOOPTYPE_LOAD && !m_bLastEnvUpdated)
					{						
						SetFocusToJMEdit_At_LoadEnd();
					}


					return FALSE;
				}
			}
		}
	}
	m_pChartItemInLoop = m_pChartItems[m_nRowIndexInLoop][m_nColIndexInLoop];

	// (2008/9/23 - Seung-Won, Bae) Check Empty for Time Type Sync.
	BOOL bEmpty = !m_pChartItemInLoop;
	if( !bEmpty && !m_lTypeInLoop && m_strCodeInLoop.IsEmpty())
	{
		bEmpty = !m_pChartItemInLoop->GetCntOfItemData();
		if(m_chTypeInLoop!=NULL)	m_pChartItemInLoop->SetType(m_chTypeInLoop);
		if(m_lNumberInLoop!=NULL)	m_pChartItemInLoop->SetTickMinDayNumber(m_lNumberInLoop);
	}
	if( bEmpty) 
	{
		if(!HeadInLoop()) 
		{
			PostInLoop();
		}
		return TRUE;
	}
	SetChartSelected(CPoint(m_nColIndexInLoop,m_nRowIndexInLoop), FALSE, TRUE);
	
	// (2008/9/25 - Seung-Won, Bae) Select Main Tick for Sync Target.
	( ( CStdDialog *)GetParent())->SelectMainRQ();

	//05. 09. 07 ==============================================================
	// 파일에서 읽어오는 정보가 없을 경우 공유로 들어오는 
	// 코드를 차트에 적용시키기 위해 LOOPTYPE_LOAD시에만  결과를 
	// 받아서 FALSE일 경우 Return시킨다.
	// sy, nam 
	BOOL bResultOfLoopTypeLoad = FALSE; 
	//=========================================================================
	switch(m_lTypeInLoop)
	{
	case LOOPTYPE_LOAD/*1*/:
		{
			m_nStatic_RecvCntShareCode++;
			bResultOfLoopTypeLoad = m_pChartItemInLoop->LoadChartInfo(m_strChartSelectedInLoop);
			if(bResultOfLoopTypeLoad == FALSE) 
			{
				// (2008/10/5 - Seung-Won, Bae) Do Next Loop.
				if(!HeadInLoop()) PostInLoop();
				return TRUE;
			}
		}
		break;
	case LOOPTYPE_FIXDATE/*2*/:
		{
			//===========================================================
			//2005. 05. 23
			//if(m_pChartItemInLoop->GetType()	!=DAY_DATA_CHART
			//	&&m_pChartItemInLoop->GetType()	!=WEEK_DATA_CHART
			//	&&m_pChartItemInLoop->GetType()	!=MONTH_DATA_CHART)
			//===========================================================
			if( 
				  m_pChartItemInLoop->GetType()	!= DAY_DATA_CHART
				&&m_pChartItemInLoop->GetType()	!= WEEK_DATA_CHART
				&&m_pChartItemInLoop->GetType()	!= MONTH_DATA_CHART
				&&m_pChartItemInLoop->GetType()	!= YEAR_DATA_CHART
//				&&m_pChartItemInLoop->GetType()	!= SEC10_DATA_CHART //05.09.28
//				&&m_pChartItemInLoop->GetType()	!= SEC30_DATA_CHART //05.09.28
				)
			{
				if(!HeadInLoop()) PostInLoop();
				return TRUE;
			}
			// (2008/8/10 - Seung-Won, Bae) Support End Date if only it is not zero. (not minute and tick)
			if( m_dEndDateInLoop)
			{
				/*
				if( m_pChartItemInLoop->ScrollToDataOfEnd(m_dEndDateInLoop))
				{

					// (2008/8/5 - Seung-Won, Bae) TRACE for Debugging
					TRACE( "\r\nCChartMng::HeadInLoop() - call TR Request, LOOPTYPE_FIXDATE, m_pChartItemInLoop->ScrollToDataOfEnd()\r\n");

					if(!HeadInLoop()) PostInLoop();
					return TRUE;
				}
				*/
				m_pChartItemInLoop->SetEndDateRequested((LPCTSTR)m_strEndDateInLoop);
				m_pChartItemInLoop->SetEndDate((LPCTSTR)m_strEndDateInLoop); //05.09.06 added by sy, nam
			}
		}
		break;
	case LOOPTYPE_REVISED/*3*/:
		{
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem == NULL) 
				break;

			GetpChartItemSelected()->SetUseRevisedValue(m_bUseRevisedInLoop);
			if(m_bDontRequestDataInLoop) 
			{
				if(!HeadInLoop()) PostInLoop();
				return TRUE;
			}
		}
		break;
	case LOOPTYPE_UPDATE/*4*/:
		{
			long lCnt = m_pChartItemInLoop->GetCntOfItemData();
			if(!m_pChartItemInLoop->GetAllCnt()||!m_pChartItemInLoop->GetCntOfItemData()) 
			{
				if(!HeadInLoop()) PostInLoop();
				return TRUE;
			}
			if(m_pChartItemInLoop==m_pChartItemSelectedInLoop
				||m_pChartItemInLoop->GetType()!=m_chTypeInLoop)
			{
				if(!HeadInLoop()) PostInLoop();
				return TRUE;
			}
			m_pChartItemInLoop->ClearDataExceptPrimaryData();
			
//			if(m_pChartItemInLoop->GetUseRateValue()!=m_bUseRateValueInLoop)
//			{
//				m_pChartItemInLoop->SetUseRateValue(m_bUseRateValueInLoop);
//			}
			
			if(m_pChartItemInLoop->GetChart_NumericValueType()!=m_nChartMngs_ChartDataNumericValue)	
			{
				m_pChartItemInLoop->SetChartNumericValue(m_nChartMngs_ChartDataNumericValue);
			}
			
// (2008/11/2 - Seung-Won, Bae) Do not support sync for all ChartItem.
//			if(m_pChartItemInLoop->GetBNormalizedMinuteTimeRule()!=m_bNormalizedMinuteTimeRuleInLoop)
//			{
//				m_pChartItemInLoop->SetBNormalizedMinuteTimeRule(m_bNormalizedMinuteTimeRuleInLoop );
//			}

			CString  strTRCode;
			for(long lPos = 0;lPos<(m_lCntInLoop-1);lPos++)
			{
				//lTrCode = atoi(m_strArrayTRCodeInLoop.GetAt(lPos));
				strTRCode = m_strArrayTRCodeInLoop.GetAt(lPos);
				m_pChartItemInLoop->SetCodeForQuery(m_strArrayCodeInLoop.GetAt(lPos),strTRCode,FALSE);
			}		
		}
		break;
	case LOOPTYPE_UPDATE_BY_EXTEND/*5*/:
		{
//			if(m_pChartItemInLoop->GetDataCode()=="") 
//			{
//				if(!HeadInLoop()) PostInLoop();
//				return TRUE;
//			}
			m_bNeedToUpdateInLoop = FALSE;
			if(m_pChartItemInLoop->GetUseRevisedValue() != m_bUseRevisedInLoop)
			{
				m_pChartItemInLoop->SetUseRevisedValue(m_bUseRevisedInLoop);
				m_bNeedToUpdateInLoop = TRUE;
			}

// (2008/11/2 - Seung-Won, Bae) Do not support sync for all ChartItem.
//			if(m_pChartItemInLoop->GetBNormalizedMinuteTimeRule() != m_bNormalizedMinuteTimeRuleInLoop)
//			{
//				m_pChartItemInLoop->SetBNormalizedMinuteTimeRule(m_bNormalizedMinuteTimeRuleInLoop);
//				m_bNeedToUpdateInLoop = TRUE;
//			}

			if(m_pChartItemInLoop->GetDataCode()!=m_strCodeInLoop&&m_bJongmokLink)
			{
				m_pChartItemInLoop->SetCodeForQuery(m_strCodeInLoop,QUERY_STRFOFOREIGN,TRUE);
				m_pChartItemInLoop->SetEndDateRequested(m_strEndDateInLoop);
				m_pChartItemInLoop->SetEndDate(m_strEndDateInLoop); //05.09.06 added by sy, nam
				m_bNeedToUpdateInLoop = TRUE;
			}
			if( (m_pChartItemInLoop->GetType()!=m_chTypeInLoop
				||m_lTickMinNumberInLoop!=atol(m_pChartItemInLoop->GetTickMinDayNumber())	)	
				&&m_bTypeLink)
			{
				m_pChartItemInLoop->SetType(m_chTypeInLoop);
				m_pChartItemInLoop->SetTickMinDayNumber(m_lTickMinNumberInLoop);
				m_bNeedToUpdateInLoop = TRUE;
			}
			if(!m_bNeedToUpdateInLoop)
			{
				if(!HeadInLoop()) PostInLoop();
				return TRUE;				
			}
		}
		break;
	case LOOPTYPE_RATE_VALUE/*6*/:// RateValue
		{
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem == NULL) 
				break;
			
			//2005. 06. 22
			//GetpChartItemSelected()->SetUseRateValue(m_bUseRateValueInLoop);
			GetpChartItemSelected()->SetChartNumericValue(m_nChartMngs_ChartDataNumericValue);
			if(m_bDontRequestDataInLoop) 
			{
				if(!HeadInLoop()) PostInLoop();
				return TRUE;
			}
		}
		break;

// (2008/11/2 - Seung-Won, Bae) Do not support sync for all ChartItem.
//	case LOOPTYPE_FAKE_CANDLE/*7*/:// ShowFakedCandle
//		{
//			CChartItem* pChartItem = NULL;
//			pChartItem = GetpChartItemSelected();
//			if(pChartItem == NULL) 
//				break;
//
//			GetpChartItemSelected()->SetBNormalizedMinuteTimeRule(m_bNormalizedMinuteTimeRuleInLoop);
//			if(m_bDontRequestDataInLoop) 
//			{
//				if(!HeadInLoop()) PostInLoop();
//				return TRUE;
//			}
//		}
//		break;

	default:
		{
			if(!m_strCodeInLoop.IsEmpty())
			{
				//if(   (m_strCodeInLoop.GetLength()==UPJONG_LENGTH)  
				switch(m_nMarketType)
				{
					case CMasterTypeInfo::STOCK_CHART:
						{
// --> [Edit]  이도영 2008/09/13
// 
//							if(m_strCodeInLoop.GetLength()== CODE_STOCK_LENGTH)				
// <-- [Edit]  이도영 2008/09/13
							{
								CChartItem* pChartItem = NULL;
								pChartItem = GetpChartItemSelected();
								if(pChartItem == NULL) 
									break;

								GetpChartItemSelected()->SetCodeForQuery( m_strCodeInLoop, QUERY_STRFOFOREIGN,TRUE);
							}
						}
						break;
					case CMasterTypeInfo::UPJONG_CHART:
						{
							if(m_strCodeInLoop.GetLength()==UPJONG_LENGTH)  				
							{
								CChartItem* pChartItem = NULL;
								pChartItem = GetpChartItemSelected();
								if(pChartItem == NULL) 
									break;

								//2005.05.06 by sy,nam
								GetpChartItemSelected()->SetCodeForQuery(m_strCodeInLoop,QUERY_STRUPJONG,TRUE);
							}			
						}
						break;
					case CMasterTypeInfo::FUTOPT_CHART:
						{
							if( (m_strCodeInLoop.GetLength()	==	FUTOPT_LENGTH	) ||
								(m_strCodeInLoop.GetLength()	==	KQOPT_LENGTH12	) ||
								(m_strCodeInLoop.GetLength()	==	KQFUT_LENGTH	) ||								
								(m_strCodeInLoop.GetLength()	==	KQOPT_LENGTH	))
								{
									CChartItem* pChartItem = NULL;
									pChartItem = GetpChartItemSelected();
									if(pChartItem == NULL) 
										break;

									//2005.05.06 by sy,nam
									GetpChartItemSelected()->SetCodeForQuery(m_strCodeInLoop,QUERY_STRFUTOPT,TRUE);
								}
						}
						break;
					case CMasterTypeInfo::STOCKFUTOPT_CHART:
						{
							if( (m_strCodeInLoop.GetLength()	==	FUTOPT_LENGTH	) ||
								(m_strCodeInLoop.GetLength()	==	KQOPT_LENGTH12	) ||
								(m_strCodeInLoop.GetLength()	==	KQFUT_LENGTH	) ||								
								(m_strCodeInLoop.GetLength()	==	KQOPT_LENGTH	))
								{
									CChartItem* pChartItem = NULL;
									pChartItem = GetpChartItemSelected();
									if(pChartItem == NULL) 
										break;

									//2005.05.06 by sy,nam
									GetpChartItemSelected()->SetCodeForQuery(m_strCodeInLoop,QUERY_STRSTOCKFUTOPT,TRUE);
								}
						}
						break;
					case CMasterTypeInfo::FOREIGN_CHART:
						{
							CChartItem* pChartItem = NULL;
							pChartItem = GetpChartItemSelected();
							if(pChartItem == NULL) 
								break;

							//2005.05.06 by sy,nam
							GetpChartItemSelected()->SetCodeForQuery(m_strCodeInLoop,QUERY_STRFOREIGN,TRUE);
						}
						break;
				}

			}

			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem == NULL) 
				break;

			if(!m_strEndDateInLoop.IsEmpty())	
			{
				//pChartItem->SetEndDateRequested(m_strEndDateInLoop);
				pChartItem->SetEndDate(m_strEndDateInLoop);
			}
			if(m_chTypeInLoop!=NULL)			pChartItem->SetType(m_chTypeInLoop);
			if(m_lNumberInLoop!=NULL)			pChartItem->SetTickMinDayNumber(m_lNumberInLoop);


			// (2008/9/14 - Seung-Won, Bae) for Pair Tick
			if (GetpChartItemSelected()->FindChartItemDataForQuery() == NULL) return FALSE;
			CString strItemCode = GetpChartItemSelected()->FindChartItemDataForQuery()->GetCodeForRqstFromChartItemData();
			if( ( ( CStdDialog *)GetParent())->GetRequestPairTickState() == E_RPTS_LOOP_START)
			{
				( ( CStdDialog *)GetParent())->SetRequestPairTickState( E_RPTS_LOOP_PAIR);
				GetpChartItemSelected()->SetCodeForQuery( "|S_" + CStdDialog::GetRealItemCode( strItemCode), QUERY_STRFOFOREIGN, TRUE);
			}
			else
			{
				if( m_pChartItemInLoop->GetTypeRequested() == TICK_DATA_CHART && atoi( m_pChartItemInLoop->GetTickMinDayNumberRequested()) == 1
					&& m_pChartItemInLoop->m_nCheHoga == 1)	// (2008/11/4 - Seung-Won, Bae) Do not support Buy/Sell for Che Chart.
						( ( CStdDialog *)GetParent())->StartRequestPairTick( strItemCode, E_RPTS_LOOP_START);
				else
				{
					( ( CStdDialog *)GetParent())->ClearPairTick( "DEFAULT_PAIR");
				}
			}
		}
		break;
	}

// --> [Edit]  강지원 2008/12/12	( 데이터 추가조회 플래그 설정 )
	GetpChartItemSelected()->SetNext(FALSE);
// <-- [Edit]  강지원 2008/12/12
	if(!RequestChartData())
	{
		if(!HeadInLoop()) PostInLoop();
	}
	return TRUE;
}


//BOOL CChartMng::InputCodeFromInputBar(LPCTSTR lpCode,long lEndDate,BOOL bForcedMode0 /*=FALSE*/)
BOOL CChartMng::InputCodeFromInputBar(CCodeDataFromInputBar* pCodeData,BOOL bForcedMode0 /*=FALSE*/)
{
	if (((CStdDialog*)GetParent())->m_nChartType == SINGLE_CHART)
		m_bJongmokLink = TRUE;

	m_bNeedForceCode = FALSE; // 공유가 들어왔기 때문에 InputBar쪽에서 강제로 코드를 날릴 필요가 없다고 설정한다.
	 
	if (!m_bIsSystemTradingMode)
	{
		//nOpenedOwner  = 0 : 메뉴/툴바에서 열림
		//nOpenedOwner  = 1 : 마우스 오른쪽 버튼에서 열림 (종목코드 포함)
		//nOpenedOwner  = 2 : 스크립트,dll에서 화면 요청 (OpenMapData 포함)
//		UINT nOpenedOwner = ::SendMessage(((CStdDialog*)GetParent())->m_hViewInMain, RMSG_GETOPENEDOWNER, 0, 0);

		if (!m_bLastEnvUpdated)
		{
			//=================================================
			// 처음에 뜰때 단 한번만 호출된다.
			//=================================================
			//분할 저장 정보를 받는다.					
			m_strStatic_ShareCode = pCodeData->m_strCode; //Static으로 단 한번 적용한다.

			BOOL bResult = FALSE;

// --> [Edit]  강지원 2008/11/02
			//	처음 불리는 부분인거 같은데... 여기서 체결로 로드
			CString strName = ((CStdDialog*)GetParent())->m_strConfig;
			CString strScreenNo = ((CStdDialog*)GetParent())->m_strScreenNo;
			strName.Replace(strScreenNo, "Che_" + strScreenNo);
			strName.Replace(".dat", ".mct");
			bResult = LoadChart(strName);
// <-- [Edit]  강지원 2008/11/02

			if(!bResult) //로드할 내용이 없는 경우 로드가 끝났다는 플래그를 반드시 해준다.
			{
				m_bLastEnvUpdated = TRUE;
				SetBNoExistLoadCnts(TRUE); // 로드 할 게 전혀 없다.
			}
			m_bLastEnvUpdated = TRUE;

			m_strStatic_ShareCode = _T("");

			if(bResult)		//로드 성공하면 바로 
				return TRUE;//Return 해 버린다.
		}
	}
	
	//{{ 2007.07.06 by LYH 특수차트일 경우 종목변경 모드로 바꿈
	if(GetpChartItemSelected())
	{
		if(GetpChartItemSelected()->GetOnRunningOneChart())
			((CInputBarDlg*)m_pInputBarDlg)->SetMode(CHANGE_ITEM);
	}
	//}}

	CString strEndDate;
	CString strCode;
	char chType = NULL;
	strEndDate.Format("%d",pCodeData->m_lEndTime);
	strCode = pCodeData->m_strCode;
	if(m_bJongmokLink&&!m_bChartExtended)
	{
		if( !GetBInitFromExtToolBar() ) //초기화 상태가 아닐때만 진행시킨다 . 05.11.24
		{
			if(m_nMode == ADD_NEW_CHART &&!bForcedMode0)
			{
				if(!SelectEmptyPos())	
				{
					if(!InsertChart())
					{
						m_strMultiCodes.Empty();
						return FALSE;
					}
					SelectEmptyPos();
				}
			}	
		}

		SetBInitFromExtToolBar(FALSE); //초기화이던 아니던 기본으로 False로 해준다.05.11.24

		InitInLoop();
		m_strEndDateInLoop = strEndDate;
		m_strCodeInLoop = pCodeData->m_strCode;
		m_lTypeInLoop = LOOPTYPE_NULL/*0*/;
		HeadInLoop();


		//2005. 05. 19 add by sy, nam ===================================
		//차트가 중첩모드가 아닌 경우 ChartItem의 중첩관련 부분을 초기화 
		if(!GetpChartItemSelected()) return FALSE;
		if(m_nMode != OVERLAPP_ITEM)
		{
			GetpChartItemSelected()->Init_OverLapped();
		}
		else
			GetpChartItemSelected()->Add_OverLappedCnt();
		//===============================================================
	}
	else
	{	
		if( !GetBInitFromExtToolBar() ) //초기화 상태가 아닐때만 진행시킨다 . 05.11.24
		{
			if(m_nMode == ADD_NEW_CHART)
			{
				if(!SelectEmptyPos())	
				{
					if(!InsertChart())
					{
						m_strMultiCodes.Empty();
						return FALSE;
					}
					SelectEmptyPos();
				}
			}
		}
		
		SetBInitFromExtToolBar(FALSE); //초기화이던 아니던 기본으로 False로 해준다.05.11.24
	
		CChartItem* pSelectedChartItem = NULL;
		pSelectedChartItem = GetpChartItemSelected();
		//2005. 05. 19 add by sy, nam ===================================
		//차트가 중첩모드가 아닌 경우 ChartItem의 중첩관련 부분을 초기화 
		if(pSelectedChartItem == NULL) return FALSE;
		if(m_nMode != OVERLAPP_ITEM)
		{
			pSelectedChartItem->Init_OverLapped();
		}
		else
			pSelectedChartItem->Add_OverLappedCnt();
		//===============================================================
// --> [Edit]  강지원 2008/10/24	( 호가 버튼일경우 조회 TR을 변경시켜준다. )
		if ( pSelectedChartItem->m_nCheHoga == 1 )
			pSelectedChartItem->SetCodeForQuery(((CInputBarDlg*)m_pInputBarDlg)->GetCurrentCode(),QUERY_STRFOFOREIGN_HOGA2,TRUE);
		else
			pSelectedChartItem->SetCodeForQuery(CStdDialog::GetRealItemCode( ((CInputBarDlg*)m_pInputBarDlg)->GetCurrentCode()),QUERY_STRFOFOREIGN,TRUE);
// <-- [Edit]  강지원 2008/10/24
		
// <-- [Edit]  강지원 2008/07/22
		//=========================================================================================
		
		//pSelectedChartItem->SetEndDateRequested(strEndDate);
		pSelectedChartItem->SetEndDate(strEndDate);

		// gm0604 added For SystemTrading 2005.7.21 Start
		if(pCodeData->m_chType)
		{			
			GetpChartItemSelected()->SetType(pCodeData->m_chType);

			//edited by sy, nam 05. 08. 29 =============================================
			CString strTypeNum;
			strTypeNum.Format("%03d", pCodeData->m_lTypeNum);

			//GetpChartItemSelected()->SetTickNumber(strTypeNum);
			//GetpChartItemSelected()->SetMinNumber(strTypeNum);
			if(pCodeData->m_lTypeNum)
			{
				GetpChartItemSelected()->SetTickMinDayNumber(pCodeData->m_lTypeNum);
			}			
			//==========================================================================
		}
		if(pCodeData->m_lDataCnt)
		{
			GetpChartItemSelected()->SetCntForRequest(pCodeData->m_lDataCnt);
		}
		if(pCodeData->m_lPtrST)
		{
			GetpChartItemSelected()->SetPtrST(pCodeData->m_lPtrST,pCodeData->m_strSTName);			
		}
		// gm0604 added For SystemTrading 2005.7.21 End


		//========================================================================================
		// 2005. 09. 23
		ApplySyncEnv_OnlyOne();
		//========================================================================================

		//========================================================================================
		// 2005. 09. 23
		m_bChangeTypeMode = FALSE;
		
		// 종목변경...
		Synchro_jipyo();

		//========================================================================================

#ifdef _WRITELOG
		// Log - ojtaso (20080107)
		((CStdDialog*)GetParent())->WriteToStringLog(_T("차트개별 관리하는 Data 수정"), pCodeData->m_strCode, pCodeData->m_strCode.GetLength());
#endif

		// (2008/9/14 - Seung-Won, Bae) for Pair Tick
		CChartItem *pChartItem = GetpChartItemSelected();
		if( pChartItem)
		{
			if( pCodeData->m_chType == TICK_DATA_CHART && pCodeData->m_lTypeNum == 1
				&& pChartItem->m_nCheHoga == 1 && pChartItem->FindChartItemDataForQuery())
			{
				int nRequestPairTickState = E_RPTS_START;
				switch( ( ( CStdDialog *)GetParent())->GetRequestPairTickState())
				{
					case E_PRTS_ROTATE_START:	nRequestPairTickState = E_PRTS_ROTATE_START;
					default:					( ( CStdDialog *)GetParent())->StartRequestPairTick( pChartItem->FindChartItemDataForQuery()->GetCodeForRqstFromChartItemData(), nRequestPairTickState);
												break;
					case E_PRTS_ROTATE_PAIR:	( ( CStdDialog *)GetParent())->RequestPairTick( pChartItem->FindChartItemDataForQuery()->GetCodeForRqstFromChartItemData(), E_PRTS_ROTATE_PAIR);
												break;
				}
			}
			else	( ( CStdDialog *)GetParent())->ClearPairTick( pChartItem->GetSelectedRQ());
		}

// --> [Edit]  강지원 2008/07/22	( 이부분이 조회요청하는부분인거 같음 )
		RequestChartData();

// --> [Edit]  강지원 2008/11/07	( 호가창이 열려있을경우... 호가창도 재조회를 시켜준다. )
		if(!m_pFloatingDlg->m_bIconState)
		{
			//호가를 요청 한다.
			((CStdDialog*)GetParent())->Request_StockHogaData();
		}
// <-- [Edit]  강지원 2008/11/07
// <-- [Edit]  강지원 2008/07/22
	}		
	return TRUE;
}

BOOL CChartMng::SetType(char chType, long lNumber)
{
	// 틱(0)/분(1)/일(2)/주(3)/월(4)/년(5)/초(6)
	if (m_bTypeLink && !m_bChartExtended)
	{
		InitInLoop();
		m_chTypeInLoop = chType;
		m_lNumberInLoop = lNumber;
		HeadInLoop();
	}
	else
	{
//		if(GetpChartItemSelected()->GetDataCode()!="")
		{
			if(!GetpChartItemSelected()) return FALSE;

			GetpChartItemSelected()->SetType(chType);
			
			// 2005. 05. 20 -- 일 차트도 간격을 넘긴다.
			if (chType == TICK_DATA_CHART || chType == SEC_DATA_CHART || chType == MIN_DATA_CHART ||
				chType == DAY_DATA_CHART || chType == HOUR_DATA_CHART)
			{
				GetpChartItemSelected()->SetTickMinDayNumber(lNumber);
			}

// --> [Edit]  강지원 2008/10/21		( 여기서 멀티차트의 구분을 해주면 안될것 같음... 박승룡 대리도 뺏다고 함.. )
			CChartItem *pChartItem = GetpChartItemSelected();
			int nChartCnt = pChartItem->GetChartItemCount();
			if (nChartCnt == 1 && GetCheHogaType() == 0 && !m_bInitFlag)
			{
//				if(!((CStdDialog*)GetParent())->IsMultiItemChart())
				{
					m_bChangeTypeMode = TRUE;

					// 주기변경...
					Synchro_jipyo();
				}
			}
			m_bInitFlag = FALSE;	//	초기화 버튼클릭시 정보를 저장하지 않기 위해서
// <-- [Edit]  강지원 2008/10/21
			//=====================================

			// (2008/9/14 - Seung-Won, Bae) for Pair Tick
			if( pChartItem && GetParent())
			{
// --> [Edit]  강지원 2008/10/24
				//	호가 버튼이 클릭되어있고 틱일경우 매도/매수 틱을 그리기위한 처리로 들어간다.
				if (GetCheHogaType() == 1 && chType == TICK_DATA_CHART && lNumber == 1 && pChartItem->FindChartItemDataForQuery())
					((CStdDialog*)GetParent())->StartRequestPairTick(pChartItem->FindChartItemDataForQuery()->GetCodeForRqstFromChartItemData());
				else
				{
					((CStdDialog*)GetParent())->ClearPairTick(pChartItem->GetSelectedRQ());

					//	선택된 종목코드가 없으면 종목콤보에서 가져다가 종목코드를 세팅한다. 
					CString strSelectCode = pChartItem->GetSelectedItemCode();
					if ( strSelectCode.IsEmpty() )
						strSelectCode = ((CInputBarDlg*)m_pInputBarDlg)->m_pCodeInput->GetCode();

					GetpChartItemSelected()->SetCodeForQuery(CStdDialog::GetRealItemCode(strSelectCode),QUERY_STRFOFOREIGN,TRUE);

					
				}
// <-- [Edit]  강지원 2008/10/24
			}

// --> [Edit]  강지원 2008/11/03
			//	TR서비스를 여기서 바꿔줘야 하지 않을까??
//			if ( GetCheHogaType() == 1 )
//				GetpChartItemSelected()->SetCodeForQuery(((CInputBarDlg*)m_pInputBarDlg)->GetCurrentCode(),QUERY_STRFOFOREIGN_HOGA2,TRUE);
//			else
//				GetpChartItemSelected()->SetCodeForQuery(CStdDialog::GetRealItemCode(((CInputBarDlg*)m_pInputBarDlg)->GetCurrentCode()),QUERY_STRFOFOREIGN,TRUE);
// <-- [Edit]  강지원 2008/11/03

			// 복수종목 추가 : 복수종목 - ojtaso (20070118)
// --> [Edit]  강지원 2008/12/12	( 데이터 추가조회 플래그 설정 )
			GetpChartItemSelected()->SetNext(FALSE);
// <-- [Edit]  강지원 2008/12/12
			RequestChartData(TRUE);
		}
	}
	return TRUE;
}

#undef OBJECT_SELECT
#include "../chart_common/BlockBaseEnum.h"

LRESULT CChartMng::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(((CStdDialog*)GetParent())->IsChartHide())
	{
		switch(message)
		{
		// 종목코드 입력시 조회처리
		case UMSG_INPUTCODETOCHARTMNG:
			{
//@유진추가
				STINPUTCODEDATA* pStInputCodeData = (STINPUTCODEDATA*)lParam;
				CString strCode = pStInputCodeData->m_strCode;
				UINT nDefault = pStInputCodeData->m_nDefault;
				UINT nMode = pStInputCodeData->m_nMode;
				nMode = ((CInputBarDlg*)m_pInputBarDlg)->GetCurrentMode(nDefault, nMode);
				GetParent()->SendMessage(UMSG_SENDSHAREDCODE, (WPARAM)(LPCTSTR)strCode);
//@유진추가
			}
			break;
		case UMSG_GETJCODECHECKEDFROMCHART:// keep going...			
			break;
		case UMSG_GETINC:
			return 0L; // return 0(send mode)..
			break;

		default:
			return 1L; // don't do antying..
			break;
		}
	}
	CString sTemp;
	switch(message)
	{
	case WM_COMMAND:
		{
			int nMenuID = (int)wParam;
			if(nMenuID >= ID_PLM_ADD_MENU_STARTANALTOOL && nMenuID <= ID_PLM_ADD_MENU_ENDANALTOOL)
			{
				int nToolBtnID = nMenuID - ID_PLM_ADD_MENU + ID_TOOLMENU_BASE;
				SetDlgToolBtnToggle(nToolBtnID, TRUE);
			}
			else
			{
				UINT nMainMenuID=0;
				for(int i=0; i<menuChart.GetSize(); i++)
				{
					if(menuChart.GetAt(i)==nMenuID)
					{
						nMainMenuID = m_menuMain.GetAt(i);
						break;
					}
				}
				if(nMainMenuID>0)
				{
					HWND hChildFrame = (HWND)GetParent()->GetParent()->SendMessage(WM_USER+104);
					if ( !hChildFrame) break;

					GetParent()->GetParent()->PostMessage( WM_COMMAND, nMainMenuID, 0);
				}
				// (2008/9/13 - Seung-Won, Bae) Support MaxMize Menu
				// return 1L;

//				if(nMenuID >= ID_PLM_ADD_MENU_BEGINMAIN && nMenuID <= ID_PLM_ADD_MENU_ENDMAIN)
//				{
//					int nMainMenuID = nMenuID - ID_PLM_ADD_MENU + ID_MAINMENU_BASE;
//					HWND hChildFrame = (HWND)GetParent()->GetParent()->SendMessage(WM_USER+104);
//					if ( !hChildFrame) break;
//
//					GetParent()->GetParent()->PostMessage( WM_COMMAND, nMainMenuID, 0);
//				}
//				else if(nMenuID >= ID_PLM_ADD_MENU_BEGINMAINCONN && nMenuID <= ID_PLM_ADD_MENU_ENDMAINCONN)
//				{
//					int nMainConnMenuID = nMenuID - ID_PLM_ADD_MENU + ID_MAINCONNMENU_BASE;
//					HWND hChildFrame = (HWND)GetParent()->GetParent()->SendMessage(WM_USER+104);
//					if ( !hChildFrame) break;
//					
//					GetParent()->GetParent()->PostMessage( WM_COMMAND, nMainConnMenuID, 0);
//					return TRUE;
//				}
			}
		}
		break;
	// 종목코드 입력시 조회처리
	case UMSG_INPUTCODETOCHARTMNG:
		{
			STINPUTCODEDATA* pStInputCodeData = (STINPUTCODEDATA*)lParam;
			CString strCode = pStInputCodeData->m_strCode;
			UINT nDefault = pStInputCodeData->m_nDefault;
			UINT nMode = pStInputCodeData->m_nMode;
			nMode = ((CInputBarDlg*)m_pInputBarDlg)->GetCurrentMode(nDefault, nMode);
			
			if(((CStdDialog*)GetParent())->IsMultiItemChart())
			{
				SetMode(nMode);
				
				if(nMode == CHANGE_ITEM)
				{
//					m_nMode = CHANGE_ITEM;
					m_bAddModeFromTab = FALSE;
//					((CInputBarDlg*)m_pInputBarDlg)->SetMode(CHANGE_ITEM);
				}
				else if(nMode == NEW_CHART)
				{
//					m_nOldMode = CHANGE_ITEM;
//					m_nMode = NEW_CHART;
//					((CInputBarDlg*)m_pInputBarDlg)->SetMode(NEW_CHART);
				}
				else if(nMode == NEW_ITEM)
				{
//					m_nOldMode = CHANGE_ITEM;
//					m_nMode = NEW_ITEM;
					m_bAddModeFromTab = TRUE;
//					((CInputBarDlg*)m_pInputBarDlg)->SetMode(NEW_ITEM);
				}
			}

//@유진삭제
//			STINPUTCODEDATA *pStInputCodeData = (STINPUTCODEDATA *)lParam;	
//@유진삭제
	
#ifdef _WRITELOG
			// Log - ojtaso (20080107)
			((CStdDialog*)GetParent())->WriteToStringLog(_T("종목탭에서 종목 변경됨"), pStInputCodeData->m_strCode, pStInputCodeData->m_strCode.GetLength());
#endif
			STINPUTCODEDATA2	stInputCodeData2;
			stInputCodeData2.m_strCode = pStInputCodeData->m_strCode;
			//if(stInputCodeData2.m_strCode.GetLength() == CODE_STOCK_LENGTH2)
			//	stInputCodeData2.m_strCode.Format("%s", pStInputCodeData->m_strCode);
			stInputCodeData2.m_bDontPutInShared = FALSE;
			if(m_bAddModeFromTab)
				stInputCodeData2.m_bRequestWithoutHesitation = TRUE;
			else
				stInputCodeData2.m_bRequestWithoutHesitation = FALSE;
			
			//@유진추가
			CChartItem* pInitChartItem = GetpChartItemSelected();
			if(pInitChartItem && !m_bOnEventMessage && pInitChartItem->m_bNowInitialState)
			{
				pInitChartItem->m_bNowInitialState = FALSE;
				stInputCodeData2.m_bRequestWithoutHesitation = TRUE;
			}
			//@유진추가

			if(m_pInputBarDlg)
			{
// --> [Edit]  강지원 2008/11/07		( 종목코드선택 및 차트 선택으로 종목코드가 바뀔때.. 구분을 해줘야 하는데... )
//				((CInputBarDlg*)m_pInputBarDlg)->SetShareCodeChanged(FALSE);
				((CInputBarDlg*)m_pInputBarDlg)->m_bDeleteChart = FALSE;
				m_pInputBarDlg->SendMessage(UMSG_INPUTCODETOINPUTBAR,(WPARAM)&stInputCodeData2);			
				((CInputBarDlg*)m_pInputBarDlg)->m_bDeleteChart = TRUE;
// <-- [Edit]  강지원 2008/11/07
			}
			if(pStInputCodeData->m_strTime != "")
			{
				m_strTimeMarkReserved = pStInputCodeData->m_strTime;
			}	
			if(wParam != NULL)
			{
				m_nMode = m_nOldMode;
				m_nOldMode = -1;
			}
		}
		break;
	case UMSG_INPUTCODEFROMINPUTBAR:
		{
			//GetpChartItemSelected()->SetKorName(CString((const char*)lParam));
			InputCodeFromInputBar((CCodeDataFromInputBar *)wParam);
		}
		break;
//	case UMSG_RECEIVE_MARKETTR:
//		{
//
//		}
//		break;
	case UMSG_RECV_MARKET_TR:
		{
			//시장 지표 TR 받음.
			CString sChatID;
			sChatID.Format("%8.8s", (char*)lParam);
			UINT nWndID = atoi(sChatID);
			CChartItem* pRcvChartItem = NULL;
			EACH_CHARTJOB_START()
				if(pChartItem->GetDlgCtrlID() == nWndID)
				{
					pRcvChartItem = pChartItem;
					goto ExitLoop3;
				}
			EACH_CHARTJOB_END();
ExitLoop3:
			//pRcvChartItem = GetpChartItemSelected();
			if(pRcvChartItem == NULL) return FALSE;

			if(!((long)wParam))
			{
				pRcvChartItem->SetNext(FALSE);

				//==========================================================
				for(int i = 0; i < pRcvChartItem->m_arrayMarketTRData.GetSize();i++)
				{
					CChartItemData* pChartItemData = NULL;
					pChartItemData = pRcvChartItem->m_arrayMarketTRData.GetAt(i);
					if(pChartItemData)
						pChartItemData->SetBNextMarketData(FALSE);
				}
				//==========================================================


				return TRUE;
			}

			pRcvChartItem->ReceiveMarketChartData((char*)lParam, (long)wParam);	


			//================================================================================
			//*/
			if(m_bLastEnvUpdated == FALSE) //로드가 다 안 끝났을 경우.
			{
				SetFocusToJMEdit_At_LoadEnd();
			}

			
			//================================================================================
		}
		break;
	case UMSG_RECEIVEFROMSERVER:
		{			
			stReceiveFromServer* pStR = (stReceiveFromServer*)lParam;

			if(!GetpChartItemSelected()) return FALSE;
			if(!pStR->m_dwLength) 
			{
				GetpChartItemSelected()->SetNext(FALSE);

				//==========================================================
				for(int i = 0; i < GetpChartItemSelected()->m_arrayMarketTRData.GetSize();i++)
				{
					CChartItemData* pChartItemData = NULL;
					pChartItemData = GetpChartItemSelected()->m_arrayMarketTRData.GetAt(i);
					if(pChartItemData)
						pChartItemData->SetBNextMarketData(FALSE);
				}
				//==========================================================

				return TRUE;
			}
			CChartItem* pSelChartItem = GetpChartItemSelected();
			// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
			long lRetValue = 0;
			if(pSelChartItem->IsLoadingRQ())
			{
				pSelChartItem->ReceiveLoadingChartData( pStR->m_pBuffer, pStR->m_dwLength);			
				pSelChartItem->SetBEnableIndicatorCalculation(TRUE);
				return CWnd::WindowProc(message, wParam, lParam);
			}
			else
			{
				pSelChartItem->SetBEnableIndicatorCalculation(FALSE);
				lRetValue = pSelChartItem->ReceiveInitalChartData( pStR->m_pBuffer, pStR->m_dwLength);			
			}

			//2007.03.22 by LYH 스윙등 전체 보기 화면 오류 때문에 viewCount 세팅하기 전에 차트 그리도록 함
			pSelChartItem->SetBEnableIndicatorCalculation(TRUE);

			// 이미 위 함수에서 차트는 그린다. =======================================================
			/*int nType = 1;
			switch(GetpChartItemSelected()->GetType())
			{
				case TICK_DATA_CHART	: nType = 0; break;
				case MIN_DATA_CHART		: nType = 1; break;
				case DAY_DATA_CHART		: nType = 2; break;
				case WEEK_DATA_CHART	: nType = 3; break;
				case MONTH_DATA_CHART	: nType = 4; break;
				case YEAR_DATA_CHART	: nType = 5; break;
			}
			long lZoomValue = m_pInputBarDlg->SendMessage(UMSG_GETZOOMVALUE,nType);
			*/			
			long lZoomValue = m_pInputBarDlg ? m_pInputBarDlg->SendMessage(UMSG_GETZOOMVALUE,pSelChartItem->GetType()) : 0;	
				
//			long lTempPageDataCount = pSelChartItem->GetTempOnePageDataCount();		
//			
//			if(lTempPageDataCount)
//			{
//				if(m_pInputBarDlg)
//					m_pInputBarDlg->SendMessage(UMSG_SETZOOMVALUE,lTempPageDataCount);
//				lZoomValue = lTempPageDataCount;
//			}

			
			if(lZoomValue)
			{
				if((lZoomValue>pSelChartItem->GetAllCnt()&&pSelChartItem->GetAllCnt()))
				{
					pSelChartItem->SetNew_OnePageDataCount(pSelChartItem->GetAllCnt());
				}
				else
				{
					pSelChartItem->SetNew_OnePageDataCount(lZoomValue);
				}				
			}
			else
			{
				pSelChartItem->SetNew_OnePageDataCount(pSelChartItem->GetAllCnt());
			}

			
			
			char Type = pSelChartItem->GetType();
			/* solomon
			if(m_strTimeMarkReserved!=""
					&&(	pSelChartItem->GetType()!= WEEK_DATA_CHART
					&&	pSelChartItem->GetType()!= MONTH_DATA_CHART
					&&	pSelChartItem->GetType()!= DAY_DATA_CHART		))
			{
				pSelChartItem->SetTimeMark(m_strTimeMarkReserved,TRUE);				
			}
			*/
			m_strTimeMarkReserved.Empty();

			
			

			if(m_bHideScale)
			{
				pSelChartItem->HideScale(m_bHideScale);
			}		
			if(m_bHideInnerToolBar)
			{
				pSelChartItem->HideInnerToolBar(m_bHideInnerToolBar);
			}
			switch(lRetValue)
			{
			case 1:
				{				
					RequestAVGForGuideLine(TRUE);										
					SendInitChartData(TRUE, TRUE/*bFromTRData*/);
					//========================================================================
					//
					//========================================================================
					
					/* 05.08.27
					BOOL bShowLock = FALSE;
					if(GetpChartItemSelected()->GetType() == DAY_DATA_CHART)
					{
						bShowLock = m_pExtInputBarDlg->SendMessage(UMSG_GETSHOWLOCK);
					}
					GetpChartItemSelected()->ShowLock(bShowLock);
					*/
					if(!SetJCodesNext())
					{
						TailInLoop();	
						if(m_nOldMode >= 0)
						{
							m_nMode = m_nOldMode;
							m_nOldMode = -1;
						}
						if(m_nMode == ADD_NEW_CHART && m_pInputBarDlg)
							m_pInputBarDlg->SendMessage(UMSG_EDIT_FOCUS);
					}
					else
					{
						if(GetBReplayChartMode() && m_pInputBarDlg)
						{
							// 2005. 10. 18
							// 복기 상태에서 받아야 할 데이터를 다 받았을 경우 Play버튼 활성화 한다.
							m_pInputBarDlg->SendMessage(UMSG_ENABLE_PLAYBTN);
						}
					}				
				}
				break;
			case 2:				
				if(!RequestChartData())
				{					
					TailInLoop();
				}
				break;
			default:				
				TailInLoop();
				break;
			}
			
			
			// gm0604 added. Start
			((CStdDialog*)GetParent())->SetChartBySystemTrading(FALSE);
			// gm0604 added. End
			CChartItem* pChartItem = GetpChartItemSelected();
			if(pChartItem)
			{
				pChartItem->UpdateSystemTradingIndexByTab();
			}			

			//05.09.08
			if(m_pInputBarDlg)
				m_pInputBarDlg->SendMessage(UMSG_END_REQUEST_CHARTDATA);
			
			if(m_bExistSyncChart)
				m_bExistSyncChart = FALSE;		

			//종목 코드에 포커스가 가도록  한다.
			if (GetBNoExistLoadCnts() && m_bLastEnvUpdated && m_pInputBarDlg)
			{
				m_pInputBarDlg->SendMessage(UMSG_EDIT_FOCUS);
				SetBNoExistLoadCnts(FALSE); //다음에 다시 안 불리게 하기 위해 False로 셋
			}
		}
		break;
	case UMSG_CHANGEMODE:
		{
			int nMode = lParam;
			// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)
			if(nMode == CHANGE_ITEM)
				SetAddMode(FALSE);
			else if(((CStdDialog*)GetParent())->IsMultiItemChart())
			{
				if(nMode == NEW_ITEM)
				{
					SetAddMode(TRUE);
//					nMode = CHANGE_CHART;
				}
				else if(nMode == OVERLAPP_ITEM)
				{
					SetAddMode(TRUE);
//					nMode = CHANGE_CHART;
				}
			}

			m_nOldMode = GetMode();
			SetMode(nMode);
		}
		break;
	case UMSG_SHOWINDICATORCONFIGDLG:
		if(!GetpChartItemSelected()) return FALSE;
		if(wParam == 1)
			GetpChartItemSelected()->ShowIndicatorConfigDlgWithName("가격 이동평균");
		else if(wParam == 2)
			GetpChartItemSelected()->ShowIndicatorConfigDlgWithName("거래량 이동평균");
		else
			GetpChartItemSelected()->ShowIndicatorConfigDlg();
		break;
	case UMSG_SETTYPE:
		{
			char chType = wParam;
			long lNumber = lParam;
			SetType( chType, lNumber);			
		}
		break;
	case UMSG_SETZOOMVALUE:
		{
			if(!GetpChartItemSelected()) return FALSE;
			GetpChartItemSelected()->SetNew_OnePageDataCount(wParam);
			if(m_pInputBarDlg)
				m_pInputBarDlg->SendMessage(UMSG_SETZOOMVALUE,wParam);
		}
		break;
	case UMSG_SETCNTS:
		{
			if(!GetpChartItemSelected()) return FALSE;
			GetpChartItemSelected()->SetCntForRequest(wParam);

// --> [Edit]  강지원 2008/11/19		( 호가차트보기일경우 추가 )
			CChartItem *pChartItem = GetpChartItemSelected();
			char chType = pChartItem->GetType();
			if( GetCheHogaType() == 1 && chType == TICK_DATA_CHART && pChartItem->FindChartItemDataForQuery())
			{
				( ( CStdDialog *)GetParent())->StartRequestPairTick( pChartItem->FindChartItemDataForQuery()->GetCodeForRqstFromChartItemData());
			}
			else
			{
				( ( CStdDialog *)GetParent())->ClearPairTick( pChartItem->GetSelectedRQ());

				//	선택된 종목코드가 없으면 종목콤보에서 가져다가 종목코드를 세팅한다. 
				CString strSelectCode = pChartItem->GetSelectedItemCode();
				if ( strSelectCode.IsEmpty() )
					strSelectCode = ((CInputBarDlg*)m_pInputBarDlg)->m_pCodeInput->GetCode();
				
				pChartItem->SetCodeForQuery(CStdDialog::GetRealItemCode(strSelectCode),QUERY_STRFOFOREIGN,TRUE);
			}
// <-- [Edit]  강지원 2008/11/19

			// 복수종목 추가 : 복수종목 - ojtaso (20070118)
// --> [Edit]  강지원 2008/12/12	( 데이터 추가조회 플래그 설정 )
			pChartItem->SetNext(FALSE);
// <-- [Edit]  강지원 2008/12/12
			RequestChartData(TRUE);
			GetpChartItemSelected()->SetSaveAllCount(wParam);
		}
		break;
	case UMSG_SETMATRIX:
		{			
			SetMatrix(wParam,lParam);
			
// --> [Edit]  이도영 2008/09/22
// 0,0 으로 선택을 돌린다.
			SetChartSelected(CPoint(0,0),TRUE);			
// <-- [Edit]  이도영 2008/09/22

// --> [Edit]  강지원 2008/11/07		( 수치조회 중이면 새로 생성되는 차트들한테도 알려준다. )
			if(m_pInquiryDlg)
			{
				((CStdDialog*)GetParent())->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_NIDLG_OVER, TRUE);
			}
// <-- [Edit]  강지원 2008/11/07

			// (2008/11/7 - Seung-Won, Bae) Set Main Mode of CrossTool.
			if( m_pExtInputBarDlg)
				if( ( ( CExtInputBarDlg *)m_pExtInputBarDlg)->m_btSub2.GetCheck())
					( ( CStdDialog *)GetParent())->SendMessage( RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_CROSSHAIRS_TOOL, TRUE);

			//SetMatrix후 차트를 선택된 

			//2005. 06. 23 add by sy, nam
			//GetParent()->SendMessage(message, wParam, lParam);
			/*if(wParam * lParam == 1)	
			{
				CreateFloatingHogaWnd();
			}
			else if(m_pFloatingDlg != NULL)
			{
				DestroyFloatingHogaWnd();				
			}*/				

			//요청된 호가 Real 이  있다면 해제한다.			
		}
		break;
	case UMSG_REQUESTMORE:
		{
//{{JS.Kim_20100906 데이터추가부르기
//			CChartItem* pSelChartItem = NULL;
//			pSelChartItem = GetpChartItemSelected();
//			if(!pSelChartItem) return FALSE;
//			CString strNext;
//			strNext.Format("%s", pSelChartItem->GetNextKey());
//			strNext.TrimLeft();
//			if(strNext.GetLength()<1)
//			{
//				 return FALSE;
//			}
//
//
//// --> [Edit]  강지원 2008/11/03
//			//	데이터가 4000건이 넘으면 조회를 막는다. 
//			if ( pSelChartItem->GetNFullDataCount() >= CHARTDATA_MAX_CNT )
//				return FALSE;
//// <-- [Edit]  강지원 2008/11/03
//
//			pSelChartItem->SetNext(TRUE);
//			pSelChartItem->ClearData();
//			// 복수종목 추가 : 복수종목 - ojtaso (20070118)
//
//
//// --> [Edit]  강지원 2008/12/27
//			CChartItemData* pChartItemData = pSelChartItem->GetItemDataAt(0);
//			long nCount(0);
//			nCount = (int)wParam /*pSelChartItem->GetAllCnt()*/ + pChartItemData->GetCntForQuery(TRUE);
//
//			if ( nCount > 2000 )
//				nCount = 2000;
//
////			pSelChartItem->SetCntForRequest(pChartItemData->GetCntForQuery(TRUE));
//			pSelChartItem->SetSaveAllCount(nCount);
//// <-- [Edit]  강지원 2008/12/27
//
//
//			RequestChartData(TRUE);
			CChartItem* pSelChartItem = NULL;
			pSelChartItem = GetpChartItemSelected();
			if(!pSelChartItem) return FALSE;
			CString strNext;
			strNext.Format("%s", pSelChartItem->GetNextKey());
			strNext.TrimLeft();
			if(strNext.GetLength()<1)
			{
				 return FALSE;
			}
			pSelChartItem->SetNext(TRUE);
			pSelChartItem->ClearData();
			// 복수종목 추가 : 복수종목 - ojtaso (20070118)
			RequestChartData(TRUE);
//}}
		}
		break;
	case UMSG_RESETCHART:
		{
			ResetChartAll();
			//CChartItem* pChartItem = NULL;
			//pChartItem = GetpChartItemSelected();
			//if(pChartItem == NULL || !pChartItem->m_bReceivedDataChart) 
			//	return 0;
			//ResetChart();
			//// 선택된 차트에 새로 요청할 데이터 default 갯수 셋팅
			//if(((CStdDialog*)GetParent())->m_nChartType==MINI_CHART) 
			//	pChartItem->SetCntForRequest(90);
			//else
			//	pChartItem->SetCntForRequest(DEFAULTCNTATONCE);
			//pChartItem->SetSaveAllCount(wParam);

			////초기화 시키도록 정보 제공.
			//if(m_pExtInputBarDlg->GetSafeHwnd())
			//	m_pExtInputBarDlg->SendMessage(UMSG_CHART_INIT_FROMOCX);

			////초기화 후 다시 TR재전송
			//if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
			//	m_pInputBarDlg->SendMessage(UMSG_CHART_INIT_FROMOCX);
		}
		break;
	case UMSG_RESETOCX:
		{
			//===========================================================
			//===========================================================
			//if(m_pExtInputBarDlg)
			//	m_pExtInputBarDlg->SendMessage(UMSG_RESETOCX, 0, 0);
			//===========================================================

			SetMatrix(1,1);
			ResetOCX();
		}
		break;
	case UMSG_APPLYFAVORITE:
		{
			CString strItem = (LPCTSTR)lParam;
			CString strTabName;
			CString strSubItemName;
			int nPos = strItem.Find(']');
			strTabName = strItem.Mid(1,nPos-1);
			strSubItemName = strItem.Right(strItem.GetLength()-nPos-2);
			if(strTabName==TAB_CHART)
			{
				if(!GetpChartItemSelected()) return FALSE;
				GetpChartItemSelected()->AddChartBlock(strSubItemName);
			}			
		}
		break;
	case UMSG_GETNAMEINDICATOR:
		{
			GetIndicatorNameListIntoArray((CStringArray *)lParam, (LPCTSTR)wParam);
		}
		break;
	case UMSG_HASGRAPH:
		{
			if(!GetpChartItemSelected()) return FALSE;
			return GetpChartItemSelected()->HasGraph((LPCTSTR)wParam);
		}
		break;
	case UMSG_DEL_CHART_BLOCK:
		{
			if(!GetpChartItemSelected()) return FALSE;
			return GetpChartItemSelected()->RemoveChart((LPCTSTR)wParam);
		}
		break;
	case UMSG_ADDCHARTBLOCK:
		{
			CChartItem* pSelChartItem = NULL;
			pSelChartItem = GetpChartItemSelected();
			if(!pSelChartItem) return FALSE;

			int nFrom = (int)lParam;


			//05.05.30 add by sy, nam =========================================
			//Input Bar의 Combo의 선택된 값도 변화 시킨다. 
			if( nFrom == FROM_TABCHART && m_pInputBarDlg && (m_pInputBarDlg->GetSafeHwnd()))
				m_pInputBarDlg->SendMessage(UMSG_CHANGE_CHARTVALUE, wParam, lParam);

			//CTabChart의 리스트 선택 항목도 변화 시킨다. 
			//			if( nFrom == FROM_INPUTBAR && (m_pTabChart->GetSafeHwnd()))
			//				m_pInputBarDlg->SendMessage(UMSG_CHANGE_CHARTVALUE, wParam, lParam);			
			//=================================================================
			
			//2007.01.26 by LYH 지표 정보를 파일에서 읽도록 수정
			CString strIndicondFile = m_strRootPath + "\\Icss\\indicond.cfg";//solomon
			CString strGroup;
			CString strJipyo = (LPCTSTR)wParam;
			::GetPrivateProfileString(strJipyo, "groupName", "", strGroup.GetBuffer(MAX_PATH), MAX_PATH, strIndicondFile);

			//if(nFrom == FROM_TABJIPYO_MARKET || nFrom == FROM_EXTINPUTBAR_MARKET)
			if(strGroup == "시장지표")
			{
				m_bMarketJipyoAdding = TRUE;
				if( pSelChartItem->GetType() != DAY_DATA_CHART )
				{
					AfxMessageBox("시장지표는 일간일때만 적용됩니다");
					return 0;
				}

				CString strMessage;
				//{{투자자 및 기타지표 메시지
				if(strJipyo == "시가총액" || strJipyo == "외국인 보유량" || strJipyo == "외국인 보유율"
					|| strJipyo == "외국인순매수" || strJipyo == "기관순매수량" || strJipyo == "기관순매수누적")
				{
					if(pSelChartItem->GetDataTRCode() != QUERY_STRFOFOREIGN)
					{
						strMessage.Format("%s 지표는 주식에서만 조회가 가능합니다", strJipyo);						
						AfxMessageBox(strMessage);
						return FALSE;
					}
				}
				else if(strJipyo == "미결제약정")
				{
					if(pSelChartItem->GetDataTRCode() != QUERY_STRFUTOPT/* && pSelChartItem->GetDataTRCode() != QUERY_STRCOMMODITY*/)	// 상품선물옵션차트 - ojtaso (20071001)
					{
						strMessage.Format("%s 지표는 선물에서만 조회가 가능합니다", strJipyo);						
						AfxMessageBox(strMessage);
						return FALSE;
					}
				}

			}

			//환경동기화 적용 상태인지 확인		05.09.26	
			if( m_pExtInputBarDlg->SendMessage(UMSG_GETENVSINK) == 0 ) //동기화 상태 아님.
			{
				if(nFrom == FROM_INPUTBAR || nFrom == FROM_TABCHART)
				{	
					if(!pSelChartItem->GetDataCode().GetLength()) 
					{
						if(m_bMarketJipyoAdding)
							m_bMarketJipyoAdding = FALSE;	

						return 0l;
					}

					CString sValue = (LPCTSTR)wParam;
					pSelChartItem->SetChartLineType(sValue);
				}				
				
				pSelChartItem->AddChartBlock((LPCTSTR)wParam);
			}
			else
			{
				CChartItem *pChartItem = NULL;
				for(int nRowIndex=0;nRowIndex<m_lRows;nRowIndex++)
				{  
					for(int nColIndex=0;nColIndex<m_lCols;nColIndex++)
					{  
						pChartItem = m_pChartItems[nRowIndex][nColIndex];  
						if(pChartItem)
						{
							char chBaseType		= pSelChartItem->GetType();  //기준 차트의 조회 시간단위 Get
							char chInLoopChartType	= pChartItem->GetType(); //빈 차트의 조회 시간단위 Get
							if(lstrlen(&chInLoopChartType) == 0)				  //지정된 조회 타입이 없으면 	
								chInLoopChartType	= pChartItem->GetTypeRequested();//요청하는 타입을 Get

							if(lstrlen(&chInLoopChartType) == 0) continue; //그래도 가져오는게 없으면 Return

							if( chBaseType == chInLoopChartType ) //기준 차트를 찾은 경우 
							{			
								if(pChartItem->m_bReceivedDataChart != TRUE)  //조회 타입이 같더라도 데이터가 없는 경우 무시한다.
									continue;

								if(nFrom == FROM_INPUTBAR || nFrom == FROM_TABCHART)
								{	
									if(!pChartItem->GetDataCode().GetLength()) continue;

									CString sValue = (LPCTSTR)wParam;
									pChartItem->SetChartLineType(sValue);
								}
								pChartItem->AddChartBlock((LPCTSTR)wParam);
							}
						}
					}
				}			
			}

			if(m_bMarketJipyoAdding)
				m_bMarketJipyoAdding = FALSE;			

			return TRUE;

		}
		break;
	case UMSG_SETJONGMOKLINK:
		{
			m_bJongmokLink = wParam;
			if(m_bJongmokLink)
			{
				if(!GetpChartItemSelected()) return FALSE;
				CCodeDataFromInputBar CodeData;
				CodeData.m_strCode = GetpChartItemSelected()->GetDataCode();
				CodeData.m_lEndTime = atol(GetpChartItemSelected()->GetEndDateRequested());
				InputCodeFromInputBar(&CodeData,TRUE);				
			}
		}
		break;
	case UMSG_SETTYPELINK:
		{
			m_bTypeLink = wParam;
			if(m_bTypeLink)
			{
				if(!GetpChartItemSelected()) return FALSE;
				char chType = GetpChartItemSelected()->GetType();
				long lTickMinNumber = atol(GetpChartItemSelected()->GetTickMinDayNumber());
				if(chType == TICK_DATA_CHART || chType== MIN_DATA_CHART || chType == DAY_DATA_CHART || chType == HOUR_DATA_CHART/*05.05.24 add by sy, nam*/)
				{
					this->SendMessage(UMSG_SETTYPE,(WPARAM)chType,(LPARAM)lTickMinNumber);				
				}
				else
				{
					this->SendMessage(UMSG_SETTYPE,(WPARAM)chType);
				}
			}
		}
		break;
	case UMSG_LOADCHART:
		{
			CPoint* pPt = (CPoint*)lParam;
			LoadChart(pPt);
		}
		break;
	case UMSG_SAVECHART:
		{
			CPoint* pPt = (CPoint*)lParam;
			SaveChart(pPt);
		}
		break;
	case UMSG_LOADSAVECHART:
		{
			CPoint* pPt = (CPoint*)lParam;
			LoadSaveChart(pPt);
		}
		break;
	case UMSG_ADDUPJONGDATA:
		{
			CString strCode;
			strCode.Format("%d",wParam);
			AddUpjongData(strCode);
		}
		break;
	case UMSG_FIXDATE:
		{
			wParam = GetpChartItemSelected()->GetNOnePageDataCount();

			InitInLoop();			
			m_strEndDateInLoop = (LPCTSTR)lParam;
			m_lZoomValueInLoop = wParam;
			m_dEndDateInLoop = atof(m_strEndDateInLoop);
			m_lTypeInLoop = LOOPTYPE_FIXDATE/*2*/;// fixdate
			HeadInLoop();

//			CString strEndDate = (LPCTSTR)lParam;
//			long lZoomValue = wParam;
//			long lColSelected = m_lColSelected;
//			long lRowSelected = m_lRowSelected;
//			double dEndDate = atof(strEndDate);
//			EACH_CHARTJOB_START()
//				if(pChartItem->GetType()=='2'||pChartItem->GetType()=='3'||pChartItem->GetType()=='4')
//				{
//					SetChartSelected(CPoint(nColIndex,nRowIndex));
//					if(!GetpChartItemSelected()->ScrollToDataOfEnd(dEndDate))
//					{
//						GetpChartItemSelected()->SetEndDateRequested((LPCTSTR)strEndDate);
//						RequestChartData();					
//					}
//					GetpChartItemSelected()->ZoomTo((short)lZoomValue,TRUE);
//				}
//			EACH_CHARTJOB_END()	
//			SetChartSelected(CPoint(lColSelected,lRowSelected));
		}
		break;
	case UMSG_SHOWSCALE:
		{
			m_bHideScale = wParam;
			ShowScale();
		}
		break;
	case UMSG_SHOWINNERTOOLBAR: 
		{
			m_bHideInnerToolBar = wParam;
			ShowInnerToolbar();
		}
	case UMSG_UPDATEINDEX:
		{
			UpdateIndex();
		}
		break;
	case UMSG_SETONECHART:
		{
			ReplaceChart(0,0,m_lRowSelected,m_lColSelected);
			SetMatrix(1,1);

			//@유진추가
			{
				CChartItem* pInitChartItem = GetpChartItemSelected();
				if(pInitChartItem)
					pInitChartItem->m_bNowInitialState = TRUE;
			}
			//@유진추가
		}
		break;
//	case UMSG_ADDMARKETDATA:
//		{
//			CString strCode = (LPCTSTR)wParam;	
//			UINT	nID		= (UINT)lParam;
//			//CWnd* pWnd = (CWnd*)lParam;
//			//AddMarketData(strCode, pWnd);
//			//AddMarketData(strCode);
//			MarketTRSend_By_OCXSignal(strCode, nID);
//		}
//		break;
	case UMSG_UPDATEREVISED:
		{
			BOOL bUseRevised = wParam;
			BOOL bDontRequestData = lParam;
			//=================================================
			//Input Bar / ExInput Bar에 체크컨트롤, 팝업메뉴에 체크 표시 Update
			//if(m_pInputBarDlg)
			//	m_pInputBarDlg->SendMessage(USMG_NOTIFY_REVISED, bUseRevised);

			//if(m_pExtInputBarDlg)
			//	m_pExtInputBarDlg->SendMessage(USMG_NOTIFY_REVISED, bUseRevised);
			//==================================================

			UpdateRevisedValue(bUseRevised,bDontRequestData);
		}
		break;

	case USMG_NOTIFY_REVISED:
		{
			BOOL bUseRevised = wParam;
			if(m_pInputBarDlg)
				m_pInputBarDlg->SendMessage(USMG_NOTIFY_REVISED, bUseRevised);
		}
		break;
	case UMSG_MULTICODES:
		{
			// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)
			if(wParam == 2)
			{
				m_nOldMode = m_nMode;
				m_nMode = ADD_NEW_CHART;
				m_bAddModeFromTab = FALSE;
			}
			else
			{
				m_bAddModeFromTab = (BOOL)wParam;
			}

			if(!GetAddGraphMode())
				SetChartSelected(CPoint(0,0));
			SetMultiCodes((LPCTSTR)lParam);
		}
		break;
	case UMSG_SHOWENVIRDLG:
		{
			if(!GetpChartItemSelected()) return FALSE;
				GetpChartItemSelected()->RunEnvironmentDlg(1);


			//GetpChartItemSelected()->ShowEnvironmentDlg(); //기존내용.
		}
		break;
	case UMSG_GETINC:
		{
			return m_nMode;
		}
		break;
	case UMSG_RATEVALUE:
		{
			//BOOL bUseRateValue = wParam;
			//BOOL bDontRequestData = lParam;
			//UpdateRateValue(bUseRateValue,bDontRequestData);

			int  nNumericChartValue = wParam;
			BOOL bDontRequestData = lParam;
			UpdateNumericChartValue(nNumericChartValue, bDontRequestData);
		}
		break;
	case UMSG_SETTIMESHAREUSEAGE:
		{
			SetTimeShareAll(wParam);
		}
		break;
	case UMSG_SETUPPERLOWPRICEMARKUSEAGE:
		{
			SetEnableBoundMarkAll(wParam);
		}
		break;
	case UMSG_SETSHOWLOCK:
		{
			BOOL bShowLock = wParam;
			EACH_CHARTJOB_START()
				if(pChartItem->GetType() == DAY_DATA_CHART)
				{
					pChartItem->ShowLock(bShowLock);
				}
				else
				{
					pChartItem->ShowLock(FALSE);
				}
			EACH_CHARTJOB_END()			
		}
		break;
//@유진삭제
//	case UMSG_SENDJCODECHECKED:
//		{
//			LPCTSTR lpCode = (LPCTSTR)lParam;
//			CString strCode;
//			strCode.Format("%s ", lpCode);
//			BOOL bCheck = wParam;
//			EACH_CHARTJOB_START()
//				if(pChartItem->GetDataCode()==strCode)
//				{
//					pChartItem->SetCheck(bCheck);
//				}
//			EACH_CHARTJOB_END()						
//		}
//		break;
//	case UMSG_GETJCODECHECKEDFROMCHART:// Check하는 종목코드를 가져온다.
//		{
//			CStringList *plistStrJongmokCodeChecked = (CStringList *)wParam;
//			CStringList *plistStrJongmokCodeHighlighted = (CStringList *)lParam;;
//			CString strJongmokCode;
//			EACH_CHARTJOB_START()
//				if(pChartItem->GetDataTRCode()==QUERY_STRFOFOREIGN)
//				{
//					strJongmokCode = pChartItem->GetDataCode();
//					if(strJongmokCode.GetLength()&&!plistStrJongmokCodeHighlighted->Find(strJongmokCode))
//					{
//						plistStrJongmokCodeHighlighted->AddTail(strJongmokCode);
//						if(pChartItem->GetCheck()&&!plistStrJongmokCodeChecked->Find(strJongmokCode))
//						{
//							plistStrJongmokCodeChecked->AddTail(strJongmokCode);
//						}
//					}
//				}
//			EACH_CHARTJOB_END()
//		}
//		break;
//@유진삭제
	case UMSG_GETJONGMOKCODE:
		{	
			if(!GetpChartItemSelected()) return FALSE;
			if(GetpChartItemSelected()->GetDataTRCode()==QUERY_STRFOFOREIGN)
			{
				CString *pStr = (CString *)wParam;
				pStr->Format("%s",GetpChartItemSelected()->GetDataCode());				
			}
		}
		break;
	case UMSG_GETCNTOFCHART:
		{
			return GetCntOfChart();
		}
		break;
	// (2008/11/2 - Seung-Won, Bae) Set Minute data for Normalized Time or Gap Revision.
	case UMSG_SET_NORMALIZEDMINUTETIMERULE_AND_GAPREVISION:
		{
			BOOL bImaginaryCandle	= wParam;	// 0. 실봉  1. 실봉 + 허봉
			BOOL bUseGapRevision	= lParam;	// 갭보정
			GetpChartItemSelected()->SetImaginaryCandleAndGapRevision( bImaginaryCandle, bUseGapRevision);
		}
		break;
	case UMSG_CHARTODRTRACE:
		{
			if(!GetpChartItemSelected()) return FALSE;
			long lFlag = (long)wParam;
			{
//				GetpChartItemSelected()->LoadAddIn("OrderTrace.dll");
//				GetpChartItemSelected()->InvokeAddIn("OrderTrace.dll",0, ((CStdDialog*)GetParent())->m_lDBMgr);
			}
		}
		break;
	case UMSG_ANALYSISPATTERN:
		{
			CChartItem* pSelChartItem = GetpChartItemSelected();
			if(!pSelChartItem) return FALSE;
			long lFlag = (long)wParam;
			pSelChartItem->InvokeAddInPattern("ChartPatternAddin.dll",NULL,lFlag);
		}
		break;
	case UMSG_APPLY_ANALYZE_PERIOD_ADDIN: //2005.12.02 미완
		{
			if(!GetpChartItemSelected()) return FALSE;			
			

			//GetpChartItemSelected()->InvokeAddIn("AnalyzePeriodAddin.dll",NULL, (long)m_pDBMgr);
			//GetParent()->SendMessage(RMS_ANALYSIS_FUNC, 1001, 1);

		}
		break;
	case UMSG_CHARTGUIDELINE:
		{
			//if(m_nMarketType != STOCK_CHART)	
			//{
			//	AfxMessageBox("주식 차트에서만 적용 가능합니다");
			//	return 0;
			//}
			CChartItem* pSelChartItem = NULL;
			pSelChartItem = GetpChartItemSelected();
			if(!pSelChartItem) return FALSE;


			if( pSelChartItem->GetChart_NumericValueType() != WON_TYPE)
			{
				AfxMessageBox("원 단위 일때만 기준선이 적용 가능합니다");
				return FALSE;
			}

//@Solomon:091123SM068
			//pSelChartItem->InvokeAddIn("ChartGuideLineAddin.dll:GUIDE_LINE",101,(long)pSelChartItem->GetSafeHwnd());

			
			CStringList strlistSetAll;
			strlistSetAll.AddTail(((CStdDialog*)GetParent())->GetAccountInfo());
			pSelChartItem->InvokeAddIn("ChartGuideLineAddin.dll:GUIDE_LINE",NULL,(long)&strlistSetAll);
			/*		//@Solomon:091123SM068
			if(strlistSetAll.GetCount())
			{
				CString strbChanged = strlistSetAll.GetHead();
				if(strbChanged=="1")
				{
					RequestAVGForGuideLine(FALSE);
				}
			}
			*/
		}
		break;
	case UMSG_PERIOD_SUMMARY:
		{
			CChartItem* pSelChartItem = GetpChartItemSelected();
			if(!pSelChartItem) return FALSE;

			pSelChartItem->InvokeAddInPeriod("ChartPeriodSummaryAddIn.dll:PERIOD_SUMMARY",lParam,0);
		}
		break;
	case UMSG_SETDRAGMODETYPE:
		{
			BOOL bType = wParam;
			EACH_CHARTJOB_START()
				pChartItem->SetDragModeType(bType);
			EACH_CHARTJOB_END()				
		}
		break;
	case UMSG_SETORDEREDINFO:
		{
			EACH_CHARTJOB_START()
				pChartItem->SendMessageToInvoke(INVOKEMSG_SHOWORDEREDINFO,NULL,NULL);
			EACH_CHARTJOB_END()			
		}
		break;		
	case UMSG_SEND2SERVERFROMINVOKE:
		{
			stSend2Server *pSt = (stSend2Server *)lParam;
			if(pSt->m_nType>0)
			{
				GetParent()->SendMessage(UMSG_SEND2SERVER,wParam,lParam);			
			}
			else
			{
				MessageBox("SEND2SERVERFROMINVOKE Error!");
			}			
		}
		break;
	case UMSG_GETCODEFROMINVOKE:
		{
			CChartItem *pChartItem = (CChartItem *)wParam;
			CString* pStrCode = (CString*)lParam;
			if(pChartItem->GetSafeHwnd())
			{
				pStrCode->Format("%s",pChartItem->GetDataCode());
			}
		}
		break;
	case UMSG_GETMAINHANDLEFROMINVOKE:
		{
		//	CWnd** ppMainFrame = (CWnd**)wParam;
		//	*ppMainFrame = ((IMainDllObMain*)g_pMainDll)->m_pMainFrame;
		}
		break;
	case UMSG_GET_TICKMINDAY_VALUE:
		{
			int nType = lParam;			
			sTemp = GetpChartItemSelected()->GetTickMinDayNumber(nType > -1 ? nType : -1);

			//2005. 06. 24 ... 달력 컨트롤 양옆의 버튼들이 반응하도록 한다 ===================
			if(m_pExtInputBarDlg->GetSafeHwnd())
				m_pExtInputBarDlg->SendMessage(message, wParam, lParam);
			//================================================================================
			return atoi(sTemp);
		}
		break;
	case UMSG_USE_LOG:
		{
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem)
			{
				long lValue = (long)wParam;				
				//pChartItem->SetAllLogScale(lValue > 0 ? TRUE : FALSE);
				pChartItem->SetLogScale("", (lValue > 0 ? TRUE : FALSE));
				pChartItem->m_CfgInfo.bShowLog = (lValue > 0 ? true : false); //2005. 08. 22 
				pChartItem->Invalidate();
			}
		}
		break;
	case UMSG_GET_TIME_FROM_DATETIMEPICKER:
		{
			long lEndDate = 0;
			if(m_pExtInputBarDlg)			
				lEndDate =m_pExtInputBarDlg->SendMessage(UMSG_GET_TIME_FROM_DATETIMEPICKER, 0, lParam);
			
			return lEndDate;
		}
		break;
	case UMSG_MOVE_CHART_VIEWRANGE:
		{
			//차트가 보고 있는 영역을 변경한다.
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem)
			{				
				pChartItem->SetStartEndDateOfView((double)wParam, (double)lParam);
			}
		}
		break;
	case UMSG_DISCONNECT_ALLREAL: //차트 Real 해제.
		{
			OnAllCodeUnAdvise();
		}
		break;
	case UMSG_SETLOG: //05.08.23
		{
			if(m_pInputBarDlg)
				m_pInputBarDlg->SendMessage(message, wParam, lParam);
		}
		break;
	case UMS_SETCHART_DATEINFO:
		{
			//차트의 날짜 정보를 변경한다
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem)
			{
				CString sDate;
				sDate.Format("%d", lParam); //날짜 정보
				
				if(wParam == 1) // 종료일 지정이면 1, 시작일 지정이면 0;
				{
//					pChartItem->SetEndDateRequested(sDate);
					pChartItem->SetEndDate(sDate);
					pChartItem->SetStartDateFlag(FALSE);
				}
				else if(wParam == 0)
				{					
					pChartItem->SetStartDate(sDate)  ;
					pChartItem->SetStartDateFlag(TRUE);
				}
			}
		}
		break;
	case USMG_PLEASE_RESIZE:
		{		
			if(m_pFloatingDlg != NULL)
			{
				BOOL bCanRequest = wParam ? FALSE : TRUE;
				if(bCanRequest) // 확장된 상태이면
				{
					if(m_pInputBarDlg)
					{
						CString strCode = GetpChartItemSelected()->GetDataCode();
						// (2008/12/27 - Seung-Won, Bae) Do not use continually item code.
						strCode = CStdDialog::GetRealItemCode( strCode, TRUE);
// --> [Edit]  강지원 2008/10/23
//						if(strCode.GetLength() != CODE_STOCK_LENGTH)
						if(strCode.GetLength() < SIMBOL_CODE_MIN_LENGTH)
						{
							m_pFloatingDlg->m_bIconState = 1;
							return 1L;
						}
// <-- [Edit]  강지원 2008/10/23
					}
				}

				m_pRectFloatingHoga = NULL;
				m_pRectFloatingHoga = (CRect*)lParam;			
				if(m_pRectFloatingHoga)
				{
					ScreenToClient(m_pRectFloatingHoga);					
					m_pFloatingDlg->MoveWindow(m_pRectFloatingHoga, TRUE);
				}

				//=====================================================================================
				// 호가 윈도우 갱신 및 이동.
				//=====================================================================================
				MoveFloatingHogaDlg();
				//=====================================================================================

				if(bCanRequest) // 확장된 상태이면
				{
					//호가를 요청 한다.
					((CStdDialog*)GetParent())->Request_StockHogaData();
				}
			}
		}
		break;
	case UMSG_SIGNAL_CLICK:
		{
//			OutputDebugString("click");
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem)
			{
				pChartItem->OpenSignalMapScr();
			}
		}
		break;
	case UMSG_STOP_APPLY_EXPTOOOL:
		{
			EACH_CHARTJOB_START()
				if(GetpChartItemSelected() != pChartItem)
				{
					pChartItem->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::T_TOOL_SELECTION, 1);
					pChartItem->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::T_TOOL_SELECTION, 1);
				}
			EACH_CHARTJOB_END()
		}
		break;
	case UMSG_FIREGSHAREDATA:
		{
			CString strCodeShare = (LPCTSTR)wParam;			
			/* solomon
			if(strCodeShare=="시간_차트")
			{

				CString strTimeInfo = (LPCTSTR)lParam;
				SetTimeMark((LPCTSTR)strTimeInfo,TRUE);
			}
			*/
//			else
//			{
//				STSHAREDDATA stData;
//				stData.m_szKey = (LPCTSTR)strCodeHead;
//				stData.m_szValue = (LPCTSTR)strCodeTail;
//				if(m_pLeftBarDlg)
//				{
//					m_pLeftBarDlg->SendMessage(UMSG_SETCHARTHOGA,NULL,(LPARAM)&stData);
//				}	
//			}	
		}
		break;
	case UMSG_ADDSYSTEMTRADINGINDEX:
		{
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem)
			{
				pChartItem->SetSystemTradingIndexByTab((CStringArray*)wParam);
			}
		}
		break;
	case UMSG_CLEAR_MAREKT_TR:
		{
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem)
			{
				pChartItem->RemoveAllMarketData();
			}
		}
		break;	
	//11.17일 추가 =======================================================
	//차트 저장시 사용자가 직접 입력한 봉 갯수를 가지고 저장하기 위해 추가했다.
	case UMSG_GETZOOMVALUE:
		{
			if(m_pInputBarDlg && ::IsWindow(m_pInputBarDlg->GetSafeHwnd()))
			{
				return m_pInputBarDlg->SendMessage(UMSG_GETZOOMVALUE, wParam, lParam);
			}
		}
		break;
	case UMSG_EXT_TOOL_RESTORE:
		{
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem)
			{
				if( pChartItem->GetDataCode().GetLength() )
				{	
					//pChartItem->InitChartItem_FromExtToolBar();
					return 1;
				}

				return 0;
			}
		}
		break;
	case UMSG_CHART_PRINT:
		{
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem)
				pChartItem->PrintChart();
		}
		break;
	case UMSG_CHART_EXCEL:
		{
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem)
				pChartItem->SetExcelData();
		}
		break;
	case UMSG_SAVEJIPYOONEFILE:
		{
			// 분석툴 개별 저장 - ojtaso (20080723)
			if(lParam == 100)
				m_bSaveAnalTool = wParam;
			else
			{
				m_bSaveJipyoOneFile = wParam;
				m_pTickCfgChart		= NULL;
				m_pSecCfgChart		= NULL;
				m_pMinCfgChart		= NULL;
				m_pDayCfgChart		= NULL;
				m_pHourCfgChart		= NULL;
				m_pWeekCfgChart		= NULL;
				m_pMonthCfgChart	= NULL;
				m_pYearCfgChart		= NULL;
//				m_pHogaCfgChart		= NULL;
			}
		}
		break;

	// 2011.01.31 by SYS >> 분차트 YYYY(년) 추가 및 분석툴 공유 기능 추가
	case UMSG_SET_ALALTOOL_SHAREFILE:
		{
			m_bShareAnalTool = wParam;
		}
		break;

	case UMSG_ALALTOOL_FILE_CHANGE:
		{
			CChartItem* pChartItem = GetpChartItemSelected();
			if (!pChartItem) 
				return FALSE;

			// 현재 추세선저장/공유 선택에 따라 추세선 다시 그림..
			CString strAnalFile = _T("");
			if (lParam == 1)
			{
				// 추세선 파일 저장...
				strAnalFile = pChartItem->GetOpenAndSaveAnalFileName(FALSE, TRUE);
				pChartItem->SaveandOpenFromAnalToolFile("", strAnalFile, FALSE, TRUE);

			}
			else
			{
				// 추세선 파일 읽음...
				strAnalFile = pChartItem->GetOpenAndSaveAnalFileName(FALSE, FALSE);
				pChartItem->SaveandOpenFromAnalToolFile(strAnalFile, "", TRUE, FALSE);
			}
		}
		break;
	// 2011.01.31 by SYS 

	case UMSG_DWMM_CHART:
		{
			LoadDWMMChart(lParam);
		}
		break;
	case UMSG_REQUEST_FORQUERYOPT:
		{
			if(!GetpChartItemSelected()) return FALSE;
			CCodeDataFromInputBar CodeData;
			CodeData.m_strCode = GetpChartItemSelected()->GetDataCode();
			CodeData.m_lEndTime = atol(GetpChartItemSelected()->GetEndDateRequested());
			InputCodeFromInputBar(&CodeData,TRUE);				
		}
		break;
	case UMSG_GETQUERYOPT:
		{
			return ((CExtInputBarDlg*)GetpExtInputBarDlg())->GetQueryOpt();
		}
		break;
//@유진삭제
//	case UDM_ADD_PLATFORM_MENU:
//		{
//			HMENU hMenu = (HMENU)lParam;
//			AddMainMenuToPlatformMenu(hMenu);
//			if(GetDlgToolBtnToggle(m_arrayAnalIdx, m_arrayAdditionIdx, m_arrayUserIdx, m_mapToolName))
//				MakePlatformMenu(hMenu);
//			break;
//		}
//	case UDM_CHECK_PLATFORM_MENU:
//		{
///*
//			CStringArray arTest;
//			CString strItem;
//			for(int nIndx = 0;nIndx < 10; ++nIndx)
//			{
//				strItem.Format("%d\tㅁㅁㅁㅁㅁ", nIndx);
//				arTest.Add(strItem);
//			}
//			
//			int nMenuID = (int)wParam - ID_PLM_ADD_MENU;
//			strItem = arTest.GetAt(nMenuID);
//			AfxMessageBox(strItem);
//*/
//			break;
//		}
//@유진삭제
	//삼성
	//case UMSG_NEWS:
	//	{
	//		CChartItem* pSelChartItem = GetpChartItemSelected();
	//		if(!pSelChartItem) return FALSE;
	//		CString strCode = pSelChartItem->GetDataCode();
	//		if(strCode.GetLength() == CODE_STOCK_LENGTH)
	//			pSelChartItem->InvokeAddInNews("ChartPeriodSummaryAddIn.dll:NEWS",lParam,(LPARAM)atoi(strCode));
	//	}
	//	break;
//@유진삭제
//	case UDM_GET_USERID:
//		return (long)(LPCSTR)m_strUserPath;
//		break;
//@유진삭제
// --> [Edit]  이도영 2008/09/13
// 
	case UMSG_SETORDERLINK:
		{
			m_bOrderLink = wParam;
			// 차트 주문 연동 - onlyojt (20091204)
			EACH_CHARTJOB_START()
				if (m_bOrderLink)
				{
					if( pChartItem->GetTypeRequested() == MIN_DATA_CHART && atoi(pChartItem->GetMinNumber()) <= 60 )
						pChartItem->InvokeAddInStr("ChartOutsideCustomAddIn.dll:ORDER_LINE", "2", "ON");
					else
						pChartItem->InvokeAddInStr("ChartOutsideCustomAddIn.dll:ORDER_LINE", "2", "OFF");
				}
				else
				{
					pChartItem->InvokeAddInStr("ChartOutsideCustomAddIn.dll:ORDER_LINE", "2", "OFF");
				}
				pChartItem->Invalidate();
			EACH_CHARTJOB_END()
		}
		break;
// <-- [Edit]  이도영 2008/09/13

	//20080914 이문수 >>
	case UMSG_CHART_CONTAINER_PRINT:
		{
			PrintChartContainer();			
		}
		break;
	case UMSG_CHART_IMG_SAVE:
		{
			SaveChartContainerToImg();			
		}
		break;
	//20080914 이문수 <<
	case UMSG_CHART_MIGYULCHE:
		{
			/* solomon
			((CStdDialog*)GetParent())->m_bMigyulcheDraw = wParam;
			EACH_CHARTJOB_START()
				if (((CStdDialog*)GetParent())->m_bMigyulcheDraw)
					pChartItem->InvokeAddInStr("ChartKoreaSorimachiAddIn.dll:ASKED_PRICE_CKSA", "3", "1");
				else
					pChartItem->InvokeAddInStr("ChartKoreaSorimachiAddIn.dll:ASKED_PRICE_CKSA", "3", "0");
				pChartItem->Invalidate();
			EACH_CHARTJOB_END()
			*/
		}
		break;
	case UMSG_CHART_INQUIRY_SHOW:		//	수치조회창 Show/Hide
		{
			if ( (int)wParam == SW_SHOW )
			{
				if ( !m_pInquiryDlg )
				{
					AFX_MANAGE_STATE(AfxGetStaticModuleState());
					//HINSTANCE hInstResourceClient = AfxGetResourceHandle();
					//AfxSetResourceHandle(g_hInstance);
					m_pInquiryDlg	= new CInquiryDlg(this);			//	수치조회창
					m_pInquiryDlg->Create(CInquiryDlg::IDD, this);
					//AfxSetResourceHandle(hInstResourceClient);
				}
			}
			else if ( (int)wParam == SW_HIDE )
			{
				if ( !m_pInquiryDlg )
				{
					m_pInquiryDlg->DestroyWindow();
					delete m_pInquiryDlg;
					m_pInquiryDlg = NULL;
				}
			}

			m_pInquiryDlg->SetShowHide((int)wParam);
		}
		break;
// --> [Edit]  강지원 2008/11/06
	case  UMSG_CHART_INQUIRY_DELETE:
		{
			if(m_pInquiryDlg)
			{
				m_pInquiryDlg->DestroyWindow();
				delete m_pInquiryDlg;
				m_pInquiryDlg = NULL;
			}
			//	부모에게 ToolBarDlg의 상태를 돌리도록 한다. 
			GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, wParam, lParam);
		}
		break;
// <-- [Edit]  강지원 2008/11/06
	}
	

	return CWnd::WindowProc(message, wParam, lParam);
}

//20080914 이문수 >>
// win.ini에 쓰기 위한 루틴
BOOL CChartMng::SetDefaultOption(CString szPaperSize, CString szWay, CString szOrientation)
{
	CString szOption;

	WriteProfileString(TEXT("OPTION_PRINTER"),TEXT("PaperSize"),(char *)(LPCTSTR)szPaperSize );
	WriteProfileString(TEXT("OPTION_PRINTER"),TEXT("PaperWay"),(char *)(LPCTSTR)szWay );
	WriteProfileString(TEXT("OPTION_PRINTER"),TEXT("PaperOrientation"),(char *)(LPCTSTR)szOrientation );

	return TRUE;
}

// win.ini에서 읽기 위한 루틴
BOOL CChartMng::GetDefaultOption(CString& szPaperSize, CString& szWay, CString& szOrientation)
{
	CString szOption;
	char size[256], way[256], orientation[256];

	GetProfileString(TEXT("OPTION_PRINTER"),TEXT("PaperSize"),_T(""),size, 256);
	GetProfileString(TEXT("OPTION_PRINTER"),TEXT("PaperWay"),_T(""),way, 256);
	GetProfileString(TEXT("OPTION_PRINTER"),TEXT("PaperOrientation"),_T(""),orientation, 256);

	szPaperSize = size;
	szWay = way;
	szOrientation = orientation;

	return TRUE;
}

BOOL CChartMng::SetDefaultPrinter(CString printer, CString driver, CString port)
{	
	CString aab;
	aab.Format("%s,winspool,%s",printer,port);
	char szName[2048];
	memset(szName,0,sizeof(szName));
	
	GetProfileString(TEXT("Windows"),TEXT("Device"),_T(",,,"),szName, sizeof(szName));
	WriteProfileString(TEXT("Windows"),TEXT("Device"),(char *)(LPCTSTR)aab );
	
	char pname[50];
	char *ppname;
	memset(&pname,0,sizeof(pname));
	strcpy(pname,(char *)(LPCTSTR)aab);
	
	ppname = pname;
	
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	
	// WIN 95
	if(osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		::SendMessage(HWND_BROADCAST,WM_WININICHANGE,0,(LPARAM)ppname);
	
	// WIN2000
	else if(osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
		::SendMessage(HWND_BROADCAST,WM_SETTINGCHANGE,0,(LPARAM)ppname);
	// Win2000으로 보시면 됩니다.
	
	//OSVERSIONINFO 참고
	
	return true;
}

void CChartMng::SaveChartContainerToImg()
{
	BOOL bExist = FALSE;
	int nReturn = 0;
	CString strMsg = "";
	CString strFileName, strFileType;
	CString strDefaultName = "TEMP";
	CFileFind FileFinder;

	char szFilter[] = "BMP Files(*.BMP)|*.BMP|Jpeg Files(*.JPG)|*.JPG||";
	CFileDialog saveDlg(FALSE, _T("BMP"), strDefaultName, NULL, szFilter, NULL);
	
	{
		if (saveDlg.DoModal() == IDOK)
		{

			/////////////////////////////////////////
			CWinPrint winPrint;
			winPrint.m_strName = "";

			CRect rectClient;
			GetClientRect(rectClient);
			CDC * pDC = GetDC();
			CDC MemDC;
			CBitmap bmp, * pOldBmp;
			MemDC.CreateCompatibleDC(pDC);
			bmp.CreateCompatibleBitmap(pDC, rectClient.Width(), rectClient.Height());
			pOldBmp = MemDC.SelectObject(&bmp);
			ReleaseDC(pDC);

			Paint(&MemDC, TRUE);

			MemDC.SelectObject(pOldBmp);
			HBITMAP hBmp = (HBITMAP)bmp;

			HDIB hDIB = winPrint.CopyBmpToDIB(hBmp);
			if(!hDIB)
			{
				MessageBox("저장에 실패했습니다.");
				return;
			}

			HDIB hFileDIB = winPrint.AddBMPFileHeader(hDIB);
			if(!hFileDIB)
			{
				GlobalFree(hDIB);
				MessageBox("저장에 실패했습니다.");
				return;
			}
			/////////////////////////////////////////
			strFileType = saveDlg.GetFileExt();
			strFileName = saveDlg.GetPathName();
			BOOL bResult= FALSE;
			bExist = FileFinder.FindFile(strFileName);
			if (bExist == TRUE)
			{
				strMsg.Format("%s 은(는) 이미 있습니다.\n덮어쓰시겠습니까?", strFileName);
				nReturn = MessageBox(strMsg, "", MB_YESNO | MB_ICONQUESTION);
				if (nReturn == IDYES)
					winPrint.SaveImg(strFileName, hFileDIB);					
			}
			else
			{
				if(!strFileName.IsEmpty())
					winPrint.SaveImg(strFileName, hFileDIB);
			}
			
			if(hDIB)
			{
				GlobalFree(hDIB);
				hDIB = NULL;
			}

			if(hFileDIB)
			{
				GlobalFree(hFileDIB);
				hFileDIB = NULL;
			}
		}
	}
}

void CChartMng::PrintChartContainer()
{
	LPDEVMODE lpDevMode = NULL;
	LPDEVNAMES lpDevNames = NULL;
	HDC hDC = NULL;
	HDIB hDIB = NULL;

	//////////////////////////////////////////////////////////////////
	// win.ini에서 설정된 값을 읽어온다.
	CString szSize, szWay, szOrientaion;
	GetDefaultOption(szSize, szWay, szOrientaion);
	int nSize = 1, nWay = 7, nOrientaion = 1;	// letter, auto, portrait

	if( szSize ) 		
		nSize = atoi(szSize);

	if( szWay )		
		nWay = atoi(szWay);
	
	if( szOrientaion)	
		nOrientaion = atoi(szOrientaion);
	
	// GetDevMode를 하지 않으면 Mode를 변경하더라도 적용되지 않는다.
	tagPDA pPrintDlg;
	AfxGetApp()->GetPrinterDeviceDefaults (&pPrintDlg);
	CPrintInfo* pInfo = new CPrintInfo;
	pInfo->m_pPD->m_pd.hDevMode = pPrintDlg.hDevMode;
	LPDEVMODE devMode = pInfo->m_pPD->GetDevMode();
	if (devMode)
	{
		devMode->dmPaperSize = nSize;
		devMode->dmDefaultSource = nWay;
		devMode->dmOrientation = nOrientaion;
		GlobalUnlock (devMode);
	}

	// 프린터를 수정하지 않으면 SetDefaultPrinter를 하지 않도록 하기 위해서
	// 임시로 저장하기 위한 변수
	LPDEVNAMES temDevNames = (LPDEVNAMES)GlobalLock(pPrintDlg.hDevNames);
	CString temDevice = (LPCTSTR)temDevNames + temDevNames->wDeviceOffset;
	CString tszDevice;
	tszDevice.Format("%s", temDevice);

	GlobalUnlock(temDevNames);
	
	/////////////////////////////////////////////////////////////////

	BOOL bTPortrait = TRUE;

	CWinPrint winPrint;
	winPrint.m_strName = "";

	CRect rectClient;
	GetClientRect(rectClient);
	CDC * pDC = GetDC();
	CDC MemDC;
	CBitmap bmp, * pOldBmp;
	MemDC.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, rectClient.Width(), rectClient.Height());
	pOldBmp = MemDC.SelectObject(&bmp);
	ReleaseDC(pDC);

	Paint(&MemDC, TRUE);

	MemDC.SelectObject(pOldBmp);
	HBITMAP hBmp = (HBITMAP)bmp;

	hDIB = winPrint.CopyBmpToDIB(hBmp);
	
	DWORD dwFlags = PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE | PD_NOSELECTION;
	CPrintDialog dlg(TRUE, dwFlags);


	if(devMode)
		dlg.m_pd.hDevMode		= devMode;
	if(pInfo)
		delete pInfo;
	
	if(dlg.DoModal()==IDOK)
	{
		lpDevMode = (LPDEVMODE)GlobalLock(dlg.m_pd.hDevMode);
		lpDevNames = (LPDEVNAMES)GlobalLock(dlg.m_pd.hDevNames);

		if( lpDevMode->dmOrientation  == DMORIENT_PORTRAIT)			bTPortrait = TRUE;
		if( lpDevMode->dmOrientation  == DMORIENT_LANDSCAPE)		bTPortrait = FALSE;

		//프린터 dc를 구하는 데 여러가지 방법을 모색하여 쓰도록
		CString szDevice = (LPCTSTR)lpDevNames + lpDevNames->wDeviceOffset;
		CString szDriver = (LPCTSTR)lpDevNames + lpDevNames->wDriverOffset;
		CString szOutput = (LPCTSTR)lpDevNames + lpDevNames->wOutputOffset;
		// 순서 주의 : driver, device, port(output)순서이다.
		hDC = ::CreateDC(szDriver, szDevice, szOutput, lpDevMode);

		///////////////////////////////////////////////////////////////
		//프린트 시작
		winPrint.fnPrintWindow(200, GetSafeHwnd(), hDC, "", bTPortrait, hDIB);
		if(hDC)
			DeleteDC(hDC);

		GlobalUnlock(lpDevMode);
		GlobalUnlock(lpDevNames);
		
		if(szDevice.Compare(tszDevice))	// 프린터를 변경하지 않으면 수정하지 않도록
			SetDefaultPrinter(szDevice, szDriver, szOutput);

		// win.ini에 쓰기 위해서 
		CString szPaperSize;
		CString szWay;
		CString szOrientation;
		szPaperSize.Format("%d", lpDevMode->dmPaperSize);
		szWay.Format("%d", lpDevMode->dmDefaultSource);
		szOrientation.Format("%d",lpDevMode->dmOrientation);

		SetDefaultOption(szPaperSize, szWay, szOrientation);
		////////////////////////////////////////////////////////////


	}
	
	if(hDIB)
	{
		GlobalFree(hDIB);
		hDIB = NULL;
	}
}
//20080914 이문수 <<

void CChartMng::GetJongmokCodes(CStringList *plistStrJongmokCode, CHighlightChartInfoList* plistHighlightChartInfo)
{
	stHighlightChartInfo stChartInfo;
	CString strJongmokCode;
	EACH_CHARTJOB_START()
		if(pChartItem->GetDataTRCode()==QUERY_STRFOFOREIGN)
		{
			strJongmokCode = pChartItem->GetDataCode();
			if(strJongmokCode.GetLength()&&!plistStrJongmokCode->Find(strJongmokCode))
			{
				plistStrJongmokCode->AddTail(strJongmokCode);
				stChartInfo.m_strJongmokCode = strJongmokCode;
				stChartInfo.m_pChartItem = pChartItem;				
				plistHighlightChartInfo->AddTail(stChartInfo);
			}
		}
	EACH_CHARTJOB_END()
}

void CChartMng::SetTimeShareAll(BOOL bUseage)
{
	EACH_CHARTJOB_START()
		pChartItem->EnableReceivingTimeShare(bUseage);
	EACH_CHARTJOB_END()
}

void CChartMng::SetEnableBoundMarkAll(BOOL bUseage)
{
	EACH_CHARTJOB_START()
		pChartItem->EnableBoundMark(bUseage);
	EACH_CHARTJOB_END()
}

BOOL CChartMng::UpdateIndex()
{
	 //호가 요청을 못하도록 한다.
	if(m_pInputBarDlg)
		m_pInputBarDlg->SendMessage(UMSG_ENABLE_SENDHOGATR, FALSE, 0);


	if(!GetpChartItemSelected()) return FALSE;

	if(GetpChartItemSelected()->GetPrimaryChartItemData() == NULL) return FALSE;
	

	InitInLoop();
	m_strArrayCodeInLoop.RemoveAll();
	m_strArrayTRCodeInLoop.RemoveAll();
	


	m_pChartItemSelectedInLoop = GetpChartItemSelected();
	m_lCntInLoop = m_pChartItemSelectedInLoop->GetCntOfItemData();
	m_chTypeInLoop = m_pChartItemSelectedInLoop->GetType();

	//m_bUseRateValueInLoop = m_pChartItemSelectedInLoop->GetUseRateValue();
	m_nChartMngs_ChartDataNumericValue = m_pChartItemSelectedInLoop->GetChart_NumericValueType();

// (2008/11/2 - Seung-Won, Bae) Do not support sync for all ChartItem.
//	m_bNormalizedMinuteTimeRuleInLoop = m_pChartItemSelectedInLoop->GetBNormalizedMinuteTimeRule();

	for(long lPos = 1;lPos<m_lCntInLoop;lPos++)
	{
		m_strArrayCodeInLoop.Add(m_pChartItemSelectedInLoop->GetDataCode(lPos));
		m_strArrayTRCodeInLoop.Add(m_pChartItemSelectedInLoop->GetDataTRCode(lPos));
	}			
	m_pChartItemSelectedInLoop->SaveandOpenFromFile("","temp.tmp",FALSE,TRUE);
	m_lTypeInLoop = LOOPTYPE_UPDATE/*4*/;//update
	HeadInLoop();

	//호가 요청을 하도록 한다.
	if(m_pInputBarDlg)
		m_pInputBarDlg->SendMessage(UMSG_ENABLE_SENDHOGATR, TRUE, 0);
	

//	CChartItem *pChartItemSelected = GetpChartItemSelected();
//	CStringArray strArrayCode;
//	CStringArray strArrayTRCode;
//	long lCnt = pChartItemSelected->GetCntOfItemData();
//	for(long lPos = 1;lPos<lCnt;lPos++)
//	{
//		strArrayCode.Add(pChartItemSelected->GetDataCode(lPos));
//		strArrayTRCode.Add(pChartItemSelected->GetDataTRCode(lPos));
//	}			
//	pChartItemSelected->SaveandOpenFromFile("","temp.tmp",FALSE,TRUE);
//	long lColSelected = m_lColSelected;
//	long lRowSelected = m_lRowSelected;
//	EACH_CHARTJOB_START()
//		SetChartSelected(CPoint(nColIndex,nRowIndex));
//		if(!pChartItem->GetAllCnt()) continue;
//		if(pChartItem==pChartItemSelected) continue;
//		pChartItem->ClearDataExceptPrimaryData();
//		for(lPos = 0;lPos<(lCnt-1);lPos++)
//		{
//			pChartItem->SetCodeForQuery(strArrayCode.GetAt(lPos),atoi(strArrayTRCode.GetAt(lPos)),FALSE);
//		}			
//		RequestChartData();
//		pChartItem->SaveandOpenFromFile("temp.tmp","",TRUE,FALSE);
//	EACH_CHARTJOB_END()
//	SetChartSelected(CPoint(lColSelected,lRowSelected));

	
	return TRUE;
}

BOOL CChartMng::UpdateShowingFacked_JustOneChart(BOOL bNormalizedMinuteTimeRule,BOOL bDontRequestData)
{
	if(!GetpChartItemSelected()) return FALSE;

	GetpChartItemSelected()->m_CfgInfo.bImaginaryCandle = bNormalizedMinuteTimeRule ? true : false;
	GetpChartItemSelected()->SetBNormalizedMinuteTimeRule(bNormalizedMinuteTimeRule);
	if(!bDontRequestData) 
		return RequestChartData();	

	return TRUE;
}

// (2008/11/2 - Seung-Won, Bae) Do not support sync for all ChartItem.
/*
BOOL CChartMng::UpdateShowingFackedCandle(BOOL bShowFackedCandle, BOOL bDontRequestData)
{
	m_bUpdatingShowingFackedCandle = TRUE;
	if(m_bChartExtended)
	{
		if(!GetpChartItemSelected()) return FALSE;
		GetpChartItemSelected()->SetBNormalizedMinuteTimeRule(bNormalizedMinuteTimeRule);
		if(!bDontRequestData) RequestChartData();
		m_bUpdatedInChartExtended = TRUE;
	}
	else
	{
		InitInLoop();
		m_bNormalizedMinuteTimeRuleInLoop = bNormalizedMinuteTimeRule;
		m_bDontRequestDataInLoop = bDontRequestData;
		m_lTypeInLoop = LOOPTYPE_FAKE_CANDLE; // ShowFackecCandle
		HeadInLoop();
	}
	m_bUpdatingShowingFackedCandle = FALSE;
	return TRUE;
}
*/

BOOL CChartMng::UpdateRevisedValue(BOOL bUseRevised,BOOL bDontRequestData)
{
	m_bUpdatingRevisedValue = TRUE;

	//2005. 08. 22 수정 주가 선택된 차트 하나만 적용한다. ====================================
	if(!GetpChartItemSelected()) return FALSE;
	//수정주가설정 변경 없음 일 경우는 리턴
	if(GetpChartItemSelected()->GetUseRevisedValue() == bUseRevised)
		return TRUE;

	GetpChartItemSelected()->SetUseRevisedValue(bUseRevised);
	if(!bDontRequestData) RequestChartData();
	
	if(m_bChartExtended)
		m_bUpdatedInChartExtended = TRUE;	
	//========================================================================================

/*	if(m_bChartExtended)
	{
		if(!GetpChartItemSelected()) return FALSE;
		GetpChartItemSelected()->SetUseRevisedValue(bUseRevised);
		if(!bDontRequestData) RequestChartData();
		m_bUpdatedInChartExtended = TRUE;
	}
	else
	{
		InitInLoop();
		m_bUseRevisedInLoop = bUseRevised;
		m_bDontRequestDataInLoop = bDontRequestData;
		m_lTypeInLoop = LOOPTYPE_REVISED; //3 RevisedValue
		HeadInLoop();

//		long lColSelected = m_lColSelected;
//		long lRowSelected = m_lRowSelected;
//		EACH_CHARTJOB_START()
//			SetChartSelected(CPoint(nColIndex,nRowIndex));
//			GetpChartItemSelected()->SetUseRevisedValue(bUseRevised);
//			if(!bDontRequestData) RequestChartData();
//		EACH_CHARTJOB_END()
//		SetChartSelected(CPoint(lColSelected,lRowSelected));
	}
*/
	m_bUpdatingRevisedValue = FALSE;
	return TRUE;
}


//DEL BOOL CChartMng::UpdateRateValue(BOOL bUseRateValue,BOOL bDontRequestData)
//DEL {
//DEL 	if(!GetpChartItemSelected()) return FALSE;	
//DEL 	GetpChartItemSelected()->SetUseRateValue(bUseRateValue);
//DEL 	if(!bDontRequestData) RequestChartData();
//DEL 	return TRUE;
//DEL }

//05.06. 22 add by sy, nam
//지존의 토글 방식이 아닌 형태가 3개(\, $, %)가 되어 서 추가된 함수 
BOOL CChartMng::UpdateNumericChartValue(int nNumericChartValue, BOOL bDontRequestData)
{
	if(!GetpChartItemSelected()) return FALSE;	
	GetpChartItemSelected()->SetChartNumericValue(nNumericChartValue);
	// 복수종목 추가 : 복수종목 - ojtaso (20070118)
	if(!bDontRequestData) RequestChartData(TRUE);
	return TRUE;
}

BOOL CChartMng::ShowScale()
{
	if(m_bHideScale)
	{
//		double dStartDate = 0, dEndDate = 0;
//		GetpChartItemSelected()->GetStartEndDateOnView(&dStartDate,&dEndDate);
//		char chType = GetpChartItemSelected()->GetType();
		EACH_CHARTJOB_START()
			if(pChartItem->GetDataCode()!="")
			{
				pChartItem->HideScale(m_bHideScale);
//				if(pChartItem->GetType()==chType)
//				{
//					pChartItem->SetStartEndDateOfView(dStartDate,dEndDate);						
//				}
			}
		EACH_CHARTJOB_END()			
	}
	else
	{
		EACH_CHARTJOB_START()
			if(!pChartItem->GetDataCode().IsEmpty())
			{
				pChartItem->HideScale(m_bHideScale);
			}
		EACH_CHARTJOB_END()			
	}
	return TRUE;
}

BOOL CChartMng::ShowInnerToolbar()
{
	if(m_bHideInnerToolBar)
	{
		EACH_CHARTJOB_START()
			if(pChartItem->GetDataCode()!="")
			{
				pChartItem-> HideInnerToolBar(m_bHideInnerToolBar);
			}
		EACH_CHARTJOB_END()			
	}
	else
	{
		EACH_CHARTJOB_START()
			if(!pChartItem->GetDataCode().IsEmpty())
			{
				pChartItem->HideInnerToolBar(m_bHideInnerToolBar);
			}
		EACH_CHARTJOB_END()			
	}
	return TRUE;
}
/*
#define TICK_DATA_CHART		'0'
#define MIN_DATA_CHART		'1'
#define DAY_DATA_CHART		'2'
#define WEEK_DATA_CHART		'3'
#define MONTH_DATA_CHART	'4'
#define YEAR_DATA_CHART		'5'
*/

BOOL CChartMng::AddUpjongData(CString strCode)
{
	if(!GetpChartItemSelected()) return FALSE;
	if(!GetpChartItemSelected()->GetCntOfItemData()||GetpChartItemSelected()->GetOnRunningOneChart()) 
	{
		return FALSE;
	}
	// 틱(0)/분(1)/일(2)/주(3)/월(4)
	if(	  GetpChartItemSelected()->GetType() == TICK_DATA_CHART
		||GetpChartItemSelected()->GetType() == MIN_DATA_CHART
		) 
	{
		GetParent()->SendMessage(UMSG_SENDTOMSGBOX,(WPARAM)&ERRORMSG2);
		return FALSE;
	}
//==================================================================================
// long형 TRCode를 사용하지 않으므로 주석처리한다. 2005.05.06 
//==================================================================================
	if(GetpChartItemSelected()->HasChartItemData(strCode, QUERY_STRUPJONG, FALSE))
	{
		GetpChartItemSelected()->RemoveChartItemData(strCode,QUERY_STRUPJONG);
	}
	else
	{
		//2005.05.06 by sy,nam
		GetpChartItemSelected()->SetCodeForQuery(strCode, QUERY_STRUPJONG, FALSE);
		RequestChartData();
	}
//==================================================================================
	return TRUE;
}

// OCX로부터 시장 지표 TR을 날려야 될 상황이 되면
// 이 함수를 타도록 한다.
//BOOL CChartMng::MarketTRSend_By_OCXSignal(CString strCode, UINT nID, int nCount, BOOL bIsNew)
BOOL CChartMng::MarketTRSend_By_OCXSignal(	int nID,						//ChartOCX WindowID
											const LPCTSTR pcszPacketNames,	//해당 시장지표 패킷 명
											int nCount,						//요청할 갯수
											const BOOL bIsNew				//TRUE면 신규조회, FALSE면 연속조회
											)
{
	CString strCode;
	strCode.Format("%s", pcszPacketNames);
	
	if(!strCode.GetLength()) 
	{
#ifdef _DEBUG
		AfxMessageBox("MarketTRSend_By_OCXSignal \n 추가할 시장지표 패킷명이 없다.");
#endif 
		return FALSE;
	}

	CChartItem* pSelChartItem = NULL;
	EACH_CHARTJOB_START()
		if( pChartItem->GetDlgCtrlID() == nID)
		{
			pSelChartItem = pChartItem;
			goto ExitLoop3; 
		}
	EACH_CHARTJOB_END()

ExitLoop3:

	if(pSelChartItem == NULL) return FALSE;

	if(!pSelChartItem->GetCntOfItemData()||pSelChartItem->GetOnRunningOneChart()) 
	{
#ifdef _DEBUG
		AfxMessageBox("MarketTRSend_By_OCXSignal \n 차트 데이터가 없거나 TR을 받고 있는 중이다");
#endif 
		return FALSE;
	}

	//시장 지표 추가시 차트가 로드되고 있는 상태이면
	//요청될 타입을 검사하고
	//이미 로드가 끝난 상태이면 현재 차트에 설정된 타입을 체크한다.
	//05. 10. 27
	if(pSelChartItem->GetType() != NULL)
	{
		if(	pSelChartItem->GetType() != DAY_DATA_CHART )
		{
#ifdef _DEBUG
		AfxMessageBox("MarketTRSend_By_OCXSignal \n 시장지표는 일간에만 적용된다 #1");
#endif 

			return FALSE;
		}			
	}
	else
	{
		if(	pSelChartItem->GetTypeRequested() != DAY_DATA_CHART )
		{
#ifdef _DEBUG
		AfxMessageBox("MarketTRSend_By_OCXSignal \n 시장지표는 일간에만 적용된다 #2");
#endif 
			return FALSE;
		}
	}

	if(strCode.Find("미결제") >= 0)
	{
		if(pSelChartItem->GetDataTRCode() != QUERY_STRFUTOPT/* && pSelChartItem->GetDataTRCode() != QUERY_STRCOMMODITY*/)		// 상품선물옵션차트 - ojtaso (20071001)
			return FALSE;
	}
	else
	{
		if(pSelChartItem->GetDataTRCode() != QUERY_STRFOFOREIGN)
			return FALSE;
	}

	CString strTR = QUERY_STRMARKET;
	//=================================================================
	//중복해서 같은 시장지표 항목이 TR이 쌓인 경우 없애버린다.
	//2005.11.01
	//=================================================================
	pSelChartItem->RemoveChartItem_OneData(strCode,strTR);
	//=================================================================

	if(!strCode.GetLength()) 
	 	return FALSE;
	
	pSelChartItem->GetChartItemMarketDataNew(strTR, strCode);		
	
	RequestChart_MarketData(strTR, strCode, pSelChartItem, nCount, bIsNew);

	return TRUE;
}

BOOL CChartMng::AddMarketData(CString strCode, BOOL bFromTree)
{
	return TRUE;
}

BOOL CChartMng::ReplaceChart(long lRow1,long lCol1, long lRow2, long lCol2)
{
	if(!m_pChartItems[lRow1][lCol1]||!m_pChartItems[lRow2][lCol2]) 
		return FALSE;

	CChartItem *pChartItemSelected = m_pChartItems[lRow1][lCol1];
	m_pChartItems[lRow1][lCol1] = m_pChartItems[lRow2][lCol2];
	m_pChartItems[lRow1][lCol1]->SetRowNCol(lRow1,lCol1);
	m_pChartItems[lRow2][lCol2] = pChartItemSelected;	
	m_pChartItems[lRow2][lCol2]->SetRowNCol(lRow2,lCol2);
	InvalidateRect(GetRectOfChart(lRow1,lCol1,1));
	if( m_pChartItems[lRow1][lCol1]->GetSafeHwnd()) m_pChartItems[lRow1][lCol1]->MoveWindow(GetRectOfChart(lRow1,lCol1));
	InvalidateRect(GetRectOfChart(lRow2,lCol2,1));
	if( m_pChartItems[lRow2][lCol2]->GetSafeHwnd()) m_pChartItems[lRow2][lCol2]->MoveWindow(GetRectOfChart(lRow2,lCol2));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	비어있는 차트를 선택한다.
//////////////////////////////////////////////////////////////////////////////
BOOL CChartMng::SelectEmptyPos()
{
	long nRowIndexOld = -1;
	long nColIndexOld = -1;
	EACH_CHARTJOB_START()
		if(pChartItem->IsEmpty())
		{
			SetChartSelected(CPoint(nColIndex,nRowIndex));
			return TRUE;
			break;
		}
	EACH_CHARTJOB_END()
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	차트를 최대화 한다.
//////////////////////////////////////////////////////////////////////////////
void CChartMng::ExtendChart(BOOL bExtend/*= TRUE*/)
{
	m_bChartExtended = bExtend;
	MoveCharts();
	Invalidate();

	// to update alot things which didn't do due to extended mode..
	if(!m_bChartExtended&&(m_bJongmokLink||m_bTypeLink||m_bUpdatedInChartExtended))
	{
		CChartItem*	pChartItem = GetpChartItemSelected();
		
		if(!pChartItem) return ;
		InitInLoop();		
		m_bNeedToUpdateInLoop = FALSE;

		m_bUseRevisedInLoop = pChartItem->GetUseRevisedValue();

		m_strCodeInLoop = pChartItem->GetDataCode();
		m_strEndDateInLoop = pChartItem->GetEndDate();
		m_chTypeInLoop = pChartItem->GetType();
		m_lTickMinNumberInLoop = atol(pChartItem->GetTickMinDayNumber());
		m_lTypeInLoop = LOOPTYPE_UPDATE_BY_EXTEND/*5*/; //updateByExtend
		HeadInLoop();
	}
}

BOOL CChartMng::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(nCode ==CN_COMMAND)
	{
		long nCmdID = nID%10;
		UINT nChartID = nID - nID%10;
		EACH_CHARTJOB_START()
			if(pChartItem->GetID()==nChartID)
			{
				switch(nCmdID)
				{
				case 1:	// Maxmize
					SetChartSelected(CPoint(nColIndex,nRowIndex));
					ExtendChart(!m_bChartExtended);
					break;
				case 2:	// Initial
					//>> 예외처리 - vntsorl (20090106)
					if(pChartItem->GetType() && GetCheHogaType() == 1)
						break;
					//<< 예외처리 - vntsorl (20090106)
					SetChartSelected(CPoint(nColIndex,nRowIndex));
					ResetOCX();	
						
					//===================================================================
					// 틱 분 일 주 월 별 저장 기준 차트가  이놈일 경우 NULL로 해준다.
					// 안 해 주면 기준 차트 형태가 망가진다.
					// 05.11.24
					//===================================================================
					switch(pChartItem->GetType())
					{			
						case TICK_DATA_CHART:
							m_pTickCfgChart = NULL;
							break;
						case MIN_DATA_CHART:
							m_pMinCfgChart = NULL;
							break;
						case DAY_DATA_CHART:
							m_pDayCfgChart = NULL;
							break;
						case HOUR_DATA_CHART:
							m_pHourCfgChart = NULL;
							break;
						case WEEK_DATA_CHART:
							m_pWeekCfgChart = NULL;
							break;
						case MONTH_DATA_CHART:
							m_pMonthCfgChart = NULL;
							break;
						case YEAR_DATA_CHART:
							m_pYearCfgChart = NULL;
							break;
						case SEC_DATA_CHART:			
							m_pSecCfgChart = NULL;
							break;
					}					
					//===================================================================

					//@유진추가
					{
						CChartItem* pInitChartItem = GetpChartItemSelected();
						if(pInitChartItem)
							pInitChartItem->m_bNowInitialState = TRUE;
					}
					break;
				case 3:	
					SetChartSelected(CPoint(nColIndex,nRowIndex));		
					SetTimer(1, 100, NULL);										
					break;
				default:
					break;				
				}
			}
		EACH_CHARTJOB_END()
	}
	return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void  CChartMng::UnhighlightJCode(CChartItem *pChartItemIn)
{
	CString strJongmokCode = pChartItemIn->GetDataCode();
	EACH_CHARTJOB_START()
		if(pChartItemIn!=pChartItem)
		{
			if(pChartItem->GetDataCode()==strJongmokCode)
			{
				return;
			}
		}
	EACH_CHARTJOB_END()					
	if(m_pLeftBarDlg)
	{
		m_pLeftBarDlg->SendMessage(UMSG_SENDJCODEUNHIGHLIGHTED,(WPARAM)(LPCTSTR)strJongmokCode);	
	}	
}
///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	복수종목시에 (종목코드,시간)을 툴바로 보낸다.
//////////////////////////////////////////////////////////////////////////////
void  CChartMng::SetMultiCodes(CString strMultiCodes)
{
	if(m_bJongmokLink)
	{
		m_bJongmokLink = 0;
		m_pExtInputBarDlg->SendMessage(UMSG_SETJONGMOKLINK,(WPARAM)m_bJongmokLink);
	}

	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	switch( ( ( CStdDialog *)GetParent())->GetRequestPairTickState())
	{
		case E_PRTS_ROTATE_START:
					strMultiCodes = m_strMultiCodes;
					( ( CStdDialog *)GetParent())->SetRequestPairTickState( E_PRTS_ROTATE_PAIR);
					break;
		default:
		{
					m_strMultiCodes = strMultiCodes;
					CChartItem *pChartItem = GetpChartItemSelected();
					if( pChartItem)
						if( pChartItem->GetType() == TICK_DATA_CHART && atoi( pChartItem->GetTickMinDayNumberRequested()) == 1)
							( ( CStdDialog *)GetParent())->SetRequestPairTickState( E_PRTS_ROTATE_START);
					break;
		}
	}

	CString strAData;
	CString strJCode;
	CString strTimeMark;
	int nPos = strMultiCodes.Find(';',0);	
	strAData = strMultiCodes.Left(nPos);
	int nPos1 = strAData.Find(',',0);	
	strJCode = strAData.Left(nPos1);
	strAData.Delete(0,nPos1+1);
	strAData.Remove(':');
	m_strTimeMarkReserved = strAData;
	strMultiCodes.Delete(0,nPos+1);

	if( E_PRTS_ROTATE_START != ( ( CStdDialog *)GetParent())->GetRequestPairTickState())
		m_strMultiCodes = strMultiCodes;

	((CStdDialog*)GetParent())->ChangeCode(strJCode);
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	복수종목조회시 다음종목으로 조회한다.
//////////////////////////////////////////////////////////////////////////////
BOOL CChartMng::SetJCodesNext()
{
	if(!m_strMultiCodes.IsEmpty())
	{
		if(m_nMode == CHANGE_CHART)
		{
			// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)
			// (2008/9/14 - Seung-Won, Bae) for Pair Tick
			if( ( !m_bAddModeFromTab
					|| E_PRTS_ROTATE_PAIR == ( ( CStdDialog *)GetParent())->GetRequestPairTickState())
				&& !SelectNextPos())
			{
				m_strMultiCodes.Empty();
				return FALSE;
			}
		}
		SetMultiCodes(m_strMultiCodes);
		return TRUE;
	}
	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	else if( E_PRTS_ROTATE_PAIR == ( ( CStdDialog *)GetParent())->GetRequestPairTickState())
		( ( CStdDialog *)GetParent())->SetRequestPairTickState( E_RPTS_STOP);
		

	// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)
	if(m_bAddModeFromTab)
	{
		CChartItem* pChartItem = NULL;
		pChartItem = GetpChartItemSelected();
		if(pChartItem)
			pChartItem->SetResetTotalCountWhileDraw(TRUE);

		m_bAddModeFromTab = FALSE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	다음의 차트를 선택한다.
//////////////////////////////////////////////////////////////////////////////
BOOL CChartMng::SelectNextPos()
{
	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	if( E_PRTS_ROTATE_START == ( ( CStdDialog *)GetParent())->GetRequestPairTickState())
		return TRUE;

	long lColSelected = m_lColSelected;
	long lRowSelected = m_lRowSelected;

	if((m_lColSelected+1)>=m_lCols)
	{
		if((m_lRowSelected+1)>=m_lRows)
		{
			return FALSE;
		}
		else
		{
			lColSelected = 0;
			lRowSelected++;			
		}
	}
	else
	{
		lColSelected++;
	}
	SetChartSelected(CPoint(lColSelected,lRowSelected));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	차트정보를 조회한다.
//////////////////////////////////////////////////////////////////////////////
// 복수종목 추가 : 복수종목 - ojtaso (20070118)
BOOL CChartMng::RequestChartData(BOOL bChangeType/* = FALSE*/)
{
	m_bChangeTypeMode = bChangeType;

	CChartItem* pChartItem = NULL;
	pChartItem = GetpChartItemSelected();
	if (!pChartItem)
		return FALSE;

	pChartItem->m_strOrderType = ((CInputBarDlg*)m_pInputBarDlg)->GetOrderType();
	stSend2Server* st = pChartItem->IsLoadingRQ() ? pChartItem->GetLoadedQueryData() : pChartItem->GetQueryData(bChangeType);

	if (st == NULL)
		return FALSE;

	st->m_nType = 0;
	st->m_pWnd = this;
//	st->m_pBuffer = (void*)(LPCTSTR)strPacket;
//	st->m_lLength= strPacket.GetLength();
	st->m_strTRCode = GetpChartItemSelected()->GetTrCodeForQuery();

	GetParent()->SendMessage(UMSG_SEND2SERVER,0,(long)st);

	if (st->m_strTRCode == QUERY_STRFOFOREIGN)
	{
		HCQ03010_IN* hcq03010InRec1 = (HCQ03010_IN*)st->m_pBuffer;
		delete hcq03010InRec1;
		hcq03010InRec1 = NULL;
	}
	else if (st->m_strTRCode == QUERY_STRFOFOREIGN_HOGA2)
	{
		HCQ03011_IN* hcq03011InRec1 = (HCQ03011_IN*)st->m_pBuffer;
		delete hcq03011InRec1;
		hcq03011InRec1 = NULL;
	}

	delete st;
	st = NULL;

	return TRUE;
}

BOOL CChartMng::RequestChart_MarketData(CString strTRCode, CString strJipyoName,
										CChartItem* pSelChartItem, int nDataRqst, BOOL bIsNew)
{
	if (pSelChartItem == NULL)
		return FALSE;

	CString strPacket = pSelChartItem->GetMarketQueryData(strJipyoName, nDataRqst, bIsNew);

	if (strPacket=="")
		return FALSE;

	stSend2Server st;
	st.m_nType			= 0;
	st.m_pWnd			= this;
	st.m_pBuffer		= (void*)(LPCTSTR)strPacket;
	st.m_lLength		= strPacket.GetLength();
	st.m_strTRCode		= strTRCode;

	GetParent()->SendMessage(UMSG_SEND2SERVER,0,(long)&st);	

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	차트내의 실시간 정보를 처리한다.
//////////////////////////////////////////////////////////////////////////////
void CChartMng::SetRealJPriceData(void *pRealData, int pRealType)
{
	CString szTemp0, szTmpCode;

	STREALTYPEDATA sndData;

	EACH_CHARTJOB_START()
// 		if ( pRealType == STOCK_30SEC_PID )		//	30초 데이터를 그리기위한 시그널 데이터
// 			pChartItem->SetRealTimeSignal( pRealData, STOCK_REAL, pRealType);
// 		else
// 		{
			if (pChartItem->IsOnDrdsAdvise())
			{
				int nType;
				int nMarketType = ((CStdDialog*)GetParent())->GetMarketTypeFromCode(pChartItem->GetDataCode(), nType);
				if (nMarketType == CMasterTypeInfo::STOCK_CHART)
					pChartItem->SetRealPriceData( pRealData, STOCK_REAL, pRealType);

				CRealData_Common* pRealData_Common = (CRealData_Common*)pRealData;

				CString strDecimalGB = CStdDialog::GetCodeType(pRealData_Common->m_strCode);
				if (strDecimalGB.GetLength() <= 0)
					strDecimalGB = "1";

				CString strPrice = "", strOpen = "", strHigh = "", strLow = "";
				double dRate;
				int nPcnt, nFcnt, nHcnt, nBase;
				char cType = pChartItem->GetDecimalRate(strDecimalGB, nPcnt, dRate, nFcnt, nHcnt, nBase);

				sndData.nDataType = nMarketType;
				sndData.szCode = (LPSTR)(LPCSTR)pRealData_Common->m_strCode;
				/*
				sndData.oPrice = (LPSTR)(LPCSTR)pRealData_Common->m_strOpen;
				sndData.hPrice = (LPSTR)(LPCSTR)pRealData_Common->m_strHigh;
				sndData.lPrice = (LPSTR)(LPCSTR)pRealData_Common->m_strLow;
				sndData.cPrice = (LPSTR)(LPCSTR)pRealData_Common->m_strPrice;
				*/
				if( nBase <= 1 )
				{
					strPrice.Format("%.*f", nPcnt, atof(pRealData_Common->m_strPrice) * dRate);
					strOpen.Format("%.*f", nPcnt, atof(pRealData_Common->m_strOpen) * dRate);
					strHigh.Format("%.*f", nPcnt, atof(pRealData_Common->m_strHigh) * dRate);
					strLow.Format("%.*f", nPcnt, atof(pRealData_Common->m_strLow) * dRate);
				}
				else
				{
					strPrice = (LPSTR)(LPCSTR)pChartItem->Gl_Format32String(atof(pRealData_Common->m_strPrice), nBase, nFcnt+nHcnt, nHcnt);
					strOpen = (LPSTR)(LPCSTR)pChartItem->Gl_Format32String(atof(pRealData_Common->m_strOpen), nBase, nFcnt+nHcnt, nHcnt);
					strHigh = (LPSTR)(LPCSTR)pChartItem->Gl_Format32String(atof(pRealData_Common->m_strHigh), nBase, nFcnt+nHcnt, nHcnt);
					strLow = (LPSTR)(LPCSTR)pChartItem->Gl_Format32String(atof(pRealData_Common->m_strLow), nBase, nFcnt+nHcnt, nHcnt);
				}
				sndData.oPrice = (LPSTR)(LPCSTR)strOpen;
				sndData.hPrice = (LPSTR)(LPCSTR)strHigh;
				sndData.lPrice = (LPSTR)(LPCSTR)strLow;
				sndData.cPrice = (LPSTR)(LPCSTR)strPrice;

				pChartItem->InvokeAddIn("ChartGuideLineAddIn.dll:GUIDE_LINE", INVOKEMSG_RECEIVEREALDATA, (long)&sndData);
			}
//		}
	EACH_CHARTJOB_END()
}

void CChartMng::SetRealUPriceData(char *pData, int nSize)
{
	//char szTemp0[16];
	//DRDS_JS0_DATA* Op = (DRDS_JS0_DATA*) pData;
	//MEMCPYWITHNULL(szTemp0,Op->upcode);		// 종목코드
	//szTemp0[sizeof(Op->upcode)] = NULL;
	//EACH_CHARTJOB_START()
	//	if(pChartItem->GetDataCode().GetLength()==UPJONG_LENGTH&&pChartItem->GetDataCode()==szTemp0&&pChartItem->IsOnDrdsAdvise())
	//	{
	//		pChartItem->SetRealPriceData( pData, nSize, UPJONG_REAL);
	//	}
	//EACH_CHARTJOB_END()

}


void CChartMng::SetRealFPriceData(char *pData, int nSize)
{
	//char szTemp0[16];
	//DRDS_SC0_DATA* Op = (DRDS_SC0_DATA*) pData;
	//MEMCPYWITHNULL(szTemp0,Op->jmcode);		// 종목코드
	//szTemp0[sizeof(Op->jmcode)] = NULL;
	//EACH_CHARTJOB_START()
	//	CString sCode = pChartItem->GetDataCode();
	//	if( (sCode.GetLength()==FUTOPT_LENGTH)&&
	//		sCode==szTemp0 &&
	//		pChartItem->IsOnDrdsAdvise()
	//	)
	//	{
	//		pChartItem->SetRealPriceData( pData, nSize, FUTURE_REAL);
	//	}
	//EACH_CHARTJOB_END()
}

void CChartMng::SetRealOPriceData(char *pData, int nSize)
{
	//char szTemp0[16];
	//DRDS_OC0_DATA* Op = (DRDS_OC0_DATA*) pData;
	//MEMCPYWITHNULL(szTemp0,Op->jmcode);		// 종목코드
	//szTemp0[sizeof(Op->jmcode)] = NULL;
	//EACH_CHARTJOB_START()
	//	CString sCode = pChartItem->GetDataCode();
	//	if( ( ( sCode.GetLength()==FUTOPT_LENGTH)	||
	//		  ( sCode.GetLength()==	KQOPT_LENGTH12) ||
	//		  ( sCode.GetLength()==KQFUT_LENGTH)	||
	//		  ( sCode.GetLength()==KQOPT_LENGTH) )&&		
	//		sCode==szTemp0&&
	//		pChartItem->IsOnDrdsAdvise()
	//	)
	//	{
	//		pChartItem->SetRealPriceData( pData, nSize, OPTION_REAL);
	//	}
	//EACH_CHARTJOB_END()

}

void CChartMng::SetRealHPriceData(char *pData, int nSize)
{
	/*char szTemp0[16];
	
	st_OC0* Op = (st_OC0*) pData;
	MEMCPYWITHNULL(szTemp0,Op->jmcode);		// 종목코드
	szTemp0[sizeof(Op->jmcode)] = NULL;
	EACH_CHARTJOB_START()
		if(pChartItem->GetDataCode().GetLength()==FUTOPT_LENGTH&&pChartItem->GetDataCode()==szTemp0&&pChartItem->IsOnDrdsAdvise())
		{
			pChartItem->SetRealPriceData( pData, nSize, 2);
		}
	EACH_CHARTJOB_END()
	*/
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	해당종목이 실시간이 요하는지 확인한다.
//////////////////////////////////////////////////////////////////////////////
BOOL CChartMng::IsAdvisingCodeOf(LPCTSTR lpCode)
{
	EACH_CHARTJOB_START()
		if(pChartItem->GetDataCode()==lpCode)
		{
			return TRUE;
		}		
	EACH_CHARTJOB_END()
	return FALSE;
}

// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
void CChartMng::AddRealCodeList(LPCTSTR lpszCode)
{
	((CStdDialog*)GetParent())->AddRealCodeList(lpszCode);
}

// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
void CChartMng::AddRealCodes()
{
	((CStdDialog*)GetParent())->AddRealCodes();
}

// 복수종목 실시간 처리 : 복수종목 - ojtaso (20070109)
void CChartMng::OnChartMngCodeAdvise(CChartItem *pItem, BOOL bAddMode)
{
	//>> 20100227_JS.Kim 복수종목에서 중첩되어 있는 종목으로 Advise 해야함
	//((CStdDialog*)GetParent())->OnCodeAdvise(pItem->GetDataCode(), bAddMode);
	CString strCode = pItem->m_lpSelChart ? pItem->m_lpSelChart->m_strItemCode : pItem->GetDataCode();
	((CStdDialog*)GetParent())->OnCodeAdvise(strCode, bAddMode);
	//<<

}

// 복수종목 실시간 처리 : 복수종목 - ojtaso (20070109)
void CChartMng::OnChartMngCodeUnAdvise(CChartItem *pItem, BOOL bAddMode)
{
	EACH_CHARTJOB_START()
		if(pChartItem!=pItem)			
		{
			//if(pItem->GetDataCode()==pChartItem->GetDataCode())
			//{
			//	return;
			//}	
			
			if(pChartItem->GetDataCode()==pItem->GetOldChartCode())
			{
				return;
			}	
		}
	EACH_CHARTJOB_END()
	//((CStdDialog*)GetParent())->OnCodeUnAdvise(pItem->GetDataCode());

	//05. 09. 07 =================================================================================
	//이전 코드를 해제하지 않고 있어서 해제 시키는 루틴 추가
	CString strOldCode = pItem->GetOldChartCode(); strOldCode.TrimLeft(); strOldCode.TrimRight();
	if(strOldCode.GetLength())
		((CStdDialog*)GetParent())->OnCodeUnAdvise(strOldCode, bAddMode);
	//============================================================================================
}

//////////////////////////////////////////////////////////////////////////////
//	작업자	:	sy, nam
//	일자	:	2005. 08.02
//	목적	:	모든 차트의 Real을 해제한다.
//////////////////////////////////////////////////////////////////////////////
void CChartMng::OnAllCodeUnAdvise()
{
	EACH_CHARTJOB_START()
		if(pChartItem!=NULL)			
		{
			if(pChartItem->GetDataCode())
			{
				((CStdDialog*)GetParent())->OnCodeUnAdvise(pChartItem->GetDataCode(), pChartItem->IsAddMode());
			}			
		}
	EACH_CHARTJOB_END()
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	저장된 차트를 불러온다.
//////////////////////////////////////////////////////////////////////////////
void CChartMng::LoadChart(CPoint *pPt)
{
	CString strSelected;
	CStringArray arrayFileTitle;
	CStringArray arrayFilePath;
	//	if(m_strUserPath.GetLength())	
	//	{
	//		CFileFind ff;
	//
	//		CString strMultiCharts = m_strUserPath + "\\*.mct";
	//				
	//		BOOL bResult = ff.FindFile(strMultiCharts);
	//		if(bResult)
	//		{
	//			BOOL bFileExist = TRUE;
	//			while(bFileExist)
	//			{
	//				bFileExist = ff.FindNextFile();
	//				
	//				CString strTemp = ff.GetFileTitle();
	//				int nPos = strTemp.ReverseFind('_');
	//				CString strFileTitle = strTemp.Mid(nPos + 1);
	//
	//				arrayFileTitle.Add(strFileTitle);
	//				arrayFilePath.Add(ff.GetFilePath());
	//			}
	//		}
	//	}
	//
	//	long lAllCnt = arrayFileTitle.GetSize();
	//리스트 로드 방식 변경(인덱스) by LYH 2006.09.28
	int nCnt = 0;
	CString strIndexFile = m_strUserPath + "\\ChartIndex.dat";
	CString strIndex, strFileTitle, strFilePath;
	char szKey[30];

	sprintf(szKey,"%s","CHART LIST");
	long lAllCnt = ::GetPrivateProfileInt(szKey, "COUNT", -1, strIndexFile);

	for(int i=0;i < lAllCnt; i++)
	{
		strIndex.Format("%02d", i);
		::GetPrivateProfileString(szKey, strIndex, "", strFileTitle.GetBuffer(MAX_PATH), MAX_PATH, strIndexFile);	
		strFileTitle.ReleaseBuffer();

		if (strFileTitle.GetLength() == 0)
			continue;
		strFilePath = m_strUserPath +"\\"+ strFileTitle +".mct";
		arrayFileTitle.Add(strFileTitle);
		arrayFilePath.Add(strFilePath);
	}

	if(!lAllCnt)
	{
		GetParent()->SendMessage(UMSG_SENDTOMSGBOX,(WPARAM)&ERRORMSG7);
		return;
	}
	CMenu menu;
	menu.CreatePopupMenu();

	long lPosAll = 0;
	long lSize = 0;
	CString strIndicator;
	for(long lIndex = 0;lIndex<lAllCnt;lIndex++)
	{
		menu.AppendMenu(MF_STRING,(UINT)1000+lIndex,arrayFileTitle[lIndex]);
	}

	UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
	int nRetValue = menu.TrackPopupMenu(nFlagsForMenu, pPt->x, pPt->y, this);
	if(!nRetValue) return;

	if(nRetValue-1000<lAllCnt)
	{
		strSelected = arrayFilePath[nRetValue-1000];
	}	

	menu.DestroyMenu();
	LoadChart(strSelected);
}

BOOL CChartMng::LoadChart(CString strLoadFile)
{
	int nCols = 0;
	int nRows = 0;
	char szTemp[16];
	CString strTemp;
	::GetPrivateProfileString("FrameInfo","COLS","",szTemp,sizeof(szTemp),strLoadFile);
	nCols = atoi(szTemp);
	::GetPrivateProfileString("FrameInfo","ROWS","",szTemp,sizeof(szTemp),strLoadFile);
	nRows = atoi(szTemp);
	if(((CStdDialog*)GetParent())->m_nChartType==SINGLE_CHART)
	{
		if(nCols > 1 || nRows > 1) 
		{
			nCols = 1;
			nRows = 1;
		}
	}

	SetMatrix(nRows,nCols);
	m_pExtInputBarDlg->SendMessage(UMSG_CLEARLINK);	


	//저장 된 분할 화면이 1일경우...공유로 받은 종목을 쓰고..
	//그 외는 공유 종목은 무시한다.=============================
	//if(nCols * nRows != 1) 
	//{
	//	m_strStatic_ShareCode = _T("");
	//	m_strCodeForLoadChart = _T("");
	//}
	//=============================================================

//	InitInLoop();
//	m_strChartSelectedInLoop = strLoadFile;
//	m_lTypeInLoop = LOOPTYPE_LOAD/*1*/;//Load	
//	HeadInLoop();


//======================================================================
	BOOL bReult = FALSE;

	InitInLoop();
	m_strChartSelectedInLoop = strLoadFile;
	m_lTypeInLoop = LOOPTYPE_LOAD/*1*/;//Load	
	bReult = HeadInLoop();

	if(m_lTypeInLoop == LOOPTYPE_LOAD)
		return bReult;

//======================================================================
	return TRUE;

}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	차트정보를 저장한다.
//////////////////////////////////////////////////////////////////////////////
void CChartMng::SaveChart(CPoint *pPt)
{
	CChartListDlg dlg;
	CString strUserPath = m_strUserPath;
	dlg.SetUserPath(strUserPath);
	dlg.m_nInfoType = 1;
	//dlg.m_pPtTopLeftOfDlg = pPt;
	dlg.m_nMarketType = m_nMarketType;
	dlg.m_pChartMng = this;
	if(dlg.DoModal()==IDOK)
	{
		if(dlg.m_strChartSelected.IsEmpty()) return;		
		SaveChart(dlg.m_strChartSelected);
	}	
}

void CChartMng::SaveChart(CString strSaveFile)
{
#ifdef _WRITELOG
	((CStdDialog*)GetParent())->WriteToStringLog("CChartMng::SaveChart::Begin", _T("멀티차트 종료"), 13);
#endif
	CString strTemp;
//2005. 08. 27======================================================================
	if(m_pInputBarDlg)
	{
		strTemp.Format("%d",m_pInputBarDlg->SendMessage(UMSG_GETREVISED));
		::WritePrivateProfileString("InputBar","Revised",strTemp,strSaveFile);	

		strTemp.Format("%d",m_pInputBarDlg->SendMessage(UMSG_GETLOG));
		::WritePrivateProfileString("InputBar","Log",strTemp,strSaveFile);	
	}
//=====================================================================================	

	strTemp.Format("%d",m_lCols);	
	::WritePrivateProfileString("FrameInfo","COLS",strTemp,strSaveFile);

	strTemp.Format("%d",m_lRows);	
	::WritePrivateProfileString("FrameInfo","ROWS",strTemp,strSaveFile);

	EACH_CHARTJOB_START()		
		pChartItem->SaveChartInfo(strSaveFile);
	EACH_CHARTJOB_END()
#ifdef _WRITELOG
		((CStdDialog*)GetParent())->WriteToStringLog("CChartMng::SaveChart::End", _T("멀티차트 종료"), 13);
#endif
}

///////////////////////////////////////////////////////////////////////////////
//	일자	:	2006.09.26
//	목적	:	저장된 차트를 불러오고 저장된 차트를 로드한다
//////////////////////////////////////////////////////////////////////////////
void CChartMng::LoadSaveChart(CPoint *pPt)
{
	//리스트 로드 방식 변경(인덱스) by LYH 2006.09.28
	CString strSelected;
	CStringArray arrayFileTitle;
	CStringArray arrayFilePath;
	int nCnt = 0;
	CString strIndexFile = m_strUserPath + "\\ChartIndex.dat";
	CString strIndex, strFileTitle, strFilePath;
	char szKey[30];

	sprintf(szKey,"%s","CHART LIST");
	long lAllCnt = ::GetPrivateProfileInt(szKey, "COUNT", -1, strIndexFile);

	for(int i=0;i < lAllCnt; i++)
	{
		strIndex.Format("%02d", i);
		::GetPrivateProfileString(szKey, strIndex, "", strFileTitle.GetBuffer(MAX_PATH), MAX_PATH, strIndexFile);	
		strFileTitle.ReleaseBuffer();

		if (strFileTitle.GetLength() == 0)
			continue;
		strFilePath = m_strUserPath +"\\"+ strFileTitle +".mct";
		arrayFileTitle.Add(strFileTitle);
		arrayFilePath.Add(strFilePath);
	}

//	if(!lAllCnt)
//	{
//		GetParent()->SendMessage(UMSG_SENDTOMSGBOX,(WPARAM)&ERRORMSG7);
//		return;
//	}
	CMenu menu;
	menu.CreatePopupMenu();

	menu.AppendMenu(MF_UNCHECKED,(UINT)101, "등록 및 삭제");
	menu.SetDefaultItem((UINT)101);
	if(((CStdDialog*)GetParent())->m_nChartType!=SINGLE_CHART)
	{
		if(m_pInputBarDlg)
		{
			BOOL bLoadSaveCode = m_pInputBarDlg->SendMessage(UMSG_GETLOADSAVECODE);
			UINT nFlags = NULL;
			if(bLoadSaveCode)
			{
				nFlags = MF_CHECKED;
			}
			else
			{
				nFlags = MF_UNCHECKED;
			}
			menu.AppendMenu(nFlags,(UINT)102,"저장종목 호출");
		}
	}

	menu.AppendMenu(MF_SEPARATOR, (UINT)103, "");

	long lPosAll = 0;
	long lSize = 0;
	CString strIndicator;
	for(long lIndex = 0;lIndex<lAllCnt;lIndex++)
	{
		menu.AppendMenu(MF_STRING,(UINT)1000+lIndex,arrayFileTitle[lIndex]);
	}

	UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
	int nRetValue = menu.TrackPopupMenu(nFlagsForMenu, pPt->x, pPt->y, this);
	if(!nRetValue) return;

	menu.DestroyMenu();
	if(101 == nRetValue) // 저장
	{
		SaveChart(pPt);
	}
	else if(102 == nRetValue) //세팅
	{
		m_pInputBarDlg->SendMessage(UMSG_SETLOADSAVECODE);
	}
	else if(nRetValue-1000<lAllCnt)	// 불러오기
	{
		strSelected = arrayFilePath[nRetValue-1000];
		//{{ 2007.02.05 by LYH 불러오기 변경
		//LoadChart(strSelected);
		m_strCodeForLoadChart = GetpChartItemSelected()->GetDataCode();

		// 저장된 차트환경의 RQ가 2 이상일 경우
		// 저장종목 호출 셋팅 및 차트화면 초기화
		{
			UINT nSize = ::GetPrivateProfileInt("0000", _T("RQSIZE"), 0, strSelected);

			CString strRQData;
			LPSTR lpszRQDAta = strRQData.GetBufferSetLength(nSize + 1);
			
			::GetPrivateProfileString("0000", _T("RQ"), _T("0"), lpszRQDAta, nSize, strSelected);
			
			strRQData.ReleaseBuffer();
			
			CString strTemp;
			int nIndex = 0;
			if(!AfxExtractSubString(strTemp, strRQData, nIndex++, _T(';')))
				return;
			
			int nCount = atoi(strTemp);
			
			if(nCount >= 2)
			{			
				BOOL bLoadSaveCode = m_pInputBarDlg->SendMessage(UMSG_GETLOADSAVECODE);
				if(bLoadSaveCode == FALSE)
					m_pInputBarDlg->SendMessage(UMSG_SETLOADSAVECODE);
			}

			//{1*1 차트로 초기화 요청
			ReplaceChart(0,0,m_lRowSelected,m_lColSelected);
			SetMatrix(1,1);
			//}
			if(m_pInputBarDlg)
				((CInputBarDlg*)m_pInputBarDlg)->SetMode(CHANGE_ITEM);
			
			ResetChart();
			ResetOCX();
			GetpChartItemSelected()->RemoveMapChartItem();
		}

		LoadChart(strSelected);
		m_strCodeForLoadChart = "";
		EACH_CHARTJOB_START()		
			pChartItem->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::T_ALL_TOOL_RESET_STATE, 0);
		EACH_CHARTJOB_END()
		//}}

	}	
}

BOOL CChartMng::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(pMsg->message) 
	{
		case WM_KEYDOWN:
		{
			UINT nChar = (UINT)pMsg->wParam;
			switch(nChar)
			{
				case VK_ESCAPE:
				case VK_RETURN:
				{
					return FALSE;
					break;					
				}
			}
			break;
		}
		default:
		{
			break;
		}
	}		
	//if(pMsg->wParam == VK_HOME)
	//	return FALSE;
	if(::IsWindow(m_hWnd))
		return GetParent()->PreTranslateMessage(pMsg);
	else
		return CWnd::PreTranslateMessage(pMsg);
}

void CChartMng::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bInRectOfChart = FALSE;
	CRect rect;
	EACH_CHARTJOB_START()
		rect = GetRectOfChart(nRowIndex,nColIndex,TRUE);
		if(rect.PtInRect(point))
		{
			SetChartSelected(CPoint(nColIndex,nRowIndex));
			bInRectOfChart = TRUE;
			break;
		}
	EACH_CHARTJOB_END()	
	
	if(!GetpChartItemSelected()) return ;
	if(bInRectOfChart&&GetpChartItemSelected()->GetDataTRCode()==QUERY_STRFOFOREIGN)
	{
		STSHAREDDATA stInfo;
		stInfo.m_szKey = "코드-코코2";
		stInfo.m_szValue = GetpChartItemSelected()->GetDataCode();
		stInfo.m_szValue.TrimRight(';');
		//((IMainDllObMain*)g_pMainDll)->m_pMainFrame->SendMessage(RMSG_CREATEOPENSHAREDATA,0, (LPARAM)&stInfo);		
		CWnd::OnRButtonUp(nFlags, point);
	}
}

//@solomon 20090925 정용호, 이함수를 호출하는데가 없다
void CChartMng::RequestAVGForGuideLine(BOOL bSelectedOnly)
{
	return;
	/*
	if(!GetpChartItemSelected()) return ;

	CUIntArray AryAvgForChartGuideLine;
	AryAvgForChartGuideLine.SetSize(4);
	GetpChartItemSelected()->InvokeAddIn("ChartGuideLine.dll",INVOKEMSG_GETAVGINFO,(long)&AryAvgForChartGuideLine);
	stSend2Server st;
	CString strPacket;
	if(bSelectedOnly)
	{
		strPacket = GetpChartItemSelected()->GetQueryDataForGuideLine(&AryAvgForChartGuideLine);
		if(strPacket=="")	return;	
		st.m_nType = 0;
		st.m_pWnd = GetpChartItemSelected();
		st.m_pBuffer = (void*)(LPCTSTR)strPacket;
		st.m_lLength= strPacket.GetLength();
		st.m_strTRCode = QUERY_STR_AVGMP;
		GetParent()->SendMessage(UMSG_SEND2SERVER,0,(long)&st);
	}
	else
	{
		EACH_CHARTJOB_START()
			strPacket = pChartItem->GetQueryDataForGuideLine(&AryAvgForChartGuideLine);
			if(strPacket=="")	continue;	
			st.m_nType = 0;
			st.m_pWnd = pChartItem;
			st.m_pBuffer = (void*)(LPCTSTR)strPacket;
			st.m_lLength= strPacket.GetLength();
			st.m_strTRCode = QUERY_STR_AVGMP;
			GetParent()->SendMessage(UMSG_SEND2SERVER,0,(long)&st);
		EACH_CHARTJOB_END()	
	}
	*/
}

void CChartMng::SetSendJongData(LPCTSTR lpQuoteCode,char chType, LPCTSTR lpMinNTickNumber)
{
/*	const int nMaxSendResultCnt = 100;
	CString strMapNo = "60000";

	STOPENMAPINFO data;
	char szHandle[9] = {0, };
	::sprintf(szHandle, "%08x", GetParent()->GetSafeHwnd());
	::memcpy(data.m_aOpenWnd, szHandle, sizeof(data.m_aOpenWnd));

	::memset(data.m_aMap, 0x00, OMID_MAPNAME);
	::strcpy(data.m_aMap, strMapNo);  // 5자리

	data.m_nOpenType = OMI_MAINMAP;

	
	CString strCodeData;	
	if(!m_lDragStart||!m_lDragEnd) 
	{
		return;
	}
	else if(m_lDragStart<m_lDragEnd) 
	{
		strCodeData.Format("%6s,%c,%s,%08d,%08d,",lpQuoteCode,chType, lpMinNTickNumber, m_lDragStart,m_lDragEnd);
	}
	else
	{
		strCodeData.Format("%6s,%c,%s,%08d,%08d,",lpQuoteCode,chType, lpMinNTickNumber, m_lDragEnd,m_lDragStart);
	}
//	CString strTest;
//	strTest = "\n[gm0604]";
//	strTest += strCodeData;
//	strTest += "\n";
//	::OutputDebugString(strTest);

	
	::memset(data.m_aOpenInfo, 0x00, OMID_OPENDATA);
	::strcpy(data.m_aOpenInfo, strCodeData);

	data.m_nInfoLen = strCodeData.GetLength();
	GetParent()->GetParent()->GetParent()->GetParent()->SendMessage(RMSG_OPENMAPWITHDATA, 0, (LPARAM)&data);
*/	
}


void CChartMng::GetIndicatorNameListIntoTreeCtrl(CTreeCtrl& rxtreeCtrl)
{
	//-----------------------------------------------------------------
	// Han Hwa 지표구조에 맞게 변경한다.
	//-----------------------------------------------------------------
	/*char* szIndexGroupName[] = { 
									"기본지표",
									"이동평균",
									"주가지표",
									"기타지표",
									"첨부지표",
									"추세지표",
									"거래량지표",
									"변동성지표",
									"탄력성지표"};
	//-----------------------------------------------------------------*/
	char* szIndexGroupName[] = { 
									//"전체지표",
									"가격지표",
									"추세지표",
									"변동성지표",
// (2008/9/13 - Seung-Won, Bae) Remove Volume Index
									"거래량지표",
									"2차파생지표",
									"기타지표",
									"시장지표",
									""
	};

	long lAllCnt = sizeof(szIndexGroupName)/sizeof(char*);

//#ifdef _DEBUG
	//=============================================================
	// 05.10.24
	// 주식만 시장 지표 추가 
	//=============================================================
	if(m_nMarketType != CMasterTypeInfo::STOCK_CHART)
		lAllCnt -= 1;
	//=============================================================
//#endif

	rxtreeCtrl.DeleteAllItems();
	CStringArray strArrayIndicator;	
	long lSize = 0;
	HTREEITEM hRootItem = NULL;
	for(long lIndex = 0;lIndex<lAllCnt;lIndex++)
	{
		hRootItem = rxtreeCtrl.InsertItem(szIndexGroupName[lIndex]);
		GetIndicatorNameListIntoArray(&strArrayIndicator, szIndexGroupName[lIndex]);
		lSize = strArrayIndicator.GetSize();
		for(long lPos=0;lPos<lSize;lPos++)
		{
			rxtreeCtrl.InsertItem(strArrayIndicator.GetAt(lPos),hRootItem);
		}		
	}

//	//====================================================================
//	// 시장 지표 테스트 하기 위해 임의로 넣는다.
//	// 나중에 지운다.
//#ifdef _DEBUG	
//	rxtreeCtrl.InsertItem("금리",hRootItem);
//	rxtreeCtrl.InsertItem("뉴스강도",hRootItem);
//#endif
//	//====================================================================
}



///////////////////////////////////////////////////////////////////////////////
//	작업자	:	Sang Yun, Nam
//	E-Mail	:	bbodongyi@hotmail.com
//	일자	:	2005.04.25
//	Input	:	p_strGroup  지표 그룹명
//				결과값이 들어갈 ComboBOx
//	목적	:	차트에 사용될 차트, 지표 목록을 가져온다.
///////////////////////////////////////////////////////////////////////////////
void CChartMng::GetIndicatorNameListIntoComboBox(CComboBox& rComboBox, LPCTSTR p_strGroup)
{
	if(p_strGroup[0]!=NULL)
	{
		CString strALine = GetIndicatorNameList(p_strGroup);
		CString strAItem;
		int nPos = 0;
		int nItem = 0;
		int nLength = 0;
		rComboBox.Clear();
			//rxlistCtrl.DeleteAllItems();
		if(!strcmp(p_strGroup,"특수지표"))
		{
//			OutputDebugString("집어 넣는다.");



			rComboBox.AddString("캔들차트");
			rComboBox.AddString("바차트");
			rComboBox.AddString("선차트");
			//rComboBox.AddString("점차트");
			//rComboBox.AddString("렌코차트");
			//rComboBox.AddString("삼선전환도");
			//rComboBox.AddString("이큐볼륨차트");
			//rComboBox.AddString("캔들볼륨차트");
			//rComboBox.AddString("P&F차트");
			
			/*
			rxlistCtrl.InsertItem(0, "");
			rxlistCtrl.SetItemText(0, 0, "캔들차트",RGB(0,0,0),CRGRID);
			rxlistCtrl.InsertItem(1, "");
			rxlistCtrl.SetItemText(1, 0, "바차트");
			rxlistCtrl.InsertItem(2, "");
			rxlistCtrl.SetItemText(2, 0, "선차트",RGB(0,0,0),CRGRID);
			*/
			nItem = 3;
		}

		if(strALine.GetLength())
		{
			while(1)
			{
				nLength = strALine.GetLength();
				if(!nLength) break;
				nPos = strALine.Find(',');
				if(nPos==-1) 
				{
					strAItem = strALine;
					strALine.Empty();
				}
				else
				{
					strAItem = strALine.Left(nPos);
					strALine.Delete(0,nPos+1);
				}

				if(strAItem=="P&&F")
				{
					strAItem = "P&F";
				}
				rComboBox.AddString(strAItem);

				/*
				if(strAItem=="P&F")
				{
					strAItem = "P&&F";
				}
				rComboBox.AddString(strAItem);
				
				rxlistCtrl.InsertItem(nItem, "");
				if(nItem%2) // odd
				{					
					rxlistCtrl.SetItemText(nItem, 0, strAItem);
				}
				else		// even
				{
					rxlistCtrl.SetItemText(nItem, 0, strAItem,RGB(0,0,0),CRGRID);					
				}*/				
				nItem++;
			}
		}
	}
	else
	{
		CString strList = GetIndicatorNameList("");
		CString strALine;
		CString strALineTitle;
		CString strAItem;
		int nPos = 0;
		int nItem = 0;
		int nLength = 0;

		rComboBox.Clear();
		rComboBox.AddString("거래량차트");
//		rxlistCtrl.DeleteAllItems();
//		rxlistCtrl.InsertItem(nItem, "");
//		rxlistCtrl.SetItemText(nItem, 0, "거래량차트",RGB(0,0,0),CRGRID);
		nItem++;

		if(strList.GetLength())
		{
			while(1)
			{
				nPos = strList.Find(':');
				if(nPos==-1) break;		
				if(nPos==0) 
				{
					nPos = strList.Find('\n');
					strList.Delete(0,nPos+1);
					continue;
				}			
				strALineTitle = strList.Left(nPos);
				if(strALineTitle=="기본지표"
					||strALineTitle=="시장지표"
					||strALineTitle=="투자자동향"
					||strALineTitle=="특수지표")
				{
					nPos = strList.Find('\n');
					strList.Delete(0,nPos+1);
					continue;
				}

				strList.Delete(0,nPos+1);
				nPos = strList.Find('\n');
				strALine = strList.Left(nPos-1);		
				strList.Delete(0,nPos+1);
				
				while(1)
				{
					nLength = strALine.GetLength();
					if(!nLength) break;
					nPos = strALine.Find(',');
					if(nPos==-1) 
					{
						strAItem = strALine;
						strALine.Empty();
					}
					else
					{
						strAItem = strALine.Left(nPos);
						strALine.Delete(0,nPos+1);
					}
					rComboBox.AddString("거래량차트");
//					rxlistCtrl.InsertItem(nItem, "");
//					if(nItem%2) // odd
//					{
//						rxlistCtrl.SetItemText(nItem, 0, strAItem);
//					}
//					else		// even
//					{
//						rxlistCtrl.SetItemText(nItem, 0, strAItem,RGB(0,0,0),CRGRID);					
//					}				
					nItem++;
				}
			}		
		}
	}

	rComboBox.SetCurSel(0);
}

void CChartMng::SetParentMultiChartClassPointer(long clsPointer)
{
	m_lMltchrtDlgParentClsPointer = clsPointer;
}


// gm0604 added 2005.7.21 Start To Receive System Trading Message
BOOL CChartMng::OnSTDataManagerMessaggeAll()
{
	EACH_CHARTJOB_START()
		pChartItem->ReceiveFromSTDataManager();
	EACH_CHARTJOB_END()
	return TRUE;
}
// gm0604 added 2005.7.21 End To Receive System Trading Message


void CChartMng::GetChartAllCode(CStringArray &arrCode)
{
	CString strCode, csCode, csName;
	BOOL bCheck1, bCheck2;


	arrCode.RemoveAll();
	
	EACH_CHARTJOB_START()
		if(pChartItem)
		{
			strCode = _T("");
			strCode = pChartItem->GetDataCode();
			strCode.TrimLeft();		strCode.TrimRight();
			
			// 상품선물옵션차트 - ojtaso (20071001)
			if(m_nMarketType == CMasterTypeInfo::FUTOPT_CHART)//@유진삭제 || m_nMarketType == COMMODITY_CHART)
			{
				bCheck1 = bCheck2 = TRUE;

//나중에 추가				
//				// 선물
//				m_pDBMgr->GetCodeName(CODE_FUTURE, strCode, csName);
//				if(!csName.GetLength())		bCheck1 = FALSE;					
//
//				//옵션
//				m_pDBMgr->GetCodeName(CODE_OPTION, strCode, csName);
//				if(!csName.GetLength())		bCheck2 = FALSE;

				if(!bCheck1 && !bCheck2)
				{
					strCode = _T("");
					pChartItem->m_strReplayTime = _T("복기가 되지 않는 종목입니다");					
					pChartItem->SetReplayChart_RealData("복기가 되지 않는 종목입니다");
				}
			}

			if( strCode.GetLength())
				arrCode.Add(strCode);
		}
	EACH_CHARTJOB_END()
}

//===================================================================================
// 환경 적용
//===================================================================================
void CChartMng::SetTimeShareOnlyOne(BOOL bUseage)
{
	CChartItem* pChartItem = NULL;
	pChartItem = GetpChartItemSelected();	
	if(pChartItem)
		pChartItem->m_CfgInfo.bRecvTimeShare = bUseage ? true : false;

	GetpChartItemSelected()->EnableReceivingTimeShare(bUseage);
}

void CChartMng::SetEnableBoundMarkOnlyOne(BOOL bUseage)
{	
	CChartItem* pChartItem = NULL;
	pChartItem = GetpChartItemSelected();	
	if(pChartItem)
		pChartItem->m_CfgInfo.bShowHLBar = bUseage  ? true : false ;

	GetpChartItemSelected()->EnableBoundMark(bUseage);	
}

void CChartMng::SetShowExRightOnlyOne(BOOL bUseage)
{
	CChartItem* pChartItem = NULL;
	pChartItem = GetpChartItemSelected();
	if(pChartItem->GetType() == DAY_DATA_CHART)
	{
		pChartItem->ShowLock(bUseage);
	}
	else
	{
		pChartItem->ShowLock(FALSE);
	}	
}

void CChartMng::SetPriceLog(BOOL bLog)	//로그 표시
{
	//SendMessage Input Bar
	if(m_pInputBarDlg)
	{
		// 이 함수에서 상단 툴바를 갱신한 후 CChartMng로 UMSG_USE_LOG 를 다시 보낸다.
		m_pInputBarDlg->SendMessage(UMSG_USE_LOG, (long)bLog);
	}
}

void CChartMng::OnChartEnvSetupOK()
{
	
}
//===================================================================================

void CChartMng::SetAllChartCfgSync()
{
	CHART_CONFIG_INFO *pInfo = NULL;
	pInfo = GetpChartItemSelected()->GetChartConfigInfo();

	EACH_CHARTJOB_START()
		pChartItem->SetChartConfigInfo(*pInfo);
	EACH_CHARTJOB_END()
}

bool CChartMng::GetImaginaryCandleToOutside(bool& bImaginaryCandle)// 허수봉설정.
{
	CHART_CONFIG_INFO *pInfo = NULL;
	pInfo = GetpChartItemSelected()->GetChartConfigInfo();
	if(pInfo == NULL) return false;
	bImaginaryCandle = pInfo->bImaginaryCandle;
	return true;
}

// (2008/11/2 - Seung-Won, Bae) for Gap Revision.
bool CChartMng::GetUseGapRevisionToOutside(bool& bUseGapRevision)	// 갭보정
{
	CHART_CONFIG_INFO *pInfo = NULL;
	pInfo = GetpChartItemSelected()->GetChartConfigInfo();
	if(pInfo == NULL) return false;
	bUseGapRevision = pInfo->bUseGapRevision;
	return true;
}

bool CChartMng::GetShowHighLowBoundToOutside(bool& bShow)			// 상/하한가 표시
{
	CHART_CONFIG_INFO *pInfo = NULL;
	pInfo = GetpChartItemSelected()->GetChartConfigInfo();
	if(pInfo == NULL) return false;
	bShow = pInfo->bShowHLBar;
	return true;
}

bool CChartMng::GetShowExRightDividendToOutside(bool& bShow)		// 권리락/배당락 표시
{
	CHART_CONFIG_INFO *pInfo = NULL;
	pInfo = GetpChartItemSelected()->GetChartConfigInfo();
	if(pInfo == NULL) return false;
	bShow = pInfo->bShowRight;
	return true;
}

bool CChartMng::GetTimeLinkageToOutside(bool& bTimeLinkage)		// 시간연동 수신
{
	CHART_CONFIG_INFO *pInfo = NULL;
	pInfo = GetpChartItemSelected()->GetChartConfigInfo();
	if(pInfo == NULL) return false;
	bTimeLinkage = pInfo->bRecvTimeShare;
	return true;
}

bool CChartMng::GetAutoSaveToOutside(bool& bAutoSave)				// 차트종료시 자동 저장 유무
{
	CHART_CONFIG_INFO *pInfo = NULL;	
	pInfo = GetpChartItemSelected()->GetChartConfigInfo();
	if(pInfo == NULL) return false;
	bAutoSave = pInfo->bAutoSave;
	return true;
}
/*
bool CChartMng::GetShowCurrentPriceToOutside(bool& bShow)
{
	CHART_CONFIG_INFO *pInfo = NULL;	
	pInfo = GetpChartItemSelected()->GetChartConfigInfo();
	if(pInfo == NULL) return false;
	bShow = pInfo->bShowCurPrice;
	return true;
}*/


//void CChartMng::SetShowCurrentPriceToOutside(const bool bShow)
//{
//	CChartItem* pChartItem = NULL;
//	pChartItem = GetpChartItemSelected();
//	pChartItem->SetShowCurrentPriceToOutside(bShow);
//}

void CChartMng::SetAutoSaveToOutside(const bool bAutoSave)
{
	CChartItem* pChartItem = NULL;
	pChartItem = GetpChartItemSelected();
	pChartItem->SetAutoSaveToOutside(bAutoSave);
}

void CChartMng::CreateFloatingHogaWnd(BOOL bForce /* = FALSE */)
{
		if( ((CStdDialog*)GetParent())->m_nChartType==MINI_MULTI_CHART ||
			((CStdDialog*)GetParent())->m_nChartType==MINI_SINGLE_CHART ) 
			return;
		if(m_nMarketType != CMasterTypeInfo::STOCK_CHART) return;
		
		//==============================================================================================
		//2005. 06. 23 현재 분할된 차트의 갯수 1 * 1 상태일 경우 Floating Hoga 창을 생성한다.
		//==============================================================================================
		DestroyFloatingHogaWnd();
	
		m_pFloatingDlg = NULL;
		m_pFloatingDlg = new CFloatingHogaDlg;	
		if(m_pFloatingDlg == NULL) return;

		m_pFloatingDlg->m_pMainDlg = this;
		m_pFloatingDlg->Create(IDD_DLG_FLOATING, this);	
		m_pFloatingDlg->SetParent(this);

		m_pFloatingDlg->SetWindowPos(&CWnd::wndTop, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE |SWP_SHOWWINDOW);

/*		
		//================================================================
		//이전 위치를 파일에서 읽어온다.
		//================================================================		
		CString strCfgFile = m_strUserDir + STOCK_ENVINFOFILE;
		char szTemp[10] = {0,};

		CRect rect, rectFloating;
		int  nleft, nTop;
		rect		.SetRectEmpty();
		rectFloating.SetRectEmpty();

		::GetPrivateProfileString("FloatingWnd",	"left"	,	"-1",szTemp,sizeof(szTemp),strCfgFile);
		nleft		= atoi(szTemp);

		::GetPrivateProfileString("FloatingWnd",	"top"	,	"-1",szTemp,sizeof(szTemp),strCfgFile);
		nTop		= atoi(szTemp);		

		m_ChartMng.GetWindowRect(rect);
		ScreenToClient(rect);					
		
		m_pFloatingDlg->GetWindowRect(rectFloating);
		ScreenToClient(rectFloating);


		int nWidth	= 0;
		int nHeight	= 0;
		//저장된 위치 정보가 없는 경우 
		if( (nleft == -1) && (nTop == -1) )
		{
			CRect extToolRect, splitRect;
			if(m_pExtInputBarDlg)
			{
				m_pExtInputBarDlg->GetWindowRect(extToolRect);
				ScreenToClient(extToolRect);
				rectFloating.top	= extToolRect.top;
				
				m_xSplitter1.GetWindowRect(splitRect);
				ScreenToClient(splitRect);
				rectFloating.left = splitRect.right;
			}		
		}
		else //저장된 위치 정보가 있는 경우 
		{	

			CRect rectStatic;
			rectFloating.left	= nleft;

			m_pFloatingDlg->m_staticSymbol.GetWindowRect(rectStatic);

			rectFloating.right	= rectFloating.left + rectStatic.Width();
			rectFloating.top	= nTop;
			rectFloating.bottom	= rectFloating.top + rectStatic.Height();
		}
			
		m_pFloatingDlg->MoveWindow(rectFloating);
		
	}*/	
	//==============================================================================================
	
}

void CChartMng::DestroyFloatingHogaWnd()
{	
#ifdef _WRITELOG
	((CStdDialog*)GetParent())->WriteToStringLog("CChartMng::DestroyFloatingHogaWnd::Begin", _T("멀티차트 종료"), 13);
#endif

	if(m_pFloatingDlg != NULL)
	{
		if(m_pFloatingDlg->GetSafeHwnd())
		{

			// Real 해제 
			if(m_pFloatingDlg->m_strOldCode.GetLength() == CODE_STOCK_LENGTH)
				((CStdDialog*)GetParent())->Request_Disconnect_StockRealHogaData(m_pFloatingDlg->m_strOldCode);

			if(m_pFloatingDlg->m_strCode.GetLength() == CODE_STOCK_LENGTH)
				((CStdDialog*)GetParent())->Request_Disconnect_StockRealHogaData(m_pFloatingDlg->m_strCode);			

			CRect floatRect;
			floatRect.SetRectEmpty();
			m_pFloatingDlg->GetWindowRect(floatRect);
			ScreenToClient(floatRect);
			
			m_pFloatingDlg->DestroyWindow();
			
			delete m_pFloatingDlg;
			m_pFloatingDlg = NULL;
		}
	}

#ifdef _WRITELOG
	((CStdDialog*)GetParent())->WriteToStringLog("CChartMng::DestroyFloatingHogaWnd::End", _T("멀티차트 종료"), 13);
#endif
}

void CChartMng::MoveFloatingHogaDlg()
{
	if(m_pFloatingDlg == NULL) 
	{		
		return;
	}
	if(m_pFloatingDlg->GetSafeHwnd())
	{
		CRect rectChart, rectFloat;
		/*CChartItem* pSelChartItem = GetpChartItemSelected();
		if(pSelChartItem == NULL) 
		{
			m_pFloatingDlg->Invalidate();
			return;
		}
		pSelChartItem->GetWindowRect(rectChart);
		*/
		GetWindowRect(rectChart);

		ScreenToClient(rectChart);

		if(m_pRectFloatingHoga == NULL)
		{
			m_pFloatingDlg->Invalidate();
			return;
		}
		if(m_pRectFloatingHoga->left > m_pRectFloatingHoga->right)
		{
			m_pFloatingDlg->Invalidate();
			return;
		}
		if(m_pRectFloatingHoga->top  > m_pRectFloatingHoga->bottom) 
		{
			m_pFloatingDlg->Invalidate();
			return;
		}

		m_pFloatingDlg->GetWindowRect(m_pRectFloatingHoga);
		ScreenToClient(m_pRectFloatingHoga);

		//======================================================================
		int nWidth	= m_pRectFloatingHoga->Width();
		int nHeight = m_pRectFloatingHoga->Height();

//		///////////////////////////////////////// gm0604 changed 2005.10.10
//		BOOL bIsSystemTradingMode = FALSE;
//		if(m_pChartItems[0][0])
//		{
//			bIsSystemTradingMode = m_pChartItems[0][0]->GetPtrST();
//		}	
//		if(bIsSystemTradingMode)
//		{
//			m_pRectFloatingHoga->top = rectChart.top + HEIGHTOFCAPTION*2+2;
//		}
//		else
//		{
		m_pRectFloatingHoga->top = rectChart.top + HEIGHTOFCAPTION+2 + 16;
//		}				
		/////////////////////////////////////////

		m_pRectFloatingHoga->left = rectChart.left + 3;
		m_pRectFloatingHoga->right = m_pRectFloatingHoga->left + nWidth;
		m_pRectFloatingHoga->bottom = m_pRectFloatingHoga->top + nHeight;
		//======================================================================	
		
	
		m_pFloatingDlg->MoveWindow(m_pRectFloatingHoga);
		m_pFloatingDlg->SetWindowPos(&CWnd::wndTop, m_pRectFloatingHoga->left,m_pRectFloatingHoga->top, nWidth,nHeight, SWP_NOMOVE | SWP_NOSIZE |SWP_SHOWWINDOW);
		//m_pFloatingDlg->SendMessage(WM_SIZE, 0, MAKELPARAM(nWidth, nHeight));

		//::InvalidateRect(m_pFloatingDlg->GetSafeHwnd(), (CONST RECT*)m_pRectFloatingHoga, TRUE);
		//m_pFloatingDlg->Invalidate();
		//m_pFloatingDlg->UpdateWindow();

//		OutputDebugString("Move ChartInval");
	}
}

CString CChartMng::GetCurSelChartItemJMName()
{
	if(!GetpChartItemSelected()) return _T("");
	return GetpChartItemSelected()->GetKorName();
}


void CChartMng::GetStartEndDateAtChartMng(double& dStartDate, double& dEndDate)
{
	if(!GetpChartItemSelected())
	{
		dStartDate = dEndDate = 0;
		return;
	}

	GetpChartItemSelected()->GetStartEndDateOnView(&dStartDate,&dEndDate);

}

void CChartMng::ApplySyncEnv_OnlyOne()
{	
	//환경동기화 적용 상태인지 확인
	if( m_pExtInputBarDlg->SendMessage(UMSG_GETENVSINK) == 0 ) return;	

	// 1 * 1 이면 Return;
	if( m_lCols * m_lRows <= 1) return;

	CChartItem* pEmptyChart = NULL	; //환경동기화가 적용될 차트
	pEmptyChart = GetpChartItemSelected();
	if(pEmptyChart == NULL) return;

	//비어있는 데이터 인지 체크
	if(pEmptyChart->m_bReceivedDataChart) return;
	



	m_bEmptyChartForSinkCfg = TRUE;
	
	// 기준이 되는 차트를 찾는다.
	// 기준차트의 요건은 조회 시간단위(틱/분/일/주/월/년)가 
	// 동일하면 무조건 OK.
	// 다르면 환경동기화 안함.
	CChartItem* pBaseChart	= NULL	;	
	EACH_CHARTJOB_START()
		char chBaseType		= pChartItem->GetType();  //기준 차트의 조회 시간단위 Get
		char chEmptyType	= pEmptyChart->GetType(); //빈 차트의 조회 시간단위 Get
		if(lstrlen(&chEmptyType) == 0)				  //지정된 조회 타입이 없으면 	
			chEmptyType	= pEmptyChart->GetTypeRequested();//요청하는 타입을 Get

		if(lstrlen(&chEmptyType) == 0) return; //그래도 가져오는게 없으면 Return

		if( chBaseType == chEmptyType ) //기준 차트를 찾은 경우 
		{			
			if(pChartItem->m_bReceivedDataChart != TRUE)  //조회 타입이 같더라도 데이터가 없는 경우 무시한다.
				continue;

			pBaseChart = pChartItem;	//기준 차트 Pointer를 저장 
			goto Finded;			  	//이중 For문이라..goto문을 써서 빠져 나간다.
		}
	EACH_CHARTJOB_END()

Finded:

	if(pBaseChart)
	{
		//TR조회가 필요한 경우 - 시장지표 일때.
		//m_strArrayCodeInLoop.RemoveAll();    //TR조회시 필요한 종목코드 저장 
		//m_strArrayTRCodeInLoop.RemoveAll();  //TR조회로서 동기화가 필요한 TR내역 저장 

		//호가 요청을 못하도록 한다.
		if(m_pInputBarDlg)
			m_pInputBarDlg->SendMessage(UMSG_ENABLE_SENDHOGATR, FALSE, 0);

		//기준이 되는 차트의 정보를 저장한다.
		pBaseChart->SaveandOpenFromFile("","temp.tmp",FALSE,TRUE);

		int	 nlCntForSyncEnv						= pBaseChart->GetCntOfItemData();				//
		char chTypeForSyncEnv						= pBaseChart->GetType();						//틱분일		

// (2008/11/2 - Seung-Won, Bae) Do not support sync for all ChartItem.
//		BOOL bNormalizedMinuteTimeRuleForSyncEnv	= pBaseChart->GetBNormalizedMinuteTimeRule();	//허수봉
		
		int  nChartMngs_ChartDataNumericValue		= pBaseChart->GetChart_NumericValueType();		//\, $, %

		//chart TR Data를 제외한 항목들이 있는 경우..별도로 저장한다.
		// lPos = 1 부터 시작하는 이유는 
		// 0번째에는 차트 데이터 이고
		// 1번째 부터 동기화 시 TR로 조회가 필요한 시장지표에 해당하는 정보들이 
		// 들어있기 때문이다.
		CStringArray strArrayCode, strArrayTRCode;
		for(long lPos = 1;lPos<nlCntForSyncEnv;lPos++)
		{
			strArrayCode.Add(pBaseChart->GetDataCode(lPos));
			strArrayTRCode.Add(pBaseChart->GetDataTRCode(lPos));
		}

		pEmptyChart->ClearDataExceptPrimaryData();
		
		
		if(pEmptyChart->GetChart_NumericValueType()!=nChartMngs_ChartDataNumericValue)	
		{
			pEmptyChart->SetChartNumericValue(nChartMngs_ChartDataNumericValue);
		}		

// (2008/11/2 - Seung-Won, Bae) Do not support sync for all ChartItem.
//		if(pEmptyChart->GetBNormalizedMinuteTimeRule()!=bNormalizedMinuteTimeRuleForSyncEnv)
//		{
//			pEmptyChart->SetBNormalizedMinuteTimeRule(bNormalizedMinuteTimeRuleForSyncEnv);
//		}

		CString  strTrCode;
		for(lPos = 0;lPos<(nlCntForSyncEnv-1);lPos++)
		{
			//lTrCode = atoi(strArrayTRCode.GetAt(lPos));
			strTrCode = strArrayTRCode.GetAt(lPos);
			pEmptyChart->SetCodeForQuery(strArrayCode.GetAt(lPos),strTrCode,FALSE);
		}	
	}	
}

void CChartMng::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1)
	{
		KillTimer(1);
		PostMessage(UMSG_SIGNAL_CLICK);
	}
	CWnd::OnTimer(nIDEvent);
}

void CChartMng::SetSystemTradingMode(BOOL bEnable, LPCTSTR lpMessage/*=NULL*/)
{
	m_bIsSystemTradingMode = bEnable;
	if(!bEnable)
	{
		ASSERT(lpMessage);
		long lPtrFree = atol(lpMessage);
		BOOL bIsFree = FALSE;
		EACH_CHARTJOB_START()
			if(pChartItem->GetPtrST()==lPtrFree)
			{
				bIsFree = TRUE;
				break;
			}
		EACH_CHARTJOB_END()
		if(bIsFree)
		{
			EACH_CHARTJOB_START()
				pChartItem->ResetOCX();
			EACH_CHARTJOB_END()
			MoveCharts();
		}

	}
}


void CChartMng::SetpChartCfgWnd(char chType, CWnd* pWnd)
{
	if ( m_pSecCfgChart	== pWnd ) m_pSecCfgChart = NULL;
	if ( m_pTickCfgChart == pWnd ) m_pTickCfgChart = NULL;
	if ( m_pMinCfgChart	== pWnd ) m_pMinCfgChart = NULL;
	if ( m_pDayCfgChart	== pWnd ) m_pDayCfgChart = NULL;
	if ( m_pHourCfgChart == pWnd ) m_pHourCfgChart = NULL;
	if ( m_pWeekCfgChart == pWnd ) m_pWeekCfgChart = NULL;
	if ( m_pMonthCfgChart == pWnd ) m_pMonthCfgChart = NULL;
	if ( m_pYearCfgChart == pWnd ) m_pYearCfgChart = NULL;

	switch(chType)
	{
//		case	SEC10_DATA_CHART:	
//		case	SEC30_DATA_CHART:	m_pSecCfgChart	= pWnd; break;
		case	SEC_DATA_CHART	:	m_pSecCfgChart	= pWnd;	break;
		case	TICK_DATA_CHART	:	m_pTickCfgChart = pWnd; break;			
		case	MIN_DATA_CHART	:	m_pMinCfgChart	= pWnd; break;	
		case	DAY_DATA_CHART	:	m_pDayCfgChart	= pWnd; break;					
		case	HOUR_DATA_CHART	:	m_pHourCfgChart = pWnd; break;
		case	WEEK_DATA_CHART	:	m_pWeekCfgChart = pWnd; break;
		case	MONTH_DATA_CHART:	m_pMonthCfgChart= pWnd; break;
		case	YEAR_DATA_CHART	:	m_pYearCfgChart = pWnd; break;
//		case	TICK_DATA_CHART_HOGA	:	m_pHogaCfgChart = pWnd; break;			
	}
}

CChartItem* CChartMng::GetpChartCfgWnd(char chType)
{
	switch(chType)
	{
//		case	SEC10_DATA_CHART:	
//		case	SEC30_DATA_CHART:	return (CChartItem*)m_pSecCfgChart	; break;
		case	SEC_DATA_CHART	:	return (CChartItem*)m_pSecCfgChart	; break;
//		case	TICK_DATA_CHART_HOGA	:
		case	TICK_DATA_CHART	:
			{
				if ( m_pTickCfgChart )
				{
					int nPreType = ((CChartItem*)m_pTickCfgChart)->m_nCheHoga;
					int nNowType = GetCheHogaType();
					if ( nPreType == nNowType )
						return (CChartItem*)m_pTickCfgChart ; 
					else
						m_pTickCfgChart = NULL;
				}

				return (CChartItem*)m_pTickCfgChart ; 
			}
			break;			
		case	MIN_DATA_CHART	:	return (CChartItem*)m_pMinCfgChart	; break;	
		case	DAY_DATA_CHART	:	return (CChartItem*)m_pDayCfgChart	; break;					
		case	HOUR_DATA_CHART	:	return (CChartItem*)m_pHourCfgChart	; break;
		case	WEEK_DATA_CHART	:	return (CChartItem*)m_pWeekCfgChart ; break;
		case	MONTH_DATA_CHART:	return (CChartItem*)m_pMonthCfgChart; break;
		case	YEAR_DATA_CHART	:	return (CChartItem*)m_pYearCfgChart ; break;
		default					:	return NULL;
	}

	return NULL;
}

// 종목변경 및 주기변경시...
void CChartMng::Synchro_jipyo()
{
//	if(m_bSaveJipyoOneFile)
//		return;

// --> [Edit]  강지원 2008/11/07		( 추가모드일때도 지표적용되도록 한다... 이미 앞에서 한종목일때만 지표적용으로 막았기 때문에... )
	// 추가 모드일때 : 복수종목 - ojtaso (20070209)
//	if(!m_bChangeTypeMode || m_bAddMode )
//		return;
// <-- [Edit]  강지원 2008/11/07

	m_bExistSyncChart = FALSE;

	//환경동기화 적용 상태인지 확인
	//환경 동기화 상태면 적용 안한다.
	if( m_pExtInputBarDlg->SendMessage(UMSG_GETENVSINK) == 1 ) return;	

	CChartItem* pSyncChart = NULL	; //지표동기화가 적용될 차트
	pSyncChart = GetpChartItemSelected();
	if(pSyncChart == NULL) return;

	//m_bEmptyChartForSinkCfg = TRUE;
	
	// 기준이 되는 차트를 찾는다.
	// 기준차트의 요건은 조회 시간단위(틱/분/일/주/월/년)가 
	// 동일하면 무조건 OK.
	// 다르면 환경동기화 안함.
	
	char chOriType		= '\0';
	char chRqstType		= '\0';

	chOriType = pSyncChart->GetType(); //차트의 기존 조회된 시간단위 Get	
	chRqstType	= pSyncChart->GetTypeRequested();//요청하는 타입을 Get

	if(lstrlen(&chRqstType) == 0) return; //그래도 가져오는게 없으면 Return

//	if(chOriType == chRqstType)	return; //같은 타입이면 저장하거나 로드하지 않는다.

	//{{JS.Kim_20100903 Solomon	추세선 저장
	//if(chOriType == chRqstType && chOriType != TICK_DATA_CHART )	return; //같은 타입이면 저장하거나 로드하지 않는다.
	//if(chOriType == NULL)  chOriType = chRqstType;
	// 분석툴 종목별 저장 - ojtaso (20080728)
	BOOL bSaveAnalTool = GetSaveAnalTool();
	if(!bSaveAnalTool && chOriType == chRqstType) return; //같은 타입이면 저장하거나 로드하지 않는다.
	//}}

	CChartItem* pSaveChart	= NULL	;
	CChartItem* pLoadChart	= NULL	;

//	if(chOriType != NULL)
//		pSaveChart = GetpChartCfgWnd(chOriType	);
//	else
//	{
//		pSaveChart = GetpChartCfgWnd(chRqstType	);
//		chOriType = chRqstType;
//	}

	//pSaveChart = GetpChartCfgWnd(chOriType	);
	//>>20120212_JS.Kim 지표별로 저장인 경우 그때그때 파일에 저장되므로 이전에 읽었던 값으로 셋팅하면 안된다
	//pLoadChart = GetpChartCfgWnd(chRqstType	);
	//}}

	//CString sJipyoFileName, sJipyoFileName1, sJipyoSaveFileName;		
	//sJipyoFileName1 = ((CStdDialog*)GetParent())->m_strScreenNo;
	//if(m_nMarketType == STOCK_CHART)
	//	sJipyoFileName1 += "_Stock";
	//else if(m_nMarketType == FUTOPT_CHART)
	//	sJipyoFileName1 += "_FutOpt";
	//else if(m_nMarketType == UPJONG_CHART)
	//	sJipyoFileName1 += "_Upjong";
	//else if(m_nMarketType == FOREIGN_CHART)
	//	sJipyoFileName1 += "_Foreign";	
	//else if(m_nMarketType == ELW_CHART)
	//	sJipyoFileName1 += "_ELW";	

	//if(pSaveChart != NULL)
	//{	
	//	if(m_bSaveJipyoOneFile)
	//	{
	//		sJipyoSaveFileName = sJipyoFileName1 + "_";
	//	}
	//	else
	//	{
	//		switch(chOriType)
	//		{	
	//			case	SEC10_DATA_CHART:	
	//			case	SEC30_DATA_CHART:	sJipyoSaveFileName = sJipyoFileName1 + "_Sec_"	; break;
	//			case	TICK_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Tick_"	; break;			
	//			case	MIN_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Min_"	; break;	
	//			case	DAY_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Day_"	; break;	
	//			case	HOUR_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Hour_"	; break;
	//			case	WEEK_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Week_"	; break;
	//			case	MONTH_DATA_CHART:	sJipyoSaveFileName = sJipyoFileName1 + "_Month_"	; break;
	//			case	YEAR_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Year_"	; break;
	//			default					:	return ;
	//		}
	//	}

	//	sJipyoSaveFileName += JIPYOSYNC_FILE;		
	//}

	if(pSyncChart)
	{
		//TR조회가 필요한 경우 - 시장지표 일때.
		//m_strArrayCodeInLoop.RemoveAll();    //TR조회시 필요한 종목코드 저장 
		//m_strArrayTRCodeInLoop.RemoveAll();  //TR조회로서 동기화가 필요한 TR내역 저장 
// --> [Edit]  강지원 2008/10/27
		//	로드된 차트가 있으면 타이틀에 사용하는 RealDataItem을 새로 안만드는거 같아서 일단 막음... ( 2X2에서 종목 동기화후 일/주/월을 클릭했을때... User폴더에 저장되어있는 파일이 없을때 )
//		if(pLoadChart->GetSafeHwnd())
//		{
//			m_bExistSyncChart = TRUE;
//		}
// <-- [Edit]  강지원 2008/10/27


// --> [Edit]  강지원 2008/12/29		( 주기변경시 이쪽으로 들어오는데 여기서 호가창의 조회플래그를 막아버려서 조회가 안됨... 그래서 이부분 막아버림 )
//		//호가 요청을 못하도록 한다.
//		if(m_pInputBarDlg)
//			m_pInputBarDlg->SendMessage(UMSG_ENABLE_SENDHOGATR, FALSE, 0);	
// <-- [Edit]  강지원 2008/12/29

		//조회 이전 차트의 정보를 저장한다.
		if(pSyncChart->HasGraph("가격차트"))
		{						
			CString sSaveFileName = pSyncChart->GetOpenAndSaveFileName(FALSE, TRUE);
			pSyncChart->SaveandOpenFromFile("", (LPCTSTR)sSaveFileName, FALSE, TRUE);

			// 분석툴 종목별 저장 - ojtaso (20080728)
			BOOL bSaveAnalTool = GetSaveAnalTool();
			if(bSaveAnalTool)
			{
				// 2011.01.31 by SYS >> 분차트 YYYY(년) 추가 및 분석툴 공유 기능 추가
				//sSaveFileName = pSyncChart->GetOpenAndSaveFileName(TRUE, TRUE);
				
				sSaveFileName = pSyncChart->GetOpenAndSaveAnalFileName(TRUE, TRUE);
				pSyncChart->SaveandOpenFromAnalToolFile("", (LPCTSTR)sSaveFileName, FALSE, TRUE);

				// 2011.01.31 by SYS <<
			}
		}

		int	 nlCntForSyncEnv	= pSyncChart->GetCntOfItemData();

		//chart TR Data를 제외한 항목들이 있는 경우..별도로 저장한다.
		// lPos = 1 부터 시작하는 이유는 
		// 0번째에는 차트 데이터 이고
		// 1번째 부터 동기화 시 TR로 조회가 필요한 시장지표에 해당하는 정보들이 
		// 들어있기 때문이다.
		CStringArray strArrayCode, strArrayTRCode;
		for(long lPos = 1;lPos<nlCntForSyncEnv;lPos++)
		{
			strArrayCode.Add(pSyncChart->GetDataCode(lPos));
			strArrayTRCode.Add(pSyncChart->GetDataTRCode(lPos));
		}

		pSyncChart->ClearDataExceptPrimaryData();
		long lTrCode = 0;
		

		for(lPos = 0;lPos<(nlCntForSyncEnv-1);lPos++)
		{
			lTrCode = atoi(strArrayTRCode.GetAt(lPos));
			pSyncChart->SetCodeForQuery(strArrayCode.GetAt(lPos),strArrayTRCode.GetAt(lPos),FALSE);
		}	
	}
	
	if(pLoadChart == NULL)
	{
		//=======================================================================================
		CString sJipyoFileName, sJipyoFileName1, sJipyoSaveFileName;		
		sJipyoFileName1 = ((CStdDialog*)GetParent())->m_strScreenNo;
		//{{2007.05.30 by LYH 시장 구분 없이 같은 파일 읽음
//		if(m_nMarketType == STOCK_CHART)
//			sJipyoFileName1 += "_Stock";
//		else if(m_nMarketType == FUTOPT_CHART)
//			sJipyoFileName1 += "_FutOpt";
//		else if(m_nMarketType == UPJONG_CHART)
//			sJipyoFileName1 += "_Upjong";
//		else if(m_nMarketType == FOREIGN_CHART)
//			sJipyoFileName1 += "_Foreign";	
//		else if(m_nMarketType == ELW_CHART)
//			sJipyoFileName1 += "_ELW";	
		// 상품선물옵션차트 - ojtaso (20071001)
//		else if(m_nMarketType == COMMODITY_CHART)	
//			sJipyoFileName1 += "_Commodity";	

		if(m_bSaveJipyoOneFile)
		{
			sJipyoSaveFileName = sJipyoFileName1 + "_"	;
		}
		else
		{
			switch(chRqstType)
			{	
//				case	SEC10_DATA_CHART:	
//				case	SEC30_DATA_CHART:	sJipyoSaveFileName = sJipyoFileName1 + "_Sec_"	; break;
				case	SEC_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Sec_"	; break;
				case	TICK_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Tick_"	; break;			
				case	MIN_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Min_"	; break;	
				case	DAY_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Day_"	; break;					
				case	HOUR_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Hour_"	; break;
				case	WEEK_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Week_"	; break;
				case	MONTH_DATA_CHART:	sJipyoSaveFileName = sJipyoFileName1 + "_Month_"	; break;
				case	YEAR_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Year_"	; break;
//				case	TICK_DATA_CHART_HOGA	:	sJipyoSaveFileName = sJipyoFileName1 + "_Hoga_"	; break;
				default					:	return ;
			}
		}

// --> [Edit]  강지원 2008/11/03		( 체결/호가 구분에 따라서 지표 저장파일을 달리 해준다. )
		if ( GetCheHogaType() == 0 )
			sJipyoSaveFileName = "Che_" + sJipyoSaveFileName + JIPYOSYNC_FILE;
		else
			sJipyoSaveFileName = "Hoga_" + sJipyoSaveFileName + JIPYOSYNC_FILE;
//		sJipyoSaveFileName += JIPYOSYNC_FILE;
// <-- [Edit]  강지원 2008/11/03

		CFileFind ff;
		CString sTempPath;
		////{{JS.Kim_20100903 Solomon	추세선 저장
		sTempPath = m_strUserPath + "\\" + sJipyoSaveFileName;
		//sTempPath = m_strUserPath + "\\Chart\\" + sJipyoSaveFileName;
		////}}
		BOOL bResult = ff.FindFile(sTempPath);
		if(!bResult)
		{
			pSyncChart->ResetChart();
//			pSyncChart->ResetRQ();
		}
		else
		{
			//{{JS.Kim_20100903 Solomon	차트 바뀔때 분석툴 선택도 Reset한다.
			GetParent()->SendMessage(UMSG_SYNC_TOOLBAR, 0, 50000);
			//}}
			m_bExistSyncChart = TRUE; //파일이 있는 경우는 그 놈을 읽을 수 있도록 flag셋한다.
		}
		//=======================================================================================
	}
	
}


void CChartMng::SetBReplayChartMode(BOOL bReplayMode)
{
	m_bReplaychartMode = bReplayMode;	

	EACH_CHARTJOB_START()
		pChartItem->SetBReplayChartMode(bReplayMode);
	EACH_CHARTJOB_END()
}


void CChartMng::SetReplayRealTitle(CString strCode, CString strSpeed, TICK_DATA* pData)
{

	int nLen = 0;
	if(m_nMarketType == CMasterTypeInfo::STOCK_CHART) nLen = CODE_STOCK_LENGTH;
	else if(m_nMarketType == CMasterTypeInfo::FUTOPT_CHART) nLen = FUTOPT_LENGTH;
	// 상품선물옵션차트 - ojtaso (20071001)
//@유진삭제	else if(m_nMarketType == COMMODITY_CHART) nLen = COMMODITY_LENGTH;
	else return;

	CString strCnts, strTime, strCurPrice, strVolume, strPrePrice;
	CString strChartCode;
	CRect rectChart;
	EACH_CHARTJOB_START()
		strChartCode = pChartItem->GetDataCode();
		if(strChartCode.GetLength()==nLen&&strChartCode==strCode)
		{
			strTime.Format("%6.6s", pData->chDate);
			strCurPrice.Format("%10.10s", pData->chClosingPrice);
			strVolume.Format("%10.10s", pData->chVolume);
			

			pChartItem->SetRealData("자료일자",		atof(strTime)		, 0);
			pChartItem->SetRealData("종가",			atof(strCurPrice)	, 0);
			pChartItem->SetRealData("기본거래량",	atof(strVolume)		, 0);

			strTime.Format("%.2s:%.2s:%.2s", pData->chDate, pData->chDate + 2, pData->chDate + 4);
			strCnts.Format("%s %s %s ", 
							pChartItem->GetKorName(),	//종목명							 
							pChartItem->GetReplay_CurDate(), //진행날짜
							strTime //진행시간							
							);
		
			pChartItem->SetReplayChart_RealData(strCnts, strCurPrice, strVolume);


/*
			//=============================================================================
			// 차트로 Real 보낸다.
			//=============================================================================		
			DrdsData r_DrdsData;
			memset(&r_DrdsData, 0x20, sizeof(DrdsData));	
			
			if(m_nMarketType == STOCK_CHART)
			{
				r_DrdsData.nID = 100;
				memset(r_DrdsData.DataName, 0x00,  32);
				memcpy(r_DrdsData.DataName, "58", 2);

				memset(r_DrdsData.KeyName, 0x00, 32);
				memcpy(r_DrdsData.KeyName, (LPTSTR)(LPCTSTR)strChartCode, strChartCode.GetLength());

				//=======================================================================================
				//자료일자 
				CString sTime;
				sTime.Format("%6.6s", pData->chDate);
				r_DrdsData.Len = 11;
				memset(r_DrdsData.FieldName, 0x00,  32);
				memcpy(r_DrdsData.FieldName, "2", 1);
				memcpy(r_DrdsData.Data, (LPTSTR)(LPCTSTR)sTime, 11 );
				pChartItem->SendMessage(WM_DRDS_DATA, sizeof(r_DrdsData), (LPARAM)(const char*)&(r_DrdsData));				

				//=======================================================================================
				//종가 
				CString sPrice;
				sPrice.Format("%10.10s", pData->chClosingPrice);
				r_DrdsData.Len = 12;
				memset(r_DrdsData.FieldName, 0x00,  32);
				memcpy(r_DrdsData.FieldName,"6", 1);
				memcpy(r_DrdsData.Data, (LPTSTR)(LPCTSTR)sPrice, 12 );
				pChartItem->SendMessage(WM_DRDS_DATA, sizeof(r_DrdsData), (LPARAM)(const char*)&(r_DrdsData));				

				//=======================================================================================
				//거래량 
				CString sVolume;
				sVolume.Format("%10.10s", pData->chVolume);
				r_DrdsData.Len = 13;
				memset(r_DrdsData.FieldName, 0x00,  32);
				memcpy(r_DrdsData.FieldName,"10", 2);
				memcpy(r_DrdsData.Data, (LPTSTR)(LPCTSTR)sVolume, 13 );
				pChartItem->SendMessage(WM_DRDS_DATA, sizeof(r_DrdsData), (LPARAM)(const char*)&(r_DrdsData));

			}
			else if(m_nMarketType == FUTOPT_CHART)
			{
				r_DrdsData.nID = 100;
				memset(r_DrdsData.DataName, 0x00,  32);
				
				CString csName = _T("");

				m_pDBMgr->GetCodeName(CODE_FUTURE, strChartCode, csName);				

				if(csName.GetLength())
					memcpy(r_DrdsData.DataName, "65", 2); //선물
				else				
					memcpy(r_DrdsData.DataName, "67", 2); //옵션

				memset(r_DrdsData.KeyName, 0x00, 32);
				memcpy(r_DrdsData.KeyName, (LPTSTR)(LPCTSTR)strChartCode, strChartCode.GetLength());

				//=======================================================================================
				//자료일자 
				CString sTime;
				sTime.Format("%6.6s", pData->chDate);
				r_DrdsData.Len = 11;
				memset(r_DrdsData.FieldName, 0x00,  32);
				memcpy(r_DrdsData.FieldName, "2", 1);
				memcpy(r_DrdsData.Data, (LPTSTR)(LPCTSTR)sTime, 11 );
				pChartItem->SendMessage(WM_DRDS_DATA, sizeof(r_DrdsData), (LPARAM)(const char*)&(r_DrdsData));				

				//=======================================================================================
				//종가 
				CString sPrice;
				sPrice.Format("%10.10s", pData->chClosingPrice);
				r_DrdsData.Len = 12;
				memset(r_DrdsData.FieldName, 0x00,  32);
				memcpy(r_DrdsData.FieldName,"3", 1);
				memcpy(r_DrdsData.Data, (LPTSTR)(LPCTSTR)sPrice, 12 );
				pChartItem->SendMessage(WM_DRDS_DATA, sizeof(r_DrdsData), (LPARAM)(const char*)&(r_DrdsData));				

				//=======================================================================================
				//거래량 
				CString sVolume;
				sVolume.Format("%10.10s", pData->chVolume);
				r_DrdsData.Len = 13;
				memset(r_DrdsData.FieldName, 0x00,  32);
				memcpy(r_DrdsData.FieldName,"10", 2);
				memcpy(r_DrdsData.Data, (LPTSTR)(LPCTSTR)sVolume, 13 );
				pChartItem->SendMessage(WM_DRDS_DATA, sizeof(r_DrdsData), (LPARAM)(const char*)&(r_DrdsData));
			}
		
			pChartItem->SendMessage(WM_DRDS_DATA_PACKET_END, 0, 0 );
		*/	
			//=============================================================================
		}
	EACH_CHARTJOB_END()
}

//================================================================================
// 복기진행 중 날짜가 바뀌었음을 차트에 알려준다.
// 2005. 12. 08 추가 
//================================================================================
void CChartMng::SetReplayChangedDate(CString strCode, CString strBojoMsg)
{
	CString strChartCode;
	EACH_CHARTJOB_START()
		strChartCode = pChartItem->GetDataCode();
		if(strChartCode.GetLength()&&strChartCode==strCode)
		{
//			OutputDebugString("복기 날짜가 바뀌었다");
//			OutputDebugString("복기 " + strBojoMsg);
			pChartItem->InitString_ForReplay(); //06.01.12 added by sy, nam

			pChartItem->SetReplay_PrePrice(strBojoMsg);

			pChartItem->SetReplay_CurDate(strBojoMsg);
			pChartItem->CDRChartOcx::SetPacketHelpMessage((LPCTSTR)strBojoMsg, false);
		}
	EACH_CHARTJOB_END()
}

//================================================================================
// 차트가 로드시 필요한 TR정보들을 모두 받
//================================================================================
void CChartMng::SetFocusToJMEdit_At_LoadEnd()
{		
	if(!m_bLastEnvUpdated)
	{
		CChartItemData * pItemData = NULL;
		int nDataSize = 0;
		int nCntComplete = 0;
		int nCntComplete2 = 0;
		EACH_CHARTJOB_START()								
			nDataSize = 0; nCntComplete = 0;
			nDataSize = pChartItem->m_arrayMarketTRData.GetSize();							
			for(int i = 0; i < nDataSize; i++)
			{
				pItemData = NULL;
				pItemData = pChartItem->m_arrayMarketTRData.GetAt(i);
				if(pItemData == NULL) continue;

				if( pItemData->IsFilled()) 
					nCntComplete++;								
			}
		
			if(nCntComplete == nDataSize) //완료가 다 되었을 경우....
			{
				nCntComplete2++;
				//m_pInputBarDlg->SendMessage(UMSG_EDIT_FOCUS);
				//m_bLastEnvUpdated = TRUE;
			}
		EACH_CHARTJOB_END()

		if(nCntComplete2 == m_lCols * m_lRows)
		{
			if(!m_bIsInLoop) //로드가 다 끝났을 때만 Focus가 가게 한다.
			{
				m_bLastEnvUpdated = TRUE;
				if(m_pInputBarDlg)
					m_pInputBarDlg->SendMessage(UMSG_EDIT_FOCUS);		
			}
		}
	}
}


BOOL CChartMng::CheckDataTypeForReplay()
{
	EACH_CHARTJOB_START()
		if(pChartItem->GetChart_NumericValueType() != WON_TYPE)
			return FALSE;
	EACH_CHARTJOB_END()

	return TRUE;
}

void CChartMng::SetEndData(LPCSTR szMessage)
{
	GetParent()->SendMessage(UMSG_SENDTOMSGBOX,(WPARAM)szMessage,(LPARAM)GetpChartItemSelected());
//	InitInLoop();
}

//-----------------------------------------------------------------------------
// Author		: Young-Hee, Lee	Date :2006/10/2
// Reason		: 일주월분 차트
//-----------------------------------------------------------------------------
BOOL CChartMng::LoadDWMMChart(BOOL bLoad)
{
	if( GetSafeHwnd() && GetpChartItemSelected())
	{
		short nType = GetpChartItemSelected()->GetCheHogaChange();
		if ( nType != 0 )
		{
			MessageBox("호가 차트일경우 1 X 1 만 지원합니다.");
			return FALSE;
		}
	}

	CString strCode;
	//chType: 틱(0)/분(1)/일(2)/주(3)/월(4)
	CChartItem* pChartItem;
	strCode = GetpChartItemSelected()->GetDataCode();
	m_pExtInputBarDlg->SendMessage(UMSG_CLEARLINK);	

	//2007.02.06 by LYH 단일차트인 경우 일주월분 나뉜 경우 다시 하나차트로 되도록 수정
	if(((CStdDialog*)GetParent())->m_nChartType==SINGLE_CHART)
	{
		if(m_lRows != 1 && m_lCols != 1)
			bLoad = FALSE;
	}
	//}}
	if(bLoad)
	{
		//{{2007.06.25 by LYH 종목추가 모드인 경우 변경모드로 수정
		int nResult = GetParent()->SendMessage(UMSG_GET_CHARTMODE, 1);
		if(nResult != CHANGE_CHART &&  nResult != ADD_NEW_CHART)
			((CInputBarDlg*)m_pInputBarDlg)->SetMode(CHANGE_CHART);
		//}}

		SetMatrix(2,2);
		m_pExtInputBarDlg->SendMessage(UMSG_SETJONGMOKLINK, TRUE);	
		m_bJongmokLink = TRUE;

		//{{ JS.Kim_20100901 solomon_74	일주월분 순서를 분일주월로 변경
		if(m_pChartItems[0][0]->GetSafeHwnd())
		{
			pChartItem = m_pChartItems[0][0];
			pChartItem->SetType(MIN_DATA_CHART);
			
			// (2008/9/25 - Seung-Won, Bae) Set for Multiple ItemCode
			if( 1 < pChartItem->GetChartItemCount())
			{
				ST_SELCHART* pSelChart = NULL;
				pChartItem->GetSelChart( "DEFAULT", pSelChart, TRUE);
				if( pSelChart) pSelChart->m_cType = MIN_DATA_CHART;
			}
		}

		if(m_pChartItems[0][1]->GetSafeHwnd())
		{
			pChartItem = m_pChartItems[0][1];
			pChartItem->SetType(DAY_DATA_CHART);
			
			// (2008/9/25 - Seung-Won, Bae) Set for Multiple ItemCode
			if( 1 < pChartItem->GetChartItemCount())
			{
				ST_SELCHART* pSelChart = NULL;
				pChartItem->GetSelChart( "DEFAULT", pSelChart, TRUE);
				if( pSelChart) pSelChart->m_cType = DAY_DATA_CHART;
			}
		}

		if(m_pChartItems[1][0]->GetSafeHwnd())
		{
			pChartItem = m_pChartItems[1][0];
			pChartItem->SetType(WEEK_DATA_CHART);
			
			// (2008/9/25 - Seung-Won, Bae) Set for Multiple ItemCode
			if( 1 < pChartItem->GetChartItemCount())
			{
				ST_SELCHART* pSelChart = NULL;
				pChartItem->GetSelChart( "DEFAULT", pSelChart, TRUE);
				if( pSelChart) pSelChart->m_cType = WEEK_DATA_CHART;
			}
		}

		if(m_pChartItems[1][1]->GetSafeHwnd())
		{
			pChartItem = m_pChartItems[1][1];
			pChartItem->SetType(MONTH_DATA_CHART);
			
			// (2008/9/25 - Seung-Won, Bae) Set for Multiple ItemCode
			if( 1 < pChartItem->GetChartItemCount())
			{
				ST_SELCHART* pSelChart = NULL;
				pChartItem->GetSelChart( "DEFAULT", pSelChart, TRUE);
				if( pSelChart) pSelChart->m_cType = MONTH_DATA_CHART;
			}
		}
		//}}
	}
	else
	{
		SetMatrix(1,1);

		if(m_pChartItems[0][0]->GetSafeHwnd())
		{
			pChartItem = m_pChartItems[0][0];
			pChartItem->SetType(DAY_DATA_CHART);
			
			// (2008/9/25 - Seung-Won, Bae) Set for Multiple ItemCode
			if( 1 < pChartItem->GetChartItemCount())
			{
				ST_SELCHART* pSelChart = NULL;
				pChartItem->GetSelChart( "DEFAULT", pSelChart, TRUE);
				if( pSelChart) pSelChart->m_cType = DAY_DATA_CHART;
			}
		}
	}

	CString strEndDate;
	char chType = NULL;
	strEndDate.Format("%d",GetpChartItemSelected()->GetEndDate());

	SetBInitFromExtToolBar(FALSE); //초기화이던 아니던 기본으로 False로 해준다.05.11.24

	InitInLoop();
	m_strEndDateInLoop = strEndDate;
	m_strCodeInLoop = strCode;
	m_lTypeInLoop = LOOPTYPE_NULL/*0*/;
	HeadInLoop();
	if(!GetpChartItemSelected()) return FALSE;
	GetpChartItemSelected()->Init_OverLapped();

	return TRUE;

}

//-----------------------------------------------------------------------------
// Author		: Young-Hee, Lee	Date :2006/12/6
// Reason		: 차트 전체초기화
//-----------------------------------------------------------------------------
void CChartMng::ResetChartAll()
{
//	SetMatrix(1,2);
//	ReplaceChart(0,0,1,2);
	//{1*1 차트로 초기화 요청
	ReplaceChart(0,0,m_lRowSelected,m_lColSelected);
	SetMatrix(1,1);

	CChartItem* pChartItem = NULL;
	pChartItem = GetpChartItemSelected();
	pChartItem->ClearChartData(0);

	//}
	if(m_pInputBarDlg)
		((CInputBarDlg*)m_pInputBarDlg)->SetMode(CHANGE_ITEM);

	m_bHideScale = FALSE;
	m_bHideInnerToolBar = FALSE;

// --> [Edit]  강지원 2008/11/03		( 설정 파일을 삭제한 후에 차트를 재설정한다. )

	//////////////////////////////////////////////////////////
	//초기화시 모든 파일 삭제 by LYH 2007.01.06
	CString sAllFileName = m_strUserPath + "\\" + "*" +((CStdDialog*)GetParent())->m_strScreenNo+ "_*";
	CFileFind ff;
	CString sFileName;
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

// --> [Edit]  강지원 2008/11/09		( 초기화할때 ChartItemData 를 삭제 )
//	ResetOCX();
// <-- [Edit]  강지원 2008/11/09

	m_pTickCfgChart		= NULL;
	m_pSecCfgChart		= NULL;
	m_pMinCfgChart		= NULL;
	m_pDayCfgChart		= NULL;
	m_pHourCfgChart		= NULL;
	m_pWeekCfgChart		= NULL;
	m_pMonthCfgChart	= NULL;
	m_pYearCfgChart		= NULL;
//	m_pHogaCfgChart		= NULL;

	m_bExistSyncChart	= FALSE;

	((CStdDialog*)GetParent())->MakeDrdsUnConnectUnAdviseFromCode(0, FALSE);
	((CStdDialog*)GetParent())->MakeDrdsUnConnectUnAdviseFromCode(1, FALSE);
	((CStdDialog*)GetParent())->MakeDrdsUnConnectUnAdviseFromCode(2, FALSE);
	// 차트 초기화때 모든 리얼 해제 : 복수종목 - ojtaso (20070621)
	((CStdDialog*)GetParent())->ResetChartAll();
// <-- [Edit]  강지원 2008/11/03

// --> [Edit]  강지원 2008/11/10		( 서버에서 데이터가 안내려오면 다른키나 조회가 안되므로 초기화 할때 풀어준다. )
	m_bInitFlag = TRUE;
	m_bIsInLoop = FALSE;
	((CStdDialog*)GetParent())->EnableWindowAll(TRUE);
// <-- [Edit]  강지원 2008/11/10

	if(pChartItem == NULL || !pChartItem->m_bReceivedDataChart) 
		return;

//	pChartItem->RemoveMapChartItem();
// --> [Edit]  강지원 2008/11/08		( N분 설정값도 초기화 하자... )
	pChartItem->SetTickNumber(_T("001"));
	pChartItem->SetSecNumber(_T("005"));
	pChartItem->SetMinNumber(_T("001"));
	pChartItem->SetDayNumber(_T("001"));
	pChartItem->SetHourNumber(_T("001"));
// <-- [Edit]  강지원 2008/11/08

	// --> [Edit]  강지원 2008/10/28	( Default 값을 실봉+허봉으로 설정하기 위해서 여기서 세팅해준다. )
	pChartItem->SetBNormalizedMinuteTimeRule( TRUE );	// 1 이면 실봉+허봉,  0이면 실봉
	// <-- [Edit]  강지원 2008/10/28
	pChartItem->SetBUseGapRevision( FALSE);

	
	pChartItem->ResetChart();
// --> [Edit]  강지원 2008/11/09
//	pChartItem->ResetData();
//	pChartItem->ResetOCX();

//	CheHoga_Save(0, FALSE , FALSE);
// <-- [Edit]  강지원 2008/11/09

	// 선택된 차트에 새로 요청할 데이터 default 갯수 셋팅
	if( ((CStdDialog*)GetParent())->m_nChartType==MINI_MULTI_CHART ||
		((CStdDialog*)GetParent())->m_nChartType==MINI_SINGLE_CHART ) 
		pChartItem->SetCntForRequest(90);
	else
		pChartItem->SetCntForRequest(DEFAULTCNTATONCE);

	pChartItem->SetSaveAllCount(DEFAULTCNTATONCE);

	pChartItem->SetChartNumericValue(WON_TYPE);

	//2007.01.17 add by LYH  툴바버튼 동기화(이전 선택 세팅)
	pChartItem->m_bDrawAutoTrendLine = 0;
	pChartItem->m_bDrawAutoPattern = 0;
	pChartItem->m_bDrawCandlePattern = 0;
	pChartItem->m_bDrawNews = 0;

	m_bHideScale = FALSE;
	ShowScale();
	m_bHideInnerToolBar = FALSE;
	ShowInnerToolbar();
	//}}

	//초기화 시키도록 정보 제공.
	if(m_pExtInputBarDlg->GetSafeHwnd())
		m_pExtInputBarDlg->SendMessage(UMSG_CHART_INIT_FROMOCX, 1);


	pChartItem->InvokeAddIn("ChartGuideLineAddIn.dll:GUIDE_LINE",200,(long)pChartItem->GetSafeHwnd());
	pChartItem->InvokeAddIn("ChartPatternAddin.dll",0,200);
	pChartItem->InvokeAddInSTIndex("ChartSTIndexAddIn.dll",200,0);	// 시스템트레이딩 전체초기화
	//////////////////////////////////////////////////////////

	// 차트 초기화인지 : 복수종목 - ojtaso (20070621)
	m_bResetChartAll = TRUE;

	//초기화 후 다시 TR재전송
	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
		m_pInputBarDlg->SendMessage(UMSG_CHART_INIT_FROMOCX, 1);

	// 차트 초기화인지 : 복수종목 - ojtaso (20070621)
	m_bResetChartAll = FALSE;

	SetBInitFromExtToolBar(FALSE);
}

BOOL CChartMng::RevisedPriceFlag()// 수정주가.
{
	CChartItem* pSelItem = GetpChartItemSelected();

	if(pSelItem)
	{
		return pSelItem->GetUseRevisedValue();
	}
	return FALSE;
}

void CChartMng::SetMarketType(int nMarketType)
{
	m_nMarketType = nMarketType;

	if(m_pFloatingDlg)
	{
		if(m_nMarketType == CMasterTypeInfo::STOCK_CHART)
			m_pFloatingDlg->ShowWindow(SW_SHOW);
		else
			m_pFloatingDlg->ShowWindow(SW_HIDE);
	}
} 

// 새로운 차트 추가 : 복수종목 - ojtaso (20070108) 
void CChartMng::SetAddMode(BOOL bAddMode)
{
	m_bAddMode = bAddMode;
}

// 종목명 얻기 : 복수종목 - ojtaso (20080213)
CString CChartMng::GetJongMokNameByCode(LPCSTR lpszItemCode, LPCTSTR lpszTRCode/* = NULL*/)
{
	return ((CStdDialog*)GetParent())->GetJongMokNameByCode(lpszItemCode, lpszTRCode);
}

// 멀티차트의 선택여부 : 복수종목 - ojtaso (20070525)
void CChartMng::SetSelChangeChart(BOOL bSelChangeChart)
{ 
	m_bSelChangeChart = bSelChangeChart; 
}

// 종목코드로 RQ를 찾고 선택 : 복수종목 - ojtaso (20070705)
BOOL CChartMng::FindSelectChartByItemCode(LPCTSTR lpszItemCode, BOOL bSelect/* = TRUE*/)
{
	CChartItem* lpSelItem = GetpChartItemSelected();
	if(lpSelItem)
		return lpSelItem->FindSelectChartByItemCode(lpszItemCode, bSelect);
	else 
		return FALSE;
}

void CChartMng::ReplaceOldCode()
{
	CChartItem* lpSelItem = GetpChartItemSelected();
	if(lpSelItem)
		lpSelItem->ResetRQ();
	//초기화 후 다시 TR재전송
	CString strRetryCode, strCurrentCode;
	strRetryCode = ((CInputBarDlg*)m_pInputBarDlg)->m_strOldCode;			
	strCurrentCode = ((CInputBarDlg*)m_pInputBarDlg)->GetCurrentCode();
	if(strCurrentCode.IsEmpty() || strRetryCode.IsEmpty() || strRetryCode == strCurrentCode)
		return;

	((CInputBarDlg*)m_pInputBarDlg)->SetShareCodeChanged(TRUE);
	// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
	((CInputBarDlg*)m_pInputBarDlg)->m_pCodeInput->SetCode(CStdDialog::GetRealItemCode(strRetryCode));
	((CInputBarDlg*)m_pInputBarDlg)->SetShareCodeChanged(FALSE);

	((CInputBarDlg*)m_pInputBarDlg)->m_strCodeFromOutside = strRetryCode;
	((CInputBarDlg*)m_pInputBarDlg)->EnableWindow(FALSE);
		
	CCodeDataFromInputBar CodeData;
	CodeData.m_strCode = strRetryCode;
	CodeData.m_lEndTime = ((CInputBarDlg*)m_pInputBarDlg)->GetEndDate();
	CodeData.m_chType = ((CInputBarDlg*)m_pInputBarDlg)->GetType();
	CodeData.m_lTypeNum = ((CInputBarDlg*)m_pInputBarDlg)->GetTypeNumber();
	if(GetSafeHwnd()) // 2005. 04. 22 by Nam
		SendMessage(UMSG_INPUTCODEFROMINPUTBAR,(WPARAM)&CodeData);

	((CInputBarDlg*)m_pInputBarDlg)->EnableWindow(TRUE);
	
//	CString strCodeReplace = ((CInputBarDlg*)m_pInputBarDlg)->m_strOldCode;
//	if(strCodeReplace.GetLength() == 0)
//	{
//		ResetOCX();
//		return;
//	}
//
//	if(m_pInputBarDlg && m_pInputBarDlg->GetSafeHwnd())
//		m_pInputBarDlg->SendMessage(UMSG_CHART_INIT_FROMOCX, 1);
//
//	int nMarketType = ((CStdDialog*)GetParent())->GetMarketTypeFromCode(strCodeReplace);
//	CString strQueryType;
//	if(nMarketType == STOCK_CHART)
//		strQueryType = QUERY_STRFOFOREIGN;
//	else if(nMarketType == UPJONG_CHART)
//		strQueryType = QUERY_STRUPJONG;
//	else if(nMarketType == FUTOPT_CHART)
//		strQueryType = QUERY_STRFUTOPT;
//	else if(nMarketType == STOCKFUTOPT_CHART)
//		strQueryType = QUERY_STRSTOCKFUTOPT;
//	else if(nMarketType == FOREIGN_CHART)
//		strQueryType = QUERY_STRFOREIGN;
//
//	CChartItem* pChartItem = GetpChartItemSelected();
//	if(pChartItem)
//		pChartItem->SetCodeForQuery(strCodeReplace, strQueryType, TRUE);
//
//	RequestChartData(TRUE);
}

void CChartMng::MakePlatformMenu(HMENU hMenu)
{
	CMenu menu;
	menu.Attach(hMenu);

	CMenu menuTool, menuAnal, menuAddition, menuUser;
	menuTool.CreatePopupMenu();
	menuAnal.CreatePopupMenu();
	menuAddition.CreatePopupMenu();
	menuUser.CreatePopupMenu();

	int nToolID;
	CString strToolName;
	UINT nFlagsChild;
	int nCheckState;
	for(int nIndx = 0; nIndx < m_arrayAnalIdx.GetSize(); ++nIndx)
	{
		nToolID = m_arrayAnalIdx.GetAt(nIndx);
		nCheckState = m_arrayAnalCheck.GetAt(nIndx);
		if( nCheckState )
		{
			nFlagsChild = MF_STRING|MF_CHECKED|MF_BYPOSITION;
		}
		else
		{
			nFlagsChild = MF_STRING|MF_UNCHECKED|MF_BYPOSITION;
		}
		if(m_mapToolName.Lookup(nToolID, strToolName))
			menuAnal.AppendMenu(nFlagsChild,(UINT)ID_PLM_ADD_MENU_STARTANALTOOL+nToolID, strToolName);
	}
	for(nIndx = 0; nIndx < m_arrayAdditionIdx.GetSize(); ++nIndx)
	{
		nToolID = m_arrayAdditionIdx.GetAt(nIndx);
		nCheckState = m_arrayAdditionCheck.GetAt(nIndx);
		if( nCheckState )
		{
			nFlagsChild = MF_STRING|MF_CHECKED|MF_BYPOSITION;
		}
		else
		{
			nFlagsChild = MF_STRING|MF_UNCHECKED|MF_BYPOSITION;
		}
		if(m_mapToolName.Lookup(nToolID, strToolName))
			menuAddition.AppendMenu(nFlagsChild,(UINT)ID_PLM_ADD_MENU_STARTANALTOOL+nToolID, strToolName);
	}
	for(nIndx = 0; nIndx < m_arrayUserIdx.GetSize(); ++nIndx)
	{
		nToolID = m_arrayUserIdx.GetAt(nIndx);
		nCheckState = m_arrayUserCheck.GetAt(nIndx);
		if( nCheckState )
		{
			nFlagsChild = MF_STRING|MF_CHECKED|MF_BYPOSITION;
		}
		else
		{
			nFlagsChild = MF_STRING|MF_UNCHECKED|MF_BYPOSITION;
		}
		if(m_mapToolName.Lookup(nToolID, strToolName))
			menuUser.AppendMenu(nFlagsChild,(UINT)ID_PLM_ADD_MENU_STARTANALTOOL+nToolID, strToolName);
	}
	
	menuTool.AppendMenu(MF_POPUP, (UINT)menuAnal.m_hMenu, "분석도구");
	menuTool.AppendMenu(MF_POPUP, (UINT)menuAddition.m_hMenu, "부가기능");
	menuTool.AppendMenu(MF_POPUP, (UINT)menuUser.m_hMenu, "사용자도구");

	CString str;
	int count = menu.GetMenuItemCount();
	for (int i = 0; i < count; i++)
	{
		if(menu.GetMenuString(i, str, MF_BYPOSITION) && (strcmp(str, "지표추가") == 0))
			break;
	}

	menu.InsertMenu(i+1, MF_BYPOSITION|MF_SEPARATOR, (UINT)0, "");
	menu.InsertMenu(i+2, MF_BYPOSITION|MF_POPUP, (UINT)menuTool.m_hMenu, "툴 선택");

	menuAnal.Detach();
	menuAddition.Detach();
	menuUser.Detach();
	menuTool.Detach();
	menu.Detach();
}

BOOL CChartMng::GetDlgToolBtnToggle(CArray<int, int>& arAnal, CArray<int, int>& arAnalCheck, CArray<int, int>& arAddition, CArray<int, int>& arAdditionCheck, CArray<int, int>& arUser, CArray<int, int>& arUserCheck, CMap<int, int, CString, CString>& mapToolName)
{
	arAnal.RemoveAll();
	arAnalCheck.RemoveAll();
	arAddition.RemoveAll();
	arAdditionCheck.RemoveAll();
	arUser.RemoveAll();
	arUserCheck.RemoveAll();
	mapToolName.RemoveAll();

	return ((CStdDialog*)GetParent())->GetDlgToolBtnToggle(arAnal, arAnalCheck, arAddition, arAdditionCheck, arUser, arUserCheck, mapToolName);
}

void CChartMng::SetDlgToolBtnToggle(int nToolBtnID, BOOL bToggle/*=TRUE*/)
{
	((CStdDialog*)GetParent())->SetDlgToolBtnToggle(nToolBtnID, bToggle);
}

void CChartMng::AddMainMenuToPlatformMenu(HMENU hMenu)
{
	HWND hChildFrame = (HWND)GetParent()->GetParent()->SendMessage(WM_USER+104);
	if ( !hChildFrame) return;
	
// --> [Edit]  강지원 2008/07/29	( 유진에서는 메인에서 추가하는 메뉴를 받아오기위해서 메시지를 날렸지만... 일단 FX에서는 뺌... )
	BCMenu* pCommonMenu = new BCMenu;
	ASSERT ( pCommonMenu );

	pCommonMenu->CreatePopupMenu();

//	::SendMessage( hChildFrame, UM_GET_SCREENLINKMENU, 0, (LPARAM)pCommonMenu );

//	if ( pCommonMenu <= 0 ) return;

	m_menuMain.RemoveAll();
	menuChart.RemoveAll();

//	long lMenu = ::SendMessage( hChildFrame, UM_GET_SCREENLINKMENU, 0, 0 );
//	if ( lMenu <= 0 ) return;
//
//	m_menuMain.RemoveAll();
//	menuChart.RemoveAll();
//
//	BCMenu* pCommonMenu = (BCMenu*)lMenu;
//	ASSERT ( pCommonMenu );
// <-- [Edit]  강지원 2008/07/29

	HMENU hChartMenu = hMenu;
	CMenu* pChartMenu = CMenu::FromHandle(hChartMenu);

	int nCommonMenuCount = pCommonMenu->GetMenuItemCount();
	int nChartMenuCount = pChartMenu->GetMenuItemCount();
	int i, nLastValidMenuPosition = -1;

	long lResult=0;
	if((nCommonMenuCount>0) && (nChartMenuCount>0))
		pChartMenu->InsertMenu(0, MF_BYPOSITION | MF_SEPARATOR);

	MENUITEMINFO info;
	info.cbSize = sizeof (MENUITEMINFO); // must fill up this field
	info.fMask = MIIM_STATE;             // get the state of the menu item

//	int j=0;
//	for(i=nCommonMenuCount-1; i>= 0;i--)
	for(i = 0; i < nCommonMenuCount; ++i)
	{
		CString strMenuString;
		UINT nCommandID = pCommonMenu->GetMenuItemID(i);
		pCommonMenu->GetMenuItemInfo(nCommandID, &info);
		pCommonMenu->GetMenuText(i, strMenuString, MF_BYPOSITION);

//		int nPosition = GetPositionInMenu(strMenuString, pChartMenu);
//		if(nPosition != -1)
//		{
//			nLastValidMenuPosition = nPosition;
//			continue;
//		}
//
//		if(pCommonMenu->GetSubMenu(i))	//서브메뉴 존재
//		{
//			continue;
//		}
//		lResult = pCommonMenu->GetMenuState(nCommandID, MF_BYCOMMAND);
//		MENUITEMINFO mi;
//		mi.cbSize = sizeof(MENUITEMINFO);
//		mi.fMask = MIIM_STATE;
//		mi.fState = lResult & ~MF_OWNERDRAW;

		UINT nFlagsChild;
		if(i == nCommonMenuCount-1 && strMenuString.GetLength()==0)
			continue;
		else if(strMenuString.GetLength()==0)
			pChartMenu->InsertMenu(i, MF_BYPOSITION | MF_SEPARATOR);
		else
		{
			BOOL bCheck = info.fState & MF_CHECKED;
			if( bCheck )
			{
				nFlagsChild = MF_STRING|MF_CHECKED|MF_BYPOSITION;
			}
			else
			{
				nFlagsChild = MF_STRING|MF_UNCHECKED|MF_BYPOSITION;
			}

			pChartMenu->InsertMenu(i, nFlagsChild, ID_PLM_ADD_MENU_BEGINMAIN+i, strMenuString);
//			::SetMenuItemInfo(hChartMenu, i, nFlagsChild, &info);
			m_menuMain.Add(nCommandID);
			menuChart.Add(ID_PLM_ADD_MENU_BEGINMAIN+i);
//			if(nCommandID >= ID_MAINMENU_BASE && nCommandID <= ID_MAINMENU_BASE_END)
//			{
//				pChartMenu->InsertMenu(nLastValidMenuPosition+1, MF_BYPOSITION | MF_STRING, nCommandID-ID_MAINMENU_BASE+ID_PLM_ADD_MENU_BEGINMAIN, strMenuString);
//				::SetMenuItemInfo(hChartMenu, nCommandID-ID_MAINMENU_BASE+ID_PLM_ADD_MENU_BEGINMAIN, MF_BYCOMMAND,&mi);
//			}
//			else if(nCommandID >= ID_MAINCONNMENU_BASE && nCommandID <= ID_MAINCONNMENU_BASE_END)
//			{
//				pChartMenu->InsertMenu(nLastValidMenuPosition+1, MF_BYPOSITION | MF_STRING, nCommandID-ID_MAINCONNMENU_BASE+ID_PLM_ADD_MENU_BEGINMAINCONN, strMenuString);
//				::SetMenuItemInfo(hChartMenu, nCommandID-ID_MAINCONNMENU_BASE+ID_PLM_ADD_MENU_BEGINMAINCONN, MF_BYCOMMAND,&mi);
//			}
		}
	}

	pCommonMenu->DestroyMenu();
	delete pCommonMenu;
}

int CChartMng::GetPositionInMenu(CString strMenu, CMenu* pMenu)
{
	int i=-1;
	int nMenuCount = pMenu->GetMenuItemCount();
	CString strTemp;
	for(i=0;i<nMenuCount;i++)
	{
		pMenu->GetMenuString(i, strTemp, MF_BYPOSITION);
		if(strTemp=="") continue;
		if(!strTemp.Compare(strMenu)) return i;
	}
	return -1;
}

LRESULT CChartMng::OnAddPlatformMenu( WPARAM wParam, LPARAM lParam)
{
	HMENU hMenu = (HMENU)lParam;
	AddMainMenuToPlatformMenu(hMenu);
	if(GetDlgToolBtnToggle(m_arrayAnalIdx, m_arrayAnalCheck, m_arrayAdditionIdx, m_arrayAdditionCheck, m_arrayUserIdx, m_arrayUserCheck, m_mapToolName))
		MakePlatformMenu(hMenu);

	return 0;
}

//LRESULT CChartMng::OnCheckPlatformMenu( WPARAM wParam, LPARAM lParam)
//{
//	return 0;
//}

// --> [Edit]  이도영 2008/09/11
// 
BOOL CChartMng::SetOrderData(char *pData)
{
	OAQ69901_OUTREC1 Outrec;
	int nPos = 0;
	char szTemp[100];

	memset(szTemp, 0x00, sizeof(szTemp));
	memcpy(szTemp, pData + nPos, 5);

	nPos += 5;

	int nCnt = atoi(szTemp);

	CString			strTickSize = "1";
	CString			strTickValue, strPriceInfo;
	int				nLog;
	int				nLogDispSize;
	int				nPrecision;
	int				ValidPoint;
	int				nMask;
	long			nTickSize;

	//@SolomonGNF IMasterDataManager* pDataManager = (IMasterDataManager*)AfxGetPctrInterface(UUID_IMasterDataManager, 3);
	IMasterDataManager* pDataManager = (IMasterDataManager*)AfxGetPctrInterface(UUID_IMasterDataManager);
	ORDER_INFO * pOrerInfo;
	for (int i = 0; i < nCnt; i++)
	{
		memcpy(&Outrec, pData + nPos, sizeof(Outrec));
		nPos += sizeof(Outrec);
		
		pOrerInfo = new ORDER_INFO;

		memset(szTemp, 0x00, sizeof(szTemp));
		memcpy(szTemp, Outrec.acntno, sizeof(Outrec.acntno));
		pOrerInfo->m_stracntno = szTemp;
		pOrerInfo->m_stracntno.TrimRight();

		memset(szTemp, 0x00, sizeof(szTemp));
		memcpy(szTemp, Outrec.series, sizeof(Outrec.series));
		pOrerInfo->m_strseries = szTemp;
		pOrerInfo->m_strseries.TrimRight();

		memset(szTemp, 0x00, sizeof(szTemp));
		memcpy(szTemp, Outrec.buysellsect, sizeof(Outrec.buysellsect));
		pOrerInfo->m_strbuysellsect = szTemp;
		pOrerInfo->m_strbuysellsect.TrimRight();

		memset(szTemp, 0x00, sizeof(szTemp));
		memcpy(szTemp, Outrec.posqty, sizeof(Outrec.posqty));
		pOrerInfo->m_strposqty = szTemp;
		pOrerInfo->m_strposqty.TrimRight();

		memset(szTemp, 0x00, sizeof(szTemp));
		memcpy(szTemp, Outrec.posprice, sizeof(Outrec.posprice));
		pOrerInfo->m_strposprice = szTemp;
		pOrerInfo->m_strposprice.TrimLeft();
		pOrerInfo->m_strposprice.TrimRight();

		CString strKey;

		strKey.Format("%s%s", pOrerInfo->m_stracntno, pOrerInfo->m_strseries);
		strKey.TrimRight();

//@SolomonGNF 		if ( !pDataManager->GetGJongmokInfo( pOrerInfo->m_strseries, nLog, nLogDispSize, nPrecision, strTickSize, strTickValue, strPriceInfo ) )
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

		CString strPrePrice = "";
		strPrePrice = pOrerInfo->m_strposprice;
		strPrePrice.TrimLeft();
		strPrePrice.TrimRight();
		strPrePrice = ((CStdDialog*)GetParent())->TbondPrcLToF(strPrePrice, nLog, nLogDispSize, ValidPoint, TRUE);
		pOrerInfo->m_dblPrice = atof(strPrePrice);
		m_mapOrder.SetAt(strKey, (CObject*)pOrerInfo);

		pOrerInfo->m_strposprice = Format32String(pOrerInfo->m_strposprice, nLog, nLogDispSize, nPrecision, nMask);
	}

	return TRUE;
}

BOOL CChartMng::SetRealOrderInfo(ORDER_INFO* pOrderInfo, int nRealType)
{
	CString strKey;
	strKey.Format("%s%s", pOrderInfo->m_stracntno, pOrderInfo->m_strseries);

	ORDER_INFO * pTempOrerInfo;
	if (nRealType == MIGYULJE_PID)
	{
		if (pOrderInfo->m_strstate == "13" ||
			pOrderInfo->m_strstate == "16" ||
			pOrderInfo->m_strstate == "19")
		{
			return FALSE;
		}

		CString			strTickSize = "1";
		CString			strTickValue, strPriceInfo;
		int				nLog;
		int				nLogDispSize;
		int				nPrecision=0;
		int				ValidPoint;
		int				nMask;
		long			nTickSize;

		//@SolomonGNF IMasterDataManager* pDataManager = (IMasterDataManager*)AfxGetPctrInterface(UUID_IMasterDataManager, 3);
		IMasterDataManager* pDataManager = (IMasterDataManager*)AfxGetPctrInterface(UUID_IMasterDataManager);

		if (m_mapOrder.Lookup(strKey, (CObject*&)pTempOrerInfo) == FALSE)
		{
			pTempOrerInfo = new ORDER_INFO;
			{
				pTempOrerInfo->m_stracntno = pOrderInfo->m_stracntno;
				pTempOrerInfo->m_strseries = pOrderInfo->m_strseries;
				pTempOrerInfo->m_strbuysellsect = pOrderInfo->m_strbuysellsect;
				pTempOrerInfo->m_strposqty = pOrderInfo->m_strposqty;
				pTempOrerInfo->m_strposprice = pOrderInfo->m_strposprice;

//@SolomonGNF				if ( !pDataManager->GetGJongmokInfo( pTempOrerInfo->m_strseries, nLog, nLogDispSize, nPrecision, strTickSize, strTickValue, strPriceInfo ) )
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

				CString strPrePrice = "";
				strPrePrice = pTempOrerInfo->m_strposprice;
				strPrePrice = ((CStdDialog*)GetParent())->TbondPrcLToF(strPrePrice, nLog, nLogDispSize, ValidPoint, TRUE);
				pTempOrerInfo->m_dblPrice = atof(strPrePrice);

				m_mapOrder.SetAt(strKey, (CObject*)pTempOrerInfo);

				pTempOrerInfo->m_strposprice = Format32String(pTempOrerInfo->m_strposprice, nLog, nLogDispSize, nPrecision, nMask);
			}
		}
		else
		{
			pTempOrerInfo->m_stracntno = pOrderInfo->m_stracntno;
			pTempOrerInfo->m_strseries = pOrderInfo->m_strseries;
			pTempOrerInfo->m_strbuysellsect = pOrderInfo->m_strbuysellsect;
			pTempOrerInfo->m_strposqty = pOrderInfo->m_strposqty;
			pTempOrerInfo->m_strposprice = pOrderInfo->m_strposprice;

//@SolomonGNF			if ( !pDataManager->GetGJongmokInfo( pOrderInfo->m_strseries, nLog, nLogDispSize, nPrecision, strTickSize, strTickValue, strPriceInfo ) )
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

			CString strPrePrice = "";
			strPrePrice = pOrderInfo->m_strposprice;
			strPrePrice = ((CStdDialog*)GetParent())->TbondPrcLToF(strPrePrice, nLog, nLogDispSize, ValidPoint, TRUE);
			pTempOrerInfo->m_dblPrice = atof(strPrePrice);

			pTempOrerInfo->m_strposprice = Format32String(pTempOrerInfo->m_strposprice, nLog, nLogDispSize, nPrecision, nMask);

			if (atoi(pOrderInfo->m_strposqty) == 0)
			{
				m_mapOrder.RemoveKey(strKey);
				delete pTempOrerInfo;
			}
		}
	}

	EACH_CHARTJOB_START()
		pChartItem->Invalidate();
	EACH_CHARTJOB_END()		
	return TRUE;
}
// <-- [Edit]  이도영 2008/09/11

// (2008/9/14 - Seung-Won, Bae) for Pair Tick
void CChartMng::RequestChartPairTickData( const char *p_szCode, BOOL p_bWithRequest)
{
	CChartItem* pSelectedChartItem = GetpChartItemSelected();
	if( pSelectedChartItem && p_szCode)
	{
		pSelectedChartItem->SetCodeForQuery( "|S_" + CStdDialog::GetRealItemCode( p_szCode),
												QUERY_STRFOFOREIGN_HOGA2, TRUE);
		if( p_bWithRequest) RequestChartData();
	}
}

//	플래그를 둔 이유는 탭에서 선택했을때는 차트에 알려줘야 하지만... 차트에서 선택했을때도 시간을 바꿔주기 위해서 여기로 들어오기때문에 무한루프에 빠질수 있어서 플래그로 막는다.
void CChartMng::SetTimeGab(int nIndex, BOOL bFlag)
{
//	m_strTimeSet = strTime; 
	////////////////////////////////////////////////////
	//		현재 RQ의 선택되어 있는 시간대의 Index 구하기
	CChartItem* pSelectedChartItem = GetpChartItemSelected();
	if( pSelectedChartItem)
	{
		if ( bFlag )
		{
			// lParam 이 0이면 Set
			pSelectedChartItem->SendMessage(UDM_CHANGE_TIMEDIFF, nIndex, 0);
			m_strTimeSet.Format("%d", nIndex);
			pSelectedChartItem->SetTimeIndex(nIndex);
			// nIndex => 0 Based 시간대 Index
		}

		ST_CHARTITEM_REALDATA* pChartItemRealData = NULL;
		if(pSelectedChartItem->m_mapChartItemRealData.Lookup(CStdDialog::GetRealItemCode((LPCTSTR)pSelectedChartItem->GetSelectedItemCode()), (LPVOID&)pChartItemRealData))
		{
			ST_SELCHART* lpSelChart = NULL;
			pSelectedChartItem->GetSelChart(NULL, lpSelChart);

			if ( lpSelChart )
			{
				lpSelChart->m_strTimeName = ((CStdDialog*)GetParent())->GetTimeIndexInfo(nIndex, 0);
			}
			else
				return;


			CString strTime = "", strTimeIndex = "";
			int	nTimeGab(0), nBaseTimeGab(0);
			nTimeGab = atoi(((CStdDialog*)GetParent())->GetTimeIndexInfo(nIndex, 1));
			strTime = pChartItemRealData->m_strTime;
			strTimeIndex = pChartItemRealData->m_strTimeIndex;

			if ( !strTimeIndex.IsEmpty() )
				nBaseTimeGab = atoi(((CStdDialog*)GetParent())->GetTimeIndexInfo(atoi(strTimeIndex), 1));

			lpSelChart->m_strTimeGab.Format("%2d", nTimeGab - nBaseTimeGab);
			lpSelChart->m_nTimeIndex = nIndex;

			if ( nIndex <= 0 )
				((CExtInputBarDlg*)m_pExtInputBarDlg)->SetExInputBarDlgTimeIndex(lpSelChart->m_nOrgTimeIndex);
			else
				((CExtInputBarDlg*)m_pExtInputBarDlg)->SetExInputBarDlgTimeIndex(nIndex);

			Invalidate();
		}
	}
	

	/*		현재 RQ의 선택되어 있는 시간대의 Index 구하기
	CChartItem* pSelectedChartItem = GetpChartItemSelected();
	if( pSelectedChartItem)
	{
	// lParam 이 1이면 Get
		long lResult = pSelectedChartItem->SendMessage(UDM_CHANGE_TIMEDIFF, 0, 1);
		// lResult => 0 Based 시간대 Index
		//			== -1 :: 원래 시간대인 상태
	}
	*/

	////////////////////////////////////////////////////
}

CString CChartMng::GetSelectTime()
{
	////////////////////////////////////////////////////
	//		현재 RQ의 선택되어 있는 시간대의 Index 구하기
	CString strTime = "";
	CChartItem* pSelectedChartItem = GetpChartItemSelected();
	if( pSelectedChartItem)
	{
		ST_CHARTITEM_REALDATA* pChartItemRealData = NULL;
		if(pSelectedChartItem->m_mapChartItemRealData.Lookup(CStdDialog::GetRealItemCode((LPCTSTR)pSelectedChartItem->GetSelectedItemCode()), (LPVOID&)pChartItemRealData))
		{
			ST_SELCHART* lpSelChart = NULL;
			pSelectedChartItem->GetSelChart(NULL, lpSelChart);
			if ( lpSelChart )
			{
// --> [Edit]  강지원 2008/12/02	( 각 거래소의 현재시간을 구한다. : GMT시간을 구해서 해단 거래소의 시간을 구한다. )
				CTime	tToday = CTime::GetCurrentTime();
				strTime.Format("%s", tToday.Format("%H%M%S") );

//				//	서울시간의 갭을구한다. 
//				int nSeoulGab = atoi(((CStdDialog*)GetParent())->GetTimeIndexInfo(22, 1));
////				int nGab = atoi(((CStdDialog*)GetParent())->GetTimeIndexInfo(atoi(lpSelChart->m_strTimeGab), 1));
//
//				if ( pSelectedChartItem->m_nTimeIndex <= 0 && 4 <= strTime.GetLength())		//	시간 탭의 해당 거래소의 시간이면 원래의 시간을 보여준다. )
//					strTime = pSelectedChartItem->GetCalcTimeGab(strTime, ( nSeoulGab + atoi(lpSelChart->m_strTimeGab) ));
//				else
//					strTime = pSelectedChartItem->GetCalcTimeGab(pChartItemRealData->m_strTime, atoi(lpSelChart->m_strTimeGab));

				CString strValue = "";
				strTime = pChartItemRealData->m_strTime.Right(6);
				if ( lpSelChart->m_nTimeIndex <= 0 && 4 <= strTime.GetLength())		//	시간 탭의 해당 거래소의 시간이면 원래의 시간을 보여준다. )
					strTime.Format("%s:%s:%s", strTime.Left(2), strTime.Mid(2, 2) , strTime.Mid(4, 2));
				else
					strTime = pSelectedChartItem->GetCalcTimeGab(strValue, pChartItemRealData->m_strTime, atoi(lpSelChart->m_strTimeGab));

////				strTime = pSelectedChartItem->GetCalcTimeGab(pChartItemRealData->m_strTime, atoi(lpSelChart->m_strTimeGab));
// <-- [Edit]  강지원 2008/12/02
			}
			
		}
	}
	return strTime;
}

BOOL CChartMng::CheHoga_Save(int nIndex, BOOL bSaveFlag/* = TRUE */, BOOL bLoadFlag/* = TRUE */)
{
	if ( nIndex == 0 )
	{
		CString strName = ((CStdDialog*)GetParent())->m_strConfig;
		CString strScreenNo = ((CStdDialog*)GetParent())->m_strScreenNo;

#ifdef	_LASTINFO_SAVE
		strName.Replace(strScreenNo, "Hoga_" + strScreenNo);
		strName.Replace(".dat", ".mct");
		if ( bSaveFlag )
			SaveChart(strName);

//		// 마지막 저장파일일경우 다른 화면번호로 저장하기위해서 추가
//		long	nHwnd(0);
//		CString strTempName = "";
//		CString strTemp = strName;
//
//		strTemp.Replace(".mct", "");
//		nHwnd = (long)GetParent()->GetSafeHwnd();
//
//		strTempName.Format("%s_%d.tmp", strTemp, nHwnd);
////		if ( bSaveFlag )
////			SaveChart(strTempName);
////		::CopyFile(strName, strTempName, TRUE);
#else
		strName.Replace(strScreenNo, "Hoga_" + strScreenNo);
		strName.Replace(".dat", ".mct");
		if ( bSaveFlag )
			SaveChart(strName);
#endif

		strName.Replace("Hoga_" + strScreenNo, "Che_" + strScreenNo);
//		strName.Replace("Hoga_0000", "Che_0000");
		//	파일을 찾아보고 없으면 틱을 누른것처럼 처리한다.
		CFileFind ff;
		BOOL bResult = ff.FindFile(strName);
		if(!bResult)
		{
//			ResetChart();
//			ResetOCX();
//			GetpChartItemSelected()->RemoveMapChartItem();

			return FALSE;
		}

		if ( bLoadFlag )
		{
			CString	strAppName;
			long lRowIndex(0), lColIndex(0);
			GetpChartItemSelected()->GetRowNCol(lRowIndex, lColIndex);
			strAppName.Format("%02d%02d",lColIndex,lRowIndex);
			CString strCode = GetpChartItemSelected()->GetSelectedItemCode();
			::WritePrivateProfileString(strAppName,_T("CHEHOGA_CODE"),strCode,strName);	
			GetpChartItemSelected()->m_bCheHogaChange = TRUE;

			//{1*1 차트로 초기화 요청
			ReplaceChart(0,0,m_lRowSelected,m_lColSelected);
			SetMatrix(1,1);
			//}
			if(m_pInputBarDlg)
				((CInputBarDlg*)m_pInputBarDlg)->SetMode(CHANGE_ITEM);
			
	//		CString strCode = ((CChartItem*)GetpChartItemSelected())->GetSelectedItemCode();
	//		GetpChartItemSelected()->SetCodeForQuery(CStdDialog::GetRealItemCode( strCode),QUERY_STRFOFOREIGN,TRUE);

//			ResetChart();
//			ResetOCX();
//			GetpChartItemSelected()->RemoveMapChartItem();

			CString KeyName = "";
			KeyName.Format("%d_%s", 1, CStdDialog::GetRealItemCode( strCode ));
			((CStdDialog*)GetParent())->m_RealTrCounter.RemoveAll();
			((CStdDialog*)GetParent())->m_RealTrCounter.AddTr(KeyName);
//			((CStdDialog*)GetParent())->MakeDrdsUnConnectUnAdviseFromCode(1, TRUE);

			LoadChart(strName);
			m_strCodeForLoadChart = "";
			EACH_CHARTJOB_START()		
				pChartItem->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::T_ALL_TOOL_RESET_STATE, 0);
			EACH_CHARTJOB_END()

			GetpChartItemSelected()->m_bCheHogaChange = FALSE;
		}
	}
	else 
	{
		CString strName = ((CStdDialog*)GetParent())->m_strConfig;
		CString strScreenNo = ((CStdDialog*)GetParent())->m_strScreenNo;

#ifdef	_LASTINFO_SAVE
		strName.Replace(strScreenNo, "Che_" + strScreenNo);
		strName.Replace(".dat", ".mct");
		if ( bSaveFlag )
			SaveChart(strName);

// --> [Edit]  강지원 2009/01/02	( 초기화시 호가 지표 저장 문제 수정 )
//		// 마지막 저장파일일경우 다른 화면번호로 저장하기위해서 추가
//		if ( bSaveFlag )
//		{
//			if ( ((CStdDialog*)GetParent())->m_strSaveKey.IsEmpty() )
//				((CStdDialog*)GetParent())->FileCopyAndSaveAndDelete(HWND_DEFAULT_COPY);
//		}
// <-- [Edit]  강지원 2009/01/02

//		long	nHwnd(0);
//		CString strTempName = "";
//		CString strTemp = strName;
//
//		strTemp.Replace(".mct", "");
//		nHwnd = (long)GetParent()->GetSafeHwnd();
//
//		strTempName.Format("%s_%d.mct", strTemp, nHwnd);
//		if ( bSaveFlag )
//			SaveChart(strTempName);
////		::CopyFile(strName, strTempName, TRUE);
#else
		strName.Replace(strScreenNo, "Che_" + strScreenNo);
		strName.Replace(".dat", ".mct");
		if ( bSaveFlag )
			SaveChart(strName);
#endif

		strName.Replace("Che_" + strScreenNo, "Hoga_" + strScreenNo);
//		strName.Replace("Che_0000", "Hoga_0000");
		//	파일을 찾아보고 없으면 틱을 누른것처럼 처리한다.
		CFileFind ff;
		BOOL bResult = ff.FindFile(strName);
		if(!bResult)
		{
//			ResetChart();
//			ResetOCX();
//			GetpChartItemSelected()->RemoveMapChartItem();

			return FALSE;
		}

		if ( bLoadFlag )
		{
			CString	strAppName;
			long lRowIndex(0), lColIndex(0);
			GetpChartItemSelected()->GetRowNCol(lRowIndex, lColIndex);
			strAppName.Format("%02d%02d",lColIndex,lRowIndex);
			CString strCode = GetpChartItemSelected()->GetSelectedItemCode();
			::WritePrivateProfileString(strAppName,_T("CHEHOGA_CODE"),strCode,strName);	
			GetpChartItemSelected()->m_bCheHogaChange = TRUE;

			//{1*1 차트로 초기화 요청
			ReplaceChart(0,0,m_lRowSelected,m_lColSelected);
			SetMatrix(1,1);
			//}
			if(m_pInputBarDlg)
				((CInputBarDlg*)m_pInputBarDlg)->SetMode(CHANGE_ITEM);

//			ResetChart();
//			ResetOCX();
//			GetpChartItemSelected()->RemoveMapChartItem();

			CString KeyName = "";
			KeyName.Format("%d_%s", 0, CStdDialog::GetRealItemCode( strCode ));
			((CStdDialog*)GetParent())->m_RealTrCounter.RemoveAll();
			((CStdDialog*)GetParent())->m_RealTrCounter.AddTr(KeyName);
//			((CStdDialog*)GetParent())->MakeDrdsUnConnectUnAdviseFromCode(0, TRUE);

			LoadChart(strName);
			m_strCodeForLoadChart = "";
			EACH_CHARTJOB_START()		
				pChartItem->SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::T_ALL_TOOL_RESET_STATE, 0);
			EACH_CHARTJOB_END()

			GetpChartItemSelected()->m_bCheHogaChange = FALSE;
		}
	}

	return TRUE;
}


CString	CChartMng::GetCtlTime()
{
	return ((CExtInputBarDlg*)m_pExtInputBarDlg)->GetCtlTime();
}

int	CChartMng::GetExInputBarDlg()
{
	return ((CExtInputBarDlg*)m_pExtInputBarDlg)->GetExInputBarDlgTimeIndex();
}

void CChartMng::SetOrderButtonEnable(BOOL bEnable)
{
	((CExtInputBarDlg*)m_pExtInputBarDlg)->SetOrderButtonEnable(bEnable);
}

void CChartMng::SetTypeExInputBarDlg(char chType)
{
	((CExtInputBarDlg*)m_pExtInputBarDlg)->SetType(chType);
}

int CChartMng::GetCheHogaType() 
{
	return ((CInputBarDlg*)m_pInputBarDlg)->GetCheHogaType();
}

//	수치조회 메시지
LRESULT CChartMng::OnInquiryDataExport( WPARAM wParam, LPARAM lParam)
{
	int nType = (int)wParam;

	if ( !m_pInquiryDlg )
		return 0;

	if ( !m_pInquiryDlg->GetShowHide() )
		return 0;

	if ( nType == 1 )	//	사용가능한 데이터 메모리 주소
	{
		CStringArray*	arList = (CStringArray*)lParam;
		m_pInquiryDlg->StartDisplayData(arList);
	}
	else if ( nType == 2 )	//	변경되어서 사용하면 안되는 메모리 주소
	{
		m_pInquiryDlg->EndDisplayData();
	}

	return 0;
}



CString CChartMng::Format32String(CString strPrice, int scale, int outdec, int indec, UINT uMask) 
{
	if ( strPrice.IsEmpty() )
		return strPrice;

	CString strValue = "", strInt = "", strMid = "", strFloat = "", strResult = "";
	long	nValue(0);
	long	nTBValue = atol(strPrice);

	if ( scale < 0 || outdec < 0 || indec < 0 )
	{
		return strPrice;
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

	if ( scale == 32 || scale == 8 ) 	//	32진법일경우 포맷처리
	{

		int nMidSize = 2;
//		strValue.Format("%d", nValue);
		strValue = strPrice;

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

////	strValue.Format("%d", nTBValue);
//	if ( bFlag )
//		strResult.Format("-%s", strValue);
//	else
//		strResult.Format("%s", strValue);

	return strValue;
}


CString	CChartMng::GetCurrentCode()
{
	return ((CInputBarDlg*)m_pInputBarDlg)->GetCurrentCode();
}
