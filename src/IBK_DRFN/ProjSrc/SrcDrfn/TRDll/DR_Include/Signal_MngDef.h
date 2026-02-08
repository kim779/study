#ifndef SIGNALMNG_TYPEDEF
#define SIGNALMNG_TYPEDEF


/****************************************************************/
/*																*/
/*						 Signal Mng Packet (Signal_MngDef.h)	*/
/*																*/
/*						SignalMng에서 사용하는 신호 Packet!		*/
/*						SVR ->  SignalMng						*/
/*														(User)	*/
/*																*/
/*                   2006. 10.  		Doori					*/
/*                      작성자 : 우동우							*/
/****************************************************************/
#include <afxtempl.h>

#include "./Signal_PacketDef.h"

#define CODE_LEN						6
#define CODE_NAME_LEN					40	//@유진 20->40
#define TIME_LEN						6
#define DATE_LEN						8
#define SIGNAL_DEGREE_LEN				2
#define IMG_INDEX_LEN					2

#define BROADCAST_COMPONENT				0x01
#define BROADCAST_ALL					0x02
#define BROADCAST_SIGNAL				0x04

#define COMPOMENT_NORMAL				0x01
#define COMPOMENT_ALRAM					0x02
#define COMPOMENT_TICKER				0x04
#define COMPOMENT_ORDER					0x08

#define	WM_BC_DATA_RECEIVED				WM_USER + 703

#define GETPATH_APP(strAppPath)       \
	char buf[255];														\
	GetModuleFileName(NULL, buf, 255);									\
	int lastlocationofbackslash = 0;									\
	int nBufCnt = strlen(buf);											\
	for (int i = 0; i < nBufCnt; i++)									\
	{																	\
	if (buf[i] == '\\')													\
	lastlocationofbackslash = i;										\
	}                                                                   \
	CString strAppPath = _T("");                                        \
	for (int j = 0; j < lastlocationofbackslash; j++)                   \
	strAppPath += buf[j];  

//////////////////////////////////////////////////////////////////////////
// Common Real Sub Struct
typedef struct tagSignal_CommonInfo
{
	char    sManagement;                   // 관리종목 (0:O, 1:X)
	char	attr_smanagement;
	char    sControl;                      // 투자정상(0), 투자주의(1), 투자경고(2), 투자위험예고(3),투자위험(4)
	char	attr_scontrol;
	char    sTradeStop;                    // 거래정지 종목 (0:O, 1:X)
	char	attr_stradestop;
	char    sInvestAttention;              // 투자유의 종목 (0:O, 1:X)
	char	attr_sinvestattention;
	char    sInsincerityNotice;            // 불성실공시 종목 (0:O, 1:X)
	char	attr_sinsinceritynotice;
	char    sConfidenceLimit;              // 신용제한 종목 (0:O, 1:X)
	char	attr_sconfidencelimit;
	char    sCodeNotice;                   // 종목공시 (0:O, 1:X)
	char	attr_scodenotice;
	char    sJungriMeme;                   // 정리매매 종목 (0:O, 1:X)
	char	attr_sjungrimeme;
	char    sMarket;                       // 시장구분  (1:Kospi, 2:Kosdaq)
	char	attr_smarket;
	char    sDepositMoney[3];              // 증거금 비율 (100, 040, 033, 025)
	char	attr_sdepositmoney;	
	//# Code Info
	char    sPrice[8];           // 현재가 (단위:원)
	char	attr_sprice;
	char    sSign;               // 전일대비부호  
	char	attr_ssign;
	char    sChage[8];           // 전일대비    
	char	attr_schange;	
	char    sChgRate[6];         // 등락율   Type(S999.99)   
	char	attr_schgrate;	
	char    sVolume[9];          // 거래량 : Unit(1,000)   
	char	attr_svolume;	
	char    sVolumeRect[6];      // 거래량 대비율: Unit(1,000)  
	char	attr_svolumerect;	
	char    sAmount[9];          // 거래대금 : Unit(100,000,000)
	char	attr_samount;
	char	sResultField[1040];				// 한투 결과필드
} Signal_CommonInfo;

typedef struct tagSignal_CommonInfo2
{
	char    sManagement;                   // 관리종목 (0:O, 1:X)
	char    sControl;                      // 투자정상(0), 투자주의(1), 투자경고(2), 투자위험예고(3),투자위험(4)
	char    sTradeStop;                    // 거래정지 종목 (0:O, 1:X)
	char    sInvestAttention;              // 투자유의 종목 (0:O, 1:X)
	char    sInsincerityNotice;            // 불성실공시 종목 (0:O, 1:X)
	char    sConfidenceLimit;              // 신용제한 종목 (0:O, 1:X)
	char    sCodeNotice;                   // 종목공시 (0:O, 1:X)
	char    sJungriMeme;                   // 정리매매 종목 (0:O, 1:X)
	char    sMarket;                       // 시장구분  (1:Kospi, 2:Kosdaq)
	char    sDepositMoney[3];              // 증거금 비율 (100, 040, 033, 025)
	//# Code Info
	char    sPrice[8];           			// 현재가 (단위:원)
	char    sSign;               			// 전일대비부호  
	char    sChage[8];           			// 전일대비    
	char    sChgRate[6];         			// 등락율   Type(S999.99)   
	char    sVolume[9];          			// 거래량 : Unit(1,000)   
	char    sVolumeRect[6];      			// 거래량 대비율: Unit(1,000)  
	char    sAmount[9];          			// 거래대금 : Unit(100,000,000)

	char	sResultField[1040];				// 한투 결과필드
} Signal_CommonInfo2;

