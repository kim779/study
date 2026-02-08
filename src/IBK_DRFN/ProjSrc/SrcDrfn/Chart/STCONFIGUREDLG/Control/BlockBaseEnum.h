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
		TOOL_GRAPHDELETE		// graph title / subGraph tool 의 삭제
	};
};

class CGraphBaseData
{
public:
	enum GRAPHTYPE
	{
		GTNONE = -1,
		Bong_Type = 0,		// 일본식, 미국식, 라인식
		Line_Type,			// 일반식, 채움식, 구름식, 수평식, 계단식, 0선비연속식, 대비식, 레인보우식
		Bar_Type,			// 수직식, 수평식, 상하식, 빗형식, 도트식
		Pie_Type,			// 평면식, 입체식
		Special_Type,		// 삼선전환도, 역시계, P&F
		Fund_Type,			// 펀드형
		DataView_Type,		// data 표시
		FuOp_Type,			// 선물옵션형

		GraphTypeCount		// GRAPHTYPE의 총갯수
	};

	// Graph Style
	enum BONGSTYLE
	{
		CandlestickBong = 0,	// 일본식
		HighLowCloseBong,		// 미국식
		LineBong,				// 라인식
		AntennaBong				// 안테나식
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
		VerticalLine			// 수직식
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
		FixedRateCombBar		// 고정비율빗형식
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
		Swing					// Swing
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
		TopDataBar = 0,			// 상단식 바형
		BottomDataBar,			// 하단식 바형
		PointSignal,			// point signal
		TopSignal,				// 상단 signal
		BottomSignal,			// 하단 signal
		CompartText				// 구분텍스트형
	};

	enum FUOPSTYLE
	{
		MarketProfile,			// MarketProfile
		FuOpPAndF				// FuOpP&F
	};


	// Graph Drawing Type
	// -> SPECIALSTYLE, FUNDSTYLE, OPTIONSTYLE 는 Drawing Type 이 없다!
	enum BONGDRAWSTYLE		// BONGSTYLE
	{
		Candlestick_Draw = 0,	// 일본식
		HighLowClose_Draw,		// 미국식
		Line_Draw,				// 라인식
		Antenna_Draw			// 안테나식
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
		FLOOR,	// 버림. 09:01:00 ~ 09:01:59 -> 09:01
		CEILING // 올림. 09:00:01 ~ 09:01:00 -> 09:01 (메리츠)
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
		STOCK = 0,	// 주식
		INDUSTRY,	// 업종
		FUTURES,	// 선물
		OPTION		// 옵션
	};

	enum ETC
	{
		NEWS,			// 뉴스
		ACCOUNT_ITEMIZE // 거래내역
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
		MENU_HORZSCALE_POSITION // x축 scale 위치
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
		ZOOM_IN
	};
};


class CAnalysis
{
public:
	enum COORDINATETYPE
	{
		RELATIVE_POINT,	// 상대 좌표형 - data 형태
		ABSOLUTE_POINT	// 절대 좌표형 - pixel 형태
	};

	enum ANALYSIS
	{
		ANALYSIS_TOOL,
		ANALYSIS_FUNCTION
	};

