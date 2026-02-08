// BlockBaseEnum.h: defines for Block Data
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCKBASEENUM_H__6AF7C18A_2AAF_4C00_82DF_4E49584F150C__INCLUDED_)
#define AFX_BLOCKBASEENUM_H__6AF7C18A_2AAF_4C00_82DF_4E49584F150C__INCLUDED_

// (2004.10.12, 배승원) BlockBaseData.h에서 Enum Data와 같이 Define 상수 만을 별도의 Header로 분리한다.
//		(DLL 등 외부 Module 개발자와 공유하기 위함)
//		* 주의. 따라서 이 부분을 수정시 외부 Module 연계 담당자에게 알려 외부 Module에도 적용되도록 한다.
//			향후 별도의 AddIn Header Folder를 두어 공유토록 조정할 예정
//		*. 또한, 수정시 값의 확장은 가능하나 변경은 되지 않도록 한다.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FLOAT.H"

static const int MARGIN = 4;
static const double ERRORVALUE = -DBL_MAX;

// VertScaleGroupList.h에서 이동 : ojtaso (20070314)
#define VSGL_TITLE_START_POSITION	( MARGIN * 3)

class CDrawingOption
{
public:
	enum DRAWINGORDER
	{
		LEFT = 0, 
		RIGHT
	};

	enum DRAWINGTYPE
	{
		ALL = 0,	// 모두 그리기
		BACKGROUND,	// 바탕만 그리기
		SCALE,		// 가로, 세로 스케일 그리기
		GRAPH		// Graph 그리기
	};
};

class CBlockBaseData
{
public:
	enum BLOCKTYPE
	{
		ROW,	// 세로형 ((행, 열)->(a, 1))
		COLUMN,	// 가로형 ((행, 열)->(1, b))
		ARRAY	// 바둑형 ((행, 열)->(a, b))
	};
	
	enum BLOCKCOMPART
	{
		COMPARTLINE,// block간의 선으로 구분 (영역 재설정 가능)
		COMPARTGAP,	// block간의 빈 공간으로 구분 (영역 재설정 불가능)
		BCNONE		// block간의 구분이 없음 (영역 재설정 불가능)
	};

	enum BLOCKBEHAVIOR
	{
		BLOCK_DELETE = 0,
		BLOCK_INSERT,
		BLOCK_MOVE
	};

	enum TOOLTYPE
	{
		TOOL_NONE = -1,			// tool 타입이 없음
		TOOL_COMPARTLINE = 0,	// block 크기 tool
		TOOL_BLOCKMOVE,			// block 이동 tool
		TOOL_BLOCKDELETE,		// block 삭제 tool
		TOOL_HORZSCALE,			// x축 scale tool
		TOOL_VERTSCALE,			// y축 scale tool
		TOOL_GRAPHTITLE,		// graph title tool (선택, 설정)
		TOOL_SUBGRAPH,			// sugGraph tool	(선택, 설정, 강조)
		TOOL_GRAPHMOVE,			// graph title / subGraph tool 의 이동
		TOOL_GRAPHDELETE,		// graph title / subGraph tool 의 삭제
		TOOL_BLOCKSIZE			// 블럭 최대화 추가 - ojtaso (20080519)
	};
};

class CGraphBaseData
{
public:
	enum GRAPHTYPE
	{
		GTNONE = -1,
		Bong_Type = 0,		// 캔들차트, 바차트(시고저종), 선차트, 안테나차트, 바차트(고저종), FLOW차트
		Line_Type,			// 일반식, 채움식, 구름식, 수평식, 계단식, 0선비연속식, 대비식, 레인보우식
		Bar_Type,			// 수직식, 수평식, 상하식, 빗형식, 도트식
		Pie_Type,			// 평면식, 입체식
		Special_Type,		// 삼선전환도, 역시계, P&F
		Fund_Type,			// 펀드형
		DataView_Type,		// data 표시
		FuOp_Type,			// 선물옵션형
		MultiBar_Type,		// 멀티바형 //@Solomon:091231

