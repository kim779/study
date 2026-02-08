// ChartMng.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "ChartMng.h"
#include "StdDialog.h"
#include "CodeDataFromInputBar.h"
#include "./define/ChartMsgDef.h"
#include "./define/DragNDropDefine.h"
#include "./define/MessageDefine.h"
#include "../COMMONTR/ChartRealDef.h"

//#include "../../COMMON_BASE/drds/DRDSDoEF.h"
//#include "../CommonTR/DrdsS31.h"
//#include "../CommonTR/DrdsJS0.h"
//#include "../CommonTR/DrdsSC0.h"
//#include "../CommonTR/DrdsOC0.h"
//#include "../../Inc/RealUpdate.h"

//#include "../../../DataMgr/DBMgr.h" //05.09.27
//#include "../../../DrMLib/DrMiddleLib.h"//05.09.27

//#include "../../INCLUDE_BASE/XfmMapInfo.h"
//#include "../../ToolKit/MainDll_Interface/Plat/form.h"	// 플랫폼 관련 부분 모음.
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT	RMSG_OPENMAPWITHDATA	= ::RegisterWindowMessage(_T("RMSG_OPENMAPWITHDATA"));
const UINT  RMSG_SAVE_FOCUS_CONTROL = ::RegisterWindowMessage("RMSG_SAVE_FOCUS_CONTROL"); //05.09.30


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
//#define UWM_SELECTEDCHART			( WM_USER + 0x2D46)
// 복수종목의 Total Count를 멀티차트에 알려줌 : 복수종목 - ojtaso (20070704)
//#define UWM_MULTITOTALCOUNT			( WM_USER + 0x2D47)

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

/////////////////////////////////////////////////////////////////////////////
// CChartMng

int		 CChartMng::m_nStatic_RecvCntShareCode = 0;
CString  CChartMng::m_strStatic_ShareCode = _T(""); //차트 로드시 공유로 들어온 종목을 한번만 적용하기 위해 
CString  CChartMng::m_strCodeForMulti = _T(""); //차트 로드시 공유로 들어온 종목을 한번만 적용하기 위해 


CChartMng::CChartMng()
{
	m_bNeedForceCode    = TRUE;
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
	m_clStartSelected =	RGB(255,241,206);//RGB(255,255,239);	//RGB(255,233,160);
	//m_clEndSelected = RGB(219,229,247);
	m_clStartUnselected = RGB(238,238,238);//RGB(225,227,233);
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
	m_bLBDnFromChart = FALSE;

	m_bEmptyDataChart		= FALSE;
	m_bEmptyChartForSinkCfg	= FALSE;



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

	m_pCodeData = NULL;
}

