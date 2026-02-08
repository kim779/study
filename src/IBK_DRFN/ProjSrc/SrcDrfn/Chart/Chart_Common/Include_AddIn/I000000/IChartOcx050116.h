// IChartOcx050116.h: interface for the IChartOcx050116 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICHARTOCX050116_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_)
#define AFX_ICHARTOCX050116_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. 주의!	050116 Version의 AddIn에서 이용되는 OCX측 Interface로 기능 추가만 가능합니다.
//		기능 삭제와 Type 변경은 허용하지 않습니다.
//		Interface 순서 변경 조차 허용되지 않습니다.
//		허용되지 않는 편집은 050116 Version의 모든 AddIn Module과 관련 Site OCX가
//			함께 수정되어 내려가는 것이 전제되어야 합니다.
// *. 050116 Version Info
//		AddIn Module				Site			Manager
//			ChartCustomAddIn			GZone			배승원
//			ChartPatternAddIn			GZone			정서정
//			ChartGuideLine				GZone			남상윤
//////////////////////////////////////////////////////////////////////

// (2004.10.08, 배승원) Char OCX의 Interface를 정의한다.
//		AddIn DLL 참조 Interface로 Macro Interface를 정의한다.
#if IChartOcxVer < 050116
	#define	IChartOCX		IChartOcx050116
	#undef	IChartOcxVer
	#define	IChartOcxVer	050116
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IPacketManager050116.h"		// for IPacketManager050116
#include "IChartManager050116.h"		// for IChartManager050116
#include "IGraphInfoManager050116.h"	// for IGraphInfoManager050116
#include "../ChartDefines.h"			// for CToolOptionInfo::TOOLOPTION

interface IChartOcx050116 : public IUnknown  
{
protected:
	virtual ~IChartOcx050116() {}

// (2004.10.19, 배승원) Windows 관련 Interface를 제공한다.
public:
	// [04/10/11] ReDrawing을 위한 Interface를 제공한다.
	virtual	void	InvalidateControl( void) = 0;
	// [04/10/11] ReDrawing을 위한 Interface를 제공한다.
	virtual HWND	GetOcxHwnd( void) = 0;
	// [04/11/02] Counter Capturing Interface를 제공한다.
	virtual HWND	AddSetCapture( void) = 0;
	// [04/11/02] Counter Capturing Interface를 제공한다.
	virtual BOOL	RemoveSetCapture( void) = 0;

// (2004.10.18, 배승원) 환경 정보를 조회하는 Interface를 제공한다.
public:
	// [04/10/18] 사용자 Folder를 구하는 Interface를 제공한다.
	virtual BOOL	GetUserFilePath( CString &p_strUserFilePath) = 0;

// (2004.10.11, 배승원) Chart의 현 상태를 확인하는 Interface를 제공한다.
public:
	// [04/10/11] 특수차트의 경우인지 확인하는 Interface를 제공한다. (역시계곡선/삼선전환도/P&F)
	virtual BOOL	IsOnSpecialChart( void) = 0;

// (2005.01.20, 배승원) ToolBar에 다음 Item이 있는지 확인하는 Interface를 제공한다.
public:
	virtual	BOOL	HasToolItem( CToolOptionInfo::TOOLOPTION p_eToolItem) = 0;

// (2004.10.11, 배승원) Data Index 및 Position 관련 Interface
public:
	// [04/10/11] 마지막 실 Data의 Index를 구하는 Interface를 제공한다.
	virtual int		GetLastRealDataIndex( void) = 0;
	// [04/10/11] ScrollBar상의 마지막 Scroll Index를 구하는 Interface를 제공한다. (여백포함)
	virtual BOOL	GetMaxScrollRange( int &p_nMaxScrollRange) = 0;
	// [04/10/11] Chart의 현재 보이는 View 영역의 첫 Data Index와 끝 Data Index를 구하거나, 설정하는 Interface를 제공한다.
	virtual BOOL	GetViewStartEndIndex( int &p_nStartIndex, int &p_nEndIndex) = 0;
	virtual BOOL	SetViewStartEndIndex( int p_nStartIndex, int p_nEndIndex) = 0;
	// [04/10/27] Data 좌표와 Screen 좌표를 변환하는 Interface를 제공한다.
	virtual int		ConvertDataToYPosition( double p_dData, double p_dDataMax, double p_dDataMin,
						int p_nYPositionTop, int p_nYPositionBottom, BOOL p_bLog, BOOL p_bReverse) = 0;
	virtual double	ConvertYPositionToData( int p_nYPosition, int p_nYPositionTop, int p_nYPositionBottom,
						double p_dDataMax, double p_dDataMin, BOOL p_bLog, BOOL p_bReverse) = 0;
	// [04/11/09] Scroll/Zoom을 쉽게 처리하는 Interface를 제공한다.
	virtual BOOL	ScrollTo( int p_nScrollValue, BOOL p_bRelative)	= 0;
	virtual BOOL	ZoomTo( int p_nZoomValue, BOOL p_bRelative)		= 0;

// (2004.10.11, 배승원) 공유 관련 Interface
public:
	// [04/10/11] 주어진 Global 공유가 설정되어 있는지 확인하는 Interface를 제공한다.
	virtual BOOL	CheckGSharedSetting( const char *p_szGSharedName, BOOL p_bStart, BOOL p_bGet, BOOL p_bSet) = 0;
	// [04/10/11] 주어진 Global 공유에 공유를 발생시키는 Interface를 제공한다.
	virtual BOOL	FireGSharedData( const char *p_szGSharedName, const char *p_szGSharedData) = 0;

// [04/10/11] OCX로부터 주요 Manager의 Interface를 구하여 관리한다.
public:
	virtual IChartManager050116 *		GetIChartManager050116( void) = 0;
	virtual IPacketManager050116 *		GetIPacketManager050116( void) = 0;
	virtual IGraphInfoManager050116 *	GetIGraphInfoManager050116( void) = 0;

// (2004.10.19, 배승원) Image 저장 및 출력을 위한 Drawing Interface를 제공한다.
public:
	virtual void	DrawForImage( CDC *p_pDC, CRect p_rctSource, BOOL p_bUseScreenColor) = 0;

// (2004.10.25, 배승원) Cursor 관련 Interface를 제공한다.
public:
	// [04/10/25] 새로운 Cursor를 설정하면서, 현재 설정된 Cursor를 확인한다.
	virtual HCURSOR		SetCursorHandle( HCURSOR p_hNewCursor) = 0;
	// [04/10/26] 현재 설정된 Cursor를 확인한다.
	virtual HCURSOR		GetCursorHandle( void) = 0;

// (2004.10.25, 배승원) Chart 전용 Font를 제공한다.
public:
	virtual CFont *		GetChartFont( void) = 0;

// (2005.01.13, 배승원) 차트의 전체보기 Mode 여부를 확인 및 설정하는 Interface를 제공한다.
public:
	virtual BOOL		CheckFullView( void) const		= 0;
	virtual void		SetFullView( BOOL p_bFullView)	= 0;

//////////////////////////////////////////////////////////////////////
// 050116 Version, 전용 Interface
//		AddIn Module				Site			Manager
//			ChartCustomAddIn			GZone			배승원
//////////////////////////////////////////////////////////////////////

// Temp Interface I : DLL Container 연계 Interface
public:
	// [04/10/11] DLL Container에게 Message하는 Interface를 제공한다.
	virtual LRESULT		SendEventMsgToDLL( UINT message, WPARAM wParam, LPARAM lParam) = 0;
};

#endif // !defined(AFX_ICHARTOCX050116_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_)
