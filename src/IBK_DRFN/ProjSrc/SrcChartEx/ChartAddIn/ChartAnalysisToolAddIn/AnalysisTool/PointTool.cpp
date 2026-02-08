// PointTool.cpp: implementation of the CPointTool class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PointTool.h"

#include "../Include_Chart/Conversion.h"
#include "../Include_Chart/DataMath.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"
#include "../Include_AddIn_133101/I133101/_IDoubleList.h"				// for IDoubleList
#include "../Include_AddIn_133101/I133101/_IPacketListManager.h"		// for IPacketListManager

#include "Coordinate.h"
#include "ElementFactory.h"
#include "DrawingData.h"
#include "PointDrawer.h"
#include "CoordinateCalculator.h"
#include "DlgAnalysisAdjustRatio.h"
#include "PointElement.h"				// for CTwoPointElement
#include "DlgAnalysisToolSet.h"			// for CDlgAnalysisToolSet

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CMoveTool

CMoveTool::CMoveTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType):
	CTool(pAnalysisData, eCoordinateType)
{
	m_ptMousePos = CPoint(-1,-1);
	Initialize();	
}

// public =====================================================================
// data 초기화
void CMoveTool::Initialize()
{
	m_bMouseMoveChanged = TRUE;
	CTool::Initialize();
	CTool::SetProcessStarting(true);
}

// ----------------------------------------------------------------------------
CElement* CMoveTool::OnLButtonDown(CDC* /*pDC*/, const CPoint& /*point*/)
{
	//m_points = CTool::GetPointsAndSetDataType(point);
	return NULL;
}

void CMoveTool::OnLButtonUp(CDC* pDC, const CPoint& point)
{
	m_bMouseMoveChanged = TRUE;
	CTool::SetProcessStarting(true);
	CTool::OnLButtonUp(pDC, point);
}

bool CMoveTool::OnMouseMove(CDC* pDC, const CPoint& point,int nFlags)
{
	m_ptMousePos = point;
	m_bMouseMoveChanged = TRUE;
	
	CPoints points = CTool::GetPointsAndSetDataType(point);	
	if( GetToolType() == CAnalysis::EN_ANALYSIS_CANDLE_LINE)
	{
		CCoordinateCalculatorData calcData = GetCoordinateCalculatorData( points.GetDrawingPoint());
		CRect region = calcData.GetRegion();
		if( region.PtInRect(point) == FALSE || !calcData.IsValidAddInInterface()) return false;

		CCoordinateCalculator& coordinateCalculator = CTool::GetCoordinateCalculator();
//>> 호가차트시 분석도구 종가따라가기 버그 수정 - vntsorl(20090202)
		ILPCSTR szRQ = calcData.m_pIChartOCX->GetCurrentRQ();
		int nR = calcData.GetBlockIndex().GetRow();
		int nC = calcData.GetBlockIndex().GetColumn();
		int nV = 0, nG = -1;
		ILPCSTR szGraphName = calcData.m_pIChartManager->FindNextIndicator( NULL, nR, nC, nV, nG, szRQ);
		if( szRQ.IsEmpty() || szGraphName.IsEmpty()
			|| !( nR == calcData.GetBlockIndex().GetRow() && nC == calcData.GetBlockIndex().GetColumn()))
			szRQ = calcData.m_pIChartManager->GetMainRQofBlock( calcData.GetBlockIndex().GetRow(), calcData.GetBlockIndex().GetColumn());
//		int nIndex = pIChartManager->FindViewDataIndex_Pt("DEFAULT", point.x);
		int nIndex = calcData.m_pIChartManager->FindViewDataIndex_Pt(szRQ, point.x);

		//<< (2008/12/26 - Seung-Lyong, Park) 메모리릭 제거
		double dCloseYValue = 0.0;
		double dOpenYValue = 0.0;
//		IPacket* pPacketClose = pPacketManager->GetPacket(_MTEXT( C0_CLOSE));
		IPacket* pPacketClose = calcData.m_pIPacketListManager->GetPacket( szRQ, _MTEXT( C0_CLOSE));
//		IPacket* pPacketOpen = pPacketManager->GetPacket( _MTEXT( C0_OPEN));
		IPacket* pPacketOpen = calcData.m_pIPacketListManager->GetPacket( szRQ, _MTEXT( C0_OPEN));
//<< 호가차트시 분석도구 종가따라가기 버그 수정 - vntsorl(20090202)
		BOOL bResult = ( pPacketClose && pPacketOpen);
		if( bResult)
		{
			bResult = pPacketClose->GetData( nIndex, dCloseYValue);
			if( bResult) bResult = pPacketOpen->GetData( nIndex, dOpenYValue);
		}
		if( pPacketClose) pPacketClose->Release();
		if( pPacketOpen) pPacketOpen->Release();	
		if( !bResult) return false;

		double dYValue = min(dCloseYValue,dOpenYValue);
		dYValue+= fabs(dCloseYValue - dOpenYValue)/2;
		points.SetPoints_Value_Y(dYValue); 
		
		CPoint posNew = coordinateCalculator.RelativeToAbsolute(calcData, points.GetSavingPoint());
		points.SetPoints_Point_Y(posNew.y);
	}
	
	CTool::SetDrawingRegions(points.GetDrawingPoint());
	if(OnMouseMove_InNotDrawingRegion(points.GetDrawingPoint()))
		return false;
	
	m_points = points;
	CTool::Invalidate();
	return true;
}

void CMoveTool::OnDraw(CDC* pDC)
{
	if(!PointInDrawingRegion(m_points.GetDrawingPoint()))
		return;

	DrawTool(pDC);	
	m_bMouseMoveChanged = FALSE;
}

