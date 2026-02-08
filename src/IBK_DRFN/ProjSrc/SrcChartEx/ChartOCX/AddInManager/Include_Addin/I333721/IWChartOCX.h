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
// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()
protected:
	CIWChartOCX( IChartCtrl *p_pIChartCtrl, CAddInManagerImp *p_pAddInManagerImp);
	~CIWChartOCX();
public:
	static IChartOCX *CreateObject( IChartCtrl *p_pIChartCtrl, CAddInManagerImp *p_pAddInManagerImp);

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
	virtual CMChartInterface *	GetMChartInterface( void);

// (2004.10.11, 배승원) OCX로부터 주요 Manager의 Interface를 구하여 관리한다.
protected:
	virtual IChartManager *			GetIChartManager( void);
	virtual IPacketListManager *	GetIPacketListManager( void);
	virtual IPacketManager *		GetIPacketManager( void);
	virtual IGraphInfoManager *		GetIGraphInfoManager( void);
	virtual IAddInToolMgr *			GetIAddInToolMgr( void);

// (2006/8/15 - Seung-Won, Bae) Create Buffer
protected:
	virtual IStringList *	CreateObjectIStringList( void);

// (2006/4/18 - Seung-Won, Bae) Get/Set OCX's Properties
protected:
	virtual BOOL GetAllProperties3( IPropertyMap *p_pIProperties, BOOL p_bWithFull);
	virtual BOOL SetAllProperties3( IPropertyMap *p_pIProperties);

// (2004.10.19, 배승원) Windows 관련 Interface를 제공한다.
protected:
	// [04.10.11] ReDrawing을 위한 Interface를 제공한다.
	virtual	long		InvalidateControl( void);
	virtual void		InvalidateControlWithResetBlockRegion( void);
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
	// (2004.10.25, 배승원) Chart 전용 Font를 제공한다.
	virtual HFONT		GetChartFont( void);

// (2004.10.18, 배승원) 환경 정보를 조회하는 Interface를 제공한다.
protected:
	// [04.10.18] 사용자 Folder를 구하는 Interface를 제공한다.
	virtual IString *	GetUserFilePathString( void);
	// 종찬(05/07/14) HTS의 Dir Path를 알려준다.
	virtual IString *	GetHtsPathString( void);
	//sy 2006.02.21. OCX Path
	virtual IString *	GetOCXPathString( void);
	// (2006/8/19 - Seung-Won, Bae) Default File (Data) Folder Path
	virtual IString *	GetDefaultPathString( void);
// (2006/1/18 - Seung-Won, Bae) Set the Design Info
protected:
	virtual void		SetStrBlockRegion( const char *p_szBlockRegion);

// (2004.10.11, 배승원) Chart의 현 상태를 확인하는 Interface를 제공한다.
protected:
	// [04.10.11] 특수차트의 경우인지 확인하는 Interface를 제공한다. (역시계곡선/삼선전환도/P&F)
	virtual BOOL		IsOnSpecialChart( void);
	// (2005.01.20, 배승원) ToolBar에 다음 Item이 있는지 확인하는 Interface를 제공한다.
	virtual	BOOL		HasToolItem( CToolOptionInfo::TOOLOPTION p_eToolItem);
	// (2006/1/20 - Seung-Won, Bae) Get Numerical Inquiry Option Setting
	virtual UINT		GetNumericalInquiryOption( void) const;
	virtual BOOL		SetNumericalInquiryOption( UINT p_uOption);
	// (2006/1/17 - Seung-Won, Bae) Check RunTimeMode
	virtual BOOL		IsRuntimeMode( void) const;
	// (2006/1/21 - Seung-Won, Bae) Check Graph Menu Usable
	virtual BOOL		IsGraphMenuShow( void) const;
	// (2006/1/22 - Seung-Won, Bae) Retrieve Bong Count Margin.
	virtual short		GetBongCountMargin( void) const;
	// (2006/1/22 - Seung-Won, Bae) Retrieve Graph RButton Type.
	virtual short		GetGraphRButtonType( void) const;
	// (2006/2/6 - Seung-Won, Bae) Get OCX Region except ToolBar Region.
	virtual	RECT		GetOcxRegion( void) const;
	// (2006/2/6 - Seung-Won, Bae) Check ScrollBar
	virtual BOOL		IsUseToolScrollBar( void) const;
	// (2006/6/24 - Seung-Won, Bae) Get Tooltip's Background color.
	virtual COLORREF	GetToolTipBackColor( void);
	// (2004.02.11, 배승원) 수치조회창이 1개 Block 내에서만 이동하지 않고,
	//		Script에서 조정한 Property에 따라 OCX 전반에 걸쳐 이동될 수 있도록 개선한다.
	//		(Property를 조회하는 Interface를 제공한다.)
	virtual BOOL		IsOcxAreaNumericalInquiry( void);
	// (2006/3/29 - Seung-Won, Bae) Dynamic Caption Setting for each Site.
	virtual BOOL		IsOnCaptionFlagOfNIDLG( void) const;
	// (2006/7/4 - Seung-Won, Bae) Retrieve the Packet Setting Info
	virtual IString *	GetPacketInfoString( void);
	// (2006/7/25 - Seung-Won, Bae) Get/Set Normalized Minute Time Rule
	virtual BOOL		GetNormalizedMinuteTimeRule( void);
	virtual BOOL		SetNormalizedMinuteTimeRule( BOOL p_bEnable);
	// (2006/7/26 - Seung-Won, Bae) Check Logging State
	virtual BOOL		IsInLogging( void);
	// (2006/7/26 - Seung-Won, Bae) Check Mode in Real Usage
	virtual BOOL		IsInUseRealData( void);
	// (2006/7/27 - Seung-Won, Bae) Get Redraw after Updated All Packet Flag
	virtual BOOL		GetRedrawAfterUpdatedAllPacketFlag( void);

