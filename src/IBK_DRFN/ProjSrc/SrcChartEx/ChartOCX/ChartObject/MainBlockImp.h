// MainBlockImp.h: interface for the CMainBlockImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINBLOCKIMP_H__50B9FD3A_84A6_4B65_98E0_B08B9A6CD78E__INCLUDED_)
#define AFX_MAINBLOCKIMP_H__50B9FD3A_84A6_4B65_98E0_B08B9A6CD78E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "../Include_Chart/Dll_Load/MainBlock.h"											// for CMainBlock
#include "../Include_Chart/Dll_Load/MainChartFormulate.h"

#include "ChartObjectBaseData.h"
#include "BlocksPosition.h"

class CGraphImp;
class CBlockImp;
class CSubGraph;
class CViewData;
class CBlocksColumn;
class CGraphSearchInfo;
class CSelectedSubGraphDeque;
class CXScaleManager;
interface IChartCtrl;

class CMainBlockImp : public CMainBlock
{
public:
	CMainBlockImp(IChartCtrl* p_pICharetCtrl);
	virtual ~CMainBlockImp();

protected:
	// block들의 모든 기본이 되는 data
	CMainBlockBaseData							m_mainBlockBaseData;
	// block + block 테두리의 굵기 + scale의 위치
	CBlocksPosition								m_blocksPosition;
	// 공식
	CMainChartFormulate							m_mainChartFormulate;
	// Column별로 list형으로 저장.
	CTypedPtrList< CObList, CBlocksColumn *>	m_BlocksColumnList;
	// ( 2006 / 09 / 26 - Sang-Woo, Cho ) 가격차트에서 최고, 최저가 Text가 보이게 할 것인지 말 것인지에 대한 값을 설정한다.
	BOOL										m_bShowPriceMinMaxText;
	// (2006/10/13 - Seung-Won, Bae) Manage Block Move & Delete Tool Flag for Position of Graph Drawing
	BOOL										m_bBlockMoveDelete;
	// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
	CScaleBaseData::SCALEGRIDTYPE				m_eLastHorzGridType;
	CScaleBaseData::SCALEGRIDTYPE				m_eLastVerticalGridType;
	// 미래영역 스케일을 그릴것인지 : ojtaso (20070724)
	BOOL										m_bDrawFutureScale;	
	// 차트DLL에서 복수종목을 사용할지 : 복수종목 - ojtaso (20070904)
	BOOL										m_bUseMultiItemMode;
	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	bool										m_bDrawBaseLine;
	// (2008/1/22 - Seung-Won, Bae) for Multi-Language
	HWND										m_hOcxWnd;

// Base Setting
public:
	// OCX
	void				SetIChartCtrl( IChartCtrl* p_pICharetCtrl);
	IChartCtrl *		GetIChartCtrl( void);
	void				SetOCXPath( const CString& strOCXPath);
	CString				GetOCXPath( void) const;
	// Packet Data DLL
	void			SetPacketList( CPacketList* pPacketList, const char *p_szRQ);
	CPacketList *	GetPacketList( void) const;
	// Indicator List DLL
	void				SetIndicatorList( CIndicatorList* pIndicatorList);
	CIndicatorList *	GetIndicatorList( void) const;
	// Chart Formulate DLL
	CMainChartFormulate *	GetMainChartFormulate( void) const;
	// X-Scale Manager
	void			SetXScaleManager(CXScaleManager* pxScaleManager);
	CXScaleManager* GetXScaleManager() const;
 
	// 복수종목 관리 : 복수종목 - ojtaso (20070111)
	void SetPacketListManager(CPacketListManager* pPacketListManager);
	CPacketListManager* GetPacketListManager() const;

	// ToolBarManager
	void			 SetToolBarManager(IToolBarManager* pToolBarManager);
	IToolBarManager* GetToolBarManager() const;

	// (2008/1/22 - Seung-Won, Bae) for Multi-Language
	HWND			GetParent_Handle( void);	

