/*************************************************************************************/
/*  1. 프로그램ID : KB_s0001                                                         */
/*  2. 프로그램명 : 주식현재가                                                       */
/*  3. 서비스명   : KB_s0001                                                         */
/*  4. 사용테이블 :                                                                  */
/*  5. 작성자     : drfn                                                             */
/*  6. 입력항목   :                                                                  */
/*  7. 출력항목   :                                                                  */
/*************************************************************************************/
#ifndef __KB_TRAN_KB_s0001_H__
#define __KB_TRAN_KB_s0001_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/
#include "Data_Common.h"

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
//#define KB_TRAN                       "KB_s0001"
#define LLONG                         long long


/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

/* InRec1:                                                                           */
typedef struct
{
    char    shcode[6];                            /* 종목코드(6)                     */
    char    mode[1];                              /* 조회모드(1)                     */
    char    count[4];                             /* 조회건수(4)                     */
}KB_s0001_InRec1;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1:                                                                          */
//char KB_s0001_OutRec1_count[4];
//typedef struct
//{
//    char    shcode[6];                            /* 종목코드(6)                     */
//    char    hname[40];                            /* 종목명(40)                      */
//    char    price[8];                             /* 현재가(8)                       */
//    char    sign[1];                              /* 현재가전일대비구분(1)           */
//    char    change[8];                            /* 현재가전일대비(8)               */
//    char    rate[7];                              /* 현재가전일대비등락율(7)         */
//    char    volume[12];                           /* 거래량(12)                      */
//    char    volrate[7];                           /* 거래량전일대비등락율(7)         */
//    char    value[15];                            /* 거래대금(15)                    */
//    char    volro[9];                             /* 거래량회전율(9)                 */
//    char    jnilvolume[12];                       /* 전일거래량(12)                  */
//    char    jnilvalue[15];                        /* 전일거래대금(15)                */
//    char    jnilclose[8];                         /* 전일종가(8)                     */
//    char    creditgubun[1];                       /* 신용가능여부(1)                 */
//    char    dambogubun[1];                        /* 담보가능여부(1)                 */
//    char    jkrate[3];                            /* 증거금비율(3)                   */
//    char    crdrate[3];                           /* 신용증거금율(3)                 */
//    char    crddate[6];                           /* 신용기간(6)                     */
//    char    crdgrp[1];                            /* 신용그룹(1)                     */
//    char    clmtgubun[1];                         /* 신용한도초과여부(1)             */
//    char    bu12gubun[2];                         /* 소속구분(2)                     */
//    char    manufgubun[2];                        /* 제조업구분(2)                   */
//    char    upcodem[3];                           /* 업종중분류(3)                   */
//    char    upcodes[3];                           /* 업종소분류(3)                   */
//    char    gwangubun[2];                         /* 관리구분(2)                     */
//    char    invwdgubun[1];                        /* 투자경고구분(1)                 */
//    char    invwgubun[2];                         /* 투자주의여부(2)                 */
//    char    dishonest[1];                         /* 불성실공시여부(1)               */
//    char    bjisugubun[2];                        /* 배당지수종목여부(2)             */
//    char    govgubun[2];                          /* 지배구조우수기업종목구분(2)     */
//    char    invdgubun[2];                         /* 위험예고여부(2)                 */
//    char    stopgubun[2];                         /* 거래정지구분(2)                 */
//    char    shusjugubun[1];                       /* 우선주구분(1)                   */
//    char    quotesgubun[1];                       /* 기세구분(1)                     */
//    char    jnilquotesgubun[1];                   /* 전일기세구분(1)                 */
//    char    rightsgubun[2];                       /* 권배락구분(2)                   */
//    char    kospi200[2];                          /* KOSPI200여부(2)                 */
//    char    kospi2gubun[1];                       /* KOSPI200구분(1)                 */
//    char    jisu50100gubun[1];                    /* KOSPI100/KOSPI50구분(1)         */
//    char    krx_100[1];                           /* KRX100지수(1)                   */
//    char    pivot[8];                             /* 피봇(8)                         */
//    char    recline[8];                           /* 기준선(8)                       */
//    char    resist1[8];                           /* 1차저항(8)                      */
//    char    support1[8];                          /* 1차지지(8)                      */
//    char    resist2[8];                           /* 2차저항(8)                      */
//    char    support2[8];                          /* 2차지지(8)                      */
//    char    resist_d[8];                          /* D저항(8)                        */
//    char    support_d[8];                         /* D지지(8)                        */
//    char    high250[8];                           /* 250일최고가(8)                  */
//    char    high250_rate[7];                      /* 250일최고가등락율(7)            */
//    char    high250date[8];                       /* 250일최고가일자(8)              */
//    char    low250[8];                            /* 250일최저가(8)                  */
//    char    low250_rate[7];                       /* 250일최저가등락율(7)            */
//    char    low250date[8];                        /* 250일최저가일자(8)              */
//    char    high52w[8];                           /* 52주최고가(8)                   */
//    char    high52w_rate[7];                      /* 52주최고가등락율(7)             */
//    char    high52wdate[8];                       /* 52주최고가일자(8)               */
//    char    low52w[8];                            /* 52주최저가(8)                   */
//    char    low52w_rate[7];                       /* 52주최저가등락율(7)             */
//    char    low52wdate[8];                        /* 52주최저가일자(8)               */
//    char    recprice[8];                          /* 기준가(8)                       */
//    char    uplmtprice[8];                        /* 상한가(8)                       */
//    char    dnlmtprice[8];                        /* 하한가(8)                       */
//    char    w_avrg[8];                            /* 가중평균주가(8)                 */
//    char    trade_cost[8];                        /* 거래비용(8)                     */
//    char    open[8];                              /* 시가(8)                         */
//    char    opensign[1];                          /* 시가전일대비구분(1)             */
//    char    openchange[8];                        /* 시가전일대비(8)                 */
//    char    openrate[7];                          /* 시가전일종가대비등락율(7)       */
//    char    opentime[6];                          /* 시가시간(6)                     */
//    char    high[8];                              /* 고가(8)                         */
//    char    highsign[1];                          /* 고가전일대비구분(1)             */
//    char    highchange[8];                        /* 고가전일대비(8)                 */
//    char    highrate[7];                          /* 고가전일종가대비등락율(7)       */
//    char    hightime[6];                          /* 고가시간(6)                     */
//    char    low[8];                               /* 저가(8)                         */
//    char    lowsign[1];                           /* 저가전일대비구분(1)             */
//    char    lowchange[8];                         /* 저가전일대비(8)                 */
//    char    lowrate[7];                           /* 저가전일종가대비등락율(7)       */
//    char    lowtime[6];                           /* 저가시간(6)                     */
//    char    target[8];                            /* 목표가(8)                       */
//    char    listdate[8];                          /* 상장일(8)                       */
//    char    floating[12];                         /* 유동주식수(12)                  */
//    char    yjrate[8];                            /* 신용융자잔고비율(8)             */
//    char    yjvolume[6];                          /* 신용융자잔고주수(6)             */
//    char    yjprice[8];                           /* 신용융자잔고금액(8)             */
//    char    subprice[8];                          /* 대용가(8)                       */
//    char    perx[6];                              /* PER(6)                          */
//    char    pbrx[6];                              /* PBR(6)                          */
//    char    psrx[6];                              /* PSR(6)                          */
//    char    epsx[8];                              /* EPS(8)                          */
//    char    parprice[8];                          /* 액면가(8)                       */
//    char    listing[12];                          /* 상장주식수(12)                  */
//    char    capital[17];                          /* 자본금(17)                      */
//    char    flmtvolume[12];                       /* 외국인주문한도수량(12)          */
//    char    fremainreal[12];                      /* 외국인주문가능수량(12)          */
//    char    jonglmtrate[6];                       /* 외국인종목한도비율(6)           */
//    char    perlmtrate[6];                        /* 외국인개인한도비율(6)           */
//    char    fholdvol[12];                         /* 외국인보유수량(12)              */
//    char    exhratio[6];                          /* 외국인보유비중(6)               */
//    char    sigatvalue[12];                       /* 시가총액(12)                    */
//    char    gsmm[2];                              /* 결산월(2)                       */
//    char    bfname[10];                           /* 전분기명(10)                    */
//    char    bfsales[12];                          /* 전분기매출액(12)                */
//    char    bfoper[12];                           /* 전분기영업이익(12)              */
//    char    bford[12];                            /* 전분기경상이익(12)              */
//    char    bfnet[12];                            /* 전분기순이익(12)                */
//    char    bfeps[9];                             /* 전분기EPS(9)                    */
//    char    bfname2[10];                          /* 전전분기명(10)                  */
//    char    bfsales2[12];                         /* 전전분기매출액(12)              */
//    char    bfoper2[12];                          /* 전전분기영업이익(12)            */
//    char    bford2[12];                           /* 전전분기경상이익(12)            */
//    char    bfnet2[12];                           /* 전전분기순이익(12)              */
//    char    bfeps2[9];                            /* 전전분기EPS(9)                  */
//    char    sales_rate[9];                        /* 매출액증감율(9)                 */
//    char    oper_rate[9];                         /* 영업이익증감율(9)               */
//    char    ord_rate[9];                          /* 경상이익증감율(9)               */
//    char    net_rate[9];                          /* 순이익증감율(9)                 */
//    char    eps_rate[9];                          /* EPS증감율(9)                    */
//    char    hotime[6];                            /* 호가수신시간(6)                 */
//    char    offerho[8];                           /* 우선매도호가(8)                 */
//    char    bidho[8];                             /* 우선매수호가(8)                 */
//    char    offerho1[8];                          /* 매도호가1(8)                    */
//    char    offerhorate1[7];                      /* 매도호가등락율1(7)              */
//    char    offerrem1[8];                         /* 매도호가수량1(8)                */
//    char    preoffercha1[8];                      /* 매도직전대비수량1(8)            */
//    char    offerho2[8];                          /* 매도호가2(8)                    */
//    char    offerhorate2[7];                      /* 매도호가등락율2(7)              */
//    char    offerrem2[8];                         /* 매도호가수량2(8)                */
//    char    preoffercha2[8];                      /* 매도직전대비수량2(8)            */
//    char    offerho3[8];                          /* 매도호가3(8)                    */
//    char    offerhorate3[7];                      /* 매도호가등락율3(7)              */
//    char    offerrem3[8];                         /* 매도호가수량3(8)                */
//    char    preoffercha3[8];                      /* 매도직전대비수량3(8)            */
//    char    offerho4[8];                          /* 매도호가4(8)                    */
//    char    offerhorate4[7];                      /* 매도호가등락율4(7)              */
//    char    offerrem4[8];                         /* 매도호가수량4(8)                */
//    char    preoffercha4[8];                      /* 매도직전대비수량4(8)            */
//    char    offerho5[8];                          /* 매도호가5(8)                    */
//    char    offerhorate5[7];                      /* 매도호가등락율5(7)              */
//    char    offerrem5[8];                         /* 매도호가수량5(8)                */
//    char    preoffercha5[8];                      /* 매도직전대비수량5(8)            */
//    char    offerho6[8];                          /* 매도호가6(8)                    */
//    char    offerhorate6[7];                      /* 매도호가등락율6(7)              */
//    char    offerrem6[8];                         /* 매도호가수량6(8)                */
//    char    preoffercha6[8];                      /* 매도직전대비수량6(8)            */
//    char    offerho7[8];                          /* 매도호가7(8)                    */
//    char    offerhorate7[7];                      /* 매도호가등락율7(7)              */
//    char    offerrem7[8];                         /* 매도호가수량7(8)                */
//    char    preoffercha7[8];                      /* 매도직전대비수량7(8)            */
//    char    offerho8[8];                          /* 매도호가8(8)                    */
//    char    offerhorate8[7];                      /* 매도호가등락율8(7)              */
//    char    offerrem8[8];                         /* 매도호가수량8(8)                */
//    char    preoffercha8[8];                      /* 매도직전대비수량8(8)            */
//    char    offerho9[8];                          /* 매도호가9(8)                    */
//    char    offerhorate9[7];                      /* 매도호가등락율9(7)              */
//    char    offerrem9[8];                         /* 매도호가수량9(8)                */
//    char    preoffercha9[8];                      /* 매도직전대비수량9(8)            */
//    char    offerho10[8];                         /* 매도호가10(8)                   */
//    char    offerhorate10[7];                     /* 매도호가등락율10(7)             */
//    char    offerrem10[8];                        /* 매도호가수량10(8)               */
//    char    preoffercha10[8];                     /* 매도직전대비수량10(8)           */
//    char    bidho1[8];                            /* 매수호가1(8)                    */
//    char    bidhorate1[7];                        /* 매수호가등락율1(7)              */
//    char    bidrem1[8];                           /* 매수호가수량1(8)                */
//    char    prebidcha1[8];                        /* 매수직전대비수량1(8)            */
//    char    bidho2[8];                            /* 매수호가2(8)                    */
//    char    bidhorate2[7];                        /* 매수호가등락율2(7)              */
//    char    bidrem2[8];                           /* 매수호가수량2(8)                */
//    char    prebidcha2[8];                        /* 매수직전대비수량2(8)            */
//    char    bidho3[8];                            /* 매수호가3(8)                    */
//    char    bidhorate3[7];                        /* 매수호가등락율3(7)              */
//    char    bidrem3[8];                           /* 매수호가수량3(8)                */
//    char    prebidcha3[8];                        /* 매수직전대비수량3(8)            */
//    char    bidho4[8];                            /* 매수호가4(8)                    */
//    char    bidhorate4[7];                        /* 매수호가등락율4(7)              */
//    char    bidrem4[8];                           /* 매수호가수량4(8)                */
//    char    prebidcha4[8];                        /* 매수직전대비수량4(8)            */
//    char    bidho5[8];                            /* 매수호가5(8)                    */
//    char    bidhorate5[7];                        /* 매수호가등락율5(7)              */
//    char    bidrem5[8];                           /* 매수호가수량5(8)                */
//    char    prebidcha5[8];                        /* 매수직전대비수량5(8)            */
//    char    bidho6[8];                            /* 매수호가6(8)                    */
//    char    bidhorate6[7];                        /* 매수호가등락율6(7)              */
//    char    bidrem6[8];                           /* 매수호가수량6(8)                */
//    char    prebidcha6[8];                        /* 매수직전대비수량6(8)            */
//    char    bidho7[8];                            /* 매수호가7(8)                    */
//    char    bidhorate7[7];                        /* 매수호가등락율7(7)              */
//    char    bidrem7[8];                           /* 매수호가수량7(8)                */
//    char    prebidcha7[8];                        /* 매수직전대비수량7(8)            */
//    char    bidho8[8];                            /* 매수호가8(8)                    */
//    char    bidhorate8[7];                        /* 매수호가등락율8(7)              */
//    char    bidrem8[8];                           /* 매수호가수량8(8)                */
//    char    prebidcha8[8];                        /* 매수직전대비수량8(8)            */
//    char    bidho9[8];                            /* 매수호가9(8)                    */
//    char    bidhorate9[7];                        /* 매수호가등락율9(7)              */
//    char    bidrem9[8];                           /* 매수호가수량9(8)                */
//    char    prebidcha9[8];                        /* 매수직전대비수량9(8)            */
//    char    bidho10[7];                           /* 매수호가10(7)                   */
//    char    bidhorate10[8];                       /* 매수호가등락율10(8)             */
//    char    bidrem10[8];                          /* 매수호가수량10(8)               */
//    char    prebidcha10[8];                       /* 매수직전대비수량10(8)           */
//    char    totofferrem[8];                       /* 매도호가총수량(8)               */
//    char    preoffercha[8];                       /* 매도호가총수량직전대비(8)       */
//    char    totbidrem[8];                         /* 매수호가총수량(8)               */
//    char    prebidcha[8];                         /* 매수호가총수량직전대비(8)       */
//    char    netbidrem[8];                         /* 순매수총수량(8)                 */
//    char    tmofferrem[8];                        /* 시간외매도잔량(8)               */
//    char    pretmoffercha[8];                     /* 시간외매도수량직전대비(8)       */
//    char    tmbidrem[8];                          /* 시간외매수잔량(8)               */
//    char    pretmbidcha[8];                       /* 시간외매수수량직전대비(8)       */
//    char    yofferho0[8];                         /* 예상매도호가(8)                 */
//    char    yofferrem0[8];                        /* 예상매도호가수량(8)             */
//    char    ypreoffercha0[8];                     /* 예상직전매도대비수량(8)         */
//    char    yofferrem[8];                         /* 예상매도호가잔량합(8)           */
//    char    ybidho0[8];                           /* 예상매수호가(8)                 */
//    char    ybidrem0[8];                          /* 예상매수호가수량(8)             */
//    char    yprebidcha0[8];                       /* 예상직전매수대비수량(8)         */
//    char    ybidrem[8];                           /* 예상매수호가잔량합(8)           */
//    char    ychetime[6];                          /* 예상체결시간(6)                 */
//    char    yeprice[8];                           /* 예상체결가격(8)                 */
//    char    yevolume[8];                          /* 예상체결수량(8)                 */
//    char    preychange[8];                        /* 예상체결가직전현재가대비(8)     */
//    char    preysign[1];                          /* 예상체결가직전현재가대비구분(1) */
//    char    jnilychange[8];                       /* 예상체결가전일종가대비(8)       */
//    char    jnilysign[1];                         /* 예상체결가전일종가대비구분(1)   */
//    char    yecgubun[1];                          /* 예상체결구분(1)                 */
//    char    yecvolume[8];                         /* 예상체결변동량(8)               */
//    char    ymdvolume[8];                         /* 예상매도체결수량(8)             */
//    char    ymsvolume[8];                         /* 예상매수체결수량(8)             */
//    char    dan_jnilclose[8];                     /* 전일단일가매매종가(8)           */
//    char    dan_jnilvolume[12];                   /* 전일단일가매매거래량(12)        */
//    char    dan_jnilvalue[15];                    /* 전일단일가매매거래대금(15)      */
//    char    dan_uplmtprice[8];                    /* 시외상한가격(8)                 */
//    char    dan_dnlmtprice[8];                    /* 시외하한가격(8)                 */
//    char    dan_preclose[8];                      /* 시외당일종가(8)                 */
//    char    dan_quotesgubun[1];                   /* 시외기세구분(1)                 */
//    char    dan_chetime[6];                       /* 시외체결시간(6)                 */
//    char    dan_price[8];                         /* 시외현재가(8)                   */
//    char    dan_opentime[6];                      /* 시외시가시간(6)                 */
//    char    dan_hightime[6];                      /* 시외고가시간(6)                 */
//    char    dan_lowtime[6];                       /* 시외저가시간(6)                 */
//    char    dan_open[8];                          /* 시외시가(8)                     */
//    char    dan_high[8];                          /* 시외고가(8)                     */
//    char    dan_low[8];                           /* 시외저가(8)                     */
//    char    dan_sign[1];                          /* 시외상/하한부호(1)              */
//    char    dan_change[8];                        /* 시외당일종가대비(8)             */
//    char    dan_volume[12];                       /* 시외매매누적거래량(12)          */
//    char    dan_value[15];                        /* 시외매매누적거래대금(15)        */
//    char    dan_cvolume[8];                       /* 시외체결수량(8)                 */
//    char    dan_cgubun[1];                        /* 시외체결구분(1)                 */
//    char    dan_mdvolume[8];                      /* 시외매도누적체결량(8)           */
//    char    dan_msvolume[8];                      /* 시외매수누적체결량(8)           */
//    char    dan_mdchecnt[8];                      /* 시외매도체결건수(8)             */
//    char    dan_mschecnt[8];                      /* 시외매수체결건수(8)             */
//    char    dan_prevolume[8];                     /* 시외직전거래량(8)               */
//    char    dan_precvolume[8];                    /* 시외직전체결수량(8)             */
//    char    dan_hotime[6];                        /* 시외수신시간(6)                 */
//    char    dan_offerho1[8];                      /* 시외매도호가1(8)                */
//    char    dan_offerrem1[8];                     /* 시외매도호가수량1(8)            */
//    char    dan_preoffercha1[8];                  /* 시외매도직전대비수량1(8)        */
//    char    dan_offerho2[8];                      /* 시외매도호가2(8)                */
//    char    dan_offerrem2[8];                     /* 시외매도호가수량2(8)            */
//    char    dan_preoffercha2[8];                  /* 시외매도직전대비수량2(8)        */
//    char    dan_offerho3[8];                      /* 시외매도호가3(8)                */
//    char    dan_offerrem3[8];                     /* 시외매도호가수량3(8)            */
//    char    dan_preoffercha3[8];                  /* 시외매도직전대비수량3(8)        */
//    char    dan_offerho4[8];                      /* 시외매도호가4(8)                */
//    char    dan_offerrem4[8];                     /* 시외매도호가수량4(8)            */
//    char    dan_preoffercha4[8];                  /* 시외매도직전대비수량4(8)        */
//    char    dan_offerho5[8];                      /* 시외매도호가5(8)                */
//    char    dan_offerrem5[8];                     /* 시외매도호가수량5(8)            */
//    char    dan_preoffercha5[8];                  /* 시외매도직전대비수량5(8)        */
//    char    dan_bidho1[8];                        /* 시외매수호가1(8)                */
//    char    dan_bidrem1[8];                       /* 시외매수호가수량1(8)            */
//    char    dan_prebidcha1[8];                    /* 시외매수직전대비수량1(8)        */
//    char    dan_bidho2[8];                        /* 시외매수호가2(8)                */
//    char    dan_bidrem2[8];                       /* 시외매수호가수량2(8)            */
//    char    dan_prebidcha2[8];                    /* 시외매수직전대비수량2(8)        */
//    char    dan_bidho3[8];                        /* 시외매수호가3(8)                */
//    char    dan_bidrem3[8];                       /* 시외매수호가수량3(8)            */
//    char    dan_prebidcha3[8];                    /* 시외매수직전대비수량3(8)        */
//    char    dan_bidho4[8];                        /* 시외매수호가4(8)                */
//    char    dan_bidrem4[8];                       /* 시외매수호가수량4(8)            */
//    char    dan_prebidcha4[8];                    /* 시외매수직전대비수량4(8)        */
//    char    dan_bidho5[8];                        /* 시외매수호가5(8)                */
//    char    dan_bidrem5[8];                       /* 시외매수호가수량5(8)            */
//    char    dan_prebidcha5[8];                    /* 시외매수직전대비수량5(8)        */
//    char    dan_totofferrem[8];                   /* 시외매도호가총수량(8)           */
//    char    dan_preoffercha[8];                   /* 시외매도호가총직전대비(8)       */
//    char    dan_totbidrem[8];                     /* 시외매수호가총수량(8)           */
//    char    dan_prebidcha[8];                     /* 시외매수호가총직전대비(8)       */
//    char    dan_ychetime[6];                      /* 시외예상체결시간(6)             */
//    char    dan_yeprice[8];                       /* 시외예상체결가격(8)             */
//    char    dan_yevolume[8];                      /* 시외예상체결수량(8)             */
//    char    dan_preychange[8];                    /* 시외예상가직전현재가대비(8)     */
//    char    dan_preysign[1];                      /* 시외예상가직전현재가대비구분(1) */
//    char    dan_jnilychange[8];                   /* 시외예상가당일종가대비(8)       */
//    char    dan_jnilysign[1];                     /* 시외예상가당일종가대비구분(1)   */
//    char    offerno1[3];                          /* 매도증권사코드1(3)              */
//    char    bidno1[3];                            /* 매수증권사코드1(3)              */
//    char    tradmdvol1[8];                        /* 총매도수량1(8)                  */
//    char    tradmsvol1[8];                        /* 총매수수량1(8)                  */
//    char    tradmdcha1[8];                        /* 매도거래량직전대비1(8)          */
//    char    tradmscha1[8];                        /* 매수거래량직전대비1(8)          */
//    char    offerno2[3];                          /* 매도증권사코드2(3)              */
//    char    bidno2[3];                            /* 매수증권사코드2(3)              */
//    char    tradmdvol2[8];                        /* 총매도수량2(8)                  */
//    char    tradmsvol2[8];                        /* 총매수수량2(8)                  */
//    char    tradmdcha2[8];                        /* 매도거래량직전대비2(8)          */
//    char    tradmscha2[8];                        /* 매수거래량직전대비2(8)          */
//    char    offerno3[3];                          /* 매도증권사코드3(3)              */
//    char    bidno3[3];                            /* 매수증권사코드3(3)              */
//    char    tradmdvol3[8];                        /* 총매도수량3(8)                  */
//    char    tradmsvol3[8];                        /* 총매수수량3(8)                  */
//    char    tradmdcha3[8];                        /* 매도거래량직전대비3(8)          */
//    char    tradmscha3[8];                        /* 매수거래량직전대비3(8)          */
//    char    offerno4[3];                          /* 매도증권사코드4(3)              */
//    char    bidno4[3];                            /* 매수증권사코드4(3)              */
//    char    tradmdvol4[8];                        /* 총매도수량4(8)                  */
//    char    tradmsvol4[8];                        /* 총매수수량4(8)                  */
//    char    tradmdcha4[8];                        /* 매도거래량직전대비4(8)          */
//    char    tradmscha4[8];                        /* 매수거래량직전대비4(8)          */
//    char    offerno5[3];                          /* 매도증권사코드5(3)              */
//    char    bidno5[3];                            /* 매수증권사코드5(3)              */
//    char    tradmdvol5[8];                        /* 총매도수량5(8)                  */
//    char    tradmsvol5[8];                        /* 총매수수량5(8)                  */
//    char    tradmdcha5[8];                        /* 매도거래량직전대비5(8)          */
//    char    tradmscha5[8];                        /* 매수거래량직전대비5(8)          */
//    char    ftradmdvol[8];                        /* 외국계증권사매도합계(8)         */
//    char    ftradmsvol[8];                        /* 외국계증권사매수합계(8)         */
//    char    ftradmdcha[8];                        /* 외국계증권사매도직전대비(8)     */
//    char    ftradmscha[8];                        /* 외국계증권사매수직전대비(8)     */
//    char    ftradnetvol[8];                       /* 외국계증권사순매수합계(8)       */
//    char    mmsdate[8];                           /* 정리매매개시일(8)               */
//    char    mmedate[8];                           /* 정리매매종료일(8)               */
//    char    memedan[5];                           /* 매매수량단위(5)                 */
//    char    etfgu[2];                             /* ETF분류(2)                      */
//    char    etfupl[2];                            /* ETF관련지수업종대(2)            */
//    char    etfupm[3];                            /* ETF관련지수업종중(3)            */
//    char    etfups[3];                            /* ETF관련지수업종소(3)            */
//    char    etfcu[8];                             /* ETFCU단위(8)                    */
//    char    etfnum[4];                            /* ETF구성종목수(4)                */
//    char    etftotcap[15];                        /* ETF순자산총액(15)               */
//    char    etfdiv[2];                            /* ETF배당주기(2)                  */
//    char    etfratio[9];                          /* ETF관련지수대비비율(9)          */
//    char    prenav[9];                            /* ETF최종NAV(9)                   */
//    char    etfcurstokcnt[10];                    /* ETF유통주식수(10)               */
//    char    etftotcap_f[15];                      /* ETF순자산총액(15)               */
//    char    etfcurtotcap_f[15];                   /* ETF유통순자산총액(15)           */
//    char    prenav_f[9];                          /* ETF최종NAV(9)                   */
//    char    gmprice[6];                           /* 최근월물현재가(6)               */
//    char    gmsign[1];                            /* 최근월물전일대비구분(1)         */
//    char    gmchange[6];                          /* 최근월물전일대비(6)             */
//    char    gmrate[6];                            /* 최근월물등락율(6)               */
//    char    gmgrdegree[7];                        /* 최근월물괴리도(7)               */
//    char    gmgrrate[7];                          /* 최근월물괴리율(7)               */
//    char    gmgrratesign[1];                      /* 최근월물괴리율부호(1)           */
//    char    navprice[7];                          /* NAV현재가(7)                    */
//    char    navsign[1];                           /* NAV전일대비구분(1)              */
//    char    navchange[6];                         /* NAV전일대비(6)                  */
//    char    navrate[6];                           /* NAV등락율(6)                    */
//    char    navgrdegree[7];                       /* NAV괴리도(7)                    */
//    char    navgrrate[7];                         /* NAV괴리율(7)                    */
//    char    navgrratesign[1];                     /* NAV괴리율부호(1)                */
//    char    kospijisu[6];                         /* KOSPI200지수(6)                 */
//    char    kospisign[1];                         /* KOSPI200전일대비구분(1)         */
//    char    kospichange[6];                       /* KOSPI200전일대비(6)             */
//    char    kospirate[6];                         /* KOSPI200등락율(6)               */
//    char    kospigrdegree[7];                     /* KOSPI200괴리도(7)               */
//    char    kospigrrate[7];                       /* KOSPI200괴리율(7)               */
//    char    kospigrratesign[1];                   /* KOSPI200괴리율부호(1)           */
//    char    issueprice[8];                        /* 발행가(8)                       */
//    char    memnum1[3];                           /* ELWLP회원번호1(3)               */
//    char    memnum2[3];                           /* ELWLP회원번호2(3)               */
//    char    memnum3[3];                           /* ELWLP회원번호3(3)               */
//    char    memnum4[3];                           /* ELWLP회원번호4(3)               */
//    char    memnum5[3];                           /* ELWLP회원번호5(3)               */
//    char    memnum6[3];                           /* ELWLP회원번호6(3)               */
//    char    memnum7[3];                           /* ELWLP회원번호7(3)               */
//    char    memnum8[3];                           /* ELWLP회원번호8(3)               */
//    char    memnum9[3];                           /* ELWLP회원번호9(3)               */
//    char    memnum10[3];                          /* ELWLP회원번호10(3)              */
//    char    espread[5];                           /* ELW스프레드(5)                  */
//    char    issuernmk[20];                        /* ELW한글발행기관명(20)           */
//    char    issuernme[20];                        /* ELW영문발행기관명(20)           */
//    char    issuercd[12];                         /* ELW발행기관코드(12)             */
//    char    item1[12];                            /* ELW기초자산1(12)                */
//    char    item2[12];                            /* ELW기초자산2(12)                */
//    char    item3[12];                            /* ELW기초자산3(12)                */
//    char    item4[12];                            /* ELW기초자산4(12)                */
//    char    item5[12];                            /* ELW기초자산5(12)                */
//    char    comprate1[5];                         /* ELW기초자산구성비율1(5)         */
//    char    comprate2[5];                         /* ELW기초자산구성비율2(5)         */
//    char    comprate3[5];                         /* ELW기초자산구성비율3(5)         */
//    char    comprate4[5];                         /* ELW기초자산구성비율4(5)         */
//    char    comprate5[5];                         /* ELW기초자산구성비율5(5)         */
//    char    elwbase[10];                          /* ELW기초가격(10)                 */
//    char    elwexec[10];                          /* ELW행사가격(10)                 */
//    char    elwopt[2];                            /* ELW권리유형(2)                  */
//    char    elwtype[2];                           /* ELW권리행사방식(2)              */
//    char    elwdetail[100];                       /* ELW권리내용(100)                */
//    char    settletype[2];                        /* ELW결제방법(2)                  */
//    char    lastdate[8];                          /* ELW최종거래일(8)                */
//    char    convrate[12];                         /* ELW전환비율(12)                 */
//    char    riserate[5];                          /* ELW가격상승참여율(5)            */
//    char    compensate[5];                        /* ELW보상율(5)                    */
//    char    payday[8];                            /* ELW지금일(8)                    */
//    char    rept[20];                             /* ELW지급대리인(20)               */
//    char    valuation[100];                       /* ELW만기평가가격방식(100)        */
//    char    invidx[2];                            /* ELW권리형태(2)                  */
//    char    lp_holdvol[13];                       /* ELWLP보유수량(13)               */
//    char    lpord[2];                             /* LP주문가능여부(2)               */
//    char    lp_multi[3];                          /* 호가수량배수(3)                 */
//    char    krx_autos[1];                         /* KRX_Autos(1)                    */
//    char    krx_semicon[1];                       /* KRX_Semicon(1)                  */
//    char    krx_health[1];                        /* KRX_HealthCare(1)               */
//    char    krx_banks[1];                         /* KRX_Banks(1)                    */
//    char    krx_it[1];                            /* KRX_IT(1)                       */
//    char    krx_energychem[1];                    /* KRX_Energy&Chemicals(1)         */
//    char    krx_steels[1];                        /* KRX_Steels(1)                   */
//    char    krx_consumer[1];                      /* KRX_Consumer_Staples(1)         */
//    char    krx_mediatel[1];                      /* KRX_Media&Telecom(1)            */
//    char    krx_construct[1];                     /* KRX_Constructions(1)            */
//    char    krx_finance[1];                       /* KRX_Financisals(1)              */
//    char    cvolume[8];                           /* 체결수량(8)                     */
//    char    cgubun[1];                            /* 체결구분(1)                     */
//    char    cmmgubun[2];                          /* 매매구분(2)                     */
//    char    mdvolume[8];                          /* 매도누적체결량(8)               */
//    char    msvolume[8];                          /* 매수누적체결량(8)               */
//    char    mdchecnt[8];                          /* 매도누적체결건수(8)             */
//    char    mschecnt[8];                          /* 매수누적체결건수(8)             */
//    char    prevolume[8];                         /* 직전거래량(8)                   */
//    char    precvolume[8];                        /* 직전체결수량(8)                 */
//    char    detour_gu[2];                         /* 우회상장구분(2)                 */
//    char    det_reason[2];                        /* 우회상장사유(2)                 */
//    char    det_st_date[8];                       /* 우회상장개시일(8)               */
//    char    det_end_date[8];                      /* 우회상장종료일(8)               */
//    char    stop_reason[2];                       /* 거래정지사유(2)                 */
//    char    divdate1[4];                          /* 배당기준일1(4)                  */
//    char    divdate2[4];                          /* 배당기준일2(4)                  */
//    char    divdate3[4];                          /* 배당기준일3(4)                  */
//    char    divdate4[4];                          /* 배당기준일4(4)                  */
//    char    confirmpay[8];                        /* 확정지급액(8)                   */
//    char    assetbasemoney[3];                    /* 자산기준통화(3)                 */
//    char    high1[8];                             /* 전일최고가(8)                   */
//    char    low1[8];                              /* 전일최저가(8)                   */
//    char    high5date[8];                         /* 5일최고가일자(8)                */
//    char    high5[8];                             /* 5일최고가(8)                    */
//    char    low5date[8];                          /* 5일최저가일자(8)                */
//    char    low5[8];                              /* 5일최저가(8)                    */
//    char    high10date[8];                        /* 10일최고가일자(8)               */
//    char    high10[8];                            /* 10일최고가(8)                   */
//    char    low10date[8];                         /* 10일최저가일자(8)               */
//    char    low10[8];                             /* 10일최저가(8)                   */
//    char    high20date[8];                        /* 20일최고가일자(8)               */
//    char    high20[8];                            /* 20일최고가(8)                   */
//    char    low20date[8];                         /* 20일최저가일자(8)               */
//    char    low20[8];                             /* 20일최저가(8)                   */
//    char    high60date[8];                        /* 60일최고가일자(8)               */
//    char    high60[8];                            /* 60일최고가(8)                   */
//    char    low60date[8];                         /* 60일최저가일자(8)               */
//    char    low60[8];                             /* 60일최저가(8)                   */
//    char    high90date[8];                        /* 90일최고가일자(8)               */
//    char    high90[8];                            /* 90일최고가(8)                   */
//    char    low90date[8];                         /* 90일최저가일자(8)               */
//    char    low90[8];                             /* 90일최저가(8)                   */
//    char    highyeardate[8];                      /* 년중최고가일자(8)               */
//    char    highyear[8];                          /* 년중최고가(8)                   */
//    char    highyear_rate[7];                     /* 년중최고가등락율(7)             */
//    char    lowyeardate[8];                       /* 년중최저가일자(8)               */
//    char    lowyear[8];                           /* 년중최저가(8)                   */
//    char    lowyear_rate[7];                      /* 년중최저가등락율(7)             */
//    char    hvolyeardate[8];                      /* 년중최고거래량일자(8)           */
//    char    hvolyear[12];                         /* 년중최고거래량(12)              */
//    char    lvolyeardate[8];                      /* 년중최저거래량일자(8)           */
//    char    lvolyear[12];                         /* 년중최저거래량(12)              */
//    char    uplmtdaycnt[8];                       /* 연속상한일수(8)                 */
//    char    updaycnt[8];                          /* 연속상승일수(8)                 */
//    char    dnlmtdaycnt[8];                       /* 연속하한일수(8)                 */
//    char    dndaycnt[8];                          /* 연속하락일수(8)                 */
//    char    bhdaycnt[8];                          /* 연속보합일수(8)                 */
//    char    upday10cnt[8];                        /* 최근10일간상승일수(8)           */
//    char    upday20cnt[8];                        /* 최근20일간상승일수(8)           */
//    char    uplmtitime[6];                        /* 상한가최초진입시간(6)           */
//    char    uplmtgubun[1];                        /* 상한유지구분(1)                 */
//    char    uplmtstime[6];                        /* 상한가최종진입시간(6)           */
//    char    uplmtetime[6];                        /* 상한가최종이탈시간(6)           */
//    char    uplmtlow[8];                          /* 상한가이후최저가(8)             */
//    char    dnlmtitime[6];                        /* 하한가최초진입시간(6)           */
//    char    dnlmtgubun[1];                        /* 하한유지구분(1)                 */
//    char    dnlmtstime[6];                        /* 하한가최종진입시간(6)           */
//    char    dnlmtetime[6];                        /* 하한가최종이탈시간(6)           */
//    char    dnlmthigh[8];                         /* 하한가이후최고가(8)             */
//    char    upneargubun[1];                       /* 상한근접유지구분(1)             */
//    char    upnearstime[6];                       /* 상한가근접진입시간(6)           */
//    char    upnearetime[6];                       /* 상한가근접이탈시간(6)           */
//    char    dnneargubun[1];                       /* 하한근접유지구분(1)             */
//    char    dnnearstime[6];                       /* 하한가근접진입시간(6)           */
//    char    dnnearetime[6];                       /* 하한가근접이탈시간(6)           */
//    char    mdvolume_c[8];                        /* 매도체결수량(8)                 */
//    char    msvolume_c[8];                        /* 매수체결수량(8)                 */
//    char    mdvolume_h[8];                        /* 매도체결수량(8)                 */
//    char    msvolume_h[8];                        /* 매수체결수량(8)                 */
//    char    msvolume_d[8];                        /* 대량매수체결수량(8)             */
//    char    mdvolume_d[8];                        /* 대량매도체결수량(8)             */
//    char    mschecnt_d[8];                        /* 대량매수체결건수(8)             */
//    char    mdchecnt_d[8];                        /* 대량매도체결건수(8)             */
//    char    status[2];                            /* 장구분(2)                       */
//    char    yeuplmt[8];                           /* 익일예상상한가(8)               */
//    char    yednlmt[8];                           /* 익일예상하한가(8)               */
//}KB_s0001_OutRec1;