		GraphTypeCount		// GRAPHTYPE의 총갯수
	};

	// Graph Style
	enum BONGSTYLE
	{
		CandlestickBong = 0,	// 캔들차트
		OpenHighLowCloseBong,	// 바차트(시고저종)
		LineBong,				// 선차트
		AntennaBong,			// 안테나차트
		NewHighLowCloseBong,	// 바차트(고저종)
		FlowBong,				// FLOW차트
		CandleVolume,			// 캔들볼륨차트
		EquiVolume				// 이큐볼륨차트
	};

	enum LINESTYLE
	{
		SimpleLine = 0,			// 일반식
		FillLine,				// 채움식
		CloudLine,				// 구름식
		HorizontalLine,			// 수평식
		StairLine,				// 계단식
		NonZeroLine,			// 0선비연속식 (값이 "0"일 경우는 그리지 않음)
		ComparisonLine,			// 대비식
		RainbowLine,			// 레인보우식
		ZigZagLine,				// 지그재그식
		LineCloudLine,			// 선형 구름식
		DataDotSimpleLine,		// 좌표점일반식
		VerticalLine,			// 수직식
		NonCloudLine			// 비움 구름식
	};

	enum BARSTYLE
	{
		VerticalBar = 0,		// 수직식
		HorizontalBar,			// 수평식
		UpDownBar,				// 상하식
		CombBar,				// 빗형식
		BoxBar,					// 박스식
		DotBar,					// 도트식
		LeftRightBar,			// 좌우식
		LeftRightCombBar,		// 좌우빗형식
		GroupBar,				// 그룹바식
		RatioBar,				// 비율식 (ex- 매수/매도 거래량 표현시)
		TextHorizontalBar,		// Text가 있는 수평식
		TextGroupBar,			// Text가 있는 그룹 바식
		UpDownHighLowBar,		// 상하등락식
		FixedRateLeftRightBar,	// 고정비율좌우식
		FixedRateCombBar,		// 고정비율빗형식
		SellByTextHorizontalBar,// 매수매도Text수평식
		FixedRateUpDownBar		// 고정비율상하식d
	};

	enum PIESTYLE
	{
		FlatPie = 0,			// 평면식
		SolidPie				// 입체식
	};

	enum SPECIALSTYLE
	{
		ThreeLineBreak = 0,		// 삼선전환도
		InverseLine,			// 역시계
		PAndF,					// P&F
		Kagi,					// Kagi
		Renko,					// Renko
// (2006.11.09 안승환) 분산차트 추가
		DisperSion				//분산
// (2006.11.09 안승환) 분산차트 추가
	};

	enum FUNDSTYLE
	{
		ComponentRatio_Line = 0,// 구성비(선형)
		ComponentRatio_Bar,		// 구성비(바형)
		ComponentRatio_Bar_Line,// 구성비(바형&선형)
		ComponentRatio_Pie,		// 구성비(파이형)
		ReturnRiskDispersion,	// 수익&위험분산
		ReturnRiskGrade,		// 수익&위험등급
		StyleBox,				// 스타일박스
		FundMarketDispersion,	// 펀드&시장분산
		Group_Bar				// 그룹(바형)
	};

	enum DATAVIEWSTYLE
	{
		TopDataBar_Not_Used = 0,	// 상단식 바형	// (2006/10/11 - Seung-Won, Bae) Not Used Anymore.
		BottomDataBar_Not_Used,		// 하단식 바형	// (2006/10/11 - Seung-Won, Bae) Not Used Anymore.
		PointSignal,				// point signal
		TopSignal,					// 상단 signal
		BottomSignal,				// 하단 signal
		CompartText					// 구분텍스트형
	};

	enum FUOPSTYLE
	{
		MarketProfile,			// MarketProfile
		FuOpPAndF				// FuOpP&F
	};

