// Tree.cpp: implementation of the CTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "symbol.h"
#include "Tree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "Conversion.h"

#include "DrawingSymbolColumn.h"
#include "DrawingSymbolRow.h"

#include <assert.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTree::~CTree()
{
	DeleteAllSymbols();
}

void CTree::ChangeRowList(const CBlockIndex &sourceBlockIndex, const CBlockIndex &TargetBlockIndex)
{
	POSITION ColPos = m_DrawingSymbolColumnList.GetHeadPosition();
	while(ColPos) {
		CDrawingSymbolColumn* pColumn = m_DrawingSymbolColumnList.GetNext(ColPos);
		pColumn->ChangeRowList(sourceBlockIndex.GetRow(), TargetBlockIndex.GetRow()); 
	}
}

void CTree::SetModifiedBlockIndexs(const CBlockIndex &sourceBlockIndex, const CBlockIndex &TargetBlockIndex)
{
	//블럭이동이 없었을때..
	if(sourceBlockIndex.GetRow() == TargetBlockIndex.GetRow() && sourceBlockIndex.GetColumn() == TargetBlockIndex.GetColumn()) 
		return;

	//블럭이동이 있었을때..
	if(sourceBlockIndex.GetRow() != TargetBlockIndex.GetRow())  //Row끼리 이동.
		ChangeRowList(sourceBlockIndex, TargetBlockIndex);
	else if(sourceBlockIndex.GetColumn() != TargetBlockIndex.GetColumn())  //Column끼리 이동.
		ChangeColumnList(sourceBlockIndex, TargetBlockIndex);
}

void CTree::ChangeColumnList(const CBlockIndex &sourceBlockIndex, const CBlockIndex &TargetBlockIndex)
{
	POSITION targetPos = NULL;
	POSITION SourcePos = NULL;
	CDrawingSymbolColumn* pSource = NULL;
	int nColumn = 0;
	POSITION ColPos = m_DrawingSymbolColumnList.GetHeadPosition();
	while(ColPos) {
		POSITION tmpPos = ColPos;
		CDrawingSymbolColumn* pColumn = m_DrawingSymbolColumnList.GetNext(ColPos);
		if(nColumn == sourceBlockIndex.GetColumn()) {
			pSource = pColumn;
			SourcePos = tmpPos;
		}
		if(nColumn == TargetBlockIndex.GetColumn()) 
			targetPos = tmpPos;
		nColumn++;
	}
	//row에서 insert, delte
	if(targetPos != NULL && SourcePos != NULL) {
		m_DrawingSymbolColumnList.InsertBefore(targetPos, pSource);
		m_DrawingSymbolColumnList.RemoveAt(SourcePos);
	}
}

void CTree::OnDraw(CDC *pDC, IChartManager* pMainBlock, CDrawingSymbol *pSelected)
{
	for(int nColumn = 0 ; nColumn< m_DrawingSymbolColumnList.GetCount() ; nColumn++) {
		CDrawingSymbolColumn* pColumn = m_DrawingSymbolColumnList.GetAt(m_DrawingSymbolColumnList.FindIndex(nColumn));
		pColumn->OnDraw(pDC, pMainBlock, pSelected, nColumn);
	}
}

bool CTree::InsertBlockIndex(const CBlockIndex &insertBlockIndex)
{
	CDrawingSymbolColumn* pColumn = GetCurrentColumn(insertBlockIndex.GetColumn());
	if(!pColumn)
		return false;

	if(!pColumn->InsertRow(insertBlockIndex.GetRow()))
		return false;  

	return true;
}

void CTree::DeleteAllSymbolsInBlock(const CBlockIndex &blockIndex)
{
	CDrawingSymbolColumn* pColumn = GetCurrentColumn(blockIndex.GetColumn());

	if(!pColumn)
		return;
	
	pColumn->DeleteAllSymbolInRow(blockIndex.GetRow());  
}

void CTree::DeleteAllSymbols()
{
	POSITION pos = m_DrawingSymbolColumnList.GetHeadPosition();
	while(pos) {
		CDrawingSymbolColumn* pColumn = m_DrawingSymbolColumnList.GetNext(pos);
		for(int i = 0; i< pColumn->GetCount() ; i++)
 			pColumn->DeleteAllSymbolInRow(i); 
		delete pColumn;
	}
	m_DrawingSymbolColumnList.RemoveAll();
/*
	POSITION ColPos = m_DrawingSymbolColumnList.GetHeadPosition();
	while(ColPos) {
		CDrawingSymbolColumn* pColumn = m_DrawingSymbolColumnList.GetNext(ColPos);
		pColumn->DeleteAllSymbols();
	}
*/
}

