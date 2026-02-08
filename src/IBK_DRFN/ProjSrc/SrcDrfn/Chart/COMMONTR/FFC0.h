#if !defined(AFX_OHHTS_FUTS_CNTG__H_BFF8EF3A_A8B6_4aa0_A693_2B5A9E3CEAE9_INCLUDED_)
#define AFX_OHHTS_FUTS_CNTG__H_BFF8EF3A_A8B6_4aa0_A693_2B5A9E3CEAE9_INCLUDED_

#include "Data_Common.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// 선물 체결(FFC0) ( FFC0,attr,key=8,group=1 )
#pragma pack( push, 1 )

#define NAME_FFC0     "FFC0"

// 입력
typedef struct _FFC0_InBlock
{
    char    kfutcode            [   8];    char    _kfutcode            ;    // [string,    8] KEY선물단축코드
} FFC0_InBlock, *LPFFC0_InBlock;
#define NAME_FFC0_InBlock     "InBlock"

// 출력
typedef struct _FFC0_OutBlock
{
    char    kfutcode            [   8];    char    _kfutcode            ;    // [string,    8] KEY선물단축코드
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
    char    volchange           [  12];    char    _volchange           ;    // [long  ,   12] 거래량전일대비
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
    char    k200jisu            [   6];    char    _k200jisu            ;    // [float ,  6.2] KOSPI200 지수
    char    theoryprice         [   6];    char    _theoryprice         ;    // [float ,  6.2] 이론가
    char    grate               [   6];    char    _grate               ;    // [float ,  6.2] 괴리도
    char    kasis               [   6];    char    _kasis               ;    // [float ,  6.2] 괴리율
    char    sbasis              [   6];    char    _sbasis              ;    // [float ,  6.2] 시장BASIS
    char    ibasis              [   6];    char    _ibasis              ;    // [float ,  6.2] 이론BASIS
    char    jgubun              [   2];    char    _jgubun              ;    // [string,    2] 장운영정보
    char    jnilvolume          [  12];    char    _jnilvolume          ;    // [long  ,   12] 전일동시간대 거래량
    char    futcode             [   8];    char    _futcode             ;    // [string,    8] 단축코드
} FFC0_OutBlock, *LPFFC0_OutBlock;
#define NAME_FFC0_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////


#endif // AFX_OHHTS_FUTS_CNTG__H_BFF8EF3A_A8B6_4aa0_A693_2B5A9E3CEAE9_INCLUDED_

///////////////////////////////////////////////////////////////////////////////////////////////////
