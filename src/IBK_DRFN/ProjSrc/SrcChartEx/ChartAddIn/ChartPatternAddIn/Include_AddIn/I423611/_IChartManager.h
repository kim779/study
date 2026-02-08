// IChartManager.h: interface for the IChartManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICHARTMANAGER_H__E56B298C_2C27_477D_8D92_4510983D4417__INCLUDED_)
#define AFX_ICHARTMANAGER_H__E56B298C_2C27_477D_8D92_4510983D4417__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "_AddInVer.h"					// for AddIn Version Define

interface IBlock;
interface IString;

class CMainBlock;
interface IChartManager : public IUnknown  
{
protected:
	virtual ~IChartManager()	{}

// (2006/1/17 - Seung-Won, Bae) Get MainBlock Address for Special Usage
public:
	virtual CMainBlock *	GetObjectAddress( void)	= 0;

// [04/10/11] Block의 행과 열수를 조회하는 Interface를 제공한다.
public:
	virtual int		GetRowCount( void) = 0;
	virtual int		GetColumnCount( void) = 0;

// [04/10/11] Block의 영역을 조회하는 Interface를 제공한다.
public:
	// Block Region
	virtual CRect		GetBlockRegion( int p_nRowIndex, int p_nColumnIndex) = 0;	
	// Graph Region
						// View의 전체 Graph 영역이다. (Block의 View 영역이다.)
	virtual CRect		GetNoFrameGraphRegion( int p_nRowIndex, int p_nColumnIndex) = 0;	
	virtual CRect		GetNoFrameGraphRegion( CPoint p_pPoint) = 0;
						// View의 Graph Drawing영역이다. (실제 Graph의 좌표 영역이다.)
	virtual CRect		GetDrawingGraphRegion( int p_nRowIndex, int p_nColumnIndex) = 0;
	virtual CRect		GetDrawingGraphRegion( CPoint p_pPoint) = 0;
	// Design Time String Info
	virtual BOOL		GetBlocksPosition( CString &p_strBlockPosition)	= 0;
	// (2006/2/6 - Seung-Won, Bae) Get Full Column Region for DragZoom
	virtual CRect		GetGraphRegionSumInColumn( const CPoint &p_ptParam) const = 0;


// [04/10/13] Block의 Graph Data를 조회하는 Interface를 제공한다.
public:
	// [04/10/13] Block내에 해당 지표가 등록되어 있는지 확인하는 Interface를 제공한다.
	virtual BOOL	CheckGraphInBlock( int p_nRowIndex, int p_nColumnIndex, const char *p_szGraphName) = 0;
	// [04/10/26] 지정되는 Graph의 Data Packet Name을 조회하는 Interface를 제공한다.
	virtual BOOL	GetPacketNameOfGraph( const char *p_szGraphName, CString &p_strPacketNames) = 0;
	// [04/10/27] 특정 Graph가 등록된 Block의 Index를 조회하는 Interface를 제공한다.
	virtual BOOL	GetBlockIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC) = 0;
	// (2006/2/13 - Seung-Won, Bae) Check! Is Empty?
	virtual BOOL	IsEmpty( void) = 0;


// [04/10/13] View의 정보를 구하는 Interface를 제공한다.
public:
	// [04/10/13] 해당 지표가 있는 View의 Min/Max값을 산출하는 Interface를 제공한다.
	virtual BOOL	GetMinMaxDataInViewWithGraph( const char *p_szGraphName, double &p_dMin, double &p_dMax) = 0;
	// [04/10/27] 지정된 Block의 Y Scale Conversion Type을 구한다.
	virtual BOOL	GetVerticalScaleConversionType( int p_nR, int p_nC, int &p_bLog, int &p_bReverse) = 0;

// (2006/6/14 - Seung-Won, Bae) Get Block Interface
public:
	virtual	IBlock *	GetBlock( int p_nRowIndex, int p_nColumnIndex) = 0;

// (2006/6/25 - Seung-Won, Bae) Graph Info
public:
	// [04/10/27] 특정 Graph가 등록된 Block, ScaleGroup, Graph의 Index를 조회하는 Interface를 제공한다.
	virtual BOOL		GetIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG) = 0;
	// RQ별 찾기 : 복수종목 - ojtaso (20070306)
	virtual BOOL		GetIndexOfGraph( const char *p_szRQ, const char *p_szGraphName, int &p_nR, int &p_nC, int &p_nV, int &p_nG) = 0;
	// (2007/1/20 - Seung-Won, Bae) Search Indicator
	virtual IString *		FindFirstIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, const char *p_szRQ) = 0;
	virtual IString *		FindNextIndicator( const char *p_szIndicatorName, int &p_nR, int &p_nC, int &p_nV, int &p_nG, const char *p_szRQ) = 0;

// [07/03/26] XScale의 정보를 구하는 Interface를 제공한다.
public:
	virtual void		SetXScaleManager(LPVOID lpXScaleManager) = 0;
	virtual void		RealDataReceived( const char *p_szRQ, BOOL bAppend, BOOL bCalc = TRUE) = 0;
	virtual BOOL		GetStartEndDrawIdx_Cur( const char *p_szRQ, int &p_nStartIndex, int &p_nEndIndex) = 0;	// 현재 화면에 보여지는 영역의 시작과 끝 위치를 얻어옵니다.
	virtual int			GetXPosition(const char *p_szRQ, int nIdx, int nCol) = 0;
	virtual int			GetXPosition( const char *p_szRQ, int nIdx, int nCol, int& nLeft, int& nRight) = 0;
	virtual void		GetGraphXAreaSE(int& nStart, int& nEnd, BOOL bIncludeMargin = FALSE) = 0;
	virtual int			GetGraphXArea_Start(BOOL bIncludeMargin = FALSE) = 0;
	virtual int			GetGraphXArea_End(BOOL bIncludeMargin = FALSE) = 0;
	virtual void		GetGraphXAreaLR(int& nLeft, int& nRight, BOOL bIncludeMargin = FALSE) = 0;
	virtual int			GetGraphXArea_Left(BOOL bIncludeMargin = FALSE) = 0;
	virtual int			GetGraphXArea_Right(BOOL bIncludeMargin = FALSE) = 0;
	virtual int			FindViewDataIndex_Pt(const char *p_szRQ, int nXPoint, int nCol=0) = 0;
	virtual time_t		GetTimeFromX(int nCursor_x, int* pnBlockColumn, int& nCenter_x) = 0;
	virtual int			GetXFromTime(time_t timeFind, int& nCenter_x) = 0;
	// VertScaleGroup의 가장 처음 Graph의 타입 : 복수종목 - ojtaso (20070504)
	virtual double		GetDoubleFromX(const int nRow, const int nCol, int nCursor_x, int* pnBlockColumn, int& nCenter_x) = 0;
	virtual int			GetXFromDouble(const int nRow, const int nCol, double dblFind, int& nCenter_x) = 0;
};

#endif // !defined(AFX_ICHARTMANAGER_H__E56B298C_2C27_477D_8D92_4510983D4417__INCLUDED_)
