// IAddInDllBase.h: interface for the IAddInDllBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IADDINDLLBASE_H__5DB5FE98_14A1_4309_8707_A7423C55F48B__INCLUDED_)
#define AFX_IADDINDLLBASE_H__5DB5FE98_14A1_4309_8707_A7423C55F48B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ADDINEVENT( EventType)		\
	EAE_##EventType

interface IAddInDllBase : public IUnknown
{
protected:
	virtual ~IAddInDllBase()	{};

// (2006/2/12 - Seung-Won, Bae) Define Base Enum Type of AddIn Event
public:
	typedef enum
	{
		EAE_NOT_DEFINED = -1,
		//	********************************************************************************
		//	* Caution! Do not change the Order. It is already referenced by other AddIn DLLs
		//	********************************************************************************
		ADDINEVENT( OnGSharedDataChange),			// OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare)
		ADDINEVENT( OnLSharedDataChange),			// OnLSharedDataChange( const char *p_szLShareKey, const char *p_szLShareValue)
		ADDINEVENT( OnSharedDataChange_Not_Used),	// OnSharedDataChange( const char *p_szShareName, const char *p_szSharedData, BOOL p_bStartShare)
		ADDINEVENT( OnResetChart),					// OnResetChart( void)
		ADDINEVENT( OnDrawBeforeGraph),				// OnDrawBeforeGraph( HDC p_hDC)
		ADDINEVENT( OnDrawAfterGraph),				// OnDrawAfterGraph( HDC p_hDC)
		ADDINEVENT( OnPacketData),					// OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset)
		//	********************************************************************************
		//	* Caution! Do not change the Order. It is already referenced by other AddIn DLLs
		//	********************************************************************************
		ADDINEVENT( OnUpdatePacketData_Not_Used),				// OnUpdatePacketData( BOOL p_bTrUpdate)
		ADDINEVENT( OnTimer),									// OnTimer( UINT p_uTimerID)
		ADDINEVENT( OnCaptureChanged),							// OnCaptureChanged(	HWND p_hWnd)
		ADDINEVENT( OnDrdsData_Not_Used),						// OnDrdsData( WPARAM p_wParam, LPARAM p_lParam)
		ADDINEVENT( OnDrdsPacketEnd_Not_Used),					// OnDrdsPacketEnd( WPARAM p_wParam, LPARAM p_lParam)
		ADDINEVENT( OnRealData_Not_Used),						// Not Used
		ADDINEVENT( OnRealRedcordEnd_Not_Used),					// Not Used
		ADDINEVENT( OnCmdMsg),									// OnCmdMsg(			UINT nID, int nCode, ICmdUI *p_pICmdUI)
		ADDINEVENT( OnAddInToolCommand_EAICommandType),			// OnAddInToolCommand(	const EAICommandType p_eCommandID)
		ADDINEVENT( OnAddInToolCommand_EAICommandType_char),	// OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam)
		//	********************************************************************************
		//	* Caution! Do not change the Order. It is already referenced by other AddIn DLLs
		//	********************************************************************************
		ADDINEVENT( OnAddInToolCommand_EAICommandType_char_long),						// OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
		ADDINEVENT( OnAddInToolCommand_EAICommandType_CBlockBehaviorData_Not_Used),		// OnAddInToolCommand(	const EAICommandType p_eCommandID, const CBlockBehaviorData *p_pParam)
		ADDINEVENT( OnAddInToolCommand_EAICommandType_CStringList_Not_Used),			// Not Used.
		ADDINEVENT( OnAddInToolCommand_EAICommandType_CStyleDataAI_Not_Used),			// Not Used.
		ADDINEVENT( OnAddInToolCommand_EAICommandType_IAddInDllBase),					// OnAddInToolCommand(	const EAICommandType p_eCommandID, const IAddInDllBase *p_pParam)
		ADDINEVENT( OnAddInToolCommand_EAICommandType_long),							// OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam)
		ADDINEVENT( OnAddInToolCommand_EAICommandType_long_char),						// OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam)
		ADDINEVENT( OnAddInToolCommand_EAICommandType_long_char_char_char),				// OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3)
		ADDINEVENT( OnAddInToolCommand_EAICommandType_long_long),						// OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
		ADDINEVENT( OnAddInToolCommand_EAICommandType_long_long_long_long_long_long_long_long_long),	// OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2, const long p_lParam3, const long p_lParam4, const long p_lParam5, const long p_lParam6, const long p_lParam7, const long p_lParam8, const long p_lParam9)
		//	********************************************************************************
		//	* Caution! Do not change the Order. It is already referenced by other AddIn DLLs
		//	********************************************************************************
		ADDINEVENT( OnAddInToolCommand_long_EAICommandType),					// OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID)
		ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char),				// OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam)
		ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char_char_CObject),	// OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, CObject *p_pParam)
		ADDINEVENT( OnAddInToolCommand_long_EAICommandType_long),				// OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam)
		ADDINEVENT( OnAddInToolCommand_long_EAICommandType_long_long),			// OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
		ADDINEVENT( OnKeyDown),													// OnKeyDown(			UINT nChar, UINT nRepCnt, UINT nFlags)
		ADDINEVENT( OnKeyUp),													// OnKeyUp(			UINT nChar, UINT nRepCnt, UINT nFlags)
		ADDINEVENT( OnMouseWheel),												// OnMouseWheel(	UINT nFlags, short zDelta, POINT pt, const DWORD p_dwMouseActionID)
		ADDINEVENT( OnMouseMove),												// OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
		ADDINEVENT( OnLButtonDown),												// OnLButtonDown(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
		//	********************************************************************************
		//	* Caution! Do not change the Order. It is already referenced by other AddIn DLLs
		//	********************************************************************************
		ADDINEVENT( OnLButtonUp),												// OnLButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
		ADDINEVENT( OnLButtonDblClk),											// OnLButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
		ADDINEVENT( OnRButtonDown),												// OnRButtonDown(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
		ADDINEVENT( OnRButtonUp),												// OnRButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
		ADDINEVENT( OnRButtonDblClk),											// OnRButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
		ADDINEVENT( OnAddInToolCommand_EAICommandType_HDC_UINT_POINT_DWORD),	// OnAddInToolCommand( const EAICommandType p_eCommandID, HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
		ADDINEVENT( OnAddInToolCommand_EAICommandType_HWND_POINT_Not_Used),		// OnAddInToolCommand(	const EAICommandType p_eCommandID, HWND p_hWnd, POINT &point)
		ADDINEVENT( OnAddInToolCommand_long_EAICommandType_IGraphInfo),			// OnAddInToolCommand( long &p_lResult, const EAICommandType p_eCommandID, IGraphInfo *p_pIGraphInfo)
		//	********************************************************************************
		//	* Caution! Do not change the Order. It is already referenced by other AddIn DLLs
		//	********************************************************************************
		ADDINEVENT( OnResetMainBlock),						// OnResetMainBlock( void)
		ADDINEVENT( OnUserIndiatorConditionLoaded),			// OnUserIndiatorConditionLoaded( void)
		ADDINEVENT( OnChartItemChanged),					// OnChartItemChanged( void)
		ADDINEVENT( OnGraphDataChanged),					// OnGraphDataChanged( void)
		ADDINEVENT( OnBlockRemoved),						// OnBlockRemoved( const int p_nRow, const int p_nColumn)
		ADDINEVENT( OnGraphAdded),							// OnGraphAdded( const char *p_szGraphOption)
		ADDINEVENT( OnManualRealData),						// OnManualRealData( const char *p_szPacketName, const double p_dData, const int p_nOption)
		ADDINEVENT( OnContextMenu),							// OnContextMenu( HWND p_hWnd, POINT &p_pt)
		ADDINEVENT( OnGraphDeleted),						// OnGraphDeleted( const char *p_szGraphName)
		ADDINEVENT( OnPacketTruncated),						// OnPacketTruncated( const int p_nDataCount)
		ADDINEVENT( OnBlockChanged),						// OnBlockChanged( const CBlockBaseData::BLOCKBEHAVIOR p_eChangeType, const int p_nSourceRow, const int p_nSourceColumn, const int p_nTargetRow, const int p_nTargetColumn)
		ADDINEVENT( OnToolCommand),							// OnToolCommand( const int p_nToolType, const int p_nCmdOption)
		ADDINEVENT( OnRealPacketEnd),						// OnRealPacketEnd( void)
		//	********************************************************************************
		//	* Caution! Do not change the Order. It is already referenced by other AddIn DLLs
		//	********************************************************************************
		ADDINEVENT( OnAddInToolCommand_EAICommandType_char_char),	// OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2)
		ADDINEVENT( OnAddInMsg),									// OnAddInMsg( const char *p_szAddInItemName, const long p_lParam)
		ADDINEVENT( OnLoadEnvironment),								// OnLoadEnvironment( CPropertyMap *p_pAddInEnvMap)
		ADDINEVENT( OnSaveEnvironment),								// OnSaveEnvironment( CPropertyMap *p_pAddInEnvMap)
		ADDINEVENT( DrawCursor),									// DrawCursor( CDC *p_pDC)
		ADDINEVENT( OnRecalculationComplete),						// OnRecalculationComplete( void)
		// 복수종목 지원을 위해 추가 : 복수종목 - ojtaso (20070125)
		ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char_char_char_CObject),	// OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3, CObject *p_pParam)
		// RQ별 Packet : 복수종목 - ojtaso (20070111)
		ADDINEVENT( OnPacketDataMultiItem),											// OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
		// 복수종목별 Real반영 : 복수종목 - ojtaso (20070131)
		ADDINEVENT( OnRealPacketEndMultiItem_Not_Used),								// OnRealPacketEndMultiItem(LPCTSTR lpszRQ)
		ADDINEVENT( OnUpdatePacketDataMultiItem_Not_Used),							// OnUpdatePacketDataMultiItem(LPCTSTR lpszRQ, BOOL p_bTrUpdate)
		ADDINEVENT( OnRealData),													// OnRealData( const char *p_szPacketName, double p_dData)
	} EAddInEventType;
};

#endif // !defined(AFX_IADDINDLLBASE_H__5DB5FE98_14A1_4309_8707_A7423C55F48B__INCLUDED_)
