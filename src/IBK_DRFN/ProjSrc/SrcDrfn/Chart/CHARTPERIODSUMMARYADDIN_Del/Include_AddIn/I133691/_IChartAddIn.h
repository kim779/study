// IChartAddIn.h: interface for the IChartAddIn class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_ICHARTADDIN_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_)
#define AFX_ICHARTADDIN_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../_IAddInDllBase.h"		// for IAddInDllBase::EAddInEventType
#include "_AddInVer.h"				// for AddIn Version Define

enum EAICommandType;
class CBlockBehaviorData;
interface ICmdUI;
interface IGraphInfo;
interface IPropertyMap;
interface IChartAddIn : public IUnknown  
{
protected:
	virtual ~IChartAddIn()	{}

// (2004.10.13, 배승원) 기본 Manager가 등록되어 있는지 확인하는 Interface를 제공한다.
public:
	virtual BOOL	HasFullManager( void) = 0;

// [05/07/08] AddIn 전용 Interface를 제공한다.
//		*. 주의. 반듯이 전용 Interface Pointer로 Converting후 IUnknown의 Pointer로 변환하여 전달한다.
public:
	virtual IUnknown *	GetAddInCustomInterface( void) = 0;

// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
public:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData) = 0;
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData) = 0;	// (2005/9/2 - Seung-Won, Bae) for String Data

