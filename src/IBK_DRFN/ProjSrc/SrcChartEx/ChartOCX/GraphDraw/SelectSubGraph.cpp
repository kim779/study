// SelectSubGraph.cpp: implementation of the CSelectSubGraph class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SelectSubGraph.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"				// for _MTEXT()
#include "GraphData.h"
#include "PacketBase.h"
#include "SelectSubGraphData.h"
#include "IndicatorInfo.h"
#include "IndicatorList.h"
#include "DataMath.h"
#include "SpecialDraw.h"
#include "BarDraw.h"
#include "PacketPointerList.h"
#include "GripDraw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSelectSubGraph::CSelectSubGraph()
{

}

CSelectSubGraph::~CSelectSubGraph()
{

}

int CSelectSubGraph::PointInSubGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData, CIndicatorList* pIndicatorList)
{
	if(pSelectSubGraphData == NULL || pSelectSubGraphData->GetSubGraphPacketList() == NULL)
		return -1;

	switch(pSelectSubGraphData->GetSubGraphType()){
	case CGraphBaseData::Bong_Type:
		{
			if(pSelectSubGraphData->GetSubGraphStyle().GetGraphBongStyle() == CGraphBaseData::Line_Draw)
					return PointInLineGraph(point, pSelectSubGraphData);
			return PointInBongGraph(point, pSelectSubGraphData);
		}
	case CGraphBaseData::Bar_Type:
		{
			switch(pSelectSubGraphData->GetSubGraphStyle().GetGraphBarStyle()){
			case CGraphBaseData::VerticalBar:
			case CGraphBaseData::HorizontalBar:
			case CGraphBaseData::UpDownBar:
			// 고정비율상하식 추가 - ojtaso (20080603)
			case CGraphBaseData::FixedRateUpDownBar:
			case CGraphBaseData::TextHorizontalBar:

			/// 김찬중... 2002년 12월 3일 추가
			case CGraphBaseData::UpDownHighLowBar:
				return PointInBarGraph(point, pSelectSubGraphData, pIndicatorList);
			case CGraphBaseData::CombBar:
				return PointInCombBarGraph(point, pSelectSubGraphData, pIndicatorList);
			case CGraphBaseData::DotBar:
				return PointInDotBarGraph(point, pSelectSubGraphData, pIndicatorList);

			/// 김찬중... 2002년 12월 5일 추가
			case CGraphBaseData::BoxBar:
				return PointInBoxBarGraph(point, pSelectSubGraphData, pIndicatorList);
			case CGraphBaseData::LeftRightBar:
				return PointInLeftRightBarGraph(point, pSelectSubGraphData, pIndicatorList);
			case CGraphBaseData::GroupBar:
				return PointInGroupBarGraph(point, pSelectSubGraphData, pIndicatorList);
			}
			return -1;
		}
	case CGraphBaseData::DataView_Type:
		{
			switch(pSelectSubGraphData->GetSubGraphStyle().GetGraphDataViewStyle())
			{
//			case CGraphBaseData::TopDataBar:
//			case CGraphBaseData::BottomDataBar:
//				return PointInDataBarGraph(point, pSelectSubGraphData);
			case CGraphBaseData::TopSignal:
			case CGraphBaseData::BottomSignal:
				return PointInTopBottomSignalGraph(point, pSelectSubGraphData);
			}
			return -1;
		}
	case CGraphBaseData::Line_Type:
		return PointInLineGraph(point, pSelectSubGraphData);
	case CGraphBaseData::Pie_Type:
		return -1;
	case CGraphBaseData::Special_Type:
	{
		switch(pSelectSubGraphData->GetSubGraphStyle().GetGraphBarStyle())
		{
		case CGraphBaseData::ThreeLineBreak:
			return PointInThreeLineBreakGraph(point, pSelectSubGraphData);
		case CGraphBaseData::InverseLine:
			return PointInInverseLineGraph(point, pSelectSubGraphData);			
		case CGraphBaseData::PAndF:
			return PointInPAndFGraph(point, pSelectSubGraphData);
		}
		return -1;
	}
	case CGraphBaseData::Fund_Type:
		return -1;
	case CGraphBaseData::FuOp_Type:
		if(pSelectSubGraphData->GetSubGraphStyle().GetGraphFuOpStyle() == CGraphBaseData::FuOpPAndF)
			return PointInPAndFGraph(point, pSelectSubGraphData);
		return -1;
	}

	return -1;	
}

//private=====================
int CSelectSubGraph::PointInBongGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData)
{
	const CList< CPacket *, CPacket *> &packetList = pSelectSubGraphData->GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return -1;

	CPacket* pOpenPacket = NULL;
	CPacket* pHighPacket = NULL; 
	CPacket* pLowPacket = NULL; 
	CPacket* pClosePacket = NULL;

	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)	return -1;
	
	if(packetList.GetCount() >= 4){
		pOpenPacket = packetList.GetNext(pos);
		pHighPacket = packetList.GetNext(pos);
		pLowPacket = packetList.GetNext(pos);
		pClosePacket = packetList.GetNext(pos);
	}
	else
		return -1;

	if(pOpenPacket == NULL || pHighPacket == NULL || pLowPacket == NULL || pClosePacket == NULL)
		return -1;

	int dataStart = pSelectSubGraphData->GetDisplayAttributes().GetDataStartInBlock();
	int dataEnd = pSelectSubGraphData->GetDisplayAttributes().GetDataEndInBlock();
	int pageDataCount = pSelectSubGraphData->GetDisplayAttributes().GetDataCountInBlock();
	
	CRect graphRegion = pSelectSubGraphData->GetGraphRegions().GetDrawingRegion();

	int dataIndex = GetDataIndex(graphRegion, point, dataStart, dataEnd, pageDataCount); 
	//int dataIndex;
	int nCol = 0;
	CXScaleManager::AREA area;
	pSelectSubGraphData->GetXScaleManager()->GetDTInAREAfRQnPt(pSelectSubGraphData->GetRQ(), point.x, nCol, &dataIndex, &area);

	double realWidth = CDraw::CalRealWidth(graphRegion, dataStart, dataEnd);
	if(dataIndex == -1)
		return -1;

	POSITION startDataPos = pOpenPacket->GetnumericDataList()->FindIndex(dataIndex);
	POSITION highDataPos = pHighPacket->GetnumericDataList()->FindIndex(dataIndex);
	POSITION lowDataPos = pLowPacket->GetnumericDataList()->FindIndex(dataIndex);
	POSITION endDataPos = pClosePacket->GetnumericDataList()->FindIndex(dataIndex);
	
	if(startDataPos == NULL || highDataPos == NULL || lowDataPos == NULL || endDataPos == NULL) 
		return -1;

	int nIndex = GetIndex(dataIndex, dataStart);
//	CPoint highDataPt = CDataConversion::GetVerticalPoint(graphRegion, count, realWidth, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMin()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMax()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pHighPacket->GetnumericDataList()->GetNext(highDataPos)));
//	CPoint startDataPt = CDataConversion::GetVerticalPoint(graphRegion, count, realWidth, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMin()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMax()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pOpenPacket->GetnumericDataList()->GetNext(startDataPos)));
//	CPoint endDataPt = CDataConversion::GetVerticalPoint(graphRegion, count, realWidth, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMin()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMax()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pClosePacket->GetnumericDataList()->GetNext(endDataPos)));
//	CPoint lowDataPt = CDataConversion::GetVerticalPoint(graphRegion, count, realWidth, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMin()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMax()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pLowPacket->GetnumericDataList()->GetNext(lowDataPos)));

	CPoint startDataPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, realWidth, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMin()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMax()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pOpenPacket->GetnumericDataList()->GetNext(startDataPos)), false);
	CPoint highDataPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, realWidth, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMin()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMax()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pHighPacket->GetnumericDataList()->GetNext(highDataPos)), true);
	CPoint lowDataPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, realWidth, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMin()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMax()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pLowPacket->GetnumericDataList()->GetNext(lowDataPos)), true);
	CPoint endDataPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex + 1, realWidth, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMin()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMax()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pClosePacket->GetnumericDataList()->GetNext(endDataPos)), false);

