// GripDraw.cpp: implementation of the CGripDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GripDraw.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"				// for _MTEXT()
#include "DataMath.h"
#include "Conversion.h"
#include "BarDraw.h"
#include "SpecialDraw.h"
#include "PacketPointerList.h"
#include "PacketBase.h"
#include "SelectSubGraph.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGripDraw::CGripDraw()
{

}

CGripDraw::~CGripDraw()
{

}

bool CGripDraw::DrawGrip(CDC* pDC, const int nIndex, const CSelectSubGraphData& selectSubGraphData)
{
	if(selectSubGraphData.GetSubGraphPacketList() == NULL || selectSubGraphData.GetSubGraphPacketList()->GetCount() <= 0) 
		return false;
	
	bool bLog = selectSubGraphData.GetVertScaleLogType();
	bool bInvert = selectSubGraphData.GetVertScaleInvertType();
	
	CRect graphRegion = selectSubGraphData.GetGraphRegions().GetDrawingRegion();

	if(graphRegion.top > graphRegion.bottom || graphRegion.left > graphRegion.right)
		return false;
	if(graphRegion.top > graphRegion.bottom || graphRegion.left > graphRegion.right)
		return false;

	switch(selectSubGraphData.GetSubGraphType()){
	case CGraphBaseData::Line_Type:
		return DrawGripAll(pDC, selectSubGraphData);
	case CGraphBaseData::Bong_Type:
		{
		if(selectSubGraphData.GetSubGraphDrawStyle().GetGraphBongDrawStyle() == CGraphBaseData::Line_Draw)
			return DrawLineBongGrip(pDC, selectSubGraphData);
		else
			return DrawGripBong(pDC, selectSubGraphData);
		return false;
		}
	case CGraphBaseData::Bar_Type:
		{
		if(selectSubGraphData.GetSubGraphStyle().GetGraphLineStyle() == CGraphBaseData::LeftRightBar)
			return DrawGripLeftRightBar(pDC, selectSubGraphData);
		if(selectSubGraphData.GetSubGraphStyle().GetGraphLineStyle() == CGraphBaseData::GroupBar)
			return DrawGripGroupBar(pDC, selectSubGraphData);
		if(selectSubGraphData.GetSubGraphStyle().GetGraphLineStyle() == CGraphBaseData::BoxBar)
			return DrawGripBoxBar(pDC, selectSubGraphData);
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
		if(selectSubGraphData.GetSubGraphStyle().GetGraphLineStyle() == CGraphBaseData::TextHorizontalBar)
			return false;
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
		else
			return DrawGripBar(pDC, selectSubGraphData);
		return false;
		}		
	case CGraphBaseData::DataView_Type:
		{
		if(selectSubGraphData.GetSubGraphStyle().GetGraphDataViewStyle() == CGraphBaseData::TopSignal || selectSubGraphData.GetSubGraphStyle().GetGraphDataViewStyle() == CGraphBaseData::BottomSignal)
			return DrawSignalGrip(pDC, nIndex, selectSubGraphData);
//		else if(selectSubGraphData.GetSubGraphStyle().GetGraphDataViewStyle() == CGraphBaseData::TopDataBar || selectSubGraphData.GetSubGraphStyle().GetGraphDataViewStyle() == CGraphBaseData::BottomDataBar)
//			return DrawDataBarGrip(pDC, nIndex, selectSubGraphData);
		return false;
		}
	case CGraphBaseData::Special_Type:
		{
		if(selectSubGraphData.GetSubGraphStyle().GetGraphDataViewStyle() == CGraphBaseData::ThreeLineBreak)
			return DrawThreeLineBreakGrip(pDC, selectSubGraphData);
		else if(selectSubGraphData.GetSubGraphStyle().GetGraphDataViewStyle() == CGraphBaseData::InverseLine)
			return DrawInverseLineGrip(pDC, selectSubGraphData);
//		else if(selectSubGraphData.GetSubGraphStyle().GetGraphDataViewStyle() == CGraphBaseData::PAndF)
//			return DrawPAndFGrip(pDC, selectSubGraphData);
		return false;
		}
//	case CGraphBaseData::FuOp_Type:
//		{
//		if(selectSubGraphData.GetSubGraphStyle().GetGraphFuOpStyle() == CGraphBaseData::FuOpPAndF)
//			return DrawPAndFGrip(pDC, selectSubGraphData);
//		return false;
//		}
	}
	
	return false;
}

/*
bool CGripDraw::DrawDataBarGrip(CDC* pDC, const int nIndex, const CSelectSubGraphData& selectSubGraphData)
{
	if(selectSubGraphData.GetSubGraphPacketList() == NULL)
		return false;

	CList<CPacket*, CPacket*>& packetList = *selectSubGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return false;

 	CPacket* dataPacket = NULL;
	int unit = 2;
	double realWidth = 0.0;
	POSITION Pos = packetList.GetHeadPosition();
	if(Pos == NULL)	return false;
	dataPacket = packetList.GetNext(Pos);
	POSITION dataPos = dataPacket->GetnumericDataList()->FindIndex(nIndex);

	if(dataPos == NULL)
		return false;
	int dataStart = selectSubGraphData.GetDisplayAttributes().GetDataStartInBlock();
	int dataEnd = selectSubGraphData.GetDisplayAttributes().GetDataEndInBlock();

	if(nIndex < dataStart || nIndex > dataEnd)
		return false;
	double dViewMin = 0;
	double dViewMax = 10;

	// (2006/10/11 - Seung-Won, Bae) Use GetDrawingRegion();
	CGraphBaseData::DATAVIEWSTYLE eDrawStyle = selectSubGraphData.GetSubGraphStyle().GetGraphDataViewStyle();
	if( eDrawStyle != CGraphBaseData::TopDataBar && eDrawStyle != CGraphBaseData::BottomDataBar) return false;
	CRect graphRegion = selectSubGraphData.GetGraphRegions().GetDrawingRegion( CGraphBaseData::DataView_Type, selectSubGraphData.GetSubGraphStyle());
	if( dataStart < 0) return false;
	
	realWidth = CDraw::CalRealWidth(graphRegion, dataStart, dataEnd);

	int nWidth = CMath::Round(realWidth/2);
	if(nWidth < 1)
		nWidth = 1;
	double data = dataPacket->GetnumericDataList()->GetNext(dataPos);
	if(data < 1)
		return false;
	CPoint dataPt; 
	dataPt = CDataConversion::GetVerticalPoint(graphRegion, (nIndex - dataStart), realWidth, dViewMin, dViewMax, CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), 5));

	CRect rect;
	if(data == 1)
		rect = CRect(dataPt.x - nWidth, graphRegion.top, dataPt.x + nWidth, graphRegion.top + graphRegion.Height()/2);
	else if(data == 2)
		rect = CRect(dataPt.x - nWidth, graphRegion.top + graphRegion.Height()/2, dataPt.x + nWidth, graphRegion.bottom);
	else if(data == 3)
		rect = CRect(dataPt.x - nWidth, graphRegion.top, dataPt.x + nWidth, graphRegion.bottom);
	else
		return false;

	rect.DeflateRect(rect.Width() / 9, rect.Height() / 9);
	rect.top = rect.top +1;

	CRect region;
	region = CRect(rect.left - unit, rect.top - unit, rect.left + unit, rect.top + unit);
	DrawGrip(pDC, region);
	region = CRect(rect.left - unit, rect.bottom - unit, rect.left + unit, rect.bottom + unit);
	DrawGrip(pDC, region);
	region = CRect(rect.right - unit, rect.top - unit, rect.right + unit, rect.top + unit);
	DrawGrip(pDC, region);
	region = CRect(rect.right - unit, rect.bottom - unit, rect.right + unit, rect.bottom + unit);
	DrawGrip(pDC, region);

	return true;
}
*/

