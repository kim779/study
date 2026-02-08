// DefineData.cpp: implementation of the CDefineData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DefineData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CDefineData

const char* CDefineData::PACKET_TYPE_TEXT = "문자";

const char* CDefineData::SCALEDATA_DATE = "자료일자";
const char* CDefineData::SCALEDATA_TEXT = "Text";
const char* CDefineData::SCALEDATA_PRICEZONE = "행사가";
const char* CDefineData::SCALEDATA_INDICATEDATA = "지표 Data";
const char* CDefineData::SCALEDATA_VOLUME = "거래량";
const char* CDefineData::SCALEDATA_PERCENT = "백분율";

const char* CDefineData::INDICATEGROUPNAME_MARKET = "시장지표";

const char* CDefineData::INDICATEDATA_PRICECHART = "가격차트";
const char* CDefineData::INDICATEDATA_VOLUMECHART = "거래량차트";
const char* CDefineData::INDICATEDATA_OPTIMIZEAIGNALCHART = "IndicatorOptimize Signal";
const char* CDefineData::INDICATEDATA_VOLUMEPRICE = "거래대금";

const char* CDefineData::INDICATEDATA_PRICEMACHART = "가격 이동평균";
const char* CDefineData::INDICATEDATA_VOLUMEMACHART = "거래량 이동평균";
const char* CDefineData::INDICATEDATA_DISPARITYCHART = "이격도";
const char* CDefineData::INDICATEDATA_GLANCEBALANCECHART = "일목균형표";
const char* CDefineData::INDICATEDATA_PSYCHCHART = "심리도";//"투자심리선";
const char* CDefineData::INDICATEDATA_NEWPSYCHCHART = "신심리선";
const char* CDefineData::INDICATEDATA_SONAR_PSYCHCHART = "Sonar+심리도";
const char* CDefineData::INDICATEDATA_BOLLINGERCHART = "Bollinger Bands";
const char* CDefineData::INDICATEDATA_BOXCHARTCHART = "Box Chart";
const char* CDefineData::INDICATEDATA_CLCCHART = "꺽은선차트";
const char* CDefineData::INDICATEDATA_ENVELOPECHART = "Envelope";
const char* CDefineData::INDICATEDATA_PARABOLICCHART = "Parabolic SAR";
const char* CDefineData::INDICATEDATA_PIVOTCHART = "Pivot";
const char* CDefineData::INDICATEDATA_RSICHART = "RSI";
const char* CDefineData::INDICATEDATA_STOCHASTICSCHART = "Stochastic";
const char* CDefineData::INDICATEDATA_STOCHASTICSFASTCHART = "Stochastics Fast";
const char* CDefineData::INDICATEDATA_STOCHASTICSSLOWCHART = "Stochastics Slow";
const char* CDefineData::INDICATEDATA_STOCHASTICSOSCILLATORCHART = "Stochastics Oscillator";
const char* CDefineData::INDICATEDATA_WILLIAMCHART = "%R";//"William's %R";
const char* CDefineData::INDICATEDATA_RAINBOWCHART = "그물차트";
const char* CDefineData::INDICATEDATA_ZIGZAGCHART = "Zig Zag";
const char* CDefineData::INDICATEDATA_PRICECHANNELCHART = "Price Channel";
const char* CDefineData::INDICATEDATA_DEMARKCHART = "Demark";
const char* CDefineData::INDICATEDATA_ERRORBANDCHART = "표준오차밴드";
const char* CDefineData::INDICATEDATA_ERRORCHANNELCHART = "표준오차채널";
const char* CDefineData::INDICATEDATA_DEVIATIONCHANNELCHART = "표준편차채널";
const char* CDefineData::INDICATEDATA_DEMACHART = "DEMA";
const char* CDefineData::INDICATEDATA_MACCHART = "MAC";
const char* CDefineData::INDICATEDATA_STARCBANDSCHART = "Starc Bands";
const char* CDefineData::INDICATEDATA_TEMACHART = "TEMA";
const char* CDefineData::INDICATEDATA_LOCKCHART = "락";

