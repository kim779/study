// IWChartManager.cpp: implementation of the CIWChartManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWChartManager.h"

#include "IWBlock.h"			// for CIWBlock

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_IMP
//////////////////////////////////////////////////////////////////////
_IWRAPPER_IUNKNOWN_MANAGER_IMP( CIWChartManager)

//////////////////////////////////////////////////////////////////////
// (2006/6/25 - Seung-Won, Bae) Partition Info
//////////////////////////////////////////////////////////////////////

// (2006/6/13 - Seung-Won, Bae) Query Block Type.
CBlockBaseData::BLOCKTYPE CIWChartManager::GetBlockType( void) const
{
	if( !m_pMainBlock) return CBlockBaseData::ROW;
	return m_pMainBlock->GetBlockType();
}

// (2006/6/25 - Seung-Won, Bae) Block Counts
int CIWChartManager::GetRowCount( void)
{
	if( m_pMainBlock) return m_pMainBlock->GetRowCount();
	return 0;
}
int CIWChartManager::GetColumnCount( void)
{
	if( m_pMainBlock) return m_pMainBlock->GetColumnCount();
	return 0;
}

// (2006/6/13 - Seung-Won, Bae) Get Row Count
int CIWChartManager::GetRowCountInColumn( int p_nColumnIndex)
{
	if( !m_pMainBlock) return -1;
	return m_pMainBlock->GetRowCountInColumn( p_nColumnIndex);
}

// Design Time String Info
BOOL CIWChartManager::GetBlocksPosition( CString &p_strBlockPosition)
{
	if( !m_pMainBlock) return FALSE;
	p_strBlockPosition = m_pMainBlock->GetBlocksPosition();
	return !p_strBlockPosition.IsEmpty();
}

// (2006/2/13 - Seung-Won, Bae) Check! Is Empty?
BOOL CIWChartManager::IsEmpty( void)
{
	if( !m_pMainBlock) return TRUE;
	return m_pMainBlock->IsEmpty();
}

//////////////////////////////////////////////////////////////////////
// (2006/6/14 - Seung-Won, Bae) Get Block Interface
//////////////////////////////////////////////////////////////////////

IBlock *CIWChartManager::GetBlock( int p_nRowIndex, int p_nColumnIndex)
{
	if( !m_pMainBlock) return NULL;

	CBlock *pBlock = m_pMainBlock->GetBlock( CBlockIndex( p_nRowIndex, p_nColumnIndex));
	if( !pBlock) return NULL;

	return CIWBlock::CreateObject( pBlock);
}


//////////////////////////////////////////////////////////////////////
// (2006/6/25 - Seung-Won, Bae) View Info
//////////////////////////////////////////////////////////////////////

// (2006/6/17 - Seung-Won, Bae) Get Background Color
COLORREF CIWChartManager::GetBackgroundColor( void)
{
	if( !m_pMainBlock) return RGB( 0, 0, 0);
	return m_pMainBlock->GetBackgroundColor();
}


//////////////////////////////////////////////////////////////////////
// (2006/6/25 - Seung-Won, Bae) Scale Info
//////////////////////////////////////////////////////////////////////

// (2006/6/13 - Seung-Won, Bae) Retrieve the Data Index Range in Scroll & Zoom View
BOOL CIWChartManager::GetDataIndexRangeInView( int &p_nStartIndex, int &p_nEndIndex)
{
	if( !m_pMainBlock) return FALSE;
	m_pMainBlock->GetDisplayDataIndex( p_nStartIndex, p_nEndIndex);
	return TRUE;
}

// (2006/6/25 - Seung-Won, Bae) Change Scale Type
void CIWChartManager::SetAllVertScalePosition( const CScaleBaseData::VERTSCALEPOSITION ePosition)
{
	if( m_pMainBlock) m_pMainBlock->SetAllVertScalePosition( ePosition);
}


