// IWChartCtrl.h: interface for the CIWChartCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined( AFX_IWCHARTCTRL_H__6470428F_7AB9_4790_B60C_C08F1BDF5990__INCLUDED_)
#define AFX_IWCHARTCTRL_H__6470428F_7AB9_4790_B60C_C08F1BDF5990__INCLUDED_

#include "IChartCtrl.h"		// for IChartCtrl
#include _ICHARTCTRL_CTRL_HEADER

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/1/8
// Comments		:	1. Chart Control Interface Class for access afx_msg Function
//						afx_msg Function can not be virtual.
//					2. Every Member Routines are called by AddInMananger with IChartCtrl
//						So Do not declare "public"
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

class CIWChartCtrl : public CObject, public IChartCtrl
{
protected:
	_ICHARTCTRL_CTRL *	m_pChartCtrl;
public:
	CIWChartCtrl( _ICHARTCTRL_CTRL *p_pChartCtrl)	{	m_pChartCtrl = p_pChartCtrl;	}

protected:
	virtual bool	AddChart_GenneralChart( const CString &strChartName)
					{	return m_pChartCtrl->AddChart_GenneralChart( strChartName);	}
	virtual bool	AddOrDeleteChart( const CString &strChartName)
					{	return m_pChartCtrl->AddOrDeleteChart( strChartName);	}
	virtual bool	AddOrDeleteChart_Genneral( const CString &strChartName)
					{	return m_pChartCtrl->AddOrDeleteChart_Genneral( strChartName);	}
	virtual bool	AddOrDeleteChart_Special( const CString &strChartName)
					{	return m_pChartCtrl->AddOrDeleteChart_Special( strChartName);	}
	virtual bool	AddRemoveGraph( LPCTSTR pcszIndicatorName)
					{	return m_pChartCtrl->AddRemoveGraph( pcszIndicatorName);	}
	virtual bool	ApplySkin()
					{	return m_pChartCtrl->ApplySkin();	}
//	virtual bool	CalculateAll( const CCalculateBaseData::CALCULATETYPE enumCalculateType)
//	virtual bool	CalculateIndicatorInTRData( CIndicatorInfo *pIndicatorInfo, const CList<CPacket *, CPacket *> &packetList)
	virtual bool	ChangeChart_PriceChartStyle( const CString &strChartName)
					{	return m_pChartCtrl->ChangeChart_PriceChartStyle( strChartName);	}
	virtual bool	ChangeChartType( CString strIndicatorName, CString strType, CString strStyle, CString strPacketNameList)
					{	return m_pChartCtrl->ChangeChartType( strIndicatorName, strType, strStyle, strPacketNameList);	}
	virtual bool	ChangeOneChart( const CString &strChartName)
					{	return m_pChartCtrl->ChangeOneChart( strChartName);	}
	virtual bool	ChangeSubGraphData( const char *pcszIndicatorName, const char *pcszSubGraphName, COLORREF clrLineColor, int nThickness, int nDrawStyle)
					{	return m_pChartCtrl->ChangeSubGraphData( pcszIndicatorName, pcszSubGraphName, clrLineColor, nThickness, nDrawStyle);	}
	virtual bool	CreateUserDirectory()
					{	return m_pChartCtrl->CreateUserDirectory();	}
	virtual bool	DeleteChart_Genneral( const CString &strChartName)
					{	return m_pChartCtrl->DeleteChart_Genneral( strChartName);	}
	virtual bool	DeleteIndicator( const char *pcszIndicatorName)
					{	return m_pChartCtrl->DeleteIndicator( pcszIndicatorName);	}
	virtual bool	GetAutoSave()
					{	return m_pChartCtrl->GetAutoSave();	}
	virtual bool	GetExistenceHighLowBound() const
					{	return m_pChartCtrl->GetExistenceHighLowBound();	}
	virtual bool	GetImaginaryCandle()
					{	return m_pChartCtrl->GetImaginaryCandle();	}
	virtual bool	GetRequestTRPacketName( const CString &strPacketNames, CString &strPacketNamesForTR) const
					{	return m_pChartCtrl->GetRequestTRPacketName( strPacketNames, strPacketNamesForTR);	}
//	virtual bool	GetShowCrossLine()
//					{	return m_pChartCtrl->GetShowCrossLine();	}
	virtual bool	GetShowExRightDividend()
					{	return m_pChartCtrl->GetShowExRightDividend();	}
	virtual bool	GetShowHighLowBound()
					{	return m_pChartCtrl->GetShowHighLowBound();	}
	virtual bool	GetShowIndicatorValue()
					{	return m_pChartCtrl->GetShowIndicatorValue();	}
	virtual bool	GetShowIndicatorVariable()
					{	return m_pChartCtrl->GetShowIndicatorVariable();	}
	virtual bool	GetShowMinMaxPrice()
					{	return m_pChartCtrl->GetShowMinMaxPrice();	}
	virtual bool	GetTimeLinkage()
					{	return m_pChartCtrl->GetTimeLinkage();	}
	virtual bool	HasGraphAdapter( const char *szGraphName)
					{	return m_pChartCtrl->HasGraphAdapter( szGraphName);	}
	virtual bool	IsExistenceBoundMark() const
					{	return m_pChartCtrl->IsExistenceBoundMark();	}
	virtual bool	IsLoad_MarketChart() const
					{	return m_pChartCtrl->IsLoad_MarketChart();	}
//	virtual bool	IsRealType( const CScaleBaseData::HORZSCALEDRAWERTYPE nType) const
	virtual bool	IsScrollInHorzScaleData() const
					{	return m_pChartCtrl->IsScrollInHorzScaleData();	}
	virtual bool	IsUseToolScrollBar()
					{	return m_pChartCtrl->IsUseToolScrollBar();	}
	virtual bool	IsWholeView() const
					{	return m_pChartCtrl->IsWholeView();	}
	virtual bool	OnReturnOrg2( const BOOL bIsMessageBox = TRUE)
					{	return m_pChartCtrl->OnReturnOrg2( bIsMessageBox);	}
//	virtual bool	ResetSaveChartCfg( CChartCfg *pChart, const bool bRegionRetain)
	virtual bool	ResetSaveChartCfg( int nOption, bool bRegionRetain)
					{	return m_pChartCtrl->ResetSaveChartCfg( nOption, bRegionRetain);	}
	virtual bool	RestoreChartCfg( const int nIndex)
					{	return m_pChartCtrl->RestoreChartCfg( nIndex);	}
	virtual bool	RollbackLastNoOneChart()
					{	return m_pChartCtrl->RollbackLastNoOneChart();	}
	virtual bool	RunToolFuncGraphColor( const COLORREF clrGraphColor)
					{	return m_pChartCtrl->RunToolFuncGraphColor( clrGraphColor);	}
	virtual bool	RunToolFuncGraphStyle( const int nParam)
					{	return m_pChartCtrl->RunToolFuncGraphStyle( nParam);	}
	virtual bool	RunToolFuncLineThickness( const int nParam)
					{	return m_pChartCtrl->RunToolFuncLineThickness( nParam);	}
//	virtual bool	RunToolFunction( const CToolOptionInfo::TOOLOPTION enumToolOption, const int nParam)
	virtual bool	SaveChartCfg()
					{	return m_pChartCtrl->SaveChartCfg();	}
	virtual bool	SaveChartCfgInCurrentOCX()
					{	return m_pChartCtrl->SaveChartCfgInCurrentOCX();	}
	virtual bool	SetAllPacketName( const CString &strIndicatorName, const CString &strNewPacketName)
					{	return m_pChartCtrl->SetAllPacketName( strIndicatorName, strNewPacketName);	}
	virtual bool	SetAllProperties_NEOCONVERT( CStringList *pPropStrList)
					{	return m_pChartCtrl->SetAllProperties_NEOCONVERT( pPropStrList);	}
//	virtual bool	SetAnalysisTool_TextPosition()
//					{	return m_pChartCtrl->SetAnalysisTool_TextPosition();	}
//	virtual bool	SetAnaylsisToolData( const CAnalysisToolData &analysisToolData)
//	virtual bool	SetOcxDataFromChartCfg( CChartCfg *pChart, bool bRegionRetain)
	virtual bool	SetPacketDataInPacketList( LPCTSTR szPacketName, long szData, long lLength, BOOL bReset)
					{	return m_pChartCtrl->SetPacketDataInPacketList( szPacketName, szData, lLength, bReset);	}
//	virtual bool	SetScrollData( const int nType = SETSCROLL_TR, const bool bIsHorzScaleDataCheck = true, const bool bIsRedraw)
//	virtual bool	SetSkinColorData( const CStyleData &sourceStyleData)
	virtual bool	SetStartIndexOfPacket( const char *pcszPacketName, int nStartDataIndex, BOOL bAddSpace)
					{	return m_pChartCtrl->SetStartIndexOfPacket( pcszPacketName, nStartDataIndex, bAddSpace);	}
	virtual bool	SetStartPeriodOfPacketAdapter( const char *szPacketName, long lStartPeriod, BOOL bAddSpace)
					{	return m_pChartCtrl->SetStartPeriodOfPacketAdapter( szPacketName, lStartPeriod, bAddSpace);	}
//	virtual bool	SetStyleData( const CStyleData &styleData)
	virtual bool	SetSubGraphPacketName( const CString &strIndicatorName, const int nSubGraphIndex, const CString &strNewPacketName)
					{	return m_pChartCtrl->SetSubGraphPacketName( strIndicatorName, nSubGraphIndex, strNewPacketName);	}

protected:
	virtual BOOL	AddChartBlock( LPCTSTR strChartName)
					{	return m_pChartCtrl->AddChartBlock( strChartName);	}
	virtual BOOL	AddOrRemoveChart( LPCTSTR strIndicatorName)
					{	return m_pChartCtrl->AddOrRemoveChart( strIndicatorName);	}
	virtual BOOL	ChangeAllFunctionName( LPCTSTR szTargetFunctionName, LPCTSTR szSourceFunctionName, BOOL bKeepOldGraphColor)
					{	return m_pChartCtrl->ChangeAllFunctionName( szTargetFunctionName, szSourceFunctionName, bKeepOldGraphColor);	}
	virtual BOOL	ChangeBlockType( short nBlockType, short nRowCount, short nColumnCount)
					{	return m_pChartCtrl->ChangeBlockType( nBlockType, nRowCount, nColumnCount);	}
	virtual BOOL	ChangeChartItem( LPCTSTR strIndicatorName, short nSubGraphIndex, LPCTSTR strNewTitle, LPCTSTR strNewPacketName, LPCTSTR strNewGraphType, LPCTSTR strNewGraphStyle, short nNewUpLineWidth, short nNewDownLineWidth, short nNewEqualLineWidth, BOOL bRunatOnce)
					{	return m_pChartCtrl->ChangeChartItem( strIndicatorName, nSubGraphIndex, strNewTitle, strNewPacketName, strNewGraphType, strNewGraphStyle, nNewUpLineWidth, nNewDownLineWidth, nNewEqualLineWidth, bRunatOnce);	}
	virtual BOOL	ChangeGraphDatas( short nRowIndex, short nColumnIndex, short nVerticalIndex, short nGraphIndex, LPCTSTR strNewIndicatorName, LPCTSTR strNewTitle, LPCTSTR strNewPacketName, LPCTSTR strNewGraphType, LPCTSTR strNewGraphStyle, short nNewUpLineWidth, short nNewDownLineWidth, short nNewEqualLineWidth, BOOL bRunatOnce)
					{	return m_pChartCtrl->ChangeGraphDatas( nRowIndex, nColumnIndex, nVerticalIndex, nGraphIndex, strNewIndicatorName, strNewTitle, strNewPacketName, strNewGraphType, strNewGraphStyle, nNewUpLineWidth, nNewDownLineWidth, nNewEqualLineWidth, bRunatOnce);	}
	virtual BOOL	ChangeGraphDrawingData( LPCTSTR p_szGraphName, BOOL p_bLowHigh, BOOL p_bLowCur, BOOL p_bHighCur, BOOL p_bPrevCur, short p_nCurData)
					{	return m_pChartCtrl->ChangeGraphDrawingData( p_szGraphName, p_bLowHigh, p_bLowCur, p_bHighCur, p_bPrevCur, p_nCurData);	}
	virtual BOOL	ChangeGraphTitle( short nRowIndex, short nColumnIndex, short nGraphIndex, LPCTSTR strNewTitle)
					{	return m_pChartCtrl->ChangeGraphTitle( nRowIndex, nColumnIndex, nGraphIndex, strNewTitle);	}
	virtual BOOL	ChangeGraphTitle2( LPCTSTR szTargetTitle, LPCTSTR szSourceTitle)
					{	return m_pChartCtrl->ChangeGraphTitle2( szTargetTitle, szSourceTitle);	}
	virtual BOOL	ChangeGraphType( LPCTSTR strIndicatorName, LPCTSTR strGraphType, LPCTSTR strGraphStyle, LPCTSTR strPacketNameList, BOOL bRunatOnce)
					{	return m_pChartCtrl->ChangeGraphType( strIndicatorName, strGraphType, strGraphStyle, strPacketNameList, bRunatOnce);	}
	virtual BOOL	ChangeHorzScalePosition( short nScalePosition, short nRowIndex, short nColumnIndex)
					{	return m_pChartCtrl->ChangeHorzScalePosition( nScalePosition, nRowIndex, nColumnIndex);	}
	virtual BOOL	ChangeIndicatorPacketName( LPCTSTR strIndicatorName, short nSubGraphIndex, LPCTSTR strNewPacketName)
					{	return m_pChartCtrl->ChangeIndicatorPacketName( strIndicatorName, nSubGraphIndex, strNewPacketName);	}
	virtual BOOL	ChangePacketType( LPCTSTR p_szPacketName, LPCTSTR p_szPacketType)
					{	return m_pChartCtrl->ChangePacketType( p_szPacketName, p_szPacketType);	}
	virtual BOOL	ChangePriceGraphDrawingData( BOOL p_bLowHigh, BOOL p_bLowCur, BOOL p_bHighCur, BOOL p_bPrevCur, short p_nCurData, short p_nDefaultType)
					{	return m_pChartCtrl->ChangePriceGraphDrawingData( p_bLowHigh, p_bLowCur, p_bHighCur, p_bPrevCur, p_nCurData, p_nDefaultType);	}
	virtual BOOL	ChangeVertScalePosition( short nScalePosition, short nRowIndex, short nColumnIndex, short nVSGroupIndex)
					{	return m_pChartCtrl->ChangeVertScalePosition( nScalePosition, nRowIndex, nColumnIndex, nVSGroupIndex);	}
	virtual BOOL	CheckGSharedSetting( const char *p_szGSharedName, BOOL p_bStart, BOOL p_bGet, BOOL p_bSet)
					{	return m_pChartCtrl->CheckGSharedSetting( p_szGSharedName, p_bStart, p_bGet, p_bSet);	}
	virtual BOOL	ClearPacketData( LPCTSTR strPacketName, BOOL bIsAbsoluteCreal)
					{	return m_pChartCtrl->ClearPacketData( strPacketName, bIsAbsoluteCreal);	}
	virtual BOOL	DestroyWindow()
					{	return m_pChartCtrl->DestroyWindow();	}
//	virtual BOOL	DoAct( CToolOptionInfo::TOOLOPTION nToolOption)
//	virtual BOOL	DrawDragRect( CDC *pdc, CPoint ptFirst, CPoint ptPrev, CPoint ptCurr, int nWeight, BOOL bMouseMove = TRUE)
//					{	return m_pChartCtrl->DrawDragRect( pdc, ptFirst, ptPrev, ptCurr, nWeight, bMouseMove);	}
//	virtual BOOL	DrawRectSemiTransparent( CDC *pdc, CRect rect, CRect oldrect)
//					{	return m_pChartCtrl->DrawRectSemiTransparent( pdc, rect, oldrect);	}
	virtual BOOL	FireGSharedData( const char *p_szGSharedName, const char *p_szGSharedData)
					{	return m_pChartCtrl->FireGSharedData( p_szGSharedName, p_szGSharedData);	}
	virtual BOOL	FireSharedData( const char *p_szShareName, const char *p_szSharedData)
					{	return FALSE;	}	// (2006/1/11 - Seung-Won, Bae) Hanwha Platform Only
	virtual BOOL	GetBoundMarkFlag()
					{	return m_pChartCtrl->GetBoundMarkFlag();	}
	virtual BOOL	GetDataStartEnd( int &p_nStartIndex, int &p_nEndIndex)
					{	return m_pChartCtrl->GetDataStartEnd( p_nStartIndex, p_nEndIndex);	}
	virtual BOOL	GetLogScale( LPCTSTR strIndicatorName)
					{	return m_pChartCtrl->GetLogScale( strIndicatorName);	}
	virtual BOOL	GetScrollRange( int &p_nRange)
					{	return m_pChartCtrl->GetScrollRange( p_nRange);	}
	virtual BOOL	GetShareInfoItem( const char *p_szShareName, int &p_nShareType, int &p_nShareArea, long &p_lCodeFilter)
					{	return FALSE;	}	// (2006/1/11 - Seung-Won, Bae) Hanwha Platform Only
	virtual BOOL	GetStartEndDateOnView( double *p_pStartDate, double *p_pEndDate)
					{	return m_pChartCtrl->GetStartEndDateOnView( p_pStartDate, p_pEndDate);	}
	virtual BOOL	HasGraph( LPCTSTR p_szGraphName)
					{	return m_pChartCtrl->HasGraph( p_szGraphName);	}
	virtual BOOL	HasGraphInSpecialBack( LPCTSTR p_szGraphName)
					{	return m_pChartCtrl->HasGraphInSpecialBack( p_szGraphName);	}
	virtual BOOL	HasToolItem( CToolOptionInfo::TOOLOPTION p_eToolItem)
					{	return m_pChartCtrl->HasToolItem( p_eToolItem);	}
	virtual BOOL	InvokeAddIn( LPCTSTR p_szAddInDllName, short p_nCommandType, long p_lData)
					{	return m_pChartCtrl->InvokeAddIn( p_szAddInDllName, p_nCommandType, p_lData);	}
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szAddInDllName, LPCTSTR p_szCommandName, LPCTSTR p_szData)
					{	return m_pChartCtrl->InvokeAddInStr( p_szAddInDllName, p_szCommandName, p_szData);	}
	virtual BOOL	IsChartType( const char *p_szChartName) const
					{	return m_pChartCtrl->IsChartType( p_szChartName);	}
	virtual BOOL	IsDaegimaemul() const
					{	return m_pChartCtrl->IsDaegimaemul();	}
	virtual BOOL	IsGraphMenuShow() const
					{	return m_pChartCtrl->m_bAddGraphMenuShow;	}
	virtual BOOL	IsOcxAreaNumericalInquiry( void) const
					{	return m_pChartCtrl->IsOcxAreaNumericalInquiry();	}
	virtual BOOL	IsPriceChartType( const char *p_szChartName) const
					{	return m_pChartCtrl->IsPriceChartType( p_szChartName);	}
	virtual BOOL	IsRunningOneChart() const
					{	return m_pChartCtrl->IsRunningOneChart();	}
	virtual BOOL	IsRunTimeMode() const
					{	return m_pChartCtrl->IsRunTimeMode();	}
	virtual BOOL	IsShowPriceRateInquiry( void) const
					{	return m_pChartCtrl->IsShowPriceRateInquiry();	}
	virtual BOOL	IsShowHorizonTextLeft( void)	const
					{	return m_pChartCtrl->m_bShowHorizonTextLeft;	}
	virtual BOOL	IsShowHorizonTextRight( void)	const
					{	return m_pChartCtrl->m_bShowHorizonTextRight;	}
	virtual BOOL	IsShowFiboText( void)			const
					{	return m_pChartCtrl->m_bShowFiboText;	}
	virtual BOOL	IsShowFourText( void)			const
					{	return m_pChartCtrl->m_bShowFourText;	}
	virtual BOOL	IsShowThreeText( void)			const
					{	return m_pChartCtrl->m_bShowThreeText;	}
	virtual BOOL	IsShowTrendLineText( void)		const
					{	return m_pChartCtrl->m_bShowTrendLineText;	}
	virtual BOOL	IsShowVerticalText( void)		const
					{	return m_pChartCtrl->m_bShowVerticalText;	}
	virtual BOOL	IsThisPacketConnected( const int nIndex, const CString strpacketname)
					{	return m_pChartCtrl->IsThisPacketConnected( nIndex, strpacketname);	}
	virtual BOOL	IsUseInquiryData() const
					{	return m_pChartCtrl->IsUseInquiryData();	}
	virtual BOOL	JoinAllGraph( BOOL bIsOneVertScaleGroup)
					{	return m_pChartCtrl->JoinAllGraph( bIsOneVertScaleGroup);	}
	virtual BOOL	JoinandDevisionforCompareChart( long nRowCount, long nColumnCount, BOOL bJoin)
					{	return m_pChartCtrl->JoinandDevisionforCompareChart( nRowCount, nColumnCount, bJoin);	}
	virtual BOOL	LoadAddIn( LPCTSTR p_szAddInDllName)
					{	return m_pChartCtrl->LoadAddIn( p_szAddInDllName);	}
	virtual BOOL	OffRealAdvise()
					{	return FALSE;	}	// (2006/1/11 - Seung-Won, Bae) Hanwha Platform Only
