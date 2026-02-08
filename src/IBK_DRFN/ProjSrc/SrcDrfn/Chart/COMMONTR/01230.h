///////////////////////////////////////////////////////////////////////////////////////////////////
// 주식호가5단조회(01230) ( 01230,block,attr,headtype=A )
#pragma pack( push, 1 )

#define NAME_01230     "01230"

// 기본입력
typedef struct _k01230In1
{
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] 단축코드
} k01230In1, *LPk01230In1;
#define NAME_k01230In1     "k01230In1"

// 출력 List
typedef struct _k01230Out1
{
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
    char    recprice            [   8];    char    _recprice            ;    // [long  ,    8] 기준가
    char    uplmtprice          [   8];    char    _uplmtprice          ;    // [long  ,    8] 상한가
    char    dnlmtprice          [   8];    char    _dnlmtprice          ;    // [long  ,    8] 하한가
    char    open                [   8];    char    _open                ;    // [long  ,    8] 시가
    char    high                [   8];    char    _high                ;    // [long  ,    8] 고가
    char    low                 [   8];    char    _low                 ;    // [long  ,    8] 저가
    char    trade_cost          [   8];    char    _trade_cost          ;    // [long  ,    8] 거래비용
    char    volro               [   9];    char    _volro               ;    // [float ,  9.2] 거래량회전율
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

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
