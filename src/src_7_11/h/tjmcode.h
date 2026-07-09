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

#define	ENameLen	20
#define HNameLen	20
#define	HCodeLen	12

struct	hjcode {
	char	code[HCodeLen];		// 코드	(12)
					// 코드(7) : A######
					// 코드(9) : 신주인수권, 증서
	char	hnam[HNameLen];		// 한글명(20)
	char	enam[ENameLen];		// 영문명
	char    schk;		        // 0x01 투자주의, 0x02 투자경고,
					// 0x04 투자위험, 0x08 투자위험예고
	char	jsiz;			// 업종자본금 대중소
					// 장내 (대:38  중:39  소:40)
					// 장외 (1:KOSDAQ 100)
					// 장외 (2:KOSDAQ MID 300)
					// 장외 (3:KOSDAQ SMALL)
					// 배당지수(jsiz += 41)
					// 신주인수권종목 FULL코드용
	char    stgb[2];                // 섹터지수종목 구분
                                        // 0x001:자동차,     0x002:반도체
                                        // 0x004:바이오텍,   0x008:금융
                                        // 0x010:정보통신,   0x020:에너지화학
                                        // 0x040:철강,       0x080:필수소비재
                                        // 0x100:미디어통신, 0x200:건설
                                        // 0x400:비은행금융업
					// 0x000:해당없음
 	char	fill2[5];               //
	char	size;			// 자본금 규모
					// KOSPI  (대:2  중:3  소:4)
					// KOSDAQ (기타서비스:3  코스닥IT종합:4)
	char	ucdm;			// 업종중분류
					// KOSDAQ:5~14  미분류:99
					// 1:종합 2:
	char	ucds;			// 업종소분류
					// KOSDAQ:15~37  벤쳐:7  미분류:99
	char	jjug;			// 제조업 구분
					// 프리보드벤처 : 3
					// KOSPI  (제조업:27)
					// KOSDAQ (벤처지수:2)
	char	kpgb;			// KOSPI, KOSDAQ 구분
					// KOSPI  (0:미포함 1:K200 2:K100 3:K50)
					// KOSDAQ (0:미포함 1:KQ50)
	char	kosd;			// 종목구분
					// 0:장내  10:장외  30:제3시장
	char	ssgb;			// 소속구분
					// 0:default
					// 4:뮤추얼펀드
					// 5:신주인수권
					// 6:리츠
					// 7:신주인수권증서
					// 8:ETF
					// 9:하이일드펀드
	char	ucmd;			// 우선주구분 (0:보통주  5:우선주)
					// 3:프리보드 제조 4: 프리보드 정보/컴퓨터  
	char	itgb;			// KOSPI-IT/KQ-IT50구분(1:채택)
	char	wsgb;			// KOSPI 지배구조지수:42 (0:보통주 1:우수)
	char	jqty;			// 기본주문단위
	char	star;			// KOSDAQ 스타지수
	char	unio;			// 통합지수종목 구분
	char	jchk;			// 종목검색용 구분
					// 0x01 불성실공시
					// 0x02 신규상장종목
					// 0x04 권리락종목
					// 0x08 우선주종목
					// 0x10 관리종목
					// 0x20 이상급등종목
					// 0x40 거래정지종목
					// 0x80 투자유의종목
	char	jch2;			// 종목검색용 구분2
					// 0x01 정리매매종목
	char	k200;		        // k200 2: 제조업 
					//	3: 전기통신
					//      4: 건설업 
					//	5: 유통 
					//      6: 금융 
};	
	
//
//	hjcode.kosd : 종목구분
//
#define	jmKOSPI		0		// KOSPI 종목
#define jmKOSDAQ	10		// 코스닥
#define	jm3RD		30		// 제3시장
#define	jmSECTOR	50		// 섹터지수 (안쓰는 값인데. 코스피, 코스닥종목과 구분하기위하여 임의로 잡았음.)

