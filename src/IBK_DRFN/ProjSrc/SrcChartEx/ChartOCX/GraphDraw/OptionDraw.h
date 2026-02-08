// OptionDraw.h: interface for the COptionDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONDRAW_H__19115C31_9FFF_41B3_ABEB_5CC6EED0A5FD__INCLUDED_)
#define AFX_OPTIONDRAW_H__19115C31_9FFF_41B3_ABEB_5CC6EED0A5FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Draw.h"

class CIndicatorInfo;
class CPacketList;

class COptionDraw : public CDraw
{
public:
	COptionDraw();
	virtual ~COptionDraw();

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
	bool DrawMarketProfile(CDC* pDC, const CRect& graphRegion, const double& dViewMin, const double& dViewMax, 
		const CGraphColor& graphColor, const CList<CPacket*, CPacket*> &packetList);
	bool DrawFuOpPAndF(CDC* pDC, const CRect& graphRegion, const CDisplayAttributes& displayAttributes,/*CIndicatorInfo* pIndicatorInfo,*/
		const CGraphColor& graphColor, const double& dViewMin, const double& dViewMax,
		const CList<CPacket*, CPacket*> &packetList);
};

#endif // !defined(AFX_OPTIONDRAW_H__19115C31_9FFF_41B3_ABEB_5CC6EED0A5FD__INCLUDED_)