// (2006/2/12 - Seung-Won, Bae) Register AddIn Event Map
public:
	// (2006/2/12 - Seung-Won, Bae) Define Using Enum Type of AddIn Event in this AddIn Interface Version
	//	********************************************************************************
	//	* Caution! It can be sorted.
	//	********************************************************************************
	typedef enum
	{
		ADDINEVENT( OnAddInToolCommand_EAICommandType),							// OnAddInToolCommand(	const EAICommandType p_eCommandID)																
		ADDINEVENT( OnAddInToolCommand_EAICommandType_CBlockBehaviorData),		// OnAddInToolCommand(	const EAICommandType p_eCommandID, const CBlockBehaviorData *p_pParam)								
		ADDINEVENT( OnAddInToolCommand_EAICommandType_HDC_UINT_POINT_DWORD),	// OnAddInToolCommand(	const EAICommandType p_eCommandID, HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
		ADDINEVENT( OnAddInToolCommand_EAICommandType_char),					// OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam)																		
		ADDINEVENT( OnAddInToolCommand_EAICommandType_char_long),				// OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)						
		ADDINEVENT( OnAddInToolCommand_EAICommandType_IAddInDllBase),			// OnAddInToolCommand(	const EAICommandType p_eCommandID, const IAddInDllBase *p_pParam)						
		ADDINEVENT( OnAddInToolCommand_EAICommandType_long),					// OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam)					
		ADDINEVENT( OnAddInToolCommand_EAICommandType_long_char),				// OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam)						
		ADDINEVENT( OnAddInToolCommand_EAICommandType_long_char_char_char),		// OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3)									
		ADDINEVENT( OnAddInToolCommand_EAICommandType_long_long),				// OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
		ADDINEVENT( OnAddInToolCommand_EAICommandType_long_long_long_long_long_long_long_long_long),	// OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2, const long p_lParam3, const long p_lParam4, const long p_lParam5, const long p_lParam6, const long p_lParam7, const long p_lParam8, const long p_lParam9)
		ADDINEVENT( OnAddInToolCommand_EAICommandType_HWND_POINT),				// OnAddInToolCommand(	const EAICommandType p_eCommandID, HWND p_hWnd, POINT &point)
		ADDINEVENT( OnAddInToolCommand_long_EAICommandType),					// OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID)														
		ADDINEVENT( OnAddInToolCommand_long_EAICommandType_IGraphInfo),			// OnAddInToolCommand( long &p_lResult, const EAICommandType p_eCommandID, IGraphInfo *p_pIGraphInfo)
		ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char),				// OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam)															
		ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char_CObject),		// OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam, CObject *p_pParam)																		
		ADDINEVENT( OnAddInToolCommand_long_EAICommandType_long),				// OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam)															
		ADDINEVENT( OnAddInToolCommand_long_EAICommandType_long_long),			// OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)																	
		ADDINEVENT( OnCaptureChanged),			// OnCaptureChanged(	HWND p_hWnd)											
		ADDINEVENT( OnCmdMsg),					// OnCmdMsg(			UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)							
		ADDINEVENT( OnDrawAfterGraph),			// OnDrawAfterGraph( HDC p_hDC)																	
		ADDINEVENT( OnDrawBeforeGraph),			// OnDrawBeforeGraph( HDC p_hDC)																		
		ADDINEVENT( OnDrdsData),				// OnDrdsData( WPARAM p_wParam, LPARAM p_lParam)											
		ADDINEVENT( OnDrdsPacketEnd),			// OnDrdsPacketEnd( WPARAM p_wParam, LPARAM p_lParam)												
		ADDINEVENT( OnGSharedDataChange),		// OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare)																		
		ADDINEVENT( OnKeyDown),					// OnKeyDown(			UINT nChar, UINT nRepCnt, UINT nFlags)	
		ADDINEVENT( OnKeyUp),					// OnKeyUp(			UINT nChar, UINT nRepCnt, UINT nFlags)
		ADDINEVENT( OnLButtonDblClk),			// OnLButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)																		
		ADDINEVENT( OnLButtonDown),				// OnLButtonDown(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)				
		ADDINEVENT( OnLButtonUp),				// OnLButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)																
		ADDINEVENT( OnLSharedDataChange),		// OnLSharedDataChange( const char *p_szLShareKey, const char *p_szLShareValue)																		
		ADDINEVENT( OnMouseMove),				// OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)		
		ADDINEVENT( OnMouseWheel),				// OnMouseWheel(	UINT nFlags, short zDelta, POINT pt, const DWORD p_dwMouseActionID)			
		ADDINEVENT( OnPacketData),				// OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset)																
		ADDINEVENT( OnRButtonDblClk),			// OnRButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)																		
		ADDINEVENT( OnRButtonDown),				// OnRButtonDown(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)																		
		ADDINEVENT( OnRButtonUp),				// OnRButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)																
		ADDINEVENT( OnResetChart),				// OnResetChart( void)																
		ADDINEVENT( OnSharedDataChange),		// OnSharedDataChange( const char *p_szShareName, const char *p_szSharedData, BOOL p_bStartShare)																		
		ADDINEVENT( OnTimer),					// OnTimer( UINT p_uTimerID)										
		ADDINEVENT( OnUpdatePacketData),		// OnUpdatePacketData( BOOL p_bTrUpdate)													
		ADDINEVENT( OnLoadEnvironment),			// OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap)
		ADDINEVENT( OnSaveEnvironment),			// OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap)
		ADDINEVENT( DrawCursor),				// DrawCursor( HDC p_hDC)
		ADDINEVENT( OnRealData),				// OnRealData( const char *p_szPacketName, double p_dData)
		// RQ별 Packet : 복수종목 - ojtaso (20070111)
		ADDINEVENT( OnPacketDataMultiItem),		// OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
		// 복수종목별 Real반영 : 복수종목 - ojtaso (20070131)
		ADDINEVENT( OnUpdatePacketDataMultiItem),	// OnUpdatePacketDataMultiItem(LPCTSTR lpszRQ, BOOL p_bTrUpdate)
		EAE_END,								// the count of Defined Type or Registered Event End (No more) Flag.
	} EAddInEventType;
public:
	virtual int		RegisterEventMap( const BOOL p_bOverExclusive) = 0;
	virtual int		UnregisterEventMap( void) = 0;

//////////////////////////////////////////////////////////////////////
// Event Routines
//////////////////////////////////////////////////////////////////////

