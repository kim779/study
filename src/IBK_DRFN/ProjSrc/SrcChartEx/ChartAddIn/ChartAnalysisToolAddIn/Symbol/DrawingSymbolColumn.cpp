// DrawingSymbolColumn.cpp: implementation of the CDrawingSymbolColumn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "symbol.h"
#include "DrawingSymbolColumn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "DrawingSymbolRow.h"
#include "Region_AllBlock.h"

#include <assert.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDrawingSymbolColumn::~CDrawingSymbolColumn()
{
	DeleteAllSymbols();
/*
	POSITION pos = m_DrawingSymbolRowList.GetHeadPosition();
	while(pos) {
		CDrawingSymbolRow* pRow = m_DrawingSymbolRowList.GetNext(pos);
		pRow->DeleteAllDrawingSymbol();
		delete pRow;
	}
	m_DrawingSymbolRowList.RemoveAll();
*/
}

void CDrawingSymbolColumn::ChangeRowList(const int nSourceRow, const int nTargetRow)
{
	if(nSourceRow >= m_DrawingSymbolRowList.GetCount() && nTargetRow >= m_DrawingSymbolRowList.GetCount())
		return;

	// sourceRowIndex가 m_RowSetList의 내용이 아닐 경우
	// -> 새로운 RowSet을 insert한다.
	int nMaxIndex = GetMax(nSourceRow, nTargetRow);
	if(nMaxIndex > (m_DrawingSymbolRowList.GetCount() - 1)){
		for(int i = m_DrawingSymbolRowList.GetCount() ; i <= nMaxIndex ; i++)
			m_DrawingSymbolRowList.AddTail(new CDrawingSymbolRow());
	}
	ChangeList(nSourceRow, nTargetRow);
}

void CDrawingSymbolColumn::OnDraw(CDC *pDC, IChartManager* pMainBlock, CDrawingSymbol *pSelected, const int nColumnIndex)
{
	CRegion_AllBlock drawingRegions;
	drawingRegions.SetDrawingRegions(pMainBlock);

	for(int nRow = 0 ; nRow< m_DrawingSymbolRowList.GetCount() ; nRow++) {
		CDrawingSymbolRow* pRow = m_DrawingSymbolRowList.GetAt(m_DrawingSymbolRowList.FindIndex(nRow));
		pRow->OnDraw(pDC, pMainBlock, pSelected, nColumnIndex, nRow, drawingRegions);
	}
}

long CDrawingSymbolColumn::CopyElement(CDrawingSymbol *pSelected)
{
	long lAddress = 0;
	long lAddressTemp = 0;
	for(int nRow = 0 ; nRow< m_DrawingSymbolRowList.GetCount() ; nRow++) {
		CDrawingSymbolRow* pRow = m_DrawingSymbolRowList.GetAt(m_DrawingSymbolRowList.FindIndex(nRow));
		lAddressTemp = pRow->CopyElement(pSelected);
		if(lAddressTemp != 0)
			lAddress = lAddressTemp;
	}

	return lAddress;
}

long CDrawingSymbolColumn::MoveElement(CDrawingSymbol *pSelected)
{
	long lAddress = 0;
	long lAddressTemp = 0;
	for(int nRow = 0 ; nRow< m_DrawingSymbolRowList.GetCount() ; nRow++) {
		CDrawingSymbolRow* pRow = m_DrawingSymbolRowList.GetAt(m_DrawingSymbolRowList.FindIndex(nRow));
		lAddressTemp = pRow->MoveElement(pSelected);
		if(lAddressTemp != 0)
			lAddress = lAddressTemp;
	}

	return lAddress;
}

bool CDrawingSymbolColumn::InsertRow(const int ninsertRowIndex)
{
	int nRow = 0;
	POSITION rowPos = m_DrawingSymbolRowList.GetHeadPosition();
	while(rowPos) {
		POSITION tmpPos = rowPos;
		CDrawingSymbolRow* pRow = m_DrawingSymbolRowList.GetNext(rowPos);
		if(nRow == ninsertRowIndex) {
			//row에서 insert
			m_DrawingSymbolRowList.InsertBefore(tmpPos, new CDrawingSymbolRow());
			return true;
		}
		nRow++;
	}
	return false;
}

