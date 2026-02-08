// CoordinateCalculator.cpp: implementation of the CCoordinateCalculator class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CoordinateCalculator.h"

#include <assert.h>

#include "../Include_Chart/Dll_Load/IMetaTable.h"						// for _MTEXT()
#include "../Include_Chart/Conversion.h"								// for IPacketListManager
#include "../Include_AddIn_133101/I133101/_IPacketListManager.h"		// for IPacketListManager
#include "../Include_Analysis/CoordinateCalculatorData.h"
#include "Coordinate.h"
#include "AnalysisData.h"
#include "DataMath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CCoordinateCalculator

// 절대 좌표(Absolute) -> 상대 좌표(Relative) : 저장을 위한 좌표
CCoordinate CCoordinateCalculator::AbsoluteToRelative(const CCoordinateCalculatorData& calData, const CPoint& point) const
{
	CCoordinate cdResult;
	if( !calData.IsValidAddInInterface()) return cdResult;

	BOOL IsStringDataType = calData.m_pIPacketListManager->IsBasePacketString();
	if( IsStringDataType)
	{
		IPacket *pIPacket = calData.GetPacket_X();
		if( !IsCalculator( pIPacket)) cdResult = CCoordinate(point);
		else
		{
			int nIndex = CDataConversion::GetPacketDataIndex_Date(calData.GetRegion(), point, 
						calData.GetDisplayDataStart(), calData.GetDisplayDataEnd());
			//nIndex = min(calData.GetDisplayDataEnd()+1,nIndex);
			double dX = 0.0;
			if( pIPacket) pIPacket->GetDataPastandFuture(nIndex, dX);
			double dY = GetData_Y(calData, point, nIndex);
			// 분차트 근접시간 적용 - ojtaso (20080811)
			ILPCSTR szPacketType = pIPacket->GetType();
			CString strPacketType( szPacketType);
			// 20081007 JS.Kim	사용자시간대 선택
			cdResult = CCoordinate(dX, dY, pIPacket->GetBaseTimeDifference(), strPacketType, nIndex); //0
		}
		if( pIPacket) pIPacket->Release();
		return cdResult;
	}

	// XScale : 관련 Interface로 사용 - vntsorl(20070410)
	IPacket *pIPacket = calData.GetPacket_X();
	if( !pIPacket) return cdResult;
	ILPCSTR szPacketType = pIPacket->GetType();
	CString strPacketType( szPacketType);
	// 분차트 근접시간 적용 - ojtaso (20080811)
	// 20081007 JS.Kim	사용자시간대 선택

	// 기존소스.
	//if( !IsCalculator( pIPacket))	
	//	cdResult = CCoordinate(point);
	//else				
	//	cdResult = CCoordinate(GetData_X(calData, point, 0), GetData_Y(calData, point, 0), pIPacket->GetBaseTimeDifference(), strPacketType, 0);
	
	if( !IsCalculator( pIPacket))	
		cdResult = CCoordinate(point);
	else				
	{
		double dDataX = GetData_X(calData, point, 0);
		double dDataY = GetData_Y(calData, point, 0);
		int nTimeDiff = pIPacket->GetBaseTimeDifference();
		cdResult = CCoordinate(dDataX, dDataY, nTimeDiff, strPacketType, 0);
	}
	pIPacket->Release();
	return cdResult;
}

