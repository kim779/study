// AddInManager.cpp: implementation of the CAddInManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AddInManager.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for g_iMetaTable
#include "AddInManagerImp.h"								// for CAddInManagerImp

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAddInManager::CAddInManager( IChartCtrl *p_pIChartCtrl, const char *p_szOcxPath)	// (2006/1/18 - Seung-Won, Bae) for Load AddIn DLL in OCX Path
{
	// (2008/1/20 - Seung-Won, Bae) for MultiLanguage.
	g_iMetaTable.LoadMetaTable();

	m_pAddInManagerImpl = new CAddInManagerImp( p_pIChartCtrl, p_szOcxPath);
}

CAddInManager::~CAddInManager()
{
	if( m_pAddInManagerImpl)
	{
		delete m_pAddInManagerImpl;
		m_pAddInManagerImpl = NULL;
	}
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) AddIn DLL을 관리한다.
//////////////////////////////////////////////////////////////////////

// (2004.10.07, 배승원) Addin DLL을 Loading하는 Interface를 제공한다.
// (2004.10.13, 배승원) AddIn DLL Loading시 바로 AddIn Interface를 제공한다.
// (2005.01.18, 배승원) AddIn Object의 Interface를 제공하지 않고, DLL Wrapper를 전달하여,
//		동적인 연결이 가능하도록 처리한다.
BOOL CAddInManager::LoadAddIn( const char *p_szAddInModule, const BOOL p_bOverExclusive)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->LoadMultipleAddInDll( p_szAddInModule, p_bOverExclusive);
	return FALSE;
}

// (2005/10/25 - Seung-Won, Bae) Unload AddIn DLL
BOOL CAddInManager::UnloadAddIn( const char *p_szAddInModule)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->UnloadMultipleAddInDll( p_szAddInModule);
	return FALSE;
}

// (2004.10.11, 배승원) Addin DLL을 일괄 Unloading하는 Interface를 제공한다.
//		AddIn Unload시에서 정상적인 OCX를 Access할 수 있도록 Window Destroy Time에서 일괄 Unload하도록 한다.
void CAddInManager::UnloadAllAddin( void)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->UnloadAllAddin();
}

// (2006/1/7 - Seung-Won, Bae) Interface for Spacial Interface of AddIn DLL
IUnknown *CAddInManager::GetAddInCustomInterface( const char *p_szAddInModule)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->GetAddInCustomInterface( p_szAddInModule);
	return NULL;
}

// (2006/8/25 - Seung-Won, Bae) Check AddIn Loaded (DLL:IDS)
BOOL CAddInManager::IsAddInLoaded( const char *p_szAddInItem)
{
	if( !m_pAddInManagerImpl) return FALSE;
	return m_pAddInManagerImpl->IsAddInLoaded( p_szAddInItem);
}

// (2006/1/7 - Seung-Won, Bae) Set Command to the AddIn DLL
BOOL CAddInManager::InvokeAddIn( const char *p_szAddInDllName, short p_nCommandType, long p_lData)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->InvokeAddIn( p_szAddInDllName, p_nCommandType, p_lData);
	return FALSE;
}
BOOL CAddInManager::InvokeAddInStr( const char *p_szAddInDllName, const char *p_szCommandName, LPCTSTR p_szData)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->InvokeAddInStr( p_szAddInDllName, p_szCommandName, p_szData);
	return FALSE;
}

// (2007/3/4 - Seung-Won, Bae) Notify OCX Destroying to Addin
void CAddInManager::OnOcxDestroyWindow( void)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnOcxDestroyWindow();
}

// (2009/12/2 - Seung-Won, Bae) Notify to other modules.
void CAddInManager::ChangeChartMode( CChartInfo::CHART_MODE p_eChartMode)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->ChangeChartMode( p_eChartMode);
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) AddIn DLL에 OCX Event를 전달한다.
//////////////////////////////////////////////////////////////////////

// (2004.10.11, 배승원) Global 공유를 전달하는 Interface를 제공한다.
// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
void CAddInManager::OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnGSharedDataChange( p_szGShareKey, p_szGShareValue, p_bStartShare);
}
void CAddInManager::OnLSharedDataChange( const char *p_szLShareKey, const char *p_szLShareValue)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnLSharedDataChange( p_szLShareKey, p_szLShareValue);
}

// (2004.10.11, 배승원) AddIn DLL들에게 ResetChart 상황을 알려 처리되도록 한다.
void CAddInManager::OnResetChart( void)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnResetChart();
}

// (2004.10.11, 배승원) Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
void CAddInManager::OnDrawBeforeGraph( CDC *p_pDC)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnDrawBeforeGraph( p_pDC ? p_pDC->GetSafeHdc() : NULL);
}

// (2004.12.25, 배승원) Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
void CAddInManager::OnDrawAfterGraph( CDC *p_pDC)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnDrawAfterGraph( p_pDC ? p_pDC->GetSafeHdc() : NULL);
}

// RQ별 Packet : 복수종목 - ojtaso (20070111)
void CAddInManager::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnPacketDataMultiItem(lpszRQ, p_szPacketNames, p_bWithReset);
}

