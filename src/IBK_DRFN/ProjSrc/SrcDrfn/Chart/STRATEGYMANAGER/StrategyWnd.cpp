// StrategyWnd.cpp : implementation file
//

#include "stdafx.h"
#include "StrategyWnd.h"
#include "LogicOperation.h"
#include "LogicST.h"
#include "../Common_ST/STConfigDef.h"
#include "../Common_ST/Commonlib.h"
#include "DealCal.h"
#include "DealCalIndexCrossAboveBasis.h"
#include "DealCalIndexCrossBelowBasis.h"

#include "DealCalIndexUpwardReversal.h"
#include "DealCalIndexDownwardReversal.h"
#include "DealCalIndexCrossAboveAnother.h"
#include "DealCalIndexCrossBelowAnother.h"

#include "FeeCal.h"
#include "FeeCalByPercent.h"
#include "FeeCalByPrice.h"

#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT RMSG_STDATAMANAGER_RESULTDATA = ::RegisterWindowMessage("RMSG_STDATAMANAGER_RESULTDATA");
//const UINT RMSG_STCONFIGURENAME = ::RegisterWindowMessage("RMSG_STCONFIGURENAME");
//const UINT RMSG_STCONTROLLER_POINTER = ::RegisterWindowMessage("RMSG_STCONTROLLER_POINTER");

#define ID_FORCEDEXITBYTIME	100

//////////////////////////////////////////////////////////////////////////
// CForcedLiquidationInfo

#define SENTINEL_FLINFO		9999
CForcedLiquidationInfo::CForcedLiquidationInfo()
: m_nExitIndex(SENTINEL_FLINFO), m_lDealValue(0L), m_pDataItem(NULL)
{
}

void CForcedLiquidationInfo::SetFLInfo(int nExitIndex, long lDealValue, CIDataItem* pDataItem)
{
	// 먼저 발생한 강제청산 Index로 셋팅
	if(m_nExitIndex > nExitIndex)
	{
		m_nExitIndex = nExitIndex;
		m_lDealValue = lDealValue;
		m_pDataItem = pDataItem;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CStrategyWnd

CStrategyWnd::CStrategyWnd()
{
	m_pLogicRootItem = NULL;

	m_nCurrentRunningLogic = 0;
	m_bIsRunning = FALSE;
	m_bHasAPData = FALSE;
	m_pDateItem = NULL;
	m_lKey = 0;
	m_lDataCnt = 0;
	m_Status = STRATEGY_NONE;

	m_dUpperPrice =0.;
	m_dLowerPrice =0.;
	m_dVolForOrder = 0.;
	m_dSumProfit = 0.;
	m_dSumLoss = 0.;
	m_dMaxProfit = 0.;
	m_dMaxLoss = 0.;
	m_dSumAmountExit = 0.;
	m_lCntProfit = 0;
	m_lCntLoss = 0;
	m_lCntProfitStraight = 0;
	m_lCntLossStraight = 0;
	m_lSumProfitStraight = 0;
	m_lSumLossStraight = 0;
	m_lSumCandleProfit = 0;
	m_lSumCandleLoss = 0;

	m_lCntCandleAccess = 0;
	m_strategyType = NONE;
	m_lCurrentDealPosition = 0;
	m_dCurrentProfitRate = 0.;
	m_lAllCandleInCalculation = 0;
	m_nLastSize = 0;
	m_pFeeCal = NULL;
	m_pSlippageCal = NULL;
	m_pChartInterface = NULL;
	InitConfiguration();

	m_dSellOption = 0.;
	m_dBuyOption = 0.;

	m_nTimerID = 0;

	// vntsorl
	m_bBuyDealTraceInit = FALSE;
	m_bSellDealTraceInit = FALSE;
	m_bBuyOrderStartSignal = FALSE;
	m_bSellOrderStartSignal = FALSE;
}

CStrategyWnd::~CStrategyWnd()
{
	RemoveAllArray();
}


BEGIN_MESSAGE_MAP(CStrategyWnd, CWnd)
	//{{AFX_MSG_MAP(CStrategyWnd)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(RMSG_STDATAMANAGER_RESULTDATA, OnSTDataManagerMessage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CStrategyWnd message handlers

int CStrategyWnd::Create(CWnd* pParentWnd, long lKey, CStringArray &rStArrayPath,long pChartItem)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(!pParentWnd)
		return -1;

	m_pChartInterface = pChartItem;
	m_lKey = lKey;
	m_strArrayPath.RemoveAll();
	int nSize = rStArrayPath.GetSize();
	if(nSize!=2) return -1;
	for(int nPos=0;nPos<nSize;nPos++)
		m_strArrayPath.Add(rStArrayPath.GetAt(nPos));

	m_logicMaker2.SetArrayPath(&m_strArrayPath);
	return CWnd::Create(NULL,"StrategyWnd",WS_CHILD,CRect(0,0,0,0),pParentWnd,0x1000);	
}

BOOL CStrategyWnd::DestoryWindow()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CWnd::DestroyWindow();
}

void CStrategyWnd::OnDestroy() 
{
	Init();	
	CWnd::OnDestroy();	
}


void CStrategyWnd::Init()
{
//	m_CriticalSection.Lock();
	m_pDateItem = NULL;
	m_strategyType = NONE;
	m_Status = STRATEGY_NONE;
	m_bHasAPData = FALSE;
	m_lDataCnt = 0;
	m_strInputCode = "";
	if(m_nTimerID) KillTimer(m_nTimerID);

	if(m_pFeeCal)
	{
		delete m_pFeeCal;
		m_pFeeCal = NULL;
	}
	if(m_pSlippageCal)
	{
		delete m_pSlippageCal;
		m_pSlippageCal = NULL;
	}

	if(m_pLogicRootItem)
	{
		if(m_pLogicRootItem->GetType()==2)
		{
			CLogicOperation* pLogicItem = (CLogicOperation*)m_pLogicRootItem;
			//delete m_pLogicRootItem;
			delete pLogicItem;
			m_pLogicRootItem = NULL;
		}
		else
		{
			CLogicST * pLogicItem = (CLogicST*)m_pLogicRootItem;
			//delete m_pLogicRootItem;
			delete pLogicItem;
			m_pLogicRootItem = NULL;
		}
	}
//	m_CriticalSection.Unlock();
}

//지표최적화에서만 사용되는 함수이다.(KHD)
BOOL CStrategyWnd::LoadSD(LPCTSTR lpStrategyDllPath,LPCTSTR lpInputCode,long lDataCnt,LPCTSTR lpEndDate, LPCTSTR lpTimeTypeNum,LPCTSTR lpTypeDealOption)
{
	Init();		
	CLogicST *pLogicST = new CLogicST;

	BOOL bRetValue = pLogicST->Init(lpStrategyDllPath,0,TRUE,lpTimeTypeNum,NULL,this->GetSafeHwnd());
	if(!bRetValue) 	
	{
		delete pLogicST;
		pLogicST = NULL;
		return FALSE;
	}
	m_strStrategyPath = lpStrategyDllPath;
	m_pLogicRootItem = pLogicST;	
	m_strategyType = ST_SD;
	m_lDataCnt = lDataCnt;
	m_strInputCode = lpInputCode;
	if(lpEndDate==NULL)
	{
		CTime t = CTime::GetCurrentTime();
		m_strEndDate = t.Format("%Y%m%d");	
	}
	else
	{
		m_strEndDate = lpEndDate;
	}

	int nShifter = 0;
	int nPos = 0;
	CString strDataDivided;
	CString strTitle;
	CString strContents;
	if(lpTypeDealOption)
	{
		while(1)
		{
			strDataDivided = Parser(lpTypeDealOption,&nShifter);
			if(strDataDivided.IsEmpty()) break;
			nPos = strDataDivided.Find('=');
			if(nPos!=-1)
			{
				strTitle = strDataDivided.Left(nPos);
				strContents = strDataDivided.Right(strDataDivided.GetLength()-nPos-1);
			}
			else
			{
				continue;
			}
			/////////////////////////////////////////
			if(strTitle==OPT_SELL)
			{
				m_strTypeSell = strContents;	
			}
			if(strTitle==OPT_BUY)
			{
				m_strTypeBuy = strContents;
			}
			if(strTitle==OPT_SELL_OPTION)
			{
				m_dSellOption = atof(strContents);
			}
			if(strTitle==OPT_BUY_OPTION)
			{
				m_dBuyOption = atof(strContents);
			}			
		}
	}
	return TRUE;
}

BOOL CStrategyWnd::LoadST(LPCTSTR lpStrategyPath,LPCTSTR lpInputCode,long lDataCnt,LPCTSTR lpEndDate)
{
	if(!lpStrategyPath||!lpInputCode||lDataCnt<=0) return FALSE;
//	Init();

	m_strStrategyPath = lpStrategyPath;//로드될 ST 파일

	m_dealDataItem.Clear();
	LoadConfiguration();

	m_lDataCnt = lDataCnt;//Data 갯수
	m_strInputCode = lpInputCode;//종목코드
	if(m_strStrategyPath.GetLength()==0||m_strInputCode.GetLength()==0) return FALSE;
	if(lpEndDate==NULL)
	{
		CTime t = CTime::GetCurrentTime();
		m_strEndDate = t.Format("%Y%m%d");	
	}
	else
	{
		m_strEndDate = lpEndDate;
	}
	//조합전략 파일 존재 유무 확인////////////////////
	CFileStatus status;
	BOOL bDLLIsExist = CFile::GetStatus(lpStrategyPath,status);
	if(!bDLLIsExist) 
		return FALSE;

	char szBuffer[256];
	::GetPrivateProfileString(STRATEGYLIST, STRATEGYLOGIC, "",szBuffer, sizeof(szBuffer), m_strStrategyPath);
	CString strAllLogic = szBuffer;

	CString strAllLogicConverted;
	char szType[2];
	char szValue[2];
	int nType = 0;
	int nValue = 0;
	int nOffsetOfAllLogic = 0;
	int nLengthOfAlllogic = strAllLogic.GetLength();	
	memset(szType,0x00,sizeof(szType));
	memset(szValue,0x00,sizeof(szValue));
	char szIndex[2] = "A";
	while(1)
	{
		if(nOffsetOfAllLogic>=nLengthOfAlllogic)	break;
		szType[0] = strAllLogic[nOffsetOfAllLogic];
		nType = atol(szType);
		nOffsetOfAllLogic++;

		if(nOffsetOfAllLogic>=nLengthOfAlllogic)	break;
		szValue[0] = strAllLogic[nOffsetOfAllLogic];
		nValue = atol(szValue);
		nOffsetOfAllLogic++;
		switch(nType)
		{
		case OPERATIONINLOGIC:// 연산자 대입 
			if(nValue==NEXTINLOGIC)
			{
				strAllLogicConverted += ">";
			}
			else if(nValue==ORINLOGIC)
			{
				strAllLogicConverted += "|";
			}
			else
			{
				strAllLogicConverted += "&";
			}
			break;
		case PARENTHESISINLOGIC:
			if(nValue==OPENPARENTHESISINLOGIC)
			{
				strAllLogicConverted += "(";
			}
			else
			{
				strAllLogicConverted += ")";
			}
			break;
		default:	//ELEMENTINLOGIC
			if(!nValue)
			{
				strAllLogicConverted += "!";
			}
			strAllLogicConverted += szIndex;
			szIndex[0]++;// 연산자가 있으면 A에서 증가시킨다. 
			break;
		}
		
	}//연산식을 만들어 낸다. EX) 112111 이면 A|B라는 연산식으로 산출된다. : KHD
	if(!strAllLogicConverted.GetLength()) 
	{
		m_strError.Format("해당 조합전략에 단위전략이 존재하지 않습니다.");
		return FALSE;
	}

	//연산자를 통해 각각 A|B라면 그 해당하는 전략SD파일을 로드하고 연산자를 저장한 CLogicOperation 클래스의 주소를 반환한다.
	m_pLogicRootItem = m_logicMaker2.Evaluates(strAllLogicConverted,m_strStrategyPath,this->GetSafeHwnd(),m_pChartInterface);//부모는 바로 전략메니져..

	if(!m_pLogicRootItem) return FALSE;
	m_strategyType = ST_ST;
	return TRUE;
}

BOOL CStrategyWnd::Stop()
{
	BOOL bRetValue = FALSE;
	if(m_pLogicRootItem)
	{
		bRetValue = m_pLogicRootItem->Stop();
	}
	if(bRetValue)
	{
		m_Status = STRATEGY_STOP;
		m_bHasAPData = FALSE;
	}
	return bRetValue;
}


long CStrategyWnd::Update(long lPos, BOOL bIsUpdateData)
{
	long lRetValue = 0;
	if(m_pLogicRootItem)
	{
		lRetValue = m_pLogicRootItem->Update(lPos, bIsUpdateData);
	}

	return lRetValue;
}
// return value: 0 = Fail, 1 = Normal Execution, 2 = Normal Execution but Raw data exists, 3 = execution after pause, 4 = cfg execuation
BOOL CStrategyWnd::Run(LPCTSTR lpOption)
{
	long lRetValue = 0;
	if(m_pLogicRootItem) 
	{
		lRetValue = m_pLogicRootItem->Run(m_strInputCode,m_lDataCnt,m_strEndDate,lpOption);			
	}
	if(lRetValue==0) return FALSE;
	if(lRetValue==3)
	{
		m_Status = STRATEGY_RUN;
	}
	else
	{
		m_Status = STRATEGY_STANDBY;
	}
	return TRUE;
}

BOOL CStrategyWnd::Pause()
{
	BOOL bRetValue = FALSE;
	if(m_pLogicRootItem)
	{
		bRetValue = m_pLogicRootItem->Pause();
	}
	if(bRetValue)	m_Status = STRATEGY_PAUSE;
	return bRetValue;
}

OPERATTIONSTATUS CStrategyWnd::GetOperationStatus()
{
	return m_Status;
}

CString CStrategyWnd::GetTime()
{
	if(m_pLogicRootItem)
	{
		return m_pLogicRootItem->GetTime();
	}
	return "";	
}


LRESULT CStrategyWnd::OnSTDataManagerMessage( WPARAM wParam, LPARAM lParam)
{
	if(m_strategyType==ST_ST)
	{
		return OnSTDataManagerMessageST( wParam, lParam);
	}
	else if(m_strategyType==ST_SD)
	{
		return OnSTDataManagerMessageSD( wParam, lParam);
	}
	return 0L;
}

LRESULT CStrategyWnd::OnSTDataManagerMessageSD( WPARAM wParam, LPARAM lParam)
{
	CLogicST *pLogicST= (CLogicST *)wParam;
	pLogicST->ReceiveFromSTDataManager(lParam);
	ST_DRAWINFO* pDrawInfo = (ST_DRAWINFO*)lParam;
	if(!m_bHasAPData)
	{
		m_ptrArraySDataItem.RemoveAll();
		m_ptrArrayPlot.RemoveAll();
		m_ptrArrayHistoryDataItem.RemoveAll();
		m_pDateItem = m_pLogicRootItem->GetDateItem();
		CPtrArray *pHistoryData = m_pLogicRootItem->GetPArrayHistoryData();
		int nSizeOfHistory = pHistoryData->GetSize();
		for(int nPosOfHistory = 0 ; nPosOfHistory<nSizeOfHistory;nPosOfHistory++)
		{
			m_ptrArrayHistoryDataItem.Add(pHistoryData->GetAt(nPosOfHistory));
		}
		CIDataItem *pCloseItem = (CIDataItem *)pHistoryData->GetAt(3);

				
		///////////////////
		// Calculatation Logic
		//////////////////
		CPtrArray* pPrtArrayPlot = pDrawInfo->pPtrArrayPlot;
		int nSizeOfPtrArrayPlot = pPrtArrayPlot->GetSize();
		ASSERT(nSizeOfPtrArrayPlot);
		CIDataItem *pPlotItem = (CIDataItem *)pPrtArrayPlot->GetAt(0);
		int nSize = pPlotItem->GetSize();
		m_dealDataItem.Clear();
		double dValue = 0;
		double dValueLast = 0;
		BOOL bSetFirst = TRUE;
		int nStartPos = pPlotItem->GetFirstDataIndex();
	
		CDealCal *pDealBuy = NULL;
		CDealCal *pDealSell = NULL;

		// buy
		/////////////////////////////////////////////////////////////////
		if(m_strTypeBuy==OPT_INDEX_CROSSABOVE_BASIS)
		{
			pDealBuy = new CDealCalIndexCrossAboveBasis(&m_dealDataItem, VALBUY, m_dBuyOption);
		}
		else if(m_strTypeBuy==OPT_INDEX_CROSSBELOW_BASIS)
		{		
			pDealBuy = new CDealCalIndexCrossBelowBasis(&m_dealDataItem, VALBUY, m_dBuyOption);
		}
		else if(m_strTypeBuy==OPT_INDEX_CROSSABOVE_ANOTHER)
		{			
			ASSERT(m_dBuyOption<pPrtArrayPlot->GetSize());
			pDealBuy = new CDealCalIndexCrossAboveAnother(&m_dealDataItem, VALBUY
										, (CIDataItem *)pPrtArrayPlot->GetAt((int)m_dBuyOption));
		}
		else if(m_strTypeBuy==OPT_INDEX_CROSSBELOW_ANOTHER)
		{			
			ASSERT(m_dBuyOption<pPrtArrayPlot->GetSize());
			pDealBuy = new CDealCalIndexCrossBelowAnother(&m_dealDataItem, VALBUY
										, (CIDataItem *)pPrtArrayPlot->GetAt((int)m_dBuyOption));
		}
		else if(m_strTypeBuy==OPT_PRICE_CROSSABOVE_INDEX)
		{		
			pDealBuy = new CDealCalIndexCrossAboveAnother(&m_dealDataItem, VALBUY, pCloseItem);
		}
		else if(m_strTypeBuy==OPT_PRICE_CROSSBELOW_INDEX)
		{			
			pDealBuy = new CDealCalIndexCrossBelowAnother(&m_dealDataItem, VALBUY, pCloseItem);
		}
		else if(m_strTypeBuy==OPT_INDEX_UPWARDREVERSAL)
		{	
			pDealBuy = new CDealCalIndexUpwardReversal(&m_dealDataItem, VALBUY);
		}
		else if(m_strTypeBuy==OPT_INDEX_DOWNWARDREVERSAL)
		{	
			pDealBuy = new CDealCalIndexDownwardReversal(&m_dealDataItem, VALBUY);
		}		
		// SELL
		/////////////////////////////////////////////////////////////////
		if(m_strTypeSell==OPT_INDEX_CROSSABOVE_BASIS)
		{
			pDealSell = new CDealCalIndexCrossAboveBasis(&m_dealDataItem, VALSELL, m_dSellOption);
		}
		else if(m_strTypeSell==OPT_INDEX_CROSSBELOW_BASIS)
		{			
			pDealSell = new CDealCalIndexCrossBelowBasis(&m_dealDataItem, VALSELL, m_dSellOption);
		}
		else if(m_strTypeSell==OPT_INDEX_CROSSABOVE_ANOTHER)
		{			
			ASSERT(m_dSellOption<pPrtArrayPlot->GetSize());
			pDealSell = new CDealCalIndexCrossAboveAnother(&m_dealDataItem, VALSELL
										, (CIDataItem *)pPrtArrayPlot->GetAt((int)m_dSellOption));
		}
		else if(m_strTypeSell==OPT_INDEX_CROSSBELOW_ANOTHER)
		{			
			ASSERT(m_dSellOption<pPrtArrayPlot->GetSize());
			pDealSell = new CDealCalIndexCrossBelowAnother(&m_dealDataItem, VALSELL
										, (CIDataItem *)pPrtArrayPlot->GetAt((int)m_dSellOption));
		}
		else if(m_strTypeSell==OPT_PRICE_CROSSABOVE_INDEX)
		{			
			pDealSell = new CDealCalIndexCrossAboveAnother(&m_dealDataItem, VALSELL, pCloseItem);
		}
		else if(m_strTypeSell==OPT_PRICE_CROSSBELOW_INDEX)
		{	
			pDealSell = new CDealCalIndexCrossBelowAnother(&m_dealDataItem, VALSELL, pCloseItem);
		}
		else if(m_strTypeSell==OPT_INDEX_UPWARDREVERSAL)
		{			
			pDealSell = new CDealCalIndexUpwardReversal(&m_dealDataItem, VALSELL);
		}
		else if(m_strTypeSell==OPT_INDEX_DOWNWARDREVERSAL)
		{	
			pDealSell = new CDealCalIndexDownwardReversal(&m_dealDataItem, VALSELL);
		}
		if(pDealBuy&&pDealSell)
		{
			for(int nPos=nStartPos;nPos<nSize;nPos++)
			{
				dValue = pPlotItem->GetAt(nPos);
				pDealBuy->SetDeal(nPos,dValue);
				pDealSell->SetDeal(nPos,dValue);
			}	
		}		
		if(pDealBuy)
		{
			delete pDealBuy;
			pDealBuy = NULL;
		}
		if(pDealSell)
		{
			delete pDealSell;
			pDealSell = NULL;
		}
		
		m_ptrArraySDataItem.Add(m_pDateItem);
		m_DrawInfo.pPtrArraySDataItem = &m_ptrArraySDataItem;
		m_DrawInfo.pPtrArrayHistoryDataItem = &m_ptrArrayHistoryDataItem;
		m_DrawInfo.pPtrArrayPlot = &m_ptrArrayPlot;
		m_DrawInfo.pDealDataItem = &m_dealDataItem;

		m_bHasAPData = TRUE;
	}
	m_Status = STRATEGY_RUN;
	GetParent()->SendMessage(RMSG_STDATAMANAGER_RESULTDATA,m_lKey,(LPARAM)&m_DrawInfo);
	return 1L;
}

void CStrategyWnd::LoadConfiguration()
{
	InitConfiguration();
	ReadConfiguration(m_strStrategyPath);
}

void CStrategyWnd::SetChartDivideInfo(int nDivideState, int nUnitPos)
{
	// 분할 상태 : m_nDivideState
	// HORZ_NOTYPE,		// 없음
	// HORZ_DAILY,		// 일간
	// HORZ_WEEKLEY,	// 주간
	// HORZ_MONTHLY,	// 월간
	// HORZ_TIME_ONE,	// 시간-한줄
	// HORZ_TICK,		// 틱
	// HORZ_TIME_TWO,	// 시간-두줄
	// HORZ_TEXT,		// text
	// HORZ_LEFTRIGHT,	// 좌우식
	// HORZ_PRICEZONE,	// 가격대
	// HORZ_DATA,		// data(거래량 포함)
	// HORZ_MARKETPROFILE, // MarketProfile
	// HORZ_TIME_SECOND	// 초

	// 분할 하위 시간구분 : m_nUnitPos
	m_nDivideState = nDivideState;
	m_nUnitPos = nUnitPos;
}

void CStrategyWnd::InitBuyDealTrace(BOOL bRealState/* = FALSE*/, int nInterval/* = 0*/)
{
	CIDataItem* pHighPrice = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pLowPrice = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);

	CIDataItem* pPrice = NULL;
	if(m_bBuyDealTraceInit)
	{
		// 조회 데이터 처리
		if(bRealState == FALSE)
		{
			if(m_buyDealTrace.IsEntryPosition())
				pPrice = pLowPrice;
			else if(m_buyDealTrace.IsExitPosition())
				pPrice = pHighPrice;

			// 주문시작신호 -> 모든신호로 선택한 경우
			if(m_buyDealTrace.IsEntryPosition() || m_buyDealTrace.IsExitPosition())
			{
				if(pPrice)
				{
					long lDealValue = (long)m_arrayDealPosition.GetAt(m_buyDealTrace.GetRawIndex());
					lDealValue |= m_buyDealTrace.GetRawDealValue();
					m_arrayDealPosition.SetAt(m_buyDealTrace.GetRawIndex(), (double)lDealValue);
					m_arrayDealName.SetAt(m_buyDealTrace.GetRawIndex(), m_buyDealTrace.GetRawDealName());
					m_arrayDealDrawPosition.SetAt(m_buyDealTrace.GetRawIndex(), pPrice->GetAt(m_buyDealTrace.GetRawIndex()));
				}
			}

			m_buyDealTrace.Next(bRealState);
		}
		// 리얼 데이터 처리
		else
		{
			// 주문시점이 [조건만족시] 인 경우
			if(m_buyDealTrace.GetTimingOrder() == 0)
			{
				if(m_buyDealTrace.IsEntryPosition())
					pPrice = pLowPrice;
				else if(m_buyDealTrace.IsExitPosition())
					pPrice = pHighPrice;

				//-------------------------------------------------------------
				// 신호표시
				if(pPrice)
				{
					long lDealValue = (long)m_arrayDealPosition.GetAt(m_buyDealTrace.GetRawIndex());
					lDealValue |= m_buyDealTrace.GetRawDealValue();
					m_arrayDealPosition.SetAt(m_buyDealTrace.GetRawIndex(), (double)lDealValue);
					m_arrayDealName.SetAt(m_buyDealTrace.GetRawIndex(), m_buyDealTrace.GetRawDealName());
					m_arrayDealDrawPosition.SetAt(m_buyDealTrace.GetRawIndex(), pPrice->GetAt(m_buyDealTrace.GetRawIndex()));

					if(m_bBuyOrderStartSignal == FALSE)
					{
						// 주문시작신호 -> 모든신호로 선택한 경우
						if(m_nOrderStartSignal == 1)
							m_bBuyOrderStartSignal = TRUE;
						// 주문시작신호 -> 진입신호로 선택한 경우
						else if(m_nOrderStartSignal == 0 && m_buyDealTrace.IsEntryPosition())
							m_bBuyOrderStartSignal = TRUE;
					}
				}

				//-------------------------------------------------------------
				// 주문처리
				if(m_bBuyOrderStartSignal)
					m_dealDataItem.SetData(m_buyDealTrace.GetRawIndex(), 
											m_buyDealTrace.GetRawDealValue(),
											m_buyDealTrace.GetRawType(),
											m_buyDealTrace.GetRawDealName(),
											m_buyDealTrace.GetRawPrice(),
											m_buyDealTrace.GetRawContract(),
											m_buyDealTrace.GetRawEntryNameForExit());

				m_buyDealTrace.Next(bRealState);
			}
			// 주문시점이 [봉완성시] 인 경우
			else if(m_buyDealTrace.GetTimingOrder() == 1)
			{
				//-------------------------------------------------------------
				// 신호표시
				if(nInterval == 0)		// 봉 갯수 그대로
				{
					m_buyDealTrace.Copy();
					if(m_buyDealTrace.GetOrdrIndex() < 0 && m_buyDealTrace.GetOrdrDealValue() == 0l)
						m_bBuyDealTraceInit = FALSE;
				}
				else					// 봉 갯수 변경
				{
					m_buyDealTrace.Next(bRealState);

					if(m_buyDealTrace.IsEntryPosition(m_buyDealTrace.GetOrdrDealValue()))
						pPrice = pLowPrice;
					else if(m_buyDealTrace.IsExitPosition(m_buyDealTrace.GetOrdrDealValue()))
						pPrice = pHighPrice;

					if(pPrice)
					{
						long lDealValue = (long)m_arrayDealPosition.GetAt(m_buyDealTrace.GetOrdrIndex());
						lDealValue |= m_buyDealTrace.GetOrdrDealValue();
						m_arrayDealPosition.SetAt(m_buyDealTrace.GetOrdrIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_buyDealTrace.GetOrdrIndex(), m_buyDealTrace.GetOrdrDealName());
						m_arrayDealDrawPosition.SetAt(m_buyDealTrace.GetOrdrIndex(), pPrice->GetAt(m_buyDealTrace.GetOrdrIndex()));

						if(m_bBuyOrderStartSignal == FALSE)
						{
							// 주문시작신호 -> 모든신호로 선택한 경우
							if(m_nOrderStartSignal == 1)
								m_bBuyOrderStartSignal = TRUE;
							// 주문시작신호 -> 진입신호로 선택한 경우
							else if(m_nOrderStartSignal == 0 && m_buyDealTrace.IsEntryPosition(m_buyDealTrace.GetOrdrDealValue()))
								m_bBuyOrderStartSignal = TRUE;
						}
					}
					else
					{
						if(m_buyDealTrace.GetOrdrIndex() < 0 && m_buyDealTrace.GetOrdrDealValue() == 0l)
							m_bBuyDealTraceInit = FALSE;
					}

					//-------------------------------------------------------------
					// 주문처리
					if(m_bBuyOrderStartSignal)
						m_dealDataItem.SetData(m_buyDealTrace.GetOrdrIndex(), 
												m_buyDealTrace.GetOrdrDealValue(),
												m_buyDealTrace.GetOrdrType(),
												m_buyDealTrace.GetOrdrDealName(),
												m_buyDealTrace.GetOrdrPrice(),
												m_buyDealTrace.GetOrdrContract(),
												m_buyDealTrace.GetOrdrEntryNameForExit());
				}
			}
		}
	}
}

