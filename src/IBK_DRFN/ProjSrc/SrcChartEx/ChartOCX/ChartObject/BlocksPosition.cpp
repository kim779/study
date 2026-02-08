// BlocksPosition.cpp: implementation of the CBlocksPosition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlocksPosition.h"

#include <assert.h>
#include "../Include_Chart/Conversion.h"
#include "BlocksPositionCalculator.h"
#include "MainBlockImp.h"						// for CMainBlockImp

// ViewDraw.dll
#include "../Include_Chart/Dll_Load/GraphFrameDrawer.h" // block 구분선을 그린다.

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CBlocksPositionBase
//////////////////////////////////////////////////////////////////////

CBlocksPositionBase::CBlocksPositionBase() : m_allBlockRegion(0, 0, 0, 0)
{
}

CBlocksPositionBase::~CBlocksPositionBase()
{
	BlockPositionReset();
}

CList< double, double> &CBlocksPositionBase::GetRowRate( void)
{
	ClearRowReal();
	return m_dlRowRate;
}

int CBlocksPositionBase::GetRowCount( void) const
{
	return m_dlRowRate.GetCount();
}

void CBlocksPositionBase::UpdateRowRealFromRowRate( void)
{
	// 0. Check 0 or 1 Block.
	if( m_dlRowRate.GetCount() < 1) return;
	m_dlRowReal.AddTail( &m_dlRowRate);

	// 1. Get the Horizontal Scale Count.
	CList< int, int> lnSumOfHScaleHeight;
	m_pMainBlock->GetSumOfHorzScaleHeightForEachBlock( lnSumOfHScaleHeight);

	// (2009/9/5 - Seung-Won, Bae) for Invalid Values. (make dummy data. or delete left data.)
	if( lnSumOfHScaleHeight.GetCount() + 1 < m_dlRowReal.GetCount())
	{
		int nMoreInfoCount = m_dlRowReal.GetCount() - ( lnSumOfHScaleHeight.GetCount() + 1);
		for( int i = 0; i < nMoreInfoCount; i++) m_dlRowReal.RemoveTail();
	}
	else if( m_dlRowReal.GetCount() < lnSumOfHScaleHeight.GetCount() + 1)
	{
		int nLessInfoCount = ( lnSumOfHScaleHeight.GetCount() + 1) - m_dlRowReal.GetCount();
		double lLastData = m_dlRowReal.GetTail();
		for( int i = 0; i < nLessInfoCount; i++) m_dlRowReal.AddTail( lLastData);
	}

	int nRegionHeight = m_allBlockRegion.Height();
	// (2009/9/6 - Seung-Won, Bae) for Block Maximizing.
	if( 0 <= m_biMaximizedBlock.GetRow())
	{
		int i = 0;
		POSITION psnRowPos = m_dlRowReal.GetHeadPosition();
		while( psnRowPos)
		{
			if( i <=  m_biMaximizedBlock.GetRow())	m_dlRowReal.SetAt( psnRowPos, 0);
			else									m_dlRowReal.SetAt( psnRowPos, nRegionHeight);
			m_dlRowReal.GetNext( psnRowPos);
			i++;
		}
		return;
	}

	int nFullHScaleHeight = 0;
	POSITION psnScaleHeight = lnSumOfHScaleHeight.GetHeadPosition();
	while( psnScaleHeight) nFullHScaleHeight += lnSumOfHScaleHeight.GetNext( psnScaleHeight);

	// 2. Get the new height of the all Block Region.
	nRegionHeight -= nFullHScaleHeight;

	// 3. ReArrange the Block Position without H Scale.
	CBlocksPositionCalculator::RecalculatePositions( m_dlRowReal, ( int)m_dlRowReal.GetTail(), nRegionHeight);

	// 4. Add H Scale to Block Region.
	double dRowPosition = 0;
	int nScaleHeightSum = 0;
	POSITION psnRow = m_dlRowReal.GetHeadPosition();
	m_dlRowReal.GetNext( psnRow);	// to skip the first base position 0.
	psnScaleHeight = lnSumOfHScaleHeight.GetHeadPosition();
	while( psnRow && psnScaleHeight)
	{
		dRowPosition = m_dlRowReal.GetAt( psnRow);
		nScaleHeightSum += lnSumOfHScaleHeight.GetAt( psnScaleHeight);
		m_dlRowReal.SetAt( psnRow, dRowPosition + nScaleHeightSum);
		m_dlRowReal.GetNext( psnRow);
		lnSumOfHScaleHeight.GetNext( psnScaleHeight);
	}
}

