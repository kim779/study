// DataSet.h: interface for the CFutureDataSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUTUREDATASET_H__004D79E3_305C_4245_8682_90D380181782__INCLUDED_)
#define AFX_FUTUREDATASET_H__004D79E3_305C_4245_8682_90D380181782__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _EXT_DATA_SET
	#define	DATASET_DECLSPEC __declspec(dllimport)
#else
	#define	DATASET_DECLSPEC __declspec(dllexport)
#endif

#include <afxtempl.h>
#include "networkdefine.h"

//20080324 이문수 >>
#define	FILE_INI					"TheorySetup.ini"

#define	SECTION_CONFIGURATION		"Config"

#define	SETUP_FILE_INI				"StrategyView.ini"

#define		THEORY_KEY_01			"기초자산가격적용"
#define		THEORY_KEY_01_1			"KOSPI200가중"
#define		THEORY_KEY_01_2			"선물가중"
#define		THEORY_KEY_02			"변동성선택"
#define		THEORY_KEY_02_1			"콜변동성"
#define		THEORY_KEY_02_2			"풋변동성"
#define		THEORY_KEY_03			"잔존일수"
#define		THEORY_KEY_04			"장중시간가치감소반영"
#define		THEORY_KEY_04_1			"시작시간"
#define		THEORY_KEY_04_2			"끝시간"

#define		ORDER_KEY_01			"매도주문시가격구분"	
#define		ORDER_KEY_02			"매도주문시단가선택"
#define		ORDER_KEY_03			"매도호가증감"
#define		ORDER_KEY_04			"매수주문시가격구분"
#define		ORDER_KEY_05			"매수주문시단가선택"
#define		ORDER_KEY_06			"매수호가증감"

#define		SLIPPAGE_ORDER_KEY_01	"우선거래종목선택"
#define		SLIPPAGE_ORDER_KEY_02	"복수우선거래주문순서"
#define		SLIPPAGE_ORDER_KEY_03	"우선거래매도주문가"
#define		SLIPPAGE_ORDER_KEY_04	"우선거래매수주문가"
#define		SLIPPAGE_ORDER_KEY_05	"차선거래매도주문가"
#define		SLIPPAGE_ORDER_KEY_06	"차선거래매수주문가"
#define		SLIPPAGE_ORDER_KEY_07	"현재가와의괴리"
#define		SLIPPAGE_ORDER_KEY_07_1 "우선거래자동주문취소"

#define		DISPLAY_KEY_01			"평균가설정"
#define		DISPLAY_KEY_02			"GREEK수치표시"
#define		DISPLAY_KEY_03			"동일종목추가시"
#define		DISPLAY_KEY_04			"정렬방식"
#define		DISPLAY_KEY_04_1		"정렬방식종류"
#define		DISPLAY_KEY_04_2		"정렬타입"
#define		DISPLAY_KEY_05			"기초자산최소값"
#define		DISPLAY_KEY_06			"기초자산최대값"
#define		DISPLAY_KEY_07			"손익표시범위"
#define		DISPLAY_KEY_07_1		"손익표시최대"
#define		DISPLAY_KEY_07_2		"손익표시최소"
#define		DISPLAY_KEY_08			"잔고수량표시"
#define		DISPLAY_KEY_09			"평가손익계산방식"

#define		ETC_KEY_01				"수수료설정"
#define		ETC_KEY_01_1			"선물수수료율"
#define		ETC_KEY_01_2			"옵션수수료율"
#define		ETC_KEY_01_3			"주식수수료율"
#define		ETC_KEY_02				"증거금관련설정"
#define		ETC_KEY_03				"매수매도변경방법"
#define		ETC_KEY_04				"현재가설정"
#define		ETC_KEY_04_1			"청산기준우선호가"
#define		ETC_KEY_05				"예상체결가적용"
#define		ETC_KEY_06				"내재변동성관련"
//20080324 이문수 <<

class CTheoryForMargin;
class CStrategyCalc;

class CAccount
{
public:
	CString m_strAccount;
	CString m_strAccountName;
	CString m_strGubun1;
	CString m_strGubun2;

public:
	CAccount & operator=(CAccount & account)
	{
		m_strAccount = account.m_strAccount;
		m_strAccountName = account.m_strAccountName;
		m_strGubun1 = account.m_strGubun1;
		m_strGubun2 = account.m_strGubun2;

		return *this;
	}
};
typedef CArray < CAccount *, CAccount *> CAccountArray;


class DATASET_DECLSPEC CHogaSet
{
public:
	
	long	m_lCount;//건수
	CString m_strPrice;//호가
	CString m_strVolume;//잔량
	
public:
	CHogaSet & operator=(CHogaSet & hoga)
	{
		m_lCount	= hoga.m_lCount;
		m_strPrice	= hoga.m_strPrice;
		m_strVolume	= hoga.m_strVolume;		
		return *this;
	}
};
#define	HOGA_CNT	5

class DATASET_DECLSPEC CContractSet
{
public:
	CString	m_strContractTime;
	CString m_strContractPrice;
	CString	m_strOffer;
	CString m_strBid;
	CString m_strContractVolume;
	CString m_strContractAttr;//체결이 매도호가로 되었는가?, 매수호가로 되었는가?

public:

	CContractSet & operator=(CContractSet & cont)
	{
		m_strContractTime	= cont.m_strContractTime;
		m_strContractPrice	= cont.m_strContractPrice;
		m_strOffer			= cont.m_strOffer;
		m_strBid			= cont.m_strBid;
		m_strContractVolume = cont.m_strContractVolume;
		m_strContractAttr	= cont.m_strContractAttr;
		return *this;
	}
	
};
typedef CArray < CContractSet * , CContractSet *> CContractArray;

class CRTDataBaseType
{
public:
	UINT m_uDataType;

};
typedef CArray < CRTDataBaseType *, CRTDataBaseType *> CRTDataBaseTypeArray;

class DATASET_DECLSPEC CDataSet : public CRTDataBaseType
{
public:
	CDataSet()
	{
		m_nRTDataType = RT_NO_DATA_TYPE;
		m_bSelect = TRUE;
		m_bRemainItem = FALSE;
		m_pTheoryForMargin = NULL;
//		m_pNDataSet = NULL;
		m_uDataType = RT_DATA_TYPE;

		m_bHedge = FALSE;
	}

	~CDataSet()
	{
		int i;
		for(i = 0 ; i < m_ContractArray.GetSize(); i++) delete m_ContractArray.GetAt(i);
		m_ContractArray.RemoveAll();
		
// 		if(m_pNDataSet) 
// 		{
// 		
// 			delete m_pNDataSet;			
// 			m_pNDataSet = NULL;
// 		}
	}

public:

	int			m_nRTDataType;					//N분 데이터인지 아닌지 여부 (나중에 더 확장할 수 있을 것이다.)
												//일반데이터 RT_DATA_TYPE	
												//N분 데이터 RTN_DATA_TYPE
												//예상체결	 RT_EXP_DATA_TYPE
	
	CUIntArray	m_nIDArray;						//해당종목의 

	CString	m_strTime;					//시간.

	CString	m_strCount;					//종목수량
	CString	m_strTradeType;				//매매구분("매수", "매도")
	CString m_strAvePrice;				//매매가(또는 평균가)
	CString m_strAveAmount;				//매매금액(정용호 추가 20080407)

	CString m_strCode;					//종목코드 
	CString m_strName;					//종목명
	CString m_strStrike;				//행사가격
	
	CString m_strCurPrice;				//현재가
	CString m_strBid;					//매수호가
	CString m_strOffer;					//매도호가
	CString m_strVolume;				//거래량
	CString	m_strMoney;					//거래금액
	CString m_strLastVolume;			//전일거래량
	CString m_strBidVol;				//매수호가 잔량
	CString m_strOfferVol;				//매도호가 잔량
	CString m_strLastPrice;				//전일종가
	CString	m_strNotTradeVolume;		//미결제량
	CString	m_strUnitVol;				//단위체결량 (실시간으로 현재가 체결데이터가 들어올 경우만 데이터 들어있다)

	CString m_strRemainDay;				//잔여일
	CString m_strLastTradeDate;			//만기일
	CString m_strHistoricalVolate;		//역사적변동성(90일)
	CString m_strCDRate;				//무위험이자율	
	CString m_strIndex;					//배당액지수

	CString m_strPredictPrice;			//주식 예상체결가 (20080416정용호추가)

//	CString m_strFlag;					//전일대비구분(대우에서만의 데이터)

	CHogaSet	m_BidArray[HOGA_CNT];
	CHogaSet	m_OfferArray[HOGA_CNT];	

	CHogaSet	m_TotalBid;
	CHogaSet	m_TotalOffer;

	CContractArray	m_ContractArray;//체결틱 데이터 저장.

	CString		m_strCoveredCallCount;		//사용자가 저장하거나 잔고정보에서 내려온 커버드콜 수량.
	BOOL		m_bSelect;					//선택되었는지 여부.
	BOOL		m_bRemainItem;				//잔고여부.
	
	//예약 
	CString		m_strReserved;				//필요한 경우 적절하게 사용.
	//(8531에서 데이터를 하단 탭 윈도우로 전달시 그리드 줄번호로 사용)
	//n분 데이터에서는 내재변동성값이 저장되어 있다.(201999999, 301999999, 401999999)

	CTheoryForMargin * m_pTheoryForMargin;	//증거금용 이론가 전달용.

	CString				m_strTPrice;//이론가 증거금 계산시에 이론가를 전달할 때 사용.(ETF일 경우 최근월선물현재가)

	CString				m_strIVForMargin;//증거금용 내재변동성


	//kp200에 대한 값...
	CString		m_strCurPrice2;				//현재가
	CString		m_strStartPrice2;			//시가
	CString		m_strHighPrice2;			//고가
	CString		m_strLowPrice2;				//저가
	CString		m_strVolume2;				//거래량
	CString		m_strMoney2;				//거래금액
	CString		m_strPrevPrice2;			//전일종가	
	CString		m_strPrevVolume2;			//전일거래량
	CString		m_strUnderlyingAssetsCode;	//기초자산 코드.
	
	///////////////////////////////////////////
	//설계에는 없던 부분이라 구조상 상당히 어색하다 
	//설계변경할 시간이 없으므로 이곳에 N분 데이터 포인터를 추가한다.
//	CDataSet *  m_pNDataSet;


	//최초 평균가 입력값.
	CString		m_strSaveAvePrice;

	CString		m_strOrderEnableCount;
	CString		m_strRemainItemCount;

	CString		m_strCoveredCallRemainItemCount;
	CString		m_strCoveredCallEnableCount;

	
	///////////////////////////////////////////////////////////////////
	//
	//	서버에서 받은 이론가, 내재변동성, 델타, 감마, 세타, 베가를 저장
	//
	////////////////////////////////////////////////////////////////////
	CString		m_strTPriceFromServer;
	CString		m_strIVFromServer;
	CString		m_strDeltaFromServer;
	CString		m_strGammaFromServer;
	CString		m_strThetaFromServer;
	CString		m_strVegaFromServer;

	BOOL		m_bHedge;					// 헷지종목여부
public:

	void AddID(int nID)
	{
		m_nIDArray.Add(nID);
	}

	void AddIDArray(CDataSet * pDataSet)
	{
		m_nIDArray.Append(pDataSet->m_nIDArray);
	}

	void DeleteAllID()
	{
		m_nIDArray.RemoveAll();
	}

	void DeleteID(UINT nID)
	{
		int i,nSize = m_nIDArray.GetSize();
		for(i = nSize - 1 ; i >= 0; i--)
			if(m_nIDArray.GetAt(i) == nID)
			{
				m_nIDArray.RemoveAt(i);
			}
	}

