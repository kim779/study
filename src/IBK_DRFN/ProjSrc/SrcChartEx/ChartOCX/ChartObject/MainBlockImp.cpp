// MainBlockImp.cpp: implementation of the CMainBlockImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainBlockImp.h"

#include <assert.h>

#include "../Include_Chart/DLL_Load/PacketBase.h"			// for CPacket
#include "../Include_Chart/DLL_Load/PacketList.h"			// for CPacketList
#include "../Include_Chart/Dll_Load/XScaleManager.h"		// for CXScaleManager
#include "../Include_Chart/Dll_Load/ToolBarManager.h"		// for IToolBarManager
#include "../Include_Chart/Dll_Load/PacketListManager.h"	// for CPacketListManager
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/Dll_Load/IndicatorInfo.h"		// for CIndicatorInfo
#include "../Include_Chart/Conversion.h"
#include "../Include_Chart/ViewData.h"
#include "../Include_Chart/IChartCtrl.h"					// for IChartCtrl
#include "BlocksColumn.h"									// 객체를 추가하거나 지우기 위해
#include "BlockImp.h"
#include "BlockDrawer.h"
#include "VertScaleGroup.h"									// for CVertScaleGroup
#include "GraphSearchInfo.h"								// for CGraphSearchInfo
#include "GraphImp.h"										// for CGraph
#include "SubGraph.h"										// for CSubGraph
#include "SelectedSubGraphDeque.h"							// for CSelectedSubGraphDeque
#include "resource.h"										// for IDS_CANNOT_MODIFY_PRICE_CHART_BLOCK
#include "PacketRQ.h"										// for CPacketRQ

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMainBlockImp::CMainBlockImp(IChartCtrl* p_pICharetCtrl)
{
	// (2009/6/25 - Seung-Won, Bae) for Balanced Row Arrange without scale.
	m_blocksPosition.SetMainBlock( this);

	// (2007/04/11 - Seung-Won, Bae) Change Bong Draw Style to Line in 1 Tick Chart.
	m_nBongGraph = 0;

	m_mainBlockBaseData.GetMainAttributes().SetMainBlockImpl(this);
	m_mainBlockBaseData.GetMainAttributes().SetIChartCtrl(p_pICharetCtrl);
	m_mainBlockBaseData.GetMainAttributes().SetMainChartFormulate( &m_mainChartFormulate);

	// ( 2006 / 09 / 26 - Sang-Woo, Cho ) 
	// 가격차트에서 최고, 최저가 Text가 보이게 할 것인지 말 것인지에 대한 설정값.
	m_bShowPriceMinMaxText = TRUE;

	// (2006/10/13 - Seung-Won, Bae) Manage Block Move & Delete Tool Flag for Position of Graph Drawing
	m_bBlockMoveDelete = FALSE;

	// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
	m_eLastHorzGridType		= CScaleBaseData::SHORTDOTTEDLINE;
	m_eLastVerticalGridType	= CScaleBaseData::SHORTDOTTEDLINE;

	//KHD: 시간구분
	// 분/틱차트에서의 일자구분선 개별옵션 - onlyojt
	m_eLastDailyGridType	= CScaleBaseData::SHORTDOTTEDLINE;
	m_WeekGridType		= CScaleBaseData::SHORTDOTTEDLINE;
	m_MonGridType		= CScaleBaseData::SHORTDOTTEDLINE;
	m_YearGridType		= CScaleBaseData::SHORTDOTTEDLINE;
	//END

	// 미래영역 스케일을 그릴것인지 : ojtaso (20070724)
	m_bDrawFutureScale = TRUE;
	
	// 차트DLL에서 복수종목을 사용할지 : 복수종목 - ojtaso (20070904)
	m_bUseMultiItemMode = TRUE;

	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	m_bDrawBaseLine = true;

	// (2008/1/22 - Seung-Won, Bae) for Multi-Language
	m_hOcxWnd = m_mainBlockBaseData.GetParent_Handle();

}

CMainBlockImp::~CMainBlockImp()
{
	m_mainBlockBaseData.GetMainAttributes().SetIChartCtrl(NULL);
	DeleteAllBlock();
}

//////////////////////////////////////////////////////////////////////
// Base Setting
//////////////////////////////////////////////////////////////////////

// ocx 주소
void CMainBlockImp::SetIChartCtrl(IChartCtrl* p_pICharetCtrl)
{
	m_mainBlockBaseData.GetMainAttributes().SetIChartCtrl(p_pICharetCtrl);
}

IChartCtrl *CMainBlockImp::GetIChartCtrl( void)
{
	return m_mainBlockBaseData.GetMainAttributes().GetIChartCtrl();
}

// prject 경로
void CMainBlockImp::SetOCXPath(const CString& strOCXPath)
{
	m_mainBlockBaseData.GetMainAttributes().SetOCXPath(strOCXPath);
}

// project 경로
CString CMainBlockImp::GetOCXPath( void) const
{
	return m_mainBlockBaseData.GetMainAttributes().GetOCXPath();
}

// packetList 을 설정
void CMainBlockImp::SetPacketList(CPacketList* pPacketList, const char *p_szRQ)
{
	m_mainBlockBaseData.GetMainAttributes().SetPacketList(pPacketList);

	// (2009/10/5 - Seung-Won, Bae) for
	if( !p_szRQ) return;
	if( !*p_szRQ) return;

	POSITION psnRQ = m_slRQHistory.Find( p_szRQ);
	while( psnRQ)
	{
		m_slRQHistory.RemoveAt( psnRQ);
		psnRQ = m_slRQHistory.Find( p_szRQ);
	}

	m_slRQHistory.AddHead( p_szRQ);
}

// packet dll 주소
CPacketList* CMainBlockImp::GetPacketList( void) const
{
	return m_mainBlockBaseData.GetPacketList();
}

// 속성에 대한 설정
void CMainBlockImp::SetIndicatorList( CIndicatorList* pIndicatorList)
{
	if( m_mainBlockBaseData.GetIndicatorList() == pIndicatorList)
		return;

	m_mainBlockBaseData.GetMainAttributes().SetIndicatorList(pIndicatorList);
	Initialize_IndicatorInfo(false);
}

// 속성dll 주소
CIndicatorList* CMainBlockImp::GetIndicatorList( void) const
{
	return m_mainBlockBaseData.GetIndicatorList();
}

CMainChartFormulate* CMainBlockImp::GetMainChartFormulate( void) const
{
	return const_cast <CMainChartFormulate*> (&m_mainChartFormulate);
}

void CMainBlockImp::SetXScaleManager(CXScaleManager* pxScaleManager)
{
	m_mainBlockBaseData.GetMainAttributes().SetXScaleManager(pxScaleManager);
}

CXScaleManager* CMainBlockImp::GetXScaleManager() const
{
	return m_mainBlockBaseData.GetXScaleManager();
}

// 복수종목 관리 : 복수종목 - ojtaso (20070111)
void CMainBlockImp::SetPacketListManager(CPacketListManager* pPacketListManager)
{
	m_mainBlockBaseData.GetMainAttributes().SetPacketListManager(pPacketListManager);
}

// 복수종목 관리 : 복수종목 - ojtaso (20070111)
CPacketListManager* CMainBlockImp::GetPacketListManager() const
{
	return m_mainBlockBaseData.GetPacketListManager();
}

void CMainBlockImp::SetToolBarManager(IToolBarManager* pToolBarManager)
{
	m_mainBlockBaseData.GetMainAttributes().SetToolBarManager(pToolBarManager);
}

IToolBarManager* CMainBlockImp::GetToolBarManager() const
{
	return m_mainBlockBaseData.GetToolBarManager();
}

// (2008/1/22 - Seung-Won, Bae) for Multi-Language
HWND CMainBlockImp::GetParent_Handle( void)
{
	return m_mainBlockBaseData.GetParent_Handle();
}

// CMainBlockBaseData
CMainBlockBaseData* CMainBlockImp::GetMainBlockBaseData() const
{
	return const_cast <CMainBlockBaseData*> (&m_mainBlockBaseData);
}

void CMainBlockImp::SetBaseScaleRegion(CDC* pDC)
{
	CSize oneTextSize = pDC->GetTextExtent("0");

	// 가로 scale의 최소 높이(y축의 길이)을 setting.
	//수정요망. -> 가로, 세로인지 체크바람!!
	if( oneTextSize.cy != m_mainBlockBaseData.GetMinHorzScaleRegionHeight())
		m_mainBlockBaseData.GetScaleAttributes().SetMinHorzScaleRegionHeight( oneTextSize.cy + MARGIN * 2);

	// 세로 scale의 최소 폭(x축의 길이)을 setting.
	int minVertScaleRegionWidth = oneTextSize.cx * 6 + MARGIN * 3;
	if(minVertScaleRegionWidth != m_mainBlockBaseData.GetMinVertScaleRegionWidth())
		m_mainBlockBaseData.SetMinVertScaleRegionWidth(minVertScaleRegionWidth);

	// 세로 scale의 최대 폭(x축의 길이)을 setting.
	int maxVertScaleRegionWidth = oneTextSize.cx * 10 + MARGIN * 3;
	if(maxVertScaleRegionWidth != m_mainBlockBaseData.GetMaxVertScaleRegionWidth())
		m_mainBlockBaseData.SetMaxVertScaleRegionWidth(maxVertScaleRegionWidth);
}

// main block의 정보 setting
bool CMainBlockImp::SetBlockEnvironment(CDC* pDC, const CRect& allBlockRegion, const CBlockBaseData::BLOCKTYPE blockType, const CBlockBaseData::BLOCKCOMPART blockCompart, 
			const CBlockIndex& blockCount, const int graphRegionFrameThickness, const int interBlockHalfGap, const CGraphRegionColor& graphRegionColor)
{
	if(m_blocksPosition.Set(allBlockRegion, blockCount, GetDoubleSizeBlockNumber(), interBlockHalfGap, (blockCompart == CBlockBaseData::COMPARTLINE) ? 1: 0) == false)
		return false;

	m_mainBlockBaseData.GetBlockAttributes().SetBlockType(blockType);
	m_mainBlockBaseData.GetBlockAttributes().SetBlockCompartType(blockCompart);
	m_mainBlockBaseData.GetGraphAttributes().SetGraphRegionFrameThickness(graphRegionFrameThickness);
	m_mainBlockBaseData.GetGraphAttributes().SetGraphRegionColor(graphRegionColor);

	MakeBlocksColumn(blockCount);

	// 가로, 세로 scale영역의 기본 Size Setting. (min/max)
	SetBaseScaleRegion(pDC);
	return true;
}

// 화면이 현재 무슨 mode인지 설정.
void CMainBlockImp::SetDesignMode(const bool designMode)
{
	m_mainBlockBaseData.GetMainAttributes().SetDesignMode(designMode);
}

// 현재 화면에 대한 Mode를 가져온다.
bool CMainBlockImp::GetDesignMode( void) const
{
	return m_mainBlockBaseData.GetMainAttributes().IsDesignMode();
}

// messagebox 띄우기 여부
void CMainBlockImp::SetMessageBoxShow(const bool bIsShow)
{
	m_mainBlockBaseData.GetViewAttributes().SetMessageBoxShow(bIsShow);
}

// graph background gap 간격
void CMainBlockImp::SetGraphBackgroundGap(const int nGap_x, const int nGap_y)
{
	m_mainBlockBaseData.GetGraphAttributes().SetGraphBackgroundGap(nGap_x, nGap_y);
}

// block 간의 gap/2
int CMainBlockImp::GetInterBlockHalfGap( void) const
{
	return m_blocksPosition.GetInterBlockHalfGap();
}

// graph 영역 테두리 두께
int CMainBlockImp::GetGraphRegionFrameThickness( void) const
{
	return m_mainBlockBaseData.GetGraphRegionFrameThickness();
}

// graph 삭제시 해당 graph의 indicatorInfo도 삭제한다.
void CMainBlockImp::SetAutoDeleteIndicatorInfo(const bool bIsAutoDelete)
{
	m_mainBlockBaseData.GetGraphAttributes().SetAutoDeleteIndicatorInfo(bIsAutoDelete);
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 26
// Return void	: 
// Param  BOOL bShowPriceMinMaxText : Graph에 Text를 보이게 할것인지 말것인지에 대한 설정값
// Comments		: 가격차트에서 최대, 최소값을 표시하는 Text가 보이게 할 것인지
//				: 말 것인지에 대한 값을 설정한다.
//-----------------------------------------------------------------------------
void CMainBlockImp::SetShowPriceMinMaxText( BOOL bShowPriceMinMaxText )
{
	m_bShowPriceMinMaxText = bShowPriceMinMaxText;
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 26
// Return BOOL	: 
// Param  void : 
// Comments		: 가격차트에서 최대, 최소값을 표시하는 Text가 보이게 할 것인지
//				: 말 것인지에 대한 설정값을 얻어온다.
//-----------------------------------------------------------------------------
BOOL CMainBlockImp::GetShowPriceMinMaxText( void )
{
	return m_bShowPriceMinMaxText;
}
	
//////////////////////////////////////////////////////////////////////
// (2006/10/13 - Seung-Won, Bae) Manage Block Move & Delete Tool Flag for Position of Graph Drawing
//////////////////////////////////////////////////////////////////////
void CMainBlockImp::SetBlockMoveDeleteFlag( BOOL p_bBlockMoveDelete)
{
	m_bBlockMoveDelete = p_bBlockMoveDelete;
}

BOOL CMainBlockImp::GetBlockMoveDeleteFlag( void) const
{
	return m_bBlockMoveDelete;
}

// block type
CBlockBaseData::BLOCKTYPE CMainBlockImp::GetBlockType() const
{
	return m_mainBlockBaseData.GetBlockType();
}

// block 구분선 타입
CBlockBaseData::BLOCKCOMPART CMainBlockImp::GetBlockCompartType() const
{
	return m_mainBlockBaseData.GetBlockCompartType();
}

// block 수
void CMainBlockImp::SetBlockCount(const CBlockIndex& blockCount)
{
	MakeBlocksColumn(blockCount);
}

// 현재 main block에 data가 있는지를 가져온다.
bool CMainBlockImp::IsEmpty( void) const
{
	bool bEmpty = (GetColumnCount() <= 0 && GetRowCount() <= 0);
	// (2009/2/18 - Seung-Won, Bae) Check Graph!
	if( !bEmpty)
	{
		CGraphSearchInfo_AnyGraph gsi;
		bEmpty = !FindGraph( &gsi);
	}
	return bEmpty;
}

// 열의 총 갯수
int CMainBlockImp::GetColumnCount() const
{
	return m_blocksPosition.GetColumnBlockCount();
}

// 행의 총 갯수
int CMainBlockImp::GetRowCount( void) const
{
	return m_blocksPosition.GetRowBlockCount();
}

int CMainBlockImp::GetRowCountInColumn( int p_nColumnIndex) const
{
	if( m_BlocksColumnList.GetCount() <= p_nColumnIndex) return -1;
	POSITION posColumn = m_BlocksColumnList.FindIndex( p_nColumnIndex);
	if( !posColumn) return -1;
	CBlocksColumn* pBlocksColumn = ( CBlocksColumn*) m_BlocksColumnList.GetAt( posColumn);
	if( !pBlocksColumn) return -1;
	return pBlocksColumn->GetBlockCount();
}

// 화면에 추가할 수 있는 총 block 수
void CMainBlockImp::SetMaxBlocksCountInView(const int maxBlockCount)
{
	m_mainBlockBaseData.GetBlockAttributes().SetMaxBlocksCountInView(maxBlockCount);
}

// 한 화면에 보일 최대 block 갯수
int CMainBlockImp::GetMaxBlocksCountInView( void) const
{
	return m_mainBlockBaseData.GetMaxBlocksCountInView();
}

// 다른 block size보다 2배인 block 설정
void CMainBlockImp::SetDoubleSizeBlockNumber(const CBlockIndex& doubleSizeBlockIndex)
{
	// size 초기화
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		pBlocksColumn->InitializeNormalSizeBlockNumber();
	}
	m_blocksPosition.InitializeNormalSizeBlockNumber();

	CBlockImp *pBlock = ( CBlockImp *)GetBlock( doubleSizeBlockIndex);
	if( !pBlock) return;
	pBlock->GetBlockData().SetDoubleSize( true);
	m_blocksPosition.SetDoubleSizeBlockNumber(doubleSizeBlockIndex);
}

// 2배 크기 block
CBlockIndex CMainBlockImp::GetDoubleSizeBlockNumber( void) const
{
	CGraphSearchInfo_DoubleSizeBlock gsi;
	if( !FindGraph( &gsi)) return CBlockIndex( -1, -1);
	return CBlockIndex( gsi.m_nRowBlockIndex, gsi.m_nColumnBlockIndex);
}

// 2배 크기 block
int CMainBlockImp::GetDoubleSizeBlockRowNumber() const
{
	return GetDoubleSizeBlockNumber().GetRow();
}

// block의 영역 설정
void CMainBlockImp::SetBlocksPosition(const CString& blocksPosition)
{
	if(blocksPosition.IsEmpty())
		return;

	m_blocksPosition.SetBlocksPosition(blocksPosition);
}

// block의 위치를 string로 준다.
CString CMainBlockImp::GetBlocksPosition( void) const
{
	return m_blocksPosition.GetBlocksPosition();
}

// block 전체 영역
bool CMainBlockImp::SetAllBlockRegion(const CRect& allBlockRegion)
{
	// 영역의 초기화 - 등비율
	//	if( false) return m_blocksPosition.InitializeBlockRegions(allBlockRegion, GetDoubleSizeBlockNumber());
	//	else
	return m_blocksPosition.RecalculateBlockRegions(allBlockRegion);
}

// block의 전체 영역
CRect CMainBlockImp::GetAllBlockRegion( void) const
{
	return m_blocksPosition.GetAllBlockRegion();
}

// -- point에 따른 것
CRect CMainBlockImp::GetGraphRegionSumInColumn( const CPoint& point) const
{
	CBlockImp *pBlock = NULL;
	CBlocksColumn *pBlocksColumn = NULL;
	POSITION posBlockColumn = m_BlocksColumnList.GetHeadPosition();
	while( posBlockColumn)
	{
		pBlocksColumn = m_BlocksColumnList.GetNext( posBlockColumn);
		if( !pBlocksColumn) continue;

		pBlock = pBlocksColumn->GetBlock( 0);
		if( !pBlock) continue;

		CRect headBlockRegion = pBlock->GetVertScaleGroupList().GetGraphRegions().GetNoFrameRegion();

		pBlock = pBlocksColumn->GetBlock( GetRowCount() - 1);
		if( !pBlock) continue;

		CRect tailBlockRegion = pBlock->GetVertScaleGroupList().GetGraphRegions().GetNoFrameRegion();
		CRect rctColumnSum( headBlockRegion.left, headBlockRegion.top, tailBlockRegion.right, tailBlockRegion.bottom);
		if( rctColumnSum.PtInRect( point)) return rctColumnSum;
	}

	return CRect(0, 0, 0, 0);
}

CRect CMainBlockImp::GetBlockRegion(const CBlockIndex& blockIndex) const
{
	if(blockIndex.GetColumn() < 0 || blockIndex.GetColumn() >= GetColumnCount())
		return CRect(0, 0, 0, 0);
	if(blockIndex.GetRow() < 0 || blockIndex.GetRow() >= GetRowCount())
		return CRect(0, 0, 0, 0);

	return m_blocksPosition.GetBlockRegion(blockIndex);
}

CRect CMainBlockImp::GetNoFrameGraphRegion(const CBlockIndex& blockIndex) const
{
	CBlockImp *pBlock = ( CBlockImp *)GetBlock( blockIndex);
	if( !pBlock) return CRect(0, 0, 0, 0);
	
	return pBlock->GetVertScaleGroupList().GetGraphRegions().GetNoFrameRegion();
}

CRect CMainBlockImp::GetNoFrameGraphRegion(const CPoint& point) const
{
	for(int blockColumnIndex = 0; blockColumnIndex < GetColumnCount(); blockColumnIndex++)
	{
		for(int blockRowIndex = 0; blockRowIndex < GetRowCount(); blockRowIndex++)
		{
			CRect region = GetNoFrameGraphRegion( CBlockIndex( blockRowIndex, blockColumnIndex));
			if(region.PtInRect(point))
				return region;
		}
	}

	return CRect(0, 0, 0, 0);
}

// Graph 분리/삽입 - ojtaso (20070731)
CRect CMainBlockImp::GetNoFrameGraphRegion(const CPoint& point, int& nInsertCol, int& nInsertRow) const
{
	for(int blockColumnIndex = 0; blockColumnIndex < GetColumnCount(); blockColumnIndex++)
	{
		for(int blockRowIndex = 0; blockRowIndex < GetRowCount(); blockRowIndex++)
		{
			CRect region = GetNoFrameGraphRegion( CBlockIndex( blockRowIndex, blockColumnIndex));
			// (2007/8/14 - Seung-Won, Bae) Include Border.
			region.InflateRect( 0, 2);
			if(region.PtInRect(point))
			{
				region.DeflateRect( 0, 2);

				if( point.y <= region.top)
				{
					nInsertRow = blockRowIndex;
					if(nInsertRow < 0)
						nInsertRow = 0;
				}
				else if(region.bottom <= point.y)
				{
					nInsertRow = blockRowIndex + 1;
					// (2007/8/14 - Seung-Won, Bae) Do not decrease in Same Value.
					if(nInsertRow > GetRowCount())
						nInsertRow--;
				}
				
				nInsertCol = blockColumnIndex;
				return region;
			}
		}
	}

	return CRect(0, 0, 0, 0);
}

CRect CMainBlockImp::GetDrawingGraphRegion(const CBlockIndex& blockIndex) const
{
	CBlockImp* pBlock = ( CBlockImp *)GetBlock(blockIndex);
	if(pBlock == NULL)
		return CRect(0, 0, 0, 0);

	return pBlock->GetVertScaleGroupList().GetGraphRegions().GetDrawingRegion();
}

// 화면에 보일 data의 범위.
void CMainBlockImp::SetDataRegionInBlock2(const int start, const int end)
{
	if(m_mainBlockBaseData.GetDisplayAttributes() == CDisplayAttributes(start, end))
		return;

	m_mainBlockBaseData.GetViewAttributes().SetDataRegionInBlock(start, end);
	CalculateDisplayDataMinMaxAllVertScaleGroup();

	//sy 2003.3.7 - 속도 개선 - scroll 변경시 그 구역만 다시 계산
	CalculateGraph( _MTEXT( C2_OVERHANGING_SUPPLY), CCalculateBaseData::SCROLL_CAL);
	// (2007/3/21 - Seung-Won, Bae) Recalc '매수매도대기매물'
	CalculateGraph( _MTEXT( C2_BUY_SELL_OVERHANGING_SUPPLY), CCalculateBaseData::SCROLL_CAL);
}

// 화면에 보이는 data 시작/끝
void CMainBlockImp::GetDisplayDataIndex(int& nDataStartInBlock, int& nDataEndInBlock) const
{
	CDisplayAttributes displayAttributes = m_mainBlockBaseData.GetViewAttributes().GetDisplayAttributes();
	nDataStartInBlock = displayAttributes.GetDataStartInBlock();
	nDataEndInBlock = displayAttributes.GetDataEndInBlock();
}

void CMainBlockImp::SetBaseHorzScalePosition()
{
	SetAllHorzScalePosition( CScaleBaseData::HORZ_HIDE);

	int nBlockRowIndex = GetRowCount() - 1;
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		CBlockImp* pBlock = pBlocksColumn->GetBlock( nBlockRowIndex);
		if( !pBlock) continue;

		// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
		//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
		SetHorzScalePosition( pBlock->GetBlockData().GetHorzScale(), CScaleBaseData::HORZ_BOTTOM);
	}
}

void CMainBlockImp::SetOnlyPriceVertScale(CDC* pDC, const bool bIsOnlyPrice)
{
	m_mainBlockBaseData.GetScaleAttributes().SetOnlyPriceVertScale(bIsOnlyPrice);
	CalculateDataForDrawing(pDC);
}

// (2006/11/12 - Seung-Won, Bae) Support VScale Min/Max Type Change for All VSCale
void CMainBlockImp::SetAllVScaleMinMaxType( CScaleBaseData::VERTMINMAXTYPE p_eVScaleMinMaxType, CDC* pDC)
{
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		if( !pBlocksColumn) return;
		pBlocksColumn->SetAllVScaleMinMaxType( p_eVScaleMinMaxType);
	}
}

