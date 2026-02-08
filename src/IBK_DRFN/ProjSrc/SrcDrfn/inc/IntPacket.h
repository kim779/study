#ifndef __DOORI__INTPACKET__HEADER__
#define	__DOORI__INTPACKET__HEADER__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////WMLGT_NEW
typedef struct {
	char	Under		[  1];				/* 관/감리구분					*/
	char	Codegb		[  1];				/* 코드구분(공유를 위해)		*/
	char	code		[ 15];				/* 종목코드						*/
	char	Name        [ 30];				/* 종목명						*/
	char	Price		[  9];				/* 현재가						*/
	char	Change		[  6];				/* 전일대비(등락폭)				*/
	char	Rate		[  5];				/* 등락율(%)					*/
	char	Sign		[  1];				/* 등락부호						*/
	char	Volume		[ 12];				/* 누적거래량					*/
	char	Offerho		[  9];				/* 매도호가						*/
	char	Bidho		[  9];				/* 매수호가						*/
	char	Ofremain	[ 12];				/* 매도 최우선잔량				*/
	char	Bidremain	[ 12];				/* 매수 최우선잔량				*/
	char	Offertot	[ 12];				/* 총매도잔량					*/
	char	Bidtot		[ 12];				/* 총매수잔량					*/
	char	Open		[  9];				/* 시가							*/
	char	High		[  9];				/* 고가							*/
	char	Low			[  9];				/* 저가							*/
	char	Value		[ 14];				/* 거래 대금	                */ 
	char	UpLmtPrice	[  9];				/* 상한가						*/
	char	DnLmtPrice	[  9];				/* 하한가						*/
	char	Openyak		[  9];				/* 미결제약정수량				*/
	char	FacePrice	[  9];				/* 액면가       				*/
	char	YDSign		[  1];				/* 전일 등락부호				*/
	char	YSGubun		[  1];				/* 연속 구분					*/
	char	YSCount		[  2];				/* 연속 일수					*/
	char	Recprice	[  9];				/* 기준가						*/

	char    cvolume     [ 9];				/* 체결량           */
    char    open_chrate [ 5];				/* 시가전일대비율   */
    char    high_chrate [ 5];				/* 고가전일대비율   */
    char    low_chrate  [ 5];				/* 저가전일대비율   */
	char    high_time   [6];				/* 고가 시간*/
	char    low_time    [6];                /* 저가 시간*/
// 예상체결 추가 - cst 2004/12/09
    char    expsign         [ 1];			// 예상체결 부호                        
    char    expprice        [ 9];			// 예상체결 가                          
    char    expchang        [ 9];			// 예상체결 대비                        
    char    expchgrt        [ 9];			// 예상체결 대비율                      
    char    expvol          [12];			// 예상체결 거래량 
	char    color           [ 1];			/* 1:Blue 2:Yellow 3:Green 4:Red        */
//	char	Name		[ 20];				/* 종목명 						*/
//	char	pos			[  2];				/* 순서                         */   
//	char	Momentvol	[ 10];				/* 체결량						*/
//	char	Rights		[  2];				/* 액면분할여부 : 4*=액면분할	*/
//	char	HSign		[  1];				/* 상한가부호(선물,옵션,주옵만)	*/
//	char	LSign		[  1];				/* 하한가부호(선물,옵션,주옵만)	*/
//	char	NewsGb		[  2];				/* 뉴스 유무 bit setting		*/
//	char	Gsiyn		[  1];				/* 1개월 공시 유무				*/
} ST_INQUIRYDATA; // 05010의 응답.

#define	 CNTMAX   2000 
#define  SH_KSIZE  6
typedef struct
{
	char	gubun[2];         /* '01'상한 '02'상승 '03'보합 '04'하한 '05'하락 '06'신고 '07'신저*/
} TR_05021_I;

typedef struct _TR_05021_S{
	char    bit[1];           /* '1'장내(KOSPI) '2'장외(KOSDOQ) */
	char	code[SH_KSIZE];          /* 종목 코드 */
	char	hname[20];        /* 종목명 */
} TR_05021_S;

typedef struct
{
	char        pWnd[4];
	char        ogubun[2];    /* 구분 */
	char        totcnt[4];    /* total Count */
	TR_05021_S  data[1];
} TR_05021_O; 

typedef struct {
	char    pWnd[4];    
	char	Count[4];
	char	Data1[1];
} SSP_GETGCODE;

typedef struct
{
	char ncode[SH_KSIZE];
}TR08325_Item_i;

typedef struct
{
	char pWnd[4];
	char nsize[3];
	TR08325_Item_i nData[1];
}TR_08325;

//OUT
typedef struct
{
	char ncode[SH_KSIZE];   // 코드
	char ncur[9];		// 현재가(기준가)
	char nsign[1];		// 등락부호
}TR08325_Item_o;

