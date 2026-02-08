// IndicatorSingleton.h: interface for the CIndicatorFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDICATORFACTORY_H__1F4A506F_3636_46B2_844F_B049AA6D1A3A__INCLUDED_)
#define AFX_INDICATORFACTORY_H__1F4A506F_3636_46B2_844F_B049AA6D1A3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Indicator.h"

class CIndicatorSingleton  
{
public:
	CIndicator* Create( IMetaTable::EDDC_INDICATOR_ID p_eID, HWND p_hOcxHwnd);

// (2006/9/6 - Seung-Won, Bae) Check ST Index Indicator
public:
	CIndicator *CreateSTIndexCalculator( IMetaTable::EDDC_INDICATOR_ID p_eID);
};



// 종찬(05/08/03) 날코딩!!!
class CSystemTradingCalculate : public CIndicator  
{
public:
	CSystemTradingCalculate( IMetaTable::EDDC_INDICATOR_ID p_eID) : CIndicator( p_eID)	{}

public:
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);
private:
	void SetGraphPacketList(const CChartFormulateData& chartFormulateData,
		CGraphCalculateData* pGraphCalculateData, CIndicatorInfo*& pIndicatorInfo, CCalculateData& calculateData);
	void SetSingalPacket(const CChartFormulateData& chartFormulateData, CGraphCalculateData* pGraphCalculateData, CIndicatorInfo* pIndicatorInfo);

	void SetGraphPacketList(CPacketList* pPacketList, CGraphCalculateData* pGraphCalculateData, CIndicatorInfo* pIndicatorInfo);
// vntsorl:[0003431] [7111]차트화면에서의 조회속도가 구차트에 비해 늦어 개선 요구
// 	void SetMAPacketList(CPacketList* pPacketList, CGraphCalculateData* pGraphCalculateData, CIndicatorInfo* pIndicatorInfo, CCalculateData& calculateData);
// (2007/1/25 - Seung-Won, Bae) Use High/Low Packet directly on Draw Graph.
//	void SetPacketList_Parabolic(CPacketList* pPacketList, CGraphCalculateData* pGraphCalculateData, CIndicatorInfo* pIndicatorInfo);
//	bool CParabolic_SetGraphPacketList(CPacketList* pPacketList, CGraphCalculateData* pGraphCalculateData, CIndicatorInfo*& pIndicatorInfo, CPacket*& pPacket, CPacket*& pPacket1, CPacket*& pPacket2);
//	bool CParabolic_InCaseNoPacketPointer(CPacketList* pPacketList, CIndicatorInfo*& pIndicatorInfo, CPacket*& pPacket, CPacket*& pPacket1, CPacket*& pPacket2, CSubGraphPacketList* pSubGraphList);
};

//거래내역
class CAccount : public CIndicator  
{
public:
	CAccount();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

private:
	bool IsCurrentDateCloserthanPreDate(const int nPredate, const int nCurrentdate, const int nSellbuyDate);
	
	void GetPacketDateType(CPacket* pDate, CPacket* pAccountDate, CString& strgraphdate, CString& strsellbuydate, CString& strcomparedate);
	void GetRealDate(const CString& stroriginaldatetype, const CString& strdatetype, const CList<double, double>& dDateList, CList<double, double>& dRealDateList);

	void GetSellandBuyList(const CPoint& point, const CList<double, double>& dRealGraphDateList, const CList<double, double>& dRealSellBuyDateList, CList<double, double>* pSellBuyList, CPacket* pBuy, CPacket* pSell);
	bool CompareDate(const bool bLast, const CString strPreDate, const CString strCurrentDate, const CString strSellBuyDate, const int ngraph, const int nsellbuy, double& nsell, double& nbuy, double* nSell, double* nBuy, CList<double, double>* pSellBuyList);

	CString GetCompareDateType(const CString& strgraphdate, const CString& strsellbuydate);
	CString WhenHHMMSSNN(const CString& strGraphDateType);
	CString WhenHHMMSS(const CString& strGraphDateType);
	CString WhenDDHHMMSS(const CString& strGraphDateType);
	CString WhenDDHHMM(const CString& strGraphDateType);
	CString WhenDDHH(const CString& strGraphDateType);
	CString WhenMMDD(const CString& strGraphDateType);
	CString WhenYYMM(const CString& strGraphDateType);
	CString WhenYYYYMM(const CString& strGraphDateType);
	CString WhenYYMMDD(const CString& strGraphDateType);
	CString WhenYYYYMMDD(const CString& strGraphDateType);

	CList<double, double>* m_pDateList;
	CList<double, double>* m_pHighPriceList;
	CList<double, double>* m_pLowPriceList;
	CList<double, double>* m_pAccountList;
	CList<double, double>* m_pAccountDateList;
	CPacket* m_pDatePacket;
	CPacket* m_pAccountDatePacket;
};

//BigNews
class CBigNews : public CIndicator  
{
public:
	CBigNews();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

private:
	bool IsValidBigNews(CList<double, double>* pGraphDateList,
					    CList<double, double>* pBigNewsDateList,
					    CPacket* pBigNews);
	void GetBigNews(const CPoint& point, CList<double, double>* pGraphDateList, CList<CString, CString>* pBigNewsList, CList<double, double>* pBigNewsDateList, CPacket* pBigNews);

	CList<double, double>* m_pDateList;
	CList<CString, CString>* m_pBignewsDataList;
	CList<double, double>*m_pNewsDateList;
};

class CBoxChart : public CIndicator  
{
public:
	CBoxChart();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
								   CGraphCalculateData* pGraphCalculateData,
								   CIndicatorInfo* pIndicatorInfo);

private:
	bool ClearPacket( const CCalculateBaseData::CALCULATETYPE calculatetype, 
		             CPacketList* pPacketList, CSubGraphPacketList* pSubGraphPacketList, CIndicatorInfo*& pIndicatorInfo);
	bool SetGraphPacketList(const CCalculateBaseData::CALCULATETYPE calculatetype,
							CPacketList* pPacketList,
							CGraphCalculateData* pGraphCalculateData,
							CIndicatorInfo*& pIndicatorInfo);

	// (2004.10.20, 배승원) Box Chart 구성시 시종 Packet을 별도의 Routine에서 산출하지 않는다.
	void GetForwardBoxChart(const CPoint& point, const double dCondition,
							CList<double, double>* pTopPriceList,
							CList<double, double>* pBottomPriceList,
							CList<double, double>* pStartPriceList,
							CList<double, double>* pEndPriceList);
	void GetBackwardBoxChart(const CPoint& point, const double dCondition,
							CList<double, double>* pTopPriceList,
							CList<double, double>* pBottomPriceList,
							CList<double, double>* pStartPriceList,
							CList<double, double>* pEndPriceList);

