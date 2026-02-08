// VertScaleDrawer.h: interface for the CVertScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VERTSCALEDRAWER_H__BF2990E3_A083_45D4_B991_CC94C3AD866D__INCLUDED_)
#define AFX_VERTSCALEDRAWER_H__BF2990E3_A083_45D4_B991_CC94C3AD866D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Drawer.h"

class CDrawingVertScaleData;
class CScaleDrawingRegion;
class CDisplayDataMinMax;
class CVertScaleRegion;
class CVerticalScale;

class CVertScaleDrawer : public CDrawer
{
public:
	CVertScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);

public:	
	virtual void DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale);
	virtual void DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale);

	bool GetDrawingEnvironment(const CDisplayDataMinMax& displayDataMinMax, const CString& strPacketType, 
		const int totalCount, double& dGap, int& start, int& end, bool& bround) const;

protected:
	// 그리기 위해 필요한 값을 가져온다.
	bool GetDrawingEnvironment(CDC* pDC, const CRect& scaleTextRegion, const CDisplayDataMinMax& displayDataMinMax,
		const CString& dataType, int& start, int& end, double& dGap, bool& bround);

	// packet type
	CString GetPacketDataType(const CString& strPacketType, const double& dGap) const;
	// scale text의 그리는 위치
	CPoint GetTextPosition(const int org_x, const int y, const CSize& textSize, const int textPos = DT_CENTER) const;
	bool GetVertScaleAbsolutePosition_y(const CRect& region, const double relative_y, 
		const CDisplayDataMinMax& displayDataMinMax, const bool bIsLog, int& nowPt_y);
	// 소숫점 이하 자릿수
	int GetRoundIndexDecimalDown(const CString& strPacketType) const;

	// scale text 가져오기.
	CString GetStrData(const double& dData, const CString& dataType, const bool bround = false, const int nRoundLength = 0) const;
	// type reset하기
	// (2007/2/21 - Seung-Won, Bae) Support Not-Auto Scale Type.
	CString GetFormatTrans( const CDisplayDataMinMax& displayDataMinMax, const CString& dataType, BOOL p_bAutoScaleType);

	// scale draw
	// 호가단위, 기준가 사용 - ojtaso (20070917)
	void DrawTextOrGrid(CDC* pDC, const double orgY, const int typeRegionY, const int start, const int end, 
		const double& dGap, const bool bround, const CDrawingVertScaleData& drawingVertScaleData, const CVerticalScale& vertScale,
		const CRect& region1, const CRect& region2, const CString& dataOrgType, const CString& dataViewType, const bool bIsSignDraw/* = true*/, const double dStandardValue = 0.);

	// graph type 그리기
	int DrawType(CDC* pDC, const CRect& typeTextRegion, const CString& dataType, const bool bround, const CScaleBaseData::VERTSCALEUNITTYPE eVertScaleUnitType, const COLORREF color);
	// gaid 그리기
	void DrawGrid( CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleBaseData::SCALEGRIDTYPE gridType, const int pt_y, const int p_nActiveFlag);
	// 눈금 그리기
	void DrawScale(CDC * pDC, const CRect& region, 
				   double dGap, double orgY, int nstart, int nend, 
				   const CDrawingVertScaleData& drawingVertScaleData);

	void GetStartAndEnd(const CDisplayDataMinMax& displayDataMinMax, const int totalCount, const double& dGap, int& start, int& end) const;
	bool GetBround(const CDisplayDataMinMax& displayDataMinMax) const;
	void GetGapEnvironment(const CString& dataType, double& dGapMM, double& dGapInGap) const;
	int GetCount(CDC* pDC, const int height) const;
	// scale 눈금 긋는다
	void DrawScaleLine(CDC* pDC, const CRect& region, const int nScaleLineCount, 
		const CDisplayDataMinMax& displayDataMinMax, const double dOrgY, const double& dGap, 
		const bool bIsLog, const bool bIsInvert, const bool bIsIncrease);
	void DrawScaleLine(CDC* pDC, const CRect& region, const int nPt_y, 
		const bool bIsInvert, const int nGap1, const int nGap2);

private:
	double GetRealData(const bool bIsLog, const double& dData) const;
	// scale 간의 간격구하기
	int GetVertScaleHight(CDC* pDC) const;

	// scale text의 그리는 위치
	int GetVertScalePosition_x(const int right_pos, const int dataLength) const;
	// 잔눈금 갯수
	int GetScaleLineCount(const double& dGap) const;

	// GetDrawingEnvironment() 을 위한 함수들
//	int GetCount(CDC* pDC, const int height) const;
//	void GetStartAndEnd(const CDisplayDataMinMax& displayDataMinMax, const int totalCount, const double& dGap, int& start, int& end) const;
//	void GetGapEnvironment(const CString& dataType, double& dGapMM, double& dGapInGap) const;
	bool GetGap(const CDisplayDataMinMax& displayDataMinMax, const CString& dataType, const int totalCount, double& dGap) const;

	bool IsScaleDataDrawing(const CRect& maxRegion, const int nTypePosition, const int nDataPosition) const;

	// 호가단위, 기준가 사용 - ojtaso (20070917)
	void DrawTextOrGrid_PlusAndMinusSign(CDC* pDC, const double orgY, const int typeRegionY, const int start, const int end, 
		const double& dGap, const bool bround, const CDrawingVertScaleData& drawingVertScaleData, const CVerticalScale& vertScale,
		const CRect& noFrameGraphRegion, const CRect& region1, const CRect& region2, const CString& dataOrgType, const CString& dataViewType, const bool bIsSignDraw,
		const double dStandardValue);
	// 호가단위, 기준가 사용 - ojtaso (20070917)
	void DrawTextOrGrid_PlusOrMinusSign(CDC* pDC, const double orgY, const int typeRegionY, const int start, const int end, 
		const double& dGap, const bool bround, const CDrawingVertScaleData& drawingVertScaleData, const CVerticalScale& vertScale,
		const CRect& noFrameGraphRegion, const CRect& region1, const CRect& region2, const CString& dataOrgType, const CString& dataViewType, const bool bIsSignDraw,
		const double dStandardValue);
	// 최대값 최소값 적어주기
	void WriteValueMinMax(CDC * pDC, const CRect region, const bool bround, const CString& dataType, const CDrawingVertScaleData& drawingVertScaleData);
};

#endif // !defined(AFX_VERTSCALEDRAWER_H__BF2990E3_A083_45D4_B991_CC94C3AD866D__INCLUDED_)
