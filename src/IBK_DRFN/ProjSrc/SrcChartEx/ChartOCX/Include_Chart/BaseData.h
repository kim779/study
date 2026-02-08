// BaseData.h: interface for the BaseData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEDATA_H__8BA084DD_9B9D_4FBF_8C93_FC178312B669__INCLUDED_)
#define AFX_BASEDATA_H__8BA084DD_9B9D_4FBF_8C93_FC178312B669__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _MSC_VER
#pragma warning( disable: 4786 )
#endif

#include "ScaleBaseData.h"
#include "GraphData.h"
// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
#include "BlockBaseEnum.h"

class CPacket;
class CSubGraphPacketList;
class CIndicatorList;

// 화면에 보일 data Setting.
class CDisplayAttributes
{
public:
	CDisplayAttributes& operator=(const CDisplayAttributes& data);
	bool operator==(const CDisplayAttributes& data);

public:
	CDisplayAttributes();
	CDisplayAttributes(const int start, const int end);

	// 화면에 보일 data Setting.
	void SetDataStartInBlock(const int start);
	void SetDataEndInBlock(const int end);

	int GetDataStartInBlock() const;
	int GetDataEndInBlock() const;
	int GetDataCountInBlock() const;

private:
	int m_nDataStartInBlock;	// block안의 보여질 처음 data
	int m_nDataEndInBlock;		// block안의 보여질 마지막 data
};


// scale region의 환경
class CScaleRegionAttributes
{
public:
	CScaleRegionAttributes();

	void SetMinHorzScaleRegionHeight(const int minHorzScaleRegionHeight);
	void SetMinVertScaleRegionWidth(const int minVertScaleRegionWidth);
	void SetMaxVertScaleRegionWidth(const int maxVertScaleRegionWidth);

	int GetMinHorzScaleRegionHeight() const;
	int GetMinVertScaleRegionWidth() const;
	int GetMaxVertScaleRegionWidth() const;

private:
	int m_nMinHorzScaleRegionHeight; // 가로 scale의 최소 높이(y축 길이)
	int m_nMinVertScaleRegionWidth;  // 세로 scale의 최소 폭(x축 길이)
	int m_nMaxVertScaleRegionWidth;  // 세로 scale의 최대 폭(x축 길이)
};


// --------------------------------------------------------------------------------------
// graph 와 관련된 영역을 저장하는 class

// (2006/10/10 - Seung-Won, Bae) Encapsulte the no Frame Region to complement Graph's OverFlow Error.
//		Caution! The Correct Method is the Fixing in Drawing Logic of all graph.
//					But, It is too Heavy Work.
//					Instead, Use the complemented Region from the Real Region Value.
class CGraphRegionsBase
{
public:
	CGraphRegionsBase();
private:
	int		m_nFrameThickness;
	CRect	m_rctFrameRegion;	// graph 영역(frame을 제외한 영역)
	CRect	m_rctNoFrameRegion;
	CRect	m_rctNoFrameAndMargin;
protected:	// for Guarantee the Real NoFrame Region without complement for Graph's OverFlow Error.
	friend class CGraphRegionCalculator;
	// (2009/1/20 - Seung-Won, Bae) Support Reverse.
	void	SetFrameRegion( const CRect& region, const int p_nFrameThickness, BOOL p_bReverse);
public:
	CRect	GetFrameRegion( void) const			{	return m_rctFrameRegion;	}
	CRect	GetNoFrameRegion() const			{	return m_rctNoFrameRegion;	}
	CRect	GetNoFrameRegionAndMargin() const	{	return m_rctNoFrameAndMargin;	}
};

