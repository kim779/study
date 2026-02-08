#if !defined(AFX_INTERFACE_OF_DRFN_CHARTBOZOMESSAGE_BY_JUNOK_LEE__INCLUDED_)
#define AFX_INTERFACE_OF_DRFN_CHARTBOZOMESSAGE_BY_JUNOK_LEE__INCLUDED_

#include "IBaseDefine.h"
#include "MasterDataSub.h"

#define		UUID_IChartBozoMsgManager		(51)

// 내부적인 인터페이스 번호
	// 실제적인 구현은 PartnerMng.dll에서 구현되어 있음. 여기에서의 내부 Interface번호
#define		LUID_IChartBozoMsgManager		1

class CChartBozoItem
{
public:
	enum{
		cbi_chanage=0,
		cbi_add,
		cbi_delete,
	};

	int		 m_nOption; // enum중 하나.
	CString  m_szKey;
	CString  m_szValue;

	CChartBozoItem(int nOption, LPCSTR szKey, LPCSTR szValue)
	{
		m_nOption = nOption;
		m_szKey = szKey;
		m_szValue = szValue;
	}
};
typedef CList<CChartBozoItem*, CChartBozoItem*> List_CChartBozoItem;

class CChartBozoInput
{
public:
//@Solomon:091123SM068 -->
	CChartBozoInput()	{ InitData();	};
	CChartBozoInput(const CChartBozoInput& obj)	{ *this = obj;	};

	void InitData()
	{
		ZeroMemory(sShcode, sizeof(sShcode));
//		ZeroMemory(sTick, sizeof(sTick));
		lTick = 0;
		ZeroMemory(sTerm, sizeof(sTerm));
		ZeroMemory(sNxtFlg, sizeof(sNxtFlg));
		ZeroMemory(sDataType, sizeof(sDataType));
		lPrice = 0;
		lVolume = 0;
		lUpLmtPrice = 0;
		lDnLmtPrice = 0;
		lOpen;
		lHigh;
		lLow;
		lPreprice;	
		lPreVolume;	
		lPreOpen;
		lPreHigh;
		lPreLow;
		//ZeroMemory(sStartTime, sizeof(sStartTime));
		lFirstClose = 0;		
		nMarket = 0;
		lWonDollar = 0;

//vntsorl_20110420:[A00000410] 맵차트에서 첫리얼에 새로운 봉이 생기는 오류수정
		nPacketType = 0;
	};

	virtual void	Copy(const CChartBozoInput& right)
	{
		m_sMarket		= right.m_sMarket;
		m_sShcode		= right.m_sShcode;
		m_sTick			= right.m_sTick;
		m_sTerm			= right.m_sTerm;
		m_sNxtFlg		= right.m_sNxtFlg;
		m_sDataType		= right.m_sDataType;
		m_sVolume		= right.m_sVolume;
		m_sUpLmtPrice	= right.m_sUpLmtPrice;
		m_sDnLmtPrice	= right.m_sDnLmtPrice;
		m_sOpen			= right.m_sOpen;
		m_sHigh			= right.m_sHigh;
		m_sLow			= right.m_sLow;
		m_sPrice		= right.m_sPrice;		// 20100326_JS.Kim.  종가 복사안하는 버그 수정
		m_sPreprice		= right.m_sPreprice;
		m_sPreVolume	= right.m_sPreVolume;
		m_sStartTime	= right.m_sStartTime;
		m_sFirstClose	= right.m_sFirstClose;
		m_sPreOpen		= right.m_sPreOpen;
		m_sPreHigh		= right.m_sPreHigh;
		m_sPreLow		= right.m_sPreLow;
		m_sWonDollor	= right.m_sWonDollor;
		memcpy(sShcode, right.sShcode, sizeof(sShcode));
		lTick			= right.lTick;
		memcpy(sTerm, right.sTerm, sizeof(sTerm));
		memcpy(sNxtFlg, right.sNxtFlg, sizeof(sNxtFlg));
		memcpy(sDataType, right.sDataType, sizeof(sDataType));
		lPrice			= right.lPrice;
		lVolume			= right.lVolume;
		lUpLmtPrice		= right.lUpLmtPrice;
		lDnLmtPrice		= right.lDnLmtPrice;
		lOpen			= right.lOpen;
		lHigh			= right.lHigh;
		lLow			= right.lLow;
		lPreprice		= right.lPreprice;
		lPreVolume		= right.lPreVolume;
		lFirstClose		= right.lFirstClose;
		lPreOpen		= right.lPreOpen;
		lPreHigh		= right.lPreHigh;
		lPreLow			= right.lPreHigh;
		nMarket			= right.nMarket;
		lWonDollar		= right.lWonDollar;
//vntsorl_20110420:[A00000410] 맵차트에서 첫리얼에 새로운 봉이 생기는 오류수정
		nPacketType		= right.nPacketType;
		m_sLastDataTime	= right.m_sLastDataTime;
		m_sVolAmt		= right.m_sVolAmt;		//[A00000651][A00000652]누적거래대금.

		m_szMarketTime	= right.m_szMarketTime;;
		m_szBaseTime	= right.m_szBaseTime;;
		m_strHogaTick = right.m_strHogaTick;
	};