//	virtual BOOL	OnAnalysisTool( CToolOptionInfo::TOOLOPTION nToolOption, int nOption = 1)
//	virtual BOOL	OnAutoTrendTool( int nOption = 1)
//					{	return m_pChartCtrl->OnAutoTrendTool( nOption);	}
//	virtual BOOL	OnLButtonDown_DesignMode( const UINT nFlags, const CPoint &point)
//					{	return m_pChartCtrl->OnLButtonDown_DesignMode( nFlags, point);	}
//	virtual BOOL	OnLButtonDown_NumericalInquiryDlg( CDC *pDC, const UINT nFlags, const CPoint &point)
//					{	return m_pChartCtrl->OnLButtonDown_NumericalInquiryDlg( pDC, nFlags, point);	}
//	virtual BOOL	OnLButtonDown_Tool( CDC *pDC, UINT nFlags, CPoint point)
//					{	return m_pChartCtrl->OnLButtonDown_Tool( pDC, nFlags, point);	}
//	virtual BOOL	OnLButtonUp_DesignMode( const UINT nFlags, const CPoint &point)
//					{	return m_pChartCtrl->OnLButtonUp_DesignMode( nFlags, point);	}
//	virtual BOOL	OnLButtonUp_Tool( CDC *pDC, UINT nFlags, CPoint point)
//					{	return m_pChartCtrl->OnLButtonUp_Tool( pDC, nFlags, point);	}
//	virtual BOOL	OnMouseMove_Tool( CDC *pDC, UINT nFlags, CPoint point)
//					{	return m_pChartCtrl->OnMouseMove_Tool( pDC, nFlags, point);	}
	virtual BOOL	OnMouseWheel( UINT nFlags, short zDelta, CPoint pt)
					{	return m_pChartCtrl->OnMouseWheel( nFlags, zDelta, pt);	}
	virtual BOOL	OnRealAdvise()
					{	return FALSE;	}	// (2006/1/11 - Seung-Won, Bae) Hanwha Platform Only
	virtual BOOL	OnRunningOneChart()
					{	return m_pChartCtrl->OnRunningOneChart();	}
	virtual BOOL	OnSetCursor( CWnd *pWnd, UINT nHitTest, UINT message)
					{	return m_pChartCtrl->OnSetCursor( pWnd, nHitTest, message);	}
	virtual BOOL	PacketDataMaker()
					{	return m_pChartCtrl->PacketDataMaker();	}
	virtual BOOL	PartitionAllGraph()
					{	return m_pChartCtrl->PartitionAllGraph();	}
	virtual BOOL	ReadUserFile( const CString &strFileName)
					{	return m_pChartCtrl->ReadUserFile( strFileName);	}
//	virtual BOOL	ReadUserFile( CSaveUserFile::FILETYPE nType)
	virtual BOOL	RemoveChart( LPCTSTR strChartName)
					{	return m_pChartCtrl->RemoveChart( strChartName);	}
	virtual BOOL	RemoveGraph_Title( short nRowIndex, short nColumnIndex, LPCTSTR szGraphTitle, BOOL bIsOnlyNoTRData, BOOL bIsAutoHorzScalePosition)
					{	return m_pChartCtrl->RemoveGraph_Title( nRowIndex, nColumnIndex, szGraphTitle, bIsOnlyNoTRData, bIsAutoHorzScalePosition);	}
	virtual BOOL	RemovePacket( LPCTSTR szPacketName)
					{	return m_pChartCtrl->RemovePacket( szPacketName);	}
	virtual BOOL	RemoveSetCapture( void)
					{	return m_pChartCtrl->RemoveSetCapture();	}
	virtual BOOL	ResetChart()
					{	return m_pChartCtrl->ResetChart();	}
	virtual BOOL	ResetPropertiesfromMainBlock()
					{	return m_pChartCtrl->ResetPropertiesfromMainBlock();	}
	virtual BOOL	ResetRealListString( LPCTSTR p_szRealList, BOOL p_bAdviseNow)
					{	return m_pChartCtrl->ResetRealListString( p_szRealList, p_bAdviseNow);	}
	virtual BOOL	RestoreMin( LPCTSTR strPacketName)
					{	return m_pChartCtrl->RestoreMin( strPacketName);	}
	virtual BOOL	RestoreMinMax( LPCTSTR strPacketName)
					{	return m_pChartCtrl->RestoreMinMax( strPacketName);	}
	virtual BOOL	RestorMax( LPCTSTR strPacketName)
					{	return m_pChartCtrl->RestorMax( strPacketName);	}
	virtual BOOL	SaveandOpenFromFile( LPCTSTR strOpenFileName, LPCTSTR strSaveFileName, BOOL bOpenOption, BOOL bSaveOption)
					{	return m_pChartCtrl->SaveandOpenFromFile( strOpenFileName, strSaveFileName, bOpenOption, bSaveOption);	}
	virtual BOOL	ScrollTo( int p_nScrollValue, BOOL p_bRelative)
					{	return m_pChartCtrl->ScrollTo( p_nScrollValue, p_bRelative);	}
	virtual BOOL	ScrollToDataOfEnd( double p_dData)
					{	return m_pChartCtrl->ScrollToDataOfEnd( p_dData);	}
	virtual BOOL	SetAllProperties2( long dwData)
					{	return m_pChartCtrl->SetAllProperties2( dwData);	}
	virtual BOOL	SetAllPropertiesFromFile( LPCTSTR strFilePath, LPCTSTR strFileName)
					{	return m_pChartCtrl->SetAllPropertiesFromFile( strFilePath, strFileName);	}
	virtual BOOL	SetDataStartEnd( int p_nStartIndex, int p_nEndIndex)
					{	return m_pChartCtrl->SetDataStartEnd( p_nStartIndex, p_nEndIndex);	}
	virtual BOOL	SetDrdsAdvise( BOOL bOnAdvise)
					{	return m_pChartCtrl->SetDrdsAdvise( bOnAdvise);	}
	virtual BOOL	SetGraphHide2( short nRowIndex, short nColumnIndex, short nVerticalIndex, short nGraphIndex, short nSubGraphIndex, BOOL bHide)
					{	return m_pChartCtrl->SetGraphHide2( nRowIndex, nColumnIndex, nVerticalIndex, nGraphIndex, nSubGraphIndex, bHide);	}
	virtual BOOL	SetGraphHide3( LPCTSTR strTitle, BOOL bIsHide)
					{	return m_pChartCtrl->SetGraphHide3( strTitle, bIsHide);	}
	virtual BOOL	SetGraphTitle( LPCTSTR strIndicatorName, short nSubGraphIndex, LPCTSTR strNewGraphTitle, BOOL bGraphShowing)
					{	return m_pChartCtrl->SetGraphTitle( strIndicatorName, nSubGraphIndex, strNewGraphTitle, bGraphShowing);	}
	virtual BOOL	SetHelpMessageRealDataRate( LPCTSTR strHelpMessageData, BOOL bIsRemove)
					{	return m_pChartCtrl->SetHelpMessageRealDataRate( strHelpMessageData, bIsRemove);	}
	virtual BOOL	SetHorzScaleData( short nRowIndex, short nColumnIndex, LPCTSTR strScaleData)
					{	return m_pChartCtrl->SetHorzScaleData( nRowIndex, nColumnIndex, strScaleData);	}
	virtual BOOL	SetIndicatorCondition( LPCTSTR strIndicatorName, short nConditionType, short nConditionIndex, double dConditionValue, BOOL bReCalculate)
					{	return m_pChartCtrl->SetIndicatorCondition( strIndicatorName, nConditionType, nConditionIndex, dConditionValue, bReCalculate);	}
	virtual BOOL	SetLogScale( LPCTSTR strIndicatorName, BOOL bLogScale)
					{	return m_pChartCtrl->SetLogScale( strIndicatorName, bLogScale);	}
	virtual BOOL	SetMax( LPCTSTR strPacketName, double dMax)
					{	return m_pChartCtrl->SetMax( strPacketName, dMax);	}
	virtual BOOL	SetMin( LPCTSTR strPacketName, double dMin)
					{	return m_pChartCtrl->SetMin( strPacketName, dMin);	}
	virtual BOOL	SetMinMax( LPCTSTR strPacketName, double dMin, double dMax)
					{	return m_pChartCtrl->SetMinMax( strPacketName, dMin, dMax);	}
	virtual BOOL	SetPacketData( LPCTSTR szPacketName, long szData, long lLength, BOOL bReset)
					{	return m_pChartCtrl->SetPacketData( szPacketName, szData, lLength, bReset);	}
	virtual BOOL	SetPacketData2( LPCTSTR strPacketName, LPCTSTR strPacketType, LPCTSTR strPacketData, short nDataCount, BOOL bReCalculate)
					{	return m_pChartCtrl->SetPacketData2( strPacketName, strPacketType, strPacketData, nDataCount, bReCalculate);	}
	virtual BOOL	SetPacketDataInAdditionPacket( LPCTSTR szPacketName, long szData, long lLength, BOOL bReset)
					{	return m_pChartCtrl->SetPacketDataInAdditionPacket( szPacketName, szData, lLength, bReset);	}
	virtual BOOL	SetPacketHelpMessage( LPCTSTR strHelpMsg, BOOL bIsAllRemove)
					{	return m_pChartCtrl->SetPacketHelpMessage( strHelpMsg, bIsAllRemove);	}
	virtual BOOL	SetRealAdviseHW( BOOL p_bAdvise)
					{	return FALSE;	}	// (2006/1/11 - Seung-Won, Bae) Hanwha Platform Only
	virtual BOOL	SetScrollPosition( const int nPos)
					{	return m_pChartCtrl->SetScrollPosition( nPos);	}
	virtual BOOL	SetScrollPosToNextPage()
					{	return m_pChartCtrl->SetScrollPosToNextPage();	}
	virtual BOOL	SetSharedData( short p_nShareType, LPCTSTR p_szSharedName, LPCTSTR p_szData)
					{	return FALSE;	}	// (2006/1/11 - Seung-Won, Bae) Hanwha Platform Only
	virtual BOOL	SetSharedDataHW( LPCTSTR p_szShareName, LPCTSTR p_szSharedData)
					{	return FALSE;	}	// (2006/1/11 - Seung-Won, Bae) Hanwha Platform Only
	virtual BOOL	SetStartEndDateOfView( double p_dStartDate, double p_dEndDate)
					{	return m_pChartCtrl->SetStartEndDateOfView( p_dStartDate, p_dEndDate);	}
	virtual BOOL	SetStartPeriodOfPacket( LPCTSTR p_szPacketName, long p_lStartPeriod, BOOL p_bAddSpace)
					{	return m_pChartCtrl->SetStartPeriodOfPacket( p_szPacketName, p_lStartPeriod, p_bAddSpace);	}
	virtual BOOL	SetStopLossPrice( BOOL bIsStopLoss, double dStopLossPrice, BOOL bIsProfit, double dProfitPrice)
					{	return m_pChartCtrl->SetStopLossPrice( bIsStopLoss, dStopLossPrice, bIsProfit, dProfitPrice);	}
	virtual BOOL	SetVertScaleData( short nRowIndex, short nColumnIndex, short nVertScaleGroupIndex, LPCTSTR strVertScaleData)
					{	return m_pChartCtrl->SetVertScaleData( nRowIndex, nColumnIndex, nVertScaleGroupIndex, strVertScaleData);	}
	virtual BOOL	UnloadAddIn( LPCTSTR p_szAddInDllName)
					{	return m_pChartCtrl->UnloadAddIn( p_szAddInDllName);	}
	virtual BOOL	ZoomTo( int p_nZoomValue, BOOL p_bRelative)
					{	return m_pChartCtrl->ZoomTo( p_nZoomValue, p_bRelative);	}

