#if !defined(AFX_PAIRBARDRAW_H__A87FD411_00FA_4A96_8027_00F397185D00__INCLUDED_)
#define AFX_PAIRBARDRAW_H__A87FD411_00FA_4A96_8027_00F397185D00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PairBarDraw.h : header file
//
#include "BarDraw.h"

/////////////////////////////////////////////////////////////////////////////
// CPairBarDraw window

class CPairBarDraw : public CBarDraw
{
// Construction
public:
	CPairBarDraw();

	virtual ~CPairBarDraw();

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

protected:
	bool DrawSellByHorizontalBar(CDC* pDC, const CRect& graphRegion, 
		const CString& strSubGraphName, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nStartIndex, const int nEndIndex, const double& dViewMax, const double& dViewMin, 
		const CList<CPacket*, CPacket*>& packetList, const CIndicatorInfo* pIndicatorInfo, const bool bLog,
		const bool bInvert,const CPacket& pPacket);
	bool GetDrawingEnvironment_SellByHorizontalBar(const CList<CPacket*, CPacket*>& packetList, 
		CList<double, double>*& pDataList, int& nDataCount, 
		double& dHorzViewMin, double& dHorzViewMax, int& nCurrentIndex, int& nHighestIndex) const;
	void DrawBar_SellHorizontalBar(CDC* pDC, const CGraphDrawStyle& drawStyle, 
		const CRect& graphRegion, const int nBarWidth, const bool bInvert, CPoint& BydataPt,CPoint& SelldataPt);
	void DrawBar_ByHorizontalBar(CDC* pDC, const CGraphDrawStyle& drawStyle, 
		const CRect& graphRegion, const int nBarWidth, const bool bInvert, CPoint& BydataPt,CPoint& SelldataPt);
	void DrawText_SellByHorizontalBar(CDC* pDC, const CRect& graphRegion, const CPoint& BydataPt,
		const CPoint& SelldataPt, CString strByData, CString strSellData, CString strFullData,
		short nWidth, const CGraphColor& graphColor);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAIRBARDRAW_H__A87FD411_00FA_4A96_8027_00F397185D00__INCLUDED_)
