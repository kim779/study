// Selecting.cpp: implementation of the CSelecting class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Selecting.h"

#include "../Include_Analysis/CoordinateCalculatorData.h"

#include "Conversion.h"
#include "PointElement.h"
#include "AnalysisData.h"
#include "CoordinateCalculator.h"
#include "Element.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CSelectingTool

CSelectingTool::CSelectingTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType):
	CTool(pAnalysisData, eCoordinateType),
	m_TypeModify(CToolEnumData::MODIFY_NONE),
	m_pSelectedElement(NULL)
{
	Initialize();
}

// public =====================================================================
// data 초기화
void CSelectingTool::Initialize()
{
	CTool::Initialize();
	CTool::SetProcessStarting(true);
	m_TypeModify = CToolEnumData::MODIFY_NONE;
	m_pSelectedElement = NULL;
	m_bOldSelectedPtUsed = false;
	m_lBtDownPoint = CPoint(-1,-1);
}

// ----------------------------------------------------------------------------
CElement* CSelectingTool::OnLButtonDown(CDC* pDC, const CPoint& point)
{
	CTool::SetDrawingRegions(point);
	m_lBtDownPoint = point;
	m_pSelectedElement = FindElement(point, m_TypeModify);
	//m_pSelectedElement = FindElement(point, m_TypeModify);
	
	if(m_pSelectedElement != NULL)
	{
		m_oldSelectedPt = CTool::GetPoints(point);
		m_pSelectedElement->DrawShadowAndGrip(pDC, CTool::GetDrawingRegions(), CTool::GetCoordinateCalculatorData(point));
	}

	return m_pSelectedElement;
}

void CSelectingTool::OnLButtonUp(CDC* pDC, const CPoint& point)
{
	if(!IsSelectedElement())
		return;

	BOOL bChanged = FALSE;
	CPoint drawingPoint = GetDrawingPoint(point);
	CString strElementData_Prev;
	// 그리는 영역안에 있을 경우만 object한다.
	if(m_lBtDownPoint != point && PointInDrawingRegion(drawingPoint))
	{ 
		bChanged = TRUE;
		CElementModifyData modifyData = GetElementModifyData(drawingPoint);
		
		strElementData_Prev = m_pSelectedElement->GetSavedElementData(CTool::GetAnalysisData()->GetElementTree().GetOrder());
		if( m_pSelectedElement->GetElementType() == CAnalysis::EN_ANALYSIS_CANDLE_BONG)
			m_pSelectedElement->ElementModify_YValue(modifyData);
		else
			m_pSelectedElement->ElementModify(modifyData);

		// Undo / Redo
		if( m_pSelectedElement->GetElementType() == CAnalysis::EN_ANALYSIS_PENCIL_LINE)
			bChanged = FALSE;

		if( m_pSelectedElement->GetElementType() == CAnalysis::EN_ANALYSIS_LINEAR_REGRESSION ||
			m_pSelectedElement->GetElementType() == CAnalysis::EN_ANALYSIS_RAFF_REGRESSION) 
		{
			int nIndex1 = modifyData.GetPacket_XIndex(m_pSelectedElement->GetCoordinate(CToolEnumData::MODIFY_START).GetX());
			int nIndex2 = modifyData.GetPacket_XIndex(m_pSelectedElement->GetCoordinate(CToolEnumData::MODIFY_END).GetX());
			
			if(abs(nIndex2 - nIndex1) > 1)
			{
				IPacketManager* pPacketManager = GetAnalysisData()->GetPacketManager();
				if( !pPacketManager) return;
				IPacket* pPacket = pPacketManager->GetPacket( _MTEXT( C0_CLOSE));
				pPacketManager->Release();

				double dXAvg = 0.0f;
				double dYAvg = 0.0f;
				POSITION pos = NULL;
				double dYValue = -1.0;
				int nStartIdx = min(nIndex1,nIndex2);
				int nEndIdx = max(nIndex1,nIndex2);
				int nCount = nEndIdx - nStartIdx + 1;	
				register int i = 0;
				for( i=nStartIdx; i<=nEndIdx; i++)
				{
					dXAvg += i;
					if( !pPacket->GetData( i, dYValue)) break;
					dYAvg += dYValue;
				}
				dXAvg /= nCount;
				dYAvg /= nCount;

				double dSumD = 0.0f;
				double dSumN = 0.0f;
				for(i=nStartIdx; i<=nEndIdx; i++)
				{
					if( !pPacket->GetData( i, dYValue)) break;

					dSumD += (i-dXAvg) * (dYValue-dYAvg);
					dSumN += (i-dXAvg) * (i-dXAvg);
				}

				double dBeta = dSumD / dSumN;
				double dAlpha = dYAvg - dBeta * dXAvg;

				double dFirstVal = dAlpha + dBeta * nStartIdx;
				double dSecondVal = dAlpha + dBeta * nEndIdx;

				if(pPacket) 
					pPacket->Release();

				CCoordinate coordMoving;
				coordMoving.SetX(dFirstVal,-1);
				coordMoving.SetY(dSecondVal);
				m_pSelectedElement->ElementModify_Coordinate(coordMoving,modifyData.GetModifyType());
			}
		}
		else if( m_pSelectedElement->GetElementType() == CAnalysis::EN_ANALYSIS_CANDLE_LINE)
		{	
			double dXValue = m_pSelectedElement->GetCoordinate(CToolEnumData::MODIFY_START).GetX();
			int	nIndex = modifyData.GetPacket_XIndex(dXValue);			
			if(nIndex < 0)
				return;

			IPacketManager* pPacketManager = GetAnalysisData()->GetPacketManager();
			if( !pPacketManager) return;
			IPacket* pPacketClose = pPacketManager->GetPacket( _MTEXT( C0_CLOSE));
			IPacket* pPacketOpen = pPacketManager->GetPacket( _MTEXT( C0_OPEN));
			pPacketManager->Release();
		
			double dCloseYValue = 0.0;
			double dOpenYValue = 0.0;
			if( pPacketClose && pPacketOpen)
			{
				pPacketClose->GetData( nIndex, dCloseYValue);
				pPacketOpen->GetData( nIndex, dOpenYValue);
			}
			if( pPacketClose) pPacketClose->Release();	
			if( pPacketOpen) pPacketOpen->Release();	
			
			double dYValue = min(dCloseYValue,dOpenYValue);
			dYValue+= fabs(dCloseYValue - dOpenYValue)/2;
			CCoordinate coordMoving;
			coordMoving.SetX(dXValue,-1);
			coordMoving.SetY(dYValue);
			m_pSelectedElement->ElementModify_Coordinate(coordMoving,modifyData.GetModifyType());
			
		}		
	}	

	if(bChanged)
		CTool::GetAnalysisData()->MoveElement(m_pSelectedElement, strElementData_Prev);
	Initialize();
}

