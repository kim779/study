// PieDraw.cpp: implementation of the CPieDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PieDraw.h"

#include "DataMath.h"
#include "Conversion.h"

#include "BaseData.h"
#include "PacketPointerList.h"

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

CPieDraw::CPieDraw()
{
	m_eGraphType = CGraphBaseData::Pie_Type;
}

CPieDraw::~CPieDraw()
{
}

bool CPieDraw::DrawGraph(CDC* pDC,
			const CRect& realRegion, 
			const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData,
			const CSubGraphData& subGraphData)
{
	const CList< CPacket *, CPacket *> &packetList = drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return false;

	const CGraphColor& graphColor = subGraphData.GetColor();
	const CEtcGraphColor& etcGraphColor = drawingGraphData.GetEtcGraphColor();
	int nRealStart = displayAttributes.GetDataStartInBlock();
	int nRealEnd = displayAttributes.GetDataEndInBlock();
	double dViewMax = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(); 
	double dViewMin = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin();

	switch( subGraphData.GetStyle().GetGraphPieStyle())
	{
	case CGraphBaseData::FlatPie:
		return DrawFlatPie(pDC, realRegion, graphColor, etcGraphColor, 
			nRealStart, nRealEnd, dViewMax, dViewMin, packetList);
	case CGraphBaseData::SolidPie:
		return DrawSolidPie(pDC, realRegion, graphColor, etcGraphColor, 
			nRealStart, nRealEnd, dViewMax, dViewMin, packetList);
	}
	return false;
}

bool CPieDraw::DrawGraph(CDC* pDC,
					   const CRect& realRegion, 
					   const CDisplayAttributes& displayAttributes, 
					   const CDrawingGraphData& drawingGraphData,
					   const CSubGraphData& subGraphData,
					   CXScaleManager* pxScaleManager)
{
	const CList< CPacket *, CPacket *> &packetList = drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return false;
	
	const CGraphColor& graphColor = subGraphData.GetColor();
	const CEtcGraphColor& etcGraphColor = drawingGraphData.GetEtcGraphColor();
	int nRealStart = displayAttributes.GetDataStartInBlock();
	int nRealEnd = displayAttributes.GetDataEndInBlock();
	double dViewMax = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(); 
	double dViewMin = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin();
	
	if(pxScaleManager && pxScaleManager->GetThreadFlag() == THREAD_CALCULATION)
		return true;

	switch( subGraphData.GetStyle().GetGraphPieStyle())
	{
	case CGraphBaseData::FlatPie:
		return DrawFlatPie(pDC, realRegion, graphColor, etcGraphColor, 
			nRealStart, nRealEnd, dViewMax, dViewMin, packetList);
	case CGraphBaseData::SolidPie:
		return DrawSolidPie(pDC, realRegion, graphColor, etcGraphColor, 
			nRealStart, nRealEnd, dViewMax, dViewMin, packetList);
	}
	return false;

}

