#ifndef __C2440STRUCT_H
#define __C2440STRUCT_H

#pragma pack(push)
#pragma pack(1)

//{{JS.Kim_20100906 데이타추가부르기
#ifndef ChartKeySize
#define		ChartKeySize		28
#endif
//}}

//TR o44003$

typedef struct _HCQ03010_IN
{
	char		keyvalue[84];		/* key 일자+시간	: key 일자+시간 */
	char		jmcode[10];			/* 종목코드			: 종목코드 */
	char		date[8];			/* 일자				: 일자 */
	char		chetime[10];		/* 시간				: 시간 */
	char		gubun[1];			/* 구분				: 0:처음1:다음 */
	char		dgubun[1];			/* 데이터구분		: 1:Tick,2:분,3:일,4:주,5:월 */
	char		nsel[4]/*[2]*/;		/* N분구분			: 0:30sec,1:1M,2:2M,... */
	char		dcnt[5];			/* 데이터건수		: 데이터건수 */
//	char		qtycd[1];			/* 0: 허수포함, 1:허수제거(실거래만쿼리) */
    char		nflag[1];           /* 조회모드(1)                 */	////JS.Kim_20100906 데이타추가부르기
	char		nkey[ChartKeySize];	/* 검색키(ChartKeySize)        */	////JS.Kim_20100906 데이타추가부르기
} HCQ03010_IN;

#define HCQ03010_IN_SZ sizeof(HCQ03010_IN)

typedef struct _o44003_OUT_HEADER
{
	char		keyvalue[18];		/* key 일자+시간	: key 일자+시간 */
	char		count[4];
} o44003_OUT_HEADER;
#define o44003_OUT_SZ sizeof(o44003_OUT_HEADER)

typedef struct _HCQ03010_OUT
{
	char		date[8];			/* 일자 					: 일자 */
	char		chetime[10];		/* 시간 					: 시간 */
	char		kdate[8];        //추가
	char		kchetime[10];    //추가
	char		open_d[10];	//15		/* 진법변환전 시가			: 진법변환전 시가 */
	char		high_d[10];	//15		/* 진법변환전 고가			: 진법변환전 고가 */
	char		low_d[10];	//15		/* 진법변환전 저가			: 진법변환전 저가 */
	char		price_d[10];//15		/* 진법변환전 종가			: 진법변환전 종가 */
	char		sign[1];			/* 전일대비구분				: 전일대비구분 */
	char		change_d[10]; //10		/* 진법변환전 전일대비		: 진법변환전 전일대비 */
	char		drate[10];			/* 전일대비등락율			: 전일대비등락율 */
	char		presign[1];			/* 직전대비구분				: 직전대비구분("+"상승"-"하락" "보합) */
	char		cvolume[10];		/* 체결량					: 체결량 */
	char		cgubun[1];			/* 체결구분("+"매수"-"매도) : 체결구분("+"매수"-"매도) */
	char		volume[10];			/* 누적거래량				: 누적거래량 */

} HCQ03010_OUT;

#define HCQ03010_OUT_SZ sizeof(HCQ03010_OUT)

//KTB는 OUT_KEY Packet 따로 요청 해야함.
typedef struct _HCQo44000_IN
{
	char	jmcode[10];
}HCQo44000_IN;  //_HCQ03010_OUT_KEY 의 인풋 TR명과 구조체명 다름. 
#define HCQo44000_IN_SZ sizeof(HCQo44000_IN)