// 절대 좌표(Absolute) -> 상대 좌표(Relative) : 현재 종가점 위치 저장을 위한 좌표
CCoordinate CCoordinateCalculator::AbsoluteToRelativeAutoPrice(const CCoordinateCalculatorData& calData, const CPoint& point) const
{
	CCoordinate cdResult;
	if( !calData.IsValidAddInInterface()) return cdResult;

	if( calData.m_pIPacketListManager->IsBasePacketString())
	{
		double dX = 0.0;
		double dY = 0.0;
		int nIndex = CDataConversion::GetPacketDataIndex_Date(calData.GetRegion(), point, 
			calData.GetDisplayDataStart(), calData.GetDisplayDataEnd());
		
		IPacket *pIPacketX = calData.GetPacket_X();
		if( !IsCalculator( pIPacketX)) 
		{
			cdResult = CCoordinate(point);
			dX = cdResult.GetX();
		}
		else
		{		
			if( pIPacketX) pIPacketX->GetDataPastandFuture(nIndex, dX);			
		}
		if( pIPacketX) pIPacketX->Release();
		
		IPacket *pIPacketY = calData.GetPacket_Y();
		if( !IsCalculator( pIPacketY)) 
		{
			cdResult = CCoordinate(point);
			dY = cdResult.GetY();
		}
		else
		{		
			if( pIPacketY) pIPacketY->GetDataPastandFuture(nIndex, dY);			
		}
		if( pIPacketY) pIPacketY->Release();
		
		cdResult.SetX(dX,0);
		cdResult.SetY(dY);
		return cdResult;
	}

	// XScale : 관련 Interface로 사용 - vntsorl(20070410)
	double dX = 0.0;
	double dY = 0.0;
	int nIndex = calData.m_pIChartManager->FindViewDataIndex_Pt("DEFAULT", point.x);	// DRI로 셋팅되어야 하지 않을까 싶음

	IPacket *pIPacketX = calData.GetPacket_X();
	if( !IsCalculator( pIPacketX)) 
	{
		cdResult = CCoordinate(point);
		dX = cdResult.GetX();
	}
	else
	{		
		dX = GetData_X(calData, point, 0);	
	}
	if( pIPacketX) pIPacketX->Release();

	IPacket *pIPacketY = calData.GetPacket_Y();
	if( !IsCalculator( pIPacketY)) 
	{
		cdResult = CCoordinate(point);
		dY = cdResult.GetY();
	}
	else
	{		
		pIPacketY->GetDataPastandFuture(nIndex, dY);			
	}
	if( pIPacketY) pIPacketY->Release();

	cdResult.SetX(dX,0);
	cdResult.SetY(dY);
	return cdResult;
}

