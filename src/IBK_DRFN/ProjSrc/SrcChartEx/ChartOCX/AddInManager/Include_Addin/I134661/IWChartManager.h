// IWChartManager.h: interface for the CIWChartManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWCHARTMANAGER_H__7FD49D7A_F392_4267_982D_CBFE6535E786__INCLUDED_)
#define AFX_IWCHARTMANAGER_H__7FD49D7A_F392_4267_982D_CBFE6535E786__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>				// for CTypedPtrList

#include "../_IWUnknown.h"
#include "_IChartManager.h"
#include "MainBlock.h"			// for CMainBlock

#include "ViewData.h"				// for CViewData

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2004.10.11
// Comments		: MainBlock의 Interface Wrapper Class를 정의한다.
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
class CIWChartManager : public CIWUnknown, public IChartManager
{
// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER_REFERENCE( m_pMainBlock)
	_IWRAPPER_IUNKNOWN_MANAGER_MGR( IChartManager, CIWChartManager)
	_IWRAPPER_IUNKNOWN_MANAGER_CREATE( IChartManager, CIWChartManager, CMainBlock, m_pMainBlock)
protected:
	static long	m_nObjectCount;
protected:
	CIWChartManager()
	{
		m_bAutoDelete = FALSE;
		_IWRAPPER_IUNKNOWN_CREATE_TRACE( CIWChartManager)
	}
	~CIWChartManager()
	{
		// (2006/3/24 - Seung-Won, Bae) Sub Graph View Data Iterator
		//		Define Iterator Information Object of SubGraphViewData
		while( !m_lSubGraphViewData.IsEmpty()) delete m_lSubGraphViewData.RemoveTail();
		
		_IWRAPPER_IUNKNOWN_DESTROY_TRACE( CIWChartManager)
	}


// (2006/1/17 - Seung-Won, Bae) Get MainBlock Address for Special Usage
protected:
	virtual CMainBlock *	GetObjectAddress( void)	{	return m_pMainBlock;	}

// (2006/6/25 - Seung-Won, Bae) Partition Info
protected:
	// (2006/6/13 - Seung-Won, Bae) Query Block Type.
	virtual CBlockBaseData::BLOCKTYPE	GetBlockType( void) const;
	// (2006/6/25 - Seung-Won, Bae) Block Counts
	virtual int							GetRowCount( void);
	virtual int							GetColumnCount( void);
	// (2006/6/13 - Seung-Won, Bae) Get Row Count
	virtual int							GetRowCountInColumn( int p_nColumnIndex);
	// Design Time String Info
	virtual BOOL						GetBlocksPosition( CString &p_strBlockPosition);
	// (2006/2/13 - Seung-Won, Bae) Check! Is Empty? (with BlockCount)
	virtual BOOL						IsEmpty( void);

// (2006/6/14 - Seung-Won, Bae) Get Block Interface
protected:
	virtual	IBlock *	GetBlock( int p_nRowIndex, int p_nColumnIndex);

// (2006/6/25 - Seung-Won, Bae) View Info
protected:
	// (2006/6/17 - Seung-Won, Bae) Get Background Color
	virtual COLORREF	GetBackgroundColor( void); 

// (2006/6/25 - Seung-Won, Bae) Scale Info
protected:
	// (2006/6/13 - Seung-Won, Bae) Retrieve the Data Index Range in Scroll & Zoom View
	virtual BOOL	GetDataIndexRangeInView( int &p_nStartIndex, int &p_nEndIndex);
	// (2006/6/25 - Seung-Won, Bae) Change Scale Type
	virtual void	SetAllVertScalePosition(const CScaleBaseData::VERTSCALEPOSITION ePosition);

// (2006/6/25 - Seung-Won, Bae) Graph Info
protected:
	// [04/10/26] 지정되는 Graph의 Data Packet Name을 조회하는 Interface를 제공한다.
	virtual BOOL	GetPacketNameOfGraph( const char *p_szGraphName, CString &p_strPacketNames);
	// [04/10/27] 특정 Graph가 등록된 Block, ScaleGroup, Graph의 Index를 조회하는 Interface를 제공한다.
	virtual BOOL	GetIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG);
	// (2006/6/27 - Seung-Won, Bae) Retrieve the all Graph Names
	virtual BOOL	GetAllGraphName( CString &p_strGraphNames);

// (2006/6/25 - Seung-Won, Bae) Mouse Position Info
protected:
	// (2006/2/6 - Seung-Won, Bae) Get Full Column Region for DragZoom
	virtual CRect		GetGraphRegionSumInColumn( const CPoint &p_ptParam) const;
	// (2006/2/6 - Seung-Won, Bae) Get Full Column Region for DragZoom
	virtual BOOL		GetBlockIndex( CPoint p_ptPoint, int &p_nRow, int &p_nColumn);

