#ifndef __LISTCTRLDATA_H__
#define __LISTCTRLDATA_H__

#include <atlconv.h>

///////////////
//data
static const int NUM_TOOLRBUTTONITEMS = 82;

static const _TCHAR *pToolRButtonItem[NUM_TOOLRBUTTONITEMS] = 
{
	_T("확대,축소"),  
	_T("전체보기"),  
	_T("수치조회창"), 
	_T("데이터보기"),   
	_T("수치조회데이터보기"),
	_T("객체설정"),  
	_T("객체강조"),  
	_T("Graph형태"),   
	_T("선굵기"),   
	_T("색설정"),   
	_T("지표설정"),   
	_T("환경설정"),
	_T("지표리스트보기"),
	_T("지표연속조회"), 
	_T("파노라마"),     
	_T("스크롤바"),
	_T("이전"),
	_T("이후"),
	_T("초기화"),  
	_T("객체삭제"), 
	_T("블럭삭제"), 
	_T("레인보우"), 
	_T("일목균형표"), 
	_T("P&F"),   
	_T("삼선전환도"),
	_T("역시계곡선"), 
	_T("대기매물차트"), 
	_T("기호Dlg"), 
	_T("사각도형"), 
	_T("원형도형"), 
	_T("삼각형"), 
	_T("Text"), 
	_T("선택"), 
	_T("추세선수익률"), 
	_T("추세선"), 
	_T("십자선"), 
	_T("수직선"), 
	_T("수평선"), 
	_T("삼등분선"), 
	_T("사등분선"), 
	_T("지지저항선"), 
	_T("피보나치 반원"), 
	_T("피보나치 원"), 
	_T("피보나치 팬라인"), 
	_T("피보나치 되돌림"), 
	_T("피보나치 시간대"), 
	_T("갠라인(우상45도)"), 
	_T("갠라인(우하45도)"), 
	_T("갠펜(우상45도)"), 
	_T("갠펜(좌상45도)"), 
	_T("갠펜(우하45도)"), 
	_T("갠펜(좌하45도)"), 
	_T("갠그리드"), 
	_T("엔드류 피치포크"), 
	_T("엘리오트 파동선"), 
	_T("타임 사이클"), 
	_T("꺽은선 분석라인"), 
	_T("스피드 라인"), 
	_T("선형 회귀선(회귀선)"), 
	_T("Raff 채널선(회귀채널)"), 
	_T("순차적으로 툴 지우기"), 
	_T("모두지우기"), 
	_T("모의매매"), 
	_T("시스템 트레이더"), 
	_T("주가예측"),    
	_T("공식추가"),   
	_T("패턴분석"),   
	_T("화면저장"),   
	_T("프린트"),
	_T("일봉"),
	_T("주봉"),
	_T("월봉"),
	_T("Display수 설정바"),
	_T("Display수 초기화"),
	_T("전체 최대최소 보기"),
	_T("돋보기"),
	_T("레이더차트"),
	_T("관심등록"),
	_T("차트공유"),
	_T("외부툴바보이기"),
	_T("자동추세분석"),
	_T("목표치분석")
};

//data
static const int NUM_PACKETNAMEITEM = 23;

static const _TCHAR *pPacketNameItem[NUM_PACKETNAMEITEM] = 
{
	_T("자료일자"),  
	_T("Text"), 
	_T("시가"),   
	_T("고가"),   
	_T("저가"), 
	_T("종가"), 
	_T("기본거래량"),     
	_T("누적거래량"),    
	_T("락유무"),    
	_T("주식배수"),    
	_T("환율"),    
	_T("전체종목수"),    
	_T("상승종목수"),    
	_T("하락종목수"),    
	_T("ADL"),    
	_T("MOBV"),    
	_T("상승종목거래량"),    
	_T("하락종목거래량"),    
	_T("선도주 거래량"),    
	_T("52신고종목수"),    
	_T("52신저종목수"),    
	_T("상장주식수"),    
	_T("거래형성종목수")    
};

static const int NUM_GRAPHNAMEITEM = 170;