void CStrategyWnd::InitSellDealTrace(BOOL bRealState/* = FALSE*/, int nInterval/* = 0*/)
{
	CIDataItem* pHighPrice = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pLowPrice = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);

	CIDataItem* pPrice = NULL;
	if(m_bSellDealTraceInit)
	{
		// 조회 데이터 처리
		if(bRealState == FALSE)
		{
			if(m_sellDealTrace.IsEntryPosition())
				pPrice = pHighPrice;
			else if(m_sellDealTrace.IsExitPosition())
				pPrice = pLowPrice;

			if(m_sellDealTrace.IsEntryPosition() || m_sellDealTrace.IsExitPosition())
			{
				if(pPrice)
				{
					long lDealValue = (long)m_arrayDealPosition.GetAt(m_sellDealTrace.GetRawIndex());
					lDealValue |= m_sellDealTrace.GetRawDealValue();
					m_arrayDealPosition.SetAt(m_sellDealTrace.GetRawIndex(), (double)lDealValue);
					m_arrayDealName.SetAt(m_sellDealTrace.GetRawIndex(), m_sellDealTrace.GetRawDealName());
					m_arrayDealDrawPosition.SetAt(m_sellDealTrace.GetRawIndex(), pPrice->GetAt(m_sellDealTrace.GetRawIndex()));
				}
			}

			m_sellDealTrace.Next(bRealState);
		}
		// 리얼 데이터 처리
		else
		{
			// 주문시점이 [조건만족시] 인 경우
			if(m_sellDealTrace.GetTimingOrder() == 0)
			{
				if(m_sellDealTrace.IsEntryPosition())
					pPrice = pHighPrice;
				else if(m_sellDealTrace.IsExitPosition())
					pPrice = pLowPrice;

				//-------------------------------------------------------------
				// 신호표시
				if(pPrice)
				{
					long lDealValue = (long)m_arrayDealPosition.GetAt(m_sellDealTrace.GetRawIndex());
					lDealValue |= m_sellDealTrace.GetRawDealValue();
					m_arrayDealPosition.SetAt(m_sellDealTrace.GetRawIndex(), (double)lDealValue);
					m_arrayDealName.SetAt(m_sellDealTrace.GetRawIndex(), m_sellDealTrace.GetRawDealName());
					m_arrayDealDrawPosition.SetAt(m_sellDealTrace.GetRawIndex(), pPrice->GetAt(m_sellDealTrace.GetRawIndex()));

					if(m_bSellOrderStartSignal == FALSE)
					{
						// 주문시작신호 -> 모든신호로 선택한 경우
						if(m_nOrderStartSignal == 1)
							m_bSellOrderStartSignal = TRUE;
						// 주문시작신호 -> 진입신호로 선택한 경우
						else if(m_nOrderStartSignal == 0 && m_sellDealTrace.IsEntryPosition())
							m_bSellOrderStartSignal = TRUE;
					}
				}

				//-------------------------------------------------------------
				// 주문처리
				if(m_bSellOrderStartSignal)
					m_dealDataItem.SetData(m_sellDealTrace.GetRawIndex(), 
											m_sellDealTrace.GetRawDealValue(),
											m_sellDealTrace.GetRawType(),
											m_sellDealTrace.GetRawDealName(),
											m_sellDealTrace.GetRawPrice(),
											m_sellDealTrace.GetRawContract(),
											m_sellDealTrace.GetRawEntryNameForExit());

				m_sellDealTrace.Next(bRealState);
			}
			// 주문시점이 [봉완성시] 인 경우
			else if(m_sellDealTrace.GetTimingOrder() == 1)
			{
				//-------------------------------------------------------------
				// 신호표시
				if(nInterval == 0)		// 봉 갯수 그대로
				{
					m_sellDealTrace.Copy();
					if(m_sellDealTrace.GetOrdrIndex() < 0 && m_sellDealTrace.GetOrdrDealValue() == 0l)
						m_bSellDealTraceInit = FALSE;
				}
				else					// 봉 갯수 변경
				{
					m_sellDealTrace.Next(bRealState);

					if(m_sellDealTrace.IsEntryPosition(m_sellDealTrace.GetOrdrDealValue()))
						pPrice = pHighPrice;
					else if(m_sellDealTrace.IsExitPosition(m_sellDealTrace.GetOrdrDealValue()))
						pPrice = pLowPrice;

					if(pPrice)
					{
						long lDealValue = (long)m_arrayDealPosition.GetAt(m_sellDealTrace.GetOrdrIndex());
						lDealValue |= m_sellDealTrace.GetOrdrDealValue();
						m_arrayDealPosition.SetAt(m_sellDealTrace.GetOrdrIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_sellDealTrace.GetOrdrIndex(), m_sellDealTrace.GetOrdrDealName());
						m_arrayDealDrawPosition.SetAt(m_sellDealTrace.GetOrdrIndex(), pPrice->GetAt(m_sellDealTrace.GetOrdrIndex()));

						if(m_bSellOrderStartSignal == FALSE)
						{
							// 주문시작신호 -> 모든신호로 선택한 경우
							if(m_nOrderStartSignal == 1)
								m_bSellOrderStartSignal = TRUE;
							// 주문시작신호 -> 진입신호로 선택한 경우
							else if(m_nOrderStartSignal == 0 && m_sellDealTrace.IsEntryPosition(m_sellDealTrace.GetOrdrDealValue()))
								m_bSellOrderStartSignal = TRUE;
						}
					}

					//-------------------------------------------------------------
					// 주문처리
					if(m_bSellOrderStartSignal)
						m_dealDataItem.SetData(m_sellDealTrace.GetOrdrIndex(), 
												m_sellDealTrace.GetOrdrDealValue(),
												m_sellDealTrace.GetOrdrType(),
												m_sellDealTrace.GetOrdrDealName(),
												m_sellDealTrace.GetOrdrPrice(),
												m_sellDealTrace.GetOrdrContract(),
												m_sellDealTrace.GetOrdrEntryNameForExit());
				}
			}
		}
	}
}

void CStrategyWnd::ExecuteBuyDealTrace(CDealData* pDealData, BOOL bRealState/* = FALSE*/, int nInterval/* = 0*/)
{
	CIDataItem* pHighPrice = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pLowPrice = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);

	CIDataItem* pPrice = NULL;

	// 조회 데이터 처리
	if(bRealState == FALSE)
	{
		if(m_buyDealTrace.Set(pDealData, bRealState))
		{
			ConfirmBuyForcedLiquidation(bRealState, nInterval);

			if(m_buyDealTrace.IsEntryPosition())
				pPrice = pLowPrice;
			else if(m_buyDealTrace.IsExitPosition())
				pPrice = pHighPrice;

			if(pPrice)
			{
				long lDealValue = (long)m_arrayDealPosition.GetAt(m_buyDealTrace.GetRawIndex());
				lDealValue |= m_buyDealTrace.GetRawDealValue();
				m_arrayDealPosition.SetAt(m_buyDealTrace.GetRawIndex(), (double)lDealValue);
				m_arrayDealName.SetAt(m_buyDealTrace.GetRawIndex(), m_buyDealTrace.GetRawDealName());
				m_arrayDealDrawPosition.SetAt(m_buyDealTrace.GetRawIndex(), pPrice->GetAt(m_buyDealTrace.GetRawIndex()));
			}
			
//			pDealData->m_nIndex = m_buyDealTrace.GetRawIndex();
//			pDealData->m_lDealValue = m_buyDealTrace.GetRawDealValue();
//			pDealData->m_lType = m_buyDealTrace.GetRawType();
//			pDealData->m_strDealName = m_buyDealTrace.GetRawDealName();
//			pDealData->m_dPrice = m_buyDealTrace.GetRawPrice();
//			pDealData->m_dContract = m_buyDealTrace.GetRawContract();
//			pDealData->m_strListEntryNameForExit.AddTail(m_buyDealTrace.GetRawEntryNameForExit());

			m_buyDealTrace.Next(bRealState);
		}
	}
	// 리얼 데이터 처리
	else
	{
		if(m_buyDealTrace.Set(pDealData, bRealState))
		{
			// 주문시점이 [조건만족시] 인 경우
			if(m_buyDealTrace.GetTimingOrder() == 0)
			{
				//-------------------------------------------------------------
				// 강제청산 확인
				ConfirmBuyForcedLiquidation(bRealState, nInterval);

				if(m_buyDealTrace.IsEntryPosition())
					pPrice = pLowPrice;
				else if(m_buyDealTrace.IsExitPosition())
					pPrice = pHighPrice;
			
				//-------------------------------------------------------------
				// 신호표시
				if(nInterval == 0)		// 봉 갯수 그대로
				{
					long lDealValue = 0l;
					if(pPrice)
					{
						// 신호 표시 셋팅
						lDealValue = (long)m_arrayDealPosition.GetAt(m_buyDealTrace.GetRawIndex());
						long lDealValueBuy = (lDealValue & (VALENTRYBUY | VALEXITLONG | 
										FL_BUY_LOSSRATE | FL_BUY_TARGETBENEFIT | FL_BUY_HPRICECPBEAR |
										FL_BUY_TARGETVALUEPRESERVE | FL_BUY_BENEFITPRESERVE | 
										FL_BUY_MINIPRICE | FL_BUY_DAYLIQUIDATION));

						if((m_buyDealTrace.IsEntryPosition(lDealValueBuy) && m_buyDealTrace.GetRawDealValue() == 0) ||
							(m_buyDealTrace.IsExitPosition(lDealValueBuy) && m_buyDealTrace.GetRawDealValue() == 0))
						{
							// 신호 표시 초기화
							lDealValue = (long)m_arrayDealPosition.GetAt(m_buyDealTrace.GetRawIndex());
							lDealValue &= ~(VALENTRYBUY | VALEXITLONG | 
											FL_BUY_LOSSRATE | FL_BUY_TARGETBENEFIT | FL_BUY_HPRICECPBEAR |
											FL_BUY_TARGETVALUEPRESERVE | FL_BUY_BENEFITPRESERVE | 
											FL_BUY_MINIPRICE | FL_BUY_DAYLIQUIDATION);
// 2008.04.04 by LYH >>
							if(pDealData->m_lDealValue >0 && m_buyDealTrace.GetRawDealValue()==0 && m_sellDealTrace.GetCurrIndex() == m_buyDealTrace.GetRawIndex())
							{
								lDealValue |= m_buyDealTrace.GetCurrDealValue();
							}
// 2008.04.04 by LYH <<
						}
						else
						{
							lDealValue |= m_buyDealTrace.GetRawDealValue();
						}

						m_arrayDealPosition.SetAt(m_buyDealTrace.GetRawIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_buyDealTrace.GetRawIndex(), m_buyDealTrace.GetRawDealName());
						m_arrayDealDrawPosition.SetAt(m_buyDealTrace.GetRawIndex(), pPrice->GetAt(m_buyDealTrace.GetRawIndex()));
					}
					else
					{
						// 신호 표시 초기화
						lDealValue = (long)m_arrayDealPosition.GetAt(m_buyDealTrace.GetRawIndex());
						lDealValue &= ~(VALENTRYBUY | VALEXITLONG | 
										FL_BUY_LOSSRATE | FL_BUY_TARGETBENEFIT | FL_BUY_HPRICECPBEAR |
										FL_BUY_TARGETVALUEPRESERVE | FL_BUY_BENEFITPRESERVE | 
										FL_BUY_MINIPRICE | FL_BUY_DAYLIQUIDATION);

// 2008.04.04 by LYH >>
						lDealValue |= m_buyDealTrace.GetRawDealValue();

						//if(pDealData->m_lDealValue >0 && m_buyDealTrace.GetRawDealValue()==0 && m_buyDealTrace.GetCurrIndex() == m_buyDealTrace.GetRawIndex())
						if((pDealData->m_lDealValue >0 || lDealValue&FORCEDEXIT) && m_buyDealTrace.GetRawDealValue()==0 && m_buyDealTrace.GetCurrIndex() == m_buyDealTrace.GetRawIndex())
						{
							lDealValue |= m_buyDealTrace.GetCurrDealValue();
						}
// 2008.04.04 by LYH >>
						m_arrayDealPosition.SetAt(m_buyDealTrace.GetRawIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_buyDealTrace.GetRawIndex(), "");
						m_arrayDealDrawPosition.SetAt(m_buyDealTrace.GetRawIndex(), 0.);
					}
				}
				else					// 봉 갯수 변경
				{
// 2008.04.04 by LYH >>
					long lDealValueCurr = (long)m_arrayDealPosition.GetAt(m_buyDealTrace.GetCurrIndex());
					int nIndex = m_buyDealTrace.GetCurrIndex();
					if(m_buyDealTrace.GetCurrDealValue() > 0 && !(IsBuy(lDealValueCurr) || IsExitLong(lDealValueCurr)))
					{
							m_arrayDealPosition.SetAt(m_buyDealTrace.GetCurrIndex(), (double)(lDealValueCurr|m_buyDealTrace.GetCurrDealValue()));
					}
// 2008.04.04 by LYH <<

					if(pPrice)
					{
						long lDealValue = (long)m_arrayDealPosition.GetAt(m_buyDealTrace.GetRawIndex());

						lDealValue |= m_buyDealTrace.GetRawDealValue();
						m_arrayDealPosition.SetAt(m_buyDealTrace.GetRawIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_buyDealTrace.GetRawIndex(), m_buyDealTrace.GetRawDealName());
						m_arrayDealDrawPosition.SetAt(m_buyDealTrace.GetRawIndex(), pPrice->GetAt(m_buyDealTrace.GetRawIndex()));
					}
				}

				if(pPrice && m_bBuyOrderStartSignal == FALSE)
				{
					// 주문시작신호 -> 모든신호로 선택한 경우
					if(m_nOrderStartSignal == 1)
						m_bBuyOrderStartSignal = TRUE;
					// 주문시작신호 -> 진입신호로 선택한 경우
					else if(m_nOrderStartSignal == 0 && m_buyDealTrace.IsEntryPosition())
						m_bBuyOrderStartSignal = TRUE;
				}

				//-------------------------------------------------------------
				// 주문처리
				if(m_bBuyOrderStartSignal)
					m_dealDataItem.SetData(m_buyDealTrace.GetRawIndex(), 
											m_buyDealTrace.GetRawDealValue(),
											m_buyDealTrace.GetRawType(),
											m_buyDealTrace.GetRawDealName(),
											m_buyDealTrace.GetRawPrice(),
											m_buyDealTrace.GetRawContract(),
											m_buyDealTrace.GetRawEntryNameForExit());

				m_buyDealTrace.Next(bRealState);
			}
			// 주문시점이 [봉완성시] 인 경우
			else if(m_buyDealTrace.GetTimingOrder() == 1)
			{
				//-------------------------------------------------------------
				// 신호표시
				if(nInterval == 0)		// 봉 갯수 그대로
				{
					m_buyDealTrace.Copy();

					//-------------------------------------------------------------
					// 강제청산 확인
					ConfirmBuyForcedLiquidation(bRealState, nInterval);

					if(m_buyDealTrace.IsForcedLiquidation(m_buyDealTrace.GetCurrDealValue()))
						pPrice = pHighPrice;

					if(pPrice)
					{
						long lDealValue = (long)m_arrayDealPosition.GetAt(m_buyDealTrace.GetCurrIndex());
						lDealValue |= m_buyDealTrace.GetCurrDealValue();
						m_arrayDealPosition.SetAt(m_buyDealTrace.GetCurrIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_buyDealTrace.GetCurrIndex(), m_buyDealTrace.GetCurrDealName());
						m_arrayDealDrawPosition.SetAt(m_buyDealTrace.GetCurrIndex(), pPrice->GetAt(m_buyDealTrace.GetCurrIndex()));

						if(m_bBuyOrderStartSignal == FALSE)
						{
							// 주문시작신호 -> 모든신호로 선택한 경우
							if(m_nOrderStartSignal == 1)
								m_bBuyOrderStartSignal = TRUE;
							// 주문시작신호 -> 진입신호로 선택한 경우
							else if(m_nOrderStartSignal == 0 && m_buyDealTrace.IsEntryPosition(m_buyDealTrace.GetCurrDealValue()))
								m_bBuyOrderStartSignal = TRUE;
						}

						//-------------------------------------------------------------
						// 주문처리
						if(m_bBuyOrderStartSignal)
							m_dealDataItem.SetData(m_buyDealTrace.GetCurrIndex(), 
													m_buyDealTrace.GetCurrDealValue(),
													m_buyDealTrace.GetCurrType(),
													m_buyDealTrace.GetCurrDealName(),
													m_buyDealTrace.GetCurrPrice(),
													m_buyDealTrace.GetCurrContract(),
													m_buyDealTrace.GetCurrEntryNameForExit());

						m_buyDealTrace.Next(bRealState);
					}
				}
				else					// 봉 갯수 변경
				{
					//-------------------------------------------------------------
					// 강제청산 확인
					ConfirmBuyForcedLiquidation(bRealState, nInterval);

					if(m_buyDealTrace.IsEntryPosition(m_buyDealTrace.GetCurrDealValue()))
						pPrice = pLowPrice;
					else if(m_buyDealTrace.IsExitPosition(m_buyDealTrace.GetCurrDealValue()))
					{
						if(m_buyDealTrace.IsEntryPosition(m_buyDealTrace.GetOrdrDealValue()))
							pPrice = pHighPrice;
						else
							m_buyDealTrace.m_DealDataCurr.m_lDealValue = 0l;
					}

					if(pPrice)
					{
						long lDealValue = (long)m_arrayDealPosition.GetAt(m_buyDealTrace.GetCurrIndex());
						lDealValue |= m_buyDealTrace.GetCurrDealValue();
						m_arrayDealPosition.SetAt(m_buyDealTrace.GetCurrIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_buyDealTrace.GetCurrIndex(), m_buyDealTrace.GetCurrDealName());
						m_arrayDealDrawPosition.SetAt(m_buyDealTrace.GetCurrIndex(), pPrice->GetAt(m_buyDealTrace.GetCurrIndex()));

						if(m_bBuyOrderStartSignal == FALSE)
						{
							// 주문시작신호 -> 모든신호로 선택한 경우
							if(m_nOrderStartSignal == 1)
								m_bBuyOrderStartSignal = TRUE;
							// 주문시작신호 -> 진입신호로 선택한 경우
							else if(m_nOrderStartSignal == 0 && m_buyDealTrace.IsEntryPosition(m_buyDealTrace.GetCurrDealValue()))
								m_bBuyOrderStartSignal = TRUE;
						}
					}

					//-------------------------------------------------------------
					// 주문처리
					if(m_bBuyOrderStartSignal)
						m_dealDataItem.SetData(m_buyDealTrace.GetCurrIndex(), 
												m_buyDealTrace.GetCurrDealValue(),
												m_buyDealTrace.GetCurrType(),
												m_buyDealTrace.GetCurrDealName(),
												m_buyDealTrace.GetCurrPrice(),
												m_buyDealTrace.GetCurrContract(),
												m_buyDealTrace.GetCurrEntryNameForExit());

					m_buyDealTrace.Next(bRealState);
				}
			}
		}
	}
}

