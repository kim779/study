// DrawingSymbol.cpp: implementation of the CDrawingSymbol class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "symbol.h"
#include "DrawingSymbol.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "CoordinateCalculator.h"
//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////

CDrawingSymbol::CDrawingSymbol()
{
	m_bFill = false;
	m_textColor = RGB(0, 0, 0);
	m_Color = RGB(255, 255, 255);
	m_Style = PS_SOLID;
	m_Weight = 1;
	m_SizeType = CSymbolBaseData::Small;
	m_SymbolType = CSymbolBaseData::SYMBOL_NONE;
}

void CDrawingSymbol::SetColor(const COLORREF& color)
{
	m_Color = color;
}

COLORREF CDrawingSymbol::GetColor() const
{
	return m_Color;
}

void CDrawingSymbol::SetStyle(const int& style)
{
	m_Style = style;
}

int CDrawingSymbol::GetStyle() const
{
	return m_Style;
}

void CDrawingSymbol::SetWeight(const int& weight)
{
	m_Weight = weight;
}

int CDrawingSymbol::GetWeight() const
{
	return m_Weight;
}

CSymbolBaseData::SIZETYPE CDrawingSymbol::GetSizeType() const
{
	return m_SizeType;
}

CSymbolBaseData::TOOLTYPE CDrawingSymbol::GetSymbolType()
{
	return m_SymbolType;
}

//point가 Block안에 있는지 확인한다.
bool CDrawingSymbol::IsPointInBlock(const CRect &drawingRegion, const CPoint &point)
{
	if(drawingRegion.PtInRect(point))
		return true;

	return false;
}

void CDrawingSymbol::SetSymbolType(const CSymbolBaseData::TOOLTYPE SymbolType)
{
	m_SymbolType = SymbolType;
}

void CDrawingSymbol::SetSizeType(const CSymbolBaseData::SIZETYPE& sizetype)
{
	m_SizeType = sizetype;
}

//void CDrawingSymbol::SetText(CString strText)
void CDrawingSymbol::SetText(LPCTSTR strText)
{
	m_strText = strText;
}

void CDrawingSymbol::SetFillState(const bool bFill)
{
	m_bFill = bFill;
}

bool CDrawingSymbol::GetFillState()
{
	return m_bFill;
}

CString CDrawingSymbol::GetText() const
{
	return m_strText;
}

CSize CDrawingSymbol::GetSymbolSize() const
{
	return CSize(-1,-1);
}

bool CDrawingSymbol::DidDrawFromTop() const
{
	return true;
}

//sy 2005.04.06
void CDrawingSymbol::SetLogFontAndTextColor(const LOGFONT& logfont, const COLORREF& color)
{
	SetLogFont(logfont);
	SetTextColor(color);
}

void CDrawingSymbol::SetLogFont(const LOGFONT& logfont)
{
	m_logfont = logfont;
}

//sy 2005.04.06
void CDrawingSymbol::SetTextColor(const COLORREF& color)
{
	m_textColor = color;
}

//sy 2005.04.06
COLORREF CDrawingSymbol::GetTextColor() const
{
	return m_textColor;
}

LOGFONT CDrawingSymbol::GetLogFont()
{
	return m_logfont;
}

//poin가 DrawingRegion안에 있는 포인터인지 확인한다.
bool CDrawingSymbol::IsAppropriatePoint(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	if(drawingRegion.PtInRect(point))
		return true;
	return false;
}

void CDrawingSymbol::RealDrawSelectedMovingMark(CDC* pDC, 
												const COLORREF& color, 
												const CPoint &oldMovepoint, 
												const CPoint& MovePt, 
												const CPoint& gap,
												const CSize& symbolsize, 
												const CRect& drawingRegion,
												const bool bDrawFromTop,BOOL bTextMoving)
{
	if(oldMovepoint.x == 0 && oldMovepoint.y == 0)
		return;
	 //point가 시작점.
	 //마우스를 중심으로 잡기 위해서 Lbutton한 시점과 저장되어 있던 시작점의 차이를 뺀다.
	 int ropOld = pDC->SetROP2(R2_NOTXORPEN);
     /*if(oldMovepoint.x != 0 && oldMovepoint.y != 0) {
		CDrawLine::DrawRectangle(pDC, CPoint(oldMovepoint.x - gap.x, oldMovepoint.y - gap.y), 
				 CPoint(oldMovepoint.x - gap.x + symbolsize.cx, oldMovepoint.y - gap.y + symbolsize.cy));
	 }*/
	 if(bTextMoving == TRUE)
	 	CDrawLine::DrawRectangle(pDC, CPoint(MovePt.x - gap.x, MovePt.y - gap.y), 
				 CPoint(MovePt.x - gap.x + symbolsize.cx, MovePt.y - gap.y + symbolsize.cy));
	 else
		 CDrawLine::DrawRectangle(pDC, CPoint(MovePt.x - gap.x - symbolsize.cx/2, MovePt.y - gap.y - symbolsize.cy/2), 
	 		 CPoint(MovePt.x - gap.x + symbolsize.cx/2, MovePt.y - gap.y + symbolsize.cy/2));
	 
	 pDC->SetROP2( ropOld );
}