// protected ==================================================================
bool CMoveTool::PointInDrawingRegion(const CPoint& point) const
{
	return CTool::GetDrawingRegions().PointInDrawingRegionsInColumn(point);
	/*if(!CTool::GetDrawingRegions().PointInDrawingRegionsInColumn(point))
		return false;

	return (CTool::GetDrawingRegions().GetRowNumber(point) == CTool::GetDrawingRegions().GetRowNumber(m_points.GetDrawingPoint()));
	*/
}

// 저장하기
void CMoveTool::SavePoints(const CPoints& points)
{
	m_points = points;
}

// ----------------------------------------------------------------------------
CPoints CMoveTool::GetPoints() const
{
	return m_points;
}

// private ====================================================================
// 그리는 영역이 아닌 곳에서의 Mouse Move.
bool CMoveTool::OnMouseMove_InNotDrawingRegion(const CPoint& point)
{
	if(PointInDrawingRegion(point))
		return false;

	// 이전 좌표가 그리는 영역이였을 경우
	// -> 그릴 영역에서 안그릴 영역으로 이동된 경우.
	if(PointInDrawingRegion(m_points.GetDrawingPoint()))
		CTool::Invalidate();

	return true;
}

///////////////////////////////////////////////////////////////////////////////
// class COnePointMoveTool

COnePointMoveTool::COnePointMoveTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType):
	CMoveTool(pAnalysisData, eCoordinateType)
{
}

// protected ==================================================================
// 이전 point에 관한 내용의 그리기.
void COnePointMoveTool::DrawTool(CDC* pDC)
{
	GetOnePointDrawer().DrawTool(pDC, GetOnePointDrawingData(), R2_NOT);
}

CElement* COnePointMoveTool::MakeElement(const CElementCommonEnvironment& environment)
{
	CCoordinate coordValue = CMoveTool::GetPoints().GetSavingPoint();
	CCoordinateCalculatorData calcData = GetCoordinateCalculatorData( CMoveTool::GetPoints().GetDrawingPoint());
	if( GetToolType() == CAnalysis::EN_ANALYSIS_CANDLE_LINE && calcData.IsValidAddInInterface())
	{
//>> 호가차트시 분석도구 종가따라가기 버그 수정 - vntsorl(20090202)
		ILPCSTR szRQ = calcData.m_pIChartOCX->GetCurrentRQ();
		int nR = calcData.GetBlockIndex().GetRow();
		int nC = calcData.GetBlockIndex().GetColumn();
		int nV = 0, nG = -1;
		ILPCSTR szGraphName = calcData.m_pIChartManager->FindNextIndicator( NULL, nR, nC, nV, nG, szRQ);
		if( szRQ.IsEmpty() || szGraphName.IsEmpty()
			|| !( nR == calcData.GetBlockIndex().GetRow() && nC == calcData.GetBlockIndex().GetColumn()))
			szRQ = calcData.m_pIChartManager->GetMainRQofBlock( calcData.GetBlockIndex().GetRow(), calcData.GetBlockIndex().GetColumn());
		int nIndex = calcData.m_pIChartManager->FindViewDataIndex_Pt(szRQ, CMoveTool::GetPoints().GetDrawingPoint().x);

		//<< (2008/12/26 - Seung-Lyong, Park) 메모리릭 제거
		double dCloseYValue = 0.0;
		double dOpenYValue = 0.0;
//		IPacket* pPacketClose = pPacketManager->GetPacket( _MTEXT( C0_CLOSE));
		IPacket* pPacketClose = calcData.m_pIPacketListManager->GetPacket( szRQ, _MTEXT( C0_CLOSE));
//		IPacket* pPacketOpen = pPacketManager->GetPacket( _MTEXT( C0_OPEN));
		IPacket* pPacketOpen = calcData.m_pIPacketListManager->GetPacket( szRQ, _MTEXT( C0_OPEN));
//<< 호가차트시 분석도구 종가따라가기 버그 수정 - vntsorl(20090202)
		BOOL bResult = ( pPacketClose && pPacketOpen);
		if( bResult)
		{
			bResult = pPacketClose->GetData( nIndex, dCloseYValue);
			if( bResult) bResult = pPacketOpen->GetData( nIndex, dOpenYValue);
		}
		if( pPacketClose) pPacketClose->Release();
		if( pPacketOpen) pPacketOpen->Release();	
		if( bResult)
		{
			double dYValue = min( dCloseYValue, dOpenYValue);
			dYValue += fabs( dCloseYValue - dOpenYValue)/2;
			coordValue.SetY( dYValue);
		}
	}
	
	return CElementFactory::Make(GetToolType(),coordValue, GetTextPosition(), environment);
}

// private ====================================================================
COnePointDrawingData COnePointMoveTool::GetOnePointDrawingData() const
{
	// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)

// 	return COnePointDrawingData(CTool::GetElementCommonEnvironment().GetDataType(), 
// 		const_cast<CTextPosition*> (GetTextPosition()), 
// 		&CTool::GetElementCommonEnvironment().GetLineExt(),
// 		&CTool::GetDrawingRegions(), CTool::GetDisplayAttributes(), 
// 		CTool::GetVertScaleType(CMoveTool::GetPoints().GetDrawingPoint()), 
// 		CMoveTool::GetPoints());

	ILPCSTR szHelpMsg = NULL;
	CString strDataType, strPacketType;	
	CCoordinateCalculatorData calData = GetCoordinateCalculatorData( CMoveTool::GetPoints().GetDrawingPoint());
	ILPCSTR szRQ = calData.m_pIChartOCX->GetCurrentRQ();
	IPacketManager * pCurPacketManager = calData.m_pIPacketListManager->GetPacketManager(szRQ);
	pCurPacketManager->GetHelpMessageItem("UD", szHelpMsg); strDataType = szHelpMsg;
	pCurPacketManager->GetHelpMessageItem("RDATET", szHelpMsg); strPacketType = szHelpMsg;
	CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = (CScaleBaseData::HORZSCALEDRAWERTYPE)atoi(strDataType);

	return COnePointDrawingData(CTool::GetElementCommonEnvironment().GetDataType(), 
		const_cast<CTextPosition*> (GetTextPosition()), 
		&CTool::GetElementCommonEnvironment().GetLineExt(),
		&CTool::GetDrawingRegions(), CTool::GetDisplayAttributes(), 
		CTool::GetVertScaleType(CMoveTool::GetPoints().GetDrawingPoint()), 
		CMoveTool::GetPoints(), eDateUnit);

	// 2011.01.27 by SYS <<

}

