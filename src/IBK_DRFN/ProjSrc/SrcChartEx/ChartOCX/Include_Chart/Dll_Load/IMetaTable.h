// IMetaTable.h: interface for the IMetaTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMETATABLE_H__B983D82E_B4D1_4263_84E4_548B78FA6538__INCLUDED_)
#define AFX_IMETATABLE_H__B983D82E_B4D1_4263_84E4_548B78FA6538__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Include/PDll.h"
#include "../../Include_Chart/BlockBaseEnum.h"	// for CChartInfo

// Meta Text
#define _MTEXT( eMetaCode)					g_iMetaTable.m_szMetaTables[ IMetaTable::eMetaCode]
#define _MTEXTN( nMetaCodeIndex)			g_iMetaTable.m_szMetaTables[ nMetaCodeIndex]
#define _MHWND( hWnd)						HWND m_hOcxWnd = hWnd	// Do not use this macro, if you possible. Please use member vaiable.

// Language Text
#define _LTEXT0( eMetaCode)					_LTEXT( PACKET_NAME,			eMetaCode)
#define _LTEXT1( eMetaCode)					_LTEXT( GRAPH_TITLE,			eMetaCode)
#define _LTEXT2( eMetaCode)					_LTEXT( INDICATOR_NAME,			eMetaCode)
#define _LTEXT3( eMetaCode)					_LTEXT( SUB_GRAPH_NAME,			eMetaCode)
#define _LTEXT4( eMetaCode)					_LTEXT( TOOL_ITEM_NAME,			eMetaCode)
#define _LTEXT5( eMetaCode)					_LTEXT( GRAPH_TYPE_STYLE,		eMetaCode)
#define _LTEXT6( eMetaCode)					_LTEXT( SCALE_TYPE_AND_ETC,		eMetaCode)
#define _LTEXT7( eMetaCode)					_LTEXT( INDICATOR_GROUP_NAME,	eMetaCode)
#define _LTEXT( eCodePage, eMetaCode)		_MTOL( eCodePage,				_MTEXT( eMetaCode))
#define _LTEXTN( eCodePage, nMetaCodeIndex)	_MTOL( eCodePage,				_MTEXTN( nMetaCodeIndex))

// Get Language Text from Meta Text
#define _MTOL0( szMetaText)					_MTOL( PACKET_NAME,				szMetaText)
#define _MTOL1( szMetaText)					_MTOL( GRAPH_TITLE,				szMetaText)
#define _MTOL2( szMetaText)					_MTOL( INDICATOR_NAME,			szMetaText)
#define _MTOL3( szMetaText)					_MTOL( SUB_GRAPH_NAME,			szMetaText)
#define _MTOL4( szMetaText)					_MTOL( TOOL_ITEM_NAME,			szMetaText)
#define _MTOL5( szMetaText)					_MTOL( GRAPH_TYPE_STYLE,		szMetaText)
#define _MTOL6( szMetaText)					_MTOL( SCALE_TYPE_AND_ETC,		szMetaText)
#define _MTOL7( szMetaText)					_MTOL( INDICATOR_GROUP_NAME,	szMetaText)
#define _MTOL( eCodePage, szMetaText)		g_iMetaTable.GetLanguageText( m_hOcxWnd, IMetaTable::eCodePage, szMetaText, FALSE)
#define _MTOLD( eCodePage, eLanguageID, szMetaText)							\
				g_iMetaTable.GetLanguageTextDirectly( IMetaTable::eCodePage, IMetaTable::eLanguageID, szMetaText)
#define _MTOLDN( nCodePage, nLanguageID, szMetaText)						\
				g_iMetaTable.GetLanguageTextDirectly( ( IMetaTable::E_CODEPAGE)nCodePage, ( IMetaTable::E_LANGUAGEID)nLanguageID, szMetaText)
#define _MTOLS( szMetaTexts, strSeperators, nCodePage1)						\
				g_iMetaTable.GetMultipleLanguageText( m_hOcxWnd, szMetaTexts, strSeperators, nCodePage1, -1)
#define _MTOLS2( szMetaTexts, strSeperators, nCodePage1, nCodePage2)		\
				g_iMetaTable.GetMultipleLanguageText( m_hOcxWnd, szMetaTexts, strSeperators, nCodePage1, nCodePage2)
// return NULL
#define _MTOL0_N( szMetaText)				_MTOL_N( PACKET_NAME,			szMetaText)
#define _MTOL1_N( szMetaText)				_MTOL_N( GRAPH_TITLE,			szMetaText)
#define _MTOL2_N( szMetaText)				_MTOL_N( INDICATOR_NAME,		szMetaText)
#define _MTOL3_N( szMetaText)				_MTOL_N( SUB_GRAPH_NAME,		szMetaText)
#define _MTOL6_N( szMetaText)				_MTOL_N( SCALE_TYPE_AND_ETC,	szMetaText)
#define _MTOL_N( eCodePage, szMetaText)		g_iMetaTable.GetLanguageText( m_hOcxWnd, IMetaTable::eCodePage, szMetaText, TRUE)

// Get Meta Text from Meta Text
#define _LTOM0( szLanguageText)				_LTOM( PACKET_NAME,				szLanguageText)
#define _LTOM1( szLanguageText)				_LTOM( GRAPH_TITLE,				szLanguageText)
#define _LTOM2( szLanguageText)				_LTOM( INDICATOR_NAME,			szLanguageText)
#define _LTOM3( szLanguageText)				_LTOM( SUB_GRAPH_NAME,			szLanguageText)
#define _LTOM4( szLanguageText)				_LTOM( TOOL_ITEM_NAME,			szLanguageText)
#define _LTOM5( szLanguageText)				_LTOM( GRAPH_TYPE_STYLE,		szLanguageText)
#define _LTOM6( szLanguageText)				_LTOM( SCALE_TYPE_AND_ETC,		szLanguageText)
#define _LTOM7( szLanguageText)				_LTOM( INDICATOR_GROUP_NAME,	szLanguageText)
#define _LTOM( eCodePage, szLanguageText)	g_iMetaTable.GetMetaText( m_hOcxWnd, IMetaTable::eCodePage, szLanguageText, FALSE)
#define _LTOMD( eCodePage, eLanguageID, szLanguageText)						\
				g_iMetaTable.GetMetaTextDirectly( IMetaTable::eCodePage, IMetaTable::eLanguageID, szLanguageText)
#define _LTOMDN( nCodePage, nLanguageID, szLanguageText)					\
				g_iMetaTable.GetMetaTextDirectly( ( IMetaTable::E_CODEPAGE)nCodePage, ( IMetaTable::E_LANGUAGEID)nLanguageID, szLanguageText)
#define _LTOMS( szLanguageTexts, strSeperators, nCodePage1)					\
				g_iMetaTable.GetMultipleMetaText( m_hOcxWnd, szLanguageTexts, strSeperators, nCodePage1, -1)
#define _LTOMS2( szLanguageTexts, strSeperators, nCodePage1, nCodePage2)	\
				g_iMetaTable.GetMultipleMetaText( m_hOcxWnd, szLanguageTexts, strSeperators, nCodePage1, nCodePage2)
// return NULL
#define _LTOM2_N( szLanguageText)			_LTOM_N( INDICATOR_NAME,		szLanguageText)
#define _LTOM6_N( szLanguageText)			_LTOM_N( SCALE_TYPE_AND_ETC,	szLanguageText)
#define _LTOM_N( eCodePage, szLanguageText)	g_iMetaTable.GetMetaText( m_hOcxWnd, IMetaTable::eCodePage, szLanguageText, TRUE)

