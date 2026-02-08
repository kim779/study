// SpecialDraw.cpp: implementation of the CSpecialDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SpecialDraw.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "DataMath.h"
#include "Conversion.h"
#include "BaseData.h"
#include "PacketPointerList.h"
#include "PacketList.h"
#include "PacketBase.h"
#include "IndicatorList.h"
#include "IndicatorInfo.h"
#include "XScaleManager.h"
#include "resource.h"										// for IDS_SMOOTHING_PERIOD

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpecialDraw::CSpecialDraw()
{
	m_eGraphType = CGraphBaseData::Special_Type;
}

CSpecialDraw::~CSpecialDraw()
{

}

bool CSpecialDraw::DrawGraph(CDC* pDC,
			const CRect& realRegion, 
			const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData,
			const CSubGraphData& subGraphData)
{
	const CList< CPacket *, CPacket *> &packetList = drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return false;
	const CGraphColor& graphColor = subGraphData.GetColor();
//	int nRealStart = displayAttributes.GetDataStartInBlock();
//	int nRealEnd = displayAttributes.GetDataEndInBlock();
	double dViewMax = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(); 
	double dViewMin = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin();
	const CPenThickness& penThickness = subGraphData.GetPenThickness();
	bool bIsInvert = drawingGraphData.DoesVertScaleInvertType();

	switch(subGraphData.GetStyle().GetGraphSpecialStyle()){
	case CGraphBaseData::ThreeLineBreak:
		return DrawThreeLineBreak(pDC, realRegion, graphColor, packetList, penThickness, drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(), drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin());
	case CGraphBaseData::InverseLine:
		return DrawInverseLine(pDC, realRegion, drawingGraphData.GetIndicatorInfo(),  
			drawingGraphData.GetEtcGraphColor(), graphColor, dViewMax, dViewMin, 
			drawingGraphData.GetDisplayDataHorzMinMax().GetDisplayMax(), drawingGraphData.GetDisplayDataHorzMinMax().GetDisplayMin(), 
			packetList, penThickness, bIsInvert, drawingGraphData.GetOcxHwnd());
	case CGraphBaseData::PAndF:
		return DrawPF(pDC, realRegion, drawingGraphData.GetIndicatorInfo(), graphColor, 
			dViewMin, dViewMax, packetList, drawingGraphData.GetOcxHwnd());
	case CGraphBaseData::Kagi:
		return DrawKagi(pDC, realRegion, graphColor, packetList, drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(), drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin());
	case CGraphBaseData::Renko:
		return CRenkoDraw().DrawRenko(pDC, realRegion, graphColor, packetList, drawingGraphData.GetIndicatorInfo(), 
			dViewMin, dViewMax, displayAttributes, drawingGraphData.GetOcxHwnd());
// (2006.11.09 안승환) 분산차트 추가
	case CGraphBaseData::DisperSion:
		return DrawDisperSion(pDC, realRegion, graphColor, packetList, drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(), drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin());
// (2006.11.09 안승환) 분산차트 추가
		
	}
	return false;
}

bool CSpecialDraw::DrawGraph(CDC* pDC, const CRect& realRegion, const CDisplayAttributes& displayAttributes, 
		const CDrawingGraphData& drawingGraphData, const CSubGraphData& subGraphData,
		const CString& strRQ, CXScaleManager* pxScaleManager)
{
	const CList< CPacket *, CPacket *> &packetList = drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return false;
	const CGraphColor& graphColor = subGraphData.GetColor();
	double dViewMax = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(); 
	double dViewMin = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin();
	const CPenThickness& penThickness = subGraphData.GetPenThickness();
	bool bIsInvert = drawingGraphData.DoesVertScaleInvertType();

	if(pxScaleManager->GetThreadFlag() == THREAD_CALCULATION)
		return true;

	switch(subGraphData.GetStyle().GetGraphSpecialStyle()){
	case CGraphBaseData::ThreeLineBreak:
		return DrawThreeLineBreak(pDC, realRegion, graphColor, packetList, penThickness,
			strRQ, pxScaleManager, drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(),
			drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin(), drawingGraphData.GetColumnIndex());
	case CGraphBaseData::PAndF:
		return DrawPF(pDC, realRegion, drawingGraphData.GetIndicatorInfo(), graphColor, 
			dViewMin, dViewMax, packetList,
			strRQ, pxScaleManager, drawingGraphData.GetOcxHwnd(), drawingGraphData.GetColumnIndex());
	case CGraphBaseData::Kagi:
		return DrawKagi(pDC, realRegion, graphColor, packetList,
			strRQ, pxScaleManager, drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(), drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin(), drawingGraphData.GetColumnIndex());
	case CGraphBaseData::Renko:
		return CRenkoDraw().DrawRenko(pDC, realRegion, graphColor, packetList, drawingGraphData.GetIndicatorInfo(), 
			dViewMin, dViewMax, displayAttributes,
			strRQ, pxScaleManager, drawingGraphData.GetOcxHwnd(), drawingGraphData.GetColumnIndex());
	}
	return false;
}
bool CSpecialDraw::DrawInverseLine(CDC* pDC, const CRect& graphRegion, 
		CIndicatorInfo* pIndicatorInfo, const CEtcGraphColor& etcGraphColor, const CGraphColor& graphColor, 
		const double& dViewMax, const double& dViewMin, const double& dViewXMax, const double& dViewXMin, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, const bool bIsInvert,
		HWND p_hOcxWnd)
{
	POSITION Pos = packetList.GetHeadPosition();
	//date packet ===============================
	POSITION datePos = packetList.GetTailPosition();
	CPacket* pDatePacket = NULL;
	CPacket* pPricePacket = NULL;
	CPacket* pVolumePacket = NULL;
	if(datePos != NULL) 
		pDatePacket = packetList.GetPrev(datePos);
	if(datePos != NULL){
		if(pDatePacket->GetName() == _MTEXT( C0_DATE_TIME) || pDatePacket->GetName() == _MTEXT( C0_DATA_TIME)){
			CPacket* pCheckPacket = packetList.GetPrev(datePos);
			if(datePos != NULL && pCheckPacket->GetName() == _MTEXT( C0_DATE_TIME))
				pDatePacket = pCheckPacket;
		}
		else
			pDatePacket = NULL;
	}

	if(Pos == NULL)	return false;
	else{
		pPricePacket = packetList.GetNext(Pos);			// 종가 packet
		if(Pos == NULL)	return false;
		else
			pVolumePacket = packetList.GetNext(Pos);	//거래량 packet
	}
	
	CList<double, double>* pPriceList = pPricePacket->GetnumericDataList();
	CList<double, double>* pVolumeList = pVolumePacket->GetnumericDataList();
	if( !pPriceList || !pVolumeList) return false;

	int dataCount = pPricePacket->GetCount();
	if(dataCount > pVolumePacket->GetCount())
		dataCount = pVolumePacket->GetCount();
	int nStartIndex = pPricePacket->GetStartPeriod();

	POSITION posPrice = pPriceList->FindIndex( nStartIndex);
	POSITION posVolume = pVolumeList->FindIndex( nStartIndex);
	if( posPrice == NULL || posVolume == NULL) return false;

	_MHWND( p_hOcxWnd);
	ML_SET_LANGUAGE_RES();
	CString baseStr;
	baseStr.LoadString( IDS_SMOOTHING_PERIOD);
	CList<double, double>* pConditionList = NULL;
	if( pIndicatorInfo != NULL)
	{
		pConditionList = pIndicatorInfo->GetCommonCalConditionList();
		if(pConditionList != NULL && pConditionList->GetCount() > 0)
			baseStr += CDataConversion::DoubleToString(pConditionList->GetHead());
	}

	double volumnMin = 0.0, volumnMax = 0.0, dPriceMin = 0.0, dPriceMax = 0.0;
	dPriceMax = dViewMax;
	dPriceMin = dViewMin;
	volumnMax = dViewXMax;
	volumnMin = dViewXMin;

	double nAcos = 0.0;

	CPen	penTitle;
	COLORREF clrTitle = ( graphColor.GetColor1() + graphColor.GetColor2()) / 2;
	penTitle.CreatePen( PS_SOLID, 1, clrTitle);

	// (2007/5/3 - Seung-Won, Bae) Support 8 Range Color.
	CPen	penStart;
	CPen	penEnd;
	CPen	penTurnUp;
	CPen	penBuy;
	CPen	penKeepBuy;
	CPen	penHoldBuy;
	CPen	penTurnDown;
	CPen	penSell;
	CPen	penKeepSell;
	CPen	penHoldSell;
	// penThickness 추가
	penStart.CreatePen(		PS_SOLID, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
	penEnd.CreatePen(		PS_SOLID, penThickness.GetDrawingThickness1(), graphColor.GetColor2());
	penTurnUp.CreatePen(	PS_SOLID, penThickness.GetDrawingThickness1(), graphColor.GetColor3());
	penBuy.CreatePen(		PS_SOLID, penThickness.GetDrawingThickness1(), graphColor.GetColor4());
	penKeepBuy.CreatePen(	PS_SOLID, penThickness.GetDrawingThickness1(), graphColor.GetColor5());
	penHoldBuy.CreatePen(	PS_SOLID, penThickness.GetDrawingThickness1(), graphColor.GetColor6());
	double dColor = 0;
	POSITION posColor = NULL;
	pConditionList = NULL;
	if( pIndicatorInfo != NULL) pConditionList = pIndicatorInfo->GetDrawConditionList();
	posColor = ( pConditionList ? pConditionList->FindIndex( 0) : NULL);
	if( posColor) dColor = pConditionList->GetAt( posColor);	else dColor = 0;
	penTurnDown.CreatePen(	PS_SOLID, penThickness.GetDrawingThickness1(), ( COLORREF)dColor);
	posColor = ( pConditionList ? pConditionList->FindIndex( 1) : NULL);
	if( posColor) dColor = pConditionList->GetAt( posColor);	else dColor = 0;
	penSell.CreatePen(		PS_SOLID, penThickness.GetDrawingThickness1(), ( COLORREF)dColor);
	posColor = ( pConditionList ? pConditionList->FindIndex( 2) : NULL);
	if( posColor) dColor = pConditionList->GetAt( posColor);	else dColor = 0;
	penKeepSell.CreatePen(	PS_SOLID, penThickness.GetDrawingThickness1(), ( COLORREF)dColor);
	posColor = ( pConditionList ? pConditionList->FindIndex( 3) : NULL);
	if( posColor) dColor = pConditionList->GetAt( posColor);	else dColor = 0;
	penHoldSell.CreatePen(	PS_SOLID, penThickness.GetDrawingThickness1(), ( COLORREF)dColor);
	
	CBrush brStart( graphColor.GetColor1()), brEnd( graphColor.GetColor2());
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&brStart);

	CPen* pOldPen = (CPen*)pDC->SelectObject(&penStart);	

	CPen *pNextPen = &penStart;
	double dBasePrice = 0.0, dBaseVolume = 0.0, dNextPrice = 0.0, dNextVolume = 0.0;
	double dVectorX = 0.0, dVectorY = 0.0, dVectorSize = 0.0, dVectorAngle = 0.0, dVectorUnitAngle = 0.0;
	int nVectorUnitCount = 0;
	CPoint pointNext, pointBase;
	for( int i = nStartIndex; i < dataCount; i++)
	{
		if(posPrice == NULL || posVolume == NULL) break;

		if( i == nStartIndex)
		{
			dNextPrice = pPriceList->GetNext( posPrice);
			dNextVolume = pVolumeList->GetNext(posVolume);
			pointNext = CDataConversion::GetVerticalPoint(graphRegion, dViewMin, dViewMax, volumnMin, volumnMax, dNextPrice, dNextVolume);
			pointBase = pointNext;
			dBasePrice = dNextPrice;
			dBaseVolume = dNextVolume;
		}
		else
		{
			pointBase = pointNext;
			dBasePrice = dNextPrice;
			dBaseVolume = dNextVolume;

			dNextPrice = pPriceList->GetNext( posPrice);
			dNextVolume = pVolumeList->GetNext(posVolume);
			pointNext = CDataConversion::GetVerticalPoint(graphRegion, dViewMin, dViewMax, volumnMin, volumnMax, dNextPrice, dNextVolume);
		}
		
		if( volumnMax - volumnMin != 0.0)	dVectorX = ( dNextVolume - dBaseVolume) * 10000 / ( volumnMax - volumnMin);
		else								dVectorX = 0.0;
		if( dPriceMax - dPriceMin != 0.0)		dVectorY = ( dNextPrice - dBasePrice) * 10000 / ( dPriceMax - dPriceMin);
		else								dVectorY = 0.0;
		dVectorSize = sqrt( dVectorX * dVectorX + dVectorY * dVectorY);
		if( dVectorSize != 0.0) 
		{
			dVectorAngle = acos( dVectorX / dVectorSize);
			if( dVectorY < 0) dVectorAngle = -dVectorAngle;
			dVectorUnitAngle = dVectorAngle;
			if( dVectorAngle < 0) dVectorUnitAngle = -dVectorUnitAngle;
			nVectorUnitCount = int( dVectorUnitAngle / ( 2 * PI));
			if( 0 < nVectorUnitCount) dVectorUnitAngle -= nVectorUnitCount * ( 2 * PI);
			if( dVectorAngle < 0) dVectorUnitAngle = 2 * PI - dVectorUnitAngle;

			if(			dVectorUnitAngle < ( 2 * PI) / 16 * 1)	pNextPen = &penTurnUp;
			else if(	dVectorUnitAngle < ( 2 * PI) / 16 * 3)	pNextPen = &penBuy;
			else if(	dVectorUnitAngle < ( 2 * PI) / 16 * 5)	pNextPen = &penKeepBuy;
			else if(	dVectorUnitAngle < ( 2 * PI) / 16 * 7)	pNextPen = &penHoldBuy;
			else if(	dVectorUnitAngle < ( 2 * PI) / 16 * 9)	pNextPen = &penTurnDown;
			else if(	dVectorUnitAngle < ( 2 * PI) / 16 * 11)	pNextPen = &penSell;
			else if(	dVectorUnitAngle < ( 2 * PI) / 16 * 13)	pNextPen = &penKeepSell;
			else if(	dVectorUnitAngle < ( 2 * PI) / 16 * 15)	pNextPen = &penHoldSell;
			else											pNextPen = &penTurnUp;
		}
		pDC->SelectObject( pNextPen);
		
		//line 을 그린다.==========================
		if( nStartIndex < i)
		{
			if(bIsInvert)
			{
				pointNext.y = CDataConversion::InvertGraphDataPt(graphRegion, pointNext.y);
				pointBase.y = CDataConversion::InvertGraphDataPt(graphRegion, pointBase.y);
			}

			CDraw::DrawLine(pDC, pointBase, pointNext);
			if( i == nStartIndex + 1)
			{
				pDC->SelectObject(&brStart);
				pDC->SelectObject(&penStart);
				CDraw::DrawRectangle( pDC, CRect( pointBase.x - 4, pointBase.y - 4, pointBase.x + 4, pointBase.y + 4));
			}
			if( i == dataCount - 1)
			{
				pDC->SelectObject(&brEnd);
				pDC->SelectObject(&penEnd);
				CDraw::DrawRectangle( pDC, CRect( pointNext.x - 4, pointNext.y - 4, pointNext.x + 4, pointNext.y + 4));
			}
		}
	}

	// for Legend
	penStart.DeleteObject();
	penEnd.DeleteObject();
	penStart.CreatePen(		PS_SOLID, 1, graphColor.GetColor1());
	penEnd.CreatePen(		PS_SOLID, 1, graphColor.GetColor2());

	// 날짜를 써주는 부분========================================================================================
	int nDate = 0;
	CString strData = "";
	if(pDatePacket != NULL && nStartIndex < dataCount)
	{
		nDate = (int)pDatePacket->GetnumericDataList()->GetAt(pDatePacket->GetnumericDataList()->FindIndex(pPricePacket->GetStartPeriod()));
		strData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::IntToString(nDate), pDatePacket->GetType(), "");
		strData += "  -      ";
		
		SIZE strDataSize = pDC->GetTextExtent(strData);
		SIZE tempSize;
		tempSize = strDataSize;
		SIZE rectangleSize;
		rectangleSize.cx = strDataSize.cy;// * 2/3;
		rectangleSize.cy = strDataSize.cy;// * 2/3;

		nDate = (int)pDatePacket->GetnumericDataList()->GetTail();
		strData += CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::IntToString(nDate), pDatePacket->GetType(), "");
		strData += "  ";
		strDataSize = pDC->GetTextExtent(strData);
	
		if((graphRegion.right - strDataSize.cx - rectangleSize.cx - 10) >= graphRegion.left)
		{
			pDC->SelectObject( &penTitle);
			pDC->SelectObject(GetStockObject(NULL_BRUSH));
			CDraw::DrawRectangle(pDC, CRect(graphRegion.right - strDataSize.cx - rectangleSize.cx - 20, graphRegion.top, graphRegion.right - 5, graphRegion.top + strDataSize.cy *2 + 7));

			int MODE = pDC->SetBkMode(TRANSPARENT);
			// modified by rich 02/10/02 텍스트 색 변경
			COLORREF clr = pDC->SetTextColor( clrTitle);
			pDC->TextOut(graphRegion.right - strDataSize.cx - rectangleSize.cx, graphRegion.top + MARGIN, strData);
			pDC->TextOut(graphRegion.right - strDataSize.cx - rectangleSize.cx, graphRegion.top + MARGIN + strDataSize.cy, baseStr);
			pDC->SetBkMode(MODE);
			pDC->SetTextColor(clr);

			// (2007/4/30 - Seung-Won, Bae) Fill the Start & End Dot.
			pDC->SelectObject( &brStart);
			pDC->SelectObject( &penStart);
			CDraw::DrawRectangle(pDC, CRect(graphRegion.right - strDataSize.cx - rectangleSize.cx * 2 - MARGIN, graphRegion.top + MARGIN, graphRegion.right - strDataSize.cx - rectangleSize.cx - MARGIN - 4, graphRegion.top + MARGIN + rectangleSize.cy - 4));

			pDC->SelectObject( &brEnd);
			pDC->SelectObject( &penEnd);
			CDraw::DrawRectangle(pDC, CRect(graphRegion.right - strDataSize.cx + tempSize.cx - rectangleSize.cx * 2 - MARGIN, graphRegion.top + MARGIN, graphRegion.right - strDataSize.cx - rectangleSize.cx + tempSize.cx- MARGIN - 4, graphRegion.top + MARGIN + rectangleSize.cy - 4));
		}
	}
	
	//==========================================================================================================
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	brStart.DeleteObject();
	brEnd.DeleteObject();
	penTitle.DeleteObject();

	penStart.DeleteObject();
	penEnd.DeleteObject();
	penTurnUp.DeleteObject();
	penBuy.DeleteObject();
	penKeepBuy.DeleteObject();
	penHoldBuy.DeleteObject();
	penTurnDown.DeleteObject();
	penSell.DeleteObject();
	penKeepSell.DeleteObject();
	penHoldSell.DeleteObject();

	return true;
}

