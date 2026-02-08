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
#include "../COMMONTR/ChartRealConverter.h"
#include "../Chart_Common/BCMenu.h"
//<<20100308_JS.Kim 솔로몬
//#include "../../../Common/KHResource.h"
//>>
#include "../../inc/RealUpdate.h"
#include "Winprint.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"	// (2008/9/13 - Seung-Won, Bae) for Multi-Language


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

#define UDM_DRAW_FOR_PRINT     WM_USER + 617 // wParam - 0, lParam - HDC //20080919 이문수 >>


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
	m_bIndexLink = FALSE;	//20100212_JS.Kim 확대 상태에서 지표동기화 처리
	m_clStartSelected =	RGB(222,232,255);
	m_clStartUnselected = RGB(229,229,229);
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

	//>> 예외처리 - vntsorl (20090106)
	m_bInitChartItem = FALSE;
	//<< 예외처리 - vntsorl (20090106)

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
	}
	m_lKey = lKey;
	VERIFY(m_pExtInputBarDlg);
	BOOL bTimeShareUseage = m_pExtInputBarDlg->SendMessage(UMSG_GETTIMESHAREUSEAGE);

	EACH_CHARTJOB_START()
	pChartItem->SetInit((long)this->GetSafeHwnd(), m_lKey, m_strRootPath, bNoToolBar);
	if(m_lMltchrtDlgParentClsPointer > 0)
		pChartItem->SetOutsideCommonInterface(m_lMltchrtDlgParentClsPointer);
	pChartItem->EnableReceivingTimeShare(bTimeShareUseage);
	EACH_CHARTJOB_END()
}

BOOL CChartMng::Create(const RECT& rect, CWnd *pParentWnd, LPCTSTR lpUserPath, LPCTSTR lpImagePath, UINT nID, long clsPointer)
{
	m_strUserPath = lpUserPath;
	m_strImagePath = lpImagePath;
	m_nID = nID;
	m_lMltchrtDlgParentClsPointer = clsPointer;
	
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
	
	OleInitialize(NULL);

	// TODO: Add your specialized creation code here
	/*
	LOGFONT lfont;
	memset(&lfont, 0, sizeof(LOGFONT));       // Zero out the structure.
	lfont.lfWidth = 0; 
	lfont.lfEscapement = 0; 
	lfont.lfOrientation = 0; 
	lfont.lfWeight = FW_NORMAL; 
	lfont.lfItalic = 0; 
	lfont.lfUnderline = 0; 
	lfont.lfStrikeOut = 0; 
	lfont.lfCharSet = DEFAULT_CHARSET; 
	lfont.lfOutPrecision = OUT_DEFAULT_PRECIS; 
	lfont.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
	lfont.lfQuality = DEFAULT_QUALITY;
	lfont.lfPitchAndFamily = FF_ROMAN; 
	strcpy(lfont.lfFaceName, "돋움체");
	lfont.lfHeight = -12;
	m_ftForTitle.CreateFontIndirect(&lfont);

	lfont.lfWeight	= 500;
	m_ftNormal.CreateFontIndirect(&lfont);
	*/
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));       // Zero out the structure.
	lf.lfHeight = 12;                      // Request a 12-pixel-height font.
	lf.lfWeight = 700;					   //LYH-20061009 FW_BOLD(=700)
	lstrcpy(lf.lfFaceName, _T("굴림체"));   // Request a face name "굴림".
	lf.lfCharSet = HANGUL_CHARSET;
	m_ftForTitle.CreateFontIndirect(&lf);

	lf.lfWeight	= 500;
	m_ftNormal.CreateFontIndirect(&lf);

	Rearrange(1,RESIZEBOTH);

	OleUninitialize();

	SetChartSelected(CPoint(0,0));	

	((CInputBarDlg*)m_pInputBarDlg)->CreatedChartMng();
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
			SaveChart(((CStdDialog*)GetParent())->m_strConfig);
		}
	}
	{
#ifdef _WRITELOG
		((CStdDialog*)GetParent())->WriteToStringLog("m_pSecCfgChart::SaveandOpenFromFile::Begin", _T("멀티차트 종료"), 13);
#endif
		if(m_pSecCfgChart)
		{		
			if(::IsWindow(m_pSecCfgChart->GetSafeHwnd()))
			{
				if( ( ((CChartItem*)m_pSecCfgChart)->GetType() == SEC_DATA_CHART ))
				{
					sJipyoSaveFileName = ((CChartItem*)m_pSecCfgChart)->GetOpenAndSaveFileName();
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
					sJipyoSaveFileName = ((CChartItem*)m_pTickCfgChart)->GetOpenAndSaveFileName();
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
					sJipyoSaveFileName = ((CChartItem*)m_pMinCfgChart)->GetOpenAndSaveFileName();
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
					sJipyoSaveFileName = ((CChartItem*)m_pDayCfgChart)->GetOpenAndSaveFileName();
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
					sJipyoSaveFileName = ((CChartItem*)m_pHourCfgChart)->GetOpenAndSaveFileName();
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
					sJipyoSaveFileName = ((CChartItem*)m_pWeekCfgChart)->GetOpenAndSaveFileName();
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
					sJipyoSaveFileName = ((CChartItem*)m_pMonthCfgChart)->GetOpenAndSaveFileName();
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
					sJipyoSaveFileName = ((CChartItem*)m_pYearCfgChart)->GetOpenAndSaveFileName();
					((CChartItem*)m_pYearCfgChart)->SaveandOpenFromFile("", sJipyoSaveFileName, FALSE,TRUE);
				}
			}
		}
	}
#ifdef _WRITELOG
	((CStdDialog*)GetParent())->WriteToStringLog("m_pYearCfgChart::SaveandOpenFromFile::End", _T("멀티차트 종료"), 13);