	// Basic Info
	CMainBlockBaseData *	GetMainBlockBaseData( void) const;
	void					SetBaseScaleRegion( CDC* pDC);
	bool					SetBlockEnvironment( CDC* pDC, const CRect& allBlockRegion, const CBlockBaseData::BLOCKTYPE blockType,
													const CBlockBaseData::BLOCKCOMPART blockCompart, const CBlockIndex& blockCount,
													const int graphRegionFrameThickness, const int interBlockHalfGap,
													const CGraphRegionColor& graphRegionColor);
	void					SetDesignMode( const bool designMode);
	bool					GetDesignMode( void) const;
	void					SetMessageBoxShow( const bool bIsShow);
	void					SetGraphBackgroundGap( const int nGap_x, const int nGap_y);
	int						GetInterBlockHalfGap( void) const;
	int						GetGraphRegionFrameThickness( void) const;
	void					SetAutoDeleteIndicatorInfo( const bool bIsAutoDelete);
	void					SetShowPriceMinMaxText( BOOL bShowPriceMinMaxText );
	BOOL					GetShowPriceMinMaxText( void );					
	void					SetBlockMoveDeleteFlag( BOOL p_bBlockMoveDelete);
	BOOL					GetBlockMoveDeleteFlag( void) const;
	// Block
	CBlockBaseData::BLOCKTYPE		GetBlockType() const;
	CBlockBaseData::BLOCKCOMPART	GetBlockCompartType() const;
	void							SetBlockCount( const CBlockIndex& blockCount);
	bool							IsEmpty( void) const;
	int								GetColumnCount( void) const;
	int								GetRowCount( void) const;
	int								GetRowCountInColumn( int p_nColumnIndex) const;
	void							SetMaxBlocksCountInView( const int maxBlockCount);
	int								GetMaxBlocksCountInView( void) const;
	void							SetDoubleSizeBlockNumber( const CBlockIndex& doubleSizeBlockIndex);
	CBlockIndex						GetDoubleSizeBlockNumber( void) const;
	int								GetDoubleSizeBlockRowNumber( void) const;
	void							SetBlocksPosition( const CString& blocksPosition);
	CString							GetBlocksPosition( void) const;
	// Region
	bool	SetAllBlockRegion( const CRect& allBlockRegion);
	CRect	GetAllBlockRegion( void) const;
	CRect	GetGraphRegionSumInColumn( const CPoint& point) const;
	CRect	GetBlockRegion( const CBlockIndex& blockIndex) const;
	CRect	GetNoFrameGraphRegion( const CBlockIndex& blockIndex) const;
	CRect	GetNoFrameGraphRegion( const CPoint& point) const;
	// Graph 분리/삽입 - ojtaso (20070731)
	CRect	GetNoFrameGraphRegion( const CPoint& point, int& nInsertCol, int& nInsertRow) const;
	CRect	GetDrawingGraphRegion( const CBlockIndex& blockIndex) const;
	// Scale
	void							SetDataRegionInBlock2( const int start, const int end);
	void							GetDisplayDataIndex( int& nDataStartInBlock, int& nDataEndInBlock) const;
	void							SetBaseHorzScalePosition();
	void							SetOnlyPriceVertScale( CDC* pDC, const bool bIsOnlyPrice);
	void							SetAllVScaleMinMaxType( CScaleBaseData::VERTMINMAXTYPE p_eVScaleMinMaxType, CDC* pDC);
	void							ReleaseAllVScaleSettingInFixedMinMax( CDC *pDC);
	// Greaph
	void	SetAllGraphsTitleHiding( const bool bIsAllHiding = true);
	void	SetGraphTitleDraw( const bool bIsCondition, const bool bIsData);
	void	EnableIndicatorCalculation( BOOL p_bEnableIndicatorCalculation);
	BOOL	IsEnableIndicatorCalculation( void) const;
			// (2007/2/22 - Seung-Won, Bae) Support Base Text Option
	void	ShowBaseText( BOOL p_bShow)	{	m_mainBlockBaseData.SetWithBaseText( p_bShow);	}
	// Color
	void				SetBackgroundColor( const COLORREF& color);
	COLORREF			GetBackgroundColor( void) const;
	void				SetGraphRegionColor( const CGraphRegionColor& graphRegionColor);
	CGraphRegionColor	GetGraphRegionColor( void) const;
	void				GetGraphRegionColor( COLORREF& backgroundColor, COLORREF& lineColor, COLORREF& frameColor) const;
	void				SetScaleColor( const COLORREF& horzGridColor, const COLORREF& vertGridColor, const COLORREF& textColor)
						{	CScaleColor scaleColor(horzGridColor, vertGridColor, textColor);
							SetScaleColor( scaleColor);	}
	void				SetScaleColor( const CScaleColor& scaleColor);
	CScaleColor &		GetScaleColor( void);
	void				GetScaleColor( COLORREF& horzGridColor, COLORREF& vertGridColor, COLORREF& textColor) const;
	void				SetEtcGraphColor( const COLORREF& graphColor1, const COLORREF& graphColor2, const COLORREF& graphColor3, const COLORREF& graphColor4, const COLORREF& graphColor5, const COLORREF& graphColor6)
						{	CEtcGraphColor etcGraphColor(graphColor1, graphColor2, graphColor3, graphColor4, graphColor5, graphColor6);
							SetEtcGraphColor(etcGraphColor);	}
	void				SetEtcGraphColor( const CEtcGraphColor& etcGraphColor);
	void				GetEtcGraphColor( COLORREF& graphColor1, COLORREF& graphColor2, COLORREF& graphColor3, COLORREF& graphColor4, COLORREF& graphColor5, COLORREF& graphColor6) const;
	// 미래영역 스케일을 그릴것인지 : ojtaso (20070724)
	BOOL				GetDrawFutureScale() const;
	void				SetDrawFutureScale(const BOOL bDrawFutureScale);