bool CSpecialDraw::DrawThreeLineBreak(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness, const double& dViewMax, const double& dViewMin)
{
	//date packet ===============================
	POSITION datePos = packetList.GetTailPosition();
	CPacket* pDatePacket = NULL;
	if(datePos != NULL) 
		pDatePacket = packetList.GetPrev(datePos);
	if(datePos != NULL){
		if(pDatePacket->GetName() == _MTEXT( C0_DATE_TIME) || pDatePacket->GetName() == _MTEXT( C0_DATA_TIME)){
			CPacket* pCheckPacket = packetList.GetPrev(datePos);
			if(datePos != NULL && pCheckPacket->GetName() == _MTEXT( C0_DATE_TIME))
				pDatePacket = pCheckPacket;
		}
		else
			pDatePacket = NULL;
	}
	//============================================

	POSITION Pos = packetList.GetHeadPosition();
	if(!Pos)
		return false;
	CPacket* pOpenPacket = packetList.GetNext(Pos); 
	if(!Pos)
		return false;
	CPacket* pClosePacket = packetList.GetNext(Pos); 
	if( !pOpenPacket || !pClosePacket) return false;
	if( !pOpenPacket->GetnumericDataList() || !pClosePacket->GetnumericDataList()) return false;

	POSITION openPos = pOpenPacket->GetnumericDataList()->GetHeadPosition();
	POSITION closePos = pClosePacket->GetnumericDataList()->GetHeadPosition();

	if(openPos == NULL || closePos == NULL) 
		return false;

	double dataMin = 0.0, dataMax = 0.0;
	int dataCount = pOpenPacket->GetCount();
	double tempdataMin = 0.0, tempdataMax = 0.0;
	pOpenPacket->GetMinMax(0, dataCount, tempdataMin, tempdataMax);
	dataMin = tempdataMin; 
	dataMax = tempdataMax;
	pClosePacket->GetMinMax(0, dataCount, tempdataMin, tempdataMax);
	if(tempdataMin < dataMin)
		dataMin = tempdataMin;
	if(tempdataMax > dataMax)
		dataMax = tempdataMax;

	CBrush upBrush	(graphColor.GetColor2());
	CBrush downBrush(graphColor.GetColor4());
	CBrush Brush(RGB(255,255,255));
		
	CPen upPen;
	CPen downPen;
	// modified by rich 02/09/17
	// penThickness 추가
	upPen.CreatePen		(PS_SOLID, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
	downPen.CreatePen	(PS_SOLID, penThickness.GetDrawingThickness1(), graphColor.GetColor3());
		
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&upBrush);
	CPen* pOldPen = (CPen*)pDC->SelectObject(&upPen);

	double Width = double(graphRegion.Width()) / double(dataCount);
	if(Width <= 1.0)
		Width = 1.0;

	for(int i = 0 ; i < dataCount ; i++){
		if(!openPos || !closePos)
			break;
	
		double startData = pOpenPacket->GetnumericDataList()->GetNext(openPos);
		double endData = pClosePacket->GetnumericDataList()->GetNext(closePos);
		
		CPoint startDataPt = CDataConversion::GetVerticalPoint(graphRegion, i, Width, dViewMin, dViewMax, startData);
		CPoint endDataPt = CDataConversion::GetVerticalPoint(graphRegion, i, Width, dViewMin, dViewMax, endData);
		
		if(startData < endData){ // red
			pDC->SelectObject(&upBrush);
			pDC->SelectObject(&upPen);
			
			if(graphColor.GetUpGraphFill() == false)
				pDC->SelectObject(GetStockObject(NULL_BRUSH));

			if(Width < 2)
				CDraw::DrawLine(pDC, CPoint(startDataPt.x, startDataPt.y), CPoint(endDataPt.x, endDataPt.y ));						
			else{	
				if((startDataPt.y - endDataPt.y) <= 1)
					CDraw::DrawLine(pDC, CPoint(startDataPt.x - int(Width/2), startDataPt.y), CPoint(startDataPt.x + int(Width/2), startDataPt.y ));						
				else
					CDraw::DrawRectangle(pDC, CRect(startDataPt.x - int(Width/2), startDataPt.y, startDataPt.x + int(Width/2), endDataPt.y));						

			}
		}
		else{	//blue
			pDC->SelectObject(&downBrush);
			pDC->SelectObject(&downPen);

//	2006.09.07 삼선전환도 칸 전환수를 수정할수 있도록 함 
//			if(graphColor.GetUpGraphFill() == false)
			if(graphColor.GetDownGraphFill() == false)
				pDC->SelectObject(GetStockObject(NULL_BRUSH));
//	2006.09.07 삼선전환도 칸 전환수를 수정할수 있도록 함 

			if(Width < 2)
				CDraw::DrawLine(pDC, CPoint(startDataPt.x, startDataPt.y), CPoint(endDataPt.x, endDataPt.y ));						
			else{
				if((endDataPt.y - startDataPt.y) <= 1)
					CDraw::DrawLine(pDC, CPoint(endDataPt.x - int(Width/2), endDataPt.y), CPoint(endDataPt.x + int(Width/2), endDataPt.y ));						
				else
					CDraw::DrawRectangle(pDC, CRect(startDataPt.x - int(Width/2), startDataPt.y, endDataPt.x + int(Width/2), endDataPt.y ));						
			}
		}
	}

	pDC->SelectObject(pOldPen);	
	upPen.DeleteObject();
	upPen.CreatePen(PS_SOLID, 1, graphColor.GetColor1());

	// 날짜를 써주는 부분
	int nDate = 0;
	CString strData = "";
	if(pDatePacket != NULL){
		nDate = (int)pDatePacket->GetnumericDataList()->GetHead();
		strData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::IntToString(nDate), pDatePacket->GetType(), "");
		strData += "  -  ";
		nDate = (int)pDatePacket->GetnumericDataList()->GetTail();
		strData += CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::IntToString(nDate), pDatePacket->GetType(), "");
		strData += "  ";
		SIZE strDataSize = pDC->GetTextExtent(strData);
		if((graphRegion.right - strDataSize.cx - 10) >= graphRegion.left)
		{
			pDC->SelectObject(&upPen);
			//pDC->SelectObject(Brush);
			pDC->SelectObject(GetStockObject(NULL_BRUSH));
			CDraw::DrawRectangle(pDC, CRect(graphRegion.right - strDataSize.cx - 10, graphRegion.top, graphRegion.right - 5, graphRegion.top + strDataSize.cy + 7));

			int MODE = pDC->SetBkMode(TRANSPARENT);
			// modified by rich 02/10/02 텍스트 색 변경
			COLORREF clr = pDC->SetTextColor(graphColor.GetColor3());
			pDC->TextOut(graphRegion.right - strDataSize.cx, graphRegion.top + MARGIN, strData);
			pDC->SetBkMode(MODE);
			pDC->SetTextColor(clr);

		}
	}
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	upBrush.DeleteObject();
	downBrush.DeleteObject();
	Brush.DeleteObject();

	upPen.DeleteObject();
	downPen.DeleteObject();

	return true;
}