typedef struct _HCQ03010_OUT_KEY
{
	char		jmcode[10];		/* 종목코드 */
	char		jmname[50];		/* 종목명 */
	char		price[10];		/* 현재가 */	
	char		sign[1];		/* 전일대비구분 */
	char		change[10];		/* 전일대비 */
	char		drate[10];		/* 전일대비등락율 */
	char		presign[1];		/* 직전대비구분 */
	char		open[10];		/* 시가 */
	char		high[10];		/* 고가 */
	char		low[10];		/* 저가 */
	char		lthigh[10];		/* LT고가 */
	char		lthighdate[8];		/* LT고가일 */
	char		ltlow[10];		/* LT저가 */
	char		ltlowdate[8];		/* LT저가일 */
	char		bday[8];		/* 영업일 */						//	GMT 시간 기준
	char		ssvolume[10];		/* 상승거래량 */
	char		hrvolume[10];		/* 하락거래량 */
	char		volume[10];		/* 누적거래량 */
	char		lastday[8];		/* 체결시간 */
	char		jsprice[10];		/* 정산가 */					//	기준가로 사용함
	char		jsday[8];		/* 정산일 */
	char		jandaycnt[5];		/* 잔존일수 */
	char		mdhoga[10];		/* 매도호가 */
	char		mshoga[10];		/* 매수호가 */
	char		baseprice[10];  //기준가
	char		volrate[10];	//전일거래량대비율

//	char		timeindex[2];		/* TIMEZONE 인덱스 */
	char		date[8];			/* 일자 */
	char		time[10];			/* 시간 */
	char		exchkind[1];		/* 거래소구분 1:CME, 2:CBOT, 3:SGX, 4:LIFFE, 5:EUREX, 6:HKFE, 7:NYBOT */
	char		estime[4];		/* 전산장(ETH) 시작(HHMM) */		//	GMT 시간 기준
	char		eetime[4];		/* 전산장(ETH) 종료(HHMM) */		//	GMT 시간 기준
	char		chetime_g[10];  //추가
}HCQ03010_OUT_KEY;

#define HCQ03010_OUT_KEY_SZ sizeof(HCQ03010_OUT_KEY)



