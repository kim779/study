// SymbolTool.cpp: implementation of the CSymbolTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "symbol.h"
#include "SymbolTool.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "DrawingSymbolFactory.h"
#include "DrawingSymbolRow.h"
#include "DrawingSymbol.h"
//block영역 data
#include "Region_AllBlock.h"
//symboldata
#include "SymbolData.h"

#include "PropertiesData_text.h"	// Added by ClassView
//메세지 define
#include "MessageDefine.h"

// Undo / Redo
#include "ElementSaverBuilder.h"

/////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////
//
CSymbolTool::CSymbolTool():
	m_bDrawStart(false)
{
	m_hOcxWnd = NULL;
}

bool CSymbolTool::IsDrawStart()
{
	return m_bDrawStart;
}

void CSymbolTool::SetDrawStart(const bool bDrawStart)
{
	m_bDrawStart = bDrawStart;
}

//point가 Block안에 있는지 확인한다.
bool CSymbolTool::IsPointInBlock(const CRect &drawingRegion, const CPoint &point)
{
	if(drawingRegion.PtInRect(point))
		return true;

	return false;
}

//커서 바꾸기
void CSymbolTool::ChangeCursor(const CCursorShape::CHARTOBJECT cursorShape)
{
	if(m_pSymbolData->GetParent() == NULL)
		return;
	
	m_pSymbolData->GetParent()->SendMessage(UM_CURSOR_EXCHANGE, cursorShape);
}

COLORREF CSymbolTool::GetColor()
{
	return m_pSymbolData->GetPropertiesData()->GetColor();
}

LOGFONT CSymbolTool::GetLogFont()
{
	return m_pSymbolData->GetPropertiesData()->GetLogFont();
}

void CSymbolTool::SetSymbolData(CSymbolData *pSymbolData)
{
	m_pSymbolData = pSymbolData;
}

CSymbolData* CSymbolTool::GetSymbolData()
{
	return m_pSymbolData;
}

// 좌표 계산을 위한 data.
CCoordinateCalculatorData CSymbolTool::GetCoordinateCalculatorData(const CPoint& point) const
{
	IChartManager *pIChartManager = m_pSymbolData->GetMainBlock();
	if( !pIChartManager) return CCoordinateCalculatorData( NULL);

	CCoordinateCalculatorData calData( pIChartManager);
	calData.GetCoordinateCalculatorData( point);
	pIChartManager->Release();
	return calData;
}

void CSymbolTool::OnLButtonUp(CDC* pDC, const CPoint& startpoint, const CPoint& endpoint, const CSymbolBaseData::TOOLTYPE toolType)
{			
	CPoint drawingPoint = GetDrawingPoint(endpoint);
 	CRect drawingRegion = GetCurDrawingRegion(startpoint);
	// 그리는 영역안에 있을 경우만 object한다.
	if(IsPointInBlock(drawingRegion, endpoint)) 
	{
		SavePointAndDraw(pDC, drawingPoint, R2_COPYPEN);
		AddElement(toolType, drawingPoint);
	}
	else
	{
		ML_SET_LANGUAGE_RES();

		CString strNotice;
		strNotice.LoadString(IDS_ESCAPE_RANGE);
		AfxMessageBox(strNotice);
	}
}

CPoint CSymbolTool::GetDrawingPoint(const CPoint &point)
{
	CCoordinateCalculator calculator;
	return calculator.AbsoluteToAbsolute(GetCoordinateCalculatorData(point), point);
}

CCoordinate CSymbolTool::GetSavingPoint(const CPoint &point)
{
	CCoordinateCalculator calculator;
	return calculator.AbsoluteToRelative(GetCoordinateCalculatorData(point), point);
}

CRect CSymbolTool::GetCurDrawingRegion(const CPoint &point)
{
	int nRow, nColumn;
	IChartManager* pIChartManager = m_pSymbolData->GetMainBlock();
	if( !pIChartManager) return CRect( 0, 0, 0, 0);
	pIChartManager->GetBlockIndex( point, nRow, nColumn);
	CBlockIndex blockIndex( nRow, nColumn);

	CRect rctResult;
	if( !blockIndex.IsAllValueBiggerThanZero()) rctResult =  CRect(0,0,0,0);
	{
		IBlock *pIBlock = pIChartManager->GetBlock( blockIndex.GetRow(), blockIndex.GetColumn());
		if( !pIBlock) rctResult = CRect( 0, 0, 0, 0);
		else
		{
			CRect rctNoFrameAndMargin;
			pIBlock->GetGraphRegions( rctResult, rctNoFrameAndMargin);
			pIBlock->Release();
			pIBlock = NULL;
		}
	}
	pIChartManager->Release();
	return rctResult;
}

/////////////////////////////////////////////////////////////////////
// OnePoint and Size Tool
/////////////////////////////////////////////////////////////////////
//
// data 초기화
void CSymbolTool_OnePoint_Size::Initial()
{
	m_startPt = CPoint(-10, -10);
	SetDrawStart(false);	
}

void CSymbolTool_OnePoint_Size::OnDraw(CDC* pDC)
{

}

CDrawingSymbol* CSymbolTool_OnePoint_Size::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	m_bDrawStart = true;
	m_startPt = point;

	return NULL;
}

