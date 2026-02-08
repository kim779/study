#pragma once
#pragma pack(1)

#define	RS_CTIM	"034"		// 체결시간
#define	RS_BASE	1		// 실시간 테스트용 기본 심볼
#define	RS_CODE	"SSSSSS"	// 실시간 테스트용 코드
#define	RS_SYM	"001"		// 실시간 테스트용 심볼

enum { ctkLEFT = 0, ctkRIGHT, ctkBOTTOM};

#define	CDT_LEFT	0x01	// used in GrpWnd
#define	CDT_RIGHT	0x02	// used in GrpWnd
#define	CDT_BOTTOM	0x04	// used in GrpWnd

#define	CLR_UP		94
#define	CLR_DOWN	95

/*----------------------------------------------*/
/* UpLoad Data Information			*/
/*----------------------------------------------*/
struct _UpHead			/* upload header		*/
{
	char	dunit[2]{};	/* 종목,선물,옵션..		*/
	char	dindex[2]{};	/* 일,주,월,분,틱		*/
	char	dcount[6]{};	/* 요청개수			*/
	char	reserve[22]{};	/* reserved			*/
};
#define	sz_UPHEADER	sizeof(struct _UpHead)

/*----------------------------------------------*/
/* DownLoad Data Information			*/
/*----------------------------------------------*/
struct _DnHead			/* download header		*/
{
	char	nHead[2]{};	/* data Head 개수		*/
	char	nData[6]{};	/* 수신 data 개수		*/
	char	irSize[2]{};	/* 1 index 데이타 크기		*/
	char	drSize[2]{};	/* 1 value 데이타 크기		*/
	char	pday[8]{};	/* 최근영업일자			*/
	char	mgap[4]{};	/* 분봉갭			*/
				/* 9xx : xx초			*/
				/* xx: xx분			*/

	char	xtype{};		/* 시계열의 문자열여부		*/
				/* default ' ' : yyyymmddhhmmss	*/
	char	xUnit[3][16]{};	/* 하/좌/우단 단위		*/
	char	tipName[64]{};	/* tooltip name(비시계열에 사용)*/
				/* tipname + \t + name \t...	*/
	char	reserve[32]{};	/* reserve			*/
};
#define	sz_DNHEAD	sizeof(struct _DnHead)

struct _DataInfo		/* download Data Info		*/
{
	char	filler[2]{};	/* "XX"				*/
	char	name[32]{};	/* 이름				*/
	char	digit[2]{};	/* 소수점 자리수		*/
	char	rtscode[12]{};	/* 실시간 코드			*/
	char	symbol[5]{};	/* 실시간 심볼			*/
	char	rtsSign[1]{};	/* 실시간 데이타의 부호필드 여부*/
				/* ' ' : 부호 무조건 제거	*/
				/* '1' : 부호('-')만 제거	*/
				/* '0' : 부호사용 안함		*/
	char	pivot[14]{};	/* 기준값			*/
	char	reserve[16]{};	/* reserve			*/
};
#define	sz_DATAINFO	sizeof(struct _DataInfo)

#define	sz_VAL	14		/* 기본사이즈 14로 하되 필요시 변경해서 사용할것 */
struct _Data
{
	char	val[sz_VAL]{};		/* Data Value			*/
};
#define	sz_DATA	sizeof(struct _Data)

/*------------------------------------------------------------------------------*/
/* nData = _DnHead.nData							*/
/* nHead = _DnHead.nHead							*/
/* iSize = _DnHead.irSize							*/
/* dSize = _DnHead.drSize							*/
/* DownData = _DnHead + (iSize*nData) + nHead * (_DataInfo + dSize*nData)	*/
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
struct _cgNIndex
{
	struct 
	{
		short		yy{};	// year
		unsigned char	mm{};	// month
		unsigned char	dd{};	// day
	}date;
	struct 
	{
		unsigned char	hh{};	// hour
		unsigned char	mm{};	// minute
		unsigned char	ss{};	// second
	}time;
};
#define	sz_cgNIndex	sizeof(struct _cgNIndex)


//struct _cgNVal 
//{
//	union 
//	{
//		struct
//		{
//			char	str[21]{};		// 비시계열의 하단 표시명 
//		}name;
//		struct _cgNIndex year{};		// 시계열의   yymmddhhmmss
//	}index;					
//	double		val[1]{};			// 그래프 값(개수는 가변, 최대 18개)
//};
//#define sz_cgNVAL	sizeof(struct _cgNVal)


struct _cgNValx
{
	std::vector<double> vData;			// 그래프 값(개수는 가변, 최대 18개)
	union
	{
		struct
		{
			char	str[21]{};		// 비시계열의 하단 표시명 
		}name;
		struct _cgNIndex year {};		// 시계열의   yymmddhhmmss
	}index;
};
#define sz_cgNVALx	sizeof(struct _cgNValx)





#define	CM_GRP		WM_USER + 1
#define	CM_CTRL		WM_USER + 2
#define	CM_PREVIEW	WM_USER + 3	// request Preview data
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
#define CTRL_Focus	0x05