bool CGripDraw::DrawLineBongGrip(CDC* pDC, const CSelectSubGraphData& selectSubGraphData)
{
 	const CList< CPacket *, CPacket *> &packetList = selectSubGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return false;

	CPacket* pPacket = NULL;
	int nUnit = 3;
	double dRealWidth = 0.0;
	
	POSITION Pos = packetList.GetHeadPosition();
	if(Pos == NULL)
		return false;

	if(packetList.GetCount() >= 4){
		POSITION packetPos = packetList.FindIndex(3);
		if(packetPos)
			pPacket = packetList.GetNext(packetPos);
	}
	else
		pPacket = packetList.GetNext(Pos);

	if(!pPacket)
		return false;

	int nStart, nEnd;
	selectSubGraphData.GetXScaleManager()->GetDrawIndex(FALSE, FALSE, nStart, nEnd);

	if(nStart < 0)	return false;
	
	dRealWidth = CDraw::CalRealWidth(selectSubGraphData.GetGraphRegions().GetDrawingRegion(), nStart, nEnd);

	int nGripInterval = CalGripInterval(selectSubGraphData.GetDisplayAttributes());
		
	int nGripNumber = 11;
	if(nGripInterval > 0)
		nGripNumber = (nEnd - nStart +1)/nGripInterval;
	nGripNumber++;
	if(nGripInterval == 1)
		nGripNumber = selectSubGraphData.GetDisplayAttributes().GetDataCountInBlock();

	double dViewMin = CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), selectSubGraphData.GetDisplayDataVertMinMax().GetDisplayMin());
	double dViewMax = CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), selectSubGraphData.GetDisplayDataVertMinMax().GetDisplayMax());

	CXScaleManager::AREA area;
	for(int i = 0; i < nGripNumber; i++){
		POSITION packet1Pos = pPacket->GetnumericDataList()->FindIndex(nStart);
		if(packet1Pos == NULL || nStart > nEnd)
			break;
		CPoint dataPt = CDataConversion::GetVerticalPoint(selectSubGraphData.GetGraphRegions().GetDrawingRegion(), nGripInterval*i, dRealWidth, dViewMin, dViewMax, CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), pPacket->GetnumericDataList()->GetNext(packet1Pos)));

		if(selectSubGraphData.GetVertScaleInvertType())
			dataPt.y = CDataConversion::InvertGraphDataPt(selectSubGraphData.GetGraphRegions().GetDrawingRegion(), dataPt.y);

		selectSubGraphData.GetXScaleManager()->GetAREAfRQnDTI(nStart, 0, &area);
		dataPt.x = area.center;

		CRect rect = CRect(dataPt.x - nUnit, dataPt.y - nUnit, dataPt.x + nUnit +1, dataPt.y + nUnit);
		if(pPacket->GetStartPeriod() < nStart)
			DrawGrip(pDC, rect);
		nStart = nStart + nGripInterval;
	}
	return true;
}