void CSymbolTool_OnePoint_Size::OnLButtonUp(CDC* pDC, const CPoint& point, const CSymbolBaseData::TOOLTYPE toolType)
{
	m_bDrawStart = false;
	CSymbolTool::OnLButtonUp(pDC, GetStartPoint(), point, toolType); 
}

bool CSymbolTool_OnePoint_Size::OnMouseMove(CDC *pDC,const UINT &nFlags, const CPoint &point)
{
	return true;
}

CPoint CSymbolTool_OnePoint_Size::GetStartPoint()
{
	return m_startPt;
}

CSize CSymbolTool_OnePoint_Size::GetSymbolSize()
{
	return m_SymbolSize;
}

void CSymbolTool_OnePoint_Size::SetStartPoint(const CPoint& startpt)
{
	m_startPt = startpt;
}

CPoint CSymbolTool_OnePoint_Size::GetDrawingPoint()
{
	return m_ptDrawingPoint;
}

void CSymbolTool_OnePoint_Size::SavePointAndDraw(CDC* pDC, const CPoint& point, const int drawMode)
{
	m_startPt = point;

	CSymbolData* pSymbolData = GetSymbolData();
	if(pSymbolData == NULL)
		return;
	
	CPropertiesData_text* pTextData = pSymbolData->GetPropertiesData();
	if(pTextData == NULL)
		return;

	GetOnePointDrawer().Draw(pDC, pTextData->GetColor(),
			pTextData->GetLogFont(), pTextData->GetTextColor(), 
			pTextData->GetSizeType(), m_startPt, GetCurDrawingRegion(point));
}

// element 만들어 list에 추가
void CSymbolTool_OnePoint_Size::AddElement(const CSymbolBaseData::TOOLTYPE toolType, const CPoint& drawingPoint)
{
	m_ptDrawingPoint = drawingPoint;
	CSymbolData* pSymbolData = GetSymbolData();
	if(pSymbolData == NULL)
		return;

	IChartManager *pIChartManager = pSymbolData->GetMainBlock();
	CBlockIndex blockIndex;
	BOOL bResult = GetBlockIndex( pIChartManager, drawingPoint, blockIndex);
	if( pIChartManager) pIChartManager->Release();
	if(! bResult) return;

	CDrawingSymbol* newDrawingSymbol = MakeDrawingSymbol(pSymbolData, toolType, drawingPoint);
	if(newDrawingSymbol == NULL)
		return;

	pSymbolData->RegisterElementToOrderManager((long)newDrawingSymbol);
	pSymbolData->GetElementTree()->Add(blockIndex, newDrawingSymbol);

	// Undo / Redo
	pSymbolData->GetUndoRedoManager().RegisterElementToUnRedoManager(URT_NEW_OR_DEL, (long)newDrawingSymbol);
}

//sy 2005.04.06
bool CSymbolTool_OnePoint_Size::GetBlockIndex(IChartManager* pMainBlock, const CPoint& point, CBlockIndex& blockIndex) const
{
	if(pMainBlock == NULL)
		return false;

	int nRow, nColumn;
	if( !pMainBlock->GetBlockIndex( point, nRow, nColumn)) return false;
	blockIndex.SetIndex( nRow, nColumn);
	return blockIndex.IsAllValueBiggerThanZero();
}

CDrawingSymbol* CSymbolTool_OnePoint_Size::MakeDrawingSymbol(CSymbolData* pSymbolData, const CSymbolBaseData::TOOLTYPE toolType, const CPoint& drawingPoint)
{
	if(pSymbolData == NULL)
		return NULL;

	if(pSymbolData->GetSymbolType() == CSymbolBaseData::SYMBOL_TEXT)
		return NULL;

	CPropertiesData_text* pTextData = pSymbolData->GetPropertiesData();
	if(pTextData == NULL)
		return NULL;
		
	return CDrawingSymbolFactory::Make(toolType, pTextData->GetSizeType(), 
		GetSavingPoint(GetStartPoint()), GetSymbolSize(), pTextData->GetColor(), 
		pTextData->GetLogFont(), pTextData->GetTextColor());
}

/////////////////////////////////////////////////////////////////////
// Two Point
/////////////////////////////////////////////////////////////////////
//
// data 초기화
void CSymbolTool_TwoPoint::Initial()
{
	m_StartPt = m_EndPt = CPoint(-10, -10);
	SetDrawStart(false);
}

CDrawingSymbol* CSymbolTool_TwoPoint::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	m_bDrawStart = true;
	m_StartPt = m_EndPt = point;

 	return NULL;
}	

void CSymbolTool_TwoPoint::OnLButtonUp(CDC* pDC, const CPoint& point, const CSymbolBaseData::TOOLTYPE toolType)
{
	m_bDrawStart = false;
	CSymbolTool::OnLButtonUp(pDC, GetStartPoint(), point, toolType); 
}