	virtual CChartBozoInput& operator = (const CChartBozoInput& item)	{ Copy(item);	return *this; };
//@Solomon:091123SM068 <--

//	#include "inc/MasterDataSub.h"에 정의된 값으로 대체 바랍니다.
// 	enum {
// 		ctnone=0,
// 		ctstk=1,
// 		ctup,
// 		ctsfo,
// 		ctfo,
// 		ctfrn,
// 	};
	
public:
	//--> IChartBozoMsgManager->GetChartBozoMsg(..)를 호출하기전에 셋팅해줘야하는 Data
	//조회입력값중에서 필요한 부분.
	CString m_sMarket;			// 시장구분
								//_ctstk : 1(KOSPI), 2(KOSDAQ), 3(ELW)
								//_ctsfo : 1(선물), 2(옵션)
								//_ctfo  : 1(선물), 2(옵션)

	CString m_sShcode,			// 종목코드
			m_sTick,			// N봉
			m_sTerm,			// 구분 : 구분(0:틱/1:분/2:일/3:주/4:월/6:초)
			m_sNxtFlg,			// 다음조회구분:1(다음조회), 0(처음조회)
			m_sDataType;		//	차트 : 0-가격차트, 1-대비차트, 2-달러차트, 3-현재가, 4-시가차트

	//조회결과중 필요한 부분.
	CString m_sPrice;		// 현재가
	CString m_sVolume;		// 거래량
	CString m_sUpLmtPrice;	// 상한가
	CString m_sDnLmtPrice;	// 하한가
	CString m_sOpen;		// 시가
	CString m_sHigh;		// 고가
	CString m_sLow;			// 저가
	CString m_sPreprice;	// 전일종가
	CString m_sPreVolume;	// 전일거래량
	CString m_sStartTime;	// 거래시작시간(HHMMSS)
	CString m_sFirstClose;	// 조회중 첫봉의 종가.
	CString m_sPreOpen;		// 전일시가
	CString m_sPreHigh;		// 전일고가
	CString m_sPreLow;		// 전일저가
	CString	m_sWonDollor;	// 원달러 환율
	//<-- IChartBozoMsgManager->GetChartBozoMsg(..)를 호출하기전에 셋팅해줘야하는 Data
	CString	m_sVolAmt;
	CString m_sBaseValue; // 기준가 
	CString m_szMarketTime;
	CString m_szBaseTime;
	CString m_strHogaTick; //호가단위

public:
	inline void SetItemData(CString& rKey, char* pData, int nSize)
	{
		rKey.Format("%*.*s", nSize, nSize, pData);
		rKey.TrimRight();
	}
	

// 여기아래 부분은 개별적으로 셋팅하거나 Call하지 마세요.
public:	
	inline void DoConvert()
	{
		memcpy(sShcode, m_sShcode, m_sShcode.GetLength());
//		memcpy(sTick, m_sTick, m_sTick.GetLength());
		lTick		= atol(m_sTick);
		memcpy(sTerm, m_sTerm, m_sTerm.GetLength());
		memcpy(sNxtFlg, m_sNxtFlg, m_sNxtFlg.GetLength());
		memcpy(sDataType, m_sDataType, m_sDataType.GetLength());
		lPrice		= atol(m_sPrice);
		lVolume		= atol(m_sVolume);
		lUpLmtPrice = atol(m_sUpLmtPrice);
		lDnLmtPrice = atol(m_sDnLmtPrice);
		lOpen		= atol(m_sOpen);
		lHigh		= atol(m_sHigh);
		lLow		= atol(m_sLow);
		lPreprice	= atol(m_sPreprice);	
		lPreVolume	= atol(m_sPreVolume);	
		//memcpy(sStartTime, m_sStartTime, m_sStartTime.GetLength());
		lFirstClose = atol(m_sFirstClose);
		nMarket		= atoi(m_sMarket);
		lPreOpen	= atoi(m_sPreOpen);
		lPreHigh	= atoi(m_sPreHigh);
		lPreLow		= atoi(m_sPreLow);
		lWonDollar	= atol(m_sWonDollor);
		lVolAmt		= atol(m_sVolAmt);
	}

