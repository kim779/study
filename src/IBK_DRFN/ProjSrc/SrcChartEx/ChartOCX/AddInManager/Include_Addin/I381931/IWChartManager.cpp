// IWChartManager.cpp: implementation of the CIWChartManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWChartManager.h"

#include "../../../Include_Chart/DLL_Load/XScaleManager.h"		// for CXScaleManager
#include "IWBlock.h"											// for CIWBlock
#include "IWString.h"											// for CIWString
#include "IWStringList.h"										// for CIWStringList
#include "IWChartOCX.h"											// for CIWChartOCX

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
// (2008/8/17 - Seung-Won, Bae) Support IChartOCX from Interface of other manager.
//////////////////////////////////////////////////////////////////////
void CIWChartManager::SetIChartOCX( IChartOCX *pIChartOCX)
{
	if( m_pIChartOCX) m_pIChartOCX->Release();
	m_pIChartOCX = pIChartOCX;
	if( m_pIChartOCX) m_pIChartOCX->AddRef();
}
IChartOCX *CIWChartManager::GetIChartOCX( void)
{
	if( m_pIChartOCX) m_pIChartOCX->AddRef();
	return m_pIChartOCX;
}

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
IString *CIWChartManager::GetBlocksPositionString( void)
{
	if( !m_pMainBlock) return NULL;

	IString *pIString = CIWString::CreateObject( m_pMainBlock->GetBlocksPosition());
	return pIString;
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

// SetAllVertScaleGridType
BOOL CIWChartManager::SetAllVertScaleGridType( int p_nGridVLineType)
{
	if( !m_pMainBlock) return FALSE;
	m_pMainBlock->SetAllVertScaleGridType( ( CScaleBaseData::SCALEGRIDTYPE)p_nGridVLineType);
	return TRUE;
}

// (2007/7/18 - Seung-Won, Bae) Support Up/Down Space
BOOL CIWChartManager::GetVertScaleSpace( const int p_nR, const int p_nC, const int p_nV, int &p_nUpSpace, int &p_nDownSpace)
{
	if( !m_pMainBlock) return FALSE;
	return m_pMainBlock->GetVertScaleSpace( p_nR, p_nC, p_nV, p_nUpSpace, p_nDownSpace);
}
BOOL CIWChartManager::SetVertScaleSpace( const int p_nR, const int p_nC, const int p_nV, const int p_nUpSpace, const int p_nDownSpace)
{
	if( !m_pMainBlock) return FALSE;
	return m_pMainBlock->SetVertScaleSpace( p_nR, p_nC, p_nV, p_nUpSpace, p_nDownSpace);
}

//////////////////////////////////////////////////////////////////////
// (2006/6/25 - Seung-Won, Bae) Graph Info
//////////////////////////////////////////////////////////////////////

// [04/10/26] 지정되는 Graph의 Data Packet Name을 조회하는 Interface를 제공한다.
IString *CIWChartManager::GetPacketNameStringOfGraph( const char *p_szGraphName)
{
	if( !m_pMainBlock) return NULL;
	return CIWString::CreateObject( m_pMainBlock->GetGraphPacketName( p_szGraphName));
}

// [04/10/27] 특정 Graph가 등록된 Block, ScaleGroup, Graph의 Index를 조회하는 Interface를 제공한다.
BOOL CIWChartManager::GetIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG)
{
	if( !m_pMainBlock) return FALSE;
	return m_pMainBlock->GetIndexOfGraph( p_szGraphName, p_nR, p_nC, p_nV, p_nG);
}

// (2006/6/27 - Seung-Won, Bae) Retrieve the all Graph Names
IString *CIWChartManager::GetAllIndicatorNameString( void)
{
	if( !m_pMainBlock) return NULL;
	CList< CString, CString> strGraphNameList;
	if( !m_pMainBlock->GetAllIndicatorName( strGraphNameList)) return NULL;
	if( strGraphNameList.IsEmpty()) return NULL;
	CIWString *pIWString = CIWString::CreateObject( NULL);
	POSITION posGraphName = strGraphNameList.GetHeadPosition();
	while( posGraphName) *pIWString += strGraphNameList.GetNext( posGraphName) + ",";
	return pIWString;
}
IStringList *CIWChartManager::GetAllIndicatorNameStringList( void)
{
	if( !m_pMainBlock) return NULL;
	CIWStringList::CStringTList *pStringTList = new CIWStringList::CStringTList;
	m_pMainBlock->GetAllIndicatorName( *pStringTList);
	return CIWStringList::CreateObject( pStringTList, TRUE);
}

