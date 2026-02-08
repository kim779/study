// SelectSubGraph.h: interface for the CSelectSubGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELECTSUBGRAPH_H__4BE85457_18DB_4B01_9C8E_04928206C2CD__INCLUDED_)
#define AFX_SELECTSUBGRAPH_H__4BE85457_18DB_4B01_9C8E_04928206C2CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "conversion.h"
#include "Draw.h"

class CPacket;
class CSelectSubGraphData;

class CSelectSubGraph : public CDraw
{
public:
	CSelectSubGraph();
	virtual ~CSelectSubGraph();

public:
	int PointInSubGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData, CIndicatorList* pIndicatorList);

public:
	// (2007/5/3 - Seung-Won, Bae) for Inquiry Full Data.
	bool GetPacketData_InverseLineGraph(const CSelectSubGraphData* pSelectSubGraphData, 
		int& nDataCount, CList<double, double>*& pCloseDataList, CList<double, double>*& pVolumeDataList, 
		POSITION& closeStartPos, POSITION& closeEndPos, POSITION& volumeStartPos, POSITION& volumeEndPos, int &p_nStartIndex) const;

private:
	int PointInBongGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData);
	int PointInLineGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData);
	int PointInSolidLineBongGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData);
	int PointInBarGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData, CIndicatorList* pIndicatorList);
	int PointInCombBarGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData, CIndicatorList* pIndicatorList);
	int PointInDotBarGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData, CIndicatorList* pIndicatorList);
//	int PointInDataBarGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData);
	int PointInTopBottomSignalGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData);

	/// 김찬중...2002년 12월 5일 추가.
	int PointInBoxBarGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData, CIndicatorList* pIndicatorList);
	int PointInLeftRightBarGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData, CIndicatorList* pIndicatorList);
	int PointInGroupBarGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData, CIndicatorList* pIndicatorList);

	int PointInThreeLineBreakGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData);
	int PointInInverseLineGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData);
	int PointInPAndFGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData);

	//몇번째 데이터 인지 가지고 오는 부분
	int GetDataIndex(CRect drawingRegion, CPoint point, int dataStart, int dataEnd, int pageDataCount);
	int GetDataIndex(CRect drawingRegion, CPoint point, int dataStart, int dataEnd);
	int GetIndex(const int dataIndex, const int dataStart) const;

};

#endif // !defined(AFX_SELECTSUBGRAPH_H__4BE85457_18DB_4B01_9C8E_04928206C2CD__INCLUDED_)