CList< double, double>	&CBlocksPositionBase::GetRowReal( void)
{
	if( m_dlRowReal.IsEmpty()) UpdateRowRealFromRowRate();
	return m_dlRowReal;
}

const CList< double, double> &CBlocksPositionBase::GetConstRowReal( void) const
{
	CBlocksPositionBase *pThis = ( CBlocksPositionBase *)this;
	return pThis->GetRowReal();
}

bool CBlocksPositionBase::MoveBlocksPosition( CList< double, double>& columnList, const CPoint& point, const int rowCompartLineNum, const int columnCompartLineNum)
{
	CList< double, double>	&lRowReal = GetRowReal();
	bool bResult = CBlocksPositionCalculator::MoveBlocksPosition( lRowReal, columnList, point, rowCompartLineNum, columnCompartLineNum);
	
	// UpdateRowRateFromRowReal()

	// 1. Get the Rate with Scale.
	m_dlRowRate.RemoveAll();
	m_dlRowRate.AddTail( &m_dlRowReal);

	// 2. Check the 0 or 1 Block.
	if( m_dlRowRate.GetCount() < 3) return bResult;
	
	// 3. Get the Horizontal Scale Count.
	CList< int, int> lnSumOfHScaleHeight;
	m_pMainBlock->GetSumOfHorzScaleHeightForEachBlock( lnSumOfHScaleHeight);
	ASSERT( lnSumOfHScaleHeight.GetCount() + 1 == m_dlRowRate.GetCount());

	// 4. Substract H Scale from Block Region.
	double dRowPosition = 0;
	int nScaleHeightSum = 0;
	POSITION psnRow = m_dlRowRate.GetHeadPosition();
	m_dlRowRate.GetNext( psnRow);	// to skip the first base position 0.
	POSITION psnScaleHeight = lnSumOfHScaleHeight.GetHeadPosition();
	while( psnRow && psnScaleHeight)
	{
		dRowPosition = m_dlRowRate.GetAt( psnRow);
		nScaleHeightSum -= lnSumOfHScaleHeight.GetAt( psnScaleHeight);
		m_dlRowRate.SetAt( psnRow, dRowPosition + nScaleHeightSum);
		m_dlRowRate.GetNext( psnRow);
		lnSumOfHScaleHeight.GetNext( psnScaleHeight);
	}

	return bResult;
}

void CBlocksPositionBase::ClearRowReal( void)
{
	m_dlRowReal.RemoveAll();
}

void CBlocksPositionBase::BlockPositionReset( void)
{
	m_dlRowRate.RemoveAll();
	m_dlRowReal.RemoveAll();
	m_lColumn.RemoveAll();
}

// 블럭 최대화 추가 - ojtaso (20080519)
void CBlocksPositionBase::SetMaximizedBlock( const CBlockIndex& maxBlockIndex)
{
	ClearRowReal();
	m_biMaximizedBlock = maxBlockIndex;
}

// 블럭 최대화 추가 - ojtaso (20080519)
CBlockIndex	CBlocksPositionBase::GetMaximizedBlock( void) const
{
	return m_biMaximizedBlock;
}

//////////////////////////////////////////////////////////////////////
// class CBlocksPosition
//////////////////////////////////////////////////////////////////////

CBlocksPosition::CBlocksPosition():
	m_nInterBlockHalfGap(0),
	m_nCompartLineThickness(1)
{
}

CBlocksPosition::~CBlocksPosition()
{
	BlockPositionReset();
}

// public =================================================================================
// block의 위치 저장한 list를 Setting.

// 모든 내용 setting.
bool CBlocksPosition::Set(const CRect& allBlockRegion, const CBlockIndex& blockCount, 
			const CBlockIndex& doubleBlockSizeIndex, const int interBlockHalfGap, const int compartLineThickness)
{
	// block들의 최소 영역 검사
	if(!IsBiggerThanMinMainRegion(allBlockRegion, blockCount, interBlockHalfGap))
		return false;

	m_allBlockRegion = allBlockRegion;
	m_nInterBlockHalfGap = interBlockHalfGap;
	m_nCompartLineThickness = compartLineThickness;
	
	CList< double, double> &lRowRate = GetRowRate();
	Add(lRowRate, blockCount.GetRow(), doubleBlockSizeIndex.GetRow(), allBlockRegion.bottom - allBlockRegion.top);
	Add(m_lColumn, blockCount.GetColumn(), doubleBlockSizeIndex.GetColumn(), allBlockRegion.right - allBlockRegion.left);
	return true;
}

