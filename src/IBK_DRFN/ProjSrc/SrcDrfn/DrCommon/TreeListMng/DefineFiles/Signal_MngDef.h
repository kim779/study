#ifndef SIGNALMNG_TYPEDEF
#define SIGNALMNG_TYPEDEF


/****************************************************************/
/*                                                              */
/*						 Signal Mng Packet (Signal_MngDef.h)							*/
/*																															*/
/*						SignalMng에서 사용하는 신호 Packet!								*/
/*						SVR -> DRDS -> SignalMng													*/
/*														(User)														*/
/*																															*/
/*                   2005. 12. 12 		Doori											*/
/*                      작성자 : 우동우													*/
/****************************************************************/
#include <afxtempl.h>

#include "./Signal_PacketDef.h"

//# 신호 종류
#define SIGNAL_REAL_KEY						"0170"
#define NEWS_REAL_KEY						  "0169"

#define _SIGNAL_REAL_KEY						0xAA
#define _NEWS_REAL_KEY						  0xA9


#define CODE_LEN						6
#define CODE_NAME_LEN				20
#define TIME_LEN						6
#define DATE_LEN						8
#define SIGNAL_NAME_LEN			40
#define SIGNAL_DEGREE_LEN		2
#define IMG_INDEX_LEN       2

#define BROADCAST_COMPONENT			0x01
#define BROADCAST_ALL						0x02
#define BROADCAST_SIGNAL			  0x04

#define COMPOMENT_NORMAL				0x01
#define COMPOMENT_ALRAM					0x02
#define COMPOMENT_TICKER				0x04
#define COMPOMENT_ORDER					0x08


//////////////////////////////////////////////////////////////////////////
// Common Real Sub Struct
typedef struct tagSignal_CommonInfo
{
	char    sManagement;                   // 관리종목 (0:O, 1:X)
  char    sControl;                      // 이상급등 종목 (0:O, 1:X)
  char    sTradeStop;                    // 거래정지 종목 (0:O, 1:X)
  char    sInvestAttention;              // 투자유의 종목 (0:O, 1:X)
  char    sInsincerityNotice;            // 불성실공시 종목 (0:O, 1:X)
  char    sConfidenceLimit;              // 신용제한 종목 (0:O, 1:X)
  char    sCodeNotice;                   // 종목공시 (0:O, 1:X)
  char    sJungriMeme;                   // 정리매매 종목 (0:O, 1:X)
  char    sMarket;                       // 시장구분  (1:Kospi, 2:Kosdaq)
  char    sDepositMoney[3];              // 증거금 비율 (100, 040, 033, 025)
  char    sPrice[8];                     // 현재가 (단위:원)
  char    sChgRate[6];                   // 전일대비등락율   Type(S999.99)
  char    sSign;                         // 전일대비등락부호
																					//							'1' : 상한
																					//							'2' : 상승
    																			//							'3' : 보합
    																			//							'4' : 하한
    																			//							'5' : 하락
    																			//							'6' : 기세상한
    																			//							'7' : 기세상승
    																			//							'8' : 기세하한
    																			//							'9' : 기세하락
    																			//							'0' : 기세보합
  char    sVolume[9];                    // 거래량 : Unit(1,000)
  char    sAmount[9];                    // 거래대금 : Unit(100,000,000)
} Signal_CommonInfo;

typedef struct tagSignal_HeadInfo
{
  char		sKey[4];
	char		sSize[4];
  char    sCompress[1];
} Signal_HeadInfo;
#define SZ_SIGNLA_HEADINFO   sizeof(Signal_HeadInfo)


