#pragma once


#include "../../h/memDC.h"
#include "../../h/axisvar.h"
#include "../../h/jmcode.h"
#include "../../h/interSt.h"
#include "../../h/symbol.h"
#include "libFunction.h"

// history 조회
const	char lkHis =	'1';
#define	CATCHMAP	"IBXXXX09"

// section name
#define	SECNAME		"CATCHENV"
// key name
#define	SAVEOPTION	"SAVEOPTION"		// 저장여부
// default value
#define	VALOPTION	0			// 저장 0:저장, 1:저장안함


#define	WM_LOCAL		WM_USER + 202	// local
#define	WM_PRICE		WM_USER + 203	// cx_price
#define	EVT_CHAR		1		// edit에 up, down 화살표키 입력시
#define	EVT_TEXT		2		// string text

#define	SC_SISE		1			// 시세조회
#define	SC_SISEDBCLICK	2			// 시세조회(double click시)
#define	SC_HISTORY	3			// 시세포착 log 조회



#define	SCRWIDTH	943
#define	SCRHEIGHT	392

#define	CLRBG			64		// background
#define	CLRGRROWB1		68		// grid row1 back
#define	CLRGRROWB2		77		// grid row2 back
#define	CLRGRROWF		69		// grid row fore
#define	CLRGRROWSF		56		// grid row fore(special)
#define	CLRGRHB			74		// grid head back
#define	CLRGRHSB		0		// grid head select
#define	CLRGRHF			76		// grid head fore
#define	CLRGRSEL		78		// grid select
#define	CLRGRLINE		65		// grid line
#define CLRBLINK		8		// blink
#define	CLRUP			161		// 이상
#define	CLRDN			162		// 이하
#define CLRTITLE		PALETTERGB(255, 255, 170)	// Y 표시 있을때 색상
#define	CLRMDT			82		// 매도 title
#define	CLRMST			85		// 매수 title
#define	CLRMDF			95		// 매도 font
#define	CLRMSF			94		// 매수 font

#define	CATCHPATH	"catch.ini"
#define	FILE_SAVE	"portfolio.ini"
const int maxGROUP = 100, maxJONGMOK = 80;

enum { CG_NOT = 0, CG_KOSPI, CG_KOSDAQ, CG_FUTURE, CG_CALL, CG_PUT };

// macro
#define	LIB_DELETE(XXX) { delete XXX; XXX = nullptr; }
#define	BUF_DELETE(XXX)	{ delete[] XXX; XXX = nullptr; }

#define	IDC_GRSISE		100
#define	IDC_GRCATCH		101
#define	IDC_GRSETUPSISE		102
#define	IDC_GRSETUPTITLE	103
#define	IDC_GRALARM		104
#define	IDC_STCCODE		105
#define	IDC_GRETC		106
#define	IDC_GRHISTORY		107

const	int	MAX_ETCCOL = 4;			// etc column수
const	int	MAX_ETCROW = 3;			// etc row
const	int	MAX_ALARM = 6;			// max alarm수

#define	CONFIRM_MARK		"Y"

const	int	SISEROW = 1;			// sise data display row
enum { ETC_ROW_CTITLE = 0, ETC_ROW_CDATA, ETC_ROW_JTITLE, ETC_ROW_JDATA };
enum { ETC_COL_CONFIRM = 0, ETC_COL_INTER, ETC_COL_SOUND, ETC_COL_JUMUN };
enum { ETC_COL_MMGB = 0, ETC_COL_JMGB, ETC_COL_NUM, ETC_COL_PRICE };

// 관심종목 등록
const char	gnCHAR = 0x7f;
const char	tabCHAR	= 0x09;
const char	lfCHAR	= 0x0A;
#define	configMAP	"IB202201"		// config map

enum tId { xNONE = 0, xALLCODE, xKOSPI, xKOSDAQ, xINTEREST, xTHEMA, xGROUP, xFUTURE, xCALL, xPUT, xETF, xELW};
//======================================================================
// grid 
//======================================================================
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
#define GRIDROW_HEIGHT2 16
	
