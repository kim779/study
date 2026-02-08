/********************************************************************/
/*																	                                */
/* 서비스 프로그램에서 사용하는 STRUCT/DEFINE/TYPEDEF 정의			    */
/*																	                                */
/*										2003. 10. 28		TEXEN		                      */
/********************************************************************/
#ifndef SERVER_TYPEDEF
#define SERVER_TYPEDEF

#define TRCODE_FINDER					"80000"

#define		MAX_IDXCNT				20	/* 최대 지표 개수(내부)		*/

#define		REAL_IDXCNT				18	/* 최대 지표 개수(외부)		*/
#define		MAX_PARAM					3		/* param 의 최대 개수 		*/
#define		MAX_CONDI					6		/* condition의 최대 개수	*/
#define		EXPLEN						80	/* 조건색의 최대 길이 		*/

#define		RESULT_MAX				2000	/* 최대 종목 수         */
#define		RESULT_FIELD_MAX	20		/* 결과필드 최대 개수 	*/

/********************************************************************/
/*  조건검색 INPUT DATA                                             */
/********************************************************************/
typedef struct
{
	long    IndexNo; 					/* 지표 index						*/
	long    GroupNo; 					/* 소속 그룹번호				*/
	long    GroupIdxCnt;			/* 그룹내 지표 개수			*/
	long    TargetBase;				/* 0:일간, 1:주간, 2:월간, 3:1분, 4:3분	*/
														/* 5:5분,  6:10분, 7:15분, 8:30분, 9:60분*/
	long    Type; 						/* 지표의 종류					*/
														/* 00XX	: 기술적 지표		*/
														/* 10XX	: 캔들분석			*/
														/* 20XX	: 시세지표			*/
														/* 30XX	: 재무지표			*/
														/* 40XX	: 기본적 분석		*/
														/* 50XX	: 시그널 지표 	*/
														/* 60XX	: 전략					*/
														/* 70XX : 패턴 (new)    */
														/* 80XX : 뉴스 (new)    */
	long    Type1; 						/* 조건 형태										  */
														/* 0: 기준값 돌파(상향/하향)      */
														/* 1: 기준선 돌파(상향/하향)      */
														/* 2: 기준값 비교(비교) 				  */
														/* 3: 기준선 비교(비교) 				  */
														/* 4: 값 비교(비교,<,<=,>,>=,==)  */
														/* 5: 범위 비교(c1이상~c2이하)    */
														/* 6: 추세(상승/하락)             */
														/* 7: 반전(상승/하락)             */
														/* 8: 순위검색									  */
														/* 9: 배열(정배열/역배열)         */
														/* 10: n% 종목 순위검색1	        */
														/* 11: n% 종목 순위검색2 	        */
														/* 20: 뉴스 체크 (new)            */
														/* 30: 패턴 체크 (new)            */
														/* 90: 기준값 돌파(상향/하향)		##삭제##*/ 
														/* 91: 기준선 돌파(상향/하향)		##삭제##*/
														/* 92: 기준값 비교(비교)    		##삭제##*/
														/* 93: 기준선 비교(비교)    		##삭제##*/
														/* 94: 값비교(비교,<,<=,>,>=,==)##삭제##*/
														/* 95: 범위 비교(c1이상~c2이하) ##삭제##*/
														/* 96: 추세(상승/하락)          ##삭제##*/
														/* 97: 반전(상승/하락)          ##삭제##*/
														/* 99:기타1                     ##삭제##*/
														/* 98:기타2                     ##삭제##*/
	long    Type2; 						/* 상세 형태						          */
														/* 0: 상향/이상/상승/정배열/매수  */
														/* 1: 하향/이하/하락/역배열/매도  */
														/* 2: 전일기준(가격지표)        ##삭제##*/
														/* 3: 당일기준(가격지표)        ##삭제##*/
														/* 99:기타                        */
	long    STerm;            /* n봉전 기준               		  */
	long    ETerm;            /* n봉이내                 			  */
	long    Rank;             /* 순위검색 종목 수               */
	long    Trend;            /* 추세횟수											  */
	long		SignalCnt1;				/* 발생회수	(new)	X번에서 X번 만족*/
	long		SignalCnt2;				/* 발생회수	(new)									*/
	long    ResultViewNum;		/* 결과값보기 필드 갯수(0:사용안함)	*/    

	char    IndexType;        /* 검색구분 0:일반 1:순위 2:순차  */
	char    SearchType;       /* 검색형태 0:일반 1:사용자정의	2:뉴스 3:패턴   (new) */

	char    Operator;         /* 연산자'0':OR '1':AND '2':NEXT	*/
	char    Reverse;          /* 결과제외 '0':A, '1':Not A		  */
	char    IdxContiGubun;    /* 지표의 연속발생 구분
															'0':단일발생, 1:연속발생지표		*/
	char    Element;          /* 조건식 사용문자(A) (new)     */
	char		Filler[2];

	double  Param[MAX_PARAM];      	/* 지표관련 변수값 (ex.12,26,9)	*/
	double  Condition[MAX_CONDI];   /* 기타 설정 변수값					    */
} User_Define;
#define SZ_User_Define sizeof(User_Define)