	// 차트DLL에서 복수종목을 사용할지 : 복수종목 - ojtaso (20070904)
	void				SetMultiItemMode(const BOOL bMultiItem);
	BOOL				GetMultiItemMode() const;
	
	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	BOOL				SetDrawBaseLine(BOOL bDrawBaseLine);
	bool				GetDrawBaseLine();

// Create
protected:
	// Graph
	BOOL	InsertGraph_GetGraphInfo( CString &p_strGraphDataBuffer, const CString &graphDataOrg, char *&p_szGraphData, CGraphBaseData::GRAPHTYPE &p_eGraphType, CString &p_strIndicatorName);
	// RQ로 검색 : 복수종목 - ojtaso (20070118)
	BOOL	InsertGraph_GetGraphInfo(LPVOID pPacketRQ, CString &p_strGraphDataBuffer, const CString &graphDataOrg, char *&p_szGraphData, CGraphBaseData::GRAPHTYPE &p_eGraphType, CString &p_strIndicatorName);
	BOOL	InsertGraph_InsertIndicatorPosition( const CGraphBaseData::GRAPHTYPE p_eGraphType, const char *p_szIndicatorName, char *p_szGraphData);
	// RQ로 검색 : 복수종목 - ojtaso (20070118)
	BOOL	InsertGraph_InsertIndicatorPosition(LPVOID pPacketRQ, const CGraphBaseData::GRAPHTYPE p_eGraphType, const char *p_szIndicatorName, char *p_szGraphData);
	BOOL	InsertGraph_ModifyBlock( char *p_szGraphData, const bool doesMaintainOldScale);
	// RQ로 검색 : 복수종목 - ojtaso (20070118)
	BOOL	InsertGraph_ModifyBlock(LPVOID pPacketRQ, char *p_szGraphData, const bool doesMaintainOldScale);
	void	InsertGraph_InsertBlock_ChangeVertScaleGroupData_Get( CString &p_strVertScaleData, CBlocksColumn* pBlocksColumn, const char *p_szScaleData, BOOL p_bPriceChartInsert) const;
	BOOL	InsertGraph_InsertBlock_ChangeVertScaleGroupData( CString &p_strVertScaleGroupData, CBlocksColumn* pBlocksColumn, const CString& graphData) const;
	BOOL	InsertGraph_InsertBlock_InsertRow( const CBlockIndex& insertPosition, const CString& graphData);
	// RQ로 검색 : 복수종목 - ojtaso (20070118)
	BOOL	InsertGraph_InsertBlock_InsertRow(LPVOID pPacketRQ, const CBlockIndex& insertPosition, const CString& graphData);
	BOOL	InsertGraph_InsertBlock_InsertColumn( const int nInsertPosition, const CString& graphData);
	// RQ로 검색 : 복수종목 - ojtaso (20070118)
	BOOL	InsertGraph_InsertBlock_InsertColumn(LPVOID pPacketRQ, const int nInsertPosition, const CString& graphData);
	BOOL	InsertGraph_InsertBlock( const CString &p_strGraphData, const bool doesMaintainOldScale);
	// RQ로 검색 : 복수종목 - ojtaso (20070118)
	BOOL	InsertGraph_InsertBlock(LPVOID pPacketRQ, const CString &p_strGraphData, const bool doesMaintainOldScale);
public:
	// Block
	void	MakeBlocksColumn( const CBlockIndex& blockCount);
	// Vertical Scale Group and Graph
	// 그래프 생성후 계산 여부 : xScaleManager - ojtaso (20070701)
	BOOL	BuildGraphsAndScales( const CString& data, BOOL p_bWithIndicatorInfoUpdate);
	void	GetAllBlocksData( CString &p_strAllBlockData) const;
	// Graph
	BOOL	InsertGraph( const CString& graphData, const CGraphBaseData::ObjectChange blockType, const bool doesMaintainOldScale);
	// RQ로 검색 : 복수종목 - ojtaso (20070118)
	BOOL	InsertGraph(LPVOID pPacketRQ, const CString& graphData, const CGraphBaseData::ObjectChange blockType, const bool doesMaintainOldScale);
	BOOL	InsertGraphInBlock( const CBlockIndex& blockIndex, const int p_nVertScaleGroupIndex, const CString& graphData, const bool doesInsertInNewBlock,
								const bool p_doesInsertInNewVertScaleGroup, const bool doesMaintainOldScale);
	// RQ를 가지고 있는 Graph 생성 : 복수종목 - ojtaso (20061224)
 	// CPacketRQ*로 변경 : 복수종목 - ojtaso (20070109)
 	BOOL	InsertGraphInBlock(LPVOID pPacketRQ, const CBlockIndex& blockIndex, const int p_nVertScaleGroupIndex, const CString& graphData, const bool doesInsertInNewBlock,
 								const bool p_doesInsertInNewVertScaleGroup, const bool doesMaintainOldScale);

// Delete
public:
	// Block
	bool	DeleteBlock( CBlockImp* pBlock, const bool bAutoHorzScaleDelete = true);				// called from Block.
	bool	DeleteBlock(const CBlockIndex& blockIndex, const bool bAutoHorzScaleDelete = true);
	bool	DeleteAllBlock();

