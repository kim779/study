
#pragma once

#define				JCOLCOUNT			59
#define				GRIDROWHEIGHT		20

enum _colINDEX
{
	colSET=0,//0
	colCHECK,//1
	colCDTYPE,//2
	colCODE,//3
	colRCOD,//4
	colNAME, //5

	colCRETYPE,//6

	colJANTYPE,//7 잔고유형
	colVOL,//잔고8
	colPOSSVOL,//가능9
	colCOST,//10 매입가
	colAMOUNT,//11
	colCURR,//12
	colBEFORERATE,
	colUPDOWNRATE,
	colPROFITPRC,//13
	colPROFIT,//14
	colRATE,//15

	colCRTCD,//16
	colCRTDATE,//17
	colCRTPRC,//신용금액 18
	

	colTMP1,//19

	colLOSS,//손실목표20
	colLSCFG, //21
	colMIT,//이익목표  22
	colMTCFG,//23
	colTS,//이익보존 24
	colTSCFG,

	colTSDIF, //고점대비하락
	colTSDIFCFG,
	colTSMAX,//고점

//	colCRTPRC2,
	colHOGATYPE,
	colSTATE,
	colEXEC,
	colJPRICE,
	colJVOL,
	colJVOLCASE,
	colCVOL,
	colJNO,
	colJMSG,

	//상한가
	colVolUNIT,
	colSISE,
	colPCUR,

};