// (2004.10.29, 배승원) Timer Event를 처리하는 Interface를 제공한다.
void CAddInManager::OnTimer( UINT p_uTimerID)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnTimer( p_uTimerID);
}

// (2004.10.29, 배승원) Mouse Capture Release Event를 AddIn DLL에게 전달하도록 한다.
//		주의! OnCaptureChanged()는 SetCapture()/RelaseCapture()에 의한 경우 외에도,
//			Focus 나 Activate, ToolTip, TrackMenu등에 의해서도 수시로 (SetCapture없이)
//			또한 자기 자신을 다시 SetCapture()하는 경우에도 호출됨을 유의한다.
void CAddInManager::OnCaptureChanged( CWnd *pWnd, const CBitmap *p_bmpMainOSB)	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnCaptureChanged(	pWnd, ( HBITMAP)( p_bmpMainOSB ? p_bmpMainOSB->GetSafeHandle() : NULL));
}

// (2006/9/25 - Seung-Won, Bae) On Reset MainBlock
void CAddInManager::OnResetMainBlock( void)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnResetMainBlock();
}

// (2006/9/25 - Seung-Won, Bae) On User IndicatorInfo Loaded
void CAddInManager::OnUserIndiatorConditionLoaded( void)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnUserIndiatorConditionLoaded();
}

// (2006/9/25 - Seung-Won, Bae) On ChartItem Changed.
void CAddInManager::OnChartItemChanged( void)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnChartItemChanged();
}

// (2006/9/25 - Seung-Won, Bae) On GraphData Changed.
void CAddInManager::OnGraphDataChanged( void)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnGraphDataChanged();
}

// (2006/9/25 - Seung-Won, Bae) On Block Removed.
void CAddInManager::OnBlockRemoved( const int p_nRow, const int p_nColumn)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnBlockRemoved( p_nRow, p_nColumn);
}

// (2006/9/25 - Seung-Won, Bae) On Graph Added.
void CAddInManager::OnGraphAdded( const char *p_szGraphName, const char *p_szIndicatorName)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnGraphAdded( p_szGraphName, p_szIndicatorName);
}

// (2006/9/25 - Seung-Won, Bae) On Graph Added.
void CAddInManager::OnManualRealData( const char *p_szPacketName, const double p_dData, const int p_nOption)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnManualRealData( p_szPacketName, p_dData, p_nOption);
}

// (2006/9/27 - Seung-Won, Bae) On Context Menu.
void CAddInManager::OnContextMenu( CWnd *p_pWnd, CPoint &p_pt)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnContextMenu( p_pWnd ? p_pWnd->GetSafeHwnd() : NULL, p_pt);
}

// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
void CAddInManager::OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnGraphDeleted( p_szGraphName, p_szIndicatorName);
}

// (2006/9/27 - Seung-Won, Bae) On Packet Truncated.
void CAddInManager::OnPacketTruncated( const int p_nDataCount)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnPacketTruncated( p_nDataCount);
}

// (2006/9/27 - Seung-Won, Bae) On Block Changed.
void CAddInManager::OnBlockChanged( const CBlockBaseData::BLOCKBEHAVIOR p_eChangeType, const int p_nSourceRow, const int p_nSourceColumn, const int p_nTargetRow, const int p_nTargetColumn)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnBlockChanged( p_eChangeType, p_nSourceRow, p_nSourceColumn, p_nTargetRow, p_nTargetColumn);
}

// (2006/9/27 - Seung-Won, Bae) On Block Changed.
void CAddInManager::OnToolCommand( const int p_nToolType, const int p_nCmdOption)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnToolCommand( p_nToolType, p_nCmdOption);
}

// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
void CAddInManager::OnLoadEnvironment( CPropertyMap *p_pAddInEnvMap)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnLoadEnvironment( p_pAddInEnvMap);
}
void CAddInManager::OnSaveEnvironment( CPropertyMap *p_pAddInEnvMap)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnSaveEnvironment( p_pAddInEnvMap);
}

// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
void CAddInManager::ClearCursor( CDC *p_pDC, const CBitmap *p_bmpMainOSB, CRect *p_pRect)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->ClearCursor( p_pDC, p_bmpMainOSB, p_pRect);
}
void CAddInManager::DrawCursor( CDC *p_pDC)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->DrawCursor( p_pDC);
}