	// 블럭 최대화 추가 - ojtaso (20080519)
	void		SetMaximizedBlock( const CBlockIndex& maxBlockIndex);
	CBlockIndex	GetMaximizedBlock( void) const;

	// Graph
	bool	DeleteGraph(const CString& strGraphName);
	BOOL	DeleteAllIndicator( const CString& strIndicatorName);
	bool	DeleteGraphWithSubGraphTitle( const CBlockIndex& blockIndex, const CString& strTitleName, const bool bIsOnlyNoTRData = true);
	BOOL	RemoveAllGraphOfRQ( LPVOID lpPacketRQ);	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	// RQ 지정 그래프 삭제 메소드 추가 - ojtaso (20080604)
	bool	DeleteGraphWithSubGraphTitleRQ(LPCTSTR lpszRQ, const CBlockIndex& blockIndex, const CString& strTitleName, const bool bIsOnlyNoTRData = true);

// Search
protected:
	// Block Column
	CBlocksColumn *	GetBlocksColumn( const int nBlockColumnIndex) const;
	// Vertical Scale Group
	CVertScaleGroup *	GetVertScaleGroup( const CBlockIndex& blockIndex, const int nScaleGroupIndex) const;
	// Graph
	CGraphImp *	GetGraph( const CBlockIndex& blockIndex, const int nScaleGroupIndex, const int nGraphIndex) const;
	// Sub Graph
	CSubGraph *	GetSubGraph( const CBlockIndex& blockIndex, const int nScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex) const;
	// VertScaleGroup의 가장 처음 Graph의 타입 : 복수종목 - ojtaso (20070504)
	CGraphImp* GetMasterGraph(const int nRow, const int nCol);

public:
	// Block
	CBlockIndex FindBlockIndex( const CPoint &point) const;
	CBlock *	GetBlock( const CBlockIndex &blockIndex) const;
	// Graph
	BOOL	FindGraph( CGraphSearchInfo *p_pGraphSearchInfo) const;
	// RQ로 검색 : 복수종목 - ojtaso (20070118)
	BOOL		FindGraph(LPVOID pPacketRQ,  CGraphSearchInfo *p_pGraphSearchInfo) const;
	BOOL		GetIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG);
	BOOL		FindFirstIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, CString &p_strGraphName, const char *p_szRQ);
	BOOL		FindNextIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, CString &p_strGraphName, const char *p_szRQ);
	CString		FindGraph( const CPoint &p_pt) const;
	// SubGraph
	bool		FindSubGraph( const CPoint& point, CSelectedSubGraphDeque& selectedSubGraphDeque) const;

	// VertScaleGroup의 가장 처음 Graph의 타입 : 복수종목 - ojtaso (20070504)
	CScaleBaseData::HORZSCALEDRAWERTYPE GetMasterValueType(const int nRow, const int nCol);
	LPCTSTR		GetMasterValueRQ(const int nRow, const int nCol);
	// 20081007 JS.Kim	사용자시간대 선택
	CString		GetCurrentRQ( void);