//
//	hjcode.ssgb : 소속구분
//
#define	jmDEF		0		// default
#define	jmSUIK		3		// 수익증권
#define jmMUFND		4		// 뮤추얼펀드
#define jmSINJU		5		// 신주인수권
#define jmREITS		6		// 리츠
#define jmSINJS		7		// 신주인수권증서
#define jmETF		8		// ETF
#define	jmHYFND		9		// 하이일드펀드
#define	jmELW		11		// ELW
#define jmFETF		13		// 해외ETF
#define jmFREG		14		// 해외원주

//
//	hjcode.ucmd : 보통주, 우선주
//
#define stockCom	0		// 보통주
#define stockPre	5		// 우선주

//
//	hjcode.jchk : 종목검색용 구분
//
#define J_BG    0x01    // 불성실공시
#define J_SJ    0x02	// 신규상장종목
#define J_KR    0x04	// 권리락종목
#define J_WS    0x08	// 우선주종목
#define J_KK    0x10	// 관리종목
#define J_GG    0x20	// 이상급등종목
#define J_GJ    0x40	// 거래정지종목
#define J_TY    0x80	// 투자유의종목

//
//	hjcode.jch2 : 종목검색용 구분2
//
#define J2_JR   0x01	// 정리매매종목



//
//	선물종목코드
//

#define FNameLen	20
#define FCodeLen	8

struct	fjcode {
	char	cod2[FCodeLen];		// 단축코드
	char	hnam[FNameLen];		// 한글명
	char	enam[ENameLen];		// 영문명
	char	mchk;			// 매매유형허용
					// 0x01:시장가허용
					// 0x02:조건부지정가허용
					// 0x04:최유리지정가허용
};

#define COCodxLen	12
#define COHnamLen	32
#define COEnamLen	32
#define COTjgbLen	3
#define COGcjsLen	12
#define COGnamLen	32
#define COFillLen	39

