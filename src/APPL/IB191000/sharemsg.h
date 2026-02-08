#pragma once
#include "../IhUtils.h"
#include "../../axis/axMsg.hxx"
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "../../h/memDc.h"
#define WM_MANAGE	(WM_USER + 1000)
#include "griditem.h"
#include "Resource.h"
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

// MK_SISECATCH
#define MO_INIT			1
#define MO_CATCH		2
// MK_GETWND의 옵션
#define MO_VIEW			0x0001	// view window
#define MO_TOOL			0x0002	// tool window
#define MO_TREE			0x0004	// tree window
#define MO_GROUP		0x0008	// group window
#define MO_BASKET		0x0010	// basket window
#define MO_GRID			0x0020	// get active grid window
#define MO_MAIN			0x0040
#define MO_RTS			0x0080
#define MO_ALLWND		(MO_TOOL|MO_TREE|MO_GROUP|MO_BASKET|MO_MAIN|MO_RTS)
#define MO_ACTIVE		1	// active group
#define MO_LEFT			2
#define MO_ALL			3
#define MO_LEFTRIGHT		1
#define MO_TOPBOTTOM		2
#define MO_SELECT		0
#define MO_VISIBLE		1
#define MO_FULL			2

// lParam

/////////////////////////////////////////////////////////////////
// ini file sec
#define SEC_TOOL	"TOOL"
#define SEC_BASKET	"BASKET"
#define SEC_GROUP	"GROUP"
#define SEC_GRID	"GRID"
#define SEC_MAIN	"MAIN"
#define SEC_TREE	"TREE"
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
#define KEY_BIG		"BIG"
#define KEY_AUTO	"AUTO"
#define KEY_SOPTION	"SOPTION"
#define KEY_MOVECFG	"MOVECFG"
#define KEY_MOVESAVE	"MOVESAVE"
#define KEY_EQUALIZER	"EQUALIZER"
// ctrl ID
#define IDC_TOOL		1000
#define IDC_BT_PREV		1001
#define IDC_BT_NEXT		1002
#define IDC_BT_BASKET		1003
#define IDC_CB_FIELD		1005
#define IDC_CB_GROUP		1006
#define IDC_BT_CONFIG		1007
#define IDC_BT_EXPECT		1008
#define IDC_BT_EXCEL		1009
#define IDC_BT_HAWKEYE		1010
#define IDC_BT_MINUS		1011
#define IDC_BT_PLUS		1012
#define IDC_BT_TURN		1013
#define IDC_SPIN_TIME		1014
#define IDC_BTNTAB		1018
#define IDC_CHECK_PLUSMINUS	1019
#define IDC_BT_SAVE		1020
#define IDC_BT_ERASE		1021
#define IDC_BT_UP		1022
#define IDC_BT_DN		1023
#define IDC_BT_CHART		1024
#define IDC_BT_AUTO		1025
#define IDC_BT_ADDSELECT	1026
#define IDC_BT_ADDVISIBLE	1027
#define IDC_BT_ADDFULL		1028
#define IDC_BT_FIELD		1029	// ADD PSH 20070911
#define IDC_BT_FIELDDROP	1030	// ADD PSH 20070911
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
//김덕기추가
#define IDC_CB_SORT			1053
#define IDC_CB_EACH			1054
#define IDC_BT_HOGA			1055
////////////

#define IDC_TREE		1100
#define IDC_BASKET		1200
#define IDC_GROUP		1300
#define IDC_GRIDWND		1400
#define IDC_GRID		1500
#define IDC_RTSGRID		1600

// Tree Item Value
#define IK_INTEREST	1	// 관심종목
#define IK_REMAIN	2	// 보유종목
#define IK_KOSPI	3	// 코스피
#define IK_KOSDAQ	4	// 코스닥
#define IK_KPUP		5	// 코스피 업종
#define IK_KQUP		6	// 코스닥 업종
#define IK_KP200	7	// 코스피 200
#define IK_KQ50		8	// 코스피 50
#define IK_KRX100	9	// KRX100
#define IK_FREEB	10	// 제3시장
#define IK_ETF		11	// ETF
#define IK_ELW		12	// ELW
#define IK_SINJU	13	// 신주인수권
#define IK_UPJISU	14	// 업종지수
#define IK_FUTURE	15	// 선물
#define IK_OPTION	16	// 옵션
#define IK_KPCALL	17	// 코스피 콜옵션
#define IK_KPPUT	18	// 코스피 풋옵션