bool CSpecialDraw::DrawThreeLineBreak(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		const CList<CPacket*, CPacket*>& packetList, const CPenThickness& penThickness,
		const CString& strRQ, CXScaleManager* pxScaleManager, const double& dViewMax, const double& dViewMin, int p_nColumn)
{
	//date packet ===============================
	POSITION datePos = packetList.GetTailPosition();
	CPacket* pDatePacket = NULL;
	if(datePos != NULL) 
		pDatePacket = packetList.GetPrev(datePos);
	if(datePos != NULL){
		if(pDatePacket->GetName() == _MTEXT( C0_DATE_TIME) || pDatePacket->GetName() == _MTEXT( C0_DATA_TIME)){
			CPacket* pCheckPacket = packetList.GetPrev(datePos);
			if(datePos != NULL && pCheckPacket->GetName() == _MTEXT( C0_DATE_TIME))
				pDatePacket = pCheckPacket;
		}
		else
			pDatePacket = NULL;
	}
	//============================================

	POSITION Pos = packetList.GetHeadPosition();
	if(!Pos)
		return false;
	CPacket* pOpenPacket = packetList.GetNext(Pos); 
	if(!Pos)
		return false;
	CPacket* pClosePacket = packetList.GetNext(Pos); 
	if( !pOpenPacket || !pClosePacket) return false;
	if( !pOpenPacket->GetnumericDataList() || !pClosePacket->GetnumericDataList()) return false;

	int nDTI, nDTI_Start, nDTI_End;
	pxScaleManager->GetDrawIndex( strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
	if(nDTI_Start == -1) return true;

	POSITION openPos = pOpenPacket->GetnumericDataList()->FindIndex(nDTI_Start);
	POSITION closePos = pClosePacket->GetnumericDataList()->FindIndex(nDTI_Start);

	if(openPos == NULL || closePos == NULL) 
		return false;

	double dataMin = 0.0, dataMax = 0.0;
	int dataCount = pOpenPacket->GetCount();
	double tempdataMin = 0.0, tempdataMax = 0.0;
	pOpenPacket->GetMinMax( nDTI_Start, nDTI_End, tempdataMin, tempdataMax);
	dataMin = tempdataMin; 
	dataMax = tempdataMax;
	pClosePacket->GetMinMax( nDTI_Start, nDTI_End, tempdataMin, tempdataMax);
	if(tempdataMin < dataMin)
		dataMin = tempdataMin;
	if(tempdataMax > dataMax)
		dataMax = tempdataMax;

	CBrush upBrush	(graphColor.GetColor2());
	CBrush downBrush(graphColor.GetColor4());
	CBrush Brush(RGB(255,255,255));
		
	CPen upPen;
	CPen downPen;
	// modified by rich 02/09/17
	// penThickness 추가
	upPen.CreatePen		(PS_SOLID, penThickness.GetDrawingThickness1(), graphColor.GetColor1());
	downPen.CreatePen	(PS_SOLID, penThickness.GetDrawingThickness1(), graphColor.GetColor3());
		
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&upBrush);
	CPen* pOldPen = (CPen*)pDC->SelectObject(&upPen);

	double Width = double(graphRegion.Width()) / double(dataCount);
	if(Width <= 1.0)
		Width = 1.0;

	CXScaleManager::PAREA parea;

	// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
	//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
	int nZeroDTI = 0;
	CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
	if( !pblaAreafDTI) return false;
	CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
	pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

	for( nDTI = nDTI_Start; nDTI <= nDTI_End; nDTI++)
	{
		if(!openPos || !closePos)
			break;
	
		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->NextDrawArea(&parea);
		parea = pblaAreafDTI->GetNext( dblCursor);

		if( !parea) break;

		double startData = pOpenPacket->GetnumericDataList()->GetNext(openPos);
		double endData = pClosePacket->GetnumericDataList()->GetNext(closePos);
		
		CPoint startDataPt = CDataConversion::GetVerticalPoint(graphRegion, nDTI - nDTI_Start, Width, dViewMin, dViewMax, startData);
		CPoint endDataPt = CDataConversion::GetVerticalPoint(graphRegion, nDTI - nDTI_Start, Width, dViewMin, dViewMax, endData);
		
		if(startData < endData){ // red
			pDC->SelectObject(&upBrush);
			pDC->SelectObject(&upPen);
			
			if(graphColor.GetUpGraphFill() == false)
				pDC->SelectObject(GetStockObject(NULL_BRUSH));

			//if(Width < 2)
			//	CDraw::DrawLine(pDC, CPoint(startDataPt.x, startDataPt.y), CPoint(endDataPt.x, endDataPt.y ));						
			//else{	
			if((startDataPt.y - endDataPt.y) <= 1)
				CDraw::DrawLine(pDC, CPoint(parea->left, startDataPt.y), CPoint(parea->right, startDataPt.y ));
			else
				CDraw::DrawRectangle(pDC, CRect(parea->left, startDataPt.y, parea->right, endDataPt.y));				

			//}
		}
		else{	//blue
			pDC->SelectObject(&downBrush);
			pDC->SelectObject(&downPen);

//	2006.09.07 삼선전환도 칸 전환수를 수정할수 있도록 함 
//			if(graphColor.GetUpGraphFill() == false)
			if(graphColor.GetDownGraphFill() == false)
				pDC->SelectObject(GetStockObject(NULL_BRUSH));
//	2006.09.07 삼선전환도 칸 전환수를 수정할수 있도록 함 

			//if(Width < 2)
			//	CDraw::DrawLine(pDC, CPoint(startDataPt.x, startDataPt.y), CPoint(endDataPt.x, endDataPt.y ));						
			//else{
			if((endDataPt.y - startDataPt.y) <= 1)
				CDraw::DrawLine(pDC, CPoint(parea->left, endDataPt.y), CPoint(parea->right, endDataPt.y ));						
			else
				CDraw::DrawRectangle(pDC, CRect(parea->left, startDataPt.y, parea->right, endDataPt.y ));						
			//}
		}
	}

	pDC->SelectObject(pOldPen);	
	upPen.DeleteObject();
	upPen.CreatePen(PS_SOLID, 1, graphColor.GetColor1());

	// 날짜를 써주는 부분
	int nDate = 0;
	CString strData = "";
	if(pDatePacket != NULL){
		nDate = (int)pDatePacket->GetnumericDataList()->GetHead();
		strData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::IntToString(nDate), pDatePacket->GetType(), "");
		strData += "  -  ";
		nDate = (int)pDatePacket->GetnumericDataList()->GetTail();
		strData += CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::IntToString(nDate), pDatePacket->GetType(), "");
		strData += "  ";
		SIZE strDataSize = pDC->GetTextExtent(strData);
		if((graphRegion.right - strDataSize.cx - 10) >= graphRegion.left)
		{
			pDC->SelectObject(&upPen);
			//pDC->SelectObject(Brush);
			pDC->SelectObject(GetStockObject(NULL_BRUSH));
			CDraw::DrawRectangle(pDC, CRect(graphRegion.right - strDataSize.cx - 10, graphRegion.top, graphRegion.right - 5, graphRegion.top + strDataSize.cy + 7));

			int MODE = pDC->SetBkMode(TRANSPARENT);
			// modified by rich 02/10/02 텍스트 색 변경
			COLORREF clr = pDC->SetTextColor(graphColor.GetColor3());
			pDC->TextOut(graphRegion.right - strDataSize.cx, graphRegion.top + MARGIN, strData);
			pDC->SetBkMode(MODE);
			pDC->SetTextColor(clr);

		}
	}
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	upBrush.DeleteObject();
	downBrush.DeleteObject();
	Brush.DeleteObject();

	upPen.DeleteObject();
	downPen.DeleteObject();

	return true;
}

//sy 2003.6.11.
bool CSpecialDraw::DrawPF(CDC* pDC,	const CRect& graphRegion, CIndicatorInfo* pIndicatorInfo, const CGraphColor& graphColor, const double& dViewMin,
						  const double& dViewMax, const CList< CPacket *, CPacket *> &packetList, HWND p_hOcxWnd)
{
	CPAndFDraw PAndFDraw;
	CPacket* pDatePacket = NULL;
	CList<double, double>* pHighList = NULL, *pLowList = NULL;
	if( !PAndFDraw.GetPacketData( packetList, pDatePacket, pHighList, pLowList))
		return false;
	int count = pHighList->GetCount() ;

	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();
	double dUnitValue;
	double dUnitCount;
	POSITION pos = pConditionList->GetHeadPosition();
	while(pos) 
	{
		dUnitCount = pConditionList->GetNext(pos);
		if(pos !=NULL)
			dUnitValue = pConditionList->GetNext(pos);
	}
//	double dUnitValue = PAndFDraw.GetUnitValue(pPacketList->GetHead()->GetPacketList(), pIndicatorInfo);
	if(dUnitValue <= 0)
		return false;

	// P&F 그리기
	bool bResult = DrawPAndF(pDC, pHighList, pLowList, 
		graphRegion, graphColor, dViewMin, dViewMax, dUnitValue);

	// 기간 및 한칸값 쓰기
	if(bResult)
		// 20080910 JS.Kim		8진법/32진법 처리
		//PAndFDraw.DrawDate(pDC, pDatePacket, graphRegion, graphColor, dUnitValue,dUnitCount, p_hOcxWnd);
		PAndFDraw.DrawDate(pDC, pDatePacket, pDatePacket->GetPacketList()->GetBaseData( _MTEXT( C0_HIGH)), 
							graphRegion, graphColor, dUnitValue,dUnitCount, p_hOcxWnd);

	return bResult;
}