	CDataSet & operator=(CDataSet & data)
	{
		m_strTime			= data.m_strTime;
		m_strCount			= data.m_strCount;
		m_strTradeType		= data.m_strTradeType;
		m_strAvePrice		= data.m_strAvePrice;
		m_strAveAmount		= data.m_strAveAmount; // 정용호 20080407
		m_strCoveredCallCount=data.m_strCoveredCallCount;

		m_nRTDataType		= data.m_nRTDataType;
		m_strCode			= data.m_strCode;
		m_strName			= data.m_strName;
		m_strStrike			= data.m_strStrike;
		m_strCurPrice		= data.m_strCurPrice;
		m_strBid			= data.m_strBid;
		m_strOffer			= data.m_strOffer;
		m_strVolume			= data.m_strVolume;
		m_strMoney			= data.m_strMoney;
		m_strUnitVol		= data.m_strUnitVol;
		m_strBidVol			= data.m_strBidVol;
		m_strOfferVol		= data.m_strOfferVol;
		m_strLastPrice		= data.m_strLastPrice;
		m_strRemainDay		= data.m_strRemainDay;
		m_strLastTradeDate	= data.m_strLastTradeDate;
		m_strHistoricalVolate=data.m_strHistoricalVolate;
		m_strCDRate			= data.m_strCDRate;
		m_strIndex			= data.m_strIndex;
		m_strReserved		= data.m_strReserved;
		m_bSelect			= data.m_bSelect;
		m_bRemainItem		= data.m_bRemainItem;
		m_strIVForMargin	= data.m_strIVForMargin;
//		m_strFlag			= data.m_strFlag;
		m_strNotTradeVolume = data.m_strNotTradeVolume;
		m_strTPriceFromServer=data.m_strTPriceFromServer;		//서버에서 받은 이론가
		m_strIVFromServer	=data.m_strIVFromServer;			//서버에서 받은 내재변동성
		m_strDeltaFromServer=data.m_strDeltaFromServer;			//서버에서 받은 델타
		m_strGammaFromServer=data.m_strGammaFromServer;			//서버에서 받은 감마
		m_strThetaFromServer=data.m_strThetaFromServer;			//서버에서 받은 세타
		m_strVegaFromServer	=data.m_strVegaFromServer;			//서버에서 받은 베가

// 		m_strNAV			= data.m_strNAV;					//NAV
// 		m_strDiffNAV		= data.m_strDiffNAV;				//NAV괴리치(현재가 대비)
// 		m_strDiffRateNAV	= data.m_strDiffRateNAV;			//NAV괴리율(현재가 대비)
// 		m_strDiffKP200		= data.m_strDiffKP200;				//KP200괴리치(현재가 대비)
// 		m_strDiffRateKP200	= data.m_strDiffRateKP200;			//KP200괴리율(현재가 대비)
// 		m_strDiffKP2001		= data.m_strDiffKP2001;				//KP200괴리치(NAV 대비)
// 		m_strDiffRateKP2001	= data.m_strDiffRateKP2001;			//KP200괴리율(NAV 대비)

		m_bHedge			= data.m_bHedge;					//헤지종목여부

		m_nIDArray.RemoveAll();		
		m_nIDArray.Append(data.m_nIDArray);

		for(int i = 0 ; i < HOGA_CNT ; i++)
		{
			m_BidArray[i] = data.m_BidArray[i];
			m_OfferArray[i]=data.m_OfferArray[i];
		}
		m_TotalBid = data.m_TotalBid;
		m_TotalOffer = data.m_TotalOffer;

		for(i = 0 ; i < m_ContractArray.GetSize(); i++) delete m_ContractArray.GetAt(i);
		m_ContractArray.RemoveAll();

		for(i = 0 ; i < data.m_ContractArray.GetSize(); i++) 
		{
			CContractSet * pNewSet = new CContractSet;
			*pNewSet = *data.m_ContractArray.GetAt(i);
			m_ContractArray.Add(pNewSet);
		}

		m_strCurPrice2		= data.m_strCurPrice2;				//현재가
		m_strStartPrice2	= data.m_strStartPrice2;			//시가
		m_strHighPrice2		= data.m_strHighPrice2;			//고가
		m_strLowPrice2		= data.m_strLowPrice2;				//저가
		m_strVolume2		= data.m_strVolume2;				//거래량
		m_strMoney2			= data.m_strMoney2;
		m_strPrevPrice2		= data.m_strPrevPrice2;			//전일종가	
		m_strPrevVolume2	= data.m_strPrevVolume2;			//전일거래량
		m_strUnderlyingAssetsCode = data.m_strUnderlyingAssetsCode;	//기초자산 코드.
		
	
		m_pTheoryForMargin = data.m_pTheoryForMargin;		
		m_strTPrice = data.m_strTPrice;

		
		m_strSaveAvePrice = data.m_strSaveAvePrice;
		m_strOrderEnableCount	= data.m_strOrderEnableCount;
		m_strRemainItemCount	= data.m_strRemainItemCount;
		m_strCoveredCallRemainItemCount = data.m_strCoveredCallRemainItemCount;
		m_strCoveredCallEnableCount = data.m_strCoveredCallEnableCount;

		return *this;
	}

	//시세데이터만 복사하는 용도.
	void CopySiseData(CDataSet & data)
	{
		m_strCode			= data.m_strCode;
		m_strName			= data.m_strName;
		m_strStrike			= data.m_strStrike;
		m_strCurPrice		= data.m_strCurPrice;
		m_strBid			= data.m_strBid;
		m_strOffer			= data.m_strOffer;
		m_strVolume			= data.m_strVolume;
		m_strMoney			= data.m_strMoney;
		m_strBidVol			= data.m_strBidVol;
		m_strOfferVol		= data.m_strOfferVol;
		m_strUnitVol		= data.m_strUnitVol;
		m_strLastPrice		= data.m_strLastPrice;
		m_strRemainDay		= data.m_strRemainDay;
		m_strLastTradeDate	= data.m_strLastTradeDate;
		m_strHistoricalVolate=data.m_strHistoricalVolate;
		m_strCDRate			= data.m_strCDRate;
		m_strIndex			= data.m_strIndex;

		m_strCurPrice2		= data.m_strCurPrice2;				//현재가
		m_strStartPrice2	= data.m_strStartPrice2;			//시가
		m_strHighPrice2		= data.m_strHighPrice2;			//고가
		m_strLowPrice2		= data.m_strLowPrice2;				//저가
		m_strVolume2		= data.m_strVolume2;				//거래량
		m_strMoney2			= data.m_strMoney2;
		m_strPrevPrice2		= data.m_strPrevPrice2;			//전일종가	
		m_strPrevVolume2	= data.m_strPrevVolume2;			//전일거래량

//		m_strFlag			= data.m_strFlag;
		m_strNotTradeVolume = data.m_strNotTradeVolume;
		m_strTPriceFromServer=data.m_strTPriceFromServer;		//서버에서 받은 이론가
		m_strIVFromServer	=data.m_strIVFromServer;			//서버에서 받은 내재변동성
		m_strDeltaFromServer=data.m_strDeltaFromServer;			//서버에서 받은 델타
		m_strGammaFromServer=data.m_strGammaFromServer;			//서버에서 받은 감마
		m_strThetaFromServer=data.m_strThetaFromServer;			//서버에서 받은 세타
		m_strVegaFromServer	=data.m_strVegaFromServer;			//서버에서 받은 베가

		
		for(int i = 0 ; i < HOGA_CNT ; i++)
		{
			m_BidArray[i] = data.m_BidArray[i];
			m_OfferArray[i]=data.m_OfferArray[i];
		}
		m_TotalBid		= data.m_TotalBid;
		m_TotalOffer	= data.m_TotalOffer;


		for(i = 0 ; i < m_ContractArray.GetSize(); i++) delete m_ContractArray.GetAt(i);
		m_ContractArray.RemoveAll();

		for(i = 0 ; i < data.m_ContractArray.GetSize(); i++) 
		{
			CContractSet * pNewSet = new CContractSet;
			*pNewSet = *data.m_ContractArray.GetAt(i);
			m_ContractArray.Add(pNewSet);
		}
	}
};

typedef CArray < CDataSet *, CDataSet *> CDataArray;
typedef CList  < CDataSet *, CDataSet *> CDataList;

class CCodeDataSet
{
public:
	CString m_strLastTradeDate;		//최종거래일
	CString m_strCodeInfo;			//코드정보(년,월)
	CString m_strRemainDay;			//잔존일
	CString m_strWorkRemainDay;		//영업잔존일
	CString m_strIndex;				//배당액지수

public:

	CCodeDataSet & operator=(CCodeDataSet & data)
	{
		m_strLastTradeDate	= data.m_strLastTradeDate;
		m_strCodeInfo		= data.m_strCodeInfo;			//코드정보(년,월)
		m_strRemainDay		= data.m_strRemainDay;			//잔존일
		m_strIndex			= data.m_strIndex;				//배당액지수
		return *this;
	}
};
typedef CArray <CCodeDataSet *, CCodeDataSet *> CCodeDataArray;



class CTradeDateInfo
{
public:
	CString m_strDate;		//영업일자
	int		m_nTradeDate;	//영업일수
};
typedef CArray < CTradeDateInfo * ,CTradeDateInfo *> CTradeDateInfoArray;



//대우만의 특수한 케이스....
class COptIVInfo
{
public:
	CString m_strUnderlyingAssetsCode;
	CString m_strIV;//평균내재변동성->이론가,민감도 구할때 기본적으로 사용되는 변동성이다.(역사적변동성을 사용하지 않는다)
};
typedef CArray < COptIVInfo *, COptIVInfo *> COptIVInfoArray;

class DATASET_DECLSPEC CVolateInfo
{
public:
	CString m_strUnderlyingAssetsShortCode;
    CString m_strIV;
	CString m_strHisVolate30;
	CString m_strHisVolate60;
	CString m_strHisVolate90;    
	
	CString m_strTradeUnit;

public:

	CVolateInfo & operator=(CVolateInfo & volate)
	{
		m_strUnderlyingAssetsShortCode  = volate.m_strUnderlyingAssetsShortCode;
		m_strIV							= volate.m_strIV;
		m_strHisVolate30				= volate.m_strHisVolate30;
		m_strHisVolate60				= volate.m_strHisVolate60;
		m_strHisVolate90				= volate.m_strHisVolate90;
		m_strTradeUnit					= volate.m_strTradeUnit;
		return *this;
	}
};
typedef CArray < CVolateInfo * , CVolateInfo *> CVolateInfoArray;


class CBaseDataSet
{
public:
	CBaseDataSet(){m_pRTDataArray = NULL;}

	~CBaseDataSet()
	{
		int i, nSize = m_TradeDateInfoArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_TradeDateInfoArray.GetAt(i);
		m_TradeDateInfoArray.RemoveAll();

		nSize = m_CodeDataArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_CodeDataArray.GetAt(i);
		m_CodeDataArray.RemoveAll();

		nSize = m_VolateInfoArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_VolateInfoArray.GetAt(i);
		m_VolateInfoArray.RemoveAll();		
	}

	double GetOptIV(CString strCode)
	{
		if(strCode.GetLength() > 6 && (strCode.Left(1) == "2" || strCode.Left(1) == "3"))
		{
			for(int i = 0 ; i < m_VolateInfoArray.GetSize() ; i++)
			{
				CVolateInfo * pOpt = m_VolateInfoArray.GetAt(i);
				if(strCode.Mid(1,2) == pOpt->m_strUnderlyingAssetsShortCode)
					return atof(pOpt->m_strIV);
			}
		}
		return 0.;
	}


	CBaseDataSet & operator=(CBaseDataSet & set)
	{
		m_strCDRate					= set.m_strCDRate;

		m_strHistoricalVolate90		= set.m_strHistoricalVolate90;

		m_strHistoricalVolate60		= set.m_strHistoricalVolate60;

		m_strHistoricalVolate30		= set.m_strHistoricalVolate30;

		m_strTodayDate				= set.m_strTodayDate;	


		int i;
		for(i = 0 ; i < m_VolateInfoArray.GetSize(); i++) delete m_VolateInfoArray.GetAt(i);
		m_VolateInfoArray.RemoveAll();
		for(i = 0 ; i < set.m_VolateInfoArray.GetSize(); i++)
		{
			CVolateInfo * pVol    = set.m_VolateInfoArray.GetAt(i);
			CVolateInfo * pNewVol = new CVolateInfo;
			*pNewVol = *pVol;
			m_VolateInfoArray.Add(pNewVol);
		}
		
		for(i = 0 ; i < m_CodeDataArray.GetSize(); i++) delete m_CodeDataArray.GetAt(i);
		m_CodeDataArray.RemoveAll();
		for(i = 0 ; i < set.m_CodeDataArray.GetSize(); i++)
		{
			CCodeDataSet * pSet = set.m_CodeDataArray.GetAt(i);
			CCodeDataSet * pNewSet = new CCodeDataSet;
			*pNewSet = *pSet;
			m_CodeDataArray.Add(pNewSet);
		}


		for(i = 0 ; i < m_TradeDateInfoArray.GetSize(); i++) delete m_TradeDateInfoArray.GetAt(i);
		m_TradeDateInfoArray.RemoveAll();
		for(i = 0 ; i < set.m_TradeDateInfoArray.GetSize(); i++)
		{
			CTradeDateInfo * pSet = set.m_TradeDateInfoArray.GetAt(i);
			CTradeDateInfo * pNewSet = new CTradeDateInfo;
			*pNewSet = *pSet;
			m_TradeDateInfoArray.Add(pNewSet);
		}		

		for(i = 0 ; i < m_TradeDateInfoArray.GetSize(); i++) delete m_TradeDateInfoArray.GetAt(i);
		m_TradeDateInfoArray.RemoveAll();
		for(i = 0 ; i < set.m_TradeDateInfoArray.GetSize(); i++)
		{
			CTradeDateInfo * pSet = set.m_TradeDateInfoArray.GetAt(i);
			CTradeDateInfo * pNewSet = new CTradeDateInfo;
			*pNewSet = *pSet;
			m_TradeDateInfoArray.Add(pNewSet);
		}		

		if(m_pRTDataArray)
		{		
			for(i = 0 ; i < m_pRTDataArray->GetSize(); i++) delete m_pRTDataArray->GetAt(i);
			delete m_pRTDataArray;
			m_pRTDataArray = NULL;
		}

		if(set.m_pRTDataArray)
		{		
			m_pRTDataArray = new CDataArray;
			for(i = 0 ; i < set.m_pRTDataArray->GetSize(); i++)
			{
				CDataSet * pSet = set.m_pRTDataArray->GetAt(i);
				CDataSet* pNewSet = new CDataSet;
				*pNewSet = *pSet;
				m_pRTDataArray->Add(pNewSet);
			}	
		}
		return *this;
	}

public:

	CString				m_strCDRate;

	CString				m_strHistoricalVolate90;

	CString				m_strHistoricalVolate60;

	CString				m_strHistoricalVolate30;

	CString				m_strATM;

	CVolateInfoArray	m_VolateInfoArray;
	
	CCodeDataArray		m_CodeDataArray;

	CTradeDateInfoArray	m_TradeDateInfoArray;

	CString				m_strTodayDate;

	CDataArray	*		m_pRTDataArray;
};

class DATASET_DECLSPEC CItemDataInfo
{
public:
	
	CString m_strCode;
	CString m_strName;
	CString m_strStrike;
/////////////////////////////////
	CString m_strCurPrice;	//현재가
	CString m_strBid;		//매수호가
	CString m_strOffer;		//매도호가
	CString m_strVolume;	//거래량
	CString m_strLastPrice; //전일종가
	CString m_strOfferVol;
	CString m_strBidVol;
//	CString m_strFlag;

	DWORD m_dwReserved;//주로 포인터를 저장할 때 사용하려 한다.(예 : CFONetworkModule)

public:

	CItemDataInfo()
	{
		m_dwReserved = 0;
	}