	//@Solomon:091231
	enum MULTI_BARSTYLE
	{
		MVerticalBar = 0,		// 수직식
	};

	// Graph Drawing Type
	// -> SPECIALSTYLE, FUNDSTYLE, OPTIONSTYLE 는 Drawing Type 이 없다!
	enum BONGDRAWSTYLE		// BONGSTYLE
	{
		Candlestick_Draw = 0,	// 캔들차트
		OpenHighLowClose_Draw,	// 바차트(시고저종)
		Line_Draw,				// 선차트
		Antenna_Draw,			// 안테나차트
		NewHighLowClose_Draw,	// 바차트(고저종)
		Flow_Draw,				// FLOW차트
		CandleVolume_Draw,			// 캔들볼륨차트
		EquiVolume_Draw				// 이큐볼륨차트
	};

	enum LINEDRAWSTYLE		// LINESTYLE
	{
		Solid_Draw = 0,			// 실선
		Dot_Draw,				// 좌표점형
		DottedLine_Draw,		// 선형 점선
		DashDot_Draw,			// 선점형
		DashDotDot_Draw			// 선점점형
	};

	enum BARDRAWSTYLE		// BARSTYLE
	{
		Bar_Draw = 0,			// 바형
		DotBar_Draw,			// 좌표점형
		NonZero_DotBar_Draw,	// NonZero좌표점형
		TextBar_Draw,			// Text형
		HorzLineBar_Draw		// 수평선바형
	};

	enum PIEDRAWSTYLE		// PIESTYLE
	{
		FlatPie_Draw = 0,		// 평면식
		SolidPie_Draw			// 입체식
	};

	enum DATABARDRAWSTYLE	// DATAVIEWSTYLE 중 xxxBar
	{
		Char_Bar = 0,			// 문자식
		Translucent_Bar,		// 반투명식
		Hatchat45Degrees_Bar,	// 45도 빗살식
		VertHorz_Bar,			// 수직수평식
		Fill_Bar				// 채움식
	};

	enum DATASIGNALDRAWSTYLE// DATAVIEWSTYLE 중 xxxSignal
	{
		Char_Signal = 0,		// 문자
		CharArrow_Signal,		// 문자+화살표
		Arrow_Signal,			// 화살표
		Triangle_Signal,		// 삼각형
		Quadrangle_Signal,		// 사각형
		Circle_Signal			// 원형
	};

	enum COMPARTTEXTDRAWSTYLE// DATAVIEWSTYLE 중 CompartText
	{
		CompartLine = 0,		// 구분선식
		Text,					// 텍스트식
		CompartLineText			// 구분선텍스트식
	};

	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	enum BARPAINTSTYLE			// BARSTYLE
	{
		None_Paint = 0,			// 기준없음
		Standard_Draw,			// 기준선
		PrevValue_Draw,			// 이전값
		PrevClose_Draw,			// 이전종가
		PrevHigh_Draw,			// 이전고가
		PrevAvg_Dra,			// 이전평균가(고저종)
		PriceCandle_Draw		// 가격차트
	};

	// 기타
	enum ObjectChange
	{
		INSERT,
		MODIFY
	};

	enum SignalTYPE
	{
		NONE,
		BUY,
		SELL
	};

	/*enum DataDrawType
	{
		LOW_HIGH_UPDOWN,	// 최저가 대비 최고가 등락률
		LOW_NOW_UPDOWN,		// 최저가 대비 현재가 등락률
		HIGH_NOW_UPDOWN,	// 최고가 대비 현재가 등락률
		PRE_NOW_UPDOWN,		// 전일가 대비 현재가 등락률
		PRE_NOW_CONTRAST	// 전일비
	};*/
};

