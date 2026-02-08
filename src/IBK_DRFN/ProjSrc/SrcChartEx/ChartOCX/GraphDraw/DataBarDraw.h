// DataBarDraw.h: interface for the CDataBarDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATABARDRAW_H__4F5447ED_31D1_4641_921B_2F666E25DE20__INCLUDED_)
#define AFX_DATABARDRAW_H__4F5447ED_31D1_4641_921B_2F666E25DE20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Draw.h"

class CDataViewDraw : public CDraw 
{
public:
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
//	bool DrawDataBar(CDC* pDC, const CRect& realRegion, const CGraphBaseData::DATABARDRAWSTYLE drawStyle, const CGraphColor& graphColor, 
//		const int nRealStart, const int nRealEnd, const double dRealWidth, const CList<CPacket*, CPacket*>& packetList, const bool bIsBottom);
	bool DrawCharBar(CDC* pDC, const CRect& realRegion, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double dRealWidth, const CList<CPacket*, CPacket*>& packetList, const bool bIsBottom);
	bool GetPacketData_CharBar(const CList<CPacket*, CPacket*>& packetList, const int nStartIndex, 
		int& nLastestRealIndex, CList<double, double>*& pDataList, POSITION& pos) const;
	bool DrawHatchat45DegreesDataBar(CDC* pDC, const CRect& realRegion, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double dRealWidth, const CList<CPacket*, CPacket*>& packetList);
	bool DrawFillDataBar(CDC* pDC, const CRect& realRegion, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double dRealWidth, const CList<CPacket*, CPacket*>& packetList);
	bool DrawPointSignal(CDC* pDC, const CRect& realRegion, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor,
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin,
		const double dRealWidth, const CList<CPacket*, CPacket*>& packetList, bool bLog, bool bInvert);
	bool DrawTopSignal(CDC* pDC, const CRect& realRegion, const CGraphBaseData::DATASIGNALDRAWSTYLE drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, const double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, bool bLog, bool bInvert);
	bool DrawTopSignal(CDC* pDC, const CRect& realRegion, const CGraphBaseData::DATASIGNALDRAWSTYLE drawStyle, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, const double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, bool bLog, bool bInvert, const CString& strRQ, CXScaleManager* pxScaleManager, int nColumn, const CDrawingGraphData& drawingGraphData);
	bool DrawCharArrowSignal(CDC* pDC, const CRect& realRegion, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, const double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, bool bLog, bool bInvert, bool bHighLow, bool bRightLeft);
	bool DrawCharArrowSignal(CDC* pDC, const CRect& realRegion, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, const double dRealWidth, 
		const CList<CPacket*, CPacket*>& packetList, bool bLog, bool bInvert, bool bHighLow, bool bRightLeft, const CString& strRQ, CXScaleManager* pxScaleManager, int nColumn, const CDrawingGraphData& drawingGraphData);
	bool DrawSignal_NoCharArrowSignal(CDC* pDC, const CRect& realRegion, 
		const CGraphBaseData::DATASIGNALDRAWSTYLE drawStyle, const CGraphColor& graphColor, 
		const double dRealWidth, const int nRealStart, const int nRealEnd, const double& dViewMax, const double& dViewMin, 
		const CList<CPacket*, CPacket*>& packetList, bool bLog, bool bInvert, bool bUpDown);

	bool DrawCompartText(CDC* pDC, const CRect& realRegion, const CGraphColor& graphColor, 
		const int nRealStart, const int nRealEnd, const double dRealWidth, const CList<CPacket*, CPacket*>& packetList);

	void DrawTriangleType(CDC *pDC, CPoint* pPoint, CPoint Point, int width, CRect graphDrawingRegion, bool checkUpDown);
	void DrawArrowType(CDC *pDC, CPoint* pPoint, CPoint Point, int width, CRect graphDrawingRegion, bool checkUpDown);

	void GetArrowLengthAndHight(const CRect& graphRegion, const int dataPt_y, const bool bHighLow, int& nLength, int& nHight, bool& bIsUp) const;
};

#endif // !defined(AFX_DATABARDRAW_H__4F5447ED_31D1_4641_921B_2F666E25DE20__INCLUDED_)
