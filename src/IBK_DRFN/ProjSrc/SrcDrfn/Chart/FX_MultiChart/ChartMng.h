#if !defined(AFX_CHARTMNG_H__FD135639_7D01_492D_BFC5_AF011DA6BD62__INCLUDED_)
#define AFX_CHARTMNG_H__FD135639_7D01_492D_BFC5_AF011DA6BD62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChartMng.h : header file
//
#include <afxtempl.h>
#include "./control/XListCtrl.h"

#ifndef __HANHWA_CHART_REPLAY_INFO_H
	#include "../chart_common/ReplayChartInfoDef.h"
#endif

//<<20100308_JS.Kim 솔로몬
#include "../COMMONTR/OAQ69901_io.h"
//>>

//#include "MFECToolTip.h"

/////////////////////////////////////////////////////////////////////////////
// CChartMng window
#define CNTOFMAXROW			(20)
#define CNTOFMAXCOL			(20)
#define	RESIZEBOTH			(0)
#define	RESIZEROW			(1)
#define	RESIZECOL			(2)

#define HEIGHTOFCAPTION		(18)
#define GAPOFCHARTS			(1)


#include "ChartItem.h"


class CCodeDataFromInputBar;
class CFloatingHogaDlg;
class CChartMng : public CWnd
{

//20080914 이문수 >>
public:	

	void SaveChartContainerToImg();

	void PrintChartContainer();

	void Paint(CDC * pDC, BOOL bPrint=FALSE);
	
	BOOL GetDefaultOption(CString& szPaperSize, CString& szWay, CString& szOrientation);

	BOOL SetDefaultOption(CString szPaperSize, CString szWay, CString szOrientation);

	BOOL SetDefaultPrinter(CString printer, CString driver, CString port);

//20080914 이문수 <<

public:
	BOOL CheckDataTypeForReplay();
	//>>>>>>>------------------------------------------------------------------------
	//Author	:	Sang Yun Nam
	//Date		:	2005.11.24
	//Reason	:   외부툴바로 부터 초기화 버튼이 눌릴경우 TR을 재조회 하는데..
	//				이때 추가모드일경우 새로운 차트가 추가되는 경우가 발생하여 
	//				이를 플래그로 구분하기 위해 추가한다.
private:
	BOOL m_bInitFromExtToolBar;
public:
	void SetBInitFromExtToolBar(BOOL bInit)	{ m_bInitFromExtToolBar = bInit;}
	BOOL GetBInitFromExtToolBar()			{ return m_bInitFromExtToolBar;	}
	//<<<<<<<<<----------------------------------------------------------------------

//Start ------------------------------------------------------------------------
	//종목 코드 입력란에 포커스가 가도록 하기 위해 아래와 같은 내용들을 추가한다.
	// m_bNoExistLoadCnts 는 화면이 처음 로드될 경우 
	// 저장된 파일에서 읽을 내용이 전혀 없을 경우 
	//
	//Author	:	Sang Yun Nam
	//Date		:	2005.11.15
private:
	BOOL m_bNoExistLoadCnts ; // 종목 코드에 포커스가 가기위해 추가된 변수
public:
	BOOL GetBNoExistLoadCnts()				{ return m_bNoExistLoadCnts	   ; }
	void SetBNoExistLoadCnts(BOOL bNoExist) { m_bNoExistLoadCnts = bNoExist; }
//End ----------------------------------------------------------------------
public :
	//BOOL MarketTRSend_By_OCXSignal(CString strCode, UINT nID); //시장 지표 TR 전송 전용..
	BOOL MarketTRSend_By_OCXSignal(int nID, const LPCTSTR pcszPacketNames, int nCount, const BOOL bIsNew); //시장 지표 TR 전송 준비
	BOOL RequestChart_MarketData(CString strTRCode, CString strJipyoName,  CChartItem* pSelChartItem, int nDataRqst, BOOL bIsNew); //시장 지표 TR send
//	BOOL RequestChart_MarketDataMore(CChartItem* pSelChartItem); //연속 조회


	BOOL m_bMarketJipyoAdding; //지표탭에서 시장지표 항목을 더블클릭 하여 들어올 경우..TRUE로 셋팅 