typedef struct tagSignal_ConditionInfo_Head
{
	Signal_HeadInfo		stHeadInfo;

  char    sAlertNum[4];      /* 신호 키정보(앞의 4자리 채움, 이후 space) */
  char    sType[1];             /* System('S'), User('U')                   */
	char    sDate[8];							/* 신호 발생일자                            */
  char    sTime[6];							/* 신호 발생시간                            */
  char    sTrueCnt[4];					/* 현재만족 종목수(기존+신규)               */
  char    sDataCnt[4];					/* 얼럿 데이터 수                           */
	char    sStatus[1];						/* 조건이상: 'E', 조건 점검:'W'             */
																/* 조건 초기화: 'I', 조건 강제종료: 'K'     */
																/* 조건 정상: 'S'														*/
  char sFiller[7];
} Signal_ConditionInfo_Head;
#define SZ_SIGNAL_CINDITIONINFO_HEAD  sizeof(Signal_ConditionInfo_Head)

typedef struct tagSignal_ConditionInfo
{
  char		sJobFlag;						  /* 신규:'N', 재진입:'R', 탈퇴:'O'        */
	char		sCode[CODE_LEN];
	char		sName[CODE_NAME_LEN];
	char    gsEntryCnt[4];        /* 진입 횟수                    */

	Signal_CommonInfo stCommonInfo;		// CommonFilter 를 위한 수정
} Signal_ConditionInfo;
#define SZ_SIGNAL_CONDITIONINFO   sizeof(Signal_ConditionInfo)


typedef struct tagSignal_NewsInfo
{
	Signal_HeadInfo		stHeadInfo;

  DR_NEWS_KEY stNewsKey;
  Signal_CommonInfo stCommonInfo;		// CommonFilter 를 위한 수정

	char	sNewsPoint[2];		/* 뉴스 점수					*/
	char	sGoodPoint[2];		/* 호재 뉴스 점수				*/
	char	sBadPoint[2];			/* 악재 뉴스 점수				*/
	  
	char	sCodeCnt[3];			/* 종목 갯수					*/
	char	sCaseCnt[3];			/* 상황문 갯수					*/
  
	char	sCode[CODE_LEN];			/* 종목 코드 				*/
  char	sName[CODE_NAME_LEN]; /* 종목명    				*/
  char	sProfferID[2];			  /* 뉴스 제공처 ID		*/
	char	sProffer[20];				  /* 뉴스 제공처 			*/
  char	sProfferTypeID[2];    /* 제공처 분류 ID		*/
	char	sProfferType[50];	    /* 제공처 분류 			*/
	char	sTitle[255];				  /* 제 목 						*/

	//-----------------------------------------------------------------------------
	// Reason		: SubNewsSignalInfo 구조체에 대응된다.
	//			- CodeList(8*n) + CaseList(6*n)
	//-----------------------------------------------------------------------------
	//char	sCodeList[6*종목갯수];			  /* 종목코드리스트	*/
	//char	sCaseList[6*상황문갯수];		  /* 상황문리스트 
} Signal_NewsInfo;

typedef struct tagSignal_SubNewsInfo
{
	LPSTR	pCodeList;					// 종목 코드 리스트. { [구분(1) + 종목코드(7)] * 종목 개수	}
	LPSTR	pSituationwordList;	// 상황문 리스트.    { 상황문(6) * 상황문 개수  }
} Signal_SubNewsInfo;