private:
	// (2004.10.20, 배승원) Box Chart를 구성하는 Packet를 4개로 정리한다.
	CPacket* m_pBoxStartPacket;
	CPacket* m_pBoxHighPacket;
	CPacket* m_pBoxLowPacket;
	CPacket* m_pBoxEndPacket;

	CList<double, double>* m_pStartPriceList;
	CList<double, double>* m_pHighPriceList;
	CList<double, double>* m_pLowPriceList;
	CList<double, double>* m_pEndPriceList;
};

class CFourierTransform : public CIndicator  
{
public:
	CFourierTransform();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

};


///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2004.02.05
//
//	분석	:	선물 전용 P&F Chart (옵션 아님. 선물전용 호가 단위 기준으로 Hard Codeing 되어 있음.)
//				1. 기본사항
//					1. 선물 전용 P&F는 t분봉 종가 혹은 Tick의 Price를 기준으로 n호가 단위의 전환도이다.
//						(삼선 전환도와 유사)
//					2. 상승은 'X', 하락은 'O'표를 하여 MarkLine을 구성한다.
//					3. 등락의 방향이 이전 고/저 가격의 n호가 단위 반전되어야만 방향 전환을 인정하고,
//						전환 깃점 다음부터의 Marking을 처리하게 된다.
//					4. Mark는 항상 호가단위 (0.05)마다 표시한다.
//					5. 일자가 변경되는 경우, 해당 Mark부터 색상을 변경한다. (적색과 청색을 반복하며 전환한다.)
//				2. 개발 정보
//					1. TR에 의한 조회시에는 Server에서 이미 P&F Chart를 그리기 위한 Data만을 집계하여 내려준다.
//						(각 Packet은 하나의 등락 MarkLine이다.)
//							자료일자	: 해당 MarkLine에 등록된 마지막 가격 Data의 시각이다.
//										  P&F Chart에는 출력되지 않지만, 이후 Real의 수신시에 일자 변경의 기준이 된다.
//							시가		: MarkLine의 시작 가격이다.
//							종가		: MarkLine의 끝 가격이다.
//										  MarkLine의 시종 가격에 의한 등락여부로 상승의 경우 'X', 하락의 경우 'O'표시한다.
//							새일자가	: 하나의 MarkLine내에서 일자가 변경되는 경우에 색상을 변경할 Mark의 가격이다.
//					2. Server에서 내려주는 Packet에는 특수한 경우를 처리하기 위한 보조 Packet이 내려온다.
//						1. 제1 기준 Packet (기준종가 Packet)	: 모든 Packet Data에 앞서 항상 들어오는 첫번째 기본 Packet이다.
//								자료일자	: 금일(조회당일)의 장시작 시간이다.
//												금일이 영업일이 아닌 경우, (Real이 없겠지?) 0으로 들어온다.
//												이 값은 조회당일, 장시작전, 분봉으로, 당일 Data만을 조회하는 경우에,
//													첫 봉을 구성하기 위한 시각으로 1분봉의 경우 1분정각의 Data '이전'까지 포함하여 봉이 형성된다.
//								시가		: 자료조회 시작일의 '전일종가'
//												Server에서 형성된 (정/소)추세가 없는 경우, 화면에 출력할 기준가이다.
//												소추세 형성시 초기 시/종가로 이용된다.
//								종가		: 이용않음
//								새일자가	: 이용않음
//						2. 제 2기준 Packet (끝봉 Packet)	: 모든 Packet Data에 앞서 항상 들어오는 두번째 기본 Packet이다.
//												끝봉은 이후의 Real 수신에 의하여 완성되는 미완성 봉으로 간주하여 별도의 Packet으로 받는다.
//												즉 아래의 소추세나 추세 Packet에 가산되지 않고 별도로 수신하여 가추세 Packet으로 구성하게 된다.
//								자료일자	: 끝봉의 시작시간이다.
//								시가		: 이용않음
//								종가		: 끝봉의 종가이다.
//								새일자가	: 이용않음
//						2. 소추세 Packet Data	: Server에 조회된 Data가 있으나,
//											  Server에서 구성된 추세가 하나도 없는 경우 내려오는, 추세 추적값 Packet이다.
//								[소추세]	: 보통 추세는 이전 추세의 마지막 추세치를 기준으로 반전을 확인하게되는데,
//													첫 추세는 이전 추세가 없는 관계로 상승/하락의 추세를 동시에 추적하면서,
//													그 폭이 지정 호가를 넘어서는 시점에 추세로 인정하게 된다.
//												추세 추적중의 고/저가를 갱신시에는 마지막으로 갱신된 것이 종가로 나타나도록 한다.
//												특히 소추세는 Drawing Packet에는 등록되지 않는다. 단지 가추세에 반영되어 보인다.
//								자료일자	: 추세 추적에 감안된 마지막 Data의 시각이다. (추세의 일자에 이용할 수 있으나 이용하지 않고있다.)
//								시가		: 추세 추적 시가로, 소추세를 구성하는 동안 발생한 최저가 혹은 최고가이다.
//								종가		: 추세 추적 종가로, 소추세를 구성하는 동안 발생한 최저가 혹은 최고가이다.
//								새일자가	: 추세 추적중에 발생한 새일자가로, 마지막 영업일의 시가가 등록되어 있다.
//												추적중에 발생한 일일 시가가 없는 경우는 0으로 설정된다.
//												특히 조회시작일의 일일 시가는 이전 추세가 형성되지 않으므로 역시 0으로 설정한다.
//						3. 추세 Packet Data	: Server에 구성된 추세 Packet이다.
//								자료일자	: 추세에 포함된 마지막 Data의 시각이다.
//												(즉 새로운 추세를 형성한 Data의, 바로 이전 Data 시각이다.
//												 추세의 일자에 이용할 수 있으나 이용하지 않고있다.)
//								시가		: 추세의 시작 가격이다. 이전 추세의 종가보다 0.05가 크거나 (상승의 경우) 작다 (하락의 경우)
//								종가		: 추세의 끝 가격이다. 시가와 종가로 상승추세인지 하락추세인지 식별한다.
//								새일자가	: 추세 진행중에 발생한 새일자가로, 추적중 일자가 변경되는 경우의 마지막 일일 시가가 등록된다.
//												추세 진행중에 발생한 일일 시가가 없는 경우는 0으로 설정된다.
//												특히 조회시작일의 일일 시가는 이전 추세가 형성되지 않으므로 역시 0으로 설정한다.
//					3. 출력용 보조 Packet
//						1. Scale Packet		: Drawing 처리시 Y Scale이 가추세에 의해 급변하는 경우를 제한하기 위하여,
//												Drawing된 값의 Min/Max Data를 관리하여, 지속적인 축소만이 가능하도록 지원하는 Packet이다.
//											  Drawing 처리시의 일관적으로 처리가 가능하도록 항상 맨 처음 추가한다.
//								자료일자	: 자료구조 없음
//								시가		: 전체 출력 Data의 최소값
//								종가		: 전체 출력 Data의 최대값
//								새일자가	: 이용 없음
//						2. 가추세 Packet Data	: Server에서 내려주는 Packet이 아닌, Real의 분봉 처리시 Tic에 의해 형성되는 추세 Packet이다.
//								[가추세]	: 가추세는 분봉의 최종 종가가 확정되지 않는 상황에서의 임시 추세를 나타내는 Packet으로,
//													Server가 아닌, 지표 산출시 제 2기준 Packet을 바탕으로 생성되며,
//													Tic에 의한 추세 반전이나 확장을 나타내는 Packet으로 항상 등록된다.
//											  가추세는 지표 출력 Routine의 일관성을 위하여, Real 뿐만 아니라 TR의 경우에도 추가한다.
//											  또한 가추세는 출력시 그 추세폭을 확인하여 반전폭이 넘어서는 경우에만 반전으로,
//													그리고, 추세확장인 경우에만 확장으로 출력처리한다.
//								자료일자	: 자료구조 없음
//								시가		: Tic에 의해 임시로 생성된 추세의 시가이다.
//								종가		: Tic에 의해 임시로 생성된 추세의 종가이다.
//								새일자가	: 가추세를 형성하는 끝봉이 이전추세와 날짜가 다른 경우 갖는 가격으로 추세의 종가이다.
//					4. Real 처리
//						1. 소추세 형성	:
//								1. Server에서 추세 Packet Data가 없이, 기준 Packet만, 혹은 소추세 Packet과 함께
//										내려오는 경우이다.
//								2. 소추세는 지표처리시 Packet으로 관리하지 않고, 최종 상황 변수로만 관리한다.
//								3. 각 분봉의 종가만을 취하여 소추세를 확장하여 나간다. (추세가 확정될때까지)
//								4. 기준 Packet의 시가(조회시작일 전일 종가)를 시종으로 설정한 가추세 Packet만을 등록한다.
//								5. 추세가 형성될 경우, 새로운 추세 Packet으로 등록시킨다.
//						2. 추세 진행	:
//								1. Server에서 추세 Packet Data를 내려주거나, Real에 의해 추세가 확정된 경우이다.
//								2. 각 Real Data를 적용하여 추세의 확장 또는 반전의 가추세 Packet을 등록한다.
//						3. Scale Packet 관리	:
//								1. 소추세/정추세/가추세를 포함한 모든 추세 Data들의 Drawing을 위하여
//										추세 Data들의 Min/Max를 Packet으로 관리한다.
//								2. 첫 지표 Packet의 시/종 가로 계속 Update한다.
//						4. 가추세 Packet 관리	:
//								1. 분봉 추세추적시 Real에의한 마지막 미완성 분봉의 추세를 관리하는 Packet으로,
//										지표 Packet의 마지막에 항상 등록시킨다.
///////////////////////////////////////////////////////////////////////////////
class CFuOpPF : public CIndicator
{
public:
	CFuOpPF();

// (2004.02.05, 배승원, 분석) Input Packet을 관리한다.
protected:
	CPacket *	m_pIMLDatePacket;			// 자료일자 Packet
	CPacket *	m_pIMLStartPricePacket;		// MarkLine 시가 Packet
	CPacket *	m_pIMLEndPricePacket;		// MarkLine 종가 Packet (Real 채결가 Packet)
	CPacket *	m_pIMLNewDatePricePacket;	// 새일자가 Packet

// (2004.02.05, 배승원, 분석) Output Packet을 관리한다.
//		(출력시에는 Date Packet을 필요로 하지 않는다.
//		 각 Mark마다 시간을 명시하지 않고, MarkLine 단위의 시간은 의미가 없다.)
protected:
	CPacket *	m_pOMLStartPricePacket;	// MarkLine 시가 Packet
	CPacket *	m_pOMLEndPricePacket;	// MarkLine 종가 Packet
	CPacket *	m_pOMLNewDatePricePacket;	// 새일자가 Packet

// (2004.02.05, 배승원, 분석) 선물 P&F Chart의 지표를 구성하는 기본 Routine이다.
protected:
	virtual void	GetData( const CChartFormulateData &chartFormulateData,
						CGraphCalculateData *pGraphCalculateData, CIndicatorInfo *&pIndicatorInfo,
						CCalculateData &calculateData);

// (2004.02.05, 배승원, 분석) 선물 P&F Chart의 TR & Real을 처리하는 Routine을 제공한다.
protected:
	void	GetTRFuOpPF( void);
	void	GetRealFuOpPF(	const CPoint &point);

// (2004.02.24, 배승원) P&F 지표를 산출하기 위한 조건값을 구하여 관리한다.
protected:
	int		m_nStickTimeWidth;		// Real 취합 시간 (분단위, 0은 Tick)
	int		m_nNewTrendPriceX100;	// 추세반전 호가단위

// (2004.02.24, 배승원) Real을 처리하기 위한 최종 상태를 관리한다.
protected:
	int		m_nLastStickTime;		// 끝봉의 시작시간 (이것을 기준으로 Real을 봉으로 취합한다.)

// (2004.02.24, 배승원) 소추세 추적 변수를 관리한다.
protected:
	int		m_nMiniTrendTime;				// 소추세의 마지막 시간이다. (이것으로 소추세 추적 상태을 나타낸다.)
											//		0  <	소추세를 추적중인 상태
											//		0  =	이미 정추세가 형성된 상태
											//		-1		아직 소추세가 형성되지 않은 상태 (끝봉에 의하여 가추세 형성 상태)
											//		-2		아직 소추세가 형성되지 않은 상태 (끝봉에 의한 가추세가 형성되지 않은 상태)
											//		-3		오류 상태
											//	특히, Server의 Packet에 문제가 있는 경우를 감안하여
											//		Server Packet을 확인하기에 앞서, 소추세 추적 변수를 초기화한다.
											//	Server Packet의 오류시에는 정추세가 이미 있는 것처럼 설정하여,
											//		Real에 의한 소추세 추적이 진행되지 않도록 한다.
	int		m_nMiniTrendStartX100;			// 소추세의 시가이다.
	int		m_nMiniTrendEndX100;			// 소추세의 종가이다.
	int		m_nMiniTrendNewDatePriceX100;	// 소추세의 새일자가이다.
	int		m_nFirstBasePriceX100;			// 첫 소추세를 Real로 형성시에 참조할 조회 전일 종가를 관리한다.

// (2004.03.01, 배승원) 마지막 정추세의 시각을 관리한다. (가추세의 새일자가 산출시 참조한다.)
protected:
	int		m_nFixedTrendTime;

// (2004.03.05, 배승원) 끝봉 시각을 추적하면서 Real을 취합하는 것은 당일 끝봉인 경우뿐이다.
//		따라서, 장시작 시각을 별도로 관리하여, 첫 Real 수신시 새일자가를 산출하도록 한다.
//		즉, 장시작시각을 별도로 관리하여, 끝봉이 오늘의 것인 경우, 어제의 것인 경우, 없는 경우를 구분하여 대응할 수 있도록 한다.
protected:
	int		m_nMarketStartTime;
};