	char sShcode[15+1];	// 종목코드
//	char sTick[3+1];	// N봉
	long lTick;			// N봉
	char sTerm[1];		// 구분 : 구분(0:틱/1:분/2:일/3:주/4:월/6:초)
	char sNxtFlg[1];	// 다음조회구분:1(다음조회), 0(처음조회)
	char sDataType[1];	//	차트 : 0-가격차트, 1-대비차트, 2-달러차트, 3-현재가, 4-시가차트

	//조회 데이터중 필요한 데이터.
// 		char sPrice[10+1];		// 현재가
// 		char sUpLmtPrice[10+1];	// 상한가
// 		char sDnLmtPrice[10+1];	// 하한가
// 		char sOpen[10+1];		// 시가
// 		char sHigh[10+1];		// 고가
// 		char sLow[10+1];		// 저가
// 		char sPreprice[10+1];	// 전일종가
// 		char sPreVolume[10+1];	// 전일거래량
// 		char sStartTime[6+1];	// 거래시작시간(HHMMSS)
// 		char sFirstClose[10+1];	// 조회중 첫봉의 종가.
	long lPrice;		// 현재가
	long lVolume;		// 거래량
	long lUpLmtPrice;	// 상한가
	long lDnLmtPrice;	// 하한가
	long lOpen;		// 시가
	long lHigh;		// 고가
	long lLow;		// 저가
	long lPreprice;	// 전일종가
	long lPreVolume;	// 전일거래량
	//char sStartTime[6+1];	// 거래시작시간(HHMMSS)
	long lFirstClose;	// 조회중 첫봉의 종가.
	long lPreOpen;
	long lPreHigh;
	long lPreLow;
	//기타
//	char cMarket[1];		// 시장구분 : 1(KOSPI), 2(KOSDAQ), 3(ELW)
	int  nMarket;			// 시장구분
							//_ctstk : 1(KOSPI), 2(KOSDAQ), 3(ELW)
							//_ctsfo : 1(선물), 2(옵션)
							//_ctfo  : 1(선물), 2(옵션)
	long lWonDollar;

//vntsorl_20110420:[A00000410] 맵차트에서 첫리얼에 새로운 봉이 생기는 오류수정
	int  nPacketType;		//PacketType을 별개로 셋팅할 때만 사용.
	CString m_sLastDataTime;		// 마지막 자료일자/시간
	long lVolAmt;			//누적거래대금.
};