// (2006/1/24 - Seung-Won, Bae) Interface for Analysis Tool Option
protected:
	virtual BOOL		IsShowTrendLineText( void)		const;
	virtual BOOL		IsShowHorizonTextLeft( void)	const;
	virtual BOOL		IsShowHorizonTextRight( void)	const;
	virtual BOOL		IsShowVerticalText( void)		const;
	virtual BOOL		IsShowThreeText( void)			const;
	virtual BOOL		IsShowFourText( void)			const;
	virtual BOOL		IsShowFiboText( void)			const;
	virtual BOOL		SetAnalysisToolTextFlag( BOOL p_bTrendLine, BOOL p_bHorizonLeft, BOOL p_bHorizonRight, BOOL p_bVerticalLine, BOOL p_bThreeLine, BOOL p_bFourLine, BOOL p_bFiboLine);

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
	virtual void	DragZoomIn( const POINT p_ptStart, const POINT p_ptEnd);
	// (2006/7/27 - Seung-Won, Bae) Reset Scroll
	virtual BOOL	SetScrollData( const int nType = 0, const BOOL bIsHorzScaleDataCheck = TRUE, const BOOL bIsRedraw = FALSE);
	// (2006/8/16 - Seung-Won, Bae) Right Virtual Candle
	virtual int		GetRightMargin( void);
	virtual BOOL	SetRightMargin( const int p_nCount);


// [04.10.11] 공유 관련 Interface
protected:
	// [04.10.11] 주어진 Global 공유가 설정되어 있는지 확인하는 Interface를 제공한다.
	virtual BOOL		CheckGSharedSetting( const char *p_szGSharedName, BOOL p_bStart, BOOL p_bGet, BOOL p_bSet);
	// [04.10.11] 주어진 Global 공유에 공유를 발생시키는 Interface를 제공한다.
	virtual BOOL		FireGSharedData( const char *p_szGSharedName, const char *p_szGSharedData);
	// (2006/7/25 - Seung-Won, Bae) Set Local Shared Info with manual.
	virtual BOOL		AddLocalShareInfo( const char *p_szLSharedName, int p_nSettingType, const char *p_szLocalSharedData);
	// (2006/7/25 - Seung-Won, Bae) Get Local Shared Data.
	virtual IString *	GetLocalSharedDataString( const char *p_szLSharedName);
	// (2006/7/26 - Seung-Won, Bae) Remove Local Shared Info with manual.
	virtual BOOL		RemoveLocalShareInfo( const char *p_szLSharedName);


// (2004.10.19, 배승원) Image 저장 및 출력을 위한 Drawing Interface를 제공한다.
protected:
	virtual void	DrawForImage( HDC p_hDC, RECT p_rctSource, BOOL p_bUseScreenColor);

// 종찬(05/06/07) 그래프의 추가,삭제,변경 관련 Interface를 제공한다.
protected:
	virtual	BOOL	AddChartBlock( const char *p_szIndicatorName);
	virtual void	AddPacket(const char* pcszPacketOption);	// Packet 추가.
	virtual void	AddGraph(short nRow, short nColumn, short nVertScaleGroup, const char* szGraphOption, BOOL bCreateBlock, BOOL bCreateVertScaleGroup, BOOL bUseAutoScale);	// Graph 추가.
	virtual bool	ChangeSubGraphData(const char* pcszGraphName, const char* pcszSubGraphName, COLORREF clrLineColor, int nThickness, int nDrawStyle);	// 그래프의 속성변경.
	virtual bool	SetStartPeriodOfPacket(const char* pcszPacketName, long lStartPeriod, BOOL bAddSpace);	// Packet의 시작일자를 지정한다.
	virtual bool	SetStartIndexOfPacket(const char* pcszPacketName, int nStartDataIndex, BOOL bAddSpace);
	virtual bool	HasGraph(const char* pcszGraphName);	// 해당 Graph가 있는지 확인한다.
	virtual BOOL	HasGraphInSpecialBack( const char *p_szGraphName);
	virtual bool	RemoveGraph(const char* pcszGraphName);
	virtual bool	RemovePacket(const char* pcszPacketName, const bool bIsOnlyDataClear);