bool CSpecialDraw::DrawPAndF(CDC* pDC, const CList<double, double>* pHighList, const CList<double, double>* pLowList, 
		const CRect& graphRegion, const CGraphColor& graphColor, 
		const double& dViewMin, const double& dViewMax, const double& dUnitValue)
{
	if(pHighList == NULL || pLowList == NULL)
		return false;

	double dUnitHeight = CPAndFDraw().GetUintHeight(graphRegion, dViewMin, dViewMax, dUnitValue);

	CPen upPen(PS_SOLID, 1, graphColor.GetColor1());
	CPen downPen(PS_SOLID, 1, graphColor.GetColor3());
	CPen* pOldPen = (CPen*) pDC->SelectObject(&upPen);
	CBrush* pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH);

	int datacount = pHighList->GetCount();
	int nIndex = 0;
	POSITION highPos = pHighList->GetHeadPosition();
	POSITION lowPos = pLowList->GetHeadPosition();
	while(highPos != NULL && lowPos != NULL){
		double dHigh = pHighList->GetNext(highPos);
		double dLow = pLowList->GetNext(lowPos);
		int nUnitCount = CMath::Round(fabs(dLow - dHigh) / dUnitValue);

		if(dHigh < dLow){
			pDC->SelectObject(&upPen);
			CPAndFDraw().DrawOAndX(pDC, graphRegion, dViewMin, dViewMax, nIndex, dHigh, 
				nUnitCount, dUnitHeight, dUnitValue, true,datacount);
		}
		else if(dHigh > dLow){
			pDC->SelectObject(&downPen);
			CPAndFDraw().DrawOAndX(pDC, graphRegion, dViewMin, dViewMax, nIndex, dHigh, 
				nUnitCount, dUnitHeight, dUnitValue, false,datacount);
		}
		else
			continue;

		nIndex++;
	}

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	upPen.DeleteObject();
	downPen.DeleteObject();
	return true;
}

bool CSpecialDraw::DrawPF(CDC* pDC,	const CRect& graphRegion, CIndicatorInfo* pIndicatorInfo, const CGraphColor& graphColor, const double& dViewMin,
						  const double& dViewMax, const CList< CPacket *, CPacket *> &packetList,
						  const CString& strRQ, CXScaleManager* pxScaleManager, HWND p_hOcxWnd, int p_nColumn)
{
	CPAndFDraw PAndFDraw;
	CPacket* pDatePacket = NULL;
	CList<double, double>* pHighList = NULL, *pLowList = NULL;
	if( !PAndFDraw.GetPacketData( packetList, pDatePacket, pHighList, pLowList))
		return false;
	int count = pHighList->GetCount() ;

	CList<double, double>* pConditionList = pIndicatorInfo->GetCommonCalConditionList();
	double dUnitValue;
	double dUnitCount;
	POSITION pos = pConditionList->GetHeadPosition();
	while(pos) 
	{
		dUnitCount = pConditionList->GetNext(pos);
		if(pos !=NULL)
			dUnitValue = pConditionList->GetNext(pos);
	}
//	double dUnitValue = PAndFDraw.GetUnitValue(pPacketList->GetHead()->GetPacketList(), pIndicatorInfo);
	if(dUnitValue <= 0)
		return false;

	// P&F 그리기
	bool bResult = DrawPAndF(pDC, pHighList, pLowList, 
		graphRegion, graphColor, dViewMin, dViewMax, dUnitValue,
		strRQ, pxScaleManager, p_nColumn);

	// 기간 및 한칸값 쓰기
	if(bResult)
		// 20080910 JS.Kim		8진법/32진법 처리
		//PAndFDraw.DrawDate(pDC, pDatePacket, graphRegion, graphColor, dUnitValue,dUnitCount, p_hOcxWnd);
		PAndFDraw.DrawDate(pDC, pDatePacket, pDatePacket->GetPacketList()->GetBaseData( _MTEXT( C0_HIGH)), 
							graphRegion, graphColor, dUnitValue,dUnitCount, p_hOcxWnd);

	return bResult;
}

bool CSpecialDraw::DrawPAndF(CDC* pDC, const CList<double, double>* pHighList, const CList<double, double>* pLowList, 
		const CRect& graphRegion, const CGraphColor& graphColor, 
		const double& dViewMin, const double& dViewMax, const double& dUnitValue,
		const CString& strRQ, CXScaleManager* pxScaleManager, int p_nColumn)
{
	if(pHighList == NULL || pLowList == NULL)
		return false;

	double dUnitHeight = CPAndFDraw().GetUintHeight(graphRegion, dViewMin, dViewMax, dUnitValue);

	CPen upPen(PS_SOLID, 1, graphColor.GetColor1());
	CPen downPen(PS_SOLID, 1, graphColor.GetColor3());
	CPen* pOldPen = (CPen*) pDC->SelectObject(&upPen);
	CBrush* pOldBrush = (CBrush*) pDC->SelectStockObject(NULL_BRUSH);

	int nDTI_Start, nDTI_End;
	pxScaleManager->GetDrawIndex( strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
	if(nDTI_Start == -1) return true;

	POSITION highPos = pHighList->FindIndex( nDTI_Start);
	POSITION lowPos = pLowList->FindIndex( nDTI_Start);

	CXScaleManager::PAREA parea;

	// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
	//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
	int nZeroDTI = 0;
	CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
	if( !pblaAreafDTI) return false;
	CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
	pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);
	
	int datacount = nDTI_End + 1;
	int nIndex = nDTI_Start;
	while( highPos != NULL && lowPos != NULL && nIndex <= nDTI_End)
	{
		double dHigh = pHighList->GetNext(	highPos);
		double dLow = pLowList->GetNext(	lowPos);

		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->NextDrawArea(&parea);
		parea = pblaAreafDTI->GetNext( dblCursor);
		if( !parea) break;

		int nUnitCount = CMath::Round(fabs(dLow - dHigh) / dUnitValue);

		if(dHigh < dLow)
		{
			pDC->SelectObject(&upPen);
			CPAndFDraw().DrawOAndX(pDC, graphRegion, dViewMin, dViewMax, nIndex, dHigh, 
				nUnitCount, dUnitHeight, dUnitValue, true,datacount, parea->left, parea->right);
		}
		else if(dHigh > dLow)
		{
			pDC->SelectObject(&downPen);
			CPAndFDraw().DrawOAndX(pDC, graphRegion, dViewMin, dViewMax, nIndex, dHigh, 
				nUnitCount, dUnitHeight, dUnitValue, false,datacount, parea->left, parea->right);
		}
		else
			continue;
		nIndex++;
	}

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	upPen.DeleteObject();
	downPen.DeleteObject();
	return true;
}
//end sy 2003.6.11.

/*-----------------------------------------------------------------------------------------
 - Function    :  DrawKagi
 - Created at  :  2005-06-22   14:44
 - Author      :  최종찬
 - Parameters  :  pDC          - 지표를 그릴 DC
 -                graphRegion  - 지표를 그릴 영역
 -                graphColor   - 선색깔 정보를 가지고 있는 Object
 -                packetList   - Kagi차트에 필요한 패킷
 - Return Value:  bool - true: 그리기 성공, false: 그리기 실패.
 - Description :  Kagi차트를 그린다.
 - Note		   :  1. Kagi의 데이타값 라인을 '조정선'이라 부른다. 주석에 '조정선'이 나와도 당황하지 말것.
				  2. Kagi차트를 그리기 위해서는 CPoint값 세개가 필요하다. index순으로 ptTrendLine,
				     ptLast, ptNext가 있는데... PtTrendLine은 수직선(상승/하락선)을 그릴때
					 상승추세또는 하락추세로 변환시킬것인지를 검사하기 위한 Point이고, 
					 ptLast와 ptNext의 y값을 연결시키면 수직선이 된다. 그리고 ptNext는
					 조정선의 시작점으로 사용이 된다.
 -----------------------------------------------------------------------------------------*/
