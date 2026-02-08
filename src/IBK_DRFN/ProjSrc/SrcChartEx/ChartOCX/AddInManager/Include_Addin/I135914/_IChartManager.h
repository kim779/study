// IChartManager.h: interface for the IChartManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICHARTMANAGER_H__E56B298C_2C27_477D_8D92_4510983D4417__INCLUDED_)
#define AFX_ICHARTMANAGER_H__E56B298C_2C27_477D_8D92_4510983D4417__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseEnum.h"				// for CScaleBaseData::VERTSCALEPOSITION
#include "_AddInVer.h"					// for AddIn Version Define
#include "_IString.h"					// for ILPCSTR

interface IBlock;
interface IStringList;
class CMainBlock;
interface IChartManager : public IUnknown  
{
// xScaleManager 관련 설정 : xScaleManager - ojtaso (20070525)
typedef enum {RMAT_RIGHT_MARGIN_FIX, RMAT_ITEM_WIDTH_ROUND} RMA_TYPE;	// 우측여백 처리 방법			- [우측 여백 고정기능, 봉(ITEM)의 폭 고정 기능]

protected:
	virtual ~IChartManager()	{}

// (2006/1/17 - Seung-Won, Bae) Get MainBlock Address for Special Usage
public:
	virtual CMainBlock *	GetObjectAddress( void)	= 0;

// (2006/6/25 - Seung-Won, Bae) Partition Info
public:
	// (2006/6/13 - Seung-Won, Bae) Query Block Type.
	virtual CBlockBaseData::BLOCKTYPE	GetBlockType( void) const = 0;
	// (2006/6/25 - Seung-Won, Bae) Block Counts
	virtual int							GetRowCount( void) = 0;
	virtual int							GetColumnCount( void) = 0;
	// (2006/6/13 - Seung-Won, Bae) Get Row Count
	virtual int							GetRowCountInColumn( int p_nColumnIndex) = 0;
	// Design Time String Info
	virtual IString *					GetBlocksPositionString( void)	= 0;
	// (2006/2/13 - Seung-Won, Bae) Check! Is Empty? (with BlockCount)
	virtual BOOL						IsEmpty( void) = 0;

// (2006/6/14 - Seung-Won, Bae) Get Block Interface
public:
	virtual	IBlock *	GetBlock( int p_nRowIndex, int p_nColumnIndex) = 0;

// (2006/6/25 - Seung-Won, Bae) View Info
public:
	// (2006/6/17 - Seung-Won, Bae) Get Background Color
	virtual COLORREF	GetBackgroundColor( void) = 0; 

// (2006/6/25 - Seung-Won, Bae) Scale Info
public:
	// (2006/6/13 - Seung-Won, Bae) Retrieve the Data Index Range in Scroll & Zoom View
	virtual BOOL	GetDataIndexRangeInView( int &p_nStartIndex, int &p_nEndIndex) = 0;
	// (2006/6/25 - Seung-Won, Bae) Change Scale Type
	virtual void	SetAllVertScalePosition(const CScaleBaseData::VERTSCALEPOSITION ePosition) = 0;
	// (2007/7/18 - Seung-Won, Bae) Support Up/Down Space
	virtual BOOL	GetVertScaleSpace( const int p_nR, const int p_nC, const int p_nV, int &p_nUpSpace, int &p_nDownSpace) = 0;
	virtual BOOL	SetVertScaleSpace( const int p_nR, const int p_nC, const int p_nV, const int p_nUpSpace, const int p_nDownSpace) = 0;

// (2006/6/25 - Seung-Won, Bae) Graph Info
public:
	// [04/10/26] 지정되는 Graph의 Data Packet Name을 조회하는 Interface를 제공한다.
	virtual IString *	GetPacketNameStringOfGraph( const char *p_szGraphName) = 0;
	// [04/10/27] 특정 Graph가 등록된 Block, ScaleGroup, Graph의 Index를 조회하는 Interface를 제공한다.
	virtual BOOL		GetIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG) = 0;
	// (2006/6/27 - Seung-Won, Bae) Retrieve the all Graph Names
	virtual IString *		GetAllIndicatorNameString( void)		= 0;
	virtual IStringList *	GetAllIndicatorNameStringList( void)	= 0;
	// (2007/1/20 - Seung-Won, Bae) Retrieve the all Graph Names
	virtual IStringList *	GetAllGraphNameStringList( void)		= 0;
	// (2007/1/20 - Seung-Won, Bae) Search Indicator
	virtual IString *		FindFirstIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, const char *p_szRQ) = 0;
	virtual IString *		FindNextIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, const char *p_szRQ) = 0;
	// (2007/3/28 - Seung-Won, Bae) Retrieve Packet Name of SubGraph
	virtual IString *		GetSubGraphPacketNameString( const char *p_szGraphName, const char *p_szSubGraphName, const int p_nPacketIndex) = 0;