// (2006/11/14 - Seung-Won, Bae) Support VScale Min/Max Type Release in TR
void CMainBlockImp::ReleaseAllVScaleSettingInFixedMinMax( CDC *pDC)
{
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		if( !pBlocksColumn) return;
		pBlocksColumn->ReleaseAllVScaleSettingInFixedMinMax();
	}

	CalculateDataForDrawing(pDC);
}

// subGraph title 숨기기 여무 setting.
void CMainBlockImp::SetAllGraphsTitleHiding(const bool bIsAllHiding)
{
	m_mainBlockBaseData.GetGraphAttributes().SetAllGraphsTitleDrawHiding(bIsAllHiding);
}

// graph title 보이는 형태
void CMainBlockImp::SetGraphTitleDraw(const bool bIsCondition, const bool bIsData)
{
	m_mainBlockBaseData.GetGraphAttributes().SetGraphTitleDraw(bIsCondition, bIsData);
}

// (2006/12/12 - Seung-Won, Bae) Support Indicator Calculation Enabling Flag
void CMainBlockImp::EnableIndicatorCalculation( BOOL p_bEnableIndicatorCalculation)
{
	m_mainBlockBaseData.EnableIndicatorCalculation( p_bEnableIndicatorCalculation);
	CalculateAllGraphs( CCalculateBaseData::TRDATA_CAL);
}

BOOL CMainBlockImp::IsEnableIndicatorCalculation( void) const
{
	return m_mainBlockBaseData.IsEnableIndicatorCalculation();
}

// 바탕색
void CMainBlockImp::SetBackgroundColor(const COLORREF& color)
{
	m_mainBlockBaseData.GetViewAttributes().SetBackgroundColor(color);
}

// graph region에 관한 색을 가져온다.
COLORREF CMainBlockImp::GetBackgroundColor( void) const
{
	return m_mainBlockBaseData.GetGraphRegionColor().GetBackgroundColor();
}

// graph 영역 색
void CMainBlockImp::SetGraphRegionColor(const CGraphRegionColor& graphRegionColor)
{
	m_mainBlockBaseData.GetGraphAttributes().SetGraphRegionColor(graphRegionColor);
}

CGraphRegionColor CMainBlockImp::GetGraphRegionColor( void) const
{
	return m_mainBlockBaseData.GetGraphRegionColor();
}

void CMainBlockImp::GetGraphRegionColor(COLORREF& backgroundColor, COLORREF& lineColor, COLORREF& frameColor) const
{
	backgroundColor = m_mainBlockBaseData.GetGraphRegionColor().GetBackgroundColor();
	lineColor = m_mainBlockBaseData.GetGraphRegionColor().GetLineColor();
	frameColor = m_mainBlockBaseData.GetGraphRegionColor().GetFrameColor();
}

// scale 가로, 세로 grid 색, 글짜 색
void CMainBlockImp::SetScaleColor(const CScaleColor& scaleColor)
{
	m_mainBlockBaseData.GetScaleAttributes().SetScaleColor(scaleColor);
}

// scale 색
CScaleColor& CMainBlockImp::GetScaleColor()
{
	return m_mainBlockBaseData.GetScaleAttributes().GetScaleColor();
}

void CMainBlockImp::GetScaleColor(COLORREF& horzGridColor, COLORREF& vertGridColor, COLORREF& textColor) const
{
	horzGridColor	= m_mainBlockBaseData.GetScaleAttributes().GetScaleColor().GetHorzScaleColor();
	vertGridColor	= m_mainBlockBaseData.GetScaleAttributes().GetScaleColor().GetVertScaleColor();
	textColor		= m_mainBlockBaseData.GetScaleAttributes().GetScaleColor().GetScaleTextColor();
}

// 기타 graph 색 설정
void CMainBlockImp::SetEtcGraphColor(const CEtcGraphColor& etcGraphColor)
{
	m_mainBlockBaseData.GetGraphAttributes().SetEtcGraphColor(etcGraphColor);
}

// 기타 graph 색 설정
void CMainBlockImp::GetEtcGraphColor(COLORREF& graphColor1, COLORREF& graphColor2, COLORREF& graphColor3, COLORREF& graphColor4, COLORREF& graphColor5, COLORREF& graphColor6) const
{
	graphColor1 = m_mainBlockBaseData.GetGraphAttributes().GetEtcGraphColor().GetGraphColor1();
	graphColor2 = m_mainBlockBaseData.GetGraphAttributes().GetEtcGraphColor().GetGraphColor2();
	graphColor3 = m_mainBlockBaseData.GetGraphAttributes().GetEtcGraphColor().GetGraphColor3();
	graphColor4 = m_mainBlockBaseData.GetGraphAttributes().GetEtcGraphColor().GetGraphColor4();
	graphColor5 = m_mainBlockBaseData.GetGraphAttributes().GetEtcGraphColor().GetGraphColor5();
	graphColor6 = m_mainBlockBaseData.GetGraphAttributes().GetEtcGraphColor().GetGraphColor6();
}

//////////////////////////////////////////////////////////////////////
// Create
//////////////////////////////////////////////////////////////////////

// 기존의 모든 block를 삭제하고 원하는 갯수로 column별로 block를 생성.
void CMainBlockImp::MakeBlocksColumn(const CBlockIndex& blockCount)
{
	DeleteAllBlock();
	for(int column = 0; column < blockCount.GetColumn(); column++)
	{
		// list에 추가하고 column별로 block를 생성한다.
		CBlocksColumn* pBlocksColumn = CBlocksColumn::Make( this);
		int rowBlocksColumn = blockCount.GetRow();
		assert(pBlocksColumn != NULL);
		pBlocksColumn->MakeBlocksColumn(rowBlocksColumn);
		m_BlocksColumnList.AddTail(pBlocksColumn);
	}
	m_blocksPosition.SetBlockCount( blockCount, GetDoubleSizeBlockNumber());

	// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
	SetAllHorzScaleGridType( m_eLastHorzGridType);
	SetAllVertScaleGridType( m_eLastVerticalGridType);

	//KHD : 시간구분
	// 분/틱차트에서의 일자구분선 개별옵션 - onlyojt
	m_eLastDailyGridType = GetFirstDailyScaleGridType(CScaleBaseData::MINTICK_DAY_GUBUN);
	m_WeekGridType= GetFirstDailyScaleGridType(CScaleBaseData::DAYCHART_WEEK_GUBUN);
	m_MonGridType= GetFirstDailyScaleGridType(CScaleBaseData::DAYCHART_MON_GUBUN);
	m_YearGridType= GetFirstDailyScaleGridType(CScaleBaseData::DAYCHART_YEAR_GUBUN);
	//END
}

// block에 있는 기존 Graph들을 모두 삭제하고 새로 생성한다.
// BlockColumn;\r\n
// Block;\r\n
// Scale Data(char);위치(int);grid타입(int);일자타입(char);시간타입(char);text타입(int);
//		log(bool);차트뒤집기(bool);2배block여부(bool);\r\n -> text타입까지 x축 scale 부분
// VertScaleGroup;\r\n
// 사용할 Data(char)(지표 Data);위치(int);grid타입(int);unit타입(int);
// Graph;\r\n
// index(int);GraphName(char);packetNames(char);signal사용여부(bool):signal보임(bool);
//		최저가최고가등락률(bool);최저가현재가등락률(bool);최고가현재가등락률(bool);전일비(bool);현재가(int);
//		최고값보기;최저값보기;부호표시;색표현방법(0:봉색,1:빨/파,2:양(빨),음(파));\r\n
// 그래프타입(int);그래프스타일(int);그리는스타일(int);SubGraphName(char)(지표가 아닌경우 = Graph1);title(char);숨김(bool);
//		색1(char);색2(char);색3(char);색4(char);색5(char);색6(char);
//		상승채움(bool);하락채움(bool);선굵기1(int);선굵기2(int);선굵기3(int);강조(bool);\r\n
// GraphEnd;\r\n
// VertScaleGroupEnd;\r\n
// BlockEnd;\r\n
// BlockColumnEnd;\r\n
// ex) 가격차트 ---------------------------------------------------------------
// BlockColumn;\r\n
// Block;\r\n
// 자료일자;3;2;/;:;0;0;0;1;\r\n
// VertScaleGroup;\r\n
// 지표 Data;0;3;0;\r\n
// Graph;\r\n
// 0;가격차트;시가,고가,저가,종가,;0:0;1;1;1;0;1;1;1;0;0\r\n
// 0;0;0;가격;가격;0;238,0,0;238,0,0;0,0,255;0,0,255;0,119,0;0,136,0;1;0;1;1;1;0;\r\n
// GraphEnd;\r\n
// VertScaleGroupEnd;\r\n
// BlockEnd;\r\n
// BlockColumnEnd;\r\n
// 그래프 생성후 계산 여부 : xScaleManager - ojtaso (20070701)
BOOL CMainBlockImp::BuildGraphsAndScales(const CString& data, BOOL p_bWithIndicatorInfoUpdate)
{
	// 그래프 생성후 계산 여부 : xScaleManager - ojtaso (20070701)
	BOOL bCalc = FALSE;

	if( data.IsEmpty()) return bCalc;

	CString strAllBlockData;
	char *szAllBlockData = strAllBlockData.GetBufferSetLength( data.GetLength());
	strcpy( szAllBlockData, data);

	char *szBlocksColumn = strstr( szAllBlockData, "BlockColumn;\r\n");
	if( !szBlocksColumn) return bCalc;
	szBlocksColumn += 14;				// "BlockColumn;\r\n"
	char *szNextBlockColumn = strstr( szBlocksColumn, "BlockColumnEnd;\r\n");
	if( szNextBlockColumn) *szNextBlockColumn = '\0';

	CBlocksColumn* pBlocksColumn = NULL;
	POSITION posBlockColumn = m_BlocksColumnList.GetHeadPosition();
	while( posBlockColumn && szBlocksColumn)
	{
		pBlocksColumn = m_BlocksColumnList.GetNext( posBlockColumn);
		if( !pBlocksColumn) continue;

		// 그래프 생성후 계산 여부 : xScaleManager - ojtaso (20070701)
		pBlocksColumn->BuildGraphsAndScales( szBlocksColumn, bCalc);

		if( szNextBlockColumn)
		{
			*szNextBlockColumn = 'B';

			szBlocksColumn = strstr( szNextBlockColumn, "BlockColumn;\r\n");
			if( szBlocksColumn)
			{
				szBlocksColumn += 14;		// "BlockColumn;\r\n"

				szNextBlockColumn = strstr( szBlocksColumn, "BlockColumnEnd;\r\n");
				if( szNextBlockColumn) *szNextBlockColumn = '\0';
			}
		}
		else szBlocksColumn = NULL;
	}

	// (2006/8/28 - Seung-Won, Bae) Set Indicator Info with new Graph Build Info.
	if( p_bWithIndicatorInfoUpdate) ChangeAllIndicatorInfoFromGraphData();

//	waitCursor.Restore();

	// 그래프 생성후 계산 여부 : xScaleManager - ojtaso (20070701)
	return bCalc;
}

// 모든 block의 data
void CMainBlockImp::GetAllBlocksData( CString &p_strAllBlockData) const
{
	CStringList slAllBlockData;
	CBlocksColumn *pBlocksColumn = NULL;
	POSITION posColumn = m_BlocksColumnList.GetHeadPosition();
	while( posColumn)
	{
		pBlocksColumn = m_BlocksColumnList.GetNext( posColumn);
		if( !pBlocksColumn) continue;
		pBlocksColumn->GetBlocksColumnAllData( slAllBlockData);
	}

	int nAllBlockDataLen = 0;
	POSITION posAllBlockData = slAllBlockData.GetHeadPosition();
	while( posAllBlockData)
	{
		CString &strAllBlockData = slAllBlockData.GetNext( posAllBlockData);
		nAllBlockDataLen += strAllBlockData.GetLength();
	}

	char *szAllBlockData = p_strAllBlockData.GetBufferSetLength( nAllBlockDataLen);
	posAllBlockData = slAllBlockData.GetHeadPosition();
	while( posAllBlockData)
	{
		CString &strAllBlockData = slAllBlockData.GetNext( posAllBlockData);
		strcpy( szAllBlockData, strAllBlockData);
		szAllBlockData += strAllBlockData.GetLength();
	}
}

BOOL CMainBlockImp::InsertGraph_GetGraphInfo( CString &p_strGraphDataBuffer, const CString &graphDataOrg, char *&p_szGraphData,
														CGraphBaseData::GRAPHTYPE &p_eGraphType, CString &p_strIndicatorName)
{
	p_szGraphData = p_strGraphDataBuffer.GetBufferSetLength( graphDataOrg.GetLength());
	strcpy( p_szGraphData, graphDataOrg);
	
	char *szIndicatorName = strstr( p_szGraphData, "Graph;\r\n");
	if( !szIndicatorName) return FALSE;
	szIndicatorName = strstr( szIndicatorName + 8, ";");
	if( !szIndicatorName) return FALSE;
	szIndicatorName++;

	char *szGraphName = strstr( szIndicatorName, ";");
	if( !szGraphName) return FALSE;
	*szGraphName = '\0';
	p_strIndicatorName = szIndicatorName;
	*szGraphName = ';';

	CString strGraphName;
	if( strncmp( szGraphName, ";G:", 3)) strGraphName = p_strIndicatorName;
	else
	{
		szGraphName += 3;
		char *szGraphNameEnd = strstr( szGraphName, ";");
		if( !szGraphNameEnd) return FALSE;
		*szGraphNameEnd = '\0';
		strGraphName = szGraphName;
		*szGraphNameEnd = ';';
	}
	if( strGraphName.IsEmpty()) return FALSE;

	char *szGraphType = strstr( szGraphName, "\r\n");
	if( !szGraphType) return FALSE;
	szGraphType += 2;										// "\r\n"
	char *szGraphTypeEnd = strstr( szGraphType, ";");
	if( !szGraphTypeEnd) return FALSE;
	*szGraphTypeEnd = '\0';
	p_eGraphType = ( CGraphBaseData::GRAPHTYPE)atoi( szGraphType);
	*szGraphTypeEnd = ';';

	CGraphSearchInfo_GraphName gsi;
	gsi.m_strGraphName = strGraphName;
	if( FindGraph( &gsi)) return FALSE;

	return TRUE;
}

// RQ로 검색 : 복수종목 - ojtaso (20070118)
BOOL CMainBlockImp::InsertGraph_GetGraphInfo(LPVOID pPacketRQ, CString &p_strGraphDataBuffer, const CString &graphDataOrg, char *&p_szGraphData,
														CGraphBaseData::GRAPHTYPE &p_eGraphType, CString &p_strIndicatorName)
{
	p_szGraphData = p_strGraphDataBuffer.GetBufferSetLength( graphDataOrg.GetLength());
	strcpy( p_szGraphData, graphDataOrg);
	
	char *szIndicatorName = strstr( p_szGraphData, "Graph;\r\n");
	if( !szIndicatorName) return FALSE;
	szIndicatorName = strstr( szIndicatorName + 8, ";");
	if( !szIndicatorName) return FALSE;
	szIndicatorName++;

	char *szGraphName = strstr( szIndicatorName, ";");
	if( !szGraphName) return FALSE;
	*szGraphName = '\0';
	p_strIndicatorName = szIndicatorName;
	*szGraphName = ';';

	CString strGraphName;
	if( strncmp( szGraphName, ";G:", 3)) strGraphName = p_strIndicatorName;
	else
	{
		szGraphName += 3;
		char *szGraphNameEnd = strstr( szGraphName, ";");
		if( !szGraphNameEnd) return FALSE;
		*szGraphNameEnd = '\0';
		strGraphName = szGraphName;
		*szGraphNameEnd = ';';
	}
	if( strGraphName.IsEmpty()) return FALSE;

	char *szGraphType = strstr( szGraphName, "\r\n");
	if( !szGraphType) return FALSE;
	szGraphType += 2;										// "\r\n"
	char *szGraphTypeEnd = strstr( szGraphType, ";");
	if( !szGraphTypeEnd) return FALSE;
	*szGraphTypeEnd = '\0';
	p_eGraphType = ( CGraphBaseData::GRAPHTYPE)atoi( szGraphType);
	*szGraphTypeEnd = ';';

	CGraphSearchInfo_GraphName gsi;
	gsi.m_strGraphName = strGraphName;
	if( FindGraph(pPacketRQ, &gsi)) return FALSE;

	return TRUE;
}

BOOL CMainBlockImp::InsertGraph_InsertIndicatorPosition( const CGraphBaseData::GRAPHTYPE p_eGraphType, const char *p_szIndicatorName, char *p_szGraphData)
{
	// 가격차트 - scale group가 같은 경우.
	if( p_eGraphType == CGraphBaseData::DataView_Type || g_iMetaTable.IsScaleGroup_Price( p_szIndicatorName))
	{
		CGraphSearchInfo_IndicatorName gsi;
		gsi.m_strIndicatorName = _MTEXT( C2_PRICE_CHART);
		if( FindGraph( &gsi))
			if( gsi.m_pVertScaleGroup)
				// graph에 따라 vertScaleGroup가 추가되는 위치(head or tail)가 틀리다.
				if( gsi.m_pVertScaleGroup->MakeGraph( p_szGraphData, !g_iMetaTable.IsVolumeForSale( p_szIndicatorName)))
					return TRUE;
	}

	// 가격차트 - scale group가 다 경우.
	if( g_iMetaTable.IsScaleGroup_PriceNoVertScale( p_szIndicatorName))
	{
		// 가격차트 scale을 다르게 쓰는 경우
		CGraphSearchInfo_IndicatorName gsi;
		gsi.m_strIndicatorName = _MTEXT( C2_PRICE_CHART);
		if( FindGraph( &gsi)) if( gsi.m_pBlockImp)
		{
			gsi.m_pBlockImp->GetVertScaleGroupList().BuildVertScaleGroups( p_szGraphData, false);
			return TRUE;
		}
	}

	// 거래량차트
	if( g_iMetaTable.IsScaleGroup_Volume( p_szIndicatorName))
	{
		// <거래량차트>
		CGraphSearchInfo_IndicatorName gsi;
		gsi.m_strIndicatorName = _MTEXT( C2_VOLUME_CHART);
		BOOL bFound = FindGraph( &gsi);
		if( !bFound)
		{
			gsi.ResetInfo();
			// <거래량 이동평균>
			gsi.m_strIndicatorName = _MTEXT( C2_VOLUME_MA);
			bFound = FindGraph( &gsi);
		}
		if( bFound)
			 if( gsi.m_pVertScaleGroup)
				 if( gsi.m_pVertScaleGroup->MakeGraph( p_szGraphData))
					 return TRUE;
	}

	return FALSE;
}

// RQ로 검색 : 복수종목 - ojtaso (20070118)
BOOL CMainBlockImp::InsertGraph_InsertIndicatorPosition(LPVOID pPacketRQ, const CGraphBaseData::GRAPHTYPE p_eGraphType, const char *p_szIndicatorName, char *p_szGraphData)
{
	// 가격차트 - scale group가 같은 경우.
	if( p_eGraphType == CGraphBaseData::DataView_Type || g_iMetaTable.IsScaleGroup_Price( p_szIndicatorName))
	{
		CGraphSearchInfo_IndicatorName gsi;
		gsi.m_strIndicatorName = _MTEXT( C2_PRICE_CHART);
		if( FindGraph(pPacketRQ, &gsi))
			if( gsi.m_pVertScaleGroup)
				// graph에 따라 vertScaleGroup가 추가되는 위치(head or tail)가 틀리다.
				if( gsi.m_pVertScaleGroup->MakeGraph(pPacketRQ, p_szGraphData, !g_iMetaTable.IsVolumeForSale( p_szIndicatorName)))
					return TRUE;
	}

	// 가격차트 - scale group가 다 경우.
	if( g_iMetaTable.IsScaleGroup_PriceNoVertScale( p_szIndicatorName))
	{
		// 가격차트 scale을 다르게 쓰는 경우
		CGraphSearchInfo_IndicatorName gsi;
		gsi.m_strIndicatorName = _MTEXT( C2_PRICE_CHART);
		if( FindGraph(pPacketRQ, &gsi)) if( gsi.m_pBlockImp)
		{
			gsi.m_pBlockImp->GetVertScaleGroupList().BuildVertScaleGroups( p_szGraphData, false);
			return TRUE;
		}
	}

	// 거래량차트
	if( g_iMetaTable.IsScaleGroup_Volume( p_szIndicatorName))
	{
		// <거래량차트>
		CGraphSearchInfo_IndicatorName gsi;
		gsi.m_strIndicatorName = _MTEXT( C2_VOLUME_CHART);
		BOOL bFound = FindGraph(pPacketRQ, &gsi);
		if( !bFound)
		{
			gsi.ResetInfo();
			// <거래량 이동평균>
			gsi.m_strIndicatorName = _MTEXT( C2_VOLUME_MA);
			bFound = FindGraph(pPacketRQ, &gsi);
		}
		if( bFound)
			 if( gsi.m_pVertScaleGroup)
				 if( gsi.m_pVertScaleGroup->MakeGraph(pPacketRQ, p_szGraphData))
					 return TRUE;
	}

	return FALSE;
}
	 
BOOL CMainBlockImp::InsertGraph_ModifyBlock( char *p_szGraphData, const bool doesMaintainOldScale)
{
	CBlockIndex modifyPosition( GetRowCount() -1, GetColumnCount() -1);
	if( !modifyPosition.IsAllValueBiggerThanZero()) return FALSE;

	// modify 할 block에 <가격차트> 가 있는 경우
	CBlockImp *pBlock = ( CBlockImp *)GetBlock( modifyPosition);
	if( !pBlock) return FALSE;

	ML_SET_LANGUAGE_RES();
	CString strMsg;

	CGraphSearchInfo_IndicatorNameWithRQ gsi;
	gsi.m_strIndicatorName	= _MTEXT( C2_PRICE_CHART);
	gsi.m_strRQ				= "DEFAULT";
	if( pBlock->GetVertScaleGroupList().FindGraph( &gsi))
	{
		strMsg.LoadString( IDS_CANNOT_MODIFY_PRICE_CHART_BLOCK);
		AfxMessageBox( strMsg, MB_ICONWARNING|MB_OK);
		return FALSE;
	}

	// modify 할 block에 <가격차트> 가 없는 경우
	CString message;
	strMsg.LoadString( IDS_DO_YOU_WANT_NTH_BLOCK_CHANGE);
	if( m_mainBlockBaseData.GetBlockType() == CBlockBaseData::ROW)
		message.Format( strMsg, modifyPosition.GetRow() +1);
	else if(m_mainBlockBaseData.GetBlockType() == CBlockBaseData::COLUMN)
		message.Format( strMsg, modifyPosition.GetColumn() +1);
	else return FALSE;

	if( m_mainBlockBaseData.GetViewAttributes().DoesMessageBoxShow())
	{
		CString strMessage;
		strMessage.LoadString( IDS_TOO_MANY_BLOCK_COUNT);
		strMessage += message;

		if( m_mainBlockBaseData.GetParent_Handle() == NULL)
		{
			if( IDYES != AfxMessageBox(strMessage, MB_ICONQUESTION | MB_YESNO))
				return FALSE;
		}
		else
		{
			strMsg.LoadString( IDS_SELECT);
			if( IDYES != MessageBox(m_mainBlockBaseData.GetParent_Handle(), strMessage, strMsg, MB_SYSTEMMODAL | MB_ICONQUESTION | MB_YESNO))
				return FALSE;
		}
	}

	pBlock->GetVertScaleGroupList().RebuildVertScaleGroups( p_szGraphData, doesMaintainOldScale);
	return true;
}

