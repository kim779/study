// IChartCtrl.h: interface for the IChartCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICHARTCTRL_H__804FF249_4729_49B6_8913_7CEDC34A43B0__INCLUDED_)
#define AFX_ICHARTCTRL_H__804FF249_4729_49B6_8913_7CEDC34A43B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "BlockBaseEnum.h."		// for CToolOptionInfo

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/1/7
// Comments		: Interface of COleControl Class in Chart OCX
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

interface IPropertyMap;

class CCommonInterface;
class COutsideCommonInterface;
class CIndicatorList;
class CMainBlock;
class CPacketList;
class CShareInfoListHW;
class CStyleData;
class CAddInManager;
class CXScaleManager;
class CPacketListManager;	// PacketListManager : 복수종목 - ojtaso (20070110)
class CPacket;
class CIndicatorInfo;

interface IChartCtrl
{
protected:
	virtual ~IChartCtrl()	{}

public:
	virtual bool	AddOrDeleteChart(const CString& strChartName) = 0;
	virtual bool	AddOrDeleteChart_Genneral(const CString& strChartName) = 0;
	virtual bool	AddOrDeleteChart_Special(const CString& strChartName) = 0;
	virtual bool	AddRemoveGraph(LPCTSTR pcszGraphName) = 0;
	virtual bool	ApplySkin() = 0;
	virtual bool	CalculateIndicatorInTRData( CIndicatorInfo* pIndicatorInfo, const CList<CPacket*, CPacket*>& packetList) = 0;
	virtual bool	ChangeChartType(CString strGraphName, CString strType, CString strStyle, CString strPacketNameList) = 0;
	virtual bool	ChangeSubGraphData(const char* pcszGraphName, const char* pcszSubGraphName, COLORREF clrLineColor, int nThickness, int nDrawStyle) = 0;
	virtual bool	CreateUserDirectory() = 0;
	virtual bool	DeleteChart_Genneral(const CString& strChartName) = 0;
	virtual bool	GetAutoSave() = 0;
	virtual bool	GetGraphDataForMaking( const CString& strGraphOption, CString& strGraphDataForMaking) const = 0;
	virtual bool	GetImaginaryCandle() = 0;
	virtual bool	GetRequestTRPacketName(const CString& strPacketNames, CString& strPacketNamesForTR) const = 0;
	virtual bool	GetShowIndicatorValue() = 0;
	virtual bool	GetShowMinMaxPrice() = 0;
	virtual bool	HasGraphAdapter(const char* szGraphName) = 0;
	virtual bool	IsScrollInHorzScaleData() const = 0;
	virtual bool	IsUseToolScrollBar() = 0;
	virtual bool	IsWholeView() const = 0;
	virtual bool	OnReturnOrg2(const BOOL bIsMessageBox = TRUE) = 0;
	virtual bool	ResetSaveChartCfg(int nOption, bool bRegionRetain = false) = 0;
	virtual bool	RestoreChartCfg(const int nIndex) = 0;
	virtual bool	RollbackLastNoOneChart() = 0;
	virtual bool	RunToolFuncGraphColor(const COLORREF clrGraphColor) = 0;
	virtual bool	RunToolFuncGraphStyle(const int nParam) = 0;
	virtual bool	RunToolFuncLineThickness(const int nParam) = 0;
	virtual bool	RunToolFunction( const CToolOptionInfo::TOOLOPTION p_eToolOption, const int p_nParam) = 0;	// 툴기능실행/해제
	virtual bool	SaveChartCfg() = 0;
	virtual bool	SaveChartCfgInCurrentOCX() = 0;
	virtual bool	SetPacketDataInPacketList(LPCTSTR szPacketName, long szData, long lLength, BOOL bReset) = 0;
	virtual bool	SetStartIndexOfPacket(const char* pcszRQ, const char* pcszPacketName, int nStartDataIndex, BOOL bAddSpace) = 0;
	virtual bool	SetStartPeriodOfPacketAdapter(const char* szPacketName, long lStartPeriod, BOOL bAddSpace) = 0;

public:
	virtual BOOL	AddChartBlock( LPCTSTR p_strIndicatorName, CString &p_strGraphName) = 0;
	virtual BOOL	AddLocalShareInfo( const char *p_szLSharedName, int p_nSettingType, const char *p_szLocalSharedData) = 0;
	virtual BOOL	AddOrRemoveChart(LPCTSTR strGraphName) = 0;
	virtual BOOL	CanGraphMoveAndDelete( void) = 0;	// (2009/6/5 - Seung-Won, Bae) Check mode for moving and deleting the graph.
	virtual BOOL	ChangeChartItem(LPCTSTR strGraphName, short nSubGraphIndex, LPCTSTR strNewTitle, LPCTSTR strNewPacketName, LPCTSTR strNewGraphType, LPCTSTR strNewGraphStyle, short nNewUpLineWidth, short nNewDownLineWidth, short nNewEqualLineWidth, BOOL bRunatOnce) = 0;
	virtual BOOL	ChangeGraphDrawingData(LPCTSTR p_szGraphName, BOOL p_bLowHigh, BOOL p_bLowCur, BOOL p_bHighCur, BOOL p_bPrevCur, short p_nCurData) = 0;
	virtual BOOL	ChangeGraphTitle(short nRowIndex, short nColumnIndex, short nGraphIndex, LPCTSTR strNewTitle) = 0;
	virtual BOOL	ChangeGraphTitle2(LPCTSTR szTargetTitle, LPCTSTR szSourceTitle) = 0;
	virtual BOOL	ChangeGraphType(LPCTSTR strGraphName, LPCTSTR strGraphType, LPCTSTR strGraphStyle, LPCTSTR strPacketNameList, BOOL bRunatOnce) = 0;
	virtual BOOL	ChangeHorzScalePosition(short nScalePosition, short nRowIndex, short nColumnIndex) = 0;
	virtual BOOL	ChangeIndicatorPacketName( LPCTSTR strIndicatorName, short nSubGraphIndex, LPCTSTR strNewPacketName) = 0;
	virtual BOOL	ChangeOneChart( const CString &p_strIndicatorName, CString &p_strGraphName) = 0;
	virtual BOOL	ChangePacketType(LPCTSTR p_szPacketName, LPCTSTR p_szPacketType) = 0;
	virtual BOOL	ChangePriceGraphDrawingData(BOOL p_bLowHigh, BOOL p_bLowCur, BOOL p_bHighCur, BOOL p_bPrevCur, short p_nCurData, short p_nDefaultType) = 0;
	virtual BOOL	ChangeVertScalePosition(short nScalePosition, short nRowIndex, short nColumnIndex, short nVSGroupIndex) = 0;
	virtual BOOL	CheckGSharedSetting( const char *p_szGSharedName, BOOL p_bStart, BOOL p_bGet, BOOL p_bSet) = 0;
	virtual BOOL	ClearPacketData(LPCTSTR strPacketName, BOOL bIsAbsoluteCreal) = 0;
	virtual BOOL	DestroyWindow() = 0;
	virtual BOOL	FireGSharedData( const char *p_szGSharedName, const char *p_szGSharedData) = 0;
	virtual BOOL	FireSharedData( const char *p_szShareName, const char *p_szSharedData) = 0;
	virtual BOOL	GetAllProperties3( IPropertyMap *p_pIProperties, BOOL p_bWithFull) = 0;
	virtual BOOL	GetBoundMarkFlag() = 0;
	virtual BOOL	GetDataStartEnd( int &p_nStartIndex, int &p_nEndIndex) = 0;
	virtual BOOL	GetGraphTitleOption( BOOL &p_bWithConditionValue, BOOL &p_bWithLastValue) = 0;
	virtual BOOL	GetLogScale(LPCTSTR strGraphName) = 0;
	virtual BOOL	GetNormalizedMinuteTimeRule( void) = 0;
	virtual BOOL	GetScrollRange( int &p_nRange) = 0;
	virtual BOOL	GetShareInfoItem( const char *p_szShareName, int &p_nShareType, int &p_nShareArea, long &p_lCodeFilter) = 0;
	virtual BOOL	GetStartEndDateOnView(double FAR* p_pStartDate, double FAR* p_pEndDate) = 0;
	virtual BOOL	GetTestEnvValue( const char *p_szEnvKey, CString &p_strEnvValue) = 0;	// (2008/5/13 - Seung-Won, Bae) Chart TEST ENV for Runtime.
	virtual BOOL	HasGraph(LPCTSTR p_szGraphName) = 0;
	virtual BOOL	HasGraphInSpecialBack(LPCTSTR p_szGraphName) = 0;
	virtual BOOL	HasToolItem( CToolOptionInfo::TOOLOPTION p_eToolItem) = 0;
	virtual BOOL	InvokeAddIn(LPCTSTR p_szAddInDllName, short p_nCommandType, long p_lData) = 0;
	virtual BOOL	InvokeAddInStr(LPCTSTR p_szAddInDllName, LPCTSTR p_szCommandName, LPCTSTR p_szData) = 0;
	virtual BOOL	IsEnableIndicatorCalculation( void) = 0;
	virtual BOOL	IsGraphMenuShow() const = 0;
	virtual BOOL	IsHideAllTitle( void) = 0;
	virtual BOOL	IsInLogging( void) = 0;
	virtual BOOL	IsInUseRealData( void) = 0;
	virtual BOOL	IsMultipleSameNameIndicator( void) = 0;			// (2007/1/20 - Seung-Won, Bae) Support Multiple Same-Name Indicator.
	virtual BOOL	IsOcxAreaNumericalInquiry( void) const = 0;
	virtual BOOL	IsOnCaptionFlagOfNIDLG( void) const = 0;	// (2006/3/29 - Seung-Won, Bae) Dynamic Caption Setting for each Site.
	// (2009/11/6 - Seung-Won, Bae) Do not disable zoom and scroll on OneChart
	virtual BOOL	IsRunningNoZoomChart( void) const = 0;
	virtual BOOL	IsRunningOneChart() const = 0;
	virtual BOOL	IsRunTimeMode() const = 0;
	virtual BOOL	IsShowPriceRateInquiry( void) const = 0;
	virtual BOOL	IsShowHorizonTextLeft( void)	const = 0;
	virtual BOOL	IsShowHorizonTextRight( void)	const = 0;
	virtual BOOL	IsShowFiboText( void)			const = 0;
	virtual BOOL	IsShowFourText( void)			const = 0;
	virtual BOOL	IsShowPriceMinMaxText( void) = 0;
	virtual BOOL	IsShowThreeText( void)			const = 0;
	virtual BOOL	IsShowTrendLineText( void)		const = 0;
	virtual BOOL	IsShowVerticalText( void)		const = 0;
	virtual BOOL	IsThisPacketConnected( const int nIndex, const CString strpacketname) = 0;
	virtual BOOL	IsUseInquiryData() const = 0;
	virtual BOOL	JoinAllGraph(BOOL bIsOneVertScaleGroup) = 0;
	virtual BOOL	JoinandDevisionforCompareChart(long nRowCount, long nColumnCount, BOOL bJoin) = 0;
	virtual BOOL	LoadAddIn(LPCTSTR p_szAddInDllName) = 0;
	virtual BOOL	OffRealAdvise() = 0;
	virtual BOOL	OnGraphAdded( const char *p_szRQName, const char *p_szGraphName, const char *p_szIndicatorName) = 0;
	virtual BOOL	OnGraphDeleted( const char *p_szRQName, const char *p_szGraphName, const char *p_szIndicatorName) = 0;
	virtual BOOL	OnRealAdvise() = 0;
	virtual BOOL	OnRunningOneChart() = 0;
	virtual BOOL	OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) = 0;
	virtual BOOL	OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn) = 0;	// (2006/11/15 - Seung-Won, Bae) Notify Tool State Changing to MChart an ToolBar
	virtual BOOL	PacketDataMaker() = 0;
	virtual BOOL	PartitionAllGraph() = 0;
	virtual BOOL	ReadUserFile(const CString& strFileName) = 0;
	virtual BOOL	RemoveChart( const char *p_szIndicator) = 0;
	virtual BOOL	RemoveGraph( LPCTSTR szGraphName) = 0;
	virtual BOOL	RemoveGraph_Title(short nRowIndex, short nColumnIndex, LPCTSTR szGraphTitle, BOOL bIsOnlyNoTRData, BOOL bIsAutoHorzScalePosition) = 0;
	virtual BOOL	RemoveLocalShareInfo( const char *p_szLSharedName) = 0;
	virtual BOOL	RemovePacket(LPCTSTR szPacketName) = 0;
	virtual BOOL	RemoveSetCapture( void) = 0;
	virtual BOOL	ResetChart() = 0;
	virtual BOOL	ResetPropertiesfromMainBlock() = 0;
	virtual BOOL	ResetRealListString(LPCTSTR p_szRealList, BOOL p_bAdviseNow) = 0;
	virtual BOOL	RestoreMin(LPCTSTR strPacketName) = 0;
	virtual BOOL	RestoreMinMax(LPCTSTR strPacketName) = 0;
	virtual BOOL	RestorMax(LPCTSTR strPacketName) = 0;
	virtual BOOL	SaveChartCFG_RemoveFromIndexToEnd( int p_nIndex) = 0;
	virtual BOOL	SaveChartCFG_ResetWithCurrentStyle( void) = 0;
	virtual BOOL	ScrollTo( int p_nScrollValue, BOOL p_bRelative) = 0;
	virtual BOOL	ScrollToDataOfEnd(double p_dData) = 0;
	virtual BOOL	SetAllProperties2(long dwData) = 0;
	virtual BOOL	SetAllProperties3( IPropertyMap *p_pIProperties, BOOL p_bReloadAddIn = TRUE) = 0;
	virtual BOOL	SetAllPropertiesFromFile(LPCTSTR strFilePath, LPCTSTR strFileName) = 0;
	virtual	BOOL	SetAllHorzScaleGridType( int p_nGridVLineType)	= 0;
	virtual	BOOL	SetAllVertScaleGridType( int p_nGridVLineType)	= 0;
	virtual BOOL	SetAnalysisToolColor( COLORREF p_clrColor) = 0;
	virtual BOOL	SetAnalysisToolTextFlag( BOOL p_bTrendLine, BOOL p_bHLineLeft, BOOL p_bHLineRight, BOOL p_bVLine,
							BOOL p_bTreeLine, BOOL p_bFourLine, BOOL p_bFiboLine) = 0;
	virtual BOOL	SetBinTrHelpMsg( const char *p_szHelpMessage)											= 0; // (2007/1/10 - Seung-Won, Bae) Binary TR Setting Interface
	virtual BOOL	SetBinTrDoubleData( const char *p_szPacketName, long p_nCount, double FAR* p_pdData)	= 0;
	virtual BOOL	SetBinTrStringData( const char *p_szPacketName, long p_nCount, long p_lData)			= 0;
	virtual BOOL	SetBinTrEnd( void)																		= 0;
	virtual BOOL	SetDrdsAdvise(BOOL bOnAdvise) = 0;
	virtual BOOL	SetGraphHide2(short nRowIndex, short nColumnIndex, short nVerticalIndex, short nGraphIndex, short nSubGraphIndex, BOOL bHide) = 0;
	virtual BOOL	SetGraphHide3(LPCTSTR strTitle, BOOL bIsHide) = 0;
	virtual BOOL	SetGraphTitle(LPCTSTR strGraphName, short nSubGraphIndex, LPCTSTR strNewGraphTitle, BOOL bGraphShowing) = 0;
	virtual BOOL	SetGraphTitleOption( BOOL p_bWithConditionValue, BOOL p_bWithLastValue) = 0;
	virtual BOOL	SetHelpMessageRealDataRate(LPCTSTR strHelpMessageData, BOOL bIsRemove) = 0;
	virtual BOOL	SetHideAllTitle( BOOL p_bHideAllTitle) = 0;
	virtual BOOL	SetHorzScaleData(short nRowIndex, short nColumnIndex, LPCTSTR strScaleData) = 0;
	virtual BOOL	SetIndicatorCondition(LPCTSTR strGraphName, short nConditionType, short nConditionIndex, double dConditionValue, BOOL bReCalculate) = 0;
	virtual BOOL	SetLogScale(LPCTSTR strGraphName, BOOL bLogScale) = 0;
	virtual BOOL	SetMax(LPCTSTR strPacketName, double dMax) = 0;
	virtual BOOL	SetMin(LPCTSTR strPacketName, double dMin) = 0;
	virtual BOOL	SetMinMax(LPCTSTR strPacketName, double dMin, double dMax) = 0;
	virtual BOOL	SetNormalizedMinuteTimeRule( BOOL p_bEnable) = 0;
	virtual BOOL	SetNumericalInquiryOption( UINT p_uOption) = 0;
	virtual BOOL	SetPacketData(LPCTSTR szPacketName, long szData, long lLength, BOOL bReset) = 0;
	virtual BOOL	SetPacketData2(LPCTSTR strPacketName, LPCTSTR strPacketType, LPCTSTR strPacketData, short nDataCount, BOOL bReCalculate) = 0;
	virtual BOOL	SetPacketDataInAdditionPacket(LPCTSTR szPacketName, long szData, long lLength, BOOL bReset) = 0;
	virtual BOOL	SetPacketHelpMessage(LPCTSTR strHelpMsg, BOOL bIsAllRemove) = 0;
	virtual BOOL	SetSharedData(short p_nShareType, LPCTSTR p_szSharedName, LPCTSTR p_szData) = 0;
	virtual BOOL	SetStartEndDateOfView(double p_dStartDate, double p_dEndDate) = 0;
	virtual BOOL	SetStartEndIndex(const int nStartIndex, const int nEndIndex) = 0;
	virtual BOOL	SetStartPeriodOfPacket(LPCTSTR p_szPacketName, long p_lStartPeriod, BOOL p_bAddSpace) = 0;
	// (2006/8/18 - Seung-Won, Bae) Style Data Setting Interfaces
	virtual	BOOL	SetStyleValueAnalysisToolColor(	COLORREF	p_clrAnalysisTool)	= 0;
	virtual	BOOL	SetStyleValueGraphBGColor(		COLORREF	p_clrGraphBG,		BOOL p_bWithUpdate = FALSE)	= 0;
	virtual	BOOL	SetStyleValueGraphBorderColor(	COLORREF	p_clrGraphBorder,	BOOL p_bWithUpdate = FALSE)	= 0;
	virtual	BOOL	SetStyleValueGraphGridColor(	COLORREF	p_clrGraphGrid,		BOOL p_bWithUpdate = FALSE)	= 0;
	virtual	BOOL	SetStyleValueGridHeight(		int			p_nGridHeight,		BOOL p_bWithUpdate = FALSE)	= 0;
	virtual	BOOL	SetStyleValueGridHLineColor(	COLORREF	p_clrGridHLine,		BOOL p_bWithUpdate = FALSE)	= 0;
	virtual	BOOL	SetStyleValueGridVLineColor(	COLORREF	p_clrGridVLine,		BOOL p_bWithUpdate = FALSE)	= 0;
	virtual	BOOL	SetStyleValueGridWidth(			int			p_nGridWidth,		BOOL p_bWithUpdate = FALSE)	= 0;
	virtual	BOOL	SetStyleValueNIDLGColor(		COLORREF	p_clrNIDLG)			= 0;
	virtual	BOOL	SetStyleValueScaleBGColor(		COLORREF	p_clrScaleBG)		= 0;
	virtual	BOOL	SetStyleValueScaleBorderColor(	COLORREF	p_clrScaleBorder)	= 0;
	virtual	BOOL	SetStyleValueScaleTextColor(	COLORREF	p_clrScaleText,		BOOL p_bWithUpdate = FALSE)	= 0;

	virtual BOOL	SetVertScaleData(short nRowIndex, short nColumnIndex, short nVertScaleGroupIndex, LPCTSTR strVertScaleData) = 0;
	virtual BOOL	ShowPriceMinMaxText( BOOL p_bShowPriceMinMaxText) = 0;
	virtual BOOL	UnloadAddIn(LPCTSTR p_szAddInDllName) = 0;
	virtual BOOL	ZoomTo( int p_nZoomValue, BOOL p_bRelative) = 0;
	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	virtual BOOL	SetDrawBaseLine(BOOL bDrawBaseLine) = 0;
	virtual bool	GetDrawBaseLine() = 0;