	CItemDataInfo & operator=(CItemDataInfo & info)
	{
		m_strCode = info.m_strCode;
		m_strName = info.m_strName;
		m_strStrike=info.m_strStrike;

		m_strCurPrice	= info.m_strCurPrice;
		m_strBid		= info.m_strBid;
		m_strOffer		= info.m_strOffer;
		m_strVolume		= info.m_strVolume;
		m_strLastPrice  = info.m_strLastPrice;
		m_strOfferVol	= info.m_strOfferVol;
		m_strBidVol		= info.m_strBidVol;
//		m_strFlag		= info.m_strFlag;
		m_dwReserved	= info.m_dwReserved;	

		return *this;
	}
};
typedef CArray < CItemDataInfo *, CItemDataInfo *> CItemInfoArray;

class DATASET_DECLSPEC COptionMonthInfo
{
public:
	CString m_strYearMonth;
	CString m_strShortYearMonth;
	CItemInfoArray	m_CallOptionInfoArray;
	CItemInfoArray	m_PutOptionInfoArray;

public:
	COptionMonthInfo(){;}

	~COptionMonthInfo()
	{
		int i, nSize = m_CallOptionInfoArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_CallOptionInfoArray.GetAt(i);
		m_CallOptionInfoArray.RemoveAll();

		nSize = m_PutOptionInfoArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_PutOptionInfoArray.GetAt(i);
		m_PutOptionInfoArray.RemoveAll();
	}

	COptionMonthInfo & operator=(COptionMonthInfo & month)
	{
		m_strYearMonth = month.m_strYearMonth;
		int i,nSize = m_CallOptionInfoArray.GetSize();
		for(i = 0 ; i < nSize ;i ++) delete m_CallOptionInfoArray.GetAt(i);
		m_CallOptionInfoArray.RemoveAll();
		
		nSize = m_PutOptionInfoArray.GetSize();
		for(i = 0 ; i < nSize ; i++) delete m_PutOptionInfoArray.GetAt(i);
		m_PutOptionInfoArray.RemoveAll();

		nSize = month.m_CallOptionInfoArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			CItemDataInfo * pOption = month.m_CallOptionInfoArray.GetAt(i);
			CItemDataInfo * pNewOption = new CItemDataInfo;
			*pNewOption = *pOption;
			m_CallOptionInfoArray.Add(pNewOption);
		}

		nSize = month.m_PutOptionInfoArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			CItemDataInfo * pOption = month.m_PutOptionInfoArray.GetAt(i);
			CItemDataInfo * pNewOption = new CItemDataInfo;
			*pNewOption = *pOption;
			m_PutOptionInfoArray.Add(pNewOption);
		}
		return *this;
	}
};
typedef CArray < COptionMonthInfo *, COptionMonthInfo *> COptionMonthInfoArray;




class DATASET_DECLSPEC CUnderlyingAssetsInfo
{
public:
	
	CString m_strShortCode; //국민은행->10, 삼성전자->11, .....
	CString m_strUnderlyingAssetsCode;
	CString m_strUnderlyingAssetsName;
	CString m_strTradeUnit;	

	COptionMonthInfoArray	m_MonthInfoArray;

	DWORD	m_dwReserved;
	
	CItemInfoArray			m_ItemArray; //기초자산이 kp200인 경우는 선물4개월물저장
	//주식인 경우는 주식을 저장...

public:

	CUnderlyingAssetsInfo()
	{
		m_dwReserved = 0;
	}

	~CUnderlyingAssetsInfo()
	{
		int i, nSize = m_MonthInfoArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_MonthInfoArray.GetAt(i);
		m_MonthInfoArray.RemoveAll();
		
		nSize = m_ItemArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_ItemArray.GetAt(i);
		m_ItemArray.RemoveAll();
	}

	CUnderlyingAssetsInfo & operator=(CUnderlyingAssetsInfo & under)
	{
		m_strShortCode = under.m_strShortCode;
		m_strUnderlyingAssetsCode = under.m_strUnderlyingAssetsCode;
		m_strUnderlyingAssetsName = under.m_strUnderlyingAssetsName;
		m_strTradeUnit			  = under.m_strTradeUnit;

		int i,nSize = m_MonthInfoArray.GetSize();
		for(i = 0 ; i < nSize ;i ++)		
			delete m_MonthInfoArray.GetAt(i);
		m_MonthInfoArray.RemoveAll();

		nSize = m_ItemArray.GetSize();
		for(i = 0 ; i < nSize ;i ++)		
			delete m_ItemArray.GetAt(i);
		m_ItemArray.RemoveAll();

		nSize = under.m_MonthInfoArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			COptionMonthInfo * pMonth = under.m_MonthInfoArray.GetAt(i);
			COptionMonthInfo * pNewMonth = new COptionMonthInfo;
			*pNewMonth = *pMonth;
			m_MonthInfoArray.Add(pNewMonth);
		}

		nSize = under.m_ItemArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			CItemDataInfo * pItem = under.m_ItemArray.GetAt(i);
			CItemDataInfo * pNewItem = new CItemDataInfo;
			*pNewItem = *pItem ;
			m_ItemArray.Add(pNewItem);
		}

		m_dwReserved = under.m_dwReserved;
		return *this;
	}
};
typedef CArray < CUnderlyingAssetsInfo *, CUnderlyingAssetsInfo *> CUnderlyingAssetsInfoArray;


//사용자 전략 저장용
class DATASET_DECLSPEC CStrategyData
{
public:
	
	CString m_strATMPos;		// ATM 에서의 거리
	CString m_strMonth;			// 근월 0, 차근월 1, 차차근월 2, 차차차근월 3
	CString m_strCallPut;		// Call 0, Put 1
	CString m_strTradeType;		// 매도 0, 매수 1
	CString m_strQty;			// 수량
	CString m_strCovedCall;		// 커버드 콜 수량
	CString m_strBase;			// 전략가이드에서 사용 기본 진입 종목인지 대안인지 판별..
	DWORD	m_dwReserved;//주로 포인터를 저장할 때 사용하려 한다.(예 : CFONetworkModule)

public:

	CStrategyData()
	{
		m_dwReserved = 0;
	}

	CStrategyData & operator=(CStrategyData & info)
	{
		m_strATMPos		= info.m_strATMPos;
		m_strMonth		= info.m_strMonth;
		m_strCallPut	= info.m_strCallPut;
		m_strTradeType	= info.m_strTradeType;
		m_strQty		= info.m_strQty;
		m_strCovedCall	= info.m_strCovedCall;
		m_strBase		= info.m_strBase;

		m_dwReserved	= info.m_dwReserved;	

		return *this;
	}
};
typedef CArray < CStrategyData *, CStrategyData *> CStrategyDataArray;

class DATASET_DECLSPEC CStrategyItemInfo
{
public:
	CString	m_strStrategyName;				// 전략명
	CStrategyDataArray m_StrategyDataArray;	// 전략 데이타
	CString m_strStrategyMsg;				// 전략 설명

public:
	CStrategyItemInfo(){;}

	~CStrategyItemInfo()
	{
		int i, nSize = m_StrategyDataArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_StrategyDataArray.GetAt(i);
		m_StrategyDataArray.RemoveAll();
	}

	CStrategyItemInfo & operator=(CStrategyItemInfo & StrategyName)
	{
		m_strStrategyName = StrategyName.m_strStrategyName;
		m_strStrategyMsg = StrategyName.m_strStrategyMsg;

		int i,nSize = m_StrategyDataArray.GetSize();
		for(i = 0 ; i < nSize ;i ++) delete m_StrategyDataArray.GetAt(i);
		m_StrategyDataArray.RemoveAll();
		
		nSize = StrategyName.m_StrategyDataArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			CStrategyData * pData = StrategyName.m_StrategyDataArray.GetAt(i);
			CStrategyData * pNewData = new CStrategyData;
			*pNewData = *pData;
			m_StrategyDataArray.Add(pNewData);
		}
		return *this;
	}
};
typedef CArray < CStrategyItemInfo *, CStrategyItemInfo *> CStrategyItemInfoArray;

class DATASET_DECLSPEC CStrategyGroup
{
public:
	CString m_strGroupName;						// 그룹명
	CStrategyItemInfoArray m_StrategyItemArray;	// 그룹의 전략

public:
	CStrategyGroup(){;}

	~CStrategyGroup()
	{
		int i, nSize = m_StrategyItemArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_StrategyItemArray.GetAt(i);
		m_StrategyItemArray.RemoveAll();
	}

	CStrategyGroup & operator=(CStrategyGroup & GroupName)
	{
		m_strGroupName = GroupName.m_strGroupName;

		int i,nSize = m_StrategyItemArray.GetSize();
		for(i = 0 ; i < nSize ;i ++) delete m_StrategyItemArray.GetAt(i);
		m_StrategyItemArray.RemoveAll();
		
		nSize = GroupName.m_StrategyItemArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			CStrategyItemInfo * pStrategy = GroupName.m_StrategyItemArray.GetAt(i);
			CStrategyItemInfo * pNewStrategy = new CStrategyItemInfo;
			*pNewStrategy = *pStrategy;
			m_StrategyItemArray.Add(pNewStrategy);
		}
		return *this;
	}
};
typedef CArray < CStrategyGroup *, CStrategyGroup *> CStrategyGroupArray;

////////////////////////////////////////////////////////
// 잔고..
/////////////////////////////////////////////////////////
//선물옵션관련 계좌.
class DATASET_DECLSPEC CRemainDataSet 
{
public:
	CString m_strCode;					//종목코드	
	CString m_strName;					//종목명
	CString m_strTradeType;				//매도 (1), 매수(2);
	CString m_strRemainCount;			//잔고수량
	CString m_strOrderEnableCount;		//청산가능수량.
	CString m_strAvePrice;				//평균단가
	CString m_strCreditType;			//신용구분 (일반-0, 융자-1, 대주-2)
	CString m_strCoveredRemainCount;	//커버드 잔고수량
	CString m_strCoveredOrderEnableCount;//커버드 청산가능수량
	CString m_strUnderlyingAssets;		//기초자산
	CString m_strAveAmount;				//매입금액(정용호 추가 20080407)

	CDataSet m_DataSet;

public:
	CRemainDataSet & operator=(CRemainDataSet & data)
	{
		m_strCode				= data.m_strCode;
		m_strName				= data.m_strName;
		m_strTradeType			= data.m_strTradeType;
		m_strRemainCount		= data.m_strRemainCount;
		m_strOrderEnableCount	= data.m_strOrderEnableCount;
		m_strAvePrice			= data.m_strAvePrice;
		m_strCreditType			= data.m_strCreditType;
		m_strCoveredRemainCount = data.m_strCoveredRemainCount;
		m_strCoveredOrderEnableCount = data.m_strCoveredOrderEnableCount;
		m_strUnderlyingAssets   = data.m_strUnderlyingAssets;
		m_strAveAmount			= data.m_strAveAmount;//매입금액(정용호 추가 20080407)
		m_DataSet				= data.m_DataSet;
		return *this;
	}
};
typedef CArray< CRemainDataSet *, CRemainDataSet *> CRemainDataArray;

class DATASET_DECLSPEC CAccountInfo
{
public:	

	//선물/옵션...
	CString				m_strFOAccountName;		//계좌명
	CString				m_strFOOrderEnableTotal;	//주문가능총액
	CString				m_strFOOrderEnableMoney;	//주문가능현금
	CString				m_strFOOrderRealBuyMoney;	//순매수금액
	CRemainDataArray	m_FORemainDataArray;

	//주식..
	CString				m_strSAccountName;
	CRemainDataArray	m_SRemainDataArray;

public:
	CAccountInfo(){;}
	~CAccountInfo()
	{
		DeleteArray();
	}

	void DeleteArray()
	{
		int i, nSize = m_FORemainDataArray.GetSize();
		for(i = 0 ; i < nSize ; i++)		
			delete m_FORemainDataArray.GetAt(i);
		m_FORemainDataArray.RemoveAll();

		nSize = m_SRemainDataArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_SRemainDataArray.GetAt(i);
		m_SRemainDataArray.RemoveAll();
	}

	CAccountInfo & operator=(CAccountInfo & account)
	{
		m_strFOAccountName = account.m_strFOAccountName;
		m_strFOOrderEnableTotal = account.m_strFOOrderEnableTotal;
		m_strFOOrderEnableMoney = account.m_strFOOrderEnableMoney;
		m_strFOOrderRealBuyMoney= account.m_strFOOrderRealBuyMoney;

		m_strSAccountName       = account.m_strSAccountName;
		
		int i,nSize = m_FORemainDataArray.GetSize();
		for(i = 0 ; i < nSize ; i++)		
			delete m_FORemainDataArray.GetAt(i);
		m_FORemainDataArray.RemoveAll();

		nSize = account.m_FORemainDataArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			CRemainDataSet * pDataSet = account.m_FORemainDataArray.GetAt(i);
			CRemainDataSet * pNewDataSet = new CRemainDataSet;
			*pNewDataSet = *pDataSet;
			m_FORemainDataArray.Add(pNewDataSet);
		}
		
		nSize = m_SRemainDataArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_SRemainDataArray.GetAt(i);
		m_SRemainDataArray.RemoveAll();

		nSize = account.m_SRemainDataArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			CRemainDataSet * pDataSet = account.m_SRemainDataArray.GetAt(i);
			CRemainDataSet * pNewDataSet = new CRemainDataSet;
			*pNewDataSet = *pDataSet;
			m_SRemainDataArray.Add(pNewDataSet);
		}

		return *this;
	}
};

// SJS 수정 (2005.09.06)
/******************* 증거금 변경관련 이론가 구간변경 ************************/
// 위탁 증거금 : 11구간 -> 31구간
// 현금 증거금 : 11구간 -> 21구간
/****************************************************************************/
#define DANGER_THEORY_COUNT	31
#define	HAVE_THEORY_COUNT	21

#define	THEORY_COUNT 11

class DATASET_DECLSPEC CTheoryForMargin
{
public:
	int				m_nDataType;			//전일데이터-1, 당일 - 2
	