// RQ로 검색 : 복수종목 - ojtaso (20070118)
BOOL CMainBlockImp::InsertGraph_ModifyBlock(LPVOID pPacketRQ, char *p_szGraphData, const bool doesMaintainOldScale)
{
	CBlockIndex modifyPosition( GetRowCount() -1, GetColumnCount() -1);
	if( !modifyPosition.IsAllValueBiggerThanZero()) return FALSE;

	// modify 할 block에 <가격차트> 가 있는 경우
	CBlockImp *pBlock = ( CBlockImp *)GetBlock( modifyPosition);
	if( !pBlock) return FALSE;

	ML_SET_LANGUAGE_RES();
	CString strMsg;

	CGraphSearchInfo_IndicatorName gsi;
	gsi.m_strIndicatorName = _MTEXT( C2_PRICE_CHART);
	if( pBlock->GetVertScaleGroupList().FindGraph(pPacketRQ, &gsi))
	{
		strMsg.LoadString( IDS_CANNOT_MODIFY_PRICE_CHART_BLOCK);
		AfxMessageBox( strMsg, MB_ICONWARNING|MB_OK);
		return FALSE;
	}

	// modify 할 block에 <가격차트> 가 없는 경우
	CString message;
	strMsg.LoadString( IDS_DO_YOU_WANT_NTH_BLOCK_CHANGE);
	if( m_mainBlockBaseData.GetBlockType() == CBlockBaseData::ROW)
		message.Format( strMsg, modifyPosition.GetRow() +1);
	else if(m_mainBlockBaseData.GetBlockType() == CBlockBaseData::COLUMN)
		message.Format( strMsg, modifyPosition.GetColumn() +1);
	else return FALSE;

	if( m_mainBlockBaseData.GetViewAttributes().DoesMessageBoxShow())
	{
		CString strMessage;
		strMessage.LoadString( IDS_TOO_MANY_BLOCK_COUNT);
		strMessage += message;

		if( m_mainBlockBaseData.GetParent_Handle() == NULL)
		{
			if( IDYES != AfxMessageBox(strMessage, MB_ICONQUESTION | MB_YESNO))
				return FALSE;
		}
		else
		{
			strMsg.LoadString( IDS_SELECT);
			if( IDYES != MessageBox(m_mainBlockBaseData.GetParent_Handle(), strMessage, strMsg, MB_SYSTEMMODAL | MB_ICONQUESTION | MB_YESNO))
				return FALSE;
		}
	}

	pBlock->GetVertScaleGroupList().RebuildVertScaleGroups( p_szGraphData, doesMaintainOldScale);
	return true;
}

// 사용할 data;위치;gridType;unitType;\r\n
void CMainBlockImp::InsertGraph_InsertBlock_ChangeVertScaleGroupData_Get( CString &p_strVertScaleData, CBlocksColumn* pBlocksColumn, const char *p_szScaleData, BOOL p_bPriceChartInsert) const
{
	assert(pBlocksColumn != NULL);

	// 사용할 data
	CString strScaleDataName;
	const char *szScaleDataName = _MTEXT( C6_INDICATOR_DATA);
	if( p_szScaleData) if( *p_szScaleData)
	{
		int nScaleDataLen = strlen( p_szScaleData);
		char *szScaleDataName2 = strScaleDataName.GetBufferSetLength( nScaleDataLen);
		strcpy( szScaleDataName2, p_szScaleData);
		if( szScaleDataName2[ nScaleDataLen - 1] == ';') szScaleDataName2[ nScaleDataLen - 1] = '\0';
		szScaleDataName = szScaleDataName2;
	}

	// (2007/7/3 - Seung-Won, Bae) Use Actived Grid VScaleGroup;
	CVerticalScale vertScale;
	CGraphSearchInfo_IndicatorName gsi;
	gsi.m_strIndicatorName = _MTEXT( C2_PRICE_CHART);
	CVertScaleGroup *pVertScaleGroup = GetVertScaleGroup( CBlockIndex( 0, 0), 0);
	if( FindGraph( &gsi)) if( gsi.m_pVertScaleGroup) pVertScaleGroup = gsi.m_pVertScaleGroup;
	if( pVertScaleGroup) vertScale = pVertScaleGroup->GetVertScale();

	// (2008/3/7 - Seung-Won, Bae) Min/Max Setting Sync Rules.
	//		- Sync 1. Up/Down space of All Price Chart
	//		- Sync 2. View or Full Min/Max setting of all chart.
	vertScale.SetVertScaleMinMaxLock( FALSE);
	if( CScaleBaseData::VERT_MINMAX_OF_USER == vertScale.GetVertScaleMinMaxType())
		vertScale.SetVertScaleMinMaxType( CScaleBaseData::VERT_MINMAX_OF_VIEW);
	if( !p_bPriceChartInsert) vertScale.SetSpace( 0, 0);

	vertScale.GetScaleAllData( p_strVertScaleData, szScaleDataName);
}

BOOL CMainBlockImp::InsertGraph_InsertBlock_ChangeVertScaleGroupData( CString &p_strVertScaleGroupData, CBlocksColumn* pBlocksColumn, const CString& graphData) const
{
	p_strVertScaleGroupData.Empty();
	if( graphData.IsEmpty()) return FALSE;

	CString strGraphData;
	char *szAllGraphData = strGraphData.GetBufferSetLength( graphData.GetLength());
	strcpy( szAllGraphData, graphData);

	BOOL bPriceChartInsert = ( 0 <= graphData.Find( _MTEXT( C2_PRICE_CHART)));

	int nVertScaleGroupData = 0;
	char *szGraphData = strstr( szAllGraphData, "VertScaleGroup;\r\n");
	if( !szGraphData)
	{
		CString vertScaleData;
		InsertGraph_InsertBlock_ChangeVertScaleGroupData_Get( vertScaleData, pBlocksColumn, NULL, bPriceChartInsert);

		nVertScaleGroupData += 17;							// "VertScaleGroup;\r\n"
		nVertScaleGroupData += vertScaleData.GetLength();
		nVertScaleGroupData += graphData.GetLength();
		nVertScaleGroupData += 20;							// "VertScaleGroupEnd;\r\n"

		char *szVertScaleGroupData = p_strVertScaleGroupData.GetBufferSetLength( nVertScaleGroupData);

		strcpy( szVertScaleGroupData, "VertScaleGroup;\r\n");
		szVertScaleGroupData += 17;

		strcpy( szVertScaleGroupData, vertScaleData);
		szVertScaleGroupData += vertScaleData.GetLength();

		strcpy( szVertScaleGroupData, graphData);
		szVertScaleGroupData += graphData.GetLength();

		strcpy( szVertScaleGroupData, "VertScaleGroupEnd;\r\n");
		szVertScaleGroupData += 20;
		return TRUE;
	}

	CStringList		slVertScaleData;
	CString			strScaleData;
	CVerticalScale	vertScale;

	char *szNextGraph = NULL;
	while( szGraphData)
	{
		szGraphData += 17; // "VertScaleGroup;\r\n"

		// Get Vert Scale Data Name
		szNextGraph = strstr( szGraphData, "\r\n");		
		if( !szNextGraph) break;
		*szNextGraph = '\0';
		vertScale.SetScaleData( szGraphData, m_mainBlockBaseData.GetParent_Handle());
		InsertGraph_InsertBlock_ChangeVertScaleGroupData_Get( strScaleData, pBlocksColumn, vertScale.GetScaleData(), bPriceChartInsert);

		szGraphData = szNextGraph + 2;	// "\r\n"
		szNextGraph = strstr( szGraphData, "VertScaleGroupEnd;\r\n");
		if( szNextGraph) *szNextGraph++ = '\0';

		slVertScaleData.AddTail( "VertScaleGroup;\r\n");
		nVertScaleGroupData += 17;
		slVertScaleData.AddTail( strScaleData);
		nVertScaleGroupData += strScaleData.GetLength();
		slVertScaleData.AddTail( szGraphData);
		nVertScaleGroupData += strlen( szGraphData);
		slVertScaleData.AddTail( "VertScaleGroupEnd;\r\n");
		nVertScaleGroupData += 20;

		if( !szNextGraph) szGraphData = NULL;
		else szGraphData = strstr( szNextGraph, "VertScaleGroup;\r\n");
	}

	if( !nVertScaleGroupData) return FALSE;
	char *szVertScaleGroupData = p_strVertScaleGroupData.GetBufferSetLength( nVertScaleGroupData);
	
	POSITION pos = slVertScaleData.GetHeadPosition();
	while( pos)
	{
		CString &strData = slVertScaleData.GetNext( pos);
		strcpy( szVertScaleGroupData, strData);
		szVertScaleGroupData += strData.GetLength();
	}

	return TRUE;
}

BOOL CMainBlockImp::InsertGraph_InsertBlock_InsertRow( const CBlockIndex& insertPosition, const CString& graphData)
{
	CBlocksColumn* pBlocksColumn = GetBlocksColumn( insertPosition.GetColumn());
	if( !pBlocksColumn) return FALSE;

	int nInsertRowPosition = insertPosition.GetRow();

	// Block을 추가한다.
	m_mainBlockBaseData.SetMessageBoxUsed( FALSE);
	// 블럭 최대화 추가 - ojtaso (20080519)
	if( GetMaximizedBlock().IsAllValueBiggerThanZero())
		SetMaximizedBlock( CBlockIndex( -1, -1));

	if( !pBlocksColumn->InsertBlocks( nInsertRowPosition, graphData)) return FALSE;

	CBlockImp* pNewBottomBlock = pBlocksColumn->GetBlock( nInsertRowPosition);
	if( !pNewBottomBlock) return FALSE;

	// (2007/6/28 - Seung-Won, Bae) Support the Auto Arrange in Only No-Dialog.
	if( !m_mainBlockBaseData.DoesMessageBoxShow() || !m_mainBlockBaseData.IsMessageBoxUsed())
	{
		// 1. Insert before the other Block.
		if( nInsertRowPosition < GetRowCount() - 1)
		{
			// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
			//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
			SetHorzScalePosition( pNewBottomBlock->GetBlockData().GetHorzScale(), CScaleBaseData::HORZ_HIDE);
		}

		// 2. Insert after all other Block.
		// 지표 추가시 HorizontalScale을 유지하는 부분을 수정.
		// 현재 추가된 위치가 하단 추가 Block인지 확인한다.
		else if( 0 < nInsertRowPosition)
		{
			// 만약 바닥에 추가했다면 그 이전의 Horizontal Scale에 따라 Scale을 조정한다.
			// 추가되기전 가장 마지막 HorizontalScale을 찾는다.
			CBlockImp* pOldBottomBlock = pBlocksColumn->GetBlock( nInsertRowPosition - 1);
			if( !pOldBottomBlock) return FALSE;

			switch( pOldBottomBlock->GetBlockData().GetHorzScale().GetHorzScalePosition())
			{
				case CScaleBaseData::HORZ_BOTH:
				{
					// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
					//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
					SetHorzScalePosition( pOldBottomBlock->GetBlockData().GetHorzScale(), CScaleBaseData::HORZ_TOP);
					SetHorzScalePosition( pNewBottomBlock->GetBlockData().GetHorzScale(), CScaleBaseData::HORZ_BOTTOM);
					break;
				}
				case CScaleBaseData::HORZ_BOTTOM:
				{
					// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
					//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
					SetHorzScalePosition( pOldBottomBlock->GetBlockData().GetHorzScale(), CScaleBaseData::HORZ_HIDE);
					SetHorzScalePosition( pNewBottomBlock->GetBlockData().GetHorzScale(), CScaleBaseData::HORZ_BOTTOM);
					break;
				}
				default:
				{
					// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
					//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
					SetHorzScalePosition( pNewBottomBlock->GetBlockData().GetHorzScale(), CScaleBaseData::HORZ_HIDE);
					break;
				}
			}
		}
		else
		{
			// (2006/12/24 - Seung-Won, Bae) the First Row inserted.
			//	It will use the Default Value.
		}
	}

	return m_blocksPosition.SetRowBlockCount( pBlocksColumn->GetBlockCount(), GetDoubleSizeBlockRowNumber());
}

// RQ로 검색 : 복수종목 - ojtaso (20070118)
BOOL CMainBlockImp::InsertGraph_InsertBlock_InsertRow(LPVOID pPacketRQ, const CBlockIndex& insertPosition, const CString& graphData)
{
	CBlocksColumn* pBlocksColumn = GetBlocksColumn( insertPosition.GetColumn());
	if( !pBlocksColumn) return FALSE;

	int nInsertRowPosition = insertPosition.GetRow();

	// Block을 추가한다.
	if( !pBlocksColumn->InsertBlocks(pPacketRQ, nInsertRowPosition, graphData)) return FALSE;

	// 블럭 최대화 추가 - ojtaso (20080519)
	if( GetMaximizedBlock().IsAllValueBiggerThanZero())
		SetMaximizedBlock( CBlockIndex( -1, -1));

	CBlockImp* pNewBottomBlock = pBlocksColumn->GetBlock( nInsertRowPosition);
	if( !pNewBottomBlock) return FALSE;

	if( nInsertRowPosition < GetRowCount() - 1)
	{
		// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
		//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
		SetHorzScalePosition( pNewBottomBlock->GetBlockData().GetHorzScale(), CScaleBaseData::HORZ_HIDE);
	}

	// Sang-Woo, Cho		2006 / 10 / 16
	// 지표 추가시 HorizontalScale을 유지하는 부분을 수정.
	// 현재 추가된 위치가 하단 추가 Block인지 확인한다.
	else if( 0 < nInsertRowPosition)
	{
		// 만약 바닥에 추가했다면 그 이전의 Horizontal Scale에 따라 Scale을 조정한다.
		// 추가되기전 가장 마지막 HorizontalScale을 찾는다.
		CBlockImp* pOldBottomBlock = pBlocksColumn->GetBlock( nInsertRowPosition - 1);
		if( !pOldBottomBlock) return FALSE;

		switch( pOldBottomBlock->GetBlockData().GetHorzScale().GetHorzScalePosition())
		{
			case CScaleBaseData::HORZ_BOTH:
			{
				// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
				//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
				SetHorzScalePosition( pOldBottomBlock->GetBlockData().GetHorzScale(), CScaleBaseData::HORZ_TOP);
				SetHorzScalePosition( pNewBottomBlock->GetBlockData().GetHorzScale(), CScaleBaseData::HORZ_BOTTOM);
				break;
			}
			case CScaleBaseData::HORZ_BOTTOM:
			{
				// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
				//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
				SetHorzScalePosition( pOldBottomBlock->GetBlockData().GetHorzScale(), CScaleBaseData::HORZ_HIDE);
				SetHorzScalePosition( pNewBottomBlock->GetBlockData().GetHorzScale(), CScaleBaseData::HORZ_BOTTOM);
				break;
			}
			default:
			{
				// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
				//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
				SetHorzScalePosition( pNewBottomBlock->GetBlockData().GetHorzScale(), CScaleBaseData::HORZ_HIDE);
				break;
			}
		}
	}
	else
	{
		// (2006/12/24 - Seung-Won, Bae) the First Row inserted.
		//	It will use the Default Value.
	}

	return m_blocksPosition.SetRowBlockCount( pBlocksColumn->GetBlockCount(), GetDoubleSizeBlockRowNumber());
}

BOOL CMainBlockImp::InsertGraph_InsertBlock_InsertColumn(const int nInsertPosition, const CString& graphData)
{
	if(!graphData.IsEmpty())
		return false;

	CBlocksColumn *pNewBlocksColumn = CBlocksColumn::Make( this);
	assert(pNewBlocksColumn != NULL);

	POSITION psnBlockColumn = m_BlocksColumnList.FindIndex( nInsertPosition);
	m_BlocksColumnList.InsertAfter( psnBlockColumn, pNewBlocksColumn);

	// block 추가
	bool bResult = pNewBlocksColumn->InsertBlocks(graphData);

	// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
	CScaleBaseData::SCALEGRIDTYPE eGridType = CScaleBaseData::SHORTDOTTEDLINE;
	if( psnBlockColumn)
	{
		CBlocksColumn *pPreBlocksColumn = m_BlocksColumnList.GetAt( psnBlockColumn);
		if( pPreBlocksColumn) eGridType = pPreBlocksColumn->GetFirstBlockVGridLineType();
	}
	pNewBlocksColumn->SetAllScaleGridType( eGridType);

	// block 위치 재설정 -> column 이 추가 되었기 때문에 위치를 재설정해줘야 한다.
	m_blocksPosition.SetBlockCount( CBlockIndex( pNewBlocksColumn->GetBlockCount(), m_BlocksColumnList.GetCount()), GetDoubleSizeBlockNumber());
	return bResult;
}

// RQ로 검색 : 복수종목 - ojtaso (20070118)
BOOL CMainBlockImp::InsertGraph_InsertBlock_InsertColumn(LPVOID pPacketRQ, const int nInsertPosition, const CString& graphData)
{
	if(!graphData.IsEmpty())
		return false;

	CBlocksColumn* pNewBlocksColumn = CBlocksColumn::Make( this);
	assert(pNewBlocksColumn != NULL);

	POSITION psnBlockColumn = m_BlocksColumnList.FindIndex( nInsertPosition);
	m_BlocksColumnList.InsertAfter( psnBlockColumn, pNewBlocksColumn);

	// block 추가
	bool bResult = pNewBlocksColumn->InsertBlocks(pPacketRQ, graphData);

	// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
	CScaleBaseData::SCALEGRIDTYPE eGridType = CScaleBaseData::SHORTDOTTEDLINE;
	if( psnBlockColumn)
	{
		CBlocksColumn *pPreBlocksColumn = m_BlocksColumnList.GetAt( psnBlockColumn);
		if( pPreBlocksColumn) eGridType = pPreBlocksColumn->GetFirstBlockVGridLineType();
	}
	pNewBlocksColumn->SetAllScaleGridType( eGridType);

	// block 위치 재설정 -> column 이 추가 되었기 때문에 위치를 재설정해줘야 한다.
	m_blocksPosition.SetBlockCount( CBlockIndex( pNewBlocksColumn->GetBlockCount(), m_BlocksColumnList.GetCount()), GetDoubleSizeBlockNumber());
	return bResult;
}

BOOL CMainBlockImp::InsertGraph_InsertBlock( const CString &p_strGraphData, const bool doesMaintainOldScale)
{
	CBlockIndex insertPosition( GetRowCount(), GetColumnCount() -1);
	if( !insertPosition.IsAllValueBiggerThanZero()) return FALSE;

	// scale 유지에 따른 data 변경
	if( !doesMaintainOldScale) 
	{
		// (2006/12/19 - Seung-Won, Bae) Make Full Block Data with Graph Data. (It is not used. and What is it?)
		//	CHorizontalScale horszScale;
		//	return horszScale.GetScaleAllData() + "0;0;\r\n" + graphData;
		AfxMessageBox( "Not Supported Graph Inserting!");
		return FALSE;
	}

	if( m_BlocksColumnList.IsEmpty()) return FALSE;
	CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetHead();
	if( !pBlocksColumn) return FALSE;

	CString strHorzScaleData;
	CHorizontalScale horzScale;
	CBlockImp *pBlock = pBlocksColumn->GetBlock( 0);
	if( pBlock) horzScale = pBlock->GetBlockData().GetHorzScale();
	// ( 2006 / 07 / 25 - Sang-Woo, Cho )
	// Block이 없거나 하나의 Block이 존재할 때에만 현재의 지표의 HorzScale을 HIDE로 하게되어있었
	// 는데 이 부분을 Block의 개수와 상관없이 존재하는 모든 지표의 HorzScale을 Hide로 바꿔주게 수정함.
	// 원래 소스는 추가되는 Block이 이미 존재하던 Block의 중간에 추가되게 되어있었는데 추가되는 Block이
	// 바닥에 추가되게 수정하면서 이미 존재하던 Block의 HorzScale을 HIDE로 바꾸어주게 수정한 것임.
	//if(pBlocksColumn->GetBlockCount() <= 1)
	if( m_mainBlockBaseData.GetBlockType() == CBlockBaseData::ROW && pBlocksColumn->GetBlockCount() < 1)
	{
		// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
		//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
		SetHorzScalePosition( horzScale, CScaleBaseData::HORZ_HIDE);
	}
	horzScale.GetScaleAllData( strHorzScaleData);

	CString strVertScaleGroupData;
	InsertGraph_InsertBlock_ChangeVertScaleGroupData( strVertScaleGroupData, pBlocksColumn, p_strGraphData);

	int nBlockData = 0;
	nBlockData += 8;									// "Block;\r\n"
	nBlockData += strHorzScaleData.GetLength();
	nBlockData += 6;									// "0;0;\r\n"
	nBlockData += strVertScaleGroupData.GetLength();
	nBlockData += 11;									// "BlockEnd;\r\n"

	CString changeData;
	char *szBlockData = changeData.GetBufferSetLength( nBlockData);

	strcpy( szBlockData, "Block;\r\n");
	szBlockData += 8;
	
	strcpy( szBlockData, strHorzScaleData);
	szBlockData += strHorzScaleData.GetLength();

	strcpy( szBlockData, "0;0;\r\n");
	szBlockData += 6;
	
	strcpy( szBlockData, strVertScaleGroupData);
	szBlockData += strVertScaleGroupData.GetLength();

	strcpy( szBlockData, "BlockEnd;\r\n");
	szBlockData += 11;

	if( m_mainBlockBaseData.GetBlockType() == CBlockBaseData::ROW)
		return InsertGraph_InsertBlock_InsertRow( insertPosition, changeData);
	else if( m_mainBlockBaseData.GetBlockType() == CBlockBaseData::COLUMN)
		return InsertGraph_InsertBlock_InsertColumn( insertPosition.GetColumn(), changeData);

	return FALSE;
}

// RQ로 검색 : 복수종목 - ojtaso (20070118)
BOOL CMainBlockImp::InsertGraph_InsertBlock(LPVOID pPacketRQ, const CString &p_strGraphData, const bool doesMaintainOldScale)
{
	CBlockIndex insertPosition( GetRowCount(), GetColumnCount() -1);
	if( !insertPosition.IsAllValueBiggerThanZero()) return FALSE;

	// scale 유지에 따른 data 변경
	if( !doesMaintainOldScale) 
	{
		// (2006/12/19 - Seung-Won, Bae) Make Full Block Data with Graph Data. (It is not used. and What is it?)
		//	CHorizontalScale horszScale;
		//	return horszScale.GetScaleAllData() + "0;0;\r\n" + graphData;
		AfxMessageBox( "Not Supported Graph Inserting!");
		return FALSE;
	}

	if( m_BlocksColumnList.IsEmpty()) return FALSE;
	CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetHead();
	if( !pBlocksColumn) return FALSE;

	CString strHorzScaleData;
	CHorizontalScale horzScale;
	CBlockImp *pBlock = pBlocksColumn->GetBlock( 0);
	if( pBlock) horzScale = pBlock->GetBlockData().GetHorzScale();
	// ( 2006 / 07 / 25 - Sang-Woo, Cho )
	// Block이 없거나 하나의 Block이 존재할 때에만 현재의 지표의 HorzScale을 HIDE로 하게되어있었
	// 는데 이 부분을 Block의 개수와 상관없이 존재하는 모든 지표의 HorzScale을 Hide로 바꿔주게 수정함.
	// 원래 소스는 추가되는 Block이 이미 존재하던 Block의 중간에 추가되게 되어있었는데 추가되는 Block이
	// 바닥에 추가되게 수정하면서 이미 존재하던 Block의 HorzScale을 HIDE로 바꾸어주게 수정한 것임.
	//if(pBlocksColumn->GetBlockCount() <= 1)
	if( m_mainBlockBaseData.GetBlockType() == CBlockBaseData::ROW && pBlocksColumn->GetBlockCount() < 1)
	{
		// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
		//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
		SetHorzScalePosition( horzScale, CScaleBaseData::HORZ_HIDE);
	}
	horzScale.GetScaleAllData( strHorzScaleData);

	CString strVertScaleGroupData;
	InsertGraph_InsertBlock_ChangeVertScaleGroupData( strVertScaleGroupData, pBlocksColumn, p_strGraphData);

	int nBlockData = 0;
	nBlockData += 8;									// "Block;\r\n"
	nBlockData += strHorzScaleData.GetLength();
	nBlockData += 6;									// "0;0;\r\n"
	nBlockData += strVertScaleGroupData.GetLength();
	nBlockData += 11;									// "BlockEnd;\r\n"

	CString changeData;
	char *szBlockData = changeData.GetBufferSetLength( nBlockData);

	strcpy( szBlockData, "Block;\r\n");
	szBlockData += 8;
	
	strcpy( szBlockData, strHorzScaleData);
	szBlockData += strHorzScaleData.GetLength();

	strcpy( szBlockData, "0;0;\r\n");
	szBlockData += 6;
	
	strcpy( szBlockData, strVertScaleGroupData);
	szBlockData += strVertScaleGroupData.GetLength();

	strcpy( szBlockData, "BlockEnd;\r\n");
	szBlockData += 11;

	if( m_mainBlockBaseData.GetBlockType() == CBlockBaseData::ROW)
		return InsertGraph_InsertBlock_InsertRow(pPacketRQ, insertPosition, changeData);
	else if( m_mainBlockBaseData.GetBlockType() == CBlockBaseData::COLUMN)
		return InsertGraph_InsertBlock_InsertColumn(pPacketRQ, insertPosition.GetColumn(), changeData);

	return FALSE;
}

