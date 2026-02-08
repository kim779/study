// ChartItem.cpp: implementation of the CChartItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartItem.h"
#include "./define/ChartMsgDef.h"
#include "ChartMng.h"
#include "../COMMONTR/ChartRealConverter.h"
#include "./define/MessageDefine.h"
#include "../../inc/RealUpdate.h"
#include "../chart_common/IRealReceiver.h"
#include "../../../SrcSite/include/CodeCtrl.h"

#include "StdDialog.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CString SetComma(CString strNumber, BOOL bShowSign=FALSE);
const UINT RMSG_GET_LINKINFODATA = ::RegisterWindowMessage(_T("RMSG_GET_LINKINFODATA"));

#define DEFAULT_CHARTDATA_COUNT_INT		400
#define DEFAULT_CHARTDATA_COUNT_CHAR	"400"

#define WMU_GET_LAST_KEY			WM_USER + 2237	// AupAgent에서 MainFrame에 마지막 조회된 Key를 요청한다.
LPCTSTR szSetAllForSignalnew[] = 
{
	"EN=1",
	"HI=0",
	"ST=",
	"GS=",
	"LS=",
	"XCM=신호등",
	"XID=",
	"CFT=굴림체;12;0;0;0;0;",
	"TKS=1",
	"UDF=1",
	"OCN=NULL"
};

LPCTSTR szSetAll[] = 
{
	"CM=Name,DRChart0",
	"AN=DRChart0",
	"ID=3,4,0",
	"RC=2,24,397,248",
	"WS=WS_CHILD|WS_VISIBLE",
	"WSE=",
//	"BT=0",
//	"BC=0",
//	"RC=1",
//	"ROWC=1",
//	"CC=1",
//	"BG=-1",
//	"BBT=1",

	"BDT=0",
//	"SLT=0",							// 실선(graph 선택시)
	"SLT=2",							// 점선(graph 선택시)
	"WV=0",
//	"WV=0",							==> Big Bang Original Source에서 사용
	"PDC=200",
	"BAC=10",			// 최대 블럭수
	"GDD=0",
//	"LM=5",	// left 마진
//	"RM=0",	// right 마진
//	"TM=5",	// top 마진
//	"BM=0",	// bottom 마진
	"AGM=1",												// 컨텍스트 메뉴 
	"EFN=",
	"IFN=",
	"EU=",
	"HI=0",
	"IR=0",
	//"TO=내부\r\n이전;;;\r\n스크롤바;;;\r\n확대,축소;개수;10개;\r\n전체보기;;;\r\n십자선;수치조회창 사용안함;;\r\nDisplay수 설정바;;;\r\n이후;;;\r\n"
	"TO=내부\r\n스크롤바;;;\r\n확대,축소;개수;10개;\r\n전체보기;;;\r\nDisplay수 설정바;;;\r\n"
	"외부\r\n"
	"RButton\r\n",

	"CL=BlockColumn;\r\n"
	"Block;\r\n"
	"자료일자;1;2;/;:;0;0;0;\r\n"
	"VertScaleGroup;\r\n"
	"지표 Data;1;2;0;\r\n"
	"VertScaleGroupEnd;\r\n"
	"BlockEnd;\r\n"
	"BlockColumnEnd;\r\n",

	"PO=\r\n",//1;자료일자;;6;HHMMSS;유;\r\n",
	"LSL=선물코드5;",
	"RL=",
	"URD=1",
	"MBT=3",
	"USESKIN=0",
//	"CBKG=255,255,255",
//	"CBKB=188,198,223",
//	"CB=255,255,255",
//	"CBB=0,0,0",
//	"CSL=204,204,204",
//	"CST=0,0,0",
//	"CGB1=8,189,246",
//	"CGB2=128,182,0",
//	"CGB3=253,122,4",
//	"CGB4=150,19,147",
//	"CGB5=255,133,11",
//	"CGB6=128,0,255",
// 부은선물 수정 (추세선 색)
	"CAT=0,0,160",
//	"CAT=82,103,146",

	"BR=row;0;216;\r\n"
	"column;0;387;",

//	"CSL2=204,204,204",
	"RDB=0",
	"UIDWCL=0",
	"CDB=0",
//	"CIW=255,255,255",
	"CFT=굴림체;12;0;0;0;0;",
	"USK=0",
	"UCTT=1",
	"UDTT=0",
	"NRBMT=1",
	"BSWMB=0",
	"BCBS=1",
	"BBMD=1",
	"BST=1",
	"BSG=1",
	"BUSRBM=1",
	"NHG=0",
	"NVG=0",
//	"CLTI=255,255,255",
	"NGRBT=1",
// --> [Edit] HS.Kim 2009-04-01
//	"NHPS=10000",
	"NHPS=2050",
// <-- [Edit] HS.Kim 2009-04-01
	"NMDCOOP=5",
	"!MapSize=0,0,400,250",
	"BRAUAP=0",
	"NIO=4",//"NIO=6",//"NIO=7",
	"GSL=시간-그리드4;시간-차트2;호가-차트2;"
	"PGCNTTR=1"		// JSR 20060221 ADD : From Doori
};

LPCTSTR szSetAllNoToolbar[] = 
{
	"CM=Name,DRChart0",
	"AN=DRChart0",
	"ID=3,4,0",
	"RC=2,24,397,248",
	"WS=WS_CHILD|WS_VISIBLE",
	"WSE=",
	"BT=0",
	"BC=0",
//	"RC=1",
	"ROWC=1",
	"CC=1",
	"BG=-1",
	"BBT=1",

	"BDT=0",
//	"SLT=0",							// 실선(graph 선택시)
	"SLT=2",							// 점선(graph 선택시)
	"WV=0",
//	"WV=0",							==> Big Bang Original Source에서 사용
	"PDC=200",
	"BAC=10",			// 최대 블럭수
	"GDD=0",
	"LM=5",	// left 마진
	"RM=0",	// right 마진
	"TM=5",	// top 마진
	"BM=0",	// bottom 마진
	"AGM=1",												// 컨텍스트 메뉴 
	"EFN=",
	"IFN=",
	"EU=",
	"HI=0",
	"IR=0",
	"TO=내부\r\n\r\n스크롤바;;;\r\n확대,축소;개수;10개;\r\n전체보기;;;\r\nDisplay수 설정바;;;\r\n"
//	"TO=내부\r\n이전;;;\r\n스크롤바;;;\r\n확대,축소;개수;10개;\r\nDisplay수 설정바;;;\r\n이후;;;\r\n"
//	"TO=내부\n스크롤바;;;\nGraph형태;;;\n선굵기;;;\n색설정;;;\n수치조회창;팝업 사용;수치Dialog 사용;\n십자선;수치조회창 사용안함;;\n이전;;;\n이후;;;\n확대,축소;개수;10개;\n돋보기;;;\r\n"
	"외부\r\n"
	"RButton\r\n",

	"CL=BlockColumn;\r\n"
	"Block;\r\n"	
	//20080915 이문수 >>
	//"자료일자;4;2;/;:;0;0;0;\r\n"
	"자료일자;1;2;/;:;0;0;0;\r\n"
	//20080915 이문수 <<
	"VertScaleGroup;\r\n"
	"지표 Data;1;2;0;\r\n"
//	"지표 Data;1;3;0;\r\n"

 
//=========================================================================================
//From_LTH (2003.7.1 ) 
// 아래에서 AddGraph롤 통해 설정하므로 주석처리함
//	"Graph;\r\n"
//	"선형;;실시간 지수;0;0;0;0;0;0;\r\n"
//	"1;0;0;Graph1;실시간지수;0;17,0,0;17,0,0;0,0,255;0,0,255;255,0,255;255,0,255;1;1;1;1;1;0; \r\n"
//	"GraphEnd;\r\n"
//=========================================================================================

/*	"Graph;\r\n",
	"선형;;매도지수;0;0;0;0;0;0;\r\n"
	"1;0;0;Graph1;매도지수;0;0,0,255;0,0,255;0,0,255;0,0,255;255,0,255;255,0,255;1;1;1;1;1;0; \r\n"
	"GraphEnd;\r\n"
	"Graph;\r\n"
	"선형;;매수지수;0;0;0;0;0;0;\r\n"
	"1;0;0;Graph1;매수지수;0;204,0,0;204,0,0;0,0,255;0,0,255;255,0,255;255,0,255;0;1;1;1;1;0; \r\n"
	"GraphEnd;\r\n"
	"Graph;\r\n"
	"선형;;거래소지수;0;0;0;0;0;0;\r\n"
	"1;0;0;Graph1;거래소지수;0;0,102,0;0,119,0;0,0,255;0,0,255;255,0,255;255,0,255;1;1;1;1;1;0; \r\n"
	"GraphEnd;\r\n"*/
	"VertScaleGroupEnd;\r\n"
	"BlockEnd;\r\n"
	"BlockColumnEnd;\r\n",

	"PO=\r\n",//1;자료일자;;6;HHMMSS;유;\r\n",
//	"1;실시간 지수;;10;× 0.01;유;\r\n"
//	"1;매도지수;;10;× 0.01;유;\r\n"
//	"1;매수지수;;10;× 0.01;유;\r\n"
//	"1;거래소지수;;10;× 0.01;유;",
//	"PO=1;자료일자;;6;HHMM  ;유;\r\n",		==> Big Bang Original Source에서 사용
//	"1;실시간 지수;;10;× 0.01;유;\r\n",

	"LSL=선물코드5;",

//	"RL=KP2.time.코드-실시간지수;"
//	"KP2.time.코드-실시간지수;"
//	"KP2.medojisu.코드-실시간지수;"
//	"KP2.mesujisu.코드-실시간지수;"
//	"KP2.upjisu.코드-실시간지수;",
	"RL=",
//	"RL=SC0.time.선물코드;"
//	"SC0.price.선물코드;",

	"URD=1",
	"MBT=3",
//	"CBKG=208,209,213",
	"USESKIN=0",
	"CBKG=255,255,255",
	"CBKB=188,198,223",
//	"CBKB=0,0,0",
//	"CBKB=255,255,255",
//	"CB=236,236,236",
	"CB=255,255,255",
//	"CBB=231,233,230",
	"CBB=0,0,0",
//	"CSL=255,255,255",
//	"CSL=204,204,204",
	"CST=0,0,0",
	"CGB1=8,189,246",
	"CGB2=128,182,0",
	"CGB3=253,122,4",
	"CGB4=150,19,147",
	"CGB5=255,133,11",
	"CGB6=128,0,255",
// 부은선물 수정 (추세선 색)
	"CAT=0,0,160",
//	"CAT=82,103,146",


	"BR=row;0;216;\r\n"
	"column;0;387;",

//	"CSL2=204,204,204",
	"RDB=0",
	"UIDWCL=0",
	"CDB=0",
	"CIW=255,255,255",
	"CFT=굴림체;12;0;0;0;0;",
	"USK=0",
	"UCTT=1",
	"UDTT=0",
	"NRBMT=1",
	"BSWMB=0",
	"BCBS=1",
	"BBMD=1",
	"BST=1",
	"BSG=1",
	"BUSRBM=1",
	"NHG=0",
	"NVG=0",
	"CLTI=255,255,255",
	"NGRBT=1",
// --> [Edit] HS.Kim 2009-04-01
//	"NHPS=10000",
	"NHPS=2050",
// <-- [Edit] HS.Kim 2009-04-01
	"NMDCOOP=5",
	"!MapSize=0,0,400,250",
	"BRAUAP=0",
	"NIO=6",//"NIO=4",//"NIO=7",
	"GSL=시간-그리드4;시간-차트2;"
//	"SLHW=시간_차트,1,0,0x00000000;시간_그리드,2,0,0x00000000;", //05. 08. 29 한화에서 쓰는 공유방식.
	//"PAGECOUNTTR=1" //05.11.28 추가
	"PGCNTTR=1"		// JSR 20060221 ADD : From Doori
/*
	"CBKG=255,255,255",
	"CBKB=0,0,0",
	"CB=255,255,255",
	"CBB=0,0,0",
	"CSL=128,128,128",
	"CST=0,0,0",
	"CGB1=0,136,0",
	"CGB2=255,55,255",
	"CGB3=255,51,51",
	"CGB4=0,109,219",
	"CGB5=255,133,11",
	"CGB6=128,0,255",
	"CAT=0,0,0",

	"BR=row;0;216;\r\n"
	"column;0;387;",

	"CSL2=128,128,128",
	"RDB=0",
	"UIDWCL=0",
	"CDB=0",
	"CIW=203,228,203",
	"CFT=굴림체;12;0;0;0;0;",
	"USK=0",
	"UCTT=0",
	"UDTT=0",
	"NRBMT=0",
	"BSWMB=1",
	"BCBS=1",
	"BBMD=0",
	"BST=0",
	"BSG=1",
	"BUSRBM=1",
	"NHG=0",
	"NVG=0",
	"CLTI=255,255,255",
	"NGRBT=1",
	"NHPS=2048",
	"NMDCOOP=5",
	"!MapSize=0,0,400,250",
	"BRAUAP=0",
*/
};

#define COLOR_UP		RGB(255,0,0)
#define COLOR_DOWN		RGB(65,65,255)
#define COLOR_STEADY	CLR_FONT_DEFAULT
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartItem::CChartItem()
{
	m_chSign = '3';
	m_bOnAdvise = FALSE;
	m_bInitalChart = FALSE;
	m_bNext = FALSE;
	m_lCntForRequest = 0;
	m_chType = NULL; /*DAY_DATA_TYPE*/
	//m_chTypeForRequest = DAY_DATA_CHART;
	SetChartRequestType(DAY_DATA_CHART);
	m_strTickMinDayForRequest = "001";	// 2Byte Tick & Min & Day Number
	m_strTickMinDay = "001";
	m_nChartNumericValueType = 0;
	
	//2005. 05. 24 add by sy, nam ==================================
	m_strTick = _T("001");
	m_strSec = _T("005");
	m_strMin = _T("001");
	m_strDay = _T("001");
	//==============================================================
	m_lPtrST = NULL;	// data pointer Of System trading is added by gm0604. 2005.7.21

	m_bRevisedPrice = FALSE;
	//m_bRateValue = FALSE;
	m_pStdDialog = NULL;
	ResetData();
	//m_lColIndex = -1;
	//m_lRowIndex = -1;
	SetRowNCol(-1, -1);
	m_nSmallChartMode = 0;
	m_bLoadChartInfo = FALSE;
	m_lNOnePageDataCount = 0;//05.09.09 0==>140
	m_bShowLock = TRUE;
	m_plDllWnd = NULL;
	m_bBuild = FALSE;
	m_bUseInnerControl = FALSE;

	m_nCntOverLap	= -1;    // 중첩된 종목 Cnt;
	m_bOverLapping	= FALSE; // 중첩 모드 Flag
	m_bLogPrice		= FALSE; // 로그 값 사용

	m_nChartNumericValueType = 0;

	m_pIL_Symbol = NULL;
	m_bSharedOn = FALSE;

	m_bReceivedDataChart = FALSE;

	Init_ChartCfgInfo();
	

	m_bReplaychartMode = FALSE;

	m_pDCForReplay = NULL;

	m_strOpen = "";
	m_strHigh = "";
	m_strLow = "";

	m_bAddInLoadPattern = FALSE;
	m_bAddInLoadPeriod = FALSE;
	m_bAddInLoadSTIndex = FALSE;
	m_bAddInLoadNews = FALSE;

	//2007.01.17 add by LYH  툴바버튼 동기화
	m_bDrawAutoTrendLine = FALSE;
	m_bDrawAutoPattern = FALSE;
	m_bDrawCandlePattern = FALSE;
	m_bDrawNews = FALSE;

	// 선택된 차트 정보는 외부dll에서 관리토록 수정 : 복수종목 - ojtaso (20070504)
	ST_SELCHART* pSelChart = new ST_SELCHART;
	m_mapChartItem.SetAt(_T("DEFAULT"), pSelChart);

	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	m_bLoadRQ =	FALSE;
	m_posLoadingRQ = NULL;
	m_strKey.Empty();
	m_lpSelChart = pSelChart;

	m_strSelectedRQ = _T("DEFAULT");

	m_bLoadDefaultRQ = FALSE;
	m_bNowInitialState = FALSE;

	m_pIRealReceiver = NULL;

// 
	m_strOrderType	= "0";	// '0':매도, '1':매수
	m_strNextKey	= "";
// <-- [Edit]  이도영 2008/09/11

	// (2008/9/28 - Seung-Won, Bae) Init Request Data Count
	m_nSaveAllCount = DEFAULT_CHARTDATA_COUNT_INT;

	m_pChartMng = NULL;
}

CChartItem::~CChartItem()
{
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::~CChartItem::Begin", _T("멀티차트 종료"), 13);
#endif

	if(m_pDCForReplay)
	{
		ReleaseDC(m_pDCForReplay);
		m_pDCForReplay = NULL;
	}

	// 선택된 차트 정보는 외부dll에서 관리토록 수정 : 복수종목 - ojtaso (20070504)
	POSITION pos = m_mapChartItem.GetStartPosition();
	ST_SELCHART* pSelChart = NULL;
	CString strKey;
	while(pos)
	{
		m_mapChartItem.GetNextAssoc(pos, strKey, (LPVOID&)pSelChart);
		delete pSelChart;
	}

	m_mapChartItem.RemoveAll();

	// 모든 리얼정보 삭제
	pos = m_mapChartItemRealData.GetStartPosition();
	ST_CHARTITEM_REALDATA* lpChartItemRealData;
	while(pos)
	{
		m_mapChartItemRealData.GetNextAssoc(pos, strKey, (LPVOID&)lpChartItemRealData);
		if(lpChartItemRealData)
			delete lpChartItemRealData;
	}

	m_mapChartItemRealData.RemoveAll();
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::~CChartItem::End", _T("멀티차트 종료"), 13);
#endif

	if(m_pIRealReceiver)
	{
		m_pIRealReceiver->Release();
		m_pIRealReceiver = NULL;
	}
}

//BEGIN_EVENTSINK_MAP(CChartItem, CDRChartOcx)
//    //{{AFX_EVENTSINK_MAP(CTOcx)
//	ON_EVENT(CChartItem, 1000, 1 /* OnLButtonDown */, OnOnLButtonDownTocxctrl1, VTS_I2 VTS_I2)
//	//}}AFX_EVENTSINK_MAP
//END_EVENTSINK_MAP()

#include "Atlconv.h"

long CLSIDFromProgID_GZone( const char *p_szProgID, LPCLSID p_pClsID)
{
	USES_CONVERSION;
	LPCOLESTR szOleProgID = T2COLE( p_szProgID);
	HRESULT hResult = ::CLSIDFromProgID( szOleProgID, p_pClsID);
	if( hResult == S_OK) return 0;
	else if( hResult == CO_E_CLASSSTRING) return -1;
	else if( hResult == REGDB_E_WRITEREGDB) return -2;
	return -3;
}