	BOOL m_bAddingMarketData;// 시장지표추가
	BOOL m_bNeedForceCode;	 // 공유가 안 들어올 경우 강제로 종목코드를 설정하여 돌린다.
	void SetReplayRealTitle(CString strCode, CString strSpeed, TICK_DATA* pData);
	void SetReplayChangedDate(CString strCode, CString strBojoMsg); //복기 리얼을 처리하는 도중 날짜가 바뀔경우 ChartOCX에 알려준다.

private:
	BOOL m_bReplaychartMode;
public:
	BOOL GetBReplayChartMode() { return m_bReplaychartMode; }
	void SetBReplayChartMode(BOOL bReplayMode);// { m_bReplaychartMode = bReplayMode;	}
	void SetBTypeLink(BOOL bTypeSync)		   { m_bTypeLink = bTypeSync;			}	// 시간 설정연동 여부.
	
private:
	// Start =====================================================================================
	// 틱/초/분/일/주/월/년 의 기준이 되는 차트의 각 Cwnd* 를 저장한다.
	BOOL	m_bExistSyncChart; // 기준이 될 저장된 지표 파일이 존재 하는지 여부.
public:
	CWnd* m_pTickCfgChart;
	CWnd* m_pSecCfgChart;
	CWnd* m_pMinCfgChart;
	CWnd* m_pDayCfgChart;
	CWnd* m_pHourCfgChart;
	CWnd* m_pWeekCfgChart;
	CWnd* m_pMonthCfgChart;
	CWnd* m_pYearCfgChart;
	

	BOOL GetbExistSyncChart()			{ return m_bExistSyncChart	;}
	BOOL SetbExistSyncChart(BOOL bExist){ m_bExistSyncChart = bExist;}


	CChartItem* GetpChartCfgWnd(char chType);
	void SetpChartCfgWnd(char chType, CWnd* pWnd);
	
	// 종목변경 및 주기변경시...
	void Synchro_jipyo();
	
	// End =======================================================================================

private:
	//==========================================================================================
	// 환경동기화 토글로 설정되어 있는 경우
	// 신규로 데이터를 조회할때..데이터 시간단위가 동일한
	// 차트를 찾아서 해당 정보를 기준으로 동기화 한다.
	//==========================================================================================

	BOOL	m_bEmptyDataChart;
	BOOL	m_bEmptyChartForSinkCfg;	
public:
	BOOL GetbEmptyDataChart()			{ return m_bEmptyDataChart	;}
	BOOL SetbEmptyDataChart(BOOL bEmpty){ m_bEmptyDataChart = bEmpty;}
	void ApplySyncEnv_OnlyOne()	;
	//==========================================================================================
public:
	//===========================================================================================
	// 05. 09. 13 종료시 자동 저장된 차트 로드시 기존 저장된 항목중 0행 0열의 값은 
	// 공유로 들어오는 종목으로 대체하기 위해서
	// Static으로 종목 공유 Count를 세어서 1번째 인 놈만 적용한다.
	static int		m_nStatic_RecvCntShareCode;
	static CString  m_strStatic_ShareCode;
	//===========================================================================================
	static CString  m_strCodeForMulti;


	//===========================================================================================
	// 05. 09. 12 실봉 허봉 하나만 적용토록 한다.
	BOOL UpdateShowingFacked_JustOneChart(BOOL bNormalizedMinuteTimeRule,BOOL bDontRequestData);
	//===========================================================================================


	//2005. 08. 25 add by sy, nam ===================================================
	void GetStartEndDateAtChartMng(double& dStartDate, double& dEndDate);
	//===============================================================================


	//2005. 08. 25 add by sy, nam ===================================================
	//하나의 차트만 보일경우 호가창을 보인다.
	CRect*			m_pRectFloatingHoga;
	CFloatingHogaDlg *m_pFloatingDlg;
	void CreateFloatingHogaWnd(BOOL bForce = FALSE);
	void DestroyFloatingHogaWnd();
	//===============================================================================

	//===============================================================================
	//차트 환경 연동
	void	SetTimeShareOnlyOne				(BOOL bUseage				);	//시간공유
	void	SetEnableBoundMarkOnlyOne		(BOOL bUseage				);	//상하한표시
	void	SetShowExRightOnlyOne			(BOOL bUseage				);	//락 표시
	void	SetPriceLog						(BOOL bLog					);	//로그 표시
	void	SetAutoSaveToOutside			(const bool bAutoSave		);	// 차트종료시 자동 저장 유무	
//	void	SetShowCurrentPriceToOutside	(const bool bShow			);	// 차트에 현재가 표시
	