///////////////////////////////////////////////////////////////////////////////
// class CTwoPointMoveTool

CTwoPointMoveTool::CTwoPointMoveTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType):
	CMoveTool(pAnalysisData, eCoordinateType)
{
}

// protected ==================================================================
// 이전 point에 관한 내용의 그리기.
void CTwoPointMoveTool::DrawTool(CDC* pDC)
{
	GetTwoPointDrawer().DrawTool(pDC, GetTwoPointDrawingData(), R2_NOT);
}

CElement* CTwoPointMoveTool::MakeElement(const CElementCommonEnvironment& environment)
{
	return CElementFactory::Make(GetToolType(), CMoveTool::GetPoints().GetSavingPoint(), 
		GetTwoPoints().GetSavingPoint(), GetTextPosition(), environment);
}

// ----------------------------------------------------------------------------
CPoints CTwoPointMoveTool::GetPoints(const CPoint& orgPoint, const int nAngle, const int nDataGap) const
{
	CPoint drawingPt = GetPoint(CRegion(CTool::GetDrawingRegions().GetCurrentFullGraphRegion(), CTool::GetDisplayAttributes()), orgPoint, nAngle, nDataGap);
	return CPoints(GetSavingPoint(orgPoint, drawingPt), drawingPt);
}

CPoint CTwoPointMoveTool::GetPoint(const CRegion& region, const CPoint& orgPoint, const int nAngle, const int nDataGap) const
{
	int x = CTwoPointMoveTool::GetPoint_X(region, orgPoint, nDataGap);
	if(x < 15)
		x = CTwoPointMoveTool::GetPoint_X(region, orgPoint, 20);
	int y = CTwoPointMoveTool::GetPoint_Y(orgPoint, x, nAngle);

	return CPoint(x, y);
}

CCoordinate CTwoPointMoveTool::GetSavingPoint(const CPoint& regionPoint, const CPoint& savingPoint) const
{
	if(!CTool::IsRelativePointType())
		return CCoordinate(savingPoint);

	CCoordinateCalculatorData calData = CTool::GetCoordinateCalculatorData(regionPoint);
	return CTool::GetCoordinateCalculator().AbsoluteToRelative(calData, savingPoint);
}

// nGap 에 있는 x좌표
int CTwoPointMoveTool::GetPoint_X(const CRegion& region, const CPoint& point, const int nGap) const
{
	CCoordinateCalculator& coordinateCalculator = CTool::GetCoordinateCalculator();
	int currentDataIndex = coordinateCalculator.GetCurrentDataIndex(region, point);
	return coordinateCalculator.GetDrawingPoint_X(region, currentDataIndex +nGap);
}

// 각에 따른 y좌표
int CTwoPointMoveTool::GetPoint_Y(const CPoint& orgPoint, const int x2, const int nAngle) const
{
	int nTan = CMath::TanI(nAngle);
	if(nTan == 0)
		return orgPoint.y;

	return (-nTan * (x2 - orgPoint.x) + orgPoint.y);
}

// private ====================================================================
CTwoPointDrawingData CTwoPointMoveTool::GetTwoPointDrawingData() const
{
	// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)

// 	return CTwoPointDrawingData(
// 				CTool::GetElementCommonEnvironment().GetDataType(),
// 				CTool::GetElementCommonEnvironment().GetEquiDiv(),  
// 				CTool::GetElementCommonEnvironment().GetPriceRangeDiv(),	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
// 				const_cast<CTextPosition*> (GetTextPosition()), 
// 				&CTool::GetElementCommonEnvironment().GetLineExt(),
// 				CTool::GetDrawingRegions().GetCurrentGraphPartRegion(), 
// 				CTool::GetDisplayAttributes(), 
// 				CTool::GetVertScaleType(CMoveTool::GetPoints().GetDrawingPoint()), 
// 				CMoveTool::GetPoints(), GetTwoPoints());

	ILPCSTR szHelpMsg = NULL;
	CString strDataType, strPacketType;	
	CCoordinateCalculatorData calData = GetCoordinateCalculatorData( CMoveTool::GetPoints().GetDrawingPoint());
	ILPCSTR szRQ = calData.m_pIChartOCX->GetCurrentRQ();
	IPacketManager * pCurPacketManager = calData.m_pIPacketListManager->GetPacketManager(szRQ);
	pCurPacketManager->GetHelpMessageItem("UD", szHelpMsg); strDataType = szHelpMsg;
	pCurPacketManager->GetHelpMessageItem("RDATET", szHelpMsg); strPacketType = szHelpMsg;
	CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = (CScaleBaseData::HORZSCALEDRAWERTYPE)atoi(strDataType);
	//
	
 	return CTwoPointDrawingData(
				CTool::GetElementCommonEnvironment().GetDataType(),
				CTool::GetElementCommonEnvironment().GetEquiDiv(),  
				CTool::GetElementCommonEnvironment().GetPriceRangeDiv(),	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
				const_cast<CTextPosition*> (GetTextPosition()), 
				&CTool::GetElementCommonEnvironment().GetLineExt(),
				CTool::GetDrawingRegions().GetCurrentGraphPartRegion(), 
				CTool::GetDisplayAttributes(), 
				CTool::GetVertScaleType(CMoveTool::GetPoints().GetDrawingPoint()), 
				CMoveTool::GetPoints(), GetTwoPoints(), eDateUnit);

	// 2011.01.27 by SYS <<
}

