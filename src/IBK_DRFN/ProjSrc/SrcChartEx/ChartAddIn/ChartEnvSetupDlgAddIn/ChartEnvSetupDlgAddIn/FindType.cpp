// FindType.cpp: implementation of the CFindType class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "FindType.h"

#include "ListCtrlData.h"
// 2008.01.25 by LYH >>
//#include "DefineData.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for IMetaTable
// 2008.01.25 by LYH <<

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseData::COLORLISTITEMTYPE CFindType::FindColorItemText( const char *szGraphName)
{
	CString GraphName = FindStochastics( szGraphName);
	for( int i = 0; i < GRAPH_RAINBOW; i++)
	{
// 2008.01.25 by LYH >>
		if(GraphName.CompareNoCase( _MTEXTN( g_szIndicatorNameList[i])) == 0)
// 2008.01.25 by LYH <<
		{
			switch(i)
			{
				case GRAPH_PRICE: //가격
					return CBaseData::PRICE;
		
				case GRAPH_VOLUME: //거래량
					return CBaseData::VOLUME;
				
				case GRAPH_P_AND_F: //상승, 하락
				case GRAPH_THREE_LINE_BREAK:
				case GRAPH_PARABOLIC: 
				case GRAPH_BOXCHART:
					return CBaseData::UP_DOWN;
				
				case GRAPH_HORIZONTALBAR:  //대기매물
					return CBaseData::DAEGIMEMUL;
				case GRAPH_INVERSE_LINE: //지표
					return CBaseData::INVERSELINE;
				
				case GRAPH_MACD:
					return CBaseData::MACD;
				
				case GRAPH_PIVOT:
					return CBaseData::PIVOT;
				
				case GRAPH_BOLLINGER:
				case GRAPH_ENVELOPE: 
				case GRAPH_MAC:
				case GRAPH_MFI:
				case GRAPH_MOMENTUM:
				case GRAPH_MI: 
				case GRAPH_OBV:
				//case GRAPH_NEGATIVE_VOLUME_INDEX:
				//case GRAPH_POSITIVE_VOLUME_INDEX:
				case GRAPH_OCMA:
				case GRAPH_RSI:
//				case GRAPH_PSYCH:		// 종찬(04/10/04) 선형;채움식; 수정
//				case GRAPH_NEWPSYCH:	// 종찬(04/10/04) 선형;채움식; 수정
//				case GRAPH_CCI:			// 종찬(04/10/04) 선형;채움식; 수정
				case GRAPH_EOM:
				case GRAPH_SONAR:
				case GRAPH_WILLIAM:
				case GRAPH_OSCP:
				case GRAPH_VR:
				case GRAPH_STOCHASTICS_FAST:
				case GRAPH_STOCHASTICS_SLOW:
				case GRAPH_TRIX:	
//
				case GRAPH_ATR:
				case GRAPH_ROC:
				case GRAPH_VOLUME_ROC:
				case GRAPH_AB_RATIO: 
				case GRAPH_RELATIVE_STRENGTH_COMPARATIVE:
				case GRAPH_FOURIER_TRANSFORM: 
				case GRAPH_PRICE_VOLUME_TREND:
				case GRAPH_DMI:
				case GRAPH_EMV:					/// Ease Of Movement
				case GRAPH_ADX:
				case GRAPH_WEIGHTCLOSE:
				case GRAPH_ChaikinsVolatility:
				case GRAPH_BANDB:
				case GRAPH_BandWidth:
				case GRAPH_BinaryWave:
				case GRAPH_Demark:
				case GRAPH_WilliamAD:
				case GRAPH_SMI:
				case GRAPH_NCO:
				case GRAPH_Sigma:
				case GRAPH_TSF:
				case GRAPH_LRS:
				case GRAPH_CLC:
				case GRAPH_TRIN:
				case GRAPH_CMF:
				case GRAPH_IMI:
				case GRAPH_STOCHASTICS_OSCILLATOR:
				case GRAPH_AD:	/// A/D Line : Accumulation/Distribution
				case GRAPH_Energy:
				case GRAPH_NEGATIVE_VOLUME_INDEX: // NVI
				case GRAPH_POSITIVE_VOLUME_INDEX: // PVI
					return CBaseData::SUBGRAPHNAME;

				//case GRAPH_AD:					/// A/D Line : Accumulation/Distribution
				case GRAPH_ZIGZAG:				/// Zig Zag
				case GRAPH_DPO:					/// Detreded Price Oscillator
				case GRAPH_ABI:					/// Absolute Breadth Index
				case GRAPH_ADL:					/// Advanced Decline Line
				case GRAPH_ADR:					/// Advanced Decline Ratio
				case GRAPH_BREADTH_THRUST:		/// Breadth Thrust
				case GRAPH_MCCLELLAN_OSCILLATOR:/// McClellan Oscillator
				case GRAPH_STANDARD_DEVIATION:	/// Standard Deviation
				case GRAPH_STIX:				/// STIX
				case GRAPH_VHF:					/// Vertical horizontal Filter
				case GRAPH_CO:
					return CBaseData::COLOR;

				//case GRAPH_CO:
				case GRAPH_MACD_OSC:
				case GRAPH_OSCV:
				case GRAPH_MaOscillator: 
					return CBaseData::SUB_UPFILL_DOWNFILL;  
				case GRAPH_RAINBOW: 
					return CBaseData::RAINBOW; //시작색상, 종료색상
				case GRAPH_GLANCEBALANCE:  //일목균형
						return CBaseData::GLANCEBLANCE;
			}
		}
	}

	//지표설정중 <거래대금>을 <거래량차트>과 같이 하기위해
	//SK에서 만 사용하기때문에 예외처리함.
	if( !strcmp( szGraphName, _MTEXT( C2_VOLUME_AMOUNT ))) return CBaseData::VOLUME;

	return CBaseData::NONE;
}

