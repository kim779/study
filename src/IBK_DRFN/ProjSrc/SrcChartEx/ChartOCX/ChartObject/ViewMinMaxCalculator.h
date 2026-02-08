// ViewMinMaxCalculator.h: interface for the CViewMinMaxCalculator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEWMINMAXCALCULATOR_H__8023B2AA_3388_46FC_8996_1E3B938E8676__INCLUDED_)
#define AFX_VIEWMINMAXCALCULATOR_H__8023B2AA_3388_46FC_8996_1E3B938E8676__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/BlockBaseData.h"

class CHorzDisplayDataMinMaxInputData;
class CVertDisplayDataMinMaxInputData;
class CDisplayDataMinMax;
class CDisplayAttributes;
class CSubGraphData;
class CSubGraphPacketList;
class CPacketList;
class CPacket;

class CViewMinMaxCalculator  
{
public:
	// view min, max 값 가져오기
	CDisplayDataMinMax GetViewHorzMinMax(const CHorzDisplayDataMinMaxInputData& inputData) const;
	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	bool GetViewVertMinMax(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange, BOOL bDrawBaseLine);

private:
	// 세로 scale의 min/max를 구할지의 여부
	bool IsViewVertMinMax(CSubGraphPacketList* pSubGraphPacketList) const;

	// 가로 scale
	// "MarketProfile" 일 경우
	CDisplayDataMinMax GetViewHorzMinMax_MarketProfile(const CHorzDisplayDataMinMaxInputData& inputData) const;

	// 세로 scale
	// -> "지표 Data" 인 경우
	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	bool GetViewVertMinMax_Indicate(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange, BOOL bDrawBaseLine);
	bool GetViewVertMinMax_Indicate_General_Gap(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange);
	bool GetViewVertMinMax_Indicate_General(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange);
	bool GetViewVertMinMax_HeadPacket(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange);
	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	bool GetViewVertMinMax_EndPacket(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange, BOOL bDrawBaseLine);
	// (2004.02.06, 배승원) 선물 전용 P&F의 경우 별도로 확인한다.
	BOOL	GetViewVertMinMax_FuPAndF( const CVertDisplayDataMinMaxInputData &inputData, CDisplayDataMinMax &displayDataMinMax);
	bool GetViewVertMinMax_PAndF(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange);
	bool GetViewVertMinMax_Sale(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange);
	bool GetViewVertMinMax_MarketProfile(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax);
	// -> 기타 나머지 모두..
	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	bool GetViewVertMinMax_Etc(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange, BOOL bDrawBaseLine);
	bool GetViewVertMinMax_Fund(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange);
	bool GetViewVertMinMax_InSubGraphPacketList(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange);
	bool GetViewVertMinMax_OutsideSubGraphPacketList(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange);
	// graph에 따른 Min/Max 변경
	bool ChangeViewVertMinMax_Indicator(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax);
	// -> graph type / graph style에 따른 경우
	bool ChangeViewVertMinMax_GraphTypeOrStyle(CSubGraphData* pSubGraphData, CDisplayDataMinMax& displayDataMinMax);
	bool ChangeViewVertMinMax_BarType(CSubGraphData* pSubGraphData, CDisplayDataMinMax& displayDataMinMax);

	bool GetViewVertMinMax_Packet(CPacket* pPacket, const CDisplayAttributes& displayAttributes, 
		const bool bWithoutZero, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange);
	bool GetPacketMinMax(CPacket* pPacket, const CDisplayAttributes& displayAttributes, const bool bWithoutZero, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange) const;
// (2006.11.09 안승환) 분산차트 추가
	bool GetViewVertMinMax_DisperSion(const CVertDisplayDataMinMaxInputData& inputData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange);
// (2006.11.09 안승환) 분산차트 추가

	// 해당 packet 가져오기
	CPacketList* GetPacketList(CSubGraphPacketList* pSubGraphPacketList) const;
	CPacket* GetPacket(CSubGraphPacketList* pSubGraphPacketList, const CString& strPacketName) const;
	bool GetDataStartAndEndInBlock(CPacket* pPacket, const CDisplayAttributes& displayAttributes, int& nStart, int& nEnd) const;
	// 화면에 보이는 data 시작 index
	int GetDataStartInBlock(CPacket* pPacket, const int nDataStartInBlock) const;
	// Min/Max에 "0" 포함 여부
	bool DoesViewMinMaxWithoutZero(CSubGraphData* pSubGraphData) const;
	// Min/Max에 gap을 줄지의 여부
	bool DoesDisplayDataMinMaxInGap(const CVertDisplayDataMinMaxInputData& inputData) const;
	bool DoesDisplayDataMinMaxInGap_Type(const CSubGraphData* pSubGraphData) const;
};

#endif // !defined(AFX_VIEWMINMAXCALCULATOR_H__8023B2AA_3388_46FC_8996_1E3B938E8676__INCLUDED_)
