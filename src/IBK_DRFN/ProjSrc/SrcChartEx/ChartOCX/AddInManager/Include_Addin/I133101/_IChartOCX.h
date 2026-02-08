// IChartOCX.h: interface for the IChartOCX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICHARTOCX_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_)
#define AFX_ICHARTOCX_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseEnum.h"		// for CToolOptionInfo::TOOLOPTION
#include "_AddInVer.h"			// for AddIn Version Define
#include "_IString.h"					// for ILPCSTR

interface IStringList;
interface IAddInDll;
interface IAddInToolMgr;
interface IChartManager;
interface IPacketManager;
interface IGraphInfoManager;
interface IPropertyMap;
interface IPacketListManager;
interface IChartCtrl;

class COutsideCommonInterface;

interface IChartOCX : public IUnknown  
{
protected:
	virtual ~IChartOCX() {}

// (2006/1/17 - Seung-Won, Bae) Get IChartCtrl of OCX for Special Usage
public:
	virtual IChartCtrl *				GetIChartCtrl( void)		= 0;
	virtual COutsideCommonInterface *	GetMChartInterface( void)	= 0;

// [04/10/11] OCX로부터 주요 Manager의 Interface를 구하여 관리한다.
public:
	virtual IChartManager *			GetIChartManager( void)					= 0;
	virtual IPacketListManager *	GetIPacketListManager( void)			= 0;
	virtual IPacketManager *		GetIDefaultPacketManager( void)			= 0;
	virtual IGraphInfoManager *		GetIGraphInfoManager( void)				= 0;
	virtual IAddInToolMgr *			GetIAddInToolMgr( void)					= 0;

// (2006/8/15 - Seung-Won, Bae) Create Buffer
public:
	virtual IStringList *	CreateObjectIStringList( void)	= 0;

// (2006/4/18 - Seung-Won, Bae) Get/Set OCX's Properties
public:
	virtual BOOL GetAllProperties3( IPropertyMap *p_pIProperties, BOOL p_bWithFull = FALSE) = 0;
	virtual BOOL SetAllProperties3( IPropertyMap *p_pIProperties) = 0;

// (2004.10.19, 배승원) Windows 관련 Interface를 제공한다.
public:
	// [04/10/11] ReDrawing을 위한 Interface를 제공한다.
	virtual	long		InvalidateControl( void) = 0;
	virtual void		InvalidateControlWithResetBlockRegion( void) = 0;
	// [04/10/11] ReDrawing을 위한 Interface를 제공한다.
	virtual HWND		GetOcxHwnd( void) = 0;
	// [04/11/02] Counter Capturing Interface를 제공한다.
	virtual HWND		AddSetCapture( void) = 0;
	// [04/11/02] Counter Capturing Interface를 제공한다.
	virtual BOOL		RemoveSetCapture( void) = 0;
	// [04/10/25] 새로운 Cursor를 설정하면서, 현재 설정된 Cursor를 확인한다.
	virtual HCURSOR		SetCursorHandle( HCURSOR p_hNewCursor) = 0;
	// [04/10/26] 현재 설정된 Cursor를 확인한다.
	virtual HCURSOR		GetCursorHandle( void) = 0;
	// (2004.10.25, 배승원) Chart 전용 Font를 제공한다.
	virtual HFONT		GetChartFont( void) = 0;
	// (2007/3/5 - Seung-Won, Bae) Lock/Unlock WindowUpdate
	virtual void		LockWindowUpdate( void)		= 0;
	virtual void		UnlockWindowUpdate( void)	= 0;
	// (2009/6/5 - Seung-Won, Bae) for CmdUI of AddIn
	virtual void		OnCmdMsg( HMENU p_hMenu, UINT p_nItemID, int p_nItemPos, int p_nItemCount, int p_nCmdCode) = 0;

// (2004.10.18, 배승원) 환경 정보를 조회하는 Interface를 제공한다.
public:
	// [04.10.18] 사용자 Folder를 구하는 Interface를 제공한다.
	virtual IString *	GetUserFilePathString( void)	= 0;
	// 종찬(05/07/14) HTS의 Dir Path를 알려준다.
	virtual IString *	GetHtsPathString( void)			= 0;
	//sy 2006.02.21. OCX Path
	virtual IString *	GetOCXPathString( void)			= 0;
	// (2006/8/19 - Seung-Won, Bae) Default File (Data) Folder Path
	virtual IString *	GetDefaultPathString( void)		= 0;
	// (2008/4/29 - Seung-Won, Bae) to use the ProgID in ChartMetaTable.dat
	virtual IString *	GetProgID( void)				= 0;
	// (2006/1/18 - Seung-Won, Bae) Set the Design Info
	virtual void		SetStrBlockRegion( const char *p_szBlockRegion)	= 0;
	// (2008/5/13 - Seung-Won, Bae) Chart TEST ENV for Runtime.
	virtual IString *	GetTestEnvValue( const char *p_szEnvKey) = 0;
	// (2009/9/2 - Seung-Won, Bae) for Image Path
	virtual IString *	GetImagePathString( void)		= 0;

// (2004.10.11, 배승원) Chart의 현 상태를 확인하는 Interface를 제공한다.
public:
	// [04/10/11] 특수차트의 경우인지 확인하는 Interface를 제공한다. (역시계곡선/삼선전환도/P&F)
	virtual BOOL			IsOnSpecialChart( void) = 0;
	// (2005.01.20, 배승원) ToolBar에 다음 Item이 있는지 확인하는 Interface를 제공한다.
	virtual	BOOL			HasToolItem( CToolOptionInfo::TOOLOPTION p_eToolItem) = 0;
	// (2006/1/20 - Seung-Won, Bae) Get Numerical Inquiry Option Setting
	virtual UINT			GetNumericalInquiryOption( void) const = 0;
	virtual BOOL			SetNumericalInquiryOption( UINT p_uOption) = 0;
	// (2006/1/17 - Seung-Won, Bae) Check RunTimeMode
	virtual BOOL			IsRuntimeMode( void) const = 0;
	// (2006/1/21 - Seung-Won, Bae) Check Graph Menu Usable
	virtual BOOL			IsGraphMenuShow( void) const = 0;
	// (2006/1/22 - Seung-Won, Bae) Retrieve Bong Count Margin.
	virtual short			GetBongCountMargin( void) const = 0;
	// (2006/1/22 - Seung-Won, Bae) Retrieve Graph RButton Type.
	virtual short			GetGraphRButtonType( void) const = 0;
	// (2006/2/6 - Seung-Won, Bae) Get OCX Region except ToolBar Region.
	virtual	RECT			GetOcxRegion( void) const = 0;
	// (2006/2/6 - Seung-Won, Bae) Check ScrollBar
	virtual BOOL			IsUseToolScrollBar( void) const = 0;
	// (2006/6/24 - Seung-Won, Bae) Get Tooltip's Background color.
	virtual COLORREF		GetToolTipBackColor( void) = 0;
	// (2004.02.11, 배승원) 수치조회창이 1개 Block 내에서만 이동하지 않고,
	//		Script에서 조정한 Property에 따라 OCX 전반에 걸쳐 이동될 수 있도록 개선한다.
	//		(Property를 조회하는 Interface를 제공한다.)
	virtual BOOL			IsOcxAreaNumericalInquiry( void) = 0;
	// (2006/3/29 - Seung-Won, Bae) Dynamic Caption Setting for each Site.
	virtual BOOL			IsOnCaptionFlagOfNIDLG( void) const = 0;
	// (2006/7/4 - Seung-Won, Bae) Retrieve the Packet Setting Info
	virtual IString *		GetPacketInfoString( void) = 0;
	// (2006/7/25 - Seung-Won, Bae) Get Normalized Minute Time Rule
	virtual BOOL			GetNormalizedMinuteTimeRule( void) = 0;
	virtual BOOL			SetNormalizedMinuteTimeRule( BOOL p_bEnable) = 0;
	// (2006/7/26 - Seung-Won, Bae) Check Logging State
	virtual BOOL			IsInLogging( void) = 0;
	// (2006/7/26 - Seung-Won, Bae) Check Mode in Real Usage
	virtual BOOL			IsInUseRealData( void) = 0;
	// (2006/11/7 - Seung-Won, Bae) Show/Hide All Graph Title
	virtual BOOL			IsHideAllTitle( void) = 0;
	virtual BOOL			SetHideAllTitle( BOOL p_bHideAllTitle) = 0;
	// (2006/11/7 - Seung-Won, Bae) Show/Hide Price Min/Max Data Text
	virtual BOOL			IsShowPriceMinMaxText( void) = 0;
	virtual BOOL			ShowPriceMinMaxText( BOOL p_bShowPriceMinMaxText) = 0;
	// (2007/1/18 - Seung-Won, Bae) Check Calculation waiting.
	virtual BOOL			IsEnableIndicatorCalculation( void) = 0;
	// (2007/1/20 - Seung-Won, Bae) Support Multiple Same-Name Indicator.
	virtual BOOL			IsMultipleSameNameIndicator( void) = 0;
	virtual void			SetMultipleSameNameIndicator( BOOL p_bEnable) = 0;
	// (2007/3/4 - Seung-Won, Bae) Make Thread for the 'CLUTCH' of Real Update.
	virtual long			GetDrawingLoadTime( void) = 0;
	// (2007/3/27 - Seung-Won, Bae) Check TR Indicator
	virtual BOOL			IsTRIndicator( const char *p_szGroupName, const char *p_szIndicatorName) = 0;
	// (2009/6/5 - Seung-Won, Bae) Check mode for moving and deleting the graph.
	virtual BOOL			CanGraphMoveAndDelete( void) = 0;

// (2006/1/24 - Seung-Won, Bae) Interface for Analysis Tool Option
public:
	virtual BOOL		IsShowTrendLineText( void)		const = 0;
	virtual BOOL		IsShowHorizonTextLeft( void)	const = 0;
	virtual BOOL		IsShowHorizonTextRight( void)	const = 0;
	virtual BOOL		IsShowVerticalText( void)		const = 0;
	virtual BOOL		IsShowThreeText( void)			const = 0;
	virtual BOOL		IsShowFourText( void)			const = 0;
	virtual BOOL		IsShowFiboText( void)			const = 0;
	virtual BOOL		SetAnalysisToolTextFlag( BOOL p_bTrendLine, BOOL p_bHorizonLeft, BOOL p_bHorizonRight, BOOL p_bVerticalLine, BOOL p_bThreeLine, BOOL p_bFourLine, BOOL p_bFiboLine) = 0;

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
	// (2005.01.13, 배승원) 차트의 전체보기 Mode 여부를 확인 및 설정하는 Interface를 제공한다.
	virtual BOOL	CheckFullView( void) const		= 0;
	virtual void	SetFullView( BOOL p_bFullView)	= 0;
	// (2006/2/6 - Seung-Won, Bae) DragZoomIn for DragZoom AddIn
	virtual void	DragZoomIn( const POINT p_ptStart, const POINT p_ptEnd) = 0;
	// (2006/7/27 - Seung-Won, Bae) Reset Scroll
	virtual void	ResetScroll( BOOL p_bWithCalcDRI = FALSE) = 0;
	// (2006/8/16 - Seung-Won, Bae) Right Virtual Candle
	virtual int		GetBongCountMargin( void) = 0;
	virtual BOOL	SetBongCountMargin( const int p_nCount) = 0;

// (2004.10.11, 배승원) 공유 관련 Interface
public:
	// [04/10/11] 주어진 Global 공유가 설정되어 있는지 확인하는 Interface를 제공한다.
	virtual BOOL		CheckGSharedSetting( const char *p_szGSharedName, BOOL p_bStart, BOOL p_bGet, BOOL p_bSet) = 0;
	// [04/10/11] 주어진 Global 공유에 공유를 발생시키는 Interface를 제공한다.
	virtual BOOL		FireGSharedData( const char *p_szGSharedName, const char *p_szGSharedData) = 0;
	// (2006/7/25 - Seung-Won, Bae) Set Local Shared Info with manual.
	virtual BOOL		AddLocalShareInfo( const char *p_szLSharedName, int p_nSettingType, const char *p_szLocalSharedData) = 0;
	// (2006/7/25 - Seung-Won, Bae) Get Local Shared Data.
	virtual IString *	GetLocalSharedDataString( const char *p_szLSharedName) = 0;
	// (2006/7/26 - Seung-Won, Bae) Remove Local Shared Info with manual.
	virtual BOOL		RemoveLocalShareInfo( const char *p_szLSharedName) = 0;

// (2004.10.19, 배승원) Image 저장 및 출력을 위한 Drawing Interface를 제공한다.
public:
	virtual void	DrawForImage( HDC p_hDC, RECT p_rctSource, BOOL p_bUseScreenColor) = 0;

// 종찬(05/06/01) 그래프의 추가, 삭제, 변경 관련 Interface를 제공한다.
public:
	virtual	IString	*	AddChartBlock( const char *p_szIndicatorName) = 0;
	virtual void		AddPacket(const char* pcszPacketOption) = 0;
	virtual void		AddGraph(short nRow, short nColumn, short nVertScaleGroup, const char* szGraphOption, BOOL bCreateBlock, BOOL bCreateVertScaleGroup, BOOL bUseAutoScale) = 0;
	virtual bool		ChangeSubGraphData(const char* pcszGraphName, const char* pcszSubGraphName, COLORREF clrLineColor, int nThickness, int nDrawStyle) = 0;
	virtual bool		SetStartPeriodOfPacket(const char* pcszPacketName, long lStartPeriod, BOOL bAddSpace) = 0;
	virtual bool		SetStartIndexOfPacket(const char* pcszPacketName, int nStartDataIndex, BOOL bAddSpace) = 0;
	virtual bool		HasGraph(const char* pcszGraphName) = 0;
	virtual BOOL		HasGraphInSpecialBack( const char *p_szGraphName) = 0;
	virtual BOOL		RemoveChart( const char *p_szIndicator) = 0;
	virtual bool		RemoveGraph(const char* pcszGraphName) = 0;
	virtual bool		RemovePacket(const char* pcszPacketName, const bool bIsOnlyDataClear) = 0;
	// (2009/4/13 - Seung-Won, Bae) for ChartList Handling
	virtual IString *	GetChartList( void) = 0;
	virtual void		SetChartList( const char *p_szChartList) = 0;

// (2006/1/21 - Seung-Won, Bae) Operations
public:
	// 종찬(05/08/22) AddIn으로 부터 리얼을 받을수 있는 Interface를 제공한다.
	// nOption = 1:Update, 2:Append
	virtual void		SetRealData(const char* pcszPacketName, double dData, short nOption) =0;
	// (2006/1/23 - Seung-Won, Bae) Set Logging Flag
	virtual void		SetLoggingFlag( const BOOL p_bInLogging) = 0;
	// (2006/1/28 - Seung-Won, Bae) Reset Tool UI
	virtual void		ReleaseOtherToolUI( const CToolOptionInfo::TOOLOPTION p_eToolOption) = 0;

// (2006/1/17 - Seung-Won, Bae) Notify from AddIn
public:
	// (2005/8/18 - Seung-Won, Bae) for Event from AddIn
	virtual void		FireAddInEvent( const char *p_szAddInName, short p_nEventID, const char *p_szData, double p_dData, long p_lData) = 0;
	// [04/10/11] DLL Container에게 Message하는 Interface를 제공한다.
	virtual LRESULT		SendEventMsgToDLL( UINT message, WPARAM wParam, LPARAM lParam) = 0;
	// (2006/11/15 - Seung-Won, Bae) Notify Tool State Changing to MChart an ToolBar
	virtual BOOL		OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn) = 0;
	// (2007/1/5 - Seung-Won, Bae) Excute Tool Command
	virtual BOOL		RunToolCommand( const CToolOptionInfo::TOOLOPTION p_eToolOption, const int p_nParam) = 0;
	// (2007/2/22 - Seung-Won, Bae) User Zoom Changed.
	virtual void		OnUserZoomChanged( const int p_nDataCountInView) = 0;

