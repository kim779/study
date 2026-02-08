// PairBarDraw.cpp : implementation file
//

#include "stdafx.h"
#include "PairBarDraw.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable
#include "XScaleManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPairBarDraw

CPairBarDraw::CPairBarDraw()
{
	m_eGraphType = CGraphBaseData::Bar_Type;
	m_graphStyle = CGraphBaseData::SellByTextHorizontalBar;
}

CPairBarDraw::~CPairBarDraw()
{
}


bool CPairBarDraw::DrawGraph(CDC* pDC,
			const CRect& realRegion, 
			const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData,
			const CSubGraphData& subGraphData)
{
	const CList< CPacket *, CPacket *> &packetList = drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return false;

	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	const CPacket& pPacket = *drawingGraphData.GetSubGraphPacketList()->GetTailPacket();
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가

	double dRealWidth = CDraw::CalRealWidth(realRegion, displayAttributes);

	int nBarWidth = CMath::Round(dRealWidth);
	if(nBarWidth <= 1) 
		nBarWidth = 1;

	const CGraphDrawStyle& drawStyle = subGraphData.GetDrawStyle();
	const CGraphColor& graphColor = subGraphData.GetColor();
	int nRealStart = displayAttributes.GetDataStartInBlock();
	int nRealEnd = displayAttributes.GetDataEndInBlock();
	if(nRealStart > nRealEnd)
		return false;
	double dViewMax = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(); 
	double dViewMin = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin();
	CIndicatorInfo* pIndicatorInfo = drawingGraphData.GetIndicatorInfo();
	const CPenThickness& penThickness = subGraphData.GetPenThickness();
	bool bLog = drawingGraphData.DoesVertScaleLogType();
	bool bInvert = drawingGraphData.DoesVertScaleInvertType();

	// (2004.04.01, 배승원) Default Indicator List를 Static으로 관리하여, 반복 Loading을 제한한다.
	//		Static내에 CIndicatorList의 Pointer를 관리하지 않는다. (CIndicatorList는 Static이 아니다.)
	//		CIndicatorInfo를 의존하지 않고 CIndicatorList를 접근할 수 있도록 한다.
	CIndicatorList *pIndicatorList = drawingGraphData.GetIndicatorList();

	switch(subGraphData.GetStyle().GetGraphBarStyle())
	{
	case CGraphBaseData::SellByTextHorizontalBar:// 매수매도Text수평식
		return DrawSellByHorizontalBar(pDC, realRegion, subGraphData.GetName(), 
			drawStyle, graphColor, nRealStart, nRealEnd, dViewMax, dViewMin, 
			packetList, pIndicatorInfo, bLog, bInvert,pPacket);
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	}
	return false;
}