const char* CDefineData::INDICATEDATA_THREELINEBRACKCHART = "삼선전환도";
const char* CDefineData::INDICATEDATA_INVERSLINECHART = "역시계곡선";
const char* CDefineData::INDICATEDATA_PANDFCHART = "P&F";
const char* CDefineData::INDICATEDATA_SWINGCHART = "SwingChart";
const char* CDefineData::INDICATEDATA_KAGICHART = "KagiChart";
const char* CDefineData::INDICATEDATA_RENKOCHART = "RenkoChart"; // YJS(05/06/21) RenKoChart ADD.
const char* CDefineData::INDICATEDATA_FUOPPANDFCHART = "FuOpP&F";
const char* CDefineData::INDICATEDATA_COMPARATIVECHART = "상대비교차트";
const char* CDefineData::INDICATEDATA_MARKETPROFILECHART = "MarketProfile";
const char* CDefineData::INDICATEDATA_MINIJISUCHART = "미니지수차트";
const char* CDefineData::INDICATEDATA_INDUSTRYJISUCHART = "업종지수";
const char* CDefineData::INDICATEDATA_MARKETJISUCHART = "시장지수";
const char* CDefineData::INDICATEDATA_INDIVIDUAL_INVESTOR = "개인투자자";
const char* CDefineData::INDICATEDATA_ORGAN_INVESTOR = "기관투자자";
const char* CDefineData::INDICATEDATA_FOREIGNER_INVESTOR = "외국인투자자";

const char* CDefineData::INDICATEDATA_PRICE = "가격";
const char* CDefineData::INDICATEDATA_VOLUME = "거래량";
const char* CDefineData::INDICATEDATA_PRESPEN = "선행스팬";
const char* CDefineData::INDICATEDATA_VOLUMEFORSALE = "대기매물";
const char* CDefineData::INDICATEDATA_VOLUMEFORSALEINPRICE = "대기매물_가격대";

const char* CDefineData::PACKETNAME_OPEN = "시가";
const char* CDefineData::PACKETNAME_HIGH = "고가";
const char* CDefineData::PACKETNAME_LOW = "저가";
const char* CDefineData::PACKETNAME_CLOSE = "종가";
const char* CDefineData::PACKETNAME_VOLUME = "기본거래량";

const char* CDefineData::PACKETNAME_ZIGDATA = "Zig_Data";
const char* CDefineData::PACKETNAME_ZIGINDEX = "Zig_Index";

const char* CDefineData::PACKETNAME_PFHIGH = "PF_고가";
const char* CDefineData::PACKETNAME_PFLOW = "PF_저가";
const char* CDefineData::PACKETNAME_PFDATE = "PF_자료일자";
const char* CDefineData::PACKETNAME_INVERSELINEPRICE = "역시계곡선_가격";
const char* CDefineData::PACKETNAME_INVERSELINEVOLUME = "역시계곡선_거래량";
const char* CDefineData::PACKETNAME_THREELINEBREAKOPEN = "ThreeLineBreak_시가";
const char* CDefineData::PACKETNAME_THREELINEBREAKCLOSE = "ThreeLineBreak_종가";
const char* CDefineData::PACKETNAME_THREELINEBREAKDATE = "ThreeLineBreak_자료일자";
const char* CDefineData::PACKETNAME_SWINGCHARTDATE = "SwingChart_자료일자";
const char* CDefineData::PACKETNAME_SWINGCHARTPRICE = "SwingChart_가격";
const char* CDefineData::PACKETNAME_KAGICHARTDATE = "Kagi_자료일자";
const char* CDefineData::PACKETNAME_KAGICHARTPRICE = "Kagi_가격";
const char* CDefineData::PACKETNAME_RENKOCHARTDATE = "RenKoChart_자료일자"; // YJS (05/06/23) RENKO CHART ADD
const char* CDefineData::PACKETNAME_RENKOCHARTOPEN = "RenKoChart_시가";	// YJS (05/06/23) RENKOCHART OPEN ADD
const char* CDefineData::PACKETNAME_RENKOCHARTCLOSE = "RenKoChart_종가"; // YJS (05/06/23) RENKO CHART ADD

// 가격 이동평균, 그물차트, 대기매물, 일목균형표, 표준오차밴드, 표준오차채널, 표준편차채널, 꺽은선차트, 
// 락, Bollinger Bands, Box Chart, DEMA, Demark, Envelope, MAC, Parabolic, Pivot, Price Channel, 
// Starc Bands, TEMA, ZigZag
const char* CDefineData::SCALEGROUP_PRICE[Price_Count] = {INDICATEDATA_PRICECHART, \
		INDICATEDATA_PRICEMACHART, INDICATEDATA_PARABOLICCHART, \
		INDICATEDATA_RAINBOWCHART, INDICATEDATA_ZIGZAGCHART, INDICATEDATA_CLCCHART, \
		INDICATEDATA_BOLLINGERCHART, INDICATEDATA_ENVELOPECHART, INDICATEDATA_BOXCHARTCHART, \
		INDICATEDATA_PIVOTCHART, INDICATEDATA_GLANCEBALANCECHART, INDICATEDATA_PRICECHANNELCHART, \
		INDICATEDATA_DEMARKCHART, INDICATEDATA_VOLUMEFORSALE, INDICATEDATA_ERRORBANDCHART, \
		INDICATEDATA_ERRORCHANNELCHART, INDICATEDATA_DEVIATIONCHANNELCHART, INDICATEDATA_LOCKCHART, \
		INDICATEDATA_DEMACHART, INDICATEDATA_MACCHART, INDICATEDATA_STARCBANDSCHART, INDICATEDATA_TEMACHART};