typedef struct _KB_s0001_OutRec1 {
    char    shcode[6];                            /* 종목코드(6)                     */
    char    recprice[8];                          /* 기준가(8)                       */
    char    yeprice[8];                           /* 예상체결가격(8)                 */
    char    yevolume[8];                          /* 예상체결수량(8)                 */
    char    hname[40];                            /* 종목명(40)                      */
    char    price[8];                             /* 현재가(8)                       */
    char    sign[1];                              /* 현재가전일대비구분(1)           */
    char    change[8];                            /* 현재가전일대비(8)               */
    char    rate[7];                              /* 현재가전일대비등락율(7)         */
    char    volume[12];                           /* 거래량(12)                      */
    char    value[15];                            /* 거래대금(15)                    */
    char    offerho1[8];                          /* 매도호가1(8)                    */
    char    offerho2[8];                          /* 매도호가2(8)                    */
    char    offerho3[8];                          /* 매도호가3(8)                    */
    char    offerho4[8];                          /* 매도호가4(8)                    */
    char    offerho5[8];                          /* 매도호가5(8)                    */
    char    offerrem1[8];                         /* 매도호가수량1(8)                */
    char    offerrem2[8];                         /* 매도호가수량2(8)                */
    char    offerrem3[8];                         /* 매도호가수량3(8)                */
    char    offerrem4[8];                         /* 매도호가수량4(8)                */
    char    offerrem5[8];                         /* 매도호가수량5(8)                */
    char    bidho1[8];                            /* 매수호가1(8)                    */
    char    bidho2[8];                            /* 매수호가2(8)                    */
    char    bidho3[8];                            /* 매수호가3(8)                    */
    char    bidho4[8];                            /* 매수호가4(8)                    */
    char    bidho5[8];                            /* 매수호가5(8)                    */
    char    bidrem1[8];                           /* 매수호가수량1(8)                */
    char    bidrem2[8];                           /* 매수호가수량2(8)                */
    char    bidrem3[8];                           /* 매수호가수량3(8)                */
    char    bidrem4[8];                           /* 매수호가수량4(8)                */
    char    bidrem5[8];                           /* 매수호가수량5(8)                */
    char    totofferrem[8];                       /* 매도호가총수량(8)               */
    char    totbidrem[8];                         /* 매수호가총수량(8)               */
    char    tmofferrem[8];                        /* 시간외매도잔량(8)               */
    char    tmbidrem[8];                          /* 시간외매수잔량(8)               */
    char    uplmtprice[8];                        /* 상한가(8)                       */
    char    dnlmtprice[8];                        /* 하한가(8)                       */
    char    preychange[8];                        /* 예상체결가직전현재가대비(8)     */
    char    high[8];                              /* 고가(8)                         */
    char    low[8];                               /* 저가(8)                         */
    char    jnilvolume[12];                       /* 전일거래량(12)                  */
    char    listing[12];                          /* 상장주식수(12)                  */
} KB_s0001_OutRec1;