protected:
	virtual BSTR	GetCustomFont()
					{	return m_pChartCtrl->GetCustomFont();	}
	virtual BSTR	GetData( LPCTSTR strPacketName, short nDataIndex)
					{	return m_pChartCtrl->GetData( strPacketName, nDataIndex);	}
	virtual BSTR	GetGraphPacketName( short nRowIndex, short nColumnIndex, LPCTSTR szGraphTitle)
					{	return m_pChartCtrl->GetGraphPacketName( nRowIndex, nColumnIndex, szGraphTitle);	}
	virtual BSTR	GetIndicatorNameList( LPCTSTR p_strGroup)
					{	return m_pChartCtrl->GetIndicatorNameList( p_strGroup);	}
	virtual BSTR	GetPacketOption()
					{	return m_pChartCtrl->GetPacketOption();	}
	virtual BSTR	GetRealSettingInfo()
					{	return m_pChartCtrl->GetRealSettingInfo();	}
	virtual BSTR	GetStartDateOfData()
					{	return m_pChartCtrl->GetStartDateOfData();	}
	virtual BSTR	GetStrBlockRegion()
					{	return m_pChartCtrl->GetStrBlockRegion();	}

protected:
	virtual const char *	GetBaseDeskPath() const
							{	return m_pChartCtrl->m_strHtsPath;	}
	virtual const char *	GetUserFilePath() const
							{	return m_pChartCtrl->m_strUserFilePath;	}
	virtual const char *	GetOCXPath() const
							{	return g_strOcxPath; }

//protected:
//	virtual CChartCfg	GetChartCfgWithPacketReset( int nOption)

//protected:
//	virtual CChartCfg *	GetChartCfg( const int nOption)
//	virtual CChartCfg *	GetLastChartCfg_NoOneChart()

public:
	virtual CCommonInterface *	GetCommonInterface( void)
					{	return ( CCommonInterface *)m_pChartCtrl;	}

protected:
	virtual CFont *	GetChartFont( void)
					{	return m_pChartCtrl->GetChartFont();	}

protected:
	virtual CFont *	GetRscFont() const
					{	return m_pChartCtrl->GetRscFont();	}

protected:
	virtual CIndicatorList *	GetIndicatorList( void)
					{	return m_pChartCtrl->GetIndicatorList();	}

protected:
	virtual CMainBlock *	GetMainBlock( void)
					{	return m_pChartCtrl->GetMainBlock();	}

protected:
	virtual COLORREF GetAnalysisToolColor( void) const
					{	return m_pChartCtrl->GetAnalysisToolColor();	}
	virtual COLORREF GetTrendLineColor()
					{	return m_pChartCtrl->GetTrendLineColor();	}

protected:
	virtual CPacketList *	GetPacketList( void)
					{	return m_pChartCtrl-> 	GetPacketList();	}

protected:
	virtual CRect	GetMainBlockRegion()
					{	return m_pChartCtrl->GetMainBlockRegion();	}
	virtual CRect	GetOcxRegion() const
					{	return m_pChartCtrl->GetOcxRegion();	}
	virtual CRect	GetToolBarRegion() const
					{	return m_pChartCtrl->GetToolBarRegion();	}
	virtual CRect	GetVirtualRegion()
					{	return m_pChartCtrl->GetVirtualRegion();	}
/*
protected:
	virtual CString	GetBlockDataForMaking( const CString &strChartName) const
					{	return m_pChartCtrl->GetBlockDataForMaking( strChartName);	}
	virtual CString	GetDataFromMainBlock()
					{	return m_pChartCtrl->GetDataFromMainBlock();	}
	virtual CString	GetDataWindowRealPacketNames( const CString &strTitle, const CString &strPacketNames) const
					{	return m_pChartCtrl->GetDataWindowRealPacketNames( strTitle, strPacketNames);	}
	virtual CString	GetDefaultFilePathAndName( const CString &strFileName) const
					{	return m_pChartCtrl->GetDefaultFilePathAndName( strFileName);	}
	virtual CString	GetDefaultPath() const
					{	return m_pChartCtrl->GetDefaultPath();	}
	virtual CString	GetDesignModePath()
					{	return m_pChartCtrl->GetDesignModePath();	}
	virtual CString	GetFilePathAndName( const CString &strFilePath, const CString &strFileName) const
					{	return m_pChartCtrl->GetFilePathAndName( strFilePath, strFileName);	}
//	virtual CString	GetGraphDataForMaking( CIndicatorInfo *p_pIndicatorInfo) const
	virtual CString	GetJoinProperties( int nOption)
					{	return m_pChartCtrl->GetJoinProperties( nOption);	}
//	virtual CString	GetOneChartName( const CToolOptionInfo::TOOLOPTION nType) const
	virtual CString	GetPropertyValue( int nType, int nOrder)
					{	return m_pChartCtrl->GetPropertyValue( nType, nOrder);	}
	virtual CString	GetUserDefaultFilePath( const CString &strBaseDeskPath = "") const
					{	return m_pChartCtrl->GetUserDefaultFilePath( strBaseDeskPath);	}
	virtual CString	GetUserDefaultFilePathAndName( const CString &strFileName) const
					{	return m_pChartCtrl->GetUserDefaultFilePathAndName( strFileName);	}
	virtual CString	GetUserFilePathAndName( const CString &strFileName) const
					{	return m_pChartCtrl->GetUserFilePathAndName( strFileName);	}
	virtual CString	GetUserPath() const
					{	return m_pChartCtrl->GetUserPath();	}
*/

//protected:
//	virtual CToolOptionInfo::TOOLOPTION	GetSavedChartType()
//	virtual CToolOptionInfo::TOOLOPTION	GetToolType( CSelectMode::MOUSESELECTMODE eSelectMode, int nType)

protected:
	virtual CWnd *	AddSetCapture( void)
					{	return m_pChartCtrl-> 	AddSetCapture();	}
	virtual CWnd *	GetMainWnd( void)
					{	return m_pChartCtrl-> 	GetMainWnd();	}
	virtual CWnd *	GetChartWindowPointer() const
					{	return m_pChartCtrl-> 	GetChartWindowPointer();	}

protected:
	virtual double	ConvertYPositionToData( const int nYPosition, const int nYPositionTop, const int nYPositionBottom,const double &dDataMax, const double &dDataMin, const bool bLog, const bool bReverse)
					{	return m_pChartCtrl->ConvertYPositionToData( nYPosition, nYPositionTop, nYPositionBottom,dDataMax, dDataMin, bLog, bReverse);	}
	virtual double	GetMax( LPCTSTR strPacketName)
					{	return m_pChartCtrl->GetMax( strPacketName);	}
	virtual double	GetMin( LPCTSTR strPacketName)
					{	return m_pChartCtrl->GetMin( strPacketName);	}

protected:
	virtual HCURSOR	GetCursorHandle( void)
					{	return m_pChartCtrl->GetCursorHandle();	}
	virtual HCURSOR	SetCursorHandle( HCURSOR p_hNewCursor)
					{	return m_pChartCtrl->SetCursorHandle( p_hNewCursor);	}

protected:
	virtual HWND	GetBaseDeskHWnd()
					{	return m_pChartCtrl->GetBaseDeskHWnd();	}
	virtual HWND	GetSafeHwnd( void) const
					{	return m_pChartCtrl->GetSafeHwnd();	}
	virtual HWND	GetWndHandle( void) const
					{	return m_pChartCtrl->GetWndHandle();	}

protected:
	virtual int	ConvertDataToYPosition( double p_dData, double p_dDataMax, double p_dDataMin, int p_nYPositionTop, int p_nYPositionBottom, BOOL p_bLog, BOOL p_bReverse)
					{	return m_pChartCtrl->ConvertDataToYPosition( p_dData, p_dDataMax, p_dDataMin, p_nYPositionTop, p_nYPositionBottom, p_bLog, p_bReverse);	}
	virtual int	GetCurrentScrollPos()
					{	return m_pChartCtrl->GetCurrentScrollPos();	}
	virtual int	GetDataCountForToolBar() const
					{	return m_pChartCtrl->GetDataCountForToolBar();	}
	virtual int	GetDataWindowEndIndex( const int nDataIndex, int &nHighlightIndex)
					{	return m_pChartCtrl->GetDataWindowEndIndex( nDataIndex, nHighlightIndex);	}
	virtual int	GetFullDataCount( const bool bIsHorzScaleDataCheck) const
					{	return m_pChartCtrl->GetFullDataCount( bIsHorzScaleDataCheck);	}
//	virtual int	GetInquiryDataWindowMoveType()
//					{	return m_pChartCtrl->GetInquiryDataWindowMoveType();	}
//	virtual int	GetInquiryDataWindowType()
//					{	return m_pChartCtrl->GetInquiryDataWindowType();	}
	virtual int	GetMarginInIndicatorInfo( const CString &strFunctionName) const
					{	return m_pChartCtrl->GetMarginInIndicatorInfo( strFunctionName);	}
	virtual int	GetMaxDataCount() const
					{	return m_pChartCtrl->GetMaxDataCount();	}
	virtual int	GetMaxRange()
					{	return m_pChartCtrl->GetMaxRange();	}
	virtual int	GetMinCountPerPage() const
					{	return m_pChartCtrl->GetMinCountPerPage();	}
	virtual int	GetOnePacketSize()
					{	return m_pChartCtrl->GetOnePacketSize();	}
	virtual int	GetOnePageDataCountForScroll() const
					{	return m_pChartCtrl->GetOnePageDataCountForScroll();	}
	virtual int	GetRightMargin()
					{	return m_pChartCtrl->GetRightMargin();	}
	virtual int	GetShowCurrentPrice()
					{	return m_pChartCtrl->GetShowCurrentPrice();	}
	virtual int	GetShowTrendLineValue()
					{	return m_pChartCtrl->GetShowTrendLineValue();	}
	virtual int	GetSkinNumber() const
					{	return m_pChartCtrl->GetSkinNumber();	}
	virtual int	GetSliderMin( void)
					{	return m_pChartCtrl->GetSliderMin();	}
	virtual int	GetUserMargin( void) const
					{	return m_pChartCtrl->GetUserMargin();	}
	virtual int	GetViewDataIndex( CPoint ptMouse)
					{	return m_pChartCtrl->GetViewDataIndex( ptMouse);	}
	virtual int	GetViewEndDataIndex( const bool bData)
					{	return m_pChartCtrl->GetViewEndDataIndex( bData);	}
	virtual int	GetYScalePosition()
					{	return m_pChartCtrl->GetYScalePosition();	}
	virtual int	OnCreate( LPCREATESTRUCT lpCreateStruct)
					{	return m_pChartCtrl->OnCreate( lpCreateStruct);	}
	virtual int	SaveCurrentChartCfg()
					{	return m_pChartCtrl->SaveCurrentChartCfg();	}
	virtual int	SetChartData2( long pChartData, int nCount, int nType)
					{	return m_pChartCtrl->SetChartData2( pChartData, nCount, nType);	}

protected:
	virtual long	GetAllProperties2( long lpData)
					{	return m_pChartCtrl->GetAllProperties2( lpData);	}
	virtual long	GetBase( LPCTSTR szKey)
					{	return m_pChartCtrl->GetBase( szKey);	}
	virtual long	GetBasePropIf()
					{	return m_pChartCtrl->GetBasePropIf();	}
	virtual long	GetCodeFilterOfShareInfoItemHW( LPCTSTR p_szShareName)
					{	return 0;	}	// (2006/1/11 - Seung-Won, Bae) Hanwha Platform Only
	virtual long	GetOnePageDataCount()
					{	return m_pChartCtrl->GetOnePageDataCount();	}
	virtual long	GetStartPeriodOfPacket( LPCTSTR p_szPacketName)
					{	return m_pChartCtrl->GetStartPeriodOfPacket( p_szPacketName);	}