void CStrategyWnd::ExecuteSellDealTrace(CDealData* pDealData, BOOL bRealState/* = FALSE*/, int nInterval/* = 0*/)
{
	CIDataItem* pHighPrice = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pLowPrice = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);

	CIDataItem* pPrice = NULL;

	// 조회 데이터 처리
	if(bRealState == FALSE)
	{
		if(m_sellDealTrace.Set(pDealData, bRealState))
		{
			ConfirmSellForcedLiquidation(bRealState, nInterval);

			if(m_sellDealTrace.IsEntryPosition())
				pPrice = pHighPrice;
			else if(m_sellDealTrace.IsExitPosition())
				pPrice = pLowPrice;

			if(pPrice)
			{
				long lDealValue = (long)m_arrayDealPosition.GetAt(m_sellDealTrace.GetRawIndex());
				lDealValue |= m_sellDealTrace.GetRawDealValue();
				m_arrayDealPosition.SetAt(m_sellDealTrace.GetRawIndex(), (double)lDealValue);
				m_arrayDealName.SetAt(m_sellDealTrace.GetRawIndex(), m_sellDealTrace.GetRawDealName());
				m_arrayDealDrawPosition.SetAt(m_sellDealTrace.GetRawIndex(), pPrice->GetAt(m_sellDealTrace.GetRawIndex()));
			}

//			pDealData->m_nIndex = m_sellDealTrace.GetRawIndex();
//			pDealData->m_lDealValue = m_sellDealTrace.GetRawDealValue();
//			pDealData->m_lType = m_sellDealTrace.GetRawType();
//			pDealData->m_strDealName = m_sellDealTrace.GetRawDealName();
//			pDealData->m_dPrice = m_sellDealTrace.GetRawPrice();
//			pDealData->m_dContract = m_sellDealTrace.GetRawContract();
//			pDealData->m_strListEntryNameForExit.AddTail(m_sellDealTrace.GetRawEntryNameForExit());

			m_sellDealTrace.Next(bRealState);
		}
	}
	// 리얼 데이터 처리
	else
	{
		if(m_sellDealTrace.Set(pDealData, bRealState))
		{
			// 주문시점이 [조건만족시] 인 경우
			if(m_buyDealTrace.GetTimingOrder() == 0)
			{
				//-------------------------------------------------------------
				// 강제청산 확인
				ConfirmSellForcedLiquidation(bRealState, nInterval);

				if(m_sellDealTrace.IsEntryPosition())
					pPrice = pHighPrice;
				else if(m_sellDealTrace.IsExitPosition())
					pPrice = pLowPrice;

				//-------------------------------------------------------------
				// 신호표시
				if(nInterval == 0)		// 봉 갯수 그대로
				{
					long lDealValue = 0l;
					if(pPrice)
					{
						// 신호 표시 셋팅
//						lDealValue = (long)m_arrayDealPosition.GetAt(m_sellDealTrace.GetRawIndex());
//						lDealValue |= m_sellDealTrace.GetRawDealValue();
						lDealValue = (long)m_arrayDealPosition.GetAt(m_sellDealTrace.GetRawIndex());
						long lDealValueSell = (lDealValue & (VALENTRYSELL | VALEXITSHORT | 
										FL_SELL_LOSSRATE | FL_SELL_TARGETBENEFIT | FL_SELL_HPRICECPBEAR |
										FL_SELL_TARGETVALUEPRESERVE | FL_SELL_BENEFITPRESERVE | 
										FL_SELL_MINIPRICE | FL_SELL_DAYLIQUIDATION));

						if((m_sellDealTrace.IsEntryPosition(lDealValueSell) && m_sellDealTrace.GetRawDealValue() == 0) ||
							(m_sellDealTrace.IsExitPosition(lDealValueSell) && m_sellDealTrace.GetRawDealValue() == 0))
						{
							// 신호 표시 초기화
							lDealValue = (long)m_arrayDealPosition.GetAt(m_sellDealTrace.GetRawIndex());
							lDealValue &= ~(VALENTRYSELL | VALEXITSHORT | 
											FL_SELL_LOSSRATE | FL_SELL_TARGETBENEFIT | FL_SELL_HPRICECPBEAR |
											FL_SELL_TARGETVALUEPRESERVE | FL_SELL_BENEFITPRESERVE | 
											FL_SELL_MINIPRICE | FL_SELL_DAYLIQUIDATION);
						}
						else
						{
							lDealValue |= m_sellDealTrace.GetRawDealValue();
						}
						m_arrayDealPosition.SetAt(m_sellDealTrace.GetRawIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_sellDealTrace.GetRawIndex(), m_sellDealTrace.GetRawDealName());
						m_arrayDealDrawPosition.SetAt(m_sellDealTrace.GetRawIndex(), pPrice->GetAt(m_sellDealTrace.GetRawIndex()));
					}
					else
					{
						// 신호 표시 초기화
						lDealValue = m_arrayDealPosition.GetAt(m_sellDealTrace.GetRawIndex());
						lDealValue &= ~(VALENTRYSELL | VALEXITSHORT | 
										FL_SELL_LOSSRATE | FL_SELL_TARGETBENEFIT | FL_SELL_HPRICECPBEAR |
										FL_SELL_TARGETVALUEPRESERVE | FL_SELL_BENEFITPRESERVE | 
										FL_SELL_MINIPRICE | FL_SELL_DAYLIQUIDATION);

// 2008.04.04 by LYH >>
						//if(pDealData->m_lDealValue >0 && m_sellDealTrace.GetRawDealValue()==0 && m_sellDealTrace.GetCurrIndex() == m_sellDealTrace.GetRawIndex())
						if((pDealData->m_lDealValue >0 || lDealValue&FORCEDEXIT) && m_sellDealTrace.GetRawDealValue()==0 && m_sellDealTrace.GetCurrIndex() == m_sellDealTrace.GetRawIndex())
						{
							lDealValue |= m_sellDealTrace.GetCurrDealValue();
						}
// 2008.04.04 by LYH <<
						
						m_arrayDealPosition.SetAt(m_sellDealTrace.GetRawIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_sellDealTrace.GetRawIndex(), "");
						m_arrayDealDrawPosition.SetAt(m_sellDealTrace.GetRawIndex(), 0.);
					}
				}
				else					// 봉 갯수 변경
				{
// 2008.04.04 by LYH >>
					long lDealValueCurr = (long)m_arrayDealPosition.GetAt(m_sellDealTrace.GetCurrIndex());
					int nIndex = m_sellDealTrace.GetCurrIndex();
					if(m_sellDealTrace.GetCurrDealValue() > 0 && !(IsSell(lDealValueCurr) || IsExitShort(lDealValueCurr)))
					{
						m_arrayDealPosition.SetAt(m_sellDealTrace.GetCurrIndex(), (double)(lDealValueCurr|m_sellDealTrace.GetCurrDealValue()));
					}
// 2008.04.04 by LYH <<
					if(pPrice)
					{
						long lDealValue = (long)m_arrayDealPosition.GetAt(m_sellDealTrace.GetRawIndex());
						lDealValue |= m_sellDealTrace.GetRawDealValue();
						m_arrayDealPosition.SetAt(m_sellDealTrace.GetRawIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_sellDealTrace.GetRawIndex(), m_sellDealTrace.GetRawDealName());
						m_arrayDealDrawPosition.SetAt(m_sellDealTrace.GetRawIndex(), pPrice->GetAt(m_sellDealTrace.GetRawIndex()));
					}
				}

				if(pPrice && m_bSellOrderStartSignal == FALSE)
				{
					// 주문시작신호 -> 모든신호로 선택한 경우
					if(m_nOrderStartSignal == 1)
						m_bSellOrderStartSignal = TRUE;
					// 주문시작신호 -> 진입신호로 선택한 경우
					else if(m_nOrderStartSignal == 0 && m_sellDealTrace.IsEntryPosition())
						m_bSellOrderStartSignal = TRUE;
				}

				//-------------------------------------------------------------
				// 주문처리
				if(m_bSellOrderStartSignal)
				{
					int nDealTraceLast = m_sellDealTrace.GetRawIndex();
					int nDealDataLast = m_dealDataItem.GetLastIndex();
					if(nDealTraceLast == nDealDataLast)
					{
						CDealData* pDealDataLast = m_dealDataItem.GetDealDataByIndex(nDealDataLast);
						if(pDealDataLast)
						{
							long lDealValue = pDealDataLast->m_lDealValue;
							lDealValue &= ~(VALENTRYSELL | VALEXITSHORT | 
											FL_SELL_LOSSRATE | FL_SELL_TARGETBENEFIT | FL_SELL_HPRICECPBEAR |
											FL_SELL_TARGETVALUEPRESERVE | FL_SELL_BENEFITPRESERVE | 
											FL_SELL_MINIPRICE | FL_SELL_DAYLIQUIDATION);

							lDealValue |= m_sellDealTrace.GetRawDealValue();
							if(lDealValue)
								m_dealDataItem.SetData(m_sellDealTrace.GetRawIndex(), 
														lDealValue,
														m_sellDealTrace.GetRawType(),
														m_sellDealTrace.GetRawDealName(),
														m_sellDealTrace.GetRawPrice(),
														m_sellDealTrace.GetRawContract(),
														m_sellDealTrace.GetRawEntryNameForExit());
						}
					}
					else
					{
						m_dealDataItem.SetData(m_sellDealTrace.GetRawIndex(), 
											m_sellDealTrace.GetRawDealValue(),
											m_sellDealTrace.GetRawType(),
											m_sellDealTrace.GetRawDealName(),
											m_sellDealTrace.GetRawPrice(),
											m_sellDealTrace.GetRawContract(),
											m_sellDealTrace.GetRawEntryNameForExit());
					}
				}

				m_sellDealTrace.Next(bRealState);
			}
			// 주문시점이 [봉완성시] 인 경우
			else if(m_buyDealTrace.GetTimingOrder() == 1)
			{
				//-------------------------------------------------------------
				// 신호표시
				if(nInterval == 0)		// 봉 갯수 그대로
				{
					m_sellDealTrace.Copy();

					//-------------------------------------------------------------
					// 강제청산 확인
					ConfirmSellForcedLiquidation(bRealState, nInterval);

					if(m_sellDealTrace.IsForcedLiquidation(m_sellDealTrace.GetCurrDealValue()))
						pPrice = pHighPrice;

					if(pPrice)
					{
						long lDealValue = (long)m_arrayDealPosition.GetAt(m_sellDealTrace.GetCurrIndex());
						lDealValue |= m_sellDealTrace.GetCurrDealValue();
						m_arrayDealPosition.SetAt(m_sellDealTrace.GetCurrIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_sellDealTrace.GetCurrIndex(), m_sellDealTrace.GetCurrDealName());
						m_arrayDealDrawPosition.SetAt(m_sellDealTrace.GetCurrIndex(), pPrice->GetAt(m_sellDealTrace.GetCurrIndex()));

						if(m_bSellOrderStartSignal == FALSE)
						{
							// 주문시작신호 -> 모든신호로 선택한 경우
							if(m_nOrderStartSignal == 1)
								m_bSellOrderStartSignal = TRUE;
							// 주문시작신호 -> 진입신호로 선택한 경우
							else if(m_nOrderStartSignal == 0 && m_sellDealTrace.IsEntryPosition(m_sellDealTrace.GetCurrDealValue()))
								m_bSellOrderStartSignal = TRUE;
						}

						//-------------------------------------------------------------
						// 주문처리
						if(m_bSellOrderStartSignal)
							m_dealDataItem.SetData(m_sellDealTrace.GetCurrIndex(), 
													m_sellDealTrace.GetCurrDealValue(),
													m_sellDealTrace.GetCurrType(),
													m_sellDealTrace.GetCurrDealName(),
													m_sellDealTrace.GetCurrPrice(),
													m_sellDealTrace.GetCurrContract(),
													m_sellDealTrace.GetCurrEntryNameForExit());

						m_sellDealTrace.Next(bRealState);
					}
				}
				else					// 봉 갯수 변경
				{
					//-------------------------------------------------------------
					// 강제청산 확인
					ConfirmSellForcedLiquidation(bRealState, nInterval);

					if(m_sellDealTrace.IsEntryPosition(m_sellDealTrace.GetCurrDealValue()))
						pPrice = pHighPrice;
					else if(m_sellDealTrace.IsExitPosition(m_sellDealTrace.GetCurrDealValue()))
					{
						if(m_sellDealTrace.IsEntryPosition(m_sellDealTrace.GetOrdrDealValue()))
							pPrice = pLowPrice;
						else
							m_sellDealTrace.m_DealDataCurr.m_lDealValue = 0l;
					}

					if(pPrice)
					{
						long lDealValue = (long)m_arrayDealPosition.GetAt(m_sellDealTrace.GetCurrIndex());
						lDealValue |= m_sellDealTrace.GetCurrDealValue();
						m_arrayDealPosition.SetAt(m_sellDealTrace.GetCurrIndex(), (double)lDealValue);
						m_arrayDealName.SetAt(m_sellDealTrace.GetCurrIndex(), m_sellDealTrace.GetCurrDealName());
						m_arrayDealDrawPosition.SetAt(m_sellDealTrace.GetCurrIndex(), pPrice->GetAt(m_sellDealTrace.GetCurrIndex()));

						if(m_bSellOrderStartSignal == FALSE)
						{
							// 주문시작신호 -> 모든신호로 선택한 경우
							if(m_nOrderStartSignal == 1)
								m_bSellOrderStartSignal = TRUE;
							// 주문시작신호 -> 진입신호로 선택한 경우
							else if(m_nOrderStartSignal == 0 && m_sellDealTrace.IsEntryPosition(m_sellDealTrace.GetCurrDealValue()))
								m_bSellOrderStartSignal = TRUE;
						}
					}

					//-------------------------------------------------------------
					// 주문처리
					if(m_bSellOrderStartSignal)
						m_dealDataItem.SetData(m_sellDealTrace.GetCurrIndex(), 
												m_sellDealTrace.GetCurrDealValue(),
												m_sellDealTrace.GetCurrType(),
												m_sellDealTrace.GetCurrDealName(),
												m_sellDealTrace.GetCurrPrice(),
												m_sellDealTrace.GetCurrContract(),
												m_sellDealTrace.GetCurrEntryNameForExit());

					m_sellDealTrace.Next(bRealState);
				}
			}
		}
	}
}

void CStrategyWnd::ExecuteDealTrace()
{
	CDealData* pDealData = NULL;
	POSITION pos = m_dealDataItem.GetHeadPosition();
	while(pos)
	{
		pDealData = m_dealDataItem.GetNext(pos);
		
		if(!m_bBuyDealTraceInit)
		{
			m_bBuyDealTraceInit = m_buyDealTrace.Init(pDealData);
			InitBuyDealTrace();
		}
		else if(m_bBuyDealTraceInit)
		{
			ExecuteBuyDealTrace(pDealData);
		}

		if(!m_bSellDealTraceInit)
		{
			m_bSellDealTraceInit = m_sellDealTrace.Init(pDealData);
			InitSellDealTrace();
		}
		else if(m_bSellDealTraceInit)
		{
			ExecuteSellDealTrace(pDealData);
		}
	}
}

void CStrategyWnd::ExecuteDealTraceReal(CDealData* pDealData, int& nTotalCount, int& nInterval)
{
	// 예외적인 경우
	if(ExecuteDealTraceExceptional(pDealData, nTotalCount, nInterval))
		return;

	// 통상적인 경우
	ExecuteDealTraceUsual(pDealData, nTotalCount, nInterval);
}

BOOL CStrategyWnd::ExecuteDealTraceExceptional(CDealData* pDealData, int& nTotalCount, int& nInterval)
{
	if(pDealData == NULL || nTotalCount <= 0)
		return FALSE;

	int nCurrIndex = nTotalCount - 1;
	pDealData->m_nIndex = nCurrIndex;

	ResizeSignalInfoArray(nInterval, nTotalCount);

	// 조회데이터가 없는 경우
	if(m_dealDataItem.GetCount() == 0)
	{
		if(!m_bBuyDealTraceInit && !m_bSellDealTraceInit)
		{
			m_bBuyDealTraceInit = m_buyDealTrace.Init(pDealData);
			m_bSellDealTraceInit = m_sellDealTrace.Init(pDealData);
			InitBuyDealTrace(TRUE);
			InitSellDealTrace(TRUE);
		}

		m_dealDataItem.SetData(nCurrIndex, pDealData->m_lDealValue, pDealData->m_lType, pDealData->m_strDealName, 
								pDealData->m_dPrice, pDealData->m_dContract, &pDealData->m_strListEntryNameForExit);

		m_nLastSize = nTotalCount;
		return TRUE;
	}

	// 1틱인 경우
	if(m_nDivideState == 5 && m_nUnitPos == 1)
	{
		if(!m_bBuyDealTraceInit)
		{
			m_bBuyDealTraceInit = m_buyDealTrace.Init(pDealData);
			InitBuyDealTrace(TRUE);
		}
		else
		{
			ExecuteBuyDealTrace(pDealData, TRUE, nInterval);
		}

		if(!m_bSellDealTraceInit)
		{
			m_bSellDealTraceInit = m_sellDealTrace.Init(pDealData);
			InitSellDealTrace(TRUE);
		}
		else
		{
			ExecuteSellDealTrace(pDealData, TRUE, nInterval);
		}

		m_nLastSize = nTotalCount;
		return TRUE;
	}

	return FALSE;
}

BOOL CStrategyWnd::ExecuteDealTraceUsual(CDealData* pDealData, int& nTotalCount, int& nInterval)
{
	int nCurrIndex = nTotalCount - 1;
	pDealData->m_nIndex = nCurrIndex;

	if(!m_bBuyDealTraceInit && !m_bSellDealTraceInit)
	{
		m_bBuyDealTraceInit = m_buyDealTrace.Init(pDealData);
		m_bSellDealTraceInit = m_sellDealTrace.Init(pDealData);
		InitBuyDealTrace(TRUE, nInterval);
		InitSellDealTrace(TRUE, nInterval);
	}

	ResizeSignalInfoArray(nInterval, nTotalCount);

	if(!m_bBuyDealTraceInit)
	{
		m_bBuyDealTraceInit = m_buyDealTrace.Init(pDealData);
		InitBuyDealTrace(TRUE);
	}
	else
	{
		ExecuteBuyDealTrace(pDealData, TRUE, nInterval);
	}

	if(!m_bSellDealTraceInit)
	{
		m_bSellDealTraceInit = m_sellDealTrace.Init(pDealData);
		InitSellDealTrace(TRUE);
	}
	else
	{
		ExecuteSellDealTrace(pDealData, TRUE, nInterval);
	}

	m_nLastSize = nTotalCount;
	return TRUE;
}

void CStrategyWnd::ConfirmBuyForcedLiquidation(BOOL bRealState/* = FALSE*/, int nInterval/* = 0*/)
{
	BOOL bConfirmState = FALSE;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))				// 조회 or 리얼일 경우 조건만족시
		bConfirmState = m_buyDealTrace.IsEntryPosition(m_buyDealTrace.GetCurrDealValue());
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)									// 리얼일 경우 봉완성시
		bConfirmState = m_buyDealTrace.IsEntryPosition(m_buyDealTrace.GetOrdrDealValue());

	if(bConfirmState)
	{
		CForcedLiquidationInfo FLInfo;
		//-------- 강제청산 옵션비교 --------
		// 손절매(손실률) 옵션 비교
		if(IsLossRate())
			BuyForcedLiquidation_LossRate(FLInfo, bRealState);
		
		// 목표수익 옵션 비교
		if(IsTargetBenefit())
			BuyForcedLiquidation_TargetBenefit(FLInfo, bRealState);
		
		// 최고가대비 하락 옵션 비교
		if(IsHPriceCPBear())
			BuyForcedLiquidation_HPriceCPBear(FLInfo, bRealState);
		
		// 목표가보존 옵션 비교
		if(IsTargetValuePreserve())
			BuyForcedLiquidation_TargetValuePreserve(FLInfo, bRealState);
		
		// 이익보존 옵션 비교
		if(IsBenefitPreserve())
			BuyForcedLiquidation_BenefitPreserve(FLInfo, bRealState);
		
		// 최소가격변화 옵션 비교
		if(IsMiniPrice())
			BuyForcedLiquidation_MiniPrice(FLInfo, bRealState);
		
		// 당일청산 옵션 비교
		if(IsDayLiquidation())
			BuyForcedLiquidation_DayLiquidation(FLInfo, bRealState);
		
		// 강제청산 적용
		BuyForcedLiquidation(FLInfo, bRealState, nInterval);
	}
}

void CStrategyWnd::ConfirmSellForcedLiquidation(BOOL bRealState/* = FALSE*/, int nInterval/* = 0*/)
{
	BOOL bConfirmState = FALSE;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))				// 조회 or 리얼일 경우 조건만족시
		bConfirmState = m_sellDealTrace.IsEntryPosition(m_sellDealTrace.GetCurrDealValue());
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)									// 리얼일 경우 봉완성시
		bConfirmState = m_sellDealTrace.IsEntryPosition(m_sellDealTrace.GetOrdrDealValue());

	if(bConfirmState)
	{
		CForcedLiquidationInfo FLInfo;
		//-------- 강제청산 옵션비교 --------
		// 손절매(손실률) 옵션 비교
		if(IsLossRate())
			SellForcedLiquidation_LossRate(FLInfo, bRealState);
		
		// 목표수익 옵션 비교
		if(IsTargetBenefit())
			SellForcedLiquidation_TargetBenefit(FLInfo, bRealState);
		
		// 최고가대비 하락 옵션 비교
		if(IsHPriceCPBear())
			SellForcedLiquidation_HPriceCPBear(FLInfo, bRealState);
		
		// 목표가보존 옵션 비교
		if(IsTargetValuePreserve())
			SellForcedLiquidation_TargetValuePreserve(FLInfo, bRealState);
		
		// 이익보존 옵션 비교
		if(IsBenefitPreserve())
			SellForcedLiquidation_BenefitPreserve(FLInfo, bRealState);
		
		// 최소가격변화 옵션 비교
		if(IsMiniPrice())
			SellForcedLiquidation_MiniPrice(FLInfo, bRealState);
		
		// 당일청산 옵션 비교
		if(IsDayLiquidation())
			SellForcedLiquidation_DayLiquidation(FLInfo, bRealState);
		
		// 강제청산 적용
		SellForcedLiquidation(FLInfo, bRealState, nInterval);
	}
}

void CStrategyWnd::BuyForcedLiquidation_LossRate(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)				// 손절매로 인한 강제청산
{
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		ASSERT(m_buyDealTrace.GetCurrDealValue() == VALENTRYBUY);	// 조회 or 리얼일 경우 조건만족시
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		ASSERT(m_buyDealTrace.GetOrdrDealValue() == VALENTRYBUY);	// 리얼일 경우 봉완성시
	
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dClose = 0.;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		dClose = pClose->GetAt(m_buyDealTrace.GetCurrIndex());		// 조회 or 리얼일 경우 조건만족시	
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		dClose = pClose->GetAt(m_buyDealTrace.GetOrdrIndex());		// 리얼일 경우 봉완성시

	double dBearValue = 0.;		// 하락한 값
	// %
	if(m_uLossRate_PerOrWon == 0)
		dBearValue = dClose - (dClose * m_dLossRate); 	// 일정비율(%) 하락한 경우
	// 원
	else
		dBearValue = dClose - m_dLossRate;				// 일정금액(원) 하락한 경우

	// EntryPosition 과 ExitPosition 사이에서 손절매 옵션으로 강제청산할 Position 순회
	double dValueForCompare;
	int nFLIndx = 0;
	int nFLCount = 0;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_buyDealTrace.GetCurrIndex() + 1;				// 조회 or 리얼일 경우 조건만족시
		nFLCount = m_buyDealTrace.GetRawIndex();
	}
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_buyDealTrace.GetOrdrIndex() + 1;				// 리얼일 경우 봉완성시
		nFLCount = m_buyDealTrace.GetCurrIndex();
	}
	
	for(int nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// 매수신호가 봉의 종가에서 일정비율 또는 일정금액보다 하락한 경우
		//if(dValueForCompare <= dBearValue)
		int nVal1 = dValueForCompare*100;
		int nVal2 = dBearValue*100;
		if(nVal1 <= nVal2)
		{
			FLInfo.SetFLInfo(nIndx, FL_BUY_LOSSRATE, pHigh);
			break;
		}
	}
}

void CStrategyWnd::BuyForcedLiquidation_TargetBenefit(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)		// 목표수익으로 인한 강제청산
{
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		ASSERT(m_buyDealTrace.GetCurrDealValue() == VALENTRYBUY);
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		ASSERT(m_buyDealTrace.GetOrdrDealValue() == VALENTRYBUY);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dClose = 0.;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		dClose = pClose->GetAt(m_buyDealTrace.GetCurrIndex());		// 조회 or 리얼일 경우 조건만족시	
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		dClose = pClose->GetAt(m_buyDealTrace.GetOrdrIndex());		// 리얼일 경우 봉완성시

	double dBullValue = 0.;		// 상승한 값
	// %
	if(m_uTargetBenefit_PerOrWon == 0)
		dBullValue = dClose + (dClose * m_dTargetBenefit);	// 일정비율(%) 상승한 경우
	// 원
	else
		dBullValue = dClose + m_dTargetBenefit;				// 일정금액(원) 상승한 경우

	// EntryPosition 과 ExitPosition 사이에서 목표수익 옵션으로 강제청산할 Position 순회
	double dValueForCompare;
	int nFLIndx = 0;
	int nFLCount = 0;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_buyDealTrace.GetCurrIndex() + 1;				// 조회 or 리얼일 경우 조건만족시
		nFLCount = m_buyDealTrace.GetRawIndex();
	}
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_buyDealTrace.GetOrdrIndex() + 1;				// 리얼일 경우 봉완성시
		nFLCount = m_buyDealTrace.GetCurrIndex();
	}

	for(int nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// 매수신호가 봉의 종가에서 일정비율 또는 일정금액보다 상승한 경우
		//if(dValueForCompare >= dBullValue)
		//소수점 보정
		int nVal1 = dValueForCompare*100;
		int nVal2 = dBullValue*100;
		if(nVal1 >= nVal2)
		{
			FLInfo.SetFLInfo(nIndx, FL_BUY_TARGETBENEFIT, pHigh);
			break;
		}
	}
}

void CStrategyWnd::BuyForcedLiquidation_HPriceCPBear(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)			// 고가대비하락으로 인한 강제청산
{
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		ASSERT(m_buyDealTrace.GetCurrDealValue() == VALENTRYBUY);
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		ASSERT(m_buyDealTrace.GetOrdrDealValue() == VALENTRYBUY);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dHighValue = 0.;
	double dCloseValue = 0.;
	double dHighEach = 0.;
	double dCloseEach = 0.;

	int nFLIndx = 0;
	int nFLCount = 0;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_buyDealTrace.GetCurrIndex() + 1;				// 조회 or 리얼일 경우 조건만족시
		nFLCount = m_buyDealTrace.GetRawIndex();
	}
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_buyDealTrace.GetOrdrIndex() + 1;				// 리얼일 경우 봉완성시
		nFLCount = m_buyDealTrace.GetCurrIndex();
	}

	for(int nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		dHighEach = pHigh->GetAt(nIndx);

		// 이후 고가가 더 높은 경우 최고가로 갱신
		if(dHighValue - dHighEach < 0.)
			dHighValue = dHighEach;


		dCloseEach = pClose->GetAt(nIndx);
		double dBearValue = 0.;		// 하락한 값
		// %
		if(m_uHPriceCPBear_PerOrWon == 0)
			dBearValue = dHighValue - (dHighValue * m_dHPriceCPBear);		// 일정비율(%) 하락한 경우
		// 원
		else
			dBearValue = dHighValue - m_dHPriceCPBear;	 					// 일정금액(원) 하락한 경우

		// 일정비율 또는 일정금액으로 종가가 계산된 기준가보다 하락한 경우
		//if(dCloseEach <= dBearValue)
		int nVal1 = dCloseEach*100;
		int nVal2 = dBearValue*100;
		if(nVal1 <= nVal2)
		{
			FLInfo.SetFLInfo(nIndx, FL_BUY_HPRICECPBEAR, pHigh);
			return;
		}
	}
}

void CStrategyWnd::BuyForcedLiquidation_TargetValuePreserve(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)	// 목표가보존을 위한 강제청산
{
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		ASSERT(m_buyDealTrace.GetCurrDealValue() == VALENTRYBUY);
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		ASSERT(m_buyDealTrace.GetOrdrDealValue() == VALENTRYBUY);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);
	
	double dClose = 0.;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		dClose = pClose->GetAt(m_buyDealTrace.GetCurrIndex());		// 조회 or 리얼일 경우 조건만족시	
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		dClose = pClose->GetAt(m_buyDealTrace.GetOrdrIndex());		// 리얼일 경우 봉완성시

	double dBullValue = 0.;		// 상승한 값
	// %
	if(m_uTargetValuePreserve_PerOrWon == 0)
		dBullValue = dClose + (dClose * m_dTargetValuePreserve);	// 일정비율(%) 상승한 경우
	// 원
	else
		dBullValue = dClose + m_dTargetValuePreserve;				// 일정금액(원) 상승한 경우

	// EntryPosition 과 ExitPosition 사이에서 목표가보존 옵션으로 강제청산할 Position 순회
	double dValueForCompare = 0.;
	double dValueForTargetValuePreserve = 0.;
	int nFLIndx = 0;
	int nFLCount = 0;
	int nIndx = 0;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_buyDealTrace.GetCurrIndex() + 1;				// 조회 or 리얼일 경우 조건만족시
		nFLCount = m_buyDealTrace.GetRawIndex();
	}
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_buyDealTrace.GetOrdrIndex() + 1;				// 리얼일 경우 봉완성시
		nFLCount = m_buyDealTrace.GetCurrIndex();
	}

	for( nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// 매수신호가 목표가보존값보다 상승한 경우
		//if(dValueForCompare >= dBullValue)
		int nVal1 = dValueForCompare*100;
		int nVal2 = dBullValue*100;
		if(nVal1 >= nVal2)
		{
			dValueForTargetValuePreserve = dBullValue;
			break;
		}
	}
	
	// 목표가보존 최고가 이후의 Position 에서 ExitPosition 사이에서 최고가 하락 돌파에 대한 강제청산 Position 순회
	for(nIndx = nIndx + 1; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// 매수신호가 목표가보존값보다 하락한 경우
		//if(dValueForCompare < dValueForTargetValuePreserve)
		int nVal1 = dValueForCompare*100;
		int nVal2 = dValueForTargetValuePreserve*100;
		if(nVal1 < nVal2)
		{
			FLInfo.SetFLInfo(nIndx, FL_BUY_TARGETVALUEPRESERVE, pHigh);
			break;
		}
	}
}

void CStrategyWnd::BuyForcedLiquidation_BenefitPreserve(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)		// 이익보존을 위한 강제청산 
{
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		ASSERT(m_buyDealTrace.GetCurrDealValue() == VALENTRYBUY);
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		ASSERT(m_buyDealTrace.GetOrdrDealValue() == VALENTRYBUY);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dClose = 0.;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		dClose = pClose->GetAt(m_buyDealTrace.GetCurrIndex());		// 조회 or 리얼일 경우 조건만족시	
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		dClose = pClose->GetAt(m_buyDealTrace.GetOrdrIndex());		// 리얼일 경우 봉완성시

	double dBullValue = 0.;		// 상승한 값
	// %
	if(m_uBenefitPreserveUp_PerOrWon == 0)
		dBullValue = dClose + (dClose * m_dBenefitPreserveUp);	// 일정비율(%) 상승한 경우
	// 원
	else
		dBullValue = dClose + m_dBenefitPreserveUp;				// 일정금액(원) 상승한 경우

	// EntryPosition 과 ExitPosition 사이에서 이익보존 옵션으로 강제청산할 Position 순회
	double dValueForCompare = 0.;
	double dValueForBenefitPreserve = 0.;
	int nFLIndx = 0;
	int nFLCount = 0;
	int nIndx = 0;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_buyDealTrace.GetCurrIndex() + 1;				// 조회 or 리얼일 경우 조건만족시
		nFLCount = m_buyDealTrace.GetRawIndex();
	}
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_buyDealTrace.GetOrdrIndex() + 1;				// 리얼일 경우 봉완성시
		nFLCount = m_buyDealTrace.GetCurrIndex();
	}

	for( nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// 매수신호가 봉의 종가에서 일정비율 또는 일정금액보다 상승한 경우
		if(dValueForCompare >= dBullValue)
		{
			dValueForBenefitPreserve = dBullValue;
			break;
		}
	}

	double dBearValue = 0.;		// 하락한 값
	// %
	if(m_uBenefitPreserveDn_PerOrWon == 0)
		dBearValue = dValueForBenefitPreserve - (dValueForBenefitPreserve * m_dBenefitPreserveDn);	// 일정비율(%) 상승한 경우
	// 원
	else
		dBearValue = dValueForBenefitPreserve - m_dBenefitPreserveDn;										// 일정금액(원) 상승한 경우

	// 이익보존 최고가 이후의 Position 에서 ExitPosition 사이에서 하락율 적용하여 강제청산할 Position 순회
	for(nIndx = nIndx + 1; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// 매수신호가 봉의 종가에서 일정비율 또는 일정금액보다 하락한 경우
		//if(dValueForCompare <= dBearValue)
		int nVal1 = dValueForCompare*100;
		int nVal2 = dBearValue*100;
		if(nVal1 <= nVal2)
		{
			FLInfo.SetFLInfo(nIndx, FL_BUY_BENEFITPRESERVE, pHigh);
			break;
		}
	}
}