/*************************************************************************************/
class CKB_s0001
{
public: //Variable
    CString		m_szshcode;							/* 종목코드(6)                     */
    CString		m_szrecprice;						/* 기준가(8)                       */
    CString		m_szyeprice;						/* 예상체결가격(8)                 */
    CString		m_szyevolume;						/* 예상체결수량(8)                 */
    CString		m_szhname;							/* 종목명(40)                      */
    CString		m_szprice;							/* 현재가(8)                       */
    CString		m_szsign;							/* 현재가전일대비구분(1)           */
    CString		m_szchange;							/* 현재가전일대비(8)               */
    CString		m_szrate;							/* 현재가전일대비등락율(7)         */
    CString		m_szvolume;							/* 거래량(12)                      */
    CString		m_szvalue;							/* 거래대금(15)                    */
    CString		m_szofferho1;						/* 매도호가1(8)                    */
    CString		m_szofferho2;						/* 매도호가2(8)                    */
    CString		m_szofferho3;						/* 매도호가3(8)                    */
    CString		m_szofferho4;						/* 매도호가4(8)                    */
    CString		m_szofferho5;						/* 매도호가5(8)                    */
    CString		m_szofferrem1;						/* 매도호가수량1(8)                */
    CString		m_szofferrem2;						/* 매도호가수량2(8)                */
    CString		m_szofferrem3;						/* 매도호가수량3(8)                */
    CString		m_szofferrem4;						/* 매도호가수량4(8)                */
    CString		m_szofferrem5;						/* 매도호가수량5(8)                */
    CString		m_szbidho1;							/* 매수호가1(8)                    */
    CString		m_szbidho2;							/* 매수호가2(8)                    */
    CString		m_szbidho3;							/* 매수호가3(8)                    */
    CString		m_szbidho4;							/* 매수호가4(8)                    */
    CString		m_szbidho5;							/* 매수호가5(8)                    */
    CString		m_szbidrem1;						/* 매수호가수량1(8)                */
    CString		m_szbidrem2;						/* 매수호가수량2(8)                */
    CString		m_szbidrem3;						/* 매수호가수량3(8)                */
    CString		m_szbidrem4;						/* 매수호가수량4(8)                */
    CString		m_szbidrem5;						/* 매수호가수량5(8)                */
    CString		m_sztotofferrem;					/* 매도호가총수량(8)               */
    CString		m_sztotbidrem;						/* 매수호가총수량(8)               */
    CString		m_sztmofferrem;						/* 시간외매도잔량(8)               */
    CString		m_sztmbidrem;						/* 시간외매수잔량(8)               */
    CString		m_szuplmtprice;						/* 상한가(8)                       */
    CString		m_szdnlmtprice;						/* 하한가(8)                       */
    CString		m_szpreychange;						/* 예상체결가직전현재가대비(8)     */
    CString		m_szhigh;							/* 고가(8)                         */
    CString		m_szlow;							/* 저가(8)                         */
    CString		m_szjnilvolume;						/* 전일거래량(12)                  */
    CString		m_szlisting;						/* 상장주식수(12)                  */

public:
	CKB_s0001()
	{
	}