// 상대 좌표 -> 절대 좌표 : 저장된 좌표를 이용해 그리기위한 좌표
CPoint CCoordinateCalculator::RelativeToAbsolute(const CCoordinateCalculatorData& calData, const CCoordinate& point) const
{
	CPoint ptResult;
	if( !calData.IsValidAddInInterface()) return ptResult;

	if( calData.m_pIPacketListManager->IsBasePacketString())
	{
		IPacket *pIPacket = calData.GetPacket_X();
		if( !IsCalculator( pIPacket)) ptResult = point.GetPixel();
		else
		{
			// X-scale 값(자료일자)으로 현재 Display되는 Data의 Index를 얻음
			// 리얼로 인해 Data는 계속 늘어남
			// 분차트 근접시간 적용 - ojtaso (20080811)
			int nIndex = GetDisplayDataCurrentIndex( pIPacket, point.GetX(), point.GetDateType());
			// 미래영역에 있는 point의 경우 실제 Index가 현 point의 Index와 서로 다를 경우의 예외처리
			if(nIndex != point.GetIndex())
			{
				// 최초 리얼시 Y-scale 및 해당 Index 는 항상 값을 갖으나 X-scale 은 0값을 갖음 (미래영역에 대한 값이 없기 때문)
				// 미래영역에 있는 point의 Data Index을 가지고 X-scale 값을 얻음
				if(point.GetX() == 0. && point.GetIndex() > 0)
				{
					double dX = 0.0;
					if( pIPacket) pIPacket->GetDataPastandFuture(point.GetIndex(), dX);	

					nIndex = point.GetIndex();
					double dXPos = point.GetX();
					if(dXPos == 0. || dXPos != dX)
					{
						// const로 셋팅되어 있어서 type casting하여 Data를 실시간으로 변동시킴
						(const_cast<CCoordinate&>(point)).SetX(dX, nIndex);
					}
				}
				// 환경설정에서 날짜 변경시 0, 1, 2값으로 셋팅되는 Index를 다시 보정
				else if(point.GetX() > 0. && (point.GetIndex() == 0 || point.GetIndex() == 1 || point.GetIndex() == 2))
				{
					(const_cast<CCoordinate&>(point)).SetIndex(nIndex);
				}
				// 데이터추가부르기 Button을 이용한 데이터갯수 변경시에 따른
				// X-Scale 값 및 데이터 인덱스 보정
				else if(nIndex > point.GetIndex() + 1)
				{
					double dX = 0.0;
					if( pIPacket) pIPacket->GetDataPastandFuture(nIndex, dX);	

					(const_cast<CCoordinate&>(point)).SetX(dX, nIndex);
				}
				// 현재 진행중인 봉 위에 추세선이 놓여 있는 경우 시간 경과에 따른 
				// X-Scale 값 및 데이터 인덱스 보정
				else
				{
					double dX = 0.0;
					if( pIPacket) pIPacket->GetDataPastandFuture(point.GetIndex(), dX);	

					nIndex = point.GetIndex();
					(const_cast<CCoordinate&>(point)).SetX(dX, nIndex);
				}
			}
			// 저장된 툴의 포지션이 미래영역에 있는 경우
			//2008.07.24 by LYH >> Index 0은 미래영역 아님. 0번째 인덱스에서 추세선등 그리기 오류 수정
			/*
			else if(nIndex == 0 && point.GetIndex() == 0)
			{
				double dX = 0.0;
				if( pIPacket)
				{
					nIndex = pIPacket->GetDataCount() - 1;
					pIPacket->GetDataPastandFuture(nIndex, dX);

					(const_cast<CCoordinate&>(point)).SetX(dX, nIndex);
				}
			}
			*/
			//2008.07.24 by LYH <<

			ptResult = CDataConversion::RelativeToAbsolute(calData.GetRegion(), 
					calData.GetDisplayDataStart(), calData.GetDisplayDataEnd(), 
					calData.GetDisplayDataMin_ScaleType(), calData.GetDisplayDataMax_ScaleType(), 
					nIndex,	GetData_Y(calData, point.GetY(), nIndex));
		}
		if( pIPacket) pIPacket->Release();
		return ptResult;
	}
	
	// XScale : 관련 Interface로 사용 - vntsorl(20070410)
	IPacket *pIPacket = calData.GetPacket_X();
	if( !IsCalculator( pIPacket)) 
		ptResult = point.GetPixel();
	else
	{
//>> 호가차트시 분석도구 종가따라가기 버그 수정 - vntsorl(20090202)
//		ILPCSTR szRQ = pIChartManager->GetMainRQofBlock( calData.GetBlockIndex().GetRow(), calData.GetBlockIndex().GetColumn());
		ILPCSTR szRQ = calData.m_pIChartOCX->GetCurrentRQ();
		int nR = calData.GetBlockIndex().GetRow();
		int nC = calData.GetBlockIndex().GetColumn();
		int nV = 0, nG = -1;
		ILPCSTR szGraphName = calData.m_pIChartManager->FindNextIndicator( NULL, nR, nC, nV, nG, szRQ);
		if( szRQ.IsEmpty() || szGraphName.IsEmpty()
			|| !( nR == calData.GetBlockIndex().GetRow() && nC == calData.GetBlockIndex().GetColumn()))
			szRQ = calData.m_pIChartManager->GetMainRQofBlock( calData.GetBlockIndex().GetRow(), calData.GetBlockIndex().GetColumn());
		int nCenter_x;
		double dX = 0.0;
		dX = point.GetX();
		
		// 2011.01.20 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
		//int nXPos = calData.m_pIChartManager->GetXFromDouble(calData.GetBlockIndex().GetRow(), calData.GetBlockIndex().GetColumn(), dX, nCenter_x);

		CString strAnalType = point.GetDateType();
		ILPCSTR szPacketType = pIPacket->GetType();
		CString strPacketType( szPacketType);

		int nBlockRow = calData.GetBlockIndex().GetRow();
		int nBlockCol = calData.GetBlockIndex().GetColumn();
		int nXPos = calData.m_pIChartManager->GetXFromDouble(nBlockRow, nBlockCol, dX, nCenter_x);
		// 2011.01.20 by SYS <<

		int nIndex = calData.m_pIChartManager->FindViewDataIndex_Pt( szRQ, nXPos);
//<< 호가차트시 분석도구 종가따라가기 버그 수정 - vntsorl(20090202)
		ptResult = CDataConversion::RelativeToAbsolute(calData.GetRegion(), 
				calData.GetDisplayDataStart(), calData.GetDisplayDataEnd(), 
				calData.GetDisplayDataMin_ScaleType(), calData.GetDisplayDataMax_ScaleType(), 
				nIndex,	GetData_Y(calData, point.GetY(), nIndex));
		ptResult.x = nXPos;
	}
	if( pIPacket) pIPacket->Release();
	return ptResult;
}

