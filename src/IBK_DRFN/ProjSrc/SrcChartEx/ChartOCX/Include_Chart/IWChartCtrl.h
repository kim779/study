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
	virtual bool	AddOrDeleteChart( const CString &strChartName)
					{	return m_pChartCtrl->AddOrDeleteChart( strChartName);	}
	virtual bool	AddOrDeleteChart_Genneral( const CString &strChartName)
					{	return m_pChartCtrl->AddOrDeleteChart_Genneral( strChartName);	}
	virtual bool	AddOrDeleteChart_Special( const CString &strChartName)
					{	return m_pChartCtrl->AddOrDeleteChart_Special( strChartName);	}
	virtual bool	AddRemoveGraph( LPCTSTR pcszGraphName)
					{	return m_pChartCtrl->AddRemoveGraph( pcszGraphName);	}
	virtual bool	ApplySkin()
					{	return m_pChartCtrl->ApplySkin();	}
	virtual bool	CalculateIndicatorInTRData(CIndicatorInfo* pIndicatorInfo, const CList<CPacket*, CPacket*>& packetList)
					{	return m_pChartCtrl->CalculateIndicatorInTRData( pIndicatorInfo,  packetList);	}
	virtual bool	ChangeChartType( CString strGraphName, CString strType, CString strStyle, CString strPacketNameList)
					{	return m_pChartCtrl->ChangeChartType( strGraphName, strType, strStyle, strPacketNameList);	}
	virtual bool	ChangeSubGraphData( const char *pcszGraphName, const char *pcszSubGraphName, COLORREF clrLineColor, int nThickness, int nDrawStyle)
					{	return m_pChartCtrl->ChangeSubGraphData( pcszGraphName, pcszSubGraphName, clrLineColor, nThickness, nDrawStyle);	}
	virtual bool	CreateUserDirectory()
					{	return m_pChartCtrl->CreateUserDirectory();	}
	virtual bool	DeleteChart_Genneral( const CString &strChartName)
					{	return m_pChartCtrl->DeleteChart_Genneral( strChartName);	}
	virtual bool	GetAutoSave()
					{	return m_pChartCtrl->GetAutoSave();	}
	virtual bool	GetGraphDataForMaking( const CString& strGraphOption, CString& strGraphDataForMaking) const
					{	return m_pChartCtrl->GetGraphDataForMaking( strGraphOption, strGraphDataForMaking);	}
	virtual bool	GetImaginaryCandle()
					{	return m_pChartCtrl->GetImaginaryCandle();	}
	virtual bool	GetRequestTRPacketName( const CString &strPacketNames, CString &strPacketNamesForTR) const
					{	return m_pChartCtrl->GetRequestTRPacketName( strPacketNames, strPacketNamesForTR);	}
	virtual bool	GetShowIndicatorValue()
					{	return m_pChartCtrl->GetShowIndicatorValue();	}
	virtual bool	GetShowMinMaxPrice()
					{	return m_pChartCtrl->GetShowMinMaxPrice();	}
	virtual bool	HasGraphAdapter( const char *szGraphName)
					{	return m_pChartCtrl->HasGraphAdapter( szGraphName);	}
	virtual bool	IsScrollInHorzScaleData() const
					{	return m_pChartCtrl->IsScrollInHorzScaleData();	}
	virtual bool	IsUseToolScrollBar()
					{	return m_pChartCtrl->IsUseToolScrollBar();	}
	virtual bool	IsWholeView() const
					{	return m_pChartCtrl->IsWholeView();	}
	virtual bool	OnReturnOrg2( const BOOL bIsMessageBox = TRUE)
					{	return m_pChartCtrl->OnReturnOrg2( bIsMessageBox);	}
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
	virtual bool	RunToolFunction( const CToolOptionInfo::TOOLOPTION p_eToolOption, const int p_nParam)	// 툴기능실행/해제
					{	return m_pChartCtrl->RunToolFunction( p_eToolOption, p_nParam);	}
	virtual bool	SaveChartCfg()
					{	return m_pChartCtrl->SaveChartCfg();	}
	virtual bool	SaveChartCfgInCurrentOCX()
					{	return m_pChartCtrl->SaveChartCfgInCurrentOCX();	}
	virtual bool	SetPacketDataInPacketList( LPCTSTR szPacketName, long szData, long lLength, BOOL bReset)
					{	return m_pChartCtrl->SetPacketDataInPacketList( szPacketName, szData, lLength, bReset);	}
	virtual bool	SetStartIndexOfPacket(const char* pcszRQ, const char *pcszPacketName, int nStartDataIndex, BOOL bAddSpace)
					{	return m_pChartCtrl->SetStartIndexOfPacket(pcszRQ, pcszPacketName, nStartDataIndex, bAddSpace);	}
	virtual bool	SetStartPeriodOfPacketAdapter( const char *szPacketName, long lStartPeriod, BOOL bAddSpace)
					{	return m_pChartCtrl->SetStartPeriodOfPacketAdapter( szPacketName, lStartPeriod, bAddSpace);	}

