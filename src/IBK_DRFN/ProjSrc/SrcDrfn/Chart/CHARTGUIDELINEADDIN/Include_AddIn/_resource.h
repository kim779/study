//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/1/22
// Comments		: Windows Command ID Define of AddIns
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

// (2006/1/22 - Seung-Won, Bae) ChartObjectAddIn's IDs
#define ID_COA_CONFIG				40001
#define ID_COA_DELETE				40002
#define ID_COA_SCALE_X_SHOW			40003
#define ID_COA_SCALE_X_HIDE			40004
#define ID_COA_CONFIG_SCALE			40005
// 20081007 JS.Kim
#define ID_COA_TIMEDIFF_0			41800
#define ID_COA_TIMEDIFF_END			41899

// (2006/1/26 - Seung-Won, Bae) ChartAnalysisAddIn's IDs
#define ID_CAA_CONFIG				40101
#define ID_CAA_DELETE				40102
#define ID_CAA_RELEASE_SERIESDRAW	40103
#define ID_CAA_DRAW_DRIFTLINE		40104
#define ID_CAA_DELETE_DRIFTLINE		40105
#define ID_CAA_CROSS_FOLLOW_CLOSE	40106	// It used in CrossDragToolAddIn and CrossOverToolAddIn Too.
#define ID_CAA_PROPERTY_CHANGE		40107	// (2006/10/17 - jwp) In ChartAnalysisAddIn - PropertyDlg Apply Message
#define ID_CAA_TOOL_COPY			40108	// (2006/11/30 - jwp) In ChartAnalysisAddIn - AnalysisTool Copy Message
#define ID_CAA_FIBO_CONFIG          40109

// (2006/2/2 - Seung-Won, Bae) ChartSymbolAddIn's IDs
#define ID_CSA_CONFIG				40201
#define ID_CSA_DELETE				40202
#define ID_CSA_SQUARE				40203
#define ID_CSA_CIRCLE				40204
#define ID_CSA_TRIANGLE				40205
#define ID_CSA_RELEASE_SERIESDRAW	40206

// (2006/3/28 - Seung-Won, Bae) SaveChartToImageAddIn's IDs
#define ID_SCI_SAVE_SCREEN			40301
#define ID_GLANCEBALANCE_CURSOR     40302		// 제공되지 않은 메뉴 제거 - ojtaso (20071025) 

// (2006/3/29 - Seung-Won, Bae) CNumericalInquiryToolAddInImp's IDs
#define ID_NIA_NIDLG_TOOL_CLOSE			40401
#define ID_NIA_NIDLG_OVER_EXECUTE		40402

// (2006/3/29 - Seung-Won, Bae) CCrosshairsToolAddInImp's IDs
#define ID_COA_CROSSHAIRS_TOOL_CLOSE		40501