// Move
public:
	// Block
	bool	MoveAllVertScaleGroups( const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex);
	bool	MoveAllVertScaleGroupsToHeadBlock( const bool bDoesMaintainOldScale = true);
	bool	MoveAllVertScaleGroupsToEachRowBlock( const CBlockIndex& targetBlockIndex, const bool bDoesMaintainOldScale = true);
	// Graph
	bool	JoinAllVertScaleGroups( const CBlockIndex& blockIndex, const bool bIsJoin = true);
	bool	MoveAllGraphs( const CBlockIndex& sourceBlockIndex, const int nSourceGroupIndex, const CBlockIndex& targetBlockIndex, const int nTargetGroupIndex = -1);
	bool	MoveGraph( const CBlockIndex& sourceBlockIndex, const int nSourceGroupIndex, const int nSourceGraphIndex, const CBlockIndex& targetBlockIndex, const int nTargetGroupIndex = -1);
	bool	MoveGraph2( const CBlockIndex& sourceBlockIndex, const int nSourceGroupIndex, const int nSourceGraphIndex, const CBlockIndex& targetBlockIndex, const int nTargetGroupIndex = -1, const BOOL bIsAddTail = FALSE );
	
// Query & Update
public:
	// Draw
	void	DrawBlocks_DrawAllBlocksBackground( CDC* pDC);
	void	DrawBlocks_DrawBlockCompartLine( CDC* pDC);
	void	CalculateDataForDrawing( CDC* pDC);
	void	DrawBlocks( CDC* pDC, const CDrawingOption::DRAWINGTYPE eDrawingType, const bool bIsCalDataForDrawing);
	void	CalculateDisplayDataMinMaxAllVertScaleGroup( void);
	void	ChangeDrawStyle(int nDrawStyle);
	// All
	// 해당 RQ만 작업 : 복수종목 - ojtaso (20070420)
	bool	GetViewDataListInAllBlock(LPCTSTR lpszRQ, CViewData& viewData) const;
	// Block
	CBlockIndex FindBlockNumber( CBlockImp* pBlock) const;
	CBlockIndex	FindCompartLine( const CPoint& point) const;
	CBlockIndex	FindBlockMovingRegion( const CPoint& point) const;
	CBlockIndex	FindBlockDeletingRegion( const CPoint& point) const;
	// 블럭 최대화 추가 - ojtaso (20080519)
	CBlockIndex FindBlockSizingRegion(const CPoint& point) const;
	bool		MoveBlock( const CBlockIndex& sourceBlockIndex, const CBlockIndex& targetBlockIndex);
	bool		MoveBlocksPosition( const CPoint& point, const CBlockIndex& compartLineIndex);
	// Horizontal Scale
	void								SetAllHorzScaleGridType( const CScaleBaseData::SCALEGRIDTYPE gridType);
	void								SetAllHorzScalePosition( const CScaleBaseData::HORZSCALEPOSITION ePosition);
	bool								SetHorzScaleData( const CBlockIndex& blockIndex, const CString& strScaleData);
	CString								GetHorzScaleData( const CBlockIndex& blockIndex) const;
	// 수치조회창의 자료일자 Format, X Scale 설정값에 따라 처리 - ojtaso (20070827)
	void								GetHorzScaleCompart( const CBlockIndex& blockIndex, CString& strCompartDate, CString& strCompartTime);
	void								SetHorzScaleCompart( const CBlockIndex& blockIndex, const CString& dateCompart, const CString& timeCompart, const CScaleBaseData::TEXTCOMPARTTYPE eTextCompart = CScaleBaseData::HORZ_TEXT_COMPART);
	bool								SetHorzScalePosition( const CBlockIndex& blockIndex, const CScaleBaseData::HORZSCALEPOSITION ePosition);
										// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
										//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
	void								SetHorzScalePosition( CHorizontalScale &p_horizontalScale, const CScaleBaseData::HORZSCALEPOSITION ePosition);
	CScaleBaseData::HORZSCALEPOSITION	GetHorzScalePosition(const CBlockIndex& blockIndex = CBlockIndex(0, 0)) const;
	CScaleBaseData::SCALEGRIDTYPE		GetFirstBlockHGridLineType( void);
	void								SetHorizontalScaleHeight( short nBlockRow, short nBlockCol, long nHeight);
	// Log and Invert Type
	bool		SetAllVertScaleLogType( const bool bIsLogType);
	bool		SetVertScaleLogType( const CBlockIndex& blockIndex, const bool bIsLogType);
	bool		SetVertScaleLogType( const CString& strGraphName, const bool bIsLogType);
	bool		SetVertScaleLogAndInvertType( const CString& strGraphName, const bool bIsLogType, const bool bIsInvertType);
	bool		SetAllVertScaleInvertType( const CBlockIndex& blockIndex, const bool bIsInvertType);
	// Vertical Scale Group
	void								SetAllVertScaleGridType( const CScaleBaseData::SCALEGRIDTYPE gridType);
	void								SetAllVertScalePosition( const CScaleBaseData::VERTSCALEPOSITION ePosition);
	bool								SetVertScaleData( const CBlockIndex& blockIndex, const int nVertScaleGroupIndex,
															const CString& strScaleData);
	// 호가단위, 기준가 사용 - ojtaso (20070917)
	void								SetVertScaleType(const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, 
															const int nScaleType, const double dTypeValue);
	bool								SetVertScalePosition( const CBlockIndex& blockIndex, const int nVertScaleGroupIndex,
																const CScaleBaseData::VERTSCALEPOSITION ePosition);
	CScaleBaseData::VERTSCALEPOSITION	GetVertScalePosition(const CBlockIndex& blockIndex = CBlockIndex(0, 0),
																const int nVertScaleGroupIndex = 0) const;
	CScaleBaseData::SCALEGRIDTYPE		GetFirstBlockVGridLineType( void);
	BOOL								GetVertScaleSpace( const int p_nR, const int p_nC, const int p_nV, int &p_nUpSpace, int &p_nDownSpace);
	BOOL								SetVertScaleSpace( const int p_nR, const int p_nC, const int p_nV, const int p_nUpSpace, const int p_nDownSpace);
	// Graph All Info
	bool	GetAllIndicatorName(CList<CString, CString>& strIndicatorNameList) const;
	bool	GetAllIndicatorAndGraphNameWithRQ(CList<CString, CString>& strIndicatorNameList, CList<CString, CString>& strGraphNameList, const char *p_szRQ);	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	bool	GetAllGraphName( CList<CString, CString> &strGraphNameList) const;
	BOOL	IsExistenceGraph( const CString &strGraphName) const;
	BOOL	IsExistenceGraphInBlock( const CString &strGraphName, const CBlockIndex& blockIndex) const;
	BOOL	IsExistenceIndicator( const CString &strIndicatorName) const;
	// 복수종목별 검사 : 복수종목 - ojtaso (20070502)
	BOOL	IsExistenceIndicator(LPVOID pPacketRQ, const CString &strIndicatorName) const;
	BOOL	IsExistenceIndicatorInBlock( const CString &strIndicatorName, const CPoint& point) const;
	BOOL	IsExistenceIndicatorInBlock( const CString &strIndicatorName, const CBlockIndex &blockIndex) const;
	// RQ로 지표검색 : 복수종목 - ojtaso (20070109)
	BOOL	IsExistenceIndicatorInBlock(LPCTSTR lpszRQ, const CString &strIndicatorName, const CBlockIndex &blockIndex) const;
	void	Initialize_IndicatorInfo( const bool bIsNULL);
	bool	ChangeGraphData(const CString& targetGraphName, const int nSubGraphIndex, const CString& sourcePacketNames, const CString& sourceSubGraphTitle, 
				const CGraphBaseData::GRAPHTYPE& sourceType, const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle, 
				const int nPenThickness1, const int nPenThickness2, const int nPenThickness3)
				{	return ChangeGraphData(targetGraphName, nSubGraphIndex, sourcePacketNames, sourceSubGraphTitle, sourceType, sourceStyle,
						sourceDrawStyle, CPenThickness(nPenThickness1, nPenThickness2, nPenThickness3, false));	}
	bool	ChangeGraphData( const CString& targetGraphName, const int nSubGraphIndex, const CString& sourcePacketNames, const CString& sourceSubGraphTitle, 
								const CGraphBaseData::GRAPHTYPE& sourceType, const CGraphStyle& sourceStyle, const CGraphDrawStyle& sourceDrawStyle,
								const CPenThickness& penThickness);
	bool	ChangeGraphDataFromIndicatorInfo( const CString& strGraphName, const bool bIsCalculate);
	const char *	GetRQofGraph( const char *p_szGraphName);	// (2009/9/27 - Seung-Won, Bae) Get RQ of Graph.
	// Graph Type & Packet
	bool	ChangeGraphType( const CString& targetGraphName, const CGraphBaseData::GRAPHTYPE& sourceType, const CGraphStyle& sourceStyle,
								const CGraphDrawStyle& sourceDrawStyle, const CString& sourcePacketNames);
	// Packet
	bool	Initialize_Packet( const CString& strPacketName, const bool bIsCalculate);
	bool	Initialize_AllPackets( const CString& strGraphName, const bool bIsCalculate);
	CString	GetGraphPacketName( const CString& strGraphName) const;
	CString	GetGraphPacketName_TitleName( const CString& strSubGraphTitle) const;
	CString	GetGraphPacketName_TitleName( const CBlockIndex& blockIndex, const CString& strSubGraphTitle) const;
	BOOL	ChangeIndicatorPacketNames( const char *p_szIndicatorName, const int p_nSubGraphIndex, const char *p_szPacketNames);
	CString	GetSubGraphPacketName( const char *p_szGraphName, const char *p_szSubGraphName, const int p_nPacketIndex);	// (2007/3/28 - Seung-Won, Bae) Retrieve Packet Name of SubGraph
	int		GetDataIndexOfInverseLine( CPoint p_pt);	// (2007/4/30 - Seung-Won, Bae) Get Data Index of Inverse Line Graph
	// Min & Max
	bool	GetDisplayDataMinMax( const CString& GraphName, double& dMin, double& dMax) const;
	// Graph Drawing Data
	bool	ChangeGraphDataDrawingData(const CString& p_strIndicatorName, const bool bLowHigh, const bool bLowCur, const bool bHighCur, const bool bPrevCur, const int nCurData);
	// GraphDataDraw 여부 추가 - ojtaso (20080609)
	bool	ShowGraphDataDrawingData(const CString& strRQ, const CString& p_strIndicatorName, const bool bShow, const int nShowType);
	bool	ChangeGraphDataDrawingData( const CString& p_strIndicatorName, const CGraphDataDrawingData& sourceGraphDrawingData);
	// Sub Graph
	CSubGraph *	FindSubGraphTitle( CDC* pDC, const CPoint& point, CRect& titleRegion) const;
	// Sub Graph Data
	CSubGraphData *	GetSubGraphData( const CString& strGraphName, const CString& strSubGraphName) const;
	bool			ChangeSubGraphData( const CString& targetGraphName, const CString& targetSubGraphName, const CSubGraphData& sourceSubGraphData);
	bool			GetGraphType( const CString& strGraphName, const int nSubGraphIndex, CGraphBaseData::GRAPHTYPE& eGraphType, CGraphStyle& graphStyle, CGraphDrawStyle& graphDrawStyle) const;
	CString			GetSubGraphNames(const CString& strGraphName, const bool bIsViewable = true) const;
	// Sub Graph Title
	bool	GetAllSubGraphTitle( const CString& strGraphName, CList<CString, CString>& titleList) const;
	bool	ChangeSubGraphTitle( const CString &orgSubGraphTitle, const CString &changeSubGraphTitle);
	bool	ChangeSubGraphTitle( const CString& strGraphName, const int nSubGraphIndex, const CString& changeSubGraphTitle);
	bool	ChangeSubGraphTitle( const CBlockIndex& blockIndex, const int nScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex, const
									CString& changeSubGraphTitle);
	bool	ChangeSubGraphTitleFromIndicator(const CString& strGraphName, const int nSubGraphIndex);
	// Sub Graph Show/Hide
	void	SetAllGraphsHiding( const CBlockIndex& blockIndex, const bool bIsHiding = true);
	bool	SetSubGraphHiding( const CString& strGraphName, const int nSubGraphIndex, const bool bIsHiding = true);
	bool	SetSubGraphHiding( const CBlockIndex& blockIndex, const CString& subGraphTitle, const bool bIsHiding = true);
	bool	SetSubGraphHiding( const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex, const bool bIsHiding = true);
			// (2007/2/20 - Seung-Won, Bae) Support Indicator Hiding
			//		It can not be replaced with AddChartBlock(), because of Condition Reset.
	void	ShowIndicator( const char *p_szIndicatorName, BOOL p_bShow, BOOL p_bRecalculate);
	// Sub Graph Color
	CGraphColor *	GetGraphColor( const CString& strGraphName, const int nSubGraphIndex = 0) const;
	bool			ChangeGraphColor1( const CBlockIndex& blockIndex, const int nVertScaleGroupIndex, const int nGraphIndex, const int nSubGraphIndex, const COLORREF& newGraphColor1);
					// (2007/2/22 - Seung-Won, Bae) Support Graph Color Changing with Graph Title
	BOOL			ChangeGraphColor( const char *p_szGraphTitle, const char *p_szColors);
	// Indicator Info
	CString	GetIndicatorName( const CBlockIndex& blockIndex, const int nVertScaleGroupIndex = 0, const int nGraphIndex = 0) const;
	bool	ChangeAllIndicatorInfoFromGraphData( void);
	BOOL	ChangeAllGraphDataFromIndicatorInfo( BOOL p_bWithCalculate);
	bool	ChangeIndicatorInfoFromGraphData( const CString& strGraphName);
	bool	ChangeAllMapDefaultIndicatorInfo( void);