bool CGripDraw::DrawSignalGrip(CDC* pDC, const int nIndex, const CSelectSubGraphData& selectSubGraphData)
{
  	const CList< CPacket *, CPacket *> &packetList = selectSubGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return false;

	CPacket* pDataPacket = NULL;
	CPacket* pPointPacket = NULL;
	int nUnit = 2;

	double dRealWidth = 0.0;
	POSITION Pos = packetList.GetHeadPosition();
	if(Pos == NULL)	return false;
	pDataPacket = packetList.GetNext(Pos);
	if(Pos == NULL)	return false;
	pPointPacket = packetList.GetNext(Pos);
	POSITION dataPos = NULL;
	POSITION pointPos = NULL;
	if(pDataPacket->GetnumericDataList() && pPointPacket->GetnumericDataList()){
		dataPos = pDataPacket->GetnumericDataList()->FindIndex(nIndex);
		pointPos = pPointPacket->GetnumericDataList()->FindIndex(nIndex);
	}
	else
		return false;
	
	if(dataPos == NULL || pointPos == NULL)
		return false;
	int nStart = selectSubGraphData.GetDisplayAttributes().GetDataStartInBlock();
	int nEnd = selectSubGraphData.GetDisplayAttributes().GetDataEndInBlock();
	if(nIndex < nStart || nIndex > nEnd)
		return false;
	int nViewCount = selectSubGraphData.GetDisplayAttributes().GetDataCountInBlock();

	double dViewMin = CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), selectSubGraphData.GetDisplayDataVertMinMax().GetDisplayMin());
	double dViewMax = CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), selectSubGraphData.GetDisplayDataVertMinMax().GetDisplayMax());

	dRealWidth = CDraw::CalRealWidth(selectSubGraphData.GetGraphRegions().GetDrawingRegion(), nStart, nEnd);
	
	double dData = pDataPacket->GetnumericDataList()->GetNext(dataPos);
	if(dData < 1.0)
		return false;
	double dPointData = CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), pPointPacket->GetnumericDataList()->GetNext(pointPos));

	CPoint dataPt = CDataConversion::GetVerticalPoint(selectSubGraphData.GetGraphRegions().GetDrawingRegion(), nViewCount, dRealWidth, dViewMin, dViewMax, dPointData);
	
	if(selectSubGraphData.GetVertScaleInvertType())
		dataPt.y = CDataConversion::InvertGraphDataPt(selectSubGraphData.GetGraphRegions().GetDrawingRegion(), dataPt.y);

	int nWidth = CMath::Round(dRealWidth/* 2/3*/);
	if(nWidth > 15)
		nWidth = 15;
	else if(nWidth < 10)
		nWidth = 10;

	CRect graphRegion;
	if(selectSubGraphData.GetSubGraphStyle().GetGraphDataViewStyle() == CGraphBaseData::TopSignal){
		if(dData > 1)
			graphRegion = CRect(dataPt.x - nWidth, dataPt.y - 4 - nWidth*2, dataPt.x + nWidth, dataPt.y - 4);
		else if(dData == 1)
			graphRegion = CRect(dataPt.x - nWidth, dataPt.y - 4 - nWidth, dataPt.x + nWidth, dataPt.y - 4);
		else
			return false;
	}
	else{
		if(dData > 1)
			graphRegion = CRect(dataPt.x - nWidth, dataPt.y + 4, dataPt.x + nWidth, dataPt.y + 4 + nWidth*2);
		else if(dData == 1)
			graphRegion = CRect(dataPt.x - nWidth, dataPt.y + 4, dataPt.x + nWidth, dataPt.y + 4 + nWidth);
		else
			return false;
	}
	CRect rect;
	rect = CRect(graphRegion.left - nUnit, graphRegion.top - nUnit, graphRegion.left + nUnit, graphRegion.top + nUnit);
	DrawGrip(pDC, rect);
	rect = CRect(graphRegion.left - nUnit, graphRegion.bottom - nUnit, graphRegion.left + nUnit, graphRegion.bottom + nUnit);
	DrawGrip(pDC, rect);
	rect = CRect(graphRegion.right - nUnit, graphRegion.top - nUnit, graphRegion.right + nUnit, graphRegion.top + nUnit);
	DrawGrip(pDC, rect);
	rect = CRect(graphRegion.right - nUnit, graphRegion.bottom - nUnit, graphRegion.right + nUnit, graphRegion.bottom + nUnit);
	DrawGrip(pDC, rect);

	return true;
}

bool CGripDraw::DrawGripAll(CDC* pDC, const CSelectSubGraphData& selectSubGraphData)
{
	const CList< CPacket *, CPacket *> &packetList = selectSubGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return false;

	CPacket* pDataPacket1 = NULL;
	CPacket* pDataPacket2 = NULL;
	int unit = 3;
	double realWidth = 0.0;
	
	//sy 2002.9.18.
	//POSITION Pos = selectSubGraphData.GetPacketList().GetHeadPosition();
	POSITION Pos = NULL;
	if( g_iMetaTable.IsViewVertMinMax_EndPacket(selectSubGraphData.GetGraphName()))
		Pos = packetList.GetTailPosition();
	else
		Pos = packetList.GetHeadPosition();
	if(Pos == NULL)	
		return false;
	//end

	pDataPacket1 = packetList.GetNext(Pos);
	
	//구름식은 라인이 두개기 때문에.
	
///	if(selectSubGraphData.GetSubGraphType() == CGraphBaseData::Line_Type && selectSubGraphData.GetSubGraphStyle().GetGraphLineStyle() == CGraphBaseData::CloudLine){
	if(selectSubGraphData.GetSubGraphType() == CGraphBaseData::Line_Type 
		&& (selectSubGraphData.GetSubGraphStyle().GetGraphLineStyle() == CGraphBaseData::CloudLine 
		|| selectSubGraphData.GetSubGraphStyle().GetGraphLineStyle() == CGraphBaseData::LineCloudLine
		|| selectSubGraphData.GetSubGraphStyle().GetGraphLineStyle() == CGraphBaseData::NonCloudLine)){
		if(Pos == NULL)
			return false;
		pDataPacket2 = packetList.GetNext(Pos);
	}

	int dataStart, dataEnd;
	selectSubGraphData.GetXScaleManager()->GetDrawIndex(FALSE, FALSE, dataStart, dataEnd);

	if(dataStart < 0)
		return false;

	realWidth = CDraw::CalRealWidth(selectSubGraphData.GetGraphRegions().GetDrawingRegion(), dataStart, dataEnd);

	int nGripInterval = CalGripInterval(selectSubGraphData.GetDisplayAttributes());
	int nGripNumber = 11;
	if(nGripInterval > 0)
		nGripNumber = (dataEnd - dataStart +1)/nGripInterval;
	nGripNumber++;
	if(nGripInterval == 1)
		nGripNumber = selectSubGraphData.GetDisplayAttributes().GetDataCountInBlock();

	double dViewMin = CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), selectSubGraphData.GetDisplayDataVertMinMax().GetDisplayMin());
	double dViewMax = CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), selectSubGraphData.GetDisplayDataVertMinMax().GetDisplayMax());
	
	POSITION packet1Pos = pDataPacket1->GetnumericDataList()->FindIndex(dataStart);
	CXScaleManager::AREA area;
	for(int i = 0; i < nGripNumber; i++){

		POSITION packet1Pos = pDataPacket1->GetnumericDataList()->FindIndex(dataStart);
		if(packet1Pos == NULL || dataStart > dataEnd)
			break;
		CPoint dataPt = CDataConversion::GetVerticalPoint(selectSubGraphData.GetGraphRegions().GetDrawingRegion(), nGripInterval*i, realWidth, dViewMin, dViewMax, CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), pDataPacket1->GetnumericDataList()->GetNext(packet1Pos)));

		if(selectSubGraphData.GetVertScaleInvertType())
			dataPt.y = CDataConversion::InvertGraphDataPt(selectSubGraphData.GetGraphRegions().GetDrawingRegion(), dataPt.y);

		selectSubGraphData.GetXScaleManager()->GetAREAfRQnDTI(dataStart, 0, &area);
		dataPt.x = area.center;

		CRect rect = CRect(dataPt.x - unit, dataPt.y - unit, dataPt.x + unit +1, dataPt.y + unit);
		if(i > 0 && pDataPacket1->GetStartPeriod() < dataStart)
			DrawGrip(pDC, rect);
		if(selectSubGraphData.GetSubGraphType() == CGraphBaseData::Line_Type 
			&& (selectSubGraphData.GetSubGraphStyle().GetGraphLineStyle() == CGraphBaseData::CloudLine
			|| selectSubGraphData.GetSubGraphStyle().GetGraphLineStyle() == CGraphBaseData::LineCloudLine
			|| selectSubGraphData.GetSubGraphStyle().GetGraphLineStyle() == CGraphBaseData::NonCloudLine)){
			POSITION packet2Pos = pDataPacket2->GetnumericDataList()->FindIndex(dataStart);
			if(packet2Pos == NULL)
				break;
			CPoint dataPt = CDataConversion::GetVerticalPoint(selectSubGraphData.GetGraphRegions().GetDrawingRegion(), nGripInterval*i, realWidth, dViewMin, dViewMax, CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), pDataPacket2->GetnumericDataList()->GetNext(packet2Pos)));
			if(selectSubGraphData.GetVertScaleInvertType())
				dataPt.y = CDataConversion::InvertGraphDataPt(selectSubGraphData.GetGraphRegions().GetDrawingRegion(), dataPt.y);

			selectSubGraphData.GetXScaleManager()->GetAREAfRQnDTI(dataStart, 0, &area);
			dataPt.x = area.center;

			CRect rect = CRect(dataPt.x - unit, dataPt.y - unit, dataPt.x + unit +1, dataPt.y + unit);
			if(i > 0 && pDataPacket2->GetStartPeriod() < dataStart)
				DrawGrip(pDC, rect);
		}
		dataStart = dataStart + nGripInterval;
	}
	return true;
}