void CDrawingSymbolColumn::DeleteAllSymbolInRow(const int nRow)
{
	int nRowIndex = 0 ; 
	POSITION rowPos = m_DrawingSymbolRowList.GetHeadPosition();
	while(rowPos) {
		POSITION prepos = rowPos;
		CDrawingSymbolRow* pRow = m_DrawingSymbolRowList.GetNext(rowPos);
		if(nRowIndex == nRow) {
			pRow->DeleteAllDrawingSymbol();
			delete pRow;
			m_DrawingSymbolRowList.RemoveAt(prepos);
		}
		nRowIndex++;
	}
}

void CDrawingSymbolColumn::DeleteAllSymbols()
{
	POSITION pos = m_DrawingSymbolRowList.GetHeadPosition();
	while(pos) {
		CDrawingSymbolRow* pRow = m_DrawingSymbolRowList.GetNext(pos);
		pRow->DeleteAllDrawingSymbol();
		delete pRow;
	}
	m_DrawingSymbolRowList.RemoveAll();
/*
	POSITION rowPos = m_DrawingSymbolRowList.GetHeadPosition();
	while(rowPos) {
		CDrawingSymbolRow* pRow =  m_DrawingSymbolRowList.GetNext(rowPos);
		pRow->DeleteAllDrawingSymbol();
	}
*/
}

int CDrawingSymbolColumn::GetCount()
{
	return m_DrawingSymbolRowList.GetCount();
}

bool CDrawingSymbolColumn::DeleteCurrentDrawingSymbol(const CDrawingSymbol *pDrawingSymbol)
{
	bool bResult = false;
	POSITION rowPos = m_DrawingSymbolRowList.GetHeadPosition();
	while(rowPos) {
		CDrawingSymbolRow* pRow =  m_DrawingSymbolRowList.GetNext(rowPos);
		bResult = pRow->DeleteCurrentDrawingSymbol(pDrawingSymbol);
		if(bResult == true)
			break;
	}
	return bResult;
}

bool CDrawingSymbolColumn::DeleteCurrentDrawingSymbol(const long lAddressSymbol,BOOL bListOnly)
{
	bool bResult = false;
	POSITION rowPos = m_DrawingSymbolRowList.GetHeadPosition();
	while(rowPos) {
		CDrawingSymbolRow* pRow =  m_DrawingSymbolRowList.GetNext(rowPos);
		bResult = pRow->DeleteCurrentDrawingSymbol(lAddressSymbol,bListOnly);
		if(bResult == true)
			break;
	}
	return bResult;
}

CDrawingSymbolRow* CDrawingSymbolColumn::GetAt(const int nIndex)
{
	if(nIndex < 0)
		return NULL;

	return m_DrawingSymbolRowList.GetAt(m_DrawingSymbolRowList.FindIndex(nIndex));
}

void CDrawingSymbolColumn::Add(const int rowIndex, CDrawingSymbol *pDrawingSymbol)
{
	if(rowIndex < m_DrawingSymbolRowList.GetCount()){
		AddElement(m_DrawingSymbolRowList.GetAt(m_DrawingSymbolRowList.FindIndex(rowIndex)), pDrawingSymbol);
		return;
	}

	AddElementInNewMakedRowSet(rowIndex, pDrawingSymbol);
}

void CDrawingSymbolColumn::AddElementInNewMakedRowSet(const int rowIndex, CDrawingSymbol *pDrawingSymbol)
{
	AddRow(rowIndex - m_DrawingSymbolRowList.GetCount() +1);
	CDrawingSymbolRow* pRow = m_DrawingSymbolRowList.GetTail();
	assert(pRow != NULL);
	AddElement(pRow, pDrawingSymbol);	
}

void CDrawingSymbolColumn::AddElement(CDrawingSymbolRow* pRow, CDrawingSymbol *pDrawingSymbol)
{
	assert(pRow != NULL);
	pRow->Add(pDrawingSymbol);
}