// (2006/3/24 - Seung-Won, Bae) Sub Graph View Data Iterator
//		Define Iterator Information Object of SubGraphViewData
class CSubGraphViewDataIterator : public CObject
{
public:
	CSubGraphViewDataIterator()
	{
		m_pViewData = NULL;
		ResetData();
		m_pViewData = new CViewData;
	}
	CSubGraphViewDataIterator( CSubGraphViewDataIterator &p_pSubGraphViewDataIterator)
	{
		m_pViewData				= p_pSubGraphViewDataIterator.m_pViewData;
		m_pColumnViewDataList	= p_pSubGraphViewDataIterator.m_pColumnViewDataList;
		m_posColumnViewData		= p_pSubGraphViewDataIterator.m_posColumnViewData;
		m_pColumnViewData		= p_pSubGraphViewDataIterator.m_pColumnViewData;
		m_pRowViewDataList		= p_pSubGraphViewDataIterator.m_pRowViewDataList;
		m_posRowViewData		= p_pSubGraphViewDataIterator.m_posRowViewData;
		m_pRowViewData			= p_pSubGraphViewDataIterator.m_pRowViewData;
		m_pSubGraphViewDataList	= p_pSubGraphViewDataIterator.m_pSubGraphViewDataList;
		m_posSubGraphViewData	= p_pSubGraphViewDataIterator.m_posSubGraphViewData;
		m_pSubGraphViewData		= p_pSubGraphViewDataIterator.m_pSubGraphViewData;
		p_pSubGraphViewDataIterator.m_pViewData = NULL;
		p_pSubGraphViewDataIterator.ResetData();
	}
	~CSubGraphViewDataIterator()	{	ResetData();	}
public:
	void	ResetData( void)
	{
		if( m_pViewData)
		{
			delete m_pViewData;
			m_pViewData = NULL;
		}
		m_pColumnViewDataList	= NULL;
		m_posColumnViewData		= NULL;
		m_pColumnViewData		= NULL;
		m_pRowViewDataList		= NULL;
		m_posRowViewData		= NULL;
		m_pRowViewData			= NULL;
		m_pSubGraphViewDataList	= NULL;
		m_posSubGraphViewData	= NULL;
		m_pSubGraphViewData		= NULL;
	}
public:
	CViewData *										m_pViewData;
	CList<CColumnViewData, CColumnViewData&> *		m_pColumnViewDataList;
	POSITION										m_posColumnViewData;
	CColumnViewData *								m_pColumnViewData;
	CList<CRowViewData, CRowViewData&> *			m_pRowViewDataList;
	POSITION										m_posRowViewData;
	CRowViewData *									m_pRowViewData;
	CList<CSubGraphViewData, CSubGraphViewData&> *	m_pSubGraphViewDataList;
	POSITION										m_posSubGraphViewData;
	CSubGraphViewData *								m_pSubGraphViewData;
};
protected:
	CTypedPtrList< CObList, CSubGraphViewDataIterator *>	m_lSubGraphViewData;
protected:
	virtual POSITION	GetSubGraphViewData_First( void);
	virtual void		GetSubGraphViewData_Next( POSITION &p_psnViewData);
	virtual void		GetSubGraphViewData_NextInColumn( POSITION &p_psnViewData);
	virtual void		GetSubGraphViewData_NextInRow( POSITION &p_psnViewData);
	virtual BOOL		GetSubGraphViewData_Index( POSITION &p_psnViewData, int p_nRow, int p_nColumn);
	virtual void		GetSubGraphViewData_Close( POSITION &p_psnViewData);
protected:
	virtual	void		GetSubGraphViewData_HorzScaleName( POSITION p_psnViewData, CString &p_strHorzScaleName);
	virtual void		GetSubGraphViewData_GraphRegion( POSITION p_psnViewData, CRect &p_rctGraphRegion);
	virtual void		GetSubGraphViewData_Title( POSITION p_psnViewData, CString &p_strTitle);
	virtual void		GetSubGraphViewData_TitleAndPacketNames( POSITION p_psnViewData, CString &p_strTitle, CString &p_strPacketNames);
protected:
	void				GetSubGraphViewData_GetFirstColumnViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator);
	void				GetSubGraphViewData_GetIndexColumnViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator, int p_nColumn);
	void				GetSubGraphViewData_GetNextColumnViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator);
	void				GetSubGraphViewData_GetFirstRowViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator);
	void				GetSubGraphViewData_GetIndexRowViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator, int p_nRow);
	void				GetSubGraphViewData_GetNextRowViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator);
	void				GetSubGraphViewData_GetFirstSubGraphViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator);
	void				GetSubGraphViewData_GetNextSubGraphViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator);
};

#endif // !defined(AFX_IWCHARTMANAGER_H__7FD49D7A_F392_4267_982D_CBFE6535E786__INCLUDED_)