	enum TOOLTYPE
	{
		TOOL_NONE = -1,
		TOOL_SELECTION,		// 선택
		TOOL_MOVE_CROSS,	// 십자선(mouse move 시)
		TOOL_DOWN_CROSS,	// 십자선(mouse down 시)
		TOOL_VERT_LINE,		// 수직선
		TOOL_HORZ_LINE,		// 수평선
		TOOL_LINE,			// 추세선
		TOOL_TRISECT,		// 삼등분선
		TOOL_QUARTER,		// 사등분선
		TOOL_FIBO_SEMICIRCLE,// 피보나치 반원
		TOOL_FIBO_CIRCLE,	// 피보나치 원
		TOOL_FIBO_FAN_LINE,	// 피보나치 팬라인
		TOOL_FIBO_RETRACE,	// 피보나치 되돌림(조정대)
		TOOL_FIBO_TIME,		// 피보나치 시간대
		TOOL_GANN_LINE45,	// 갠라인(우상45도)
		TOOL_GANN_LINE315,	// 갠라인(우하45도)
		TOOL_GANN_FAN45,	// 갠펜(우상45도)
		TOOL_GANN_FAN135,	// 갠펜(좌상45도)
		TOOL_GANN_FAN225,	// 갠펜(좌하45도)
		TOOL_GANN_FAN315,	// 갠펜(우하45도)
		TOOL_GANN_GRID,		// 갠그리드
		TOOL_ANDREWS_PITCHFORK,	// 엔드류 피치포크
		TOOL_ELLIOT_WAVE_LINE,	// 엘리오트 파동선
		TOOL_CYCLE_LINES,	// 사이클 구간
		TOOL_CUT_LINE,		// 꺽은선 분석라인
		TOOL_SPEED_LINE,	// 스피드 라인
		TOOL_LINEAR_REGRESSION,// 선형 회귀선(회귀선)
		TOOL_RAFF_REGRESSION,// Raff 채널선(회귀채널)
		// (2004.12.09, 배승원) 추세선의 연장기능을 추가한다.
		TOOL_LINE_EXT1,		// 단방향 연장 추세선
		TOOL_LINE_EXT2		// 양방향 연장 추세선
	};
};


