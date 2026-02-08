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
interface IChartOCX;
interface IStringList;
class CMainBlock;
interface IChartManager : public IUnknown  
{
protected:
	virtual ~IChartManager()	{}

// (2008/8/17 - Seung-Won, Bae) Support IChartOCX from Interface of other manager.
public:
	virtual IChartOCX *	GetIChartOCX( void) = 0;

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
	// (2009/5/28 - Seung-Won, Bae) Check! Is Data Empty?
	virtual BOOL						IsEmptyData( void) = 0;
	// (2009/6/5 - Seung-Won, Bae) Reset partition info.
	virtual void						SetBlockCount( int p_nRowCount, int p_nColumnCount) = 0;

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
	// SetAllVertScaleGridType
	virtual	BOOL		SetAllVertScaleGridType( int p_nGridVLineType)	= 0;
	// (2007/7/18 - Seung-Won, Bae) Support Up/Down Space
	virtual BOOL	GetVertScaleSpace( const int p_nR, const int p_nC, const int p_nV, int &p_nUpSpace, int &p_nDownSpace) = 0;
	virtual BOOL	SetVertScaleSpace( const int p_nR, const int p_nC, const int p_nV, const int p_nUpSpace, const int p_nDownSpace) = 0;
	virtual CScaleBaseData::HORZSCALEDRAWERTYPE	GetMasterValueType( const int nRow, const int nCol) = 0;
	virtual void	GetHorzScaleCompart( const int p_nR, const int p_nC, ILPCSTR &p_szDateCompart, ILPCSTR &p_szTimeCompart) = 0;

// (2006/6/25 - Seung-Won, Bae) Graph Info
public:
	// [04/10/26] 지정되는 Graph의 Data Packet Name을 조회하는 Interface를 제공한다.
	virtual IString *		GetPacketNameStringOfGraph( const char *p_szGraphName) = 0;
	// [04/10/27] 특정 Graph가 등록된 Block, ScaleGroup, Graph의 Index를 조회하는 Interface를 제공한다.
	virtual BOOL			GetIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG) = 0;
	// (2006/6/27 - Seung-Won, Bae) Retrieve the all Graph Names
	virtual IString *		GetAllIndicatorNameString( void)		= 0;
	virtual IStringList *	GetAllIndicatorNameStringList( void)	= 0;
	// (2007/1/20 - Seung-Won, Bae) Retrieve the all Graph Names
	virtual IStringList *	GetAllGraphNameStringList( void)	= 0;
	// (2007/1/20 - Seung-Won, Bae) Search Indicator
	virtual IString *		FindFirstIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, const char *p_szRQ) = 0;
	virtual IString *		FindNextIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, const char *p_szRQ) = 0;
	// (2007/2/9 - Seung-Won, Bae) Search Graph
	virtual IString *		FindGraph( POINT p_pt) = 0;
	// (2007/3/28 - Seung-Won, Bae) Retrieve Packet Name of SubGraph
	virtual IString *		GetSubGraphPacketNameString( const char *p_szGraphName, const char *p_szSubGraphName, const int p_nPacketIndex) = 0;
	// (2007/4/30 - Seung-Won, Bae) Get Data Index of Inverse Line Graph
	virtual int				GetDataIndexOfInverseLine( POINT p_pt) = 0;
	// (2009/9/27 - Seung-Won, Bae) Get RQ of graph.
	virtual IString *		GetRQofGraph( const char *p_szGraphName) = 0;

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
	virtual POSITION	GetSubGraphViewData_First( const char *p_szRQ)																					= 0;
	virtual BOOL		GetSubGraphViewData_Next( POSITION &p_psnViewData, BOOL p_bAutoClose = TRUE)										= 0;
	virtual BOOL		GetSubGraphViewData_NextInColumn( POSITION &p_psnViewData, BOOL p_bAutoClose = TRUE)								= 0;
	virtual BOOL		GetSubGraphViewData_NextInRow( POSITION &p_psnViewData, BOOL p_bAutoClose = TRUE)									= 0;
	virtual BOOL		GetSubGraphViewData_Index( POSITION &p_psnViewData, int p_nRow, int p_nColumn)										= 0;
	virtual void		GetSubGraphViewData_Close( POSITION &p_psnViewData)																	= 0;