typedef struct tagSignal_HeadInfo
{
	char		sKey[4];
	char		attr_skey[1];
	char		sSize[4];   
	char		attr_ssize[1];
	char		sCompress[1];
	char		attr_scompress[1];
} Signal_HeadInfo;
#define SZ_SIGNLA_HEADINFO   sizeof(Signal_HeadInfo)

typedef struct tagSignal_HeadInfoAlert
{
	char		sKey[4];
} Signal_HeadInfoAlert;
#define SZ_SIGNLA_HEADINFO   sizeof(Signal_HeadInfo)



typedef struct tagSignal_ConditionInfo_Head
{
//	Signal_HeadInfoAlert	stHeadInfo;
	
	char    sAlertNum[4];         /* 신호 키정보(앞의 4자리 채움, 이후 space) */
	char    sKey[4];  
	char    sType[1];             /* System('S'), User('U')                   */
	char    sDate[8];							/* 신호 발생일자                            */
	char    sTime[6];							/* 신호 발생시간                            */
	char    sTrueCnt[4];					/* 현재만족 종목수(기존+신규)               */
	char    sDataCnt[4];					/* 얼럿 데이터 수                           */
	char    sStatus[1];						/* 조건이상: 'E', 조건 점검:'W'             */
	/* 조건 초기화: 'I', 조건 강제종료: 'K'     */
	/* 조건 정상: 'S'														*/
//유진삭제	char sFiller[7]; 
} Signal_ConditionInfo_Head;
#define SZ_SIGNAL_CINDITIONINFO_HEAD  sizeof(Signal_ConditionInfo_Head)

typedef struct tagSignal_ConditionInfo
{
	char		sJobFlag;						  /* 신규:'N', 재진입:'R', 탈퇴:'O'        */
	char		sCode[CODE_LEN];
	char		sName[20]; //[CODE_NAME_LEN];
	char    sEntryCnt[4];         /* 진입 횟수                    */
	
	Signal_CommonInfo stCommon;
} Signal_ConditionInfo;
#define SZ_SIGNAL_CONDITIONINFO   sizeof(Signal_ConditionInfo)

typedef struct tagSignal_ConditionInfo2
{
	char		sJobFlag;						  /* 신규:'N', 재진입:'R', 탈퇴:'O'        */
	char		sCode[CODE_LEN];
	char		sName[20]; //[CODE_NAME_LEN];
	char    sEntryCnt[4];         /* 진입 횟수                    */
	
	Signal_CommonInfo2 stCommon;
} Signal_ConditionInfo2;
#define SZ_SIGNAL_CONDITIONINFO2   sizeof(Signal_ConditionInfo2)


typedef struct tagNews_CommonInfo
{
	DR_NEWS_KEY stNewsKey;
	
	char	sDate				[8];		/* 뉴스 일자				*/
	char	sTime				[6];		/* 뉴스 시간				*/

	char	sNewsPoint			[2];		/* 뉴스 점수					*/
	char	sCode				[CODE_LEN];	/* 종목 코드 				*/
	char	sName				[40];		//@유진변경sName[CODE_NAME_LEN]; /* 종목명    				*/
	char	sProfferID			[2];		/* 뉴스 제공처 ID		*/
	char	sProffer			[20];		/* 뉴스 제공처 			*/
	char	sProfferTypeID		[3];		//@유진변경sProfferTypeID[2];    /* 제공처 분류 ID		*/
	char	sProfferType		[50];	    /* 제공처 분류 			*/
	char	sTitle				[255];		/* 제 목 						*/
} News_CommonInfo;

typedef struct tagSignal_NewsInfo     //# [OR] News All Input
{
	News_CommonInfo   stNewsCommon;
	char    sFiller[3];
	Signal_CommonInfo stCommon;
} Signal_NewsInfo;

typedef struct tagSignal_NewsInfo_Head    //# News Real Data Packet!
{
//	char	dgubun				[4];	/* 데이터구분   "NWPS"		*/
//	char	nwcode				[6];	/* 패킷구분코드 "NEWS01"	*/
	
	Signal_HeadInfo		stHeadInfo;
	Signal_NewsInfo   stNewsInfo;
	
//유진수정	char	sCaseLen[8];			/* 상황문 갯수				*/
	char	sCaseLen[2];			/* 상황문 갯수				*/
	//# + 상황문 리스트.    { 상황문(6) * 상황문 개수  }
	char	sCSCd[180];		/* 상황문 CodeList; 30*6	*/
} Signal_NewsInfo_Head;

#define TRCODE_NEWS_ALL			"htif_o001013"

