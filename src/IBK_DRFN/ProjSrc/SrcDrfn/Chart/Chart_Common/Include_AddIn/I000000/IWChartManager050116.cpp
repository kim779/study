// IWChartManager050116.cpp: implementation of the CIWChartManager050116 class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// *. 주의!	050116 Version의 AddIn에서 이용되는 OCX측 Interface의 Wrapper로 기능 추가만 가능합니다.
//		기능 삭제와 Type 변경은 허용하지 않습니다.
//		허용되지 않는 편집은 050116 Version의 모든 AddIn Module과 관련 Site OCX가
//			함께 수정되어 내려가는 것이 전제되어야 합니다.
// *. 050116 Version Info
//		AddIn Module				Site			Manager
//			ChartCustomAddIn			GZone			배승원
//			ChartPatternAddIn			GZone			정서정
//			ChartGuideLine				GZone			남상윤
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWChartManager050116.h"

#include "MainBlock.h"				// for CMainBlock

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIWChartManager050116::CIWChartManager050116( CMainBlock *p_pMainBlock)
{
	// (2004.10.11, 배승원) MainBlock Object의 Pointer를 관리한다.
	m_pMainBlock = p_pMainBlock;
	if( !m_pMainBlock)
	{
		AfxMessageBox( "CIWChartManager050116 Null Wrapping Error!");
		return;
	}
}

CIWChartManager050116::~CIWChartManager050116()
{

}

//////////////////////////////////////////////////////////////////////
// [04/10/11] Block의 행과 열수를 조회하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

int CIWChartManager050116::GetRowCount( void)
{
	if( m_pMainBlock) return m_pMainBlock->GetRowCount();
	return 0;
}
int CIWChartManager050116::GetColumnCount( void)
{
	if( m_pMainBlock) return m_pMainBlock->GetColumnCount();
	return 0;
}

//////////////////////////////////////////////////////////////////////
// [04/10/11] Block의 영역을 조회하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// Block Region
CRect CIWChartManager050116::GetBlockRegion( int p_nRowIndex, int p_nColumnIndex)
{
	if( !m_pMainBlock) return CRect( 0, 0, 0, 0);
	CBlockIndex blockIndex( p_nRowIndex, p_nColumnIndex);
	return m_pMainBlock->GetBlockRegion( blockIndex);
}

// Graph Region
// View의 Graph Drawing영역이다. (실제 Graph의 좌표 영역이다.)
CRect CIWChartManager050116::GetNoFrameGraphRegion( int p_nRowIndex, int p_nColumnIndex)	// Block Region
{
	if( !m_pMainBlock) return CRect( 0, 0, 0, 0);
	CBlockIndex blockIndex( p_nRowIndex, p_nColumnIndex);
	return m_pMainBlock->GetNoFrameGraphRegion( blockIndex);
}
CRect CIWChartManager050116::GetNoFrameGraphRegion( CPoint p_pPoint)
{
	if( m_pMainBlock) return m_pMainBlock->GetNoFrameGraphRegion( p_pPoint);
	return CRect( 0, 0, 0, 0);
}
// View의 Graph Drawing영역이다. (실제 Graph의 좌표 영역이다.)
CRect CIWChartManager050116::GetDrawingGraphRegion( int p_nRowIndex, int p_nColumnIndex)	// Graph Region
{
	CBlockIndex blockIndex( p_nRowIndex, p_nColumnIndex);
	if( m_pMainBlock) return m_pMainBlock->GetDrawingGraphRegion( blockIndex);
	return CRect( 0, 0, 0, 0);
}
CRect CIWChartManager050116::GetDrawingGraphRegion( CPoint p_pPoint)
{
	if( m_pMainBlock) return m_pMainBlock->GetDrawingGraphRegion( p_pPoint);
	return CRect( 0, 0, 0, 0);
}

//////////////////////////////////////////////////////////////////////
// [04/10/13] Block의 Graph Data를 조회하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// [04/10/13] Block내에 해당 지표가 등록되어 있는지 확인하는 Interface를 제공한다.
BOOL CIWChartManager050116::CheckGraphInBlock( int p_nRowIndex, int p_nColumnIndex, const char *p_szGraphName)
{
	if( !m_pMainBlock) return FALSE;
	CBlockIndex blockIndex( p_nRowIndex, p_nColumnIndex);
	return m_pMainBlock->IsExistenceGraphInBlock( p_szGraphName, blockIndex);
}

// [04/10/26] 지정되는 Graph의 Data Packet Name을 조회하는 Interface를 제공한다.
BOOL CIWChartManager050116::GetPacketNameOfGraph( const char *p_szGraphName, CString &p_strPacketNames)
{
	if( !m_pMainBlock) return FALSE;
	p_strPacketNames = m_pMainBlock->GetGraphPacketName( p_szGraphName);
	return !p_strPacketNames.IsEmpty();
}

// [04/10/27] 특정 Graph가 등록된 첫 Block의 Index를 조회하는 Interface를 제공한다.
BOOL CIWChartManager050116::GetBlockIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC)
{
	// 0. 먼저 MainBlock DLL을 확인한다.
	if( !m_pMainBlock) return FALSE;
	// 1. Block Count를 확인한다.
	int nBlockRow		= m_pMainBlock->GetRowCount();
	int nBlockColumn	= m_pMainBlock->GetColumnCount();
	// 2. 각 Block마다 해당 Graph가 있는지 확인한다.
	for( p_nR = 0; p_nR < nBlockRow; p_nR++)
	{
		for( p_nC = 0; p_nC < nBlockColumn; p_nC++)
			if( CheckGraphInBlock( p_nR, p_nC, p_szGraphName)) break;
		if( p_nC < nBlockColumn) break;
	}
	// 3. 해당 Graph를 찾은 경우를 처리한다.
	if( p_nR < nBlockRow) return TRUE;
	// 4. 해당 Graph를 찾지 못한 경우를 처리한다.
	p_nR = p_nC = 0;
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// [04/10/13] View의 정보를 구하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// [04/10/13] 해당 지표가 있는 View의 Min/Max값을 산출하는 Interface를 제공한다.
BOOL CIWChartManager050116::GetMinMaxDataInViewWithGraph( const char *p_szGraphName, double &p_dMin, double &p_dMax)
{
	// 0. 먼저 Return 값을 초기화한다.
	p_dMin = p_dMax = ERRORVALUE;
	if( !m_pMainBlock) return FALSE;
	return m_pMainBlock->GetDisplayDataMinMax( p_szGraphName, p_dMin, p_dMax);
}

// [04/10/27] 지정된 Block의 Y Scale Conversion Type을 구한다.
BOOL CIWChartManager050116::GetVerticalScaleConversionType( int p_nR, int p_nC, int &p_bLog, int &p_bReverse)
{
	// 0. 먼저 MainBlock DLL을 확인한다.
	if( !m_pMainBlock) return FALSE;

	// 1. Conversion Type을 구한다.
	CBlockIndex blockIndex( p_nR, p_nC);

	bool bReverse = 0, bLog = 0;
	BOOL bResult = m_pMainBlock->GetVertScaleType( blockIndex, bLog, bReverse);
	p_bReverse	= bReverse;
	p_bLog		= bLog;

	return bResult;
}