// Calculate Indicator
public:
	void	CalculateAllGraphs( const CCalculateBaseData::CALCULATETYPE eCalType);
	bool	CalculateAllGraphs( const CList<CString, CString>& packetNameList, const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsHeadGraph);
	bool	CalculateGraph( const CString& strGraphName, const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsClearPacketData = false);
//{{ (2007/5/28 - LYH) For Weighted Volume Chart
	void	SetWeightedVolume( const int nGraphDrawStyle);
//}}

	// 한번에 그래프 속성 변경 가능한 메쏘드 추가 - ojtaso (20080602)
	void	ChangeGraphPropertyAtOnce(LPCTSTR lpszRQ, LPCTSTR lpszTargetTitle, LPCTSTR lpszNewTitle, short nGraphType, short nGraphStyle, short nDrawStyle, LPCTSTR lpszColors, short nThickness1, short nThickness2, short nThickness3);

	// 블럭의 VertScaleGroupCount를 얻음 - ojtaso (20080618)
	long	GetVertScaleGroupCount(const CBlockIndex& blockIndex);

// (2007/04/11 - Seung-Won, Bae) Change Bong Draw Style to Line in 1 Tick Chart.
// (2008/12/19 - Seung-Won, Bae) Calc it in VScale Draw for Including BaseLine.
protected:
	int								m_nBongGraph;
	CGraphImp *						m_pBongGraph[ 50];
	CGraphBaseData::BONGSTYLE		m_eBongStyle[ 50];
	CGraphBaseData::BONGDRAWSTYLE	m_eBongDrawStyle[ 50];