bool CBlocksPosition::SetBlockCount(const CBlockIndex& blockCount, const CBlockIndex& doubleBlockSizeIndex)
{
	// block들의 최소 영역 검사
	if(!IsBiggerThanMinMainRegion(m_allBlockRegion, blockCount, m_nInterBlockHalfGap))
		return false;

	if(GetRowBlockCount() == blockCount.GetRow() && GetColumnBlockCount() == blockCount.GetColumn())
		return false;

	CList< double, double> &lRowRate = GetRowRate();
	Add(lRowRate, blockCount.GetRow(), doubleBlockSizeIndex.GetRow(), m_allBlockRegion.bottom - m_allBlockRegion.top);
	Add(m_lColumn, blockCount.GetColumn(), doubleBlockSizeIndex.GetColumn(), m_allBlockRegion.right - m_allBlockRegion.left);
	return true;
}

// 행의 총 갯수 setting. --> block 위치를 모두 지우고 다시 정한다.
bool CBlocksPosition::SetRowBlockCount(const int rowCount, const int doubleSizeRowNum)
{	
	// block들의 최소 영역 검사
	if(!IsBiggerThanMinMainRegion(m_allBlockRegion, CBlockIndex(rowCount, (m_lColumn.GetCount() -1)), m_nInterBlockHalfGap))
		return false;

	CList< double, double> &lRowRate = GetRowRate();
	Add(lRowRate, rowCount, doubleSizeRowNum, m_allBlockRegion.bottom - m_allBlockRegion.top);
	return true;
}

// 열의 총 갯수 setting. --> block 위치를 모두 지우고 다시 정한다.
bool CBlocksPosition::SetColumnBlockCount(const int columnCount, const int doubleSizeColumnNum)
{
	// block들의 최소 영역 검사
	if(!IsBiggerThanMinMainRegion(m_allBlockRegion, CBlockIndex( GetRowCount() -1, columnCount), m_nInterBlockHalfGap))
		return false;

	Add(m_lColumn, columnCount, doubleSizeColumnNum, m_allBlockRegion.right - m_allBlockRegion.left);
	return true;
}

// block size가 2배일 경우 영역 setting
void CBlocksPosition::SetDoubleSizeBlockNumber(const CBlockIndex& doubleSizeBlockIndex)
{
	CList< double, double> &lRowRate = GetRowRate();
	Add(lRowRate, GetRowBlockCount(), doubleSizeBlockIndex.GetRow(), m_allBlockRegion.bottom - m_allBlockRegion.top);
	Add(m_lColumn, GetColumnBlockCount(), doubleSizeBlockIndex.GetColumn(), m_allBlockRegion.right - m_allBlockRegion.left);
}

// ---------------------------------------------------------------------------------------
// 기존 block의 위치를 수정.
// double block 을 적용
/*
bool CBlocksPosition::InitializeBlockRegions(const CRect& allBlockRegion, const CBlockIndex& doubleSizeBlockIndex)
{
	// block의 영역위치를 바꿀만한 영역인지를 체크하여 최소영역보다 클경우만 위치 바꾼다.
	if(!IsBiggerThanMinMainRegion(allBlockRegion, CBlockIndex((m_lRow.GetCount() -1), (m_lColumn.GetCount() -1)), m_nInterBlockHalfGap))
		return false;

	CBlocksPositionCalculator::InitializePositions(allBlockRegion, 
		doubleSizeBlockIndex.GetRow(), doubleSizeBlockIndex.GetColumn(), m_lRow, m_lColumn, 
		m_nInterBlockHalfGap, m_nCompartLineThickness);
	m_allBlockRegion = allBlockRegion;
	return true;
}
*/

