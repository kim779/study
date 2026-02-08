///////////////////////////////////////////////////////////////////////////////////////////////////
// 주식옵션 체결(JOC0) ( JOC0,attr,key=8,group=1 )
#pragma pack( push, 1 )

#define NAME_JOC0     "JOC0"

// 입력
typedef struct _JOC0_InBlock
{
    char    koptcode            [   8];    char    _koptcode            ;    // [string,    8] KEY옵션단축코드
} JOC0_InBlock, *LPJOC0_InBlock;
#define NAME_JOC0_InBlock     "InBlock"

// 출력
typedef struct _JOC0_OutBlock
{
    char    koptcode            [   8];    char    _koptcode            ;    // [string,    8] KEY옵션단축코드
    char    chetime             [   6];    char    _chetime             ;    // [string,    6] 체결시간
    char    sign                [   1];    char    _sign                ;    // [string,    1] 전일대비구분
    char    change              [   7];    char    _change              ;    // [long  ,    7] 전일대비
    char    drate               [   7];    char    _drate               ;    // [float ,  7.2] 등락율
    char    price               [   7];    char    _price               ;    // [long  ,    7] 현재가
    char    open                [   7];    char    _open                ;    // [long  ,    7] 시가
    char    high                [   7];    char    _high                ;    // [long  ,    7] 고가
    char    low                 [   7];    char    _low                 ;    // [long  ,    7] 저가
    char    cgubun              [   1];    char    _cgubun              ;    // [string,    1] 체결구분('+':매수 '-':매도)
    char    cvolume             [   7];    char    _cvolume             ;    // [string,    7] 체결량
    char    volume              [   7];    char    _volume              ;    // [string,    7] 누적거래량
    char    volchange           [   7];    char    _volchange           ;    // [string,    7] 거래량전일대비
    char    voldrate            [   8];    char    _voldrate            ;    // [float ,  8.2] 거래량등락율
    char    value               [  15];    char    _value               ;    // [float , 15.2] 누적거래대금
    char    mdvolume            [   7];    char    _mdvolume            ;    // [float ,  7.2] 매도누적체결량
    char    mdchecnt            [   7];    char    _mdchecnt            ;    // [long  ,    7] 매도누적체결건수
    char    msvolume            [   7];    char    _msvolume            ;    // [float ,    7] 매수누적체결량
    char    mschecnt            [   7];    char    _mschecnt            ;    // [long  ,    7] 매수누적체결건수
    char    openyak             [   8];    char    _openyak             ;    // [long  ,    8] 미결제약정수량
    char    openyakchange       [   8];    char    _openyakchange       ;    // [long  ,    8] 미결제약정전일대비
    char    optcode             [   8];    char    _optcode             ;    // [string,    8] 옵션단축코드
} JOC0_OutBlock, *LPJOC0_OutBlock;
#define NAME_JOC0_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