	int				m_nID;				//아이디.

	CString			m_strCode;
	CString			m_strUnit;
	CString			m_strOffSet;	
	CString			m_strOffSet1;
	CString			m_strAdjSect;		//
	CString			m_strProductID;
	CString			m_strProductID1;
	CString			m_strUnderlyingAssetsShortCode;
	CString			m_strStrike;
	CString			m_strAdjPractice;
	

	double m_dDangerTheoryArray[DANGER_THEORY_COUNT];
	double m_dHaveTheoryArray[HAVE_THEORY_COUNT];

	double m_dDangerTheoryArrayNext[DANGER_THEORY_COUNT];
	double m_dHaveTheoryArrayNext[HAVE_THEORY_COUNT];

public:

	CTheoryForMargin(){;}

	CTheoryForMargin & operator=(CTheoryForMargin & theory)
	{
		m_nDataType = theory.m_nDataType;
		m_strCode   = theory.m_strCode;
		m_nID		= theory.m_nID;
		m_strUnit	= theory.m_strUnit;
		m_strOffSet = theory.m_strOffSet;
		m_strOffSet1= theory.m_strOffSet1;
		m_strAdjSect= theory.m_strAdjSect;
		m_strProductID=theory.m_strProductID;
		m_strProductID1=theory.m_strProductID1;
		m_strUnderlyingAssetsShortCode = theory.m_strUnderlyingAssetsShortCode;
		m_strStrike = theory.m_strStrike;
		m_strAdjPractice = theory.m_strAdjPractice;
		
		memcpy(m_dDangerTheoryArray, theory.m_dDangerTheoryArray, sizeof(m_dDangerTheoryArray));
		memcpy(m_dHaveTheoryArray,   theory.m_dHaveTheoryArray,   sizeof(m_dHaveTheoryArray));

		memcpy(m_dDangerTheoryArrayNext, theory.m_dDangerTheoryArrayNext, sizeof(m_dDangerTheoryArrayNext));
		memcpy(m_dHaveTheoryArrayNext, theory.m_dHaveTheoryArrayNext, sizeof(m_dHaveTheoryArrayNext));

		return *this;
	}
};
typedef CArray < CTheoryForMargin *, CTheoryForMargin *> CTheoryForMarginArray;



class CItemSet
{
public:
	CString m_strCode;
	CString m_strName;
	CString m_strCount;
	CString m_strTradeType;
	CString m_strAvePrice;
public:
	CItemSet & operator=(CItemSet & item)
	{
		m_strCode = item.m_strCode;
		m_strName = item.m_strName;
		m_strCount= item.m_strCount;
		m_strTradeType = item.m_strTradeType;
		m_strAvePrice = item.m_strAvePrice;
		return *this;
	}
};
typedef CArray < CItemSet *, CItemSet *> CItemArray;


#define	NEWADD						1	//추가
#define	APPEND						2	//적용

class CPassDataSet
{
public:

	CString		m_strAccount;

	CString		m_strPassword;

	CString		m_strUnderlyingAssetsCode;

	CString		m_strUnderlyingAssetsName;	

	int			m_nMode;			//추가 모드 : NEWADD, 적용모드 : APPEND

	LONGLONG	m_lProfit;			//확정손익

	CDataArray	m_DataArray;	

    CString     m_strUnderlyingStartPrice;      // KOSPI200 시작 지수
	CString		m_strUnderlyingEndPrice;		// KOSPI200 끝 지수
	
public:
	CPassDataSet(){	m_lProfit = 0, m_nMode = NEWADD;}

	~CPassDataSet()
	{
		for(int i = 0 ; i < m_DataArray.GetSize();  i++) delete m_DataArray.GetAt(i);		
	}
	
	CPassDataSet & operator=(CPassDataSet & data)
	{
		m_strAccount				= data.m_strAccount;
		m_strPassword				= data.m_strPassword;
		m_strUnderlyingAssetsCode	= data.m_strUnderlyingAssetsCode;
		m_strUnderlyingAssetsName	= data.m_strUnderlyingAssetsName;
		m_nMode						= data.m_nMode;
		m_lProfit					= data.m_lProfit;
		
		int i ; 
		for(i = 0 ; i < m_DataArray.GetSize(); i++) delete m_DataArray.GetAt(i);
		m_DataArray.RemoveAll();

		for(i = 0 ; i < data.m_DataArray.GetSize(); i++)
		{
			CDataSet * pDataSet = data.m_DataArray.GetAt(i);
			CDataSet * pNewSet  = new CDataSet ;
			*pNewSet = *pDataSet;
			m_DataArray.Add(pNewSet);
		}
		return *this;
	}
};

class CUserFileSaveData
{
public:
	CString m_strSaveName;

	CPassDataSet m_PassDataSet;
public:

	CUserFileSaveData(){;}

	CUserFileSaveData & operator=(CUserFileSaveData & item)
	{
		m_strSaveName	= item.m_strSaveName;
		
		m_PassDataSet	= item.m_PassDataSet;
		return *this;
	}
};
typedef CArray < CUserFileSaveData *, CUserFileSaveData *> CUserFileSaveDataArray;

class DATASET_DECLSPEC CUserDataFile
{
public:
	CString m_strGroupName;						// 그룹명
	CUserFileSaveDataArray m_UserFileDataArray;	// 그룹의 파일

public:
	CUserDataFile(){;}

	~CUserDataFile()
	{
		int i, nSize = m_UserFileDataArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
			delete m_UserFileDataArray.GetAt(i);
		m_UserFileDataArray.RemoveAll();
	}

	CUserDataFile & operator=(CUserDataFile & GroupName)
	{
		m_strGroupName = GroupName.m_strGroupName;

		int i,nSize = m_UserFileDataArray.GetSize();
		for(i = 0 ; i < nSize ;i ++) delete m_UserFileDataArray.GetAt(i);
		m_UserFileDataArray.RemoveAll();
		
		nSize = GroupName.m_UserFileDataArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			CUserFileSaveData * pStrategy = GroupName.m_UserFileDataArray.GetAt(i);
			CUserFileSaveData * pNewStrategy = new CUserFileSaveData;
			*pNewStrategy = *pStrategy;
			m_UserFileDataArray.Add(pNewStrategy);
		}
		return *this;
	}
};
typedef CArray < CUserDataFile *, CUserDataFile *> CUserDataFileArray;

#define	THEORY_SETUP_UNDERLYING_FOCUS	1

class CSetupDataSet
{
public:

	int	m_nAvePrice; // 최초입력값 유지 - 0, 자동갱신 - 1
	
	int	m_nGreek;	 //수량적용하지 않음 - 0, 입력한 수량으로 계산 - 1
	
	int m_nProfitType; //현재가 대비 - 0, 이론가 대비 - 1, 만기가 대비 - 2
	
	int m_nAddItem;    //1종목씩 추가 - 0, 복수종목 추가 - 1
	
	int m_nAddSameItemType; // 새로운 줄로 추가 - 0, 평균가, 수량합산 후 한 줄로 표시-1
	
	BOOL m_bAddCommission;  //수수료를 손익에 반영
	
	double m_dFutureCom;	//선물 수수료율(%단위)
	
	double m_dOptionCom;	//옵션 수수료율(%단위)
	
	double	m_dStockCom;    //주식 수수료율(%단위)

	BOOL	m_bSort;//정렬여부.

	int		m_nSort;	//유가증권별/월물/행사가별 - 0
						//유가증권별/행사가별/월물 - 1
						//월물/유가증권별/행사가별 - 2
						//월물/행사가별/유가증권별 - 3
						//행사가별/유가증권별/월물 - 4
						//행사가별/월물/유가증권별 - 5

	int		m_nSortType; //오름차순-0, 내림차순-1

	double	m_dUnderlyingAssetsMin; //기초자산 최소값

	double	m_dUnderlyingAssetsMax; //기초자산 최대값

	int		m_nYRange; // 자동설정 - 0, 수동설정 - 1

	double	m_dYMax;   // 최대y값

	double	m_dYMin;	//최소y값

	int		m_nTradeInputChange; //매도 / 매수 입력 방식  0 - 콤보박스 , 1 - 토글방식

public:

	CSetupDataSet()
	{
		m_bSort = FALSE;
	}

	CSetupDataSet & operator=(CSetupDataSet & data)
	{
		m_nAvePrice			= data.m_nAvePrice;		
		m_nGreek			= data.m_nGreek;
		m_nProfitType		= data.m_nProfitType;
		m_nAddItem			= data.m_nAddItem;
		m_nAddSameItemType	= data.m_nAddSameItemType;
		m_bAddCommission	= data.m_bAddCommission;
		m_dFutureCom		= data.m_dFutureCom;
		m_dOptionCom		= data.m_dOptionCom;
		m_dStockCom			= data.m_dStockCom;
		m_bSort				= data.m_bSort;
		m_nSort				= data.m_nSort;
		m_nSortType			= data.m_nSortType;
		m_dUnderlyingAssetsMin=data.m_dUnderlyingAssetsMin;
		m_dUnderlyingAssetsMax=data.m_dUnderlyingAssetsMax;
		m_nYRange			= data.m_nYRange;
		m_dYMax				= data.m_dYMax;
		m_dYMin				= data.m_dYMin;
		m_nTradeInputChange = data.m_nTradeInputChange;
		return *this;
	}
};

// Add By LJP 20080123 Start -->
class CSetupTheorySet
{
public:
	int		m_nBasicPrice;	///< 0 : KOSPI200지수 - 배당지수, 1 : 선물최근월물, 2: 자동계산, 3 : 가중
	
	int		m_nKOSPI200;	///< 가중 KOSPI200 지수
	int		m_nFuture;		///< 가중 선물

	int		m_nIVType;		///< 0 : 총평균 내재변동성, 1 : 역사적변동성 90일, 2 : 내재변동성, 3 : 사용자선택

	double	m_dCallIV;		///< 사용자정의 콜 변동성	
	double	m_dPutIV;		///< 사용자정의 풋 변동성
	
	
	int		m_nRemainDay;	///< 0 : 거래일수 기준, 1 : 달력일수 기준
	BOOL	m_bTimeValue;	///< 장중 시간가치 감소를 반영
	CString	m_strFromTime;	///< 시간가치 감소시간대 시작 시간
	CString	m_strToTime;	///< 시간가치 감소시간대 끝 시간
	int		m_nFocusIndex;//기준가 버튼을 클릭시 환경설정창에 포커스가 이론가탭에 기준가로 가도록 한다.(m_nFocusIndex -> 1)

public:

	CSetupTheorySet() {m_nFocusIndex = 0;}

	CSetupTheorySet & operator=(CSetupTheorySet & data)
	{
		m_nBasicPrice	= data.m_nBasicPrice;
		m_nKOSPI200		= data.m_nKOSPI200;
		m_nFuture		= data.m_nFuture;
		m_nIVType		= data.m_nIVType;
		m_dCallIV		= data.m_dCallIV;
		m_dPutIV		= data.m_dPutIV;
		m_nRemainDay	= data.m_nRemainDay;
		m_bTimeValue	= data.m_bTimeValue;
		m_strFromTime	= data.m_strFromTime;
		m_strToTime		= data.m_strToTime;
		m_nFocusIndex = data.m_nFocusIndex;
		return *this;
	}

	BOOL LoadTheoryInitFile(CString strUserPath)
	{		
		CString sFileName;
		TCHAR Temp[100];		

		sFileName.Format("%s\\%s", strUserPath, FILE_INI);
		
		GetPrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_01, "0", Temp, sizeof(Temp), sFileName); 
		m_nBasicPrice = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_01_1, "100", Temp, sizeof(Temp), sFileName); 
		m_nKOSPI200 = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_01_2, "0", Temp, sizeof(Temp), sFileName); 
		m_nFuture = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_02, "0", Temp, sizeof(Temp), sFileName); 
		m_nIVType = atoi(Temp);
		
		GetPrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_02_1, "0", Temp, sizeof(Temp), sFileName); 
		m_dCallIV = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_02_2, "0", Temp, sizeof(Temp), sFileName); 
		m_dPutIV = atoi(Temp);
		
		GetPrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_03, "0", Temp, sizeof(Temp), sFileName); 
		m_nRemainDay = atoi(Temp);
		
		GetPrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_04, "0", Temp, sizeof(Temp), sFileName); 
		m_bTimeValue = atoi(Temp);
		
		GetPrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_04_1, "0900", Temp, sizeof(Temp), sFileName); 
		m_strFromTime = Temp;
		
		GetPrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_04_2, "1515", Temp, sizeof(Temp), sFileName); 
		m_strToTime = Temp;

		return TRUE;
	}

	void SaveTheoryInitFile(CString strUserPath)
	{
		CString sFileName;
		CString sTemp;

		sFileName.Format("%s\\%s", strUserPath, FILE_INI);	

		sTemp.Format("%d", m_nBasicPrice);
		WritePrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_01, sTemp, sFileName);

		sTemp.Format("%d", m_nKOSPI200);
		WritePrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_01_1, sTemp, sFileName);

		sTemp.Format("%d", m_nFuture);
		WritePrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_01_2, sTemp, sFileName);

		sTemp.Format("%d", m_nIVType);
		WritePrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_02, sTemp, sFileName);

		sTemp.Format("%.4f", m_dCallIV);
		WritePrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_02_1, sTemp, sFileName);

		sTemp.Format("%.4f", m_dPutIV);
		WritePrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_02_2, sTemp, sFileName);

		sTemp.Format("%d", m_nRemainDay);
		WritePrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_03, sTemp, sFileName);

		sTemp.Format("%d", m_bTimeValue);
		WritePrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_04, sTemp, sFileName);

		sTemp.Format("%s", m_strFromTime);
		WritePrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_04_1, sTemp, sFileName);

		sTemp.Format("%s", m_strToTime);
		WritePrivateProfileString(SECTION_CONFIGURATION, THEORY_KEY_04_2, sTemp, sFileName);	
	}
};