void CDrawingSymbol::DrawbyMovedSize(CDC* pDC,
									 const CPoint& oldpoint,
									 const CPoint &Endpoint,
									 const CRect& drawingRegion,
									 const CCoordinateCalculatorData& coordinateData)
{
	CCoordinateCalculator calculator;
	CPoint start_pt = calculator.RelativeToAbsolute(coordinateData, GetStartPoint());
//	CPoint end_pt = CCoordinateCalculator::RelativeToAbsolute(coordinateData, GetEndPoint());

	int ropOld = pDC->SetROP2(R2_NOTXORPEN);
	if(oldpoint.x != 0 && oldpoint.y != 0) 
		CDrawLine::DrawRectangle(pDC, start_pt, oldpoint);
	CDrawLine::DrawRectangle(pDC, start_pt, Endpoint);
	pDC->SetROP2( ropOld );
}

//////////////
//OnePoint저장.
/////////////
// protected ----------------------------------------------------------------------------
/*
void CDrawingSymbol_OnePoint::SetText(LPCTSTR text)
{
	m_strText = text;
}

CString CDrawingSymbol_OnePoint::GetText() const
{
	return m_strText;
}
*/
CCoordinate CDrawingSymbol_OnePoint::GetStartPoint() const
{
	return m_startPt;
}

CCoordinate CDrawingSymbol_OnePoint::GetEndPoint() const
{
	return CCoordinate(-1,-1);
}

void CDrawingSymbol_OnePoint::SetStartPoint(const CCoordinate& point)
{
	m_startPt = point;
}

void CDrawingSymbol_OnePoint::SetStartCoord(const CCoordinate& point)
{
	m_startPt = point;
}

void CDrawingSymbol_OnePoint::SetEndCoord(const CCoordinate& point)
{
}

CSize CDrawingSymbol_OnePoint::GetSymbolSize() const
{
	return m_symbolsize;
}

void CDrawingSymbol_OnePoint::SetSymbolSize(const CSize& symbolSize)
{
	m_symbolsize = symbolSize;
}

void CDrawingSymbol_OnePoint::TranslatePoint(const bool bSize, const CPoint &point, const CPoint &gap, const CRect &drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CCoordinateCalculator calculator;
	//LBUttonUP한 부분이 drawingRegion안에 위치한 부분일때..포인트 바꾸지 않음.
	//LBUttonUP한 부분이 객체가 위치한 부분일때..포인트 바꾸지 않음.
//>> (2008/12/16 - Seung-Lyong Park) Symbol 복사
//	if(IsSelected(drawingRegion, point, coordinateData)) 
//		return;
//<< (2008/12/16 - Seung-Lyong Park) Symbol 복사

	CPoint ptStart = point;
	if(GetSymbolType() == CSymbolBaseData::SYMBOL_TEXT)
		ptStart -= gap;
	//SetStartPoint(calculator.AbsoluteToRelative(coordinateData, point));
	SetStartPoint(calculator.AbsoluteToRelative(coordinateData, ptStart));
}

bool CDrawingSymbol_OnePoint::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x, start_pt.y, start_pt.x + GetSymbolSize().cx, start_pt.y + GetSymbolSize().cy);
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;
	else
		return false;
}

void CDrawingSymbol_OnePoint::DrawSelectedMovingMark(CDC* pDC, const CPoint &oldMovepoint, const CPoint& MovePt, const CPoint& LBttonPt, const CRect& drawingRegion, const CCoordinateCalculatorData& calcData,BOOL bTextMoving)
{
	CPoint gap(0,0);
	if(bTextMoving == TRUE)
	{
		CCoordinateCalculator calculator;
		CPoint start_pt = calculator.RelativeToAbsolute(calcData, GetStartPoint());
		
		CPoint lbuttonPt = calculator.AbsoluteToAbsolute(calcData, LBttonPt);
		gap = CPoint(lbuttonPt.x - start_pt.x, lbuttonPt.y - start_pt.y);
	}

	RealDrawSelectedMovingMark(pDC,
							   GetColor(),
							   oldMovepoint,
							   MovePt, 
							   gap,
							   GetSymbolSize(),
							   drawingRegion,
							   DidDrawFromTop(),bTextMoving);
}

