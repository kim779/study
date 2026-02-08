#pragma once

#define TK_PIBONEWS		(1)

#define LINE_SIZE	130		/* LINE size		*/
#define TITL_SIZE   132     /* TITLE size       */
#define SCODE_CNT		10
#define SCODE_SIZE		12

struct  pibonews_mid {
    char    date[8];        /* 자료일자     */
    char    gubn[2];        /* 정보구분     */
    char    subg[4];        /* 분류코드     */
    char    seqn[10];       /* 일련번호     */
    char    titl[TITL_SIZE];     /* 뉴스 제목        */
};

struct  pibonews_mod {
	char	cflag[1];				//2017.02.10 KSJ 데이터 맞춤
	char    titl[TITL_SIZE];     /* 뉴스 제목        */
    char    subcod[SCODE_CNT][SCODE_SIZE]; /* 서브 코드 */
    char    size[7];        /* size of data     */	//2015.11.20 KSJ size 5 -> 7
    char    data[1];
};

struct hjcode {				/* 현물종목코드 정보            */
    char    codx[12];       /* 코드(7) : A######            */
							/* 코드(9) : 신주인수권, 증서   */
    char    hnam[40];       /* 한글명                       */
    char    enam[40];       /* 영문명                       */
    char    ecnf;           /* ECN (1:거래가능)             */
    char    jsiz;           /* 업종자본금 대중소            */
							/* 장내(대: 38, 중: 39, 소: 40) */
							/* 장외(1: KOSDAQ 100)          */
							/* 장외(2: KOSDAQ MID 300)      */
							/* 장외(3: KOSDAQ SMALL)        */
							/* ############################ */
							/* 배당지수 : (jsiz += 41)      */
							/* 신주인수권종목 FULL코드 용   */
    char    symb[7];        /* SYMBOL                       */
    char    size;           /* 시가총액규모(2, 3, 4)        */
    char    ucdm;           /* 업종중분류       99:미분류   */
    char    ucds;           /* 업종소분류       99:미분류   */
							/* KOSDAQ 7:벤쳐                */
    char    jjug;           /* 제조업 구분(27: 제조업)      */
							/* KOSDAQ 벤쳐 종합(=2)         */
    char    kpgb;           /* KOSPI200/KOSDAQ 구분         */
							/* 0:미포함 1:K200 2:K100 3:K50 */
							/* 0:미포함 1:KQ50              */
    char    kosd;           /* 종목구분                     */
							/* 0:장내  10:장외  30:제3시장  */
    char    ssgb;           /* 소속구분                     */
							/* 0:일반                       */
							/* 3:외국인예탁증서             */
							/* 4:뮤추얼펀드                 */
							/* 5:신주인수권(중권)           */
							/* 6:리츠                       */
							/* 7:신주인수권(증서)           */
							/* 8:ETF                        */
							/* 9:하이일드펀드(수익증권)     */
							/* 10: 선박 11:ELW 12:인프라    */
							/* 13: 해외ETF 14:해외원주      */
    char    ucmd;           /* 우선주구분 0:보통주 5:우선주 */
    char    itgb;           /* KOSPI-IT/KQ-IT50구분(1:채택) */
    char    wsgb;           /* 0:일반 1:지배구조우수기업    */
    char    jqty;           /* 기본주문단위                 */
    char    star;           /* Star 종목구분                */
    char    unio;           /* 통합지수종목 구분            */
							/* 0: 일반, 1:통합지수          */
							/* 2006.01.12 htsadd            */
    char    jchk;           /* 종목검색용 구분              */
							/* 0x01     불성실공시종목      */
							/* 0x02     신규상장종목        */
							/* 0x04     권리락종목          */
							/* 0x08     우선주종목          */
							/* 0x10     관리종목            */
							/* 0x20     감리종목            */
							/* 0x40     거래정지종목        */
							/* 0x80     투자유의종목        */
    char    elwf;           /* 0: 일반, 1: 기초자산         */
    char    sjjs[10];       /* 상장주식수(단위:주)          */
    char    kqpg;           /* 프리미어지수(1:프리미어지수) */
							/* 2:조기종료ELW                */
    char    fill[ 9];       /* reserved                     */
};