class CSetupFOOrderSet
{
public:
	int		m_nPriceTypeSell;	///< 매도주문 가격구분(지정가-0, 시장가-1, 조건부지정가-2, 최유리지정가-3)
	int		m_nCPriceTypeSell;	///< 매도주문 단가구분(현재가-0, 매도호가-1, 매수호가-2, 상한가-3, 하한가-4)
	int		m_nTickSell;		///< 매도호가증감

	
	int		m_nPriceTypeBuy;	///< 매수주문 가격구분(지정가-0, 시장가-1, 조건부지정가-2, 최유리지정가-3)
	int		m_nCPriceTypeBuy;	///< 매수주문 단가구분(현재가-0, 매도호가-1, 매수호가-2, 상한가-3, 하한가-4)
	int		m_nTickBuy;			///< 매수호가증감 

public:
	CSetupFOOrderSet(){}

	CSetupFOOrderSet & operator=(CSetupFOOrderSet & data)
	{
		m_nPriceTypeSell	= data.m_nPriceTypeSell;
		m_nCPriceTypeSell	= data.m_nCPriceTypeSell;
		m_nTickSell			= data.m_nTickSell;
		m_nPriceTypeBuy		= data.m_nPriceTypeBuy;
		m_nCPriceTypeBuy	= data.m_nCPriceTypeBuy;
		m_nTickBuy			= data.m_nTickBuy;
		return *this;
	}

	BOOL LoadOrderInitFile(CString strUserPath)
	{		
		CString sFileName;
		TCHAR Temp[100];

		sFileName.Format("%s\\%s", strUserPath, FILE_INI);
		
		GetPrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_01, "0", Temp, sizeof(Temp), sFileName); 
		m_nPriceTypeSell = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_02, "1", Temp, sizeof(Temp), sFileName); 
		m_nCPriceTypeSell = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_03, "0", Temp, sizeof(Temp), sFileName); 
		m_nTickSell = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_04, "0", Temp, sizeof(Temp), sFileName); 
		m_nPriceTypeBuy = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_05, "1", Temp, sizeof(Temp), sFileName); 
		m_nCPriceTypeBuy = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_06, "0", Temp, sizeof(Temp), sFileName); 
		m_nTickBuy = atoi(Temp);

		return TRUE;
	}

	void SaveOrderInitFile(CString strUserPath)
	{
		CString sFileName;
		CString sTemp;		

		sFileName.Format("%s\\%s", strUserPath, FILE_INI);	

		sTemp.Format("%d", m_nPriceTypeSell);
		WritePrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_01, sTemp, sFileName);

		sTemp.Format("%d", m_nCPriceTypeSell);
		WritePrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_02, sTemp, sFileName);

		sTemp.Format("%d", m_nTickSell);
		WritePrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_03, sTemp, sFileName);

		sTemp.Format("%d", m_nPriceTypeBuy);
		WritePrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_04, sTemp, sFileName);

		sTemp.Format("%d", m_nCPriceTypeBuy);
		WritePrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_05, sTemp, sFileName);
		
		sTemp.Format("%d", m_nTickBuy);
		WritePrivateProfileString(SECTION_CONFIGURATION, ORDER_KEY_06, sTemp, sFileName);
	}
};

class CSetupSlippageZeroOrderSet
{
public:
	int		m_nFirstTrade;		///< 0 : 호가갭이 큰 종목, 1 : 현재가 높은 종목
	
	int		m_nFirstTradeSeq;	///< 0 : 모든 우선거래 동시주문, 1 : #1 Set의 우선거래부터 순차주문
	
	int		m_nFirstTradeSell;	///< 0 : 1차매도호가, 1 : 2차매도호가, 2 : 3차매도호가, 3 : 4차매도호가, 4 : 5차매도호가, 5 : 사용자지정가, 6 : 최유리지정가, 7 : 현재가, 8 : 1차매수호가, 9 : 2차매수호가, 10 : 3차매수호가, 11 : 4차매수호가, 12 : 5차매수호가, 13 : 시장가
	int		m_nFirstTradeBuy;	///< 0 : 1차매수호가, 1 : 2차매수호가, 2 : 3차매수호가, 3 : 4차매수호가, 4 : 5차매수호가, 5 : 사용자지정가, 6 : 최유리지정가, 7 : 현재가, 8 : 1차매도호가, 9 : 2차매도호가, 10 : 3차매도호가, 11 : 4차매도호가, 12 : 5차매도호가, 13 : 시장가
	
	int		m_nSecondTradeSell;	///< 0 : 1차매수호가, 1 : 2차매수호가, 2 : 3차매수호가, 3 : 4차매수호가, 4 : 5차매수호가, 5 : 시장가, 6 : 최유리지정가, 7 : 사용자지정가, 8 : 현재가, 9 : 1차매도호가, 10 : 2차매도호가, 11 : 3차매도호가, 12 : 4차매도호가, 13 : 5차매도호가
	int		m_nSecondTradeBuy;	///< 0 : 1차매도호가, 1 : 2차매도호가, 2 : 3차매도호가, 3 : 4차매도호가, 4 : 5차매도호가, 5 : 시장가, 6 : 최유리지정가, 7 : 사용자지정가, 8 : 현재가, 9 : 1차매수호가, 10 : 2차매수호가, 11 : 3차매수호가, 12 : 4차매수호가, 13 : 5차매수호가

	int		m_nTick;			///< 현재와의 괴리
	BOOL	m_bCancelOrder;		///< 우선거래 자동 주문취소 여부

public:
	CSetupSlippageZeroOrderSet(){}
	CSetupSlippageZeroOrderSet & operator=(CSetupSlippageZeroOrderSet & data)
	{
		m_nFirstTrade		= data.m_nFirstTrade;
		m_nFirstTradeSeq	= data.m_nFirstTradeSeq;
		m_nFirstTradeSell	= data.m_nFirstTradeSell;
		m_nFirstTradeBuy	= data.m_nFirstTradeBuy;
		m_nSecondTradeSell	= data.m_nSecondTradeSell;
		m_nSecondTradeBuy	= data.m_nSecondTradeBuy;
		m_nTick				= data.m_nTick;
		m_bCancelOrder		= data.m_bCancelOrder;
		return *this;
	}

	
	BOOL LoadSlippageInitFile(CString strUserPath)
	{
		CString sFileName;
		TCHAR Temp[100];

		sFileName.Format("%s\\%s", strUserPath, FILE_INI);
		
		GetPrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_01, "0", Temp, sizeof(Temp), sFileName); 
		m_nFirstTrade = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_02, "0", Temp, sizeof(Temp), sFileName); 
		m_nFirstTradeSeq = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_03, "0", Temp, sizeof(Temp), sFileName); 
		m_nFirstTradeSell = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_04, "0", Temp, sizeof(Temp), sFileName); 
		m_nFirstTradeBuy = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_05, "0", Temp, sizeof(Temp), sFileName); 
		m_nSecondTradeSell = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_06, "0", Temp, sizeof(Temp), sFileName); 
		m_nSecondTradeBuy = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_07, "0", Temp, sizeof(Temp), sFileName); 
		m_nTick = atoi(Temp);
		
		GetPrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_07_1, "0", Temp, sizeof(Temp), sFileName); 
		m_bCancelOrder = atoi(Temp);

		return TRUE;
	}

	void SaveSlippageInitFile(CString strUserPath)
	{
		CString sFileName;
		CString sTemp;
		
		sFileName.Format("%s\\%s", strUserPath, FILE_INI);	

		sTemp.Format("%d", m_nFirstTrade);
		WritePrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_01, sTemp, sFileName);

		sTemp.Format("%d", m_nFirstTradeSeq);
		WritePrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_02, sTemp, sFileName);

		sTemp.Format("%d", m_nFirstTradeSell);
		WritePrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_03, sTemp, sFileName);

		sTemp.Format("%d", m_nFirstTradeBuy);
		WritePrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_04, sTemp, sFileName);

		sTemp.Format("%d", m_nSecondTradeSell);
		WritePrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_05, sTemp, sFileName);

		sTemp.Format("%d", m_nSecondTradeBuy);
		WritePrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_06, sTemp, sFileName);

		sTemp.Format("%d", m_nTick);
		WritePrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_07, sTemp, sFileName);

		sTemp.Format("%d", m_bCancelOrder);
		WritePrivateProfileString(SECTION_CONFIGURATION, SLIPPAGE_ORDER_KEY_07_1, sTemp, sFileName);
	}

	
};

class CSetupDisplaySet
{
public:
	int		m_nAvgPrice;		///< 0 : 최초입력값 유지, 1 : 자동갱신
	int		m_nGreekDisplay;	///< 0 : 수량 적용 안함(1계약으로 계산), 1: 입력한 수량으로 계산
	
	int		m_nAddSameCode;		///< 0 : 새로운 줄로 추가, 1 : 평균가, 수량 합산 후 한 줄로 표시
	
	BOOL	m_bSort;			///< 정렬여부
	int		m_nSort;			///< 0 : 유가증권별/월물/행사가별
								///< 1 : 유가증권별/행사가별/월물
								///< 2 : 월물/유가증권별/행사가별
								///< 3 : 월물/행사가별/유가증권별
								///< 4 : 행사가별/유가증권별/월물
								///< 5 : 행사가별/월물/유가증권별
	int		m_nSortType;		///< 0 : 오름차순, 1 : 내림차순
	
	double	m_dXAxisMin;		///< 기초자산 최소값
	double	m_dXAxisMax;		///< 기초자산 최대값
	int		m_nYRange;			///< 0: 자동설정, 1 : 수동설정
	double	m_dYMax;			///< 최대 y값
	double	m_dYMin;			///< 최소 y값
	
	int		m_nRemainDisplay;	///< 0 : 현재 잔고 수량, 1 : 청산 가능 수량
	int		m_nProfitType;		///< 0 : 현대가, 1 : 이론가, 2 : 만기가
public:
	CSetupDisplaySet()
	{
		m_nAvgPrice		= 0;
		m_nGreekDisplay = 0;
		m_nAddSameCode	= 0;
		m_bSort			= FALSE;
		m_nSort			= 0;	
		m_nSortType		= 0;
		m_dXAxisMin		= 0.0;
		m_dXAxisMax		= 0.0;
		m_nYRange		= 0;
		m_dYMax			= 0.0;
		m_dYMin			= 0.0;
		m_nRemainDisplay= 0;
		m_nProfitType	= 0;
	}
	CSetupDisplaySet & operator=(CSetupDisplaySet & data)
	{
		m_nAvgPrice			= data.m_nAvgPrice;
		m_nGreekDisplay		= data.m_nGreekDisplay;
		m_nAddSameCode		= data.m_nAddSameCode;
		m_bSort				= data.m_bSort;
		m_nSort				= data.m_nSort;
		m_nSortType			= data.m_nSortType;

		m_dXAxisMin			= data.m_dXAxisMin;
		m_dXAxisMax			= data.m_dXAxisMax;
		m_nYRange			= data.m_nYRange;
		m_dYMax				= data.m_dYMax;
		m_dYMin				= data.m_dYMin;
		m_nRemainDisplay	= data.m_nRemainDisplay;
		m_nProfitType		= data.m_nProfitType;
		return *this;
	}

	BOOL LoadDisplayInitFile(CString strUserPath)
	{		
		CString sFileName;
		TCHAR Temp[100];

		sFileName.Format("%s\\%s", strUserPath, FILE_INI);
		
		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_01, "0", Temp, sizeof(Temp), sFileName); 
		m_nAvgPrice = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_02, "0", Temp, sizeof(Temp), sFileName); 
		m_nGreekDisplay = atoi(Temp);
		
		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_03, "0", Temp, sizeof(Temp), sFileName); 
		m_nAddSameCode = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_04, "0", Temp, sizeof(Temp), sFileName); 
		m_bSort = atoi(Temp);
		
		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_04_1, "0", Temp, sizeof(Temp), sFileName); 
		m_nSort = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_04_2, "0", Temp, sizeof(Temp), sFileName); 
		m_nSortType = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_05, "0", Temp, sizeof(Temp), sFileName); 
		m_dXAxisMin = atof(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_06, "0", Temp, sizeof(Temp), sFileName); 
		m_dXAxisMax = atof(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_07, "0", Temp, sizeof(Temp), sFileName); 
		m_nYRange = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_07_1, "0", Temp, sizeof(Temp), sFileName); 
		m_dYMax = atof(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_07_2, "0", Temp, sizeof(Temp), sFileName); 
		m_dYMin = atof(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_08, "0", Temp, sizeof(Temp), sFileName); 
		m_nRemainDisplay = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_09, "0", Temp, sizeof(Temp), sFileName); 
		m_nProfitType = atoi(Temp);

		return TRUE;
	}

	void SaveDisplayInitFile(CString strUserPath)
	{	
		CString sFileName;
		CString sTemp;

		sFileName.Format("%s\\%s", strUserPath, FILE_INI);	

		sTemp.Format("%d", m_nAvgPrice);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_01, sTemp, sFileName);

		sTemp.Format("%d", m_nGreekDisplay);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_02, sTemp, sFileName);

		sTemp.Format("%d", m_nAddSameCode);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_03, sTemp, sFileName);

		sTemp.Format("%d", m_bSort);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_04, sTemp, sFileName);

		sTemp.Format("%d", m_nSort);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_04_1, sTemp, sFileName);

		sTemp.Format("%d", m_nSortType);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_04_2, sTemp, sFileName);

		sTemp.Format("%.2f", m_dXAxisMin);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_05, sTemp, sFileName);
		
		sTemp.Format("%.2f", m_dXAxisMax);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_06, sTemp, sFileName);

		sTemp.Format("%d", m_nYRange);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_07, sTemp, sFileName);

		sTemp.Format("%.2f", m_dYMax);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_07_1, sTemp, sFileName);

		sTemp.Format("%.2f", m_dYMin);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_07_2, sTemp, sFileName);

		sTemp.Format("%d", m_nRemainDisplay);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_08, sTemp, sFileName);

		sTemp.Format("%d", m_nProfitType);
		WritePrivateProfileString(SECTION_CONFIGURATION, DISPLAY_KEY_09, sTemp, sFileName);
	}
};