CPoint CDrawingSymbol_OnePoint::GetAbsolutePoint(const CCoordinateCalculatorData &calData)
{
	CCoordinateCalculator calculator;
	return calculator.RelativeToAbsolute(calData, GetStartPoint());
}

//////////////
//TwoPoint저장.
/////////////
void CDrawingSymbol_TwoPoint::SetStartPoint(const CCoordinate &point)
{
	m_StartPt = point;
}

void CDrawingSymbol_TwoPoint::SetEndPoint(const CCoordinate &point)
{
	m_EndPt = point;
}

void CDrawingSymbol_TwoPoint::SetStartCoord(const CCoordinate& point)
{
	m_StartPt = point;
}

void CDrawingSymbol_TwoPoint::SetEndCoord(const CCoordinate& point)
{
	m_EndPt = point;
}

CCoordinate CDrawingSymbol_TwoPoint::GetStartPoint() const
{
	return m_StartPt;
}

CCoordinate CDrawingSymbol_TwoPoint::GetEndPoint() const
{
	return m_EndPt;
}

//변경된 point를 저장한다.
void CDrawingSymbol_TwoPoint::TranslatePoint(const bool bSize, const CPoint &point, const CPoint &gap, const CRect &drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CCoordinateCalculator calculator;
	CPoint start_pt, end_pt;
	GetAbsolutePoints(coordinateData, start_pt, end_pt);

	//LBUttonUP한 부분이 객체가 위치한 부분일때..포인트 바꾸지 않음.
	if(bSize == true) { //size조절
		if(!IsAppropriatePoint(drawingRegion, point, coordinateData))
			return;
//invert
		SetEndPoint(calculator.AbsoluteToRelative(coordinateData, point));
	}
	else { //객체 이동
		if(!IsAppropriatePoint(drawingRegion, start_pt + gap, coordinateData) || !IsAppropriatePoint(drawingRegion, end_pt + gap, coordinateData))
			return;

		SetStartPoint(calculator.AbsoluteToRelative(coordinateData, start_pt + gap));
		SetEndPoint(calculator.AbsoluteToRelative(coordinateData, end_pt + gap));
	}
}

//poin가 DrawingRegion안에 있는 포인터인지 확인한다.
bool CDrawingSymbol_TwoPoint::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CCoordinateCalculator calculator;

	CPoint start_pt, end_pt;
	GetAbsolutePoints(coordinateData, start_pt, end_pt);
//invert
	if(start_pt.y > end_pt.y) {
		SetStartPoint(calculator.AbsoluteToRelative(coordinateData, CPoint(start_pt.x, end_pt.y)));
		SetEndPoint(calculator.AbsoluteToRelative(coordinateData, CPoint(end_pt.x, start_pt.y)));
	}
	CRect rcsymbol(start_pt.x, start_pt.y, end_pt.x, end_pt.y);
	if(rcsymbol.PtInRect(point))
		return true;
	else
		return false;
}

void CDrawingSymbol_TwoPoint::DrawSelectedMovingMark(CDC* pDC, const CPoint &oldMovepoint, const CPoint& MovePt, const CPoint& LBttonPt, const CRect& drawingRegion, const CCoordinateCalculatorData& calcData,BOOL bTextMoving)
{
	CCoordinateCalculator calculator;

	CPoint start_pt, end_pt;
	GetAbsolutePoints(calcData, start_pt, end_pt);

	CPoint oldmovePt;
	if(oldMovepoint.x == 0 && oldMovepoint.y == 0)
		oldmovePt = CPoint(0,0);
	else
		oldmovePt = calculator.AbsoluteToAbsolute(calcData, oldMovepoint);
	CPoint movePt = calculator.AbsoluteToAbsolute(calcData, MovePt);
	CPoint lbuttonPt = calculator.AbsoluteToAbsolute(calcData, LBttonPt);

	CRect rcMark = CRect(start_pt.x,start_pt.y,end_pt.x,end_pt.y);
	CPoint centPt = rcMark.CenterPoint();
	CPoint gap(lbuttonPt.x - start_pt.x, lbuttonPt.y - start_pt.y);
	if(bTextMoving == TRUE)
	{
		gap = CPoint(lbuttonPt.x - centPt.x, lbuttonPt.y - centPt.y);
	}
	
	//마우스 부분을 중심으로 잡기 위해서...
	RealDrawSelectedMovingMark(pDC, GetColor(), oldmovePt , movePt, gap, CSize(end_pt.x - start_pt.x, end_pt.y - start_pt.y), drawingRegion, DidDrawFromTop(),FALSE);
}