// (2007/1/20 - Seung-Won, Bae) Retrieve the all Graph Names
IStringList *CIWChartManager::GetAllGraphNameStringList( void)
{
	if( !m_pMainBlock) return NULL;
	CIWStringList::CStringTList *pStringTList = new CIWStringList::CStringTList;
	m_pMainBlock->GetAllGraphName( *pStringTList);
	return CIWStringList::CreateObject( pStringTList, TRUE);
}

// (2007/1/20 - Seung-Won, Bae) Search Indicator
IString *CIWChartManager::FindFirstIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, const char *p_szRQ)
{
	if( !m_pMainBlock) return NULL;
	CString strGraphName;
	if( !m_pMainBlock->FindFirstIndicator( p_szIndicatorName, p_nR, p_nC, p_nV, p_nG, strGraphName, p_szRQ)) return NULL;
	return CIWString::CreateObject( strGraphName);
}
IString *CIWChartManager::FindNextIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG)
{
	if( !m_pMainBlock) return NULL;
	CString strGraphName;
	if( !m_pMainBlock->FindNextIndicator( p_szIndicatorName, p_nR, p_nC, p_nV, p_nG, strGraphName, "DEFAULT")) return NULL;
	return CIWString::CreateObject( strGraphName);
}

// (2007/2/9 - Seung-Won, Bae) Search Graph
IString *CIWChartManager::FindGraph( POINT p_pt)
{
	if( !m_pMainBlock) return NULL;

	CString strGraphName = m_pMainBlock->FindGraph( p_pt);
	if( strGraphName.IsEmpty()) return NULL;

	return CIWString::CreateObject( strGraphName);
}

// (2007/3/28 - Seung-Won, Bae) Retrieve Packet Name of SubGraph
IString *CIWChartManager::GetSubGraphPacketName( const char *p_szGraphName, const char *p_szSubGraphName, const int p_nPacketIndex)
{
	if( !m_pMainBlock) return NULL;

	CString strPacketName = m_pMainBlock->GetSubGraphPacketName( p_szGraphName, p_szSubGraphName, p_nPacketIndex);
	if( strPacketName.IsEmpty()) return NULL;

	return CIWString::CreateObject( strPacketName);
}

// (2007/4/30 - Seung-Won, Bae) Get Data Index of Inverse Line Graph
int CIWChartManager::GetDataIndexOfInverseLine( POINT p_pt)
{
	if( !m_pMainBlock) return -1;

	return m_pMainBlock->GetDataIndexOfInverseLine( p_pt);
}

//////////////////////////////////////////////////////////////////////
// (2006/6/25 - Seung-Won, Bae) Mouse Position Info
//////////////////////////////////////////////////////////////////////

// (2006/2/6 - Seung-Won, Bae) Get Full Column Region for DragZoom
RECT CIWChartManager::GetGraphRegionSumInColumn( const POINT p_ptParam) const
{
	RECT rctEmpty = { 0, 0, 0, 0};
	if( !m_pMainBlock) return rctEmpty;
	return m_pMainBlock->GetGraphRegionSumInColumn( p_ptParam);
}