BOOL CChartItem::Create(CWnd *pWnd,LPRECT lpRect,int nID,long lRow,long lCol, LPCTSTR lpImagePath, BOOL bUseInnerControl)
{
	m_pStdDialog = NULL;

	// (2008/11/21 - Seung-Won, Bae) for No Ctrl.
	m_pChartMng = ( CChartMng *)pWnd;

	CString sTempPath;
	sTempPath = m_pChartMng->m_strRootPath + "\\Data\\";
	BOOL lResult = CDRChartOcx::Create(NULL,WS_VISIBLE|WS_CHILD,*lpRect,pWnd,nID);
	if(!lResult)
		return FALSE;
	
	SetNChartMode(2);

	m_pStdDialog = (CStdDialog*)pWnd->GetParent();

#ifdef _DEBUG
	if(m_pStdDialog == NULL)
		AfxMessageBox("CChartItem::Create...pStdDialog is NULL");
#endif

	m_nID = nID;

	SetRowNCol(lRow, lCol);
	CString strImagePath = m_strImagePath = lpImagePath;
	CString strImageUp;
	CString strImageDown;
	m_bUseInnerControl = bUseInnerControl;

	if(bUseInnerControl)
	{
		m_btMaxmize.Create("",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_CLIPSIBLINGS,CRect(0,0,0,0),pWnd,(m_nID+1));
// (2008/9/13 - Seung-Won, Bae) Remove Init Button.
		m_btInit.Create("",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_CLIPSIBLINGS,CRect(0,0,0,0),pWnd,(m_nID+2));
		m_btMaxmize.SetWindowPos(&CWnd::wndBottom,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		m_btInit.SetWindowPos(&CWnd::wndBottom,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

		m_btMaxmize.FP_vSetImage(&(((CStdDialog*)m_pStdDialog)->m_ImgList_W15H15), 0, 2);	
		m_btMaxmize.FP_vSetToolTipText("최대화/복원");

// (2008/9/13 - Seung-Won, Bae) Remove Init Button.
		m_btInit.FP_vSetImage(&(((CStdDialog*)m_pStdDialog)->m_ImgList_W15H15), 8, 10, FALSE, _T(""), FALSE, 11);	
		m_btInit.FP_vSetToolTipText("삭제");
	}

	//차트 보여주는 선물/주식/업종/해외 구분.
	if(m_pStdDialog)
		m_nMarketType = ((CStdDialog*)m_pStdDialog)->m_nMarketType;

	return lResult;
	//================================================================================================================================
}

void CChartItem::ResetData()
{
	SetDrdsAdvise(FALSE);
	m_bInitalChart = FALSE;
	CTime t = CTime::GetCurrentTime();
	CString strDate = t.Format("%Y%m%d");
	SetEndDateRequested(strDate);
	SetEndDate(strDate);

	m_bNext = FALSE;
	m_lCntForRequest = DEFAULTCNTATONCE;
	m_lAllCnt = 0;
	m_lPtrST = NULL;// gm0604 added 2005.9.6
	m_strKorName.Empty();

	//==========================================================
	for(int i = 0; i < m_arrayMarketTRData.GetSize();i++)
	{
		CChartItemData* pChartItemData = NULL;
		pChartItemData = m_arrayMarketTRData.GetAt(i);
		if(pChartItemData)
			pChartItemData->SetBNextMarketData(FALSE);
	}
	//==========================================================

	// 수정주가 클릭시 전체수 고정시키기 위해 추가 by LYH 2007.01.11
	m_nTotCountForRevised = 0;
}


///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	차트를 초기화한다.
//  수정    :   sy, nam
//  수정일자:   2005.05.10
//  수정이유:   BaseDesk를 한화에서 안 쓰므로 인해 직접 HTS실행 경로를 넘긴다.
//////////////////////////////////////////////////////////////////////////////

void CChartItem::SetInit(long p_lDllWnd, long lKey, LPCTSTR strHtsPath, BOOL bNoToolBar /*=FALSE*/)
{
	m_plDllWnd = p_lDllWnd;
	m_pBaseDesk2 = (CXIBaseDesk*)lKey;
	CDRChartOcx::SetBaseDesk((long)&m_xIBaseDesk);
	CDRChartOcx::SetHtsPath(strHtsPath);
	//CDRChartOcx::SetContainerDllInfo((long)p_lDllWnd,GetDlgCtrlID());
	if( GetSafeHwnd()) CDRChartOcx::SetContainerDllInfo((long)p_lDllWnd,GetDlgCtrlID());
	SetOutsideCommonInterface( m_pChartMng->m_lMltchrtDlgParentClsPointer); 
	SetNChartMode(2);
	SetAllProperties(bNoToolBar);	

	// 복수종목을 사용할지 : 복수종목 - ojtaso (20070904)
	SetMultiItemMode(((CStdDialog*)m_pStdDialog)->IsMultiItemChart());

#ifdef _DEBUG
	LoadAddIn("FX_ChartGuideLineAddin.dll:GUIDE_LINE");
#else	
	LoadAddIn("FX_ChartGuideLineAddin.dll:GUIDE_LINE");
#endif

	//수정 :KHD
	m_bAddInLoadSTIndex = TRUE;
	LoadAddIn("ChartSTIndexAddIn.dll");
	InvokeAddIn("ChartSTIndexAddIn.dll", 3, (long)this);

	//{{ 2007.06.26 by LYH 주문선, Excel Import 추가
	LoadAddIn("ChartPeriodSummaryAddIn.dll:ORDER_LINE");
	LoadAddIn("ChartDataListViewerAddIn.dll:EXCEL_IMPORT");
	//}}
	//{{ 2007.06.26 by LYH Undo,Redo 추가
	LoadAddIn("ChartPeriodSummaryAddIn.dll:ORDER_LINE");
	LoadAddIn("ChartCommonAddIn.dll:UNDO_REDO_MANAGER;");
	//}}

	// 2008.10.15 by LYH >> 패닝 <<
	//[A00000516]솔로몬: FX마진 종합차트, 해외선물 종합차트의  Y축 스케일에서 마우스로 클릭 후 스케일 조정하는 기능을 막아달라. 주문시에만 동작하도록.
	//LoadAddIn("ChartCommonAddIn.dll:PAN;");
//하단 데이터 조회
	LoadAddInWithOverExclusive( "ChartCommonAddIn.dll:MCHART_DVIEW;");
	InvokeAddIn( "ChartCommonAddIn.dll:MCHART_DVIEW", 0, ( long)m_pChartMng->m_lMltchrtDlgParentClsPointer);

	//자동스크롤
	LoadAddIn("ChartCommonAddIn.dll:AUTO_SCROLL");
	LoadAddIn("ChartCommonAddIn.dll:PRICE_YSCALE_SETUP");
	LoadAddInWithOverExclusive("ChartAnalysisToolAddIn.dll:NIDLG_OVER");

	// Set Pointer & StdDialog Handle
	m_listAddIn.AddTail("FX_ChartGuideLineAddin.dll:GUIDE_LINE");
	SendMessageToInvoke(INVOKEMSG_INIT,NULL,0);
// --> [Edit]  이도영 2008/09/12
// 
	// FX차트 주문 연동 - onlyojt (20091204)
	LoadAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE");
	LoadAddIn("ChartKoreaSorimachiAddIn.dll:ASKED_PRICE_CKSA");
	//// FX차트 잔고내역 보이기
	//LoadAddIn("ChartKoreaSorimachiAddIn.dll:USER_COMMENT");

	CString strPointer;
	CMapStringToOb * pMap = &((CChartMng*)GetParent())->m_mapOrder;
	strPointer.Format("%d", pMap);
	InvokeAddInStr("ChartKoreaSorimachiAddIn.dll:ASKED_PRICE_CKSA", "1", (LPCTSTR)strPointer);
// <-- [Edit]  이도영 2008/09/12
}


BOOL CChartItem::SetAllProperties(BOOL bNoToolbar /*=FALSE*/)
{
	CStringList strlistSetAll;
	if(bNoToolbar)
	{
		for(int i=0;i<sizeof(szSetAllNoToolbar)/sizeof(LPCTSTR);i++)
			strlistSetAll.AddTail(szSetAllNoToolbar[i]);	
	}
	else
	{
		for(int i=0;i<sizeof(szSetAll)/sizeof(LPCTSTR);i++)
			strlistSetAll.AddTail(szSetAll[i]);	
	}

	CDRChartOcx::SetAllProperties2((long)&strlistSetAll);
	strlistSetAll.RemoveAll();
	return TRUE;
}

void CChartItem::ResetOCX(BOOL bNotCodeInputInit/*= TRUE*/)//종목 코드 초기화가 Default, TRUE이면 초기화 안한다.
{	
	Init_ChartCfgInfo();
	ResetData();
	DestoryItemData();
	GetParent()->Invalidate();	
	CDRChartOcx::ShowInnerToolBar(FALSE);
	//CDRChartOcx::EmptyChart(TRUE, FALSE);	
	CString strChartList =	"BlockColumn;\r\n"
		"Block;\r\n"
		//20080915 이문수 >>
		//"자료일자;4;2;/;:;0;0;0;\r\n"
		"자료일자;1;2;/;:;0;0;0;\r\n"
		//20080915 이문수 <<
		"VertScaleGroup;\r\n"
		"지표 Data;1;2;0;\r\n"
		"VertScaleGroupEnd;\r\n"
		"BlockEnd;\r\n"
		"BlockColumnEnd;";
	CDRChartOcx::EmptyChart2(strChartList, TRUE, FALSE);
	m_bBuild = FALSE;

	if(bNotCodeInputInit == FALSE)
	{
		 //종목초기화 하지 않는다. 차트가 로드된후 단 한번만 불린다.
		((CChartMng*)GetParent())->SendInitChartData(FALSE);
	}
	else
	{
		((CChartMng*)GetParent())->SendInitChartData();

		//@유진추가
		ST_CHARTITEM_REALDATA* pChartItemRealData;
		if(m_mapChartItemRealData.Lookup(m_strSelectedItemCode, (LPVOID&)pChartItemRealData))
		{
			if(pChartItemRealData->m_nRefCount > 1)
				pChartItemRealData->m_nRefCount--;
			else
			{
				delete pChartItemRealData;
				m_mapChartItemRealData.RemoveKey(m_strSelectedItemCode);
			}
		}
		//@유진추가
	}
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	최대화 버튼, 닫기버튼의 위치를 잡아준다
//////////////////////////////////////////////////////////////////////////////
void CChartItem::SetButton(const LPRECT lpRect)
{
	//long lTrCode = atol(GetDataTRCode());
	CString strTRCode = GetDataTRCode();
	CRect rect = lpRect;
	CRect rectMaxmize(0,0,0,0);
// (2008/9/13 - Seung-Won, Bae) Remove Init Button.
	CRect rectInit(0,0,0,0);
	
	if(!rect.IsRectNull())
	{	
// (2008/9/13 - Seung-Won, Bae) Remove Init Button.
		rectMaxmize.left  = rect.right-WIDTHOFTITLEBUTTON-POSXOFMAXMIZE;
		rectMaxmize.top   = rect.top + POSYOFMAXMIZE + 1;
		rectMaxmize.right = rect.right-POSXOFMAXMIZE;
		rectMaxmize.bottom = rect.top + POSYOFMAXMIZE + HEIGHTOFTITLEBUTTON;

	    rectInit.left =  rect.right-WIDTHOFTITLEBUTTON-POSXOFINIT;
		rectInit.top =	rect.top + POSYOFINIT + 1;
		rectInit.right = rect.right-POSXOFINIT;
		rectInit.bottom = rect.top + POSYOFINIT + HEIGHTOFTITLEBUTTON;
	}
	if(m_btMaxmize.GetSafeHwnd())
	{
		m_btMaxmize.MoveWindow(rectMaxmize);
	}
// (2008/9/13 - Seung-Won, Bae) Remove Init Button.
	if(m_btInit.GetSafeHwnd())
	{
		m_btInit.MoveWindow(rectInit);
	}
}


///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	차트의 Title을 Draw한다.
//////////////////////////////////////////////////////////////////////////////
void CChartItem::DrawTitle(CRect rectIn,CDC* pDC,CFont *pFt, CFont *pFtNormal, COLORREF crBackIn )
{
	// 복수종목 실시간 : 복수종목 - ojtaso (20070523)
	ST_CHARTITEM_REALDATA* pChartItemRealData;
	if(!m_mapChartItemRealData.Lookup(m_strSelectedItemCode, (LPVOID&)pChartItemRealData))
		return;

	//>> 예외처리 - vntsorl (20090106)
	if(m_arrayChartItemData.GetSize() <= 0)
		return;
	//<< 예외처리 - vntsorl (20090106)

	// 복수종목 실시간 : 복수종목 - ojtaso (20070523)
	m_strKorName = pChartItemRealData->m_strItemName;

	CRect rect = rectIn;

// (2008/9/13 - Seung-Won, Bae) Remove Init Button.
	if(m_btInit.GetSafeHwnd())
	{
		rect.right -= WIDTHOFTITLEBUTTON;
	}
	if(m_btMaxmize.GetSafeHwnd())
	{
		rect.right -= POSXOFMAXMIZE;
	}	

	COLORREF crText = NULL;
	COLORREF crTextSise = NULL;
	COLORREF crBackSise = NULL;
	///////////////////////////////////////////////////////////////////////////////	
	CFont *pFtDefault = (CFont *)pDC->SelectObject(pFt);

	CRect rectTitlePos;
	CRect rectTitleType1;	
	CRect rectTitleType2;
	CRect rectTitleJongmok;

	CRect rectTitleName;
	CRect rectTitlePrice;
	CRect rectTitleChange;
	CRect rectTitleRate;
	CRect rectTitleVolume;
	CRect rectTitleOpenHighLow;

	CString strTitlePos;

	CString strTitleName;
	CString strTitlePrice;
	CString strTitleChange;
	CString strTitleRate;
	CString strTitleVolume;
	CString strTitleOpenHighLow;

	CString strTitleType;

	CSize szTitlePos;

	if(m_bUseInnerControl)
	{
		long lCols = ((CChartMng*)GetParent())->GetCols();
		long lCntOfChart = ((CChartMng*)GetParent())->GetCntOfChart();
		strTitlePos.Format("[%d / %d]", m_lColIndex + 1 + m_lRowIndex*lCols,lCntOfChart);
		szTitlePos = pDC->GetOutputTextExtent(strTitlePos);
		rectTitlePos.SetRect(rect.right-szTitlePos.cx,rect.top+4,rect.right,rect.bottom);
	}
	else
	{
		szTitlePos.cx = 0;
		szTitlePos.cy = 0;
	}
	BOOL bShortTitleMode = FALSE;

	if(!m_strKorName.IsEmpty())
	{
		//long lTrCode = atol(GetDataTRCode());
		CString strTRCode = GetDataTRCode();
		CString strSign = "";
		switch(pChartItemRealData->m_chSign)
		{
		case DOWN:			// Down
			strSign = "▼";			
			if(pChartItemRealData->m_strChange.GetLength()>0 && pChartItemRealData->m_strChange[0] == '-')
				pChartItemRealData->m_strChange = pChartItemRealData->m_strChange.Mid(1);
			break;
		case DOWN_LIMIT:	// DownLimit
			strSign = "↓";
			if(pChartItemRealData->m_strChange.GetLength()>0 && pChartItemRealData->m_strChange[0] == '-')
				pChartItemRealData->m_strChange = pChartItemRealData->m_strChange.Mid(1);
			break;	
		case UP:
			strSign = "▲";			
			break;
		case UP_LIMIT:
			strSign = "↑";			
			break;
		}

		if (strTRCode == QUERY_STRFX)
		{			
			//	데이터의 포맷을 맞춰주기위해서... 다시 계산 ( 0번째에 들어있는것을 봄.. 어차피 1개밖에 안들어있으니까.. )
			CChartItemData* pChartItemData = m_arrayChartItemData.GetAt (0);
			
			if (pChartItemData)
			{
 				CString strPipLowest = pChartItemRealData->m_strPipLowest;
				CString strPrice = "", strOpen = "", strHigh = "", strLow = "";
/*
				int nPos(0);
				if ( strPipLowest.Find('.') >= 0 )
					nPos = strPipLowest.GetLength() - strPipLowest.Find(".") - 1;

				strPrice.Format ("%0.*f", nPos, atof (pChartItemRealData->m_strPrice) * atof (strPipLowest));
				strOpen.Format ("%0.*f", nPos, atof (pChartItemRealData->m_strOpen) * atof (strPipLowest));
				strHigh.Format ("%0.*f", nPos, atof (pChartItemRealData->m_strHigh) * atof (strPipLowest));
				strLow.Format ("%0.*f", nPos, atof (pChartItemRealData->m_strLow) * atof (strPipLowest));
*/
				strPrice = pChartItemRealData->m_strPrice;
				strOpen = pChartItemRealData->m_strOpen;
				strHigh = pChartItemRealData->m_strHigh;
				strLow = pChartItemRealData->m_strLow;

				// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
				strTitleName.Format(" %s ", CStdDialog::GetRealItemCode( m_strKorName));
				
				//{{ 2007.06.26 by LYH 예상체결인 경우 현재가 앞에 (예) 붙임
				if(m_chSign == '9')	//예상체결
					strTitlePrice.Format("(예) %s ",::SetComma(strPrice));
				else
					strTitlePrice.Format(" %s ",::SetComma(strPrice));
				//}}

				strTitleVolume = "";		// 20080915 이문수 거래량이 없기때문에>>

				if (pChartItemRealData->m_strOpen.GetLength () > 0)
					strTitleOpenHighLow.Format ("시:%s 고:%s 저:%s", ::SetComma(strOpen), ::SetComma(strHigh), ::SetComma(strLow));
			}
		}

		CSize szTitleName = pDC->GetOutputTextExtent (strTitleName);
		CSize szTitlePrice = pDC->GetOutputTextExtent (strTitlePrice);
		CSize szTitleChange = pDC->GetOutputTextExtent (strTitleChange);
		CSize szTitleRate = pDC->GetOutputTextExtent (strTitleRate);
		CSize szTitleVolume = pDC->GetOutputTextExtent (strTitleVolume);
		CSize szTitleOpenHighLow = pDC->GetOutputTextExtent (strTitleOpenHighLow);

		CRect rectShift;
		CRect rectUnion;
		CRect rectUnionTitle;
		BOOL  bSetEmpty = FALSE;

		rectUnionTitle.SetRectEmpty ();
		rectShift = rect;
		rectShift.top += 4;
		rectShift.OffsetRect (1, 0);
 
		rectTitleName.SetRect (rectShift.left, rectShift.top,rectShift.left + szTitleName.cx, rectShift.top + szTitleName.cy);
		rectUnionTitle.UnionRect(rectUnionTitle, rectTitleName);

		rectShift.left = rectTitleName.right + 2;
		rectTitlePrice.SetRect (rectShift.left, rectShift.top, rectShift.left + szTitlePrice.cx, rectShift.top + szTitlePrice.cy);
		rectUnion.UnionRect (rect,rectTitlePrice);
		if (rectUnion != rect)
		{
			rectTitlePrice.SetRectEmpty ();
			bSetEmpty = TRUE;
		}
		else
		{
			rectUnionTitle.UnionRect (rectUnionTitle, rectTitlePrice);
		}

		rectShift.left = rectTitlePrice.right + 2;
		rectTitleChange.SetRect(rectShift.left,rectShift.top,rectShift.left+szTitleChange.cx,rectShift.top + szTitleChange.cy);
		rectUnion.UnionRect(rect,rectTitleChange);
		if(rectUnion!=rect||bSetEmpty)
		{
			rectTitleChange.SetRectEmpty();
			bSetEmpty = TRUE;
		}
		else
		{
			rectUnionTitle.UnionRect(rectUnionTitle,rectTitleChange);
		}

		rectShift.left = rectTitleChange.right + 2;
		rectTitleVolume.SetRect(rectShift.left,rectShift.top,rectShift.left+szTitleVolume.cx,rectShift.top + szTitleVolume.cy);
		rectUnion.UnionRect(rect,rectTitleVolume);
		if(rectUnion!=rect||bSetEmpty)
		{
			rectTitleVolume.SetRectEmpty();
			bSetEmpty = TRUE;
		}
		else
		{
			rectUnionTitle.UnionRect(rectUnionTitle,rectTitleVolume);
		}

		rectTitleJongmok = rectUnionTitle;
		rectTitleJongmok.InflateRect(2,3);
		if(rectTitleJongmok!=m_rectTitleJongmok)
		{
			m_rectTitleJongmokPast = m_rectTitleJongmok;
			m_rectTitleJongmok = rectTitleJongmok;		
		}

		rectShift.left = rectTitleVolume.right + 5;
		rectTitleOpenHighLow.SetRect(rectShift.left,rectShift.top,rectShift.left+szTitleOpenHighLow.cx,rectShift.top + szTitleOpenHighLow.cy);
		rectUnion.UnionRect(rect,rectTitleOpenHighLow);
		if(rectUnion!=rect||bSetEmpty)
		{
			rectTitleOpenHighLow.SetRectEmpty();
			bSetEmpty = TRUE;
		}

	}

	char chType = NULL;

	if(m_chType == NULL)
		if(m_chTypeForRequest == NULL)		
			chType = DAY_DATA_CHART;
		else
			chType = m_chTypeForRequest;
	else
		chType = m_chType;


	if(chType!=NULL)
	{
		// 틱(0)/분(1)/일(2)/주(3)/월(4)
		switch(chType)
		{
		case TICK_DATA_CHART:
			strTitleType.Format("[%d틱]",atoi((LPCTSTR)m_strTick));
			break;
		case SEC_DATA_CHART:
			strTitleType.Format("[%d초]",atoi((LPCTSTR)m_strSec));
			break;
		case MIN_DATA_CHART:
			if("300" == m_strMin) // 30초
				strTitleType.Format("[30초]");
			else
				strTitleType.Format("[%d분]",atoi((LPCTSTR)m_strMin));
			break;
		case DAY_DATA_CHART:
			strTitleType.Format("[%d일]",atoi((LPCTSTR)m_strDay));
			break;
		case HOUR_DATA_CHART:
			strTitleType.Format("[%d시]", atoi((LPCTSTR)m_strHour));
			break;
		case WEEK_DATA_CHART:
			strTitleType.Format("[주]");
			break;
		case MONTH_DATA_CHART:
			strTitleType.Format("[월]");
			break;
		case YEAR_DATA_CHART:
			strTitleType.Format("[년]");
			break;
		default:
			strTitleType = _T("[]");
			break;
		}

		if(strTitleType.GetLength())
		{
			CSize szTitleType = pDC->GetOutputTextExtent(strTitleType);
			rectTitleType1.SetRect(rect.right-szTitleType.cx-szTitlePos.cx,rect.top+4,rect.right-szTitlePos.cx,rect.bottom);
			rectTitleType2.SetRect(rect.right-szTitleType.cx,rect.top+4,rect.right,rect.bottom);
		}

		if(rectTitleType1.left < rectTitleOpenHighLow.right)
		{
			rectTitleOpenHighLow.SetRectEmpty();
		}

	}	

	if(!m_strKorName.IsEmpty())
	{
		CRect rectInflate;
		///////////////////////////////////////////////////////////////////////////////
		//	JSR-20051128 ADD Begin.
		// 복수종목 관리map을 이용 : 복수종목 - ojtaso (20070612)
		switch(pChartItemRealData->m_chSign)
		{
		case UP_LIMIT:	// UpLimit
			crText = RGB(255,255,255);
			crTextSise = RGB(255,255,255);
			crBackSise = COLOR_UP;
			break;
		case UP:	// Up
			crText = COLOR_STEADY;
			crTextSise = COLOR_UP;
			break;
		case DOWN:	// Down
			crText = COLOR_STEADY;
			crTextSise = COLOR_DOWN;
			break;
		case DOWN_LIMIT:	// DownLimit
			crText = RGB(255, 255, 255);
			crTextSise = RGB(255,255,255);
			crBackSise = COLOR_DOWN;
			break;
		default://'3':	// Steady
			crText = COLOR_STEADY;
			crTextSise = COLOR_STEADY;
			break;
		}
		pDC->SetTextColor(crText);
		//	JSR-20051128 ADD End.
		///////////////////////////////////////////////////////////////////////////////		
		if(!crBackSise)
		{
			pDC->SetBkMode(TRANSPARENT);							/* JSR-20051128 ADD */
			rectInflate = rectTitleName;
			rectInflate.InflateRect(1,3);
			/*			pDC->DrawEdge(rectInflate,EDGE_ETCHED   ,BF_RECT);		JSR-20051128 Modify	*/
		}
		else
		{
			rectInflate = rectTitleName;
			rectInflate.InflateRect(1,3);							/* JSR-20051128 Modify */
			pDC->FillSolidRect(rectInflate,crBackSise);
		}
		pDC->TextOut(rectTitleName.left,rectTitleName.top,strTitleName);

		//		if(!crBackSise)
		//		{
		//////////// DC 초기값 다시 세팅 JSR-20051128 ADD Begin. //////////////////////
		COLORREF crText = NULL;
		COLORREF crTextSise = NULL;
		COLORREF crBackSise = NULL;
		pDC->SetBkMode(TRANSPARENT);

		// 복수종목 관리map을 이용 : 복수종목 - ojtaso (20070612)
		switch(pChartItemRealData->m_chSign)
		{
		case UP_LIMIT:	// UpLimit
		case UP:	// Up
			crText = COLOR_UP;
			crTextSise = COLOR_UP;
			break;
		case DOWN:	// Down
		case DOWN_LIMIT:	// DownLimit
			crText = COLOR_DOWN;
			crTextSise = COLOR_DOWN;
			break;
		//{{ 2007.06.26 by LYH 예상체결 적용
		case '9':	// 예상체결
			{
				int nChange = atoi(m_strChange);
				if( nChange > 0 )
				{
					crText = COLOR_UP;
					crTextSise = COLOR_UP;
				}
				else if(nChange < 0)
				{
					crText = COLOR_DOWN;
					crTextSise = COLOR_DOWN;
				}
				else
				{
					crText = COLOR_STEADY;
					crTextSise = COLOR_STEADY;
				}
			}
			break;
		//}}
		default://'3':	// Steady
			crText = COLOR_STEADY;
			crTextSise = COLOR_STEADY;
			break;
		}
		pDC->SetTextColor(crText);
		// JSR-20051128 ADD End.
		///////////////////////////////////////////////////////////////////////////////
		if(!rectTitlePrice.IsRectEmpty())
		{
			rectInflate = rectTitlePrice;
			rectInflate.InflateRect(1,3);
			pDC->TextOut(rectTitlePrice.left,rectTitlePrice.top,strTitlePrice);		
		}
		if(!rectTitleChange.IsRectEmpty())
		{
			rectInflate = rectTitleChange;
			rectInflate.InflateRect(1,3);
			pDC->TextOut(rectTitleChange.left,rectTitleChange.top,strTitleChange);
		}
		pDC->SetTextColor(COLOR_STEADY);
		if(!rectTitleVolume.IsRectEmpty())
		{
			rectInflate = rectTitleVolume;
			rectInflate.InflateRect(1,3);
			pDC->TextOut(rectTitleVolume.left,rectTitleVolume.top,strTitleVolume);
		}

		if(!rectTitleOpenHighLow.IsRectEmpty())
		{
			rectInflate = rectTitleOpenHighLow;
			rectInflate.InflateRect(1,3);
			pDC->TextOut(rectTitleOpenHighLow.left,rectTitleOpenHighLow.top,strTitleOpenHighLow);
		}
		if(m_lPtrST)
		{
			pDC->TextOut(rectTitleName.left,rectIn.top+HEIGHTOFCAPTION+4,m_strName);		
		}		
	}

	CRect rectInter11;
	CRect rectInter12;
	CRect rectInter2;
	rectInter11.IntersectRect(m_rectTitleJongmok,rectTitleType1);
	rectInter12.IntersectRect(m_rectTitleJongmok,rectTitleType2);
	rectInter2.IntersectRect(m_rectTitleJongmok,rectTitlePos);

	pFtDefault = (CFont *)pDC->SelectObject(pFtNormal);
	if(rectInter11.IsRectEmpty())
	{
		pDC->TextOut(rectTitleType1.left,rectTitleType1.top,strTitleType);
		if(rectTitleType1!=m_rectTitleType)
		{
			m_rectTitleTypePast = m_rectTitleType;
			m_rectTitleType = rectTitleType1;
		}
		if(rectInter2.IsRectEmpty())
		{
			pDC->TextOut(rectTitlePos.left,rectTitlePos.top,strTitlePos);			
			if(rectTitlePos!=m_rectTitlePos)
			{
				m_rectTitlePosPast = m_rectTitlePos;
				m_rectTitlePos = rectTitlePos;
			}

		}	
	}
	else if(rectInter12.IsRectEmpty())
	{
		pDC->TextOut(rectTitleType2.left,rectTitleType2.top,strTitleType);
		if(rectTitleType2!=m_rectTitleType)
		{
			m_rectTitleTypePast = m_rectTitleType;
			m_rectTitleType = rectTitleType2;
		}		
	}
	else
	{
		if(rectInter2.IsRectEmpty())
		{
			pDC->TextOut(rectTitlePos.left,rectTitlePos.top,strTitlePos);			
			if(rectTitlePos!=m_rectTitlePos)
			{
				m_rectTitlePosPast = m_rectTitlePos;
				m_rectTitlePos = rectTitlePos;
			}
		}	
	}
	pDC->SelectObject(pFtDefault);
	
	m_btMaxmize.Invalidate();
	m_btInit.Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	차트지표를 추가한다.
//////////////////////////////////////////////////////////////////////////////
BOOL CChartItem::AddChartBlock(LPCTSTR lpChartName)
{
	if(m_bInitalChart)
	{
		CString strPnF = "P&&F";
		if(strPnF==lpChartName)
		{
			lpChartName = "P&F";
		}
		return CDRChartOcx::AddChartBlock( lpChartName);
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	해당차트지표가 있는지 확인한다.
//////////////////////////////////////////////////////////////////////////////
BOOL CChartItem::HasGraph(LPCTSTR p_szGraphName)
{
	// Default RQ 검사 - ojtaso (20080416)
	if(m_strSelectedRQ != _T("DEFAULT"))
		return TRUE;
	
	CString strPnF = "P&&F";
	if(strPnF==p_szGraphName)
	{
		p_szGraphName = "P&F";
	}
	if(GetOnRunningOneChart()) // if Chart Item In View is P&F, etc..., 
	{
		return CDRChartOcx::HasGraphInSpecialBack(p_szGraphName);
	}
	return CDRChartOcx::HasGraph( p_szGraphName);
}

BOOL CChartItem::DestroyWindow()
{
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::DestroyWindow::Begin", _T("멀티차트 종료"), 13);
#endif

	CChartItem* pRecv = NULL;
	pRecv = ((CChartMng*)GetParent())->GetpChartCfgWnd(m_chType);
	if(pRecv->GetSafeHwnd() == GetSafeHwnd())
		((CChartMng*)GetParent())->SetpChartCfgWnd(m_chType, NULL);
	



	//예전 종목으로 Real을 해제 하므로  ======================================
	//현재의 코드를 예전 코드로 저장해 두고..	
	//2005. 09. 06
//	SetOldChartCode(GetDataCode()); 
//	((CChartMng*)GetParent())->OnChartMngCodeUnAdvise(this, IsAddMode());
	//========================================================================


	// Call InvokeAddIn Func. for ChartPatternAddin ------------------------------->
	// (차트패턴분석 dll에서 Modal Dialog가 생성되어 있는경우 먼저 종료시키기 위해)
	// wParam - 0:Create, 1:Destroy
	if(m_bAddInLoadPattern)
		InvokeAddInPattern("ChartPatternAddin.dll",NULL,0);
	// <----------------------------------------------------------------------------

#ifdef _WRITELOG
	WriteToStringLog("DestroyWindow::ClearData::Begin", _T("멀티차트 종료"), 13);
#endif
	ClearData();
#ifdef _WRITELOG
	WriteToStringLog("DestroyWindow::ClearData::End", _T("멀티차트 종료"), 13);
	WriteToStringLog("DestroyWindow::SetDrdsAdvise::Begin", _T("멀티차트 종료"), 13);
#endif

//	SetDrdsAdvise(FALSE);

#ifdef _WRITELOG
	WriteToStringLog("DestroyWindow::SetDrdsAdvise::End", _T("멀티차트 종료"), 13);
#endif
	DestoryItemData();	
	m_btMaxmize.DestroyWindow();
// (2008/9/13 - Seung-Won, Bae) Remove Init Button.
	m_btInit.DestroyWindow();

#ifdef _WRITELOG
	WriteToStringLog("CChartItem::DestroyWindow::End", _T("멀티차트 종료"), 13);
#endif
	//Han Hwa        m_signal.OffDrdsAdvise();
	return CDRChartOcx::DestroyWindow();
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	조회시 Tick/Min/Day 수치.정보를 Set한다.
//  
//////////////////////////////////////////////////////////////////////////////
void CChartItem::SetTickMinDayNumber(UINT nNumber)
{
	if(MIN_DATA_CHART == m_chTypeForRequest && 0 == nNumber)
		nNumber = 300;

	if(nNumber>=1000 || nNumber <= 0) 					
	{
		AfxMessageBox("1000이하의 값으로 다시 설정하여 주십시오");
		return;
	}
	
	ClearData();
	m_strTickMinDayForRequest.Format("%03d",nNumber);
	
	switch(m_chTypeForRequest)
	{
		case TICK_DATA_CHART:
			SetTickNumber(m_strTickMinDayForRequest);
			break;
		case SEC_DATA_CHART:
			SetSecNumber(m_strTickMinDayForRequest);
			break;
		case MIN_DATA_CHART:
			SetMinNumber(m_strTickMinDayForRequest);
			break;
		case DAY_DATA_CHART:
			SetDayNumber(m_strTickMinDayForRequest);
			break;		
		case HOUR_DATA_CHART:
			SetHourNumber(m_strTickMinDayForRequest);
	}	
}



///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	요구되어지는 데이터의 마지막 날짜를 Set한다.
//////////////////////////////////////////////////////////////////////////////
void CChartItem::SetEndDateRequested(LPCTSTR lpDate)
{	
	m_strDateForRequest = lpDate;
	if(m_strDateForRequest.GetLength()!=8)
	{
		CTime t = CTime::GetCurrentTime();
		m_strDateForRequest = t.Format("%Y%m%d");	
	}

	ClearData(); //05. 08. 29
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	실시간 정보를 반영한다.
//	lDataKind = MarketType
//////////////////////////////////////////////////////////////////////////////
void CChartItem::SetRealPriceData (void *pData, long lDataKind)
{
//<<20100308_JS.Kim 솔로몬
	CRealData_Common_FX	*pRealData_Common_FX = (CRealData_Common_FX *)pData;
	CRealData_Common	*pRealData_Common;

	CString		strTemp = "";
	BOOL		bRet = FALSE;
	CString		strSign, strTime;

	m_strRealItemCode = pRealData_Common_FX->m_strCode;

	// 복수종목 관리map을 이용 : 복수종목 - ojtaso (20070612)
	POSITION pos = m_mapChartItem.GetStartPosition ();
	ST_SELCHART* pSelChart = NULL;
	CString strKey;
	CString strPriceForChart;
	int nOrderType = 0;

	while(pos)
	{
		m_mapChartItem.GetNextAssoc (pos, strKey, (LPVOID&)pSelChart);

		// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
		if (pSelChart && !::lstrcmp (CStdDialog::GetRealItemCode (pSelChart->m_strItemCode), m_strRealItemCode))
		{
			if( pSelChart->m_strItemCode.Left(3) == "|S_" )			nOrderType = 0;		//// 매도
			else if( pSelChart->m_strItemCode.Left(3) == "|B_" )	nOrderType = 1;		//// 매수
			else													nOrderType = pSelChart->m_nOrderType;

			if( nOrderType == 0 )		// 매도
				pRealData_Common = &(pRealData_Common_FX->m_RealData_S);
			else						// 매수
				pRealData_Common = &(pRealData_Common_FX->m_RealData_B);

			ST_CHARTITEM_REALDATA* pChartItemRealData = NULL;
			BOOL bRealItem = m_mapChartItemRealData.Lookup (pSelChart->m_strItemCode, (LPVOID&)pChartItemRealData);


			//@실시간 데이터의 역전현상 체크.
			if( !pChartItemRealData )		continue;
			if( pChartItemRealData->m_strTime.GetLength()>0 &&
				pChartItemRealData->m_strTime.Compare(pRealData_Common->m_strTime)>0 &&
				atoi(pRealData_Common->m_strTime) > 5959 )
			{
				return;
			}
			pChartItemRealData->m_strTime = pRealData_Common->m_strTime;


			m_chSign = pRealData_Common->m_strSign.GetAt(0);

			if(bRealItem)
				pChartItemRealData->m_chSign = m_chSign;

			m_strPrice = pRealData_Common->m_strPrice;

			CString	strTemp;
			CString	strData;
			
			// 현재가
			strPriceForChart = m_strPrice;
			m_strPrice.TrimLeft();
/*
			strTemp.Format("%.0f", lPoint * atof (m_strPrice.operator LPCTSTR ()));
			strData.Format("%s", strTemp);

			if(bRealItem)
				pChartItemRealData->m_strPrice = strData;
*/
			if(bRealItem)
				pChartItemRealData->m_strPrice = m_strPrice;

			// 시가
			m_strOpen = pRealData_Common->m_strOpen;
			m_strOpen.TrimLeft();
			if(bRealItem)
				pChartItemRealData->m_strOpen = m_strOpen;

			// 고가
			m_strHigh = pRealData_Common->m_strHigh;
			m_strHigh.TrimLeft();
			if(bRealItem)
				pChartItemRealData->m_strHigh = m_strHigh;

			// 저가
			m_strLow = pRealData_Common->m_strLow;
			m_strLow.TrimLeft();
			if(bRealItem)
				pChartItemRealData->m_strLow = m_strLow;
				
			// 등락
			m_strChange = pRealData_Common->m_strChange;
			m_strChange.TrimLeft ();
			if (m_strChange == "")				
				m_strChange = "0";
			else if (m_strChange [0] == '-')	
			{
				if (m_strChange [0] == '.')
					m_strChange = '0' + m_strChange;
				else if (m_strChange [0] == '.')
					m_strChange = '0' + m_strChange;
			}

			if(bRealItem)
				pChartItemRealData->m_strChange = m_strChange;
			
			// 등락율
			m_strChrate = pRealData_Common->m_strChrate;
			if (m_strChrate [0] == '-')
			{
				if (m_strChrate [0] == '.')
					m_strChrate = '0' + m_strChrate;
			}
			else if (m_strChrate [0] == '.')
			{
				m_strChrate = '0' + m_strChrate;
			}

			if(bRealItem)
				pChartItemRealData->m_strChrate = m_strChrate;

			// 거래량
			m_strVolume = "0";
			if(bRealItem)
				pChartItemRealData->m_strVolume = m_strVolume;

			strTime = pRealData_Common->m_strTime;

			if (bRealItem)
			{
				IRealReceiver *pIRealReceiver = (IRealReceiver *)GetAddInCustomInterface ("ChartCommonAddIn.dll:DIRECT_REAL");
				if( !pIRealReceiver) return;

				pIRealReceiver->SetRealDataRQ (strKey, "_DateTime_", atof(strTime));
				
				if ((pSelChart->m_strChartGubun == QUERY_STRJONGMOK) && 
												(pSelChart->m_nChartNumericValueType == PERCENT_TYPE))
					pIRealReceiver->SetRealDataRQ(strKey, "_Close_", atof (pChartItemRealData->m_strChrate));

				else if ((pSelChart->m_strChartGubun == QUERY_STRJONGMOK) && 
													(pSelChart->m_nChartNumericValueType == DOLLAR_TYPE))
				{
					double dStdDollar = atof (((CStdDialog*)m_pStdDialog)->m_strDollar) / 100.;
					if (dStdDollar == 0.0)
						strPriceForChart = "0";
					else 
						strPriceForChart.Format ("%10.2f", (atof (strPriceForChart) / dStdDollar) * 100.);

					pIRealReceiver->SetRealDataRQ (strKey, "_Close_", atof (strPriceForChart));
				}
				else
					pIRealReceiver->SetRealDataRQ (strKey, "_Close_", atof (strPriceForChart));

				pIRealReceiver->SetRealDataRQ (strKey, "_Volume_",	atof (pChartItemRealData->m_strVolume));

				BOOL ret2 = pIRealReceiver->SetRealPacketEndRQ (strKey);
				pIRealReceiver->Release ();
			}
		}
	}
	//end

	//{{ 2006.10.09 by LYH 리얼 Repaint 문제 해결
	if (!::lstrcmp (m_strRealItemCode, CStdDialog::GetRealItemCode (m_strSelectedItemCode)))
	{
		CRect rectTemp;
		if (rectTemp.UnionRect(m_rectTitleJongmok,m_rectTitlePos))
			GetParent()->InvalidateRect(rectTemp);
	}
	//}}

/*
	CString		strTemp = "";
	BOOL		bRet = FALSE;
	CString		strSign, strTime;

	NEWREALDATA_ST		*pRealData = (NEWREALDATA_ST *)pData;
	CRealData_Common	pRealData_Common;
	char				tmpBuf [128];
	char				tmpBuf2 [128];
	int					nPoint = 0;
	long				lPoint = 0;

	memset (tmpBuf, 0x00, sizeof (tmpBuf));
	memcpy (tmpBuf, pRealData->PIP_LOWEST, sizeof (pRealData->PIP_LOWEST));
	nPoint = atoi (tmpBuf);
	lPoint = pow (10, nPoint);

	memset (tmpBuf, 0x00, sizeof (tmpBuf));
	memcpy (tmpBuf,	pRealData->SHCODE,			sizeof (pRealData->SHCODE));
	strTemp = tmpBuf;
	strTemp.TrimLeft ();
	strTemp.TrimRight ();

	pRealData_Common.m_strCode = strTemp;

	// Real-Data는 HHMMSS만..
	memset (tmpBuf, 0x00, sizeof (tmpBuf));
	memcpy (tmpBuf,	&pRealData->REAL_TIME[8], 6);
	pRealData_Common.m_strTime = tmpBuf;

	if (m_strOrderType == "0")		// 매도
	{
		memset (tmpBuf, 0x00, sizeof (tmpBuf));
		memcpy (tmpBuf,	pRealData->OFFER_SIGN,	sizeof (pRealData->OFFER_SIGN));
		pRealData_Common.m_strSign = tmpBuf;

		memset (tmpBuf2, 0x00, sizeof (tmpBuf2));
		memcpy (tmpBuf2, pRealData->OFFER, sizeof (pRealData->OFFER));
		pRealData_Common.m_strPrice = tmpBuf2;

		memset (tmpBuf, 0x00, sizeof (tmpBuf));
		memcpy (tmpBuf,	pRealData->OFFER_CHANGE_PRICE, sizeof (pRealData->OFFER_CHANGE_PRICE));
		pRealData_Common.m_strChange = tmpBuf;

		double  dChangeRate = atof (tmpBuf) / atof (tmpBuf2);
		pRealData_Common.m_strChrate.Format ("%f", dChangeRate);

		memset (tmpBuf, 0x00, sizeof (tmpBuf));
		memcpy (tmpBuf,	pRealData->OFFEROPEN_PRICE,	sizeof (pRealData->OFFEROPEN_PRICE));
		pRealData_Common.m_strOpen = tmpBuf;

		memset (tmpBuf, 0x00, sizeof (tmpBuf));
		memcpy (tmpBuf,	pRealData->OFFERHIGH_PRICE,	sizeof (pRealData->OFFERHIGH_PRICE));
		pRealData_Common.m_strHigh = tmpBuf;

		memset (tmpBuf, 0x00, sizeof (tmpBuf));
		memcpy (tmpBuf,	pRealData->OFFERLOW_PRICE,	sizeof (pRealData->OFFERLOW_PRICE));
		pRealData_Common.m_strLow = tmpBuf;

	}
	else							// 매수
	{
		memset (tmpBuf, 0x00, sizeof (tmpBuf));
		memcpy (tmpBuf,	pRealData->BID_SIGN, sizeof (pRealData->BID_SIGN));
		pRealData_Common.m_strSign = tmpBuf;

		
		memset (tmpBuf2, 0x00, sizeof (tmpBuf2));
		memcpy (tmpBuf2,	pRealData->BID,	sizeof (pRealData->BID));
		pRealData_Common.m_strPrice = tmpBuf2;

		memset (tmpBuf, 0x00, sizeof (tmpBuf));
		memcpy (tmpBuf,	pRealData->BID_CHANGE_PRICE, sizeof (pRealData->BID_CHANGE_PRICE));
		pRealData_Common.m_strChange = tmpBuf;

		double  dChangeRate = atof (tmpBuf) / atof (tmpBuf2);
		pRealData_Common.m_strChrate.Format ("%f", dChangeRate);

		memset (tmpBuf, 0x00, sizeof (tmpBuf));
		memcpy (tmpBuf,	pRealData->BIDOPEN_PRICE, sizeof (pRealData->BIDOPEN_PRICE));
		pRealData_Common.m_strOpen = tmpBuf;

		memset (tmpBuf, 0x00, sizeof (tmpBuf));
		memcpy (tmpBuf,	pRealData->BIDHIGH_PRICE, sizeof (pRealData->BIDHIGH_PRICE));
		pRealData_Common.m_strHigh = tmpBuf;

		memset (tmpBuf, 0x00, sizeof (tmpBuf));
		memcpy (tmpBuf,	pRealData->BIDLOW_PRICE, sizeof (pRealData->BIDLOW_PRICE));
		pRealData_Common.m_strLow = tmpBuf;



	}

	// 거래량이 없어서 "0"으로 조정..
	pRealData_Common.m_strVolume = "0";

	memset (tmpBuf, 0x00, sizeof (tmpBuf));
	memcpy (tmpBuf,	pRealData->BID, sizeof (pRealData->BID));
	pRealData_Common.m_strBid = tmpBuf;

	memset (tmpBuf, 0x00, sizeof (tmpBuf));
	memcpy (tmpBuf,	pRealData->OFFER, sizeof (pRealData->OFFER));
	pRealData_Common.m_strOffer = tmpBuf;

	m_strRealItemCode = pRealData_Common.m_strCode;

	// 복수종목 관리map을 이용 : 복수종목 - ojtaso (20070612)
	POSITION pos = m_mapChartItem.GetStartPosition ();
	ST_SELCHART* pSelChart = NULL;
	CString strKey;
	CString strPriceForChart;

	while(pos)
	{
		m_mapChartItem.GetNextAssoc (pos, strKey, (LPVOID&)pSelChart);

		// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
		if (!::lstrcmp (CStdDialog::GetRealItemCode (pSelChart->m_strItemCode), m_strRealItemCode))
		{
			ST_CHARTITEM_REALDATA* pChartItemRealData = NULL;
			BOOL bRealItem = m_mapChartItemRealData.Lookup (pSelChart->m_strItemCode, (LPVOID&)pChartItemRealData);

			m_chSign = pRealData_Common.m_strSign.GetAt(0);

			if(bRealItem)
				pChartItemRealData->m_chSign = m_chSign;

			if( pSelChart->m_strItemCode.GetAt( 0) == '|')
			{
				if( pSelChart->m_strItemCode.GetAt( 1) == 'S')
						m_strPrice = pRealData_Common.m_strOffer;
				else	m_strPrice = pRealData_Common.m_strBid;
			}
			else
			{
				if( m_strOrderType == "0")
						m_strPrice = pRealData_Common.m_strOffer;
				else	m_strPrice = pRealData_Common.m_strBid;
			}

			CString	strTemp;
			CString	strData;
			
			// 현재가
			strPriceForChart = m_strPrice;
			m_strPrice.TrimLeft();

			strTemp.Format("%.0f", lPoint * atof (m_strPrice.operator LPCTSTR ()));
			strData.Format("%s", strTemp);

			if(bRealItem)
				pChartItemRealData->m_strPrice = strData;

			// 시가
			m_strOpen = pRealData_Common.m_strOpen;
			m_strOpen.TrimLeft();

			strTemp.Format("%.0f", lPoint * atof (m_strOpen.operator LPCTSTR ()));
			strData.Format("%s", strTemp);

			if(bRealItem)
				pChartItemRealData->m_strOpen = strData;

			// 고가
			m_strHigh = pRealData_Common.m_strHigh;
			m_strHigh.TrimLeft();

			strTemp.Format("%.0f", lPoint * atof (m_strHigh.operator LPCTSTR ()));
			strData.Format("%s", strTemp);

			if(bRealItem)
				pChartItemRealData->m_strHigh = strData;

			// 저가
			m_strLow = pRealData_Common.m_strLow;
			m_strLow.TrimLeft();

			strTemp.Format("%.0f", lPoint * atof (m_strLow.operator LPCTSTR ()));
			strData.Format("%s", strTemp);

			if(bRealItem)
				pChartItemRealData->m_strLow = strData;
				
			// 등락
			m_strChange = pRealData_Common.m_strChange;
			m_strChange.TrimLeft ();

			if (m_strChange == "")				
				m_strChange = "0";
			else if (m_strChange [0] == '-')	
			{
				if (m_strChange [0] == '.')
					m_strChange = '0' + m_strChange;
				else if (m_strChange [0] == '.')
					m_strChange = '0' + m_strChange;
			}

			strTemp.Format("%.0f", lPoint * atof (m_strChange.operator LPCTSTR ()));
			strData.Format("%s", strTemp);

			if(bRealItem)
				pChartItemRealData->m_strChange = strData;
			
			// 등락율
			m_strChrate = pRealData_Common.m_strChrate;
			if (m_strChrate [0] == '-')
			{
				if (m_strChrate [0] == '.')
					m_strChrate = '0' + m_strChrate;
			}
			else if (m_strChrate [0] == '.')
			{
				m_strChrate = '0' + m_strChrate;
			}

			strTemp.Format("%.0f", lPoint * atof (m_strChrate.operator LPCTSTR ()));
			strData.Format("%s", strTemp);

			if(bRealItem)
				pChartItemRealData->m_strChrate = strData;

			// 거래량
			m_strVolume = pRealData_Common.m_strVolume;
			m_strVolume.TrimLeft();
			if(bRealItem)
				pChartItemRealData->m_strVolume = m_strVolume;

			strTime = pRealData_Common.m_strTime;

			strTemp.Format("%.0f", lPoint * atof (strPriceForChart.operator LPCTSTR ()));
			strData.Format("%s", strTemp);

			strPriceForChart = strData;

			if (bRealItem)
			{
				IRealReceiver *pIRealReceiver = (IRealReceiver *)GetAddInCustomInterface ("ChartCommonAddIn.dll:DIRECT_REAL");
				if( !pIRealReceiver) return;

				pIRealReceiver->SetRealDataRQ (strKey, "_DateTime_", atof(strTime));
				
				if ((pSelChart->m_strChartGubun == QUERY_STRJONGMOK) && 
												(pSelChart->m_nChartNumericValueType == PERCENT_TYPE))
					pIRealReceiver->SetRealDataRQ(strKey, "_Close_", atof (pChartItemRealData->m_strChrate));

				else if ((pSelChart->m_strChartGubun == QUERY_STRJONGMOK) && 
													(pSelChart->m_nChartNumericValueType == DOLLAR_TYPE))
				{
					double dStdDollar = atof (((CStdDialog*)m_pStdDialog)->m_strDollar) / 100.;
					if (dStdDollar == 0.0)
						strPriceForChart = "0";
					else 
						strPriceForChart.Format ("%10.2f", (atof (strPriceForChart) / dStdDollar) * 100.);

					pIRealReceiver->SetRealDataRQ (strKey, "_Close_", atof (strPriceForChart));
				}
				else
					pIRealReceiver->SetRealDataRQ (strKey, "_Close_", atof (strPriceForChart));

				pIRealReceiver->SetRealDataRQ (strKey, "_Volume_",	atof (pChartItemRealData->m_strVolume));

				BOOL ret2 = pIRealReceiver->SetRealPacketEndRQ (strKey);
				pIRealReceiver->Release ();
			}
		}
	}
	//end

	//{{ 2006.10.09 by LYH 리얼 Repaint 문제 해결
	CString szSelectedItemCode = m_strSelectedItemCode.Right (7);

	if (!::lstrcmp (m_strRealItemCode, szSelectedItemCode))
	{
		CRect rectTemp;
		if (rectTemp.UnionRect(m_rectTitleJongmok,m_rectTitlePos))
			GetParent()->InvalidateRect(rectTemp);
	}
	//}}
*/
//>>
}


///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	실시간 정보 처리여부를 처리한다.
//////////////////////////////////////////////////////////////////////////////
BOOL CChartItem::SetDrdsAdvise(long lOnAdvise)
{
	long bAdviceForChart = FALSE;
	switch(lOnAdvise)
	{
	case 1:
		m_bOnAdvise = TRUE;
		bAdviceForChart = TRUE;
		break;
	case 2:
		m_bOnAdvise = TRUE;
		bAdviceForChart = FALSE;
		break;
	case 0:
		m_bOnAdvise = FALSE;
		bAdviceForChart = FALSE;
		break;
	}

	//=================================================================
	// 복기 모드 땜시 추가한다..
	// 2005.10.14
	//=================================================================
	if(m_bReplaychartMode && m_bOnAdvise)
	{		
		m_bOnAdvise = FALSE;
		bAdviceForChart = FALSE;
		m_strReplayTime = _T("복기모드 상태입니다                         ");		
	}
	//=================================================================
	
	if(!::IsWindow(this->GetSafeHwnd()))
	{
		return FALSE;
	}
	if(GetParent()->GetSafeHwnd())
	{
		// 복수종목 실시간 처리 : 복수종목 - ojtaso (20070109)
		BOOL bAddMode = IsAddMode();
		if(m_bOnAdvise)
		{
			//Han Hwa        m_signal.OnDrdsAdvise(GetDataCode());
			((CChartMng*)GetParent())->OnChartMngCodeAdvise(this, bAddMode);
		}
		// 복수종목 실시간 처리 : 복수종목 - ojtaso (20070109)
		else if(!m_bReplaychartMode && !m_bOnAdvise && bAddMode)
		{
			return TRUE;
		}
		else
		{
			((CChartMng*)GetParent())->OnChartMngCodeUnAdvise(this, bAddMode);
		}	
	}
	long lEndDate = atol(GetEndDate());
	CTime t = CTime::GetCurrentTime();
	long lToday = atol(t.Format("%Y%m%d"));
	if(lEndDate<lToday)
	{
		bAdviceForChart = FALSE;
	}	
	return CDRChartOcx::SetDrdsAdvise(bAdviceForChart);
}

// 복수종목 추가 모드인지 : 복수종목 - ojtaso (20070119)
BOOL CChartItem::IsAddMode()
{
	// 차트 추가모드이면 이전 종목을 해제 하지 않는다
	// 단 현재 종목의 타입을 바꿀때는 종목을 해제한다
	if(((CChartMng*)GetParent())->m_bChangeTypeMode)
		return FALSE;
	else if(((CChartMng*)GetParent())->GetAddGraphMode())
		return TRUE;
	else
		return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	조회를 위한 코드를 세팅한다.
//////////////////////////////////////////////////////////////////////////////
//void CChartItem::SetCodeForQuery(LPCTSTR lpCode, UINT nQueryType, BOOL bIsPrimaryData) 
void CChartItem::SetCodeForQuery(LPCTSTR lpCode, CString strQueryType, BOOL bIsPrimaryData) 
{	
#ifdef _DEBUG	
	if(!strQueryType.GetLength())
	{
		AfxMessageBox("CChartItem::SetCodeForQuery TR 번호 없다");
		return;
	}
#endif
// 복수종목 적용 : 복수종목 - ojtaso (20070503)
//	CString strOldCode = GetDataCode();
	
	if(strQueryType != QUERY_STRMARKET)
		SetOldChartCode(m_strSelectedItemCode);

	if(IsAvailibleCode(lpCode))
		GetChartItemDataNew(strQueryType,lpCode,TRUE,bIsPrimaryData);
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	해당 차트 데이터가 존재하는지 체크한다.
//////////////////////////////////////////////////////////////////////////////
BOOL CChartItem::HasChartItemData(LPCTSTR lpCode, CString strQueryType, BOOL bFindWithPrimaryData, BOOL bAutoRemoveUselessData/*=TRUE*/) 
{
	CChartItemData *pChartItemData = GetChartItemDataNew(strQueryType,lpCode,FALSE);	
	if(pChartItemData)
	{
		BOOL bRetValue = HasGraph(pChartItemData->GetChartItemDataTitle());	
		if(bRetValue)
		{
			return TRUE;
		}
		if(bAutoRemoveUselessData)
		{
			RemoveChartItemData(lpCode,strQueryType);
		}
	}

#ifdef _DEBUG
		CString sTemp;
		sTemp.Format("HasChartItemData::m_arrayChartItemData 갯수 %d", m_arrayChartItemData.GetSize());
		OutputDebugString(sTemp);
#endif

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	조회를 위한 TR-Code를 받는다 
//////////////////////////////////////////////////////////////////////////////
CString CChartItem::GetTrCodeForQuery()
{
	CChartItemData *pChartItemData = FindChartItemDataForQuery();
	if(pChartItemData)
	{
		return pChartItemData->GetTrCode();
	}
	return "";
}



char CChartItem::GetType() 
{ 
	if (!m_arrayChartItemData.GetSize ())
	{
		return m_chTypeForRequest;
	}

	// 틱(0)/분(1)/일(2)/주(3)/월(4)
	return m_chType; 
}
 

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	조회를 위한 Input Spec을 구성한다.
//////////////////////////////////////////////////////////////////////////////
// 복수종목 추가 : 복수종목 - ojtaso (20070118)
stSend2Server* CChartItem::GetQueryData(BOOL bChangeType/* = FALSE*/)
{
	if(	m_strDateForRequest.IsEmpty()	// 검색시작일
		||m_lCntForRequest==0			// 검색할 데이터 갯수
		||m_chTypeForRequest==NULL		// 틱(0)/분(1)/일(2)/주(3)/월(4)
		) 
	{
		return NULL;
	}
	CChartItemData *pChartItemData = FindChartItemDataForQuery();
	if(pChartItemData)
	{
		// 처음 차트엔 종목코드를 세팅, 추가되는 차트는 RQ별로 그래프 삽입 : 복수종목 - ojtaso (20061224)
		CChartMng* pMng = (CChartMng*)GetParent();
		LPCTSTR lpszItemCode = pChartItemData->GetCodeForRqstFromChartItemData();

		// 처음 Block에 차트추가하는 경우(CHANGE_ITEM)
		// 새로 Block을 추가하는 차트추가인 경우(NEW_ITEM)
		if((!m_bBuild || pMng->GetAddBlockMode()) && !bChangeType)
		{
			ST_CHARTITEM_REALDATA* pChartItemRealData;
			if(m_mapChartItemRealData.Lookup(lpszItemCode, (LPVOID&)pChartItemRealData))
				pChartItemRealData->m_nRefCount++;
			else
			{
				pChartItemRealData = new ST_CHARTITEM_REALDATA;

				pChartItemRealData->m_nRefCount = 1;
				m_mapChartItemRealData.SetAt(lpszItemCode, (LPVOID)pChartItemRealData);
			}

			AddChartItem(pChartItemData, lpszItemCode, pMng->GetJongMokNameByCode(lpszItemCode), m_chTypeForRequest, GetTickMinDayNumber(), GetCntForRequest(), GetChart_NumericValueType(), m_bBuild);
		}
		else if(m_bBuild)	// 단순 차트 변경(CHANGE_ITEM)
		{
			if(pMng->IsAbleToAdd() || !pMng->GetExistSyncChart())
			{
				CString szSelectedItemCode = m_strSelectedItemCode.Right(7);
				if(::lstrcmp(szSelectedItemCode, lpszItemCode))
				{
					ST_CHARTITEM_REALDATA* pChartItemRealData;

					// (2008/9/14 - Seung-Won, Bae) for Pair Tick
					int nRequestPairTickState = ( ( CStdDialog *)m_pStdDialog)->GetRequestPairTickState();
					if( nRequestPairTickState != E_RPTS_PAIR
						&& nRequestPairTickState != E_RPTS_LOOP_PAIR
						&& nRequestPairTickState != E_PRTS_ROTATE_PAIR)
						if( m_mapChartItemRealData.Lookup(szSelectedItemCode, (LPVOID&)pChartItemRealData))
					{
						if(pChartItemRealData->m_nRefCount > 1)
							pChartItemRealData->m_nRefCount--;
						else
						{
							delete pChartItemRealData;
							m_mapChartItemRealData.RemoveKey(szSelectedItemCode);
						}
					}

					if(m_mapChartItemRealData.Lookup(lpszItemCode, (LPVOID&)pChartItemRealData))
						pChartItemRealData->m_nRefCount++;
					else
					{
						pChartItemRealData = new ST_CHARTITEM_REALDATA;

						pChartItemRealData->m_nRefCount = 1;
						m_mapChartItemRealData.SetAt(lpszItemCode, (LPVOID)pChartItemRealData);
					}
				}
			}

			if(m_strSelectedItemCode.IsEmpty())
				m_strSelectedItemCode = lpszItemCode;
			else	// 분석툴 종목별 저장 - ojtaso (20080728)
			{
				//[A00000451]alzioyes:FX차트 매수/매도 변경시 셋팅안되는 부분 있어서 추가함.
				ST_CHARTITEM_REALDATA* pChartItemRealData;
				if(m_mapChartItemRealData.Lookup(lpszItemCode, (LPVOID&)pChartItemRealData)==FALSE)
				{
					pChartItemRealData = new ST_CHARTITEM_REALDATA;
					pChartItemRealData->m_nRefCount = 1;
					m_mapChartItemRealData.SetAt(lpszItemCode, (LPVOID)pChartItemRealData);
				}

				//우리선물QA125-080920 alzioyes.
				//종목주기도 있으므로 무조건 저장한다.

				// 2011.01.31 by SYS >> 분차트 YYYY(년) 추가 및 분석툴 공유 기능 추가
				//SaveandOpenFromAnalToolFile("", GetOpenAndSaveFileName(TRUE), FALSE, TRUE);

				CString strAnalFile = GetOpenAndSaveAnalFileName(TRUE);
				SaveandOpenFromAnalToolFile("", strAnalFile, FALSE, TRUE);
				
// 				BOOL bSaveAnal = TRUE;
// 				BOOL bShareAnalTool = ((CChartMng*)GetParent())->GetShareAnalTool();
// 				if (bShareAnalTool)
// 				{
// 					// 분석툴공유 모드인 경우는 
// 					// 주기변경에 대해서 분석툴을 파일에 저장하지 않는다.
// 					if (bChangeType)
// 						bSaveAnal = FALSE;
// 				}
// 				
// 				if (bSaveAnal)
// 				{
// 					CString strAnalFile = GetOpenAndSaveAnalFileName(TRUE);
// 					SaveandOpenFromAnalToolFile("", strAnalFile, FALSE, TRUE);
// 				}
				// 2011.01.31 by SYS <<
			}

			AddChartItem(pChartItemData, lpszItemCode, pMng->GetJongMokNameByCode(lpszItemCode, pChartItemData->GetTrCode()), m_chTypeForRequest, GetTickMinDayNumber(), GetCntForRequest(), GetChart_NumericValueType(), FALSE);
		}

		pChartItemData->m_strOrderType = m_strOrderType;
		return pChartItemData->GetQueryData(m_bNext, m_lpSelChart);
	}
	return NULL;
}

// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
stSend2Server* CChartItem::GetLoadedQueryData()
{
	if(	m_strDateForRequest.IsEmpty()	// 검색시작일
		||m_lCntForRequest==0			// 검색할 데이터 갯수
		||m_chTypeForRequest==NULL		// 틱(0)/분(1)/일(2)/주(3)/월(4)
		) 
	{
		return NULL;
	}

	CChartItemData *pChartItemData = FindChartItemDataForQuery();
	if(pChartItemData)
	{
		if(m_bLoadDefaultRQ && !m_posLoadingRQ)
		{
			m_bLoadDefaultRQ = FALSE;
			m_strKey = _T("DEFAULT");

			m_mapChartItem.Lookup(m_strKey, (LPVOID&)m_lpSelChart);
		}
		else
		{
			if(!m_posLoadingRQ)
				m_posLoadingRQ = m_mapChartItem.GetStartPosition();

			m_mapChartItem.GetNextAssoc(m_posLoadingRQ, m_strKey, (LPVOID&)m_lpSelChart);

			if(m_posLoadingRQ && !m_strKey.Compare(_T("DEFAULT")))
			{
				m_bLoadDefaultRQ = TRUE;
				m_mapChartItem.GetNextAssoc(m_posLoadingRQ, m_strKey, (LPVOID&)m_lpSelChart);
			}
		}

		ST_CHARTITEM_REALDATA* pChartItemRealData;
		if(m_mapChartItemRealData.Lookup(m_lpSelChart->m_strItemCode, (LPVOID&)pChartItemRealData))
			pChartItemRealData->m_nRefCount++;
		else
		{
			pChartItemRealData = new ST_CHARTITEM_REALDATA;

			pChartItemRealData->m_nRefCount = 1;
			m_mapChartItemRealData.SetAt(m_lpSelChart->m_strItemCode, (LPVOID)pChartItemRealData);

			//{{2007.05.30 by LYH 주식, 선물, 옵션 복수일 때 TR번호,종목번호 어긋나는 버그 수정
			pChartItemData->SetTRCode(m_lpSelChart->m_strChartGubun);
			pChartItemData->SetCodeForQuery(m_lpSelChart->m_strItemCode);
			//}}
		}

		if(!m_posLoadingRQ && !m_strKey.Compare(_T("DEFAULT")))
		{
			m_bLoadRQ = FALSE;

			CDRChartOcx::SetCurrentRQ(m_strKey);

			// (2008/9/24 - Seung-Won, Bae) Set Sell/Buy Postfix in Tick
			CString strItemName = CStdDialog::GetRealItemCode( m_lpSelChart->m_strItemName);
			if( m_lpSelChart->m_cType == '0' && atoi( m_lpSelChart->m_strTickMinDayNumber) == 1)
				strItemName = strItemName + "(" + m_lpSelChart->m_strItemCode.GetAt( 1) + ")";
			// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
			CDRChartOcx::AddChartItem(m_lpSelChart->m_strItemCode, strItemName, FALSE);
			return pChartItemData->GetQueryData(m_bNext, m_lpSelChart);
		}

		// (2008/9/24 - Seung-Won, Bae) Set Sell/Buy Postfix in Tick
		CString strItemName = CStdDialog::GetRealItemCode( m_lpSelChart->m_strItemName);
		if( m_lpSelChart->m_cType == '0' && atoi( m_lpSelChart->m_strTickMinDayNumber) == 1)
			strItemName = strItemName + "(" + m_lpSelChart->m_strItemCode.GetAt( 1) + ")";
		// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
		if(CDRChartOcx::AddChartItemRQ( m_strKey, m_lpSelChart->m_strItemCode, strItemName))
			return pChartItemData->GetQueryData(m_bNext, m_lpSelChart);
	}  

	return NULL;
}


// 복수종목 선택 : 복수종목 - ojtaso (20080215)
void CChartItem::OnSelectChartItem(ST_SELCHART* pSelChart, BOOL bInitKey/* = TRUE*/)
{
	CChartItemData *pChartItemData = GetPrimaryChartItemData();
	if(pChartItemData)
		pChartItemData->OnSelectChartItem(pSelChart);

	m_strSelectedItemCode = pSelChart->m_strItemCode;
	if(m_strOldCode.IsEmpty())
		m_strOldCode = m_strSelectedItemCode;

	m_strKorName = pSelChart->m_strItemName;

	m_chTypeForRequest = m_chType = pSelChart->m_cType;
	m_strTickMinDayForRequest = pSelChart->m_strTickMinDayNumber;

	switch(m_chTypeForRequest)
	{
	case TICK_DATA_CHART:
		SetTickNumber(m_strTickMinDayForRequest);
		break;
	case SEC_DATA_CHART:
		SetSecNumber(m_strTickMinDayForRequest);
		break;
	case MIN_DATA_CHART:
		SetMinNumber(m_strTickMinDayForRequest);
		break;
	case DAY_DATA_CHART:
		SetDayNumber(m_strTickMinDayForRequest);
		break;		
	case HOUR_DATA_CHART:
		SetHourNumber(m_strTickMinDayForRequest);
	}	

	//	SetCntForRequest(pSelChart->m_lCount);
	SetChartNumericValue(pSelChart->m_nChartNumericValueType);

	m_strOrderType.Format("%d", pSelChart->m_nOrderType);	//20100316_JS.Kim 솔로몬.FX

	CRect rectTemp;
	if(rectTemp.UnionRect(m_rectTitleJongmok,m_rectTitlePos))
		GetParent()->InvalidateRect(rectTemp);

	m_lpSelChart = pSelChart;
}

// 복수종목 삭제 : 복수종목 - ojtaso (20080215)
void CChartItem::OnDeleteChartItem(LPCTSTR lpszRQ, ST_SELCHART*& pSelChart)
{
	ST_CHARTITEM_REALDATA* pChartItemRealData;
	if(m_mapChartItemRealData.Lookup(pSelChart->m_strItemCode, (LPVOID&)pChartItemRealData))
	{
		if(pChartItemRealData->m_nRefCount > 1)
			pChartItemRealData->m_nRefCount--;
		else
		{
			delete pChartItemRealData;
			m_mapChartItemRealData.RemoveKey(pSelChart->m_strItemCode);
		}
	}

	if(!m_strOldCode.CompareNoCase(pSelChart->m_strItemCode))
		m_strOldCode = m_strSelectedItemCode;

	m_mapChartItem.RemoveKey(lpszRQ);
	delete pSelChart;

	m_lpSelChart = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	조회를 위한 타입을 설정한다.
//			 chType: 틱(0)/분(1)/일(2)/주(3)/월(4)
//////////////////////////////////////////////////////////////////////////////
void CChartItem::SetType(char chType) 
{ 
	ClearData(); 
	SetChartRequestType(chType);
}

// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
long CChartItem::ReceiveLoadingChartData (void* szData, long nLength)
{
	CChartItemData *pChartItemData = FindChartItemDataForQuery ();
	if (!pChartItemData)
		return 0L;

	BOOL bRetValue = pChartItemData->MakeChartData (szData, nLength, m_lpSelChart);

	pChartItemData->AddLoadedPacket (m_strKey, m_lpSelChart);
	pChartItemData->SetLoadedPacketData (m_strKey, m_lpSelChart);

	ST_CHARTITEM_REALDATA* pChartItemRealData;

	if(m_mapChartItemRealData.Lookup (m_lpSelChart->m_strItemCode, (LPVOID&)pChartItemRealData))
	{
		if (m_lpSelChart->m_strChartGubun == QUERY_STRFX)
		{
			SM_fx112_OutRec1* pfx112OutRec1 = (SM_fx112_OutRec1*)szData;

			//	시고저값 저장
			pChartItemData->GetOHLValue(pChartItemRealData->m_strOpen, pChartItemRealData->m_strHigh, pChartItemRealData->m_strLow);
			//	종목명
			pChartItemRealData->m_strItemName = pChartItemData->GetReciveValue(0);
			//	부호
			pChartItemRealData->m_chSign = ' ';
			//	현재가
			pChartItemRealData->m_strPrice = pChartItemData->GetReciveValue(1);
			//pChartItemRealData->m_strPrice.Replace(".", "");
			//	시가
			//pChartItemRealData->m_strOpen.Replace(".", "");
			//	고가
			//pChartItemRealData->m_strHigh.Replace(".", "");
			//	저가
			//pChartItemRealData->m_strLow.Replace(".", "");
			//	소수점 자리 단위
			pChartItemRealData->m_strPipLowest = pChartItemData->GetPipLowest();
		}
	}

	//>>20100227_JS.Kim 복수종목 실시간 등록
	CString strDateToday;
	CTime t = CTime::GetCurrentTime();
	strDateToday = t.Format("%Y%m%d");

	BOOL bIsPrimaryChartItemData = IsPrimaryChartItemData(pChartItemData);
	if(strDateToday==m_strEndDate)
	{
		if(m_lPtrST)
		{
			pChartItemData->RemoveRealItem();
			SetDrdsAdvise(2);
		}
		else
		{			
			pChartItemData->AddRealItem();
			
			if(bIsPrimaryChartItemData)
			{
				SetDrdsAdvise(1);
			}
		}
	}
	else
	{
		if(bIsPrimaryChartItemData)
		{
			SetDrdsAdvise(2);
		}
	}
	//<<

	CChartMng* lpChartMng = (CChartMng*)GetParent();
	lpChartMng->RequestChartData();

	return 0L;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	차트데이터조회결과값을 받는다.
//////////////////////////////////////////////////////////////////////////////
long CChartItem::ReceiveInitalChartData(void* szData, long nLength)
{	
	CString strJMCode;
	CChartItemData *pChartItemData = FindChartItemDataForQuery();
	if(!pChartItemData)
	{
		return 0;
	}

	// FX차트 주문 연동 - onlyojt (20091204)
	if (((CChartMng*)GetParent())->GetOrderLink())
	{
		InvokeAddInStr("ChartOutsideCustomAddIn.dll:ORDER_LINE", "1", "ON");
		if( m_chTypeForRequest == MIN_DATA_CHART && atoi(GetMinNumber()) <= 60 )
			InvokeAddInStr("ChartOutsideCustomAddIn.dll:ORDER_LINE", "2", "ON");
		else
			InvokeAddInStr("ChartOutsideCustomAddIn.dll:ORDER_LINE", "2", "OFF");
		InvokeAddInStr("ChartOutsideCustomAddIn.dll:ORDER_LINE", "3", (m_strOrderType=="0" ? "SELL":"BUY"));
//		InvokeAddInStr("ChartKoreaSorimachiAddIn.dll:USER_COMMENT", "1", "ON");
	}
	else
	{
		InvokeAddInStr("ChartOutsideCustomAddIn.dll:ORDER_LINE", "1", "OFF");
		InvokeAddInStr("ChartOutsideCustomAddIn.dll:ORDER_LINE", "2", "OFF");
//		InvokeAddInStr("ChartKoreaSorimachiAddIn.dll:USER_COMMENT", "1", "OFF");
	}

	// 2011.02.10 by onlyojt >> 분차트 YYYY(년) 추가 및 분석툴 공유 기능 추가
	// 추세선 공유상태에서 주기 변경때의 오류 현상 수정 - onlyojt
	// 2011.02.11 by SYS : 추세선공유 모드인 경우만 처리하도록 수정함
	BOOL bShareAnalTool = ((CChartMng*)GetParent())->GetShareAnalTool();
	if (bShareAnalTool)
	{
		SendMessage(RMS_ANALYSIS_FUNC, CToolOptionInfo::TOOLOPTION::T_DELETE_ALL);
	}
	// 2011.02.10 by onlyojt <<

	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	CChartMng* lpChartMng = (CChartMng*)GetParent();

	//2005. 09. 13 pm 06:26 선영대리 요청으로 다 뺀다 ChangePriceGraphDrawingData(TRUE,TRUE,TRUE,TRUE,FALSE,1);	
	BOOL bRetValue = pChartItemData->MakeChartData(szData,nLength, m_lpSelChart);
	if(!bRetValue)
	{
		return 0;
	}
	m_strOpen = "";
	m_strHigh = "";
	m_strLow = "";

	CString strTrCode = _T(""); //호가 요청 때문에 밖으로 뺐다. 2005. 07. 15
	// 복수종목 관리 : 복수종목 - ojtaso (20070328)
	CString strCode = m_lpSelChart ? m_lpSelChart->m_strItemCode : pChartItemData->GetCode();

	if(IsPrimaryChartItemData(pChartItemData)&&!m_bNext)	// Update Title Info..
	{	
		m_bReceivedDataChart = TRUE;
	
		strTrCode = pChartItemData->GetTrCode();
		if(strTrCode == QUERY_STRFX)
		{			
			char szTemp[64];
			SM_fx112_OutRec1* pfx112OutRec1 = (SM_fx112_OutRec1*)szData;

			m_chType = m_chTypeForRequest;			
			SetEndDate(m_strDateForRequest); //05.09.06 m_strEndDate = m_strDateForRequest;
			m_strTickMinDay = m_strTickMinDayForRequest;
			switch(m_chType)
			{
				case TICK_DATA_CHART:
					SetTickNumber(m_strTickMinDay);
					break;
				case MIN_DATA_CHART:
					SetMinNumber(m_strTickMinDay);
					break;
				case DAY_DATA_CHART:
					SetDayNumber(m_strTickMinDay);
					break;
				case HOUR_DATA_CHART:
					SetHourNumber(m_strTickMinDay);
				default:
					m_strTickMinDay = "001";
					break;
			}

			//=======================================================================
			// 2005. 10. 12
			// 조회 될때마다 각 조회 구분별 최종 지표 기준 OCX로 삼는다.
			//=======================================================================
			lpChartMng->SetpChartCfgWnd(m_chType, (CWnd*)this);
			//=======================================================================

			//{{2006.10.09 by LYH 시가, 고가, 저가 추가
			long lCount = 0;
			MEMCPYWITHNULL (szTemp, pChartItemData->m_pfx112_ChartData->fcnt);
			lCount = atol(szTemp);

// --> [Edit]  강지원 2008/09/05
			//	시고저값 저장
			pChartItemData->GetOHLValue(m_strOpen, m_strHigh, m_strLow);
			//	종목명
			m_strKorName = pChartItemData->GetReciveValue(0);
			//	부호
			m_chSign = ' ';
			//	현재가
			m_strPrice = pChartItemData->GetReciveValue(1);
// <-- [Edit]  강지원 2008/09/05
			//}}
		}
		else
			strTrCode = _T("");

		ST_CHARTITEM_REALDATA* pChartItemRealData;
		if(m_mapChartItemRealData.Lookup(strCode, (LPVOID&)pChartItemRealData))
		{
			pChartItemRealData->m_strItemName = m_strKorName;
			pChartItemRealData->m_chSign = m_chSign;
			//m_strPrice.Replace(".", "");
			pChartItemRealData->m_strPrice = m_strPrice;
			pChartItemRealData->m_strChange = m_strChange;
			pChartItemRealData->m_strChrate = m_strChrate;
			pChartItemRealData->m_strVolume = m_strVolume;
			//m_strOpen.Replace(".", "");
			pChartItemRealData->m_strOpen = m_strOpen;
			//m_strHigh.Replace(".", "");
			pChartItemRealData->m_strHigh = m_strHigh;
			//m_strLow.Replace(".", "");
			pChartItemRealData->m_strLow = m_strLow;
			//	소수점 자리 단위
			pChartItemRealData->m_strPipLowest = pChartItemData->GetPipLowest();
		}

		GetParent()->InvalidateRect(lpChartMng->GetRectOfChart(m_lRowIndex,m_lColIndex,1));

		//20080915 이문수 >>
		InvokeAddIn("FX_ChartGuideLineAddin.dll:GUIDE_LINE", 300, atoi(m_strOrderType));
		//20080915 이문수 <<
		SendMessageToInvoke(INVOKEMSG_CHANGECODE, (LONG)(LPCTSTR)GetDataCode(),GetDataCode().GetLength());

		//{{ 주문선에 종목코드, 종목명 넘김
		if(m_strSelectedRQ == "DEFAULT")
		{
			CString strData = GetDataCode()+":"+m_strKorName;
			InvokeAddInStr("ChartPeriodSummaryAddIn.dll:ORDER_LINE", "102", strData);
		}
		//}}
	}
	else
	{
		strTrCode = pChartItemData->GetTrCode();
		if(strTrCode == QUERY_STRFX)
		{
			SM_fx112_OutRec1* pstOutput = (SM_fx112_OutRec1*)szData;
			CString strNextKey;
			strNextKey.Format("%*.*s", sizeof(pstOutput->nkey), sizeof(pstOutput->nkey), pstOutput->nkey);
			strNextKey.TrimLeft();
			if(strNextKey.GetLength() <1)
			{
				CString szMessage = "   이미 모든 데이터를 조회하였습니다.   ";
				lpChartMng->SetEndData(szMessage);
			}
		}
	}

	if(bRetValue)
	{
		if(!BuildChart()) return 0;
		if(FindChartItemDataForQuery())
		{
			return 2; // 해당ChartItem에서 추가적인 Request가 필요할때..
		}
		else
		{
			if(m_bLoadChartInfo)
			{
				SaveandOpenFromFile(GetOpenAndSaveFileName(),"",TRUE,FALSE);
				m_bLoadChartInfo = FALSE;
			}
			
			if(strTrCode == QUERY_STRFX)
			{
				GetParent()->GetParent()->SendMessage(UMSG_SENDTR_HOGA_DATA, (WPARAM)&strCode, 0);
			}

			// 복수종목 불러오기 : 복수종목 - ojtaso (20080521)
			if(m_mapChartItem.GetCount() > 1)
				CDRChartOcx::SetCurrentRQ(m_strSelectedRQ);

			return 1; // 해당ChartItem에서 추가적인 Request 완료...
		}
	}

	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	받은 데이터로 차트를 구성한다.
//////////////////////////////////////////////////////////////////////////////
BOOL CChartItem::BuildChart()
{
	// 새로운 차트 추가 : 복수종목 - ojtaso (20070108)
	BOOL bAdd = m_bBuild;

	m_bBuild = TRUE;
	long lSize = 0;
	CChartItemData *pChartItemData = FindChartItemDataForQuery();
	BOOL bIsPrimaryChartItemData = IsPrimaryChartItemData(pChartItemData);
	
	InvokeAddInStr("ChartKoreaSorimachiAddIn.dll:ASKED_PRICE_CKSA", "2", (LPCTSTR)m_strKorName);
	
	// 2005. 05. 18 add by sy, nam ===========================================
	// 중첩일 경우 몇개의 종목이 중첩되는지 그 총 수를 저장한다.
	if(m_bOverLapping == FALSE)
		m_nCntOverLap = 0;

	pChartItemData->m_nCntOverLap = m_nCntOverLap;
	// 복수종목 - ojtaso (20070108)
	CChartMng* pChartMng = (CChartMng*)GetParent();
	//========================================================================
	if(m_nSmallChartMode==1)
	{
		pChartItemData->AddPacketAndGraph(TRUE);
	}
	else if(bAdd && !pChartMng->m_bChangeTypeMode) 	// 새로운 차트 추가 : 복수종목 - ojtaso (20070108) 
	{
		if(((CChartMng*)GetParent())->GetMode() != OVERLAPP_ITEM)
			pChartItemData->AddPacketAndGraph2(pChartMng->GetAddGraphMode());
		else
			pChartItemData->AddPacketAndGraph2(pChartMng->GetAddGraphMode(), TRUE);
	}
	else
	{
		pChartItemData->AddPacketAndGraph();
		m_strOldCode = pChartItemData->GetCodeForRqstFromChartItemData();
	}
	
	// 선택된 종목명 세팅 : 복수종목 - ojtaso (20070108)
	if(!m_lpSelChart)
		m_strSelectedItemCode = pChartItemData->GetCodeForRqstFromChartItemData();

	m_bInitalChart = TRUE;
	if(bIsPrimaryChartItemData)
	{
		SetDrdsAdvise(0);		
	}

	// 새로운 차트 추가 : 복수종목 - ojtaso (20070108) 
	pChartMng->m_bChangeTypeMode = FALSE;

	// 2005. 08. 17 리얼 처리 환경이 바뀌면서 
	// 복수종목 추가 모드인지 : 복수종목 - ojtaso (20070119)
	if(!IsAddMode())
		pChartItemData->RemoveRealItem();

	ShowInnerToolBar(TRUE);

	// 분석툴 개별 저장 - ojtaso (20080723)
	BOOL bSaveAnalTool = ((CChartMng*)GetParent())->GetSaveAnalTool();
	BOOL bRetValueOfSetPacketData = FALSE;
	if(bSaveAnalTool)
		bRetValueOfSetPacketData = pChartItemData->SetPacketData(!m_bNext);

	//추가
	if(((CChartMng*)GetParent())->GetExistSyncChart())
	{
		SaveandOpenFromFile(GetOpenAndSaveFileName(), "", TRUE, FALSE);
	}
	if(m_bLoadChartInfo)
	{
		SaveandOpenFromFile(GetOpenAndSaveFileName(), "", TRUE, FALSE);
		m_bLoadChartInfo = FALSE;
	}
	if(bSaveAnalTool) // 분석툴 종목별 저장 - ojtaso (20080728)
	{
		// 2011.01.31 by SYS >> 분차트 YYYY(년) 추가 및 분석툴 공유 기능 추가
		//SaveandOpenFromAnalToolFile(GetOpenAndSaveFileName(TRUE), "", TRUE, FALSE);

		CString strAnalFile = GetOpenAndSaveAnalFileName(TRUE);
		SaveandOpenFromAnalToolFile(strAnalFile, "", TRUE, FALSE);
		// 2011.01.31 by SYS <<
	}

	//end

	if( !HasGraph("가격차트"))
	{
		AddGraph(0,0,0,"가격차트"		,FALSE	,FALSE	,FALSE	); 
		AddGraph(0,0,0,"가격 이동평균"	,FALSE	,FALSE	,FALSE	);
	}

	BOOL bNextOld = m_bNext;	
	// 분석툴 개별 저장 - ojtaso (20080723)
	if(!bSaveAnalTool)
		bRetValueOfSetPacketData = pChartItemData->SetPacketData(!m_bNext);
	
	if(m_nChartNumericValueType == WON_TYPE)
	{
		SetBShowPriceRateInquiry(TRUE);
	}
	else
	{
		SetBShowPriceRateInquiry(FALSE);
	}

	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	if(!bRetValueOfSetPacketData) 
	{
		if(m_lpSelChart)
			m_lpSelChart = NULL;

		return FALSE;
	}

	if(bIsPrimaryChartItemData)
	{
		lSize = pChartItemData->GetCnt();
		if(lSize)
		{
			if(bNextOld)
			{
				m_lAllCnt += lSize;
			}
			else
			{
				m_lAllCnt = lSize;
			}
		}
	}

	CString strDateToday;
	CTime t = CTime::GetCurrentTime();
	strDateToday = t.Format("%Y%m%d");

	if(strDateToday==m_strEndDate)
	{
		if(m_lPtrST)
		{
			pChartItemData->RemoveRealItem();
			SetDrdsAdvise(2);
		}
		else
		{			
			pChartItemData->AddRealItem();
			
			if(bIsPrimaryChartItemData)
			{
				SetDrdsAdvise(1);
			}
		}
	}
	else
	{
		if(bIsPrimaryChartItemData)
		{
			SetDrdsAdvise(2);
		}
	}

	CString strSharedTime = ((CChartMng*)GetParent())->GetSharedTime();
	if(!strSharedTime.IsEmpty())
	{
		SetTimeMark(strSharedTime,TRUE);
	}
	Invalidate();	
	if(m_lPtrST)
	{
		InvokeAddIn("ChartStrategyAddin.dll",1,m_lPtrST);
	}

	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	if(m_lpSelChart)
	{
		CDRChartOcx::SetCurrentRQ(m_strSelectedRQ);

		if(GetSelChart(m_strSelectedRQ, m_lpSelChart))
		{
			CChartItemData *pItemData = m_arrayChartItemData.GetAt(0);
			int nSize = m_arrayChartItemData.GetSize();

			BOOL bFiil = pItemData->IsFilled();

			OnSelectChartItem(m_lpSelChart, FALSE);

			pItemData->m_bIsFilled = bFiil;
		}

		m_lpSelChart = NULL;
	}

	return TRUE;
}

BOOL CChartItem::ResetChart()
{
	BOOL bRetValue = CDRChartOcx::ResetChart();	

	// 모든 리얼정보 삭제
	//{{JS.Kim_20100917. 종목변경없이 주기만 바꾸는 경우 실시간 수신 못받는 버그 수정
	if(((CChartMng*)GetParent())->GetExistSyncChart() == FALSE)
	//}}
	{
		POSITION pos = m_mapChartItemRealData.GetStartPosition();
		ST_CHARTITEM_REALDATA* lpChartItemRealData;
		CString strKey;
		while(pos)
		{
			m_mapChartItemRealData.GetNextAssoc(pos, strKey, (LPVOID&)lpChartItemRealData);
			if(lpChartItemRealData)
				delete lpChartItemRealData;
		}

		m_mapChartItemRealData.RemoveAll();
		m_strSelectedItemCode.Empty();
	}

	return bRetValue;
}

void CChartItem::ResetRQ()
{
	m_mapChartItemRealData.RemoveAll();
	m_strSelectedItemCode.Empty();
}

//////////////////////////////////////////////////////////////
///////////////////// func For Save & Load ChartInfo
BOOL CChartItem::LoadChartInfo (LPCTSTR lpTitle, BOOL bSetChartTick /* TRUE */, BOOL bNotCodeInputInit /* TRUE */)
{	
	CString strAppName;
	CString strTemp;
	CString strTemp2;
	char szTemp[128];
	char szTemp2[128];

	//차트 로드시 기존 저장된 차트가 하나도 없는 경우를 체크하기 위해 
	strAppName.Format ("%02d%02d",m_lColIndex,m_lRowIndex);
	::GetPrivateProfileString (strAppName, "CntOfGraph", "", szTemp, sizeof (szTemp), lpTitle);
	long lCntOfGraph = atol (szTemp);

	if(lCntOfGraph == 0)				// 저장된 차트 갯수가 전혀 없는 경우 
		bNotCodeInputInit = FALSE;		// 공유로 내려오는 종목코드를 그대로 CodeInput에 두기위해 FALSE로 셋한다.

	BOOL bLoadSaveCode = ((CChartMng*)GetParent())->GetpInputBarDlg()->SendMessage(UMSG_GETLOADSAVECODE);
	char* lpszExt = _T(".mct");
	if(::strstr(lpTitle, lpszExt))
	{
		if(bLoadSaveCode)
		{
			// 복수종목 로드 : 복수종목 - ojtaso (20070118)
			LoadRQInfo(strAppName, lpTitle);
		}
	}	

	if (bNotCodeInputInit == FALSE)
		ResetOCX(FALSE);			//종목코드 부분을 지우지 않는다.
	else
	{
		ResetChart();
		ResetOCX();					//종목코드 부분을 지운다.
	}

	//수정주가 디폴트 TRUE로 수정 2006.03.16 by LYH
	m_CfgInfo.bRevisedValue = TRUE;

	if(lCntOfGraph == 0) return FALSE;

	CChartItemData *pData = NULL;
	int nMarketType = -1;

	for (long lPos = 0; lPos < lCntOfGraph; lPos++)
	{
		strTemp.Format ("Code%02d",lPos);
		strTemp2.Format ("TRCode%02d",lPos);
		::GetPrivateProfileString (strAppName, strTemp, "", szTemp, sizeof (szTemp), lpTitle);
		::GetPrivateProfileString (strAppName, strTemp2, "", szTemp2, sizeof (szTemp2), lpTitle);
	
		if(lPos == 0)
		{
			CString strPreCode = ((CChartMng*)GetParent())->m_strCodeForLoadChart;
			if (strPreCode.GetLength () > 0 && ((CChartMng*)GetParent ())->GetpInputBarDlg ())
			{
				if (!bLoadSaveCode)
				{
					memset(szTemp, 0x00, sizeof(szTemp));
					memcpy(szTemp, (LPTSTR)(LPCTSTR)strPreCode, strPreCode.GetLength());
					nMarketType = ((CStdDialog*)m_pStdDialog)->GetMarketTypeFromCode (strPreCode);

					switch(nMarketType) 
					{
					case FX_CHART:
						memset(szTemp2, 0x00, sizeof(szTemp2));
						memcpy(szTemp2, (LPTSTR)(LPCTSTR)QUERY_STRFX, sizeof(szTemp2));
						break;
					}
				}
			}
			else
			{
				CString strQuery;
				strQuery.Format("%s", szTemp2);
				CString strShareCode = ((CChartMng*)GetParent())->m_strStatic_ShareCode;

				if (strQuery == QUERY_STRFX && strShareCode.GetLength() == CODE_STOCK_LENGTH)
				{
					if (((CChartMng*)GetParent ())->m_nStatic_RecvCntShareCode > 0)
					{
						if(strShareCode.GetLength() && IsAvailibleCode(strShareCode))
						{
							memset(szTemp, 0x00, sizeof(szTemp));
							memcpy(szTemp, (LPTSTR)(LPCTSTR)strShareCode, strShareCode.GetLength());
						}
					}
				}
			}
		}

		//==========================================================================
		//LYH 추가 2006.02.14
		CString strCode;
		strCode.Format("%s", szTemp);
		strCode.TrimLeft();
		if(strCode.GetLength()<1)
		{
			strCode = ((CChartMng*)GetParent())->m_strCodeForMulti;
			if(strCode.GetLength())
			{
				memset(szTemp, 0x00, sizeof(szTemp));
				memcpy(szTemp, (LPTSTR)(LPCTSTR)strCode, strCode.GetLength());
			}
		}
		//==========================================================================
			
		GetChartItemDataNew (szTemp2/*TRCode*/, szTemp/*종목Code*/);
	
		if( CString(szTemp).GetLength() == 0) 
		{
			ResetOCX();
			return FALSE;
		}
	}

	if( ((CStdDialog*)GetParent()->GetParent())->m_nChartType==MINI_MULTI_CHART ||
		((CStdDialog*)GetParent()->GetParent())->m_nChartType==MINI_SINGLE_CHART )
	{
		::GetPrivateProfileString(strAppName,"AllCnt","90",szTemp,sizeof(szTemp),lpTitle);
		m_lCntForRequest = atol(szTemp);
		if( (m_lCntForRequest<=0) || (m_lCntForRequest>CHTMXREQUEST) )	
			m_lCntForRequest = 90;
	}
	else
	{
		::GetPrivateProfileString(strAppName,"AllCnt", DEFAULT_CHARTDATA_COUNT_CHAR,szTemp,sizeof(szTemp),lpTitle);
		m_lCntForRequest = atol(szTemp);
		if( (m_lCntForRequest<=0) || (m_lCntForRequest>CHTMXREQUEST) )	
			m_lCntForRequest = DEFAULTCNTATONCE;
	}

	m_nSaveAllCount = m_lCntForRequest;

	::GetPrivateProfileString(strAppName,"CntInView","200",szTemp,sizeof(szTemp),lpTitle);
	long lTemp = atol(szTemp);
	if(lTemp<=0||lTemp>m_lCntForRequest)
	{
		SetNew_OnePageDataCount(200);
	}
	else
	{
		SetNew_OnePageDataCount(lTemp);
	}

	memset(szTemp, 0x00, sizeof(szTemp));
	::GetPrivateProfileString(strAppName,"TickValue",	"001",	szTemp,sizeof(szTemp),lpTitle);	
	SetTickNumber(CString(szTemp));

	memset(szTemp, 0x00, sizeof(szTemp));
	::GetPrivateProfileString(strAppName,"MinValue",	"001",	szTemp,sizeof(szTemp),lpTitle);
	SetMinNumber(CString(szTemp));
	
	memset(szTemp, 0x00, sizeof(szTemp));
	::GetPrivateProfileString(strAppName,"DayValue",	"001",	szTemp,sizeof(szTemp),lpTitle);
	SetDayNumber(CString (szTemp));

	if(bSetChartTick)
	{
		::GetPrivateProfileString(strAppName,"Type","2",szTemp,sizeof(szTemp),lpTitle);

		if(szTemp[0]<TICK_DATA_CHART || szTemp[0]>SEC_DATA_CHART) 
		{
			szTemp[0] = DAY_DATA_CHART;
		}
		BOOL bSetType = TRUE;

		if(bSetType)
			SetChartRequestType(szTemp[0]);
	}

	switch(m_chTypeForRequest)
	{
		case TICK_DATA_CHART: m_strTickMinDayForRequest = GetTickNumber(); break;
		case MIN_DATA_CHART : m_strTickMinDayForRequest = GetMinNumber() ; break;			
		case DAY_DATA_CHART : m_strTickMinDayForRequest = GetDayNumber() ; break;
		case HOUR_DATA_CHART: m_strTickMinDayForRequest = GetHourNumber(); break;
		default				: m_strTickMinDayForRequest = "001"			 ; break;  
	}	

	if(m_strTickMinDayForRequest.IsEmpty())
	{
		m_strTickMinDayForRequest = "001";
	}

	// 원, %, 달러로 3가지 구분이 필요한다. 
	::GetPrivateProfileString(strAppName,"ChartUnitValue","0",szTemp,sizeof(szTemp),lpTitle);	
	SetChartNumericValue(atoi(szTemp));

	//=================================================================================
	// 2005. 08. 27 add by nam
	// 마지막 차트의 내용을 저장 
	//=================================================================================
	// bRealCandle			// 실봉 or 실봉 + 허봉
	// bShowHLBar			// 상하한바 표시
	// bShowRight			// 귄리락 / 배당락 표시
	// bRecvTimeShare		// 시간연동 수신
	// bAutoSave			// 차트 종료시 자동저장 유무
	// bShowLog				// 차트 로그표시 여부
	// bRevisedValue		// 수정주가 표시 여부
	// bStartDate			// 차트에 선택되고 있는 날짜가 상단 툴바에서 조회 시작일을 선택했는지 여부.	


	//실봉 허봉
	::GetPrivateProfileString(strAppName,	"bRealCandle"		,"1",szTemp,sizeof(szTemp),lpTitle);	

	m_CfgInfo.bRealCandle =  atoi(szTemp) > 0 ? TRUE : FALSE; 
	SetBNormalizedMinuteTimeRule( atoi(szTemp) > 0 ? FALSE : TRUE);//atoi(szTemp)가 1 이면 실봉,  0이면 실봉

	//상하한바
	::GetPrivateProfileString(strAppName,	"bShowHLBar"		,"1",szTemp,sizeof(szTemp),lpTitle);	
	m_CfgInfo.bShowHLBar = atoi(szTemp) ? true : false;
	EnableBoundMark(m_CfgInfo.bShowHLBar);	

	//시간공유
	::GetPrivateProfileString(strAppName,	"bRecvTimeShare"	,"1",szTemp,sizeof(szTemp),lpTitle);	
	m_CfgInfo.bRecvTimeShare = atoi(szTemp) ? true : false;	
	EnableReceivingTimeShare(m_CfgInfo.bRecvTimeShare);

	//자동저장
	::GetPrivateProfileString(strAppName,	"bAutoSave"			,"0",szTemp,sizeof(szTemp),lpTitle);	
	m_CfgInfo.bAutoSave = atoi(szTemp) ? true : false;

	//로그 보이기 여부.
	::GetPrivateProfileString(strAppName,	"bShowLog"			,"0",szTemp,sizeof(szTemp),lpTitle);	
	m_CfgInfo.bShowLog = atoi(szTemp) ? true : false;
	GetParent()->SendMessage( UMSG_SETLOG, m_CfgInfo.bShowLog, 0);

	//수정주가.
	::GetPrivateProfileString(strAppName,	"bRevisedValue"		,"0",szTemp,sizeof(szTemp),lpTitle);
	SetUseRevisedValue(atoi(szTemp) ? true : false);
	//m_CfgInfo.bRevisedValue = atoi(szTemp) ? true : false;
	GetParent()->SendMessage(USMG_NOTIFY_REVISED, atoi(szTemp));
	//=================================================================================
	
	if(((CChartMng*)GetParent())->m_strCodeForLoadChart.GetLength() == 0)
	{
		// 복수종목 로드 : 복수종목 - ojtaso (20070118)
		LoadRQInfo(strAppName, lpTitle);
	}

	return TRUE;
}

BOOL CChartItem::IsAvailibleCode(LPCTSTR lpszCodeName)
{
	return TRUE;
}

// 복수종목 로드 : 복수종목 - ojtaso (20070118)
void CChartItem::LoadRQInfo(LPCTSTR lpszAppName, LPCTSTR lpszTitle)
{
	UINT nSize = ::GetPrivateProfileInt(lpszAppName, _T("RQSIZE"), 0, lpszTitle);

	CString strRQData;
	LPSTR lpszRQDAta = strRQData.GetBufferSetLength(nSize + 1);

	::GetPrivateProfileString(lpszAppName, _T("RQ"), _T("0"), lpszRQDAta, nSize, lpszTitle);

	strRQData.ReleaseBuffer();

	CString strTemp;
	int nIndex = 0;
	if(!AfxExtractSubString(strTemp, strRQData, nIndex++, _T(';')))
		return;

	int nCount = atoi(strTemp);

	if(!nCount)
		return;

	AfxExtractSubString(m_strSelectedRQ, strRQData, nIndex++, _T(';'));

	m_bLoadRQ = TRUE;

	CChartMng* lpChartMng = ((CChartMng*)GetParent());
	CString strShareCode = lpChartMng->m_strStatic_ShareCode;

	BOOL bChangeCodeByShare = TRUE;
	BOOL bDefualtRQ = FALSE;

	CString strRQ;
	for(int i = 0; i < nCount; i++)
	{
		AfxExtractSubString(strRQ, strRQData, nIndex++, _T(';'));

		ST_SELCHART* lpSelChart = NULL;

		bDefualtRQ = !strRQ.Compare(_T("DEFAULT"));

		if(!bDefualtRQ )
			lpSelChart = new ST_SELCHART;
		else
			m_mapChartItem.Lookup(strRQ, (LPVOID&)lpSelChart);

		AfxExtractSubString(lpSelChart->m_strChartGubun, strRQData, nIndex++, _T(';'));

		if(bDefualtRQ && lpSelChart)
		{
			if(((lpSelChart->m_strChartGubun == QUERY_STRFX/* && strShareCode.GetLength() == CODE_STOCK_LENGTH*/) || 
				(lpSelChart->m_strChartGubun == QUERY_STRFUTOPT/* && (strShareCode.GetLength() == FUTOPT_LENGTH)*/)) &&
				lpChartMng->m_nStatic_RecvCntShareCode > 0 && !strShareCode.IsEmpty() && bChangeCodeByShare
				)
			{
				if(((CStdDialog*)GetParent()->GetParent())->m_strConfig.Find("__") < 0)
				{
					lpSelChart->m_strItemCode = strShareCode;
					lpSelChart->m_strItemName = ((CStdDialog*)m_pStdDialog)->GetJongMokNameByCode(strShareCode);
					nIndex += 2;
				}
				else
				{
					AfxExtractSubString(lpSelChart->m_strItemCode, strRQData, nIndex++, _T(';'));
					AfxExtractSubString(lpSelChart->m_strItemName, strRQData, nIndex++, _T(';'));

					if(lpSelChart->m_strItemCode.IsEmpty() || lpSelChart->m_strItemName.IsEmpty())
					{
						lpSelChart->m_strItemCode = strShareCode;
						lpSelChart->m_strItemName = ((CStdDialog*)m_pStdDialog)->GetJongMokNameByCode(strShareCode);
					}
				}
			}
			else
			{
				AfxExtractSubString(lpSelChart->m_strItemCode, strRQData, nIndex++, _T(';'));
				AfxExtractSubString(lpSelChart->m_strItemName, strRQData, nIndex++, _T(';'));
			}
		}
		else
		{
			AfxExtractSubString(lpSelChart->m_strItemCode, strRQData, nIndex++, _T(';'));
			AfxExtractSubString(lpSelChart->m_strItemName, strRQData, nIndex++, _T(';'));

			if(bChangeCodeByShare && lpSelChart)
			{
				bChangeCodeByShare = lpSelChart->m_strItemCode.CompareNoCase(strShareCode);
				if(!bChangeCodeByShare)
					m_strSelectedRQ = strRQ;
			}
		}

		AfxExtractSubString(strTemp, strRQData, nIndex++, _T(';'));
		lpSelChart->m_cType = strTemp.GetAt(0);
		AfxExtractSubString(lpSelChart->m_strTickMinDayNumber, strRQData, nIndex++, _T(';'));
		AfxExtractSubString(strTemp, strRQData, nIndex++, _T(';'));
		lpSelChart->m_lCount = atol(strTemp);
		AfxExtractSubString(strTemp, strRQData, nIndex++, _T(';'));
		lpSelChart->m_nChartNumericValueType = atoi(strTemp);
		AfxExtractSubString(strTemp, strRQData, nIndex++, _T(';'));
		lpSelChart->m_strKey = strTemp;
		//<<20100316_JS.Kim 솔로몬.FX
		AfxExtractSubString(strTemp, strRQData, nIndex++, _T(';'));
		lpSelChart->m_nOrderType = atoi(strTemp);
		//>>

		m_mapChartItem.SetAt(strRQ, lpSelChart);

		if(!m_strSelectedRQ.Compare(strRQ))
		{
			m_strSelectedItemCode = lpSelChart->m_strItemCode;
			m_lpSelChart = lpSelChart;
		}
	}
}

BOOL CChartItem::LoadChartSetting(LPCTSTR lpTitle)
{
	//05.09.26 ===========================================================
	if( (m_lColIndex < 0) || (m_lColIndex > 20) ) return FALSE;
	if( (m_lRowIndex < 0) || (m_lRowIndex > 20) ) return FALSE;
	//====================================================================

	int nSize = m_arrayChartItemData.GetSize();	
	if(!nSize) return FALSE;
	CString strAppName;

	

	strAppName.Format("%02d%02d",m_lColIndex,m_lRowIndex);
	CString strLoadFileName = lpTitle;
	int nPos = strLoadFileName.ReverseFind('.');
	strLoadFileName.Delete(nPos,strLoadFileName.GetLength()-nPos);
	while(1)
	{
		nPos = strLoadFileName.Find('\\');
		if(nPos!=-1)
		{
			strLoadFileName.Delete(0,nPos+1);
		}
		else
		{
			break;
		}	
	}
	strLoadFileName += '_';
	strLoadFileName += strAppName;

	SaveandOpenFromFile(strLoadFileName,"", TRUE, FALSE);
	
	// 분석툴 종목별 저장 - ojtaso (20080728)
	SaveandOpenFromAnalToolFile(strLoadFileName, "", TRUE, FALSE);


	//====================================================================================
	// 05.09.23
	// 한화에서 서버에 큐가 많이 쌓여 차트 Data조회를 날려도 
	// 응답을 못 받는 경우가 자주 일어난다. ㅡ.ㅡ;;
	// 이럴 경우 다음 번 차트가 정상적인 데이터 조회시 
	// 차트가 제대로 안 그려지는 현상이 발생하는 데
	// 이 문제를 해결하기 위해 차트 블럭이 있는지 체크해서 없다면 
	// 필요한 기본 차트 블럭을 넣는다.
	//====================================================================================
	if( !HasGraph("가격차트"))
	{
		AddGraph(0,0,0,"가격차트"		,FALSE	,FALSE	,FALSE	); 
		AddGraph(0,0,0,"가격 이동평균"	,FALSE	,FALSE	,FALSE	);
//		AddGraph(1,0,0,"거래량차트"		,TRUE	,TRUE	,TRUE	);
		//AddGraph(1,0,0,"거래량 이동평균",FALSE	,FALSE	,FALSE	);
	}
	//====================================================================================


	if( GetAllCnt() < GetNOnePageDataCount() )
		SetNew_OnePageDataCount(GetAllCnt());

	return TRUE;
}

BOOL CChartItem::SaveChartInfo(LPCTSTR lpTitle, BOOL bOnlyJongmok /*=FALSE*/)
{
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::SaveChartInfo::Begin", _T("멀티차트 종료"), 13);
#endif
	if(!m_bUseInnerControl&&!m_bBuild) return FALSE;

	long lSize = m_arrayChartItemData.GetSize();
	CString strAppName;
	CString strTemp;	
	CString strTemp2;	
	strAppName.Format("%02d%02d",m_lColIndex,m_lRowIndex);

	long lCntOfGraph = m_arrayChartItemData.GetSize();	
	
	CChartItemData *pData = NULL;
	BOOL bHasChartItemData = FALSE;
	long lCntChartItemData = 0;

	if(lCntOfGraph == 0)
	{
		::WritePrivateProfileString(strAppName, "Code00","",lpTitle);
		::WritePrivateProfileString(strAppName, "TRCode00","",lpTitle);	
	}
	else
	{	
		for(long lPos=0;lPos<lCntOfGraph;lPos++)
		{
			bHasChartItemData = TRUE;
			pData = m_arrayChartItemData.GetAt(lPos);		
			if(lPos!=0)
			{
				//bHasChartItemData = HasChartItemData(pData->GetCode(),pData->GetLTrCode(), FALSE, FALSE);
				//한화에서는 CString Type TRCode를 쓴다. 05.05. 06
				CString strCode		= pData->GetCode();
				CString strTRCode	= pData->GetTrCode();

				bHasChartItemData = HasChartItemData(pData->GetCode(),pData->GetTrCode(), FALSE, FALSE);
			}
			if(bHasChartItemData)
			{
				strTemp.Format("Code%02d",lCntChartItemData);
				strTemp2.Format("TRCode%02d",lCntChartItemData);

				if(bOnlyJongmok && lPos == 0)
				{
					::WritePrivateProfileString(strAppName,strTemp,"",lpTitle);
					::WritePrivateProfileString(strAppName,strTemp2,pData->GetTrCode(),lpTitle);
				}
				else
				{	
					::WritePrivateProfileString(strAppName,strTemp,pData->GetCode(),lpTitle);
					::WritePrivateProfileString(strAppName,strTemp2,pData->GetTrCode(),lpTitle);
				}
				
				lCntChartItemData++;
			}
		}
	}



	strTemp.Format("%d",lCntChartItemData);
	::WritePrivateProfileString(strAppName,"CntOfGraph",strTemp,lpTitle);
	
	strTemp.Format("%d",m_nSaveAllCount);
	if(m_nSaveAllCount <10)
		strTemp.Format("%d",DEFAULTCNTATONCE);
	::WritePrivateProfileString(strAppName,"AllCnt",strTemp,lpTitle);

	strTemp.Format("%d",GetNOnePageDataCount());	
	if(atoi(strTemp) <1)
		strTemp.Format("%d",200);
	::WritePrivateProfileString(strAppName,"CntInView",strTemp,lpTitle);

	strTemp.Format("%c",m_chType);
	::WritePrivateProfileString(strAppName,"Type",strTemp,lpTitle);
	
	//=================================================================================
	// 2005. 05. 23 added  by nam	- 틱분월 별로 별도로 저장한다.
	// 2005. 09. 12 edited by nam	- 틱분월 모두 저장한다.
	//=================================================================================
	::WritePrivateProfileString(strAppName,"TickValue",	m_strTick,	lpTitle);		
	::WritePrivateProfileString(strAppName,"SecValue",	m_strSec,	lpTitle);		
	::WritePrivateProfileString(strAppName,"MinValue" ,	m_strMin ,	lpTitle);
	::WritePrivateProfileString(strAppName,"DayValue" ,	m_strDay ,	lpTitle);
	
	//05.06.22 by sy, nam
	// 원, %, 달러로 3가지 구분이 필요한다. 
	strTemp.Format("%d",GetChart_NumericValueType());
	::WritePrivateProfileString(strAppName,"ChartUnitValue",strTemp,lpTitle);	
	
	//=================================================================================
	// 2005. 08. 27 add by nam
	// 마지막 차트의 내용을 저장 
	//=================================================================================
	// bRealCandle			// 실봉 or 실봉 + 허봉
	// bShowHLBar			// 상하한바 표시
	// bShowRight			// 귄리락 / 배당락 표시
	// bRecvTimeShare		// 시간연동 수신
	// bAutoSave			// 차트 종료시 자동저장 유무
	// bShowLog				// 차트 로그표시 여부
	// bRevisedValue		// 수정주가 표시 여부
	// bStartDate			// 차트에 선택되고 있는 날짜가 상단 툴바에서 조회 시작일을 선택했는지 여부.

	strTemp.Format("%d", m_CfgInfo.bRealCandle		);	
	::WritePrivateProfileString(strAppName,	"bRealCandle"		,strTemp,lpTitle);

	strTemp.Format("%d", m_CfgInfo.bShowHLBar		);
	::WritePrivateProfileString(strAppName,	"bShowHLBar"		,strTemp,lpTitle);

	strTemp.Format("%d", m_CfgInfo.bRecvTimeShare	);
	::WritePrivateProfileString(strAppName,	"bRecvTimeShare"	,strTemp,lpTitle);

	strTemp.Format("%d", m_CfgInfo.bAutoSave		);
	::WritePrivateProfileString(strAppName,	"bAutoSave"			,strTemp,lpTitle);

	strTemp.Format("%d", m_CfgInfo.bShowLog			);
	::WritePrivateProfileString(strAppName,	"bShowLog"			,strTemp,lpTitle);

	strTemp.Format("%d", m_CfgInfo.bRevisedValue	);
	::WritePrivateProfileString(strAppName,	"bRevisedValue"		,strTemp,lpTitle);
	//=================================================================================

	if(HasGraph("가격차트")) //05.10.26 =============== 가격 차트가 있을 때만 저장한다.
	{
		SaveChartSetting(lpTitle);
	}

	// 복수종목 저장 : 복수종목 - ojtaso (20070118)
	SaveRQInfo(strAppName, lpTitle);

#ifdef _WRITELOG
	WriteToStringLog("CChartItem::SaveChartInfo::End", _T("멀티차트 종료"), 13);
#endif
	return TRUE;
}

// 복수종목 저장 : 복수종목 - ojtaso (20070118)
void CChartItem::SaveRQInfo(LPCTSTR lpszAppName, LPCTSTR lpszTitle)
{
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::SaveRQInfo::Begin", _T("멀티차트 종료"), 13);
#endif
	POSITION pos = m_mapChartItem.GetStartPosition();
	CString strKey;
	ST_SELCHART* lpSelChart = NULL;

	CString strValue, strTemp;
	strValue.Format(_T("%d;%s"), m_mapChartItem.GetCount(), m_strSelectedRQ);

	while(pos)
	{
		m_mapChartItem.GetNextAssoc(pos, strKey, (LPVOID&)lpSelChart);
		strTemp.Format(_T(";%s;%s;%s;%s;%c;%s;%ld;%d;%s;%d"), strKey,
			lpSelChart->m_strChartGubun,
			lpSelChart->m_strItemCode,
			lpSelChart->m_strItemName,
			lpSelChart->m_cType,
			lpSelChart->m_strTickMinDayNumber,
			lpSelChart->m_lCount,
			lpSelChart->m_nChartNumericValueType,
			lpSelChart->m_strKey,
			lpSelChart->m_nOrderType);

		strValue += strTemp;
	}

	strTemp.Format(_T("%d"), strValue.GetLength() + m_strSelectedRQ.GetLength() + 1);

	::WritePrivateProfileString(lpszAppName,	"RQSIZE"	,strTemp,lpszTitle);
	::WritePrivateProfileString(lpszAppName,	"RQ"		,strValue,lpszTitle);
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::SaveRQInfo::End", _T("멀티차트 종료"), 13);
#endif
}

BOOL CChartItem::SaveChartSetting(LPCTSTR lpTitle)
{
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::SaveChartSetting::Begin", _T("멀티차트 종료"), 13);
#endif
	CString strAppName;
	strAppName.Format("%02d%02d",m_lColIndex,m_lRowIndex);
	CString strSaveFileName = lpTitle;

	int nPos = strSaveFileName.ReverseFind('.');
	strSaveFileName.Delete(nPos,strSaveFileName.GetLength()-nPos);
	while(1)
	{
		nPos = strSaveFileName.Find('\\');
		if(nPos!=-1)
		{
			strSaveFileName.Delete(0,nPos+1);
		}
		else
		{
			break;
		}	
	}
	strSaveFileName += '_';
	strSaveFileName += strAppName;
	SaveandOpenFromFile("", strSaveFileName, FALSE, TRUE);
	
	// 분석툴 종목별 저장 - ojtaso (20080728)
	SaveandOpenFromAnalToolFile("", strSaveFileName, FALSE, TRUE);

#ifdef _WRITELOG
	WriteToStringLog("CChartItem::SaveChartSetting::End", _T("멀티차트 종료"), 13);
#endif
	return TRUE;
}

BOOL CChartItem::RemoveChartItemData(LPCTSTR lpCode, CString strQueryType) 
{
	CChartItemData *pChartItemData = NULL;
	long lIndex = GetPosChartItemData(strQueryType,lpCode);
	if(lIndex==-1) return FALSE;
	pChartItemData = m_arrayChartItemData.GetAt(lIndex);
	delete pChartItemData;
	pChartItemData = NULL;
	m_arrayChartItemData.RemoveAt(lIndex);
	return TRUE;
}

BOOL CChartItem::RemoveChartItem_OneData(LPCTSTR lpCode, CString strQueryType) 
{
	CChartItemData *pChartItemData = NULL;		

		
	for(int nPos=0; nPos <m_arrayMarketTRData.GetSize() ;nPos++)
	{
		pChartItemData = NULL;
		pChartItemData = m_arrayMarketTRData.GetAt(nPos);

		if(pChartItemData == NULL) continue;

		CString sT1 = pChartItemData->GetTrCode();
		CString sT2 = pChartItemData->GetCodeForRqstFromChartItemData();

		if(sT1==strQueryType
			&&sT2==CString(lpCode))
		{
			if(nPos==-1)		
			{
				pChartItemData = NULL;
				continue;
			}
#ifdef _DEBUG
			CString sTemp;
			sTemp.Format("시장 지표 제거 %s__%s", lpCode, strQueryType);
			OutputDebugString(sTemp);
#endif
			
			delete pChartItemData;
			pChartItemData = NULL;
			m_arrayMarketTRData.RemoveAt(nPos);
		}
	}
	
	return TRUE;
}




BOOL CChartItem::DestoryItemData()
{
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::DestoryItemData::Begin", _T("멀티차트 종료"), 13);
#endif

	m_bReceivedDataChart = FALSE; //05.09.22

	CChartItemData *pChartItemData = NULL;
#ifdef _WRITELOG
	WriteToStringLog("m_arrayChartItemData::RemoveAll::Begin", _T("멀티차트 종료"), 13);
#endif
	int nSize = m_arrayChartItemData.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pChartItemData = m_arrayChartItemData.GetAt(nPos);
		delete pChartItemData;
	}
	m_arrayChartItemData.RemoveAll();
#ifdef _WRITELOG
	WriteToStringLog("m_arrayChartItemData::RemoveAll::End", _T("멀티차트 종료"), 13);
	WriteToStringLog("m_arrayChartItemDataBackup::RemoveAll::Begin", _T("멀티차트 종료"), 13);
#endif
	nSize = m_arrayChartItemDataBackup.GetSize();
	for(nPos=0;nPos<nSize;nPos++)
	{
		pChartItemData = m_arrayChartItemDataBackup.GetAt(nPos);
		delete pChartItemData;
	}
	m_arrayChartItemDataBackup.RemoveAll();
#ifdef _WRITELOG
	WriteToStringLog("m_arrayChartItemDataBackup::RemoveAll::End", _T("멀티차트 종료"), 13);
	WriteToStringLog("m_arrayMarketTRData::RemoveAll::Begin", _T("멀티차트 종료"), 13);
#endif

	//==============================================================
	//시장 지표 관련 TR Array를 제거한다.
	//==============================================================
	nSize = m_arrayMarketTRData.GetSize();
	for(nPos=0;nPos<nSize;nPos++)
	{
		pChartItemData = m_arrayMarketTRData.GetAt(nPos);
		delete pChartItemData;
	}
	m_arrayMarketTRData.RemoveAll();
	//==============================================================
#ifdef _WRITELOG
	WriteToStringLog("m_arrayMarketTRData::RemoveAll::End", _T("멀티차트 종료"), 13);
#endif

#ifdef _WRITELOG
	WriteToStringLog("CChartItem::DestoryItemData::End", _T("멀티차트 종료"), 13);
#endif	
	return TRUE;
}

CChartItemData *CChartItem::FindChartItemDataForQuery()
{
	CChartItemData *pItemData = NULL;
	int nSize = m_arrayChartItemData.GetSize();
	
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pItemData = m_arrayChartItemData.GetAt(nPos);
		if(!pItemData->IsFilled())
		{
			return pItemData;
		}
	}
	return NULL;
}

BOOL CChartItem::ClearDataExceptPrimaryData()
{
	CChartItemData *pItemData = NULL;
	int nSize = m_arrayChartItemData.GetSize();
	for(int nPos=nSize-1;nPos>0;nPos--)
	{
		pItemData = m_arrayChartItemData.GetAt(nPos);
		m_arrayChartItemData.RemoveAt(nPos);
		delete pItemData;
	}
	return TRUE;
}

BOOL CChartItem::ClearData()
{
	CChartItemData *pItemData = NULL;
	int nSize = m_arrayChartItemData.GetSize();

	// gm0604 added 2005.10.10
	if(m_lPtrST&&nSize>0)
	{
		m_lPtrST = NULL;// gm0604 added 2005.9.6
		CChartItemData *pItemDataNew = NULL;
		pItemData = m_arrayChartItemData.GetAt(0);
		CString strTrCode = pItemData->GetTrCode();
		CString strCode = pItemData->GetCode();
		pItemData->Init(this,strTrCode);
		pItemData->SetCodeForQuery(strCode);	

		CStringArray arrayGraph;
		InvokeAddIn("ChartStrategyAddin.dll",8,(long)&arrayGraph);
		int nSize = arrayGraph.GetSize();
		for(int i=0;i<nSize;i++)
		{
			RemoveChart(arrayGraph.GetAt(i));
		}
		((CChartMng*)GetParent())->MoveCharts(m_lRowIndex,m_lColIndex);
	}
	m_lPtrST = NULL;// gm0604 added 2005.9.6


	for(int nPos=0;nPos<nSize;nPos++)
	{
		pItemData = m_arrayChartItemData.GetAt(nPos);
		pItemData->ClearData();
	}	
	if(GetDataTRCode()==QUERY_STRFX)
	{
		((CChartMng*)GetParent())->UnhighlightJCode(this);
	}		
	return TRUE;
}

CChartItemData* CChartItem::GetChartItemDataNew(CString strTRCode,LPCTSTR lpCodeForQuery, BOOL bCreateIfNoExist /*= TRUE*/,BOOL bIsPrimaryData /*= FALSE */)
{
	CString strCodeForQuery;
	CChartItemData *pItemData = NULL;
	if(lpCodeForQuery!=NULL)
	{
		strCodeForQuery = lpCodeForQuery;
	}	
	if(strCodeForQuery.IsEmpty()) return NULL;

	int nSize = m_arrayChartItemData.GetSize();	
	if(bIsPrimaryData&&nSize)
	{
		// Clear the same ItemData Without PrimaryData..
		for(int nPos=1;nPos<nSize;nPos++)
		{
			pItemData = m_arrayChartItemData.GetAt(nPos);
			if(pItemData->GetTrCode()==strTRCode
				&&pItemData->GetCode()==strCodeForQuery)
			{
				delete pItemData;
				pItemData = NULL;
				m_arrayChartItemData.RemoveAt(nPos);
				nSize--;
			}
		}

		pItemData = m_arrayChartItemData.GetAt(0);
		//if(pItemData->GetLTrCode()==lTrCode)	// if Primary Data 's TrCode is the same as New Data's , ...
		//한화에서는 CString Type TRCode를 쓴다. 05.05. 06
		if(pItemData->GetTrCode()==strTRCode)	// if Primary Data 's TrCode is the same as New Data's , ...
		{
			ClearData();
			pItemData->SetCodeForQuery(strCodeForQuery);
		}
		else
		{
			//{{2007.05.30 by LYH 시장 구분 없이 종목 추가 하도록 수정
			ClearData();
			pItemData->SetTRCode(strTRCode);
			pItemData->SetCodeForQuery(strCodeForQuery);
			//}}
		}		
		return pItemData;
	}	

	for(int nPos=0;nPos<nSize;nPos++)
	{
		pItemData = m_arrayChartItemData.GetAt(nPos);
		if(strCodeForQuery.IsEmpty())
		{
			if(pItemData->GetTrCode()==strTRCode)
			{
				if(nPos==0&&!bIsPrimaryData)
				{
					return NULL;
				}
				return pItemData;
			}
		}
		else
		{
			if(pItemData->GetTrCode()==strTRCode
				&&pItemData->GetCode()==strCodeForQuery)
			{
				if(nPos==0&&!bIsPrimaryData)
				{
					return NULL;
				}
				return pItemData;
			}
		}
	}


	if(bCreateIfNoExist)
	{	
		pItemData = new CChartItemData(this,strTRCode);
		pItemData->SetCodeForQuery(strCodeForQuery);	
		m_arrayChartItemData.Add(pItemData);
		//if(m_signal.GetSafeHwnd())
		//{
		//	m_signal.OffDrdsAdvise();
		//	m_signal.OnDrdsAdvise(strCodeForQuery);
		//}
		
#ifdef _DEBUG
		CString sTemp;
		sTemp.Format("GetChartItemDataNew::m_arrayChartItemData 갯수 %d", m_arrayChartItemData.GetSize());
		OutputDebugString(sTemp);
#endif 
		return pItemData;
	}
	return NULL;	
}

//long CChartItem::GetPosChartItemData(long lTrCode,LPCTSTR lpCodeForQuery)
long CChartItem::GetPosChartItemData(CString strTrCode,LPCTSTR lpCodeForQuery)
{
	CChartItemData *pItemData = NULL;
	//CString strTrCode;
	//strTrCode.Format("%d",lTrCode);
	int nSize = m_arrayChartItemData.GetSize();	
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pItemData = m_arrayChartItemData.GetAt(nPos);
		if(pItemData->GetTrCode()==strTrCode
			&&pItemData->GetCode()==lpCodeForQuery)
		{
			return nPos;
		}
	}
	return -1;
}

CChartItemData *CChartItem::GetPrimaryChartItemData()
{
	CChartItemData *pItemDataInArray = NULL;
	if(!m_arrayChartItemData.GetSize()) return NULL;
	return m_arrayChartItemData.GetAt(0);
}

BOOL CChartItem::IsPrimaryChartItemData(CChartItemData *pChartItemData)
{
	CChartItemData *pItemDataInArray = NULL;
	if(!m_arrayChartItemData.GetSize()) return FALSE;
	pItemDataInArray = m_arrayChartItemData.GetAt(0);
	if(pItemDataInArray==pChartItemData)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CChartItem::SaveandOpenFromFile(LPCTSTR strOpenFileName, LPCTSTR strSaveFileName, BOOL bOpenOption, BOOL bSaveOption)
{
	//==============================================================================================
	// 파일을 저장하기 전에 해당 차트의 사용자가 입력한 봉갯수를 기반으로 저장한다.
	// 스크롤 한 상태로 저장하지 않는다.
	// 2005. 11. 17
	//==============================================================================================		
	//파일명이 temp.tmp인 경우는 
	//기간동기화
	//환경 동기화
	//할때 쓰는 임시 파일명이므로 
	//사용자가 임의로 설정한 기준을 흔를지 않기 위해 
	//사용자가 기본으로 입력한 봉갯수 적용을 해서 파일을 저장하지 않는다.
	CString sSaveFile;
	sSaveFile.Format("%s", strSaveFileName); sSaveFile.TrimLeft(); sSaveFile.TrimRight();

	if( (_T("temp.tmp") != sSaveFile) && sSaveFile.GetLength() )
	{
		if(::IsWindow(GetParent()->GetSafeHwnd()))
		{
			int nCnt = GetParent()->SendMessage(UMSG_GETZOOMVALUE, GetType());
			if(nCnt > 0)
				CDRChartOcx::SetNOnePageDataCount(nCnt);
		}		
	}
	//==============================================================================================

	//2007.01.29 by LYH
	CString sOpenFile;
	sOpenFile.Format("%s", strOpenFileName); sOpenFile.TrimLeft(); sOpenFile.TrimRight();
	if(sOpenFile.Find(".dat") >0)
	{
		CFileFind ff;
		CString sTempPath;
		sTempPath = ((CChartMng*)GetParent())->GetUserpath() + "\\" + sOpenFile;
		BOOL bResult = ff.FindFile(sTempPath);
		if(!bResult)
		{
			ResetChart();
		}
	}
	//}}

// --> [Edit]  강지원 2009/07/06	( 임시 )
// 입력바의 표시봉의 갯수가 보존되지 않아서 값을 확인하려고 넣은 코드임.
// DRFN에 문의해봐야 할것같다는 의견임.
	int nCnt = GetNOnePageDataCount();
// <-- [Edit]  강지원 2009/07/06

	BOOL bRetValue = CDRChartOcx::SaveandOpenFromFile( strOpenFileName, strSaveFileName, bOpenOption, bSaveOption);	

// --> [Edit]  강지원 2009/07/06	( 임시 )
// 입력바의 표시봉의 갯수가 보존되지 않아서 값을 확인하려고 넣은 코드임.
// DRFN에 문의해봐야 할것같다는 의견임.
	nCnt = GetNOnePageDataCount();
// <-- [Edit]  강지원 2009/07/06
	//==============================================================
	// 2005. 08. 23 added by sy, nam
	// 차트의 로드일 경우  LogScale여부를 보인다.
	if(bOpenOption) 
	{
		m_CfgInfo.bShowLog = GetLogScale(_T("")) ? true : false;
		m_bLogPrice = m_CfgInfo.bShowLog;
		GetParent()->SendMessage( UMSG_SETLOG, m_CfgInfo.bShowLog, 0);


		if( GetAllCnt() < GetNOnePageDataCount() ) //05.11.24 봉갯수가 적게 보이는 경우...
			SetNew_OnePageDataCount(GetAllCnt());

	}
	//==============================================================

	CDRChartOcx::SetStrUserIndicatorListFileName("");
	CDRChartOcx::SetStrUserEnvironmentFileName("");

	

	/* 락에 대한 정보 컨트롤은 OCX에서... 05.11.24
	if(bOpenOption)
	{
		m_bShowLock = m_CfgInfo.bShowRight;
		ShowLock(m_bShowLock);
	}
	*/
	return bRetValue;
}

// 분석툴 종목별 저장 - ojtaso (20080728)
BOOL CChartItem::SaveandOpenFromAnalToolFile(LPCTSTR strOpenFileName, LPCTSTR strSaveFileName, BOOL bOpenOption, BOOL bSaveOption)
{
// --> [Edit]  이도영 2008/09/19
 
//	CDRChartOcx::SetItemInfo(m_strSelectedItemCode, m_strSelectedItemCode);
	BOOL bRetValue = CDRChartOcx::SaveandOpenFromAnalToolFile( strOpenFileName, strSaveFileName, bOpenOption, bSaveOption);	

	CDRChartOcx::SetStrUserIndicatorListFileName("");
	CDRChartOcx::SetStrUserEnvironmentFileName("");

	return bRetValue;
// <-- [Edit]  이도영 2008/09/19
}

void CChartItem::SetNext(BOOL bNext)
{ 
	if(!m_arrayChartItemData.GetSize()) return;
	m_bNext = bNext;
}

void CChartItem::SetCntForRequest(UINT nCntForRequest) 
{ 
	ClearData();
	if(nCntForRequest>CHTMXREQUEST)
	{
		nCntForRequest = CHTMXREQUEST;
	}
	else if(nCntForRequest <= 0) //05.10.14
		nCntForRequest = DEFAULTCNTATONCE;	 //05.10.14

	m_lCntForRequest = nCntForRequest;
	m_lAllCnt = 0;
}

CString CChartItem::GetDataCode(int nIndex/*=0*/)
{ 
	if(m_arrayChartItemData.GetSize()<=nIndex) return "";
	return m_arrayChartItemData.GetAt(nIndex)->GetCode();	
}

CString CChartItem::GetDataTRCode(int nIndex/*=0*/)
{ 
	if(m_arrayChartItemData.GetSize()<=nIndex) return "";
	return m_arrayChartItemData.GetAt(nIndex)->GetTrCode();	
}

long CChartItem::GetCntForRequest()
{ 
	if(m_nTotCountForRevised > 0)
	{
		int nRet = m_nTotCountForRevised;
		m_nTotCountForRevised = 0;
		return nRet;
	}
	return m_lCntForRequest;
}


CString CChartItem::GetEndDateRequested() 
{ 
	return m_strDateForRequest;
}

CString CChartItem::GetEndDate() 
{ 
	return m_strEndDate;
}


//void CChartItem::SetUseFackedCandle(BOOL bUse)
//{
//	ClearData();
//	m_bShowingFackedValue = bUse;
//}



void CChartItem::SetUseLogValue(BOOL bUse)
{	
	m_bLogPrice = bUse;
	m_CfgInfo.bShowLog = bUse ? true : false;
}
/*
void CChartItem::SetUseRateValue(BOOL bUse)
{	
	CChartItemData *pItemData = NULL;
	int nSize = m_arrayChartItemData.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pItemData = m_arrayChartItemData.GetAt(nPos);
		pItemData->ClearData();
		pItemData->SetUseRateValue(bUse);
	}	
	m_bRateValue = bUse;
}*/

//2005. 06. 22 add by sy, nam 
//각 차트의 %, $, W의 구분값을 셋팅 한다. 
void CChartItem::SetChartNumericValue(int nNumericValue)
{	
	CChartItemData *pItemData = NULL;
	int nSize = m_arrayChartItemData.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pItemData = m_arrayChartItemData.GetAt(nPos);
		pItemData->ClearData();
		pItemData->SetChartNumericValue(nNumericValue);
		
	}	
	//m_nChartNumericValueType =nNumericValue; 05.12.01
	SetChart_NumericValueType(nNumericValue);
}

void CChartItem::SetBNormalizedMinuteTimeRule(BOOL bUse)
{
	CChartItemData *pItemData = NULL;
	int nSize = m_arrayChartItemData.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pItemData = m_arrayChartItemData.GetAt(nPos);
		pItemData->ClearData();		
	}	

	CDRChartOcx::SetBNormalizedMinuteTimeRule(bUse);
}

void CChartItem::HideScale(BOOL bHide)
{
	if(bHide)	
	{
		ChangeHorzScalePosition(CDRChartOcx::HORZ_HIDE, -1, -1);
		ChangeVertScalePosition(CDRChartOcx::VERT_NONE, -1, -1,-1);					
		//ShowInnerToolBar(FALSE);
	}
	else
	{
		ChangeHorzScalePosition(CDRChartOcx::HORZ_BOTTOM, GetRowBlockCount()-1, 0);
		ChangeVertScalePosition(CDRChartOcx::VERT_RIGHT, -1, -1,-1);					
		//ShowInnerToolBar(TRUE);
	}
}

void CChartItem::HideInnerToolBar(BOOL bHide)
{
	if(bHide)	
	{
		ShowInnerToolBar(FALSE);
	}
	else
	{
		ShowInnerToolBar(TRUE);
	}
}

void CChartItem::InvalidateType()
{
	switch(m_chTypeForRequest)
	{
		case TICK_DATA_CHART	:
		case MIN_DATA_CHART		:
		case HOUR_DATA_CHART	:
		case WEEK_DATA_CHART	:
		case MONTH_DATA_CHART	:
		case YEAR_DATA_CHART	:
		case SEC_DATA_CHART:
		{
			// remove market
			CChartItemData *pChartItemData = NULL;
			int nSize = m_arrayChartItemData.GetSize();
			for(int nPos=nSize-1;nPos>0;nPos--)
			{
				pChartItemData = m_arrayChartItemData.GetAt(nPos);
				CString strTRCode = pChartItemData->GetTrCode();
				CString strCode = pChartItemData->GetCode();
				if(strTRCode == QUERY_STRMARKET)
				{
					m_arrayChartItemData.RemoveAt(nPos);
					delete pChartItemData;
				}
			}
		}
		break;
	}
}


BOOL CChartItem::PtInRectJongmokTitle(POINT point) const
{
	return m_rectTitleJongmok.PtInRect(point);
}


//const UINT RMSG_FORMSTATECHANGE = ::RegisterWindowMessage(_T("RMSG_FORMSTATECHANGE"));
void CChartItem::ResetChartCfgWith(short p_nIndex)
{
	long lCnt = GetNOnePageDataCount();
	SetNew_OnePageDataCount(200);

	//전체보기 설정
	SetBWholeView(0);

	CDRChartOcx::ResetChartCfgWith(p_nIndex);
	SetNew_OnePageDataCount(lCnt);
}

long CChartItem::GetTempOnePageDataCount() 
{
	long lTemp = m_lNOnePageDataCount;
	m_lNOnePageDataCount = 0;
	return lTemp; 
}


CString CChartItem::GetQueryDataForGuideLine(const CUIntArray* pAryAvgForChartGuideLine)
{

	if(GetPrimaryChartItemData())
	{
		CString strQuoteCode = GetPrimaryChartItemData()->GetCode();
		if(strQuoteCode.GetLength()==CODE_STOCK_LENGTH)
		{
			CString strData;
			D10418_I st10418i;		
			memset(&st10418i,0x20,sizeof(st10418i));
			st10418i.chNull = NULL;
			strData.Format("%d",GetSafeHwnd());
			memcpy(st10418i.szBuffer,(LPCTSTR)strData,strData.GetLength());
			memcpy(st10418i.shcode,(LPCTSTR)strQuoteCode,sizeof(st10418i.shcode));
			st10418i.shcode[6] = ' ';
			strData.Format("%3d",pAryAvgForChartGuideLine->GetAt(0));
			memcpy(st10418i.nAvg1,(LPCTSTR)strData,sizeof(st10418i.nAvg1));
			strData.Format("%3d",pAryAvgForChartGuideLine->GetAt(1));
			memcpy(st10418i.nAvg2,(LPCTSTR)strData,sizeof(st10418i.nAvg2));
			strData.Format("%3d",pAryAvgForChartGuideLine->GetAt(2));
			memcpy(st10418i.nAvg3,(LPCTSTR)strData,sizeof(st10418i.nAvg3));		
			strData.Format("%3d",pAryAvgForChartGuideLine->GetAt(3));
			memcpy(st10418i.nAvg4,(LPCTSTR)strData,sizeof(st10418i.nAvg4));

			if(
			(atoi(st10418i.nAvg1) == 0) &&
			(atoi(st10418i.nAvg2) == 0) &&
			(atoi(st10418i.nAvg3) == 0) &&
			(atoi(st10418i.nAvg4) == 0) 			
			)
			return _T("");

			return (char*)&st10418i;
		}
	}
	return "";
}

void CChartItem::ShowLock(BOOL bShow)
{
}

BOOL CChartItem::SendMessageToInvoke(short nCommandType, long lData, long lSize)
{
	POSITION pos = m_listAddIn.GetHeadPosition();
	CString strAddIn;
	stMSGToInvoke stMSG;
	while(pos)
	{
		strAddIn = m_listAddIn.GetNext(pos);
		if(nCommandType==INVOKEMSG_INIT)
		{
			LoadAddIn(strAddIn);			
		}
		stMSG.m_hChartMng	= (HWND)m_plDllWnd;
		stMSG.m_pChartItem	= this;
		stMSG.m_pBuffer		= (void*)lData;
		stMSG.m_dwLength	= lSize;
		InvokeAddIn(strAddIn,nCommandType,(long)&stMSG);
	}
	return TRUE;
}

void CChartItem::Init_OverLapped()
{
	m_nCntOverLap	= 0;		// 중첩된 종목 Cnt;
	m_bOverLapping	= FALSE;	// 중첩 모드 Flag
}

void CChartItem::Add_OverLappedCnt()
{
	m_nCntOverLap++;		// 중첩된 종목 Cnt;
	m_bOverLapping	= TRUE;	// 중첩 모드 Flag
}

char CChartItem::GetTypeRequested()
{
	return m_chTypeForRequest;
}

void CChartItem::LoadTickMinDayInfo(CString stCfgFile)
{
	CString strAppName;
	CString strTemp;
	CString strTemp2;
	char szTemp[128] = {0,};

	strAppName.Format("%02d%02d",m_lColIndex,m_lRowIndex);	

	::GetPrivateProfileString(strAppName,"Type","2",szTemp,sizeof(szTemp),stCfgFile);
	//=====================================================================================
	//2005. 05. 23 년 차트가 들어감으로 수정한다. by sy, nam
	//if(szTemp[0]<'0' || szTemp[0]>'4') 
	//=====================================================================================
	if(szTemp[0]<TICK_DATA_CHART || szTemp[0]>SEC_DATA_CHART)
	{
		szTemp[0] = DAY_DATA_CHART;
	}

	SetChartRequestType(szTemp[0]);
	//=====================================================================================


	//=====================================================================================
	// 차트의 틱분일 별 해당 Value를 읽어서 m_strTickMinDayForRequest 멤버에 저장한다.
	//=====================================================================================
	//::GetPrivateProfileString(strAppName,"TickMinNum","01",szTemp,sizeof(szTemp),lpTitle);	
	memset(szTemp, 0x00, sizeof(szTemp));
	::GetPrivateProfileString(strAppName,"TickValue",	"001",	szTemp,sizeof(szTemp),stCfgFile);
	SetTickNumber(CString(szTemp));
	
	::GetPrivateProfileString(strAppName,"MinValue",	"001",	szTemp,sizeof(szTemp),stCfgFile);
	SetMinNumber(CString(szTemp));
	
	::GetPrivateProfileString(strAppName,"DayValue",	"001",	szTemp,sizeof(szTemp),stCfgFile);
	SetDayNumber(CString(szTemp));
	
	::GetPrivateProfileString(strAppName,"HourValue",	"001",	szTemp,sizeof(szTemp),stCfgFile);
	SetHourNumber(CString(szTemp));
	
	switch(m_chTypeForRequest)
	{
		case TICK_DATA_CHART:	m_strTickMinDayForRequest = GetTickNumber();	break;
		case MIN_DATA_CHART	:	m_strTickMinDayForRequest = GetMinNumber ();	break;
		case DAY_DATA_CHART	:	m_strTickMinDayForRequest = GetDayNumber ();	break;
		case HOUR_DATA_CHART:	m_strTickMinDayForRequest = GetHourNumber();	break;
		default				:	m_strTickMinDayForRequest = "001"		   ;	break;
	}	


	m_strTickMinDayForRequest = szTemp;
	if(m_strTickMinDayForRequest.IsEmpty())
	{
		m_strTickMinDayForRequest = "001";
	}
}

CString CChartItem::GetTickMinDayNumber(int nChartType)
{
	//===========================================================================
	// 특정 차트 상태의 값을 사용자가 넘겨 해당 설정 값을 얻어 가려고 할때 
	// nChartType에 -1이상의 값을 넘긴다.
	// 05. 05. 25 add by sy, nam 
	//===========================================================================
	if(nChartType > -1)
	{
		if(nChartType == 0)
			return GetTickNumber();
		else if(nChartType == 1)
			return GetMinNumber();
		else if(nChartType == 6)
			return GetSecNumber();
		else if(nChartType == 2)
			return GetDayNumber();
		else if (nChartType == 7)
			return GetHourNumber();
		else
			return "001";
	}
	//===========================================================================


	switch(m_chTypeForRequest)
	{
		case TICK_DATA_CHART:						
			return GetTickNumber();
			break;
		case SEC_DATA_CHART:			
			return GetSecNumber();
			break;
		case MIN_DATA_CHART:			
			return GetMinNumber();
			break;
		case DAY_DATA_CHART:			
			return GetDayNumber();
			break;
		case HOUR_DATA_CHART:
			return GetHourNumber();
		default:
			return "001";
			break;
	}
	
	return "001";
}

CString CChartItem::GetTickNumber() 
{
	return m_strTick;
}

CString CChartItem::GetSecNumber()	
{
	return m_strSec;
}

CString CChartItem::GetMinNumber()	
{
	return m_strMin;
}

CString CChartItem::GetDayNumber()
{
	return m_strDay;
}

CString CChartItem::GetHourNumber()
{
	return m_strHour;
}

void	CChartItem::SetTickNumber	(CString strValue)
{
	if( atoi(strValue) <= 0) 
	{
		strValue = "001";
	}
	m_strTickMinDay = m_strTick	= strValue;
}

void	CChartItem::SetSecNumber	(CString strValue)
{
	if( atoi(strValue) <= 0)
	{
		strValue = "005";
	}
	m_strTickMinDay = m_strSec	= strValue;
}

void	CChartItem::SetMinNumber	(CString strValue)
{
	if( atoi(strValue) <= 0)
	{
		strValue = "001";
	}
	m_strTickMinDay = m_strMin	= strValue;
}

void	CChartItem::SetDayNumber	(CString strValue)
{
	if( atoi(strValue) <= 0)
		strValue = "001";
	m_strTickMinDay = m_strDay	= strValue;
}

void	CChartItem::SetHourNumber	(CString strValue)
{
	if( atoi(strValue) <= 0)
		strValue = "001";
	m_strTickMinDay = m_strHour	= strValue;
}

// gm0604 added to Update System Trading Data (2005.7.21)
BOOL CChartItem::ReceiveFromSTDataManager()
{
	if(m_lPtrST&&m_bBuild)
	{
		InvokeAddIn("ChartStrategyAddin.dll",1,m_lPtrST);
	}
	return TRUE;	
}

void CChartItem::SetPtrST(long lPtr, LPCTSTR lpSTName)
{
	m_lPtrST = lPtr;
	m_strName = lpSTName;
}

// gm0604 added to clear System Trading Info 2005.10.10
void CChartItem::ClearPtrST()
{
#ifdef _WRITELOG
	WriteToStringLog("CChartItem::ClearPtrST::Begin", _T("멀티차트 종료"), 13);
#endif

	if(m_lPtrST)
	{
		CStringArray arrayGraph;
		InvokeAddIn("ChartStrategyAddin.dll",8,(long)&arrayGraph);
		int nSize = arrayGraph.GetSize();
		for(int i=0;i<nSize;i++)
		{
			RemoveChart(arrayGraph.GetAt(i));
		}
	}
	m_lPtrST = NULL;// gm0604 added 2005.9.6

#ifdef _WRITELOG
	WriteToStringLog("CChartItem::ClearPtrST::End", _T("멀티차트 종료"), 13);
#endif
}
// gm0604 added to Update System Trading Data (2005.7.21)


void CChartItem::SetChartRequestType(char chType)
{
	m_chTypeForRequest = chType;
}


CString CChartItem::GetReplayFlag()
{
	// 복기차트 사용여부 : 복수종목 - ojtaso (20070530)
	int nResult = GetParent()->GetParent()->SendMessage(UMSG_GET_CHARTMODE, 1);
	if(nResult == REPLAY_ITEM)
	{
		return _T("1");
	}
	else 
		return _T("0");
}

CString CChartItem::GetReplayStartTime()
{
	CString strDate;
	int nDate = GetParent()->GetParent()->SendMessage(UMSG_GET_REPLAY_STARTTIME);
	strDate.Format("%04d00", nDate);
	return strDate;	
}

//05.10.14
CString CChartItem::GetReplayDayBefore()
{
	CString strDayBefore;
	int nDay = GetParent()->GetParent()->SendMessage(UMSG_GET_REPLAY_DAYBEFORE);
	strDayBefore.Format("%02d", nDay);
	return strDayBefore;
}

void CChartItem::Init_ChartCfgInfo()
{
	m_CfgInfo.bAutoSave			= true	;
	m_CfgInfo.bRealCandle		= true	;
	//SetNew_BNormalizedMinuteTimeRule(FALSE); //실봉이 기본이다.
	m_CfgInfo.bRecvTimeShare	= false	;
	m_CfgInfo.bShowHLBar		= true	;	
	m_CfgInfo.bShowRight		= true	;
	m_CfgInfo.bShowLog			= false ; //05. 08. 25
	m_CfgInfo.bStartDate		= false	; //05. 08. 25
}

void CChartItem::SetChartConfigInfo(CHART_CONFIG_INFO &ChartCfgInfo)
{
	m_CfgInfo.bAutoSave			= ChartCfgInfo.bAutoSave		;
	//=========================================================================	
	m_CfgInfo.bRealCandle		= ChartCfgInfo.bRealCandle		; 
	//05.11.25
	//if(ChartCfgInfo.bRealCandle == true)  //실봉이면 
	//	SetNew_BNormalizedMinuteTimeRule(FALSE); 
	//else
	//	SetNew_BNormalizedMinuteTimeRule(TRUE); 
	//=========================================================================

	m_CfgInfo.bRecvTimeShare	= ChartCfgInfo.bRecvTimeShare	;
	m_CfgInfo.bShowHLBar		= ChartCfgInfo.bShowHLBar		;
	m_CfgInfo.bShowRight		= ChartCfgInfo.bShowRight		;	
	m_CfgInfo.bShowLog			= ChartCfgInfo.bShowLog			; //05. 08. 25
	m_CfgInfo.bStartDate		= ChartCfgInfo.bStartDate		; //05. 08. 25

}

void CChartItem::InitShareManager()
{
}



int	CChartItem::GetSignalImageIndex_FromSignalDegree(int nType, int nSignalDegree)
{
	// Image\\신호등기호_16.bmp 이미지의 Index 
	int nRet_ImageIndex = 0;
	if(nType == e_SAS)
	{
		switch(nSignalDegree) 
		{
		case 3:
			nRet_ImageIndex = 4;
			break;
		case 2:
			nRet_ImageIndex = 5;
			break;
		case 1:
			nRet_ImageIndex = 6;
			break;
		case 0:
			nRet_ImageIndex = 7;
			break;
		case -1:
			nRet_ImageIndex = 8;
			break;
		case -2:
			nRet_ImageIndex = 9;
			break;
		case -3:
			nRet_ImageIndex = 10;
			break;
		default:
			nRet_ImageIndex = 7;
			break;
		}
		
		return nRet_ImageIndex;
	}
	else if(nType == e_NKS)
	{
		if( nSignalDegree ==0 )
			nRet_ImageIndex = 2;
		else if( nSignalDegree > 0 )
			nRet_ImageIndex = 1;
		else if( nSignalDegree < 0 )
			nRet_ImageIndex = 3;


		return nRet_ImageIndex;
	
	}
	else if(nType == e_UAS)
	{
		switch(nSignalDegree) 
		{
		case 3:
			nRet_ImageIndex = 11;
			break;
		case 2:
			nRet_ImageIndex = 12;
			break;
		case 1:
			nRet_ImageIndex = 13;
			break;
		case 0:
			nRet_ImageIndex = 14;
			break;
		case -1:
			nRet_ImageIndex = 15;
			break;
		case -2:
			nRet_ImageIndex = 16;
			break;
		case -3:
			nRet_ImageIndex = 17;
			break;
		default:
			nRet_ImageIndex = 14;
			break;
		}
	
		return nRet_ImageIndex;
	}

	return nRet_ImageIndex;
}

void CChartItem::SetSignalData(CString sShareName, BYTE* pData, int nLen)
{
}

void CChartItem::SetAutoSaveToOutside(const bool bAutoSave)
{
	m_CfgInfo.bAutoSave = bAutoSave;
}

BOOL CChartItem::GetUseRevisedValue()
{
	m_bRevisedPrice = m_CfgInfo.bRevisedValue;
	return m_bRevisedPrice;
}


void CChartItem::SetUseRevisedValue(BOOL bUse)
{
	//{{ 수정주가 클릭시 전체수 고정시키기 위해 추가 by LYH 2007.01.11
	if(m_bRevisedPrice != bUse)
	{
		if(m_lAllCnt>m_lCntForRequest)
		{
			if(m_lAllCnt>CHTMXREQUEST)
			{
				m_nTotCountForRevised = CHTMXREQUEST;
			}
			else
			{
				m_nTotCountForRevised = m_lAllCnt;
			}				
		}	
	}
	//}}
	ClearData();
	m_bRevisedPrice = bUse;
	m_CfgInfo.bRevisedValue = bUse ? true : false;
}

void CChartItem::SetEndDate(LPCTSTR endDate)  
{
	m_strEndDate = endDate;
	if(m_strEndDate.GetLength()!=8)
	{
		CTime t = CTime::GetCurrentTime();
		m_strEndDate = t.Format("%Y%m%d");	
	}
}


void CChartItem::SetOldChartCode(CString strCode)
{
	m_strOldCode = strCode;
}


void CChartItem::OpenSignalMapScr()
{	
}

BOOL CChartItem::AddInEvent_FromChart(LPCSTR _szParam)
{
	//20100308_JS.Kim 솔로몬 aaaaaaaaaaaaaa
	CString strParam;
	char *szParam = strParam.GetBufferSetLength( strlen( _szParam));
	strcpy( szParam, _szParam);

	CString strData;
	strData.Format("%s", szParam);
	if(strData.GetLength() < 1)
	{
		return FALSE;
	}
	
	CString strName;
	int nFind = strData.Find(",");
	strName = strData.Left(nFind);
	strData = strData.Mid(nFind + 1);

	if (strName == "ChartCommonAddIn.dll:ORDER_LINK")
	{
		CString strParam1, strParam2, strPrice, strParam4, strParam5;

		nFind = strData.Find(",");
		strParam1 = strData.Left(nFind);
		strData = strData.Mid(nFind + 1);
		
		nFind = strData.Find(",");
		strParam2 = strData.Left(nFind);
		strData = strData.Mid(nFind + 1);
		
		nFind = strData.Find(",");
		strPrice = strData.Left(nFind);
		strData = strData.Mid(nFind + 1);
		
		nFind = strData.Find(",");
		strParam4 = strData.Left(nFind);
		strData = strData.Mid(nFind + 1);
		
		strParam5 = strData;

		CString strName;
		strName = m_strSelectedItemCode;

		char *pData = new char[100];
		memset(pData, 0x00, 100);
		sprintf(pData, "%s,%s,%s", m_strSelectedItemCode.Right(7), strPrice, m_strOrderType);
		::SendMessage(((CStdDialog*)m_pStdDialog)->m_hViewInMain, RMSG_GET_LINKINFODATA, 1, (LPARAM)pData);
		delete []pData;
	}
	else if (strName == "ChartPeriodSummaryAddIn.dll:ORDER_LINE")
	{
		CString strParam1, strParam2, strParam3, strParam4, strParam5;

		// 
		nFind = strData.Find(",");
		strParam1 = strData.Left(nFind);
		strData = strData.Mid(nFind + 1);
		
		// 종목코드
		nFind = strData.Find(",");
		strParam2 = strData.Left(nFind);
		//strParam2 = CStdDialog::GetRealItemCode(strParam2);
		strData = strData.Mid(nFind + 1);
		
		// 가격
		nFind = strData.Find(",");
		strParam3 = strData.Left(nFind);
		strData = strData.Mid(nFind + 1);
		
		// 1/*0:매도, 1:매수*/
		nFind = strData.Find(",");
		strParam4 = strData.Left(nFind);
		strData = strData.Mid(nFind + 1);
		
		strParam5 = strData;

		CString strName;
		strName = CStdDialog::GetRealItemCode(m_strSelectedItemCode);

/*
		strParam2 = strParam2.Right(7);
		char *pData = new char[100];
		memset(pData, 0x00, 100);
		sprintf(pData, "%s,%s,%s", strParam2, strParam3, strParam5);
		::SendMessage(((CStdDialog*)m_pStdDialog)->m_hViewInMain, RMSG_GET_LINKINFODATA, 1, (LPARAM)pData);
		delete []pData;
*/
		//dPirce, Qty, nOrderType, szOrgOrderID,
		((CStdDialog*)m_pStdDialog)->OnOrderInfomationChanged(atof(strParam3), atoi(strParam1), 1/*ORDERTYPE_ENTRY*/, "", "", "", "");
	}
	//>>
	return TRUE;
}


void CChartItem::SetRowNCol(long lRowIndex, long lColIndex)
{ 
	if(lRowIndex < -1)
		OutputDebugString("aa");

	m_lColIndex = lColIndex;
	m_lRowIndex = lRowIndex;
}

BOOL CChartItem::SetPacketData(LPCTSTR szPacketName, long szData, long nLength, BOOL bReset)
{
	if(m_lPtrST)
	{
		CPtrArray ptrArray;
		ptrArray.Add((void*)m_lPtrST);
		CString strType;
		char chType = GetType();
		if(chType==DAY_DATA_CHART)
		{
			strType = "일";
		}
		else if(chType==MIN_DATA_CHART)
		{
			strType = "분";
		}
		else if(chType==TICK_DATA_CHART)
		{
			strType = "틱";
		}
		else if(chType==HOUR_DATA_CHART)
		{
			strType = "시";
		}
		else
		{
			ASSERT(0);
		}			
		ptrArray.Add((void*)(LPCTSTR)strType);
		BOOL bRetValue = InvokeAddIn("ChartStrategyAddin.dll",6,(long)&ptrArray);
		if(bRetValue)
		{
			szData = (long)ptrArray.GetAt(0);
			nLength = (long)ptrArray.GetAt(1);
		}		
		bRetValue = CDRChartOcx::SetPacketData(szPacketName,szData,nLength,bReset);
		bRetValue = InvokeAddIn("ChartStrategyAddin.dll",7,szData);
		return bRetValue;
	}
	else
	{
		// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)
		if(((CChartMng*)GetParent())->GetAddModeFromTab())
			CDRChartOcx::SetResetTotalCountWhileDraw(FALSE);

		return CDRChartOcx::SetPacketData(szPacketName,szData,nLength,bReset);
	}
	return TRUE;
	
}





void CChartItem::RemoveAllMarketData()
{
	CChartItemData *pChartItemData = NULL;
	for(int nPos=0;nPos<m_arrayMarketTRData.GetSize();nPos++)
	{
		pChartItemData = NULL;
		pChartItemData = m_arrayMarketTRData.GetAt(nPos);										
		if(pChartItemData)
		{
			CString strTRCode = pChartItemData->GetTrCode();
			CString strCode = pChartItemData->GetCode();
			
			delete pChartItemData;
			pChartItemData = NULL;
		}
	}

	m_arrayMarketTRData.RemoveAll();
}


void CChartItem::SetNew_OnePageDataCount(int nPCnt)
{
	//SetNOnePageDataCount(nPCnt); <== 종료시에 속성값으로만 페이지 카운트를 조절하기 사용한다.
	CDRChartOcx::SetOnePageDataCount(nPCnt); //<== 실제 페이지 카운트를 조정한다.
#ifdef _DEBUG
	CString sTemp;
	sTemp.Format("SetNew_OnePageDataCount : %d", nPCnt);
	OutputDebugString(sTemp);
#endif
}


CChartItemData* CChartItem::GetChartItemMarketDataNew(CString strTRCode,LPCTSTR lpCodeForQuery)
{
	CString strCodeForQuery;
	CChartItemData *pItemData = NULL;
	if(lpCodeForQuery!=NULL)
	{
		strCodeForQuery = lpCodeForQuery;
	}	
	if(strCodeForQuery.IsEmpty()) return NULL;

	int nSize = m_arrayMarketTRData.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pItemData = m_arrayMarketTRData.GetAt(nPos);
		if(pItemData->GetTrCode()==strTRCode
			&&pItemData->GetCodeForRqstFromChartItemData()==strCodeForQuery)
		{
			delete pItemData;
			pItemData = NULL;
			m_arrayMarketTRData.RemoveAt(nPos);
			nSize--;
		}
	}

	
	pItemData = new CChartItemData(this,strTRCode);
	pItemData->SetCodeForQuery(strCodeForQuery); //요청할 종목코드(시장 지표명) Set	
	m_arrayMarketTRData.Add(pItemData);
#ifdef _DEBUG
		CString sTemp;
		sTemp.Format("GetChartItemDataNew::m_arrayMarketTRData 갯수 %d", m_arrayMarketTRData.GetSize());
		OutputDebugString(sTemp);
#endif 
	
	return pItemData;
}


CString CChartItem::GetMarketQueryData(CString strJipyo, int nCntRqst, BOOL bIsNew)
{
	if(	m_strDateForRequest.IsEmpty()	// 검색시작일
		||GetCntForRequest()==NULL		// 검색할 데이터 갯수
		||m_chTypeForRequest==NULL		// 틱(0)/분(1)/일(2)/주(3)/월(4)
		||strJipyo.GetLength() == 0		
		) 
	{
		return "";
	}

	for(int i = 0; i < m_arrayMarketTRData.GetSize(); i++)
	{
		CChartItemData *pChartItemData = m_arrayMarketTRData.GetAt(i);
		if(pChartItemData && pChartItemData->GetCodeForRqstFromChartItemData() == strJipyo)
		{
			pChartItemData->SetBNextMarketData(bIsNew ? FALSE : TRUE); //bIsNew가 TRUE면 신규조회 이므로 인자는 False를 넘긴다.
			return pChartItemData->GetMarketQueryData_ChartItemData(bIsNew ? FALSE : TRUE, nCntRqst);
		}
	}
	return "";
}


long CChartItem::ReceiveMarketChartData(void* szData, long nLength)
{	
	CString strJMCode;
	CChartItemData *pChartItemData = NULL;
	
	CString strData;
	strData.Format("%*.*s", nLength, nLength, szData);
	m_strArrRecvMarketTRData.Add(strData);
	
	CString strPacketName, strCode;	
	for(int i = 0 ; i < m_arrayMarketTRData.GetSize() ; i++)
	{
		pChartItemData = m_arrayMarketTRData.GetAt(i);
		
		for(int j = 0; j < m_strArrRecvMarketTRData.GetSize(); j++)
		{			
			CString strRecvData = m_strArrRecvMarketTRData.GetAt(j);
			if(!strRecvData.GetLength()) continue;

			if( (pChartItemData->GetTrCode() == QUERY_STRMARKET) )
			{
				EU_p0607_OutRec1* pEu0607OutRec1 = (EU_p0607_OutRec1*)strRecvData.GetBuffer(strRecvData.GetLength());
				strPacketName.Format("%100.100s", pEu0607OutRec1->dataNm);  strPacketName.TrimRight();
				strCode.Format("%8.8s"			, pEu0607OutRec1->shcode);	 strCode.TrimRight();
			}

			if(pChartItemData->GetCodeForRqstFromChartItemData() != strPacketName)
				continue;	

			m_strArrRecvMarketTRData.RemoveAt(j);
		
			if(!pChartItemData)
			{
		#ifdef _DEBUG
				CString sTemp;
				sTemp.Format("%d", pChartItemData);
				AfxMessageBox("CChartItem::ReceiveMarketChartData \n pChartItemData pointer is NULL\n" + sTemp);
		#endif
				return 0;
			}

			BOOL bRetValue = pChartItemData->MakeChartData(szData,nLength);
			if(!bRetValue)
			{
				return 0;
			}

			CString strTrCode = _T("");
			strTrCode = pChartItemData->GetTrCode();
			
			if(bRetValue)
			{
				if(!BuildMaketChart(pChartItemData)) return 0;		
				return 1; 			
			}
		}		
	}



	m_strArrRecvMarketTRData.RemoveAll();	
	return 0;
}


///////////////////////////////////////////////////////////////////////////////
//	작업자	:	
//	E-Mail	:	
//	일자	:	2005.11.12
//	목적	:	받은 데이터로 시장 지표 차트를 구성한다.
//////////////////////////////////////////////////////////////////////////////
BOOL CChartItem::BuildMaketChart(CChartItemData* pChartItemData)
{
	m_bBuild = TRUE;
	long lSize = 0;	
	
	// 2005. 05. 18 add by sy, nam ===========================================
	// 중첩일 경우 몇개의 종목이 중첩되는지 그 총 수를 저장한다.
	if(m_bOverLapping == FALSE)
		m_nCntOverLap = 0;

	pChartItemData->m_nCntOverLap = m_nCntOverLap;
	//========================================================================
	if(m_nSmallChartMode==1)
	{
		pChartItemData->AddPacketAndGraph(TRUE);
	}
	else
	{
		pChartItemData->AddPacketAndGraph();
	}
	
	m_bInitalChart = TRUE;
	
	BOOL bNextOld = m_bNext;	
	BOOL bRetValueOfSetPacketData = pChartItemData->SetPacketDataInAdditionPacket(!m_bNext);
	  
	
	if(!bRetValueOfSetPacketData) return FALSE;
	
	Invalidate();	
	
	return TRUE;
}

void CChartItem::SetBReplayChartMode(BOOL bReplayMode)
{ 
	m_bReplaychartMode = bReplayMode;	

	if( bReplayMode)
	{
		m_strReplayTime = _T("복기모드 상태입니다                         ");		
	}
	else
	{
		m_strReplayTime = _T("");
	}	
}

void CChartItem::SetReplayChart_RealData(CString strCnts, CString strPrice, CString strVolume)
{
	m_strReplayTime			= strCnts;
	m_strCurPrcForReplay	= strPrice;		m_strCurPrcForReplay.TrimLeft(); m_strCurPrcForReplay.TrimRight();
	
	strVolume.Remove('-'); strVolume.TrimLeft(); strVolume.TrimRight();
	m_strVolumeForReplay.Format("%d", atoi(m_strVolumeForReplay) + atoi(strVolume));

	m_strChrateForReplay.Format("%.2f", atof(m_strCurPrcForReplay) / atof(m_strPrePrcForReplay) * 100  - 100.00);

	m_strReplayTime = strCnts;
	
	OutputDebugString(strCnts);

	CRect rectTemp;
	if(!m_rectTitleJongmokPast.IsRectEmpty())
	{
		if(rectTemp.SubtractRect(m_rectTitleJongmokPast,m_rectTitleJongmok))
		{
			GetParent()->InvalidateRect(rectTemp);
		}		
		m_rectTitleJongmokPast.SetRectEmpty();
	}
	GetParent()->InvalidateRect(m_rectTitleJongmok);

	if(!m_rectTitleTypePast.IsRectEmpty())
	{
		if(rectTemp.SubtractRect(m_rectTitleTypePast,m_rectTitleType))
		{
			GetParent()->InvalidateRect(rectTemp);
		}		
		m_rectTitleTypePast.SetRectEmpty();
	}
	GetParent()->InvalidateRect(m_rectTitleType);
	
	if(!m_rectTitlePosPast.IsRectEmpty())
	{
		if(rectTemp.SubtractRect(m_rectTitlePosPast,m_rectTitlePos))
		{
			GetParent()->InvalidateRect(rectTemp);
		}
		m_rectTitlePosPast.SetRectEmpty();
	}

	GetParent()->Invalidate();
}

const char* CChartItem::GetNextKey(BOOL bMarket)
{
	if( m_arrayChartItemData.GetSize() )
	{
		CChartItemData* pItemData = NULL;
		pItemData = m_arrayChartItemData.GetAt(0); //첫번째가 차트 기본 데이터
		if(bMarket)
			return pItemData->GetNextKeyOld();
		else
			return pItemData->GetNextKey();
	}
	return NULL;	
}


void CChartItem::DrawReplayChartTitle(CRect rectIn,CDC* pDC,CFont *pFt, CFont *pFtNormal, COLORREF crBackIn )
{
	CRect rect = rectIn;

// (2008/9/13 - Seung-Won, Bae) Remove Init Button.
	if(m_btInit.GetSafeHwnd())
	{
		rect.right -= WIDTHOFTITLEBUTTON;
	}
	if(m_btMaxmize.GetSafeHwnd())
	{
		rect.right -= POSXOFMAXMIZE;
	}	

	COLORREF crText = NULL;
	COLORREF crTextSise = NULL;
	COLORREF crBackSise = NULL;
	
	crText = CLR_FONT_DEFAULT;
	crTextSise = CLR_FONT_DEFAULT;
	
	
	CFont *pFtDefault = (CFont *)pDC->SelectObject(pFt);

	CRect rectTitlePos;
	CRect rectTitleType1;	
	CRect rectTitleType2;
	CRect rectTitleName;

	CString strTitlePos;	// [1/1]
	CString strTitleName;	// 종목명


	CString strTitleType;

	long lCols = ((CChartMng*)GetParent())->GetCols();
	long lCntOfChart = ((CChartMng*)GetParent())->GetCntOfChart();
	strTitlePos.Format("[%d / %d]", m_lColIndex + 1 + m_lRowIndex*lCols,lCntOfChart);
	CSize szTitlePos = pDC->GetOutputTextExtent(strTitlePos);
	rectTitlePos.SetRect(rect.right-szTitlePos.cx,rect.top+4,rect.right,rect.bottom);

	BOOL bShortTitleMode = FALSE;
	if(!m_strReplayTime.IsEmpty())
	{		
		m_strReplayTime.TrimRight();
		strTitleName.Format(" %s  %s (%s%%) %s   ",m_strReplayTime, ::SetComma(m_strCurPrcForReplay), m_strChrateForReplay, ::SetComma(m_strVolumeForReplay));		

		CSize szTitleName = pDC->GetOutputTextExtent(strTitleName);
		
		
		CRect rectShift;
		CRect rectUnion;
		CRect rectUnionTitle, rectTitleJongmok;
		BOOL  bSetEmpty = FALSE;
		
		rectUnionTitle.SetRectEmpty();
		rectShift = rect;
		rectShift.top += 4;
		rectShift.OffsetRect(1,0);

		rectTitleName.SetRect(rectShift.left,rectShift.top,rectShift.left+szTitleName.cx,rectShift.top + szTitleName.cy);
		rectUnionTitle.UnionRect(rectUnionTitle,rectTitleName);						
		
		rectTitleJongmok = rectUnionTitle;
		rectTitleJongmok.InflateRect(2,3);
		if(rectTitleJongmok!=m_rectTitleJongmok)
		{
			m_rectTitleJongmokPast = m_rectTitleJongmok;
			m_rectTitleJongmok = rectTitleJongmok;		
		}
		
	}
	
	char chType = NULL;
	if(m_chType == NULL)
		if(m_chTypeForRequest == NULL)		
			chType = DAY_DATA_CHART;
		else
			chType = m_chTypeForRequest;
	else
		chType = m_chType;

	if(chType!=NULL)
	{		
		// 틱(0)/분(1)/일(2)/주(3)/월(4)
		switch(chType)
		{
		case TICK_DATA_CHART:
			strTitleType.Format("[%s틱]",(LPTSTR)(LPCTSTR)m_strTick);
			break;
		case MIN_DATA_CHART:
			if("300" == m_strMin)
				strTitleType.Format("[30초]");
			else
				strTitleType.Format("[%s분]",(LPTSTR)(LPCTSTR)m_strMin);
			break;
		case DAY_DATA_CHART:
			strTitleType.Format("[%s일]",(LPTSTR)(LPCTSTR)m_strDay);
			break;
		case WEEK_DATA_CHART:
			strTitleType.Format("[주]");
			break;
		case MONTH_DATA_CHART:
			strTitleType.Format("[월]");
			break;
		case YEAR_DATA_CHART:
			strTitleType.Format("[년]");
			break;
		case SEC_DATA_CHART:
			strTitleType.Format("[초]");
			break;

		default:
			strTitleType = _T("[]");
			break;
		}

		if(strTitleType.GetLength())
		{
			CSize szTitleType = pDC->GetOutputTextExtent(strTitleType);
			rectTitleType1.SetRect(rect.right-szTitleType.cx-szTitlePos.cx,rect.top+4,rect.right-szTitlePos.cx,rect.bottom);
			rectTitleType2.SetRect(rect.right-szTitleType.cx,rect.top+4,rect.right,rect.bottom);
		}
	}	

	if(!crBackSise)
	{
		pDC->SetBkMode(TRANSPARENT);		
	}	
	else
	{
		pDC->SetBkMode(OPAQUE);				
		pDC->SetBkColor(crBackSise);
	}
	pDC->SetTextColor(crTextSise);


	if(!m_strReplayTime.IsEmpty())
	{
		CRect rectInflate;
		if(!crBackSise)
		{
			rectInflate = rectTitleName;
			rectInflate.InflateRect(1,3);
			pDC->DrawEdge(rectInflate,EDGE_ETCHED   ,BF_RECT);
		}
		else
		{
			rectInflate = rectTitleName;
			rectInflate.InflateRect(0,1);
			pDC->FillSolidRect(rectInflate,crBackSise);
		}
		pDC->TextOut(rectTitleName.left,rectTitleName.top, strTitleName);
	}

	CRect rectInter11;
	CRect rectInter12;
	CRect rectInter2;
	rectInter11.IntersectRect(m_rectTitleJongmok,rectTitleType1);
	rectInter12.IntersectRect(m_rectTitleJongmok,rectTitleType2);
	rectInter2.IntersectRect(m_rectTitleJongmok,rectTitlePos);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(crText);
	if(rectInter11.IsRectEmpty())
	{
		pDC->TextOut(rectTitleType1.left,rectTitleType1.top,strTitleType);
		if(rectTitleType1!=m_rectTitleType)
		{
			m_rectTitleTypePast = m_rectTitleType;
			m_rectTitleType = rectTitleType1;
		}
		if(rectInter2.IsRectEmpty())
		{
			pDC->TextOut(rectTitlePos.left,rectTitlePos.top,strTitlePos);			
			if(rectTitlePos!=m_rectTitlePos)
			{
				m_rectTitlePosPast = m_rectTitlePos;
				m_rectTitlePos = rectTitlePos;
			}
			
		}	
	}
	else if(rectInter12.IsRectEmpty())
	{
		pDC->TextOut(rectTitleType2.left,rectTitleType2.top,strTitleType);
		if(rectTitleType2!=m_rectTitleType)
		{
			m_rectTitleTypePast = m_rectTitleType;
			m_rectTitleType = rectTitleType2;
		}		
	}
	else
	{
		if(rectInter2.IsRectEmpty())
		{
			pDC->TextOut(rectTitlePos.left,rectTitlePos.top,strTitlePos);			
			if(rectTitlePos!=m_rectTitlePos)
			{
				m_rectTitlePosPast = m_rectTitlePos;
				m_rectTitlePos = rectTitlePos;
			}
		}	
	}
	pDC->SelectObject(pFtDefault);
}


void CChartItem::SetNew_BNormalizedMinuteTimeRule(BOOL bFakeCandle)
{
	m_CfgInfo.bRealCandle = bFakeCandle ? false : true; //bFakeCandle이 TRUE면 허봉,  FALSE면 실봉
														//단 m_CfgInfo.bRealCandle는 실봉이면 TRUE이므로	
														//bFakeCandle과 반대의 값을 셋한다.
	CDRChartOcx::SetBNormalizedMinuteTimeRule(bFakeCandle);
}

void CChartItem::InitChartItem_FromExtToolBar()
{
	m_bShowLock = TRUE;
}

void CChartItem::SetChart_NumericValueType(int nValue)
{
	m_nChartNumericValueType = nValue;

	//기준선 Addin에게 chart type을 알린다.
	InvokeAddIn("FX_ChartGuideLineAddin.dll", 104, nValue);	
}

void CChartItem::SetSystemTradingIndexByTab(CStringArray *pstrArraySTName)
{
	int nSize = pstrArraySTName->GetSize();
	m_strArrayNameBySystemTradingTab.RemoveAll();
	m_strArrayNameBySystemTradingTab.Append(*pstrArraySTName);	
	BOOL bRetValue = InvokeAddInSTIndex("ChartSTIndexAddIn.dll",1,(long)pstrArraySTName);
	if(!bRetValue)
	{
		m_strArrayNameBySystemTradingTab.RemoveAll();
	}
}

void CChartItem::UpdateSystemTradingIndexByTab()
{
	if(m_strArrayNameBySystemTradingTab.GetSize())
	{
		InvokeAddInSTIndex("ChartSTIndexAddIn.dll",1,(long)&m_strArrayNameBySystemTradingTab);
	}	
}

void CChartItem::SetReplay_CurDate(CString  strBojoMsg)
{
	strBojoMsg.TrimRight();
	int nFind = strBojoMsg.Find("=", 0);
	strBojoMsg = strBojoMsg.Mid(nFind + 1, 8);

	m_strCurReplayDate.Format("%s-%s-%s",strBojoMsg.Left(4), strBojoMsg.Mid(4, 2), strBojoMsg.Right(2));
	
}

void CChartItem::InitString_ForReplay()
{
	m_strChrateForReplay = _T(""); //복기 전일대비 상승률
	m_strSignForReplay	 = _T(""); //복기 상하 신호.

	m_strCurPrcForReplay = _T("");
	m_strVolumeForReplay = _T("0");
}

void CChartItem::SetReplay_PrePrice(CString  strBojoMsg)
{
	CString strFilter = "PREVPRICE=종가:";

	strBojoMsg.TrimRight();

	//@->|로 변경
	int nFind	= strBojoMsg.Find(strFilter, 0);
	if(nFind<0)
		return;

	int nFind2	= strBojoMsg.Find("|", nFind);

	m_strPrePrcForReplay = strBojoMsg.Mid(	nFind + strFilter.GetLength(),
											nFind2 - nFind - strFilter.GetLength() );

	m_strPrePrcForReplay.TrimRight();
	m_strPrePrcForReplay.TrimLeft();
}


void CChartItem::PrintChart()
{
	CDRChartOcx::PrintChatImage();
}

void CChartItem::SetExcelData()
{
	CDRChartOcx::ExportToExcel();
}

LPCSTR CChartItem::CXIBaseDesk::GetBasePath()
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	//<<20100308_JS.Kim 솔로몬
	//m_szPath = (LPCSTR)AfxGetPctrData(10, 2);		// nID=10, ROOT 디렉토리 구하기. 참고문서(정리_PC데이터매니저.xls)
	m_szPath = (LPCSTR)AfxGetPctrData(10);		// nID=10, ROOT 디렉토리 구하기. 참고문서(정리_PC데이터매니저.xls)
	//>>
	return m_szPath;
}

HWND  CChartItem::CXIBaseDesk::GetDeskHwnd()
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)
	
	return pThis->m_pBaseDesk2->GetDeskHwnd();
}

LPDISPATCH CChartItem::CXIBaseDesk::GetUnknown()
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	return pThis->m_pBaseDesk2->GetUnknown();
}

LPUNKNOWN CChartItem::CXIBaseDesk::GetCtrlUnknown(LPCTSTR szName)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	return pThis->m_pBaseDesk2->GetCtrlUnknown(szName);
}

HWND CChartItem::CXIBaseDesk::GetCtrlHwnd(LPCTSTR szName)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	return pThis->m_pBaseDesk2->GetCtrlHwnd(szName);
}

LPVOID CChartItem::CXIBaseDesk::GetSharedInterface()
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)
	return NULL;
}

BOOL CChartItem::CXIBaseDesk::InvokeEvent(HWND _hWnd, LPCSTR _szEventName, LPCSTR _szParam)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)
	if( _szEventName == CString( "AddInEvent"))
	{
		pThis->AddInEvent_FromChart(_szParam);
		return TRUE;
	}
	return false;
}