public:
	virtual BSTR	GetCustomFont() = 0;
	virtual BSTR	GetData(LPCTSTR strPacketName, short nDataIndex) = 0;
	virtual BSTR	GetGraphPacketName(short nRowIndex, short nColumnIndex, LPCTSTR szGraphTitle) = 0;
	virtual BSTR	GetIndicatorNameList( LPCTSTR p_strGroup) = 0;
	virtual BSTR	GetPacketOption() = 0;
	virtual BSTR	GetRealSettingInfo() = 0;
	virtual BSTR	GetStartDateOfData() = 0;
	virtual BSTR	GetStrBlockRegion() = 0;

public:
	virtual const char *GetBaseDeskPath( void) = 0;
	virtual const char *GetChartList( void) = 0;
	virtual const char *GetCurrentRQ( void) = 0;
	virtual const char *GetLocalSharedData( const char *p_szLSharedName) = 0;
	virtual const char *GetOCXPath( void) = 0;
	virtual const char *GetPacketInfoStr( void) = 0;
	virtual const char *GetUserFilePath( void) = 0;

public:
	virtual CAddInManager *				GetAddInManager( void) = 0;
	virtual CCommonInterface *			GetCommonInterface( void) = 0;
	virtual CMainBlock *				GetMainBlock(void) = 0;
	virtual COutsideCommonInterface *	GetMChartInterface( void) = 0;
	virtual CIndicatorList *			GetIndicatorList( void) = 0;
	virtual CPacketList *				GetPacketList( void) = 0;
	// PacketListManager : 복수종목 - ojtaso (20070110)
	virtual CPacketListManager *		GetPacketListManager( void) = 0;
	virtual CXScaleManager *			GetXScaleManager( void) = 0;
	virtual CChartInfo::CHART_MODE		GetChartMode( void) = 0;	// (2009/9/1 - Seung-Won, Bae) Define Chart Mode.

