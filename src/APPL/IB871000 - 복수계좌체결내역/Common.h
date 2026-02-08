#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

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

#define CODECTRL_WIDTH		(80)
#define HISTORY_WIDTH		(600)
#define HISTORY_ITEM_HEIGHT	(20)

// Control 윈도우 높이
#define CONTROL_WND_HEIGHT	(31)

// GUIDE 윈도우 높이
#define GUIDE_WND_HEIGHT		(22)

// SCROLLBAR 넓이
#define SCROLLBAR_WIDTH		(18)

// 그리드 셀 높이
#define GRID_CELL_HEIGHT	(18)

#define MAX_FHOGA			(5)
#define FCODE_LEN			(8)
#define DOUBLE_PREFIX		(0.00005)

typedef enum { CT_NONE=0, CT_FUTURE, CT_OPTION, CT_SPREAD } CODE_TYPE;

