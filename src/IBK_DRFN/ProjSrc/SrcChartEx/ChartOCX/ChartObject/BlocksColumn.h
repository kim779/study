// BlocksColumn.h: interface for the CBlocksColumn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCKSCOLUMN_H__E9DD8A9E_2E33_4E9B_9A74_125FE05CA57C__INCLUDED_)
#define AFX_BLOCKSCOLUMN_H__E9DD8A9E_2E33_4E9B_9A74_125FE05CA57C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "../Include_Chart/ScaleBaseData.h"

class CBlockImp;
class CSubGraph;
class CBlockData;
class CGraphColor;
class CPenThickness;
class CGraphRegions;
class CSubGraphData;
class CIndicatorList;
class CIndicatorInfo;
class CVertScaleGroup;
class CColumnViewData;
class CBlocksPosition;
class CGraphSearchInfo;
class CGraphRegionColor;
class CMainBlockBaseData;
class CMainChartFormulate;
class CSelectedSubGraphDeque;
class CMainBlockImp;
class CHorizontalScalePtrDataList;
class CInsertingBlockHorzScaleData;
class CBlocksColumn : public CObject
{
public:
	CBlocksColumn( CMainBlockImp* pMain);
	virtual ~CBlocksColumn();

private:
	CMainBlockImp *				m_pMain;
	// blocksColumn 의 세로 scale의 폭 저장
	CVertScaleRegionWidth					m_vertScaleRegionWidth;
	// 같은 column에 있는 block들을 저장
	CTypedPtrList < CObList, CBlockImp *>	m_BlockList;

// Base Setting
protected:
	// Main Block
	CMainBlockBaseData *	GetMainBlockBaseData( void) const;
public:
	// Block
	void	InitializeNormalSizeBlockNumber( void);
	// Vertical Scale
	void	SetAllVScaleMinMaxType( CScaleBaseData::VERTMINMAXTYPE p_eVScaleMinMaxType);
	void	ReleaseAllVScaleSettingInFixedMinMax( void);

// Create
public:
	// Block
	static CBlocksColumn *	Make( CMainBlockImp * pMain);
	void					MakeBlocksColumn( const int nRowCount);
	// 그래프 생성후 계산 여부 : xScaleManager - ojtaso (20070701)
	void					BuildGraphsAndScales( char *p_szBlockColumnData, BOOL& bCalc);
	// RQ로 검색 : 복수종목 - ojtaso (20070118)
	void					BuildGraphsAndScales(LPVOID pPacketRQ, char *p_szBlockColumnData);
	void					GetBlocksColumnAllData( CStringList &p_slBlockColumnData) const;
	// Graph
	bool					InsertBlock_LoadDialog(CInsertingBlockHorzScaleData& insertingBlockHorzScaleData);
	CBlockImp *				InsertBlock_GetBlockForInsert(const int nInsertPosition, const bool bIsAutoEmptyBlock = true);
	POSITION				InsertBlock_WithPosition( CBlockImp* pInsertBlock, const int nInsertPosition, const bool bIsAutoOneBlockCount);
	bool					InsertBlock( const int nInsertPosition, char *p_szBlockData);
	// RQ로 검색 : 복수종목 - ojtaso (20070118)
	bool					InsertBlock(LPVOID pPacketRQ, const int nInsertPosition, char *p_szBlockData);
	// RQ로 검색 : 복수종목 - ojtaso (20070118)
	bool					InsertBlocks(LPVOID pPacketRQ, const int nInsertPosition, const CString& strGraphData);
	bool					InsertBlocks( const int nInsertPosition, const CString& strGraphData);
	bool					InsertBlocks( const CString& graphData);
	// RQ로 검색 : 복수종목 - ojtaso (20070118)
	bool					InsertBlocks(LPVOID pPacketRQ, const CString& strGraphData);
	bool					InsertBlock( const int nInsertPosition, const bool bDoesMaintainOldScale);

// Delete
public:
	// Block
	static void	Delete(CBlocksColumn* pBlocksColumn);
	void		DeleteAll( void);
	bool		DeleteBlock( CBlockImp* pBlock, const bool bAutoHorzScaleDelete);
	// Graph
	BOOL	DeleteAllIndicator( const char *p_szIndicatorName);

// Search
public:
	// Block
	CBlockImp *	GetBlock( int p_nRowIndex) const;
	// Vertical Scale Group
	CVertScaleGroup *	GetVertScaleGroup( const int nBlockRowIndex, const int nVertScaleGroupIndex) const;
	// Graph
	BOOL	FindGraph( CGraphSearchInfo *p_pGraphSearchInfo) const;
	// RQ로 검색 : 복수종목 - ojtaso (20070118)
	BOOL	FindGraph(LPVOID pPacketRQ,  CGraphSearchInfo *p_pGraphSearchInfo) const;

// Move
public:
	// Block
	bool	MoveBlock( CBlocksPosition* pBlocksPosition, const int nSourceBlockIndex, const int nTargetBlockIndex);
	bool	MoveAllVertScaleGroupsInPartition( const int nBlockRowIndex, const bool bDoesMaintainOldScale);

// Query & Update
public:
	// Draw
	void	DrawAllBlocksBackground( CDC* pDC, const int nBlockColumnIndex, const CBlocksPosition& blocksPosition);
	void	DrawAllHorzAndVertScales( CDC* pDC, const int nBlockColumnIndex, const CBlocksPosition& blocksPosition);
	void	DrawAllGraphs( CDC* pDC, const int nBlockColumnIndex, const CBlocksPosition& blocksPosition);
	void	CalculateDisplayDataMinMaxAllVertScaleGroup( void);
	void	CalculateDrawingRegion( CDC* pDC, const int nBlockColumnIndex, const CBlocksPosition& blocksPosition);