class CGraphRegions : public CGraphRegionsBase
{
public:
	CGraphRegions();

private:
	int		m_nFontSize;		// font의 size
	int		m_nLineCount;		// 타이틀 줄 수 : 복수종목 - ojtaso (20070227)
	bool	m_bIsTitle;			// graph title 존재여부
	bool	m_bIsText;			// 최고가/최저가를 적을 지의 여부
	bool	m_bIsVertScale;		// vert scale 의 유무
	BOOL	m_bBlockMoveDelete;	// (2006/10/13 - Seung-Won, Bae) Manage Block Move & Delete Tool Flag for Position of Graph Drawing

public:
	bool				IsTitle() const;
	bool				IsText() const;
	void				SetText( const bool bIsText);
	void				SetTitle( const bool bIsTitle);
	void				SetFontSize(const int nSize);
	void				SetVertScale( const bool bIsVertScale);
	void				OffsetFontSize( const int nSize);
	void				SetBlockMoveDeleteFlag( const BOOL p_bBlockMoveDelete)	{	m_bBlockMoveDelete = p_bBlockMoveDelete;	}
	// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
	void				SetTitleLine(const int nLine);
	void				OffsetTitleLine(const int nLine);
	CRect				GetTitleRegion() const;
	CRect				GetDrawingRegion( const CGraphBaseData::GRAPHTYPE eGraphType = CGraphBaseData::GTNONE, const CGraphStyle& graphStyle = CGraphStyle( -1)) const; // title과 textHeight와 오른쪽 여백을 제외한 영역
	// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
	int					GetTitleLineCount() const;
};


// graph, scale영역을 저장하는 class
class CDrawingRegion
{
public:
	void SetGraphRegions(const CGraphRegions& graphRegions);
	void SetHorzScaleRegion(const CRect& topRegion, const CRect& bottomRegion);
	void SetVertScaleRegion(const CRect& leftRegion, const CRect& rightRegion);
	void OffsetDrawingRegion(const int nFontSize);

	CGraphRegions& GetGraphRegions();
	CGraphRegions GetGraphRegions() const;
	CRect GetGraphFrameRegion( void) const;
	CHorzScaleRegion GetHorzScaleRegion() const;
	CVertScaleRegion GetVertScaleRegion() const;

private:
	CGraphRegions m_graphRegions; // graph 영역
	CHorzScaleRegion m_horzScaleRegion;	// 가로 scale 영역
	CVertScaleRegion m_vertScaleRegion;	// 세로 scale 영역
};


// --------------------------------------------------------------------------------------
// graph를 그리기 위한 data
class CPacketList;
class CMainBlock;
class CDrawingGraphData
{
public:
	// (2004.04.01, 배승원) Default Indicator List를 Static으로 관리하여, 반복 Loading을 제한한다.
	//		Static내에 CIndicatorList의 Pointer를 관리하지 않는다. (CIndicatorList는 Static이 아니다.)
	//		CIndicatorInfo를 의존하지 않고 CIndicatorList를 접근할 수 있도록 관리한다.
	// (2007/2/22 - Seung-Won, Bae) Support Base Text Option
	CDrawingGraphData(CIndicatorList* pIndicatorList, 
		const CDrawingOption::DRAWINGORDER eDrawingOrderType, 
		const CGraphRegions& graphRegions,
		const CRect& scaleLeftTextRegion,		// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
		const CRect& scaleRightTextRegion,		// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
		const CDisplayDataMinMax& displayDataHorzMinMax, const CString& horzScalePacketName, 
		const CEtcGraphColor& etcGraphColor, const CDisplayAttributes& displayAttributes, 
		const bool bIsVertScaleLogType, const bool bIsVertScaleInvertType, 
		const bool bIsConditionTitleDraw, const bool bIsDataTitleDraw, CPacketList *p_pPacketList,
		BOOL p_bWithBaseText,
		const bool bDrawBaseLine,	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
		HWND p_hOcxWnd,
		int	p_nColumn,
		CChartInfo::CHART_MODE p_eChartMode,	// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
		CMainBlock *pMainBlock);

	void SetDisplayDataVertMinMax(const CDisplayDataMinMax& displayDataVertMinMax);
	void SetGraphName(const CString& GraphName);
	void SetPacketNames(const CString& packetNames);
	void SetTitleDraw(const bool bIsConditionTitleDraw, const bool bIsDataTitleDraw);
	void SetConditionTitleDraw(const bool bIsConditionTitleDraw);
	void SetDataTitleDraw(const bool bIsDataTitleDraw);
	void SetGraphDataDrawingData(const CGraphDataDrawingData& graphDataDrawingData);
	void SetPriceChartColor(CGraphColor* pGraphColor);
	void SetIndicatorInfo(CIndicatorInfo* pIndiInfo);
	void SetSubGraphPacketList(CSubGraphPacketList* pSubGraphPacketList);
	void SetPacketList( CPacketList *p_pPacketList)			{	m_pPacketList = p_pPacketList;	}

