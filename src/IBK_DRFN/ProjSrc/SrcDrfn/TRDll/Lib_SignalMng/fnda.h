///////////////////////////////////////////////////////////////////////////////////////////////////
// 종목검색 시스템 유저 신호(FNDA) ( FNDA,attr,key=4,group=1 )
#pragma pack( push, 1 )

#define NAME_FNDA     "FNDA"

// 입력
typedef struct _FNDA_InBlock
{
    char    alcode              [   4];    char    _alcode              ;    // [string,    4] KEY패킷구분코드
} FNDA_InBlock, *LPFNDA_InBlock;
#define NAME_FNDA_InBlock     "InBlock"

// 출력
typedef struct _FNDA_OutBlock
{
    char    alcode              [   4];    char    _alcode              ;    // [string,    4] KEY패킷구분코드
    char    gsalertgubun        [   1];    char    _gsalertgubun        ;    // [string,    1] System('S') User('U')
    char    gsdate              [   8];    char    _gsdate              ;    // [string,    8] 신호발생일자
    char    gstime              [   6];    char    _gstime              ;    // [string,    6] 신호발생시간
    char    gstruecnt           [   4];    char    _gstruecnt           ;    // [string,    4] 현재만족 종목수(기존+신규)
    char    gsdatacnt           [   4];    char    _gsdatacnt           ;    // [long  ,    4] 전송종목수
    char    gsstatus            [   1];    char    _gsstatus            ;    // [string,    1] 신호구분
    char    gsjobflag           [   1];    char    _gsjobflag           ;    // [string,    1] 신규:'N' 재진입:'R' 탈퇴:'O'
    char    gscode              [   6];    char    _gscode              ;    // [string,    6] 종목코드
    char    gshname             [  20];    char    _gshname             ;    // [string,   20] 종목명
    char    gsentrycnt          [   4];    char    _gsentrycnt          ;    // [long  ,    4] 진입횟수
    char    gsmanagement        [   1];    char    _gsmanagement        ;    // [string,    1] 관리종목
    char    gscontrol           [   1];    char    _gscontrol           ;    // [string,    1] 이상급등종목
    char    gstradestop         [   1];    char    _gstradestop         ;    // [string,    1] 거래정지종목
    char    gsinvestattention   [   1];    char    _gsinvestattention   ;    // [string,    1] 투자유의종목
    char    gsinsinceritynotice [   1];    char    _gsinsinceritynotice ;    // [string,    1] 불성실공시종목
    char    gsconfidencelimit   [   1];    char    _gsconfidencelimit   ;    // [string,    1] 신용제한종목
    char    gscodenotice        [   1];    char    _gscodenotice        ;    // [string,    1] 종목공시
    char    gsjungrimeme        [   1];    char    _gsjungrimeme        ;    // [string,    1] 정리매매종목
    char    gsmarket            [   1];    char    _gsmarket            ;    // [string,    1] 시장구분
    char    gsdepositmoney      [   3];    char    _gsdepositmoney      ;    // [long  ,    3] 증거금비율
    char    gsprice             [   8];    char    _gsprice             ;    // [long  ,    8] 현재가
    char    gssign              [   1];    char    _gssign              ;    // [string,    1] 전일대비등락부호
    char    gschange            [   8];    char    _gschange            ;    // [long  ,    8] 전일대비
    char    gschgrate           [   6];    char    _gschgrate           ;    // [float ,  6.2] 전일대비등락율
    char    gsvolume            [   9];    char    _gsvolume            ;    // [long  ,    9] 거래량
    char    gsvolumerate        [   6];    char    _gsvolumerate        ;    // [float ,  6.2] 거래량전일대비율
    char    gsamount            [   9];    char    _gsamount            ;    // [long  ,    9] 거래대금
} FNDA_OutBlock, *LPFNDA_OutBlock;
#define NAME_FNDA_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////