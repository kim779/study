// IChartManager.h: interface for the IChartManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICHARTMANAGER_H__E56B298C_2C27_477D_8D92_4510983D4417__INCLUDED_)
#define AFX_ICHARTMANAGER_H__E56B298C_2C27_477D_8D92_4510983D4417__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Include_Chart/BlockBaseEnum.h"				// for CScaleBaseData::VERTSCALEPOSITION
#include "_AddInVer.h"					// for AddIn Version Define
#include "_IString.h"					// for ILPCSTR

interface IBlock;
interface IStringList;
class CMainBlock;
interface IChartManager : public IUnknown  
{
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

// (2006/6/25 - Seung-Won, Bae) Graph Info
public:
	// [04/10/26] 지정되는 Graph의 Data Packet Name을 조회하는 Interface를 제공한다.
	virtual IString *	GetPacketNameStringOfGraph( const char *p_szGraphName) = 0;
	// [04/10/27] 특정 Graph가 등록된 Block, ScaleGroup, Graph의 Index를 조회하는 Interface를 제공한다.
	virtual BOOL		GetIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG) = 0;
	// RQ별 찾기 : 복수종목 - ojtaso (20070306)
	virtual BOOL		GetIndexOfGraph( const char *p_szRQ, const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG) = 0;
	// (2006/6/27 - Seung-Won, Bae) Retrieve the all Graph Names
	virtual IString *	GetAllIndicatorNameString( void) = 0;
	// (2006/8/15 - Seung-Won, Bae) Retrieve the all Graph Names with List
	virtual IStringList *	GetAllIndicatorNameStringList( void) = 0;

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

public:
	virtual void		SetXScaleManager(LPVOID lpXScaleManager) = 0;
	virtual void		RealDataReceived( const char *p_szRQ, BOOL bAppend, BOOL bCalc = TRUE) = 0;
	// XScale : 관련 Interface로 사용 - vntsorl(20070412)
	virtual BOOL		GetStartEndDrawIdx_Cur( const char *p_szRQ, int &p_nStartIndex, int &p_nEndIndex) = 0;	// 현재 화면에 보여지는 영역의 시작과 끝 위치를 얻어옵니다.
	// XScale : 관련 Interface로 사용 - vntsorl(20070412)
	virtual int			GetXPosition(const char *p_szRQ, int nIdx, int nCol) = 0;
	virtual int			GetXPosition( const char *p_szRQ, int nIdx, int nCol, int& nLeft, int& nRight) = 0;
	virtual void		GetGraphXAreaSE(int& nStart, int& nEnd, BOOL bIncludeMargin = FALSE) = 0;
	virtual int			GetGraphXArea_Start(BOOL bIncludeMargin = FALSE) = 0;
	virtual int			GetGraphXArea_End(BOOL bIncludeMargin = FALSE) = 0;

	virtual void		GetGraphXAreaLR(int& nLeft, int& nRight, BOOL bIncludeMargin = FALSE) = 0;
	virtual int			GetGraphXArea_Left(BOOL bIncludeMargin = FALSE) = 0;
	virtual int			GetGraphXArea_Right(BOOL bIncludeMargin = FALSE) = 0;

	// XScale : 관련 Interface로 사용 - vntsorl(20070412)
	virtual int			FindViewDataIndex_Pt(const char *p_szRQ, int nXPoint) = 0;
	virtual time_t		GetTimeFromX(int nCursor_x, int* pnBlockColumn, int& nCenter_x) = 0;
	virtual int			GetXFromTime(time_t timeFind, int& nCenter_x) = 0;

	// XScale : 관련 Interface로 사용 - vntsorl(20070412)
	virtual double		GetDoubleFromX(const int nRow, const int nCol, int nCursor_x, int* pnBlockColumn, int& nCenter_x) = 0;
	virtual int			GetXFromDouble(const int nRow, const int nCol, double dblFind, int& nCenter_x) = 0;
};

#endif // !defined(AFX_ICHARTMANAGER_H__E56B298C_2C27_477D_8D92_4510983D4417__INCLUDED_)