class CScaleBaseData
{
public:
	// 가로 scale Draw type
	enum HORZSCALEDRAWERTYPE
	{
		HORZ_NOTYPE,	// 없음
		HORZ_DAILY,		// 일간
		HORZ_WEEKLEY,	// 주간
		HORZ_MONTHLY,	// 월간
		HORZ_TIME_ONE,	// 시간-한줄
		HORZ_TICK,		// 틱

		HORZ_TIME_TWO,	// 시간-두줄
		HORZ_TEXT,		// text
		HORZ_LEFTRIGHT,	// 좌우식
		HORZ_PRICEZONE,	// 가격대
		HORZ_DATA,		// data(거래량 포함)
		HORZ_MARKETPROFILE, // MarketProfile
		HORZ_TIME_SECOND	// 초
	};

	// 세로 scale Draw type
	enum VERTSCALEDRAWERTYPE
	{
		VERT_NOTYPE,		// 없음
		VERT_PRICE,			// 가격타입
		VERT_VOLUME,		// 거래량타입
		VERT_SALE,			// 대기매물
		VERT_MARKETPROFILE, // MarketProfile
		VERT_BASELINE,		// 기준선타입
		VERT_TEXT,			// text
		VERT_PACKETDATA,	// 순수 packet Data
		VERT_LOG,			// log
		VERT_PERCENT,		// 백분율
		VERT_TOPBUTTOM_PLUS	// +상하식
	};

	// scale grid
	enum SCALEGRIDTYPE
	{
		SOLIDLINE,		 // 실선
		LONGDOTTEDLINE,  // 점선(- - -)
		SHORTDOTTEDLINE, // 점선(·····)
		SLNONE			 // 없음
	};

	// scale 모양
	enum SCALETYPE
	{
		HORZ_SCALE,
		VERT_SCALE
	};

	// 가로 scale 위치
	enum HORZSCALEPOSITION
	{
		HORZ_TOP,
		HORZ_BOTTOM,
		HORZ_BOTH,
		HORZ_HIDE,
		HORZ_NONE
	};

	// 세로 scale 위치
	enum VERTSCALEPOSITION
	{
		VERT_LEFT,
		VERT_RIGHT,
		VERT_BOTH,
		VERT_NONE
	};

	// Text Compart Type
	enum TEXTCOMPARTTYPE
	{
		HORZ_TEXT_COMPART,	// 가로
		VERT_TEXT_COMPART,	// 세로
		CENTER_HORZ_TEXT_COMPART // 가로(Text가 중앙에 있기)
	};

	// 세로 scale unit type
	enum VERTSCALEUNITTYPE
	{
		VERT_NUM,		// 숫자 x1000
		VERT_NUM_CHAR	// 숫자+문자 x1000, x만
	};

	// (2006/11/11 - Seung-Won, Bae) Define Y Scale Min/Max Setting Type
	enum VERTMINMAXTYPE
	{
		VERT_MINMAX_OF_VIEW,
		VERT_MINMAX_OF_FULL,
		VERT_MINMAX_OF_USER
	};
};

// ----------------------------------------------------------------------------
class CCalculateBaseData
{
public:
	enum CALCULATETYPE
	{
		TRDATA_CAL,
		REALDATA_CAL,
		SCROLL_CAL
	};
};

class CPacketType
{
public:
	// real type
	enum REALTYPE
	{
		FLOOR,		// 버림. 09:01:00 ~ 09:01:59 -> 09:01
		CEILING,	// 올림. 09:00:01 ~ 09:01:00 -> 09:01 (메리츠)
		CEILING2	// 올림. 09:00:00 ~ 09:00:59 -> 09:01 (KOSCOM)
	};

	enum REALMODE
	{
		APPEND,	
		UPDATE
	};

	enum REQUESTNEWPACKETTYPE	// 새로 요청하는 패킷의 타입
	{
		PRICE_ORIGINAL,
		PRICE_MODIFY,
		EX_PACKET,
		NEWS_PACKET,
		ACCOUNT_ITEMIZE_PACKET
	};