class CSymbol
{
public:
	enum SYMBOLTOOL
	{
		SYMBOL_NONE,
		SYMBOL_SQUARE, //사각도형
		SYMBOL_CIRCLE,	// 원
		SYMBOL_TRIANGLE,	// 삼각형
		SYMBOL_TEXT,  // 문자
		SYMBOL_BULET, //블릿
		SYMBOL_SELECTION //선택
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
		T_INFLATE,			// 확대,축소"),
		T_WHOLEVIEW,		// 전체보기
		T_INQUIRY_VALUE,	// 수치조회창"), 
		T_DATALIST_REPORT,	// 데이터보기"),   
		T_INQUIRY_STATUS,	// 수치조회데이터보기"),
		T_OBJ_CFG,			// 객체설정"),   
		T_OBJ_HIGHLIGHTt,	// 객체강조"),
		T_GRAPH_STYLE,		// Graph형태"),   
		T_GRAPH_THICKNESS,	// 선굵기"),   
		T_GRAPH_COLOR,		// 색설정"),   
		T_INDICATOR_CFG,	// 지표설정"),   
		T_ENVIRONMENT_CFG,	// 환경설정"),
		T_INDICATOR_LIST,	// 지표리스트보기"),
		T_INDICATOR_SERIES,	// 지표연속조회"), 
		T_PANORAMA,			// 파노라마"),     
		T_SCROLL,			// 스크롤바"),
		T_PREV,				// 이전"),
		T_NEXT,				// 이후"),
		T_RESTORE,			// 초기화"),  		
		T_REMOVE_OBJ,		// 객체삭제"), 
		T_REMOVE_BLOCK,		// 블럭삭제"), 
		T_CHART_RAINBOW,	// 레인보우"), 
		T_CHART_BALANCE,	// 일목균형표"), 
		T_CHART_PANDF,		// P&F"),   
		T_CHART_THREELINEBREAK,// 삼선전환도"),
		T_CHART_REVERSE,	// 역시계곡선"), 
		T_CHART_BARFORSALE,	// 대기매물차트"), 
		T_SYMBOL_BULET,		// 기호Dlg"), 
		T_SYMBOL_SQUARE,	// 사각도형
		T_SYMBOL_CIRCLE,	// 원형도형
		T_SYMBOL_TRIANGLE,	// 삼각형
		T_SYMBOL_TEXT,		// 문자
		T_TOOL_SELECTION,		// 선택
		T_TOOL_LINE_PROFIT,		// 추세선수익률
		T_TOOL_LINE,			// 추세선
		T_TOOL_LINE_CROSS,		// 십자선
		T_TOOL_LINE_VERT,		// 수직선
		T_TOOL_LINE_HORZ,		// 수평선
		T_TOOL_LINE_TRISECT,	// 삼등분선
		T_TOOL_LINE_QUARTER,	// 사등분선
		T_TOOL_LINE_RESISTANCE,	// 지지저항선
		T_TOOL_FIBONACCI_ARC,	// 피보나치 반원
		T_TOOL_FIBO_CIRCLE,		// 피보나치 원
		T_TOOL_FIBONACCI_FAN,	// 피보나치 팬라인
		T_TOOL_FIBO_RETRACE,	// 피보나치 되돌림(조정대)
		T_TOOL_FIBONACCI_TIME,	// 피보나치 시간대
		T_TOOL_GANN_LINE45,		// 갠라인(우상45도)
		T_TOOL_GANN_LINE315,	// 갠라인(우하45도)
		T_TOOL_GANN_FAN45,		// 갠펜(우상45도)
		T_TOOL_GANN_FAN135,		// 갠펜(좌상45도)
		T_TOOL_GANN_FAN225,		// 갠펜(좌하45도)
		T_TOOL_GANN_FAN315,		// 갠펜(우하45도)
		T_TOOL_GANN_GRID,		// 갠그리드
		T_TOOL_ANDREWS_PITCHFORK,	// 엔드류 피치포크
		T_TOOL_ELLIOT_WAVE_LINE,	// 엘리오트 파동선
		T_TOOL_CYCLE_LINES,		// 사이클 구간
		T_TOOL_CUT_LINE,		// 꺽은선 분석라인
		T_TOOL_SPEED_LINE,		// 스피드 라인
		T_TOOL_LINEAR_REGRESSION,// 선형 회귀선(회귀선)
		T_TOOL_RAFF_REGRESSION,	// Raff 채널선(회귀채널)
		T_DELETE_INORDER,		// 순차적으로 툴 지우기
		T_DELETE_ALL,			// 모두지우기
		T_TRADER_REPORT,		// 모의매매"), 
		T_SYSTEM_REPORT,		// 시스템 트레이더 변수최적화"), 
		T_PRICE_FORCAST,		// 주가예측"),    
		T_MAKE_FORMULA,			// 공식추가"),   
		T_PATTERN,				// 봉패턴분석"),   
		T_SAVE_CHART_SCREEN,	// 화면저장"),   
		T_PRINT,				// 프린트"),
		T_DAILY,				// 일봉")
		T_WEEKLY,				// 주봉
		T_MONTHLY,				// 월봉
		T_DISPLAY_SLIDERBAR,	// Display수 설정
		T_DISPLAY_RESTORE,		// Display수 초기화
		T_MINMAX_FULLDATA,		// 전체 최대 최소 보기
		T_MAGNIFYINGGLASS,		// 돋보기
		T_RADAR,				// 레이더차트
		T_REGISTER_CONCERN,		// 관심등록
		T_JOINT_ChARTPAGE,		// 차트 공유
		T_SHOW_OUTERTOOLBAR,	// 외부툴바보이기
		T_AUTOLINE_ANALYSIS,	// 자동추세분석
		T_TARGET_ANALYSIS,		// 목표치분석
		T_SYSTEM_TRADE_INDICATOR,	// 시스템 트레이더 지표최적화 
		T_STRATEGIC_DEVELOPMENT,	// 전략개발
		T_STRATEGIC_VALUATION,		// 전략평가
		// (2004.12.09, 배승원) 추세선의 연장기능을 추가한다.
		T_TOOL_LINE_EXT1,		// 단방향 연장 추세선
		T_TOOL_LINE_EXT2		// 양방향 연장 추세선
	};
};

class CIndicatorBaseData  
{
public:
	enum INDICATOR_NAME_NO_CAL
	{
		INDI_PRICE = 0,	// 가격차트
		INDI_VOLUME,	// 거래량차트
		INDI_OPTIMIZE_SIGNAL,	// IndicatorOptimize Signal

		NUM_NO_CALCULATION
	};

