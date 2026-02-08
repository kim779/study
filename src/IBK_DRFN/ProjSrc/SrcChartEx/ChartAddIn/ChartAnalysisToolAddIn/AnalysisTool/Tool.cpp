// Tool.cpp: implementation of the CTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tool.h"

#include "../Include_Analysis/CoordinateCalculatorData.h"
#include "../Include_Analysis/ElementEnvironment.h"

#include "Coordinate.h"
#include "ElementFactory.h"
#include "CoordinateCalculator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CTool

CTool::CTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType):
	m_pAnalysisData(pAnalysisData),
	m_bProcessStarting(false)
{
	IChartOCX* pIChartOCX = NULL;
	if(m_pAnalysisData)
	{
		pIChartOCX = m_pAnalysisData->GetChartOCX();
	}
	SetOcxHwnd(pIChartOCX ? pIChartOCX->GetOcxHwnd() : NULL);
	Initialize();
	SetCoordinateType(eCoordinateType);
}

// public =====================================================================
void CTool::SetAnalysisData(CAnalysisData* pAnalysisData)
{
	m_pAnalysisData = pAnalysisData;
}

void CTool::SetCoordinateType(const CAnalysis::COORDINATETYPE eCoordinateType)
{
	m_commonEnvironment.GetDataType().SetCoordinateType(eCoordinateType);
	if(!IsRelativePointType())
		m_commonEnvironment.GetDataType().Reset_AbsolutePoint();
}

void CTool::SetProcessStarting(const bool bStarting)
{
	m_bProcessStarting = bStarting;
}

void CTool::SetAutoPrice(const bool bIsAutoPrice)
{
	m_commonEnvironment.SetAutoPrice(bIsAutoPrice);
}

void CTool::SetPenColor(const COLORREF& color)
{
	m_commonEnvironment.GetPen().SetColor(color);
}

void CTool::SetTextColor(const COLORREF& color)
{
	CTextPosition* pTextPosition = GetTextPosition();
	if(pTextPosition == NULL)
		return;

	pTextPosition->GetTextEnvironment().SetColor(color);
}

void CTool::SetLineExt(const int& nLeft,const int& nRight)
{
	m_commonEnvironment.SetLineExt(nLeft,nRight);
}
// ----------------------------------------------------------------------------
// 그리기 시작 유무.
bool CTool::DoesProcessStarting() const
{
	return m_bProcessStarting;
}

bool CTool::DoesElements() const
{
	if(m_pAnalysisData == NULL)
		return false;

	return (m_pAnalysisData->GetElementTree().GetElementCount() > 0);
}

CElementCommonEnvironment& CTool::GetElementCommonEnvironment()
{
	return m_commonEnvironment;
}

// ----------------------------------------------------------------------------
void CTool::Invalidate()
{
	if(m_pAnalysisData == NULL)
		return;

	m_pAnalysisData->Invalidate();
}

// ----------------------------------------------------------------------------
// data 의 초기화n
void CTool::Initialize()
{
	ChangeCursor(CToolEnumData::MODIFY_NONE);
	m_bProcessStarting = false;
	m_drawingRegions.Initialize();
}

void CTool::OnLButtonUp(CDC* pDC, const CPoint& point)
{
	// 그리는 영역안에 있을 경우만 object한다.
	if(IsMakeElement(point)){
		CPoints points = GetPoints(point);
		SavePoints(points);
		MakeElement(points.GetDrawingPoint());
	}

	Initialize();
}

void CTool::OnRButtonUp(CDC* pDC, const CPoint& point)
{
	Initialize();
}

// protected ==================================================================
bool CTool::PointInDrawingRegion(const CPoint& point) const
{
	if(GetDrawingRegions().GetCount() <= 0)
		return false;

	return (GetDrawingRegions().PointInCurrentRegion(point));
}

const CTextPosition* CTool::GetTextPosition() const
{
	return NULL;
}

CTextPosition* CTool::GetTextPosition()
{
	return NULL;
}

CCoordinateCalculator& CTool::GetCoordinateCalculator() const
{
	return CCoordinateCalculatorFinder::FindCoordinateCalculator(IsAutoPrice());
}

bool CTool::IsMakeElement(const CPoint& point) const
{
	return PointInDrawingRegion(point);
}

CElement* CTool::MakeElement(const CElementCommonEnvironment& /*environment*/)
{
	return NULL;
}

// ----------------------------------------------------------------------------
bool CTool::IsRelativePointType() const
{
	return m_commonEnvironment.GetDataType().IsRelativePointType();
}

bool CTool::IsAutoPrice() const
{
	return m_commonEnvironment.IsAutoPrice();
}

