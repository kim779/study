// FundDraw.cpp: implementation of the CFundDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FundDraw.h"

#include <math.h>
#include "Conversion.h"

#include "BaseData.h"
#include "PacketPointerList.h"
#include "PieDraw.h"

#include "PacketBase.h"
#include "XScaleManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFundDraw::CFundDraw()
{
	m_eGraphType = CGraphBaseData::Fund_Type;
}

CFundDraw::~CFundDraw()
{
}

bool CFundDraw::DrawGraph(CDC* pDC,
			const CRect& realRegion, 
			const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData,
			const CSubGraphData& subGraphData)
{
	const CGraphColor& graphColor = subGraphData.GetColor();
	const CEtcGraphColor& etcGraphColor = drawingGraphData.GetEtcGraphColor();
	int nRealStart = displayAttributes.GetDataStartInBlock();
	int nRealEnd = displayAttributes.GetDataEndInBlock();
	int nDataEnd = drawingGraphData.GetDisplayAttributes().GetDataEndInBlock();
	double dViewMax = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(); 
	double dViewMin = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin();
	const CList<CPacket*, CPacket*> &packetList = drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	const CPenThickness& penThickness = subGraphData.GetPenThickness();

	switch(subGraphData.GetStyle().GetGraphSpecialStyle()){
	case CGraphBaseData::ComponentRatio_Bar:
		return DrawComponentRatio_Bar(pDC, etcGraphColor, realRegion, graphColor, 
			nRealStart, nDataEnd, nRealEnd, dViewMax, dViewMin, packetList, penThickness);
	case CGraphBaseData::StyleBox:
		return DrawStyleBox(pDC, etcGraphColor, realRegion, graphColor, nRealStart, 
			dViewMax, dViewMin, packetList, penThickness);
	case CGraphBaseData::ReturnRiskGrade:
		return DrawReturnRiskGrade(pDC, etcGraphColor, realRegion, graphColor, 
			nRealStart, dViewMax, dViewMin, packetList, penThickness);
	case CGraphBaseData::ComponentRatio_Line:
		return DrawComponentRatio_Line(pDC, etcGraphColor, realRegion, graphColor, 
			nRealStart, nDataEnd, nRealEnd, dViewMax, dViewMin, packetList, penThickness);
	case CGraphBaseData::ReturnRiskDispersion:
		return DrawReturnRiskDispersion(pDC, etcGraphColor, realRegion, graphColor, 
			nRealStart, dViewMax, dViewMin, 
			drawingGraphData.GetDisplayDataHorzMinMax().GetDisplayMax(), 
			drawingGraphData.GetDisplayDataHorzMinMax().GetDisplayMin(), packetList);
	case CGraphBaseData::FundMarketDispersion:
		return DrawFundMarketDispersion(pDC, etcGraphColor, realRegion, graphColor, 
			nRealStart, dViewMax, dViewMin, packetList, penThickness);
	case CGraphBaseData::ComponentRatio_Pie:
		return DrawComponentRatio_Pie(pDC, realRegion, displayAttributes, drawingGraphData, subGraphData);
	case CGraphBaseData::ComponentRatio_Bar_Line:
		return DrawComponentRatio_Bar_Line(pDC, etcGraphColor, realRegion, graphColor, 
			nRealStart, nDataEnd, nRealEnd, dViewMax, dViewMin, packetList, penThickness);
	case CGraphBaseData::Group_Bar:
		return DrawGroupBar(pDC, etcGraphColor, realRegion, graphColor, nRealStart, 
			nDataEnd, nRealEnd, dViewMax, dViewMin, packetList, penThickness);
	}

	return false;
}

bool CFundDraw::DrawGraph(CDC* pDC,
					   const CRect& realRegion, 
					   const CDisplayAttributes& displayAttributes, 
					   const CDrawingGraphData& drawingGraphData,
					   const CSubGraphData& subGraphData,
					   CXScaleManager* pxScaleManager)
{
	const CGraphColor& graphColor = subGraphData.GetColor();
	const CEtcGraphColor& etcGraphColor = drawingGraphData.GetEtcGraphColor();
	int nRealStart = displayAttributes.GetDataStartInBlock();
	int nRealEnd = displayAttributes.GetDataEndInBlock();
	int nDataEnd = drawingGraphData.GetDisplayAttributes().GetDataEndInBlock();
	double dViewMax = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(); 
	double dViewMin = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin();
	const CList<CPacket*, CPacket*> &packetList = drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	const CPenThickness& penThickness = subGraphData.GetPenThickness();
	
	if(pxScaleManager && pxScaleManager->GetThreadFlag() == THREAD_CALCULATION)
		return true;

	switch(subGraphData.GetStyle().GetGraphSpecialStyle()){
	case CGraphBaseData::ComponentRatio_Bar:
		return DrawComponentRatio_Bar(pDC, etcGraphColor, realRegion, graphColor, 
			nRealStart, nDataEnd, nRealEnd, dViewMax, dViewMin, packetList, penThickness);
	case CGraphBaseData::StyleBox:
		return DrawStyleBox(pDC, etcGraphColor, realRegion, graphColor, nRealStart, 
			dViewMax, dViewMin, packetList, penThickness);
	case CGraphBaseData::ReturnRiskGrade:
		return DrawReturnRiskGrade(pDC, etcGraphColor, realRegion, graphColor, 
			nRealStart, dViewMax, dViewMin, packetList, penThickness);
	case CGraphBaseData::ComponentRatio_Line:
		return DrawComponentRatio_Line(pDC, etcGraphColor, realRegion, graphColor, 
			nRealStart, nDataEnd, nRealEnd, dViewMax, dViewMin, packetList, penThickness);
	case CGraphBaseData::ReturnRiskDispersion:
		return DrawReturnRiskDispersion(pDC, etcGraphColor, realRegion, graphColor, 
			nRealStart, dViewMax, dViewMin, 
			drawingGraphData.GetDisplayDataHorzMinMax().GetDisplayMax(), 
			drawingGraphData.GetDisplayDataHorzMinMax().GetDisplayMin(), packetList);
	case CGraphBaseData::FundMarketDispersion:
		return DrawFundMarketDispersion(pDC, etcGraphColor, realRegion, graphColor, 
			nRealStart, dViewMax, dViewMin, packetList, penThickness);
	case CGraphBaseData::ComponentRatio_Pie:
		return DrawComponentRatio_Pie(pDC, realRegion, displayAttributes, drawingGraphData, subGraphData);
	case CGraphBaseData::ComponentRatio_Bar_Line:
		return DrawComponentRatio_Bar_Line(pDC, etcGraphColor, realRegion, graphColor, 
			nRealStart, nDataEnd, nRealEnd, dViewMax, dViewMin, packetList, penThickness);
	case CGraphBaseData::Group_Bar:
		return DrawGroupBar(pDC, etcGraphColor, realRegion, graphColor, nRealStart, 
			nDataEnd, nRealEnd, dViewMax, dViewMin, packetList, penThickness);
	}
	
	return false;

}