int CCoordinateCalculator::RelativeToAbsolute_Y(const CCoordinateCalculatorData& calData,int nIndex,const double& dYValue) const
{
	int nYPos = -1;
	IPacket *pIPacket = calData.GetPacket_X();
	if( IsCalculator( pIPacket)) 
	{
		nYPos = CDataConversion::RelativeToAbsolute_Y(calData.GetRegion(), GetData_Y(calData, dYValue, nIndex),
				calData.GetDisplayDataMin_ScaleType(), calData.GetDisplayDataMax_ScaleType());
	}
	if( pIPacket) pIPacket->Release();
	return nYPos;
}

// 절대 좌표 -> 절대 좌표 : 그리기 위한 좌표
CPoint CCoordinateCalculator::AbsoluteToAbsolute(const CCoordinateCalculatorData& calData, const CPoint& point) const
{
	IPacket *pIPacket = calData.GetPacket_X();
	BOOL bResult = IsCalculator( pIPacket);
	if( pIPacket)
	{
		pIPacket->Release();
		pIPacket = NULL;
	}
	if( !bResult) return point;

	CCoordinate relativePt = AbsoluteToRelative(calData, point);
	CPoint drawingPoint = RelativeToAbsolute_NoAutoPrice(calData, relativePt);
	return drawingPoint;
}

// 두점간의 간격
CCoordinate CCoordinateCalculator::GetGap(const CCoordinateCalculatorData& calData, const CPoint& point1, const CPoint& point2) const
{
	CCoordinate cdResult;
	if( !calData.IsValidAddInInterface()) return cdResult;

	IPacket *pIPacket = calData.GetPacket_X();
	ILPCSTR szPacketType = pIPacket->GetType();
	CString strPacketType( szPacketType);
	if( calData.m_pIPacketListManager->IsBasePacketString())
	{
		// 20081007 JS.Kim	사용자시간대 선택
		if( !IsCalculator( pIPacket)) cdResult = CCoordinate((double)(point2.x - point1.x), (double)(point2.y - point1.y), pIPacket->GetBaseTimeDifference(), strPacketType, 0);
		else
		{
			CCoordinate relativePt1 = AbsoluteToRelative(calData, point1);
			CCoordinate relativePt2 = AbsoluteToRelative(calData, point2);
			int nIndex = GetDisplayDataCurrentIndex( pIPacket, relativePt2.GetX())
				- GetDisplayDataCurrentIndex(pIPacket, relativePt1.GetX());
			// 분차트 근접시간 적용 - ojtaso (20080811)
			cdResult = CCoordinate( ( double)nIndex, (relativePt2.GetY() - relativePt1.GetY()), pIPacket->GetBaseTimeDifference(), strPacketType, 0);
		}
	}
	else
	{
		// packet 이 없을 경우엔 그냥 좌표로 한다!!
		// 분차트 근접시간 적용 - ojtaso (20080811)
		// 20081007 JS.Kim	사용자시간대 선택
		if( !IsCalculator( pIPacket)) cdResult = CCoordinate((double)(point2.x - point1.x), (double)(point2.y - point1.y), pIPacket->GetBaseTimeDifference(), strPacketType, 0);
		else
		{
			// XScale : 관련 Interface로 사용 - vntsorl(20070327)
			CCoordinate AbsolutePt1 = AbsoluteToAbsolute(calData, point1);
			CCoordinate AbsolutePt2 = AbsoluteToAbsolute(calData, point2);
			CCoordinate relativePt1 = AbsoluteToRelative(calData, point1);
			CCoordinate relativePt2 = AbsoluteToRelative(calData, point2);
			// 분차트 근접시간 적용 - ojtaso (20080811)
			cdResult = CCoordinate( ( AbsolutePt2.GetX() - AbsolutePt1.GetX()), (relativePt2.GetY() - relativePt1.GetY()), pIPacket->GetBaseTimeDifference(), strPacketType, 0);
		}
	}

	if( pIPacket) pIPacket->Release();
	return cdResult;
}