//sy 2002.9.24.
bool CGripDraw::DrawGripBar(CDC* pDC, const CSelectSubGraphData& selectSubGraphData)
{
	if(selectSubGraphData.GetSubGraphType() != CGraphBaseData::Bar_Type)
		return false;

	const CList< CPacket *, CPacket *> &packetList = selectSubGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return false;

	CPacket* pDataPacket1 = NULL;
	CPacket* pDataPacket2 = NULL;
	int unit = 3;
	double realWidth;
	
	POSITION Pos = packetList.GetHeadPosition();
	if(Pos == NULL)	
		return false;

	pDataPacket1 = packetList.GetNext(Pos);
	
	CGraphBaseData::BARSTYLE eGraphStyle = selectSubGraphData.GetSubGraphStyle().GetGraphBarStyle();
	//구름식은 라인이 두개기 때문에.
	if(eGraphStyle == CGraphBaseData::CombBar){
		if(Pos == NULL)
			return false;
		pDataPacket2 = packetList.GetNext(Pos);
	}

	//sy test
	int dataStart = 0, dataEnd = 0;
	CRect drawingRegion(0, 0, 0, 0);
	if(!GetGripDrawData_BarType(pDataPacket1, selectSubGraphData, dataStart, dataEnd, drawingRegion))
		return false;

	selectSubGraphData.GetXScaleManager()->GetDrawIndex(FALSE, FALSE, dataStart, dataEnd);

	realWidth = CDraw::CalRealWidth(drawingRegion, dataStart, dataEnd);
	if(eGraphStyle == CGraphBaseData::HorizontalBar || eGraphStyle == CGraphBaseData::TextHorizontalBar)
		realWidth = (double)drawingRegion.Height()/(double)(dataEnd - dataStart + 1);

	int nGripInterval = CalGripInterval(selectSubGraphData.GetDisplayAttributes());
	if(eGraphStyle == CGraphBaseData::HorizontalBar || eGraphStyle == CGraphBaseData::TextHorizontalBar)
		nGripInterval = CalHGripNumber(selectSubGraphData.GetDisplayAttributes(), pDataPacket1->GetCount());

	
	int nGripNumber = 11;
	if(nGripInterval > 0)
		nGripNumber = (dataEnd - dataStart +1)/nGripInterval;
	else
		nGripNumber = 1;

	if(nGripInterval == 1 && eGraphStyle != CGraphBaseData::HorizontalBar && eGraphStyle != CGraphBaseData::TextHorizontalBar)
		nGripNumber = selectSubGraphData.GetDisplayAttributes().GetDataCountInBlock();

	double dViewMin = CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), selectSubGraphData.GetDisplayDataVertMinMax().GetDisplayMin());
	double dViewMax = CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), selectSubGraphData.GetDisplayDataVertMinMax().GetDisplayMax());
	if(eGraphStyle == CGraphBaseData::HorizontalBar || eGraphStyle == CGraphBaseData::TextHorizontalBar){
		double tempDataMin = 0.0, tempDataMax = 0.0; 
		pDataPacket1->GetMinMax(0, dataEnd, tempDataMin, tempDataMax);	
		dViewMin = 0;/*CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), tempDataMin);*/
		dViewMax = CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), tempDataMax);
	}
	if(eGraphStyle == CGraphBaseData::CombBar){
		//min 과 max를 새로 구한다.=====================================================
		double datamax = 0.0, datamin = 0.0, tempmax = 0.0, tempmin = 0.0;
		pDataPacket1->GetMinMax(dataStart, dataEnd, datamin, datamax); 
		datamin = fabs(datamin);
		datamax = fabs(datamax);
		if(datamin > datamax)
			datamax = datamin;

		pDataPacket2->GetMinMax(dataStart, dataEnd, tempmax, tempmin); 
		tempmin = fabs(tempmin);
		tempmax = fabs(tempmax);
		if(tempmin > tempmax)
			tempmax = tempmin;
		if(tempmax > datamax)
			datamax = tempmax;
		datamin = datamax * -1;
		//==============================================================================
		dViewMin = CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), datamin);
		dViewMax = CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), datamax);
	}

	
	CXScaleManager::AREA area;
	for(int i = 0; i < nGripNumber; i++){
		POSITION packet1Pos = pDataPacket1->GetnumericDataList()->FindIndex(dataStart);
		if(packet1Pos == NULL || dataStart > dataEnd)
			break;
		CPoint dataPt; 
		if(eGraphStyle == CGraphBaseData::HorizontalBar || eGraphStyle == CGraphBaseData::TextHorizontalBar)
			dataPt = CDataConversion::GetHorizontalPoint(drawingRegion, nGripInterval*i, realWidth, dViewMin, dViewMax, CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), pDataPacket1->GetnumericDataList()->GetNext(packet1Pos)));
		else if(eGraphStyle == CGraphBaseData::CombBar){
			double tempData = pDataPacket1->GetnumericDataList()->GetNext(packet1Pos);
			if(tempData < 0)
				tempData = tempData * -1;
			tempData = CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), tempData);
			dataPt = CDataConversion::GetVerticalPoint(drawingRegion, nGripInterval*i, realWidth, dViewMin, dViewMax, tempData);
		}
		else
			dataPt = CDataConversion::GetVerticalPoint(drawingRegion, nGripInterval*i, realWidth, dViewMin, dViewMax, CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), pDataPacket1->GetnumericDataList()->GetNext(packet1Pos)));

		if(selectSubGraphData.GetVertScaleInvertType())
			dataPt.y = CDataConversion::InvertGraphDataPt(drawingRegion, dataPt.y);

		selectSubGraphData.GetXScaleManager()->GetAREAfRQnDTI(dataStart, 0, &area);
		dataPt.x = area.center;

		CRect rect = CRect(dataPt.x - unit, dataPt.y - unit, dataPt.x + unit +1, dataPt.y + unit);
		if(i > 0 && pDataPacket1->GetStartPeriod() < dataStart)
			DrawGrip(pDC, rect);
		else if(pDataPacket1->GetStartPeriod() == dataStart){
			if(eGraphStyle == CGraphBaseData::HorizontalBar || eGraphStyle == CGraphBaseData::TextHorizontalBar)
				DrawGrip(pDC, rect);
		}

		dataStart = dataStart + nGripInterval;
	}
	return true;
}