public:
	virtual CFont *	GetChartFont( void) = 0;
	virtual CFont *	GetRscFont() const = 0;

public:
	virtual COLORREF	GetClrGraphBase1() = 0;
	virtual COLORREF	GetClrGraphBase2() = 0;
	virtual COLORREF	GetClrGraphBase3() = 0;
	virtual COLORREF	GetClrGraphBase4() = 0;
	virtual COLORREF	GetClrGraphBase5() = 0;
	virtual COLORREF	GetClrGraphBase6() = 0;
	// (2006/8/18 - Seung-Won, Bae) Style Data Setting Interfaces
	virtual	COLORREF	GetStyleValueAnalysisToolColor( void)	= 0;
	virtual	COLORREF	GetStyleValueGraphBGColor( void)		= 0;
	virtual	COLORREF	GetStyleValueGraphBorderColor( void)	= 0;
	virtual	COLORREF	GetStyleValueGraphGridColor( void)		= 0;
	virtual	COLORREF	GetStyleValueGridHeight( void)			= 0;
	virtual	COLORREF	GetStyleValueGridHLineColor( void)		= 0;
	virtual	COLORREF	GetStyleValueGridVLineColor( void)		= 0;
	virtual	COLORREF	GetStyleValueGridWidth( void)			= 0;
	virtual	COLORREF	GetStyleValueNIDLGColor( void)			= 0;
	virtual	COLORREF	GetStyleValueScaleBGColor( void)		= 0;
	virtual	COLORREF	GetStyleValueScaleBorderColor( void)	= 0;
	virtual	COLORREF	GetStyleValueScaleTextColor( void)		= 0;
	virtual COLORREF	GetToolTipBackColor( void) = 0;