//  packet이 3개로 구성되어 있다.
// 0. part
// 1. DATA
bool CPieDraw::DrawFlatPie(CDC* pDC, const CRect& graphDrawingRegion, const CGraphColor& graphColor, const CEtcGraphColor& etcGraphColor, const int dataStart, const int dataRealEnd, const double& dataViewMax, const double& dataViewMin, const CList<CPacket*, CPacket*>& packetList)
{
	// 1. Check and Get the Data and Legend Packet
	POSITION headPos = packetList.GetHeadPosition();
	if(headPos == NULL) return false;
	CPacket* textPacket = NULL;     // text packet
	CPacket* dataPacket = NULL;		// data packet	
	if(packetList.GetCount() >= 2)
	{
		textPacket = packetList.GetNext(headPos);
		dataPacket = packetList.GetNext(headPos);
	}
	else return false;
	CList<CString, CString>* pTextList = textPacket->GetstrDataList();
	CList<double, double>* pDataList = dataPacket->GetnumericDataList();
	if(pTextList == NULL || pDataList == NULL) return false;

	// 2. Check First Data
	POSITION textPosition = pTextList->FindIndex(dataStart);
	POSITION dataPosition = pDataList->FindIndex(dataStart);	
	if(textPosition == NULL || dataPosition == NULL) return false;

	// 3. Get the Common Head/Tail Space and Max Length and Count in Legend String.
	//그래프옆에 데이터를 표현하기 위해.
	//데이터의 최대 길이와 갯수를 가지고 와서 계산한다.
	int nMaxLabelLength = 0;
	int nMaxValueLength = 0;
	int nDataCount = textPacket->GetCount();
	CString strPacketType = dataPacket->GetType();
	int nLegendCount = 0;
	for(int i = 0; i < nDataCount; i++)
	{
		if( textPosition == NULL || dataPosition == NULL) break;
		CString Str = pTextList->GetNext(textPosition);
		double nowData = pDataList->GetNext(dataPosition);
		// 데이타가 없을때는 표현하지 않는다.
		if(nowData <= 0) continue;

		Str.TrimLeft(" ");
		Str.TrimRight(" ");
		int nLabelLength = pDC->GetTextExtent( Str).cx;
		if( nLabelLength > nMaxLabelLength) nMaxLabelLength = nLabelLength;

		Str = ":" + CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::DoubleToString( nowData), strPacketType);
		int nValueLength = pDC->GetTextExtent( Str).cx;
		if( nValueLength > nMaxValueLength) nMaxValueLength = nValueLength;

		nLegendCount++;
	}

	// (2006/10/13 - Seung-Won, Bae) Draw Legend Rectangle
	// modified by rich 02/10/15 세로텍스트 간격 2pixels 추가
	int tempHeight = pDC->GetTextExtent("0").cy + 2;
	CRect graphRegion = graphDrawingRegion;
	CRect rctLegend( 0, graphRegion.top, 0, graphRegion.top + tempHeight * nLegendCount + 6);
	int textColumn = 0;
	if( ( graphRegion.Height() - 6) / tempHeight > 0) textColumn = ( nLegendCount - 1) / ( ( graphRegion.Height() - 6) / tempHeight) + 1;
	else textColumn = nLegendCount;
	rctLegend.right = graphRegion.right;
	graphRegion.right = graphRegion.right - ( nMaxLabelLength + nMaxValueLength + 10) * textColumn;
	rctLegend.left = graphRegion.right - 3;
	if( textColumn > 1)
	{
		rctLegend.bottom = graphRegion.bottom;
		if( rctLegend.Height() < tempHeight + 6) rctLegend.bottom = rctLegend.top + tempHeight + 6;
	}

	CRect pieRect = graphDrawingRegion;
	pieRect.DeflateRect( pieRect.Width() / 20, pieRect.Height() / 20);