///	int bongWidth = int(realWidth*0.8);

	//invert scale을 위해서.
	CPoint mousePoint = point;
	if(pSelectSubGraphData->GetVertScaleInvertType())
		mousePoint.y = CDataConversion::InvertGraphDataPt(graphRegion, point.y);


	startDataPt.x = area.left;
	endDataPt.x = area.right;

///	if(highDataPt.y <= mousePoint.y && lowDataPt.y >= mousePoint.y && mousePoint.x >= highDataPt.x - bongWidth && mousePoint.x <= highDataPt.x + bongWidth)
	if(highDataPt.y <= mousePoint.y && lowDataPt.y >= mousePoint.y && mousePoint.x >= startDataPt.x && mousePoint.x <= endDataPt.x)
		return dataIndex;
		
	return -1;
}

int CSelectSubGraph::PointInCombBarGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData, CIndicatorList* pIndicatorList)
{
	const CList< CPacket *, CPacket *> &packetList = pSelectSubGraphData->GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return -1;

	CPacket* m_dataPacket1 = NULL;
	CPacket* m_dataPacket2 = NULL;
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)	
		return -1;
	m_dataPacket1 = packetList.GetNext(pos);
	if(pos == NULL)	
		return -1;
	m_dataPacket2 = packetList.GetNext(pos);

	int dataStart = pSelectSubGraphData->GetDisplayAttributes().GetDataStartInBlock();
	int dataEnd = pSelectSubGraphData->GetDisplayAttributes().GetDataEndInBlock();
	int pageDataCount = pSelectSubGraphData->GetDisplayAttributes().GetDataCountInBlock();
			
	//min 과 max를 새로 구한다.=====================================================
	double datamax = 0.0, datamin = 0.0, tempmax = 0.0, tempmin = 0.0;
	m_dataPacket1->GetMinMax(dataStart, dataEnd, datamin, datamax); 
	datamin = fabs(datamin);
	datamax = fabs(datamax);
	if(datamin > datamax)
		datamax = datamin;
	m_dataPacket2->GetMinMax(dataStart, dataEnd, tempmax, tempmin); 
	tempmin = fabs(tempmin);
	tempmax = fabs(tempmax);
	if(tempmin > tempmax)
		tempmax = tempmin;
	if(tempmax > datamax)
		datamax = tempmax;
	datamin = datamax * -1;
	//==============================================================================
	CRect graphRegion = pSelectSubGraphData->GetGraphRegions().GetDrawingRegion();

	int dataIndex = GetDataIndex(graphRegion, point, dataStart, dataEnd, pageDataCount); 
	if(dataIndex == -1)
		return -1;
	
	POSITION Data1Pos = m_dataPacket1->GetnumericDataList()->FindIndex(dataIndex);
	POSITION Data2Pos = m_dataPacket2->GetnumericDataList()->FindIndex(dataIndex);
	if(Data1Pos == NULL || Data2Pos == NULL)
		return -1;

	double data1 = m_dataPacket1->GetnumericDataList()->GetNext(Data1Pos);
	if(data1 < 0)
		data1 = data1 * -1.0;
	double data2 = m_dataPacket2->GetnumericDataList()->GetNext(Data2Pos);
	if(data2 > 0)
		data2 = data2 * -1.0;
	
	double realWidth = CDraw::CalRealWidth(graphRegion, dataStart, dataEnd);
	int barWidth = int(realWidth*0.8);
	int nIndex = GetIndex(dataIndex, dataStart);
	CPoint dataPt1 = CDataConversion::GetVerticalPoint(graphRegion, nIndex, realWidth, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), datamin), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), datamax), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), data1));
	CPoint dataPt2 = CDataConversion::GetVerticalPoint(graphRegion, nIndex, realWidth, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), datamin), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), datamax), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), data2));

	//invert scale을 위해서.
	CPoint mousePoint = point;
	if(pSelectSubGraphData->GetVertScaleInvertType())
		mousePoint.y = CDataConversion::InvertGraphDataPt(graphRegion, point.y);

	if(dataPt1.y <= dataPt2.y){
		if(dataPt1.y <= mousePoint.y && dataPt2.y >= mousePoint.y && mousePoint.x >=dataPt1.x - barWidth && mousePoint.x <= dataPt1.x + barWidth)
			return dataIndex;
	}
	else{
		if(dataPt1.y >= mousePoint.y && dataPt2.y <= mousePoint.y && mousePoint.x >=dataPt1.x - barWidth && mousePoint.x <= dataPt1.x + barWidth)
			return dataIndex;
	}

	return -1;
}

int CSelectSubGraph::PointInDotBarGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData, CIndicatorList* pIndicatorList)
{			
	const CList< CPacket *, CPacket *> &packetList = pSelectSubGraphData->GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return -1;
	POSITION Pos = packetList.GetHeadPosition();
	if(Pos == NULL)
		return -1;
	CPacket* dataPacket;
	dataPacket = packetList.GetNext(Pos);

	int dataStart = pSelectSubGraphData->GetDisplayAttributes().GetDataStartInBlock();
	int dataEnd = pSelectSubGraphData->GetDisplayAttributes().GetDataEndInBlock();
	int pageDataCount = pSelectSubGraphData->GetDisplayAttributes().GetDataCountInBlock();
	CRect graphRegion = pSelectSubGraphData->GetGraphRegions().GetDrawingRegion();
	int dataIndex = GetDataIndex(graphRegion, point, dataStart, dataEnd, pageDataCount); 

	if(dataIndex == -1)
		return -1;
	 
	POSITION dataPos = dataPacket->GetnumericDataList()->FindIndex(dataIndex);
	if(dataPos == NULL) 
		return -1;

	double realWidth = CDraw::CalRealWidth(graphRegion, dataStart, dataEnd);
	double dataViewMin = CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMin());
	double dataViewMax = CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMax());
	int nIndex = GetIndex(dataIndex, dataStart);
	CPoint dataPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, realWidth, dataViewMin, dataViewMax, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), dataPacket->GetnumericDataList()->GetNext(dataPos)));
	int barWidth = int(realWidth*0.8);

	//invert scale을 위해서.
	CPoint mousePoint = point;
	if(pSelectSubGraphData->GetVertScaleInvertType())
		mousePoint.y = CDataConversion::InvertGraphDataPt(graphRegion, point.y);

	if(mousePoint.y <= dataPt.y + barWidth && mousePoint.y >= dataPt.y - barWidth && mousePoint.x >=dataPt.x - barWidth && mousePoint.x <= dataPt.x + barWidth)
		return dataIndex;

	return -1;
}