// 기존 위치를 수정.
bool CBlocksPosition::RecalculateBlockRegions(const CRect& allBlockRegion)
{
	if(m_allBlockRegion == allBlockRegion)
		return false;

	// block의 영역위치를 바꿀만한 영역인지를 체크하여 최소영역보다 클경우만 위치 바꾼다.
	if(!IsBiggerThanMinMainRegion(allBlockRegion, CBlockIndex( GetRowCount() - 1, (m_lColumn.GetCount() -1)), m_nInterBlockHalfGap))
		return false;

	// (2009/6/26 - Seung-Won, Bae) Do not Recalc the Row Rate. (it is same rate.)
	//		Just clear row real.
	CBlocksPositionCalculator::RecalculatePositions( m_lColumn, ( m_allBlockRegion.right - m_allBlockRegion.left), ( allBlockRegion.right - allBlockRegion.left));

	m_allBlockRegion = allBlockRegion;
	ClearRowReal();
	return true;
}

void CBlocksPosition::InitializeNormalSizeBlockNumber()
{
	CList< double, double> &lRowRate = GetRowRate();
	Add(lRowRate, GetRowBlockCount(), -1, m_allBlockRegion.bottom - m_allBlockRegion.top);
	Add(m_lColumn, GetColumnBlockCount(), -1, m_allBlockRegion.right - m_allBlockRegion.left);
}

// 받은 인자값으로 모든 block의 위치에 setting.
void CBlocksPosition::SetBlocksPosition(const CString& blocksPosition)
{
	// block 위치 적용.
	CList< double, double> &lRowRate = GetRowRate();
	CBlocksPositionCalculator::SetBlocksPosition(lRowRate, m_allBlockRegion.Height(), 
		CDataConversion::GetMidToTrimDataIndex(blocksPosition, "row;", "\r\n"));
	CBlocksPositionCalculator::SetBlocksPosition(m_lColumn, m_allBlockRegion.Width(), 
		CDataConversion::GetMidToTrimDataIndex(blocksPosition, "column;", "\r\n"));
}

// block간의 gap / 2 setting. --> list 수정 필요없음.
void CBlocksPosition::SetInterBlockHalfGap(const int interBlockHalfGap)
{
	m_nInterBlockHalfGap = interBlockHalfGap;
}

// 구분선의 굵기 setting. --> list 수정 필요없음.
void CBlocksPosition::SetCompartLineThickness(const int compartLineThickness)
{
	if(compartLineThickness >= 0)
		m_nCompartLineThickness = compartLineThickness;
}

// ----------------------------------------------------------------------------------------
// 모든 block의 위치(m_lRow, m_lColumn)를 string로 가져오기.
// --> Gap를 포함한 영역
CString CBlocksPosition::GetBlocksPosition() const
{
	const CList< double, double> &lRowRate = GetConstRowRate();
	CString rowsPosition = CBlocksPositionCalculator::GetSavedPosition(lRowRate, "row;");
	CString columnsPosition = CBlocksPositionCalculator::GetSavedPosition(m_lColumn, "column;");
	return (rowsPosition + columnsPosition);
}

// 모든 block의 영역을 구한다.
CRect CBlocksPosition::GetAllBlockRegion() const
{
	return m_allBlockRegion;
}

// blocksColumn의 전체 영역을 가져온다.
CRect CBlocksPosition::GetBlocksColumnRegion(const int columnIndex) const
{
	assert(columnIndex >= 0 && columnIndex < GetColumnBlockCount());

	CRect firstBlockRegion = GetBlockRegion(CBlockIndex(0, columnIndex));
	return CRect(firstBlockRegion.left, m_allBlockRegion.top, firstBlockRegion.right, m_allBlockRegion.bottom);
}

// row, column번째의 block영역을 구한다. (Gap이 포함안된 영역) => 절대좌표
CRect CBlocksPosition::GetBlockRegion(const CBlockIndex& blockIndex) const
{
	assert(blockIndex.GetColumn() >= 0 && blockIndex.GetColumn() < GetColumnBlockCount());
	assert(blockIndex.GetRow() >= 0 && blockIndex.GetRow() < GetRowBlockCount());

	double x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	CBlocksPositionCalculator::GetBlockRegion(m_allBlockRegion.left, m_nInterBlockHalfGap, m_nCompartLineThickness, m_lColumn, blockIndex.GetColumn(), x1, x2);
	const CList< double, double> &lRowReal = GetConstRowReal();
	CBlocksPositionCalculator::GetBlockRegion(m_allBlockRegion.top, m_nInterBlockHalfGap, m_nCompartLineThickness, lRowReal, blockIndex.GetRow(), y1, y2);
	return CRect( ( int)x1, ( int)y1, ( int)x2, ( int)y2);
}

