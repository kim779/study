#if !defined(AFX_STRATEGYWND_H__44B9B7B2_99DB_4966_95EC_AC22B1A93F42__INCLUDED_)
#define AFX_STRATEGYWND_H__44B9B7B2_99DB_4966_95EC_AC22B1A93F42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StrategyWnd.h : header file
//
//#include <AFXMT.H>
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CStrategyWnd window
#ifdef _DEBUG
#define _SETLOG
#endif

#include "../Common_ST/IStrategyItem.h"
#include "../Common_ST/STControllerDef.h"
#include "../Common_ST/STControllerDef2.h"
#include "../Common_ST/SDataItem.h"

#include "LogicMaker2.h"
#include "../Common_ST/DealDataItem.h"

class CForcedLiquidationInfo
{
public:
	CForcedLiquidationInfo();

public:
	int				m_nExitIndex;
	long			m_lDealValue;
	CIDataItem*		m_pDataItem;

public:
	void SetFLInfo(int nExitIndex, long lDealValue, CIDataItem* pDataItem);
};

class CFeeCal;
class CLogicItem;
class CStrategyWnd : public CWnd, public CIStrategyItem
{
// Construction
public:
	virtual ~CStrategyWnd();

	// 20060705 함수인자 제거 : HWND hSocket,/*long pDBMgr,*/
	//int Create(CWnd* pParentWnd,HWND hSocket,/*long pDBMgr,*/ long lKey, CStringArray &rStArrayPath);
	virtual int Create(CWnd* pParentWnd, long lKey, CStringArray &rStArrayPath, long pChartItem);

	virtual BOOL DestoryWindow();
	virtual HWND GetSafeHwnd( ) const;
	// atcion command
	virtual BOOL LoadSD(LPCTSTR lpStrategyDllPath,LPCTSTR lpInputCode,long lDataCnt,LPCTSTR lpEndDate, LPCTSTR lpTimeTypeNum,LPCTSTR lpTypeDealOption);	
	virtual BOOL LoadST(LPCTSTR lpStrategyPath,LPCTSTR lpInputCode,long lDataCnt,LPCTSTR lpEndDate);
	virtual BOOL Run(LPCTSTR lpOption);
	virtual BOOL Stop();
	virtual BOOL Pause();
	virtual long Update(long lPos, BOOL bIsUpdateData);		//@060922전략Real관련수정
	virtual CString GetTime();
	virtual long GetCount();
	virtual CString GetInputCode();
	virtual CString GetEndDate();
	virtual long GetDataCount(long lIndex);
	virtual CString GetTime(long lIndex);
	virtual long GetPtrDrawInfo(long lIndex);
	virtual BOOL CalculateStatisticalInfo(long lStartPos, long lEndPos);
	virtual long GetCntStrategyResult();
	virtual CStrategyResult* GetStrategyResult(long lIndex);
	virtual double GetStatisticalInfo(long lPos);
	virtual LPCTSTR GetErrorMessage();
	virtual BOOL HasAPData();
	virtual OPERATTIONSTATUS GetOperationStatus();
	virtual CString GetInputs(long lIndexST, long lIndexInput);
	virtual void GetCurrentPositonNProfit(long& rLCurrentDealPosition, double &rDCurrentProfitRate);
	virtual CString GetName(long lIndex);
	virtual CString GetPath(long lIndex);
	virtual CString GetProfileInfo(LPCTSTR lpAppName,LPCTSTR lpKeyName);
	virtual BOOL SetUpperLowerPrice(double dUpperPrice,double dLowerPrice);
	virtual BOOL SetVolumeForCalculatation(double dVol);// to calculate statical info..
	virtual double GetVolumeInDealData();