// 현재 point의 index를 가져오기.
int CCoordinateCalculator::GetCurrentDataIndex(const CRegion& region, const CPoint& point) const
{
	// XScale : 관련 Interface로 사용 - vntsorl(20070327)
	return CDataConversion::GetPacketDataIndex_Date(region.GetRect(), point, 
			region.GetDisplayAttributes().GetDataStartInBlock(), 
			region.GetDisplayAttributes().GetDataEndInBlock());
//	CCoordinateCalculatorData calData;
//	CPoints points = GetPoints(point, calData);
//	return ->FindViewDataIndex_Pt(point.x);
}

// 그릴 x좌표 가져오기.
int CCoordinateCalculator::GetDrawingPoint_X(const CRegion& region, const int nCurrentIndex) const
{
	// XScale : 관련 Interface로 사용 - vntsorl(20070327)
	return CDataConversion::RelativeToAbsolute_X(region.GetRect(), 
			region.GetDisplayAttributes().GetDataStartInBlock(), 
			region.GetDisplayAttributes().GetDataEndInBlock(), nCurrentIndex);
//	CCoordinateCalculatorData calData;
//	CPoints points = GetPoints(point, calData);
//	return ->GetXPosition(nCurrentIndex, 0);
}

// private ====================================================================
bool CCoordinateCalculator::IsCalculator(IPacket* pPacket) const
{
	return (pPacket != NULL && pPacket->GetDataCount() > 0);
}

// ----------------------------------------------------------------------------
// 분차트 근접시간 적용 - ojtaso (20080811)
int CCoordinateCalculator::GetDisplayDataCurrentIndex(IPacket* pPacket, const double dData, const CString strType/* = _T("")*/) const
{
	if(pPacket == NULL)
		return -1;
	
	return pPacket->GetIndexFromData(dData, strType);
}

// 상대 좌표 -> 절대 좌표 : 저장된 좌표를 이용해 그리기위한 좌표
CPoint CCoordinateCalculator::RelativeToAbsolute_NoAutoPrice(const CCoordinateCalculatorData& calData, const CCoordinate& point) const
{
	CPoint ptResult;
	if( !calData.IsValidAddInInterface()) return ptResult;

	IPacket *pIPacket = calData.GetPacket_X();
	if( calData.m_pIPacketListManager->IsBasePacketString())
	{
		if(!IsCalculator( pIPacket)) ptResult = point.GetPixel();
		else
		{
			int nIndex = GetDisplayDataCurrentIndex( pIPacket, point.GetX());
			ptResult = CDataConversion::RelativeToAbsolute(calData.GetRegion(), 
				calData.GetDisplayDataStart(), calData.GetDisplayDataEnd(), 
				calData.GetDisplayDataMin_ScaleType(), calData.GetDisplayDataMax_ScaleType(), 
				nIndex,	calData.ApplyLog(point.GetY()));
		}
	}
	else
	{
		// XScale : 관련 Interface로 사용 - vntsorl(20070410)
		if(!IsCalculator( pIPacket)) ptResult = point.GetPixel();
		else
		{
			int nCenter_x;
			double dX = 0.0;
			dX = point.GetX();
			int nXPos = calData.m_pIChartManager->GetXFromDouble( calData.GetBlockIndex().GetRow(), calData.GetBlockIndex().GetColumn(), dX, nCenter_x);
			int nIndex = calData.m_pIChartManager->FindViewDataIndex_Pt("DEFAULT", nXPos);
			ptResult = CDataConversion::RelativeToAbsolute(calData.GetRegion(), 
					calData.GetDisplayDataStart(), calData.GetDisplayDataEnd(), 
					calData.GetDisplayDataMin_ScaleType(), calData.GetDisplayDataMax_ScaleType(), 
					nIndex,	calData.ApplyLog(point.GetY()));
			ptResult.x = nXPos;
		}
	}

	if( pIPacket) pIPacket->Release();
	return ptResult;
}


