// CalculatorData.h: interface for the CCalculatorData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCULATORDATA_H__1679F9A8_3B3F_428D_AA4F_DBA441F7A1B4__INCLUDED_)
#define AFX_CALCULATORDATA_H__1679F9A8_3B3F_428D_AA4F_DBA441F7A1B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/BaseData.h"

class CPacket;

// min, max를 구하기 위해 필요한 조건값 (가로)
class CHorzDisplayDataMinMaxInputData
{
public:
	CHorzDisplayDataMinMaxInputData(CPacket* pPacket, 
		const CString& strPacketName, const CDisplayAttributes& displayAttributes);

	void SetPacket(CPacket* pPacket);
	void SetPacketName(const CString& strPacketName);
	void SetDisplayAttributes(const CDisplayAttributes& displayAttributes);

	CPacket* GetPacket() const;
	CString GetPacketName() const;
	CDisplayAttributes GetDisplayAttributes() const;

private:
	CPacket* m_pPacket;
	CString m_strPacketName;
	CDisplayAttributes m_displayAttributes;
};


// min, max를 구하기 위해 필요한 조건값 (세로)
class CVertDisplayDataMinMaxInputData
{
public:
	CVertDisplayDataMinMaxInputData& operator=(const CVertDisplayDataMinMaxInputData& data);

public:
	CVertDisplayDataMinMaxInputData();
	CVertDisplayDataMinMaxInputData( const CString & p_strIndicatorName, const CString& GraphName, const CString& scaleData, const CDisplayAttributes& displayAttributes);

	void SetSubGraphData(CSubGraphData* pSubGraphData);
	void SetSubGraphPacketList(CSubGraphPacketList* pSubGraphPacketList);

	CString GetGraphName() const;
	CString GetIndicatorName( void) const;
	CString GetScaleData() const;
	CGraphBaseData::GRAPHTYPE GetSubGraphType() const;
	CGraphStyle GetSubGraphStyle() const;
	CDisplayAttributes GetDisplayAttributes() const;
	CSubGraphData* GetSubGraphData() const;
	CSubGraphPacketList* GetSubGraphPacketList() const;
	void SetDataStartEndInBlock(int nStart, int nEnd);	// yiilyoul@magicn.com - X-ScaleManager (2006.02.06)

private:
	// 구하고자 하는 graph 정보
	CString m_strGraphName;
	CString m_strIndicatorName;
	CString m_strScaleData;
	CDisplayAttributes m_displayAttributes;
	CSubGraphData* m_pSubGraphData;
	CSubGraphPacketList* m_pSubGraphPacketList;
};


// --------------------------------------------------------------------------------------
// CDrawingRegion/CGraphRegions 을 구하기 위해 필요한 class
class CDrawingRegionCalData
{
public:
	CDrawingRegionCalData(
		const CRect& blockRegion,
		const int graphRegionFrameThickness, 
		const bool bIsGraphTextDrawCheck,
		const bool bIsGraphTextDraw, 
		const bool bIsGraphTitleDraw,
		const bool bIsVertScale, 
		const CHorzScaleRegionHeight& horzScaleRegionHeight,
		const CVertScaleRegionWidth& vertScaleRegionWidth,
		const BOOL p_bBlockMoveDelete
		);

	void SetGraphTitleDraw(const bool bIsGraphTitleDraw);
	void SetMaxVertScaleRegionWidth(const int maxVertScaleRegionWidth);
	void SetDisplayAttributes(const CDisplayAttributes& displayAttributes);

	CRect GetBlockRegion() const;
	int GetGraphRegionFrameThickness() const;
	bool IsGraphTitleDraw() const;
	bool IsGraphTextDrawCheck() const;
	bool IsGraphTextDraw() const;
	bool IsVertScale() const;
	int GetMaxVertScaleRegionWidth() const;
	const CHorzScaleRegionHeight& GetHorzScaleRegionHeight() const;
	const CVertScaleRegionWidth& GetVertScaleRegionWidth() const;
	CDisplayAttributes GetDisplayAttributes() const;

private:
	CRect m_blockRegion;				// block region
	int m_nGraphRegionFrameThickness;	// graph region의 테두리 굵기
	bool m_bIsGraphTextDrawCheck;		// graph text를 쓸지를 체크
	bool m_bIsGraphTextDraw;			// graph text를 쓸지의 유무
	bool m_bIsGraphTitleDraw;			// graph 타이틀을 쓸지의 유무
	bool m_bIsVertScale;				// vert scale 의 유무
	int m_nMaxVertScaleRegionWidth;		// 세로 scale의 최대 폭
	CHorzScaleRegionHeight m_horzScaleRegionHeight; // 가로 scale의 높이
	CVertScaleRegionWidth m_vertScaleRegionWidth;	// 세로 scale의 폭
	CDisplayAttributes m_displayAttributes;

// (2006/10/13 - Seung-Won, Bae) Manage Block Move & Delete Tool Flag for Position of Graph Drawing
protected:
	BOOL	m_bBlockMoveDelete;
public:
	BOOL	GetBlockMoveDeleteFlag( void) const	{	return m_bBlockMoveDelete;	}
};