public:
	virtual	IString *	GetSubGraphViewData_HorzScaleNameString( POSITION p_psnViewData)															= 0;
	virtual RECT		GetSubGraphViewData_GraphRegion( POSITION p_psnViewData)																	= 0;
	virtual IString *	GetSubGraphViewData_TitleString( POSITION p_psnViewData)																	= 0;
	virtual IString *	GetSubGraphViewData_PacketNameString( POSITION p_psnViewData)																= 0;
	virtual IString *	GetSubGraphViewData_RQString( POSITION p_psnViewData)																		= 0;
	virtual void		GetSubGraphViewData_TitleAndPacketNameAndRQString( POSITION p_psnViewData, ILPCSTR &p_pszTitle, ILPCSTR &p_pszPacketNames, ILPCSTR &p_pszRQ)	= 0;

// [07/03/26] XScale의 정보를 구하는 Interface를 제공한다.
public:
	virtual void		RealDataReceived( const char *p_szRQ, BOOL p_bIndexedReal = FALSE) = 0;
	virtual BOOL		GetStartEndDrawIdx_Cur( const char *p_szRQ, int &p_nStartIndex, int &p_nEndIndex) = 0;	// 현재 화면에 보여지는 영역의 시작과 끝 위치를 얻어옵니다.
	virtual int			GetXPosition(const char *p_szRQ, int nIdx, int nCol) = 0;
	virtual int			GetXPosition( const char *p_szRQ, int nIdx, int nCol, int& nLeft, int& nRight) = 0;
	virtual void		GetGraphXAreaSE(int& nStart, int& nEnd, BOOL bIncludeMargin = FALSE) = 0;
	virtual int			GetGraphXArea_Start(BOOL bIncludeMargin = FALSE) = 0;
	virtual int			GetGraphXArea_End(BOOL bIncludeMargin = FALSE) = 0;
	virtual void		GetGraphXAreaLR(int& nLeft, int& nRight, BOOL bIncludeMargin = FALSE) = 0;
	virtual int			GetGraphXArea_Left(BOOL bIncludeMargin = FALSE) = 0;
	virtual int			GetGraphXArea_Right(BOOL bIncludeMargin = FALSE) = 0;
	virtual int			FindViewDataIndex_Pt(const char *p_szRQ, int nXPoint, int nCol=0, BOOL p_bWithBoundCheck = FALSE) = 0;
	virtual time_t		GetTimeFromX(int nCursor_x, int* pnBlockColumn, int& nCenter_x) = 0;
	virtual int			GetXFromTime(time_t timeFind, int& nCenter_x) = 0;
	// VertScaleGroup의 가장 처음 Graph의 타입 : 복수종목 - ojtaso (20070504)
	virtual double		GetDoubleFromX(const int nRow, const int nCol, int nCursor_x, int* pnBlockColumn, int& nCenter_x, const char *p_szRQ = NULL) = 0;
	virtual int			GetXFromDouble(const int nRow, const int nCol, double dblFind, int& nCenter_x) = 0;

// xScaleManager 관련 설정 : xScaleManager - ojtaso (20070525)
public:
	virtual void		SetXScaleManager(LPVOID lpXScaleManager) = 0;

	// 봉폭고정 : 0, 봉개수 고정 : 1
	virtual void		SetCandleTypeOnResize( int nType) = 0;					// 우측 여백 동작 방식 설정
	virtual int			GetCandleTypeOnResize() = 0;

	// 봉간격 일정 : xScaleManager - ojtaso (20070528)
	virtual void		SetFixedInterval(BOOL bFixedInterval, BOOL bCalc) = 0;
	virtual BOOL		IsFixedInterval() = 0;

// (2009/5/10 - Seung-Won, Bae) Scroll with added and trucated by Real
public:
	virtual void	SetFlagForCalcDRIandScrollByReal( BOOL p_bWithTrucated = FALSE) = 0;

// (2008/10/20 - Seung-Won, Bae) Retreive RQ
public:
	virtual IString *	GetMainRQofBlock( int p_nRow, int p_nCol) = 0;
};

#endif // !defined(AFX_ICHARTMANAGER_H__E56B298C_2C27_477D_8D92_4510983D4417__INCLUDED_)