	bool	GetImaginaryCandleToOutside		(bool& bImaginaryCandle		);	// 허수봉설정.
	bool	GetShowHighLowBoundToOutside	(bool& bShow				);	// 상/하한가 표시
	bool	GetShowExRightDividendToOutside	(bool& bShow				);	// 권리락/배당락 표시
	bool	GetTimeLinkageToOutside			(bool& bTimeLinkage			);	// 시간연동 수신
	bool	GetAutoSaveToOutside			(bool& bAutoSave			);	// 차트종료시 자동 저장 유무
//	bool	GetShowCurrentPriceToOutside	(bool& bShow				);	// 차트에 현재가 표시



	void OnChartEnvSetupOK();
	void SetAllChartCfgSync();		//환경동기화 반응
	//===============================================================================




	//================================================================================
	// 현재 멀티차트 종목 변경시 어떻게 반응할지의  Mode값이 저장된다.	
	// 2005. 08. 11
	//CHANGE_CHART			0		//전환모드
	//ADD_NEW_CHART			1		//추가모드
	//REPLAY_CHART			2		//복기모드
	//OVERLAPP_CHART		3		//중첩모드
	void SetMode(int nMode) {m_nMode = nMode;}
	int  GetMode()			{return m_nMode; } //2005. 08. 11
	//================================================================================


	//2005. 08.02  모든 차트의 리얼 해제 -----------------------------
	void OnAllCodeUnAdvise();

	//2005. 08.02 모든 차트 OCX의 종목 리스트 Get --------------------
	void GetChartAllCode(CStringArray &arrCode);

	//2005. 06.24  DBMgr Set -----------------------------
	//CDrMLib_CDBMgr*	m_pDBMgr;
	//void				SetpDBMgr(CDrMLib_CDBMgr* pDBMgr){ m_pDBMgr = pDBMgr;}	
	//CDrMLib_CDBMgr*	GetpDBMgr()				{return m_pDBMgr;	 }
	//--------------------------------------------------------------------------

	//2005. 06.03 CStdDialog의 부모클래스 관련 -----------------------------
	long m_lMltchrtDlgParentClsPointer; 
	void SetParentMultiChartClassPointer(long clsPointer);
	//--------------------------------------------------------------------------

	//2005. 05.24 public으로 뺐음.
	CChartItem* GetpChartItemSelected();	

	//2005.05.16 add by Nam ----------------------------------------------------
	//차트 구분 주식, 선물옵션, 업종 차트 
	int		m_nMarketType;
	void	SetMarketType(int nMarketType);
	void	ReplaceOldCode();
	BOOL	m_bOnEventMessage;
	//--------------------------------------------------------------------------

	CString m_strRootPath;
	// 2005. 04. 25 -- Sang Yun, Nam ==== Get 지표, 차트 리스트 
	void GetIndicatorNameListIntoComboBox(CComboBox& rComboBox, LPCTSTR p_strGroup);
	

	CChartMng();
	void SetBaseDesk(long lKey);
    BOOL Create(const RECT& rect, CWnd *pParentWnd, LPCTSTR lpUserPath, LPCTSTR m_szImagePath, UINT nID, long clsPointer);
	void SetpInputBarDlg(CWnd *pWnd) { m_pInputBarDlg = pWnd;}
	void SetpExtInputBarDlg(CWnd *pWnd) { m_pExtInputBarDlg = pWnd;}
	void SetpLeftBarDlg(CWnd *pWnd) { m_pLeftBarDlg = pWnd;}
	void GetIndicatorNameListIntoListCtrl(CXListCtrl& rxlistCtrl, LPCTSTR p_strGroup);
	void GetIndicatorNameListIntoTreeCtrl(CTreeCtrl& rxtreeCtrl);
	void GetIndicatorNameListIntoArray(CStringArray *pStrArray, LPCTSTR p_strGroup);
	long GetCols() { return m_lCols;}
	long GetRows() { return m_lRows;}
	long GetCntOfChart() { return (m_lCols*m_lRows); }	
	CChartItem* GetChartItem(int row, int col) { return m_pChartItems[row][col]; }	
	CRect GetRectOfChart(int nRowIndex,int nColIndex,int nTitle = 0); // nTitle 0:Chart, 1:Title, 2:Chart+Title
	
	
	
