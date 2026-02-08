#pragma once

#define IMG_ZOOMOUT	0
#define IMG_ZOOMIN	3
#define IMG_PRE		9
#define IMG_STOP	12
#define IMG_NEXT	15
#define IMG_EXCEL	18

enum {ctl_NO = 0, ctl_ZOOMOUT, ctl_ZOOMIN, ctl_PRE, ctl_STOP, ctl_NEXT, ctl_EXCEL};

#define	R_CURR	"023"	// 현재가
#define	R_DIFF	"024"	// 전일대비
#define	R_GVOL	"027"	// 거래량
#define	R_SIGA	"029"	// 시가
#define	R_KOGA	"030"	// 고가
#define	R_JEGA	"031"	// 저가
#define	R_CVOL	"032"	// 체결량
#define	R_UDYL	"033"	// 등락율
#define	R_CTIM	"034"	// 체결시간

//#define	ER_CURR	"723"	// 현재가
#define	ER_DIFF	"724"	// 전일대비
#define	ER_GVOL	"727"	// 거래량
#define	ER_SIGA	"729"	// 시가
#define	ER_KOGA	"730"	// 고가
#define	ER_JEGA	"731"	// 저가
#define	ER_CVOL	"732"	// 체결량
#define	ER_UDYL	"733"	// 등락율
#define	ER_CTIM	"734"	// 체결시간

#define	BGIndex	68
#define	CLR_UP		94
#define	CLR_DOWN	95

enum { ctkLEFT, ctkRIGHT, ctkBOTTOM};

#define	CDT_LEFT	1
#define	CDT_RIGHT	2

#pragma pack(1)
struct _cdHeader	// upload header
{
	char		dunit[2]{};	// 종목,선물,옵션..
	char		dindex[2]{};	// 일,주,월,분,틱
	char		dkind[2]{};	// data struture
	char		dcount[6]{};	// data request count
	char		symbol[6]{};	// rts symbol
	char		reserver[10]{};	// reserved
};

struct _cdJuga		// download header
{
	char		nData[6]{};	// data 개수
	char		digit[2]{};	// 자리수
	char		pday[8]{};	// 최근영업일자
	char		mgap[4]{};	// 분봉갭
					// 9xx : xx초
					// xx: xx분
	char		pjgga[7]{};	// 전일가
	char		ujgga[7]{};	// 상한가
	char		djgga[7]{};	// 하한가
};
#define	sz_CDJUGA	sizeof(struct _cdJuga)

/************* 특성차트 Start *********/
struct _cdMultiline/* Multi Line CHAT*/
{
	char	nData[6]{};		/* data 개수*/
	char	pday[8]{};		/* 최근영업일자*/
	char	mgap[4]{};		/* 분봉갭*/
					/* 초:X9xx 일별: 0000*/
	struct {			/* 영역(상,하)*/
		char    digit[2]{};	/* 자리수*/
		char    lcnt[2]{};	/* 해당영역의 라인개수*/
		char    rcod[5][6]{};	/* 각라인의 RTS symbol*/
					/* 빈영역은 ' '로 채울것*/
	} region[2];			/* 상하영역 구분*/
};
#define sz_CDMultiL     sizeof(struct _cdMultiline)

struct _cdMulticode
{
	char	nData[6]{};		// data 개수
	char	pday[8]{};		// 최근영업일자
	char	mgap[4]{};		// 분봉갭
					// 9xx : xx초
					// xx: xx분
	char	lcnt[2]{};		// Line cnt
	char	rgn[5][1]{};		// 위치 영역
	char	name[5][16]{};		// 차트 이름
	char	rcod[5][12]{};		// RTS Code
	char	rsym[5][6]{};		// RTS Symbol 
	char	digit[5][2]{};		// 소수점 자리수
	char	jgga[5][6]{};		// 전일종가
	char	ctype[5][1]{};		// 차트 유형(Line:'1', Bong:'B')
	char	reserve[11]{};		// 
};
#define	sz_CDMULTICODE	sizeof(struct _cdMulticode)

/************* 특성차트 End *********/

