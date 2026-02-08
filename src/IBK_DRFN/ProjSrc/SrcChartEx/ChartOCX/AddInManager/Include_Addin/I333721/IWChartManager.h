// IWChartManager.h: interface for the CIWChartManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWCHARTMANAGER_H__7FD49D7A_F392_4267_982D_CBFE6535E786__INCLUDED_)
#define AFX_IWCHARTMANAGER_H__7FD49D7A_F392_4267_982D_CBFE6535E786__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>										// for CTypedPtrList

#include "../../../Include_Chart/DLL_Load/MainBlock.h"		// for CMainBlock
#include "../../../Include_Chart/ViewData.h"				// for CViewData
#include "../_IWUnknown.h"
#include "_IChartManager.h"
#include "_IChartOCX.h"										// for IChartOCX

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
	_IWRAPPER_IUNKNOWN_MANAGER( IChartManager, CIWChartManager, CMainBlock, m_pMainBlock)
protected:
	virtual void	PreDestruct( void)
	{
		// (2006/3/24 - Seung-Won, Bae) Sub Graph View Data Iterator
		//		Define Iterator Information Object of SubGraphViewData
		while( !m_lSubGraphViewData.IsEmpty()) delete m_lSubGraphViewData.RemoveTail();
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
	virtual IString *					GetBlocksPositionString( void);
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
	virtual IString *	GetPacketNameStringOfGraph( const char *p_szGraphName);
	// [04/10/27] 특정 Graph가 등록된 Block, ScaleGroup, Graph의 Index를 조회하는 Interface를 제공한다.
	virtual BOOL		GetIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG);
	// RQ별 찾기 : 복수종목 - ojtaso (20070306)
	virtual BOOL		GetIndexOfGraph( const char *p_szRQ, const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG);
	// (2006/6/27 - Seung-Won, Bae) Retrieve the all Graph Names
	// (2006/6/27 - Seung-Won, Bae) Retrieve the all Graph Names
	virtual IString *	GetAllIndicatorNameString( void);
	// (2006/8/15 - Seung-Won, Bae) Retrieve the all Graph Names with List
	virtual IStringList *	GetAllIndicatorNameStringList( void);

// (2006/6/25 - Seung-Won, Bae) Mouse Position Info
protected:
	// (2006/2/6 - Seung-Won, Bae) Get Full Column Region for DragZoom
	virtual RECT		GetGraphRegionSumInColumn( const POINT p_ptParam) const;
	// (2006/2/6 - Seung-Won, Bae) Get Full Column Region for DragZoom
	virtual BOOL		GetBlockIndex( const POINT p_ptPoint, int &p_nRow, int &p_nColumn);

// (2006/7/27 - Seung-Won, Bae) Graph Calulation.
protected:
	virtual BOOL		Initialize_GraphInfo( const bool p_bIsNULL);
	virtual BOOL		Initialize_AllPackets( const char *p_szGraphName, BOOL p_bWithCalculate);
	virtual BOOL		ChangeAllGraphInfoFromGraphData( void);
	virtual BOOL		ChangeGraphDataFromGraphInfo( const char *p_szGraphName, BOOL p_bWithCalculate);
	virtual BOOL		CalculateAllGraphs( const CCalculateBaseData::CALCULATETYPE eCalType = CCalculateBaseData::TRDATA_CAL);
	virtual BOOL		CalculateGraph( const char *p_szGraphName, const CCalculateBaseData::CALCULATETYPE eCalType, BOOL p_bWithClearPacketData);

// (2007/1/20 - Seung-Won, Bae) Search Indicator
protected:
	virtual IString *		FindFirstIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, const char *p_szRQ = "DEFAULT");
	virtual IString *		FindNextIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG);

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
	virtual BOOL		GetSubGraphViewData_Next( POSITION &p_psnViewData, BOOL p_bAutoClose = TRUE);
	virtual BOOL		GetSubGraphViewData_NextInColumn( POSITION &p_psnViewData, BOOL p_bAutoClose = TRUE);
	virtual BOOL		GetSubGraphViewData_NextInRow( POSITION &p_psnViewData, BOOL p_bAutoClose = TRUE);
	virtual BOOL		GetSubGraphViewData_Index( POSITION &p_psnViewData, int p_nRow, int p_nColumn);
	virtual void		GetSubGraphViewData_Close( POSITION &p_psnViewData);