static struct	_gridHdr {
	char	szhdr[50];
	UINT	width;		// width
	UINT	attr;		// attribute
	char	symbol[8];	// symbol
	UINT	format;		// format
	UINT	attrx;		// attr
	bool	c_merge;
	bool	r_merge;
	char	szhdr0[50];
	int		c_flag1;
	int		c_flag2;
	int		itogglecol;
	double  lcolrate;
} gridHdr[] = 
{
	{"설정",		30,		GVAT_TEXT,		"",		GVFM_CENTER,	GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.0316},	
	{"감시",		26,		GVAT_CHECK,		"",		GVFM_RIGHT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.02745},	
	{"T",			0,		GVAT_HIDDEN,		"",		GVFM_LEFT,		0,							0, 1,		"",							0, 0, 0, 0.f},	

	{"종목코드",	0,		GVAT_TEXT,		"",		GVFM_LEFT,		0,							0, 1,		"",							0, 0, 0, 0.f},	
	{"리얼코드",	0,		GVAT_HIDDEN,	"1021",	GVFM_LEFT,		0,							0, 1,		"",							0, 0, 0, 0.f},	
	{"종목명",		100,	GVAT_TEXT,		"",		GVFM_LEFT,		0,							1, 1,		"",							0, 0, 3, 0.11},	
	{"",			53,		GVAT_TEXT,		"",		GVFM_CENTER,	GVAT_TOOLTIP,							1, 1,		"",							0, 0, 0, 0.0559},	

	{"잔고유형",	0,		GVAT_HIDDEN,	"",		GVFM_LEFT,		0,							0, 0,		"",							1, 0, 0, 0.f},	
	{"수량",		38,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_TOOLTIP|GVAT_COMMA,					0, 0,		"",							0, 0, 9, 0.040126},		
	{"가능",		0,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_TOOLTIP|GVAT_COMMA,					0, 0,		"",							0, 0, 8, 0.f},		
	{"매입가",		60,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_TOOLTIP|GVAT_COMMA,					0, 0,		"",							0, 0, 0, 0.0633},		
	{"매입금액",	0,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_TOOLTIP|GVAT_COMMA,				0, 0,		"",							0, 0, 0, 0.f},		
	{"현재가", 		50,		GVAT_TEXT,		"2023",	GVFM_RIGHT,		GVAT_TOOLTIP|GVAT_COMMA|GVAT_CONDITIONx, 0, 0,		"",							0, 0, 0, 0.0527},		
	{"전일대비",	0,		GVAT_HIDDEN,	"",		GVFM_LEFT,		0,							0, 0,		"",							1, 0, 0, 0.f},
	{"등락률",	0,		GVAT_HIDDEN,	"",		GVFM_LEFT,		0,							0, 0,		"",							1, 0, 0, 0.f},
	{"평가금액", 	64,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_TOOLTIP|GVAT_COMMA,					0, 0,		"",							0, 0, 0, 0.06758},	
	{"평가손익", 	64,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_TOOLTIP|GVAT_COMMA|GVAT_CONDITION,	0, 0,		"",							0, 0, 0, 0.06758},		
	{"손익률", 		60,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_TOOLTIP|GVAT_COMMA|GVAT_CONDITION,	0, 0,		"",							0, 0, 0, 0.0633},		//15

	{"코드", 		0,		GVAT_TEXT,		"",		GVFM_LEFT,		0,							0, 0,		"",							0, 0, 0, 0.f},	//16	
	{"대출일", 		66,		GVAT_TEXT,		"",		GVFM_LEFT,		GVAT_TOOLTIP,							0, 0,		"",			0, 0, 0, 0.06969},//17		
	{"신용금액", 	0,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,					0, 0,		"",							0, 0, 0, 0.f},//18

	{"", 			0,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,					0, 1,		"잔         고",			1, 1, 0, 0.f}, //19

	{"손실목표", 	56,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,					0, 0,		"",							0, 0, 23, 0.059134},		
	{"손실조건", 	0,		GVAT_TEXT,		"",		GVFM_RIGHT,		0,							0, 0,		"",							0, 0, 22, 0.f},		
	{"이익목표", 	56,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,					0, 0,		"",							0, 0, 25, 0.059134},	
	{"이익조건", 	0,		GVAT_TEXT,		"",		GVFM_RIGHT,		0,							0, 0,		"",							0, 0, 24, 0.f},	
	{"이익보전", 	56,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,					0, 0,		"",							0, 0, 27, 0.059134},		
	{"보존조건", 	0,		GVAT_TEXT,		"",		GVFM_RIGHT,		0,							0, 0,		"주문발동조건",				0, 1, 26, 0.f},		

	{"고점대비",	0,		GVAT_HIDDEN,	"",		GVFM_RIGHT,		0,							0, 1,		"",							0, 0, 0, 0.f},
	{"고점조건",	0,		GVAT_HIDDEN,	"",		GVFM_CENTER,	0,							0, 1,		"",							0, 0, 0, 0.f},
	{"고점",		0,		GVAT_HIDDEN,	"",		0,				0,							1, 1,		"",							0, 0, 0, 0.f},

	{"호가",		0,		GVAT_HIDDEN,	"",		0,				0,							0, 0,		"",							1, 0, 0, 0.f},//29
	{"상태", 		0,		GVAT_HIDDEN,	"",		GVFM_CENTER,	0,							0, 0,		"",							0, 0, 0, 0.f},//		
	{"실현", 		30,		GVAT_TEXT,		"",		GVFM_CENTER,	0,							0, 0,		"",							0, 0, 0, 0.0316},//		
	{"주문가", 		46,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,					0, 0,		"",							0, 0, 0, 0.04857},	
	{"주문량", 		46,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,					0, 0,		"",							0, 0, 36, 0.04857	},		
	{"수량조건", 	0,		GVAT_TEXT,		"",		GVFM_RIGHT,		0,							0, 0,		"",							0, 0, 35, 0.f},		
	{"수량2", 		0,		GVAT_TEXT,		"",		GVFM_RIGHT,		0,							0, 0,		"",							0, 0, 0, 0.f},		
	{"주문No", 		46,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_TOOLTIP,				0, 0,		"주         문",			0, 0, 0, 0.04857},	//947
	{"처리메세지", 	0,		GVAT_HIDDEN,	"",		GVFM_LEFT,		0,							0, 0,		"주         문",			0, 1, 0, 0.f},	

	{"수량단위",	0,		GVAT_HIDDEN,	"1377",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"상한가", 		0,		GVAT_HIDDEN,	"2302",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"매수5", 	0,		GVAT_HIDDEN,	"3075",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"매수4", 	0,		GVAT_HIDDEN,	"3074",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"매수3", 	0,		GVAT_HIDDEN,	"3073",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"매수2", 	0,		GVAT_HIDDEN,	"3072",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"매수1", 	0,		GVAT_HIDDEN,	"3071",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"현재가", 		0,		GVAT_HIDDEN,	"2023",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"매도1", 	0,		GVAT_HIDDEN,	"3051",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"매도2", 	0,		GVAT_HIDDEN,	"3052",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"매도3", 	0,		GVAT_HIDDEN,	"3053",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"매도4", 	0,		GVAT_HIDDEN,	"3054",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"매도5", 	0,		GVAT_HIDDEN,	"3055",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"하한가", 		0,		GVAT_HIDDEN,	"2303",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"시  가", 		0,		GVAT_HIDDEN,	"2029",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"고  가", 		0,		GVAT_HIDDEN,	"2030",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"저  가", 		0,		GVAT_HIDDEN,	"2031",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},	
	{"상장주식수", 		50,		GVAT_HIDDEN,	"12374",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},
	{"기준가", 		50,		GVAT_HIDDEN,	"1336",	GVFM_LEFT,		GVAT_COMMA|GVAT_CONDITIONx,	0, 1,		"",							0, 0, 0, 0.f},
};




