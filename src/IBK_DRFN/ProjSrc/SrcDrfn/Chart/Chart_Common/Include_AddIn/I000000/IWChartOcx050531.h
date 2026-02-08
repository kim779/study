// IWChartOcx050531.h: interface for the CIWChartOcx050531 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWCHARTOCX050531_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_)
#define AFX_IWCHARTOCX050531_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_

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

// (2004.10.08, 배승원) 050531 Version의 Chart Ocx Interface Wrapper Class를 정의한다.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\IWUnknown.h"		// for CIWUnknown
#include "IChartOcx050531.h"	// for IChartOcx050531

class MChartOcx;
class CIWChartOcx050531 : public CIWUnknown, public IChartOcx050531
{
public:
	CIWChartOcx050531( MChartOcx *p_pChartOCX);
	virtual ~CIWChartOcx050531();

// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()

// (2004.10.08, 배승원)	Chart OCX의 Pointer를 관리한다.
//		실제로 각 OCX의 Ctrl Class이며, StdAfx.h에 Macro로 정의하여 이용한다.
protected:
	MChartOcx *		m_pChartOcx;

// (2004.10.19, 배승원) Windows 관련 Interface를 제공한다.
public:
	// [04.10.11] ReDrawing을 위한 Interface를 제공한다.
	virtual	void	InvalidateControl( void);
	// [04/10/11] ReDrawing을 위한 Interface를 제공한다.
	virtual HWND	GetOcxHwnd( void);
	// [04/11/02] Counter Capturing Interface를 제공한다.
	virtual HWND	AddSetCapture( void);
	// [04/11/02] Counter Capturing Interface를 제공한다.
	virtual BOOL	RemoveSetCapture( void);

// (2004.10.18, 배승원) 환경 정보를 조회하는 Interface를 제공한다.
public:
	// [04.10.18] 사용자 Folder를 구하는 Interface를 제공한다.
	virtual BOOL	GetUserFilePath( CString &p_strUserFilePath);
	// [04.10.25] Chart 전용 Font를 제공한다.
	virtual CFont *		GetChartFont( void);

// (2004.10.11, 배승원) Chart의 현 상태를 확인하는 Interface를 제공한다.
public:
	// [04.10.11] 특수차트의 경우인지 확인하는 Interface를 제공한다. (역시계곡선/삼선전환도/P&F)
	virtual BOOL	IsOnSpecialChart( void);

// (2005.01.20, 배승원) ToolBar에 다음 Item이 있는지 확인하는 Interface를 제공한다.
public:
	virtual	BOOL	HasToolItem( CToolOptionInfo::TOOLOPTION p_eToolItem);

// (2004.10.11, 배승원) Data Index 및 Position 관련 Interface
public:
	// [04/10/11] 마지막 실 Data의 Index를 구하는 Interface를 제공한다.
	virtual int		GetLastRealDataIndex( void);
	// [04/10/11] ScrollBar상의 마지막 Scroll Index를 구하는 Interface를 제공한다. (여백포함)
	virtual BOOL	GetMaxScrollRange( int &p_nMaxScrollRange);
	// [04/10/11] Chart의 현재 보이는 View 영역의 첫 Data Index와 끝 Data Index를 구하거나, 설정하는 Interface를 제공한다.
	virtual BOOL	GetViewStartEndIndex( int &p_nStartIndex, int &p_nEndIndex);
	virtual BOOL	SetViewStartEndIndex( int p_nStartIndex, int p_nEndIndex);
	// [04/10/27] Data 좌표와 Screen 좌표를 변환하는 Interface를 제공한다.
	virtual int		ConvertDataToYPosition( double p_dData, double p_dDataMax, double p_dDataMin,
						int p_nYPositionTop, int p_nYPositionBottom, BOOL p_bLog, BOOL p_bReverse);
	virtual double	ConvertYPositionToData( int p_nYPosition, int p_nYPositionTop, int p_nYPositionBottom,
						double p_dDataMax, double p_dDataMin, BOOL p_bLog, BOOL p_bReverse);
	// [04/11/09] Scroll/Zoom을 쉽게 처리하는 Interface를 제공한다.
	virtual BOOL	ScrollTo( int p_nScrollValue, BOOL p_bRelative);
	virtual BOOL	ZoomTo( int p_nZoomValue, BOOL p_bRelative);

// [04.10.11] 공유 관련 Interface
public:
	// [04.10.11] 주어진 Global 공유가 설정되어 있는지 확인하는 Interface를 제공한다.
	virtual BOOL	CheckGSharedSetting( const char *p_szGSharedName, BOOL p_bStart, BOOL p_bGet, BOOL p_bSet);
	// [04.10.11] 주어진 Global 공유에 공유를 발생시키는 Interface를 제공한다.
	virtual BOOL	FireGSharedData( const char *p_szGSharedName, const char *p_szGSharedData);

// (2004.10.11, 배승원) OCX로부터 주요 Manager의 Interface를 구하여 관리한다.
public:
	virtual IChartManager050531 *		GetIChartManager050531( void);
	virtual IPacketManager050531 *		GetIPacketManager050531( void);
	virtual IGraphInfoManager050531 *	GetIGraphInfoManager050531( void);

// (2004.10.19, 배승원) Image 저장 및 출력을 위한 Drawing Interface를 제공한다.
public:
	virtual void	DrawForImage( CDC *p_pDC, CRect p_rctSource, BOOL p_bUseScreenColor);

// (2004.10.25, 배승원) Cursor 관련 Interface를 제공한다.
public:
	// [04/10/25] 새로운 Cursor를 설정하면서, 현재 설정된 Cursor를 확인한다.
	virtual HCURSOR		SetCursorHandle( HCURSOR p_hNewCursor);
	// [04/10/26] 현재 설정된 Cursor를 확인한다.
	virtual HCURSOR		GetCursorHandle( void);

// (2005.01.13, 배승원) 차트의 전체보기 Mode 여부를 확인 및 설정하는 Interface를 제공한다.
public:
	virtual BOOL		CheckFullView( void) const;
	virtual void		SetFullView( BOOL p_bFullView);

// 종찬(05/06/07) 그래프의 추가,삭제,변경 관련 Interface를 제공한다.
public:
	virtual void		AddPacket(const char* pcszPacketOption);	// Packet 추가.
	virtual void		AddGraph(short nRow, short nColumn, short nVertScaleGroup, const char* szGraphOption, BOOL bCreateBlock, BOOL bCreateVertScaleGroup, BOOL bUseAutoScale);	// Graph 추가.
	virtual bool		ChangeSubGraphData(const char* pcszIndicatorName, const char* pcszSubGraphName, COLORREF clrLineColor, int nThickness, int nDrawStyle);	// 그래프의 속성변경.
	virtual bool		SetStartPeriodOfPacket(const char* pcszPacketName, long lStartPeriod, BOOL bAddSpace);	// Packet의 시작일자를 지정한다.
	virtual bool		SetStartIndexOfPacket(const char* pcszPacketName, int nStartDataIndex, BOOL bAddSpace);
	virtual bool		HasGraph(const char* pcszGraphName);	// 해당 Graph가 있는지 확인한다.
	virtual bool		RemoveGraph(const char* pcszIndicatorName);
	virtual bool		RemovePacket(const char* pcszPacketName, const bool bIsOnlyDataClear);

// 종찬(05/07/14) HTS의 Dir Path를 알려준다.
	virtual LPCTSTR		GetHtsPath(void);