protected:
	virtual BOOL	AddChartBlock( LPCTSTR p_szIndicatorName, CString &p_strGraphName)
					{	return m_pChartCtrl->AddChartBlockWithGetGraphName( p_szIndicatorName, p_strGraphName);	}
	virtual BOOL	AddLocalShareInfo( const char *p_szLSharedName, int p_nSettingType, const char *p_szLocalSharedData)
					{
						if( !m_pChartCtrl->m_plLocalShareInfoList) return FALSE;
						return ( BOOL)m_pChartCtrl->m_plLocalShareInfoList->AddLocalShareInfo( p_szLSharedName, p_nSettingType, p_szLocalSharedData);
					}
	virtual BOOL	AddOrRemoveChart( LPCTSTR strGraphName)
					{	return m_pChartCtrl->AddOrRemoveChart( strGraphName);	}
	virtual BOOL	CanGraphMoveAndDelete( void)	// (2009/6/5 - Seung-Won, Bae) Check mode for moving and deleting the graph.
					{	return m_pChartCtrl->CanGraphMoveAndDelete();	}
	virtual BOOL	ChangeChartItem( LPCTSTR strGraphName, short nSubGraphIndex, LPCTSTR strNewTitle, LPCTSTR strNewPacketName, LPCTSTR strNewGraphType, LPCTSTR strNewGraphStyle, short nNewUpLineWidth, short nNewDownLineWidth, short nNewEqualLineWidth, BOOL bRunatOnce)
					{	return m_pChartCtrl->ChangeChartItem( strGraphName, nSubGraphIndex, strNewTitle, strNewPacketName, strNewGraphType, strNewGraphStyle, nNewUpLineWidth, nNewDownLineWidth, nNewEqualLineWidth, bRunatOnce);	}
	virtual BOOL	ChangeGraphDrawingData( LPCTSTR p_szGraphName, BOOL p_bLowHigh, BOOL p_bLowCur, BOOL p_bHighCur, BOOL p_bPrevCur, short p_nCurData)
					{	return m_pChartCtrl->ChangeGraphDrawingData( p_szGraphName, p_bLowHigh, p_bLowCur, p_bHighCur, p_bPrevCur, p_nCurData);	}
	virtual BOOL	ChangeGraphTitle( short nRowIndex, short nColumnIndex, short nGraphIndex, LPCTSTR strNewTitle)
					{	return m_pChartCtrl->ChangeGraphTitle( nRowIndex, nColumnIndex, nGraphIndex, strNewTitle);	}
	virtual BOOL	ChangeGraphTitle2( LPCTSTR szTargetTitle, LPCTSTR szSourceTitle)
					{	return m_pChartCtrl->ChangeGraphTitle2( szTargetTitle, szSourceTitle);	}
	virtual BOOL	ChangeGraphType( LPCTSTR strGraphName, LPCTSTR strGraphType, LPCTSTR strGraphStyle, LPCTSTR strPacketNameList, BOOL bRunatOnce)
					{	return m_pChartCtrl->ChangeGraphType( strGraphName, strGraphType, strGraphStyle, strPacketNameList, bRunatOnce);	}
	virtual BOOL	ChangeHorzScalePosition( short nScalePosition, short nRowIndex, short nColumnIndex)
					{	return m_pChartCtrl->ChangeHorzScalePosition( nScalePosition, nRowIndex, nColumnIndex);	}
	virtual BOOL	ChangeIndicatorPacketName( LPCTSTR strIndicatorName, short nSubGraphIndex, LPCTSTR strNewPacketName)
					{	return m_pChartCtrl->ChangeIndicatorPacketName( strIndicatorName, nSubGraphIndex, strNewPacketName);	}
	virtual BOOL	ChangeOneChart( const CString &p_strIndicatorName, CString &p_strGraphName)
					{	return m_pChartCtrl->ChangeOneChart( p_strIndicatorName, p_strGraphName);	}
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
	virtual BOOL	IsEnableIndicatorCalculation( void)
					{	return m_pChartCtrl->GetBEnableIndicatorCalculation();	}
	virtual BOOL	FireGSharedData( const char *p_szGSharedName, const char *p_szGSharedData)
					{	return m_pChartCtrl->FireGSharedData( p_szGSharedName, p_szGSharedData);	}
	virtual BOOL	FireSharedData( const char *p_szShareName, const char *p_szSharedData)
					{	return FALSE;	}	// (2006/1/11 - Seung-Won, Bae) Hanwha Platform Only
	virtual BOOL	GetAllProperties3( IPropertyMap *p_pIProperties, BOOL p_bWithFull)
					{
						m_pChartCtrl->m_bFullGetAllProperties3 = p_bWithFull;
						return m_pChartCtrl->GetAllProperties3( ( long)p_pIProperties);
					}
	virtual BOOL	GetBoundMarkFlag()
					{	return m_pChartCtrl->GetBoundMarkFlag();	}
	virtual BOOL	GetDataStartEnd( int &p_nStartIndex, int &p_nEndIndex)
					{	return m_pChartCtrl->GetDataStartEnd( p_nStartIndex, p_nEndIndex);	}
	virtual BOOL	GetGraphTitleOption( BOOL &p_bWithConditionValue, BOOL &p_bWithLastValue)
					{
						p_bWithConditionValue	= m_pChartCtrl->m_bUseConditiontoTitle;
						p_bWithLastValue		= m_pChartCtrl->m_bUseDatatoTitle;
						return TRUE;
					}
	virtual BOOL	GetLogScale( LPCTSTR strGraphName)
					{	return m_pChartCtrl->GetLogScale( strGraphName);	}
	virtual BOOL	GetNormalizedMinuteTimeRule( void)
					{	return m_pChartCtrl->m_bNormalizedMinuteTimeRule;	}
	virtual BOOL	GetScrollRange( int &p_nRange)
					{	return m_pChartCtrl->GetScrollRange( p_nRange);	}
	virtual BOOL	GetShareInfoItem( const char *p_szShareName, int &p_nShareType, int &p_nShareArea, long &p_lCodeFilter)
					{	return FALSE;	}	// (2006/1/11 - Seung-Won, Bae) Hanwha Platform Only
	virtual BOOL	GetStartEndDateOnView( double *p_pStartDate, double *p_pEndDate)
					{	return m_pChartCtrl->GetStartEndDateOnView( p_pStartDate, p_pEndDate);	}
	virtual BOOL	GetTestEnvValue( const char *p_szEnvKey, CString &p_strEnvValue)	// (2008/5/13 - Seung-Won, Bae) Chart TEST ENV for Runtime.
					{	return m_pChartCtrl->GetTestEnvValue( p_szEnvKey, p_strEnvValue);	}
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
	virtual BOOL	IsGraphMenuShow() const
					{	return m_pChartCtrl->m_bAddGraphMenuShow;	}
	virtual BOOL	IsHideAllTitle( void)
					{	return m_pChartCtrl->m_bAllGraphTitleHiding;	}
	virtual BOOL	IsInLogging( void)
					{	return m_pChartCtrl->m_bInLogging;	}
	virtual BOOL	IsInUseRealData( void)
					{	return m_pChartCtrl->m_bUseRealData;	}
	virtual BOOL	IsMultipleSameNameIndicator( void)			// (2007/1/20 - Seung-Won, Bae) Support Multiple Same-Name Indicator.
					{		return m_pChartCtrl->m_bMultipleSameNameIndicator;	}
	virtual BOOL	IsOcxAreaNumericalInquiry( void) const
					{	return m_pChartCtrl->IsOcxAreaNumericalInquiry();	}
	virtual BOOL	IsOnCaptionFlagOfNIDLG( void) const		// (2006/3/29 - Seung-Won, Bae) Dynamic Caption Setting for each Site.
					{	return m_pChartCtrl->IsOnCaptionFlagOfNIDLG();	}
	// (2009/11/6 - Seung-Won, Bae) Do not disable zoom and scroll on OneChart
	virtual BOOL	IsRunningNoZoomChart( void) const
					{	return m_pChartCtrl->IsRunningNoZoomChart();	}
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
	virtual BOOL	IsShowPriceMinMaxText( void)
					{	return m_pChartCtrl->m_bShowPriceMinMaxText;	}
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
	virtual BOOL	OnGraphAdded( const char *p_szRQName, const char *p_szGraphName, const char *p_szIndicatorName)
					{	return m_pChartCtrl->OnGraphAdded( p_szRQName, p_szGraphName, p_szIndicatorName);	}
	virtual BOOL	OnGraphDeleted( const char *p_szRQName, const char *p_szGraphName, const char *p_szIndicatorName)
					{	return m_pChartCtrl->OnGraphDeleted( p_szRQName, p_szGraphName, p_szIndicatorName);		}
	virtual BOOL	OnMouseWheel( UINT nFlags, short zDelta, CPoint pt)
					{	return m_pChartCtrl->OnMouseWheel( nFlags, zDelta, pt);	}
	virtual BOOL	OnRealAdvise()
					{	return FALSE;	}	// (2006/1/11 - Seung-Won, Bae) Hanwha Platform Only
	virtual BOOL	OnRunningOneChart()
					{	return m_pChartCtrl->OnRunningOneChart();	}
	virtual BOOL	OnSetCursor( CWnd *pWnd, UINT nHitTest, UINT message)
					{	return m_pChartCtrl->OnSetCursor( pWnd, nHitTest, message);	}
	virtual BOOL	OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn)	// (2006/11/15 - Seung-Won, Bae) Notify Tool State Changing to MChart an ToolBar
					{	return m_pChartCtrl->OnToolStateChanged( p_eToolID, p_bTurnOn);	}
	virtual BOOL	PacketDataMaker()
					{	return m_pChartCtrl->PacketDataMaker();	}
	virtual BOOL	PartitionAllGraph()
					{	return m_pChartCtrl->PartitionAllGraph();	}
	virtual BOOL	ReadUserFile( const CString &strFileName)
					{	return m_pChartCtrl->ReadUserFile( strFileName);	}
	virtual BOOL	RemoveChart( const char *p_szIndicator)
					{	return m_pChartCtrl->RemoveChart( p_szIndicator);	}
	virtual BOOL	RemoveGraph( LPCTSTR szGraphName)
					{	return m_pChartCtrl->RemoveGraphWithGraphName( szGraphName);	}
	virtual BOOL	RemoveGraph_Title( short nRowIndex, short nColumnIndex, LPCTSTR szGraphTitle, BOOL bIsOnlyNoTRData, BOOL bIsAutoHorzScalePosition)
					{	return m_pChartCtrl->RemoveGraph_Title( nRowIndex, nColumnIndex, szGraphTitle, bIsOnlyNoTRData, bIsAutoHorzScalePosition);	}
	virtual BOOL	RemoveLocalShareInfo( const char *p_szLSharedName)
					{
						if( !m_pChartCtrl->m_plLocalShareInfoList) return FALSE;
						return ( BOOL)m_pChartCtrl->m_plLocalShareInfoList->RemoveLocalShareInfo( p_szLSharedName);
					}
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
	virtual BOOL	SaveChartCFG_RemoveFromIndexToEnd( int p_nIndex)
					{	return m_pChartCtrl->m_SaveChartcfg.RemoveFromIndexToEnd( p_nIndex);	}
	virtual BOOL	SaveChartCFG_ResetWithCurrentStyle( void)
					{
						m_pChartCtrl->m_SaveChartcfg.RemoveAll();
						m_pChartCtrl->SaveChartCfg();
						return TRUE;
					}
	virtual BOOL	ScrollTo( int p_nScrollValue, BOOL p_bRelative)
					{	return m_pChartCtrl->ScrollTo( p_nScrollValue, p_bRelative);	}
	virtual BOOL	ScrollToDataOfEnd( double p_dData)
					{	return m_pChartCtrl->ScrollToDataOfEnd( p_dData);	}
	virtual BOOL	SetAllProperties2( long dwData)
					{	return m_pChartCtrl->SetAllProperties2( dwData);	}
	virtual BOOL	SetAllProperties3( IPropertyMap *p_pIProperties, BOOL p_bReloadAddIn = TRUE)
					{
						// (2006/7/31 - Seung-Won, Bae) Control AddIn Reload for AddIn Request
						m_pChartCtrl->m_bSetAllProperties3WithReloadAddIn = p_bReloadAddIn;
						return m_pChartCtrl->SetAllProperties3( ( long)p_pIProperties);
					}
	virtual BOOL	SetAllPropertiesFromFile( LPCTSTR strFilePath, LPCTSTR strFileName)
					{	return m_pChartCtrl->SetAllPropertiesFromFile( strFilePath, strFileName);	}
	virtual	BOOL	SetAllHorzScaleGridType( int p_nGridVLineType)
					{
						// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
						m_pChartCtrl->m_pMainBlock->SetAllHorzScaleGridType( ( CScaleBaseData::SCALEGRIDTYPE)p_nGridVLineType);
						return TRUE;
					}
	virtual	BOOL	SetAllVertScaleGridType( int p_nGridVLineType)
					{
						// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
						m_pChartCtrl->m_pMainBlock->SetAllVertScaleGridType( ( CScaleBaseData::SCALEGRIDTYPE)p_nGridVLineType);
						return TRUE;
					}
	virtual BOOL	SetAnalysisToolColor( COLORREF p_clrColor)
					{	m_pChartCtrl->SetClrAnalysisTool( p_clrColor);	return TRUE;	}
	virtual BOOL	SetAnalysisToolTextFlag( BOOL p_bTrendLine, BOOL p_bHLineLeft, BOOL p_bHLineRight, BOOL p_bVLine,
							BOOL p_bTreeLine, BOOL p_bFourLine, BOOL p_bFiboLine)
					{
						m_pChartCtrl->ShowAnalysisToolText(	p_bVLine, p_bHLineLeft, p_bHLineRight, p_bTrendLine, 
												p_bTreeLine, p_bFourLine, p_bFiboLine);
						return TRUE;
					}
	// (2007/1/10 - Seung-Won, Bae) Binary TR Setting Interface
	virtual BOOL	SetBinTrHelpMsg( const char *p_szHelpMessage)
					{	return m_pChartCtrl->SetBinTrHelpMsg( p_szHelpMessage);	}
	virtual BOOL	SetBinTrDoubleData( const char *p_szPacketName, long p_nCount, double FAR* p_pdData)
					{	return m_pChartCtrl->SetBinTrDoubleData( p_szPacketName, p_nCount, p_pdData);	}
	virtual BOOL	SetBinTrStringData( const char *p_szPacketName, long p_nCount, long p_lData)
					{	return m_pChartCtrl->SetBinTrStringData( p_szPacketName, p_nCount, p_lData);	}
	virtual BOOL	SetBinTrEnd( void)
					{	return m_pChartCtrl->SetBinTrEnd();	}
	virtual BOOL	SetDrdsAdvise( BOOL bOnAdvise)
					{	return m_pChartCtrl->SetDrdsAdvise( bOnAdvise);	}
	virtual BOOL	SetGraphHide2( short nRowIndex, short nColumnIndex, short nVerticalIndex, short nGraphIndex, short nSubGraphIndex, BOOL bHide)
					{	return m_pChartCtrl->SetGraphHide2( nRowIndex, nColumnIndex, nVerticalIndex, nGraphIndex, nSubGraphIndex, bHide);	}
	virtual BOOL	SetGraphHide3( LPCTSTR strTitle, BOOL bIsHide)
					{	return m_pChartCtrl->SetGraphHide3( strTitle, bIsHide);	}
	virtual BOOL	SetGraphTitle( LPCTSTR strGraphName, short nSubGraphIndex, LPCTSTR strNewGraphTitle, BOOL bGraphShowing)
					{	return m_pChartCtrl->SetGraphTitle( strGraphName, nSubGraphIndex, strNewGraphTitle, bGraphShowing);	}
	virtual BOOL	SetGraphTitleOption( BOOL p_bWithConditionValue, BOOL p_bWithLastValue)
					{
						m_pChartCtrl->m_bUseConditiontoTitle	= p_bWithConditionValue;
						m_pChartCtrl->m_bUseDatatoTitle			= p_bWithLastValue;
						m_pChartCtrl->m_pMainBlock->SetGraphTitleDraw( p_bWithConditionValue, p_bWithLastValue);
						return TRUE;
					}
	virtual BOOL	SetHelpMessageRealDataRate( LPCTSTR strHelpMessageData, BOOL bIsRemove)
					{	return m_pChartCtrl->SetHelpMessageRealDataRate( strHelpMessageData, bIsRemove);	}
	virtual BOOL	SetHorzScaleData( short nRowIndex, short nColumnIndex, LPCTSTR strScaleData)
					{	return m_pChartCtrl->SetHorzScaleData( nRowIndex, nColumnIndex, strScaleData);	}
	virtual BOOL	SetHideAllTitle( BOOL p_bHideAllTitle)
					{	m_pChartCtrl->SetBHideAllTitle( p_bHideAllTitle);	return TRUE;	}
	virtual BOOL	SetIndicatorCondition( LPCTSTR strGraphName, short nConditionType, short nConditionIndex, double dConditionValue, BOOL bReCalculate)
					{	return m_pChartCtrl->SetIndicatorCondition( strGraphName, nConditionType, nConditionIndex, dConditionValue, bReCalculate);	}
	virtual BOOL	SetLogScale( LPCTSTR strGraphName, BOOL bLogScale)
					{	return m_pChartCtrl->SetLogScale( strGraphName, bLogScale);	}
	virtual BOOL	SetMax( LPCTSTR strPacketName, double dMax)
					{	return m_pChartCtrl->SetMax( strPacketName, dMax);	}
	virtual BOOL	SetMin( LPCTSTR strPacketName, double dMin)
					{	return m_pChartCtrl->SetMin( strPacketName, dMin);	}
	virtual BOOL	SetMinMax( LPCTSTR strPacketName, double dMin, double dMax)
					{	return m_pChartCtrl->SetMinMax( strPacketName, dMin, dMax);	}
	virtual BOOL	SetNormalizedMinuteTimeRule( BOOL p_bEnable)
					{
						m_pChartCtrl->m_bNormalizedMinuteTimeRule = p_bEnable;
						m_pChartCtrl->OnbNormalizedMinuteTimeRule();
						return TRUE;
					}
	virtual BOOL	SetNumericalInquiryOption( UINT p_uOption)
					{	m_pChartCtrl->SetNumericalInquiryOption( p_uOption);	return TRUE;	}
		
	virtual BOOL	SetPacketData( LPCTSTR szPacketName, long szData, long lLength, BOOL bReset)
					{	return m_pChartCtrl->SetPacketData( szPacketName, szData, lLength, bReset);	}
	virtual BOOL	SetPacketData2( LPCTSTR strPacketName, LPCTSTR strPacketType, LPCTSTR strPacketData, short nDataCount, BOOL bReCalculate)
					{	return m_pChartCtrl->SetPacketData2( strPacketName, strPacketType, strPacketData, nDataCount, bReCalculate);	}
	virtual BOOL	SetPacketDataInAdditionPacket( LPCTSTR szPacketName, long szData, long lLength, BOOL bReset)
					{	return m_pChartCtrl->SetPacketDataInAdditionPacket( szPacketName, szData, lLength, bReset);	}
	virtual BOOL	SetPacketHelpMessage( LPCTSTR strHelpMsg, BOOL bIsAllRemove)
					{	return m_pChartCtrl->SetPacketHelpMessage( strHelpMsg, bIsAllRemove);	}
	virtual BOOL	SetSharedData( short p_nShareType, LPCTSTR p_szSharedName, LPCTSTR p_szData)
					{	return FALSE;	}	// (2006/1/11 - Seung-Won, Bae) Hanwha Platform Only
	virtual BOOL	SetStartEndDateOfView( double p_dStartDate, double p_dEndDate)
					{	return m_pChartCtrl->SetStartEndDateOfView( p_dStartDate, p_dEndDate);	}
	virtual BOOL	SetStartEndIndex( const int nStartIndex, const int nEndIndex)
					{	return m_pChartCtrl->SetStartEndIndex( nStartIndex, nEndIndex);	}
	virtual BOOL	SetStartPeriodOfPacket( LPCTSTR p_szPacketName, long p_lStartPeriod, BOOL p_bAddSpace)
					{	return m_pChartCtrl->SetStartPeriodOfPacket( p_szPacketName, p_lStartPeriod, p_bAddSpace);	}
	// (2006/8/18 - Seung-Won, Bae) Style Data Setting Interfaces
	virtual	BOOL	SetStyleValueAnalysisToolColor(	COLORREF	p_clrAnalysisTool)
					{	m_pChartCtrl->SetClrAnalysisTool( p_clrAnalysisTool);	return TRUE;	}
	virtual	BOOL	SetStyleValueGraphBGColor(		COLORREF	p_clrGraphBG,		BOOL p_bWithUpdate = FALSE)
					{
						m_pChartCtrl->m_clrBlock = p_clrGraphBG;
						if( p_bWithUpdate) 	m_pChartCtrl->m_pMainBlock->SetGraphRegionColor( CGraphRegionColor( m_pChartCtrl->m_clrBlock, m_pChartCtrl->m_clrLattice, m_pChartCtrl->m_clrBlockBorder));
						return TRUE;
					}
	virtual	BOOL	SetStyleValueGraphBorderColor(	COLORREF	p_clrGraphBorder,	BOOL p_bWithUpdate = FALSE)
					{
						m_pChartCtrl->m_clrBlockBorder = p_clrGraphBorder;
						if( p_bWithUpdate) 	m_pChartCtrl->m_pMainBlock->SetGraphRegionColor( CGraphRegionColor( m_pChartCtrl->m_clrBlock, m_pChartCtrl->m_clrLattice, m_pChartCtrl->m_clrBlockBorder));
						return TRUE;
					}
	virtual	BOOL	SetStyleValueGraphGridColor(	COLORREF	p_clrGraphGrid,		BOOL p_bWithUpdate = FALSE)
					{
						m_pChartCtrl->m_clrLattice = p_clrGraphGrid;
						if( p_bWithUpdate) 	m_pChartCtrl->m_pMainBlock->SetGraphRegionColor( CGraphRegionColor( m_pChartCtrl->m_clrBlock, m_pChartCtrl->m_clrLattice, m_pChartCtrl->m_clrBlockBorder));
						return TRUE;
					}
	virtual	BOOL	SetStyleValueGridHeight(		int			p_nGridHeight,		BOOL p_bWithUpdate = FALSE)
					{
						m_pChartCtrl->m_nVertGap = p_nGridHeight;
						if( p_bWithUpdate) m_pChartCtrl->m_pMainBlock->SetGraphBackgroundGap( m_pChartCtrl->m_nHorzGap, m_pChartCtrl->m_nVertGap);
						return TRUE;
					}
	virtual	BOOL	SetStyleValueGridHLineColor(	COLORREF	p_clrGridHLine,		BOOL p_bWithUpdate = FALSE)
					{
						m_pChartCtrl->m_clrScaleLine2 = p_clrGridHLine;
						if( p_bWithUpdate) m_pChartCtrl->m_pMainBlock->SetScaleColor( m_pChartCtrl->m_clrScaleLine2, m_pChartCtrl->m_clrScaleLine, m_pChartCtrl->m_clrScaleText);
						return TRUE;
					}
	virtual	BOOL	SetStyleValueGridVLineColor(	COLORREF	p_clrGridVLine,		BOOL p_bWithUpdate = FALSE)
					{
						m_pChartCtrl->m_clrScaleLine = p_clrGridVLine;
						if( p_bWithUpdate) m_pChartCtrl->m_pMainBlock->SetScaleColor( m_pChartCtrl->m_clrScaleLine2, m_pChartCtrl->m_clrScaleLine, m_pChartCtrl->m_clrScaleText);
						return TRUE;
					}
	virtual	BOOL	SetStyleValueGridWidth(			int			p_nGridWidth,		BOOL p_bWithUpdate = FALSE)
					{
						m_pChartCtrl->m_nHorzGap = p_nGridWidth;
						if( p_bWithUpdate) m_pChartCtrl->m_pMainBlock->SetGraphBackgroundGap( m_pChartCtrl->m_nHorzGap, m_pChartCtrl->m_nVertGap);
						return TRUE;
					}
	virtual	BOOL	SetStyleValueNIDLGColor(		COLORREF	p_clrNIDLG)
					{	m_pChartCtrl->m_clrInquiryWindow = p_clrNIDLG;			return TRUE;	}
	virtual	BOOL	SetStyleValueScaleBGColor(		COLORREF	p_clrScaleBG)
					{	m_pChartCtrl->m_clrBackground = p_clrScaleBG;			return TRUE;	}
	virtual	BOOL	SetStyleValueScaleBorderColor(	COLORREF	p_clrScaleBorder)
					{	m_pChartCtrl->m_clrBackBorder = p_clrScaleBorder;		return TRUE;	}
	virtual	BOOL	SetStyleValueScaleTextColor(	COLORREF	p_clrScaleText,		BOOL p_bWithUpdate = FALSE)
					{
						m_pChartCtrl->m_clrScaleText = p_clrScaleText;
						if( p_bWithUpdate) m_pChartCtrl->m_pMainBlock->SetScaleColor( m_pChartCtrl->m_clrScaleLine2, m_pChartCtrl->m_clrScaleLine, m_pChartCtrl->m_clrScaleText);
						return TRUE;
					}
	
	virtual BOOL	SetVertScaleData( short nRowIndex, short nColumnIndex, short nVertScaleGroupIndex, LPCTSTR strVertScaleData)
					{	return m_pChartCtrl->SetVertScaleData( nRowIndex, nColumnIndex, nVertScaleGroupIndex, strVertScaleData);	}
	virtual BOOL	ShowPriceMinMaxText( BOOL p_bShowPriceMinMaxText)
					{	m_pChartCtrl->SetBShowPriceMinMaxText( p_bShowPriceMinMaxText);	return TRUE;	}
	virtual BOOL	UnloadAddIn( LPCTSTR p_szAddInDllName)
					{	return m_pChartCtrl->UnloadAddIn( p_szAddInDllName);	}
	virtual BOOL	ZoomTo( int p_nZoomValue, BOOL p_bRelative)
					{	return m_pChartCtrl->ZoomTo( p_nZoomValue, p_bRelative);	}
	// 기준선 보임 환경설정과 Scale 자동포함 기능 - ojtaso (20071101)
	virtual BOOL	SetDrawBaseLine(BOOL bDrawBaseLine) 
					{ return m_pChartCtrl->m_pMainBlock->SetDrawBaseLine(bDrawBaseLine); }
	virtual bool	GetDrawBaseLine() 
					{ return m_pChartCtrl->m_pMainBlock->GetDrawBaseLine(); }

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
	virtual const char *	GetBaseDeskPath()
							{	return m_pChartCtrl->GetBaseDeskPath();	}
	virtual const char *	GetChartList( void)
							{	return m_pChartCtrl->GetChartList();	}
	virtual const char *	GetCurrentRQ( void)
							{	return m_pChartCtrl->m_strRQ;	}
	virtual const char *	GetLocalSharedData( const char *p_szLSharedName)
							{
								if( !m_pChartCtrl->m_plLocalShareInfoList) return NULL;
								return m_pChartCtrl->m_plLocalShareInfoList->GetLocalSharedData( p_szLSharedName);
							}
	virtual const char *	GetOCXPath()
							{	return g_strOcxPath; }
	virtual const char *	GetPacketInfoStr( void)
							{	return m_pChartCtrl->m_strPacketOption;	}
	virtual const char *	GetUserFilePath()
							{	return m_pChartCtrl->m_strUserFilePath;	}

