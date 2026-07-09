#if !defined(AXIS_GRAPH_OBJECT_H)
#define AXIS_GRAPH_OBJECT_H

#define	GD_UNUSED		-999999999
#define	DKEY_ORGDATA	1	// received data key range : 1 ~ 20
#define	DKEY_APPDATA	20	// application data key range : 20 ~ 

enum { cPRC = 0, cLIN, cVOL, cAMT};
enum { dpNONE = 0, dpBASIC = 1, dpTICK = 2, dpVIDX = 3};

enum {	toolNO = -1,
	toolHIDE, toolMOUSE, toolHISTORY, toolDELETE, toolINPUT,
	toolWIDTH, toolPALETTE, toolTEND, toolTENDEX, toolHLINE,
	toolARC, toolUARROW, toolGIGAN, toolDAEDEUNG, toolFIBOLINE,
	toolGANNFAN, toolTRILINE, toolVLINE, toolCROSS, toolTRI,
	toolRECT, toolDARROW, toolLARROW, toolRARROW, toolFIBOFAN,
	toolFIBOFAN2, toolFIBOARC, toolFIBORET, toolRGANNFAN, toolGANNLINE,
	toolQUARDLINE, toolACCELFAN, toolACCELARC, toolDAEDEUNGCFG, toolGIGAN2, 
	toolTRILINELEVEL, toolAndrewPf, toolPatternUP, toolPatternDN};

enum { popLINE = 40, popFIGURE, popARROW, popGIGAN, popDAEDEUNG, popFIBO, popGANN, popETC};
enum { toolVIEW = 50, toolSETUP, toolNUM};

struct _gIndex{
	struct {
		short		yy;	// year
		unsigned char	mm;	// month
		unsigned char	dd;	// day
	}date;
	struct {
		unsigned char	hh;	// hour
		unsigned char	mm;	// minute
		unsigned char	ss;	// second
	}time;
};
#define	sz_gIndex	sizeof(struct _gIndex)
/*
struct	_gBasic {
	int		fpr;			// 시가
	int		hpr;			// 고가
	int		lpr;			// 저가
	int		epr;			// 종가
	double		trn;			// 거래량
	double		amt;			// 거래대금
	struct _gIndex	index;

	unsigned char	check;			// information check
	WORD		rate;			// check rate
	int		mgjy;			// 미결제약정
//	WORD		ucnt;			// 상승종목수
//	WORD		dcnt;			// 하락종목수


	//**WORD		fobtain;		// 외구인 지분율
	//**WORD		fexhaust;		// 외국인 소진율
	//**int		fmmvol;			// 외국인 순매수량
	//** 2005.11.08 add by macho
	//int     fmmamt;			// 외국인 순매수금액
	//int     orgvol;			// 기관계 순매수량
	//int		orgamt;			// 기관계 순매수금액
	//int		invdata[6];

	int	orgnvol;
	int orgnamt;
	int frgnvol;
	int frgnamt;
	int fobtain;
	int fexhaust;
};
#define	sz_gBASIC	sizeof(struct _gBasic)
*/
struct	_gTick {
	int		epr;			// 종가
	double		trn;			// 거래량
	struct _gIndex	index;
};
#define	sz_gTICK	sizeof(struct _gTick)

struct _CoDraw
{
	int	region;
	DWORD	coWork;		// 2006.10.11 schbang	//BYTE	coWork;
	double	PMax;
	double	PMin;
	double	VMax;
	double	VMin;
	double	AMax;
	double	AMin;
	int	tickx;
	int	ticky;
	double	dMacdMax;	// 2006.10.11 schbang
	double	dMacdMin;	// 2006.10.11 schbang
};
#define	sz_CODRAW	sizeof(struct _CoDraw)

//
//	_CoDraw.coWork can be any combination of the following values.
//
#define	MC_PRC		0x01	// 가격 차트
#define	MC_VOL		0x02	// 거래량 차트
#define	MC_AMT		0x04	// 거래대금 차트
//#define	MC_USR		0x08	// User 차트
#define	MC_MACD		0x08	// MACD		2006.10.11 schbang

/** remarking by macho
#define	FOBRATE		0x01	// GK_FOBRATE
#define	FEXRATE		0x02	// GK_FEXRATE
#define	FRGNVOL		0x04	// GK_FRGNVOL
//** add 2005.11.07 by macho
#define FRGNAMT		0x08	// GK_FRGNAMT
#define ORGNVOL		0x10	// GK_ORGNVOL
#define ORGNAMT		0x20	// GK_ORGNAMT
**/

//** add 2006.12.12 by macho-

#define FRGNHAVE	0x01		//** 외국인보유현황
#define FRGNRATE	0x02		//** 외국인보유비율
#define PSNLPBUY	0x04		//** 개인순매수량
#define FRGNPBUY	0x08		//** 외국인순매수량
#define ORGNPBUY	0x10		//** 기관순매수량
#define PSNLTBUY	0x20		//** 개인누적순매수량
#define FRGNTBUY	0x40		//** 외국인누적순매수량
#define ORGNTBUY	0x80		//** 기관누적순매수량


struct _toolConfig {
	CImageList*	pImgLst;
	char*		pcBuf;
	int		iCount;
	char*		pcBufResult;
	int		iCountResult;
};

struct _aidConfig {
	char*		pcBuf;
	int		iCount;
	char*		pcBufResult;
	int		iCountResult;
};

#endif