void CDrawingSymbol_TwoPoint::GetAbsolutePoints(const CCoordinateCalculatorData &calData, CPoint &startPoint, CPoint &endPoint) const
{
	CCoordinateCalculator calculator;
	startPoint = calculator.RelativeToAbsolute(calData, GetStartPoint());
	endPoint = calculator.RelativeToAbsolute(calData, GetEndPoint());
}

//////////////////////////
//저장할 객체.
//////////////////////////
///////////////////////////////
////Text
///////////////////////////////

CText::CText(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFontAndTextColor(logfont, textColor);
}

CText::CText(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor,const CString& strText)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFontAndTextColor(logfont, textColor);
	SetText(strText);
}

void CText::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
//2008.10.24 by LYH >> 텍스트를 상위로 움직이면 텍스트 내용이 사라짐 수정
//	if(!IsPointInBlock(drawingRegion, start_pt)) 
//		return;
//2008.10.24 by LYH <<

	SetSymbolSize(m_textDrawer.DrawText(pDC, GetColor(), GetLogFont(), GetSizeType(), start_pt, drawingRegion, GetText()));
}

void CText::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_textDrawer.DrawSelectedMark(pDC, GetColor(), start_pt,GetSymbolSize(), drawingRegion,TRUE);
}

/////////////////////////////////////////////
////도형 - 네모
///////////////////////////////

///
//도형 - 네모
CFigure_Square::CFigure_Square(const CSymbolBaseData::TOOLTYPE SymbolType,
							   const CCoordinate& startPt,
							   const CCoordinate& endPt, 
							   const COLORREF& color,
							   const int& nLineStyle,
							   const int& nLineWeight,
							   const bool& bFill,
							   const LOGFONT& logfont,
							   const bool bDrawFromTop)
{
	SetSymbolType(SymbolType);
	SetStartPoint(startPt);
	SetEndPoint(endPt);
	SetColor(color);
	SetStyle(nLineStyle);
	SetWeight(nLineWeight);
	SetLogFont(logfont);
	SetFillState(bFill);
	SetDrawFromTop(bDrawFromTop);
}

void CFigure_Square::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt, end_pt;
	GetAbsolutePoints(coordinateData, start_pt, end_pt);

//	if(!IsPointInBlock(drawingRegion, start_pt)) 
//		return;
//invert
	m_squareDrawer.Draw(pDC, GetColor(), GetStyle() , GetWeight(),GetLogFont(), GetFillState(), start_pt, CPoint(end_pt.x, end_pt.y), drawingRegion, DidDrawFromTop());
}

void CFigure_Square::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt, end_pt;
	GetAbsolutePoints(coordinateData, start_pt, end_pt);

	m_squareDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, end_pt, drawingRegion);
}

bool CFigure_Square::IsAllValueBiggerThanZero(const CCoordinate& start, const CCoordinate& end) const
{
	return (start.GetX() >= 0 && end.GetX() >= 0);
}


/////////////////////////////////////////////
////도형 - 원형
///////////////////////////////

CFigure_Circle::CFigure_Circle(const CSymbolBaseData::TOOLTYPE SymbolType,
							   const CCoordinate& startPt,
							   const CCoordinate& endPt,
							   const COLORREF& color,
							   const int& nLineStyle,
							   const int& nLineWeight,
							   const bool& bFill,
							   const LOGFONT& logfont,
							   const bool bDrawFromTop)
{
	SetSymbolType(SymbolType);
	SetStartPoint(startPt);
	SetEndPoint(endPt);
	SetColor(color);
	SetStyle(nLineStyle);
	SetWeight(nLineWeight);
	SetLogFont(logfont);
	SetFillState(bFill);
	SetDrawFromTop(bDrawFromTop);
}

void CFigure_Circle::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{	
	CPoint start_pt, end_pt;
	GetAbsolutePoints(coordinateData, start_pt, end_pt);
	//객체 이동할때 마우스의 위치와 시작 포인터가 달라서..그림이 안그려짐..이동은 되는데..
//	if(!IsPointInBlock(drawingRegion, start_pt)) 
//		return;

	m_circleDrawer.Draw(pDC, GetColor(),GetStyle() , GetWeight(), GetLogFont(), GetFillState(), start_pt, end_pt, drawingRegion, DidDrawFromTop());
}


void CFigure_Circle::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt, end_pt;
	GetAbsolutePoints(coordinateData, start_pt, end_pt);
	m_circleDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, end_pt, drawingRegion);
}

void CFigure_Circle::DrawbyMovedSize(CDC* pDC, 
									 const CPoint& oldpoint,
									 const CPoint &Endpoint,
									 const CRect& drawingRegion, 
									 const CCoordinateCalculatorData& coordinateData)
{
	CCoordinateCalculator calculator;
	CPoint start_pt = calculator.RelativeToAbsolute(coordinateData, GetStartPoint());
	m_circleDrawer.DrawbyMovedSize(pDC, start_pt, oldpoint, Endpoint, drawingRegion);
}