protected:
	virtual CAddInManager *		GetAddInManager( void)
								{	return m_pChartCtrl->m_pAddInManager;	}
	virtual CCommonInterface *	GetCommonInterface( void)
								{	return ( CCommonInterface *)m_pChartCtrl;	}
	virtual CMainBlock *		GetMainBlock( void)
								{	return m_pChartCtrl->GetMainBlock();	}
	virtual COutsideCommonInterface *	GetMChartInterface( void)
								{	return m_pChartCtrl->m_OutsideCommonInterfaceFunction.GetOutsideCommonInterface();	}
	virtual CIndicatorList *	GetIndicatorList( void)
								{	return m_pChartCtrl->GetIndicatorList();	}
	virtual CPacketList *		GetPacketList( void)
								{	return m_pChartCtrl->GetPacketList();	}
	// PacketListManager : 복수종목 - ojtaso (20070110)
	virtual CPacketListManager*	GetPacketListManager( void)
								{	return m_pChartCtrl->GetPacketListManager();	}
	virtual CXScaleManager *	GetXScaleManager( void)
								{	return m_pChartCtrl->m_pxScaleManager;	}
	virtual CChartInfo::CHART_MODE GetChartMode( void)		// (2009/9/1 - Seung-Won, Bae) Define Chart Mode.
								{	return ( CChartInfo::CHART_MODE)m_pChartCtrl->m_nChartMode;	}

