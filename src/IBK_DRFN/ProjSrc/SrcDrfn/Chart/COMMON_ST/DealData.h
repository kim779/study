// DealData.h: interface for the CDealData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEALDATA_H__91F3027D_1166_48C9_9BBA_67ABF195A063__INCLUDED_)
#define AFX_DEALDATA_H__91F3027D_1166_48C9_9BBA_67ABF195A063__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDealData  
{
public:
	CDealData();	
	virtual ~CDealData();
	CDealData(const CDealData& rhs);
	
	CDealData operator= (const CDealData& rhs );	// ASSIGN
	CDealData operator& (const CDealData& rhs );	// AND
	CDealData operator| (const CDealData& rhs );	// OR

	void Clear();

	int			m_nIndex;
	long		m_lDealValue;
	long		m_lType;
	double		m_dPrice;
	double		m_dContract;
	CString		m_strDealName;
	CStringList m_strListEntryNameForExit;
};

class CDealTraceBase
{
public:
	CDealTraceBase();	
	virtual ~CDealTraceBase();

public:
	BOOL	IsNonePosition();						// 신호 없음
	BOOL	IsNonePosition(long lDealValue);		// 신호 없음
	BOOL	IsBuy(long lDealValue);					// 매수 신호
	BOOL	IsExitShort(long lDealValue);			// 매도청산 신호
	BOOL	IsSell(long lDealValue);				// 매도 신호
	BOOL	IsExitLong(long lDealValue);			// 매수청산 신호
//	BOOL	IsForcedLiquidation(long lDealValue);	// 강제청산 신호

	BOOL	IsClearStateOfRaw();					// 초기화 상태(Raw)인가?
	BOOL	IsClearStateOfCurr();					// 초기화 상태(Curr)인가?
	BOOL	IsClearStateOfAll();					// 초기화 상태(All)인가?

	BOOL	IsSameDealData(const CDealData* pDealData);

	int		GetRawIndex();
	long	GetRawDealValue();
	long	GetRawType();
	CString	GetRawDealName();
	double	GetRawPrice();
	double	GetRawContract();
	CStringList*	GetRawEntryNameForExit();

	int		GetCurrIndex();
	long	GetCurrDealValue();
	long	GetCurrType();
	CString	GetCurrDealName();
	double	GetCurrPrice();
	double	GetCurrContract();
	CStringList*	GetCurrEntryNameForExit();

	int		GetOrdrIndex();
	long	GetOrdrDealValue();
	long	GetOrdrType();
	CString	GetOrdrDealName();
	double	GetOrdrPrice();
	double	GetOrdrContract();
	CStringList*	GetOrdrEntryNameForExit();

//	void	InitRepeatCount();
//	void	SetForcedLiquidation(int nIndex, long lDealValue, BOOL bRealState = FALSE);

	void	InitDealData();
	void	SetRepeatInfo(const int nUseRepeatEntry = 0, const int nRepeatEntryCount = 1);
//	BOOL	IncreaseRepeatCount();
	void	SetTimingInfo(int nTiming_Order = 1, int nTiming_ForcedLiquidation = 1);
	int		GetTimingOrder();
	int		GetTimingForcedLiquidation();

	BOOL	Set(const CDealData* pDealData);
	void	Copy();
	void	Next();

	CDealData	m_DealDataRaw;						// 실시간 Raw DealData
	CDealData	m_DealDataCurr;						// 주문 및 강제청산 DealData (조건만족시)
	CDealData	m_DealDataOrdr;						// 주문 및 강제청산 DealData (봉완성시)

public:
	virtual BOOL	IsEntryPosition();
	virtual BOOL	IsExitPosition();

protected:
	int		m_nUseRepeatEntry;						// 중복진입 허용여부
	int		m_nRepeatEntryCount;					// 중복진입 횟수
//	int		m_nRepeatCount;							// 중복진입 Trace

//	BOOL	LessThanRepeatEntryCount();

	int		m_nTiming_Order;						// 주문시점 (0 : 조건만족시, 1 : 봉완성시)
	int		m_nTiming_ForcedLiquidation;			// 강제청산시점 (0 : 조건만족시, 1 : 봉완성시)
};

class CBuyDealTrace : public CDealTraceBase
{
public:
	CBuyDealTrace();	
	virtual ~CBuyDealTrace();

public:
	BOOL	IsEntryPosition(long lDealValue);
	BOOL	IsExitPosition(long lDealValue);
	BOOL	IsForcedLiquidation(long lDealType);
	void	SetForcedLiquidation(int nIndex, long lDealValue, BOOL bRealState = FALSE);

	void	InitRepeatCount();
	BOOL	IncreaseRepeatCount();
	BOOL	LessThanRepeatEntryCount();

	BOOL	Init(CDealData* pDealData);
	BOOL	Set(const CDealData* pDealData, BOOL bRealState = FALSE);
	void	Next(BOOL bRealState = FALSE);

private:
	int		m_nRepeatCount;							// 중복진입 Trace

public:
	virtual BOOL	IsEntryPosition();
	virtual BOOL	IsExitPosition();
};

class CSellDealTrace : public CDealTraceBase
{
public:
	CSellDealTrace();	
	virtual ~CSellDealTrace();

public:
	BOOL	IsEntryPosition(long lDealValue);
	BOOL	IsExitPosition(long lDealValue);
	BOOL	IsForcedLiquidation(long lDealType);
	void	SetForcedLiquidation(int nIndex, long lDealValue, BOOL bRealState = FALSE);

	void	InitRepeatCount();
	BOOL	IncreaseRepeatCount();
	BOOL	LessThanRepeatEntryCount();

	BOOL	Init(CDealData* pDealData);
	BOOL	Set(const CDealData* pDealData, BOOL bRealState = FALSE);
	void	Next(BOOL bRealState = FALSE);

private:
	int		m_nRepeatCount;							// 중복진입 Trace

public:
	virtual BOOL	IsEntryPosition();
	virtual BOOL	IsExitPosition();
};

#endif // !defined(AFX_DEALDATA_H__91F3027D_1166_48C9_9BBA_67ABF195A063__INCLUDED_)