BOOL CMainBlockImp::InsertGraph(const CString& graphData, const CGraphBaseData::ObjectChange blockType, const bool doesMaintainOldScale)
{
	// 현재 존재하는 block의 수가 0 이면 insert 하지 않는다!!
	if( graphData.IsEmpty() || GetColumnCount() * GetRowCount() <= 0) return FALSE;

	// 바둑판 모양일 경우엔 변경이 없다!!
	if( m_mainBlockBaseData.GetBlockType() != CBlockBaseData::ROW && m_mainBlockBaseData.GetBlockType() != CBlockBaseData::COLUMN) return FALSE;

	char *szGraphData = NULL;
	CString strGraphDataBuffer, strIndicatorName;
	CGraphBaseData::GRAPHTYPE eGraphType = CGraphBaseData::GTNONE;
	if( !InsertGraph_GetGraphInfo( strGraphDataBuffer, graphData, szGraphData, eGraphType, strIndicatorName)) return FALSE;

	if( InsertGraph_InsertIndicatorPosition( eGraphType, strIndicatorName, szGraphData)) return true;

	if(blockType == CGraphBaseData::MODIFY || m_mainBlockBaseData.GetMaxBlocksCountInView() <= GetColumnCount() * GetRowCount())
		return InsertGraph_ModifyBlock( szGraphData, doesMaintainOldScale);

	// 마지막에 block 추가 (graph 추가)
	assert( blockType == CGraphBaseData::INSERT);
	return InsertGraph_InsertBlock( graphData, doesMaintainOldScale);
}

// RQ로 검색 : 복수종목 - ojtaso (20070118)
BOOL CMainBlockImp::InsertGraph(LPVOID pPacketRQ, const CString& graphData, const CGraphBaseData::ObjectChange blockType, const bool doesMaintainOldScale)
{
	// 현재 존재하는 block의 수가 0 이면 insert 하지 않는다!!
	if( graphData.IsEmpty() || GetColumnCount() * GetRowCount() <= 0) return FALSE;

	// 바둑판 모양일 경우엔 변경이 없다!!
	if( m_mainBlockBaseData.GetBlockType() != CBlockBaseData::ROW && m_mainBlockBaseData.GetBlockType() != CBlockBaseData::COLUMN) return FALSE;

	char *szGraphData = NULL;
	CString strGraphDataBuffer, strIndicatorName;
	CGraphBaseData::GRAPHTYPE eGraphType = CGraphBaseData::GTNONE;
	if( !InsertGraph_GetGraphInfo(pPacketRQ, strGraphDataBuffer, graphData, szGraphData, eGraphType, strIndicatorName)) return FALSE;

	if( InsertGraph_InsertIndicatorPosition(pPacketRQ, eGraphType, strIndicatorName, szGraphData)) return true;

	if(blockType == CGraphBaseData::MODIFY || m_mainBlockBaseData.GetMaxBlocksCountInView() <= GetColumnCount() * GetRowCount())
	{
		// (2008/11/9 - Seung-Won, Bae) Do not update last block for deletion of RQ.
		//	Remove Block and Recreate it.
		//	return InsertGraph_ModifyBlock(pPacketRQ, szGraphData, doesMaintainOldScale);
		DeleteBlock( CBlockIndex( GetRowCount() - 1, 0));
		pPacketRQ = m_mainBlockBaseData.GetMainAttributes().GetPacketListManager()->GetPacketRQ( "DEFAULT");
	}

	// 마지막에 block 추가 (graph 추가)
	assert( blockType == CGraphBaseData::INSERT);
	return InsertGraph_InsertBlock(pPacketRQ, graphData, doesMaintainOldScale);
}

// doesInsertInNewBlock == true : 새로 block을 만들고 그 block에 graph를 추가한다.
// doesInsertInNewVertScaleGroup == true : 새로 vertScaleGroup을 만들고 graph를 추가한다.
// doesMaintainOldScale == true : 마지막 block만 x축 scale을 보이게 한다.
// 단, <가격차트>, <거래량차트>, <투자자동향>에 포함되는 지표인 경우는 위의 bool 변수를 무시!!
BOOL CMainBlockImp::InsertGraphInBlock( const CBlockIndex& blockIndex, const int p_nVertScaleGroupIndex, const CString& graphData, 
		const bool doesInsertInNewBlock, const bool p_doesInsertInNewVertScaleGroup, const bool doesMaintainOldScale)
{
	if( !blockIndex.IsAllValueBiggerThanZero() || p_nVertScaleGroupIndex < 0 || graphData.IsEmpty()) return FALSE;
	if( m_mainBlockBaseData.GetBlockType() == CBlockBaseData::ROW && 0 < blockIndex.GetColumn()
		|| m_mainBlockBaseData.GetBlockType() == CBlockBaseData::COLUMN && 0 < blockIndex.GetRow()) return FALSE;

	int nVertScaleGroupIndex = p_nVertScaleGroupIndex;
	bool doesInsertInNewVertScaleGroup = p_doesInsertInNewVertScaleGroup;

	// 포함되는 지표 아닌 경우.
	// 해당 위치에 block을 새로 추가하여 그 block에 graph 삽입.
	if( doesInsertInNewBlock)
	{
		if( m_mainBlockBaseData.GetBlockType() == CBlockBaseData::COLUMN || m_BlocksColumnList.IsEmpty())
		{
			int nInsertPosition = blockIndex.GetColumn();
			if( nInsertPosition < 0 || nInsertPosition > m_BlocksColumnList.GetCount()) return FALSE;

			CBlocksColumn* pBlocksColumn = CBlocksColumn::Make( this);
			assert(pBlocksColumn != NULL);
			POSITION psnBlockColumn = NULL;
			if( nInsertPosition == m_BlocksColumnList.GetCount())
			{
				psnBlockColumn = m_BlocksColumnList.GetTailPosition();
				m_BlocksColumnList.AddTail( pBlocksColumn);
			}
			else 
			{
				psnBlockColumn = m_BlocksColumnList.FindIndex( nInsertPosition);
				m_BlocksColumnList.InsertAfter( psnBlockColumn, pBlocksColumn);
			}
			// 블럭 최대화 추가 - ojtaso (20080519)
			if( GetMaximizedBlock().IsAllValueBiggerThanZero())
				SetMaximizedBlock( CBlockIndex( -1, -1));

			if( !pBlocksColumn->InsertBlock( 0, (IsEmpty() ? true : doesMaintainOldScale))) return FALSE;

			// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
			CScaleBaseData::SCALEGRIDTYPE eGridType = CScaleBaseData::SHORTDOTTEDLINE;
			if( psnBlockColumn)
			{
				CBlocksColumn *pPreBlocksColumn = m_BlocksColumnList.GetAt( psnBlockColumn);
				if( pPreBlocksColumn) eGridType = pPreBlocksColumn->GetFirstBlockVGridLineType();
			}

			pBlocksColumn->SetAllScaleGridType( eGridType);
			
			// block 위치 재설정 -> column 이 추가 되었기 때문에 위치를 재설정해줘야 한다.
			if( !m_blocksPosition.SetBlockCount( CBlockIndex( 1, m_BlocksColumnList.GetCount()), GetDoubleSizeBlockNumber())) return FALSE;
		}
		// 차트가 하나도 없는 경우는 vertScaleGroup을 만들어 줘야 한다.
		else if( m_mainBlockBaseData.GetBlockType() == CBlockBaseData::ROW)
		{
			CBlocksColumn *pBlocksColumn = GetBlocksColumn( 0);
			if( !pBlocksColumn) return FALSE;

			// 블럭 최대화 추가 - ojtaso (20080519)
			if( GetMaximizedBlock().IsAllValueBiggerThanZero())
				SetMaximizedBlock( CBlockIndex( -1, -1));
			
			if( !pBlocksColumn->InsertBlock( blockIndex.GetRow(), (IsEmpty() ? true : doesMaintainOldScale))) return FALSE;
			if( !m_blocksPosition.SetRowBlockCount( pBlocksColumn->GetBlockCount(), GetDoubleSizeBlockRowNumber())) return FALSE;
		}
		else return FALSE;

		// 새로 block을 만든 경우엔 vertScaleGroup가 <무조건 0> 이다.
		// -> (block이 새로 만들어진때 이미 하나의 vertScaleGroup도 만들어진다.)
		nVertScaleGroupIndex = 0;
		doesInsertInNewVertScaleGroup = false;
	}

	CBlockImp *pBlock = ( CBlockImp *)GetBlock( blockIndex);
	if(pBlock == NULL) return FALSE;

	return pBlock->GetVertScaleGroupList().InsertGraphInVertScaleGroup( nVertScaleGroupIndex, graphData, doesInsertInNewVertScaleGroup);
}

// doesInsertInNewBlock == true : 새로 block을 만들고 그 block에 graph를 추가한다.
// doesInsertInNewVertScaleGroup == true : 새로 vertScaleGroup을 만들고 graph를 추가한다.
// doesMaintainOldScale == true : 마지막 block만 x축 scale을 보이게 한다.
// 단, <가격차트>, <거래량차트>, <투자자동향>에 포함되는 지표인 경우는 위의 bool 변수를 무시!!
// CPacketRQ*로 변경 : 복수종목 - ojtaso (20070109)
BOOL CMainBlockImp::InsertGraphInBlock(LPVOID pPacketRQ, const CBlockIndex& blockIndex, const int p_nVertScaleGroupIndex, const CString& graphData, 
		const bool doesInsertInNewBlock, const bool p_doesInsertInNewVertScaleGroup, const bool doesMaintainOldScale)
{
	if( !blockIndex.IsAllValueBiggerThanZero() || p_nVertScaleGroupIndex < 0 || graphData.IsEmpty()) return FALSE;
	if( m_mainBlockBaseData.GetBlockType() == CBlockBaseData::ROW && 0 < blockIndex.GetColumn()
		|| m_mainBlockBaseData.GetBlockType() == CBlockBaseData::COLUMN && 0 < blockIndex.GetRow()) return FALSE;

	int nVertScaleGroupIndex = p_nVertScaleGroupIndex;
	bool doesInsertInNewVertScaleGroup = p_doesInsertInNewVertScaleGroup;

	// 포함되는 지표 아닌 경우.
	// 해당 위치에 block을 새로 추가하여 그 block에 graph 삽입.
	if( doesInsertInNewBlock)
	{
		if( m_mainBlockBaseData.GetBlockType() == CBlockBaseData::COLUMN || m_BlocksColumnList.IsEmpty())
		{
			int nInsertPosition = blockIndex.GetColumn();
			if( nInsertPosition < 0 || nInsertPosition > m_BlocksColumnList.GetCount()) return FALSE;

			CBlocksColumn* pBlocksColumn = CBlocksColumn::Make( this);
			assert(pBlocksColumn != NULL);
			POSITION psnBlockColumn = NULL;
			if( nInsertPosition == m_BlocksColumnList.GetCount())
			{
				psnBlockColumn = m_BlocksColumnList.GetTailPosition();
				m_BlocksColumnList.AddTail( pBlocksColumn);
			}
			else 
			{
				psnBlockColumn = m_BlocksColumnList.FindIndex( nInsertPosition);
				m_BlocksColumnList.InsertAfter( psnBlockColumn, pBlocksColumn);
			}
			if( !pBlocksColumn->InsertBlock( 0, (IsEmpty() ? true : doesMaintainOldScale))) return FALSE;

			// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
			CScaleBaseData::SCALEGRIDTYPE eGridType = CScaleBaseData::SHORTDOTTEDLINE;
			if( psnBlockColumn)
			{
				CBlocksColumn *pPreBlocksColumn = m_BlocksColumnList.GetAt( psnBlockColumn);
				if( pPreBlocksColumn) eGridType = pPreBlocksColumn->GetFirstBlockVGridLineType();
			}

			pBlocksColumn->SetAllScaleGridType( eGridType);
			
			// block 위치 재설정 -> column 이 추가 되었기 때문에 위치를 재설정해줘야 한다.
			if( !m_blocksPosition.SetBlockCount( CBlockIndex( 1, m_BlocksColumnList.GetCount()), GetDoubleSizeBlockNumber())) return FALSE;
		}
		// 차트가 하나도 없는 경우는 vertScaleGroup을 만들어 줘야 한다.
		else if( m_mainBlockBaseData.GetBlockType() == CBlockBaseData::ROW)
		{
			// (2008/11/9 - Seung-Won, Bae) Do not add new block with max count.
			if( m_mainBlockBaseData.GetMaxBlocksCountInView() <= GetColumnCount() * GetRowCount())
				return FALSE;

			CBlocksColumn *pBlocksColumn = GetBlocksColumn( 0);
			if( !pBlocksColumn) return FALSE;
			if( !pBlocksColumn->InsertBlock( blockIndex.GetRow(), (IsEmpty() ? true : doesMaintainOldScale))) return FALSE;
			if( !m_blocksPosition.SetRowBlockCount( pBlocksColumn->GetBlockCount(), GetDoubleSizeBlockRowNumber())) return FALSE;
		}
		else return FALSE;

		// 새로 block을 만든 경우엔 vertScaleGroup가 <무조건 0> 이다.
		// -> (block이 새로 만들어진때 이미 하나의 vertScaleGroup도 만들어진다.)
		nVertScaleGroupIndex = 0;
		doesInsertInNewVertScaleGroup = false;
	}

	CBlockImp *pBlock = ( CBlockImp *)GetBlock( blockIndex);
	if(pBlock == NULL) return FALSE;

	return pBlock->GetVertScaleGroupList().InsertGraphInVertScaleGroup(pPacketRQ, nVertScaleGroupIndex, graphData, doesInsertInNewVertScaleGroup);
} 

//////////////////////////////////////////////////////////////////////
// Delete
//////////////////////////////////////////////////////////////////////

bool CMainBlockImp::DeleteBlock(CBlockImp* pBlock, const bool bAutoHorzScaleDelete)
{
	// (2009/9/14 - Seung-Won, Bae) Do not delete default Block.
	ML_SET_LANGUAGE_RES();
	CString strMsg;
	CGraphSearchInfo_IndicatorNameWithRQ gsi;
	gsi.m_strIndicatorName	= _MTEXT( C2_PRICE_CHART);
	gsi.m_strRQ				= "DEFAULT";
	if( pBlock->GetVertScaleGroupList().FindGraph( &gsi))
	{
		strMsg.LoadString( IDS_CANNOT_MODIFY_PRICE_CHART_BLOCK);
		AfxMessageBox( strMsg, MB_ICONWARNING|MB_OK);
		return false;
	}

	if( !pBlock) return false;
	if( m_BlocksColumnList.IsEmpty()) return false;

	// 블럭 최대화 추가 - ojtaso (20080519)
	if( GetMaximizedBlock().IsAllValueBiggerThanZero())
		SetMaximizedBlock( CBlockIndex( -1, -1));

	switch( m_mainBlockBaseData.GetBlockType())
	{
		case CBlockBaseData::ROW:
		{
			CBlocksColumn *pBlocksColumn = m_BlocksColumnList.GetHead();
			if( !pBlocksColumn) return false; 

			CGraphSearchInfo_Block gsi;
			gsi.m_pBlockImp = pBlock;
			if( !pBlocksColumn->FindGraph( &gsi)) return false;
			int nRowNumber = gsi.m_nRowBlockIndex;
			if( nRowNumber < 0) return false;

			// Block을 삭제하기전에 Horizontal Scale의 Type을 얻어온다.
			CHorizontalScale horizontalScale = pBlock->GetBlockData().GetHorzScale();;
			BOOL bIsBottom = ( nRowNumber == m_blocksPosition.GetRowBlockCount() - 1 );

			if( !pBlocksColumn->DeleteBlock( pBlock, bAutoHorzScaleDelete)) return false;
			m_blocksPosition.SetRowBlockCount( m_blocksPosition.GetRowBlockCount() - 1, GetDoubleSizeBlockRowNumber());

			// 해당 column에 block이 없을 경우 blocksColumn도 삭제
			if( pBlocksColumn->GetBlockCount() <= 0)
			{
				POSITION removePos = m_BlocksColumnList.Find(pBlocksColumn);
				m_BlocksColumnList.RemoveAt( removePos);
				CBlocksColumn::Delete( pBlocksColumn);
				m_blocksPosition.SetColumnBlockCount( m_blocksPosition.GetColumnBlockCount() - 1, GetDoubleSizeBlockNumber().GetColumn());
				break;
			}

			// Sang-Woo, Cho		2006 / 10 / 13
			// 지표의 제거시 가장 아래 Block의 Horizontal Scale을 보이게 한다.
			// Begin
			if( bIsBottom)
			{
				CBlockImp *pBottomBlock = pBlocksColumn->GetBlock( nRowNumber - 1);
				if( pBottomBlock) switch( horizontalScale.GetHorzScalePosition())
				{
					case CScaleBaseData::HORZ_BOTH:
					case CScaleBaseData::HORZ_BOTTOM:
					{
						// 가장 아래 Block의 HorizontalScale을 얻어와서 HORZ_HIDE이면 HORZ_BOTTOM으로 변경한다.
						CHorizontalScale &horizontalScale = pBottomBlock->GetBlockData().GetHorzScale();
						// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
						//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
						switch( horizontalScale.GetHorzScalePosition())
						{
							case CScaleBaseData::HORZ_TOP:	SetHorzScalePosition( horizontalScale, CScaleBaseData::HORZ_BOTH);
															break;
							case CScaleBaseData::HORZ_HIDE:	SetHorzScalePosition( horizontalScale, CScaleBaseData::HORZ_BOTTOM);
															break;
						}
						break;
					}
				}
			}

			return true;
		}
		case CBlockBaseData::COLUMN:
		{
			CGraphSearchInfo_Block gsi;
			gsi.m_pBlockImp = pBlock;
			if( !FindGraph( &gsi)) return NULL;
			if( gsi.m_pBlocksColumn == NULL) return false;

			POSITION removePos = m_BlocksColumnList.Find( gsi.m_pBlocksColumn);
			// (2007/2/16 - Seung-Won, Bae) Remove in list after Deleting the Object.
			//		In Destructor, can be access the List.
			CBlocksColumn::Delete( gsi.m_pBlocksColumn);
			m_BlocksColumnList.RemoveAt( removePos);
			m_blocksPosition.SetColumnBlockCount( m_blocksPosition.GetColumnBlockCount() - 1, GetDoubleSizeBlockNumber().GetColumn());
			break;
		}
		default:	return false;
	}

	return true;
}

bool CMainBlockImp::DeleteBlock(const CBlockIndex& blockIndex, const bool bAutoHorzScaleDelete)
{
	CBlockImp* pBlock = ( CBlockImp *)GetBlock(blockIndex);
	if(pBlock == NULL)
		return false;

	return DeleteBlock(pBlock, bAutoHorzScaleDelete);
}

// 블럭 최대화 추가 - ojtaso (20080519)
void CMainBlockImp::SetMaximizedBlock( const CBlockIndex& maxBlockIndex)
{
	// (2009/9/6 - Seung-Won, Bae) Hide HScale to restore.
	if( GetMaximizedBlock().IsAllValueBiggerThanZero())
	{
		CBlockImp *pBlockImp = ( CBlockImp *)GetBlock( GetMaximizedBlock());
		if( pBlockImp)
		{
			CHorizontalScale &hscaleMax = pBlockImp->GetBlockData().GetHorzScale();
			int nRow = GetMaximizedBlock().GetRow();
			int nColumn = GetMaximizedBlock().GetColumn();
			// 1. Top
			if( nRow != 0 && ( hscaleMax.GetHorzScalePosition() == CScaleBaseData::HORZ_TOP || hscaleMax.GetHorzScalePosition() == CScaleBaseData::HORZ_BOTH))
			{
				CBlockImp *pTopBlockImp = ( CBlockImp *)GetBlock( CBlockIndex( 0, nColumn));
				if( pTopBlockImp)
				{
					CHorizontalScale &hscaleTop = pTopBlockImp->GetBlockData().GetHorzScale();
					if( hscaleTop.GetHorzScalePosition() == CScaleBaseData::HORZ_TOP || hscaleTop.GetHorzScalePosition() == CScaleBaseData::HORZ_BOTH)
					{
						if( hscaleMax.GetHorzScalePosition() == CScaleBaseData::HORZ_TOP)
						{
							SetHorzScalePosition( hscaleMax, CScaleBaseData::HORZ_HIDE);
							hscaleMax.SetScaleHeight( -1);
						}
						else if( hscaleMax.GetHorzScalePosition() == CScaleBaseData::HORZ_BOTH)
							SetHorzScalePosition( hscaleMax, CScaleBaseData::HORZ_BOTTOM);
					}
				}
			}
			// 2. Bottom
			if( nRow != m_blocksPosition.GetRowBlockCount() - 1 && ( hscaleMax.GetHorzScalePosition() == CScaleBaseData::HORZ_BOTTOM || hscaleMax.GetHorzScalePosition() == CScaleBaseData::HORZ_BOTH))
			{
				CBlockImp *pBottomBlockImp = ( CBlockImp *)GetBlock( CBlockIndex( m_blocksPosition.GetRowBlockCount() - 1, nColumn));
				if( pBottomBlockImp)
				{
					CHorizontalScale &hscaleBottom = pBottomBlockImp->GetBlockData().GetHorzScale();
					if( hscaleBottom.GetHorzScalePosition() == CScaleBaseData::HORZ_BOTTOM || hscaleBottom.GetHorzScalePosition() == CScaleBaseData::HORZ_BOTH)
					{
						if( hscaleMax.GetHorzScalePosition() == CScaleBaseData::HORZ_BOTTOM)
						{
							SetHorzScalePosition( hscaleMax, CScaleBaseData::HORZ_HIDE);
							hscaleMax.SetScaleHeight( -1);
						}
						else if( hscaleMax.GetHorzScalePosition() == CScaleBaseData::HORZ_BOTH)
							SetHorzScalePosition( hscaleMax, CScaleBaseData::HORZ_TOP);
					}
				}
			}
		}
	}

	m_blocksPosition.SetMaximizedBlock( maxBlockIndex);

	// (2009/9/6 - Seung-Won, Bae) Show HScale to maximize.
	if( GetMaximizedBlock().IsAllValueBiggerThanZero())
	{
		CBlockImp *pBlockImp = ( CBlockImp *)GetBlock( GetMaximizedBlock());
		if( pBlockImp)
		{
			CHorizontalScale &hscaleMax = pBlockImp->GetBlockData().GetHorzScale();
			int nRow = GetMaximizedBlock().GetRow();
			int nColumn = GetMaximizedBlock().GetColumn();
			// 1. Top
			if( nRow != 0 && ( hscaleMax.GetHorzScalePosition() != CScaleBaseData::HORZ_TOP && hscaleMax.GetHorzScalePosition() != CScaleBaseData::HORZ_BOTH))
			{
				CBlockImp *pTopBlockImp = ( CBlockImp *)GetBlock( CBlockIndex( 0, nColumn));
				if( pTopBlockImp)
				{
					CHorizontalScale &hscaleTop = pTopBlockImp->GetBlockData().GetHorzScale();
					if( hscaleTop.GetHorzScalePosition() == CScaleBaseData::HORZ_TOP || hscaleTop.GetHorzScalePosition() == CScaleBaseData::HORZ_BOTH)
					{
						if( hscaleMax.GetHorzScalePosition() == CScaleBaseData::HORZ_HIDE)
						{
							SetHorzScalePosition( hscaleMax, CScaleBaseData::HORZ_TOP);
							hscaleMax.SetScaleHeight( hscaleTop.GetScaleHeight());
						}
						else if( hscaleMax.GetHorzScalePosition() == CScaleBaseData::HORZ_BOTTOM)
						{
							SetHorzScalePosition( hscaleMax, CScaleBaseData::HORZ_BOTH);
							if( hscaleMax.GetScaleHeight() < hscaleTop.GetScaleHeight())
								hscaleMax.SetScaleHeight( hscaleTop.GetScaleHeight());
						}
					}
				}
			}
			// 2. Bottom
			if( nRow != m_blocksPosition.GetRowBlockCount() - 1 && ( hscaleMax.GetHorzScalePosition() != CScaleBaseData::HORZ_BOTTOM && hscaleMax.GetHorzScalePosition() != CScaleBaseData::HORZ_BOTH))
			{
				CBlockImp *pBottomBlockImp = ( CBlockImp *)GetBlock( CBlockIndex( m_blocksPosition.GetRowBlockCount() - 1, nColumn));
				if( pBottomBlockImp)
				{
					CHorizontalScale &hscaleBottom = pBottomBlockImp->GetBlockData().GetHorzScale();
					if( hscaleBottom.GetHorzScalePosition() == CScaleBaseData::HORZ_BOTTOM || hscaleBottom.GetHorzScalePosition() == CScaleBaseData::HORZ_BOTH)
					{
						if( hscaleMax.GetHorzScalePosition() == CScaleBaseData::HORZ_HIDE)
						{
							SetHorzScalePosition( hscaleMax, CScaleBaseData::HORZ_BOTTOM);
							hscaleMax.SetScaleHeight( hscaleBottom.GetScaleHeight());
						}
						else if( hscaleMax.GetHorzScalePosition() == CScaleBaseData::HORZ_TOP)
						{
							SetHorzScalePosition( hscaleMax, CScaleBaseData::HORZ_BOTH);
							if( hscaleMax.GetScaleHeight() < hscaleBottom.GetScaleHeight())
								hscaleMax.SetScaleHeight( hscaleBottom.GetScaleHeight());
						}
					}
				}
			}
		}
	}
}

