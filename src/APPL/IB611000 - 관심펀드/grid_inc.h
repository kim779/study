#pragma once
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
	{"",			55,		GVAT_TEXT,		"",		GVFM_LEFT,	GVAT_TOOLTIP,				1, 1,		"",							0, 0, 0, 0.11},
	{"펀드명      ",		260,	GVAT_TEXT,		"",		GVFM_LEFT,		GVAT_TOOLTIP,		0, 1,		"",							0, 0, 3, 0.0559},	
	{"기준가",		55,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,			0, 1,		"",							1, 0, 0, 0.f},	

	{"1일", 		41,		GVAT_TEXT,		"",	GVFM_RIGHT,		GVAT_COMMA,	0, 0,		"",							0, 0, 0, 0.0527},		
	{"1주", 		41,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,	0, 0,		"",							0, 0, 0, 0.06758},	
	{"1개월", 		41,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,	0, 0,		"",							0, 0, 0, 0.06758},		
	{"3개월", 		41,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,	0, 0,		"",							0, 0, 0, 0.06758},		
	{"6개월", 		41,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,	0, 0,		"",							0, 0, 0, 0.0633},		//15
	{"1년", 		41,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,	0, 0,		"",							0, 0, 0, 0.0633},		//15
	{"3년", 		41,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,	0, 0,		"",							0, 0, 0, 0.0633},		//15
	{"연초",		41,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,	0, 0,		"",							0, 0, 0, 0.0633},		//15
	{"누적", 		41,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_COMMA,	0, 0,		"",							0, 0, 0, 0.0633},		//15
//  

 	{"", 			0,		GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_CONDITIONx,		0, 1,		"수    익    률",			1, 1, 0, 0.f}, //19// 	
	{"간접투자유형", 80,  	GVAT_TEXT,		"",		GVFM_LEFT,		GVAT_TEXT,	0, 1,		"",							0, 0, 0, 0.0633},		//15	
	{"기간(일)",	 55,	GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_TEXT,	0, 1,		"",							0, 0, 0, 0.0633},		//15	
	{"설정일",		 70,	GVAT_TEXT,		"",		GVFM_CENTER,	GVAT_TEXT,	0, 1,		"",							0, 0, 0, 0.0633},		//15	
	{"주식편입비율(%)", 97,GVAT_TEXT,		"",		GVFM_RIGHT,		GVAT_TEXT,	0, 1,		"",							0, 0, 0, 0.0633},		//15	
	{"투자등급명", 75 ,		GVAT_TEXT,		"",		GVFM_LEFT,		GVAT_TEXT,	0, 1,		"",							0, 0, 0, 0.0633},		//15
//	{"", 0,		GVAT_HIDDEN,		"",		GVFM_RIGHT,		GVAT_CONDITIONx,	1, 1,		"",							1, 1, 0, 0.f},	
};

enum _colINDEX
{
	colSET=0,//0
		colCHECK,//0
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