static const _TCHAR *pGraphNameItem[NUM_GRAPHNAMEITEM] = 
{
	_T("가격차트"),				// 0
	_T("거래량차트"),  
	_T("가격 이동평균"),
	_T("거래량 이동평균"),
	_T("대기매물"), 	
	_T("이격도"),     
	_T("일목균형표"),	
	_T("투자심리선"),
    _T("신심리선"),
	_T("AB Ratio"),    
	_T("ATR"),    				// 10
	_T("Bollinger Bands"),
    _T("Box Chart"),
	_T("CCI"), 
	_T("꺽은선차트"),
	_T("CMF"),
	_T("CO"),    
	_T("DMI"),    
	_T("Envelope"),
    _T("IMI"),
	_T("Fourier Transform"),   	// 20
	_T("MAC"),    
	_T("MACD"),    
	_T("MACD Oscillator"),    
	_T("MFI"),    
	_T("MI"),    
	_T("Momentum"),    
	_T("NVI"),    
	_T("OBV"),    
	_T("OCMA"),
	_T("OSCV"), 				// 30
	_T("Parabolic"), 
	_T("Pivot"), 
	_T("PVI"), 
	_T("OSCP"), 
	_T("PVT"), 
	_T("상대비교차트"), 
	_T("RoC"),    
	_T("RSI"), 	
	_T("SONAR"), 	
	_T("Stochastics Fast"), 	// 40
	_T("Stochastics Slow"),
	_T("Stochastics Oscillator"),
	_T("TRIN"),
	_T("VROC"), 
	_T("VR"), 	
	_T("William's %R"), 	
	_T("TRIX"), 	
	_T("그물차트"),	
	_T("삼선전환도"),
	_T("역시계곡선"), 			// 50
	_T("P&F"),
	_T("FuOpP&F"),
	_T("거래내역"),
	_T("락"),
	_T("Big News"),
	_T("MarketProfile"),

	///대우의 지표에서 추가된 내용
	_T("A/D Line"),
	_T("Zig Zag"),
	_T("EMV"),
	_T("DPO"),					// 60
	_T("ABI"),
	_T("ADL"),
	_T("ADR"),
	_T("Breadth Thrust"),
	_T("McClellan Oscillator"),
	_T("Standard Deviation"),
	_T("STIX"),
	_T("VHF"),

	//교보
	_T("ADX"),
	_T("Weighted Close"),		// 70
	_T("Chaikin's Volatility"),
	_T("Band %B"),
	_T("Band Width"),
	_T("Binary Wave"),
	_T("EOM"),
	_T("Demark"),
	_T("Will A/D"),
	_T("SMI"),
	_T("Swing Index"),
	_T("ASI"),					// 80
	_T("RSquared"),
	_T("Standard Error"),
	_T("Energy"),
	_T("TSF Oscillator"),
	_T("TSI"),
	_T("이동평균 채널"),
	_T("NCO"),
	_T("SIGMA"),
	_T("MA Oscillator"),
	_T("TSF"),					// 90
	_T("LRS"),
	_T("CandleVolume"),
	_T("IndicatorOptimize Signal"),
	_T("SwingChart"),
	_T("KagiChart"),
	_T("RenkoChart"),			// YJS 05/06/21 RenKoChart ADD.
	// ADD: 종찬(04/04/06) 메리츠증권 시장지표 10개추가
	_T("고객예탁금"),				
	_T("선물,옵션 미결제 약정"),	
	_T("선물거래예수금"),			
	_T("시장지수"),				// 100	
	_T("신용융자잔고"), 
	_T("업종지수"),					
	_T("외국인지분율"),		
	_T("외국인순매수"),				
	_T("위탁자미수금"),
	// ADD: 종찬(04/04/06) 메리츠증권 시장지표 3개추가
	_T("개인투자자"), 
	_T("기관투자자"),
	_T("외국인투자자"),

	// 한화
	_T("7Binary Wave"), 
	_T("VA Oscillator"),			// 110
	_T("Price Channel"), 
	_T("Aroon"), 
	_T("Formula"), 
	_T("TDI"), 
	_T("Elder Ray Bear"),
	_T("Elder Ray Bull"),
	_T("Standard Error"), 
	_T("Volume Ratio"), 
	_T("VROC"), 
	_T("등락주선(ADL)"),	// 120
	_T("등락주비율(ADR)"), 
	_T("심리도"), 
	_T("DEMA"), 
	_T("TEMA"), 
	_T("이격도(단순)"),
	_T("이격도(지수)"),
	_T("Starc Bands"),
	_T("NDI"),
	_T("%R"),
	_T("Qstic"),	// 130
	_T("Force Index"),
	_T("%B+Stochastic"),
	_T("LRS+Stochastic"),
	_T("MACD+Stochastic"),
	_T("Momentum+Stochastic"),
	_T("OBV+Momentum"),
	_T("OBV+Stochastic"),
	_T("ROC+Stochastic"),
	_T("RSI+MACD"),
	_T("RSI+Stochastic"),	// 140
	_T("Sonar+심리도"),
	_T("Stochastic+RSI"),
	_T("표준오차밴드"),
	_T("표준오차채널"),
	_T("표준편차채널"),
	_T("Parabolic SAR"),
	_T("ADXR"),
	_T("MAO"),
	_T("Price Oscillator"),
	_T("Chaikin's Oscillator"),	// 150
	_T("Volume Oscillator"),
	_T("SROC"),
	_T("Mass Index"),
	_T("RMI"),
	_T("DRF"),
	_T("금리"),
	_T("뉴스 강도"),
	_T("뉴스 건수"),
	_T("시가총액"),
	_T("신용잔고"),	// 160
	_T("예탁금"),
	_T("외국인보유량"),
	_T("외국인보유율"),
	_T("외국인소진율"),
	_T("투자자순매수(선물)"),
	_T("투자자순매수(Call)"),
	_T("투자자순매수(KOSDAQ)"),
	_T("투자자순매수(KOSPI)"),
	_T("투자자순매수(Put)"),
	_T("매수매도대기매물"),
	_T("Energy"),
	_T("신용잔고율"),
	_T("신용공여율"),
	_T("신용잔고수량"),
	_T("신용잔고증감")
};