//
//	gKind specify one of the following values
//
#define	CGK_LINE	1
#define	CGK_BONG	2
#define	CGK_BAR		3
#define	CGK_VOL		4
#define	CGK_PMA		5
#define	CGK_VMA		6
#define	CGK_AMT		7

//
//	dIndex specify one of the following values
//
#define	CDI_TICK	1
#define	CDI_MIN		2
#define	CDI_DAY		3
#define	CDI_WEEK	4
#define	CDI_MON		5
#define	CDI_TICK2	6

//
//	dKind specify one of the following values
//
#define	CDK_TICK	1
#define	CDK_BONG	2

#define	CDK_VAL		3
#define	CDK_VAL2	4
#define	CDK_VIDX	5
#define	CDK_NMVL	6
#define	CDK_VAL3	7

//
//	dUnit specify one of the following values
//
#define	CDU_JONGMOK	1
#define	CDU_UPJONG	2
#define	CDU_FUTURE	3
#define	CDU_OPTION	4
#define	CDU_ECN		5
#define	CDU_FOREIGN	8		// 해외시세
//
//	_gInfo.dKind == CDK_TICK
//
struct _cdTick
{
	char		epr[7]{};		// 체결가
	char		date[6]{};	// 체결시간 [hh/mm/ss]
};
#define	sz_cdTick	sizeof(struct _cdTick)

//
//	_gInfo.dKind == CDK_TICK2
//
struct _cdTick2
{
	char		epr[7]{};		// 체결가
	char		trn[8]{};		// 거래량
	char		date[6]{};	// 체결시간 [hh/mm/ss]
};
#define	sz_cdTick2	sizeof(struct _cdTick2)

//
//	_gInfo.dKind == CDK_BONG
//

struct _cdBong
{
	char		fpr[7]{};		// 시가
	char		hpr[7]{};		// 고가
	char		lpr[7]{};		// 저가
	char		epr[7]{};		// 종가
	char		trn[10]{};	// 거래량
	char		date[14]{};	// 체결시간 [yyyy/mm/dd/hh/mm/ss]
};
#define	sz_cdBong	sizeof(struct _cdBong)

//
//	_gInfo.dKind == CDK_VAL
//
struct	_cdVal {
	char		val[10]{};
};
#define	sz_cdVal	sizeof(struct _cdVal)

//
//	_gInfo.dKind == CDK_VAL2
//
struct	_cdVal2 {
	char		date[14]{};	// [yyyy/mm/dd/hh/mm/ss]
	char		val1[10]{};
	char		val2[10]{};	
};
#define	sz_cdVal2	sizeof(struct _cdVal2)

//
//	_gInfo.dKind == CDK_VAL3
//
struct	_cdVal3 {
	char		date[14]{};	// [yyyy/mm/dd/hh/mm/ss]
	char		val1[10]{};
	char		val2[10]{};
	char		val3[10]{};
};
#define	sz_cdVal3	sizeof(struct _cdVal3)

//
//	_gInfo.dKind == CDK_VIDX
//
struct	_cdVidx {
	char		date[14]{};		// 일자
	char		val[10]{};		// value
};
#define	sz_cdVidx	sizeof(struct _cdVidx)

//
//	_gInfo.dKind == CDK_NMVL
//
struct	_cdNameVal {
	char		name[20]{};
	unsigned char	color[4]{};
	char		val[10]{};
};
#define	sz_cdNMVL	sizeof(struct _cdNameVal)

struct	_cdValue2 { 
	char		yval[10]{}; 
	char		xval[10]{}; 
	char		date[14]{};	// [yyyy/mm/dd/hh/mm/ss] 
}; 
#define	sz_cdValue2	sizeof(struct _cdValue2) 

//
//************* client only Start *********/
//
struct _cgIndex{
	struct {
		short		yy{};	// year
		unsigned char	mm{};	// month
		unsigned char	dd{};	// day
	}date;
	struct {
		unsigned char	hh{};	// hour
		unsigned char	mm{};	// minute
		unsigned char	ss{};	// second
	}time;
};
#define	sz_cgIndex	sizeof(struct _cgIndex)

struct _cgTime{
	unsigned char	hh{};	// hour
	unsigned char	mm{};	// minute
	unsigned char	ss{};	// second
};
#define	sz_cgTime	sizeof(struct _cgTime)