public:
	virtual CRect	GetAllBlockRegion( void) = 0;
	virtual CRect	GetMainBlockRegion() = 0;
	virtual CRect	GetOcxRegion() const = 0;
	virtual CRect	GetToolBarRegion() const = 0;
	virtual CRect	GetVirtualRegion() = 0;

public:
	virtual CWnd *	AddSetCapture( void) = 0;
	virtual CWnd *	GetMainWnd( void) = 0;
	virtual CWnd *	GetChartWindowPointer() const = 0;

public:
	virtual double	ConvertYPositionToData(const int nYPosition, const int nYPositionTop, const int nYPositionBottom,const double& dDataMax, const double& dDataMin, const bool bLog, const bool bReverse) = 0;
	virtual double	GetMax(LPCTSTR strPacketName) = 0;
	virtual double	GetMin(LPCTSTR strPacketName) = 0;

public:
	virtual HCURSOR	GetCursorHandle( void) = 0;
	virtual HCURSOR	SetCursorHandle( HCURSOR p_hNewCursor) = 0;

public:
	virtual HWND	GetBaseDeskHWnd() = 0;
	virtual HWND	GetSafeHwnd( void) const = 0;
	virtual HWND	GetWndHandle(void) const = 0;

public:
	virtual int	ConvertDataToYPosition( double p_dData, double p_dDataMax, double p_dDataMin, int p_nYPositionTop, int p_nYPositionBottom, BOOL p_bLog, BOOL p_bReverse) = 0;
	virtual int GetAddInSiteID( void) = 0;
	virtual int	GetBongCountMargin() = 0;
	virtual int	GetDataCountForToolBar() const = 0;
	virtual	int	GetFirstBlockHGridLineType( void) = 0;
	virtual int	GetFirstBlockVGridLineType( void) = 0;
	virtual int	GetFullDataCount(const bool bIsHorzScaleDataCheck) const = 0;
	virtual int	GetFutureTimeCount( void) = 0;		// (2009/5/17 - Seung-Won, Bae) for Future Time
	virtual int	GetMarginInIndicatorInfo(const CString& strGraphName) const = 0;
	virtual int	GetMaxDataCount() const = 0;
	virtual int	GetMaxRange() = 0;
	virtual int	GetMinCountPerPage() const = 0;
	virtual int	GetOnePacketSize() = 0;
	virtual int	GetOnePageDataCountForScroll() const = 0;
	// X-Scale 다음 표시 가능 갯수 구하기 (yiilyoul@magicn.com - 20070126)
	virtual int	GetPageCountNextStep(const int nSliderPos_Prev, const int nSliderPos_New, const UINT nSBCode, int nMin, int nMax) = 0;
	virtual int	GetRightMarginWidth( void) = 0;		// (2009/5/13 - Seung-Won, Bae) for Right Margin
	virtual int	GetShowTrendLineValue() = 0;
	virtual int	GetSkinNumber() const = 0;
	virtual int	GetSliderMin( void) = 0;
	virtual int	GetUserMargin(void) const = 0;
	virtual int	OnCreate(LPCREATESTRUCT lpCreateStruct) = 0;
	virtual int SaveChartCFG_SaveNow( void) = 0;
	virtual int	SetChartData2(long pChartData, int nCount, int nType) = 0;
	virtual int	ShowIndicatorConfigDlg( CIndicatorInfo *p_pIndicatorInfo) = 0;
	virtual int	GetMouseWheelIndex( void) = 0;
	virtual int	GetPriceMinMaxShowType( void) = 0;