//sy 2004.12.16.
bool CGripDraw::GetGripDrawData_BarType(CPacket* pDataPacket, const CSelectSubGraphData& selectSubGraphData, 
		int& nDataStartIndex, int& nDataEndIndex, CRect& drawingRegion) const
{
	if(pDataPacket == NULL || pDataPacket->GetCount() <= 0)
		return false;

	nDataStartIndex = selectSubGraphData.GetDisplayAttributes().GetDataStartInBlock();
	nDataEndIndex = selectSubGraphData.GetDisplayAttributes().GetDataEndInBlock();
	drawingRegion = selectSubGraphData.GetGraphRegions().GetDrawingRegion();

	if(selectSubGraphData.GetGraphName() != _MTEXT( C2_OVERHANGING_SUPPLY))
		return true;

	// "대기매물" 일 경우
	//-> 종가의 min/max영역이 실제 그리는 영역이다.
	//-> dataIndex 최고가, 현재가를 표현하는걸 제외한게 data count 이다.
	drawingRegion = CBarDraw().GetDrawingRegion(pDataPacket, drawingRegion, 
		_MTEXT( C2_OVERHANGING_SUPPLY), nDataStartIndex, nDataEndIndex, 
		selectSubGraphData.GetDisplayDataVertMinMax().GetDisplayMin(), 
		selectSubGraphData.GetDisplayDataVertMinMax().GetDisplayMax());
	
	if(selectSubGraphData.GetSubGraphStyle().GetGraphBarStyle() == CGraphBaseData::HorizontalBar || 
		selectSubGraphData.GetSubGraphStyle().GetGraphBarStyle() == CGraphBaseData::TextHorizontalBar)
	{
		nDataStartIndex = 0;
		nDataEndIndex = pDataPacket->GetCount() -1;
		// "대기매물" 일 경우
		if(pDataPacket->GetName() == _MTEXT( C2_OVERHANGING_SUPPLY))
			nDataEndIndex -= 2;
	}
	
	return true;
}
//sy end

bool CGripDraw::DrawGripBoxBar(CDC* pDC, const CSelectSubGraphData& selectSubGraphData)
{
	const CList< CPacket *, CPacket *> &packetList = selectSubGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() < 6)
		return false;

	POSITION pos = packetList.GetHeadPosition();
	if(pos == NULL)	return false;
	
	int unit = 3;
	double dRealWidth = 0.0;

	CPacket* pHighPacket = NULL;	
	CPacket* pLowPacket = NULL;	
	CPacket* pOpenPacket = NULL;	
	CPacket* pClosePacket = NULL;

	int nCondition = 0;
	POSITION p = packetList.GetHeadPosition();
	if(p == NULL)	return false;
	CPacket* pPacket = packetList.GetNext(p);
	if(pPacket == NULL)	return false;

	CBarDraw barDraw;
	bool bForward = barDraw.CheckBoxBarType(pPacket);
	/// Forward
	if(bForward){
		pHighPacket = packetList.GetAt(packetList.FindIndex(0));	
		pLowPacket = packetList.GetAt(packetList.FindIndex(1));

		nCondition = GetBoxCondition(pHighPacket, true);
	}
	/// Backward
	else{
		pHighPacket = packetList.GetAt(packetList.FindIndex(2));	
		pLowPacket = packetList.GetAt(packetList.FindIndex(3));

		nCondition = GetBoxCondition(pHighPacket, false);
	}
	pOpenPacket = packetList.GetAt(packetList.FindIndex(4));
	pClosePacket = packetList.GetAt(packetList.FindIndex(5));	

	if(nCondition <= 0 || pOpenPacket == NULL || pHighPacket == NULL || pLowPacket == NULL || pClosePacket == NULL)
		return false;

	int nStart = selectSubGraphData.GetDisplayAttributes().GetDataStartInBlock();
	int nEnd = selectSubGraphData.GetDisplayAttributes().GetDataEndInBlock();

	if(nStart < 0)
		return false;

	dRealWidth = CDraw::CalRealWidth(selectSubGraphData.GetGraphRegions().GetDrawingRegion(), nStart, nEnd);
	int nGripInterval = CalBoxGripInterval(selectSubGraphData.GetDisplayAttributes(), nCondition);

	int nGripNumber = 11;
	if(nGripInterval > 0)
		nGripNumber = (nEnd - nStart +1)/nGripInterval;
	nGripNumber++;
	if(nGripInterval == 1)
		nGripNumber = selectSubGraphData.GetDisplayAttributes().GetDataCountInBlock();

	double dViewMin = CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), selectSubGraphData.GetDisplayDataVertMinMax().GetDisplayMin());
	double dViewMax = CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), selectSubGraphData.GetDisplayDataVertMinMax().GetDisplayMax());

	int nIndex = nStart;
	if(bForward)
	{
		int nDev = (nIndex) / nCondition;
		nIndex -= nDev * nCondition;
		int nScreenRest = nCondition - nIndex;
		nIndex = 0;
		nIndex += nScreenRest;		
		nIndex += nCondition / 2;
	}
	else
	{
		int nRest = pOpenPacket->GetnumericDataList()->GetCount() % nCondition;
		if(nStart < nRest)
			nIndex += nRest - nStart + nCondition / 2;
		else
		{
			nIndex -= nRest;
			int nDev = (nIndex) / nCondition;
			nIndex -= nDev * nCondition;
			int nScreenRest = nCondition - nIndex;
			nIndex = 0;
			nIndex += nScreenRest;		
			nIndex += nCondition / 2;
		}	
	}
	

	CRect& rect = selectSubGraphData.GetGraphRegions().GetDrawingRegion();
	for(int i = 0; i < nGripNumber; i++){
		POSITION highPos = pHighPacket->GetnumericDataList()->FindIndex(nStart + nIndex);
		POSITION lowPos = pLowPacket->GetnumericDataList()->FindIndex(nStart + nIndex);
		if(highPos == NULL || lowPos == NULL || nStart > nEnd)
			break;
		
		double dHigh = CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), pHighPacket->GetnumericDataList()->GetNext(highPos));
		double dLow = CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), pLowPacket->GetnumericDataList()->GetNext(lowPos));
		double dGrip = dLow + (dHigh - dLow)/2.0;

		CPoint gripPt(0,0);
		if(nCondition % 2)
			gripPt = CDataConversion::GetVerticalPoint(selectSubGraphData.GetGraphRegions().GetDrawingRegion(), nIndex, dRealWidth, dViewMin, dViewMax, dGrip, true);
		else
			gripPt = CDataConversion::GetVerticalPoint(selectSubGraphData.GetGraphRegions().GetDrawingRegion(), nIndex, dRealWidth, dViewMin, dViewMax, dGrip, false);
		
		if(selectSubGraphData.GetVertScaleInvertType())
			gripPt.y = CDataConversion::InvertGraphDataPt(selectSubGraphData.GetGraphRegions().GetDrawingRegion(), gripPt.y);

		CRect rect = CRect(gripPt.x - unit, gripPt.y - unit, gripPt.x + unit +1, gripPt.y + unit);
