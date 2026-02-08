#ifndef	DEFINFO_H_
#define DEFINFO_H_

#include "../../h/axisgenv.h"

/////////////////////////////////////////////////////////////////
// draw component info

enum { btnNOT = 0, btnDown, btnUp, btnMove, btnDis };

#define	DATEIMAGE	"GTI_DATE.bmp"
#define	TIMEIMAGE	"GTI_TIME.bmp"
#define	NTIMEIMAGE	"GTI_NTIME.bmp"
#define	NBTNIMAGE	"GTI_NBTN.bmp"
#define	CODEIMAGE	"GTI_CODE.bmp"
#define	IETCIMAGE	"GTI_ETC.bmp"
#define	IOPTIMAGE	"GTI_OPT.bmp"
#define	ICFGIMAGE	"GTI_CFG.bmp"
#define	TOOLIMAGE	"GTT_TOOL.bmp"
#define	TOOL2IMAGE	"GTT_TOOL2.bmp"
#define	CTRLIMAGE	"GTC_CTRL.bmp"
#define	CTRL2IMAGE	"GTC_CTRL2.bmp"
#define IUNITIMAGE	"GTI_UNIT.bmp"

struct	_comp
{
	int	cID;		// component ID
	CRect	cRC;		// component Rect
	int	status;		// component status
	bool	bshow;		// bool show
	int	imggen;		// unselect image
	int	imgsel;		// select image
	int	imgmov;		// move image
	int	imgdis;		// disable image
	char	str[11];	// image is not => text
	char	comment[32];	// comment
};
#define sz_COMP		sizeof(struct _comp)

#define	IC_EDIT	99
enum {	IC_SCROLL, IC_SLIDER, IC_EXPAND, IC_REDUCE, IC_DISPLAY,
	IC_BEFORE, IC_STOP, IC_NEXT, IC_EXCEL, IC_IMAGE,
	IC_GRID, IC_TABLE, IC_LINK1, IC_LINK2, IC_LINK3,
	IC_LINK4};

enum {	BTN_CFG = 0, CMB_UNIT, /** macho add **/BTN_UNIT, CMB_KKIND, CMB_CODE, BTN_CODE,
	BTN_K200, BTN_FUTURE,
	BTN_FO1, BTN_FO2, BTN_FO3, BTN_FO4, BTN_FO5,
	/*CMB_SKIND, */BTN_DDAY, BTN_DWEEK, BTN_DMON,
	BTN_DMIN, BTN_DTICK, BTN_DOLLAR, CMB_TIME, BTN_TIME1,
	BTN_TIME2, BTN_TIME3, BTN_TIME4, BTN_TIME5, STC_GIGAN, EDT_GIGAN,
	STC_DISP, EDT_DISP, CALENDAR, CHK_TODAY, BTN_MENU,
	BTN_SAVE, BTN_TOOL, STC_JNAME};


struct _info	{
	BYTE	kind;
	BYTE	imggen;
	BYTE	imgmov;
	BYTE	imgsel;
	BYTE	imgdis;
	char	disp[11];
	char	com[32];
};
#define	sz_INFO		sizeof(struct _info)

#define	sz_JCODE	6
#define	sz_FCODE	8
#define	sz_OPTION	8
#define	sz_INDEX	3	// 2006.11.16 lsh 2->3  

/**
#define	upKospi		0
#define	upKos200	10
#define	upKos100	20
#define	upKosdaq	90
#define	upKq50		91
#define upKoStar	92	// add macho@2005.11.03
#define upKrx100	93	// Added by schbang 2006.02.20
#define upSector	94	// Added by schbang 2006.02.15
**/
#define upKospi		001
#define upKos200	101
#define upKos100	401
#define upKosdaq	201
#define upKq50		301
#define upKoStar	303
#define upKrx100	999
#define upSector	501
#define upFreeboard 601


#define	fnKind1		0
#define	fnKind2		1
#define	fnKind3		2
#define	fnKind4		3
#define	fnKind5		4

/**
#define	fnCode1		".DJI"
#define	fnCode2		"COMP"
#define	fnCode3		"SPX"
#define	fnCode4		"SOX"
#define	fnCode5		"1DNH"
**/
#define fnCode1		"DJI@DJI"
#define fnCode2		"NAS@IXIC"
#define fnCode3		"SPI@SPX"
#define fnCode4		"USI@SOXX"
#define fnCode5		"CME$ND"

#define	JTRG	"1301"		// 주식
#define	FTRG	"30301"		// 선물
#define	OTRG	"40301"		// 옵션
#define	CFTRG	"60301"		// 주식선물
#define	COTRG	"70301"		// 주식옵션

// 20090915 : ADD
#define CDFTRG	"100301"	// 상품선물(POOP)
#define CDOTRG	"110301"	// 상품옵션(POOP)

#define	FOTRG	"ed_focod"	// 선물,옵션,주식선물,주식옵션 통합
#define	STRG	"gscode"	// 해외
#define	ITRG	"gicode"	// 업종 (xx(구분) + xx(코드))

/////////////////////////////////////////////////////////////////


#endif