//sy 2002.9.24.
int CSelectSubGraph::PointInBarGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData, CIndicatorList* pIndicatorList)
{
	if(pSelectSubGraphData == NULL || pSelectSubGraphData->GetSubGraphType() != CGraphBaseData::Bar_Type)
		return -1;

	const CList< CPacket *, CPacket *> &packetList = pSelectSubGraphData->GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return -1;
	
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)	
		return -1;
	CPacket* pDataPacket = packetList.GetNext(pos);
	if(pDataPacket == NULL)	return -1;

	//sy 2004.12.45.
	int dataStart = 0, dataEnd = 0;
	CRect graphRegion(0, 0, 0, 0);
	if(!CGripDraw().GetGripDrawData_BarType(pDataPacket, *pSelectSubGraphData, 
		dataStart, dataEnd, graphRegion))
		return -1;
	//sy end

	int pageDataCount = dataEnd - dataStart + 1;
	int dataIndex = 0;
	CXScaleManager::AREA area;
	CGraphBaseData::BARSTYLE eGraphStyle = pSelectSubGraphData->GetSubGraphStyle().GetGraphBarStyle();
	if(eGraphStyle == CGraphBaseData::HorizontalBar || eGraphStyle == CGraphBaseData::TextHorizontalBar){
		if(pDataPacket->GetName() == _MTEXT( C2_OVERHANGING_SUPPLY))
			dataIndex = GetDataIndex(graphRegion, point, 0, dataEnd);
	}
	else
	{
		int nCol = 0;
		pSelectSubGraphData->GetXScaleManager()->GetDTInAREAfRQnPt(pSelectSubGraphData->GetRQ(), point.x, nCol, &dataIndex, &area);
	}

	if(dataIndex == -1)
		return -1;
	 
	POSITION dataPos = pDataPacket->GetnumericDataList()->FindIndex(dataIndex);
	if(dataPos == NULL) 
		return -1;

	double realWidth = CDraw::CalRealWidth(graphRegion, dataStart, dataEnd);
	double dataViewMin = CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMin());
	double dataViewMax = CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMax());
	int nIndex = GetIndex(dataIndex, dataStart);
	CPoint dataPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, realWidth, dataViewMin, dataViewMax, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pDataPacket->GetnumericDataList()->GetNext(dataPos)));
//	CPoint startPt = CDataConversion::GetVerticalPoint(graphRegion, count, realWidth, dataViewMin, dataViewMax, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pDataPacket->GetnumericDataList()->GetNext(dataPos)), false);
//	CPoint endPt = CDataConversion::GetVerticalPoint(graphRegion, count+1, realWidth, dataViewMin, dataViewMax, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pDataPacket->GetnumericDataList()->GetNext(dataPos)), false);

	//invert scale을 위해서.
	CPoint mousePoint = point;
	if(pSelectSubGraphData->GetVertScaleInvertType())
		mousePoint.y = CDataConversion::InvertGraphDataPt(graphRegion, point.y);

	int nGap = int(realWidth*0.8/2.0);
	if(nGap < 6)
		nGap = 6;
	if(eGraphStyle == CGraphBaseData::HorizontalBar || eGraphStyle == CGraphBaseData::TextHorizontalBar){
		realWidth = (double)graphRegion.Height()/(double)pageDataCount;
		double viewDataMin = 0.0, viewDataMax = 0;
		pDataPacket->GetMinMax(0, pDataPacket->GetCount(), viewDataMin, viewDataMax);
		viewDataMin = 0.0;
		dataPos = pDataPacket->GetnumericDataList()->FindIndex(dataIndex);
		dataPt = CDataConversion::GetHorizontalPoint(graphRegion, nIndex, realWidth, viewDataMin, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), viewDataMax), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pDataPacket->GetnumericDataList()->GetNext(dataPos)));
		if(nGap > 6)
			nGap = int(realWidth*0.8/2.0);
	}
	// 고정비율상하식 추가 - ojtaso (20080603)
	else if(eGraphStyle == CGraphBaseData::UpDownBar || eGraphStyle == CGraphBaseData::FixedRateUpDownBar)
	{
		double standardData = 0.0;   // 임이의 기준가~~~
		CIndicatorInfo* pIndicatorInfo = NULL;
		if(pIndicatorList != NULL)
			pIndicatorInfo = pIndicatorList->GetGraphIndicatorInfo( EILT_USER, pSelectSubGraphData->GetGraphName(), pSelectSubGraphData->GetIndicatorName(), EIIGT_WITH_DEFAULT_GET);
		if(pIndicatorInfo != NULL){
			CList<double, double>* pBaseLineList = pIndicatorInfo->GetBaseLineList();
			if(pBaseLineList != NULL)
				standardData = pBaseLineList->GetHead();
		}
		CPoint standardDataPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, realWidth, dataViewMin, dataViewMax, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), standardData));
		CRect region(standardDataPt, standardDataPt);
		region.InflateRect(nGap, nGap);
		if(region.PtInRect(mousePoint))
			return dataIndex;
	}
	
	dataPt.x = area.center;
	if(pSelectSubGraphData->GetSubGraphStyle().GetGraphBarStyle() == CGraphBaseData::VerticalBar)
	{
		if(mousePoint.x >= area.left && mousePoint.x <= area.right
			&& mousePoint.y >= (dataPt.y-1) && mousePoint.y <= graphRegion.bottom)
			return dataIndex;
	}
	else
	{
		if(mousePoint.x >= area.left && mousePoint.x <= area.right
			&& mousePoint.y >= (dataPt.y-5) && mousePoint.y <= (dataPt.y+5))
			return dataIndex;
	}

	return -1;
}

int CSelectSubGraph::PointInBoxBarGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData, CIndicatorList* pIndicatorList)
{
	const CList< CPacket *, CPacket *> &packetList = pSelectSubGraphData->GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return -1;
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)	return -1;
	
	if(packetList.GetCount() < 6)	return -1;

	CPacket* pHighPacket = NULL;	
	CPacket* pLowPacket = NULL;	
	CPacket* pOpenPacket = NULL;	
	CPacket* pClosePacket = NULL;	

	/// Forward
	POSITION p = packetList.GetHeadPosition();
	if(p == NULL)	return -1;
	CPacket* pPacket = packetList.GetNext(p);
	if(pPacket == NULL)	return -1;

	CBarDraw barDraw;
	if(barDraw.CheckBoxBarType(pPacket)){
		pHighPacket = packetList.GetAt(packetList.FindIndex(0));	
		pLowPacket = packetList.GetAt(packetList.FindIndex(1));
	}
	/// Backward
	else{
		pHighPacket = packetList.GetAt(packetList.FindIndex(2));	
		pLowPacket = packetList.GetAt(packetList.FindIndex(3));
	}
	pOpenPacket = packetList.GetAt(packetList.FindIndex(4));
	pClosePacket = packetList.GetAt(packetList.FindIndex(5));	

	if(pOpenPacket == NULL || pHighPacket == NULL || pLowPacket == NULL || pClosePacket == NULL)
		return -1;

	int nStart = pSelectSubGraphData->GetDisplayAttributes().GetDataStartInBlock();
	int nEnd = pSelectSubGraphData->GetDisplayAttributes().GetDataEndInBlock();
	int pageDataCount = pSelectSubGraphData->GetDisplayAttributes().GetDataCountInBlock();
	
	CRect graphRegion = pSelectSubGraphData->GetGraphRegions().GetDrawingRegion();

	int nDataIndex = GetDataIndex(graphRegion, point, nStart, nEnd, pageDataCount); 
	double dRealWidth = CDraw::CalRealWidth(graphRegion, nStart, nEnd);
	if(nDataIndex == -1)	return -1;

	POSITION openPos = pOpenPacket->GetnumericDataList()->FindIndex(nDataIndex);
	POSITION highPos = pHighPacket->GetnumericDataList()->FindIndex(nDataIndex);
	POSITION lowPos = pLowPacket->GetnumericDataList()->FindIndex(nDataIndex);
	POSITION closePos = pClosePacket->GetnumericDataList()->FindIndex(nDataIndex);
	
	if(openPos == NULL || highPos == NULL || lowPos == NULL || closePos == NULL) 
		return -1;

	int nIndex = GetIndex(nDataIndex, nStart);

	double dHigh = pHighPacket->GetnumericDataList()->GetAt(highPos);
	double dLow = pLowPacket->GetnumericDataList()->GetAt(lowPos);
	CPoint openPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dRealWidth, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMin()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMax()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pOpenPacket->GetnumericDataList()->GetNext(openPos)), false);
	CPoint highPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dRealWidth, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMin()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMax()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pHighPacket->GetnumericDataList()->GetNext(highPos)), true);
	CPoint lowPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dRealWidth, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMin()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMax()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pLowPacket->GetnumericDataList()->GetNext(lowPos)), true);
	CPoint closePt = CDataConversion::GetVerticalPoint(graphRegion, nIndex + 1, dRealWidth, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMin()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMax()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pClosePacket->GetnumericDataList()->GetNext(closePos)), false);

	//invert scale을 위해서.
	CPoint mousePt = point;
	if(pSelectSubGraphData->GetVertScaleInvertType())
		mousePt.y = CDataConversion::InvertGraphDataPt(graphRegion, point.y);

	if(highPt.y <= mousePt.y && lowPt.y >= mousePt.y && mousePt.x >= openPt.x && mousePt.x <= closePt.x)
		return nDataIndex;
		
	return -1;
}
/// 보류
int CSelectSubGraph::PointInLeftRightBarGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData, CIndicatorList* pIndicatorList)
{
	return -1;
}