// block 의 총 갯수
int CBlocksPosition::GetBlockCount() const
{
	return (GetRowBlockCount() * GetColumnBlockCount());
}

// 행의 총 갯수
int CBlocksPosition::GetRowBlockCount() const
{
	if( GetRowCount() <= 0) return 0;
	return GetRowCount() - 1;
}

// 열의 총 갯수
int CBlocksPosition::GetColumnBlockCount() const
{
	if(m_lColumn.GetCount() <= 0)
		return 0;

	return (m_lColumn.GetCount() -1);
}

bool CBlocksPosition::GetRowAndColumn(const CPoint& point, CBlockIndex& blockIndex) const
{
	for(int columnIndex = 0; columnIndex < GetColumnBlockCount(); columnIndex++){
		for(int rowIndex = 0; rowIndex < GetRowBlockCount(); rowIndex++){
			if(GetBlockRegion(CBlockIndex(rowIndex, columnIndex)).PtInRect(point)){
				blockIndex.SetIndex(rowIndex, columnIndex);
				return true;
			}
		}
	}
	return false;
}

bool CBlocksPosition::GetRowAndColumn(const CRect& region, CBlockIndex& blockIndex) const
{
	for(int columnIndex = 0; columnIndex < GetColumnBlockCount(); columnIndex++){
		for(int rowIndex = 0; rowIndex < GetRowBlockCount(); rowIndex++){
			if(CBlocksPositionCalculator::IsRectInRect(GetBlockRegion(CBlockIndex(rowIndex, columnIndex)), region)){
				blockIndex.SetIndex(rowIndex, columnIndex);
				return true;
			}
		}		
	}
	return false;
}

// block간의 Gap / 2
int CBlocksPosition::GetInterBlockHalfGap() const
{
	return m_nInterBlockHalfGap;
}

// ----------------------------------------------------------------------------------------
// 구분선을 그린다.
void CBlocksPosition::DrawBlockCompartLine(CDC* pDC, const COLORREF& compartLineColor)
{
	DrawRowCompartLine(pDC, compartLineColor);
	DrawColumnCompartLine(pDC, compartLineColor);
}

// ----------------------------------------------------------------------------------------
// point가 row, column에 해당되는 영역에 적합한 지를 찾는다.
// --> 해당 구분선이 몇번째 행과 열에 있는지 찾기.
CBlockIndex CBlocksPosition::FindCompartLine(const CPoint& point) const
{
	int rowIndex = -1, columnIndex = -1;
	const CList< double, double> &lRowReal = GetConstRowReal();
	if(!CBlocksPositionCalculator::FindCompartLine(CPoint(m_allBlockRegion.left, m_allBlockRegion.top), 
		lRowReal, m_lColumn, point, rowIndex, columnIndex))
		return CBlockIndex(-1, -1);

	return CBlockIndex(rowIndex, columnIndex);
}

// 구분선의 선택된 경우 point 위치로 position 변경
bool CBlocksPosition::MoveBlocksPosition(const CPoint& point, const CBlockIndex& compartLineIndex)
{
	return CBlocksPositionBase::MoveBlocksPosition( m_lColumn, point, compartLineIndex.GetRow(), compartLineIndex.GetColumn());
}

// ----------------------------------------------------------------------------------------
// block 위치 변경
bool CBlocksPosition::MoveBlockPosition(const int sourceIndex, const int targetIndex, const CBlockBaseData::BLOCKTYPE type)
{
	if(type == CBlockBaseData::ROW)
	{
		CList< double, double> &lRowRate = GetRowRate();
		return CBlocksPositionCalculator::MoveBlockPosition( lRowRate, sourceIndex, targetIndex);
	}
	else if(type == CBlockBaseData::COLUMN)
		return CBlocksPositionCalculator::MoveBlockPosition(m_lColumn, sourceIndex, targetIndex);

	return false;
}