protected:
	virtual CFont *	GetChartFont( void)
					{	return m_pChartCtrl->GetChartFont();	}
	virtual CFont *	GetRscFont() const
					{	return m_pChartCtrl->GetRscFont();	}

protected:
	virtual COLORREF	GetClrGraphBase1()
						{	return m_pChartCtrl->GetClrGraphBase1();	}
	virtual COLORREF	GetClrGraphBase2()
						{	return m_pChartCtrl->GetClrGraphBase2();	}
	virtual COLORREF	GetClrGraphBase3()
						{	return m_pChartCtrl->GetClrGraphBase3();	}
	virtual COLORREF	GetClrGraphBase4()
						{	return m_pChartCtrl->GetClrGraphBase4();	}
	virtual COLORREF	GetClrGraphBase5()
						{	return m_pChartCtrl->GetClrGraphBase5();	}
	virtual COLORREF	GetClrGraphBase6()
						{	return m_pChartCtrl->GetClrGraphBase6();	}
	// (2006/8/18 - Seung-Won, Bae) Style Data Setting Interfaces
	virtual	COLORREF	GetStyleValueAnalysisToolColor( void)
						{	return m_pChartCtrl->m_clrAnalysisTool;		}
	virtual	COLORREF	GetStyleValueGraphBGColor( void)
						{	return m_pChartCtrl->m_clrBlock;			}
	virtual	COLORREF	GetStyleValueGraphBorderColor( void)
						{	return m_pChartCtrl->m_clrBlockBorder;		}
	virtual	COLORREF	GetStyleValueGraphGridColor( void)
						{	return m_pChartCtrl->m_clrLattice;			}
	virtual	COLORREF	GetStyleValueGridHeight( void)
						{	return m_pChartCtrl->m_nVertGap;			}
	virtual	COLORREF	GetStyleValueGridHLineColor( void)
						{	return m_pChartCtrl->m_clrScaleLine2;		}
	virtual	COLORREF	GetStyleValueGridVLineColor( void)
						{	return m_pChartCtrl->m_clrScaleLine2;		}
	virtual	COLORREF	GetStyleValueGridWidth( void)
						{	return m_pChartCtrl->m_nHorzGap;			}
	virtual	COLORREF	GetStyleValueNIDLGColor( void)
						{	return m_pChartCtrl->m_clrInquiryWindow;	}
	virtual	COLORREF	GetStyleValueScaleBGColor( void)
						{	return m_pChartCtrl->m_clrBackground;		}
	virtual	COLORREF	GetStyleValueScaleBorderColor( void)
						{	return m_pChartCtrl->m_clrBackBorder;		}
	virtual	COLORREF	GetStyleValueScaleTextColor( void)
						{	return m_pChartCtrl->m_clrScaleText;		}

	virtual COLORREF	GetToolTipBackColor( void)
						{	return m_pChartCtrl->GetToolTipBackColor();	}

