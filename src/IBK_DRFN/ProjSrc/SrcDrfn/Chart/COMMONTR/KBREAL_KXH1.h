#if !defined(AFX_NAME_KXYS_256A215D_A45E12R__98B1A__INCLUDED_)
#define AFX_NAME_KXYS_256A215D_A45E12R__98B1A__INCLUDED_

///////////////////////////////////////////////////////////////////////////////////////////////////
// KRX 예상체결(KXYS) ( KXYS,attr,key=6,group=1 )
#pragma pack( push, 1 )

#define NAME_KXYS     "KXYS"

// 입력
typedef struct _KXYS_InBlock
{
    char    kshcode             [   6];    char    _kshcode             ;    // [string,    6] KEY단축코드
} KXYS_InBlock, *LPKXYS_InBlock;
#define NAME_KXYS_InBlock     "InBlock"

// 출력
typedef struct _KXYS_OutBlock
{
    char    kshcode             [   6];    char    _kshcode             ;    // [string,    6] KEY단축코드
    char    hotime              [   6];    char    _hotime              ;    // [string,    6] 호가시간
    char    yeprice             [   8];    char    _yeprice             ;    // [long  ,    8] 예상체결가격
    char    preysign            [   1];    char    _preysign            ;    // [string,    1] 예상체결가직전현재가대비구분
    char    preychange          [   8];    char    _preychange          ;    // [long  ,    8] 예상체결가직전현재가대비
    char    preydrate           [   6];    char    _preydrate           ;    // [float ,  6.2] 예상체결가직전현재가등락율
    char    jnilysign           [   1];    char    _jnilysign           ;    // [string,    1] 예상체결가전일종가대비구분
    char    jnilychange         [   8];    char    _jnilychange         ;    // [long  ,    8] 예상체결가전일종가대비
    char    jnilydrate          [   6];    char    _jnilydrate          ;    // [float ,  6.2] 예상체결가전일종가등락율
    char    yecvolume           [   8];    char    _yecvolume           ;    // [long  ,    8] 예상체결변동량
    char    yevolume            [  12];    char    _yevolume            ;    // [long  ,   12] 예상체결수량
    char    yvolchange          [  12];    char    _yvolchange          ;    // [long  ,   12] 예상체결수량전일거래량대비
    char    yofferho0           [   8];    char    _yofferho0           ;    // [long  ,    8] 예상매도호가
    char    ybidho0             [   8];    char    _ybidho0             ;    // [long  ,    8] 예상매수호가
    char    yofferrem0          [  12];    char    _yofferrem0          ;    // [long  ,   12] 예상매도호가수량
    char    ybidrem0            [  12];    char    _ybidrem0            ;    // [long  ,   12] 예상매수호가수량
    char    ypreoffercha0       [  12];    char    _ypreoffercha0       ;    // [long  ,   12] 예상직전매도대비수량
    char    yprebidcha0         [  12];    char    _yprebidcha0         ;    // [long  ,   12] 예상직전매수대비수량
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] 단축코드
} KXYS_OutBlock, *LPKXYS_OutBlock;
#define NAME_KXYS_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // AFX_NAME_KXYS_256A215D_A45E12R__98B1A__INCLUDED_
