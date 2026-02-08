// VertScaleGroupList.h: interface for the CVertScaleGroupList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VERTSCALEGROUPLIST_H__96BBE01E_D550_4317_8F57_CC0C202C4F40__INCLUDED_)
#define AFX_VERTSCALEGROUPLIST_H__96BBE01E_D550_4317_8F57_CC0C202C4F40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "../Include_Chart/BaseData.h"

// BlockBaseEnum.h로 이동 : ojtaso (20070314)
// #define VSGL_TITLE_START_POSITION	( MARGIN * 3)

class CGraphImp;
class CBlockImp;
class CSubGraph;
class CBlockData;
class CRowViewData;
class CBlocksColumn;
class CVertScaleGroup;
class CGraphSearchInfo;
class CMainBlockBaseData;
class CMainChartFormulate;
class CSelectedSubGraphDeque;
class CVerticalScalePtrDataList;
class CVerticalScaleGroupDataList;

class CVertScaleGroupList  
{
public:
	CVertScaleGroupList( CMainBlockBaseData* pMainBlockBaseData);
	virtual ~CVertScaleGroupList();

protected:
	CMainBlockBaseData *						m_pMainBlockBaseData;
	CBlockImp *									m_pBlock;						// 상위 class 주소	
	CTypedPtrList< CObList, CVertScaleGroup *>	m_realVertScaleGroupList;		// CVertScaleGroup들을 list로..
	CDrawingRegion								m_drawingRegion;				// graph/scale 그리는 영역.

// Base Info
public:
	// Basic Info
	void		SetBlock( CBlockImp* pBlock);
	CBlockImp *	GetBlock( void) const;
	// Regions
	CDrawingRegion	GetDrawingRegion( void) const;
	CGraphRegions	GetGraphRegions( void) const;
	CRect			GetSubGraphTitleRegion( CDC* pDC, const CSubGraph* pSubGraph) const;

// Create
public:
	void	BuildVertScaleGroups( char *p_szVertScaleGroupData, const bool bInitialize = true);
	// RQ로 검색 : 복수종목 - ojtaso (20070118)
	void	BuildVertScaleGroups(LPVOID pPacketRQ, char *p_szVertScaleGroupData, const bool bInitialize = true);
	void	RebuildVertScaleGroups( char *p_szVertScaleGroupData, const bool doesMaintainOldScale);
	void	GetVertScaleGroupsAllData( CStringList &p_slVertScaleGroupAllData) const;
	bool	AddTailInNoEqual( CVertScaleGroup* pVertScaleGroup);
	bool	MakeVertScaleGroups_Empty( const int nVertScaleGroupCount = 1);
	bool	InsertGraphInVertScaleGroup( const int nVertScaleGroupIndex, const CString& graphData, const bool doesInsertInNewVertScaleGroup);
	// RQ를 가지고 있는 Graph 생성 : 복수종목 - ojtaso (20061224)
	// CPacketRQ*로 변경 : 복수종목 - ojtaso (20070109)
	bool InsertGraphInVertScaleGroup(LPVOID pPacketRQ, const int nVertScaleGroupIndex, const CString& graphData, const bool doesInsertInNewVertScaleGroup);
	void	AddTail( CVertScaleGroup* pVertScaleGroup);

// Delete
public:
	void	DeleteAll( void);
	void	RemoveAll( const bool bIsDeleteBlock);
	BOOL	RemoveAt( CVertScaleGroup* pVertScaleGroup, const bool bIsDeleteBlock);
	bool	DeleteVertScaleGroup( CVertScaleGroup* pVertScaleGroup, const bool bIsDeleteBlock);
	// Graph
	BOOL	DeleteAllIndicator( const char *p_szIndicatorName);

// Search
public:
	// Vertical Scale Group
	CVertScaleGroup *	GetVertScaleGroup( const int nIndex) const;
	// Graph
	CGraphImp *	GetGraph( const int p_nV, const int p_nG) const;
	BOOL		FindGraph( CGraphSearchInfo *p_pGraphSearchInfo) const;
	// RQ로 검색 : 복수종목 - ojtaso (20070118)
	BOOL		FindGraph(LPVOID pPacketRQ, CGraphSearchInfo *p_pGraphSearchInfo) const;
	// Sub Graph
	CSubGraph *	GetSubGraph( const int p_nV, const int p_nG, const int p_nSG) const;
	CSubGraph *	FindSubGraphTitle( CDC* pDC, const CPoint& point, CRect& titleRegion) const;
	bool		FindSubGraph( const CPoint& point, CSelectedSubGraphDeque& selectedSubGraphDeque) const;

// Move
public:
	// Vertical Scale Group
	bool	MoveAllVertScaleGroups( CVertScaleGroupList* pSourceGroupList);
	bool	ReArrangeAllVertScaleGroups( const bool bDoJoin = TRUE);
	// Graph
	bool	MoveAllGraphs( CVertScaleGroup* pSourceGroup, const int nTargetGroupIndex, const bool bIsDeleteBlock);
	bool	MoveGraph( CGraphImp* pSourceGraph, const int nTargetGroupIndex, const bool bIsDeleteBlock, const bool bIsAddTail = TRUE);

// Query & Update
public:
	// All Data
	// 해당 RQ만 작업 : 복수종목 - ojtaso (20070420)
	void	GetRowViewData(LPCTSTR lpszRQ, CRowViewData& rowViewData) const;
	// Vertical Scale Group
	int		GetCount( void) const;
	CString	GetVertScaleGroupDataList_GetVertScaleTitle( CVertScaleGroup* pVertScaleGroup, const int nIndex) const;
	bool	GetVertScaleGroupDataList( CVerticalScaleGroupDataList& dataList) const;
	void	GetVertScalePtrDataList( CVerticalScalePtrDataList& dataList) const;
	void	SetScaleGridType( const CScaleBaseData::SCALEGRIDTYPE gridType);
	void	SetAllVertScalePosition( const CScaleBaseData::VERTSCALEPOSITION ePosition);
	void	SetAllVScaleMinMaxType( CScaleBaseData::VERTMINMAXTYPE p_eVScaleMinMaxType);
	void	ReleaseAllVScaleSettingInFixedMinMax( void);
	void	CalculateDisplayDataMinMaxAllVertScaleGroup( void);
	void	SetActivate( CVertScaleGroup *p_pVertScaleGroup);
	void	SetActivate( CVertScaleGroup *p_pVertScaleGroup, int p_nActiveFlag);
	void	SetAllVertScaleInvertType( BOOL p_bInvert);
	int		GetActiveVScaleIndex( CScaleBaseData::VERTSCALEPOSITION p_eVScalePosition);
	// Graph
	bool	GetAllIndicatorName( CList<CString, CString>& strIndicatorNameList) const;
	bool	GetAllGraphName( CList<CString, CString>& strGraphNameList) const;
	void	SetAllGraphsHiding( const bool bIsHiding);
	// SubGraph
	bool	SetSubGraphHiding( const CString& subGraphTitle, const bool bIsHiding);
			// (2007/2/20 - Seung-Won, Bae) Support Indicator Hiding
			//		It can not be replaced with AddChartBlock(), because of Condition Reset.
	void	ShowIndicator( const char *p_szIndicatorName, BOOL p_bShow, BOOL p_bRecalculate);
	// Packet
	bool	Initialize_Packet( const CString& strPacketName, const bool bIsCalculate);
	BOOL	ChangeIndicatorPacketNames( const char *p_szIndicatorName, const int p_nSubGraphIndex, const char *p_szPacketNames);
	// Indicator Info
	void	Initialize_IndicatorInfo( const bool bIsNULL);
	bool	ChangeAllIndicatorInfoFromGraphData( void);
	BOOL	ChangeAllGraphDataFromIndicatorInfo( BOOL p_bWithCalculate);
	bool	ChangeAllMapDefaultIndicatorInfo( CIndicatorList* pIndicatorList);

// Calculate Indicator
public:
	void	CalculateAllGraphs( const CCalculateBaseData::CALCULATETYPE eCalType);
	bool	CalculateAllGraphs( const CList<CString, CString>& packetNameList, const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsHeadGraph);

// Draw
public:
	void	GetVertScaleRegionWidth( CDC *p_pDC, int &p_nLeftWidth, int &p_nRightWidth);
	void	CalculateDrawingRegion( CDC* pDC, const CDrawingRegion& drawingRegion);
	void	DrawVertScale( CDC* pDC, const CBlockData& blockData);
	void	DrawGraphs( CDC* pDC, const CBlockData& blockData);
	// 타이틀 여러줄로 표시 : 복수종목 - ojtaso (20070227)
	void	RecalculateTitleRegion(BOOL bIncreaseLine);

// (2009/10/9 - Seung-Won, Bae) for Thread Draw.
protected:
	CDrawingGraphData *		m_pDrawingGraphData;
};

#endif // !defined(AFX_VERTSCALEGROUPLIST_H__96BBE01E_D550_4317_8F57_CC0C202C4F40__INCLUDED_)
