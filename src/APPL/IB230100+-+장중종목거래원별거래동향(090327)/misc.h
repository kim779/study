#pragma once

#define	clBackIn	66
#define	clBack		64
#define	clcontentsBack		181
#define clHeadText	130
#define	clText		69
#define	clUp		94
#define	clDown		95
#define	clHead		74
#define	clHeadBack	77
#define	clHText		76
#define	clData		68
#define	clLine		65
#define	clRow1		66
#define	clRow2		67
#define	clBuyH		85
#define	clSellH		82
#define	clBuy		85
#define	clSell		82
#define	clSText		81
#define clBuyhead   164
#define clCellhead  162

#define	IDC_CODE	10001
#define	IDC_SEARCH	10002
#define	IDC_SYM		10003
#define	IDC_STOP	10004
#define	IDC_INQ		10005
#define	IDC_TBL1	10006
#define	IDC_TBL2	10007
#define	IDC_GRID	10008
#define	IDC_SCHG	10009
#define	IDC_SVOL	10010
#define	IDC_BVOL	10011
#define	IDC_BCHG	10012

#define	hROW		18

//
//	table columns
//
#define	nROWx		6
#define	nCOLx		5

#define	iSCHG		0		// 매도 변동량
#define	iSVOL		1		// 매도 수량
#define	iSVOLp		2		// 매도 수량 (예전치)
#define	iSNAM		3		// 매도 거래원
#define	iSCOD		4		// 매도 거래원 코드

#define	iBCOD		0		// 매수 거래원 코드
#define	iBNAM		1		// 매수 거래원
#define	iBVOL		2		// 매수 수량
#define	iBVOLp		3		// 매수 수량 (예전치)
#define	iBCHG		4		// 매수 변동량

struct	_tbls	{
	char*	heads;
	int	width;
	int	hclr;
	int	dclr;
	int	tclr;
	DWORD	format;
	DWORD	attr;
} tbls1[nCOLx] = {
	{ "변동량",	72,	clHeadBack,	clSell,		clText,	GVFM_RIGHT,	GVAT_COMMA		},	// 0	매도 변동량
	{ "매도수량",	85,	clHeadBack,	clSell,		clText,	GVFM_RIGHT,	GVAT_COMMA		},	// 1	매도 수량
	{ "매도수량p",	0,	clHeadBack,	clSell,		clText,	GVFM_RIGHT,	GVAT_HIDDEN		},	// 2	매도 수량 (예전치)
	{ "매도상위",	98,	clHeadBack,	clSell,		clText,	GVFM_CENTER,	GVAT_CONDITIONx		},	// 3	매도 거래원
	{ "매도코드",	0,	clHeadBack,	clSell,		clText,	GVFM_LEFT,	GVAT_HIDDEN		},	// 4	매도 거래원 코드	
};

struct	_tbls	tbls2[nCOLx] = {
	{ "매수코드",	0,	clHeadBack,		clBuy,		clText,		GVFM_LEFT,	GVAT_HIDDEN		},	// 0	매수 거래원 코드
	{ "매수상위",	97,	clHeadBack,		clBuy,		clText,		GVFM_CENTER,	GVAT_CONDITIONx		},	// 1	매수 거래원
	{ "매수수량",	85,	clHeadBack,		clBuy,		clText,		GVFM_RIGHT,	GVAT_COMMA		},	// 2	매수 수량
	{ "매수수량p",	0,	clHeadBack,		clBuy,		clText,		GVFM_RIGHT,	GVAT_HIDDEN		},	// 3	매수 수량 (예전치)
	{ "변동량",	72,	clHeadBack,		clBuy,		clText,		GVFM_RIGHT,	GVAT_COMMA		},	// 4	매수 변동량
};