class CGlanceBalance : public CIndicator  
{
public:
	CGlanceBalance();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									CGraphCalculateData* pGraphCalculateData,
									CIndicatorInfo* pIndicatorInfo);

private:
	bool SetGraphPacketList(const CCalculateBaseData::CALCULATETYPE calculatetype,
							CPacketList* pPacketList,
							CGraphCalculateData* pGraphCalculateData,
							CIndicatorInfo*& pIndicatorInfo,
							CPacket*& pTuringPoint, CPacket*& pStandardLine,
							CPacket*& pLagSpan);

	bool InCaseNoPacketPointer(const CCalculateBaseData::CALCULATETYPE calculatetype,
							   CPacketList* pPacketList,
							   CIndicatorInfo*& pIndicatorInfo,
							   CGraphCalculateData* pGraphCalculateData,
							   CPacket*& pTuringPoint, CPacket*& pStandardLine,
							   CPacket*& pLagSpan);

	bool GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList,
						   CSubGraphPacketList* pSubGraphList, CIndicatorInfo*& pIndicatorInfo);

	void GetLeadSpan1(const CPoint& point, const int nDataCount, double dCondition, CList<double, double>* pTurningPointLine, CList<double, double>* pStandardLine, CList<double, double>& dLeadSpanList);
	void GetLeadSpan2(const CPoint& point, const double& dSpanCond1, const double& dSpanCond2, CList<double, double>* pTopPriceList, CList<double, double>* pBottomPriceList, CList<double, double>& dLeadSpanList);
	void GetLagSpan(const CPoint& point, double dCondition, CList<double, double>* pEndPriceList, CList<double, double>& dLagSpanList);
	void GetMA(const CPoint& point, double dCondition, CList<double, double>* pTopPriceList, CList<double, double>* pBottomPriceList, CList<double, double>& dResultList);
	void GetStandardLine(const CPoint& point, double dCondition, CList<double, double>* pTopPriceList, CList<double, double>* pBottomPriceList, CList<double, double>& dStandardLineList);
	void GetTurningPointLine(const CPoint& point, double dCondition, CList<double, double>* pTopPriceList, CList<double, double>* pBottomPriceList, CList<double, double>& dTurningPointLineList);

	CPacket* m_pLeadSpan1;
	CPacket* m_pLeadSpan2;
	CList<double, double>* m_pHighPriceList;
	CList<double, double>* m_pLowPriceList;
	CList<double, double>* m_pEndPriceList;
	CPacket* m_pHighPacket;

};