bool CSymbolTool_TwoPoint::OnMouseMove(CDC *pDC,const UINT &nFlags, const CPoint &point)
{
	//봉하나의 크기만큼 이동했을때만 drag 그림을 그린다.
	CCoordinateCalculatorData calData = GetCoordinateCalculatorData(point);
 	CRect drawingRegion = GetCurDrawingRegion(m_StartPt);

	if(IsDrawStart() && IsPointInBlock(drawingRegion, m_StartPt)) {
		CCoordinateCalculator calculator;
		CPoint ptAbsoluteStart = calculator.AbsoluteToAbsolute(calData, GetStartPoint());
		CPoint ptAbsoluteEnd = calculator.AbsoluteToAbsolute(calData, GetEndPoint());
		CPoint ptAbsoluteCurrent = calculator.AbsoluteToAbsolute(calData, point);

		if(ptAbsoluteEnd.x == ptAbsoluteCurrent.x &&
		   ptAbsoluteEnd.y == ptAbsoluteCurrent.y)
		   return false;

		int oldROP = pDC->SetROP2(R2_NOTXORPEN);    
		if(GetEndPoint().x != -1 && GetEndPoint().y != -1) {
			GetTwoPointDrawer().DrawRectangle(pDC,
				ptAbsoluteStart,
				ptAbsoluteEnd, drawingRegion);
		}
		GetTwoPointDrawer().DrawRectangle(pDC, ptAbsoluteStart, ptAbsoluteCurrent, drawingRegion);
		pDC->SetROP2(oldROP);
		m_EndPt = point;
	}
	return true;
}

CPoint CSymbolTool_TwoPoint::GetStartPoint()
{
	return m_StartPt;
}

CPoint CSymbolTool_TwoPoint::GetEndPoint()
{
	return m_EndPt;
}

void CSymbolTool_TwoPoint::SetStartPoint(const CPoint& startpt)
{
	m_StartPt = startpt;
}

void CSymbolTool_TwoPoint::SetEndPoint(const CPoint& endpt)
{
	m_EndPt = endpt;
}

void CSymbolTool_TwoPoint::SavePointAndDraw(CDC *pDC, const CPoint &point, const int drawMode)
{
	//색깔이 안 
	ReplaceStartandEndpt(point);

	GetTwoPointDrawer().Draw(pDC,
			GetSymbolData()->GetPropertiesData()->GetColor(), 
		    GetSymbolData()->GetPropertiesData()->GetStyle(),
		    GetSymbolData()->GetPropertiesData()->GetWeight(),
			GetSymbolData()->GetPropertiesData()->GetLogFont(),
			GetSymbolData()->GetPropertiesData()->GetFillState(),
			m_StartPt,
			m_EndPt,
			GetCurDrawingRegion(point),
			m_bDrawFromTop);
}

// element 만들어 list에 추가
void CSymbolTool_TwoPoint::AddElement(const CSymbolBaseData::TOOLTYPE toolType, const CPoint& drawingPoint)
{
	IChartManager *pIChartManager = GetSymbolData()->GetMainBlock();
	if( !pIChartManager) return;

	int nRow, nColumn;
	if( pIChartManager->GetBlockIndex( drawingPoint, nRow, nColumn))
	{
		CBlockIndex blockIndex( nRow, nColumn);
		if(!blockIndex.IsAllValueBiggerThanZero())
		{
			pIChartManager->Release();
			return;
		}

		CSymbolData* pSymbolData = GetSymbolData();
		if(pSymbolData == NULL)
		{
			pIChartManager->Release();
			return;
		}

		CDrawingSymbol* newDrawingSymbol = CDrawingSymbolFactory::Make(toolType,
							   GetSavingPoint(GetStartPoint()), GetSavingPoint(GetEndPoint()),
							   pSymbolData->GetPropertiesData()->GetColor(), 
							   pSymbolData->GetPropertiesData()->GetStyle(),
							   pSymbolData->GetPropertiesData()->GetWeight(),
							   pSymbolData->GetPropertiesData()->GetFillState(),
							   pSymbolData->GetPropertiesData()->GetLogFont(), m_bDrawFromTop);
		if(newDrawingSymbol == NULL)
		{
			pIChartManager->Release();
			return;
		}

		long lAddress = (long)newDrawingSymbol;
		pSymbolData->RegisterElementToOrderManager(lAddress);
		CDrawingSymbol* anewDrawingSymbol = (CDrawingSymbol*)lAddress;
		pSymbolData->GetElementTree()->Add(blockIndex,newDrawingSymbol);		

		// Undo / Redo
		pSymbolData->GetUndoRedoManager().RegisterElementToUnRedoManager(URT_NEW_OR_DEL, lAddress);
	}
	pIChartManager->Release();
}
//////////////////////////////////////////////////////////////////////
////SelectTool
//////////////////////////////////////////////////////////////////////

CSelectTool::CSelectTool()
{
	Initial();
}

void CSelectTool::Initial()
{
	//CSymbolTool::Initial();
	m_oldMovePoint.x = 0;
	m_oldMovePoint.y = 0;
	m_LButtonPoint = CPoint(-1,-1);
	SetDrawStart(false);
	m_pDrawingSymbol = NULL;
	m_eModifyType = CSymbolBaseData::MODIFY_NONE;
}

CDrawingSymbol* CSelectTool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	m_ptOldMouse = point;
	m_pDrawingSymbol = GetSymbolData()->FindSymbol(point);
	if(m_pDrawingSymbol == NULL)
		return NULL;

	if(m_eModifyType == CSymbolBaseData::MODIFY_END)
		m_eModifyType = CSymbolBaseData::MODIFY_DROP;

	m_LButtonPoint = point;
	SetDrawStart(true);
	return m_pDrawingSymbol;
}