class CSetupEtcSet
{
public:
	BOOL	m_bAddCommission;		///< 수수료를 손익에 반영	
	double	m_dFutureCom;			///< 선물 수수료율(%단위)	
	double	m_dOptionCom;			///< 옵션 수수료율(%단위)	
	double	m_dStockCom;			///< 주식 수수료율(%단위)

	BOOL	m_bApplyRealBuy;		///< 순매수대금을 증거금계산에 포함.
	
	int		m_nTradeInputChange;	///< 매수/매도 입력 방식  0 : 콤보박스 , 1 : 토글방식
	
	int		m_nCPrice;				///< 0 : 현재가, 1 : 상대호가
//	BOOL	m_bOrderEnableHoga;		///< 청산기준 우선호가 

	BOOL	m_bApplyPredictPrice;	///< 예상체결가 적용 여부

	int		m_nIV;					///< 0 : 잔존일수 조정 반영, 1 : 잔존일수 조정 미반영
public:
	CSetupEtcSet(){}

	CSetupEtcSet & operator=(CSetupEtcSet & data)
	{
		m_bAddCommission		= data.m_bAddCommission;
		m_dFutureCom			= data.m_dFutureCom;
		m_dOptionCom			= data.m_dOptionCom;
		m_dStockCom				= data.m_dStockCom;
		m_bApplyRealBuy			= data.m_bApplyRealBuy;
		m_nTradeInputChange		= data.m_nTradeInputChange;
		m_nCPrice				= data.m_nCPrice;
//		m_bOrderEnableHoga		= data.m_bOrderEnableHoga;
		m_bApplyPredictPrice	= data.m_bApplyPredictPrice;
		m_nIV					= data.m_nIV;
		return *this;
	}

	BOOL LoadEtcInitFile(CString strUserPath)
	{
		CString sFileName;
		TCHAR Temp[100];

		sFileName.Format("%s\\%s", strUserPath, FILE_INI);

		GetPrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_01, "0", Temp, sizeof(Temp), sFileName); 
		m_bAddCommission = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_01_1, "0", Temp, sizeof(Temp), sFileName); 
		m_dFutureCom = atof(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_01_2, "0", Temp, sizeof(Temp), sFileName); 
		m_dOptionCom = atof(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_01_3, "0", Temp, sizeof(Temp), sFileName); 
		m_dStockCom = atof(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_02, "0", Temp, sizeof(Temp), sFileName); 
		m_bApplyRealBuy = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_03, "0", Temp, sizeof(Temp), sFileName); 
		m_nTradeInputChange = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_04, "0", Temp, sizeof(Temp), sFileName); 
		m_nCPrice = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_05, "0", Temp, sizeof(Temp), sFileName); 
		m_bApplyPredictPrice = atoi(Temp);

		GetPrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_06, "0", Temp, sizeof(Temp), sFileName); 
		m_nIV = atoi(Temp);

		return TRUE;
	}

	void SaveEtcInitFile(CString strUserPath)
	{	
		CString sFileName;
		CString sTemp;

		sFileName.Format("%s\\%s", strUserPath, FILE_INI);	

		sTemp.Format("%d", m_bAddCommission);
		WritePrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_01, sTemp, sFileName);

		sTemp.Format("%.2f", m_dFutureCom);
		WritePrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_01_1, sTemp, sFileName);

		sTemp.Format("%.2f", m_dOptionCom);
		WritePrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_01_2, sTemp, sFileName);

		sTemp.Format("%.2f", m_dStockCom);
		WritePrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_01_3, sTemp, sFileName);

		sTemp.Format("%d", m_bApplyRealBuy);
		WritePrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_02, sTemp, sFileName);

		sTemp.Format("%d", m_nTradeInputChange);
		WritePrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_03, sTemp, sFileName);

		sTemp.Format("%d", m_nCPrice);
		WritePrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_04, sTemp, sFileName);

		sTemp.Format("%d", m_bApplyPredictPrice);
		WritePrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_05, sTemp, sFileName);

		sTemp.Format("%d", m_nIV);
		WritePrivateProfileString(SECTION_CONFIGURATION, ETC_KEY_06, sTemp, sFileName);
	}
};
// <-- End

class CSetupSet
{
public:
	CSetupTheorySet*			m_pTheorySet;
	CSetupFOOrderSet*			m_pFOOrderSet;
	CSetupSlippageZeroOrderSet*	m_pSlippageZeroSet;
	CSetupDisplaySet*			m_pDisplaySet;
	CSetupEtcSet*				m_pEtcSet;

public:

	CSetupSet()
	{
		m_pTheorySet		= new CSetupTheorySet;
		m_pFOOrderSet		= new CSetupFOOrderSet;
		m_pSlippageZeroSet	= new CSetupSlippageZeroOrderSet;
		m_pDisplaySet		= new CSetupDisplaySet;
		m_pEtcSet			= new CSetupEtcSet;
	}

	void SetSetupTheorySet(CSetupTheorySet * pSet)
	{
		*m_pTheorySet = *pSet;
	}

	void SetSetupFOOrderSet(CSetupFOOrderSet * pSet)
	{
		*m_pFOOrderSet = *pSet;
	}

	void SetSetupSlippageZeroSet(CSetupSlippageZeroOrderSet * pSet)
	{
		*m_pSlippageZeroSet = *pSet;
	}

	void SetSetupDisplaySet(CSetupDisplaySet * pSet)
	{
		*m_pDisplaySet = *pSet;
	}

	void SetSetupEtcSet(CSetupEtcSet * pSet)
	{
		*m_pEtcSet = *pSet;
	}	

	~CSetupSet()
	{
		if(m_pTheorySet)		delete m_pTheorySet;
		if(m_pFOOrderSet)		delete m_pFOOrderSet;
		if(m_pSlippageZeroSet)	delete m_pSlippageZeroSet;
		if(m_pDisplaySet)		delete m_pDisplaySet;
		if(m_pEtcSet)			delete m_pEtcSet;
	}

	CSetupSet & operator=(CSetupSet & setup)
	{
		*m_pTheorySet		= *setup.m_pTheorySet;
		*m_pFOOrderSet		= *setup.m_pFOOrderSet;
		*m_pSlippageZeroSet	= *setup.m_pSlippageZeroSet;
		*m_pDisplaySet		= *setup.m_pDisplaySet;
		*m_pEtcSet			= *setup.m_pEtcSet;

		return *this;
	}
};


class CDlgUserData
{
public:
	int CurTab;
	int CurMonth;
	int CurATM;
	int CurQty;
	CString StrategyGroup;
	CString StrategyName;
	
	int OptMonth;
	CString OptQty;
	CString FutQty;
	int CurView;
public:

	CDlgUserData()
	{
		CurTab = -1, CurMonth = -1, CurATM = -1, CurQty = 1, OptMonth = -1,
		CurView = -1, StrategyGroup = "", StrategyName = "", OptQty = "", FutQty = "";
	}

	CDlgUserData & operator=(CDlgUserData & item)
	{
		CurTab		  = item.CurTab;
		CurMonth	  = item.CurMonth;
		CurATM		  = item.CurATM;
		CurQty		  = item.CurQty;
		StrategyGroup = item.StrategyGroup;
		StrategyName  = item.StrategyName;

		OptMonth	  = item.OptMonth;
		OptQty		  = item.OptQty;
		FutQty		  = item.FutQty;
		CurView		  = item.CurView;

		return *this;
	}
};

///////////////////////////////////////////////////////////////////////
//
//
//
//
//
///////////////////////////////////////////////////////////////////////
class CInfoData
{
public:
	
	CString m_strFixedProfit;				//확정손익 + 수수료.
	CString m_strOnlyFixedProfit;			//확정손익만..
	CString m_strUnderlyingAssetsCode;		//현재 선택된 기초자산..
	CString	m_strUnderlyingAssetsName;		//현재 선택된 기초자산명..

//	CSetupDataSet *	m_pSetupDataSet;
	CSetupTheorySet*	m_pSetupTheorySet;
	CSetupFOOrderSet*	m_pSetupFOOrderSet;
	CSetupSlippageZeroOrderSet*	m_pSetupSlippageZeroOrderSet;
	CSetupDisplaySet*	m_pSetupDisplaySet;
	CSetupEtcSet*		m_pSetupEtcSet;
	BOOL	m_bSelectCalc;//TRUE: 증거금 쓰래드 돌동안 LButtonDown을 클릭해서 선택증거금계산했을 경우..
	
public:

	CInfoData()
	{
		m_bSelectCalc = FALSE;
	}
};


class CChartDataSet
{
public:
	CDataArray	*	m_pChartAddItemArray;
	CDataArray	*	m_pChartDelItemArray;

public:
	CChartDataSet()
	{
		m_pChartAddItemArray = NULL;
		m_pChartDelItemArray = NULL;
	}
};
////////////////////////////////////////////////////////////////////////////

class CRemainDaySet
{
public:
	CString m_strYM;
	CString m_strRemainDayNextDay;

public:

	CRemainDaySet & operator=(CRemainDaySet & set)
	{
		m_strYM = set.m_strYM;
		m_strRemainDayNextDay = set.m_strRemainDayNextDay;
		return *this;
	}

};
typedef CArray < CRemainDaySet * , CRemainDaySet *> CRemainDayArray;

class CMarginInfo
{
public:

	//20080328 이문수 >>
	//하나대투에서 위탁과 유지에 순위험증거금에 할증
	CString	m_strHanaPremiumRatioFlag;	//이 값이 2이면 할증적용
	CString	m_strDangerPremiumRatio;	//위탁순위험증거금할증율(%아닌 원래값 저장)
	CString m_strHavePremiumRatio;		//유지순위험증거금할증율(%아닌 원래값 저장)
	//20080328 이문수 <<

	CString m_strBranchNo;				/* 지점번호                 */
	CString m_strGoodsJobSect;			/* 상품업무구분             */
	CString m_strAccountSeq;/* 계좌일련번호             */
	
	CString m_strSect;/* 구분                     */
	/* 일방해소시 증거금 적용 여부  0 - 적용 , 1 - 미적용           */


	CString m_strDangerPercent;/* 개시증거금율             */
	CString m_strHavePercent;/* 유지증거금율             */
	CString m_strDangerMinPercent;/* 개시최소증거금율         */
	CString m_strHaveMinPercent; /* 유지최소증거금율         */
	CString m_strFutureMulti;/* 선물승수                 */
	CString m_strOptionMulti;/* 옵션승수                 */
	CString m_strFutureMinMargin;/* 최소증거금               */
             /* 선물계약당최소증거금 - 일반                                  */


	CString m_strOptionMinMargin;/* 최소증거금1              */
             /* 옵션계약당최소증거금 - 일반                                  */


//	CString m_strFutureMinMargin1; /* 최소증거금2              */
             /* 선물계약당최소증거금 - 일방해소증거금 미적용                 */

//	CString m_strOptionMinMargin1; /* 최소증거금3              */
             /* 옵션계약당최소증거금 - 일방해소증거금 미적용                 */

	CString m_strSpreadDangerMargin;/* 증거금액                 */
             /* 스프레드위탁증거금                                           */

	CString m_strSpreadTradeMargin;/* 증거금액1                */
             /* 스프레드매매증거금                                           */

//	CString m_strDayPerYearForTheory;/* 영업일수                 */
             /* 익일이론가 계산용 연간영업일수                               */


	CString m_strWrMoneySect;
	CString m_strSide_Opg_wrmoney_rate;
	CString m_strSide_Maintn_wrmoney_rate;

	CString m_strCashRate;

    
	CRemainDayArray	m_RemainDayArray;//잔여일 정보배열.

//    CString m_strOrdDate;  // 당일주문영업일(증거금 계산용 기준일)
public:

	CMarginInfo(){ m_strCashRate = "0.05";}

	~CMarginInfo()
	{
		int i,nSize = m_RemainDayArray.GetSize();
		for(i = 0 ; i < nSize ; i++) delete m_RemainDayArray.GetAt(i);
	}