typedef struct 
{
	char pWnd[4];
	char ncnt[3];
	char ndate[8]; //YYYYMMDD
	char ntime[6]; //HHMMSS

	TR08325_Item_o nData[1];	
}TR08325_o;

#define TR_ROLLDATA  "08325"
#define TR_INTEREST  "05010"	//관심 리스트 데이터 받기
#define TR_INTETC	 "05020"	//특이종목 현재가, 전일비 등 관심 데이터 받기.
#define TR_INTETC2	 "05021"	//특이종목리스트만 받기
#define TR_HAVEACCOUNT	"71020"
#define TR_IFLIST	 "20010"
#define TR_SPECIALITEM "20011"
#define TR_IFMYNET		"80000"

#define TR_ERR_DATANOTRECEIVE	0
#define TR_ERR_COMM				1
#define TR_OK					2
#define TR_INFOREMOVE			3

#define INT_COMM_TRRECEIVE		0
#define INT_COMM_TRRECEIVE2		1
#define INT_DATA_SENDTR			3
#define INT_DATA_RESETBACKUP	13


#define NOTIFY_REFLESHROLL  1	// 돌려보기에 업데이트 된 것을 알림
#define NOTIFY_RESETALLDATA 3   // 모든 관심 관련된 것에 업데이트된 것을 알미
#define NOTIFY_RESETALLETC1 6   // 관심에서 변경된 것을 요청 되었을때 처리 
                               // 보낸 놈은 처리 안하는 것임.
//2004.12.02 by LYH
#define NOTIFY_CHANGEDFILE   7   // 서버에서 파일 내려받기를 한 경우. 관심 파일 변경 처리

#define NOTIFY_ADVISE 5
#define NOTIFY_UNADVISE 4

// IN, OUT 동일한 형식으로 패킷을 주고 받을 것임.
#define REQUEST_ERROR		 0
#define REQUEST_INTALL		 1 // 관심 모든 정보 얻기(메모리 카피해야 함) 
#define REQUEST_REG_IF		 2 // 조건 등록 
#define REQUEST_REG_ITEM     3 // 관심종목 등록
#define REQUEST_REG_SIGNAL   4 // 뉴스 등록
#define REQUEST_REG_OPENDLG  5 // 관심 등록창 열기 
#define REQUEST_INTALLRETURN 6 // 관심 모든 정보 얻기(메모리 카피할 필요 없음)
#define REQUEST_GETCODE		 7 // 코드 메니져 얻기
#define REQUEST_GETIF		 8 // 임시 그룹 정보 얻기
#define REQUEST_GETZZIM		 9 // 사용 안함
#define REQUEST_INTMODIFYFALSE 10 // 
#define REQUEST_CHANGEGROUP 11
#define REQUEST_REG_NEWS    12
#define REQUEST_GETNEWS     13
#define REQUEST_REMOVEZZIM  14 // 사용 안함
#define REQUEST_SYNCGROUP   15
#define REQUEST_GETSYNCGROUP 16
#define REQUEST_GIVEENVINFO 17
#define REQUEST_CANOPENDLG		18
#define REQUEST_GROUPINFO	20
#define REQUEST_GETNOGROUP	21
#define REQUEST_MYGETNETLIB 22
typedef struct
{
//	char Iwho[2]; // 0 : 관심, 1 : 메인, 2 : 조건, 3 : 신호, 4 : 차트
	char Igubun[2]; // 0 : ERROR, 1 : 1전체 정보(종목리스트 포함), 2 : 조건 그룹 등록, 3 : 종목 등록    
	char Icnt[3]; // 그룹 수
	char IopenDlg[1]; // 0 : Open, 1 : No Open
	void* Idata;  // 그룹 데이터들(CPtrArray) 
	char Ititle[256];
}INT_TOTAL;

typedef struct
{
	char Ino[2];		// 그룹 번호 
	char Ititle[20];	// 그룹 명
	char Icnt[3];		// 그룹 안에 종목 수
	void* Idata;		//종목 데이터들(CPtrArray)
	char Itime[6];
	char Idate[8];
}INT_GROUP_ST;

/*
typedef struct
{
	char Icode[12];  // 코드  
	char Igubun[2];		   // 0:Kospi, 1:kosdoq,,,
	char Itime[6];		   // HHMMSS   시,분,초
	char Idate[8];		   // YYYYMMDD 년,월,일
	char Ivol[10];		   // 수량 	
	char Ival[10];		   // 편입가격
	void* Idata;
	char IName[30];
} INT_ITEM_ST;*/
typedef struct
{
	char Icode[12];  // 코드  
	char Igubun[2];		   // 0:Kospi, 1:kosdoq,,,
	char Itime[6];		   // HHMMSS   시,분,초
	char Idate[8];		   // YYYYMMDD 년,월,일
	char Ivol[10];		   // 수량 	
	char Ival[10];		   // 편입가격
	char ICheck[1];		   // 체크유무
	void* Idata;
	char IName[30];
} INT_ITEM_ST;