//20091117 SJ_KIM
/******************************************************************************/
/*  HCQ03010  종목 ( 틱, 분, 일, 주, 월 )  TR I/O                             */
/******************************************************************************/
// typedef struct _HCQ03010_IN
// {
// 	char		keyvalue[18];		/* key 일자+시간	: key 일자+시간 */
// 	char		jmcode[10];			/* 종목코드			: 종목코드 */
// 	char		date[8];			/* 일자				: 일자 */
// 	char		chetime[10];		/* 시간				: 시간 */
// 	char		gubun[1];			/* 구분				: 0:처음1:다음 */
// 	char		dgubun[1];			/* 데이터구분		: 1:Tick,2:분,3:일,4:주,5:월 */
// 	char		nsel[3];			/* N분구분			: 0:30sec,1:1M,2:2M,... */
// 	char		dcnt[5];			/* 데이터건수		: 데이터건수 */
// // --> [Edit]  강지원 2008/10/20	( HCQ3005변경 )
// 	char		qtycd[1];			/* 0: 허수포함, 1:허수제거(실거래만쿼리) */
// // <-- [Edit]  강지원 2008/10/20
// 
// } HCQ03010_IN;
// 
// #define HCQ03010_IN_SZ sizeof(HCQ03010_IN)
// 
// typedef struct _HCQ03010_OUT
// {
// 	char		date[8];			/* 일자 					: 일자 */
// 	char		chetime[10];		/* 시간 					: 시간 */
// 	char		open_d[15];			/* 진법변환전 시가			: 진법변환전 시가 */
// 	char		high_d[15];			/* 진법변환전 고가			: 진법변환전 고가 */
// 	char		low_d[15];			/* 진법변환전 저가			: 진법변환전 저가 */
// 	char		price_d[15];		/* 진법변환전 종가			: 진법변환전 종가 */
// 	char		sign[1];			/* 전일대비구분				: 전일대비구분 */
// 	char		change_d[15];		/* 진법변환전 전일대비		: 진법변환전 전일대비 */
// 	char		drate[10];			/* 전일대비등락율			: 전일대비등락율 */
// 	char		presign[1];			/* 직전대비구분				: 직전대비구분("+"상승"-"하락" "보합) */
// 	char		cvolume[10];		/* 체결량					: 체결량 */
// 	char		cgubun[1];			/* 체결구분("+"매수"-"매도) : 체결구분("+"매수"-"매도) */
// 	char		volume[10];			/* 누적거래량				: 누적거래량 */
// 
// } HCQ03010_OUT;
// 
// #define HCQ03010_OUT_SZ sizeof(HCQ03010_OUT)
// 
// //KTB는 OUT_KEY Packet 따로 요청 해야함.
// typedef struct _HCQo44000_IN
// {
// 	char	jmcode[10];
// }HCQo44000_IN;  //_HCQ03010_OUT_KEY 의 인풋 TR명과 구조체명 다름. 
// 
// typedef struct _HCQ03010_OUT_KEY
// {
// 	char		keyvalue[18];		/* key 일자+시간	: key 일자+시간 */
// 
// // --> [Edit]  강지원 2008/10/20	( HCQ3005변경 )
// 	char		timeindex[2];		/* TIMEZONE 인덱스 */
// 	char		date[8];			/* 일자 */
// 	char		time[6];			/* 시간 */
// 	char		summertime[1];		/* SUMMER TIME여부 1:섬머타임, 0:노멀 */
// 	char		exchkind[1];		/* 거래소구분 1:CME, 2:CBOT, 3:SGX, 4:LIFFE, 5:EUREX, 6:HKFE, 7:NYBOT */
// 
// 	char		jmcode[10];		/* 종목코드 */
// 	char		jmname[50];		/* 종목명 */
// 	char		estime[6];		/* 전산장(ETH) 시작(HHMM) */		//	GMT 시간 기준
// 	char		eetime[6];		/* 전산장(ETH) 종료(HHMM) */		//	GMT 시간 기준
// 	char		price[10];		/* 현재가 */
// 	char		sign[1];		/* 전일대비구분 */
// 	char		change[10];		/* 전일대비 */
// 	char		drate[10];		/* 전일대비등락율 */
// 	char		presign[1];		/* 직전대비구분 */
// 	char		open[10];		/* 시가 */
// 	char		high[10];		/* 고가 */
// 	char		low[10];		/* 저가 */
// 	char		lthigh[10];		/* LT고가 */
// 	char		lthighdate[8];		/* LT고가일 */
// 	char		ltlow[10];		/* LT저가 */
// 	char		ltlowdate[8];		/* LT저가일 */
// 	char		bday[8];		/* 영업일 */						//	GMT 시간 기준
// 	char		ssvolume[10];		/* 상승거래량 */
// 	char		hrvolume[10];		/* 하락거래량 */
// 	char		volume[10];		/* 누적거래량 */
// 	char		lastday[8];		/* 체결시간 */
// 	char		jsprice[10];		/* 정산가 */					//	기준가로 사용함
// 	char		jsday[8];		/* 정산일 */
// 	char		jandaycnt[5];		/* 잔존일수 */
// 	char		mdhoga[10];		/* 매도호가 */
// 	char		mshoga[10];		/* 매수호가 */
// // <-- [Edit]  강지원 2008/10/20
// 
// 	char		display_cnt[ 5];	/* 조회 리스트 COUNT    */
// 
// }HCQ03010_OUT_KEY;
// 
// #define HCQ03010_OUT_KEY_SZ sizeof(HCQ03010_OUT_KEY)




/******************************************************************************/
/*  HCQ03011	호가챠트   TR I/O			                          */
/******************************************************************************/
//TR o56501$
typedef struct _HCQ03011_IN
{
	char		keyvalue[18];		/* key 일자+시간	: key 일자+시간 */
	char		jmcode[10];			/* 종목코드			: 종목코드 */
	char		date[8];			/* 일자				: 일자 */
	char		chetime[10];		/* 시간				: 시간 */
	char		dcnt[5];			/* 데이터건수		: 데이터건수 */

} HCQ03011_IN;
#define HCQ03011_IN_SZ sizeof(HCQ03011_IN)