CDrawingSymbolColumn* CTree::GetCurrentColumn(const int nColumn)
{

	for(int nColumnIndex = 0; nColumnIndex< m_DrawingSymbolColumnList.GetCount() ; nColumnIndex++) {
		CDrawingSymbolColumn* pColumn = m_DrawingSymbolColumnList.GetAt(m_DrawingSymbolColumnList.FindIndex(nColumnIndex));
		if(nColumnIndex == nColumn) 
			return pColumn;
	}
	return NULL;
}

CDrawingSymbolRow* CTree::GetCurBlockDrawingSymbol(const CBlockIndex& TargetBlockIndex)
{
	for(int nColumn = 0; nColumn< m_DrawingSymbolColumnList.GetCount(); nColumn++) {
		CDrawingSymbolColumn* pColumn = m_DrawingSymbolColumnList.GetAt(m_DrawingSymbolColumnList.FindIndex(nColumn));
		for(int nRow = 0; nRow< pColumn->GetCount(); nRow++) {
			CDrawingSymbolRow* pRow = pColumn->GetAt(nRow);
			if(nRow == TargetBlockIndex.GetRow() && nColumn == TargetBlockIndex.GetColumn())
				return pRow;		
		}
	}
	return NULL;
/*
	POSITION ColPos = m_DrawingSymbolColumnList.GetHeadPosition();
	while(ColPos) {
		CDrawingSymbolColumn* pColumn = m_DrawingSymbolColumnList.GetNext(ColPos);
		POSITION rowPos = pColumn->GetHeadPosition_RowList(); 
		while(rowPos) {
			CDrawingSymbolRow* pRow = pColumn->GetNext_RowList(rowPos);
			if(pRow->GetRow() == TargetBlockIndex.GetRow() && pRow->GetColumn() == TargetBlockIndex.GetColumn())
				return pRow;
		}
	}
	return NULL;
*/
}

CString CTree::GetSavedSymbolsData(const COrder& order)
{
	CString strValue = "";
	POSITION pos = m_DrawingSymbolColumnList.GetHeadPosition();
	while(pos) {
		CDrawingSymbolColumn* pColumn = m_DrawingSymbolColumnList.GetNext(pos);
		strValue += pColumn->GetSavedSymbolsData(order); 
		strValue += "BlockColumnEnd;\r\n";
	}
	return strValue;

/*
	CString strValue = "";
	POSITION pos = m_DrawingSymbolColumnList.GetHeadPosition();
	while(pos) {
		CDrawingSymbolColumn* pColumn = m_DrawingSymbolColumnList.GetNext(pos);
		POSITION ColumnPos = pColumn->GetHeadPosition_RowList();
		while(ColumnPos) {
			CDrawingSymbolRow* pRow = pColumn->GetNext_RowList(ColumnPos); 
			strValue += pRow->GetSavedSymbolsData(); 
			strValue += "BlockEnd;\r\n";			
		}
		strValue += "BlockColumnEnd;\r\n";
	}
	return strValue;
*/
}

bool CTree::DeleteCurrentDrawingSymbol(const CDrawingSymbol *pDrawingSymbol)
{
	POSITION pos = m_DrawingSymbolColumnList.GetHeadPosition();
	while(pos) {
		CDrawingSymbolColumn* pColumn = m_DrawingSymbolColumnList.GetNext(pos);
		return pColumn->DeleteCurrentDrawingSymbol(pDrawingSymbol);
	}
	return false;
}

bool CTree::DeleteCurrentDrawingSymbol(const long lAddressSymbol,BOOL bListOnly)
{
	POSITION pos = m_DrawingSymbolColumnList.GetHeadPosition();
	while(pos) {
		CDrawingSymbolColumn* pColumn = m_DrawingSymbolColumnList.GetNext(pos);
		return pColumn->DeleteCurrentDrawingSymbol(lAddressSymbol,bListOnly);
	}
	return false;
}

void CTree::Add(const CBlockIndex &blockIndex, CDrawingSymbol *pDrawingSymbol)
{
	Add(blockIndex.GetColumn(), blockIndex.GetRow(), pDrawingSymbol);
}

