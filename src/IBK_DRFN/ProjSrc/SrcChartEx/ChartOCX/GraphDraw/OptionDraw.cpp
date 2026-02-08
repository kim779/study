// OptionDraw.cpp: implementation of the COptionDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OptionDraw.h"

#include "DataMath.h"
#include "Conversion.h"
#include "SpecialDraw.h"

#include "BaseData.h"
#include "PacketPointerList.h"
#include "IndicatorList.h"
#include "IndicatorInfo.h"

#include "PacketList.h"
#include "PacketBase.h"
#include "XScaleManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COptionDraw::COptionDraw()
{
	m_eGraphType = CGraphBaseData::FuOp_Type;
}

COptionDraw::~COptionDraw()
{

}

bool COptionDraw::DrawGraph(CDC* pDC,
			const CRect& realRegion, 
			const CDisplayAttributes& displayAttributes, 
			const CDrawingGraphData& drawingGraphData,
			const CSubGraphData& subGraphData)
{
	const CList< CPacket *, CPacket *> &packetList = drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if( packetList.GetCount() <= 0)
		return false;
	double dViewMin = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin();
	double dViewMax = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(); 

	switch(subGraphData.GetStyle().GetGraphFuOpStyle()){
	case CGraphBaseData::MarketProfile:
		return DrawMarketProfile(pDC, realRegion, dViewMin, dViewMax, 
			subGraphData.GetColor(), packetList);
	case CGraphBaseData::FuOpPAndF:
		return DrawFuOpPAndF(pDC, realRegion, displayAttributes/*drawingGraphData.GetIndicatorInfo()*/, 
			subGraphData.GetColor(), dViewMin, dViewMax, packetList);
	}

	return false;
}