	//KHD : 시간최적화 봉의 갯수
	virtual int GetTimeBong(){return m_nBong;}
	virtual void SetTimeBong(int nBong){m_nBong = nBong; }
	//KHD : 변수최적화 변수
	virtual void SetValOptimizer(LONG str){
		m_pstValOptimazer =str;
	}
	virtual LONG GetValOptimizer(){
		return (LONG)m_pstValOptimazer;
	}
	virtual CString GetSendValString(){
		return m_stSendValue;
	}
	virtual void SetSendValString(CString stSend){
		m_stSendValue = stSend;
	}
	// atcion command
	virtual void OnSTSetRealDataST();
	virtual void	SetChartDivideInfo(int nDivideState, int nUnitPos);	// 차트 시간구분 정보
	
public:
	BOOL IsRunning();
	void RemoveAllArray();
	CStrategyResult *CreateStrategyResultData();
	void CalaulateStrategyResultData();
	LRESULT OnSTDataManagerMessageSD( WPARAM wParam, LPARAM lParam);
	LRESULT OnSTDataManagerMessageST( WPARAM wParam, LPARAM lParam);
	void OnSTSetAPDataST();
	void Init();
	void ExitAtTodayByTimeSelected(BOOL bStartTimer = TRUE);

// Attributes
private:
	CLogicMaker2 m_logicMaker2;
	CString m_strTimeTypeNum;
	int	 m_nLastSize;
	// For Action..
	double m_dUpperPrice;
	double m_dLowerPrice;
	double m_dVolForOrder;
	OPERATTIONSTATUS m_Status;
	ST_DRAWINFO_EX m_DrawInfo;
	CPtrArray m_ptrArrayPlot;
	CPtrArray m_ptrArraySDataItem;
	CPtrArray m_ptrArrayHistoryDataItem;

	CLogicItem* m_pLogicRootItem;
	int m_nCurrentRunningLogic;
	long m_pChartInterface;
	CString m_strStrategyPath;
	CISDataItem* m_pDateItem;
	CDealDataItem m_dealDataItem;
	BOOL m_bHasAPData;
	BOOL m_bIsRunning;
	long m_lKey;
	CString m_strInputCode;
	long m_lDataCnt;
	CString m_strEndDate;
	CString m_strCurrentTime;
	CString m_strError;

	// For SD Load
	CString m_strTypeSell;
	CString m_strTypeBuy;
	double m_dSellOption;
	double m_dBuyOption;

	// For Order Setting
	long m_lFirstOrderType;// 0:Buy, 1:Sell, 2:All
	BOOL m_bUseProfitReverseRate;
	double m_dProfitReverseRate;
	BOOL m_bEnableContinuty;
	long m_lMaxContinuty;
	double m_dMinimumEquity;
	double m_dMinimumFuture;
	double m_dFeeForEntry;
	double m_dFeeForExit;
	long m_lFeeUnit;
	double m_dSlippageForEntry;
	double m_dSlippageForExit;
	long m_lSlippageUnit;


	CFeeCal *m_pFeeCal;
	CFeeCal *m_pSlippageCal;

	enum ORDER_TIME	
	{
		ATCompletingCandle = 0,
		AtEventingSignal = 1
	};
	ORDER_TIME m_OrderTimeOfForcedExit;

	// For Calculation Of Realtime Exchange
	long m_lCurrentDealPosition;	// deal status
	double m_dCurrentProfitRate;	// rate status
	double m_dCurrentProfitRateOld;
	double m_dVolumeBought;
	double m_dAmountBought;
	double m_dAmountAccmulated;
	double m_dVolumeSold;
	double m_dAmountSold;
	
	// for Calculation of Statistical infomation..
	CArray<CStrategyResult*,CStrategyResult*>	m_arrayStrategyResult;
	double m_dSumLoss;
	double m_dMaxProfit;
	double m_dMaxLoss;
	double m_dSumAmountExit;
	double m_dSumProfit;
	long m_lCntProfit;
	long m_lCntLoss;
	long m_lCntProfitStraight;
	long m_lCntLossStraight;
	long m_lSumProfitStraight;
	long m_lSumLossStraight;
	long m_lSumCandleProfit;
	long m_lSumCandleLoss;
	long m_lCntCandleAccess;
	long m_lAllCandleInCalculation;

	CStringArray m_strArrayPath; //0: system, 1:User

	enum ST_TYPE {
		NONE,
		ST_ST = 1,
		ST_SD
	};
	ST_TYPE m_strategyType;
	
