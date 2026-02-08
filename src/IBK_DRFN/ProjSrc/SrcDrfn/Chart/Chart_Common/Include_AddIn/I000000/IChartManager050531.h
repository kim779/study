// IChartManager050531.h: interface for the IChartManager050531 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICHARTMANAGER050531_H__E56B298C_2C27_477D_8D92_4510983D4417__INCLUDED_)
#define AFX_ICHARTMANAGER050531_H__E56B298C_2C27_477D_8D92_4510983D4417__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. 주의!	050531 Version의 AddIn에서 이용되는 OCX측 Interface로 기능 추가만 가능합니다.
//		기능 삭제와 Type 변경은 허용하지 않습니다.
//		Interface 순서 변경 조차 허용되지 않습니다.
//		허용되지 않는 편집은 050531 Version의 모든 AddIn Module과 관련 Site OCX가
//			함께 수정되어 내려가는 것이 전제되어야 합니다.
// *. 050531 Version Info
//		AddIn Module				Site			Manager
//		ChartCustomAddIn			한화			배승원
//		ChartAssistIndexAddIn		한화			오근문
//////////////////////////////////////////////////////////////////////

// (2004.10.11, 배승원) MainBlock 의 Object Interface를 정의한다.
//		AddIn DLL 참조 Interface로 Macro Interface를 정의한다.
#if IChartManagerVer < 050531
	#define	IChartManager		IChartManager050531
	#undef	IChartManagerVer
	#define	IChartManagerVer	050531
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

interface IChartManager050531 : public IUnknown  
{
protected:
	virtual ~IChartManager050531()	{}

// [04/10/11] Block의 행과 열수를 조회하는 Interface를 제공한다.
public:
	virtual int		GetRowCount( void) = 0;
	virtual int		GetColumnCount( void) = 0;

// [04/10/11] Block의 영역을 조회하는 Interface를 제공한다.
public:
	// Block Region
	virtual CRect	GetBlockRegion( int p_nRowIndex, int p_nColumnIndex) = 0;	
	// Graph Region
					// View의 전체 Graph 영역이다. (Block의 View 영역이다.)
	virtual CRect	GetNoFrameGraphRegion( int p_nRowIndex, int p_nColumnIndex) = 0;	
	virtual CRect	GetNoFrameGraphRegion( CPoint p_pPoint) = 0;
					// View의 Graph Drawing영역이다. (실제 Graph의 좌표 영역이다.)
	virtual CRect	GetDrawingGraphRegion( int p_nRowIndex, int p_nColumnIndex) = 0;
	virtual CRect	GetDrawingGraphRegion( CPoint p_pPoint) = 0;

// [04/10/13] Block의 Graph Data를 조회하는 Interface를 제공한다.
public:
	// [04/10/13] Block내에 해당 지표가 등록되어 있는지 확인하는 Interface를 제공한다.
	virtual BOOL	CheckGraphInBlock( int p_nRowIndex, int p_nColumnIndex, const char *p_szGraphName) = 0;
	// [04/10/26] 지정되는 Graph의 Data Packet Name을 조회하는 Interface를 제공한다.
	virtual BOOL	GetPacketNameOfGraph( const char *p_szGraphName, CString &p_strPacketNames) = 0;
	// [04/10/27] 특정 Graph가 등록된 Block의 Index를 조회하는 Interface를 제공한다.
	virtual BOOL	GetBlockIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC) = 0;

// [04/10/13] View의 정보를 구하는 Interface를 제공한다.
public:
	// [04/10/13] 해당 지표가 있는 View의 Min/Max값을 산출하는 Interface를 제공한다.
	virtual BOOL	GetMinMaxDataInViewWithGraph( const char *p_szGraphName, double &p_dMin, double &p_dMax) = 0;
	// [04/10/27] 지정된 Block의 Y Scale Conversion Type을 구한다.
	virtual BOOL	GetVerticalScaleConversionType( int p_nR, int p_nC, int &p_bLog, int &p_bReverse) = 0;
};

#endif // !defined(AFX_ICHARTMANAGER050531_H__E56B298C_2C27_477D_8D92_4510983D4417__INCLUDED_)