// (2009/6/25 - Seung-Won, Bae) for Balanced Row Arrange without scale.
public:
	void	GetSumOfHorzScaleHeightForEachBlock( CList< int, int> &p_lnSumOfHScaleHeight);

// (2009/10/3 - Seung-Won, Bae) Manage Selected RQ History.
protected:
	CStringList		m_slRQHistory;
public:
	CStringList &	GetRQHistory( void)		{	return m_slRQHistory;	}

	// KTB 체결잔고표시 - ojtaso (20091019)
	long	AddHorizontalLine(LPCSTR lpszRQ, LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice);
	void	DeleteHorizontalLine(LPCSTR lpszRQ, long nIndex);
	void	DeleteAllHorizontalLine(LPCSTR lpszRQ);
	void	UpdateHorizontalLine(LPCSTR lpszRQ, long nIndex, LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice);

// (2009/10/9 - Seung-Won, Bae) for Thread Draw.
public:
	int		m_nThreadCount;

	// 한번에 그래프 속성 얻어올 수 메쏘드 추가 - (@Solomon-20100120)
	bool GetGraphPropertyAtOnce(LPCTSTR lpszRQ, LPCTSTR lpszTargetTitle, short &nGraphType, short &nGraphStyle, short &nDrawStyle, COLORREF &crColors, short &nThickness1, short &nThickness2, short &nThickness3);