public:
	// [04/10/11] 공유를 전달하는 Interface를 제공한다.
	// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare) = 0;
	// (2005/11/21 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnLSharedDataChange( const char *p_szLShareKey, const char *p_szLShareValue) = 0;
	// (2005/8/10 - Seung-Won, Bae) use in Only Hanwha
	virtual void	OnSharedDataChange( const char *p_szShareName, const char *p_szSharedData, BOOL p_bStartShare) = 0;

	// [04/10/11] AddIn DLL들에게 ResetChart 상황을 알려 처리되도록 한다.
	virtual void	OnResetChart( void) = 0;
	// [04/10/11] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawBeforeGraph( HDC p_hDC) = 0;
	// [04/12/25] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawAfterGraph( HDC p_hDC) = 0;
	// [04/10/13] TR의 수신을 AddIn DLL들에게 알려 대응토록 한다.
	virtual void	OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset) = 0;
	// [04/12/07] TR이나 Real의 수신에 의해 Packet Data가 Update되었음을 AddIn DLL들에 알려 지표등을 처리토록 한다.
	virtual void	OnUpdatePacketData( BOOL p_bTrUpdate) = 0;
	// (2007/2/8 - Seung-Won, Bae) On Real Data
	virtual void	OnRealData( const char *p_szPacketName, double p_dData)	= 0;
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset) = 0;
	// 복수종목별 Real반영 : 복수종목 - ojtaso (20070131)
	virtual void	OnUpdatePacketDataMultiItem(LPCTSTR lpszRQ, BOOL p_bTrUpdate) = 0;

// (2004.10.29, 배승원) AddIn DLL에게 그밖의 Windows Event를 전달한다.
public:
	// [04/10/29] Timer Event를 처리하는 Interface를 제공한다.
	virtual void	OnTimer( UINT p_uTimerID) = 0;
	// [04/10/29] Mouse Capture Release Event를 AddIn DLL에게 전달하도록 한다.
	//		주의! OnCaptureChanged()는 SetCapture()/RelaseCapture()에 의한 경우 외에도,
	//			Focus 나 Activate, ToolTip, TrackMenu등에 의해서도 수시로 (SetCapture없이)
	//			또한 자기 자신을 다시 SetCapture()하는 경우에도 호출됨을 유의한다.
	virtual void	OnCaptureChanged(	HWND p_hWnd) = 0;

// (2005/2/4 - Seung-Won, Bae) Real 관련 Event를 전달한다.
// (2005/8/16 - Seung-Won, Bae) Not used in Hanwha
public:
	// [04/10/14] Real의 수신을 전달한다.
	//				속도 관계로 DRDS의 Data를 그대로 Pass한다.
	virtual void	OnDrdsData( WPARAM p_wParam, LPARAM p_lParam) = 0;
	// [04/10/15] Real Packet의 수신이 끝났음을 AddIn DLL에게 알려 처리토록 한다.
	virtual void	OnDrdsPacketEnd( WPARAM p_wParam, LPARAM p_lParam) = 0;

public:
	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	virtual void	OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap)	= 0;
	virtual void	OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap)	= 0;
	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	virtual RECT	DrawCursor( HDC p_hDC)	= 0;

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
public:
	virtual BOOL	OnCmdMsg(			UINT nID, int nCode, ICmdUI *p_pICmdUI) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID)	= 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam) = 0;
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const CBlockBehaviorData *p_pParam) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const IAddInDllBase *p_pParam) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam) = 0;
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam) = 0;
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2, const long p_lParam3, const long p_lParam4, const long p_lParam5, const long p_lParam6, const long p_lParam7, const long p_lParam8, const long p_lParam9) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, HWND p_hWnd, POINT &point) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, IGraphInfo *p_pIGraphInfo) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam, CObject *p_pParam) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2) = 0;
public:
	virtual BOOL	OnKeyDown(			UINT nChar, UINT nRepCnt, UINT nFlags)	= 0;
	virtual BOOL	OnKeyUp(			UINT nChar, UINT nRepCnt, UINT nFlags)	= 0;
	virtual BOOL	OnMouseWheel(		UINT nFlags, short zDelta, POINT pt, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnLButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnLButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnLButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnRButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnRButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnRButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)	= 0;
};

#endif // !defined(AFX_ICHARTADDIN_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_)