void CSelectTool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	if(!GetCurDrawingRegion(m_LButtonPoint).PtInRect(point)) {
		Initial(); //객체 이동 정지
		return;
	}

	if(m_pDrawingSymbol != NULL) 
	{
		CPoint ptGap(0,0);
		CSymbolBaseData::TOOLTYPE eType = m_pDrawingSymbol->GetSymbolType();
		CCoordinateCalculator calculator;
		CCoordinateCalculatorData calData = GetCoordinateCalculatorData(point);

		CString strSymbolData_Prev;
		CElementSaverBuilder builder;
		strSymbolData_Prev = builder.GetSavedSymbolData_ForUndoRedo(GetSymbolData()->GetElementTree()->GetOrder(), m_pDrawingSymbol);
		if(eType == CSymbolBaseData::SYMBOL_TEXT)
		{
			CCoordinate coordStart = m_pDrawingSymbol->GetStartPoint();
			ptGap = GetGapMouseMove(calculator.RelativeToAbsolute(calData, coordStart), calculator.AbsoluteToAbsolute(calData, m_LButtonPoint));
		}
		else
		{
			ptGap = GetGapMouseMove(calculator.AbsoluteToAbsolute(calData, m_LButtonPoint), calculator.AbsoluteToAbsolute(calData, point));		
		}
		

		if(m_eModifyType == CSymbolBaseData::MODIFY_NONE)
		{
			// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
			//m_pDrawingSymbol->TranslatePoint(false, calculator.AbsoluteToAbsolute(calData, point),
			//	ptGap,GetCurDrawingRegion(point), calData);
			if (m_LButtonPoint == point)
			{
				// 단순, 심볼 L클릭인 경우는 (즉, 심볼 MOVE가 아닌경우)
				// 심볼 이동 TranslatePoint 변환할 필요가 없다.
			}
			else
			{
				m_pDrawingSymbol->TranslatePoint(false, calculator.AbsoluteToAbsolute(calData, point),
					ptGap,GetCurDrawingRegion(point), calData);
			}
			// 2011.02.07 by SYS <<
		}
		else if(m_eModifyType == CSymbolBaseData::MODIFY_DROP)
		{
			m_pDrawingSymbol->TranslatePoint(true, calculator.AbsoluteToAbsolute(calData, point),
							ptGap,GetCurDrawingRegion(point), calData);
		}

		GetSymbolData()->MoveSymbol(m_pDrawingSymbol, strSymbolData_Prev);
	}
	//Initial(); //객체 이동 정지
	m_LButtonPoint = CPoint(-1,-1);
}

void CSelectTool::OnDraw(CDC* pDC)
{
	if(m_pDrawingSymbol == NULL) 
		return;

	if(!IsDrawStart()) 
		return;

	if(IsFigure()) //도형일때만 size 조절이 가능하다
		OnMouseMove_Figure(pDC, m_ptOldMouse);
	else
		OnMouseMove_Symbol(pDC, m_ptOldMouse);
}
 
bool CSelectTool::IsFigure()
{
	CSymbolBaseData::TOOLTYPE eType = m_pDrawingSymbol->GetSymbolType();
	if(eType == CSymbolBaseData::SYMBOL_SQUARE || eType == CSymbolBaseData::SYMBOL_CIRCLE || eType == CSymbolBaseData::SYMBOL_TRIANGLE) //사각, 원, 삼각
		return true;
	return false;
}

bool CSelectTool::OnMouseMove(CDC *pDC,const UINT &nFlags, const CPoint &point)
{
	if(nFlags != MK_LBUTTON)
		return false;

	m_ptOldMouse = point;
	if(!IsDrawStart()) 
		m_pDrawingSymbol = GetSymbolData()->FindSymbol(m_LButtonPoint);

	if(m_pDrawingSymbol == NULL) 
		return false;

	m_oldMovePoint = point;
	GetSymbolData()->Invalidate();
//	if(IsFigure()) //도형일때만 size 조절이 가능하다
//		OnMouseMove_Figure(pDC, point);
//	else
//		OnMouseMove_Symbol(pDC, point);

	return true;
}

void CSelectTool::OnMouseMove_Figure(CDC *pDC, const CPoint &point)
{
	//이동할때는 커서를 바꾸지 않는다.
	//이동할때 커서를 바꾸면, end에서 none로 바뀌어버리고 size 조절이 아닌, 이동만 되버린다.
	if(m_eModifyType != CSymbolBaseData::MODIFY_DROP) { 
		ChangeMousePosition(point);
		if(m_eModifyType == CSymbolBaseData::MODIFY_END) 
			CSymbolTool::ChangeCursor(CCursorShape::TOOL_SIZENESW);
		else
			CSymbolTool::ChangeCursor(CCursorShape::TOOL_SIZEALL);
	}

	if(m_pDrawingSymbol != NULL && IsDrawStart()) 
	{
		BOOL bTextMoving = FALSE;
		if( m_pDrawingSymbol->GetSymbolType() == CSymbolBaseData::SYMBOL_TEXT || 
			m_pDrawingSymbol->GetSymbolType() == CSymbolBaseData::SYMBOL_SQUARE ||
			m_pDrawingSymbol->GetSymbolType() == CSymbolBaseData::SYMBOL_BULET)
			bTextMoving = TRUE;

		CCoordinateCalculatorData coordinateData = GetCoordinateCalculatorData(m_LButtonPoint);
		if(m_eModifyType == CSymbolBaseData::MODIFY_NONE)
			m_pDrawingSymbol->DrawSelectedMovingMark(pDC, m_oldMovePoint, point, m_LButtonPoint, GetCurDrawingRegion(point), coordinateData,bTextMoving); 
		else if(m_eModifyType == CSymbolBaseData::MODIFY_DROP)
			m_pDrawingSymbol->DrawbyMovedSize(pDC, m_oldMovePoint, point, GetCurDrawingRegion(point), coordinateData);
		//m_oldMovePoint = point;		
	}
}

