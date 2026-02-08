// LineDraw.h: interface for the CLineDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINEDRAW_H__9E62656D_2F57_4990_A5D9_9318BDF0C3A7__INCLUDED_)
#define AFX_LINEDRAW_H__9E62656D_2F57_4990_A5D9_9318BDF0C3A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Draw.h"

class CIndicatorInfo;
class CXScaleManager;
class CCalcDataForDraw;

class CLineDraw : public CDraw  
{
public:
	CLineDraw();
	virtual ~CLineDraw();

	virtual bool DrawGraph(CDC* pDC,
			const CRect& realRegion, 
			const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData,
			const CSubGraphData& subGraphData);

	virtual bool DrawGraph(CDC* pDC,
			const CRect& realRegion, 
			const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData,
			const CSubGraphData& subGraphData,
			const CString& strRQ,
			CXScaleManager* pxScaleManager);

private:
	enum ENUM_BRUSH
	{
		UP_BRUSH,
		DOWN_BRUSH,

		BRUSH_COUNT
	};
	
	enum ENUM_PEN
	{
		STANDARD_PEN,
		UP_PEN,
		DOWN_PEN,
		BOHAP_PEN,

		PEN_COUNT
	};

	typedef struct RGB_Element{
		double R;
		double G;
		double B;
	} RGBElement;

	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	bool DrawSimpleLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, 
		const CGraphColor& graphColor, CIndicatorInfo* pIndicatorInfo, int nRealStart, int nRealEnd, 
		const double& dViewMax, const double& dViewMin, double dRealWidth, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, const CGraphBaseLineStyle& baseLineStyle, bool bLog, bool bInvert);
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	bool DrawSimpleLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, 
		const CGraphColor& graphColor, CIndicatorInfo* pIndicatorInfo, int nRealStart, int nRealEnd, 
		const double& dViewMax, const double& dViewMin, double dRealWidth, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, const CGraphBaseLineStyle& baseLineStyle, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, const CSubGraphData& subGraphData, int p_nColumn);
	bool DrawZigzagLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert);
	bool DrawZigzagLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn);
	bool DrawFillLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, 
		const CGraphColor& graphColor, CIndicatorInfo* pIndicatorInfo, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, double dRealWidth, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert);
	bool DrawFillLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, 
		const CGraphColor& graphColor, CIndicatorInfo* pIndicatorInfo, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, double dRealWidth, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, CSubGraphData& subGraphData, int p_nColumn);
	bool DrawHorizontalLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert);
	bool DrawHorizontalLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn);
	bool DrawVerticalLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert);
	bool DrawVerticalLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn);
	bool DrawStairLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert);
	bool DrawStairLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn);
	bool DrawCloudLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert);
	bool DrawCloudLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn);
	bool DrawNonZeroLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, 
		const CGraphColor& graphColor, CIndicatorInfo* pIndicatorInfo, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, double dRealWidth, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert);
	bool DrawNonZeroLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, 
		const CGraphColor& graphColor, CIndicatorInfo* pIndicatorInfo, const int nRealStart, const int nRealEnd, 
		const double& dViewMax, const double& dViewMin, double dRealWidth, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert,	const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn);
	bool DrawComparisonLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw, BOOL p_bWithBaseText, 
		bool bBaseLine);	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	bool DrawComparisonLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert, bool bGraphTextDraw, BOOL p_bWithBaseText, bool bBaseLine,  // 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn);
	bool DrawRainbowLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, bool bLog, bool bInvert);
	bool DrawRainbowLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn);
	bool DrawLineCloudLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert);
	bool DrawLineCloudLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		CIndicatorInfo* pIndicatorInfo, const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn);
	bool DrawDataDotSimple(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, 
		const CGraphColor& graphColor, CIndicatorInfo* pIndicatorInfo, int nRealStart, int nRealEnd, 
		const double& dViewMax, const double& dViewMin, double dRealWidth, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert);
	bool DrawDataDotSimple(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, 
		const CGraphColor& graphColor, CIndicatorInfo* pIndicatorInfo, int nRealStart, int nRealEnd, 
		const double& dViewMax, const double& dViewMin, double dRealWidth, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn);
	bool DrawNonCloudLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert);
	bool DrawNonCloudLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		CIndicatorInfo* pIndicatorInfo, const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn);
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	bool DrawBaseLine_SimpleLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, 
		const CGraphColor& graphColor, CIndicatorInfo* pIndicatorInfo, 
		const double& dViewMax, const double& dViewMin, double dRealWidth, bool bInvert,
		const CPenThickness& penThickness, const CGraphBaseLineStyle& baseLineStyle);
	bool DrawDataLine_SimpleLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, 
		const CGraphColor& graphColor, CIndicatorInfo* pIndicatorInfo, int nRealStart, int nRealEnd, 
		const double& dViewMax, const double& dViewMin, double dRealWidth, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert,
		const CString& strRQ, CXScaleManager* pxScaleManager, CSubGraphData& subGraphData, int p_nColumn);
	bool DrawDataLine_SimpleLine(CDC* pDC, const CRect& graphRegion, const CGraphDrawStyle& drawStyle, 
		const CGraphColor& graphColor, CIndicatorInfo* pIndicatorInfo, int nRealStart, int nRealEnd, 
		const double& dViewMax, const double& dViewMin, double dRealWidth, const CList<CPacket*, CPacket*>& packetList, 
		const CPenThickness& penThickness, bool bLog, bool bInvert);
	void DrawArrow(CDC* pDC, const CPoint& point, const COLORREF& color);

	int CalculateDeclination(const CPoint startPt, const CPoint endPt, const CPoint standardStartPt, const CPoint standardEndPt);
	void GetStandardValue(CDC* pDC, CPacket *pPacket, CIndicatorInfo *pIndicatorInfo, 
		double dRealMin, double dRealMax, double dViewMin, double dViewMax, double dRealWidth, 
		bool bInvert, const CRect& graphRegion, double& dStandardMin, double& dStandardMax);
	bool GetEnvironment_SimpleLine(CIndicatorInfo* pIndicatorInfo, CPacket* pPacket, const CRect& region, 
		const double& dRealWidth, const int nRealStart, const double& dViewMin, const double& dViewMax, const bool bLog, const bool bInvert, 
		CList<double, double>*& pDataList, POSITION& pos, int& nDrawStart, int& nDrawEnd, int& nArrowIndex, double& dRealMin, double& dRealMax, CPoint& prePt);
