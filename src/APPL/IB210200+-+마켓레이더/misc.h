#pragma once

#define GAP_TOP			3
#define GAP_SIDE		1
#define GAP_BOTTOM		3
#define GAP_BLOCK		5
#define GAP_PANEL		4
#define GAP_CTRL		6

#define	MAP_WIDTH	447 + 2 * GAP_SIDE + 2 * GAP_PANEL
#define	MAP_HEIGHT	386 + GAP_TOP + GAP_BOTTOM + GAP_BLOCK + 2 * GAP_PANEL

#define PANAL_HEIGHT	26
#define BUTTON_HEIGHT	20
#define BUTTON_WIDTH	36
#define BUTTON_WIDTH_INQ 37

#define MAP_RECT CRect(0, 0, MAP_WIDTH, MAP_HEIGHT)

#define	IDC_CODE	10001
#define	IDC_SEARCH	10002
#define	IDC_SYM		10003
#define	IDC_STOP	10004
#define	IDC_INQ		10005
#define	IDC_SETUP	10006
#define	IDC_NEXT	10007
#define	IDC_GRID	10008
#define IDC_PANEL	10009
#define IDC_PANEL2	10010

#define JONGMUK_CODE	"CODE"

#define	hROW		18
#define	nROWx		20
#define	nROW		1000
#define	nCOL		5

#define	iCODE		0
#define	iTIME		1
#define	iNAME		2
#define	iDESC		3
#define	iMAPS		4

#define INCREASE_WIDTH 12
#define INCREASE_HEIGHT 2


#define ROUND_PANEL		1
#define ROUND_CONTENTS	2


#define	clBack	64
#define clBox	65
#define	clText	130
#define	clUp	94
#define	clDown	95

#define clBtnText 186
//판넬
#define clPanel	66
#define clPanelLine 165

//contents 외곽
#define clContents 181
#define clGuide	26

//tab
#define	clTabf	70
#define	clTabsf	71

//grid
#define	clHead	74
#define	clHText	76
#define	clData	68
#define	clLine	93
#define	clFocus	78
#define	clRow1	68
#define	clRow2	77
#define clSel	78

//
//	grid columns
//
struct	_infos {
	char*	heads;
	int	width;
	DWORD	format;
	DWORD	attr;
} infos[nCOL] = {
	{ "코드",	0,	GVFM_CENTER,	GVAT_HIDDEN	},	// 0	코드
	{ "시간",	40,	GVFM_CENTER,	0		},	// 1	시간
	{ "종목명",	80,	GVFM_LEFT,		0 },	// 2	종목명
	{ "특이사항",	310,	GVFM_LEFT,	0		},	// 3	특이사항
	{ "MAP",	0,	GVFM_LEFT,	GVAT_HIDDEN	},	// 4	mapname
};

#define	nROWx		20
//========================================================================================================
//
//	I/O Format
//

#define	watchTR	"pihoscrn"
#define	oopTR	"pooppoop"
#define	wKEY	1
#define	oKEY	0
#define	maxR	20

struct	_inR	{
	char	code[6];		// code
	char	data[4][10];		// flag data
	char	keys[7];		// next key
	char	stat[8];		// 대상제외조건
	char    jchk[8];		// kospi,kosdaq,프리보드,ELW, 4byte(fill)
	char	gvol[4];		// 거래량 (천주)
};
#define	L_inR		sizeof(struct _inR)

#define i_sKK	0	// 관리종목
#define	i_sGJ	1	// 거래정지
#define	i_sTY	2	// 투자유의
#define i_sYS	3	// 우선주
#define i_sGG	4	// 감리종목
#define i_sJL	5	// 정리매매
#define i_sBS	6	// 불성실공시
#define i_sHK	7	// 현금증거금100%


struct	_recR	{
	char	code[7];		// code			021
	char	data[10];               // flag			801
	char	time[4];                // time (HHMM)		802
	char	name[16];               // name			803
	char	desc[80];               // description		804
};
#define	L_recR		sizeof(struct _recR)


struct	_outR	{
	char	keys[7];		// next key
					// if next exists
	char    nrec[4];                // # of records
};
#define	L_outR		sizeof(struct _outR)


