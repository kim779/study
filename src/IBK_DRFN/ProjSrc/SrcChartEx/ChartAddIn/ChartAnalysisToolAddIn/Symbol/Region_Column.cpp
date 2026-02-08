// Region_Column.cpp: implementation of the CRegion_Column class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "symbol.h"
#include "Region_Column.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "Region_Row.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegion_Column::CRegion_Column(const int nColumnIndex, const int nRowCount)
{
	m_nColumn = nColumnIndex;
	MakeRowList(nRowCount);
}

CRegion_Column::~CRegion_Column()
{
	POSITION pos = m_RowRegionList.GetHeadPosition();
	while(pos) {
		CRegion_Row* pRowRegion = m_RowRegionList.GetNext(pos);
		delete pRowRegion;
	}
	m_RowRegionList.RemoveAll();
}


void CRegion_Column::MakeRowList(const int nRowCount)
{
	for(int nRow = 0 ; nRow < nRowCount ; nRow++)
		m_RowRegionList.AddTail(new CRegion_Row(nRow));
}

void CRegion_Column::SetBlockRegion(const int nRowIndex, const CRect &blockregion)
{
	POSITION pos = m_RowRegionList.GetHeadPosition();
	while(pos) {
		CRegion_Row* pRowRegion = m_RowRegionList.GetNext(pos);
		if(pRowRegion->GetRowIndex()  == nRowIndex)
			pRowRegion->SetBlockRegion(blockregion); 
	}
}

CRect CRegion_Column::GetBlockRegion(const int nRowIndex) const
{
	POSITION pos = m_RowRegionList.GetHeadPosition();
	while(pos) {
		CRegion_Row* pRowRegion = m_RowRegionList.GetNext(pos);
		if(pRowRegion->GetRowIndex()  == nRowIndex)
			return pRowRegion->GetBlockRegion(); 
	}
	return CRect(0,0,0,0);
}

int CRegion_Column::GetColumnIndex()
{
	return m_nColumn;
}

bool CRegion_Column::InsertRow(const int nRow)
{
	bool bResult = false;
	POSITION rowPos = m_RowRegionList.GetHeadPosition();
	while(rowPos) {
		POSITION tmpPos = rowPos;
		CRegion_Row* pRow = m_RowRegionList.GetNext(rowPos);
		if(pRow->GetRowIndex() == nRow) {
			//row¿¡¼­ insert
			m_RowRegionList.InsertBefore(tmpPos, new CRegion_Row(nRow));
			bResult =  true;
		}
	}
	SetModifiedRowIndexs();
	return bResult;
}

void CRegion_Column::SetModifiedRowIndexs()
{
	POSITION rowPos = m_RowRegionList.GetHeadPosition();
	int nRowIndex = 0;
	while(rowPos) {
		CRegion_Row* pRow = m_RowRegionList.GetNext(rowPos);
		pRow->SetRowIndex(nRowIndex);
		nRowIndex++;
	}
}