// (2006/1/21 - Seung-Won, Bae) Operations
protected:
	// 종찬(05/08/22) 리얼을 처리할수 있는 Interface를 제공한다.
	// nOption = 1:Update, 2:Append
	virtual void	SetRealData(const char* pcszPacketName, double dData, short nOption);
	// (2006/1/23 - Seung-Won, Bae) Set Logging Flag
	virtual void	SetLoggingFlag( const BOOL p_bInLogging);
	// (2006/1/28 - Seung-Won, Bae) Reset Tool UI
	virtual void	ReleaseOtherToolUI( const CToolOptionInfo::TOOLOPTION p_eToolOption);

// (2006/1/17 - Seung-Won, Bae) Notify from AddIn
protected:
	// (2005/8/18 - Seung-Won, Bae) for Event from AddIn
	virtual void	FireAddInEvent( const char *p_szAddInName, short p_nEventID, const char *p_szData, double p_dData, long p_lData);
	// [04.10.11] DLL Container에게 Message하는 Interface를 제공한다.
	virtual LRESULT	SendEventMsgToDLL( UINT message, WPARAM wParam, LPARAM lParam);

// (2006/3/23 - Seung-Won, Bae) Set/Get StyleData
protected:	// Get
	virtual	COLORREF	GetStyleValueGraphBGColor(		void);
	virtual	COLORREF	GetStyleValueGraphBorderColor(	void);
	virtual	COLORREF	GetStyleValueGraphGridColor(	void);
	virtual	int			GetStyleValueGridHeight(		void);
	virtual	int			GetStyleValueGridWidth(			void);
	virtual	COLORREF	GetStyleValueGridHLineColor(	void);
	virtual	COLORREF	GetStyleValueGridVLineColor(	void);
	virtual	COLORREF	GetStyleValueScaleTextColor(	void);
	virtual	int			GetFirstBlockHGridLineType(		void);
	virtual	int			GetFirstBlockVGridLineType(		void);
	virtual	COLORREF	GetStyleValueScaleBGColor(		void);
	virtual	COLORREF	GetStyleValueScaleBorderColor(	void);
	virtual	COLORREF	GetStyleValueAnalysisToolColor( void);
	virtual	COLORREF	GetStyleValueNIDLGColor(		void);
public:	// SetGraphRegionColor
	virtual	BOOL		SetStyleValueGraphBGColor(		COLORREF	p_clrGraphBG,		BOOL p_bWithUpdate = FALSE);
	virtual	BOOL		SetStyleValueGraphBorderColor(	COLORREF	p_clrGraphBorder,	BOOL p_bWithUpdate = FALSE);
	virtual	BOOL		SetStyleValueGraphGridColor(	COLORREF	p_clrGraphGrid,		BOOL p_bWithUpdate = FALSE);
public:	// SetGraphBackgroundGap
	virtual	BOOL		SetStyleValueGridHeight(		int			p_nGridHeight,		BOOL p_bWithUpdate = FALSE);
	virtual	BOOL		SetStyleValueGridWidth(			int			p_nGridWidth,		BOOL p_bWithUpdate = FALSE);
public:	// SetScaleColor
	virtual	BOOL		SetStyleValueGridHLineColor(	COLORREF	p_clrGridHLine,		BOOL p_bWithUpdate = FALSE);
	virtual	BOOL		SetStyleValueGridVLineColor(	COLORREF	p_clrGridVLine,		BOOL p_bWithUpdate = FALSE);
	virtual	BOOL		SetStyleValueScaleTextColor(	COLORREF	p_clrScaleText,		BOOL p_bWithUpdate = FALSE);
public:	// SetAllHorzScaleGridType & SetAllVertScaleGridType
	virtual	BOOL		SetAllHorzScaleGridType(		int			p_nGridHLineType,	BOOL p_bWithUpdate = FALSE);
	virtual	BOOL		SetAllVertScaleGridType(		int			p_nGridVLineType,	BOOL p_bWithUpdate = FALSE);
public:	// Updated in OnDraw
	virtual	BOOL		SetStyleValueScaleBGColor(		COLORREF	p_clrScaleBG);
	virtual	BOOL		SetStyleValueScaleBorderColor(	COLORREF	p_clrScaleBorder);
public:	// Updated in EAI_ALL_APPLY_STYLE
	virtual	BOOL		SetStyleValueAnalysisToolColor(	COLORREF	p_clrAnalysisTool);
	virtual	BOOL		SetStyleValueNIDLGColor(		COLORREF	p_clrNIDL);

// (2006/8/16 - Seung-Won, Bae) Set/Get the Graph Title Option
protected:
	virtual BOOL	GetGraphTitleOption( BOOL &p_bWithConditionValue, BOOL &p_bWithLastValue);
	virtual BOOL	SetGraphTitleOption( BOOL p_bWithConditionValue, BOOL p_bWithLastValue);

// (2006/8/15 - Seung-Won, Bae) with SaveChartCFG
protected:
	virtual BOOL	SaveChartCFG_RemoveFromIndexToEnd( const int p_nIndex);
	virtual BOOL	SaveChartCFG_ResetWithCurrentStyle( void);
	virtual BOOL	SaveChartCFG_RestoreChartOfIndex( const int p_nIndex);
	virtual int		SaveChartCFG_SaveNow( void);
};

#endif // !defined(AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_)