bool CFundDraw::DrawComponentRatio_Bar(CDC* pDC, const CEtcGraphColor& etcGraphColor, 
		const CRect& graphDrawingRegion, const CGraphColor& graphColor, 
		const int dataStart, const int dataEnd, const int dataRealEnd, 
		const double& dataViewMax, const double& dataViewMin, 
		const CList<CPacket*, CPacket*>& packetList, CPenThickness penThickness)
{
	int count = 0;
	POSITION packetHeadPos = packetList.GetHeadPosition();
	if(packetHeadPos == NULL) 
		return false;

	int packetCount = packetList.GetCount();
	int i = 0;
	
	CList<POSITION, POSITION> packetPosition;
	for(i = 0; i < packetCount; i++){
		if(packetHeadPos == NULL)
			return false;

		CPacket* pPacket = packetList.GetNext(packetHeadPos);
		if(pPacket == NULL)
			return false;
		else{
			//displayData
			//POSITION pos = pPacket->GetnumericDataList()->FindIndex(dataStart-1);
			POSITION pos = pPacket->GetnumericDataList()->FindIndex(dataStart);
			if(pos == NULL)
				return false;
			else
				packetPosition.AddTail(pos);
		}
	}
	double widthRatio = 8.0/10.0;
	double realWidth = CDraw::CalRealWidth(graphDrawingRegion, dataStart, dataEnd);
	int bongWidth = (int)(realWidth*widthRatio);
	if(bongWidth <= 1) 
		bongWidth = 1;
	int lineThickness = CDraw::CalUpLineThickness(penThickness, bongWidth);

	bongWidth = bongWidth - lineThickness;
	bongWidth = bongWidth / 2;

	CPen pen, pen1, pen2, pen3, pen4, pen5, pen6, pen7, pen8, pen9, pen10;
	pen.CreatePen(PS_SOLID, 1, etcGraphColor.GetGraphColor5());

	pen1.CreatePen	(PS_SOLID, 1, graphColor.GetColor1());
	pen2.CreatePen	(PS_SOLID, 1, graphColor.GetColor2());
	pen3.CreatePen	(PS_SOLID, 1, graphColor.GetColor3());
	pen4.CreatePen	(PS_SOLID, 1, graphColor.GetColor4());
	pen5.CreatePen	(PS_SOLID, 1, graphColor.GetColor5());
	pen6.CreatePen	(PS_SOLID, 1, graphColor.GetColor6());
	pen7.CreatePen	(PS_SOLID, 1, etcGraphColor.GetGraphColor1());
	pen8.CreatePen	(PS_SOLID, 1, etcGraphColor.GetGraphColor2());
	pen9.CreatePen	(PS_SOLID, 1, etcGraphColor.GetGraphColor3());
	pen10.CreatePen	(PS_SOLID, 1, etcGraphColor.GetGraphColor4());

	int brushCount = 10;
	CBrush firstBrush	(graphColor.GetColor1());
	CBrush secondBrush	(graphColor.GetColor2());
	CBrush thirdBrush	(graphColor.GetColor3());
	CBrush fourBrush	(graphColor.GetColor4());
	CBrush fiveBrush	(graphColor.GetColor5());
	CBrush sixBrush		(graphColor.GetColor6());
	CBrush sevenBrush	(etcGraphColor.GetGraphColor1());
	CBrush eightBrush	(etcGraphColor.GetGraphColor2());
	CBrush nineBrush	(etcGraphColor.GetGraphColor3());
	CBrush tenBrush		(etcGraphColor.GetGraphColor4());

	CBrush* pOldBrush;
	CPen* pOldPen; 
	pOldBrush = (CBrush*)pDC->SelectObject(&firstBrush);
	pOldPen = (CPen*)pDC->SelectObject(&pen);

	for(i = dataStart; i <= dataRealEnd; i++){
		count++;
		packetHeadPos = packetList.GetHeadPosition();
		int saveDataY = 0;
		int beforeDataY;
		CPoint DataPt;
		
		for(int j = 0; j < packetCount; j++){
			CPacket* dataPacket = packetList.GetNext(packetHeadPos);
			POSITION dataPosPosition = packetPosition.FindIndex(j);
			POSITION tempPos = dataPosPosition;
			POSITION dataPos = packetPosition.GetNext(dataPosPosition);
			if(packetHeadPos == NULL || dataPosPosition == NULL || dataPos == NULL)
				break;
			CString strData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::DoubleToString(dataPacket->GetnumericDataList()->GetNext(dataPos)), dataPacket->GetType());
			strData.TrimRight("%");
			double data = atof(strData);
			packetPosition.SetAt(tempPos, dataPos);
			DataPt = CDataConversion::GetVerticalPoint(graphDrawingRegion, count, realWidth, dataViewMin, dataViewMax, data);
			if(data == 0)
				DataPt.y = graphDrawingRegion.bottom;
			if(data != 0 && graphDrawingRegion.bottom == DataPt.y)
				DataPt.y --;

			saveDataY = saveDataY + (graphDrawingRegion.bottom - DataPt.y);
			CRect rect;
			if(j == 0)
				rect = CRect(DataPt.x - bongWidth - lineThickness/2, DataPt.y, DataPt.x + bongWidth + lineThickness, graphDrawingRegion.bottom);
			else
				rect = CRect(DataPt.x - bongWidth - lineThickness/2, graphDrawingRegion.bottom - saveDataY, DataPt.x + bongWidth + lineThickness, graphDrawingRegion.bottom - beforeDataY);
			
			beforeDataY	= saveDataY;
			
			int selectBrush = (j+1) - (brushCount * ((j+1)/ brushCount));
			if(selectBrush == 1){
				pDC->SelectObject(&pen1);
				pDC->FillRect(rect, &firstBrush);
			}
			else if(selectBrush == 2){				
				pDC->SelectObject(&pen2);
				pDC->FillRect(rect, &secondBrush);
			}
			else if(selectBrush == 3){
				pDC->SelectObject(&pen3);
				pDC->FillRect(rect, &thirdBrush);
			}
			else if(selectBrush == 4){
				pDC->SelectObject(&pen4);
				pDC->FillRect(rect, &fourBrush);
			}
			else if(selectBrush == 5){
				pDC->SelectObject(&pen5);
				pDC->FillRect(rect, &fiveBrush);
			}
			else if(selectBrush == 6){
				pDC->SelectObject(&pen6);
				pDC->FillRect(rect, &sixBrush);
			}
			else if(selectBrush == 7){
				pDC->SelectObject(&pen7);
				pDC->FillRect(rect, &sevenBrush);
			}
			else if(selectBrush == 8){
				pDC->SelectObject(&pen8);
				pDC->FillRect(rect, &eightBrush);
			}
			else if(selectBrush == 9){
				pDC->SelectObject(&pen9);
				pDC->FillRect(rect, &nineBrush);
			}
			else if(selectBrush == 10){
				pDC->SelectObject(&pen10);
				pDC->FillRect(rect, &tenBrush);
			}
			else
				pDC->FillRect(rect, &firstBrush);
			
			if(rect.Height() <= 1 && rect.Height() > 0)
				CDraw::DrawLine(pDC, CPoint(rect.left, rect.top), CPoint(rect.right, rect.top));

		}

		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->SelectObject(&pen);
		//CDraw::DrawRectangle(pDC, CRect(DataPt.x - bongWidth - lineThickness/2, graphDrawingRegion.top, DataPt.x + bongWidth + lineThickness, graphDrawingRegion.bottom));
	}
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	firstBrush.DeleteObject();
	secondBrush.DeleteObject();
	thirdBrush.DeleteObject();
	fourBrush.DeleteObject();
	fiveBrush.DeleteObject();
	sixBrush.DeleteObject();
	sevenBrush.DeleteObject();
	eightBrush.DeleteObject();
	nineBrush.DeleteObject();
	tenBrush.DeleteObject();

	pen.DeleteObject();
	pen1.DeleteObject();
	pen2.DeleteObject();
	pen3.DeleteObject();
	pen4.DeleteObject();
	pen5.DeleteObject();
	pen6.DeleteObject();
	pen7.DeleteObject();
	pen8.DeleteObject();
	pen9.DeleteObject();
	pen10.DeleteObject();
	
	return true;
}