/* -------------------------------------------------------------------
입력된 값중 필요한 데이터.
	sShcode[15+1] 종목코드
//	sTick[3+1]	N봉
	lTick		N봉
	sTerm		구분 : 구분(0:틱/1:분/2:일/3:주/4:월/6:초)
	sNxtFlg		다음조회구분:1(다음조회), 0(처음조회)

조회 데이터중 필요한 데이터.
	sPrice[10+1] : 현재가
	sUpLmtPrice[10+1] 상한가
	sDnLmtPrice[10+1] 하한가
	sOpen[10+1] 시가
	sHigh[10+1] 고가
	sLow[10+1]  저가
	sPreprice[10+1]  전일종가
	sPreVolume[10+1] 전일거래량
	sStartTime[6+1] 거래시작시간(HHMMSS)
	sFirstClose[10+1] 조회중 첫봉의 종가.

기타
	nMarket			// 시장구분
					//_ctstk : 1(KOSPI), 2(KOSDAQ), 3(ELW)
					//_ctsfo : 1(선물), 2(옵션)
					//_ctfo  : 1(선물), 2(옵션)
------------------------------------------------------------------- */

DECLARE_INTERFACE(IChartBozoMsgManager)
{
	//szFIDKey : FIDKey
	// pChartBInput : input정보.
	// rString : 보조메시지 얻기.
	// nOption : 데이터의 처리형태에 대한 옵션
	//			 0 : 데이터에 point(.)가 없이 올때에 처리하는 형태. 기존의 DRFN사이트의 처리방식.
	//				 주식=> x1, 업종/선물 x0.01
	//			 1 : 데이터 자체가 이미 포맷이 맞춰진 형태. 따라서 x1 형태로 처리한다.
	//			 default는 0(기존 DRFN에서 사용하던 방식)
	// return : 보조메시지 길이
	//
	// 시장지표(CMasterTypeInfo::MARKET_CHART)일때는 Next여부만 필요.
	STDMETHOD_(int, GetChartBozoMsg)(LPCSTR szFIDKey, CChartBozoInput* pChartBInput, CString &rString, int nOption=0) PURE;

	// return : 보조메시지 길이
	STDMETHOD_(int, ChangeChartBozoMsg)(CString &rString, List_CChartBozoItem* pList) PURE;

// 	//STDMETHOD_(void, MakeChtBojo)(LPCTSTR lpszFormat, ...) PURE;	
// 	STDMETHOD_(int, GetFidType)(LPCSTR szFIDMajor) PURE;
// 	STDMETHOD_(int, GetFidType)(int nFIDMajor) PURE;
// 	STDMETHOD_(int, GetTrType)(LPCSTR szTrName) PURE;
// 
// 	STDMETHOD_(int, MakeChtBojo_ctstk)(CChartBozoInput* pChartBInput, CString& rString) PURE;
// 	STDMETHOD_(int, MakeChtBojo_ctup)(CChartBozoInput* pChartBInput, CString& rString) PURE;
// 	STDMETHOD_(int, MakeChtBojo_ctsfo)(CChartBozoInput* pChartBInput, CString& rString) PURE;
// 	STDMETHOD_(int, MakeChtBojo_ctfo)(CChartBozoInput* pChartBInput, CString& rString) PURE;
// 	STDMETHOD_(int, MakeChtBojo_ctfrn)(CChartBozoInput* pChartBInput, CString& rString) PURE;
};

//#include "../Chart/commontr/EU_p0602.h"
// 	static int MakeChtBojo_ctup();
// 	static int MakeChtBojo_ctsfo();
// 	static int MakeChtBojo_ctfo();
// 	static int MakeChtBojo_ctfrn();

// fidxmst.cDataKind = 'X';    /* 데이터 종류 구분자              */
/* P : 미국지수                    */
/* Q : 미국종목(다우30,나스닥100,     
       S&P 500 종목만 해당)	        */
/* D : 미국상장 국내기업(ADR)      */
/* C : 상품 선물                   */
/* F : CME 선물                    */
/* G : 유럽상장국내기업(GDR)       */
/* X : 환율                        */
/* R : 금리                        */
/* L : 리보금리                    */
/* B : 주요국정부채                */
/* N : 국내금리                    */
/* M : 반도체                      */
/* H : 세계주요종목                */
/* E : ECN						   */
/* W : 세계주가지수                */

#endif //AFX_INTERFACE_OF_DRFN_CHARTBOZOMESSAGE_BY_JUNOK_LEE__INCLUDED_