void CStrategyWnd::BuyForcedLiquidation_MiniPrice(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)			// 최소가격으로 인한 강제청산
{
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		ASSERT(m_buyDealTrace.GetCurrDealValue() == VALENTRYBUY);
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		ASSERT(m_buyDealTrace.GetOrdrDealValue() == VALENTRYBUY);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dClose = 0.;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		dClose = pClose->GetAt(m_buyDealTrace.GetCurrIndex());		// 조회 or 리얼일 경우 조건만족시	
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		dClose = pClose->GetAt(m_buyDealTrace.GetOrdrIndex());		// 리얼일 경우 봉완성시

	if(m_nBongCount > 0)
	{
		// EntryPosition 과 ExitPosition 사이에서 최소가격변화 옵션으로 강제청산할 Position 순회
		double dValueForCompare;
		double dRatio = m_dMiniPrice;
		int nFLIndx = 0;
		int nFLCount = 0;
		if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		{
			nFLIndx = m_buyDealTrace.GetCurrIndex() + m_nBongCount;	// 조회 or 리얼일 경우 조건만족시
			nFLCount = m_buyDealTrace.GetRawIndex();
		}
		else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		{
			nFLIndx = m_buyDealTrace.GetOrdrIndex() + m_nBongCount;	// 리얼일 경우 봉완성시
			nFLCount = m_buyDealTrace.GetCurrIndex();
		}

		for(int nIndx = nFLIndx; nIndx <= nFLCount; )
		{
			dValueForCompare = pClose->GetAt(nIndx);

			double dBullValue = 0.;		// 상승한 값
			// %
			if(m_uMiniPrice_PerOrWon == 0)
				dBullValue = dClose + (dClose * dRatio); 	// 증가되는 일정비율(%) 상승한 경우
			// 원
			else
				dBullValue = dClose + dRatio;				// 증가되는 일정금액(원) 상승한 경우

			// 종가가 일정비율 또는 일정금액이내로 횡보를 유지할 경우
			// 즉 기준가가 종가보다 높을 경우
			//if(dValueForCompare <= dBullValue)
			int nVal1 = dValueForCompare*100;
			int nVal2 = dBullValue*100;
			if(nVal1 <= nVal2)
			{
				FLInfo.SetFLInfo(nIndx, FL_BUY_MINIPRICE, pHigh);
				break;
			}
			nIndx += m_nBongCount;
			dRatio += m_dMiniPrice;
		}
	}	
}

void CStrategyWnd::BuyForcedLiquidation_DayLiquidation(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)		// 당일청산으로 인한 강제청산
{
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
		ASSERT(m_buyDealTrace.GetCurrDealValue() == VALENTRYBUY);
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		ASSERT(m_buyDealTrace.GetOrdrDealValue() == VALENTRYBUY);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);

	CString szDate;
	CString szHour;
	int		nMin;
	CString szCheckHour;
	int		nCheckMin;

	// 분 간격
	int	nInterval = m_nUnitPos;
	int nFLIndx = 0;
	int nFLCount = 0;
	if(bRealState == FALSE || (bRealState && m_buyDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_buyDealTrace.GetCurrIndex() + 1;			// 조회 or 리얼일 경우 조건만족시
		nFLCount = m_buyDealTrace.GetRawIndex();
	}
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_buyDealTrace.GetOrdrIndex() + 1;			// 리얼일 경우 봉완성시
		nFLCount = m_buyDealTrace.GetCurrIndex();
	}
	
	for(int nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		// 월일시분(예 : 10281310)
		szDate.Format("%08.8s", m_pDateItem->GetAt(nIndx));
		szHour = szDate.Mid(4, 2);
		nMin = atoi(szDate.Mid(6, 2));

		// 설정된 시분(예 : 1310)
		szCheckHour = m_szTimeDayLiquidation.Mid(0, 2);
		nCheckMin = atoi(m_szTimeDayLiquidation.Mid(2));
		if(szCheckHour.Compare(szHour) == 0 &&		// Hour Check
			nCheckMin <= nMin && abs(nCheckMin - nMin) <= nInterval)		// Min Check
		{
			FLInfo.SetFLInfo(nIndx, FL_BUY_DAYLIQUIDATION, pHigh);
			break;
		}
	}	
}

void CStrategyWnd::BuyForcedLiquidation(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/, int nInterval/* = 0*/)
{
	if(FLInfo.m_nExitIndex == SENTINEL_FLINFO || 
		FLInfo.m_lDealValue == 0L ||
		FLInfo.m_pDataItem == NULL)
		return;

	if(bRealState && m_buyDealTrace.GetTimingOrder() == 0)			// 조회 or 리얼일 경우 조건만족시
	{
		// 봉 미완성 상태 & 강제청산시점이 [봉완성시] 인 경우
		if(nInterval == 0 && m_buyDealTrace.GetTimingForcedLiquidation() == 1)
			return;
	}
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)		// 리얼일 경우 봉완성시
	{
		// 봉 미완성 상태 & 강제청산시점이 [봉완성시] 인 경우
		if(nInterval == 0 && m_buyDealTrace.GetTimingForcedLiquidation() == 1)
			return;
	}

	// 중복진입 횟수 초기화
	m_buyDealTrace.InitRepeatCount();

	// 강제청산 셋팅
	m_buyDealTrace.SetForcedLiquidation(FLInfo.m_nExitIndex, FLInfo.m_lDealValue, bRealState);
}

void CStrategyWnd::SellForcedLiquidation_LossRate(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)				// 손절매로 인한 강제청산
{
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		ASSERT(m_sellDealTrace.GetCurrDealValue() == VALENTRYSELL);	// 조회 or 리얼일 경우 조건만족시
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		ASSERT(m_sellDealTrace.GetOrdrDealValue() == VALENTRYSELL);	// 리얼일 경우 봉완성시
	
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dClose = 0.;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		dClose = pClose->GetAt(m_sellDealTrace.GetCurrIndex());		// 조회 or 리얼일 경우 조건만족시	
	else if(bRealState && m_buyDealTrace.GetTimingOrder() == 1)
		dClose = pClose->GetAt(m_sellDealTrace.GetOrdrIndex());		// 리얼일 경우 봉완성시

	double dBullValue = 0.;		// 상승한 값
	// %
	if(m_uLossRate_PerOrWon == 0)
		dBullValue = dClose + (dClose * m_dLossRate); 	// 일정비율(%) 상승한 경우
	// 원
	else
		dBullValue = dClose + m_dLossRate;				// 일정금액(원) 상승한 경우

	// EntryPosition 과 ExitPosition 사이에서 손절매 옵션으로 강제청산할 Position 순회
	double dValueForCompare;
	int nFLIndx = 0;
	int nFLCount = 0;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_sellDealTrace.GetCurrIndex() + 1;				// 조회 or 리얼일 경우 조건만족시
		nFLCount = m_sellDealTrace.GetRawIndex();
	}
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_sellDealTrace.GetOrdrIndex() + 1;				// 리얼일 경우 봉완성시
		nFLCount = m_sellDealTrace.GetCurrIndex();
	}

	for(int nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// 매수신호가 봉의 종가에서 일정비율 또는 일정금액보다 상승한 경우
		//if(dValueForCompare >= dBullValue)
		int nVal1 = dValueForCompare*100;
		int nVal2 = dBullValue*100;
		if(nVal1 >= nVal2)
		{
			FLInfo.SetFLInfo(nIndx, FL_SELL_LOSSRATE, pHigh);
			break;
		}
	}
}

void CStrategyWnd::SellForcedLiquidation_TargetBenefit(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)		// 목표수익으로 인한 강제청산
{
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		ASSERT(m_sellDealTrace.GetCurrDealValue() == VALENTRYSELL);
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		ASSERT(m_sellDealTrace.GetOrdrDealValue() == VALENTRYSELL);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dClose = 0.;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		dClose = pClose->GetAt(m_sellDealTrace.GetCurrIndex());		// 조회 or 리얼일 경우 조건만족시	
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		dClose = pClose->GetAt(m_sellDealTrace.GetOrdrIndex());		// 리얼일 경우 봉완성시

	double dBearValue = 0.;		// 하락한 값
	// %
	if(m_uTargetBenefit_PerOrWon == 0)
		dBearValue = dClose - (dClose * m_dTargetBenefit);	// 일정비율(%) 하락한 경우
	// 원
	else
		dBearValue = dClose - m_dTargetBenefit;				// 일정금액(원) 하락한 경우

	// EntryPosition 과 ExitPosition 사이에서 목표수익 옵션으로 강제청산할 Position 순회
	double dValueForCompare;
	int nFLIndx = 0;
	int nFLCount = 0;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_sellDealTrace.GetCurrIndex() + 1;				// 조회 or 리얼일 경우 조건만족시
		nFLCount = m_sellDealTrace.GetRawIndex();
	}
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_sellDealTrace.GetOrdrIndex() + 1;				// 리얼일 경우 봉완성시
		nFLCount = m_sellDealTrace.GetCurrIndex();
	}

	for(int nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// 매수신호가 봉의 종가에서 일정비율 또는 일정금액보다 하락한 경우
		//if(dValueForCompare <= dBearValue)
		int nVal1 = dValueForCompare*100;
		int nVal2 = dBearValue*100;
		if(nVal1 <= nVal2)
		{
			FLInfo.SetFLInfo(nIndx, FL_SELL_TARGETBENEFIT, pHigh);
			break;
		}
	}
}

void CStrategyWnd::SellForcedLiquidation_HPriceCPBear(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)			// 고가대비하락으로 인한 강제청산
{
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		ASSERT(m_sellDealTrace.GetCurrDealValue() == VALENTRYSELL);
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		ASSERT(m_sellDealTrace.GetOrdrDealValue() == VALENTRYSELL);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pLow = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dLowValue = 999999999999.;
	double dCloseValue = 0.;
	double dLowEach = 0.;
	double dCloseEach = 0.;

	int nFLIndx = 0;
	int nFLCount = 0;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_sellDealTrace.GetCurrIndex() + 1;				// 조회 or 리얼일 경우 조건만족시
		nFLCount = m_sellDealTrace.GetRawIndex();
	}
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_sellDealTrace.GetOrdrIndex() + 1;				// 리얼일 경우 봉완성시
		nFLCount = m_sellDealTrace.GetCurrIndex();
	}

	for(int nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		dLowEach = pLow->GetAt(nIndx);

		// 이후 저가가 더 낮은 경우 최저가로 갱신
		if(dLowValue - dLowEach > 0.)
			dLowValue = dLowEach;


		dCloseEach = pClose->GetAt(nIndx);
		double dBullValue = 0.;		// 상승한 값
		// %
		if(m_uHPriceCPBear_PerOrWon == 0)
			dBullValue = dLowValue + (dLowValue * m_dHPriceCPBear);		// 일정비율(%) 상승한 경우
		// 원
		else
			dBullValue = dLowValue + m_dHPriceCPBear;	 					// 일정금액(원) 상승한 경우

		// 일정비율 또는 일정금액으로 종가가 계산된 기준가보다 상승한 경우
		//if(dCloseEach >= dBullValue)
		int nVal1 = dCloseEach*100;
		int nVal2 = dBullValue*100;
		if(nVal1 >= nVal2)
		{
			FLInfo.SetFLInfo(nIndx, FL_SELL_HPRICECPBEAR, pHigh);
			return;
		}
	}
}

void CStrategyWnd::SellForcedLiquidation_TargetValuePreserve(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)	// 목표가보존을 위한 강제청산
{
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		ASSERT(m_sellDealTrace.GetCurrDealValue() == VALENTRYSELL);
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		ASSERT(m_sellDealTrace.GetOrdrDealValue() == VALENTRYSELL);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dClose = 0.;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		dClose = pClose->GetAt(m_sellDealTrace.GetCurrIndex());		// 조회 or 리얼일 경우 조건만족시	
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		dClose = pClose->GetAt(m_sellDealTrace.GetOrdrIndex());		// 리얼일 경우 봉완성시

	double dBearValue = 0.;		// 하락한 값
	// %
	if(m_uTargetValuePreserve_PerOrWon == 0)
		dBearValue = dClose - (dClose * m_dTargetValuePreserve);	// 일정비율(%) 하락한 경우
	// 원
	else
		dBearValue = dClose - m_dTargetValuePreserve;				// 일정금액(원) 하락한 경우

	// EntryPosition 과 ExitPosition 사이에서 목표가보존 옵션으로 강제청산할 Position 순회
	double dValueForCompare = 0.;
	double dValueForTargetValuePreserve = 0.;
	int nFLIndx = 0;
	int nFLCount = 0;
	int nIndx = 0;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_sellDealTrace.GetCurrIndex() + 1;				// 조회 or 리얼일 경우 조건만족시
		nFLCount = m_sellDealTrace.GetRawIndex();
	}
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_sellDealTrace.GetOrdrIndex() + 1;				// 리얼일 경우 봉완성시
		nFLCount = m_sellDealTrace.GetCurrIndex();
	}

	for( nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// 매수신호가 목표가보존값보다 하락한 경우
		//if(dValueForCompare <= dBearValue)
		int nVal1 = dValueForCompare*100;
		int nVal2 = dBearValue*100;
		if(nVal1 <= nVal2)
		{
			dValueForTargetValuePreserve = dBearValue;
			break;
		}
	}
	
	// 목표가보존 최고가 이후의 Position 에서 ExitPosition 사이에서 최고가 상승 돌파에 대한 강제청산 Position 순회
	for(nIndx = nIndx + 1; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// 매수신호가 목표가보존값보다 상승한 경우
		//if(dValueForCompare > dValueForTargetValuePreserve)
		int nVal1 = dValueForCompare*100;
		int nVal2 = dValueForTargetValuePreserve*100;
		if(nVal1 > nVal2)
		{
			FLInfo.SetFLInfo(nIndx, FL_SELL_TARGETVALUEPRESERVE, pHigh);
			break;
		}
	}
}

void CStrategyWnd::SellForcedLiquidation_BenefitPreserve(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)		// 이익보존을 위한 강제청산 
{
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		ASSERT(m_sellDealTrace.GetCurrDealValue() == VALENTRYSELL);
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		ASSERT(m_sellDealTrace.GetOrdrDealValue() == VALENTRYSELL);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dClose = 0.;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		dClose = pClose->GetAt(m_sellDealTrace.GetCurrIndex());		// 조회 or 리얼일 경우 조건만족시	
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		dClose = pClose->GetAt(m_sellDealTrace.GetOrdrIndex());		// 리얼일 경우 봉완성시

	double dBearValue = 0.;		// 상승한 값
	// %
	if(m_uBenefitPreserveUp_PerOrWon == 0)
		dBearValue = dClose - (dClose * m_dBenefitPreserveUp);	// 일정비율(%) 하락한 경우
	// 원
	else
		dBearValue = dClose - m_dBenefitPreserveUp;				// 일정금액(원) 하락한 경우

	// EntryPosition 과 ExitPosition 사이에서 이익보존 옵션으로 강제청산할 Position 순회
	double dValueForCompare = 0.;
	double dValueForBenefitPreserve = 0.;
	int nFLIndx = 0;
	int nFLCount = 0;
	int nIndx = 0;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_sellDealTrace.GetCurrIndex() + 1;				// 조회 or 리얼일 경우 조건만족시
		nFLCount = m_sellDealTrace.GetRawIndex();
	}
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_sellDealTrace.GetOrdrIndex() + 1;				// 리얼일 경우 봉완성시
		nFLCount = m_sellDealTrace.GetCurrIndex();
	}

	for( nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// 매수신호가 봉의 종가에서 일정비율 또는 일정금액보다 하락한 경우
		//if(dValueForCompare <= dBearValue)
		int nVal1 = dValueForCompare*100;
		int nVal2 = dBearValue*100;
		if(nVal1 <= nVal2)
		{
			dValueForBenefitPreserve = dBearValue;
			break;
		}
	}

	double dBullValue = 0.;		// 하락한 값
	// %
	if(m_uBenefitPreserveDn_PerOrWon == 0)
		dBullValue = dValueForBenefitPreserve + (dValueForBenefitPreserve * m_dBenefitPreserveDn);	// 일정비율(%) 상승한 경우
	// 원
	else
		dBullValue = dValueForBenefitPreserve + m_dBenefitPreserveDn;								// 일정금액(원) 상승한 경우

	// 이익보존 최고가 이후의 Position 에서 ExitPosition 사이에서 하락율 적용하여 강제청산할 Position 순회
	for(nIndx = nIndx + 1; nIndx <= nFLCount; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// 매수신호가 봉의 종가에서 일정비율 또는 일정금액보다 상승한 경우
		//if(dValueForCompare >= dBullValue)
		int nVal1 = dValueForCompare*100;
		int nVal2 = dBullValue*100;
		if(nVal1 >= nVal2)
		{
			FLInfo.SetFLInfo(nIndx, FL_SELL_BENEFITPRESERVE, pHigh);
			break;
		}
	}	
}

void CStrategyWnd::SellForcedLiquidation_MiniPrice(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)			// 최소가격으로 인한 강제청산
{
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		ASSERT(m_sellDealTrace.GetCurrDealValue() == VALENTRYSELL);
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		ASSERT(m_sellDealTrace.GetOrdrDealValue() == VALENTRYSELL);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dClose = 0.;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		dClose = pClose->GetAt(m_sellDealTrace.GetCurrIndex());		// 조회 or 리얼일 경우 조건만족시	
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		dClose = pClose->GetAt(m_sellDealTrace.GetOrdrIndex());		// 리얼일 경우 봉완성시

	if(m_nBongCount > 0)
	{
		// EntryPosition 과 ExitPosition 사이에서 최소가격변화 옵션으로 강제청산할 Position 순회
		double dValueForCompare;
		double dRatio = m_dMiniPrice;
		int nFLIndx = 0;
		int nFLCount = 0;
		if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		{
			nFLIndx = m_sellDealTrace.GetCurrIndex() + m_nBongCount;	// 조회 or 리얼일 경우 조건만족시
			nFLCount = m_sellDealTrace.GetRawIndex();
		}
		else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		{
			nFLIndx = m_sellDealTrace.GetOrdrIndex() + m_nBongCount;	// 리얼일 경우 봉완성시
			nFLCount = m_sellDealTrace.GetCurrIndex();
		}

		for(int nIndx = nFLIndx; nIndx <= nFLCount; )
		{
			dValueForCompare = pClose->GetAt(nIndx);

			double dBearValue = 0.;		// 하락한 값
			// %
			if(m_uMiniPrice_PerOrWon == 0)
				dBearValue = dClose - (dClose * dRatio); 	// 증가되는 일정비율(%) 상승한 경우
			// 원
			else
				dBearValue = dClose - dRatio;				// 증가되는 일정금액(원) 상승한 경우

			// 종가가 일정비율 또는 일정금액이내로 횡보를 유지할 경우
			// 즉 기준가가 종가보다 높을 경우
			//if(dValueForCompare >= dBearValue)
			int nVal1 = dValueForCompare*100;
			int nVal2 = dBearValue*100;
			if(nVal1 >= nVal2)
			{
				FLInfo.SetFLInfo(nIndx, FL_SELL_MINIPRICE, pHigh);
				break;
			}
			nIndx += m_nBongCount;
			dRatio += m_dMiniPrice;
		}
	}	
}

void CStrategyWnd::SellForcedLiquidation_DayLiquidation(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/)		// 당일청산으로 인한 강제청산
{
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
		ASSERT(m_sellDealTrace.GetCurrDealValue() == VALENTRYSELL);
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
		ASSERT(m_sellDealTrace.GetOrdrDealValue() == VALENTRYSELL);

	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);

	CString szDate;
	CString szHour;
	int		nMin;
	CString szCheckHour;
	int		nCheckMin;

	// 분 간격
	int	nInterval = m_nUnitPos;
	int nFLIndx = 0;
	int nFLCount = 0;
	if(bRealState == FALSE || (bRealState && m_sellDealTrace.GetTimingOrder() == 0))
	{
		nFLIndx = m_sellDealTrace.GetCurrIndex() + 1;			// 조회 or 리얼일 경우 조건만족시
		nFLCount = m_sellDealTrace.GetRawIndex();
	}
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)
	{
		nFLIndx = m_sellDealTrace.GetOrdrIndex() + 1;			// 리얼일 경우 봉완성시
		nFLCount = m_sellDealTrace.GetCurrIndex();
	}

	for(int nIndx = nFLIndx; nIndx <= nFLCount; ++nIndx)
	{
		// 월일시분(예 : 10281310)
		szDate.Format("%08.8s", m_pDateItem->GetAt(nIndx));
		szHour = szDate.Mid(4, 2);
		nMin = atoi(szDate.Mid(6, 2));

		// 설정된 시분(예 : 1310)
		szCheckHour = m_szTimeDayLiquidation.Mid(0, 2);
		nCheckMin = atoi(m_szTimeDayLiquidation.Mid(2));
		if(szCheckHour.Compare(szHour) == 0 &&		// Hour Check
			nCheckMin <= nMin && abs(nCheckMin - nMin) <= nInterval)		// Min Check
		{
			FLInfo.SetFLInfo(nIndx, FL_SELL_DAYLIQUIDATION, pHigh);
			break;
		}
	}	
}

void CStrategyWnd::SellForcedLiquidation(CForcedLiquidationInfo& FLInfo, BOOL bRealState/* = FALSE*/, int nInterval/* = 0*/)
{
	if(FLInfo.m_nExitIndex == SENTINEL_FLINFO || 
		FLInfo.m_lDealValue == 0L ||
		FLInfo.m_pDataItem == NULL)
		return;

	if(bRealState && m_sellDealTrace.GetTimingOrder() == 0)			// 조회 or 리얼일 경우 조건만족시
	{
		// 봉 미완성 상태 & 강제청산시점이 [봉완성시] 인 경우
		if(nInterval == 0 && m_sellDealTrace.GetTimingForcedLiquidation() == 1)
			return;
	}
	else if(bRealState && m_sellDealTrace.GetTimingOrder() == 1)	// 리얼일 경우 봉완성시
	{
		// 봉 미완성 상태 & 강제청산시점이 [봉완성시] 인 경우
		if(nInterval == 0 && m_sellDealTrace.GetTimingForcedLiquidation() == 1)
			return;
	}

	// 중복진입 횟수 초기화
	m_sellDealTrace.InitRepeatCount();

	// 강제청산 셋팅
	m_sellDealTrace.SetForcedLiquidation(FLInfo.m_nExitIndex, FLInfo.m_lDealValue, bRealState);
}

//성과분석은 일루 오지..
void CStrategyWnd::OnSTSetAPDataST()
{
	m_Status = STRATEGY_RUN;

	m_ptrArraySDataItem.RemoveAll();
	m_ptrArrayPlot.RemoveAll();
	m_ptrArrayHistoryDataItem.RemoveAll();			

	CPtrArray *pHistoryData = m_pLogicRootItem->GetPArrayHistoryData();
	int nSizeOfHistory = pHistoryData->GetSize();
	for(int nPosOfHistory = 0 ; nPosOfHistory<nSizeOfHistory;nPosOfHistory++)
	{
		m_ptrArrayHistoryDataItem.Add(pHistoryData->GetAt(nPosOfHistory));
	}

	CTime t = CTime::GetCurrentTime();
	m_strCurrentTime = t.Format("%Y%m%d");
	m_strCurrentTime.Delete(0,2);
	m_pDateItem = m_pLogicRootItem->GetDateItem();
	m_nLastSize = m_pDateItem->GetSize();
	m_strTimeTypeNum = m_pLogicRootItem->GetTime();

	CDealData *pDealData = NULL;
	for(int nPos=0;nPos < m_nLastSize; nPos++)
	{
		pDealData = m_pLogicRootItem->GetValue(m_pDateItem->GetAt(nPos),m_pDateItem->GetType(),(LPCTSTR)m_strCurrentTime,nPos, m_strTimeTypeNum);

		// 조회처리시 봉이 2봉 이상으로 설정되어 있고 
		// 조회된 맨 마지막 봉이 신호조건을 만족하는 경우 (아직 봉이 완성되지 않은 상태)
		// 기존은 봉이 완성되지 않았지만 신호조건을 만족하므로 신호표시가 되었다.
		// 하지만 이로 인해 리얼 업데이트시 신호조건을 파악하는데 버그가 발생하여
		// 맨 마지막 봉의 신호조건은 무시하도록 처리한다.
		if( (m_nDivideState == 4) && /*m_nUnitPos != 60) &&*/		// 분봉에서만 적용됨
			(nPos == m_nLastSize - 1) &&							// 맨 마지막 봉
			((pDealData->m_lDealValue & VALBUY) || (pDealData->m_lDealValue & VALSELL)))	// 신호조건
			break;

		m_dealDataItem.SetData(nPos
								,pDealData->m_lDealValue
								,pDealData->m_lType
								,pDealData->m_strDealName
								,pDealData->m_dPrice
								,pDealData->m_dContract
								,&pDealData->m_strListEntryNameForExit);
	}
	

	//-------- 초기화 상태 셋팅 -------- 
	InitStrategySignalInfo();

	m_buyDealTrace.SetRepeatInfo(m_nUseRepeatEntry, m_nRepeatEntryCount);
	m_sellDealTrace.SetRepeatInfo(m_nUseRepeatEntry, m_nRepeatEntryCount);	

	ExecuteDealTrace();

	m_buyDealTrace.SetTimingInfo(m_nTiming_Order, m_nTiming_ForcedLiquidation);
	m_sellDealTrace.SetTimingInfo(m_nTiming_Order, m_nTiming_ForcedLiquidation);

//	//-------- 전략신호정보 초기화 -------- 
//	CDealData* pEntryDealData	= NULL;
//	CDealData* pExitDealData	= NULL;
//	CDealData* pDealData_ForReal = NULL;
//	InitStrategySignalInfo();
//
//	//--------  신호정보 조회시작 -------- 
//	m_bIsFirstSignal = TRUE;
//	m_nRepeatCount = 0;								// 중복진입 횟수추적 초기화
//	m_bCheckPrev_EntryPosition = TRUE;				// 이전 신호가 진입신호
//	BOOL bExist_ForcedLiquidation = FALSE;			// 강제청산 발생유무
//	POSITION pos = m_dealDataItem.GetHeadPosition();
//	while(pos)
//	{
//		// 진입 Position
//		pEntryDealData = m_dealDataItem.GetNext(pos);
//
//		// 청산신호가 맨처음인 경우 (이 조건문에 TRUE인 경우 청산신호였음을 의미함)
//		if(IsFirstLiquidation(pEntryDealData, bExist_ForcedLiquidation))
//			continue;
//
//		// 진입신호가 마지막인 경우
//		if(IsLastEntry(pEntryDealData, bExist_ForcedLiquidation, pos))
//			break;
//
//		// 청산 Position
//		pExitDealData = m_dealDataItem.GetNext(pos);
//
//		// 강제청산 옵션확인
//		Confirm_ForcedLiquidation(pEntryDealData, pExitDealData, bExist_ForcedLiquidation);
//	}
	

	//20081111 이문수 >>
	//변수최적화에서만 쓰여야 한다.////
	//((CLogicST*)m_pLogicRootItem)->SetPosition(NULL);	
	m_pLogicRootItem->SetPosition(NULL);


	m_DrawInfo.bIsInRealTime = FALSE;
	m_bHasAPData = TRUE;

	m_ptrArraySDataItem.Add(m_pDateItem);
	m_DrawInfo.pPtrArraySDataItem = &m_ptrArraySDataItem;

	

	m_DrawInfo.pPtrArrayHistoryDataItem = &m_ptrArrayHistoryDataItem;
	m_DrawInfo.pPtrArrayPlot = &m_ptrArrayPlot;

	
	m_DrawInfo.pDealDataItem = &m_dealDataItem;
	m_DrawInfo.pArDealPosition = &m_arrayDealPosition;
	m_DrawInfo.pArDealName = &m_arrayDealName;
	m_DrawInfo.pArDealDrawPosition = &m_arrayDealDrawPosition;
	m_DrawInfo.STPath = m_strStrategyPath;//KHD : Path : 성과분석에서 저장정보를 보내기위해..
	

	if(m_bUseDayLiquidation)
	{
		ExitAtTodayByTimeSelected();
	}




//	// 실시간 업데이트를 위한 준비
//	// 주문정보 셋팅완료 여부 초기화
//	m_bSetComplete_OrderInfo = FALSE;
//	
//	// 주문정보 제거가능 여부 초기화
//	m_bCanRemove_OrderInfo = TRUE;
}