///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2004.02.03
//
//	분석	:	대기 매물 Chart
//					1. 종가와 거래량 Packet을 Input으로하여, 대기매물 가격대 Packet과
//						대기매물 거래량누계 Packet을 산출한다.
//					2. 대기매물 가격대 Packet과 거래량누계 Packet은 고가에서 저가의 가격대 순으로 등록되어 있다.
//					3. 대기매물 가격대 Packet에 등록되는 값은 각 가격대 구간의 하한값이다. (SetPriceZon() 참고)
//					4. 대기매물 거래량누계 Packet을 실제 조건치보다 2개 더 초기화한다.
//						그것은 향후 Drawing 시점에서 참조할 Data로 최종 종가의 가격대 Index와
//						산출 구간내의 가격대별 거래량누계중 최대인 가격대의 Index를 관리한다.
//						Drawing시에 최종 가격대와 최대 가격대의 High Light에 참조된다.
///////////////////////////////////////////////////////////////////////////////
class CHorizontalBar : public CIndicator  
{
public:
	CHorizontalBar();

public:
	virtual void GetData(const CChartFormulateData& chartFormulateData,
					CGraphCalculateData* pGraphCalculateData, CIndicatorInfo*& pIndicatorInfo,
					CCalculateData& calculateData);

// (2004.02.03, 배승원, 분석) 대기매물 지표의 산출에 필요한 Packet을 관리한다.
protected:
	CList<double, double>* m_pEndPriceList;
	CList<double, double>* m_pVolumeList;
	CPacket* m_pHorZonPacket;
	CPacket* m_pHorZonPricePacket;

// (2004.02.03, 배승원, 분석) 대기매물 지표의 산출 구간의 기본값을 관리한다.
// (2006.10.23 안승환) 대기매물 호가별 기능 추가
// (2008/4/23 - Seung-Won, Bae) Make the price unit double
	double	m_dHoga;
// (2006.10.23 안승환) 대기매물 호가별 기능 추가
protected:
	int		m_StartPoint;	// 대기매물 산출 범위의 시작 Index이다.
	int		m_EndPoint;		// 대기매물 산출 범위의 끝 Index이다.
	double	m_Max;			// 대기매물 산출 범위의 최고 종가이다.
	double	m_Min;			// 대기매물 산출 범위의 최저 종가이다.

private:
// (2006.10.23 안승환) 대기매물 호가별 기능 추가
//	void GetHorizontalBar(const CChartFormulateData& chartFormulateData, 
//				const CPoint& point, double dCondition, CList<double, double>* pEndPriceList,
//				CList<double, double>* pVolumeList);
	void GetHorizontalBar(const CChartFormulateData& chartFormulateData, 
				const CPoint& point, double dCondition, CList<double, double>* pEndPriceList,
				CList<double, double>* pVolumeList,double dConditionType, double dHoga);
// (2006.10.23 안승환) 대기매물 호가별 기능 추가
};

class CInverseLine : public CIndicator
{
public:
	CInverseLine();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									  CGraphCalculateData* pGraphCalculateData,
									  CIndicatorInfo* pIndicatorInfo);

