#pragma once

#define	IDC_GRID	10001
#define	IDC_SETUP	10002
#define	IDC_STOP	10003
#define IDC_PANEL	10004
#define IDC_PANEL2	10004

#define	hROW		18
#define	nROW		300
#define	nCOL		9

#define	iCODE		0	// 코드
#define	iTIME		1	// 시간
#define	iNAME		2	// 종목명
#define	iCURR		3	// 현재가
#define	iDIFF		4	// 대비
#define	iRATE		5	// 등락률
#define	iQTY		6	// 거래량
#define	iCVOL		7	// 순간체결량
#define	iCAMT		8	// 순간체결금액

#define SAVE_HEIGHT "HEIGHT"

#define GAP_TOP			3
#define GAP_SIDE		1
#define GAP_BOTTOM		3
#define GAP_BLOCK		5
#define GAP_PANEL		4

#define PANAL_HEIGHT	26
#define BUTTON_HEIGHT	20
#define STOPBT_WIDTH	36
#define SETUPBT_WIDTH	84
#define INCREASE_WIDTH	7
#define INCREASE_HEIGHT	2

#define	MAP_WIDTH	568 + 2 * (GAP_SIDE + GAP_PANEL)
#define	MAP_HEIGHT	385 + GAP_TOP + GAP_BOTTOM + GAP_BLOCK + 2 * GAP_PANEL

#define WINDOW_RECT		CRect(0, 0, MAP_WIDTH, MAP_HEIGHT)

#define ROUND_PANEL		1
#define ROUND_CONTENTS	2

#define	clBack	64
#define clBox	65
#define	clText	69
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
	{ "코드",		60,	GVFM_CENTER,	0					},	// 0	코드
	{ "시간",		0,	GVFM_CENTER,	GVAT_HIDDEN				},	// 1	시간
	{ "종목명",		100,	GVFM_LEFT,	0					},	// 2	종목명
	{ "현재가",		70,	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx		},	// 3	현재가
	{ "대비",		60,	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONTRAST		},	// 4	대비
	{ "등락률",		60,	GVFM_RIGHT,	GVAT_COMMA|GVAT_FLOAT2|GVAT_CONDITION	},	// 5	등락률
	{ "거래량",		80,	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx		},	// 6	거래량
	{ "체결량",		80,	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx		},	// 7	순간체결량
	{ "금액(만)",	70,	GVFM_RIGHT,	GVAT_COMMA				},	// 8	순간체결금액
};


//========================================================================================================
//
//	I/O Format
//

#define	trN	"pibf2101" //2012.11.13 KSJ pibo --> pibf로 수정

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
    char    tree[15][2];        /* 00: 전체                         */
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
