///////////////////////////////////////////////////////////////////////////////////////////////////
// KRX 체결(KXS3) ( KXS3,attr,key=6,group=1 )
#pragma pack( push, 1 )

#define NAME_KXS3     "KXS3"

// 입력
typedef struct _KXS3_InBlock
{
    char    kshcode             [   6];    char    _kshcode             ;    // [string,    6] KEY단축코드
} KXS3_InBlock, *LPKXS3_InBlock;
#define NAME_KXS3_InBlock     "InBlock"

// 출력
typedef struct _KXS3_OutBlock
{
    char    kshcode             [   6];    char    _kshcode             ;    // [string,    6] KEY단축코드
    char    chetime             [   6];    char    _chetime             ;    // [string,    6] 체결시간
    char    sign                [   1];    char    _sign                ;    // [string,    1] 전일대비구분
    char    change              [   8];    char    _change              ;    // [long  ,    8] 전일대비
    char    drate               [   6];    char    _drate               ;    // [float ,  6.2] 등락율
    char    price               [   8];    char    _price               ;    // [long  ,    8] 현재가
    char    opentime            [   6];    char    _opentime            ;    // [string,    6] 시가시간
    char    open                [   8];    char    _open                ;    // [long  ,    8] 시가
    char    hightime            [   6];    char    _hightime            ;    // [string,    6] 고가시간
    char    high                [   8];    char    _high                ;    // [long  ,    8] 고가
    char    lowtime             [   6];    char    _lowtime             ;    // [string,    6] 저가시간
    char    low                 [   8];    char    _low                 ;    // [long  ,    8] 저가
    char    openchange          [   8];    char    _openchange          ;    // [long  ,    8] 시가전일대비
    char    opendrate           [   6];    char    _opendrate           ;    // [float ,  6.2] 시가전일대비등락율
    char    highchange          [   8];    char    _highchange          ;    // [long  ,    8] 고가전일대비
    char    highdrate           [   6];    char    _highdrate           ;    // [float ,  6.2] 고가전일대비등락율
    char    lowchange           [   8];    char    _lowchange           ;    // [long  ,    8] 저가전일대비
    char    lowdrate            [   6];    char    _lowdrate            ;    // [float ,  6.2] 저가전일대비등락율
    char    cgubun              [   1];    char    _cgubun              ;    // [string,    1] 체결구분('+':매수 '-':매도)
    char    cvolume             [   8];    char    _cvolume             ;    // [long  ,    8] 체결량
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] 누적거래량
    char    volchange           [  12];    char    _volchange           ;    // [long  ,   12] 거래량전일대비
    char    voldrate            [   8];    char    _voldrate            ;    // [float ,  8.2] 거래량등락율
    char    voltrate            [   6];    char    _voltrate            ;    // [float ,  6.2] 거래량회전율
    char    value               [  15];    char    _value               ;    // [long  ,   15] 누적거래대금
    char    valchange           [  15];    char    _valchange           ;    // [float , 15.2] 거래대금전일대비
    char    valdrate            [  10];    char    _valdrate            ;    // [float , 10.2] 거래대금등락율
    char    mdvolume            [  12];    char    _mdvolume            ;    // [long  ,   12] 매도누적체결량
    char    mdchecnt            [   8];    char    _mdchecnt            ;    // [long  ,    8] 매도누적체결건수
    char    msvolume            [  12];    char    _msvolume            ;    // [long  ,   12] 매수누적체결량
    char    mschecnt            [   8];    char    _mschecnt            ;    // [long  ,    8] 매수누적체결건수
    char    cpower              [   9];    char    _cpower              ;    // [float ,  9.2] 체결강도
    char    w_avrg              [   8];    char    _w_avrg              ;    // [float ,  8.2] 가중 평균가
    char    offerho1            [   8];    char    _offerho1            ;    // [long  ,    8] 매도호가
    char    bidho1              [   8];    char    _bidho1              ;    // [long  ,    8] 매수호가
    char    offerrem1           [  12];    char    _offerrem1           ;    // [long  ,   12] 매도호가잔량1
    char    bidrem1             [  12];    char    _bidrem1             ;    // [long  ,   12] 매수호가잔량1
    char    totofferrem         [  12];    char    _totofferrem         ;    // [long  ,   12] 매도호가총수량
    char    totbidrem           [  12];    char    _totbidrem           ;    // [long  ,   12] 매수호가총수량
    char    status              [   2];    char    _status              ;    // [long  ,    2] 장구분정보
    char    jnilvolume          [  12];    char    _jnilvolume          ;    // [long  ,   12] 전일동시간대거래량(30초단위)
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] 단축코드
} KXS3_OutBlock, *LPKXS3_OutBlock;
#define NAME_KXS3_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
