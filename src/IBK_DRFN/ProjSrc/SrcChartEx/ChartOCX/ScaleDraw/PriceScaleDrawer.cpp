// PriceScaleDrawer.cpp: implementation of the CPriceScaleDrawer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PriceScaleDrawer.h"

#include "../Include_Chart/DLL_Load/PacketBase.h"		// for CPacket
#include "ScaleRegion.h"
#include "BaseData.h"
#include "Conversion.h"
#include "DataMath.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CPriceScaleDrawer

CPriceScaleDrawer::CPriceScaleDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
	: CVertScaleDrawer( p_hOcxWnd, p_eChartMode)
{
}

// scale text, grid를 그린다.
void CPriceScaleDrawer::DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale)
{
	DrawTextOrGrid(pDC, drawingVertScaleData, scaleRegion, vertScale, CDrawer::GetPacketDataType(drawingVertScaleData.GetPacket()));
}

// 양쪽
void CPriceScaleDrawer::DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale)
{
	DrawTextOrGrid_Both(pDC, drawingVertScaleData, vertScaleRegion, vertScale, CDrawer::GetPacketDataType(drawingVertScaleData.GetPacket()));
}

// protected ============================================================================
// (2009/12/8 - Seung-Won, Bae) Comment and Adjusting
void CPriceScaleDrawer::DrawTextOrGrid(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CScaleDrawingRegion& scaleRegion, const CVerticalScale& vertScale, const CString& dataType)
{
	// 0. 주 Data Packet을 확인한다.
	CPacket *pPacket = drawingVertScaleData.GetPacket();
	if( !pPacket) return;

	// 0. 스케일 영역에 스케일 Text를 찍을 수 있는 갯수 확인
	int totalCount = CVertScaleDrawer::GetCount( pDC, ( scaleRegion.GetScaleTextRegion()).Height()) +1;
	if( totalCount <= 0) return;

	int start = 0, end = 0;	// Scale 구분선 시작위치와 끝위치
	double dGap = 0.0;		// Scale 구분선 간격
	bool bround = false;	// 자동 Scale 단위 조정 기능 여부 (?)
	if( !GetPriceDrawingEnvironment( drawingVertScaleData.GetDisplayDataMinMax(), dataType, totalCount,
			dGap, start, end, bround, vertScale, drawingVertScaleData.GetHogaUnit()))
	{
		if(!CVertScaleDrawer::GetDrawingEnvironment( pDC, scaleRegion.GetScaleTextRegion(), 
			drawingVertScaleData.GetDisplayDataMinMax(), dataType, start, end, dGap, bround))
		return;
	}
	//}}

	// 절사 할 자리수 구하기 - 정확한 타입 다시 쓰기 12.24.2001
	BOOL bAutoScaleType = pPacket->IsAutoScaleType();
	CString strFormatTrans = CVertScaleDrawer::GetFormatTrans(drawingVertScaleData.GetDisplayDataMinMax(), dataType, bAutoScaleType);
	pPacket->SetScaleType( bAutoScaleType ? strFormatTrans : pPacket->GetType());

	// type 쓰기
	int typeRegionY = CVertScaleDrawer::DrawType(pDC, scaleRegion.GetScaleTextRegion(),	strFormatTrans, 
		bround, vertScale.GetVertScaleUnitType(), drawingVertScaleData.GetScaleColor().GetScaleTextColor());

	// scale 쓰기
	// 호가단위, 기준가 사용 - ojtaso (20070917)
	CVertScaleDrawer::DrawTextOrGrid(pDC, drawingVertScaleData.GetDisplayDataMinMax().GetDisplayMin(), 
		typeRegionY, start, end, dGap, bround, drawingVertScaleData, vertScale,
		scaleRegion.GetScaleTextRegion(), CRect(0, 0, 0, 0), dataType, strFormatTrans, true, drawingVertScaleData.GetStandardValue());
}

