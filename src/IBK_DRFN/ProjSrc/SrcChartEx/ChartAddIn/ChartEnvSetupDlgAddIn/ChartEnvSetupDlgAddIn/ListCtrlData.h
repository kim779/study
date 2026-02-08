#ifndef __LISTCTRLDATA_H__
#define __LISTCTRLDATA_H__

#include <atlconv.h>

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for IMetaTable

///////////////
//data
typedef enum EGraphType
{
	GRAPH_NONE_TYPE = -1,
	GRAPH_PRICE,				// 0
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
	GRAPH_DPO,					/// Detreded Price Oscillator			// 60
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
	GRAPH_WEIGHTCLOSE,							// 70
	GRAPH_ChaikinsVolatility,
	GRAPH_BANDB,
	GRAPH_BandWidth,
	GRAPH_BinaryWave,
	GRAPH_EOM,
	GRAPH_Demark,
	GRAPH_WilliamAD,
	GRAPH_SMI,
	GRAPH_SwingIndex,
	GRAPH_ASI,									// 80
	GRAPH_RSquared,
	GRAPH_StandardError,
	GRAPH_Energy,
	GRAPH_TSFOscillator,
	GRAPH_TSI,
	GRAPH_MAChannel,
	GRAPH_NCO,
	GRAPH_Sigma,
	GRAPH_MaOscillator,
	GRAPH_TSF,										// 90
//	GRAPH_IMI,
	GRAPH_LRS,
	GRAPH_CandleVolume,
	GRAPH_IndicatorOptimizeSignal,
	GRAPH_SwingChart,
	GRAPH_KagiChart,
	GRAPH_RENKO_CHART,			// RenKoChart ADD.
// (2006.11.09 안승환) 분산차트 추가
	GRAPH_DISPERSION_CHART,
// (2006.11.09 안승환) 분산차트 추가
	// ADD: 종찬(04/04/06) 메리츠증권 시장지표
	GRAPH_CustomerDeposition,	// 고객예탁금
	GRAPH_FutureOptionIncomplete,	// 선물옵션미결제 약정
	GRAPH_TradeYeSuKm,	// 거래예수금					// 100
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
	GRAPH_7BINARYWAVE,								// 110
	GRAPH_VA_OSCILLATOR,
	GRAPH_PRICE_CHANNEL,
	GRAPH_AROON,
	GRAPH_FORMULA,
	GRAPH_TDI,
	GRAPH_ELDER_RAY_BEAR,
	GRAPH_ELDER_RAY_BULL,
	GRAPH_STANDARD_ERROR,
	GRAPH_VOLUME_RATIO,
	GRAPH_VROC,										// 120
	GRAPH_UPDOWN_ADL,
	GRAPH_UPDOWN_ADR,
	GRAPH_PSYCO_LINE,
	GRAPH_DEMA,
	GRAPH_TEMA, 
	GRAPH_DISPARITY_SIMPLE,
	GRAPH_DISPARITY_COMPLEX,
	GRAPH_STARC_BAND,
	GRAPH_NDI,
	GRAPH_PERCENT_R,							// 130
	GRAPH_QSTIC,
	GRAPH_FORCE_INDEX,
	GRAPH_PERCENT_B_STOCHASTIC,
	GRAPH_LRS_STOCHASTIC,
	GRAPH_MACD_STOCHASTIC,
	GRAPH_MOMENTUM_STOCHASTIC,
	GRAPH_OBV_MOMENTUM,
	GRAPH_OBV_STOCHASTIC,
	GRAPH_ROC_STOCHASTIC,
	GRAPH_RSI_MACD,								// 140
	GRAPH_RSI_STOCHASTIC,
	GRAPH_SONAR_PSYCO_LINE,
	GRAPH_STOCHASTIC_RSI,
	GRAPH_STANDARD_ERROR_BAND,
	GRAPH_STANDARD_ERROR_CHANNEL,
	GRAPH_STANDARD_GAP_BAND,
	GRAPH_PARABOLIC_SAR,
	GRAPH_ADXR,
	GRAPH_MAO,
	GRAPH_PRICE_OSCILLATOR,						// 150
	GRAPH_CHARKINS_OSCILLATOR,
	GRAPH_VOLUME_OSCILLATOR,
	GRAPH_SROC,
	GRAPH_MASS_INDEX,
	GRAPH_RMI,
	GRAPH_DRF,
	GRAPH_INTEREST,							// 금리
	GRAPH_NEWS_INTENSITY,					// 뉴스 강도
	GRAPH_NEWS_COUNT,						// 뉴스 건수
	GRAPH_AVOLS,							// 시가총액( Aggregate Value Of Listed Stock )		// 160
	GRAPH_DEBIT_BALANCE,					// 신용잔고( Debit Balance )
	GRAPH_DEPOSIT,							// 예탁금( Deposit )
	GRAPH_FOREIGNER_RESERVE_QUANTITY,		// 외국인 보유량
	GRAPH_FOREIGNER_RESERVE_RATIO,			// 외국인 보유율
	GRAPH_FOREIGNER_EXHAUSTED_RATIO,		// 외국인 소진율
	GRAPH_INVESTER_NET_BUYER_FUTURES,		// 투자자 순매수(선물)
	GRAPH_INVESTER_NET_BUYER_CALL_OPTION,	// 투자자 순매수(콜옵션)
	GRAPH_INVESTER_NET_BUYER_KOSDAQ,		// 투자자 순매수(KOSDAQ)
	GRAPH_INVESTER_NET_BUYER_KOSPI,			// 투자자 순매수(KOSPI)
	GRAPH_INVESTER_NET_BUYER_PUT_OPTION,	// 투자자 순매수(풋옵션)			// 170
	GRAPH_SELLBYHORIZONTALBAR,				//매수매도대기매물
	GRAPH_ENERGY,							// Energy
	GRAPH_CREDIT_BALANCE_RATE,					// 신용잔고율
	GRAPH_CREDIT_GIVING_RATE,					// 신용공여율
	GRAPH_CREDIT_BALANCE_QTY,					// 신용잔고수량
	GRAPH_CREDIT_BALANCE_VARIATION,				// 신용잔고증감

	GRAPH_COUNT
};