// 스타일 박스는 packet이 5개로 구성되어 있다.
// 0. 일자
// 1. x축 위치
// 2. y축 위치
// 3. x축 값
// 4. y축 값
// 그리는 데 필요한것은 위의 1, 2번 packet만 필요하다.
// 주위점 x, y 좌 표는 총 16개 이다.
bool CFundDraw::DrawStyleBox(CDC* pDC, const CEtcGraphColor& etcGraphColor, const CRect& graphDrawingRegion, const CGraphColor& graphColor, const int dataStart, const double& dataViewMax, const double& dataViewMin, const CList<CPacket*, CPacket*>& packetList, CPenThickness penThickness)
{
	int count = 0;
	POSITION headPos = packetList.GetHeadPosition();
	if(headPos == NULL) 
		return false;
	CPacket* xPositionPacket; // x축 위치 packet
	CPacket* yPositionPacket; // y축 위치 packet

	if(packetList.GetCount() >= 3){
		xPositionPacket = packetList.GetNext(headPos);
		yPositionPacket = packetList.GetNext(headPos);
	}
	else
		return false;

	//displayData
	//POSITION xPosition = xPositionPacket->GetnumericDataList()->FindIndex(dataStart-1);
	//POSITION yPosition = yPositionPacket->GetnumericDataList()->FindIndex(dataStart-1);
	POSITION xPosition = xPositionPacket->GetnumericDataList()->FindIndex(dataStart);
	POSITION yPosition = yPositionPacket->GetnumericDataList()->FindIndex(dataStart);
	if(xPosition == NULL || yPosition == NULL)
		return false;
	
	CBrush ellipsBrush(graphColor.GetColor2());
	
	CPen ellipsPen;
	CPen linePen;
	CPen gridPen;

 	ellipsPen.CreatePen	(PS_SOLID, penThickness.GetThickness1(), graphColor.GetColor1());
	linePen.CreatePen	(PS_SOLID, penThickness.GetThickness1(), graphColor.GetColor4());
	gridPen.CreatePen	(PS_SOLID, penThickness.GetThickness1(), graphColor.GetColor3());

	CBrush* pOldBrush;
	CPen* pOldPen; 
	pOldBrush = (CBrush*)pDC->SelectObject(&ellipsBrush);
	pOldPen = (CPen*)pDC->SelectObject(&gridPen);
	
	POSITION xPositionEnd = xPosition, yPositionEnd = yPosition;
	int gridCount = 15;
	int ellipsThickness = 5;
	CPoint startDataPt;
	CPoint endDataPt;
	//grid를 그려준다. ========================================================================================
	CRect graphRegion = graphDrawingRegion;
	graphRegion.right = graphDrawingRegion.right + MARGIN * 2;
	CDraw::DrawLine(pDC, CPoint(graphRegion.left, graphRegion.top), CPoint(graphRegion.right, graphRegion.top));
	CDraw::DrawLine(pDC, CPoint(graphRegion.left, graphRegion.top + graphRegion.Height()/3), CPoint(graphRegion.right, graphRegion.top + graphRegion.Height()/3));
	CDraw::DrawLine(pDC, CPoint(graphRegion.left, graphRegion.top + graphRegion.Height()*2/3), CPoint(graphRegion.right, graphRegion.top + graphRegion.Height()*2/3));
	CDraw::DrawLine(pDC, CPoint(graphRegion.left, graphRegion.bottom), CPoint(graphRegion.right, graphRegion.bottom));
	CDraw::DrawLine(pDC, CPoint(graphRegion.left, graphRegion.top), CPoint(graphRegion.left, graphRegion.bottom));
	CDraw::DrawLine(pDC, CPoint(graphRegion.left + graphRegion.Width()/3, graphRegion.top), CPoint(graphRegion.left + graphRegion.Width()/3, graphRegion.bottom));
	CDraw::DrawLine(pDC, CPoint(graphRegion.left + graphRegion.Width()*2/3, graphRegion.top), CPoint(graphRegion.left + graphRegion.Width()*2/3, graphRegion.bottom));
	CDraw::DrawLine(pDC, CPoint(graphRegion.right, graphRegion.top), CPoint(graphRegion.right, graphRegion.bottom));

	for(int i = dataStart; i <= xPositionPacket->GetCount(); i++){
		if(xPosition == NULL || xPositionEnd == NULL || yPosition == NULL || yPositionEnd == NULL)
			break;

		count++;
		if(count == 1){
			startDataPt = CDataConversion::GetVerticalPoint(graphRegion, 0.0, (double)gridCount, 0.0, (double)gridCount, 
				yPositionPacket->GetnumericDataList()->GetNext(yPosition), xPositionPacket->GetnumericDataList()->GetNext(xPosition));
			endDataPt = startDataPt;
			yPosition = yPositionEnd;
			xPosition = xPositionEnd;
		}
		else{
			startDataPt = CDataConversion::GetVerticalPoint(graphRegion, 0.0, (double)gridCount, 0.0, (double)gridCount, yPositionPacket->GetnumericDataList()->GetNext(yPosition), xPositionPacket->GetnumericDataList()->GetNext(xPosition));
			yPositionEnd = yPosition;
			xPositionEnd = xPosition;
			endDataPt = CDataConversion::GetVerticalPoint(graphRegion, 0.0, (double)gridCount, 0.0, (double)gridCount, yPositionPacket->GetnumericDataList()->GetNext(yPositionEnd), xPositionPacket->GetnumericDataList()->GetNext(xPositionEnd));
		}
		// line을 먼저 그린다.
		pDC->SelectObject(&linePen);
		CDraw::DrawLine(pDC, startDataPt, endDataPt);

		//첫번째 데이터는 사각형으로 그리고 나머지 데이터는 원으로 그린다.
		CRect rect(endDataPt.x - ellipsThickness, endDataPt.y - ellipsThickness, endDataPt.x + ellipsThickness, endDataPt.y + ellipsThickness);
		
		if(count == 1){
			pDC->SelectObject(GetStockObject(NULL_BRUSH));
			pDC->SelectObject(&ellipsPen);
			CDraw::DrawRectangle(pDC, rect);
		}
		else{
			pDC->SelectObject(&ellipsBrush);
			pDC->SelectObject(&ellipsPen);
			pDC->Ellipse(rect);
		}
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	ellipsBrush.DeleteObject();

	ellipsPen.DeleteObject();
	linePen.DeleteObject();
	gridPen.DeleteObject();

	return true;
}

// 수익위험등급은 packet이 5개로 구성되어 있다.
// 0. 구분
// 1. x축 위치
// 2. y축 위치
// 3. x축 값
// 4. y축 값
// 그리는 데 필요한것은 위의 0, 1, 2번 packet만 필요하다.
// 주위점 x, y 좌 표는 총 16개 이다.
bool CFundDraw::DrawReturnRiskGrade(CDC* pDC, const CEtcGraphColor& etcGraphColor, const CRect& graphDrawingRegion, const CGraphColor& graphColor, const int dataStart, const double& dataViewMax, const double& dataViewMin, const CList<CPacket*, CPacket*>& packetList, CPenThickness penThickness)
{
	POSITION headPos = packetList.GetHeadPosition();
	if(headPos == NULL) 
		return false;
	CPacket* partPacket;      // 구분 packet
	CPacket* xPositionPacket; // x축 위치 packet
	CPacket* yPositionPacket; // y축 위치 packet

	if(packetList.GetCount() >= 3){
		partPacket = packetList.GetNext(headPos);
		xPositionPacket = packetList.GetNext(headPos);
		yPositionPacket = packetList.GetNext(headPos);
	}
	else
		return false;

	//displayData
	//POSITION partPosition = partPacket->GetstrDataList()->FindIndex(dataStart-1);
	//POSITION xPosition = xPositionPacket->GetnumericDataList()->FindIndex(dataStart-1);
	//POSITION yPosition = yPositionPacket->GetnumericDataList()->FindIndex(dataStart-1);
	POSITION partPosition = partPacket->GetstrDataList()->FindIndex(dataStart);
	POSITION xPosition = xPositionPacket->GetnumericDataList()->FindIndex(dataStart);
	POSITION yPosition = yPositionPacket->GetnumericDataList()->FindIndex(dataStart);
	if(partPosition == NULL || xPosition == NULL || yPosition == NULL)
		return false;
	
	CBrush FBrush(graphColor.GetColor2());
	CBrush UBrush(graphColor.GetColor1());
	CBrush BBrush(graphColor.GetColor4());
	
	CPen gridPen;
	int lineThickness = penThickness.GetThickness1();
	lineThickness = 4;
	gridPen.CreatePen(PS_INSIDEFRAME, lineThickness, RGB(192,192,192));
	
	CBrush* pOldBrush;
	CPen* pOldPen; 
	pOldBrush = (CBrush*)pDC->SelectObject(&FBrush);
	pOldPen = (CPen*)pDC->SelectObject(&gridPen);

	CRect graphRegion = graphDrawingRegion;
	graphRegion.bottom = graphDrawingRegion.bottom;
	graphRegion.left = graphDrawingRegion.left;
	graphRegion.right = graphDrawingRegion.right + MARGIN * 2;
	double height = graphRegion.Height();
	double width = graphRegion.Width();

	//grid line을 뺀 한칸의 width를 구한다.
	width = (graphRegion.Width() - 5 * lineThickness) / 5;
	height = (graphRegion.Height() - 5 * lineThickness) / 5;
	int count = 0;
	for(int i = dataStart; i <= xPositionPacket->GetCount(); i++){
		if(yPosition == NULL || xPosition == NULL || partPosition == NULL)
			break;
		count++;
		int gridMin = 0;
		int gridCount = 5;
		CPoint dataPt = CDataConversion::GetVerticalPoint(graphRegion, (double)gridMin, (double)gridCount, (double)gridMin, (double)gridCount, yPositionPacket->GetnumericDataList()->GetNext(yPosition), xPositionPacket->GetnumericDataList()->GetNext(xPosition));

		CRect rect(dataPt.x - int(width) - 1, dataPt.y + int(height), dataPt.x, dataPt.y);
		CString strpart = partPacket->GetstrDataList()->GetNext(partPosition);
		
		if(strpart == "F")
			pDC->FillRect(rect, &FBrush);
		else if(strpart == "U")
			pDC->FillRect(rect, &UBrush);
		else if(strpart == "B")
			pDC->FillRect(rect, &BBrush);
	}

	height = double (double(graphRegion.Height())/5);
	width = double (double(graphRegion.Width())/5);

	pDC->SelectObject(GetStockObject(NULL_BRUSH));
	for(int k = 0; k < 5 ; k++){ 
		double plusWidth = width * k;
		double minusWidth = width * (4 - k) - 4;
		for(int j = 0; j < 5 ; j++){ 
			double Height = height * j;
			double plusHeight = height * (j + 1);
			DrawBox(pDC, CRect(graphRegion.left + int(plusWidth), graphRegion.top + int(Height), graphRegion.right - (graphRegion.left + int(minusWidth)), graphRegion.top + int(plusHeight)));
		}
	}
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	FBrush.DeleteObject();
	UBrush.DeleteObject();
	BBrush.DeleteObject();
	
	gridPen.DeleteObject();

	return true;
}

// 선아래쪽은 채우고 모든 packet을 한거번에 그려야 한다.
// 일반적으로 날짜 packet과 거래소, 코스닥, 채권, 기타packet을 쓴다.
bool CFundDraw::DrawComponentRatio_Line(CDC* pDC, const CEtcGraphColor& etcGraphColor, const CRect& graphDrawingRegion, const CGraphColor& graphColor, const int dataStart, const int dataEnd, const int dataRealEnd, const double& dataViewMax, const double& dataViewMin, const CList<CPacket*, CPacket*>& packetList, CPenThickness penThickness)
{
	POSITION packetHeadPos = packetList.GetHeadPosition();
	if(packetHeadPos == NULL) 
		return false;

	int packetCount = packetList.GetCount();
	int i = 0;
	
	CList<POSITION, POSITION> packetPosition;
	for(i = 0; i < packetCount; i++){
		if(packetHeadPos == NULL)
			return false;

		CPacket* pPacket = packetList.GetNext(packetHeadPos);
		if(pPacket == NULL)
			return false;
		else{
			//displayData
			//POSITION pos = pPacket->GetnumericDataList()->FindIndex(dataStart-1);
			POSITION pos = pPacket->GetnumericDataList()->FindIndex(dataStart);
			if(pos == NULL)
				return false;
			else
				packetPosition.AddTail(pos);
		}
	}

	double realWidth = CDraw::CalRealWidth(graphDrawingRegion, dataStart, dataEnd);
	int lineThickness = penThickness.GetThickness1();
	lineThickness = 3;

	CPen pen1, pen2, pen3, pen4, pen5, pen6, pen7, pen8, pen9, pen10;

	pen1.CreatePen	(PS_SOLID, lineThickness, graphColor.GetColor1());
	pen2.CreatePen	(PS_SOLID, lineThickness, graphColor.GetColor2());
	pen3.CreatePen	(PS_SOLID, lineThickness, graphColor.GetColor3());
	pen4.CreatePen	(PS_SOLID, lineThickness, graphColor.GetColor4());
	pen5.CreatePen	(PS_SOLID, lineThickness, graphColor.GetColor5());
	pen6.CreatePen	(PS_SOLID, lineThickness, graphColor.GetColor6());
	pen7.CreatePen	(PS_SOLID, lineThickness, etcGraphColor.GetGraphColor1());
	pen8.CreatePen	(PS_SOLID, lineThickness, etcGraphColor.GetGraphColor2());
	pen9.CreatePen	(PS_SOLID, lineThickness, etcGraphColor.GetGraphColor3());
	pen10.CreatePen	(PS_SOLID, lineThickness, etcGraphColor.GetGraphColor4());

	CBrush firstBrush	(graphColor.GetColor1());
	CBrush secondBrush	(graphColor.GetColor2());
	CBrush thirdBrush	(graphColor.GetColor3());
	CBrush fourBrush	(graphColor.GetColor4());
	CBrush fiveBrush	(graphColor.GetColor5());
	CBrush sixBrush		(graphColor.GetColor6());
	CBrush sevenBrush	(etcGraphColor.GetGraphColor1());
	CBrush eightBrush	(etcGraphColor.GetGraphColor2());
	CBrush nineBrush	(etcGraphColor.GetGraphColor3());
	CBrush tenBrush		(etcGraphColor.GetGraphColor4());

	CBrush* pOldBrush;
	CPen* pOldPen; 
	pOldBrush = (CBrush*)pDC->SelectObject(&firstBrush);
	pOldPen = (CPen*)pDC->SelectObject(&pen1);

	CPoint fourPoint[4];
	int count = 0;
	for(i = dataStart; i <= dataRealEnd; i++){
		count++;
		packetHeadPos = packetList.GetHeadPosition();
		CPoint tempDataPt = CPoint(0, 0);
		CPoint tempBeforeDataPt = CPoint(0, 0);
		double tempData = 0;
		double beforeTempData = 0;

		for(int j = 0; j < packetCount; j++){
			CPacket* dataPacket = packetList.GetNext(packetHeadPos);

			POSITION dataPosPosition = packetPosition.FindIndex(j);
			POSITION tempPos = dataPosPosition;
			POSITION dataPos = packetPosition.GetNext(dataPosPosition);
	
			CString strData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::DoubleToString(dataPacket->GetnumericDataList()->GetNext(dataPos)), dataPacket->GetType());
			strData.TrimRight("%");
			double data = atof(strData);
			data = data + tempData;
			tempData = data;
		
			packetPosition.SetAt(tempPos, dataPos);
			if(count > 1){
				//displayData
				//POSITION beforeDataPos = dataPacket->GetnumericDataList()->FindIndex(dataStart + count - 3);
				POSITION beforeDataPos = dataPacket->GetnumericDataList()->FindIndex(dataStart + count - 2);
				CString beforeStrData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::DoubleToString(dataPacket->GetnumericDataList()->GetNext(beforeDataPos)), dataPacket->GetType());
				beforeStrData.TrimRight("%");
			
				double beforeData = atof(beforeStrData);
				beforeData = beforeData + beforeTempData;
				beforeTempData = beforeData;

				CPoint nowDataPt = CDataConversion::GetVerticalPoint(graphDrawingRegion, count, realWidth, dataViewMin, dataViewMax, data);
				CPoint beforeDataPt = CDataConversion::GetVerticalPoint(graphDrawingRegion, count-1, realWidth, dataViewMin, dataViewMax, beforeData);

				if(j==0)
					InsertFourPointToArray(fourPoint, beforeDataPt, nowDataPt, CPoint(nowDataPt.x, graphDrawingRegion.bottom), CPoint(beforeDataPt.x, graphDrawingRegion.bottom));
				else
					InsertFourPointToArray(fourPoint, beforeDataPt, nowDataPt, CPoint(nowDataPt.x, tempDataPt.y), CPoint(beforeDataPt.x, tempBeforeDataPt.y));
				
				tempDataPt.y = nowDataPt.y;
				tempBeforeDataPt.y = beforeDataPt.y;

				//pen, brush 선택해 주어야 한다~~
				int residual = 0;
				int brushNumber = 10;
				residual = (j+1) - (brushNumber * ((j+1)/ brushNumber));

				if(residual == 1){
					pDC->SelectObject(&firstBrush);
					pDC->SelectObject(&pen1);
				}
				else if(residual == 2){
					pDC->SelectObject(&secondBrush);
					pDC->SelectObject(&pen2);
				}
				else if(residual == 3){
					pDC->SelectObject(&thirdBrush);
					pDC->SelectObject(&pen3);
				}
				else if(residual == 4){
					pDC->SelectObject(&fourBrush);
					pDC->SelectObject(&pen4);
				}
				else if(residual == 5){
					pDC->SelectObject(&fiveBrush);
					pDC->SelectObject(&pen5);
				}
				else if(residual == 6){
					pDC->SelectObject(&sixBrush);
					pDC->SelectObject(&pen6);
				}
				else if(residual == 7){
					pDC->SelectObject(&sevenBrush);
					pDC->SelectObject(&pen7);
				}
				else if(residual == 8){
					pDC->SelectObject(&eightBrush);
					pDC->SelectObject(&pen8);
				}
				else if(residual == 9){
					pDC->SelectObject(&nineBrush);
					pDC->SelectObject(&pen9);
				}
				else{
					pDC->SelectObject(&tenBrush);
					pDC->SelectObject(&pen10);
				}
				pDC->Polygon(fourPoint,4);
				CDraw::DrawLine(pDC, beforeDataPt, nowDataPt);
			}
		}
	}
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	firstBrush.DeleteObject();
	secondBrush.DeleteObject();
	thirdBrush.DeleteObject();
	fourBrush.DeleteObject();
	fiveBrush.DeleteObject();
	sixBrush.DeleteObject();
	sevenBrush.DeleteObject();
	eightBrush.DeleteObject();
	nineBrush.DeleteObject();
	tenBrush.DeleteObject();

	pen1.DeleteObject();
	pen2.DeleteObject();
	pen3.DeleteObject();
	pen4.DeleteObject();
	pen5.DeleteObject();
	pen6.DeleteObject();
	pen7.DeleteObject();
	pen8.DeleteObject();
	pen9.DeleteObject();
	pen10.DeleteObject();
	
	return true;
}

