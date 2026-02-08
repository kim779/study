// DrawingSymbolFactory.cpp: implementation of the CDrawingSymbolFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "symbol.h"
#include "DrawingSymbolFactory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "DrawingSymbol.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawingSymbol* CDrawingSymbolFactory::Copy(CDrawingSymbol *pDrawingSymbol)
{
	CSymbolBaseData::TOOLTYPE SymbolType = pDrawingSymbol->GetSymbolType();
	CSymbolBaseData::SIZETYPE sizetype = pDrawingSymbol->GetSizeType();

	COLORREF color = pDrawingSymbol->GetColor();
	int nLineStyle = pDrawingSymbol->GetStyle();
	int nLineWeight = pDrawingSymbol->GetWeight();
	bool bFill = pDrawingSymbol->GetFillState();
	LOGFONT logfont = pDrawingSymbol->GetLogFont(); 
	bool bDrawFromTop = true;
	CSize SymbolSize = pDrawingSymbol->GetSymbolSize();
	COLORREF textColor = pDrawingSymbol->GetTextColor();

	CCoordinate startPt = pDrawingSymbol->GetStartPoint();
	CCoordinate EndPt = pDrawingSymbol->GetEndPoint();
	
	switch(SymbolType)
	{
		case CSymbolBaseData::SYMBOL_SQUARE:// 사각도형
			return new CFigure_Square(SymbolType, startPt, EndPt, color,nLineStyle,nLineWeight,bFill, logfont, bDrawFromTop);
		case CSymbolBaseData::SYMBOL_CIRCLE:	// 원도형
			return new CFigure_Circle(SymbolType, startPt, EndPt, color,nLineStyle,nLineWeight,bFill, logfont, bDrawFromTop);
		case CSymbolBaseData::SYMBOL_TRIANGLE:	// 삼각형
			return new CFigure_Triangle(SymbolType, startPt, EndPt, color,nLineStyle,nLineWeight,bFill, logfont, bDrawFromTop);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case CSymbolBaseData::SYMBOL_TEXT:// Text
			return new CText(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor,pDrawingSymbol->GetText());
		case CSymbolBaseData::SYMBOL_ARROW_LEFT:	// 왼쪽 화살표
			return new CArrow_Left(SymbolType, sizetype, startPt, SymbolSize, color, logfont);
		case CSymbolBaseData::SYMBOL_ARROW_RIGHT:	// 오른쪽 화살표
			return new CArrow_Right(SymbolType, sizetype, startPt, SymbolSize, color, logfont);
		case CSymbolBaseData::SYMBOL_ARROW_UP: //위쪽 화살표
			return new CArrow_Up(SymbolType, sizetype, startPt, SymbolSize, color, logfont);
		case CSymbolBaseData::SYMBOL_ARROW_DOWN:  //아래쪽 화살표
			return new CArrow_Down(SymbolType, sizetype, startPt, SymbolSize, color, logfont);
		case CSymbolBaseData::SYMBOL_ARROW_LEFTTOP:  //왼쪽 위 방향 화살표
			return new CArrow_LeftTop(SymbolType, sizetype, startPt, SymbolSize, color, logfont);
		case CSymbolBaseData::SYMBOL_ARROW_LEFTBOTTOM: //왼쪽 아래 방향 화살표
			return new CArrow_LeftBottom(SymbolType, sizetype, startPt, SymbolSize, color, logfont);
		case CSymbolBaseData::SYMBOL_ARROW_RIGHTTOP:  //오른쪽 위 방향 화살표
			return new CArrow_RightTop(SymbolType, sizetype, startPt, SymbolSize, color, logfont);
		case CSymbolBaseData::SYMBOL_ARROW_RIGHTBOTTOM:  //오른쪽 아래 방향 화살표
			return new CArrow_RightBottom(SymbolType, sizetype, startPt, SymbolSize, color, logfont);
		case CSymbolBaseData::SYMBOL_NUM_ONE:  //숫자 1
			return new CNumber_One(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
		case CSymbolBaseData::SYMBOL_NUM_TWO:  //2
			return new CNumber_Two(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
		case CSymbolBaseData::SYMBOL_NUM_THREE: //3
			return new CNumber_Three(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
		case CSymbolBaseData::SYMBOL_NUM_FOUR:  //4
			return new CNumber_Four(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
		case CSymbolBaseData::SYMBOL_NUM_FIVE:  //5
			return new CNumber_Five(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
		case CSymbolBaseData::SYMBOL_NUM_SIX:  //6
			return new CNumber_Six(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
		case CSymbolBaseData::SYMBOL_NUM_SEVEN:  //7
			return new CNumber_Seven(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
		case CSymbolBaseData::SYMBOL_NUM_EIGHT:  //8
			return new CNumber_Eight(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
		case CSymbolBaseData::SYMBOL_NUM_NINE:  //9
			return new CNumber_Nine(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
		//case CSymbolBaseData::SYMBOL_NUM_Ten:  //10
		//	return new CNumber_Ten(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
		case CSymbolBaseData::SYMBOL_TEXT_A:  //a
			return new CText_A(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
		case CSymbolBaseData::SYMBOL_TEXT_B: //b
			return new CText_B(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
		case CSymbolBaseData::SYMBOL_TEXT_C: //c
			return new CText_C(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
		case CSymbolBaseData::SYMBOL_TEXT_D: //d
			return new CText_D(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
		case CSymbolBaseData::SYMBOL_TEXT_E: //e
			return new CText_E(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
		case CSymbolBaseData::SYMBOL_TEXT_F: //f
			return new CText_F(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
		case CSymbolBaseData::SYMBOL_TEXT_G: //g
			return new CText_G(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
		case CSymbolBaseData::SYMBOL_TEXT_H: //h
			return new CText_H(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
		case CSymbolBaseData::SYMBOL_CHAR_1: //h
			return new CChar_1(SymbolType, sizetype, startPt, SymbolSize, color, logfont);//, textColor);
		case CSymbolBaseData::SYMBOL_CHAR_2: //h
			return new CChar_2(SymbolType, sizetype, startPt, SymbolSize, color, logfont);//, textColor);
		case CSymbolBaseData::SYMBOL_CHAR_3: //h
			return new CChar_3(SymbolType, sizetype, startPt, SymbolSize, color, logfont);//, textColor);
		case CSymbolBaseData::SYMBOL_CHAR_4: //h
			return new CChar_4(SymbolType, sizetype, startPt, SymbolSize, color, logfont);//, textColor);
		case CSymbolBaseData::SYMBOL_CHAR_5: //h
			return new CChar_5(SymbolType, sizetype, startPt, SymbolSize, color, logfont);//, textColor);
		case CSymbolBaseData::SYMBOL_CHAR_6: //h
			return new CChar_6(SymbolType, sizetype, startPt, SymbolSize, color, logfont);//, textColor);
		default:
			return NULL;
	}

	return NULL;
}

CDrawingSymbol* CDrawingSymbolFactory::Make(const CSymbolBaseData::TOOLTYPE SymbolType,
											const CCoordinate& startPt,
											const CCoordinate& EndPt,
											const COLORREF& color,
											const int& nLineStyle,
											const int& nLineWeight,
											const bool& bFill,
											const LOGFONT& logfont,
											const bool bDrawFromTop)
{
	switch(SymbolType){
		case CSymbolBaseData::SYMBOL_SQUARE:// 사각도형
			return new CFigure_Square(SymbolType, startPt, EndPt, color,nLineStyle,nLineWeight,bFill, logfont, bDrawFromTop);
		case CSymbolBaseData::SYMBOL_CIRCLE:	// 원도형
			return new CFigure_Circle(SymbolType, startPt, EndPt, color,nLineStyle,nLineWeight,bFill, logfont, bDrawFromTop);
		case CSymbolBaseData::SYMBOL_TRIANGLE:	// 삼각형
			return new CFigure_Triangle(SymbolType, startPt, EndPt, color,nLineStyle,nLineWeight,bFill, logfont, bDrawFromTop);
		default:
			return NULL;
	}
}

CDrawingSymbol* CDrawingSymbolFactory::Make(const CSymbolBaseData::TOOLTYPE SymbolType,
											const CSymbolBaseData::SIZETYPE& sizetype,
											const CCoordinate& startPt,
											const CSize& SymbolSize,
											const COLORREF& color, 
											const LOGFONT& logfont, 
											const COLORREF& textColor)
{
	switch(SymbolType){
	case CSymbolBaseData::SYMBOL_TEXT:// Text
		return new CText(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
	case CSymbolBaseData::SYMBOL_ARROW_LEFT:	// 왼쪽 화살표
		return new CArrow_Left(SymbolType, sizetype, startPt, SymbolSize, color, logfont);
	case CSymbolBaseData::SYMBOL_ARROW_RIGHT:	// 오른쪽 화살표
		return new CArrow_Right(SymbolType, sizetype, startPt, SymbolSize, color, logfont);
	case CSymbolBaseData::SYMBOL_ARROW_UP: //위쪽 화살표
		return new CArrow_Up(SymbolType, sizetype, startPt, SymbolSize, color, logfont);
	case CSymbolBaseData::SYMBOL_ARROW_DOWN:  //아래쪽 화살표
		return new CArrow_Down(SymbolType, sizetype, startPt, SymbolSize, color, logfont);
	case CSymbolBaseData::SYMBOL_ARROW_LEFTTOP:  //왼쪽 위 방향 화살표
		return new CArrow_LeftTop(SymbolType, sizetype, startPt, SymbolSize, color, logfont);
	case CSymbolBaseData::SYMBOL_ARROW_LEFTBOTTOM: //왼쪽 아래 방향 화살표
		return new CArrow_LeftBottom(SymbolType, sizetype, startPt, SymbolSize, color, logfont);
	case CSymbolBaseData::SYMBOL_ARROW_RIGHTTOP:  //오른쪽 위 방향 화살표
		return new CArrow_RightTop(SymbolType, sizetype, startPt, SymbolSize, color, logfont);
	case CSymbolBaseData::SYMBOL_ARROW_RIGHTBOTTOM:  //오른쪽 아래 방향 화살표
		return new CArrow_RightBottom(SymbolType, sizetype, startPt, SymbolSize, color, logfont);
	case CSymbolBaseData::SYMBOL_NUM_ONE:  //숫자 1
		return new CNumber_One(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
	case CSymbolBaseData::SYMBOL_NUM_TWO:  //2
		return new CNumber_Two(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
	case CSymbolBaseData::SYMBOL_NUM_THREE: //3
		return new CNumber_Three(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
	case CSymbolBaseData::SYMBOL_NUM_FOUR:  //4
		return new CNumber_Four(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
	case CSymbolBaseData::SYMBOL_NUM_FIVE:  //5
		return new CNumber_Five(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
	case CSymbolBaseData::SYMBOL_NUM_SIX:  //6
		return new CNumber_Six(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
	case CSymbolBaseData::SYMBOL_NUM_SEVEN:  //7
		return new CNumber_Seven(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
	case CSymbolBaseData::SYMBOL_NUM_EIGHT:  //8
		return new CNumber_Eight(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
	case CSymbolBaseData::SYMBOL_NUM_NINE:  //9
		return new CNumber_Nine(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
	//case CSymbolBaseData::SYMBOL_NUM_SIX:  //10
	//	return new CNumber_Ten(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
	case CSymbolBaseData::SYMBOL_TEXT_A:  //a
		return new CText_A(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
	case CSymbolBaseData::SYMBOL_TEXT_B: //b
		return new CText_B(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
	case CSymbolBaseData::SYMBOL_TEXT_C: //c
		return new CText_C(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
	case CSymbolBaseData::SYMBOL_TEXT_D: //d
		return new CText_D(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
	case CSymbolBaseData::SYMBOL_TEXT_E: //e
		return new CText_E(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
	case CSymbolBaseData::SYMBOL_TEXT_F: //f
		return new CText_F(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
	case CSymbolBaseData::SYMBOL_TEXT_G: //g
		return new CText_G(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
	case CSymbolBaseData::SYMBOL_TEXT_H: //h
		return new CText_H(SymbolType, sizetype, startPt, SymbolSize, color, logfont, textColor);
	case CSymbolBaseData::SYMBOL_CHAR_1: //h
		return new CChar_1(SymbolType, sizetype, startPt, SymbolSize, color, logfont);//, textColor);
	case CSymbolBaseData::SYMBOL_CHAR_2: //h
		return new CChar_2(SymbolType, sizetype, startPt, SymbolSize, color, logfont);//, textColor);
	case CSymbolBaseData::SYMBOL_CHAR_3: //h
		return new CChar_3(SymbolType, sizetype, startPt, SymbolSize, color, logfont);//, textColor);
	case CSymbolBaseData::SYMBOL_CHAR_4: //h
		return new CChar_4(SymbolType, sizetype, startPt, SymbolSize, color, logfont);//, textColor);
	case CSymbolBaseData::SYMBOL_CHAR_5: //h
		return new CChar_5(SymbolType, sizetype, startPt, SymbolSize, color, logfont);//, textColor);
	case CSymbolBaseData::SYMBOL_CHAR_6: //h
		return new CChar_6(SymbolType, sizetype, startPt, SymbolSize, color, logfont);//, textColor);
	default:
		return NULL;
	}
}

// 객체를 삭제한다. Make에서 new를 했으므로..
void CDrawingSymbolFactory::Remove(CDrawingSymbol* symbol)
{
	delete symbol;
}