BOOL CChartItem::CXIBaseDesk::InvokeEvent2(LPCSTR _szCtrlName, LPCSTR _szEventName, LPCSTR _szParam)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)
	return pThis->m_pBaseDesk2->InvokeEvent2(_szCtrlName, _szEventName, _szParam);
}
	
BOOL CChartItem::CXIBaseDesk::SendKey(HWND _hWnd, UINT nChar, BOOL _bIsShift, BOOL _bIsCtrl) 
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	return pThis->m_pBaseDesk2->SendKey(_hWnd, nChar, _bIsShift, _bIsCtrl);
}

LPVOID CChartItem::CXIBaseDesk::GetLSharedData()
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	return pThis->m_pBaseDesk2->GetLSharedData();
}



///////////////////////////////////////////////////////////////////////////////
// 만든이            : 한동혁(Dong-Hyuk, Han)
// Email             : aviman@dooriic.co.kr
// 만든 날짜         : 2002/2/1
// 함수 이름         : CChartItem::CXIBaseDesk::CallCommand
// 리턴 타입         : void
// 파라미터
//		LPCSTR _szCmd  : 
// 함수 설명         : 
///////////////////////////////////////////////////////////////////////////////
void CChartItem::CXIBaseDesk::CallCommand(LPCSTR _szCmd)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	pThis->m_pBaseDesk2->CallCommand(_szCmd);
}

