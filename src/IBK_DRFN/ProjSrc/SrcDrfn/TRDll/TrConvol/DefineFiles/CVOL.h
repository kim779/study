///////////////////////////////////////////////////////////////////////////////////////////////////
// 종목검색 순간체결량(CVOL) ( CVOL,attr,key=4,group=1 )
#pragma pack( push, 1 )

#define NAME_CVOL     "CVOL"

// 입력
typedef struct _CVOL_InBlock
{
    char    alcode              [   4];    char    _alcode              ;    // [string,    4] 패킷구분코드
} CVOL_InBlock, *LPCVOL_InBlock;
#define NAME_CVOL_InBlock     "InBlock"

// 출력
typedef struct _CVOL_OutBlock
{
    char    alcode              [   4];    char    _alcode              ;    // [string,    4] 패킷구분코드
    char    gsalertgubun        [   1];    char    _gsalertgubun        ;    // [string,    1] System('S') User('U')
    char    gsdate              [   8];    char    _gsdate              ;    // [string,    8] 신호발생일자
    char    gstime              [   6];    char    _gstime              ;    // [string,    6] 신호발생시간
    char    gstruecnt           [   4];    char    _gstruecnt           ;    // [long  ,    4] 현재만족종목수(기존+신규)
    char    gsdatacnt           [   4];    char    _gsdatacnt           ;    // [long  ,    4] 전송종목수
    char    gsstatus            [   1];    char    _gsstatus            ;    // [string,    1] 신호발생:'S' 조건이상:'E'
    char    sjobflag            [   1];    char    _sjobflag            ;    // [string,    1] 신규:'N' 재진입:'R' 탈퇴:'O'
    char    scode               [   6];    char    _scode               ;    // [string,    6] 종목코드
    char    shname              [  20];    char    _shname              ;    // [string,   20] 종목명
    char    sentrycnt           [   4];    char    _sentrycnt           ;    // [long  ,    4] 진입횟수
    char    sup_l               [   2];    char    _sup_l               ;    // [string,    2] 업종대분류(대2:중3:소3)
    char    sup_m               [   3];    char    _sup_m               ;    // [string,    3] 업종중분류
    char    sup_s               [   3];    char    _sup_s               ;    // [string,    3] 업종소분류
    char    sprefershare        [   1];    char    _sprefershare        ;    // [string,    1] 우선주
    char    smanagement         [   1];    char    _smanagement         ;    // [string,    1] 관리종목
    char    scontrol            [   1];    char    _scontrol            ;    // [string,    1] 이상급등종목
    char    stradestop          [   1];    char    _stradestop          ;    // [string,    1] 거래정지종목
    char    sinsinceritynotice  [   1];    char    _sinsinceritynotice  ;    // [string,    1] 불성실공시종목
    char    sconfidencelimit    [   1];    char    _sconfidencelimit    ;    // [string,    1] 신용제한종목
    char    scodenotice         [   1];    char    _scodenotice         ;    // [string,    1] 종목공시
    char    sjungrimeme         [   1];    char    _sjungrimeme         ;    // [string,    1] 정리매매종목
    char    smarket             [   1];    char    _smarket             ;    // [string,    1] 시장구분
    char    sdepositmoney       [   3];    char    _sdepositmoney       ;    // [long  ,    3] 증거금비율
    char    sinvestrisk         [   1];    char    _sinvestrisk         ;    // [string,    1] 투자위험종목
    char    sinvestwarn         [   1];    char    _sinvestwarn         ;    // [string,    1] 투자경고종목
    char    sinvestattention    [   1];    char    _sinvestattention    ;    // [string,    1] 투자주의종목
    char    sinvestrisknotice   [   1];    char    _sinvestrisknotice   ;    // [string,    1] 투자위험예고종목
    char    sprice              [   8];    char    _sprice              ;    // [long  ,    8] 현재가
    char    ssign               [   1];    char    _ssign               ;    // [string,    1] 전일대비등락부호
    char    schange             [   8];    char    _schange             ;    // [long  ,    8] 전일대비
    char    schgrate            [   6];    char    _schgrate            ;    // [float ,  6.2] 전일대비등락율
    char    svolume             [   9];    char    _svolume             ;    // [long  ,    9] 거래량
    char    svolumerate         [   6];    char    _svolumerate         ;    // [float ,  6.2] 거래량전일대비율
    char    samount             [   9];    char    _samount             ;    // [long  ,    9] 거래대금
    char    scvolume            [   9];    char    _scvolume            ;    // [long  ,    9] 순간체결량
    char    scvolumerate        [   6];    char    _scvolumerate        ;    // [long  ,    6] 누적거래량대비순간체결량
    char    scamount            [   9];    char    _scamount            ;    // [long  ,    9] 순간체결대금
    char    scgubun             [   1];    char    _scgubun             ;    // [string,    1] 체결구분('+':매수 '-':매도)
} CVOL_OutBlock, *LPCVOL_OutBlock;
#define NAME_CVOL_OutBlock     "OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////