bool COptionDraw::DrawGraph(CDC* pDC,
							const CRect& realRegion, 
							const CDisplayAttributes& displayAttributes, 
							const CDrawingGraphData& drawingGraphData,
							const CSubGraphData& subGraphData,
							CXScaleManager* pxScaleManager)
{
	if(pxScaleManager && pxScaleManager->GetThreadFlag() == THREAD_CALCULATION)
		return true;

	const CList< CPacket *, CPacket *> &packetList = drawingGraphData.GetSubGraphPacketList()->GetSubGraphPacketList();
	if( packetList.GetCount() <= 0)
		return false;
	double dViewMin = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMin();
	double dViewMax = drawingGraphData.GetDisplayDataVertMinMax().GetDisplayMax(); 
	
	switch(subGraphData.GetStyle().GetGraphFuOpStyle()){
	case CGraphBaseData::MarketProfile:
		return DrawMarketProfile(pDC, realRegion, dViewMin, dViewMax, 
			subGraphData.GetColor(), packetList);
	case CGraphBaseData::FuOpPAndF:
		return DrawFuOpPAndF(pDC, realRegion, displayAttributes/*drawingGraphData.GetIndicatorInfo()*/, 
			subGraphData.GetColor(), dViewMin, dViewMax, packetList);
	}
	
	return false;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2004.01.20
//
//	분석	:	MP Chart의 출력 Routine이다.
//
//	수정	:	(2004.02.03, 배승원) 최종 종가에 Last Bracket Mark가 출력되도록 한다.
//					이를 위하여 CMProfile Chart의 Member로 Last Price를 기억하여 출력시 참조토록한다.
//					Server에서는 TR Packet 끝의 추가 Packet Data로 Alphabet이 '*'인 Data를 받아,
//						그 Packet Data의 Price로 CMProfile Chart의 Last Price를 지정한다.
//						이 보조 Packet은 복수일 조회라 하더라도 마지막에 1개만 추가된다.
//				(2004.02.05, 배승원) MP Chart의 출력시 TEXT 출력은 MP Chart가 3개 이내인 경우가 아니라,
//					TEXT의 크기가 특정 크기 이하로 내려가는 경우로 제한한다.
//				(2004.02.05, 배승원) 거래량과 Bracket수 영역은 단위 Bracket의 크기보다 작지 않도록 한다.
///////////////////////////////////////////////////////////////////////////////
#define _MARKETPROFILE_MIN_FONTSIZE	10	// (pt)
bool COptionDraw::DrawMarketProfile(CDC* pDC, const CRect& graphRegion, const double& dViewMin, const double& dViewMax, 
		const CGraphColor& graphColor, const CList<CPacket*, CPacket*> &packetList)
{
	// 1. 먼저 오류의 경우를 제외한다.
	// 1.1 Packet List에 문제가 있는 경우를 제외한다.
	if( packetList.GetCount() <= 0)	return false;
	// 1.2 MarketProfile Packet이 없는 경우를 제외한다.
	CMPPacket* pMPPacket = (CMPPacket*) packetList.GetHead();
	if( !pMPPacket) return false;
	// 1.3 MarketProfile Packet의 MP 단위 Chart List를 구하지 못하는 경우를 제외한다.
	CList<CMProfile*, CMProfile*> *pMProfileList = pMPPacket->GetMarketProfileList();
	// 1.3.1 CMProfile Data Object의 수를 관리하여 이후에 이용할 수 있도록 한다.
	int nMProfileCount = pMProfileList->GetCount();
	if( !pMProfileList || nMProfileCount < 1) return false;

	// 2. 화면의 구성을 배치를 구한다.
	//		구하는 순서는
	//			1. 먼저 Bracket의 높이를 산정한다. (이것은 1회의 산출로 확정된다.)
	//			2. 산출된 Bracket의 높이를 기준으로 Font 크기를 산출한다.
	//				*. 이때 Font의 크기를 기준으로 Text 출력여부를 확인한다.
	//			3. 산출된 Font 크기와 Text 출력여부를 이용하여 전체 영역의 크기를 산출한다.
	//			4. 산출된 전체 영역의 기본 크기가 주어진 화면 영역보다 크면, 그 크기의 비율에 맞춰 Bracket의 폭을 조절한다.
	//				*. 이때 Text의 출력이 포함된 경우라면, 다시 확인하여 Text를 출력할 상황인지를 보고,
	//					Text를 출력하지 않는 것으로 조정할 경우, 다시 비율에 맞춰 늘려 조정토록 한다.
	//			5. 산출된 전체 영역의 기본 크기가 주어진 화면 영역보다 작으면 남은 영역을 분할하여 Bracket의 폭에 추가 보정한다.
	// 2.1 Bracket의 높이를 구하다. 
	//		graphRegion			: Graph 출력 영역 Rect
	//		dViewMin, dViewMax	: Graph 출력 영역의 Scale 값			(Logical graphRegion)
	//		nBracketHeight		: Graph 출력 영역의 단위 Bracket Height	
	//		nPriceUnit			: Graph 출력 영역의 단위 Scale 값		(Logical nBracketHeight)
	int nPriceUnitX100 = pMPPacket->GetPriceUnitX100();
	int nBracketHeight = ( graphRegion.Height() * nPriceUnitX100) / int( dViewMax * 100 - dViewMin * 100);
	// 2.2 Bracket의 높이를 기준으로 MP Chart의 기본 영역을 산출한다.
	// 2.2.1 먼저 Font를 생성한다. (이후의 거래량 영역과 Bracket수 영역의 크기 산출에 이용된다.)
	//		(Font 크기 추정방식은 MSDN의 Knowledge Base, Win32 Device Driver Kit, [Q74299] INFO: Calculating The Logical Height and Point Size of a Font 참조)
	//		(어차피 적절한 Font를 생성하는 것이 목적이므로 주어진 Height를 Negative로 설정하여 적절한 Font Object를 생성한다.)
	// 2.2.1.1 추정 Font를 생성한다.
	CFont fFont;
	fFont.CreateFont( -nBracketHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
	// 2.2.1.2 추정 Font의 Internal Leading을 산출한다.
	TEXTMETRIC tmMetric;
	CFont* pOldfont = ( CFont *)pDC->SelectObject( &fFont);
	pDC->GetTextMetrics( &tmMetric);
	int nInternalLeading = tmMetric.tmInternalLeading;
	// 2.2.1.3 정확히 필요한 Font의 크기를 산출한다.
	int nFontHeight = MulDiv( nBracketHeight - nInternalLeading, pDC->GetDeviceCaps( LOGPIXELSY), 72);
	// 2.2.1.3.5 (2004.02.05, 배승원) MP Chart의 출력시 TEXT 출력은 MP Chart가 3개 이내인 경우가 아니라,
	//		TEXT의 크기가 특정 크기 이하로 내려가는 경우로 제한한다.
	// 2.2.1.3.5.1 먼저 TEXT 출력을 생략해야 하는 경우인지 확인한다.
	BOOL bWithText = ( _MARKETPROFILE_MIN_FONTSIZE <= nFontHeight);
	// 2.2.2 거래량 영역과 Bracket수 영역을 구한다. (거래량 영역과 Bracket 영역을 유지하는 경우는 CMProfile이 3개 이내인 경우이다.)
	int nVolumeWidth = 0, nBracketCntWidth = 0;
	CString strMaxVolume, strMaxBracketCnt;
	if( bWithText)
	{
		// 2.2.2.1  정확한 Font를 생성한다.
		pDC->SelectObject( pOldfont);
		fFont.DeleteObject();
		fFont.CreateFont( nFontHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
						CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
		pOldfont = ( CFont *)pDC->SelectObject( &fFont);
		// 2.2.2.3 먼저 거래량 영역의 폭을 산출한다.
		strMaxVolume.Format( "%ld", ( long)pMPPacket->GetMaxVolume());
		nVolumeWidth = pDC->GetTextExtent( strMaxVolume).cx + 4;			// 좌우 내부 여백 and 경계선 1 Pixel 추가
		// 2.2.2.2 Bracket수 영역의 폭을 산출한다.
		strMaxBracketCnt.Format( "%d", pMPPacket->GetMaxHitting());
		nBracketCntWidth = pDC->GetTextExtent( strMaxBracketCnt).cx + 4;	// 좌우 내부 여백 and 경계선 1 Pixel 추가
	}
	// 2.2.3 Bracket의 기본 크기를 구한다.
	// 2.2.3.0 생성한 Font의 폭을 기준으로 Bracket의 기본 크기를 구한다.
	// (2004.01.26, 배승원) Font의 폭은 GetTextExtent()를 이용한다. (LOGFONT는 Width를 제공하지 않는다. 단지 지정했던것을 기억할 뿐)
	// (2004.02.05, 배승원) MP Chart의 출력시 TEXT 출력은 MP Chart가 3개 이내인 경우가 아니라,
	//		TEXT의 크기가 특정 크기 이하로 내려가는 경우로 제한한다.
	//		Text가 출력되지 않을 경우의 Bracket Width는 기본적으로 Bracket Height과 같도록 한다.
	int nBracketWidth = ( bWithText ? pDC->GetTextExtent( "W").cx + 2 : nBracketHeight); // 좌우 여백 or 경계선 1 Pixel 추가
	// 2.2.4 전체 영역의 기본 크기를 구한다.
	//	(처음과 끝의 Bracket Width는 CMProfile Chart의 좌우 여백을 의미한다.)
	int nMaxBracketCountInPacket = pMPPacket->GetMaxBracketCount();
	int nRequestRegionX = ( nBracketWidth + nVolumeWidth + nBracketCntWidth + nBracketWidth * nMaxBracketCountInPacket + nBracketWidth)
		* nMProfileCount;
	// 2.3 구해진 기본 영역과 비교하여, 주어진 화면이 작은 경우를 처리한다.
	//	(비교 크기의 비율에 맞춰 Bracket의 폭을 조절하고, 이후 Font와 다른 영역들도 추가 조정한다.)
	if( graphRegion.Width() < nRequestRegionX)
	{
		// 2.3.1 비율에 맞춰 Bracket의 단위 폭을 산출한다.
		nBracketWidth = ( nBracketWidth * graphRegion.Width()) / nRequestRegionX;
		// (2004.02.05, 배승원) MP Chart의 출력시 TEXT 출력은 MP Chart가 3개 이내인 경우가 아니라,
		//		TEXT의 크기가 특정 크기 이하로 내려가는 경우로 제한한다.
		//		이번 조정에의해 Text를 출력할 상황이 안되는 경우를 처리한다. (원래 안되는 경우 말고..)
		if( bWithText)
		{
			// 2.3.2 새로운 Font의 크기를 산출하여, Text를 출력할 상황인지를 확인한다.
			nFontHeight = ( nFontHeight * graphRegion.Width()) / nRequestRegionX;
			bWithText = ( _MARKETPROFILE_MIN_FONTSIZE <= nFontHeight);
			// 2.3.3 Text를 출력할 상황으로 확인된 경우의 Text 영역의 크기를 조정한다.
			if( bWithText)
			{
				// 2.3.3.2 Font의 크기를 조정한다. (이전 Font를 제거하고 새로이 생성한다.)
				pDC->SelectObject( *pOldfont);
				fFont.DeleteObject();
				// 2.3.3.3 새로운 Font를 생성한다.
				fFont.CreateFont( nFontHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
								CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
				pOldfont = ( CFont *)pDC->SelectObject( &fFont);
				// 2.3.3.4 거래량 영역과 Bracket수의 영역도 산출한다.
				// 2.3.3.4.1 먼저 거래량 영역의 폭을 산출한다.
				nVolumeWidth = pDC->GetTextExtent( strMaxVolume).cx + 4;			// 좌우 내부 여백 and 경계선 1 Pixel 추가
				// 2.3.3.4.2 Bracket수 영역의 폭을 산출한다.
				nBracketCntWidth = pDC->GetTextExtent( strMaxBracketCnt).cx + 4;	// 좌우 내부 여백 and 경계선 1 Pixel 추가
			}
			// 2.3.4 Text를 출력할 상황이 되지 않는 경우, Text 출력 영역을 Clear시킨다.
			else
			{
				nVolumeWidth = 0;
				nBracketCntWidth = 0;
			}
		}
		// 2.3.5 조정된 부분 영역값으로 전체 영역의 크기를 산출한다.
		nRequestRegionX = ( nBracketWidth + nVolumeWidth + nBracketCntWidth + nBracketWidth * nMaxBracketCountInPacket + nBracketWidth)
			* nMProfileCount;
	}
	// (2004.02.05, 배승원) 거래량과 Bracket수 영역이 단위 Bracket의 크기보다 작지 않도록 한다.
	//		실제 거래량과 Bracket수 영역을 보관하여 Text 출력시 출력열이 중앙 정렬되도록 한다. (Text 중앙정렬 말고)
	int nRealVolumeWidth		= nVolumeWidth;
	int nRealBracketCntWidth	= nBracketCntWidth;
	// 2.4 조정된 영역에 의해 남는 영역은, 분할하여 Bracket의 폭에 추가 보정한다.
	//		(생성되는 여백이 Bracket 수보다 크면 Bracket의 Width를 조절한다. BracketWidth는 좌우측 여백크기에도 영향을 미치는 것을 감안한다.)
	if( nRequestRegionX < graphRegion.Width())
	{
		// (2004.02.05, 배승원) 거래량과 Bracket수 영역이 단위 Bracket의 크기보다 작지 않도록 한다.
		//		작게되는 경우는, 최종적으로 요구되는 전체 영역을 비율로 축소 조정한 후,
		//		남은 영역을 Bracket으로 분할하여 Bracket을 늘리는 경우에 해당한다.
		//		따라서, Bracket을 늘리는 시점에서 거래량과 Bracket수 영역도 늘리도록한다.
		int nAddWidth = ( ( graphRegion.Width() - nRequestRegionX) / nMProfileCount)
			/ ( nMaxBracketCountInPacket + 2 + ( bWithText ? 2 : 0));	// 좌우 여백외에 거래량과 Bracket수 영역도 감안한다.
		// 2.4.1 BracketWidth를 보정한다.
		nBracketWidth += nAddWidth;
		// 2.4.2 거래량과 Bracket수 영역도 보정한다.
		if( bWithText)
		{
			nVolumeWidth		+= nAddWidth;
			nBracketCntWidth	+= nAddWidth;
		}
	}

	// 3. 각 CMProfile Chart를 해당 영역에 Drawing 처리한다.
	// 3.0 DC에 기본 설정을 처리하여 이후 이용토록 한다.
	int			nOldBkMode		= pDC->SetBkMode( TRANSPARENT);
	COLORREF	clrOldTextColor	= pDC->GetTextColor();
	CBrush		*pOldBrush		= pDC->GetCurrentBrush();
	CPen		*pOldPen		= pDC->GetCurrentPen();

	// 3.1 CMProfile Chart의 처리에 필요한 기본값을 구하여 관리한다. 이후 참조된다.
	// 3.1.0 Start/End Bracket Color를 구하여 관리한다. 이후 참조된다.
	const DWORD dwStartColor	= ( DWORD)graphColor.GetColor2();
	const DWORD dwEndColor		= ( DWORD)graphColor.GetColor1();
	// 3.1.1 Volume이나 BracketCount의 출력에 이용할 Text Color를 관리한다.
	const COLORREF clrMaxTextColor		= graphColor.GetColor2();
	const COLORREF clrNormalTextColor	= graphColor.GetColor5();
	// 3.1.2 Bracket을 출력하기 위하여 필요한 색상을 관리한다.
	const COLORREF clrFirstBracketColor	= graphColor.GetColor3();
	const COLORREF clrLastBracketColor	= graphColor.GetColor4();
	const COLORREF clrBracketTextColor	= graphColor.GetColor6();
	// 3.1.2 (2004.01.27, 배승원) Bracket 행간의 빈틈이 없도록 만들기 위하여 정수 연산을 처리한다.
	//		(좌표 Scale의 Min/Max를 X100으로 구하여 관리한다.)
	const int nViewMinX100	= int( dViewMin * 100);
	const int nViewMaxX100	= int( dViewMax * 100);

	// 3.1.3 MProfile Chart의 폭을 산출하고, 이후 Drawing에 이용될 기준 Left를 설정한다.
	// (2004.02.03, 배승원) MP Chart의 Width는 위의 RequestRegionX의 원리가 맞으나,
	//		아래의 MP Profile Width는 매 MP Chart를 그리기 위한 기준 Left 이동 간격으로,
	//		꼭 필요한 폭만큼 이동하지 않고, MP Chart가 화면에 알맞게 분포되도록 (X Scale에 맞도록) 전체 화면을 분할한 수치로 한다.
	int nMProfileWidth			= graphRegion.Width() / nMProfileCount;	// nBracketWidth + nVolumeWidth + nBracketCntWidth + nBracketWidth * nMaxBracketCountInPacket + nBracketWidth;
	int nMProfileLeft			= graphRegion.left;
	nMProfileLeft				+= nBracketWidth;	// 좌측 여백을 미리 띄워놓고 시작한다.
	nMProfileLeft				-= nMProfileWidth;	// Loop에서 바로 증가되므로 미리 감하여 놓는다.

	// 3.2 각 CMProfile Chart를 해당 영역에 Drawing 처리한다.
	POSITION posMProfile = pMProfileList->GetHeadPosition();
	while( posMProfile)
	{
		CMProfile* pMarketProfile = pMProfileList->GetNext( posMProfile);

		// 3.3 먼저 CMPorfile Chart내에 등록된 Bracket Group인 CMPItem이 있는지 확인한다.
		CList < CMPItem*, CMPItem*> *pMPItemList = pMarketProfile->GetMPItemList();
		POSITION posMPItem = pMPItemList->GetHeadPosition();
		if( !posMPItem) continue;

		// 3.4 각 CMProfile Chart를 Drawing하기 위해 필요한 CMProfile의 기본정보를 구하여 관리한다.
		int		nTotalBracketCount			= pMarketProfile->GetTotalBracketCount();
		int		nMaxBracketCountInMProfile	= pMarketProfile->GetMaxBracketCount();
		DWORD	dwMaxVolume					= pMarketProfile->GetMaxVolume();
		nMProfileLeft						+= nMProfileWidth;

		// 3.5 거래량과 Bracket Count 출력 영역의 Box를 그린다.
		if(	bWithText)
		{
			// 3.5.1 가격대의 범주를 구한다.
			int nMinX100, nMaxX100;
			if( !pMarketProfile->GetPriceMinMaxX100( nMinX100, nMaxX100)) return false;
			// 3.5.2 MProfile Chart의 Screen 상하 영역을 구한다.
			// (2004.01.27, 배승원) Bracket 행간의 빈틈이 없도록 만들기 위하여 정수 연산을 처리한다.
			int nMProfileTop	= graphRegion.bottom - int( graphRegion.Height() * ( nMaxX100 + nPriceUnitX100	- nViewMinX100) / double( nViewMaxX100 - nViewMinX100));
			int nMProfileBottom	= graphRegion.bottom - int( graphRegion.Height() * ( nMinX100					- nViewMinX100) / double( nViewMaxX100 - nViewMinX100));
			// 3.5.3 거래량과 Bracket Count 출력 영역의 Box를 그린다.
			pDC->SelectObject( GetStockObject( WHITE_BRUSH));
			pDC->SelectObject( GetStockObject( BLACK_PEN));
			pDC->Rectangle( &CRect( nMProfileLeft, nMProfileTop, nMProfileLeft + nVolumeWidth + nBracketCntWidth, nMProfileBottom));
			pDC->MoveTo( nMProfileLeft + nVolumeWidth, nMProfileTop);
			pDC->LineTo( nMProfileLeft + nVolumeWidth, nMProfileBottom);
		}

		// 3.6 각 CMProfile의 가격대 행(CMPItem)별로 Drawing 처리한다.
		while( posMPItem)
		{
			CMPItem *pMPItem = pMPItemList->GetNext( posMPItem);

			// 3.7 각 가격대 행(CMPItem)별 Drawing 영역(Top,Bottom)을 산출한다.
			// (2004.01.27, 배승원) Bracket 행간의 빈틈이 없도록 만들기 위하여 정수 연산을 처리한다.
			int nMPItemTop		= graphRegion.bottom - int( graphRegion.Height() * ( pMPItem->GetPriceX100() + nPriceUnitX100	- nViewMinX100) / double( nViewMaxX100 - nViewMinX100));
			int nMPItemBottom	= graphRegion.bottom - int( graphRegion.Height() * ( pMPItem->GetPriceX100()					- nViewMinX100) / double( nViewMaxX100 - nViewMinX100));

			// 3.8 먼저 거래량과 Bracket수를 출력시킨다. (물론 CMProfile Chart가 2개 이내인 경우)
			// (2004.02.05, 배승원) 거래량과 Bracket수 영역이 단위 Bracket의 크기보다 작지 않도록 한다.
			//		실제 거래량과 Bracket수 영역을 보관하여 Text 출력시 출력열이 중앙 정렬되도록 한다. (Text 중앙정렬 말고)
			if(	bWithText)
			{
				// 3.8.0 거래량과 Bracket 수를 구한다.
				int		nBracketCount	= pMPItem->GetBracketList()->GetCount();
				DWORD	dwVolume		= pMPItem->GetVolume();

				// 3.8.1 거래량과 Bracket 수를 TEXT로 구한다.
				CString strVolume, strBracketCount;
				strBracketCount.Format( "%d", nBracketCount);
				strVolume.Format( "%lu", dwVolume);

				// 3.8.2 거래량을 먼저 출력시킨다.
				// 3.8.2.1 먼저 거래량의 색상을 설정한다.
				if( dwVolume == dwMaxVolume) pDC->SetTextColor( clrMaxTextColor);
				else pDC->SetTextColor( clrNormalTextColor);
				// 3.8.2.2 Text를 출력한다.
				CRect rctVolumeRect( nMProfileLeft, nMPItemTop, nMProfileLeft + nVolumeWidth - 1, nMPItemBottom);
				rctVolumeRect.DeflateRect( ( rctVolumeRect.Width() - nRealVolumeWidth) / 2, 0);
				pDC->DrawText( strVolume, rctVolumeRect, DT_SINGLELINE | DT_NOCLIP | DT_RIGHT | DT_VCENTER);

				// 3.8.3 Bracket수를 출력시킨다.
				// 3.8.3.1 Bracket수의 출력 색상을 설정한다.
				if( nBracketCount == nMaxBracketCountInMProfile) pDC->SetTextColor( clrMaxTextColor);
				else pDC->SetTextColor( clrNormalTextColor);
				// 3.8.3.2 Text를 출력한다. (Why Right - 2 ? 'Rectangle()'로 Drawing한 Border와 공백을 포함하여 2 Pixel)
				CRect rctBracketCntRect( nMProfileLeft + nVolumeWidth, nMPItemTop, nMProfileLeft + nVolumeWidth + nBracketCntWidth - 2, nMPItemBottom);
				rctBracketCntRect.DeflateRect( ( rctBracketCntRect.Width() - nRealBracketCntWidth) / 2, 0);
				pDC->DrawText( strBracketCount, rctBracketCntRect, DT_SINGLELINE | DT_NOCLIP | DT_RIGHT | DT_VCENTER);
			}

			// 3.9 각 가격대 행별로 Bracket을 출력처리한다.
			// 3.9.0 각 Bracket의 좌측 기준 좌표를 산출하여 관리한다.
			int	 nMProfileBracketLeft	= nMProfileLeft + nVolumeWidth + nBracketCntWidth;
			nMProfileBracketLeft		-= nBracketWidth;	// Loop에서 바로 증가되므로 미리 감하여 놓는다.
			// 3.9.1 각 가격대 행별로 Bracket을 출력처리한다.
			CList < CBracket *, CBracket *> *pBracketList = pMPItem->GetBracketList();
			POSITION posBracket = pBracketList->GetHeadPosition();
			//	(2004.02.03, 배승원) 최종 종가에 Last Bracket Mark가 출력되도록 한다.
			//		여기서 해당 Bracket Row가 종가 Row인지 확인한다. (MP Chart의 종가는 MP Chart가 관리한다.)
			BOOL bLastPriceBracketRow = ( pMPItem->GetPriceX100() == pMarketProfile->GetLastPriceX100());
			while( posBracket)
			{
				CBracket *pBracket = pBracketList->GetNext( posBracket);

				// 3.10 다음 Bracket의 Left 좌표를 증가시킨다.
				nMProfileBracketLeft += nBracketWidth;

				// 3.11 Bracket을 Color Box로 Drawing한다.
				int nBracketIndex = pBracket->GetIndex();
				CRect rctBracket( nMProfileBracketLeft, nMPItemTop, nMProfileBracketLeft + nBracketWidth, nMPItemBottom);
				//	(2004.02.03, 배승원) 최종 종가에 Last Bracket Mark가 출력되도록 한다.
				//		따라서, Index와 상관없이 최종 종가가 있는 Bracket행의 마지막 Bracket인지를 확인하여야 한다.
				//		이 색상 지정은 시작 색상이나 중간 Gradation 색상보다 우선한다.
				if( bLastPriceBracketRow && !posBracket)
					pDC->FillSolidRect( rctBracket, clrLastBracketColor);
				else
				{
					if( nBracketIndex == 1)
						pDC->FillSolidRect( rctBracket, clrFirstBracketColor);
					else
						pDC->FillSolidRect( rctBracket,
							RGB(	GetRValue( dwStartColor) + ( GetRValue( dwEndColor) - GetRValue( dwStartColor)) / ( double)nTotalBracketCount * nBracketIndex,
									GetGValue( dwStartColor) + ( GetGValue( dwEndColor) - GetGValue( dwStartColor)) / ( double)nTotalBracketCount * nBracketIndex,
									GetBValue( dwStartColor) + ( GetBValue( dwEndColor) - GetBValue( dwStartColor)) / ( double)nTotalBracketCount * nBracketIndex));
				}

				// 3.12 Bracket내에 Text를 출력한다.
				if(	bWithText)
				{
					// (2004.02.24, 배승원) 시작 Bracket뿐만 아니라, 끝 Bracket의 글자색도 검은색으로 한다.
					if( nBracketIndex == 1 || bLastPriceBracketRow && !posBracket) pDC->SetTextColor( clrNormalTextColor);
					else pDC->SetTextColor( clrBracketTextColor);
					pDC->DrawText( pBracket->GetItem(), 1, rctBracket, DT_SINGLELINE | DT_NOCLIP | DT_CENTER | DT_VCENTER);
				}
			}
		}
	}

	// 3.0 DC에 기본 설정을 처리하여 이후 이용토록 한다.
	pDC->SelectObject( pOldBrush);
	pDC->SelectObject( pOldPen);
	pDC->SetTextColor( clrOldTextColor);
	pDC->SetBkMode( nOldBkMode);
	pDC->SelectObject( pOldfont);
	fFont.DeleteObject();

	return true;
}


///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2004.02.05
//
//	분석	:	선물 전용 P&F Chart (옵션 아님. 선물전용 호가 단위 기준으로 Hard Codeing 되어 있음.)
//				1. 기본사항
//					1. 선물 전용 P&F는 t분봉 종가 혹은 Tick의 Price를 기준으로 n호가 단위의 전환도이다.
//						(삼선 전환도와 유사)
//					2. 상승은 'X', 하락은 'O'표를 하여 MarkLine을 구성한다.
//					3. 등락의 방향이 이전 고/저 가격의 n호가 단위 반전되어야만 방향 전환을 인정하고,
//						전환 깃점 다음부터의 Marking을 처리하게 된다.
//					4. Mark는 항상 호가단위 (0.05)마다 표시한다.
//					5. 일자가 변경되는 경우, 해당 Mark부터 색상을 변경한다. (적색과 청색을 반복하며 전환한다.)
//				2. 개발 정보
//					1. 출력 Packet
//						1. Scale Packet		: Drawing 처리시 Y Scale이 가추세에 의해 급변하는 경우를 제한하기 위하여,
//												Drawing된 값의 Min/Max Data를 관리하여, 지속적인 축소만이 가능하도록 지원하는 Packet이다.
//											  Drawing 처리시의 일관적으로 처리가 가능하도록 항상 맨 처음 추가한다.
//											  Drawing시 일괄적으로 무시한다.
//								시가		: 전체 출력 Data의 최소값
//								종가		: 전체 출력 Data의 최대값
//								새일자가	: 이용 없음
//						2. 추세 Packet Data	: Server에 구성된 추세 Packet이다.
//								시가		: 추세의 시작 가격이다. 이전 추세의 종가보다 0.05가 크거나 (상승의 경우) 작다 (하락의 경우)
//								종가		: 추세의 끝 가격이다. 시가와 종가로 상승추세인지 하락추세인지 식별한다.
//								새일자가	: 추세 진행중에 발생한 새일자가로, 추적중 일자가 변경되는 경우의 마지막 일일 시가가 등록된다.
//												추세 진행중에 발생한 일일 시가가 없는 경우는 0으로 설정된다.
//												특히 조회시작일의 일일 시가는 이전 추세가 형성되지 않으므로 역시 0으로 설정한다.
//						3. 가추세 Packet Data	: Server에서 내려주는 Packet이 아닌, Real의 분봉 처리시 Tic에 의해 형성되는 추세 Packet이다.
//								[가추세]	: 가추세는 분봉의 최종 종가가 확정되지 않는 상황에서의 임시 추세를 나타내는 Packet으로,
//													Server가 아닌, 지표 산출시 제 2기준 Packet을 바탕으로 생성되며,
//													Tic에 의한 추세 반전이나 확장을 나타내는 Packet으로 항상 등록된다.
//											  가추세는 지표 출력 Routine의 일관성을 위하여, Real 뿐만 아니라 TR의 경우에도 추가한다.
//											  또한 가추세는 출력시 추세확장인 경우, 이전 추세에 포함하여 확장으로 출력처리하고,
//													반전의 경우 그 추세폭을 확인하여 반전으로 처리한다.
//								시가		: Tic에 의해 임시로 생성된 추세의 시가이다.
//								종가		: Tic에 의해 임시로 생성된 추세의 종가이다.
//								새일자가	: 가추세를 형성하는 끝봉이 이전추세와 날짜가 다른 경우 갖는 가격으로 추세의 종가이다.
//
//	수정	:	(2004.02.05, 배승원) 기간 및 한칸 단위 크기와 전환 기준 칸수등을 출력할 필요가 없어 삭제한다.
///////////////////////////////////////////////////////////////////////////////
bool COptionDraw::DrawFuOpPAndF(CDC* pDC, const CRect& graphRegion, const CDisplayAttributes& displayAttributes,/*CIndicatorInfo* pIndicatorInfo,*/ 
		const CGraphColor& graphColor, const double& dViewMin, const double& dViewMax,
		const CList<CPacket*, CPacket*> &packetList)
{
	// 1. 먼저 P&F 지표 Packet의 Data List를 구한다.
	// 1.1 Packet의 개수를 확인한다.
	if( packetList.GetCount() < 3) return false;
	// 1.2 Packet Object Pointer를 구한다.
	POSITION posPnF = packetList.GetHeadPosition();
	CList< double, double> *pMLStartPriceList	= packetList.GetNext( posPnF)->GetnumericDataList();	// MarkLine의 시가
	CList< double, double> *pMLEndPriceList		= packetList.GetNext( posPnF)->GetnumericDataList();	// MarkLine의 종가
	CList< double, double> *pMLNewDatePriceList	= packetList.GetNext( posPnF)->GetnumericDataList();	// MarkLine의 새일자가
	if( !pMLStartPriceList || !pMLEndPriceList || !pMLNewDatePriceList) return false;

	// 2. Drawing에 필요한 환경 값을 산출한다.
	// 2.1 먼저 MarkLine의 개수를 확인한다.
	//	(2004.02.25, 배승원) MarkLine에는 Scale과 가추세 Packet이 항상 포함되어 있다.
	//		그러나 MarkLine의 폭을 산출하기 위하여는 정추세보다 하나 더 많게 설정한다.
	//		(가추세에 의한 급변을 없애기 위함이다.)
	int nMLCount = pMLStartPriceList->GetCount() - 1;
	// 2.2 Draw 영역의 Min/Max Price 값을 산출한다.
	int nViewMinX100 = dViewMin * 100;
	int nViewMaxX100 = dViewMax * 100;

	// 3. Mark의 크기를 산출한다.
	// 3.1 P&F Chart의 Mark 1개 (1호가)의 높이를 구한다.
	int nMarkHeight = int( ( graphRegion.Height() * 5) / double( nViewMaxX100 - nViewMinX100));
	if( nMarkHeight == 0) nMarkHeight = 1;
	// 3.2 Mark의 폭을 구한다.
	// 3.2.1 먼저 높이로 기본폭을 선정한다.
	int nMarkWidth = nMarkHeight;
	// 3.2.2 기본 폭으로 출력시 화면을 벗어나는 경우를 확인하여 조정한다.
	if( graphRegion.Width() < nMarkWidth * nMLCount)
	{
		nMarkWidth = graphRegion.Width() / nMLCount;
		if( nMarkWidth == 0) nMarkWidth = 1;
	}

	// 4. Drawing을 위하여 DC 설정을 처리한다.
	// 4.1 Drawing에 필요한 Pen을 생성한다.
	CPen pnPen1(	PS_SOLID, 1, graphColor.GetColor1());
	CPen pnPen2(	PS_SOLID, 1, graphColor.GetColor3());
	// 4.2 첫 Marking 색상을 설정한다.
	CPen* pOldPen = ( CPen *)pDC->SelectObject( &pnPen1);
	// 4.3 NULL Brush를 설정한다.
	CBrush *pOldBrush = ( CBrush *)pDC->SelectStockObject( NULL_BRUSH);

	// 5. Mark Line을 Iterating하면서 출력한다.
	// 5.0 반목 처리에 필요한 변수를 정의한다.
	BOOL	bUpMark;
	int		nMarkLeft, nMarkTop, nMarkRight, nMarkBottom;
	int		nDrawPriceX100, nStartPriceX100, nEndPriceX100, nNewDataPriceX100;
	int		nMarkDirectionPriceX100, nUpLimitPriceX100, nDownLimitPriceX100;
	// 5.1 Data List의 Head Position을 산출한다.
	int			nMLIndex = 0;
	POSITION	posStartPrice	= pMLStartPriceList->GetHeadPosition();
	POSITION	posEndPrice		= pMLEndPriceList->GetHeadPosition();
	POSITION	posNewDatePrice	= pMLNewDatePriceList->GetHeadPosition();
	// (2004.02.25, 배승원) Scale Packet을 무시한다. (물론 있는지 확인하고)
	if( posStartPrice && posEndPrice && posNewDatePrice)
	{
		pMLStartPriceList->GetNext(		posStartPrice	);
		pMLEndPriceList->GetNext(		posEndPrice		);
		pMLNewDatePriceList->GetNext(	posNewDatePrice	);
	}
	while( posStartPrice && posEndPrice && posNewDatePrice)
	{
		// 5.2 먼저 각 MarkLine의 시가/저가/새일자가를 조회한다.
		nStartPriceX100		= int( pMLStartPriceList->GetNext(		posStartPrice	) * 100);
		nEndPriceX100		= int( pMLEndPriceList->GetNext(		posEndPrice		) * 100);
		nNewDataPriceX100	= int( pMLNewDatePriceList->GetNext(	posNewDatePrice	) * 100);

		//	(2004.02.25, 배승원) 마지막 MarkLine인 가추세가 확장 가추세인 경우,
		//		이전 정추세에 이어서 Drawing을 처리하도록 확인한다.
		//		즉 MarkLine Index를 감소시켜 이전 추세에 연장하여 그린다.
		//		특히 가추세 하나만 있는 경우에 오동작하지 않도록 주의한다.
		// (2004.02.25, 배승원) 만약 MarkLine의 시가와 고가가 같은 값이면,
		//			형식 가추세 Packet이거나 연장 가추세 Packet이다.
		//		따라서, 이경우 추세 방향을 바꾸지 않는다.
		if( nMLIndex && nMLIndex == nMLCount - 1
			&& ( ( bUpMark && nStartPriceX100 <= nEndPriceX100)
				|| ( !bUpMark && nEndPriceX100 <= nStartPriceX100)))	nMLIndex--;

		// 5.3 Mark Line의 Left 좌표와 Right 좌표를 산출한다.
		nMarkLeft		= graphRegion.left + nMarkWidth * nMLIndex;
		nMarkRight		= nMarkLeft + nMarkWidth;

		// 5.4 상승과 하락에 따라 구분하여 Drawing을 처리할 수 있도록 Flag을 설정하고,
		//		아래에서부터 위로 Drawing할 수 있도록 Mark Drawing Direction Price와
		//			Up/Down Limit Price를 조정한다.
		// (2004.02.25, 배승원) 만약 MarkLine의 시가와 고가가 같은 값이면,
		//			형식 가추세 Packet이거나 연장 가추세 Packet이다.
		//		따라서, 이경우 추세 방향을 바꾸지 않는다.
		if( nStartPriceX100 != nEndPriceX100) bUpMark = ( nStartPriceX100 < nEndPriceX100);
		if( bUpMark)
		{
			nMarkDirectionPriceX100 = 5;
			nUpLimitPriceX100		= nEndPriceX100;
			nDownLimitPriceX100		= nStartPriceX100;
		}
		else
		{
			nMarkDirectionPriceX100 = -5;
			nUpLimitPriceX100		= nStartPriceX100;
			nDownLimitPriceX100		= nEndPriceX100;
		}

		// 5.5 Mark를 반복 출력하여 MarkLine을 Draw 처리한다.
		for( nDrawPriceX100 = nStartPriceX100; nDownLimitPriceX100 <= nDrawPriceX100 && nDrawPriceX100 <= nUpLimitPriceX100; nDrawPriceX100 += nMarkDirectionPriceX100)
		{
			// 5.5.1 해당 가격의 좌표를 산출한다.
			//	(특히 Mark가 가격 Scale 표식의 중간에 위치할 수 있도록 0.025 호가단위만큼 하향 조정한다.
			nMarkBottom = graphRegion.bottom - int( ( graphRegion.Height() * ( nDrawPriceX100 * 10 - 25  - nViewMinX100 * 10)) / double( nViewMaxX100 * 10 - nViewMinX100 * 10));
			nMarkTop	= nMarkBottom - nMarkHeight;

			// 5.5.2 일자가 변경된 가격이면, 색상을 변경한다.
			if( nDrawPriceX100 == nNewDataPriceX100)
			{
				if( pDC->GetCurrentPen() == &pnPen1) pDC->SelectObject( &pnPen2);
				else pDC->SelectObject( &pnPen1);
			}

			// 5.5.3 Mark를 출력한다.
			// 5.5.3.1 상승 Mark를 출력한다. (X Marking)
			//	(주의, LineTo는 마지막 점을 출력하지 않는 것을 감안하여 Start 좌표도 그려지지 않도록 조정한다.)
			if( bUpMark)
			{
				pDC->MoveTo( nMarkLeft + 1,	nMarkBottom - 1);
				pDC->LineTo( nMarkRight,	nMarkTop);
				pDC->MoveTo( nMarkLeft + 1,	nMarkTop + 1);
				pDC->LineTo( nMarkRight,	nMarkBottom);
			}
			// 5.4.2.2 하락 Mark를 출력한다. (O Marking)
			else pDC->Ellipse( nMarkLeft, nMarkTop, nMarkRight, nMarkBottom);
		}

		// 5.5 다음 Mark Line의 처리를 위하여 Index를 증가시킨다.
		nMLIndex++;
	}

	// 6. Drawing을 위하여 설정한 DC를 복원한다.
	pDC->SelectObject( pOldPen);
	pDC->SelectObject(pOldBrush);
	pnPen1.DeleteObject();
	pnPen2.DeleteObject();

	return true;
}
