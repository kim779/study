// DrawingRegionCalculator.h: interface for the CDrawingRegionCalculator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWINGREGIONCALCULATOR_H__16FD5E76_FAA3_4228_8E7F_E9BAF4B69903__INCLUDED_)
#define AFX_DRAWINGREGIONCALCULATOR_H__16FD5E76_FAA3_4228_8E7F_E9BAF4B69903__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/BlockBaseData.h"

class CScaleCompart;
class CDrawingRegion;
class CHorzScaleRegionHeight;
class CVertScaleRegionWidth;
class CTextSizeCalculator;
class CPacketList;
class CPacket;
class CIndicatorInfo;
class CGraphRegions;
class CDisplayAttributes;
class CDisplayDataMinMax;
class CDrawingRegionCalData;
class CVertScaleMaxWidthCalculatorData;
class CHorzScaleMaxWidthCalculatorData;
class CSubGraphPacketList;

// 그리는 영역에 대한 계산 class
class CDrawingRegionCalculator  
{
public:
	// 그리는 영역(graph, scale)을 구한다.
	// (2009/1/20 - Seung-Won, Bae) Support Reverse.
	CDrawingRegion GetDrawingRegion(CDC* pDC, const CDrawingRegionCalData& calData, BOOL p_bReverse) const;

private:
	// scale 영역
	CRect GetLeftScaleRegion(const CRect& blockRegion, const CRect& graphFrameRegion, const CRect& graphRegion, const int nWidth) const;
	CRect GetRightScaleRegion(const CRect& blockRegion, const CRect& graphFrameRegion, const CRect& graphRegion, const int nWidth) const;
	CRect GetTopScaleRegion(	const CRect& blockRegion, const CRect &graphDrawRegion, const CRect& graphFrameRegion) const;
	CRect GetBottomScaleRegion(	const CRect& blockRegion, const CRect &graphDrawRegion, const CRect& graphFrameRegion) const;
};


// graph 영역을 계산하는 class
class CGraphRegionCalculator
{
public:
	// graph 영역들.
	// (2009/1/20 - Seung-Won, Bae) Support Reverse.
	CGraphRegions GetGraphRegions(CDC* pDC, const CDrawingRegionCalData& calData, BOOL p_bReverse) const;

private:
	// graph frame이 포함된 영역을 구한다.
	CRect GetGraphFrameRegion(CDC* pDC, const CDrawingRegionCalData& calData) const;

	// 최대 graph frame region(frame 포함) --> (block region - scale region)
	CRect GetMaxGraphFrameRegion(const CRect& blockRegion, const CHorzScaleRegionHeight& horzScaleRegionHeight, const CVertScaleRegionWidth& vertScaleRegionWidth) const;
	// view data count에 의해 graph영역릉 조절한다. (세로 scale 영역에 넘겨준다.)
	CRect GetRealGraphRegion(const CDrawingRegionCalData& calData, const CRect& maxGraphDrawingRegion) const;

	// 세로 scale 의 타입
	CScaleBaseData::VERTSCALEPOSITION GetVertScalePosition(const CVertScaleRegionWidth& vertScaleRegionWidth) const;
	// 세로 scale영역에 줄 폭 (graph의 x축 길이)
	int GetRemainedSpaceWidth(const int graphDrawingRegionWidth, const int viewDataCount) const;

	// graph drawing region의 왼쪽&오른쪽 좌표를 구한다. ==> space 적용
	int GetLeft(const CScaleBaseData::VERTSCALEPOSITION vertScalePosition, const int orgLeft, 
		const int maxVertScaleRegionWidth, const int leftVertScaleRegionWidth, const int remainedSpaceWidth) const;
	int GetRight(const CScaleBaseData::VERTSCALEPOSITION vertScalePosition, const int orgRight, 
		const int maxVertScaleRegionWidth, const int rightVertScaleRegionWidth, const int remainedSpaceWidth) const;
	int GetData(const int orgData, const int maxVertScaleRegionWidth, const int vertScaleRegionWidth, const int remainedSpaceWidth, const int scalePosition) const;

	// graph text를 쓸 영역 유무
	bool IsGraphTextDraw(const CRect& graphRegion, const CDrawingRegionCalData& calData) const;
};