typedef enum FORMULATE
{
	GRAPH_PRICE = 0,				// 0
	GRAPH_VOLUME,
	GRAPH_PRICE_MA,
	GRAPH_VOLUME_MA,
	GRAPH_HORIZONTALBAR,
	GRAPH_DISPARITY,
	GRAPH_GLANCEBALANCE,
	GRAPH_PSYCH,
	GRAPH_NEWPSYCH,
	GRAPH_AB_RATIO,
	GRAPH_ATR,					// 10
	GRAPH_BOLLINGER,
	GRAPH_BOXCHART,
	GRAPH_CCI,
	GRAPH_CLC,
	GRAPH_CMF,
	GRAPH_CO,
	GRAPH_DMI,
	GRAPH_ENVELOPE,
	GRAPH_IMI,
	GRAPH_FOURIER_TRANSFORM,	// 20
	GRAPH_MAC,				
	GRAPH_MACD,				
	GRAPH_MACD_OSC,			
	GRAPH_MFI,				
	GRAPH_MI,				
	GRAPH_MOMENTUM,	
	GRAPH_NEGATIVE_VOLUME_INDEX, // NVI
	GRAPH_OBV,				
	GRAPH_OCMA,	
	GRAPH_OSCV,					// 30
	GRAPH_PARABOLIC,		
	GRAPH_PIVOT,				
	GRAPH_POSITIVE_VOLUME_INDEX, // PVI
	GRAPH_OSCP,
	GRAPH_PRICE_VOLUME_TREND, // PVT
	GRAPH_RELATIVE_STRENGTH_COMPARATIVE,
	GRAPH_ROC,	
	GRAPH_RSI,				
	GRAPH_SONAR,			
	GRAPH_STOCHASTICS_FAST,		// 40
	GRAPH_STOCHASTICS_SLOW,
	GRAPH_STOCHASTICS_OSCILLATOR,
	GRAPH_TRIN,
	GRAPH_VOLUME_ROC,	
	GRAPH_VR,			
	GRAPH_WILLIAM,			
	GRAPH_TRIX,
	GRAPH_RAINBOW,
	GRAPH_THREE_LINE_BREAK,
	GRAPH_INVERSE_LINE,			// 50
	GRAPH_P_AND_F,
	GRAPH_FUOPPF,
	GRAPH_AccountItemize,
	GRAPH_Lock,
	GRAPH_BigNews,
	GRAPH_MarketProfile,

	///김찬중...대우소스에서 추가한 지표
	GRAPH_AD,					/// A/D Line : Accumulation/Distribution
	GRAPH_ZIGZAG,				/// Zig Zag
	GRAPH_EMV,					/// Ease Of Movement
	GRAPH_DPO,					/// Detreded Price Oscillator
	GRAPH_ABI,					/// Absolute Breadth Index
	GRAPH_ADL,					/// Advanced Decline Line
	GRAPH_ADR,					/// Advanced Decline Ratio
	GRAPH_BREADTH_THRUST,		/// Breadth Thrust
	GRAPH_MCCLELLAN_OSCILLATOR,	/// McClellan Oscillator
	GRAPH_STANDARD_DEVIATION,	/// Standard Deviation
	GRAPH_STIX,					/// STIX
	GRAPH_VHF,					/// Vertical horizontal Filter

	//교보
	GRAPH_ADX,
	GRAPH_WEIGHTCLOSE,
	GRAPH_ChaikinsVolatility,
	GRAPH_BANDB,
	GRAPH_BandWidth,
	GRAPH_BinaryWave,
	GRAPH_EOM,
	GRAPH_Demark,
	GRAPH_WilliamAD,
	GRAPH_SMI,
	GRAPH_SwingIndex,
	GRAPH_ASI,
	GRAPH_RSquared,
	GRAPH_StandardError,
	GRAPH_Energy,
	GRAPH_TSFOscillator,
	GRAPH_TSI,
	GRAPH_MAChannel,
	GRAPH_NCO,
	GRAPH_Sigma,
	GRAPH_MaOscillator,
	GRAPH_TSF,
//	GRAPH_IMI,
	GRAPH_LRS,
	GRPAH_CandleVolume,
	GRAPH_IndicatorOptimizeSignal,
	GRAPH_SwingChart,
	GRAPH_KagiChart,
	GRAPH_RENKO_CHART,			// RenKoChart ADD.
	// ADD: 종찬(04/04/06) 메리츠증권 시장지표
	GRAPH_CustomerDeposition,	// 고객예탁금
	GRAPH_FutureOptionIncomplete,	// 선물옵션미결제 약정
	GRAPH_TradeYeSuKm,	// 거래예수금
	GRAPH_MarketPriceIndex,	// 시장지수
	GRAPH_FinancingBalance,	// 신용융자잔고
	GRAPH_IndustryPriceIndex,	// 업종지수
	GRAPH_ForeignerJiBunYul,	// 외국인지분율
	GRAPH_ForeignerPureBuy,	// 외국인순매수
	GRAPH_CommissionUncollected,	// 위탁자미수금
	// ADD: 종찬(04/08/02) 투자자동향에 Pakcet설정부분 추가
	GRAPH_Individual_Investor, // 개인투자자
	GRAPH_Organ_Investor, // 기관투자자
	GRAPH_Foreigner_Investor,  // 외국인투자자

	// 한화
	GRAPH_7BINARYWAVE,
	GRAPH_VA_OSCILLATOR,
	GRAPH_PRICE_CHANNEL,
	GRAPH_AROON,
	GRAPH_FORMULA,
	GRAPH_TDI,
	GRAPH_ELDER_RAY_BEAR,
	GRAPH_ELDER_RAY_BULL,
	GRAPH_STANDARD_ERROR,
	GRAPH_VOLUME_RATIO,
	GRAPH_VROC,
	GRAPH_UPDOWN_ADL,
	GRAPH_UPDOWN_ADR,
	GRAPH_PSYCO_LINE,
	GRAPH_DEMA,
	GRAPH_TEMA, 
	GRAPH_DISPARITY_SIMPLE,
	GRAPH_DISPARITY_COMPLEX,
	GRAPH_STARC_BAND,
	GRAPH_NDI,
	GRAPH_PERCENT_R,
	GRAPH_QSTIC,
	GRAPH_FORCE_INDEX,
	GRAPH_PERCENT_B_STOCHASTIC,
	GRAPH_LRS_STOCHASTIC,
	GRAPH_MACD_STOCHASTIC,
	GRAPH_MOMENTUM_STOCHASTIC,
	GRAPH_OBV_MOMENTUM,
	GRAPH_OBV_STOCHASTIC,
	GRAPH_ROC_STOCHASTIC,
	GRAPH_RSI_MACD,
	GRAPH_RSI_STOCHASTIC,
	GRAPH_SONAR_PSYCO_LINE,
	GRAPH_STOCHASTIC_RSI,
	GRAPH_STANDARD_ERROR_BAND,
	GRAPH_STANDARD_ERROR_CHANNEL,
	GRAPH_STANDARD_GAP_BAND,
	GRAPH_PARABOLIC_SAR,
	GRAPH_ADXR,
	GRAPH_MAO,
	GRAPH_PRICE_OSCILLATOR,
	GRAPH_CHARKINS_OSCILLATOR,
	GRAPH_VOLUME_OSCILLATOR,
	GRAPH_SROC,
	GRAPH_MASS_INDEX,
	GRAPH_RMI,
	GRAPH_DRF,								// Daily Raw Figure
	GRAPH_INTEREST,							// 금리
	GRAPH_NEWS_INTENSITY,					// 뉴스 강도
	GRAPH_NEWS_COUNT,						// 뉴스 건수
	GRAPH_AVOLS,							// 시가총액( Aggregate Value Of Listed Stock ) 
	GRAPH_DEBIT_BALANCE,					// 신용잔고( Debit Balance )
	GRAPH_DEPOSIT,							// 예탁금( Deposit )
	GRAPH_FOREIGNER_RESERVE_QUANTITY,		// 외국인 보유량
	GRAPH_FOREIGNER_RESERVE_RATIO,			// 외국인 보유율
	GRAPH_FOREIGNER_EXHAUSTED_RATIO,		// 외국인 소진율
	GRAPH_INVESTER_NET_BUYER_FUTURES,		// 투자자 순매수(선물)
	GRAPH_INVESTER_NET_BUYER_CALL_OPTION,	// 투자자 순매수(콜옵션)
	GRAPH_INVESTER_NET_BUYER_KOSDAQ,		// 투자자 순매수(KOSDAQ)
	GRAPH_INVESTER_NET_BUYER_KOSPI,			// 투자자 순매수(KOSPI)
	GRAPH_INVESTER_NET_BUYER_PUT_OPTION,	// 투자자 순매수(풋옵션)
	GRAPH_SELLBYHORIZONTALBAR,				//매수매도대기매물
	GRAPH_ENERGY,							// Energy
	GRAPH_CREDIT_BALANCE_RATE,					// 신용잔고율
	GRAPH_CREDIT_GIVING_RATE,					// 신용공여율
	GRAPH_CREDIT_BALANCE_QTY,					// 신용잔고수량
	GRAPH_CREDIT_BALANCE_VARIATION,				// 신용잔고증감

	GRAPH_COUNT
};