// * 사용자 컬러 정보를 리턴한다.
OLE_COLOR CChartItem::CXIBaseDesk::GetUserColor(short nIndex, LPCTSTR szName, short nFlag)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	return pThis->m_pBaseDesk2->GetUserColor(nIndex, szName, nFlag);
}

// * 현재 아무것도 키값에 대한 정의가 되어있지 않으므로 널리턴.
// * 요구 또는 필요가 생길경우 여기에서 구현한다.
long CChartItem::CXIBaseDesk::GetDeskInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	return pThis->m_pBaseDesk2->GetDeskInfo(szKey);
}

long CChartItem::CXIBaseDesk::GetUserInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)
		
	return pThis->m_pBaseDesk2->GetUserInfo(szKey);
}

long CChartItem::CXIBaseDesk::GetFrameInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)
		
	return pThis->m_pBaseDesk2->GetFrameInfo(szKey);
}

long CChartItem::CXIBaseDesk::GetMainInfo(LPCSTR szKey)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)
	return NULL;
}




//* 수정 시작 (2002.8.22 001) 모니터링 툴 추가
BOOL CChartItem::CXIBaseDesk::FP_vAdviseDebug(LPCSTR sName, UINT pMonitoringCommand)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	return pThis->m_pBaseDesk2->FP_vAdviseDebug(sName, pMonitoringCommand);
}