// 블럭 최대화 추가 - ojtaso (20080519)
CBlockIndex	CMainBlockImp::GetMaximizedBlock( void) const
{
	return m_blocksPosition.GetMaximizedBlock();
}

// 리스트의 모든 내용(block 들) 삭제
bool CMainBlockImp::DeleteAllBlock()
{
	if( m_BlocksColumnList.IsEmpty()) return false;

	// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
	CBlockImp *pBlock = ( CBlockImp *)GetBlock( CBlockIndex( 0, 0));
	if( pBlock)
	{
		m_eLastHorzGridType = GetFirstBlockHGridLineType();
		m_eLastVerticalGridType = GetFirstBlockVGridLineType();

		//KHD : 시간구분
		// 분/틱차트에서의 일자구분선 개별옵션 - onlyojt
		m_eLastDailyGridType = GetFirstDailyScaleGridType(CScaleBaseData::MINTICK_DAY_GUBUN);
		m_WeekGridType= GetFirstDailyScaleGridType(CScaleBaseData::DAYCHART_WEEK_GUBUN);
		m_MonGridType= GetFirstDailyScaleGridType(CScaleBaseData::DAYCHART_MON_GUBUN);
		m_YearGridType= GetFirstDailyScaleGridType(CScaleBaseData::DAYCHART_YEAR_GUBUN);
		//END 
	}

	// (2007/2/16 - Seung-Won, Bae) Remove in list after Deleting the Object.
	//		In Destructor, can be access the List.
	while( !m_BlocksColumnList.IsEmpty())
	{
		CBlocksColumn::Delete( m_BlocksColumnList.GetHead());
		m_BlocksColumnList.RemoveHead();
	}
	m_blocksPosition.BlockPositionReset();
	return true;
}

bool CMainBlockImp::DeleteGraph( const CString& strGraphName)
{
	if( strGraphName.IsEmpty()) return false;

	CGraphSearchInfo_GraphName gsi;
	gsi.m_strGraphName = strGraphName;
	if( !FindGraph( &gsi)) return false;
	if( !gsi.m_pVertScaleGroup || !gsi.m_pGraphImp) return false;

	return gsi.m_pVertScaleGroup->DeleteGraphWithNotify( gsi.m_pGraphImp, true);
}

BOOL CMainBlockImp::DeleteAllIndicator( const CString& strIndicatorName)
{
	if( strIndicatorName.IsEmpty()) return false;

	BOOL bResult = FALSE;
	CBlocksColumn *pBlocksColumn = NULL;
	POSITION posBlockColumn = m_BlocksColumnList.GetHeadPosition();
	while( posBlockColumn)
	{
		pBlocksColumn = m_BlocksColumnList.GetNext( posBlockColumn);
		if( !pBlocksColumn) continue;
		if( pBlocksColumn->DeleteAllIndicator( strIndicatorName)) bResult = TRUE;
	}
	return bResult;
}

bool CMainBlockImp::DeleteGraphWithSubGraphTitle( const CBlockIndex& blockIndex, const CString& strTitleName, const bool bIsOnlyNoTRData)
{
	if( strTitleName.IsEmpty()) return false;

	CGraphSearchInfo_SubGraphTitle gsi;
	gsi.m_strSubGraphTitle = strTitleName;
	
	if( blockIndex.GetRow() < 0 || blockIndex.GetColumn() < 0)
	{
		if( !FindGraph( &gsi)) return false;
	}
	else
	{
		CBlockImp* pBlock = ( CBlockImp *)GetBlock( blockIndex);
		if( !pBlock) return false;
		if( !pBlock->GetVertScaleGroupList().FindGraph( &gsi)) return false;
	}
	if( !gsi.m_pVertScaleGroup || !gsi.m_pGraphImp) return false;

	return gsi.m_pVertScaleGroup->DeleteGraphWithNotify( gsi.m_pGraphImp, true);
}

// (2008/9/14 - Seung-Won, Bae) for Pair Tick
BOOL CMainBlockImp::RemoveAllGraphOfRQ( LPVOID lpPacketRQ)
{
	if( !lpPacketRQ) return FALSE;
	CGraphSearchInfo_AnyGraph gsi;
	if( FindGraph( lpPacketRQ, &gsi))
	{
		gsi.m_pVertScaleGroup->DeleteGraphWithNotify( gsi.m_pGraphImp, true);
		return TRUE;
	}
	return FALSE;
}

// RQ 지정 그래프 삭제 메소드 추가 - ojtaso (20080604)
bool CMainBlockImp::DeleteGraphWithSubGraphTitleRQ(LPCTSTR lpszRQ, const CBlockIndex& blockIndex, const CString& strTitleName, const bool bIsOnlyNoTRData)
{
	if( strTitleName.IsEmpty()) return false;

	CGraphSearchInfo_SubGraphTitleWithRQ gsi;
	gsi.m_strSubGraphTitle = strTitleName;
	gsi.m_strRQ = lpszRQ;
	
	if( blockIndex.GetRow() < 0 || blockIndex.GetColumn() < 0)
	{
		if( !FindGraph( &gsi)) return false;
	}
	else
	{
		CBlockImp* pBlock = ( CBlockImp *)GetBlock( blockIndex);
		if( !pBlock) return false;
		if( !pBlock->GetVertScaleGroupList().FindGraph( &gsi)) return false;
	}
	if( !gsi.m_pVertScaleGroup || !gsi.m_pGraphImp) return false;

	return gsi.m_pVertScaleGroup->DeleteGraphWithNotify( gsi.m_pGraphImp, true);
}

//////////////////////////////////////////////////////////////////////
// Search Object
//////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------------------
CBlocksColumn *CMainBlockImp::GetBlocksColumn( const int nBlockColumnIndex) const
{
	POSITION pos = m_BlocksColumnList.FindIndex(nBlockColumnIndex);
	if(pos == NULL)
		return NULL;

	return m_BlocksColumnList.GetAt(pos);
}

CVertScaleGroup *CMainBlockImp::GetVertScaleGroup( const CBlockIndex& blockIndex, const int nScaleGroupIndex) const
{
	CBlockImp *pBlock = ( CBlockImp *)GetBlock( blockIndex);
	if( !pBlock) return NULL;

	return pBlock->GetVertScaleGroupList().GetVertScaleGroup( nScaleGroupIndex);
}

CGraphImp *CMainBlockImp::GetGraph( const CBlockIndex& blockIndex, const int nScaleGroupIndex, const int nGraphIndex) const
{
	CBlockImp *pBlock = ( CBlockImp *)GetBlock( blockIndex);
	if( !pBlock) return NULL;

	return pBlock->GetVertScaleGroupList().GetGraph( nScaleGroupIndex, nGraphIndex);
}

CSubGraph *CMainBlockImp::GetSubGraph( const CBlockIndex& blockIndex, const int nScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex) const
{
	CBlockImp *pBlock = ( CBlockImp *)GetBlock( blockIndex);
	if( !pBlock) return NULL;

	return pBlock->GetVertScaleGroupList().GetSubGraph( nScaleGroupIndex, nGraphIndex, nSubGraphIndex);
}

CBlockIndex CMainBlockImp::FindBlockIndex(const CPoint& point) const
{
	for(int blockColumnIndex = 0; blockColumnIndex < GetColumnCount(); blockColumnIndex++)
	{
		for(int blockRowIndex = 0; blockRowIndex < GetRowCount(); blockRowIndex++)
		{
			CRect blockRegion = m_blocksPosition.GetBlockRegion(CBlockIndex(blockRowIndex, blockColumnIndex));
			if(blockRegion.PtInRect(point))
				return CBlockIndex(blockRowIndex, blockColumnIndex);
		}
	}

	return CBlockIndex(-1, -1);
}

CBlock* CMainBlockImp::GetBlock(const CBlockIndex& blockIndex) const
{
	if( !blockIndex.IsAllValueBiggerThanZero()) return NULL;

	CBlocksColumn* pBlocksColumn = GetBlocksColumn( blockIndex.GetColumn());
	if( !pBlocksColumn) return NULL;

	return pBlocksColumn->GetBlock(blockIndex.GetRow());
}

// (2007/2/22 - Seung-Won, Bae) Support Next Search.
BOOL CMainBlockImp::FindGraph( CGraphSearchInfo *p_pGraphSearchInfo) const
{
	if( !p_pGraphSearchInfo)	return FALSE;

	int nColumnBlockIndex = p_pGraphSearchInfo->m_nColumnBlockIndex;
	POSITION posBlockColumn = ( p_pGraphSearchInfo->m_posColumnBlock ? p_pGraphSearchInfo->m_posColumnBlock : m_BlocksColumnList.FindIndex( nColumnBlockIndex));

	CBlocksColumn *pBlocksColumn = NULL;
	POSITION posCurrentBlockColumn = NULL;
	while( posBlockColumn)
	{
		posCurrentBlockColumn = posBlockColumn;
		pBlocksColumn = m_BlocksColumnList.GetNext( posBlockColumn);
		if( !pBlocksColumn) continue;
		if( p_pGraphSearchInfo->FindGraph( pBlocksColumn))
		{
			p_pGraphSearchInfo->m_posColumnBlock = posCurrentBlockColumn;
			p_pGraphSearchInfo->m_nColumnBlockIndex = nColumnBlockIndex;
			p_pGraphSearchInfo->m_pBlocksColumn = pBlocksColumn;
			return TRUE;
		}
		nColumnBlockIndex++;
	}

	// (2007/4/11 - Seung-Won, Bae) Reset for Next ~ Search
	p_pGraphSearchInfo->m_posColumnBlock	= NULL;
	p_pGraphSearchInfo->m_nColumnBlockIndex	= 0;
	return FALSE;
}

// RQ로 검색 : 복수종목 - ojtaso (20070118)
BOOL CMainBlockImp::FindGraph(LPVOID pPacketRQ,  CGraphSearchInfo *p_pGraphSearchInfo) const
{
 	if( !p_pGraphSearchInfo)	return FALSE;
 
 	int nColumnBlockIndex = 0;
 	CBlocksColumn *pBlocksColumn = NULL;
 	POSITION posBlockColumn = m_BlocksColumnList.GetHeadPosition();
 	while( posBlockColumn)
 	{
 		pBlocksColumn = m_BlocksColumnList.GetNext( posBlockColumn);
 		if( !pBlocksColumn) continue;
 		if( pBlocksColumn->FindGraph(pPacketRQ, p_pGraphSearchInfo))
 		{
 			p_pGraphSearchInfo->m_nColumnBlockIndex = nColumnBlockIndex;
 			p_pGraphSearchInfo->m_pBlocksColumn = pBlocksColumn;
 			return TRUE;
 		}
 		nColumnBlockIndex++;
 	}
 	return FALSE;
}

// (2006/6/25 - Seung-Won, Bae) Search Index of Graph
BOOL CMainBlockImp::GetIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG)
{
	p_nR = p_nC = p_nV = p_nG = -1;
	if( !p_szGraphName) return FALSE;
	if( !*p_szGraphName) return FALSE;

	CGraphSearchInfo_GraphName gsi;
	gsi.m_strGraphName = p_szGraphName;
	if( !FindGraph( &gsi)) return FALSE;

	p_nR = gsi.m_nRowBlockIndex;
	p_nC = gsi.m_nColumnBlockIndex;
	p_nV = gsi.m_nVertScaleGroupIndex;
	p_nG = gsi.m_nGraphIndex;
	return TRUE;
}

// (2007/1/20 - Seung-Won, Bae) Search Indicator
BOOL CMainBlockImp::FindFirstIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, CString &p_strGraphName, const char *p_szRQ)
{
	p_nR = p_nC = p_nV = p_nG = -1;
	p_strGraphName.Empty();

	CGraphSearchInfo_AnyGraph					gsi;

	CGraphSearchInfo_AnyGraphWithRQ				gsiRQ;
	gsiRQ.m_strRQ								= p_szRQ;

	CGraphSearchInfo_IndicatorName				gsiIndicatorName;
	gsiIndicatorName.m_strIndicatorName			= p_szIndicatorName;

	CGraphSearchInfo_IndicatorNameWithRQ		gsiIndicatorNameWithRQ;
	gsiIndicatorNameWithRQ.m_strRQ				= p_szRQ;
	gsiIndicatorNameWithRQ.m_strIndicatorName	= p_szIndicatorName;
	
	CGraphSearchInfo *pGSI = &gsi;
	if( p_szIndicatorName && p_szRQ)		if( *p_szIndicatorName && *p_szRQ)
												pGSI = &gsiIndicatorNameWithRQ;
	if( pGSI == &gsi && p_szIndicatorName)	if( *p_szIndicatorName)
												pGSI = &gsiIndicatorName;
	if( pGSI == &gsi && p_szRQ)				if( *p_szRQ)
												pGSI = &gsiRQ;

	if( !FindGraph( pGSI)) return FALSE;
	if( !pGSI->m_pGraphImp) return FALSE;

	p_nR			= pGSI->m_nRowBlockIndex;
	p_nC			= pGSI->m_nColumnBlockIndex;
	p_nV			= pGSI->m_nVertScaleGroupIndex;
	p_nG			= pGSI->m_nGraphIndex;
	p_strGraphName	= pGSI->m_pGraphImp->GetGraphData().GetGraphName();
	return TRUE;
}
BOOL CMainBlockImp::FindNextIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, CString &p_strGraphName, const char *p_szRQ)
{
	p_strGraphName.Empty();

	CGraphSearchInfo_AnyGraph					gsi;

	CGraphSearchInfo_AnyGraphWithRQ				gsiRQ;
	gsiRQ.m_strRQ								= p_szRQ;

	CGraphSearchInfo_IndicatorName				gsiIndicatorName;
	gsiIndicatorName.m_strIndicatorName			= p_szIndicatorName;

	CGraphSearchInfo_IndicatorNameWithRQ		gsiIndicatorNameWithRQ;
	gsiIndicatorNameWithRQ.m_strRQ				= p_szRQ;
	gsiIndicatorNameWithRQ.m_strIndicatorName	= p_szIndicatorName;
	
	CGraphSearchInfo *pGSI = &gsi;
	if( p_szIndicatorName && p_szRQ)		if( *p_szIndicatorName && *p_szRQ)
												pGSI = &gsiIndicatorNameWithRQ;
	if( pGSI == &gsi && p_szIndicatorName)	if( *p_szIndicatorName)
												pGSI = &gsiIndicatorName;
	if( pGSI == &gsi && p_szRQ)				if( *p_szRQ)
												pGSI = &gsiRQ;

	pGSI->m_nRowBlockIndex			= p_nR;
	pGSI->m_nColumnBlockIndex		= p_nC;
	pGSI->m_nVertScaleGroupIndex	= p_nV;
	pGSI->m_nGraphIndex				= p_nG;
	pGSI->SetNextStartGraph();

	if( !FindGraph( pGSI)) return FALSE;
	if( !pGSI->m_pGraphImp) return FALSE;

	p_nR			= pGSI->m_nRowBlockIndex;
	p_nC			= pGSI->m_nColumnBlockIndex;
	p_nV			= pGSI->m_nVertScaleGroupIndex;
	p_nG			= pGSI->m_nGraphIndex;
	p_strGraphName	= pGSI->m_pGraphImp->GetGraphData().GetGraphName();
	return TRUE;
}

CString CMainBlockImp::FindGraph( const CPoint &p_pt) const
{
	CSelectedSubGraphDeque selectedSubGraphDeque;
	if( !FindSubGraph( p_pt, selectedSubGraphDeque)) return "";
	if( selectedSubGraphDeque.GetCount() <= 0) return "";

	CSubGraph *pSubGraph = selectedSubGraphDeque.GetSubGraphTail();
	if( !pSubGraph) return "";

	CGraphImp *pGraph = pSubGraph->GetGraph();
	if( !pGraph) return "";

	return pGraph->GetGraphData().GetGraphName();
}

// subGraph 을 찾기.
bool CMainBlockImp::FindSubGraph(const CPoint& point, CSelectedSubGraphDeque& selectedSubGraphDeque) const
{	
	CBlockIndex bi = FindBlockIndex(point);
	CBlockImp* pBlock = ( CBlockImp *)GetBlock( bi);
	if(pBlock == NULL)
		return false;

	return pBlock->GetVertScaleGroupList().FindSubGraph(point, selectedSubGraphDeque);
}

//////////////////////////////////////////////////////////////////////
// Move
//////////////////////////////////////////////////////////////////////

bool CMainBlockImp::MoveAllVertScaleGroups( const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex)
{
	CBlockImp* pTargetBlock = ( CBlockImp *)GetBlock(targetBlockIndex);
	CBlockImp* pSourceBlock = ( CBlockImp *)GetBlock(sourceBlockIndex);
	if(pTargetBlock == NULL || pSourceBlock == NULL || pTargetBlock == pSourceBlock)
		return false;

	return pTargetBlock->GetVertScaleGroupList().MoveAllVertScaleGroups(&pSourceBlock->GetVertScaleGroupList());
}

// ----------------------------------------------------------------------------
// vertScaleGroup들을 해당 block에 이동
// --> 모든 block은 첫번째 block에 이동되며 이동된 빈 block은 삭제된다.
//		bIsAutoBaseHorzScalePosition = true : 가장 하단에 있는 block 만 Y축 scale 이 보이고 나머지 block는 숨김.
bool CMainBlockImp::MoveAllVertScaleGroupsToHeadBlock( const bool bDoesMaintainOldScale)
{
	// 모든 block에 있는 vertScaletGroup를 첫번째 block에 이동시킨다. (VertScaleGroup 는 n개)
	bool bResult = false;
	int nRowCount = GetRowCount();
	int nColumnCount = GetColumnCount();
	for(int nColumnIndex = 0; nColumnIndex < nColumnCount; nColumnIndex++)
	{
		for(int nRowIndex = 1; nRowIndex < nRowCount; nRowIndex++)
		{
			if( MoveAllVertScaleGroups(CBlockIndex(1, 0), CBlockIndex(0, 0)))
				bResult = true;
		}
	}

	if(bDoesMaintainOldScale && bResult)
		SetBaseHorzScalePosition();

	return bResult;
}

// --> 해당 block에 있는 모든 vertScaleGroup를 각각 block로 만든다.
//		bIsAutoBaseHorzScalePosition = true : 가장 하단에 있는 block 만 Y축 scale 이 보이고 나머지 block는 숨김.
bool CMainBlockImp::MoveAllVertScaleGroupsToEachRowBlock(const CBlockIndex& targetBlockIndex, const bool bDoesMaintainOldScale)
{
	CBlocksColumn* pTargetBlocksColumn = GetBlocksColumn(targetBlockIndex.GetColumn());
	if(pTargetBlocksColumn == NULL)
		return false;

	if( !pTargetBlocksColumn->MoveAllVertScaleGroupsInPartition(targetBlockIndex.GetRow(), bDoesMaintainOldScale))
		return false;

	int nRowCount = m_BlocksColumnList.GetHead()->GetBlockCount();
	int nColumnCount = m_BlocksColumnList.GetCount();
	if( nRowCount != GetRowCount() || nColumnCount != GetColumnCount())
		m_blocksPosition.SetBlockCount( CBlockIndex( nRowCount, nColumnCount), GetDoubleSizeBlockNumber());
	return true;
}

// vertScaleGroup의 join
// --> 해당 block 에 있는 vertScaleGroup를 분할/중첩한다. (bIsJoin = true : 중첩)
bool CMainBlockImp::JoinAllVertScaleGroups(const CBlockIndex& blockIndex, const bool bIsJoin)
{
	CBlockImp* pSourceBlock = ( CBlockImp *)GetBlock(blockIndex);
	if(pSourceBlock == NULL)
		return false;

	return pSourceBlock->GetVertScaleGroupList().ReArrangeAllVertScaleGroups( bIsJoin);
}

// graph 이동.
// --> nTargetGroupIndex번째 TargertGroup가 있으면 join
//     nTargetGroupIndex == -1 이면 join이 아닌 AddTail() 이다!!
bool CMainBlockImp::MoveAllGraphs(const CBlockIndex& sourceBlockIndex, const int nSourceGroupIndex, const CBlockIndex& targetBlockIndex, const int nTargetGroupIndex)
{
	CBlockImp* pTargetBlock = ( CBlockImp *)GetBlock(targetBlockIndex);
	CBlockImp* pSourceBlock = ( CBlockImp *)GetBlock(sourceBlockIndex);
	if(pTargetBlock == NULL || pSourceBlock == NULL)
		return false;

	return pTargetBlock->GetVertScaleGroupList().MoveAllGraphs( pSourceBlock->GetVertScaleGroupList().GetVertScaleGroup(nSourceGroupIndex), nTargetGroupIndex, true);
}

bool CMainBlockImp::MoveGraph(const CBlockIndex& sourceBlockIndex, const int nSourceGroupIndex, const int nSourceGraphIndex,
										 const CBlockIndex& targetBlockIndex, const int nTargetGroupIndex)
{
	CBlockImp* pTargetBlock = ( CBlockImp *)GetBlock(targetBlockIndex);
	CBlockImp* pSourceBlock = ( CBlockImp *)GetBlock(sourceBlockIndex);
	if(pTargetBlock == NULL || pSourceBlock == NULL)
		return false;

	CGraphImp *pGraph = pSourceBlock->GetVertScaleGroupList().GetGraph( nSourceGroupIndex, nSourceGraphIndex);
	if( !pGraph) return false;

	return pTargetBlock->GetVertScaleGroupList().MoveGraph( pGraph, nTargetGroupIndex, true);
}

bool CMainBlockImp::MoveGraph2(const CBlockIndex& sourceBlockIndex, const int nSourceGroupIndex, const int nSourceGraphIndex,
							  const CBlockIndex& targetBlockIndex, const int nTargetGroupIndex, const BOOL bIsAddTail)
{
	CBlockImp* pTargetBlock = ( CBlockImp *)GetBlock(targetBlockIndex);
	CBlockImp* pSourceBlock = ( CBlockImp *)GetBlock(sourceBlockIndex);
	if(pTargetBlock == NULL || pSourceBlock == NULL)
		return false;

	//최소 하나의 그래프는 존재
	if( pSourceBlock->GetVertScaleGroupList().GetCount() == 1 )
	{
		if( pSourceBlock->GetVertScaleGroupList().GetVertScaleGroup( 0 )->GetGraphCount() == 1 )
			return false;
	}
	
	CGraphImp *pGraph = pSourceBlock->GetVertScaleGroupList().GetGraph( nSourceGroupIndex, nSourceGraphIndex);
	if( !pGraph) return false;
	
	return pTargetBlock->GetVertScaleGroupList().MoveGraph( pGraph, nTargetGroupIndex, true, bIsAddTail );
}

//////////////////////////////////////////////////////////////////////
// Query & Update
//////////////////////////////////////////////////////////////////////

// BlockBackground 만 그린다..
void CMainBlockImp::DrawBlocks_DrawAllBlocksBackground( CDC* pDC)
{
	// (2009/9/6 - Seung-Won, Bae) for Block Maximizing.
	if( GetMaximizedBlock().IsAllValueBiggerThanZero())
	{
		CBlockImp *pBlockImp = ( CBlockImp *)GetBlock( GetMaximizedBlock());
		if( pBlockImp)
		{
			pBlockImp->DrawBlockBackground( pDC, m_blocksPosition.GetBlockRegion( GetMaximizedBlock()), 
				m_blocksPosition.GetInterBlockHalfGap());
			return;
		}
	}

	//sy test
	int blockColumnIndex = 0;
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = (CBlocksColumn*) m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		pBlocksColumn->DrawAllBlocksBackground(pDC, blockColumnIndex, m_blocksPosition);
		blockColumnIndex++;
	}
}