///		if(i > 0)
			DrawGrip(pDC, rect);
		nIndex += nGripInterval;
	}
	return true;
}

/// 보류
bool CGripDraw::DrawGripLeftRightBar(CDC* pDC, const CSelectSubGraphData& selectSubGraphData)
{
	return true;
}

/// 보류
bool CGripDraw::DrawGripGroupBar(CDC* pDC, const CSelectSubGraphData& selectSubGraphData)
{
	return true;
}
		
bool CGripDraw::DrawGripBong(CDC* pDC, const CSelectSubGraphData& selectSubGraphData)
{
	const CList< CPacket *, CPacket *> &packetList = selectSubGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() < 4)
		return false;

	int unit = 3;
	double dRealWidth = 0.0;

	POSITION headPos = packetList.GetHeadPosition();
	if(headPos == NULL)	return false;

	CPacket* pDataPacket1 = packetList.GetNext(headPos);
	if(headPos == NULL || pDataPacket1 == NULL)	return false;
	CPacket* pDataPacket2 = packetList.GetNext(headPos);
	if(headPos == NULL || pDataPacket2 == NULL)	return false;
	CPacket* pDataPacket3 = packetList.GetNext(headPos);
	if(headPos == NULL || pDataPacket3 == NULL)	return false;
	CPacket* pDataPacket4 = packetList.GetNext(headPos);	
	if(pDataPacket4 == NULL)	return false;

	int nStart, nEnd;
	selectSubGraphData.GetXScaleManager()->GetDrawIndex(FALSE, FALSE, nStart, nEnd);

	if(nStart < 0)
		return false;

	dRealWidth = CDraw::CalRealWidth(selectSubGraphData.GetGraphRegions().GetDrawingRegion(), nStart, nEnd);

	int nGripInterval = CalGripInterval(selectSubGraphData.GetDisplayAttributes());

	int nGripNumber = 11;
	if(nGripInterval > 0)
		nGripNumber = (nEnd - nStart +1)/nGripInterval;
	nGripNumber++;
	if(nGripInterval == 1)
		nGripNumber = selectSubGraphData.GetDisplayAttributes().GetDataCountInBlock();

	double dViewMin = CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), selectSubGraphData.GetDisplayDataVertMinMax().GetDisplayMin());
	double dViewMax = CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), selectSubGraphData.GetDisplayDataVertMinMax().GetDisplayMax());

	//CString strRQ;
	//selectSubGraphData.GetXScaleManager()->SetCurDrawInfo((CString&)strRQ, nStart);
	CXScaleManager::AREA area;
	for(int i = 0; i < nGripNumber; i++)
	{
		POSITION packet2Pos = pDataPacket2->GetnumericDataList()->FindIndex(nStart);
		POSITION packet3Pos = pDataPacket3->GetnumericDataList()->FindIndex(nStart);
		if(packet2Pos == NULL || packet3Pos == NULL || nStart > nEnd)
			break;
		
		double dData2 = CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), pDataPacket2->GetnumericDataList()->GetNext(packet2Pos));
		double dData3 = CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), pDataPacket3->GetnumericDataList()->GetNext(packet3Pos));
		double dData = dData3 + (dData2 - dData3)/2.0;
		CPoint dataPt = CDataConversion::GetVerticalPoint(selectSubGraphData.GetGraphRegions().GetDrawingRegion(), nGripInterval*i, dRealWidth, dViewMin, dViewMax, dData);
		if(selectSubGraphData.GetVertScaleInvertType())
				dataPt.y = CDataConversion::InvertGraphDataPt(selectSubGraphData.GetGraphRegions().GetDrawingRegion(), dataPt.y);

		selectSubGraphData.GetXScaleManager()->GetAREAfRQnDTI(nStart, 0, &area);
		dataPt.x = area.center;

		CRect rect = CRect(dataPt.x - unit, dataPt.y - unit, dataPt.x + unit +1, dataPt.y + unit);
		if(i > 0)
			DrawGrip(pDC, rect);
		nStart = nStart + nGripInterval;
	}
	return true;
}

