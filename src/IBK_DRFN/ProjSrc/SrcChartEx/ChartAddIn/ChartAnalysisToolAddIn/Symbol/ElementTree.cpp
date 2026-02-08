// ElementTree.cpp: implementation of the CElementTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "symbol.h"
#include "ElementTree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "DrawingSymbolRow.h"
//builder
#include "ElementSaverBuilder.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void CElementTree::SetModifiedBlockIndexs(const CBlockIndex &sourceBlockIndex, const CBlockIndex &TargetBlockIndex)
{
	m_tree.SetModifiedBlockIndexs(sourceBlockIndex, TargetBlockIndex); 
}

bool CElementTree::InsertBlockIndex(const CBlockIndex &insertBlockIndex)
{
	return m_tree.InsertBlockIndex(insertBlockIndex);
}

void CElementTree::DeleteAllSymbolsInBlock(const CBlockIndex &blockIndex)
{
	CList<CDrawingSymbol*, CDrawingSymbol*> elementList;
	m_tree.GetElementList(blockIndex, elementList);

//	if(elementList.GetCount() <= 0)
//		return;

	m_order.RemoveAllInRow(elementList);
	m_tree.DeleteAllSymbolsInBlock(blockIndex);
}

bool CElementTree::DeleteCurrentDrawingSymbol(const CDrawingSymbol *pDrawingSymbol)
{
	m_order.RemoveAt(pDrawingSymbol);
	return m_tree.DeleteCurrentDrawingSymbol(pDrawingSymbol);
}

void CElementTree::Add(const CBlockIndex &blockIndex, CDrawingSymbol *pDrawingSymbol)
{
	m_order.Add(pDrawingSymbol);
	m_tree.Add(blockIndex, pDrawingSymbol);
}

void CElementTree::AddColumnRowSet(const CBlockIndex& blockIndex)
{
	m_tree.AddColumnRowSet(blockIndex);
}

void CElementTree::DrawElement(CDC *pDC, IChartManager* pMainBlock, CDrawingSymbol *pSelected)
{
	m_tree.OnDraw(pDC, pMainBlock, pSelected);
}

//{{ 2007.07.16 by LYH 
//long CElementTree::CopyElement(CDrawingSymbol *pSelected)
//{
//	return m_tree.CopyElement(pSelected);
//}

long CElementTree::CopyElement(CDrawingSymbol *pSelected)
{
	long lAddress = m_tree.CopyElement(pSelected);
	if(lAddress)
		m_order.Add((CDrawingSymbol*)lAddress);
	return lAddress;
}
//}}

long CElementTree::MoveElement(CDrawingSymbol *pSelected)
{
	return m_tree.MoveElement(pSelected);
}

CDrawingSymbol* CElementTree::FindSymbol(const CPoint &point, IChartManager *pMainBlock)
{
	int nRow, nColumn;
	if( !pMainBlock->GetBlockIndex(point, nRow, nColumn)) return NULL;;
	CBlockIndex blockIndex( nRow, nColumn);
	if(!blockIndex.IsAllValueBiggerThanZero())
		return NULL;

	CDrawingSymbolRow* pRow = m_tree.GetCurBlockDrawingSymbol(blockIndex);
	if(pRow == NULL)
		return NULL;
	return pRow->Find(point, blockIndex.GetColumn(), blockIndex.GetRow(), pMainBlock);
}

CDrawingSymbol* CElementTree::GetTail()
{
	return m_order.GetTail();
/*
	CBlockIndex blockIndex = pMainBlock->FindBlockNumber(point);
	if(!blockIndex.IsAllValueBiggerThanZero())
		return NULL;

	CDrawingSymbolRow* pRow = m_tree.GetCurBlockDrawingSymbol(blockIndex);	
	if(pRow != NULL)
		return pRow->GetTail();
	return NULL;
*/
}

// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
//void CElementTree::BuildSymbolsFromString(const CString &data)
void CElementTree::BuildSymbolsFromString(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType, const CString &data)
// 2011.02.07 by SYS <<
{
	DeleteAllSymbols();

	if(data.IsEmpty())
		return;

	CElementSaverBuilder bulider;

	// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	//bulider.BuildSymbolsFromString(data, m_tree, m_order); 
	bulider.BuildSymbolsFromString(eScaleType, data, m_tree, m_order); 
	// 2011.02.07 by SYS <<
}

CString CElementTree::GetSavedSymbolsData()
{
	return m_tree.GetSavedSymbolsData(m_order);
}

void CElementTree::DeleteAllSymbols()
{
	m_order.RemoveAll();
	m_tree.DeleteAllSymbols();
}

void CElementTree::DeleteLastSymbol()
{
	CDrawingSymbol* pSymbol = m_order.GetTail();
	if(pSymbol == NULL)
		return;

	DeleteCurrentDrawingSymbol(pSymbol);
}

bool CElementTree::DeleteSymbolPos(const long lAddress,BOOL bListOnly)
{
	return m_tree.DeleteCurrentDrawingSymbol(lAddress,bListOnly);
}

// 현재 있는 element count
int CElementTree::GetElementCount() const
{
	return m_order.GetCount();
}

// element 찾기
void CElementTree::ChangeAllSymbolsColor(const COLORREF& penColor, const COLORREF& textColor)
{
	m_tree.ChangeAllSymbolsColor(penColor, textColor);
}

// block에 있는 element count
void CElementTree::GetElementListOfBlock(const CBlockIndex& blockIndex, CList<CDrawingSymbol*, CDrawingSymbol*>& elementList)
{
	elementList.RemoveAll();
	m_tree.GetElementList(blockIndex, elementList);
}

CBlockIndex CElementTree::GetBlockIndexInSymbol(const CDrawingSymbol* pSelectedSymbol) const
{
	assert(pSelectedSymbol != NULL);
	
	return m_tree.GetBlockIndexInSymbol(pSelectedSymbol);
}

CTree& CElementTree::GetTree()
{
	return m_tree;
}

COrder& CElementTree::GetOrder()
{
	return m_order;
}