void CSelectTool::OnMouseMove_Symbol(CDC *pDC, const CPoint &point)
{
	CSymbolTool::ChangeCursor(CCursorShape::TOOL_SIZEALL);
	m_eModifyType = CSymbolBaseData::MODIFY_NONE;

    //객체 이동할때 선표시
	if(m_pDrawingSymbol != NULL && IsDrawStart()) 
	{
		BOOL bTextMoving = FALSE;
		if(m_pDrawingSymbol->GetSymbolType() == CSymbolBaseData::SYMBOL_TEXT)
			bTextMoving = TRUE;

		CCoordinateCalculatorData calcData = GetCoordinateCalculatorData(m_LButtonPoint);
		m_pDrawingSymbol->DrawSelectedMovingMark(pDC, m_oldMovePoint, point, m_LButtonPoint, GetCurDrawingRegion(point), calcData,bTextMoving); 

		m_oldMovePoint = point;
	}
}

CRect CSelectTool::Draw(CDC* pDC, CDrawingSymbolRow* pDrawingSymbolMgr)
{
	return CRect(0,0,0,0);
}

CPoint CSelectTool::GetGapMouseMove(const CPoint &pt1, const CPoint &pt2)
{
	return CPoint(pt2.x - pt1.x, pt2.y - pt1.y);
}

void CSelectTool::SavePointAndDraw(CDC *pDC, const CPoint &point, const int drawMode)
{
	m_pDrawingSymbol->DrawSelectedMark(pDC,
			GetCurDrawingRegion(point),
			GetCoordinateCalculatorData(point));
}

void CSelectTool::ChangeMousePosition(const CPoint& curpos)
{
//invert
	CCoordinateCalculator calculator;

	CPoint abs_curpos = GetDrawingPoint(curpos);
	CCoordinateCalculatorData calData = GetCoordinateCalculatorData(curpos);
	CPoint abs_objectendpos = calculator.RelativeToAbsolute(calData, m_pDrawingSymbol->GetEndPoint());

CRect rc(abs_objectendpos.x - 10, abs_objectendpos.y - 10, abs_objectendpos.x + 15, abs_objectendpos.y + 15);
//	CRect rc(abs_objectendpos.x - 6, abs_objectendpos.y - 6, abs_objectendpos.x + 15, abs_objectendpos.y +15);
	if(rc.PtInRect(abs_curpos))
		m_eModifyType = CSymbolBaseData::MODIFY_END;
	else
		m_eModifyType = CSymbolBaseData::MODIFY_NONE;
}

// element 만들어 list에 추가
void CSelectTool::AddElement(const CSymbolBaseData::TOOLTYPE toolType, const CPoint& drawingPoint)
{
}

//////////////////////////////////////////////////////////////////////
// //도형 - 네모
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CFigure_Square_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_TwoPoint::OnLButtonDown(pDC, point);
}

void CFigure_Square_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_TwoPoint::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_SQUARE);
}

bool CFigure_Square_Tool::OnMouseMove(CDC *pDC,const UINT &nFlags, const CPoint &point)
{
	return CSymbolTool_TwoPoint::OnMouseMove(pDC,nFlags, point);
}

bool CFigure_Square_Tool::GetRightPoint(const CPoint &startpoint, const CPoint &endpoint)
{
	if(startpoint.x < endpoint.x && startpoint.y < endpoint.y)
		return true;
	return false;
}

CSymbolDraw_TwoPoint& CFigure_Square_Tool::GetTwoPointDrawer()
{
	return m_squaredrawer;
}

//////////////////////////////////////////////////////////////////////
////도형 - 동그라미
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CFigure_Circle_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_TwoPoint::OnLButtonDown(pDC, point);
}

void CFigure_Circle_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_TwoPoint::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_CIRCLE);
}

bool CFigure_Circle_Tool::OnMouseMove(CDC *pDC,const UINT &nFlags, const CPoint &point)
{
	return CSymbolTool_TwoPoint::OnMouseMove(pDC,nFlags, point);
}

CSymbolDraw_TwoPoint& CFigure_Circle_Tool::GetTwoPointDrawer()
{
	return m_circledrawer;
}


//////////////////////////////////////////////////////////////////////
////도형 - 삼각형
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CFigure_Triangle_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_TwoPoint::OnLButtonDown(pDC, point);
}

void CFigure_Triangle_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_TwoPoint::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_TRIANGLE);
}

bool CFigure_Triangle_Tool::OnMouseMove(CDC *pDC,const UINT &nFlags, const CPoint &point)
{
	return CSymbolTool_TwoPoint::OnMouseMove(pDC,nFlags, point);
}

CSymbolDraw_TwoPoint& CFigure_Triangle_Tool::GetTwoPointDrawer()
{
	return m_triangleDrawer;
}