/*
	// modified by rich 02/10/15 파이위치 가운데 아래로
	CRect tempRect;
	tempRect.left = graphDrawingRegion.CenterPoint().x - pieRect.Width()/2;
	tempRect.top = graphDrawingRegion.CenterPoint().y - pieRect.Height();
	tempRect.right = graphDrawingRegion.CenterPoint().x + pieRect.Width()/2;
	tempRect.bottom = graphDrawingRegion.CenterPoint().y + pieRect.Height();
	pieRect = tempRect;
*/

	CRect backPieRect = pieRect;
	backPieRect.OffsetRect(0, -15);
	CPoint downCenterPoint(pieRect.left + pieRect.Width() / 2, pieRect.top + pieRect.Height() / 2);
	CPoint upCenterPoint(backPieRect.left + backPieRect.Width() / 2, backPieRect.top + backPieRect.Height() / 2);
	CList<double, double> angleList;
	GetPieAllAngles(pDataList, angleList);

	// ?. Make Brush
	CBrush brush[10];
	brush[0].CreateSolidBrush(graphColor.GetColor1());
	brush[1].CreateSolidBrush(graphColor.GetColor2());
	brush[2].CreateSolidBrush(graphColor.GetColor3());
	brush[3].CreateSolidBrush(graphColor.GetColor4());
	brush[4].CreateSolidBrush(graphColor.GetColor5());
	brush[5].CreateSolidBrush(graphColor.GetColor6());
	brush[6].CreateSolidBrush(etcGraphColor.GetGraphColor1());
	brush[7].CreateSolidBrush(etcGraphColor.GetGraphColor2());
	brush[8].CreateSolidBrush(etcGraphColor.GetGraphColor3());
	brush[9].CreateSolidBrush(etcGraphColor.GetGraphColor4());

	// ?. Make Pie Pen.
	int lineThickness = 1;
	CPen linePen;
	linePen.CreatePen(PS_SOLID, lineThickness, RGB(0,0,0));

	// 바닥판 그리는부분
	DrawPie(pDC, brush, linePen, angleList, pieRect, downCenterPoint);

	// (2006/10/13 - Seung-Won, Bae) Draw Legend Rectangle
	CPen* pOldPen = (CPen*)pDC->SelectStockObject( BLACK_PEN);
	pDC->Rectangle( rctLegend);
	
	// ?. Make Pen.
	CPen pen[10];
	pen[0].CreatePen(PS_SOLID, 1, graphColor.GetColor1());
	pen[1].CreatePen(PS_SOLID, 1, graphColor.GetColor2());
	pen[2].CreatePen(PS_SOLID, 1, graphColor.GetColor3());
	pen[3].CreatePen(PS_SOLID, 1, graphColor.GetColor4());
	pen[4].CreatePen(PS_SOLID, 1, graphColor.GetColor5());
	pen[5].CreatePen(PS_SOLID, 1, graphColor.GetColor6());
	pen[6].CreatePen(PS_SOLID, 1, etcGraphColor.GetGraphColor1());
	pen[7].CreatePen(PS_SOLID, 1, etcGraphColor.GetGraphColor2());
	pen[8].CreatePen(PS_SOLID, 1, etcGraphColor.GetGraphColor3());
	pen[9].CreatePen(PS_SOLID, 1, etcGraphColor.GetGraphColor4());

	pDC->SelectObject(&pen[0]);
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&brush[0]);

	// (2006/10/13 - Seung-Won, Bae) Draw Legend after Pie.
	//out
	int tempCount = 0;
	int textCount = 0;
	int column = 0;
	textPosition = pTextList->FindIndex(dataStart);
	dataPosition = pDataList->FindIndex(dataStart);
	int MODE = pDC->SetBkMode(TRANSPARENT);
	COLORREF clr = pDC->GetTextColor();
	for(int i = dataStart; i <= dataRealEnd; i++, tempCount++)
	{
		if(textPosition == NULL || dataPosition == NULL)
			break;
		double nowData = pDataList->GetNext(dataPosition);
		CString Str = pTextList->GetNext(textPosition);
		// 데이타가 없을때는 표현하지 않는다.
		if(nowData <= 0) continue;

		int residual = 0;
		int brushNumber = 10;
		residual = tempCount - (int(tempCount / brushNumber) * brushNumber);
		pDC->SelectObject(&brush[residual]);
		pDC->SelectObject(&pen[residual]);

		//sy 2002.10.24
		CRect rect(graphRegion.right, graphRegion.top, graphRegion.right, graphRegion.top);
		rect.InflateRect(0, 0, ( nMaxLabelLength + nMaxValueLength + MARGIN), tempHeight);

		// text 위치가 영역을 벗어날 경우 처리
		int nGap_Y = tempHeight*textCount +MARGIN;
		if(rect.top < graphDrawingRegion.top)
			nGap_Y += (graphDrawingRegion.top - rect.top);
		rect.OffsetRect( ( nMaxLabelLength + nMaxValueLength + 10) * column, nGap_Y);
		//end

		// modified by rich 02/10/15 텍스트색 검정으로
		pDC->SetTextColor(RGB(0,0,0));

		pDC->FillRect(CRect(rect.left, rect.top, rect.left +5, rect.bottom),&brush[residual]);

		Str.TrimLeft(" ");
		Str.TrimRight(" ");
		pDC->ExtTextOut(rect.left+6, rect.top, NULL/*ETO_CLIPPED*/, CRect(rect.left+6, rect.top, rect.right, rect.bottom), Str, NULL);

		Str = ":" + CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::DoubleToString(nowData), strPacketType);
		pDC->ExtTextOut(rect.left+6 + nMaxLabelLength, rect.top, NULL/*ETO_CLIPPED*/, CRect(rect.left+6  + nMaxLabelLength + nMaxValueLength, rect.top, rect.right, rect.bottom), Str, NULL);

		textCount++;
		if( rctLegend.Height() - 6 < tempHeight * ( textCount + 1))
		{
			textCount =0;
			column++;
		}
	}

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	linePen.DeleteObject();
	for(int k = 0; k < 10; k++)
	{
		brush[k].DeleteObject();
		pen[k].DeleteObject();
	}
	
	return true;
}

void CPieDraw::DrawPie(CDC* pDC, CBrush* pBrush, CPen& linePen, const CList<double, double>& angleList, 
		const CRect& pieRect, const CPoint& centerPoint)
{
	if(pBrush == NULL)
		return;

	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&pBrush[0]);
	CPen* pOldPen = (CPen*)pDC->SelectObject(&linePen);

	CPoint point4[4];
	POSITION anglePos = angleList.GetHeadPosition();
	double dPrevAngle = PI / 2;
	int nIndex = 0;
	while(anglePos != NULL)
	{
		int residual = GetBrushIndex(10, nIndex++);
		double dAngle = angleList.GetNext(anglePos);
		CPoint point[2];
		if(!GetPiePoints(pieRect, centerPoint, dPrevAngle, dAngle, point))
			break;

		double dCurTotalAngle = dPrevAngle + dAngle;
		if(point[0] == point[1] && dAngle < 0.5){
			dPrevAngle = dPrevAngle + dAngle;
			continue;
		}
				
		pDC->SelectObject(&pBrush[residual]);

		//sy 2002.11.14. -> 파이의 각도가 중심점의 위와 아래 일경우
		if(point[0].x != point[1].x){
			pDC->Pie(pieRect, point[0], point[1]);
		}
		else{
			// 현재 각이 너무 작을 경우에는 기본값보다 조금 크게해서 표현한다.
			// -> 이전 값도 그 표현 영역이 작을 경우가 겹칠수 있다.
			if(dAngle < 0.05){ 
				CPoint pt(point[0]);
				pt.x += 2;
				pDC->Pie(pieRect, pt, point[1]);
				//그냥 선으로 표현하는 경우
				//CDraw::DrawLine(pDC, CenterPiePoint, point[1]);
			}
			else
				pDC->Pie(pieRect, point[0], point[1]);
		}

		if(dAngle == PI*2)
			pDC->Ellipse(pieRect);

		dPrevAngle = dPrevAngle + dAngle;
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}