bool CSelectingTool::OnMouseMove(CDC* pDC, const CPoint& point,int nFlags)
{
	if(nFlags != MK_LBUTTON)
		return false;

	//m_ptOldMouse = point;
	// element을 찾기 -> 현재 선택된 element가 없을 경우.
	if(!IsSelectedElement()){
		CTool::ChangeCursor(CToolEnumData::MODIFY_NONE);
		return FindElement(point);
	}
	
	/*if( m_pSelectedElement->GetElementType() == CAnalysis::EN_ANALYSIS_CANDLE_BONG)
	{
		CElementModifyData modifyData = GetElementModifyData(point);
		switch(m_TypeModify){
		case CToolEnumData::MODIFY_START:
		case CToolEnumData::MODIFY_END:
			m_pSelectedElement->ElementModify_YValue(modifyData);
			break;
		}
	}*/

	// 선택된 element 수정
	if(ModifyElement(pDC, point)){
		CTool::ChangeCursor(m_TypeModify);
		return true;
	}

	CTool::ChangeCursor(CToolEnumData::DRAW_NODROP);
	return false;
}

void CSelectingTool::OnDraw(CDC* pDC)
{
	if(m_pSelectedElement == NULL) 
		return;

	if(!IsSelectedElement()){
		CTool::GetDrawingRegions();
		return;
	}

	//Draw_EndPoint(pDC);
}

// protected ==================================================================
CAnalysis::TOOLTYPE CSelectingTool::GetToolType() const
{
	return CAnalysis::EN_ANALYSIS_SELECTION;
}

/*// 그리는 영역들에 속한 point 인지의 여부.
bool CSelectingTool::PointInDrawingRegion(const CPoint& point) const
{
	return (CTool::GetDrawingRegions().GetCurrentRegion().PtInRect(point));
}*/

// 저장하기
void CSelectingTool::SavePoints(const CPoints& points)
{
	m_oldSelectedPt = points;
}

// private ====================================================================
// 현재 선택된 element가 있는지의 여부
bool CSelectingTool::IsSelectedElement() const
{
	return IsFindedModifyElement(m_pSelectedElement, m_TypeModify);
}