CAnalysisData* CTool::GetAnalysisData() const
{
	return m_pAnalysisData;
}

const CDrawingRegions& CTool::GetDrawingRegions() const
{
	return m_drawingRegions;
}

CDrawingRegions& CTool::GetDrawingRegions()
{
	return m_drawingRegions;
}

CElementCommonEnvironment CTool::GetElementCommonEnvironment() const
{
	return m_commonEnvironment;
}

// ----------------------------------------------------------------------------
CDisplayAttributes CTool::GetDisplayAttributes() const
{
	if(m_pAnalysisData == NULL)
		return CDisplayAttributes();

	return m_pAnalysisData->GetDisplayAttributes();
}

CVertScaleType CTool::GetVertScaleType(const CPoint& point) const
{
	if(IsNullMainBlock())
		return CVertScaleType();

	bool bIsLog = false, bIsInvert = false;
	m_pAnalysisData->GetVertScaleType(point, bIsLog, bIsInvert);
	return CVertScaleType(bIsLog, bIsInvert);
}

// ----------------------------------------------------------------------------
CPoints CTool::GetPoints(const CPoint& point) const
{
	IChartManager *pIChartManager = NULL;
	if( m_pAnalysisData) pIChartManager = m_pAnalysisData->GetMainBlock();
	CCoordinateCalculatorData calData( pIChartManager);
	pIChartManager->Release();

	return GetPoints(point, calData);
}

CCoordinate CTool::GetPosCoordinate(const CPoint& point) const
{	
	if(!IsRelativePointType())
		return CCoordinate(point);

	CCoordinateCalculatorData calData = GetCoordinateCalculatorData(point);
	CCoordinateCalculator& coordinateCalculator = GetCoordinateCalculator();
	return coordinateCalculator.AbsoluteToRelativeAutoPrice(calData, point);
}

CPoints CTool::GetPointsAndSetDataType(const CPoint& point)
{
	IChartManager *pIChartManager = NULL;
	if( m_pAnalysisData) pIChartManager = m_pAnalysisData->GetMainBlock();
	CCoordinateCalculatorData calData( pIChartManager);
	pIChartManager->Release();

	CPoints points = GetPoints(point, calData);
	SetDataTypeEnvironment(calData);

	return points;
}

CPoint CTool::GetDrawingPointAndSetDataType(const CPoint& point)
{
	IChartManager *pIChartManager = NULL;
	if( m_pAnalysisData) pIChartManager = m_pAnalysisData->GetMainBlock();
	CCoordinateCalculatorData calData( pIChartManager);
	pIChartManager->Release();

	CPoint drawingPoint = GetDrawingPoint(point, calData);
	SetDataTypeEnvironment(calData);

	IPacket *pIPacket = calData.GetPacket_X();
	if( pIPacket) pIPacket->Release();

	return drawingPoint;
}

CPoints CTool::RefreshPointsAndSetDataType(const CPoint& point)
{
	CCoordinateCalculatorData calData = GetCoordinateCalculatorData(point);
	CCoordinateCalculator& coordinateCalculator = GetCoordinateCalculator();
	CCoordinate savingPoint = GetCoordinateCalculator().AbsoluteToRelative(calData, point);
	CPoint drawingPoint = GetCoordinateCalculator().RelativeToAbsolute(calData, savingPoint);
	
	return CPoints(savingPoint, drawingPoint);	
}

// 좌표 계산을 위한 data.
CCoordinateCalculatorData CTool::GetCoordinateCalculatorData(const CPoint& point) const
{
	if( IsNullMainBlock()) return CCoordinateCalculatorData( NULL);
	return m_pAnalysisData->GetCoordinateCalculatorData(point);
}

// ----------------------------------------------------------------------------
// 그릴 영역들을 바꾸기.
void CTool::SetDrawingRegions(const CPoint& point)
{
	if(IsNullMainBlock())
		return;

	IChartManager *pIChartManager = m_pAnalysisData->GetMainBlock();
	if( !pIChartManager) return;
	m_drawingRegions.SetDrawingRegions( pIChartManager, point);
	pIChartManager->Release();
	pIChartManager = NULL;
}

// 커서 바꾸기.
void CTool::ChangeCursor(const CToolEnumData::ModifyType type)
{
	if(m_pAnalysisData == NULL || m_pAnalysisData->GetParent() == NULL)
		return;

	switch(type){
	case CToolEnumData::MODIFY_NONE:
		m_pAnalysisData->SendMessageToParent(CCursorShape::OBJECT_NONE);
		break;
	case CToolEnumData::MODIFY_CENTER:
		m_pAnalysisData->SendMessageToParent(CCursorShape::TOOL_SIZEALL);
		break;
	case CToolEnumData::MODIFY_START:
	case CToolEnumData::MODIFY_MIDDLE:
	case CToolEnumData::MODIFY_END:
	case CToolEnumData::MODIFY_X:
	case CToolEnumData::MODIFY_Y:
		m_pAnalysisData->SendMessageToParent(CCursorShape::TOOL_SIZENESW);
		break;
	}
}