bool CPieDraw::DrawSolidPie(CDC* pDC, const CRect& graphDrawingRegion, const CGraphColor& graphColor, const CEtcGraphColor& etcGraphColor, const int dataStart, const int dataRealEnd, const double& dataViewMax, const double& dataViewMin, const CList<CPacket*, CPacket*>& packetList)
{
	// 1. Check and Get the Data and Legend Packet
	POSITION headPos = packetList.GetHeadPosition();
	if( headPos == NULL) return false;
	CPacket* textPacket = NULL;     // text packet
	CPacket* dataPacket = NULL;		// data packet	
	if(packetList.GetCount() >= 2)
	{
		textPacket = packetList.GetNext(headPos);
		dataPacket = packetList.GetNext(headPos);
	}
	else return false;
	CList<CString, CString>* pTextList = textPacket->GetstrDataList();
	CList<double, double>* pDataList = dataPacket->GetnumericDataList();
	if(pTextList == NULL || pDataList == NULL) return false;

	// 2. Check First Data
	POSITION textPosition = pTextList->FindIndex(dataStart);
	POSITION dataPosition = pDataList->FindIndex(dataStart);	
	if(textPosition == NULL || dataPosition == NULL) return false;

	// 3. Get the Common Head/Tail Space and Max Length and Count in Legend String.
	//그래프옆에 데이터를 표현하기 위해.
	//데이터의 최대 길이와 갯수를 가지고 와서 계산한다.
	int nMaxLabelLength = 0;
	int nMaxValueLength = 0;
	int nDataCount = textPacket->GetCount();
	CString strPacketType = dataPacket->GetType();
	int nLegendCount = 0;
	for(int i = 0; i < nDataCount; i++)
	{
		if(textPosition == NULL || dataPosition == NULL) break;
		CString Str = pTextList->GetNext(textPosition);
		double nowData = pDataList->GetNext(dataPosition);
		// 데이타가 없을때는 표현하지 않는다.
		if(nowData <= 0) continue;

		Str.TrimLeft(" ");
		Str.TrimRight(" ");
		int nLabelLength = pDC->GetTextExtent( Str).cx;
		if( nLabelLength > nMaxLabelLength) nMaxLabelLength = nLabelLength;

		Str = ":" + CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::DoubleToString( nowData), strPacketType);
		int nValueLength = pDC->GetTextExtent( Str).cx;
		if( nValueLength > nMaxValueLength) nMaxValueLength = nValueLength;

		nLegendCount++;
	}

	// (2006/10/13 - Seung-Won, Bae) Draw Legend Rectangle
	// modified by rich 02/10/15 세로텍스트 간격 2pixels 추가
	int tempHeight = pDC->GetTextExtent("0").cy + 2;
	CRect graphRegion = graphDrawingRegion;
	CRect rctLegend( 0, graphRegion.top, 0, graphRegion.top + tempHeight * nLegendCount + 6);
	int textColumn = 0;
	if( ( graphRegion.Height() - 6) / tempHeight > 0) textColumn = ( nLegendCount - 1) / ( ( graphRegion.Height() - 6) / tempHeight) + 1;
	else textColumn = nLegendCount;
	rctLegend.right = graphRegion.right;
	graphRegion.right = graphRegion.right - ( nMaxLabelLength + nMaxValueLength + 10) * textColumn;
	rctLegend.left = graphRegion.right - 3;
	if( textColumn > 1)
	{
		rctLegend.bottom = graphRegion.bottom;
		if( rctLegend.Height() < tempHeight + 6) rctLegend.bottom = rctLegend.top + tempHeight + 6;
	}

	CRect pieRect = graphDrawingRegion;
	pieRect.DeflateRect( pieRect.Width() / 20, pieRect.Height() / 20);