typedef struct _o56501_OUT_HEADER
{
	char		keyvalue[18];		/* key 일자+시간	: key 일자+시간 */
	char		count[4];
} o56501_OUT_HEADER;
#define o56501_OUT_SZ sizeof(o56501_OUT_HEADER)

typedef struct _HCQ03011_OUT
{
	char		date[8];			/* 일자					: 일자 */
	char		chhotime[10];		/* 시간					: 시간 */
	char		kdate[8];           //추가
	char		kchhotime[10];		//추
	char		mdhoga_d[15];		/* 진법변환전 매도호가	: 진법변환전 매도호가 */
	char		mdhorem[10];		/* 매도잔량				: 매도잔량 */
	char		mshoga_d[15];		/* 진법변환전 매수호가	: 진법변환전 매수호가 */
	char		mshorem[10];		/* 매수잔량				: 매수잔량 */

} HCQ03011_OUT;
#define HCQ03011_OUT_SZ sizeof(HCQ03011_OUT)

typedef struct _HCQ03011_OUT_KEY
{
	char		jmcode[10];		/* 종목코드 */
	char		jmname[50];		/* 종목명 */
	char		price[10];		/* 현재가 */	
	char		sign[1];		/* 전일대비구분 */
	char		change[10];		/* 전일대비 */
	char		drate[10];		/* 전일가대비등락율 */
	char		presign[1];		/* 직전대비구분 */
	char		open[10];		/* 시가 */
	char		high[10];		/* 고가 */
	char		low[10];		/* 저가 */
	char		lthigh[10];		/* LT고가 */
	char		lthighdate[8];		/* LT고가일 */
	char		ltlow[10];		/* LT저가 */
	char		ltlowdate[8];		/* LT저가일 */
	char		bday[8];		/* 영업일 */						//	GMT 시간 기준
	char		ssvolume[10];		/* 상승거래량 */
	char		hrvolume[10];		/* 하락거래량 */
	char		volume[10];		/* 누적거래량 */
	char		lastday[8];		/* 체결시간 */
	char		jsprice[10];		/* 정산가 */					//	기준가로 사용함
	char		jsday[8];		/* 정산일 */
	char		jandaycnt[5];		/* 잔존일수 */
	char		mdhoga[10];		/* 매도호가 */
	char		mshoga[10];		/* 매수호가 */
	char		baseprice[10];  //기준가
	char		volrate[10];	//전일거래량대비율

//	char		timeindex[2];		/* TIMEZONE 인덱스 */
	char		date[8];			/* 일자 */
	char		time[10];			/* 시간 */
	char		exchkind[1];		/* 거래소구분 1:CME, 2:CBOT, 3:SGX, 4:LIFFE, 5:EUREX, 6:HKFE, 7:NYBOT */
	char		estime[4];		/* 전산장(ETH) 시작(HHMM) */		//	GMT 시간 기준
	char		eetime[4];		/* 전산장(ETH) 종료(HHMM) */		//	GMT 시간 기준
	char		chetime_g[10];  //추가
}HCQ03011_OUT_KEY;
#define HCQ03011_OUT_KEY_SZ sizeof(HCQ03011_OUT_KEY)
// typedef struct _HCQ03011_IN
// {
// 	char		keyvalue[18];		/* key 일자+시간	: key 일자+시간 */
// 	char		jmcode[10];			/* 종목코드			: 종목코드 */
// 	char		date[8];			/* 일자				: 일자 */
// 	char		chetime[10];		/* 시간				: 시간 */
// 	char		dcnt[5];			/* 데이터건수		: 데이터건수 */
// 
// } HCQ03011_IN;
// 
// #define HCQ03011_IN_SZ sizeof(HCQ03011_IN)
// 
// typedef struct _HCQ03011_OUT
// {
// 	char		date[8];			/* 일자					: 일자 */
// 	char		chhotime[10];		/* 시간					: 시간 */
// //	char		mdhoga[10];			/* 매도호가				: 매도호가 */
// 	char		mdhoga_d[15];		/* 진법변환전 매도호가	: 진법변환전 매도호가 */
// 	char		mdhorem[10];		/* 매도잔량				: 매도잔량 */
// //	char		mshoga[10];			/* 매수호가				: 매수호가 */
// 	char		mshoga_d[15];		/* 진법변환전 매수호가	: 진법변환전 매수호가 */
// 	char		mshorem[10];		/* 매수잔량				: 매수잔량 */
// 
// } HCQ03011_OUT;
// 
// #define HCQ03011_OUT_SZ sizeof(HCQ03011_OUT)
// 
// 
// typedef struct _HCQ03011_OUT_KEY
// {
// 	char		keyvalue[18];			/* key 일자+시간	: key 일자+시간 */
// 
// // --> [Edit]  강지원 2008/10/28
// 	char		timeindex[2];		/* TIMEZONE 인덱스 */
// 	char		date[8];		/* 일자 */
// 	char		time[6];		/* 시간 */
// 	char		summertime[1];		/* SUMMER TIME여부 1:섬머타임, 0:노멀 */
// 	char		exchkind[1];		/* 거래소구분 1:CME, 2:CBOT, 3:SGX, 4:LIFFE, 5:EUREX, 6:HKFE, 7:NYBOT */
// 
// 	char		jmcode[10];		/* 종목코드 */
// 	char		jmname[50];		/* 종목명 */
// 	char		estime[6];		/* 전산장(ETH) 시작(HHMM) */
// 	char		eetime[6];		/* 전산장(ETH) 종료(HHMM) */
// 	char		price[10];		/* 현재가 */
// 	char		sign[1];		/* 전일대비구분 */
// 	char		change[10];		/* 전일대비 */
// 	char		drate[10];		/* 전일대비등락율 */
// 	char		presign[1];		/* 직전대비구분 */
// 	char		open[10];		/* 시가 */
// 	char		high[10];		/* 고가 */
// 	char		low[10];		/* 저가 */
// 	char		lthigh[10];		/* LT고가 */
// 	char		lthighdate[8];		/* LT고가일 */
// 	char		ltlow[10];		/* LT저가 */
// 	char		ltlowdate[8];		/* LT저가일 */
// 	char		bday[8];		/* 영업일 */
// 	char		ssvolume[10];		/* 상승거래량 */
// 	char		hrvolume[10];		/* 하락거래량 */
// 	char		volume[10];		/* 누적거래량 */
// 	char		lastday[8];		/* 체결시간 */
// 	char		jsprice[10];		/* 정산가 */
// 	char		jsday[8];		/* 정산일 */
// 	char		jandaycnt[5];		/* 잔존일수 */
// 	char		mdhoga[10];		/* 매도호가 */
// 	char		mshoga[10];		/* 매수호가 */
// // <-- [Edit]  강지원 2008/10/28
// 
//     char		display_cnt[ 5];		/* 조회 리스트 COUNT    */
// }HCQ03011_OUT_KEY;
// 
// #define HCQ03011_OUT_KEY_SZ sizeof(HCQ03011_OUT_KEY)