struct _cgTick
{
	struct _cgTime	time{};	// 체결시간
	int		epr{};	// 체결가
	int		diff{};	// 대비
	float		udyl{};	// 등락율
};
#define	sz_cgTick	sizeof(struct _cgTick)

struct _cgTick2
{
	struct _cgTime	time{};	// 체결시간
	int		epr{};	// 체결가
	int		trn{};	// 거래량
	int		diff{};	// 대비
	float		udyl{};	// 등락율
};
#define	sz_cgTick2	sizeof(struct _cgTick2)

struct _cgTickEx
{
	struct _cgTime	time{};	// 체결시간
	int		epr{};	// 체결가
	int		trn{};	// 거래량
};
#define	sz_cgTickEx	sizeof(struct _cgTickEx)

struct _cgBong
{
	struct _cgIndex	index{};		// 시간/일자
	int		fpr{};		// 시가
	int		hpr{};		// 고가
	int		lpr{};		// 저가
	int		epr{};		// 종가
	double		trn{};		// 거래량
};
#define	sz_cgBong	sizeof(struct _cgBong)

struct _cgVal {	
	double		val{};
};
#define	sz_cgVAL		sizeof(struct _cgVal)

struct _cgVal2 {
	struct _cgIndex	index{};
	double		val[2]{};
};
#define sz_cgVAL2	sizeof(struct _cgVal2)

struct _cgSVal2 {
	char		name[14]{};
	double		val[2]{};
};
#define sz_cgSVAL2	sizeof(struct _cgSVal2)

struct _cgVal3 {
	struct _cgIndex	index{};
	double		val[3]{};
};
#define sz_cgVAL3	sizeof(struct _cgVal3)

struct _cgVal4 {
	struct _cgIndex	index{};
	double		val[4]{};
};
#define sz_cgVAL4	sizeof(struct _cgVal4)

struct _cgVidx {
	double		val{};
	struct _cgIndex	index{};
};
#define	sz_cgVIDX	sizeof(struct _cgVidx)

struct _cgNameval {
	char		name[20]{};	// 
	DWORD		color{};
	double		val{};
};
#define	sz_cgNAMEVAL	sizeof(struct _cgNameval)



#define	CM_GRP		WM_USER + 1
#define	CM_CTRL		WM_USER + 2
//
//	each Message's LOWORD(wParam) can specify one of the following values
//

//
//	CM_GRP => GRP_
//
#define	GRP_Control	0x01
	// HIWORD(wParam) : type
	#define	GetScrData	1	// get scroll data, return data len, lParam:data reference
	#define	ChgDispPos	2	// change pos, LOWORD(lParam) : dispPos, HIWORD(lParam) : dispEnd
					// SB_LINEUP, SB_LINEDOWN
	#define	ChgDispScale	3	// change scale, lParam:scale
					// (-) : m_dispDay 감소, (+) : m_dispDay 증가
	#define	ChgDisplay	4	// change display, lParam:display cnt, 
					// Slide Bar
	#define	autoScroll	5	// lParam: true-set, false-reset
	#define	ChgShift	6	// lParam : shift value
	#define	ExcelData	7	//
	#define ChgParam	8	// lParam : Param info
	#define	outConfig	9	// 
//
//	CM_CTRL => CTRL_
//
#define	CTRL_Enable	0x01	// HIWORD(wParam) : click event enable, (TRUE,FALSE)
#define	CTRL_ChgScrPos	0x02	// HIWORD(wParam) : nSBCode, lParam : pos
#define	CTRL_UpdateScr	0x03	// Update Scroll
#define	CTRL_Clear	0x04	// clear ctrl
#define CTRL_Focus 0x05 // focus

struct _MinMaxChk	
{
	int	region{};
	int	coMinMax{};
	double	Max{};
	double	Min{};
	int	tickx{};
	int	ticky{};
};

struct _gInfo		// used in client only
{
	int	gKind{};
	int	rKey{};
	int	dKey{};
	int	dIndex{};
	int	dKind{};
	COLORREF	tRGB{};
	COLORREF	pRGB{};
};
/************* client only End *********/

#pragma pack()