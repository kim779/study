#if !defined(AFX_OHHTS_ELW_CNTG__H_B8898524_2F02_4517_AC5A_E782C176A44F_INCLUDED_)
#define AFX_OHHTS_ELW_CNTG__H_B8898524_2F02_4517_AC5A_E782C176A44F_INCLUDED_

#include "Data_Common.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// ELW 체결(EWs3) ( EWs3,attr,key=6,group=1 )
#pragma pack( push, 1 )

#define NAME_EWs3     "EWs3"

// 입력
typedef struct _EWs3_InBlock
{
    char    kshcode             [   6];    char    _kshcode             ;    // [string,    6] KEY단축코드
} EWs3_InBlock, *LPEWs3_InBlock;
#define NAME_EWs3_InBlock     "InBlock"

// 출력
typedef struct _EWs3_OutBlock
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
    char    cgubun              [   1];    char    _cgubun              ;    // [string,    1] 체결구분('+':매수 '-':매도)
    char    cvolume             [   8];    char    _cvolume             ;    // [long  ,    8] 체결량
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] 누적거래량
    char    value               [  15];    char    _value               ;    // [long  ,   15] 누적거래대금
    char    mdvolume            [  12];    char    _mdvolume            ;    // [long  ,   12] 매도누적체결량
    char    mdchecnt            [   8];    char    _mdchecnt            ;    // [long  ,    8] 매도누적체결건수
    char    msvolume            [  12];    char    _msvolume            ;    // [long  ,   12] 매수누적체결량
    char    mschecnt            [   8];    char    _mschecnt            ;    // [long  ,    8] 매수누적체결건수
    char    cpower              [   9];    char    _cpower              ;    // [float ,  9.2] 체결강도
    char    w_avrg              [   8];    char    _w_avrg              ;    // [long  ,    8] 가중평균가
    char    offerho1            [   8];    char    _offerho1            ;    // [long  ,    8] 매도호가
    char    bidho1              [   8];    char    _bidho1              ;    // [long  ,    8] 매수호가
    char    status              [   2];    char    _status              ;    // [long  ,    2] 장구분정보
    char    jnilvolume          [  12];    char    _jnilvolume          ;    // [long  ,   12] 전일동시간대거래량
    char    premium             [   8];    char    _premium             ;    // [float ,    8] 프리미엄
    char    moneyness           [   1];    char    _moneyness           ;    // [string,    1] ATM구분(1:ATM 2:ITM 3:OTM)
    char    shcode              [   6];    char    _shcode              ;    // [string,    6] 단축코드
} EWs3_OutBlock, *LPEWs3_OutBlock;
#define NAME_EWs3_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // AFX_OHHTS_ELW_CNTG__H_B8898524_2F02_4517_AC5A_E782C176A44F_INCLUDED_

///////////////////////////////////////////////////////////////////////////////////////////////////