	CMarginInfo & operator=(CMarginInfo & margin)
	{
		m_strBranchNo = margin.m_strBranchNo ;				/* 지점번호                 */
		m_strGoodsJobSect= margin.m_strGoodsJobSect;			/* 상품업무구분             */
		m_strAccountSeq= margin.m_strAccountSeq;/* 계좌일련번호             */
		
		m_strSect= margin.m_strSect;/* 구분                     */
		/* 일방해소시 증거금 적용 여부  0 - 적용 , 1 - 미적용           */


		m_strDangerPercent= margin.m_strDangerPercent;/* 개시증거금율             */
		m_strHavePercent= margin.m_strHavePercent;/* 유지증거금율             */
		m_strDangerMinPercent= margin.m_strDangerMinPercent;/* 개시최소증거금율         */
		m_strHaveMinPercent= margin.m_strHaveMinPercent; /* 유지최소증거금율         */
		m_strFutureMulti= margin.m_strFutureMulti;/* 선물승수                 */
		m_strOptionMulti= margin.m_strOptionMulti;/* 옵션승수                 */
		m_strFutureMinMargin= margin.m_strFutureMinMargin;/* 최소증거금               */
				 /* 선물계약당최소증거금 - 일반                                  */


		m_strOptionMinMargin= margin.m_strOptionMinMargin;/* 최소증거금1              */
				 /* 옵션계약당최소증거금 - 일반                                  */


//		m_strFutureMinMargin1= margin.m_strFutureMinMargin1; /* 최소증거금2              */
				 /* 선물계약당최소증거금 - 일방해소증거금 미적용                 */

//		m_strOptionMinMargin1= margin.m_strOptionMinMargin1; /* 최소증거금3              */
				 /* 옵션계약당최소증거금 - 일방해소증거금 미적용                 */

		m_strSpreadDangerMargin= margin.m_strSpreadDangerMargin;/* 증거금액                 */
				 /* 스프레드위탁증거금                                           */

		m_strSpreadTradeMargin= margin.m_strSpreadTradeMargin;/* 증거금액1                */
				 /* 스프레드매매증거금                                           */

//		m_strDayPerYearForTheory= margin.m_strDayPerYearForTheory;/* 영업일수                 */
				 /* 익일이론가 계산용 연간영업일수                               */

		m_strCashRate = margin.m_strCashRate;

		m_strWrMoneySect = margin.m_strWrMoneySect;
		m_strSide_Opg_wrmoney_rate = margin.m_strSide_Opg_wrmoney_rate;
		m_strSide_Maintn_wrmoney_rate = margin.m_strSide_Maintn_wrmoney_rate;

//		if(m_strOrdDate.GetLength() != 0)
//			m_strOrdDate = margin.m_strOrdDate;
		
		
		//20080328 이문수 >>
		m_strHanaPremiumRatioFlag	= margin.m_strHanaPremiumRatioFlag;	//이 값이 2이면 할증적용
		m_strDangerPremiumRatio		= margin.m_strDangerPremiumRatio;	//위탁순위험증거금할증율(%아닌 원래값 저장)
		m_strHavePremiumRatio		= margin.m_strHavePremiumRatio;		//유지순위험증거금할증율(%아닌 원래값 저장)
		//20080328 이문수 <<


		int i,nSize = m_RemainDayArray.GetSize();
		for(i = 0 ; i < nSize ; i++) delete m_RemainDayArray.GetAt(i);
		m_RemainDayArray.RemoveAll();



		nSize = margin.m_RemainDayArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			CRemainDaySet * pSet = margin.m_RemainDayArray.GetAt(i);
			CRemainDaySet * pNewSet = new CRemainDaySet;
			*pNewSet = *pSet;
			m_RemainDayArray.Add(pNewSet);
		}		
		return *this;
	}

	CString GetRemainDay(CString strCode)
	{
		for(int i = 0 ;  i < m_RemainDayArray.GetSize(); i++)
		{
			CRemainDaySet * pRemainSet = m_RemainDayArray.GetAt(i);
			if(pRemainSet->m_strYM.GetLength() > 1 && pRemainSet->m_strYM == strCode.Mid(3,2))
			{
				return pRemainSet->m_strRemainDayNextDay;
			}
		}
		return "";
	}
};

class CAverageIVSet
{
public:
	CString m_strUnderlyingAssetsCode;
	CString m_strCallAverageIV;
	CString m_strPutAverageIV;
	CString m_strVolate;

public:
	CAverageIVSet & operator=(CAverageIVSet & set)
	{
		
		m_strUnderlyingAssetsCode	= set.m_strUnderlyingAssetsCode;
		m_strCallAverageIV			= set.m_strCallAverageIV;
		m_strPutAverageIV			= set.m_strPutAverageIV;
		m_strVolate					= set.m_strVolate;

		return *this;
	}

};
typedef CArray < CAverageIVSet * , CAverageIVSet *> CAverageIVArray;

class CAverageIV
{
public:
	char	m_cTradeState;
	CString m_strType;//장종료구분(1 - 장중, 2 - 장종료)

	CAverageIVArray m_AverageIVArray;
public:
	CAverageIV & operator=(CAverageIV & iv)
	{
		m_strType			=iv.m_strType;
		m_cTradeState		=iv.m_cTradeState;

		int i;
		for(i = 0 ; i < m_AverageIVArray.GetSize(); i++) delete m_AverageIVArray.GetAt(i);
		m_AverageIVArray.RemoveAll();

		for(i = 0 ; i < iv.m_AverageIVArray.GetSize(); i++) 
		{
			CAverageIVSet * pSet = iv.m_AverageIVArray.GetAt(i);
			CAverageIVSet * pNewSet = new CAverageIVSet;
			*pNewSet = *pSet;
			m_AverageIVArray.Add(pNewSet);
		}
		return *this;
	}

	~CAverageIV()
	{
		for(int i = 0 ; i < m_AverageIVArray.GetSize(); i++) delete m_AverageIVArray.GetAt(i);
	}
};

////////////////////////////////////////////////////////////////////////////////////
//
//	옵션 월물별 20일 내재변동성 데이터
//
////////////////////////////////////////////////////////////////////////////////////
class COptionIVSet
{
public:
	CString m_strStrike;
	CStringArray m_strIVArray;

public:

	COptionIVSet & operator=(COptionIVSet & option)
	{
		m_strStrike = option.m_strStrike;
		
		m_strIVArray.RemoveAll();

		m_strIVArray.Append(option.m_strIVArray);

		return *this;
	}
};
typedef CArray < COptionIVSet *, COptionIVSet *> COptionIVArray;

class COptionIVMonthSet
{
public:
	CString m_strEndDate;
	COptionIVArray	m_CallArray;
	COptionIVArray	m_PutArray;

public:
	COptionIVMonthSet(){;}
	~COptionIVMonthSet()
	{
		int i;
		for(i = 0 ; i < m_CallArray.GetSize(); i++) delete m_CallArray.GetAt(i);
		for(i = 0 ; i < m_PutArray.GetSize();  i++) delete m_PutArray.GetAt(i);
	}

	COptionIVMonthSet & operator=(COptionIVMonthSet & month)
	{
		int i;
		m_strEndDate = month.m_strEndDate;
		
		for(i = 0 ; i < m_CallArray.GetSize(); i++) delete m_CallArray.GetAt(i);
		m_CallArray.RemoveAll();
		for(i = 0 ; i < m_PutArray.GetSize();  i++) delete m_PutArray.GetAt(i);
		m_PutArray.RemoveAll();

		for(i = 0 ; i < month.m_CallArray.GetSize(); i++)
		{
			COptionIVSet * pIV = month.m_CallArray.GetAt(i);
			COptionIVSet * pNewIV = new COptionIVSet;
			*pNewIV = *pIV;
			m_CallArray.Add(pNewIV);
		}

		for(i = 0 ; i < month.m_PutArray.GetSize(); i++)
		{
			COptionIVSet * pIV = month.m_PutArray.GetAt(i);
			COptionIVSet * pNewIV = new COptionIVSet;
			*pNewIV = *pIV;
			m_PutArray.Add(pNewIV);
		}		
		return *this;
	}
};

//20080308 이문수 >>
class CPriceProgressInput
{
public:
	CString m_strCode;
	int		m_nDataCount;
	char	m_nDataBit;
	CString	m_strDate;
	CString	m_strTime;

public:
	CPriceProgressInput(){ m_nDataCount = 0; m_nDataBit = 0;	}
	CPriceProgressInput & operator=(CPriceProgressInput & Input)
	{
		m_strCode	= Input.m_strCode;
		m_nDataCount= Input.m_nDataCount;
		m_nDataBit	= Input.m_nDataBit;
		m_strDate	= Input.m_strDate;
		m_strTime	= Input.m_strTime;
		return *this;
	}
};
typedef CList < CPriceProgressInput * , CPriceProgressInput * > CPriceProgressInputList;
//20080308 이문수 <<

////////////////////////////////////////////////////////////////////////////////////
// 김성곤 추가 2003.02.07
class CPriceProgressItem
{
public:
	CString		m_strDate;
	CString		m_strTime;
	CString		m_strLastPrice;					// 종가
	CString		m_strDecisionTime;				// 확정시간

	//////////////////////////////
	//20080306 이문수 >>
	//임시 보관형
	CString		m_strRemainDay;					//잔존일
	CString		m_strRatio;						//배당액지수
	CString		m_strCDRate;					//cd금리
	//20080306 이문수 <<
	//////////////////////////////

public:
	CPriceProgressItem& operator =(CPriceProgressItem &PriceProgressItem)
	{
		m_strDate			= PriceProgressItem.m_strDate;
		m_strTime			= PriceProgressItem.m_strTime;
		m_strLastPrice		= PriceProgressItem.m_strLastPrice;

		//20080306 이문수 >>
		m_strRemainDay		= PriceProgressItem.m_strRemainDay;
		m_strRatio			= PriceProgressItem.m_strRatio;
		m_strCDRate			= PriceProgressItem.m_strCDRate;
		//20080306 이문수 <<
		
		return *this;
	}
};

typedef CList <CPriceProgressItem *, CPriceProgressItem *> CPriceProgressItemList;
typedef CArray <CPriceProgressItem *, CPriceProgressItem *> CPriceProgressItemArray;

class CPriceProgressSubItem
{
public:
	CString		m_strDate;
	CString		m_strRemainDay;					// 잔존일수
	CString		m_strAvgIntRate;				// 금리평균
	CString		m_strRatio;						// 비율
	CString		m_strVolate;					// 총평균변동성
	CString		m_strHisVolate;					// 역사적변동성
public:
	CPriceProgressSubItem& operator =(CPriceProgressSubItem &PriceProgressSubItem)
	{
		m_strDate		= PriceProgressSubItem.m_strDate;
		m_strRemainDay	= PriceProgressSubItem.m_strRemainDay;
		m_strAvgIntRate	= PriceProgressSubItem.m_strAvgIntRate;
		m_strRatio		= PriceProgressSubItem.m_strRatio;
		m_strVolate		= PriceProgressSubItem.m_strVolate;
		m_strHisVolate	= PriceProgressSubItem.m_strHisVolate;

		return *this;
	}
};

typedef CList <CPriceProgressSubItem *, CPriceProgressSubItem *> CPriceProgressSubItemList;
typedef CArray <CPriceProgressSubItem *, CPriceProgressSubItem *> CPriceProgressSubItemArray;

class CPriceProgressInfo
{
public:
	CString		m_strCode;

	CPriceProgressItemList		m_PriceProgressItemList;
	CPriceProgressSubItemList	m_PriceProgressSubItemList;

	int		m_iReferenceCount;

public:
	CPriceProgressInfo()
	{
		m_iReferenceCount = 1;
	}

	~CPriceProgressInfo()
	{
		DeleteAllItem();
	}

	void DeleteAllItem()
	{
		POSITION Pos = m_PriceProgressItemList.GetHeadPosition();

		while(Pos)
		{
			CPriceProgressItem *pPriceProgressItem = m_PriceProgressItemList.GetNext(Pos);

			if(pPriceProgressItem != NULL)
				delete pPriceProgressItem;
		}
		m_PriceProgressItemList.RemoveAll();

		Pos = m_PriceProgressSubItemList.GetHeadPosition();

		while(Pos)
		{
			CPriceProgressSubItem *pPriceProgressSubItem = m_PriceProgressSubItemList.GetNext(Pos);

			if(pPriceProgressSubItem != NULL)
				delete pPriceProgressSubItem;
		}
		m_PriceProgressSubItemList.RemoveAll();
	}

	CPriceProgressInfo& operator =(CPriceProgressInfo &PriceProgressInfo)
	{
		DeleteAllItem();

		m_strCode		= PriceProgressInfo.m_strCode;

		POSITION Pos = PriceProgressInfo.m_PriceProgressItemList.GetHeadPosition();

		while(Pos)
		{
			CPriceProgressItem *pPriceProgressItem = new CPriceProgressItem;
			CPriceProgressItem *pTempPriceProgressItem = PriceProgressInfo.m_PriceProgressItemList.GetNext(Pos);
			
			*pPriceProgressItem = *pTempPriceProgressItem;

			m_PriceProgressItemList.AddTail(pPriceProgressItem);
		}

		Pos = PriceProgressInfo.m_PriceProgressSubItemList.GetHeadPosition();

		while(Pos)
		{
			CPriceProgressSubItem *pPriceProgressSubItem = new CPriceProgressSubItem;
			CPriceProgressSubItem *pTempPriceProgressSubItem = 
				PriceProgressInfo.m_PriceProgressSubItemList.GetNext(Pos);

			*pPriceProgressSubItem = *pTempPriceProgressSubItem;

			m_PriceProgressSubItemList.AddTail(pPriceProgressSubItem);
		}

		return *this;
	}

	CPriceProgressSubItem *FindSubItem(LPCTSTR pszDate)
	{
		POSITION Pos = m_PriceProgressSubItemList.GetHeadPosition();

		while(Pos)
		{
			CPriceProgressSubItem *pPriceProgressSubItem = m_PriceProgressSubItemList.GetNext(Pos);

			if(pPriceProgressSubItem == NULL)
				continue;

			if(pPriceProgressSubItem->m_strDate == pszDate)
				return pPriceProgressSubItem;
		}

		return NULL;
	}
};

typedef CArray<CPriceProgressInfo *, CPriceProgressInfo *> CPriceProgressInfoArray;

class CPriceProgressData
{
public:
	int		m_iReceiveMode;		// 데이터 연속 구분 모드(1: Middle, 2: End)

	CPriceProgressInfoArray		m_arrPriceProgressInfo;

public:
	~CPriceProgressData()
	{
		DeleteAllItem();
	}

	void DeleteAllItem()
	{
		int iCount = m_arrPriceProgressInfo.GetSize();

		for(int _i=0; _i<iCount; _i++)
		{
			CPriceProgressInfo *pPriceProgressInfo = m_arrPriceProgressInfo.GetAt(_i);

			if(pPriceProgressInfo != NULL)
				delete pPriceProgressInfo;
		}
		m_arrPriceProgressInfo.RemoveAll();
	}

	CPriceProgressData& operator =(CPriceProgressData &PriceProgressData)
	{
		DeleteAllItem();

		m_iReceiveMode		= PriceProgressData.m_iReceiveMode;
		
		int iCount = PriceProgressData.m_arrPriceProgressInfo.GetSize();
		
		for(int _i=0; _i<iCount; _i++)
		{
			CPriceProgressInfo *pPriceProgressInfo = new CPriceProgressInfo;
			CPriceProgressInfo *pTempPriceProgressInfo = PriceProgressData.m_arrPriceProgressInfo.GetAt(_i);
			
			*pPriceProgressInfo = *pTempPriceProgressInfo;

			m_arrPriceProgressInfo.Add(pPriceProgressInfo);
		}

		return *this;
	}
};