// block 구분선을 그린다.
void CMainBlockImp::DrawBlocks_DrawBlockCompartLine(CDC* pDC)
{
	if(m_mainBlockBaseData.GetBlockCompartType() != CBlockBaseData::COMPARTLINE)
		return;

	// (2009/9/6 - Seung-Won, Bae) for Block Maximizing.
	if( GetMaximizedBlock().IsAllValueBiggerThanZero())
		return;

	m_blocksPosition.DrawBlockCompartLine(pDC, m_mainBlockBaseData.GetGraphRegionColor().GetFrameColor());
}

void CMainBlockImp::ChangeDrawStyle(int nDrawStyle)
{
	if(nDrawStyle == CGraphBaseData::Bong_Type)
	{
		CPacketListManager *pPacketListManager = GetPacketListManager();
		// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
		// (2007/4/12 - Seung-Won, Bae) Comment for Next Version Processing
		// (2007/04/11 - Seung-Won, Bae) Change Bong Draw Style to Line in 1 Tick Chart.
		// (2008/12/19 - Seung-Won, Bae) Calc it in VScale Draw for Including BaseLine.
		m_nBongGraph = 0;
		CGraphSearchInfo_GraphType gsi;
		gsi.m_eGraphType = CGraphBaseData::Bong_Type;
		while( FindGraph( &gsi) && m_nBongGraph < 50)
		{
			if( gsi.m_pGraphImp && pPacketListManager)
			{
				// (2009/2/23 - Seung-Won, Bae) support Multi-RQ
				CPacketList *pPacketList = pPacketListManager->GetPacketList( gsi.m_pGraphImp->GetGraphRQ());
				if( pPacketList) if( CScaleBaseData::HORZ_TICK == pPacketList->GetDateUnitType() && pPacketList->GetTimeInterval() == 1)
				{
					CSubGraphList &subGraphList = gsi.m_pGraphImp->GetSubGraphList();
					CSubGraph *pSubGraph = subGraphList.GetSubGraph( 0);
					if( pSubGraph)
					{
						CSubGraphData &subGraphData = pSubGraph->GetSubGraphData();
						m_eBongStyle[ m_nBongGraph] = subGraphData.GetStyle().GetGraphBongStyle();
						if( CGraphBaseData::LineBong != m_eBongStyle[ m_nBongGraph])
						{
							m_pBongGraph[ m_nBongGraph] = gsi.m_pGraphImp;
							m_eBongDrawStyle[ m_nBongGraph] = subGraphData.GetDrawStyle().GetGraphBongDrawStyle();
							
							subGraphData.SetStyle(		CGraphStyle( CGraphBaseData::LineBong));
							subGraphData.SetDrawStyle(	CGraphDrawStyle( CGraphBaseData::Line_Draw));
							
							m_nBongGraph++;
						}
					}
				}
			}
			
			gsi.SetNextStartGraph();
		}
	}
	else if(nDrawStyle == CGraphBaseData::Line_Type)
	{
		CXScaleManager* pxScaleManager = GetXScaleManager();
		// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
		// (2007/04/11 - Seung-Won, Bae) Change Bong Draw Style to Line in 1 Tick Chart.
		if( m_nBongGraph && pxScaleManager->GetThreadFlag() != THREAD_CALCULATION)
		{
			int i = 0;
			for( i = 0; i < m_nBongGraph; i++)
			{
				CSubGraphList &subGraphList = m_pBongGraph[ i]->GetSubGraphList();
				CSubGraph *pSubGraph = subGraphList.GetSubGraph( 0);
				if( pSubGraph)
				{
					CSubGraphData &subGraphData = pSubGraph->GetSubGraphData();
					subGraphData.SetStyle(		CGraphStyle( m_eBongStyle[ i]));
					subGraphData.SetDrawStyle(	CGraphDrawStyle( m_eBongDrawStyle[ i]));
				}
			}
		}
	}
}

void CMainBlockImp::DrawBlocks(CDC* pDC, const CDrawingOption::DRAWINGTYPE eDrawingType, const bool bIsCalDataForDrawing)
{
	// 모두 그리기
	if(eDrawingType == CDrawingOption::ALL)
	{
		DrawBlocks_DrawAllBlocksBackground(pDC);
		DrawBlocks_DrawBlockCompartLine(pDC);
	}
	// 바탕만 그리기
	else if(eDrawingType == CDrawingOption::BACKGROUND)
	{
		DrawBlocks_DrawAllBlocksBackground(pDC);
		DrawBlocks_DrawBlockCompartLine(pDC);
	}
	// 가로, 세로 스케일 그리기
	else if(eDrawingType == CDrawingOption::SCALE)
	{	
		// (2009/9/6 - Seung-Won, Bae) for Block Maximizing.
		if( GetMaximizedBlock().IsAllValueBiggerThanZero())
		{
			CBlockImp *pBlockImp = ( CBlockImp *)GetBlock( GetMaximizedBlock());
			if( pBlockImp)
			{
				pBlockImp->DrawHorzAndVertScale( pDC, m_blocksPosition.GetBlockRegion( GetMaximizedBlock()));
				return;
			}
		}

		//sy test
		int blockColumnIndex = 0;
		POSITION pos = m_BlocksColumnList.GetHeadPosition();
		while(pos != NULL)
		{
			CBlocksColumn* pBlocksColumn = (CBlocksColumn*) m_BlocksColumnList.GetNext(pos);
			assert(pBlocksColumn != NULL);
			pBlocksColumn->DrawAllHorzAndVertScales(pDC, blockColumnIndex, m_blocksPosition);
			blockColumnIndex++;
		}
	}
	// Graph 그리기
	else if(eDrawingType == CDrawingOption::GRAPH)
	{
		CXScaleManager* pxScaleManager = GetXScaleManager();
		//20100522 이문수 >>
		//업종 차트추가모드에서 화학을 추가할 때 다운되는 현상처리
		if(!pxScaleManager)
			return;
		//20100522 이문수 <<

		// (2009/10/9 - Seung-Won, Bae) for Thread Draw.
		if( THREAD_CALCULATION == pxScaleManager->GetThreadFlag())
			m_nThreadCount = 0;

		// (2009/9/6 - Seung-Won, Bae) for Block Maximizing.
		if( GetMaximizedBlock().IsAllValueBiggerThanZero())
		{
			CBlockImp *pBlockImp = ( CBlockImp *)GetBlock( GetMaximizedBlock());
			if( pBlockImp)
				pBlockImp->DrawAllGraphs( pDC, m_blocksPosition.GetBlockRegion( GetMaximizedBlock()));
		}
		else
		{
			//sy test
			int blockColumnIndex = 0;
			POSITION pos = m_BlocksColumnList.GetHeadPosition();
			while(pos != NULL)
			{
				CBlocksColumn* pBlocksColumn = (CBlocksColumn*) m_BlocksColumnList.GetNext(pos);
				assert(pBlocksColumn != NULL);

				pBlocksColumn->DrawAllGraphs(pDC, blockColumnIndex, m_blocksPosition);
				blockColumnIndex++;
			}
		}

		// (2009/10/9 - Seung-Won, Bae) for Thread Draw.
		//20100506 이문수 교착상태의심 쓰레드 사용중단 >>
		//if( THREAD_CALCULATION == pxScaleManager->GetThreadFlag())
		//	while( 0 < m_nThreadCount) Sleep( 1);

	}
}

// vert scale Min/Max 를 계산한다.
void CMainBlockImp::CalculateDisplayDataMinMaxAllVertScaleGroup()
{
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		pBlocksColumn->CalculateDisplayDataMinMaxAllVertScaleGroup();
	}
}

// drawing를 위한 data 계산한다.
// -> VertScale Min/Max, Drawing Region
void CMainBlockImp::CalculateDataForDrawing(CDC* pDC)
{
	//sy test
	// VertScale Min/Max
	CalculateDisplayDataMinMaxAllVertScaleGroup();

	// Drawing Region
	// 가로, 세로 scale영역의 기본 Size Setting. (min/max)
	SetBaseScaleRegion( pDC);

	// (2009/2/19 - Seung-Won, Bae) Set BlockCount for Multi-Column X Scale Manager.
	CXScaleManager* pxScaleManager = GetXScaleManager();
	if( pxScaleManager) pxScaleManager->SetBlockColumnCount( m_BlocksColumnList.GetCount());

	// 각 영역을 구하고 min/max 를 구한다.
	int blockColumnIndex = 0;
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = (CBlocksColumn*) m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		pBlocksColumn->CalculateDrawingRegion( pDC, blockColumnIndex, m_blocksPosition);

		// X-Scale (yiilyoul@magicn.com - 20070223)
		// 화면 사이즈가 변할경우 X-Scale의 표시 갯수도 변할수 있기 때문에 이를 처리해 줍니다.(봉폭 고정 기능)
		CBlockIndex bi( 0, blockColumnIndex);
		
		// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
		if( pxScaleManager) pxScaleManager->SetColumnWidth( blockColumnIndex, m_blocksPosition.GetBlockRegion( bi).Width());

		CRect rtRegion = GetNoFrameGraphRegion( bi);
		// (2009/1/14 - Seung-Won, Bae) Do not set recalc to TRUE for avoding the deadlock.
		if( pxScaleManager) pxScaleManager->SetScreenRegion( blockColumnIndex, rtRegion.left, rtRegion.Width(), FALSE);

		blockColumnIndex++;
	}

	if( pxScaleManager)
	{
		int nDRI_Start, nDRI_End;
		nDRI_Start = pxScaleManager->GetStartDrawIdx_Cur();
		nDRI_End = pxScaleManager->GetEndDrawIdx_Cur();
		SetDataRegionInBlock2( nDRI_Start, nDRI_End);
	}
}

// 해당 RQ만 작업 : 복수종목 - ojtaso (20070420)
bool CMainBlockImp::GetViewDataListInAllBlock(LPCTSTR lpszRQ, CViewData& viewData) const
{
	viewData.RemoveAll();
	
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		CColumnViewData columnViewData;
		pBlocksColumn->GetColumnViewData(lpszRQ, columnViewData);

		if(columnViewData.GetCount())
			viewData.AddTail(columnViewData);
	}
	return (viewData.GetColumnViewDataCount() > 0);
}

CBlockIndex CMainBlockImp::FindBlockNumber(CBlockImp* pBlock) const
{
	if(pBlock == NULL) return CBlockIndex(-1, -1);

	CGraphSearchInfo_Block gsi;
	gsi.m_pBlockImp = pBlock;
	if( !FindGraph( &gsi)) return CBlockIndex( -1, -1);
	
	return CBlockIndex( gsi.m_nRowBlockIndex, gsi.m_nColumnBlockIndex);
}

// point에 해당되는 구분선을 찾기
CBlockIndex CMainBlockImp::FindCompartLine(const CPoint& point) const
{
	return m_blocksPosition.FindCompartLine(point);
}

// block move region을 찾는다.
CBlockIndex CMainBlockImp::FindBlockMovingRegion(const CPoint& point) const
{
	CBlockIndex blockIndex = FindBlockIndex(point);
	if( !blockIndex.IsAllValueBiggerThanZero()) return CBlockIndex(-1, -1);

	// (2006/10/12 - Seung-Won, Bae) Use CGraphRegions
	CBlockImp* pBlock = ( CBlockImp *)GetBlock( blockIndex);
	if( pBlock == NULL) return CBlockIndex(-1, -1);

	// block move 영역
	CRect selectRegion = CBlockDrawer().GetBlockMoveRegion( pBlock->GetVertScaleGroupList().GetGraphRegions().GetNoFrameRegion().TopLeft());
	if( selectRegion.PtInRect(point)) return blockIndex;

	return CBlockIndex(-1, -1);
}

CBlockIndex CMainBlockImp::FindBlockDeletingRegion(const CPoint& point) const
{
	CBlockIndex blockIndex = FindBlockIndex(point);
	if(!blockIndex.IsAllValueBiggerThanZero())
		return CBlockIndex(-1, -1);

	// (2006/10/12 - Seung-Won, Bae) Use CGraphRegions
	CBlockImp* pBlock = ( CBlockImp *)GetBlock( blockIndex);
	if( pBlock == NULL) return CBlockIndex(-1, -1);
	CRect blockRegion = pBlock->GetVertScaleGroupList().GetGraphRegions().GetNoFrameRegion();
	CRect deleteRegion = CBlockDrawer().GetBlockDeleteRegion( CPoint( blockRegion.right, blockRegion.top));
	if( deleteRegion.PtInRect(point)) return blockIndex;

	return CBlockIndex(-1, -1);
}

// 블럭 최대화 추가 - ojtaso (20080519)
CBlockIndex CMainBlockImp::FindBlockSizingRegion(const CPoint& point) const
{
	CBlockIndex blockIndex = FindBlockIndex(point);
	if(!blockIndex.IsAllValueBiggerThanZero())
		return CBlockIndex(-1, -1);

	// (2006/10/12 - Seung-Won, Bae) Use CGraphRegions
	CBlockImp* pBlock = ( CBlockImp *)GetBlock( blockIndex);
	if( pBlock == NULL) return CBlockIndex(-1, -1);
	CRect blockRegion = pBlock->GetVertScaleGroupList().GetGraphRegions().GetNoFrameRegion();
	CRect sizeRegion = CBlockDrawer().GetBlockSizeRegion( CPoint( blockRegion.right, blockRegion.top));
	if( sizeRegion.PtInRect(point)) return blockIndex;

	return CBlockIndex(-1, -1);
}

bool CMainBlockImp::MoveBlock(const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex)
{
	if(!sourceBlockIndex.IsAllValueBiggerThanZero() || !targetBlockIndex.IsAllValueBiggerThanZero())
		return false;

	// columnBlock 간의 이동
	if( sourceBlockIndex.GetColumn() != targetBlockIndex.GetColumn())
	{
		int sourceColumnIndex = sourceBlockIndex.GetColumn();
		int targetColumnIndex = targetBlockIndex.GetColumn();

		// block 위치 및 값 변경
		if( !m_blocksPosition.MoveBlockPosition(sourceColumnIndex, targetColumnIndex, CBlockBaseData::COLUMN)) return false;

		POSITION sourcePos = m_BlocksColumnList.FindIndex( sourceColumnIndex);
		POSITION targetPos = m_BlocksColumnList.FindIndex( targetColumnIndex);
		bool bInsertBefore = (sourceColumnIndex > targetColumnIndex);

		CBlocksColumn* pSourceBlocksColomn = m_BlocksColumnList.GetAt( sourcePos);
		if(bInsertBefore) m_BlocksColumnList.InsertBefore(targetPos, pSourceBlocksColomn);
		else m_BlocksColumnList.InsertAfter(targetPos, pSourceBlocksColomn);

		m_BlocksColumnList.RemoveAt(sourcePos);

		return true;
	}

	// 같은 column 안에서 block간의 이동
	// column 안의 block 이동
	POSITION blocksColumnPos = m_BlocksColumnList.FindIndex( sourceBlockIndex.GetColumn());
	int nSourceBlockIndex = sourceBlockIndex.GetRow();
	int nTargetBlockIndex = targetBlockIndex.GetRow();

	if( blocksColumnPos == NULL || nSourceBlockIndex == nTargetBlockIndex) return false;

	CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetAt(blocksColumnPos);
	assert(pBlocksColumn != NULL);

	return pBlocksColumn->MoveBlock(&m_blocksPosition, nSourceBlockIndex, nTargetBlockIndex);
}

// 구분선을 찾은 경우 point 위치로 block 위치를 변경
bool CMainBlockImp::MoveBlocksPosition(const CPoint& point, const CBlockIndex& compartLineIndex)
{
	return m_blocksPosition.MoveBlocksPosition(point, compartLineIndex);
}

void CMainBlockImp::SetAllHorzScaleGridType(const CScaleBaseData::SCALEGRIDTYPE gridType)
{
	//모든 graph의 grid type를 수정한다.
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		pBlocksColumn->SetAllHorzScaleGridType(gridType);
	}
}

void CMainBlockImp::SetAllHorzScalePosition(const CScaleBaseData::HORZSCALEPOSITION ePosition)
{
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		pBlocksColumn->SetAllHorzScalePosition(ePosition);
	}
}

// scale을 표현할 data (ex) "자료일자" 등
bool CMainBlockImp::SetHorzScaleData(const CBlockIndex& blockIndex, const CString& strScaleData)
{
	CBlockImp* pBlock = ( CBlockImp *)GetBlock(blockIndex);
	if(pBlock == NULL)
		return false;

	pBlock->GetBlockData().GetHorzScale().SetData(strScaleData);
	return true;
}

// scale data
CString CMainBlockImp::GetHorzScaleData( const CBlockIndex& blockIndex) const
{
	CBlockImp* pBlock = ( CBlockImp *)GetBlock(blockIndex);
	if(pBlock == NULL)
		return "";

	return pBlock->GetBlockData().GetHorzScale().GetScaleData();
}

// 수치조회창의 자료일자 Format, X Scale 설정값에 따라 처리 - ojtaso (20070827)
void CMainBlockImp::GetHorzScaleCompart( const CBlockIndex& blockIndex, CString& strCompartDate, CString& strCompartTime)
{
	CBlockImp* pBlock = ( CBlockImp *)GetBlock(blockIndex);
	if(pBlock == NULL)
		return;

	CScaleCompart& scaleCompart = pBlock->GetBlockData().GetHorzScale().GetScaleCompart();
	strCompartDate = scaleCompart.GetDateCompart();
	strCompartTime = scaleCompart.GetTimeCompart();
}

void CMainBlockImp::SetHorzScaleCompart(const CBlockIndex& blockIndex, const CString& dateCompart, const CString& timeCompart, const CScaleBaseData::TEXTCOMPARTTYPE eTextCompart)
{
	CBlockImp* pBlock = ( CBlockImp *)GetBlock(blockIndex);
	if(pBlock == NULL)
		return;

	pBlock->GetBlockData().GetHorzScale().GetScaleCompart().SetDateCompart(dateCompart);
	pBlock->GetBlockData().GetHorzScale().GetScaleCompart().SetTimeCompart(timeCompart);
	pBlock->GetBlockData().GetHorzScale().GetScaleCompart().SetScaleTextType(eTextCompart);
}

bool CMainBlockImp::SetHorzScalePosition(const CBlockIndex& blockIndex, const CScaleBaseData::HORZSCALEPOSITION ePosition)
{
	CBlockImp* pBlock = ( CBlockImp *)GetBlock(blockIndex);
	if(pBlock == NULL)
		return false;

	// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
	//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
	SetHorzScalePosition( pBlock->GetBlockData().GetHorzScale(), ePosition);
	return true;
}

// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
void CMainBlockImp::SetHorzScalePosition( CHorizontalScale &p_horizontalScale, const CScaleBaseData::HORZSCALEPOSITION ePosition)
{
	m_blocksPosition.SetHorzScalePosition( p_horizontalScale, ePosition);
}

CScaleBaseData::HORZSCALEPOSITION CMainBlockImp::GetHorzScalePosition( const CBlockIndex& blockIndex) const
{
	CBlockImp* pBlock = ( CBlockImp *)GetBlock(blockIndex);
	if(pBlock == NULL) return CScaleBaseData::HORZ_NONE;

	return pBlock->GetBlockData().GetHorzScale().GetHorzScalePosition();
}

CScaleBaseData::SCALEGRIDTYPE CMainBlockImp::GetFirstBlockHGridLineType( void)
{
	CBlockImp* pBlock = ( CBlockImp *)GetBlock( CBlockIndex( 0, 0));
	if( pBlock == NULL) return CScaleBaseData::SLNONE;

	return pBlock->GetBlockData().GetHorzScale().GetGridType();
}

void CMainBlockImp::SetHorizontalScaleHeight(short nBlockRow, short nBlockCol, long nHeight)
{
	CBlockImp* pBlock = ( CBlockImp *)GetBlock( CBlockIndex( nBlockRow, nBlockCol));
	if( pBlock)
	{
		pBlock->GetBlockData().SetHorizontalScaleHeight( nHeight);
		m_blocksPosition.ClearRowReal();
	}
}


// block의 세로 scale의 log type setting.
bool CMainBlockImp::SetAllVertScaleLogType(const bool bIsLogType)
{
	bool bResult = false;
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		if(pBlocksColumn->SetAllVertScaleLogType(bIsLogType))
			bResult = true;
	}
	return bResult;
}

bool CMainBlockImp::SetVertScaleLogType(const CBlockIndex& blockIndex, const bool bIsLogType)
{
	CBlockImp* pBlock = ( CBlockImp *)GetBlock( blockIndex);
	if(pBlock == NULL) return false;

	pBlock->GetBlockData().SetVertScaleLogType( bIsLogType);
	return true;
}

bool CMainBlockImp::SetVertScaleLogType(const CString& strGraphName, const bool bIsLogType)
{
	if( strGraphName.IsEmpty()) return false;
	
	CGraphSearchInfo_GraphName gsi;
	gsi.m_strGraphName = strGraphName;
	if( !FindGraph( &gsi)) return false;
	if( !gsi.m_pBlockImp) return false;

	gsi.m_pBlockImp->GetBlockData().SetVertScaleLogType( bIsLogType);
	return true;
}

bool CMainBlockImp::SetVertScaleLogAndInvertType(const CString& strGraphName, const bool bIsLogType, const bool bIsInvertType)
{
	if( strGraphName.IsEmpty()) return false;
	
	CGraphSearchInfo_GraphName gsi;
	gsi.m_strGraphName = strGraphName;
	if( !FindGraph( &gsi)) return false;
	if( !gsi.m_pBlockImp) return false;
	if( !gsi.m_pVertScaleGroup) return false;

	gsi.m_pBlockImp->GetBlockData().SetVertScaleLogType(bIsLogType);
	gsi.m_pVertScaleGroup->SetVertScaleInvertType(bIsInvertType);
	return true;
}

// block의 세로 scale의 invert type setting.
bool CMainBlockImp::SetAllVertScaleInvertType(const CBlockIndex& blockIndex, const bool bIsInvertType)
{
	CBlockImp* pBlock = ( CBlockImp *)GetBlock( blockIndex);
	if(pBlock == NULL)
		return false;

	pBlock->GetVertScaleGroupList().SetAllVertScaleInvertType(bIsInvertType);
	return true;
}

void CMainBlockImp::SetAllVertScaleGridType(const CScaleBaseData::SCALEGRIDTYPE gridType)
{
	// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
	//	m_mainBlockBaseData.GetScaleAttributes().SetBaseScaleGridType(gridType);
	
	//모든 graph의 grid type를 수정한다.
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		pBlocksColumn->SetAllVertScaleGridType(gridType);
	}
}

void CMainBlockImp::SetAllVertScalePosition( const CScaleBaseData::VERTSCALEPOSITION ePosition)
{
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		pBlocksColumn->SetAllVertScalePosition( ePosition);
	}
}

// scale을 표현할 data (ex) "지표 Data", "종가", "시가,고가,저가," 등
bool CMainBlockImp::SetVertScaleData(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const CString& strScaleData)
{
	CVertScaleGroup *pVertScaleGroup = GetVertScaleGroup( blockIndex, nVertScaleGroupIndex);
	if( !pVertScaleGroup) return false;

	pVertScaleGroup->GetVertScale().SetData( strScaleData);
	return true;
}

// 호가단위, 기준가 사용 - ojtaso (20070917)
void CMainBlockImp::SetVertScaleType(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nScaleType, const double dTypeValue)
{
	CVertScaleGroup *pVertScaleGroup = GetVertScaleGroup( blockIndex, nVertScaleGroupIndex);
	if(pVertScaleGroup)
	{
		CVerticalScale& vertScale = pVertScaleGroup->GetVertScale();
		vertScale.SetVertUserHogaScaleType(nScaleType);
		if(nScaleType == 0)
			vertScale.SetVertScaleUserNumber(dTypeValue);
		else		
			vertScale.SetVertScaleHoGaNumber(dTypeValue);
	}
}