void CFigure_Circle::RealDrawSelectedMovingMark(CDC* pDC,
												const COLORREF& color,
												const CPoint &oldMovepoint,
												const CPoint& MovePt, 
												const CPoint& gap,
												const CSize& symbolsize, 
												const CRect& drawingRegion,
												const bool bDrawFromTop,BOOL bTextMoving)
{
	m_circleDrawer.RealDrawSelectedMovingMark(pDC, color, oldMovepoint , MovePt, gap, symbolsize, drawingRegion, bDrawFromTop);
}

/////////////////////////////////////////////
////도형 - 삼각형
///////////////////////////////

CFigure_Triangle::CFigure_Triangle(const CSymbolBaseData::TOOLTYPE SymbolType,
								   const CCoordinate& startPt,
								   const CCoordinate& endPt,
								   const COLORREF& color,
								   const int& nLineStyle,
								   const int& nLineWeight,
								   const bool& bFill,
								   const LOGFONT& logfont,
								   const bool bDrawFromTop)
{
	SetSymbolType(SymbolType);
	SetStartPoint(startPt);
	SetEndPoint(endPt);
	SetColor(color);
	SetStyle(nLineStyle);
	SetWeight(nLineWeight);
	SetLogFont(logfont);
	SetFillState(bFill);
	SetDrawFromTop(bDrawFromTop);
}

void CFigure_Triangle::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt, end_pt;
	GetAbsolutePoints(coordinateData, start_pt, end_pt);
	//객체 이동할때 마우스의 위치와 시작 포인터가 달라서..그림이 안그려짐..이동은 되는데..
//	if(!IsPointInBlock(drawingRegion, start_pt)) 
//		return;

	m_triangleDrawer.Draw(pDC, GetColor(),GetStyle() , GetWeight(), GetLogFont(), GetFillState(), start_pt, end_pt, drawingRegion, DidDrawFromTop());
}


void CFigure_Triangle::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt, end_pt;
	GetAbsolutePoints(coordinateData, start_pt, end_pt);
	m_triangleDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, end_pt, drawingRegion);
}

void CFigure_Triangle::DrawbyMovedSize(CDC* pDC,
									   const CPoint& oldpoint, 
									   const CPoint &Endpoint, 
									   const CRect& drawingRegion, 
									   const CCoordinateCalculatorData& coordinateData)
{
	CCoordinateCalculator calculator;
	CPoint start_pt = calculator.RelativeToAbsolute(coordinateData, GetStartPoint());
	m_triangleDrawer.DrawbyMovedSize(pDC, 
									 start_pt, 
									 oldpoint, 
									 Endpoint,
									 drawingRegion,
									 DidDrawFromTop());
}

void CFigure_Triangle::RealDrawSelectedMovingMark(CDC* pDC,
												  const COLORREF& color, 
												  const CPoint &oldMovepoint,
												  const CPoint& MovePt, 
												  const CPoint& gap, 
												  const CSize& symbolsize,
												  const CRect& drawingRegion,
												  const bool bDrawFromTop,BOOL bTextMoving)
{
	m_triangleDrawer.RealDrawSelectedMovingMark(pDC, color, oldMovepoint , MovePt, gap, symbolsize, drawingRegion, bDrawFromTop);
}

/////////////////////////////////////////////
////Left
///////////////////////////////

///
//화살표 - 왼쪽
CArrow_Left::CArrow_Left(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFont(logfont);
}

void CArrow_Left::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_arrowleftDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CArrow_Left::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_arrowleftDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CArrow_Left::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x, start_pt.y - (GetSymbolSize().cy/2), start_pt.x + GetSymbolSize().cx, start_pt.y + (GetSymbolSize().cy/2));
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

void CArrow_Left::DrawSelectedMovingMark(CDC* pDC, const CPoint &oldMovepoint, const CPoint& MovePt, const CPoint& LBttonPt, const CRect& drawingRegion, const CCoordinateCalculatorData& calcData,BOOL bTextMoving)
{
	m_arrowleftDrawer.DrawSelectedMovingMark(pDC, GetColor(), oldMovepoint, MovePt, GetSymbolSize(), drawingRegion);
}

/////////////////////////////////////////////
////Right
///////////////////////////////
//화살표 - 오른쪽
CArrow_Right::CArrow_Right(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFont(logfont);
}

void CArrow_Right::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_arrowrightDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CArrow_Right::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_arrowrightDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CArrow_Right::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x - GetSymbolSize().cx, start_pt.y - (GetSymbolSize().cy/2), start_pt.x, start_pt.y + (GetSymbolSize().cy/2));
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

