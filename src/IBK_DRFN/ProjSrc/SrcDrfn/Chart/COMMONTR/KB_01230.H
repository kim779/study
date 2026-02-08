#if !defined(AFX_KB_01230__H_B8898524_2F02_4517_AC5A_E782C176A44F_INCLUDED_)
#define AFX_KB_01230__H_B8898524_2F02_4517_AC5A_E782C176A44F_INCLUDED_

#include "Data_Common.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//// 주식호가5단조회(01230) ( 01230,block,attr,headtype=A )
// 주식호가조회(c1002) ( c1002,block,attr,headtype=A )
#pragma pack( push, 1 )

#define NAME_01230     "c1002"

// 기본입력
typedef struct _k01230In1
{
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] 단축코드
} k01230In1, *LPk01230In1;
#define NAME_k01230In1     "k01230In1"

// 출력 List
typedef struct _k01230Out1
{
//    char    hname               [  20];    char    _hname               ;    // [string,   20] 종목명
//    char    price               [   8];    char    _price               ;    // [long  ,    8] 현재가
//    char    sign                [   1];    char    _sign                ;    // [string,    1] 대비구분
//    char    change              [   8];    char    _change              ;    // [long  ,    8] 대비
//    char    rate                [   7];    char    _rate                ;    // [float ,  7.2] 등락율
//    char    volume              [  12];    char    _volume              ;    // [float ,   12] 거래량
//    char    volrate             [   7];    char    _volrate             ;    // [float ,  7.2] 전일거래량대비
//    char    offerho             [   8];    char    _offerho             ;    // [long  ,    8] 우선매도호가
//    char    bidho               [   8];    char    _bidho               ;    // [long  ,    8] 우선매수호가
//    char    fholdvol            [  12];    char    _fholdvol            ;    // [float ,   12] 외국인보유수량
//    char    exhratio            [   6];    char    _exhratio            ;    // [float ,  6.2] 외국인보유비중
//    char    hotime              [   6];    char    _hotime              ;    // [string,    6] 호가수신시간
//    char    offerho1            [   8];    char    _offerho1            ;    // [long  ,    8] 매도호가1
//    char    offerrem1           [   8];    char    _offerrem1           ;    // [long  ,    8] 매도호가수량1
//    char    preoffercha1        [   8];    char    _preoffercha1        ;    // [long  ,    8] 매도직전대비수량1
//    char    offerho2            [   8];    char    _offerho2            ;    // [long  ,    8] 매도호가2
//    char    offerrem2           [   8];    char    _offerrem2           ;    // [long  ,    8] 매도호가수량2
//    char    preoffercha2        [   8];    char    _preoffercha2        ;    // [long  ,    8] 매도직전대비수량2
//    char    offerho3            [   8];    char    _offerho3            ;    // [long  ,    8] 매도호가3
//    char    offerrem3           [   8];    char    _offerrem3           ;    // [long  ,    8] 매도호가수량3
//    char    preoffercha3        [   8];    char    _preoffercha3        ;    // [long  ,    8] 매도직전대비수량3
//    char    offerho4            [   8];    char    _offerho4            ;    // [long  ,    8] 매도호가4
//    char    offerrem4           [   8];    char    _offerrem4           ;    // [long  ,    8] 매도호가수량4
//    char    preoffercha4        [   8];    char    _preoffercha4        ;    // [long  ,    8] 매도직전대비수량4
//    char    offerho5            [   8];    char    _offerho5            ;    // [long  ,    8] 매도호가5
//    char    offerrem5           [   8];    char    _offerrem5           ;    // [long  ,    8] 매도호가수량5
//    char    preoffercha5        [   8];    char    _preoffercha5        ;    // [long  ,    8] 매도직전대비수량5
//    char    bidho1              [   8];    char    _bidho1              ;    // [long  ,    8] 매수호가1
//    char    bidrem1             [   8];    char    _bidrem1             ;    // [long  ,    8] 매수호가수량1
//    char    prebidcha1          [   8];    char    _prebidcha1          ;    // [long  ,    8] 매수직전대비수량1
//    char    bidho2              [   8];    char    _bidho2              ;    // [long  ,    8] 매수호가2
//    char    bidrem2             [   8];    char    _bidrem2             ;    // [long  ,    8] 매수호가수량2
//    char    prebidcha2          [   8];    char    _prebidcha2          ;    // [long  ,    8] 매수직전대비수량2
//    char    bidho3              [   8];    char    _bidho3              ;    // [long  ,    8] 매수호가3
//    char    bidrem3             [   8];    char    _bidrem3             ;    // [long  ,    8] 매수호가수량3
//    char    prebidcha3          [   8];    char    _prebidcha3          ;    // [long  ,    8] 매수직전대비수량3
//    char    bidho4              [   8];    char    _bidho4              ;    // [long  ,    8] 매수호가4
//    char    bidrem4             [   8];    char    _bidrem4             ;    // [long  ,    8] 매수호가수량4
//    char    prebidcha4          [   8];    char    _prebidcha4          ;    // [long  ,    8] 매수직전대비수량4
//    char    bidho5              [   8];    char    _bidho5              ;    // [long  ,    8] 매수호가5
//    char    bidrem5             [   8];    char    _bidrem5             ;    // [long  ,    8] 매수호가수량5
//    char    prebidcha5          [   8];    char    _prebidcha5          ;    // [long  ,    8] 매수직전대비수량5
//    char    totofferrem         [   8];    char    _totofferrem         ;    // [long  ,    8] 매도호가총수량
//    char    preoffercha         [   8];    char    _preoffercha         ;    // [long  ,    8] 매도호가총수량직전대비
//    char    totbidrem           [   8];    char    _totbidrem           ;    // [long  ,    8] 매수호가총수량
//    char    prebidcha           [   8];    char    _prebidcha           ;    // [long  ,    8] 매수호가총수량직전대비
//    char    yeprice             [   8];    char    _yeprice             ;    // [long  ,    8] 예상체결가격
//    char    yevolume            [   8];    char    _yevolume            ;    // [long  ,    8] 예상체결수량
//    char    jnilysign           [   1];    char    _jnilysign           ;    // [string,    1] 예상체결가전일종가대비구분
//    char    jnilychange         [   8];    char    _jnilychange         ;    // [long  ,    8] 예상체결가전일종가대비
//    char    yerate              [   6];    char    _yerate              ;    // [float ,  6.2] 예상등락율
//    char    tmofferrem          [   8];    char    _tmofferrem          ;    // [long  ,    8] 시간외매도잔량
//    char    tmbidrem            [   8];    char    _tmbidrem            ;    // [long  ,    8] 시간외매수잔량
//    char    ho_status           [   1];    char    _ho_status           ;    // [long  ,    1] 장구분
//    char    recprice            [   8];    char    _recprice            ;    // [long  ,    8] 기준가
//    char    uplmtprice          [   8];    char    _uplmtprice          ;    // [long  ,    8] 상한가
//    char    dnlmtprice          [   8];    char    _dnlmtprice          ;    // [long  ,    8] 하한가
//    char    open                [   8];    char    _open                ;    // [long  ,    8] 시가
//    char    high                [   8];    char    _high                ;    // [long  ,    8] 고가
//    char    low                 [   8];    char    _low                 ;    // [long  ,    8] 저가
//    char    trade_cost          [   8];    char    _trade_cost          ;    // [long  ,    8] 거래비용
    char    hname               [  20];    char    _hname               ;    // [string,   20] 종목명
    char    price               [   8];    char    _price               ;    // [long  ,    8] 현재가
    char    sign                [   1];    char    _sign                ;    // [string,    1] 대비구분
    char    change              [   8];    char    _change              ;    // [long  ,    8] 대비
    char    rate                [   7];    char    _rate                ;    // [float ,  7.2] 등락율
    char    volume              [  12];    char    _volume              ;    // [float ,   12] 거래량
    char    volrate             [   7];    char    _volrate             ;    // [float ,  7.2] 전일거래량대비
    char    offerho             [   8];    char    _offerho             ;    // [long  ,    8] 우선매도호가
    char    bidho               [   8];    char    _bidho               ;    // [long  ,    8] 우선매수호가
    char    fholdvol            [  12];    char    _fholdvol            ;    // [float ,   12] 외국인보유수량
    char    exhratio            [   6];    char    _exhratio            ;    // [float ,  6.2] 외국인보유비중
    char    hotime              [   6];    char    _hotime              ;    // [string,    6] 호가수신시간
    char    offerho1            [   8];    char    _offerho1            ;    // [long  ,    8] 매도호가1
    char    offerrem1           [   8];    char    _offerrem1           ;    // [long  ,    8] 매도호가수량1
    char    preoffercha1        [   8];    char    _preoffercha1        ;    // [long  ,    8] 매도직전대비수량1
    char    offerho2            [   8];    char    _offerho2            ;    // [long  ,    8] 매도호가2
    char    offerrem2           [   8];    char    _offerrem2           ;    // [long  ,    8] 매도호가수량2
    char    preoffercha2        [   8];    char    _preoffercha2        ;    // [long  ,    8] 매도직전대비수량2
    char    offerho3            [   8];    char    _offerho3            ;    // [long  ,    8] 매도호가3
    char    offerrem3           [   8];    char    _offerrem3           ;    // [long  ,    8] 매도호가수량3
    char    preoffercha3        [   8];    char    _preoffercha3        ;    // [long  ,    8] 매도직전대비수량3
    char    offerho4            [   8];    char    _offerho4            ;    // [long  ,    8] 매도호가4
    char    offerrem4           [   8];    char    _offerrem4           ;    // [long  ,    8] 매도호가수량4
    char    preoffercha4        [   8];    char    _preoffercha4        ;    // [long  ,    8] 매도직전대비수량4
    char    offerho5            [   8];    char    _offerho5            ;    // [long  ,    8] 매도호가5
    char    offerrem5           [   8];    char    _offerrem5           ;    // [long  ,    8] 매도호가수량5
    char    preoffercha5        [   8];    char    _preoffercha5        ;    // [long  ,    8] 매도직전대비수량5
    char    offerho6            [   8];    char    _offerho6            ;    // [long  ,    8] 매도호가6
    char    offerrem6           [   8];    char    _offerrem6           ;    // [long  ,    8] 매도호가수량6
    char    preoffercha6        [   8];    char    _preoffercha6        ;    // [long  ,    8] 매도직전대비수량6
    char    offerho7            [   8];    char    _offerho7            ;    // [long  ,    8] 매도호가7
    char    offerrem7           [   8];    char    _offerrem7           ;    // [long  ,    8] 매도호가수량7
    char    preoffercha7        [   8];    char    _preoffercha7        ;    // [long  ,    8] 매도직전대비수량7
    char    offerho8            [   8];    char    _offerho8            ;    // [long  ,    8] 매도호가8
    char    offerrem8           [   8];    char    _offerrem8           ;    // [long  ,    8] 매도호가수량8
    char    preoffercha8        [   8];    char    _preoffercha8        ;    // [long  ,    8] 매도직전대비수량8
    char    offerho9            [   8];    char    _offerho9            ;    // [long  ,    8] 매도호가9
    char    offerrem9           [   8];    char    _offerrem9           ;    // [long  ,    8] 매도호가수량9
    char    preoffercha9        [   8];    char    _preoffercha9        ;    // [long  ,    8] 매도직전대비수량9
    char    offerho10           [   8];    char    _offerho10           ;    // [long  ,    8] 매도호가10
    char    offerrem10          [   8];    char    _offerrem10          ;    // [long  ,    8] 매도호가수량10
    char    preoffercha10       [   8];    char    _preoffercha10       ;    // [long  ,    8] 매도직전대비수량10
    char    bidho1              [   8];    char    _bidho1              ;    // [long  ,    8] 매수호가1
    char    bidrem1             [   8];    char    _bidrem1             ;    // [long  ,    8] 매수호가수량1
    char    prebidcha1          [   8];    char    _prebidcha1          ;    // [long  ,    8] 매수직전대비수량1
    char    bidho2              [   8];    char    _bidho2              ;    // [long  ,    8] 매수호가2
    char    bidrem2             [   8];    char    _bidrem2             ;    // [long  ,    8] 매수호가수량2
    char    prebidcha2          [   8];    char    _prebidcha2          ;    // [long  ,    8] 매수직전대비수량2
    char    bidho3              [   8];    char    _bidho3              ;    // [long  ,    8] 매수호가3
    char    bidrem3             [   8];    char    _bidrem3             ;    // [long  ,    8] 매수호가수량3
    char    prebidcha3          [   8];    char    _prebidcha3          ;    // [long  ,    8] 매수직전대비수량3
    char    bidho4              [   8];    char    _bidho4              ;    // [long  ,    8] 매수호가4
    char    bidrem4             [   8];    char    _bidrem4             ;    // [long  ,    8] 매수호가수량4
    char    prebidcha4          [   8];    char    _prebidcha4          ;    // [long  ,    8] 매수직전대비수량4
    char    bidho5              [   8];    char    _bidho5              ;    // [long  ,    8] 매수호가5
    char    bidrem5             [   8];    char    _bidrem5             ;    // [long  ,    8] 매수호가수량5
    char    prebidcha5          [   8];    char    _prebidcha5          ;    // [long  ,    8] 매수직전대비수량5
    char    bidho6              [   8];    char    _bidho6              ;    // [long  ,    8] 매수호가6
    char    bidrem6             [   8];    char    _bidrem6             ;    // [long  ,    8] 매수호가수량6
    char    prebidcha6          [   8];    char    _prebidcha6          ;    // [long  ,    8] 매수직전대비수량6
    char    bidho7              [   8];    char    _bidho7              ;    // [long  ,    8] 매수호가7
    char    bidrem7             [   8];    char    _bidrem7             ;    // [long  ,    8] 매수호가수량7
    char    prebidcha7          [   8];    char    _prebidcha7          ;    // [long  ,    8] 매수직전대비수량7
    char    bidho8              [   8];    char    _bidho8              ;    // [long  ,    8] 매수호가8
    char    bidrem8             [   8];    char    _bidrem8             ;    // [long  ,    8] 매수호가수량8
    char    prebidcha8          [   8];    char    _prebidcha8          ;    // [long  ,    8] 매수직전대비수량8
    char    bidho9              [   8];    char    _bidho9              ;    // [long  ,    8] 매수호가9
    char    bidrem9             [   8];    char    _bidrem9             ;    // [long  ,    8] 매수호가수량9
    char    prebidcha9          [   8];    char    _prebidcha9          ;    // [long  ,    8] 매수직전대비수량9
    char    bidho10             [   8];    char    _bidho10             ;    // [long  ,    8] 매수호가10
    char    bidrem10            [   8];    char    _bidrem10            ;    // [long  ,    8] 매수호가수량10
    char    prebidcha10         [   8];    char    _prebidcha10         ;    // [long  ,    8] 매수직전대비수량10
    char    totofferrem         [   8];    char    _totofferrem         ;    // [long  ,    8] 매도호가총수량
    char    preoffercha         [   8];    char    _preoffercha         ;    // [long  ,    8] 매도호가총수량직전대비
    char    totbidrem           [   8];    char    _totbidrem           ;    // [long  ,    8] 매수호가총수량
    char    prebidcha           [   8];    char    _prebidcha           ;    // [long  ,    8] 매수호가총수량직전대비
    char    yeprice             [   8];    char    _yeprice             ;    // [long  ,    8] 예상체결가격
    char    yevolume            [   8];    char    _yevolume            ;    // [long  ,    8] 예상체결수량
    char    jnilysign           [   1];    char    _jnilysign           ;    // [string,    1] 예상체결가전일종가대비구분
    char    jnilychange         [   8];    char    _jnilychange         ;    // [long  ,    8] 예상체결가전일종가대비
    char    yerate              [   6];    char    _yerate              ;    // [float ,  6.2] 예상등락율
    char    tmofferrem          [   8];    char    _tmofferrem          ;    // [long  ,    8] 시간외매도잔량
    char    tmbidrem            [   8];    char    _tmbidrem            ;    // [long  ,    8] 시간외매수잔량
    char    ho_status           [   1];    char    _ho_status           ;    // [long  ,    1] 장구분
    char    jnilclose           [   8];    char    _jnilclose           ;    // [long  ,    8] 전일종가
    char    recprice            [   8];    char    _recprice            ;    // [long  ,    8] 기준가
    char    uplmtprice          [   8];    char    _uplmtprice          ;    // [long  ,    8] 상한가
    char    dnlmtprice          [   8];    char    _dnlmtprice          ;    // [long  ,    8] 하한가
    char    open                [   8];    char    _open                ;    // [long  ,    8] 시가
    char    high                [   8];    char    _high                ;    // [long  ,    8] 고가
    char    low                 [   8];    char    _low                 ;    // [long  ,    8] 저가
    char    trade_cost          [   8];    char    _trade_cost          ;    // [long  ,    8] 거래비용
    char    pivot               [   8];    char    _pivot               ;    // [long  ,    8] 피봇
    char    recline             [   8];    char    _recline             ;    // [long  ,    8] 기준선
    char    resist1             [   8];    char    _resist1             ;    // [long  ,    8] 1차저항
    char    resist2             [   8];    char    _resist2             ;    // [long  ,    8] 2차저항
    char    support1            [   8];    char    _support1            ;    // [long  ,    8] 1차지지
    char    support2            [   8];    char    _support2            ;    // [long  ,    8] 2차지지
    char    resist_d            [   8];    char    _resist_d            ;    // [long  ,    8] D저항
    char    support_d           [   8];    char    _support_d           ;    // [long  ,    8] D지지
    char    parprice            [   8];    char    _parprice            ;    // [long  ,    8] 액면가
    char    subprice            [   8];    char    _subprice            ;    // [long  ,    8] 대용가
    char    sigatvalue          [  12];    char    _sigatvalue          ;    // [float ,   12] 시가총액
    char    capital             [  12];    char    _capital             ;    // [float ,   12] 자본금
    char    listing             [  12];    char    _listing             ;    // [float ,   12] 상장주식수
    char    perx                [   6];    char    _perx                ;    // [float ,  6.2] PER (주가수익비율 - 전기)
    char    pbrx                [   6];    char    _pbrx                ;    // [float ,  6.2] PBR (주가순자산비율)
    char    high52w             [   8];    char    _high52w             ;    // [long  ,    8] 250일최고가
    char    high52w_rate        [   7];    char    _high52w_rate        ;    // [float ,  7.2] 250일최고가등락율
    char    high52wdate         [   8];    char    _high52wdate         ;    // [string,    8] 250일최고가일자
    char    low52w              [   8];    char    _low52w              ;    // [long  ,    8] 250일최저가
    char    low52w_rate         [   7];    char    _low52w_rate         ;    // [float ,  7.2] 250일최저가등락율
    char    low52wdate          [   8];    char    _low52wdate          ;    // [string,    8] 250일최저가일자
    char    value               [  15];    char    _value               ;    // [float ,   15] 거래대금
    char    volro               [   9];    char    _volro               ;    // [float ,  9.2] 거래량회전율
    char    jnilvolume          [  12];    char    _jnilvolume          ;    // [float ,   12] 전일거래량
} k01230Out1, *LPk01230Out1;
#define NAME_k01230Out1     "k01230Out1"