/*
typedef struct tagSIGNAL_HEAD
{
  char		sSize[4];
  char    sType[4];
} SIGNAL_HEAD;
#define SZ_SIGNAL_HEAD  sizeof(SIGNAL_HEAD)

typedef struct tagSIGNAL_CONDITION
{
  SIGNAL_HEAD	stHEAD;
  
  char	sCode[CODE_LEN];								  // 종목코드
  char	sName[CODE_NAME_LEN];						  // 종목명
  char	sDate[DATE_LEN];								  // 발생 일자 
  char	sTime[TIME_LEN];								  // 발생 시간. HHMMSS
  char	sSignalImageIndex[IMG_INDEX_LEN];	// 신호강도에 해당하는 이미지 인덱스 셋팅 
  char	sSignalDegree[SIGNAL_DEGREE_LEN]; // 신호 강도. "-3" ~ "3"
  char	sOrderType;											  // 주문 타입. '0': 주문을 내지 않음. '1': 반자동, '2': 자동
  
  char	sSignalName[SIGNAL_NAME_LEN];		  // System Alert 인덱스에 해당하는 이름
} SIGNAL_CONDITION;
#define SZ_SIGNAL_CONDITION  sizeof(SIGNAL_CONDITION)


typedef struct tagSIGANL_NEWS
{
  SIGNAL_HEAD	stHEAD;
  
  DR_NEWS_KEY stNewsKey;

  char	sCode[CODE_LEN];									// 대표 종목 코드. 대표 종목이 발생하지 않는 경우 NULL
  char	sSignalImageIndex[IMG_INDEX_LEN];	// 신호강도에 해당하는 이미지 인덱스 셋팅 
  char	sSignalDegree[SIGNAL_DEGREE_LEN];	// 신호 강도. "-3" ~ "3"
  char	sOrderType;												// 주문 타입. '0': 주문을 내지 않음. '1': 반자동, '2': 자동

  char	sProfferID[2];		// 제공처 아이디
  char	sProffer[20];			// 제공처 명
  char	sProfferType[3];	// 제공처별 분류 아이디
  char	sProfferNM[30];		// 제공처별 분류명
  char	sTitle[300];			// 제목
    
  //# 2006.01.12 => Not User!
  char	sGoodPoint[2];   	// 호재 뉴스 점수. + 상황문 발생
  char	sBadPoint[2];		 	// 악재 뉴스 점수. - 상황문 발생.
} SIGANL_NEWS;
#define SZ_SIGANL_NEWS  sizeof(SIGANL_NEWS)
*/




typedef struct tagKeywordData
{
	int nIndex;

	int nType;		//# 0 : Sys, 1 : User
	CString strTitle;
	int  nBasePoint;

	CString strCode;
	CString strCode_S, strName_S;
	int nPoint_S;
	CString strCode_V, strName_V;
	int nPoint_V;
} KeywordData;

typedef struct tagConditionData
{
	int nIndex;

	CString strTitle;
	int  nBasePoint;

	CString strCode;
	CString strPath;
} ConditionData;

typedef struct tagConditionDataGroup
{
	CString strGroup;
	CList <ConditionData, ConditionData&> *plistData;
} ConditionDataGroup;


#define SETINIT_SIGNALDATA(pstSetSignalData)         \
  pstSetSignalData->nIndex = 0;                      \
  pstSetSignalData->strKey = "";                     \
	pstSetSignalData->nType = 0;                       \
                                                     \
	pstSetSignalData->stKeywordData.nIndex = 0;        \
	pstSetSignalData->stKeywordData.nType = 0;         \
	pstSetSignalData->stKeywordData.strTitle = "";     \
	pstSetSignalData->stKeywordData.nBasePoint = 0;    \
	pstSetSignalData->stKeywordData.strCode = "";      \
	pstSetSignalData->stKeywordData.strCode_S = "";    \
  pstSetSignalData->stKeywordData.strName_S = "";    \
	pstSetSignalData->stKeywordData.nPoint_S = 0;      \
	pstSetSignalData->stKeywordData.strCode_V = "";    \
  pstSetSignalData->stKeywordData.strName_V = "";    \
	pstSetSignalData->stKeywordData.nPoint_V = 0;      \
                                                     \
  pstSetSignalData->stConditionData.nIndex = 0;      \
	pstSetSignalData->stConditionData.strTitle = "";   \
	pstSetSignalData->stConditionData.nBasePoint = 0;  \
	pstSetSignalData->stConditionData.strCode = "";    \
	pstSetSignalData->stConditionData.strPath = "";    \
	                                                   \
	pstSetSignalData->strTitle = "";                   \
	pstSetSignalData->nPoint = 0;                      \
	pstSetSignalData->nSend_Signal = 0;                \
	pstSetSignalData->nSend_Ticker = 0;                \
	pstSetSignalData->nSend_Order = 0;                 \
	pstSetSignalData->nState = 0;