//////////////////////////////////////////////////////////////////////
// //text
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CTextTool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CTextTool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	m_textDrawer.SetParent(GetSymbolData()->GetParent());
	m_textDrawer.SetDrawingRegion(GetCurDrawingRegion(point));
	m_textDrawer.SetTextToolObject(this);
	//sy 2005.04.06.
	//기호dlg의 기호들과 다른게 color 적용.
	COLORREF oldcolor = GetSymbolData()->GetPropertiesData()->GetColor();
	//GetSymbolData()->GetPropertiesData()->SetColor(RGB(0,0,0));

	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_TEXT);
	//GetSymbolData()->GetPropertiesData()->SetColor(oldcolor);
}

CPoint CTextTool::GetStartPoint()
{
	return CSymbolTool_OnePoint_Size::GetStartPoint(); 
}

LOGFONT CTextTool::GetLogFont()
{
	return CSymbolTool::GetLogFont();
/*
	LOGFONT logfont;
	memset(&logfont, 0, sizeof(LOGFONT));       // zero out structure
	lstrcpy(logfont.lfFaceName, "굴림");
	logfont.lfWidth = 7;
	logfont.lfHeight = 16;
	logfont.lfWeight = FW_NORMAL;
	logfont.lfItalic = FALSE;
	logfont.lfUnderline = FALSE;
	logfont.lfStrikeOut = FALSE;

	logfont.lfPitchAndFamily = FIXED_PITCH;
	logfont.lfQuality = DEFAULT_QUALITY;
	logfont.lfCharSet = HANGUL_CHARSET;
	logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	return logfont;
*/
}

//point가 Block안에 있는지 확인한다.
bool CTextTool::IsPointInBlock(const CRect &drawingRegion, const CPoint &point)
{
	//기본 edit 크기
	CRect rcsymbol(GetStartPoint().x, GetStartPoint().y, GetStartPoint().x + 25, GetStartPoint().y + 20);
	if(drawingRegion.PtInRect(point) && drawingRegion.PtInRect(rcsymbol.BottomRight()))
		return true;
	
	return false;
}

void CTextTool::SetEditText(const CString &strText)
{
	if(strText.IsEmpty() == FALSE)
	{
		CSymbolData* pSymbolData = GetSymbolData();
		if(pSymbolData == NULL)
			return;

		CPropertiesData_text* pTextData = pSymbolData->GetPropertiesData();
		if(pTextData == NULL)
			return;

		CDrawingSymbol* newDrawingSymbol = CDrawingSymbolFactory::Make(CSymbolBaseData::SYMBOL_TEXT,pTextData->GetSizeType(), 
											GetSavingPoint(GetDrawingPoint()), GetSymbolSize(), pTextData->GetColor(), 
											pTextData->GetLogFont(), pTextData->GetTextColor());
		
		IChartManager *pIChartManager = pSymbolData->GetMainBlock();
		CBlockIndex blockIndex;
		int nRow, nColumn;
		if( !pIChartManager->GetBlockIndex( GetDrawingPoint(), nRow, nColumn)) 
		{
			if( pIChartManager) pIChartManager->Release();
			return;
		}
		blockIndex.SetIndex( nRow, nColumn);
		
		BOOL bResult = blockIndex.IsAllValueBiggerThanZero();
		if( pIChartManager) pIChartManager->Release();
		if(! bResult) return;

		newDrawingSymbol->SetText(strText);

		pSymbolData->RegisterElementToOrderManager((long)newDrawingSymbol);
		pSymbolData->GetElementTree()->Add(blockIndex, newDrawingSymbol);		

		// Undo / Redo
		pSymbolData->GetUndoRedoManager().RegisterElementToUnRedoManager(URT_NEW_OR_DEL, (long)newDrawingSymbol);
	}

	SetDrawStart(false);
}

CSymbolDraw_OnePoint_Size& CTextTool::GetOnePointDrawer()
{
	return m_textDrawer;
}

CWnd* CTextTool::GetParent()
{
	return GetSymbolData()->GetParent();
}

bool CTextTool::Delete()
{
	bool bResult = false;
	CDrawingSymbol* pdrawingsymbol = GetSymbolData()->GetElementTree()->GetTail();
	if(pdrawingsymbol != NULL) 
	{
		bResult = GetSymbolData()->GetElementTree()->DeleteCurrentDrawingSymbol(pdrawingsymbol);
		GetSymbolData()->Invalidate();
		return bResult;
	}
	return bResult;
}

//////////////////////////////////////////////////////////////////////
// //LeftArrow
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CArrow_Left_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CArrow_Left_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_ARROW_LEFT);
}

CSymbolDraw_OnePoint_Size& CArrow_Left_Tool::GetOnePointDrawer()
{
	return m_arrowleftDrawer;
}

//////////////////////////////////////////////////////////////////////
// //rightArrow
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CArrow_Right_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CArrow_Right_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_ARROW_RIGHT);
}

CSymbolDraw_OnePoint_Size& CArrow_Right_Tool::GetOnePointDrawer()
{
	return m_arrowrightDrawer;
}

//////////////////////////////////////////////////////////////////////
// //ARROW_UP
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CArrow_Up_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CArrow_Up_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_ARROW_UP);
}


CSymbolDraw_OnePoint_Size& CArrow_Up_Tool::GetOnePointDrawer()
{
	return m_arrowUpDrawer;	
}

//////////////////////////////////////////////////////////////////////
// //ARROW_DOWN
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CArrow_Down_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CArrow_Down_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_ARROW_DOWN);
}