	int m_nBong; //KHD : 시간최적화에 쓸 현재 봉의 수
	LONG m_pstValOptimazer;//KHD :변수최적화에서...
	CString m_stSendValue;//KHD : 변수최적화에서...
	// For Action..
	// 수수료 및 갯수 설정 
	double m_dMedoRate;//매수청산
	double m_dMesuRate;//매수진입
	long   m_dqty;//갯수
	BOOL    m_bncbTax;// FALSE : % TRUE:Point
	//슬리피지
	double m_dMesuSlip;//SlipigyBuy
	double m_dMedoSlip;//SlipigySell
	BOOL   m_bSlipTex;//cbSplipigy FALSE : % TRUE : Point

	UINT	m_nTimerID;

public:
// 차트 시간구분 정보 셋팅
	int		m_nDivideState;										// 틱(0)/분(1)/일(2)/주(3)/월(4)/년(5)
	int		m_nUnitPos;											// 1(0),5(1),10(2),15(3)


// 전략설정 정보얻기
	void	InitConfiguration();								// 전략설정 초기화
	void	LoadConfiguration();								// 전략설정 셋팅
	void	ReadConfiguration(CString szPath);					// 전략설정 읽음
	void	ReadConfig_Signal(CString szPath);					// 신호(Setup) 설정 읽음
	void	ReadConfig_ForcedLiquidation(CString szPath);		// 강제청산(Forced Liquidation) 설정 읽음
	void	ReadConfig_Setup(CString szPath);					// 운용(Setup) 설정 읽음
	void	ReadConfig_Order(CString szPath);					// 주문(Order) 설정 읽음

	// 강제청산 관련변수 및 함수
	int		m_nTiming_ForcedLiquidation;	// 강제청산 시점 (Real인 경우만 사용)
	BOOL	m_bIsFirstSignal;				// 최초 신호인지?

	//손절매
	BOOL	m_bUseLossRate;
	double	m_dLossRate;
	UINT	m_uLossRate_PerOrWon;			// 손절매 % or 원

	//목표수익
	BOOL	m_bUseTargetBenefit;
	double	m_dTargetBenefit;
	UINT	m_uTargetBenefit_PerOrWon;		// 목표수익 % or 원

	//당일 고가대비 하락
	BOOL	m_bUseHPriceCPBear;
	double	m_dHPriceCPBear;
	UINT	m_uHPriceCPBear_PerOrWon;		// 고가대비하락 % or 원

	//당일 저가대비 상승
	BOOL	m_bUseLPriceCPBull;
	double	m_dLPriceCPBull;
	UINT	m_uLPriceCPBull_PerOrWon;		// 저가대비상승 % or 원

	//목표가보존
	BOOL	m_bUseTargetValuePreserve;
	double	m_dTargetValuePreserve;				// 목표가
	UINT	m_uTargetValuePreserve_PerOrWon;		// 목표가보존 % or 원

	//이익보존
	BOOL	m_bUseBenefitPreserve;
	double	m_dBenefitPreserveUp;				// 상승율
	double	m_dBenefitPreserveDn;				// 하락율
	UINT	m_uBenefitPreserveUp_PerOrWon;		// 이익보존(상승율) % or 원
	UINT	m_uBenefitPreserveDn_PerOrWon;		// 이익보존(하락율) % or 원

	//최소가격변화
	BOOL	m_bUseMiniPrice;
	int		m_nBongCount;
	double	m_dMiniPrice;
	UINT	m_uMiniPrice_PerOrWon;			// 최소가격 % or 원

	//당일청산 관련
	BOOL	m_bUseDayLiquidation;
	CString	m_szTimeDayLiquidation;
	UINT	m_uDayLiquidation_PerOrWon;		// 당일청산 % or 원

	// 전략신호정보 배열
	CArray<double, double> m_arrayDealPosition;
	CArray<CString, LPCTSTR> m_arrayDealName;
	CArray<double, double> m_arrayDealDrawPosition;
	CDealData	m_TempDealData;
	CDealData	m_TempCondDealData;
	CDealData	m_TempForcedDealData;