bool CGripDraw::DrawThreeLineBreakGrip(CDC* pDC, const CSelectSubGraphData& selectSubGraphData)
{
	const CList< CPacket *, CPacket *> &packetList = selectSubGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return false;

	int unit = 3;
	POSITION Pos = packetList.GetHeadPosition();
	if(!Pos)
		return false;
	CPacket* pOpenPacket = packetList.GetNext(Pos); 
	if(!Pos)
		return false;
	CPacket* pClosePacket = packetList.GetNext(Pos); 

	POSITION openPos = pOpenPacket->GetnumericDataList()->GetHeadPosition();
	POSITION closePos = pClosePacket->GetnumericDataList()->GetHeadPosition();

	if(openPos == NULL || closePos == NULL) 
		return false;

	int nStart = 0;
	int nEnd = pOpenPacket->GetCount() -1;
	int pageDataCount = nEnd - nStart + 1;

	if(nStart < 0)
		return false;

	double realWidth = (double)selectSubGraphData.GetGraphRegions().GetDrawingRegion().Width()/(double)(nEnd - nStart + 1);

	int nGripCount = CalHGripNumber(selectSubGraphData.GetDisplayAttributes(), pOpenPacket->GetCount());
	
	int nGripNumber = 11;
	if(nGripCount > 0)
		nGripNumber = (nEnd - nStart +1)/nGripCount;
	else
		nGripNumber = 1;

	// (2009/11/6 - Seung-Won, Bae) Do not disable zoom and scroll on OneChart
	double dViewMin = selectSubGraphData.GetDisplayDataVertMinMax().GetDisplayMin();
	double dViewMax = selectSubGraphData.GetDisplayDataVertMinMax().GetDisplayMax();
	int dataCount = pOpenPacket->GetCount();
	double tempdataMin = 0.0, tempdataMax = 0.0;
	pOpenPacket->GetMinMax(0, dataCount, tempdataMin, tempdataMax);
//	dViewMin = tempdataMin; 
//	dViewMax = tempdataMax;
	pClosePacket->GetMinMax(0, dataCount, tempdataMin, tempdataMax);
//	if(tempdataMin < dViewMin)
//		dViewMin = tempdataMin;
//	if(tempdataMax > dViewMax)
//		dViewMax = tempdataMax;

	// Graph Drawing 때 오른쪽 여백 적용 - ojtaso (20080124)
	CXScaleManager::AREA area;
	for(int i = 0; i < nGripNumber; i++){
		POSITION openPos = pOpenPacket->GetnumericDataList()->FindIndex(nStart);
		POSITION closePos = pClosePacket->GetnumericDataList()->FindIndex(nStart);
		if(openPos == NULL || closePos == NULL || nStart > nEnd)
			break;

		double dOpen = CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), pOpenPacket->GetnumericDataList()->GetNext(openPos));
		double dClose = CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), pClosePacket->GetnumericDataList()->GetNext(closePos));

		double dGrip = dClose + (dOpen - dClose) / 2.0;

		CPoint gripPt = CDataConversion::GetVerticalPoint(selectSubGraphData.GetGraphRegions().GetDrawingRegion(), nGripCount*i, realWidth, dViewMin, dViewMax, CDraw::CalLogValue(selectSubGraphData.GetVertScaleLogType(), dGrip));

		if(selectSubGraphData.GetVertScaleInvertType())
			gripPt.y = CDataConversion::InvertGraphDataPt(selectSubGraphData.GetGraphRegions().GetDrawingRegion(), gripPt.y);

		// Graph Drawing 때 오른쪽 여백 적용 - ojtaso (20080124)
		selectSubGraphData.GetXScaleManager()->GetAREAfRQnDTI(nStart, 0, &area);
		gripPt.x = area.center;
		
		CRect rect = CRect(gripPt.x - unit, gripPt.y - unit, gripPt.x + unit +1, gripPt.y + unit);
///		if(pOpenPacket->GetStartPeriod() == nStart)
		if(i > 0)
			DrawGrip(pDC, rect);

		nStart = nStart + nGripCount;
	}

	return true;
}

//sy 2004.2.17.
// (2007/5/3 - Seung-Won, Bae) for Inquiry Full Data.
bool CGripDraw::DrawInverseLineGrip(CDC* pDC, const CSelectSubGraphData& selectSubGraphData)
{
	int nDataCount = 0, nStartIndex = 0;
	CList<double, double>* pCloseDataList = NULL, *pVolumeDataList = NULL;
	POSITION closeStartPos = NULL, closeEndPos = NULL, volumeStartPos = NULL, volumeEndPos = NULL;
	if(!CSelectSubGraph().GetPacketData_InverseLineGraph(&selectSubGraphData, nDataCount, 
		pCloseDataList, pVolumeDataList, closeStartPos, closeEndPos, 
		volumeStartPos, volumeEndPos, nStartIndex))
		return false;

	CRect graphRegion = selectSubGraphData.GetGraphRegions().GetDrawingRegion();
	double dVertViewMin = selectSubGraphData.GetDisplayDataVertMinMax().GetDisplayMin();
	double dVertViewMax = selectSubGraphData.GetDisplayDataVertMinMax().GetDisplayMax(); 
	double dHorzViewMin = selectSubGraphData.GetDisplayDataHorzMinMax().GetDisplayMin();
	double dHorzViewMax = selectSubGraphData.GetDisplayDataHorzMinMax().GetDisplayMax();

	int nUnit = 3;
	int nGripCount = nDataCount / 10;
	if(nGripCount < 5)
		nGripCount = 5;
	int nGripNumber = 0;

	bool bIsInvert = selectSubGraphData.GetVertScaleInvertType();
	for(int nIndex = nStartIndex; nIndex < nDataCount; nIndex++)
	{
		if(closeStartPos == NULL || volumeStartPos == NULL)
			break;

		double dCloseData = pCloseDataList->GetNext(closeStartPos);
		double dVolumeData = pVolumeDataList->GetNext(volumeStartPos);

		if(nGripCount != nGripNumber){
			nGripNumber++;
			continue;
		}

		CPoint gripPt = CDataConversion::GetVerticalPoint(graphRegion, 
				dVertViewMin, dVertViewMax, dHorzViewMin, dHorzViewMax, 
				dCloseData, dVolumeData);
		if(bIsInvert)
			gripPt.y = CDataConversion::InvertGraphDataPt(graphRegion, gripPt.y);

		CRect rect = CRect(gripPt.x - nUnit, gripPt.y - nUnit, gripPt.x + nUnit +1, gripPt.y + nUnit);
		DrawGrip(pDC, rect);

		nGripNumber = 0;
	}

	return true;
}
//sy end

