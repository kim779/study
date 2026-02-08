// DrawingSymbolFactory.h: interface for the CDrawingSymbolFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWINGSYMBOLFACTORY_H__13ED0774_1C73_43DF_83DE_A2B2B04014E2__INCLUDED_)
#define AFX_DRAWINGSYMBOLFACTORY_H__13ED0774_1C73_43DF_83DE_A2B2B04014E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Coordinate.h"
//#include "BlockBaseData.h"
#include "SymbolBaseData.h"

class CDrawingSymbol;
class CDrawingSymbolFactory  
{
public:
	static CDrawingSymbol* Copy(CDrawingSymbol *pDrawingSymbol);
	static CDrawingSymbol* Make(const CSymbolBaseData::TOOLTYPE SymbolType,
								const CCoordinate& startPt,
								const CCoordinate& EndPt, 
								const COLORREF& color,
								const int& nLineStyle,
								const int& nLineWeight,
								const bool& bFill,
								const LOGFONT& logfont,
								const bool bDrawFromTop);
	static CDrawingSymbol* Make(const CSymbolBaseData::TOOLTYPE SymbolType,
								const CSymbolBaseData::SIZETYPE& sizetype, 
								const CCoordinate& startPt, 
								const CSize& SymbolSize,
								const COLORREF& color, 
								const LOGFONT& logfont, 
								const COLORREF& textColor);
	static void Remove(CDrawingSymbol* symbol);
};

#endif // !defined(AFX_DRAWINGSYMBOLFACTORY_H__13ED0774_1C73_43DF_83DE_A2B2B04014E2__INCLUDED_)