// scale text size구하는 것에 대한 class
class CTextSizeCalculator
{
// (2008/5/14 - Seung-Won, Bae) for Multi-Language of Special Date or Price Unit Text
protected:
	HWND	m_hOcxWnd;

// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
protected:
	CChartInfo::CHART_MODE	m_eChartMode;

public:
	CTextSizeCalculator( HWND p_hOcxWnd, CChartInfo::CHART_MODE	p_eChartMode)
		{	m_hOcxWnd = p_hOcxWnd;	m_eChartMode = p_eChartMode;	}

public:
	// 가로 scale의 최대 text size
	CSize GetMaxHorzScaleTextSize(CDC* pDC, const CHorzScaleMaxWidthCalculatorData& calData) const;
	// 세로 scale의 최대 text size.
	CSize GetMaxVertScaleTextSize(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const;

private:
	// text size를 구할지의 유무
	bool IsTextSizeCalculator(CPacket* pPacket) const;
	bool IsTextSizeCalculator(CSubGraphPacketList* pSubGraphPacketList) const;
	// <기준선> 이 있는지의 여부
	bool IsBaseLineInIndicateData(CIndicatorInfo* pIndicatorInfo) const;
	// <기준선> 이 그리는 영역인지의 여부
	bool IsBaseLineInRegion(const CVertScaleMaxWidthCalculatorData& calData) const; 

	// scale data text size.
	CSize GetMaxScaleText_Data(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const;
	// 오른쪽 scale data rate text size.
	CSize GetMaxScaleText_Rate(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const;

	// scale data text size. -> <자료일자>인 경우.
	CSize GetMaxScaleTextSize_DataDate(CDC* pDC, CPacket* pPacket, const CScaleCompart& scaleCompart) const;
	// scale data text size. -> <지표 data>인 경우.
	CSize GetMaxScaleTextSize_IndicateData(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const;
	//						 -> <대기매물>
	CSize GetMaxScaleTextSize_Sale(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const;
	//						 -> <기준선>
	CSize GetMaxScaleTextSize_BaseLine(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const;
	// scale data text size. -> <packet>인 경우.
	CSize GetMaxScaleTextSize_Packet(CDC* pDC, const CHorzScaleMaxWidthCalculatorData& calData) const;
	CSize GetMaxScaleTextSize_Packet(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const;
	CSize GetMaxScaleTextSize_Packet(CDC* pDC, CPacket* pPacket, const CVertScaleMaxWidthCalculatorData& calData) const;

	bool GetPacketMinMax(CPacket* pPacket, const CDisplayAttributes& displayAttributes, double& dMin, double& dMax) const;

	// 가장 긴 길이 가져오기
	int GetLongLength(CIndicatorInfo* pIndicatorInfo) const;
	int GetMaxPacketDataLength( const CString& strGraphName, CSubGraphPacketList* pSubGraphPacketList,
								const CDisplayAttributes& displayAttributes, const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType,
								const CDisplayDataMinMax &p_displaySubGraphMinMax) const;
	POSITION GetDrawingDataLengthStartPosition(const CString& strGraphName, CSubGraphPacketList* pSubGraphPacketList) const;
	int GetDrawingDataLength( CPacket* pPacket, const CDisplayAttributes& displayAttributes, const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType) const;
	int GetDrawingDataLength_WithMinMax( CPacket* pPacket, const CDisplayAttributes& displayAttributes, const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType,
											const CDisplayDataMinMax &p_displaySubGraphMinMax) const;
	int GetDrawingDataLength_Text(CPacket* pPacket) const;
	int GetDrawingDataLength_Number(CPacket* pPacket, const CDisplayAttributes& displayAttributes, const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType) const;
	int GetDrawingDataLength_Number_WithMinMax(CPacket* pPacket, const CDisplayAttributes& displayAttributes, const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType,
											const CDisplayDataMinMax &p_displaySubGraphMinMax) const;
	bool GetDrawingData_Number(CPacket* pPacket, const CDisplayAttributes& displayAttributes, CString& strData, CString& strFormatTrans) const;
	bool GetDrawingData_Number_WithMinMax(CPacket* pPacket, const CDisplayAttributes& displayAttributes, CString& strData, CString& strFormatTrans,
											const CDisplayDataMinMax &p_displaySubGraphMinMax) const;
	//double GetLongLengthData(const double& dData1, const double& dData2, const CString& dataType) const;
	int GetDataLength(const CString& strData) const;
	int GetFormatLength(const CString& strFormat, const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType) const;
	
	// packet 을 가져온다.
	CPacket* GetVertScalePacket(CSubGraphPacketList* pSubGraphPacketList, const CString& strPacketName) const;
	CPacket* GetVertScalePacket(CPacketList* pPacketList, const CString& strPacketNames) const;
	// <자료일자> packet의 처음 data와 type를 가져온다.
	bool GetDatePacketHeadDataAndType_DataDate(CPacket* pPacket, CString& headPacketData, CString& dataType) const;

	CSize GetMaxScaleTextSize(CDC* pDC, const int nLength, const CScaleBaseData::TEXTCOMPARTTYPE textCompart = CScaleBaseData::HORZ_TEXT_COMPART) const;
	CSize GetMaxScaleTextSize(const CSize& textSize, const int nLength, const CScaleBaseData::TEXTCOMPARTTYPE textCompart = CScaleBaseData::HORZ_TEXT_COMPART) const;

	// 오른쪽에 scale 영역에서 쓰는 부분으로 <현재가> 관련 부분
	//bool GetPrices(const CVertScaleMaxWidthCalculatorData& calData, double& dPrevPrice, double& dCurPrice) const;
	//CString GetCurrentPriceText(const double& dCurPrice) const;
	//CString GetRateText(const double& dPrevPrice, const double& dCurPrice) const;

	// (2008/6/13 - Seung-Won, Bae) for Support BuySell Overhanging Supply.
	CSize GetMaxScaleTextSize_Sale2(CDC* pDC, const CVertScaleMaxWidthCalculatorData& calData) const;
};

#endif // !defined(AFX_DRAWINGREGIONCALCULATOR_H__16FD5E76_FAA3_4228_8E7F_E9BAF4B69903__INCLUDED_)