bool CSpecialDraw::DrawKagi(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		const CList<CPacket*, CPacket*>& packetList, const double& dViewMax, const double& dViewMin)
{
	// 카기차트의 패킷들은 0 - Kagi_가격, 1 - Kagi_자료일자 순으로 들어있다. (ChartFormulate.dll의 CKagiChart 클래스 참조)
	// GetHead()를 수행하면 Kagi_가격이 GetTail()을 수행하면 Kagi_자료일자를 
	// 구할수 있다.
	CPacket* pKagiPrice = packetList.GetHead();
	if(pKagiPrice == NULL)
		return false;
	CList<double, double>* pKagiPriceList = pKagiPrice->GetnumericDataList();
	if(pKagiPriceList == NULL)
		return false;

	// 데이타 갯수를 가져온다.
	const int nDataCount = pKagiPriceList->GetCount();
	if(nDataCount <= 0)
		return false;
	// 조정선 길이(단위 너비) = 그래프영역/데이타갯수
	double dUnitWidth = CMath::Round(double(graphRegion.Width()) / double(nDataCount));
	if(dUnitWidth < 1.0)
		dUnitWidth = 1.0;

	// 최대값과 최소값을 구한다.
	double dMin, dMax;
	pKagiPrice->GetMinMax(0, nDataCount, dMin, dMax);

	// 첫번째 색깔(상승추세 색)과 두번째 색깔(하락추세 색)로 Pen을 만든다.
	CPen penUpTrend;	// 상승추세를 그릴 펜
	CPen penDownTrend;	// 하락추세를 그릴 펜
	// 상승추세용 펜은 두꺼워야 한다.
	penUpTrend.CreatePen(PS_SOLID, 2, graphColor.GetColor1());
	penDownTrend.CreatePen(PS_SOLID, 1, graphColor.GetColor2());

	// OldPen을 기억해둬라! ㅋ~~
	CPen* pOldPen;

	// 최초 상승추세로 시작인지 하락추세로 시작인지 확인하여 초기 Pen을 결정한다.
	if(pKagiPriceList->GetAt(pKagiPriceList->FindIndex(0)) < pKagiPriceList->GetAt(pKagiPriceList->FindIndex(1)))	// 상승이면
		pOldPen = (CPen*)pDC->SelectObject(&penUpTrend);
	else	// 하락이면
		pOldPen = (CPen*)pDC->SelectObject(&penDownTrend);

	POSITION pos = pKagiPriceList->GetHeadPosition();

	// 일단초기 조정선만 그린다.
	double dStartData = pKagiPriceList->GetNext(pos);
	CPoint ptStart = CDataConversion::GetVerticalPoint(graphRegion, 0, dUnitWidth, dViewMin, dViewMax, dStartData, false);
	CDraw::DrawLine(pDC, ptStart, CPoint(ptStart.x + dUnitWidth, ptStart.y));
	
	double dPrevData = dStartData;						// 중심선값을 구하기 위한 이전값데이타.
	CPoint ptTrendLine(ptStart);						// 상승/하락 추세를 구분할 위치값용 변수
	CPoint ptLast(ptStart.x + dUnitWidth, ptStart.y); 	// 이전 조정선위치값을 기억한다.
	
	// 수직선을 그리고 난 다음 조정선을 그린다.
	for(int i = 1; i < nDataCount; i++)
	{
		// 조정선 값과 위치를 구해온다.
		double dData = pKagiPriceList->GetNext(pos);
		CPoint ptNext = CDataConversion::GetVerticalPoint(graphRegion, i, dUnitWidth, dViewMin, dViewMax, dData, false);

		// 중심선 값과 위치를 구해온다.
		double dCenterLine = (dData + dPrevData) / 2.0;
		CPoint ptCenterLine = CDataConversion::GetVerticalPoint(graphRegion, i, dUnitWidth, dViewMin, dViewMax, dCenterLine, false);
		
		// 수직선 그리기. (y축은 위쪽이 기준임을 명심하라!)
		if(ptNext.y < ptLast.y)
		{	// 상승선
			if( ptLast.y > ptTrendLine.y && ptTrendLine.y > ptNext.y)
			{	// 상승추세로 반전된다.
				// 이전 조정선까지는 기존의 Pen으로 계속그린다.
				CDraw::DrawLine(pDC, ptLast, CPoint(ptLast.x, ptTrendLine.y));

				// 중심선을 그린다.
				if(ptTrendLine.y < ptCenterLine.y)
					CDraw::DrawLine(pDC, CPoint(ptLast.x, ptCenterLine.y), CPoint(ptLast.x + dUnitWidth/3, ptCenterLine.y));	

				// 이전 조정선부터는 상승추세용 Pen으로 그린다.
				pDC->SelectObject(&penUpTrend);
				CDraw::DrawLine(pDC, CPoint(ptLast.x, ptTrendLine.y), CPoint(ptLast.x, ptNext.y));

				// 중심선이 추세반전된 이후에 위치되어 있는 넘이면 상승추세로 된 Pen으로 다시 그려준다.
				if(ptTrendLine.y >= ptCenterLine.y)
					CDraw::DrawLine(pDC, CPoint(ptLast.x, ptCenterLine.y), CPoint(ptLast.x + dUnitWidth/3, ptCenterLine.y));
			}
			else
			{	// 추세반전이 없다. 현재의 Pen으로 계속 그린다.
				CDraw::DrawLine(pDC, ptLast, CPoint(ptLast.x, ptNext.y));

				// 중심선을 그린다.
				CDraw::DrawLine(pDC, CPoint(ptLast.x, ptCenterLine.y), CPoint(ptLast.x + dUnitWidth/3, ptCenterLine.y));
			}
		}//end of if(ptNext.y ...
		else
		{	// 하락선
			if(ptLast.y < ptTrendLine.y && ptTrendLine.y < ptNext.y)
			{	// 하락추세로 반전된다.
				// 이전 조정선까지는 기존의 Pen으로 계속 그린다.
				CDraw::DrawLine(pDC, ptLast, CPoint(ptLast.x, ptTrendLine.y));
				
				// 중심선을 그린다.
				if(ptTrendLine.y > ptCenterLine.y)
					CDraw::DrawLine(pDC, CPoint(ptLast.x, ptCenterLine.y), CPoint(ptLast.x + dUnitWidth/3, ptCenterLine.y));

				// 이전 조정선부터는 하락추세용 Pen으로 그린다.
				pDC->SelectObject(&penDownTrend);
				CDraw::DrawLine(pDC, CPoint(ptLast.x, ptTrendLine.y), CPoint(ptLast.x, ptNext.y));

				// 중심선이 추세반전된 이후에 위치되어 있는 넘이라면 하락추세 Pen으로 다시 그려준다.
				if(ptTrendLine.y <= ptCenterLine.y)
					CDraw::DrawLine(pDC, CPoint(ptLast.x, ptCenterLine.y), CPoint(ptLast.x + dUnitWidth/3, ptCenterLine.y));
			}
			else
			{	// 추세반전이 없다. 현재의 Pen으로 계속 그린다.
				CDraw::DrawLine(pDC, ptLast, CPoint(ptLast.x, ptNext.y));

				// 중심선을 그린다.
				CDraw::DrawLine(pDC, CPoint(ptLast.x, ptCenterLine.y), CPoint(ptLast.x + dUnitWidth/3, ptCenterLine.y));
			}
		}//end of else

		// 조정선 그리기.
		CDraw::DrawLine(pDC, ptNext, CPoint(ptNext.x + dUnitWidth, ptNext.y));

		// 추세구분용Point값과 이전 조정선 위치값 저장.
		dPrevData = dData;
		ptTrendLine = ptLast;
		ptLast.x = ptNext.x + dUnitWidth;
		ptLast.y = ptNext.y;
	}//end of for(int i = 1...

	penUpTrend.DeleteObject();
	penDownTrend.DeleteObject();

	pDC->SelectObject(pOldPen);
	return true;
}

bool CSpecialDraw::DrawKagi(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		const CList<CPacket*, CPacket*>& packetList,
		const CString& strRQ, CXScaleManager* pxScaleManager, const double& dViewMax, const double& dViewMin, int p_nColumn)
{
	// 카기차트의 패킷들은 0 - Kagi_가격, 1 - Kagi_자료일자 순으로 들어있다. (ChartFormulate.dll의 CKagiChart 클래스 참조)
	// GetHead()를 수행하면 Kagi_가격이 GetTail()을 수행하면 Kagi_자료일자를 
	// 구할수 있다.
	CPacket* pKagiPrice = packetList.GetHead();
	CPacket* pKagiDate = packetList.GetTail();
	if(pKagiPrice == NULL)
		return false;
	CList<double, double>* pKagiPriceList = pKagiPrice->GetnumericDataList();
	CList<double, double>* pKagiDateList = pKagiDate->GetnumericDataList();
	if( pKagiPriceList == NULL || pKagiDateList == NULL)
		return false;

	int nDTI_Start, nDTI_End;
	pxScaleManager->GetDrawIndex( strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
	if(nDTI_Start == -1) return true;

	// 데이타 갯수를 가져온다.
	const int nDataCount = nDTI_End - nDTI_Start + 1;
	if(nDataCount <= 0)
		return false;

	// 조정선 길이(단위 너비) = 그래프영역/데이타갯수
	double dUnitWidth = CMath::Round(double(graphRegion.Width()) / double(nDataCount));
	if(dUnitWidth < 1.0)
		dUnitWidth = 1.0;

	// 최대값과 최소값을 구한다.
	double dMin, dMax;
	pKagiPrice->GetMinMax( nDTI_Start, nDTI_End, dMin, dMax);

	// 첫번째 색깔(상승추세 색)과 두번째 색깔(하락추세 색)로 Pen을 만든다.
	CPen penUpTrend;	// 상승추세를 그릴 펜
	CPen penDownTrend;	// 하락추세를 그릴 펜
	// 상승추세용 펜은 두꺼워야 한다.
	penUpTrend.CreatePen(PS_SOLID, 2, graphColor.GetColor1());
	penDownTrend.CreatePen(PS_SOLID, 1, graphColor.GetColor2());

	// OldPen을 기억해둬라! ㅋ~~
	CPen* pOldPen;

	// 최초 상승추세로 시작인지 하락추세로 시작인지 확인하여 초기 Pen을 결정한다.
	if(pKagiPriceList->GetAt(pKagiPriceList->FindIndex(0)) < pKagiPriceList->GetAt(pKagiPriceList->FindIndex(1)))	// 상승이면
		pOldPen = (CPen*)pDC->SelectObject(&penUpTrend);
	else	// 하락이면
		pOldPen = (CPen*)pDC->SelectObject(&penDownTrend);

	POSITION pos = pKagiPriceList->FindIndex( nDTI_Start);
	POSITION pos1 = pKagiDateList->FindIndex( nDTI_Start);
	if( !pos || !pos1) return true;

	CXScaleManager::PAREA parea;

	// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
	//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
	int nZeroDTI = 0;
	CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
	if( !pblaAreafDTI) return false;
	CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
	pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

	// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
	//	pxScaleManager->NextDrawArea(&parea);
	parea = pblaAreafDTI->GetNext( dblCursor);
	if(!parea) return true;

	// 일단초기 조정선만 그린다.
	double dStartData = pKagiPriceList->GetNext(pos);
	//double dStartDate = pKagiDateList->GetNext(pos1);
	CPoint ptStart = CDataConversion::GetVerticalPoint(graphRegion, 0, dUnitWidth, dViewMin, dViewMax, dStartData, false);
	ptStart.x = parea->left;
	CDraw::DrawLine(pDC, ptStart, CPoint(ptStart.x + dUnitWidth, ptStart.y));
	
	double dPrevData = dStartData;						// 중심선값을 구하기 위한 이전값데이타.
	CPoint ptTrendLine(ptStart);						// 상승/하락 추세를 구분할 위치값용 변수
	CPoint ptLast(ptStart.x + dUnitWidth, ptStart.y); 	// 이전 조정선위치값을 기억한다.
	
	// 수직선을 그리고 난 다음 조정선을 그린다.
	for( int i = nDTI_Start + 1; i <= nDTI_End; i++)
	{
		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->NextDrawArea(&parea);
		parea = pblaAreafDTI->GetNext( dblCursor);
		if(!parea) break;
		
		// 조정선 값과 위치를 구해온다.
		double dDate = pKagiDateList->GetNext(pos1);
		double dData = pKagiPriceList->GetNext(pos);
		CPoint ptNext = CDataConversion::GetVerticalPoint(graphRegion, i, dUnitWidth, dViewMin, dViewMax, dData, false);

		ptLast.x = parea->left;
		ptNext.x = parea->left;

		// 중심선 값과 위치를 구해온다.
		double dCenterLine = (dData + dPrevData) / 2.0;
		CPoint ptCenterLine = CDataConversion::GetVerticalPoint(graphRegion, i, dUnitWidth, dViewMin, dViewMax, dCenterLine, false);
		
		// 수직선 그리기. (y축은 위쪽이 기준임을 명심하라!)
		if(ptNext.y < ptLast.y)
		{	// 상승선
			if( ptLast.y > ptTrendLine.y && ptTrendLine.y > ptNext.y)
			{	// 상승추세로 반전된다.
				// 이전 조정선까지는 기존의 Pen으로 계속그린다.
				CDraw::DrawLine(pDC, ptLast, CPoint(ptLast.x, ptTrendLine.y));

				// 중심선을 그린다.
				if(ptTrendLine.y < ptCenterLine.y)
					CDraw::DrawLine(pDC, CPoint(ptLast.x, ptCenterLine.y), CPoint(ptLast.x + dUnitWidth/3, ptCenterLine.y));	

				// 이전 조정선부터는 상승추세용 Pen으로 그린다.
				pDC->SelectObject(&penUpTrend);
				CDraw::DrawLine(pDC, CPoint(ptLast.x, ptTrendLine.y), CPoint(ptLast.x, ptNext.y));

				// 중심선이 추세반전된 이후에 위치되어 있는 넘이면 상승추세로 된 Pen으로 다시 그려준다.
				if(ptTrendLine.y >= ptCenterLine.y)
					CDraw::DrawLine(pDC, CPoint(ptLast.x, ptCenterLine.y), CPoint(ptLast.x + dUnitWidth/3, ptCenterLine.y));
			}
			else
			{	// 추세반전이 없다. 현재의 Pen으로 계속 그린다.
				CDraw::DrawLine(pDC, ptLast, CPoint(ptLast.x, ptNext.y));

				// 중심선을 그린다.
				CDraw::DrawLine(pDC, CPoint(ptLast.x, ptCenterLine.y), CPoint(ptLast.x + dUnitWidth/3, ptCenterLine.y));
			}
		}//end of if(ptNext.y ...
		else
		{	// 하락선
			if(ptLast.y < ptTrendLine.y && ptTrendLine.y < ptNext.y)
			{	// 하락추세로 반전된다.
				// 이전 조정선까지는 기존의 Pen으로 계속 그린다.
				CDraw::DrawLine(pDC, ptLast, CPoint(ptLast.x, ptTrendLine.y));
				
				// 중심선을 그린다.
				if(ptTrendLine.y > ptCenterLine.y)
					CDraw::DrawLine(pDC, CPoint(ptLast.x, ptCenterLine.y), CPoint(ptLast.x + dUnitWidth/3, ptCenterLine.y));

				// 이전 조정선부터는 하락추세용 Pen으로 그린다.
				pDC->SelectObject(&penDownTrend);
				CDraw::DrawLine(pDC, CPoint(ptLast.x, ptTrendLine.y), CPoint(ptLast.x, ptNext.y));

				// 중심선이 추세반전된 이후에 위치되어 있는 넘이라면 하락추세 Pen으로 다시 그려준다.
				if(ptTrendLine.y <= ptCenterLine.y)
					CDraw::DrawLine(pDC, CPoint(ptLast.x, ptCenterLine.y), CPoint(ptLast.x + dUnitWidth/3, ptCenterLine.y));
			}
			else
			{	// 추세반전이 없다. 현재의 Pen으로 계속 그린다.
				CDraw::DrawLine(pDC, ptLast, CPoint(ptLast.x, ptNext.y));

				// 중심선을 그린다.
				CDraw::DrawLine(pDC, CPoint(ptLast.x, ptCenterLine.y), CPoint(ptLast.x + dUnitWidth/3, ptCenterLine.y));
			}
		}//end of else

		// 조정선 그리기.
		CDraw::DrawLine(pDC, ptNext, CPoint(ptNext.x + dUnitWidth, ptNext.y));

		// 추세구분용Point값과 이전 조정선 위치값 저장.
		dPrevData = dData;
		ptTrendLine = ptLast;
		ptLast.x = ptNext.x + dUnitWidth;
		ptLast.y = ptNext.y;
	}//end of for(int i = 1...

	pDC->SelectObject(pOldPen);

	penUpTrend.DeleteObject();
	penDownTrend.DeleteObject();

	return true;
}

// (2006.11.09 안승환) 분산차트 추가
bool CSpecialDraw::DrawDisperSion(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
				const CList<CPacket*, CPacket*>& packetList, const double& dViewMax, const double& dViewMin)
{
	
	POSITION Pos = packetList.GetHeadPosition();
	if(!Pos)
		return false;
	CPacket* pClosePacket = packetList.GetNext(Pos); 
	if(!Pos)
		return false;
	CPacket* pOpenPacket = packetList.GetNext(Pos); 
	if(!Pos)
		return false;
	CPacket* pVolumePacket = packetList.GetNext(Pos); 
	if( !pOpenPacket || !pClosePacket || !pVolumePacket) return false;
	if( !pOpenPacket->GetnumericDataList() || !pClosePacket->GetnumericDataList() || !pVolumePacket->GetnumericDataList())
		return false;

	POSITION openPos = pOpenPacket->GetnumericDataList()->GetHeadPosition();
	POSITION closePos = pClosePacket->GetnumericDataList()->GetHeadPosition();
	POSITION volumePos = pVolumePacket->GetnumericDataList()->GetHeadPosition();

	if(openPos == NULL || closePos == NULL || volumePos == NULL) 
		return false;
	// 데이타 갯수를 가져온다.
	const int nDataCount = pOpenPacket->GetCount();
	if(nDataCount <= 0)
		return false;

	// 최대값과 최소값을 구한다.
	double dMin, dMax,volumnMin = 0.0, volumnMax = 0.0;
	pClosePacket->GetMinMax(0, nDataCount, dMin, dMax);
	pVolumePacket->GetMinMax(0, nDataCount, volumnMin, volumnMax);

	// 첫번째 색깔(상승추세 색)과 두번째 색깔(하락추세 색)로 Pen을 만든다.
	CBrush	upBrush	(graphColor.GetColor1());
	CBrush	downBrush	(graphColor.GetColor3());

	CPen upPen;	// 상승추세를 그릴 펜
	CPen downPen;	// 하락추세를 그릴 펜
	// 상승추세용 펜은 두꺼워야 한다.
	upPen.CreatePen(PS_SOLID, 1, graphColor.GetColor1());
	downPen.CreatePen(PS_SOLID, 1, graphColor.GetColor3());

	CPen* pOldPen = (CPen*)pDC->SelectObject(&upPen);
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&upBrush);
	
	CPoint startPt;
	double	dOpenPrice,dClosePrice;

	for(int i = 0 ; i < nDataCount ; i++)
	{
		if(!openPos || !closePos || !volumePos)
			break;
		dOpenPrice = pOpenPacket->GetnumericDataList()->GetNext(openPos);
		dClosePrice = pClosePacket->GetnumericDataList()->GetNext(closePos);
		startPt = CDataConversion::GetVerticalPoint(graphRegion, dViewMin, dViewMax, volumnMin, 
			volumnMax, dClosePrice, pVolumePacket->GetnumericDataList()->GetNext(volumePos));

		if(dOpenPrice < dClosePrice)// Red
		{ 
			pDC->SelectObject(&upPen);
			pDC->SelectObject(&upBrush);

			pDC->Ellipse(startPt.x,startPt.y,startPt.x+4,startPt.y+4);
//			pDC->Ellipse(startPt.x,startPt.y,5,5);
		}
		else	//Blue
		{
			pDC->SelectObject(&downPen);
			pDC->SelectObject(&downBrush);
			pDC->Ellipse(startPt.x,startPt.y,startPt.x+4,startPt.y+4);
		}

	}
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	upPen.DeleteObject();
	downPen.DeleteObject();
	upBrush.DeleteObject();
	downBrush.DeleteObject();

	return true;
	
}
// (2006.11.09 안승환) 분산차트 추가

