#if !defined(AFX_OHHTS_OPTN_CNTG__H_A0D8DC83_1BE0_40a1_9798_33283A4E05BF_INCLUDED_)
#define AFX_OHHTS_OPTN_CNTG__H_A0D8DC83_1BE0_40a1_9798_33283A4E05BF_INCLUDED_

#include "Data_Common.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// 옵션 체결(OOC0) ( OOC0,attr,key=8,group=1 )
#pragma pack( push, 1 )

#define NAME_OOC0     "OOC0"

// 입력
typedef struct _OOC0_InBlock
{
    char    koptcode            [   8];    char    _koptcode            ;    // [string,    8] KEY옵션단축코드
} OOC0_InBlock, *LPOOC0_InBlock;
#define NAME_OOC0_InBlock     "InBlock"

// 출력
typedef struct _OOC0_OutBlock
{
    char    koptcode            [   8];    char    _koptcode            ;    // [string,    8] KEY옵션단축코드
    char    chetime             [   6];    char    _chetime             ;    // [string,    6] 체결시간
    char    sign                [   1];    char    _sign                ;    // [string,    1] 전일대비구분
    char    change              [   6];    char    _change              ;    // [float ,  6.2] 전일대비
    char    drate               [   6];    char    _drate               ;    // [float ,  6.2] 등락율
    char    price               [   6];    char    _price               ;    // [float ,  6.2] 현재가
    char    open                [   6];    char    _open                ;    // [float ,  6.2] 시가
    char    high                [   6];    char    _high                ;    // [float ,  6.2] 고가
    char    low                 [   6];    char    _low                 ;    // [float ,  6.2] 저가
    char    cgubun              [   1];    char    _cgubun              ;    // [string,    1] 체결구분('+':매수 '-':매도)
    char    cvolume             [   6];    char    _cvolume             ;    // [long  ,    6] 체결량
    char    volume              [  12];    char    _volume              ;    // [long  ,   12] 누적거래량
    char    volchange           [  12];    char    _volchange           ;    // [float , 12.2] 거래량전일대비
    char    voldrate            [   8];    char    _voldrate            ;    // [float ,  8.2] 거래량등락율
    char    value               [  12];    char    _value               ;    // [long  ,   12] 누적거래대금
    char    mdvolume            [  12];    char    _mdvolume            ;    // [long  ,   12] 매도누적체결량
    char    mdchecnt            [   8];    char    _mdchecnt            ;    // [long  ,    8] 매도누적체결건수
    char    msvolume            [  12];    char    _msvolume            ;    // [long  ,   12] 매수누적체결량
    char    mschecnt            [   8];    char    _mschecnt            ;    // [long  ,    8] 매수누적체결건수
    char    cpower              [   9];    char    _cpower              ;    // [float ,  9.2] 체결강도
    char    offerho1            [   6];    char    _offerho1            ;    // [float ,  6.2] 매도호가1
    char    bidho1              [   6];    char    _bidho1              ;    // [float ,  6.2] 매수호가1
    char    openyak             [   8];    char    _openyak             ;    // [long  ,    8] 미결제약정수량
    char    openyakchange       [   8];    char    _openyakchange       ;    // [long  ,    8] 미결제약정전일대비
    char    k200jisu            [   6];    char    _k200jisu            ;    // [float ,  6.2] KOSPI200지수
    char    eqva                [   7];    char    _eqva                ;    // [float ,  7.2] KOSPI등가
    char    theoryprice         [   6];    char    _theoryprice         ;    // [float ,  6.2] 이론가
    char    grate               [   6];    char    _grate               ;    // [float ,  6.2] 괴리도
    char    kasis               [   6];    char    _kasis               ;    // [float ,  6.2] 괴리율
    char    impv                [   6];    char    _impv                ;    // [float ,  6.2] 내재변동성
    char    intvalue            [   6];    char    _intvalue            ;    // [float ,  6.2] 내재가치
    char    timevalue           [   6];    char    _timevalue           ;    // [float ,  6.2] 시간가치
    char    jgubun              [   2];    char    _jgubun              ;    // [string,    2] 장운영정보
    char    jnilvolume          [  12];    char    _jnilvolume          ;    // [long  ,   12] 전일동시간대거래량
    char    optcode             [   8];    char    _optcode             ;    // [string,    8] 옵션단축코드
} OOC0_OutBlock, *LPOOC0_OutBlock;
#define NAME_OOC0_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////


#endif // AFX_OHHTS_OPTN_CNTG__H_A0D8DC83_1BE0_40a1_9798_33283A4E05BF_INCLUDED_

///////////////////////////////////////////////////////////////////////////////////////////////////