///////////////////////////////////////////////////////////////////////////////
// class CCoordinateCalculator_AutoPrice

// protected ==================================================================
// XScale : 관련 Interface로 사용 - vntsorl(20070410)
double CCoordinateCalculator_AutoPrice::GetData_X(const CCoordinateCalculatorData& calData, const CPoint& point, const int /*nIndex*/) const
{
	double dResult = 0.0;
	if( !calData.IsValidAddInInterface()) return dResult;

	int nBlockColumn, nCenter_x;
	int nRow, nCol;
	calData.m_pIChartManager->GetBlockIndex( point, nRow, nCol);
	
//>> 호가차트시 분석도구 종가따라가기 버그 수정 - vntsorl(20090202)
	ILPCSTR szRQ = calData.m_pIChartOCX->GetCurrentRQ();
	int nR = nRow;
	int nC = nCol;
	int nV = 0, nG = -1;
	ILPCSTR szGraphName = calData.m_pIChartManager->FindNextIndicator( NULL, nR, nC, nV, nG, szRQ);
	if( szRQ.IsEmpty() || szGraphName.IsEmpty() || !( nR == nRow && nC == nCol))
		szRQ = calData.m_pIChartManager->GetMainRQofBlock( nRow, nCol);
//	dResult = pIChartManager->GetDoubleFromX(nRow, nCol, point.x, &nBlockColumn, nCenter_x);
	dResult = calData.m_pIChartManager->GetDoubleFromX( nRow, nCol, point.x, &nBlockColumn, nCenter_x, szRQ);
//<< 호가차트시 분석도구 종가따라가기 버그 수정 - vntsorl(20090202)

	return dResult;
}

double CCoordinateCalculator_AutoPrice::GetData_Y(const CCoordinateCalculatorData& calData, const CPoint& point, const int /*nIndex_*/) const
{
	if( !calData.IsValidAddInInterface()) return -HUGE_VAL;

	int nIndex = calData.m_pIChartManager->FindViewDataIndex_Pt( calData.GetRQ(), point.x, calData.GetBlockIndex().GetColumn(), FALSE);
	IPacket *pIPacket = calData.GetPacket_Y();

	double dY = 0.0;
	if( pIPacket && IsAutoPriceCalculator( pIPacket, nIndex))
			pIPacket->GetData( nIndex, dY);
	else	dY = CDataConversion::AbsoluteToRelative_Y( calData.GetRegion(), point.y, 
				calData.GetDisplayDataMin_ScaleType(), calData.GetDisplayDataMax_ScaleType());
	
	if( pIPacket) pIPacket->Release();

	// (2007/5/10 - Seung-Won, Bae) Check No Y Scale Min/Max.
	if( -HUGE_VAL == dY) return -HUGE_VAL;

	return calData.ApplyExp(dY);
}

double CCoordinateCalculator_AutoPrice::GetData_Y(const CCoordinateCalculatorData& calData, const double& dData, const int nIndex) const
{
	IPacket *pIPacket = calData.GetPacket_Y();
	double dY = dData;
	if( pIPacket && IsAutoPriceCalculator( pIPacket, nIndex)) pIPacket->GetData( nIndex, dY);
	if( pIPacket) pIPacket->Release();
	return calData.ApplyLog(dY);
}

