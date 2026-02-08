// MetaTable.cpp: implementation of the IMetaTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IMetaTable.h"

#include "../../Include/ProgIDInfo.h"		// for CProgIDInfo
#include "../MapStringToWord.h"			// for CMapStringToWord

//////////////////////////////////////////////////////////////////////
// IMetaTable
//////////////////////////////////////////////////////////////////////

IMetaTable	g_iMetaTable;

IMetaTable::IMetaTable( void)
{
	m_szMetaTables		= NULL;
	m_szIndicatorNames	= NULL;
#ifndef _MT_MANUAL_LOAD
	LoadMetaTable();
#endif
}

IMetaTable::~IMetaTable()
{
}

IMetaTable::E_METACODE IMetaTable::m_eToolOptions[] =
{
	C4_ZOOMIN_ZOOMOUT,
	C4_WHOLEVIEW,
	C4_DATADLG_DRAG,
	C4_WHOLE_DATA_VIEW,
	C4_DATA_STATUSBAR,
	C4_CONFIG,
	C4_HIGHLIGHT,
	C4_GRAPHSTYLE,
	C4_LINE_THICKNESS,
	C4_COLOR,
	C4_INDICATOR_CONFIGURATION,
	C4_ENVIRONMENT_CONFIGURATION,
	C4_INDICATOR_LIST,
	C4_SERIAL_INDICATORS,
	C4_PANORAMA,
	C4_SCROLLBAR,
	C4_PREV,
	C4_NEXT,
	C4_RESET,
	C4_DELETE_SELECTED_TOOL,
	C4_DELETE_BLOCK,
	C4_RAINBOW,
	C4_OBVIOUSLYBALANCECHART,
	C4_P_AND_F,
	C4_THREELINEBREAK,
	C4_INVERSETIMELINE,
	C4_OVERHANGINGSUPPLY,
	C4_SYMBOLS,
	C4_RECTANGLE,
	C4_CIRCLE,
	C4_TRIANGLE,
	C4_TEXT,
	C4_SELECT_MODE,
	C4_TREND_LINE_PROFIT,
	C4_TREND_LINE,
	C4_CROSSHAIR_DRAG,
	C4_VERTICAL_LINE,
	C4_HORIZONTAL_LINE,
	C4_TRISECTION_LINE,
	C4_QUARTERSECTION_LINE,
	C4_RESISTANCE_AND_SUPPORT,
	C4_FIBONACCI_ARC,
	C4_FIBONACCI_CIRCLE,
	C4_FIBONACCI_FANLINE,
	C4_FIBONACCI_RETRACE,
	C4_FIBONACCI_TIME,
	C4_GANN_LINE_45_DEGREES,
	C4_GANN_LINE_135_DEGREES,
	C4_GANN_FAN_45_DEGREES,
	C4_GANN_FAN_135_DEGREES,
	C4_GANN_FAN_225_DEGREES,
	C4_GANN_FAN_315_DEGREES,
	C4_GANN_GRID,
	C4_ANDREWS_PITCHFORK,
	C4_ELLIOT_WAVELINE,
	C4_CYCLE_LINE,
	C4_CUT_LINE,
	C4_SPEED_LINE,
	C4_LINEAR_REGRESSION,
	C4_RAFF_REGRESSION,
	C4_DELETE_TOOL_WITH_REVERSE_ORDER,
	C4_DELETE_ALL_TOOL,
	C4_TRADER_REPORT,
	C4_SYSTEM_TRADER,
	C4_PRICE_FORECAST,
	C4_MAKE_FORMULA,
	C4_PATTERN_ANALYSIS,
	C4_SAVE_SCREEN,
	C4_PRINT2,
	C4_DAILY_CANDLE,
	C4_WEEKLY_CANDLE,
	C4_MONTHLY_CANDLE,
	C4_DISPLAY_COUNT_SLIDERBAR,
	C4_RESET_DISPLAY_COUNT,
	C4_SET_MIN_MAX_WITH_FULL_DATA,
	C4_MAGNIFYING_GLASS,
	C4_RADER_CHART,
	C4_REGISTER_TO_CONCERN,
	C4_JOIN_CHART_PAGE,
	C4_SHOW_OUTTER_TOOLBAR,
	C4_AUTOTRENDLINE_ANALYSIS,
	C4_TARGET_ANALYSIS,
	C4_SYSTEM_TRADER_INDICATOR_OPTIMIZATION,
	C4_STRATEGY_DEVELOPMENT,
	C4_STRATEGY_VALUATION,
	C4_TREND_LINE_SINGLE_DIRECTION_EXTENSIBLE,
	C4_TREND_LINE_BOTH_DIRECTION_EXTENSIBLE,
	C4_AUTO_SCROLL,
	C4_INVERT_CHART,
	C4_CROSSHAIR_TOOL,
	C4_CROSSHAIR_TOOL_WITH_DATADLG,
	C4_DATADLG_OVERTOOL,
	C4_CROSS_LINE,
	C4_CROSS_LINE_WITH_DATADLG,
	C4_DATADLG_CURSOR,
	C4_CROSSHAIR_CURSOR,
	C4_SPEED_FAN,
	C4_SPEED_ARC,
	C4_ANGLE_LINE,
	C4_CANDLE_LINE,
	C4_DRAW_CANDLE,
	C4_TRACING_TOOLTIP,
	C4_ADJUST_RATIO,
	C4_PERIOD_SUMMARY,
	C4_ENABLE_TIMEMARK,
	C4_Y_SCALE_SETUP,
	C4_DRAG_X_ZOOM,
	C4_DRAG_Y_ZOOM,
	C4_PAN,
	C4_UNDO,
	C4_REDO,
	C4_PENCIL_LINE,
	C4_PRINT,
	C4_ORDER_LINE,
	C4_ORDER_LINE_CONFIGURATION,
	C4_EXPORT_EXCEL_DATA,
	C4_ORDER_LINK
};
int IMetaTable::m_nToolOptionCount = sizeof( IMetaTable::m_eToolOptions) / sizeof( IMetaTable::m_eToolOptions[ 0]);