	enum PRICETYPE
	{
		ORIGINAL,	// 일반주가
		MODIFY		// 수정주가
	};

	enum EX
	{	
		EX_NONE = -1,
		EX_ALL = 0,			// 락
		EX_DIVIDEND,	// 배당락
		EX_RIGHTS		// 권리락		
	};

	enum COMMODITY_BOND_NAME
	{
		NONAME = -1,
		STOCK = 0,		// 주식
		INDUSTRY,		// 업종
		FUTURES,		// 선물
		OPTION,			// 옵션
		ELW,			// ELW
		FOREIGNINDEX,	// 해외지수
		FOFOREIGN		// 해외선물
	};

	enum ETC
	{
		NEWS,			// 뉴스
		ACCOUNT_ITEMIZE // 거래내역
	};

	enum MARKETDAYTYPE
	{
		TODAY_ONLY		= 0,
		WITH_NEXT_DAY
	};
};

class CPacketDataInfo  
{
public:
	enum DATAINSERTTYPE
	{
		NONE = -1,
		NEW,
		INSERT,
		APPEND,
		REAL_APPEND,
		PART_NEW
	};
};

// ----------------------------------------------------------------------------
class CMenuType
{
public:
	// menu의 종류
	enum MENUTYPE
	{
		MENU_NONE = -1,			// menu 가 없다.
		MENU_GRAPH_HIGHLIGHT,	// 강조가 있는 graph menu
		MENU_GRAPH_NO_HIGHLIGHT,// 강조가 없는 graph menu
		MENU_DATAVIEWTYPE_GRAPH,// graph type 이 <DataView_Type>일 경우의 graph menu
		MENU_HORZ_SCALE,		// 가로 scale menu
		MENU_VERT_SCALE,		// 세로 scale menu
		MENU_VERT_HORZ_SCALE,	// 세로,가로 scale menu
		MENU_ANALYSTOOL,		// 분석 도구 menu
		MENU_SYMBOL				// symbol menu
	};

	enum ELEMENTMENU
	{
		MENU_DELETE,	// 삭제
		MENU_SET,		// 설정 Dlg
		MENU_COLOR,		// color
		MENU_THICKNESS,	// thickness
		MENU_HIGHLIGHT,	// 강조
		MENU_DRAWSTYLE,	// draw style
		MENU_DATAINDEX,	// data index를 구하기
		MENU_HORZSCALE_POSITION, // x축 scale 위치
		MENU_INVERTSCALE,	// 지표 뒤집기 - ojtaso (20071023) 
		MENU_FIBO_SET,		// 피보나치 비율값 설정 Dlg		// 2008.08.20 김진순 피보나치 확장
		MENU_TIME_DIFF,	// graph 시간대 // // 20081007 JS.Kim	사용자시간대 선택
		MENU_DELETE_ALL,	// 전체삭제
	};
};


class CCursorShape
{
public:
	enum CHARTOBJECT
	{
		OBJECT_NONE,
		BLOCKPOS_SPLITH, // 가로(행)
		BLOCKPOS_SPLITV, // 세로(열)
		BLOCKPOS_SPLITA, // 가로, 세로(행,열)
		OBJECT_SELECT,	 // 선택하기
		OBJECT_MOVE,	 // 이동하기
		TOOL_DRAW,		 // 객체그리기
		TOOL_NODROP,	 // 이동 안됨
		TOOL_SIZEALL,	 // 객체이동
		TOOL_SIZENESW,   // 객체크기변경
		TOOL_AUTOTREND,	 // 자동추세분석
		VIEW_ZOOM_IN,	 // 확대
		WHOLE_VIEW,		 // 전체보기
		OBJECT_REMOVE,		// Remove
		OBJECT_INSERT,		// Insert Graph 분리/삽입 - ojtaso (20070731)
	};
};


class CToolOptionInfo
{
public:
	enum OPTION
	{
		INNER,
		OUTER,
		MENU
	};