CBaseData::TABITEMTYPE CFindType::FindTabItemText( const char *p_szIndicatorName)
{
	CString GraphName = FindStochastics( p_szIndicatorName);
	for(int i = 0; i < GRAPH_RAINBOW; i++) {
// 2008.01.25 by LYH >>
		//if(GraphName.CompareNoCase( g_szIndicatorNameList[ i]) == 0) {
		if(GraphName.CompareNoCase( _MTEXTN( g_szIndicatorNameList[i]) ) == 0) {
// 2008.01.25 by LYH <<
			switch(i){//설정
				case GRAPH_PRICE: //가격 
				case GRAPH_VOLUME: //거래량
				
				case GRAPH_P_AND_F:
				case GRAPH_RENKO_CHART:
// (2006.11.09 안승환) 분산차트 추가
				case GRAPH_DISPERSION_CHART:
// (2006.11.09 안승환) 분산차트 추가

				case GRAPH_THREE_LINE_BREAK:
				case GRAPH_PRICE_MA: //이동평균
				case GRAPH_VOLUME_MA:
				case GRAPH_ENVELOPE: 
				case GRAPH_PARABOLIC: 
				case GRAPH_INVERSE_LINE: 
				case GRAPH_BOLLINGER:
				case GRAPH_RAINBOW:
				case GRAPH_ATR:
				case GRAPH_GLANCEBALANCE: 
				case GRAPH_MACD_OSC:
				case GRAPH_NEGATIVE_VOLUME_INDEX:
				case GRAPH_POSITIVE_VOLUME_INDEX:
				case GRAPH_PIVOT:
				case GRAPH_PRICE_VOLUME_TREND:
				case GRAPH_VOLUME_ROC:
				case GRAPH_ABI:					/// Absolute Breadth Index
				case GRAPH_AD:					/// A/D Line : Accumulation/Distribution
				case GRAPH_STANDARD_DEVIATION:	/// Standard Deviation
				case GRAPH_VHF:					/// Vertical horizontal Filter
				case GRAPH_ZIGZAG:				/// Zig Zag
				case GRAPH_BOXCHART:
				case GRAPH_EMV:					/// Ease Of Movement
				case GRAPH_MAC:
				//case GRAPH_SONAR:
				//case GRAPH_TRIX:
				case GRAPH_OCMA:
				case GRAPH_CO: // CO : Chaikin's Oscillator -> baseLine 이 있지만 셋팅할 필요는 없다.
				case GRAPH_OBV:
				case GRAPH_OSCP:
				case GRAPH_OSCV:
				case GRAPH_RELATIVE_STRENGTH_COMPARATIVE:
				case GRAPH_FOURIER_TRANSFORM: 
				case GRAPH_ADL:					/// Advanced Decline Line
				//case GRAPH_DPO:					/// Detreded Price Oscillator
//교보 추가
				case GRAPH_ADX:
				case GRAPH_WEIGHTCLOSE:
				case GRAPH_ChaikinsVolatility:
				case GRAPH_BANDB:
				case GRAPH_BandWidth:
				case GRAPH_BinaryWave:
				case GRAPH_EOM:
				case GRAPH_Demark:
				case GRAPH_WilliamAD:
				case GRAPH_SMI:
				case GRAPH_NCO:
				case GRAPH_Sigma:
				case GRAPH_MaOscillator:
				case GRAPH_TSF:
				case GRAPH_LRS:
				case GRAPH_CLC:
				case GRAPH_TRIN:
				case GRAPH_CMF:
				case GRAPH_IMI:
				case GRAPH_STOCHASTICS_OSCILLATOR:
				//sy 2004.05.12.
				//-> 기존엔 특수차트이 였으나 <가격차트>에 첨부되는 차트로 수정함.
				case GRAPH_HORIZONTALBAR:
					return CBaseData::CONFIGURATION;
				
				//case GRAPH_HORIZONTALBAR: //대기매물
				//	return CBaseData::DAEGI;
				//sy end
				case GRAPH_CCI://설정, 수평선
				case GRAPH_DISPARITY: 
				case GRAPH_PSYCH: 
				case GRAPH_NEWPSYCH:
				case GRAPH_DMI:
				case GRAPH_MFI:
				case GRAPH_MOMENTUM:
				case GRAPH_MI: 
				case GRAPH_RSI:
				case GRAPH_WILLIAM:
				case GRAPH_VR:
				case GRAPH_STOCHASTICS_FAST:
				case GRAPH_STOCHASTICS_SLOW:
				case GRAPH_ROC:
				case GRAPH_AB_RATIO: 
				case GRAPH_ADR:					/// Advanced Decline Ratio
				case GRAPH_BREADTH_THRUST:		/// Breadth Thrust
				case GRAPH_MCCLELLAN_OSCILLATOR:	/// McClellan Oscillator
				case GRAPH_STIX:					/// STIX
				case GRAPH_MACD:
				//sy 2004.2.10
				// 종찬(04/09/22) DPO와 TSI는 매매신호보기가 적용안됨.
//				case GRAPH_DPO:					/// Detreded Price Oscillator
				case GRAPH_Energy:
				case GRAPH_SONAR:
				case GRAPH_TRIX:
//				case GRAPH_TSI:
				// 04/09/22
					return CBaseData::CONFIGU_HORIZONLINE;
			}
		}
	}
	return CBaseData::CONFIGURATION;
}



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 12
// Return CBaseData::DLGTYPE : 
// Param  const CString& strGraphName : 
// Comments		: 지표값에 따라서 해당되는 Dialog Type을 결정한다.
//-----------------------------------------------------------------------------
CBaseData::DLGTYPE CFindType::FindDlgType( const char *p_szIndicatorName)
{
	CString GraphName = FindStochastics( p_szIndicatorName);
	
	for( int i = 0 ; i < GRAPH_COUNT ; i++ )
	{
// 2008.01.25 by LYH >>
		//if( GraphName.CompareNoCase( g_szIndicatorNameList[i] ) == 0)
		if( GraphName.CompareNoCase( _MTEXTN( g_szIndicatorNameList[i]) ) == 0)
// 2008.01.25 by LYH <<
		{
			switch( i )
			{
				case GRAPH_PRICE: //price 
					return CBaseData::Price;
				
				case GRAPH_VOLUME: //volume
					return CBaseData::Volume;
				
				case GRAPH_MAC:
					return CBaseData::SeveralSub_OneCondition;
					
				case GRAPH_THREE_LINE_BREAK: //Sub 1, No-Condition, fill
				case GRAPH_P_AND_F:
					return CBaseData::DT_SPECIAL_TYPE_A;

				case GRAPH_SwingChart:
				case GRAPH_KagiChart:
					return CBaseData::DT_SPECIAL_TYPE_B;

				case GRAPH_RENKO_CHART:
// (2006.11.09 안승환) 분산차트 추가
				case GRAPH_DISPERSION_CHART:
// (2006.11.09 안승환) 분산차트 추가
					return CBaseData::DT_SPECIAL_TYPE_C;

				case GRAPH_INVERSE_LINE: 
					return CBaseData::DT_SPECIAL_TYPE_D;
					
				// Sang-Woo, Cho		2006 / 04 / 19

				// "대기매물" 지표설정을 별도로 분리함.
				// Begin
				case GRAPH_HORIZONTALBAR:
				case GRAPH_SELLBYHORIZONTALBAR:
					return CBaseData::DT_SPECIAL_TYPE_E;
				// End

				//[가격이동평균] 설정창 변경
				case GRAPH_PRICE_MA: //이동평균
					return CBaseData::PriceMA;
				
				case GRAPH_VOLUME_MA:
					return CBaseData::MovingAverage;
					
				// Sang-Woo, Cho		2005 / 11 / 10
				// 일목균형표 지표 UI 분리
				// Begin
				case GRAPH_GLANCEBALANCE:
					return CBaseData::GLANCEBALANCE;
				// End

				// Sang-Woo, Cho		2006 / 05 / 02
				// BinaryWave 설정창 분리
				case GRAPH_BinaryWave:
					return CBaseData::DT_COMMON_TYPE_B;

				default:
					return CBaseData::DT_COMMON_TYPE_A;
			}
		}
	}

	//지표설정중 <거래대금>을 <거래량차트>과 같이 하기위해
	//SK에서 만 사용하기때문에 예외처리함.
	if( !strcmp( p_szIndicatorName, _MTEXT( C2_VOLUME_AMOUNT ))) return CBaseData::Volume;

	return CBaseData::OneSub_NoCondition;
}