protected:
	virtual LPUNKNOWN	GetCtrlUnkownID( LPCTSTR szName)
					{	return m_pChartCtrl->GetCtrlUnkownID( szName);	}
	virtual LPUNKNOWN	GetDeskUnkownID()
					{	return m_pChartCtrl->GetDeskUnkownID();	}

protected:
	virtual LRESULT	OnAddIndicatorFromChartList( WPARAM wParam, LPARAM lParam)
					{	return m_pChartCtrl->OnAddIndicatorFromChartList( wParam, lParam);	}
	virtual LRESULT	OnAddIndicatorFromSystem( WPARAM wParam, LPARAM lParam)
					{	return m_pChartCtrl->OnAddIndicatorFromSystem( wParam, lParam);	}
	virtual LRESULT	OnAddIndicatorInNewPacket( WPARAM wParam, LPARAM lParam)
					{	return m_pChartCtrl->OnAddIndicatorInNewPacket( wParam, lParam);	}
//	virtual LRESULT	OnAnalysisFunction( WPARAM wParam, LPARAM lParam)
//					{	return m_pChartCtrl->OnAnalysisFunction( wParam, lParam);	}
	virtual LRESULT	OnCursorExchange( WPARAM wParam, LPARAM lParam)
					{	return m_pChartCtrl->OnCursorExchange( wParam, lParam);	}
	virtual LRESULT	OnGetHWDesignUI( WPARAM wParam, LPARAM lParam)
					{	return NULL;	}	// (2006/1/11 - Seung-Won, Bae) Hanwha Platform Only
	virtual LRESULT	OnGetRealTableManager( WPARAM wParam, LPARAM lParam)
					{	return NULL;	}	// (2006/1/11 - Seung-Won, Bae) Hanwha Platform Only
	virtual LRESULT	OnGetUserMargin( WPARAM wParam, LPARAM lParam)
					{	return m_pChartCtrl->OnGetUserMargin( wParam, lParam);	}
	virtual LRESULT	OnGraphMargin( WPARAM wParam, LPARAM lParam)
					{	return m_pChartCtrl->OnGraphMargin( wParam, lParam);	}
	virtual LRESULT	OnGraphSelectedIndex( WPARAM wParam, LPARAM lParam)
					{	return m_pChartCtrl->OnGraphSelectedIndex( wParam, lParam);	}
//	virtual LRESULT	OnGraphThickness( WPARAM wParam, LPARAM lParam)
//					{	return m_pChartCtrl->OnGraphThickness( wParam, lParam);	}
//	virtual LRESULT	OnGraphTypeandStyle( WPARAM wParam, LPARAM lParam)
//					{	return m_pChartCtrl->OnGraphTypeandStyle( wParam, lParam);	}
	virtual LRESULT	OnInitProperties( WPARAM wParam, LPARAM lParam)
					{	return m_pChartCtrl->OnInitProperties( wParam, lParam);	}
	virtual LRESULT	OnPPGFont( WPARAM wParam, LPARAM lParam)
					{	return m_pChartCtrl->OnPPGFont( wParam, lParam);	}
	virtual LRESULT	OnPPGProperties( WPARAM wParam, LPARAM lParam)
					{	return m_pChartCtrl->OnPPGProperties( wParam, lParam);	}
	virtual LRESULT	OnRealData( WPARAM wParam, LPARAM lParam)
					{	return FALSE;	}	// (2006/1/11 - Seung-Won, Bae) Hanwha Platform Only
	virtual LRESULT	OnRedrawFrSPDiscretion( WPARAM wParam, LPARAM lParam)
					{	return m_pChartCtrl->OnRedrawFrSPDiscretion( wParam, lParam);	}
//	virtual LRESULT	OnReleaseSeriesDrawFromMain( WPARAM wParam, LPARAM lParam)
//					{	return m_pChartCtrl->OnReleaseSeriesDrawFromMain( wParam, lParam);	}
	virtual LRESULT	OnSaveScrollInfo( WPARAM wParam, LPARAM lParam)
					{	return m_pChartCtrl->OnSaveScrollInfo( wParam, lParam);	}
	virtual LRESULT	OnScroll( WPARAM wParam, LPARAM lParam)
					{	return m_pChartCtrl->OnScroll( wParam, lParam);	}
	virtual LRESULT	OnSetEtc( WPARAM wParam, LPARAM lParam)
					{	return m_pChartCtrl->OnSetEtc( wParam, lParam);	}
//	virtual LRESULT	OnToolInner( WPARAM wParam, LPARAM lParam)
//					{	return m_pChartCtrl->OnToolInner( wParam, lParam);	}
	virtual LRESULT	RmsgFormStateChange( WPARAM wParam, LPARAM lParam)
					{	return m_pChartCtrl->RmsgFormStateChange( wParam, lParam);	}
	virtual LRESULT	SendEventMsgToDLL( UINT message, WPARAM wParam = NULL, LPARAM lParam = NULL)
					{	return m_pChartCtrl->SendEventMsgToDLL( message, wParam, lParam );	}
	virtual LRESULT	WindowProc( UINT message, WPARAM wParam, LPARAM lParam)
					{	return m_pChartCtrl->WindowProc( message, wParam, lParam);	}

protected:
	virtual OLE_COLOR	GetClrAnalysisTool()
					{	return m_pChartCtrl->GetClrAnalysisTool();	}
	virtual OLE_COLOR	GetClrBackBorder()
					{	return m_pChartCtrl->GetClrBackBorder();	}
	virtual OLE_COLOR	GetClrBackground()
					{	return m_pChartCtrl->GetClrBackground();	}
	virtual OLE_COLOR	GetClrBlock()
					{	return m_pChartCtrl->GetClrBlock();	}
	virtual OLE_COLOR	GetClrBlockBorder()
					{	return m_pChartCtrl->GetClrBlockBorder();	}
	virtual OLE_COLOR	GetClrGraphBase1()
					{	return m_pChartCtrl->GetClrGraphBase1();	}
	virtual OLE_COLOR	GetClrGraphBase2()
					{	return m_pChartCtrl->GetClrGraphBase2();	}
	virtual OLE_COLOR	GetClrGraphBase3()
					{	return m_pChartCtrl->GetClrGraphBase3();	}
	virtual OLE_COLOR	GetClrGraphBase4()
					{	return m_pChartCtrl->GetClrGraphBase4();	}
	virtual OLE_COLOR	GetClrGraphBase5()
					{	return m_pChartCtrl->GetClrGraphBase5();	}
	virtual OLE_COLOR	GetClrGraphBase6()
					{	return m_pChartCtrl->GetClrGraphBase6();	}
	virtual OLE_COLOR	GetClrInquiryWindow()
					{	return m_pChartCtrl->GetClrInquiryWindow();	}
	virtual OLE_COLOR	GetClrLattice()
					{	return m_pChartCtrl->GetClrLattice();	}
	virtual OLE_COLOR	GetClrScaleLine()
					{	return m_pChartCtrl->GetClrScaleLine();	}
	virtual OLE_COLOR	GetClrScaleLine2()
					{	return m_pChartCtrl->GetClrScaleLine2();	}
	virtual OLE_COLOR	GetClrScaleText()
					{	return m_pChartCtrl->GetClrScaleText();	}

protected:
	virtual short	ChangeChart( short nBlockRowPosition, short nBlockColumnPosition, LPCTSTR strGraphTitle, LPCTSTR strIndicatorName, LPCTSTR strPacketName, LPCTSTR strGraphType, LPCTSTR strGraphStyle, short nLineWidth, OLE_COLOR clrLineColor)
					{	return m_pChartCtrl->ChangeChart( nBlockRowPosition, nBlockColumnPosition, strGraphTitle, strIndicatorName, strPacketName, strGraphType, strGraphStyle, nLineWidth, clrLineColor);	}
	virtual short	ClearChartData( short nOption)
					{	return m_pChartCtrl->ClearChartData( nOption);	}
	virtual short	GetBongCountMargin( void) const
					{	return m_pChartCtrl->m_nBongCountMargin;	}
	virtual short	GetColumnBlockCount()
					{	return m_pChartCtrl->GetColumnBlockCount();	}
	virtual short	GetDataCount()
					{	return m_pChartCtrl->GetDataCount();	}
	virtual short	GetGraphRButtonType( void) const
					{	return m_pChartCtrl->m_nGraphRButtonType;	}
	virtual short	GetRowBlockCount()
					{	return m_pChartCtrl->GetRowBlockCount();	}
	virtual short	GetVertScalePosition( short p_nRowIndex, short p_nColumnIndex, short p_nVSGroupIndex)
					{	return m_pChartCtrl->GetVertScalePosition( p_nRowIndex, p_nColumnIndex, p_nVSGroupIndex);	}
	virtual short	SetChartData( long pChartData, short nCount, short nType)
					{	return m_pChartCtrl->SetChartData( pChartData, nCount, nType);	}
	virtual short	SetChartDataFromFile( LPCTSTR szFileName, short nType)
					{	return m_pChartCtrl->SetChartDataFromFile( szFileName, nType);	}
	virtual short	SetChartDataStr( LPCTSTR szChartData, short nCount, short nType)
					{	return m_pChartCtrl->SetChartDataStr( szChartData, nCount, nType);	}
	virtual short	SetGraphHide( short nRow, short nColumn, LPCTSTR strTitle, BOOL bHide)
					{	return m_pChartCtrl->SetGraphHide( nRow, nColumn, strTitle, bHide);	}
	virtual short	testGetPacketdatasize( LPCTSTR pIn)
					{	return m_pChartCtrl->testGetPacketdatasize( pIn);	}
	virtual short	ZoomToWithRightFix( short p_nZoomValue, BOOL p_bWithRightFix)
					{	return m_pChartCtrl->ZoomToWithRightFix( p_nZoomValue, p_bWithRightFix);	}

protected:
	virtual UINT	OnGetDlgCode()
					{	return m_pChartCtrl->OnGetDlgCode();	}
	virtual UINT	GetNumericalInquiryOption( void) const
					{	return m_pChartCtrl->GetNumericalInquiryOption();	}

protected:
	virtual void	AddandDeleteIndicator( LPCTSTR strIndicatorName)
					{	m_pChartCtrl->AddandDeleteIndicator( strIndicatorName);	}
	virtual void	AddGraph( short nRow, short nColumn, short nVertScaleGroup, LPCTSTR szGraphOption, BOOL bCreateBlock, BOOL bCreateVertScaleGroup, BOOL bUseAutoScale)
					{	m_pChartCtrl->AddGraph( nRow, nColumn, nVertScaleGroup, szGraphOption, bCreateBlock, bCreateVertScaleGroup, bUseAutoScale);	}
	virtual void	AddGraphAdapter( short nRow, short nColumn, short nVertScaleGroup, const char *szGraphOption, BOOL bCreateBlock, BOOL bCreateVertScaleGroup, BOOL bUseAutoScale)
					{	m_pChartCtrl->AddGraphAdapter( nRow, nColumn, nVertScaleGroup, szGraphOption, bCreateBlock, bCreateVertScaleGroup, bUseAutoScale);	}
	virtual void	AddPacket( LPCTSTR szPacketOption)
					{	m_pChartCtrl->AddPacket( szPacketOption);	}
	virtual void	AddPacketAdapter( const char *pcszPacketOption)
					{	m_pChartCtrl->AddPacketAdapter( pcszPacketOption);	}
	virtual void	AddRealItem( LPCTSTR szRealInfo)
					{	m_pChartCtrl->AddRealItem( szRealInfo);	}
	virtual void	BehaviorBlock( const long pBlockBehaviorData)
					{	m_pChartCtrl->BehaviorBlock( pBlockBehaviorData);	}
	virtual void	ChartEnvSetupOk()
					{	m_pChartCtrl->ChartEnvSetupOk();	}
	virtual void	ClearTimeMark()
					{	m_pChartCtrl->ClearTimeMark();	}
	virtual void	CreateBufferBitmap( CDC *pdc, const CRect &rcBounds)
					{	m_pChartCtrl->CreateBufferBitmap( pdc, rcBounds);	}
	virtual void	CreateToolBar()
					{	m_pChartCtrl->CreateToolBar();	}
	virtual void	DeleteZoomInOutInquiryDlg()
					{	m_pChartCtrl->DeleteZoomInOutInquiryDlg();	}
//	virtual void	DoPropExchange( CPropExchange *pPX)
//	virtual void	DragMouseZoom( CDC *pDC, CPoint point)
//					{	m_pChartCtrl->DragMouseZoom( pDC, point);	}
	// (2006/2/6 - Seung-Won, Bae) DragZoomIn for DragZoom AddIn
	virtual void	DragZoomIn( const CPoint &p_ptStart, const CPoint &p_ptEnd)
					{	m_pChartCtrl->DragZoomIn( p_ptStart, p_ptEnd);	}
	virtual void	EnableBoundMark( BOOL p_bEnable)
					{	m_pChartCtrl->EnableBoundMark( p_bEnable);	}
	virtual void	EnableReceivingTimeShare( BOOL p_bEnable)
					{	m_pChartCtrl->EnableReceivingTimeShare( p_bEnable);	}
	virtual void	FillInquiryByMouseMove()
					{	m_pChartCtrl->FillInquiryByMouseMove();	}
	virtual void	FireAddInEvent( LPCTSTR p_szAddInName, short p_nEventID, LPCTSTR p_szData, double p_dData, long p_lData)
					{	m_pChartCtrl->FireAddInEvent( p_szAddInName, p_nEventID, p_szData, p_dData, p_lData);	}
	virtual void	FireChangeOnePageDataCount( long nOnePageDataCount)
					{	m_pChartCtrl->FireChangeOnePageDataCount( nOnePageDataCount);	}
	virtual void	FireDataUpandDown( short nOption, LPCTSTR strStartDate, LPCTSTR strEndDate)
					{	m_pChartCtrl->FireDataUpandDown( nOption, strStartDate, strEndDate);	}
	virtual void	FireGetSettedChartInfoFromServer()
					{	m_pChartCtrl->FireGetSettedChartInfoFromServer();	}
	virtual void	FireRequestBongData( short nOption, short nTemp)
					{	m_pChartCtrl->FireRequestBongData( nOption, nTemp);	}
	virtual void	FireRequestMapLoad( short nOption, LPCTSTR strCode, LPCTSTR strDate)
					{	m_pChartCtrl->FireRequestMapLoad( nOption, strCode, strDate);	}
	virtual void	FireRequestNewPacket( short nOption, LPCTSTR strName, LPCTSTR strCode, LPCTSTR strDate)
					{	m_pChartCtrl->FireRequestNewPacket( nOption, strName, strCode, strDate);	}
	virtual void	FireRequestPacketData( LPCTSTR strDateType, LPCTSTR strStartDate, LPCTSTR strPacketNames)
					{	m_pChartCtrl->FireRequestPacketData( strDateType, strStartDate, strPacketNames);	}
	virtual void	FireRestore()
					{	m_pChartCtrl->FireRestore();	}
	virtual void	FireReturnOrgChart( long nOnePageDataCount)
					{	m_pChartCtrl->FireReturnOrgChart( nOnePageDataCount);	}
	virtual void	FireSaveSettedChartInfoToServer()
					{	m_pChartCtrl->FireSaveSettedChartInfoToServer();	}
	virtual void	FP_vSetInitFont()
					{	m_pChartCtrl->FP_vSetInitFont();	}
	virtual void	FV_vInitFont()
					{	m_pChartCtrl->FV_vInitFont();	}