	CDrawingOption::DRAWINGORDER GetDrawingOrderType() const;
	bool DoesVertScaleLogType() const;
	bool DoesVertScaleInvertType() const;
	const CGraphRegions& GetGraphRegions() const;
	CRect GetScaleTextRegion() const;
	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	CRect GetLeftScaleTextRegion() const;
	CRect GetRightScaleTextRegion() const;
	CDisplayDataMinMax GetDisplayDataHorzMinMax() const;
	CDisplayDataMinMax GetDisplayDataVertMinMax() const;
	CString GetGraphName() const;
	CString GetPacketName() const;
	CString GetHorzScalePacketName() const;
	bool DoesConditionTitleDraw() const;
	bool DoesDataTitleDraw() const;
	CGraphDataDrawingData GetGraphDataDrawingData() const;
	CDisplayAttributes GetDisplayAttributes() const;
	CEtcGraphColor GetEtcGraphColor() const;
	CGraphColor* GetPriceChartColor() const;
	CIndicatorList* GetIndicatorList() const;
	CIndicatorInfo* GetIndicatorInfo() const;
	CSubGraphPacketList* GetSubGraphPacketList() const;
	CPacketList *GetPacketList( void) const					{	return m_pPacketList;	}

	// 지표 뒤집기 - ojtaso (20071023) 
	void SetVertScaleInvertType(const bool bIsVertScaleInvertType);

	// (2007/2/22 - Seung-Won, Bae) Support Base Text Option
	BOOL	GetWithBaseText( void) const				{	return m_bWithBaseText;	}
	void	SetWithBaseText( BOOL p_bWithBaseText)		{	m_bWithBaseText = p_bWithBaseText;	}
	CString GetIndicatorName() const;

	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	void	SetVertScaleDisplayAttributes(CScaleBaseData::VERTSCALEPOSITION eVertPos,
											BOOL bTextDrawing);
	CScaleBaseData::VERTSCALEPOSITION GetVertScalePosition() const { return m_eVertPos; }
	BOOL	GetTextDrawing() const { return m_bTextDrawing; }

	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	bool	GetDrawBaseLine() const { return m_bDrawBaseLine; }

private:
	CDrawingOption::DRAWINGORDER m_eDrawingOrderType;
	CGraphRegions m_graphRegions;
	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	CRect m_scaleLeftTextRegion;
	CRect m_scaleRightTextRegion;
	CDisplayDataMinMax m_displayDataHorzMinMax;
	CDisplayDataMinMax m_displayDataVertMinMax;
	bool m_bIsVertScaleLogType;
	bool m_bIsVertScaleInvertType;
	CString m_strGraphName;
	CString m_strPacketNames;
	CString m_strHorzScalePacketName;
	bool m_bIsConditionTitleDraw;
	bool m_bIsDataTitleDraw;
	CGraphDataDrawingData m_drawingData;	// Graph에 data 표시
	CDisplayAttributes m_displayAttributes;
	CEtcGraphColor m_etcGraphColor;
	CGraphColor* m_pPriceChartColor;
	CIndicatorInfo* m_pIndiInfo;
	CSubGraphPacketList* m_pSubGraphPacketList;
	// (2004.04.01, 배승원) Default Indicator List를 Static으로 관리하여, 반복 Loading을 제한한다.
	//		Static내에 CIndicatorList의 Pointer를 관리하지 않는다. (CIndicatorList는 Static이 아니다.)
	//		CIndicatorInfo를 의존하지 않고 CIndicatorList를 접근할 수 있도록 관리한다.
	CIndicatorList* m_pIndicatorList;
	CPacketList *m_pPacketList;
	// (2007/2/22 - Seung-Won, Bae) Support Base Text Option
	BOOL	m_bWithBaseText;
	// Y Scale상에 종가 표시 수정 - ojtaso (20070807)
	CScaleBaseData::VERTSCALEPOSITION m_eVertPos;
	BOOL	m_bTextDrawing;

	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	bool	m_bDrawBaseLine;

// (2008/1/16 - Seung-Won, Bae) for MultiLanguage
protected:
	HWND	m_hOcxWnd;
public:
	HWND	GetOcxHwnd( void) const		{	return m_hOcxWnd;	}

// (2009/2/13 - Seung-Won, Bae) for Multi-Column.
protected:
	int		m_nColumnIndex;
public:
	int		GetColumnIndex( void) const	{	return m_nColumnIndex;	}

// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
protected:
	CChartInfo::CHART_MODE	m_eChartMode;
public:
	CChartInfo::CHART_MODE	GetChartMode( void) const		{	return m_eChartMode;	}

// (2010/4/18 - Seung-Won, Bae) MainBlock to search price chart on Volume Chart Drawing.
protected:
	CMainBlock *	m_pMainBlock;
public:
	CMainBlock *	GetMainBlock( void) const		{	return m_pMainBlock;	}
};