// Image
static const int ROOT = 0;
static const int BLOCK = 1;
static const int VERT_SCALE = 2;
static const int GRAPH = 3;
static const int CHILD = 4;

// 종찬(05/08/09)------- 한화지표용.
static const int HANHWA_INDICATOR_COUNT = 90;
static const _TCHAR* pchIndicatorList[HANHWA_INDICATOR_COUNT] =
{
	"그물망", "매물차트", "일목균형표", "표준오차밴드", "표준오차채널",
	"표준편차채널", "Bollinger Bands", "Box Chart", "DEMA", "Demark",
	"Envelope", "MAC", "Parabolic SAR", "Pivot", "Price Channel",
	"Starc Bands", "TEMA", "Zig Zag", "AD Line", "ADX",//20
	"ADXR", "Aroon", "CCI", "Formula", "LRS",
	"MACD", "MACD Oscillator", "MAO", "Momentum", "NDI",
	"Price Oscillator", "ROC", "Sonar", "SROC", "TDI",
	"TRIX", "VHF", "%R", "이격도(단순)", "이격도(지수)",//40
	"AB Ratio", "ATR", "Band Width", "Band %B", "Chaikin's Volatility",
	"DPO", "DRF", "Elder Ray Bear", "Elder Ray Bull", "Mass Index",
	"NCO", "Qstic", "RMI", "RSI", "Sigma",
	"Standard Deviation", "Standard Error", "Stochastic Fast", "Stochastic Slow", "Chaikin's Oscillator",//60
	"CMF", "EOM", "Force Index", "MFI", "NVI",
	"OBV", "PVI", "PVT", "VA Oscillator", "Volume Oscillator",
	"Volume Ratio", "VROC", "%B+Stochastic", "LRS+Stochastic", "MACD+Stochastic",
	"Momentum+Stochastic", "OBV+Momentum", "OBV+Stochastic", "ROC+Stochastic", "RSI+MACD",	// 80
	"RSI+Stochastic", "Sonar+심리도", "Stochastic+RSI", "심리도", "Binary Wave",
	"7Binary Wave", "ADL", "ADR", "가격 이동평균", "거래량 이동평균" //90
};

