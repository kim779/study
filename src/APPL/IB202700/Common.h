#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

#include "../IhUtils.h"
#include "../../axis/axMsg.hxx"
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "../../h/memDc.h"

#include "grid/griditem.h"

#define WM_MANAGE	(WM_USER + 1000)
/////////////////////////////////////////////////////////////////
//////  WM_MAINWND	Msg Kind
/////////////////////////////////////////////////////////////////
// LOWORD(wParam)
#define MK_GETROOT		1	// ROOT Directory
#define MK_GETUSER		2	// USER명
#define MK_SETPAL		3	// set color
#define MK_SENDTR		4	// sendtr
#define MK_VARIANT		5	// variant
#define MK_GETGROUP		6	// get group count
#define MK_GROUP		7	// HIWORD(wParam) 1 => add 0 => minus
#define MK_GETBITMAP		8	// get bitmap handle
#define MK_TREEVISIBLE		9	// get treesize
#define MK_BASKETVISIBLE	10	// get treesize
#define MK_ISEXPAND		11	// 그룹추가시 확장모드인가 분할모드인가...
#define MK_SIZE			12	// 화면사이즈 조절시 사용
#define MK_OPENSCR		13	// 화면 열기
#define MK_EXPECT		14	// 예상가 관련
#define MK_EXCEL		15	// 엑셀로 보내기
#define MK_AUTOEXPECT   16  // 예상가 자동/수동
#define MK_SELGROUP		17	// 그룹선택에 관한 메시지
#define MK_MEMOUPDATE		18	// 메모 변경
#define MK_CLICKGROUP		19	// 그룹 클릭시 작업을 위한 옵션
#define MK_CLOSEGRID		20	// 그리드 창을 닫는다
#define MK_SETDATAKIND		21	// 데이터 종류
#define MK_TREEDATA		22	// 트리선택시 그룹으로 보내지는 메시지
#define MK_GETDATATITLE		23	// 현재 데이터의 타이틀을 요구한다.
#define MK_GETWND		24	// 윈도우 포인터 가져오기
#define MK_GETAFIELD		25	// 현재 선택된 필드 인덱스를 가져온다 From ToolWindow
#define MK_TURNCODE		26	// 종목돌려보기
#define MK_HAWKEYE		28	// hawkeye의 사용여부 확인
#define MK_INPUTOVER		29	// 오버되는 종목코드 입력하기
#define MK_SYNC			30	// get critical section
#define MK_RECVDATA		31	// data를 받았을 때 메시지
#define MK_GETMAX		32	// get max data
#define MK_RTSDATA		33	// rtsdata
#define MK_HIDECODE		34	// hide code search combo
#define MK_KILLFOCUS		35	// kill focus
#define MK_ENDDRAG		36	// end drag
#define MK_INSERTROW		37	
#define MK_DELETEROW		38	
#define MK_SETUPOK		39	// 설정 변경
#define MK_SAVE			40	// 관심그룹 저장
#define MK_SELINT		41	// 관심그룹 선택
#define MK_SELFIELD		42	// 필드 선택
#define MK_SEARCHOPT		43	// get search option
#define MK_CHANGEFIELD		44	// Change Field
#define MK_TOOLSIZE		45	// toolbar size
#define MK_CHART		46	// 차트 사용여부
#define MK_GETDATAKIND		47	
#define MK_GETID		48	// user id
#define MK_NEWS			50	// 뉴스
#define MK_MEMO			51	// 메모
#define MK_PROCDLL		52	// 내자신에게 procdll이 들어오는것을 버린다.
#define MK_SISECATCH		53	// 시세감시
#define MK_GROUPHEIGHT		54
#define MK_HSCROLL			55	// ADD PSH 20070911
#define MK_MKPROP			56	// ADD PSH 20070917
#define MK_GETMOVECFG		57	// ADD PSH 20070917
#define MK_SETMOVECFG		58	// ADD PSH 20070917
#define MK_GETMOVESAVE		59	// ADD PSH 20070917
#define MK_SETMOVESAVE		60	// ADD PSH 20070917
#define MK_TOOLBUTTON		61	// ADD PSH 20070917
#define MK_SORTBASE			62	// ADD PSH 20070917
#define MK_AUTOSAVE			63	// ADD PSH 20070917
#define MK_ALERTWAVE		64	// ADD PSH 20070917
#define MK_MARKETCOLOR		65	// ADD PSH 20070917
#define MK_MARKETCODE		66	// ADD PSH 20070917
#define MK_RTSVISIBLE		67
#define MK_SHOWTOOLTIP		70
#define MK_GETCBINDEX       71  
#define MK_CLOSE			72
#define MK_ISTREE			73
#define MK_INTSAVE      	74
#define MK_GROUPSAVE      	75
#define	MK_SELECTTREE		76
#define MK_DELETEMEMO		77
#define MK_INSERTCODE		78
#define MK_SENDTREE			79
#define MK_SELECTGROUP		80
#define MK_INITGRID			81
#define MK_CALLINTEREST		82
#define MK_SELECTFIRST		83
#define MK_SENDREMAIN		84
#define MK_REMAINDATA		84
#define MK_TRIGGEROW		85
#define MK_REFRESH			86
#define MK_EQUALIZER		87
#define MK_CHANGERECOMMANDFIELD 88
#define MK_UPJONGDATA		89
#define MK_RECOMMAND		90
#define MK_SAVERECOMMANDGROUP	91
#define MK_DELREMCOMMANDGROUP 92
#define MK_ARRANGE			93
#define MK_GETARRANGE		94
#define MK_SETARRANGE		95
#define MK_AUTOARRANGE		96
#define	MK_SETARRANGEACTIVE	97
#define MK_SETARRANGENOACTIVE 98
#define MK_TREERESEND		99
#define MK_GETTREEITEM		100
#define MK_READONLYRECOM	101
#define MK_ISSUE			102
#define MK_CLEARGROUP		103
#define MK_CLEARSECONDSEND	104
#define MK_SAVECFG			105
#define MK_GETKEY			106
#define MK_NEWSNCHART		107		//2011.12.02	KSJ
#define MK_MARGIN			108		//2011.12.05	KSJ
#define MK_INFO				109		//2011.12.05	KSJ
#define MK_TICKER			110		//2011.12.12	KSJ
#define MK_RTSDATAx			111	// 2012.01.19 KSJ rtsdatax


// ctrl ID
#define IDC_MENU_TOOL		1031	// ADD PSH 20070927
#define IDC_MENU_INTERSAVE	1032	// ADD PSH 20070911
#define IDC_MENU_INTERTAKE	1033	// ADD PSH 20070911
#define IDC_MENU_BASESORT	1034	// ADD PSH 20070911
#define IDC_MENU_INFO		1035	// ADD PSH 20070911
#define IDC_MENU_MARGIN		1036	// ADD PSH 20070911
#define IDC_MENU_MARKER		1037	// ADD PSH 20070911
#define IDC_MENU_TICKER		1038	// ADD PSH 20070911
#define IDC_MENU_TICKER_SETUP	1039	// ADD PSH 20070911
#define IDC_MENU_SAVE_CND1	1040	// ADD PSH 20070911
#define IDC_MENU_SAVE_CND2	1041	// ADD PSH 20070911
#define IDC_MENU_MARKET_COLOR	1042	// ADD PSH 20070911
#define IDC_MENU_CODE		1043	// ADD PSH 20070911
#define IDC_MENU_MULTICHART	1044	// ADD HJJ 20080416
#define IDC_MENU_SHOWRTSWND	1045	// ADD HJJ 20080416
#define IDC_MENU_FIELD		1050	// ADD PSH 20070911
#define IDC_BT_EQUALIZER	1051
#define IDC_BT_DOMINO		1052
#define IDC_CB_ARRANGE		1053
#define IDC_MENU_UPJONGSORT	1054
#define IDC_MENU_NAMESORT	1055
#define IDC_MENU_CODESORT	1056
#define IDC_MENU_NEWSNCHART	1057	// 2011.12.02 KSJ 뉴스와 차트