// 가로 scale을 그리기 위한 data
class CDrawingHorzScaleData
{
public:
	// 미래영역 스케일을 그릴것인지 : ojtaso (20070724)
	CDrawingHorzScaleData(const CRect& drawingGraphRegion, const CGraphBaseData::GRAPHTYPE& eHeadGraphType, 
		const CGraphStyle& headGraphStyle, const CGraphRegionColor& graphRegionColor, 
		const CScaleColor& scaleColor, const CDisplayAttributes& displayAttributes, 
		const CDisplayDataMinMax& displayDataMinMax, CPacket* pPacket, BOOL bDrawFutureScale, int p_nColumn);

	CRect GetDrawingGraphRegion() const;
	CGraphBaseData::GRAPHTYPE GetHeadGraphType() const;
	CGraphStyle GetHeadGraphStyle() const;
	CScaleBaseData::HORZSCALEDRAWERTYPE GetScaleDrawerType() const;
	CScaleColor GetScaleColor() const;
	CGraphRegionColor GetGraphRegionColor() const;
	COLORREF GetCompartLineColor() const;
	CDisplayAttributes GetDisplayAttributes() const;
	CDisplayDataMinMax GetDisplayDataMinMax() const;
	CPacket* GetPacket() const;
	// 미래영역 스케일을 그릴것인지 : ojtaso (20070724)
	BOOL GetDrawFutureScale() const;
	void SetDrawFutureScale(const BOOL bDrawFutureScale);

	void SetScaleDrawerType(const CScaleBaseData::HORZSCALEDRAWERTYPE& drawType);

private:
	CRect m_drawingGraphRegion;
	CGraphBaseData::GRAPHTYPE m_eHeadGraphType;
	CGraphStyle m_headGraphStyle;
	CScaleBaseData::HORZSCALEDRAWERTYPE m_TYPEScaleDrawer; // scale draw type
	CScaleColor m_scaleColor;				// scale color
	CGraphRegionColor m_graphRegionColor;	// graph 바탕색
	CDisplayAttributes m_displayAttributes;	// block에 보일 위치
	CDisplayDataMinMax m_displayDataMinMax;	// 가로(x)의 min, max
	CPacket* m_pPacket;
	BOOL m_bDrawFutureScale;				// 미래영역 스케일을 그릴것인지 : ojtaso (20070724)

// (2009/2/13 - Seung-Won, Bae) for Multi-Column.
protected:
	int		m_nColumnIndex;
public:
	int		GetColumnIndex( void) const		{	return m_nColumnIndex;	}
};


class CIndicatorInfo;

// 세로 scale을 그리기 위한 data
class CDrawingVertScaleData
{
public:
	CDrawingVertScaleData(	const CRect& noFrameGraphRegion,
							const CRect& drawingGraphRegion, 
							const CGraphRegionColor& graphRegionColor,
							const CScaleColor& scalrColor, 
							const bool bIsVertScaleInvertType,
							const bool bIsVertScaleLogType,
							const CDisplayAttributes& displayAttributes,
							CPacketList* pPacketList);	// 호가단위, 기준가 사용 - ojtaso (20070917)