	enum TOOLOPTION
	{
		T_INFLATE,						// 확대,축소,
		T_WHOLEVIEW,					// 전체보기
		T_NIDLG_DRAG,					// 수치조회창, // (2006/6/28 - Seung-Won, Bae) Rename for Actual Function with Old Map Usage.
		T_DATALIST_REPORT,				// 데이터보기,   
		T_INQUIRY_STATUS,				// 수치조회데이터보기,
		T_OBJ_CFG,						// 객체설정,   
		T_OBJ_HIGHLIGHTt,				// 객체강조,
		T_GRAPH_STYLE,					// Graph형태,   
		T_GRAPH_THICKNESS,				// 선굵기,   
		T_GRAPH_COLOR,					// 색설정,   
		T_INDICATOR_CFG,				// 지표설정,   
		T_ENVIRONMENT_CFG,				// 환경설정,
		T_INDICATOR_LIST,				// 지표리스트보기,
		T_INDICATOR_SERIES,				// 지표연속조회, 
		T_PANORAMA,						// 파노라마,     
		T_SCROLL,						// 스크롤바,
		T_PREV,							// 이전,
		T_NEXT,							// 이후,
		T_RESTORE,						// 초기화,  		
		T_REMOVE_OBJ,					// 객체삭제, 
		T_REMOVE_BLOCK,					// 블럭삭제, 
		T_CHART_RAINBOW,				// 레인보우, 
		T_CHART_BALANCE,				// 일목균형표, 
		T_CHART_PANDF,					// P&F,   
		T_CHART_THREELINEBREAK,			// 삼선전환도,
		T_CHART_REVERSE,				// 역시계곡선, 
		T_CHART_BARFORSALE,				// 대기매물차트, 
		T_SYMBOL_BULET,					// 기호Dlg, 
		T_SYMBOL_SQUARE,				// 사각도형
		T_SYMBOL_CIRCLE,				// 원형도형
		T_SYMBOL_TRIANGLE,				// 삼각형
		T_SYMBOL_TEXT,					// 문자
		T_TOOL_SELECTION,				// 선택
		T_TOOL_LINE_PROFIT,				// 추세선수익률
		T_TOOL_LINE,					// 추세선
		T_CROSSHAIRS_DRAG,				// Crosshairs with drag // (2006/6/28 - Seung-Won, Bae) Rename for Actual Function with Old Map Usage.
		T_TOOL_LINE_VERT,				// 수직선
		T_TOOL_LINE_HORZ,				// 수평선
		T_TOOL_LINE_TRISECT,			// 삼등분선
		T_TOOL_LINE_QUARTER,			// 사등분선
		T_TOOL_LINE_RESISTANCE,			// 지지저항선
		T_TOOL_FIBONACCI_ARC,			// 피보나치 반원
		T_TOOL_FIBO_CIRCLE,				// 피보나치 원
		T_TOOL_FIBONACCI_FAN,			// 피보나치 팬라인
		T_TOOL_FIBO_RETRACE,			// 피보나치 되돌림(조정대)
		T_TOOL_FIBONACCI_TIME,			// 피보나치 시간대
		T_TOOL_GANN_LINE45,				// 갠라인(우상45도)
		T_TOOL_GANN_LINE315,			// 갠라인(우하45도)
		T_TOOL_GANN_FAN45,				// 갠펜(우상45도)
		T_TOOL_GANN_FAN135,				// 갠펜(좌상45도)
		T_TOOL_GANN_FAN225,				// 갠펜(좌하45도)
		T_TOOL_GANN_FAN315,				// 갠펜(우하45도)
		T_TOOL_GANN_GRID,				// 갠그리드
		T_TOOL_ANDREWS_PITCHFORK,		// 엔드류 피치포크
		T_TOOL_ELLIOT_WAVE_LINE,		// 엘리오트 파동선
		T_TOOL_CYCLE_LINES,				// 사이클 구간
		T_TOOL_CUT_LINE,				// 꺽은선 분석라인
		T_TOOL_SPEED_LINE,				// 스피드 라인
		T_TOOL_LINEAR_REGRESSION,		// 선형 회귀선(회귀선)
		T_TOOL_RAFF_REGRESSION,			// Raff 채널선(회귀채널)
		T_DELETE_INORDER,				// 순차적으로 툴 지우기
		T_DELETE_ALL,					// 모두지우기
		T_TRADER_REPORT,				// 모의매매, 
		T_SYSTEM_REPORT,				// 시스템 트레이더 변수최적화, 
		T_PRICE_FORCAST,				// 주가예측,    
		T_MAKE_FORMULA,					// 공식추가,   
		T_PATTERN,						// 봉패턴분석,   
		T_SAVE_CHART_SCREEN,			// 화면저장,   
		T_PRINT,						// 프린트,
		T_DAILY,						// 일봉
		T_WEEKLY,						// 주봉
		T_MONTHLY,						// 월봉
		T_DISPLAY_SLIDERBAR,			// Display수 설정
		T_DISPLAY_RESTORE,				// Display수 초기화
		T_MINMAX_FULLDATA,				// 전체 최대 최소 보기
		T_MAGNIFYINGGLASS,				// 돋보기
		T_RADAR,						// 레이더차트
		T_REGISTER_CONCERN,				// 관심등록
		T_JOINT_ChARTPAGE,				// 차트 공유
		T_SHOW_OUTERTOOLBAR,			// 외부툴바보이기
		T_AUTOLINE_ANALYSIS,			// 자동추세분석		// (2005/11/17 - Seung-Won, Bae) Not Used Now.
		T_TARGET_ANALYSIS,				// 목표치분석
		T_SYSTEM_TRADE_INDICATOR,		// 시스템 트레이더 지표최적화 
		T_STRATEGIC_DEVELOPMENT,		// 전략개발
		T_STRATEGIC_VALUATION,			// 전략평가
		T_TOOL_LINE_EXT1,				// 단방향 연장 추세선	// (2004.12.09, 배승원) 추세선의 연장기능을 추가한다.
		T_TOOL_LINE_EXT2,				// 양방향 연장 추세선
		T_AUTOSCROLL,					// 자동스크롤	
		T_CHART_INVERT,					// 차트뒤집기.
		T_CROSSHAIRS_TOOL,				// (2006/7/3 - Seung-Won, Bae) Crosshairs and NIDLG Tools
		T_CROSSHAIRS_TOOL_WITH_NIDLG,
		T_NIDLG_TOOL,
		T_TOOL_LINE_CROSS,				// 십자선 (CrossLine Object Tool)
		T_TOOL_LINE_CROSS_WITH_NIDLG,
		T_NIDLG_OVER,					// (2006/2/16 - Seung-Won, Bae) Over NIDLG and Crosshairs
		T_CROSSHAIRS_OVER,	
		T_TOOL_SPEED_FAN,				// 스피드 Fan추세선
		T_TOOL_SPEED_ARC,				// 스피드 Arc추세선
		T_TOOL_ANGLE_LINE,				// Angle Line
		T_TOOL_CANDLE_LINE,				// Candle중심선
		T_TOOL_CANDLE_BONG,				// Candle봉
		T_TOOL_TRACEING_TT,				// TracingToolTip추세선
		T_TOOL_ADJUST_RATIO,			// 조정백분율
		T_TOOL_PERIOD_SUMMARY,			// 기간요약선
		T_ENABLE_TIMEMARK,				// (2006/11/24 - Seung-Won, Bae) TimeMark, Fire&Receiving Enable Command Tool
		T_PRICE_YSCALE_SETUP,			// (2006/11/25 - Seung-Won, Bae) Price Chart YScale Setup Tool
		T_DRAG_X_ZOOM,					// (2006/11/27 - Sang-Woo, Cho) XScale Zoom
		T_DRAG_Y_ZOOM,					// (2006/11/27 - Sang-Woo, Cho) YScale Zoom
		T_PAN,							// (2006/11/27 - Sang-Woo, Cho) Pan
		T_UNDO_WORK,					// (2006/12/26 - Sang-Woo, Cho ) Undo
		T_REDO_WORK,					// (2006/12/26 - Sang-Woo, Cho ) Redo
		T_TOOL_PENCIL_LINE,				// 연필추세선
		T_PRINT_CHART,					// 인쇄
		T_ORDER_LINE,					// 주문선
		T_ORDER_LINE_MANAGER,			// 주문선 관리
		T_EXCEL_IMPORT,					// 엑셀데이터 불러오기
		T_ORDER_LINK,					// (2008/09/01 - Jin-Soon, Kim) 주문창 연동 
		T_DATA_SIMULATOR,				// 20081011 이문수 데이터시뮬레이션 >>
		T_TOOL_BALANCE_BASIC,			// 일목수치			// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청