// (2006/3/23 - Seung-Won, Bae) Set/Get StyleData
public:	// Get
	virtual	COLORREF	GetStyleValueGraphBGColor(		void) = 0;
	virtual	COLORREF	GetStyleValueGraphBorderColor(	void) = 0;
	virtual	COLORREF	GetStyleValueGraphGridColor(	void) = 0;
	virtual	int			GetStyleValueGridHeight(		void) = 0;
	virtual	int			GetStyleValueGridWidth(			void) = 0;
	virtual	COLORREF	GetStyleValueGridHLineColor(	void) = 0;
	virtual	COLORREF	GetStyleValueGridVLineColor(	void) = 0;
	virtual	COLORREF	GetStyleValueScaleTextColor(	void) = 0;
	virtual	COLORREF	GetStyleValueScaleBGColor(		void) = 0;
	virtual	COLORREF	GetStyleValueScaleBorderColor(	void) = 0;
	virtual	COLORREF	GetStyleValueAnalysisToolColor( void) = 0;
	virtual	COLORREF	GetStyleValueNIDLGColor(		void) = 0;
public:	// SetGraphRegionColor
	virtual	BOOL		SetStyleValueGraphBGColor(		COLORREF	p_clrGraphBG,		BOOL p_bWithUpdate = FALSE)	= 0;
	virtual	BOOL		SetStyleValueGraphBorderColor(	COLORREF	p_clrGraphBorder,	BOOL p_bWithUpdate = FALSE)	= 0;
	virtual	BOOL		SetStyleValueGraphGridColor(	COLORREF	p_clrGraphGrid,		BOOL p_bWithUpdate = FALSE)	= 0;