struct _NMinMaxChk	
{
	int	region{};		// 영역(0-8)
	int	coMinMax[2]{};	// 최대최소 동시적용 여부(좌/우)
	double	Max[2]{};		// 동시사용 최대값(좌/우)
	double	Min[2]{};		// 동시사용 최소값(좌/우)
	int	tickx[2]{};	// 그래프 라벨 X위치(좌/우) 
	int	ticky[2]{};	// 그래프 라벨 Y위치(좌/우) 
};
#define	sz_NMINMAXCHK	sizeof(_NMinMaxChk)

struct _gNInfo
{
	int	gKind{};		// graph kind
	int	rKey{};		// region key(0-8)
	int	dKey{};		// data key(0-17)
	int	dIndex{};		// 일주월분틱
	int	mIndex{};		// MinMax 색인(0,1)
	int	lIndex{};		// label 색인
	int	Tick{};		// Tick 그리는 정보
	COLORREF	tRGB{};	// foreground
	COLORREF	pRGB{};	// background
	int	lwidth{};
	int	lstyle{};
};

////////////////////////////////////////////////
// 설정정보
////////////////////////////////////////////////

struct _GrpInfo	// 전체 설정정보
{
	char	tick{};		// 전체 틱정보,				저장시 + CT_CHAR
	char	ctrl{};		// 컨트롤 사용여부,			저장시 + CT_CHAR
	char	option1{};	// 수평선/수직선/최대최소 그리기	저장시 + CT_CHAR
	char	option2{};	// 미리보기/역순/수직하단틱		저장시 + CT_CHAR
	char	req[5]{};		// 그래프 요청개수
	char	show[5]{};	// 그래프 출력개수
	char	gcnt[2]{};	// 그래프 개수(최대 18)
	char	rcnt[2]{};	// 영역 개수(최대 9)
	char	rinfo[18]{};	// 영역 분할정보(2byte * 최대 9개)
	char	index[1]{};	// 색인-일주월분틱
	char	option3{};	// 개별 실시간				저장시 + CT_CHAR
	char	res[8]{};		// reserve
};
#define	sz_GRPINFO	sizeof(struct _GrpInfo)

struct _GrpLine	// 개별 그래프 정보
{
	char	kind[2]{};	// 그래프종류
	char	region{};		// 영역(1-9)		저장시 + CT_CHAR
	char	tick{};		// 좌우틱정보		저장시 + CT_CHAR
	char	color[10]{};	// 그래프 색상
	char	width{};		// 그래프 라인 두께	저장시 + CT_CHAR
	char	style{};		// 그래프 라인 스타일	저장시 + CT_CHAR
	char	option1{};	// option1
	char	option2{};	// option2
	char	res[4]{};		// reserve
};
#define	sz_GRPLINE	sizeof(struct _GrpLine)

#define	MAX_REGION	9	// 최대 9개
#define	MAX_GRAPH	MAX_REGION*2

// _GrpLine.kind
#define	IGK_LINE	1	// 선 차트
#define	IGK_2DVBAR	2	// 2D 수직막대
#define	IGK_3DVBAR	3	// 3D 수직막대
#define	IGK_2DUDBAR	4	// 2D 상하 수직막대
#define	IGK_3DUDBAR	5	// 3D 상하 수직막대
#define	IGK_2D0UDBAR	6	// 2D 0대칭 상하 수직막대
#define	IGK_3D0UDBAR	7	// 3D 0대칭 상하 수직막대
#define	IGK_DOT		8	// 점 차트
#define	IGK_YBASELINE	9	// y기준 대비 선차트(pivot과 y축값을 비교하여 적색/파랑 선 표시)
#define	IGK_XBASEVBAR	10	// x기준 대비 수직막대(pivot과 x축값을 비교하여 적색/파랑 Bar 표시)
#define	IGK_2D3NBAR	11	// 2D 3N 수평막대 => 매수/매도/보합 체결에 의한 매물대차트
#define	IGK_2D1NBAR	12	// 2D 1N 수평막대

// _GrpInfo.tick/_GrpLine.tick
#define	ICF_CHAR	0x30	// 저장시 '0'~'9'까지의 문자로 저장하기 위해 사용
#define	ICF_NONE	0x00	// 
#define	ICF_LEFT	0x01	// 좌측틱 그리기
#define	ICF_RIGHT	0x02	// 우측틱 그리기
#define	ICF_BOTTOM	0x04	// 하단틱 그리기

// _GrpInfo.option1
#define	ICF_HDRAW	0x01	// 수평선 그리기
#define	ICF_VDRAW	0x02	// 수직선 그리기
#define	ICF_MINMAX	0x04	// 최대최소값 표시

// _GrpInfo.option2
#define	ICF_PREVIEW	0x01	// 미리보기
#define	ICF_REVDRAW	0x02	// 역순으로 그리기 
#define	ICF_TICKVERTIC	0x04	// 하단틱 수직으로 그리기

// GrpInfo.option3
#define	ICF_ITEMRTS	0x01	// 항목별 실시간

// _GrpLine.option1
#define	IGO_NAMER	0x01	// 이름을 우측에 표시
#define	IGO_MINMAX	0x02	// 같은 영역 차트에 한해 최대최소값 동일하게 적용

struct _TestInfo	// test data info for Preview
{
	int	gcount{};		// grount count
	int	gKind[32]{};	// graph kind
	bool	bVerticStr{};	// vertical tick
	bool	bItemRts{};	// 항목별 실시간 여부
};

#pragma pack()