// private ================================================================================
// block의 영역위치를 바꿀만한 영역인지를 체크
// --> block 전체 최소영역보다 클경우만 모든 data 변경 가능
bool CBlocksPosition::IsBiggerThanMinMainRegion(const CRect& allBlockRegion, const CBlockIndex& blockCount, const int interBlockHalfGap) const
{
	if(!IsBiggerThanMinMainRegion(blockCount.GetRow(), (allBlockRegion.bottom - allBlockRegion.top), interBlockHalfGap))
		return false;

	return IsBiggerThanMinMainRegion(blockCount.GetColumn(), (allBlockRegion.right - allBlockRegion.left), interBlockHalfGap);
}

bool CBlocksPosition::IsBiggerThanMinMainRegion(const int blockCount, const int allBlocksRegion, const int interBlockHalfGap) const
{
	return CBlocksPositionCalculator::IsBiggerThanMinMainRegion(blockCount, allBlocksRegion, interBlockHalfGap*2, m_nCompartLineThickness);
}

// ----------------------------------------------------------------------------------------
// block의 영역을 CList로 저장한다.
// list에 추가한다.
void CBlocksPosition::Add(CList< double, double>& positionList, const int blockCount, const int doubleSizeBlockNum, const int allRegionLength)
{
	positionList.RemoveAll();
	if(blockCount <= 0)
		return;

	// 균등 분할
	if(IsEqualityPartition(blockCount, doubleSizeBlockNum)){
		CBlocksPositionCalculator::Add_EqualityPartition(positionList, blockCount, 
			allRegionLength, m_nInterBlockHalfGap, m_nCompartLineThickness);
	}
	// 2배 분할
	else{
		CBlocksPositionCalculator::Add_DoubleSize(positionList, blockCount, 
			doubleSizeBlockNum, allRegionLength, m_nInterBlockHalfGap, m_nCompartLineThickness);
	}
}

bool CBlocksPosition::IsEqualityPartition(const int blockCount, const int doubleSizeBlockNum) const
{
	return (blockCount == 1 || doubleSizeBlockNum < 0 || doubleSizeBlockNum >= blockCount);
}

// ----------------------------------------------------------------------------------------
void CBlocksPosition::DrawRowCompartLine(CDC* pDC, const COLORREF& compartLineColor)
{
	// 속도가 느리는 경우 end point 구하는 부분 수정하기. end.y == start.y
	CPoint absoluteOrgPt(m_allBlockRegion.left, m_allBlockRegion.top);
	CGraphFrameDrawer graphFrameDrawer;
	const CList< double, double> &lRowReal = GetConstRowReal();
	for(int row = 1; row < GetRowCount() - 1; row++)
	{
		CPoint start = CBlocksPositionCalculator::GetAbsolutePoint(absoluteOrgPt, lRowReal, m_lColumn, 0, row);
		CPoint end = CBlocksPositionCalculator::GetAbsolutePoint(absoluteOrgPt, lRowReal, m_lColumn, (m_lColumn.GetCount()-1), row);
		graphFrameDrawer.DrawBlockCompartLine(pDC, start, end, compartLineColor);
	}
}

void CBlocksPosition::DrawColumnCompartLine(CDC* pDC, const COLORREF& compartLineColor)
{
	// 속도가 느리는 경우 end point 구하는 부분 수정하기. end.x == start.x
	CPoint absoluteOrgPt(m_allBlockRegion.left, m_allBlockRegion.top);
	CGraphFrameDrawer graphFrameDrawer;
	const CList< double, double> &lRowReal = GetConstRowReal();
	for(int column = 1; column < m_lColumn.GetCount() -1; column++)
	{
		CPoint start = CBlocksPositionCalculator::GetAbsolutePoint( absoluteOrgPt, lRowReal, m_lColumn, column, 0);
		CPoint end = CBlocksPositionCalculator::GetAbsolutePoint( absoluteOrgPt, lRowReal, m_lColumn, column, GetRowCount() - 1);
		graphFrameDrawer.DrawBlockCompartLine(pDC, start, end, compartLineColor);
	}
}

// ----------------------------------------------------------------------------------------

// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
void CBlocksPosition::SetHorzScalePosition( CHorizontalScale &p_horizontalScale, const CScaleBaseData::HORZSCALEPOSITION ePosition)
{
	ClearRowReal();
	p_horizontalScale.SetHorzScalePosition( ePosition);
}