bool CSelectingTool::IsRelativePointType() const
{
	if(!IsSelectedElement())
		return false;

	return (m_pSelectedElement->GetCoordinateType() == CAnalysis::RELATIVE_POINT);
}

// 수정할 element를 찾은 경우 
bool CSelectingTool::IsFindedModifyElement(CElement* pElement, const CToolEnumData::ModifyType modifyType) const
{
	return (pElement != NULL && modifyType != CToolEnumData::MODIFY_NONE);
}

// ----------------------------------------------------------------------------
// 좌표
CPoint CSelectingTool::GetDrawingPoint(const CPoint& point) const
{
	if(!IsRelativePointType())
		return point;

	CCoordinateCalculatorData calData = CTool::GetCoordinateCalculatorData(point);
	return CTool::GetCoordinateCalculator().AbsoluteToAbsolute(calData, point);
}

CCoordinate CSelectingTool::GetSavingPoint(const CPoint& point) const
{
	if(!IsRelativePointType())
		return CCoordinate(point);

	CCoordinateCalculatorData calData = CTool::GetCoordinateCalculatorData(point);
	return CTool::GetCoordinateCalculator().AbsoluteToRelative(calData, point);	
}

CPoint CSelectingTool::GetSelectedElementDrawPoint(const CCoordinateCalculatorData& calData, const CPoint& point) const
{
	assert(m_pSelectedElement != NULL);

	CPoint lBtDownDrawingPoint = m_lBtDownPoint;
	if(IsRelativePointType())
		lBtDownDrawingPoint = GetDrawingPoint(m_lBtDownPoint);
	
	if(m_TypeModify != CToolEnumData::MODIFY_CENTER)
		return point;

	return CPoint((point.x - lBtDownDrawingPoint.x), (point.y - lBtDownDrawingPoint.y));
}

// ----------------------------------------------------------------------------
// 선택된 element를 그리기 위해 필요한 data
//>> (2008/12/16 - Seung-Lyong Park)
CSelectedElementDrawingData CSelectingTool::GetSelectedElementDrawData()
{
	assert(m_pSelectedElement != NULL);

	CCoordinateCalculatorData calData = CTool::GetCoordinateCalculatorData(m_oldSelectedPt.GetDrawingPoint());
	return CSelectedElementDrawingData(m_TypeModify,&CTool::GetDrawingRegions(), calData, 
										CPoints(m_oldSelectedPt.GetSavingPoint(),GetSelectedElementDrawPoint(calData, m_oldSelectedPt.GetDrawingPoint())));
}
//<< (2008/12/16 - Seung-Lyong Park)

CSelectedElementDrawingData CSelectingTool::GetSelectedElementDrawData(const CPoints& points)
{
	assert(m_pSelectedElement != NULL);

	CCoordinateCalculatorData calData = CTool::GetCoordinateCalculatorData(points.GetDrawingPoint());
	return CSelectedElementDrawingData(m_TypeModify,&CTool::GetDrawingRegions(), calData, 
										CPoints(points.GetSavingPoint(),GetSelectedElementDrawPoint(calData, points.GetDrawingPoint())));
}

// element를 modify 할 때 필요한 data
CElementModifyData CSelectingTool::GetElementModifyData(const CPoint& point) const
{
	CElementModifyData elementModifyData;
	elementModifyData.SetModifyType(m_TypeModify);
	IChartManager *pIChartManager = GetAnalysisData()->GetMainBlock();
	elementModifyData.SetIChartManager( pIChartManager);
	if( pIChartManager) pIChartManager->Release();
	pIChartManager = NULL;
	elementModifyData.SetBlockIndex(point);

	if(IsRelativePointType())
		GetElementModifyData_Data(point, elementModifyData);
	else
		GetElementModifyData_Pixel(point, elementModifyData);

	return elementModifyData;
}

void CSelectingTool::GetElementModifyData_Data(const CPoint& point, CElementModifyData& elementModifyData) const
{
	if(m_TypeModify == CToolEnumData::MODIFY_CENTER)
	{
		CCoordinateCalculatorData calData = CTool::GetCoordinateCalculatorData(point);
		IPacket *pIPacket = calData.GetPacket_X();
		elementModifyData.SetPacket_X( pIPacket);
		if( pIPacket) pIPacket->Release();
		elementModifyData.SetPoint(CTool::GetCoordinateCalculator().GetGap(calData, m_lBtDownPoint, point));
	}
	else
	{
		if( m_pSelectedElement->GetElementType() == CAnalysis::EN_ANALYSIS_CANDLE_LINE ||
			m_pSelectedElement->GetElementType() == CAnalysis::EN_ANALYSIS_LINEAR_REGRESSION ||
			m_pSelectedElement->GetElementType() == CAnalysis::EN_ANALYSIS_RAFF_REGRESSION)
		{
			CCoordinateCalculatorData calData = CTool::GetCoordinateCalculatorData(point);
			IPacket *pIPacket = calData.GetPacket_X();
			elementModifyData.SetPacket_X( pIPacket);
			if( pIPacket) pIPacket->Release();
		}		
		elementModifyData.SetPoint(GetSavingPoint(point));
	}
}