/******************************************************************************/
/*  HCQ01120  현재가시세  TR I/O										      */
/******************************************************************************/
typedef struct _HCQ01120_IN
{
	char		jmcode[10];			/* 종목코드 : | */

} HCQ01120_IN;

#define HCQ01120_IN_SZ sizeof(HCQ01120_IN)

typedef struct _HCQ01120_OUT
{
	char		jmcode[10];			/* 종목코드 : 종목코드 */
	char		jmname[50];			/* 종목명 : | */
	char		price[10];			/* 현재가 : 현재가 */
	char		sign[1];			/* 전일대비구분 : 전일대비구분 */
	char		change[10];			/* 전일대비 : 전일대비 */
	char		drate[10];			/* 전일대비등락율 : 전일대비등락율 */
	char		presign[1];			/* 직전대비구분 : 직전대비구분 */
	char		open[10];			/* 시가 : 시가 */
	char		high[10];			/* 고가 : 고가 */
	char		low[10];			/* 저가 : 저가 */
	char		lthigh[10];			/* LT고가 : LT고가 */
	char		lthighdate[8];		/* LT고가일 : LT고가일 */
	char		ltlow[10];			/* LT저가 : LT저가 */
	char		ltlowdate[8];		/* LT저가일 : LT저가일 */
	char		bday[8];			/* 영업일 : 영업일 */
	char		ssvolume[10];		/* 상승거래량 : 상승거래량 */
	char		hrvolume[10];		/* 하락거래량 : 하락거래량 */
	char		volume[10];			/* 누적거래량 : 누적거래량 */
	char		chetime[8];			/* 체결시간 : 체결시간 */
	char		jsprice[10];		/* 정산가 : 정산가 */
	char		jsday[8];			/* 정산일 : 정산일 */
	char		jandaycnt[5];		/* 잔존일수 : 잔존일수 */
	char		mdhoga[10];			/* 매도호가 : 매도호가 */
	char		mshoga[10];			/* 매수호가 : 매수호가 */
	
} HCQ01120_OUT;	
	
