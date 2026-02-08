///////////////////////////////////////////////////////////////////////////////////////////////////
// 주식선물 체결(JFC0) ( JFC0,attr,key=8,group=1 )
#pragma pack( push, 1 )

#define NAME_JFC0     "JFC0"

// 입력
typedef struct _JFC0_InBlock
{
    char    kfutcode            [   8];    char    _kfutcode            ;    // [string,    8] KEY선물단축코드
} JFC0_InBlock, *LPJFC0_InBlock;
#define NAME_JFC0_InBlock     "InBlock"

// 출력
typedef struct _JFC0_OutBlock
{
    char    kfutcode            [   8];    char    _kfutcode            ;    // [string,    8] KEY선물단축코드
    char    chetime             [   6];    char    _chetime             ;    // [string,    6] 체결시간
    char    sign                [   1];    char    _sign                ;    // [string,    1] 전일대비구분
    char    change              [   7];    char    _change              ;    // [long  ,    7] 전일대비
    char    drate               [   6];    char    _drate               ;    // [float ,  6.2] 등락율
    char    price               [   7];    char    _price               ;    // [long  ,    7] 현재가
    char    open                [   7];    char    _open                ;    // [long  ,    7] 시가
    char    high                [   7];    char    _high                ;    // [long  ,    7] 고가
    char    low                 [   7];    char    _low                 ;    // [long  ,    7] 저가
    char    cgubun              [   1];    char    _cgubun              ;    // [string,    1] 체결구분('+':매수 '-':매도)
    char    cvolume             [   7];    char    _cvolume             ;    // [long  ,    7] 체결량
    char    volume              [   7];    char    _volume              ;    // [long  ,    7] 누적거래량
    char    volchange           [   7];    char    _volchange           ;    // [long  ,    7] 거래량전일대비
    char    voldrate            [   8];    char    _voldrate            ;    // [float ,  8.2] 거래량등락율
    char    value               [  15];    char    _value               ;    // [float , 15.2] 누적거래대금
    char    mdvolume            [   7];    char    _mdvolume            ;    // [long  ,    7] 매도누적체결량
    char    mdchecnt            [   7];    char    _mdchecnt            ;    // [long  ,    7] 매도누적체결건수
    char    msvolume            [   7];    char    _msvolume            ;    // [long  ,    7] 매수누적체결량
    char    mschecnt            [   7];    char    _mschecnt            ;    // [long  ,    7] 매수누적체결건수
    char    cpower              [   9];    char    _cpower              ;    // [float ,  9.2] 체결강도
    char    offerho1            [   7];    char    _offerho1            ;    // [long  ,    7] 매도호가1
    char    bidho1              [   7];    char    _bidho1              ;    // [long  ,    7] 매수호가1
    char    openyak             [   8];    char    _openyak             ;    // [long  ,    8] 미결제약정수량
    char    openyakcha          [   8];    char    _openyakcha          ;    // [long  ,    8] 미결제약정직전대비
    char    openyakchange       [   8];    char    _openyakchange       ;    // [long  ,    8] 미결제약정전일대비
    char    baseprice           [   7];    char    _baseprice           ;    // [long  ,    7] 기초자산가격
    char    theoryprice         [   7];    char    _theoryprice         ;    // [long  ,    7] 이론가
    char    grate               [   7];    char    _grate               ;    // [long  ,    7] 괴리도
    char    kasis               [   6];    char    _kasis               ;    // [float ,  6.2] 괴리율
    char    sbasis              [   7];    char    _sbasis              ;    // [long  ,    7] 시장BASIS
    char    ibasis              [   7];    char    _ibasis              ;    // [long  ,    7] 이론BASIS
    char    futcode             [   8];    char    _futcode             ;    // [string,    8] 선물단축코드
} JFC0_OutBlock, *LPJFC0_OutBlock;
#define NAME_JFC0_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