// (2006/2/6 - Seung-Won, Bae) Get Full Column Region for DragZoom
BOOL CIWChartManager::GetBlockIndex( const POINT p_ptPoint, int &p_nRow, int &p_nColumn)
{
	if( !m_pMainBlock) return FALSE;
	CBlockIndex bi = m_pMainBlock->FindBlockIndex( p_ptPoint);
	p_nRow = bi.GetRow();
	p_nColumn = bi.GetColumn();
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// (2006/7/27 - Seung-Won, Bae) Graph Calulation.
//////////////////////////////////////////////////////////////////////

BOOL CIWChartManager::Initialize_GraphInfo( const bool p_bIsNULL)
{
	if( !m_pMainBlock) return FALSE;
	m_pMainBlock->Initialize_IndicatorInfo( p_bIsNULL);
	return TRUE;
}

BOOL CIWChartManager::Initialize_AllPackets( const char *p_szGraphName, BOOL p_bWithCalculate)
{
	if( !m_pMainBlock) return FALSE;
	return m_pMainBlock->Initialize_AllPackets( p_szGraphName, p_bWithCalculate ? true : false);
}

BOOL CIWChartManager::ChangeAllGraphInfoFromGraphData( void)
{
	if( !m_pMainBlock) return FALSE;
	return m_pMainBlock->ChangeAllIndicatorInfoFromGraphData();
}

BOOL CIWChartManager::ChangeAllGraphDataFromGraphInfo( BOOL p_bWithCalculate)
{
	if( !m_pMainBlock) return FALSE;
	return m_pMainBlock->ChangeAllGraphDataFromIndicatorInfo( p_bWithCalculate);
}

BOOL CIWChartManager::ChangeGraphDataFromGraphInfo( const char *p_szGraphName, BOOL p_bWithCalculate)
{
	if( !m_pMainBlock) return FALSE;
	return m_pMainBlock->ChangeGraphDataFromIndicatorInfo( p_szGraphName, p_bWithCalculate ? true : false);
}

BOOL CIWChartManager::CalculateAllGraphs( const CCalculateBaseData::CALCULATETYPE eCalType)
{
	if( !m_pMainBlock) return FALSE;
	m_pMainBlock->CalculateAllGraphs( eCalType);
	return TRUE;
}

BOOL CIWChartManager::CalculateGraph( const char *p_szGraphName, const CCalculateBaseData::CALCULATETYPE eCalType, BOOL p_bWithClearPacketData)
{
	if( !m_pMainBlock) return FALSE;
	m_pMainBlock->CalculateGraph( p_szGraphName, eCalType, p_bWithClearPacketData ? true : false);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2006/3/24 - Seung-Won, Bae) Sub Graph View Data Iterator
//////////////////////////////////////////////////////////////////////
POSITION CIWChartManager::GetSubGraphViewData_First( const char *p_szRQ)
{
	if( !m_pMainBlock) return NULL;

	CSubGraphViewDataIterator sgvdIterator;
	if( !m_pMainBlock->GetViewDataListInAllBlock(p_szRQ, *sgvdIterator.m_pViewData)) return NULL;

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
BOOL CIWChartManager::GetSubGraphViewData_Next( POSITION &p_psnViewData, BOOL p_bAutoClose)
{
	if( !p_psnViewData) return FALSE;
	CSubGraphViewDataIterator *pSGVDIterator = m_lSubGraphViewData.GetAt( p_psnViewData);
	if( !pSGVDIterator) return TRUE;

	GetSubGraphViewData_GetNextSubGraphViewDataPosition( pSGVDIterator);
	if( pSGVDIterator->m_posSubGraphViewData) return TRUE;

	GetSubGraphViewData_GetNextRowViewDataPosition( pSGVDIterator);
	while( pSGVDIterator->m_posRowViewData)
	{
		GetSubGraphViewData_GetFirstSubGraphViewDataPosition( pSGVDIterator);
		if( pSGVDIterator->m_posSubGraphViewData) return TRUE;
		GetSubGraphViewData_GetNextRowViewDataPosition( pSGVDIterator);
	}

	GetSubGraphViewData_GetNextColumnViewDataPosition( pSGVDIterator);
	while( pSGVDIterator->m_posColumnViewData)
	{
		GetSubGraphViewData_GetFirstRowViewDataPosition( pSGVDIterator);
		while( pSGVDIterator->m_posRowViewData)
		{
			GetSubGraphViewData_GetFirstSubGraphViewDataPosition( pSGVDIterator);
			if( pSGVDIterator->m_posSubGraphViewData) return TRUE;
			GetSubGraphViewData_GetNextRowViewDataPosition( pSGVDIterator);
		}
	}

	if( p_bAutoClose) GetSubGraphViewData_Close( p_psnViewData);
	return FALSE;
}
BOOL CIWChartManager::GetSubGraphViewData_NextInColumn( POSITION &p_psnViewData, BOOL p_bAutoClose)
{
	if( !p_psnViewData) return FALSE;
	CSubGraphViewDataIterator *pSGVDIterator = m_lSubGraphViewData.GetAt( p_psnViewData);
	if( !pSGVDIterator) return TRUE;

	GetSubGraphViewData_GetNextSubGraphViewDataPosition( pSGVDIterator);
	if( pSGVDIterator->m_posSubGraphViewData) return TRUE;

	GetSubGraphViewData_GetNextRowViewDataPosition( pSGVDIterator);
	while( pSGVDIterator->m_posRowViewData)
	{
		GetSubGraphViewData_GetFirstSubGraphViewDataPosition( pSGVDIterator);
		if( pSGVDIterator->m_posSubGraphViewData) return TRUE;
		GetSubGraphViewData_GetNextRowViewDataPosition( pSGVDIterator);
	}

	if( p_bAutoClose) GetSubGraphViewData_Close( p_psnViewData);
	return FALSE;
}
BOOL CIWChartManager::GetSubGraphViewData_NextInRow( POSITION &p_psnViewData, BOOL p_bAutoClose)
{
	if( !p_psnViewData) return FALSE;
	CSubGraphViewDataIterator *pSGVDIterator = m_lSubGraphViewData.GetAt( p_psnViewData);
	if( !pSGVDIterator) return TRUE;

	GetSubGraphViewData_GetNextSubGraphViewDataPosition( pSGVDIterator);
	if( pSGVDIterator->m_posSubGraphViewData) return TRUE;

	if( p_bAutoClose) GetSubGraphViewData_Close( p_psnViewData);
	return FALSE;
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
IString *CIWChartManager::GetSubGraphViewData_HorzScaleNameString( POSITION p_psnViewData)
{
	if( !p_psnViewData) return NULL;
	CSubGraphViewDataIterator *pSGVDIterator = m_lSubGraphViewData.GetAt( p_psnViewData);
	if( !pSGVDIterator)						return NULL;
	if( !pSGVDIterator->m_pRowViewData)		return NULL;

	return CIWString::CreateObject( pSGVDIterator->m_pRowViewData->GetHorzScaleName());
}
RECT CIWChartManager::GetSubGraphViewData_GraphRegion( POSITION p_psnViewData)
{
	RECT rctEmpty = { 0, 0, 0, 0};
	if( !p_psnViewData) return rctEmpty;
	CSubGraphViewDataIterator *pSGVDIterator = m_lSubGraphViewData.GetAt( p_psnViewData);
	if( !pSGVDIterator)						return rctEmpty;
	if( !pSGVDIterator->m_pRowViewData)		return rctEmpty;

	return pSGVDIterator->m_pRowViewData->GetDrawingGraphRegion();
}
IString *CIWChartManager::GetSubGraphViewData_TitleString( POSITION p_psnViewData)
{
	if( !p_psnViewData) return NULL;
	CSubGraphViewDataIterator *pSGVDIterator = m_lSubGraphViewData.GetAt( p_psnViewData);
	if( !pSGVDIterator)								return NULL;
	if( !pSGVDIterator->m_pSubGraphViewData)		return NULL;
	
	return CIWString::CreateObject( pSGVDIterator->m_pSubGraphViewData->GetTitle());
}
void CIWChartManager::GetSubGraphViewData_TitleAndPacketNameString( POSITION p_psnViewData, ILPCSTR &p_pszTitle, ILPCSTR &p_pszPacketNames)
{
	p_pszTitle			= NULL;
	p_pszPacketNames	= NULL;

	if( !p_psnViewData) return;
	CSubGraphViewDataIterator *pSGVDIterator = m_lSubGraphViewData.GetAt( p_psnViewData);
	if( !pSGVDIterator)								return;
	if( !pSGVDIterator->m_pSubGraphViewData)		return;

	p_pszTitle			= CIWString::CreateObject( pSGVDIterator->m_pSubGraphViewData->GetTitle());
	p_pszPacketNames	= CIWString::CreateObject( pSGVDIterator->m_pSubGraphViewData->GetPacketNames());
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

// XScale
void CIWChartManager::SetXScaleManager(LPVOID lpXScaleManager)
{
	m_pXScaleManager = (CXScaleManager*)lpXScaleManager;
}

void CIWChartManager::RealDataReceived( const char *p_szRQ, BOOL bAppend, BOOL bCalc)
{
	if( m_pXScaleManager) m_pXScaleManager->RealDataReceived( p_szRQ);
}


// 현재 화면에 보여지는 영역의 시작과 끝 위치를 얻어옵니다.
BOOL CIWChartManager::GetStartEndDrawIdx_Cur( const char *p_szRQ, int &p_nStartIndex, int &p_nEndIndex)
{
	if( !m_pXScaleManager) return FALSE;
	//m_pXScaleManager->GetStartEndDrawIdx_Cur(p_nStartIndex, p_nEndIndex);
	m_pXScaleManager->GetDrawIndex(p_szRQ, FALSE, FALSE, p_nStartIndex, p_nEndIndex);
	return TRUE;
}

// 자료 인덱스로 x위치를 얻어온다.
int CIWChartManager::GetXPosition(const char *p_szRQ, int nIdx, int nCol)
{
	if( !m_pXScaleManager) return FALSE;
	CXScaleManager::AREA area;
	m_pXScaleManager->GetAREAfRQnDTI(p_szRQ, nIdx, nCol, &area);

	int nRet = area.center;
	return nRet;
}

int CIWChartManager::GetXPosition( const char *p_szRQ, int nIdx, int nCol, int& nLeft, int& nRight)
{
	if( !m_pXScaleManager) return FALSE;
	CXScaleManager::AREA area;
	m_pXScaleManager->GetAREAfRQnDTI(p_szRQ, nIdx, nCol, &area);

	nLeft = area.left;
	nRight = area.right;
	int nRet = area.center;
	return nRet;
}

void CIWChartManager::GetGraphXAreaSE(int& nStart, int& nEnd, BOOL bIncludeMargin)
{
	if(m_pXScaleManager)
	{
		m_pXScaleManager->GetGraphXAreaSE(nStart, nEnd, bIncludeMargin);
	}
}

int CIWChartManager::GetGraphXArea_Start(BOOL bIncludeMargin)
{
	if( !m_pXScaleManager) return -1;
	return m_pXScaleManager->GetGraphXArea_Start(bIncludeMargin);
}

int CIWChartManager::GetGraphXArea_End(BOOL bIncludeMargin)
{
	if( !m_pXScaleManager) return -1;
	return m_pXScaleManager->GetGraphXArea_End(bIncludeMargin);
}

void CIWChartManager::GetGraphXAreaLR(int& nLeft, int& nRight, BOOL bIncludeMargin)
{
	if(m_pXScaleManager)
	{
		m_pXScaleManager->GetGraphXAreaLR(nLeft, nRight, bIncludeMargin);
	}	
}

int CIWChartManager::GetGraphXArea_Left(BOOL bIncludeMargin)
{
	if( !m_pXScaleManager) return -1;
	return m_pXScaleManager->GetGraphXArea_Left(bIncludeMargin);
}

int CIWChartManager::GetGraphXArea_Right(BOOL bIncludeMargin)
{
	if( !m_pXScaleManager) return -1;
	return m_pXScaleManager->GetGraphXArea_Right(bIncludeMargin);
}

int CIWChartManager::FindViewDataIndex_Pt(const char *p_szRQ, int nXPoint, int nCol, BOOL p_bWithBoundCheck)
{
	if( !m_pXScaleManager) return -1;
	int nRet = 0;
	m_pXScaleManager->GetDTInAREAfRQnPt(p_szRQ, nXPoint, nCol, &nRet, NULL, p_bWithBoundCheck);
	return nRet;
}

time_t CIWChartManager::GetTimeFromX(int nCursor_x, int* pnBlockColumn, int& nCenter_x)
{
	if( !m_pXScaleManager) return 0L;
	CXScaleManager::PAREA pparea;
	time_t ret = m_pXScaleManager->GetTimeFromX(nCursor_x, pnBlockColumn, &pparea);
	nCenter_x = pparea->center;
	return ret;
}

int CIWChartManager::GetXFromTime(time_t timeFind, int& nCenter_x)
{
	if( !m_pXScaleManager) return -1;
	CXScaleManager::PAREA pparea = NULL;
	int ret = m_pXScaleManager->GetXFromTime( 0, timeFind, &pparea);
	if( !pparea) return -1;
	nCenter_x = pparea->center;
	return ret;
}

// VertScaleGroup의 가장 처음 Graph의 타입 : 복수종목 - ojtaso (20070504)
double CIWChartManager::GetDoubleFromX(const int nRow, const int nCol, int nCursor_x, int* pnBlockColumn, int& nCenter_x)
{
	if( !m_pXScaleManager) return 0L;
	CXScaleManager::PAREA pparea;
	double ret = m_pXScaleManager->GetDoubleFromX(nRow, nCol, nCursor_x, pnBlockColumn, &pparea);
	nCenter_x = pparea->center;
	return ret;
}

// VertScaleGroup의 가장 처음 Graph의 타입 : 복수종목 - ojtaso (20070504)
int CIWChartManager::GetXFromDouble(const int nRow, const int nCol, double dblFind, int& nCenter_x)
{
	if( !m_pXScaleManager) return -1;
	CXScaleManager::PAREA pparea;
	int ret = m_pXScaleManager->GetXFromDouble(nRow, nCol, dblFind, &pparea);
	nCenter_x = pparea->center;
	return ret;
}