void CArrow_Right::DrawSelectedMovingMark(CDC* pDC, const CPoint &oldMovepoint, const CPoint& MovePt, const CPoint& LBttonPt, const CRect& drawingRegion, const CCoordinateCalculatorData& calcData,BOOL bTextMoving)
{
	m_arrowrightDrawer.DrawSelectedMovingMark(pDC, GetColor(), oldMovepoint, MovePt, GetSymbolSize(), drawingRegion);
}

/////////////////////////////////////////////
////Arrow_Up
///////////////////////////////
CArrow_Up::CArrow_Up(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFont(logfont);
}

void CArrow_Up::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_arrowUpDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CArrow_Up::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_arrowUpDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CArrow_Up::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	//CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y, start_pt.x + (GetSymbolSize().cx/2), start_pt.y + GetSymbolSize().cy);
	CRect rcsymbol(start_pt.x, start_pt.y, start_pt.x + GetSymbolSize().cx, start_pt.y + GetSymbolSize().cy);
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

void CArrow_Up::DrawSelectedMovingMark(CDC* pDC, const CPoint &oldMovepoint, const CPoint& MovePt, const CPoint& LBttonPt, const CRect& drawingRegion, const CCoordinateCalculatorData& calcData,BOOL bTextMoving)
{
	m_arrowUpDrawer.DrawSelectedMovingMark(pDC, GetColor(), oldMovepoint, MovePt, GetSymbolSize(), drawingRegion);
}

/////////////////////////////////////////////
////Arrow_Down
///////////////////////////////
CArrow_Down::CArrow_Down(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFont(logfont);
}

void CArrow_Down::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_arrowdownDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CArrow_Down::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_arrowdownDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CArrow_Down::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	//CRect rcsymbol(start_pt.x, start_pt.y, start_pt.x + GetSymbolSize().cx, start_pt.y + GetSymbolSize().cy);
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

void CArrow_Down::DrawSelectedMovingMark(CDC* pDC, const CPoint &oldMovepoint, const CPoint& MovePt, const CPoint& LBttonPt, const CRect& drawingRegion, const CCoordinateCalculatorData& calcData,BOOL bTextMoving)
{
	m_arrowdownDrawer.DrawSelectedMovingMark(pDC, GetColor(), oldMovepoint, MovePt, GetSymbolSize(), drawingRegion);
}

/////////////////////////////////////////////
////Arrow_LeftTop
///////////////////////////////
CArrow_LeftTop::CArrow_LeftTop(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFont(logfont);
}

void CArrow_LeftTop::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_arrowLeftTopDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CArrow_LeftTop::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_arrowLeftTopDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CArrow_LeftTop::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	//CRect rcsymbol(start_pt.x, start_pt.y, start_pt.x + GetSymbolSize().cx, start_pt.y + GetSymbolSize().cy);
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

void CArrow_LeftTop::DrawSelectedMovingMark(CDC* pDC, const CPoint &oldMovepoint, const CPoint& MovePt, const CPoint& LBttonPt, const CRect& drawingRegion, const CCoordinateCalculatorData& calcData,BOOL bTextMoving)
{
	m_arrowLeftTopDrawer.DrawSelectedMovingMark(pDC, GetColor(), oldMovepoint, MovePt, GetSymbolSize(), drawingRegion);
}

/////////////////////////////////////////////
////Arrow_LeftBottom
///////////////////////////////
CArrow_LeftBottom::CArrow_LeftBottom(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFont(logfont);
}

void CArrow_LeftBottom::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_arrowLeftBottomDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CArrow_LeftBottom::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_arrowLeftBottomDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CArrow_LeftBottom::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x, start_pt.y - GetSymbolSize().cy, start_pt.x + GetSymbolSize().cx, start_pt.y);
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

void CArrow_LeftBottom::DrawSelectedMovingMark(CDC* pDC, const CPoint &oldMovepoint, const CPoint& MovePt, const CPoint& LBttonPt, const CRect& drawingRegion, const CCoordinateCalculatorData& calcData,BOOL bTextMoving)
{
	m_arrowLeftBottomDrawer.DrawSelectedMovingMark(pDC, GetColor(), oldMovepoint, MovePt, GetSymbolSize(), drawingRegion);
}

/////////////////////////////////////////////
////Arrow_RightTop
///////////////////////////////
CArrow_RightTop::CArrow_RightTop(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFont(logfont);
}

void CArrow_RightTop::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_arrowRightTopDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CArrow_RightTop::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_arrowRightTopDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CArrow_RightTop::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x - GetSymbolSize().cx, start_pt.y, start_pt.x, start_pt.y + GetSymbolSize().cy);
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

