/************************************************************/
/*																													*/
/*	         Server =>  STRUCT/DEFINE/TYPEDEF 정의	        */
/*																													*/
/*					2005. 1.  			Doori														*/
/*					작성자 : 우동우																	*/
/************************************************************/

#ifndef SERVER_TYPEDEF
#define SERVER_TYPEDEF

#define ERR_EXPCODE						"E00010"

#define LEN_CODE      7
#define LEN_UPCODE    4

// #define TRNO_GROUPTOBASE      "HKST03610100"/*"d1029"*/
// #define TRNO_GROUPTOBASE_UP   "HKST03610000"/*"d1020"*/

#define TRNO_TRNO_MSTCODE	  "HKST01100200"

#define TRNO_GROUPTODATE      "30503"

#define CODE_MAX_CNT		    50
#define RESULT_MAX_CNT2		  /*400*/100/*180*/
#define RESULT_MAX_CNT		  /*400*/100/*180*/

typedef struct tagGroupToBase_In
{
	char sTrIndex[4];

	char sType[1];							/* 0:분, 일:1, 주:2, 월:3 */
	char sDate[8];
	char sTime[6];
	char sTerm[2];							/* 분 term                */
	char sCnt[3];						    /* 종목개수               */
	char sCodeList[CODE_MAX_CNT][LEN_CODE]; /* 종목코드리스트         */
} GroupToBase_In;
#define SZ_GROUPTOBASE_IN sizeof(GroupToBase_In)

typedef struct tagPriceData
{
    char    sPrice[9];       /* 가격             */
} PriceData;

typedef struct tagBaseData_Code
{
  char sMarket[1];
  char sCode[7];		/* 종목코드 */
  char sName[20];		/* 종목코드 */
  char sPrice[9];		/* 현재가   */
  char sSign[1];		/* 등락부호 */
  char sChange[9];		/* 전일대비 */
  char sChgRate[7];		/* 등락율   */
  char sPre[9];			/* 전일종가 */
} BaseData_Code;
#define SZ_BASEDATA_CODE   sizeof(BaseData_Code)

typedef CArray<BaseData_Code, BaseData_Code&>	ArrBaseData;

typedef struct tagCommonGroupToBase_Out
{
	char sTrIndex[4];
	char sCnt[3];
	char sDate[RESULT_MAX_CNT][8];
	char sTime[RESULT_MAX_CNT][6];
	char sDataCnt[3];
}CommonGroupToBase_Out;
#define SZ_COMMON_GROUPTOBASE_OUT sizeof(CommonGroupToBase_Out)

typedef struct tagGroupToBase_Out
{
	char sTrIndex[4];

	char sCnt[3];
	char sDate[RESULT_MAX_CNT][8];
	char sTime[RESULT_MAX_CNT][6];
	char sDataCnt[3];
	PriceData stPrice[CODE_MAX_CNT][RESULT_MAX_CNT2];
	BaseData_Code stBase[CODE_MAX_CNT];
} GroupToBase_Out;
#define SZ_GROUPTOBASE_OUT sizeof(GroupToBase_Out)


typedef struct tagGroupToBase_UPIn
{
	char sTrIndex[4];

	char  sType[1];           /* 0:분, 일:1, 주:2, 월:3 */
	char  sDate[8];
	char  sTime[6];
	char  sTerm[2];           /* 분 term                */
	char  sCnt[3];            /* 업종개수               */
	char  sCodeList[CODE_MAX_CNT][LEN_UPCODE];     /* 업종코드리스트         */
} GroupToBase_UPIn;
#define SZ_GROUPTOBASE_UPIN sizeof(GroupToBase_UPIn)

typedef struct tagBaseData_UpCode
{
  char sCode[4];   /* 업종코드 */
  char sPrice[9];  /* 현재지수 */
  char sOpen[9];   /* 시작지수 */
  char sPre[9];    /* 전일지수 */
} BaseData_UpCode;
#define SZ_BASEDATA_UPCODE   sizeof(BaseData_UpCode)

typedef struct tagGroupToBase_UPOut
{
	char sTrIndex[4];
	char sCnt[3];
	char sDate[RESULT_MAX_CNT][8];
	char sTime[RESULT_MAX_CNT][6];
	char  sDataCnt[3];
	/*PriceData stPrice[CODE_MAX_CNT][RESULT_MAX_CNT2];*/
	PriceData stPrice[CODE_MAX_CNT][RESULT_MAX_CNT2];
	BaseData_UpCode stBase[CODE_MAX_CNT];
} GroupToBase_UPOut;
#define SZ_GROUPTOBASE_UPOUT sizeof(GroupToBase_UPOut)

typedef struct tagMSTItemInfo
{
	int nMarket;
	char szItemName[20];
} MSTITEMINFO;
#define SZ_MSTITEMINFO		sizeof(MSTITEMINFO)

typedef CMap<CString, LPCTSTR, MSTITEMINFO, MSTITEMINFO&>	MSTItemInfoMap;

//관심조회관련 tr
struct _uinfo {
	char gubn[2]; // 업무구분
	   // "MY" : 관심종목
	   // "UL" : 사용자 데이터 목록 
	   // "UD" : 사용자 데이터
	char dirt[1]; // 전송방향 'U' : pc==>host, 'D' : host==>pc, 'X' : host==>pc (conversion), 'R' : 재조회
	char cont[1]; // 연속구분 'F':First, 'M':Middle, 'L':First&Last, 'C':Cancel, 
		 // 'S':그룹편집, 'E':그룹조합처리, 'b':DB백업, 'R':DB원복, 'V':최종상태저장, 'r':최종상태복원
		 // 'D':그룹삭제, 'j':해당그룹종목전송, 'g':그룹리스트조회, 'G':그룹종목전송, 'C':전송취소
	char name[80]; // file name
	char nblc[5]; // 블럭개수, 파일크기(max:64k)
	char retc[1]; // return value 'O':정상 'E':에러 'U'신규 정상
	char emsg[40]; // error message
};

struct _ginfo {
	char gnox[2]; // group number
	char gnam[20]; // group name
	char jrec[4]; // _jinfo count
};

struct _jinfo {
	char gubn[1]; // 종목구분 0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
	char code[12]; // 종목코드
	char xprc[10]; // 보유단가
	char xnum[10]; // 보유수량
};

struct _updn {
	_uinfo uinfo;
	_ginfo ginfo;
};

const int sz_uinfo = sizeof(struct _uinfo);
const int sz_ginfo = sizeof(struct _ginfo);
const int sz_jinfo = sizeof(struct _jinfo);
#endif // SERVER_TYPEDEF