void IMetaTable::LoadMetaTable( void)
{
	if( !m_szMetaTables)
	{
		CString strOcxPath;
		HMODULE hLanguageTableDLL = GetModuleHandle( "ChartLanguageTable.dll");
		if( !hLanguageTableDLL)
		{
			// (2008/4/29 - Seung-Won, Bae) to use the ProgID in ChartMetaTable.dat
			char szProgID[ 256];
			DWORD dwError = GetPrivateProfileString( "Chart Version", "ProgID", "Prog ID Error", szProgID, 255, m_strDataFolderPath + "ChartMetaTable.dat");

			CProgIDInfo::ComServerFromProgID( szProgID, strOcxPath);
			strOcxPath = strOcxPath.Left( strOcxPath.ReverseFind( '\\') + 1);
		}

		LoadDll( strOcxPath + "ChartLanguageTable.dll");
		m_szMetaTables = GetMetaTable( m_strDataFolderPath, NULL);
		m_szIndicatorNames = GetIndicatorNameTable();
	}
}

const char *IMetaTable::GetIndicatorName( int p_nIndicatorID)
{
	if( p_nIndicatorID < EDDC_PRICECHART || EDDC_INDICATOR_COUNT <= p_nIndicatorID) return NULL;
	return m_szIndicatorNames[ p_nIndicatorID];
}

const char *IMetaTable::GetMetaText( HWND p_hOcxWnd, E_CODEPAGE p_eCodePage, const char *p_szLanguageText, BOOL p_bReturnNULL)
{
	const char *szMetaText = GetMetaText_( p_hOcxWnd, p_eCodePage, p_szLanguageText, p_bReturnNULL);
	if( !szMetaText && !p_bReturnNULL) return p_szLanguageText;
	return szMetaText;
}

const char *IMetaTable::GetLanguageText( HWND p_hOcxWnd, E_CODEPAGE p_eCodePage, const char *p_szMetaText, BOOL p_bReturnNULL)
{
	const char *szLanguageText = GetLanguageText_( p_hOcxWnd, p_eCodePage, p_szMetaText, p_bReturnNULL);
	if( !szLanguageText && !p_bReturnNULL) return p_szMetaText;
	return szLanguageText;
}