///////////////////////////////////////////////////////////////////////////////
// class CDownTool

CDownTool::CDownTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType):
	CTool(pAnalysisData, eCoordinateType)
{	
	CTool::Initialize();
	CTool::SetProcessStarting(false);
}

// public =====================================================================
// data 초기화
void CDownTool::Initialize()
{
	CTool::Initialize();
	if(GetToolType() == CAnalysis::EN_ANALYSIS_DOWN_CROSS)
		CTool::SetProcessStarting(true);
	else
		CTool::SetProcessStarting(false);
}

// ----------------------------------------------------------------------------
CElement* CDownTool::OnLButtonDown(CDC* /*pDC*/, const CPoint& point)
{
	CTool::SetProcessStarting(true);
	CTool::SetDrawingRegions(point);
	if(GetToolType() != CAnalysis::EN_ANALYSIS_DOWN_CROSS)
		CTool::ChangeCursor(CToolEnumData::DRAW_PEN);

	return NULL;
}

bool CDownTool::OnMouseMove(CDC* pDC, const CPoint& point,int nFlags)
{	
	if(PointInDrawingRegion(point))
	{
		if(GetToolType() != CAnalysis::EN_ANALYSIS_DOWN_CROSS)
			CTool::ChangeCursor(CToolEnumData::DRAW_PEN);
		return true;
	}

	CTool::ChangeCursor(CToolEnumData::DRAW_NODROP);
	return false;
}

///////////////////////////////////////////////////////////////////////////////
// class COneOrTwoPointTool

COneOrTwoPointTool::COneOrTwoPointTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType):
	CDownTool(pAnalysisData, eCoordinateType)
{
	CTool::Initialize();
	CTool::SetProcessStarting(false);
}

// public =====================================================================
CElement* COneOrTwoPointTool::OnLButtonDown(CDC* pDC, const CPoint& point)
{
	m_upPoints = CTool::GetPointsAndSetDataType(point);

	m_ptPoint = point;
	return CDownTool::OnLButtonDown(pDC, m_upPoints.GetDrawingPoint());
}

void COneOrTwoPointTool::OnLButtonUp(CDC* pDC, const CPoint& point)
{	
	CTool::SetProcessStarting(false);
	CPoint ptPos(point);
	if(GetToolType() == CAnalysis::EN_ANALYSIS_CANDLE_BONG)
		ptPos.x = m_upPoints.GetDrawingPoint().x;		
	
	CTool::OnLButtonUp(pDC, ptPos);
}

bool COneOrTwoPointTool::OnMouseMove(CDC* pDC, const CPoint& point,int nFlags)
{
	if(PointInDrawingRegion(point) == FALSE)
		return false;

	Draw_UpPoint(pDC);		
	if(!MouseMove(pDC, point))
		return false;

	CTool::Invalidate();	
	return CDownTool::OnMouseMove(pDC, m_upPoints.GetDrawingPoint(),nFlags);
}

void COneOrTwoPointTool::OnDraw(CDC* pDC)
{	
	Draw_UpPoint(pDC);
}

// protected ==================================================================
// 저장하기
//void COneOrTwoPointTool::SaveAndDraw(CDC* pDC, const CPoints& points, const int /*drawMode*/)
/*{
	m_upPoints = points;
	Draw_UpPoint(pDC);
}*/

void COneOrTwoPointTool::SavePoints(const CPoints& points)
{
	m_upPoints = points;
}

// ----------------------------------------------------------------------------
CPoints COneOrTwoPointTool::GetUpPoints() const
{
	return m_upPoints;
}

void COneOrTwoPointTool::SetUpPoints(const CPoints& points)
{
	m_upPoints = points;
}

///////////////////////////////////////////////////////////////////////////////
// class COnePointDownTool

COnePointDownTool::COnePointDownTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType):
	COneOrTwoPointTool(pAnalysisData, eCoordinateType)
{
	CTool::Initialize();
	CTool::SetProcessStarting(false);	
}

COnePointDownTool::~COnePointDownTool()
{
}

// public =====================================================================
CElement* COnePointDownTool::OnLButtonDown(CDC* pDC, const CPoint& point)
{	
	return COneOrTwoPointTool::OnLButtonDown(pDC, point);
}

bool COnePointDownTool::OnMouseMove(CDC* pDC, const CPoint& point,int nFlags)
{
	if(!COnePointDownTool::MouseMove(pDC, point))
		return false;

	if(!CDownTool::OnMouseMove(pDC, COneOrTwoPointTool::GetUpPoints().GetDrawingPoint(),nFlags))
		return false;

	CTool::Invalidate();
	return true;
}

void COnePointDownTool::OnDraw(CDC* pDC)
{ 
	Draw_UpPoint(pDC);	
}

// protected ==================================================================
bool COnePointDownTool::PointInDrawingRegion(const CPoint& point) const
{
	return CTool::GetDrawingRegions().PointInDrawingRegionsInColumn(point);
}
	
CElement* COnePointDownTool::MakeElement(const CElementCommonEnvironment& environment)
{
	return CElementFactory::Make(GetToolType(), COneOrTwoPointTool::GetUpPoints().GetSavingPoint(), GetTextPosition(), environment);
}

// ----------------------------------------------------------------------------
/*bool COnePointDownTool::MouseMove(CDC* pDC, const CPoint& point)
{
	if(!m_bPrePointUsed){
		m_bPrePointUsed = true;
		return false;
	}

	CPoints drawingPts = CTool::GetPointsAndSetDataType(point);
	CTool::SetDrawingRegions(drawingPts.GetDrawingPoint());
	COneOrTwoPointTool::SetUpPoints(drawingPts);
	Draw_UpPoint(pDC);
	return true;
}*/