		T_TOOLOPTION_COUNT,				// (2006/11/24 - Seung-Won, Bae) OCX Tool Counts
		//////////////////////////////////////////////////////////////////////
		// (2005/10/26 - Seung-Won, Bae) Add AddIn Tool ID
		//////////////////////////////////////////////////////////////////////
		// Order Line AddIn
		T_ADDIN_ORDER_LINE = 1000,

		// Auto Scroll AddIn
		T_ADDIN_AUTOSCROLL_PLAY_LEFT = 1010,	
		T_ADDIN_AUTOSCROLL_PLAY_RIGHT,
		T_ADDIN_AUTOSCROLL_STOP,

		// Refresh the Button State
		T_ALL_TOOL_TRUE_QUERY = 1020,
		T_ALL_TOOL_RESET_STATE,

		T_SELECT_ENV_TAB_INDEX = 2000	// 환경설정 탭별 선택 가능 - ojtaso (20080109)
	};
};

// (2006/1/20 - Seung-Won, Bae) from OCX for sharing with AddIn.
// 수치조회 관련 Definition
#define NUMERICALINQUIRYDLG 0x1
#define CROSSLINEVIEW		0x2
#define TOOLTIPVIEW			0x4
#define NIDLG_MANUALMOVE	0x8
#define TIMEMARKER			0x10	// 시간연동선 사용 옵션
#define TOOLTIPVIEW_ALL		0x20	// 풍선도움말보기시 전체보기