const char *IMetaTable::GetMetaTextFromOther( HWND p_hOcxWnd, E_CODEPAGE p_eCodePage, const char *p_szOtherMetaText, BOOL p_bReturnNULL)
{
	const char *szMetaText = GetMetaTextFromOther_( p_hOcxWnd, p_eCodePage, p_szOtherMetaText, p_bReturnNULL);
	if( !szMetaText && !p_bReturnNULL) return p_szOtherMetaText;
	return szMetaText;
}

const char *IMetaTable::GetMetaTextForScript( HWND p_hOcxWnd, E_CODEPAGE p_eCodePage, const char *p_szOtherMOrLText, BOOL p_bReturnNULL)
{
	const char *szMetaText = GetMetaTextForScript_( p_hOcxWnd, p_eCodePage, p_szOtherMOrLText, p_bReturnNULL);
	if( !szMetaText && !p_bReturnNULL) return p_szOtherMOrLText;
	return szMetaText;
}

const char *IMetaTable::GetMetaTextDirectly( E_CODEPAGE p_eCodePage, E_LANGUAGEID p_eLanguageID, const char *p_szLanguageText)
{
	const char *szMetaText = GetMetaTextDirectly_( p_eCodePage, p_eLanguageID, p_szLanguageText);
	if( !szMetaText) return p_szLanguageText;
	return szMetaText;
}

const char *IMetaTable::GetLanguageTextDirectly( E_CODEPAGE p_eCodePage, E_LANGUAGEID p_eLanguageID, const char *p_szMetaText)
{
	const char *szLanguageText = GetLanguageTextDirectly_( p_eCodePage, p_eLanguageID, p_szMetaText);
	if( !szLanguageText) return p_szMetaText;
	return szLanguageText;
}


CString IMetaTable::GetMultipleMetaText( HWND p_hOcxWnd, CString strLanguageTexts, CString strSeperators, int nCodePage1, int nCodePage2)
{
	CString strMetaTexts, strItem, strItemOrg;

	while( !strLanguageTexts.IsEmpty())
	{
		strItemOrg = strLanguageTexts.SpanExcluding( strSeperators);
		strLanguageTexts = strLanguageTexts.Mid( strItemOrg.GetLength());
		if( !strItemOrg.IsEmpty())
		{
			strItem = g_iMetaTable.GetMetaText( p_hOcxWnd, ( IMetaTable::E_CODEPAGE)nCodePage1, strItemOrg, TRUE);
			if( strItem.IsEmpty() && 0 <= nCodePage2)
				strItem = g_iMetaTable.GetMetaText( p_hOcxWnd, ( IMetaTable::E_CODEPAGE)nCodePage2, strItemOrg, FALSE);
			strMetaTexts += strItem;
		}

		strItemOrg = strLanguageTexts.SpanIncluding( strSeperators);
		strLanguageTexts = strLanguageTexts.Mid( strItemOrg.GetLength());
		strMetaTexts += strItemOrg;
	}

	return strMetaTexts;
}

CString	IMetaTable::GetMultipleLanguageText( HWND p_hOcxWnd, CString strMetaTexts, CString strSeperators, int nCodePage1, int nCodePage2)
{
	CString strLanguageTexts, strItem, strItemOrg;

	while( !strMetaTexts.IsEmpty())
	{
		strItemOrg = strMetaTexts.SpanExcluding( strSeperators);
		strMetaTexts = strMetaTexts.Mid( strItemOrg.GetLength());
		if( !strItemOrg.IsEmpty())
		{
			strItem = g_iMetaTable.GetLanguageText( p_hOcxWnd, ( IMetaTable::E_CODEPAGE)nCodePage1, strItemOrg, TRUE);
			if( strItem.IsEmpty() && 0 <= nCodePage2)
				strItem = g_iMetaTable.GetLanguageText( p_hOcxWnd, ( IMetaTable::E_CODEPAGE)nCodePage2, strItemOrg, FALSE);
			strLanguageTexts += strItem;
		}

		strItemOrg = strMetaTexts.SpanIncluding( strSeperators);
		strMetaTexts = strMetaTexts.Mid( strItemOrg.GetLength());
		strLanguageTexts += strItemOrg;
	}

	return strLanguageTexts;
}