/// 보류
int CSelectSubGraph::PointInGroupBarGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData, CIndicatorList* pIndicatorList)
{
	return -1;
}

int CSelectSubGraph::PointInLineGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData)
{
	if(pSelectSubGraphData == NULL || pSelectSubGraphData->GetSubGraphPacketList() == NULL)
		return -1;

	const CList< CPacket *, CPacket *> &packetList = pSelectSubGraphData->GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return -1;

	int nUnit = 8;
	CPacket* pPacket1 = NULL;
	CPacket* pPacket2 = NULL;
	bool checkCloudLine = false;
	// (2007/4/5 - Seung-Won, Bae) Check Line Graph Type for Bong Graph of Line Style.
	if( pSelectSubGraphData->GetSubGraphType() == CGraphBaseData::Line_Type)
	{
		if(pSelectSubGraphData->GetSubGraphStyle().GetGraphLineStyle() == CGraphBaseData::CloudLine
			|| pSelectSubGraphData->GetSubGraphStyle().GetGraphLineStyle() == CGraphBaseData::LineCloudLine
			|| pSelectSubGraphData->GetSubGraphStyle().GetGraphLineStyle() == CGraphBaseData::NonCloudLine)
			checkCloudLine = true;
	}

	//sy 2002.9.18.
	//POSITION pos = packetList.GetHeadPosition();
	POSITION pos = NULL;
	if( g_iMetaTable.IsViewVertMinMax_EndPacket(pSelectSubGraphData->GetGraphName()))
		pos = packetList.GetTailPosition();
	else
		pos = packetList.GetHeadPosition();
	if(pos == NULL)	
		return -1;
	//end

	pPacket1 = packetList.GetNext(pos);

	//구름식
	if(checkCloudLine){
		if(pos == NULL)
			return -1;
		pPacket2 = packetList.GetNext(pos);
	}
	
	//int dataStart = pSelectSubGraphData->GetDisplayAttributes().GetDataStartInBlock();
	//int dataEnd = pSelectSubGraphData->GetDisplayAttributes().GetDataEndInBlock();
	int pageDataCount = pSelectSubGraphData->GetDisplayAttributes().GetDataCountInBlock();
	
	int dataStart, dataEnd;
	pSelectSubGraphData->GetXScaleManager()->GetDrawIndex(pSelectSubGraphData->GetRQ(), FALSE, FALSE, dataStart, dataEnd);
	
	POSITION dataPos = pPacket1->GetnumericDataList()->FindIndex(dataStart);
	if(dataPos == NULL) 
		return -1;

	CRect graphRegion = pSelectSubGraphData->GetGraphRegions().GetDrawingRegion();

	//int dataIndex = GetDataIndex(graphRegion, point, dataStart, dataEnd, pageDataCount); 
	int dataIndex;
	int nCol = 0;
	CXScaleManager::AREA area;
	pSelectSubGraphData->GetXScaleManager()->GetDTInAREAfRQnPt(pSelectSubGraphData->GetRQ(), point.x, nCol, &dataIndex, &area);

	if(dataIndex == -1)
		return -1;
	 
	if(area.left<0 || area.center<0)
		return -1;

	dataPos = pPacket1->GetnumericDataList()->FindIndex(dataIndex);
	
	POSITION dataPos2;
	//구름식
	if(checkCloudLine)
		dataPos2 = pPacket2->GetnumericDataList()->FindIndex(dataIndex);

	bool packet1InIndex= true;
	bool packet2InIndex= true;

	if(dataPos == NULL)
		packet1InIndex = false;
	if(dataPos2 == NULL)
		packet2InIndex = false;

	double realWidth = CDraw::CalRealWidth(graphRegion, dataStart, dataEnd);
	int lineWidth = int(realWidth*(8.0/10.0));
	double realData = 0.0;
	double realData2 = realData;
	
	if(packet1InIndex)
		realData = pPacket1->GetnumericDataList()->GetNext(dataPos);
	
	//구름식
	if(checkCloudLine){
		if(packet2InIndex)
			realData2 = pPacket1->GetnumericDataList()->GetNext(dataPos2);
	}

	// NonZeroLine일 경우
	if(pSelectSubGraphData->GetSubGraphStyle().GetGraphBarStyle() == CGraphBaseData::NonZeroLine){
		if(realData == 0)
			return -1;
	}
	double dataViewMin = CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMin());
	double dataViewMax = CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMax());
	int nIndex = GetIndex(dataIndex, dataStart);
	CPoint dataPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, realWidth, dataViewMin, dataViewMax, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), realData));
	CPoint dataPt2 = dataPt;

	if(checkCloudLine)
		dataPt2 = CDataConversion::GetVerticalPoint(graphRegion, nIndex, realWidth, dataViewMin, dataViewMax, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), realData2));

	int realPointY = 0;
	int realPointY2 = 0;
	double tempData = 0.0, tempData2 = 0.0;

	//invert scale을 위해서.
	CPoint mousePoint = point;
	if(pSelectSubGraphData->GetVertScaleInvertType())
		mousePoint.y = CDataConversion::InvertGraphDataPt(graphRegion, point.y);

	dataPt.x = dataPt2.x = area.center;

	//일반적인 line인 경우 ===============================================================================
	if(dataPt.x < point.x){
		//수평식 line 일 경우
		if(pSelectSubGraphData->GetSubGraphStyle().GetGraphBarStyle() == CGraphBaseData::HorizontalLine){
			if(packet1InIndex == true && dataPt.x + lineWidth >= mousePoint.x && mousePoint.y <= dataPt.y + nUnit && mousePoint.y >= dataPt.y - nUnit)
				return dataIndex;
			else
				return -1;
		}

		if(dataPos == NULL)
			return -1;
		tempData = pPacket1->GetnumericDataList()->GetNext(dataPos);
		
		if(checkCloudLine){
			if(dataPos2 == NULL)
				return -1;
			else
				tempData2 = pPacket2->GetnumericDataList()->GetNext(dataPos2);
		}

		// NonZeroLine일 경우
		if(pSelectSubGraphData->GetSubGraphStyle().GetGraphBarStyle() == CGraphBaseData::NonZeroLine){
			if(tempData == 0)
				return -1;
		}
		CPoint tempDataPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex+1, realWidth, dataViewMin, dataViewMax, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), tempData));
		CPoint tempDataPt2 = tempDataPt;
		if(checkCloudLine)
			tempDataPt2 = CDataConversion::GetVerticalPoint(graphRegion, nIndex+1, realWidth, dataViewMin, dataViewMax, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), tempData2));
		
		//{{XScale적용
		pSelectSubGraphData->GetXScaleManager()->GetAREAfRQnDTI(pSelectSubGraphData->GetRQ(), dataIndex+1, nCol, &area);
		tempDataPt.x = tempDataPt2.x = area.center;
		if(dataPt.x == tempDataPt.x)
			return -1;
		//}}
		//StairLine
		if(pSelectSubGraphData->GetSubGraphStyle().GetGraphBarStyle() == CGraphBaseData::StairLine){
			if(packet1InIndex == true && dataPt.x + lineWidth/2 >= mousePoint.x && mousePoint.y <= dataPt.y + nUnit && mousePoint.y >= dataPt.y - nUnit)
				return dataIndex;
			
			else if(packet1InIndex == true && mousePoint.x <= dataPt.x + realWidth/2 + nUnit && mousePoint.x >= dataPt.x + realWidth/2 - nUnit && tempDataPt.y > dataPt.y){
				if(point.y <= tempDataPt.y && mousePoint.y >= dataPt.y)
					return dataIndex;
			}
			else if(packet1InIndex == true && mousePoint.x <= dataPt.x + realWidth/2 + nUnit && mousePoint.x >= dataPt.x + realWidth/2 - nUnit && tempDataPt.y < dataPt.y){
				if(point.y >= tempDataPt.y && mousePoint.y <= dataPt.y)
					return dataIndex;
			}
			else
				return -1;
		}
		
		//realPointY = (tempDataPt.y - dataPt.y) * (mousePoint.x - dataPt.x) / (tempDataPt.x - dataPt.x) + dataPt.y;
		if(tempDataPt.y > dataPt.y)
			realPointY = (tempDataPt.y - dataPt.y) * (mousePoint.x - dataPt.x) / (tempDataPt.x - dataPt.x) + dataPt.y;
		else
			realPointY = dataPt.y - (dataPt.y - tempDataPt.y) * (mousePoint.x - dataPt.x) / (tempDataPt.x - dataPt.x);

		if(checkCloudLine){
			//realPointY2 = (tempDataPt2.y - dataPt2.y) * (mousePoint.x - dataPt2.x) / (tempDataPt2.x - dataPt2.x) + dataPt2.y;
			if(tempDataPt2.y > dataPt2.y)
				realPointY2 = (tempDataPt2.y - dataPt2.y) * (mousePoint.x - dataPt2.x) / (tempDataPt2.x - dataPt2.x) + dataPt2.y;
			else
				realPointY2 = dataPt2.y - (dataPt2.y - tempDataPt2.y) * (mousePoint.x - dataPt2.x) / (tempDataPt2.x - dataPt2.x);
		}
			

		if(packet1InIndex == true && realPointY >= mousePoint.y - nUnit && realPointY <= mousePoint.y + nUnit)
			return dataIndex;
		if(checkCloudLine){
			if(packet2InIndex == true && realPointY2 >= mousePoint.y - nUnit && realPointY2 <= mousePoint.y + nUnit)
				return dataIndex;
		}

	}
	else if(dataPt.x > mousePoint.x){
		//수평식 line 일 경우
		if(pSelectSubGraphData->GetSubGraphStyle().GetGraphBarStyle() == CGraphBaseData::HorizontalLine){
			if(packet1InIndex == true && dataPt.x - lineWidth <= mousePoint.x && mousePoint.y <= dataPt.y + nUnit && mousePoint.y >= dataPt.y - nUnit)
				return dataIndex;
			else
				return -1;
		}

		if(dataIndex -1 < 0)
			return -1;
		dataPos = pPacket1->GetnumericDataList()->FindIndex(dataIndex -1);
		if(dataPos == NULL)
			return -1;
		tempData = pPacket1->GetnumericDataList()->GetNext(dataPos);
		if(checkCloudLine){
			if(dataPos2 == NULL)
				return -1;
			else
				tempData2 = pPacket2->GetnumericDataList()->GetNext(dataPos2);
		}

		// NonZeroLine일 경우
		if(pSelectSubGraphData->GetSubGraphStyle().GetGraphBarStyle() == CGraphBaseData::NonZeroLine){
			if(tempData == 0)
				return -1;
		}

		CPoint tempDataPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex-1, realWidth, dataViewMin, dataViewMax, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), tempData));
		CPoint tempDataPt2 = tempDataPt;
		if(checkCloudLine)
			tempDataPt2 = CDataConversion::GetVerticalPoint(graphRegion, nIndex-1, realWidth, dataViewMin, dataViewMax, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), tempData2));
		
		//{{XScale적용
		pSelectSubGraphData->GetXScaleManager()->GetAREAfRQnDTI(pSelectSubGraphData->GetRQ(), dataIndex-1, nCol, &area);
		tempDataPt.x = tempDataPt2.x = area.center;
		if(dataPt.x == tempDataPt.x)
			return -1;
		//}}
		//StairLine
		if(pSelectSubGraphData->GetSubGraphStyle().GetGraphBarStyle() == CGraphBaseData::StairLine)
		{
			// (2009/11/6 - Seung-Won, Bae) Do not disable zoom and scroll on OneChart
			mousePoint.x += lineWidth/2;
			if(packet1InIndex == true && dataPt.x <= mousePoint.x && mousePoint.y <= tempDataPt.y + nUnit && mousePoint.y >= tempDataPt.y - nUnit)
				return dataIndex;
			else if(packet1InIndex == true && mousePoint.x <= dataPt.x - realWidth/2 + nUnit && mousePoint.x >= dataPt.x - realWidth/2 - nUnit && tempDataPt.y > dataPt.y){
				if(point.y <= tempDataPt.y && mousePoint.y >= dataPt.y)
					return dataIndex;
			}
			else if(packet1InIndex == true && mousePoint.x <= dataPt.x - realWidth/2 + nUnit && mousePoint.x >= dataPt.x - realWidth/2 - nUnit && tempDataPt.y < dataPt.y){
				if(point.y >= tempDataPt.y && mousePoint.y <= dataPt.y)
					return dataIndex;
			}
			else
				return -1;
		}
		
		//realPointY = ((tempDataPt.y - dataPt.y) * (dataPt.x - mousePoint.x) / (tempDataPt.x - dataPt.x)) + dataPt.y;
		if(tempDataPt.y > dataPt.y)
			realPointY = ((tempDataPt.y - dataPt.y) * (mousePoint.x - dataPt.x) / (tempDataPt.x - dataPt.x)) + dataPt.y;
		else
			realPointY = dataPt.y - ((dataPt.y - tempDataPt.y) * (mousePoint.x - dataPt.x) / (tempDataPt.x - dataPt.x));
		
		if(checkCloudLine){
		//	realPointY2 = (tempDataPt2.y - dataPt2.y) * (dataPt2.x - mousePoint.x) / (tempDataPt2.x - dataPt2.x) + dataPt2.y;
			if(tempDataPt2.y > dataPt2.y)
				realPointY2 = ((tempDataPt2.y - dataPt2.y) * (mousePoint.x - dataPt2.x) / (tempDataPt2.x - dataPt2.x)) + dataPt2.y;
			else
				realPointY2 = dataPt2.y - ((dataPt2.y - tempDataPt2.y) * (mousePoint.x - dataPt2.x) / (tempDataPt2.x - dataPt2.x));
		}
			
		
		if(packet1InIndex == true && realPointY >= mousePoint.y - nUnit && realPointY <= mousePoint.y + nUnit)
			return dataIndex;
		if(checkCloudLine){
			if(packet2InIndex == true && realPointY2 >= mousePoint.y - nUnit && realPointY2 <= mousePoint.y + nUnit)
				return dataIndex;
		}

	}
	else if(packet1InIndex == true && dataPt.x == mousePoint.x && mousePoint.y <= dataPt.y + nUnit && mousePoint.y >= dataPt.y - nUnit)
		return dataIndex;
	else if(packet2InIndex == true && dataPt2.x == mousePoint.x && mousePoint.y <= dataPt2.y + nUnit && mousePoint.y >= dataPt2.y - nUnit)
		return dataIndex;

	return -1;
}