	//////////////////////////////////////////////////////////////////////////
	// vntsorl 추가
	BOOL			m_bBuyDealTraceInit;		// 최초 매수/매수청산 DealTrace 셋팅
	BOOL			m_bSellDealTraceInit;		// 최초 매도/매도청산 DealTrace 셋팅
	CBuyDealTrace	m_buyDealTrace;				// 매수/매수청산 DealTrace 객체
	CSellDealTrace	m_sellDealTrace;			// 매도/매도청산 DealTrace 객체

	BOOL			m_bBuyOrderStartSignal;		// 매수/매수청산 주문시작신호 처리여부(리얼)
	BOOL			m_bSellOrderStartSignal;	// 매도/매도청산 주문시작신호 처리여부(리얼)

	void	InitBuyDealTrace(BOOL bRealState = FALSE, int nInterval = 0);
	void	InitSellDealTrace(BOOL bRealState = FALSE, int nInterval = 0);
	void	ExecuteBuyDealTrace(CDealData* pDealData, BOOL bRealState = FALSE, int nInterval = 0);
	void	ExecuteSellDealTrace(CDealData* pDealData, BOOL bRealState = FALSE, int nInterval = 0);
	void	ExecuteDealTrace();
	void	ExecuteDealTraceReal(CDealData* pDealData, int& nTotalCount, int& nInterval);
	BOOL	ExecuteDealTraceExceptional(CDealData* pDealData, int& nTotalCount, int& nInterval);
	BOOL	ExecuteDealTraceUsual(CDealData* pDealData, int& nTotalCount, int& nInterval);

	void	ConfirmBuyForcedLiquidation(BOOL bRealState = FALSE, int nInterval = 0);
	void	ConfirmSellForcedLiquidation(BOOL bRealState = FALSE, int nInterval = 0);

	void	BuyForcedLiquidation_LossRate(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);				// 손절매로 인한 강제청산
	void	BuyForcedLiquidation_TargetBenefit(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);		// 목표수익으로 인한 강제청산
	void	BuyForcedLiquidation_HPriceCPBear(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);			// 고가대비하락으로 인한 강제청산
	void	BuyForcedLiquidation_TargetValuePreserve(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);	// 목표가보존을 위한 강제청산
	void	BuyForcedLiquidation_BenefitPreserve(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);		// 이익보존을 위한 강제청산 
	void	BuyForcedLiquidation_MiniPrice(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);			// 최소가격으로 인한 강제청산
	void	BuyForcedLiquidation_DayLiquidation(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);		// 당일청산으로 인한 강제청산
	void	BuyForcedLiquidation(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE, int nInterval = 0);

	void	SellForcedLiquidation_LossRate(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);			// 손절매로 인한 강제청산
	void	SellForcedLiquidation_TargetBenefit(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);		// 목표수익으로 인한 강제청산
	void	SellForcedLiquidation_HPriceCPBear(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);		// 고가대비하락으로 인한 강제청산
	void	SellForcedLiquidation_TargetValuePreserve(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);	// 목표가보존을 위한 강제청산
	void	SellForcedLiquidation_BenefitPreserve(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);		// 이익보존을 위한 강제청산 
	void	SellForcedLiquidation_MiniPrice(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);			// 최소가격으로 인한 강제청산
	void	SellForcedLiquidation_DayLiquidation(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE);		// 당일청산으로 인한 강제청산
	void	SellForcedLiquidation(CForcedLiquidationInfo& FLInfo, BOOL bRealState = FALSE, int nInterval = 0);
	//////////////////////////////////////////////////////////////////////////

	void	InitStrategySignalInfo();																// 전략설정 초기화
	BOOL	IsFirstLiquidation(CDealData* pEntryDealData, BOOL& bExist_ForcedLiquidation);			// 청산신호가 맨처음인가?
	BOOL	IsLastEntry(CDealData* pEntryDealData, BOOL& bExist_ForcedLiquidation, POSITION& posLast);	// 진입신호가 마지막인가?
	BOOL	IsLastEntry_ForReal(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation);
	void	BuyOrExitShort(CDealData* pEntryDealData, BOOL& bExist_ForcedLiquidation);				// 매수 / 매도청산
	void	SellOrExitLong(CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation);				// 매도 / 매수청산
	void	SellOrExitLong_ForReal(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation);
	void	Confirm_ForcedLiquidation(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation);			// 강제청산 옵션확인
	void	Confirm_ForcedLiquidation_ForReal(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation);	// 강제청산 옵션확인

