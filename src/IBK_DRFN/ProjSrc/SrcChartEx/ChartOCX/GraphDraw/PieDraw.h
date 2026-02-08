// PieDraw.h: interface for the CPieDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PIEDRAW_H__27BFDA29_241B_4774_834C_1A1527CCD93F__INCLUDED_)
#define AFX_PIEDRAW_H__27BFDA29_241B_4774_834C_1A1527CCD93F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Draw.h"

class CPieDraw : public CDraw   
{
public:
	CPieDraw();
	virtual ~CPieDraw();

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
		CXScaleManager* pxScaleManager);



private:
	void DrawPie(CDC* pDC, CBrush* pBrush, CPen& linePen, const CList<double, double>& angleList, 
		const CRect& pieRect, const CPoint& centerPoint);
	bool DrawFlatPie(CDC* pDC, const CRect& graphDrawingRegion, const CGraphColor& graphColor, const CEtcGraphColor& etcGraphColor, const int dataStart, const int dataRealEnd, const double& dataViewMax, const double& dataViewMin, const CList<CPacket*, CPacket*>& packetList);
	bool DrawSolidPie(CDC* pDC, const CRect& graphDrawingRegion, const CGraphColor& graphColor, const CEtcGraphColor& etcGraphColor, const int dataStart, const int dataRealEnd, const double& dataViewMax, const double& dataViewMin, const CList<CPacket*, CPacket*>& packetList);
	void DrawSidePole(CDC* pDC, CBrush* pBrush, CPen* pPen, CPen* pLinePen, const CRect& pieRect, 
		const CPoint& upCenterPoint, const CPoint& downCenterPoint, const bool bIsLeft);
	void DrawCenterPole(CDC* pDC, CBrush* pBrush, CPen* pPen, CPen* pLinePen, const CPoint points[4]);
	void DrawText(CDC* pDC, CBrush* pBrush, const CRect& graphRegion, const CString& strPacketType, CList<CString, CString>* pTextList, CList<double, double>* pDataList);

	int GetBrushIndex(const int nBrushCount, const int nDataIndex) const;
	bool GetPiePoints(const CRect& pieRect, const CPoint& centerPoint, 
		const double& dPrevAngle, const double& dAngle, CPoint points[2]) const;
	bool GetPillarPoints(const CRect& pieRect, const CPoint& upCenterPoint, 
		const CPoint& downCenterPoint, const double& dAngle, CPoint points[4]) const;
	bool GetPieAllAngles(CList<double, double>* pDataList, CList<double, double>& angleList) const;
	bool GetDataList(const CString& strPacketType, CList<double, double>* pDataList, CList<CString, CString>& dataList) const;
	void GetDataListInCheckLength(CList<CString, CString>* pOrgDataList, const int nMaxLength, const bool bRight, CList<CString, CString>& dataList) const;
	int GetMaxLength(const CList<CString, CString>& dataList) const;
	int GetTextWidth(const CSize& size, const CList<CString, CString>& titleList, const CList<CString, CString>& dataList) const;

	bool IsDrawLiftRight(const double& dCurAngle, const bool bIsLeftPole, const bool bIsRightPole) const;
	bool InBetweenAngle(const double& dAngle1, const double& dAngle2, const double& dCurAngle) const;
	bool IsDrawingText(const CRect& graphRegion, const int nTextLeftPt) const;
};

#endif // !defined(AFX_PIEDRAW_H__27BFDA29_241B_4774_834C_1A1527CCD93F__INCLUDED_)