// HIWORD(wParam)	// Msg Option
// common
#define MO_NONE			0

#define MO_SHOW			1
#define MO_HIDE			2

#define MO_MINUS		1
#define MO_PLUS			2

#define MO_ON			1
#define MO_OFF			2

// option
#define MO_SET			1
#define MO_GET			2
#define MO_ADD			3
#define MO_DEL			4

#define MO_USE			2

// 기타 sync 옵션
#define MO_LOCK			1
#define MO_UNLOCK		2

// MK_GETWND의 옵션
#define MO_SELECT		0
#define MO_VISIBLE		1
#define MO_FULL			2

// Ledger filler
#define ORDER_INFO_IDX          (0x01)

#define DEF_ROWH		20

#define CONFIG_INI     "%s\\user\\%s\\IB202700.ini"

#define FILE_CONFIG	"intercfg.ini"		// 뉴관심종목에서 새로이 만들어진 관심종목의 전체적인 환경을 저장하는 파일 기본파일
#define FILE_GROUP	"portfolio.ini"		// 관심종목의 그룹을 저장하는 파일
#define FILE_SUBDATA	"portfolio.i"		// 확장자 뒤에 두자리는 숫자로 저장된다.
#define FILE_FIELD	"interfield.ini"	// 필드 저장파일

// ini file sec
#define SEC_GROUP	"GROUP"
#define SEC_GRID	"GRID"
#define SEC_MAIN	"MAIN"
#define SEC_FIELDS	"FIELDS"
#define SEC_GROUPNAME	"GROUPNAME"
#define SEC_GROUPORDER	"GROUPORDER"
#define SEC_GRID	"GRID"
#define SEC_FOLDER	"FOLDER"
#define SEC_INTERFIELD	"interField"
#define KEY_SELECTGROUP "SELECTGROUPDLL"

// ini file key
#define KEY_FIELD	"FIELD"
#define KEY_VERSION	"VER"
#define KEY_TIME	"TIME"
#define KEY_EXPAND	"EXPAND"
#define KEY_FILLMODE	"FILLMODE"
#define KEY_COUNT	"COUNT"
#define KEY_RATE	"RATE"
#define KEY_HEIGHT	"HEIGHT"
#define KEY_WIDTH	"WIDTH"
#define KEY_TREESIZE	"TREESIZE"
#define KEY_BASKETSIZE	"BASKETSIZE"
#define KEY_TREEUSE	"TREEUSE"
#define KEY_BASKETUSE	"BASKETUSE"
#define KEY_GROUPSIZE	"GROUPSIZE"
#define KEY_ACTIVE	"ACTIVE"
#define KEY_SELECT	"SELECT"
#define KEY_DATA	"DATA"
#define KEY_DATA2027	"DATA2027"
#define KEY_BKCOLOR	"BKCOLOR"
#define KEY_COLOR1	"COLOR1"
#define KEY_COLOR2	"COLOR2"
#define KEY_REALCOLOR	"REALCOLOR"
#define KEY_FRCOLOR	"FRCOLOR"
#define KEY_FLASHTYPE	"FLASHTYPE"
#define KEY_LINKMAP	"LINKMAP"
#define KEY_FONTPOINT	"FONTPOINT"
#define KEY_FONTNAME	"FONTNAME"
#define KEY_CURRBOLD	"CURRBOLD"
#define KEY_NAMEBOLD	"NAMEBOLD"
#define KEY_ALLBOLD	"ALLBOLD"
#define KEY_BIG		"BIG"
#define KEY_AUTO	"AUTO"
#define KEY_SOPTION	"SOPTION"
#define KEY_MOVECFG	"MOVECFG"
#define KEY_MOVESAVE	"MOVESAVE"
#define KEY_EQUALIZER	"EQUALIZER"
#define KEY_CBARRANGE	"CBARRANGE"
#define MAP_GROUPEDIT  "IB191010 /S/s/p5/t"
#define MAP_ORDERLIMIT "IB0000B2 /S/s/p5/t"
#define MAP_CHEG       "IB310200 /S/s/p5/t"
#define MAP_CHART      "IB711400 /p5/t"
#define MAP_JANGO      "IB320100 /S/s/p5/t"
#define MAP_SISECHEG   "IB400900 /S/s/p5/t"
#define MAP_CONFIG	   "IB202201" 

// define Window Messeage
#define WM_APP_SIG			(WM_USER+0xF0)
#define WP_ACCN_CHANGE			(0x00000001)
#define WP_JNGO_CHANGE			(0x00000002)
#define WP_CODE_CHANGE			(0x00000003)
#define WP_MADO_ALL_CANCEL		(0x00000004)
#define WP_MASU_ALL_CANCEL		(0x00000005)
#define WP_ORDER				(0x00000006)
#define WP_MOUSE_IN				(0x00000007)
#define WP_MOUSE_OUT			(0x00000008)
#define WP_CANCEL_ORDER			(0x00000009)
#define WP_MODIFY_ORDER			(0x0000000A)
#define WP_CANCEL_RSVD			(0x0000000B)     
#define WP_MODIFY_RSVD			(0x0000000C)
#define WP_CURRCELL_CHANGE		(0x0000000D)
#define WP_RSVD_ORDER			(0x0000000E)
#define WP_MADO_RSVD_ALL_CANCEL	(0x0000000F)
#define WP_MASU_RSVD_ALL_CANCEL	(0x00000010)
#define WP_HOGA_CLICK			(0x00000011)
#define WP_CTRL2_FOLD           (0x00000012)
#define WP_CTRL2_FOLD_DISABLE   (0x00000013)
#define WP_ONECLICK_MODE        (0x00000014)
#define WP_SET_JCNT             (0x00000015)
#define WP_CTRL_FOLD			(0x00000016)
#define WP_RSVD_REALORDER		(0x00000017)

#define WM_APP_REFRESH		(WM_USER+0xF1)
#define WP_MICHEG			(0x00000001)
#define WP_JANGO			(0x00000002)
#define WP_RESERVE			(0x00000003)
#define WP_ACCOUNT			(0x00000004)
#define WP_MICHEG_INSERT    (0x00000005)
#define WP_MICHEG_UPDATE    (0x00000006)
#define WP_MICHEG_DELETE    (0x00000007)
#define WP_ORDER_RESULT     (0x00000008)
#define WP_FOLDSTAT			(0x00000009)

//2013.02.07 CX_OptionEx 추가
#define WM_OPTIONEX			(WM_USER+0xF2)