//	virtual void	GetAnalysisToolData( CAnalysisToolData &analysisToolData)
//	virtual void	GetIndicatorDefaultData( CIndicatorDefaultData &indiDefaultData) const
	virtual void	GetListViewerData( BSTR *bstrTitle, BSTR *bstrHorzScale, BSTR *bstrPacketNamesPerBlock, BSTR *bstrPacketNames, BSTR *bstrSaveFilePath, BOOL *bViewPacketSequence, long *lpPacketList)
					{	m_pChartCtrl->GetListViewerData( bstrTitle, bstrHorzScale, bstrPacketNamesPerBlock, bstrPacketNames, bstrSaveFilePath, bViewPacketSequence, lpPacketList);	}
//	virtual void	GetStyleData( CStyleData &styleData) const
	virtual void	HideTooltip( void)
					{	m_pChartCtrl->HideTooltip();	}
//	virtual void	HideWindow()
//					{	m_pChartCtrl->HideWindow();	}
	virtual void	InvalidateChart()
					{	m_pChartCtrl->InvalidateChart();	}
	virtual void	InvalidateControl( LPCRECT lpRect = NULL, BOOL bErase = TRUE)
					{	m_pChartCtrl->InvalidateControl( lpRect, bErase);	}
	virtual void	InvalidateOCX( const bool bErase = true)
					{	m_pChartCtrl->InvalidateOCX( bErase);	}
	virtual void	InvalidateOleControl()
					{	m_pChartCtrl->InvalidateOleControl();	}
	virtual void	InvalidateRect( const CRect &region, const bool bErase)
					{	m_pChartCtrl->InvalidateRect( region, bErase);	}
	virtual void	MakeSaveShortKey( LPCTSTR szFileName, short nFlag)
					{	m_pChartCtrl->MakeSaveShortKey( szFileName, nFlag);	}
	virtual void	NeoAddInEvent( const char *p_szAddInName, short p_nEventID, const char *p_szData, double p_dData, long p_lData)
					{	m_pChartCtrl->NeoAddInEvent( p_szAddInName, p_nEventID, p_szData, p_dData, p_lData);	}
	virtual void	NeoChangeOnePageDataCount( int nOnePageDataCount)
					{	m_pChartCtrl->NeoChangeOnePageDataCount( nOnePageDataCount);	}
	virtual void	NeoFireDataUpandDown( int nOption)
					{	m_pChartCtrl->NeoFireDataUpandDown( nOption);	}
	virtual void	NeoFireGetSettedChartInfoFromServer()
					{	m_pChartCtrl->NeoFireGetSettedChartInfoFromServer();	}
	virtual void	NeoFireSaveSettedChartInfoToServer()
					{	m_pChartCtrl->NeoFireSaveSettedChartInfoToServer();	}
	virtual void	NeoRequestBongData( int nOption, int nTemp)
					{	m_pChartCtrl->NeoRequestBongData( nOption, nTemp);	}
	virtual void	NeoRequestMapLoad( int nOption, CString strCode, CString strDate)
					{	m_pChartCtrl->NeoRequestMapLoad( nOption, strCode, strDate);	}
	virtual void	NeoRequestNewPacket( int nOption, CString strName, CString strCode, CString strDate)
					{	m_pChartCtrl->NeoRequestNewPacket( nOption, strName, strCode, strDate);	}
	virtual void	NeoRequestPacketData( const CString &strDateType, const CString &strStartDate, const CString &strPacketNames)
					{	m_pChartCtrl->NeoRequestPacketData( strDateType, strStartDate, strPacketNames);	}
	virtual void	NeoRestore()
					{	m_pChartCtrl->NeoRestore();	}
	virtual void	NeoReturnOrgChart( const int nOnePageDataCount)
					{	m_pChartCtrl->NeoReturnOrgChart( nOnePageDataCount);	}
	virtual void	OnAidIndAbratio()
					{	m_pChartCtrl->OnAidIndAbratio();	}
	virtual void	OnAidIndAdline()
					{	m_pChartCtrl->OnAidIndAdline();	}
	virtual void	OnAidIndAdx()
					{	m_pChartCtrl->OnAidIndAdx();	}
	virtual void	OnAidIndAsi()
					{	m_pChartCtrl->OnAidIndAsi();	}
	virtual void	OnAidIndAtr()
					{	m_pChartCtrl->OnAidIndAtr();	}
	virtual void	OnAidIndBalance()
					{	m_pChartCtrl->OnAidIndBalance();	}
	virtual void	OnAidIndBandB()
					{	m_pChartCtrl->OnAidIndBandB();	}
	virtual void	OnAidIndBandwidth()
					{	m_pChartCtrl->OnAidIndBandwidth();	}
	virtual void	OnAidIndBinarywave()
					{	m_pChartCtrl->OnAidIndBinarywave();	}
	virtual void	OnAidIndBollinger()
					{	m_pChartCtrl->OnAidIndBollinger();	}
	virtual void	OnAidIndBox()
					{	m_pChartCtrl->OnAidIndBox();	}
	virtual void	OnAidIndBrokenline()
					{	m_pChartCtrl->OnAidIndBrokenline();	}
	virtual void	OnAidIndCci()
					{	m_pChartCtrl->OnAidIndCci();	}
	virtual void	OnAidIndCmf()
					{	m_pChartCtrl->OnAidIndCmf();	}
	virtual void	OnAidIndCo()
					{	m_pChartCtrl->OnAidIndCo();	}
	virtual void	OnAidIndCommissionuncollected()
					{	m_pChartCtrl->OnAidIndCommissionuncollected();	}
	virtual void	OnAidIndCustomerdeposition()
					{	m_pChartCtrl->OnAidIndCustomerdeposition();	}
	virtual void	OnAidIndDaegi()
					{	m_pChartCtrl->OnAidIndDaegi();	}
	virtual void	OnAidIndDemark()
					{	m_pChartCtrl->OnAidIndDemark();	}
	virtual void	OnAidIndDisparity()
					{	m_pChartCtrl->OnAidIndDisparity();	}
	virtual void	OnAidIndDmi()
					{	m_pChartCtrl->OnAidIndDmi();	}
	virtual void	OnAidIndDpo()
					{	m_pChartCtrl->OnAidIndDpo();	}
	virtual void	OnAidIndEmv()
					{	m_pChartCtrl->OnAidIndEmv();	}
	virtual void	OnAidIndEnergy()
					{	m_pChartCtrl->OnAidIndEnergy();	}
	virtual void	OnAidIndEnvelope()
					{	m_pChartCtrl->OnAidIndEnvelope();	}
	virtual void	OnAidIndEom()
					{	m_pChartCtrl->OnAidIndEom();	}
	virtual void	OnAidIndFinancingbalance()
					{	m_pChartCtrl->OnAidIndFinancingbalance();	}
	virtual void	OnAidIndForeignerjibunyul()
					{	m_pChartCtrl->OnAidIndForeignerjibunyul();	}
	virtual void	OnAidIndForeignerpurebuy()
					{	m_pChartCtrl->OnAidIndForeignerpurebuy();	}
	virtual void	OnAidIndFutureoptionincomplete()
					{	m_pChartCtrl->OnAidIndFutureoptionincomplete();	}
	virtual void	OnAidIndImi()
					{	m_pChartCtrl->OnAidIndImi();	}
	virtual void	OnAidIndIndustrypriceindex()
					{	m_pChartCtrl->OnAidIndIndustrypriceindex();	}
	virtual void	OnAidIndInvestorForeigner()
					{	m_pChartCtrl->OnAidIndInvestorForeigner();	}
	virtual void	OnAidIndInvestorOrg()
					{	m_pChartCtrl->OnAidIndInvestorOrg();	}
	virtual void	OnAidIndInvestorPerson()
					{	m_pChartCtrl->OnAidIndInvestorPerson();	}
	virtual void	OnAidIndLrs()
					{	m_pChartCtrl->OnAidIndLrs();	}
	virtual void	OnAidIndMac()
					{	m_pChartCtrl->OnAidIndMac();	}
	virtual void	OnAidIndMacd()
					{	m_pChartCtrl->OnAidIndMacd();	}
	virtual void	OnAidIndMacdosc()
					{	m_pChartCtrl->OnAidIndMacdosc();	}
	virtual void	OnAidIndMaosc()
					{	m_pChartCtrl->OnAidIndMaosc();	}
	virtual void	OnAidIndMarketpriceindex()
					{	m_pChartCtrl->OnAidIndMarketpriceindex();	}
	virtual void	OnAidIndMfi()
					{	m_pChartCtrl->OnAidIndMfi();	}
	virtual void	OnAidIndMi()
					{	m_pChartCtrl->OnAidIndMi();	}
	virtual void	OnAidIndMomentum()
					{	m_pChartCtrl->OnAidIndMomentum();	}
	virtual void	OnAidIndNco()
					{	m_pChartCtrl->OnAidIndNco();	}
	virtual void	OnAidIndNetchart()
					{	m_pChartCtrl->OnAidIndNetchart();	}
	virtual void	OnAidIndNewpsychology()
					{	m_pChartCtrl->OnAidIndNewpsychology();	}
	virtual void	OnAidIndNvi()
					{	m_pChartCtrl->OnAidIndNvi();	}
	virtual void	OnAidIndObv()
					{	m_pChartCtrl->OnAidIndObv();	}
	virtual void	OnAidIndOcma()
					{	m_pChartCtrl->OnAidIndOcma();	}
	virtual void	OnAidIndOscp()
					{	m_pChartCtrl->OnAidIndOscp();	}
	virtual void	OnAidIndOscv()
					{	m_pChartCtrl->OnAidIndOscv();	}
	virtual void	OnAidIndParabolic()
					{	m_pChartCtrl->OnAidIndParabolic();	}
	virtual void	OnAidIndPivot()
					{	m_pChartCtrl->OnAidIndPivot();	}
	virtual void	OnAidIndPnf()
					{	m_pChartCtrl->OnAidIndPnf();	}
	virtual void	OnAidIndPsychology()
					{	m_pChartCtrl->OnAidIndPsychology();	}
	virtual void	OnAidIndPvi()
					{	m_pChartCtrl->OnAidIndPvi();	}
	virtual void	OnAidIndPvt()
					{	m_pChartCtrl->OnAidIndPvt();	}
	virtual void	OnAidIndRainbow()
					{	m_pChartCtrl->OnAidIndRainbow();	}
	virtual void	OnAidIndReverseclock()
					{	m_pChartCtrl->OnAidIndReverseclock();	}
	virtual void	OnAidIndRoc()
					{	m_pChartCtrl->OnAidIndRoc();	}
	virtual void	OnAidIndRsi()
					{	m_pChartCtrl->OnAidIndRsi();	}
	virtual void	OnAidIndRsquared()
					{	m_pChartCtrl->OnAidIndRsquared();	}
	virtual void	OnAidIndSigma()
					{	m_pChartCtrl->OnAidIndSigma();	}
	virtual void	OnAidIndSmi()
					{	m_pChartCtrl->OnAidIndSmi();	}
	virtual void	OnAidIndSonar()
					{	m_pChartCtrl->OnAidIndSonar();	}
	virtual void	OnAidIndStandarddeviation()
					{	m_pChartCtrl->OnAidIndStandarddeviation();	}
	virtual void	OnAidIndStandarderror()
					{	m_pChartCtrl->OnAidIndStandarderror();	}
	virtual void	OnAidIndStochasticsfast()
					{	m_pChartCtrl->OnAidIndStochasticsfast();	}
	virtual void	OnAidIndStochasticsfast2()
					{	m_pChartCtrl->OnAidIndStochasticsfast2();	}
	virtual void	OnAidIndStochasticsfast3()
					{	m_pChartCtrl->OnAidIndStochasticsfast3();	}
	virtual void	OnAidIndStochasticsosc()
					{	m_pChartCtrl->OnAidIndStochasticsosc();	}
	virtual void	OnAidIndStochasticsosc2()
					{	m_pChartCtrl->OnAidIndStochasticsosc2();	}
	virtual void	OnAidIndStochasticsosc3()
					{	m_pChartCtrl->OnAidIndStochasticsosc3();	}
	virtual void	OnAidIndStochasticsslow()
					{	m_pChartCtrl->OnAidIndStochasticsslow();	}
	virtual void	OnAidIndStochasticsslow2()
					{	m_pChartCtrl->OnAidIndStochasticsslow2();	}
	virtual void	OnAidIndStochasticsslow3()
					{	m_pChartCtrl->OnAidIndStochasticsslow3();	}
	virtual void	OnAidIndSwingindex()
					{	m_pChartCtrl->OnAidIndSwingindex();	}
	virtual void	OnAidIndThreeline()
					{	m_pChartCtrl->OnAidIndThreeline();	}
	virtual void	OnAidIndTradeyesukm()
					{	m_pChartCtrl->OnAidIndTradeyesukm();	}
	virtual void	OnAidIndTrix()
					{	m_pChartCtrl->OnAidIndTrix();	}
	virtual void	OnAidIndTsf()
					{	m_pChartCtrl->OnAidIndTsf();	}
	virtual void	OnAidIndTsfosc()
					{	m_pChartCtrl->OnAidIndTsfosc();	}
	virtual void	OnAidIndTsi()
					{	m_pChartCtrl->OnAidIndTsi();	}
	virtual void	OnAidIndVhf()
					{	m_pChartCtrl->OnAidIndVhf();	}
	virtual void	OnAidIndVr()
					{	m_pChartCtrl->OnAidIndVr();	}
	virtual void	OnAidIndVroc()
					{	m_pChartCtrl->OnAidIndVroc();	}
	virtual void	OnAidIndWeightedclose()
					{	m_pChartCtrl->OnAidIndWeightedclose();	}
	virtual void	OnAidIndWill()
					{	m_pChartCtrl->OnAidIndWill();	}
	virtual void	OnAidIndWilliam()
					{	m_pChartCtrl->OnAidIndWilliam();	}
	virtual void	OnAidIndZz()
					{	m_pChartCtrl->OnAidIndZz();	}
	virtual void	OnbAddGraphMenuShowChanged()
					{	m_pChartCtrl->OnbAddGraphMenuShowChanged();	}
	virtual void	OnBBlockMoveDeleteChanged()
					{	m_pChartCtrl->OnBBlockMoveDeleteChanged();	}
	virtual void	OnBControlBlockSizeChanged()
					{	m_pChartCtrl->OnBControlBlockSizeChanged();	}
	virtual void	OnBDoesUseSkinColorChanged()
					{	m_pChartCtrl->OnBDoesUseSkinColorChanged();	}
	virtual void	OnBFireRealEventChanged()
					{	m_pChartCtrl->OnBFireRealEventChanged();	}
	virtual void	OnbGraphDragDropChanged()
					{	m_pChartCtrl->OnbGraphDragDropChanged();	}
	virtual void	OnbHideChanged()
					{	m_pChartCtrl->OnbHideChanged();	}
	virtual void	OnbIsInputChanged()
					{	m_pChartCtrl->OnbIsInputChanged();	}
	virtual void	OnbNormalizedMinuteTimeRule()
					{	m_pChartCtrl->OnbNormalizedMinuteTimeRule();	}
	virtual void	OnBOcxAreaNumericalInquiryChanged()
					{	m_pChartCtrl->OnBOcxAreaNumericalInquiryChanged();	}
	virtual void	OnBOnePageDataCountInTRCountChanged()
					{	m_pChartCtrl->OnBOnePageDataCountInTRCountChanged();	}
	virtual void	OnBRedrawAfterUpdatedAllPacketsChanged()
					{	m_pChartCtrl->OnBRedrawAfterUpdatedAllPacketsChanged();	}
	virtual void	OnBSelectGraphChanged()
					{	m_pChartCtrl->OnBSelectGraphChanged();	}
	virtual void	OnBSelectTitleChanged()
					{	m_pChartCtrl->OnBSelectTitleChanged();	}
	virtual void	OnBShowPriceRateInquiryChanged()
					{	m_pChartCtrl->OnBShowPriceRateInquiryChanged();	}
	virtual void	OnBShowWarningMessageBoxChanged()
					{	m_pChartCtrl->OnBShowWarningMessageBoxChanged();	}
	virtual void	OnBUseConditiontoTitleChanged()
					{	m_pChartCtrl->OnBUseConditiontoTitleChanged();	}
	virtual void	OnBUseDatatoTitleChanged()
					{	m_pChartCtrl->OnBUseDatatoTitleChanged();	}
	virtual void	OnBUseInquiryDlgWhenUseCrossLineChanged()
					{	m_pChartCtrl->OnBUseInquiryDlgWhenUseCrossLineChanged();	}
	virtual void	OnbUseRealDataChanged()
					{	m_pChartCtrl->OnbUseRealDataChanged();	}
	virtual void	OnBUseScaleRButtonMenuChanged()
					{	m_pChartCtrl->OnBUseScaleRButtonMenuChanged();	}
	virtual void	OnBUseShortKeyChanged()
					{	m_pChartCtrl->OnBUseShortKeyChanged();	}
	virtual void	OnbWholeViewChanged()
					{	m_pChartCtrl->OnbWholeViewChanged();	}
	virtual void	OnCaptureChanged( CWnd *pWnd)
					{	m_pChartCtrl->OnCaptureChanged( pWnd);	}