void CStrategyWnd::OnSTSetRealDataST()
{
	int nTotalCount = m_pDateItem->GetSize();
	int nCurrIndex = nTotalCount - 1;
	int nInterval = nTotalCount - m_nLastSize;

	CDealData* pDealData = m_pLogicRootItem->GetValue(m_pDateItem->GetAt(nCurrIndex), m_pDateItem->GetType(), (LPCTSTR)m_strCurrentTime, nCurrIndex, m_strTimeTypeNum);

	ExecuteDealTraceReal(pDealData, nTotalCount, nInterval);

//	// 실시간 업데이트후 총 봉신호 갯수
//	int nTotalSignalCnt = m_pDateItem->GetSize();
//	
//	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
//	CIDataItem* pLow = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);
//	int nInterval = nTotalSignalCnt - m_nLastSize;			// 실시간 업데이트로 추가된 봉신호 갯수
//
//	// 신호정보가 없을 경우 실시간 업데이트된 봉신호만 셋팅
//	CDealData* pDealDataReal = m_pLogicRootItem->GetValue(m_pDateItem->GetAt(nTotalSignalCnt - 1), m_pDateItem->GetType(), (LPCTSTR)m_strCurrentTime, nTotalSignalCnt - 1, m_strTimeTypeNum);
//
//	// 조회데이터가 없거나 1틱일 경우
//	if(m_dealDataItem.GetCount() == 0 || (m_nDivideState == 5 && m_nUnitPos == 1))
//	{
//		ResizeSignalInfoArray(nInterval, nTotalSignalCnt);
//
//		// 조회데이터가 없는 경우
//		if(m_dealDataItem.GetCount() == 0)
//		{
//			int nRealPosition = nTotalSignalCnt - 1;
//			m_dealDataItem.SetData(nRealPosition, 
//									pDealDataReal->m_lDealValue,
//									pDealDataReal->m_lType,
//									pDealDataReal->m_strDealName,
//									pDealDataReal->m_dPrice,
//									pDealDataReal->m_dContract,
//									&pDealDataReal->m_strListEntryNameForExit);
//
//			// 주문정보 셋팅완료 여부 초기화
//			m_bSetComplete_OrderInfo = FALSE;
//			
//			// 주문정보 제거가능 여부 초기화
//			m_bCanRemove_OrderInfo = TRUE;
//
//			m_nLastSize = nTotalSignalCnt;
//			return;
//		}
//
//		// 1틱일 경우
//		CDealData* pEntryDealData = m_dealDataItem.GetTail();
//		CDealData* pExitDealData = pDealDataReal;
//
//		// 중복진입 허용 여부 확인
//		if((m_nUseRepeatEntry && m_nRepeatEntryCount >= m_nRepeatCount) ||
//		   (m_nUseRepeatEntry == 0 && 
//			   (IsBuyOrExitShort(pEntryDealData->m_lDealValue) && (IsSellOrExitLong(pExitDealData->m_lDealValue) || IsForcedLiquidation(pExitDealData->m_lDealValue)) ||
//			   (IsBuyOrExitShort(pExitDealData->m_lDealValue) && (IsSellOrExitLong(pEntryDealData->m_lDealValue) || IsForcedLiquidation(pEntryDealData->m_lDealValue))))))
//		{
//			pExitDealData->m_nIndex = nTotalSignalCnt - 1;
//
//			// 진입 - 진입/청산/일반신호인 경우
//			BOOL bExist_ForcedLiquidation = FALSE;		// 강제청산 발생유무
//			if(IsBuyOrExitShort(pEntryDealData->m_lDealValue))
//			{
//				// 강제청산 옵션확인
//				Confirm_ForcedLiquidation_ForReal(pEntryDealData, pExitDealData, bExist_ForcedLiquidation);
//			}
//			// // 청산 - 진입/일반신호인 경우
//			else if(IsSellOrExitLong(pEntryDealData->m_lDealValue) || IsForcedLiquidation(pEntryDealData->m_lDealValue))
//			{
//				// 새로 들어온 신호가 진입신호인 경우
//				IsLastEntry_ForReal(pEntryDealData, pExitDealData, bExist_ForcedLiquidation);
//			}
//		}
//
//		m_TempDealData.Clear();
//
//		// 주문정보 셋팅완료 여부 초기화
//		m_bSetComplete_OrderInfo = FALSE;
//		
//		// 주문정보 제거가능 여부 초기화
//		m_bCanRemove_OrderInfo = TRUE;
//
//		m_nLastSize = nTotalSignalCnt;
//		return;
//	}
//
//	if(nInterval != 0)
//	{
//		ResizeSignalInfoArray(nInterval, nTotalSignalCnt);
//
//		CDealData* pEntryDealData = m_dealDataItem.GetTail();
//		CDealData* pExitDealData = &m_TempDealData;
//
//		// 중복진입 허용 여부 확인
//		if((m_nUseRepeatEntry && m_nRepeatEntryCount >= m_nRepeatCount) || m_nUseRepeatEntry == 0)
//		{
//			pExitDealData->m_nIndex = nTotalSignalCnt - 1;
//
//			// 주문시점 (1 : 봉 완성시)
//			if(m_nTiming_Order == 1)
//			{
//				// 강제청산이 조건만족시일 경우
//				if(!m_nTiming_ForcedLiquidation && IsForcedLiquidation(m_TempForcedDealData.m_lDealValue))
//				{
//					// 봉완성시의 주문시점이 조건만족시의 강제창산 시점과 같을 경우
//					if(m_TempForcedDealData.m_nIndex <= m_TempDealData.m_nIndex &&
//						(!IsSellOrExitLong(m_TempDealData.m_lDealValue) || !IsForcedLiquidation(m_TempDealData.m_lDealValue)))
//					{
//						m_TempDealData = m_TempForcedDealData;
//						m_TempForcedDealData.Clear();
//					}
//				}
//
//				int nRealPosition = -1;
//				if(IsBuyOrExitShort(pExitDealData->m_lDealValue) && (IsSellOrExitLong(pEntryDealData->m_lDealValue) || IsForcedLiquidation(pEntryDealData->m_lDealValue)))
//				{
//					// 신호모양이 아래쪽에 놓이므로 저가로 셋팅
//					nRealPosition = pExitDealData->m_nIndex;
//					m_arrayDealPosition.SetAt(nRealPosition, pExitDealData->m_lDealValue);
//					m_arrayDealName.SetAt(nRealPosition, pExitDealData->m_strDealName);
//					m_arrayDealDrawPosition.SetAt(nRealPosition, pLow->GetAt(nRealPosition));
//
//					// 중복진입 횟수 증가
//					m_nRepeatCount++;
////					FILE *fp = fopen("c:\\Logs\\RepeatCount.txt", "a+t");
////					if(fp != NULL) 
////					{
////						fprintf(fp, "[++DivideState][%d]-[UnitPos][%d]-[RepeatCount][%d]-[Func][%s]\n", m_nDivideState, m_nUnitPos, m_nRepeatCount, "OnSTSetRealDataST()봉완성");
////						fclose(fp);
////					}
//				}
//				else if(IsBuyOrExitShort(pEntryDealData->m_lDealValue) && (IsSellOrExitLong(pExitDealData->m_lDealValue) || IsForcedLiquidation(pExitDealData->m_lDealValue)))
//				{
//					// 신호모양이 위쪽에 놓이므로 고가로 셋팅
//					nRealPosition = pExitDealData->m_nIndex;
//					m_arrayDealPosition.SetAt(nRealPosition, pExitDealData->m_lDealValue);
//					m_arrayDealName.SetAt(nRealPosition, pExitDealData->m_strDealName);
//					m_arrayDealDrawPosition.SetAt(nRealPosition, pHigh->GetAt(nRealPosition));
//
//					// 중복진입 횟수추적 초기화
//					m_nRepeatCount = 0;
////					FILE *fp = fopen("c:\\Logs\\RepeatCount.txt", "a+t");
////					if(fp != NULL) 
////					{
////						fprintf(fp, "--[DivideState][%d]-[UnitPos][%d]-[RepeatCount][%d]-[Func][%s]\n", m_nDivideState, m_nUnitPos, m_nRepeatCount, "OnSTSetRealDataST()봉완성");
////						fclose(fp);
////					}
//				}
//				else
//				{
//					// 신호없음
//					m_arrayDealPosition.SetAt(pExitDealData->m_nIndex, pExitDealData->m_nIndex);
//					m_arrayDealName.SetAt(pExitDealData->m_nIndex, "");
//					m_arrayDealDrawPosition.SetAt(pExitDealData->m_nIndex, 0);
//				}
//
//				m_dealDataItem.SetData(pExitDealData->m_nIndex, 
//										pExitDealData->m_lDealValue,
//										pExitDealData->m_lType,
//										pExitDealData->m_strDealName,
//										pExitDealData->m_dPrice,
//										pExitDealData->m_dContract,
//										&pExitDealData->m_strListEntryNameForExit);	
//			}
//			// 조건만족시
//			else
//			{
//				// 강제청산이 봉완성시일 경우
//				if(m_nTiming_ForcedLiquidation && IsForcedLiquidation(m_TempDealData.m_lDealValue))
//				{
//					// 조건만족시의 주문시점이 봉완성시의 강제창산 시점과 같을 경우
//					if(m_TempCondDealData.m_nIndex == -1 || 
//						(m_TempDealData.m_nIndex <= m_TempCondDealData.m_nIndex && 
//						(!IsSellOrExitLong(m_TempCondDealData.m_lDealValue) || !IsForcedLiquidation(m_TempCondDealData.m_lDealValue))))
//					{
//						m_TempCondDealData = m_TempDealData;
//					}
//
//					// 중복진입 횟수추적 초기화
//					m_nRepeatCount = 0;
////					FILE *fp = fopen("c:\\Logs\\RepeatCount.txt", "a+t");
////					if(fp != NULL) 
////					{
////						fprintf(fp, "--[DivideState][%d]-[UnitPos][%d]-[RepeatCount][%d]-[Func][%s]\n", m_nDivideState, m_nUnitPos, m_nRepeatCount, "OnSTSetRealDataST()");
////						fclose(fp);
////					}
//				}
//				m_dealDataItem.SetData(m_TempCondDealData.m_nIndex, 
//										m_TempCondDealData.m_lDealValue,
//										m_TempCondDealData.m_lType,
//										m_TempCondDealData.m_strDealName,
//										m_TempCondDealData.m_dPrice,
//										m_TempCondDealData.m_dContract,
//										&m_TempCondDealData.m_strListEntryNameForExit);
//			}
//		}
//
//		m_TempDealData.Clear();
//		m_TempCondDealData.Clear();
//
//		// 주문정보 셋팅완료 여부 초기화
//		m_bSetComplete_OrderInfo = FALSE;
//		
//		// 주문정보 제거가능 여부 초기화
//		m_bCanRemove_OrderInfo = TRUE;
//
//		m_nLastSize = nTotalSignalCnt;
//		return;
//	}
//
//	// 마지막 및 실시간 업데이트된 신호정보를 셋팅
//	CDealData* pEntryDealData = m_dealDataItem.GetTail();
//	CDealData* pExitDealData = pDealDataReal;
//	pExitDealData->m_nIndex = nTotalSignalCnt - 1;
//
//	// 마지막 봉신호가 일반 실시간 또는 진입신호인 경우
//	BOOL bExist_ForcedLiquidation = FALSE;		// 강제청산 발생유무
//	if(IsBuyOrExitShort(pEntryDealData->m_lDealValue))
//	{
//		// 강제청산 옵션확인
//		Confirm_ForcedLiquidation_ForReal(pEntryDealData, pExitDealData, bExist_ForcedLiquidation);
//	}
//	// 마지막 봉신호가 청산 또는 강제청산신호인 경우
//	else if(IsSellOrExitLong(pEntryDealData->m_lDealValue) || IsForcedLiquidation(pEntryDealData->m_lDealValue))
//	{
//		// 새로 들어온 신호가 진입신호인 경우
//		IsLastEntry_ForReal(pEntryDealData, pExitDealData, bExist_ForcedLiquidation);
//	}
//
//	m_nLastSize = nTotalSignalCnt;
}

void CStrategyWnd::ResizeSignalInfoArray(int nInterval, int nTotalSignalCnt)
{
	// 총 봉신호 갯수가 줄어든 경우 (차트에서의 데이터 관리로 인함)
	if(nInterval < 0)
	{
		// 줄어든 봉신호 갯수만큼 과거 봉신호를 삭제
		m_arrayDealPosition.RemoveAt(0, (nInterval * (-1)) + 1);
		m_arrayDealName.RemoveAt(0, (nInterval * (-1)) + 1);
		m_arrayDealDrawPosition.RemoveAt(0, (nInterval * (-1)) + 1);

		// 실시간 업데이트후 총 봉신호 갯수로 Size 재조정
		m_arrayDealPosition.SetSize(nTotalSignalCnt);
		m_arrayDealName.SetSize(nTotalSignalCnt);
		m_arrayDealDrawPosition.SetSize(nTotalSignalCnt);
	}
	// 총 봉신호 갯수가 늘어난 경우 
	else if(nInterval > 0)
	{
		// 실시간 업데이트후 총 봉신호 갯수로 Size 재조정
		m_arrayDealPosition.SetSize(nTotalSignalCnt);
		m_arrayDealName.SetSize(nTotalSignalCnt);
		m_arrayDealDrawPosition.SetSize(nTotalSignalCnt);
	}
}

LRESULT CStrategyWnd::OnSTDataManagerMessageST( WPARAM wParam, LPARAM lParam)
{
	CLogicST *pLogicST= (CLogicST *)wParam;
	pLogicST->ReceiveFromSTDataManager(lParam);//DrawInfo를 보낸다.:KHD
	
	if(m_pLogicRootItem->IsFilled())
	{
		m_DrawInfo.bIsInRealTime = TRUE;
		if(!m_bHasAPData)
		{
			OnSTSetAPDataST();
		}
		else
		{
			OnSTSetRealDataST();
		}		

		// 신호정보 및 주문정보 적용
		GetParent()->SendMessage(RMSG_STDATAMANAGER_RESULTDATA,m_lKey,(LPARAM)&m_DrawInfo);

//		// 조건만족시에 대한 주문정보 적용 후 주문정보 제거
//		// 조건만족시 최초 신호에 대한 주문만 실행되기 때문
//		if(m_bSetComplete_OrderInfo && m_bCanRemove_OrderInfo && (m_nDivideState != 5 || m_nUnitPos != 1))
//		{
//			m_dealDataItem.RemoveTail();
//			m_bCanRemove_OrderInfo = FALSE;
//		}

		m_pLogicRootItem->InitFilledSet();
	}	
	return 1L;
}

BOOL CStrategyWnd::IsRunning()
{
	return m_bIsRunning;
}

BOOL CStrategyWnd::HasAPData()
{
	return m_bHasAPData;
}

CString CStrategyWnd::GetInputCode()
{
	return m_strInputCode;
}

CString CStrategyWnd::GetEndDate()
{
	return m_strEndDate;
}

long CStrategyWnd::GetCount()
{
	if(m_pLogicRootItem)
	{
		return (m_pLogicRootItem->GetCount()+1);
	}
	return 0;	
}

long CStrategyWnd::GetDataCount(long lIndex)
{
	if(m_pLogicRootItem)
	{
		CLogicST *pLogicST = m_pLogicRootItem->GetLogicST(lIndex);
		if(pLogicST)
		{
			return pLogicST->GetDataCount();
		}
		else
		{
			return m_pDateItem->GetSize();
		}
	}
	return 0;
}

CString CStrategyWnd::GetInputs(long lIndexST, long lIndexInput)
{
	CLogicST *pLogicST = m_pLogicRootItem->GetLogicST(lIndexST);
	if(pLogicST)
	{
		return pLogicST->GetInputs(lIndexInput);
	}
	return "#";
}

CString CStrategyWnd::GetTime(long lIndex)
{
	CLogicST *pLogicST = m_pLogicRootItem->GetLogicST(lIndex);
	if(pLogicST)
	{
		return pLogicST->GetTime();
	}
	else
	{
		return m_pLogicRootItem->GetTime();
	}

	return "";
}

CString CStrategyWnd::GetName(long lIndex)
{
	CString strName;
	CLogicST *pLogicST = m_pLogicRootItem->GetLogicST(lIndex);
	if(pLogicST)
	{
		strName = pLogicST->GetPath();
	}
	else
	{
		strName = m_strStrategyPath;
	}
	int nPos = strName.ReverseFind('\\');
	if(nPos!=-1)
	{
		strName.Delete(0,nPos+1);
	}
	return strName;
}

CString CStrategyWnd::GetPath(long lIndex)
{
	CLogicST *pLogicST = m_pLogicRootItem->GetLogicST(lIndex);
	if(pLogicST)
	{
		return pLogicST->GetPath();
	}
	else
	{
		return m_strStrategyPath;
	}
	return "";
}

long CStrategyWnd::GetPtrDrawInfo(long lIndex)
{
	if(!m_pLogicRootItem) return NULL;
	CLogicST *pLogicST = m_pLogicRootItem->GetLogicST(lIndex);
	if(pLogicST)
	{
		return pLogicST->GetPtrST();
	}
	else
	{
		return (long)&m_DrawInfo;
	}
	return NULL;
}

void CStrategyWnd::RemoveAllArray()
{
	CStrategyResult *pStrategyResult = NULL;
	int nSize = m_arrayStrategyResult.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		pStrategyResult = m_arrayStrategyResult.GetAt(nPos);
		delete pStrategyResult;
	}
	m_arrayStrategyResult.RemoveAll();
}

CStrategyResult *CStrategyWnd::CreateStrategyResultData()
{
	CStrategyResult *pStrategyResult = new CStrategyResult;
	m_arrayStrategyResult.Add(pStrategyResult);
	return pStrategyResult;
}


void CStrategyWnd::CalaulateStrategyResultData()
{
	int nLastPos = m_arrayStrategyResult.GetSize() - 1;
	CStrategyResult *pStrategyResultLast = m_arrayStrategyResult.GetAt(nLastPos);
	CStrategyResult *pStrategyResultBefore = NULL;
	if(nLastPos>0)
		pStrategyResultBefore = m_arrayStrategyResult.GetAt(nLastPos-1);

	// 매수청산
	if(pStrategyResultLast->m_DealType==CStrategyResult::EXITLONG)
	{
		CStrategyResult *pStrategyResultInArray = NULL;
		CStrategyResult *pStrategyResultExitBefore = NULL;
		double dSumBought = 0;

		if(m_pFeeCal && m_pSlippageCal)
		{
			m_pFeeCal->Init();
			m_pSlippageCal->Init();
			
			// 중복진입을 고려하여 For문으로 조회
			for(int nPos=(nLastPos-1);nPos>=0;nPos--)
			{
				pStrategyResultInArray = m_arrayStrategyResult.GetAt(nPos);
				if(pStrategyResultInArray->m_DealType!=CStrategyResult::BUY)
				{
					pStrategyResultExitBefore = pStrategyResultInArray;
					break;
				}
				dSumBought += pStrategyResultInArray->m_dAmount;
//				m_pFeeCal->Entry(pStrategyResultInArray->m_dPrice,pStrategyResultInArray->m_lQty);
//				m_pSlippageCal->Entry(pStrategyResultInArray->m_dPrice,pStrategyResultInArray->m_lQty);
			}
			pStrategyResultLast->m_dProfit = pStrategyResultLast->m_dAmount - dSumBought;
//				- m_pFeeCal->Exit(pStrategyResultLast->m_dPrice, pStrategyResultLast->m_lQty)
//				- m_pSlippageCal->Exit(pStrategyResultLast->m_dPrice, pStrategyResultLast->m_lQty);
		}
		else
		{
			// 중복진입을 고려하여 For문으로 조회
			for(int nPos=(nLastPos-1);nPos>=0;nPos--)
			{
				pStrategyResultInArray = m_arrayStrategyResult.GetAt(nPos);
				if(pStrategyResultInArray->m_DealType!=CStrategyResult::BUY)
				{
					pStrategyResultExitBefore = pStrategyResultInArray;
					break;
				}
				dSumBought += pStrategyResultInArray->m_dAmount;
			}
			pStrategyResultLast->m_dProfit = pStrategyResultLast->m_dAmount - dSumBought;
		}
		pStrategyResultLast->m_dProfitRate = pStrategyResultLast->m_dProfit/dSumBought*100;
		if(pStrategyResultExitBefore)
		{
			pStrategyResultLast->m_dProfitAccmulated 
				= pStrategyResultExitBefore->m_dProfitAccmulated + pStrategyResultLast->m_dProfit;			
		}
		else
		{
			pStrategyResultLast->m_dProfitAccmulated = pStrategyResultLast->m_dProfit;
		}
		if(pStrategyResultBefore)
		{
			pStrategyResultLast->m_dEnteredAmount = pStrategyResultBefore->m_dEnteredAmount;
			pStrategyResultLast->m_dProfitRateAccmulated 
				= pStrategyResultLast->m_dProfitAccmulated/pStrategyResultLast->m_dEnteredAmount*100;
		}

	}
	// 매도청산
	else if(pStrategyResultLast->m_DealType==CStrategyResult::EXITSHORT)
	{
		CStrategyResult *pStrategyResultInArray = NULL;
		CStrategyResult *pStrategyResultExitBefore = NULL;
		double dSumBought = 0;
		if(m_pFeeCal&&m_pSlippageCal)
		{
			m_pFeeCal->Init();
			m_pSlippageCal->Init();

			// 중복진입을 고려하여 For문으로 조회
			for(int nPos=(nLastPos-1);nPos>=0;nPos--)
			{
				pStrategyResultInArray = m_arrayStrategyResult.GetAt(nPos);
				if(pStrategyResultInArray->m_DealType!=CStrategyResult::SELL)
				{
					pStrategyResultExitBefore = pStrategyResultInArray;
					break;
				}
				dSumBought += pStrategyResultInArray->m_dAmount;			
//				m_pFeeCal->Entry(pStrategyResultInArray->m_dPrice,pStrategyResultInArray->m_lQty);
//				m_pSlippageCal->Entry(pStrategyResultInArray->m_dPrice,pStrategyResultInArray->m_lQty);
			}
			pStrategyResultLast->m_dProfit = dSumBought - pStrategyResultLast->m_dAmount;
//					- m_pFeeCal->Exit(pStrategyResultLast->m_dPrice, pStrategyResultLast->m_lQty)
//					- m_pSlippageCal->Exit(pStrategyResultLast->m_dPrice, pStrategyResultLast->m_lQty);
		}
		else
		{
			// 중복진입을 고려하여 For문으로 조회
			for(int nPos=(nLastPos-1);nPos>=0;nPos--)
			{
				pStrategyResultInArray = m_arrayStrategyResult.GetAt(nPos);
				if(pStrategyResultInArray->m_DealType!=CStrategyResult::SELL)
				{
					pStrategyResultExitBefore = pStrategyResultInArray;
					break;
				}
				dSumBought += pStrategyResultInArray->m_dAmount;			
			}
			pStrategyResultLast->m_dProfit = dSumBought - pStrategyResultLast->m_dAmount;
		}

		pStrategyResultLast->m_dProfitRate = pStrategyResultLast->m_dProfit/dSumBought*100;
		if(pStrategyResultExitBefore)
		{
			pStrategyResultLast->m_dProfitAccmulated 
				= pStrategyResultExitBefore->m_dProfitAccmulated + pStrategyResultLast->m_dProfit;			
		}
		else
		{
			pStrategyResultLast->m_dProfitAccmulated = pStrategyResultLast->m_dProfit;
		}
		if(pStrategyResultBefore)
		{
			pStrategyResultLast->m_dEnteredAmount = pStrategyResultBefore->m_dEnteredAmount;
			pStrategyResultLast->m_dProfitRateAccmulated 
				= pStrategyResultLast->m_dProfitAccmulated/pStrategyResultLast->m_dEnteredAmount*100;
		}
	}
	// 매수
	else if(pStrategyResultLast->m_DealType==CStrategyResult::BUY)
	{	
		if(pStrategyResultBefore)
		{
			pStrategyResultLast->m_dEnteredAmount 
				= pStrategyResultLast->m_dAmount + pStrategyResultBefore->m_dEnteredAmount;
		}
		else
		{
			pStrategyResultLast->m_dEnteredAmount = pStrategyResultLast->m_dAmount;		
		}		
	}
	// 매도
	else if(pStrategyResultLast->m_DealType==CStrategyResult::SELL)
	{	
		if(pStrategyResultBefore)
		{
			pStrategyResultLast->m_dEnteredAmount 
				= pStrategyResultLast->m_dAmount + pStrategyResultBefore->m_dEnteredAmount;
		}
		else
		{
			pStrategyResultLast->m_dEnteredAmount = pStrategyResultLast->m_dAmount;		
		}		
	}
}

double CStrategyWnd::ApplySlipAndTex(double dMulData, BOOL bType,double nNum)
{
	double dResultSlip;
	double dTex;
	if(m_lSlippageUnit)//0: % , 1=P
	{
		dResultSlip = dMulData + m_dSlippageForEntry*nNum;
	}
	else//%
	{
		dResultSlip = dMulData * (m_dSlippageForExit*0.01);
	}

	if(bType)//진입
	{
		if(m_lFeeUnit)
		{
			dTex = ((dMulData+ dResultSlip)+m_dFeeForEntry);
			return (dMulData + dResultSlip + dTex);
		}
		else
		{	dTex = ((dMulData + dResultSlip)*(m_dFeeForEntry*0.01));
			return (dMulData + dResultSlip + dTex);
		}
	}
	else//청산
	{
		if(m_lFeeUnit)
		{
			dTex = ((dMulData+ dResultSlip)+m_dFeeForExit);
			return (dMulData + dResultSlip - dTex);
		}
		else
		{
			dTex = ((dMulData+ dResultSlip)*(m_dFeeForExit*0.01));
			return (dMulData + dResultSlip - dTex);
		}
	}

	return 0;
}

