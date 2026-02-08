// IWChartManager050531.h: interface for the CIWChartManager050531 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWCHARTMANAGER050531_H__7FD49D7A_F392_4267_982D_CBFE6535E786__INCLUDED_)
#define AFX_IWCHARTMANAGER050531_H__7FD49D7A_F392_4267_982D_CBFE6535E786__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. 주의!	050531 Version의 AddIn에서 이용되는 OCX측 Interface의 Wrapper로 기능 추가만 가능합니다.
//		기능 삭제와 Type 변경은 허용하지 않습니다.
//		허용되지 않는 편집은 050531 Version의 모든 AddIn Module과 관련 Site OCX가
//			함께 수정되어 내려가는 것이 전제되어야 합니다.
// *. 050531 Version Info
//		AddIn Module				Site			Manager
//		ChartCustomAddIn			한화			배승원
//		ChartAssistIndexAddIn		한화			오근문
//////////////////////////////////////////////////////////////////////

// (2004.10.11, 배승원) MainBlock의 Interface Wrapper Class를 정의한다.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../IWUnknown.h"
#include "IChartManager050531.h"

class CMainBlock;
class CIWChartManager050531 : public CIWUnknown, public IChartManager050531
{
public:
	CIWChartManager050531( CMainBlock *p_pMainBlock);
	virtual ~CIWChartManager050531();

// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()

// (2004.10.11, 배승원) MainBlock Object의 Pointer를 관리한다.
protected:
	CMainBlock *	m_pMainBlock;

// [04/10/11] Block의 행과 열수를 조회하는 Interface를 제공한다.
public:
	virtual int		GetRowCount( void);
	virtual int		GetColumnCount( void);

// [04/10/11] Block의 영역을 조회하는 Interface를 제공한다.
public:
	// Block Region
	virtual CRect	GetBlockRegion( int p_nRowIndex, int p_nColumnIndex);	
	// Graph Region
					// View의 Graph Drawing영역이다. (실제 Graph의 좌표 영역이다.)
	virtual CRect	GetNoFrameGraphRegion( int p_nRowIndex, int p_nColumnIndex);
	virtual CRect	GetNoFrameGraphRegion( CPoint p_pPoint);
					// View의 Graph Drawing영역이다. (실제 Graph의 좌표 영역이다.)
	virtual CRect	GetDrawingGraphRegion( int p_nRowIndex, int p_nColumnIndex);
	virtual CRect	GetDrawingGraphRegion( CPoint p_pPoint);

// [04/10/13] Block의 Graph Data를 조회하는 Interface를 제공한다.
public:
	// [04/10/13] Block내에 해당 지표가 등록되어 있는지 확인하는 Interface를 제공한다.
	virtual BOOL	CheckGraphInBlock( int p_nRowIndex, int p_nColumnIndex, const char *p_szGraphName);
	// [04/10/26] 지정되는 Graph의 Data Packet Name을 조회하는 Interface를 제공한다.
	virtual BOOL	GetPacketNameOfGraph( const char *p_szGraphName, CString &p_strPacketNames);
	// [04/10/27] 특정 Graph가 등록된 첫 Block의 Index를 조회하는 Interface를 제공한다.
	virtual BOOL	GetBlockIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC);

// [04/10/13] View의 정보를 구하는 Interface를 제공한다.
public:
	// [04/10/13] 해당 지표가 있는 View의 Min/Max값을 산출하는 Interface를 제공한다.
	virtual BOOL	GetMinMaxDataInViewWithGraph( const char *p_szGraphName, double &p_dMin, double &p_dMax);
	// [04/10/27] 지정된 Block의 Y Scale Conversion Type을 구한다.
	virtual BOOL	GetVerticalScaleConversionType( int p_nR, int p_nC, int &p_bLog, int &p_bReverse);
};

#endif // !defined(AFX_IWCHARTMANAGER050531_H__7FD49D7A_F392_4267_982D_CBFE6535E786__INCLUDED_)
