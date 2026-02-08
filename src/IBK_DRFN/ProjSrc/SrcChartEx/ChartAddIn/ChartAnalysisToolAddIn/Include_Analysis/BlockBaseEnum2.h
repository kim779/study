// BlockBaseEnum2.h: defines for Block Data
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCKBASEENUM2_H__6AF7C18A_2AAF_4C00_82DF_4E49584F150C__INCLUDED_)
#define AFX_BLOCKBASEENUM2_H__6AF7C18A_2AAF_4C00_82DF_4E49584F150C__INCLUDED_

// (2004.10.12, 배승원) BlockBaseData.h에서 Enum Data와 같이 Define 상수 만을 별도의 Header로 분리한다.
//		(DLL 등 외부 Module 개발자와 공유하기 위함)
//		* 주의. 따라서 이 부분을 수정시 외부 Module 연계 담당자에게 알려 외부 Module에도 적용되도록 한다.
//			향후 별도의 AddIn Header Folder를 두어 공유토록 조정할 예정
//		*. 또한, 수정시 값의 확장은 가능하나 변경은 되지 않도록 한다.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
		EN_ANALYSIS_NONE = -1,
		EN_ANALYSIS_SELECTION		= 0,		// 선택
		EN_ANALYSIS_MOVE_CROSS		= 1,		// 십자선(mouse move 시)
		EN_ANALYSIS_DOWN_CROSS		= 2,		// 십자선(mouse down 시)
		EN_ANALYSIS_VERT_LINE		= 3,		// 수직선
		EN_ANALYSIS_HORZ_LINE		= 4,		// 수평선
		EN_ANALYSIS_LINE			= 5,		// 추세선
		EN_ANALYSIS_TRISECT			= 6,		// 삼등분선
		EN_ANALYSIS_QUARTER			= 7,		// 사등분선
		EN_ANALYSIS_FIBO_SEMICIRCLE = 8,		// 피보나치 반원
		EN_ANALYSIS_FIBO_CIRCLE		= 9,		// 피보나치 원
		EN_ANALYSIS_FIBO_FAN_LINE	= 10,		// 피보나치 팬라인
		EN_ANALYSIS_FIBO_RETRACE	= 11,		// 피보나치 되돌림(조정대)
		EN_ANALYSIS_FIBO_TIME		= 12,		// 피보나치 시간대
		EN_ANALYSIS_GANN_LINE45		= 13,		// 갠라인(우상45도)
		EN_ANALYSIS_GANN_LINE315	= 14,		// 갠라인(우하45도)
		EN_ANALYSIS_GANN_FAN45		= 15,		// 갠펜(우상45도)
		EN_ANALYSIS_GANN_FAN135		= 16,		// 갠펜(좌상45도)
		EN_ANALYSIS_GANN_FAN225		= 17,		// 갠펜(좌하45도)
		EN_ANALYSIS_GANN_FAN315		= 18,		// 갠펜(우하45도)
		EN_ANALYSIS_GANN_GRID		= 19,		// 갠그리드
		EN_ANALYSIS_ANDREWS_PITCHFORK	= 20,	// 엔드류 피치포크
		EN_ANALYSIS_ELLIOT_WAVE_LINE	= 21,	// 엘리오트 파동선
		EN_ANALYSIS_CYCLE_LINES		= 22,		// 사이클 구간
		EN_ANALYSIS_CUT_LINE		= 23,		// 꺽은선 분석라인
		EN_ANALYSIS_SPEED_LINE		= 24,		// 스피드 라인
		EN_ANALYSIS_SPEED_FAN		= 25,		// 스피드 Fan
		EN_ANALYSIS_SPEED_ARC		= 26,		// 스피드 호
		EN_ANALYSIS_LINEAR_REGRESSION	= 27,	// 선형 회귀선(회귀선)
		EN_ANALYSIS_RAFF_REGRESSION	= 28,		// Raff 채널선(회귀채널)		
		EN_ANALYSIS_ANGLE_LINE		= 29,		// 각도추세선
		EN_ANALYSIS_CANDLE_LINE		= 30,		// 캔들중심선
		EN_ANALYSIS_CANDLE_BONG		= 31,		// 캔들가상봉
		EN_ANALYSIS_ADJUST_RATIO	= 32,		// 조정백분율
		EN_ANALYSIS_PERIOD_SUMMARY	= 33,		// 가격변화선
		EN_ANALYSIS_TRACING_TT		= 34,		// 자동툴팁
		EN_ANALYSIS_PENCIL_LINE		= 35,		// 연필추세선
		EN_ANALYSIS_BALANCE_BASIC	= 36,		// 일목수치			// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청
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

// (2006/1/20 - Seung-Won, Bae) from OCX for sharing with AddIn.
// 수치조회 관련 Definition
#define NUMERICALINQUIRYDLG 0x1
#define CROSSLINEVIEW		0x2
#define TOOLTIPVIEW			0x4
#define NIDLG_MANUALMOVE	0x8
#define TIMEMARKER			0x10	// 시간연동선 사용 옵션
#define TOOLTIPVIEW_ALL		0x20	// 풍선도움말보기시 전체보기

#endif // !defined(AFX_BLOCKBASEENUM2_H__6AF7C18A_2AAF_4C00_82DF_4E49584F150C__INCLUDED_)