// private ====================================================================
bool CCoordinateCalculator_AutoPrice::IsAutoPriceCalculator(IPacket* pPacket, const int nIndex) const
{
	if(pPacket == NULL)
		return false;

	return (nIndex >= 0 && nIndex < pPacket->GetDataCount());
}


///////////////////////////////////////////////////////////////////////////////
// class CCoordinateCalculator_NoAutoPrice

// protected ==================================================================
// XScale : 관련 Interface로 사용 - vntsorl(20070410)
double CCoordinateCalculator_NoAutoPrice::GetData_X(const CCoordinateCalculatorData& calData, const CPoint& point, const int /*nIndex*/) const
{
	int nBlockColumn, nCenter_x;
	int nRow, nCol;
	double dResult = 0.0;
	calData.m_pIChartManager->GetBlockIndex(point, nRow, nCol);
	dResult = calData.m_pIChartManager->GetDoubleFromX(nRow, nCol, point.x, &nBlockColumn, nCenter_x);

	return dResult;
}

double CCoordinateCalculator_NoAutoPrice::GetData_Y(const CCoordinateCalculatorData& calData, const CPoint& point, const int /*nIndex*/) const
{
	double dY = CDataConversion::AbsoluteToRelative_Y(calData.GetRegion(), point.y, 
				calData.GetDisplayDataMin_ScaleType(), calData.GetDisplayDataMax_ScaleType());

	// (2007/5/10 - Seung-Won, Bae) Check No Y Scale Min/Max.
	if( -HUGE_VAL == dY) return -HUGE_VAL;
	
	return calData.ApplyExp(dY);
}

double CCoordinateCalculator_NoAutoPrice::GetData_Y(const CCoordinateCalculatorData& calData, const double& dData, const int nIndex) const
{
	return calData.ApplyLog(dData);
}

///////////////////////////////////////////////////////////////////////////////
// class CCoordinateCalculatorFinder

CCoordinateCalculator_AutoPrice CCoordinateCalculatorFinder::m_autoPriceCal;
CCoordinateCalculator_NoAutoPrice CCoordinateCalculatorFinder::m_noAutoPriceCal;

CCoordinateCalculator& CCoordinateCalculatorFinder::FindCoordinateCalculator(const bool bIsAutoPrice)
{
	if(bIsAutoPrice)
		return m_autoPriceCal;
	return m_noAutoPriceCal;
}


///////////////////////////////////////////////////////////////////////////////
// class CSelectableRegionCalculator


CSelectableRegionCalculator::CSelectableRegionCalculator():
	m_nProximityRange(4)
{
}

// 선택할 영역의 범위
int CSelectableRegionCalculator::GetProximityRange() const
{
	return m_nProximityRange;
}

// 두점을 잇는 선을 두르는 영역.
void CSelectableRegionCalculator::GetLineRegion(const CPoint& point1, const CPoint& point2, CRgn& region) const
{
	// 수평선
	if(point1.y == point2.y)
	{
		CRect temp = GetHorzLineRegion(point1, point2);
		region.CreateRectRgn(temp.left, temp.top, temp.right, temp.bottom);
	}
	// 수직선
	else if(point1.x == point2.x)
	{
		CRect temp = GetVertLineRegion(point1, point2);
		region.CreateRectRgn(temp.left, temp.top, temp.right, temp.bottom);
	}
	// 사선
	else
		GetAngleLineRegion(point1, point2, region);
}

// 원점: origin, 반지름: l인 원에 point가 있는지 검사
bool CSelectableRegionCalculator::IsPointInCircle(const CPoint& origin, const CPoint& point) const
{
	return (pow(m_nProximityRange*2, 2) >= (pow(origin.x - point.x, 2) + pow(origin.y - point.y, 2)));
	//return (pow(m_nProximityRange*2, 3) >= (pow(origin.x - point.x, 3) + pow(origin.y - point.y, 3)));
}