void CDrawingSymbolColumn::GetElementList(const int nRowIndex, CList<CDrawingSymbol*, CDrawingSymbol*>& elementList)
{
	for(int nRow = 0 ; nRow < m_DrawingSymbolRowList.GetCount() ; nRow++) {
		if(nRowIndex == nRow) {
			CDrawingSymbolRow* pRow = m_DrawingSymbolRowList.GetAt(m_DrawingSymbolRowList.FindIndex(nRow));
			pRow->GetElementList(elementList);
		}
	}
}

// block 추가시 elementTree을 추가하기.
bool CDrawingSymbolColumn::AddRowSet()
{	
	m_DrawingSymbolRowList.AddTail(new CDrawingSymbolRow());
	return true;
}

CString CDrawingSymbolColumn::GetSavedSymbolsData(const COrder &order)
{
	CString strValue = "";
	POSITION Pos = m_DrawingSymbolRowList.GetHeadPosition();
	while(Pos) {
		CDrawingSymbolRow* pRow = m_DrawingSymbolRowList.GetNext(Pos); 
		strValue += pRow->GetSavedSymbolsData(order); 
		strValue += "BlockEnd;\r\n";			
	}
	return strValue;
}

void CDrawingSymbolColumn::AddRow(const int nRowCount)
{
	for(int i = 0; i < nRowCount; i++)
		m_DrawingSymbolRowList.AddTail(new CDrawingSymbolRow());
}

void CDrawingSymbolColumn::ChangeList(const int nSourceRow, const int nTargetRow)
{
	POSITION targetPos = NULL;
	POSITION SourcePos = NULL;
	CDrawingSymbolRow* pSource = NULL;
	int nRow = 0;
	POSITION rowPos = m_DrawingSymbolRowList.GetHeadPosition();
	while(rowPos) {
		POSITION tmpPos = rowPos;
		CDrawingSymbolRow* pRow = m_DrawingSymbolRowList.GetNext(rowPos);
		if(nRow == nSourceRow) {
			pSource = pRow;
			SourcePos = tmpPos;
		}
		if(nRow == nTargetRow) 
			targetPos = tmpPos;

		nRow++;
	}
	//row에서 insert, delte
	if(targetPos != NULL && SourcePos != NULL) {
		if(nSourceRow < nTargetRow)
			m_DrawingSymbolRowList.InsertAfter(targetPos, pSource);
		else if(nSourceRow > nTargetRow)
			m_DrawingSymbolRowList.InsertBefore(targetPos, pSource);

		m_DrawingSymbolRowList.RemoveAt(SourcePos);
	}
}

int CDrawingSymbolColumn::GetMax(const int nSourceRow, const int nTargetRow)
{
	return (nSourceRow > nTargetRow) ? nSourceRow: nTargetRow;	
}

void CDrawingSymbolColumn::ChangeAllSymbolsColor(const COLORREF& penColor, const COLORREF& textColor)
{
	POSITION Pos = m_DrawingSymbolRowList.GetHeadPosition();
	while(Pos) {
		CDrawingSymbolRow* pRow = m_DrawingSymbolRowList.GetNext(Pos); 
		assert(pRow != NULL);
		pRow->ChangeAllSymbolsColor(penColor,textColor);
	}
}

CBlockIndex CDrawingSymbolColumn::GetBlockIndexInSymbol(const CDrawingSymbol* pSelectedSymbol, const int columnIndex) const
{
	if(pSelectedSymbol == NULL)
		return CBlockIndex();

	CBlockIndex blockIndex;
	for(int rowIndex = 0; rowIndex < m_DrawingSymbolRowList.GetCount(); ++rowIndex)
	{
		CDrawingSymbolRow* pRowSet = m_DrawingSymbolRowList.GetAt(m_DrawingSymbolRowList.FindIndex(rowIndex));
		assert(pRowSet != NULL);
		if(pRowSet->IsInSameSymbol(pSelectedSymbol))
			return CBlockIndex( rowIndex, columnIndex);
	}
	return CBlockIndex();
}