// Get Meta Text from Other Meta Text
#define _MTOM0( szOtherMetaText)			_MTOMH( PACKET_NAME,			szOtherMetaText)
#define _MTOM1( szOtherMetaText)			_MTOMH( GRAPH_TITLE,			szOtherMetaText)
#define _MTOM2( szOtherMetaText)			_MTOM( INDICATOR_NAME,			szOtherMetaText)
#define _MTOM3( szOtherMetaText)			_MTOM( SUB_GRAPH_NAME,			szOtherMetaText)
#define _MTOM4( szOtherMetaText)			_MTOM( TOOL_ITEM_NAME,			szOtherMetaText)
#define _MTOM5( szOtherMetaText)			_MTOM( GRAPH_TYPE_STYLE,		szOtherMetaText)
#define _MTOM6( szOtherMetaText)			_MTOM( SCALE_TYPE_AND_ETC,		szOtherMetaText)
#define _MTOM7( szOtherMetaText)			_MTOM( INDICATOR_GROUP_NAME,	szOtherMetaText)
#define _MTOM( eCodePage, szOtherMetaText)		g_iMetaTable.GetMetaTextFromOther( NULL, IMetaTable::eCodePage, szOtherMetaText, FALSE)
#define _MTOMH( eCodePage, szOtherMetaText)	g_iMetaTable.GetMetaTextFromOther( m_hOcxWnd, IMetaTable::eCodePage, szOtherMetaText, FALSE)
#define _MTOMS( szOtherMetaTexts, strSeperators, nCodePage1)				\
			g_iMetaTable.GetMultipleMetaTextFromOther( m_hOcxWnd, szOtherMetaTexts, strSeperators, nCodePage1, -1)
// return NULL
#define _MTOM6_N( szOtherMetaText)			_MTOM_N( SCALE_TYPE_AND_ETC,	szOtherMetaText)
#define _MTOM_N( eCodePage, szOtherMetaText)	g_iMetaTable.GetMetaTextFromOther( NULL, IMetaTable::eCodePage, szOtherMetaText, TRUE)

// Get Meta Text from Other Meta Text or Language Text for Script
#define _STOM0( szOtherMOrLText)			_STOM( PACKET_NAME,				szOtherMOrLText)
#define _STOM1( szOtherMOrLText)			_STOM( GRAPH_TITLE,				szOtherMOrLText)
#define _STOM2( szOtherMOrLText)			_STOM( INDICATOR_NAME,			szOtherMOrLText)
#define _STOM3( szOtherMOrLText)			_STOM( SUB_GRAPH_NAME,			szOtherMOrLText)
#define _STOM4( szOtherMOrLText)			_STOM( TOOL_ITEM_NAME,			szOtherMOrLText)
#define _STOM5( szOtherMOrLText)			_STOM( GRAPH_TYPE_STYLE,		szOtherMOrLText)
#define _STOM6( szOtherMOrLText)			_STOM( SCALE_TYPE_AND_ETC,		szOtherMOrLText)
#define _STOM7( szOtherMOrLText)			_STOM( INDICATOR_GROUP_NAME,	szOtherMOrLText)
#define _STOM( eCodePage, szOtherMOrLText)	g_iMetaTable.GetMetaTextForScript( m_hOcxWnd, IMetaTable::eCodePage, szOtherMOrLText, FALSE)
#define _STOMS( szOtherMOrLTexts, strSeperators, nCodePage)				\
			g_iMetaTable.GetMultipleMetaTextForScript( m_hOcxWnd, szOtherMOrLTexts, strSeperators, nCodePage, -1)
// return NULL
#define _STOM6_N( szOtherMOrLText)			_STOM( SCALE_TYPE_AND_ETC,		szOtherMOrLText)
#define _STOM_N( eCodePage, szOtherMOrLText)	g_iMetaTable.GetMetaTextForScript( m_hOcxWnd, IMetaTable::eCodePage, szOtherMOrLText, TRUE)

// Get Meta Text from Meta Text with Creation.
#define _CRTM0( szLanguageText)				_CRTMH( PACKET_NAME,		szLanguageText)
#define _CRTM1( szLanguageText)				_CRTMH( GRAPH_TITLE,		szLanguageText)
#define _CRTMH( eCodePage, szLanguageText)	g_iMetaTable.CreateLocalMetaText( m_hOcxWnd, IMetaTable::eCodePage, szLanguageText)
#define _SETM( szMetaText, szLanguageText)	g_iMetaTable.SetLocalMetaText( m_hOcxWnd, szMetaText, szLanguageText)

// Resource DLL Operation
#define ML_LANGUAGE_RES_					g_iMetaTable.LoadResourceDLL( m_hOcxWnd, AfxGetStaticModuleState()->m_hCurrentInstanceHandle)
#define ML_DEFAULT_RES_						AfxGetStaticModuleState()->m_hCurrentInstanceHandle
#define ML_SET_LANGUAGE_RES()				CMultiLanguageResourceSetter __mlrs__( ML_LANGUAGE_RES_)
#define ML_SET_DEFAULT_RES()				CMultiLanguageResourceSetter __mlrs__( ML_DEFAULT_RES_)

//////////////////////////////////////////////////////////////////////
// IMetaTable
//////////////////////////////////////////////////////////////////////
class IMetaTable : public PDLL
{
public:
	IMetaTable();
	virtual ~IMetaTable();

public:
	enum E_LANGUAGEID
	{
		ENGLISH = 0, 
		KOREAN,

		LANGUAGE_ID_END
	};
	enum E_CODEPAGE
	{
		PACKET_NAME,					// 0
		GRAPH_TITLE,					// 1
		INDICATOR_NAME,					// 2
		SUB_GRAPH_NAME,					// 3
		TOOL_ITEM_NAME,					// 4
		GRAPH_TYPE_STYLE,				// 5
		SCALE_TYPE_AND_ETC,				// 6
		INDICATOR_GROUP_NAME,			// 7
		SYSTEM_TRADING,					// 8