//여기서 성과분석 데이터가 들어간다. :KHD
BOOL CStrategyWnd::CalculateStatisticalInfo(long lStartPos, long lEndPos)
{

	if(m_Status==STRATEGY_NONE||m_Status==STRATEGY_STANDBY) 
	{
		ASSERT(0);
		return FALSE;
	}
	CString strDate;
	//long  lResultDealType = 0;
	double dResultClose = 0;
	double dResultSlip =0;//슬리피지 : KHD
	int nSize = m_pDateItem->GetSize();
	CIDataItem *pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	RemoveAllArray();

	// vntsorl
	CStrategyResult *pStrategyResultLast = NULL;
	CStrategyResult *pStrategyResult = NULL;
	double dVolume = m_nOrderAmount;//수정 : 설정내용에 맞게..
	long lBuyVolOfHolding = 0;
	long lSellVolOfHolding = 0;
	if(lEndPos>=nSize)
	{
		lEndPos = nSize - 1;
	}
	else if(lStartPos==-1||lEndPos==-1)
	{
		lStartPos = 0;
		lEndPos = nSize - 1;
	}
	m_lAllCandleInCalculation = lEndPos - lStartPos + 1;
	long lLastDealIndex = 0;
	CDealData* pDealData = NULL;
	
	POSITION pos = m_dealDataItem.GetHeadPosition();
	int nDealPositionSize = m_arrayDealPosition.GetSize();
	double dDealPosition = 0;
	for(int nIndx = 0; nIndx < nDealPositionSize; ++nIndx)
	{
		if(dDealPosition = m_arrayDealPosition.GetAt(nIndx))
		{

			while(pos)
			{
				if((pDealData = m_dealDataItem.GetNext(pos)) && (pDealData->m_nIndex == nIndx))
				{
					if(pDealData->m_nIndex>=lStartPos&&pDealData->m_nIndex<=lEndPos)
					{
						if(pDealData->m_lDealValue!=0) //이 값을 찾아야 한다.(옵션세팅이래..)
						{

//////////////////////////////////////////////////////////////////////////
							strDate = m_pDateItem->GetAt(nIndx);		
							dResultClose = pClose->GetAt(nIndx);	// Close Price

							// 매수 / 매수청산
							if((long)dDealPosition & (VALENTRYBUY | VALEXITLONG | 
										FL_BUY_LOSSRATE | FL_BUY_TARGETBENEFIT | FL_BUY_HPRICECPBEAR |
										FL_BUY_TARGETVALUEPRESERVE | FL_BUY_BENEFITPRESERVE | 
										FL_BUY_MINIPRICE | FL_BUY_DAYLIQUIDATION))
							{
								if(lBuyVolOfHolding > 0)
								{
									pStrategyResult = CreateStrategyResultData();
									pStrategyResult->m_DealType = CStrategyResult::EXITLONG;	//매수청산
									pStrategyResult->m_dTime = atof(strDate);
									pStrategyResult->m_lCntCandleInAccess = nIndx - lLastDealIndex;
									pStrategyResult->m_dPrice = dResultClose;
									pStrategyResult->m_lQty = lBuyVolOfHolding;

									//슬리피지 구하기 
									pStrategyResult->m_dAmount = ApplySlipAndTex(dResultClose * dVolume, FALSE, dVolume);

									CalaulateStrategyResultData();
									lBuyVolOfHolding = 0;
								}				
								if((long)dDealPosition & VALENTRYBUY)
								{
									pStrategyResult = CreateStrategyResultData();
									pStrategyResult->m_DealType = CStrategyResult::BUY;//매수
									pStrategyResult->m_dTime = atof(strDate);					
									pStrategyResult->m_dPrice = dResultClose;
									pStrategyResult->m_lQty = (long)dVolume;

									//슬리피지 구하기 
									pStrategyResult->m_dAmount = ApplySlipAndTex(dResultClose * dVolume, TRUE, dVolume);

									CalaulateStrategyResultData();
									if(!lBuyVolOfHolding)
									{
										lLastDealIndex = nIndx;
									}
									lBuyVolOfHolding += (long)dVolume;
								}				
							}
							
							// 매도 / 매도청산
							if((long)dDealPosition & (VALENTRYSELL | VALEXITSHORT | 
										FL_SELL_LOSSRATE | FL_SELL_TARGETBENEFIT | FL_SELL_HPRICECPBEAR |
										FL_SELL_TARGETVALUEPRESERVE | FL_SELL_BENEFITPRESERVE | 
										FL_SELL_MINIPRICE | FL_SELL_DAYLIQUIDATION))
							{
								if(lSellVolOfHolding < 0)
								{
									pStrategyResult = CreateStrategyResultData();
									pStrategyResult->m_DealType = CStrategyResult::EXITSHORT;

									pStrategyResult->m_dTime = atof(strDate);
									pStrategyResult->m_lCntCandleInAccess = nIndx - lLastDealIndex;
									pStrategyResult->m_dPrice = dResultClose;
									pStrategyResult->m_lQty = lSellVolOfHolding * -1;
									pStrategyResult->m_dAmount = ApplySlipAndTex(dResultClose * dVolume, FALSE, dVolume);

									CalaulateStrategyResultData();
									lSellVolOfHolding = 0;

								}				
								if(m_strInputCode.GetLength() != QUOTEINDEXLENGTH && ((long)dDealPosition & VALENTRYSELL))
								{
									pStrategyResult = CreateStrategyResultData();
									pStrategyResult->m_DealType = CStrategyResult::SELL;
									
									pStrategyResult->m_dTime = atof(strDate);
									pStrategyResult->m_dPrice = dResultClose;
									pStrategyResult->m_lQty = (long)dVolume;
									pStrategyResult->m_dAmount = ApplySlipAndTex(dResultClose * dVolume, TRUE, dVolume);

									CalaulateStrategyResultData();
									if(!lSellVolOfHolding)
									{
										lLastDealIndex = (long)nIndx;
									}
									lSellVolOfHolding -= (long)dVolume;
								}
							}
//////////////////////////////////////////////////////////////////////////
						}
					}
					break;
				}
			}
		}
	}
	
//	CStrategyResult *pStrategyResultLast = NULL;
//	CStrategyResult *pStrategyResult = NULL;
//	double dVolume = m_nOrderAmount;//수정 : 설정내용에 맞게..
//	long lVolOfHolding = 0;
//	if(lEndPos>=nSize)
//	{
//		lEndPos = nSize - 1;
//	}
//	else if(lStartPos==-1||lEndPos==-1)
//	{
//		lStartPos = 0;
//		lEndPos = nSize - 1;
//	}
//	m_lAllCandleInCalculation = lEndPos - lStartPos + 1;
//	long lLastDealIndex = 0;
//	CDealData* pDealData = NULL;
//	
//	POSITION pos = m_dealDataItem.GetHeadPosition();
//	int nDealPositionSize = m_arrayDealPosition.GetSize();
//	double dDealPosition = 0;
//	for(int nIndx = 0; nIndx < nDealPositionSize; ++nIndx)
//	{
//		if(dDealPosition = m_arrayDealPosition.GetAt(nIndx))
//		{
//			while(pos)
//			{
//				if((pDealData = m_dealDataItem.GetNext(pos)) && (pDealData->m_nIndex == nIndx))
//				{
//					if(pDealData->m_nIndex>=lStartPos&&pDealData->m_nIndex<=lEndPos)
//					{
//						if(pDealData->m_lDealValue!=0) //이 값을 찾아야 한다.(옵션세팅이래..)
//						{			
//							strDate = m_pDateItem->GetAt(pDealData->m_nIndex);		
//							dResultClose = pClose->GetAt(pDealData->m_nIndex);	// Close Price
//							if(pDealData->m_lDealValue&VALBUY)	//매도청산
//							{
//								if(lVolOfHolding<0)	// ExitShort........m_dMesuRate:진입쪽
//								{
//									pStrategyResult = CreateStrategyResultData();
//									pStrategyResult->m_DealType = CStrategyResult::EXITSHORT;//매도청산
//
//
//									pStrategyResult->m_dTime = atof(strDate);
//									pStrategyResult->m_lCntCandleInAccess = pDealData->m_nIndex - lLastDealIndex;
//									pStrategyResult->m_dPrice = dResultClose;
//									pStrategyResult->m_lQty = lVolOfHolding*-1;
////									pStrategyResult->m_dAmount = dResultClose*lVolOfHolding*-1;// //KHD
//									//슬리피지 구하기 
//									pStrategyResult->m_dAmount = ApplySlipAndTex( dResultClose*dVolume ,FALSE,dVolume);
//
//									CalaulateStrategyResultData();
//									lVolOfHolding = 0;
//
//								}				
//								if((pDealData->m_lDealValue&VALENTRYBUY))	//매수Buy....매수진입m_dMesuRate
//								{
//									pStrategyResult = CreateStrategyResultData();
//									pStrategyResult->m_DealType = CStrategyResult::BUY;//매수
//									pStrategyResult->m_dTime = atof(strDate);					
//									pStrategyResult->m_dPrice = dResultClose;
//									pStrategyResult->m_lQty = (long)dVolume;
////									pStrategyResult->m_dAmount = dResultClose*dVolume;//KHD
//									//슬리피지 구하기 
//									pStrategyResult->m_dAmount = ApplySlipAndTex( dResultClose*dVolume ,TRUE,dVolume);
//
//									CalaulateStrategyResultData();
//									if(!lVolOfHolding)
//									{
//										lLastDealIndex = pDealData->m_nIndex;
//									}
//									lVolOfHolding+=(long)dVolume;
//								}				
//							}
//							else if(pDealData->m_lDealValue&VALSELL || IsForcedLiquidation(pDealData->m_lDealValue))	//매수청산	
//							{
//								if(lVolOfHolding>0)				// Exitlong....
//								{
//									pStrategyResult = CreateStrategyResultData();
//									pStrategyResult->m_DealType = CStrategyResult::EXITLONG;//매수청산
//
//									pStrategyResult->m_dTime = atof(strDate);
//									pStrategyResult->m_lCntCandleInAccess = pDealData->m_nIndex - lLastDealIndex;
//									pStrategyResult->m_dPrice = dResultClose;
//									pStrategyResult->m_lQty = lVolOfHolding;
////									pStrategyResult->m_dAmount = dResultClose*lVolOfHolding;//KHD
//									pStrategyResult->m_dAmount = ApplySlipAndTex( dResultClose*dVolume ,FALSE,dVolume);
//
//									CalaulateStrategyResultData();
//									lVolOfHolding = 0;
//
//								}				
//								if(m_strInputCode.GetLength()!=QUOTEINDEXLENGTH&&(pDealData->m_lDealValue&VALENTRYSELL))	// 매도Sell....
//								{
//									pStrategyResult = CreateStrategyResultData();
//									pStrategyResult->m_DealType = CStrategyResult::SELL;//매도
//									pStrategyResult->m_dTime = atof(strDate);
//									pStrategyResult->m_dPrice = dResultClose;
//									pStrategyResult->m_lQty = (long)dVolume;
////									pStrategyResult->m_dAmount = dResultClose*dVolume;//KHD
//									pStrategyResult->m_dAmount = ApplySlipAndTex( dResultClose*dVolume ,TRUE,dVolume);
//
//									CalaulateStrategyResultData();
//									if(!lVolOfHolding)
//									{
//										lLastDealIndex = (long)pDealData->m_nIndex;
//									}
//									lVolOfHolding-=(long)dVolume;
//								}
//							}
//						}
//					}
//					break;
//				}
//			}//while
//		}
//	}

	if(lBuyVolOfHolding > 0)
	{
		pStrategyResult = CreateStrategyResultData();		
		strDate = m_pDateItem->GetAt(lEndPos);	
		dResultClose = pClose->GetAt(lEndPos);
		pStrategyResult->m_DealType = CStrategyResult::EXITLONG;

		pStrategyResult->m_dTime = atof(strDate);
		pStrategyResult->m_dPrice = dResultClose;
		pStrategyResult->m_lQty = lBuyVolOfHolding;
		pStrategyResult->m_dAmount = ApplySlipAndTex(dResultClose * dVolume, FALSE, dVolume);

		pStrategyResult->m_bOnlyValuate = TRUE;

		CalaulateStrategyResultData();
		lBuyVolOfHolding = 0;
	}

	if(lSellVolOfHolding < 0)
	{
		pStrategyResult = CreateStrategyResultData();
		strDate = m_pDateItem->GetAt(lEndPos);	
		dResultClose = pClose->GetAt(lEndPos);
		pStrategyResult->m_DealType = CStrategyResult::EXITSHORT;
		pStrategyResult->m_dTime = atof(strDate);
		pStrategyResult->m_dPrice = dResultClose;
		pStrategyResult->m_lQty = lSellVolOfHolding * -1;
		pStrategyResult->m_dAmount = ApplySlipAndTex(dResultClose * dVolume, FALSE, dVolume);

		pStrategyResult->m_bOnlyValuate = TRUE;

		CalaulateStrategyResultData();
		lSellVolOfHolding = 0;
	}

//	if(lVolOfHolding>0 )//매수청산
//	{
//		pStrategyResult = CreateStrategyResultData();		
//		strDate = m_pDateItem->GetAt(lEndPos);//nSize-1);		
//		dResultClose = pClose->GetAt(lEndPos);//nSize-1);	// Last Close Price
//		pStrategyResult->m_DealType = CStrategyResult::EXITLONG;//매수청산
//
//		pStrategyResult->m_dTime = atof(strDate);
//		pStrategyResult->m_dPrice = dResultClose;
//		pStrategyResult->m_lQty = lVolOfHolding;
////		pStrategyResult->m_dAmount = dResultClose*lVolOfHolding;//KHD
//		pStrategyResult->m_dAmount = ApplySlipAndTex( dResultClose*dVolume ,FALSE,dVolume);//추가
//		pStrategyResult->m_bOnlyValuate = TRUE;
//
//		CalaulateStrategyResultData();
//		lVolOfHolding = 0;
//	}
//
//	else if(lVolOfHolding<0)
//	{
//		pStrategyResult = CreateStrategyResultData();
//		strDate = m_pDateItem->GetAt(lEndPos);//nSize-1);		
//		dResultClose = pClose->GetAt(lEndPos);//nSize-1);	// Last Close Price
//		pStrategyResult->m_DealType = CStrategyResult::EXITSHORT;//매도 청산
//		pStrategyResult->m_dTime = atof(strDate);
//		pStrategyResult->m_dPrice = dResultClose;
//		pStrategyResult->m_lQty = lVolOfHolding*-1;
////		pStrategyResult->m_dAmount = dResultClose*lVolOfHolding*-1;//KHD
//		pStrategyResult->m_dAmount = ApplySlipAndTex( dResultClose*dVolume ,FALSE,dVolume);//추가
//		pStrategyResult->m_bOnlyValuate = TRUE;
//
//		CalaulateStrategyResultData();
//		lVolOfHolding = 0;
//
//	}
	
	m_dSumProfit = 0; 
	m_dSumLoss = 0;
	m_dMaxProfit = 0;
	m_dMaxLoss = 0;
	m_lCntProfit = 0;
	m_lCntLoss = 0;
	m_dSumAmountExit = 0;
	m_lCntProfitStraight = 0;
	m_lCntLossStraight = 0;
	m_lSumProfitStraight = 0;
	m_lSumLossStraight = 0;
	m_lSumCandleProfit = 0;
	m_lSumCandleLoss = 0;

	double dSumStraight = 0;
	long lCntStraight = 0;
	long lTypeStraight = 0;
	
	long lSize = m_arrayStrategyResult.GetSize();
	CStrategyResult *pStrategyResultInArray = NULL;
	for(long lPos=0;lPos<lSize;lPos++)
	{
		pStrategyResultInArray = m_arrayStrategyResult.GetAt(lPos);
		if(pStrategyResultInArray->m_dProfit>0)
		{
			m_dSumProfit += pStrategyResultInArray->m_dProfit;
			m_lSumCandleProfit += pStrategyResultInArray->m_lCntCandleInAccess;
			if(m_dMaxProfit<pStrategyResultInArray->m_dProfit)
			{
				m_dMaxProfit = pStrategyResultInArray->m_dProfit;
			}
			m_lCntProfit++;
			if(lTypeStraight==1)	// Keeping straight...
			{
				dSumStraight += pStrategyResultInArray->m_dProfit;
				lCntStraight++;
			}
			else
			{	
				lTypeStraight = 1;
				lCntStraight = 1;
				dSumStraight = pStrategyResultInArray->m_dProfit;
			}
			if(lTypeStraight==1&&m_lCntProfitStraight<lCntStraight)	// Change..
			{
				m_lCntProfitStraight = lCntStraight;
				m_lSumProfitStraight = (long)dSumStraight;
			}
		}
		else if(pStrategyResultInArray->m_dProfit<0)
		{
			m_dSumLoss += pStrategyResultInArray->m_dProfit;
			m_lSumCandleLoss += pStrategyResultInArray->m_lCntCandleInAccess;
			if(m_dMaxLoss>pStrategyResultInArray->m_dProfit)
			{
				m_dMaxLoss = pStrategyResultInArray->m_dProfit;
			}
			m_lCntLoss++;			
			if(lTypeStraight==2)	// Keeping straight...
			{
				dSumStraight += pStrategyResultInArray->m_dProfit;
				lCntStraight++;
			}
			else
			{
				lTypeStraight = 2;
				lCntStraight = 1;
				dSumStraight = pStrategyResultInArray->m_dProfit;
			}
			if(lTypeStraight==2&&m_lCntLossStraight<lCntStraight)	// Change..
			{
				m_lSumLossStraight = (long)dSumStraight;
				m_lCntLossStraight = lCntStraight;
			}
		}
		if(pStrategyResultInArray->m_DealType == CStrategyResult::EXITLONG
			||pStrategyResultInArray->m_DealType == CStrategyResult::EXITSHORT)
		{
			m_dSumAmountExit += pStrategyResultInArray->m_dAmount;
		}		
	}
	return TRUE;
}

//KHD : 강제청산 여부 확인 
BOOL CStrategyWnd::IsForcedLiquidation(long lDealValue)
{
	switch(lDealValue)
	{
	case VALSONJUL:		
	case VALSOOIK:		
	case VALGOGA:		
	case VALJUGA:		
	case VALMIN:			
	case VALDANGIL:
	case VALTARGETPRESERV:
	case VALBENEFITPRESERV:
		return TRUE;
	}
	return FALSE;
}

long CStrategyWnd::GetCntStrategyResult()
{
	return m_arrayStrategyResult.GetSize();
}

CStrategyResult* CStrategyWnd::GetStrategyResult(long lIndex)
{
	return m_arrayStrategyResult.GetAt(lIndex);
}

LPCTSTR CStrategyWnd::GetErrorMessage()
{
	return m_strError;
}


double CStrategyWnd::GetStatisticalInfo(long lPos)
{

	CStrategyResult *pStrategyResult = NULL;
	long lSize = m_arrayStrategyResult.GetSize();
	if(!lSize) return 0;
	switch(lPos)
	{
	case PROFITRATEACCMULATED://누적수익률
		{
			for(long lIndex=lSize-1;lIndex>=0;lIndex--)
			{
				pStrategyResult = m_arrayStrategyResult.GetAt(lIndex);
				if(pStrategyResult->m_dProfitRateAccmulated!=0)
				{
					return pStrategyResult->m_dProfitRateAccmulated;
				}
			}
			return 0.;			
		}
		break;
	case PROFITRATEAVG://건별 평균 수익률 
		{	
			double Accumulate = 0;
			for(long lIndex =0;lIndex<lSize;lIndex++)
			{
				Accumulate+= m_arrayStrategyResult.GetAt(lIndex)->m_dProfitRate;
			}
			return Accumulate/(m_lCntProfit + m_lCntLoss);//누적수익률 연산 수정:KHD
		}
		break;
	case PROFITRATECURRENT:
		{
			pStrategyResult = m_arrayStrategyResult.GetAt(lSize-1);
			return pStrategyResult->m_dProfitRate;//pStrategyResult->m_dProfitRateAccmulated; : 수정KHD: 건별수익률마지막값으로 보냄
		}
		break;
	case PROFITLOSSALL:
		{
			return (m_dSumProfit+m_dSumLoss);
		}
		break;
	case PROFITALL:
		{
			return m_dSumProfit;
		}
		break;
	case LOSSALL:
		{
			return m_dSumLoss;
		}
		break;
	case PROFITLOSSRATEALL:
		{
			if(m_dSumLoss==0)
			{
				return (m_dSumProfit)*-1;
			}
			return (m_dSumProfit/m_dSumLoss)*-1;
		}
		break;
	case MAXPROFIT:
		{
			return m_dMaxProfit;
		}
		break;
	case MAXLOSS:
		{
			return m_dMaxLoss;
		}
		break;
	case PROFITLOSSAVG:
		{
			if((m_lCntProfit+m_lCntLoss)==0)
			{
				return (m_dSumProfit+m_dSumLoss);
			}
			return (m_dSumProfit+m_dSumLoss)/(m_lCntProfit+m_lCntLoss);
		}
		break;
	case PROFITAVG:
		{
			if(m_lCntProfit==0)
			{
				return m_dSumProfit;
			}
			return (m_dSumProfit)/(m_lCntProfit);
		}
		break;
	case LOSSAVG:
		{
			if(m_lCntLoss==0)
			{	
				return m_dSumLoss;
			}
			return (m_dSumLoss)/(m_lCntLoss);
		}
		break;
	case AMOUNTENTEREDALL:
		{
			pStrategyResult = m_arrayStrategyResult.GetAt(lSize-1);
			return pStrategyResult->m_dEnteredAmount;
		}
		break;
	case AMOUNTEXITALL:
		{
			return m_dSumAmountExit;
		}
		break;
	case CNTALL:
		{
			return (m_lCntProfit + m_lCntLoss);
		}
		break;
	case CNTPROFIT:
		{
			return m_lCntProfit;
		}
		break;
	case CNTLOSS:
		{
			return m_lCntLoss;
		}
		break;
	case RATEPROFITLOSS:
		{
			if(m_lCntLoss==0)
			{
				return m_lCntProfit;
			}
			return (double)(m_lCntProfit/(double)m_lCntLoss);
		}
		break;
	case RATEPROFIT:
		{
			if((m_lCntProfit + m_lCntLoss)==0)
			{
				return m_lCntProfit;
			}
			return (double)(m_lCntProfit*100)/(double)(m_lCntProfit + m_lCntLoss);
		}
		break;
	case RATELOSS:
		{
			if((m_lCntProfit + m_lCntLoss)==0)
			{
				return m_lCntLoss;
			}
			return (double)(m_lCntLoss*100)/(double)(m_lCntProfit + m_lCntLoss);
		}
		break;
	case CHANGERATE:
		{
			CStrategyResult *pStrategyResultFirst = NULL;
			pStrategyResult = m_arrayStrategyResult.GetAt(lSize-1);
			pStrategyResultFirst = m_arrayStrategyResult.GetAt(0);
			if(pStrategyResultFirst->m_dPrice==0)//수정: KHD : (첫 가격 - 마지막 가격 / 첫 가격)기간등락률
			{
				return ((pStrategyResult->m_dPrice - pStrategyResultFirst->m_dPrice)/1)*100;
			}
			return ((pStrategyResult->m_dPrice - pStrategyResultFirst->m_dPrice)/pStrategyResultFirst->m_dPrice)*100;
		}
		break;
	case MAXPROFITSTRAIGHT:
		{
			return m_lCntProfitStraight;
		}
		break;
	case MAXLOSSSTRAIGHT:
		{
			return m_lCntLossStraight;
		}
		break;
	case RATEPROFITEXCEED:///전략 초과 수익률 : 누적수익률(0) - 기간등락률(21)
		{
			CStrategyResult *pStrategyResultFirst = NULL;
			pStrategyResult = m_arrayStrategyResult.GetAt(lSize-1);
			pStrategyResultFirst = m_arrayStrategyResult.GetAt(0);
			if(pStrategyResultFirst->m_dPrice==0)
			{
				return (pStrategyResult->m_dPrice-pStrategyResultFirst->m_dPrice);
			}
			return (pStrategyResult->m_dPrice-pStrategyResultFirst->m_dPrice)/pStrategyResultFirst->m_dPrice;
		}
		break;
	case PROFITSTRAIGHT:
		{
			return m_lSumProfitStraight;
		}
		break;
	case LOSSSTRAIGHT:
		{
			return m_lSumLossStraight;
		}
		break;
	case CNTCANDLE:
		{
			return m_lAllCandleInCalculation;
		}
		break;
	case CNTCANDLEPROFIT:
		{
			return m_lSumCandleProfit;	
		}
		break;
	case CNTCANDLELOSS:
		{
			return m_lSumCandleLoss;
		}
		break;
	case RATEACCESS:
		{
			return (double)(m_lSumCandleProfit+m_lSumCandleLoss)*100/(double)m_lAllCandleInCalculation;
		}
		break;
	case CNTCANDLEACCESS:
		{
			return (m_lSumCandleProfit+m_lSumCandleLoss);
		}
		break;
	case CNTCANDLEEXIT:
		{
			return (m_lAllCandleInCalculation-(m_lSumCandleProfit+m_lSumCandleLoss));
		}
		break;
	case CNTCANDLEAVGDEAL:
		{
			if((m_lCntProfit+m_lCntLoss)==0)
			{
				return (m_lSumCandleProfit+m_lSumCandleLoss);
			}
			double value =(m_lSumCandleProfit+m_lSumCandleLoss)/(double)(m_lCntProfit+m_lCntLoss);
			return value;
		}
		break;
	case CNTCANDLEAVGPROFIT:
		{
			if(m_lCntProfit==0)
			{
				return m_lSumCandleProfit;
			}
			double value = m_lSumCandleProfit/(double)m_lCntProfit; 
			return value;
		}
		break;
	case CNTCANDLEAVGLOSS:
		{
			if(m_lCntLoss==0)
			{
				return m_lSumCandleLoss;
			}
			double value = m_lSumCandleLoss/(double)m_lCntLoss;
			return value;
		}
		break;

	}
	return 0;
}


void CStrategyWnd::GetCurrentPositonNProfit(long& rLCurrentDealPosition, double &rDCurrentProfitRate)
{
	rLCurrentDealPosition = m_lCurrentDealPosition;
	rDCurrentProfitRate = m_dCurrentProfitRate;
}

CString CStrategyWnd::GetProfileInfo(LPCTSTR lpAppName,LPCTSTR lpKeyName)
{
	CString strProfileInfo;
	if(m_strategyType==ST_ST)
	{
		CString strProfilePath = m_strStrategyPath;
		char szBuffer[256];
		::GetPrivateProfileString(lpAppName,lpKeyName,"",szBuffer,sizeof(szBuffer),strProfilePath);
		strProfileInfo = szBuffer;
	}
	return strProfileInfo;
}

BOOL CStrategyWnd::SetUpperLowerPrice(double dUpperPrice,double dLowerPrice)
{
	m_dUpperPrice = dUpperPrice;
	m_dLowerPrice = dLowerPrice;
	return TRUE;
}

BOOL CStrategyWnd::SetVolumeForCalculatation(double dVol)
{
	m_dVolForOrder = dVol;
	return TRUE;
}

double CStrategyWnd::GetVolumeInDealData()
{
	POSITION pos = m_dealDataItem.GetTailPosition();
	if(pos)
	{
		CDealData* pDealData = m_dealDataItem.GetPrev(pos);
		if(pDealData->m_lDealValue&&VALBUY||pDealData->m_lDealValue&&VALSELL)
		{
			return pDealData->m_dContract;
		}
		else// ExitLong, ExitShort
		{
			if(pDealData->m_lType&DEALEXITDEFINED)
			{
				return pDealData->m_dContract;
			}
			else
			{
				double dSumOfContract = 0.;
				long lValueToKeep = 0;
				while(pos)
				{
					CDealData* pDealData = m_dealDataItem.GetPrev(pos);
					if(!lValueToKeep)
					{
						if(pDealData->m_lDealValue&&VALBUY)
						{
							lValueToKeep = VALBUY;
							dSumOfContract = pDealData->m_dContract;
						}
						else if(pDealData->m_lDealValue&&VALSELL)
						{
							lValueToKeep = VALSELL;
							dSumOfContract = pDealData->m_dContract;
						}
						else
						{
							break;
						}
					}
					else
					{
						if(pDealData->m_lDealValue&&VALSELL&&lValueToKeep==VALSELL)
						{
							dSumOfContract += pDealData->m_dContract;
						}
						else if(pDealData->m_lDealValue&&VALBUY&&lValueToKeep==VALBUY)
						{
							dSumOfContract += pDealData->m_dContract;
						}
						else
						{
							break;
						}
					}
				}
				return dSumOfContract;
			}			
		}

	}
	return 0;
}

void CStrategyWnd::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent==m_nTimerID)
	{
		ExitAtTodayByTimeSelected(FALSE);
	}
	CWnd::OnTimer(nIDEvent);
}


