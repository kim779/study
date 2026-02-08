// DrawingSymbol.h: interface for the CDrawingSymbol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWINGSYMBOL_H__63F59AE2_09CA_42DA_8E1F_E44F1DFFE217__INCLUDED_)
#define AFX_DRAWINGSYMBOL_H__63F59AE2_09CA_42DA_8E1F_E44F1DFFE217__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SymbolBaseData.h"

#include "SymbolDraw.h"
#include "Coordinate.h"

class CCoordinateCalculatorData;
class CDrawingSymbol
{
public:
	CDrawingSymbol();

	LOGFONT GetLogFont();
	COLORREF GetTextColor() const;
	void SetLogFontAndTextColor(const LOGFONT& logfont, const COLORREF& color);
	void SetLogFont(const LOGFONT& logfont);
	void SetTextColor(const COLORREF& color);
	void SetSizeType(const CSymbolBaseData::SIZETYPE& sizetype);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData) = 0;
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData) = 0;
	virtual void DrawSelectedMovingMark(CDC* pDC, const CPoint &oldMovepoint, const CPoint& MovePt, const CPoint& LBttonPt, const CRect& drawingRegion, const CCoordinateCalculatorData& calcData,BOOL bTextMoving)= 0;

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData) = 0;
	virtual bool IsAppropriatePoint(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);
	virtual void TranslatePoint(const bool bSize, const CPoint& point, const CPoint& gap, const CRect &drawingRegion, const CCoordinateCalculatorData& coordinateData) = 0;

	virtual void DrawbyMovedSize(CDC* pDC, 
								 const CPoint& oldpoint, 
								 const CPoint &Endpoint,
								 const CRect& drawingRegion, 
								 const CCoordinateCalculatorData& coordinateData);

	virtual CString GetText() const;
	virtual CCoordinate GetStartPoint() const = 0;
	virtual CCoordinate GetEndPoint() const = 0;
	virtual void SetStartCoord(const CCoordinate& point) = 0;
	virtual void SetEndCoord(const CCoordinate& point) = 0;
	virtual CSize GetSymbolSize() const;
	virtual bool DidDrawFromTop() const;
	virtual void SetText(LPCTSTR text);

	int GetWeight() const;
	int GetStyle() const;
	COLORREF GetColor() const;
	CSymbolBaseData::SIZETYPE GetSizeType() const;

	//붙여넣기할때 복사되는 대상의 Type을 얻는다.
	CSymbolBaseData::TOOLTYPE GetSymbolType();

	void SetWeight(const int& weight);
	void SetStyle(const int& style);
	void SetColor(const COLORREF& color);
	void SetSymbolType(const CSymbolBaseData::TOOLTYPE SymbolType);

	void SetFillState(const bool bFill);	
	bool GetFillState();
		
protected:
	virtual void RealDrawSelectedMovingMark(CDC* pDC,
											const COLORREF& color, 
											const CPoint &oldMovepoint,
											const CPoint& MovePt, 
											const CPoint& gap,
											const CSize& symbolsize,
											const CRect& drawingRegion,
											const bool bDrawFromTop,
											BOOL bTextMoving);
	//point가 그래프영역에 포함되는지 검사한다.
	bool IsPointInBlock(const CRect &drawingRegion, const CPoint &point);

private:
	bool m_bFill;

	CCoordinate m_StartPt;
	CCoordinate m_EndPt;
	
	CString m_strText;
	LOGFONT m_logfont;
	COLORREF m_textColor;//sy 2005.04.06
	COLORREF m_Color;
	int m_Style;
	int m_Weight;
	CSymbolBaseData::SIZETYPE m_SizeType;
	CSymbolBaseData::TOOLTYPE m_SymbolType;
};