bool CSelectableRegionCalculator::IsPointInCircle(const CPoint& origin, const int nRadius, const CPoint& point) const
{
	return (pow(nRadius, 2) >= (pow(origin.x - point.x, 2) + pow(origin.y - point.y, 2)));
	//return (pow(nRadius, 3) >= (pow(origin.x - point.x, 3) + pow(origin.y - point.y, 3)));
}


// private ====================================================================
// 수평선
CRect CSelectableRegionCalculator::GetHorzLineRegion(const CPoint& point1, const CPoint& point2) const
{
	assert(point1.y == point2.y);

	CRect selectableRegion(point1.x, (point1.y - m_nProximityRange), point2.x, (point1.y + m_nProximityRange +1));
	selectableRegion.NormalizeRect();

	return selectableRegion;
}

// 수직선
CRect CSelectableRegionCalculator::GetVertLineRegion(const CPoint& point1, const CPoint& point2) const
{
	assert(point1.x == point2.x);

	CRect selectableRegion((point1.x - m_nProximityRange), point1.y, (point1.x + m_nProximityRange +1), point2.y);
	selectableRegion.NormalizeRect();

	return selectableRegion;
}

// 사선
void CSelectableRegionCalculator::GetAngleLineRegion(const CPoint& point1, const CPoint& point2, CRgn& region) const
{
	assert(point1.x != point2.x && point1.y != point2.y);

	// (0, 0)을 기준으로 한 ProximityRange 떨어진 두개의 좌표 구하기.
	CPoint basePt1, basePt2;
	GetVerticalPointsFromLength(point1, point2, basePt1, basePt2);

	POINT selectableRegionPoints[4];
	selectableRegionPoints[0] = GetPointOnMovedOrigin(point1, basePt1);
	selectableRegionPoints[1] = GetPointOnMovedOrigin(point1, basePt2);
	selectableRegionPoints[2] = GetPointOnMovedOrigin(point2, basePt2);
	selectableRegionPoints[3] = GetPointOnMovedOrigin(point2, basePt1);

	region.CreatePolygonRgn(selectableRegionPoints, 4, ALTERNATE);
}

// ----------------------------------------------------------------------------
// 좌표 이동
POINT CSelectableRegionCalculator::GetPointOnMovedOrigin(const CPoint& origin, const CPoint& point) const
{
	POINT movePoint;
	movePoint.x = origin.x + point.x;
	movePoint.y = origin.y + point.y;

	return movePoint;
}

// 특정 길이 만큼 떨어진 좌표 구히기
// --> 기울기가 a 이고 (0, 0)을 지나고 (0, 0)에서 특정길이 +- L 떨어지 좌표 구하기.
// --> x1 = k, y1 = a*k,    x2 = -k, y2 = a*-k
// --> k = |(L*L)/(a*a + 1)| 의 루트값
void CSelectableRegionCalculator::GetVerticalPointsFromLength(const CPoint& pt1, const CPoint& pt2, CPoint& selectableRegionPt1, CPoint& selectableRegionPt2) const
{
	// 두점의 수직기울기 구한다. == 현재 구할 두 좌표의 기울기
	double lineAngle = GetLineVerticalAngle(GetLineAngle(pt1, pt2));

	double x = 0.0;
	x = sqrt(fabs(pow(m_nProximityRange, 2) / (pow(lineAngle, 2) +1)));

	selectableRegionPt1 = CPoint(CMath::Round(x), CMath::Round(lineAngle * x));
	selectableRegionPt2 = CPoint(-selectableRegionPt1.x, -selectableRegionPt1.y);
}

// ----------------------------------------------------------------------------
// 좌표의 기울기.
// --> 두점의 기울기 : (y2-y1)/(x2-x1)
double CSelectableRegionCalculator::GetLineAngle(const CPoint& point1, const CPoint& point2) const
{
	return ((double)(point2.y - point1.y) / (double)(point2.x - point1.x));
}

// 수직의 기울기.
// --> -1/a
double CSelectableRegionCalculator::GetLineVerticalAngle(const double& dAngle) const
{
	assert(dAngle != 0.0);
	return (-1.0/dAngle);
}
