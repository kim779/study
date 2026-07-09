#if !defined(UGINFO_H__INCLUDED_)
#define UGINFO_H__INCLUDED_

#define	R_CURR	"023"	// 현재가
#define	R_DIFF	"024"	// 전일대비
#define	R_GVOL	"027"	// 거래량
#define	R_SIGA	"029"	// 시가
#define	R_KOGA	"030"	// 고가
#define	R_JEGA	"031"	// 저가
#define	R_CVOL	"032"	// 체결량
#define	R_UDYL	"033"	// 등락율
#define	R_CTIM	"034"	// 체결시간
#define	R_MYGL	"201"	// 미결제약정(선물)

#define	BGIndex	4

enum { ctkLEFT = 0, ctkRIGHT, ctkBOTTOM};

#define	CDT_LEFT	0x01	// used in GrpWnd
#define	CDT_RIGHT	0x02	// used in GrpWnd
#define	CDT_BOTTOM	0x04	// used in GrpWnd

/*----------------------------------------------*/
/* UpLoad Data Information			*/
/*----------------------------------------------*/
struct _UpHead				/* upload header		*/
{
	char		dunit[2];	/* 종목,선물,옵션..		*/
	char		dindex[2];	/* 일,주,월,분,틱		*/
	char		dcount[6];	/* data request count		*/
	char		reserve[22];	/* reserved			*/
};
#define	sz_UPHEADER	sizeof(struct _UpHead)

/*----------------------------------------------*/
/* DownLoad Data Information			*/
/*----------------------------------------------*/
struct _DnHead				/* download header		*/
{
	char		nHead[2];	/* data Head 개수		*/
	char		nData[6];	/* 수신 data 개수		*/
	char		pday[8];	/* 최근영업일자			*/
	char		mgap[4];	/* 분봉갭			*/
					/* 9xx : xx초			*/
					/* xx: xx분			*/

	char		tipName[20];	/* tooltip name			*/
	char		xtype;		/* 시계열이 문자열여부		*/
					/* default ' ' : ymdhms		*/
	char		xUnit[3][16];	/* 하/좌/우단 단위		*/
};
#define	sz_DNHEAD	sizeof(struct _DnHead)

struct _DataInfo			/* download Data Info		*/
{
	char		filler[2];	/* "XX"				*/
	char		name[32];	/* 이름				*/
	char		digit[2];	/* 소수점 자리수		*/
	char		rtscode[12];	/* 실시간 코드			*/
	char		symbol[5];	/* 실시간 심볼			*/
	char		rtsorg[1];	/* 실시간 데이타의 부호필드 여부*/
					/* default '1' : 부호 사용안함	*/
	char		reserve[31];	/* reserve			*/
};
#define	sz_DATAINFO	sizeof(struct _DataInfo)

#define	sz_VAL	16
struct _Data
{
	char	val[sz_VAL];		/* Data Value			*/
};
#define	sz_DATA	sizeof(struct _Data)

/*------------------------------------------------------------------------------*/
/* nData = _DnHead.nData							*/
/* nHead = _DnHead.nHead							*/
/* DownData = _DnHead + _Data[nData] + nHead * (_DataInfo + _Data[nData])	*/
/*------------------------------------------------------------------------------*/

//
//	dIndex specify one of the following values
//
#define	CDI_TICK	1		// 틱
#define	CDI_MIN		2		// 분
#define	CDI_DAY		3		// 일
#define	CDI_WEEK	4		// 주
#define	CDI_MON		5		// 월

//
//************* client only ********/
//
struct _cgIndex
{
	struct 
	{
		short		yy;	// year
		unsigned char	mm;	// month
		unsigned char	dd;	// day
	}date;
	struct 
	{
		unsigned char	hh;	// hour
		unsigned char	mm;	// minute
		unsigned char	ss;	// second
	}time;
};
#define	sz_cgIndex	sizeof(struct _cgIndex)

