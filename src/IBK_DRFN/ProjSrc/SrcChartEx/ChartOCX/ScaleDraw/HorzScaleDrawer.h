// HorzScaleDrawer.h: interface for the CHorzScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HORZSCALEDRAWER_H__2A95AF4E_41EC_4386_9397_18C4E1DD5FF0__INCLUDED_)
#define AFX_HORZSCALEDRAWER_H__2A95AF4E_41EC_4386_9397_18C4E1DD5FF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Drawer.h"

class CDrawingHorzScaleData;
class CDisplayAttributes;
class CDisplayDataMinMax;
class CHorzScaleRegion;
class CHorizontalScale;
class CScaleCompart;
class CPacket;
class CXScaleManager;

class CHorzScaleDrawer : public CDrawer
{
public:
	CHorzScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);
public:
	virtual void DrawTextOrGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CRect& scaleRegion, const CHorizontalScale& horzScale) = 0;
	virtual bool DrawTextOrGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CRect& scaleRegion, const CHorizontalScale& horzScale, CXScaleManager* pxScaleManager, const char *p_szRQ)
	{
		return false;
	}

	virtual void DrawTextOrGrid_Both(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale) = 0;
	virtual bool DrawTextOrGrid_Both(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale, CXScaleManager* pxScaleManager, const char *p_szRQ)
	{
		return false;
	}

 
	virtual void DrawGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorizontalScale& horzScale) = 0;
	virtual bool DrawGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, const CHorzScaleRegion& horzScaleRegion, const CHorizontalScale& horzScale, CXScaleManager* pxScaleManager, const char *p_szRQ)
	{
		return false;
	}

protected:
	bool IsKoreanStyle(const CString& packetType) const;
	bool IsTextPosion(const int nPrevPos, const int nCurPos, const int nIndex, BOOL bReverse = FALSE) const;

	// text를 그리기 위한 값들을 가져온다.
	void GetDrawingEnvironment(const CRect& region, const CDisplayAttributes& displayAttributes, 
		const int textHeight, double& onePtWidth, int& posPt_y) const;
	void GetDrawingEnvironment(const CRect& region1, const CRect& region2, const CDisplayAttributes& displayAttributes, 
		const int textHeight, double& onePtWidth, int& posPt_y1, int& posPt_y2) const;
	bool GetDrawingEnvironment(CDC* pDC, CPacket* pPacket, const CRect& scaleTextRegion, const CDisplayDataMinMax& displayDataMinMax,
		const double& dOnPtWidth, int& start, int& end, double& dGap, bool& bround, bool bStand = false) const;

	void GetStartAndEnd(const CDisplayDataMinMax& displayDataMinMax, const int totalCount, const int nGap, int& start, int& end) const;
	// 처음 text의 위치(orgX)를 가져온다.
	int GetHorzScalePosition_x(const int count, const int orgPt_x, const double& onePtWidth) const;

	// 원하는 형태의 data로 변경
	int GetChangeNumbericData(CPacket* pPacket, const int findIndex, const CString& packetType, const CString& trimStr) const;
	int GetChangeNumbericData2(const int orgData, const CString& packetType, const CString& trimStr) const;

	// int 형 data를 원하는 형태의 string형 data로 변경
	CString GetChangeStringData(const CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType, 
		const int nData, const CString& packetType, const CString& compartType, const CString& trim) const;
	CString GetChangeStringData(const CScaleBaseData::HORZSCALEDRAWERTYPE scaleDrawerType,
		const int nData, const CString& packetType, const CString& trim, const CScaleCompart& scaleCompart) const;

	void DrawMultiLineTextOut(CDC* pDC, const CPoint& point, const int textHeight, const CString& data, const CString& preData, const COLORREF color);
	void DrawMultiLineTextOut(CDC* pDC, const CPoint& point, const int textHeight, const CString& data, const COLORREF color);
	// gaid 그리기
	void DrawGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData, 
		const CScaleBaseData::SCALEGRIDTYPE gridType, const int pt_x, const int nWidth = 1);
	// text compart line 그리기
	void DrawTextCompartLine(CDC* pDC, const CRect& scaleRegion, const CPoint& nowPos, const bool bIsTopPos, const COLORREF gridColor);

	// 가로 숫자 데이타 표현
/*	void DrawTextOrGrid_PlusOrMinusSign(CDC* pDC, const int orgY, const int typeRegionY, const int start, const int end, 
		const int nGap, const bool bround, const CDrawingHorzScaleData& drawingHorzScaleData, 
		const CRect& region1, const CRect& region2, const CString& dataType);
	void DrawTextOrGrid_PlusAndMinusSign(CDC* pDC, const int orgY, const int typeRegionY, const int start, const int end, 
		const int nGap, const bool bround, const CDrawingHorzScaleData& drawingHorzScaleData, 
		const CRect& region1, const CRect& region2, const CString& dataType);

	// graph type 그리기
	int DrawType(CDC* pDC, const CRect& typeTextRegion, const CString& dataType, const bool bround, const COLORREF color);
*/

	int GetFindEndIndex(const CString& data, const CString& findData) const;

private:

	// 처음 text의 위치(orgY)를 가져온다.
	int GetHorzScalePosition_y(const int org_y, const int orgHight, const int textHeight) const;
	// x축 scale 에 맞는 gap을 구한다.
	double GetGap(CDC* pDC, CPacket* pPacket, const CRect& scaleTextRegion, 
		const CDisplayDataMinMax& displayDataMinMax, const double& dOnPtWidth, const double& dOrgGap) const;
	
protected:
	// 분/틱차트에서의 일자구분선 개별옵션 - onlyojt
	void DrawGrid(CDC* pDC, const CDrawingHorzScaleData& drawingHorzScaleData,
		const CScaleBaseData::SCALEGRIDTYPE gridType, const COLORREF clrGridColor, const int pt_x, const int nWidth = 1);

};

#endif // !defined(AFX_HORZSCALEDRAWER_H__2A95AF4E_41EC_4386_9397_18C4E1DD5FF0__INCLUDED_)