	//2005. 08. 26 BOOL bLbuttonDown  추가 
	//마우스로 선택시 발생하는 이벤트 에 대해서만 처리하려고 수정함.
	// sy, nam
	BOOL m_bLBDnFromChart;  //차트 아이템에서 LButtonDown시 
	void SendInitChartData(BOOL bNotCodeInputInit = TRUE, BOOL bFromTRData = FALSE); 
	//====================================================================================================

	void SetRealJPriceData(void *pRealData);
	void SetRealUPriceData(char *pData, int nSize);
	void SetRealFPriceData(char *pData, int nSize);
	void SetRealOPriceData(char *pData, int nSize); //옵션		-- 신규 추가 05.06.21 by sy,nam
	void SetRealHPriceData(char *pData, int nSize); //해외지수  -- 신규 추가 05.06.21 by sy,nam
	

	BOOL SelectNextPos();
	BOOL SelectEmptyPos();
	BOOL InsertChart();
	//void MoveCharts();
	void MoveCharts(int nRowIndexOutside= -1,int nColIndexOutside = -1); // parameter is added to move one chart gm0604 2005.10.10
	//void SetChartSelected(CPoint szNewPos,BOOL bInvalidateEvenIfPosisequal =FALSE);
	void SetChartSelected(CPoint szNewPos,BOOL bInvalidateEvenIfPosisequal =FALSE, BOOL bQueryData=FALSE);
	BOOL IsAdvisingCodeOf(LPCTSTR lpCode);
	void SetMultiCodes(CString strMultiCodes);	
	// 복수종목 실시간 처리 : 복수종목 - ojtaso (20070109)
	void OnChartMngCodeAdvise(CChartItem *pItem, BOOL bAddMod);
	// 복수종목 실시간 처리 : 복수종목 - ojtaso (20070109)
	void OnChartMngCodeUnAdvise(CChartItem *pItem, BOOL bAddMod);
	CWnd *GetpInputBarDlg() { return m_pInputBarDlg;}
	CWnd *GetpExtInputBarDlg() { return m_pExtInputBarDlg;}
	void SetSharedTime(LPCTSTR lpSharedTime) { m_strSharedTime = lpSharedTime;}
	CString GetSharedTime() { return m_strSharedTime;}
	CWnd *GetpLeftBarDlg() { return m_pLeftBarDlg;}
	void UnhighlightJCode(CChartItem *pChartItemIn);
	void SetEndData(LPCSTR szMessage);
	BOOL	IsToBeReceivedMore() 
	{
		if(m_strMultiCodes.IsEmpty())
			return FALSE;
		return TRUE;
	}

	// gm0604 added to Update System Trading Data (2005.7.21)
	BOOL OnSTDataManagerMessaggeAll();
	// gm0604 added 2005/10/06
	void SetSystemTradingMode(BOOL bEnable, LPCTSTR lpMessage = NULL);		// gm0604 added 2005/8/23

	void SetTimeMark(LPCTSTR p_szMarkTime, BOOL p_bWithScroll);

	//저장파일 불러오기 전 코드 저장함. 저장 파일 코드 무시하고 이 코드로 대체
	CString  m_strCodeForLoadChart;
	long ResetChart();
	void ResetOCX();
private:
	// gm0604 added to Update System Trading Data (2005.10.06)
	BOOL m_bIsSystemTradingMode;
	// gm0604 added to Update System Trading Data (2005.10.06)
	void GetJongmokCodes(CStringList *plistStrJongmokCode,CHighlightChartInfoList* plistHighlightChartInfo);
	void SendJongmokCodesToLeftBar();
	BOOL ReplaceChart(long lRow1,long lCol1, long lRow2, long lCol2);
	CString GetIndicatorNameList(LPCTSTR p_strGroup);
	BOOL SetMatrix(long lRows,long lCols);
//	long ResetChart();
//	void ResetOCX();
	BOOL SetType(char chType, long lNumber);
	