private:
	bool SetGraphPacketList(CPacketList* pPacketList,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo*& pIndicatorInfo);

	bool InCaseNoPacketPointer(CPacketList* pPacketList, 
								CIndicatorInfo*& pIndicatorInfo, 
								CSubGraphPacketList* pSubGraphList);

	bool GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList);

	CPacket* m_pPriceMA;
	CPacket* m_pVolumeMA;
	CPacket* m_pEndPricePacket;
	CPacket* m_pVolumePacket;
	CList<double, double>* m_pEndPriceList;
	CList<double, double>* m_pVolumeList;

};

class CSwingChart : public CIndicator
{
public:
	CSwingChart();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);
	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									  CGraphCalculateData* pGraphCalculateData,
									  CIndicatorInfo* pIndicatorInfo);
	void GetSwingChart(const CPoint& point,
					   double dUnitValue,
					   double dInverseUnit,
					   CList<double, double>* pEndPriceList,
					   CList<double, double>* pDateList,
					   CPacket* pSwingPrice,
					   CPacket* pSwingDate);

private:
	bool GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculateType,
						   CPacketList* pPacketList);
	bool SetGraphPacketList(CPacketList* pPacketList,
							CGraphCalculateData* pGraphCalculateData,
							CIndicatorInfo*& pIndicatorInfo);
	bool InCaseNoPacketPointer(CPacketList* pPacketList,
							   CIndicatorInfo*& pIndicatorInfo,
							   CSubGraphPacketList* pSubGraphList);

	CPacket* m_pEndPricePacket;
	CPacket* m_pDatePacket;
	CPacket* m_pSwingPrice;	// Swing Price 패킷
	CPacket* m_pSwingDate;	// Swing 자료일자 패킷
};

class CKagiChart : public CIndicator
{
public:
	CKagiChart();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									  CGraphCalculateData* pGraphCalculateData,
									  CIndicatorInfo* pIndicatorInfo);
private:
	// 조건값 사용자 지정 가능 - ojtaso (20070913)
	void GetKagiChart(const CPoint& point,
					 double dUnitValue,
					 double dInverseUnit,
					 CList<double, double>* pEndPriceList,
					 CList<double, double>* pDateList,
					 CPacket* pKagiPrice,
					 CPacket* PKagiDate);
	bool GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculateType,
						   CPacketList* pPacketList);
	bool SetGraphPacketList(CPacketList* pPacketList,
							CGraphCalculateData* pGraphCalculateData,
							CIndicatorInfo*& pIndicatorInfo);
	bool InCaseNoPacketPointer(CPacketList* pPacketList,
							   CIndicatorInfo*& pIndicatorInfo,
							   CSubGraphPacketList* pSubGraphList);

	CSwingChart m_swingChart;
	CPacket* m_pEndPricePacket;
	CPacket* m_pDatePacket;
	CPacket* m_pKagiPrice;		// Kagi 가격.
	CPacket* m_pKagiDate;	// Kagi 지표의 자료일자
};

//락유무
class CLock : public CIndicator  
{
public:
	CLock( HWND p_hOcxWnd);
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

private:
	bool SetGraphPacketList(CPacketList* pPacketList,
							CGraphCalculateData* pGraphCalculateData,
							const CString& strPacketNames, 
							CIndicatorInfo*& pIndicatorInfo,
							CPacket*& pLock);
	
	void GetLock(const CPoint& point, CList<double, double>* pLockList, CList<double, double>* pLockRatioTextList, CPacket* pLock);
	CString GetData(const int nData) const;
	CString GetRatioTextData(const double nRatioData) const;

private:
	CList<double, double>* m_pLockYesOrNotList;
	CList<double, double>* m_pLockRatioTextList;

// (2008/1/22 - Seung-Won, Bae) for Multi-Language
protected:
	HWND				m_hOcxWnd;
};

class CMPPacket;
class CMProfile;
class CMarketProfileChart : public CIndicator
{
public:
	CMarketProfileChart();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	void	GetTotalMarketProfile( const CPoint& point, CList<double, double>* pConditionList,
				CMPPacket* pMarketPacket, int Start, int End, CScaleBaseData::HORZSCALEDRAWERTYPE nType);
	void	GetDayMarketProfile(const CPoint& point, CList<double, double>* pConditionList,
				CMPPacket* pMarketPacket, CScaleBaseData::HORZSCALEDRAWERTYPE nType);
	void	GetRealMarketProfile(const CPoint& point, CList<double, double>* pConditionList,
				CMPPacket* pMarketPacket, CScaleBaseData::HORZSCALEDRAWERTYPE nType);
	void	InsertUp( CMProfile* pMarketProfile, int nPriceX100, double dVolume, CString& strAlpa, int nIndex, int nDate);
	void	InsertDown( CMProfile* pMarketProfile, int nPriceX100, double dVolume, CString& strAlpa, int nIndex, int nDate);
	int		GetMDay( int nDate);

private:
	CList<double, double>* m_pEndPriceList;
	CList<double, double>* m_pVolumeList;
	CList<double, double>* m_pDateList;
	CList<double, double>* m_pIndexList;
	CList<CString, CString>* m_pAlpabetList;

	CString m_strDateType;
	CPacketList* m_pPacketList;
	CCalculateBaseData::CALCULATETYPE m_CalType;

// (2004.01.19, 배승원) CMPItem에서 CBracket의 신규 추가를 식별하기 위하여 조건 시간과 가격을 관리한다.
// (2004.01.26, 배승원) Real 처리시를 위해 일별/통합 여부도 관리한다.
protected:
	int		m_nTimeUnit;
	int		m_nPriceUnitX100;
	int		m_nTotalMPChart;	// 1:통합, 0:일별

// (2004.01.19, 배승원) 최종적으로 등록한 Data의 값들 관리한다.
//		이후의 Data 추가시 참조하기 위함이다.
// (2004.01.20, 배승원) 가격만큼은 정수로 관리한다. (double연산의 오류를 없애기 위함이다.)
protected:
	int		m_CurrentIndex;			// 마지막으로 등록된 CBracket이 있는 CMPItem의 Index 번호이다.
	int		m_CurrentTime;			// 마지막으로 등록된 CBracket의 Data Time이다.
	int		m_CurrentPriceX100;		// 미자막으로 등록된 CBracket이 있는 CMPItem의 가격이다.
	int		m_CurrentBracketIndex;	// 마지막으로 등록된 CBracket의 Index값이다.
	char	m_CurrentChar;			// 마지막으로 등록된 CBracket의 Character Item이다.

// (2004.01.26, 배승원) 마지막 'A'의 시간을 관리하여, Real을 처리토록한다.
protected:
	int		m_LastAMinute;			// 시간 진법을 이용하지 않는다.
};


class CPF : public CIndicator  
{
public:
	CPF();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									CGraphCalculateData* pGraphCalculateData,
									CIndicatorInfo* pIndicatorInfo);
