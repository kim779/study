#ifndef SERVER_TYPEDEF
#define SERVER_TYPEDEF

//#include "../../inc/UseVector.h"
/************************************************************/
/*																													*/
/*	  Draw Condition Server =>  STRUCT/DEFINE/TYPEDEF 정의	*/
/*																													*/
/*					2008. 1.  			Doori														*/
/*					작성자 : 우동우																	*/
/************************************************************/

//#define TRNO_DRAW_RESULT		"50011"
//#define TRNO_UNIVERS				"07608"
//#define TRNO_MSTCODE        "50008"

#define	RESULT_MAX_OFFSET		1
#define RESULT_MAX				4000	//3000 -> 4000

#define QUERY_STRJONGMOK_MIN		_T("HKST03010200")//"p0612"		//분데이터 전용

#define TRNO_DRAW_RESULT			"HKST03680000"  //"hfit_o071009"	//"p0624"     //패턴 검색
#define TRNO_THEMEGROUP				"HFIBOA076220" //테마 그룹 조회
#define TRNO_THEMEITEM				"HFIBOA076210" //테마 종목 조회

#define LEN_CODE					7
#define LEN_NAME					20

#define	RESULT_FIELD_MAX			30		/* 결과필드 최대 개수 	*/
#define RESULT_FIELD_MAX_EUGENE     240

#define CNT_DRAW_VAL				120
#define CNT_DRAW_VAL_EUGENE			480

#define DEFAULTCNTATONCE (400)
#define GRDMX			(3000)
#define CHTMX			(5200)
#define CHTMXREQUEST	(5000)
#define BOJO_MSG_LENGTH	(256)


typedef struct tagMSTCode
{
	char    sMarket[1];
	char    sCode[7];
	char    sName[20];
	char    sSetPos[4];
	char    sUsjg[1];               /* 우선주   (0:해당없음,1:해당) */
	char    sUnder[1];              /* 관리종목                     */
	char    sStop[1];               /* 거래정지종목                 */
	char    sDishonest[1];          /* 불성실공시종목               */
	char    sConfidencelimit[1];    /* 신용제한종목?               */
	char    sJrmm[1];          	    /* 정리매매종목                 */
	char    sDrat[3];               /* 증거금비율                   */
	char    sMrktalrm[1];           /* 시장경보
								  1:투자주의, 2:투자경고
								  3:투자위험, 4:투자위험예고*/
	char	sMonth[2];				/* 결산월 */
	char	sETF[1];				/* ETF							*/
} MSTCode;


#define COND_MST_VER	1



typedef struct tagDRAW_DATA
{
    long        lAccuVal;               // 최근 가중치 적용여부
                                        //  : 1~ 10, 미적용시 0으로.. (0.005)
    long        lDraw_Cnt;              // 그리기 기간범위(20~120)
    long        lErrRate;               // 오차율 = 10
    long        lGapRate;               // 필터 범위 = 40
    long        lDraw_Value[CNT_DRAW_VAL];       // 그리기 좌표값(0~100)
                                        //  : 그리지 않을경우 999로 입력
} DRAW_DATA;

typedef struct tagDRAW_IN
{
    long nTrIndex;
    
    long nS_Time;           // 대상시간
														/* 0:일간, 1:주간, 2:월간, 3:1분, 4:3분	*/
														/* 5:5분,  6:10분, 7:15분, 8:30분, 9:60분*/
                            //  : 일/주/월 등.. 조건검색과 동일
    long lTarget;           // 대상데이터 => 4 고정
                            //   : 시,고,저,종,거래량(1,2,3,4,5) 
    long lMoveAvg;          // 이평기간 => 1고정
                            //   : 이평 미적용시 1 입력
    long lS_Pos;            // 검색 시작위치
    long lS_Range;          // 검색 범위
    long lSimilar;          // 유사도
    char cBoxFlag;					// 횡보 체크 여부 ( > 5 )
														//  : 횡보 체크시에 1로 체크

    long nFieldEditerCnt;								// 필드편집 개수   [Fix]
    long nFieldList[RESULT_FIELD_MAX];	// 필드편집 번호리스트 [Just Fix]
	                       
	char		sTargetBit[RESULT_MAX];  /* 검색 대상 종목		*/
    DRAW_DATA stDrawData;
} DRAW_IN;

#define SZ_DRAW_IN  sizeof(DRAW_IN)

