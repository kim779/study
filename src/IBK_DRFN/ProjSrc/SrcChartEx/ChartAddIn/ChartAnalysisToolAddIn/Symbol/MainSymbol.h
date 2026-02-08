// MainSymbol.h: interface for the CMainSymbol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINSYMBOL_H__4618CED0_E651_42B6_AD07_DC26FD884311__INCLUDED_)
#define AFX_MAINSYMBOL_H__4618CED0_E651_42B6_AD07_DC26FD884311__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Analysis/Include_AddIn.h"				// for Ixxxxxx.h
#include "../Include_Analysis/BlockBaseEnum2.h"				// for CSymbol

#include "BlockBaseData.h"

class CMainSymbolImplementation;
class CMainSymbol
{
public:
	CSymbol::SYMBOLTOOL GetToolType();
	CSymbol::SYMBOLTOOL GetSelectedToolType();
	void InitializeBlock(CDC* pDC, CWnd* pParentWnd);
	CMainSymbol();
	~CMainSymbol();

	// ChartObject.dll 주소
	void SetMainBlock_Object(IChartManager* pMainBlock,IChartOCX *p_pIChartOCX);
	void CreateSymbolDlg();
	BOOL IsExistBuletDlg();
	void DeleteSymbolDlg();	
	void SetCurrentSymbolTool(const CSymbol::SYMBOLTOOL symbolType);
	void SetShapeDrawingType(const COLORREF& crLine,const int& nStyle,const int& nWeight,const bool& bFill);
	bool OnLButtonDown(CDC* pDC, const CPoint &point);
	bool OnLButtonUp(CDC* pDC, const CPoint &point);
	bool OnMouseMove(CDC* pDC,const UINT &nFlags, const CPoint &point);
	bool OnRButtonUp(const CPoint &point);
	bool OnLButtonDbClk(CDC* pDC, const CPoint &point);
	void OnDraw(CDC* pDC);
	bool InsertBlockIndex(const CBlockIndex& insertBlockIndex);
	void SetModifiedBlockIndexs(const CBlockIndex &sourceBlockIndex, const CBlockIndex &TargetBlockIndex);
	// element 삭제
	void DeleteCurrentSymbols();
	// element 삭제, 맨마지막에 생성된 객체를 삭제
	void DeleteLastSymbol();
	bool DeleteSymbolPos(const long lAddress);
	void DeleteAllSymbols();
	void DeleteAllSymbolsInBlock(const CBlockIndex& blockIndex);
	bool OnRButtonMenu(const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);
	void OnSetColor(const COLORREF& color);
	CString GetSavedSymbolsData();

	void BuildSymbolsFromString(const CString& Data);
	
	void SetFilePath(const CString& path);
	void SetElementCopy();
	bool UndoElement(const long lAddress);
	bool RedoElement(const long lAddress);

	void DrawSymbolTool(CDC *pDC);
	CRect GetDrawingTotalRegion(CPoint point);

	void ChangeAllSymbolsColor(const COLORREF& crLine,const COLORREF& crText);
	// 20081007 JS.Kim	사용자시간대 선택
	void RecalcSymbolXPoint(const char* szName, int nTimeDiff);

	// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	void SetXScaleDrawType(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType);
	// 2011.02.07 by SYS <<


private:
	CMainSymbolImplementation* m_Implementation;
};

#endif // !defined(AFX_MAINSYMBOL_H__4618CED0_E651_42B6_AD07_DC26FD884311__INCLUDED_)