CSymbolDraw_OnePoint_Size& CArrow_Down_Tool::GetOnePointDrawer()
{
	return m_arrowdownDrawer;
}

//////////////////////////////////////////////////////////////////////
// //ARROW_LEFTTOP
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CArrow_LeftTop_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CArrow_LeftTop_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_ARROW_LEFTTOP);
}

CSymbolDraw_OnePoint_Size& CArrow_LeftTop_Tool::GetOnePointDrawer()
{
	return m_arrowLeftTopDrawer;
}

//////////////////////////////////////////////////////////////////////
// //ARROW_LEFTBOTTOM
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CArrow_LeftBottom_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CArrow_LeftBottom_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_ARROW_LEFTBOTTOM);
}

CSymbolDraw_OnePoint_Size& CArrow_LeftBottom_Tool::GetOnePointDrawer()
{
	return m_arrowLeftBottomDrawer;
}

//////////////////////////////////////////////////////////////////////
// //ARROW_RIGHTTOP
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CArrow_RightTop_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CArrow_RightTop_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_ARROW_RIGHTTOP);
}

CSymbolDraw_OnePoint_Size& CArrow_RightTop_Tool::GetOnePointDrawer()
{
	return m_arrowRightTopDrawer;
}

//////////////////////////////////////////////////////////////////////
// //ARROW_RIGHTBOTTOM
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CArrow_RightBottom_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CArrow_RightBottom_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_ARROW_RIGHTBOTTOM);
}

CSymbolDraw_OnePoint_Size& CArrow_RightBottom_Tool::GetOnePointDrawer()
{
	return m_arrowRightBottomDrawer;
}

//////////////////////////////////////////////////////////////////////
// //NUM_One
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CNumber_One_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CNumber_One_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_NUM_ONE);
}

CSymbolDraw_OnePoint_Size& CNumber_One_Tool::GetOnePointDrawer()
{
	return m_numberOneDrawer;
}

//////////////////////////////////////////////////////////////////////
// //NUM_Two
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CNumber_Two_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CNumber_Two_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_NUM_TWO);
}

CSymbolDraw_OnePoint_Size& CNumber_Two_Tool::GetOnePointDrawer()
{
	return m_numberTwoDrawer;
}

//////////////////////////////////////////////////////////////////////
////NUM_Three
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CNumber_Three_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CNumber_Three_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_NUM_THREE);
}

CSymbolDraw_OnePoint_Size& CNumber_Three_Tool::GetOnePointDrawer()
{
	return m_numberThreeDrawer;
}

//////////////////////////////////////////////////////////////////////
// //NUM_Four
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CNumber_Four_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CNumber_Four_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_NUM_FOUR);
}

CSymbolDraw_OnePoint_Size& CNumber_Four_Tool::GetOnePointDrawer()
{
	return m_numberFourDrawer;
}

//////////////////////////////////////////////////////////////////////
// //NUM_Five
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CNumber_Five_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CNumber_Five_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_NUM_FIVE);
}

CSymbolDraw_OnePoint_Size& CNumber_Five_Tool::GetOnePointDrawer()
{
	return m_numberFiveDrawer;
}

//////////////////////////////////////////////////////////////////////
// //NUM_Six
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CNumber_Six_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CNumber_Six_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_NUM_SIX);
}

CSymbolDraw_OnePoint_Size& CNumber_Six_Tool::GetOnePointDrawer()
{
	return m_numberSixDrawer;
}

//////////////////////////////////////////////////////////////////////
// //NUM_Seven
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CNumber_Seven_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CNumber_Seven_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_NUM_SEVEN);
}

CSymbolDraw_OnePoint_Size& CNumber_Seven_Tool::GetOnePointDrawer()
{
	return m_numberSevenDrawer;
}

//////////////////////////////////////////////////////////////////////
// //NUM_Eight
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CNumber_Eight_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CNumber_Eight_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_NUM_EIGHT);
}

CSymbolDraw_OnePoint_Size& CNumber_Eight_Tool::GetOnePointDrawer()
{
	return m_numberEightDrawer;
}

//////////////////////////////////////////////////////////////////////
// //NUM_Nine
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CNumber_Nine_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CNumber_Nine_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_NUM_NINE);
}

CSymbolDraw_OnePoint_Size& CNumber_Nine_Tool::GetOnePointDrawer()
{
	return m_numberNineDrawer;
}

//////////////////////////////////////////////////////////////////////
// //Text_A
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CText_A_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CText_A_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_TEXT_A);
}

CSymbolDraw_OnePoint_Size& CText_A_Tool::GetOnePointDrawer()
{
	return m_textADrawer;
}

//////////////////////////////////////////////////////////////////////
// //Text_B
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CText_B_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CText_B_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_TEXT_B);
}

CSymbolDraw_OnePoint_Size& CText_B_Tool::GetOnePointDrawer()
{
	return m_textBDrawer;
}

//////////////////////////////////////////////////////////////////////
// //Text_C
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CText_C_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CText_C_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_TEXT_C);
}

CSymbolDraw_OnePoint_Size& CText_C_Tool::GetOnePointDrawer()
{
	return m_textCDrawer;
}

//////////////////////////////////////////////////////////////////////
// //Text_D
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CText_D_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CText_D_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_TEXT_D);
}

