#ifndef SERVER_TYPEDEF
#define SERVER_TYPEDEF

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

#define QUERY_STRJONGMOK_MIN		_T("HKST03010200")

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
#define BOJO_MSG_LENGTH	(400)

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
	char	sMonth[2];				/* 결산월						*/
	char	sETF[1];				/* ETF							*/
} MSTCode;


#define COND_MST_VER	1





/*  -------------------------------------------------------------
** 2009.03.24   sashia.lee
** Candle 검색 Packet
*/
#pragma pack(1)  // struct member alignment 1byte
#define		COUNT_CANDLE_DATA 5
typedef struct
{
	int     iHigh;
	int     iClose;
	int     iOpen;
	int     iLow;
	int		iBodyRate;// (-)면 이하, (+)값이면 이상
} CANDLE_IN_DATA;

typedef struct
{
	int     nTrIndex;
	int     iS_Time;						// 대상시간 - 일/주/월 등.. 조건검색과 동일
	int     iS_Range;						// 검색 범위[1~5]
	int     iSimilar;						// 유사도

	int    FieldEditerCnt;                  // 필드편집 개수   [Fix]
	int    FieldList[RESULT_FIELD_MAX];     // 필드편집 번호리스트 [JustFix]

	int     iPreGap;						// 기준 봉( X봉전 Data 조회)
	int     iPreTrand;						// 이전 추세 구분
	int     iIgnoreColor;					// 양음봉 구분 무시
	int		iIgnoreTail;					// 꼬리 무시
	int		iApplyBodyRate;					// 몸통길이 적용

	char    sTargetBit[RESULT_MAX];         // 검색 대상 종목

	CANDLE_IN_DATA      tCandleData[COUNT_CANDLE_DATA];		// 캔들 데이터
} CANDLE_IN;
#pragma pack()

typedef struct
{
	int    nTrIndex;

	int    lTotalCnt;						// 검색된 전제 종목 수
	int    lOutFieldCnt;                    // Out Field Count
	int    lOutListPacketSize;              // 결과 List 사이즈(한종목)
	int    lFindTime;                       // 검색시간

	char   sDateTime_Start  [14];			// 검색결과의 마지막날짜 YYYYMMDDhhmmss
	char   sDateTime_End    [14];			// 검색결과의 시작날짜

} CANDLE_OUT_HEAD;

typedef struct
{
	char    sJmcode         [ 7];
	char    Filler1         [ 1];

	char    sHname          [20];
	char    sSign           [ 1];
	char    sMarket         [ 1];
	char    sS_Time			[ 2];

	//  int     iTarget;                /* 일주월분     */
	char        sDateTime_Start [14];/* 검색결과의 마지막날짜 YYYYMMDDhhmmss */
	char        sDateTime_End   [14];/* 검색결과의 시작날짜  */

	double  dFieldData      [RESULT_FIELD_MAX];
} CANDLE_OUT_LIST;
// 2009.03.24   sashia.lee  ------------------------------------


typedef struct
{
	char            iHigh[8];               /*      고가            */
	char            iClose[8];              /*      종가            */
	char            iOpen[8];               /*      시가            */
	char            iLow[8];                /*      저가            */
	char            iBodyRate[8];           /*      등락율          */
} HKST03660000_IN2;

typedef struct
{
	char    nTrIndex[4];                    /* 콜구분                       */
	char    iS_Time[4];                     /* 대상시간                     */
	char    iS_Range[4];                    /* 검색범위                     */
	char    iSimilar[4];                    /* 유사도                       */
	char    FieldEditerCnt[4];              /* 필드편집개수                 */
	char    FieldList[240];                  /* 필드편집번호리스트           */
	char    iPreGap[4];                     /* 기준 봉( X봉전 Data 조회)    */
	char    iPreTrend[4];                   /* 이전 추세 구분                       */
	char    iIgnoreColor[4];                /* 양음봉 구분 없음.                    */
	char    iIgnoreTail[4];                 /* 꼬리 무시.                           */
	char    iApplyBodyRate;                 /* 등락율 적용 유무.    1:적용, order:적용하지 않음.    */
	char    sTargetBit[4000];         		/* 검색 대상 종목               */	//3000 -> 4000

	HKST03660000_IN2                        data[5];                /* */
} HKST03660000_IN1;

typedef struct
{
	char    nTrIndex[4];                /* 콜구분                       */
	char    lTotalJongCnt[4];           /* 검색종목수                   */
	char    lOutFieldCnt[4];            /* OutFieldCount                */
	char    lOutListPacketSize[4];      /* 결과List사이즈               */
	char    lFindTime[6];               /* 검색시간                     */
} HKST03660000_OUT1;

typedef struct
{
	char    sJmcode[7];                 /* 종목코드                     */
	char    sHname[20];                 /* 종목명                       */
	char    sSign[1];                   /* 등락부호                     */
	char    sMarket[1];                 /* 시장구분                     */
	char    sS_Time[2];
	char    sStartDateTime[14];         /* 검색된 시작시간          */
	char    sEndDateTime[14];           /* 검색된 종료시간          */
	char    dFieldData[60][16];         /* 유동필드결과값               */
} HKST03660000_OUT2;




typedef struct _S03010_I
{
	char    shcode		[15];		/* 종목코드								*/
	char    jangGB		[ 1]; 
	char	date		[8];		/* 검색 시작일							*/
	char	eDate		[8];		/* 검색 종료일							*/
	char	num			[4];		/* 검색할 데이터 갯수					*/
	char	unit		[3];		/* 틱/분/일 조회간격단위				*///<========2005.08.11
	char    button		[1];		/* 틱(0)/분(1)/일(2)/주(3)/월(4)/년(5)	*/
	char    modGb		[1];		/* 수정주가 구분 1:수정주가반영			*/	
	char	nflag		[1];		/* 다음(1) / 조회(0) 구분자 			*/
	char	nkey		[512];		/* 다음 / 이전 Fetch Key				*/
	char	nDataGubun	[1];		/* '0': 가격, '1':비율					*/
	char    ImgRgb		[1];        /* '0':허수봉 '1':실봉					*/
	char    Irecordgb	[1];		/* 복기 구분값(1)						*///<========2005.08.11
	char    nDayBefore	[2];		/* 복기 시작 n일전 						*///<========2005.10.14
	char    Itime		[6];		/* 복기시간 (hhmmss)					*///<========2005.08.11 
	char    nowData[1]; /* 당일조회(1:당일조회) */
	/// don't sent to server..
	char	chNull;
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
	char    tmp[5];                              /* 차트카운트(5)                   */
	char    bojomsg[400];                         /* 보조메시지(400)                 */
// 	char	name[20];			/* 종목명			*/
// 	char    price[10]; 		    /* 현재가   		*/
// 	char    sign[1];                              /* 등락부호(1)                     */
// 	char    change[10];     	/* 대비     		*/
// 	char    chrate[6];      	/* 대비율   		*/
// 	char    volume[10];     	/* 거래량   		*/
// 	char    preprice[10];                         /* 기준가(10)                      */
// 	char    nkey[512];           /* 이전키           */
// 	//------------- PosForChart
// 	char    tmp[5];         	/* Chart Count      */
// 	char    bojomsg[BOJO_MSG_LENGTH];    	/* Chart Bojo Data  90->256으로 바꿈 05.05.06*/
// 	S03010_S  chart[CHTMX];
} S03010_O;


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
	char		dFieldData[320];
}p0624OutBlock2;
#define SZ_p0624OutBlock2	sizeof(p0624OutBlock2)
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

