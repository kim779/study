#ifndef SERVER_TYPEDEF
#define SERVER_TYPEDEF

/************************************************************/
/*																													*/
/*	One Clk Condition Server =>  STRUCT/DEFINE/TYPEDEF 정의	*/
/*																													*/
/*					2005. 1.  			Doori														*/
/*					작성자 : 우동우																	*/
/************************************************************/

#define CONDITION_MAXCNT		50

#define TRNO_CONDIONECLK_LIST			"p0823"//"p0623"			//"hfit_o071006"
//#define TRNO_CONDIONECLK_RESULT			"d1022"//"p0622"			//"hfit_o071007"
#define TRNO_TEMACLK_LIST				"p0828"//"p0628"			//"hfit_o71008		"37006"
//#define TRNO_TEMACLK_RESULT				"37007"
//#define TRNO_ALERT_REALREG				"hfit_o071002"


//# 원클릭 종목 조회!
typedef struct
{
	int			nTrIndex;
	char        sType;      //# '1' : 예상체결, '0' : 그외
	char        sFiller[3];

	//char        sFilterGubun;          /* 사용: 1, 미사용: 0               */
	//User_Finder stFilter;              /* 필터조건                         */

	User_Finder stCondition;           /* 검색조건                         */
} TR_RESULT_IN;
#define SZ_TR_RESULT_IN  sizeof(TR_RESULT_IN)

//# 테마 종목 조회
typedef struct
{
	int         nTrIndex;

	char        sFilterGubun;			/* 사용: 1, 미사용: 0               */
	User_Finder stFilter;					/* 필터조건                         */
	char        sCode[4];					/* 테마 코드												*/
} TR_RESULT_TEMA_IN;
#define SZ_TR_RESULT_TEMA_IN  sizeof(TR_RESULT_TEMA_IN)

typedef struct
{
	int		giCallIndex;

	char    sJongCnt       [4];        /* 검색종목수                       */
	char    sHigh          [4];        /* 상한 종목수                      */
	char    sUp            [4];        /* 상승 종목수                      */
	char    sNoch          [4];        /* 보합 종목수                      */
	char    sDown          [4];        /* 하락 종목수                      */
	char    sLow           [4];        /* 하한 종목수                      */
	char    sUpRate        [6];        /* 상승비중                         */
	char    sChgrateAvg    [6];        /* 평균등락율                       */
} TR_RESULT_HEADER;
#define SZ_TR_RESULT_HEADER  sizeof(TR_RESULT_HEADER)

typedef struct
{
    char    sMarket       [1];        /* 장구분                           */
    char    sCode         [6];        /* 종목코드                         */
    char    sName		  [20];       /* 종목명                           */
    char    sSignCnt      [3];        /* 등락연속봉수                     */
    
    char    sPrice        [8];        /* 현재가                           */
    char    sSign         [1];        /* 등락부호                         */
    char    sChange       [8];        /* 전일대비                         */
    char    sChgRate      [6];        /* 전일대비 등락율                  */
    char    sVolume       [9];        /* 거래량                           */
    char    sVolumeRate   [6];        /* 거래량 전일대비율                */
} TR_RESULT_LIST;
#define SZ_TR_RESULT_LIST    sizeof(TR_RESULT_LIST)
 
//# 조건 리스트 조회!
typedef struct
{
	int			nTrIndex;

	char        sCode[6];              /* 조회대상 종목                    */
	char        sFiller[2];
	int         nConditionCnt;         /* 검색조건 수                      */
	char        sRankGubun[CONDITION_MAXCNT];     //# Add : 2006.02.10 
	char        sFiller2[2];
	//User_Finder stCondition[50];      /* 검색조건                        */
} TR_LIST_IN;
#define SZ_TR_LIST_IN  sizeof(TR_LIST_IN)

typedef struct
{
	int		nTrIndex;
	int     nConditionCnt;             /* 검색 조건수                      */
  //char  nCondiIndexList[50];       /* 검색 조건 Index List             */
} TR_LIST_OUT;
#define SZ_TR_LIST_OUT sizeof(TR_LIST_OUT)

//# 테마 리스트 조회
typedef struct
{
	int         nTrIndex;            /* 클라이언트 조회 구분        */

	char        sCode[6];            /* 조회대상 종목               */
	char        sFilterGubun;        /* 사용: 1, 미사용: 0          */
	User_Finder stFilter;            /* 필터조건                    */
} TR_LIST_TEMA_IN;
#define SZ_TR_LIST_TEMA_IN sizeof(TR_LIST_TEMA_IN)

typedef struct
{
	int         nTrIndex;						/* 클라이언트 조회 구분 */
	int         nCnt;								/* 테마 항목 수			    */
} TR_LIST_TEMA_OUT_HEADER;
#define SZ_TR_LIST_TEMA_OUT_HEADER sizeof(TR_LIST_TEMA_OUT_HEADER)

typedef struct
{
  char        sCode[4];						/* 테마 코드			    */
	char        sName[20];					/* 테마 명						*/
} TR_LIST_TEMA_OUT;
#define SZ_TR_LIST_TEMA_OUT sizeof(TR_LIST_TEMA_OUT)



typedef struct tagREAL_USER_IN
{
  char    sType;          /* S : SYS, U : USER                    */
  char    sFlag;          /* 구분 등록:'E',변경:'C',멈춤:'P',검색:'R',삭제:'D'*/
  char    sAlertNum[4];   /* 등록시엔 space임...                      */
  char    sSendCnt[2];    /* 진입 신호 통보횟수                       */
  User_Finder stUserFinder;
  char    sUserID[20];    /* 사용자 ID                                */
} REAL_USER_IN;
#define SZ_REAL_USER_IN  sizeof(REAL_USER_IN)


typedef struct tagREAL_USER_OUT
{
  char    sType;        /* S : SYS, U : USER                    */
  char    sFlag;        /* 구분등록:'E',변경:'C',멈춤(시작):'P(S)',삭제:'D' */
  char    sResultFlag;  /* 성공:'s', 실aa7788패:'E', 사용자초과:'F'   */
  char    sTime[6];     /* 시간(HHMMSS)                         */
  char    sAlertNum[4];
  char    sErrMsg[40];  /* 실패시 메시지                        */
} REAL_USER_OUT;
#define SZ_REAL_USER_OUT  sizeof(REAL_USER_OUT)


#endif // SERVER_TYPEDEF