public:
	virtual long	GetAllProperties2(long lpData) = 0;
	virtual long	GetBase(LPCTSTR szKey) = 0;
	virtual long	GetBasePropIf() = 0;
	virtual long	GetDrawingLoadTime( void) = 0;								// (2007/3/4 - Seung-Won, Bae) Make Thread for the 'CLUTCH' of Real Update.
	virtual long	GetOnePageDataCount() = 0;
	virtual long	GetStartPeriodOfPacket(LPCTSTR p_szPacketName) = 0;

public:
	virtual LPCTSTR		GetMasterValueRQ( const int nRow, const int nCol) = 0;

public:
	virtual LPUNKNOWN	GetCtrlUnkownID(LPCTSTR szName) = 0;
	virtual LPUNKNOWN	GetDeskUnkownID() = 0;
	virtual LPUNKNOWN	GetAddInCustomInterface( LPCTSTR p_szAddInName) = 0;

public:
	virtual LRESULT	OnCursorExchange(WPARAM wParam, LPARAM lParam) = 0;
	virtual LRESULT	OnGetHWDesignUI( WPARAM wParam, LPARAM lParam) = 0;
	virtual LRESULT	OnGetRealTableManager( WPARAM wParam, LPARAM lParam) = 0;
	virtual LRESULT	OnGetUserMargin(WPARAM wParam, LPARAM lParam) = 0;
	virtual LRESULT	OnGraphSelectedIndex(WPARAM wParam, LPARAM lParam) = 0;
	virtual LRESULT	OnRealData( WPARAM wParam, LPARAM lParam) = 0;
	virtual LRESULT	OnSetEtc(WPARAM wParam, LPARAM lParam) = 0;
	virtual LRESULT	RmsgFormStateChange( WPARAM wParam, LPARAM lParam) = 0;
	virtual LRESULT	SendEventMsgToDLL( UINT message, WPARAM wParam = NULL, LPARAM lParam = NULL) = 0;
	virtual LRESULT	WindowProc(UINT message, WPARAM wParam, LPARAM lParam) = 0;

public:
	virtual short	ClearChartData(short nOption) = 0;
	virtual short	GetBongCountMargin( void) const = 0;
	virtual short	GetColumnBlockCount() = 0;
	virtual short	GetDataCount() = 0;
	virtual short	GetGraphRButtonType( void) const = 0;
	virtual short	GetRowBlockCount() = 0;
	virtual short	GetVertScalePosition(short p_nRowIndex, short p_nColumnIndex, short p_nVSGroupIndex) = 0;
	virtual short	SetChartData(long pChartData, short nCount, short nType) = 0;
	virtual short	SetChartDataFromFile(LPCTSTR szFileName, short nType) = 0;
	virtual short	SetChartDataStr(LPCTSTR szChartData, short nCount, short nType) = 0;
	virtual short	SetGraphHide(short nRow, short nColumn, LPCTSTR strTitle, BOOL bHide) = 0;
	virtual short	testGetPacketdatasize(LPCTSTR pIn) = 0;
	virtual short	ZoomToWithRightFix(short p_nZoomValue, BOOL p_bWithRightFix) = 0;

public:
	virtual UINT	OnGetDlgCode() = 0;
	virtual UINT	GetNumericalInquiryOption( void) const = 0;