	// All
	// 해당 RQ만 작업 : 복수종목 - ojtaso (20070420)
	void							GetColumnViewData(LPCTSTR lpszRQ, CColumnViewData& columnViewData) const;
	CScaleBaseData::SCALEGRIDTYPE	GetFirstBlockVGridLineType( void);
	// Block
	int		GetBlockCount( void) const;
	void	SetAllScaleGridType( const CScaleBaseData::SCALEGRIDTYPE eGridType);
	void	SetAllHorzScaleGridType( const CScaleBaseData::SCALEGRIDTYPE eGridType);
	void	SetAllHorzScalePosition( const CScaleBaseData::HORZSCALEPOSITION ePosition);
	void	GetHorzScalePtrDataList( const CBlockImp* pCurrentBlock, CHorizontalScalePtrDataList& horzScalePtrDataList) const;
	void	SetAllVertScaleGridType( const CScaleBaseData::SCALEGRIDTYPE eGridType);
	void	SetAllVertScalePosition( const CScaleBaseData::VERTSCALEPOSITION ePosition);
	bool	SetAllVertScaleLogType( const bool bIsLogType);
	void	SetMaintainOldScale( CBlockImp* pBlock);
	// Graph
	bool	GetAllIndicatorName( CList<CString, CString>& strIndicatorNameList) const;
	bool	GetAllGraphName( CList<CString, CString>& strGraphNameList) const;
	bool	Initialize_Packet( const CString& strPacketName, const bool bIsCalculate);
	void	Initialize_IndicatorInfo( const bool bIsNULL);
	BOOL	ChangeIndicatorPacketNames( const char *p_szIndicatorName, const int p_nSubGraphIndex, const char *p_szPacketNames);
	// (2007/2/20 - Seung-Won, Bae) Support Indicator Hiding
	//		It can not be replaced with AddChartBlock(), because of Condition Reset.
	void	ShowIndicator( const char *p_szIndicatorName, BOOL p_bShow, BOOL p_bRecalculate);
	// Indicator Info
	bool	ChangeAllIndicatorInfoFromGraphData( void);
	BOOL	ChangeAllGraphDataFromIndicatorInfo( BOOL p_bWithCalculate);
	bool	ChangeAllMapDefaultIndicatorInfo( CIndicatorList* pIndicatorList);

// Notify
public:
	void	SendMessageToParent_BlockInsert( const int nSourceBlockIndex);

// Calculate Indicator
public:
	void	CalculateAllGraphs( const CCalculateBaseData::CALCULATETYPE eCalType);
	bool	CalculateAllGraphs( const CList<CString, CString>& packetNameList, const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsHeadGraph);

// (2008/1/22 - Seung-Won, Bae) for Multi-Language
protected:
	HWND				m_hOcxWnd;

// (2009/2/13 - Seung-Won, Bae) for Multi-Column.
public:
	int		GetColumnIndex( void);

// (2009/6/25 - Seung-Won, Bae) for Balanced Row Arrange without scale.
public:
	void	GetSumOfHorzScaleHeightForEachBlock( CList< int, int> &p_lnSumOfHScaleHeight);

// (2009/10/2 - Seung-Won, Bae) Get Horizontal Scale RQ for X Scale of each block with different time scale.
public:
	CString		GetHorzScaleDataRQ( CBlockImp *pBlock);
};

#endif // !defined(AFX_BLOCKSCOLUMN_H__E9DD8A9E_2E33_4E9B_9A74_125FE05CA57C__INCLUDED_)