////////////
//OnePoint
///////////
class CDrawingSymbol_OnePoint : public CDrawingSymbol  
{
public:
	virtual void TranslatePoint(const bool bSize, const CPoint& point, const CPoint& gap, const CRect &drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMovingMark(CDC* pDC, const CPoint &oldMovepoint, const CPoint& MovePt, const CPoint& LBttonPt, const CRect& drawingRegion, const CCoordinateCalculatorData& calcData,BOOL bTextMoving = FALSE);

	virtual void SetStartCoord(const CCoordinate& point);
	virtual void SetEndCoord(const CCoordinate& point);

	virtual CCoordinate GetStartPoint() const;
	virtual CCoordinate GetEndPoint() const;

protected:
	CPoint GetAbsolutePoint(const CCoordinateCalculatorData& calData);	
	CSize GetSymbolSize() const;

//	CString GetText() const;
//	virtual void SetText(LPCTSTR text);
	void SetStartPoint(const CCoordinate& point);
	void SetSymbolSize(const CSize& symbolSize);

private:
	CCoordinate m_startPt;
	CSize m_symbolsize;

//	CString m_strText;
};

///////////////
//Two Point
//////////////
class CDrawingSymbol_TwoPoint : public CDrawingSymbol 
{
public:
	void SetDrawFromTop(const bool bDrawFromTop);
	bool DidDrawFromTop() const;
	virtual void TranslatePoint(const bool bSize, const CPoint& point, const CPoint& gap, const CRect &drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMovingMark(CDC* pDC, const CPoint &oldMovepoint, const CPoint& MovePt, const CPoint& LBttonPt, const CRect& drawingRegion, const CCoordinateCalculatorData& calcData,BOOL bTextMoving);
	
	virtual void SetStartCoord(const CCoordinate& point);
	virtual void SetEndCoord(const CCoordinate& point);

	//start, end position을 얻는다.
	virtual CCoordinate GetStartPoint() const;
	virtual CCoordinate GetEndPoint() const;
protected:
	void GetAbsolutePoints(const CCoordinateCalculatorData& calData, CPoint& startPoint, CPoint& endPoint) const;
	
	//start, end position을 저장한다.
	void SetStartPoint(const CCoordinate& point);
	void SetEndPoint(const CCoordinate &point);

private:
	bool m_bDrawFromTop;
	CCoordinate m_StartPt;
	CCoordinate m_EndPt;
};

///////////
//Square object
///////////
class CFigure_Square : public CDrawingSymbol_TwoPoint
{
public:
	CFigure_Square(const CSymbolBaseData::TOOLTYPE SymbolType, 
				   const CCoordinate& startPt,
				   const CCoordinate& endPt, 
				   const COLORREF& color,
				   const int& nLineStyle,
				   const int& nLineWeight,
				   const bool& bFill,
				   const LOGFONT& logfont,
				   const bool bDrawFromTop);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

private:
	bool IsAllValueBiggerThanZero(const CCoordinate& start, const CCoordinate& end) const;

	CFigure_Square_Draw m_squareDrawer;
};

///////////
//Circle object
///////////
class CFigure_Circle : public CDrawingSymbol_TwoPoint 
{
public:
	CFigure_Circle(const CSymbolBaseData::TOOLTYPE SymbolType,
				   const CCoordinate& startPt, 
				   const CCoordinate& endPt,
				   const COLORREF& color,
				   const int& nLineStyle,
				   const int& nLineWeight,
				   const bool& bFill,
				   const LOGFONT& logfont,
				   const bool bDrawFromTop);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawbyMovedSize(CDC* pDC,
								 const CPoint& oldpoint,
								 const CPoint &Endpoint,
								 const CRect& drawingRegion,
								 const CCoordinateCalculatorData& coordinateData);
	virtual void RealDrawSelectedMovingMark(CDC* pDC, 
											const COLORREF& color,
											const CPoint &oldMovepoint,
											const CPoint& MovePt, 
											const CPoint& gap, 
											const CSize& symbolsize,
											const CRect& drawingRegion,
											const bool bDrawFromTop,BOOL bTextMoving);

private:
	CFigure_Circle_Draw m_circleDrawer;
};

///////////
//Circle object
///////////
class CFigure_Triangle : public CDrawingSymbol_TwoPoint 
{
public:
	CFigure_Triangle(const CSymbolBaseData::TOOLTYPE SymbolType,
					 const CCoordinate& startPt,
					 const CCoordinate& endPt,
					 const COLORREF& color,
					 const int& nLineStyle,
					 const int& nLineWeight,
					 const bool& bFill,
					 const LOGFONT& logfont,
					 const bool bDrawFromTop);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawbyMovedSize(CDC* pDC,
								 const CPoint& oldpoint,
								 const CPoint &Endpoint,
								 const CRect& drawingRegion,
								 const CCoordinateCalculatorData& coordinateData);