	// 종찬(05/08/22) 리얼을 처리할수 있는 Interface를 제공한다.
	virtual void		SetRealData(const char* pcszPacketName, double dData, short nOption);

// (2005/8/18 - Seung-Won, Bae) for Event from AddIn
public:
	virtual void		FireAddInEvent( const char *p_szAddInName, short p_nEventID, const char *p_szData, double p_dData, long p_lData);

//////////////////////////////////////////////////////////////////////
// 050531 Version, 전용 Interface
//		AddIn Module				Site			Manager
//		ChartCustomAddIn			한화			배승원
//		ChartAssistIndexAddIn		한화			오근문
//////////////////////////////////////////////////////////////////////

// Temp Interface I : DLL Container 연계 Interface
public:
	// [04.10.11] DLL Container에게 Message하는 Interface를 제공한다.
	virtual LRESULT		SendEventMsgToDLL( UINT message, WPARAM wParam, LPARAM lParam);


// (2005/8/10 - Seung-Won, Bae) used in Only Hanwha
public:
	// (2005/8/10 - Seung-Won, Bae) Retrieve the Share Info Item Setting
	virtual BOOL	GetShareInfoItem( const char *p_szShareName, int &p_nShareType, int &p_nShareArea, long &p_lCodeFilter);
	// (2005/8/10 - Seung-Won, Bae) Fire Shared Data
	virtual BOOL	FireSharedData( const char *p_szShareName, const char *p_szSharedData);
};

#endif // !defined(AFX_IWCHARTOCX050531_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_)