//	virtual void	OnChartList( int nOption)
//					{	m_pChartCtrl->OnChartList( nOption);	}
//	virtual void	OnCircle()
//					{	m_pChartCtrl->OnCircle();	}
//	virtual void	OnConfigIndicator()
//					{	m_pChartCtrl->OnConfigIndicator();	}
	virtual void	OnContextFont()
					{	m_pChartCtrl->OnContextFont();	}
	virtual void	OnContextInit()
					{	m_pChartCtrl->OnContextInit();	}
	virtual void	OnContextMenu( CWnd *pWnd, CPoint point)
					{	m_pChartCtrl->OnContextMenu( pWnd, point);	}
	virtual void	OnContextProperties()
					{	m_pChartCtrl->OnContextProperties();	}
	virtual void	OnContextRestart()
					{	m_pChartCtrl->OnContextRestart();	}
	virtual void	OnDataListViewer( int nOption)
					{	m_pChartCtrl->OnDataListViewer( nOption);	}
//	virtual void	OnDelDriftline()
//					{	m_pChartCtrl->OnDelDriftline();	}
	virtual void	OnDeleteBlock()
					{	m_pChartCtrl->OnDeleteBlock();	}
//	virtual void	OnDeleteIndicator()
//					{	m_pChartCtrl->OnDeleteIndicator();	}
	virtual void	OnDraw( CDC *pdc, const CRect &rcBounds, const CRect &rcInvalid)
					{	m_pChartCtrl->OnDraw( pdc, rcBounds, rcInvalid);	}
//	virtual void	OnDrawDriftline()
//					{	m_pChartCtrl->OnDrawDriftline();	}
	virtual void	OnDrawForPrint( CDC &memoryDC, CBitmap &bufferBitmap, CRect rcSource, BOOL bScreenColor = TRUE)
					{	m_pChartCtrl->OnDrawForPrint( memoryDC, bufferBitmap, rcSource, bScreenColor);	}
	virtual void	OnDrawStockItemCode( CDC &memoryDC, CRect &rect)
					{	m_pChartCtrl->OnDrawStockItemCode( memoryDC, rect);	}
	virtual void	OnDrawUsingOptimizingcolor( CDC &memoryDC, CBitmap &bufferBitmap, CRect rect)
					{	m_pChartCtrl->OnDrawUsingOptimizingcolor( memoryDC, bufferBitmap, rect);	}
	virtual void	OnDrawUsingScreencolor( CDC &memoryDC, CBitmap &bufferBitmap, CRect rect)
					{	m_pChartCtrl->OnDrawUsingScreencolor( memoryDC, bufferBitmap, rect);	}
	virtual void	OnFullMinMax( int nOption)
					{	m_pChartCtrl->OnFullMinMax( nOption);	}
	virtual void	OnFuncDataList()
					{	m_pChartCtrl->OnFuncDataList();	}
//	virtual void	OnFuncEnvironment()
//					{	m_pChartCtrl->OnFuncEnvironment();	}
//	virtual void	OnFuncIndicatorCfg()
//					{	m_pChartCtrl->OnFuncIndicatorCfg();	}
//	virtual void	OnFuncIndicatorList()
//					{	m_pChartCtrl->OnFuncIndicatorList();	}
//	virtual void	OnInquiryValue( const int nOption, const bool bIsReplay)
//					{	m_pChartCtrl->OnInquiryValue( nOption, bIsReplay);	}
	virtual void	OnInvalidate()
					{	m_pChartCtrl->OnInvalidate();	}
	virtual void	OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags)
					{	m_pChartCtrl->OnKeyDown( nChar, nRepCnt, nFlags);	}
	virtual void	OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags)
					{	m_pChartCtrl->OnKeyUp( nChar, nRepCnt, nFlags);	}
	virtual void	OnLButtonDblClk( UINT nFlags, CPoint point)
					{	m_pChartCtrl->OnLButtonDblClk( nFlags, point);	}
	virtual void	OnLButtonDown( UINT nFlags, CPoint point)
					{	m_pChartCtrl->OnLButtonDown( nFlags, point);	}
	virtual void	OnLButtonUp( UINT nFlags, CPoint point)
					{	m_pChartCtrl->OnLButtonUp( nFlags, point);	}
	virtual void	OnMenuCrossline()
					{	m_pChartCtrl->OnMenuCrossline();	}
	virtual void	OnMenuDatawindow()
					{	m_pChartCtrl->OnMenuDatawindow();	}
//	virtual void	OnMouseMove( const CPoint &point)
//					{	m_pChartCtrl->OnMouseMove( point);	}
//	virtual void	OnMouseMove( UINT nFlags, CPoint point)
//					{	m_pChartCtrl->OnMouseMove( nFlags, point);	}
//	virtual void	OnMouseMove_DesignMode( const UINT nFlags, const CPoint &point)
//					{	m_pChartCtrl->OnMouseMove_DesignMode( nFlags, point);	}
	virtual void	OnnBlockBorderThicknessChanged()
					{	m_pChartCtrl->OnnBlockBorderThicknessChanged();	}
	virtual void	OnnBlockColumnCountChanged()
					{	m_pChartCtrl->OnnBlockColumnCountChanged();	}
	virtual void	OnnBlockCompartChanged()
					{	m_pChartCtrl->OnnBlockCompartChanged();	}
	virtual void	OnnBlockGapChanged()
					{	m_pChartCtrl->OnnBlockGapChanged();	}
	virtual void	OnnBlockRowCountChanged()
					{	m_pChartCtrl->OnnBlockRowCountChanged();	}
	virtual void	OnnBlockTypeChanged()
					{	m_pChartCtrl->OnnBlockTypeChanged();	}
	virtual void	OnNBongCountMarginChanged()
					{	m_pChartCtrl->OnNBongCountMarginChanged();	}
	virtual void	OnnBorderTypeChanged()
					{	m_pChartCtrl->OnnBorderTypeChanged();	}
	virtual void	OnnBottomMarginChanged()
					{	m_pChartCtrl->OnnBottomMarginChanged();	}
	virtual void	OnNColumnDoubleSizeBlockChanged()
					{	m_pChartCtrl->OnNColumnDoubleSizeBlockChanged();	}
	virtual void	OnNGraphRButtonTypeChanged()
					{	m_pChartCtrl->OnNGraphRButtonTypeChanged();	}
	virtual void	OnNHoldingPacketSizeChanged()
					{	m_pChartCtrl->OnNHoldingPacketSizeChanged();	}
	virtual void	OnNHorzGapChanged()
					{	m_pChartCtrl->OnNHorzGapChanged();	}
	virtual void	OnNInquiryTypeChanged()
					{	m_pChartCtrl->OnNInquiryTypeChanged();	}
	virtual void	OnnLeftMarginChanged()
					{	m_pChartCtrl->OnnLeftMarginChanged();	}
	virtual void	OnNMarginBorderTypeChanged()
					{	m_pChartCtrl->OnNMarginBorderTypeChanged();	}
	virtual void	OnNMinDataCountOnOnePageChanged()
					{	m_pChartCtrl->OnNMinDataCountOnOnePageChanged();	}
	virtual void	OnNNumericalInquiryOptionChanged()
					{	m_pChartCtrl->OnNNumericalInquiryOptionChanged();	}
	virtual void	OnnOnePageDataCountChanged()
					{	m_pChartCtrl->OnnOnePageDataCountChanged();	}
	virtual void	OnNRButtonMenuTypeChanged()
					{	m_pChartCtrl->OnNRButtonMenuTypeChanged();	}
	virtual void	OnnRightMarginChanged()
					{	m_pChartCtrl->OnnRightMarginChanged();	}
	virtual void	OnNRowDoubleSizeBlockChanged()
					{	m_pChartCtrl->OnNRowDoubleSizeBlockChanged();	}
	virtual void	OnnScaleLineTypeChanged()
					{	m_pChartCtrl->OnnScaleLineTypeChanged();	}
	virtual void	OnnTopMarginChanged()
					{	m_pChartCtrl->OnnTopMarginChanged();	}
	virtual void	OnNUseInquiryWithCrosslineChanged()
					{	m_pChartCtrl->OnNUseInquiryWithCrosslineChanged();	}
	virtual void	OnnUserBlockAddCountChanged()
					{	m_pChartCtrl->OnnUserBlockAddCountChanged();	}
	virtual void	OnNVertGapChanged()
					{	m_pChartCtrl->OnNVertGapChanged();	}
	virtual void	OnPacketTRData()
					{	m_pChartCtrl->OnPacketTRData();	}
	virtual void	OnPacketTRData( const CString &strTRPacketNames, const bool bIsResert)
					{	m_pChartCtrl->OnPacketTRData( strTRPacketNames, bIsResert);	}
//	virtual void	OnPatternAnalysis( const BOOL bIsShow = TRUE)
//					{	m_pChartCtrl->OnPatternAnalysis( bIsShow);	}
	virtual void	OnPricechartMinmax()
					{	m_pChartCtrl->OnPricechartMinmax();	}
	virtual void	OnPrint()
					{	m_pChartCtrl->OnPrint();	}
	virtual void	OnRButtonDown( UINT nFlags, CPoint point)
					{	m_pChartCtrl->OnRButtonDown( nFlags, point);	}
	virtual void	OnRButtonUp( UINT nFlags, CPoint point)
					{	m_pChartCtrl->OnRButtonUp( nFlags, point);	}
//	virtual void	OnReleaseSeriesdraw()
//					{	m_pChartCtrl->OnReleaseSeriesdraw();	}
	virtual void	OnResetState()
					{	m_pChartCtrl->OnResetState();	}
	virtual void	OnRestore()
					{	m_pChartCtrl->OnRestore();	}
	virtual void	OnRestore( int nOption)
					{	m_pChartCtrl->OnRestore( nOption);	}
	virtual void	OnRestoreDisplayDataCount()
					{	m_pChartCtrl->OnRestoreDisplayDataCount();	}
	virtual void	OnReturnOrg()
					{	m_pChartCtrl->OnReturnOrg();	}
	virtual void	OnReturnOrg( const bool bIsMessageBox)
					{	m_pChartCtrl->OnReturnOrg( bIsMessageBox);	}
	virtual void	OnReturnOrgDisplay()
					{	m_pChartCtrl->OnReturnOrgDisplay();	}
	virtual void	OnReverseYscale()
					{	m_pChartCtrl->OnReverseYscale();	}
	virtual void	OnSaveScreen()
					{	m_pChartCtrl->OnSaveScreen();	}
//	virtual void	OnScaleSettingMessage( const ETCMESSAGETYPE eEtcMessageType, const int nData)
//	virtual void	OnScaleX()
//					{	m_pChartCtrl->OnScaleX();	}
//	virtual void	OnScaleXHide()
//					{	m_pChartCtrl->OnScaleXHide();	}
	virtual void	OnScreenMinmax()
					{	m_pChartCtrl->OnScreenMinmax();	}
	virtual void	OnSetFocus( CWnd *pOldWnd)
					{	m_pChartCtrl->OnSetFocus( pOldWnd);	}
	virtual void	OnShowToolbar()
					{	m_pChartCtrl->OnShowToolbar();	}
	virtual void	OnSimpleDetailDataview()
					{	m_pChartCtrl->OnSimpleDetailDataview();	}
	virtual void	OnSimpleIndiInitialize()
					{	m_pChartCtrl->OnSimpleIndiInitialize();	}
	virtual void	OnSize( UINT nType, int cx, int cy)
					{	m_pChartCtrl->OnSize( nType, cx, cy);	}
//	virtual void	OnSquare()
//					{	m_pChartCtrl->OnSquare();	}
	virtual void	OnstrChartListChanged()
					{	m_pChartCtrl->OnstrChartListChanged();	}
	virtual void	OnStrGlobalSharedListChanged()
					{	m_pChartCtrl->OnStrGlobalSharedListChanged();	}
	virtual void	OnstrLocalSharedListChanged()
					{	m_pChartCtrl->OnstrLocalSharedListChanged();	}
	virtual void	OnStrMapNameChanged()
					{	m_pChartCtrl->OnStrMapNameChanged();	}
	virtual void	OnstrPacketOptionChanged()
					{	m_pChartCtrl->OnstrPacketOptionChanged();	}
	virtual void	OnstrRealListChanged()
					{	m_pChartCtrl->OnstrRealListChanged();	}
	virtual void	OnStrShareInfoListHWChanged()
					{}	// (2006/1/11 - Seung-Won, Bae) Hanwha Platform Only
	virtual void	OnstrToolbarOptionChanged()
					{	m_pChartCtrl->OnstrToolbarOptionChanged();	}
	virtual void	OnstrUserEnvironmentFileNameChanged()
					{	m_pChartCtrl->OnstrUserEnvironmentFileNameChanged();	}
	virtual void	OnstrUserIndicatorListFileNameChanged()
					{	m_pChartCtrl->OnstrUserIndicatorListFileNameChanged();	}
	virtual void	OnstrUserSeriesListFileNameChanged()
					{	m_pChartCtrl->OnstrUserSeriesListFileNameChanged();	}