bool COnePointDownTool::MouseMove(CDC* pDC, const CPoint& point)
{
	CPoints drawingPts = CTool::GetPointsAndSetDataType(point);
	CTool::SetDrawingRegions(drawingPts.GetDrawingPoint());
	if(OnMouseMove_InNotDrawingRegion(drawingPts.GetDrawingPoint()))
		return false;
	
	COneOrTwoPointTool::SetUpPoints(drawingPts);
	return true;
}

void COnePointDownTool::Draw_UpPoint(CDC* pDC)
{
	//GetOnePointDrawer().DrawTool(pDC, GetOnePointDrawingData(), R2_NOT);
	GetOnePointDrawer().DrawElement(pDC, GetOnePointDrawingData(), R2_NOT);
}

// private ====================================================================
// 그리는 영역이 아닌 곳에서의 Mouse Move.
bool COnePointDownTool::OnMouseMove_InNotDrawingRegion(const CPoint& point)
{
	if(PointInDrawingRegion(point))
		return false;

	// 이전 좌표가 그리는 영역이였을 경우
	// -> 그릴 영역에서 안그릴 영역으로 이동된 경우.
	if(PointInDrawingRegion(COneOrTwoPointTool::GetUpPoints().GetDrawingPoint()))
		CTool::Invalidate();

	return true;
}

COnePointDrawingData COnePointDownTool::GetOnePointDrawingData() const
{
	// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)

// 	return COnePointDrawingData(CTool::GetElementCommonEnvironment().GetDataType(), 
// 		const_cast<CTextPosition*> (GetTextPosition()), 
// 		&CTool::GetElementCommonEnvironment().GetLineExt(),
// 		&CTool::GetDrawingRegions(), CTool::GetDisplayAttributes(), 
// 		CTool::GetVertScaleType(COneOrTwoPointTool::GetUpPoints().GetDrawingPoint()), 
// 		COneOrTwoPointTool::GetUpPoints());

	ILPCSTR szHelpMsg = NULL;
	CString strDataType, strPacketType;	
	CCoordinateCalculatorData calData = GetCoordinateCalculatorData( COneOrTwoPointTool::GetUpPoints().GetDrawingPoint());
	ILPCSTR szRQ = calData.m_pIChartOCX->GetCurrentRQ();
	IPacketManager * pCurPacketManager = calData.m_pIPacketListManager->GetPacketManager(szRQ);
	pCurPacketManager->GetHelpMessageItem("UD", szHelpMsg); strDataType = szHelpMsg;
	pCurPacketManager->GetHelpMessageItem("RDATET", szHelpMsg); strPacketType = szHelpMsg;
	CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = (CScaleBaseData::HORZSCALEDRAWERTYPE)atoi(strDataType);
	//

	return COnePointDrawingData(CTool::GetElementCommonEnvironment().GetDataType(), 
		const_cast<CTextPosition*> (GetTextPosition()), 
		&CTool::GetElementCommonEnvironment().GetLineExt(),
		&CTool::GetDrawingRegions(), CTool::GetDisplayAttributes(), 
		CTool::GetVertScaleType(COneOrTwoPointTool::GetUpPoints().GetDrawingPoint()), 
		COneOrTwoPointTool::GetUpPoints(), eDateUnit);

	// 2011.01.27 by SYS <<
}

///////////////////////////////////////////////////////////////////////////////
// class CTwoPointDownTool

CTwoPointDownTool::CTwoPointDownTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType):
	COneOrTwoPointTool(pAnalysisData, eCoordinateType)
{
}

// public =====================================================================
CElement* CTwoPointDownTool::OnLButtonDown(CDC* pDC, const CPoint& point)
{
	COneOrTwoPointTool::OnLButtonDown(pDC, point);
	m_downPoints = COneOrTwoPointTool::GetUpPoints();	
	return NULL;
}

// protected ==================================================================
bool CTwoPointDownTool::PointInDrawingRegion(const CPoint& point) const
{
	CRect currentRegion = CTool::GetDrawingRegions().GetCurrentFullGraphRegion();
	if(currentRegion.IsRectEmpty())
		return false;

	return (currentRegion.top <= point.y && currentRegion.bottom >= point.y);
}

bool CTwoPointDownTool::IsMakeElement(const CPoint& point) const
{
	if(PointInDrawingRegion(point) == FALSE)
		return false;

	if(m_downPoints.GetSavingPoint() == CTool::GetPoints(point).GetSavingPoint())
		return false;

	return true;
}