class CThreadStruct
{
public:
	
	CWnd * m_pWnd;
	BOOL * m_pbExeThread;
	CStrategyCalc * m_pStrategyCalc;
	CStrategyCalc * m_pStrategyCalcHedge;
	CDataArray	m_ChartAddItemArray;
	CDataArray	m_ChartDelItemArray;

	
	//20080317 이문수 >>
	//확률탭에 계산에 사용
	CString		m_strRemainDay;
	CString		m_strVolate;
	BOOL		m_bProbWeight;	
	//20080317 이문수 <<

public:
	
	CThreadStruct()
	{
		m_pWnd = NULL;
		m_pStrategyCalc = NULL;
		m_pStrategyCalcHedge = NULL;
		m_pbExeThread = NULL;

		//20080317 이문수 >>
		m_bProbWeight = FALSE;
		//20080317 이문수 <<

	}

	~CThreadStruct()
	{
		int i;
		for(i = 0 ; i < m_ChartAddItemArray.GetSize(); i++) delete m_ChartAddItemArray.GetAt(i);
		for(i = 0 ; i < m_ChartDelItemArray.GetSize(); i++) delete m_ChartDelItemArray.GetAt(i);
	}
};

#define	GREEK_UNDERLYING_MODE		0
#define	GREEK_VOLATE_MODE			1
#define	GREEK_REMAIN_DAY_MODE		2

#define	GREEK_DELTA_MODE			0
#define	GREEK_GAMMA_MODE			1
#define	GREEK_THETA_MODE			2
#define	GREEK_VEGA_MODE				3
#define	GREEK_THEORY_PROFIT_MODE	4

class CInputInfoSet
{
public:

	BOOL	m_bSelectItem;

	int		m_nIndex; //시뮬레이션 그리드의 줄 번호.

	int		m_nXMode; //0 : 기초자산, 1:변동성, 2: 잔존일

	int     m_nYMode; //0: 델타, 1: 감마, 2: 세타, 3: 베가 4 : 이론손익
	
	double	m_dCallVolate;
	
	double	m_dPutVolate;
	
	double	m_dUnderlyingAssetsPrice;
	
	int		m_nRemainDay;

public:
	CInputInfoSet()
	{
		m_nIndex= 0;
		m_nXMode= 0;
		m_nYMode= 0;
		m_dCallVolate = 0.;
		m_dPutVolate  = 0.;
		m_dUnderlyingAssetsPrice = 0;
		m_nRemainDay = 1;
		m_bSelectItem = TRUE;
	}



	CInputInfoSet & operator=(CInputInfoSet & InfoSet)
	{		
		m_nIndex= InfoSet.m_nIndex;
		m_nXMode= InfoSet.m_nXMode;
		m_nYMode= InfoSet.m_nYMode;
		m_dCallVolate = InfoSet.m_dCallVolate;
		m_dPutVolate  = InfoSet.m_dPutVolate;
		m_dUnderlyingAssetsPrice = InfoSet.m_dUnderlyingAssetsPrice;
		m_nRemainDay = InfoSet.m_nRemainDay;
		m_bSelectItem = InfoSet.m_bSelectItem;
		return *this;
	}
};
typedef CArray <CInputInfoSet *, CInputInfoSet *> CInputInfoArray;

class CInputInfo


{
public:
	int m_nXMode;
	int m_nYMode;

	double	m_dXMin;
	double	m_dXMax;




	CInputInfoArray	m_InputInfoArray;



public:

	CInputInfo(){ m_nXMode = 0, m_nYMode = 0, m_dXMin = 0., m_dXMax = 0;}
	~CInputInfo()
	{	
		for(int i = 0 ; i < m_InputInfoArray.GetSize(); i++) delete m_InputInfoArray.GetAt(i);		




	}
	
	CInputInfo & operator=(CInputInfo & Info)
	{
		m_nXMode	= Info.m_nXMode;
		m_nYMode	= Info.m_nYMode;
		
		m_dXMax		= Info.m_dXMax;
		m_dXMin		= Info.m_dXMin;

		for(int i = 0 ; i < m_InputInfoArray.GetSize(); i++) delete m_InputInfoArray.GetAt(i);
		m_InputInfoArray.RemoveAll();


		for(i = 0 ; i < Info.m_InputInfoArray.GetSize(); i++)
		{
			CInputInfoSet * pInfo = Info.m_InputInfoArray.GetAt(i);
			CInputInfoSet * pNewInfo = new CInputInfoSet;
			*pNewInfo = *pInfo;
			m_InputInfoArray.Add(pNewInfo);


		}

		return *this;
	}
};
typedef CArray <CInputInfo * , CInputInfo *> CInputArray;


class CThreadIO4GreekSimulator
{
public:
	CInputInfo *	m_pInputInfo;
	CWnd *			m_pWnd;
	CStrategyCalc *	m_pStrategyCalc;

public:
	
	CThreadIO4GreekSimulator()
	{
		m_pInputInfo = NULL;
		m_pWnd = NULL;
		m_pStrategyCalc = NULL;
	}

	~CThreadIO4GreekSimulator()
	{
		if(m_pInputInfo)	delete m_pInputInfo;
	}

};

class CGreekSimulSetupItem
{
public:
	int			m_nXMode;
	CString		m_strUnderlyingAssetsCode;

	CString		m_strCallVolateStart;
	CString		m_strPutVolateStart;
	CString		m_strRemainDayStart;
	CString		m_strUnderlyingAssetsPriceStart;

	CString		m_strCallVolateDelta;
	CString		m_strPutVolateDelta;
	CString		m_strRemainDayDelta;
	CString		m_strUnderlyingAssetsPriceDelta;

	int			m_nDataCount;

public:	

	CGreekSimulSetupItem(int nXMode)
	{
		m_nXMode = nXMode;
		m_nDataCount = 3;
		m_strRemainDayStart = "1";
	}


	CGreekSimulSetupItem & operator=(CGreekSimulSetupItem & SetupItem)
	{
		m_nXMode = SetupItem.m_nXMode;
		m_strCallVolateDelta = SetupItem.m_strCallVolateDelta;
		m_strCallVolateStart = SetupItem.m_strCallVolateStart;
		m_strPutVolateDelta = SetupItem.m_strPutVolateDelta;
		m_strPutVolateStart = SetupItem.m_strPutVolateStart;
		m_strRemainDayStart = SetupItem.m_strRemainDayStart;
		m_strRemainDayDelta = SetupItem.m_strRemainDayDelta;
		m_strUnderlyingAssetsPriceDelta = SetupItem.m_strUnderlyingAssetsPriceDelta;
		m_strUnderlyingAssetsPriceStart = SetupItem.m_strUnderlyingAssetsPriceStart;
		m_nDataCount = SetupItem.m_nDataCount;
		m_strUnderlyingAssetsCode = SetupItem.m_strUnderlyingAssetsCode;
		return *this;
	}
};

typedef CArray <CGreekSimulSetupItem *, CGreekSimulSetupItem *> CGreekSimulSetupArray;

class DATASET_DECLSPEC CIVPastInputData
{
public:

	int     m_nOutputCount;

	CString m_strCode1; 
	CString m_strCode2;

	CString m_strRqType;	//조회구분
	CString	m_strVolate;	//변동성

	CStringArray	m_strOutputTypeArray; //출력구분분	

public:

	CIVPastInputData()
	{
		m_nOutputCount = 9;
		for(int i = 0  ;i < m_nOutputCount ; i++) m_strOutputTypeArray.Add("0");
	}

	CIVPastInputData & operator=(CIVPastInputData & input)
	{
		m_strCode1 = input.m_strCode1;
		m_strCode2 = input.m_strCode2;
		m_strRqType= input.m_strRqType;
		m_strVolate= input.m_strVolate;

		m_strOutputTypeArray.RemoveAll();
		m_strOutputTypeArray.Append(input.m_strOutputTypeArray);

		return *this;
	}
};

class DATASET_DECLSPEC CIVPastDataSet
{
public:
	CString m_strDate;			  //날짜
	CString	m_strTime;			  //시간
	CStringArray	m_strIVArray; //내재변동성

public:

	CIVPastDataSet & operator=(CIVPastDataSet & data)
	{
		m_strDate	= data.m_strDate;
		m_strTime	= data.m_strTime;

		m_strIVArray.RemoveAll();
		m_strIVArray.Append(data.m_strIVArray);
		return *this;
	}
};
typedef CArray < CIVPastDataSet *, CIVPastDataSet *> CIVPastDataArray;


class DATASET_DECLSPEC CIVPastData
{
public:
	char	m_cIntervalType;	//기간 구분
	char	m_cRealType;		//실시간 구분
	char	m_cUseType;			//사용유무
	CString	m_strCurrent;		//현재가
	CString	m_strPreClose;		//전일종가
	CString m_strVolume;		//거래량
	CString	m_strNormalCode;	//종목코드 노말 ??
	CString	m_strName;			//종목명

	CString	m_strCode1;
	CString	m_strCode2;
	CString m_strIV1;
	CString m_strIV2;
	CString m_strDate;

	CIVPastDataArray	m_IVPastDataArray;
public:

	~CIVPastData()
	{
		for(int i = 0 ; i < m_IVPastDataArray.GetSize(); i++) delete m_IVPastDataArray.GetAt(i);
	}

	CIVPastData & operator=(CIVPastData & data)
	{
		m_cIntervalType = data.m_cIntervalType;
		m_cRealType		= data.m_cRealType;
		m_cUseType		= data.m_cUseType;
		m_strCurrent	= data.m_strCurrent;
		m_strPreClose	= data.m_strPreClose;
		m_strVolume		= data.m_strVolume;
		m_strNormalCode	= data.m_strNormalCode;
		m_strName		= data.m_strName;
		
		m_strCode1		= data.m_strCode1;
		m_strCode2		= data.m_strCode2;
		m_strIV1		= data.m_strIV1;
		m_strIV2		= data.m_strIV2;
		m_strDate		= data.m_strDate;

		int i;
		for(i = 0 ; i < m_IVPastDataArray.GetSize(); i++) delete m_IVPastDataArray.GetAt(i);
		m_IVPastDataArray.RemoveAll();

		for(i = 0 ; i < data.m_IVPastDataArray.GetSize(); i++)
		{
			CIVPastDataSet * pSet = data.m_IVPastDataArray.GetAt(i);
			CIVPastDataSet * pNewSet = new CIVPastDataSet;
			*pNewSet = *pSet;
			m_IVPastDataArray.Add(pNewSet);
		}
		return *this;
	}

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//20080228 이문수 >>
//하나대투에서 추가된 패킷
//기존에 대우에서 손익추이데이터로 받던 데이터패킷 중 종목별로 중복성을 띄는 데이터를 분리해서 기초자산별로 
//요청하고 받도록 했다.
//20080228 이문수 <<
//////////////////////////////////////////////////////////////////////////////////////////////////////////
class CTotalIVDataSet
{
public:
	CString m_strBaseCode;	//01(kp200), 10(주식옵션1), ....
	CString m_strCallIV;	//콜총평균내재변동성
	CString m_strPutIV;		//풋총평균내재변동성
	CString m_strTotIV;		//총총평균내재변동성
	CString m_strStatus;    //장구분상태 (1-장중, 2-장종료)

public:
	
	CTotalIVDataSet & operator=(CTotalIVDataSet & Set)
	{
		m_strBaseCode	= Set.m_strBaseCode;
		m_strCallIV		= Set.m_strCallIV;
		m_strPutIV		= Set.m_strPutIV;
		m_strTotIV		= Set.m_strTotIV;
		m_strStatus		= Set.m_strStatus;
		return *this;
	}

};
typedef CArray < CTotalIVDataSet *, CTotalIVDataSet *> CTotalIVDataArray;


//20080304 이문수 >>
class CIV4DateSet
{
public:
	CString		m_strDate;
	CString		m_strClose;
	CString		m_strCallIV;
	CString		m_strPutIV;
	CString		m_strTotIV;
	CString		m_strHis90IV;

public:
	CIV4DateSet(){;}

	CIV4DateSet(CIV4DateSet & Set)
	{
		Copy(Set);
	}

	CIV4DateSet & operator=(CIV4DateSet & Set)
	{
		Copy(Set);
		return *this;
	}

	void Copy(CIV4DateSet & Set)
	{
		m_strDate  = Set.m_strDate;
		m_strClose = Set.m_strClose;
		m_strCallIV= Set.m_strCallIV;
		m_strPutIV = Set.m_strPutIV;
		m_strTotIV = Set.m_strTotIV;
		m_strHis90IV=Set.m_strHis90IV;
	}
};

typedef CArray < CIV4DateSet * , CIV4DateSet * > CIV4DateArray;

class CIV4ItemSet
{
public:

	CString			m_strCode;
	CIV4DateArray	m_IVDateArray;

public:

	CIV4ItemSet & operator=(CIV4ItemSet & Set)
	{
		int i;
		m_strCode = Set.m_strCode;
		for(i = 0 ; i < m_IVDateArray.GetSize(); i++ )		
			delete m_IVDateArray.GetAt(i);
		
		for(i = 0 ;i < Set.m_IVDateArray.GetSize(); i++)
		{
			CIV4DateSet * pSet = Set.m_IVDateArray.GetAt(i);
			CIV4DateSet * pNewSet = new CIV4DateSet;
			*pNewSet = *pSet;
			m_IVDateArray.Add(pNewSet);
		}

		return *this;
	}
};

typedef CArray < CIV4ItemSet *, CIV4ItemSet * > CIV4ItemArray;
//20080304 이문수 <<
#endif 