//  packet이 4개로 구성되어 있다.
// 0. 구분
// 1. 펀드명
// 2. return(y축)
// 3. risk(x축)
// 그리는 데 필요한것은 위의 0, 2, 3번 packet만 필요하다.
bool CFundDraw::DrawReturnRiskDispersion(CDC* pDC, const CEtcGraphColor& etcGraphColor, const CRect& graphDrawingRegion, const CGraphColor& graphColor, const int dataStart, const double& dataViewMax, const double& dataViewMin, const double& dataViewXMax, const double& dataViewXMin, const CList<CPacket*, CPacket*>& packetList)
{
	int count = 0;
	POSITION headPos = packetList.GetHeadPosition();
	if(headPos == NULL) 
		return false;
	CPacket* partPacket;      // 구분 packet
	CPacket* fundNamePacket;
	CPacket* xPositionPacket; // x축 위치 packet
	CPacket* yPositionPacket; // y축 위치 packet

	if(packetList.GetCount() >= 3){
		partPacket = packetList.GetNext(headPos);
		fundNamePacket = packetList.GetNext(headPos);
		yPositionPacket = packetList.GetNext(headPos);
		xPositionPacket = packetList.GetNext(headPos);
	}
	else
		return false;

	//displayData
	//POSITION partPosition = partPacket->GetstrDataList()->FindIndex(dataStart-1);
	//POSITION xPosition = xPositionPacket->GetnumericDataList()->FindIndex(dataStart-1);
	//POSITION yPosition = yPositionPacket->GetnumericDataList()->FindIndex(dataStart-1);
	POSITION partPosition = partPacket->GetstrDataList()->FindIndex(dataStart);
	POSITION xPosition = xPositionPacket->GetnumericDataList()->FindIndex(dataStart);
	POSITION yPosition = yPositionPacket->GetnumericDataList()->FindIndex(dataStart);
	if(xPosition == NULL || yPosition == NULL)
		return false;
	
	int realDrawStart = yPositionPacket->GetStartPeriod() +1;
	int dataCount = xPositionPacket->GetCount();
	double xPosMin = 0.0, xPosMax = 0.0, yPosMin = 0.0, yPosMax = 0.0;
	xPosMin = dataViewXMin;
	xPosMax = dataViewXMax;
	yPosMin = dataViewMin;
	yPosMax = dataViewMax;
	
	CBrush UBrush	(graphColor.GetColor2());
	CBrush BBrush	(graphColor.GetColor1());
	CBrush KBrush	(graphColor.GetColor4());
	CBrush FBrush	(graphColor.GetColor3());
	CBrush elseBrush(graphColor.GetColor6());
	
	CBrush* pOldBrush;
	pOldBrush = (CBrush*)pDC->SelectObject(&UBrush);
	int unitSize = 8;
	for(int i = dataStart; i <= xPositionPacket->GetCount(); i++){
		count++;
		
		CPoint point4[4];
		CPoint DataPt = CDataConversion::GetVerticalPoint(graphDrawingRegion, yPosMin, yPosMax, xPosMin, xPosMax, yPositionPacket->GetnumericDataList()->GetNext(yPosition), xPositionPacket->GetnumericDataList()->GetNext(xPosition));
		// 구분 타입에 따라서 그려지는 모양을 선택하고 brush를 선택하면 된다.=====
		CString part = partPacket->GetstrDataList()->GetNext(partPosition);
		if(part == "U"){
			pDC->SelectObject(&UBrush);
			pDC->FillRect(CRect(DataPt.x - unitSize, DataPt.y - unitSize, DataPt.x + unitSize, DataPt.y + unitSize), &UBrush); 
		}
		else if(part == "B"){
			pDC->SelectObject(&BBrush);
			pDC->Ellipse(CRect(DataPt.x - unitSize, DataPt.y - unitSize, DataPt.x + unitSize, DataPt.y + unitSize));
		}
		else if(part == "K"){
			pDC->SelectObject(&KBrush);
			InsertFourPointToArray(point4, CPoint(DataPt.x - unitSize, DataPt.y), CPoint(DataPt.x, DataPt.y - unitSize), CPoint(DataPt.x + unitSize, DataPt.y), CPoint(DataPt.x, DataPt.y + unitSize));
			pDC->Polygon(point4, 4);
		}
		else if(part == "F"){
			pDC->SelectObject(&FBrush);
			InsertFourPointToArray(point4, CPoint(DataPt.x - unitSize, DataPt.y), CPoint(DataPt.x, DataPt.y - unitSize), CPoint(DataPt.x + unitSize, DataPt.y), CPoint(0, 0));
			pDC->Polygon(point4, 3);
		}
		else{
			pDC->SelectObject(&elseBrush);
			pDC->Ellipse(CRect(DataPt.x - unitSize/4, DataPt.y - unitSize/4, DataPt.x + unitSize/4, DataPt.y + unitSize/4));
		}
	}

	pDC->SelectObject(pOldBrush);

	UBrush.DeleteObject();
	BBrush.DeleteObject();
	KBrush.DeleteObject();
	FBrush.DeleteObject();
	elseBrush.DeleteObject();

	return true;
}