//////////////////////////////////////////////////////////////////////
// (2006/6/25 - Seung-Won, Bae) Graph Info
//////////////////////////////////////////////////////////////////////

// [04/10/26] 지정되는 Graph의 Data Packet Name을 조회하는 Interface를 제공한다.
BOOL CIWChartManager::GetPacketNameOfGraph( const char *p_szGraphName, CString &p_strPacketNames)
{
	if( !m_pMainBlock) return FALSE;
	p_strPacketNames = m_pMainBlock->GetGraphPacketName( p_szGraphName);
	return !p_strPacketNames.IsEmpty();
}

// [04/10/27] 특정 Graph가 등록된 Block, ScaleGroup, Graph의 Index를 조회하는 Interface를 제공한다.
BOOL CIWChartManager::GetIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG)
{
	if( !m_pMainBlock) return FALSE;
	return m_pMainBlock->GetIndexOfGraph( p_szGraphName, p_nR, p_nC, p_nV, p_nG);
}

// (2006/6/27 - Seung-Won, Bae) Retrieve the all Graph Names
BOOL CIWChartManager::GetAllGraphName( CString &p_strGraphNames)
{
	if( !m_pMainBlock) return FALSE;
	CList< CString, CString> strGraphNameList;
	if( !m_pMainBlock->GetAllIndicatorName( strGraphNameList)) return NULL;
	if( strGraphNameList.IsEmpty()) return NULL;
	POSITION posGraphName = strGraphNameList.GetHeadPosition();
	while( posGraphName) p_strGraphNames += strGraphNameList.GetNext( posGraphName) + ",";
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// (2006/6/25 - Seung-Won, Bae) Mouse Position Info
//////////////////////////////////////////////////////////////////////

// (2006/2/6 - Seung-Won, Bae) Get Full Column Region for DragZoom
CRect CIWChartManager::GetGraphRegionSumInColumn( const CPoint &p_ptParam) const
{
	if( !m_pMainBlock) return CRect( 0, 0, 0, 0);
	return m_pMainBlock->GetGraphRegionSumInColumn( p_ptParam);
}

// (2006/2/6 - Seung-Won, Bae) Get Full Column Region for DragZoom
BOOL CIWChartManager::GetBlockIndex( CPoint p_ptPoint, int &p_nRow, int &p_nColumn)
{
	if( !m_pMainBlock) return FALSE;
	CBlockIndex bi = m_pMainBlock->FindBlockIndex( p_ptPoint);
	p_nRow = bi.GetRow();
	p_nColumn = bi.GetColumn();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2006/3/24 - Seung-Won, Bae) Sub Graph View Data Iterator
//////////////////////////////////////////////////////////////////////
POSITION CIWChartManager::GetSubGraphViewData_First( void)
{
	if( !m_pMainBlock) return NULL;

	CSubGraphViewDataIterator sgvdIterator;
	if( !m_pMainBlock->GetViewDataListInAllBlock( NULL, *sgvdIterator.m_pViewData)) return NULL;

	CList<CColumnViewData, CColumnViewData&> &columnViewDataList = sgvdIterator.m_pViewData->GetColumnViewDataList();
	sgvdIterator.m_pColumnViewDataList	= &columnViewDataList;

	GetSubGraphViewData_GetFirstColumnViewDataPosition( &sgvdIterator);
	if( !sgvdIterator.m_posColumnViewData) return NULL;

	GetSubGraphViewData_GetFirstRowViewDataPosition( &sgvdIterator);
	if( !sgvdIterator.m_posRowViewData) return NULL;

	GetSubGraphViewData_GetFirstSubGraphViewDataPosition( &sgvdIterator);
	if( !sgvdIterator.m_posSubGraphViewData) return NULL;

	m_lSubGraphViewData.AddTail( new CSubGraphViewDataIterator( sgvdIterator));
	return m_lSubGraphViewData.GetTailPosition();
}
void CIWChartManager::GetSubGraphViewData_Next( POSITION &p_psnViewData)
{
	if( !p_psnViewData) return;
	CSubGraphViewDataIterator *pSGVDIterator = m_lSubGraphViewData.GetAt( p_psnViewData);
	if( !pSGVDIterator) return;

	GetSubGraphViewData_GetNextSubGraphViewDataPosition( pSGVDIterator);
	if( pSGVDIterator->m_posSubGraphViewData) return;

	GetSubGraphViewData_GetNextRowViewDataPosition( pSGVDIterator);
	while( pSGVDIterator->m_posRowViewData)
	{
		GetSubGraphViewData_GetFirstSubGraphViewDataPosition( pSGVDIterator);
		if( pSGVDIterator->m_posSubGraphViewData) return;
		GetSubGraphViewData_GetNextRowViewDataPosition( pSGVDIterator);
	}

	GetSubGraphViewData_GetNextColumnViewDataPosition( pSGVDIterator);
	while( pSGVDIterator->m_posColumnViewData)
	{
		GetSubGraphViewData_GetFirstRowViewDataPosition( pSGVDIterator);
		while( pSGVDIterator->m_posRowViewData)
		{
			GetSubGraphViewData_GetFirstSubGraphViewDataPosition( pSGVDIterator);
			if( pSGVDIterator->m_posSubGraphViewData) return;
			GetSubGraphViewData_GetNextRowViewDataPosition( pSGVDIterator);
		}
	}

	GetSubGraphViewData_Close( p_psnViewData);
}
void CIWChartManager::GetSubGraphViewData_NextInColumn( POSITION &p_psnViewData)
{
	if( !p_psnViewData) return;
	CSubGraphViewDataIterator *pSGVDIterator = m_lSubGraphViewData.GetAt( p_psnViewData);
	if( !pSGVDIterator) return;

	GetSubGraphViewData_GetNextSubGraphViewDataPosition( pSGVDIterator);
	if( pSGVDIterator->m_posSubGraphViewData) return;

	GetSubGraphViewData_GetNextRowViewDataPosition( pSGVDIterator);
	while( pSGVDIterator->m_posRowViewData)
	{
		GetSubGraphViewData_GetFirstSubGraphViewDataPosition( pSGVDIterator);
		if( pSGVDIterator->m_posSubGraphViewData) return;
		GetSubGraphViewData_GetNextRowViewDataPosition( pSGVDIterator);
	}

	GetSubGraphViewData_Close( p_psnViewData);
}
void CIWChartManager::GetSubGraphViewData_NextInRow( POSITION &p_psnViewData)
{
	if( !p_psnViewData) return;
	CSubGraphViewDataIterator *pSGVDIterator = m_lSubGraphViewData.GetAt( p_psnViewData);
	if( !pSGVDIterator) return;

	GetSubGraphViewData_GetNextSubGraphViewDataPosition( pSGVDIterator);
	if( pSGVDIterator->m_posSubGraphViewData) return;

	GetSubGraphViewData_Close( p_psnViewData);
}
BOOL CIWChartManager::GetSubGraphViewData_Index( POSITION &p_psnViewData, int p_nRow, int p_nColumn)
{
	if( !p_psnViewData) return FALSE;
	CSubGraphViewDataIterator *pSGVDIterator = m_lSubGraphViewData.GetAt( p_psnViewData);
	if( !pSGVDIterator) return FALSE;

	GetSubGraphViewData_GetIndexColumnViewDataPosition( pSGVDIterator, p_nColumn);
	if( !pSGVDIterator->m_posColumnViewData) return FALSE;

	GetSubGraphViewData_GetIndexRowViewDataPosition( pSGVDIterator, p_nRow);
	if( !pSGVDIterator->m_posRowViewData) return FALSE;

	GetSubGraphViewData_GetFirstSubGraphViewDataPosition( pSGVDIterator);
	if( !pSGVDIterator->m_posSubGraphViewData) return FALSE;

	return TRUE;
}
void CIWChartManager::GetSubGraphViewData_Close( POSITION &p_psnViewData)
{
	if( !p_psnViewData) return;
	CSubGraphViewDataIterator *pSGVDIterator = m_lSubGraphViewData.GetAt( p_psnViewData);
	if( !pSGVDIterator) return;

	m_lSubGraphViewData.RemoveAt( p_psnViewData);
	delete pSGVDIterator;
	p_psnViewData = NULL;
}
void CIWChartManager::GetSubGraphViewData_HorzScaleName( POSITION p_psnViewData, CString &p_strHorzScaleName)
{
	p_strHorzScaleName.Empty();

	if( !p_psnViewData) return;
	CSubGraphViewDataIterator *pSGVDIterator = m_lSubGraphViewData.GetAt( p_psnViewData);
	if( !pSGVDIterator)						return;
	if( !pSGVDIterator->m_pRowViewData)		return;

	p_strHorzScaleName = pSGVDIterator->m_pRowViewData->GetHorzScaleName();
}
void CIWChartManager::GetSubGraphViewData_GraphRegion( POSITION p_psnViewData, CRect &p_rctGraphRegion)
{
	p_rctGraphRegion.SetRectEmpty();

	if( !p_psnViewData) return;
	CSubGraphViewDataIterator *pSGVDIterator = m_lSubGraphViewData.GetAt( p_psnViewData);
	if( !pSGVDIterator)						return;
	if( !pSGVDIterator->m_pRowViewData)		return;

	p_rctGraphRegion = pSGVDIterator->m_pRowViewData->GetDrawingGraphRegion();
}
void CIWChartManager::GetSubGraphViewData_Title( POSITION p_psnViewData, CString &p_strTitle)
{
	p_strTitle.Empty();

	if( !p_psnViewData) return;
	CSubGraphViewDataIterator *pSGVDIterator = m_lSubGraphViewData.GetAt( p_psnViewData);
	if( !pSGVDIterator)								return;
	if( !pSGVDIterator->m_pSubGraphViewData)		return;

	p_strTitle = pSGVDIterator->m_pSubGraphViewData->GetTitle();
}
void CIWChartManager::GetSubGraphViewData_TitleAndPacketNames( POSITION p_psnViewData, CString &p_strTitle, CString &p_strPacketNames)
{
	p_strTitle.Empty();
	p_strPacketNames.Empty();

	if( !p_psnViewData) return;
	CSubGraphViewDataIterator *pSGVDIterator = m_lSubGraphViewData.GetAt( p_psnViewData);
	if( !pSGVDIterator)								return;
	if( !pSGVDIterator->m_pSubGraphViewData)		return;

	p_strTitle = pSGVDIterator->m_pSubGraphViewData->GetTitle();
	p_strPacketNames = pSGVDIterator->m_pSubGraphViewData->GetPacketNames();
}
void CIWChartManager::GetSubGraphViewData_GetFirstColumnViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator)
{
	if( !p_pSGVDIterator) return;
	p_pSGVDIterator->m_posColumnViewData	= NULL;
	p_pSGVDIterator->m_pColumnViewData		= NULL;
	p_pSGVDIterator->m_pRowViewDataList		= NULL;

	if( !p_pSGVDIterator->m_pColumnViewDataList) return;
	p_pSGVDIterator->m_posColumnViewData	= p_pSGVDIterator->m_pColumnViewDataList->GetHeadPosition();
	if( !p_pSGVDIterator->m_posColumnViewData)		return;

	CColumnViewData &columnViewData = p_pSGVDIterator->m_pColumnViewDataList->GetAt( p_pSGVDIterator->m_posColumnViewData);
	p_pSGVDIterator->m_pColumnViewData		= &columnViewData;
	CList<CRowViewData, CRowViewData&>&rowViewDataList = columnViewData.GetRowViewDataList();
	p_pSGVDIterator->m_pRowViewDataList		= &rowViewDataList;
}
void CIWChartManager::GetSubGraphViewData_GetIndexColumnViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator, int p_nColumn)
{
	if( !p_pSGVDIterator) return;
	p_pSGVDIterator->m_posColumnViewData	= NULL;
	p_pSGVDIterator->m_pColumnViewData		= NULL;
	p_pSGVDIterator->m_pRowViewDataList		= NULL;

	if( !p_pSGVDIterator->m_pColumnViewDataList) return;
	if( p_nColumn < 0 || p_pSGVDIterator->m_pColumnViewDataList->GetCount() <= p_nColumn) return;
	p_pSGVDIterator->m_posColumnViewData	= p_pSGVDIterator->m_pColumnViewDataList->FindIndex( p_nColumn);
	if( !p_pSGVDIterator->m_posColumnViewData)		return;

	CColumnViewData &columnViewData = p_pSGVDIterator->m_pColumnViewDataList->GetAt( p_pSGVDIterator->m_posColumnViewData);
	p_pSGVDIterator->m_pColumnViewData		= &columnViewData;
	CList<CRowViewData, CRowViewData&>&rowViewDataList = columnViewData.GetRowViewDataList();
	p_pSGVDIterator->m_pRowViewDataList		= &rowViewDataList;
}
void CIWChartManager::GetSubGraphViewData_GetNextColumnViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator)
{
	if( !p_pSGVDIterator)							return;
	p_pSGVDIterator->m_pColumnViewData	= NULL;
	p_pSGVDIterator->m_pRowViewDataList	= NULL;

	if( !p_pSGVDIterator->m_pColumnViewDataList)	return;
	if( !p_pSGVDIterator->m_posColumnViewData)		return;
	p_pSGVDIterator->m_pColumnViewDataList->GetNext( p_pSGVDIterator->m_posColumnViewData);
	if( !p_pSGVDIterator->m_posColumnViewData)		return;

	CColumnViewData &columnViewData = p_pSGVDIterator->m_pColumnViewDataList->GetAt( p_pSGVDIterator->m_posColumnViewData);
	p_pSGVDIterator->m_pColumnViewData		= &columnViewData;
	CList<CRowViewData, CRowViewData&>&rowViewDataList = columnViewData.GetRowViewDataList();
	p_pSGVDIterator->m_pRowViewDataList		= &rowViewDataList;
}
void CIWChartManager::GetSubGraphViewData_GetFirstRowViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator)
{
	if( !p_pSGVDIterator) return;
	p_pSGVDIterator->m_posRowViewData			= NULL;
	p_pSGVDIterator->m_pRowViewData				= NULL;
	p_pSGVDIterator->m_pSubGraphViewDataList	= NULL;

	if( !p_pSGVDIterator->m_pRowViewDataList)	return;
	p_pSGVDIterator->m_posRowViewData	= p_pSGVDIterator->m_pRowViewDataList->GetHeadPosition();
	if( !p_pSGVDIterator->m_posRowViewData)			return;

	CRowViewData &rowViewData = p_pSGVDIterator->m_pRowViewDataList->GetAt( p_pSGVDIterator->m_posRowViewData);
	p_pSGVDIterator->m_pRowViewData = &rowViewData;
	CList<CSubGraphViewData, CSubGraphViewData&> &subGraphViewDataList = rowViewData.GetSubGraphViewDataList().GetSubGraphViewDataList();
	p_pSGVDIterator->m_pSubGraphViewDataList = &subGraphViewDataList;
}
void CIWChartManager::GetSubGraphViewData_GetIndexRowViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator, int p_nRow)
{
	if( !p_pSGVDIterator) return;
	p_pSGVDIterator->m_posRowViewData			= NULL;
	p_pSGVDIterator->m_pRowViewData				= NULL;
	p_pSGVDIterator->m_pSubGraphViewDataList	= NULL;

	if( !p_pSGVDIterator->m_pRowViewDataList)	return;
	if( p_nRow < 0 || p_pSGVDIterator->m_pRowViewDataList->GetCount() <= p_nRow) return;
	p_pSGVDIterator->m_posRowViewData	= p_pSGVDIterator->m_pRowViewDataList->FindIndex( p_nRow);
	if( !p_pSGVDIterator->m_posRowViewData)			return;

	CRowViewData &rowViewData = p_pSGVDIterator->m_pRowViewDataList->GetAt( p_pSGVDIterator->m_posRowViewData);
	p_pSGVDIterator->m_pRowViewData = &rowViewData;
	CList<CSubGraphViewData, CSubGraphViewData&> &subGraphViewDataList = rowViewData.GetSubGraphViewDataList().GetSubGraphViewDataList();
	p_pSGVDIterator->m_pSubGraphViewDataList = &subGraphViewDataList;
}
void CIWChartManager::GetSubGraphViewData_GetNextRowViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator)
{
	if( !p_pSGVDIterator)							return;
	p_pSGVDIterator->m_pRowViewData				= NULL;
	p_pSGVDIterator->m_pSubGraphViewDataList	= NULL;

	if( !p_pSGVDIterator->m_pRowViewDataList)		return;
	if( !p_pSGVDIterator->m_posRowViewData)			return;
	p_pSGVDIterator->m_pRowViewDataList->GetNext( p_pSGVDIterator->m_posRowViewData);
	if( !p_pSGVDIterator->m_posRowViewData)			return;

	CRowViewData &rowViewData = p_pSGVDIterator->m_pRowViewDataList->GetAt( p_pSGVDIterator->m_posRowViewData);
	p_pSGVDIterator->m_pRowViewData				= &rowViewData;
	CList<CSubGraphViewData, CSubGraphViewData&> &subGraphViewDataList = rowViewData.GetSubGraphViewDataList().GetSubGraphViewDataList();
	p_pSGVDIterator->m_pSubGraphViewDataList	= &subGraphViewDataList;
}
void CIWChartManager::GetSubGraphViewData_GetFirstSubGraphViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator)
{
	if( !p_pSGVDIterator) return;
	p_pSGVDIterator->m_posSubGraphViewData		= NULL;
	p_pSGVDIterator->m_pSubGraphViewData		= NULL;

	if( !p_pSGVDIterator->m_pSubGraphViewDataList)	return;
	p_pSGVDIterator->m_posSubGraphViewData	= p_pSGVDIterator->m_pSubGraphViewDataList->GetHeadPosition();
	if( !p_pSGVDIterator->m_posSubGraphViewData)	return;

	CSubGraphViewData &subGraphViewData		= p_pSGVDIterator->m_pSubGraphViewDataList->GetAt( p_pSGVDIterator->m_posSubGraphViewData);
	p_pSGVDIterator->m_pSubGraphViewData	= &subGraphViewData;
}
void CIWChartManager::GetSubGraphViewData_GetNextSubGraphViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator)
{
	if( !p_pSGVDIterator)							return;
	p_pSGVDIterator->m_pSubGraphViewData	= NULL;

	if( !p_pSGVDIterator->m_pSubGraphViewDataList)	return;
	if( !p_pSGVDIterator->m_posSubGraphViewData)	return;
	p_pSGVDIterator->m_pSubGraphViewDataList->GetNext( p_pSGVDIterator->m_posSubGraphViewData);
	if( !p_pSGVDIterator->m_posSubGraphViewData)	return;

	CSubGraphViewData &subGraphViewData = p_pSGVDIterator->m_pSubGraphViewDataList->GetAt( p_pSGVDIterator->m_posSubGraphViewData);
	p_pSGVDIterator->m_pSubGraphViewData	= &subGraphViewData;
}