void CStrategyWnd::ExitAtTodayByTimeSelected(BOOL bStartTimer/*=TRUE*/)
{	
	if(m_bUseDayLiquidation)
	{
		CTime t = CTime::GetCurrentTime();
		long lCurrentTime = atol(t.Format("%H%M"));
		if(lCurrentTime >= atol(m_szTimeDayLiquidation))
		{
			OnSTSetRealDataST();
			GetParent()->SendMessage(RMSG_STDATAMANAGER_RESULTDATA, m_lKey, (LPARAM)&m_DrawInfo);
			if(m_nTimerID) KillTimer(m_nTimerID);
		}		
		if(bStartTimer)
			m_nTimerID = SetTimer(ID_FORCEDEXITBYTIME, 60000, NULL);
	}
	else
	{
		if(m_nTimerID) KillTimer(m_nTimerID);
	}
}

void CStrategyWnd::InitConfiguration()
{
	m_dFeeForEntry = 0.; 
	m_dFeeForExit = 0.;
	m_lFeeUnit = 0;
	m_dSlippageForEntry = 0.;
	m_dSlippageForExit = 0.;
	m_lSlippageUnit = 0;

	if(m_pFeeCal)
	{
		delete m_pFeeCal;
		m_pFeeCal = NULL;
	}
	if(m_pSlippageCal)
	{
		delete m_pSlippageCal;
		m_pSlippageCal = NULL;
	}
}

HWND CStrategyWnd::GetSafeHwnd( ) const
{
	return CWnd::GetSafeHwnd();
}

//시간최적화에서 봉의 갯수를 가져오고 Set해야 한다.

void CStrategyWnd::ReadConfiguration(CString szPath)
{
//-------- 신호 설정내용 -------- 
	ReadConfig_Signal(szPath);
//-------- 강제청산 설정내용 --------
	ReadConfig_ForcedLiquidation(szPath);
//-------- 운용 설정내용 --------
	ReadConfig_Setup(szPath);
//-------- 주문 설정내용 --------
	ReadConfig_Order(szPath);
}

void CStrategyWnd::ReadConfig_Signal(CString szPath)
{
	char		szBuffer[256];
	CString		strComment;
	long		lShapeSignal;
	COLORREF	clrSignal;

	// Buy Shape
	if(!::GetPrivateProfileString(STCONFIG_SIGNAL, SHAPEBUY, "", szBuffer, sizeof(szBuffer), szPath))
		lShapeSignal = 0;
	else
		lShapeSignal = atol(szBuffer);
	strComment.Format("%d", lShapeSignal);
	m_dealDataItem.SetComment(SHAPEBUY, strComment);

	// Buy Color
	if(!::GetPrivateProfileString(STCONFIG_SIGNAL, COLORBUY, "", szBuffer, sizeof(szBuffer), szPath))
		clrSignal = SIGNALBUYDEFAULT;
	else
		clrSignal = atol(szBuffer);
	strComment.Format("%d", clrSignal);
	m_dealDataItem.SetComment(COLORBUY, strComment);

	// Sell Shape
	if(!::GetPrivateProfileString(STCONFIG_SIGNAL, SHAPESELL, "", szBuffer, sizeof(szBuffer), szPath))
		lShapeSignal = 0;
	else
		lShapeSignal = atol(szBuffer);
	strComment.Format("%d", lShapeSignal);
	m_dealDataItem.SetComment(SHAPESELL, strComment);

	// Sell Color
	if(!::GetPrivateProfileString(STCONFIG_SIGNAL, COLORSELL, "", szBuffer, sizeof(szBuffer), szPath))
		clrSignal = SIGNALSELLDEFAULT;
	else
		clrSignal = atol(szBuffer);
	strComment.Format("%d", clrSignal);
	m_dealDataItem.SetComment(COLORSELL, strComment);

	// ExitShort Shape
	if(!::GetPrivateProfileString(STCONFIG_SIGNAL, SHAPEEXITSHORT, "", szBuffer, sizeof(szBuffer), szPath))
		lShapeSignal = 0;
	else
		lShapeSignal = atol(szBuffer);
	strComment.Format("%d", lShapeSignal);
	m_dealDataItem.SetComment(SHAPEEXITSHORT, strComment);

	// ExitShort Color
	if(!::GetPrivateProfileString(STCONFIG_SIGNAL, COLOREXITSHORT, "", szBuffer, sizeof(szBuffer), szPath))
		clrSignal = SIGNALEXITSHORTDEFAULT;
	else
		clrSignal = atol(szBuffer);
	strComment.Format("%d", clrSignal);
	m_dealDataItem.SetComment(COLOREXITSHORT, strComment);

	// ExitLong Shape
	if(!::GetPrivateProfileString(STCONFIG_SIGNAL, SHAPEEXITLONG, "", szBuffer, sizeof(szBuffer), szPath))
		lShapeSignal = 0;
	else
		lShapeSignal = atol(szBuffer);
	strComment.Format("%d", lShapeSignal);
	m_dealDataItem.SetComment(SHAPEEXITLONG, strComment);

	// ExitLong Color
	if(!::GetPrivateProfileString(STCONFIG_SIGNAL, COLOREXITLONG, "", szBuffer, sizeof(szBuffer), szPath))
		clrSignal = SIGNALEXITLONGDEFAULT;
	else
		clrSignal = atol(szBuffer);
	strComment.Format("%d", clrSignal);
	m_dealDataItem.SetComment(COLOREXITLONG, strComment);

	// Signal Name
	::GetPrivateProfileString(STCONFIG_SIGNAL, SHOWSIGNALNAME, "0", szBuffer, sizeof(szBuffer), szPath);
	strComment.Format("%d,", atol(szBuffer));
	m_dealDataItem.SetComment(SHOWSIGNALNAME, strComment);

	// Signal Qty
	::GetPrivateProfileString(STCONFIG_SIGNAL, SHOWSIGNALQTY, "0", szBuffer, sizeof(szBuffer), szPath);
	strComment.Format("%d,", atol(szBuffer));
	m_dealDataItem.SetComment(SHOWSIGNALQTY, strComment);
}