const char* CDefineData::SCALEGROUP_PLUSPERCENT[PlusPercent_Count] = {INDICATEDATA_PSYCHCHART, \
		INDICATEDATA_SONAR_PSYCHCHART, INDICATEDATA_RSICHART, \
		INDICATEDATA_STOCHASTICSFASTCHART, INDICATEDATA_STOCHASTICSSLOWCHART};
const char* CDefineData::SCALEGROUP_MINUSPERCENT[MinusPercent_Count] = {INDICATEDATA_WILLIAMCHART};
const char* CDefineData::SCALEGROUP_PLUSMINUSPERCENT[PlusMinusPercent_Count] = {\
		INDICATEDATA_STOCHASTICSOSCILLATORCHART, INDICATEDATA_NEWPSYCHCHART};


// public ===============================================================================
// TR 발생해야 하는 그룹인지 체크
bool CDefineData::IsIndicatorGroup_TR(const CString& groupName)
{
	// <시장지표>
	return (groupName == INDICATEGROUPNAME_MARKET);
}

// ----------------------------------------------------------------------------
// 해당 scale group 인치 체크
bool CDefineData::IsScaleGroup_Price(const CString& functionName)
{
	if(functionName.IsEmpty())
		return false;

	// <대기매물> : 기존엔 특수형이 였으나 <가격차트>에 첨부되는 차트로 수정함.

	for(int i = 0; i < Price_Count; i++)
	{
		if(functionName == SCALEGROUP_PRICE[i])
			return true;
	}
	return false;
}

// <가격차트> group 이지만 scale은 따로 쓰는 경우.
bool CDefineData::IsScaleGroup_PriceNoVertScale(const CString& functionName)
{
	if(functionName.IsEmpty())
		return false;

	// <업종지수>, <시장지수>
	return (functionName == INDICATEDATA_INDUSTRYJISUCHART || 
		functionName == INDICATEDATA_MARKETJISUCHART);
}

bool CDefineData::IsScaleGroup_Volume(const CString& functionName)
{
	// <거래량차트>, <거래량 이동평균>
	return (functionName == INDICATEDATA_VOLUMECHART || 
		functionName == INDICATEDATA_VOLUMEMACHART);
}

bool CDefineData::IsScaleGroup_InverseLine(const CString& functionName)
{
	return (functionName == INDICATEDATA_INVERSLINECHART);
}

bool CDefineData::IsScaleGroup_PlusPercent(const CString& functionName)
{
	if(functionName.IsEmpty())
		return false;

	CString strStochastic_Etc1, strStochastic_Etc2;
	strStochastic_Etc1.Format("%s+", INDICATEDATA_STOCHASTICSCHART);
	strStochastic_Etc2.Format("+%s", INDICATEDATA_STOCHASTICSCHART);
	for(int i = 0; i < PlusPercent_Count; i++)
	{
		if(functionName == SCALEGROUP_PLUSPERCENT[i])
			return true;
		else if(functionName.Find(strStochastic_Etc1) >= 0 || functionName.Find(strStochastic_Etc2) >= 0)
			return true;
	}
	return false;
}

bool CDefineData::IsScaleGroup_MinusPercent(const CString& functionName)
{
	if(functionName.IsEmpty())
		return false;

	for(int i = 0; i < MinusPercent_Count; i++)
	{
		if(functionName == SCALEGROUP_MINUSPERCENT[i])
			return true;
	}
	return false;
}

