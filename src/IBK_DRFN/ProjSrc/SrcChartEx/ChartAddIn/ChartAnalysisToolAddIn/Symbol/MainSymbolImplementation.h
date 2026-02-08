// MainSymbolImplementation.h: interface for the CMainSymbolImplementation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINSYMBOLIMPLEMENTATION_H__1F229275_48B5_490D_B1E9_0E54D5FA62EC__INCLUDED_)
#define AFX_MAINSYMBOLIMPLEMENTATION_H__1F229275_48B5_490D_B1E9_0E54D5FA62EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <afxtempl.h>

#include "../Include_Analysis/BlockBaseEnum2.h"				// for CSymbol

//기호 Dlg
#include "SymbolDlg.h"
//symboldata
#include "SymbolData.h"
//CBlockIndex
#include "BlockBaseData.h"
#include "SymbolSingleton.h"

class CSymbolTool;
class CMainSymbolImplementation  
{
public:
	CMainSymbolImplementation();
	virtual ~CMainSymbolImplementation();

	//Object Dll를 받는다.
	void SetMainBlock_Object(IChartManager* pMainBlock,IChartOCX *p_pIChartOCX);
	void InitializeBlock(CDC* pDC, CWnd* pParentWnd);
	void SetCurrentSymbolTool(const CSymbol::SYMBOLTOOL symbolType);
	void SetShapeDrawingType(const COLORREF& crLine,const int& nStyle,const int& nWeight,const bool& bFill);
	//기호Dlg를 생성하고 띄운다.
	void CreateSymbolDlg();

	bool OnLButtonDown(CDC* pDC, const CPoint &point);
	bool OnLButtonDbClk(CDC* pDC, const CPoint &point);
	bool OnLButtonUp(CDC* pDC, const CPoint &point);
	bool OnMouseMove(CDC *pDC,const UINT &nFlags, const CPoint &point);
	bool OnRButtonUp(const CPoint &point);
	bool OnRButtonMenu(const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);

	void OnDraw(CDC *pDC);	
	void OnSetColor(const COLORREF& color);

	//Block을 Insert한다.
	bool InsertBlockIndex(const CBlockIndex& insertBlockIndex);
	//Block의 이동으로 Row, Column을 바꾼다.
	void SetModifiedBlockIndexs(const CBlockIndex& sourceBlockIndex, const CBlockIndex& TargetBlockIndex);

	//해당 Symbol을 지운다.
	bool DeleteCurrentSymbols();
	void DeleteAllSymbols();
	//블럭까지 지운다.
	void DeleteAllSymbolsInBlock(const CBlockIndex& blockIndex);

	void DeleteSymbolDlg();

	void Initial();
	CString GetSavedSymbolsData();
	void BuildSymbolsFromString(const CString& Data);

	void SetFilePath(const CString& path);

	CSymbol::SYMBOLTOOL GetToolType();
	CSymbol::SYMBOLTOOL GetSelectedToolType();
	void DeleteLastSymbol();
	bool DeleteSymbolPos(const long lAddress);
	bool UndoElement(const long lAddress);
	bool RedoElement(const long lAddress);

	void SetElementCopy();

	void DrawSymbolTool(CDC *pDC);
	CRect GetDrawingTotalRegion(CPoint point);

	void ChangeAllSymbolsColor(const COLORREF& crLine,const COLORREF& crText);
	// 20081007 JS.Kim	사용자시간대 선택
	bool RecalcSymbolXPoint(const char* szName, int nTimeDiff);
	void RecalcElementListXPoint(int row, int col, int nBaseTimeDiff, IPacket* pPacket);

	// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	CScaleBaseData::HORZSCALEDRAWERTYPE m_eScaleType;
	void SetXScaleDrawType(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType);
	CScaleBaseData::HORZSCALEDRAWERTYPE GetXScaleDrawType();
	// 2011.02.07 by SYS <<

protected:
	static CMap<int, int, CSymbolDlg*, CSymbolDlg*>	g_mapSymbolDlg;
	static CMap<int, int, CSymbolBaseData::TOOLTYPE,CSymbolBaseData::TOOLTYPE> g_mapSymbolType;
	
public:
	static	void	InitSymbolDlg();
	static	void	DestSymbolDlg();
	static	BOOL	IsExistBuletDlg();
	static  void	SetSymbolType(const CSymbolBaseData::TOOLTYPE bulettype);

private:
	IChartOCX *m_pIChartOCX;
	
	CPoint m_ptOldMouse;

	// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	CPoint m_ptOldLBtnUp;
	// 2011.02.08 by SYS <<

	//기호 Dlg의 포인터.
	CSymbolDlg* m_pSymbolDlg;
	//선택된 Symbol의 포인터.
	CDrawingSymbol* m_pDrawingSymbol;
	//선택된 Symbol Tool.
	CSymbolTool* m_pSymbolTool;	
	//symboldata
	CSymbolData m_symboldata;
	CSymbolSingleton m_singleton;
	
	//mouse을 내가 잡고 있어야 하는건지의 여부.
	bool IsMyMouse();
	//현재의 Tool
	void GetSymbolTool();
	//color 
	bool SetColor(const COLORREF& symbolColor);
};

#endif // !defined(AFX_MAINSYMBOLIMPLEMENTATION_H__1F229275_48B5_490D_B1E9_0E54D5FA62EC__INCLUDED_)