CSymbolDraw_OnePoint_Size& CText_D_Tool::GetOnePointDrawer()
{
	return m_textDDrawer;
}

//////////////////////////////////////////////////////////////////////
// //Text_E
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CText_E_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CText_E_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_TEXT_E);
}

CSymbolDraw_OnePoint_Size& CText_E_Tool::GetOnePointDrawer()
{
	return m_textEDrawer;
}

//////////////////////////////////////////////////////////////////////
// //Text_F
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CText_F_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CText_F_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_TEXT_F);
}

CSymbolDraw_OnePoint_Size& CText_F_Tool::GetOnePointDrawer()
{
	return m_textFDrawer;
}

//////////////////////////////////////////////////////////////////////
// //Text_G
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CText_G_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CText_G_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_TEXT_G);
}

CSymbolDraw_OnePoint_Size& CText_G_Tool::GetOnePointDrawer()
{
	return m_textGDrawer;
}

//////////////////////////////////////////////////////////////////////
// //Text_H
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CText_H_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CText_H_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_TEXT_H);
}

CSymbolDraw_OnePoint_Size& CText_H_Tool::GetOnePointDrawer()
{
	return m_textHDrawer;
}

//////////////////////////////////////////////////////////////////////
// //CHAR_1
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CChar_1_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CChar_1_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_CHAR_1);
}

CSymbolDraw_OnePoint_Size& CChar_1_Tool::GetOnePointDrawer()
{
	return m_char1Drawer;
}

//////////////////////////////////////////////////////////////////////
// //CHAR_2
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CChar_2_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CChar_2_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_CHAR_2);
}

CSymbolDraw_OnePoint_Size& CChar_2_Tool::GetOnePointDrawer()
{
	return m_char2Drawer;
}

//////////////////////////////////////////////////////////////////////
// //CHAR_3
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CChar_3_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CChar_3_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_CHAR_3);
}

CSymbolDraw_OnePoint_Size& CChar_3_Tool::GetOnePointDrawer()
{
	return m_char3Drawer;
}

//////////////////////////////////////////////////////////////////////
// //CHAR_4
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CChar_4_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CChar_4_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_CHAR_4);
}

CSymbolDraw_OnePoint_Size& CChar_4_Tool::GetOnePointDrawer()
{
	return m_char4Drawer;
}

//////////////////////////////////////////////////////////////////////
// //CHAR_5
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CChar_5_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CChar_5_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_CHAR_5);
}

CSymbolDraw_OnePoint_Size& CChar_5_Tool::GetOnePointDrawer()
{
	return m_char5Drawer;
}

//////////////////////////////////////////////////////////////////////
// //CHAR_6
//////////////////////////////////////////////////////////////////////
CDrawingSymbol* CChar_6_Tool::OnLButtonDown(CDC *pDC, const CPoint &point)
{
	return CSymbolTool_OnePoint_Size::OnLButtonDown(pDC, point);
}

void CChar_6_Tool::OnLButtonUp(CDC *pDC, const CPoint &point)
{
	CSymbolTool_OnePoint_Size::OnLButtonUp(pDC, point, CSymbolBaseData::SYMBOL_CHAR_6);
}

CSymbolDraw_OnePoint_Size& CChar_6_Tool::GetOnePointDrawer()
{
	return m_char6Drawer;
}

void CSymbolTool_TwoPoint::ReplaceStartandEndpt(const CPoint &currentPt)
{
	//오른쪽 아래에서 왼쪽 위로 그릴때.
	if(currentPt.x < m_StartPt.x && currentPt.y < m_StartPt.y) {
		m_bDrawFromTop = false;
		m_EndPt.x = m_StartPt.x;
		m_EndPt.y = m_StartPt.y;

		m_StartPt.x = currentPt.x;
		m_StartPt.y = currentPt.y;
	}
	//오른쪽 위에서 왼쪽 아래로 그릴때..
	else if(currentPt.x < m_StartPt.x) { 
		m_bDrawFromTop = true;
		m_EndPt.x = m_StartPt.x;
		m_StartPt.x = currentPt.x;
	}
	//왼쪽 아래에서 오른쪽 위로 그릴때.
	else if(currentPt.y < m_StartPt.y) {
		m_bDrawFromTop = false;
		m_EndPt.y = m_StartPt.y;
		m_StartPt.y = currentPt.y;
	}
	else {
		m_bDrawFromTop = true;
		m_EndPt = currentPt;
	}
}

void CSymbolTool_TwoPoint::OnDraw(CDC *pDC)
{
	//봉하나의 크기만큼 이동했을때만 drag 그림을 그린다.
	CCoordinateCalculatorData calData = GetCoordinateCalculatorData(GetEndPoint());
 	CRect drawingRegion = GetCurDrawingRegion(m_StartPt);

	if(IsDrawStart() && IsPointInBlock(drawingRegion, m_StartPt)) {
		CCoordinateCalculator calculator;
		CPoint ptAbsoluteStart = calculator.AbsoluteToAbsolute(calData, GetStartPoint());
		CPoint ptAbsoluteEnd = calculator.AbsoluteToAbsolute(calData, GetEndPoint());

		int oldROP = pDC->SetROP2(R2_NOTXORPEN);    
		GetTwoPointDrawer().DrawRectangle(pDC, ptAbsoluteStart, ptAbsoluteEnd, drawingRegion);
		pDC->SetROP2(oldROP);
	}
}