void CTree::AddColumnRowSet(const CBlockIndex &blockIndex)
{
	AddColumn(blockIndex.GetColumn() - m_DrawingSymbolColumnList.GetCount() +1);
	CDrawingSymbolColumn* pColumnSet = m_DrawingSymbolColumnList.GetTail();
	assert(pColumnSet != NULL);
	pColumnSet->AddRowSet();	
}

void CTree::Add(const int nColumn, const int nRow, CDrawingSymbol *pDrawingSymbol)
{
	if(nColumn < m_DrawingSymbolColumnList.GetCount()){
		AddElement(nRow, m_DrawingSymbolColumnList.GetAt(m_DrawingSymbolColumnList.FindIndex(nColumn)), pDrawingSymbol);
		return;
	}

	AddElementInNewMakedColumnSet(CBlockIndex(nRow, nColumn), pDrawingSymbol);
}

void CTree::AddElementInNewMakedColumnSet(const CBlockIndex &blockIndex, CDrawingSymbol *pDrawingSymbol)
{
	AddColumn(blockIndex.GetColumn() - m_DrawingSymbolColumnList.GetCount() +1);
	CDrawingSymbolColumn* pColumn = m_DrawingSymbolColumnList.GetTail();
	assert(pColumn != NULL);
	AddElement(blockIndex.GetRow(), pColumn, pDrawingSymbol);	
}

void CTree::AddElement(const int rowIndex, CDrawingSymbolColumn* pColumn, CDrawingSymbol *pDrawingSymbol)
{
	assert(pColumn != NULL);
	pColumn->Add(rowIndex, pDrawingSymbol);
}

long CTree::CopyElement(CDrawingSymbol *pSelected)
{
	long lAddress = 0;
	long lAddressTemp = 0;
	POSITION ColPos = m_DrawingSymbolColumnList.GetHeadPosition();
	while(ColPos) 
	{
		CDrawingSymbolColumn* pColumn = m_DrawingSymbolColumnList.GetNext(ColPos);
		lAddressTemp = pColumn->CopyElement(pSelected); 
		if(lAddressTemp != 0)
			lAddress = lAddressTemp;
	}

	return lAddress;
}

long CTree::MoveElement(CDrawingSymbol *pSelected)
{
	long lAddress = 0;
	long lAddressTemp = 0;
	POSITION ColPos = m_DrawingSymbolColumnList.GetHeadPosition();
	while(ColPos) 
	{
		CDrawingSymbolColumn* pColumn = m_DrawingSymbolColumnList.GetNext(ColPos);
		lAddressTemp = pColumn->MoveElement(pSelected); 
		if(lAddressTemp != 0)
			lAddress = lAddressTemp;
	}

	return lAddress;
}

void CTree::GetElementList(const CBlockIndex &blockIndex, CList<CDrawingSymbol*, CDrawingSymbol*>& elementList)
{
	CDrawingSymbolColumn* pColumn = GetCurrentColumn(blockIndex.GetColumn());

	if(!pColumn)
		return;
	
	pColumn->GetElementList(blockIndex.GetRow(), elementList);  
}

void CTree::AddColumn(const int nColumnCount)
{
	for(int i = 0; i < nColumnCount; i++)
		m_DrawingSymbolColumnList.AddTail(new CDrawingSymbolColumn());
}

// element 색 변경하기.
void CTree::ChangeAllSymbolsColor(const COLORREF& penColor, const COLORREF& textColor)
{
	POSITION ColPos = m_DrawingSymbolColumnList.GetHeadPosition();
	while(ColPos) 
	{
		CDrawingSymbolColumn* pColumnSet = m_DrawingSymbolColumnList.GetNext(ColPos);
		assert(pColumnSet != NULL);
		pColumnSet->ChangeAllSymbolsColor(penColor, textColor);
	}
}

CBlockIndex CTree::GetBlockIndexInSymbol(const CDrawingSymbol* pSelectedSymbol) const
{
	if(pSelectedSymbol == NULL)
		return CBlockIndex();

	CBlockIndex blockIndex;
	for(int columnIndex = 0; columnIndex < m_DrawingSymbolColumnList.GetCount(); ++columnIndex)
	{
		CDrawingSymbolColumn* pColumnSet = m_DrawingSymbolColumnList.GetAt(m_DrawingSymbolColumnList.FindIndex(columnIndex));
		assert(pColumnSet != NULL);
		blockIndex = pColumnSet->GetBlockIndexInSymbol(pSelectedSymbol, columnIndex);
		if(blockIndex.IsAllValueBiggerThanZero())
			break;
	}
	return blockIndex;
}