bool CMainBlockImp::SetVertScalePosition(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const CScaleBaseData::VERTSCALEPOSITION ePosition)
{
	CVertScaleGroup *pVertScaleGroup = GetVertScaleGroup( blockIndex, nVertScaleGroupIndex);
	if( !pVertScaleGroup) return false;

	pVertScaleGroup->GetVertScale().SetVertScalePosition(ePosition);
	return true;
}

CScaleBaseData::VERTSCALEPOSITION CMainBlockImp::GetVertScalePosition(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex) const
{
	if( !blockIndex.IsAllValueBiggerThanZero() || nVertScaleGroupIndex < 0) return CScaleBaseData::VERT_NONE;

	CVertScaleGroup* pVertScaleGroup = GetVertScaleGroup( blockIndex, nVertScaleGroupIndex);
	if( !pVertScaleGroup) return CScaleBaseData::VERT_NONE;
	return pVertScaleGroup->GetVertScale().GetVertScalePosition();
}

CScaleBaseData::SCALEGRIDTYPE CMainBlockImp::GetFirstBlockVGridLineType( void)
{
	if( !m_BlocksColumnList.IsEmpty())
	{
		CBlocksColumn *pBlockColumn = m_BlocksColumnList.GetHead();
		if( pBlockColumn) return pBlockColumn->GetFirstBlockVGridLineType();
	}
	return CScaleBaseData::SHORTDOTTEDLINE;
}

BOOL CMainBlockImp::GetVertScaleSpace( const int p_nR, const int p_nC, const int p_nV, int &p_nUpSpace, int &p_nDownSpace)
{
	CVertScaleGroup *pVertScaleGroup = GetVertScaleGroup( CBlockIndex( p_nR, p_nC), p_nV);
	if( !pVertScaleGroup) return FALSE;
	pVertScaleGroup->GetVertScale().GetSpace( p_nUpSpace, p_nDownSpace);
	return TRUE;
}

BOOL CMainBlockImp::SetVertScaleSpace( const int p_nR, const int p_nC, const int p_nV, const int p_nUpSpace, const int p_nDownSpace)
{
	CVertScaleGroup *pVertScaleGroup = GetVertScaleGroup( CBlockIndex( p_nR, p_nC), p_nV);
	if( !pVertScaleGroup) return FALSE;
	CVerticalScale &vertScale = pVertScaleGroup->GetVertScale();
	if( !vertScale.IsVertScaleMinMaxLock()) vertScale.SetSpace( p_nUpSpace, p_nDownSpace);
	return TRUE;
}

bool CMainBlockImp::GetAllIndicatorName(CList<CString, CString>& strIndicatorNameList) const
{
	strIndicatorNameList.RemoveAll();
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		pBlocksColumn->GetAllIndicatorName(strIndicatorNameList);
	}
	return (strIndicatorNameList.GetCount() > 0);
}

// (2008/9/14 - Seung-Won, Bae) for Pair Tick
bool CMainBlockImp::GetAllIndicatorAndGraphNameWithRQ(CList<CString, CString>& strIndicatorNameList, CList<CString, CString>& strGraphNameList, const char *p_szRQ)
{
	strIndicatorNameList.RemoveAll();

	if( !p_szRQ) return false;
	if( !*p_szRQ) return false;

	CGraphSearchInfo_AnyGraph gsi;
	while( FindGraph( &gsi))
	{
		if( gsi.m_pGraphImp)
		{
			CPacketRQ *pPacketRQ = gsi.m_pGraphImp->GetPacketRQ();
			if( pPacketRQ)
			{
				const char *szRQ = pPacketRQ->GetRQ();
				if( szRQ)
				{
					if( *szRQ)
					{
						if( !strcmp( szRQ, p_szRQ))
						{
							CIndicatorInfo *pIndicatorInfo = gsi.m_pGraphImp->GetIndicatorInfo();
							if( pIndicatorInfo)
							{
								strIndicatorNameList.AddTail( pIndicatorInfo->GetIndicatorName());
								strGraphNameList.AddTail( pIndicatorInfo->GetGraphName());
							}
						}
					}
				}
			}
		}
		gsi.SetNextStartGraph();
	}
	return (strIndicatorNameList.GetCount() > 0);
}

bool CMainBlockImp::GetAllGraphName(CList<CString, CString>& strGraphNameList) const
{
	strGraphNameList.RemoveAll();
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		pBlocksColumn->GetAllGraphName(strGraphNameList);
	}
	return (strGraphNameList.GetCount() > 0);
}

BOOL CMainBlockImp::IsExistenceGraph( const CString &strGraphName) const
{
	if( strGraphName.IsEmpty()) return false;
	
	CGraphSearchInfo_GraphName gsi;
	gsi.m_strGraphName = strGraphName;
	return FindGraph( &gsi);
}

BOOL CMainBlockImp::IsExistenceGraphInBlock( const CString &strGraphName, const CBlockIndex& blockIndex) const
{
	if( strGraphName.IsEmpty() || blockIndex.GetRow() < 0 || blockIndex.GetColumn() < 0) return false;

	CBlockImp *pBlock = ( CBlockImp *)GetBlock( blockIndex);
	if(pBlock == NULL) return false;

	CGraphSearchInfo_GraphName gsi;
	gsi.m_strGraphName = strGraphName;
	return pBlock->GetVertScaleGroupList().FindGraph( &gsi);
}

BOOL CMainBlockImp::IsExistenceIndicator( const CString &strIndicatorName) const
{
	if( strIndicatorName.IsEmpty()) return false;

	CGraphSearchInfo_IndicatorName gsi;
	gsi.m_strIndicatorName = strIndicatorName;
	return FindGraph( &gsi);
}

// 복수종목별 검사 : 복수종목 - ojtaso (20070502)
BOOL CMainBlockImp::IsExistenceIndicator(LPVOID pPacketRQ, const CString &strIndicatorName) const
{
	if( strIndicatorName.IsEmpty()) return false;

	CGraphSearchInfo_IndicatorName gsi;
	gsi.m_strIndicatorName = strIndicatorName;
	return FindGraph(pPacketRQ, &gsi);
}

// 해당 point가 있는 block에 해당 지표가 있는지의 유무
BOOL CMainBlockImp::IsExistenceIndicatorInBlock( const CString &strIndicatorName, const CPoint& point) const
{
	if( strIndicatorName.IsEmpty()) return false;
	CBlockIndex bi = FindBlockIndex( point);
	if( bi.GetRow() < 0 || bi.GetColumn() < 0) return false;
	CBlockImp *pBlock = ( CBlockImp *)GetBlock( bi);
	if( !pBlock) return false;

	CGraphSearchInfo_IndicatorName gsi;
	gsi.m_strIndicatorName = strIndicatorName;
	return pBlock->GetVertScaleGroupList().FindGraph( &gsi);
}

BOOL CMainBlockImp::IsExistenceIndicatorInBlock( const CString &strIndicatorName, const CBlockIndex& blockIndex) const
{
	if( strIndicatorName.IsEmpty() || blockIndex.GetRow() < 0 || blockIndex.GetColumn() < 0) return false;

	CBlockImp *pBlock = ( CBlockImp *)GetBlock( blockIndex);
	if( !pBlock) return false;

	CGraphSearchInfo_IndicatorName gsi;
	gsi.m_strIndicatorName = strIndicatorName;
	return pBlock->GetVertScaleGroupList().FindGraph( &gsi);
}

// RQ로 지표검색 : 복수종목 - ojtaso (20070109)
BOOL CMainBlockImp::IsExistenceIndicatorInBlock(LPCTSTR lpszRQ, const CString &strIndicatorName, const CBlockIndex& blockIndex) const
{
	if( strIndicatorName.IsEmpty() || blockIndex.GetRow() < 0 || blockIndex.GetColumn() < 0) return false;

	CBlockImp *pBlock = ( CBlockImp *)GetBlock( blockIndex);
	if( !pBlock) return false;

	CGraphSearchInfo_IndicatorNameWithRQ gsi;
	gsi.m_strRQ				= lpszRQ;
	gsi.m_strIndicatorName	= strIndicatorName;

	return pBlock->GetVertScaleGroupList().FindGraph( &gsi);
}

// graph의 indicatorInfo 초기화
void CMainBlockImp::Initialize_IndicatorInfo(const bool bIsNULL)
{
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		pBlocksColumn->Initialize_IndicatorInfo(bIsNULL);
	}
}

bool CMainBlockImp::ChangeGraphData( const CString& targetGraphName, const int nSubGraphIndex, const CString& sourcePacketNames,
											   const CString& sourceSubGraphTitle, const CGraphBaseData::GRAPHTYPE& sourceType, const CGraphStyle& sourceStyle,
											   const CGraphDrawStyle& sourceDrawStyle, const CPenThickness& penThickness)
{
	if( targetGraphName.IsEmpty()) return false;
	CGraphSearchInfo_GraphName gsi;
	gsi.m_strGraphName = targetGraphName;
	if( !FindGraph( &gsi)) return false;
	if( !gsi.m_pGraphImp) return false;

	gsi.m_pGraphImp->SetPacketNames( sourcePacketNames);
	return gsi.m_pGraphImp->GetSubGraphList().SetData( nSubGraphIndex, sourceSubGraphTitle, sourceType, sourceStyle, sourceDrawStyle, penThickness);
}

bool CMainBlockImp::ChangeGraphDataFromIndicatorInfo( const CString& strGraphName, const bool bIsCalculate)
{
	CGraphSearchInfo_GraphName gsi;
	gsi.m_strGraphName = strGraphName;
	if( !FindGraph( &gsi)) return false;
	if( !gsi.m_pGraphImp) return false;

	return gsi.m_pGraphImp->ChangeGraphDataFromIndicatorInfo( bIsCalculate);
}

// (2009/9/27 - Seung-Won, Bae) Get RQ of Graph.
const char *CMainBlockImp::GetRQofGraph( const char *p_szGraphName)
{
	CGraphSearchInfo_GraphName gsi;
	gsi.m_strGraphName = p_szGraphName;
	if( !FindGraph( &gsi)) return NULL;
	if( !gsi.m_pGraphImp) return NULL;
	return gsi.m_pGraphImp->GetGraphRQ();
}

// graph type 변경.
bool CMainBlockImp::ChangeGraphType( const CString& targetGraphName, const CGraphBaseData::GRAPHTYPE& sourceType, const CGraphStyle& sourceStyle,
													const CGraphDrawStyle& sourceDrawStyle, const CString& sourcePacketNames)
{
	if( targetGraphName.IsEmpty()) return false;
	if( !( sourceType >=0 && sourceStyle.GetGraphStyle() >= 0 && sourceDrawStyle.GetDrawStyle() >= 0)) return false;

	CGraphSearchInfo_GraphName gsi;
	gsi.m_strGraphName = targetGraphName;
	if( !FindGraph( &gsi)) return false;
	if( !gsi.m_pGraphImp) return false;

	gsi.m_pGraphImp->SetPacketNames( sourcePacketNames);
	gsi.m_pGraphImp->GetSubGraphList().SetType(0, sourceType, sourceStyle, sourceDrawStyle);

//{{ (2007/5/28 - LYH) For Weighted Volume Chart
//	CheckWeighted..()
//	가격차트의 것 확인해서 바뀌어야 하는 경우만하세요...
	if(sourceType == CGraphBaseData::Bong_Type)
		SetWeightedVolume(sourceStyle.GetGraphBongStyle());
//}}
	return true;
}

// strPacketName 가 포함된 모든 subGraph의 packet들을 초기화
bool CMainBlockImp::Initialize_Packet(const CString& strPacketName, const bool bIsCalculate)
{
	bool bResult = false;
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		if(pBlocksColumn->Initialize_Packet(strPacketName, bIsCalculate))
			bResult = true;
	}
	return bResult;
}

bool CMainBlockImp::Initialize_AllPackets( const CString& strGraphName, const bool bIsCalculate)
{
	if( strGraphName.IsEmpty()) return FALSE;

	CGraphSearchInfo_GraphName gsi;
	gsi.m_strGraphName = strGraphName;
	if( !FindGraph( &gsi)) return false;
	if( !gsi.m_pGraphImp) return false;

	gsi.m_pGraphImp->Initialize_AllPackets( bIsCalculate);
	return true;
}

// graph packetName
CString CMainBlockImp::GetGraphPacketName(const CString& strGraphName) const
{
	if( strGraphName.IsEmpty()) return "";
	CGraphSearchInfo_GraphName gsi;
	gsi.m_strGraphName = strGraphName;
	if( !FindGraph( &gsi)) return "";
	if( !gsi.m_pGraphImp) return "";

	return gsi.m_pGraphImp->GetGraphData().GetPacketNames();
}

CString CMainBlockImp::GetGraphPacketName_TitleName( const CString& strSubGraphTitle) const
{
	if( strSubGraphTitle.IsEmpty()) return "";
	CGraphSearchInfo_SubGraphTitle gsi;
	gsi.m_strSubGraphTitle = strSubGraphTitle;
	if( !FindGraph( &gsi)) return "";
	if( !gsi.m_pGraphImp) return "";

	return gsi.m_pGraphImp->GetGraphData().GetPacketNames();
}

CString CMainBlockImp::GetGraphPacketName_TitleName(const CBlockIndex& blockIndex, const CString& strSubGraphTitle) const
{
	if( strSubGraphTitle.IsEmpty()) return "";
	CBlockImp* pBlock = ( CBlockImp *)GetBlock( blockIndex);
	if( !pBlock == NULL) return "";

	CGraphSearchInfo_SubGraphTitle gsi;
	gsi.m_strSubGraphTitle = strSubGraphTitle;
	if( !pBlock->GetVertScaleGroupList().FindGraph( &gsi)) return "";
	if( !gsi.m_pGraphImp) return "";

	return gsi.m_pGraphImp->GetGraphData().GetPacketNames();
}

// Change Input Packet Names and Calculate again.
BOOL CMainBlockImp::ChangeIndicatorPacketNames( const char *p_szIndicatorName, const int p_nSubGraphIndex, const char *p_szPacketNames)
{
	if( !p_szIndicatorName || !p_szPacketNames) return FALSE;
	if( !*p_szIndicatorName || !*p_szPacketNames) return FALSE;

	BOOL bResult = FALSE, bResult2 = FALSE;
	CBlocksColumn *pBlocksColumn = NULL;
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while( pos)
	{
		pBlocksColumn = m_BlocksColumnList.GetNext( pos);
		if( !pBlocksColumn) continue;
		bResult2 = pBlocksColumn->ChangeIndicatorPacketNames( p_szIndicatorName, p_nSubGraphIndex, p_szPacketNames);
		bResult = bResult || bResult2;
	}
	return bResult;
}

// (2007/3/28 - Seung-Won, Bae) Retrieve Packet Name of SubGraph
CString CMainBlockImp::GetSubGraphPacketName( const char *p_szGraphName, const char *p_szSubGraphName, const int p_nPacketIndex)
{
	if( !p_szGraphName || !p_szSubGraphName || p_nPacketIndex < 0) return "";
	if( !*p_szGraphName || !*p_szSubGraphName) return "";

	CGraphSearchInfo_GraphNameAndSubGraphName gsi;
	gsi.m_strGraphName = p_szGraphName;
	gsi.m_strSubGraphName = p_szSubGraphName;
	if( !FindGraph( &gsi)) return "";
	if( !gsi.m_pSubGraph) return "";

	CSubGraphPacketList &subGraphPacketList = gsi.m_pSubGraph->GetSubGraphPacketList();
	if( subGraphPacketList.GetCount() <= p_nPacketIndex) return "";

	CPacket *pPacket = subGraphPacketList.GetPacket( p_nPacketIndex);
	if( !pPacket) return "";

	return pPacket->GetName();
}

// (2007/4/30 - Seung-Won, Bae) Get Data Index of Inverse Line Graph
int CMainBlockImp::GetDataIndexOfInverseLine( CPoint p_pt)
{
	CSelectedSubGraphDeque selectedSubGraphDeque;
	if( !FindSubGraph( p_pt, selectedSubGraphDeque)) return -1;
	if( selectedSubGraphDeque.GetCount() <= 0) return -1;
	CSelectedSubGraphData selectedSubGraphData = selectedSubGraphDeque.GetTail();

	CSubGraph *pSubGraph = selectedSubGraphData.GetSubGraph();
	if( !pSubGraph) return -1;
	CGraphImp *pGraph = pSubGraph->GetGraph();
	if( !pGraph) return -1;
	if( pGraph->GetGraphData().GetIndicatorName() != _MTEXT( C2_INVERSE_TIME_LINE)) return -1;

	return selectedSubGraphData.GetDataIndex();
}

// data min/max
bool CMainBlockImp::GetDisplayDataMinMax(const CString& GraphName, double& dMin, double& dMax) const
{
	if( GraphName.IsEmpty()) return false;
	
	CGraphSearchInfo_GraphName gsi;
	gsi.m_strGraphName = GraphName;
	if( !FindGraph( &gsi)) return false;
	if( !gsi.m_pVertScaleGroup) return false;

	CDisplayDataMinMax vertDisplayDataMinMax = gsi.m_pVertScaleGroup->GetDisplayDataMinMax();
	dMin = vertDisplayDataMinMax.GetDisplayMin();
	dMax = vertDisplayDataMinMax.GetDisplayMax();
	return true;
}

bool CMainBlockImp::ChangeGraphDataDrawingData(const CString& p_strIndicatorName, const bool bLowHigh, const bool bLowCur, const bool bHighCur, const bool bPrevCur, const int nCurData)
{
	CGraphSearchInfo_IndicatorName gsi;
	gsi.m_strIndicatorName = p_strIndicatorName;
	
	// (2007/4/12 - Seung-Won, Bae) Support Sequencial Search.
	while( FindGraph( &gsi))
	{
		if( !gsi.m_pGraphImp) return false;

		CGraphDataDrawingData& sourceGraphDrawingData = gsi.m_pGraphImp->GetGraphData().GetGraphDataDrawingData();
		sourceGraphDrawingData.SetDoesCalcRiseFallRate_LowHigh( bLowHigh);
		sourceGraphDrawingData.SetDoesCalcRiseFallRate_LowCur( bLowCur);
		sourceGraphDrawingData.SetDoesCalcRiseFallRate_HighCur( bHighCur);
		sourceGraphDrawingData.SetDoesCalcRate_PrevCur( bPrevCur);
		sourceGraphDrawingData.SetCalcRiseFallRate_CurData( nCurData);
		gsi.m_pGraphImp->SetGraphDataDrawingData( sourceGraphDrawingData);

		gsi.SetNextStartGraph();
	}
	return true;
}


// GraphDataDrawingData 변경.
bool CMainBlockImp::ChangeGraphDataDrawingData(const CString& p_strIndicatorName, const CGraphDataDrawingData& sourceGraphDrawingData)
{
	CGraphSearchInfo_IndicatorName gsi;
	gsi.m_strIndicatorName = p_strIndicatorName;
	
	// (2007/4/12 - Seung-Won, Bae) Support Sequencial Search.
	while( FindGraph( &gsi))
	{
		if( !gsi.m_pGraphImp) return false;
		gsi.m_pGraphImp->SetGraphDataDrawingData( sourceGraphDrawingData);

		gsi.SetNextStartGraph();
	}
	return true;
}

// GraphDataDraw 여부 추가 - ojtaso (20080609)
bool CMainBlockImp::ShowGraphDataDrawingData(const CString& strRQ, const CString& p_strIndicatorName, const bool bShow, const int nShowType)
{
	CGraphSearchInfo_IndicatorNameWithRQ gsi;
	gsi.m_strRQ = strRQ;
	gsi.m_strIndicatorName = p_strIndicatorName;
	
	// (2007/4/12 - Seung-Won, Bae) Support Sequencial Search.
	while( FindGraph( &gsi))
	{
		if( !gsi.m_pGraphImp) return false;
		gsi.m_pGraphImp->ShowGraphDataDrawingData(bShow, nShowType);

		gsi.SetNextStartGraph();
	}
	return true;
}

// subGraph Title을 이용하여 SubGraph 찾기.
CSubGraph* CMainBlockImp::FindSubGraphTitle(CDC* pDC, const CPoint& point, CRect& titleRegion) const
{
	CBlockIndex bi = FindBlockIndex( point);
	if( bi.GetRow() < 0 || bi.GetColumn() < 0) return NULL;
	CBlockImp *pBlock = ( CBlockImp *)GetBlock( bi);
	if( !pBlock) return NULL;

	return pBlock->GetVertScaleGroupList().FindSubGraphTitle( pDC, point, titleRegion);
}

// subGraphData
CSubGraphData* CMainBlockImp::GetSubGraphData(const CString& strGraphName, const CString& strSubGraphName) const
{
	if( strGraphName.IsEmpty() || strSubGraphName.IsEmpty()) return NULL;

	CGraphSearchInfo_GraphNameAndSubGraphName gsi;
	gsi.m_strGraphName		= strGraphName;
	gsi.m_strSubGraphName	= strSubGraphName;
	if( !FindGraph( &gsi)) return NULL;
	if( !gsi.m_pSubGraph) return NULL;
	
	return &gsi.m_pSubGraph->GetSubGraphData();
}

// subGraph data 변경.
bool CMainBlockImp::ChangeSubGraphData( const CString& targetGraphName, const CString& targetSubGraphName, const CSubGraphData& sourceSubGraphData)
{
	if( targetGraphName.IsEmpty() || targetSubGraphName.IsEmpty()) return false;

	CGraphSearchInfo_GraphNameAndSubGraphName gsi;
	gsi.m_strGraphName		= targetGraphName;
	gsi.m_strSubGraphName	= targetSubGraphName;
	if( !FindGraph( &gsi)) return false;
	if( !gsi.m_pSubGraph) return false;

	gsi.m_pSubGraph->GetSubGraphData() = sourceSubGraphData;
	return true;
}

// graph type
bool CMainBlockImp::GetGraphType( const CString& strGraphName, const int nSubGraphIndex, CGraphBaseData::GRAPHTYPE& eGraphType, CGraphStyle& graphStyle, CGraphDrawStyle& graphDrawStyle) const
{
	if( strGraphName.IsEmpty() || nSubGraphIndex < 0) return false;

	CGraphSearchInfo_GraphNameAndSubGraphIndex gsi;
	gsi.m_strGraphName		= strGraphName;
	gsi.m_nSubGraphIndex	= nSubGraphIndex;
	if( !FindGraph( &gsi)) return false;
	if( !gsi.m_pSubGraph) return false;

	eGraphType		= gsi.m_pSubGraph->GetSubGraphData().GetType();
	graphStyle		= gsi.m_pSubGraph->GetSubGraphData().GetStyle();
	graphDrawStyle	= gsi.m_pSubGraph->GetSubGraphData().GetDrawStyle();
	return true;
}

// subGraph names (bIsViewable = true : 현재 보이고 있는 subGraph)
CString CMainBlockImp::GetSubGraphNames(const CString& strGraphName, const bool bIsViewable) const
{
	if( strGraphName.IsEmpty()) return "";

	CGraphSearchInfo_GraphName gsi;
	gsi.m_strGraphName = strGraphName;
	if( !FindGraph( &gsi)) return "";
	if( !gsi.m_pGraphImp) return "";

	if( bIsViewable) return gsi.m_pGraphImp->GetSubGraphList().GetViewableSubGraphNames();
	return gsi.m_pGraphImp->GetSubGraphList().GetSubGraphNames();
}

// subGraph의 title
bool CMainBlockImp::GetAllSubGraphTitle( const CString& strGraphName, CList<CString, CString>& titleList) const
{
	if( strGraphName.IsEmpty()) return false;

	CGraphSearchInfo_GraphName gsi;
	gsi.m_strGraphName = strGraphName;
	if( !FindGraph( &gsi)) return false;
	if( !gsi.m_pGraphImp) return false;
	
	return gsi.m_pGraphImp->GetSubGraphList().GetAllSubGraphTitle( titleList);
}

// subGraph title 변경.
bool CMainBlockImp::ChangeSubGraphTitle( const CString& orgSubGraphTitle, const CString& changeSubGraphTitle)
{
	if( orgSubGraphTitle.IsEmpty()) return false;

	CGraphSearchInfo_SubGraphTitle gsi;
	gsi.m_strSubGraphTitle = orgSubGraphTitle;
	if( !FindGraph( &gsi)) return false;
	if( !gsi.m_pSubGraph) return false;

	gsi.m_pSubGraph->GetSubGraphData().SetTitle( changeSubGraphTitle);
	return false;
}

