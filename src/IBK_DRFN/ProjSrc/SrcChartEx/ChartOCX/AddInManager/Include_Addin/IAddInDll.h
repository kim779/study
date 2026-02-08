// IAddInDll.h: interface for the IAddInDll class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IADDINDLL_H__BB3A45D2_5941_4D70_AAEB_1DF7AAC43A35__INCLUDED_)
#define AFX_IADDINDLL_H__BB3A45D2_5941_4D70_AAEB_1DF7AAC43A35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Include_Chart/BlockBaseEnum.h"	// for CBlockBaseData
#include "_IAddInDllBase.h"						// for IAddInDllBase

enum EAICommandType;
class CAddInManagerImp;
class CIndicatorInfo;
interface IPropertyMap;
interface IAddInManager;
interface IAddInDll : public IAddInDllBase
{
protected:
	virtual ~IAddInDll()	{};

// [04/10/07] AddIn DLL을 초기화시키는 Interface를 제공한다.
// (2004.10.08, 배승원) 초기화시에 IAddInManager(ChartOcx Interface Gateway)를 전달하여
//		DLL의 AddIn Interface를 확보한다.
// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	virtual BOOL	InitAddInDll( const char *p_szAddInItem, IAddInManager *p_pIAddInManager, IAddInDll *p_pIAddInDll) = 0;

// [04/10/11] AddIn DLL의 이름을 조회하는 Interface를 제공한다.
// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	virtual const char *	GetAddInDllName( void) = 0;
	virtual const char *	GetAddInItemName( void) = 0;

// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
public:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData) = 0;
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData) = 0;	// (2005/9/2 - Seung-Won, Bae) for String Data

// [05/07/08] AddIn 전용 Interface를 제공한다.
//		*. 주의. 반듯이 전용 Interface Pointer로 Converting후 IUnknown의 Pointer로 변환하여 전달한다.
public:
	virtual IUnknown *	GetAddInCustomInterface( void) = 0;
	
// (2006/2/12 - Seung-Won, Bae) Register AddIn Event Map
public:
	virtual EAddInEventType						GetGlobalAddInEventType( const long p_eAddInEventType) = 0;
	virtual IAddInDllBase::EAddInEventType *	GetAdditionalRequestEventMap( void) = 0;
public:
	virtual int		RegisterEventMap( CAddInManagerImp *p_pAddInManager, const BOOL p_bOverExclusive) = 0;
	virtual int		UnregisterEventMap( CAddInManagerImp *p_pAddInManager) = 0;

// (2007/3/4 - Seung-Won, Bae) Notify OCX Destroying to Addin for Reset OCX's Interface
public:
	virtual void	OnOcxDestroyWindow( void) = 0;

// (2009/12/2 - Seung-Won, Bae) Notify to other modules.
public:
	virtual void	ChangeChartMode( CChartInfo::CHART_MODE p_eChartMode) = 0;

//////////////////////////////////////////////////////////////////////
// Event Routines
//////////////////////////////////////////////////////////////////////