//# News All Out Packet
typedef struct tagSignal_AllNewsInfo
{
	Signal_NewsInfo   stNewsInfo;
	
	char	sCodeLen[8];			/* 종목 갯수					*/
	char	sCaseLen[8];			/* 상황문 갯수				*/
	char  sBodyLen[12];
	/*
	//# + 종목 코드 리스트. { [구분(1) + 종목코드(7)] * 종목 개수	}
	//# + 상황문 리스트.    { 상황문(6) * 상황문 개수  }
	//# + 본문
	*/
} Signal_AllNewsInfo;

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
//#define CNT_DIRECTION		8
#define CNT_DIRECTION_0		8
#define CNT_DIRECTION_1		11	// 8+3

typedef struct tagFilterInfo
{
	int nInCodeType;        //# 0 : Market, 1 : PortPolio 
	CString strInCodeData;  //# [0 : 전체, 1 : 코스피, 2 : 코스닥] or [PortPolio Name]
	CStringArray *psaInCode;  //# User After Load!
	
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
	
	int nDirectionChk[CNT_DIRECTION_1];
	
	CStringArray saOutCode;
} FilterInfo;
#define SZ_FILTERINFO	sizeof(FilterInfo)


//-->@유진에서 추가
typedef struct __REALNEWS_HEAD
{
//	char	dgubun				[4];	/* 데이터구분   "NWPS"		*/
//	char	nwcode				[6];	/* 패킷구분코드 "NEWS01"	*/

//	char	sKey				[4];	/* sPrfCd(2)+sBrnchCd(2) */
//	char	sSize				[4];	/* dgubun(4)+nwcode(4) 제외한 구조체 크기 */
//	char	sCompress			[1];	/* '0'=FALSE; 압축유무:항상'0' */
	Signal_HeadInfo				stHeadInfo;

//	char	sNsKey				[26];	/* 날짜[8]+시간[6]+순서[8]+구분[2]+서브[2] */
	DR_NEWS_KEY					stNewsKey;
	char						attr_snskey[1];

	char	sDate				[8];	/* 뉴스 일자				*/
	char	attr_sdate			[1];
	char	sTime				[6];	/* 뉴스 시간				*/
	char	attr_stime			[1];
	char	sNsPnt				[2];	/* 뉴스 점수 = 총점			*/
	char	attr_snspnt			[1];
	char	sRepCd				[6];	/* 대표 코드 				*/
	char	attr_srepcd			[1];
	char	sRepNm				[40];	/* 대표 코드명				*/
	char	attr_srepnm			[1];
	char	sPrfCd				[2]; 	/* 뉴스 제공처 ID			*/
	char	attr_sprfcd			[1];
	char	sPrfNm				[20]; 	/* 뉴스 제공처				*/
	char	attr_sprfnm			[1];
	char	sBrnchCd			[3];	/* 뉴스 제공처별 분류 ID	*/
	char	attr_sbrnchcd		[1];
	char	sBrnchNm			[50];	/* 뉴스 제공처별 분류		*/
	char	attr_sbrnchnm		[1];	
	char	sTitle				[255];	/* 뉴스 제목				*/
	char	attr_stitle			[1];
//	char	sManagement			[1];	/* 0=정상,1=관리종목		*/
//	char	sControl			[1];	/* 0=정상,1=감리종목		*/
//	char	sTradeStop			[1];	/* 0=정상,1=거래정지/중단	*/
//	char	sInvestAttention	[1];	/* 0=정상,1=투자유의지정(KOSDAQ) */
//	char	sInsincerityNotice	[1];	/* 0=정상,1=불성실공시지정	*/
//	char	sConfidenceLimit	[1];	/* 0=정상,1=신용제한		*/
//	char	sCodeNotice			[1];	/* 0=정상,1=종목공시		*/
//	char	sJungriMeme			[1];	/* 0=장상,1=정리매매종목	*/
//	char	sMarket				[1];	/* 1=거래소,2=코스닥,3=프리보드 */
//	char	sDepositMoney		[3];	/* 증거금비율:100,040,033,025 */
//	char	sPrice				[8];	/* 현재가(단위:원)			*/
//	char	sSign				[1];	/* '3':하한,'4':하락,'5':보합,'6':상승,'7':상한 */
//	char	sChange				[8];	/* 전일대비					*/
//	char	sChgRate			[6];	/* 대비율					*/
//	char	sVolume				[9];	/* 거래량(단위:천주)		*/
//	char	sVolumeRect			[6];	/* 거래량대비율				*/
//	char	sAmount				[9];	/* 거래대금(단위:백만원)	*/
	Signal_CommonInfo			stCommon;

	char	sCSCnt				[2];	/* 상황문 개수				*/	
	char	attr_scscnt			[1];
	char	sCSCd				[180];	/* 상황문 CodeList; 30*6	*/	
	char	attr_scscd			[1];
} REALNEWS_HEAD;
#define SZ_REALNEWS_HEAD sizeof(REALNEWS_HEAD)
//<--

typedef struct stRealData
{
	BYTE	chCount;
	char	szKeyCode[4];
	int		nSize;
	LPSTR	pDataBuf;
	int		lRealStruct;
	BYTE	chRealType;
} ST_REALDATA;

#endif