void CChartItem::CXIBaseDesk::FP_vUnadviseDebug(LPCSTR sName, UINT pMonitoringCommand)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	pThis->m_pBaseDesk2->FP_vUnadviseDebug(sName, pMonitoringCommand);
}

LPCSTR CChartItem::CXIBaseDesk::FP_sGetMyVBName(HWND hWnd)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	return pThis->m_pBaseDesk2->FP_sGetMyVBName(hWnd);
}

void CChartItem::CXIBaseDesk::FP_vSendDebug(LPCSTR csSection, LPCSTR csKey, LPCSTR csValue)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	pThis->m_pBaseDesk2->FP_vSendDebug(csSection, csKey, csValue);
}
//* 수정 끝 (2002.8.22 001) 


// CControlPos의 m_awndControls 맴버변수의 포인터를 리턴한다.
// 이 변수는 리사이즈 될 컴포넌트의 정보를 들고 있다.
long CChartItem::CXIBaseDesk::FP_nGetCtrlPos()
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	return pThis->m_pBaseDesk2->FP_nGetCtrlPos();
}


// 컴포넌트를 리사이즈 시킨다.
void CChartItem::CXIBaseDesk::FP_vResizeCtrl()
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)
	
	pThis->m_pBaseDesk2->FP_vResizeCtrl();
}

