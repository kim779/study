// IStrategyItem.h: interface for the CIStrategyItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISTRATEGYITEM_H__5013587A_E05B_4CCC_88A3_9BC73BDDD09C__INCLUDED_)
#define AFX_ISTRATEGYITEM_H__5013587A_E05B_4CCC_88A3_9BC73BDDD09C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CIStrategyItem CIStrategyItem0001

#define PROFITRATEACCMULATED	0	// 누적수익률
#define PROFITRATEAVG			1	// 건별평균수익률
#define PROFITRATECURRENT		2	// 현재포지션수익률
#define PROFITLOSSALL			3	// 총손익
#define PROFITALL				4	// 총이익
#define LOSSALL					5	// 총손실
#define PROFITLOSSRATEALL		6	// 총손익/총손실
#define MAXPROFIT				7	// 건별최대이익
#define MAXLOSS					8	// 건별최대손실
#define PROFITLOSSAVG			9	// 건별평균손익
#define PROFITAVG				10	// 건별평균이익
#define LOSSAVG					11	// 건별평균손실

#define AMOUNTENTEREDALL		12	// 총진입금액
#define AMOUNTEXITALL			13	// 총청산

#define CNTALL					15	// 총매매수
#define CNTPROFIT				16	// 이익매매수
#define CNTLOSS					17	// 손실매매수

#define RATEPROFITLOSS			18	// 이익수/손실수
#define RATEPROFIT				19	// 이익매매율
#define RATELOSS				20	// 손실매매율

#define CHANGERATE				21	// 기간등락율
#define MAXPROFITSTRAIGHT		22	// 최대이익연속수 
#define MAXLOSSSTRAIGHT			23	// 최대연속손실수
#define RATEPROFITEXCEED		24	// 전략초과수익률
#define PROFITSTRAIGHT			25	// 최대연속이익
#define LOSSSTRAIGHT			26	// 최대연속손실
#define CNTCANDLE				27	// 총봉수
#define CNTCANDLEPROFIT			28	// 이익매매봉수
#define CNTCANDLELOSS			29	// 손실매매봉수
#define RATEACCESS				30	// 기간참여율
#define CNTCANDLEACCESS			31	// 진입상태봉수
#define CNTCANDLEEXIT			32	// 청산상태봉수
#define CNTCANDLEAVGDEAL		33	// 평균매매봉수
#define CNTCANDLEAVGPROFIT		34	// 이익매매평균봉수
#define CNTCANDLEAVGLOSS		35	// 손실매매평균봉수


class CStrategyResult  
{
public:
	CStrategyResult() 
	{
		m_DealType = NONE;
		m_dTime = 0;
		m_dPrice = 0;
		m_lQty = 0;
		m_dAmount = 0;
		m_dEnteredAmount = 0;
		m_dProfit = 0;
		m_dProfitAccmulated = 0;
		m_dProfitRate = 0;
		m_dProfitRateAccmulated = 0;
		m_lCntCandleInAccess = 0;
		m_bOnlyValuate = FALSE;
	}
	enum DEALTYPE {
		NONE,
		BUY,
		EXITSHORT,
		SELL,
		EXITLONG
	};
	
	BOOL	m_bOnlyValuate;
	DEALTYPE m_DealType;			// 거래형태
	double	m_dTime;				// 시간
	double  m_dPrice;				// 가격
	long m_lQty;					// 수량
	double m_dAmount;				// 진입/청산금액

	// Calculated By CStrategyWnd::CalaulateStrategyResultData()
	long m_lCntCandleInAccess;		// 봉수
	double m_dEnteredAmount;		// 누적진입금액
	double m_dProfit;				// 건별수익
	double m_dProfitAccmulated;		// 누적수익
	double m_dProfitRate;			// 손익률
	double m_dProfitRateAccmulated;	// 누적수익률
};

enum OPERATTIONSTATUS {
	STRATEGY_NONE = 0,
	STRATEGY_RUN,
	STRATEGY_PAUSE,
	STRATEGY_STOP,
	STRATEGY_STANDBY
};




class CIStrategyItem0001  
{
public:
	
	virtual ~CIStrategyItem() {};
	// 20060705 함수인자 제거 : HWND hSocket,/*long pDBMgr,*/
	//virtual int Create(CWnd* pParentWnd,HWND hSocket,/*long pDBMgr,*/ long lKey, CStringArray &rStArrayPath) = 0;
	virtual int Create(CWnd* pParentWnd, long lKey, CStringArray &rStArrayPath,long pChartItem) = 0;

	virtual BOOL DestoryWindow() = 0;
	virtual HWND GetSafeHwnd( ) const = 0;
	// atcion command
	virtual BOOL LoadSD(LPCTSTR lpStrategyDllPath,LPCTSTR lpInputCode,long lDataCnt,LPCTSTR lpEndDate, LPCTSTR lpTimeTypeNum,LPCTSTR lpTypeDealOption = NULL) = 0;	
	virtual BOOL LoadST(LPCTSTR lpStrategyPath,LPCTSTR lpInputCode,long lDataCnt,LPCTSTR lpEndDate) = 0;	
	virtual BOOL Run(LPCTSTR lpOption = NULL) = 0;
	virtual BOOL Stop() = 0;
	virtual BOOL Pause() = 0;
	virtual long Update(long lPos, BOOL bIsUpdateData) = 0;		//@060922전략Real관련수정
	virtual CString GetTime() = 0;
	virtual long GetCount() = 0;
	virtual CString GetInputCode() = 0;
	virtual CString GetEndDate() = 0;
	virtual long GetDataCount(long lIndex) = 0;
	virtual CString GetTime(long lIndex) = 0;
	virtual long GetPtrDrawInfo(long lIndex = 999) = 0;
	virtual BOOL CalculateStatisticalInfo(long lStartPos= -1, long lEndPos= -1) = 0;
	virtual long GetCntStrategyResult() = 0;
	virtual CStrategyResult* GetStrategyResult(long lIndex) = 0;
	virtual double GetStatisticalInfo(long lPos) = 0;
	virtual LPCTSTR GetErrorMessage() = 0;
	virtual BOOL HasAPData() = 0;
	virtual OPERATTIONSTATUS GetOperationStatus() = 0;
	virtual CString GetInputs(long lIndexST, long lIndexInput) = 0;
	virtual void GetCurrentPositonNProfit(long& rLCurrentDealPosition, double &rDCurrentProfitRate) = 0;
	virtual CString GetName(long lIndex) = 0; 
	virtual CString GetPath(long lIndex) = 0;
	virtual CString GetProfileInfo(LPCTSTR lpAppName,LPCTSTR lpKeyName) = 0;
	virtual	BOOL SetUpperLowerPrice(double dUpperPrice,double dLowerPrice) = 0;
	virtual BOOL SetVolumeForCalculatation(double dVol) = 0;
	virtual double GetVolumeInDealData() = 0;

	//KHD : 시간최적화 봉의 갯수
	virtual int GetTimeBong() = 0;
	virtual void SetTimeBong(int nBong) = 0;
	//KHD : 변수최적화 변수
	virtual void SetValOptimizer(LONG Val)=0;
	virtual LONG GetValOptimizer() =0;
	virtual CString	 GetSendValString()=0;
	virtual void SetSendValString(CString)=0;
	// atcion command
	virtual void OnSTSetRealDataST()=0;
	
	// 차트 시간구분 정보 셋팅
	virtual void SetChartDivideInfo(int nDivideState, int nUnitPos) = 0;
};

#endif // !defined(AFX_ISTRATEGYITEM_H__5013587A_E05B_4CCC_88A3_9BC73BDDD09C__INCLUDED_)