	virtual void  Convert(KB_s0001_OutRec1* pInput)
	{
		CString sTemp1 ;
		char sTmp[50+1];

		DATA_MEMCOPY(sTmp, pInput, shcode);			/* 종목코드(6)                     */
		DATA_MEMCOPY(sTmp, pInput, recprice);		/* 기준가(8)                       */
		DATA_MEMCOPY(sTmp, pInput, yeprice);		/* 예상체결가격(8)                 */
		DATA_MEMCOPY(sTmp, pInput, yevolume);		/* 예상체결수량(8)                 */
		DATA_MEMCOPY(sTmp, pInput, hname);			/* 종목명(40)                      */
		DATA_MEMCOPY(sTmp, pInput, price);			/* 현재가(8)                       */
		DATA_MEMCOPY(sTmp, pInput, sign);			/* 현재가전일대비구분(1)           */
		DATA_MEMCOPY(sTmp, pInput, change);			/* 현재가전일대비(8)               */
		DATA_MEMCOPY(sTmp, pInput, rate);			/* 현재가전일대비등락율(7)         */
		DATA_MEMCOPY(sTmp, pInput, volume);			/* 거래량(12)                      */
		DATA_MEMCOPY(sTmp, pInput, value);			/* 거래대금(15)                    */
		DATA_MEMCOPY(sTmp, pInput, offerho1);		/* 매도호가1(8)                    */
		DATA_MEMCOPY(sTmp, pInput, offerho2);		/* 매도호가2(8)                    */
		DATA_MEMCOPY(sTmp, pInput, offerho3);		/* 매도호가3(8)                    */
		DATA_MEMCOPY(sTmp, pInput, offerho4);		/* 매도호가4(8)                    */
		DATA_MEMCOPY(sTmp, pInput, offerho5);		/* 매도호가5(8)                    */
		DATA_MEMCOPY(sTmp, pInput, offerrem1);		/* 매도호가수량1(8)                */
		DATA_MEMCOPY(sTmp, pInput, offerrem2);		/* 매도호가수량2(8)                */
		DATA_MEMCOPY(sTmp, pInput, offerrem3);		/* 매도호가수량3(8)                */
		DATA_MEMCOPY(sTmp, pInput, offerrem4);		/* 매도호가수량4(8)                */
		DATA_MEMCOPY(sTmp, pInput, offerrem5);		/* 매도호가수량5(8)                */
		DATA_MEMCOPY(sTmp, pInput, bidho1);			/* 매수호가1(8)                    */
		DATA_MEMCOPY(sTmp, pInput, bidho2);			/* 매수호가2(8)                    */
		DATA_MEMCOPY(sTmp, pInput, bidho3);			/* 매수호가3(8)                    */
		DATA_MEMCOPY(sTmp, pInput, bidho4);			/* 매수호가4(8)                    */
		DATA_MEMCOPY(sTmp, pInput, bidho5);			/* 매수호가5(8)                    */
		DATA_MEMCOPY(sTmp, pInput, bidrem1);		/* 매수호가수량1(8)                */
		DATA_MEMCOPY(sTmp, pInput, bidrem2);		/* 매수호가수량2(8)                */
		DATA_MEMCOPY(sTmp, pInput, bidrem3);		/* 매수호가수량3(8)                */
		DATA_MEMCOPY(sTmp, pInput, bidrem4);		/* 매수호가수량4(8)                */
		DATA_MEMCOPY(sTmp, pInput, bidrem5);		/* 매수호가수량5(8)                */
		DATA_MEMCOPY(sTmp, pInput, totofferrem);	/* 매도호가총수량(8)               */
		DATA_MEMCOPY(sTmp, pInput, totbidrem);		/* 매수호가총수량(8)               */
		DATA_MEMCOPY(sTmp, pInput, tmofferrem);		/* 시간외매도잔량(8)               */
		DATA_MEMCOPY(sTmp, pInput, tmbidrem);		/* 시간외매수잔량(8)               */
		DATA_MEMCOPY(sTmp, pInput, uplmtprice);		/* 상한가(8)                       */
		DATA_MEMCOPY(sTmp, pInput, dnlmtprice);		/* 하한가(8)                       */
		DATA_MEMCOPY(sTmp, pInput, preychange);		/* 예상체결가직전현재가대비(8)     */
		DATA_MEMCOPY(sTmp, pInput, high);			/* 고가(8)                         */
		DATA_MEMCOPY(sTmp, pInput, low);			/* 저가(8)                         */
		DATA_MEMCOPY(sTmp, pInput, jnilvolume);		/* 전일거래량(12)                  */
		DATA_MEMCOPY(sTmp, pInput, listing);		/* 상장주식수(12)                  */
	}
};

#endif