// Scroll할 데이타 타입을 지정한다. 
//#define SETSCROLL_TR			1	// TR데이타가 들어오는 경우.
//#define SETSCROLL_REAL		2	// 리얼(자동갱신)데이타가 들어오는 경우.
//#define SETSCROLL_REMOVEDATA	3	// 데이타가 너무 많아 오래된 데이타를 지우는 경우.
//#define SETSCROLL_WHOLEVIEW		4   // 전체보기

// (2009/9/1 - Seung-Won, Bae) Define Chart Mode.
// class CChartInfo
// {
// public:
// 	enum CHART_MODE
// 	{
// 		DEVELOPER = 0,
// 		HTS,
// 		FX,
// 		WORLD_ON,
// 		KRX_CME
// 	};
// };


class CCommonInterfaceEnum
{
public:
	enum ETCMESSAGETYPE
	{
		PRICE_LOG = 0
	};

	// PacketRQ의 상태를 알리도록 인터페이스 제공 : 복수종목 - ojtaso (20070531)
	enum PACKET_RQ_STATUS
	{
		PACKETLISTMANAGER_NEW_RQ = 0,
		PACKETLISTMANAGER_CHANGE_RQ,
		PACKETLISTMANAGER_DELETE_RQ
	};
};

#endif // !defined(AFX_BLOCKBASEENUM_H__6AF7C18A_2AAF_4C00_82DF_4E49584F150C__INCLUDED_)
