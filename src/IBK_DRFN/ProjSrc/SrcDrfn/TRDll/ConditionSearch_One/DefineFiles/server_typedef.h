#ifndef SERVER_TYPEDEF
#define SERVER_TYPEDEF

/************************************************************/
/*																													*/
/*	One Clk Condition Server =>  STRUCT/DEFINE/TYPEDEF 정의	*/
/*																													*/
/*					2005. 1.  			Doori														*/
/*					작성자 : 우동우																	*/
/************************************************************/

#include "../../Dr_Include/EU_p0622.h"
#include "../../Dr_Include/EU_p0628.h"	//테마조회

#define CONDITION_MAXCNT		50

#define TRNO_CONDIONECLK_LIST		"p0823"	//"p0623"			//"hfit_o071006"

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
	int		nTrIndex;

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
	char	sFiller		  [4];
	// 한투추가
	char  gdResultField   [960];	  /*  결과필드 (16 * 60 = 960)    */
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
	char        sName[36];					/* 테마 명						*/
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
  
  char	  sPBID[8];		/*한투 PBID*/
  char	  sDelTRid[12]; /*해제 TRrid*/
  
  char    sErrMsg[40];  /* 실패시 메시지                        */
} REAL_USER_OUT;
#define SZ_REAL_USER_OUT  sizeof(REAL_USER_OUT)

//DLL TO Main신호관리자
typedef struct	tagSIGNALMSGINFO
{
	TCHAR	m_sgType[2+1];		//  - 신호구분	: 신호 상세분류
	TCHAR	m_sgTime[6+1];		//  - 발생시간	: HH:MM:SS
	TCHAR	m_sgnskey[26+1];		//  - /* 날짜[8]+시간[6]+일련번호[8]+뉴스구분[2]+ 뉴스서브구분[2]
	TCHAR	m_sgSeqno[8+1];		//  - 일련번호
	TCHAR	m_sgCodeGb[1+1];		//  - 1:코스피,2:코스닥,3:프리보드,4:ELW
	TCHAR	m_sgCode[8+1];		//  - 종목코드	: 주식(1), 선물(2)/옵션(3), 업종지수(4)
	TCHAR	m_sgContext[512+1];	//  - 신호내용	: 뉴스타이틀 / 조건내용등
	TCHAR	m_sgGubun[1+1];		//  - 매도수 구분	: 일반(1) / 매수(2) / 매도(3)
	TCHAR	m_sgBlink[2+1];		//  - 신호속성	: 깜빡임 횟수
	TCHAR	m_sgContinue[1+1];	//  - 신호속성	: 유지여부
	TCHAR	m_sgPriority[2+1];	//  - 신호속성	: 우선순위 
	TCHAR	m_sgIcon[2+1];		//  - 신호속성	: 표시Icon Index
	TCHAR	m_sgBugaInfo[10+1];	//  - 부가정보	: DRFN에서 보내는 신호중 신호강도정보
} EUGENE_SIGNALMSGINFO;

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