#define HCQ01120_OUT_SZ sizeof(HCQ01120_OUT)


/******************************************************************************/
/*  HCQ01112  호가정보  TR I/O										      */
/******************************************************************************/
typedef struct _HCQ01112_IN
{
	char		jmcode[10];			/* 종목코드 : | */

} HCQ01112_IN;

#define HCQ01112_IN_SZ sizeof(HCQ01112_IN)

typedef struct _HCQ01112_OUT
{
	char		jmcode[10];			/* 종목코드 : 종목코드 */
	char		hotime[8];			/* 호가시간 : 호가시간 */
	char		mdhoga1[10];		/* 매도호가1 : 매도호가1 */
	char		mshoga1[10];		/* 매수호가1 : 매수호가1 */
	char		mdhorem1[10];		/* 매도호가잔량1 : 매도호가잔량1 */
	char		mshorem1[10];		/* 매수호가잔량1 : 매수호가잔량1 */
	char		mdhocnt1[10];		/* 매도호가건수1 : 매도호가건수1 */
	char		mshocnt1[10];		/* 매수호가건수1 : 매수호가건수1 */
	char		mdhoga2[10];		/* 매도호가2 : 매도호가2 */
	char		mshoga2[10];		/* 매수호가2 : 매수호가2 */
	char		mdhorem2[10];		/* 매도호가잔량2 : 매도호가잔량2 */
	char		mshorem2[10];		/* 매수호가잔량2 : 매수호가잔량2 */
	char		mdhocnt2[10];		/* 매도호가건수2 : 매도호가건수2 */
	char		mshocnt2[10];		/* 매수호가건수2 : 매수호가건수2 */
	char		mdhoga3[10];		/* 매도호가3 : 매도호가3 */
	char		mshoga3[10];		/* 매수호가3 : 매수호가3 */
	char		mdhorem3[10];		/* 매도호가잔량3 : 매도호가잔량3 */
	char		mshorem3[10];		/* 매수호가잔량3 : 매수호가잔량3 */
	char		mdhocnt3[10];		/* 매도호가건수3 : 매도호가건수3 */
	char		mshocnt3[10];		/* 매수호가건수3 : 매수호가건수3 */
	char		mdhoga4[10];		/* 매도호가4 : 매도호가4 */
	char		mshoga4[10];		/* 매수호가4 : 매수호가4 */
	char		mdhorem4[10];		/* 매도호가잔량4 : 매도호가잔량4 */
	char		mshorem4[10];		/* 매수호가잔량4 : 매수호가잔량4 */
	char		mdhocnt4[10];		/* 매도호가건수4 : 매도호가건수4 */
	char		mshocnt4[10];		/* 매수호가건수4 : 매수호가건수4 */
	char		mdhoga5[10];		/* 매도호가5 : 매도호가5 */
	char		mshoga5[10];		/* 매수호가5 : 매수호가5 */
	char		mdhorem5[10];		/* 매도호가잔량5 : 매도호가잔량5 */
	char		mshorem5[10];		/* 매수호가잔량5 : 매수호가잔량5 */
	char		mdhocnt5[10];		/* 매도호가건수5 : 매도호가건수5 */
	char		mshocnt5[10];		/* 매수호가건수5 : 매수호가건수5 */
	char		totmdhorem[10];		/* 총매도호가잔량 : 총매도호가잔량 */
	char		totmshorem[10];		/* 총매수호가잔량 : 총매수호가잔량 */
	char		totmdhocnt[10];		/* 총매도호가건수 : 총매도호가건수 */
	char		totmshocnt[10];		/* 총매수호가건수 : 총매수호가건수 */
	char		open[10];			/* 시가 : 시가 */
	char		high[10];			/* 고가 : 고가 */
	char		low[10];			/* 저가 : 저가 */
	char		price[10];			/* 현재가 : 현재가 */
	char		sign[1];			/* 전일대비구분 : 전일대비구분 */
	char		change[10];			/* 전일대비 : 전일대비 */
	char		presign[1];			/* 직전대비구분 : 직전대비구분 */
	char		jsprice[10];		/* 정산가 : 정산가 */
	char		volume[10];			/* 누적거래량 : 누적거래량 */
	char		drate[10];			/* 전일대비등락율 : 전일대비등락율 */

	char		ho_drate1[10];		/*  [매도호가등락율1]매도호가등락율1     */
	char		hb_drate1[10];		/*  [매수호가등락율1]매도호가등락율2     */
	char		ho_drate2[10];		/*  [매도호가등락율2]매도호가등락율3     */
	char		hb_drate2[10];		/*  [매수호가등락율2]매도호가등락율4     */
	char		ho_drate3[10];		/*  [매도호가등락율3]매도호가등락율5     */
	char		hb_drate3[10];		/*  [매수호가등락율3]매수호가등락율1     */
	char		ho_drate4[10];		/*  [매도호가등락율4]매수호가등락율2     */
	char		hb_drate4[10];		/*  [매수호가등락율4]매수호가등락율3     */
	char		ho_drate5[10];		/*  [매도호가등락율5]매수호가등락율4     */
	char		hb_drate5[10];		/*  [매수호가등락율5]매수호가등락율5     */
	char		tot_bidrem[10];		/*  [총순매수잔량]총순매수잔량     */
	char		totofferremcha[10];		/*  [직전매도대비총수량]직전매도대비총수량     */
	char		totbidremcha[10];		/*  [직전매수대비총수량]직전매수대비총수량     */
	char		totoffercntcha[10];		/*  [직전매도대비총건수]직전매도대비총건수     */
	char		totbidcntcha[10];		/*  [직전매수대비총건수]직전매수대비총건수     */
	
} HCQ01112_OUT;	
	
