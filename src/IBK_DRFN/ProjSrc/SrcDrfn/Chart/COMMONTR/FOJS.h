///////////////////////////////////////////////////////////////////////////////////////////////////
// 상품선물 ( FOJS,attr,key=10,keycnt=2000,bufcnt=5,group=1 )
#pragma pack( push, 1 )

#define NAME_FOJS     "FOJS"

// 입력
typedef struct _FOJS_InBlock
{
    char    symbol              [  10];    char    _symbol              ;    // [string,   10] KEY심볼코드
} FOJS_InBlock, *LPFOJS_InBlock;
#define NAME_FOJS_InBlock     "InBlock"

// 출력
typedef struct _FOJS_OutBlock
{
    char    symbol              [  10];    char    _symbol              ;    // [string,   10] KEY심볼코드
    char    date                [   8];    char    _date                ;    // [string,    8] 일자
    char    time                [  10];    char    _time                ;    // [string,   10] 시간
    char    kodate              [   8];    char    _kodate              ;    // [string,    8] 한국일자
    char    kotime              [  10];    char    _kotime              ;    // [string,   10] 한국시간
    char    open                [   8];    char    _open                ;    // [string,    8] 시가
    char    high                [   8];    char    _high                ;    // [string,    8] 고가
    char    low                 [   8];    char    _low                 ;    // [string,    8] 저가
    char    price               [   8];    char    _price               ;    // [string,    8] 현재가
    char    sign                [   1];    char    _sign                ;    // [string,    1] 전일대비구분
    char    change              [   8];    char    _change              ;    // [string,    8] 전일대비
    char    drate               [   6];    char    _drate               ;    // [float ,  6.2] 등락율
    char    bid                 [   8];    char    _bid                 ;    // [string,    8] 매수호가
    char    bidrem              [   9];    char    _bidrem              ;    // [long  ,    9] 매수잔량
    char    offer               [   8];    char    _offer               ;    // [string,    8] 매도호가
    char    offerrem            [   9];    char    _offerrem            ;    // [long  ,    9] 매도잔량
    char    cvolume             [   9];    char    _cvolume             ;    // [long  ,    9] 체결수량
    char    volume              [   9];    char    _volume              ;    // [long  ,    9] 거래량
} FOJS_OutBlock, *LPFOJS_OutBlock;
#define NAME_FOJS_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
