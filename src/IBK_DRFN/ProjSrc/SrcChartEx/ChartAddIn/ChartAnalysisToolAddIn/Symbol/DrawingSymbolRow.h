// DrawingSymbolRow.h: interface for the CDrawingSymbolRow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWINGSYMBOLROW_H__E482D324_FE58_4BA8_B74B_0A4E4DD32FD1__INCLUDED_)
#define AFX_DRAWINGSYMBOLROW_H__E482D324_FE58_4BA8_B74B_0A4E4DD32FD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "../Include_Analysis/Include_AddIn.h"				// for Ixxxxxx.h

#include "Order.h"

class CCoordinateCalculatorData;
class CRegion_AllBlock;
class CDrawingSymbol;
class CDrawingSymbolRow  
{
public:
	void GetElementList(CList<CDrawingSymbol*, CDrawingSymbol*>& elementList);

	CDrawingSymbolRow();
	virtual ~CDrawingSymbolRow();

	//m_SymbolList와 관련된 Add, find, delete이다.
	CDrawingSymbol* Add(CDrawingSymbol* pSymbol);
	CDrawingSymbol* Find(const CPoint &point, const int nColumn, const int nRow, IChartManager* pMainBlock);
	CDrawingSymbol* GetTail();

	bool DeleteCurrentDrawingSymbol(const CDrawingSymbol* pDrawingSymbol);
	bool DeleteCurrentDrawingSymbol(const long lAddressSymbol,BOOL bListOnly);
	void DeleteAllDrawingSymbol();

	//그래프영역, column, row, count를 얻는다.
	int GetCount();
	CRect GetGraphRegion(const int nColumn, const int nRow, const CRegion_AllBlock& drawingRegions);
	CDrawingSymbol* GetAt(const int nIndex);

	CString GetSavedSymbolsData(const COrder &order);
	//가지고 있는 list의 모든 기호를 그린다.
	void OnDraw(CDC* pDC, IChartManager* pMainBlock, CDrawingSymbol *pSelected, const int nColumn, const int nRow, const CRegion_AllBlock& drawingRegions);
	long CopyElement(CDrawingSymbol *pSelected);
	long MoveElement(CDrawingSymbol *pSelected);

	void ChangeAllSymbolsColor(const COLORREF& penColor, const COLORREF& textColor);

	// Undo / Redo
	bool IsInSameSymbol(const CDrawingSymbol* pSymbol) const;

private:
	CList<CDrawingSymbol*, CDrawingSymbol*> m_Symbol_List;
};

#endif // !defined(AFX_DRAWINGSYMBOLROW_H__E482D324_FE58_4BA8_B74B_0A4E4DD32FD1__INCLUDED_)