		CODE_PAGE_END
	};
	enum E_METACODE
	{
		C0_DATE_TIME						= 0,	// 0
		C0_SECOND_TIME,								// 1
		C0_OPEN,									// 2
		C0_HIGH,									// 3
		C0_LOW,										// 4
		C0_CLOSE,									// 5
		C0_TEXT,									// 6
		C0_VOLUME,									// 7
		C0_ACCUMULATED_VOLUME,						// 8
		C0_LOCK,									// 9
		C0_ACCOUNT_RATE,							// 10
		C0_EXCHANGE_RATE,							// 11
		C0_TOTAL_ITEM_COUNT,						// 12
		C0_UP_ITEM_COUNT,							// 13
		C0_DOWN_ITEM_COUNT,							// 14
		C0_ADL,										// 15
		C0_MOBV,									// 16
		C0_UP_ITEM_VOLUME,							// 17
		C0_DOWN_ITEM_VOLUME,						// 18
		C0_GUIDE_ITEM_VOLUME,						// 19
		C0_W52_NEW_HIGH_ITEM_COUNT,					// 20
		C0_W52_NEW_LOW_ITEM_COUNT,					// 21
		C0_MARKET_STOCK_COUNT,						// 22
		C0_DEALED_STOCK_COUNT,						// 23
		C0_INVERSETIMELINE_VOLUME,					// 24
		C0_INVERSETIMELINE_PRICE,					// 25
		C0_SWING_CHART_PRICE,						// 26
		C0_SWING_CHART_DATE_TIME,					// 27
		C0_KAGI_DATE_TIME,							// 28
		C0_KAGI_PRICE,								// 29
		C0_PF_HIGH,									// 30
		C0_PF_LOW,									// 31
		C0_PF_DATE_TIME,							// 32
		C0_RENKO_CHART_OPEN,						// 33
		C0_RENKO_CHART_CLOSE,						// 34
		C0_RENKO_CHART_DATETIME,					// 35
		C0_THREE_LINE_BREAK_OPEN,					// 36
		C0_THREE_LINE_BREAK_CLOSE,					// 37
		C0_THREE_LINE_BREAK_DATE_TIME,				// 38
		C0_COMBINE_HIGH_LOW,						// 39
		C0_COMBINE_HIGH_LOW_CLOSE,					// 40
		C0_NEW_DATE_PRICE,							// 41
		C0_BUY_SELL_OVERHANGING_SUPPLY_PRICE_BAND,	// 42
		C0_BUY_OVERHANGING_SUPPLY,					// 43
		C0_SELL_OVERHANGING_SUPPLY,					// 44
		C0_PROGRAM_TRADE,							// 45
		C0_PER_INVESTOR,							// 46
		C0_DATA_TIME,								// 47
		C0_FOREIGNHOLDING_DATETIME,					// 48
		C0_FOREIGNHOLDING,							// 49
		C0_FOREIGNPUREBUY_DATETIME,					// 50
		C0_FOREIGNPUREBUY,							// 51
		C0_TOTALAMOUNT_DATETIME,					// 52
		C0_TOTALAMOUNT,								// 53
		C0_SUSPENSECONTRACT_DATETIME,				// 54
		C0_SUSPENSECONTRACT,						// 55
		C0_FOREIGNHOLDINGRATE_DATETIME,				// 56
		C0_FOREIGNHOLDINGRATE,						// 57
		C0_ORGANPUREBUY_DATETIME,					// 58
		C0_ORGANPUREBUY,							// 59
		C0_KOSPIPUREBUY_DATETIME,					// 60
		C0_KOSPIPUREBUY_PERSON,						// 61
		C0_KOSPIPUREBUY_ORGAN,						// 62
		C0_KOSPIPUREBUY_FOREIGN,					// 63
		C0_KOSDAQPUREBUY_DATETIME,					// 64
		C0_KOSDAQPUREBUY_PERSON,					// 65
		C0_KOSDAQPUREBUY_ORGAN,						// 66
		C0_KOSDAQPUREBUY_FOREIGN,					// 67
		C0_K200PUREBUY_DATETIME,					// 68
		C0_K200PUREBUY_PERSON,						// 69
		C0_K200PUREBUY_ORGAN,						// 70
		C0_K200PUREBUY_FOREIGN,						// 71
		C0_FUTUREPUREBUY_DATETIME,					// 72
		C0_FUTUREPUREBUY_PERSON,					// 73
		C0_FUTUREPUREBUY_ORGAN,						// 74
		C0_FUTUREPUREBUY_FOREIGN,					// 75
		C0_CALLOPTIONPUREBUY_DATETIME,				// 76
		C0_CALLOPTIONPUREBUY_PERSON,				// 77
		C0_CALLOPTIONPUREBUY_ORGAN,					// 78
		C0_CALLOPTIONPUREBUY_FOREIGN,				// 79
		C0_PUTOPTIONPUREBUY_DATETIME,				// 80
		C0_PUTOPTIONPUREBUY_PERSON,					// 81
		C0_PUTOPTIONPUREBUY_ORGAN,					// 82
		C0_PUTOPTIONPUREBUY_FOREIGN,				// 83
		C0_ORGANPUREBUYACCUMULATION_DATETIME,		// 84
		C0_ORGANPUREBUYACCUMULATION,				// 85
		C0_KOSPIPUREBUY,							// 86
		C0_KOSDAQPUREBUY,							// 87
		C0_K200PUREBUY,								// 88
		C0_FUTUREPUREBUY,							// 89
		C0_CALLOPTIONPUREBUY,						// 90
		C0_PUTOPTIONPUREBUY,						// 91
		C0_RAINBOW_PACKET,							// 92
		C0_BUY_SELL_OVERHANGING_SUPPLY,				// 93
		C0_VOLUME_AMOUNT,							// 94
		C0_PROGRAMTRADE_PUREBUY_KOSPI,				// 95
		C0_PROGRAMTRADE_PUREBUY_KOSDAQ,				// 96