bool CDefineData::IsScaleGroup_PlusMinusPercent(const CString& functionName)
{
	if(functionName.IsEmpty())
		return false;

	for(int i = 0; i < PlusMinusPercent_Count; i++)
	{
		if(functionName == SCALEGROUP_PLUSMINUSPERCENT[i])
			return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
// x축 스케일을 표현하지 않는지 체크
bool CDefineData::IsNoXScale_SpecialGroup(const CString& functionName)
{
	if(functionName == INDICATEDATA_PANDFCHART)	// "P&F"
		return true;
//	else if(functionName == INDICATEDATA_THREELINEBRACKCHART) // "삼선전환도"
//		return true;

	return false;
}

// ----------------------------------------------------------------------------
// <거래량 차트>, <거래대금> 인 경우
bool CDefineData::IsDrawGroup_VolumeType(const CString& functionName)
{
	return (functionName == INDICATEDATA_VOLUMECHART || functionName == INDICATEDATA_VOLUMEPRICE);
}

// <P&F> 형태의 지표인지 체크
bool CDefineData::IsDrawGroup_PAndF(const CString& functionName)
{
	return (functionName.Find(INDICATEDATA_PANDFCHART) != -1);
}

// <특수차트> 인지의 여부
bool CDefineData::IsSpecialGroupGraph(const CString& functionName)
{
	if(functionName == INDICATEDATA_THREELINEBRACKCHART) // "삼선전환도"
		return true;
	else if(functionName == INDICATEDATA_INVERSLINECHART) // "역시계곡선"
		return true;
	else if(functionName == INDICATEDATA_PANDFCHART)	// "P&F"
		return true;
	else if(functionName == INDICATEDATA_SWINGCHART)	// "SwingChart"
		return true;
	else if(functionName == INDICATEDATA_KAGICHART)		// "KagiChart"
		return true;
	else if(functionName == INDICATEDATA_RENKOCHART)	// "RenKoChart" YJS (05/06/23) RENKO CHART ADD
		return true;

	return false;
}

// <투자자동향> 인지의 여부
bool CDefineData::IsInvestorGroupGraph(const CString& functionName)
{
	// <개인투자자>, <기관투자자>, <외국인투자자>
	return (functionName == INDICATEDATA_INDIVIDUAL_INVESTOR || 
		functionName == INDICATEDATA_ORGAN_INVESTOR || 
		functionName == INDICATEDATA_FOREIGNER_INVESTOR);
}

// ----------------------------------------------------------------------------
// min/max 구하는 packet이 마지막(2) 인 경우
bool CDefineData::IsViewVertMinMax_EndPacket(const CString& functionName)
{
	if(functionName.IsEmpty())
		return false;

	if(functionName == INDICATEDATA_ZIGZAGCHART)	// Zig Zag
		return true;
	else if(functionName == INDICATEDATA_CLCCHART)	// 꺽은선차트
		return true;

	return false;
}

// ----------------------------------------------------------------------------
// condition title 인지 체크
bool CDefineData::IsConditionTitle(const CString& functionName)
{
	if(functionName.IsEmpty())
		return false;

	if(functionName == INDICATEDATA_PRICECHART)	// 가격차트
		return false;
	else if(functionName == INDICATEDATA_VOLUMECHART)	// 거래량차트
		return false;
	else if(functionName == INDICATEDATA_PRICEMACHART)	// 가격 이동평균
		return false;
	else if(functionName == INDICATEDATA_VOLUMEMACHART)	// 거래량 이동평균
		return false;
	else if(functionName == INDICATEDATA_DISPARITYCHART)// 이격도
		return false;
	else if(functionName == INDICATEDATA_COMPARATIVECHART)	// 비교차트
		return false;
	else if(functionName.Find(INDICATEDATA_PANDFCHART) != -1)// P&F, FuOpP&F
		return false;

	return true;
}

// data title 인지 체크
bool CDefineData::IsDataTitle(const CString& functionName)
{
	if(functionName.IsEmpty())
		return false;

	if(functionName == INDICATEDATA_PRICECHART)	// 가격차트
		return false;
	else if(functionName == INDICATEDATA_VOLUMECHART)	// 거래량차트
		return false;
	else if(functionName == INDICATEDATA_RAINBOWCHART)	// 그물차트
		return false;
	else if(functionName == INDICATEDATA_BOXCHARTCHART)	// "Box Chart"
		return false;
	else if(functionName == INDICATEDATA_CLCCHART)		// 꺽은선차트
		return false;
	else if(functionName == INDICATEDATA_ZIGZAGCHART)	// Zig Zag
		return false;
	else if(functionName == INDICATEDATA_VOLUMEFORSALE)	// 대기매물
		return false;

	return true;
}

// title이 조건값인지의 여부
bool CDefineData::IsEqualTitleAndCondition(const CString& functionName)
{
	if(functionName.IsEmpty())
		return false;

	if(functionName == INDICATEDATA_PRICEMACHART)	// 가격 이동평균
		return true;
	else if(functionName == INDICATEDATA_VOLUMEMACHART)	// 거래량 이동평균
		return true;

	return false;
}

// ----------------------------------------------------------------------------