struct _cgVal 
{
	union 
	{
		struct
		{
			char	str[21];	// 비시계열의 하단 표시명 
		}name;
		struct _cgIndex year;		// 시계열의   yymmddhhmmss
	}index;					
	double		val[1];			// 그래프 값(최소 1개부터 가변적임)
};
#define sz_cgVAL	sizeof(struct _cgVal)


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

struct _MinMaxChk	
{
	int	region;		// 영역
	int	coMinMax[2];	// 최대최소 동시적용 여부
	double	Max[2];		// 동시사용 최대값
	double	Min[2];		// 동시사용 최소값
	int	tickx[2];	// 그래프 라벨 X위치(좌/우) 
	int	ticky[2];	// 그래프 라벨 Y위치(좌/우) 
};

struct _gInfo			// used in client only
{
	int	gKind;		// graph kind
	int	rKey;		// region key
	int	dKey;		// data key
	int	dIndex;		// 일주월분틱
	int	mIndex;		// 최대최소 색인(0,1)
	int	lIndex;		// label 색인
	int	Tick;		// Tick 그리는 정보
	COLORREF	tRGB;	// foreground
	COLORREF	pRGB;	// background
	int	lwidth;
	int	lstyle;
};

////////////////////////////////////////////////
// 설정정보
////////////////////////////////////////////////

struct _GrpInfo	// 전체 설정정보
{
	char	tick;		// 전체 틱정보,				저장시 + CT_CHAR
	char	ctrl;		// 컨트롤 사용여부,			저장시 + CT_CHAR
	char	option1;	// 수평선/수직선/최대최소 그리기	저장시 + CT_CHAR
	char	option2;	// reserved				저장시 + CT_CHAR
	char	req[5];		// 그래프 요청개수
	char	show[5];	// 그래프 출력개수
	char	gcnt[2];	// 그래프 개수
	char	rcnt[2];	// 영역 개수
};
#define	sz_GRPINFO	sizeof(struct _GrpInfo)

struct _GrpLine	// 개별 그래프 정보
{
	char	kind;		// 그래프종류		저장시 + CT_CHAR
	char	region;		// 영역			저장시 + CT_CHAR
	char	tick;		// 좌우틱정보		저장시 + CT_CHAR
	char	color[10];	// 그래프 색상
	char	width;		// 그래프 라인 두께	저장시 + CT_CHAR
	char	style;		// 그래프 라인 스타일	저장시 + CT_CHAR
};
#define	sz_GRPLINE	sizeof(struct _GrpLine)

#define	MAX_REGION	9	// 영역은 1~9 최대 9개

// _GrpLine.kind
#define	GK_LINE		1	// 라인차트
#define	GK_2DBAR	2	// 2D Bar
#define	GK_3DBAR	3	// 3D Bar
#define	GK_2DUDBAR	4	// 2D UpDown Bar
#define	GK_3DUDBAR	5	// 3D UpDown Bar
#define	GK_2D0UDBAR	6	// 2D 0기준 상하대칭 Bar
#define	GK_3D0UDBAR	7	// 3D 0기준 상하대칭 Bar
#define	GK_DOT		8	// 점차트

// _GrpInfo.tick/_GrpLine.tick
#define	CF_CHAR		0x30	// 저장시 '0'~'9'까지의 문자로 저장하기 위해 사용
#define	CF_NONE		0x00	// 안그림
#define	CF_LEFT		0x01	// 좌측틱
#define	CF_RIGHT	0x02	// 우측틱
#define	CF_BOTTOM	0x04	// 하단틱

// _GrpInfo.option1
#define	CF_HDRAW	0x01	// 수평선 그리기
#define	CF_VDRAW	0x02	// 수직선 그리기
#define	CF_MINMAX	0x04	// 최대최소 그리기

// _GrpInfo.option2

#endif // !defined(UGINFO_H__INCLUDED_)