		C2_PRICE_MA							= 200,	// 200
		C2_VOLUME_MA,								// 201
		C2_PRICE_CHART,								// 202
		C2_VOLUME_CHART,							// 203
		C2_RAINBOW_CHART,							// 204
		C2_OVERHANGING_SUPPLY,						// 205
		C2_OBVIOUSLY_BALANCE_CHART,					// 206
		C2_BOLLINGER_BANDS,							// 207
		C2_BOX_CHART,								// 208
		C2_DEMA,									// 209
		C2_DEMARK,									// 210
		C2_ENVELOPE,								// 211
		C2_MAC,										// 212
		C2_PARABOLIC_SAR,							// 213
		C2_PIVOT,									// 214
		C2_PRICE_CHANNEL,							// 215
		C2_STARC_BANDS,								// 216
		C2_TEMA,									// 217
		C2_ZIG_ZAG,									// 218
		C2_DISPARITY_SMA,							// 219
		C2_DISPARITY_EMA,							// 220
		C2_AB_RATIO,								// 221
		C2_ATR,										// 222
		C2_BAND_PERCENT_B,							// 223
		C2_BAND_WIDTH,								// 224
		C2_CHAIKINS_VOLATILITY,						// 225
		C2_DPO,										// 226
		C2_DRF,										// 227
		C2_ELDER_RAY_BEAR,							// 228
		C2_ELDER_RAY_BULL,							// 229
		C2_MASS_INDEX,								// 230
		C2_NCO,										// 231
		C2_QSTIC,									// 232
		C2_RMI,										// 233
		C2_RSI,										// 234
		C2_SIGMA,									// 235
		C2_STANDARD_DEVIATION,						// 236
		C2_STANDARD_ERROR,							// 237
		C2_STOCHASTIC_FAST,							// 238
		C2_STOCHASTIC_FAST2,						// 239
		C2_STOCHASTIC_FAST3,						// 240
		C2_STOCHASTIC_SLOW,							// 241
		C2_STOCHASTIC_SLOW2,						// 242
		C2_STOCHASTIC_SLOW3,						// 243
		C2_PERCENT_R,								// 244
		C2_AD_LINE,									// 245
		C2_ADX,										// 246
		C2_ADXR,									// 247
		C2_AROON,									// 248
		C2_CCI,										// 249
		C2_FORMULA,									// 250
		C2_LRS,										// 251
		C2_MACD,									// 252
		C2_MACD_OSCILLATOR,							// 253
		C2_MAO,										// 254
		C2_MOMENTUM,								// 255
		C2_NDI,										// 256
		C2_PRICE_OSCILLATOR,						// 257
		C2_ROC,										// 258
		C2_SONAR,									// 259
		C2_SROC,									// 260
		C2_TRIX,									// 261
		C2_VHF,										// 262
		C2_CMF,										// 263
		C2_CHAIKINS_OSCILLATOR,						// 264
		C2_EOM,										// 265
		C2_FORCE_INDEX,								// 266
		C2_MFI,										// 267
		C2_NVI,										// 268
		C2_OBV,										// 269
		C2_PVI,										// 270
		C2_VA_OSCILLATOR,							// 271
		C2_VOLUME_OSCILLATOR,						// 272
		C2_VOLUME_RATIO,							// 273
		C2_VROC,									// 274
		C2_PSYCHOLOGICAL_LINE,						// 275
		C2_7_BINARY_WAVE,							// 276
		C2_BINARY_WAVE,								// 277
		C2_PVT,										// 278
		C2_LRS_STOCHASTIC,							// 279
		C2_MACD_STOCHASTIC,							// 280
		C2_MOMENTUM_STOCHASTIC,						// 281
		C2_OBV_MOMENTUM,							// 282
		C2_OBV_STOCHASTIC,							// 283
		C2_ROC_STOCHASTIC,							// 284
		C2_RSI_MACD,								// 285
		C2_RSI_STOCHASTIC,							// 286
		C2_SONAR_PSYCHOLOGY,						// 287
		C2_STOCHASTIC_RSI,							// 288
		C2_PERCENT_B_STOCHASTIC,					// 289
		C2_THREE_LINE_BREAK,						// 290
		C2_INVERSE_TIME_LINE,						// 291
		C2_P_AND_F,									// 292
		C2_SWING_CHART,								// 293
		C2_KAGI_CHART,								// 294
		C2_RENKO_CHART,								// 295
		C2_VOLUME_AMOUNT,							// 296
		C2_TSF,										// 297
		C2_DMI,										// 298
		C2_DISPARITY,								// 299
		C2_VR								= 800,	// 800
		C2_BUY_SELL_OVERHANGING_SUPPLY,				// 801
		C2_COMPARISON_CHART,						// 802
		C2_FUTURE_OPTION_P_AND_F,					// 803
		C2_DISPERSION,								// 804
		C2_MINI_INDEX_CHART,						// 805
		C2_BROKEN_LINE_CHART,						// 806
		C2_STANDARD_ERROR_BAND,						// 807
		C2_BWI,										// 808
		C2_HISTORICAL_VOLATILITY,					// 809
		C2_TDI,										// 810
		C2_LRL,										// 811
		C2_NFI,										// 812
		C2_INDUSTRY_INDEX,							// 813
		C2_MARKET_INDEX,							// 814
		C2_DEAL_LIST,								// 815
		C2_BIG_NEWS,								// 816
		C2_FOURIER_TRANSFORM,						// 817
		C2_INVESTMENT_PSYCHOLOGICAL_LINE,			// 818
		C2_ADR,										// 819
		C2_BREADTH_THRUST,							// 820
		C2_MCCLELLAN_OSCILLATOR,					// 821
		C2_STIX,									// 822
		C2_RATIOCHART,								// 823
		C2_DOLLARCHART,								// 824
		C2_LOCK,									// 825
		C2_TOTAL_AMOUNT,							// 826
		C2_FOREIGN_HOLDING,							// 827
		C2_FOREIGN_HOLDING_RATE,					// 828
		C2_FOREIGN_EXHAUSTION_RATE,					// 829
		C2_FOREIGN_PURE_BUY,						// 830
		C2_ORGAN_PURE_BUY_ACCUMULATION,				// 831
		C2_ORGAN_PURE_BUY,							// 832
		C2_SUSPENSE_CONTRACT,						// 833
		C2_KOSPI_PER_INVESTOR,						// 834
		C2_KOSDAQ_PER_INVESTOR,						// 835
		C2_K200_PER_INVESTOR,						// 836
		C2_FUTURE_PER_INVESTOR,						// 837
		C2_CALLOPTION_PER_INVESTOR,					// 838
		C2_PUTOPTION_PER_INVESTOR,					// 839
		C2_PROGRAMTRADE_PUREBUY_KOSPI,				// 840
		C2_PROGRAMTRADE_PUREBUY_KOSDAQ,				// 841
		C2_SELLCLOSE,								// 842
		C2_BUYCLOSE,								// 843
//KHD : IBK 추가 
		C2_PERSON_PUREBUY,							// 844
		C2_PERSON_PUREBUYACCUM,						// 845
		C2_FOREIGN_PUREBUYACCM,						// 846
		C2_NEW_PSYCHOLOGY_LINE,						// 847
		C2_LINE_PRICE_MA,							// 848
		C2_ORGAN_FINANCE_PUREBUYACCUM,				// 849
		C2_ORGAN_FINANCE_PUREBUY,					// 850
		C2_ORGAN_INSURE_PUREBUYACCUM,				// 851
		C2_ORGAN_INSURE_PUREBUY,					// 852
		C2_ORGAN_INVESTMENT_PUREBUYACCUM,			// 853
		C2_ORGAN_INVESTMENT_PUREBUY,				// 854
		C2_ORGAN_BANK_PUREBUYACCUM,					// 855
		C2_ORGAN_BANK_PUREBUY,						// 856
		C2_ORGAN_ETCFINANCE_PUREBUYACCUM,			// 857
		C2_ORGAN_ETCFINANCE_PUREBUY,				// 858
		C2_ORGAN_PENSIONFUND_PUREBUYACCUM,			// 859
		C2_ORGAN_PENSIONFUND_PUREBUY,				// 860
		C2_ORGAN_ETCCORP_PUREBUYACCUM,				// 861
		C2_ORGAN_ETCCORP_PUREBUY,					// 862
		C2_ORGAN_PRIVATEEQUITY_PUREBUYACCUM,		// 863
		C2_ORGAN_PRIVATEEQUITY_PUREBUY,				// 864
		C2_ORGAN_COUNTRY_PUREBUYACCUM,				// 865
		C2_ORGAN_COUNTRY_PUREBUY,					// 866
		C2_ORGAN__CREDIT_BALANCE_RATE,				// 867
		C2_ORGAN__CREDIT_GIVING_RATE,				// 868
		C2_ORGAN__CREDIT_BALANCE_QTY,				// 869
		C2_ORGAN__CREDIT_BALANCE_VARIATION,			// 870
		C2_FOREIGN_ORGAN_PUREBUY,					// 871
		C2_FOREIGN_ORGAN_PUREBUYACCUM,				// 872
//END 
		C3_MARKET_PROFILE					= 300,	// 300
		C3_PRICE,									// 301
		C3_LOCK,									// 302
		C3_OVERHANGING_SUPPLY,						// 303
		C3_PRESPAN,									// 304
		C3_VOLUME,									// 305
		C3_BIG_NEWS,								// 306
		C3_POST_SPAN,								// 307
		C3_PRE_SPAN_1,								// 308
		C3_PRE_SPAN_2,								// 309
		C3_NEWS_AND_NOTICE,							// 310
		C3_PRICE_MA_1,								// 311
		C3_PRICE_MA_2,								// 312
		C3_PRICE_MA_3,								// 313
		C3_PRICE_MA_4,								// 314
		C3_PRICE_MA_5,								// 315
		C3_PRICE_MA_6,								// 316
		C3_PRICE_MA_7,								// 317
		C3_PRICE_MA_8,								// 318
		C3_PRICE_MA_9,								// 319
		C3_PRICE_MA_10,								// 320
		C3_VOLUME_MA_1,								// 321
		C3_VOLUME_MA_2,								// 322
		C3_VOLUME_MA_3,								// 323
		C3_VOLUME_MA_4,								// 324
		C3_VOLUME_MA_5,								// 325
		C3_VOLUME_MA_6,								// 326
		C3_OPENGUIDELINE,							// 327
		C3_HIGHGUIDELINE,							// 328
		C3_LOWGUIDELINE,							// 329
		C3_BUY_SELL_OVERHANGING_SUPPLY,				// 330

