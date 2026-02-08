// IWChartManager.h: interface for the CIWChartManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWCHARTMANAGER_H__7FD49D7A_F392_4267_982D_CBFE6535E786__INCLUDED_)
#define AFX_IWCHARTMANAGER_H__7FD49D7A_F392_4267_982D_CBFE6535E786__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../Include_Chart/Dll_Load/MainBlock.h"		// for CMainBlock
#include "../_IWUnknown.h"
#include "_IChartManager.h"
#include "_IChartOCX.h"										// for IChartOCX

// (2004.10.11, 배승원) MainBlock의 Interface Wrapper Class를 정의한다.
class CIWChartManager : public CIWUnknown, public IChartManager
{
// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER( IChartManager, CIWChartManager, CMainBlock, m_pMainBlock)


// (2006/1/17 - Seung-Won, Bae) Get MainBlock Address for Special Usage
protected:
	virtual CMainBlock *	GetObjectAddress( void)	{	return m_pMainBlock;	}

// [04/10/11] Block의 행과 열수를 조회하는 Interface를 제공한다.
protected:
	virtual int		GetRowCount( void);
	virtual int		GetColumnCount( void);

// [04/10/11] Block의 영역을 조회하는 Interface를 제공한다.
protected:
	// Block Region
	virtual CRect		GetBlockRegion( int p_nRowIndex, int p_nColumnIndex);	
	// Graph Region
						// View의 Graph Drawing영역이다. (실제 Graph의 좌표 영역이다.)
	virtual CRect		GetNoFrameGraphRegion( int p_nRowIndex, int p_nColumnIndex);
	virtual CRect		GetNoFrameGraphRegion( CPoint p_pPoint);
						// View의 Graph Drawing영역이다. (실제 Graph의 좌표 영역이다.)
	virtual CRect		GetDrawingGraphRegion( int p_nRowIndex, int p_nColumnIndex);
	virtual CRect		GetDrawingGraphRegion( CPoint p_pPoint);
	// Design Time String Info
	virtual BOOL		GetBlocksPosition( CString &p_strBlockPosition);
	// (2006/2/6 - Seung-Won, Bae) Get Full Column Region for DragZoom
	virtual CRect		GetGraphRegionSumInColumn( const CPoint &p_ptParam) const;

// [04/10/13] Block의 Graph Data를 조회하는 Interface를 제공한다.
protected:
	// [04/10/13] Block내에 해당 지표가 등록되어 있는지 확인하는 Interface를 제공한다.
	virtual BOOL	CheckGraphInBlock( int p_nRowIndex, int p_nColumnIndex, const char *p_szGraphName);
	// [04/10/26] 지정되는 Graph의 Data Packet Name을 조회하는 Interface를 제공한다.
	virtual BOOL	GetPacketNameOfGraph( const char *p_szGraphName, CString &p_strPacketNames);
	// [04/10/27] 특정 Graph가 등록된 첫 Block의 Index를 조회하는 Interface를 제공한다.
	virtual BOOL	GetBlockIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC);
	// (2006/2/13 - Seung-Won, Bae) Check! Is Empty?
	virtual BOOL	IsEmpty( void);

// [04/10/13] View의 정보를 구하는 Interface를 제공한다.
protected:
	// [04/10/13] 해당 지표가 있는 View의 Min/Max값을 산출하는 Interface를 제공한다.
	virtual BOOL	GetMinMaxDataInViewWithGraph( const char *p_szGraphName, double &p_dMin, double &p_dMax);
	// [04/10/27] 지정된 Block의 Y Scale Conversion Type을 구한다.
	virtual BOOL	GetVerticalScaleConversionType( int p_nR, int p_nC, int &p_bLog, int &p_bReverse);

// (2006/6/14 - Seung-Won, Bae) Get Block Interface
protected:
	virtual	IBlock *	GetBlock( int p_nRowIndex, int p_nColumnIndex);

// (2006/6/25 - Seung-Won, Bae) Graph Info
protected:
	// [04/10/27] 특정 Graph가 등록된 Block, ScaleGroup, Graph의 Index를 조회하는 Interface를 제공한다.
	virtual BOOL		GetIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG);
	// RQ별 찾기 : 복수종목 - ojtaso (20070306)
	virtual BOOL		GetIndexOfGraph( const char *p_szRQ, const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG) ;
	// (2007/1/20 - Seung-Won, Bae) Search Indicator
	virtual IString *		FindFirstIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, const char *p_szRQ);
	virtual IString *		FindNextIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, const char *p_szRQ);
	
// [07/03/26] XScale의 정보를 구하는 Interface를 제공한다.
protected:
	CXScaleManager*		m_pXScaleManager;
	void SetXScaleManager(LPVOID lpXScaleManager);
	
	virtual void	RealDataReceived( const char *p_szRQ, BOOL bAppend, BOOL bCalc);
	virtual BOOL	GetStartEndDrawIdx_Cur( const char *p_szRQ, int &p_nStartIndex, int &p_nEndIndex);	// 현재 화면에 보여지는 영역의 시작과 끝 위치를 얻어옵니다.
	virtual int		GetXPosition(const char *p_szRQ, int nIdx, int nCol);
	virtual int		GetXPosition( const char *p_szRQ, int nIdx, int nCol, int& nLeft, int& nRight);

	virtual void	GetGraphXAreaSE(int& nStart, int& nEnd, BOOL bIncludeMargin = FALSE);
	virtual int		GetGraphXArea_Start(BOOL bIncludeMargin = FALSE);
	virtual int		GetGraphXArea_End(BOOL bIncludeMargin = FALSE);

	virtual void	GetGraphXAreaLR(int& nLeft, int& nRight, BOOL bIncludeMargin = FALSE);
	virtual int		GetGraphXArea_Left(BOOL bIncludeMargin = FALSE);
	virtual int		GetGraphXArea_Right(BOOL bIncludeMargin = FALSE);
	virtual int		FindViewDataIndex_Pt(const char *p_szRQ, int nXPoint, int nCol=0);
	virtual time_t	GetTimeFromX(int nCursor_x, int* pnBlockColumn, int& nCenter_x);
	virtual int		GetXFromTime(time_t timeFind, int& nCenter_x);	
	// VertScaleGroup의 가장 처음 Graph의 타입 : 복수종목 - ojtaso (20070504)
	virtual double	GetDoubleFromX(const int nRow, const int nCol, int nCursor_x, int* pnBlockColumn, int& nCenter_x);
	virtual int		GetXFromDouble(const int nRow, const int nCol, double dblFind, int& nCenter_x);
};

#endif // !defined(AFX_IWCHARTMANAGER_H__7FD49D7A_F392_4267_982D_CBFE6535E786__INCLUDED_)