public:	// SetGraphBackgroundGap
	virtual	BOOL		SetStyleValueGridHeight(		int			p_nGridHeight,		BOOL p_bWithUpdate = FALSE)	= 0;
	virtual	BOOL		SetStyleValueGridWidth(			int			p_nGridWidth,		BOOL p_bWithUpdate = FALSE)	= 0;
public:	// SetScaleColor
	virtual	BOOL		SetStyleValueGridHLineColor(	COLORREF	p_clrGridHLine,		BOOL p_bWithUpdate = FALSE)	= 0;
	virtual	BOOL		SetStyleValueGridVLineColor(	COLORREF	p_clrGridVLine,		BOOL p_bWithUpdate = FALSE)	= 0;
	virtual	BOOL		SetStyleValueScaleTextColor(	COLORREF	p_clrScaleText,		BOOL p_bWithUpdate = FALSE)	= 0;
public:	// Updated in OnDraw
	virtual	BOOL		SetStyleValueScaleBGColor(		COLORREF	p_clrScaleBG)		= 0;
	virtual	BOOL		SetStyleValueScaleBorderColor(	COLORREF	p_clrScaleBorder)	= 0;
public:	// Updated in EAI_ALL_APPLY_STYLE
	virtual	BOOL		SetStyleValueAnalysisToolColor(	COLORREF	p_clrAnalysisTool)	= 0;
	virtual	BOOL		SetStyleValueNIDLGColor(		COLORREF	p_clrNIDL)			= 0;