static IMetaTable::E_METACODE g_szIndicatorNameList[ GRAPH_COUNT] =
{
	IMetaTable::C2_PRICE_CHART,				// 0
	IMetaTable::C2_VOLUME_CHART,  
	IMetaTable::C2_PRICE_MA,
	IMetaTable::C2_VOLUME_MA,
	IMetaTable::C2_OVERHANGING_SUPPLY, 	
	IMetaTable::C2_DISPARITY,     
	IMetaTable::C2_OBVIOUSLY_BALANCE_CHART,	
	IMetaTable::C2_PSYCHOLOGICAL_LINE,
    IMetaTable::C4_TEXT,	//신심리선
	IMetaTable::C2_AB_RATIO,   
	IMetaTable::C2_ATR,	   				// 10
	IMetaTable::C2_BOLLINGER_BANDS,
    IMetaTable::C2_BOX_CHART,
	IMetaTable::C2_CCI, 
	IMetaTable::C2_BROKEN_LINE_CHART,
	IMetaTable::C2_CMF,
	IMetaTable::C4_TEXT,	//CO    
	IMetaTable::C2_DMI,    
	IMetaTable::C2_ENVELOPE,
    IMetaTable::C4_TEXT,	//IMI
	IMetaTable::C4_TEXT,	//Fourier Transform   	// 20
	IMetaTable::C2_MAC,    
	IMetaTable::C2_MACD,    
	IMetaTable::C2_MACD_OSCILLATOR,   
	IMetaTable::C2_MFI,    
	IMetaTable::C4_TEXT,	//MI    
	IMetaTable::C2_MOMENTUM,
	IMetaTable::C2_NVI,    
	IMetaTable::C2_OBV,    
	IMetaTable::C4_TEXT,	//OCMA
	IMetaTable::C4_TEXT,	//OSCV 				// 30
	IMetaTable::C2_PARABOLIC_SAR,
	IMetaTable::C2_PIVOT,
	IMetaTable::C2_PVI, 
	IMetaTable::C4_TEXT,	//OSCP 
	IMetaTable::C2_PVT, 
	IMetaTable::C2_COMPARISON_CHART,
	IMetaTable::C2_ROC,    
	IMetaTable::C2_RSI, 	
	IMetaTable::C2_SONAR,
	IMetaTable::C2_STOCHASTIC_FAST, 	 			// 40
	IMetaTable::C2_STOCHASTIC_SLOW,
	IMetaTable::C4_TEXT,	//Stochastics Oscillator
	IMetaTable::C4_TEXT,	//TRIN
	IMetaTable::C2_VROC,	//VROC 
	IMetaTable::C2_VR, 	
	IMetaTable::C4_TEXT,	//William's %R 	
	IMetaTable::C2_TRIX, 	
	IMetaTable::C2_RAINBOW_CHART,	
	IMetaTable::C2_THREE_LINE_BREAK,
	IMetaTable::C2_INVERSE_TIME_LINE,			// 50
	IMetaTable::C2_P_AND_F,
	IMetaTable::C2_FUTURE_OPTION_P_AND_F,
	IMetaTable::C4_TEXT,	//거래내역
	IMetaTable::C3_LOCK,	//락
	IMetaTable::C4_TEXT,	//Big News
	IMetaTable::C3_MARKET_PROFILE,

	///대우의 지표에서 추가된 내용
	IMetaTable::C2_AD_LINE,
	IMetaTable::C2_ZIG_ZAG,
	IMetaTable::C4_TEXT,	//EMV
	IMetaTable::C2_DPO,					// 60
	IMetaTable::C4_TEXT,	//ABI
	IMetaTable::C4_TEXT,	//ADL
	IMetaTable::C4_TEXT,	//ADR
	IMetaTable::C4_TEXT,	//Breadth Thrust
	IMetaTable::C4_TEXT,	//McClellan Oscillator
	IMetaTable::C2_STANDARD_DEVIATION,
	IMetaTable::C4_TEXT,	//STIX
	IMetaTable::C2_VHF,

	//교보
	IMetaTable::C2_ADX,
	IMetaTable::C4_TEXT,	//Weighted Close					// 70
	IMetaTable::C2_CHAIKINS_VOLATILITY,
	IMetaTable::C2_BAND_PERCENT_B,
	IMetaTable::C2_BAND_WIDTH,
	IMetaTable::C2_BINARY_WAVE,
	IMetaTable::C2_EOM,
	IMetaTable::C2_DEMARK,
	IMetaTable::C4_TEXT,	//Will A/D
	IMetaTable::C4_TEXT,	//SMI
	IMetaTable::C4_TEXT,	//Swing Index
	IMetaTable::C4_TEXT,	//ASI						// 80
	IMetaTable::C4_TEXT,	//RSquared
	IMetaTable::C4_TEXT,
	IMetaTable::C4_TEXT,	//Energy
	IMetaTable::C4_TEXT,	//TSF Oscillator
	IMetaTable::C4_TEXT,	//
	IMetaTable::C4_TEXT,	//이동평균 채널
	IMetaTable::C2_NCO,
	IMetaTable::C2_SIGMA,
	IMetaTable::C4_TEXT,	//MA Oscillator
	IMetaTable::C2_TSF,									// 90
	IMetaTable::C2_LRS,
	IMetaTable::C5_CANDLE_VOLUME_CHART,
	IMetaTable::C4_TEXT,	//IndicatorOptimize Signal
	IMetaTable::C2_SWING_CHART,
	IMetaTable::C2_KAGI_CHART,
	IMetaTable::C2_RENKO_CHART,			// YJS 05/06/21 RenKoChart ADD.
// (2006.11.09 안승환) 분산차트 추가
	IMetaTable::C2_DISPERSION,
// (2006.11.09 안승환) 분산차트 추가
	// ADD: 종찬(04/04/06) 메리츠증권 시장지표 10개추가
	IMetaTable::C4_TEXT,	//고객예탁금				
	IMetaTable::C4_TEXT,	//선물,옵션 미결제 약정	
	IMetaTable::C4_TEXT,	//선물거래예수금		// 100
	IMetaTable::C4_TEXT,	//시장지수
	IMetaTable::C4_TEXT,	//신용융자잔고 
	IMetaTable::C4_TEXT,	//업종지수					
	IMetaTable::C4_TEXT,	//외국인지분율		
	IMetaTable::C4_TEXT,	//외국인순매수				
	IMetaTable::C4_TEXT,	//위탁자미수금
	// ADD: 종찬(04/04/06) 메리츠증권 시장지표 3개추가
	IMetaTable::C4_TEXT,	//개인투자자 
	IMetaTable::C4_TEXT,	//기관투자자
	IMetaTable::C4_TEXT,	//외국인투자자

	// 한화
	IMetaTable::C2_7_BINARY_WAVE,			// 110
	IMetaTable::C2_VA_OSCILLATOR,
	IMetaTable::C2_PRICE_CHANNEL,
	IMetaTable::C2_AROON, 
	IMetaTable::C2_FORMULA,
	IMetaTable::C4_TEXT,	//TDI 
	IMetaTable::C2_ELDER_RAY_BEAR,
	IMetaTable::C2_ELDER_RAY_BULL,
	IMetaTable::C2_STANDARD_ERROR,
	IMetaTable::C2_VOLUME_RATIO,
	IMetaTable::C2_VROC,					// 120
	IMetaTable::C4_TEXT,	//등락주선(ADL)
	IMetaTable::C4_TEXT,	//등락주비율(ADR) 
	IMetaTable::C2_PSYCHOLOGICAL_LINE,
	IMetaTable::C2_DEMA, 
	IMetaTable::C2_TEMA,
	IMetaTable::C2_DISPARITY_SMA,
	IMetaTable::C2_DISPARITY_EMA,
	IMetaTable::C2_STARC_BANDS,
	IMetaTable::C2_NDI,
	IMetaTable::C2_PERCENT_R,				// 130
	IMetaTable::C2_QSTIC,
	IMetaTable::C2_FORCE_INDEX,
	IMetaTable::C2_PERCENT_B_STOCHASTIC,
	IMetaTable::C2_LRS_STOCHASTIC,
	IMetaTable::C2_MACD_STOCHASTIC,
	IMetaTable::C2_MOMENTUM_STOCHASTIC,
	IMetaTable::C2_OBV_MOMENTUM,
	IMetaTable::C2_OBV_STOCHASTIC,
	IMetaTable::C2_ROC_STOCHASTIC,
	IMetaTable::C2_RSI_MACD,				// 140
	IMetaTable::C2_RSI_STOCHASTIC,
	IMetaTable::C2_SONAR_PSYCHOLOGY,
	IMetaTable::C2_STOCHASTIC_RSI,
	IMetaTable::C2_STANDARD_ERROR_BAND,
	IMetaTable::C4_TEXT,	//표준오차채널
	IMetaTable::C4_TEXT,	//표준편차채널
	IMetaTable::C2_PARABOLIC_SAR,
	IMetaTable::C2_ADXR,
	IMetaTable::C2_MAO,
	IMetaTable::C2_PRICE_OSCILLATOR,		// 150
	IMetaTable::C2_CHAIKINS_OSCILLATOR,
	IMetaTable::C2_VOLUME_OSCILLATOR,
	IMetaTable::C2_SROC,
	IMetaTable::C2_MASS_INDEX,
	IMetaTable::C2_RMI,
	IMetaTable::C2_DRF,
	IMetaTable::C4_TEXT,	//금리
	IMetaTable::C4_TEXT,	//뉴스 강도
	IMetaTable::C4_TEXT,	//뉴스 건수
	IMetaTable::C4_TEXT,	//시가총액				// 160
	IMetaTable::C4_TEXT,	//신용잔고
	IMetaTable::C4_TEXT,	//예탁금
	IMetaTable::C4_TEXT,	//외국인보유량
	IMetaTable::C4_TEXT,	//외국인보유율
	IMetaTable::C4_TEXT,	//외국인소진율
	IMetaTable::C4_TEXT,	//투자자순매수(선물)
	IMetaTable::C4_TEXT,	//투자자순매수(Call)
	IMetaTable::C4_TEXT,	//투자자순매수(KOSDAQ)
	IMetaTable::C4_TEXT,	//투자자순매수(KOSPI)
	IMetaTable::C4_TEXT,	//투자자순매수(Put)		// 170
	IMetaTable::C2_BUY_SELL_OVERHANGING_SUPPLY,	//매수매도대기매물
	IMetaTable::C4_TEXT,	//Energy
	IMetaTable::C4_TEXT,	// 신용잔고율
	IMetaTable::C4_TEXT,	// 신용공여율
	IMetaTable::C4_TEXT,	// 신용잔고수량
	IMetaTable::C4_TEXT	// 신용잔고증감
};