// (2006/6/11 - Seung-Won, Bae) Add Indicator Command IDs
#define ID_AIC_START_ID						40600
#define ID_AIC_PRICE_CHART_CANDLE			40601	// 캔들차트
#define ID_AIC_PRICE_CHART_LINE				40602	// 선차트
#define ID_AIC_PRICE_CHART_FLOW				40603	// FLOW 차트
#define ID_AIC_PRICE_CHART_BAR_OHLC			40604	// 바차트(시고저종)
#define ID_AIC_PRICE_CHART_BAR_HLC			40605	// 바차트(고저종)
#define ID_AIC_PRICE_CHART_CANDLEVOLUME		40606	// 캔들볼륨차트
#define ID_AIC_PRICE_CHART_EQUIVOLUME		40607	// 이큐볼륨차트
#define ID_AIC_PRICE_MA						40608	// 가격 이동평균
#define	ID_AIC_VOLUME_CHART					40609	// 거래량 차트
#define	ID_AIC_VOLUME_MA					40610	// 거래량 이동평균
#define	ID_AIC_RAINBOW						40611	// 그물 차트
#define	ID_AIC_VOLUME_FOR_SALE				40612	// 대기매물
#define	ID_AIC_GLANCE_BALANCE				40613	// 일목균형표
#define	ID_AIC_BOLLINGER_BANDS				40614	// Bollinger Bands
#define	ID_AIC_BOX_CHART					40615	// Box Chart
#define	ID_AIC_DEMA							40616	// DEMA
#define	ID_AIC_DEMARK						40617	// Demark
#define	ID_AIC_ENVELOPE						40618	// Envelope
#define	ID_AIC_MAC							40619	// MAC
#define	ID_AIC_PARABOLIC_SAR				40620	// Parabolic SAR
#define	ID_AIC_PIVOT						40621	// Pivot
#define	ID_AIC_PRICE_CHANNEL				40622	// Price Channel
#define	ID_AIC_STARC_BANDS					40623	// Starc Bands
#define	ID_AIC_TEMA							40624	// TEMA
#define	ID_AIC_ZIG_ZAG						40625	// Zig Zag
#define	ID_AIC_DISPARITY_SMA				40626	// 이격도(단순)
#define	ID_AIC_DISPARITY_EMA				40627	// 이격도(지수)
#define	ID_AIC_AB_RATIO						40628	// AB Ratio
#define	ID_AIC_ATR							40629	// ATR
#define	ID_AIC_BAND_PERCENT_B				40630	// Band %B
#define	ID_AIC_BAND_WIDTH					40631	// Band Width
#define	ID_AIC_CHAIKINS_VOLATILITY			40632	// Chaikin's Volatility
#define	ID_AIC_DPO							40633	// DPO
#define	ID_AIC_DRF							40634	// DRF
#define	ID_AIC_ELDER_RAY_BEAR				40635	// Elder Ray Bear
#define	ID_AIC_ELDER_RAY_BULL				40636	// Elder Ray Bull
#define	ID_AIC_MASS_INDEX					40637	// Mass Index
#define	ID_AIC_NCO							40638	// NCO
#define	ID_AIC_QSTIC						40639	// QStic
#define	ID_AIC_RMI							40640	// RMI
#define	ID_AIC_RSI							40641	// RSI
#define	ID_AIC_SIGMA						40642	// Sigma
#define	ID_AIC_STANDARD_DEVIATION			40643	// Standard Deviation
#define	ID_AIC_STANDARD_ERROR				40644	// Standard Error
#define	ID_AIC_STOCHASTIC_FAST				40645	// Stochastic Fast
#define	ID_AIC_STOCHASTIC_FAST2				40646	// Stochastic Fast2
#define	ID_AIC_STOCHASTIC_FAST3				40647	// Stochastic Fast3
#define	ID_AIC_STOCHASTIC_SLOW				40648	// Stochastic Slow
#define	ID_AIC_STOCHASTIC_SLOW2				40649	// Stochastic Slow2
#define	ID_AIC_STOCHASTIC_SLOW3				40650	// Stochastic Slow3
#define	ID_AIC_PERCENT_R					40651	// %R
#define	ID_AIC_AD_LINE						40652	// AD Line
#define	ID_AIC_ADX							40653	// ADX
#define	ID_AIC_ADXR							40654	// ADXR
#define	ID_AIC_AROON						40655	// Aroon
#define	ID_AIC_CCI							40656	// CCI
#define	ID_AIC_FORMULA						40657	// Formula
#define	ID_AIC_LRS							40658	// LRS
#define	ID_AIC_MACD							40659	// MACD
#define	ID_AIC_MACD_OSCILLATOR				40660	// MACD Oscillator
#define	ID_AIC_MAO							40661	// MAO
#define	ID_AIC_MOMENTUM						40662	// Momentum
#define	ID_AIC_NDI							40663	// NDI
#define	ID_AIC_PRICE_OSCILLATOR				40664	// Price Oscillator
#define	ID_AIC_ROC							40665	// ROC
#define	ID_AIC_SONAR						40666	// Sonar
#define	ID_AIC_SROC							40667	// SROC
#define	ID_AIC_TRIX							40668	// TRIX
#define	ID_AIC_VHF							40669	// VHF
#define	ID_AIC_CMF							40670	// CMF
#define	ID_AIC_CHAIKINS_OSCILLATOR			40671	// Chaikin's Oscillator
#define	ID_AIC_EOM							40672	// EOM
#define	ID_AIC_FORCE_INDEX					40673	// Force Index
#define	ID_AIC_MFI							40674	// MFI
#define	ID_AIC_NVI							40675	// NVI
#define	ID_AIC_OBV							40676	// OBV
#define	ID_AIC_PVI							40677	// PVI
#define	ID_AIC_VA_OSCILLATOR				40678	// VA Oscillator
#define	ID_AIC_VOLUME_OSCILLATOR			40679	// Volume Oscillator
#define	ID_AIC_VOLUME_RATIO					40680	// Volume Ratio
#define	ID_AIC_VROC							40681	// VROC
#define	ID_AIC_PSYCHOLOGICAL				40682	// 심리도
#define	ID_AIC_7_BINARY_WAVE				40683	// 7 Binary Wave
#define	ID_AIC_BINARY_WAVE					40684	// Binary Wave
#define	ID_AIC_PVT							40685	// PVT
#define	ID_AIC_LRS_STOCHASTIC				40686	// LRS + Stochastic
#define	ID_AIC_MACD_STOCHASTIC				40687	// MACD + Stochastic
#define	ID_AIC_MOMENTUM_STOCHASTIC			40688	// Momentum + Stochastic
#define	ID_AIC_OBV_MOMENTUM					40689	// OBV + Momentum
#define	ID_AIC_OBV_STOCHASTIC				40690	// OBV + Stochastic
#define	ID_AIC_ROC_STOCHASTIC				40691	// ROC + Stochastic
#define	ID_AIC_RSI_MACD						40692	// RSI + MACD
#define	ID_AIC_RSI_STOCHASTIC				40693	// RSI + Stochastic
#define	ID_AIC_SONAR_PSYCHOLOGICAL			40694	// Sonar + 심리도
#define	ID_AIC_STOCHASTIC_RSI				40695	// Stochastic + RSI
#define	ID_AIC_PERCENT_B_STOCHASTIC			40696	// %B + Stochastic
#define	ID_AIC_THREE_LINE_BREAK				40697	// 삼선전환도
#define	ID_AIC_INVERSE_LINE					40698	// 역시계곡선
#define	ID_AIC_P_AND_F						40699	// P&&F
#define	ID_AIC_SWING						40700	// Swing Chart
#define	ID_AIC_KAGI							40701	// Kagi Chart
#define	ID_AIC_RENKO						40702	// Renko Chart
#define ID_AIC_DISPERSION					40703	// 분산차트
#define	ID_AIC_VOLUMEPRICE					40704	// 거래대금
#define	ID_AIC_LRL							40705	// LRL
#define	ID_AIC_TSF							40706	// TSF
#define	ID_AIC_DMI							40707	// DMI
#define	ID_AIC_VR							40708	// VR
#define	ID_AIC_DISPARITY					40709	// 이격도
#define	ID_AIC_DISPERSIONCHART				40710	// 분산형
#define ID_AIC_BWI							40711	// BWI
#define ID_AIC_HV							40712	// 역사적변동성
#define ID_AIC_ORGAN_PURE_BUY_ACCUMULATION	40713	// 기관순매수누적
#define ID_AIC_ORGAN_PURE_BUY				40714	// 기관순매수량
#define ID_AIC_OPEN_INTEREST				40715	// 미결제약정
#define ID_AIC_MARKET_VALUE					40716	// 시가총액
#define ID_AIC_FOREIGNERS_HOLDING			40717	// 외국인 보유량
#define ID_AIC_FOREIGNERS_HOLDING_PERCENT	40718	// 외국인 보유율
#define ID_AIC_FOREIGNERS_NET_PURCHASE		40719	// 외국인 순매수
#define ID_AIC_KOSPI_PER_INVESTOR			40720	// 투자자별거래소
#define ID_AIC_KOSDAQ_PER_INVESTOR			40721	// 투자자별코스닥
#define ID_AIC_K200_PER_INVESTOR			40722	// 투자자별K200
#define ID_AIC_FUTURE_PER_INVESTOR			40723	// 투자자별선물
#define ID_AIC_CALLOPTION_PER_INVESTOR		40724	// 투자자별콜옵션
#define ID_AIC_PUTOPTION_PER_INVESTOR		40725	// 투자자별풋옵션
#define ID_AIC_PROGRAMTRADE_PUREBUY_KOSPI	40726	// 프로그램순매수 거래소
#define ID_AIC_PROGRAMTRADE_PUREBUY_KOSDAQ	40727	// 프로그램순매수 코스닥
#define ID_AIC_END_ID						40999