///////////////////////////////////////////////////////////////////////////////
// class CPAndFDraw - sy 2003.6.11.

// P&F의 그리기 위해 필요한 packet을 가져오기
bool CPAndFDraw::GetPacketData( const CList< CPacket *, CPacket *> &packetList, CPacket*& pDatePacket, CList<double, double>*& pHighList,
							   CList<double, double>*& pLowList) const
{
	if( packetList.GetCount() < 2)
		return false;

	POSITION pos = packetList.GetHeadPosition();
	while(pos != NULL){
		CPacket* pPacket = packetList.GetNext(pos);
		if(pPacket == NULL)
			return false;

		if(pPacket->GetName().Find("PF_") == -1)
			return false;

		if(pPacket->GetName() == _MTEXT( C0_PF_DATE_TIME)){
			pDatePacket = pPacket->GetPacketList()->GetBaseData( _MTEXT( C0_DATE_TIME));
			continue;
		}

		if(pHighList == NULL){
			pHighList = pPacket->GetnumericDataList();
		}
		else if(pLowList == NULL){
			pLowList = pPacket->GetnumericDataList();
		}
	}

	return (pHighList != NULL && pLowList != NULL);
}

// P&F의 한칸값
double CPAndFDraw::GetUnitValue(CPacketList* pPacketList, CIndicatorInfo* pIndicatorInfo) const
{
	if(pPacketList == NULL)
		return 0.0;

	double dMin = 0.0, dMax = 0.0;
	GetPacketMinMax(pPacketList->GetBaseData( _MTEXT( C0_CLOSE)), dMin, dMax);

	// 첫번째 조건값 -> value
	// 두번째 조건값 -> 계산 방법(%, 수치)
	double dCond0 = 0.0, dCond1 = 0.0;
	if(!GetConditionValue(pIndicatorInfo, 0, dCond0) ||
		!GetConditionValue(pIndicatorInfo, 1, dCond1))
		return (dMax - dMin) / 50.0;

	if(dCond1 == 1)		//퍼센트
		return ((dMax - dMin) * (dCond0 / 100));
	else if(dCond1 == 2)//단순 수치
		return dCond0;

	return (dMax - dMin) / 50.0;
}

// P&F 한칸의 높이
double CPAndFDraw::GetUintHeight(const CRect& region, 
		const double& dViewMin, const double& dViewMax, const double& dUnitValue) const
{
	if(dViewMax == dViewMin)
		return false;

	double dUnitHeight = (region.Height()  * dUnitValue) / (dViewMax - dViewMin);
	if(dUnitHeight < 1)
		dUnitHeight = 1.0;

	return dUnitHeight;
}

// ----------------------------------------------------------------------------
// O, X 그리기
void CPAndFDraw::DrawOAndX(CDC* pDC, const CRect& graphRegion, 
		const double& dMin, const double& dMax, const int nIndex, const double& dData, 
		const int nUnitCount, const double& dUnitHeight, const double& dUnitValue, const bool bIsX,const int nDataCount)
{
// 2006.08.24 화면 싸이즈 조절시에 Drawing루틴 수정
	int nUnitWidth = CMath::Round(graphRegion.Width() / (nDataCount+1));
//	int nUnitWidth1 = CMath::Round(dUnitHeight / 2.0);
	if(nUnitWidth <= 1)
		nUnitWidth = 1;
	CPoint prePt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, nUnitWidth, dMin, dMax, dData);
//	CPoint prePt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dUnitHeight, dMin, dMax, dData);

	for(int i = 0 ; i < nUnitCount ; i++)
	{
		CPoint drawStartPt(prePt.x , prePt.y);
//		CPoint drawStartPt(prePt.x - nUnitWidth, prePt.y);
		CPoint dataPt(0, 0);
		if(bIsX){
			dataPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, nUnitWidth, dMin, dMax, dData + dUnitValue * (i + 1));
//			dataPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dUnitHeight, dMin, dMax, dData + dUnitValue * (i + 1));
			DrawX(pDC, drawStartPt, CPoint(dataPt.x + nUnitWidth, dataPt.y));
		}
		else{
			dataPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, nUnitWidth, dMin, dMax, dData - dUnitValue * (i + 1));
//			dataPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dUnitHeight, dMin, dMax, dData - dUnitValue * (i + 1));
			pDC->Ellipse(drawStartPt.x, drawStartPt.y, dataPt.x + nUnitWidth, dataPt.y);
		}
		prePt = dataPt;
	}
}

void CPAndFDraw::DrawOAndX(CDC* pDC, const CRect& graphRegion, 
		const double& dMin, const double& dMax, const int nIndex, const double& dData, 
		const int nUnitCount, const double& dUnitHeight, const double& dUnitValue, const bool bIsX,const int nDataCount,
		int nLeft, int nRight)
{
// 2006.08.24 화면 싸이즈 조절시에 Drawing루틴 수정
	int nUnitWidth = CMath::Round(graphRegion.Width() / (nDataCount+1));
//	int nUnitWidth1 = CMath::Round(dUnitHeight / 2.0);
	if(nUnitWidth <= 1)
		nUnitWidth = 1;
	CPoint prePt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, nUnitWidth, dMin, dMax, dData);
//	CPoint prePt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dUnitHeight, dMin, dMax, dData);

	for(int i = 0 ; i < nUnitCount ; i++)
	{
		CPoint drawStartPt(prePt.x , prePt.y);
//		CPoint drawStartPt(prePt.x - nUnitWidth, prePt.y);
		CPoint dataPt(0, 0);
		if(bIsX){
			dataPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, nUnitWidth, dMin, dMax, dData + dUnitValue * (i + 1));
//			dataPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dUnitHeight, dMin, dMax, dData + dUnitValue * (i + 1));
			//DrawX(pDC, drawStartPt, CPoint(dataPt.x + nUnitWidth, dataPt.y));
			DrawX(pDC, CPoint(nLeft, prePt.y), CPoint(nRight, dataPt.y));
		}
		else{
			dataPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, nUnitWidth, dMin, dMax, dData - dUnitValue * (i + 1));