//<<<<<<<<<<<<  리스트검색 : 20010 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef struct _D20010_I
{
    char    dtGb        [ 1];   /* 0:전체 1:검색 2:종목 */
    char    Gb          [ 1];   /* 1:시세 2:투자자 3:지표신호 4:추천전략*/
    char    jmcode      [ 6];   /* 종목코드             */
    char    text        [50];   /* 검색어               */
} D20010_I;

typedef struct _D20010_OG
{
    char    grptmnm     [30];   /* 테마명   */
    char    grptmcd     [ 4];   /* 테마코드 */
} D20010_OG;

typedef struct _D20010_O
{
	char		dtGb		[1];
	char		Gb			[1];
    char        grdCnt      [ 4];
    char		MsgCnt      [ 2];
    char		grdBjMsg    [18];
    D20010_OG   grdData     [1];
} D20010_O;


//<<<<<<<<<<<<  종목검색 : 20011 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef struct _D20011_I
{
    char    grptmcd      [ 4];  /* 테마코드 */
} D20011_I;

typedef struct _D20011_OG
{
    char    sign        [ 1];
    char    hname       [20];
    char    jmcode      [ 6];
    char    price       [ 9];
    char    change      [ 9];
    char    chgrate     [ 9];
    char    volume      [10];
} D20011_OG;

typedef struct _D20011_O
{
	char    grptmcd			[ 4];  /* 테마코드 */
    char        grptmnm     [30];

    char        sang        [ 3];
    char        up          [ 3];
    char        bo          [ 3];
    char        dn          [ 3];
    char        ha          [ 3];
    char        tot         [ 5];

    char        uprate      [ 6];
    char        rate        [ 6];
    char        uprate_sign [ 1];

    char        grdCnt      [ 6];
    char        grdBjMsg    [18];
    D20011_OG   grdData     [1];
} D20011_O;

typedef struct _D71020_I
{
    char    nxtGb       [ 1];       /* 다음, 연속 구분          */
    char    winid       [ 4];       /* 메인쪽에서 전송해온 DATA */
    char    gejanum     [11];       /* 계좌번호                 */
    char    jmcode1     [12];       /* 종목코드:전체조회시는 공백으로 전송  */
	char    singb       [ 1];       /* 신용구분:전체조회시는 공백으로 전송  */
    				    /* 처음 조회시에는 다음조회에 넣어야할 값들은 모두 
    				       공백으로 넣어서 전송바람.	    */
    char    jmcode      [ 7];       /* 'A'&종목코드:다음조회시 시작종목코드 */
    char    loanDay     [ 8];       /* 다음조회시 대출일자      */
    char    sinGb       [ 2];       /* 신용구분 : 00            */
    char    OnOfftp     [ 2];       /* On Off tp: 00            */
} D71020_I;

typedef struct _D71020_OG
{
									/* '1':장내, '2':코스닥,  '3':선물      */
									/* '4':옵션, '5':제3시장, '6':주식옵션  */
    char    mkgb        [ 1];       /* 시장구분         */
    char    jmcode      [ 7];       /* 종목코드         */
    char    hname       [20];       /* 종목명           */
    char    change      [ 9];       /* 대비             */
    char    chgrate     [ 9];       /* 대비율           */
    char    price       [ 9];       /* 현재가           */
    char    volume      [ 9];       /* 잔고수량         */
    char    danga       [ 9];       /* 매입단가         */
    char    value       [12];       /* 매입금액         */
    char    evaval      [12];       /* 평가금액         */
    char    benefit     [12];       /* 손익             */
    char    bnftrt      [ 9];       /* 수익률           */
    char    bidho       [ 9];       /* 매수호가         */
    char    offerho     [ 9];       /* 매도호가         */
    char    hprice      [ 9];       /* 상한가           */
    char    lprice      [ 9];       /* 하한가           */
    char    loadDay     [ 8];       /* 대출일           */
} D71020_OG;

typedef struct _D71020_O
{
    char        nxtGb       [ 1];   /* 0:연속없음. 1:연속있음   */
                                    /* Key는 맨 마지막 종목코드 */
    char        winid       [ 4];   /* 받은 data를 그대로 반송  */

 	char        grdCnt      [ 6];
    char        grdBjMsg    [20];

    D71020_OG   grdData     [1];
} D71020_O;

const UINT RMSG_INTREQUEST = ::RegisterWindowMessage("RMSG_INTREQUEST");
const UINT RMSG_CONCERNHWND	= ::RegisterWindowMessage(_T("RMSG_CONCERNHWND"));

const UINT RMSG_GETRECENTCODE = ::RegisterWindowMessage("RMSG_GETRECENTCODE");
const UINT RMSG_NOTIFYRECENTCODE = ::RegisterWindowMessage("RMSG_NOTIFYRECENTCODE");
const UINT RMSG_CHANGESKIN = ::RegisterWindowMessage(_T("RMSG_CHANGESKIN"));
#endif // __DOORI__INTERESTDEF__HEADER__