// {{ OCX에서 DESK로 메세지 보낼 수 있도록 요청(김용식) 2003.05.19
void CChartItem::CXIBaseDesk::SetMapMessage(long lType, LPCSTR lpData)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

	pThis->m_pBaseDesk2->SetMapMessage(lType, lpData);
}
// }}



int CChartItem::CXIBaseDesk::FP_nGetDataInt(int nCommand)
{
	MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

		return 1;
//	return pThis->m_pBaseDesk2->FP_nGetDataInt(nCommand);
}

void CChartItem::SetSaveAllCount(int nAllCnt)
{
	m_nSaveAllCount = nAllCnt;
}

/////////////////////////////////////////////////////////////////////////////
// 분석툴 종목별 저장 - ojtaso (20080728)
CString CChartItem::GetOpenAndSaveFileName(BOOL bWithTypeValue/* = FALSE*/)
{
	if(!m_arrayChartItemData.GetSize()) return "temp.tmp";
	
	CChartItemData* pItemData = m_arrayChartItemData.GetAt(0);
	CString sJipyoSaveFileName, sJipyoFileName;
	
	// --> [Edit]  강지원 2009/04/14
#ifdef	_LASTINFO_SAVE
	sJipyoFileName.Format("%s", ((CStdDialog*)m_pStdDialog)->m_strScreenNo);
#else
	sJipyoFileName.Format("%s%s", ((CStdDialog*)m_pStdDialog)->m_strScreenNo, ((CStdDialog*)m_pStdDialog)->m_strSaveKey);
#endif
	// <-- [Edit]  강지원 2009/04/14
	
	char chOriType = GetType();
	// 분석툴 종목별 저장 - ojtaso (20080728)
	if(((CChartMng*)GetParent())->GetSaveJipyoOneFile() && !bWithTypeValue)
	{
		sJipyoSaveFileName = sJipyoFileName + "_";
	}
	else
	{
		switch(chOriType)
		{	
		case	SEC_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Sec_"	; break;
		case	TICK_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Tick_"	; break;			
		case	MIN_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Min_"	; break;	
		case	DAY_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Day_"	; break;	
		case	HOUR_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Hour_"	; break;
		case	WEEK_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Week_"	; break;
		case	MONTH_DATA_CHART:	sJipyoSaveFileName = sJipyoFileName + "_Month_"	; break;
		case	YEAR_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Year_"	; break;
		default					:	return "";
		}
	}
	sJipyoSaveFileName += JIPYOSYNC_FILE;		
	return sJipyoSaveFileName;
}

