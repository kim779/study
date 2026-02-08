// ElementTree.h: interface for the CElementTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELEMENTTREE_H__D8C0A3A5_5A41_4103_B4E7_FEC4B98765F2__INCLUDED_)
#define AFX_ELEMENTTREE_H__D8C0A3A5_5A41_4103_B4E7_FEC4B98765F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseData.h"
#include "Order.h"
#include "Tree.h"

class CRegion_AllBlock;
class CDrawingSymbol;
class CElementTree  
{
public:
	void DeleteLastSymbol();
	bool DeleteSymbolPos(const long lAddress,BOOL bListOnly=FALSE);
	//객체 생성.
	void Add(const CBlockIndex& blockIndex, CDrawingSymbol* pDrawingSymbol);
	void AddColumnRowSet(const CBlockIndex& blockIndex);

	//point에 해당하는 블럭의 맨 마지막 symbol의 포인터를 얻는다.
	CDrawingSymbol* GetTail();
	CDrawingSymbol* FindSymbol(const CPoint &point, IChartManager* pMainBlock);

	// 현재 있는 element count
	int GetElementCount() const;
	// block에 있는 element count
	void GetElementListOfBlock(const CBlockIndex& blockIndex, CList<CDrawingSymbol*, CDrawingSymbol*>& elementList);

	//생성되어 있는 모든 객체를 draw한다.
	void DrawElement(CDC *pDC, IChartManager* pMainBlock, CDrawingSymbol *pSelected);
	long CopyElement(CDrawingSymbol *pSelected);	
	long MoveElement(CDrawingSymbol *pSelected);

	//symbol지우기
	bool DeleteCurrentDrawingSymbol(const CDrawingSymbol* pDrawingSymbol);
	void DeleteAllSymbolsInBlock(const CBlockIndex &blockIndex);
	void DeleteAllSymbols();

	//block추가, 블럭 이동과 관려해서 symbol리스트를 이동시킨다.
	bool InsertBlockIndex(const CBlockIndex &insertBlockIndex);
	void SetModifiedBlockIndexs(const CBlockIndex &sourceBlockIndex, const CBlockIndex &TargetBlockIndex);

	//객체 저장에 관련된 내용.
	CString GetSavedSymbolsData();

	// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//void BuildSymbolsFromString(const CString &data);
	void BuildSymbolsFromString(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType, const CString &data);
	// 2011.02.07 by SYS <<

	void ChangeAllSymbolsColor(const COLORREF& penColor, const COLORREF& textColor);

	CBlockIndex GetBlockIndexInSymbol(const CDrawingSymbol* pSelectedSymbol) const;
	CTree& GetTree();
	COrder& GetOrder();
	
private:
	COrder m_order;
	CTree m_tree;

};

#endif // !defined(AFX_ELEMENTTREE_H__D8C0A3A5_5A41_4103_B4E7_FEC4B98765F2__INCLUDED_)