#define ROUND_ALL    0
#define ROUND_UP     1
#define ROUND_DN     2
#define ROUND_DNLEFT 3

#define ROUND_PANEL		1
#define ROUND_CONTENTS	2
// send key
// char 한자리에대한 치환 스트럭쳐
struct _trkey
{
	UINT	kind:5;
	UINT	group:3;
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

// default value
#define DEF_ROWH		20
#define DEF_FIELD		"필드 00"
#define DEF_FIELD_CONTENT	_T("필드 00;2024;2027;2025;2026;3041;3061;3101;3106;")
#define DEF_FIELD_CONTENT1	_T("필드 00;2024;2033;2027;7777;")

// file names
#define FILE_CONFIG	"intercfg.ini"		// 뉴관심종목에서 새로이 만들어진 관심종목의 전체적인 환경을 저장하는 파일
#define FILE_GROUP	"portfolio.ini"		// 관심종목의 그룹을 저장하는 파일
#define FILE_SUBDATA	"portfolio.i"		// 확장자 뒤에 두자리는 숫자로 저장된다.
#define FILE_FIELD	"interfield.ini"	// 필드 저장파일

// windowsize
#define REGURAR_XSIZE 300//511
#define MAX_GROUP	30 //BUFFET 10->6
#define MAX_FIELD	10
#define MAX_LINE	100
#define MAX_RTSROW	50
#define MAX_RTSCOL	9

#define SIZE_TREE	150
#define SIZE_BASKET	200
#define SIZE_TOOL	52
#define SIZE_RTS	400	
#define SIZE_WIDTH	1012
#define SIZE_GROUP	(SIZE_WIDTH - SIZE_BASKET - SIZE_TREE - 210)
#define SIZE_GROUPMINX	0//BUFFET 300
#define SIZE_GROUPMINY	0//BUFFET 200
#define SIZE_BASKETMINY	100
#define SIZE_TREEMINX	100
#define SIZE_RTSMINX	200
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

#define	gClrBack	192
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
#define	sfcurrMAP	"IB400300"		// 주식선물 현재가 @@Zeta 화면 아직 없음
#define	focurrMAP	"IB440100"		// 주식옵션현재가
#define	icurrMAP	"IB290300"		// 업종 현재가
#define	ecurrMAP	"IB280200"		// elw 현재가
#define	chartMAP	"IB700000"		// chart
#define	configMAP	"IB202201"		// config map

#define	upACTION	1

#define gSYMBOL	_T("$90303")
#define gEXPECT	_T("90991")
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

const int colorKKRED  = PALETTERGB(255,   0,   0);
const int colorDKRED  = PALETTERGB(255, 137, 155);
const int colorLTRED  = PALETTERGB(255, 201, 206);

const int colorKKBLUE = PALETTERGB(  0,   0, 255);
const int colorDKBLUE = PALETTERGB(126, 176, 255);
const int colorLTBLUE = PALETTERGB(200, 222, 255);

#define CLR_UP		94
#define CLR_DN		95
#define CLR_TEXT	69
#define CLR_BACK	68
#define CLR_HTEXT	76
#define CLR_HBACK	74
#define CLR_LINE	75
#define CLR_FOCUS	78

#define	F_POINT		9

#define OPT_BULSUNGSIL	0x00000001	// 불성실
#define OPT_SINGYU	0x00000002	// 신규
#define OPT_GUNRIRAK	0x00000004	// 권리락
#define OPT_WOOSUN	0x00000008	// 우선주
#define OPT_GUANRI	0x00000010	// 관리종목
#define OPT_GAMRI	0x00000020	// 감리종목
#define OPT_TRADESTOP	0x00000040	// 거래정지종목
#define OPT_TRADEWARN	0x00000080	// 투자유의종목
#define OPT_KOSDAQ	0x00000100	// 코스닥
#define OPT_KOSPI	0x00000200	// 코스피종목

#define OPT_STOCK	(OPT_KOSDAQ|OPT_KOSPI)
#define OPT_DEFAULT	(OPT_GUNRIRAK|OPT_WOOSUN|OPT_GUANRI|OPT_GAMRI|OPT_TRADESTOP|OPT_TRADEWARN)

#define	sCURR	"023"
#define	sDIFF	"024"
#define	sASK	"025"
#define	sBID	"026"
#define	sRATE	"033"
#define	sVOL	"027"

enum {JK_FUTURE, JK_SPREAD, JK_CALLOPT, JK_PUTOPT, JK_JCALLOPT, JK_JPUTOPT, JK_STAR, JK_SFUTURE, JK_SFSPREAD, JK_KTBFUTURE, JK_USDFUTURE, JK_GOLDFUTURE};
static struct _mapInfo {
	char	title[48];
	char	scrN[5];
	char	mapN[9];
	char	option[64];
} mapInfo[] = {
			{"시세변동판", "2101", "IB210100", ""},
			{"마켓레이다", "2102", "IB210200", ""}
		};

// needs 값
// 1 : 디폴트
// 2 : 디폴드 이면서 필수 항목
// 7 : 일봉
// 8 : ???
// 9 : 보유단가, 보유수량 등.. 수익률 관련 필드
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

static struct	_gridHdr {
	UINT	stid;		// string table id
	UINT	width;		// fixed width
	UINT	attr;		// fixed attribute
	char	symbol[8];	// nonfixed symbol
	UINT	format;		// nonfixed format
	UINT	attrx;		// nonfixed attr
	UINT	needs;		// default field
} gridHdr[] = 
{
	{IDS_GH_RCOD,		0,	GVAT_HIDDEN,			"1021",	GVFM_LEFT,	0,					2},	// 0	리얼타임코드
	{IDS_GH_EXPECT,		0,	GVAT_HIDDEN,			"7788",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 1	시가
	{IDS_GH_OPEN,		0,	GVAT_HIDDEN,			"2029",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 2	시가
	{IDS_GH_HIGH,		0,	GVAT_HIDDEN,			"2030",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 3	고가
	{IDS_GH_LOW,		0,	GVAT_HIDDEN,			"2031",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 4	저가
	{IDS_GH_PRECURR,	0,	GVAT_HIDDEN,			"2934",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 5	종가
	{IDS_GH_PREOPEN,	0,	GVAT_HIDDEN,			"2931",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 6	전일종가
	{IDS_GH_PREHIGH,	0,	GVAT_HIDDEN,			"2932",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 7	전일시가
	{IDS_GH_PRELOW,		0,	GVAT_HIDDEN,			"2933",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 8	전일고가
	{IDS_GH_VOL,		0,	GVAT_HIDDEN,			"2027",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 9	전일저가
	{IDS_GH_INFO,		0,	GVAT_HIDDEN,			"1370",	GVFM_RIGHT,	GVAT_CONDITIONx,				2},	// 9	종목특이사항
	{IDS_GH_RATE,		0,	GVAT_HIDDEN,			"2033",	GVFM_RIGHT,	GVAT_CONDITIONx|GVAT_SIGNx,				2},	// 10	등락률
	{IDS_GH_TVOL,		0,	GVAT_HIDDEN,			"2032",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	// 11	체결량
	{-2,				0,	GVAT_HIDDEN,			"1354",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		2},	//		증거금율

	{-1,			20,	GVAT_TEXT,			"9###", GVFM_CENTER,	GVAT_TOOLTIP|GVAT_COMMA|GVAT_CONDITIONx,2},	// 12	거래량	
	{IDS_GH_CODE,		0,	GVAT_SORTSTR|GVAT_DRAGMOVE,	"1301",	GVFM_CENTER,	GVAT_TOOLTIP|GVAT_DRAGDROP,		2},	// 13	종목코드
	{IDS_GH_NAME,		110,	GVAT_SORTSTR,			"1022",	GVFM_LEFT,	GVAT_TOOLTIP|GVAT_EDIT|GVAT_DRAGDROP,	2},	// 12	종목명
	{IDS_GH_CURR,		70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2023",	GVFM_RIGHT,	GVAT_COMMA,				2},	// 13	현재가

	{IDS_GH_DIFF,		60,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"2024",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONTRAST,		1},	// 14	대비
	{IDS_GH_VOL,		80,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2027",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		1},	// 15	거래량
	{IDS_GH_RATE,		50,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"2033",	GVFM_RIGHT,	GVAT_CONDITION|GVAT_SIGNx,				0},	// 16	등락률
	{IDS_GH_TPRC,		70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2028",	GVFM_RIGHT,	GVAT_COMMA,				0},	// 17	거래대금
	{IDS_GH_ASK,		60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2025",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		1},	// 18	매도
	{IDS_GH_BID,		60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2026",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		1},	// 19	매수
	{IDS_GH_ASKSIZE,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"3041",	GVFM_RIGHT,	GVAT_COMMA,				1},	// 20
	{IDS_GH_BIDSIZE,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"3061",	GVFM_RIGHT,	GVAT_COMMA,				1},	// 21
	{IDS_GH_TASKSIZE,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"3101",	GVFM_RIGHT,	GVAT_COMMA,				1},	// 22
	{IDS_GH_TBIDSIZE,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"3106",	GVFM_RIGHT,	GVAT_COMMA,				1},	// 23
	{IDS_GH_ASKRATE,	60,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"3104",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 24
	{IDS_GH_BIDRATE,	60,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"3109",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 25
	{IDS_GH_HOGARATE,	50,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"3107", GVFM_RIGHT,	GVAT_CONDITION,				0},	// 26
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
	{IDS_GH_CHESTRENGTH,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2036",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		0},	// 40
	{IDS_GH_ROTATION,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2350",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 41
	{IDS_GH_FORERATE,	70,	GVAT_SORTABS|GVAT_DRAGMOVE,	"2204",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,		0},	// 42
	{IDS_GH_DATEBONG,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"7777",	0,		GVAT_TOOLTIP,				7},	// 43
	{IDS_GH_POSSNUM,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"###1",	GVFM_RIGHT,	GVAT_COMMA,			9},	// 44
	{IDS_GH_POSSPRC,	60,	GVAT_SORTABS|GVAT_DRAGMOVE,	"###2",	GVFM_RIGHT,	GVAT_COMMA,			9},	// 45
	{IDS_GH_POSSPROFIT,	70,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"###3",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,		9},	// 46
	{IDS_GH_POSSRATE,	50,	GVAT_SORTVAL|GVAT_DRAGMOVE,	"###4",	GVFM_RIGHT,	GVAT_CONDITION,				9},	// 47
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
	{IDS_GH_RECOMMDATE,	70,	GVAT_SORTABS,	"1910",	GVFM_RIGHT | GVAT_DATE,	0,								12},	// 96 체결 이퀄라이저	
	{IDS_GH_RECOMBASICCOST,	100,	GVAT_SORTABS,	"1911",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	13},	// 96 체결 이퀄라이저	
	{IDS_GH_RECOMSUIKRATE,	60,	GVAT_SORTABS,	"1912",	GVFM_RIGHT,	GVAT_CONDITION|GVAT_SIGNx,  14},	// 96 체결 이퀄라이저	
	{IDS_GH_RECOMBMJISU,	70,	GVAT_SORTABS,	"1913",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	15},	// 96 체결 이퀄라이저	
	{IDS_GH_RECOMBMRATE,	60,	GVAT_SORTABS,	"1914",	GVFM_RIGHT,	GVAT_CONDITION|GVAT_SIGNx,	16},	// 96 체결 이퀄라이저	
	{IDS_GH_RECOMBMGOAL,	60,	GVAT_SORTABS,	"1916",	GVFM_RIGHT,	GVAT_CONDITION|GVAT_SIGNx,	17}	// 96 목표가 	
};

// mapnames
#define MAP_CONFIG	"IB202201"	// 설정dll
#define	trUPDOWN	"pidomyst"
#define	trUPDOWN2	"pihomyst"
// tree_setitemdata()

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
		xRECOMMAND  // IBK추천종목
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
		colMAR,
		colSIG,  
		colCODE, 
		colNAME, 
		colCURR
	};
enum _rtmAction { rtmCELL = 0, rtmROW, rtmNONE };
enum _dblAction { dblNONE = 0, dblCURR, dblCHART, dblTRIGGER, dblUSER };

// treeID.depth
#define TD_ROOT			0x00
#define TD_MID			0x01
#define TD_ITEM			0x02


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



class CGridData
{
public:
	CGridData() { Reset(); }
	~CGridData() {}

	void Reset() { m_kind = 0, m_arDatas.RemoveAll() ; }
	void SetKind(UINT kind) { m_kind = kind ; }
	UINT GetKind() { return m_kind; }
	int  GetCount() { return m_arDatas.GetSize(); }
	void operator = (CGridData& input)
	{
		this->SetKind(input.GetKind());
		m_arDatas.Copy(input.m_arDatas);
	}

public:
	
	UINT		m_kind;
	CStringArray	m_arDatas;	// 종목\t보유수량\t가격
};

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

class CRecvData
{
public:
	CRecvData() {}
	~CRecvData() {}
public:
	void SetData(CString userdata, WPARAM wParam, LPARAM lParam)
	{
		m_userdata = userdata;
		m_wParam = wParam;
		m_lParam = lParam;
	}

	CString	m_userdata;
	WPARAM	m_wParam;
	LPARAM	m_lParam;
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
		int	nIndex = m_arValue.Add(value);
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
		int	ncnt = m_arKey.GetSize();
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

struct	_uinfo {
	char	gubn[2];	// 업무구분
				// "MY" : 관심종목
				// "UL" : 사용자 데이터 목록 
				// "UD" : 사용자 데이터
	char	dirt[1];	// 전송방향 'U' : pc==>host, 'D' : host==>pc, 'X' : host==>pc (conversion), 'R' : 재조회
	char	cont[1];	// 연속구분 'F':First, 'M':Middle, 'L':First&Last, 'C':Cancel
	char	name[80];	// file name
	char	nblc[5];	// 블럭개수, 파일크기(max:64k)
	char	retc[1];	// return value 'O':정상 'E':에러
	char	emsg[40];	// error message
};

struct	_ginfo {
	char	gnox[2];	// group number
	char	gnam[20];	// group name
	char	jrec[4];	// _jinfo count
//	char	jptr[1];	// _jinfo pointer
};

struct	_jinfo {
	char	gubn[1];	// 종목구분	0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
	char	code[12];	// 종목코드
	char	xprc[10];	// 보유단가
	char	xnum[10];	// 보유수량
};

/*
struct	_bookmarkinfo {
	char	gubn[1];	// 종목구분	0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
	char	code[12];	// 종목코드
	int		bookmark;	// 북마크 여부
};
*/
struct	_updn {
	_uinfo	uinfo;
	_ginfo	ginfo;
};

const int sz_uinfo   = sizeof(struct _uinfo);
const int sz_ginfo   = sizeof(struct _ginfo);
const int sz_jinfo   = sizeof(struct _jinfo);
const int sz_updn    = sizeof(struct _updn);

struct _marker {
	COLORREF clrMarkerBKG;
	COLORREF clrMarkerTXT;
	BOOL bShadow;
	BOOL bAllApply;
};

#define	trN	"pibo2101"

struct	_inR	{
	char	func[1];		// 'S' : set
							// 'C' : clear
	char	cvol[3];		// 체결량 범위   (cvol * 1,000)
	char	camt[3];		// 체결금액 범위 (camt * 10,000,000)
	char	ador[1];		// 'a' : and
					// 'o' : or
	char	prcf[1];		// 가격 범위 설정 flag
					// 1 or 0
	char	sprc[7];		// 가격범위 시작값
	char	eprc[7];		// 가격범위 끝값
	char	updn[5];		// 상한, 상승, 보합, 하한, 하락
					// 1 or 0
    char    tree[2];            /* 00: 전체                       */
                                /* 01: 거래소,      02: 코스닥      */
                                /* 03: KP200,       04: KRX100      */
                                /* 05: 프리보드,    06: ETF         */
                                /* 07: ELW,         08: ELW기초자산 */
                                /* 나머지 조건은 client에서 처리*/
};

#define	L_inR		sizeof(struct _inR)

struct	_outR	{
	char	retc[1];		// return code
					// 0 : OK
					// 9 : err
};
#define	L_outR		sizeof(struct _outR)

struct	_rtsR	{
	char	flag[3];		// flag (= "W__" : w + blank + blank)
	char	datl[3];		// message length (key length + data length)
};
// + datb		// real-time message
			// key(="quote") \t data(cols \t cols \t ...)
#define	L_rtsR		sizeof(struct _rtsR)

/*******************************************************************************
 * ELW기초자산별
 ******************************************************************************/
struct elw_mid
{
	char iflag[1];		//
	char ASCF[1];		//0:상위 1:하위
	char CBKCOD[1];		//권리유형code 
	char ELWUD[12];		//기초자산code
	char ELWLP[12];		//발행사code
	char JJIS[1];		//잔존일수
	char CO_SORT[2];		//sorting 구분
	char RTSF[9];		//1.RTS 0.NOT
	char C_GVOL[1];		//1:그리드소팅
	char gsrt[1];		
	char dummy[1];
};

struct elw_grid
{
    char    seqn[4];
    char    cod2[7];            /* RTS COD      */
    char    code[6];            /* 종목코드     */
    char    hnam[20];           /* 종목명       */
    char    curr[8];            /* 현재가       */
    char    diff[7];            /* 대비         */
    char    rate[6];            /* 등락율       */
    char    mdga[8];            /* 매도호가     */
    char    msga[8];            /* 매수호가     */
    char    cvol[9];            /* 체결량       */
    char    gvol[9];            /* 거래량       */
    char    gamt[12];           /* 거래대금     */
    char    dvol[8];            /* 순매도잔량   */
    char    svol[8];            /* 순매수잔량   */
    char    irga[7];            /* 이론가       */
    char    grat[7];            /* 괴리율       */
    char    prty[7];            /* 패리티       */
    char    gerr[7];            /* 기어링비율   */
    char    brer[7];            /* 손익분기율   */
    char    cfpv[7];            /* 자본지지점   */
    char    gcjs[18];           /* 기초자산     */
    char    bhsa[20];           /* 발행사       */
    char    jjis[6];            /* 잔존일수     */
    char    hsgg[9];            /* 환산가       */
    char    hsga[9];            /* 행사가격     */
    char    jhre[9];            /* 전환비율     */
    char    delt[9];            /* 델타         */
    char    eger[9];            /* e*기어링     */
};

struct elw_mod
{
	char oflag[1];
	char nrec[4];				//Grid Row 갯수
	struct elw_grid rec[1];
};




/*******************************************************************************
 * 보유종목 조회
 ******************************************************************************/
struct rem_mid
{
	char nrec[5];	// 무조건 1
	char accn[11];	// 계좌번호
	char fill[9];
};

struct o_rec
{
	char code[12];
};

struct rem_mod
{
	char irec[5];
	char accn[11];
	char fill[9];
	char nrec[5];
	struct o_rec grid[100];
};

#define L_rem_mid	sizeof(rem_mid)
#define L_rem_mod	sizeof(rem_mod)
 

/*******************************************************************************
 * 주식잔고 조회
 ******************************************************************************/
struct jango_mid
{
	char nrec[5];	// 무조건 1
	char accn[11];	// 계좌번호
	char fill[9];
	char pass[8];	//password
	char zQryTp;	//조회구분 0;전체 1;ELW 2;ELW제외
	char zD2balBase;	//D2잔고기준조회 0;전체 1;D2잔고0제외
};

struct out_rec
{
	char code[12];					//종목코드
	char marketCode[2];				//등록시장코드
	char qty[16];					//잔고수량
	char todayBuyChegQty[16];		//금일매수체결수량
	char todaySellChegQty[16];		//금일매도체결수량
	char dealbaseQty[16];			//매매기준잔고수량
	char AvrUprc[13];				//평균단가
	char NowPrc[13];				//현재가
	char buyAmt[16];				//매수금액
	char EvalAmt[16];				//평가금액
	char EvalSonikAmt[16];			//평가손익금액
	char sonikRate[12];				//손익률
	char creditAmt[16];				//신용금액
	char loanDt[8];					//대출일
	char balPtnCode[2];				//유가증권잔고유형코드
	char sellAbleQty[16];			//매도가능수량 
	char sellUnercQty[16];			//매도미체결수량
	char loanDtlcode[2];			//대출상세분류코드
	char remainLoanAmt[16];			//잔여대출금액
};

struct jango_mod
{
	char inout[35];
	char irec[5];
	char dummy[187];
	char nrec[5];
	struct out_rec grid[100];
};

#define L_jango_mid	sizeof(jango_mid)
#define L_jango_mod	sizeof(jango_mod)


/*******************************************************************************
 * 선물잔고 조회
 ******************************************************************************/
struct future_mid
{
	char nrec[5];
	char accn[11];	// 계좌번호
	char fill[9];
	char pass[8];	//password
};

struct out_grid
{
	char zFnoIsuNo[32];				//종목번호
	char zIsuNm[40];				//종목명 
	char zIsuMdclssCode[2];			//종목중분류코드
	char zFnoIsuPtnTp[1];			//선물옵션종목유형구분
	char zBnsTp[1];					//매매구분 	
	char dFnoTrdUnitAmt[19];		//선물옵션거래단위금액		
	char lUnsttQty[16];				//미결제수량 
	char lLqdtOrdAbleQty[16];		//청산주문가능수량
	char dAvrUprc[13];				//평균단가
	char lUnsttAmt[16];				//미결제금액
	char dCurprc[13];				//현재가
	char lFnoEvalAmt[16];			//선물옵션평가금액
	char lEvalPnl[16];				//평가손익
	char dPnlRat[12];				//손익률
};

struct future_mod
{
	char inout[33];
	char out0[5];
	char lAllBnsplAmt[16];
	char nrec[5];
	struct out_grid grid[100];
};

#define L_future_mid	sizeof(future_mid)
#define L_future_mod	sizeof(future_mod)

#define FutureService _T("SONBQ021")
#define StockService  _T("SONAQ052")


/*******************************************************************************
* 주식 잔고평가 조회 IO FORMAT
******************************************************************************/
struct  s_mid   {
    char    accn[11];
    char    pswd[8];
    char    cod2[12];
    char    allf[1];    /* 0.신용제외 1.All */
    char    fill[10];
};

struct  s_rec   {
    char    cod2[12];   /* 종목코드         */
    char    hnam[40];   /* 종목명           */
    char    jggb[2];    /* 잔고코드         */
    char    jqty[10];   /* 잔고수량         */
    char    xqty[10];   /* 매도가능         */
    char    pamt[10];   /* 매입평균가       */
    char    mamt[15];   /* 매입금액         */
	
    char    curr[10];   /* 현재가     (*)   */
    char    camt[15];   /* 평가금액         */
    char    tamt[15];   /* 평가손익   (*)   */
    char    srat[10];   /* 평가수익률 (*)   */
    char    sycd[2];    /* 신용코드         */
    char    sydt[8];    /* 대출일           */
    char    samt[15];   /* 신용금액         */
}; 
struct  s_mod   {
    char    accn[11];
    char    nrec[4];
    struct  s_rec   rec[100];
}; 

/*******************************************************************************
* 선물/옵션 잔고평가 조회 IO FORMAT
******************************************************************************/
struct	f_mid	{		// 선물 Input
	char	accn[11];
	char	pswd[8];
	char	date[8];
	char	cod2[8];
};

struct	f_rec	{
	char	cod2[8];	/* 종목코드		*/
	char	hnam[30];	/* 종목명		*/
	char	dsgb[6];	/* 구분			*/
	
	char	bqty[10];	/* 수량			*/
	char	cqty[10];	/* 매도가능수량		*/
	char	pprc[10];	/* 평균가/정산가	*/
	char	curr[10];	/* 현재가		*/
	char	diff[10];	/* 평균가 대비		*/
	char	pamt[15];	/* 평가금액		*/
	char	psam[15];	/* 평가손익		*/
	char	prat[10];	/* 수익률		*/
	char	tamt[15];	/* 매입금액		*/
};

struct	f_mod	{
	char	accn[11];
	char	nrec[4];
	struct	f_rec	rec[100];
};

#define L_smid	sizeof(struct s_mid)				// Length of stock input
#define L_smod	sizeof(struct s_mod) - sizeof(struct s_rec)	// Length of stock output
#define L_srec	sizeof(struct s_rec)				// Length of stock Grid

#define L_fmid	sizeof(struct f_mid)				// Length of Future input
#define L_fmod	sizeof(struct f_mod) - sizeof(struct f_rec)	// Length of Future output
#define L_frec	sizeof(struct f_rec)				// Length of Future Grid
/*
struct i_rec
{
	char zRmnCnfTp[1];				// 실명확인구분 
	char zOtptTp[1];				// 출력구분     
	char zBalTrxTp[1];				// 잔고처리구분 
	char zIntaccNo[20];				// 종합계좌번호 
	char zPwd[8];					// 비밀번호     
};

struct rem_mid
{
	char In[5];						// 레코드갯수   
	struct i_rec rec[99];
};

struct o_remrec
{
	char zAcntNo[20];				// 계좌번호             
	char zAcntNm[40];				// 계좌명               
	char zIsuNo[12];				// 종목번호             
	char zIsuNm[40];				// 종목명               
	char zTp[1];					// 구분                 
	char zTpNm1[40];				// 구분명1              
	char lHldQty[15];				// 보유수량             
	char dAvrUprc[9];				// 평균단가             
	char dCurPrc[9];				// 현재가               
	char lEvalAmt[15];				// 평가금액             
	char dEvalRat[3];				// 평가비율             
	char lEvalPnlAmt[15];			// 평가손익금액         
};

struct o_prcrec
{
	char zAcntNoClssPtnCode[2];		// 계좌번호분류유형코드 
	char zAcntNo[20];				// 계좌번호             
	char zAcntNm2[40];				// 계좌명               
	char zPdptnCode[2];				// 상품유형코드         
	char zPdptnNm[40];				// 상품유형명           
	char lPrsmptDpsD2[15];			// 추정예수금(D+2)      
	char lFnoEvalAmt[15];			// 선물옵션평가금액     
	char lFnoEvalPnlAmt[15];		// 선물옵션평가손익금액 
	char lStkEvalAmt[15];			// 주식평가금액         
	char lStkEvalPnlAmt[15];		// 주식평가손익금액     
	char lBndEvalAmt[15];			// 채권평가금액         
	char lBndEvalPnlAmt[15];		// 채권평가손익금액     
	char lIvtrstEvalAmt[15];		// 투자신탁평가금액     
	char lIvtrstEvalPnlAmt[15];		// 투자신탁평가손익금액 
	char lEtcSecEvalAmt[15];		// 기타증권평가금액     
	char lEtcSecEvalPnlAmt[15];		// 기타증권평가손익금액 
	char lCrdtMloanAmt[15];			// 신용융자금액         
	char lLoanAmt[15];				// 대출금액             
	char lSloanPldgAmt[15];			// 대주담보금액         
	char lIndvAcntSum[15];			// 개별계좌소계         
	char SecDtl[5];					// 레코드갯수           
	struct o_remrec rec[15];
};

struct o_rec
{
	char zAcntNm[40];				// 계좌명               
	char zClntNm[40];				// 고객명               
	char BalSt[5];					// 레코드갯수           
	struct o_prcrec rec[1];
};

struct rem_mod
{
	char Out[5];					// 레코드갯수           
	struct o_rec rec[1];
};

#define L_rem_mid	sizeof(rem_mid)
#define L_i_rec		sizeof(i_rec)
#define L_rem_mod	sizeof(rem_mod)
#define L_o_rec		sizeof(o_rec)
#define L_o_prcrec	sizeof(o_prcrec)
#define L_o_remrec	sizeof(o_remrec)

*/

