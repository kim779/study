// *****************************************************************************
// (C) COPYRIGHT Winix Information Inc. 2000
// All Rights Reserved
// Licensed Materials - Property of WINIX
//
// This program contains proprietary information of Winix Information.
// All embodying confidential information, ideas and expressions can't be
// reproceduced, or transmitted in any form or by any means, electronic, 
// mechanical, or otherwise without the written permission of Winix Information.
//
//  Components   : jmcode.h
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2000-07	Initial version
// *****************************************************************************

#ifndef _JMCODE
#define _JMCODE

//
//	현물 종목코드
//

#define FENameLen	20
#define	ENameLen	40
#define HNameLen	40
#define	HCodeLen	12
#define	HSymbolLen	7

struct	hjcode {
	char	code[HCodeLen];		// 코드	(12)
					// 코드(7) : A######
					// 코드(9) : 신주인수권, 증서
	char	hnam[HNameLen];		// 한글명(32)
	char	enam[ENameLen];		/* 영문명			*/
	char	ecng;			// ECN 구분
					// 거래가능:1
	char	jsiz;			// 업종자본금 대중소
					// 장내 (대:38  중:39  소:40)
					// 장외 (1:KOSDAQ 100)
					// 장외 (2:KOSDAQ MID 300)
					// 장외 (3:KOSDAQ SMALL)
					// 배당지수(jsiz += 41)
					// 신주인수권종목 FULL코드용
	char	symb[HSymbolLen];	// SYMBOL
	char	size;			// 자본금 규모
					// KOSPI  (대:2  중:3  소:4)
					// KOSDAQ (기타서비스:3  코스닥IT종합:4)
	char	ucdm;			// 업종중분류
					// KOSDAQ:5~14  미분류:99
	char	ucds;			// 업종소분류
					// KOSDAQ:15~37  벤쳐:7  미분류:99
	char	jjug;			// 제조업 구분
					// KOSPI  (제조업:27)
					// KOSDAQ (벤처지수:2)
	char	kpgb;			// KOSPI, KOSDAQ 구분
					// KOSPI  (0:미포함 1:K200 2:K100 3:K50)
					// KOSDAQ (0:미포함 1:KQ50)
	char	kosd;			// 종목구분
					// 0:장내  10:장외  30:제3시장
	char    ssgb;	// 소속구분
					// 0:일반
                    // 3:외국인예탁증서
                    // 4:뮤추얼펀드
                    // 5:신주인수권(중권)
                    // 6:리츠
                    // 7:신주인수권(증서)
                    // 8:ETF
                    // 9:하이일드펀드(수익증권)
                    // 10: 선박 12:인프라
                    // 13: 해외ETF 14:해외원주
/*	char	ssgb;			// 소속구분
					// 0:default
					// 3:외국주
					// 4:뮤추얼펀드
					// 5:신주인수권
					// 6:리츠
					// 7:신주인수권증서
					// 8:ETF
					// 9:하이일드펀드
*/
	char	ucmd;			// 우선주구분 (0:보통주  5:우선주)
	char	itgb;			// KOSPI-IT/KQ-IT50구분(1:채택)
	char	wsgb;			// KOSPI 지배구조지수:42 (0:보통주 1:우수)
	char	jqty;			// 기본주문단위
	char	star;			// KOSDAQ 스타지수
	char	unio;			// 통합지수
	char    jchk;			// 종목검색용 구분
					// 0x01     불성실공시종목
					// 0x02     신규상장종목
					// 0x04     권리락종목
					// 0x08     우선주종목
					// 0x10     관리종목
					// 0x20     감리종목
					// 0x40     거래정지종목
					// 0x80     투자유의종목
	char    elwf;			// 0: 일반, 1: 기초자산 20070716 추가
	char	sjjs[10];		// 상장주식수(단위 주)
	char	fill[10];		// reserved
};	
	
//
//	hjcode.kosd : 종목구분
//
#define	jmKOSPI		0		// KOSPI 종목
#define jmKOSDAQ	10		// 코스닥
#define	jm3RD		30		// 제3시장

//
//	hjcode.ssgb : 소속구분
//
#define	jmDEF		0		// default
#define jmMUFND		4		// 뮤추얼펀드
#define jmSINJU		5		// 신주인수권
#define jmREITS		6		// 리츠
#define jmSINJS		7		// 신주인수권증서
#define jmETF		8		// ETF
#define	jmHYFND		9		// 하이일드펀드
#define	jmELW		11		// ELW
#define	jmFOREIGN	3		// 외국주 20070327
#define jmSHIP		10		// 선박
#define jmINFRA		12		// 인프라