bool CMainBlockImp::ChangeSubGraphTitle( const CString& strGraphName, const int nSubGraphIndex, const CString& changeSubGraphTitle)
{
	if( strGraphName.IsEmpty() || nSubGraphIndex < 0) return false;

	CGraphSearchInfo_GraphName gsi;
	gsi.m_strGraphName = strGraphName;
	if( !FindGraph( &gsi)) return false;
	if( !gsi.m_pGraphImp) return false;

	CSubGraph *pSubGraph = gsi.m_pGraphImp->GetSubGraphList().FindSubGraph( nSubGraphIndex);
	if( !pSubGraph) return false;

	pSubGraph->GetSubGraphData().SetTitle( changeSubGraphTitle);
	return true;
}

bool CMainBlockImp::ChangeSubGraphTitle( const CBlockIndex& blockIndex, const int nScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex, const CString& changeSubGraphTitle)
{
	CSubGraph *pSubGraph = GetSubGraph( blockIndex, nScaleGroupIndex, nGraphIndex, nSubGraphIndex);
	if( pSubGraph == NULL) return false;

	pSubGraph->GetSubGraphData().SetTitle( changeSubGraphTitle);
	return true;
}

bool CMainBlockImp::ChangeSubGraphTitleFromIndicator(const CString& strGraphName, const int nSubGraphIndex)
{
	if( strGraphName.IsEmpty() || nSubGraphIndex < 0) return false;

	CGraphSearchInfo_GraphName gsi;
	gsi.m_strGraphName = strGraphName;
	if( !FindGraph( &gsi)) return false;
	if( !gsi.m_pGraphImp) return false;
	
	return gsi.m_pGraphImp->ChangeSubGraphTitleFromIndicator( nSubGraphIndex);
}

// subGraph를 보이기 setting.
void CMainBlockImp::SetAllGraphsHiding(const CBlockIndex& blockIndex, const bool bIsHiding)
{
	CBlockImp* pBlock = ( CBlockImp *)GetBlock( blockIndex);
	if(pBlock == NULL)
		return;

	pBlock->GetVertScaleGroupList().SetAllGraphsHiding(bIsHiding);
}

bool CMainBlockImp::SetSubGraphHiding( const CString& strGraphName, const int nSubGraphIndex, const bool bIsHiding)
{
	if( strGraphName.IsEmpty()) return false;

	CGraphSearchInfo_GraphNameAndSubGraphIndex gsi;
	gsi.m_strGraphName		= strGraphName;
	gsi.m_nSubGraphIndex	= nSubGraphIndex;
	if( !FindGraph( &gsi)) return false;
	if( !gsi.m_pVertScaleGroup || !gsi.m_pGraphImp || !gsi.m_pSubGraph) return false;

	if( !gsi.m_pSubGraph->SetHiding( bIsHiding)) return false;
	gsi.m_pVertScaleGroup->CalculateDisplayDataMinMax();
	return true;
}

bool CMainBlockImp::SetSubGraphHiding(const CBlockIndex& blockIndex, const CString& subGraphTitle, const bool bIsHiding)
{
	// (2007/2/16 - Seung-Won, Bae) Do not skip no title SubGraph.
	//	if( subGraphTitle.IsEmpty()) return false;

	CBlockImp* pBlock = ( CBlockImp *)GetBlock( blockIndex);
	if( !pBlock) return false;

	// (2007/2/16 - Seung-Won, Bae) Set hide the All Graph with Same Name.
	return pBlock->GetVertScaleGroupList().SetSubGraphHiding( subGraphTitle, bIsHiding);
}

bool CMainBlockImp::SetSubGraphHiding(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex, const bool bIsHiding)
{
	CVertScaleGroup *pVerticalScaleGroup = GetVertScaleGroup( blockIndex, nVertScaleGroupIndex);
	if( !pVerticalScaleGroup) return false;

	CGraphImp *pGraph = pVerticalScaleGroup->GetGraph( nGraphIndex);
	if( !pGraph) return false;

	CSubGraph *pSubGraph = pGraph->GetSubGraphList().GetSubGraph( nSubGraphIndex);
	if( !pSubGraph) return false;

	if( !pSubGraph->SetHiding( bIsHiding)) return false;
	pVerticalScaleGroup->CalculateDisplayDataMinMax();
	return true;
}

// (2007/2/20 - Seung-Won, Bae) Support Indicator Hiding
//		It can not be replaced with AddChartBlock(), because of Condition Reset.
void CMainBlockImp::ShowIndicator( const char *p_szIndicatorName, BOOL p_bShow, BOOL p_bRecalculate)
{
	if( !p_szIndicatorName) return;
	if( !*p_szIndicatorName) return;

	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		pBlocksColumn->ShowIndicator( p_szIndicatorName, p_bShow, p_bRecalculate);
	}
}

CGraphColor* CMainBlockImp::GetGraphColor(const CString& strGraphName, const int nSubGraphIndex) const
{
	if( strGraphName.IsEmpty() || nSubGraphIndex < 0) return NULL;
	CGraphSearchInfo_GraphNameAndSubGraphIndex gsi;
	gsi.m_strGraphName		= strGraphName;
	gsi.m_nSubGraphIndex	= nSubGraphIndex;
	if( !FindGraph( &gsi)) return NULL;
	if( !gsi.m_pSubGraph) return NULL;
	
	return &gsi.m_pSubGraph->GetSubGraphData().GetColor();
}

bool CMainBlockImp::ChangeGraphColor1( const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex, const COLORREF& newGraphColor1)
{
	CGraphImp *pGraph = GetGraph( blockIndex, nVertScaleGroupIndex, nGraphIndex);
	if( pGraph == NULL) return false;

	CGraphColor *pGraphColor = pGraph->GetSubGraphList().GetGraphColor( nSubGraphIndex);
	if(pGraphColor == NULL) return false;

	pGraphColor->SetColor1( newGraphColor1);
	return true;
}

// (2007/2/22 - Seung-Won, Bae) Support Graph Color Changing with Graph Title
BOOL CMainBlockImp::ChangeGraphColor( const char *p_szGraphTitle, const char *p_szColors)
{
	if( !p_szColors) return FALSE;
	if( !*p_szColors) return FALSE;

	CGraphSearchInfo_SubGraphTitle gsi;
	gsi.m_strSubGraphTitle = p_szGraphTitle;

	CString strColors( p_szColors);
	strColors.Replace( '.', ',');

	// (2007/4/12 - Seung-Won, Bae) Support Sequencial Search.
	while( FindGraph( &gsi))
	{
		if( gsi.m_pSubGraph) gsi.m_pSubGraph->GetSubGraphData().GetColor().SetData( strColors, FALSE);

		gsi.SetNextStartSubGraph();
	}
	return TRUE;
}

// 가지고 있는 graph GraphName
CString CMainBlockImp::GetIndicatorName(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nGraphIndex) const
{
	CGraphImp *pGraph = GetGraph( blockIndex, nVertScaleGroupIndex, nGraphIndex);
	if( !pGraph) return "";
	return pGraph->GetGraphData().GetIndicatorName();
}

// -> IndicatorInfo를 gaphData(subGraph포함)로 변경.
bool CMainBlockImp::ChangeAllIndicatorInfoFromGraphData( void)
{
	if(GetIndicatorList() == NULL)
		return false;

	bool bResult = false;
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		if(pBlocksColumn->ChangeAllIndicatorInfoFromGraphData())
			bResult = true;
	}
	return bResult;
}

BOOL CMainBlockImp::ChangeAllGraphDataFromIndicatorInfo( BOOL p_bWithCalculate)
{
	if( GetIndicatorList() == NULL) return FALSE;

	BOOL bResult = FALSE;
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		if( pBlocksColumn->ChangeAllGraphDataFromIndicatorInfo( p_bWithCalculate)) bResult = TRUE;
	}

	return bResult;
}

bool CMainBlockImp::ChangeIndicatorInfoFromGraphData(const CString& strGraphName)
{
	if( strGraphName.IsEmpty()) return false;

	CGraphSearchInfo_GraphName gsi;
	gsi.m_strGraphName = strGraphName;
	if( !FindGraph( &gsi)) return false;
	if( !gsi.m_pGraphImp) return false;
	
	return gsi.m_pGraphImp->ChangeIndicatorInfoFromGraphData();
}

// -> 해당 graph를 Map Default IndicatorInfo로 변경
bool CMainBlockImp::ChangeAllMapDefaultIndicatorInfo( void)
{
	CIndicatorList* pIndicatorList = GetIndicatorList();
	if(pIndicatorList == NULL)
		return false;

	bool bResult = false;
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		if(pBlocksColumn->ChangeAllMapDefaultIndicatorInfo( pIndicatorList))
			bResult = true;
	}
	return bResult;
}

//////////////////////////////////////////////////////////////////////
// Calculate Indicator
//////////////////////////////////////////////////////////////////////

// 지표를 계산한다.
void CMainBlockImp::CalculateAllGraphs(const CCalculateBaseData::CALCULATETYPE eCalType)
{
	if(m_mainBlockBaseData.GetPacketList() == NULL || m_mainBlockBaseData.GetIndicatorList() == NULL)
		return;

	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		pBlocksColumn->CalculateAllGraphs(eCalType);
	}
}

bool CMainBlockImp::CalculateAllGraphs(const CList<CString, CString>& packetNameList, const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsHeadGraph)
{
	if(packetNameList.GetCount() <= 0)
		return false;

	bool bResult = false;
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		if(!pBlocksColumn->CalculateAllGraphs(packetNameList, eCalType, bIsHeadGraph))
			continue;

		bResult = true;
		if(bIsHeadGraph)
			return true;
	}
	return bResult;
}

bool CMainBlockImp::CalculateGraph(const CString& strGraphName, const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsClearPacketData)
{
	if( strGraphName.IsEmpty()) return false;
	CGraphSearchInfo_GraphName gsi;
	gsi.m_strGraphName = strGraphName;
	if( !FindGraph( &gsi)) return false;
	if( !gsi.m_pGraphImp) return false;

	return gsi.m_pGraphImp->CalculateGraph( eCalType, true, bIsClearPacketData, false);
}

// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
BOOL CMainBlockImp::SetDrawBaseLine(BOOL bDrawBaseLine)
{
	m_bDrawBaseLine = bDrawBaseLine ? true : false;
	
	return TRUE;
}

// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
bool CMainBlockImp::GetDrawBaseLine()
{
	return m_bDrawBaseLine; 
}

// VertScaleGroup의 가장 처음 Graph의 타입 : 복수종목 - ojtaso (20070504)
CGraphImp* CMainBlockImp::GetMasterGraph(const int nRow, const int nCol)
{
	CBlockIndex blockIndex(nRow, nCol);
	CBlockImp* pBlock = ( CBlockImp *)GetBlock(blockIndex);
	if( !pBlock) return NULL;

	CVertScaleGroup* pVertScaleGroup = pBlock->GetVertScaleGroupList().GetVertScaleGroup(pBlock->GetFirstVisibleVScaleIndex());
	if( !pVertScaleGroup) return NULL;
	return pVertScaleGroup->GetGraph(0);
}

// VertScaleGroup의 가장 처음 Graph의 타입 : 복수종목 - ojtaso (20070504)
CScaleBaseData::HORZSCALEDRAWERTYPE CMainBlockImp::GetMasterValueType(const int nRow, const int nCol)
{
	CGraphImp* pGraph = GetMasterGraph(nRow, nCol);

	if(!pGraph)
		return CScaleBaseData::HORZSCALEDRAWERTYPE::HORZ_NOTYPE;

	CPacketList* pPacketList = m_mainBlockBaseData.GetMainAttributes().GetPacketListManager()->GetPacketList(pGraph->GetGraphRQ());
	CPacket* pPacket = pPacketList->GetBaseData( _MTEXT( C0_DATE_TIME));

	if(!pPacket)
		return CScaleBaseData::HORZSCALEDRAWERTYPE::HORZ_NOTYPE;

	return (CScaleBaseData::HORZSCALEDRAWERTYPE)pPacket->GetDateUnitType();
}

// VertScaleGroup의 가장 처음 Graph의 타입 : 복수종목 - ojtaso (20070504)
LPCTSTR CMainBlockImp::GetMasterValueRQ(const int nRow, const int nCol)
{
	CGraphImp* pGraph = GetMasterGraph(nRow, nCol);
	if(!pGraph)
		return _T("DEFAULT");
	
	return pGraph->GetGraphRQ();
}

// 20081007 JS.Kim	사용자시간대 선택
CString CMainBlockImp::GetCurrentRQ( void)
{
	CPacketListManager *pPacketListManager = GetPacketListManager();
	if( !pPacketListManager) return "";
	return m_mainBlockBaseData.GetMainAttributes().GetIChartCtrl()->GetCurrentRQ();
}

//{{ (2007/5/28 - LYH) For Weighted Volume Chart
void CMainBlockImp::SetWeightedVolume(const int nGraphDrawStyle)
{
	CXScaleManager* pxScaleManager = GetXScaleManager();
	BOOL bWeightedVolume = pxScaleManager->GetWeightedVolume();
	if(nGraphDrawStyle == CGraphBaseData::CandleVolume || nGraphDrawStyle == CGraphBaseData::EquiVolume)
	{
		//{{ 2007.07.09 by LYH 거래량 패킷이 없는 경우 WeightedVolume 세팅 하지 않음
		//if(!bWeightedVolume)
		//	GetXScaleManager()->SetWeightedVolume(TRUE, TRUE);
		CPacketList* pPacketList = m_mainBlockBaseData.GetMainAttributes().GetPacketListManager()->GetPacketList("DEFAULT");
		CPacket* pPacket = pPacketList->GetBaseData( _MTEXT( C0_VOLUME));
		if(pPacket)
		{
			if(!bWeightedVolume)
				GetXScaleManager()->SetWeightedVolume(TRUE, TRUE);
		}
		//}}
	}
	else
	{
		if(bWeightedVolume)
			GetXScaleManager()->SetWeightedVolume(FALSE, TRUE);
	}
}
//}}

// 미래영역 스케일을 그릴것인지 : ojtaso (20070724)
BOOL CMainBlockImp::GetDrawFutureScale() const
{
	return m_bDrawFutureScale;
}

// 미래영역 스케일을 그릴것인지 : ojtaso (20070724)
void CMainBlockImp::SetDrawFutureScale(const BOOL bDrawFutureScale)
{
	m_bDrawFutureScale = bDrawFutureScale;
}

// 차트DLL에서 복수종목을 사용할지 : 복수종목 - ojtaso (20070904)
void CMainBlockImp::SetMultiItemMode(const BOOL bMultiItem)
{
	m_bUseMultiItemMode = bMultiItem;
}

// 차트DLL에서 복수종목을 사용할지 : 복수종목 - ojtaso (20070904)
BOOL CMainBlockImp::GetMultiItemMode() const
{
	return m_bUseMultiItemMode;
}

// 블럭의 VertScaleGroupCount를 얻음 - ojtaso (20080618)
long CMainBlockImp::GetVertScaleGroupCount(const CBlockIndex& blockIndex)
{
	CBlockImp* pBlock = (CBlockImp*)GetBlock(blockIndex);
	if(!pBlock)
		return -1;

	return pBlock->GetVertScaleGroupList().GetCount();
}

// (2009/6/25 - Seung-Won, Bae) for Balanced Row Arrange without scale.
void CMainBlockImp::GetSumOfHorzScaleHeightForEachBlock( CList< int, int> &p_lnSumOfHScaleHeight)
{
	POSITION posColumn = m_BlocksColumnList.GetHeadPosition();
	while( posColumn)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext( posColumn);
		assert(pBlocksColumn != NULL);
		pBlocksColumn->GetSumOfHorzScaleHeightForEachBlock( p_lnSumOfHScaleHeight);
	}
}

// 한번에 그래프 속성 변경 가능한 메쏘드 추가 - ojtaso (20080602)
void CMainBlockImp::ChangeGraphPropertyAtOnce(LPCTSTR lpszRQ, LPCTSTR lpszTargetTitle, LPCTSTR lpszNewTitle, short nGraphType, short nGraphStyle, short nDrawStyle, LPCTSTR lpszColors, short nThickness1, short nThickness2, short nThickness3) 
{
	CGraphSearchInfo_SubGraphTitleWithRQ gsi;
	gsi.m_strRQ = lpszRQ;
	gsi.m_strSubGraphTitle = lpszTargetTitle;
	if( !FindGraph( &gsi)) return;

	if(gsi.m_pSubGraph)
	{
		if(::lstrlen(lpszNewTitle) > 0)
			gsi.m_pSubGraph->GetSubGraphData().SetTitle(lpszNewTitle);
		
		if(::lstrlen(lpszColors) > 0)
			gsi.m_pSubGraph->GetSubGraphData().GetColor().SetData(lpszColors, FALSE);
	}

	if(gsi.m_pGraphImp)
		gsi.m_pGraphImp->ChangeGraphPropertyAtOnce(gsi.m_nSubGraphIndex, nGraphType, nGraphStyle, nDrawStyle, nThickness1, nThickness2, nThickness3);
}

// 한번에 그래프 속성 얻어올 수 메쏘드 추가 - (@Solomon-20100120)
bool CMainBlockImp::GetGraphPropertyAtOnce(LPCTSTR lpszRQ, LPCTSTR lpszTargetTitle, short &nGraphType, short &nGraphStyle, short &nDrawStyle, COLORREF &crColors, short &nThickness1, short &nThickness2, short &nThickness3)
{
	CGraphSearchInfo_SubGraphTitleWithRQ gsi;
	gsi.m_strRQ = lpszRQ;
	gsi.m_strSubGraphTitle = lpszTargetTitle;
	if( !FindGraph( &gsi)) return false;
	
	if(gsi.m_pSubGraph)
	{
		CGraphColor& getClr = gsi.m_pSubGraph->GetSubGraphData().GetColor();
		crColors = getClr.GetColor1();
	}
	
	if(gsi.m_pGraphImp)
		return gsi.m_pGraphImp->GetGraphPropertyAtOnce(gsi.m_nSubGraphIndex, nGraphType, nGraphStyle, nDrawStyle, nThickness1, nThickness2, nThickness3);

	return false;
}

// KTB 체결잔고표시 - ojtaso (20091019)
long CMainBlockImp::AddHorizontalLine(LPCSTR lpszRQ, LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice)
{
	CGraphSearchInfo_GraphNameWithRQ gsi;
	gsi.m_strRQ	= lpszRQ;
	gsi.m_strGraphName = _MTEXT(C2_PRICE_CHART);
	if(!FindGraph( &gsi)) 
		return -1;

	return gsi.m_pGraphImp->AddHorizontalLine(lpszTitle, clrLine, nLine, dPrice);
}

// KTB 체결잔고표시 - ojtaso (20091019)
void CMainBlockImp::DeleteHorizontalLine(LPCSTR lpszRQ, long nIndex)
{
	CGraphSearchInfo_GraphNameWithRQ gsi;
	gsi.m_strRQ	= lpszRQ;
	gsi.m_strGraphName = _MTEXT(C2_PRICE_CHART);
	if(!FindGraph( &gsi)) 
		return;

	gsi.m_pGraphImp->DeleteHorizontalLine(nIndex);
}

// KTB 체결잔고표시 - ojtaso (20091019)
void CMainBlockImp::DeleteAllHorizontalLine(LPCSTR lpszRQ)
{
	CGraphSearchInfo_GraphNameWithRQ gsi;
	gsi.m_strRQ	= lpszRQ;
	gsi.m_strGraphName = _MTEXT(C2_PRICE_CHART);
	if(!FindGraph( &gsi)) 
		return;

	gsi.m_pGraphImp->DeleteAllHorizontalLine();
}

// KTB 체결잔고표시 - ojtaso (20091019)
void CMainBlockImp::UpdateHorizontalLine(LPCSTR lpszRQ, long nIndex, LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice)
{
	CGraphSearchInfo_GraphNameWithRQ gsi;
	gsi.m_strRQ	= lpszRQ;
	gsi.m_strGraphName = _MTEXT(C2_PRICE_CHART);
	if(!FindGraph( &gsi)) 
		return;

	gsi.m_pGraphImp->UpdateHorizontalLine(nIndex, lpszTitle, clrLine, nLine, dPrice);
}


//20100518_솔로몬  해당 지표의 SubGraphIndex의 Hide 속성을 Get
BOOL CMainBlockImp::GetSubGraphHiding( const CString& strGraphName, const int nSubGraphIndex)
{
	if( strGraphName.IsEmpty()) return FALSE;

	CGraphSearchInfo_GraphNameAndSubGraphIndex gsi;
	gsi.m_strGraphName		= strGraphName;
	gsi.m_nSubGraphIndex	= nSubGraphIndex;
	if( !FindGraph( &gsi)) return FALSE;
	if( !gsi.m_pVertScaleGroup || !gsi.m_pGraphImp || !gsi.m_pSubGraph) return FALSE;

	return gsi.m_pSubGraph->GetHiding();
}

// Grid 시간구분선
CScaleBaseData::SCALEGRIDTYPE CMainBlockImp::GetFirstDailyScaleGridType(CScaleBaseData::SCALEGRIDTIMEGUBUN nType)
{
	CBlockImp* pBlock = ( CBlockImp *)GetBlock( CBlockIndex( 0, 0));
	if( pBlock == NULL) return CScaleBaseData::SLNONE;

	return pBlock->GetBlockData().GetHorzScale().GetPeriodGridType(nType);
}

BOOL CMainBlockImp::GetFirstDailyScaleTime(CScaleBaseData::SCALEGRIDTIMEGUBUN nType)
{
	CBlockImp* pBlock = ( CBlockImp *)GetBlock( CBlockIndex( 0, 0));
	if( pBlock == NULL) return CScaleBaseData::SLNONE;

	return pBlock->GetBlockData().GetHorzScale().GetCheckTime(nType);
}

const unsigned short CMainBlockImp::GetFirstBlockGridLineWidth_TimeGubun( int nType)
{
	CBlockImp* pBlock = ( CBlockImp *)GetBlock( CBlockIndex( 0, 0));
	if( pBlock == NULL) return 1;

	return pBlock->GetBlockData().GetHorzScale().GetGridWidth_Time((CScaleBaseData::SCALEGRIDTIMEGUBUN)nType);
}

void CMainBlockImp::SetAllDailyScaleGridType(const CScaleBaseData::SCALEGRIDTYPE gridType, const CScaleBaseData::SCALEGRIDTIMEGUBUN eTimeType, BOOL bCheck)
{
	int nCount = GetRowCount();
	CBlockImp* pBlock=NULL;
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		for (int i=0; i<nCount; i++)
		{
			pBlock = pBlocksColumn->GetBlock(i);

			if (pBlock)
				pBlock->GetBlockData().GetHorzScale().SetPeriodGridType(eTimeType, gridType, bCheck);
		}
	}
}

void CMainBlockImp::SetAllScaleGridWidth_TimeGubun(CScaleBaseData::SCALEGRIDTIMEGUBUN nType, const unsigned short nGridWidth)
{
	int nCount = GetRowCount();
	CBlockImp* pBlock=NULL;
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		for (int i=0; i<nCount; i++)
		{
			pBlock = pBlocksColumn->GetBlock(i);

			if (pBlock)
				pBlock->GetBlockData().GetHorzScale().SetGridWidth_Time((CScaleBaseData::SCALEGRIDTIMEGUBUN)nType, nGridWidth);
		}
	}
}

void CMainBlockImp::SetAllScaleGridColor_TimeGubun(CScaleBaseData::SCALEGRIDTIMEGUBUN nType, const unsigned long nGridColor)
{
	int nCount = GetRowCount();
	CBlockImp* pBlock=NULL;
	POSITION pos = m_BlocksColumnList.GetHeadPosition();
	while(pos != NULL)
	{
		CBlocksColumn* pBlocksColumn = m_BlocksColumnList.GetNext(pos);
		assert(pBlocksColumn != NULL);
		for (int i=0; i<nCount; i++)
		{
			pBlock = pBlocksColumn->GetBlock(i);

			if (pBlock)
				pBlock->GetBlockData().GetHorzScale().SetGridColor_Time((CScaleBaseData::SCALEGRIDTIMEGUBUN)nType, nGridColor);
		}
	}
}

const unsigned long CMainBlockImp::GetFirstBlockGridLineColor_TimeGubun( int nType)
{
	CBlockImp* pBlock = ( CBlockImp *)GetBlock( CBlockIndex( 0, 0));
	if( pBlock == NULL) return 1;

	return pBlock->GetBlockData().GetHorzScale().GetGridColor_Time((CScaleBaseData::SCALEGRIDTIMEGUBUN)nType);
}