//			dataPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex, dUnitHeight, dMin, dMax, dData - dUnitValue * (i + 1));
			//pDC->Ellipse(drawStartPt.x, drawStartPt.y, dataPt.x + nUnitWidth, dataPt.y);
			pDC->Ellipse(nLeft, drawStartPt.y, nRight, dataPt.y);
		}
		prePt = dataPt;
	}
}

// P&F의 Text(날짜) 쓰기
void CPAndFDraw::DrawDate(CDC* pDC, CPacket* pDatePacket, CPacket* pHighPacket, const CRect& graphRegion, 
		const CGraphColor& graphColor, const double& dUnitValue,const double& dUnitCount, HWND p_hOcxWnd)
{
	CString strText;
	CList<CString, CString> strTextList;
	// 20080910 JS.Kim		8진법/32진법 처리
	//if(!GetText(pDatePacket, dUnitValue,dUnitCount, strTextList, p_hOcxWnd))
	if(!GetText(pDatePacket, pHighPacket, dUnitValue,dUnitCount, strTextList, p_hOcxWnd))
		return;

	CRect textRegion = GetTextRegion(pDC, graphRegion, strTextList);
	// (2004.06.28, 배승원) P&F Info를 출력할 영역이 Graph Region의 반이상을 차지하는 경우 출력하지 않는다.
	if(textRegion.IsRectEmpty() || graphRegion.Width() < textRegion.Width() * 2 || graphRegion.Height() < textRegion.Height() * 2)
		return;

	// 테두리 그리기
	DrawTextOutline(pDC, textRegion, ((strTextList.GetCount() -1) * MARGIN), graphColor.GetColor1());

	// Text 쓰기
	CDraw::TextOut_MultiLine(pDC, textRegion.TopLeft(), MARGIN, strTextList, graphColor.GetColor3());
}


// private ====================================================================
bool CPAndFDraw::GetPacketMinMax(CPacket* pPacket, double& dMin, double& dMax) const
{
	if(pPacket == NULL)
		return false;

	return (pPacket->GetMinMax(0, pPacket->GetCount() -1, dMin, dMax) >= 0);
}

bool CPAndFDraw::GetConditionValue(CIndicatorInfo* pIndicatorInfo, const int nIndex, double& dValue) const
{
	if(pIndicatorInfo == NULL)
		return false;

	return GetListValue(pIndicatorInfo->GetCommonCalConditionList(), nIndex, dValue);
}

bool CPAndFDraw::GetListValue(CList<double, double>* pList, const int nIndex, double& dValue) const
{
	if(pList == NULL)
		return false;

	POSITION pos = pList->FindIndex(nIndex);
	if(pos == NULL)
		return false;

	dValue = pList->GetAt(pos);
	return true;
}

CRect CPAndFDraw::GetTextRegion(CDC* pDC, const CRect& graphRegion, const CList<CString, CString>& strTextList) const
{
	CSize size = GetTextSize(pDC, strTextList);
	if(size.cx > graphRegion.Width() || size.cy > graphRegion.Height())
		return CRect(0, 0, 0, 0);

	CRect textRegion(CPoint(graphRegion.right - size.cx, graphRegion.top), size);
	textRegion.OffsetRect(-MARGIN *2, MARGIN *2);
	return textRegion;
}

CSize CPAndFDraw::GetTextSize(CDC* pDC, const CList<CString, CString>& strTextList) const
{
	CSize totalSize(0, 0);
	POSITION pos = strTextList.GetHeadPosition();
	while(pos != NULL){
		CString strText = strTextList.GetNext(pos);
		if(strText.IsEmpty())
			continue;

		CSize size = pDC->GetTextExtent(strText);
		if(totalSize.cx < size.cx)
			totalSize.cx = size.cx;
		totalSize.cy += size.cy;
	}

	return totalSize;
}

// ----------------------------------------------------------------------------
bool CPAndFDraw::GetText(CPacket* pDatePacket, CPacket* pHighPacket, const double& dUnitValue, const double& dUnitCount,CList<CString, CString>& strTextList, HWND p_hOcxWnd) const
{
	strTextList.RemoveAll();

	// 시작날짜 - 끝날짜
	CString strDate = GetDateText(pDatePacket);
	if(strDate.IsEmpty())
		return false;
	strTextList.AddTail(strDate);

	// 한칸값
	_MHWND( p_hOcxWnd);
	ML_SET_LANGUAGE_RES();
	CString strUnitValue;
	strUnitValue.LoadString( IDS_VALUE_OF_A_BLOCK);
	// 20080910 JS.Kim		8진법/32진법 처리
	//strUnitValue += CDataConversion::DoubleToString(dUnitValue, pDatePacket->GetType());
	strUnitValue += CDataConversion::DoubleToString(dUnitValue, pHighPacket->GetType());
	strTextList.AddTail( strUnitValue);

	// 3칸 전환
	strUnitValue = CDataConversion::DoubleToString( dUnitCount);
	CString strText;
	strText.LoadString( IDS_BLOCK_COUNT_FOR_DIVERSION);
	strUnitValue += strText;
	strTextList.AddTail( strUnitValue);

	return (strTextList.GetCount() > 0);
}

// 기간
CString CPAndFDraw::GetDateText(CPacket* pDatePacket) const
{
	if(pDatePacket == NULL || pDatePacket->GetnumericDataList() == NULL)
		return "";

	// (2008/10/12 - Seung-Won, Bae) Check Empty List
	CList < double, double> *pDateList = pDatePacket->GetnumericDataList();
	if( pDateList->IsEmpty()) return "";

	// 시작날짜 - 끝날짜
	int nDate = (int)pDateList->GetHead();
	CString strDate = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::IntToString(nDate), pDatePacket->GetType(), "");
	strDate += "  -  ";
	nDate = (int)pDateList->GetTail();
	strDate += CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::IntToString(nDate), pDatePacket->GetType(), "");
	return strDate;
}

void CPAndFDraw::DrawX(CDC* pDC, const CPoint& pt1, const CPoint& pt2)
{
	CRect region(pt1, pt2);
	region.NormalizeRect();
	
	// X 의 그리는 방향이 틀리면 안된다.
	CDraw::DrawLine(pDC, CPoint(region.left, region.bottom), 
		CPoint(region.right, region.top));
	CDraw::DrawLine(pDC, CPoint(region.right, region.bottom), 
		CPoint(region.left, region.top));
}

void CPAndFDraw::DrawTextOutline(CDC* pDC, const CRect& textRegion, const int nLineGap, const COLORREF& penColor)
{
	CRect outlineRegion = textRegion;
	outlineRegion.bottom += nLineGap;
	outlineRegion.InflateRect(MARGIN, MARGIN);
	// 테두리 그리기
	CDraw::DrawRectangle(pDC, outlineRegion, penColor);
}


///////////////////////////////////////////////////////////////////////////////
// class CRenkoDraw

// Function : A DrawRenKo Function of The Special-Chart.
// Date     : 2005/06/24
// Made By  : Yoo Je Sun

bool CRenkoDraw::DrawRenko(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		 const CList<CPacket*, CPacket*>& packetList, const CIndicatorInfo* pIndicatorInfo, 
		 const double& dDataMin, const double& dDataMax, const CDisplayAttributes& displayAttributes, HWND p_hOcxWnd)
{
	CList<double, double>* pHighDataSave = NULL;
	CList<double, double>* pLowDataSave = NULL;
	CList<double, double>* pTypeDataSave = NULL;
	if(!GetDataList(packetList, pHighDataSave, pLowDataSave, pTypeDataSave)) 
		return false;

	// 블럭을 그릴 Pen과 Brush를 선택한다.
	CBrush brushArray[GDI_COUNT];
	CBrush* pOldBrush = pDC->GetCurrentBrush();
	CPen penArray[GDI_COUNT];
	CPen* pOldPen = pDC->GetCurrentPen();
	CreateGdiObject(brushArray, penArray, graphColor);

	// 기타 그리기 위해 필요한 속성. -> 한 block의 폭, x좌표의 시작위치 등.
	double dRealWidth = 1;
	int nPrevPt_x(0), nStartIndex(0), nEndIndex(0);
	GetEctEnvironment(graphRegion, displayAttributes, dRealWidth, nPrevPt_x, nStartIndex, nEndIndex);

	// 그릴 시작 위치.
	POSITION highDataPos = pHighDataSave->FindIndex(nStartIndex);
	POSITION lowDataPos = pLowDataSave->FindIndex(nStartIndex);
	POSITION typeDataPos = pTypeDataSave->FindIndex(nStartIndex);
	for(int nIndex = nStartIndex; nIndex <= nEndIndex; nIndex++)
	{
		if(highDataPos == NULL || lowDataPos == NULL)
			break;

		// Block Type에 따라 pen/brust 선택 (0:Down, 1:Up)
		SelectGdiObject(pDC, GetBlockType(pTypeDataSave, typeDataPos), brushArray, penArray);

		// Block Size
		double dHighData = pHighDataSave->GetNext(highDataPos);
		double dLowData = pLowDataSave->GetNext(lowDataPos);
		CPoint curHighPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex + 1, dRealWidth, dDataMin, dDataMax, dHighData, false);
		CPoint curLowPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex + 1, dRealWidth, dDataMin, dDataMax, dLowData, false);

		// Block Draw
		CDraw::DrawRectangle(pDC, CRect(nPrevPt_x, curHighPt.y, curLowPt.x, curLowPt.y));

		// Prev X-Point를 저장
		nPrevPt_x = curLowPt.x; 
	}

	// 그리기 도구 모두 초기화.
	DestroyGdiObject(pDC, brushArray, penArray, pOldBrush, pOldPen);

	// Text 그리기
	DrawDate(pDC, packetList.GetTail(), packetList.GetHead(), pIndicatorInfo, graphRegion, graphColor, p_hOcxWnd);
	return true;
}