/*
	// modified by rich 02/10/15 파이위치 가운데 아래로
	CRect tempRect;
	tempRect.left = graphDrawingRegion.CenterPoint().x - pieRect.Width()/2;
	tempRect.top = graphDrawingRegion.CenterPoint().y - pieRect.Height();
	tempRect.right = graphDrawingRegion.CenterPoint().x + pieRect.Width()/2;
	tempRect.bottom = graphDrawingRegion.CenterPoint().y + pieRect.Height();
	pieRect = tempRect;
*/

	pieRect.top += 15;
	CRect backPieRect = pieRect;
	backPieRect.OffsetRect(0, -15);
	CPoint downCenterPoint(pieRect.left + pieRect.Width() / 2, pieRect.top + pieRect.Height() / 2);
	CPoint upCenterPoint(backPieRect.left + backPieRect.Width() / 2, backPieRect.top + backPieRect.Height() / 2);
	CList<double, double> angleList;
	GetPieAllAngles(pDataList, angleList);

	// ?. Make Brush
	CBrush brush[10];
	brush[0].CreateSolidBrush(graphColor.GetColor1());
	brush[1].CreateSolidBrush(graphColor.GetColor2());
	brush[2].CreateSolidBrush(graphColor.GetColor3());
	brush[3].CreateSolidBrush(graphColor.GetColor4());
	brush[4].CreateSolidBrush(graphColor.GetColor5());
	brush[5].CreateSolidBrush(graphColor.GetColor6());
	brush[6].CreateSolidBrush(etcGraphColor.GetGraphColor1());
	brush[7].CreateSolidBrush(etcGraphColor.GetGraphColor2());
	brush[8].CreateSolidBrush(etcGraphColor.GetGraphColor3());
	brush[9].CreateSolidBrush(etcGraphColor.GetGraphColor4());

	// ?. Make Pie Pen.
	int lineThickness = 1;
	CPen linePen;
	linePen.CreatePen(PS_SOLID, lineThickness, RGB(0,0,0));

	// 바닥판 그리는부분
	DrawPie(pDC, brush, linePen, angleList, pieRect, downCenterPoint);

	// ?. Make Pen.
	CPen pen[10];
	pen[0].CreatePen(PS_SOLID, 1, graphColor.GetColor1());
	pen[1].CreatePen(PS_SOLID, 1, graphColor.GetColor2());
	pen[2].CreatePen(PS_SOLID, 1, graphColor.GetColor3());
	pen[3].CreatePen(PS_SOLID, 1, graphColor.GetColor4());
	pen[4].CreatePen(PS_SOLID, 1, graphColor.GetColor5());
	pen[5].CreatePen(PS_SOLID, 1, graphColor.GetColor6());
	pen[6].CreatePen(PS_SOLID, 1, etcGraphColor.GetGraphColor1());
	pen[7].CreatePen(PS_SOLID, 1, etcGraphColor.GetGraphColor2());
	pen[8].CreatePen(PS_SOLID, 1, etcGraphColor.GetGraphColor3());
	pen[9].CreatePen(PS_SOLID, 1, etcGraphColor.GetGraphColor4());

	// 양쪽 기둥 그리기.
	double dPrevAngle = PI / 2;
	bool bIsLeftPole = true, bIsRightPole = true;
	int nIndex = 0;
	POSITION anglePos = angleList.GetHeadPosition();
	while(anglePos != NULL && (bIsLeftPole || bIsRightPole))
	{
		int residual = GetBrushIndex(10, nIndex++);
		double dAngle = angleList.GetNext(anglePos);
		double dCurTotalAngle = dPrevAngle + dAngle;
		if(dAngle == 0 || !IsDrawLiftRight(dCurTotalAngle, bIsLeftPole, bIsRightPole)){
			dPrevAngle = dCurTotalAngle;
			continue;
		}

		// 왼쪽 사각 그리는 부분 -> 180도 이상일때
		if(bIsLeftPole && dCurTotalAngle >= PI){
			DrawSidePole(pDC, &brush[residual], &pen[residual], &linePen, 
				pieRect, upCenterPoint, downCenterPoint, true);
			bIsLeftPole = false;
		}
		// 오른쪽 사각 그리는 부분 -> 360도 이상일때
		if(bIsRightPole && dCurTotalAngle >= PI*2){
			DrawSidePole(pDC, &brush[residual], &pen[residual], &linePen, 
				pieRect, upCenterPoint, downCenterPoint, false);
			bIsRightPole = false;
		}
		dPrevAngle = dCurTotalAngle;
	}

	// 가운데 기둥 그리기.
	// "왼쪽->가운데",  "오른쪽->가운데"두 형태로 그린다. --> 기둥 겹침을 방지위함.
	// "왼쪽->가운데" (180도 ~ 270도)
	CPoint point4[4];
	anglePos = angleList.GetHeadPosition();
	dPrevAngle = PI / 2;
	nIndex = -1;
	while(anglePos != NULL)
	{
		nIndex++;
		double dAngle = angleList.GetNext(anglePos);
		double dCurTotalAngle = dPrevAngle + dAngle;
		if(!GetPillarPoints(pieRect, upCenterPoint, downCenterPoint, dCurTotalAngle, point4))
			break;

		// 현재 각도가 90도 미만, 270도 초과 -> 그리지 않는다.
		if(dCurTotalAngle < PI/2 || dCurTotalAngle > PI/2*3)
			break;

		// 현재 각도가 180도 ~ 270도
		if(InBetweenAngle(PI, PI/2*3, dCurTotalAngle))
		{
			int residual = GetBrushIndex(10, nIndex++) + 1;
			if(residual > 9)
				residual = 0;
			DrawCenterPole(pDC, &brush[residual], &pen[residual], &linePen, point4);
		}
	
		dPrevAngle = dCurTotalAngle;
	}

	// "오른쪽->가운데" (270도 ~ 360도)
	anglePos = angleList.GetTailPosition();
	dPrevAngle = PI/2*5;
	nIndex = angleList.GetCount();
	while(anglePos != NULL)
	{
		nIndex--;
		double dAngle = angleList.GetPrev(anglePos);
		double dCurTotalAngle = dPrevAngle - dAngle;
		if(!GetPillarPoints(pieRect, upCenterPoint, downCenterPoint, dCurTotalAngle, point4))
			break;

		// 현재 각도가 270도 이하 -> 그리지 않는다.
		if(dCurTotalAngle <= PI/2*3)
			break;

		// 현재 각도가 270도 ~ 360도
		// break 로 하면 다음에 오는 angle이 만약 270 ~ 360 일 경우 표현을 못한다.
		if(InBetweenAngle(PI/2*3, PI*2, dCurTotalAngle))
		{
			int residual = GetBrushIndex(10, nIndex) - 1;
			if(residual < 0)
				residual = 9;
			DrawCenterPole(pDC, &brush[residual], &pen[residual], &linePen, point4);
		}
		dPrevAngle = dCurTotalAngle;
	}

	// 상판 그리는 부분
	pieRect.OffsetRect(0, -15);
	DrawPie(pDC, brush, linePen, angleList, pieRect, upCenterPoint);

	// (2006/10/13 - Seung-Won, Bae) Draw Legend Rectangle
	CPen* pOldPen = (CPen*)pDC->SelectObject(&pen[0]);
	pDC->Rectangle( rctLegend);
	
	pDC->SelectObject(&pen[0]);
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&brush[0]);

	// (2006/10/13 - Seung-Won, Bae) Draw Legend after Pie.
	//out
	int tempCount = 0;
	int textCount = 0;
	int column = 0;
	textPosition = pTextList->FindIndex(dataStart);
	dataPosition = pDataList->FindIndex(dataStart);
	int MODE = pDC->SetBkMode(TRANSPARENT);
	COLORREF clr = pDC->GetTextColor();
	for(int i = dataStart; i <= dataRealEnd; i++, tempCount++)
	{
		if(textPosition == NULL || dataPosition == NULL)
			break;
		double nowData = pDataList->GetNext(dataPosition);
		CString Str = pTextList->GetNext(textPosition);
		// 데이타가 없을때는 표현하지 않는다.
		if(nowData <= 0) continue;

		int residual = 0;
		int brushNumber = 10;
		residual = tempCount - (int(tempCount / brushNumber) * brushNumber);
		pDC->SelectObject(&brush[residual]);
		pDC->SelectObject(&pen[residual]);

		//sy 2002.10.24
		CRect rect(graphRegion.right, graphRegion.top, graphRegion.right, graphRegion.top);
		rect.InflateRect(0, 0, (nMaxLabelLength + nMaxValueLength + MARGIN), tempHeight);

		// text 위치가 영역을 벗어날 경우 처리
		int nGap_Y = tempHeight*textCount +MARGIN;
		if(rect.top < graphDrawingRegion.top)
			nGap_Y += (graphDrawingRegion.top - rect.top);
		rect.OffsetRect( ( nMaxLabelLength + nMaxValueLength + 10) * column, nGap_Y);
		//end

		// modified by rich 02/10/15 텍스트색 검정으로
		pDC->SetTextColor( RGB(0,0,0));

		pDC->FillRect(CRect(rect.left, rect.top, rect.left +5, rect.bottom),&brush[residual]);//ETO_CLIPPED

		Str.TrimLeft(" ");
		Str.TrimRight(" ");
		pDC->ExtTextOut(rect.left+6, rect.top, NULL/*ETO_CLIPPED*/, CRect(rect.left+6, rect.top, rect.right, rect.bottom), Str, NULL);

		Str = ":" + CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::DoubleToString(nowData), strPacketType);
		pDC->ExtTextOut(rect.left+6 + nMaxLabelLength, rect.top, NULL/*ETO_CLIPPED*/, CRect(rect.left+6  + nMaxLabelLength + nMaxValueLength, rect.top, rect.right, rect.bottom), Str, NULL);

		textCount++;
		if( rctLegend.Height() - 6 < tempHeight * ( textCount + 1))
		{
			textCount =0;
			column++;
		}
	}

	pDC->SelectObject(pOldPen);
	linePen.DeleteObject();
	pDC->SelectObject(pOldBrush);

	for(int k = 0; k < 10; k++)
	{
		brush[k].DeleteObject();
		pen[k].DeleteObject();
	}
	
	return true;
}