		C4_ZOOMIN_ZOOMOUT					= 400,	// 400
		C4_WHOLEVIEW,								// 401
		C4_DATADLG_DRAG,							// 402
		C4_WHOLE_DATA_VIEW,							// 403
		C4_DATA_STATUSBAR,							// 404
		C4_CONFIG,									// 405
		C4_HIGHLIGHT,								// 406
		C4_GRAPHSTYLE,								// 407
		C4_LINE_THICKNESS,							// 408
		C4_COLOR,									// 409
		C4_INDICATOR_CONFIGURATION,					// 410
		C4_ENVIRONMENT_CONFIGURATION,				// 411
		C4_INDICATOR_LIST,							// 412
		C4_SERIAL_INDICATORS,						// 413
		C4_PANORAMA,								// 414
		C4_SCROLLBAR,								// 415
		C4_PREV,									// 416
		C4_NEXT,									// 417
		C4_RESET,									// 418
		C4_DELETE_SELECTED_TOOL,					// 419
		C4_DELETE_BLOCK,							// 420
		C4_RAINBOW,									// 421
		C4_OBVIOUSLYBALANCECHART,					// 422
		C4_P_AND_F,									// 423
		C4_THREELINEBREAK,							// 424
		C4_INVERSETIMELINE,							// 425
		C4_OVERHANGINGSUPPLY,						// 426
		C4_SYMBOLS,									// 427
		C4_RECTANGLE,								// 428
		C4_CIRCLE,									// 429
		C4_TRIANGLE,								// 430
		C4_TEXT,									// 431
		C4_SELECT_MODE,								// 432
		C4_TREND_LINE_PROFIT,						// 433
		C4_TREND_LINE,								// 434
		C4_CROSSHAIR_DRAG,							// 435
		C4_VERTICAL_LINE,							// 436
		C4_HORIZONTAL_LINE,							// 437
		C4_TRISECTION_LINE,							// 438
		C4_QUARTERSECTION_LINE,						// 439
		C4_RESISTANCE_AND_SUPPORT,					// 440
		C4_FIBONACCI_ARC,							// 441
		C4_FIBONACCI_CIRCLE,						// 442
		C4_FIBONACCI_FANLINE,						// 443
		C4_FIBONACCI_RETRACE,						// 444
		C4_FIBONACCI_TIME,							// 445
		C4_GANN_LINE_45_DEGREES,					// 446
		C4_GANN_LINE_135_DEGREES,					// 447
		C4_GANN_FAN_45_DEGREES,						// 448
		C4_GANN_FAN_135_DEGREES,					// 449
		C4_GANN_FAN_225_DEGREES,					// 450
		C4_GANN_FAN_315_DEGREES,					// 451
		C4_GANN_GRID,								// 452
		C4_ANDREWS_PITCHFORK,						// 453
		C4_ELLIOT_WAVELINE,							// 454
		C4_CYCLE_LINE,								// 455
		C4_CUT_LINE,								// 456
		C4_SPEED_LINE,								// 457
		C4_LINEAR_REGRESSION,						// 458
		C4_RAFF_REGRESSION,							// 459
		C4_DELETE_TOOL_WITH_REVERSE_ORDER,			// 460
		C4_DELETE_ALL_TOOL,							// 461
		C4_TRADER_REPORT,							// 462
		C4_SYSTEM_TRADER,							// 463
		C4_PRICE_FORECAST,							// 464
		C4_MAKE_FORMULA,							// 465
		C4_PATTERN_ANALYSIS,						// 466
		C4_SAVE_SCREEN,								// 467
		C4_PRINT2,									// 468
		C4_DAILY_CANDLE,							// 469
		C4_WEEKLY_CANDLE,							// 470
		C4_MONTHLY_CANDLE,							// 471
		C4_DISPLAY_COUNT_SLIDERBAR,					// 472
		C4_RESET_DISPLAY_COUNT,						// 473
		C4_SET_MIN_MAX_WITH_FULL_DATA,				// 474
		C4_MAGNIFYING_GLASS,						// 475
		C4_RADER_CHART,								// 476
		C4_REGISTER_TO_CONCERN,						// 477
		C4_JOIN_CHART_PAGE,							// 478
		C4_SHOW_OUTTER_TOOLBAR,						// 479
		C4_AUTOTRENDLINE_ANALYSIS,					// 480
		C4_TARGET_ANALYSIS,							// 481
		C4_SYSTEM_TRADER_INDICATOR_OPTIMIZATION,	// 482
		C4_STRATEGY_DEVELOPMENT,					// 483
		C4_STRATEGY_VALUATION,						// 484
		C4_TREND_LINE_SINGLE_DIRECTION_EXTENSIBLE,	// 485
		C4_TREND_LINE_BOTH_DIRECTION_EXTENSIBLE,	// 486
		C4_AUTO_SCROLL,								// 487
		C4_INVERT_CHART,							// 488
		C4_CROSSHAIR_TOOL,							// 489
		C4_CROSSHAIR_TOOL_WITH_DATADLG,				// 490
		C4_DATADLG_OVERTOOL,						// 491
		C4_CROSS_LINE,								// 492
		C4_CROSS_LINE_WITH_DATADLG,					// 493
		C4_DATADLG_CURSOR,							// 494
		C4_CROSSHAIR_CURSOR,						// 495
		C4_SPEED_FAN,								// 496
		C4_SPEED_ARC,								// 497
		C4_ANGLE_LINE,								// 498
		C4_CANDLE_LINE,								// 499
		C4_DRAW_CANDLE						= 900,	// 900
		C4_TRACING_TOOLTIP,							// 901
		C4_ADJUST_RATIO,							// 902
		C4_PERIOD_SUMMARY,							// 903
		C4_ENABLE_TIMEMARK,							// 904
		C4_Y_SCALE_SETUP,							// 905
		C4_DRAG_X_ZOOM,								// 906
		C4_DRAG_Y_ZOOM,								// 907
		C4_PAN,										// 908
		C4_UNDO,									// 909
		C4_REDO,									// 910
		C4_PENCIL_LINE,								// 911
		C4_PRINT,									// 912
		C4_ORDER_LINE,								// 913
		C4_ORDER_LINE_CONFIGURATION,				// 914
		C4_EXPORT_EXCEL_DATA,						// 915
		C4_INNER_TOOL_OPTION,						// 916
		C4_OUTER_TOOL_OPTION,						// 917
		C4_USED,									// 918
		C4_COUNT,									// 919
		C4_10_UNIT,									// 920
		C4_20_UNIT,									// 921
		C4_30_UNIT,									// 922
		C4_40_UNIT,									// 923
		C4_50_UNIT,									// 924
		C4_RATE,									// 925
		C4_BUTTON_TYPE,								// 926
		C4_ZOOM_IN,									// 927
		C4_ZOOM_OUT,								// 928
		C4_LEFT_AUTO_SCROLL,						// 929
		C4_RIGHT_AUTO_SCROLL,						// 930
		C4_STOP_AUTO_SCROLL,						// 931
		C4_USE_POPUP,								// 932
		C4_NOT_USE_POPUP,							// 933
		C4_GRAPH_ORDER,								// 934
		C4_PACKET_ORDER,							// 935
		C4_OVER_DLG,								// 936
		C4_WITH_DATA_DLG,							// 937
		C4_NO_DATA_DLG,								// 938
		C4_WITH_DATA_DLG2,							// 939
		C4_NO_DATA_DLG2,							// 940
		C4_COLORED_SIGN,							// 941
		C4_NOT_COLORED_SIGN,						// 942
		C4_FLAT_TYPE,								// 943
		C4_ORDER_LINK,								// 944
		C4_DATA_SIMULATOR,							// 945
		C4_TOOL_BALNACE_BASIC,						// 946

		C5_DATA_BAR_TYPE					= 500,	// 500
		C5_DATA_SIGNAL_TYPE,						// 501
		C5_CLASSIFICATION_TEXT_TYPE,				// 502
		C5_CANDLE_TYPE,								// 503
		C5_LINE_TYPE,								// 504
		C5_BAR_TYPE,								// 505
		C5_PIE_TYPE,								// 506
		C5_SPECIAL_TYPE,							// 507
		C5_FUND_TYPE,								// 508
		C5_DATA_EXPRESSION_TYPE,					// 509
		C5_FUTURE_OPTION_TYPE,						// 510
		C5_CANDLE_CHART,							// 511
		C5_BAR_CHART_OPEN_HIGH_LOW_CLOSE,			// 512
		C5_LINE_CHART,								// 513
		C5_ANTENNA_CHART,							// 514
		C5_BAR_CHART,								// 515
		C5_BAR_CHART_HIGH_LOW_CLOSE,				// 516
		C5_FLOW_CHART,								// 517
		C5_CANDLE_VOLUME_CHART,						// 518
		C5_EQUI_VOLUME_CHART,						// 519
		C5_JAPANESE_STYLE,							// 520
		C5_AMERICAN_STYLE,							// 521
		C5_LINE_STYLE,								// 522
		C5_ANTENNA_STYLE,							// 523