bool CRenkoDraw::DrawRenko(CDC* pDC, const CRect& graphRegion, const CGraphColor& graphColor, 
		 const CList<CPacket*, CPacket*>& packetList, const CIndicatorInfo* pIndicatorInfo, 
		 const double& dDataMin, const double& dDataMax, const CDisplayAttributes& displayAttributes,
		 const CString& strRQ, CXScaleManager* pxScaleManager, HWND p_hOcxWnd, int p_nColumn)
{
	CList<double, double>* pHighDataSave = NULL;
	CList<double, double>* pLowDataSave = NULL;
	CList<double, double>* pTypeDataSave = NULL;
	if(!GetDataList(packetList, pHighDataSave, pLowDataSave, pTypeDataSave)) 
		return false;

	// 블럭을 그릴 Pen과 Brush를 선택한다.
	CBrush brushArray[GDI_COUNT];
	CBrush* pOldBrush = pDC->GetCurrentBrush();
	CPen penArray[GDI_COUNT];
	CPen* pOldPen = pDC->GetCurrentPen();
	CreateGdiObject(brushArray, penArray, graphColor);

	// 기타 그리기 위해 필요한 속성. -> 한 block의 폭, x좌표의 시작위치 등.
	double dRealWidth = 1;
	int nPrevPt_x(0), nStartIndex(0), nEndIndex(0);
	GetEctEnvironment(graphRegion, displayAttributes, dRealWidth, nPrevPt_x, nStartIndex, nEndIndex);

	int nDTI_Start, nDTI_End;
	pxScaleManager->GetDrawIndex( strRQ, FALSE, FALSE, nDTI_Start, nDTI_End);
	if(nDTI_Start == -1) return true;

	// 그릴 시작 위치.
	POSITION highDataPos = pHighDataSave->FindIndex(nDTI_Start);
	POSITION lowDataPos = pLowDataSave->FindIndex(nDTI_Start);
	POSITION typeDataPos = pTypeDataSave->FindIndex(nDTI_Start);

	CXScaleManager::PAREA parea;

	// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
	//	pxScaleManager->SetCurDrawInfo((CString&)strRQ, nDTI_Start);
	int nZeroDTI = 0;
	CDataBlockList< CXScaleManager::AREA> *pblaAreafDTI = pxScaleManager->GetDTIArea( p_nColumn, strRQ, nZeroDTI);
	if( !pblaAreafDTI) return false;
	CDataBlockList< CXScaleManager::AREA>::DblCursor dblCursor;
	pblaAreafDTI->GetCursor( nDTI_Start, nZeroDTI, dblCursor);

	for( int nIndex = nDTI_Start; nIndex <= nDTI_End; nIndex++)
	{
		if(highDataPos == NULL || lowDataPos == NULL)
			break;

		// (2009/1/10 - Seung-Won, Bae) Do not support draw info for multi-thread drawing.
		//	pxScaleManager->NextDrawArea(&parea);
		parea = pblaAreafDTI->GetNext( dblCursor);

		if( !parea) break;

		// Block Type에 따라 pen/brust 선택 (0:Down, 1:Up)
		SelectGdiObject(pDC, GetBlockType(pTypeDataSave, typeDataPos), brushArray, penArray);

		// Block Size
		double dHighData = pHighDataSave->GetNext(highDataPos);
		double dLowData = pLowDataSave->GetNext(lowDataPos);
		CPoint curHighPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex + 1, dRealWidth, dDataMin, dDataMax, dHighData, false);
		CPoint curLowPt = CDataConversion::GetVerticalPoint(graphRegion, nIndex + 1, dRealWidth, dDataMin, dDataMax, dLowData, false);

		// Block Draw
		//@NH 100517-by alzioyes.
		if(parea->left>=parea->right)
			CDraw::DrawRectangle(pDC, CRect(parea->left, curHighPt.y, parea->left+1, curLowPt.y));
		else
			CDraw::DrawRectangle(pDC, CRect(parea->left, curHighPt.y, parea->right, curLowPt.y));

		// Prev X-Point를 저장
		//nPrevPt_x = curLowPt.x; 
	}

	// 그리기 도구 모두 초기화.
	DestroyGdiObject(pDC, brushArray, penArray, pOldBrush, pOldPen);

	// Text 그리기
	// 20080908 김진순	8진법/32진법 표시 적용
	//DrawDate(pDC, packetList.GetTail(), pIndicatorInfo, graphRegion, graphColor, p_hOcxWnd);
	DrawDate(pDC, packetList.GetTail(), packetList.GetHead(), pIndicatorInfo, graphRegion, graphColor, p_hOcxWnd);
	return true;
}

// private ====================================================================
bool CRenkoDraw::GetDataList(const CList<CPacket*, CPacket*>& packetList, CList<double, double>*& pHighDataSave, 
		CList<double, double>*& pLowDataSave, CList<double, double>*& pTypeDataSave) const
{
	if(packetList.GetCount() < 3) 
		return false;

	POSITION headPos = packetList.GetHeadPosition();
	CPacket* pHighDataPacket = packetList.GetNext(headPos);
	CPacket* pLowDataPacket = packetList.GetNext(headPos);
	if(pHighDataPacket == NULL || pLowDataPacket == NULL)
		return false;

	pHighDataSave = pHighDataPacket->GetnumericDataList();
	pLowDataSave = pLowDataPacket->GetnumericDataList();
	pTypeDataSave = pHighDataPacket->GetTempList();
	
	return (pHighDataSave != NULL && pLowDataSave != NULL);
}

void CRenkoDraw::GetEctEnvironment(const CRect& graphRegion, const CDisplayAttributes& displayAttributes, 
		double& dRealWidth, int& nPrevPt_x, int& nStartIndex, int& nEndIndex)
{
	// 한 블럭의 사이즈를 구한다. (전체 그래프 X축 영역/총 RenKoChart Packet수)
	// -> 최소 블럭사이즈를 1로 정한다.
	dRealWidth = CDraw::CalRealWidth(graphRegion, displayAttributes);
	if(dRealWidth < 1.0)
		dRealWidth = 1;
	
	// 첫 차트 X축 좌표
	nPrevPt_x = graphRegion.left;

	nStartIndex = displayAttributes.GetDataStartInBlock();
	nEndIndex = displayAttributes.GetDataEndInBlock();
}

int CRenkoDraw::GetBlockType(const CList<double, double>* pTypeDataSave, POSITION& pos) const
{
	if(pTypeDataSave == NULL || pos == NULL)
		return 0;

	int nType = pTypeDataSave->GetNext(pos);
	return (nType < 0 ? 0: nType);
}

// ----------------------------------------------------------------------------
void CRenkoDraw::CreateGdiObject(CBrush* pBrush, CPen* pPen, const CGraphColor& graphColor)
{
	pPen[UP_TYPE].CreatePen(PS_SOLID, 1, graphColor.GetColor1());
	pPen[DOWN_TYPE].CreatePen(PS_SOLID, 1, graphColor.GetColor3());

	pBrush[UP_TYPE].CreateSolidBrush(graphColor.GetColor2());
	pBrush[DOWN_TYPE].CreateSolidBrush(graphColor.GetColor4());
}

void CRenkoDraw::DestroyGdiObject(CDC* pDC, CBrush* pBrush, CPen* pPen, CBrush* pOldBrush, CPen* pOldPen)
{
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	for(int i = 0; i < GDI_COUNT; i++)
		pPen[i].DeleteObject();

	for(int i = 0; i < GDI_COUNT; i++)
		pBrush[i].DeleteObject();
}

void CRenkoDraw::SelectGdiObject(CDC* pDC, const int nType, CBrush* pBrush, CPen* pPen)
{
	if(nType <= 0)
	{
		// 하락
		pDC->SelectObject(pPen[DOWN_TYPE]);
		pDC->SelectObject(pBrush[DOWN_TYPE]);
	}
	else
	{
		// 상승
		pDC->SelectObject(pPen[UP_TYPE]);
		pDC->SelectObject(pBrush[UP_TYPE]);
	}
}

// ----------------------------------------------------------------------------
// Text(날짜) 쓰기
void CRenkoDraw::DrawDate(CDC* pDC, CPacket* pDatePacket, CPacket* pHighDataPacket, const CIndicatorInfo* pIndicatorInfo, 
		const CRect& graphRegion, const CGraphColor& graphColor, HWND p_hOcxWnd)
{
	CString strText;
	CList<CString, CString> strTextList;
	// 20080908 김진순	8진법/32진법 표시 적용
	//if(!GetText(pDatePacket, pIndicatorInfo, strTextList, p_hOcxWnd))
	if(!GetText(pDatePacket, pHighDataPacket, pIndicatorInfo, strTextList, p_hOcxWnd))
		return;

	CRect textRegion = GetTextRegion(pDC, graphRegion, strTextList);
	if(!IsTextDrawing(graphRegion, textRegion))
		return;

	// 테두리 그리기
	DrawTextOutline(pDC, textRegion, ((strTextList.GetCount() -1) * MARGIN), graphColor.GetColor1());

	// Text 쓰기
	CDraw::TextOut_MultiLine(pDC, textRegion.TopLeft(), MARGIN, strTextList, graphColor.GetColor3());
}

void CRenkoDraw::DrawTextOutline(CDC* pDC, const CRect& textRegion, const int nLineGap, const COLORREF& penColor)
{
	CRect outlineRegion = textRegion;
	outlineRegion.bottom += nLineGap;
	outlineRegion.InflateRect(MARGIN, MARGIN);
	// 테두리 그리기
	CDraw::DrawRectangle(pDC, outlineRegion, penColor);
}

// ----------------------------------------------------------------------------
bool CRenkoDraw::GetText(const CPacket* pDatePacket, CPacket* pHighDataPacket, const CIndicatorInfo* pIndicatorInfo, CList<CString, CString>& strTextList, HWND p_hOcxWnd) const
{
	strTextList.RemoveAll();
	
	// 시작날짜 - 끝날짜
	CString strText = GetDateText(const_cast<CPacket*>(pDatePacket));
	if(strText.IsEmpty())
		return false;
	strTextList.AddTail(strText);

	// 벽돌 크기
	// 20080908 김진순	8진법/32진법 표시 적용
	//strText = GetRengaText(pIndicatorInfo, p_hOcxWnd);
	strText = GetRengaText(pHighDataPacket, pIndicatorInfo, p_hOcxWnd);
	if(strText.IsEmpty())
		return false;
	strTextList.AddTail(strText);

	return (strTextList.GetCount() > 0);
}

// 기간
CString CRenkoDraw::GetDateText(CPacket* pDatePacket) const
{
	if(pDatePacket == NULL)
		return "";

	CList<double, double>* pDataList = pDatePacket->GetnumericDataList();
	if(pDataList == NULL || pDataList->GetCount() <= 0)
		return "";

	// 시작날짜 - 끝날짜
	int nDate = (int)pDataList->GetHead();
	CString strDate = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::IntToString(nDate), pDatePacket->GetType(), "");
	strDate += " ~ ";
	nDate = (int)pDataList->GetTail();
	strDate += CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::IntToString(nDate), pDatePacket->GetType(), "");
	return strDate;
}

// 벽돌 크기
CString CRenkoDraw::GetRengaText(CPacket* pHighDataPacket, const CIndicatorInfo* pIndicatorInfo, HWND p_hOcxWnd) const
{
	if(pIndicatorInfo == NULL)
		return "";

	// 벽돌크기 : 2,000
	CList<double, double>* pCalCondList = pIndicatorInfo->GetCommonCalConditionList();
	if(pCalCondList == NULL || pCalCondList->GetCount() <= 0)
		return "";

	_MHWND( p_hOcxWnd);
	ML_SET_LANGUAGE_RES();
	CString strFormat;
	strFormat.LoadString( IDS_FORMAT_BLOCK_VALUE);
	double dRanga = pCalCondList->GetHead();
	// 20080908 김진순	8진법/32진법 표시 적용
	//CString strRanga = CDataConversion::GetStringThousandFormat(CDataConversion::DoubleToString(dRanga));
	CString strRanga = CDataConversion::GetStringThousandFormat(dRanga, pHighDataPacket ? pHighDataPacket->GetType() : "");
	CString strRangaText;
	strRangaText.Format( strFormat, strRanga);

	return strRangaText;
}

CRect CRenkoDraw::GetTextRegion(CDC* pDC, const CRect& graphRegion, const CList<CString, CString>& strTextList) const
{
	CSize size = GetTextSize(pDC, strTextList);
	if(size.cx > graphRegion.Width() || size.cy > graphRegion.Height())
		return CRect(0, 0, 0, 0);

	CRect textRegion(CPoint(graphRegion.right - size.cx, graphRegion.top), size);
	textRegion.OffsetRect(-MARGIN *2, MARGIN *2);
	return textRegion;
}

CSize CRenkoDraw::GetTextSize(CDC* pDC, const CList<CString, CString>& strTextList) const
{
	CSize totalSize(0, 0);
	POSITION pos = strTextList.GetHeadPosition();
	while(pos != NULL){
		CString strText = strTextList.GetNext(pos);
		if(strText.IsEmpty())
			continue;

		CSize size = pDC->GetTextExtent(strText);
		if(totalSize.cx < size.cx)
			totalSize.cx = size.cx;
		totalSize.cy += size.cy;
	}

	return totalSize;
}

bool CRenkoDraw::IsTextDrawing(const CRect& graphRegion, const CRect& textRegion) const
{
	if(graphRegion.IsRectEmpty() || textRegion.IsRectEmpty())
		return false;

	// text를 출력할 영역이 Graph Region의 반이상을 차지하는 경우 출력하지 않는다.
	return (graphRegion.Width() > textRegion.Width() * 2 && graphRegion.Height() > textRegion.Height() * 2);
}