//20100518_솔로몬  해당 지표의 SubGraphIndex의 Hide 속성을 Get
	BOOL GetSubGraphHiding( const CString& strGraphName, const int nSubGraphIndex);


	//	시간구분선
	CScaleBaseData::SCALEGRIDTYPE				m_eLastDailyGridType;
	CScaleBaseData::SCALEGRIDTYPE				m_WeekGridType;
	CScaleBaseData::SCALEGRIDTYPE				m_MonGridType;
	CScaleBaseData::SCALEGRIDTYPE				m_YearGridType;

	virtual CScaleBaseData::SCALEGRIDTYPE GetFirstDailyScaleGridType( CScaleBaseData::SCALEGRIDTIMEGUBUN nType);
	virtual BOOL GetFirstDailyScaleTime( CScaleBaseData::SCALEGRIDTIMEGUBUN nType);
	virtual const unsigned short GetFirstBlockGridLineWidth_TimeGubun( int);

	virtual void SetAllDailyScaleGridType(const CScaleBaseData::SCALEGRIDTYPE gridType, const CScaleBaseData::SCALEGRIDTIMEGUBUN eTimeType, BOOL p_bCheckTime);
	virtual void SetAllScaleGridWidth_TimeGubun( CScaleBaseData::SCALEGRIDTIMEGUBUN nType, const unsigned short nGridWidth);

	virtual const unsigned long GetFirstBlockGridLineColor_TimeGubun( int);
	virtual void SetAllScaleGridColor_TimeGubun( CScaleBaseData::SCALEGRIDTIMEGUBUN nType, const unsigned long nGridColor);
};

#endif // !defined(AFX_MAINBLOCKIMP_H__50B9FD3A_84A6_4B65_98E0_B08B9A6CD78E__INCLUDED_)