//
//	hjcode.ucmd : 보통주, 우선주
//
#define stockCom	0		// 보통주
#define stockPre	5		// 우선주



//
//	선물종목코드
//

#define FNameLen	20
#define FCodeLen	8

struct	fjcode {
	char	cod2[FCodeLen];		// 단축코드
	char	hnam[FNameLen];		// 한글명
	char	enam[FENameLen];	// 영문명
	char	mchk;			// 매매유형허용
					// 0x01:시장가허용
					// 0x02:조건부지정가허용
					// 0x04:최유리지정가허용
};

struct  sfcode {            /* 주식선물 종목코드 정보       */
	char    codx[8];        /* 단축코드(8)                  */
	char    hnam[32];       /* 한글명                       */
	char    enam[32];       /* 영문명                       */
	char    mchk;           /* 매매유형허용                 */
	/* 0x01:시장가허용              */
	/* 0x02:조건부지정가허용        */
	/* 0x04:최유리지정가허용        */
	char    tjgb[2];        /* 거래대상 종목구분            */
	char    gcod[12];       /* 기초자산종목코드             */
	char    gnam[32];       /* 기초자산 종목명              */
	char    fill[48];       /* reserved                     */
};

//
//	옵션코드 
//

#define OPriceLen	5
#define ONameLen	20
#define OCodeLen	9

//
//	옵션종목코드 HEADER
//
struct	ojcodh {
	char	cjym[4][6];		// CALL 옵션 종목 타이틀
	char	pjym[4][6];		// PUT  옵션 종목 타이틀
};

//
//	옵션종목코드
//
struct	ojcode {
	char	price[OPriceLen];	// 행사가격 (999.99)
	char	atmg;			// ATM 구분
	struct	{
		char	yorn;		// 종목	코드 유무
		char	cod2[OCodeLen];	// 단축종목코드
		char	hnam[ONameLen];	// 한글종목명
		char	enam[FENameLen];	// 영문명
		char	mchk;		// 매매유형허용
					// 0x01:시장가허용
					// 0x02:조건부지정가허용
					// 0x04:최유리지정가허용
	} call[4]; 
	struct	{
		char	yorn;		// 종목	코드 유무
		char	cod2[OCodeLen];	// 단축종목코드
		char	hnam[ONameLen];	// 한글종목명
		char	enam[FENameLen];	// 영문명
		char	mchk;		// 매매유형허용
					// 0x01:시장가허용
					// 0x02:조건부지정가허용
					// 0x04:최유리지정가허용
	} put[4]; 
};


//
//	채권코드
//

#define	CCodeLen	12
#define	CNameLen	40

struct	cjcode	{
	char	code[CCodeLen];
	char	cgub;			// 채권구분
					// 1:국민주택1종 당월분
					// 2:국민주택1종 전월분
					// 3:서울도시철도 당월분
					// 4:서울도시철도 전월분
					// 5:지역개발채권 당월분
					// 6:지역개발채권 전월분
					// 7:지방도시철도 당월분
					// 8:지방도시철도 전월분
					// 99:전환사채
	char	hnam[CNameLen];		// 채권이름
	char    ngub;			// 구분 0: 전체, 1:최근종목
};

//
//	cjcode : 채권구분
//
#define	cjSo	1			// 소액채권
#define	cjCh	2			// 전환사채


//
//	업종코드
//

#define	UNameLen	20

struct	upcode	{
	char	jgub;			// 장구분
					// 0:장내 1:장외
					// 1: KOSPI200
					// 3: KOSDAQ50(1), KQ-IT 50(2)
					// 4: K100P(1), K50(2), IT(3
					// 6: SECTOR, 7:프리보드 
	char	ucod;			// 업종코드
	char	hnam[UNameLen];		// 한글명
};

//
//	upcode.jgup : 장구분 : 신한변경
//
#define	upKOSPI		0		// 1~41
#define upKOSPI200	1		// 1~6
#define	upKOSDAQ	2		// 1~40
#define upKOSDAQ50	3		// KOSDAQ50   : ucod = 1
					// KOSDAQIT50 : ucod = 2
#define upKOSPIx	4		// KOSPI100   : ucod = 1
					// KOSPI50    : ucod = 2
					// KOSPIIT    : ucod = 3
#define	upSECTOR	6		// 섹터지수
#define upFREEBOARD	7		// 프리보드   
#define	upINTEGRATE	9		// 통합지수

#define	upNOT		10		// not
#define	upOTHER		11		// KOSPI IT, KOSPI200
					// KOSPI100, KOSDAQ50


//
//	KOFEX
//

//
//	미래에셋
//

#define KCodeLen        15
#define KNameLen        30