#define HCQ01112_OUT_SZ sizeof(HCQ01112_OUT)


// --> [Edit]  김현수 2009/08/23
//======================================================================
// 스프레드 차트 (HCQ03050)
//======================================================================

typedef struct _HCQ03050_IN
{
	char		keyvalue[18];		/* 일자 */
	char		jmcode1[10];		/* 종목코드 1 */
	char		jmcode2[10];		/* 종목코드 2 */
	char		date[8];		/* 일자 */
	char		chetime[10];		/* 시간 */
	char		gubun[1];		/* 0:처음,1:다음 */
	char		dgubun[1];		/* 1:Tick,2:분,3:일,4:주,5:월 */
	char		nsel[3];		/* 0:30sec,1:1M,2:2M,... */
	char		dcnt[5];		/* 데이터건수 */
}HCQ03050_IN;
#define HCQ03050_IN_SZ sizeof(HCQ03050_IN)


typedef struct _HCQ03050_OUT1_KEY
{
	char		keyvalue[18];		/* key 일자+시간 */
	char		display_cnt[ 5];	/* 조회 리스트 COUNT    */

}HCQ03050_OUT1_KEY;
#define HCQ03050_OUT1_KEY_SZ sizeof(HCQ03050_OUT1_KEY)


typedef struct _HCQ03050_OUT1
{
	char		date1[8];		/* 일자 */
	char		chetime1[10];		/* 시간 */
	char		open_d1[15];		/* 진법변환전 시가 */
	char		high_d1[15];		/* 진법변환전 고가 */
	char		low_d1[15];		/* 진법변환전 저가 */
	char		price_d1[15];		/* 진법변환전 종가 */
	char		sign1[1];		/* 전일대비구분 */
	char		change_d1[15];		/* 진법변환전 전일대비 */
	char		drate1[10];		/* 전일대비등락율 */
	char		presign1[1];		/* 직전대비구분("+"상승"-"하락" "보합) */
	char		cvolume1[10];		/* 체결량 */
	char		cgubun1[1];		/* 체결구분("+"매수"-"매도) */
	char		volume1[10];		/* 누적거래량 */
}HCQ03050_OUT1;
#define HCQ03050_OUT1_SZ sizeof(HCQ03050_OUT1)