protected:
	virtual	IString *	GetSubGraphViewData_HorzScaleNameString( POSITION p_psnViewData);
	virtual RECT		GetSubGraphViewData_GraphRegion( POSITION p_psnViewData);
	virtual IString *	GetSubGraphViewData_TitleString( POSITION p_psnViewData);
	virtual void		GetSubGraphViewData_TitleAndPacketNameString( POSITION p_psnViewData, ILPCSTR &p_pszTitle, ILPCSTR &p_pszPacketNames);
protected:
	void				GetSubGraphViewData_GetFirstColumnViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator);
	void				GetSubGraphViewData_GetIndexColumnViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator, int p_nColumn);
	void				GetSubGraphViewData_GetNextColumnViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator);
	void				GetSubGraphViewData_GetFirstRowViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator);
	void				GetSubGraphViewData_GetIndexRowViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator, int p_nRow);
	void				GetSubGraphViewData_GetNextRowViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator);
	void				GetSubGraphViewData_GetFirstSubGraphViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator);
	void				GetSubGraphViewData_GetNextSubGraphViewDataPosition( CSubGraphViewDataIterator *p_pSGVDIterator);
	
protected:
	// XScale : 관련 Interface로 사용 - vntsorl(20070412)
	CXScaleManager*		m_pXScaleManager;
	void SetXScaleManager(LPVOID lpXScaleManager);
	
	virtual void	RealDataReceived( const char *p_szRQ, BOOL bAppend, BOOL bCalc);
	// XScale : 관련 Interface로 사용 - vntsorl(20070412)
	virtual BOOL	GetStartEndDrawIdx_Cur( const char *p_szRQ, int &p_nStartIndex, int &p_nEndIndex);	// 현재 화면에 보여지는 영역의 시작과 끝 위치를 얻어옵니다.
	// XScale : 관련 Interface로 사용 - vntsorl(20070412)
	virtual int		GetXPosition(const char *p_szRQ, int nIdx, int nCol);
	virtual int		GetXPosition( const char *p_szRQ, int nIdx, int nCol, int& nLeft, int& nRight);

	virtual void	GetGraphXAreaSE(int& nStart, int& nEnd, BOOL bIncludeMargin = FALSE);
	virtual int		GetGraphXArea_Start(BOOL bIncludeMargin = FALSE);
	virtual int		GetGraphXArea_End(BOOL bIncludeMargin = FALSE);

	virtual void	GetGraphXAreaLR(int& nLeft, int& nRight, BOOL bIncludeMargin = FALSE);
	virtual int		GetGraphXArea_Left(BOOL bIncludeMargin = FALSE);
	virtual int		GetGraphXArea_Right(BOOL bIncludeMargin = FALSE);
	// XScale : 관련 Interface로 사용 - vntsorl(20070412)
	virtual int		FindViewDataIndex_Pt(const char *p_szRQ, int nXPoint);
	virtual time_t	GetTimeFromX(int nCursor_x, int* pnBlockColumn, int& nCenter_x);
	virtual int		GetXFromTime(time_t timeFind, int& nCenter_x);	
	// XScale : 관련 Interface로 사용 - vntsorl(20070412)
	// VertScaleGroup의 가장 처음 Graph의 타입 : 복수종목 - ojtaso (20070504)
	virtual double	GetDoubleFromX(const int nRow, const int nCol, int nCursor_x, int* pnBlockColumn, int& nCenter_x);
	virtual int		GetXFromDouble(const int nRow, const int nCol, double dblFind, int& nCenter_x);	
};

#endif // !defined(AFX_IWCHARTMANAGER_H__7FD49D7A_F392_4267_982D_CBFE6535E786__INCLUDED_)
