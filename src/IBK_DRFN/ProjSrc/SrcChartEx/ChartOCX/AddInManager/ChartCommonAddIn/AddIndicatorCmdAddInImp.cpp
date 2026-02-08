// AddIndicatorCmdAddInImp.cpp: implementation of the CAddIndicatorCmdAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AddIndicatorCmdAddInImp.h"

#include "../../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_AddIn/_resource.h"							// for ID_CAA_...
#include "../Include_AddIn/I000000/_ICmdUI.h"					// for ICmdUI
#include "../Include_AddIn/I000000/_IChartOCX.h"				// for IChartOCX
#include "resource.h"											// for IDS_CANNOT_MULTIPLE_PRICECHART

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAddIndicatorCmdAddInImp::CAddIndicatorCmdAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
}

CAddIndicatorCmdAddInImp::~CAddIndicatorCmdAddInImp()
{

}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CAddIndicatorCmdAddInImp::m_strAddInItemName = "ADD_INDICATOR_CMD";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CAddIndicatorCmdAddInImp)
	ONADDINEVENT( OnCmdMsg)
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
BOOL CAddIndicatorCmdAddInImp::OnCmdMsg( UINT nID, int nCode, ICmdUI *p_pICmdUI)
{
	if( nID < ID_AIC_START_ID || ID_AIC_END_ID < nID) return FALSE;

	CString strIndicatorName;
	switch( nID)
	{
		// (2006/8/25 - Seung-Won, Bae) Check Popup Menu Root ID to enable chart menu with force for auto delete function.
		case ID_AIC_START_ID:				if( CN_UPDATE_COMMAND_UI == nCode && p_pICmdUI)
												p_pICmdUI->Enable( TRUE);
											return TRUE;

		// 캔들차트
		case ID_AIC_PRICE_CHART_CANDLE:			strIndicatorName = _MTEXT( C5_CANDLE_CHART);
												break;
		// 선차트
		case ID_AIC_PRICE_CHART_LINE:			strIndicatorName = _MTEXT( C5_LINE_CHART);
												break;
		// FLOW 차트
		case ID_AIC_PRICE_CHART_FLOW:			strIndicatorName = _MTEXT( C5_FLOW_CHART);
												break;
		// 바차트(시고저종)
		case ID_AIC_PRICE_CHART_BAR_OHLC:		strIndicatorName = _MTEXT( C5_BAR_CHART_OPEN_HIGH_LOW_CLOSE);
												break;
		// 바차트(고저종)
		case ID_AIC_PRICE_CHART_BAR_HLC:		strIndicatorName = _MTEXT( C5_BAR_CHART_HIGH_LOW_CLOSE);
												break;
		// 캔들볼륨차트
		case ID_AIC_PRICE_CHART_CANDLEVOLUME:	strIndicatorName = _MTEXT( C5_CANDLE_VOLUME_CHART);
												break;
		// 이큐볼륨차트
		case ID_AIC_PRICE_CHART_EQUIVOLUME:		strIndicatorName = _MTEXT( C5_EQUI_VOLUME_CHART);
												break;
		// 가격 이동평균
		case ID_AIC_PRICE_MA:				strIndicatorName = _MTEXT( C2_PRICE_MA);
											break;
		// 거래량 차트
		case ID_AIC_VOLUME_CHART:			strIndicatorName = _MTEXT( C2_VOLUME_CHART);
											break;
		// 거래량 이동평균
		case ID_AIC_VOLUME_MA:				strIndicatorName = _MTEXT( C2_VOLUME_MA);
											break;
		// 그물 차트
		case ID_AIC_RAINBOW:				strIndicatorName = _MTEXT( C2_RAINBOW_CHART);
											break;
		// 대기매물
		case ID_AIC_VOLUME_FOR_SALE:		strIndicatorName = _MTEXT( C2_OVERHANGING_SUPPLY);
											break;
		// 일목균형표
		case ID_AIC_GLANCE_BALANCE:			strIndicatorName = _MTEXT( C2_OBVIOUSLY_BALANCE_CHART);
											break;
		// Bollinger Bands
		case ID_AIC_BOLLINGER_BANDS:		strIndicatorName = _MTEXT( C2_BOLLINGER_BANDS);
											break;
		// Box Chart
		case ID_AIC_BOX_CHART:				strIndicatorName = _MTEXT( C2_BOX_CHART);
											break;
		// DEMA
		case ID_AIC_DEMA:					strIndicatorName = _MTEXT( C2_DEMA);
											break;
		// Demark
		case ID_AIC_DEMARK:					strIndicatorName = _MTEXT( C2_DEMARK);
											break;
		// Envelope
		case ID_AIC_ENVELOPE:				strIndicatorName = _MTEXT( C2_ENVELOPE);
											break;
		// MAC
		case ID_AIC_MAC:					strIndicatorName = _MTEXT( C2_MAC);
											break;
		// Parabolic SAR
		case ID_AIC_PARABOLIC_SAR:			strIndicatorName = _MTEXT( C2_PARABOLIC_SAR);
											break;
		// Pivot
		case ID_AIC_PIVOT:					strIndicatorName = _MTEXT( C2_PIVOT);
											break;
		// Price Channel
		case ID_AIC_PRICE_CHANNEL:			strIndicatorName = _MTEXT( C2_PRICE_CHANNEL);
											break;
		// Starc Bands
		case ID_AIC_STARC_BANDS:			strIndicatorName = _MTEXT( C2_STARC_BANDS);
											break;
		// TEMA
		case ID_AIC_TEMA:					strIndicatorName = _MTEXT( C2_TEMA);
											break;
		// Zig Zag
		case ID_AIC_ZIG_ZAG:				strIndicatorName = _MTEXT( C2_ZIG_ZAG);
											break;
		// 이격도(단순)
		case ID_AIC_DISPARITY_SMA:			strIndicatorName = _MTEXT( C2_DISPARITY_SMA);
											break;
		// 이격도(지수)
		case ID_AIC_DISPARITY_EMA:			strIndicatorName = _MTEXT( C2_DISPARITY_EMA);
											break;
		// AB Ratio
		case ID_AIC_AB_RATIO:				strIndicatorName = _MTEXT( C2_AB_RATIO);
											break;
		// ATR
		case ID_AIC_ATR:					strIndicatorName = _MTEXT( C2_ATR);
											break;
		// Band %B
		case ID_AIC_BAND_PERCENT_B:			strIndicatorName = _MTEXT( C2_BAND_PERCENT_B);
											break;
		// Band Width
		case ID_AIC_BAND_WIDTH:				strIndicatorName = _MTEXT( C2_BAND_WIDTH);
											break;
		// Chaikin's Volatility
		case ID_AIC_CHAIKINS_VOLATILITY:	strIndicatorName = _MTEXT( C2_CHAIKINS_VOLATILITY);
											break;
		// DPO
		case ID_AIC_DPO:					strIndicatorName = _MTEXT( C2_DPO);
											break;
		// DRF
		case ID_AIC_DRF:					strIndicatorName = _MTEXT( C2_DRF);
											break;
		// Elder Ray Bear
		case ID_AIC_ELDER_RAY_BEAR:			strIndicatorName = _MTEXT( C2_ELDER_RAY_BEAR);
											break;
		// Elder Ray Bull
		case ID_AIC_ELDER_RAY_BULL:			strIndicatorName = _MTEXT( C2_ELDER_RAY_BULL);
											break;
		// Mass Index
		case ID_AIC_MASS_INDEX:				strIndicatorName = _MTEXT( C2_MASS_INDEX);
											break;
		// NCO
		case ID_AIC_NCO:					strIndicatorName = _MTEXT( C2_NCO);
											break;
		// QStic
		case ID_AIC_QSTIC:					strIndicatorName = _MTEXT( C2_QSTIC);
											break;
		// RMI
		case ID_AIC_RMI:					strIndicatorName = _MTEXT( C2_RMI);
											break;
		// RSI
		case ID_AIC_RSI:					strIndicatorName = _MTEXT( C2_RSI);
											break;
		// Sigma
		case ID_AIC_SIGMA:					strIndicatorName = _MTEXT( C2_SIGMA);
											break;
		// Standard Deviation
		case ID_AIC_STANDARD_DEVIATION:		strIndicatorName = _MTEXT( C2_STANDARD_DEVIATION);
											break;
		// Standard Error
		case ID_AIC_STANDARD_ERROR:			strIndicatorName = _MTEXT( C2_STANDARD_ERROR);
											break;
		// Stochastic Fast
		case ID_AIC_STOCHASTIC_FAST:		strIndicatorName = _MTEXT( C2_STOCHASTIC_FAST);
											break;
		// Stochastic Fast2
		case ID_AIC_STOCHASTIC_FAST2:		strIndicatorName = _MTEXT( C2_STOCHASTIC_FAST2);
											break;
		// Stochastic Fast3
		case ID_AIC_STOCHASTIC_FAST3:		strIndicatorName = _MTEXT( C2_STOCHASTIC_FAST3);
											break;
		// Stochastic Slow
		case ID_AIC_STOCHASTIC_SLOW:		strIndicatorName = _MTEXT( C2_STOCHASTIC_SLOW);
											break;
		// Stochastic Slow2
		case ID_AIC_STOCHASTIC_SLOW2:		strIndicatorName = _MTEXT( C2_STOCHASTIC_SLOW2);
											break;
		// Stochastic Slow3
		case ID_AIC_STOCHASTIC_SLOW3:		strIndicatorName = _MTEXT( C2_STOCHASTIC_SLOW3);
											break;
		// %R
		case ID_AIC_PERCENT_R:				strIndicatorName = _MTEXT( C2_PERCENT_R);
											break;
		// AD Line
		case ID_AIC_AD_LINE:				strIndicatorName = _MTEXT( C2_AD_LINE);
											break;
		// ADX
		case ID_AIC_ADX:					strIndicatorName = _MTEXT( C2_ADX);
											break;
		// ADXR
		case ID_AIC_ADXR:					strIndicatorName = _MTEXT( C2_ADXR);
											break;
		// Aroon
		case ID_AIC_AROON:					strIndicatorName = _MTEXT( C2_AROON);
											break;
		// CCI
		case ID_AIC_CCI:					strIndicatorName = _MTEXT( C2_CCI);
											break;
		// Formula
		case ID_AIC_FORMULA:				strIndicatorName = _MTEXT( C2_FORMULA);
											break;
		// LRS
		case ID_AIC_LRS:					strIndicatorName = _MTEXT( C2_LRS);
											break;
		// MACD
		case ID_AIC_MACD:					strIndicatorName = _MTEXT( C2_MACD);
											break;
		// MACD Oscillator
		case ID_AIC_MACD_OSCILLATOR:		strIndicatorName = _MTEXT( C2_MACD_OSCILLATOR);
											break;
		// MAO
		case ID_AIC_MAO:					strIndicatorName = _MTEXT( C2_MAO);
											break;
		// Momentum
		case ID_AIC_MOMENTUM:				strIndicatorName = _MTEXT( C2_MOMENTUM);
											break;
		// NDI
		case ID_AIC_NDI:					strIndicatorName = _MTEXT( C2_NDI);
											break;
		// Price Oscillator
		case ID_AIC_PRICE_OSCILLATOR:		strIndicatorName = _MTEXT( C2_PRICE_OSCILLATOR);
											break;
		// ROC
		case ID_AIC_ROC:					strIndicatorName = _MTEXT( C2_ROC);
											break;
		// Sonar
		case ID_AIC_SONAR:					strIndicatorName = _MTEXT( C2_SONAR);
											break;
		// SROC
		case ID_AIC_SROC:					strIndicatorName = _MTEXT( C2_SROC);
											break;
		// TRIX
		case ID_AIC_TRIX:					strIndicatorName = _MTEXT( C2_TRIX);
											break;
		// VHF
		case ID_AIC_VHF:					strIndicatorName = _MTEXT( C2_VHF);
											break;
		// CMF
		case ID_AIC_CMF:					strIndicatorName = _MTEXT( C2_CMF);
											break;
		// Chaikin's Oscillator
		case ID_AIC_CHAIKINS_OSCILLATOR:	strIndicatorName = _MTEXT( C2_CHAIKINS_OSCILLATOR);
											break;
		// EOM
		case ID_AIC_EOM:					strIndicatorName = _MTEXT( C2_EOM);
											break;
		// Force Index
		case ID_AIC_FORCE_INDEX:			strIndicatorName = _MTEXT( C2_FORCE_INDEX);
											break;
		// MFI
		case ID_AIC_MFI:					strIndicatorName = _MTEXT( C2_MFI);
											break;
		// NVI
		case ID_AIC_NVI:					strIndicatorName = _MTEXT( C2_NVI);
											break;
		// OBV
		case ID_AIC_OBV:					strIndicatorName = _MTEXT( C2_OBV);
											break;
		// PVI
		case ID_AIC_PVI:					strIndicatorName = _MTEXT( C2_PVI);
											break;
		// VA Oscillator
		case ID_AIC_VA_OSCILLATOR:			strIndicatorName = _MTEXT( C2_VA_OSCILLATOR);
											break;
		// Volume Oscillator
		case ID_AIC_VOLUME_OSCILLATOR:		strIndicatorName = _MTEXT( C2_VOLUME_OSCILLATOR);
											break;
		// Volume Ratio
		case ID_AIC_VOLUME_RATIO:			strIndicatorName = _MTEXT( C2_VOLUME_RATIO);
											break;
		// VROC
		case ID_AIC_VROC:					strIndicatorName = _MTEXT( C2_VROC);
											break;
		// 심리도
		case ID_AIC_PSYCHOLOGICAL:			strIndicatorName = _MTEXT( C2_PSYCHOLOGICAL_LINE);
											break;
		// 신심리도
		case ID_AIC_NEW_PSYCHOLOGICAL:		strIndicatorName = _MTEXT( C2_NEW_PSYCHOLOGY_LINE);
											break;
		// 7 Binary Wave
		case ID_AIC_7_BINARY_WAVE:			strIndicatorName = _MTEXT( C2_7_BINARY_WAVE);
											break;
		// Binary Wave
		case ID_AIC_BINARY_WAVE:			strIndicatorName = _MTEXT( C2_BINARY_WAVE);
											break;
		// PVT
		case ID_AIC_PVT:					strIndicatorName = _MTEXT( C2_PVT);
											break;
		// LRS + Stochastic
		case ID_AIC_LRS_STOCHASTIC:			strIndicatorName = _MTEXT( C2_LRS_STOCHASTIC);
											break;
		// MACD + Stochastic
		case ID_AIC_MACD_STOCHASTIC:		strIndicatorName = _MTEXT( C2_MACD_STOCHASTIC);
											break;
		// Momentum + Stochastic
		case ID_AIC_MOMENTUM_STOCHASTIC:	strIndicatorName = _MTEXT( C2_MOMENTUM_STOCHASTIC);
											break;
		// OBV + Momentum
		case ID_AIC_OBV_MOMENTUM:			strIndicatorName = _MTEXT( C2_OBV_MOMENTUM);
											break;
		// OBV + Stochastic
		case ID_AIC_OBV_STOCHASTIC:			strIndicatorName = _MTEXT( C2_OBV_STOCHASTIC);
											break;
		// ROC + Stochastic
		case ID_AIC_ROC_STOCHASTIC:			strIndicatorName = _MTEXT( C2_ROC_STOCHASTIC);
											break;
		// RSI + MACD
		case ID_AIC_RSI_MACD:				strIndicatorName = _MTEXT( C2_RSI_MACD);
											break;
		// RSI + Stochastic
		case ID_AIC_RSI_STOCHASTIC:			strIndicatorName = _MTEXT( C2_RSI_STOCHASTIC);
											break;
		// Sonar + 심리도
		case ID_AIC_SONAR_PSYCHOLOGICAL:	strIndicatorName = _MTEXT( C2_SONAR_PSYCHOLOGY);
											break;
		// Stochastic + RSI
		case ID_AIC_STOCHASTIC_RSI:			strIndicatorName = _MTEXT( C2_STOCHASTIC_RSI);
											break;
		// %B + Stochastic
		case ID_AIC_PERCENT_B_STOCHASTIC:	strIndicatorName = _MTEXT( C2_PERCENT_B_STOCHASTIC);
											break;
		// 삼선전환도
		case ID_AIC_THREE_LINE_BREAK:		strIndicatorName = _MTEXT( C2_THREE_LINE_BREAK);
											break;
		// 역시계곡선
		case ID_AIC_INVERSE_LINE:			strIndicatorName = _MTEXT( C2_INVERSE_TIME_LINE);
											break;
		// P&&F
		case ID_AIC_P_AND_F:				strIndicatorName = _MTEXT( C2_P_AND_F);
											break;
		// Swing Chart
		case ID_AIC_SWING:					strIndicatorName = _MTEXT( C2_SWING_CHART);
											break;
		// Kagi Chart
		case ID_AIC_KAGI:					strIndicatorName = _MTEXT( C2_KAGI_CHART);
											break;
		// Renko Chart
		case ID_AIC_RENKO:					strIndicatorName = _MTEXT( C2_RENKO_CHART);
											break;
		// 분산차트
		case ID_AIC_DISPERSION:				strIndicatorName = _MTEXT( C2_DISPERSION);
											break;
		// 거래대금
		case ID_AIC_VOLUMEPRICE:			strIndicatorName = _MTEXT( C2_VOLUME_AMOUNT);
											break;
		// TSF
		case ID_AIC_TSF:					strIndicatorName = _MTEXT( C2_TSF);
											break;
		// DMI
		case ID_AIC_DMI:					strIndicatorName = _MTEXT( C2_DMI);
											break;
		// 이격도
		case ID_AIC_DISPARITY:				strIndicatorName = _MTEXT( C2_DISPARITY);
											break;
		// VR
		case ID_AIC_VR:						strIndicatorName = _MTEXT( C2_VR);
											break;

		// (2009/6/19 - Seung-Won, Bae) Add Market Indicator.
		// 기관순매수누적
		case ID_AIC_ORGAN_PURE_BUY_ACCUMULATION:	strIndicatorName = _MTEXT( C2_ORGAN_PURE_BUY_ACCUMULATION);
													break;
		// 기관순매수량
		case ID_AIC_ORGAN_PURE_BUY:					strIndicatorName = _MTEXT( C2_ORGAN_PURE_BUY);
													break;
		// 미결제약정
		case ID_AIC_OPEN_INTEREST:					strIndicatorName = _MTEXT( C2_SUSPENSE_CONTRACT);
													break;
		// 시가총액
		case ID_AIC_MARKET_VALUE:					strIndicatorName = _MTEXT( C2_TOTAL_AMOUNT);
													break;
		// 외국인 보유량
		case ID_AIC_FOREIGNERS_HOLDING:				strIndicatorName = _MTEXT( C2_FOREIGN_HOLDING);
													break;
		// 외국인 보유율
		case ID_AIC_FOREIGNERS_HOLDING_PERCENT:		strIndicatorName = _MTEXT( C2_FOREIGN_HOLDING_RATE);
													break;
		// 외국인 순매수
		case ID_AIC_FOREIGNERS_NET_PURCHASE:		strIndicatorName = _MTEXT( C2_FOREIGN_PURE_BUY);
													break;
		// 투자자별거래소
		case ID_AIC_KOSPI_PER_INVESTOR:				strIndicatorName = _MTEXT( C2_KOSPI_PER_INVESTOR);
													break;
		// 투자자별코스닥
		case ID_AIC_KOSDAQ_PER_INVESTOR:			strIndicatorName = _MTEXT( C2_KOSDAQ_PER_INVESTOR);
													break;
		// 투자자별K200
		case ID_AIC_K200_PER_INVESTOR:				strIndicatorName = _MTEXT( C2_K200_PER_INVESTOR);
													break;
		// 투자자별선물
		case ID_AIC_FUTURE_PER_INVESTOR:			strIndicatorName = _MTEXT( C2_FUTURE_PER_INVESTOR);
													break;
		// 투자자별콜옵션
		case ID_AIC_CALLOPTION_PER_INVESTOR:		strIndicatorName = _MTEXT( C2_CALLOPTION_PER_INVESTOR);
													break;
		// 투자자별풋옵션
		case ID_AIC_PUTOPTION_PER_INVESTOR:			strIndicatorName = _MTEXT( C2_PUTOPTION_PER_INVESTOR);
													break;
		// 프로그램순매수 거래소
		case ID_AIC_PROGRAMTRADE_PUREBUY_KOSPI:		strIndicatorName = _MTEXT( C2_PROGRAMTRADE_PUREBUY_KOSPI);
													break;
		// 프로그램순매수 코스닥
		case ID_AIC_PROGRAMTRADE_PUREBUY_KOSDAQ:	strIndicatorName = _MTEXT( C2_PROGRAMTRADE_PUREBUY_KOSDAQ);
													break;
		default: return FALSE;
	}

	if( CN_UPDATE_COMMAND_UI == nCode)
	{
		if( !p_pICmdUI) return FALSE;
		p_pICmdUI->Enable( TRUE);
		if( !m_pIChartOCX->IsMultipleSameNameIndicator())
		{
			BOOL bExist = m_pIChartOCX->HasGraph( strIndicatorName);
			if( !bExist) if( m_pIChartOCX->IsOnSpecialChart()) bExist = m_pIChartOCX->HasGraphInSpecialBack( strIndicatorName);
			p_pICmdUI->SetCheck( bExist);
		}
		return TRUE;
	}

	if( CN_COMMAND == nCode)
	{
		ILPCSTR szGraphName = m_pIChartOCX->AddChartBlock( strIndicatorName);
		return TRUE;
	}

	return TRUE;
}