	BOOL InputCodeFromInputBar(CCodeDataFromInputBar* pCodeData,BOOL bForcedMode0 = FALSE);
	BOOL AddMarketData(CString strCode, BOOL bFromTree = FALSE);
	BOOL ShowScale();
	BOOL ShowInnerToolbar();
	BOOL UpdateRevisedValue(BOOL bUseRevised,BOOL bDontRequestData);

	//05.06.22 add by sy, nam
	BOOL UpdateNumericChartValue(int nNumericChartValue, BOOL bDontRequestData);

	BOOL UpdateShowingFackedCandle(BOOL bShowFackedCandle, BOOL bDontRequestData);
	BOOL UpdateIndex();
	BOOL SetJCodesNext();
	void PaintTitle(int nRowIndex,int nColIndex,CChartItem* pChartItem,CDC *pDC);
	BOOL Rearrange(long lCnt,int nType);
	


	void ExtendChart(BOOL bExtend = TRUE);
	void LoadChart(CPoint *pPt);
	void SaveChart(CPoint *pPt);
	void LoadSaveChart(CPoint *pPt);
public:
	// 복수종목 추가 : 복수종목 - ojtaso (20070118)
	BOOL RequestChartData(BOOL bChangeType = FALSE);
	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	BOOL RequestLoadedChartData();

	BOOL LoadChart(CString strLoadFile); // 05.09.07 차트 로드시 공유로 들어오는 종목을 처리하기 위해 Return형을 void->BOOL 로 한다.
	void SaveChart(CString strSaveFile);

	// 새로운 차트 추가 : 복수종목 - ojtaso (20070108) 
	void SetAddMode(BOOL bAddMode);
	BOOL GetAddGraphMode() { return (m_bAddMode && !m_bSelChangeChart) || m_bAddModeFromTab; }
	BOOL GetAddBlockMode() { return m_bAddMode || m_bAddModeFromTab; }
	// m_bAddMode : UMSG_CHANGEMODE 를 통해 셋팅되며 CHANGE_ITEM일 경우 False
	// m_bSelChangeChart : 중첩을 위한 옵션 False (CHANGE_ITEM이 되도록 할 것인가 의미)

	BOOL GetAddModeFromTab() { return m_bAddModeFromTab; }
	void SetAddModeFromTab(BOOL bAddModeFromTab) { m_bAddModeFromTab = bAddModeFromTab; }

	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	void AddRealCodeList(LPCTSTR lpszCode);
	void AddRealCodes();

	// 멀티차트의 선택여부 : 복수종목 - ojtaso (20070525)
	void SetSelChangeChart(BOOL bSelChangeChart);

private:
	void SetTimeShareAll(BOOL bUseage);
	void SetEnableBoundMarkAll(BOOL bUseage);
	void RequestAVGForGuideLine(BOOL bSelectedOnly);
	void SetSendJongData(LPCTSTR lpQuoteCode,char chType, LPCTSTR lpMinNTickNumber);


// Attributes

public:
	//멀티차트를 실행할때 전에 최종적으로 저장되었던 환경정보를 사용해서 초기 환경설정해주는 플래그에요
	//예전에 보고있던 지표정보라든가 주기 기간등이 저장되었다가 그 정보 불러서 적용하는거죠
	BOOL	m_bLastEnvUpdated;
	BOOL	m_bChangeTypeMode;			// 새로운 차트 추가 : 복수종목 - ojtaso (20070108) 
	// m_bChangeTypeMode : CHANGE_ITEM이 되도록 할 것인가 의미

private:
	long	m_lInitialZoomValue;
	BOOL	m_bUpdatingRevisedValue;
	BOOL	m_bUpdatedInChartExtended;
	BOOL	m_bUpdatingShowingFackedCandle;
	long	m_lKey;				// SetBySetBaseDesk
	CWnd    *m_pInputBarDlg;
	CWnd	*m_pExtInputBarDlg;
	CWnd	*m_pLeftBarDlg;
	UINT	m_nID;	// ID For Chart Component
	COLORREF m_clStartSelected;
	COLORREF m_clEndSelected;
	COLORREF m_clStartUnselected;
	COLORREF m_clEndUnselected;
	BOOL	 m_bChartExtended;
	CFont	m_ftForTitle;
	CFont	m_ftNormal;
	int		m_nMode;	// CHANGE_CHART(0): 변경, ADD_NEW_CHART(1):추가, OVERLAP_CHART(2):중첩
	int		m_nOldMode;
	/// Chart Matrix Variable & Func Start
	//CChartItem* GetpChartItemSelected();
	CChartItem* m_pChartItems[CNTOFMAXROW][CNTOFMAXCOL];
	long m_lCols;
	long m_lRows;
	long m_lColSelected;
	long m_lRowSelected;
	long m_lColDragFrom;
	long m_lRowDragFrom;
	CString m_strMultiCodes;
	CString m_strSharedTime;
	BOOL m_bClicked;
	BOOL m_bDragNDrop;
	BOOL m_bJongmokLink;			// 종목연동
	BOOL m_bTypeLink;				// 설정연동
	BOOL m_bIndexLink;				//20100212_JS.Kim 확대 상태에서 지표동기화 처리
	CString m_strUserPath;
	CString m_strImagePath;
	BOOL m_bNextQuery;
	BOOL m_bHideScale;
	BOOL m_bHideInnerToolBar;
	BOOL m_bOrderLink;				// 주문연동

