// IChartManager.h: interface for the IChartManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICHARTMANAGER_H__E56B298C_2C27_477D_8D92_4510983D4417__INCLUDED_)
#define AFX_ICHARTMANAGER_H__E56B298C_2C27_477D_8D92_4510983D4417__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "_AddInVer.h"					// for AddIn Version Define
#include "BlockBaseEnum.h"				// for CScaleBaseData

interface IBlock;
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
	virtual BOOL						GetBlocksPosition( CString &p_strBlockPosition)	= 0;
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
	virtual BOOL	GetPacketNameOfGraph( const char *p_szGraphName, CString &p_strPacketNames) = 0;
	// [04/10/27] 특정 Graph가 등록된 Block, ScaleGroup, Graph의 Index를 조회하는 Interface를 제공한다.
	virtual BOOL	GetIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG) = 0;
	// (2006/6/27 - Seung-Won, Bae) Retrieve the all Graph Names
	virtual BOOL	GetAllGraphName( CString &p_strGraphNames) = 0;

// (2006/6/25 - Seung-Won, Bae) Mouse Position Info
public:
	// (2006/2/6 - Seung-Won, Bae) Get Full Column Region for DragZoom
	virtual CRect		GetGraphRegionSumInColumn( const CPoint &p_ptParam) const = 0;
	// (2006/2/6 - Seung-Won, Bae) Get Full Column Region for DragZoom
	virtual BOOL		GetBlockIndex( CPoint p_ptPoint, int &p_nRow, int &p_nColumn) = 0;

// (2006/3/24 - Seung-Won, Bae) Sub Graph View Data Iterator
public:
	virtual POSITION	GetSubGraphViewData_First( void)																					= 0;
	virtual void		GetSubGraphViewData_Next( POSITION &p_psnViewData)																	= 0;
	virtual void		GetSubGraphViewData_NextInColumn( POSITION &p_psnViewData)															= 0;
	virtual void		GetSubGraphViewData_NextInRow( POSITION &p_psnViewData)																= 0;
	virtual BOOL		GetSubGraphViewData_Index( POSITION &p_psnViewData, int p_nRow, int p_nColumn)										= 0;
	virtual void		GetSubGraphViewData_Close( POSITION &p_psnViewData)																	= 0;
public:
	virtual	void		GetSubGraphViewData_HorzScaleName( POSITION p_psnViewData, CString &p_strHorzScaleName)								= 0;
	virtual void		GetSubGraphViewData_GraphRegion( POSITION p_psnViewData, CRect &p_rctGraphRegion)									= 0;
	virtual void		GetSubGraphViewData_Title( POSITION p_psnViewData, CString &p_strTitle)												= 0;
	virtual void		GetSubGraphViewData_TitleAndPacketNames( POSITION p_psnViewData, CString &p_strTitle, CString &p_strPacketNames)	= 0;
};

#endif // !defined(AFX_ICHARTMANAGER_H__E56B298C_2C27_477D_8D92_4510983D4417__INCLUDED_)