void CPieDraw::DrawText(CDC* pDC, CBrush* pBrush, const CRect& graphRegion, const CString& strPacketType, CList<CString, CString>* pTextList, CList<double, double>* pDataList)
{
	if(pBrush == NULL || pTextList == NULL || pDataList == NULL)
		return;

	CList<CString, CString> dataList;
	if(!GetDataList(strPacketType, pDataList, dataList))
		return;
	CList<CString, CString> titleList;
	GetDataListInCheckLength(pTextList, GetMaxLength(*pTextList), true, titleList);

	CSize size = pDC->GetTextExtent("0");
	int nPos_x = graphRegion.right - GetTextWidth(size, titleList, dataList);
	int nPos_y = graphRegion.top + size.cy;
	if(!IsDrawingText(graphRegion, nPos_x))
		return;
}

void CPieDraw::DrawSidePole(CDC* pDC, CBrush* pBrush, CPen* pPen, CPen* pLinePen, const CRect& pieRect, 
		const CPoint& upCenterPoint, const CPoint& downCenterPoint, const bool bIsLeft)
{
	CRect rect(upCenterPoint, downCenterPoint);
	int nHalf = CMath::Round((double)pieRect.Width()/2.0);
	rect.NormalizeRect();

	if(bIsLeft)
		rect.InflateRect(nHalf, 0, 0, 1);
	else
		rect.InflateRect(0, 0, nHalf, 1);

	CBrush *pOrgBrush = pDC->SelectObject(pBrush);
	CPen *pOrgPen = pDC->SelectObject( pPen);
	pDC->Rectangle(rect);

	pDC->SelectObject(pLinePen);
	if(bIsLeft)
		CDraw::DrawLine(pDC, rect.TopLeft(), CPoint(rect.left, rect.bottom));
	else
		CDraw::DrawLine(pDC, CPoint(rect.right, rect.top), rect.BottomRight());

	pDC->SelectObject( pOrgBrush);
	pDC->SelectObject( pOrgPen);
}