struct  cocode  {                       /* 상품선물 종목코드 정보       */
        char    codx[COCodxLen];               /* 단축코드 (12)                */
        char    hnam[COHnamLen];               /* 한글명                       */
        char    enam[COEnamLen];               /* 영문명                       */
        char    filler;                 /*                              */
        char    tjgb[COTjgbLen];                /* 기초자산 ID                  */
                                        /* B03 : 3년국채                */
                                        /* B05 : 5년국채                */
                                        /* B10 : 10년국채               */
                                        /* CDR : CD                     */
                                        /* MSB : 통안채                 */
                                        /* USD : USD                    */
                                        /* JPY : JPY                    */
                                        /* EUR : 유럽유로               */
                                        /* GLD : 금                     */
                                        /* PMT : 돈육                   */
                                        /* MGD : 미니금                 */
        char    gcjs[COGcjsLen];               /* 기초자산 종목코드(7)         */
        char    gnam[COGnamLen];               /* 기초자산 종목명              */
        char    cmgb;                   /* 최근월물 구분                */
                                        /* 1 : 최근월 2 : 비해당        */
        char    fill[COFillLen];               /* reserved                     */
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
		char	enam[ENameLen];	// 영문명
		char	mchk;		// 매매유형허용
					// 0x01:시장가허용
					// 0x02:조건부지정가허용
					// 0x04:최유리지정가허용
	} call[4]; 
	struct	{
		char	yorn;		// 종목	코드 유무
		char	cod2[OCodeLen];	// 단축종목코드
		char	hnam[ONameLen];	// 한글종목명
		char	enam[ENameLen];	// 영문명
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

typedef struct	cjcode	{
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
}pcjcode;


typedef struct aagcode{
	char code[6];	// 발행기관 코드...
	char name[50];	// 발행기관이름...   
	char up[2];	// 코드
}AAGCODE;

typedef struct agcode{
	char code[6];	// 발행기관 코드...
	char name[50];	// 발행기관이름...   
	char up[2];	// 코드
	char gubn; //발행여부
	char balcode[14]; //코드
}AGCODE;
#define	L_agcode sizeof(struct agcode)
//
//	cjcode : 채권구분
//
typedef struct CJcode{
	char code[12];	// 채권 코드...  12  
	char name[100];	// 채권이름...   100
	char kind[4];	// 채권종류...   4
	char		idct[6];  // 지표구분  6
	char		valday[10];  //발행일   10
	char		mtday[10];   //만기일   10
	char	    mtry[6]; //만기구분    6    
	char		itrt[10];  //금리 10
	char		intertype[6]; //이자유형 6
	char		credit[6];  //신용등급  6
	char		minfore[10];  //민평4    10 
	char		minthree[10];  //민평3   10
	char		update[14];	//업데이트 시각 14	
}CJCODE;

#define	L_CJcode sizeof(struct CJcode)
//
//	업종코드
//

#define	UNameLen	20

struct	upcode	{			// 업종코드 정보
	char	jgub;			// 장구분
					
					// 0:장내 
					// 1:kospi200
					// 2:코스닥
					// 3:KOSTAR
					// 4:K100P(1), K50(2), IT(3)
					// 5:섹터지수
					// 6:프리보드업종
					// 9:통합지수 

	char	ucod;			// 업종코드
	char	hnam[UNameLen];		// 한글명
};



//
//	upcode.jgup : 장구분 : 신한변경
//
#define	upKOSPI		0		// 1~41
#define upKOSPI200	1		// 1~6
#define	upKOSDAQ	2		// 1~40
#define upKOSTAR	3		// KOSTAR
#define upKOSPIx	4		// KOSPI100   : ucod = 1
					// KOSPI50    : ucod = 2
					// KOSPIIT    : ucod = 3
#define	upSECTOR	5		// 섹터지수
#define upFREEBOARD	6		// 프리보드   
#define	upINTEGRATE	9		// 통합지수 999

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
	char	codx[ELWCodeLen];	// ELW종목코드(6) J######
	char	hnam[ELWHNameLen];	// 한글명
	char	enam[ELWENameLen];	// 형문명
	char	symb[7];		// SYMBOL
	char	lphn[5][3];		// ELW - LP 회원번호 1~5
	char	lpse[5];		// ELW 스프레드 (999V99)
	char	hbge[ELWENameLen];	// ELW 한글발행기관명
	char	ebge[ELWENameLen];	// ELW 영문발행기관명
	char	bgce[12];		// ELW 발행기관코드
	char	gcjs[5][12];		// ELW 기초자산 1~5
	char	grat[5][5];		// ELW 기초자산구성비율 1~5 (999V99)
	char	gcge[10];		// ELW 기초가격 (99999999V99)
	char	hsge[10];		// ELW 행사가격 (99999999V99)
	char	krye[2];		// ELW 권리유형 01:CALL, 02:PUT, 03:기타
	char	khbe[2];		// ELW 권리행사방식 01:유럽형, 02:미국형, 03:기타
	char	mjmt[8];		// ELW 만기월 (YYYYMMDD)
	char	fill[40];		// reserved
};

#define L_elwcode	sizeof(struct elwcode)

//
// 주식선물...
//
struct  sfcode {            /* 주식선물 종목코드 정보       */
    char    codx[8];        /* 단축코드(8)                  */
    char    hnam[50];       /* 한글명                       */
    char    enam[50];       /* 영문명                       */
    char    mchk;           /* 매매유형허용                 */
                            /* 0x01:시장가허용              */
                            /* 0x02:조건부지정가허용        */
                            /* 0x04:최유리지정가허용        */
    char    tjgb[2];        /* 거래대상 종목구분            */
    char    gcod[12];       /* 기초자산종목코드             */
    char    gnam[40];       /* 기초자산 종목명              */
    char    fill[20];       /* reserved                     */
};

#define	L_sfcode	sizeof(struct sfcode)

#endif 