typedef struct tagDRAW_OUT_HEAD
{
    long nTrIndex;
    
    long  lTotalCnt;			/* 검색된 전제 종목 수      */
    long  lOutFieldCnt;				/* Out Field Count          */
    long  lOutListPacketSize;	/* 결과 List 사이즈(한종목) */
    long  lFindTime;					/* 검색시간									*/
} DRAW_OUT_HEAD;


typedef struct tagDRAW_OUT_LIST
{
  char        sJmcode     [LEN_CODE];
  char        Filler1     [ 1];

  char        sHname      [20];
  char        sSign       [ 1];
  char        sMarket     [ 1];
  char        Filler2     [ 2];

  long        iTarget;    /* 일주월분     */
  char        sDateTime_Start [14];/* 검색결과의 마지막날짜 YYYYMMDDhhmmss */
  char        sDateTime_End   [14];/* 검색결과의 시작날짜  */

  double      dFieldData[RESULT_FIELD_MAX];
} DRAW_OUT_LIST;
#define SZ_DRAW_OUT_LIST  sizeof(DRAW_OUT_LIST)

////////////////////////////////////////////////////
typedef struct
{
	char		nTrIndex[4];
	char		lTotalJongCnt[4];
	char		lOutFieldCnt[4];
	char		lOutListPacketSize[4];
	char		lFindTime[6];
}p0624OutBlock1;
#define SZ_p0624OutBlock1	sizeof(p0624OutBlock1)

typedef struct 
{
	char		sJmcode[7];
	char		sHname[20];
	char		sSign[1];
	char		sMarket[1];
	char		iTarget[4];
	char		sDateTime_Start[14];
	char		sDateTime_End[14];
//	char		dFieldData[320];
	char		dFieldData[960];
}p0624OutBlock2;
#define SZ_p0624OutBlock2	sizeof(p0624OutBlock2)
/////////////////////////////////////////////////////


typedef struct _S03010_I
{
	char    shcode		[ 15];  /* 종목코드        */
	char    jangGB		[ 1];                            /* 시장구분(1)                     */
	char	date		[ 8];  /* 검색 시작일       */
	char    eDate		[ 8];  /* 검색 종료일 */
	char	num			[ 4];  /* 검색할 데이터 갯수     */
	char	unit		[ 3];  /* 틱/분/일 조회간격단위    *///<========2005.08.11
	char    button		[ 1];  /* 틱(0)/분(1)/일(2)/주(3)/월(4)/년(5) */
	char    modGb		[ 1];  /* 수정주가 구분 1:수정주가반영   */ 
	char	nflag		[ 1];  /* 다음(1) / 조회(0) 구분자    */
	char	nkey		[512];  /* 다음 / 이전 Fetch Key    */
	char	nDataGubun	[ 1];  /* '0': 가격, '1':비율     */
	char    ImgRgb		[ 1];  /* '0':허수봉 '1':실봉     */
	char    Irecordgb	[ 1];  /* 복기 구분값(1)      *///<========2005.08.11
	char    nDayBefore	[ 2];  /* 복기 시작 n일전       *///<========2005.10.14
	char    Itime		[ 6];  /* 복기시간 (hhmmss)     *///<========2005.08.11  
	char        nowData[1]; /* 당일조회(1:당일조회) */
	/// don't sent to server..
	char chNull;
} S03010_I;
#define S03010_I_SIZE	sizeof(S03010_I)
//--------------------------------[OUTPUT반복 ]---------------------------------------//

// Chart Equity History Info Structure(In Server)
typedef struct _S_03010_S
{
	char    date[8];			/* 날짜     		*/
	char    open[10];			/* 시가     		*/
	char    high[10];			/* 고가     		*/
	char    low[10];			/* 저가     		*/
	char    close[10];			/* 종가   			*/
	char    volume[10];     	/* 거래량   		*/
	char	rights[1];			/* 배당락, 권리락..	*/
//	char	rightsrate[6];			/* 배당락, 권리락..	*/
} S03010_S;


// Chart Equity History Info Structure (In Client)
typedef struct _S03010_CHART
{
	char		tmp		[ 5];				/* Chart Count      */
	char		bojomsg	[BOJO_MSG_LENGTH];  /* Chart Bojo Data  */
	S03010_S	chart	[CHTMX];
} S03010_CHART;