//	virtual void	OnSymbol( CToolOptionInfo::TOOLOPTION nToolOption, int nOption)
	virtual void	OnSzBasePacketNameChanged()
					{	m_pChartCtrl->OnSzBasePacketNameChanged();	}
	virtual void	OnTestDatawindow()
					{	m_pChartCtrl->OnTestDatawindow();	}
	virtual void	OnTimer( UINT nIDEvent)
					{	m_pChartCtrl->OnTimer( nIDEvent);	}
	virtual void	OnToExcel()
					{	m_pChartCtrl->OnToExcel();	}
//	virtual void	OnTriangle()
//					{	m_pChartCtrl->OnTriangle();	}
	virtual void	OnUpdateAidIndAbratio( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndAbratio( pCmdUI);	}
	virtual void	OnUpdateAidIndAdline( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndAdline( pCmdUI);	}
	virtual void	OnUpdateAidIndAdx( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndAdx( pCmdUI);	}
	virtual void	OnUpdateAidIndAsi( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndAsi( pCmdUI);	}
	virtual void	OnUpdateAidIndAtr( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndAtr( pCmdUI);	}
	virtual void	OnUpdateAidIndBalance( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndBalance( pCmdUI);	}
	virtual void	OnUpdateAidIndBandB( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndBandB( pCmdUI);	}
	virtual void	OnUpdateAidIndBandwidth( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndBandwidth( pCmdUI);	}
	virtual void	OnUpdateAidIndBinarywave( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndBinarywave( pCmdUI);	}
	virtual void	OnUpdateAidIndBollinger( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndBollinger( pCmdUI);	}
	virtual void	OnUpdateAidIndBox( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndBox( pCmdUI);	}
	virtual void	OnUpdateAidIndBrokenline( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndBrokenline( pCmdUI);	}
	virtual void	OnUpdateAidIndCci( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndCci( pCmdUI);	}
	virtual void	OnUpdateAidIndCmf( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndCmf( pCmdUI);	}
	virtual void	OnUpdateAidIndCo( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndCo( pCmdUI);	}
	virtual void	OnUpdateAidIndDaegi( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndDaegi( pCmdUI);	}
	virtual void	OnUpdateAidIndDemark( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndDemark( pCmdUI);	}
	virtual void	OnUpdateAidIndDisparity( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndDisparity( pCmdUI);	}
	virtual void	OnUpdateAidIndDmi( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndDmi( pCmdUI);	}
	virtual void	OnUpdateAidIndDpo( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndDpo( pCmdUI);	}
	virtual void	OnUpdateAidIndEmv( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndEmv( pCmdUI);	}
	virtual void	OnUpdateAidIndEnergy( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndEnergy( pCmdUI);	}
	virtual void	OnUpdateAidIndEnvelope( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndEnvelope( pCmdUI);	}
	virtual void	OnUpdateAidIndEom( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndEom( pCmdUI);	}
	virtual void	OnUpdateAidIndImi( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndImi( pCmdUI);	}
	virtual void	OnUpdateAidIndLrs( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndLrs( pCmdUI);	}
	virtual void	OnUpdateAidIndMac( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndMac( pCmdUI);	}
	virtual void	OnUpdateAidIndMacd( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndMacd( pCmdUI);	}
	virtual void	OnUpdateAidIndMacdosc( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndMacdosc( pCmdUI);	}
	virtual void	OnUpdateAidIndMaosc( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndMaosc( pCmdUI);	}
	virtual void	OnUpdateAidIndMfi( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndMfi( pCmdUI);	}
	virtual void	OnUpdateAidIndMi( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndMi( pCmdUI);	}
	virtual void	OnUpdateAidIndMomentum( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndMomentum( pCmdUI);	}
	virtual void	OnUpdateAidIndNco( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndNco( pCmdUI);	}
	virtual void	OnUpdateAidIndNetchart( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndNetchart( pCmdUI);	}
	virtual void	OnUpdateAidIndNewpsychology( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndNewpsychology( pCmdUI);	}
	virtual void	OnUpdateAidIndNvi( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndNvi( pCmdUI);	}
	virtual void	OnUpdateAidIndObv( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndObv( pCmdUI);	}
	virtual void	OnUpdateAidIndOcma( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndOcma( pCmdUI);	}
	virtual void	OnUpdateAidIndOscp( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndOscp( pCmdUI);	}
	virtual void	OnUpdateAidIndOscv( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndOscv( pCmdUI);	}
	virtual void	OnUpdateAidIndParabolic( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndParabolic( pCmdUI);	}
	virtual void	OnUpdateAidIndPivot( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndPivot( pCmdUI);	}
	virtual void	OnUpdateAidIndPsychology( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndPsychology( pCmdUI);	}
	virtual void	OnUpdateAidIndPvi( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndPvi( pCmdUI);	}
	virtual void	OnUpdateAidIndPvt( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndPvt( pCmdUI);	}
	virtual void	OnUpdateAidIndRoc( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndRoc( pCmdUI);	}
	virtual void	OnUpdateAidIndRsi( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndRsi( pCmdUI);	}
	virtual void	OnUpdateAidIndRsquared( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndRsquared( pCmdUI);	}
	virtual void	OnUpdateAidIndSigma( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndSigma( pCmdUI);	}
	virtual void	OnUpdateAidIndSmi( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndSmi( pCmdUI);	}
	virtual void	OnUpdateAidIndSonar( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndSonar( pCmdUI);	}
	virtual void	OnUpdateAidIndStandarddeviation( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndStandarddeviation( pCmdUI);	}
	virtual void	OnUpdateAidIndStandarderror( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndStandarderror( pCmdUI);	}
	virtual void	OnUpdateAidIndStochasticsfast( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndStochasticsfast( pCmdUI);	}
	virtual void	OnUpdateAidIndStochasticsfast2( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndStochasticsfast2( pCmdUI);	}
	virtual void	OnUpdateAidIndStochasticsfast3( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndStochasticsfast3( pCmdUI);	}
	virtual void	OnUpdateAidIndStochasticsosc( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndStochasticsosc( pCmdUI);	}
	virtual void	OnUpdateAidIndStochasticsosc2( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndStochasticsosc2( pCmdUI);	}
	virtual void	OnUpdateAidIndStochasticsosc3( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndStochasticsosc3( pCmdUI);	}
	virtual void	OnUpdateAidIndStochasticsslow( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndStochasticsslow( pCmdUI);	}
	virtual void	OnUpdateAidIndStochasticsslow2( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndStochasticsslow2( pCmdUI);	}
	virtual void	OnUpdateAidIndStochasticsslow3( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndStochasticsslow3( pCmdUI);	}
	virtual void	OnUpdateAidIndSwingindex( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndSwingindex( pCmdUI);	}
	virtual void	OnUpdateAidIndTrix( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndTrix( pCmdUI);	}
	virtual void	OnUpdateAidIndTsf( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndTsf( pCmdUI);	}
	virtual void	OnUpdateAidIndTsfosc( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndTsfosc( pCmdUI);	}
	virtual void	OnUpdateAidIndTsi( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndTsi( pCmdUI);	}
	virtual void	OnUpdateAidIndVhf( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndVhf( pCmdUI);	}
	virtual void	OnUpdateAidIndVr( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndVr( pCmdUI);	}
	virtual void	OnUpdateAidIndVroc( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndVroc( pCmdUI);	}
	virtual void	OnUpdateAidIndWeightedclose( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndWeightedclose( pCmdUI);	}
	virtual void	OnUpdateAidIndWill( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndWill( pCmdUI);	}
	virtual void	OnUpdateAidIndWilliam( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndWilliam( pCmdUI);	}
	virtual void	OnUpdateAidIndZz( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateAidIndZz( pCmdUI);	}
//	virtual void	OnUpdateCircle( CCmdUI *pCmdUI)
//					{	m_pChartCtrl->OnUpdateCircle( pCmdUI);	}
	virtual void	OnUpdateContextInit( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateContextInit( pCmdUI);	}
	virtual void	OnUpdateContextProperties( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateContextProperties( pCmdUI);	}
	virtual void	OnUpdateContextRestart( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateContextRestart( pCmdUI);	}
	virtual void	OnUpdateCrossFollowClose( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateCrossFollowClose( pCmdUI);	}
//	virtual void	OnUpdateDeleteIndicator( CCmdUI *pCmdUI)
//					{	m_pChartCtrl->OnUpdateDeleteIndicator( pCmdUI);	}
	virtual void	OnUpdateFuncEnvironment( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateFuncEnvironment( pCmdUI);	}
	virtual void	OnUpdateMenuCrossline( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateMenuCrossline( pCmdUI);	}
	virtual void	OnUpdateMenuDatawindow( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateMenuDatawindow( pCmdUI);	}
	virtual void	OnUpdatePricechartMinmax( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdatePricechartMinmax( pCmdUI);	}
	virtual void	OnUpdateReverseYscale( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateReverseYscale( pCmdUI);	}
	virtual void	OnUpdateScreenMinmax( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateScreenMinmax( pCmdUI);	}
	virtual void	OnUpdateShowToolbar( CCmdUI *pCmdUI)
					{	m_pChartCtrl->OnUpdateShowToolbar( pCmdUI);	}
//	virtual void	OnUpdateSquare( CCmdUI *pCmdUI)
//					{	m_pChartCtrl->OnUpdateSquare( pCmdUI);	}
//	virtual void	OnUpdateTriangle( CCmdUI *pCmdUI)
//					{	m_pChartCtrl->OnUpdateTriangle( pCmdUI);	}
	virtual void	OnWindowPosChanged( WINDOWPOS *lpwndpos)
					{	m_pChartCtrl->OnWindowPosChanged( lpwndpos);	}
	// (2006/1/28 - Seung-Won, Bae) Reset Tool UI
	virtual void	ReleaseToolUI( const CToolOptionInfo::TOOLOPTION p_eToolOption)
					{	m_pChartCtrl->ReleaseToolUI( p_eToolOption);	}
	virtual void	RemoveRealItem( LPCTSTR szRealInfo)
					{	m_pChartCtrl->RemoveRealItem( szRealInfo);	}
	virtual void	ReplaceMATitle( CString &szpricema, int nOption = 0)
					{	m_pChartCtrl->ReplaceMATitle( szpricema, nOption);	}
	virtual void	RequestTRPacket( const CString &strIndicatorName, const CString &strPacketNames)
					{	m_pChartCtrl->RequestTRPacket( strIndicatorName, strPacketNames);	}
	virtual void	RequestTRPacket( const CString &strIndicatorName, const CString &strPacketNames, const int nCount, const bool bIsNew)
					{	m_pChartCtrl->RequestTRPacket( strIndicatorName, strPacketNames, nCount, bIsNew);	}
//	virtual void	ResetAnalysisToolTextPosition( void)
//					{	m_pChartCtrl->ResetAnalysisToolTextPosition();	}
	virtual void	ResetChartCfgWith( short p_nIndex)
					{	m_pChartCtrl->ResetChartCfgWith( p_nIndex);	}
//	virtual bool	EmptyChart(const bool bIsDeleteAllPacket, const bool bIsDefault)
//					{	return m_pChartCtrl->EmptyChart(bIsDeleteAllPacket, bIsDefault);	}
	virtual void	RunEnvironmentDlg( short nOption)
					{	m_pChartCtrl->RunEnvironmentDlg( nOption);	}
	virtual void	SaveandApplyGraphStyle( short nSaveDataType, short nChangeDataType)
					{	m_pChartCtrl->SaveandApplyGraphStyle( nSaveDataType, nChangeDataType);	}
	virtual void	SendMessagetoMain( int nType, int nOption = -1)
					{	m_pChartCtrl->SendMessagetoMain( nType, nOption  -1);	}
	virtual void	SendTimeOfIndexToDLL( UINT p_uMessage, int p_nIndex)
					{	m_pChartCtrl->SendTimeOfIndexToDLL( p_uMessage, p_nIndex);	}
	virtual void	SetAllLogScale( BOOL bLogScale)
					{	m_pChartCtrl->SetAllLogScale( bLogScale);	}
	virtual void	SetAllProperties_Chart()
					{	m_pChartCtrl->SetAllProperties_Chart();	}
	virtual void	SetAutoSave( const bool bAutoSave, const bool bOutside = true)
					{	m_pChartCtrl->SetAutoSave( bAutoSave, bOutside);	}
	virtual void	SetBackgroundColors( OLE_COLOR p_rgbBlockArea, OLE_COLOR p_rgbScaleArea)
					{	m_pChartCtrl->SetBackgroundColors( p_rgbBlockArea, p_rgbScaleArea);	}
	virtual void	SetBaseDesk( long lKey)
					{	m_pChartCtrl->SetBaseDesk( lKey);	}
	virtual void	SetBongCountMargin( short nBongCountMargin)
					{	m_pChartCtrl->SetBongCountMargin( nBongCountMargin);	}
	virtual void	SetChartAllInfo()
					{	m_pChartCtrl->SetChartAllInfo();	}
	virtual void	SetClrAnalysisTool( OLE_COLOR nNewValue)
					{	m_pChartCtrl->SetClrAnalysisTool( nNewValue);	}
	virtual void	SetClrBackBorder( OLE_COLOR nNewValue)
					{	m_pChartCtrl->SetClrBackBorder( nNewValue);	}
	virtual void	SetClrBackground( OLE_COLOR nNewValue)
					{	m_pChartCtrl->SetClrBackground( nNewValue);	}
	virtual void	SetClrBlock( OLE_COLOR nNewValue)
					{	m_pChartCtrl->SetClrBlock( nNewValue);	}
	virtual void	SetClrBlockBorder( OLE_COLOR nNewValue)
					{	m_pChartCtrl->SetClrBlockBorder( nNewValue);	}
	virtual void	SetClrGraphBase1( OLE_COLOR nNewValue)
					{	m_pChartCtrl->SetClrGraphBase1( nNewValue);	}
	virtual void	SetClrGraphBase2( OLE_COLOR nNewValue)
					{	m_pChartCtrl->SetClrGraphBase2( nNewValue);	}
	virtual void	SetClrGraphBase3( OLE_COLOR nNewValue)
					{	m_pChartCtrl->SetClrGraphBase3( nNewValue);	}
	virtual void	SetClrGraphBase4( OLE_COLOR nNewValue)
					{	m_pChartCtrl->SetClrGraphBase4( nNewValue);	}
	virtual void	SetClrGraphBase5( OLE_COLOR nNewValue)
					{	m_pChartCtrl->SetClrGraphBase5( nNewValue);	}
	virtual void	SetClrGraphBase6( OLE_COLOR nNewValue)
					{	m_pChartCtrl->SetClrGraphBase6( nNewValue);	}
	virtual void	SetClrInquiryWindow( OLE_COLOR nNewValue)
					{	m_pChartCtrl->SetClrInquiryWindow( nNewValue);	}
	virtual void	SetClrLattice( OLE_COLOR nNewValue)
					{	m_pChartCtrl->SetClrLattice( nNewValue);	}
	virtual void	SetClrScaleLine( OLE_COLOR nNewValue)
					{	m_pChartCtrl->SetClrScaleLine( nNewValue);	}
	virtual void	SetClrScaleLine2( OLE_COLOR nNewValue)
					{	m_pChartCtrl->SetClrScaleLine2( nNewValue);	}
	virtual void	SetClrScaleText( OLE_COLOR nNewValue)
					{	m_pChartCtrl->SetClrScaleText( nNewValue);	}
	virtual void	SetContainerDllInfo( long p_lDllWnd, short p_nID)
					{	m_pChartCtrl->SetContainerDllInfo( p_lDllWnd, p_nID);	}