/********************************************************************/
/*  사용자 정의 조건검색 패킷                                       */
/********************************************************************/
typedef struct
{
	long		TrIndex;
	long    TargetIdxCnt;                		/* 대상 지표 갯수       */
	long    TargetResultViewCnt;            /* 대상 값보기필드 개수 */
	char    TargetExp[EXPLEN];              /* 대상조건 조합 표현 식*/
	
	long    ComplexIdxCnt;                  /* 복합 지표 갯수       */
	long    ComplexResultViewCnt;           /* 복합 값보기필드 개수 */
	char    ComplexExp[EXPLEN];             /* 복합조건 조합 표현 식*/
	
	long    FieldEditerCnt;                 /* 필드편집 개수        */
	long    FieldList[REAL_IDXCNT];					/* 필드편집 번호리스트  */

	long		ScriptCnt;											/* 스크립트 개수 (new)	*/
	
	/*
	char    TargetBitChk;                   // TargetBit 사용 여부 체크
	char		Jongmoklist[RESULT_MAX][6];			// 관심그룹 종목코드
	char    Filler;                      		// Filler
	*/
	
	//# 2004. 04 => 변경!
	char    TargetBitChk;                   	/* TargetBit 사용 여부 체크 */
  char    TargetBitlist[RESULT_MAX];
  char    Sort_flag;
    
  long    BackDay;				/* backtest 일자(미사용:-1) */	
  long    BackTime;				/* backtest 시간(미사용:-1) */	
} User_Finder;
#define SZ_User_Finder  sizeof(User_Finder)

/********************************************************************/
/*  Output Packet Struct                                            */
/********************************************************************/
typedef struct {
	long		TrIndex;
	long    TotalJongCnt;			  /* 검색된 전제 종목 수      */
	long    OutFieldCnt;			  /* Out Field Count          */
	long    OutListPacketSize;  /* 결과 List 사이즈(한종목) */
	long	  FindTime;					  /* 검색시간									*/

	char    ResultBit[RESULT_MAX];	    /* 결과종목 bit list        */
} OutPacket;
#define SZ_OutPacket sizeof(OutPacket)

/************************************************************************/
/*  Output List Packet Struct                                           */
/************************************************************************/
typedef struct {
	char	Daebi;					/* 전일대비 부호 	*/ // 1:상한 2:상승 3:보합 4:하한 5:하락
	char	Code[6];				/* 종목 코드	 		*/
	char	KoreanName[20];	/* 종목명		 			*/ 
	char	Filler[5];
	long	OrderRank;			/* 순위검색 순위 	*/	
	long	BasePrice;
	long	ResultField[RESULT_FIELD_MAX]; 
} OutListPacket;
#define SZ_OutListPacket sizeof(OutListPacket)

/************************************************************************/
/*  사용자 함수 패킷 list(사용자 함수 개수를 헤더에 삽입) (new)         */
/************************************************************************/
typedef struct
{
    char    FuncName[20];		/* 사용자 함수 명		*/
    long    IndexNo;        /* 해당 지표 번호   */
    long    ScriptLen;      /* 스크립트의 길이  */
} User_Script;








//# [영업일] ##########################3
#define JOBDATE_MAX  9999

#define TRCODE_JOBDATE				"92820"

typedef struct tagJobDate_In
{
  char    date[8];
  char    count[4];
  char	  chNull;
} JobDate_In;
#define SZ_JobDate_In sizeof(JobDate_In)

typedef struct tagJobDate_Out
{
	char        count[4];
	char				date[JOBDATE_MAX][8];
} JobDate_Out;
#define SZ_JobDate_Out sizeof(JobDate_Out)

#endif // SERVER_TYPEDEF