//  packet이 2개로 구성되어 있다.
// 0. fund
// 1. market
bool CFundDraw::DrawFundMarketDispersion(CDC* pDC, const CEtcGraphColor& etcGraphColor, const CRect& graphDrawingRegion, const CGraphColor& graphColor, const int dataStart, const double& dataViewMax, const double& dataViewMin, const CList<CPacket*, CPacket*>& packetList, CPenThickness penThickness)
{
	POSITION headPos = packetList.GetHeadPosition();
	if(headPos == NULL) 
		return false;
	CPacket* fundPacket;        // fund packet
	CPacket* marketPacket;		// market packet	

	if(packetList.GetCount() >= 2){
		fundPacket = packetList.GetNext(headPos);
		marketPacket = packetList.GetNext(headPos);
	}
	else
		return false;

	//displayData
	//POSITION fundPosition = fundPacket->GetnumericDataList()->FindIndex(dataStart-1);
	//POSITION marketPosition = marketPacket->GetnumericDataList()->FindIndex(dataStart-1);
	POSITION fundPosition = fundPacket->GetnumericDataList()->FindIndex(dataStart);
	POSITION marketPosition = marketPacket->GetnumericDataList()->FindIndex(dataStart);
	
	if(fundPosition == NULL || marketPosition == NULL)
		return false;
	
	int realDrawStart = fundPacket->GetStartPeriod() +1;
	int dataCount = fundPacket->GetCount();
	double xPosMin = 0.0, xPosMax = 0.0, yPosMin = 0.0, yPosMax = 0.0;
	fundPacket->GetMinMax(realDrawStart, dataCount, yPosMin, yPosMax);
	marketPacket->GetMinMax(realDrawStart, dataCount, xPosMin, xPosMax);
	
	CBrush unitBrush(graphColor.GetColor2());
	
	CPen linePen;
	CPen zoroLinePen;
	int lineThickness = penThickness.GetThickness1();
	zoroLinePen.CreatePen(PS_SOLID, 1, graphColor.GetColor1());
	linePen.CreatePen(PS_SOLID, lineThickness, graphColor.GetColor6());
	
	CBrush* pOldBrush;
	CPen* pOldPen; 
	pOldBrush = (CBrush*)pDC->SelectObject(&unitBrush);
	pOldPen = (CPen*)pDC->SelectObject(&linePen);
	int unitSize = 5;
	int count = 0;
	for(int i = dataStart; i <= dataCount; i++){
		count++;
		CPoint point4[4];
		double fundData = fundPacket->GetnumericDataList()->GetNext(fundPosition);
		double marketData = marketPacket->GetnumericDataList()->GetNext(marketPosition);
		
		CPoint DataPt = CDataConversion::GetVerticalPoint(graphDrawingRegion, yPosMin, yPosMax, xPosMin, xPosMax, fundData, marketData);
		InsertFourPointToArray(point4, CPoint(DataPt.x - unitSize, DataPt.y), CPoint(DataPt.x, DataPt.y - unitSize), CPoint(DataPt.x + unitSize, DataPt.y), CPoint(DataPt.x, DataPt.y + unitSize));
		pDC->Polygon(point4, 4);
	}

	//0선 기준을 그려준다
	pDC->SelectObject(&zoroLinePen);
	CPoint zeroPoint = CDataConversion::GetVerticalPoint(graphDrawingRegion, yPosMin, yPosMax, xPosMin, xPosMax, 0.0, 0.0);
	if(yPosMin < 0 && yPosMax > 0 )
		CDraw::DrawLine(pDC, CPoint(graphDrawingRegion.left, zeroPoint.y), CPoint(graphDrawingRegion.right, zeroPoint.y));
	if(xPosMin < 0 && xPosMax > 0 )
		CDraw::DrawLine(pDC, CPoint(zeroPoint.x, graphDrawingRegion.top), CPoint(zeroPoint.x, graphDrawingRegion.bottom));

	// line 을 그려주는 부분을 추가해야 한다.
	CPoint ptStart( 0, 0);
	CPoint ptEnd( 0, 0);

	pDC->SelectObject(&linePen);
	//==============================================================================
	//sy 2005.10.04 fund는 사용하지 않기 때문에 사용 안함
	//fundPacket->GetFundTypeStartandEndPoint(ptStart, ptEnd);
	//==============================================================================
	CPoint startPoint = CDataConversion::GetVerticalPoint(graphDrawingRegion, yPosMin, yPosMax, xPosMin, xPosMax, ptStart.y, ptStart.x);
	CPoint endPoint = CDataConversion::GetVerticalPoint(graphDrawingRegion, yPosMin, yPosMax, xPosMin, xPosMax, ptEnd.y, ptEnd.x);
	
	CDraw::DrawLine(pDC, startPoint, endPoint);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	
	unitBrush.DeleteObject();


	linePen.DeleteObject();
	zoroLinePen.DeleteObject();
	
	return true;
}

