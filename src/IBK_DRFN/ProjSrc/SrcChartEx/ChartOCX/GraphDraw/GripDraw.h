// GripDraw.h: interface for the CGripDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIPDRAW_H__EF04D1D4_12AC_4B9B_B8CD_AE29DFC82A97__INCLUDED_)
#define AFX_GRIPDRAW_H__EF04D1D4_12AC_4B9B_B8CD_AE29DFC82A97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "Draw.h"
#include "BlockBaseData.h"
#include "SelectSubGraphData.h"

class CSubGraphData;
class CDrawingGraphData;
class CPacket;
class CDisplayAttributes;

class CGripDraw : public CDraw 
{
public:
	CGripDraw();
	virtual ~CGripDraw();

	bool DrawGrip(CDC* pDC, const int nIndex, const CSelectSubGraphData& selectSubGraphData);// X축의 Start, End 값(packet index의 시작 위치) 

	bool GetGripDrawData_BarType(CPacket* pDataPacket, const CSelectSubGraphData& selectSubGraphData, 
		int& nDataStartIndex, int& nDataEndIndex, CRect& drawingRegion) const;

private:
	const int GetBoxCondition(CPacket* pPacket, bool bForward);
	bool DrawGripAll(CDC* pDC, const CSelectSubGraphData& selectSubGraphData);
	bool DrawGripBar(CDC* pDC, const CSelectSubGraphData& selectSubGraphData);

	///김찬중...2002년 12월 9일 3마리 추가
	bool DrawGripBoxBar(CDC* pDC, const CSelectSubGraphData& selectSubGraphData);
	bool DrawGripLeftRightBar(CDC* pDC, const CSelectSubGraphData& selectSubGraphData);
	bool DrawGripGroupBar(CDC* pDC, const CSelectSubGraphData& selectSubGraphData);

	bool DrawGripBong(CDC* pDC, const CSelectSubGraphData& selectSubGraphData);
	bool DrawLineBongGrip(CDC* pDC, const CSelectSubGraphData& selectSubGraphData);
	
	bool DrawSignalGrip(CDC* pDC, const int nIndex, const CSelectSubGraphData& selectSubGraphData);
//	bool DrawDataBarGrip(CDC* pDC, const int nIndex, const CSelectSubGraphData& selectSubGraphData);

	bool DrawThreeLineBreakGrip(CDC* pDC, const CSelectSubGraphData& selectSubGraphData);
	bool DrawInverseLineGrip(CDC* pDC, const CSelectSubGraphData& selectSubGraphData);
//	bool DrawPAndFGrip(CDC* pDC, const CSelectSubGraphData& selectSubGraphData);
	
	int CalGripInterval(const CDisplayAttributes& displayAttributes);
	int CalBoxGripInterval(const CDisplayAttributes& displayAttributes, const int nCondition);
	int CalHGripNumber(const CDisplayAttributes& displayAttributes, int totalCount);
	int CalDataEnd(const CList<CPacket*, CPacket*>& PacketList, const CDisplayAttributes& displayAttributes);

	void DrawGrip(CDC* pDC, CRect rect);
};

#endif // !defined(AFX_GRIPDRAW_H__EF04D1D4_12AC_4B9B_B8CD_AE29DFC82A97__INCLUDED_)