protected:
	virtual CRect	GetAllBlockRegion( void)
					{	return m_pChartCtrl->GetAllBlockRegion();	}
	virtual CRect	GetMainBlockRegion()
					{	return m_pChartCtrl->GetMainBlockRegion();	}
	virtual CRect	GetOcxRegion() const
					{	return m_pChartCtrl->GetOcxRegion();	}
	virtual CRect	GetToolBarRegion() const
					{	return m_pChartCtrl->GetToolBarRegion();	}
	virtual CRect	GetVirtualRegion()
					{	return m_pChartCtrl->GetVirtualRegion();	}

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
	virtual int	GetAddInSiteID( void)
					{	return m_pChartCtrl->GetAddInSiteID();	}
	virtual int	GetBongCountMargin()
					{	return m_pChartCtrl->GetBongCountMargin();	}
	virtual int	GetDataCountForToolBar() const
					{	return m_pChartCtrl->GetDataCountForToolBar();	}
	virtual	int	GetFirstBlockHGridLineType( void)
					{	return m_pChartCtrl->m_pMainBlock->GetFirstBlockHGridLineType();	}
	virtual int	GetFirstBlockVGridLineType( void)
					{	return m_pChartCtrl->m_pMainBlock->GetFirstBlockVGridLineType();	}
	virtual int	GetFullDataCount( const bool bIsHorzScaleDataCheck) const
					{	return m_pChartCtrl->GetFullDataCount( bIsHorzScaleDataCheck);	}
	virtual int	GetFutureTimeCount( void)		// (2009/5/17 - Seung-Won, Bae) for Future Time
					{	return m_pChartCtrl->GetFutureTimeCount();	}
	virtual int	GetMarginInIndicatorInfo( const CString &strGraphName) const
					{	return m_pChartCtrl->GetMarginInIndicatorInfo( strGraphName);	}
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
	// X-Scale 다음 표시 가능 갯수 구하기 (yiilyoul@magicn.com - 20070126)
	virtual int	GetPageCountNextStep(const int nSliderPos_Prev, const int nSliderPos_New, const UINT nSBCode, int nMin, int nMax)
					{	return m_pChartCtrl->GetPageCountNextStep(nSliderPos_Prev, nSliderPos_New, nSBCode, nMin, nMax); }
//	virtual BOOL GetReverse()
//					{	return m_pChartCtrl->GetReverse(); }
	virtual int	GetRightMarginWidth( void)		// (2009/5/13 - Seung-Won, Bae) for Right Margin
					{	return m_pChartCtrl->GetRightMarginWidth();	}
	virtual int	GetShowTrendLineValue()
					{	return m_pChartCtrl->GetShowTrendLineValue();	}
	virtual int	GetSkinNumber() const
					{	return m_pChartCtrl->GetSkinNumber();	}
	virtual int	GetSliderMin( void)
					{	return m_pChartCtrl->GetSliderMin();	}
	virtual int	GetUserMargin( void) const
					{	return m_pChartCtrl->GetUserMargin();	}
	virtual int	OnCreate( LPCREATESTRUCT lpCreateStruct)
					{	return m_pChartCtrl->OnCreate( lpCreateStruct);	}
	virtual int	SaveChartCFG_SaveNow( void)
					{
						if( m_pChartCtrl->SaveChartCfg()) return m_pChartCtrl->m_SaveChartcfg.GetCfgCount() - 1;
						return -1;
					}
	virtual int	SetChartData2( long pChartData, int nCount, int nType)
					{	return m_pChartCtrl->SetChartData2( pChartData, nCount, nType);	}
	virtual int	ShowIndicatorConfigDlg( CIndicatorInfo *p_pIndicatorInfo)
					{	return m_pChartCtrl->ShowIndicatorConfigDlg( p_pIndicatorInfo);	}
	virtual int	GetMouseWheelIndex( void)
					{	return m_pChartCtrl->GetMouseWheelIndex();	}
	virtual int	GetPriceMinMaxShowType( void)
					{	return m_pChartCtrl->GetPriceMinMaxShowType();	}

protected:
	virtual long	GetAllProperties2( long lpData)
					{	return m_pChartCtrl->GetAllProperties2( lpData);	}
	virtual long	GetBase( LPCTSTR szKey)
					{	return m_pChartCtrl->GetBase( szKey);	}
	virtual long	GetBasePropIf()
					{	return m_pChartCtrl->GetBasePropIf();	}
	// (2007/3/4 - Seung-Won, Bae) Make Thread for the 'CLUTCH' of Real Update.
	// ******************** Caution : On Multi-Thread ********************
	virtual long	GetDrawingLoadTime( void)
					{	return m_pChartCtrl->m_lDrawingLoadTime;	}
	virtual long	GetOnePageDataCount()
					{	return m_pChartCtrl->GetOnePageDataCount();	}
	virtual long	GetStartPeriodOfPacket( LPCTSTR p_szPacketName)
					{	return m_pChartCtrl->GetStartPeriodOfPacket( p_szPacketName);	}