CElement* CTwoPointDownTool::MakeElement(const CElementCommonEnvironment& environment)
{
	CElement *pElement = NULL;
	CCoordinate coordStart = m_downPoints.GetSavingPoint();
	CCoordinate coordEnd = COneOrTwoPointTool::GetUpPoints().GetSavingPoint();

	CCoordinateCalculatorData calcData = GetCoordinateCalculatorData( m_downPoints.GetDrawingPoint());
	if( calcData.IsValidAddInInterface())
		if( GetToolType() == CAnalysis::EN_ANALYSIS_LINEAR_REGRESSION ||
			GetToolType() == CAnalysis::EN_ANALYSIS_RAFF_REGRESSION)
	{
		double dChannelHeight = 0.0;

//>> 호가차트시 분석도구 종가따라가기 버그 수정 - vntsorl(20090202)
		ILPCSTR szRQ = calcData.m_pIChartOCX->GetCurrentRQ();
		int nR = calcData.GetBlockIndex().GetRow();
		int nC = calcData.GetBlockIndex().GetColumn();
		int nV = 0, nG = -1;
		ILPCSTR szGraphName = calcData.m_pIChartManager->FindNextIndicator( NULL, nR, nC, nV, nG, szRQ);
		if( szRQ.IsEmpty() || szGraphName.IsEmpty()
			|| !( nR == calcData.GetBlockIndex().GetRow() && nC == calcData.GetBlockIndex().GetColumn()))
			szRQ = calcData.m_pIChartManager->GetMainRQofBlock( calcData.GetBlockIndex().GetRow(), calcData.GetBlockIndex().GetColumn());
//		int nIndex1 = pIChartManager->FindViewDataIndex_Pt("DEFAULT", m_downPoints.GetDrawingPoint().x);
//		int nIndex2 = pIChartManager->FindViewDataIndex_Pt("DEFAULT", COneOrTwoPointTool::GetUpPoints().GetDrawingPoint().x);
		int nIndex1 = calcData.m_pIChartManager->FindViewDataIndex_Pt( szRQ, m_downPoints.GetDrawingPoint().x);
		int nIndex2 = calcData.m_pIChartManager->FindViewDataIndex_Pt( szRQ, COneOrTwoPointTool::GetUpPoints().GetDrawingPoint().x);
		
		if(abs(nIndex2 - nIndex1) > 1)
		{
//			IPacket* pPacket = pPacketManager->GetPacket( _MTEXT( C0_CLOSE));
			IPacket* pPacket = calcData.m_pIPacketListManager->GetPacket( szRQ, _MTEXT( C0_CLOSE));
//<< 호가차트시 분석도구 종가따라가기 버그 수정 - vntsorl(20090202)
			if( pPacket)
			{
				double dXAvg = 0.0f;
				double dYAvg = 0.0f;
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

				coordStart.SetY(dFirstVal);
				coordEnd.SetY(dSecondVal);
				
				if(GetToolType() == CAnalysis::EN_ANALYSIS_RAFF_REGRESSION)
				{				
					double dValDiffrence = 0.0;
					for(i=nStartIdx; i<=nEndIdx; i++)
					{
						if( !pPacket->GetData( i, dYValue)) break;
						dValDiffrence = fabs(dYValue - (dAlpha + dBeta * i));
						dChannelHeight = max(dChannelHeight, dValDiffrence);
					}				
				}
				pPacket->Release();
			}
		}

		pElement = CElementFactory::Make(GetToolType(),coordStart,coordEnd,GetTextPosition(),environment,dChannelHeight);
	}
	else
	{
		pElement = CElementFactory::Make(GetToolType(),coordStart,coordEnd,GetTextPosition(),environment);
	}
	
	return pElement;
}

// ----------------------------------------------------------------------------
bool CTwoPointDownTool::MouseMove(CDC* pDC, const CPoint& point)
{
	CPoint ptPos(point);
	if(GetToolType() == CAnalysis::EN_ANALYSIS_CANDLE_BONG)
	{
		CCoordinateCalculatorData calcData = GetCoordinateCalculatorData(m_downPoints.GetDrawingPoint());
		int nWidth = (int)max(1,(calcData.GetRegion().Width()/calcData.GetDisplayDataCount()));
		if(m_downPoints.GetDrawingPoint().x > point.x)
			ptPos.x = m_downPoints.GetDrawingPoint().x - nWidth-1;
		else 
			ptPos.x = m_downPoints.GetDrawingPoint().x + nWidth+1;
	}
	SavePoints(CTool::GetPoints(ptPos));
	Draw_UpPoint(pDC);
	return true;
}

void CTwoPointDownTool::Draw_UpPoint(CDC* pDC)
{
	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	CTwoPointDrawingData &drawingData = GetTwoPointDrawingData();
	if( CDlgAnalysisToolSet::G_PRICE_RANGE_DIV_CHECK == drawingData.GetPriceRangeDivType())
	{
		CPoints ptsStart = drawingData.GetStartPoints();
		CPoints ptsEnd = drawingData.GetEndPoints();
		double dMax = 0.0;
		double dMin = DBL_MAX;
		CCoordinateCalculator& coordinateCalculator = CCoordinateCalculatorFinder::FindCoordinateCalculator(false);
		const CCoordinateCalculatorData &calData = CTool::GetCoordinateCalculatorData( ptsStart.GetDrawingPoint());
		CCoordinate cdStart = coordinateCalculator.AbsoluteToRelative( calData, ptsStart.GetDrawingPoint());
		CCoordinate cdEnd = coordinateCalculator.AbsoluteToRelative( calData, ptsEnd.GetDrawingPoint());
		if( CTwoPointElement::UpdatePointsForPriceRangeOption( calData, cdStart, cdEnd, dMax, dMin))
		{
			cdStart.SetY( dMax);
			cdEnd.SetY( dMin);
			CPoint startPoint = coordinateCalculator.RelativeToAbsolute_NoAutoPrice(calData, cdStart);
			CPoint endPoint = coordinateCalculator.RelativeToAbsolute_NoAutoPrice(calData, cdEnd);
			drawingData.SetStartYPoints_Pos( startPoint.y);
			drawingData.SetEndYPoints_Pos( endPoint.y);
		}
	}

	GetTwoPointDrawer().DrawTool(pDC, drawingData, R2_NOT);
}

// ----------------------------------------------------------------------------
CPoints CTwoPointDownTool::GetDownPoints() const
{
	return m_downPoints;
}