#define SETCOPY_SIGNALDATA(stSetSignalData, pstSetSignalData)             \
  pstSetSignalData->nIndex = stSetSignalData.nIndex;                      \
  pstSetSignalData->strKey = stSetSignalData.strKey;                      \
	pstSetSignalData->nType = stSetSignalData.nType;                        \
                                                                          \
	pstSetSignalData->stKeywordData.nIndex = stSetSignalData.stKeywordData.nIndex;        \
	pstSetSignalData->stKeywordData.nType = stSetSignalData.stKeywordData.nType;          \
	pstSetSignalData->stKeywordData.strTitle = stSetSignalData.stKeywordData.strTitle;    \
	pstSetSignalData->stKeywordData.nBasePoint = stSetSignalData.stKeywordData.nBasePoint;\
	pstSetSignalData->stKeywordData.strCode = stSetSignalData.stKeywordData.strCode;      \
	pstSetSignalData->stKeywordData.strCode_S = stSetSignalData.stKeywordData.strCode_S;  \
  pstSetSignalData->stKeywordData.strName_S = stSetSignalData.stKeywordData.strName_S;  \
	pstSetSignalData->stKeywordData.nPoint_S = stSetSignalData.stKeywordData.nPoint_S;    \
	pstSetSignalData->stKeywordData.strCode_V = stSetSignalData.stKeywordData.strCode_V;  \
  pstSetSignalData->stKeywordData.strName_V = stSetSignalData.stKeywordData.strName_V;  \
	pstSetSignalData->stKeywordData.nPoint_V = stSetSignalData.stKeywordData.nPoint_V;    \
                                                                                        \
  pstSetSignalData->stConditionData.nIndex = stSetSignalData.stConditionData.nIndex;    \
	pstSetSignalData->stConditionData.strTitle = stSetSignalData.stConditionData.strTitle;\
	pstSetSignalData->stConditionData.nBasePoint = stSetSignalData.stConditionData.nBasePoint;\
	pstSetSignalData->stConditionData.strCode = stSetSignalData.stConditionData.strCode;  \
	pstSetSignalData->stConditionData.strPath = stSetSignalData.stConditionData.strPath;  \
	                                                                                      \
	pstSetSignalData->strTitle = stSetSignalData.strTitle;                  \
	pstSetSignalData->nPoint = stSetSignalData.nPoint;                      \
	pstSetSignalData->nSend_Signal = stSetSignalData.nSend_Signal;          \
	pstSetSignalData->nSend_Ticker = stSetSignalData.nSend_Ticker;          \
	pstSetSignalData->nSend_Order = stSetSignalData.nSend_Order;            \
	pstSetSignalData->nState = stSetSignalData.nState;


typedef struct tagSetSignalData
{
	int nIndex;
  CString strKey;
  	
	int nType;			//# 0 : Keyword, 1 : Condition
	KeywordData			stKeywordData;
	ConditionData		stConditionData;
	
	CString strTitle;
	int  nPoint;

	int  nSend_Signal;
	int  nSend_Ticker;
	int  nSend_Order;

	int nState;
} SetSignalData;


typedef struct tagSubFilterInfo
{
	int nTime;
	int nData;
} SubFilterInfo;


#define CNT_CHANGEVAL		5
#define CNT_DIRECTION		8

typedef struct tagFilterInfo
{
	int nMarketType;			//# 0 : 전체, 1 : 코스피, 2 : 코스닥
	
	int nNowPriceChk;
	int nNowPriceUp, nNowPriceDown;
	
	int nChangRateChk;
	int nChangRateUp, nChangRateDown;
		
	int nChangValChk[CNT_CHANGEVAL];
	
	int nDealVolType;		//# 0 : 고정, 1 : Time
	int nDealVol;
	CList <SubFilterInfo, SubFilterInfo&>	*plistDealTimeVol;
		
	int nDealPriceType;	//# 0 : 고정, 1 : Time
	int nDealPrice;
	CList <SubFilterInfo, SubFilterInfo&>	*plistDealTimePrice;
	
  int nNewsType;      //# 0 : 전체, 1 : 뉴스, 2 : 공시

	int nDirectionChk[CNT_DIRECTION];

	CStringArray saOutCode;
} FilterInfo;
#define SZ_FILTERINFO	sizeof(FilterInfo)



#endif