public:
	virtual void	AddandDeleteIndicator(LPCTSTR strGraphName) = 0;
	virtual void	AddGraph(short nRow, short nColumn, short nVertScaleGroup, LPCTSTR szGraphOption, BOOL bCreateBlock, BOOL bCreateVertScaleGroup, BOOL bUseAutoScale) = 0;
	virtual void	AddGraphAdapter(short nRow, short nColumn, short nVertScaleGroup, const char* szGraphOption, BOOL bCreateBlock, BOOL bCreateVertScaleGroup, BOOL bUseAutoScale) = 0;
	virtual void	AddPacket(LPCTSTR szPacketOption) = 0;
	virtual void	AddPacketAdapter(const char* pcszPacketOption) = 0;
	virtual void	AddRealItem(LPCTSTR szRealInfo) = 0;
	virtual void	BehaviorBlock(const long pBlockBehaviorData) = 0;
	virtual void	ClearTimeMark() = 0;
	virtual void	CreateBufferBitmap(CDC* pdc, const CRect& rcBounds) = 0;
	virtual void	CreateToolBar() = 0;
	virtual void	DeletedGraph( LPCTSTR lpszRQ, BOOL bRemovablePacketRQ) = 0;
	virtual void	DeleteZoomInOutInquiryDlg() = 0;
	virtual void	DragZoomIn( const CPoint &p_ptStart, const CPoint &p_ptEnd) = 0;	// (2006/2/6 - Seung-Won, Bae) DragZoomIn for DragZoom AddIn
	virtual void	EnableBoundMark(BOOL p_bEnable) = 0;
	virtual void	EnableReceivingTimeShare(BOOL p_bEnable) = 0;
	virtual void	FireAddInEvent(LPCTSTR p_szAddInName, short p_nEventID, LPCTSTR p_szData, double p_dData, long p_lData) = 0;
	virtual void	FireChangeOnePageDataCount(long nOnePageDataCount) = 0;
	virtual void	FireDataUpandDown(short nOption, LPCTSTR strStartDate, LPCTSTR strEndDate) = 0;
	virtual void	FireGetSettedChartInfoFromServer() = 0;
	virtual void	FireRequestBongData( short nOption, short nTemp) = 0;
	virtual void	FireRequestMapLoad(short nOption, LPCTSTR strCode, LPCTSTR strDate) = 0;
	virtual void	FireRequestNewPacket(short nOption, LPCTSTR strName, LPCTSTR strCode, LPCTSTR strDate) = 0;
	virtual void	FireRequestPacketData(LPCTSTR strDateType, LPCTSTR strStartDate, LPCTSTR strPacketNames) = 0;
	virtual void	FireRestore() = 0;
	virtual void	FireReturnOrgChart(long nOnePageDataCount) = 0;
	virtual void	FireSaveSettedChartInfoToServer() = 0;
	virtual void	FP_vSetInitFont() = 0;
	virtual void	FV_vInitFont() = 0;
	virtual void	GetDefaultPath( CString &p_strDefaultPath) const = 0;
	virtual void	GetImagePath( CString &p_strImagePath) const = 0;		// (2009/9/2 - Seung-Won, Bae) for Image Path
	virtual void	GetListViewerData(BSTR FAR* bstrTitle, BSTR FAR* bstrHorzScale, BSTR FAR* bstrPacketNamesPerBlock, BSTR FAR* bstrPacketNames, BSTR FAR* bstrSaveFilePath, BOOL FAR* bViewPacketSequence, long FAR* lpPacketList) = 0;
	virtual void	GetProgID( CString &p_strProgID) = 0;		// (2008/4/29 - Seung-Won, Bae) to use the ProgID in ChartMetaTable.dat
	virtual long	InvalidateControl( LPCRECT lpRect = NULL, BOOL bErase = TRUE) = 0;
	virtual void	InvalidateOCX(const bool bErase = true) = 0;
	virtual void	InvalidateOleControl(const bool bIsResetBlockRegion = false) = 0;
	virtual void	InvalidateRect(const CRect& region, const bool bErase) = 0;
	virtual void	LockWindowUpdate( void)		= 0;	// (2007/3/5 - Seung-Won, Bae) Lock/Unlock WindowUpdate
	virtual void	LockScrollUIUpdateWithScrollInfo( BOOL p_bLockScrollUIUpdate) = 0;	// (2009/5/19 - Seung-Won, Bae) for Auto Resize of Print and Image Capture
	virtual void	MakeSaveShortKey(LPCTSTR szFileName, short nFlag) = 0;
	virtual void	NeoAddInEvent( const char *p_szAddInName, short p_nEventID, const char *p_szData, double p_dData, long p_lData) = 0;
	virtual void	NeoFireDataUpandDown(int nOption) = 0;
	virtual void	NeoFireGetSettedChartInfoFromServer() = 0;
	virtual void	NeoFireSaveSettedChartInfoToServer() = 0;
	virtual void	NeoRequestBongData(int nOption, int nTemp) = 0;
	virtual void	NeoRequestMapLoad(int nOption, CString strCode, CString strDate) = 0;
	virtual void	NeoRequestNewPacket(int nOption, CString strName, CString strCode, CString strDate) = 0;
	virtual void	NeoRequestPacketData(const CString& strDateType, const CString& strStartDate, const CString& strPacketNames) = 0;
	virtual void	NeoRestore() = 0;
	virtual void	NeoReturnOrgChart(const int nOnePageDataCount) = 0;
	virtual void	NotifyRQStatusToAddin( CCommonInterfaceEnum::PACKET_RQ_STATUS eStatus, LPCTSTR lpszRQ) = 0;
	virtual void	OnbAddGraphMenuShowChanged() = 0;
	virtual void	OnBBlockMoveDeleteChanged() = 0;
	virtual void	OnBControlBlockSizeChanged() = 0;
	virtual void	OnBDoesUseSkinColorChanged() = 0;
	virtual void	OnBFireRealEventChanged() = 0;
	virtual void	OnbGraphDragDropChanged() = 0;
	virtual void	OnbHideChanged() = 0;
	virtual void	OnbIsInputChanged() = 0;
	virtual void	OnbNormalizedMinuteTimeRule() = 0;
	virtual void	OnBOcxAreaNumericalInquiryChanged() = 0;
	virtual void	OnBOnePageDataCountInTRCountChanged() = 0;
	virtual void	OnBSelectGraphChanged() = 0;
	virtual void	OnBSelectTitleChanged() = 0;
	virtual void	OnBShowPriceRateInquiryChanged() = 0;
	virtual void	OnBShowWarningMessageBoxChanged() = 0;
	virtual void	OnBUseConditiontoTitleChanged() = 0;
	virtual void	OnBUseDatatoTitleChanged() = 0;
	virtual void	OnBUseInquiryDlgWhenUseCrossLineChanged() = 0;
	virtual void	OnbUseRealDataChanged() = 0;
	virtual void	OnBUseScaleRButtonMenuChanged() = 0;
	virtual void	OnBUseShortKeyChanged() = 0;
	virtual void	OnbWholeViewChanged() = 0;
	virtual void	OnCaptureChanged(CWnd *pWnd) = 0;
	virtual void	OnCmdMsg( HMENU p_hMenu, UINT p_nItemID, int p_nItemPos, int p_nItemCount, int p_nCmdCode) = 0;		// (2009/6/5 - Seung-Won, Bae) for CmdUI of AddIn
	virtual void	OnDeleteBlock() = 0;
	virtual void	OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid) = 0;
	virtual void	OnDrawForPrint( CDC &memoryDC, CBitmap &bufferBitmap, CRect rcSource, BOOL bScreenColor = TRUE) = 0;
	virtual void	OnDrawStockItemCode( CDC &memoryDC, CRect &rect) = 0;
	virtual void	OnDrawUsingOptimizingcolor( CDC &memoryDC, CBitmap &bufferBitmap, CRect rect) = 0;
	virtual void	OnDrawUsingScreencolor( CDC &memoryDC, CBitmap &bufferBitmap, CRect rect) = 0;
	virtual void	OnFullMinMax(int nOption) = 0;
	virtual void	OnLButtonDown(UINT nFlags, CPoint point) = 0;
	virtual void	OnLButtonUp(UINT nFlags, CPoint point) = 0;
	virtual void	OnMenuCrossline() = 0;
	virtual void	OnMenuDatawindow() = 0;
	virtual void	OnnBlockBorderThicknessChanged() = 0;
	virtual void	OnnBlockColumnCountChanged() = 0;
	virtual void	OnnBlockCompartChanged() = 0;
	virtual void	OnnBlockGapChanged() = 0;
	virtual void	OnnBlockRowCountChanged() = 0;
	virtual void	OnnBlockTypeChanged() = 0;
	virtual void	OnNBongCountMarginChanged() = 0;
	virtual void	OnnBorderTypeChanged() = 0;
	virtual void	OnnBottomMarginChanged() = 0;
	virtual void	OnNColumnDoubleSizeBlockChanged() = 0;
	virtual void	OnNGraphRButtonTypeChanged() = 0;
	virtual void	OnNHoldingPacketSizeChanged() = 0;
	virtual void	OnNHorzGapChanged() = 0;
	virtual void	OnNInquiryTypeChanged() = 0;
	virtual void	OnnLeftMarginChanged() = 0;
	virtual void	OnNMarginBorderTypeChanged() = 0;
	virtual void	OnNMinDataCountOnOnePageChanged() = 0;
	virtual void	OnNNumericalInquiryOptionChanged() = 0;
	virtual void	OnnOnePageDataCountChanged() = 0;
	virtual void	OnNRButtonMenuTypeChanged() = 0;
	virtual void	OnnRightMarginChanged() = 0;
	virtual void	OnNRowDoubleSizeBlockChanged() = 0;
	virtual void	OnnTopMarginChanged() = 0;
	virtual void	OnNUseInquiryWithCrosslineChanged() = 0;
	virtual void	OnnUserBlockAddCountChanged() = 0;
	virtual void	OnNVertGapChanged() = 0;
	virtual void	OnPacketTRData() = 0;
	virtual void	OnPacketTRData(const CString& strTRPacketNames, const bool bIsResert) = 0;
	virtual void	OnPricechartMinmax() = 0;
	virtual void	OnPrint() = 0;
	virtual void	OnResetState() = 0;
	virtual void	OnReturnOrg() = 0;
	virtual void	OnReturnOrg(const bool bIsMessageBox) = 0;
	virtual void	OnReturnOrgDisplay() = 0;
	virtual void	OnReverseYscale() = 0;
	virtual void	OnScaleSettingMessage( const CCommonInterfaceEnum::ETCMESSAGETYPE eEtcMessageType, const int nData) = 0;
	virtual void	OnScreenMinmax() = 0;
	virtual void	OnSetFocus(CWnd* pOldWnd) = 0;
	virtual void	OnSize(UINT nType, int cx, int cy) = 0;
	virtual void	OnStrGlobalSharedListChanged() = 0;
	virtual void	OnstrLocalSharedListChanged() = 0;
	virtual void	OnStrMapNameChanged() = 0;
	virtual void	OnstrPacketOptionChanged() = 0;
	virtual void	OnstrRealListChanged() = 0;
	virtual void	OnStrShareInfoListHWChanged() = 0;
	virtual void	OnstrToolbarOptionChanged() = 0;
	virtual void	OnstrUserEnvironmentFileNameChanged() = 0;
	virtual void	OnstrUserIndicatorListFileNameChanged() = 0;
	virtual void	OnSzBasePacketNameChanged() = 0;
	virtual void	OnTimer(UINT nIDEvent) = 0;
	virtual void	OnToExcel() = 0;
	virtual void	OnUpdateMenuCrossline(CCmdUI* pCmdUI) = 0;
	virtual void	OnUpdateMenuDatawindow(CCmdUI* pCmdUI) = 0;
	virtual void	OnUpdatePricechartMinmax(CCmdUI* pCmdUI) = 0;
	virtual void	OnUpdateReverseYscale(CCmdUI* pCmdUI) = 0;
	virtual void	OnUpdateScreenMinmax(CCmdUI* pCmdUI) = 0;
	virtual void	OnUserZoomChanged( const int p_nDataCountInView) = 0;	// (2007/2/22 - Seung-Won, Bae) User Zoom Changed.
	virtual void	OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) = 0;
	// (2006/1/28 - Seung-Won, Bae) Reset Tool UI
	virtual void	ReleaseOtherToolUI( const CToolOptionInfo::TOOLOPTION p_eToolOption) = 0;
	virtual void	RemoveRealItem(LPCTSTR szRealInfo) = 0;
	virtual void	ReplaceMATitle(CString &szpricema, int nOption = 0) = 0;
	virtual void	RequestTRPacket(const CString& strGraphName, const CString& strPacketNames) = 0;
	virtual void	RequestTRPacket(const CString& strGraphName, const CString& strPacketNames, const int nCount, const bool bIsNew) = 0;
	virtual void	ResetChartCfgWith(short p_nIndex) = 0;
	virtual void	ResetScroll( BOOL p_bWithCalcDRI = FALSE) = 0;
	virtual void	RunEnvironmentDlg(short nOption) = 0;
	virtual void	SaveandApplyGraphStyle(short nSaveDataType, short nChangeDataType) = 0;
	virtual void	SelectedGraph( LPCTSTR lpszRQ, BOOL bSendEvent = TRUE) = 0;
	virtual void	SendTimeOfIndexToDLL( UINT p_uMessage, int p_nIndex) = 0;
	virtual void	SetAllLogScale(BOOL bLogScale) = 0;
	virtual void	SetAllProperties_Chart() = 0;
	virtual void	SetAutoSave(const bool bAutoSave, const bool bOutside = true) = 0;
	virtual void	SetBackgroundColors(OLE_COLOR p_rgbBlockArea, OLE_COLOR p_rgbScaleArea) = 0;
	virtual void	SetBaseDesk(long lKey) = 0;
	virtual void	SetBongCountMargin(short nBongCountMargin) = 0;
	virtual void	SetChartAllInfo() = 0;
	virtual void	SetChartList( const char *p_szChartList) = 0;
	virtual void	SetClrAnalysisTool(OLE_COLOR nNewValue) = 0;
	virtual void	SetClrBackBorder(OLE_COLOR nNewValue) = 0;
	virtual void	SetClrBackground(OLE_COLOR nNewValue) = 0;
	virtual void	SetClrBlock(OLE_COLOR nNewValue) = 0;
	virtual void	SetClrBlockBorder(OLE_COLOR nNewValue) = 0;
	virtual void	SetClrGraphBase1(OLE_COLOR nNewValue) = 0;
	virtual void	SetClrGraphBase2(OLE_COLOR nNewValue) = 0;
	virtual void	SetClrGraphBase3(OLE_COLOR nNewValue) = 0;
	virtual void	SetClrGraphBase4(OLE_COLOR nNewValue) = 0;
	virtual void	SetClrGraphBase5(OLE_COLOR nNewValue) = 0;
	virtual void	SetClrGraphBase6(OLE_COLOR nNewValue) = 0;
	virtual void	SetClrInquiryWindow(OLE_COLOR nNewValue) = 0;
	virtual void	SetClrLattice(OLE_COLOR nNewValue) = 0;
	virtual void	SetClrScaleLine(OLE_COLOR nNewValue) = 0;
	virtual void	SetClrScaleLine2(OLE_COLOR nNewValue) = 0;
	virtual void	SetClrScaleText(OLE_COLOR nNewValue) = 0;
	virtual void	SetContainerDllInfo(long p_lDllWnd, short p_nID) = 0;
	virtual void	SetCursorExchange(CCursorShape::CHARTOBJECT enumChartObject) = 0;
	virtual void	SetCustomFont(LPCTSTR lpszNewValue) = 0;
	virtual void	SetDataIndexByDate(LPCTSTR pcszStartDate, LPCTSTR pcszEndDate) = 0;
	virtual void	SetDataToMainBlock(const CString& strData) = 0;
	virtual void	SetDragModeType(short p_nDragModeType) = 0;
	virtual void	SetDrdsAdviseRQ(LPCTSTR lpszRQ, BOOL bOnAdvise) = 0;
	virtual void	SetFirstNMTRClock( void) = 0;						// (2009/12/11 - Seung-Won, Bae) Append lost NMTR data.
	virtual void	SetFutureTimeCount( int nFutureTimeCount) = 0;		// (2009/5/17 - Seung-Won, Bae) for Future Time
	virtual void	SetGraphSelectedDataIndex(const int nDataIndex, LPCTSTR pcszGraphName) = 0;
	virtual void	SetGraphThickness(const int nGraphThickness1) = 0;
	virtual void	SetGraphTypeStyle(const int nGraphType, const int nDrawStyle) = 0;
	virtual void	SetHtsPath(LPCTSTR pcszHtsPath) = 0;
	virtual void	SetInquiryValue( const BOOL nIs) = 0;
	virtual void	SetItemInfo(LPCTSTR lpstrCode, LPCTSTR lpstrName) = 0;
	virtual void	SetLoggingFlag( const BOOL p_bInLogging) = 0;
	virtual void	SetMainBlock(BOOL bUserMode) = 0;
	virtual void	SetMainBlock_MainBlock(const bool bUserMode) = 0;
	virtual void	SetMainBlock_ToolBar(const bool bUserMode) = 0;
	virtual void	SetMultiChartDlg(long lpMultiChartDlg) = 0;
	virtual void	SetMultipleSameNameIndicator( BOOL p_bEnable) = 0;		// (2007/1/20 - Seung-Won, Bae) Support Multiple Same-Name Indicator.
	virtual void	SetOnePageDataCount(long nOnePageDataCount) = 0;
	virtual void	SetOnePageDataCount2(long nOnePageDataCount, BOOL bIncludeBongCountMargin) = 0;
	virtual void	SetOnePageDataCountInTRCount(BOOL bIsTRCount, BOOL bIsNowChange) = 0;
	virtual void	SetOnRealProcessing( BOOL p_bOn) = 0;	// (2008/12/21 - Seung-Won, Bae) Manage On Real Processing
	virtual void	SetOutsideCommonInterface(long lpOutsideCommonInterface) = 0;
	virtual void	SetPacketOption(LPCTSTR strPacketOption) = 0;
	virtual void	SetRealData(LPCTSTR strPacketName, double dData, short nOption) = 0;
	virtual void	SetRightMarginWidth( int nRightMarginWidth) = 0;	// (2009/5/13 - Seung-Won, Bae) for Right Margin
	virtual void	SetRscMng( long lKey) = 0;
	virtual void	SetRunTimeMode(BOOL bRunTimeMode) = 0;
	virtual void	SetShowMinMaxPrice(const bool bShow, const bool bOutside = true) = 0;
	virtual void	SetShowTrendLineValue(const int nPosition, const bool bOutside = true) = 0;
	virtual void	SetStateWholeView(const int nParam = 0) = 0;
	virtual void	SetStrBlockRegion(LPCTSTR lpszNewValue) = 0;
	virtual void	SetTimeMark(LPCTSTR p_szMarkTime, BOOL p_bWithScroll) = 0;
	virtual void	SetUserEnvironmentAndIndicatorListFileName(const CString& strFileName) = 0;
	virtual void	SetWholeView( const BOOL bWholeView) = 0;
	virtual void	ShowAnalysisToolText(BOOL p_bTimeLine, BOOL p_bPriceLineLeft, BOOL p_bPriceLineRight, BOOL p_bTrendLine, BOOL p_bTrisectionLine, BOOL p_bQuadrisectionLine, BOOL p_bFibonacciRetraceLine) = 0;
	virtual void	ShowEnvironmentDlg() = 0;
	virtual void	ShowIndicatorConfigDlg() = 0;
	virtual void	ShowInnerToolBar(BOOL p_bShow) = 0;
	virtual void	testLoadAnalysisTools(short nToolOption, short nOption) = 0;
	virtual void	UnlockWindowUpdate( void)	= 0;	// (2007/3/5 - Seung-Won, Bae) Lock/Unlock WindowUpdate
	virtual void	UpdateZoomAndScrollInfo( int p_nFullRange, int p_nStartPos, int p_nViewCount) = 0;
	virtual void	UserFileVersionChecker() = 0;
	virtual void	WriteUserFile(const CString& strFileName) = 0;
	// VertScaleGroup의 가장 처음 Graph의 타입 : 복수종목 - ojtaso (20070504)
	virtual CScaleBaseData::HORZSCALEDRAWERTYPE GetMasterValueType(const int nRow, const int nCol) = 0;
	// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
	virtual const char*	GetUseSTDataName( void) = 0;
	virtual void	SetMouseWheelIndex( int nMouseWheelIndex) = 0;
	virtual void	SetPriceMinMaxShowType( int nMinMaxRatio) = 0;