//	virtual void	SetCursorExchange( const CCursorShape::CHARTOBJECT enumChartObject)
	virtual void	SetCustomFont( LPCTSTR lpszNewValue)
					{	m_pChartCtrl->SetCustomFont( lpszNewValue);	}
	virtual void	SetDataIndexByDate( LPCTSTR pcszStartDate, LPCTSTR pcszEndDate)
					{	m_pChartCtrl->SetDataIndexByDate( pcszStartDate, pcszEndDate);	}
	virtual void	SetDataRegionInBlock( short nDataStartIndex)
					{	m_pChartCtrl->SetDataRegionInBlock( nDataStartIndex);	}
	virtual void	SetDataToMainBlock( const CString &strData)
					{	m_pChartCtrl->SetDataToMainBlock( strData);	}
	virtual void	SetDataWindowData( CPoint point)
					{	m_pChartCtrl->SetDataWindowData( point);	}
	virtual void	SetDataWindowHeader( CPoint point)
					{	m_pChartCtrl->SetDataWindowHeader( point);	}
	virtual void	SetDataWindowScaleData( const CString &strHorzScaleName, const int nDataIndex, CString &strViewData)
					{	m_pChartCtrl->SetDataWindowScaleData( strHorzScaleName, nDataIndex, strViewData);	}
	virtual void	SetDataWindowStart( CPoint point)
					{	m_pChartCtrl->SetDataWindowStart( point);	}
	virtual void	SetDataWindowViewData( const CString &strTitle, const CString &strPacketNames, const int nDataIndex, CString &strViewData)
					{	m_pChartCtrl->SetDataWindowViewData( strTitle, strPacketNames, nDataIndex, strViewData);	}
	virtual void	SetDragModeType( short p_nDragModeType)
					{	m_pChartCtrl->SetDragModeType( p_nDragModeType);	}
	virtual void	SetEtc( const int nOption, const int nBongCountMargin)
					{	m_pChartCtrl->SetEtc( nOption, nBongCountMargin);	}
	virtual void	SetGraphMargin( LPCTSTR pcszFunctionName, const bool bIsAddMargin)
					{	m_pChartCtrl->SetGraphMargin( pcszFunctionName, bIsAddMargin);	}
	virtual void	SetGraphSelectedDataIndex( const int nDataIndex, LPCTSTR pcszIndicatorName)
					{	m_pChartCtrl->SetGraphSelectedDataIndex( nDataIndex, pcszIndicatorName);	}
	virtual void	SetGraphThickness( const int nGraphThickness1)
					{	m_pChartCtrl->SetGraphThickness( nGraphThickness1);	}
	virtual void	SetGraphTypeStyle( const int nGraphType, const int nDrawStyle)
					{	m_pChartCtrl->SetGraphTypeStyle( nGraphType, nDrawStyle);	}
	virtual void	SetHtsPath( LPCTSTR pcszHtsPath)
					{	m_pChartCtrl->SetHtsPath( pcszHtsPath);	}
	virtual void	SetImaginaryCandle( const bool bImaginaryCandle, const bool bOutside = true)
					{	m_pChartCtrl->SetImaginaryCandle( bImaginaryCandle, bOutside);	}
//	virtual void	SetInquiryDataWindowMoveType( const int nMoveType, const bool bOutside = true)
//					{	m_pChartCtrl->SetInquiryDataWindowMoveType( nMoveType, bOutside);	}
//	virtual void	SetInquiryDataWindowType()
//					{	m_pChartCtrl->SetInquiryDataWindowType();	}
//	virtual void	SetInquiryDataWindowType( const int nWindowType, const bool bOutside = true)
//					{	m_pChartCtrl->SetInquiryDataWindowType( nWindowType, bOutside);	}
//	virtual void	SetInquiryDlgMode( const int nMode)
//					{	m_pChartCtrl->SetInquiryDlgMode( nMode);	}
	virtual void	SetInquiryValue( const BOOL nIs)
					{	m_pChartCtrl->SetInquiryValue( nIs);	}
	virtual void	SetItemInfo( LPCTSTR lpstrCode, LPCTSTR lpstrName)
					{	m_pChartCtrl->SetItemInfo( lpstrCode, lpstrName);	}
	virtual void	SetLoggingFlag( const BOOL p_bInLogging)
					{	m_pChartCtrl->m_bInLogging = p_bInLogging;	}
	virtual void	SetMainBlock( BOOL bUserMode)
					{	m_pChartCtrl->SetMainBlock( bUserMode);	}
	virtual void	SetMainBlock_MainBlock( const bool bUserMode)
					{	m_pChartCtrl->SetMainBlock_MainBlock( bUserMode);	}
//	virtual void	SetMainBlock_Tool( const bool bUserMode)
//					{	m_pChartCtrl->SetMainBlock_Tool( bUserMode);	}
	virtual void	SetMainBlock_ToolBar( const bool bUserMode)
					{	m_pChartCtrl->SetMainBlock_ToolBar( bUserMode);	}
	virtual void	SetMainBlockData( const CString &strPropertyName)
					{	m_pChartCtrl->SetMainBlockData( strPropertyName);	}
	virtual void	SetMultiChartDlg( long lpMultiChartDlg)
					{}	// (2006/1/11 - Seung-Won, Bae) Hanwha Platform Only
//	virtual void	SetNoneSelectMode( const CSelectMode::MOUSESELECTMODE eSelectMode)
	virtual void	SetOnePageDataCount( long nOnePageDataCount)
					{	m_pChartCtrl->SetOnePageDataCount( nOnePageDataCount);	}
	virtual void	SetOnePageDataCount2( long nOnePageDataCount, BOOL bIncludeBongCountMargin)
					{	m_pChartCtrl->SetOnePageDataCount2( nOnePageDataCount, bIncludeBongCountMargin);	}
	virtual void	SetOnePageDataCountForScroll( const int nType, const int nTotalDataCount)
					{	m_pChartCtrl->SetOnePageDataCountForScroll( nType, nTotalDataCount);	}
	virtual void	SetOnePageDataCountInTRCount( BOOL bIsTRCount, BOOL bIsNowChange)
					{	m_pChartCtrl->SetOnePageDataCountInTRCount( bIsTRCount, bIsNowChange);	}
	virtual void	SetOutsideCommonInterface( long lpOutsideCommonInterface)
					{	m_pChartCtrl->SetOutsideCommonInterface( lpOutsideCommonInterface);	}
	virtual void	SetPacketOption( LPCTSTR strPacketOption)
					{	m_pChartCtrl->SetPacketOption( strPacketOption);	}
//	virtual void	SetProperty_Bool( CPropertiesList *pPropertiesList, CString strPropertyName, BOOL bValue)
//	virtual void	SetProperty_Color( CPropertiesList *pPropertiesList, CString strPropertyName, COLORREF clrColor)
//	virtual void	SetProperty_Int( CPropertiesList *pPropertiesList, CString strPropertyName, int nValue)
//	virtual void	SetProperty_String( CPropertiesList *pPropertiesList, CString strPropertyName, CString strValue)
	virtual void	SetRealData( LPCTSTR strPacketName, double dData, short nOption)
					{	m_pChartCtrl->SetRealData( strPacketName, dData, nOption);	}
	virtual void	SetRealDataAdapter( const char *pcszPacketName, double dData, short nOption)
					{	m_pChartCtrl->SetRealDataAdapter( pcszPacketName, dData, nOption);	}
	virtual void	SetRightMargin( const int nRightMargin, const bool bOutside = true)
					{	m_pChartCtrl->SetRightMargin( nRightMargin, bOutside);	}
	virtual void	SetRscMng( long lKey)
					{	m_pChartCtrl->SetRscMng( lKey);	}
	virtual void	SetRunTimeMode( BOOL bRunTimeMode)
					{	m_pChartCtrl->SetRunTimeMode( bRunTimeMode);	}
	virtual void	SetScrollBar()
					{	m_pChartCtrl->SetScrollBar();	}
	virtual void	SetScrollInfo( const int nOnePageDataCount)
					{	m_pChartCtrl->SetScrollInfo( nOnePageDataCount);	}
//	virtual void	SetSelectMode( const CToolOptionInfo::TOOLOPTION nToolOption, const int nParam)
//	virtual void	SetShowCrossLine( const bool bShow, const bool bOutside = true)
//					{	m_pChartCtrl->SetShowCrossLine( bShow, bOutside);	}
	virtual void	SetShowCurrentPrice( const int nShowType, const bool bOutside = true)
					{	m_pChartCtrl->SetShowCurrentPrice( nShowType, bOutside);	}
	virtual void	SetShowExRightDividend( const bool bShow, const bool bOutside = true)
					{	m_pChartCtrl->SetShowExRightDividend( bShow, bOutside);	}
	virtual void	SetShowHighLowBound( const bool bShow, const bool bOutside = true)
					{	m_pChartCtrl->SetShowHighLowBound( bShow, bOutside);	}
	virtual void	SetShowIndicatorValue( const bool bShow, const bool bOutside = true)
					{	m_pChartCtrl->SetShowIndicatorValue( bShow, bOutside);	}
	virtual void	SetShowIndicatorVariable( const bool bShow, const bool bOutside = true)
					{	m_pChartCtrl->SetShowIndicatorVariable( bShow, bOutside);	}
	virtual void	SetShowMinMaxPrice( const bool bShow, const bool bOutside = true)
					{	m_pChartCtrl->SetShowMinMaxPrice( bShow, bOutside);	}
	virtual void	SetShowTrendLineValue( const int nPosition, const bool bOutside = true)
					{	m_pChartCtrl->SetShowTrendLineValue( nPosition, bOutside);	}
	virtual void	SetStartEndIndex( const int nStartIndex, const int nEndIndex)
					{	m_pChartCtrl->SetStartEndIndex( nStartIndex, nEndIndex);	}
	virtual void	SetStateWholeView( const int nParam = 0)
					{	m_pChartCtrl->SetStateWholeView( nParam);	}
	virtual void	SetStrBlockRegion( LPCTSTR lpszNewValue)
					{	m_pChartCtrl->SetStrBlockRegion( lpszNewValue);	}
//	virtual void	SetStyleDataInOCX( const CStyleData &styleData)
	virtual void	SetTimeLinkage( const bool bTimeLinkage, const bool bOutside = true)
					{	m_pChartCtrl->SetTimeLinkage( bTimeLinkage, bOutside);	}
	virtual void	SetTimeMark( LPCTSTR p_szMarkTime, BOOL p_bWithScroll)
					{	m_pChartCtrl->SetTimeMark( p_szMarkTime, p_bWithScroll);	}
	virtual void	SetTrendLineColor( const COLORREF clrLineColor, const bool bOutside = true)
					{	m_pChartCtrl->SetTrendLineColor( clrLineColor, bOutside);	}
	virtual void	SetUserEnvironmentAndIndicatorListFileName( const CString &strFileName)
					{	m_pChartCtrl->SetUserEnvironmentAndIndicatorListFileName( strFileName);	}
	virtual void	SetWholeView( const BOOL bWholeView)
					{	m_pChartCtrl->SetWholeView( bWholeView);	}
	virtual void	SetYScalePosition( const int nPosition, const bool bOutside = true)
					{	m_pChartCtrl->SetYScalePosition( nPosition, bOutside);	}
	virtual void	ShowAnalysisToolText( BOOL p_bTimeLine, BOOL p_bPriceLineLeft, BOOL p_bPriceLineRight, BOOL p_bTrendLine, BOOL p_bTrisectionLine, BOOL p_bQuadrisectionLine, BOOL p_bFibonacciRetraceLine)
					{	m_pChartCtrl->ShowAnalysisToolText( p_bTimeLine, p_bPriceLineLeft, p_bPriceLineRight, p_bTrendLine, p_bTrisectionLine, p_bQuadrisectionLine, p_bFibonacciRetraceLine);	}
	virtual void	ShowEnvironmentDlg()
					{	m_pChartCtrl->ShowEnvironmentDlg();	}
	virtual void	ShowIndicatorConfigDlg()
					{	m_pChartCtrl->ShowIndicatorConfigDlg();	}
	virtual void	ShowInnerToolBar( BOOL p_bShow)
					{	m_pChartCtrl->ShowInnerToolBar( p_bShow);	}
	// (2006/2/7 - Seung-Won, Bae) Show and Hide Numerical Inquiry Dialog
	virtual void	ShowNumericalInquiryDlg( const BOOL p_bShow, const BOOL p_bAutoMove)
					{	m_pChartCtrl->ShowNumericalInquiryDlg( p_bShow, p_bAutoMove);	}
	virtual void	ShowNumericalInquiryDlg( CDC *p_pDC, const CPoint &p_ptPoint)
					{	m_pChartCtrl->ShowNumericalInquiryDlg( p_pDC, p_ptPoint);	}
	virtual void	ShowTooltip( const CPoint &p_ptPoint)
					{	m_pChartCtrl->ShowTooltip( p_ptPoint);	}
	virtual void	testLoadAnalysisTools( short nToolOption, short nOption)
					{	m_pChartCtrl->testLoadAnalysisTools( nToolOption, nOption);	}
	virtual void	UserFileVersionChecker()
					{	m_pChartCtrl->UserFileVersionChecker();	}
	virtual void	WriteUserFile( const CString &strFileName)
					{	m_pChartCtrl->WriteUserFile( strFileName);	}
//	virtual void	WriteUserFile( CSaveUserFile::FILETYPE nType)
};

#endif // !defined( AFX_IWCHARTCTRL_H__6470428F_7AB9_4790_B60C_C08F1BDF5990__INCLUDED_)