	BOOL	IsBuy(long lDealType);						// 매수 사용여부
	BOOL	IsExitShort(long lDealType);				// 매도청산 사용여부
	BOOL	IsSell(long lDealType);						// 매도 사용여부
	BOOL	IsExitLong(long lDealType);					// 매수청산 사용여부
	BOOL	IsBuyOrExitShort(long lDealType);			// 매수 / 매도청산 사용여부
	BOOL	IsSellOrExitLong(long lDealType);			// 매도 / 매수청산 사용여부

	BOOL	IsLossRate();								// 손절매 사용여부
	BOOL	IsTargetBenefit();							// 목표수익 사용여부
	BOOL	IsHPriceCPBear();							// 고가대비하락 사용여부
	BOOL	IsLPriceCPBull();							// 저가대비상승 사용여부
	BOOL	IsBenefitPreserve();						// 이익보존 사용여부
	BOOL	IsTargetValuePreserve();					// 목표가보존 사용여부
	BOOL	IsMiniPrice();								// 최소가격 사용여부
	BOOL	IsDayLiquidation();							// 당일청산 사용여부
	BOOL	IsForcedLiquidation(long lDealValue);		//강제청산 여부

	void	ForcedLiquidation_ForLossRate(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo);				// 손절매로 인한 강제청산
	void	ForcedLiquidation_ForTargetBenefit(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo);		// 목표수익으로 인한 강제청산
	void	ForcedLiquidation_ForHPriceCPBear(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo);			// 고가대비하락으로 인한 강제청산
//	void	ForcedLiquidation_ForLPriceCPBull(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo);			// 저가대비상승으로 인한 강제청산
	void	ForcedLiquidation_ForTargetValuePreserve(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo);	// 목표가보존을 위한 강제청산
	void	ForcedLiquidation_ForBenefitPreserve(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo);		// 이익보존을 위한 강제청산 
	void	ForcedLiquidation_ForMiniPrice(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo);			// 최소가격으로 인한 강제청산
	void	ForcedLiquidation_ForDayLiquidation(CDealData* pEntryDealData, CDealData* pExitDealData, BOOL& bExist_ForcedLiquidation, CForcedLiquidationInfo& FLInfo);		// 당일청산으로 인한 강제청산
	void	ForcedLiquidation(CDealData* pExitDealData, CForcedLiquidationInfo& FLInfo);
	void	ForcedLiquidation_ForReal(CDealData* pExitDealData, CForcedLiquidationInfo& FLInfo);

	void	ResizeSignalInfoArray(int nInterval, int nTotalSignalCnt);		// 신호정보 관련 Array 사이즈 조정 (실시간 업데이트시 발생)
	double	ApplySlipAndTex(double dMulData, BOOL bType,double nNum);

	// 운용 관련변수 및 함수
	int		m_nUseRepeatEntry;				// 중복진입 여부
	int		m_nRepeatEntryCount;			// 중복진입 허용횟수
	int		m_nRepeatCount;					// 중복진입 횟수추적

	// 주문 관련변수 및 함수
	int		m_nOrderAmount;					// 주문수량
	int		m_nTiming_Order;				// 주문시점 (Real인 경우만 사용)
	int		m_nOrderStartSignal;			// 주문시작신호
	BOOL	m_bCheckPrev_EntryPosition;		// 마지막 신호 DealValue
	BOOL	m_bSetComplete_OrderInfo;		// 주문정보 셋팅완료 (조건 만족시 주문신호 발생을 위해 주문정보 셋팅 관련)
	BOOL	m_bCanRemove_OrderInfo;			// 주문정보 제거가능 (조건 만족시 주문신호 발생후 주문정보 제거 관련 셋팅)

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStrategyWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	CStrategyWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStrategyWnd)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	LRESULT OnSTDataManagerMessage( WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
		
//	CCriticalSection m_CriticalSection;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRATEGYWND_H__44B9B7B2_99DB_4966_95EC_AC22B1A93F42__INCLUDED_)