void CStrategyWnd::ReadConfig_ForcedLiquidation(CString szPath)
{
	CString szTmp;
	COLORREF clrSignal = RGB(0,0,0);
	szTmp.GetBufferSetLength(MAX_PATH);

	// 강제청산시점 (0 : 조건 만족시 / 1 : 봉완성시)
	m_nTiming_ForcedLiquidation = ::GetPrivateProfileInt(STCONFIG_CLEAR, CLEARPOINT, 0, szPath);
	
	// 손절매(손실률)
	GetPrivateProfileString(STCONFIG_CLEAR, USELOSSRATE, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_bUseLossRate = atoi(szTmp);

	// 손절매옵션 사용할 경우
	if(IsLossRate())
	{
		GetPrivateProfileString(STCONFIG_CLEAR, LOSSRATE, "", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);

		// % or 원
		m_uLossRate_PerOrWon = GetPrivateProfileInt(STCONFIG_CLEAR, LOSSRATEUNIT, 0, szPath);
		if(m_uLossRate_PerOrWon == 0)
			m_dLossRate = atof(szTmp) * 0.01;	// %
		else
//			m_dLossRate = atoi(szTmp);			// 원
			m_dLossRate = atof(szTmp);			// 원,Point
	}

	// 목표수익	
	GetPrivateProfileString(STCONFIG_CLEAR, USETARGETBENEFIT, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_bUseTargetBenefit = atoi(szTmp);

	// 목표수익옵션 사용할 경우
	if(IsTargetBenefit())
	{
		GetPrivateProfileString(STCONFIG_CLEAR, TARGETBENEFIT, "", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);

		// % or 원
		m_uTargetBenefit_PerOrWon = GetPrivateProfileInt(STCONFIG_CLEAR, TARGETBENEFITUNIT, 0, szPath);
		if(m_uTargetBenefit_PerOrWon == 0)
			m_dTargetBenefit = atof(szTmp) * 0.01;	// %
		else
//			m_dTargetBenefit = atoi(szTmp);			// 원
			m_dTargetBenefit = atof(szTmp);			// 원,Point
	}

	// 최고가대비 하락
	GetPrivateProfileString(STCONFIG_CLEAR, USEHIGHPRICE, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_bUseHPriceCPBear = atoi(szTmp);

	// 최고가대비 하락옵션 사용할 경우
	if(IsHPriceCPBear())
	{
		GetPrivateProfileString(STCONFIG_CLEAR, HIGHPRICE, "", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);

		// % or 원
		m_uHPriceCPBear_PerOrWon = GetPrivateProfileInt(STCONFIG_CLEAR, HIGHPRICEUNIT, 0, szPath);
		if(m_uHPriceCPBear_PerOrWon == 0)
			m_dHPriceCPBear = atof(szTmp) * 0.01;		// %
		else
//			m_dHPriceCPBear = atoi(szTmp);				// 원
			m_dHPriceCPBear = atof(szTmp);				// 원,Point
	}

//	// 당일 저가대비 상승
//	GetPrivateProfileString(STCONFIG_CLEAR, USELOWPRICE, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
//	m_bUseLPriceCPBull = atoi(szTmp);
//
//	// 당일 저가대비 상승옵션 사용할 경우
//	if(IsLPriceCPBull())
//	{
//		GetPrivateProfileString(STCONFIG_CLEAR, LOWPRICE, "", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
//
//		// % or 원
//		m_uLPriceCPBull_PerOrWon = GetPrivateProfileInt(STCONFIG_CLEAR, LOWPRICEUNIT, 0, szPath);
//		if(m_uLPriceCPBull_PerOrWon == 0)
//			m_dLPriceCPBull = atof(szTmp) * 0.01;		// %
//		else
//			m_dLPriceCPBull = atoi(szTmp);				// 원
//	}

	// 목표가보존
	GetPrivateProfileString(STCONFIG_CLEAR, USETARGETVALUEPRESERV, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_bUseTargetValuePreserve = atoi(szTmp);

	// 목표가보존옵션 사용할 경우
	if(IsTargetValuePreserve())
	{
		GetPrivateProfileString(STCONFIG_CLEAR, TARGETVALUEPRESERV, "", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);

		// % or 원
		m_uTargetValuePreserve_PerOrWon = GetPrivateProfileInt(STCONFIG_CLEAR, TARGETVALUEPRESERVUNIT, 0, szPath);
		if(m_uTargetValuePreserve_PerOrWon == 0)
			m_dTargetValuePreserve = atof(szTmp) * 0.01;		// %
		else
//			m_dTargetValuePreserve = atoi(szTmp);				// 원
			m_dTargetValuePreserve = atof(szTmp);				// 원,Point
	}

	// 이익보존
	GetPrivateProfileString(STCONFIG_CLEAR, USEBENEFITPRESERV, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_bUseBenefitPreserve = atoi(szTmp);

	// 이익보존옵션 사용할 경우
	if(IsBenefitPreserve())
	{
		GetPrivateProfileString(STCONFIG_CLEAR, BENEFITPRESERVUP, "", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);

		// 이익보존(상승율)% or 원
		m_uBenefitPreserveUp_PerOrWon = GetPrivateProfileInt(STCONFIG_CLEAR, BENEFITPRESERVUPUNIT, 0, szPath);
		if(m_uBenefitPreserveUp_PerOrWon == 0)
			m_dBenefitPreserveUp = atof(szTmp) * 0.01;		// %
		else
//			m_dBenefitPreserveUp = atoi(szTmp);				// 원
			m_dBenefitPreserveUp = atof(szTmp);				// 원,Point

		GetPrivateProfileString(STCONFIG_CLEAR, BENEFITPRESERVDN, "", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);

		// 이익보존(하락율)% or 원
		m_uBenefitPreserveDn_PerOrWon = GetPrivateProfileInt(STCONFIG_CLEAR, BENEFITPRESERVDNUNIT, 0, szPath);
		if(m_uBenefitPreserveDn_PerOrWon == 0)
			m_dBenefitPreserveDn = atof(szTmp) * 0.01;		// %
		else
//			m_dBenefitPreserveDn = atoi(szTmp);				// 원
			m_dBenefitPreserveDn = atof(szTmp);				// 원,Point
	}

	// 최소가격변화
	GetPrivateProfileString(STCONFIG_CLEAR, USEMINIPRICE, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_bUseMiniPrice = atoi(szTmp);

	// 최소가격변화옵션 사용할 경우
	if(IsMiniPrice())
	{
		GetPrivateProfileString(STCONFIG_CLEAR, MINIPRICE, "", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);

		// % or 원
		m_uMiniPrice_PerOrWon = GetPrivateProfileInt(STCONFIG_CLEAR, MINIPRICEUNIT, 0, szPath);
		if(m_uMiniPrice_PerOrWon == 0)
			m_dMiniPrice = atof(szTmp) * 0.01;		// %
		else
//			m_dMiniPrice = atoi(szTmp);				// 원
			m_dMiniPrice = atof(szTmp);				// 원,Point

		// 봉
		GetPrivateProfileString(STCONFIG_CLEAR, MINIPRICEBONGCOUNT, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
		m_nBongCount = atoi(szTmp);
	}

	// 당일청산
	GetPrivateProfileString(STCONFIG_CLEAR, USETODAYCLEAR, "", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_bUseDayLiquidation = atoi(szTmp);

	// 당일청산옵션 사용할 경우
	if(IsDayLiquidation())
	{
		// 0 : 오전 / 1 : 오후
		m_uDayLiquidation_PerOrWon = GetPrivateProfileInt(STCONFIG_CLEAR, TODAYCLEARUINT, 0, szPath);

		// 시간
		GetPrivateProfileString(STCONFIG_CLEAR, TODAYCLEARTIME, "", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
		m_szTimeDayLiquidation = szTmp;
		m_szTimeDayLiquidation.Remove(':');

		// 오후일 경우
		if(m_uDayLiquidation_PerOrWon > 0)
			m_szTimeDayLiquidation.Format("%ld", atol(m_szTimeDayLiquidation) + 1200);
	}

	szTmp.ReleaseBuffer();
}

void CStrategyWnd::ReadConfig_Setup(CString szPath)
{
	CString szTmp;
	szTmp.GetBufferSetLength(MAX_PATH);

	// 중복진입 여부(0 : 중복진입 허용, 1 : 중복진입 허용안함)
	m_nUseRepeatEntry = GetPrivateProfileInt(STCONFIG_SETUP, USEREPEATBUY, 1, szPath) ? FALSE : TRUE;

	// 중복진입 허용횟수
	m_nRepeatEntryCount = GetPrivateProfileInt(STCONFIG_SETUP, REPEATBUYCOUNT, 0, szPath);
	if(m_nRepeatEntryCount < 2)
		m_nRepeatEntryCount = 1;

	// 거래비용 읽음
	::GetPrivateProfileString(STCONFIG_SETUP, TAXBUY, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_dFeeForEntry = atof(szTmp);		// 수수료 + 세금 (진입)

	::GetPrivateProfileString(STCONFIG_SETUP, TAXSELL, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_dFeeForExit = atof(szTmp);		// 수수료 + 세금 (청산)

	::GetPrivateProfileString(STCONFIG_SETUP, TAXUNIT, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_lFeeUnit = atol(szTmp);			// 수수료 + 세금 (단위)

	::GetPrivateProfileString(STCONFIG_SETUP, SLIPIGYBUY, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_dSlippageForEntry = atof(szTmp);	// 슬리피지 (진입)

	::GetPrivateProfileString(STCONFIG_SETUP, SLIPIGYSELL, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_dSlippageForExit = atof(szTmp);	// 슬리피지 (청산)

	::GetPrivateProfileString(STCONFIG_SETUP, SLIPIGYUNIT, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	m_lSlippageUnit = atol(szTmp);		// 슬리피지 (단위)

	szTmp.ReleaseBuffer();

	// 수수료 + 세금
	if(m_pFeeCal)
	{
		delete m_pFeeCal;
		m_pFeeCal = NULL;
	}
	if(m_lFeeUnit == 0)				// Percent
		m_pFeeCal = new CFeeCalByPercent(m_dFeeForEntry, m_dFeeForExit);
	else if(m_lFeeUnit == 1)		// Pt(원)
		m_pFeeCal = new CFeeCalByPrice(m_dFeeForEntry, m_dFeeForExit);

	// 슬리피지
	if(m_pSlippageCal)
	{
		delete m_pSlippageCal;
		m_pSlippageCal = NULL;
	}
	if(m_lSlippageUnit == 0)		// Percent
		m_pSlippageCal = new CFeeCalByPercent(m_dSlippageForEntry, m_dSlippageForExit);
	else if(m_lSlippageUnit == 1)	// Pt(원)
		m_pSlippageCal = new CFeeCalByPrice(m_dSlippageForEntry, m_dSlippageForExit);
}

void CStrategyWnd::ReadConfig_Order(CString szPath)
{
	CString szTmp;
	szTmp.GetBufferSetLength(MAX_PATH);

	// 주문수량
	m_nOrderAmount = ::GetPrivateProfileInt(STCONFIG_ORDER, ORDERAMOUNT, 1, szPath);
	if(m_nOrderAmount <= 0)
		m_nOrderAmount = 1;

	// 주문시점 (0 : 조건 만족시 / 1 : 봉완성시)
	m_nTiming_Order = ::GetPrivateProfileInt(STCONFIG_ORDER, ORDERTIME, 0, szPath);

	// 주문시작신호 (0 : 진입신호 / 1 : 모든신호)
	m_nOrderStartSignal = ::GetPrivateProfileInt(STCONFIG_ORDER, ORDERSTARTSIGNAL, 0, szPath);

	szTmp.ReleaseBuffer();
}

void CStrategyWnd::InitStrategySignalInfo()
{
	int nTotalDataCnt = m_pDateItem->GetSize();
	m_arrayDealPosition.RemoveAll();
	m_arrayDealName.RemoveAll();
	m_arrayDealDrawPosition.RemoveAll();
	m_arrayDealPosition.SetSize(nTotalDataCnt);
	m_arrayDealName.SetSize(nTotalDataCnt);
	m_arrayDealDrawPosition.SetSize(nTotalDataCnt);
}

BOOL CStrategyWnd::IsFirstLiquidation(CDealData* pEntryDealData, BOOL& bExist_ForcedLiquidation)
{
	int nEntryPosition = pEntryDealData->m_nIndex;
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	BOOL bRet = FALSE;

	// 청산신호
	if(IsSellOrExitLong(pEntryDealData->m_lDealValue))
	{
		// 맨처음에 들어오는 청산신호
		if(m_bIsFirstSignal)
		{
			// 주문시작신호 -> 진입신호로 선택한 경우
			if(m_nOrderStartSignal == 0);
			// 주문시작신호 -> 모든신호로 선택한 경우
			else if(m_nOrderStartSignal == 1)
			{
				// 신호모양이 위쪽에 놓이므로 고가로 셋팅
				m_arrayDealPosition.SetAt(nEntryPosition, pEntryDealData->m_lDealValue);
				m_arrayDealName.SetAt(nEntryPosition, pEntryDealData->m_strDealName);
				m_arrayDealDrawPosition.SetAt(nEntryPosition, pHigh->GetAt(nEntryPosition));

				// 이제부턴 처음 이외의 신호를 의미
				m_bIsFirstSignal = FALSE;
			}
		}
		// 맨처음을 제외한 청산신호 (중복진입 허용으로 이전 신호가 진입이었던 경우)
		// 이전 신호가 진입이 아닌 경우는 무시하고 다음 신호를 조회함
		else if(m_bCheckPrev_EntryPosition)
		{
			// 신호모양이 위쪽에 놓이므로 고가로 셋팅
			m_arrayDealPosition.SetAt(nEntryPosition, pEntryDealData->m_lDealValue);
			m_arrayDealName.SetAt(nEntryPosition, pEntryDealData->m_strDealName);
			m_arrayDealDrawPosition.SetAt(nEntryPosition, pHigh->GetAt(nEntryPosition));
		}

		// 이전 신호를 청산신호로 셋팅
		m_bCheckPrev_EntryPosition = FALSE;

		// 중복진입 횟수추적 초기화
		m_nRepeatCount = 0;

		// 청산신호이므로 TRUE 리턴
		bRet = TRUE;
	}

	return bRet;
}

BOOL CStrategyWnd::IsLastEntry(CDealData* pEntryDealData, BOOL& bExist_ForcedLiquidation, POSITION& posLast)
{
	int nEntryPosition = pEntryDealData->m_nIndex;
	CIDataItem* pLow = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);

	if(posLast == NULL && IsBuyOrExitShort(pEntryDealData->m_lDealValue))
	{
		// 이전 신호가 진입인 경우
		if(m_bCheckPrev_EntryPosition)
		{
			// 중복진입 허용인 경우
			if(m_nUseRepeatEntry && m_nRepeatEntryCount > m_nRepeatCount++)
			{
				// 신호모양이 아래쪽에 놓이므로 저가로 셋팅
				m_arrayDealPosition.SetAt(nEntryPosition, pEntryDealData->m_lDealValue);		
				m_arrayDealName.SetAt(nEntryPosition, pEntryDealData->m_strDealName);
				m_arrayDealDrawPosition.SetAt(nEntryPosition, pLow->GetAt(nEntryPosition));
			}
		}
		// 이전 신호가 청산인 경우
		else
		{
			// 신호모양이 아래쪽에 놓이므로 저가로 셋팅
			m_arrayDealPosition.SetAt(nEntryPosition, pEntryDealData->m_lDealValue);		
			m_arrayDealName.SetAt(nEntryPosition, pEntryDealData->m_strDealName);
			m_arrayDealDrawPosition.SetAt(nEntryPosition, pLow->GetAt(nEntryPosition));

			m_bCheckPrev_EntryPosition = FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CStrategyWnd::IsLastEntry_ForReal(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation)
{
	int nExitPosition = pExitDealData->m_nIndex;
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pLow = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);

	if(IsBuyOrExitShort(pExitDealData->m_lDealValue))
	{
		// 봉 완성시를 위한 백업
		m_TempDealData = *pExitDealData;
		
		// 주문시점 (0 : 조건 만족시)
		if(m_nTiming_Order == 0 || (m_nDivideState == 5 && m_nUnitPos == 1))
		{
			// 신호모양이 아래쪽에 놓이므로 저가로 셋팅
			m_arrayDealPosition.SetAt(nExitPosition, pExitDealData->m_lDealValue);		
			m_arrayDealName.SetAt(nExitPosition, pExitDealData->m_strDealName);
			m_arrayDealDrawPosition.SetAt(nExitPosition, pLow->GetAt(nExitPosition));

			// 아직 주문정보가 셋팅되지 않은 경우
			if(m_bSetComplete_OrderInfo == FALSE)
			{
				m_dealDataItem.SetData(m_TempDealData.m_nIndex, 
										m_TempDealData.m_lDealValue,
										m_TempDealData.m_lType,
										m_TempDealData.m_strDealName,
										m_TempDealData.m_dPrice,
										m_TempDealData.m_dContract,
										&m_TempDealData.m_strListEntryNameForExit);

				m_TempCondDealData = m_TempDealData;

				// 주문정보 셋팅완료
				m_bSetComplete_OrderInfo = TRUE;

				// 중복진입 횟수증가
				m_nRepeatCount++;
//				FILE *fp = fopen("c:\\Logs\\RepeatCount.txt", "a+t");
//				if(fp != NULL) 
//				{
//					fprintf(fp, "++[DivideState][%d]-[UnitPos][%d]-[RepeatCount][%d]-[Func][%s]\n", m_nDivideState, m_nUnitPos, m_nRepeatCount, "IsLastEntry_ForReal");
//					fclose(fp);
//				}
			}
		}

		return TRUE;
	}
	// DealValue가 0값인 신호처리
	else if(pExitDealData->m_lDealValue == 0)
	{
		// 봉 완성시를 위한 백업
		m_TempDealData = *pExitDealData;

		// 주문시점이 조건 만족시일 경우
		if(m_nTiming_Order == 0 || (m_nDivideState == 5 && m_nUnitPos == 1))
		{
			// 신호 셋팅 취소
			m_arrayDealPosition.SetAt(nExitPosition, pExitDealData->m_lDealValue);
			m_arrayDealName.SetAt(nExitPosition, "");
			m_arrayDealDrawPosition.SetAt(nExitPosition, 0);

			m_TempDealData.Clear();
		}
	}

	return FALSE;
}

void CStrategyWnd::Confirm_ForcedLiquidation(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation)
{
	//-------- 매수 / 매도청산-------- 
	BuyOrExitShort(pEntryDealData, bExist_ForcedLiquidation);

	CForcedLiquidationInfo FLInfo;
	//-------- 강제청산 옵션비교 --------
	// 손절매(손실률) 옵션 비교
	if(IsLossRate())
		ForcedLiquidation_ForLossRate(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);

	// 목표수익 옵션 비교
	if(IsTargetBenefit())
		ForcedLiquidation_ForTargetBenefit(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);

	// 최고가대비 하락 옵션 비교
	if(IsHPriceCPBear())
		ForcedLiquidation_ForHPriceCPBear(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);

//	// 당일 저가대비 상승 옵션 비교
//	if(IsLPriceCPBull())
//		ForcedLiquidation_ForLPriceCPBull(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);

	// 목표가보존 옵션 비교
	if(IsTargetValuePreserve())
		ForcedLiquidation_ForTargetValuePreserve(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);

	// 이익보존 옵션 비교
	if(IsBenefitPreserve())
		ForcedLiquidation_ForBenefitPreserve(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);

	// 최소가격변화 옵션 비교
	if(IsMiniPrice())
		ForcedLiquidation_ForMiniPrice(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);

	// 당일청산 옵션 비교
	if(IsDayLiquidation())
		ForcedLiquidation_ForDayLiquidation(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);

	// 강제청산 적용 (발생한 강제청산 중 먼저 발생한 위치로 셋팅)
	if(bExist_ForcedLiquidation)
		ForcedLiquidation(pExitDealData, FLInfo);

	//-------- 매도 / 매수청산 -------- 
	SellOrExitLong(pExitDealData, bExist_ForcedLiquidation);
}

void CStrategyWnd::Confirm_ForcedLiquidation_ForReal(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation)
{
	CForcedLiquidationInfo FLInfo;
	//-------- 강제청산 옵션비교 --------
	// 손절매(손실률) 옵션 비교
	if(IsLossRate())
		ForcedLiquidation_ForLossRate(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);
	
	// 목표수익 옵션 비교
	if(IsTargetBenefit())
		ForcedLiquidation_ForTargetBenefit(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);
	
	// 최고가대비 하락 옵션 비교
	if(IsHPriceCPBear())
		ForcedLiquidation_ForHPriceCPBear(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);
	
	//	// 당일 저가대비 상승 옵션 비교
	//	if(IsLPriceCPBull())
	//		ForcedLiquidation_ForLPriceCPBull(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);
	
	// 목표가보존 옵션 비교
	if(IsTargetValuePreserve())
		ForcedLiquidation_ForTargetValuePreserve(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);
	
	// 이익보존 옵션 비교
	if(IsBenefitPreserve())
		ForcedLiquidation_ForBenefitPreserve(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);
	
	// 최소가격변화 옵션 비교
	if(IsMiniPrice())
		ForcedLiquidation_ForMiniPrice(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);
	
	// 당일청산 옵션 비교
	if(IsDayLiquidation())
		ForcedLiquidation_ForDayLiquidation(pEntryDealData, pExitDealData, bExist_ForcedLiquidation, FLInfo);
	
	// 강제청산 적용 (발생한 강제청산 중 먼저 발생한 위치로 셋팅)
	if(bExist_ForcedLiquidation)
		ForcedLiquidation_ForReal(pExitDealData, FLInfo);

	//-------- 매도 / 매수청산 -------- 
	SellOrExitLong_ForReal(pEntryDealData, pExitDealData, bExist_ForcedLiquidation);
}

// 매수 / 매도청산
void CStrategyWnd::BuyOrExitShort(CDealData* pEntryDealData, BOOL& bExist_ForcedLiquidation)
{
	int nEntryPosition = pEntryDealData->m_nIndex;
	CIDataItem* pLow = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);

	// 무조건 진입신호만 들어옴
	// 중복진입 허용인 경우 
	if(m_nUseRepeatEntry)
	{
		// 중복진입 허용갯수만큼 진입표시 셋팅
		if(m_nRepeatEntryCount > m_nRepeatCount++)
		{
			// 신호모양이 아래쪽에 놓이므로 저가로 셋팅
			m_arrayDealPosition.SetAt(nEntryPosition, pEntryDealData->m_lDealValue);
			m_arrayDealName.SetAt(nEntryPosition, pEntryDealData->m_strDealName);
			m_arrayDealDrawPosition.SetAt(nEntryPosition, pLow->GetAt(nEntryPosition));

			// 이제부턴 처음 이외의 신호를 의미
			m_bIsFirstSignal = FALSE;
		}
	}
	// 중복진입 허용하지 않은 경우
	else
	{
		// 처음으로 들어오는 진입신호인 경우
		if(m_bIsFirstSignal)
		{
			// 신호모양이 아래쪽에 놓이므로 저가로 셋팅
			m_arrayDealPosition.SetAt(nEntryPosition, pEntryDealData->m_lDealValue);
			m_arrayDealName.SetAt(nEntryPosition, pEntryDealData->m_strDealName);
			m_arrayDealDrawPosition.SetAt(nEntryPosition, pLow->GetAt(nEntryPosition));

			// 이제부턴 처음 이외의 신호를 의미
			m_bIsFirstSignal = FALSE;
		}
		// 이전 신호가 청산인 경우
		else if(!m_bCheckPrev_EntryPosition)
		{
			// 신호모양이 아래쪽에 놓이므로 저가로 셋팅
			m_arrayDealPosition.SetAt(nEntryPosition, pEntryDealData->m_lDealValue);
			m_arrayDealName.SetAt(nEntryPosition, pEntryDealData->m_strDealName);
			m_arrayDealDrawPosition.SetAt(nEntryPosition, pLow->GetAt(nEntryPosition));
		}
	}

	// 이전 신호를 진입신호로 셋팅
	m_bCheckPrev_EntryPosition = TRUE;

	// 강제청산 유무 초기화
	bExist_ForcedLiquidation = FALSE;
}

// 매도 / 매수청산
void CStrategyWnd::SellOrExitLong(CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation)
{
	int nExitPosition = pExitDealData->m_nIndex;
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pLow = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);

	// 일반 청산
	if(!bExist_ForcedLiquidation && IsSellOrExitLong(pExitDealData->m_lDealValue))
	{
		// 신호모양이 위쪽에 놓이므로 고가로 셋팅
		m_arrayDealPosition.SetAt(nExitPosition, pExitDealData->m_lDealValue);
		m_arrayDealName.SetAt(nExitPosition, pExitDealData->m_strDealName);
		m_arrayDealDrawPosition.SetAt(nExitPosition, pHigh->GetAt(nExitPosition));

		// 이전 신호를 청산신호로 셋팅
		m_bCheckPrev_EntryPosition = FALSE;		
		
		// 중복진입 횟수추적 초기화
		m_nRepeatCount = 0;
	}
	// 중복진입 여부확인
	else if(!bExist_ForcedLiquidation && IsBuyOrExitShort(pExitDealData->m_lDealValue))
	{
		// 중복진입 허용인 경우 허용갯수만큼 진입표시 셋팅
		if(m_nUseRepeatEntry && m_nRepeatEntryCount > m_nRepeatCount++)
		{
			// 신호모양이 위쪽에 놓이므로 고가로 셋팅
			m_arrayDealDrawPosition.SetAt(nExitPosition, pLow->GetAt(nExitPosition));
			m_arrayDealPosition.SetAt(nExitPosition, pExitDealData->m_lDealValue);
			m_arrayDealName.SetAt(nExitPosition, pExitDealData->m_strDealName);
		}
	}
}

// 매도 / 매수청산
void CStrategyWnd::SellOrExitLong_ForReal(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation)
{
	int nExitPosition = pExitDealData->m_nIndex;
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pLow = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);

	// 일반 청산
	if(!bExist_ForcedLiquidation && IsSellOrExitLong(pExitDealData->m_lDealValue))
	{
		// 중복진입 확인만 (중복진입 카운트는 증가하지 않음)
		if((m_nUseRepeatEntry && m_nRepeatEntryCount >= m_nRepeatCount) || m_nUseRepeatEntry == 0)
		{
			// 봉 완성시를 위한 백업
			m_TempDealData = *pExitDealData;
			
			// 주문시점이 조건 만족시일 경우
			if(m_nTiming_Order == 0 || (m_nDivideState == 5 && m_nUnitPos == 1))
			{
				// 신호모양이 위쪽에 놓이므로 고가로 셋팅
				m_arrayDealPosition.SetAt(nExitPosition, pExitDealData->m_lDealValue);
				m_arrayDealName.SetAt(nExitPosition, pExitDealData->m_strDealName);
				m_arrayDealDrawPosition.SetAt(nExitPosition, pHigh->GetAt(nExitPosition));
				
				// 아직 주문정보가 셋팅되지 않은 경우
				if(m_bSetComplete_OrderInfo == FALSE)
				{
					m_dealDataItem.SetData(m_TempDealData.m_nIndex, 
											m_TempDealData.m_lDealValue,
											m_TempDealData.m_lType,
											m_TempDealData.m_strDealName,
											m_TempDealData.m_dPrice,
											m_TempDealData.m_dContract,
											&m_TempDealData.m_strListEntryNameForExit);
					
					m_TempCondDealData = m_TempDealData;
					
					// 주문정보 셋팅완료
					m_bSetComplete_OrderInfo = TRUE;
					
					// 중복진입 횟수추적 초기화
					m_nRepeatCount = 0;
//					FILE *fp = fopen("c:\\Logs\\RepeatCount.txt", "a+t");
//					if(fp != NULL) 
//					{
//						fprintf(fp, "--[DivideState][%d]-[UnitPos][%d]-[RepeatCount][%d]-[Func][%s]\n", m_nDivideState, m_nUnitPos, m_nRepeatCount, "SellOrExitLong_ForReal");
//						fclose(fp);
//					}
				}
			}
		}
	}
	// 중복진입 여부확인
	else if(!bExist_ForcedLiquidation && IsBuyOrExitShort(pExitDealData->m_lDealValue))
	{
		// 중복진입 확인만 (중복진입 카운트는 증가하지 않음)
		if(m_nUseRepeatEntry && m_nRepeatEntryCount > m_nRepeatCount)
		{
			// 봉 완성시를 위한 백업
			m_TempDealData = *pExitDealData;

			// 주문시점이 조건 만족시일 경우
			if(m_nTiming_Order == 0 || (m_nDivideState == 5 && m_nUnitPos == 1))
			{
				// 1틱의 경우 중복허용 1회이상인 경우
				if(m_nRepeatEntryCount == m_nRepeatCount)
					return;

				// 신호모양이 위쪽에 놓이므로 고가로 셋팅
				m_arrayDealPosition.SetAt(nExitPosition, pExitDealData->m_lDealValue);
				m_arrayDealName.SetAt(nExitPosition, pExitDealData->m_strDealName);
				m_arrayDealDrawPosition.SetAt(nExitPosition, pLow->GetAt(nExitPosition));

				// 아직 주문정보가 셋팅되지 않은 경우
				if(m_bSetComplete_OrderInfo == FALSE)
				{
					m_dealDataItem.SetData(m_TempDealData.m_nIndex, 
											m_TempDealData.m_lDealValue,
											m_TempDealData.m_lType,
											m_TempDealData.m_strDealName,
											m_TempDealData.m_dPrice,
											m_TempDealData.m_dContract,
											&m_TempDealData.m_strListEntryNameForExit);

					m_TempCondDealData = m_TempDealData;

					// 주문정보 셋팅완료
					m_bSetComplete_OrderInfo = TRUE;

					// 중복진입 횟수증가
					m_nRepeatCount++;
//					FILE *fp = fopen("c:\\Logs\\RepeatCount.txt", "a+t");
//					if(fp != NULL) 
//					{
//						fprintf(fp, "++[DivideState][%d]-[UnitPos][%d]-[RepeatCount][%d]-[Func][%s]\n", m_nDivideState, m_nUnitPos, m_nRepeatCount, "SellOrExitLong_ForReal");
//						fclose(fp);
//					}
				}
			}
		}
	}
	// DealValue가 0값인 신호처리
	else if(!bExist_ForcedLiquidation && !pExitDealData->m_lDealValue)
	{
		// 봉 완성시를 위한 백업
		m_TempDealData = *pExitDealData;

		// 주문시점이 조건 만족시일 경우
		if(m_nTiming_Order == 0 || (m_nDivideState == 5 && m_nUnitPos == 1))
		{
			// 신호 셋팅 취소
			m_arrayDealPosition.SetAt(nExitPosition, pExitDealData->m_lDealValue);
			m_arrayDealName.SetAt(nExitPosition, "");
			m_arrayDealDrawPosition.SetAt(nExitPosition, 0);

			m_TempDealData.Clear();
		}
	}
}

// 매수 사용여부
BOOL CStrategyWnd::IsBuy(long lDealType)
{
	return lDealType & VALENTRYBUY;
}

// 매도청산 사용여부
BOOL CStrategyWnd::IsExitShort(long lDealType)
{
	return lDealType & VALEXITSHORT;
}

// 매도 사용여부
BOOL CStrategyWnd::IsSell(long lDealType)
{
	return lDealType & VALENTRYSELL;
}

// 매수청산 사용여부
BOOL CStrategyWnd::IsExitLong(long lDealType)
{
	return lDealType & VALEXITLONG;
}

// 매수 / 매도청산 사용여부
BOOL CStrategyWnd::IsBuyOrExitShort(long lDealType)
{
	return IsBuy(lDealType) || IsExitShort(lDealType);
}

// 매도 / 매수청산 사용여부
BOOL CStrategyWnd::IsSellOrExitLong(long lDealType)
{
	return IsSell(lDealType) || IsExitLong(lDealType);
}

// 손절매 사용여부
BOOL CStrategyWnd::IsLossRate()
{
	return m_bUseLossRate;
}

// 목표수익 사용여부
BOOL CStrategyWnd::IsTargetBenefit()
{
	return m_bUseTargetBenefit;
}

// 최고가대비하락 사용여부
BOOL CStrategyWnd::IsHPriceCPBear()
{
	return m_bUseHPriceCPBear;
}

//// 저가대비상승 사용여부
//BOOL CStrategyWnd::IsLPriceCPBull()
//{
//	return m_bUseLPriceCPBull;
//}

BOOL CStrategyWnd::IsTargetValuePreserve()
{
	return m_bUseTargetValuePreserve;
}

BOOL CStrategyWnd::IsBenefitPreserve()
{
	return m_bUseBenefitPreserve;
}

// 최소가격 사용여부
BOOL CStrategyWnd::IsMiniPrice()
{
	return m_bUseMiniPrice;
}

// 당일청산 사용여부
BOOL CStrategyWnd::IsDayLiquidation()
{
//	// 차후에 m_nDivideState값 사용
//	int nDivideState = 0;
//	if(m_pDateItem)
//		nDivideState = m_pDateItem->GetType();

	return (m_bUseDayLiquidation && (m_nDivideState == 4 && m_nUnitPos != 60));	// 분봉에서만 적용됨
}

// 손절매로 인한 강제청산
void CStrategyWnd::ForcedLiquidation_ForLossRate(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo)
{
	int nEntryPosition = pEntryDealData->m_nIndex;			// 진입 위치
	int nExitPosition = pExitDealData->m_nIndex;			// 청산 위치
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);
	double dClose = pClose->GetAt(nEntryPosition);			// 종가

	double dBearValue = 0.;		// 하락한 값
	// %
	if(m_uLossRate_PerOrWon == 0)
		dBearValue = dClose - (dClose * m_dLossRate); 	// 일정비율(%) 하락한 경우
	// 원
	else
		dBearValue = dClose - m_dLossRate;				// 일정금액(원) 하락한 경우

	// EntryPosition 과 ExitPosition 사이에서 손절매 옵션으로 강제청산할 Position 순회
	double dValueForCompare;
	for(int nIndx = nEntryPosition + 1; nIndx <= nExitPosition; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// 매수신호가 봉의 종가에서 일정비율 또는 일정금액보다 하락한 경우
		if(dValueForCompare <= dBearValue)
		{
			FLInfo.SetFLInfo(nIndx, VALSONJUL, pHigh);

			// 강제청산 발생
			bExist_ForcedLiquidation = TRUE;
			break;
		}
	}
}

// 목표수익으로 인한 강제청산
void CStrategyWnd::ForcedLiquidation_ForTargetBenefit(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo)
{
	int nEntryPosition = pEntryDealData->m_nIndex;			// 진입 위치
	int nExitPosition = pExitDealData->m_nIndex;			// 청산 위치
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);
	double dClose = pClose->GetAt(nEntryPosition);			// 종가

	double dBullValue = 0.;		// 상승한 값
	// %
	if(m_uTargetBenefit_PerOrWon == 0)
		dBullValue = dClose + (dClose * m_dTargetBenefit);	// 일정비율(%) 상승한 경우
	// 원
	else
		dBullValue = dClose + m_dTargetBenefit;				// 일정금액(원) 상승한 경우

	// EntryPosition 과 ExitPosition 사이에서 목표수익 옵션으로 강제청산할 Position 순회
	double dValueForCompare;
	for(int nIndx = nEntryPosition + 1; nIndx <= nExitPosition; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// 매수신호가 봉의 종가에서 일정비율 또는 일정금액보다 상승한 경우
		if(dValueForCompare >= dBullValue)
		{
			FLInfo.SetFLInfo(nIndx, VALSOOIK, pHigh);

			// 강제청산 발생
			bExist_ForcedLiquidation = TRUE;
			break;
		}
	}
}

// 고가대비하락으로 인한 강제청산
void CStrategyWnd::ForcedLiquidation_ForHPriceCPBear(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo)
{
	int nEntryPosition = pEntryDealData->m_nIndex;		// 진입 위치
	int nExitPosition = pExitDealData->m_nIndex;		// 청산 위치
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);

	double dHighValue = 0.;
	double dCloseValue = 0.;
	double dHighEach = 0.;
	double dCloseEach = 0.;

	for(int nIndx = nEntryPosition + 1; nIndx <= nExitPosition; ++nIndx)
	{
		dHighEach = pHigh->GetAt(nIndx);

		// 이후 고가가 더 높은 경우 최고가로 갱신
		if(dHighValue - dHighEach < 0.)
			dHighValue = dHighEach;


		dCloseEach = pClose->GetAt(nIndx);
		double dBearValue = 0.;		// 하락한 값
		// %
		if(m_uHPriceCPBear_PerOrWon == 0)
			dBearValue = dHighValue - (dHighValue * m_dHPriceCPBear);		// 일정비율(%) 하락한 경우
		// 원
		else
			dBearValue = dHighValue - m_dHPriceCPBear;	 					// 일정금액(원) 하락한 경우

		// 일정비율 또는 일정금액으로 종가가 계산된 기준가보다 하락한 경우
		if(dCloseEach <= dBearValue)
		{
			FLInfo.SetFLInfo(nIndx, VALGOGA, pHigh);

			// 강제청산 발생
			bExist_ForcedLiquidation = TRUE;
			return;
		}
	}
}

//// 저가대비상승으로 인한 강제청산
//void CStrategyWnd::ForcedLiquidation_ForLPriceCPBull(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo)
//{
//	int nEntryPosition = pEntryDealData->m_nIndex;		// 진입 위치
//	int nExitPosition = pExitDealData->m_nIndex;		// 청산 위치
//	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
//	CIDataItem* pLow = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(2);
//	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);
//	double dLowValue = 0.;
//	double dCloseValue = 0.;
//	for(int nIndx = nEntryPosition + 1; nIndx <= nExitPosition; ++nIndx)
//	{
//		double dLowEach = pLow->GetAt(nIndx);
//		double dCloseEach = pClose->GetAt(nIndx);
//		// 이후 저가가 더 낮은 경우 최저가로 갱신
//		if(dLowValue - dLowEach > 0.)
//			dLowValue = dLowEach;
//		double dBullValue = 0.;		// 상승한 값
//		// %
//		if(m_uLPriceCPBull_PerOrWon == 0)
//			dBullValue = dLowValue + (dLowValue * m_dLPriceCPBull);	// 일정비율(%) 상승한 경우
//		// 원
//		else
//			dBullValue = dLowValue + m_dLPriceCPBull; 				// 일정금액(원) 상승한 경우
//		// 일정비율 또는 일정금액으로 종가가 계산된 기준가보다 상승한 경우
//		if(dCloseEach >= dBullValue)
//		{
//			FLInfo.SetFLInfo(nIndx, VALJUGA, pHigh);
//			// 강제청산 발생
//			bExist_ForcedLiquidation = TRUE;
//			break;
//		}
//	}
//}

// 목표가보존을 위한 강제청산
void CStrategyWnd::ForcedLiquidation_ForTargetValuePreserve(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo)
{
	int nEntryPosition = pEntryDealData->m_nIndex;			// 진입 위치
	int nExitPosition = pExitDealData->m_nIndex;			// 청산 위치
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);
	double dClose = pClose->GetAt(nEntryPosition);			// 종가

	double dBullValue = 0.;		// 상승한 값
	int nIndx = 0;
	// %
	if(m_uTargetValuePreserve_PerOrWon == 0)
		dBullValue = dClose + (dClose * m_dTargetValuePreserve);	// 일정비율(%) 상승한 경우
	// 원
	else
		dBullValue = dClose + m_dTargetValuePreserve;				// 일정금액(원) 상승한 경우

	// EntryPosition 과 ExitPosition 사이에서 목표가보존 옵션으로 강제청산할 Position 순회
	double dValueForCompare = 0.;
	double dValueForTargetValuePreserve = 0.;
	for( nIndx = nEntryPosition + 1; nIndx <= nExitPosition; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// 매수신호가 봉의 종가에서 일정비율 또는 일정금액보다 상승한 경우
		if(dValueForCompare >= dBullValue)
		{
			dValueForTargetValuePreserve = dBullValue;
			break;
		}
	}
	
	// 목표가보존 최고가 이후의 Position 에서 ExitPosition 사이에서 최고가 하락 돌파에 대한 강제청산 Position 순회
	for(nIndx = nIndx + 1; nIndx <= nExitPosition; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// 매수신호가 봉의 종가에서 일정비율 또는 일정금액보다 상승한 경우
		if(dValueForCompare < dValueForTargetValuePreserve)
		{
			FLInfo.SetFLInfo(nIndx, VALTARGETPRESERV, pHigh);

			// 강제청산 발생
			bExist_ForcedLiquidation = TRUE;
			break;
		}
	}
}

// 이익보존을 위한 강제청산
void CStrategyWnd::ForcedLiquidation_ForBenefitPreserve(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo)
{
	int nEntryPosition = pEntryDealData->m_nIndex;			// 진입 위치
	int nExitPosition = pExitDealData->m_nIndex;			// 청산 위치
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);
	double dClose = pClose->GetAt(nEntryPosition);			// 종가

	double dBullValue = 0.;		// 상승한 값
	int nIndx = 0;
	// %
	if(m_uBenefitPreserveUp_PerOrWon == 0)
		dBullValue = dClose + (dClose * m_dBenefitPreserveUp);	// 일정비율(%) 상승한 경우
	// 원
	else
		dBullValue = dClose + m_dBenefitPreserveUp;				// 일정금액(원) 상승한 경우

	// EntryPosition 과 ExitPosition 사이에서 이익보존 옵션으로 강제청산할 Position 순회
	double dValueForCompare = 0.;
	double dValueForBenefitPreserve = 0.;
	for( nIndx = nEntryPosition + 1; nIndx <= nExitPosition; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// 매수신호가 봉의 종가에서 일정비율 또는 일정금액보다 상승한 경우
		if(dValueForCompare >= dBullValue)
		{
			dValueForBenefitPreserve = dBullValue;
			break;
		}
	}

	double dBearValue = 0.;		// 하락한 값
	// %
	if(m_uBenefitPreserveDn_PerOrWon == 0)
		dBearValue = dValueForBenefitPreserve - (dValueForBenefitPreserve * m_dBenefitPreserveDn);	// 일정비율(%) 상승한 경우
	// 원
	else
		dBearValue = dValueForBenefitPreserve - m_dBenefitPreserveDn;										// 일정금액(원) 상승한 경우

	// 이익보존 최고가 이후의 Position 에서 ExitPosition 사이에서 하락율 적용하여 강제청산할 Position 순회
	for(nIndx = nIndx + 1; nIndx <= nExitPosition; ++nIndx)
	{
		dValueForCompare = pClose->GetAt(nIndx);

		// 매수신호가 봉의 종가에서 일정비율 또는 일정금액보다 하락한 경우
		if(dValueForCompare <= dBearValue)
		{
			FLInfo.SetFLInfo(nIndx, VALBENEFITPRESERV, pHigh);

			// 강제청산 발생
			bExist_ForcedLiquidation = TRUE;
			break;
		}
	}
}

// 최소가격으로 인한 강제청산
void CStrategyWnd::ForcedLiquidation_ForMiniPrice(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo)
{
	int nEntryPosition = pEntryDealData->m_nIndex;			// 진입 위치
	int nExitPosition = pExitDealData->m_nIndex;			// 청산 위치
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);
	CIDataItem* pClose = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(3);
	double dClose = pClose->GetAt(nEntryPosition);			// 종가

	if(m_nBongCount > 0)
	{
		// EntryPosition 과 ExitPosition 사이에서 최소가격변화 옵션으로 강제청산할 Position 순회
		double dValueForCompare;
		double dRatio = m_dMiniPrice;
		for(int nIndx = nEntryPosition + m_nBongCount; nIndx <= nExitPosition; )
		{
			dValueForCompare = pClose->GetAt(nIndx);

			double dBullValue = 0.;		// 상승한 값
			// %
			if(m_uMiniPrice_PerOrWon == 0)
				dBullValue = dClose + (dClose * dRatio); 	// 증가되는 일정비율(%) 상승한 경우
			// 원
			else
				dBullValue = dClose + dRatio;				// 증가되는 일정금액(원) 상승한 경우

			// 종가가 일정비율 또는 일정금액이내로 횡보를 유지할 경우
			// 즉 기준가가 종가보다 높을 경우
			if(dValueForCompare <= dBullValue)
			{
				FLInfo.SetFLInfo(nIndx, VALMIN, pHigh);

				// 강제청산 발생
				bExist_ForcedLiquidation = TRUE;
				break;
			}
			nIndx += m_nBongCount;
			dRatio += m_dMiniPrice;
		}
	}
}

// 당일청산으로 인한 강제청산
void CStrategyWnd::ForcedLiquidation_ForDayLiquidation(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo)
{
	int nEntryPosition = pEntryDealData->m_nIndex;		// 진입 위치
	int nExitPosition = pExitDealData->m_nIndex;		// 청산 위치
	CIDataItem* pHigh = (CIDataItem *)m_ptrArrayHistoryDataItem.GetAt(1);

	CString szDate;
	CString szHour;
	int		nMin;
	CString szCheckHour;
	int		nCheckMin;

	// 분 간격
	int	nInterval = m_nUnitPos;

	for(int nIndx = nEntryPosition + 1; nIndx <= nExitPosition; ++nIndx)
	{
		// 월일시분(예 : 10281310)
		szDate.Format("%08.8s", m_pDateItem->GetAt(nIndx));
		szHour = szDate.Mid(4, 2);
		nMin = atoi(szDate.Mid(6, 2));

		// 설정된 시분(예 : 1310)
		szCheckHour = m_szTimeDayLiquidation.Mid(0, 2);
		nCheckMin = atoi(m_szTimeDayLiquidation.Mid(2));
		if(szCheckHour.Compare(szHour) == 0 &&		// Hour Check
			nCheckMin <= nMin && abs(nCheckMin - nMin) <= nInterval)		// Min Check
		{
			FLInfo.SetFLInfo(nIndx, VALDANGIL, pHigh);

			// 강제청산 발생
			bExist_ForcedLiquidation = TRUE;
			break;
		}
	}
}

void CStrategyWnd::ForcedLiquidation(CDealData* pExitDealData, CForcedLiquidationInfo& FLInfo)
{
	// 중복진입 횟수추적 초기화
	m_nRepeatCount = 0;

	// 강제청산 옵션으로 다시 셋팅
	pExitDealData->m_lDealValue = FLInfo.m_lDealValue;
	pExitDealData->m_nIndex = FLInfo.m_nExitIndex;

	// 신호정보 셋팅
	m_arrayDealPosition.SetAt(FLInfo.m_nExitIndex, FLInfo.m_lDealValue);
	m_arrayDealDrawPosition.SetAt(FLInfo.m_nExitIndex, FLInfo.m_pDataItem->GetAt(FLInfo.m_nExitIndex));

	// 이전 신호를 청산신호로 셋팅
	m_bCheckPrev_EntryPosition = FALSE;
}

void CStrategyWnd::ForcedLiquidation_ForReal(CDealData* pExitDealData, CForcedLiquidationInfo& FLInfo)
{
	// 강제청산 옵션으로 다시 셋팅
	pExitDealData->m_lDealValue = FLInfo.m_lDealValue;
	pExitDealData->m_nIndex = FLInfo.m_nExitIndex;

	// 봉 완성시를 위한 백업
	m_TempDealData = *pExitDealData;

	// 강제청산시점이 조건 만족시일 경우
	if(m_nTiming_ForcedLiquidation == 0 || (m_nDivideState == 5 && m_nUnitPos == 1))
	{
		// 신호정보 셋팅
		m_arrayDealPosition.SetAt(FLInfo.m_nExitIndex, FLInfo.m_lDealValue);
		m_arrayDealDrawPosition.SetAt(FLInfo.m_nExitIndex, FLInfo.m_pDataItem->GetAt(FLInfo.m_nExitIndex));

		// 아직 주문정보가 셋팅되지 않은 경우
		if(m_bSetComplete_OrderInfo == FALSE)
		{
			m_dealDataItem.SetData(m_TempDealData.m_nIndex, 
									m_TempDealData.m_lDealValue,
									m_TempDealData.m_lType,
									m_TempDealData.m_strDealName,
									m_TempDealData.m_dPrice,
									m_TempDealData.m_dContract,
									&m_TempDealData.m_strListEntryNameForExit);

			m_TempCondDealData = m_TempDealData;
			m_TempForcedDealData = m_TempDealData;

			// 주문정보 셋팅완료
			m_bSetComplete_OrderInfo = TRUE;

			// 중복진입 횟수추적 초기화
			m_nRepeatCount = 0;
//			FILE *fp = fopen("c:\\Logs\\RepeatCount.txt", "a+t");
//			if(fp != NULL) 
//			{
//				fprintf(fp, "--[DivideState][%d]-[UnitPos][%d]-[RepeatCount][%d]-[Func][%s]\n", m_nDivideState, m_nUnitPos, m_nRepeatCount, "ForcedLiquidation_ForReal");
//				fclose(fp);
//			}
		}
	}
}