private:
	bool SetGraphPacketList(CPacketList* pPacketList,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo*& pIndicatorInfo);

	bool InCaseNoPacketPointer(CPacketList* pPacketList, 
								CIndicatorInfo*& pIndicatorInfo, 
								CSubGraphPacketList* pSubGraphList);

	bool GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList);
/*	void WhenDownduringUp(const int& nRet,
						  const double& dClose,
						  const double& dDate,
						  const double dUnitSize,
						  bool& UPCheck,
						  double& dBaseData,
						  double& conversionValue,
						  CList<double, double>& dTOPList,
						  CList<double, double>& dBOTTOMList,
						  CList<double, double>& dDATEList);
	void WhenDownduringDown(const double& dClose,
							const double& dDate,
							const double dUnitSize,
							double& dBaseData,
							CList<double, double>& dTOPList,
							CList<double, double>& dBOTTOMList,
							CList<double, double>& dDATEList);
	void WhenUpduringDown(const int& nRet,
						  const double& dClose,
						  const double& dDate,
						  const double dUnitSize,
						  bool& UPCheck,
						  double& dBaseData,
						  double& conversionValue,
						  CList<double, double>& dTOPList,
						  CList<double, double>& dBOTTOMList, 
						  CList<double, double>& dDATEList);
	void WhenUpduringUp(const double& dClose,
						const double& dDate,
						const double dUnitSize,
						double& dBaseData,
						CList<double, double>& dTOPList,
						CList<double, double>& dBOTTOMList,
						CList<double, double>& dDATEList);*/
	void GetSecondDataandStartPoint(CList<double, double>* pClosePriceList,
									const double dUnitSize,
									int& startpoint,
									double& seconddata);
	void WhenUpduringUp(const double& dClose, const double& dDate, const double dUnitSize,
						 double& dBaseData, CPacket* pTOPPacket, CPacket* pBOTTOMPacket,
						 CPacket* pDATEPacket);
	void WhenUpduringDown(const int& nRet, const double& dClose, const double& dDate,
						   const double dUnitSize, bool& UPCheck, double& dBaseData,
						   double& conversionValue, CPacket* pTOPPacket, CPacket* pBOTTOMPacket,
						   CPacket* pDATEPacket);
	void WhenDownduringDown(const double& dClose, const double& dDate, const double dUnitSize, 
							 double& dBaseData, CPacket* pTOPPacket, CPacket* pBOTTOMPacket,
							 CPacket* pDATEPacket);
	void WhenDownduringUp(const int& nRet, const double& dClose, const double& dDate,
						   const double dUnitSize, bool& UPCheck, double& dBaseData,
						   double& conversionValue, CPacket* pTOPPacket, CPacket* pBOTTOMPacket,
						   CPacket* pDATEPacket);
/*	void GetPF(const CPoint &point, const double nMin, const double nMax, CList<double, double>* pConditionList,
			   CList<double, double>* pClosePriceList, CList<double, double>* pDateList,
			   CList<double, double>& dTOPList, CList<double, double>& dBOTTOMList,
			   CList<double, double>& dDATEList); */
//	void GetPF(const CPoint &point, const double nMin, const double nMax, CList<double, double>* pConditionList,
//			   CList<double, double>* pClosePriceList, CList<double, double>* pDateList,
//			   CPacket* dTOPList, CPacket* dBOTTOMList, CPacket* dDATEList);
	void GetPF(const CPoint &point, const double nMin, const double nMax, CList<double, double>* pConditionList,
			   CList<double, double>* pClosePriceList, CList<double, double>* pDateList,
			   CPacket* dTOPList, CPacket* dBOTTOMList, CPacket* dDATEList,
			   CList<double, double>* pDrawConditionList,BOOL bReal,BOOL bDataType, CPacketList* pPacketList);

	double GetUnitValue(const double nMin, const double nMax, CList<double, double>* pConditionList);

	CPacket* m_pTLBTop;
	CPacket* m_pTLBBottom;
	CPacket* m_pTLBDate;
	CPacket* m_pEndPricePacket;
	CPacket* m_pDatePacket;
	CList<double, double>* m_pEndPriceList;
	CList<double, double>* m_pDatePriceList;
};

class CRainbow : public CIndicator
{
public:
	CRainbow();
	~CRainbow();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									CGraphCalculateData* pGraphCalculateData,
									CIndicatorInfo* pIndicatorInfo);

private:
	bool SetGraphPacketList(const CChartFormulateData& chartFormulateData,
							  CGraphCalculateData* pGraphCalculateData);

	bool IsIndicatorCalculated(const CCalculateBaseData::CALCULATETYPE calculatetype);

	bool GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, const double dPacketCount, CPacketList* pPacketList, CIndicatorInfo* pIndicatorInfo);
	void GetRainbow(const CPoint& point,
					const double dFirstCondition, 
				    const double dIntervalofMV, 
				    const double dPacketCount, 
					const CString& strType,
				    CList<double, double>* pEndPriceList, 
				    CPacketList* pPacketList,
					CChartInfo::CHART_MODE p_eChartMode);

	CList<CPacket*, CPacket*> m_RainbowList;
	CList<double, double>* m_pEndPriceList;
	CPacket* m_pEndPricePacket;
};

class CRelative_Strength : public CIndicator  
{
public:
	CRelative_Strength();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									CGraphCalculateData* pGraphCalculateData,
									CIndicatorInfo* pIndicatorInfo);

private:
	bool GetCondition(const CIndicatorInfo* pIndicatorInfo, double& dCond) const;
	void GetRSC(const CPoint& point, const double& dCond, const CPacket* pClosePrice, CPacket* pRSC);
	double GetRSCData(const double& dFirstData, const double& dPriceData, const double& dCond) const;
	bool GetEnvironmentDataForCalRSC(const CPacket* pClosePrice, 
		CList<double, double>*& pClosePriceDataList, int& nCount, double& dMin, double& dMax, double& dFirstData) const;
	bool InitializeRSC(CPacket* pRSC, const int nStartIndex, const int nCount, const double& dCond, const double& dMin, const double& dMax);

	void GetRSC(const CPoint &point, const double dCondition, const double nMin, const double nMax, CList<double, double>* pClosePriceList, CPacket* pRSC);
	bool GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, const CString& strSubName, CPacketList* pPacketList, CIndicatorInfo* pIndicatorInfo);

	void SetPacketTypes(const double& dCond);
	bool SetGraphPacketList(const CString& strSubGraphName,
							CPacketList* pPacketList,
							CGraphCalculateData* pGraphCalculateData,
							CIndicatorInfo*& pIndicatorInfo);