		C6_CHARACTER						= 600,	// 600
		C6_YES,										// 601
		C6_NO,										// 602
		C6_POINT,									// 603
		C6_DATE_TIME,								// 604
		C6_TEXT,									// 605
		C6_EXERCISED_PRICE,							// 606
		C6_INDICATOR_DATA,							// 607
		C6_PERCENT,									// 608
		C6_ASKED_PRICE_CHART,						// 609
		C6_CODE_ASKED_PRICE,						// 610
		C6_PREVIOUS_DAY_COMPARISON,					// 611
		C6_CHANGE_RATE,								// 612
		C6_TIME_CHART,								// 613
		C6_TIME_GRID,								// 614
		C6_PRICE_BAND,								// 615
		C6_PREVIOUS_DAY_COMP,						// 616
		C6_NEWS,									// 617
		C6_NOTICE,									// 618
		C6_TRILLION,								// 619
		C6_HUNDRED_BILLION,							// 620
		C6_TEN_BILLION,								// 621
		C6_BILLION,									// 622
		C6_HUNDRED_MILLION,							// 623
		C6_TEN_MILLION,								// 624
		C6_MILLION,									// 625
		C6_HUNDRED_THOUSAND,						// 626
		C6_TEN_THOUSAND,							// 627
		C6_END_OF_OPENING_TRADE,					// 628
		C6_END_OF_CLOSING_TRADE,					// 629
		C6_OVERTIME_TRADE,							// 630
		C6_END_OF_TRADING,							// 631
		C6_END_OF_OVERTIME_TRADE,					// 632
		C6_YEAR,									// 633
		C6_MONTH,									// 634
		C6_DAY,										// 635
		C6_HOUR,									// 636
		C6_MINUTE,									// 637
		C6_SECOND,									// 638
		C6_START_DATE,								// 639
		C6_END_DATE,								// 640
		C6_MAXIMUM_PRICE,							// 641
		C6_MINIMUM_PRICE,							// 642
		C6_MAXIMUM_VOLUME,							// 643
		C6_MINIMUM_VOLUME,							// 644
		C6_TIME,									// 645
		C6_DATE,									// 646
		C6_USER_DEFINE,								// 647
		C6_INPUT_ITEM,								// 648
		C6_END_OF_SINGLE_PRICE,						// 649
		C6_AFTER_CLOSING,							// 650

		C7_MARKET							= 700,	// 700
		C7_2ND_DERIVED,								// 701
		C7_PRICE,									// 702
		C7_VOLUME,									// 703
		C7_BASE,									// 704
		C7_ETC,										// 705
		C7_VOLATILITY,								// 706
		C7_STOCKS,									// 707
		C7_TREND,									// 708
		C7_ELASTICITY,								// 709
		C7_SPECIAL,									// 710
		C7_INVESTOR,								// 711

		META_CODE_END						= 1000,	// 1000
	};		
	// (2006/9/4 - Seung-Won, Bae) Support Indicator IDs
	enum EDDC_INDICATOR_ID
	{
		// 기본지표
		EDDC_PRICECHART,				// 가격차트
// vntsorl:[0003431] [7111]차트화면에서의 조회속도가 구차트에 비해 늦어 개선 요구
// 		EDDC_ST_PRICEMACHART,			// 가격 이동평균
		EDDC_OCX_PRICEMACHART,			// 가격 이동평균
		EDDC_VOLUMECHART,				// 거래량차트
// 		EDDC_ST_VOLUMEMACHART,			// 거래량 이동평균
		EDDC_OCX_VOLUMEMACHART,			// 거래량 이동평균

		// 가격지표
		EDDC_OCX_RAINBOWCHART,			// 그물차트
		EDDC_OCX_VOLUMEFORSALE,			// 대기매물
		EDDC_OCX_GLANCEBALANCECHART,	// 일목균형표
		EDDC_ST_BOLLINGERCHART,			// Bollinger Bands
		EDDC_OCX_BOXCHARTCHART,			// Box Chart
		EDDC_ST_DEMACHART,					// DEMA
		EDDC_ST_DEMARKCHART,				// Demark
		EDDC_ST_ENVELOPECHART,				// Envelope
		EDDC_ST_MACCHART,					// MAC
		EDDC_ST_PARABOLICCHART,			// Parabolic SAR
		EDDC_ST_PIVOTCHART,				// Pivot
		EDDC_ST_PRICECHANNELCHART,			// Price Channel
		EDDC_ST_STARCBANDSCHART,			// Starc Bands
		EDDC_ST_TEMACHART,					// TEMA
		EDDC_OCX_ZIGZAGCHART,			// Zig Zag
		EDDC_OCX_CLCCHART,			// 꺽은선차트
		EDDC_ST_STD_ERROR_BAND,			// 표준오차밴드

		// 변동성지표
		EDDC_ST_DISPARITYCHART,				// 이격도
		EDDC_ST_DISPARITYCHART_SMA,			// 이격도(단순)
		EDDC_ST_DISPARITYCHART_EMA,			// 이격도(지수)
		EDDC_ST_ABRATIO,					// AB Ratio
		EDDC_ST_ATR,						// ATR
		EDDC_ST_BAND_PERCENT_B,				// Band %B
		EDDC_ST_BAND_WIDTH,					// Band Width
		EDDC_ST_BWI,						// BWI
		EDDC_ST_CHAIKINS_VOLATILITY,		// Chaikin's Volatility
		EDDC_ST_DPO,						// DPO
		EDDC_ST_DRF,						// DRF
		EDDC_ST_ELDER_RAY_BEAR,				// Elder Ray Bear
		EDDC_ST_ELDER_RAY_BULL,				// Elder Ray Bull
		EDDC_ST_HV,							// 역사적변동성
		EDDC_ST_MASSINDEX,					// Mass Index
		EDDC_ST_NCO,						// NCO
		EDDC_ST_QSTIC,						// Qstic
		EDDC_ST_RMI,						// RMI
		EDDC_ST_RSICHART,					// RSI
		EDDC_ST_SIGMA,						// Sigma
		EDDC_ST_STANDARD_DEVIATION,			// Standard Deviation
		EDDC_ST_STANDARD_ERROR,				// Standard Error
		EDDC_ST_STOCHASTICSFASTCHART,		// Stochastic Fast
		EDDC_ST_STOCHASTICSFASTCHART2,		// Stochastic Fast2
		EDDC_ST_STOCHASTICSFASTCHART3,		// Stochastic Fast3
		EDDC_ST_STOCHASTICSSLOWCHART,		// Stochastic Slow
		EDDC_ST_STOCHASTICSSLOWCHART2,		// Stochastic Slow2
		EDDC_ST_STOCHASTICSSLOWCHART3,		// Stochastic Slow3
		EDDC_ST_WILLIAMCHART,				// William's %R

		// 추세지표
		EDDC_ST_ADLINE,						// AD Line
		EDDC_ST_ADX,						// ADX
		EDDC_ST_ADXR,						// ADXR
		EDDC_ST_AROON,						// Aroon
		EDDC_ST_CCI,						// CCI
		EDDC_ST_FORMULA,					// Formula
		EDDC_ST_LRS,						// LRS
		EDDC_ST_MACD,						// MACD
		EDDC_ST_MACD_OSCILLATOR,			// MACD Oscillator
		EDDC_ST_MAO,						// MAO
		EDDC_ST_MOMENTUM,					// Momentum
		EDDC_ST_NDI,						// NDI
		EDDC_ST_PRICEOSCILLATOR,			// Price Oscillator
		EDDC_ST_ROC,						// ROC
		EDDC_ST_SONAR,						// Sonar
		EDDC_ST_SROC,						// SROC
		EDDC_ST_TDI,						// TDI
		EDDC_ST_TRIX,						// TRIX
		EDDC_ST_VHF,						// VHF
		EDDC_ST_LRL,						// LRL
		EDDC_ST_TSF,						// TSF
		EDDC_ST_DMI,						// DMI
		EDDC_ST_VR,							// VR

		// 거래량지표
		EDDC_ST_CMF,						// CMF
		EDDC_ST_CHAIKINS_OSCILLATOR,		// Chaikin's Oscillator
		EDDC_ST_EOM,						// EOM
		EDDC_ST_FORCE_INDEX,				// Force Index
		EDDC_ST_MFI,						// MFI
		EDDC_ST_NVI,						// NVI
		EDDC_ST_OBV,						// OBV
		EDDC_ST_PVI,						// PVI
		EDDC_ST_VA_OSCILLATOR,				// VA Oscillator
		EDDC_ST_VOLUME_OSCILLATOR,			// Volume Oscillator
		EDDC_ST_VOLUME_RATIO,				// Volume Ratio
		EDDC_ST_VROC,						// VROC

		// 기타지표
		EDDC_ST_PSYCHCHART,					// 심리도// ,//// 투자심리선
		EDDC_ST_7_BINARY_WAVE,				// 7Binary Wave
		EDDC_ST_BINARY_WAVE,				// Binary Wave
		EDDC_ST_PVT,						// PVT
		EDDC_ST_NFI,						// NFI
		EDDC_ST_NEW_PSYCHOLOGY_LINE,		// 신심리도