	CRect GetNoFrameGraphRegion() const;
	CRect GetDrawingGraphRegion() const;
	bool GetVertScaleInvertType() const;
	bool GetVertScaleLogType() const;
	CString GetSubGraphNames() const;
	CString GetPacketNames() const;
	CString GetIndicatorName() const;
	CScaleBaseData::VERTSCALEDRAWERTYPE GetScaleDrawerType() const;
	CScaleColor GetScaleColor() const;
	CGraphRegionColor GetGraphRegionColor() const;
	COLORREF GetCompartLineColor() const;
	CDisplayAttributes GetDisplayAttributes() const;
	CDisplayDataMinMax GetDisplayDataMinMax() const;
	CIndicatorInfo* GetIndicatorInfo() const;
	CPacket* GetPacket() const;
	// 호가단위, 기준가 사용 - ojtaso (20070917)
	// (2008/4/28 - Seung-Won, Bae) for VolumeForSafe
	double GetHogaUnit( BOOL bVolumeForSale = FALSE) const;
	// 호가단위, 기준가 사용 - ojtaso (20070917)
	double GetStandardValue() const;

	void SetSubGraphNames(const CString& subGraphNames);
	void SetPacketNames(const CString& packetNames);
	void SetIndicatorName(const CString& IndicatorName);
	void SetScaleDrawerType(const CScaleBaseData::VERTSCALEDRAWERTYPE& drawType);
	void SetDisplayDataMinMax(const CDisplayDataMinMax& displayDataMinMax);
	void SetIndicatorInfo(CIndicatorInfo* pIndicatorInfo);
	void SetPacket(CPacket* pPacket);

	// 지표 뒤집기 - ojtaso (20071023) 
	void SetVertScaleInvertType(const bool bIsVertScaleInvertType);

private:
	CRect m_noFrameGraphRegion;
	CRect m_drawingGraphRegion;
	bool m_bIsVertScaleInvertType;
	CString m_strSubGraphNames;
	CString m_strPacketNames;
	CString m_strIndicatorName;
	CScaleBaseData::VERTSCALEDRAWERTYPE m_TYPEScaleDrawer; // scale draw type
	CScaleColor m_scaleColor;	// scale color
	CGraphRegionColor m_graphRegionColor;	// graph 바탕색
	CDisplayAttributes m_displayAttributes;	  // block에 보일 위치
	CDisplayDataMinMax m_displayDataMinMax;	  // 세로(y)의 min, max
	CIndicatorInfo* m_pIndicatorInfo;
	CPacket* m_pPacket;
	CPacketList *m_pPacketList;					// 호가단위, 기준가 사용 - ojtaso (20070917)

// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
//	CScaleBaseData::SCALEGRIDTYPE	m_eScaleGridType;
//	CScaleBaseData::SCALEGRIDTYPE GetBaseScaleGridType( void) const;
};


// --------------------------------------------------------------------------------------
// 오른쪽 스케일 영역에 그릴 text에 대한 정보
class CTextInRightScaleRegionDrawer
{
public:
	CTextInRightScaleRegionDrawer(const CString& strGraphName = "", CSubGraphPacketList* pSubGraphPacketList = NULL, 
		const CRect& region = CRect(0, 0, 0, 0), const int nCurDataType = 0, const int nDataEnd = 0);

	void SetGraphName(const CString& strGraphName);
	void SetSubGraphPacketList(CSubGraphPacketList* pSubGraphPacketList);
	void SetRegion(const CRect& region);
	void SetCurDataType(const int nCurDataType);
	void SetDataEnd(const int nDataEnd);

	CString GetGraphName() const;
	CSubGraphPacketList* GetSubGraphPacketList() const;
	CRect GetRegion() const;
	int GetCurDataType() const;
	int GetDataEnd() const;

	bool IsTextDrawing() const;

private:
	CString m_strGraphName;
	CSubGraphPacketList* m_pSubGraphPacketList;
	CRect m_region;
	int m_nCurDataType;
	int m_nDataEnd;
};

// --------------------------------------------------------------------------------------
// 20081007 JS.Kim	사용자시간대 선택
// graph에서 사용하는 시간대 정보
class	CTimeDiffInfo
{
public:
	CTimeDiffInfo()
	{
		m_nIndex = -1;
		m_nDiffTime = 0;
	}
	CTimeDiffInfo(CString strName, int nDiffTime)
	{
		m_nIndex = -1;
		m_strName = strName;
		m_nDiffTime = nDiffTime;
	}

public:
	int				m_nIndex;
	CString			m_strName;
	int				m_nDiffTime;
};

#endif // !defined(AFX_BASEDATA_H__8BA084DD_9B9D_4FBF_8C93_FC178312B669__INCLUDED_)