void CTool::ChangeCursor(const CToolEnumData::DrawType type)
{
	if(m_pAnalysisData == NULL || m_pAnalysisData->GetParent() == NULL)
		return;

	switch(type){
	case CToolEnumData::DRAW_NONE:
		m_pAnalysisData->SendMessageToParent(CCursorShape::OBJECT_NONE);
		break;
	case CToolEnumData::DRAW_PEN:
		m_pAnalysisData->SendMessageToParent(CCursorShape::TOOL_DRAW);
		break;
	case CToolEnumData::DRAW_NODROP:
		m_pAnalysisData->SendMessageToParent(CCursorShape::TOOL_NODROP);
		break;
	}
}

// ----------------------------------------------------------------------------

void CTool::MakeElement(const CPoint& drawingPoint)
{
	if(IsNullMainBlock())
		return;

	CBlockIndex blockIndex = m_pAnalysisData->GetBlockIndexInPoint(drawingPoint);
	if(!blockIndex.IsAllValueBiggerThanZero())
		return;
 
	CElement *pElement = MakeElement(m_commonEnvironment);
//>> 호가차트시 분석도구 종가따라가기 버그 수정 - vntsorl(20090202)
	if( pElement->GetElementType() != CToolEnumData::Linear_Regression &&
		pElement->GetElementType() != CToolEnumData::Raff_Regression )
		pElement->GetCommonEnvironment().SetAutoPrice(false);
//<< 호가차트시 분석도구 종가따라가기 버그 수정 - vntsorl(20090202)
	m_pAnalysisData->RegisterElementToOrderManager((long)pElement);
	m_pAnalysisData->GetElementTree().Add(blockIndex,pElement);	
	
	// Undo / Redo
	m_pAnalysisData->GetUndoRedoManager().RegisterElementToUnRedoManager(URT_NEW_OR_DEL, (long)pElement);
}

// private ====================================================================
// MainBlock이 존재하는지의 유무
bool CTool::IsNullMainBlock() const
{
	if( !m_pAnalysisData) return true;

	IChartManager *pIChartManager = m_pAnalysisData->GetMainBlock();
	if( !pIChartManager) return true;
	pIChartManager->Release();
	pIChartManager = NULL;
	return false;
}

CPoints CTool::GetPoints(const CPoint& point, CCoordinateCalculatorData& calData) const
{
	if(!IsRelativePointType())
		return CPoints(CCoordinate(point), point);

	calData = GetCoordinateCalculatorData(point);
	CCoordinateCalculator& coordinateCalculator = GetCoordinateCalculator();
	CCoordinate savingPoint = coordinateCalculator.AbsoluteToRelative(calData, point);
	CPoint drawingPoint = coordinateCalculator.RelativeToAbsolute(calData, savingPoint);
	//CPoint drawingPoint = point;
	return CPoints(savingPoint, drawingPoint);
}

CPoint CTool::GetDrawingPoint(const CPoint& point, CCoordinateCalculatorData& calData) const
{
	if(!IsRelativePointType())
		return point;

	calData = GetCoordinateCalculatorData(point);
	return GetCoordinateCalculator().AbsoluteToAbsolute(calData, point);
}

// ----------------------------------------------------------------------------
void CTool::SetDataTypeEnvironment(const CCoordinateCalculatorData& calData)
{
	if(!IsRelativePointType())
	{
		m_commonEnvironment.GetDataType().Reset_AbsolutePoint();
	}
	else
	{
		m_commonEnvironment.GetDataType().SetVert(calData.GetVertDataType());
		IPacket *pIPacket = calData.GetPacket_X();
		if( pIPacket)
		{
			ILPCSTR szPacketType = pIPacket->GetType();
			CString strPacketType( szPacketType);
			m_commonEnvironment.GetDataType().SetHorz( strPacketType);
			pIPacket->Release();
		}
// 2008.10.15 by LYH >> 8,32bit지원
		IPacket *pIPacketY = calData.GetPacket_Y();
		if( pIPacketY)
		{
			ILPCSTR szPacketType = pIPacketY->GetType( false);
			m_commonEnvironment.GetDataType().SetStrVert( CString( szPacketType));
			pIPacketY->Release();
		}
// 2008.10.15 by LYH <<
	}
}