	enum INDICATOR_NAME_CAL
	{
		INDI_PRICE_MA = 0,	// 가격 이동평균
		INDI_VOLUME_MA,		// 거래량 이동평균
		INDI_HORIZONTALBAR,	// 대기매물
		INDI_DISPARITY,		// 이격도
		INDI_GLANCEBALANCE,	// 일목균형표
		INDI_PSYCH,			// 투자심리선
		INDI_NEWPSYCH,		// 신투자심리선
		INDI_AB_RATIO,		// AB Ratio
		INDI_ATR,			// ATR : Average True Range
		INDI_BOLLINGER,		// Bollinger Bands
		INDI_BOXCHART,		// Box Chart
		INDI_CCI,			// CCI
		INDI_CLC,			// 꺽은선차트
		INDI_CMF,			// CMF
		INDI_CHAIKINS_OSC,	// CO : Chaikin's Oscillator
		INDI_DMI,			// DMI
		INDI_ENVELOPE,		// Envelope
		INDI_IMI,			// IMI
		INDI_FOURIER_TRANSFORM,	// Fourier Transform
		INDI_MAC,			// MAC
		INDI_MACD,			// MACD
		INDI_MACD_OSC,		// MACD Oscillator
		INDI_MFI,			// MFI
		INDI_MI,			// MI : MassIndex
		INDI_MOMENTUM,		// Momentum
		INDI_NVI,			// NVI : Negative Volume Index
		INDI_OBV,			// OBV
		INDI_OCMA,			// OCMA
		INDI_OSCV,			// OSCV : Volume Oscillator
		INDI_PARABOLIC,		// Parabolic
		INDI_PIVOT,			// Pivot
		INDI_PVI,			// PVI : Positive Volume Index
		INDI_OSCP,			// OSCP : Price Oscillator
		INDI_PVT,			// PVT : Price Volume Trend
		INDI_RSCOMPARATIVE,	// 상대비교차트
		INDI_ROC,			// RoC : Rate of Change
		INDI_RSI,			// RSI
		INDI_SONAR,			// SONAR
		INDI_STOCHASTICS_FAST,	// Stochastics Fast
		INDI_STOCHASTICS_SLOW,	// Stochastics Slow
		INDI_STOCHASTICS_OSC,	// Stochastics Oscillator
		INDI_TRIN,			// TRIN
		INDI_VOLUME_ROC,	// VROC : Volume Rate of Change
		INDI_VR,			// VR
		INDI_WILLIAM,		// William's %R
		INDI_TRIX,			// TRIX
		INDI_RAINBOW,		// 그물차트
		INDI_THREE_LINE_BREAK,	// 삼선전환도
		INDI_INVERSE_LINE,		// 역시계곡선
		INDI_PF,				// P&F
		INDI_ACCOUNTITEMIZE,	// 거래내역
		INDI_LOCK,			// 락
		INDI_BIGNEWS,		// Big News
		INDI_AD,			// A/D Line : Accumulation/Distribution
		INDI_ZIGZAG,		// Zig Zag
		INDI_EMV,			// EMV
		INDI_DPO,			// DPO
		INDI_ABI,			// ABI
		INDI_ADL,			// ADL
		INDI_ADR,			// ADR
		INDI_BREADTH_THRUST,		// Breadth Thrust
		INDI_MCCLELLAN_OSC,			// McClellan Oscillator
		INDI_STANDARD_DEVIATION,	// Standard Deviation
		INDI_STIX,			// STIX
		INDI_VHF,			// VHF
		INDI_ADX,			// ADX
		INDI_WEIGHTCLOSE,	// Wighted Close
		INDI_CHAIKIN,		// Chaikin's Volatility
		INDI_BANDB,			// Band %B
		INDI_BANDWIDTH,		// Band Width
		INDI_BINARYWAVE,	// Binary Wave
		INDI_EOM,			// EOM
		INDI_DEMARK,		// Demark
		INDI_WILLIAMAD,		// Will A/D
		INDI_SMI,			// SMI
		INDI_SWINGINDEX,	// SWINGINDEX
		INDI_ASI,			// ASI
		INDI_RSQUARED,		// RSquared
		INDI_STANDARDERROR,	// Standard Error
		INDI_ENERGY,		// Energy
		INDI_TSF_OSC,		// TSF Oscillator
		INDI_TSI,			// TSI
		INDI_MACHANNEL,		// 이동평균 채널
		INDI_NCO,			// NCO
		INDI_SIGMA,			// SIGMA
		INDI_MA_OSC,		// MA Oscillator
		INDI_TSF,			// TSF
		INDI_LRS,			// LRS
		INDI_CANDLEVOLUME,	// CandleVolume
		
		NUM_CALCULATION
	};
};

#endif // !defined(AFX_BLOCKBASEENUM_H__6AF7C18A_2AAF_4C00_82DF_4E49584F150C__INCLUDED_)