/*
int CSelectSubGraph::PointInDataBarGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData)
{
	if(pSelectSubGraphData->GetSubGraphDrawStyle().GetGraphDataBarDrawStyle() != CGraphBaseData::Fill_Bar)
		return -1;

	CList<CPacket*, CPacket*>& packetList = *pSelectSubGraphData->GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return -1;

	CPacket* m_dataPacket;
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)	
		return -1;
	m_dataPacket = packetList.GetNext(pos);

	int dataStart = pSelectSubGraphData->GetDisplayAttributes().GetDataStartInBlock();
	int dataEnd = pSelectSubGraphData->GetDisplayAttributes().GetDataEndInBlock();
	int pageDataCount = pSelectSubGraphData->GetDisplayAttributes().GetDataCountInBlock();

	// (2006/10/11 - Seung-Won, Bae) Use GetDrawingRegion();
	CRect graphRegion = pSelectSubGraphData->GetGraphRegions().GetDrawingRegion( CGraphBaseData::DataView_Type, pSelectSubGraphData->GetSubGraphStyle());

	int dataIndex = GetDataIndex(graphRegion, point, dataStart, dataEnd, pageDataCount); 
	double realWidth = CDraw::CalRealWidth(graphRegion, dataStart, dataEnd);
	if(dataIndex == -1)
		return -1;

	POSITION dataPos = m_dataPacket->GetnumericDataList()->FindIndex(dataIndex);
	
	if(dataPos == NULL) 
		return -1;
	double dataMin= 0;
	double dataMax= 10;

	double dData = m_dataPacket->GetnumericDataList()->GetNext(dataPos);
	if(dData < 1)
		return -1;
	if(point.y <= graphRegion.top && point.y >= graphRegion.bottom)
		return -1;

	int nIndex = GetIndex(dataIndex, dataStart);
	CPoint dataPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, realWidth, dataMin, dataMax, dData);

	if(dData == 1){
		if(point.y >= graphRegion.top && point.y <= graphRegion.top + graphRegion.Height()/2)
			return dataIndex;
	}
	else if(dData == 2){
		if(point.y >= graphRegion.top + graphRegion.Height()/2 && point.y <= graphRegion.bottom)
			return dataIndex;
	}
	else
		return dataIndex;

	return -1;
}
*/

