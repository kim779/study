#define TRNO_DRAW		"50011"
#define RESULT_FIELD_MAX	20	/* 결과필드 최대 개수 	*/

/* 
** DRAW 검색 Packet
*/
typedef struct
{
    long        iAccuVal    ;               /* 최근 가중치 적용여부 : 1~ 10, 미적용시 0으로.. (0.005)*/
                                              
    long        iDraw_Cnt   ;               /* 그리기 기간범위(20~120)  */
    long        iErrRate    ;               /* 오차율				*/
    long        iGapRate    ;               /* 필터 범위			*/
    long        iDraw_Value [120];          /* 그리기 좌표값(0~100)	: 그리지 않을경우 999로 입력*/
                                              
} DRAW_DATA;
#define SZ_DRAW_DATA	sizeof(DRAW_DATA)

typedef struct
{
    long		nTrIndex;
    
    long        iS_Time     ;               /* 대상시간	: 일/주/월 등.. 조건검색과 동일		*/
                                              
    long        iTarget     ;               /* 대상데이터 => 4 고정	   : 시,고,저,종,거래량(1,2,3,4,5)		*/
                                            
    long        iMoveAvg   	;           /* 이평기간 => 1고정: 이평 미적용시 1 입력				*/
                                              
    long        iS_Pos      ;               /* 검색 시작위치		*/
    long        iS_Range    ;              /* 검색 범위			*/
    long        iSimilar    ;            /* 유사도				*/
    char		sBoxFlag;		/* 횡보 체크 여부	   : 횡보 체크시에 1로 체크	*/
	                                
	                                          
    long	    FieldEditerCnt;             /* 필드편집 개수   [Fix] */
    long	 	FieldList[RESULT_FIELD_MAX];	/* 필드편집 번호리스트 [Just Fix] */
	                                          
    DRAW_DATA   tDraw_Cond;
} DRAW_IN;
#define SZ_DRAW_IN		sizeof(DRAW_IN)

typedef struct
{
    long nTrIndex;
    
    long  lTotalJongCnt;		/* 검색된 전제 종목 수      */
    long  lOutFieldCnt;			/* Out Field Count          */
    long  lOutListPacketSize;		/* 결과 List 사이즈(한종목) */
    long  lFindTime;			/* 검색시간					*/
} DRAW_OUT_HEAD;
#define SZ_DRAW_OUT_HEAD	sizeof(DRAW_OUT_HEAD)

typedef struct
{
    char        sJmcode     [6];
    char        sHname      [20];
    char        sSign       [1];
    char        sMarket     [1];

    char        sSimilar    [6];

    long        iTarget;	//# 일주월분
    char        sDateTime_Start [14];
    char        sDateTime_End   [14];
    
    double	dFieldData[RESULT_FIELD_MAX];	//# 5
       
    /*
    char        sPrice      [9];	//현재가
    char        sChange     [8];	// 대비
    char        sChgRate    [6];	//등락률
    char        sVolume     [12];	//거래량
    char        sVolRate    [8];	//전일비
    */
} DRAW_OUT_LIST;
#define SZ_DRAW_OUT_LIST	sizeof(DRAW_OUT_LIST)