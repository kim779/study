// MetaTable.cpp: implementation of the CMetaTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartLanguageTable.h"
#include "MetaTable.h"

#include <io.h>						// for _filelength()
#include "CodePage.h"				// for CCodePage

#define _CODE_PAGE_KEY				"[CodePage "

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const char *CMetaTable::m_szNullString = "";
const char *( CMetaTable::m_szMetaTexts[ IMetaTable::META_CODE_END + 1]);

CMetaTable::CMetaTable()
{
	for( int i = IMetaTable::PACKET_NAME; i < IMetaTable::CODE_PAGE_END; i++) m_pCodePage[ i] = NULL;
	for( int i = IMetaTable::C0_DATE_TIME; i < IMetaTable::META_CODE_END; i++) m_szMetaTexts[ i] = m_szNullString;
	for( int i = IMetaTable::EDDC_PRICECHART; i < IMetaTable::EDDC_INDICATOR_COUNT; i++) m_szIndicatorNames[ i] = m_szNullString;
}

CMetaTable::~CMetaTable()
{
	for( int i = IMetaTable::PACKET_NAME; i < IMetaTable::CODE_PAGE_END; i++)
		if( m_pCodePage[ i]) delete m_pCodePage[ i];
}

void CMetaTable::LoadMetaTable( const char *p_szMetaTableName, const char *p_szDataFolder)
{
	// (2008/1/7 - Seung-Won, Bae) Load Meta Table File
	FILE *stream = fopen( p_szDataFolder + CString( "ChartMetaTable.dat"), "rb");
	if( stream == NULL)
	{
		AfxMessageBox( "Error in reading Chart Meta File!");
		return;
	}

	//fseek( stream, 0L, SEEK_SET);
	//long nbytes = _filelength( stream->_file);
	fseek(stream, 0L, SEEK_END);//tour2k
	long nbytes = ftell(stream);
	fseek(stream, 0L, SEEK_SET);

	char* buffer = 	m_strMetaFileStringData.GetBufferSetLength( nbytes);
	fread( buffer, sizeof(char), nbytes, stream);
    fclose( stream);

	// Get Code Page
	char *szNext = buffer;
	szNext = strstr( szNext, "[CodePage ");
	char *szNextCodePage = NULL;
	char *szNextLine = NULL;
	char *szNextItem = NULL;
	int nCodePage = 0;
	while( szNext)
	{
		szNext += strlen( _CODE_PAGE_KEY);

		szNextCodePage = strstr( szNext, "[CodePage ");
		if( szNextCodePage) *szNextCodePage = '\0';

		szNextLine = strstr( szNext, "\r\n");
		if( szNextLine)
		{
			szNextLine++;
			*szNextLine = '\0';
			
			szNextItem = strchr( szNext, ']');
			if( szNextItem)
			{
				*szNextItem = '\0';

				nCodePage = atoi( szNext);
				if( IMetaTable::PACKET_NAME <= nCodePage && nCodePage < IMetaTable::META_CODE_END)
				{
					if( !m_pCodePage[ nCodePage]) m_pCodePage[ nCodePage] = new CCodePage( nCodePage);

					szNext = szNextLine;
					szNext++;

					if( !m_pCodePage[ nCodePage]->LoadMetaTable( szNext)) return;
				}
			}
		}

		szNext = szNextCodePage;
	}

	// 기본지표
	m_szIndicatorNames[ IMetaTable::EDDC_PRICECHART]				= m_szMetaTexts[ IMetaTable::C2_PRICE_CHART];			// 가격차트
// vntsorl:[0003431] [7111]차트화면에서의 조회속도가 구차트에 비해 늦어 개선 요구
// 	m_szIndicatorNames[ IMetaTable::EDDC_ST_PRICEMACHART]			= m_szMetaTexts[ IMetaTable::C2_PRICE_MA];				// 가격 이동평균
	m_szIndicatorNames[ IMetaTable::EDDC_OCX_PRICEMACHART]			= m_szMetaTexts[ IMetaTable::C2_PRICE_MA];				// 가격 이동평균
	m_szIndicatorNames[ IMetaTable::EDDC_VOLUMECHART]				= m_szMetaTexts[ IMetaTable::C2_VOLUME_CHART];			// 거래량차트
// 	m_szIndicatorNames[ IMetaTable::EDDC_ST_VOLUMEMACHART]			= m_szMetaTexts[ IMetaTable::C2_VOLUME_MA];				// 거래량 이동평균
	m_szIndicatorNames[ IMetaTable::EDDC_OCX_VOLUMEMACHART]			= m_szMetaTexts[ IMetaTable::C2_VOLUME_MA];				// 거래량 이동평균

	// 가격지표
	m_szIndicatorNames[ IMetaTable::EDDC_OCX_RAINBOWCHART]			= m_szMetaTexts[ IMetaTable::C2_RAINBOW_CHART];			// 그물차트
	m_szIndicatorNames[ IMetaTable::EDDC_OCX_VOLUMEFORSALE]			= m_szMetaTexts[ IMetaTable::C2_OVERHANGING_SUPPLY];			// 대기매물
	m_szIndicatorNames[ IMetaTable::EDDC_OCX_GLANCEBALANCECHART]	= m_szMetaTexts[ IMetaTable::C2_OBVIOUSLY_BALANCE_CHART];		// 일목균형표
	m_szIndicatorNames[ IMetaTable::EDDC_ST_BOLLINGERCHART]			= m_szMetaTexts[ IMetaTable::C2_BOLLINGER_BANDS];				// Bollinger Bands
	m_szIndicatorNames[ IMetaTable::EDDC_OCX_BOXCHARTCHART]			= m_szMetaTexts[ IMetaTable::C2_BOX_CHART];			// Box Chart
	m_szIndicatorNames[ IMetaTable::EDDC_ST_DEMACHART]				= m_szMetaTexts[ IMetaTable::C2_DEMA];					// DEMA
	m_szIndicatorNames[ IMetaTable::EDDC_ST_DEMARKCHART]			= m_szMetaTexts[ IMetaTable::C2_DEMARK];					// Demark
	m_szIndicatorNames[ IMetaTable::EDDC_ST_ENVELOPECHART]			= m_szMetaTexts[ IMetaTable::C2_ENVELOPE];				// Envelope
	m_szIndicatorNames[ IMetaTable::EDDC_ST_MACCHART]				= m_szMetaTexts[ IMetaTable::C2_MAC];					// MAC
	m_szIndicatorNames[ IMetaTable::EDDC_ST_PARABOLICCHART]			= m_szMetaTexts[ IMetaTable::C2_PARABOLIC_SAR];				// Parabolic SAR
	m_szIndicatorNames[ IMetaTable::EDDC_ST_PIVOTCHART]				= m_szMetaTexts[ IMetaTable::C2_PIVOT];					// Pivot
	m_szIndicatorNames[ IMetaTable::EDDC_ST_PRICECHANNELCHART]		= m_szMetaTexts[ IMetaTable::C2_PRICE_CHANNEL];			// Price Channel
	m_szIndicatorNames[ IMetaTable::EDDC_ST_STARCBANDSCHART]		= m_szMetaTexts[ IMetaTable::C2_STARC_BANDS];				// Starc Bands
	m_szIndicatorNames[ IMetaTable::EDDC_ST_TEMACHART]				= m_szMetaTexts[ IMetaTable::C2_TEMA];					// TEMA
	m_szIndicatorNames[ IMetaTable::EDDC_OCX_ZIGZAGCHART]			= m_szMetaTexts[ IMetaTable::C2_ZIG_ZAG];				// Zig Zag
	m_szIndicatorNames[ IMetaTable::EDDC_OCX_CLCCHART]				= m_szMetaTexts[ IMetaTable::C2_BROKEN_LINE_CHART];				// 꺽은선차트
	m_szIndicatorNames[ IMetaTable::EDDC_ST_STD_ERROR_BAND]			= m_szMetaTexts[ IMetaTable::C2_STANDARD_ERROR_BAND];				// 표준오차밴드

	// 변동성지표
	m_szIndicatorNames[ IMetaTable::EDDC_ST_DISPARITYCHART]			= m_szMetaTexts[ IMetaTable::C2_DISPARITY];				// 이격도
	m_szIndicatorNames[ IMetaTable::EDDC_ST_DISPARITYCHART_SMA]		= m_szMetaTexts[ IMetaTable::C2_DISPARITY_SMA];			// 이격도(단순)
	m_szIndicatorNames[ IMetaTable::EDDC_ST_DISPARITYCHART_EMA]		= m_szMetaTexts[ IMetaTable::C2_DISPARITY_EMA];			// 이격도(지수)
	m_szIndicatorNames[ IMetaTable::EDDC_ST_ABRATIO]				= m_szMetaTexts[ IMetaTable::C2_AB_RATIO];						// AB Ratio
	m_szIndicatorNames[ IMetaTable::EDDC_ST_ATR]					= m_szMetaTexts[ IMetaTable::C2_ATR];							// ATR
	m_szIndicatorNames[ IMetaTable::EDDC_ST_BAND_PERCENT_B]			= m_szMetaTexts[ IMetaTable::C2_BAND_PERCENT_B];				// Band %B
	m_szIndicatorNames[ IMetaTable::EDDC_ST_BAND_WIDTH]				= m_szMetaTexts[ IMetaTable::C2_BAND_WIDTH];					// Band Width
	m_szIndicatorNames[ IMetaTable::EDDC_ST_BWI]					= m_szMetaTexts[ IMetaTable::C2_BWI];					// BWI
	m_szIndicatorNames[ IMetaTable::EDDC_ST_CHAIKINS_VOLATILITY]	= m_szMetaTexts[ IMetaTable::C2_CHAIKINS_VOLATILITY];			// Chaikin's Volatility
	m_szIndicatorNames[ IMetaTable::EDDC_ST_DPO]					= m_szMetaTexts[ IMetaTable::C2_DPO];							// DPO
	m_szIndicatorNames[ IMetaTable::EDDC_ST_DRF]					= m_szMetaTexts[ IMetaTable::C2_DRF];							// DRF
	m_szIndicatorNames[ IMetaTable::EDDC_ST_ELDER_RAY_BEAR]			= m_szMetaTexts[ IMetaTable::C2_ELDER_RAY_BEAR];				// Elder Ray Bear
	m_szIndicatorNames[ IMetaTable::EDDC_ST_ELDER_RAY_BULL]			= m_szMetaTexts[ IMetaTable::C2_ELDER_RAY_BULL];				// Elder Ray Bull
	m_szIndicatorNames[ IMetaTable::EDDC_ST_HV]						= m_szMetaTexts[ IMetaTable::C2_HISTORICAL_VOLATILITY];							// 역사적변동성
	m_szIndicatorNames[ IMetaTable::EDDC_ST_MASSINDEX]				= m_szMetaTexts[ IMetaTable::C2_MASS_INDEX];					// Mass Index
	m_szIndicatorNames[ IMetaTable::EDDC_ST_NCO]					= m_szMetaTexts[ IMetaTable::C2_NCO];							// NCO
	m_szIndicatorNames[ IMetaTable::EDDC_ST_QSTIC]					= m_szMetaTexts[ IMetaTable::C2_QSTIC];						// Qstic
	m_szIndicatorNames[ IMetaTable::EDDC_ST_RMI]					= m_szMetaTexts[ IMetaTable::C2_RMI];							// RMI
	m_szIndicatorNames[ IMetaTable::EDDC_ST_RSICHART]				= m_szMetaTexts[ IMetaTable::C2_RSI];					// RSI
	m_szIndicatorNames[ IMetaTable::EDDC_ST_SIGMA]					= m_szMetaTexts[ IMetaTable::C2_SIGMA];						// Sigma
	m_szIndicatorNames[ IMetaTable::EDDC_ST_STANDARD_DEVIATION]		= m_szMetaTexts[ IMetaTable::C2_STANDARD_DEVIATION];			// Standard Deviation
	m_szIndicatorNames[ IMetaTable::EDDC_ST_STANDARD_ERROR]			= m_szMetaTexts[ IMetaTable::C2_STANDARD_ERROR];				// Standard Error
	m_szIndicatorNames[ IMetaTable::EDDC_ST_STOCHASTICSFASTCHART]	= m_szMetaTexts[ IMetaTable::C2_STOCHASTIC_FAST];		// Stochastic Fast
	m_szIndicatorNames[ IMetaTable::EDDC_ST_STOCHASTICSFASTCHART2]	= m_szMetaTexts[ IMetaTable::C2_STOCHASTIC_FAST2];		// Stochastic Fast2
	m_szIndicatorNames[ IMetaTable::EDDC_ST_STOCHASTICSFASTCHART3]	= m_szMetaTexts[ IMetaTable::C2_STOCHASTIC_FAST3];		// Stochastic Fast3
	m_szIndicatorNames[ IMetaTable::EDDC_ST_STOCHASTICSSLOWCHART]	= m_szMetaTexts[ IMetaTable::C2_STOCHASTIC_SLOW];		// Stochastic Slow
	m_szIndicatorNames[ IMetaTable::EDDC_ST_STOCHASTICSSLOWCHART2]	= m_szMetaTexts[ IMetaTable::C2_STOCHASTIC_SLOW2];		// Stochastic Slow2
	m_szIndicatorNames[ IMetaTable::EDDC_ST_STOCHASTICSSLOWCHART3]	= m_szMetaTexts[ IMetaTable::C2_STOCHASTIC_SLOW3];		// Stochastic Slow3
	m_szIndicatorNames[ IMetaTable::EDDC_ST_WILLIAMCHART]			= m_szMetaTexts[ IMetaTable::C2_PERCENT_R];				// William's %R

	// 추세지표
	m_szIndicatorNames[ IMetaTable::EDDC_ST_ADLINE]					= m_szMetaTexts[ IMetaTable::C2_AD_LINE];						// AD Line
	m_szIndicatorNames[ IMetaTable::EDDC_ST_ADX]					= m_szMetaTexts[ IMetaTable::C2_ADX];							// ADX
	m_szIndicatorNames[ IMetaTable::EDDC_ST_ADXR]					= m_szMetaTexts[ IMetaTable::C2_ADXR];						// ADXR
	m_szIndicatorNames[ IMetaTable::EDDC_ST_AROON]					= m_szMetaTexts[ IMetaTable::C2_AROON];						// Aroon
	m_szIndicatorNames[ IMetaTable::EDDC_ST_CCI]					= m_szMetaTexts[ IMetaTable::C2_CCI];							// CCI
	m_szIndicatorNames[ IMetaTable::EDDC_ST_FORMULA]				= m_szMetaTexts[ IMetaTable::C2_FORMULA];						// Formula
	m_szIndicatorNames[ IMetaTable::EDDC_ST_LRS]					= m_szMetaTexts[ IMetaTable::C2_LRS];							// LRS
	m_szIndicatorNames[ IMetaTable::EDDC_ST_MACD]					= m_szMetaTexts[ IMetaTable::C2_MACD];						// MACD
	m_szIndicatorNames[ IMetaTable::EDDC_ST_MACD_OSCILLATOR]		= m_szMetaTexts[ IMetaTable::C2_MACD_OSCILLATOR];				// MACD Oscillator
	m_szIndicatorNames[ IMetaTable::EDDC_ST_MAO]					= m_szMetaTexts[ IMetaTable::C2_MAO];							// MAO
	m_szIndicatorNames[ IMetaTable::EDDC_ST_MOMENTUM]				= m_szMetaTexts[ IMetaTable::C2_MOMENTUM];					// Momentum
	m_szIndicatorNames[ IMetaTable::EDDC_ST_NDI]					= m_szMetaTexts[ IMetaTable::C2_NDI];							// NDI
	m_szIndicatorNames[ IMetaTable::EDDC_ST_PRICEOSCILLATOR]		= m_szMetaTexts[ IMetaTable::C2_PRICE_OSCILLATOR];				// Price Oscillator
	m_szIndicatorNames[ IMetaTable::EDDC_ST_ROC]					= m_szMetaTexts[ IMetaTable::C2_ROC];							// ROC
	m_szIndicatorNames[ IMetaTable::EDDC_ST_SONAR]					= m_szMetaTexts[ IMetaTable::C2_SONAR];						// Sonar
	m_szIndicatorNames[ IMetaTable::EDDC_ST_SROC]					= m_szMetaTexts[ IMetaTable::C2_SROC];						// SROC
	m_szIndicatorNames[ IMetaTable::EDDC_ST_TDI]					= m_szMetaTexts[ IMetaTable::C2_TDI];							// TDI
	m_szIndicatorNames[ IMetaTable::EDDC_ST_TRIX]					= m_szMetaTexts[ IMetaTable::C2_TRIX];						// TRIX
	m_szIndicatorNames[ IMetaTable::EDDC_ST_VHF]					= m_szMetaTexts[ IMetaTable::C2_VHF];							// VHF
	m_szIndicatorNames[ IMetaTable::EDDC_ST_LRL]					= m_szMetaTexts[ IMetaTable::C2_LRL];							// LRL
	m_szIndicatorNames[ IMetaTable::EDDC_ST_TSF]					= m_szMetaTexts[ IMetaTable::C2_TSF];							// TSF
	m_szIndicatorNames[ IMetaTable::EDDC_ST_DMI]					= m_szMetaTexts[ IMetaTable::C2_DMI];							// DMI
	m_szIndicatorNames[ IMetaTable::EDDC_ST_VR]						= m_szMetaTexts[ IMetaTable::C2_VR];							// VR

	// 거래량지표
	m_szIndicatorNames[ IMetaTable::EDDC_ST_CMF]					= m_szMetaTexts[ IMetaTable::C2_CMF];							// CMF
	m_szIndicatorNames[ IMetaTable::EDDC_ST_CHAIKINS_OSCILLATOR]	= m_szMetaTexts[ IMetaTable::C2_CHAIKINS_OSCILLATOR];			// Chaikin's Oscillator
	m_szIndicatorNames[ IMetaTable::EDDC_ST_EOM]					= m_szMetaTexts[ IMetaTable::C2_EOM];							// EOM
	m_szIndicatorNames[ IMetaTable::EDDC_ST_FORCE_INDEX]			= m_szMetaTexts[ IMetaTable::C2_FORCE_INDEX];					// Force Index
	m_szIndicatorNames[ IMetaTable::EDDC_ST_MFI]					= m_szMetaTexts[ IMetaTable::C2_MFI];							// MFI
	m_szIndicatorNames[ IMetaTable::EDDC_ST_NVI]					= m_szMetaTexts[ IMetaTable::C2_NVI];							// NVI
	m_szIndicatorNames[ IMetaTable::EDDC_ST_OBV]					= m_szMetaTexts[ IMetaTable::C2_OBV];						// OBV
	m_szIndicatorNames[ IMetaTable::EDDC_ST_PVI]					= m_szMetaTexts[ IMetaTable::C2_PVI];							// PVI
	m_szIndicatorNames[ IMetaTable::EDDC_ST_VA_OSCILLATOR]			= m_szMetaTexts[ IMetaTable::C2_VA_OSCILLATOR];				// VA Oscillator
	m_szIndicatorNames[ IMetaTable::EDDC_ST_VOLUME_OSCILLATOR]		= m_szMetaTexts[ IMetaTable::C2_VOLUME_OSCILLATOR];			// Volume Oscillator
	m_szIndicatorNames[ IMetaTable::EDDC_ST_VOLUME_RATIO]			= m_szMetaTexts[ IMetaTable::C2_VOLUME_RATIO];				// Volume Ratio
	m_szIndicatorNames[ IMetaTable::EDDC_ST_VROC]					= m_szMetaTexts[ IMetaTable::C2_VROC];						// VROC

	// 기타지표
	m_szIndicatorNames[ IMetaTable::EDDC_ST_PSYCHCHART]				= m_szMetaTexts[ IMetaTable::C2_PSYCHOLOGICAL_LINE];				// 심리도, 투자심리선
	m_szIndicatorNames[ IMetaTable::EDDC_ST_7_BINARY_WAVE]			= m_szMetaTexts[ IMetaTable::C2_7_BINARY_WAVE];			// 7Binary Wave
	m_szIndicatorNames[ IMetaTable::EDDC_ST_BINARY_WAVE]			= m_szMetaTexts[ IMetaTable::C2_BINARY_WAVE];					// Binary Wave
	m_szIndicatorNames[ IMetaTable::EDDC_ST_PVT]					= m_szMetaTexts[ IMetaTable::C2_PVT];							// PVT
	m_szIndicatorNames[ IMetaTable::EDDC_ST_NFI]					= m_szMetaTexts[ IMetaTable::C2_NFI];							// NFI
	m_szIndicatorNames[ IMetaTable::EDDC_ST_NEW_PSYCHOLOGY_LINE]	= m_szMetaTexts[ IMetaTable::C2_NEW_PSYCHOLOGY_LINE];			// 신심리도

	// 2차 파생지표
	m_szIndicatorNames[ IMetaTable::EDDC_ST_LRS_STOCHASTIC]			= m_szMetaTexts[ IMetaTable::C2_LRS_STOCHASTIC];				// LRS+Stochastic
	m_szIndicatorNames[ IMetaTable::EDDC_ST_MACD_STOCHASTIC]		= m_szMetaTexts[ IMetaTable::C2_MACD_STOCHASTIC];				// MACD+Stochastic
	m_szIndicatorNames[ IMetaTable::EDDC_ST_MOMENTUM_STOCHASTIC]	= m_szMetaTexts[ IMetaTable::C2_MOMENTUM_STOCHASTIC];			// Momentum+Stochastic
	m_szIndicatorNames[ IMetaTable::EDDC_ST_OBV_MOMENTUM]			= m_szMetaTexts[ IMetaTable::C2_OBV_MOMENTUM];				// OBV+Momentum
	m_szIndicatorNames[ IMetaTable::EDDC_ST_OBV_STOCHASTIC]			= m_szMetaTexts[ IMetaTable::C2_OBV_STOCHASTIC];				// OBV+Stochastic
	m_szIndicatorNames[ IMetaTable::EDDC_ST_ROC_STOCHASTIC]			= m_szMetaTexts[ IMetaTable::C2_ROC_STOCHASTIC];				// ROC+Stochastic
	m_szIndicatorNames[ IMetaTable::EDDC_ST_RSI_MACD]				= m_szMetaTexts[ IMetaTable::C2_RSI_MACD];					// RSI+MACD
	m_szIndicatorNames[ IMetaTable::EDDC_ST_RSI_STOCHASTIC]			= m_szMetaTexts[ IMetaTable::C2_RSI_STOCHASTIC];				// RSI+Stochastic
	m_szIndicatorNames[ IMetaTable::EDDC_ST_SONAR_PSYCHCHART]		= m_szMetaTexts[ IMetaTable::C2_SONAR_PSYCHOLOGY];			// Sonar+심리도
	m_szIndicatorNames[ IMetaTable::EDDC_ST_STOCHASTIC_RSI]			= m_szMetaTexts[ IMetaTable::C2_STOCHASTIC_RSI];				// Stochastic+RSI
	m_szIndicatorNames[ IMetaTable::EDDC_ST_PERCENT_B_STOCHASTIC]	= m_szMetaTexts[ IMetaTable::C2_PERCENT_B_STOCHASTIC];		// %B+Stochastic

	// 특수지표
	m_szIndicatorNames[ IMetaTable::EDDC_OCX_THREELINEBRACKCHART]	= m_szMetaTexts[ IMetaTable::C2_THREE_LINE_BREAK];		// 삼선전환도
	m_szIndicatorNames[ IMetaTable::EDDC_OCX_INVERSLINECHART]		= m_szMetaTexts[ IMetaTable::C2_INVERSE_TIME_LINE];			// 역시계곡선
	m_szIndicatorNames[ IMetaTable::EDDC_OCX_PANDFCHART]			= m_szMetaTexts[ IMetaTable::C2_P_AND_F];				// P&F
	m_szIndicatorNames[ IMetaTable::EDDC_OCX_SWINGCHART]			= m_szMetaTexts[ IMetaTable::C2_SWING_CHART];				// SwingChart
	m_szIndicatorNames[ IMetaTable::EDDC_OCX_KAGICHART]				= m_szMetaTexts[ IMetaTable::C2_KAGI_CHART];				// KagiChart
	m_szIndicatorNames[ IMetaTable::EDDC_OCX_RENKOCHART]			= m_szMetaTexts[ IMetaTable::C2_RENKO_CHART];				// RenkoChart
	m_szIndicatorNames[ IMetaTable::EDDC_DISPERSIONCHART]			= m_szMetaTexts[ IMetaTable::C2_DISPERSION];				// 분산Chart	// (2006.11.09 안승환) 분산차트 추가

	// 비정규 지표
	m_szIndicatorNames[ IMetaTable::EDDC_OCX_COMPARATIVECHART]		= m_szMetaTexts[ IMetaTable::C2_COMPARISON_CHART];		// 상대비교차트
	m_szIndicatorNames[ IMetaTable::EDDC_OCX_LOCKCHART]				= m_szMetaTexts[ IMetaTable::C3_LOCK];				// 락
	m_szIndicatorNames[ IMetaTable::EDDC_OCX_MARKETPROFILECHART]	= m_szMetaTexts[ IMetaTable::C3_MARKET_PROFILE];		// MarketProfile
	m_szIndicatorNames[ IMetaTable::EDDC_OCX_FUOPPANDFCHART]		= m_szMetaTexts[ IMetaTable::C2_FUTURE_OPTION_P_AND_F];			// FuOpP&F
	m_szIndicatorNames[ IMetaTable::EDDC_OCX_VOLUMESELLBUYCHART]	= m_szMetaTexts[ IMetaTable::C2_BUY_SELL_OVERHANGING_SUPPLY];			// 매수매도대기매물

	// 기타 시장지표
	m_szIndicatorNames[ IMetaTable::EDDC_VOLUMEPRICE]					= m_szMetaTexts[ IMetaTable::C2_VOLUME_AMOUNT];					// 거래대금
	m_szIndicatorNames[ IMetaTable::EDDC_MINIJISUCHART]					= m_szMetaTexts[ IMetaTable::C2_MINI_INDEX_CHART];				// 미니지수차트
	m_szIndicatorNames[ IMetaTable::EDDC_INDUSTRYJISUCHART]				= m_szMetaTexts[ IMetaTable::C2_INDUSTRY_INDEX];				// 업종지수
	m_szIndicatorNames[ IMetaTable::EDDC_MARKETJISUCHART]				= m_szMetaTexts[ IMetaTable::C2_MARKET_INDEX];					// 시장지수
	m_szIndicatorNames[ IMetaTable::EDDC_OCX_ACCOUNT_ITEMSIZE]			= m_szMetaTexts[ IMetaTable::C2_DEAL_LIST];						// 거래내역
	m_szIndicatorNames[ IMetaTable::EDDC_OCX_BIG_NEWS]					= m_szMetaTexts[ IMetaTable::C2_BIG_NEWS];						// Big News
	m_szIndicatorNames[ IMetaTable::EDDC_OCX_FOURIER_TRANSFORM]			= m_szMetaTexts[ IMetaTable::C2_FOURIER_TRANSFORM];				// Fourier Transform
	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_PUREBUY_ACCUMULATION]	= m_szMetaTexts[ IMetaTable::C2_ORGAN_PURE_BUY_ACCUMULATION];	// 기관순매수누적
	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_PUREBUY]					= m_szMetaTexts[ IMetaTable::C2_ORGAN_PURE_BUY];				// 기관순매수량
	m_szIndicatorNames[ IMetaTable::EDDC_SUSPENSE_CONTRACT]				= m_szMetaTexts[ IMetaTable::C2_SUSPENSE_CONTRACT];				// 미결제약정
	m_szIndicatorNames[ IMetaTable::EDDC_TOTAL_AMOUNT]					= m_szMetaTexts[ IMetaTable::C2_TOTAL_AMOUNT];					// 시가총액
	m_szIndicatorNames[ IMetaTable::EDDC_FOREIGN_HOLDING]				= m_szMetaTexts[ IMetaTable::C2_FOREIGN_HOLDING];				// 외국인 보유량
	m_szIndicatorNames[ IMetaTable::EDDC_FOREIGN_HOLDING_RATE]			= m_szMetaTexts[ IMetaTable::C2_FOREIGN_HOLDING_RATE];			// 외국인 보유율
	m_szIndicatorNames[ IMetaTable::EDDC_FOREIGN_PUREBUY]				= m_szMetaTexts[ IMetaTable::C2_FOREIGN_PURE_BUY];				// 외국인 순매수
	m_szIndicatorNames[ IMetaTable::EDDC_KOSPI_PER_INVESTER]			= m_szMetaTexts[ IMetaTable::C2_KOSPI_PER_INVESTOR];			// 투자자별거래소
	m_szIndicatorNames[ IMetaTable::EDDC_KOSDAQ_PER_INVESTER]			= m_szMetaTexts[ IMetaTable::C2_KOSDAQ_PER_INVESTOR];			// 투자자별코스닥
	m_szIndicatorNames[ IMetaTable::EDDC_K200_PER_INVESTER]				= m_szMetaTexts[ IMetaTable::C2_K200_PER_INVESTOR];				// 투자자별K200
	m_szIndicatorNames[ IMetaTable::EDDC_FUTURE_PER_INVESTER]			= m_szMetaTexts[ IMetaTable::C2_FUTURE_PER_INVESTOR];			// 투자자별선물
	m_szIndicatorNames[ IMetaTable::EDDC_CALL_OPTION_PER_INVESTER]		= m_szMetaTexts[ IMetaTable::C2_CALLOPTION_PER_INVESTOR];		// 투자자별콜옵션
	m_szIndicatorNames[ IMetaTable::EDDC_PUT_OPTION_PER_INVESTER]		= m_szMetaTexts[ IMetaTable::C2_PUTOPTION_PER_INVESTOR];		// 투자자별풋옵션
	m_szIndicatorNames[ IMetaTable::EDDC_PROGRAM_TRADE_PUREBUY_KOSPI]	= m_szMetaTexts[ IMetaTable::C2_PROGRAMTRADE_PUREBUY_KOSPI];	// 프로그램순매수 거래소
	m_szIndicatorNames[ IMetaTable::EDDC_PROGRAM_TRADE_PUREBUY_KOSDAQ]	= m_szMetaTexts[ IMetaTable::C2_PROGRAMTRADE_PUREBUY_KOSDAQ];	// 프로그램순매수 코스닥
	
	m_szIndicatorNames[ IMetaTable::EDDC_PERSON_PUREBUY]				= m_szMetaTexts[ IMetaTable::C2_PERSON_PUREBUY];				// 프로그램순매수 코스닥
	m_szIndicatorNames[ IMetaTable::EDDC_PERSON_PUREBUYACCUM]			= m_szMetaTexts[ IMetaTable::C2_PERSON_PUREBUYACCUM];			// 프로그램순매수 코스닥
	m_szIndicatorNames[ IMetaTable::EDDC_FOREIGN_PUREBUYACCM]			= m_szMetaTexts[ IMetaTable::C2_FOREIGN_PUREBUYACCM];			// 프로그램순매수 코스닥
	m_szIndicatorNames[ IMetaTable::EDDC_LINE_PRICE_MA]					= m_szMetaTexts[ IMetaTable::C2_LINE_PRICE_MA];					// 프로그램순매수 코스닥

	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_FINANCE_PUREBUYACCUM]	= m_szMetaTexts[ IMetaTable::C2_ORGAN_FINANCE_PUREBUYACCUM];	// 기관(금융투자) 순매수누적
	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_FINANCE_PUREBUY]			= m_szMetaTexts[ IMetaTable::C2_ORGAN_FINANCE_PUREBUY];			// 기관(금융투자) 순매수량
	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_INSURE_PUREBUYACCUM]		= m_szMetaTexts[ IMetaTable::C2_ORGAN_INSURE_PUREBUYACCUM];		// 기관(보험) 순매수누적
	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_INSURE_PUREBUY]			= m_szMetaTexts[ IMetaTable::C2_ORGAN_INSURE_PUREBUY];			// 기관(보험) 순매수량
	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_INVESTMENT_PUREBUYACCUM]	= m_szMetaTexts[ IMetaTable::C2_ORGAN_INVESTMENT_PUREBUYACCUM];	// 기관(투신) 순매수누적
	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_INVESTMENT_PUREBUY]		= m_szMetaTexts[ IMetaTable::C2_ORGAN_INVESTMENT_PUREBUY];		// 기관(투신) 순매수량
	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_PRIVATEEQUITY_PUREBUYACCUM]	= m_szMetaTexts[ IMetaTable::C2_ORGAN_PRIVATEEQUITY_PUREBUYACCUM];	// 기관(사모) 순매수누적
	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_PRIVATEEQUITY_PUREBUY]	= m_szMetaTexts[ IMetaTable::C2_ORGAN_PRIVATEEQUITY_PUREBUY];	// 기관(사모) 순매수량
	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_BANK_PUREBUYACCUM]		= m_szMetaTexts[ IMetaTable::C2_ORGAN_BANK_PUREBUYACCUM];		// 기관(은행) 순매수누적
	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_BANK_PUREBUY]			= m_szMetaTexts[ IMetaTable::C2_ORGAN_BANK_PUREBUY];			// 기관(은행) 순매수량
	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_ETCFINANCE_PUREBUYACCUM]	= m_szMetaTexts[ IMetaTable::C2_ORGAN_ETCFINANCE_PUREBUYACCUM];	// 기관(기타금융) 순매수누적
	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_ETCFINANCE_PUREBUY]	= m_szMetaTexts[ IMetaTable::C2_ORGAN_ETCFINANCE_PUREBUY];	// 기관(기타금융) 순매수량
	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_PENSIONFUND_PUREBUYACCUM]	= m_szMetaTexts[ IMetaTable::C2_ORGAN_PENSIONFUND_PUREBUYACCUM];	// 기관(연기금) 순매수누적
	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_PENSIONFUND_PUREBUY]		= m_szMetaTexts[ IMetaTable::C2_ORGAN_PENSIONFUND_PUREBUY];		// 기관(연기금) 순매수량
	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_COUNTRY_PUREBUYACCUM]	= m_szMetaTexts[ IMetaTable::C2_ORGAN_COUNTRY_PUREBUYACCUM];	// 기관(국가) 순매수누적
	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_COUNTRY_PUREBUY]			= m_szMetaTexts[ IMetaTable::C2_ORGAN_COUNTRY_PUREBUY];			// 기관(국가) 순매수량
	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_ETCCORP_PUREBUYACCUM]	= m_szMetaTexts[ IMetaTable::C2_ORGAN_ETCCORP_PUREBUYACCUM];	// 기관(기타법인) 순매수누적
	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_ETCCORP_PUREBUY]		= m_szMetaTexts[ IMetaTable::C2_ORGAN_ETCCORP_PUREBUY];		// 기관(기타법인) 순매수량
	
	m_szIndicatorNames[ IMetaTable::EDDC_ADL]		= m_szMetaTexts[ IMetaTable::C0_ADL];						// 업종시장지표 ADL
	m_szIndicatorNames[ IMetaTable::EDDC_ADR]		= m_szMetaTexts[ IMetaTable::C2_ADR];					// 업종시장지표 ADR

	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_CREDIT_BALANCE_RATE]			= m_szMetaTexts[ IMetaTable::C2_ORGAN__CREDIT_BALANCE_RATE];		// 신용잔고율
	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_CREDIT_GIVING_RATE]			= m_szMetaTexts[ IMetaTable::C2_ORGAN__CREDIT_GIVING_RATE];			// 신용공여율
	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_CREDIT_BALANCE_QTY]			= m_szMetaTexts[ IMetaTable::C2_ORGAN__CREDIT_BALANCE_QTY];			// 신용잔고수량
	m_szIndicatorNames[ IMetaTable::EDDC_ORGAN_CREDIT_BALANCE_VARIATION]	= m_szMetaTexts[ IMetaTable::C2_ORGAN__CREDIT_BALANCE_VARIATION];	// 신용잔고증감

	m_szIndicatorNames[ IMetaTable::EDDC_FOREIGN_ORGAN_PUREBUY]				= m_szMetaTexts[ IMetaTable::C2_FOREIGN_ORGAN_PUREBUY];				// 외국인+기관 순매수
	m_szIndicatorNames[ IMetaTable::EDDC_FOREIGN_ORGAN_PUREBUYACCUM]		= m_szMetaTexts[ IMetaTable::C2_FOREIGN_ORGAN_PUREBUYACCUM];		// 외국인+기관 순매수누적

	m_mapIndicatorID.InitHashTable( int( IMetaTable::EDDC_INDICATOR_COUNT * 1.5));
	for( int i = 0; i < IMetaTable::EDDC_INDICATOR_COUNT; i++) 
		m_mapIndicatorID.SetAt( m_szIndicatorNames[ i], i);
}