// private ====================================================================
CTwoPointDrawingData CTwoPointDownTool::GetTwoPointDrawingData() const
{
	// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)

// 	return CTwoPointDrawingData(
// 				CTool::GetElementCommonEnvironment().GetDataType(), 
// 				CTool::GetElementCommonEnvironment().GetEquiDiv(),
// 				CTool::GetElementCommonEnvironment().GetPriceRangeDiv(),	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
// 				const_cast<CTextPosition*> (GetTextPosition()), 
// 				&CTool::GetElementCommonEnvironment().GetLineExt(),
// 				CTool::GetDrawingRegions().GetCurrentGraphPartRegion(), 
// 				CTool::GetDisplayAttributes(), 
// 				CTool::GetVertScaleType(COneOrTwoPointTool::GetUpPoints().GetDrawingPoint()), 
// 				m_downPoints,
// 				COneOrTwoPointTool::GetUpPoints());	

	ILPCSTR szHelpMsg = NULL;
	CString strDataType, strPacketType;	
	CCoordinateCalculatorData calData = GetCoordinateCalculatorData( COneOrTwoPointTool::GetUpPoints().GetDrawingPoint());
	ILPCSTR szRQ = calData.m_pIChartOCX->GetCurrentRQ();
	IPacketManager * pCurPacketManager = calData.m_pIPacketListManager->GetPacketManager(szRQ);
	pCurPacketManager->GetHelpMessageItem("UD", szHelpMsg); strDataType = szHelpMsg;
	pCurPacketManager->GetHelpMessageItem("RDATET", szHelpMsg); strPacketType = szHelpMsg;
	CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = (CScaleBaseData::HORZSCALEDRAWERTYPE)atoi(strDataType);

	return CTwoPointDrawingData(
				CTool::GetElementCommonEnvironment().GetDataType(), 
				CTool::GetElementCommonEnvironment().GetEquiDiv(),
				CTool::GetElementCommonEnvironment().GetPriceRangeDiv(),	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
				const_cast<CTextPosition*> (GetTextPosition()), 
				&CTool::GetElementCommonEnvironment().GetLineExt(),
				CTool::GetDrawingRegions().GetCurrentGraphPartRegion(), 
				CTool::GetDisplayAttributes(), 
				CTool::GetVertScaleType(COneOrTwoPointTool::GetUpPoints().GetDrawingPoint()), 
				m_downPoints,
				COneOrTwoPointTool::GetUpPoints(), eDateUnit);	

	// 2011.01.27 by SYS <<

}

///////////////////////////////////////////////////////////////////////////////
// class CThreePointDownTool

CThreePointDownTool::CThreePointDownTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType):
	CDownTool(pAnalysisData, eCoordinateType)
{
	CTool::Initialize();
	CTool::SetProcessStarting(false);
}

// public =====================================================================
// data 초기화
void CThreePointDownTool::Initialize()
{
	CDownTool::Initialize();
	m_nPointCount = 0;
}

// ----------------------------------------------------------------------------
CElement* CThreePointDownTool::OnLButtonDown(CDC* pDC, const CPoint& point)
{
	return CDownTool::OnLButtonDown(pDC, CTool::GetDrawingPointAndSetDataType(point));
}

void CThreePointDownTool::OnLButtonUp(CDC* pDC, const CPoint& point)
{
	if(PointInDrawingRegion(point)){		
		CPoints points = CTool::GetPoints(point);
		SavePoints(points);
		if(!IsMakeElement(point))
			return;

		if( GetToolType() == CAnalysis::EN_ANALYSIS_ADJUST_RATIO)
		{
			//세번째 점을 그린다
			CTool::Invalidate();	
			
			ML_SET_LANGUAGE_RES();
			CDlgAnalysisAdjustRatio dlg;
			dlg.SetValue(0,CTool::GetPosCoordinate(m_points1.GetDrawingPoint()).GetY());
			dlg.SetValue(1,CTool::GetPosCoordinate(m_points2.GetDrawingPoint()).GetY());
			dlg.SetValue(2,CTool::GetPosCoordinate(m_points3.GetDrawingPoint()).GetY());
			dlg.DoModal();
		}
		else
		{
			CTool::MakeElement(points.GetDrawingPoint());
		}		
	}
	
	Initialize();
}

void CThreePointDownTool::OnDraw(CDC* pDC)
{
	if(m_nPointCount <= 0)
		return;

	GetThreePointDrawer().DrawTool(pDC, GetThreePointDrawingData(), R2_NOT/*R2_COPYPEN*/);
}

// protected ==================================================================
// 저장하기
void CThreePointDownTool::SavePoints(const CPoints& points)
{
	if(m_nPointCount == 0)
		m_points1 = points;
	else if(m_nPointCount == 1)
		m_points2 = points;
	else if(m_nPointCount == 2)
		m_points3 = points;
	else
		return;

	m_nPointCount++;
}

bool CThreePointDownTool::IsMakeElement(const CPoint& /*point*/) const
{
	return IsEndPoint();
}

// ----------------------------------------------------------------------------
CElement* CThreePointDownTool::MakeElement(const CElementCommonEnvironment& environment)
{
	if(!IsEndPoint())
		return NULL;

	return CElementFactory::Make(GetToolType(), 
		m_points1.GetSavingPoint(), m_points2.GetSavingPoint(), m_points3.GetSavingPoint(), 
		GetTextPosition(), environment);
}


// private ====================================================================
bool CThreePointDownTool::IsEndPoint() const
{
	return (m_nPointCount == 3);
}

// ----------------------------------------------------------------------------
CThreePointDrawingData CThreePointDownTool::GetThreePointDrawingData() const
{
	// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)

// 	return CThreePointDrawingData(CTool::GetElementCommonEnvironment().GetDataType(), 
// 		const_cast<CTextPosition*> (GetTextPosition()), 
// 		&CTool::GetElementCommonEnvironment().GetLineExt(),
// 		CTool::GetDrawingRegions().GetCurrentFullGraphRegion(), CTool::GetDisplayAttributes(), 
// 		CTool::GetVertScaleType(m_points1.GetDrawingPoint()),
// 		m_nPointCount, m_points1, m_points2, m_points3);

	ILPCSTR szHelpMsg = NULL;
	CString strDataType, strPacketType;	
	CCoordinateCalculatorData calData = GetCoordinateCalculatorData( m_points1.GetDrawingPoint());
	ILPCSTR szRQ = calData.m_pIChartOCX->GetCurrentRQ();
	IPacketManager * pCurPacketManager = calData.m_pIPacketListManager->GetPacketManager(szRQ);
	pCurPacketManager->GetHelpMessageItem("UD", szHelpMsg); strDataType = szHelpMsg;
	pCurPacketManager->GetHelpMessageItem("RDATET", szHelpMsg); strPacketType = szHelpMsg;
	CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = (CScaleBaseData::HORZSCALEDRAWERTYPE)atoi(strDataType);

	return CThreePointDrawingData(CTool::GetElementCommonEnvironment().GetDataType(), 
									const_cast<CTextPosition*> (GetTextPosition()), 
									&CTool::GetElementCommonEnvironment().GetLineExt(),
									CTool::GetDrawingRegions().GetCurrentFullGraphRegion(), CTool::GetDisplayAttributes(), 
									CTool::GetVertScaleType(m_points1.GetDrawingPoint()),
									m_nPointCount, m_points1, m_points2, m_points3, eDateUnit);

	// 2011.01.27 by SYS <<
}