CString IMetaTable::GetMultipleMetaTextFromOther( HWND p_hOcxWnd, CString strOtherMetaTexts, CString strSeperators, int nCodePage1, int nCodePage2)
{
	CString strMetaTexts, strItem, strItemOrg;

	while( !strOtherMetaTexts.IsEmpty())
	{
		strItemOrg = strOtherMetaTexts.SpanExcluding( strSeperators);
		strOtherMetaTexts = strOtherMetaTexts.Mid( strItemOrg.GetLength());
		if( !strItemOrg.IsEmpty())
		{
			strItem = g_iMetaTable.GetMetaTextFromOther( p_hOcxWnd, ( IMetaTable::E_CODEPAGE)nCodePage1, strItemOrg, TRUE);
			if( strItem.IsEmpty() && 0 <= nCodePage2)
				strItem = g_iMetaTable.GetMetaTextFromOther( p_hOcxWnd, ( IMetaTable::E_CODEPAGE)nCodePage2, strItemOrg, FALSE);
			strMetaTexts += strItem;
		}

		strItemOrg = strOtherMetaTexts.SpanIncluding( strSeperators);
		strOtherMetaTexts = strOtherMetaTexts.Mid( strItemOrg.GetLength());
		strMetaTexts += strItemOrg;
	}

	return strMetaTexts;
}

CString IMetaTable::GetMultipleMetaTextForScript( HWND p_hOcxWnd, CString strOtherMOrLTexts, CString strSeperators, int nCodePage1, int nCodePage2)
{
	CString strMetaTexts, strItem, strItemOrg;

	while( !strOtherMOrLTexts.IsEmpty())
	{
		strItemOrg = strOtherMOrLTexts.SpanExcluding( strSeperators);
		strOtherMOrLTexts = strOtherMOrLTexts.Mid( strItemOrg.GetLength());
		if( !strItemOrg.IsEmpty())
		{
			strItem = g_iMetaTable.GetMetaTextForScript( p_hOcxWnd, ( IMetaTable::E_CODEPAGE)nCodePage1, strItemOrg, TRUE);
			if( strItem.IsEmpty() && 0 <= nCodePage2)
				strItem = g_iMetaTable.GetMetaTextForScript( p_hOcxWnd, ( IMetaTable::E_CODEPAGE)nCodePage2, strItemOrg, FALSE);
			strMetaTexts += strItem;
		}

		strItemOrg = strOtherMOrLTexts.SpanIncluding( strSeperators);
		strOtherMOrLTexts = strOtherMOrLTexts.Mid( strItemOrg.GetLength());
		strMetaTexts += strItemOrg;
	}

	return strMetaTexts;
}


//////////////////////////////////////////////////////////////////////
// Scale Type
//////////////////////////////////////////////////////////////////////

// 해당 scale group 인치 체크
bool IMetaTable::IsScaleGroup_Price(const CString& IndicatorName)
{
	switch( GetIndicatorID( IndicatorName))
	{
		case EDDC_PRICECHART:
		case EDDC_ST_PRICEMACHART:
		case EDDC_ST_PARABOLICCHART:
		case EDDC_OCX_RAINBOWCHART:
		case EDDC_OCX_ZIGZAGCHART:
		case EDDC_OCX_CLCCHART:
		case EDDC_ST_BOLLINGERCHART:
		case EDDC_ST_ENVELOPECHART:
		case EDDC_OCX_BOXCHARTCHART:
		case EDDC_ST_PIVOTCHART:
		case EDDC_OCX_GLANCEBALANCECHART:
		case EDDC_ST_PRICECHANNELCHART:
		case EDDC_ST_DEMARKCHART:
		case EDDC_OCX_VOLUMEFORSALE:
		case EDDC_OCX_LOCKCHART:
		case EDDC_ST_DEMACHART:
		case EDDC_ST_MACCHART:
		case EDDC_ST_STARCBANDSCHART:
		case EDDC_ST_TEMACHART:
									return true;
	}
	return false;
}

