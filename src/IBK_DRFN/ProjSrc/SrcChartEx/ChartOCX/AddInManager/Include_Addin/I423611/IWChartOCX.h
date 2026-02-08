// IWChartOCX.h: interface for the CIWChartOCX class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_)
#define AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\_IWUnknown.h"	// for CIWUnknown
#include "_IChartOCX.h"		// for IChartOCX

class CAddInManagerImp;
class CCommonInterface;
interface IChartCtrl;
class CIWChartOCX : public CIWUnknown, public IChartOCX
{
public:
	CIWChartOCX( IChartCtrl *p_pIChartCtrl, CAddInManagerImp *p_pAddInManagerImp);
	virtual ~CIWChartOCX();

// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()

// (2004.10.08, 배승원)	Chart OCX의 Pointer를 관리한다.
//		실제로 각 OCX의 Ctrl Class이며, StdAfx.h에 Macro로 정의하여 이용한다.
protected:
	IChartCtrl *	m_pIChartCtrl;
// (2006/1/17 - Seung-Won, Bae) Manage CAddInManagerImp Object Pointer to create CWIAddInToolMgr Object
protected:
	CAddInManagerImp *	m_pAddInManagerImp;

// (2006/1/17 - Seung-Won, Bae) Get ChartCtrl Address for Special Usage
protected:
	virtual CCommonInterface *	GetCommonInterface( void);

// (2004.10.11, 배승원) OCX로부터 주요 Manager의 Interface를 구하여 관리한다.
protected:
	virtual IChartManager *		GetIChartManager( void);
	virtual IPacketManager *	GetIPacketManager( void);
	virtual IGraphInfoManager *	GetIGraphInfoManager( void);
	virtual IAddInToolMgr *		GetIAddInToolMgr( void);

// (2004.10.19, 배승원) Windows 관련 Interface를 제공한다.
protected:
	// [04.10.11] ReDrawing을 위한 Interface를 제공한다.
	virtual	long		InvalidateControl( void);
	// [04/10/11] ReDrawing을 위한 Interface를 제공한다.
	virtual HWND		GetOcxHwnd( void);
	// [04/11/02] Counter Capturing Interface를 제공한다.
	virtual HWND		AddSetCapture( void);
	// [04/11/02] Counter Capturing Interface를 제공한다.
	virtual BOOL		RemoveSetCapture( void);
	// [04/10/25] 새로운 Cursor를 설정하면서, 현재 설정된 Cursor를 확인한다.
	virtual HCURSOR		SetCursorHandle( HCURSOR p_hNewCursor);
	// [04/10/26] 현재 설정된 Cursor를 확인한다.
	virtual HCURSOR		GetCursorHandle( void);
	// [04.10.25] Chart 전용 Font를 제공한다.
	virtual CFont *		GetChartFont( void);

// (2004.10.18, 배승원) 환경 정보를 조회하는 Interface를 제공한다.
protected:
	// [04.10.18] 사용자 Folder를 구하는 Interface를 제공한다.
	virtual const char *	GetUserFilePath( void);
	// 종찬(05/07/14) HTS의 Dir Path를 알려준다.
	virtual const char *	GetHtsPath( void);
	//sy 2006.02.21. OCX Path
	virtual const char *	GetOCXPath( void);
// (2006/1/18 - Seung-Won, Bae) Set the Design Info
protected:
	virtual void		SetStrBlockRegion( const char *p_szBlockRegion);

// (2004.10.11, 배승원) Chart의 현 상태를 확인하는 Interface를 제공한다.
protected:
	// [04.10.11] 특수차트의 경우인지 확인하는 Interface를 제공한다. (역시계곡선/삼선전환도/P&F)
	virtual BOOL	IsOnSpecialChart( void);
	// (2005.01.20, 배승원) ToolBar에 다음 Item이 있는지 확인하는 Interface를 제공한다.
	virtual	BOOL	HasToolItem( CToolOptionInfo::TOOLOPTION p_eToolItem);
	// (2006/1/20 - Seung-Won, Bae) Get Numerical Inquiry Option Setting
	virtual UINT	GetNumericalInquiryOption( void) const;
	// (2006/1/17 - Seung-Won, Bae) Check RunTimeMode
	virtual BOOL	IsRuntimeMode( void) const;
	// (2006/1/21 - Seung-Won, Bae) Check Graph Menu Usable
	virtual BOOL	IsGraphMenuShow( void) const;
	// (2006/1/22 - Seung-Won, Bae) Retrieve Bong Count Margin.
	virtual short	GetBongCountMargin( void) const;
	// (2006/1/22 - Seung-Won, Bae) Retrieve Graph RButton Type.
	virtual short	GetGraphRButtonType( void) const;
	// (2006/2/6 - Seung-Won, Bae) Get OCX Region except ToolBar Region.
	virtual	CRect	GetOcxRegion( void) const;
	// (2006/2/6 - Seung-Won, Bae) Check ScrollBar
	virtual BOOL	IsUseToolScrollBar( void) const;

// (2006/1/24 - Seung-Won, Bae) Interface for Analysis Tool Option
protected:
	virtual BOOL		IsShowTrendLineText( void)		const;
	virtual BOOL		IsShowHorizonTextLeft( void)	const;
	virtual BOOL		IsShowHorizonTextRight( void)	const;
	virtual BOOL		IsShowVerticalText( void)		const;
	virtual BOOL		IsShowThreeText( void)			const;
	virtual BOOL		IsShowFourText( void)			const;
	virtual BOOL		IsShowFiboText( void)			const;
	virtual COLORREF	GetAnalysisToolColor( void)		const;

// (2004.10.11, 배승원) Data Index 및 Position 관련 Interface
protected:
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
	// (2005.01.13, 배승원) 차트의 전체보기 Mode 여부를 확인 및 설정하는 Interface를 제공한다.
	virtual BOOL	CheckFullView( void) const;
	virtual void	SetFullView( BOOL p_bFullView);
	// (2006/2/6 - Seung-Won, Bae) DragZoomIn for DragZoom AddIn
	virtual void	DragZoomIn( const CPoint &p_ptStart, const CPoint &p_ptEnd);

// [04.10.11] 공유 관련 Interface
protected:
	// [04.10.11] 주어진 Global 공유가 설정되어 있는지 확인하는 Interface를 제공한다.
	virtual BOOL	CheckGSharedSetting( const char *p_szGSharedName, BOOL p_bStart, BOOL p_bGet, BOOL p_bSet);
	// [04.10.11] 주어진 Global 공유에 공유를 발생시키는 Interface를 제공한다.
	virtual BOOL	FireGSharedData( const char *p_szGSharedName, const char *p_szGSharedData);
// (2005/8/10 - Seung-Won, Bae) used in Only Hanwha
protected:
	// (2005/8/10 - Seung-Won, Bae) Retrieve the Share Info Item Setting
	virtual BOOL	GetShareInfoItem( const char *p_szShareName, int &p_nShareType, int &p_nShareArea, long &p_lCodeFilter);
	// (2005/8/10 - Seung-Won, Bae) Fire Shared Data
	virtual BOOL	FireSharedData( const char *p_szShareName, const char *p_szSharedData);

// (2004.10.19, 배승원) Image 저장 및 출력을 위한 Drawing Interface를 제공한다.
protected:
	virtual void	DrawForImage( CDC *p_pDC, CRect p_rctSource, BOOL p_bUseScreenColor);

// 종찬(05/06/07) 그래프의 추가,삭제,변경 관련 Interface를 제공한다.
protected:
	virtual void		AddPacket(const char* pcszPacketOption);	// Packet 추가.
	virtual void		AddGraph(short nRow, short nColumn, short nVertScaleGroup, const char* szGraphOption, BOOL bCreateBlock, BOOL bCreateVertScaleGroup, BOOL bUseAutoScale);	// Graph 추가.
	virtual bool		ChangeSubGraphData(const char* pcszGraphName, const char* pcszSubGraphName, COLORREF clrLineColor, int nThickness, int nDrawStyle);	// 그래프의 속성변경.
	virtual bool		SetStartPeriodOfPacket(const char* pcszPacketName, long lStartPeriod, BOOL bAddSpace);	// Packet의 시작일자를 지정한다.
	virtual bool		SetStartIndexOfPacket(const char* pcszPacketName, int nStartDataIndex, BOOL bAddSpace);
	virtual bool		HasGraph(const char* pcszGraphName);	// 해당 Graph가 있는지 확인한다.
	virtual bool		RemoveGraph(const char* pcszGraphName);
	virtual bool		RemovePacket(const char* pcszPacketName, const bool bIsOnlyDataClear);

// 종찬(05/08/22) 리얼을 처리할수 있는 Interface를 제공한다.
// nOption = 1:Update, 2:Append
protected:
	virtual void		SetRealData(const char* pcszPacketName, double dData, short nOption);
	// (2006/1/20 - Seung-Won, Bae) Show Tooltip
	virtual void		ShowTooltip( const CPoint &p_ptPoint);
	virtual void		HideTooltip( void);
	// (2006/1/21 - Seung-Won, Bae) Save Current Graph State
	virtual BOOL		SaveChartCfg( void);
	// (2006/1/23 - Seung-Won, Bae) Set Logging Flag
	virtual void		SetLoggingFlag( const BOOL p_bInLogging);
	// (2006/1/28 - Seung-Won, Bae) Reset Tool UI
	virtual void		ReleaseToolUI( const CToolOptionInfo::TOOLOPTION p_eToolOption);
	// (2006/2/7 - Seung-Won, Bae) Show and Hide Numerical Inquiry Dialog
	virtual void		ShowNumericalInquiryDlg( const BOOL p_bShow, const BOOL p_bAutoMove);
	virtual void		ShowNumericalInquiryDlg( CDC *p_pDC, const CPoint &p_ptPoint);

// (2006/1/17 - Seung-Won, Bae) Notify from AddIn
protected:
	// (2005/8/18 - Seung-Won, Bae) for Event from AddIn
	virtual void		FireAddInEvent( const char *p_szAddInName, short p_nEventID, const char *p_szData, double p_dData, long p_lData);
	// [04.10.11] DLL Container에게 Message하는 Interface를 제공한다.
	virtual LRESULT		SendEventMsgToDLL( UINT message, WPARAM wParam, LPARAM lParam);
	// (2006/11/15 - Seung-Won, Bae) Notify Tool State Changing to MChart an ToolBar
	virtual BOOL		OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn);
	// (2007/1/5 - Seung-Won, Bae) Excute Tool Command
	virtual BOOL		RunToolCommand( const CToolOptionInfo::TOOLOPTION p_eToolOption, const int p_nParam);

// [07.05.18] 복수종목
	virtual IString* GetSelectedRQ( void);
};

#endif // !defined(AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_)