///////////////////////////////////////////////////////////////////////////////
// class CMultiPointDownTool

CMultiPointDownTool::CMultiPointDownTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType):
	CDownTool(pAnalysisData, eCoordinateType)
{
	CTool::Initialize();
	CTool::SetProcessStarting(false);
}

// public =====================================================================
// data 초기화
void CMultiPointDownTool::Initialize()
{
	CDownTool::Initialize();
	m_nPontsCount = 0;	
}

// ----------------------------------------------------------------------------
CElement* CMultiPointDownTool::OnLButtonDown(CDC* pDC, const CPoint& point)
{
	if(PointInDrawingRegion(point))
	{
		m_ptListPoints[m_nPontsCount] = CTool::GetPoints(point);
		m_nPontsCount++;
	}

	return CDownTool::OnLButtonDown(pDC, CTool::GetDrawingPointAndSetDataType(point));
}

bool CMultiPointDownTool::OnMouseMove(CDC* pDC, const CPoint& point,int nFlags)
{
	if(nFlags != MK_LBUTTON)
		return false;

	if(PointInDrawingRegion(point) && IsMakeElement(point))
	{
		m_ptListPoints[m_nPontsCount] = CTool::GetPointsAndSetDataType(point);
		m_nPontsCount++;
		CTool::Invalidate();	
	}
		
	return true;
}

void CMultiPointDownTool::OnLButtonUp(CDC* pDC, const CPoint& point)
{
	if(PointInDrawingRegion(point))
	{
		if(!IsMakeElement(point))
		{
			CTool::MakeElement(m_ptListPoints[m_nPontsCount-1].GetDrawingPoint());
			Initialize();
			return;
		}

		m_ptListPoints[m_nPontsCount] = CTool::GetPoints(point);		
		CTool::MakeElement(m_ptListPoints[m_nPontsCount].GetDrawingPoint());
		m_nPontsCount++;
	}
	
	Initialize();
}

void CMultiPointDownTool::OnDraw(CDC* pDC)
{
	if(m_nPontsCount < 2)
		return;

	GetMultiPointDrawer().DrawTool(pDC, GetMultiPointDrawingData(), R2_NOT/*R2_COPYPEN*/);
}

// protected ==================================================================

void CMultiPointDownTool::SavePoints(const CPoints & point)
{
}

bool CMultiPointDownTool::IsMakeElement(const CPoint& /*point*/) const
{
	return IsEndPoint();
}

// ----------------------------------------------------------------------------
CElement* CMultiPointDownTool::MakeElement(const CElementCommonEnvironment& environment)
{
	//if(!IsEndPoint())
	//	return NULL;

	return CElementFactory::Make(GetToolType(),m_nPontsCount,m_ptListPoints,environment);	
}

// private ====================================================================
//MaxCount 500개로 제약 
bool CMultiPointDownTool::IsEndPoint() const
{
	return (m_nPontsCount < 500);
}

// ----------------------------------------------------------------------------
CMultiPointDrawingData CMultiPointDownTool::GetMultiPointDrawingData() const
{
	// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)

// 	return CMultiPointDrawingData(CTool::GetElementCommonEnvironment().GetDataType(), 
// 		const_cast<CTextPosition*> (GetTextPosition()), 
// 		&CTool::GetElementCommonEnvironment().GetLineExt(),
// 		CTool::GetDrawingRegions().GetCurrentFullGraphRegion(), CTool::GetDisplayAttributes(), 
// 		CTool::GetVertScaleType(CDownTool::GetMousePos()), 
// 		m_nPontsCount,m_ptListPoints);

	ILPCSTR szHelpMsg = NULL;
	CString strDataType, strPacketType;	
	CCoordinateCalculatorData calData = GetCoordinateCalculatorData( CDownTool::GetMousePos());
	ILPCSTR szRQ = calData.m_pIChartOCX->GetCurrentRQ();
	IPacketManager * pCurPacketManager = calData.m_pIPacketListManager->GetPacketManager(szRQ);
	pCurPacketManager->GetHelpMessageItem("UD", szHelpMsg); strDataType = szHelpMsg;
	pCurPacketManager->GetHelpMessageItem("RDATET", szHelpMsg); strPacketType = szHelpMsg;
	CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = (CScaleBaseData::HORZSCALEDRAWERTYPE)atoi(strDataType);

	return CMultiPointDrawingData(CTool::GetElementCommonEnvironment().GetDataType(), 
									const_cast<CTextPosition*> (GetTextPosition()), 
									&CTool::GetElementCommonEnvironment().GetLineExt(),
									CTool::GetDrawingRegions().GetCurrentFullGraphRegion(), CTool::GetDisplayAttributes(), 
									CTool::GetVertScaleType(CDownTool::GetMousePos()), 
									m_nPontsCount,m_ptListPoints, eDateUnit);
	// 2011.01.27 by SYS <<

}