/*static const _TCHAR *g_szIndicatorNameList[ GRAPH_COUNT] = 
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
	_T("OCMA"), 				// 30
	_T("OSCV"),
	_T("Parabolic"), 
	_T("Pivot"), 
	_T("PVI"), 
	_T("OSCP"), 
	_T("PVT"), 
	_T("상대비교차트"), 
	_T("RoC"),    
	_T("RSI"), 	
	_T("SONAR"), 	 			// 40
	_T("Stochastics Fast"),
	_T("Stochastics Slow"),
	_T("Stochastics Oscillator"),
	_T("TRIN"),
	_T("VROC"), 
	_T("VR"), 	
	_T("William's %R"), 	
	_T("TRIX"), 	
	_T("그물차트"),	
	_T("삼선전환도"),			// 50
	_T("역시계곡선"), 
	_T("P&F"),
	_T("FuOpP&F"),
	_T("거래내역"),
	_T("락"),
	_T("Big News"),
	_T("MarketProfile"),

	///대우의 지표에서 추가된 내용
	_T("AD Line"),
	_T("Zig Zag"),
	_T("EMV"),					// 60
	_T("DPO"),
	_T("ABI"),
	_T("ADL"),
	_T("ADR"),
	_T("Breadth Thrust"),
	_T("McClellan Oscillator"),
	_T("Standard Deviation"),
	_T("STIX"),
	_T("VHF"),

	//교보
	_T("ADX"),					// 70
	_T("Weighted Close"),
	_T("Chaikin's Volatility"),
	_T("Band %B"),
	_T("Band Width"),
	_T("Binary Wave"),
	_T("EOM"),
	_T("Demark"),
	_T("Will A/D"),
	_T("SMI"),
	_T("Swing Index"),			// 80
	_T("ASI"),
	_T("RSquared"),
	_T("Standard Error"),
	_T("Energy"),
	_T("TSF Oscillator"),
	_T("TSI"),
	_T("이동평균 채널"),
	_T("NCO"),
	_T("SIGMA"),
	_T("MA Oscillator"),		// 90
	_T("TSF"),
	_T("LRS"),
	_T("CandleVolume"),
	_T("IndicatorOptimize Signal"),
	_T("SwingChart"),
	_T("KagiChart"),
	_T("RenkoChart"),			// YJS 05/06/21 RenKoChart ADD.
// (2006.11.09 안승환) 분산차트 추가
	_T("분산형"),
// (2006.11.09 안승환) 분산차트 추가
	// ADD: 종찬(04/04/06) 메리츠증권 시장지표 10개추가
	_T("고객예탁금"),				
	_T("선물,옵션 미결제 약정"),	
	_T("선물거래예수금"),		// 100
	_T("시장지수"),
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
	_T("7Binary Wave"),			// 110
	_T("VA Oscillator"),
	_T("Price Channel"), 
	_T("Aroon"), 
	_T("Formula"), 
	_T("TDI"), 
	_T("Elder Ray Bear"),
	_T("Elder Ray Bull"),
	_T("Standard Error"), 
	_T("Volume Ratio"), 
	_T("VROC"),					// 120
	_T("등락주선(ADL)"),
	_T("등락주비율(ADR)"), 
	_T("심리도"), 
	_T("DEMA"), 
	_T("TEMA"), 
	_T("이격도(단순)"),
	_T("이격도(지수)"),
	_T("Starc Bands"),
	_T("NDI"),
	_T("%R"),					// 130
	_T("Qstic"),
	_T("Force Index"),
	_T("%B+Stochastic"),
	_T("LRS+Stochastic"),
	_T("MACD+Stochastic"),
	_T("Momentum+Stochastic"),
	_T("OBV+Momentum"),
	_T("OBV+Stochastic"),
	_T("ROC+Stochastic"),
	_T("RSI+MACD"),				// 140
	_T("RSI+Stochastic"),
	_T("Sonar+심리도"),
	_T("Stochastic+RSI"),
	_T("표준오차밴드"),
	_T("표준오차채널"),
	_T("표준편차채널"),
	_T("Parabolic SAR"),
	_T("ADXR"),
	_T("MAO"),
	_T("Price Oscillator"),		// 150
	_T("Chaikin's Oscillator"),
	_T("Volume Oscillator"),
	_T("SROC"),
	_T("Mass Index"),
	_T("RMI"),
	_T("DRF"),
	_T("금리"),
	_T("뉴스 강도"),
	_T("뉴스 건수"),
	_T("시가총액"),				// 160
	_T("신용잔고"),
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
	_T("Energy")
};
*/
#endif