/*
//sy 2003.6.11. -> P&F grip 그리기
bool CGripDraw::DrawPAndFGrip(CDC* pDC, const CSelectSubGraphData& selectSubGraphData)
{
	CSubGraphPacketList* pSubGraphPacketList = selectSubGraphData.GetSubGraphPacketList();
	if(pSubGraphPacketList == NULL)
		return false;

	CPAndFDraw PAndFDraw;
	CPacket* pDatePacket = NULL;
	CList<double, double>* pHighList = NULL, *pLowList = NULL;
	if(!PAndFDraw.GetPacketData(pSubGraphPacketList->GetSubGraphPacketList(), 
		pDatePacket, pHighList, pLowList))
		return false;

	double dUnitValue = PAndFDraw.GetUnitValue(pSubGraphPacketList->GetSubGraphPacketList()->GetHead()->GetPacketList(), 
		selectSubGraphData.GetIndicatorInfo());
	if(dUnitValue < 1)
		return false;

	CRect graphRegion = selectSubGraphData.GetGraphRegions().GetDrawingRegion();
	double dViewMin = selectSubGraphData.GetDisplayDataVertMinMax().GetDisplayMin();
	double dViewMax = selectSubGraphData.GetDisplayDataVertMinMax().GetDisplayMax();
	double dUnitHeight = CPAndFDraw().GetUintHeight(graphRegion, dViewMin, dViewMax, dUnitValue);

	int nIndex = 0;
	POSITION highPos = pHighList->GetHeadPosition();
	POSITION lowPos = pLowList->GetHeadPosition();
	while(highPos != NULL && lowPos != NULL){
		double dHigh = pHighList->GetNext(highPos);
		double dLow = pLowList->GetNext(lowPos);

		if(nIndex % 2 == 0){
			CPoint highPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dUnitHeight, dViewMin, dViewMax, dHigh);
			CPoint lowPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dUnitHeight, dViewMin, dViewMax, dLow);

			CRect rect(highPt, lowPt);
			CPoint centerPt = rect.CenterPoint();
			rect.SetRect(centerPt, centerPt);
			rect.InflateRect(MARGIN, MARGIN);
			DrawGrip(pDC, rect);
		}
		nIndex++;
	}
	return true;
}
*/

//private ==========================================
int CGripDraw::CalGripInterval(const CDisplayAttributes& displayAttributes)
{
	//sy 2002.8.20.
	int nGripNumber = 11;
	int pageDataCount = displayAttributes.GetDataCountInBlock();
	if(nGripNumber >= pageDataCount)
		return 1;
	else if(pageDataCount/2 <= nGripNumber)
		return 5;
	
	return pageDataCount / nGripNumber;
}

int CGripDraw::CalBoxGripInterval(const CDisplayAttributes& displayAttributes, const int nCondition)
{
	//sy 2002.8.20.
	int nGripNumber = 11;
	int nBoxCount = displayAttributes.GetDataCountInBlock() / nCondition;
	if(nGripNumber >= nBoxCount)
		return 1 * nCondition;
	else if(nBoxCount/2 <= nGripNumber)
		return 2 * nCondition;
	
	return nBoxCount / nGripNumber * nCondition;
}

int CGripDraw::CalHGripNumber(const CDisplayAttributes& displayAttributes, int totalCount)
{
	int nGripNumber = 10;
	//diaplayData
	//int pageDataCount = (totalCount -1) - displayAttributes.GetDataStartInBlock() + 1;
	int pageDataCount = totalCount - displayAttributes.GetDataStartInBlock() +1;
	if(nGripNumber >= pageDataCount)
		return 1;
	else
		return pageDataCount / nGripNumber;

	return nGripNumber;
}


int CGripDraw::CalDataEnd(const CList<CPacket*, CPacket*>& PacketList, const CDisplayAttributes& displayAttributes)
{
	if(PacketList.GetCount() == 0) 
		return false;

	POSITION pos = PacketList.GetHeadPosition();
	if(pos == NULL)
		return false;
	CPacket* pPacket = PacketList.GetNext(pos);
	if(pPacket == NULL)
		return false;

	//diaplayData
	int tempDataEnd = 0;
	if(pPacket->GetType() == _MTEXT( C6_CHARACTER))
		tempDataEnd = pPacket->GetstrDataList()->GetCount() -1;
	else
		tempDataEnd = pPacket->GetnumericDataList()->GetCount() -1;

	if(PacketList.GetCount() >= 2){
		for(int i = 1; i < PacketList.GetCount(); i++){
			pPacket = PacketList.GetNext(pos);
			if(pPacket == NULL)
				return false;
			int tempcount;
			if(pPacket->GetType() == _MTEXT( C6_CHARACTER))
				tempcount = pPacket->GetstrDataList()->GetCount() -1;
			else
				tempcount = pPacket->GetnumericDataList()->GetCount() -1;

			if(tempDataEnd > tempcount)
				tempDataEnd = tempcount;
		}
	}

	if(tempDataEnd < displayAttributes.GetDataEndInBlock())
		return tempDataEnd;

	return displayAttributes.GetDataEndInBlock();
}

void CGripDraw::DrawGrip(CDC* pDC, CRect rect)
{
	int old = pDC->GetROP2();
	pDC->SetROP2(R2_NOT);
	CDraw::DrawRectangle(pDC, rect);
	pDC->SetROP2(old);
}

/// 박스 조건을 알 수 없으므로(Indicator를 받지 못함) 값을 비교하여 박스 조건을 알아냄.
const int CGripDraw::GetBoxCondition(CPacket *pPacket, bool bForward)
{
	if(pPacket == NULL)	return 0;

	int nCondition = 0;
	if(bForward){
		POSITION p = pPacket->GetnumericDataList()->GetHeadPosition();
		if(p == NULL)	return 0;
		double d1 = 0.0, d2 = 0.0;
		d1 = d2 = pPacket->GetnumericDataList()->GetNext(p);
		if(p == NULL)	return 0;
		while(d1 == d2)
		{
			if(p == NULL)	return 0;
			d1 = pPacket->GetnumericDataList()->GetNext(p);
			nCondition++;
		}
	}
	/// Backward
	else{
		POSITION p = pPacket->GetnumericDataList()->GetTailPosition();
		if(p == NULL)	return 0;
		double d1 = 0.0, d2 = 0.0;
		d1 = d2 = pPacket->GetnumericDataList()->GetPrev(p);
		if(p == NULL)	return 0;
		while(d1 == d2)
		{
			if(p == NULL)	return 0;
			d1 = pPacket->GetnumericDataList()->GetPrev(p);
			nCondition++;
		}
	}
	return nCondition;
}