bool CPairBarDraw::DrawGraph(CDC* pDC,
							 const CRect& realRegion, 
							 const CDisplayAttributes& displayAttributes, 
							 const CDrawingGraphData& drawingGraphData,
							 const CSubGraphData& subGraphData,
							 CXScaleManager* pxScaleManager)
{
	if(pxScaleManager && pxScaleManager->GetThreadFlag() == THREAD_CALCULATION)
		return true;

	const CList< CPacket *, CPacket *> &packetList = drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if(packetList.GetCount() <= 0)
		return false;
	
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	const CPacket& pPacket = *drawingGraphData.GetSubGraphPacketList()->GetTailPacket();
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	
	double dRealWidth = CDraw::CalRealWidth(realRegion, displayAttributes);
	
	int nBarWidth = CMath::Round(dRealWidth);
	if(nBarWidth <= 1) 
		nBarWidth = 1;
	
	const CGraphDrawStyle& drawStyle = subGraphData.GetDrawStyle();
	const CGraphColor& graphColor = subGraphData.GetColor();
	int nRealStart = displayAttributes.GetDataStartInBlock();
	int nRealEnd = displayAttributes.GetDataEndInBlock();
	if(nRealStart > nRealEnd)
		return false;
	double dViewMax = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(); 
	double dViewMin = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin();
	CIndicatorInfo* pIndicatorInfo = drawingGraphData.GetIndicatorInfo();
	const CPenThickness& penThickness = subGraphData.GetPenThickness();
	bool bLog = drawingGraphData.DoesVertScaleLogType();
	bool bInvert = drawingGraphData.DoesVertScaleInvertType();
	
	// (2004.04.01, 배승원) Default Indicator List를 Static으로 관리하여, 반복 Loading을 제한한다.
	//		Static내에 CIndicatorList의 Pointer를 관리하지 않는다. (CIndicatorList는 Static이 아니다.)
	//		CIndicatorInfo를 의존하지 않고 CIndicatorList를 접근할 수 있도록 한다.
	CIndicatorList *pIndicatorList = drawingGraphData.GetIndicatorList();
	
	switch(subGraphData.GetStyle().GetGraphBarStyle())
	{
	case CGraphBaseData::SellByTextHorizontalBar:// 매수매도Text수평식
		return DrawSellByHorizontalBar(pDC, realRegion, subGraphData.GetName(), 
			drawStyle, graphColor, nRealStart, nRealEnd, dViewMax, dViewMin, 
			packetList, pIndicatorInfo, bLog, bInvert,pPacket);
		// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	}
	return false;
}

bool CPairBarDraw::DrawSellByHorizontalBar(CDC* pDC, const CRect& graphRegion, 
		const CString& strSubGraphName, const CGraphDrawStyle& drawStyle, const CGraphColor& graphColor, 
		const int nStartIndex, const int nEndIndex, const double& dViewMax, const double& dViewMin, 
		const CList<CPacket*, CPacket*>& packetList, const CIndicatorInfo* pIndicatorInfo, const bool bLog, 
		const bool bInvert,const CPacket& pPacket)
{
	//총 누적거래량,매수 누적거래량,매도 누적거래량
	CList<double, double>* pTotalDataList = NULL;
	CList<double, double>* pByDataList = NULL;
	CList<double, double>* pSellDataList = NULL;

	int nDataCount = 0, nCurrentIndex = -1, nHighestIndex = -1;
	double dHorzViewMin = 0.0, dHorzViewMax = 0.0;
	if(!GetDrawingEnvironment_SellByHorizontalBar(packetList, pTotalDataList, nDataCount, 
		dHorzViewMin, dHorzViewMax, nCurrentIndex, nHighestIndex))
		return false;
	//매도 누적거래량 구하기
	POSITION TailPas = packetList.GetTailPosition();
	CPacket* pSellByPacket = packetList.GetPrev(TailPas);
	if(pSellByPacket == NULL)
		return false;
	pSellDataList = pSellByPacket->GetnumericDataList();
	
	//매수 누적거래량 구하기
	pSellByPacket = packetList.GetPrev(TailPas);;
	if(pSellByPacket == NULL)
		return false;
	pByDataList = pSellByPacket->GetnumericDataList();
	
	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	const CList<double, double>* pHogaDataList = NULL;
	pSellByPacket = packetList.GetPrev(TailPas);;
	if(pSellByPacket == NULL)
		return false;
	pHogaDataList = pSellByPacket->GetnumericDataList();

	if(pHogaDataList == NULL || pHogaDataList->GetCount() <= 0)
		return false;
	POSITION pos = pHogaDataList->GetHeadPosition();

	CRect drawingRegion = GetDrawingRegion(packetList.GetHead(), graphRegion, 
		strSubGraphName, nStartIndex, nEndIndex, dViewMin, dViewMax);
	int nBarWidth = 1;
	double dBarMaxWidth = 0.0;
	GetBarWidth(drawingRegion, nDataCount, dBarMaxWidth, nBarWidth);
	
	double	dMax,dMin,dHogaData;
	dMax = CDraw::CalLogValue(bLog,dViewMax);
	dMin = CDraw::CalLogValue(bLog,dViewMin);

	CPoint oldByDataPt,ByDataPt,oldSellDataPt,SellDataPt;
	double dByData,doldByData,dSellData,doldSellData;

	// (2006.10.23 안승환) 대기매물 호가별 기능 추가
	double dTotalData = GetTotalData(pTotalDataList, nDataCount);

	//sy 2005.01.26. -> 대기매물의 text type.
	// (2008/6/13 - Seung-Won, Bae) Show Values with Option.
//	int nTextType = CBarDraw::GetTextType(pIndicatorInfo);
	//<대기매물> 이 아닌경우에는 text를 모두 보여준다.
	int nTextType = 0, nBuySellText = 1;
	if( pIndicatorInfo) if( g_iMetaTable.IsVolumeForSale( pIndicatorInfo->GetIndicatorName()))
	{
		//0 : 모두 1 : 숨김  2 : 비율만  3 : Data만
		CList<double, double>* pDrawCondList = pIndicatorInfo->GetDrawConditionList();
		if( pDrawCondList)
		{
			int nCount = pDrawCondList->GetCount();
			if( 0 < nCount)
			{
				nTextType = pDrawCondList->GetHead();
				if( pIndicatorInfo->GetIndicatorName() == _MTEXT( C2_BUY_SELL_OVERHANGING_SUPPLY) && 2 == nCount)
					nBuySellText = pDrawCondList->GetTail();
			}
		}
	}

	// 그리기 위해 필요한 값들
	int nOrgMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF nOrgTextColor = pDC->GetTextColor();
	CPen* pOrgPen = pDC->GetCurrentPen();
	CBrush* pOrgBrush = pDC->GetCurrentBrush();
	CPen pen1, pen2, pen3;
	CBrush brush1, brush2, brush3;
	CreatePenAndBrush(graphColor, pen1, pen2, pen3, brush1, brush2, brush3);

	POSITION dataBuyPos = pByDataList->GetHeadPosition();
	POSITION dataSellPos = pSellDataList->GetHeadPosition();
	for(int nIndex = 0; nIndex <= nDataCount; nIndex++)
	{
		if( !pos || !dataBuyPos || !dataSellPos) break;

		// (2006.10.23 안승환) 대기매물 호가별 기능 추가
		if(pos) dHogaData = pHogaDataList->GetNext(pos);
		dByData = pByDataList->GetNext( dataBuyPos);
		dSellData = pSellDataList->GetNext( dataSellPos);

		ByDataPt.x   = CDataConversion::GetHorizontalPoint_X(drawingRegion,	dHorzViewMin,dHorzViewMax,dByData);
		SellDataPt.x = CDataConversion::GetHorizontalPoint_X(drawingRegion,	dHorzViewMin,dHorzViewMax,dSellData);
			// (2008/4/23 - Seung-Won, Bae) Move 3 pixel down to include Bottom Limmit.
		ByDataPt.y = SellDataPt.y = CMath::Round((dMax - CDraw::CalLogValue(bLog, dHogaData))/(dMax - dMin) * graphRegion.Height() + graphRegion.top) + 3;

		if(nIndex > 0)
		{
			//위치 조정을 위해서 -2 를 해줌(bottom = nBarWidth )
			//top에는 +1을 해준다.(DrawBar_TextHorizontalBar() 여기에서)
			nBarWidth = ByDataPt.y - oldByDataPt.y - 2;
			// (2008/4/23 - Seung-Won, Bae) Set minimum width for disapearing.
			if( nBarWidth < 3) nBarWidth = 3;

			SelectPenAndBrushObject(pDC, &pen1, &pen2, &pen3, &brush1, &brush2, &brush3, 
				graphColor, nIndex-1, nIndex-1, nIndex-1);
			DrawBar_SellHorizontalBar(pDC, drawStyle, drawingRegion, nBarWidth, bInvert, oldByDataPt,oldSellDataPt);
			SelectPenAndBrushObject(pDC, &pen1, &pen2, &pen3, &brush1, &brush2, &brush3, 
				graphColor, nIndex-1, -1, -1);
			DrawBar_ByHorizontalBar(pDC, drawStyle, drawingRegion, nBarWidth, bInvert, oldByDataPt,oldSellDataPt);

			// (2008/6/13 - Seung-Won, Bae) Show Values with Option.
			if( doldByData != 0 || doldSellData != 0)
			{
				CString strByData	= ( nBuySellText == 1 ? GetDrawingText( nTextType, dTotalData, doldByData)					: "");
				CString strSellData	= ( nBuySellText == 1 ? GetDrawingText( nTextType, dTotalData, doldSellData)				: "");
				CString strFullData	= ( nBuySellText == 0 ? GetDrawingText( nTextType, dTotalData, doldByData + doldSellData)	: "");
				DrawText_SellByHorizontalBar( pDC, drawingRegion, oldByDataPt, oldSellDataPt, strByData, strSellData, strFullData, nBarWidth, graphColor);
			}

			oldByDataPt = ByDataPt;
			oldSellDataPt = SellDataPt;
			doldByData = dByData;
			doldSellData = dSellData;
		}
		else
		{
			oldByDataPt = ByDataPt;
			oldSellDataPt = SellDataPt;
			doldByData = dByData;
			doldSellData = dSellData;
			// (2008/4/23 - Seung-Won, Bae) Move 3 pixel down to include Bottom Limmit.
			oldByDataPt.y = oldSellDataPt.y = oldSellDataPt.y - 4;
		}
	}

	pDC->SetBkMode(nOrgMode);
	pDC->SetTextColor(nOrgTextColor);
	pDC->SelectObject(pOrgBrush);
	pDC->SelectObject(pOrgPen);

	pen1.DeleteObject();
	pen2.DeleteObject();
	pen3.DeleteObject();
	brush1.DeleteObject();
	brush2.DeleteObject();
	brush3.DeleteObject();
	return true;

}

bool CPairBarDraw::GetDrawingEnvironment_SellByHorizontalBar(const CList<CPacket*, CPacket*>& packetList, 
		CList<double, double>*& pDataList, int& nDataCount, 
		double& dHorzViewMin, double& dHorzViewMax, int& nCurrentIndex, int& nHighestIndex) const
{
	if(packetList.GetCount() <= 0)
		return false;

	CPacket* pPacket = packetList.GetHead();
	if(pPacket == NULL)
		return false;

	pDataList = pPacket->GetnumericDataList();
	if(pDataList == NULL || pDataList->GetCount() <= 0)
		return false;

	nDataCount = pPacket->GetCount();
	nHighestIndex = nHighestIndex = -1;
	// "매수매도대기매물" 일 경우
	if( g_iMetaTable.IsVolumeForSale(pPacket->GetName()) && nDataCount > 2)
	{
		nDataCount -= 2;
		POSITION pos = pDataList->GetTailPosition();
		// 현재, 최대 index
		nHighestIndex = (int)pDataList->GetPrev(pos);
		nCurrentIndex = (int)pDataList->GetPrev(pos);
	}

	pPacket->GetMinMax(0, nDataCount, dHorzViewMin, dHorzViewMax);	
	dHorzViewMin = 0;

	return true;
}

void CPairBarDraw::DrawBar_SellHorizontalBar(CDC* pDC, const CGraphDrawStyle& drawStyle, 
		const CRect& graphRegion, const int nBarWidth, const bool bInvert, CPoint& ByDataPt,CPoint& SellDataPt)
{
	switch( drawStyle.GetGraphBarDrawStyle())
	{
		case CGraphBaseData::Bar_Draw:
		{
			CRect SellbarRect(graphRegion.left, SellDataPt.y + 1  , SellDataPt.x, SellDataPt.y + nBarWidth);
			// (2006.10.23 안승환) 대기매물 호가별 기능 추가
			// 수평바 그리는 부분
			// -> 바형
			if(SellbarRect.Height() <= 1)
			{
				CDraw::DrawLine(pDC, CPoint(SellbarRect.right, SellbarRect.top), CPoint(SellbarRect.right, SellbarRect.top));
			}
			else
			{
				CDraw::DrawRectangle(pDC, SellbarRect);
			}
			break;
		}
		case CGraphBaseData::HorzLineBar_Draw:
		{
			CRect SellbarRect(graphRegion.left, SellDataPt.y + 1  , SellDataPt.x, ( SellDataPt.y + nBarWidth / 2));
			// (2006.10.23 안승환) 대기매물 호가별 기능 추가
			// 수평바 그리는 부분
			// -> 바형
			if(SellbarRect.Height() <= 1)
			{
				CDraw::DrawLine(pDC, CPoint(SellbarRect.right, SellbarRect.top), CPoint(SellbarRect.right, SellbarRect.top));
			}
			else
			{
				CDraw::DrawRectangle(pDC, SellbarRect);
			}
			break;
		}
	}

//	if(bInvert)
//	{
//		InvertBar(graphRegion, barRect);
//		dataPt = CPoint(barRect.right, barRect.CenterPoint().y);
//	}

	// -> 도트형
/*	else if(drawStyle.GetGraphBarDrawStyle() == CGraphBaseData::DotBar_Draw)
	{
		if(barRect.Height() / 2 < 1)
			DrawDotBar(pDC, CPoint(dataPt.x, dataPt.y), 1);
		else
			DrawDotBar(pDC, CPoint(dataPt.x, dataPt.y), nBarWidth);
	}
*/
}

void CPairBarDraw::DrawBar_ByHorizontalBar(CDC* pDC, const CGraphDrawStyle& drawStyle, 
		const CRect& graphRegion, const int nBarWidth, const bool bInvert, CPoint& ByDataPt,CPoint& SellDataPt)
{
	switch( drawStyle.GetGraphBarDrawStyle())
	{
		case CGraphBaseData::Bar_Draw:
		{
			CRect BybarRect( SellDataPt.x, ByDataPt.y + 1, SellDataPt.x + ( ByDataPt.x - graphRegion.left), (ByDataPt.y + nBarWidth));
			// (2006.10.23 안승환) 대기매물 호가별 기능 추가
			// 수평바 그리는 부분
			// -> 바형
			if(BybarRect.Height() <= 1)
			{
				CDraw::DrawLine(pDC, CPoint(BybarRect.left, BybarRect.top), CPoint(BybarRect.right, BybarRect.top));
			}
			else
			{
				CDraw::DrawRectangle(pDC, BybarRect);
			}
			break;
		}
		case CGraphBaseData::HorzLineBar_Draw:
		{
			CRect BybarRect(graphRegion.left, ByDataPt.y + 1 + nBarWidth / 2 , ByDataPt.x, (ByDataPt.y + nBarWidth));
			// (2006.10.23 안승환) 대기매물 호가별 기능 추가
			// 수평바 그리는 부분
			// -> 바형
			if(BybarRect.Height() <= 1)
			{
				CDraw::DrawLine(pDC, CPoint(BybarRect.left, BybarRect.top), CPoint(BybarRect.right, BybarRect.top));
			}
			else
			{
				CDraw::DrawRectangle(pDC, BybarRect);
			}
			break;
		}
	}

//	if(bInvert)
//	{
//		InvertBar(graphRegion, barRect);
//		dataPt = CPoint(barRect.right, barRect.CenterPoint().y);
//	}

	// -> 도트형
/*	else if(drawStyle.GetGraphBarDrawStyle() == CGraphBaseData::DotBar_Draw)
	{
		if(barRect.Height() / 2 < 1)
			DrawDotBar(pDC, CPoint(dataPt.x, dataPt.y), 1);
		else
			DrawDotBar(pDC, CPoint(dataPt.x, dataPt.y), nBarWidth);
	}
*/
}

void CPairBarDraw::DrawText_SellByHorizontalBar(CDC* pDC, const CRect& graphRegion, const CPoint& BydataPt, 
						  const CPoint& SelldataPt, CString strByData, CString strSellData, CString strFullData, short nWidth, const CGraphColor& graphColor)
{
	// 1. Get Region
	CRect rctSell(	graphRegion.left,								SelldataPt.y,	SelldataPt.x,									SelldataPt.y + nWidth);
	CRect rctBuy(	SelldataPt.x,									BydataPt.y,		SelldataPt.x + BydataPt.x - graphRegion.left,	BydataPt.y + nWidth);
	CRect rctFull(	SelldataPt.x + BydataPt.x - graphRegion.left,	SelldataPt.y,	graphRegion.right,								SelldataPt.y + nWidth);

	// 2. Get Text Size
	CSize titleBySize	= pDC->GetTextExtent( strByData);
	CSize titleSellSize	= pDC->GetTextExtent( strSellData);
	CSize titleFullSize	= pDC->GetTextExtent( strFullData);

	// 2.5 Sell Buy Text Overflow
	if( rctSell.Width()	< titleSellSize.cx && rctSell.Width() + rctBuy.Width() < titleBySize.cx + titleSellSize.cx)
	{
		strFullData = strSellData + " / " + strByData;
		strSellData = "";
		strByData = "";
	}

	// 3. Draw Text
	pDC->SetTextColor( graphColor.GetColor2());
	pDC->DrawText( strSellData,	rctSell,	DT_VCENTER | DT_SINGLELINE | DT_NOCLIP | ( rctSell.Width()	< titleSellSize.cx	? DT_LEFT	: DT_RIGHT));
	pDC->SetTextColor( graphColor.GetColor4());
	pDC->DrawText( strByData,	rctBuy,		DT_VCENTER | DT_SINGLELINE | DT_NOCLIP | ( rctBuy.Width()	< titleBySize.cx	? DT_LEFT	: DT_RIGHT));
	pDC->SetTextColor( graphColor.GetColor6());
	pDC->DrawText( strFullData,	rctFull,	DT_VCENTER | DT_SINGLELINE | DT_NOCLIP | ( rctFull.Width()	< titleFullSize.cx	? DT_RIGHT	: DT_LEFT));
}