int CSelectSubGraph::PointInTopBottomSignalGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData)
{
	bool checkUpDown = true;
	if(pSelectSubGraphData->GetSubGraphStyle().GetGraphDataViewStyle() == CGraphBaseData::BottomSignal)
		checkUpDown = false;

	const CList< CPacket *, CPacket *> &packetList = pSelectSubGraphData->GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return -1;

	CPacket* dataPacket = NULL;
	CPacket* pointPacket = NULL;
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)	
		return -1;
	dataPacket = packetList.GetNext(pos);
	if(pos == NULL)	
		return -1;
	pointPacket = packetList.GetNext(pos);

	int dataStart = pSelectSubGraphData->GetDisplayAttributes().GetDataStartInBlock();
	int dataEnd = pSelectSubGraphData->GetDisplayAttributes().GetDataEndInBlock();
	int pageDataCount = pSelectSubGraphData->GetDisplayAttributes().GetDataCountInBlock();

	CRect graphRegion = pSelectSubGraphData->GetGraphRegions().GetDrawingRegion();
	int dataIndex = 0;
	dataIndex = GetDataIndex(graphRegion, point, dataStart, dataEnd, pageDataCount); 
	double realWidth = CDraw::CalRealWidth(graphRegion, dataStart, dataEnd);
	if(dataIndex == -1)
		return -1;

	POSITION dataPos = NULL;
	POSITION pointPos = NULL;
	if(dataPacket->GetnumericDataList() && pointPacket->GetnumericDataList()){
		dataPos = dataPacket->GetnumericDataList()->FindIndex(dataIndex);
		pointPos = pointPacket->GetnumericDataList()->FindIndex(dataIndex);
	}
	else
		return -1;
	
	
	if(dataPos == NULL) 
		return -1;

	double dData = dataPacket->GetnumericDataList()->GetNext(dataPos);
	if(dData < 1)
		return -1;
	
	int nWidth = CMath::Round(realWidth/* 2/3*/);
	if(nWidth > 15)
		nWidth = 15;
	else if(nWidth < 10)
		nWidth = 10;

	int nIndex = GetIndex(dataIndex, dataStart);
	CPoint dataPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, realWidth, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMin()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMax()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pointPacket->GetnumericDataList()->GetNext(pointPos)));

	int unit = 2;
	
	if(checkUpDown){
		dataPt.y = dataPt.y - 4;
		if(dData == 1){
			if(point.y <= dataPt.y + unit && point.y >= dataPt.y - nWidth - unit && point.x >= dataPt.x - nWidth -unit && point.x <= dataPt.x + nWidth + unit)
				return dataIndex;
		}
		else{
			if(point.y <= dataPt.y + unit&& point.y >= dataPt.y - nWidth*2 - unit && point.x >= dataPt.x - nWidth - unit && point.x <= dataPt.x + nWidth + unit)
				return dataIndex;
		}
	}
	else{
		dataPt.y = dataPt.y + 4;
		if(dData == 1){
			if(point.y >= dataPt.y - unit && point.y <= dataPt.y + nWidth + unit && point.x >= dataPt.x - nWidth - unit && point.x <= dataPt.x + nWidth + unit)
				return dataIndex;
		}
		else{
			if(point.y >= dataPt.y - unit && point.y <= dataPt.y + nWidth*2 + unit && point.x >= dataPt.x - nWidth - unit && point.x <= dataPt.x + nWidth + unit)
				return dataIndex;
		}
	}
	return -1;
}

int CSelectSubGraph::PointInSolidLineBongGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData)
{
	const CList< CPacket *, CPacket *> &packetList = pSelectSubGraphData->GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return -1;

	int nUnit = 10;
	CPacket* pPacket = NULL;
	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)	
		return -1;
	pPacket = packetList.GetNext(pos);
	if(pPacket == NULL)	return -1;

	int dataStart = pSelectSubGraphData->GetDisplayAttributes().GetDataStartInBlock();
	int dataEnd = pSelectSubGraphData->GetDisplayAttributes().GetDataEndInBlock();
	int pageDataCount = pSelectSubGraphData->GetDisplayAttributes().GetDataCountInBlock();