void CPieDraw::DrawCenterPole(CDC* pDC, CBrush* pBrush, CPen* pPen, CPen* pLinePen, const CPoint points[4])
{
	if(abs(points[0].x - points[1].x) <= 1)
		return;

	CBrush *pBrushOrg = pDC->SelectObject( pBrush);
	CPen *pPenOrg = pDC->SelectObject( pPen);
	pDC->Polygon(const_cast<CPoint*> (points), 4);

	pDC->SelectObject(pLinePen);
	CDraw::DrawLine(pDC, points[1], points[2]);

	pDC->SelectObject( pBrushOrg);
	pDC->SelectObject( pPenOrg);
}

// ----------------------------------------------------------------------------
int CPieDraw::GetBrushIndex(const int nBrushCount, const int nDataIndex) const
{
	return (nDataIndex - (nDataIndex +1) / nBrushCount * nBrushCount);
}

bool CPieDraw::GetPiePoints(const CRect& pieRect, const CPoint& centerPoint, 
		const double& dPrevAngle, const double& dAngle, CPoint points[2]) const
{
	if(dAngle < 0)
		return false;

	int nRX = pieRect.Width() / 2;
	int nRY = pieRect.Height() / 2;
	int x = centerPoint.x + (int)(nRX * cos(dPrevAngle));
	int y = centerPoint.y - (int)(nRY * sin(dPrevAngle));
	int x1 = centerPoint.x + (int)(nRX * cos(dPrevAngle + dAngle));
	int y1 = centerPoint.y - (int)(nRY * sin(dPrevAngle + dAngle));

	points[0] = CPoint(x, y);
	points[1] = CPoint(x1, y1);
	return true;
}