// (2006/6/11 - Seung-Won, Bae) Data List Viewer AddIn's ID
#define ID_DLV_SHOW_DATA_LIST_VIEWER	41001
#define	ID_DLV_DATA_SIMULATOR			41002 //20081011 이문수 데이터시뮬레이션 >>

// (2006/7/4 - Seung-Won, Bae) Design Property Page AddIn's ID
#define ID_DPP_LAYOUT_AND_COLOR		41101
#define ID_DPP_PROPERTY_PAGE		41102
#define ID_DPP_MENU_RESTART			41103
#define ID_DPP_PROPERTY_FONT		41104
#define ID_DPP_DESIGN_APPLIED		41105
#define ID_DPP_LML_TABLE			41106
#define ID_DPP_GML_TABLE			41107

// (2006/8/15 - Seung-Won, Bae) Environment Setup AddIn's ID
#define ID_ESP_SHOW_ENVIRONMENT_DLG	41201
#define ID_ESP_SHOW_INDICATOR_DLG	41202

// 2006.10.23 김현호 삼성 Addin 사용 IDs
#define ID_KSA_NEWS_NEWSINFO			41301
#define ID_KSA_PERIODSUMMARY_DELETE		41302
#define ID_KSA_PERIODSUMMARY_CONFIG		41303
#define ID_KSA_COMMON_RELEASE			41304
#define ID_KSA_PERIODSUMMARY_EXECUTE	41305
#define ID_KSA_ORDERLINE_CONFIG			41306
#define ID_KSA_ORDERLINE_DELETE			41307
#define ID_KSA_ORDERLINE_DELETEFROMFILE	41308
#define ID_KSA_ORDERLINE_EXECUTE		41309
#define ID_KSA_ORDERLINE_RELEASE		41310

// ( 2006 / 11 / 21 - Sang-Woo, Cho ) AutoScroll Timer ID
#define ID_ASA_AUTOSCROLL_TIMER			41401

// (2007/1/6 - Seung-Won, Bae) Platform Menu IDs
#define ID_PLM_ADD_MENU			41500
#define ID_PLM_MENU1			41501
#define ID_PLM_MENU2			41502
#define ID_PLM_MENU3			41503
#define ID_PLM_MENU4			41504
#define ID_PLM_MENU5			41505
#define ID_PLM_MENU6			41506
#define ID_PLM_ADD_MENU_END		41699

// (2006/6/11 - Seung-Won, Bae) Do not delete last Carriage Return.
