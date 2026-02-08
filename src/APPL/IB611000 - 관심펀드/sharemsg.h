#pragma once
#include "../IhUtils.h"
#include "../../axis/axMsg.hxx"
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "../../h/memDc.h"
#define WM_MANAGE	(WM_USER + 1000)
//#include "griditem.h"

/////////////////////////////////////////////////////////////////
//////  WM_MAINWND	Msg Kind
/////////////////////////////////////////////////////////////////
// LOWORD(wParam)

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
#define KEY_CBARRANGE	"CBARRANGE"

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
#define IDC_CB_ARRANGE		1053
#define IDC_MENU_UPJONGSORT	1054
#define IDC_MENU_NAMESORT	1055
#define IDC_MENU_CODESORT	1056
#define IDC_TREE		1100
#define IDC_BASKET		1200
#define IDC_GROUP		1300
#define IDC_GRIDWND		1400
#define IDC_GRID		1500
#define IDC_RTSGRID		1600
#define IDC_EXPAND		1700





 #define DEF_FIELD_CONTENT1	_T("필드 00;2024;2033;2027;7777;")

// file names
#define FILE_CONFIG	"intercfg.ini"		// 뉴관심종목에서 새로이 만들어진 관심종목의 전체적인 환경을 저장하는 파일
#define FILE_GROUP	"FDportfolio.ini"		// 관심펀드의 그룹을 저장하는 파일
#define FILE_SUBDATA	"FDportfolio.i"		// 확장자 뒤에 두자리는 숫자로 저장된다.
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

#define DetailFund "IB874200"