		// 2차 파생지표
		EDDC_ST_LRS_STOCHASTIC,				// LRS+Stochastic
		EDDC_ST_MACD_STOCHASTIC,			// MACD+Stochastic
		EDDC_ST_MOMENTUM_STOCHASTIC,		// Momentum+Stochastic
		EDDC_ST_OBV_MOMENTUM,				// OBV+Momentum
		EDDC_ST_OBV_STOCHASTIC,				// OBV+Stochastic
		EDDC_ST_ROC_STOCHASTIC,				// ROC+Stochastic
		EDDC_ST_RSI_MACD,					// RSI+MACD
		EDDC_ST_RSI_STOCHASTIC,				// RSI+Stochastic
		EDDC_ST_SONAR_PSYCHCHART,			// Sonar+심리도
		EDDC_ST_STOCHASTIC_RSI,				// Stochastic+RSI
		EDDC_ST_PERCENT_B_STOCHASTIC,		// %B+Stochastic

		// 특수지표
		EDDC_OCX_THREELINEBRACKCHART,		// 삼선전환도
		EDDC_OCX_INVERSLINECHART,			// 역시계곡선
		EDDC_OCX_PANDFCHART,				// P&F
		EDDC_OCX_SWINGCHART,				// SwingChart
		EDDC_OCX_KAGICHART,					// KagiChart
		EDDC_OCX_RENKOCHART,				// RenkoChart// , // YJS(05/06/21) RenKoChart ADD.			
		EDDC_DISPERSIONCHART,				// 분산차트				// (2006.11.09 안승환) 분산차트 추가

		// 비정규 지표
		EDDC_OCX_COMPARATIVECHART,			// 상대비교차트
		EDDC_OCX_LOCKCHART,					// 락
		EDDC_OCX_MARKETPROFILECHART,		// MarketProfile
		EDDC_OCX_FUOPPANDFCHART,			// FuOpP&F
		EDDC_OCX_VOLUMESELLBUYCHART,		//매수매도대기매물

		// 기타 시장지표
		EDDC_VOLUMEPRICE,					// 거래대금
		EDDC_MINIJISUCHART,					// 미니지수차트
		EDDC_INDUSTRYJISUCHART,				// 업종지수
		EDDC_MARKETJISUCHART,				// 시장지수
		EDDC_OCX_ACCOUNT_ITEMSIZE,			// 거래내역
		EDDC_OCX_BIG_NEWS,					// Big News
		EDDC_OCX_FOURIER_TRANSFORM,			// Fourier Transform
		EDDC_ORGAN_PUREBUY_ACCUMULATION,	// 기관순매수누적
		EDDC_ORGAN_PUREBUY,					// 기관순매수량
		EDDC_SUSPENSE_CONTRACT,				// 미결제약정
		EDDC_TOTAL_AMOUNT,					// 시가총액
		EDDC_FOREIGN_HOLDING,				// 외국인 보유량
		EDDC_FOREIGN_HOLDING_RATE,			// 외국인 보유율
		EDDC_FOREIGN_PUREBUY,				// 외국인 순매수
		EDDC_KOSPI_PER_INVESTER,			// 투자자별거래소
		EDDC_KOSDAQ_PER_INVESTER,			// 투자자별코스닥
		EDDC_K200_PER_INVESTER,				// 투자자별K200
		EDDC_FUTURE_PER_INVESTER,			// 투자자별선물
		EDDC_CALL_OPTION_PER_INVESTER,		// 투자자별콜옵션
		EDDC_PUT_OPTION_PER_INVESTER,		// 투자자별풋옵션
		EDDC_PROGRAM_TRADE_PUREBUY_KOSPI,	// 프로그램순매수 거래소
		EDDC_PROGRAM_TRADE_PUREBUY_KOSDAQ,	// 프로그램순매수 코스닥
		
		EDDC_PERSON_PUREBUY,//844
		EDDC_PERSON_PUREBUYACCUM, //845
		EDDC_FOREIGN_PUREBUYACCM,//846
		EDDC_LINE_PRICE_MA,//848

		EDDC_ORGAN_FINANCE_PUREBUYACCUM,		// 기관(금융투자) 순매수누적
		EDDC_ORGAN_FINANCE_PUREBUY,				// 기관(금융투자) 순매수량
		EDDC_ORGAN_INSURE_PUREBUYACCUM,			// 기관(보험) 순매수누적
		EDDC_ORGAN_INSURE_PUREBUY,				// 기관(보험) 순매수량
		EDDC_ORGAN_INVESTMENT_PUREBUYACCUM,		// 기관(투신) 순매수누적
		EDDC_ORGAN_INVESTMENT_PUREBUY,			// 기관(투신) 순매수량
		EDDC_ORGAN_PRIVATEEQUITY_PUREBUYACCUM,	// 기관(사모) 순매수누적
		EDDC_ORGAN_PRIVATEEQUITY_PUREBUY,		// 기관(사모) 순매수량
		EDDC_ORGAN_BANK_PUREBUYACCUM,			// 기관(은행) 순매수누적
		EDDC_ORGAN_BANK_PUREBUY,				// 기관(은행) 순매수량
		EDDC_ORGAN_ETCFINANCE_PUREBUYACCUM,	// 기관(기타금융) 순매수누적
		EDDC_ORGAN_ETCFINANCE_PUREBUY,		// 기관(기타금융) 순매수량
		EDDC_ORGAN_PENSIONFUND_PUREBUYACCUM,	// 기관(연기금) 순매수누적
		EDDC_ORGAN_PENSIONFUND_PUREBUY,			// 기관(연기금) 순매수량
		EDDC_ORGAN_COUNTRY_PUREBUYACCUM,		// 기관(국가) 순매수누적
		EDDC_ORGAN_COUNTRY_PUREBUY,				// 기관(국가) 순매수량
		EDDC_ORGAN_ETCCORP_PUREBUYACCUM,		// 기관(기타법인) 순매수누적
		EDDC_ORGAN_ETCCORP_PUREBUY,			// 기관(기타법인) 순매수량
		EDDC_ADL,													// 업종 시장지표 ADL
		EDDC_ADR,													// 업종 시장지표 ADR
		EDDC_ORGAN_CREDIT_BALANCE_RATE,				// 신용잔고율
		EDDC_ORGAN_CREDIT_GIVING_RATE,				// 신용공여율
		EDDC_ORGAN_CREDIT_BALANCE_QTY,				// 신용잔고수량
		EDDC_ORGAN_CREDIT_BALANCE_VARIATION,		// 신용잔고증감
		EDDC_FOREIGN_ORGAN_PUREBUY,					// 외국인+기관 순매수
		EDDC_FOREIGN_ORGAN_PUREBUYACCUM,			// 외국인+기관 순매수누적