struct	_syms	{
	char*	sym;
	char*	rts;
	int	flag;
	int	row;
	int	col;
} syms[] = {
	{ "1021",	"021",		-1,	0,	0	},	// rts key
	{ "17413",	"",		-2,	0,	0	},	// symbol control
	{ "2179",	"179",		0xff,	0,	0	},	// static - schg
	{ "2176",	"176",		0xfe,	0,	0	},	// static - svol
	{ "2177",	"177",		0xfd,	0,	0	},	// static - bvol
	{ "2180",	"180",		0xfc,	0,	0	},	// static - bchg

	{ "2136",	"",		0,	1,	iSCHG	},	// 매도변동량
	{ "2137",	"",		0,	2,	iSCHG	},
	{ "2138",	"",		0,	3,	iSCHG	},
	{ "2139",	"",		0,	4,	iSCHG	},
	{ "2140",	"",		0,	5,	iSCHG	},
	{ "2131",	"131",		0,	1,	iSVOL	},	// 매도수량
	{ "2132",	"132",		0,	2,	iSVOL	},
	{ "2133",	"133",		0,	3,	iSVOL	},
	{ "2134",	"134",		0,	4,	iSVOL	},
	{ "2135",	"135",		0,	5,	iSVOL	},
	{ "2121",	"121",		0,	1,	iSNAM	},	// 매도거래원
	{ "2122",	"122",		0,	2,	iSNAM	},
	{ "2123",	"123",		0,	3,	iSNAM	},
	{ "2124",	"124",		0,	4,	iSNAM	},
	{ "2125",	"125",		0,	5,	iSNAM	},
	{ "2126",	"126",		0,	1,	iSCOD	},	// 매도거래원 코드
	{ "2127",	"127",		0,	2,	iSCOD	},
	{ "2128",	"128",		0,	3,	iSCOD	},
	{ "2129",	"129",		0,	4,	iSCOD	},
	{ "2130",	"130",		0,	5,	iSCOD	},
	{ "2156",	"156",		1,	1,	iBCOD	},	// 매수거래원 코드
	{ "2157",	"157",		1,	2,	iBCOD	},
	{ "2158",	"158",		1,	3,	iBCOD	},
	{ "2159",	"159",		1,	4,	iBCOD	},
	{ "2160",	"160",		1,	5,	iBCOD	},
	{ "2151",	"151",		1,	1,	iBNAM	},	// 매수거래원
	{ "2152",	"152",		1,	2,	iBNAM	},
	{ "2153",	"153",		1,	3,	iBNAM	},
	{ "2154",	"154",		1,	4,	iBNAM	},
	{ "2155",	"155",		1,	5,	iBNAM	},
	{ "2161",	"161",		1,	1,	iBVOL	},	// 매수수량
	{ "2162",	"162",		1,	2,	iBVOL	},
	{ "2163",	"163",		1,	3,	iBVOL	},
	{ "2164",	"164",		1,	4,	iBVOL	},
	{ "2165",	"165",		1,	5,	iBVOL	},
	{ "2166",	"",		1,	1,	iBCHG	},	// 매수변동량
	{ "2167",	"",		1,	2,	iBCHG	},
	{ "2168",	"",		1,	3,	iBCHG	},
	{ "2169",	"",		1,	4,	iBCHG	},
	{ "2170",	"",		1,	5,	iBCHG	},
};
#define	L_syms		sizeof(syms)/sizeof(struct _syms)

//
//	grid columns
//
#define	nROW		30
#define	nCOL		9

#define	iCODE		0	// 코드
#define	iNAME		1	// 거래원
#define	iSVOLx		2	// 매도수량
#define	iBVOLx		3	// 매수수량
#define	iBBVL		4	// 순매수량
#define	iBBVLp		5	// 순매수량 (예전치)
#define	iCHAG		6	// 변동량
#define	iTIME		7	// 최종시간
#define	iRATE		8	// 비중

//20011.11.16 KSJ
//변동량 GVAT_CONDITION 추가

struct	_infos {
	char*	heads;
	int	width;
	int	dclr;
	int	tclr;
	DWORD	format;
	DWORD	attr;
} infos[nCOL] = {
	{ "코드",	0,	clData,		clText,		GVFM_CENTER,	GVAT_HIDDEN			},	// 0	코드
	{ "거래원",	80,	clData,		clText,		GVFM_LEFT,	GVAT_CONDITIONx			},	// 1	거래원
	{ "매도수량",	70,	clSell,		clSText,	GVFM_RIGHT,	GVAT_COMMA			},	// 2	매도수량
	{ "매수수량",	70,	clBuy,		clText,		GVFM_RIGHT,	GVAT_COMMA			},	// 3	매수수량
	{ "순매수량",	70,	clData,		clText,		GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION	},	// 4	순매수량
	{ "순매수량p",	0,	clData,		clText,		GVFM_RIGHT,	GVAT_HIDDEN			},	// 5	순매수량 (예전치)
	{ "변동량",	60,	clData,		clText,		GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION			},	// 6	변동량
	{ "최종시간",	60,	clData,		clText,		GVFM_CENTER,	0				},	// 7	최종시간
	{ "비중",	60,	clData,		clText,		GVFM_RIGHT,	0				},	// 8	비중
};
#define	GRID_WIDTH		470

//========================================================================================================
//
//	I/O Format
//

#define	gridTR	"pino2301"
#define	oopTR	"pooppoop"
#define	gKEY	1
#define	oKEY	0

struct	_inR	{
	char	code[12];		// 종목코드
};
#define	L_inR		sizeof(struct _inR)

struct	_recR	{
	char	code[3];		// 거래원코드
	char	name[20];		// 거래원명
	char	dvol[9];		// 매도량
	char	svol[9];		// 매수량
	char	smsu[9];		// 순매수량
	char	chag[9];		// 변동량
	char	time[6];		// 수신시간
	char	rate[5];		// 비중
};
#define	L_recR		sizeof(struct _recR)

struct	_outR	{
	char	rts[7];			// rts code
	char	nrec[4];		// nrec
};
#define	L_outR		sizeof(struct _outR)