// (2007/1/18 - Seung-Won, Bae) Notify Recalculation.
void CAddInManager::OnRecalculationComplete( void)
{
	if( m_pAddInManagerImpl) m_pAddInManagerImpl->OnRecalculationComplete();
}

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////
BOOL CAddInManager::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo);
	return FALSE;
}
BOOL CAddInManager::OnAddInMsg( const char *p_szAddInItemName, const long p_lParam)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnAddInMsg( p_szAddInItemName, p_lParam);
	return FALSE;
}
BOOL CAddInManager::OnAddInToolCommand(	const EAICommandType p_eCommandID)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnAddInToolCommand( p_eCommandID);
	return FALSE;
}
BOOL CAddInManager::OnAddInToolCommand(	const EAICommandType p_eCommandID, CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnAddInToolCommand( p_eCommandID, pDC ? pDC->GetSafeHdc() : NULL, nFlags, point, p_dwMouseActionID);
	return FALSE;
}
BOOL CAddInManager::OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnAddInToolCommand( p_eCommandID, p_szParam);
	return FALSE;
}
BOOL CAddInManager::OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnAddInToolCommand( p_eCommandID, p_szParam, p_lParam);
	return FALSE;
}
BOOL CAddInManager::OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnAddInToolCommand( p_eCommandID, p_szParam1, p_szParam2);
	return FALSE;
}
BOOL CAddInManager::OnAddInToolCommand(	const EAICommandType p_eCommandID, const IAddInDll *p_pParam)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnAddInToolCommand( p_eCommandID, p_pParam);
	return FALSE;
}
BOOL CAddInManager::OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnAddInToolCommand( p_eCommandID, p_lParam);
	return FALSE;
}
BOOL CAddInManager::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnAddInToolCommand( p_eCommandID, p_lParam, p_szParam);
	return FALSE;
}
BOOL CAddInManager::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnAddInToolCommand( p_eCommandID, p_lParam, p_szParam1, p_szParam2, p_szParam3);
	return FALSE;
}
BOOL CAddInManager::OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnAddInToolCommand( p_eCommandID, p_lParam1, p_lParam2);
	return FALSE;
}
BOOL CAddInManager::OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2, const long p_lParam3, const long p_lParam4, const long p_lParam5, const long p_lParam6, const long p_lParam7, const long p_lParam8, const long p_lParam9)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnAddInToolCommand( p_eCommandID, p_lParam1, p_lParam2, p_lParam3, p_lParam4, p_lParam5, p_lParam6, p_lParam7, p_lParam8, p_lParam9);
	return FALSE;
}
BOOL CAddInManager::OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnAddInToolCommand( p_lResult, p_eCommandID);
	return FALSE;
}
BOOL CAddInManager::OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, CIndicatorInfo *p_pParam)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnAddInToolCommand( p_lResult, p_eCommandID, p_pParam);
	return FALSE;
}
BOOL CAddInManager::OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnAddInToolCommand( p_lResult, p_eCommandID, p_szParam);
	return FALSE;
}
BOOL CAddInManager::OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, CObject *p_pParam)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnAddInToolCommand( p_lResult, p_eCommandID, p_szParam1, p_szParam2, p_pParam);
	return FALSE;
}

// 복수종목 지원을 위해 추가 : 복수종목 - ojtaso (20070125)
BOOL CAddInManager::OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3, CObject *p_pParam)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnAddInToolCommand( p_lResult, p_eCommandID, p_szParam1, p_szParam2, p_szParam3, p_pParam);
	return FALSE;
}

BOOL CAddInManager::OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnAddInToolCommand( p_lResult, p_eCommandID, p_lParam);
	return FALSE;
}
BOOL CAddInManager::OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnAddInToolCommand( p_lResult, p_eCommandID, p_lParam1, p_lParam2);
	return FALSE;
}
BOOL CAddInManager::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnKeyDown( nChar, nRepCnt, nFlags);
	return FALSE;
}
BOOL CAddInManager::OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnKeyUp( nChar, nRepCnt, nFlags);
	return FALSE;
}
BOOL CAddInManager::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt, const DWORD p_dwMouseActionID)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnMouseWheel( nFlags, zDelta, pt, p_dwMouseActionID);
	return FALSE;
}
BOOL CAddInManager::OnMouseMove( CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID, const CBitmap *p_bmpMainOSB)	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnMouseMove( pDC ? pDC->GetSafeHdc() : NULL, nFlags, point, p_dwMouseActionID, ( HBITMAP)( p_bmpMainOSB ? p_bmpMainOSB->GetSafeHandle() : NULL));
	return FALSE;
}
BOOL CAddInManager::OnLButtonDown( CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnLButtonDown( pDC ? pDC->GetSafeHdc() : NULL, nFlags, point, p_dwMouseActionID);
	return FALSE;
}
BOOL CAddInManager::OnLButtonUp( CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnLButtonUp( pDC ? pDC->GetSafeHdc() : NULL, nFlags, point, p_dwMouseActionID);
	return FALSE;
}
BOOL CAddInManager::OnLButtonDblClk( CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnLButtonDblClk( pDC ? pDC->GetSafeHdc() : NULL, nFlags, point, p_dwMouseActionID);
	return FALSE;
}
BOOL CAddInManager::OnRButtonDown( CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnRButtonDown(	pDC ? pDC->GetSafeHdc() : NULL, nFlags, point, p_dwMouseActionID);
	return FALSE;
}
BOOL CAddInManager::OnRButtonUp( CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnRButtonUp( pDC ? pDC->GetSafeHdc() : NULL, nFlags, point, p_dwMouseActionID);
	return FALSE;
}
BOOL CAddInManager::OnRButtonDblClk( CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)
{
	if( m_pAddInManagerImpl) return m_pAddInManagerImpl->OnRButtonDblClk( pDC ? pDC->GetSafeHdc() : NULL, nFlags, point, p_dwMouseActionID);
	return FALSE;
}
