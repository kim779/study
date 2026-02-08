// Tree.h: interface for the CTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREE_H__5BFCF715_9AB7_4186_8DDD_0D3DDB2E9EA2__INCLUDED_)
#define AFX_TREE_H__5BFCF715_9AB7_4186_8DDD_0D3DDB2E9EA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "../Include_Analysis/Include_AddIn.h"				// for Ixxxxxx.h
#include "BlockBaseData.h"

//순서대로 저장된 리스트 
#include "Order.h"

class CDrawingSymbolColumn;
class CDrawingSymbolRow;
class CDrawingSymbol;
class CTree  
{
public:
	virtual ~CTree();

	//모든 Symbol를 그린다.
	void OnDraw(CDC *pDC, IChartManager* pMainBlock, CDrawingSymbol *pSelected);
	long CopyElement(CDrawingSymbol *pSelected);	
	long MoveElement(CDrawingSymbol *pSelected);

	//data저장할 string을 만든다.
	CString GetSavedSymbolsData(const COrder& order);

	void GetElementList(const CBlockIndex &blockIndex, CList<CDrawingSymbol*, CDrawingSymbol*>& elementList);
	//현재 해당 Row 찾기
	CDrawingSymbolRow* GetCurBlockDrawingSymbol(const CBlockIndex& TargetBlockIndex);

	//기호가 추가되었을때 add한다.
	void Add(const CBlockIndex &blockIndex, CDrawingSymbol *pDrawingSymbol);
	void Add(const int nColumn, const int nRow, CDrawingSymbol *pDrawingSymbol);
	void AddColumnRowSet(const CBlockIndex &blockIndex);
	//Row, Column을 추가, 삭제하고 나서 순서를 다시 맞춘다.
	void SetModifiedBlockIndexs(const CBlockIndex &sourceBlockIndex, const CBlockIndex &TargetBlockIndex);

	//Block을 Insert한다.
	bool InsertBlockIndex(const CBlockIndex &insertBlockIndex);
	
	//해당 Symbol을 지운다.
	void DeleteAllSymbols();
	void DeleteAllSymbolsInBlock(const CBlockIndex &blockIndex);
	bool DeleteCurrentDrawingSymbol(const CDrawingSymbol *pDrawingSymbol);
	bool DeleteCurrentDrawingSymbol(const long lAddressSymbol,BOOL bListOnly);

	//추세선 모든 색상 변경
	void ChangeAllSymbolsColor(const COLORREF& penColor, const COLORREF& textColor);

	// Undo / Redo
	CBlockIndex GetBlockIndexInSymbol(const CDrawingSymbol* pSelectedSymbol) const;

private:
	void AddColumn(const int nColumnCount);
	//Column리스트.
	CList<CDrawingSymbolColumn*, CDrawingSymbolColumn* > m_DrawingSymbolColumnList; 
	//현재 칼럼 얻기.
	CDrawingSymbolColumn* GetCurrentColumn(const int nColumn);

	//기호가 추가되었을때 add한다. 
	//이미 column이 있을때와 새로 Column을 만들어서 추가해야 하는경우
	void AddElement(const int rowIndex, CDrawingSymbolColumn* pColumn, CDrawingSymbol *pDrawingSymbol);
	void AddElementInNewMakedColumnSet(const CBlockIndex &blockIndex, CDrawingSymbol *pDrawingSymbol);

	//Block의 Row, Column이 변경되었을때 m_DrawingSymbolColumnList의 정보를 변경한다.
	void ChangeRowList(const CBlockIndex &sourceBlockIndex, const CBlockIndex &TargetBlockIndex);
	void ChangeColumnList(const CBlockIndex &sourceBlockIndex, const CBlockIndex &TargetBlockIndex);

};

#endif // !defined(AFX_TREE_H__5BFCF715_9AB7_4186_8DDD_0D3DDB2E9EA2__INCLUDED_)