// define Color!
#define COLOR_LINE			93
#define COLOR_TABLE			75
#define COLOR_BK			64
#define COLOR_GRIDHEAD		74
#define COLOR_GUIDEBACK		213
#define COLOR_PANELBACK		66
#define COLOR_BODYBACK		181
#define COLOR_GRIDHEADTXT	76
#define COLOR_DATA			99
#define COLOR_TEXT			69
#define COLOR_TABLEHEAD		96
#define	COLOR_MDB			171		// 매도 back
#define	COLOR_MSB			170		// 매수 back
#define COLOR_PLUSTXT		94
#define COLOR_MINUSTXT		95
#define COLOR_FOCUS			78

#define BACK_INDEX					64
#define CMD_BACK_INDEX				66
#define CONTENTS_BACK_INDEX			181
#define CONTENTS_BORDER_INDEX		93
#define TAB_BORDER_INDEX			44
#define GUIDE_BACK_INDEX			34
#define GUIDE_TXT_INDEX				130
#define CLR_TAB_FORE		70
#define CLR_TAB_SELFORE		71

#define PADDING_VALUE	(6)
#define ARROW_BOX_WIDTH		(4)

#define CODECTRL_WIDTH		(80)

// Expand Hoga
#define EXPAND_HOGA_WIDTH   (25)

// 전체 윈도우의 높이
#define MAIN_WND_HEIGHT		(624)

// 계좌/종목코드 윈도우의 높이
#define GROUP_WND_HEIGHT		(94)
#define ACCN_WND_HEIGHT         (132)
#define SISE_WND_HEIGHT         (45)

// 주문설정/종목선택 그리드 넓이
#define CTRL_WND_WIDTH		(180)

// 주문가능수량/ 실시간잔고/ 미체결내역 넓이
#define CTRL2_WND_WIDTH     (400)

// GUIDE 윈도우 높이
#define GUID_WND_HEIGHT		(22)

// SCROLLBAR 넓이
#define SCROLLBAR_WIDTH		(18)

// 그리드 셀 높이
#define GRID_CELL_HEIGHT	(18)

#define MAX_FHOGA			(5)
#define FCODE_LEN			(8)
#define DOUBLE_PREFIX		(0.00005)
#define MARKET_JPRC			(-99999999)

#define MAX_GRID	5 //BUFFET 10->6
#define MAX_LINE	100

#define gSYMBOL	_T("$90303")
#define gEXPECT	_T("90991")
#define gFILTERING	_T("90302")		//2011.12.27 KSJ
#define	CCOD		"1301"
#define	HCOD		"1301"
#define	FCOD		"30301"
#define	OCOD		"40301"
#define	PCOD		"PCOD"
#define	UCOD		"20301"
#define ELWCOD		"E1301"
#define SINJUCOD	"scode"
#define THIRDCOD	"1301"
#define MCSYMBOL	"IB7020"
#define SFOD		"100301"	// 주식, 금리, 통화선물 종목코드 KSJ 2012.10.11

const int colorKKRED  = PALETTERGB(255,   0,   0);
const int colorDKRED  = PALETTERGB(255, 137, 155);
const int colorLTRED  = PALETTERGB(255, 201, 206);

const int colorKKBLUE = PALETTERGB(  0,   0, 255);
const int colorDKBLUE = PALETTERGB(126, 176, 255);
const int colorLTBLUE = PALETTERGB(200, 222, 255);

// Delimeter for parsing
#define P_DELI	0x7f
#define P_TAB	0x09
#define P_NEW	0x0a
#define P_PER	0x25

#define	deliMITER	";"
#define mchartMAP	"IB702000"		// 복수종목 멀티차트
#define	kcurrMAP	"IB200100"		// kospi  현재가
#define	fcurrMAP	"IB400100"		// future 현재가
#define	ocurrMAP	"IB400200"		// option 현재가
#define	sfcurrMAP	"IB425000"		// 주식선물 현재가 KSJ 2012.10.11
#define	cccurrMAP	"IB453000"		// 금리/통화선물 현재가 KSJ 2012.10.11
#define	focurrMAP	"IB440100"		// 주식옵션현재가
#define	icurrMAP	"IB290300"		// 업종 현재가
#define	ecurrMAP	"IB280200"		// elw 현재가
#define	chartMAP	"IB711200"		// chart
#define	configMAP	"IB202201"		// config map

struct	_timerID
{
	UINT	kind  : 4;		// 4bit안에 다섯가지의 값을 담는다. // none, news, chart, bong, content... 
	UINT	row   : 8;		// row값
	UINT	col   : 6;		// col값
	UINT	dummy : 14;		
};

#define TK_NORMAL	0x00		// 리얼타임시 사용한는 kind
#define TK_NEWS		0x01		// 뉴스
#define TK_CHART	0x02		// 차트
#define TK_CODE		0x03		// 종목
#define TK_BONG		0x04		// 일봉데이터 
#define TK_CONTENT	0x05		// 안보여지는 텍스트
#define TK_RCODE	0x06		// 종목코드
#define TK_MEMO		0x07


enum _rtmAction { rtmCELL = 0, rtmROW, rtmNONE };
enum _dblAction { dblNONE = 0, dblCURR, dblCHART, dblTRIGGER, dblUSER };

enum { JK_FUTURE, JK_SPREAD, JK_CALLOPT, JK_PUTOPT, JK_JCALLOPT, JK_JPUTOPT, JK_STAR, JK_SFUTURE, JK_SFSPREAD, JK_KTBFUTURE, JK_USDFUTURE, JK_GOLDFUTURE };

// for speed parsing
class StringProxy : public pair<LPCSTR,LPCSTR> {
public:
	StringProxy() : pair<LPCSTR,LPCSTR>() {}
	StringProxy(LPCSTR st, LPCSTR ed) : pair<LPCSTR, LPCSTR>(st, ed) {}
	string ToStr() { return string(first, second); }
	CString ToCStr() { return CString(first, second-first); }
};

class op_notice_spliter {
public:
	bool operator() (char ch)
	{
		return (ch=='\r')||(ch=='\n')||(ch=='\t');
	}
};

typedef map<int, StringProxy> RealData;
typedef enum { CT_NONE=0, CT_FUTURE, CT_OPTION, CT_SPREAD } CODE_TYPE;
class SiseData {
public:
	SiseData() { code=""; type=CT_NONE; }
	CString code;
	CODE_TYPE type;
	char udgb;
	int gjga, siga, koga, jega, curr, pcurr;
	int rate, gvol, mgjv, shga, hhga, diff;
	int t_dvol, t_svol, t_dcnt, t_scnt, t_sdch;
	vector<int> fhoga;
	vector<int> frate;
	struct  {
		int mdga;
		int msga;
		int dvol;
		int svol;
		int dcnt;
		int scnt;
	} hoga[MAX_FHOGA];
};

class Order {
public:
	Order() { ZeroMemory(this, sizeof(Order)); }
	char acno[12];
	char code[12];
	int ojno;	// 원주문번호
	int juno;	// 주문번호
	int odgb;	// 주문구분
	int oprc;	// 주문가격 (MARKET_JPRC 일 경우에는 시장가주문 2011.02.21 )
	int oqty;	// 주문수량
	int dprc;	// 체결가격
	int dqty;	// 체결수량
	int wqty;	// 미체결수량
	bool stop;  // stop주문여부
	int row;	// 그리드Row
	int col;	// 그리드Col
	bool bmarket; //시장가 주문 여부
};

class OrderResult {
public:
	CString acno;
	CString juno;
	CString ojno;
	CString emsg;
};