void CArrow_RightTop::DrawSelectedMovingMark(CDC* pDC, const CPoint &oldMovepoint, const CPoint& MovePt, const CPoint& LBttonPt, const CRect& drawingRegion, const CCoordinateCalculatorData& calcData,BOOL bTextMoving)
{
	m_arrowRightTopDrawer.DrawSelectedMovingMark(pDC, GetColor(), oldMovepoint, MovePt, GetSymbolSize(), drawingRegion);
}

/////////////////////////////////////////////
////Arrow_RightBottom
///////////////////////////////
CArrow_RightBottom::CArrow_RightBottom(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFont(logfont);
}

void CArrow_RightBottom::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_arrowRightBottomDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CArrow_RightBottom::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_arrowRightBottomDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CArrow_RightBottom::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x - GetSymbolSize().cx, start_pt.y - GetSymbolSize().cy, start_pt.x, start_pt.y);
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

void CArrow_RightBottom::DrawSelectedMovingMark(CDC* pDC, const CPoint &oldMovepoint, const CPoint& MovePt, const CPoint& LBttonPt, const CRect& drawingRegion, const CCoordinateCalculatorData& calcData,BOOL bTextMoving)
{
	m_arrowRightBottomDrawer.DrawSelectedMovingMark(pDC, GetColor(), oldMovepoint, MovePt, GetSymbolSize(), drawingRegion);
}

/////////////////////////////////////////////
////Num_ONe
///////////////////////////////
CNumber_One::CNumber_One(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFontAndTextColor(logfont, textColor);
}

void CNumber_One::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_numberOneDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CNumber_One::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_numberOneDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CNumber_One::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	//CRect rcsymbol(start_pt.x, start_pt.y, start_pt.x + GetSymbolSize().cx, start_pt.y + GetSymbolSize().cy);
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

/////////////////////////////////////////////
////Num_Two
///////////////////////////////
CNumber_Two::CNumber_Two(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFontAndTextColor(logfont, textColor);
}

void CNumber_Two::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_numberTwoDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CNumber_Two::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_numberTwoDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CNumber_Two::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	//CRect rcsymbol(start_pt.x, start_pt.y, start_pt.x + GetSymbolSize().cx, start_pt.y + GetSymbolSize().cy);
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

/////////////////////////////////////////////
////Num_Three
///////////////////////////////
CNumber_Three::CNumber_Three(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFontAndTextColor(logfont, textColor);
}

void CNumber_Three::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_numberThreeDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CNumber_Three::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_numberThreeDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CNumber_Three::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

/////////////////////////////////////////////
////Num_Four
///////////////////////////////
CNumber_Four::CNumber_Four(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFontAndTextColor(logfont, textColor);
}

void CNumber_Four::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_numberFourDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CNumber_Four::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_numberFourDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CNumber_Four::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

/////////////////////////////////////////////
////Num_Five
///////////////////////////////
CNumber_Five::CNumber_Five(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFontAndTextColor(logfont, textColor);
}

void CNumber_Five::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_numberFiveDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CNumber_Five::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_numberFiveDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CNumber_Five::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

/////////////////////////////////////////////
////Num_Six
///////////////////////////////
CNumber_Six::CNumber_Six(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFontAndTextColor(logfont, textColor);
}

void CNumber_Six::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_numberSixDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CNumber_Six::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_numberSixDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CNumber_Six::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

/////////////////////////////////////////////
////Num_Seven
///////////////////////////////
CNumber_Seven::CNumber_Seven(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFontAndTextColor(logfont, textColor);
}

void CNumber_Seven::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_numberSevenDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CNumber_Seven::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_numberSevenDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CNumber_Seven::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

/////////////////////////////////////////////
////Num_Eight
///////////////////////////////
CNumber_Eight::CNumber_Eight(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFontAndTextColor(logfont, textColor);
}

void CNumber_Eight::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_numberEightDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CNumber_Eight::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_numberEightDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CNumber_Eight::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

/////////////////////////////////////////////
////Num_Nine
///////////////////////////////
CNumber_Nine::CNumber_Nine(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFontAndTextColor(logfont, textColor);
}

void CNumber_Nine::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_numberNineDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CNumber_Nine::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_numberNineDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CNumber_Nine::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

/////////////////////////////////////////////
////Text_A
///////////////////////////////
CText_A::CText_A(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFontAndTextColor(logfont, textColor);
}

void CText_A::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_textADrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CText_A::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_textADrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CText_A::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

/////////////////////////////////////////////
////Text_B
///////////////////////////////
CText_B::CText_B(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFontAndTextColor(logfont, textColor);
}

void CText_B::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_textBDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CText_B::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_textBDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CText_B::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

