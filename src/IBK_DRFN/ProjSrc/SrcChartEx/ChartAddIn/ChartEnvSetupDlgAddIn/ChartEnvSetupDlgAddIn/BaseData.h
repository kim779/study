// BaseData.h: interface for the CBaseData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEDATA_H__7AC3DE75_7AED_4DDA_893F_BA6368AC1DBE__INCLUDED_)
#define AFX_BASEDATA_H__7AC3DE75_7AED_4DDA_893F_BA6368AC1DBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define	UDM_INDICATOR_GRAPH_INSERT	( WM_USER + 701	)		// MACD Oscillator 추가하라는 사용자 Message


// (2006/4/19 - Seung-Won, Bae) Not Used. So Comment
/*
static const int NUM_ITEM = 50;
// 조건값
static const _TCHAR *_pConditiontitle[ NUM_ITEM ] = 
{
	_T("한칸값"),		_T("전환칸"),		_T("구간설정"),			_T("단기"),				_T("중기"),   
	_T("장기"),			_T("Signal"),		_T("기간"),				_T("승수"),				_T("평활기간"),
	_T("이평기간"),		_T("누적기간"),		_T("밴드범위"),			_T("최대값"),			_T("간격"),
	_T("Open이평"),		_T("Close이평"),	_T("전환기간"),			_T("기준기간"),			_T("선행2기간"),
	_T("ADX"),			_T("ADXR"),			_T("이동평균"),			_T("상하한"),			_T("증가단위"),
	_T("표시갯수"),		_T("조건"),			_T("묶음갯수"),			_T("날짜"),				_T("수치"),
	_T("Roc 기간"),		_T("Stoc 기간"),	_T("Macd signal"),		_T("표준편차"),			_T("가속변수"),
	_T("최대허용값"),	_T("소나기간"),		_T("1차 평활함수"),		_T("2차 평활함수"),		_T("구간수")
	_T("텍스트(수치)"), _T("")
};
*/

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 18
// Comments		: 기본 Data의 Setting 값을 설정한 class
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
class CBaseData  
{
public:
	// 칼라 listbox에 들어가야하는 Item의 string
	enum COLORLISTITEMTYPE
	{
		NONE,					
		PRICE,					// 가격
		VOLUME,					// 거래량
		UP_DOWN,				// 상승, 하락
		DAEGIMEMUL,				// 대기매물
		COLOR,					// 색상
		SUBGRAPHNAME,			// SubGraph이름
		RAINBOW,				// 시작색상, 종료색상
		SUB_UPFILL_DOWNFILL,	// SubGraph이름, 상승채움, 하락채움
		GLANCEBLANCE,			// 일목 전환, 기준, 후행스팬, 선행스팬, 상승채움, 하락채움.
		INVERSELINE,			
		MACD,					// MACD
		PIVOT					// Pivot
	};

	
	enum TABITEMTYPE
	{
		CONFIGURATION,			// 설정
		CONFIGU_BASELINE,		// 설정, 기준선
		CONFIGU_HORIZONLINE,	// 설정, 수평선
		CONFIGU_SIGNAL			// 설정, signal
	};


	// 지표에 따른 SubTabPage
	enum DLGTYPE
	{
		SeveralSub_FourCondition_WithCombo,		// 여러 sub, condition4계산방법
		SeveralSub_FourCondition,				// 여러 sub, condition4
		SeveralSub_ThreeCondition,				// 여러 Sub, Condition 3
		SeveralSub_TwoCondition,				// 여러 Sub, Condition 2
		SeveralSub_OneCondition,				// 여러 Sub, Condition 1
		SeveralSub_NoCondition,					// 여러 Sub, No Condition
		OneSub_NoCondition,						// Sub 1, No-Condition
		OneSub_OneCondition,					// Sub 1, Condition 1
		OneSub_TwoCondition,					// Sub 1, Condition 2
		OneSub_ThreeCondition,					// Sub 1, Condition 3
		OneSub_SevenCondition,					// sub 1, Condition 7
		Price,									// price
		Volume,									// volume
		MovingAverage,							// MovingAverage
		PriceMA,								// PriceMA
		Disparity,								// 이격도
		Daegimenul,								// 대기매물, 채움
		GLANCEBALANCE,							// 일목 균형표 
		OneSub_ThreeCondi_WithCombo,			// Sub 1, 콤보를 가진 Condition 3
		OneSub_TwoCondi_WithCombo,				// Sub 1, 콤보를 가진 Conditon2 
		SeveralSub_ThreeCondi_WithCmbo,			// 여러 Sub, 콤보를 가진 Conditon3 
		Market_Indicator_First,					// 시장지표중 가장 많은 형태.
		Market_Indicator_InvestorTrend,			// 투자자동향에 Packet설정부분 추가
		DT_SPECIAL_TYPE_A,						// 삼선전환도
		DT_SPECIAL_TYPE_B,						// P&F, 역시계곡선, Kagi
		DT_SPECIAL_TYPE_C,						// RENKO,분산챠트
		DT_SPECIAL_TYPE_D,
		DT_SPECIAL_TYPE_E,						// 대기매물 설정 분리 Sang-Woo, Cho		2006 / 04 / 19
		DT_COMMON_TYPE_A,						// 대부분의 모든값
		DT_COMMON_TYPE_B						// Sang-Woo, Cho	2006 / 05 / 02 - BinaryWave 때문에 추가.
	};

	// 채움 속성
	enum FILLTYPE
	{
		Fill,				// 상승, 하락 채움
		NoFill				// 채움 필요없음.
	};

	// 가격차트와 같이 그려지는지..아닌지..
	enum DRAWPOSITIONTYPE
	{
		DrawWithPrice,		// 가격차트와 같이 그려진다.
		NotDrawWithPrice 
	};

	// 수평선 설정값의 영역
	enum LIMITHORIZONTALLINE
	{
		ZeroToOneZeroZero,
		MinusZeroToOneZeroZero,
		Others
	};
	
	// 그래프의 현재 상태
	enum GRAPHSTATE
	{
		OnlySetting,
		Insert,
		Delete
	};

	// 리스트 컨트롤의 부모.
	enum PARENTOFLIST
	{
		Tab,
		Dialog
	};
};


#endif // !defined(AFX_BASEDATA_H__7AC3DE75_7AED_4DDA_893F_BA6368AC1DBE__INCLUDED_)