//	int GetArrowIndex(CIndicatorInfo* pIndicatorInfo, CPacket* pPacket, const int nDrawEnd);
	int GetArrowIndex(CIndicatorInfo* pIndicatorInfo, const CString &p_strSubGraphName, const int nDrawEnd);
	bool IsDrawArrow(CIndicatorInfo* pIndicatorInfo);
	int GetConditionDataInName(CIndicatorInfo* pIndicatorInfo, const CString& strName);
	int GetPartConditionDataInName(CIndicatorInfo* pIndicatorInfo, const CString& strName);
	int GetCommonConditionDataInName(CIndicatorInfo* pIndicatorInfo, const CString& strName);
	CPoint GetPoint(CList<double, double>* pDataList, const CRect& region, 
		const double& dRealWidth, const double& dRealMin, const double& dRealMax, 
		const double& dData, const int nIndex, const bool bLog, const bool bInvert);
//	void CreatePen(CPen& pen, const CGraphDrawStyle& drawStyle, int penThickness, COLORREF color);
	void Draw_SolidBong(CDC* pDC, const CPoint& curPt, const int &nBase, const double& dRealWidth, CPen* pPen);
	void Draw_SolidBong(CDC* pDC, const CPoint& prePt, const CPoint& curPt, const int &nBase, CPen* pPen);

	void InitGdiObject(CBrush* pBrush, CPen* pPen, const CGraphColor& graphColor, 
		const double& dWidth, const CPenThickness& penThickness, const bool bIsDrawingThickness, const CGraphDrawStyle& drawStyle);
	void DestroyGdiObject(CDC* pDC, CBrush* pBrush, CPen* pPen, CBrush* pOldBrush, CPen* pOldPen);
	void CheckMinMaxOrLastYValue(bool* bFirst, int* nTop, int nViewY);
	void SetPoint_As_XPoint_IsSame(CCalcDataForDraw* pCalcData, int& nIndex, int* nTop, int nPreLeft,
									int nLastX, int nLastViewY);
};

#endif // !defined(AFX_LINEDRAW_H__9E62656D_2F57_4990_A5D9_9318BDF0C3A7__INCLUDED_)