/////////////////////////////////////////////
////Text_C
///////////////////////////////
CText_C::CText_C(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFontAndTextColor(logfont, textColor);
}

void CText_C::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_textCDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CText_C::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_textCDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CText_C::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

/////////////////////////////////////////////
////Text_D
///////////////////////////////
CText_D::CText_D(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFontAndTextColor(logfont, textColor);
}

void CText_D::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_textDDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CText_D::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_textDDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CText_D::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

/////////////////////////////////////////////
////Text_E
///////////////////////////////
CText_E::CText_E(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFontAndTextColor(logfont, textColor);
}

void CText_E::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_textEDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CText_E::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_textEDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CText_E::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

/////////////////////////////////////////////
////Text_F
///////////////////////////////
CText_F::CText_F(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFontAndTextColor(logfont, textColor);
}

void CText_F::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_textFDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CText_F::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_textFDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CText_F::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

/////////////////////////////////////////////
////Text_G
///////////////////////////////
CText_G::CText_G(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFontAndTextColor(logfont, textColor);
}

void CText_G::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_textGDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CText_G::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_textGDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CText_G::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

/////////////////////////////////////////////
////Text_H
///////////////////////////////
CText_H::CText_H(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFontAndTextColor(logfont, textColor);
}

void CText_H::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_textHDrawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CText_H::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_textHDrawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CText_H::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

/////////////////////////////////////////////
////Char_1
///////////////////////////////
CChar_1::CChar_1(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFont(logfont);
}

void CChar_1::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_char1Drawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CChar_1::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_char1Drawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CChar_1::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	//CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y, start_pt.x + (GetSymbolSize().cx/2), start_pt.y + GetSymbolSize().cy);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

/////////////////////////////////////////////
////Char_2
///////////////////////////////
CChar_2::CChar_2(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFont(logfont);
}

void CChar_2::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_char2Drawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CChar_2::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_char2Drawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CChar_2::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	//CRect rcsymbol(start_pt.x, start_pt.y, start_pt.x + GetSymbolSize().cx, start_pt.y + GetSymbolSize().cy);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

/////////////////////////////////////////////
////Char_3
///////////////////////////////
CChar_3::CChar_3(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFont(logfont);
}

void CChar_3::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_char3Drawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CChar_3::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_char3Drawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CChar_3::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	//CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y, start_pt.x + (GetSymbolSize().cx/2), start_pt.y + GetSymbolSize().cy);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

/////////////////////////////////////////////
////Char_4
///////////////////////////////
CChar_4::CChar_4(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFont(logfont);
}

void CChar_4::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_char4Drawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CChar_4::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_char4Drawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CChar_4::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	//CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y, start_pt.x + (GetSymbolSize().cx/2), start_pt.y + GetSymbolSize().cy);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

/////////////////////////////////////////////
////Char_5
///////////////////////////////
CChar_5::CChar_5(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFont(logfont);
}

void CChar_5::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_char5Drawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CChar_5::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_char5Drawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CChar_5::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	//CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y, start_pt.x + (GetSymbolSize().cx/2), start_pt.y + GetSymbolSize().cy);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

/////////////////////////////////////////////
////Char_6
///////////////////////////////
CChar_6::CChar_6(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont)
{
	SetSymbolType(SymbolType);
	SetSizeType(sizetype);
	SetSymbolSize(symbol_size);
	SetStartPoint(startPt);
	SetColor(color);
	SetLogFont(logfont);
}

void CChar_6::Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	if(!IsPointInBlock(drawingRegion, start_pt)) 
		return;

	SetSymbolSize(m_char6Drawer.Draw(pDC, GetColor(), GetLogFont(), GetTextColor(), GetSizeType(), start_pt, drawingRegion));
}

void CChar_6::DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	m_char6Drawer.DrawSelectedMark(pDC, GetColor(), start_pt, GetSymbolSize(), drawingRegion);
}

bool CChar_6::IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData)
{
	CPoint start_pt = GetAbsolutePoint(coordinateData);
	//CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y, start_pt.x + (GetSymbolSize().cx/2), start_pt.y + GetSymbolSize().cy);
	CRect rcsymbol(start_pt.x - (GetSymbolSize().cx/2), start_pt.y - (GetSymbolSize().cy/2), start_pt.x + (GetSymbolSize().cx/2), start_pt.y+(GetSymbolSize().cy/2));
	if(drawingRegion.PtInRect(point) && rcsymbol.PtInRect(point))
		return true;

	return false;
}

bool CDrawingSymbol_TwoPoint::DidDrawFromTop() const
{
	return m_bDrawFromTop;
}

void CDrawingSymbol_TwoPoint::SetDrawFromTop(const bool bDrawFromTop)
{
	m_bDrawFromTop = bDrawFromTop;
}