	CString m_strTimeMarkReserved;	// TimeMark Setting Resevered
	
	BOOL		m_bNeedToUpdateInLoop;
	CChartItem	*m_pChartItemSelectedInLoop;
	CStringArray m_strArrayCodeInLoop;
	CStringArray m_strArrayTRCodeInLoop;
	long	m_lCntInLoop;
	BOOL	m_bDontRequestDataInLoop;
	BOOL	m_bUseRevisedInLoop;
	//BOOL	m_bUseRateValueInLoop;
	int		m_nChartMngs_ChartDataNumericValue;

	BOOL	m_bNormalizedMinuteTimeRuleInLoop;
	long	m_lZoomValueInLoop;
	CString m_strChartSelectedInLoop;
	char	m_chTypeInLoop;
	long	m_lNumberInLoop;
	long	m_lTickMinNumberInLoop;
	CString m_strEndDateInLoop;
	double	m_dEndDateInLoop;
	CString m_strCodeInLoop;

	long m_lColSelectedInLoop;
	long m_lRowSelectedInLoop;
	CChartItem *m_pChartItemInLoop;
	int m_nRowIndexInLoop;
	int m_nColIndexInLoop;
	long m_lTypeInLoop;
	
	enum {	LOOPTYPE_NULL	 = 0,
			LOOPTYPE_LOAD	, //1
			LOOPTYPE_FIXDATE, //2
			LOOPTYPE_REVISED, //3
			LOOPTYPE_UPDATE	, //4
			LOOPTYPE_UPDATE_BY_EXTEND, //5
			LOOPTYPE_RATE_VALUE,//6
			LOOPTYPE_FAKE_CANDLE = 7,
			LOOPTYPE_MULTI_LOAD = 8
	};
	
	BOOL m_bIsInLoop;
	void InitInLoop();
	BOOL HeadInLoop();
	BOOL TailInLoop();
	BOOL PostInLoop();

	long m_lDragStart;
	long m_lDragEnd;
	BOOL m_bSaveJipyoOneFile;

	// 분석툴 개별 저장 - ojtaso (20080723)
	BOOL m_bSaveAnalTool;

	// 2011.01.12 by SYS >> 분차트 YYYY(년) 추가 및 분석툴 공유 기능 추가
	BOOL m_bShareAnalTool;
	// 2011.01.12 by SYS

	//일주월분 차트
	BOOL LoadDWMMChart(BOOL bLoad);

	//차트 전체 초기화
	void ResetChartAll();
	// 새로운 차트 추가 : 복수종목 - ojtaso (20070108) 
	BOOL m_bAddMode;

	// 멀티차트의 선택여부 : 복수종목 - ojtaso (20070525)
	BOOL m_bSelChangeChart;
	BOOL m_bAddModeFromTab;			// Tab의 추가모드는 따로 저장
	
	BOOL m_bResetChartAll;			// 차트 초기화인지 : 복수종목 - ojtaso (20070621)

