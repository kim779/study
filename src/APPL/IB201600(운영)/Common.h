#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>

using namespace std;

// Ledger filler
#define ORDER_INFO_IDX          (0x10)

#ifdef _DEF_IB300600_
	#define CONFIG_INI     "%s\\user\\%s\\IB300600.ini"
#else
	#define CONFIG_INI     "%s\\user\\%s\\IB191000.ini"
#endif

#define MAP_GROUPEDIT  "IB191010 /S/s/p5/t"
#define MAP_ORDERLIMIT "IB0000B2 /S/s/p5/t"
#define MAP_CHEG       "IB310200 /S/s/p5/t"
#define MAP_CHART      "IB720000 /S/s/p5/t"
#define MAP_JANGO      "IB320100 /S/s/p5/t"

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

#define WM_APP_REFRESH		(WM_USER+0xF1)
#define WP_MICHEG			(0x00000001)
#define WP_JANGO			(0x00000002)
#define WP_RESERVE			(0x00000003)
#define WP_ACCOUNT			(0x00000004)
#define WP_MICHEG_INSERT    (0x00000005)
#define WP_MICHEG_UPDATE    (0x00000006)
#define WP_MICHEG_DELETE    (0x00000007)
#define WP_ORDER_RESULT     (0x00000008)

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

