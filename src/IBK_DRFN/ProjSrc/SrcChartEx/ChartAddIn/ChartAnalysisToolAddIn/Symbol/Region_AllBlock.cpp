// Region_AllBlock.cpp: implementation of the CRegion_AllBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "symbol.h"
#include "Region_AllBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "../Include_Analysis/CoordinateCalculatorData.h"
#include "Region_Column.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegion_AllBlock::CRegion_AllBlock()
{

}

CRegion_AllBlock::~CRegion_AllBlock()
{
	DeleteColumnBlockList();
}

void CRegion_AllBlock::MakeColumnList(const int nColumnCount, const int nRowCount)
{
	for(int nColumn = 0 ; nColumn < nColumnCount ; nColumn++)
		m_ColumnBlockRegionList.AddTail(new CRegion_Column(nColumn, nRowCount));
}

void CRegion_AllBlock::SetBlockRegion(const int nColumnIndex, const int nRowIndex, const CRect &blockregion)
{
	POSITION pos = m_ColumnBlockRegionList.GetHeadPosition();
	while(pos) {
		CRegion_Column* pColumnRegion = m_ColumnBlockRegionList.GetNext(pos);
		if(pColumnRegion->GetColumnIndex()  == nColumnIndex)
			pColumnRegion->SetBlockRegion(nRowIndex, blockregion); 
	}
}

CRect CRegion_AllBlock::GetBlockRegion(const int nColumnIndex, const int nRowIndex) const
{
	POSITION pos = m_ColumnBlockRegionList.GetHeadPosition();
	while(pos) {
		CRegion_Column* pColumnRegion = m_ColumnBlockRegionList.GetNext(pos);
		if(pColumnRegion->GetColumnIndex()  == nColumnIndex)
			return pColumnRegion->GetBlockRegion(nRowIndex); 
	}
	return CRect(0,0,0,0);
}

bool CRegion_AllBlock::InsertBlockIndex(const CBlockIndex &blockIndex)
{
	POSITION pos = m_ColumnBlockRegionList.GetHeadPosition();
	while(pos) {
		CRegion_Column* pColumnRegion = m_ColumnBlockRegionList.GetNext(pos);
		if(pColumnRegion->GetColumnIndex()  == blockIndex.GetColumn()) {
			pColumnRegion->InsertRow(blockIndex.GetRow()); 
			return true;
		}
	}
	return false;
}

void CRegion_AllBlock::DeleteColumnBlockList()
{
	POSITION pos = m_ColumnBlockRegionList.GetHeadPosition();
	while(pos) {
		CRegion_Column* pColumnRegion = m_ColumnBlockRegionList.GetNext(pos);
		delete pColumnRegion;
	}
	m_ColumnBlockRegionList.RemoveAll();

}
/*
void CRegion_AllBlock::GetCoordinateCalculatorData(CBlockIndex blockIndex, CCoordinateCalculatorData &coordinateData)
{
	if(m_pMainBlock == NULL)
		return;
	
	m_pMainBlock->GetCoordinateCalculatorData(blockIndex, coordinateData);
}
*/
void CRegion_AllBlock::Initialize()
{
	DeleteColumnBlockList();
}

void CRegion_AllBlock::SetAllBlockRegion(IChartManager* pMainBlock)
{
	if(pMainBlock == NULL)
		return;

	for(int nColumn = 0 ; nColumn < pMainBlock->GetColumnCount() ; nColumn++) {
		for(int nRow = 0 ; nRow < pMainBlock->GetRowCount() ; nRow++)
		{
			IBlock *pIBlock = pMainBlock->GetBlock( nRow, nColumn);
			if( !pIBlock) continue;
			CRect rctNoFrame, rctNoFrameAndMargin;
			pIBlock->GetGraphRegions( rctNoFrame, rctNoFrameAndMargin);
			pIBlock->Release();
			pIBlock = NULL;

			SetBlockRegion(nColumn, nRow, rctNoFrame);  
		}
	}
}

void CRegion_AllBlock::SetDrawingRegions(IChartManager* pMainBlock)
{
	Initialize();
	MakeColumnList(pMainBlock->GetColumnCount(), pMainBlock->GetRowCount());
	SetAllBlockRegion(pMainBlock);
}

