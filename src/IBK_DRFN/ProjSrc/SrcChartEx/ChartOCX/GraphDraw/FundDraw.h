// FundDraw.h: interface for the CFundDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUNDDRAW_H__7405C1E1_C2B1_4ED0_8CE4_5716894BED25__INCLUDED_)
#define AFX_FUNDDRAW_H__7405C1E1_C2B1_4ED0_8CE4_5716894BED25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Draw.h"

class CFundDraw : public CDraw      
{
public:
	CFundDraw();
	virtual ~CFundDraw();

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
	//bool DrawOutCome(CDC* pDC, const CRect& graphDrawingRegion, const CGraphColor& graphColor, const int dataStart, const int dataEnd, const int dataRealEnd, const int dataViewMax, const int dataViewMin, const CList<CPacket*, CPacket*>& packetList);
	bool DrawComponentRatio_Bar(CDC* pDC, const CEtcGraphColor& etcGraphColor, const CRect& graphDrawingRegion, const CGraphColor& graphColor, const int dataStart, const int dataEnd, const int dataRealEnd, const double& dataViewMax, const double& dataViewMin, const CList<CPacket*, CPacket*>& packetList, CPenThickness penThickness); 
	bool DrawStyleBox(CDC* pDC, const CEtcGraphColor& etcGraphColor, const CRect& graphDrawingRegion, const CGraphColor& graphColor, const int dataStart, const double& dataViewMax, const double& dataViewMin, const CList<CPacket*, CPacket*>& packetList, CPenThickness penThickness); 
	bool DrawReturnRiskGrade(CDC* pDC, const CEtcGraphColor& etcGraphColor, const CRect& graphDrawingRegion, const CGraphColor& graphColor, const int dataStart, const double& dataViewMax, const double& dataViewMin, const CList<CPacket*, CPacket*>& packetList, CPenThickness penThickness);
	bool DrawComponentRatio_Line(CDC* pDC, const CEtcGraphColor& etcGraphColor, const CRect& graphDrawingRegion, const CGraphColor& graphColor, const int dataStart, const int dataEnd, const int dataRealEnd, const double& dataViewMax, const double& dataViewMin, const CList<CPacket*, CPacket*>& packetList, CPenThickness penThickness);
	bool DrawReturnRiskDispersion(CDC* pDC, const CEtcGraphColor& etcGraphColor, const CRect& graphDrawingRegion, const CGraphColor& graphColor, const int dataStart, const double& dataViewMax, const double& dataViewMin, const double& dataViewXMax, const double& dataViewXMin, const CList<CPacket*, CPacket*>& packetList);
	bool DrawFundMarketDispersion(CDC* pDC, const CEtcGraphColor& etcGraphColor, const CRect& graphDrawingRegion, const CGraphColor& graphColor, const int dataStart, const double& dataViewMax, const double& dataViewMin, const CList<CPacket*, CPacket*>& packetList, CPenThickness penThickness);
	bool DrawComponentRatio_Pie(CDC* pDC, const CRect& realRegion, const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData, const CSubGraphData& subGraphData);
	bool DrawComponentRatio_Bar_Line(CDC* pDC, const CEtcGraphColor& etcGraphColor, const CRect& graphDrawingRegion, const CGraphColor& graphColor, const int dataStart, const int dataEnd, const int dataRealEnd, const double& dataViewMax, const double& dataViewMin, const CList<CPacket*, CPacket*>& packetList, CPenThickness penThickness);
	bool DrawGroupBar(CDC* pDC, const CEtcGraphColor& etcGraphColor, const CRect& graphDrawingRegion, const CGraphColor& graphColor, const int dataStart, const int dataEnd, const int dataRealEnd, const double& dataViewMax, const double& dataViewMin, const CList<CPacket*, CPacket*>& packetList, CPenThickness penThickness);

	void DrawBox(CDC* pDC, CRect rect);
	
};

#endif // !defined(AFX_FUNDDRAW_H__7405C1E1_C2B1_4ED0_8CE4_5716894BED25__INCLUDED_)