protected:
	virtual LPCTSTR		GetMasterValueRQ( const int nRow, const int nCol)
						{	return m_pChartCtrl->GetMasterValueRQ( nRow, nCol);	}

protected:
	virtual LPUNKNOWN	GetCtrlUnkownID( LPCTSTR szName)
					{	return m_pChartCtrl->GetCtrlUnkownID( szName);	}
	virtual LPUNKNOWN	GetDeskUnkownID()
					{	return m_pChartCtrl->GetDeskUnkownID();	}
	virtual LPUNKNOWN	GetAddInCustomInterface( LPCTSTR p_szAddInName)
					{	return m_pChartCtrl->GetAddInCustomInterface( p_szAddInName);	}

protected:
	virtual LRESULT	OnCursorExchange( WPARAM wParam, LPARAM lParam)
					{	return m_pChartCtrl->OnCursorExchange( wParam, lParam);	}
	virtual LRESULT	OnGetHWDesignUI( WPARAM wParam, LPARAM lParam)
					{	return NULL;	}	// (2006/1/11 - Seung-Won, Bae) Hanwha Platform Only
	virtual LRESULT	OnGetRealTableManager( WPARAM wParam, LPARAM lParam)
					{	return NULL;	}	// (2006/1/11 - Seung-Won, Bae) Hanwha Platform Only
	virtual LRESULT	OnGetUserMargin( WPARAM wParam, LPARAM lParam)
					{	return m_pChartCtrl->OnGetUserMargin( wParam, lParam);	}
	virtual LRESULT	OnGraphSelectedIndex( WPARAM wParam, LPARAM lParam)
					{	return m_pChartCtrl->OnGraphSelectedIndex( wParam, lParam);	}
	virtual LRESULT	OnRealData( WPARAM wParam, LPARAM lParam)
					{	return FALSE;	}	// (2006/1/11 - Seung-Won, Bae) Hanwha Platform Only
	virtual LRESULT	OnSetEtc( WPARAM wParam, LPARAM lParam)
					{	return m_pChartCtrl->OnSetEtc( wParam, lParam);	}
	virtual LRESULT	RmsgFormStateChange( WPARAM wParam, LPARAM lParam)
					{	return m_pChartCtrl->RmsgFormStateChange( wParam, lParam);	}
	virtual LRESULT	SendEventMsgToDLL( UINT message, WPARAM wParam = NULL, LPARAM lParam = NULL)
					{	return m_pChartCtrl->SendEventMsgToDLL( message, wParam, lParam );	}
	virtual LRESULT	WindowProc( UINT message, WPARAM wParam, LPARAM lParam)
					{	return m_pChartCtrl->WindowProc( message, wParam, lParam);	}