private:
	CPacket* m_pRS;
	CList<double, double>* m_pEndPriceList;
	CPacket* m_pEndPricePacket;
};

/**************************************************************************
* Chart Name : RenKo Chart
* Made By    : Yoo Je Sun
* Date       : 2005.06.21.
**************************************************************************/
class CRenKoChart : public CIndicator  
{
public:
	CRenKoChart();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									  CGraphCalculateData* pGraphCalculateData,
									  CIndicatorInfo* pIndicatorInfo);

private:
	bool SetGraphPacketList(CPacketList* pPacketList,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo*& pIndicatorInfo);

	bool InCaseNoPacketPointer(CPacketList* pPacketList, 
								CIndicatorInfo*& pIndicatorInfo, 
								CSubGraphPacketList* pSubGraphList);

	void GetRenKoChart(const CPoint &point, double dBrickCondition,
								CList<double, double>* pClosePriceList,
								CList<double, double>* pDateList);

	bool GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList);
	
	// 벽돌의 사이즈를 종목별로 가져오는 함수
	double GetBrickSize(CIndicatorInfo* pIndicatorInfo, CPacket* pClosePacket, CPacketList* pPacketList, CChartInfo::CHART_MODE p_eChartMode) const;
	double GetBrickSize(CIndicatorInfo* pIndicatorInfo) const;
	double GetBrickSize(CPacket* pClosePacket) const;
	double GetBrickSize_Stock(CPacket* pClosePacket) const;
	double GetBrickSize_Price(CPacket* pClosePacket, CPacketList* pPacketList, CChartInfo::CHART_MODE p_eChartMode) const;
	double CalculateBrickSizeUsingTickValue(double brickSize, double tickValue) const;

private:
	CPacket* m_pClosePrice;
	CPacket* m_pDate;

	CPacket* m_pRenKoHighPrice;	// RenKoChart Open Price (시가 패킷)
	CPacket* m_pRenKoLowPrice;	// RenKoChart Close Price (종가 패킷)
	CPacket* m_pRenKoDate;		// RenKoChart Date(자료일자 패킷)
};

class CThreeLineBreak : public CIndicator  
{
public:
	CThreeLineBreak();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
									CGraphCalculateData* pGraphCalculateData,
									CIndicatorInfo* pIndicatorInfo);

private:
	bool SetGraphPacketList(CPacketList* pPacketList,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo*& pIndicatorInfo);

	bool InCaseNoPacketPointer(CPacketList* pPacketList, 
								CIndicatorInfo*& pIndicatorInfo, 
								CSubGraphPacketList* pSubGraphList);

//	void GetThreeLineBreak(const CPoint &point, CList<double, double>* pClosePriceList, CList<double, double>* pDateList, CList<double, double>& dTOPList, CList<double, double>& dBOTTOMList, CList<double, double>& dDATEList);
//	(2006.09.07,안승환) 삼선전환도 칸 전환수를 수정할수 있도록 함 
	void GetThreeLineBreak(const CPoint &point,
										CList<double, double>* pClosePriceList,
										CList<double, double>* pDateList,
										CPacket* dTOPList,
										CPacket* dBOTTOMList,
										CPacket* dDATEList,
										CList<double, double>* pConditionList);
//	(2006.09.07,안승환) 삼선전환도 칸 전환수를 수정할수 있도록 함 
	bool GetSubGraphPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, CPacketList* pPacketList);

	CPacket* m_pTLBTop;
	CPacket* m_pTLBBottom;
	CPacket* m_pTLBDate;
	CList<double, double>* m_pEndPriceList;
	CList<double, double>* m_pDateList;

};

class CZigzag : public CIndicator  
{
public:
	CZigzag();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo* pIndicatorInfo);

	void GetZigzag(const CPoint& point, double dCondition1, double dCondition2,
				   CList<double, double>* pEndPriceList, CList<double, double>* pZigDataList,
				   CList<double, double>* pZigIndexList, bool CLCflage, 
				   CScaleBaseData::HORZSCALEDRAWERTYPE m_nType);

private:
	double MakeChangHighValue(double prevValue, double dCondition2, double dCondition1,
						      CScaleBaseData::HORZSCALEDRAWERTYPE nType);
	double MakeChangLowValue(double prevValue, double dCondition2, double dCondition1,
							 CScaleBaseData::HORZSCALEDRAWERTYPE nType);
	bool ClearPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, 
						  CPacketList* pPacketList, CSubGraphPacketList* pSubGraphPacketList, CIndicatorInfo*& pIndicatorInfo);
	bool SetGraphPacketList(const CCalculateBaseData::CALCULATETYPE calculatetype,
							CPacketList* pPacketList,
							CGraphCalculateData* pGraphCalculateData,
							CIndicatorInfo*& pIndicatorInfo);
	double GetRealDataIndex(CList<double, double>* pEndPriceList,CList<double, double>* pZigDataList, 
					             CList<double, double>* pZigIndexList, int& nPoint);
	
public:
	double m_MoveAverge;
	double   m_Startpoint;

private:
	enum DIRECTION {NONE, HIGH, LOW} m_lastDirection;
	double	m_PrevHighValue, m_PrevLowValue, m_ChangeValue;
	double m_Min;
	double m_Max;
	CPacket* m_pZigDataPacket;
	CPacket* m_pZigIndexPacket;
	CPacket* m_pEndPacket;
	CList<double, double>* m_pEndPriceList;
};

//******************************************************************************************************
//꺽은선 차트...
//2002,9,11 SJ
//******************************************************************************************************
class CCLC : public CIndicator
{
public:
	CCLC();
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

	virtual void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo* pIndicatorInfo);

private:
	void GetCLC(const CPoint& point, double dCondition1, double dCondition2,
			  	  CList<double, double>* pEndPriceList, CPacket* pCLCDataPacket,
				  CPacket* pCLCIndexPacket);
	double MakeChangHighValue(double prevValue, double dCondition2, double dCondition1,
		CScaleBaseData::HORZSCALEDRAWERTYPE nType);
	double MakeChangLowValue(double prevValue, double dCondition2, double dCondition1,
		CScaleBaseData::HORZSCALEDRAWERTYPE nType);
	bool SetGraphPacketList(const CCalculateBaseData::CALCULATETYPE calculatetype,
							CPacketList* pPacketList,
							CGraphCalculateData* pGraphCalculateData,
							CIndicatorInfo*& pIndicatorInfo);
	bool ClearPacket(const CCalculateBaseData::CALCULATETYPE calculatetype, 
					 CPacketList* pPacketList, CSubGraphPacketList* pSubGraphPacketList);
	bool SetCLCIndexList(double nPrevIndex, CList<double, double>* pEndPriceList, 
						 CList<double, double>& dZigzagIndexList, 
						 CList<double, double>* pCLCIndexList, CList<double, double>* pCLCDataList);