///	POSITION dataPos = pPacket->GetnumericDataList()->FindIndex(dataStart-1);
	POSITION dataPos = pPacket->GetnumericDataList()->FindIndex(dataStart);
	if(dataPos == NULL) 
		return -1;

	CRect graphRegion = pSelectSubGraphData->GetGraphRegions().GetDrawingRegion();

	int dataIndex = GetDataIndex(graphRegion, point, dataStart, dataEnd, pageDataCount); 
	//int dataIndex;
	int nCol = 0;
	pSelectSubGraphData->GetXScaleManager()->GetDTInAREAfRQnPt(pSelectSubGraphData->GetRQ(), point.x, nCol, &dataIndex, NULL);

	if(dataIndex == -1)
		return -1;

	dataPos = pPacket->GetnumericDataList()->FindIndex(dataIndex);
	
	bool packet1InIndex= true;

	if(dataPos == NULL)
		packet1InIndex = false;

	double realWidth = CDraw::CalRealWidth(graphRegion, dataStart, dataEnd);
	int lineWidth = int(realWidth*(8.0/10.0));
	double realData = 0.0;
	double realData2 = realData;
	
	if(packet1InIndex)
		realData = pPacket->GetnumericDataList()->GetNext(dataPos);
	
	double dataViewMin = CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMin());
	double dataViewMax = CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMax());

	int nIndex = GetIndex(dataIndex, dataStart);
	CPoint dataPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, realWidth, dataViewMin, dataViewMax, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), realData));

	int realPointY = 0;
	int realPointY2 = 0;
	double tempData = 0.0, tempData2 = 0.0;

	//invert scale을 위해서.
	CPoint mousePoint = point;
	if(pSelectSubGraphData->GetVertScaleInvertType())
		mousePoint.y = CDataConversion::InvertGraphDataPt(graphRegion, point.y);

	//일반적인 line인 경우 ===============================================================================
	if(dataPt.x < mousePoint.x){
	
		if(dataPos == NULL)
			return -1;
		tempData = pPacket->GetnumericDataList()->GetNext(dataPos);
		
		CPoint tempDataPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex+1, realWidth, dataViewMin, dataViewMax, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), tempData));
			
		if(tempDataPt.y > dataPt.y)
			realPointY = (tempDataPt.y - dataPt.y) * (mousePoint.x - dataPt.x) / (tempDataPt.x - dataPt.x) + dataPt.y;
		else 
			realPointY = dataPt.y - (dataPt.y - tempDataPt.y) * (mousePoint.x - dataPt.x) / (tempDataPt.x - dataPt.x);
		if(packet1InIndex == true && realPointY >= mousePoint.y - nUnit && realPointY <= mousePoint.y + nUnit)
			return dataIndex;
	}
	else if(dataPt.x > mousePoint.x){
		if(dataIndex -1 < 0)
			return -1;
		dataPos = pPacket->GetnumericDataList()->FindIndex(dataIndex -1);
		if(dataPos == NULL)
			return -1;
		tempData = pPacket->GetnumericDataList()->GetNext(dataPos);
	
		CPoint tempDataPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex-1, realWidth, dataViewMin, dataViewMax, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), tempData));
		
		if(tempDataPt.y > dataPt.y)
			realPointY = ((tempDataPt.y - dataPt.y) * (mousePoint.x - dataPt.x) / (tempDataPt.x - dataPt.x)) + dataPt.y;
		else 
			realPointY = dataPt.y - ((dataPt.y - tempDataPt.y) * (mousePoint.x - dataPt.x) / (tempDataPt.x - dataPt.x));
		if(packet1InIndex == true && realPointY >= mousePoint.y - nUnit && realPointY <= mousePoint.y + nUnit)
			return dataIndex;
	
	}
	else if(packet1InIndex == true && dataPt.x == mousePoint.x && mousePoint.y <= dataPt.y + nUnit && mousePoint.y >= dataPt.y - nUnit)
		return dataIndex;
	
	return -1;
}

int CSelectSubGraph::PointInThreeLineBreakGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData)
{
	const CList< CPacket *, CPacket *> &packetList = pSelectSubGraphData->GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return -1;

	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)	return -1;
	
	CPacket* pOpenPacket = packetList.GetNext(pos);
	CPacket* pClosePacket = packetList.GetNext(pos);

	if(pOpenPacket == NULL || pClosePacket == NULL)
		return -1;

	int dataStart = 0;
	int dataEnd = pOpenPacket->GetCount() -1;
	int pageDataCount = dataEnd - dataStart + 1;

	CRect graphRegion = pSelectSubGraphData->GetGraphRegions().GetDrawingRegion();
	int dataIndex = 0;
	int nRealCount = pOpenPacket->GetCount();
	dataIndex = GetDataIndex(graphRegion, point, dataStart, dataEnd, pageDataCount); 

	if(dataIndex == -1)
		return -1;
	 
	POSITION dataPos = pOpenPacket->GetnumericDataList()->FindIndex(dataIndex);
	if(dataPos == NULL) 
		return -1;

	double realWidth = CDraw::CalRealWidth(graphRegion, dataStart, dataEnd);
	double dataViewMin = CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMin());
	double dataViewMax = CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMax());
	int nIndex = GetIndex(dataIndex, dataStart);

	// Graph Drawing 때 오른쪽 여백 적용 - ojtaso (20080124)
	int nCol = 0;
	CXScaleManager::AREA area;
	pSelectSubGraphData->GetXScaleManager()->GetDTInAREAfRQnPt(pSelectSubGraphData->GetRQ(), point.x, nCol, &dataIndex, &area);
	
//	int nGap = int(realWidth * 0.8 / 2.0);
//	if(nGap < 6)
//		nGap = 6;
	realWidth = (double)graphRegion.Width()/(double)pageDataCount;
	double viewDataMin = 0.0, viewDataMax = 0;
	pOpenPacket->GetMinMax(0, pOpenPacket->GetCount(), viewDataMin, viewDataMax);
	viewDataMin = 0.0;

	POSITION openPos = pOpenPacket->GetnumericDataList()->FindIndex(dataIndex);
	POSITION closePos = pClosePacket->GetnumericDataList()->FindIndex(dataIndex);
	
	if(openPos == NULL || closePos == NULL) 
		return -1;

	double dOpen = CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pOpenPacket->GetnumericDataList()->GetAt(openPos));
	double dClose = CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pClosePacket->GetnumericDataList()->GetAt(closePos));

	CPoint openPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, realWidth, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMin()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMax()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pOpenPacket->GetnumericDataList()->GetNext(openPos)), false);
	CPoint closePt = CDataConversion::GetVerticalPoint(graphRegion, nIndex+1, realWidth, CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMin()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMax()), CDraw::CalLogValue(pSelectSubGraphData->GetVertScaleLogType(), pClosePacket->GetnumericDataList()->GetNext(closePos)), false);

	//invert scale을 위해서.
	CPoint mousePoint = point;
	if(pSelectSubGraphData->GetVertScaleInvertType())
		mousePoint.y = CDataConversion::InvertGraphDataPt(graphRegion, point.y);

	// Graph Drawing 때 오른쪽 여백 적용 - ojtaso (20080124)
	openPt.x = area.left;
	closePt.x = area.right;

	if(openPt.y <= closePt.y){
		if(openPt.y <= mousePoint.y && closePt.y >= mousePoint.y && mousePoint.x >= openPt.x && mousePoint.x <= closePt.x)
		return dataIndex;
	}
	else{
		if(openPt.y >= mousePoint.y && closePt.y <= mousePoint.y && mousePoint.x >= openPt.x && mousePoint.x <= closePt.x)
		return dataIndex;
	}

	return -1;
}