	// 분석툴바 메뉴추가
	CArray<int, int>	m_arrayAnalIdx;			//분석도구리스트 인덱스
	CArray<int, int>	m_arrayAnalCheck;		//분석도구리스트 체크상태
	CArray<int, int>	m_arrayAdditionIdx;		//부가기능리스트 인덱스
	CArray<int, int>	m_arrayAdditionCheck;	//부가기능리스트 체크상태
	CArray<int, int>	m_arrayUserIdx;			//사용자설정리스트 인덱스
	CArray<int, int>	m_arrayUserCheck;		//사용자설정리스트 체크상태
	CMap<int, int, CString, CString>	m_mapToolName;			//분석도구리스트 이름

	void MakePlatformMenu(HMENU hMenu);
	BOOL GetDlgToolBtnToggle(CArray<int, int>& arAnal, CArray<int, int>& arAnalCheck, CArray<int, int>& arAddition, CArray<int, int>& arAdditionCheck, CArray<int, int>& arUser, CArray<int, int>& arUserCheck, CMap<int, int, CString, CString>& mapToolName);
	void SetDlgToolBtnToggle(int nToolBtnID, BOOL bToggle = TRUE);

	// 메인 메뉴추가
	void AddMainMenuToPlatformMenu(HMENU hMenu);
	int GetPositionInMenu(CString strMenu, CMenu* pMenu);

// Operations
public:
	BOOL GetSaveJipyoOneFile(){return m_bSaveJipyoOneFile;};
	
	// 분석툴 개별 저장 - ojtaso (20080723)
	BOOL GetSaveAnalTool(){return m_bSaveAnalTool;};

	// 2011.01.12 by SYS >> 분차트 YYYY(년) 추가 및 분석툴 공유 기능 추가
	BOOL GetShareAnalTool(){return m_bShareAnalTool;};
	// 2011.01.12 by SYS

	BOOL GetExistSyncChart(){return m_bExistSyncChart;};

	// 종목명 얻기 : 복수종목 - ojtaso (20080213)
	CString		GetJongMokNameByCode(LPCSTR lpszItemCode, LPCTSTR lpszTRCode = NULL);
	// 종목코드로 RQ를 찾고 선택 : 복수종목 - ojtaso (20070705)
	BOOL		FindSelectChartByItemCode(LPCTSTR lpszItemCode, BOOL bSelect = TRUE);

	BOOL		IsAbleToAdd() { return (!m_bChangeTypeMode || m_bResetChartAll); }
// --> [Edit]  이도영 2008/09/11
// 주문 정보 
	CMapStringToOb	m_mapOrder;
	BOOL		SetOrderData(char *pData);
	BOOL		GetOrderLink() { return m_bOrderLink; };

	BOOL		SetRealOrderInfo(ORDER_INFO* pOrderInfo, int nRealType);

	CString		GetCurrentCode();
// <-- [Edit]  이도영 2008/09/11
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartMng)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:			
	void SetFocusToJMEdit_At_LoadEnd();
	CString GetCurSelChartItemJMName();
	void MoveFloatingHogaDlg();
	virtual ~CChartMng();
	HBITMAP			m_hBitmap;
	BOOL RevisedPriceFlag();// 수정주가.

	CString GetUserpath(){return m_strUserPath;};
	BOOL IsInLoop() {return m_bIsInLoop;};

	// Generated message map functions
protected:
	//{{AFX_MSG(CChartMng)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point); 		
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	LRESULT OnEventMessage( WPARAM wParam, LPARAM lParam);
	LRESULT OnGetUserID( WPARAM wParam, LPARAM lParam);
	LRESULT OnMagnifyingGlass( WPARAM wParam, LPARAM lParam);
	LRESULT OnAddPlatformMenu( WPARAM wParam, LPARAM lParam);
//	LRESULT OnCheckPlatformMenu( WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
		
	CUIntArray	m_menuMain, menuChart;

// (2008/9/14 - Seung-Won, Bae) for Pair Tick
public:
	void	RequestChartPairTickData( const char *p_szCode, BOOL p_bWithRequest = TRUE);

	//>> 예외처리 - vntsorl (20090106)
	BOOL	m_bInitChartItem;
	//<< 예외처리 - vntsorl (20090106)

	//<<20100308_JS.Kim 솔로몬
	CString		Format32String(CString strPrice, int scale, int outdec, int indec, UINT uMask);
	//>>
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTMNG_H__FD135639_7D01_492D_BFC5_AF011DA6BD62__INCLUDED_)
