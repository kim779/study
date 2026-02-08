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
#define ID_COA_DELETE_ALL			40006

// (2006/1/26 - Seung-Won, Bae) ChartAnalysisAddIn's IDs
#define ID_CAA_CONFIG				40101
#define ID_CAA_DELETE				40102
#define ID_CAA_RELEASE_SERIESDRAW	40103
#define ID_CAA_DRAW_DRIFTLINE		40104
#define ID_CAA_DELETE_DRIFTLINE		40105
#define ID_CAA_CROSS_FOLLOW_CLOSE	40106	// It used in CrossDragToolAddIn and CrossOverToolAddIn Too.
#define ID_CAA_PROPERTY_CHANGE		40107	// (2006/10/17 - jwp) In ChartAnalysisAddIn - PropertyDlg Apply Message
#define ID_CAA_TOOL_COPY			40108	// (2006/11/30 - jwp) In ChartAnalysisAddIn - AnalysisTool Copy Message

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
#define ID_AIC_START_ID				40600
#define	ID_AIC_PRICE_CHART			40601	// 가격 차트
#define ID_AIC_PRICE_MA				40602	// 가격 이동평균
#define	ID_AIC_VOLUME_CHART			40603	// 거래량 차트
#define	ID_AIC_VOLUME_MA			40604	// 거래량 이동평균
#define	ID_AIC_RAINBOW				40605	// 그물 차트
#define	ID_AIC_VOLUME_FOR_SALE		40606	// 대기매물
#define	ID_AIC_GLANCE_BALANCE		40607	// 일목균형표
#define	ID_AIC_BOLLINGER_BANDS		40608	// Bollinger Bands
#define	ID_AIC_BOX_CHART			40609	// Box Chart
#define	ID_AIC_DEMA					40610	// DEMA
#define	ID_AIC_DEMARK				40611	// Demark
#define	ID_AIC_ENVELOPE				40612	// Envelope
#define	ID_AIC_MAC					40613	// MAC
#define	ID_AIC_PARABOLIC_SAR		40614	// Parabolic SAR
#define	ID_AIC_PIVOT				40615	// Pivot
#define	ID_AIC_PRICE_CHANNEL		40616	// Price Channel
#define	ID_AIC_STARC_BANDS			40617	// Starc Bands
#define	ID_AIC_TEMA					40618	// TEMA
#define	ID_AIC_ZIG_ZAG				40619	// Zig Zag
#define	ID_AIC_DISPARITY_SMA		40620	// 이격도(단순)
#define	ID_AIC_DISPARITY_EMA		40621	// 이격도(지수)
#define	ID_AIC_AB_RATIO				40622	// AB Ratio
#define	ID_AIC_ATR					40623	// ATR
#define	ID_AIC_BAND_PERCENT_B		40624	// Band %B
#define	ID_AIC_BAND_WIDTH			40625	// Band Width
#define	ID_AIC_CHAIKINS_VOLATILITY	40626	// Chaikin's Volatility
#define	ID_AIC_DPO					40627	// DPO
#define	ID_AIC_DRF					40628	// DRF
#define	ID_AIC_ELDER_RAY_BEAR		40629	// Elder Ray Bear
#define	ID_AIC_ELDER_RAY_BULL		40630	// Elder Ray Bull
#define	ID_AIC_MASS_INDEX			40631	// Mass Index
#define	ID_AIC_NCO					40632	// NCO
#define	ID_AIC_QSTIC				40633	// QStic
#define	ID_AIC_RMI					40634	// RMI
#define	ID_AIC_RSI					40635	// RSI
#define	ID_AIC_SIGMA				40636	// Sigma
#define	ID_AIC_STANDARD_DEVIATION	40637	// Standard Deviation
#define	ID_AIC_STANDARD_ERROR		40638	// Standard Error
#define	ID_AIC_STOCHASTIC_FAST		40639	// Stochastic Fast
#define	ID_AIC_STOCHASTIC_FAST2		40640	// Stochastic Fast2
#define	ID_AIC_STOCHASTIC_FAST3		40641	// Stochastic Fast3
#define	ID_AIC_STOCHASTIC_SLOW		40642	// Stochastic Slow
#define	ID_AIC_STOCHASTIC_SLOW2		40643	// Stochastic Slow2
#define	ID_AIC_STOCHASTIC_SLOW3		40644	// Stochastic Slow3
#define	ID_AIC_PERCENT_R			40645	// %R
#define	ID_AIC_AD_LINE				40646	// AD Line
#define	ID_AIC_ADX					40647	// ADX
#define	ID_AIC_ADXR					40648	// ADXR
#define	ID_AIC_AROON				40649	// Aroon
#define	ID_AIC_CCI					40650	// CCI
#define	ID_AIC_FORMULA				40651	// Formula
#define	ID_AIC_LRS					40652	// LRS
#define	ID_AIC_MACD					40653	// MACD
#define	ID_AIC_MACD_OSCILLATOR		40654	// MACD Oscillator
#define	ID_AIC_MAO					40655	// MAO
#define	ID_AIC_MOMENTUM				40656	// Momentum
#define	ID_AIC_NDI					40657	// NDI
#define	ID_AIC_PRICE_OSCILLATOR		40658	// Price Oscillator
#define	ID_AIC_ROC					40659	// ROC
#define	ID_AIC_SONAR				40660	// Sonar
#define	ID_AIC_SROC					40661	// SROC
#define	ID_AIC_TRIX					40662	// TRIX
#define	ID_AIC_VHF					40663	// VHF
#define	ID_AIC_CMF					40664	// CMF
#define	ID_AIC_CHAIKINS_OSCILLATOR	40665	// Chaikin's Oscillator
#define	ID_AIC_EOM					40666	// EOM
#define	ID_AIC_FORCE_INDEX			40667	// Force Index
#define	ID_AIC_MFI					40668	// MFI
#define	ID_AIC_NVI					40669	// NVI
#define	ID_AIC_OBV					40670	// OBV
#define	ID_AIC_PVI					40671	// PVI
#define	ID_AIC_VA_OSCILLATOR		40672	// VA Oscillator
#define	ID_AIC_VOLUME_OSCILLATOR	40673	// Volume Oscillator
#define	ID_AIC_VOLUME_RATIO			40674	// Volume Ratio
#define	ID_AIC_VROC					40675	// VROC
#define	ID_AIC_PSYCHOLOGICAL		40676	// 심리도
#define	ID_AIC_7_BINARY_WAVE		40677	// 7 Binary Wave
#define	ID_AIC_BINARY_WAVE			40678	// Binary Wave
#define	ID_AIC_PVT					40679	// PVT
#define	ID_AIC_LRS_STOCHASTIC		40680	// LRS + Stochastic
#define	ID_AIC_MACD_STOCHASTIC		40681	// MACD + Stochastic
#define	ID_AIC_MOMENTUM_STOCHASTIC	40682	// Momentum + Stochastic
#define	ID_AIC_OBV_MOMENTUM			40683	// OBV + Momentum
#define	ID_AIC_OBV_STOCHASTIC		40684	// OBV + Stochastic
#define	ID_AIC_ROC_STOCHASTIC		40685	// ROC + Stochastic
#define	ID_AIC_RSI_MACD				40686	// RSI + MACD
#define	ID_AIC_RSI_STOCHASTIC		40687	// RSI + Stochastic
#define	ID_AIC_SONAR_PSYCHOLOGICAL	40688	// Sonar + 심리도
#define	ID_AIC_STOCHASTIC_RSI		40689	// Stochastic + RSI
#define	ID_AIC_PERCENT_B_STOCHASTIC	40690	// %B + Stochastic
#define	ID_AIC_THREE_LINE_BREAK		40691	// 삼선전환도
#define	ID_AIC_INVERSE_LINE			40692	// 역시계곡선
#define	ID_AIC_P_AND_F				40693	// P&&F
#define	ID_AIC_SWING				40694	// Swing Chart
#define	ID_AIC_KAGI					40695	// Kagi Chart
#define	ID_AIC_RENKO				40696	// Renko Chart
#define	ID_AIC_VOLUMEPRICE			40697	// 거래대금
#define	ID_AIC_LRL					40698	// LRL
#define	ID_AIC_TSF					40699	// TSF
#define	ID_AIC_DMI					40700	// DMI
#define	ID_AIC_VR					40701	// VR
#define	ID_AIC_DISPARITY			40702	// 이격도
#define	ID_AIC_DISPERSIONCHART		40703	// 분산형
#define ID_AIC_BWI					40704	// BWI
#define ID_AIC_HV					40705	// 역사적변동성
#define ID_AIC_END_ID				40999

// (2006/6/11 - Seung-Won, Bae) Data List Viewer AddIn's ID
#define ID_DLV_SHOW_DATA_LIST_VIEWER	41001

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

// (2007/3/5 - Seung-Won, Bae) Direct Real Test Timer ID
#define ID_TEST_REAL_TIMER_ID	41701


// (2006/6/11 - Seung-Won, Bae) Do not delete last Carriage Return.