static const int SUBGRAPHCOUNT1 = 28;
static const _TCHAR* pchSubGraph1[SUBGRAPHCOUNT1] =
{
	"그물망", "매물차트", "Box Chart", "DEMA", "Parabolic SAR",
	"TEMA", "Zig Zag", "Formula", "MACD Oscillator", "MAO",
	"VHF", "이격도(단순)", "이격도(지수)", "Band Width", "Elder Ray Bear",
	"Elder Ray Bull", "RMI", "Sigma", "Standard Deviation", "Standard Error",	// 20
	"CMF", "Volume Ratio", "VROC", "Sonar+심리도", "심리도",
	"Binary Wave", "ADL", "ADR",
};

static const int SUBGRAPHCOUNT2 = 49;
static const _TCHAR* pchSubGraph2[SUBGRAPHCOUNT2] =
{
	"AD Line", "ADXR", "Aroon", "CCI", "LRS",
	"MACD", "Momentum", "NDI", "Price Oscillator", "ROC",	// 10
	"Sonar", "SROC", "TRIX", "%R", "AB Ratio",
	"ATR", "Band %B", "Chaikin's Volatility", "DPO", "Price Channel",	// 20
	"Mass Index", "NCO", "Qstic", "RSI", "Stochastic Fast",
	"Stochastic Slow", "Chaikin's Oscillator", "EOM", "Force Index", "MFI",	// 30
	"NVI", "OBV", "PVI", "PVT", "VA Oscillator",
	"Volume Oscillator", "%B+Stochastic", "LRS+Stochastic", "MACD+Stochastic", "Momentum+Stochastic",	// 40
	"OBV+Momentum", "OBV+Stochastic", "ROC+Stochastic", "RSI+MACD", "RSI+Stochastic",
	"Stochastic+RSI", "7Binary Wave", "DRF", "Demark"
};
	
static const int SUBGRAPHCOUNT3 = 9;
static const _TCHAR* pchSubGraph3[SUBGRAPHCOUNT3] =
{
	"표준오차밴드", "표준오차채널", "표준편차채널", "Bollinger Bands", "투자자순매수(Put)",
	"Envelope", "Starc Bands", "ADX", "TDI"
};

static const int SUBGRAPHCOUNT4 = 1;
static const _TCHAR* pchSubGraph4[SUBGRAPHCOUNT4] =
{
	"MAC"
};

static const int SUBGRAPHCOUNT5 = 2;
static const _TCHAR* pchSubGraph5[SUBGRAPHCOUNT5] =
{
	"일목균형표", "Pivot"
};

#endif