protected:
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
	virtual void	AddandDeleteIndicator( LPCTSTR strGraphName)
					{	m_pChartCtrl->AddandDeleteIndicator( strGraphName);	}
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
	virtual void	ClearTimeMark()
					{	m_pChartCtrl->ClearTimeMark();	}
	virtual void	CreateBufferBitmap( CDC *pdc, const CRect &rcBounds)
					{	m_pChartCtrl->CreateBufferBitmap( pdc, rcBounds);	}
	virtual void	CreateToolBar()
					{	m_pChartCtrl->CreateToolBar();	}
	virtual void	DeletedGraph( LPCTSTR lpszRQ, BOOL bRemovablePacketRQ)
					{	m_pChartCtrl->DeletedGraph( lpszRQ, bRemovablePacketRQ);	}
	virtual void	DeleteZoomInOutInquiryDlg()
					{	m_pChartCtrl->DeleteZoomInOutInquiryDlg();	}
	// (2006/2/6 - Seung-Won, Bae) DragZoomIn for DragZoom AddIn
	virtual void	DragZoomIn( const CPoint &p_ptStart, const CPoint &p_ptEnd)
					{	m_pChartCtrl->DragZoomIn( p_ptStart, p_ptEnd);	}
	virtual void	EnableBoundMark( BOOL p_bEnable)
					{	m_pChartCtrl->EnableBoundMark( p_bEnable);	}
	virtual void	EnableReceivingTimeShare( BOOL p_bEnable)
					{	m_pChartCtrl->EnableReceivingTimeShare( p_bEnable);	}
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
	virtual void	GetDefaultPath( CString &p_strDefaultPath) const
					{	p_strDefaultPath = m_pChartCtrl->GetDefaultPath();	}
	virtual void	GetImagePath( CString &p_strImagePath) const			// (2009/9/2 - Seung-Won, Bae) for Image Path
					{	p_strImagePath = m_pChartCtrl->GetImagePath();	}
	virtual void	GetListViewerData( BSTR *bstrTitle, BSTR *bstrHorzScale, BSTR *bstrPacketNamesPerBlock, BSTR *bstrPacketNames, BSTR *bstrSaveFilePath, BOOL *bViewPacketSequence, long *lpPacketList)
					{	m_pChartCtrl->GetListViewerData( bstrTitle, bstrHorzScale, bstrPacketNamesPerBlock, bstrPacketNames, bstrSaveFilePath, bViewPacketSequence, lpPacketList);	}
	virtual void	GetProgID( CString &p_strProgID)			// (2008/4/29 - Seung-Won, Bae) to use the ProgID in ChartMetaTable.dat
					{	m_pChartCtrl->GetProgID( p_strProgID);	}
	virtual long	InvalidateControl( LPCRECT lpRect = NULL, BOOL bErase = TRUE)
					{	m_pChartCtrl->InvalidateControl( lpRect, bErase);
						return m_pChartCtrl->m_lDrawingLoadTime;			} 
	virtual void	InvalidateOCX( const bool bErase = true)
					{	m_pChartCtrl->InvalidateOCX( bErase);	}
	virtual void	InvalidateOleControl(const bool bIsResetBlockRegion = false)
					{	m_pChartCtrl->InvalidateOleControl();	}
	virtual void	InvalidateRect( const CRect &region, const bool bErase)
					{	m_pChartCtrl->InvalidateRect( region, bErase);	}
	// (2007/3/5 - Seung-Won, Bae) Lock/Unlock WindowUpdate
	virtual void	LockWindowUpdate( void)
					{	m_pChartCtrl->LockWindowUpdate();	}
	// (2009/5/19 - Seung-Won, Bae) for Auto Resize of Print and Image Capture
	virtual void	LockScrollUIUpdateWithScrollInfo( BOOL p_bLockScrollUIUpdate)
					{	m_pChartCtrl->LockScrollUIUpdateWithScrollInfo( p_bLockScrollUIUpdate);	}
	virtual void	MakeSaveShortKey( LPCTSTR szFileName, short nFlag)
					{	m_pChartCtrl->MakeSaveShortKey( szFileName, nFlag);	}
	virtual void	NeoAddInEvent( const char *p_szAddInName, short p_nEventID, const char *p_szData, double p_dData, long p_lData)
					{	m_pChartCtrl->NeoAddInEvent( p_szAddInName, p_nEventID, p_szData, p_dData, p_lData);	}
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
	virtual void	NotifyRQStatusToAddin( CCommonInterfaceEnum::PACKET_RQ_STATUS eStatus, LPCTSTR lpszRQ)
					{	m_pChartCtrl->NotifyRQStatusToAddin( eStatus, lpszRQ);	}
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
	virtual void	OnCmdMsg( HMENU p_hMenu, UINT p_nItemID, int p_nItemPos, int p_nItemCount, int p_nCmdCode)	// (2009/6/5 - Seung-Won, Bae) for CmdUI of AddIn
					{	m_pChartCtrl->OnCmdMsg( p_hMenu, p_nItemID, p_nItemPos, p_nItemCount, p_nCmdCode);	}
	virtual void	OnDeleteBlock()
					{	m_pChartCtrl->OnDeleteBlock();	}
	virtual void	OnDraw( CDC *pdc, const CRect &rcBounds, const CRect &rcInvalid)
					{	m_pChartCtrl->OnDraw( pdc, rcBounds, rcInvalid);	}
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
	virtual void	OnPricechartMinmax()
					{	m_pChartCtrl->OnPricechartMinmax();	}
	virtual void	OnPrint()
					{	m_pChartCtrl->OnPrint();	}
	virtual void	OnRButtonDown( UINT nFlags, CPoint point)
					{	m_pChartCtrl->OnRButtonDown( nFlags, point);	}
	virtual void	OnRButtonUp( UINT nFlags, CPoint point)
					{	m_pChartCtrl->OnRButtonUp( nFlags, point);	}
	virtual void	OnResetState()
					{	m_pChartCtrl->OnResetState();	}
	virtual void	OnReturnOrg()
					{	m_pChartCtrl->OnReturnOrg();	}
	virtual void	OnReturnOrg( const bool bIsMessageBox)
					{	m_pChartCtrl->OnReturnOrg( bIsMessageBox);	}
	virtual void	OnReturnOrgDisplay()
					{	m_pChartCtrl->OnReturnOrgDisplay();	}
	virtual void	OnReverseYscale()
					{	m_pChartCtrl->OnReverseYscale();	}
	virtual void	OnScaleSettingMessage( const CCommonInterfaceEnum::ETCMESSAGETYPE eEtcMessageType, const int nData)
					{	m_pChartCtrl->OnScaleSettingMessage( eEtcMessageType, nData);	}
	virtual void	OnScreenMinmax()
					{	m_pChartCtrl->OnScreenMinmax();	}
	virtual void	OnSetFocus( CWnd *pOldWnd)
					{	m_pChartCtrl->OnSetFocus( pOldWnd);	}
	virtual void	OnSize( UINT nType, int cx, int cy)
					{	m_pChartCtrl->OnSize( nType, cx, cy);	}
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
	virtual void	OnSzBasePacketNameChanged()
					{	m_pChartCtrl->OnSzBasePacketNameChanged();	}
	virtual void	OnTimer( UINT nIDEvent)
					{	m_pChartCtrl->OnTimer( nIDEvent);	}
	virtual void	OnToExcel()
					{	m_pChartCtrl->OnToExcel();	}
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
	// (2007/2/22 - Seung-Won, Bae) User Zoom Changed.
	virtual void	OnUserZoomChanged( const int p_nDataCountInView)
					{	m_pChartCtrl->OnUserZoomChanged( p_nDataCountInView);	}
	virtual void	OnWindowPosChanged( WINDOWPOS *lpwndpos)
					{	m_pChartCtrl->OnWindowPosChanged( lpwndpos);	}
	// (2006/1/28 - Seung-Won, Bae) Reset Tool UI
	virtual void	ReleaseOtherToolUI( const CToolOptionInfo::TOOLOPTION p_eToolOption)
					{	m_pChartCtrl->ReleaseOtherToolUI( p_eToolOption);	}
	virtual void	RemoveRealItem( LPCTSTR szRealInfo)
					{	m_pChartCtrl->RemoveRealItem( szRealInfo);	}
	virtual void	ReplaceMATitle( CString &szpricema, int nOption = 0)
					{	m_pChartCtrl->ReplaceMATitle( szpricema, nOption);	}
	virtual void	RequestTRPacket( const CString &strGraphName, const CString &strPacketNames)
					{	m_pChartCtrl->RequestTRPacket( strGraphName, strPacketNames);	}
	virtual void	RequestTRPacket( const CString &strGraphName, const CString &strPacketNames, const int nCount, const bool bIsNew)
					{	m_pChartCtrl->RequestTRPacket( strGraphName, strPacketNames, nCount, bIsNew);	}
	virtual void	ResetChartCfgWith( short p_nIndex)
					{	m_pChartCtrl->ResetChartCfgWith( p_nIndex);	}
	virtual void	ResetScroll( BOOL p_bWithCalcDRI = FALSE)
					{	m_pChartCtrl->ResetScroll( p_bWithCalcDRI);	}
	virtual void	RunEnvironmentDlg( short nOption)
					{	m_pChartCtrl->RunEnvironmentDlg( nOption);	}
	virtual void	SaveandApplyGraphStyle( short nSaveDataType, short nChangeDataType)
					{	m_pChartCtrl->SaveandApplyGraphStyle( nSaveDataType, nChangeDataType);	}
	virtual void	SelectedGraph( LPCTSTR lpszRQ, BOOL bSendEvent/* = TRUE*/)
					{	m_pChartCtrl->SelectedGraph( lpszRQ, bSendEvent);	}
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
	virtual void	SetChartList( const char *p_szChartList)
					{	m_pChartCtrl->SetChartList( p_szChartList);	}
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
	virtual void	SetCursorExchange(CCursorShape::CHARTOBJECT enumChartObject)
					{	m_pChartCtrl->SetCursorExchange( enumChartObject);	}
	virtual void	SetCustomFont( LPCTSTR lpszNewValue)
					{	m_pChartCtrl->SetCustomFont( lpszNewValue);	}
	virtual void	SetDataIndexByDate( LPCTSTR pcszStartDate, LPCTSTR pcszEndDate)
					{	m_pChartCtrl->SetDataIndexByDate( pcszStartDate, pcszEndDate);	}
	virtual void	SetDataToMainBlock( const CString &strData)
					{	m_pChartCtrl->SetDataToMainBlock( strData);	}
	virtual void	SetDrdsAdviseRQ(LPCTSTR lpszRQ, BOOL bOnAdvise)
					{	m_pChartCtrl->SetDrdsAdviseRQ( lpszRQ, bOnAdvise);	}
	virtual void	SetDragModeType( short p_nDragModeType)
					{	m_pChartCtrl->SetDragModeType( p_nDragModeType);	}
	virtual void	SetFirstNMTRClock( void)						// (2009/12/11 - Seung-Won, Bae) Append lost NMTR data.
					{	m_pChartCtrl->SetFirstNMTRClock();	}
	virtual void	SetFutureTimeCount( int nFutureTimeCount)		// (2009/5/17 - Seung-Won, Bae) for Future Time
					{	m_pChartCtrl->SetFutureTimeCount( nFutureTimeCount);	}
	// (2009/2/19 - Seung-Won, Bae) Do not use SendMessage() for overloaded message situation. Use flag.
	virtual void	SetGraphSelectedDataIndex( const int nDataIndex, LPCTSTR pcszGraphName)
					{	m_pChartCtrl->SetGraphSelectedDataIndex( nDataIndex, pcszGraphName);	}
	virtual void	SetGraphThickness( const int nGraphThickness1)
					{	m_pChartCtrl->SetGraphThickness( nGraphThickness1);	}
	virtual void	SetGraphTypeStyle( const int nGraphType, const int nDrawStyle)
					{	m_pChartCtrl->SetGraphTypeStyle( nGraphType, nDrawStyle);	}
	virtual void	SetHtsPath( LPCTSTR pcszHtsPath)
					{	m_pChartCtrl->SetHtsPath( pcszHtsPath);	}
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
	virtual void	SetMainBlock_ToolBar( const bool bUserMode)
					{	m_pChartCtrl->SetMainBlock_ToolBar( bUserMode);	}
	virtual void	SetMultiChartDlg( long lpMultiChartDlg)
					{}	// (2006/1/11 - Seung-Won, Bae) Hanwha Platform Only
	virtual void	SetMultipleSameNameIndicator( BOOL p_bEnable)		// (2007/1/20 - Seung-Won, Bae) Support Multiple Same-Name Indicator.
					{	m_pChartCtrl->m_bMultipleSameNameIndicator = p_bEnable;	}
	virtual void	SetOnePageDataCount( long nOnePageDataCount)
					{	m_pChartCtrl->SetOnePageDataCount( nOnePageDataCount);	}
	virtual void	SetOnePageDataCount2( long nOnePageDataCount, BOOL bIncludeBongCountMargin)
					{	m_pChartCtrl->SetOnePageDataCount2( nOnePageDataCount, bIncludeBongCountMargin);	}
	virtual void	SetOnePageDataCountInTRCount( BOOL bIsTRCount, BOOL bIsNowChange)
					{	m_pChartCtrl->SetOnePageDataCountInTRCount( bIsTRCount, bIsNowChange);	}
	virtual void	SetOnRealProcessing( BOOL p_bOn)	// (2008/12/21 - Seung-Won, Bae) Manage On Real Processing
					{	m_pChartCtrl->SetOnRealProcessing( p_bOn);	}
	virtual void	SetOutsideCommonInterface( long lpOutsideCommonInterface)
					{	m_pChartCtrl->SetOutsideCommonInterface( lpOutsideCommonInterface);	}
	virtual void	SetPacketOption( LPCTSTR strPacketOption)
					{	m_pChartCtrl->SetPacketOption( strPacketOption);	}
	virtual void	SetRealData( LPCTSTR strPacketName, double dData, short nOption)
					{	m_pChartCtrl->SetRealData( strPacketName, dData, nOption);	}
	virtual void	SetRightMarginWidth( int nRightMarginWidth)		// (2009/5/13 - Seung-Won, Bae) for Right Margin
					{	m_pChartCtrl->SetRightMarginWidth( nRightMarginWidth);	}
	virtual void	SetRscMng( long lKey)
					{	m_pChartCtrl->SetRscMng( lKey);	}
	virtual void	SetRunTimeMode( BOOL bRunTimeMode)
					{	m_pChartCtrl->SetRunTimeMode( bRunTimeMode);	}
	virtual void	SetShowMinMaxPrice( const bool bShow, const bool bOutside = true)
					{	m_pChartCtrl->SetShowMinMaxPrice( bShow, bOutside);	}
	virtual void	SetShowTrendLineValue( const int nPosition, const bool bOutside = true)
					{	m_pChartCtrl->SetShowTrendLineValue( nPosition, bOutside);	}
	virtual void	SetStateWholeView( const int nParam = 0)
					{	m_pChartCtrl->SetStateWholeView( nParam);	}
	virtual void	SetStrBlockRegion( LPCTSTR lpszNewValue)
					{	m_pChartCtrl->SetStrBlockRegion( lpszNewValue);	}
	virtual void	SetTimeMark( LPCTSTR p_szMarkTime, BOOL p_bWithScroll)
					{	m_pChartCtrl->SetTimeMark( p_szMarkTime, p_bWithScroll);	}
	virtual void	SetUserEnvironmentAndIndicatorListFileName( const CString &strFileName)
					{	m_pChartCtrl->SetUserEnvironmentAndIndicatorListFileName( strFileName);	}
	virtual void	SetWholeView( const BOOL bWholeView)
					{	m_pChartCtrl->SetWholeView( bWholeView);	}
	virtual void	ShowAnalysisToolText( BOOL p_bTimeLine, BOOL p_bPriceLineLeft, BOOL p_bPriceLineRight, BOOL p_bTrendLine, BOOL p_bTrisectionLine, BOOL p_bQuadrisectionLine, BOOL p_bFibonacciRetraceLine)
					{	m_pChartCtrl->ShowAnalysisToolText( p_bTimeLine, p_bPriceLineLeft, p_bPriceLineRight, p_bTrendLine, p_bTrisectionLine, p_bQuadrisectionLine, p_bFibonacciRetraceLine);	}
	virtual void	ShowEnvironmentDlg()
					{	m_pChartCtrl->ShowEnvironmentDlg();	}
	virtual void	ShowIndicatorConfigDlg()
					{	m_pChartCtrl->ShowIndicatorConfigDlg();	}
	virtual void	ShowInnerToolBar( BOOL p_bShow)
					{	m_pChartCtrl->ShowInnerToolBar( p_bShow);	}
	virtual void	testLoadAnalysisTools( short nToolOption, short nOption)
					{	m_pChartCtrl->testLoadAnalysisTools( nToolOption, nOption);	}
	// (2007/3/5 - Seung-Won, Bae) Lock/Unlock WindowUpdate
	virtual void	UnlockWindowUpdate( void)
					{	m_pChartCtrl->UnlockWindowUpdate();	}
	virtual void	UpdateZoomAndScrollInfo( int p_nFullRange, int p_nStartPos, int p_nViewCount)
					{	m_pChartCtrl->UpdateZoomAndScrollInfo( p_nFullRange, p_nStartPos, p_nViewCount);	}
	virtual void	UserFileVersionChecker()
					{	m_pChartCtrl->UserFileVersionChecker();	}
	virtual void	WriteUserFile( const CString &strFileName)
					{	m_pChartCtrl->WriteUserFile( strFileName);	}
	// 조건값 사용자 지정 가능 - ojtaso (20070913)
	virtual void	ReCalculateIndicator(void)
					{	m_pChartCtrl->ReCalculateIndicator(); }
	// VertScaleGroup의 가장 처음 Graph의 타입 : 복수종목 - ojtaso (20070504)
	virtual CScaleBaseData::HORZSCALEDRAWERTYPE GetMasterValueType(const int nRow, const int nCol)
					{	return m_pChartCtrl->GetMasterValueType(nRow, nCol);	}
	// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
	virtual const char *	GetUseSTDataName()
							{	return m_pChartCtrl->GetUseSTDataName();	}
	// FX차트 주문 연동 - onlyojt (20091204)
	virtual BOOL OnOrderInfomationChanged(const double dPirce, const int Qty, const int nOrderType, LPCTSTR szOrgOrderID, LPCTSTR strCustItem1, LPCTSTR strCustItem2, LPCTSTR strCustItem3)
					{ return m_pChartCtrl->OnOrderInfomationChanged(dPirce, Qty, nOrderType, szOrgOrderID, strCustItem1, strCustItem2, strCustItem3); }
	virtual void	SetMouseWheelIndex(int nMouseWheelIndex)
					{	m_pChartCtrl->SetMouseWheelIndex(nMouseWheelIndex);	}
	virtual void	SetPriceMinMaxShowType(int nMinMaxRatio)
					{	m_pChartCtrl->SetPriceMinMaxShowType(nMinMaxRatio);	}

	//	시간구분선
	virtual	int		GetFirstBlockHScaleGridType_TimeGubun( int nType)
					{	return m_pChartCtrl->m_pMainBlock->GetFirstDailyScaleGridType((CScaleBaseData::SCALEGRIDTIMEGUBUN)nType);	}
	virtual	BOOL	GetFirstBlockHScaleType_TimeGubun( int nType)
					{	return m_pChartCtrl->m_pMainBlock->GetFirstDailyScaleTime((CScaleBaseData::SCALEGRIDTIMEGUBUN)nType);	}
	virtual	COLORREF	GetStyleValueHScaleGridColor_TimeGubun( int nType)
					{	return m_pChartCtrl->m_pMainBlock->GetFirstBlockGridLineColor_TimeGubun(nType);	}
	virtual int		GetStyleValueScaleLineThickness_TimeGubun( int nType)
					{	return m_pChartCtrl->m_pMainBlock->GetFirstBlockGridLineWidth_TimeGubun(nType);	}
	virtual	BOOL	SetAllHScaleGridType_TimeGubun(int p_nHScaleGridType, BOOL p_bCheckTime = FALSE, int p_nHScaleTimeType = 0)
					{	m_pChartCtrl->m_pMainBlock->SetAllDailyScaleGridType((CScaleBaseData::SCALEGRIDTYPE)p_nHScaleGridType, (CScaleBaseData::SCALEGRIDTIMEGUBUN)p_nHScaleTimeType, p_bCheckTime ); return TRUE;	}
	virtual	BOOL	SetStyleValueHScaleGridColor_TimeGubun(	COLORREF p_clrHScaleGrid_DayColor, COLORREF p_clrHScaleGrid_WeekColor, COLORREF p_clrHScaleGrid_MonColor, COLORREF p_clrHScaleGrid_YearColor, BOOL p_bWithUpdate = FALSE)
					{
						m_pChartCtrl->m_pMainBlock->SetAllScaleGridColor_TimeGubun(CScaleBaseData::MINTICK_DAY_GUBUN,	p_clrHScaleGrid_DayColor);
						m_pChartCtrl->m_pMainBlock->SetAllScaleGridColor_TimeGubun(CScaleBaseData::DAYCHART_WEEK_GUBUN, p_clrHScaleGrid_WeekColor);
						m_pChartCtrl->m_pMainBlock->SetAllScaleGridColor_TimeGubun(CScaleBaseData::DAYCHART_MON_GUBUN,	p_clrHScaleGrid_MonColor);
						m_pChartCtrl->m_pMainBlock->SetAllScaleGridColor_TimeGubun(CScaleBaseData::DAYCHART_YEAR_GUBUN,	p_clrHScaleGrid_YearColor);
						return TRUE;
					}
	virtual	BOOL	SetStyleValueScaleLineThickness_TimeGubun( int nType, int p_nLineThickness)
					{ m_pChartCtrl->m_pMainBlock->SetAllScaleGridWidth_TimeGubun((CScaleBaseData::SCALEGRIDTIMEGUBUN)nType, p_nLineThickness);	return TRUE; }
};

#endif // !defined( AFX_IWCHARTCTRL_H__6470428F_7AB9_4790_B60C_C08F1BDF5990__INCLUDED_)