//sy 2004.2.17.
// (2007/5/3 - Seung-Won, Bae) for Inquiry Full Data.
int CSelectSubGraph::PointInInverseLineGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData)
{
	int nDataCount = 0, nStartIndex = 0;
	CList<double, double>* pCloseDataList = NULL, *pVolumeDataList = NULL;
	POSITION closeStartPos = NULL, closeEndPos = NULL, volumeStartPos = NULL, volumeEndPos = NULL;
	if(!GetPacketData_InverseLineGraph(pSelectSubGraphData, nDataCount, 
		pCloseDataList, pVolumeDataList, closeStartPos, closeEndPos, 
		volumeStartPos, volumeEndPos, nStartIndex))
		return -1;

	CRect graphRegion = pSelectSubGraphData->GetGraphRegions().GetDrawingRegion();
	double dVertViewMin = pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMin();
	double dVertViewMax = pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMax(); 
	double dHorzViewMin = pSelectSubGraphData->GetDisplayDataHorzMinMax().GetDisplayMin();
	double dHorzViewMax = pSelectSubGraphData->GetDisplayDataHorzMinMax().GetDisplayMax();

	int nUnit = 8;
	bool bIsInvert = pSelectSubGraphData->GetVertScaleInvertType();
	CPoint prevPt = CDataConversion::GetVerticalPoint(graphRegion, 
				dVertViewMin, dVertViewMax, dHorzViewMin, dHorzViewMax, 
				pCloseDataList->GetNext( closeStartPos), pVolumeDataList->GetNext( volumeStartPos));

	for(int nIndex = nStartIndex + 1; nIndex < nDataCount; nIndex++)
	{
		if(closeStartPos == NULL || volumeStartPos == NULL)
			break;

		CPoint curPt = CDataConversion::GetVerticalPoint(graphRegion, 
				dVertViewMin, dVertViewMax, dHorzViewMin, dHorzViewMax, 
				pCloseDataList->GetNext(closeStartPos), pVolumeDataList->GetNext(volumeStartPos));
		if(bIsInvert)
			curPt.y = CDataConversion::InvertGraphDataPt(graphRegion, curPt.y);

		CRgn rgn;
		if(abs(curPt.x - prevPt.x) <= abs(curPt.y - prevPt.y))
		{
			CPoint ptArray[4];
			ptArray[0] = CPoint(prevPt.x - nUnit, prevPt.y);
			ptArray[1] = CPoint(prevPt.x + nUnit, prevPt.y);
			ptArray[2] = CPoint(curPt.x + nUnit, curPt.y);
			ptArray[3] = CPoint(curPt.x - nUnit, curPt.y);
			rgn.CreatePolygonRgn(ptArray, 4, WINDING);
		}
		else
		{
			CPoint ptArray[4];
			ptArray[0] = CPoint(prevPt.x, prevPt.y - nUnit);
			ptArray[1] = CPoint(prevPt.x, prevPt.y + nUnit);
			ptArray[2] = CPoint(curPt.x, curPt.y + nUnit);
			ptArray[3] = CPoint(curPt.x, curPt.y - nUnit);
			rgn.CreatePolygonRgn(ptArray, 4, WINDING);
		}
		if(rgn.PtInRegion(point))
		{
			// (2007/5/3 - Seung-Won, Bae) for Inquiry Full Data.
			if( abs( point.x - prevPt.x) < abs( curPt.x - point.x) || abs( point.y - prevPt.y) < abs( curPt.y - point.y))
				return nIndex - 1;
			
			return nIndex;
		}
		prevPt = curPt;
	}

	return -1;
}
bool CSelectSubGraph::GetPacketData_InverseLineGraph(const CSelectSubGraphData* pSelectSubGraphData, 
		int& nDataCount, CList<double, double>*& pCloseDataList, CList<double, double>*& pVolumeDataList, 
		POSITION& closeStartPos, POSITION& closeEndPos, POSITION& volumeStartPos, POSITION& volumeEndPos, int &p_nStartIndex) const
{
	if(pSelectSubGraphData == NULL || pSelectSubGraphData->GetSubGraphPacketList() == NULL)
		return false;

	const CList< CPacket *, CPacket *> &packetList = pSelectSubGraphData->GetSubGraphPacketList()->GetSubGraphPacketList();
	if( packetList.GetCount() < 2)
		return false;

	// 종가 packet
	CPacket* pClosePacket = packetList.GetHead();
	if(pClosePacket == NULL)
		return false;
	pCloseDataList = pClosePacket->GetnumericDataList();
	if(pCloseDataList == NULL)
		return false;
	p_nStartIndex = pClosePacket->GetStartPeriod();
	closeStartPos = closeEndPos = pCloseDataList->FindIndex( p_nStartIndex);

	//거래량 packet
	CPacket* pVolumePacket = packetList.GetAt(packetList.FindIndex(1));
	if(pVolumePacket == NULL)
		return false;
	pVolumeDataList = pVolumePacket->GetnumericDataList();
	if(pVolumeDataList == NULL)
		return false;
	volumeStartPos = volumeEndPos = pVolumeDataList->FindIndex( p_nStartIndex);

	nDataCount = pCloseDataList->GetCount();
	if(nDataCount > pVolumeDataList->GetCount())
		nDataCount = pVolumeDataList->GetCount();

	return (closeStartPos != NULL && volumeStartPos != NULL);
}
//sy end

//sy 2003.6.11. -> P&F 그래프를 선택할수 있게 하기
int CSelectSubGraph::PointInPAndFGraph(const CPoint& point, const CSelectSubGraphData* pSelectSubGraphData)
{
	if(pSelectSubGraphData == NULL)
		return -1;

	CSubGraphPacketList* pSubGraphPacketList = pSelectSubGraphData->GetSubGraphPacketList();
	if(pSubGraphPacketList == NULL)
		return -1;

	CPAndFDraw PAndFDraw;
	CPacket* pDatePacket = NULL;
	CList<double, double>* pHighList = NULL, *pLowList = NULL;
	if(!PAndFDraw.GetPacketData( pSubGraphPacketList->GetSubGraphPacketList(), 
		pDatePacket, pHighList, pLowList))
		return -1;

	double dUnitValue = PAndFDraw.GetUnitValue( pSubGraphPacketList->GetSubGraphPacketList().GetHead()->GetPacketList(), 
		pSelectSubGraphData->GetIndicatorInfo());
	if(dUnitValue < 1)
		return -1;

	CRect graphRegion = pSelectSubGraphData->GetGraphRegions().GetDrawingRegion();
	double dViewMin = pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMin();
	double dViewMax = pSelectSubGraphData->GetDisplayDataVertMinMax().GetDisplayMax();
	double dUnitHeight = CPAndFDraw().GetUintHeight(graphRegion, dViewMin, dViewMax, dUnitValue);
	
	// (2009/11/6 - Seung-Won, Bae) Do not disable zoom and scroll on OneChart
	// Graph Drawing 때 오른쪽 여백 적용 - ojtaso (20080124)
	int nCol = 0;
	int nIndex = 0;
	CXScaleManager::AREA area;
	pSelectSubGraphData->GetXScaleManager()->GetDTInAREAfRQnPt( pSelectSubGraphData->GetRQ(), point.x, nCol, &nIndex, &area);
	POSITION highPos = pHighList->FindIndex( nIndex);
	POSITION lowPos = pLowList->FindIndex( nIndex);
	if( highPos != NULL && lowPos != NULL)
	{
		double dHigh = pHighList->GetNext(highPos);
		double dLow = pLowList->GetNext(lowPos);

		CPoint highPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex -1, dUnitHeight, dViewMin, dViewMax, dHigh);
		CPoint lowPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dUnitHeight, dViewMin, dViewMax, dLow);
		CRect rect( area.left, highPt.y, area.right, lowPt.y);
		rect.NormalizeRect();

		if( rect.PtInRect(point))
			return nIndex;
	}
	return -1;
}

//몇번째 데이터 인지 가지고 오는 부분 ========================================================================
int CSelectSubGraph::GetDataIndex(CRect drawingRegion, CPoint point, int dataStart, int dataEnd, int pageDataCount)
{
	drawingRegion.right = drawingRegion.right;
	if(drawingRegion.right <= point.x)
		return -1;
	int index = CDataConversion::AbsoluteToRelative_X(drawingRegion, point.x, pageDataCount);
	if(index >= 0)
		index = dataStart + index;
	if(index > dataEnd)
		return -1;

	return index;
}

int CSelectSubGraph::GetDataIndex(CRect drawingRegion, CPoint point, int dataStart, int dataEnd)
{
	if(drawingRegion.bottom < point.y || point.y < drawingRegion.top)
		return -1;
	int index = 0;
	if(dataEnd - dataStart +1 <= 0)
		return index;

	//return (CMath::Round((double)((absolute_x - region.left)*pageDataCount) / (double)region.Width()));
	index =  int((double)((point.y - drawingRegion.top)*(dataEnd - (dataStart -1))) / (double)drawingRegion.Height());

	if(index >= 0)
		index = dataStart + index;
	if(index > dataEnd)
		return -1;

	return index;
}

/// dataStart로부터 몇번째 인지를 알려줌
int CSelectSubGraph::GetIndex(const int dataIndex, const int dataStart) const
{
	if(dataIndex < dataStart)
		return dataStart;
	
	return (dataIndex - dataStart);
}