	virtual void RealDrawSelectedMovingMark(CDC* pDC, 
											const COLORREF& color,
											const CPoint &oldMovepoint, 
											const CPoint& MovePt,
											const CPoint& gap, 
											const CSize& symbolsize, 
											const CRect& drawingRegion,
											const bool bDrawFromTop,BOOL bTextMoving);

private:
	CFigure_Triangle_Draw m_triangleDrawer;
};

///////////
//Text object
///////////
class CText : public CDrawingSymbol_OnePoint
{
public:
	CText(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor);
	CText(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor,const CString& strText);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

private:
	CTextDraw m_textDrawer;
};

///////////
//왼쪽 화살표 object
///////////
class CArrow_Left : public CDrawingSymbol_OnePoint
{
public:
	CArrow_Left(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMovingMark(CDC* pDC, const CPoint &oldMovepoint, const CPoint& MovePt, const CPoint& LBttonPt, const CRect& drawingRegion, const CCoordinateCalculatorData& calcData,BOOL bTextMoving);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CArrow_Left_Draw m_arrowleftDrawer;
};

///////////
//오른쪽 화살표 object
///////////
class CArrow_Right : public CDrawingSymbol_OnePoint
{
public:
	CArrow_Right(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMovingMark(CDC* pDC, const CPoint &oldMovepoint, const CPoint& MovePt, const CPoint& LBttonPt, const CRect& drawingRegion, const CCoordinateCalculatorData& calcData,BOOL bTextMoving);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CArrow_Right_Draw m_arrowrightDrawer;
};


///////////
//Arrow_Up
///////////
class CArrow_Up : public CDrawingSymbol_OnePoint
{
public:
	CArrow_Up(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMovingMark(CDC* pDC, const CPoint &oldMovepoint, const CPoint& MovePt, const CPoint& LBttonPt, const CRect& drawingRegion, const CCoordinateCalculatorData& calcData,BOOL bTextMoving);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CArrow_Up_Draw m_arrowUpDrawer;
};

///////////
//Arrow_Down
///////////
class CArrow_Down : public CDrawingSymbol_OnePoint
{
public:
	CArrow_Down(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMovingMark(CDC* pDC, const CPoint &oldMovepoint, const CPoint& MovePt, const CPoint& LBttonPt, const CRect& drawingRegion, const CCoordinateCalculatorData& calcData,BOOL bTextMoving);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CArrow_Down_Draw m_arrowdownDrawer;
};

///////////
//Arrow_LeftTop
///////////
class CArrow_LeftTop : public CDrawingSymbol_OnePoint
{
public:
	CArrow_LeftTop(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMovingMark(CDC* pDC, const CPoint &oldMovepoint, const CPoint& MovePt, const CPoint& LBttonPt, const CRect& drawingRegion, const CCoordinateCalculatorData& calcData,BOOL bTextMoving);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CArrow_LeftTop_Draw m_arrowLeftTopDrawer;
};

///////////
//Arrow_LeftBottom
///////////
class CArrow_LeftBottom : public CDrawingSymbol_OnePoint
{
public:
	CArrow_LeftBottom(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMovingMark(CDC* pDC, const CPoint &oldMovepoint, const CPoint& MovePt, const CPoint& LBttonPt, const CRect& drawingRegion, const CCoordinateCalculatorData& calcData,BOOL bTextMoving);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CArrow_LeftBottom_Draw m_arrowLeftBottomDrawer;
};

///////////
//Arrow_RightTop
///////////
class CArrow_RightTop : public CDrawingSymbol_OnePoint
{
public:
	CArrow_RightTop(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMovingMark(CDC* pDC, const CPoint &oldMovepoint, const CPoint& MovePt, const CPoint& LBttonPt, const CRect& drawingRegion, const CCoordinateCalculatorData& calcData,BOOL bTextMoving);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CArrow_RightTop_Draw m_arrowRightTopDrawer;
};

///////////
//Arrow_RightBottom
///////////
class CArrow_RightBottom : public CDrawingSymbol_OnePoint
{
public:
	CArrow_RightBottom(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMovingMark(CDC* pDC, const CPoint &oldMovepoint, const CPoint& MovePt, const CPoint& LBttonPt, const CRect& drawingRegion, const CCoordinateCalculatorData& calcData,BOOL bTextMoving);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CArrow_RightBottom_Draw m_arrowRightBottomDrawer;
};

///////////
//Num_ONe
///////////
class CNumber_One : public CDrawingSymbol_OnePoint
{
public:
	CNumber_One(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, 
		const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, 
		const LOGFONT& logfont, const COLORREF& textColor);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CNumber_One_Draw m_numberOneDrawer;
};

///////////
//Num_Two
///////////
class CNumber_Two : public CDrawingSymbol_OnePoint
{
public:
	CNumber_Two(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, 
		const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, 
		const LOGFONT& logfont, const COLORREF& textColor);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CNumber_Two_Draw m_numberTwoDrawer;
};

///////////
//Num_Three
///////////
class CNumber_Three : public CDrawingSymbol_OnePoint
{
public:
	CNumber_Three(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, 
		const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, 
		const LOGFONT& logfont, const COLORREF& textColor);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CNumber_Three_Draw m_numberThreeDrawer;
};

///////////
//Num_Four
///////////
class CNumber_Four : public CDrawingSymbol_OnePoint
{
public:
	CNumber_Four(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, 
		const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, 
		const LOGFONT& logfont, const COLORREF& textColor);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CNumber_Four_Draw m_numberFourDrawer;
};

///////////
//Num_Five
///////////
class CNumber_Five : public CDrawingSymbol_OnePoint
{
public:
	CNumber_Five(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, 
		const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, 
		const LOGFONT& logfont, const COLORREF& textColor);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CNumber_Five_Draw m_numberFiveDrawer;
};

///////////
//Num_Six
///////////
class CNumber_Six : public CDrawingSymbol_OnePoint
{
public:
	CNumber_Six(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, 
		const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, 
		const LOGFONT& logfont, const COLORREF& textColor);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CNumber_Six_Draw m_numberSixDrawer;
};

///////////
//Num_Seven
///////////
class CNumber_Seven : public CDrawingSymbol_OnePoint
{
public:
	CNumber_Seven(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, 
		const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, 
		const LOGFONT& logfont, const COLORREF& textColor);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CNumber_Seven_Draw m_numberSevenDrawer;
};

///////////
//Num_Eight
///////////
class CNumber_Eight : public CDrawingSymbol_OnePoint
{
public:
	CNumber_Eight(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, 
		const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, 
		const LOGFONT& logfont, const COLORREF& textColor);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CNumber_Eight_Draw m_numberEightDrawer;
};

///////////
//Num_Nine
///////////
class CNumber_Nine : public CDrawingSymbol_OnePoint
{
public:
	CNumber_Nine(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, 
		const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, 
		const LOGFONT& logfont, const COLORREF& textColor);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CNumber_Nine_Draw m_numberNineDrawer;
};

///////////
//Text_A
///////////
class CText_A : public CDrawingSymbol_OnePoint
{
public:
	CText_A(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CText_A_Draw m_textADrawer;
};

///////////
//Text_B
///////////
class CText_B : public CDrawingSymbol_OnePoint
{
public:
	CText_B(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CText_B_Draw m_textBDrawer;
};

///////////
//Text_C
///////////
class CText_C : public CDrawingSymbol_OnePoint
{
public:
	CText_C(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CText_C_Draw m_textCDrawer;
};

///////////
//Text_D
///////////
class CText_D : public CDrawingSymbol_OnePoint
{
public:
	CText_D(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CText_D_Draw m_textDDrawer;
};

///////////
//Text_E
///////////
class CText_E : public CDrawingSymbol_OnePoint
{
public:
	CText_E(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CText_E_Draw m_textEDrawer;
};

///////////
//Text_F
///////////
class CText_F : public CDrawingSymbol_OnePoint
{
public:
	CText_F(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CText_F_Draw m_textFDrawer;
};

///////////
//Text_G
///////////
class CText_G : public CDrawingSymbol_OnePoint
{
public:
	CText_G(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CText_G_Draw m_textGDrawer;
};

///////////
//Text_H
///////////
class CText_H : public CDrawingSymbol_OnePoint
{
public:
	CText_H(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont, const COLORREF& textColor);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CText_H_Draw m_textHDrawer;
};

///////////
//Char_1
///////////
class CChar_1 : public CDrawingSymbol_OnePoint
{
public:
	CChar_1(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CChar_1_Draw m_char1Drawer;
};

///////////
//Char_2
///////////
class CChar_2 : public CDrawingSymbol_OnePoint
{
public:
	CChar_2(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CChar_2_Draw m_char2Drawer;
};

///////////
//Char_3
///////////
class CChar_3 : public CDrawingSymbol_OnePoint
{
public:
	CChar_3(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CChar_3_Draw m_char3Drawer;
};

///////////
//Char_4
///////////
class CChar_4 : public CDrawingSymbol_OnePoint
{
public:
	CChar_4(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CChar_4_Draw m_char4Drawer;
};

///////////
//Char_5
///////////
class CChar_5 : public CDrawingSymbol_OnePoint
{
public:
	CChar_5(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CChar_5_Draw m_char5Drawer;
};

///////////
//Char_6
///////////
class CChar_6 : public CDrawingSymbol_OnePoint
{
public:
	CChar_6(const CSymbolBaseData::TOOLTYPE SymbolType, const CSymbolBaseData::SIZETYPE& sizetype, const CCoordinate& startPt, const CSize& symbol_size, const COLORREF& color, const LOGFONT& logfont);

	virtual void Draw(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);
	virtual void DrawSelectedMark(CDC* pDC, const CRect& drawingRegion, const CCoordinateCalculatorData& coordinateData);

	virtual bool IsSelected(const CRect& drawingRegion, const CPoint &point, const CCoordinateCalculatorData& coordinateData);

private:
	CChar_6_Draw m_char6Drawer;
};
#endif // !defined(AFX_DRAWINGSYMBOL_H__63F59AE2_09CA_42DA_8E1F_E44F1DFFE217__INCLUDED_)