bool CPieDraw::GetPillarPoints(const CRect& pieRect, const CPoint& upCenterPoint, 
		const CPoint& downCenterPoint, const double& dAngle, CPoint points[4]) const
{
	if(dAngle < 0)
		return false;

	int nRX = pieRect.Width() / 2;
	int nRY = pieRect.Height() / 2;
	int x = upCenterPoint.x + (int)(nRX * cos(dAngle));
	int y = upCenterPoint.y - (int)(nRY * sin(dAngle));
	int x1 = downCenterPoint.x + (int)(nRX * cos(dAngle));
	int y1 = downCenterPoint.y - (int)(nRY * sin(dAngle));

	points[0] = upCenterPoint;
	points[1] = CPoint(x, y);
	points[2] = CPoint(x1, y1);
	points[3] = downCenterPoint;
	return true;
}

bool CPieDraw::GetPieAllAngles(CList<double, double>* pDataList, CList<double, double>& angleList) const
{
	angleList.RemoveAll();
	if(pDataList == NULL)
		return false;

	double dDataSum = 0.0;
	POSITION pos = pDataList->GetHeadPosition();
	while(pos != NULL){
		double dData = pDataList->GetNext(pos);
		if(dData < 0)
			break;
		dDataSum += dData;	
	}

	pos = pDataList->GetHeadPosition();
	while(pos != NULL){
		double dData = pDataList->GetNext(pos);
		if(dData < 0)
			break;

		double dAngle = (dData * PI * 2) / dDataSum;
		angleList.AddTail(dAngle);
	}

	return (angleList.GetCount() > 0);
}

bool CPieDraw::GetDataList(const CString& strPacketType, CList<double, double>* pDataList, CList<CString, CString>& dataList) const
{
	dataList.RemoveAll();
	if(pDataList == NULL || pDataList->GetCount() <= 0)
		return false;

	int nMaxLength = 0;
	CList<CString, CString> tempList;
	POSITION pos = pDataList->GetHeadPosition();
	while(pos != NULL)
	{
		CString strData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::DoubleToString(pDataList->GetNext(pos)), strPacketType);
		tempList.AddTail(strData);
		int nLength = strData.GetLength();
		if(nLength > nMaxLength)
			nMaxLength = nLength;
	}

	GetDataListInCheckLength(&tempList, nMaxLength, false, dataList);

	return (dataList.GetCount() > 0);
}

void CPieDraw::GetDataListInCheckLength(CList<CString, CString>* pOrgDataList, const int nMaxLength, const bool bRight, CList<CString, CString>& dataList) const
{
	if(nMaxLength <= 0 || pOrgDataList == NULL || pOrgDataList->GetCount() <= 0)
		return;

	dataList.RemoveAll();
	POSITION pos = pOrgDataList->GetHeadPosition();
	while(pos != NULL)
	{
		CString strData = pOrgDataList->GetNext(pos);
		strData.TrimRight(" ");
		for(int i = strData.GetLength(); i < nMaxLength; i++)
		{
			if(bRight)
				strData = strData + " ";
			else
				strData = " " + strData;
		}
		dataList.AddTail(strData);
	}
}

int CPieDraw::GetMaxLength(const CList<CString, CString>& dataList) const
{
	int nMaxLength = 0;
	POSITION pos = dataList.GetHeadPosition();
	while(pos != NULL)
	{
		CString strData = dataList.GetNext(pos);
		strData.TrimRight(" ");
		int nLength = strData.GetLength();
		if(nLength > nMaxLength)
			nMaxLength = nLength;
	}

	return nMaxLength;
}

int CPieDraw::GetTextWidth(const CSize& size, const CList<CString, CString>& titleList, const CList<CString, CString>& dataList) const
{
	int nTotalLength = 0;
	if(titleList.GetHeadPosition() != NULL)
		nTotalLength += titleList.GetHead().GetLength();
	if(dataList.GetHeadPosition() != NULL)
		nTotalLength += dataList.GetHead().GetLength();

	int nWidth = size.cy * (nTotalLength + 3);
	nWidth += (MARGIN * 2);
	return nWidth;
}

// ----------------------------------------------------------------------------
bool CPieDraw::IsDrawLiftRight(const double& dCurAngle, const bool bIsLeftPole, const bool bIsRightPole) const
{
	if(!bIsLeftPole && !bIsRightPole)
		return false;
	return (dCurAngle >= PI && (dCurAngle <= PI*2 || bIsRightPole));
}

bool CPieDraw::InBetweenAngle(const double& dAngle1, const double& dAngle2, const double& dCurAngle) const
{
	return (dCurAngle >= dAngle1 && dCurAngle <= dAngle2);
}

bool CPieDraw::IsDrawingText(const CRect& graphRegion, const int nTextLeftPt) const
{
	if(nTextLeftPt <= 0)
		return false;

	if(graphRegion.left > nTextLeftPt || graphRegion.right < nTextLeftPt)
		return false;

	int nCentrePt_x = graphRegion.CenterPoint().x;
	return ((nCentrePt_x + (graphRegion.Width()/4)) >= nTextLeftPt);
}