/////////////////////////////////////////////////////////////////////////////
// 2011.01.31 by SYS >> 분차트 YYYY(년) 추가 및 분석툴 공유 기능 추가
// 분석툴 종목별 저장
CString CChartItem::GetOpenAndSaveAnalFileName(BOOL bWithTypeValue/* = FALSE*/)
{
	if(!m_arrayChartItemData.GetSize()) return "temp.tmp";

	CChartItemData* pItemData = m_arrayChartItemData.GetAt(0);
	CString sJipyoSaveFileName, sJipyoFileName;

// --> [Edit]  강지원 2009/04/14
#ifdef	_LASTINFO_SAVE
	sJipyoFileName.Format("%s", ((CStdDialog*)m_pStdDialog)->m_strScreenNo);
#else
	sJipyoFileName.Format("%s%s", ((CStdDialog*)m_pStdDialog)->m_strScreenNo, ((CStdDialog*)m_pStdDialog)->m_strSaveKey);
#endif
// <-- [Edit]  강지원 2009/04/14

	char chOriType = GetType();
// 분석툴 종목별 저장 - ojtaso (20080728)
	if(((CChartMng*)GetParent())->GetSaveJipyoOneFile() && !bWithTypeValue)
	{
		sJipyoSaveFileName = sJipyoFileName + "_";
	}
	else
	{
		BOOL bShareAnalTool = ((CChartMng*)GetParent())->GetShareAnalTool();
		if (bShareAnalTool)
		{
			// 분석툴 공유 처리 (하나의 파일에 추세선 정보 저장함)
			sJipyoSaveFileName = sJipyoFileName + "_Share_"	;
		}
		else
		{
			switch(chOriType)
			{	
			case	SEC_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Sec_"	; break;
			case	TICK_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Tick_"	; break;			
			case	MIN_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Min_"	; break;	
			case	DAY_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Day_"	; break;	
			case	HOUR_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Hour_"	; break;
			case	WEEK_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Week_"	; break;
			case	MONTH_DATA_CHART:	sJipyoSaveFileName = sJipyoFileName + "_Month_"	; break;
			case	YEAR_DATA_CHART	:	sJipyoSaveFileName = sJipyoFileName + "_Year_"	; break;
			default					:	return "";
			}
		}
	}
	sJipyoSaveFileName += JIPYOSYNC_FILE;		
	return sJipyoSaveFileName;
}