// (2004.10.11, 배승원) AddIn DLL에 OCX Event를 전달한다.
public:
	// [04/10/11] Global 공유를 전달하는 Interface를 제공한다.
	// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare) = 0;
	// (2005/11/21 - Seung-Won, Bae) Notify Local Shared Data Changed to AddIn
	virtual void	OnLSharedDataChange( const char *p_szLShareKey, const char *p_szLShareValue) = 0;
	// [04/10/11] 외부 ToolBar의 Message를 전달하여 AddIn DLL에서도 처리할 수 있도록 한다.
	virtual void	OnResetChart( void) = 0;
	// [04/10/11] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawBeforeGraph( HDC p_hDC) = 0;
	// [04/12/25] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawAfterGraph( HDC p_hDC) = 0;
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset) = 0;
	// [04/10/29] Timer Event를 처리하는 Interface를 제공한다.
	virtual void	OnTimer( UINT p_uTimerID) = 0;
	// [04/10/29] Mouse Capture Release Event를 AddIn DLL에게 전달하도록 한다.
	//		주의! OnCaptureChanged()는 SetCapture()/RelaseCapture()에 의한 경우 외에도,
	//			Focus 나 Activate, ToolTip, TrackMenu등에 의해서도 수시로 (SetCapture없이)
	//			또한 자기 자신을 다시 SetCapture()하는 경우에도 호출됨을 유의한다.
	virtual void	OnCaptureChanged( CWnd *pWnd, const HBITMAP p_hbmpMainOSB)	= 0;	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
	// [04/10/14] Real의 수신을 전달한다.
	//				속도 관계로 DRDS의 Data를 그대로 Pass한다.
	virtual void	OnResetMainBlock( void)	= 0;
	// (2006/9/25 - Seung-Won, Bae) On User IndicatorInfo Loaded
	virtual void	OnUserIndiatorConditionLoaded( void) = 0;
	// (2006/9/25 - Seung-Won, Bae) On ChartItem Changed.
	virtual void	OnChartItemChanged( void) = 0;
	// (2006/9/25 - Seung-Won, Bae) On GraphData Changed.
	virtual void	OnGraphDataChanged( void) = 0;
	// (2006/9/25 - Seung-Won, Bae) On Block Removed.
	virtual void	OnBlockRemoved( const int p_nRow, const int p_nColumn) = 0;
	// (2006/9/25 - Seung-Won, Bae) On Graph Added.
	virtual void	OnGraphAdded( const char *p_szGraphName, const char *p_szIndicatorName) = 0;
	// (2006/9/25 - Seung-Won, Bae) On Graph Added.
	virtual void	OnManualRealData( const char *p_szPacketName, const double p_dData, const int p_nOption) = 0;
	// (2006/9/27 - Seung-Won, Bae) On Context Menu.
	virtual void	OnContextMenu( HWND p_hWnd, POINT &p_pt) = 0;
	// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
	virtual void	OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName) = 0;
	// (2006/9/27 - Seung-Won, Bae) On Packet Truncated.
	virtual void	OnPacketTruncated( const int p_nDataCount) = 0;
	// (2006/9/27 - Seung-Won, Bae) On Block Changed.
	virtual void	OnBlockChanged( const CBlockBaseData::BLOCKBEHAVIOR p_eChangeType, const int p_nSourceRow, const int p_nSourceColumn, const int p_nTargetRow, const int p_nTargetColumn) = 0;
	// (2006/11/24 - Seung-Won, Bae) On Tool Command
	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption) = 0;
	// (2007/2/8 - Seung-Won, Bae) On Real Data
	virtual void	OnRealData( const char *p_szPacketName, double p_dData, const char *p_szRQ) = 0;
	virtual void	OnIndexedRealData( const char *p_szPacketName, const int p_nIndex, double p_dData) = 0;
	// (2006/10/4 - Seung-Won, Bae) On Real Packet End
	virtual void	OnRealPacketEnd( const char *p_szRQ) = 0;
	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	virtual void	OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap) = 0;
	virtual void	OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap) = 0;
	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	virtual RECT	DrawCursor( HDC p_hDC) = 0;
	// (2007/1/18 - Seung-Won, Bae) Notify Recalculation.
	virtual void	OnRecalculationComplete( void) = 0;
	// (2008/10/22 - Seung-Won, Bae) for Real Filtering
	//	Do not return FALSE. if you do, the real data can not be delivered to other AddIn and OCX.
	//	You can change the Real Data Value. it will be set to chart.
	virtual BOOL	OnPreRealData( const char *p_szPacketName, double &p_dData, const char *p_szRQ = NULL, BOOL p_bRealData = TRUE) = 0;
	virtual BOOL	OnPreIndexedRealData( const char *p_szPacketName, const int p_nIndex, double &p_dData, BOOL p_bRealData = TRUE) = 0;
	virtual BOOL	OnPreRealPacketEnd( const char *p_szRQ = NULL, BOOL p_bRealData = TRUE) = 0;
	virtual BOOL	OnPreNMTRClock( __int64 &p_nRealClockSecs) = 0;

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
public:
	virtual BOOL	OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) = 0;
	virtual BOOL	OnAddInMsg( const char *p_szAddInItemName, const long p_lParam) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID)	= 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2) = 0;
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const IAddInDll *p_pParam) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam) = 0;
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam) = 0;
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2, const long p_lParam3, const long p_lParam4, const long p_lParam5, const long p_lParam6, const long p_lParam7, const long p_lParam8, const long p_lParam9) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, CIndicatorInfo *p_pParam) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, CObject *p_pParam) = 0;
	// 복수종목 지원을 위해 추가 : 복수종목 - ojtaso (20070125)
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3, CObject *p_pParam) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)	= 0;
public:
	virtual BOOL	OnKeyDown(						UINT nChar, UINT nRepCnt, UINT nFlags)	= 0;
	virtual BOOL	OnKeyUp(						UINT nChar, UINT nRepCnt, UINT nFlags)	= 0;
	virtual BOOL	OnMouseWheel(					UINT nFlags, short zDelta, POINT pt, const DWORD p_dwMouseActionID)		= 0;
	virtual BOOL	OnMouseMove(					HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)	= 0;	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
	virtual BOOL	OnLButtonDown(					HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnLButtonUp(					HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnLButtonDblClk(				HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnRButtonDown(					HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnRButtonUp(					HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)	= 0;
	virtual BOOL	OnRButtonDblClk(				HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)	= 0;
};

#endif // !defined(AFX_IADDINDLL_H__BB3A45D2_5941_4D70_AAEB_1DF7AAC43A35__INCLUDED_)