// --------------------------------------------------------------------------------------
// 가로 scale text 크기 구하기 위해 필요한 class
class CHorzScaleMaxWidthCalculatorData
{
public:
	CHorzScaleMaxWidthCalculatorData(CScaleCompart* pScaleCompart, CPacket* pPacket, 
		const CString& strPacketName, const CDisplayAttributes& displayAttributes);

	CScaleCompart* GetScaleCompart() const;
	CPacket* GetPacket() const;
	CString GetPacketName() const;
	CDisplayAttributes GetDisplayAttributes() const;

private:
	CScaleCompart* m_pScaleCompart;
	CPacket* m_pPacket;
	CString m_strPacketName;
	CDisplayAttributes m_displayAttributes;
};

// 세로 scale text 크기 구하기 위해 필요한 class
class CVertScaleMaxWidthCalculatorData
{
public:
	// (2008/6/13 - Seung-Won, Bae) for Support BuySell Overhanging Supply.
	CVertScaleMaxWidthCalculatorData(const bool bIsRight, const CDisplayAttributes& displayAttributes, const CVerticalScale& vertScale, BOOL p_bIsDualScale);
	
	void SetCalcRiseFallRate_CurData(const int nCalcRiseFallRate_CurData);
	void SetGraphName(const CString GraphName);
	void SetSubGraphName(const CString& subGraphName);
	void SetIndicatorInfo(CIndicatorInfo* pIndiInfo);
	void SetSubGraphPacketList(CSubGraphPacketList* pSubGraphPacketList);

	bool DoesRight() const;
	int GetCalcRiseFallRate_CurData() const;
	CVerticalScale GetVerticalScale() const;
	CVerticalScale& GetVerticalScale();
	CString GetGraphName() const;
	CString GetSubGraphName() const;
	CString GetVertScalePacketName() const;
	CDisplayAttributes GetDisplayAttributes() const;
	CIndicatorInfo* GetIndicatorInfo() const;
	CSubGraphPacketList* GetSubGraphPacketList() const;

	// (2008/6/13 - Seung-Won, Bae) for Support BuySell Overhanging Supply.
	void	SetDualScale( BOOL p_bIsDualScale);
	BOOL	IsDualScale( void) const;

private:
	bool m_bIsRight;
	int m_nCalcRiseFallRate_CurData;	// 전일가 대비 현재가 등락률 (현재가)
	CVerticalScale m_vertScale;
	CString m_strGraphName;
	CString m_strSubGraphName;
	CDisplayAttributes m_displayAttributes;
	CIndicatorInfo* m_pIndicatorInfo;
	CSubGraphPacketList* m_pSubGraphPacketList;

	// (2008/6/13 - Seung-Won, Bae) for Support BuySell Overhanging Supply.
	BOOL	m_bIsDualScale;

// (2009/1/17 - Seung-Won, Bae) Support Min/Max cache. Do not make Min/Max every time of updating data.
//		It is initialized in CSubGraph::GetDisplayDataMinMax() for CMainBlockImp::CalculateDataForDrawing().
protected:
	CDisplayDataMinMax			m_displayDataMinMax;
public:
	void						SetSubGraphDisplayMinMax( const CDisplayDataMinMax &p_displayDataMinMax);
	const CDisplayDataMinMax &	GetSubGraphDisplayMinMax( void) const		{	return m_displayDataMinMax;		}
};

#endif // !defined(AFX_CALCULATORDATA_H__1679F9A8_3B3F_428D_AA4F_DBA441F7A1B4__INCLUDED_)