CBaseData::FILLTYPE CFindType::FindFillTypeWhenOneSub_ThreeCondi(const CString& strGraphName)
{
	CString GraphName = strGraphName;
	GraphName = FindStochastics(strGraphName);
	
	for( int i = 0 ; i < GRAPH_COUNT ; i++ )
	{
// 2008.01.25 by LYH >>
		//if( GraphName.CompareNoCase( g_szIndicatorNameList[i] ) == 0 )
		if( GraphName.CompareNoCase( _MTEXTN( g_szIndicatorNameList[i]) ) == 0 )
// 2008.01.25 by LYH <<
		{
			switch( i ) 
			{
				case GRAPH_MACD_OSC: //Sub1, Condition 3, fill
				case GRAPH_OSCV: 
					return CBaseData::Fill;
			}
		}
	}
	
	return CBaseData::NoFill;
}


CBaseData::FILLTYPE CFindType::FindFillTypeWhenOneSub_NoCondi( const CString& strGraphName )
{
	CString GraphName = strGraphName;
	GraphName = FindStochastics(strGraphName);
	
	for( int i = 0 ; i < GRAPH_COUNT ; i++)
	{
// 2008.01.25 by LYH >>
		//if( GraphName.CompareNoCase(g_szIndicatorNameList[i] ) == 0 )
		if( GraphName.CompareNoCase( _MTEXTN( g_szIndicatorNameList[i]) ) == 0 )
// 2008.01.25 by LYH <<
		{
			switch(i)
			{
				case GRAPH_THREE_LINE_BREAK: // Sub 1, No Condi, fill
					return CBaseData::Fill;	
				
				//case GRAPH_NEGATIVE_VOLUME_INDEX: // Sub 1, No-Condition
				//case GRAPH_POSITIVE_VOLUME_INDEX: // PVI
				case GRAPH_FOURIER_TRANSFORM:
				case GRAPH_PRICE_VOLUME_TREND:	
				case GRAPH_ABI:					/// Absolute Breadth Index
				case GRAPH_AD:					/// A/D Line : Accumulation/Distribution
				case GRAPH_ADL:					/// Advanced Decline Line
				case GRAPH_WEIGHTCLOSE:
				case GRAPH_WilliamAD:
					return CBaseData::NoFill;
			}
		}
	}
	
	return CBaseData::NoFill;
}