//--------------------------------[OUTPUT     ]-----------------------------------//
// Chart Equity  Info Structure (In Server's Output )
typedef struct _S03010_O 
{
	char	name[30];			/* 종목명			*/
	char    price[10]; 		    /* 현재가   		*/
    char    open[10];			/* 시가             */
    char    high[10];			/* 고가             */
    char    low[10];			/* 저가             */
	char    sign[1];        	/* 등락부호 		*/
	char    change[10];     	/* 대비     		*/
	char    chrate[6];      	/* 대비율   		*/
	char    volume[10];     	/* 거래량   		*/
	char    nkey[21];           /* 이전키           */
	//------------- PosForChart
	char    tmp[5];         	/* Chart Count      */
	char    bojomsg[BOJO_MSG_LENGTH];    	/* Chart Bojo Data  90->256으로 바꿈 05.05.06*/
	S03010_S  chart[CHTMX];
} S03010_O;


typedef struct
{
	char    name[20];                             /* 종목명(20)                      */
	char    price[10];                            /* 현재가(10)                      */
	char    sign[1];                              /* 등락부호(1)                     */
	char    change[10];                           /* 대비(10)                        */
	char    chrate[6];                            /* 대비율(6)                       */
	char    volume[10];                           /* 거래량(10)                      */
	char    preprice[10];                         /* 기준가(10)                      */
	char	sellprice[10];						// 최우선 매도호가
	char	buyprice[10];						// 최우선 매수호가
	char	prevolume[18];						// 전일거래량
	char	prevolchrate[18];					// 전일거래량 대비
	char	volcycle[11];						// 거래량회전율
	char	value[18];							// 거래대금
	char	defprice[11];						// 액면가
	char	stockcount[18];						// 상장주식
	char	defvalue[22];						// 자본금
	char	totalprice[18];						// 시가총액
	char	PER[11];							// PER
	char	EPS[14];							// EPS
	char	PBR[11];							// PBR
	char    nkey[512];                     /* 이전키(14)                      */
	char    fcnt[5];                              /* 차트카운트(5)                   */
	char    bojomsg[400];                         /* 보조메시지(400)                 */
// 	char    name[20];                             /* 종목명(20)                      */
// 	char    price[10];                            /* 현재가(10)                      */
// 	char    sign[1];                              /* 등락부호(1)                     */
// 	char    change[10];                           /* 대비(10)                        */
// 	char    chrate[6];                            /* 대비율(6)                       */
// 	char    volume[10];                           /* 거래량(10)                      */
// 	char    preprice[10];                         /* 기준가(10)                      */
// 	char    nkey[512];                             /* 이전키(14)                      */
// 	char    fcnt[5];                              /* 차트카운트(5)                   */
// 	char    bojomsg[400];                         /* 보조메시지(400)                 */
// 	S03010_S  chart[CHTMX];
} EU_p0602_OutRec1_EX;






/* OutRec2:                                                                          */
// 사용하지 않는 Field 주석처리
//char EU_p0602_OutRec2_count[4];
typedef struct
{
	char    date[8];                              /* 날짜(8)                         */
	char    open[10];                             /* 시가(10)                        */
	char    high[10];                             /* 고가(10)                        */
	char    low[10];                              /* 저가(10)                        */
	char    close[10];                            /* 종가(10)                        */
	char    volume[10];                           /* 거래량(10)                      */
	char    rights[1];                            /* 권리락(1)                       */
} EU_p0602_OutRec2;



typedef struct    
{
	char	shcode		[15];	/* 종목코드			*/
	char	date		[8];	/* 검색시작일		*/
	char	num			[4];	/* 조회건수			*/
	char	unit		[3];	/* 조회단위(틱/분/일)								*/	
	char	button		[1];	/* 조회구분(틱(0)/분(1)/일(2)/주(3)/월(4)/년(5))	*/
	char	modGb		[1];	/* 수정주가(1:수정주가반영)							*/
	char	nflag		[1];	/* 조회모드(다음(1) / 조회(0) 구분자)				*/
	char	nkey		[21];	/* 검색키(다음 / 이전 Fetch Key)					*/
	char	nDataGubun	[1];	/* 가격구분('0': 가격, '1':비율)					*/
	char	ImgRgb		[1];	/* 봉구분('0':허수봉 '1':실봉)						*/
	char	Irecordgb	[1];	/* 복기구분값(1)	*/
	char	nDayBefore	[2];	/* 복기시작n일전	*/
	char	Itime		[6];	/* 복기시간(hhmmss) */
	char        nowData[1]; /* 당일조회(1:당일조회) */
	// don't sent to server..
	char	chNull;


}p0602InBlock;

