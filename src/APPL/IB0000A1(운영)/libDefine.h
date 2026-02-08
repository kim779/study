#pragma once

#include "xxgrid.h"

// grid 
struct _grInfo {
	char*	heads;
	int	width;
	char*	symbols;
	UINT	format;
	DWORD	attr;
	char*	defaultVal;
	int	param;
};
#define GRIDROW_HEIGHT  18


// grInfoEdit : grid column define
static struct _grInfo grInfo[] = {
	//{ "대표",	0,	"A001",	GVFM_CENTER,	GVAT_HIDDEN|GVAT_ONLYROW|GVAT_CHECK,	"",	0},
	{ "번호",	40,	"A002",	GVFM_RIGHT,	GVAT_ONLYROW,			"",	0},
	{ "계좌번호",	110,	"A003",	GVFM_CENTER,	GVAT_ONLYROW|GVAT_SORTSTR,	"",	0},
	{ "고객명",	60,	"A004",	GVFM_LEFT,	GVAT_ONLYROW|GVAT_SORTSTR,	"",	0},
	{ "비밀번호",	56,	"A005",	GVFM_LEFT,	GVAT_ONLYROW|GVAT_EDIT,		"",	0},
	//{ "비율",	0 /*50*/,	"A006",	GVFM_RIGHT,	GVAT_ONLYROW|GVAT_EDIT|GVAT_SORTVAL|GVAT_HIDDEN,	"",	0},
	{ "계좌별칭",	80,	"A007",	GVFM_LEFT,	GVAT_ONLYROW|GVAT_SORTSTR|GVAT_EDIT,"",	0},
	{ "검증결과",   60,    "A008", GVFM_LEFT,	GVAT_ONLYROW|GVAT_SORTSTR|GVAT_EDIT,"",	0},
};
enum { IECOL_PRI = 0, IECOL_SEQ, IECOL_ACCNUM, IECOL_ACCNAME, IECOL_PASSWORD, IECOL_RATE, IECOL_ALIAS };

//===========================================================================================================
const int MAX_SEND = 14*1024;			// 최대 send size(14K넘는경우)

// Send Sub Key
#define SC_ACCCUST		100		// 고객계좌조회
#define SC_ACCLIST		101		// 직원계좌조회
#define SC_ACCLISTBYGROUP	102		// 그룹계좌조회
#define SC_SAVEACC		103		// 그룹계좌저장(화면닫음)
#define SC_SAVEACCONLY		104		// 그룹계좌저장(저장만함)
#define	SC_INPUTACCOUNTVALID	106		// 입력 계좌 비번 검사
#define	SC_ACCOUNTNAME		107		// 계좌검증
#define	SC_ACCPASSVALID		109		// 계좌 유효성검사(비번edit입력시)
#define	SC_DLGACCOUNTNAME	111		// 계좌명조회

// macro
#define	LIB_DELETE(XXX) { delete XXX; XXX = NULL; }
#define	BUF_DELETE(XXX)	{ delete[] XXX;  XXX = NULL; }

// IDC define
#define	IDC_XXGRID		1001

// section
#define	GRACC			"GRACC"
#define	GREDIT			"GREDIT"

// screen
//**#define	SCRWIDTH	748
//**#define	SCRHEIGHT	458

// COLOR
#define	CLRBG		64			// background color

#define	WM_ACCSEARCH	WM_USER + 110		// 계좌명 조회


// ledger
#define LEDGER_ECODE(param) ((_ledgerH*)param)->ecod
#define LEDGER_CONF(param)  ((_ledgerH*)param)->next[0]