//  packet이 3개로 구성되어 있다.
// 0. part
// 1. 구성비
// 2. 평각액
bool CFundDraw::DrawComponentRatio_Pie(CDC* pDC, const CRect& realRegion, const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData, const CSubGraphData& subGraphData)
{
	return CPieDraw().DrawGraph(pDC, realRegion, displayAttributes, drawingGraphData, subGraphData);
}

bool CFundDraw::DrawComponentRatio_Bar_Line(CDC* pDC, const CEtcGraphColor& etcGraphColor, const CRect& graphDrawingRegion, const CGraphColor& graphColor, const int dataStart, const int dataEnd, const int dataRealEnd, const double& dataViewMax, const double& dataViewMin, const CList<CPacket*, CPacket*>& packetList, CPenThickness penThickness)
{
	POSITION packetHeadPos = packetList.GetHeadPosition();
	if(packetHeadPos == NULL) 
		return false;

	int i = 0;
	int packetCount = packetList.GetCount();
	CList<POSITION, POSITION> packetPosition;
	for( i = 0; i < packetCount; i++){
		CPacket* pPacket = packetList.GetNext(packetHeadPos);
		if(pPacket == NULL)
			return false;
		else{
			//displayData
			//POSITION pos = pPacket->GetnumericDataList()->FindIndex(dataStart-1);
			POSITION pos = pPacket->GetnumericDataList()->FindIndex(dataStart);
			if(pos == NULL)
				return false;
			else
				packetPosition.AddTail(pos);
		}
	}
	double widthRatio = 8.0/10.0;
	double realWidth = CDraw::CalRealWidth(graphDrawingRegion, dataStart, dataEnd);
	int bongWidth = (int)(realWidth*widthRatio);
	if(bongWidth <= 1) 
		bongWidth = 1;
	int lineThickness = CDraw::CalUpLineThickness(penThickness, bongWidth);

	bongWidth = bongWidth - lineThickness;
	bongWidth = bongWidth / 3;

	CPen pen, pen1, pen2, pen3, pen4, pen5, pen6, pen7, pen8, pen9, pen10;
	pen.CreatePen(PS_SOLID, lineThickness, etcGraphColor.GetGraphColor5());

	pen1.CreatePen	(PS_SOLID, lineThickness, graphColor.GetColor1());
	pen2.CreatePen	(PS_SOLID, lineThickness, graphColor.GetColor2());
	pen3.CreatePen	(PS_SOLID, lineThickness, graphColor.GetColor3());
	pen4.CreatePen	(PS_SOLID, lineThickness, graphColor.GetColor4());
	pen5.CreatePen	(PS_SOLID, lineThickness, graphColor.GetColor5());
	pen6.CreatePen	(PS_SOLID, lineThickness, graphColor.GetColor6());
	pen7.CreatePen	(PS_SOLID, lineThickness, etcGraphColor.GetGraphColor1());
	pen8.CreatePen	(PS_SOLID, lineThickness, etcGraphColor.GetGraphColor2());
	pen9.CreatePen	(PS_SOLID, lineThickness, etcGraphColor.GetGraphColor3());
	pen10.CreatePen	(PS_SOLID, lineThickness, etcGraphColor.GetGraphColor4());

	int brushCount = 10;
	CBrush firstBrush	(graphColor.GetColor1());
	CBrush secondBrush	(graphColor.GetColor2());
	CBrush thirdBrush	(graphColor.GetColor3());
	CBrush fourBrush	(graphColor.GetColor4());
	CBrush fiveBrush	(graphColor.GetColor5());
	CBrush sixBrush		(graphColor.GetColor6());
	CBrush sevenBrush	(etcGraphColor.GetGraphColor1());
	CBrush eightBrush	(etcGraphColor.GetGraphColor2());
	CBrush nineBrush	(etcGraphColor.GetGraphColor3());
	CBrush tenBrush		(etcGraphColor.GetGraphColor4());

	CBrush* pOldBrush;
	CPen* pOldPen; 
	pOldBrush = (CBrush*)pDC->SelectObject(&firstBrush);
	pOldPen = (CPen*)pDC->SelectObject(&pen);

	int count = 0;
	for(i = dataStart; i <= dataRealEnd; i++){
		count++;
		packetHeadPos = packetList.GetHeadPosition();
		int saveDataY = 0;
		int beforeDataY;
		int linePt = 0;
		CPoint DataPt;
		CPoint beforeDataPt;
		for(int j = 0; j < packetCount; j++){
			CPacket* dataPacket = packetList.GetNext(packetHeadPos);
			POSITION dataPosPosition = packetPosition.FindIndex(j);
			POSITION tempPos = dataPosPosition;
			POSITION dataPos = packetPosition.GetNext(dataPosPosition);
			CString strData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::DoubleToString(dataPacket->GetnumericDataList()->GetNext(dataPos)), dataPacket->GetType());
			strData.TrimRight("%");

			double data = atof(strData);

			packetPosition.SetAt(tempPos, dataPos);
			DataPt = CDataConversion::GetVerticalPoint(graphDrawingRegion, count, realWidth, dataViewMin, dataViewMax, data);

			if(data == 0)
				DataPt.y = graphDrawingRegion.bottom;
			if(data != 0 && graphDrawingRegion.bottom == DataPt.y)
				DataPt.y --;

			saveDataY = saveDataY + (graphDrawingRegion.bottom - DataPt.y);
			CRect rect;
			//============================================================================================
			if(count > 1){
				//displayData
				//POSITION beforeDataPos = dataPacket->GetnumericDataList()->FindIndex(dataStart + count - 3);
				POSITION beforeDataPos = dataPacket->GetnumericDataList()->FindIndex(dataStart + count - 2);
				CString beforeStrData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::DoubleToString(dataPacket->GetnumericDataList()->GetNext(beforeDataPos)), dataPacket->GetType());
				beforeStrData.TrimRight("%");

				double beforeData = atof(beforeStrData);
				beforeDataPt = CDataConversion::GetVerticalPoint(graphDrawingRegion, count-1, realWidth, dataViewMin, dataViewMax, beforeData);
				if(beforeData == 0)
					beforeDataPt.y = graphDrawingRegion.bottom;
				if(beforeData != 0 && graphDrawingRegion.bottom == beforeDataPt.y)
					beforeDataPt.y --;

				linePt = linePt + (graphDrawingRegion.bottom - beforeDataPt.y);
			}
			//============================================================================================
			
			if(j == 0)
				rect = CRect(DataPt.x - bongWidth - lineThickness/2, DataPt.y, DataPt.x + bongWidth + lineThickness, graphDrawingRegion.bottom);
			else
				rect = CRect(DataPt.x - bongWidth - lineThickness/2, graphDrawingRegion.bottom - saveDataY, DataPt.x + bongWidth + lineThickness, graphDrawingRegion.bottom - beforeDataY);
				
			beforeDataY	= saveDataY;
			
			int selectBrush = (j+1) - (brushCount * ((j+1)/ brushCount));
				
			if(selectBrush == 1){
				if(count > 1){
					pDC->SelectObject(&pen1);
					CDraw::DrawLine(pDC, CPoint(beforeDataPt.x + bongWidth, graphDrawingRegion.bottom - linePt), CPoint(rect.left, rect.top));
				}
				pDC->FillRect(rect, &firstBrush);
			}
			else if(selectBrush == 2){	
				if(count > 1){
					pDC->SelectObject(&pen2);
					CDraw::DrawLine(pDC, CPoint(beforeDataPt.x + bongWidth, graphDrawingRegion.bottom - linePt), CPoint(rect.left, rect.top));
				}
				pDC->FillRect(rect, &secondBrush);
			}
			else if(selectBrush == 3){
				if(count > 1){
					pDC->SelectObject(&pen3);
					CDraw::DrawLine(pDC, CPoint(beforeDataPt.x + bongWidth, graphDrawingRegion.bottom - linePt), CPoint(rect.left, rect.top));
				}
				pDC->FillRect(rect, &thirdBrush);
			}
			else if(selectBrush == 4){
				if(count > 1){
					pDC->SelectObject(&pen4);
					CDraw::DrawLine(pDC, CPoint(beforeDataPt.x + bongWidth, graphDrawingRegion.bottom - linePt), CPoint(rect.left, rect.top));
				}
				pDC->FillRect(rect, &fourBrush);
			}
			else if(selectBrush == 5){
				if(count > 1){
					pDC->SelectObject(&pen5);
					CDraw::DrawLine(pDC, CPoint(beforeDataPt.x + bongWidth, graphDrawingRegion.bottom - linePt), CPoint(rect.left, rect.top));
				}
				pDC->FillRect(rect, &fiveBrush);
			}
			else if(selectBrush == 6){
				if(count > 1){
					pDC->SelectObject(&pen6);
					CDraw::DrawLine(pDC, CPoint(beforeDataPt.x + bongWidth, graphDrawingRegion.bottom - linePt), CPoint(rect.left, rect.top));
				}
				pDC->FillRect(rect, &sixBrush);
			}
			else if(selectBrush == 7){
				if(count > 1){
					pDC->SelectObject(&pen7);
					CDraw::DrawLine(pDC, CPoint(beforeDataPt.x + bongWidth, graphDrawingRegion.bottom - linePt), CPoint(rect.left, rect.top));
				}
				pDC->FillRect(rect, &sevenBrush);
			}
			else if(selectBrush == 8){
				if(count > 1){
					pDC->SelectObject(&pen8);
					CDraw::DrawLine(pDC, CPoint(beforeDataPt.x + bongWidth, graphDrawingRegion.bottom - linePt), CPoint(rect.left, rect.top));
				}
				pDC->FillRect(rect, &eightBrush);
			}
			else if(selectBrush == 9){
				if(count > 1){
					pDC->SelectObject(&pen9);
					CDraw::DrawLine(pDC, CPoint(beforeDataPt.x + bongWidth, graphDrawingRegion.bottom - linePt), CPoint(rect.left, rect.top));
				}
				pDC->FillRect(rect, &nineBrush);
			}
			else if(selectBrush == 10){
				if(count > 1){
					pDC->SelectObject(&pen10);
					CDraw::DrawLine(pDC, CPoint(beforeDataPt.x + bongWidth, graphDrawingRegion.bottom - linePt), CPoint(rect.left, rect.top));
				}
				pDC->FillRect(rect, &tenBrush);
			}
			else
				pDC->FillRect(rect, &firstBrush);

			if(rect.Height() <= 1 && rect.Height() > 0)
				CDraw::DrawLine(pDC, CPoint(rect.left, rect.top), CPoint(rect.right, rect.top));
		}
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->SelectObject(&pen);
	//	CDraw::DrawRectangle(pDC, CRect(DataPt.x - bongWidth - lineThickness/2, graphDrawingRegion.top, DataPt.x + bongWidth + lineThickness, graphDrawingRegion.bottom));
	}
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	firstBrush.DeleteObject();
	secondBrush.DeleteObject();
	thirdBrush.DeleteObject();
	fourBrush.DeleteObject();
	fiveBrush.DeleteObject();
	sixBrush.DeleteObject();
	sevenBrush.DeleteObject();
	eightBrush.DeleteObject();
	nineBrush.DeleteObject();
	tenBrush.DeleteObject();

	pen.DeleteObject();
	pen1.DeleteObject();
	pen2.DeleteObject();
	pen3.DeleteObject();
	pen4.DeleteObject();
	pen5.DeleteObject();
	pen6.DeleteObject();
	pen7.DeleteObject();
	pen8.DeleteObject();
	pen9.DeleteObject();
	pen10.DeleteObject();
	return true;
}