typedef struct _k01230Out1_Ex
{
	char    price               [   8];
	char    uplmtprice          [   8];
	char    dnlmtprice          [   8];
	char    offerho3            [   8];
	char    offerho2            [   8];
	char    offerho1            [   8];
	char    bidho1              [   8];
	char    bidho2              [   8];
	char    bidho3              [   8];
} k01230Out1_Ex, *LPk01230Out1_Ex;

typedef struct _k01230Out2
{
    char    recprice            [   8];    // [long  ,    8] 기준가
    char    yeprice             [   8];    // [long  ,    8] 예상체결가격
    char    yevolume            [   8];    // [long  ,    8] 예상체결수량
    char    hname               [  20];    // [string,   20] 종목명
    char    price               [   8];    // [long  ,    8] 현재가
    char    sign                [   1];    // [string,    1] 대비구분
    char    change              [   8];    // [long  ,    8] 대비
    char    rate                [   7];    // [float ,  7.2] 등락율
    char    volume              [  12];    // [float ,   12] 거래량
    char    trade_cost          [   8];    // [long  ,    8] 거래비용
    char    offerho1            [   8];    // [long  ,    8] 매도호가1
    char    offerho2            [   8];    // [long  ,    8] 매도호가2
    char    offerho3            [   8];    // [long  ,    8] 매도호가3
    char    offerho4            [   8];    // [long  ,    8] 매도호가4
    char    offerho5            [   8];    // [long  ,    8] 매도호가5
    char    offerrem1           [   8];    // [long  ,    8] 매도호가수량1
    char    offerrem2           [   8];    // [long  ,    8] 매도호가수량2
    char    offerrem3           [   8];    // [long  ,    8] 매도호가수량3
    char    offerrem4           [   8];    // [long  ,    8] 매도호가수량4
    char    offerrem5           [   8];    // [long  ,    8] 매도호가수량5
    char    bidho1              [   8];    // [long  ,    8] 매수호가1
    char    bidho2              [   8];    // [long  ,    8] 매수호가2
    char    bidho3              [   8];    // [long  ,    8] 매수호가3
    char    bidho4              [   8];    // [long  ,    8] 매수호가4
    char    bidho5              [   8];    // [long  ,    8] 매수호가5
    char    bidrem1             [   8];    // [long  ,    8] 매수호가수량1
    char    bidrem2             [   8];    // [long  ,    8] 매수호가수량2
    char    bidrem3             [   8];    // [long  ,    8] 매수호가수량3
    char    bidrem4             [   8];    // [long  ,    8] 매수호가수량4
    char    bidrem5             [   8];    // [long  ,    8] 매수호가수량5
    char    totofferrem         [   8];    // [long  ,    8] 매도호가총수량
    char    totbidrem           [   8];    // [long  ,    8] 매수호가총수량
    char    tmofferrem          [   8];    // [long  ,    8] 시간외매도잔량
    char    tmbidrem            [   8];    // [long  ,    8] 시간외매수잔량
    char    uplmtprice          [   8];    // [long  ,    8] 상한가
    char    dnlmtprice          [   8];    // [long  ,    8] 하한가
    char    yerate              [   6];    // [float ,  6.2] 예상등락율
    char    high                [   8];    // [long  ,    8] 고가
    char    low                 [   8];    // [long  ,    8] 저가
    char    volrate             [   7];    // [float ,  7.2] 전일거래량대비
	char	jnilysign			[   1];	   // [string,    1] 예상체결가전일종가대비구분
} k01230Out2, *LPk01230Out2;