void CPriceScaleDrawer::DrawTextOrGrid_Both(CDC* pDC, const CDrawingVertScaleData& drawingVertScaleData, const CVertScaleRegion& vertScaleRegion, const CVerticalScale& vertScale, const CString& dataType)
{
	CPacket *pPacket = drawingVertScaleData.GetPacket();
	if( !pPacket) return;

	CRect scaleTextRegion_left = vertScaleRegion.GetLeftScaleTextRegion();
	CRect scaleTextRegion_right = vertScaleRegion.GetRightScaleTextRegion();

	//sy 2002.8.28. --> gap에 따른 packetDataType 수정(지표인 경우)
	CString changeDataType = dataType;
	//if(!drawingVertScaleData.GetGraphName().IsEmpty())
	//	changeDataType = CVertScaleDrawer::GetPacketDataType(dGap);

	int start = 0, end = 0;
	double dGap = 0.0;
	bool bround = false;

	//2007.01.22 Scale사용자 설정,호가단위별
	int totalCount = CVertScaleDrawer::GetCount(pDC, (scaleTextRegion_left.Height())) +1;
	if(totalCount <= 0)
		return;

//	if(!CVertScaleDrawer::GetDrawingEnvironment(pDC, scaleTextRegion_left, 
//		drawingVertScaleData.GetDisplayDataMinMax(), dataType, start, end, dGap, bround))
//		return;

	//{{2007.07.09 by LYH 세팅 되어 있지 않은 경우 원래 Gap계산 방식으로 처리
	// 호가단위, 기준가 사용 - ojtaso (20070917)
	if(!GetPriceDrawingEnvironment(drawingVertScaleData.GetDisplayDataMinMax(), dataType, totalCount,
								dGap,start, end,  bround,vertScale, drawingVertScaleData.GetHogaUnit()))
	{
		if(!CVertScaleDrawer::GetDrawingEnvironment(pDC, scaleTextRegion_left, 
		drawingVertScaleData.GetDisplayDataMinMax(), dataType, start, end, dGap, bround))
		return;
	}
	//}}

	// 절사 할 자리수 구하기 - 정확한 타입 다시 쓰기 12.24.2001
	BOOL bAutoScaleType = pPacket->IsAutoScaleType();
	CString strFormatTrans = CVertScaleDrawer::GetFormatTrans(drawingVertScaleData.GetDisplayDataMinMax(), changeDataType, bAutoScaleType);
	pPacket->SetScaleType( bAutoScaleType ? strFormatTrans : pPacket->GetType());

	// type 쓰기
	int typeRegionY = CVertScaleDrawer::DrawType(pDC, scaleTextRegion_left, strFormatTrans, 
		bround, vertScale.GetVertScaleUnitType(), drawingVertScaleData.GetScaleColor().GetScaleTextColor());
	typeRegionY = CVertScaleDrawer::DrawType(pDC, scaleTextRegion_right, strFormatTrans, 
		bround, vertScale.GetVertScaleUnitType(), drawingVertScaleData.GetScaleColor().GetScaleTextColor());

	// scale 쓰기	
	// 호가단위, 기준가 사용 - ojtaso (20070917)
	CVertScaleDrawer::DrawTextOrGrid(pDC, drawingVertScaleData.GetDisplayDataMinMax().GetDisplayMin(), 
		typeRegionY, start, end, dGap, bround, drawingVertScaleData, vertScale, 
		scaleTextRegion_left, scaleTextRegion_right, dataType, strFormatTrans, true, drawingVertScaleData.GetStandardValue());
}

// (2009/12/8 - Seung-Won, Bae) Comment and Adjusting
// 0. 호가단위, 사용자 갯수 지정을 기본으로 한다.
bool CPriceScaleDrawer::GetPriceDrawingEnvironment( const CDisplayDataMinMax& displayDataMinMax, const CString& strPacketType, 
		const int totalCount, double& dGap, int& start, int& end, bool& bround, const CVerticalScale& vertScale,
		const double dHogaUnit)
{
	ASSERT( 0 < totalCount);

	// 0. Get Default Gap Unit.
	if( dHogaUnit <= 0)	dGap = 0;
	else				dGap = dHogaUnit;

	int nHoGaNumber = ( int)vertScale.GetVertScaleHoGaNumber();
	if( 0 < nHoGaNumber)	dGap = dGap * nHoGaNumber;
	else					dGap = 0;

	// 사용자 단위일때 갯수 지정이 0(자동)이면 최대 갯수를 찍을 수 있는 폭으로 구성한다.
	int nHogaScaleType = vertScale.GetVertUserHogaScaleType();
	int nUserCount = ( int)vertScale.GetVertScaleUserNumber();
	double dScaleMaxGap = displayDataMinMax.GetCount();
	if( 1 != nHogaScaleType || dGap <= 0)
	{
		// vntsorl:[0000268] 차트 환경 설정에서 전일기준 pip으로 선택하면 y축의 pip 대비 데이터가 잘 못 나오는 오류
		if( nHogaScaleType == 0 && nUserCount == 0)
			return false;

		int nCount = nUserCount;
		if( nCount <= 0) nCount = totalCount - 1;
		if( nCount <= 0) return false;

		dGap = dScaleMaxGap / nCount;

		// 호가 정보를 받는 경우 폭을 호가단위로 조정한다.
		double dMinScaleUnit = dHogaUnit;
		// 호가 정보가 없는 경우 Packet Type을 기반으로 한다.
		if( dMinScaleUnit <= 0)
		{
			if( strPacketType.Find( '.') < 0)
			{
				double dDigits = log10( dGap);
				if( dDigits != int( dDigits)) dDigits = int( dDigits);
				dMinScaleUnit = pow( 10, dDigits);
			}
			else if( strPacketType.Find( '/') < 0)
			{
				int nStart = strPacketType.Find( '0');
				if( nStart < 0) return false;
				dMinScaleUnit = atof( strPacketType.Mid( nStart));
			}
			else
			{
				int nBase = 0, nDecLen1 = 0, nDecLen2 = 0;
				if( !CDataConversion::GetBaseFormatInfo( strPacketType, nBase, nDecLen1, nDecLen2))
					return false;
				dMinScaleUnit = ( 1.0 / nBase) / pow( 10, nDecLen2);
			}
		}

		if( dGap <= dMinScaleUnit) dGap = dMinScaleUnit;
		else
		{
			double dHoGaNumber = dGap / dMinScaleUnit;
			// 호가의 배수 의미 자리숫자는 한자리가 되도록 한다.
			int nHoGaDigits = int( log10( dHoGaNumber));
			dHoGaNumber = int( dHoGaNumber / pow( 10, nHoGaDigits)) * pow( 10, nHoGaDigits);
			dGap = dMinScaleUnit * dHoGaNumber;
		}
	}

	// 구해진 단위 Gap을 자동 조정해야 하는 경우 (명확한 사용자 지정이 아닌경우)
	if( dGap <= 0) return false;
	CVertScaleDrawer::GetStartAndEnd( displayDataMinMax, int( dScaleMaxGap / dGap) + 1, dGap, start, end);
	bround = CVertScaleDrawer::GetBround(displayDataMinMax);
	return true;
}