CBaseData::FILLTYPE CFindType::FindFillTypeWhenOneSub_OneCondi(const CString& strGraphName)
{
	CString GraphName = strGraphName;
	GraphName = FindStochastics(strGraphName);
	for(int i = 0; i < GRAPH_COUNT; i++) {
// 2008.01.25 by LYH >>
		//if(GraphName.CompareNoCase(g_szIndicatorNameList[i]) == 0) {
		if(GraphName.CompareNoCase( _MTEXTN( g_szIndicatorNameList[i]) ) == 0) {
// 2008.01.25 by LYH <<
			switch(i){
				case GRAPH_CCI: //fill
				case GRAPH_PSYCH: // Sub 1, Condition 1, fill
				case GRAPH_NEWPSYCH:
				case GRAPH_ATR:// Sub 1, Condition 1, nofill
				case GRAPH_MOMENTUM:
				case GRAPH_VR:
				case GRAPH_VOLUME_ROC:
				case GRAPH_P_AND_F:
				case GRAPH_INVERSE_LINE: 
				case GRAPH_BOXCHART:
				case GRAPH_ADR:					/// Advanced Decline Ratio
				case GRAPH_VHF:					/// Vertical horizontal Filter
				case GRAPH_ChaikinsVolatility:
				case GRAPH_Sigma:
				case GRAPH_LRS:
				case GRAPH_TRIN:
				case GRAPH_CMF:
				case GRAPH_IMI:
				case GRAPH_EOM:
					return CBaseData::NoFill;
			}
		}
	}
	return CBaseData::NoFill;
}