class Micheg {
public:
	Micheg() { ZeroMemory(this, sizeof(Micheg)); }
	char acno[12];		// 계좌번호
	char code[12];		// 종목코드
	int oprc;			// 주문가격
	int d_cnt;			// 매도 미체결건수
	int s_cnt;			// 매수 미체결건수
	int d_wqty;			// 매도 미체결수량
	int s_wqty;			// 매수 미체결수량
};

class Jango {
public:
	Jango() { ZeroMemory(this, sizeof(Jango)); }
	void operator=(const Jango &rhs) { CopyMemory(this, &rhs, sizeof(Jango)); }
	char flag;		// for Window Message
	char acno[12];	// 계좌번호
	char code[12];	// 종목코드
	char name[40];	// 종목명
	char gubn[10];	// 구분
	int  jqty;		// 보유수량
	int  xqty;		// 청산가능수량
	int  pprc;		// 평균가
	double camt;	// 평가금액
	double tamt;	// 평가손익
	double srat;	// 수익률
	double mamt;	// 매입금액
};

class ReserveData
{
public:
	ReserveData() { ZeroMemory(this, sizeof(ReserveData)); }
	int odgb;	// 주문구분 1:매도, 2:매수
	int cprc;	// 체크가격
	int oprc;	// 주문가격
	int ptic;	// 체크/주문 가격 Tick
	int oqty;	// 주문수량
	int row;	// 그리드Row
	int col;	// 그리드Col
	bool bmarket; //시장가 주문 여부
};

class ModifyData
{
public:
	ModifyData() { ZeroMemory(this, sizeof(ModifyData)); }
	int odgb;	// 주문구분 1:매도, 2:매수
	int fprc;	// 정정할 주문가격
	int tprc;	// 주문할 주문가격
	int rtic;   // 이동 Tick for Rsvd
};

class CancelData
{
public:
	CancelData() { ZeroMemory(this, sizeof(CancelData)); }
	int odgb;	// 주문구분 1:매도, 2:매수
	int cprc;	// 취소주문가격 
};

typedef vector<ReserveData> RsvdDataArray;			// 예약주문내역목록
typedef map<string, Jango> CodeJangoMap;			// 종목별 잔고 현황
typedef map<int, Order> OrderMap;					// 주문번호별 주문정보 맵
typedef map<string, OrderMap> CodeOrderMap;			// 종목별 주문번호별 주문정보 맵
typedef map<int, Micheg> PriceMichegMap;			// 주문가격별 미체결 수량정보
typedef map<string, PriceMichegMap> CodeMichegMap;	// 종목별 주문가격별 미체결 수량정보

class Account {				// 계좌종합정보
public:
	Account() : IsValid(FALSE),IsOwn(FALSE) {}
	BOOL IsValid{};				// 검증된 계좌인지
	CString acno, pswd, acnm;		// 계좌번호, 비밀번호
	int mult{};				// 주문 배수
	CodeOrderMap order;			// 종목별 > 주문번호별 > 미체결 주문정보
	CodeJangoMap jango;			// 종목별 잔고내역
	CodeMichegMap micheg;			// 종목별 주문가격별 미체결 수량정보
	BOOL IsOwn{};
};

typedef map<string, Account> AccountMap;	// 계좌번호별 계좌종합정보 맵

// Delimeter for parsing
#define P_DELI	0x7f
#define P_TAB	0x09
#define PTAB	"\t"
#define P_NEW	0x0a
#define PNEW	"\n"
#define P_PER	0x25