void CSelectingTool::GetElementModifyData_Pixel(const CPoint& point, CElementModifyData& elementModifyData) const
{
	if(m_TypeModify == CToolEnumData::MODIFY_CENTER)
		// 분차트 근접시간 적용 - ojtaso (20080811)
		// 20081007 JS.Kim	사용자시간대 선택
		elementModifyData.SetPoint(CCoordinate((double)(point.x - m_lBtDownPoint.x), (double)(point.y - m_lBtDownPoint.y), 0, "", 0));
	else
		elementModifyData.SetPoint(GetSavingPoint(point));
}

// ----------------------------------------------------------------------------
// element를 찾을지의 여부.
bool CSelectingTool::IsFindElement()
{
	return (CTool::GetAnalysisData() != NULL && CTool::GetDrawingRegions().GetCount() > 0);
}

// element 찾기
bool CSelectingTool::FindElement(const CPoint& point)
{
	if(!CTool::DoesElements())
		return false;

	CTool::SetDrawingRegions(point);
	CToolEnumData::ModifyType modifyType = CToolEnumData::MODIFY_NONE;
	CElement* pElement = FindElement(point, modifyType);
	if(!IsFindedModifyElement(pElement, modifyType))
		return false;

	CTool::ChangeCursor(modifyType);
	return true;
}

CElement* CSelectingTool::FindElement(const CPoint& point, CToolEnumData::ModifyType& modifyType)
{
	if(!IsFindElement())
		return NULL;

	IChartManager *pIChartManager = CTool::GetAnalysisData()->GetMainBlock();
	if( !pIChartManager) 
		return NULL;
	CElement* pElement = CTool::GetAnalysisData()->GetElementTree().FindElement( pIChartManager,
		&CExtractingDataOfModifyType( &CTool::GetDrawingRegions(), point, pIChartManager), modifyType);
	pIChartManager->Release();
	pIChartManager = NULL;

	if(pElement != NULL)
		CTool::SetCoordinateType(pElement->GetCoordinateType());

	return pElement;
}

// ----------------------------------------------------------------------------
bool CSelectingTool::ModifyElement(CDC* pDC, const CPoint& point)
{
	switch(m_TypeModify){
	case CToolEnumData::MODIFY_CENTER:
	case CToolEnumData::MODIFY_START:
	case CToolEnumData::MODIFY_MIDDLE:
	case CToolEnumData::MODIFY_END:
	case CToolEnumData::MODIFY_X:
	case CToolEnumData::MODIFY_Y:
		return Modify_InDrawingRegion(pDC, point);
	}

	assert(false);
	return false;
}

// 그리는 영역안에서만 element 수정
bool CSelectingTool::Modify_InDrawingRegion(CDC* pDC, const CPoint& point)
{
	if(!PointInDrawingRegion(point))
		return false;
    
	CPoints points = CTool::GetPoints(point);
	DrawNewToolDrawing(pDC,points);
	return true;
}

// ----------------------------------------------------------------------------
// 이전 그림을 지우며 새로 그리기.
void CSelectingTool::DrawNewToolDrawing(CDC* pDC, const CPoints& points)
{
	SavePoints(points);
	Draw_EndPoint(pDC);
 
//	CElementModifyData modifyData = GetElementModifyData(m_oldSelectedPt.GetDrawingPoint());
//	m_pSelectedElement->ElementModify(modifyData);

	//CTool::Invalidate();
}

void CSelectingTool::Draw_EndPoint(CDC* pDC)
{
	m_pSelectedElement->DrawSelected(pDC, GetSelectedElementDrawData(m_oldSelectedPt));
}

// ----------------------------------------------------------------------------

//>> (2008/12/16 - Seung-Lyong Park) Set Position of Selected Element.
void CSelectingTool::SetBtDownPoint(const CPoint& point)
{
	m_lBtDownPoint = point;
	if(m_pSelectedElement != NULL)
	{
		m_oldSelectedPt = CTool::GetPoints(point);
	}
}
//<< (2008/12/16 - Seung-Lyong Park) Set Position of Selected Element.