CBaseData::FILLTYPE CFindType::FindFillTypeWhenOneSub_TwoCondi(const CString& strGraphName)
{
	CString GraphName = strGraphName;
	GraphName = FindStochastics(strGraphName);
	
	for( int i = 0 ; i < GRAPH_COUNT ; i++ )
	{
// 2008.01.25 by LYH >>
		//if( GraphName.CompareNoCase( g_szIndicatorNameList[i] ) == 0 )
		if( GraphName.CompareNoCase( _MTEXTN( g_szIndicatorNameList[i]) ) == 0 )
// 2008.01.25 by LYH <<
		{
			switch(i)
			{
				//sy 2004.2.10
				//case GRAPH_CO: //Sub 1, Condition 2, fill
				case GRAPH_STOCHASTICS_OSCILLATOR: //Sub 1, Condition 2, fill
					return CBaseData::Fill;
	
				case GRAPH_MI: // Sub 1, Condition 2, nofill
				case GRAPH_PARABOLIC:
				case GRAPH_OSCP:
				case GRAPH_RAINBOW:
				case GRAPH_STIX:
				case GRAPH_BANDB:
				case GRAPH_BandWidth:
					return CBaseData::NoFill;
			}
		}
	}
	return CBaseData::NoFill;
}
		 

CBaseData::DRAWPOSITIONTYPE CFindType::FindDrawPositionType(const CString &strGraphName)
{
	CString GraphName = strGraphName;
	GraphName = FindStochastics(strGraphName);

	for( int i = 0 ; i < GRAPH_COUNT ; i++ )
	{
// 2008.01.25 by LYH >>
		//if( GraphName.CompareNoCase(g_szIndicatorNameList[i]) == 0 )
		if( GraphName.CompareNoCase( _MTEXTN( g_szIndicatorNameList[i]) ) == 0 )
// 2008.01.25 by LYH <<
		{
			switch( i )
			{
				case GRAPH_PARABOLIC: 
				case GRAPH_RAINBOW:
				case GRAPH_GLANCEBALANCE:
				case GRAPH_BOLLINGER:
				case GRAPH_PIVOT:
				case GRAPH_ENVELOPE: 
				case GRAPH_CLC:
					return CBaseData::DrawWithPrice;
			}
		}
	}
	
	return CBaseData::NotDrawWithPrice;
}

CBaseData::LIMITHORIZONTALLINE CFindType::FindLimitValueofHorizontalLine( const char *szGraphName)
{
	CString GraphName = FindStochastics(szGraphName);

	for( int i = 0 ; i < GRAPH_COUNT ; i++ )
	{
// 2008.01.25 by LYH >>
		//if( GraphName.CompareNoCase(g_szIndicatorNameList[i]) == 0 )
		if( GraphName.CompareNoCase( _MTEXTN( g_szIndicatorNameList[i]) ) == 0 )
// 2008.01.25 by LYH <<
		{
			switch(i)
			{
				case GRAPH_PSYCH: // 수평선의 값 0~100
//				case GRAPH_NEWPSYCH:
				case GRAPH_MOMENTUM:
// (2007/6/4 - Seung-Won, Bae) Do not Support -100~100 Limitation.
//				case GRAPH_CCI:
				case GRAPH_EOM:
				case GRAPH_RSI:
				case GRAPH_STOCHASTICS_FAST:
				case GRAPH_STOCHASTICS_SLOW:
					return CBaseData::ZeroToOneZeroZero;
			
				case GRAPH_WILLIAM: //수평선의 값 -0~-100
					return CBaseData::MinusZeroToOneZeroZero;
			}
		}
	}

	return CBaseData::Others; // 수평선이 값 -1000~1000
}
		