// (2006/6/25 - Seung-Won, Bae) Mouse Position Info
public:
	// (2006/2/6 - Seung-Won, Bae) Get Full Column Region for DragZoom
	virtual RECT		GetGraphRegionSumInColumn( const POINT p_ptParam) const = 0;
	// (2006/2/6 - Seung-Won, Bae) Get Full Column Region for DragZoom
	virtual BOOL		GetBlockIndex( const POINT p_ptPoint, int &p_nRow, int &p_nColumn) = 0;

// (2006/7/27 - Seung-Won, Bae) Graph Calulation.
public:
	virtual BOOL		Initialize_GraphInfo( const bool p_bIsNULL) = 0;
	virtual BOOL		Initialize_AllPackets( const char *p_szGraphName, BOOL p_bWithCalculate) = 0;
	virtual BOOL		ChangeAllGraphInfoFromGraphData( void) = 0;
	virtual BOOL		ChangeAllGraphDataFromGraphInfo( BOOL p_bWithCalculate) = 0;
	virtual BOOL		ChangeGraphDataFromGraphInfo( const char *p_szGraphName, BOOL p_bWithCalculate) = 0;
	virtual BOOL		CalculateAllGraphs( const CCalculateBaseData::CALCULATETYPE eCalType = CCalculateBaseData::TRDATA_CAL) = 0;
	virtual BOOL		CalculateGraph( const char *p_szGraphName, const CCalculateBaseData::CALCULATETYPE eCalType, BOOL p_bWithClearPacketData) = 0;
 
// (2006/3/24 - Seung-Won, Bae) Sub Graph View Data Iterator
public:
	virtual POSITION	GetSubGraphViewData_First( void)																					= 0;
	virtual BOOL		GetSubGraphViewData_Next( POSITION &p_psnViewData, BOOL p_bAutoClose = TRUE)										= 0;
	virtual BOOL		GetSubGraphViewData_NextInColumn( POSITION &p_psnViewData, BOOL p_bAutoClose = TRUE)								= 0;
	virtual BOOL		GetSubGraphViewData_NextInRow( POSITION &p_psnViewData, BOOL p_bAutoClose = TRUE)									= 0;
	virtual BOOL		GetSubGraphViewData_Index( POSITION &p_psnViewData, int p_nRow, int p_nColumn)										= 0;
	virtual void		GetSubGraphViewData_Close( POSITION &p_psnViewData)																	= 0;
public:
	virtual	IString *	GetSubGraphViewData_HorzScaleNameString( POSITION p_psnViewData)															= 0;
	virtual RECT		GetSubGraphViewData_GraphRegion( POSITION p_psnViewData)																	= 0;
	virtual IString *	GetSubGraphViewData_TitleString( POSITION p_psnViewData)																	= 0;
	virtual void		GetSubGraphViewData_TitleAndPacketNameString( POSITION p_psnViewData, ILPCSTR &p_pszTitle, ILPCSTR &p_pszPacketNames)		= 0;

// xScaleManager 관련 설정 : xScaleManager - ojtaso (20070525)
public:
	virtual void		SetXScaleManager(LPVOID lpXScaleManager) = 0;

	// 봉폭고정 : 0, 봉개수 고정 : 1
	virtual void		SetCandleTypeOnResize(int nType, BOOL p_bNotUsed) = 0;					// 우측 여백 동작 방식 설정
	virtual int			GetCandleTypeOnResize() = 0;

	// 봉간격 일정 : xScaleManager - ojtaso (20070528)
	virtual void		SetFixedInterval(BOOL bFixedInterval, BOOL bCalc) = 0;
	virtual BOOL		GetFixedInterval() = 0;
};

#endif // !defined(AFX_ICHARTMANAGER_H__E56B298C_2C27_477D_8D92_4510983D4417__INCLUDED_)