CChartMng::~CChartMng()
{
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
	ON_REGISTERED_MESSAGE( RMSG_SELCHARTCODE, OnRmsgSelChartCode )
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
	if( ((CStdDialog*)GetParent())->m_nChartType==MINI_MULTI_CHART ||
		((CStdDialog*)GetParent())->m_nChartType==MINI_SINGLE_CHART ) 
		bNoToolBar = TRUE;

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
//-->@080613-001 : 등락률에서 차트선택시 개별설정창 뜨지 안게 처리
	if(((CStdDialog*)GetParent())->m_nScreenType==CStdDialog::Screen_fluctuation)
		pChartItem->SetBSelectTitle(FALSE);
//<--
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

//	if(((CStdDialog*)GetParent())->m_nChartType==SINGLE_CHART)
//		m_bSaveJipyoOneFile = m_pInputBarDlg->SendMessage(UMSG_GETSAVEJIPYOONEFILE);
//	else if(m_pExtInputBarDlg)
//		m_bSaveJipyoOneFile = m_pExtInputBarDlg->SendMessage(UMSG_GETSAVEJIPYOONEFILE);
	
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

#ifdef _DEBUG
		EACH_CHARTJOB_START()
			pChartItem->InvokeAddInSTIndex("ChartSTIndexAddIn.dll",2,0);
		EACH_CHARTJOB_END()		
#endif

	BOOL bSavedFromExtInputBarGlg = FALSE;
	CString sJipyoFileName1, sJipyoSaveFileName;

	if(m_pExtInputBarDlg)
	{
		bSavedFromExtInputBarGlg = m_pExtInputBarDlg->SendMessage(UMSG_GETSAVED); //종료시 저장은 무조건 전부 
		if(bSavedFromExtInputBarGlg)//모두 저장시 푼다.
		{
			SaveChart(((CStdDialog*)GetParent())->m_strConfig);
		}
	}
	m_ftForTitle.DeleteObject();		
	m_ftNormal.DeleteObject();
	EACH_CHARTJOB_START()
		pChartItem->DestroyWindow();
		delete pChartItem;
		EACH_CHART_MAKENULL()
	EACH_CHARTJOB_END()

	m_lCols = 0;
	m_lRows = 0;
	
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

	int nHeightOffset = HEIGHTOFCAPTION;
	if(((CStdDialog*)GetParent())->m_nScreenType==CStdDialog::Screen_fluctuation)
		nHeightOffset = 0;

	if(nTitle==1) // Title
	{
		//rectOfChart.bottom = rectOfChart.top + HEIGHTOFCAPTION;
		rectOfChart.bottom = rectOfChart.top + nHeightOffset;
	}
	else if(nTitle==0) // Chart
	{
		//rectOfChart.top += HEIGHTOFCAPTION;
		rectOfChart.top += nHeightOffset;
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
	
	
	//2005. 09. 05 added by sy, nam ==================================
	//CString strCode = GetpChartItemSelected()->GetDataCode();
	//m_pInputBarDlg->SendMessage(UMSG_SETTEXT_CODE_INPUTBAR, 0, (LPARAM)(LPTSTR)(LPCTSTR)strCode);
	//================================================================
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
	return;

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
	if(m_nMarketType == CMasterTypeInfo::STOCK_CHART)
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
	return;
// 		//@비교차트에서 처리할 부분이 있나????
// 
// 		pStrArray->RemoveAll();
// 		if(p_strGroup[0]==NULL || p_strGroup == "전체지표")
// 		{
// 			CString strList = GetIndicatorNameList("");
// 			CString strALine;
// 			CString strALineTitle;
// 			CString strAItem;
// 			int nPos = 0;
// 			int nLength = 0;
// 			pStrArray->Add("거래량차트");
// 			if(strList.GetLength())
// 			{
// 				while(1)
// 				{
// 					nPos = strList.Find(':');
// 					if(nPos==-1) break;		
// 					if(nPos==0) 
// 					{
// 						nPos = strList.Find('\n');
// 						strList.Delete(0,nPos+1);
// 						continue;
// 					}			
// 					strALineTitle = strList.Left(nPos);
// 					if(strALineTitle=="기본지표"
// 						||strALineTitle=="시장지표"
// 						||strALineTitle=="투자자동향"
// 						||strALineTitle=="특수지표")
// 					{
// 						nPos = strList.Find('\n');
// 						strList.Delete(0,nPos+1);
// 						continue;
// 					}
// 
// 					strList.Delete(0,nPos+1);
// 					nPos = strList.Find('\n');
// 					strALine = strList.Left(nPos-1);		
// 					strList.Delete(0,nPos+1);
// 
// 					while(1)
// 					{
// 						nLength = strALine.GetLength();
// 						if(!nLength) break;
// 						nPos = strALine.Find(',');
// 						if(nPos==-1) 
// 						{
// 							strAItem = strALine;
// 							strALine.Empty();
// 						}
// 						else
// 						{
// 							strAItem = strALine.Left(nPos);
// 							strALine.Delete(0,nPos+1);
// 						}
// 						pStrArray->Add(strAItem);
// 					}
// 				}		
// 			}		
// 		}
// 		else
// 		{
// 			CString strGroup = p_strGroup;
// 			if(strGroup=="특수지표")
// 			{
// 				pStrArray->Add("캔들차트");
// 				pStrArray->Add("바차트");
// 				pStrArray->Add("선차트");
// 				pStrArray->Add("FLOW차트");
// 				//pStrArray->Add("점차트");
// 				//pStrArray->Add("렌코차트");
// 				//pStrArray->Add("삼선전환도");
// 				pStrArray->Add("캔들볼륨차트");
// 				pStrArray->Add("이큐볼륨차트");
// 			}
// 
// 			CString strALine = GetIndicatorNameList(p_strGroup);
// 			CString strAItem;
// 			int nPos = 0;
// 			int nLength = 0;
// 
// 			if(strALine.GetLength())
// 			{
// 				while(1)
// 				{
// 					nLength = strALine.GetLength();
// 					if(!nLength) break;
// 					nPos = strALine.Find(',');
// 					if(nPos==-1) 
// 					{
// 						strAItem = strALine;
// 						strALine.Empty();
// 					}
// 					else
// 					{
// 						strAItem = strALine.Left(nPos);
// 						strALine.Delete(0,nPos+1);
// 					}
// 					//if(strAItem=="P&F")
// 					//{
// 					//	strAItem = "P&&F";
// 					//}
// 					if(strAItem=="가격차트")
// 					{
// 						continue;
// 					}
// 					pStrArray->Add(strAItem);
// 				}
// 			}
// 		}
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
	return;
	//@ 비교차트에서 할 부분이 있나???

// 		if(p_strGroup[0]!=NULL)
// 		{
// 			CString strALine = GetIndicatorNameList(p_strGroup);
// 			CString strAItem;
// 			int nPos = 0;
// 			int nItem = 0;
// 			int nLength = 0;
// 			rxlistCtrl.DeleteAllItems();
// 			if(!strcmp(p_strGroup,"특수지표"))
// 			{
// 				rxlistCtrl.InsertItem(0, "");
// 				rxlistCtrl.SetItemText(0, 0, "캔들차트",RGB(0,0,0),CRGRID);
// 
// 				rxlistCtrl.InsertItem(1, "");
// 				rxlistCtrl.SetItemText(1, 0, "바차트");
// 
// 				rxlistCtrl.InsertItem(2, "");
// 				rxlistCtrl.SetItemText(2, 0, "선차트",RGB(0,0,0),CRGRID);
// 
// 				//rxlistCtrl.InsertItem(3, "");
// 				//rxlistCtrl.SetItemText(3, 0, "점차트");
// 
// 				//rxlistCtrl.InsertItem(4, "");
// 				//rxlistCtrl.SetItemText(4, 0, "렌코차트");
// 
// 				//rxlistCtrl.InsertItem(4, "");
// 				//rxlistCtrl.SetItemText(4, 0, "삼선전환도",RGB(0,0,0),CRGRID);
// 
// 				//rxlistCtrl.InsertItem(6, "");
// 				//rxlistCtrl.SetItemText(6, 0, "이큐볼륨차트");
// 				
// 				//rxlistCtrl.InsertItem(7, "");
// 				//rxlistCtrl.SetItemText(7, 0, "캔들볼륨차트",RGB(0,0,0),CRGRID);
// 				
// 				nItem = 3;
// 			}
// 
// 			if(strALine.GetLength())
// 			{
// 				while(1)
// 				{
// 					nLength = strALine.GetLength();
// 					if(!nLength) break;
// 					nPos = strALine.Find(',');
// 					if(nPos==-1) 
// 					{
// 						strAItem = strALine;
// 						strALine.Empty();
// 					}
// 					else
// 					{
// 						strAItem = strALine.Left(nPos);
// 						strALine.Delete(0,nPos+1);
// 					}
// 					//if(strAItem=="P&F")
// 					//{
// 					//	strAItem = "P&&F";
// 					//}
// 					rxlistCtrl.InsertItem(nItem, "");
// 					if(nItem%2) // odd
// 					{
// 						rxlistCtrl.SetItemText(nItem, 0, strAItem);
// 					}
// 					else		// even
// 					{
// 						rxlistCtrl.SetItemText(nItem, 0, strAItem,RGB(0,0,0),CRGRID);					
// 					}				
// 					nItem++;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			CString strList = GetIndicatorNameList("");
// 			CString strALine;
// 			CString strALineTitle;
// 			CString strAItem;
// 			int nPos = 0;
// 			int nItem = 0;
// 			int nLength = 0;
// 			rxlistCtrl.DeleteAllItems();
// 			rxlistCtrl.InsertItem(nItem, "");
// 			rxlistCtrl.SetItemText(nItem, 0, "거래량차트",RGB(0,0,0),CRGRID);
// 			nItem++;
// 
// 			if(strList.GetLength())
// 			{
// 				while(1)
// 				{
// 					nPos = strList.Find(':');
// 					if(nPos==-1) break;		
// 					if(nPos==0) 
// 					{
// 						nPos = strList.Find('\n');
// 						strList.Delete(0,nPos+1);
// 						continue;
// 					}			
// 					strALineTitle = strList.Left(nPos);
// 					if(strALineTitle=="기본지표"
// 						||strALineTitle=="시장지표"
// 						||strALineTitle=="투자자동향"
// 						||strALineTitle=="특수지표")
// 					{
// 						nPos = strList.Find('\n');
// 						strList.Delete(0,nPos+1);
// 						continue;
// 					}
// 
// 					strList.Delete(0,nPos+1);
// 					nPos = strList.Find('\n');
// 					strALine = strList.Left(nPos-1);		
// 					strList.Delete(0,nPos+1);
// 					
// 					while(1)
// 					{
// 						nLength = strALine.GetLength();
// 						if(!nLength) break;
// 						nPos = strALine.Find(',');
// 						if(nPos==-1) 
// 						{
// 							strAItem = strALine;
// 							strALine.Empty();
// 						}
// 						else
// 						{
// 							strAItem = strALine.Left(nPos);
// 							strALine.Delete(0,nPos+1);
// 						}
// 						rxlistCtrl.InsertItem(nItem, "");
// 						if(nItem%2) // odd
// 						{
// 							rxlistCtrl.SetItemText(nItem, 0, strAItem);
// 						}
// 						else		// even
// 						{
// 							rxlistCtrl.SetItemText(nItem, 0, strAItem,RGB(0,0,0),CRGRID);					
// 						}				
// 						nItem++;
// 					}
// 				}		
// 			}
// 		}
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
		}
		break;
	case WM_RBUTTONUP:
		{
			CDRChartOcx *pChart = (CDRChartOcx *)GetDlgItem(wParam);		
			if(pChart)
			{
				BOOL bShowMenu = FALSE;
				EACH_CHARTJOB_START()
					if(pChart==pChartItem)
					{
						//if(!bShowMenu && pMsgFromChart->message==WM_RBUTTONUP && m_pShareInputData->m_nScreenType==CStdDialog::Screen_fluctuation)
						if(!bShowMenu && pMsgFromChart->message==WM_RBUTTONUP)
						{
							bShowMenu = fnMenuLoad(TRUE, m_pShareInputData->m_nScreenType);
							//if(fnMenuLoad())	return 0;
						}

						//}}
						break;
					}	
				EACH_CHARTJOB_END()	

				if(bShowMenu) fnMenuLoad(FALSE, m_pShareInputData->m_nScreenType);
			}			
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
			/*
			CChartItem* pChartItem = NULL;
			pChartItem = GetpChartItemSelected();
			if(pChartItem == NULL) 
				break;

			CKBChart *pChart = (CKBChart *)GetDlgItem(wParam);
			if(!pMsgFromChart->lParam) return 0;
			if(GetpChartItemSelected()==pChart)
			{
				//--------------------------------------------------------------------------------
				//2005. 05. 27 InputBar가 아닌 ExtInputBarDlg로 보낸다.
				//m_pInputBarDlg->SendMessage(UMSG_SETVIEWENDTIME,pMsgFromChart->lParam);
				if(m_pExtInputBarDlg)
					m_pExtInputBarDlg->SendMessage(UMSG_SETVIEWENDTIME,pMsgFromChart->lParam);
				//--------------------------------------------------------------------------------
			}*/
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
			if(pChart->GetDataTRCode()==QUERY_STRJONGMOK)
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

			//if(!m_bReplaychartMode) //06.02.03  복기 시 초기화로 인한 TR 전송하는 것을 막는다.
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
			}		
		}
		break;
		// 복수종목 차트 선택 : 복수종목 - ojtaso (20070122)
	case UWM_SELECTEDCHART:
		{
			if(pMsgFromChart->lParam)
			{
				// 선택된 차트 정보는 외부dll에서 관리토록 수정 : 복수종목 - ojtaso (20070504)
				CChartItem* pSelItem = GetpChartItemSelected();
				if(pSelItem)
				{
					CString pSelRQ = (LPCTSTR)pMsgFromChart->lParam;
					ST_SELCHART* pSelChart = NULL;

					if(pSelItem->GetSelChart(pSelRQ, pSelChart))
					{
						if(!pMsgFromChart->wParam)	// 차트 삭제
						{//KHD : 죽는 현상이 있어서 일단 주석처리 

							//리얼 및 SCD삭제
							((CStdDialog*)GetParent())->OnDeleteChartItem(pSelChart->m_strItemCode);
							// 리스트에서 삭제
							((CStdDialog*)GetParent())->DeleteToLeftBar(pSelChart->m_strItemCode);


							// RQ에서 삭제
// 							pSelItem->OnDeleteChartItem(pSelRQ, pSelChart);
// 							// (2010/12/22 - Seung-Won, Bae) Select Other RQ after deletion.
// 							pSelItem->GetSelChart( NULL, pSelChart);
// 							pSelRQ = pSelItem->GetSelectedRQ();
// 							if( !pSelRQ.IsEmpty()) // 선택됨
// 							{
// 								((CStdDialog*)GetParent())->OnSelectChartItem(pSelChart);
// 								pSelItem->OnSelectChartItem(pSelChart);
// 							}
// 					//END 
 						}
 						else						// 선택됨
 						{
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
			((CInputBarDlg*)m_pInputBarDlg)->SetMultiTotalCount(pMsgFromChart->lParam);
		break;

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
	static CString szID;
	szID.Format("%s%s", ((CStdDialog*)GetParent())->m_strUserID, "\\Chart");
	return (long)(LPCSTR)szID;
	
	//기존루틴  return (long)(LPCSTR)((CStdDialog*)GetParent())->m_strUserID;
}

void CChartMng::SetTimeMark(LPCTSTR p_szMarkTime, BOOL p_bWithScroll)
{
	EACH_CHARTJOB_START()
		pChartItem->SetTimeMark(p_szMarkTime,p_bWithScroll);
	EACH_CHARTJOB_END()	
}


/*
void CChartMng::OnPaint() 
{
	CRect clientRc;
	GetClientRect(&clientRc);
	CPaintDC dc(this);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, clientRc.Width(), clientRc.Height());
	CBitmap * obmp = memDC.SelectObject(&bmp);

	CDC * pDC = &memDC;

	//	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect,RGB(128,135,145));
	rect.DeflateRect(1,1,1,1);
	pDC->FillSolidRect(rect, RGB(237,243,249));	//RGB(222,222,222));
	EACH_CHARTJOB_START()
		PaintTitle(nRowIndex,nColIndex,pChartItem,pDC);			
	//		pChartItem->Invalidate();
	EACH_CHARTJOB_END()

		dc.BitBlt(clientRc.left, clientRc.top, clientRc.Width(), clientRc.Height(), &memDC, 0,0, SRCCOPY);

	memDC.SelectObject(obmp);
}
*/
void CChartMng::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect,RGB(0,0,0));

	EACH_CHARTJOB_START()
		PaintTitle(nRowIndex,nColIndex,pChartItem,&dc);
		pChartItem->Invalidate();
	EACH_CHARTJOB_END()

}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	Input	:	
//	목적	:	차트의 Title들을 Draw한다.
///////////////////////////////////////////////////////////////////////////////
/*
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
*/

//void CChartMng::PaintTitle(int nRowIndex,int nColIndex,CChartItem* pChartItem,CDC *pDC)
void CChartMng::PaintTitle(int nRowIndex,int nColIndex,CChartItem* pChartItem,CPaintDC *pDC)
{
	// 등락률차트이면 아무처리 않는다.
	if(((CStdDialog*)GetParent())->m_nScreenType==CStdDialog::Screen_fluctuation)	return;

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
		pChartItem->DrawTitle(backup, pDC,&m_ftForTitle, &m_ftNormal, m_clStartSelected);
	}
	else
	{
		pChartItem->DrawTitle(backup, pDC,&m_ftForTitle, &m_ftNormal, m_clStartUnselected);
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

//void CChartMng::MoveCharts(int nRowIndexOutside/*= -1*/,int nColIndexOutside/* = -1*/)
/*
void CChartMng::MoveCharts(int nRowIndexOutside, int nColIndexOutside)
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
			//pChartItem->SetRootPath(m_strRootPath);
		}
		//====================================================================================
		pChartItem->MoveWindow(rect);	

	}
	
	Invalidate(TRUE);
}
*/

//void CChartMng::MoveCharts(int nRowIndexOutside, int nColIndexOutside)
void CChartMng::MoveCharts()
{
	EACH_CHARTJOB_START()
	
	CRect rect;
	if(m_pShareInputData->m_nScreenType==0)
		rect = GetRectOfChart(nRowIndex,nColIndex);
	else
		rect = GetRectOfChart(nRowIndex,nColIndex, 0);
	//2005. 06. 07 =======================================================================
	//새로이 생성되는 ChartOCX(chartItem)에게 Drds를 요청 가능한 ClassPointer를 넘겨준다.
	//added by sang yun, nam
	if(m_lMltchrtDlgParentClsPointer > 0)
		pChartItem->SetOutsideCommonInterface(m_lMltchrtDlgParentClsPointer); 
	//====================================================================================

	//2005. 07. 18 ==============================================================
	//비교차트에서 종목 타이틀을 선택가능하게 한다.
	if(((CStdDialog*)GetParent())->m_nScreenType!=CStdDialog::Screen_fluctuation)
		pChartItem->SetBSelectTitle(TRUE);
	//===========================================================================	
	//=================================================================
	CString sTemp;
	sTemp.Format("MoveCharts l:%d, t:%d, r:%d, b:%d", rect.left, rect.top, rect.right, rect.bottom);
	OutputDebugString(sTemp);
	if(rect.Width() == 0)
		rect.right = rect.left + 100;

	if(rect.Height() == 0)
		rect.bottom = rect.top + 100;
	//=================================================================
	pChartItem->MoveWindow(rect);
	
	EACH_CHARTJOB_END()	
	Invalidate();
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
					pChartItem->m_pShareInputData = m_pShareInputData;

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
						char chType = m_pInputBarDlg ? (char)m_pInputBarDlg->SendMessage(UMSG_GETTYPEFROMINPUTBAR) : '0';
						long lNumber = m_pInputBarDlg ? m_pInputBarDlg->SendMessage(UMSG_GETTYPENUMFROMINPUTBAR) : 0;
						pChartItem->SetType(chType);
						pChartItem->SetTickMinDayNumber(lNumber);
					}
					if(m_lKey)
					{
						if( ((CStdDialog*)GetParent())->m_nChartType==MINI_MULTI_CHART ||
							((CStdDialog*)GetParent())->m_nChartType==MINI_SINGLE_CHART )
						{
							pChartItem->SetInit((long)this->GetSafeHwnd(), m_lKey, m_strRootPath, TRUE);
						}
						else
						{
							pChartItem->SetInit((long)this->GetSafeHwnd(), m_lKey, m_strRootPath, FALSE);						
						}
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

					pChartItem->SetBNormalizedMinuteTimeRule(pChartItem->m_CfgInfo.bRealCandle ? FALSE : TRUE);

					pChartItem->SetDragModeType				(nAnalysisType);

//-->@080613-001 : 등락률에서 차트선택시 개별설정창 뜨지 안게 처리
					if(((CStdDialog*)GetParent())->m_nScreenType==CStdDialog::Screen_fluctuation)
						pChartItem->SetBSelectTitle(FALSE);
//<--
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
//			if( (pChartItem->GetDataTRCode()==QUERY_STRJONGMOK	) ||
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
//			//if(pChartItem->GetDataTRCode()==QUERY_STRJONGMOK)
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
//			if(pChartItem->GetDataTRCode()==QUERY_STRJONGMOK)
//			{
//공유 두번 날라가서 막음 by LYH 2007.01.22
//				GetParent()->SendMessage(UMSG_SENDSHAREDCODE,(WPARAM)(LPCTSTR)pChartItem->GetDataCode());				
//			}
//			((CCompareChartDlg*)GetParent())->SendMessage(RMSG_SETFOCUSTOCODE);
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
	m_bNormalizedMinuteTimeRuleInLoop = NULL;
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
	m_pChartItemInLoop = m_pChartItems[m_nRowIndexInLoop][m_nColIndexInLoop];
	if(!m_pChartItemInLoop) 
	{
		if(!HeadInLoop()) 
		{
			PostInLoop();
		}
		return TRUE;
	}
	SetChartSelected(CPoint(m_nColIndexInLoop,m_nRowIndexInLoop), FALSE, TRUE);

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
				PostInLoop();
				m_bIsInLoop = FALSE;
				m_bLastEnvUpdated = TRUE;
				return FALSE;				
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
			if(m_pChartItemInLoop->ScrollToDataOfEnd(m_dEndDateInLoop))
			{
				if(!HeadInLoop()) PostInLoop();
				return TRUE;
			}
			m_pChartItemInLoop->SetEndDateRequested((LPCTSTR)m_strEndDateInLoop);
			m_pChartItemInLoop->SetEndDate((LPCTSTR)m_strEndDateInLoop); //05.09.06 added by sy, nam
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
			if(m_pChartItemInLoop->GetBNormalizedMinuteTimeRule() != m_bNormalizedMinuteTimeRuleInLoop)
			{
				m_pChartItemInLoop->SetBNormalizedMinuteTimeRule(m_bNormalizedMinuteTimeRuleInLoop);
				m_bNeedToUpdateInLoop = TRUE;
			}			
			if(m_pChartItemInLoop->GetDataCode()!=m_strCodeInLoop&&m_bJongmokLink)
			{
				m_pChartItemInLoop->SetCodeForQuery(m_strCodeInLoop,QUERY_STRJONGMOK,TRUE);
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
				//if(   (m_strCodeInLoop.GetLength()==UPJONG_LENGTH)  
				switch(m_nMarketType)
				{
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
					case CMasterTypeInfo::STOCK_CHART:
						{
							if(m_strCodeInLoop.GetLength()== CODE_STOCK_LENGTH)				
							{
								CChartItem* pChartItem = NULL;
								pChartItem = GetpChartItemSelected();
								if(pChartItem == NULL) 
									break;

								GetpChartItemSelected()->SetCodeForQuery(m_strCodeInLoop,QUERY_STRJONGMOK,TRUE);
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
//					case FOREIGN_CHART:
//						{
//							CChartItem* pChartItem = NULL;
//							pChartItem = GetpChartItemSelected();
//							if(pChartItem == NULL) 
//								break;
//
//							//2005.05.06 by sy,nam
//							GetpChartItemSelected()->SetCodeForQuery(m_strCodeInLoop,QUERY_STRFOREIGN,TRUE);
//						}
//						break;
//@유진삭제
//					//2007.02.02
//					case ELW_CHART:
//						{
//							CChartItem* pChartItem = NULL;
//							pChartItem = GetpChartItemSelected();
//							if(pChartItem == NULL) 
//								break;
//
//							GetpChartItemSelected()->SetCodeForQuery(m_strCodeInLoop,QUERY_STRELW,TRUE);
//						}
//						break;
//						// 상품선물옵션차트 - ojtaso (20071001)
//					case COMMODITY_CHART:
//						{
//							if( (m_strCodeInLoop.GetLength()	==	COMMODITY_LENGTH	))
//							{
//								CChartItem* pChartItem = NULL;
//								pChartItem = GetpChartItemSelected();
//								if(pChartItem == NULL) 
//									break;
//
//								//2005.05.06 by sy,nam
//								GetpChartItemSelected()->SetCodeForQuery(m_strCodeInLoop,QUERY_STRCOMMODITY,TRUE);
//							}
//						}
//						break;
//@유진삭제
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
		}
		break;
	}

	if(!RequestChartData())
	{
		if(!HeadInLoop()) PostInLoop();
	}
	return TRUE;
}


//BOOL CChartMng::InputCodeFromInputBar(LPCTSTR lpCode,long lEndDate,BOOL bForcedMode0 /*=FALSE*/)
BOOL CChartMng::InputCodeFromInputBar(CCodeDataFromInputBar* pCodeData,BOOL bForcedMode0 /*=FALSE*/)
{
	m_pCodeData = pCodeData;
	GetpChartItemSelected()->SetChartRequestType(pCodeData->m_chType);

	if(((CStdDialog*)GetParent())->m_nChartType==SINGLE_CHART)
		m_bJongmokLink = TRUE;

	m_bNeedForceCode = FALSE; // 공유가 들어왔기 때문에 InputBar쪽에서 강제로 코드를 날릴 필요가 없다고 설정한다.
	 
	//if(!m_bIsSystemTradingMode)
	{
		//nOpenedOwner  = 0 : 메뉴/툴바에서 열림
		//nOpenedOwner  = 1 : 마우스 오른쪽 버튼에서 열림 (종목코드 포함)
		//nOpenedOwner  = 2 : 스크립트,dll에서 화면 요청 (OpenMapData 포함)		
		//UINT nOpenedOwner = g_pViewInMain->SendMessage(RMSG_GETOPENEDOWNER);
		UINT nOpenedOwner = ::SendMessage(((CStdDialog*)GetParent())->m_hViewInMain,RMSG_GETOPENEDOWNER,0,0);
		if(!m_bLastEnvUpdated)
		{
			//=================================================
			// 처음에 뜰때 단 한번만 호출된다.
			//=================================================
			if( ((CStdDialog*)GetParent())->m_nChartType==MINI_MULTI_CHART ||
				((CStdDialog*)GetParent())->m_nChartType==MINI_SINGLE_CHART )
			{			
				if(GetpChartItemSelected())
				{	
					GetpChartItemSelected()->LoadChartInfo(((CStdDialog*)GetParent())->m_strConfig);
					m_bLastEnvUpdated = TRUE;
				}
			}
			else
			{
				//분할 저장 정보를 받는다.					
				//>>>> Start==================================================================================
				//BOOL bLoad = m_pExtInputBarDlg->SendMessage(UMSG_GETSAVED);
//@유진삭제
//				BOOL bLoad = TRUE;		// 나중에 확인
//				if(bLoad)
//				{	
//					//SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
//@유진삭제
					
					m_strStatic_ShareCode = pCodeData->m_strCode; //Static으로 단 한번 적용한다.

					BOOL bResult = FALSE;
					bResult = LoadChart(((CStdDialog*)GetParent())->m_strConfig);


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

		switch(m_nMarketType)
		{
			case CMasterTypeInfo::UPJONG_CHART:
				{
					if(strCode.GetLength()==UPJONG_LENGTH) 
					{
						pSelectedChartItem->SetCodeForQuery(pCodeData->m_strCode,QUERY_STRUPJONG,TRUE);
					}
				}
				break;
			case CMasterTypeInfo::FUTOPT_CHART:
				{
					if( (strCode.GetLength()==FUTOPT_LENGTH	)	||
						(strCode.GetLength()==KQOPT_LENGTH	)	||
						(strCode.GetLength()==KQFUT_LENGTH	)	||
						(strCode.GetLength()==KQOPT_LENGTH12)	
					)
					{
						//2005.05.06 by sy,nam  
						pSelectedChartItem->SetCodeForQuery(pCodeData->m_strCode,QUERY_STRFUTOPT,TRUE);
					}
				}
				break;
			case CMasterTypeInfo::STOCK_CHART: 
				{
					if(strCode.GetLength()==CODE_STOCK_LENGTH)
					{			
						//2005.05.06 by sy,nam
						pSelectedChartItem->SetCodeForQuery(pCodeData->m_strCode,QUERY_STRJONGMOK,TRUE);
					}	
				}
				break;
			case CMasterTypeInfo::STOCKFUTOPT_CHART:
				{
					if( (strCode.GetLength()==FUTOPT_LENGTH	)	||
						(strCode.GetLength()==KQOPT_LENGTH	)	||
						(strCode.GetLength()==KQFUT_LENGTH	)	||
						(strCode.GetLength()==KQOPT_LENGTH12)	
					)
					{
						//2005.05.06 by sy,nam  
						pSelectedChartItem->SetCodeForQuery(pCodeData->m_strCode,QUERY_STRSTOCKFUTOPT,TRUE);
					}
				}
				break;
			case FOREIGN_CHART:				
				{			
					//2005.07.06 by sy,nam			
					pSelectedChartItem->SetCodeForQuery(pCodeData->m_strCode,QUERY_STRFOREIGN,TRUE);
				}
				break;
//@유진삭제
//			case ELW_CHART:				
//				{			
//					//2005.07.06 by sy,nam			
//					pSelectedChartItem->SetCodeForQuery(pCodeData->m_strCode,QUERY_STRELW,TRUE);
//				}
//				break;
//			case COMMODITY_CHART:	// 상품선물옵션차트 - ojtaso (20071001)
//				{
//					if( (strCode.GetLength()==COMMODITY_LENGTH	))
//					{
//						//2005.05.06 by sy,nam  
//						pSelectedChartItem->SetCodeForQuery(pCodeData->m_strCode,QUERY_STRCOMMODITY,TRUE);
//					}
//				}
//				break;
//@유진삭제
			default:
#ifdef _DEBUG
				AfxMessageBox("InputCodeFromInputBar == 차트 구분값이 없다");
#endif
				break;
		}
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
		// gm0604 added For SystemTrading 2005.7.21 End


		//========================================================================================
		// 2005. 09. 23
		ApplySyncEnv_OnlyOne();
		//========================================================================================

		//========================================================================================
		// 2005. 09. 23
		m_bChangeTypeMode = FALSE;
		//========================================================================================

#ifdef _WRITELOG
		// Log - ojtaso (20080107)
		((CStdDialog*)GetParent())->WriteToStringLog(_T("차트개별 관리하는 Data 수정"), pCodeData->m_strCode, pCodeData->m_strCode.GetLength());
#endif
		RequestChartData();
	}		
	return TRUE;
}

BOOL CChartMng::SetType(char chType, long lNumber)
{
	// 틱(0)/분(1)/일(2)/주(3)/월(4)/년(5)
	
	if(m_bTypeLink&&!m_bChartExtended)
	{
		InitInLoop();
		m_chTypeInLoop = chType;
		m_lNumberInLoop = lNumber;
		HeadInLoop();
//		long lColSelected = m_lColSelected;
//		long lRowSelected = m_lRowSelected;
//		EACH_CHARTJOB_START()
//			SetChartSelected(CPoint(nColIndex,nRowIndex));
//			// 틱(0)/분(1)/일(2)/주(3)/월(4)
//			GetpChartItemSelected()->SetType(chType);
//			GetpChartItemSelected()->SetTickNumber(lNumber);
//			GetpChartItemSelected()->SetMinNumber(lNumber);
//			RequestChartData();
//		EACH_CHARTJOB_END()
//		SetChartSelected(CPoint(lColSelected,lRowSelected));
	}
	else
	{
		
//		if(GetpChartItemSelected()->GetDataCode()!="")
		{
			if(!GetpChartItemSelected()) return FALSE;

			GetpChartItemSelected()->SetType(chType);
			
			// 2005. 05. 20 -- 일 차트도 간격을 넘긴다.
			if(chType == TICK_DATA_CHART || chType == MIN_DATA_CHART || 
				chType == DAY_DATA_CHART || chType == HOUR_DATA_CHART)
			{
				GetpChartItemSelected()->SetTickMinDayNumber(lNumber);
			}
			//else
			//	GetpChartItemSelected()->SetTickMinDayNumber(1);

			//=====================================
			//05. 10. 12
			//if(!((CStdDialog*)GetParent())->IsSimpleChart())

			if(!((CStdDialog*)GetParent())->IsMultiItemChart())
			{
				m_bChangeTypeMode = TRUE;
			}
			//=====================================

			// 복수종목 추가 : 복수종목 - ojtaso (20070118)
			RequestChartData(TRUE);
		}
	}
	return TRUE;
}

//@080520-004
//-->
void CChartMng::SetType_NoRequest(char chType, long lNumber)
{
	CChartItem* pChartItem = NULL;
	pChartItem = GetpChartItemSelected();
	if(pChartItem == NULL) return;

	pChartItem->SetType(chType);

//-->080529 필요한 동작인가?	
//	if(chType == TICK_DATA_CHART || chType == MIN_DATA_CHART || chType == DAY_DATA_CHART)
//	{
//		pChartItem->SetCntForRequest((UINT)400); //조회 요청 기본값
//		pChartItem->SetTickMinDayNumber(lNumber);
//	}
//	else if(chType == MONTH_DATA_CHART)
//	{
//		pChartItem->SetCntForRequest((UINT)140); //조회 요청 기본값
//	}
//<--
}
//<--


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
//@유진삭제
//				// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)
//				m_bAddModeFromTab = (BOOL)nMode;
//				if(!m_bAddModeFromTab)
//					((CInputBarDlg*)m_pInputBarDlg)->SetMode(0);
//
//				STINPUTCODEDATA *pStInputCodeData = (STINPUTCODEDATA *)lParam;			
//				GetParent()->SendMessage(UMSG_SENDSHAREDCODE,(WPARAM)(LPCTSTR)pStInputCodeData->m_strCode);
//@유진삭제
			}
			break;
		case UMSG_GETJCODECHECKEDFROMCHART:// keep going...			
			break;
		case UMSG_GETINC:
			return 0L; // return 0(send mode)..
			break;
//@유진삭제
//		case UDM_GET_USERID:
//			return (long)(LPCSTR)m_strUserPath;
//			break;		
//@유진삭제

		default:
			return 1L; // don't do antying..
			break;
		}
	}
	CString sTemp;
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
			
//@유진삭제
//			// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)
//			if(nMode == NEW_CHART)
//			{
//				m_nOldMode = CHANGE_ITEM;
//				m_nMode = NEW_CHART;
//				m_bAddModeFromTab = FALSE;
//				if(m_pInputBarDlg)
//					((CInputBarDlg*)m_pInputBarDlg)->SetCurrentCode("");
//			}
//@유진삭제
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
			//GetpChartItemSelected()->SetKorName(CString((const char*)lParam));
			InputCodeFromInputBar((CCodeDataFromInputBar *)wParam);
		}
		break;
//	case UMSG_RECEIVE_MARKETTR:
//		{
//
//		}
//		break;		
	case UMSG_RECEIVEFROMSERVER:
		{
			stReceiveFromServer* pStR = (stReceiveFromServer*)lParam;

			if(!GetpChartItemSelected()) return FALSE;
			if(!pStR->m_dwLength) 
			{
				GetpChartItemSelected()->SetNext(FALSE);

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
			
			((CStdDialog*)GetParent())->SetChartBySystemTrading(FALSE);
			if(m_pInputBarDlg)
				m_pInputBarDlg->SendMessage(UMSG_END_REQUEST_CHARTDATA);
			if(m_pLeftBarDlg)
				m_pLeftBarDlg->SendMessage(UMSG_END_REQUEST_CHARTDATA);
			
			//종목 코드에 포커스가 가도록  한다.
			if(GetBNoExistLoadCnts() && m_bLastEnvUpdated && m_pInputBarDlg)
			{
				m_pInputBarDlg->SendMessage(UMSG_EDIT_FOCUS);
				SetBNoExistLoadCnts(FALSE); //다음에 다시 안 불리게 하기 위해 False로 셋
			}
			//m_pInputBarDlg->SendMessage(UMSG_EDIT_FOCUS);
			//pSelChartItem->SetBEnableIndicatorCalculation(TRUE);
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
//		if(!GetpChartItemSelected()) return FALSE;
//		if(wParam == 1)
//			GetpChartItemSelected()->ShowIndicatorConfigDlgWithName("가격 이동평균");
//		else if(wParam == 2)
//			GetpChartItemSelected()->ShowIndicatorConfigDlgWithName("거래량 이동평균");
//		else
//			GetpChartItemSelected()->ShowIndicatorConfigDlg();
		break;
	case UMSG_SETTYPE:
		{
			char chType = wParam;
			long lNumber = lParam;
			SetType( chType, lNumber);
			break;

//--> 테스트 조회
//			SaveChart(((CStdDialog*)GetParent())->m_strConfig);
//			CString strAppName = "0000"; // 비교차트는 row,col 한개므로.
//			GetpChartItemSelected()->SaveRQInfo(strAppName, ((CStdDialog*)GetParent())->m_strConfig, chType);
//
//			CString szType(chType);
//			::WritePrivateProfileString(strAppName,"Type", szType, ((CStdDialog*)GetParent())->m_strConfig);
//			LoadChart(((CStdDialog*)GetParent())->m_strConfig);
//<--
		}
		break;
//@080520-004
//-->
	case UMSG_SETTYPE_NO_REQUEST:
		{		
			char chType  = (char)wParam;
			long lNumber = (long)lParam;
			SetType_NoRequest( chType, lNumber);			
		}
		break;
//<--
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
			// 복수종목 추가 : 복수종목 - ojtaso (20070118)
			RequestChartData(TRUE);
			GetpChartItemSelected()->SetSaveAllCount(wParam);
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
			//ResetChartAll();
			ResetChart();
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
// 	case UMSG_GETNAMEINDICATOR:
// 		{
// 			GetIndicatorNameListIntoArray((CStringArray *)lParam, (LPCTSTR)wParam);
// 		}
// 		break;
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
			// CString strIndicondFile = m_strRootPath + "\\data\\indicond.cfg";
			CString strIndicondFile = m_strRootPath + "\\Icss\\indicond.cfg";	//@Solomon
			CString strGroup;
			CString strJipyo = (LPCTSTR)wParam;
			::GetPrivateProfileString(strJipyo, "groupName", "", strGroup.GetBuffer(MAX_PATH), MAX_PATH, strIndicondFile);

			//환경동기화 적용 상태인지 확인		05.09.26	
			if( m_pExtInputBarDlg->SendMessage(UMSG_GETENVSINK) == 0 ) //동기화 상태 아님.
			{
				if(nFrom == FROM_INPUTBAR || nFrom == FROM_TABCHART)
				{	
					if(!pSelChartItem->GetDataCode().GetLength()) 
					{
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
//				if(pChartItem->GetDataTRCode()==QUERY_STRJONGMOK)
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
	case UMSG_GETJONGMOKCODE:
		{	
			if(!GetpChartItemSelected()) return FALSE;
			if(GetpChartItemSelected()->GetDataTRCode()==QUERY_STRJONGMOK)
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
			//if(m_nMarketType != CMasterTypeInfo::STOCK_CHART)	
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
//			pSelChartItem->InvokeAddIn("ChartGuideLineAddin.dll:GUIDE_LINE",101,(long)pSelChartItem->GetSafeHwnd());

			
			CStringList strlistSetAll;
			strlistSetAll.AddTail(((CStdDialog*)GetParent())->GetAccountInfo());
			pSelChartItem->InvokeAddIn("ChartGuideLineAddin.dll:GUIDE_LINE",NULL,(long)&strlistSetAll);
			if(strlistSetAll.GetCount())
			{
				CString strbChanged = strlistSetAll.GetHead();
				if(strbChanged=="1")
				{
					RequestAVGForGuideLine(FALSE);
				}
			}
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

			//@.080726-001
			OnAllCodeScdClear();
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
					//pChartItem->SetEndDateRequested(sDate);
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
	case UMSG_CHANGE_COMPARECHART_TYPE:
		{
			char cType[1];
			CString sTemp;
			sTemp.Format("%1.1s", (char*)wParam);
			memcpy(cType, sTemp, 1);
			//cType = (char*)wParam;
			GetpChartItemSelected()->SetCompareChartType(*cType);
			Invalidate();
		}
		break;
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
	}	

	return CWnd::WindowProc(message, wParam, lParam);
}

void CChartMng::GetJongmokCodes(CStringList *plistStrJongmokCode, CHighlightChartInfoList* plistHighlightChartInfo)
{
	stHighlightChartInfo stChartInfo;
	CString strJongmokCode;
	EACH_CHARTJOB_START()
		if(pChartItem->GetDataTRCode()==QUERY_STRJONGMOK)
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

//		long lColSelected = m_lColSelected;
//		long lRowSelected = m_lRowSelected;
//		EACH_CHARTJOB_START()
//			SetChartSelected(CPoint(nColIndex,nRowIndex));
//			GetpChartItemSelected()->SetUseRevisedValue(bUseRevised);
//			if(!bDontRequestData) RequestChartData();
//		EACH_CHARTJOB_END()
//		SetChartSelected(CPoint(lColSelected,lRowSelected));
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

BOOL CChartMng::AddMarketData(CString strCode, BOOL bFromTree)
{

/*	CChartItem* pSelChartItem = NULL;
	pSelChartItem = GetpChartItemSelected();

	if(pSelChartItem == NULL) return FALSE;
	
	
	if(!pSelChartItem->GetCntOfItemData()||pSelChartItem->GetOnRunningOneChart()) 
	{
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
			return FALSE;
		}			
	}
	else
	{
		if(	pSelChartItem->GetTypeRequested() != DAY_DATA_CHART )
		{
			return FALSE;
		}
	}

	if(strCode.Find("뉴스") < 0)  //뉴스 관련 지표가 아니라면 
	{
		if(pSelChartItem->HasChartItemData(strCode,QUERY_STRMARKET,FALSE))
		{
			pSelChartItem->RemoveChartItemData(strCode,QUERY_STRMARKET);
		}
		else
		{
//			//=================================================================
//			//중복해서 같은 시장지표 항목이 TR이 쌓인 경우 없애버린다.
//			//2005.11.01
//			//=================================================================
//			pSelChartItem->RemoveChartItem_AllData(strCode,QUERY_STRMARKET);
//			//=================================================================


			if(!strCode.GetLength()) 
				return FALSE;

			pSelChartItem->SetCodeForQuery(strCode,QUERY_STRMARKET,FALSE);
//			if(m_bLastEnvUpdated) //로드가 끝났을 때 요청한다.
//			{
//#ifdef _DEBUG
//		CString sTemp;
//		sTemp.Format("Query 보냄 %s", strCode);
//		OutputDebugString(sTemp);
//#endif
//				
//			}
//
//			if(m_bMarketJipyoAdding)
//			{
				RequestChartData();
//			}
		}
	}
	else
	{		
		if(pSelChartItem->HasChartItemData(strCode,QUERY_STRMARKET_NEWS,FALSE))
		{
			pSelChartItem->RemoveChartItemData(strCode,QUERY_STRMARKET_NEWS);
		}
		else
		{
			pSelChartItem->SetCodeForQuery(strCode,QUERY_STRMARKET_NEWS,FALSE);
//			if(m_bLastEnvUpdated) //로드가 끝났을 때 요청한다.
//			{
//#ifdef _DEBUG
//		CString sTemp;
//		sTemp.Format("Query 보냄 %s", strCode);
//		OutputDebugString(sTemp);
//#endif
//				//RequestChartData();
//			}
//
//			if(m_bMarketJipyoAdding)
//			{
				RequestChartData();
//			}
		}
	}
//==================================================================================
*/
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

	
	// 호가 창 Show ---------------------------------------------------
	// 2005. 04. 28 by Nam
	//-----------------------------------------------------------------
	//HWND hParent = GetParent()->GetSafeHwnd();		
	//::SendMessage(hParent, UMSG_FORCE_SHOW_HOGAWND, m_bChartExtended, 0 );		
	//-----------------------------------------------------------------
	

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
					SetChartSelected(CPoint(nColIndex,nRowIndex));
					//ResetOCX();
					GetParent()->SendMessage(UMSG_INITIAL_BTNCLICK, 0, 0);
					break;
				case 3:	
					SetChartSelected(CPoint(nColIndex,nRowIndex));		
					SetTimer(1, 100, NULL);										
					break;
//				case 4:	// Check
//					SetChartSelected(CPoint(nColIndex,nRowIndex));	
//					EACH_CHARTJOB_START()
//						if(!GetpChartItemSelected()) break;
//						if(GetpChartItemSelected()!=pChartItem&&GetpChartItemSelected()->GetDataCode()==pChartItem->GetDataCode())
//						{
//							pChartItem->SetCheck(GetpChartItemSelected()->GetCheck());
//						}
//					EACH_CHARTJOB_END()					
//					m_pLeftBarDlg->SendMessage(UMSG_SENDJCODECHECKED,GetpChartItemSelected()->GetCheck(),(LPARAM)(LPCTSTR)GetpChartItemSelected()->GetDataCode());
//					break;
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
	m_pLeftBarDlg =	((CStdDialog*)GetParent())->GetpLeftBarDlg(); //KHD : 간혹 핸들이 사라져서 명시적으로 바꿈. 
	if(m_pLeftBarDlg)// && m_pLeftBarDlg->m_hWnd && IsWindow(m_pLeftBarDlg->m_hWnd))
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

	m_strMultiCodes = strMultiCodes;
	CString strAData;
	CString strJCode;
	CString strTimeMark;
	int nPos = m_strMultiCodes.Find(';',0);	
	strAData = m_strMultiCodes.Left(nPos);
	int nPos1 = strAData.Find(',',0);	
	strJCode = strAData.Left(nPos1);
	strAData.Delete(0,nPos1+1);
	strAData.Remove(':');
	m_strTimeMarkReserved = strAData;
	m_strMultiCodes.Delete(0,nPos+1);
	if(m_pInputBarDlg)
	{
		STINPUTCODEDATA2	stInputCodeData2;
		stInputCodeData2.m_strCode = strJCode;
		//if(stInputCodeData2.m_strCode.GetLength()==CODE_STOCK_LENGTH2)
		//	stInputCodeData2.m_strCode.Format("%s ", strJCode);
		stInputCodeData2.m_bDontPutInShared = TRUE;
		stInputCodeData2.m_bRequestWithoutHesitation = TRUE;
		m_pInputBarDlg->SendMessage(UMSG_INPUTCODETOINPUTBAR,(WPARAM)&stInputCodeData2);
	}
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	복수종목조회시 다음종목으로 조회한다.
//////////////////////////////////////////////////////////////////////////////
BOOL   CChartMng::SetJCodesNext()
{
	if(!m_strMultiCodes.IsEmpty())
	{
		if(m_nMode == CHANGE_CHART)
		{
			// 복수종목 추가 UI : 복수종목 - ojtaso (20070522)
			if(!m_bAddModeFromTab && !SelectNextPos())
			{
				m_strMultiCodes.Empty();
				return FALSE;
			}
		}
		SetMultiCodes(m_strMultiCodes);
		return TRUE;
	}	

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
	if(!pChartItem) return FALSE;

//-->080530-001금 등락률차트일 때 자동으로 세팅이 안되서 호출함.
	if(m_pShareInputData->m_DataInputBar.m_nUnitType!=pChartItem->GetChart_NumericValueType())
		pChartItem->SetChartNumericValue(m_pShareInputData->m_DataInputBar.m_nUnitType);

	pChartItem->SetNew_OnePageDataCount(m_pShareInputData->m_DataInputBar.m_lDispCnt);
	if(m_pInputBarDlg)
	{
		CInputBarDlg* pInputBarDlg = (CInputBarDlg*)m_pInputBarDlg;
		pInputBarDlg->SetMode(pInputBarDlg->GetCurrentMode());
	}
//<--

	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	CString strPacket = pChartItem->IsLoadingRQ() ? pChartItem->GetLoadedQueryData() : pChartItem->GetQueryData(bChangeType);

//	OutputDebugString("Send" + strPacket);

	if(strPacket=="")	
	{
		return FALSE;
	}

	stSend2Server st;
	st.m_nType = 0;
	st.m_pWnd = this;
	st.m_pBuffer = (void*)(LPCTSTR)strPacket;
	st.m_lLength= strPacket.GetLength();
	st.m_strTRCode = GetpChartItemSelected()->GetTrCodeForQuery();

#ifdef _WRITELOG
	// Log - ojtaso (20080107)
	((CStdDialog*)GetParent())->WriteToStringLog(_T("서버에게 보낼 Data 가공"), pChartItem->GetSelectedItemCode(), pChartItem->GetSelectedItemCode().GetLength());
#endif
//	CRect rect;
//	GetWindowRect(rect);
//	InvalidateRect(rect);
	GetParent()->SendMessage(UMSG_SEND2SERVER,0,(long)&st);	

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2004.06.04
//	목적	:	차트내의 실시간 정보를 처리한다.
//////////////////////////////////////////////////////////////////////////////
void CChartMng::SetRealJPriceData(void *pRealData)
{
	CString szTemp0;

	// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
//	CRealUpdateData* cReal = (CRealUpdateData*)pRealData;
//	szTemp0 = cReal->m_strMainKeyVal;

	CRealData_Common* pRealData_Common = (CRealData_Common*)pRealData;
	
	// 장시작구분값이 장중이 아니면 실시간 처리를 안하게끔 처리.
	if (pRealData_Common->m_bIsRealJang == FALSE)
		return;
// 	if(  pRealData_Common->m_strPrice.IsEmpty()==TRUE 
// 		||pRealData_Common->m_strValue.IsEmpty()==TRUE )
// 		return;

	EACH_CHARTJOB_START()
		if(/*pChartItem->GetDataCode()==szTemp0&&*/pChartItem->IsOnDrdsAdvise())			
		{
			int nMarketType = ((CStdDialog*)GetParent())->m_nMarketType;
			if(nMarketType == CMasterTypeInfo::STOCK_CHART)
				pChartItem->SetRealPriceData( pRealData, STOCK_REAL);
			else if(nMarketType == CMasterTypeInfo::UPJONG_CHART)
				pChartItem->SetRealPriceData( pRealData, UPJONG_REAL);
			else if(nMarketType == CMasterTypeInfo::FUTOPT_CHART)
				pChartItem->SetRealPriceData( pRealData, FUTURE_REAL);
			else if(nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
				pChartItem->SetRealPriceData( pRealData, FUTURE_REAL);
			else
				pChartItem->SetRealPriceData( pRealData, UPJONG_REAL);
		}
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
	((CStdDialog*)GetParent())->OnCodeAdvise(pItem->GetDataCode(), bAddMode);
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

//-->@.080728-001 조회시작일 종가저장.
void CChartMng::OnChartMngCodeScdClear(CChartItem *pItem, BOOL bAddMode)
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

	CString strOldCode = pItem->GetOldChartCode(); strOldCode.TrimLeft(); strOldCode.TrimRight();
	if(strOldCode.GetLength())
		((CStdDialog*)GetParent())->OnCodeScdClear(strOldCode, bAddMode);
}
//<--

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

//-->@.080728-001 조회시작일 종가저장.
void CChartMng::OnAllCodeScdClear()
{
	EACH_CHARTJOB_START()
		if(pChartItem!=NULL)			
		{
			if(pChartItem->GetDataCode())
			{
				((CStdDialog*)GetParent())->OnCodeScdClear(pChartItem->GetDataCode(), pChartItem->IsAddMode());
			}			
		}
	EACH_CHARTJOB_END()
}
//<--

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
	//		CString strCompareCharts = m_strUserPath + "\\*.mct";
	//				
	//		BOOL bResult = ff.FindFile(strCompareCharts);
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
	if(bInRectOfChart&&GetpChartItemSelected()->GetDataTRCode()==QUERY_STRJONGMOK)
	{
		STSHAREDDATA stInfo;
		stInfo.m_szKey = "코드-코코2";
		stInfo.m_szValue = GetpChartItemSelected()->GetDataCode();
		stInfo.m_szValue.TrimRight(';');
		//((IMainDllObMain*)g_pMainDll)->m_pMainFrame->SendMessage(RMSG_CREATEOPENSHAREDATA,0, (LPARAM)&stInfo);		
		CWnd::OnRButtonUp(nFlags, point);
	}
}

//@Solomon 091007-Alzioyes
void CChartMng::RequestAVGForGuideLine(BOOL bSelectedOnly)
{
// 	if(!GetpChartItemSelected()) return ;
// 
// 	CUIntArray AryAvgForChartGuideLine;
// 	AryAvgForChartGuideLine.SetSize(4);
// 	GetpChartItemSelected()->InvokeAddIn("ChartGuideLine.dll",INVOKEMSG_GETAVGINFO,(long)&AryAvgForChartGuideLine);
// 	stSend2Server st;
// 	CString strPacket;
// 	if(bSelectedOnly)
// 	{
// 		strPacket = GetpChartItemSelected()->GetQueryDataForGuideLine(&AryAvgForChartGuideLine);
// 		if(strPacket=="")	return;	
// 		st.m_nType = 0;
// 		st.m_pWnd = GetpChartItemSelected();
// 		st.m_pBuffer = (void*)(LPCTSTR)strPacket;
// 		st.m_lLength= strPacket.GetLength();
// 		st.m_strTRCode = QUERY_STR_AVGMP;
// 		GetParent()->SendMessage(UMSG_SEND2SERVER,0,(long)&st);
// 	}
// 	else
// 	{
// 		EACH_CHARTJOB_START()
// 			strPacket = pChartItem->GetQueryDataForGuideLine(&AryAvgForChartGuideLine);
// 			if(strPacket=="")	continue;	
// 			st.m_nType = 0;
// 			st.m_pWnd = pChartItem;
// 			st.m_pBuffer = (void*)(LPCTSTR)strPacket;
// 			st.m_lLength= strPacket.GetLength();
// 			st.m_strTRCode = QUERY_STR_AVGMP;
// 			GetParent()->SendMessage(UMSG_SEND2SERVER,0,(long)&st);
// 		EACH_CHARTJOB_END()	
// 	}
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
// 		//-----------------------------------------------------------------
// 		// Han Hwa 지표구조에 맞게 변경한다.
// 		//-----------------------------------------------------------------
// 		/*char* szIndexGroupName[] = { 
// 										"기본지표",
// 										"이동평균",
// 										"주가지표",
// 										"기타지표",
// 										"첨부지표",
// 										"추세지표",
// 										"거래량지표",
// 										"변동성지표",
// 										"탄력성지표"};
// 		//-----------------------------------------------------------------*/
// 		char* szIndexGroupName[] = { 
// 										//"전체지표",
// 										"가격지표",
// 										"추세지표",
// 										"변동성지표",
// 										"거래량지표",
// 										"2차파생지표",
// 										"기타지표",
// 										"시장지표",
// 										""
// 		};
// 
// 		long lAllCnt = sizeof(szIndexGroupName)/sizeof(char*);
// 
// 	//#ifdef _DEBUG
// 		//=============================================================
// 		// 05.10.24
// 		// 주식만 시장 지표 추가 
// 		//=============================================================
// 		if(m_nMarketType != CMasterTypeInfo::STOCK_CHART)
// 			lAllCnt -= 1;
// 		//=============================================================
// 	//#endif
// 
// 		rxtreeCtrl.DeleteAllItems();
// 		CStringArray strArrayIndicator;	
// 		long lSize = 0;
// 		HTREEITEM hRootItem = NULL;
// 		for(long lIndex = 0;lIndex<lAllCnt;lIndex++)
// 		{
// 			hRootItem = rxtreeCtrl.InsertItem(szIndexGroupName[lIndex]);
// 			GetIndicatorNameListIntoArray(&strArrayIndicator, szIndexGroupName[lIndex]);
// 			lSize = strArrayIndicator.GetSize();
// 			for(long lPos=0;lPos<lSize;lPos++)
// 			{
// 				rxtreeCtrl.InsertItem(strArrayIndicator.GetAt(lPos),hRootItem);
// 			}		
// 		}
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
	return;
// 		//@비교차트에서 사용할 부분이 있나???
// 
// 		if(p_strGroup[0]!=NULL)
// 		{
// 			CString strALine = GetIndicatorNameList(p_strGroup);
// 			CString strAItem;
// 			int nPos = 0;
// 			int nItem = 0;
// 			int nLength = 0;
// 			rComboBox.Clear();
// 				//rxlistCtrl.DeleteAllItems();
// 			if(!strcmp(p_strGroup,"특수지표"))
// 			{
// 	//			OutputDebugString("집어 넣는다.");
// 
// 
// 
// 				rComboBox.AddString("캔들차트");
// 				rComboBox.AddString("바차트");
// 				rComboBox.AddString("선차트");
// 				//rComboBox.AddString("점차트");
// 				//rComboBox.AddString("렌코차트");
// 				//rComboBox.AddString("삼선전환도");
// 				//rComboBox.AddString("이큐볼륨차트");
// 				//rComboBox.AddString("캔들볼륨차트");
// 				//rComboBox.AddString("P&F차트");
// 				
// 				/*
// 				rxlistCtrl.InsertItem(0, "");
// 				rxlistCtrl.SetItemText(0, 0, "캔들차트",RGB(0,0,0),CRGRID);
// 				rxlistCtrl.InsertItem(1, "");
// 				rxlistCtrl.SetItemText(1, 0, "바차트");
// 				rxlistCtrl.InsertItem(2, "");
// 				rxlistCtrl.SetItemText(2, 0, "선차트",RGB(0,0,0),CRGRID);
// 				*/
// 				nItem = 3;
// 			}
// 
// 			if(strALine.GetLength())
// 			{
// 				while(1)
// 				{
// 					nLength = strALine.GetLength();
// 					if(!nLength) break;
// 					nPos = strALine.Find(',');
// 					if(nPos==-1) 
// 					{
// 						strAItem = strALine;
// 						strALine.Empty();
// 					}
// 					else
// 					{
// 						strAItem = strALine.Left(nPos);
// 						strALine.Delete(0,nPos+1);
// 					}
// 
// 					if(strAItem=="P&&F")
// 					{
// 						strAItem = "P&F";
// 					}
// 					rComboBox.AddString(strAItem);
// 
// 					/*
// 					if(strAItem=="P&F")
// 					{
// 						strAItem = "P&&F";
// 					}
// 					rComboBox.AddString(strAItem);
// 					
// 					rxlistCtrl.InsertItem(nItem, "");
// 					if(nItem%2) // odd
// 					{					
// 						rxlistCtrl.SetItemText(nItem, 0, strAItem);
// 					}
// 					else		// even
// 					{
// 						rxlistCtrl.SetItemText(nItem, 0, strAItem,RGB(0,0,0),CRGRID);					
// 					}*/				
// 					nItem++;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			CString strList = GetIndicatorNameList("");
// 			CString strALine;
// 			CString strALineTitle;
// 			CString strAItem;
// 			int nPos = 0;
// 			int nItem = 0;
// 			int nLength = 0;
// 
// 			rComboBox.Clear();
// 			rComboBox.AddString("거래량차트");
// 	//		rxlistCtrl.DeleteAllItems();
// 	//		rxlistCtrl.InsertItem(nItem, "");
// 	//		rxlistCtrl.SetItemText(nItem, 0, "거래량차트",RGB(0,0,0),CRGRID);
// 			nItem++;
// 
// 			if(strList.GetLength())
// 			{
// 				while(1)
// 				{
// 					nPos = strList.Find(':');
// 					if(nPos==-1) break;		
// 					if(nPos==0) 
// 					{
// 						nPos = strList.Find('\n');
// 						strList.Delete(0,nPos+1);
// 						continue;
// 					}			
// 					strALineTitle = strList.Left(nPos);
// 					if(strALineTitle=="기본지표"
// 						||strALineTitle=="시장지표"
// 						||strALineTitle=="투자자동향"
// 						||strALineTitle=="특수지표")
// 					{
// 						nPos = strList.Find('\n');
// 						strList.Delete(0,nPos+1);
// 						continue;
// 					}
// 
// 					strList.Delete(0,nPos+1);
// 					nPos = strList.Find('\n');
// 					strALine = strList.Left(nPos-1);		
// 					strList.Delete(0,nPos+1);
// 					
// 					while(1)
// 					{
// 						nLength = strALine.GetLength();
// 						if(!nLength) break;
// 						nPos = strALine.Find(',');
// 						if(nPos==-1) 
// 						{
// 							strAItem = strALine;
// 							strALine.Empty();
// 						}
// 						else
// 						{
// 							strAItem = strALine.Left(nPos);
// 							strALine.Delete(0,nPos+1);
// 						}
// 						rComboBox.AddString("거래량차트");
// 	//					rxlistCtrl.InsertItem(nItem, "");
// 	//					if(nItem%2) // odd
// 	//					{
// 	//						rxlistCtrl.SetItemText(nItem, 0, strAItem);
// 	//					}
// 	//					else		// even
// 	//					{
// 	//						rxlistCtrl.SetItemText(nItem, 0, strAItem,RGB(0,0,0),CRGRID);					
// 	//					}				
// 						nItem++;
// 					}
// 				}		
// 			}
// 		}
// 
// 		rComboBox.SetCurSel(0);
}

void CChartMng::SetParentCompareChartClassPointer(long clsPointer)
{
	m_lMltchrtDlgParentClsPointer = clsPointer;
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
			if(m_nMarketType == CMasterTypeInfo::FUTOPT_CHART || m_nMarketType == COMMODITY_CHART)
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
//					pChartItem->m_strReplayTime = _T("복기가 되지 않는 종목입니다");					
//					pChartItem->SetReplayChart_RealData("복기가 되지 않는 종목입니다");
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
		for(long lPos = 0;lPos<(nlCntForSyncEnv-1);lPos++)
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

		if(m_pChartItems[0][0]->GetSafeHwnd())
		{
			pChartItem = m_pChartItems[0][0];
			pChartItem->SetType(DAY_DATA_CHART);
		}

		if(m_pChartItems[0][1]->GetSafeHwnd())
		{
			pChartItem = m_pChartItems[0][1];
			pChartItem->SetType(WEEK_DATA_CHART);
		}

		if(m_pChartItems[1][0]->GetSafeHwnd())
		{
			pChartItem = m_pChartItems[1][0];
			pChartItem->SetType(MONTH_DATA_CHART);
		}

		if(m_pChartItems[1][1]->GetSafeHwnd())
		{
			pChartItem = m_pChartItems[1][1];
			pChartItem->SetType(MIN_DATA_CHART);
		}
	}
	else
	{
		SetMatrix(1,1);

		if(m_pChartItems[0][0]->GetSafeHwnd())
		{
			pChartItem = m_pChartItems[0][0];
			pChartItem->SetType(DAY_DATA_CHART);
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

int CChartMng::GetRQCount()
{
	CChartItem* lpSelItem = GetpChartItemSelected();
	if(lpSelItem)
		return lpSelItem->GetRQCount();
	else 
		return 0;
}

BOOL CChartMng::fnMenuLoad(BOOL bOnlyCheck, int nScreenType)
{
	CPoint pt, ps;
	GetCursorPos(&ps);
	pt = ps;
	ScreenToClient(&pt);

	CRect rect;
	GetClientRect(rect);
	if(rect.PtInRect(pt))
	{
		if(!bOnlyCheck) ((CInputBarDlg*)m_pInputBarDlg)->ShowScreen1Menu(ps, nScreenType);
		return TRUE;
	}

	return FALSE;

//	CMenu menu;
//    menu.CreatePopupMenu();
//
//	int nIndex=0;
//	menu.AppendMenu(MF_STRING,(UINT)1000+nIndex,"설정");
//
//	UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
//
//	//CPoint ps;
//	//GetCursorPos(&ps);
//
//	int nRetValue = menu.TrackPopupMenu(nFlagsForMenu, ps.x, ps.y, this);
//
//	if(!nRetValue) return;
//	menu.DestroyMenu();
}

LRESULT CChartMng::OnRmsgSelChartCode(WPARAM wparam, LPARAM lparam)
{
	CChartItem* pSelChartItem = GetpChartItemSelected();
	if(pSelChartItem)
	{
		return (LRESULT)pSelChartItem->GetNewRequestCode();
//		if(pSelChartItem->m_lpSelChart)
//			return (LRESULT)(LPCSTR)pSelChartItem->m_lpSelChart->m_strItemCode;			
	}
	return NULL;
}