BOOL CChartItem::InvokeAddInPattern(LPCTSTR p_szAddInDllName, short p_nCommandType, long p_lData)
{
	if(!m_bAddInLoadPattern)
	{
		LoadAddIn(p_szAddInDllName);
		m_bAddInLoadPattern = TRUE;
	}
	if(p_lData == 1)
		m_bDrawAutoTrendLine = TRUE;
	else if(p_lData == 2)
		m_bDrawAutoPattern = TRUE;
	else if(p_lData == 3)
		m_bDrawCandlePattern = TRUE;
	else if(p_lData == 4)
		m_bDrawAutoTrendLine = FALSE;
	else if(p_lData == 5)
		m_bDrawAutoPattern = FALSE;
	else if(p_lData == 6)
		m_bDrawCandlePattern = FALSE;
	return InvokeAddIn(p_szAddInDllName, p_nCommandType, p_lData);
}

BOOL CChartItem::InvokeAddInPeriod(LPCTSTR p_szAddInDllName, short p_nCommandType, long p_lData)
{
	if(!m_bAddInLoadPeriod)
	{
		LoadAddIn(p_szAddInDllName);
		m_bAddInLoadPeriod = TRUE;
	}
	return InvokeAddIn(p_szAddInDllName, p_nCommandType, p_lData);
}

BOOL CChartItem::InvokeAddInSTIndex(LPCTSTR p_szAddInDllName, short p_nCommandType, long p_lData)
{
	if(!m_bAddInLoadSTIndex)
	{
		LoadAddIn(p_szAddInDllName);
		InvokeAddIn(p_szAddInDllName, 3, (long)this);
		m_bAddInLoadSTIndex = TRUE;
	}

	if(p_nCommandType == 200)//초기화
	{
		m_strArrayNameBySystemTradingTab.RemoveAll();
	}

	return InvokeAddIn(p_szAddInDllName, p_nCommandType, p_lData);
}

BOOL CChartItem::InvokeAddInNews(LPCTSTR p_szAddInDllName, short p_nCommandType, long p_lData)
{
	if(!m_bAddInLoadNews)
	{
		LoadAddIn(p_szAddInDllName);
		m_bAddInLoadNews = TRUE;
	}
	return InvokeAddIn(p_szAddInDllName, p_nCommandType, p_lData);
}

CString CChartItem::GetLastHistoryCode(int nMarketType)
{
	//<<20100308_JS.Kim 솔로몬
	//IMasterDataManager* pDataManager = (IMasterDataManager*)AfxGetPctrInterface(UUID_IMasterDataManager, 2);
	IMasterDataManager* pDataManager = (IMasterDataManager*)AfxGetPctrInterface(UUID_IMasterDataManager);
	//>>
	CString strCode;
	if(pDataManager)
	{
		// 주식
		if(nMarketType == FX_CHART)
		{
			strCode = pDataManager->GetLastCode("01");
			if(strCode.GetLength() >= 11)
			{
				strCode = strCode.Mid(5, CODE_STOCK_LENGTH);
			}
		}
	}

	return strCode;
}

CString CChartItem::GetQueryOpt()
{
	int nQueryOpt = ((CChartMng*)GetParent())->SendMessage(UMSG_GETQUERYOPT);
	CString strQueryOpt;
	strQueryOpt.Format("%d", nQueryOpt);
	return strQueryOpt;
}

//@유진추가
/////////////////////////////////////////////////////////////////////////////

// 선택된 차트 정보는 외부dll에서 관리토록 수정 : 복수종목 - ojtaso (20070504)
void CChartItem::AddChartItem(CChartItemData* pChartItemData, LPCTSTR lpszItemCode, LPCTSTR lpszItemName, char cType, LPCTSTR lpszTickMinDayNumber, long lCount, long lChartNumericValueType, BOOL bBuiltChart)
{
	CString strChartGubun = pChartItemData->GetTrCode();
	// (2008/9/14 - Seung-Won, Bae) Make Sell/Buy ItemCode (Sell:|S_~, Buy:|B_~)
	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	int nRequestPairTickState = ( ( CStdDialog *)m_pStdDialog)->GetRequestPairTickState();
	switch( nRequestPairTickState)
	{
		case E_RPTS_PAIR:
		case E_RPTS_LOOP_PAIR:
		case E_PRTS_ROTATE_PAIR:
		{
			CString strRQ = m_strSelectedRQ;
			if( strRQ.Right( 5) != "_PAIR") strRQ = strRQ + "_PAIR";
			bBuiltChart = !HasIndicatorWithRQ( "가격차트", strRQ);
			//aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
			//m_strSelectedRQ = CDRChartOcx::AddPairItem( lpszItemCode, CStdDialog::GetRealItemCode( lpszItemName) + "(S)", bBuiltChart);
			m_strSelectedRQ = CDRChartOcx::AddPairItem( lpszItemCode, CStdDialog::GetRealItemCode( lpszItemName) + "(S)", bBuiltChart);
			CDRChartOcx::SetCurrentRQ(m_strSelectedRQ);
			//aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
			break;
		}
		default:
		{
			CString strItemName = CStdDialog::GetRealItemCode( lpszItemName);
			switch( nRequestPairTickState)
			{
				case E_RPTS_START:
				case E_RPTS_LOOP_START:
				case E_PRTS_ROTATE_START:
											strItemName = strItemName + "(B)";
											SetChartOrderType("0");//aaaaaaaaaaaaaa
											break;
			}
			//aaaaaaaaaaaaaaaaaaaaaaaaa
			//m_strSelectedRQ = CDRChartOcx::AddChartItem( lpszItemCode, strItemName, bBuiltChart);
			m_strSelectedRQ = CDRChartOcx::AddChartItem( lpszItemCode, strItemName, bBuiltChart);
			CDRChartOcx::SetCurrentRQ(m_strSelectedRQ);
			//aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
			break;
		}
	}

	ST_SELCHART* pSelChart = NULL;
	if(bBuiltChart)
	{
		pSelChart = new ST_SELCHART;
	}
	else
	{
		if(!m_mapChartItem.Lookup(m_strSelectedRQ, (LPVOID&)pSelChart))
			return;
	}

	pSelChart->m_strChartGubun = strChartGubun;
	pSelChart->m_strItemCode = lpszItemCode;
	pSelChart->m_strItemName = lpszItemName;
	pSelChart->m_cType = cType;
	pSelChart->m_strTickMinDayNumber = lpszTickMinDayNumber;
	pSelChart->m_lCount = lCount;
	pSelChart->m_nChartNumericValueType = lChartNumericValueType;
	pSelChart->m_strKey = pChartItemData->GetNextKey();
	//<<20100316_JS.Kim 솔로몬.FX
	pSelChart->m_nOrderType = atoi(m_strOrderType);
	//>>

	m_mapChartItem.SetAt(m_strSelectedRQ, pSelChart);

	m_lpSelChart = pSelChart;

}

// 선택된 차트 정보는 외부dll에서 관리토록 수정 : 복수종목 - ojtaso (20070504)
// 삭제된 RQ는 저장하지 않도록 수정 - ojtaso (20080509)
BOOL CChartItem::GetSelChart(LPCTSTR lpszRQ, ST_SELCHART*& pSelChart, BOOL bChangeSelRQ/* = TRUE*/)
{
	BOOL bResult = FALSE;

	if(!lpszRQ)
	{
		if(m_strSelectedRQ.IsEmpty())
			m_strSelectedRQ = _T("DEFAULT");

		bResult = m_mapChartItem.Lookup(m_strSelectedRQ, (LPVOID&)pSelChart);
	}
	else
	{
		bResult = m_mapChartItem.Lookup(lpszRQ, (LPVOID&)pSelChart);
		if(bResult && bChangeSelRQ)
			m_strSelectedRQ = lpszRQ;
	}

	return bResult;
}

// 종목코드로 RQ를 찾고 선택 : 복수종목 - ojtaso (20070705)
BOOL CChartItem::FindSelectChartByItemCode(LPCTSTR lpszItemCode, BOOL bSelect/* = TRUE*/)
{
	CString strRQ;
	ST_SELCHART* lpSelChart;

	POSITION pos = m_mapChartItem.GetStartPosition();
	while(pos)
	{
		m_mapChartItem.GetNextAssoc(pos, strRQ, (LPVOID&)lpSelChart);

		if(!lpSelChart->m_strItemCode.CompareNoCase(lpszItemCode))
		{
			if(bSelect)
			{
				m_strSelectedRQ = strRQ;
				CDRChartOcx::SetCurrentRQ(m_strSelectedRQ);

				if(GetSelChart(m_strSelectedRQ, lpSelChart))
				{
					OnSelectChartItem(lpSelChart);
					((CStdDialog*)m_pStdDialog)->OnSelectChartItem(lpSelChart);
				}
			}

			return TRUE;
		}
	}

	return FALSE;
}

// 분차트 마지막 키인 경우 - ojtaso (20071226)
void CChartItem::EnableRequestMoreButton(BOOL bEnable)
{
	((CStdDialog*)m_pStdDialog)->EnableRequestMoreButton(bEnable);
}

#ifdef _WRITELOG
// Log - ojtaso (20080107)
void CChartItem::WriteToStringLog( const char *p_szSection, const char *p_szData, int p_nDataLen)
{
	((CStdDialog*)m_pStdDialog)->WriteToStringLog(p_szSection, p_szData, p_nDataLen);
}
#endif

CChartItemData* CChartItem::GetItemDataAt( int index )
{
	return m_arrayChartItemData.GetAt(index);
}

// (2008/9/14 - Seung-Won, Bae) for Pair Tick
void CChartItem::SetCurrentRQ( CString strRQ)
{
	m_strSelectedRQ = strRQ;
	CDRChartOcx::SetCurrentRQ(m_strSelectedRQ);
	ST_SELCHART *lpSelChart = NULL;
	if(GetSelChart(m_strSelectedRQ, lpSelChart))
	{
		OnSelectChartItem( lpSelChart);
		( ( CStdDialog *)m_pStdDialog)->OnSelectChartItem( lpSelChart);
	}
}

void CChartItem::RemoveMapChartItem()
{
	// 선택된 차트 정보는 외부dll에서 관리토록 수정 : 복수종목 - ojtaso (20070504)
	POSITION pos = m_mapChartItem.GetStartPosition();
	ST_SELCHART* pSelChart = NULL;
	CString strKey;
	while(pos)
	{
		m_mapChartItem.GetNextAssoc(pos, strKey, (LPVOID&)pSelChart);
		delete pSelChart;
	}

	m_mapChartItem.RemoveAll();

	pSelChart = new ST_SELCHART;
	m_mapChartItem.SetAt(_T("DEFAULT"), pSelChart);
}

//<<20100316_JS.Kim 솔로몬.FX
void CChartItem::SetChartOrderType(CString	strType)
{
	m_strOrderType = strType;
	if( m_lpSelChart )
		m_lpSelChart->m_nOrderType = atoi(strType);
}

ST_CHARTITEM_REALDATA* CChartItem::GetChartItemRealData(CString strRealItemCode)
{
	ST_CHARTITEM_REALDATA* pChartItemRealData = NULL;
	m_mapChartItemRealData.Lookup(strRealItemCode, (LPVOID&)pChartItemRealData);
	return pChartItemRealData;
}
//>>