typedef struct
{
	char	name		[20];	/* 종목명		0	*/
	char	jang		[10];	/* 장구분		1	*/
	char	price		[10];	/* 현재가		2	*/
	char	sign		[1];	/* 등락부호		3	*/
	char	change		[10];	/* 대비			4	*/
	char	chrate		[6];	/* 대비율		5	*/
	char	volume		[10];	/* 거래량		6	*/
	char	offer		[10];	/* 매도호가		7	*/
	char	bid			[10];	/* 매수호가		8	*/
	char	preprice	[10];	/* 기준가		9	*/
	char	nkey		[14];	/* 이전키		10	*/
	char	cntsz		[6];	/* 차트사이즈	11	*/
	char	fcnt		[5];	/* 차트카운트	12	*/
	char	bojomsg		[400];	/* 보조메시지	13	*/
}p0602OutBlock1;

typedef struct
{
	char	date		[8];	/* 날짜				*/
	char	open		[10];	/* 시가				*/
	char	high		[10];	/* 고가				*/
	char	low			[10];	/* 저가				*/
	char	close		[10];	/* 종가				*/
	char	volume		[10];	/* 거래량			*/
	char	value		[12];	/* 거래대금			*/
	char	rights		[1];	/* 권리락			*/
}p0602OutBlock2;


//////////////////////////////
typedef struct
{
	char		nTrIndex[4];
	char		iS_Time[4];
	char		iTarget[4];
	char		iMoveAvg[4]; 
	char		iS_Pos[4];
	char		iS_Range[4];
	char		iSimilar[4];
	char		sBoxFlag[1];
	char		FieldEditerCnt[4];
	char		FieldList[RESULT_FIELD_MAX_EUGENE];
	char		sTargetBit[RESULT_MAX];
	char		iAccuVal[4];
	char		iDraw_Cnt[4];
	char		iErrRate[4];
	char		iGapRate[4];
	char		iDraw_Value[CNT_DRAW_VAL_EUGENE];	

}p0624InBlock;
#define SZ_p0624InBlock sizeof(p0624InBlock)

// typedef struct
// {
// 	char		nTrIndex[4];
// 	char		lTotalJongCnt[4];
// 	char		lOutFieldCnt[4];
// 	char		lOutListPacketSize[4];
// 	char		lFindTime[6];
// }p0624OutBlock1;
// #define SZ_p0624OutBlock1	sizeof(p0624OutBlock1)
// 
// typedef struct 
// {
// 	char		sJmcode[7];
// 	char		sHname[20];
// 	char		sSign[1];
// 	char		sMarket[1];
// 	char		iTarget[4];
// 	char		sDateTime_Start[14];
// 	char		sDateTime_End[14];
// 	char		dFieldData[320];
// }p0624OutBlock2;
// #define SZ_p0624OutBlock2	sizeof(p0624OutBlock2)
///////////////////////////////////////////////////////////


typedef struct
{
	char		giCallIndex[4];
	char		gsCode[6];
	char		gsFilterGubun[1];
}p0628InBlock;
#define SZ_p0628InBlock sizeof(p0628InBlock)

typedef struct
{
	char		giCallIndex[4];
	char		ThemeCnt[4];
}p0628OutBlock1;
#define SZ_p0628OutBlock1 sizeof(p0628OutBlock1)

typedef struct
{
	char		gsThemeCode[4];
	char		gsThemeName[36];
}p0628OutBlock2;
#define SZ_p0628OutBlock2 sizeof(p0628OutBlock2)

typedef struct
{
	char		giCallIndex[4];
	char		giThemeCode[4];
}p0630InBlock;
#define SZ_p0630InBlock sizeof(p0630InBlock)

typedef struct
{
	char		giCallIndex[4];
	char		giCnt[4];
}p0630OutBlock1;
#define SZ_p0630OutBlock1 sizeof(p0630OutBlock1)

typedef struct
{
	char		gsItemCode[6];
	char		gsMarketFlag[1];
}p0630OutBlock2;
#define SZ_p0630OutBlock2 sizeof(p0630OutBlock2)

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