public:
	// 조건값 사용자 지정 가능 - ojtaso (20070913)
	virtual void	ReCalculateIndicator() = 0;
	// FX차트 주문 연동 - onlyojt (20091204)
	virtual BOOL	OnOrderInfomationChanged(const double dPirce, const int Qty, const int nOrderType, LPCTSTR szOrgOrderID, LPCTSTR strCustItem1, LPCTSTR strCustItem2, LPCTSTR strCustItem3) = 0;


// (2022/2/18 - sang-sin, lee)
public:
	//	시간구분선
	virtual	int			GetFirstBlockHScaleGridType_TimeGubun(int ) = 0;
	virtual BOOL        GetFirstBlockHScaleType_TimeGubun(int ) = 0;
	virtual	COLORREF	GetStyleValueHScaleGridColor_TimeGubun(	int ) = 0;
	virtual	int			GetStyleValueScaleLineThickness_TimeGubun(int	) = 0;

	//KHD : 시간구분
	virtual	BOOL		SetAllHScaleGridType_TimeGubun(int p_nHScaleGridType, BOOL p_bCheckTime = FALSE, int p_nHScaleTimeType = 0)	= 0;
	virtual	BOOL		SetStyleValueHScaleGridColor_TimeGubun(COLORREF p_clrHScaleGridColor, COLORREF p_clrHScaleGrid_WeekColor, COLORREF p_clrHScaleGrid_MonColor, COLORREF p_clrHScaleGrid_YearColor, BOOL p_bWithUpdate = FALSE) = 0;
	virtual	BOOL		SetStyleValueScaleLineThickness_TimeGubun(int nType, int p_nLineThickness) = 0;
};

#endif // !defined(AFX_ICHARTCTRL_H__804FF249_4729_49B6_8913_7CEDC34A43B0__INCLUDED_)