typedef struct _HCQ03050_OUT2_KEY
{
	char		display_cnt[ 5];	/* 조회 리스트 COUNT    */

}HCQ03050_OUT2_KEY;
#define HCQ03050_OUT2_KEY_SZ sizeof(HCQ03050_OUT2_KEY)


typedef struct _HCQ03050_OUT2
{
	char		date2[8];		/* 일자 */
	char		chetime2[10];		/* 시간 */
	char		open_d2[15];		/* 진법변환전 시가 */
	char		high_d2[15];		/* 진법변환전 고가 */
	char		low_d2[15];		/* 진법변환전 저가 */
	char		price_d2[15];		/* 진법변환전 종가 */
	char		sign2[1];		/* 전일대비구분 */
	char		change_d2[15];		/* 진법변환전 전일대비 */
	char		drate2[10];		/* 전일대비등락율 */
	char		presign2[1];		/* 직전대비구분("+"상승"-"하락" "보합) */
	char		cvolume2[10];		/* 체결량 */
	char		cgubun2[1];		/* 체결구분("+"매수"-"매도) */
	char		volume2[10];		/* 누적거래량 */
}HCQ03050_OUT2;
#define HCQ03050_OUT2_SZ sizeof(HCQ03050_OUT2)

typedef struct _ST_CHART_DATA
{
	CString		szDate;			/* 일자 */
	CString		szTime;			/* 시간 */
	CString		szOpen1;		/* 진법변환전 시가 */
	CString		szHigh1;		/* 진법변환전 고가 */
	CString		szLow1;			/* 진법변환전 저가 */
	CString		szClose1;		/* 진법변환전 종가 */

	CString		szOpen2;		/* 진법변환전 시가 */
	CString		szHigh2;		/* 진법변환전 고가 */
	CString		szLow2;			/* 진법변환전 저가 */
	CString		szClose2;		/* 진법변환전 종가 */

}ST_CHART_DATA;
//======================================================================
// <-- [Edit]  김현수 2009/08/23

#pragma pack(pop)

#endif