bool CFundDraw::DrawGroupBar(CDC* pDC, const CEtcGraphColor& etcGraphColor, const CRect& graphDrawingRegion, const CGraphColor& graphColor, const int dataStart, const int dataEnd, const int dataRealEnd, const double& dataViewMax, const double& dataViewMin, const CList<CPacket*, CPacket*>& packetList, CPenThickness penThickness)
{
	POSITION pos = packetList.GetHeadPosition();
	
	int packetCount = packetList.GetCount();

	CPacket* pPacket;
	for(int j = 0; j < packetCount; j++){
		if(pos == NULL)
			return false;
		pPacket = packetList.GetNext(pos);
		if(pPacket == NULL)
			return false;
	}

	double widthRatio=8.0/10.0;	//전체 barwidth에서 바가 차지하는 비율  
	double realWidth = CDraw::CalRealWidth(graphDrawingRegion, dataStart, dataEnd);
	double barWidth = int(realWidth*widthRatio);
	double realBarWidth = barWidth / packetCount;

	int lineThickness = CDraw::CalUpLineThickness(penThickness, int(realBarWidth));

	int brushCount = 10;
	CBrush firstBrush	(graphColor.GetColor1());
	CBrush secondBrush	(graphColor.GetColor2());
	CBrush thirdBrush	(graphColor.GetColor3());
	CBrush fourBrush	(graphColor.GetColor4());
	CBrush fiveBrush	(graphColor.GetColor5());
	CBrush sixBrush		(graphColor.GetColor6());
	CBrush sevenBrush	(etcGraphColor.GetGraphColor1());
	CBrush eightBrush	(etcGraphColor.GetGraphColor2());
	CBrush nineBrush	(etcGraphColor.GetGraphColor3());
	CBrush tenBrush		(etcGraphColor.GetGraphColor4());

	CPen pen;
	pen.CreatePen(PS_SOLID, lineThickness, etcGraphColor.GetGraphColor5());

	CBrush* pOldBrush;
	CPen* pOldPen; 
	pOldBrush = (CBrush*)pDC->SelectObject(&firstBrush);
	pOldPen = (CPen*)pDC->SelectObject(&pen);

	// 기준선 = 0.0
	CPoint standardDataPt = CDataConversion::GetVerticalPoint(graphDrawingRegion, 1, realWidth, dataViewMin, dataViewMax, 0.0);
	if(standardDataPt.y > graphDrawingRegion.top && standardDataPt.y < graphDrawingRegion.bottom)
		CDraw::DrawLine(pDC, CPoint(graphDrawingRegion.left, standardDataPt.y), CPoint(graphDrawingRegion.right, standardDataPt.y));

	int count = 0;
	for(int i = dataStart; i <= dataRealEnd; i++){
		count++;
		pos = packetList.GetHeadPosition();
		for(int k = 0; k < packetCount; k++){
			pPacket = packetList.GetNext(pos);
			
			POSITION packetPos = pPacket->GetnumericDataList()->FindIndex(i - 1);
			/*CString strData = CDataConversion::GetStringFormatTrans(CDataConversion::IntToString(pPacket->GetnumericDataList()->GetNext(packetPos)), pPacket->GetType());
			strData.TrimRight("%");*/

			double volumnData = pPacket->GetnumericDataList()->GetNext(packetPos);
			
			CPoint dataPt = CDataConversion::GetVerticalPoint(graphDrawingRegion, count, realWidth, dataViewMin, dataViewMax, volumnData);
			CRect rect(dataPt.x - int(barWidth/2) + int(realBarWidth * k), dataPt.y, dataPt.x - int(barWidth/2) + int(realBarWidth * (k+1)), standardDataPt.y);
	
			if(rect.top < graphDrawingRegion.top)
				rect.top = graphDrawingRegion.top;
			if(rect.bottom < graphDrawingRegion.top)
				rect.bottom = graphDrawingRegion.top;
			if(rect.bottom > graphDrawingRegion.bottom)
				rect.bottom = graphDrawingRegion.bottom;
			if(rect.top > graphDrawingRegion.bottom)
				rect.top = graphDrawingRegion.bottom;

			int selectBrush = (k+1) - (brushCount * ((k+1)/ brushCount));

			if(selectBrush == 1)
				pDC->SelectObject(&firstBrush);
			else if(selectBrush == 2)				
				pDC->SelectObject(&secondBrush);
			else if(selectBrush == 3)
				pDC->SelectObject(&thirdBrush);
			else if(selectBrush == 4)
				pDC->SelectObject(&fourBrush);
			else if(selectBrush == 5)
				pDC->SelectObject(&fiveBrush);
			else if(selectBrush == 6)
				pDC->SelectObject(&sixBrush);
			else if(selectBrush == 7)
				pDC->SelectObject(&sevenBrush);
			else if(selectBrush == 8)
				pDC->SelectObject(&eightBrush);
			else if(selectBrush == 9)
				pDC->SelectObject(&nineBrush);
			else if(selectBrush == 10)
				pDC->SelectObject(&tenBrush);
//			else
//				pDC->SelectObject(&firstBrush);

			// 바를 그려주는 부분 
			if(barWidth < 1)
					CDraw::DrawLine(pDC, CPoint(rect.left, rect.top), CPoint(rect.left, rect.bottom));
			else{
				if(rect.top == rect.bottom)
					CDraw::DrawLine(pDC, CPoint(rect.left, rect.bottom), CPoint(rect.right, rect.bottom));
				else
					CDraw::DrawRectangle(pDC, rect);
			}
		}	
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	firstBrush.DeleteObject();
	secondBrush.DeleteObject();
	thirdBrush.DeleteObject();
	fourBrush.DeleteObject();
	fiveBrush.DeleteObject();
	sixBrush.DeleteObject();
	sevenBrush.DeleteObject();
	eightBrush.DeleteObject();
	nineBrush.DeleteObject();
	tenBrush.DeleteObject();

	pen.DeleteObject();
	return false;
}

void CFundDraw::DrawBox(CDC* pDC, CRect rect)
{
	CDraw::DrawRectangle(pDC, rect);
	pDC->DrawEdge(rect, EDGE_SUNKEN, BF_RECT);
}