struct	kfcode	{
	char	kcod[2];		// Commodity Code
					// 01 :	USD Currency Futures
					// 02 : USD Currency Call Option
					// 03 : USD Currency Put Option
					// 04 : GOLD Currency Futures
					// 05 : CD Futures
					// 06 : KTB Futures
					// 07 : KOSDAQ50 Futures
	char	codx[KCodeLen];		// 종목코드
	char	hnam[KNameLen];		// 한글명
};

#define kfTOTAL		0
#define	kfUSDFUTURE	1
#define kfUSDCALL	2
#define kfUSDPUT	3
#define kfGOLD		4
#define kfCD		5
#define kfKTB		6
#define kfKOSDAQ	7


//
//	신한
//

#define KFOCodeLen      15
#define KFONameLen      30
#define KFOPriceLen	5
#define KFOMonthLen	4

struct	kfocode	{		
	char	codx[KFOCodeLen];	// 단축코드
					// 선물보통: 6자리
					// 선물복합: 8자리
					// 옵션보통: 9자리
					// 옵션복합:11자리
					// 나머지 NULL
	char	hnam[KFONameLen];	// 한글명
	char	flag;			// 선물옵션 구분 FLAG
					// 1:선물
					// 2:CALL 옵션
					// 3:PUT  옵션
	char	mont[KFOMonthLen];	// 1-12 월물
	char	comb;			// 정형복합구분
					// 1:Time  Spread
					// 2:Price Spread
					// 0:복합 비해당
	char	atmf;			// ATM 해당여부
					// 1   :ATM
					// else:비해당
	char	hsga[KFOPriceLen];	// 행사가격 (옵션에만 해당)
};


//
//	현물옵션
//

#define PCodeLen	8

struct pjcode {
	char	gubn;		// 1:KOSPI, 2:KOSDAQ
	char	tjgb[2];	// 거래대상종목구분
	char	snam[20];	// 거래대상종목명

	char	codx[8];	// 단축코드
	char	hnam[30];	// 한글명
	char	enam[30];	// 영문명
	char	gcod[12];	// 기초자산종목코드
	char	mont[4];	// 월물
	char	atmx;		// ATM종목여부 1:ATM종목
	char	hsga[PCodeLen];	// 행사가
	char	mchk;		// 매매유형허용
				// 0x01:시장가허용
				// 0x02:조건부지정가허용
				// 0x04:최유리지정가허용
};

#define	L_pjcode	sizeof(struct pjcode)


//
//	ELW
//

#define	ELWCodeLen	12
#define	ELWHNameLen	32
#define	ELWENameLen	20

struct elwcode {
	char	codx[ELWCodeLen];// ELW종목코드(6) J######
	char	hnam[ELWHNameLen];	// 한글명
	char	enam[ELWENameLen];	// 형문명
	char	symb[7];	// SYMBOL
	char	lphn[5][3];	// ELW - LP 회원번호 1~5
	char	lpse[5];	// ELW 스프레드 (999V99)
	char	hbge[ELWENameLen];	// ELW 한글발행기관명
	char	ebge[ELWENameLen];	// ELW 영문발행기관명
	char	bgce[12];	// ELW 발행기관코드
	char	gcjs[5][12];	// ELW 기초자산 1~5
	char	grat[5][5];	// ELW 기초자산구성비율 1~5 (999V99)
	char	gcge[10];	// ELW 기초가격 (99999999V99)
	char	hsge[10];	// ELW 행사가격 (99999999V99)
	char	krye[2];	// ELW 권리유형 01:CALL, 02:PUT, 03:기타
	char	khbe[2];	// ELW 권리행사방식 01:유럽형, 02:미국형, 03:기타
	char	mjmt[8];	// ELW 만기월 (YYYYMMDD)
	char	fill[40];	// reserved
};

#define L_elwcode	sizeof(struct elwcode)


// 소매채권(sjcode.dat)
#define	SCodeLen	12
#define	SNameLen	20

struct	sjcode	{
	char	code[SCodeLen];
	char	cgub;			// 채권구분
					// 1: 국채
					// 2: 통안채
					// 3: 금융채
					// 4: 비금융특수채
					// 5: 최사채
					// 6: 기타
	char	hnam[SNameLen];		// 채권이름
	char    ngub;			// 구분 0: 전체, 1:최근종목
};

//
//	sjcode : 소매채권구분
//
#define	sjKC	1			// 국채
#define	sjTA	2			// 통안채
#define	sjKY	3			// 금융채
#define	sjNK	4			// 비금융특수채
#define	sjCS	5			// 최사채
#define	sjET	6			// 기타


#endif 