static struct	_gridHdr {
	long	stid;		// string table id
	UINT	width;		// fixed width
	UINT	attr;		// fixed attribute
	char	symbol[8];	// nonfixed symbol
	UINT	format;		// nonfixed format
	UINT	attrx;		// nonfixed attr
	UINT	needs;		// default field
} gridHdr[] = 
{
	{IDS_GH_RCOD,		0,	GVAT_HIDDEN,			"1021",	GVFM_LEFT,	0,					2},	// 0	리얼타임코드
	{IDS_GH_EXPECT,		0,	GVAT_HIDDEN,			"7788",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 1	예상체결flag
	{IDS_GH_OPEN,		0,	GVAT_HIDDEN,			"2029",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 2	시가
	{IDS_GH_HIGH,		0,	GVAT_HIDDEN,			"2030",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 3	고가
	{IDS_GH_LOW,		0,	GVAT_HIDDEN,			"2031",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 4	저가
	{IDS_GH_PRECURR,	0,	GVAT_HIDDEN,			"2934",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 5	종가
	{IDS_GH_PREOPEN,	0,	GVAT_HIDDEN,			"2931",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 6	전일종가
	{IDS_GH_PREHIGH,	0,	GVAT_HIDDEN,			"2932",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 7	전일시가
	{IDS_GH_PRELOW,		0,	GVAT_HIDDEN,			"2933",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 8	전일고가
	{IDS_GH_VOL,		0,	GVAT_HIDDEN,			"2027",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 9	전일저가
	{IDS_GH_INFO,		0,	GVAT_HIDDEN,			"1370",	GVFM_RIGHT,	GVAT_CONDITIONx,				2},	// 10	종목특이사항
	{IDS_GH_RATE,		0,	GVAT_HIDDEN,			"2033",	GVFM_RIGHT,	GVAT_CONDITIONx|GVAT_SIGNx,		2},	// 11	등락률
	{IDS_GH_TVOL,		0,	GVAT_HIDDEN,			"2032",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 12	체결량
	{IDS_GH_7852,		0,	GVAT_HIDDEN,	"7852",	GVFM_RIGHT,	GVAT_COMMA,				2},	// 13	현재가
	{IDS_GH_7853,		0,	GVAT_HIDDEN,	"7853",	GVFM_RIGHT,	GVAT_COMMA,				2},	// 14	거래량
	{IDS_GH_7854,		0,	GVAT_HIDDEN,	"7854",	GVFM_RIGHT,	GVAT_COMMA,				2},	// 15	대비
	{IDS_GH_7855,		0,	GVAT_HIDDEN,	"7855",	GVFM_RIGHT,	GVAT_COMMA,				2},	// 16	등락률
	{-2,				0,	GVAT_HIDDEN,			"1354",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 17		증거금율
	{-2,				0,	GVAT_HIDDEN,			"1355",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 18		신용보증금증거금율
	
	{-1,			0,	GVAT_TEXT,			"9###", GVFM_CENTER,	GVAT_TOOLTIP|GVAT_COMMA|GVAT_CONDITIONx,2},	// 19	거래량	
	{IDS_GH_CODE,		0,	GVAT_SORTSTR|GVAT_DRAGMOVE,	"1301",	GVFM_CENTER,	GVAT_TOOLTIP|GVAT_DRAGDROP,		2},	// 20	종목코드
	{IDS_GH_NAME,		110,	GVAT_SORTSTR,			"1022",	GVFM_LEFT,	GVAT_TOOLTIP|GVAT_EDIT|GVAT_DRAGDROP,	2},	// 21	종목명
	{IDS_GH_CURR,		70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2023",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,				2},	// 22	현재가

	{IDS_GH_DIFF,		60,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"2024",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONTRAST,		1},	// 14	대비
	{IDS_GH_VOL,		80,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2027",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		1},	// 15	거래량
	{IDS_GH_RATE,		50,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"2033",	GVFM_RIGHT,	GVAT_CONDITION|GVAT_SIGNx,				0},	// 16	등락률
	{IDS_GH_TPRC,		70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2028",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 17	거래대금
	{IDS_GH_ASK,		70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"3051",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		1},	// 18	매도	//2013.02.19 KSJ 2025 --> 3051 심볼변경
	{IDS_GH_BID,		70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"3071",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		1},	// 19	매수	//2013.02.19 KSJ 2026 --> 3071 심볼변경
	{IDS_GH_ASKSIZE,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"3041",	GVFM_RIGHT,	GVAT_COMMA,				1},	// 20
	{IDS_GH_BIDSIZE,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"3061",	GVFM_RIGHT,	GVAT_COMMA,				1},	// 21
	{IDS_GH_TASKSIZE,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"3101",	GVFM_RIGHT,	GVAT_COMMA,				1},	// 22
	{IDS_GH_TBIDSIZE,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"3106",	GVFM_RIGHT,	GVAT_COMMA,				1},	// 23
	{IDS_GH_ASKRATE,	60,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"3104",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 24
	{IDS_GH_BIDRATE,	60,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"3109",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 25
	{IDS_GH_HOGARATE,	50,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"3107", GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 26
	{IDS_GH_OPEN,		60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2029",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		0},	// 27
	{IDS_GH_HIGH,		60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2030",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		0},	// 28
	{IDS_GH_LOW,		60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2031",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		0},	// 29
	{IDS_GH_OPEN_P,		100,	GVAT_SORTABS|GVAT_DRAGMOVE,	"#1##",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		1},	// 27
	{IDS_GH_HIGH_P,		100,	GVAT_SORTABS|GVAT_DRAGMOVE,	"#2##",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		1},	// 28
	{IDS_GH_LOW_P,		100,	GVAT_SORTABS|GVAT_DRAGMOVE,	"#3##",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		1},	// 29
	{IDS_GH_P_OPEN,		50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"#4##",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		3},	// 27
	{IDS_GH_P_HIGH,		50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"#5##",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		3},	// 28
	{IDS_GH_P_LOW,		50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"#6##",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		3},	// 29
	{IDS_GH_TVOL,		60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2032",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		0},	// 30
	{IDS_GH_OPENRATEW,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"##1#",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		8},	// 31
	{IDS_GH_OPENRATEP,	50,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"##2#",	GVFM_RIGHT,	GVAT_CONDITIONx,			8},	// 32
	{IDS_GH_HIGHRATEW,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"##3#",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		8},	// 33
	{IDS_GH_HIGHRATEP,	50,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"##4#",	GVFM_RIGHT,	GVAT_CONDITIONx,			8},	// 34
	{IDS_GH_LOWRATEW,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"##5#",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		8},	// 35
	{IDS_GH_LOWRATEP,	50,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"##6#",	GVFM_RIGHT,	GVAT_CONDITIONx,			8},	// 36
	{IDS_GH_PRECURR,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2934",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		0},	// 37
	{IDS_GH_PREVOL,		80,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2321",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		0},	// 38
	{IDS_GH_AVOFS,		70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2305",	GVFM_RIGHT,	GVAT_COMMA,				6},	// 39 시가총액 (신관심종목 시가총액 추가_RTS 반영 _ 이전 심벌(2305))
	{IDS_GH_CHESTRENGTH,	60,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"2036",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		0},	// 40
	{IDS_GH_ROTATION,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2350",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 41
	{IDS_GH_FORERATE,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2204",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 42
	{IDS_GH_DATEBONG,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"7777",	0,		GVAT_TOOLTIP,				7},	// 43
	{IDS_GH_POSSNUM,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"###1",	GVFM_RIGHT,	GVAT_COMMA,			9},	// 44
	{IDS_GH_POSSPRC,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"###2",	GVFM_RIGHT,	GVAT_COMMA,			9},	// 45
	{IDS_GH_POSSPROFIT,	70,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"###3",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		9},	// 46
	{IDS_GH_POSSRATE,	50,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"###4",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		9},	// 47
	{IDS_GH_CAPITAL,	55,	GVAT_SORTABS|GVAT_DRAGMOVE,	"1333",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 48 자본금
	{IDS_GH_STOCKCOUNT,	55,	GVAT_SORTABS|GVAT_DRAGMOVE,	"1334",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 49 상장주식수
	{IDS_GH_PRETPRC,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2322",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 50 전일거래대금
	{IDS_GH_UPDNRANGE,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2342",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 51 등락폭
	{IDS_GH_LIMITRANGE,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2343",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 52 제한폭
	{IDS_GH_PRETRATE,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2403",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		10},	// 53 전일거래대비율
	{IDS_GH_REGLINE2,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2421",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 54 2차 저항선
	{IDS_GH_REGLINE1,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2422",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 55 1차 저항선
	{IDS_GH_PIVOT,		50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2423",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 56 PIVOT
	{IDS_GH_SUPLINE1,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2424",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 57 1차 지지선
	{IDS_GH_SUPLINE2,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2425",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 58 2차 지지선
	{IDS_GH_MAX52,		63,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5311",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 59 52주 최고가
	{IDS_GH_MIN52,		63,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5312",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 60 52주 최저가
	{IDS_GH_MAXYEAR,	63,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5313",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 61 년중 최고가
	{IDS_GH_MINYEAR,	63,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5314",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 62 년중 최저가
	{IDS_GH_MAXVOL52,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5315",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 63 52주 최고거래량
	{IDS_GH_MINVOL52,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5316",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 64 52주 최저거래량
	{IDS_GH_MAXVOLYEAR,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5317",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 65 년중 최고거래량
	{IDS_GH_MINVOLYEAR,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5318",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 66 년중 최저거래량
	{IDS_GH_MAXDATE52,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5321",	GVFM_RIGHT,	0,					0},	// 67 52주 최고가일
	{IDS_GH_MINDATE52,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5322",	GVFM_RIGHT,	0,					0},	// 68 52주 최저가일
	{IDS_GH_MAXDATEYEAR,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5323",	GVFM_RIGHT,	0,					0},	// 69 년중 최고가일
	{IDS_GH_MINDATEYEAR,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5324",	GVFM_RIGHT,	0,					0},	// 70 년중 최저가일
	{IDS_GH_MAXVOLDATE52,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5325",	GVFM_RIGHT,	0,					0},	// 71 52주 최고거래량일
	{IDS_GH_MINVOLDATE52,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5326",	GVFM_RIGHT,	0,					0},	// 72 52주 최저거래량일
	{IDS_GH_MAXVOLDATEYEAR,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5327",	GVFM_RIGHT,	0,					0},	// 73 년중 최고거래량일
	{IDS_GH_MINVOLDATEYEAR,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5328",	GVFM_RIGHT,	0,					0},	// 74 년중 최저거래량일
	{IDS_GH_MAXDIFF52,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5331",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 75 52주 최고가대비
	{IDS_GH_MINDIFF52,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5332",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 76 52주 최저가대비
	{IDS_GH_MAXDIFFYEAR,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5333",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 77 년중 최고가대비
	{IDS_GH_MINDIFFYEAR,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5334",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 78 년중 최저가대비
	{IDS_GH_MAXRATE52,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5341",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 79 52주 최고가대비율
	{IDS_GH_MINRATE52,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5342",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 80 52주 최저가대비율
	{IDS_GH_MAXRATEYEAR,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5343",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 81 년중 최고가대비율
	{IDS_GH_MINRATEYEAR,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5344",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 82 년중 최저가대비율
	{IDS_GH_CONTUPLIMIT,	30,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5345",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 83 연속상한일수
	{IDS_GH_CONTUP,		30,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5346",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 84 연속상승일수
	{IDS_GH_CONTDNLIMIT,	30,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5347",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 85 연속하한일수
	{IDS_GH_CONTDN,		30,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5348",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 86 연속하락일수
	{IDS_GH_ELWLEFTDATE,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"1842",	GVFM_RIGHT,	0,					0},	// 89 ELW 잔존일수
	{IDS_GH_ELWLASTDATE,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"1854",	GVFM_RIGHT,	0,					0},	// 90 ELW 최종거래일
	{IDS_GH_ELWCRATE,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"1855",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 91 ELW 전환비율
	{IDS_GH_EPSFULL,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5353",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 92 EPS 전기
	{IDS_GH_EPSHALF,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5354",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 93 EPS 반기
	{IDS_GH_PERFULL,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5355",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 94 PER 전기
	{IDS_GH_PERHALF,	50,	GVAT_SORTABS|GVAT_DRAGMOVE,	"5356",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 95 PER 반기
	{IDS_GH_EQUALIZER,	100,	GVAT_SORTABS|GVAT_DRAGMOVE,	"7778",	0,		GVAT_TOOLTIP,				11},	// 96 체결 이퀄라이저	
	{IDS_GH_RECOMMDATE,	70,	GVAT_SORTSTR,			"1910",	GVFM_RIGHT,	0,					12},	// 96 추천종목 	
	{IDS_GH_RECOMBASICCOST,	0,	GVAT_HIDDEN,			"1911",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		13},		// 96 추천종목 	
	{IDS_GH_RECOMSUIKRATE,	60,	GVAT_SORTVAL|GVAT_SORTABS,	"1912",	GVFM_RIGHT,	GVAT_CONDITION|GVAT_SIGNx,  		14},		// 96 추천종목 	
	{IDS_GH_RECOMBMJISU,	0,	GVAT_HIDDEN,			"1913",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		15},		// 96 추천종목 	
	{IDS_GH_RECOMBMRATE,	60,	GVAT_SORTVAL|GVAT_SORTABS,	"1914",	GVFM_RIGHT,	GVAT_CONDITION|GVAT_SIGNx,		16},		// 96 추천종목 	
	{IDS_GH_JISUGUBN,	0,	GVAT_HIDDEN,			"1915",	GVFM_RIGHT,	GVAT_CONDITION|GVAT_SIGNx,		18},		// 96 지수구분 	
	{IDS_GH_RECOMBMGOAL,	60,	GVAT_SORTVAL|GVAT_SORTABS,	"1916",	GVFM_RIGHT,	GVAT_CONDITION|GVAT_SIGNx,		17},		// 96 추천종목  	
	{IDS_GH_UPJONGCODE,	0,	GVAT_HIDDEN,			"1918",	GVFM_RIGHT,	GVAT_COMMA,				19},	// 96 업종코드	
	{IDS_GH_NEWCURR,	0,	GVAT_HIDDEN,			"2022",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		21},	// 새로 추가된 현재가
	{IDS_GH_MPRICE,		70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"3146",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		0},	// 22
	{ IDS_GH_MPRICEPER,	70,	GVAT_SORTABS | GVAT_DRAGMOVE,	"3181",	GVFM_RIGHT,	GVAT_CONDITION | GVAT_SIGNx,		0 }	// 22
};


enum _xKIND { 
	xALLCODE = 1,	// 전종목
		xKOSPI,		// 코스피
		xKOSDAQ,	// 코스닥
		xFUTURE,	// 선물
		xCALLCODE,	// 콜옵션
		xPUTCODE, 	// 풋옵션	
		xKOPSI200,	// 코스피 200
		xKOSDAQ50,	// 코스닥 50
		xKRX100,	// KRX100
		xFREECODE,	// 프리보드
		xETFCODE,	// ETF		
		xELWCODE,	// ELW
		xELWBASE,	// ELW기초자산
		xSINJUCODE,	// 신주인수권
		xKOSPIJISU,	// 코스피지수
		xKOSDAQJISU,	// 코스닥지수
		xTHEMACODE,	// 테마별
		xGROUPCODE,	// 그룹사별
		xINTEREST,	// 관심종목
		xPCALL,		// 주식옵션(콜)
		xPPUT,		// 주식옵션(풋)
		xKOSPIUP,	// 코스피업종
		xKOSDAQUP,	// 코스닥업종
		xKOSPIex,	// 코스피 추가
		xKOSDAQex,	// 코스닥 추가
		xREMAIN,	// 계좌별 보유잔고
		xISSUE,		// 이슈종목
		xELWISSUE,	// 발행회사별
		xELWASSET,	// 기초자산별
		xRECOMMAND,  // IBK추천종목
		xETFALL,
		xETFCOMPANY,
		xETFTHEMA,
		xETFDOMINTER,
		xELWGEN,
		xELWEARLY,
		xKOSPIFUTURE,			//코스피선물
		xSTOCKFUTURE,			//주식선물
		xINTERESTRATEFUTURE,	//금리선물
		xCURRENCYFUTURE,			//통화선물
		xKONEX,		// 2013.06.25 KSJ 코넥스 추가
		xETNCODE,	// 2014.11.06 KSJ ETN 추가
		xSPACCODE	// 2015.11.03 KSJ spac 추가
};

enum _colINDEX
{
	colRCOD = 0,
		colEXPECT,
		colOPEN, 
		colHIGH, 
		colLOW,   
		colPCURR,  
		colPOPEN, 
		colPHIGH, 
		colPLOW, 
		colVOL,
		colINFO,
		colRATE,
		colTVOL,
		col7852,		//2013.07.01 KSJ 현재가(처음에 예상가가 조회되더라도 현재가를 보여줌)
		col7853,		//2013.07.01 KSJ 거래량
		col7854,		//2013.07.01 KSJ 대비
		col7855,		//2013.07.01 KSJ 등락률
		colMAR,
		colMAR1,		//2014.07.18 신용증거금율 추가
		colSIG,			//뉴스/차트
		colCODE,		//종목코드
		colNAME,		//종목명
		colCURR			//현재가
};

// send key
// char 한자리에대한 치환 스트럭쳐
struct _trkey
{
	UINT	kind:5;
	UINT	group:3;	//최대 3bit 이므로 그룹갯수가 최대 7개
};

// treeID.depth
#define TD_ROOT			0x00
#define TD_MID			0x01
#define TD_ITEM			0x02

#define IDC_GRID			1001
#define IDC_GROUP			2001

#define IDC_CB_GROUP		1002
#define IDC_BT_CONFIG		1003
#define IDC_BT_MINUS		1004
#define IDC_BT_PLUS			1005
#define IDC_BT_SAVE			1006
#define IDC_BT_SAVE_GRID	1201
#define IDC_BT_ERASE_GRID	1301

#define SIZE_TOOL	25
#define SIZE_WIDTH	1012
#define SIZE_GROUP	(SIZE_WIDTH - 210)
#define SIZE_GROUPMINX	0//BUFFET 300
#define SIZE_GROUPMINY	0//BUFFET 200
#define SIZE_HEIGHT	530
#define SIZE_DIVIDE	3
#define SIZE_BUTTONS	20
#define SIZE_GAPS	3
#define SIZE_GAPL	5
#define SIZE_COMBO	80
#define SIZE_DROPDOWN	200
#define SIZE_ROWHEIGHT	18
#define SIZE_MINWIDTH	0//511
#define GAP1		1
#define GAP2		2

// DWORD 값에 대한 bit structure 
struct _treeID
{
	UINT	kind:7;		// 대분류 _xKIND 값
	UINT	skind:7;	// 중분류 지정값 또는 인덱스
	UINT	degree:2;	// value값이 %02d(두자리) %03d(세자리) 여부
	UINT	value:10;	// 아이템 지정값 또는 인덱스 1 ~ 1024까지의 값
	UINT	depth:3;	// 대분류/중분류/아이템
	UINT	dummy:3;	
};

#define CAST_TREEID(x)	((struct _treeID*)&x)

[[gsl::suppress(26497)]]
static UINT MAKE_TREEID(UINT kind, UINT skind = 0, UINT degree = 0, UINT value = 0, UINT depth = 0, UINT dummy = 0)
{
	UINT	ret = 0;
	struct _treeID*	treeID = (struct _treeID*)&ret;
	treeID->kind = kind;
	treeID->skind = skind;
	treeID->degree = degree;
	treeID->value = value;
	treeID->depth = depth;
	treeID->dummy = dummy;
	
	return ret;
}

static struct _treeID CONVERT_TREEID(UINT value)
{
	struct _treeID	treeID;
	memcpy(&treeID, &value, sizeof(value));
	return treeID;
}

class CSendData
{
public:
	CSendData() { Reset(); }
	~CSendData() {}
public:
	void Reset()
	{
		m_trcode = _T("");
		m_data = _T("");
		m_userdata = _T("");
		m_key = 0;
	}

	void SetTRCODE(CString trcode) { m_trcode = trcode; }
	void SetData(char* ptr, int len) { m_data = ptr, m_datalen = len; }
	void SetUserData(CString userdata) { m_userdata = userdata; }
	void SetDataKey(int key) { m_key = key; }

	void SetData(CString trcode, int key, char* ptr, int len, CString userdata)
	{
		SetTRCODE(trcode);
		SetDataKey(key);
		SetData(ptr, len);
		SetUserData(userdata);
	}

	CString GetUserData() { return m_userdata; }
	int GetDataKey() { return m_key; }
	CString GetTRCODE() { return m_trcode; }
	char* GetData() { return m_data; }
	int GetDataLen() { return m_datalen; }

	CString	m_trcode;
	char*	m_data;
	int	m_datalen;
	CString m_userdata;
	int	m_key;
};

//class CRecvData
//{
//public:
//	CRecvData() {}
//	~CRecvData() {}
//public:
//	void SetData(CString userdata, WPARAM wParam, LPARAM lParam)
//	{
//		m_userdata = userdata;
//		m_wParam = wParam;
//		m_lParam = lParam;
//	}
//
//	CString	m_userdata;
//	WPARAM	m_wParam{};
//	LPARAM	m_lParam{};
//};

class CRecvData
{
public:
	CRecvData() {}
	~CRecvData() {}
public:
	void SetData(int type, int len, char* lParam)
	{
		key = type;
		size = len;
		data = lParam;
	}

	int     key{};
	int     size{};
	char* data{};
};



class CRTSData
{
public:
	CRTSData(){Reset();}
	~CRTSData(){}
public:
	void Reset() { code = "";fms.RemoveAll() ; arChange.RemoveAll();}

	BOOL SetData(CString code,CString datB)
	{
		//OutputDebugString("RTSDATA SETDATA\n");
		this->code = code;
// 		TRACE("CODE : %s\n",code);
		CString symbol,entry;
		int idx = 0;
		bChange = FALSE;
		arChange.RemoveAll();

		BOOL bChae = FALSE;
		BOOL bHo = FALSE;

		bHoga = FALSE;

		while (!datB.IsEmpty())
		{
			idx = datB.Find('\t');
			if (idx == -1) break;
			
			symbol = datB.Left(idx++);
			datB = datB.Mid(idx);
			idx = datB.Find('\t');
			
			if (idx == -1)
			{
				entry = datB;	
				datB = _T("");
			}
			else
			{
				entry = datB.Left(idx++); 
				datB = datB.Mid(idx);
			}
			
			if(entry == "0" || entry == "-0" || entry == "0.00" || entry == "+0" || entry == " 0")
			{
				entry = " ";
			}
			
			CString tmp;

			if(fms.Lookup(symbol,tmp))
			{
				if(tmp != entry)
				{
					arChange.SetAt(symbol,"1");
					bChange = TRUE;
				}
			}
			else
			{
				arChange.SetAt(symbol,"1");
				bChange = TRUE;
			}
// 			TRACE("FMS SIZE : %d\n",fms.GetCount());
// 			TRACE("%s === %s\n",symbol,entry);
			fms.SetAt(symbol, entry);

			if(symbol == "034")
			{
				bChae = TRUE;
			}
			else if(symbol == "040")
			{
				bHo = TRUE;
			}
			//TRACE("%s\n",code);
		}

		if(bChae == FALSE && bHo == TRUE)
		{
			bHoga = TRUE;
		}

		return bChange;
	}

	BOOL SetData(CString code,CString datB,CMapStringToString &fmsSrc)
	{
// 		OutputDebugString("RTSDATA SETDATA\n");
// 		OutputDebugString(datB + "\n");
		this->code = code;
		//TRACE("CODE : %s\n",code);
		CString symbol,entry;
		int idx = 0;
		bChange = FALSE;
		arChange.RemoveAll();
		
		BOOL bChae = FALSE;
		BOOL bHo = FALSE;
		
		bHoga = FALSE;
		
		while (!datB.IsEmpty())
		{
			idx = datB.Find('\t');
			if (idx == -1) break;
			
			symbol = datB.Left(idx++);
			datB = datB.Mid(idx);
			idx = datB.Find('\t');
			
			if (idx == -1)
			{
				entry = datB;	
				datB = _T("");
			}
			else
			{
				entry = datB.Left(idx++); 
				datB = datB.Mid(idx);
			}
			
			if(entry == "0" || entry == "-0" || entry == "0.00" || entry == "+0" || entry == " 0")
			{
				entry = " ";
			}
			
			CString tmp;
			
			if(fms.Lookup(symbol,tmp))
			{
				tmp.TrimLeft();
				tmp.TrimRight();

				if(tmp != entry)
				{
					arChange.SetAt(symbol,"1");
					bChange = TRUE;
				}
				else
				{
					//OutputDebugString("SYMBOL DATA BLANK\n");
					if(symbol == "111")
					{
						if(fms.Lookup("023",tmp))
						{
							if(tmp != entry)
							{
								fms.SetAt("023",entry);
								bChange = TRUE;
							}
						}
					}
					else if(symbol == "115")
					{
						if(fms.Lookup("024",tmp))
						{
							if(tmp != entry)
							{
								fms.SetAt("024",entry);
								bChange = TRUE;
							}
						}
					}
					else if(symbol == "116")
					{
						if(fms.Lookup("033",tmp))
						{
							if(tmp != entry)
							{
								fms.SetAt("033",entry);
								bChange = TRUE;
							}
						}
					}
				}
				

				if(symbol == "023")
				{
					bChange = TRUE;
				}
				else if(symbol == "024")
				{
					bChange = TRUE;
				}
				else if(symbol == "033")
				{
					bChange = TRUE;
				}
// 				else if(symbol == "111" || symbol == "115" || symbol == "116")
// 				{
// 					if(symbol == "111")
// 					{
// 						if(fms.Lookup("023",tmp))
// 						{
// 							if(tmp != entry)
// 								bChange = TRUE;
// 						}
// 					}
// 					else if(symbol == "115")
// 					{
// 						if(fms.Lookup("024",tmp))
// 						{
// 							if(tmp != entry)
// 								bChange = TRUE;
// 						}
// 					}
// 					else if(symbol == "116")
// 					{
// 						if(fms.Lookup("033",tmp))
// 						{
// 							if(tmp != entry)
// 								bChange = TRUE;
// 						}
// 					}
// 				}
			}
			else
			{
				arChange.SetAt(symbol,"1");
				bChange = TRUE;
			}
			// 			TRACE("FMS SIZE : %d\n",fms.GetCount());
// 			CString s;
// 			s.Format("%s === %s\n",symbol,entry);
// 			OutputDebugString(s);

			if(bChange == TRUE || symbol == "111" || symbol == "023")
				fmsSrc.SetAt(symbol, entry);

			fms.SetAt(symbol,entry);
			
			if(symbol == "034")
			{
				bChae = TRUE;
			}
			else if(symbol == "040")
			{
				bHo = TRUE;
			}
			//TRACE("%s\n",code);
		}
		
		if(bChae == FALSE && bHo == TRUE)
		{
			bHoga = TRUE;
		}
		
		return bChange;
	}

	CString GetData(CString symbol)
	{
		CString tmp;

		if(fms.Lookup(symbol,tmp))
		{
			return tmp;
		}

		return ""; 
	}

	BOOL IsHoga()
	{
		return bHoga;
	}

	BOOL Lookup(CString symbol)
	{
		CString value;
		//TRACE("LOOKUP START %s\n",symbol);
		const BOOL b = fms.Lookup(symbol,value);
		//TRACE("LOOKUP END %s\n",value);

		return b;
	}

	CString GetSymbolData(CString symbol)
	{
		CString value;

		fms.Lookup(symbol,value);

		return value;
	}

	BOOL IsChange(CString symbol)
	{
		CString tmp;
		return arChange.Lookup(symbol,tmp);
	}

	void RemoveAll(){fms.RemoveAll();}

	CString code;
	CMapStringToString fms;
	CMapStringToString arChange;
	BOOL	bChange;
	BOOL	bHoga;
};

class CIHQueue
{
public:
	CIHQueue() {}
	~CIHQueue() {}

	int GetCount()
	{
		return m_mapData.GetCount();
	}

	void RemoveAll()
	{
		m_arKey.RemoveAll();
		m_arValue.RemoveAll();
		m_mapKeyToIndex.RemoveAll();
		m_mapData.RemoveAll();
	}

	void RemoveKey(WORD key)
	{
		WORD	index;
		if (m_mapKeyToIndex.Lookup(key, index))
		{
			m_arKey.RemoveAt(index);
			m_arValue.RemoveAt(index);
			reIndex();
		}
		m_mapData.RemoveKey(key);
	}

	CSendData GetValue(int nIndex)
	{
		CSendData	ret;
		if (nIndex < 0 || nIndex >= m_arValue.GetSize())
			return ret;

		return m_arValue.GetAt(nIndex);
	}

	WORD GetKey(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_arKey.GetSize())
			return 0;

		return m_arKey.GetAt(nIndex);
	}

	CSendData GetData(WORD key)
	{
		CSendData	sdata;
		m_mapData.Lookup(key, sdata);
		return sdata;
	}

	void SetData(WORD key, CSendData value)
	{
		m_mapData.SetAt(key, value);
		const int nIndex = m_arValue.Add(value);
		m_arKey.Add(key);
		m_mapKeyToIndex.SetAt(key, nIndex);
	}

	BOOL IsExist(WORD key)
	{
		CSendData value;
		return m_mapData.Lookup(key, value);
	}

	void reIndex()
	{
		const int ncnt = m_arKey.GetSize();
		for ( int ii = 0 ; ii < ncnt ; ii++ )
		{
			m_mapKeyToIndex.SetAt(m_arKey.GetAt(ii), ii);
		}
	}
	
private:
	CArray<CSendData, CSendData>		m_arValue;
	CWordArray				m_arKey;
	CMap<WORD, WORD, WORD, WORD>		m_mapKeyToIndex;
	CMap<WORD, WORD, CSendData, CSendData>		m_mapData;
};

#define TRKEY_GROUPACCNLIST	0x01	// 보유종목 Tree관련 tr
#define TRKEY_ACCCUSTOM		0x02	// 고객 계좌 
#define TRKEY_ACCSTAFF		0x03	// 고객 계좌 
#define TRKEY_REMAIN		0x04	// 잔고조회
#define TRKEY_SEARCH		0x05	// 등락종목 관련 카테고리 TRKEY
#define TRKEY_NEWS			0x06	// 뉴스종목
#define TRKEY_GRIDNEW		0x07	// 그리드 새로조회
#define TRKEY_GRIDROW		0x08	// 그리드 라인조회
#define TRKEY_GRIDNEWS		0x09	// 뉴스를 보여주는 그리드 종목tr
#define TRKEY_GRIDSAVE		0x10	// 각종목저장
#define TRKEY_ELWISSUESEC   0x13    // ELW 발행회사별
#define TRKEY_ELWBASEASSET  0x14    // ELW 기초자산별
#define TRKEY_TRCOUNT		0x15    // 드래그카운트 조회
#define TRKEY_RECOMMAND		0x16    // 추천종목 조회
#define TRKEY_GRIDUPJONG	0x17    // 추천종목 조회
#define TRKEY_MARKETTIME	0x18    // 장운영시간 조회


#define MY_DEBUG

#ifdef MY_DEBUG
	#define DECLARE_MYDEBUG()				extern FILE *_fpdbg;
	#define INIT_MYDEBUG(path)				_fpdbg = fopen(path, "wb");
	#define UNINIT_MYDEBUG()				fclose(_fpdbg);
	#define FPRT(y)							if(_fpdbg) { fprintf(_fpdbg, y"\r\n"); fflush(_fpdbg); }
	#define FPRT1(y, a1)					if(_fpdbg) { fprintf(_fpdbg, y"\r\n", a1); fflush(_fpdbg); }
	#define FPRT2(y, a1, a2)				if(_fpdbg) { fprintf(_fpdbg, y"\r\n", a1, a2);  fflush(_fpdbg); }
	#define FPRT3(y, a1, a2, a3)			if(_fpdbg) { fprintf(_fpdbg, y"\r\n", a1, a2, a3);  fflush(_fpdbg); }
	#define FPRT4(y, a1, a2, a3, a4)		if(_fpdbg) { fprintf(_fpdbg, y"\r\n", a1, a2, a3, a4);  fflush(_fpdbg); }
	#define FPRT5(y, a1, a2, a3, a4, a5)	if(_fpdbg) { fprintf(_fpdbg, y"\r\n", a1, a2, a3, a4, a5);  fflush(_fpdbg); }
#else
	#define DECLARE_MYDEBUG()
	#define INIT_MYDEBUG(path)
	#define UNINIT_MYDEBUG()
	#define FPRT(y)						
	#define FPRT1(y, a1)					
	#define FPRT2(y, a1, a2)				
	#define FPRT3(y, a1, a2, a3)			
	#define FPRT4(y, a1, a2, a3, a4)		
	#define FPRT5(y, a1, a2, a3, a4, a5)	
#endif