		EDDC_INDICATOR_COUNT
	};
		
// (2008/1/8 - Seung-Won, Bae) It used in Only OCX for Initilizing and Terminating.
friend class CKTBChartCtrl;
friend class CKTBChartApp;
public:
	CString			m_strDataFolderPath;
protected:
	DECLARE_FUNCTION2(	GetMetaTable,				const char **, NULL, const char *, const char *);
	DECLARE_FUNCTION4(	CreateLocalLanguageTable,	BOOL, FALSE, HWND, E_LANGUAGEID, const char *, BOOL);
	DECLARE_FUNCTION1(	RemoveLocalLanguageTable,	BOOL, FALSE, HWND);
	DECLARE_FUNCTION0(	GetIndicatorNameTable,		const char **, NULL);
	DECLARE_FUNCTION0(	UnloadAllResourceDLL,		BOOL, FALSE);

// (2008/1/14 - Seung-Won, Bae) It used in Only Design Property DLL for Design.
friend class CChartDesignPropertyAddInImp;
friend class CDataOfPacketPage;
friend class CDlgSubGraphSetting;
friend class CDFontPropPage;
protected:
	DECLARE_FUNCTION1(	GetLocalLanguageTable,	const char *, NULL, HWND);
	DECLARE_FUNCTION3(	CreateLocalMetaText,	const char *, NULL, HWND, E_CODEPAGE, const char *);
	DECLARE_FUNCTION3(	SetLocalMetaText,		BOOL, FALSE, HWND, const char *, const char *);

public:
	const char **		m_szMetaTables;
	static E_METACODE	m_eToolOptions[];
	static int			m_nToolOptionCount;
protected:
	DECLARE_FUNCTION4(	GetMetaText_,					const char *, NULL, HWND, E_CODEPAGE, const char *, BOOL);
	DECLARE_FUNCTION4(	GetLanguageText_,				const char *, NULL, HWND, E_CODEPAGE, const char *, BOOL);
	DECLARE_FUNCTION4(	GetMetaTextFromOther_,			const char *, NULL, HWND, E_CODEPAGE, const char *, BOOL);
	DECLARE_FUNCTION4(	GetMetaTextForScript_,			const char *, NULL, HWND, E_CODEPAGE, const char *, BOOL);
	DECLARE_FUNCTION3(	GetMetaTextDirectly_,			const char *, NULL, E_CODEPAGE, E_LANGUAGEID, const char *);
	DECLARE_FUNCTION3(	GetLanguageTextDirectly_,		const char *, NULL, E_CODEPAGE, E_LANGUAGEID, const char *);
public:
	DECLARE_FUNCTION1(	GetCurrentLanguageID,			E_LANGUAGEID, ENGLISH, HWND);
	DECLARE_FUNCTION2(	LoadResourceDLL,				HMODULE, AfxGetStaticModuleState()->m_hCurrentInstanceHandle, HWND, HMODULE);
	DECLARE_FUNCTION1(	GetMetaTextCount,				int, 0, E_CODEPAGE);
	DECLARE_FUNCTION2(	GetAllMetaTexts,				BOOL, FALSE, E_CODEPAGE, char **);
public:
	void			LoadMetaTable( void);
	const char *	GetIndicatorName( int p_nIndicatorID);
public:
	const char *	GetMetaText( HWND p_hOcxWnd, E_CODEPAGE p_eCodePage, const char *p_szLanguageText, BOOL p_bReturnNULL);
	const char *	GetLanguageText( HWND p_hOcxWnd, E_CODEPAGE p_eCodePage, const char *p_szMetaText, BOOL p_bReturnNULL);
	const char *	GetMetaTextFromOther( HWND p_hOcxWnd, E_CODEPAGE p_eCodePage, const char *p_szOtherMetaText, BOOL p_bReturnNULL);
	const char *	GetMetaTextForScript( HWND p_hOcxWnd, E_CODEPAGE p_eCodePage, const char *p_szOtherMOrLText, BOOL p_bReturnNULL);
	const char *	GetMetaTextDirectly( E_CODEPAGE p_eCodePage, E_LANGUAGEID p_eLanguageID, const char *p_szLanguageText);
	const char *	GetLanguageTextDirectly( E_CODEPAGE p_eCodePage, E_LANGUAGEID p_eLanguageID, const char *p_szMetaText);
	CString			GetMultipleMetaText( HWND p_hOcxWnd, CString strLanguageTexts, CString strSeperators, int nCodePage1, int nCodePage2);
	CString			GetMultipleLanguageText( HWND p_hOcxWnd, CString strMetaTexts, CString strSeperators, int nCodePage1, int nCodePage2);
	CString			GetMultipleMetaTextFromOther( HWND p_hOcxWnd, CString strOtherMetaTexts, CString strSeperators, int nCodePage1, int nCodePage2);
	CString			GetMultipleMetaTextForScript( HWND p_hOcxWnd, CString strOtherMOrLTexts, CString strSeperators, int nCodePage1, int nCodePage2);

public:
	const char **		m_szIndicatorNames;
public:
	DECLARE_FUNCTION1(	GetIndicatorID,					int, -1, const char *);

// Scale Type
public:	
	bool IsScaleGroup_Price(const CString& IndicatorName);
	bool IsScaleGroup_PriceNoVertScale(const CString& IndicatorName);
	bool IsScaleGroup_Volume(const CString& IndicatorName);

// Vertical Min/Max Type	~ CViewMinMaxCalculator::GetViewVertMinMax_Indicate()
public:	
	bool IsViewVertMinMax_EndPacket(const CString& IndicatorName);	// min/max 구하는 packet이 마지막(2) 인 경우
	bool IsScaleGroup_InverseLine(const CString& IndicatorName);
	bool IsDrawGroup_PAndF(const CString& IndicatorName);				// <P&F> 형태의 지표인지 체크
	bool IsScaleGroup_PlusPercent(const CString& IndicatorName);
	bool IsScaleGroup_MinusPercent(const CString& IndicatorName);

// Title Type	~ CGraphImp::ChangeTitleDraw()
public:	
	bool IsConditionTitle(const CString& IndicatorName);				// title에 쓸 내용인지 체크
	bool IsDataTitle(const CString& IndicatorName);					// title이 조건값인지의 여부

// Title Type	~ CGraphPartData::GetConditionTitle()
public:
	bool IsTitleOnlyCondition(const CString& IndicatorName);			// "title = title + 조건값" 인지의 여부
	bool IsTitleOrCondition(const CString& IndicatorName);
	bool IsTitleAndCondition(const CString& IndicatorName);			// 지표에 조건값 서브그래프별 표시 - ojtaso (20071116)

// Query Indicator Static Properties (ETC)
public:
	bool IsNoXScale_SpecialGroup(const CString& IndicatorName);
	bool IsTitleIncludeCondition(const CString& IndicatorName);		// "title = 조건값" 인지의 여부
	bool IsDrawGroup_VolumeType(const CString& IndicatorName);		// 해당 그리는 group 인지 체크
	bool IsSpecialGroupGraph( const char *p_szIndicatorName);			// <특수차트> 인지의 여부
	// (2009/11/6 - Seung-Won, Bae) Do not disable zoom and scroll on OneChart
	bool IsNoneZoomChart( const char *p_szIndicatorName);

// (2006/9/6 - Seung-Won, Bae) ST Index Indicator
public:
	BOOL	IsSTIndexIndicator( int p_nID);

// (2006/11/1 - Seung-Won, Bae) Support Price Chart Type.
public:
	BOOL	IsChartType( const char *p_szTypeName);
	BOOL	IsPriceChartType( const char *p_szTypeName);
	BOOL	IsVolumeForSale(const CString& IndicatorName);
	
// (2007/1/22 - Seung-Won, Bae) Support the Indicator that can not be added with mulitple.
public:
	BOOL	CanNotMultipleAddedOnMultipleSameNameIndicator( const char *p_szIndicatorName);

// (2009/9/16 - Seung-Won, Bae) for Market Index and Server Data
public:
	BOOL	IsMarketOrServerIndex( const char *p_szIndicatorName);

// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
public:
	BOOL	IsIndexNeedVolume( const char *p_szIndicatorName);
	BOOL	IsIndicatorInThisMode( const char *p_szIndicatorName, CChartInfo::CHART_MODE p_eChartMode);

// (2009/10/23 - Seung-Won, Bae) for Special Time
public:
	BOOL	IsSpecialTime( int p_nTime);
};
extern IMetaTable g_iMetaTable;

//////////////////////////////////////////////////////////////////////
// CMultiLanguageResourceSetter
//////////////////////////////////////////////////////////////////////
class CMultiLanguageResourceSetter
{
protected:
	HINSTANCE	m_hInstance;
public:
	CMultiLanguageResourceSetter( HINSTANCE p_hResource)
	{
		m_hInstance = AfxGetResourceHandle();
		AfxSetResourceHandle( p_hResource);
	}
	~CMultiLanguageResourceSetter()
	{
		AfxSetResourceHandle( m_hInstance);
	}
};

#endif // !defined(AFX_IMETATABLE_H__B983D82E_B4D1_4263_84E4_548B78FA6538__INCLUDED_)