class CKB_01230
{
public: //Variable
//    CString		m_szshcode;							/* 종목코드(6)                     */
    CString		m_szrecprice;						/* 기준가(8)                       */
    CString		m_szyeprice;						/* 예상체결가격(8)                 */
    CString		m_szyevolume;						/* 예상체결수량(8)                 */
    CString		m_szhname;							/* 종목명(40)                      */
    CString		m_szprice;							/* 현재가(8)                       */
    CString		m_szsign;							/* 현재가전일대비구분(1)           */
    CString		m_szchange;							/* 현재가전일대비(8)               */
    CString		m_szrate;							/* 현재가전일대비등락율(7)         */
    CString		m_szvolume;							/* 거래량(12)                      */
    CString		m_sztrade_cost;						/* 거래대금(15)                    */
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
    CString		m_szyerate;							/* 예상체결가직전현재가대비(8)     */
    CString		m_szhigh;							/* 고가(8)                         */
    CString		m_szlow;							/* 저가(8)                         */
    CString		m_szvolrate;						/* 전일거래량(12)                  */
	CString		m_szjnilysign;						/* 예상체결가전일종가대비구분(1)   */
//    CString		m_szlisting;						/* 상장주식수(12)                  */

public:
	CKB_01230()
	{
	}

	virtual void  Convert(LPk01230Out2 pInput)
	{
		CString sTemp1 ;
		char sTmp[50+1];

//		DATA_MEMCOPY(sTmp, pInput, shcode);			/* 종목명(20)                      */
		DATA_MEMCOPY(sTmp, pInput, recprice);		/* 기준가(8)                       */
		DATA_MEMCOPY(sTmp, pInput, yeprice);		/* 예상체결가격(8)                 */
		DATA_MEMCOPY(sTmp, pInput, yevolume);		/* 예상체결수량(8)                 */
		DATA_MEMCOPY(sTmp, pInput, hname);			/* 종목명(40)                      */
		DATA_MEMCOPY(sTmp, pInput, price);			/* 현재가(8)                       */
		DATA_MEMCOPY(sTmp, pInput, sign);			/* 현재가전일대비구분(1)           */
		DATA_MEMCOPY(sTmp, pInput, change);			/* 현재가전일대비(8)               */
		DATA_MEMCOPY(sTmp, pInput, rate);			/* 현재가전일대비등락율(7)         */
		DATA_MEMCOPY(sTmp, pInput, volume);			/* 거래량(12)                      */
		DATA_MEMCOPY(sTmp, pInput, trade_cost);		/* 거래대금(15)                    */
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
		DATA_MEMCOPY(sTmp, pInput, yerate);			/* 예상체결가직전현재가대비(8)     */
		DATA_MEMCOPY(sTmp, pInput, high);			/* 고가(8)                         */
		DATA_MEMCOPY(sTmp, pInput, low);			/* 저가(8)                         */
		DATA_MEMCOPY(sTmp, pInput, volrate);		/* 전일거래량(12)                  */
		DATA_MEMCOPY(sTmp, pInput, jnilysign);		/* 예상체결가전일종가대비구분(1)   */
//		DATA_MEMCOPY(sTmp, pInput, listing);		/* 상장주식수(12)                  */
	}
};

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // AFX_KB_01230__H_B8898524_2F02_4517_AC5A_E782C176A44F_INCLUDED_