public:
	void CalculateRatio(const CPoint& point, CList<double, double>& dZigzagList, CList<double, double>* pEndPriceList);
	double m_step;
	CScaleBaseData::HORZSCALEDRAWERTYPE m_nType;

private:
	enum DIRECTION {NONE, HIGH, LOW} m_lastDirection;
	double	m_PrevHighValue;
	double  m_PrevLowValue;
	CPacket* m_pCLCDataPacket;
	CPacket* m_pCLCIndexPacket;
	CPacket* m_pEndPacket;
	CList<double, double>* m_pEndPriceList;
};

///////////////////////////////////////////////////////////////////////////////
//매수매도 대기매물바
////////////////////////////////////////////////////////////////////////////////
class CSellBuyHorizontalBar : public CIndicator  
{
public:
	CSellBuyHorizontalBar();

public:
	virtual void GetData(const CChartFormulateData& chartFormulateData,
					CGraphCalculateData* pGraphCalculateData, CIndicatorInfo*& pIndicatorInfo,
					CCalculateData& calculateData);
protected:
	CList<double, double>* m_pEndPriceList;
	CList<double, double>* m_pVolumeList;
	CList<double, double>* m_pBuyVolumeList;	//매수거래량
	CList<double, double>* m_pSellVolumeList;	//매도 거래량

	CPacket* m_pHorZonPacket;
	CPacket* m_pHorZonPricePacket;
	CPacket* m_pHorZonBuyPacket;
	CPacket* m_pHorZonSellPacket;

// (2008/4/23 - Seung-Won, Bae) Make the price unit double
	double	m_dHoga;
protected:
	int		m_StartPoint;	// 대기매물 산출 범위의 시작 Index이다.
	int		m_EndPoint;		// 대기매물 산출 범위의 끝 Index이다.
	double	m_Max;			// 대기매물 산출 범위의 최고 종가이다.
	double	m_Min;			// 대기매물 산출 범위의 최저 종가이다.

private:
	void GetSellBuyHorizontalBar(const CChartFormulateData& chartFormulateData, 
				const CPoint& point, double dCondition, CList<double, double>* pEndPriceList,
				CList<double, double>* pVolumeList,CList<double, double>* pSellVolumeList,
				CList<double, double>* pBuyVolumeList,double dConditionType,double dHoga);
};

// vntsorl:[0003431] [7111]차트화면에서의 조회속도가 구차트에 비해 늦어 개선 요구
// (2010/3/20 - Seung-Won, Bae) Do not use ST MA
#define _MA_COUNT	20
class CMA : public CIndicator
{
typedef CList<double, double> CDoublePacketData;

public:
	CMA( IMetaTable::EDDC_INDICATOR_ID p_eID);
	virtual ~CMA();

	virtual void	GetData(const CChartFormulateData& chartFormulateData,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo*& pIndicatorInfo,
							 CCalculateData& calculateData);
	inline void		GetMA(const CChartFormulateData& chartFormulateData,
							 CGraphCalculateData* pGraphCalculateData,
							 CIndicatorInfo*& pIndicatorInfo,
							 CCalculateData& calculateData);

protected:
	void	GetSimpleMA(		const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket);
	void	GetExponentialMA(	const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket);
	void	GetWeightedMA(		const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket);
	void	GetGeometricMA(		const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket);
	void	GetHarmonicMA(		const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket);
	void	GetTriangularMA(	const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket);
	void	GetAdaptiveMA(		const int &p_nStartIndex, const double &dPeriod, CDoublePacketData *pdlSourceList, CPacket *pMAPacket);

// (2010/4/15 - Seung-Won, Bae) Manage Environment State.
protected:
	double						m_daMAPeriod[ _MA_COUNT];
	int							m_naMAType[ _MA_COUNT];
	CPacket *					m_ppaMAPacket[ _MA_COUNT];
	CDoublePacketData *			m_pdlaSourceList[ _MA_COUNT];
	int							m_naInputPacketType[ _MA_COUNT];	// 1: Normal, 2:HL/2, 3:HLC/3
	BOOL						m_bShow[ _MA_COUNT];
	CDoublePacketData *			m_pEndList;
	CDoublePacketData *			m_pHighList;
	CDoublePacketData *			m_pLowList;
	CStringArray				m_saSubGraphName;
	CList< double, double>		m_dlDoubleList1[ _MA_COUNT];
	CList< double, double>		m_dlDoubleList2[ _MA_COUNT];

// (2010/4/19 - Seung-Won, Bae) for Sub Graph Iterating.
protected:
	int									m_nSubGraphIndex;
	CCalculateBaseData::CALCULATETYPE	m_eCalcType;
	CPacketList *						m_pPacketList;

protected:
	inline void		SetMAValue( const int &p_nStartIndex, const int &nUpdateIndex, const double &dMA, CPacket *pMAPacket);

// (2010/4/19 - Seung-Won, Bae) for Signal Sub Graph Index.
protected:
	int							m_nSignalMA1;
	int							m_nSignalMA2;

// (2010/6/8 - Seung-Won, Bae) Retrieve the first visible SubGrpah Index to check last calculation.
protected:
	int		m_nFirstVisibleSubGraph;
public:
	int		GetFirstVisibleSubGraph( void)		{	return m_nFirstVisibleSubGraph;	}
};

class CNoCalculate : public CIndicator  
{
public:
	CNoCalculate( IMetaTable::EDDC_INDICATOR_ID p_eID) : CIndicator( p_eID)	{}

public:
	virtual void GetData(const CChartFormulateData& chartFormulateData,
						 CGraphCalculateData* pGraphCalculateData,
						 CIndicatorInfo*& pIndicatorInfo,
						 CCalculateData& calculateData);

private:
	bool InCaseNoPacketPointer(const CChartFormulateData& chartFormulateData, 
		const int nSubGraphCount, const int nSubGraphIndex, CSubGraphPacketList* pSubGraphList);
	CString GetPacketNames(const CString& strOrgPacketNames, const int nSubGraphCount, const int nSubGraphIndex) const;

	void SetGraphPacketList(const CChartFormulateData& chartFormulateData, CGraphCalculateData* pGraphCalculateData);
	//미니지수
	void CalculateBalancePacket(const CChartFormulateData& chartFormulateData, CCalculateData& calculateData);
	void CalExCeptionPacket(const CChartFormulateData& chartFormulateData, CCalculateData& calculateData);	
};

#endif // !defined(AFX_INDICATORFACTORY_H__1F4A506F_3636_46B2_844F_B049AA6D1A3A__INCLUDED_)