bool CFindType::DoThisIndicatorUseOnePricePacket( const char *p_szIndicatorName)
{
	CString GraphName = FindStochastics(p_szIndicatorName);
	
	for( int i = 0 ; i < GRAPH_COUNT ; i++ )
	{
// 2008.01.25 by LYH >>
		//if( GraphName.CompareNoCase(g_szIndicatorNameList[i]) == 0 )
		if( GraphName.CompareNoCase( _MTEXTN( g_szIndicatorNameList[i]) ) == 0 )
// 2008.01.25 by LYH <<
		{
			switch(i)
			{
				case GRAPH_GLANCEBALANCE:
				case GRAPH_HORIZONTALBAR:
				case GRAPH_BOXCHART:
				case GRAPH_Demark:
				case GRAPH_MAC:
				case GRAPH_PARABOLIC_SAR:
				case GRAPH_PIVOT:
				case GRAPH_PRICE_CHANNEL:
				case GRAPH_AD:
				case GRAPH_ADX:
				case GRAPH_ADXR:
				case GRAPH_AROON:
				case GRAPH_NDI:
				case GRAPH_AB_RATIO:
				case GRAPH_ATR:
				case GRAPH_ChaikinsVolatility:
				case GRAPH_DRF:
				case GRAPH_MASS_INDEX:
				case GRAPH_QSTIC:
				case GRAPH_CHARKINS_OSCILLATOR:
				case GRAPH_CMF:
				case GRAPH_EOM:
				case GRAPH_VOLUME_RATIO:
				case GRAPH_VROC:
				case GRAPH_PERCENT_B_STOCHASTIC:
				case GRAPH_LRS_STOCHASTIC:
				case GRAPH_MACD_STOCHASTIC:
				case GRAPH_MOMENTUM_STOCHASTIC:
				case GRAPH_OBV_MOMENTUM:
				case GRAPH_OBV_STOCHASTIC:
				case GRAPH_ROC_STOCHASTIC:
				case GRAPH_RSI_MACD:
				case GRAPH_RSI_STOCHASTIC:
				case GRAPH_SONAR_PSYCO_LINE:
				case GRAPH_STOCHASTIC_RSI:
				case GRAPH_INTEREST:						// 금리
				case GRAPH_NEWS_INTENSITY:					// 뉴스 강도
				case GRAPH_NEWS_COUNT:						// 뉴스 건수
				case GRAPH_AVOLS:							// 시가총액( Aggregate Value Of Listed Stock ) 
				case GRAPH_DEBIT_BALANCE:					// 신용잔고( Debit Balance )
				case GRAPH_DEPOSIT:							// 예탁금( Deposit )
				case GRAPH_FOREIGNER_RESERVE_QUANTITY:		// 외국인 보유량
				case GRAPH_FOREIGNER_RESERVE_RATIO:			// 외국인 보유율
				case GRAPH_FOREIGNER_EXHAUSTED_RATIO:		// 외국인 소진율
				case GRAPH_INVESTER_NET_BUYER_FUTURES:		// 투자자 순매수(선물)
				case GRAPH_INVESTER_NET_BUYER_CALL_OPTION:	// 투자자 순매수(콜옵션)
				case GRAPH_INVESTER_NET_BUYER_KOSDAQ:		// 투자자 순매수(KOSDAQ)
				case GRAPH_INVESTER_NET_BUYER_KOSPI:		// 투자자 순매수(KOSPI)
				case GRAPH_INVESTER_NET_BUYER_PUT_OPTION:	// 투자자 순매수(풋옵션)
				case GRAPH_PRICE_MA:
				case GRAPH_BinaryWave:
				case GRAPH_7BINARYWAVE:
				case GRAPH_CLC:

				// Sang-Woo, Cho		2006 / 05 / 09
				// VA Oscillator & Volume Oscillator에 Packet 선택부분을 숨김.
				case GRAPH_VA_OSCILLATOR:
				case GRAPH_VOLUME_OSCILLATOR:
					return true;
			}
		}
	}

	return false; 
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 12
// Return CString : 
// Param  const CString &strGraphName : 
// Comments		: 
//-----------------------------------------------------------------------------
CString CFindType::FindStochastics( const CString &strGraphName )
{
	// "Stochastics Fast"
	if( strGraphName.Find(_MTEXT( C2_STOCHASTIC_FAST )) != -1 ) 
		return _MTEXT( C2_STOCHASTIC_FAST );
	
	// "Stochastics Slow"
	else if( strGraphName.Find(_MTEXT( C2_STOCHASTIC_SLOW )) != -1 ) 
		return _MTEXT( C2_STOCHASTIC_SLOW );

	return strGraphName;
}