// (2006/8/16 - Seung-Won, Bae) Set/Get the Graph Title Option
public:
	virtual BOOL	GetGraphTitleOption( BOOL &p_bWithConditionValue, BOOL &p_bWithLastValue)	= 0;
	virtual BOOL	SetGraphTitleOption( BOOL p_bWithConditionValue, BOOL p_bWithLastValue)		= 0;

// (2006/8/15 - Seung-Won, Bae) with SaveChartCFG
public:
	virtual BOOL	SaveChartCFG_RemoveFromIndexToEnd( const int p_nIndex)	= 0;
	virtual BOOL	SaveChartCFG_ResetWithCurrentStyle( void)				= 0;
	virtual BOOL	SaveChartCFG_RestoreChartOfIndex( const int p_nIndex)	= 0;
	virtual int		SaveChartCFG_SaveNow( void)								= 0;	// Saved CFG Index will be returned. ( Count - 1)

// (2007/1/10 - Seung-Won, Bae) Binary TR Setting Interface
public:
	virtual BOOL	SetBinTrHelpMsg( const char *p_szHelpMessage)											= 0;
	virtual BOOL	SetBinTrDoubleData( const char *p_szPacketName, long p_nCount, double FAR* p_pdData)	= 0;
	virtual BOOL	SetBinTrStringData( const char *p_szPacketName, long p_nCount, long p_lData)			= 0;
	virtual BOOL	SetBinTrEnd( void)																		= 0;

	virtual IString *	GetCurrentRQ( void) = 0;

// (2008/12/21 - Seung-Won, Bae) Manage On Real Processing
public:
	virtual void	SetOnRealProcessing( BOOL p_bOn) = 0;

// (2009/5/13 - Seung-Won, Bae) for Right Margin
public:
	virtual void	SetRightMarginWidth( int nRightMarginWidth) = 0;
	virtual int		GetRightMarginWidth( void) = 0;

// (2009/5/17 - Seung-Won, Bae) for Future Time
public:
	virtual void	SetFutureTimeCount( int nFutureTimeCount) = 0;
	virtual int		GetFutureTimeCount( void) = 0;

// (2009/5/19 - Seung-Won, Bae) for Auto Resize of Print and Image Capture
public:
	virtual void	LockScrollUIUpdateWithScrollInfo( BOOL p_bLockScrollUIUpdate) = 0;

// (2009/9/1 - Seung-Won, Bae) Define Chart Mode.
public:
	virtual CChartInfo::CHART_MODE		GetChartMode( void) = 0;
};

#endif // !defined(AFX_ICHARTOCX_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_)
