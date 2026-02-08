///////////////////////////////////////////////////////////////////////////////////////////////////
// 지수 실시간(IXIJ) ( IXIJ,attr,key=3,group=1 )
#pragma pack( push, 1 )

#define NAME_IXIJ     "IXIJ"

// 입력
typedef struct _IXIJ_InBlock
{
    char    kupcode             [   3];    char    _kupcode             ;    // [string,    3] KEY업종코드
} IXIJ_InBlock, *LPIXIJ_InBlock;
#define NAME_IXIJ_InBlock     "InBlock"

// 출력
typedef struct _IXIJ_OutBlock
{
    char    kupcode             [   3];    char    _kupcode             ;    // [string,    3] KEY업종코드
    char    time                [   6];    char    _time                ;    // [string,    6] 시간
    char    jisu                [   8];    char    _jisu                ;    // [float ,  8.2] 지수
    char    sign                [   1];    char    _sign                ;    // [string,    1] 전일대비구분
    char    change              [   8];    char    _change              ;    // [float ,  8.2] 전일대비
    char    drate               [   6];    char    _drate               ;    // [float ,  6.2] 등락율
    char    cvolume             [   8];    char    _cvolume             ;    // [long  ,    8] 체결량
    char    volume              [   8];    char    _volume              ;    // [long  ,    8] 거래량
    char    volchange           [   8];    char    _volchange           ;    // [long  ,    8] 거래량전일대비
    char    voldrate            [   8];    char    _voldrate            ;    // [float ,  8.2] 거래량등락율
    char    value               [   8];    char    _value               ;    // [long  ,    8] 거래대금
    char    upjo                [   4];    char    _upjo                ;    // [long  ,    4] 상한종목수
    char    highjo              [   4];    char    _highjo              ;    // [long  ,    4] 상승종목수
    char    unchgjo             [   4];    char    _unchgjo             ;    // [long  ,    4] 보합종목수
    char    lowjo               [   4];    char    _lowjo               ;    // [long  ,    4] 하락종목수
    char    downjo              [   4];    char    _downjo              ;    // [long  ,    4] 하한종목수
    char    upjrate             [   6];    char    _upjrate             ;    // [float ,  6.2] 상승종목비율
    char    openjisu            [   8];    char    _openjisu            ;    // [float ,  8.2] 시가지수
    char    opentime            [   6];    char    _opentime            ;    // [string,    6] 시가시간
    char    highjisu            [   8];    char    _highjisu            ;    // [float ,  8.2] 고가지수
    char    hightime            [   6];    char    _hightime            ;    // [string,    6] 고가시간
    char    lowjisu             [   8];    char    _lowjisu             ;    // [float ,  8.2] 저가지수
    char    lowtime             [   6];    char    _lowtime             ;    // [string,    6] 저가시간
    char    frgsvolume          [   8];    char    _frgsvolume          ;    // [long  ,    8] 외인순매수수량
    char    orgsvolume          [   8];    char    _orgsvolume          ;    // [long  ,    8] 기관순매수수량
    char    frgsvalue           [  10];    char    _frgsvalue           ;    // [long  ,   10] 외인순매수금액
    char    orgsvalue           [  10];    char    _orgsvalue           ;    // [long  ,   10] 기관순매수금액
    char    upcode              [   3];    char    _upcode              ;    // [string,    3] 업종코드
} IXIJ_OutBlock, *LPIXIJ_OutBlock;
#define NAME_IXIJ_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