// <가격차트> group 이지만 scale은 따로 쓰는 경우.
bool IMetaTable::IsScaleGroup_PriceNoVertScale(const CString& IndicatorName)
{
	if(IndicatorName.IsEmpty()) return false;

	// <업종지수>, <시장지수>
	switch( GetIndicatorID( IndicatorName))
	{
		case EDDC_INDUSTRYJISUCHART:
		case EDDC_MARKETJISUCHART:
									return true;
	}
	return false;
}

bool IMetaTable::IsScaleGroup_Volume(const CString& IndicatorName)
{
	// <거래량차트>, <거래량 이동평균>
	switch( GetIndicatorID( IndicatorName))
	{
		case EDDC_VOLUMECHART:
		case EDDC_ST_VOLUMEMACHART:
									return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
// Vertical Min/Max Type	~ CViewMinMaxCalculator::GetViewVertMinMax_Indicate()
//////////////////////////////////////////////////////////////////////

// min/max 구하는 packet이 마지막(2) 인 경우
bool IMetaTable::IsViewVertMinMax_EndPacket(const CString& IndicatorName)
{
	if( IndicatorName.IsEmpty()) return false;

	switch( GetIndicatorID( IndicatorName))
	{
		case EDDC_OCX_ZIGZAGCHART:
		case EDDC_OCX_CLCCHART:
										return true;
	}

	return false;
}

bool IMetaTable::IsScaleGroup_InverseLine(const CString& IndicatorName)
{
	switch( GetIndicatorID( IndicatorName))
	{
		case EDDC_OCX_INVERSLINECHART:	
			return true;
	}

	return false;
}

// <P&F> 형태의 지표인지 체크
bool IMetaTable::IsDrawGroup_PAndF(const CString& IndicatorName)
{
	return (IndicatorName.Find( _MTEXT( C2_P_AND_F)) != -1);
}

bool IMetaTable::IsScaleGroup_PlusPercent(const CString& IndicatorName)
{
	switch( GetIndicatorID( IndicatorName))
	{
		case EDDC_INDUSTRYJISUCHART:
		case EDDC_ST_PSYCHCHART:
		case EDDC_ST_SONAR_PSYCHCHART:
		case EDDC_ST_RSICHART:
		case EDDC_ST_STOCHASTICSFASTCHART:
		case EDDC_ST_STOCHASTICSSLOWCHART:
		case EDDC_ST_LRS_STOCHASTIC:
		case EDDC_ST_MACD_STOCHASTIC:
		case EDDC_ST_MOMENTUM_STOCHASTIC:
		case EDDC_ST_OBV_STOCHASTIC:
		case EDDC_ST_ROC_STOCHASTIC:
		case EDDC_ST_RSI_STOCHASTIC:
		case EDDC_ST_STOCHASTIC_RSI:
		case EDDC_ST_PERCENT_B_STOCHASTIC:
											return true;
	}
	return false;
}

bool IMetaTable::IsScaleGroup_MinusPercent(const CString& IndicatorName)
{
	if(IndicatorName.IsEmpty()) return false;

	switch( GetIndicatorID( IndicatorName))
	{
		case EDDC_ST_WILLIAMCHART:	return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////
// Title Type	~ CGraphImp::ChangeTitleDraw()
//////////////////////////////////////////////////////////////////////

// condition title 인지 체크
bool IMetaTable::IsConditionTitle(const CString& IndicatorName)
{
	if(IndicatorName.IsEmpty()) return false;

	if( IsTitleIncludeCondition( IndicatorName))	// 가격 이동평균, 거래량 이동평균, 이격도
		return false;
	else if(IndicatorName.Find( _MTEXT( C2_P_AND_F)) != -1)// P&F, FuOpP&F
		return false;

	switch( GetIndicatorID( IndicatorName))
	{
		case EDDC_PRICECHART:
		case EDDC_VOLUMECHART:
		case EDDC_OCX_COMPARATIVECHART:
												return false;
	}

	return true;
}

// data title 인지 체크
bool IMetaTable::IsDataTitle(const CString& IndicatorName)
{
	if(IndicatorName.IsEmpty())
		return false;

	switch( GetIndicatorID( IndicatorName))
	{
		case EDDC_PRICECHART:
		case EDDC_VOLUMECHART:
		case EDDC_OCX_RAINBOWCHART:
		case EDDC_OCX_ZIGZAGCHART:
		case EDDC_OCX_CLCCHART:
		case EDDC_OCX_BOXCHARTCHART:
		case EDDC_OCX_VOLUMEFORSALE:
		case EDDC_OCX_VOLUMESELLBYCHART:
												return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// Title Type	~ CGraphPartData::GetConditionTitle()
//////////////////////////////////////////////////////////////////////

// "title = 조건값" 인지의 여부
bool IMetaTable::IsTitleOnlyCondition(const CString& IndicatorName)
{
	if(IndicatorName.IsEmpty()) return false;
	switch( GetIndicatorID( IndicatorName))
	{
		case EDDC_ST_PRICEMACHART:
		case EDDC_ST_VOLUMEMACHART:
												return true;
	}

	return false;
}

// "title = title_조건값" 인지의 여부
bool IMetaTable::IsTitleOrCondition(const CString& IndicatorName)
{
	if(IndicatorName.IsEmpty())
		return false;

	// 지표에 조건값 서브그래프별 표시 - ojtaso (20071116)
//	if( IndicatorName == INDICATEDATA_ST_DISPARITYCHART_SMA || IndicatorName == INDICATEDATA_ST_DISPARITYCHART_EMA) return true; // 이격도

	return false;
}

// "title = title(조건값)" 인지의 여부
// 지표에 조건값 서브그래프별 표시 - ojtaso (20071116)
bool IMetaTable::IsTitleAndCondition(const CString& IndicatorName)
{
	if(IndicatorName.IsEmpty())
		return false;

	// 지표에 조건값 서브그래프별 표시 - ojtaso (20071116)
	switch( GetIndicatorID( IndicatorName))
	{
		case EDDC_ST_DISPARITYCHART:
		case EDDC_ST_DISPARITYCHART_SMA:
		case EDDC_ST_DISPARITYCHART_EMA:
												return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////
// Query Indicator Static Properties (ETC)
//////////////////////////////////////////////////////////////////////

// x축 스케일을 표현하지 않는지 체크
bool IMetaTable::IsNoXScale_SpecialGroup(const CString& IndicatorName)
{
	switch( GetIndicatorID( IndicatorName))
	{
		case EDDC_OCX_PANDFCHART:
								return true;
	}

//	else if(IndicatorName == INDICATEDATA_OCX_THREELINEBRACKCHART) // "삼선전환도"
//		return true;

	return false;
}

// title에 조건값을 사용하는지의 여부
bool IMetaTable::IsTitleIncludeCondition(const CString& IndicatorName)
{
	if(IndicatorName.IsEmpty())
		return false;

	if(IsTitleOnlyCondition(IndicatorName))	// 가격 이동평균, 거래량 이동평균
		return true;

	return IsTitleOrCondition(IndicatorName); // 이격도
}

// <거래량 차트>, <거래대금> 인 경우
bool IMetaTable::IsDrawGroup_VolumeType(const CString& IndicatorName)
{
	switch( GetIndicatorID( IndicatorName))
	{
		case EDDC_VOLUMECHART:
		case EDDC_VOLUMEPRICE:
								return true;
	}
	return false;
}

// <특수차트> 인지의 여부
bool IMetaTable::IsSpecialGroupGraph( const char *p_szIndicatorName)
{
	switch( GetIndicatorID( p_szIndicatorName))
	{
		case EDDC_OCX_THREELINEBRACKCHART:
		case EDDC_OCX_INVERSLINECHART:
		case EDDC_OCX_PANDFCHART:
		case EDDC_OCX_SWINGCHART:
		case EDDC_OCX_KAGICHART:
		case EDDC_OCX_RENKOCHART:
		case EDDC_DISPERSIONCHART:		// (2006.11.09 안승환) 분산차트 추가
										return true;
	}
	return false;
}


// (2006/9/6 - Seung-Won, Bae) ST Index Indicator
//		Caution! in later, ask it on ChartSTIndexAddIn.dll
BOOL IMetaTable::IsSTIndexIndicator( int p_nID)
{
	switch( p_nID)
	{
		// (2006/11/19 - Seung-Won, Bae) Dongbu, SK List
		case EDDC_ST_WILLIAMCHART:
		case EDDC_ST_7_BINARY_WAVE:
		case EDDC_ST_PRICEMACHART:
		case EDDC_ST_VOLUMEMACHART:
		case EDDC_ST_STD_ERROR_BAND:
		case EDDC_ST_PSYCHCHART:
		case EDDC_ST_DISPARITYCHART_SMA:
		case EDDC_ST_DISPARITYCHART_EMA:
		case EDDC_ST_ABRATIO:
		case EDDC_ST_ADLINE:
		case EDDC_ST_ADX:
		case EDDC_ST_ADXR:
		case EDDC_ST_AROON:
		case EDDC_ST_ATR:
		case EDDC_ST_BAND_PERCENT_B:
		case EDDC_ST_BAND_WIDTH:
		case EDDC_ST_BINARY_WAVE:
		case EDDC_ST_BWI:
		case EDDC_ST_BOLLINGERCHART:
		case EDDC_ST_CCI:
		case EDDC_ST_CHAIKINS_OSCILLATOR:
		case EDDC_ST_CHAIKINS_VOLATILITY:
		case EDDC_ST_CMF:
		case EDDC_ST_DEMACHART:
		case EDDC_ST_DEMARKCHART:
		case EDDC_ST_DPO:
		case EDDC_ST_DRF:
		case EDDC_ST_ELDER_RAY_BEAR:
		case EDDC_ST_ELDER_RAY_BULL:
		case EDDC_ST_ENVELOPECHART:
		case EDDC_ST_EOM:
		case EDDC_ST_FORMULA:
		case EDDC_ST_FORCE_INDEX:
		case EDDC_ST_HV:
		case EDDC_ST_LRS:
		case EDDC_ST_MACCHART:
		case EDDC_ST_MACD:
		case EDDC_ST_MACD_OSCILLATOR:
		case EDDC_ST_MAO:
		case EDDC_ST_MASSINDEX:
		case EDDC_ST_MFI:
		case EDDC_ST_MOMENTUM:
		case EDDC_ST_NCO:
		case EDDC_ST_NDI:
		case EDDC_ST_NVI:
		case EDDC_ST_OBV:
		case EDDC_ST_PARABOLICCHART:
		case EDDC_ST_PIVOTCHART:
		case EDDC_ST_PRICECHANNELCHART:
		case EDDC_ST_PRICEOSCILLATOR:
		case EDDC_ST_PVI:
		case EDDC_ST_PVT:
		case EDDC_ST_QSTIC:
		case EDDC_ST_RMI:
		case EDDC_ST_ROC:
		case EDDC_ST_RSICHART:
		case EDDC_ST_SIGMA:
		case EDDC_ST_SONAR:
		case EDDC_ST_SROC:
		case EDDC_ST_STANDARD_DEVIATION:
		case EDDC_ST_STANDARD_ERROR:
		case EDDC_ST_STARCBANDSCHART:
		case EDDC_ST_STOCHASTICSFASTCHART:
		case EDDC_ST_STOCHASTICSFASTCHART2:
		case EDDC_ST_STOCHASTICSFASTCHART3:
		case EDDC_ST_STOCHASTICSSLOWCHART:
		case EDDC_ST_STOCHASTICSSLOWCHART2:
		case EDDC_ST_STOCHASTICSSLOWCHART3:
		case EDDC_ST_TDI:
		case EDDC_ST_TEMACHART:
		case EDDC_ST_TRIX:
		case EDDC_ST_VA_OSCILLATOR:
		case EDDC_ST_VHF:
		case EDDC_ST_VOLUME_OSCILLATOR:
		case EDDC_ST_VOLUME_RATIO:
		case EDDC_ST_VROC:
		case EDDC_ST_PERCENT_B_STOCHASTIC:
		case EDDC_ST_LRS_STOCHASTIC:
		case EDDC_ST_MACD_STOCHASTIC:
		case EDDC_ST_MOMENTUM_STOCHASTIC:
		case EDDC_ST_OBV_MOMENTUM:
		case EDDC_ST_OBV_STOCHASTIC:
		case EDDC_ST_ROC_STOCHASTIC:
		case EDDC_ST_RSI_MACD:
		case EDDC_ST_RSI_STOCHASTIC:
		case EDDC_ST_SONAR_PSYCHCHART:
		case EDDC_ST_STOCHASTIC_RSI:
		// (2006/11/19 - Seung-Won, Bae) Samsung Added.
		case EDDC_ST_DISPARITYCHART:
		case EDDC_ST_DMI:
		case EDDC_ST_LRL:
		case EDDC_ST_NFI:
		case EDDC_ST_TSF:
		case EDDC_ST_VR:
									return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
// (2006/11/1 - Seung-Won, Bae) Support Price Chart Type.
//////////////////////////////////////////////////////////////////////
// (2004.05.21, 배승원) 켄들차트/바차트/선차트의 Chart Type을 제공한다.
//		가격차트인지 특수 차트인지 식별하는 Routine을 제공한다.
BOOL IMetaTable::IsChartType( const char *p_szChartName)
{
	return IsSpecialGroupGraph( p_szChartName) || IsPriceChartType( p_szChartName);
}
BOOL IMetaTable::IsPriceChartType( const char *p_szChartName)
{
	static CMapStringToWord mapType;
	if( mapType.GetCount() <= 0)
	{
		mapType.SetAt( _MTEXT( C5_CANDLE_CHART), 0);
		mapType.SetAt( _MTEXT( C5_BAR_CHART), 0);
		mapType.SetAt( _MTEXT( C5_BAR_CHART_OPEN_HIGH_LOW_CLOSE), 0);
		mapType.SetAt( _MTEXT( C5_ANTENNA_CHART), 0);
		mapType.SetAt( _MTEXT( C5_LINE_CHART), 0);
		mapType.SetAt( _MTEXT( C5_BAR_CHART_HIGH_LOW_CLOSE), 0);
		mapType.SetAt( _MTEXT( C5_FLOW_CHART), 0);
		mapType.SetAt( _MTEXT( C5_CANDLE_VOLUME_CHART), 0);
		mapType.SetAt( _MTEXT( C5_EQUI_VOLUME_CHART), 0);
	}

	WORD wTemp;
	return mapType.Lookup( p_szChartName, wTemp);
}
BOOL IMetaTable::IsVolumeForSale(const CString& IndicatorName)
{
	if(IndicatorName.IsEmpty())
		return FALSE;

	switch( GetIndicatorID( IndicatorName))
	{
		case EDDC_OCX_VOLUMEFORSALE:
		case EDDC_OCX_VOLUMESELLBYCHART:
												return TRUE;
	}

	return FALSE;	
}

//////////////////////////////////////////////////////////////////////
// (2007/1/22 - Seung-Won, Bae) Support the Indicator that can not be added with mulitple.
//////////////////////////////////////////////////////////////////////
BOOL IMetaTable::CanNotMultipleAddedOnMultipleSameNameIndicator( const char *p_szIndicatorName)
{
	static CMapStringToWord mapType;
	if( mapType.GetCount() <= 0)
	{
		mapType.SetAt( _MTEXT( C2_OVERHANGING_SUPPLY), 0);
		mapType.SetAt( _MTEXT( C2_RAINBOW_CHART), 0);
		mapType.SetAt( _MTEXT( C2_OBVIOUSLY_BALANCE_CHART), 0);
		mapType.SetAt( _MTEXT( C2_VOLUME_CHART), 0);
		mapType.SetAt( _MTEXT( C2_DEMA), 0);
		mapType.SetAt( _MTEXT( C2_PIVOT), 0);
	}

	WORD wTemp;
	return mapType.Lookup( p_szIndicatorName, wTemp);
}