#endif

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
	//지표 돌려보기를 멈춘다. ===================================================================
	GetParent()->PostMessage(UMSG_STOP_JIPYOCYCLE, 0, 0);
	//===========================================================================================
	
	CPoint ptOutput(m_lColSelected,m_lRowSelected);
	if(szNewPos.y>=m_lRows||szNewPos.x>=m_lCols
		||((szNewPos.x ==m_lColSelected)&&(szNewPos.y==m_lRowSelected))) 
	{
		if(!bInvalidateEvenIfPosisequal)
		{
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
	if(m_bLastEnvUpdated)
		m_pChartItems[m_lRowSelected][m_lColSelected]->SetFocus();
	//}}

	//===============================================================================
	::SendMessage( GetParent()->GetParent()->GetSafeHwnd(), RMSG_SAVE_FOCUS_CONTROL, 0, 0);
	//===============================================================================

	int nMode = ((CInputBarDlg*)m_pInputBarDlg)->GetCurrentMode();
	if(nMode == CHANGE_ITEM)
	{
		if(bQueryData)
			SendInitChartData(TRUE, TRUE);
		else
			SendInitChartData();
	}

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

	_stData.m_lTotalCnt = pChartItem->GetAllCnt();
	pChartItem->GetStartEndDateOnView(&dStartDate,&dEndDate);
	_stData.m_lStartDate = (long)dStartDate;
	_stData.m_lEndDate = (long)dEndDate;
	if(pChartItem->GetBWholeView())
		_stData.m_lZoomValue = _stData.m_lTotalCnt;
	else	
		_stData.m_lZoomValue = pChartItem->GetNOnePageDataCount();
	// 멀티차트의 선택여부 : 복수종목 - ojtaso (20070525)
	ST_SELCHART* lpSelChart;
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
//	if(((CStdDialog*)GetParent())->GetpMainBaseInfo()) // gm0604 added ..
//	{
		//============================================================
		//2005. 09. 05 한화 뷁
		long lFromTRdata = 0;
		if(bFromTRData)
			lFromTRdata  = 1;
		//============================================================

		if(m_pInputBarDlg)
			m_pInputBarDlg->SendMessage(UMSG_SETINITIALCHARTDATA,lFromTRdata,(LPARAM)&_stData);
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
						int m_lColSelectedOld = m_lColSelected;
						int m_lRowSelectedOld = m_lRowSelected;
						SetChartSelected(CPoint(nColIndex,nRowIndex));

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
	case UWM_ZOOMVALUE:
		{
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem == NULL) 
				break;

			CDRChartOcx *pChart = (CDRChartOcx *)GetDlgItem(wParam);		
			if(GetpChartItemSelected()==pChart&&m_bLastEnvUpdated)
			{
				//R버튼으로 발생하는 전체 초기화 라는 의미 05.12.05 선영대리 요청=================================
				if(pMsgFromChart->wParam == 0) 
				//=============================================================================
				if(m_pInputBarDlg)
					m_pInputBarDlg->SendMessage(UMSG_SETZOOMVALUE,pMsgFromChart->lParam);
			}
		}		
		break;
	case UWM_VIEWENDTIME:
		{
			//한화 요청으로 차트 스크롤시 달력을 변경시키는 거 하지 않는다.
			return 0l;
		}
		break;
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
			if(strCodeHead=="시간-차트")
			{
				CChartItem *pChart = (CChartItem *)GetDlgItem(wParam);
				SetTimeMark((LPCTSTR)strCodeTail,TRUE);
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
			if(pChart->GetDataTRCode()==QUERY_STRFX)
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
				if( pChartItem->GetType() == '0' && atoi( pChartItem->GetTickMinDayNumberRequested()) == 1)
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
					//aaaaaaaaaaaaaaa if(pSelItem->GetSelChart(pSelRQ, pSelChart, pMsgFromChart->wParam))
					if(pSelItem->GetSelChart(pSelRQ, pSelChart, FALSE))
					{
						if(!pMsgFromChart->wParam)	// 차트 삭제
						{
							((CStdDialog*)GetParent())->OnDeleteChartItem(pSelChart->m_strItemCode);
							pSelItem->OnDeleteChartItem(pSelRQ, pSelChart);
						}
						else						// 선택됨
						{
							//>> 예외처리 - vntsorl (20090106)
							if(m_bInitChartItem == TRUE)
								break;
							//<< 예외처리 - vntsorl (20090106)
							
							((CStdDialog*)GetParent())->OnSelectChartItem(pSelChart);
							pSelItem->OnSelectChartItem(pSelChart);
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
			CChartItem *pChart = (CChartItem *)GetDlgItem(wParam);
			CChartItem* pSelItem = GetpChartItemSelected();

			if (pChart == pSelItem)
			{
				((CInputBarDlg*)m_pInputBarDlg)->SetMultiTotalCount(pMsgFromChart->lParam);
			}
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
	//@Solomon-091106 차트파일이 너무 많이 생성되어 Chart폴더를 별도로 처리하기위해.
	//OCX에서 이것을 기준으로 처리를 하고 있는 듯함.
	CString szID;
	szID.Format("%s%s", ((CStdDialog*)GetParent())->m_strUserID, "\\Chart");
	return (long)(LPCSTR)szID;

	//기존루틴  (long)(LPCSTR)((CStdDialog*)GetParent())->m_strUserID;
}

LRESULT CChartMng::OnMagnifyingGlass( WPARAM wParam, LPARAM lParam)
{
	((CStdDialog*)GetParent())->OnMagnifyingglass((BOOL)wParam);
	return 0L;
}

void CChartMng::SetTimeMark(LPCTSTR p_szMarkTime, BOOL p_bWithScroll)
{
	EACH_CHARTJOB_START()
		pChartItem->SetTimeMark(p_szMarkTime,p_bWithScroll);
	EACH_CHARTJOB_END()	
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

	if(pChartItem==GetpChartItemSelected())
	{
		if(pChartItem->GetBReplayChartMode())
		{
			pChartItem->DrawReplayChartTitle(backup, pDC,&m_ftForTitle,&m_ftNormal,m_clStartSelected);
		}
		else
			pChartItem->DrawTitle(backup, pDC,&m_ftForTitle,&m_ftNormal,m_clStartSelected);			
	}
	else
	{
		if(pChartItem->GetBReplayChartMode())
		{
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
		}
		//====================================================================================
		pChartItem->MoveWindow(rect);	
		EACH_CHARTJOB_END()

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
					if( ((CStdDialog*)GetParent())->m_nChartType >= MINI_MULTI_CHART)
					{
						pChartItem->Create(this,GetRectOfChart(nRowIndex,nColIndex),m_nID,nRowIndex,nColIndex, m_strImagePath, TRUE);
						pChartItem->SetSmallChartMode(2);
					}
					else
					{
						if(!pChartItem->Create(this,GetRectOfChart(nRowIndex,nColIndex),m_nID,nRowIndex,nColIndex, m_strImagePath, TRUE))
						{
							delete pChartItem;
							return FALSE;
						}
					}					
					
					//<<< 20100108 
					CString strUserPath;
					strUserPath.Format("%s\\", ((CStdDialog*)GetParent())->GetUserID());
					pChartItem->SetEncUserID(strUserPath);



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
						char chType = m_pInputBarDlg ? (char)m_pInputBarDlg->SendMessage(UMSG_GETTYPEFROMINPUTBAR) : '0';
						long lNumber = m_pInputBarDlg ? m_pInputBarDlg->SendMessage(UMSG_GETTYPENUMFROMINPUTBAR) : 0;
						pChartItem->SetType(chType);
						pChartItem->SetTickMinDayNumber(lNumber);
					}
					if(m_lKey)
					{
						pChartItem->SetInit((long)this->GetSafeHwnd(), m_lKey, m_strRootPath, FALSE);
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
					
					INT  nAnalysisType = m_pExtInputBarDlg->SendMessage(UMSG_GETDRAGMODETYPE);
					
					pChartItem->SetUseRevisedValue			(bRevised);
					pChartItem->SetUseLogValue				(bLogPrice);
					
					pChartItem->EnableReceivingTimeShare	(pChartItem->m_CfgInfo.bRecvTimeShare	);
					pChartItem->EnableBoundMark				(pChartItem->m_CfgInfo.bShowHLBar		);

					pChartItem->SetBNormalizedMinuteTimeRule(pChartItem->m_CfgInfo.bRealCandle ? FALSE : TRUE);

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
	//<<20100308_JS.Kim 솔로몬
	//((CInputBarDlg*)m_pInputBarDlg)->m_ctrlCodeInput.SetFocus();
	((CInputBarDlg*)m_pInputBarDlg)->m_pCodeInput->SetFocus();
	//>>
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

	GetpChartItemSelected()->ResetOCX();
}

void CChartMng::InitInLoop()
{	
	m_bDontRequestDataInLoop = NULL;
	m_bUseRevisedInLoop = NULL;
	m_bNormalizedMinuteTimeRuleInLoop = NULL;
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
	if (!m_bIsInLoop)
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
						long lZoomValue = m_pInputBarDlg ? m_pInputBarDlg->SendMessage(UMSG_GETZOOMVALUE,m_pChartItemInLoop->GetType()) : 0;	
						if(lZoomValue > m_pChartItemInLoop->GetAllCnt())
							m_pChartItemInLoop->SetNew_OnePageDataCount(m_pChartItemInLoop->GetAllCnt());
						else
							m_pChartItemInLoop->SetNew_OnePageDataCount(lZoomValue);
					}
				}
				break;
			case LOOPTYPE_FIXDATE/*2*/:// fixdate
				m_pChartItemInLoop->ZoomToWithRightFix((short)m_lZoomValueInLoop,TRUE);
				break;
			case LOOPTYPE_UPDATE/*4*/:
				if(m_pChartItemInLoop!=m_pChartItemSelectedInLoop)
				{
					if(m_pChartItemInLoop->GetAllCnt()&&m_pChartItemInLoop->GetCntOfItemData()&&m_pChartItemInLoop->GetType()==m_chTypeInLoop) 
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
			/*
			else
			{
				//<<20100324_JS.Kim 파일에서 읽는 경우 Pair Tick 처리
				CString strItemCode;
				CChartItemData* pItemData = m_pChartItemInLoop->FindChartItemDataForQuery();
				if( pItemData )
					strItemCode = pItemData->GetCodeForRqstFromChartItemData();
				else
					strItemCode = m_pChartItemInLoop->GetDataCode();
				if( m_pChartItemInLoop->GetTypeRequested() == '0' && atoi( m_pChartItemInLoop->GetTickMinDayNumberRequested()) == 1)
						( ( CStdDialog *)GetParent())->StartRequestPairTick( strItemCode, E_RPTS_START);
				else
					( ( CStdDialog *)GetParent())->ClearPairTick( "DEFAULT_PAIR");
				//>>
			}
			*/
		}
		break;
	case LOOPTYPE_FIXDATE/*2*/:
		{
			//===========================================================
			//2005. 05. 23
			//===========================================================
			//>>20100211_JS.Kim 기간 동기화시에 Zoom 값도 동기화
			m_pChartItemInLoop->SetNew_OnePageDataCount(m_lZoomValueInLoop);
			m_pChartItemInLoop->ZoomToWithRightFix((short)m_lZoomValueInLoop,TRUE);
			//<<

			if( 
				  m_pChartItemInLoop->GetType()	!= DAY_DATA_CHART
				&&m_pChartItemInLoop->GetType()	!= WEEK_DATA_CHART
				&&m_pChartItemInLoop->GetType()	!= MONTH_DATA_CHART
				&&m_pChartItemInLoop->GetType()	!= YEAR_DATA_CHART
				)
			{
				if(!HeadInLoop()) PostInLoop();
				return TRUE;
			}
			// (2008/8/10 - Seung-Won, Bae) Support End Date if only it is not zero. (not minute and tick)
			if( m_dEndDateInLoop)
			{
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
			
			if(m_pChartItemInLoop->GetChart_NumericValueType()!=m_nChartMngs_ChartDataNumericValue)	
			{
				m_pChartItemInLoop->SetChartNumericValue(m_nChartMngs_ChartDataNumericValue);
			}
			

			if(m_pChartItemInLoop->GetBNormalizedMinuteTimeRule()!=m_bNormalizedMinuteTimeRuleInLoop)
			{
				m_pChartItemInLoop->SetBNormalizedMinuteTimeRule(m_bNormalizedMinuteTimeRuleInLoop );
			}

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
			m_bNeedToUpdateInLoop = FALSE;
			if(m_pChartItemInLoop->GetUseRevisedValue() != m_bUseRevisedInLoop)
			{
				m_pChartItemInLoop->SetUseRevisedValue(m_bUseRevisedInLoop);
				m_bNeedToUpdateInLoop = TRUE;
			}
			if(m_pChartItemInLoop->GetBNormalizedMinuteTimeRule() != m_bNormalizedMinuteTimeRuleInLoop)
			{
				m_pChartItemInLoop->SetBNormalizedMinuteTimeRule(m_bNormalizedMinuteTimeRuleInLoop);
				m_bNeedToUpdateInLoop = TRUE;
			}			
			if(m_pChartItemInLoop->GetDataCode()!=m_strCodeInLoop&&m_bJongmokLink)
			{
				m_pChartItemInLoop->SetCodeForQuery(m_strCodeInLoop,QUERY_STRFX,TRUE);
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
			//>>20100212_JS.Kim 확대 상태에서 지표동기화 처리
			if( m_bIndexLink )
			{
				m_lTypeInLoop = LOOPTYPE_UPDATE;
				m_bIndexLink = FALSE;
			}
			//<<
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
	case LOOPTYPE_FAKE_CANDLE/*7*/:// ShowFakedCandle
		{
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem == NULL) 
				break;

			GetpChartItemSelected()->SetBNormalizedMinuteTimeRule(m_bNormalizedMinuteTimeRuleInLoop);
			if(m_bDontRequestDataInLoop) 
			{
				if(!HeadInLoop()) PostInLoop();
				return TRUE;
			}
		}
		break;
	default:
		{
			if(!m_strCodeInLoop.IsEmpty())
			{
				switch(m_nMarketType)
				{
					case FX_CHART:
						{
							{
								CChartItem* pChartItem = NULL;
								pChartItem = GetpChartItemSelected();
								if(pChartItem == NULL) 
									break;

								GetpChartItemSelected()->SetCodeForQuery( m_strCodeInLoop, QUERY_STRFX,TRUE);
							}
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
				GetpChartItemSelected()->SetCodeForQuery( "|S_" + CStdDialog::GetRealItemCode( strItemCode), QUERY_STRFX, TRUE);
			}
			else
			{
				if( m_pChartItemInLoop->GetTypeRequested() == '0' && atoi( m_pChartItemInLoop->GetTickMinDayNumberRequested()) == 1)
						( ( CStdDialog *)GetParent())->StartRequestPairTick( strItemCode, E_RPTS_LOOP_START);
				else
				{
					( ( CStdDialog *)GetParent())->ClearPairTick( "DEFAULT_PAIR");
				}
			}
		}
		break;
	}

	if(!RequestChartData())
	{
		if(!HeadInLoop()) PostInLoop();
	}
	return TRUE;
}

BOOL CChartMng::InputCodeFromInputBar(CCodeDataFromInputBar* pCodeData,BOOL bForcedMode0 /*=FALSE*/)
{
	if(((CStdDialog*)GetParent())->m_nChartType==SINGLE_CHART)
		m_bJongmokLink = TRUE;

	// 공유가 들어왔기 때문에 InputBar쪽에서 강제로 코드를 날릴 필요가 없다고 설정한다.
	m_bNeedForceCode = FALSE; 
	 
	if(!m_bIsSystemTradingMode)
	{
		//nOpenedOwner  = 0 : 메뉴/툴바에서 열림
		//nOpenedOwner  = 1 : 마우스 오른쪽 버튼에서 열림 (종목코드 포함)
		//nOpenedOwner  = 2 : 스크립트,dll에서 화면 요청 (OpenMapData 포함)		
		//<<20100308_JS.Kim 솔로몬
		//UINT nOpenedOwner = ::SendMessage(((CStdDialog*)GetParent())->m_hViewInMain,RMSG_GETOPENEDOWNER,0,0);
		//>>
		if(!m_bLastEnvUpdated)
		{
			//=============================================================
			// 처음에 뜰때 단 한번만 호출된다. - 분할 저장 정보를 받는다.
			//=============================================================
			m_strStatic_ShareCode = pCodeData->m_strCode;	// Static으로 단 한번 적용한다.

			BOOL bResult = FALSE;
			bResult = LoadChart(((CStdDialog*)GetParent())->m_strConfig);

			if (!bResult) //로드할 내용이 없는 경우 로드가 끝났다는 플래그를 반드시 해준다.
			{
				SetBNoExistLoadCnts(TRUE); // 로드 할 게 전혀 없다.
			}
			else
			{
				//>> JS.Kim_20100929  solomon.. PAIR RQ는 저장하지 않도록 수정..
				//파일에서 읽는 경우 Pair Tick 처리
				CString strItemCode;
				CChartItemData* pItemData = GetpChartItemSelected()->FindChartItemDataForQuery();
				if( pItemData )
					strItemCode = pItemData->GetCodeForRqstFromChartItemData();
				else
					strItemCode = GetpChartItemSelected()->GetDataCode();
				if( GetpChartItemSelected()->GetTypeRequested() == '0' && atoi( GetpChartItemSelected()->GetTickMinDayNumberRequested()) == 1)
				{
					( ( CStdDialog *)GetParent())->StartRequestPairTick( strItemCode, E_RPTS_START);
					RequestChartData();

				}
				else
					( ( CStdDialog *)GetParent())->ClearPairTick( "DEFAULT_PAIR");
				//<<
			}

			m_bLastEnvUpdated = TRUE;
			m_strStatic_ShareCode = _T("");

			if (bResult)		// 로드 성공하면 바로 
				return TRUE;	// Return 해 버린다.
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

		pSelectedChartItem->SetCodeForQuery(pCodeData->m_strCode,QUERY_STRFX,TRUE);
// <-- [Edit]  강지원 2008/07/22
		//=========================================================================================
		
		pSelectedChartItem->SetEndDate(strEndDate);

		// gm0604 added For SystemTrading 2005.7.21 Start
		if(pCodeData->m_chType)
		{			
			GetpChartItemSelected()->SetType(pCodeData->m_chType);

			//edited by sy, nam 05. 08. 29 =============================================
			CString strTypeNum;
			strTypeNum.Format("%03d", pCodeData->m_lTypeNum);

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
			if( pCodeData->m_chType == '0' && pCodeData->m_lTypeNum == 1)
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

		RequestChartData();
	}		
	return TRUE;
}

BOOL CChartMng::SetType(char chType, long lNumber)
{
	// 틱(0)/분(1)/일(2)/주(3)/월(4)/년(5)/초(6)
	//>>20100226_JS.Kim	주기별로 별도 저장하는 경우에 확대상태에서 주기 바꾸면 확대플래그만 해제한다
	BOOL bOrgExtended = m_bChartExtended;
	if( !m_bSaveJipyoOneFile && m_bChartExtended )
	{
		//ExtendChart(!m_bChartExtended);
		m_bChartExtended = !m_bChartExtended;
	}
	//<<
	
	if(m_bTypeLink&&!m_bChartExtended)
	{
		InitInLoop();
		m_chTypeInLoop = chType;
		m_lNumberInLoop = lNumber;
		//>>20100226_JS.Kim		분할차트에서 한 분할에만 지표넣고 주기 동기화한 겅우 주기 변경할 때의 처리
		m_bChangeTypeMode = TRUE;
		//<<
		HeadInLoop();
	}
	else
	{
		
		//>>20100226_JS.Kim	주기별로 별도 저장하는 경우에 확대상태에서 주기 바꾸면 확대플래그 해제했던거 원래대로
		m_bChartExtended = bOrgExtended;
		//<<

//		if(GetpChartItemSelected()->GetDataCode()!="")
		{
			if(!GetpChartItemSelected()) return FALSE;

			char chOldType = GetpChartItemSelected()->GetType();
			GetpChartItemSelected()->SetType(chType);
			
			// 2005. 05. 20 -- 일 차트도 간격을 넘긴다.
			if(chType == TICK_DATA_CHART || chType == SEC_DATA_CHART || chType == MIN_DATA_CHART || 
				chType == DAY_DATA_CHART || chType == HOUR_DATA_CHART)
			{
				GetpChartItemSelected()->SetTickMinDayNumber(lNumber);
			}

			//>>20091222_JS.Kim 동부
			//if(!((CStdDialog*)GetParent())->IsMultiItemChart())
			//>>
			{
				m_bChangeTypeMode = TRUE;

				// 주기변경...
				Synchro_jipyo();
			}
			//=====================================

			// (2008/9/14 - Seung-Won, Bae) for Pair Tick
			CChartItem *pChartItem = GetpChartItemSelected();
			if( pChartItem && GetParent())
			{
				if(chType == '0' && lNumber == 1 && pChartItem->FindChartItemDataForQuery())
				{
					( ( CStdDialog *)GetParent())->StartRequestPairTick( pChartItem->FindChartItemDataForQuery()->GetCodeForRqstFromChartItemData());
				}
				else
				{
					( ( CStdDialog *)GetParent())->ClearPairTick( pChartItem->GetSelectedRQ());

					if(chOldType == TICK_DATA_CHART && chType != TICK_DATA_CHART)
					{
						//<<20100329_JS.Kim 여기서 왜 ClearChartData()를 Call하지 ....
						// 복수종목인 경우 이 함수로 인해 다른 RQ의 데이타가 다 지워지는 문제 발생하여 막음
						//GetpChartItemSelected()->ClearChartData(0);
						//>>
						GetpChartItemSelected()->LockWindowUpdate();
					}
					//{{JS.Kim_20100902 Solomon 분/틱차트 주기 설정창
					//	1Tick에서 N Tick으로 변경할때 ClearPairTick 으로 인해 원래 틱으로 돌아가는 문제 발생하여
					//	여기서 다시 N Tick으로 셋팅하여 준다
					GetpChartItemSelected()->SetTickMinDayNumber(lNumber);
					//}}

					//{{JS.Kim_20100917 1틱 -> N틱으로 전환시 매도/매수 구분이 잘못되어 실시간이 바뀌어 오는 버그 수정
					CString strCode = pChartItem->m_lpSelChart ? pChartItem->m_lpSelChart->m_strItemCode : pChartItem->GetDataCode();
					pChartItem->SetCodeForQuery( (pChartItem->m_strOrderType=="0" ? "|S_":"|B_") + CStdDialog::GetRealItemCode( strCode), QUERY_STRFX, TRUE);
					//}}
				}
				//<<20100402_JS.Kim.솔로몬 미니차트. 위의 틱처리하는 과정에서 chType이 예전값(Tick)으로 돌아가는 현상이 발생하여 여기서 한번 더 해준다.
				GetpChartItemSelected()->SetType(chType);
				//>>
			}

			// 복수종목 추가 : 복수종목 - ojtaso (20070118)
			RequestChartData(TRUE);
		}
	}
	//>>20100226_JS.Kim	주기별로 별도 저장하는 경우에 확대상태에서 주기 바꾸면 확대 해제한다
	m_bChartExtended = bOrgExtended;
	//<<
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
				STINPUTCODEDATA* pStInputCodeData = (STINPUTCODEDATA*)lParam;
				CString strCode = pStInputCodeData->m_strCode;
				UINT nDefault = pStInputCodeData->m_nDefault;
				UINT nMode = pStInputCodeData->m_nMode;
				nMode = ((CInputBarDlg*)m_pInputBarDlg)->GetCurrentMode(nDefault, nMode);
				GetParent()->SendMessage(UMSG_SENDSHAREDCODE, (WPARAM)(LPCTSTR)strCode);
			}
			break;
		case UMSG_GETJCODECHECKEDFROMCHART:	// keep going...			
			break;
		case UMSG_GETINC:
			return 0L;						// return 0(send mode)..
			break;
		default:
			return 1L;						// don't do antying..
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
					m_bAddModeFromTab = FALSE;
				}
				else if(nMode == NEW_ITEM)
				{
					m_bAddModeFromTab = TRUE;
				}
			}

	
#ifdef _WRITELOG
			// Log - ojtaso (20080107)
			((CStdDialog*)GetParent())->WriteToStringLog(_T("종목탭에서 종목 변경됨"), pStInputCodeData->m_strCode, pStInputCodeData->m_strCode.GetLength());
#endif
			STINPUTCODEDATA2	stInputCodeData2;
			stInputCodeData2.m_strCode = pStInputCodeData->m_strCode;
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
				m_pInputBarDlg->SendMessage(UMSG_INPUTCODETOINPUTBAR,(WPARAM)&stInputCodeData2);			
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
			InputCodeFromInputBar((CCodeDataFromInputBar *)wParam);
		}
		break;
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


			if(m_bLastEnvUpdated == FALSE) //로드가 다 안 끝났을 경우.
			{
				SetFocusToJMEdit_At_LoadEnd();
			}
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

				//<<< 20091109 by kmlee 데이타가 조회되지 않는 경우에.. 기존 챠트 내용을지우도록..
				// 필요없으면 생략해도 됨...
				CChartItem* pSelChartItem = GetpChartItemSelected();
				pSelChartItem->EmptyChart (TRUE, FALSE);
				//>>>
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

			
			long lZoomValue = m_pInputBarDlg ? m_pInputBarDlg->SendMessage(UMSG_GETZOOMVALUE,pSelChartItem->GetType()) : 0;	
			
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
			if(m_strTimeMarkReserved!=""
					&&(	pSelChartItem->GetType()!= WEEK_DATA_CHART
					&&	pSelChartItem->GetType()!= MONTH_DATA_CHART
					&&	pSelChartItem->GetType()!= DAY_DATA_CHART		))
			{
				pSelChartItem->SetTimeMark(m_strTimeMarkReserved,TRUE);				
			}
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
			
/*******************************************************************************************
/* 김선영 대리 요청으로 차트 TR을 받기 전에 차트에서 저장한 파일을 읽어온다.
/* 해서 아래 내용을 주석처리
/*******************************************************************************************/
//시작 
//*/ 
			if(m_bExistSyncChart)
			{

				m_bExistSyncChart = FALSE;		
			}
//끝 
//*/
			//종목 코드에 포커스가 가도록  한다.
			if(GetBNoExistLoadCnts() && m_bLastEnvUpdated && m_pInputBarDlg)
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
				}
				else if(nMode == OVERLAPP_ITEM)
				{
					SetAddMode(TRUE);
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
// --> [Edit]  BH.Ahn 2010/02/10	데이터 요청수 변경시 틱차트 데이터를 한번만 요청하는 문제 수정
			CChartItem *pChartItem = GetpChartItemSelected();
			if (!pChartItem)
			{
				return FALSE;
			}

			pChartItem->SetCntForRequest(wParam);
			// 복수종목 추가 : 복수종목 - ojtaso (20070118)

			long lNumber = 0;
			char chType = pChartItem->GetType();
			lNumber = atoi(pChartItem->GetTickMinDayNumber(-1));

			if( pChartItem && GetParent() )
			{
				if (chType == '0' && lNumber == 1 && pChartItem->FindChartItemDataForQuery())
					( ( CStdDialog *)GetParent())->StartRequestPairTick( pChartItem->FindChartItemDataForQuery()->GetCodeForRqstFromChartItemData());
				else
				{
					( ( CStdDialog *)GetParent())->ClearPairTick( pChartItem->GetSelectedRQ());
				}
			}

			RequestChartData(TRUE);

			pChartItem->SetSaveAllCount(wParam);
// <-- [Edit]  BH.Ahn 2010/02/10
		}
		break;
	case UMSG_SETMATRIX:
		{			
			SetMatrix(wParam,lParam);
			
// --> [Edit]  이도영 2008/09/22
// 0,0 으로 선택을 돌린다.
			SetChartSelected(CPoint(0,0),TRUE);			
// <-- [Edit]  이도영 2008/09/22


			//요청된 호가 Real 이  있다면 해제한다.			
		}
		break;
	case UMSG_REQUESTMORE:
		{
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
		}
		break;
	case UMSG_RESETCHART:
		{
			ResetChartAll();
		}
		break;
	case UMSG_RESETOCX:
		{
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

			
			//2007.01.26 by LYH 지표 정보를 파일에서 읽도록 수정
			CString strIndicondFile = m_strRootPath + "\\Icss\\indicond.cfg";//solomon
			CString strGroup;
			CString strJipyo = (LPCTSTR)wParam;
			::GetPrivateProfileString(strJipyo, "groupName", "", strGroup.GetBuffer(MAX_PATH), MAX_PATH, strIndicondFile);

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
					if(pSelChartItem->GetDataTRCode() != QUERY_STRFX)
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
	case UMSG_FIXDATE:
		{
			wParam = GetpChartItemSelected()->GetNOnePageDataCount();

			InitInLoop();			
			m_strEndDateInLoop = (LPCTSTR)lParam;
			m_lZoomValueInLoop = wParam;
			m_dEndDateInLoop = atof(m_strEndDateInLoop);
			m_lTypeInLoop = LOOPTYPE_FIXDATE/*2*/;// fixdate
			HeadInLoop();
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
	case UMSG_UPDATEREVISED:
		{
			BOOL bUseRevised = wParam;
			BOOL bDontRequestData = lParam;
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
	case UMSG_GETJONGMOKCODE:
		{	
			if(!GetpChartItemSelected()) return FALSE;
			if(GetpChartItemSelected()->GetDataTRCode()==QUERY_STRFX)
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
	case UMSG_SETNORMALIZEDMINUTETIMERULE:
		{
			BOOL bShowFackedCandle = wParam; //0. 실봉  1. 실봉 + 허봉
			BOOL bDontRequestData = lParam;
			
			//
			GetpChartItemSelected()->m_CfgInfo.bRealCandle = wParam > 0 ? false : true;
			UpdateShowingFackedCandle(bShowFackedCandle,bDontRequestData);
		}
		break;
	case UMSG_CHARTODRTRACE:
		{
			if(!GetpChartItemSelected()) return FALSE;
			long lFlag = (long)wParam;
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
		}
		break;
	case UMSG_CHARTGUIDELINE:
		{
			CChartItem* pSelChartItem = NULL;
			pSelChartItem = GetpChartItemSelected();
			if(!pSelChartItem) return FALSE;


			if( pSelChartItem->GetChart_NumericValueType() != WON_TYPE)
			{
				AfxMessageBox("원 단위 일때만 기준선이 적용 가능합니다");
				return FALSE;
			}
//@Solomon:091123SM068
//			pSelChartItem->InvokeAddIn("FX_ChartGuideLineAddin.dll:GUIDE_LINE",101,(long)pSelChartItem->GetSafeHwnd());

			
			CStringList strlistSetAll;
			strlistSetAll.AddTail(((CStdDialog*)GetParent())->GetAccountInfo());
			pSelChartItem->InvokeAddIn("FX_ChartGuideLineAddin.dll:GUIDE_LINE",NULL,(long)&strlistSetAll);
/*
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
						if(strCode.GetLength() != CODE_STOCK_LENGTH)
						{
							m_pFloatingDlg->m_bIconState = 1;
							return 1L;
						}
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
			if(strCodeShare=="시간_차트")
			{

				CString strTimeInfo = (LPCTSTR)lParam;
				SetTimeMark((LPCTSTR)strTimeInfo,TRUE);
			}
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
				strAnalFile = pChartItem->GetOpenAndSaveAnalFileName(TRUE);
				pChartItem->SaveandOpenFromAnalToolFile("", strAnalFile, FALSE, TRUE);
			}
			else
			{
				// 추세선 파일 읽음...
				strAnalFile = pChartItem->GetOpenAndSaveAnalFileName(TRUE);
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
	case UMSG_SETORDERLINK:
		{
			m_bOrderLink = wParam;
			// FX차트 주문 연동 - onlyojt (20091204)
			EACH_CHARTJOB_START()
				if (m_bOrderLink)
				{
					pChartItem->InvokeAddInStr("ChartOutsideCustomAddIn.dll:ORDER_LINE", "1", "ON");
					if( pChartItem->GetTypeRequested() == MIN_DATA_CHART && atoi(pChartItem->GetMinNumber()) <= 60 )
						pChartItem->InvokeAddInStr("ChartOutsideCustomAddIn.dll:ORDER_LINE", "2", "ON");
					else
						pChartItem->InvokeAddInStr("ChartOutsideCustomAddIn.dll:ORDER_LINE", "2", "OFF");
					pChartItem->InvokeAddInStr("ChartOutsideCustomAddIn.dll:ORDER_LINE", "3", (pChartItem->m_strOrderType=="0" ? "SELL":"BUY"));

					//pChartItem->InvokeAddInStr("ChartKoreaSorimachiAddIn.dll:USER_COMMENT", "1", "ON");
				}
				else
				{
					pChartItem->InvokeAddInStr("ChartOutsideCustomAddIn.dll:ORDER_LINE", "1", "OFF");
					pChartItem->InvokeAddInStr("ChartOutsideCustomAddIn.dll:ORDER_LINE", "2", "OFF");
					//pChartItem->InvokeAddInStr("ChartKoreaSorimachiAddIn.dll:USER_COMMENT", "1", "OFF");
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
		if(pChartItem->GetDataTRCode()==QUERY_STRFX)
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

	//>>20100212_JS.Kim 확대 상태에서 지표동기화 처리
	if(m_bChartExtended)
	{
		m_bUpdatedInChartExtended = TRUE;	
		m_bIndexLink = TRUE;
	}
	//<<

	InitInLoop();
	m_strArrayCodeInLoop.RemoveAll();
	m_strArrayTRCodeInLoop.RemoveAll();
	
	m_pChartItemSelectedInLoop = GetpChartItemSelected();
	m_lCntInLoop = m_pChartItemSelectedInLoop->GetCntOfItemData();
	m_chTypeInLoop = m_pChartItemSelectedInLoop->GetType();

	//m_bUseRateValueInLoop = m_pChartItemSelectedInLoop->GetUseRateValue();
	m_nChartMngs_ChartDataNumericValue = m_pChartItemSelectedInLoop->GetChart_NumericValueType();

	m_bNormalizedMinuteTimeRuleInLoop = m_pChartItemSelectedInLoop->GetBNormalizedMinuteTimeRule();
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
	
	return TRUE;
}

BOOL CChartMng::UpdateShowingFacked_JustOneChart(BOOL bNormalizedMinuteTimeRule,BOOL bDontRequestData)
{
	if(!GetpChartItemSelected()) return FALSE;

	GetpChartItemSelected()->m_CfgInfo.bRealCandle = bNormalizedMinuteTimeRule ? false : true; //05.11.25
	GetpChartItemSelected()->SetBNormalizedMinuteTimeRule(bNormalizedMinuteTimeRule);
	if(!bDontRequestData) 
		return RequestChartData();	

	return TRUE;
}


BOOL CChartMng::UpdateShowingFackedCandle(BOOL bNormalizedMinuteTimeRule,BOOL bDontRequestData)
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
		m_lTypeInLoop = LOOPTYPE_FAKE_CANDLE/*7*/; // ShowFackecCandle
		HeadInLoop();
	}
	m_bUpdatingShowingFackedCandle = FALSE;
	return TRUE;
}

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

	m_bUpdatingRevisedValue = FALSE;
	return TRUE;
}

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
		EACH_CHARTJOB_START()
			if(pChartItem->GetDataCode()!="")
			{
				pChartItem->HideScale(m_bHideScale);
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
		if(pSelChartItem->GetDataTRCode() != QUERY_STRFX)
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
	m_pChartItems[lRow1][lCol1]->MoveWindow(GetRectOfChart(lRow1,lCol1));
	InvalidateRect(GetRectOfChart(lRow2,lCol2,1));
	m_pChartItems[lRow2][lCol2]->MoveWindow(GetRectOfChart(lRow2,lCol2));
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
		if(!GetpChartItemSelected()) return ;
		InitInLoop();		
		m_bNeedToUpdateInLoop = FALSE;
		m_bUseRevisedInLoop = GetpChartItemSelected()->GetUseRevisedValue();
		m_bNormalizedMinuteTimeRuleInLoop = GetpChartItemSelected()->GetBNormalizedMinuteTimeRule();
		m_strCodeInLoop = GetpChartItemSelected()->GetDataCode();
		m_strEndDateInLoop = GetpChartItemSelected()->GetEndDate();
		m_chTypeInLoop = GetpChartItemSelected()->GetType();
		m_lTickMinNumberInLoop = atol(GetpChartItemSelected()->GetTickMinDayNumber());
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
					m_bInitChartItem = TRUE;
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
					//@유진추가

					//CDRChartOcx::ResetChartCfgWith(140);
					//2005. 07. 28 ==========================
					// 현재 구성이 1 * 1 또는 하나의 창만 보일 경우 
					// 호가창 내용 Clear & Init

					//>> 예외처리 - vntsorl (20090106)
					m_bInitChartItem = FALSE;
					//<< 예외처리 - vntsorl (20090106)
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
						if( pChartItem->GetType() == '0' && atoi( pChartItem->GetTickMinDayNumberRequested()) == 1)
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

	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	if( E_PRTS_ROTATE_START != ( ( CStdDialog *)GetParent())->GetRequestPairTickState())
		m_strMultiCodes = strMultiCodes;

//@유진추가
	((CStdDialog*)GetParent())->ChangeCode(strJCode);
//@유진추가
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

// --> [Edit]  강지원 2008/07/24	( 통신 패킷 만드는 부분인데... 값을 CString 으로 넘기기때문에 우리랑 맞지 않아서 수정함 )
	m_bChangeTypeMode = bChangeType;

	CChartItem* pChartItem = NULL;
	pChartItem = GetpChartItemSelected();
	if(!pChartItem) return FALSE;

	pChartItem->m_strOrderType = ((CInputBarDlg*)m_pInputBarDlg)->GetOrderType();

	stSend2Server* st = pChartItem->IsLoadingRQ() ? pChartItem->GetLoadedQueryData() : pChartItem->GetQueryData(bChangeType);

	if( st == NULL )	
	{
		return FALSE;
	}

	st->m_nType = 0;
	st->m_pWnd = this;
	st->m_strTRCode = GetpChartItemSelected()->GetTrCodeForQuery();

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	((CStdDialog*)GetParent())->WriteToStringLog(_T("서버에게 보낼 Data 가공"), pChartItem->GetSelectedItemCode(), pChartItem->GetSelectedItemCode().GetLength());
#endif

	GetParent()->SendMessage (UMSG_SEND2SERVER, 0, (long)st);	

	if ( st->m_strTRCode == QUERY_STRFX )
	{
		SM_fx112_InRec1* fx112InRec1 = (SM_fx112_InRec1*)st->m_pBuffer;
		delete fx112InRec1;
		fx112InRec1 = NULL;
	}

	delete st;
	st = NULL;

	return TRUE;
}

BOOL CChartMng::RequestChart_MarketData(CString		strTRCode,
										CString		strJipyoName,
										CChartItem* pSelChartItem,
										int			nDataRqst,
										BOOL		bIsNew
										)
{
	if(pSelChartItem == NULL) return FALSE;
	CString strPacket = pSelChartItem->GetMarketQueryData(strJipyoName, nDataRqst, bIsNew);


	if(strPacket=="")	
	{
		return FALSE;
	}
	
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
void CChartMng::SetRealJPriceData (void *pRealData2)
{
	CString szTmpCode;
	STREALTYPEDATA sndData;

	EACH_CHARTJOB_START()

	if (pChartItem->IsOnDrdsAdvise ())			
	{
		int nMarketType = ((CStdDialog*)GetParent ())->GetMarketTypeFromCode (pChartItem->GetDataCode ());

		pChartItem->SetRealPriceData (pRealData2, STOCK_REAL);

		double dPipLowest = 1.0;
		int nStart(0);

		CString		strPrice = "";
		CString		strOpen	= "";
		CString		strHigh = "";
		CString		strLow = "";
		CString		strPosition = "";

		CChartItemData* pChartItemData = pChartItem->GetItemDataAt (0);

		if (pChartItemData)
		{
			strPosition = pChartItemData->GetPipLowest ();
			nStart = strPosition.GetLength() - strPosition.Find(".") - 1;
			dPipLowest = atof(strPosition);
		}

		CRealData_Common_FX	*pRealData_Common_FX = (CRealData_Common_FX *)pRealData2;
		CRealData_Common	*pRealData_Common;

		szTmpCode = pChartItem->GetSelectedItemCode();
		//szTmpCode = szTmpCode.Right(7);

		//if(!::lstrcmp(szTmpCode, pRealData_Common_FX->m_strCode))
		if (!::lstrcmp (CStdDialog::GetRealItemCode (szTmpCode), pRealData_Common_FX->m_strCode))
		{
			int nOrderType = 0;
			if( szTmpCode.Left(3) == "|S_" )			nOrderType = 0;		//// 매도
			else if( szTmpCode.Left(3) == "|B_" )		nOrderType = 1;		//// 매수
			else										nOrderType = atoi(pChartItem->m_strOrderType);

			if( nOrderType == 0 )		// 매도
				pRealData_Common = &(pRealData_Common_FX->m_RealData_S);
			else						// 매수
				pRealData_Common = &(pRealData_Common_FX->m_RealData_B);

			sndData.nDataType = nMarketType;
//			sndData.szCode = (LPSTR)(LPCSTR)pRealData_Common->m_strCode;
			sndData.szCode = (LPSTR)(LPCSTR)szTmpCode;

			//strOpen.Format("%0.*f", nStart, atof(pRealData_Common.m_strOpen) * dPipLowest);
			sndData.oPrice = (LPSTR)(LPCSTR)pRealData_Common->m_strOpen;
			//strHigh.Format("%0.*f", nStart, atof(pRealData_Common.m_strHigh) * dPipLowest);
			sndData.hPrice = (LPSTR)(LPCSTR)pRealData_Common->m_strHigh;
			//strLow.Format("%0.*f", nStart, atof(pRealData_Common.m_strLow) * dPipLowest);
			sndData.lPrice = (LPSTR)(LPCSTR)pRealData_Common->m_strLow;

			//if (pChartItem->m_strOrderType == "0")
			//{
			//	strPrice.Format("%0.*f", nStart, atof(pRealData_Common.m_strOffer) * dPipLowest);
			//}
			//else
			//{
			//	strPrice.Format("%0.*f", nStart, atof(pRealData_Common.m_strBid) * dPipLowest);
			//}
			sndData.cPrice = (LPSTR)(LPCSTR)pRealData_Common->m_strPrice;

			pChartItem->InvokeAddIn("FX_ChartGuideLineAddin.dll:GUIDE_LINE",
							INVOKEMSG_RECEIVEREALDATA,
							(long)&sndData);
		}
	}

	EACH_CHARTJOB_END()
}

void CChartMng::SetRealUPriceData(char *pData, int nSize)
{
}


void CChartMng::SetRealFPriceData(char *pData, int nSize)
{
}

void CChartMng::SetRealOPriceData(char *pData, int nSize)
{
}

void CChartMng::SetRealHPriceData(char *pData, int nSize)
{
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
			if(pChartItem->GetDataCode()==pItem->GetOldChartCode())
			{
				return;
			}	
		}
	EACH_CHARTJOB_END()

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

	int nCnt = 0;
	CString strIndexFile = m_strUserPath + "\\FX_ChartIndex.dat";
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
	CString strIndexFile = m_strUserPath + "\\FX_ChartIndex.dat";
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
	if(bInRectOfChart&&GetpChartItemSelected()->GetDataTRCode()==QUERY_STRFX)
	{
		STSHAREDDATA stInfo;
		stInfo.m_szKey = "코드-코코2";
		stInfo.m_szValue = GetpChartItemSelected()->GetDataCode();
		stInfo.m_szValue.TrimRight(';');
		//((IMainDllObMain*)g_pMainDll)->m_pMainFrame->SendMessage(RMSG_CREATEOPENSHAREDATA,0, (LPARAM)&stInfo);		
		CWnd::OnRButtonUp(nFlags, point);
	}
}


void CChartMng::RequestAVGForGuideLine(BOOL bSelectedOnly)
{
	if(!GetpChartItemSelected()) return ;

	CUIntArray AryAvgForChartGuideLine;
	AryAvgForChartGuideLine.SetSize(4);
	GetpChartItemSelected()->InvokeAddIn("FX_ChartGuideLineAddin.dll",INVOKEMSG_GETAVGINFO,(long)&AryAvgForChartGuideLine);
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
}

void CChartMng::SetSendJongData(LPCTSTR lpQuoteCode,char chType, LPCTSTR lpMinNTickNumber)
{
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
//									"거래량지표",
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
	if(m_nMarketType != FX_CHART)
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
		if(!strcmp(p_strGroup,"특수지표"))
		{
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

	arrCode.RemoveAll ();
	
	EACH_CHARTJOB_START()

	if (pChartItem)
	{
		strCode = _T("");
		strCode = pChartItem->GetDataCode();
		strCode.TrimLeft();		
		strCode.TrimRight();
		
		if (strCode.GetLength ())
			arrCode.Add (strCode);
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
	bImaginaryCandle = pInfo->bRealCandle;
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
		if(m_nMarketType != FX_CHART) return;
		
		//==============================================================================================
		//2005. 06. 23 현재 분할된 차트의 갯수 1 * 1 상태일 경우 Floating Hoga 창을 생성한다.
		//==============================================================================================
		DestroyFloatingHogaWnd();
	
		m_pFloatingDlg = NULL;
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

		m_pRectFloatingHoga->top = rectChart.top + HEIGHTOFCAPTION+2 + 16;
		/////////////////////////////////////////

		m_pRectFloatingHoga->left = rectChart.left + 3;
		m_pRectFloatingHoga->right = m_pRectFloatingHoga->left + nWidth;
		m_pRectFloatingHoga->bottom = m_pRectFloatingHoga->top + nHeight;
		//======================================================================	
		
	
		m_pFloatingDlg->MoveWindow(m_pRectFloatingHoga);
		m_pFloatingDlg->SetWindowPos(&CWnd::wndTop, m_pRectFloatingHoga->left,m_pRectFloatingHoga->top, nWidth,nHeight, SWP_NOMOVE | SWP_NOSIZE |SWP_SHOWWINDOW);
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
		BOOL bNormalizedMinuteTimeRuleForSyncEnv	= pBaseChart->GetBNormalizedMinuteTimeRule();	//허수봉
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

		if(pEmptyChart->GetBNormalizedMinuteTimeRule()!=bNormalizedMinuteTimeRuleForSyncEnv)
		{
			pEmptyChart->SetBNormalizedMinuteTimeRule(bNormalizedMinuteTimeRuleForSyncEnv);
		}

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
	switch(chType)
	{
		case	SEC_DATA_CHART	:	m_pSecCfgChart	= pWnd; break;
		case	TICK_DATA_CHART	:	m_pTickCfgChart = pWnd; break;			
		case	MIN_DATA_CHART	:	m_pMinCfgChart	= pWnd; break;	
		case	DAY_DATA_CHART	:	m_pDayCfgChart	= pWnd; break;					
		case	HOUR_DATA_CHART	:	m_pHourCfgChart = pWnd; break;
		case	WEEK_DATA_CHART	:	m_pWeekCfgChart = pWnd; break;
		case	MONTH_DATA_CHART:	m_pMonthCfgChart= pWnd; break;
		case	YEAR_DATA_CHART	:	m_pYearCfgChart = pWnd; break;
	}
}

CChartItem* CChartMng::GetpChartCfgWnd(char chType)
{
	switch(chType)
	{
		case	SEC_DATA_CHART	:	return (CChartItem*)m_pSecCfgChart	; break;
		case	TICK_DATA_CHART	:	return (CChartItem*)m_pTickCfgChart ; break;			
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
	//>>20091222_JS.Kim 동부  RQ가 여러개 있는 경우만 개별 저장 안하도록 수정
	//// 추가 모드일때 : 복수종목 - ojtaso (20070209)
	//if(!m_bChangeTypeMode || m_bAddMode )
	//	return;
	/*
	if( GetpChartItemSelected()->GetType() != TICK_DATA_CHART && GetpChartItemSelected()->GetChartItemCount() > 1 )
		return;
	if( GetpChartItemSelected()->GetType() == TICK_DATA_CHART && GetpChartItemSelected()->GetChartItemCount() > 2 )
		return;
	*/
//	if( GetpChartItemSelected()->GetChartItemCount() > 1 )
//		return;
	//>>

	//<<20100329_JS.Kim 솔로몬
	if(m_bSaveJipyoOneFile)
		return;
	//>>

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

	//{{JS.Kim_20100903 Solomon	추세선 저장
	//if(chOriType == chRqstType) return; //같은 타입이면 저장하거나 로드하지 않는다.
	//if(chOriType == NULL)  chOriType = chRqstType;
	// 분석툴 종목별 저장 - ojtaso (20080728)
	BOOL bSaveAnalTool = GetSaveAnalTool();
	if(!bSaveAnalTool && chOriType == chRqstType) return; //같은 타입이면 저장하거나 로드하지 않는다.
	//}}
	

	CChartItem* pSaveChart	= NULL	;
	CChartItem* pLoadChart	= NULL	;

	//>>20120212_JS.Kim 지표별로 저장인 경우 그때그때 파일에 저장되므로 이전에 읽었던 값으로 셋팅하면 안된다
	//pLoadChart = GetpChartCfgWnd(chRqstType	);
	//>>

	if(pSyncChart)
	{
		//TR조회가 필요한 경우 - 시장지표 일때.
		//m_strArrayCodeInLoop.RemoveAll();    //TR조회시 필요한 종목코드 저장 
		//m_strArrayTRCodeInLoop.RemoveAll();  //TR조회로서 동기화가 필요한 TR내역 저장 
		if(pLoadChart->GetSafeHwnd())
		{
			m_bExistSyncChart = TRUE;
		}


		//호가 요청을 못하도록 한다.
		if(m_pInputBarDlg)
			m_pInputBarDlg->SendMessage(UMSG_ENABLE_SENDHOGATR, FALSE, 0);	

		//조회 이전 차트의 정보를 저장한다.
		if(pSyncChart->HasGraph("가격차트"))
		{			
			CString sSaveFileName = pSyncChart->GetOpenAndSaveFileName();
			pSyncChart->SaveandOpenFromFile("", (LPCTSTR)sSaveFileName, FALSE, TRUE);

			// 분석툴 종목별 저장 - ojtaso (20080728)
			BOOL bSaveAnalTool = GetSaveAnalTool();
			if(bSaveAnalTool)
			{
				// 2011.01.31 by SYS >> 분차트 YYYY(년) 추가 및 분석툴 공유 기능 추가
				//sSaveFileName = pSyncChart->GetOpenAndSaveFileName(TRUE);
				
				sSaveFileName = pSyncChart->GetOpenAndSaveAnalFileName(TRUE);
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

		if(m_bSaveJipyoOneFile)
		{
			sJipyoSaveFileName = sJipyoFileName1 + "_"	;
		}
		else
		{
			switch(chRqstType)
			{	
			case	SEC_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Sec_"	; break;
			case	TICK_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Tick_"	; break;			
			case	MIN_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Min_"	; break;	
			case	DAY_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Day_"	; break;					
			case	HOUR_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Hour_"	; break;
			case	WEEK_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Week_"	; break;
			case	MONTH_DATA_CHART:	sJipyoSaveFileName = sJipyoFileName1 + "_Month_"; break;
			case	YEAR_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName1 + "_Year_"	; break;
			default					:	return ;
			}
		}

		sJipyoSaveFileName += JIPYOSYNC_FILE;

		CFileFind ff;
		CString sTempPath;
		sTempPath = m_strUserPath + "\\" + sJipyoSaveFileName;
		BOOL bResult = ff.FindFile(sTempPath);
		if(!bResult)
		{
			pSyncChart->ResetChart();
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
	if(m_nMarketType == FX_CHART) nLen = CODE_STOCK_LENGTH;
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
	CString strCode;
	//chType: 틱(0)/분(1)/일(2)/주(3)/월(4)
	CChartItem* pChartItem;
	strCode = GetpChartItemSelected()->GetDataCode();

	//>> (2008/12/22 - Seung-Lyong, Park) 빈차트 선택후 일주월분 처리시 종목컨트롤 종목으로 처리
	if(strCode.IsEmpty())
	{
		strCode = ((CInputBarDlg*)m_pInputBarDlg)->GetCurrentCode();
		if(strCode.IsEmpty())
			return FALSE;
	}
	//<< (2008/12/22 - Seung-Lyong, Park) 빈차트 선택후 일주월분 처리시 종목컨트롤 종목으로 처리

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
	//{1*1 차트로 초기화 요청
	ReplaceChart(0,0,m_lRowSelected,m_lColSelected);
	SetMatrix(1,1);
	//}
	if(m_pInputBarDlg)
		((CInputBarDlg*)m_pInputBarDlg)->SetMode(CHANGE_ITEM);

	m_bHideScale = FALSE;
	m_bHideInnerToolBar = FALSE;

	CChartItem* pChartItem = NULL;
	pChartItem = GetpChartItemSelected();
	if(pChartItem == NULL || !pChartItem->m_bReceivedDataChart) 
		return;
	pChartItem->ResetChart();

	// 선택된 차트에 새로 요청할 데이터 default 갯수 셋팅
	if( ((CStdDialog*)GetParent())->m_nChartType==MINI_MULTI_CHART ||
		((CStdDialog*)GetParent())->m_nChartType==MINI_SINGLE_CHART ) 
		pChartItem->SetCntForRequest(90);
	else
		pChartItem->SetCntForRequest(DEFAULTCNTATONCE);

	pChartItem->SetSaveAllCount(0);

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

	pChartItem->InvokeAddIn("FX_ChartGuideLineAddin.dll:GUIDE_LINE",200,(long)pChartItem->GetSafeHwnd());
	pChartItem->InvokeAddIn("ChartPatternAddin.dll",0,200);
	pChartItem->InvokeAddInSTIndex("ChartSTIndexAddIn.dll",200,0);	// 시스템트레이딩 전체초기화

	m_pTickCfgChart		= NULL;
	m_pSecCfgChart		= NULL;
	m_pMinCfgChart		= NULL;
	m_pDayCfgChart		= NULL;
	m_pHourCfgChart		= NULL;
	m_pWeekCfgChart		= NULL;
	m_pMonthCfgChart	= NULL;
	m_pYearCfgChart		= NULL;

	m_bExistSyncChart	= FALSE;

	// 차트 초기화때 모든 리얼 해제 : 복수종목 - ojtaso (20070621)
	((CStdDialog*)GetParent())->ResetChartAll();

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
		if(m_nMarketType == FX_CHART)
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
	//<<20100308_JS.Kim 솔로몬
	//( ( CInputBarDlg *)m_pInputBarDlg)->m_ctrlCodeInput.SetDataText( CStdDialog::GetRealItemCode( strRetryCode));
	((CInputBarDlg*)m_pInputBarDlg)->m_pCodeInput->SetCode(CStdDialog::GetRealItemCode(strRetryCode));
	((CInputBarDlg*)m_pInputBarDlg)->SetShareCodeChanged(FALSE);
	//>>

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
	//<<20100316_JS.Kim 솔로몬
//	HWND hChildFrame = (HWND)GetParent()->GetParent()->SendMessage(WM_USER+104);
//	if ( !hChildFrame) return;
	//>>
	
// --> [Edit]  강지원 2008/07/29	( 유진에서는 메인에서 추가하는 메뉴를 받아오기위해서 메시지를 날렸지만... 일단 FX에서는 뺌... )
	BCMenu* pCommonMenu = new BCMenu;
	ASSERT ( pCommonMenu );

	pCommonMenu->CreatePopupMenu();

	m_menuMain.RemoveAll();
	menuChart.RemoveAll();

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

	for(i = 0; i < nCommonMenuCount; ++i)
	{
		CString strMenuString;
		UINT nCommandID = pCommonMenu->GetMenuItemID(i);
		pCommonMenu->GetMenuItemInfo(nCommandID, &info);
		pCommonMenu->GetMenuText(i, strMenuString, MF_BYPOSITION);

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
			m_menuMain.Add(nCommandID);
			menuChart.Add(ID_PLM_ADD_MENU_BEGINMAIN+i);
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
//<<20100308_JS.Kim 솔로몬
	/*
	EU_T3930_OUTREC Outrec;
	int nPos = 0;
	char szTemp[100];

	memset(szTemp, 0x00, sizeof(szTemp));
	memcpy(szTemp, pData + nPos, 4);

	nPos += 4;

	int nCnt = atoi(szTemp);

	ORDER_INFO * pOrerInfo;
	for (int i = 0; i < nCnt; i++)
	{
		memcpy(&Outrec, pData + nPos, sizeof(Outrec));
		nPos += sizeof(Outrec);
		
		pOrerInfo = new ORDER_INFO;

		memset(szTemp, 0x00, sizeof(szTemp));
		memcpy(szTemp, Outrec.szAccount, sizeof(Outrec.szAccount));
		pOrerInfo->szAccount = szTemp;
		pOrerInfo->szAccount.TrimRight();

		memset(szTemp, 0x00, sizeof(szTemp));
		memcpy(szTemp, Outrec.szCurNo, sizeof(Outrec.szCurNo));
		pOrerInfo->szCurNo = szTemp;
		pOrerInfo->szCurNo.TrimRight();

		memset(szTemp, 0x00, sizeof(szTemp));
		memcpy(szTemp, Outrec.szSide, sizeof(Outrec.szSide));
		pOrerInfo->szSide = szTemp;
		pOrerInfo->szSide.TrimRight();

		memset(szTemp, 0x00, sizeof(szTemp));
		memcpy(szTemp, Outrec.szPrice, sizeof(Outrec.szPrice));
		pOrerInfo->szPrice = szTemp;
		pOrerInfo->szPrice.TrimRight();

		memset(szTemp, 0x00, sizeof(szTemp));
		memcpy(szTemp, Outrec.szAmount, sizeof(Outrec.szAmount));
		pOrerInfo->nAmount = atoi(szTemp);

		memset(szTemp, 0x00, sizeof(szTemp));
		memcpy(szTemp, Outrec.szCustomItem, sizeof(Outrec.szCustomItem));
		pOrerInfo->szCustomItem = szTemp;

		CString strKey;

		strKey.Format("%s%s", pOrerInfo->szCustomItem.Left(21), pOrerInfo->szCustomItem.Mid(25));
		strKey.TrimRight();

		m_mapOrder.SetAt(strKey, (CObject*)pOrerInfo);

	}
	*/
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
//>>
	return TRUE;
}

BOOL CChartMng::SetRealOrderInfo(ORDER_INFO* pOrderInfo, int nRealType)
{
//<<20100308_JS.Kim 솔로몬
	/*
	CString strKey;
	strKey.Format("%s", pOrderInfo->szCustomItem);

	ORDER_INFO * pTempOrerInfo;
	if (nRealType == MICHEGEUL_PID)
	{
		if (pOrderInfo->szOrderType == "URE") // 정정인 경우
		{
			strKey.Format("%s", pOrderInfo->szOrgCustomItem);
			if (m_mapOrder.Lookup(strKey, (CObject*&)pTempOrerInfo) == FALSE)
			{
				// 체결이 왔는데 미체결에서 없는 건 이상한 것임
				return FALSE;
			}
			m_mapOrder.RemoveKey(strKey);
			pTempOrerInfo->szPrice = pOrderInfo->szPrice;
			pTempOrerInfo->szCustomItem = pOrderInfo->szCustomItem;
			strKey.Format("%s", pOrderInfo->szCustomItem);

			m_mapOrder.SetAt(strKey, (CObject*)pTempOrerInfo);
		}
		else
		{
			if (m_mapOrder.Lookup(strKey, (CObject*&)pTempOrerInfo) == FALSE)
			{
				pTempOrerInfo = new ORDER_INFO;
				{
					pTempOrerInfo->szAccount = pOrderInfo->szAccount;
					pTempOrerInfo->szCurNo = pOrderInfo->szCurNo;
					pTempOrerInfo->szSide = pOrderInfo->szSide;
					pTempOrerInfo->szPrice = pOrderInfo->szPrice;
					pTempOrerInfo->nAmount = pOrderInfo->nAmount;
					pTempOrerInfo->szCustomItem = pOrderInfo->szCustomItem;
					pTempOrerInfo->szOrgCustomItem = pOrderInfo->szOrgCustomItem;

					m_mapOrder.SetAt(strKey, (CObject*)pTempOrerInfo);
				}
			}
			else
			{
				return FALSE;
			}
		}
	}
	else if (nRealType == CHEGEUL_PID)
	{
		if (pOrderInfo->szOrderType == "DE") // 취소인 경우
		{
			strKey.Format("%s", pOrderInfo->szOrgCustomItem);
			strKey.TrimRight();
		}
		if (m_mapOrder.Lookup(strKey, (CObject*&)pTempOrerInfo) == FALSE)
		{
			// 체결이 왔는데 미체결에서 없는 건 이상한 것임
			strKey.Format("%s", pOrderInfo->szOrgCustomItem);
			strKey.TrimRight();
			if (m_mapOrder.Lookup(strKey, (CObject*&)pTempOrerInfo) == FALSE)
			{
				return FALSE;
			}
			if (pTempOrerInfo->nAmount <= pOrderInfo->nAmount)
			{
				//전체 체결 완료
				m_mapOrder.RemoveKey(strKey);
				delete pTempOrerInfo;
			}
			else
			{
// --> [Edit] 이도영 2009-06-02
// 부분체결시 남아있는 주문에 대해 그리드에서 수정되면 반영 필요.
//				pTempOrerInfo->nAmount -= pOrderInfo->nAmount;
				//전체 체결 완료
				m_mapOrder.RemoveKey(strKey);
				delete pTempOrerInfo;
// <-- [Edit] 이도영 2009-06-02
			}
		}
		else
		{
			if (pTempOrerInfo->nAmount <= pOrderInfo->nAmount)
			{
				//전체 체결 완료
				m_mapOrder.RemoveKey(strKey);
				delete pTempOrerInfo;
			}
			else
			{
// --> [Edit] 이도영 2009-06-02
// 부분체결시 남아있는 주문에 대해 그리드에서 수정되면 반영 필요.
//				pTempOrerInfo->nAmount -= pOrderInfo->nAmount;
				//전체 체결 완료
				m_mapOrder.RemoveKey(strKey);
				delete pTempOrerInfo;
// <-- [Edit] 이도영 2009-06-02
			}
		}
	}

	EACH_CHARTJOB_START()
		pChartItem->Invalidate();
	EACH_CHARTJOB_END()		
	*/
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
//>>
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
												QUERY_STRFX, TRUE);
		if( p_bWithRequest) RequestChartData();
	}
}

//<<20100308_JS.Kim 솔로몬
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
//>>

CString	CChartMng::GetCurrentCode()
{
	return ((CInputBarDlg*)m_pInputBarDlg)->GetCurrentCode();
}