///////////////////////////////////////////////////////////////////////////////////////////////////
// 선물옵션호가(06110) ( 06110,block,attr,headtype=A )
#pragma pack( push, 1 )

#define NAME_06110     "06110"

// 기본입력
typedef struct _k06110In1
{
    char    futcode             [   8];    char    _futcode             ;    // [string,    8] 종목코드
} k06110In1, *LPk06110In1;
#define NAME_k06110In1     "k06110In1"

// 출력
typedef struct _k06110Out1
{
    char    futcode             [   8];    char    _futcode             ;    // [string,    8] 종목코드
    char    chetime             [   6];    char    _chetime             ;    // [string,    6] 체결시간
    char    price               [   6];    char    _price               ;    // [float ,  6.2] 현재가
    char    sign                [   1];    char    _sign                ;    // [string,    1] 전일대비구분
    char    change              [   6];    char    _change              ;    // [float ,  6.2] 전일대비　
    char    drate               [   6];    char    _drate               ;    // [float ,  6.2] 등락률
    char    jnilvolume          [   8];    char    _jnilvolume          ;    // [float ,    8] 전일거래량
    char    volume              [   8];    char    _volume              ;    // [float ,    8] 누적거래량
    char    volrate             [   6];    char    _volrate             ;    // [float ,  6.2] 전일거래량대비율
    char    jnilvalue           [  15];    char    _jnilvalue           ;    // [float ,   15] 전일거래대금
    char    value               [  15];    char    _value               ;    // [float ,   15] 누적거래대금
    char    offerho1            [   6];    char    _offerho1            ;    // [float ,  6.2] 매도호가1
    char    bidho1              [   6];    char    _bidho1              ;    // [float ,  6.2] 매수호가1
    char    offerrem1           [   6];    char    _offerrem1           ;    // [long  ,    6] 매도호가수량1
    char    bidrem1             [   6];    char    _bidrem1             ;    // [long  ,    6] 매수호가수량1
    char    offercnt1           [   4];    char    _offercnt1           ;    // [long  ,    4] 매도호가건수1
    char    bidcnt1             [   4];    char    _bidcnt1             ;    // [long  ,    4] 매수호가건수1
    char    offerho2            [   6];    char    _offerho2            ;    // [float ,  6.2] 매도호가2
    char    bidho2              [   6];    char    _bidho2              ;    // [float ,  6.2] 매수호가2
    char    offerrem2           [   6];    char    _offerrem2           ;    // [long  ,    6] 매도호가수량2
    char    bidrem2             [   6];    char    _bidrem2             ;    // [long  ,    6] 매수호가수량2
    char    offercnt2           [   4];    char    _offercnt2           ;    // [long  ,    4] 매도호가건수2
    char    bidcnt2             [   4];    char    _bidcnt2             ;    // [long  ,    4] 매수호가건수2
    char    offerho3            [   6];    char    _offerho3            ;    // [float ,  6.2] 매도호가3
    char    bidho3              [   6];    char    _bidho3              ;    // [float ,  6.2] 매수호가3
    char    offerrem3           [   6];    char    _offerrem3           ;    // [long  ,    6] 매도호가수량3
    char    bidrem3             [   6];    char    _bidrem3             ;    // [long  ,    6] 매수호가수량3
    char    offercnt3           [   4];    char    _offercnt3           ;    // [long  ,    4] 매도호가건수3
    char    bidcnt3             [   4];    char    _bidcnt3             ;    // [long  ,    4] 매수호가건수3
    char    offerho4            [   6];    char    _offerho4            ;    // [float ,  6.2] 매도호가4
    char    bidho4              [   6];    char    _bidho4              ;    // [float ,  6.2] 매수호가4
    char    offerrem4           [   6];    char    _offerrem4           ;    // [long  ,    6] 매도호가수량4
    char    bidrem4             [   6];    char    _bidrem4             ;    // [long  ,    6] 매수호가수량4
    char    offercnt4           [   4];    char    _offercnt4           ;    // [long  ,    4] 매도호가건수4
    char    bidcnt4             [   4];    char    _bidcnt4             ;    // [long  ,    4] 매수호가건수4
    char    offerho5            [   6];    char    _offerho5            ;    // [float ,  6.2] 매도호가5
    char    bidho5              [   6];    char    _bidho5              ;    // [float ,  6.2] 매수호가5
    char    offerrem5           [   6];    char    _offerrem5           ;    // [long  ,    6] 매도호가수량5
    char    bidrem5             [   6];    char    _bidrem5             ;    // [long  ,    6] 매수호가수량5
    char    offercnt5           [   4];    char    _offercnt5           ;    // [long  ,    4] 매도호가건수5
    char    bidcnt5             [   4];    char    _bidcnt5             ;    // [long  ,    4] 매수호가건수5
    char    totofferrem         [   6];    char    _totofferrem         ;    // [long  ,    6] 매도호가총수량
    char    totbidrem           [   6];    char    _totbidrem           ;    // [long  ,    6] 매수호가총수량
    char    totoffercnt         [   4];    char    _totoffercnt         ;    // [long  ,    4] 매도호가총건수
    char    totbidcnt           [   4];    char    _totbidcnt           ;    // [long  ,    4] 매수호가총건수
    char    totofferremcha      [   8];    char    _totofferremcha      ;    // [long  ,    8] 직전매도대비총수량
    char    totbidremcha        [   8];    char    _totbidremcha        ;    // [long  ,    8] 직전매수대비총수량
    char    offerhodrate1       [   6];    char    _offerhodrate1       ;    // [float ,  6.2] 매도호가등락률1
    char    bidhodrate1         [   6];    char    _bidhodrate1         ;    // [float ,  6.2] 매수호가등락률1
    char    offerhodrate2       [   6];    char    _offerhodrate2       ;    // [float ,  6.2] 매도호가등락률2
    char    bidhodrate2         [   6];    char    _bidhodrate2         ;    // [float ,  6.2] 매수호가등락률2
    char    offerhodrate3       [   6];    char    _offerhodrate3       ;    // [float ,  6.2] 매도호가등락률3
    char    bidhodrate3         [   6];    char    _bidhodrate3         ;    // [float ,  6.2] 매수호가등락률3
    char    offerhodrate4       [   6];    char    _offerhodrate4       ;    // [float ,  6.2] 매도호가등락률4
    char    bidhodrate4         [   6];    char    _bidhodrate4         ;    // [float ,  6.2] 매수호가등락률4
    char    offerhodrate5       [   6];    char    _offerhodrate5       ;    // [float ,  6.2] 매도호가등락률5
    char    bidhodrate5         [   6];    char    _bidhodrate5         ;    // [float ,  6.2] 매수호가등락률5
    char    theoryprice         [   6];    char    _theoryprice         ;    // [float ,  6.2] 이론가
    char    ibasis              [   6];    char    _ibasis              ;    // [float ,  6.2] 이론BASIS
    char    kdiff               [   6];    char    _kdiff               ;    // [float ,  6.2] 괴리도
    char    kasis               [   6];    char    _kasis               ;    // [float ,  6.2] 괴리율
    char    sbasis              [   6];    char    _sbasis              ;    // [float ,  6.2] 시장BASIS
    char    jnilopenyak         [   8];    char    _jnilopenyak         ;    // [long  ,    8] 전일미결제약정수량　
    char    openyak             [   8];    char    _openyak             ;    // [long  ,    8] 미결제약정수량
    char    openupdn            [   8];    char    _openupdn            ;    // [long  ,    8] 미결제증감수량
    char    open                [   6];    char    _open                ;    // [float ,  6.2] 시가
    char    high                [   6];    char    _high                ;    // [float ,  6.2] 고가
    char    low                 [   6];    char    _low                 ;    // [float ,  6.2] 저가
    char    jnilclose           [   6];    char    _jnilclose           ;    // [float ,  6.2] 전일종가
    char    uplmtprice          [   6];    char    _uplmtprice          ;    // [float ,  6.2] 상한가
    char    dnlmtprice          [   6];    char    _dnlmtprice          ;    // [float ,  6.2] 하한가
    char    recprice            [   6];    char    _recprice            ;    // [float ,  6.2] 기준가
} k06110Out1, *LPk06110Out1;
#define NAME_k06110Out1     "k06110Out1"

typedef struct _k06110Out1_Ex
{
	char    price               [   6];
	char    uplmtprice          [   6];
	char    dnlmtprice          [   6];
	char    offerho3            [   6];
	char    offerho2            [   6];
	char    offerho1            [   6];
	char    bidho1              [   6];
	char    bidho2              [   6];
	char    bidho3              [   6];
} k06110Out1_Ex, *LPk06110Out1_Ex;

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
