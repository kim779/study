// IWAddInDll.h: interface for the CIWAddInDll class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWADDINDLL_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_)
#define AFX_IWADDINDLL_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../_IWUnknown.h"		// for CIWUnknown
#include "../IAddInDll.h"		// for IAddInDll
#include "_IChartAddIn.h"		// for IChartAddIn

// (2004.10.07, 배승원) Addin DLL의 Interface Wrapper를 정의한다.
interface IChartCtrl;
class CIWAddInDll : public CIWUnknown, public IAddInDll
{
// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()
protected:
	CIWAddInDll( const char *p_szAddInModule, HMODULE p_hAddInDll, IChartCtrl *p_pIChartCtrl);
	~CIWAddInDll();
public:
	static IAddInDll *CreateObject( const char *p_szAddInModule, HMODULE p_hAddInDll, IChartCtrl *p_pIChartCtrl);

// (2004.10.07, 배승원) Addin DLL의 Module Name과 Handle을 관리하여 DLL Interface를 호출할 수 있도록 한다.
protected:
	HMODULE			m_hAddInDll;
	CString			m_strModuleName;
	CString			m_strItemName;
	IChartCtrl *	m_pIChartCtrl;

// (2004.10.07, 배승원) AddIn Interface Wrapper의 Version Value를 관리한다.
public:
	typedef enum EIVERSION	{ IVERSION = _IWRAPPER_DECIMAL( _AIVER)	};

//[04/10/07] AddIn DLL을 초기화시키는 Interface를 제공한다.
// (2004.10.08, 배승원) 초기화시에 IAddInManager(ChartOCX Interface Gateway)를 전달하여
//		DLL의 AddIn Interface를 확보한다.
// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
protected:
	IChartAddIn *	m_pIChartAddIn;
	IAddInManager *	m_pIAddInManager;
protected:
	virtual BOOL	InitAddInDll( const char *p_szAddInItem, IAddInManager *p_pIAddInManager, IAddInDll *p_pIAddInDll);

// [04/10/11] AddIn DLL의 이름을 조회하는 Interface를 제공한다.
protected:
	virtual const char *	GetAddInDllName( void)	{	return m_strModuleName;	}
	// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
	virtual const char *	GetAddInItemName( void)	{	return m_strItemName;	}

// [05/07/08] AddIn 전용 Interface를 제공한다.
//		*. 주의. 반듯이 전용 Interface Pointer로 Converting후 IUnknown의 Pointer로 변환하여 전달한다.
protected:
	virtual IUnknown *	GetAddInCustomInterface( void)
						{	return m_pIChartAddIn->GetAddInCustomInterface();	}

// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
protected:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData)
					{	return m_pIChartAddIn->InvokeAddIn( p_nCommandType, p_lData);	}
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData)	// (2005/9/2 - Seung-Won, Bae) for String Data
					{	return m_pIChartAddIn->InvokeAddInStr( p_szCommandName, p_szData);	}

// (2006/9/26 - Seung-Won, Bae) Additional Requesting Event Map
#define _IWADLL_ADDITIONAL_EVENT_COUNT	256
protected:
	BOOL										m_bOverExclusive;
	int											m_nAdditionalEventCount;
	IAddInDllBase::EAddInEventType				m_eaAdditionalEvent[ _IWADLL_ADDITIONAL_EVENT_COUNT];
protected:
	void										AddAdditionalRequestEvent( IAddInDllBase::EAddInEventType p_eAddInEventType);
	void										ResetAdditionalRequestEvent( void);
	virtual IAddInDllBase::EAddInEventType *	GetAdditionalRequestEventMap( void)	{	return m_eaAdditionalEvent;	}

// (2006/2/12 - Seung-Won, Bae) Register AddIn Event Map
protected:
	virtual int		RegisterEventMap( CAddInManagerImp *p_pAddInManager, const BOOL p_bOverExclusive);
	virtual int		UnregisterEventMap( CAddInManagerImp *p_pAddInManager);
protected:
	virtual IAddInDll::EAddInEventType	GetGlobalAddInEventType( const long p_eAddInEventType);

// (2007/3/4 - Seung-Won, Bae) Notify OCX Destroying to Addin for Reset OCX's Interface
protected:
	virtual void	OnOcxDestroyWindow( void)
					{	m_pIChartAddIn->OnOcxDestroyWindow();	}

// (2009/12/2 - Seung-Won, Bae) Notify to other modules.
protected:
	virtual void	ChangeChartMode( CChartInfo::CHART_MODE p_eChartMode)
					{	m_pIChartAddIn->ChangeChartMode( p_eChartMode);	}

//////////////////////////////////////////////////////////////////////
// Event Routines
//////////////////////////////////////////////////////////////////////

// (2004.10.11, 배승원) AddIn DLL에 OCX Event를 전달한다.
protected:
	// [04/10/11] 공유를 전달하는 Interface를 제공한다.
	// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare)
					{	m_pIChartAddIn->OnGSharedDataChange( p_szGShareKey, p_szGShareValue, p_bStartShare);	}
	virtual void	OnLSharedDataChange( const char *p_szLShareKey, const char *p_szLShareValue)
					{	m_pIChartAddIn->OnLSharedDataChange( p_szLShareKey, p_szLShareValue);	}
	// [04/10/11] AddIn DLL들에게 ResetChart 상황을 알려 처리되도록 한다.
	virtual void	OnResetChart( void)
					{	m_pIChartAddIn->OnResetChart();	}
	// [04/10/11] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawBeforeGraph( HDC p_hDC)
					{	m_pIChartAddIn->OnDrawBeforeGraph( p_hDC);	}
	// [04/12/25] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawAfterGraph( HDC p_hDC)
					{	m_pIChartAddIn->OnDrawAfterGraph( p_hDC);	}
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
					{	m_pIChartAddIn->OnPacketDataMultiItem(lpszRQ, p_szPacketNames, p_bWithReset);	}
	// [04/12/07] TR이나 Real의 수신에 의해 Packet Data가 Update되었음을 AddIn DLL들에 알려 지표등을 처리토록 한다.
	virtual void	OnUpdatePacketData( BOOL p_bTrUpdate = FALSE)
					{	m_pIChartAddIn->OnUpdatePacketData( p_bTrUpdate);	}
	// [04/10/29] Timer Event를 처리하는 Interface를 제공한다.
	virtual void	OnTimer( UINT p_uTimerID)
					{	m_pIChartAddIn->OnTimer( p_uTimerID);	}
	// [04/10/29] Mouse Capture Release Event를 AddIn DLL에게 전달하도록 한다.
	virtual void	OnCaptureChanged( CWnd *pWnd, const HBITMAP p_hbmpMainOSB)	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
					{	m_pIChartAddIn->OnCaptureChanged( pWnd ? pWnd->GetSafeHwnd() : NULL, p_hbmpMainOSB);	}
	// (2006/9/25 - Seung-Won, Bae) On Reset MainBlock
	virtual void	OnResetMainBlock( void)
					{	m_pIChartAddIn->OnResetMainBlock();	}
	// (2006/9/25 - Seung-Won, Bae) On User IndicatorInfo Loaded
	virtual void	OnUserIndiatorConditionLoaded( void)
					{	m_pIChartAddIn->OnUserIndiatorConditionLoaded();	}
	// (2006/9/25 - Seung-Won, Bae) On ChartItem Changed.
	virtual void	OnChartItemChanged( void)
					{	m_pIChartAddIn->OnChartItemChanged();	}
	// (2006/9/25 - Seung-Won, Bae) On GraphData Changed.
	virtual void	OnGraphDataChanged( void)
					{	m_pIChartAddIn->OnGraphDataChanged();	}
	// (2006/9/25 - Seung-Won, Bae) On Block Removed.
	virtual void	OnBlockRemoved( const int p_nRow, const int p_nColumn)
					{	m_pIChartAddIn->OnBlockRemoved( p_nRow, p_nColumn);	}
	// (2006/9/25 - Seung-Won, Bae) On Graph Added.
	virtual void	OnGraphAdded( const char *p_szGraphName, const char *p_szIndicatorName)
					{	m_pIChartAddIn->OnGraphAdded( p_szGraphName, p_szIndicatorName);	}
	// (2006/9/25 - Seung-Won, Bae) On Graph Added.
	virtual void	OnManualRealData( const char *p_szPacketName, const double p_dData, const int p_nOption)
					{	m_pIChartAddIn->OnManualRealData( p_szPacketName, p_dData, p_nOption);	}
	// (2006/9/27 - Seung-Won, Bae) On Context Menu.
	virtual void	OnContextMenu( HWND p_hWnd, POINT &p_pt)
					{	m_pIChartAddIn->OnContextMenu( p_hWnd, p_pt);	}
	// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
	virtual void	OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName)
					{	m_pIChartAddIn->OnGraphDeleted( p_szGraphName, p_szIndicatorName);	}
	// (2006/9/27 - Seung-Won, Bae) On Packet Truncated.
	virtual void	OnPacketTruncated( const int p_nDataCount)
					{	m_pIChartAddIn->OnPacketTruncated( p_nDataCount);	}
	// (2006/9/27 - Seung-Won, Bae) On Block Changed.
	virtual void	OnBlockChanged( const CBlockBaseData::BLOCKBEHAVIOR p_eChangeType, const int p_nSourceRow, const int p_nSourceColumn, const int p_nTargetRow, const int p_nTargetColumn)
					{	m_pIChartAddIn->OnBlockChanged( p_eChangeType, p_nSourceRow, p_nSourceColumn, p_nTargetRow, p_nTargetColumn);	}
	// (2006/11/24 - Seung-Won, Bae) On Tool Command
	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption)
					{	m_pIChartAddIn->OnToolCommand( p_nToolType, p_nCmdOption);	}
	// (2007/2/8 - Seung-Won, Bae) On Real Data
	virtual void	OnRealData( const char *p_szPacketName, double p_dData, const char *p_szRQ)
					{	m_pIChartAddIn->OnRealData( p_szPacketName, p_dData, p_szRQ);	}
	virtual void OnIndexedRealData( const char *p_szPacketName, const int p_nIndex, double p_dData)
					{	m_pIChartAddIn->OnIndexedRealData( p_szPacketName, p_nIndex, p_dData); }
	// (2006/10/4 - Seung-Won, Bae) On Real Packet End
	virtual void	OnRealPacketEnd( const char *p_szRQ)
					{	m_pIChartAddIn->OnRealPacketEnd( p_szRQ);	}
	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	virtual void	OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap)
					{	m_pIChartAddIn->OnLoadEnvironment( p_pIAddInEnvMap);	}
	virtual void	OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap)
					{	m_pIChartAddIn->OnSaveEnvironment( p_pIAddInEnvMap);	}
	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	virtual RECT	DrawCursor( HDC p_hDC)
					{	return m_pIChartAddIn->DrawCursor( p_hDC);	}
	// (2007/1/18 - Seung-Won, Bae) Notify Recalculation.
	virtual void	OnRecalculationComplete( void)
					{	m_pIChartAddIn->OnRecalculationComplete();	}
	// (2008/10/22 - Seung-Won, Bae) for Real Filtering
	//	Do not return FALSE. if you do, the real data can not be delivered to other AddIn and OCX.
	//	You can change the Real Data Value. it will be set to chart.
	virtual BOOL	OnPreRealData( const char *p_szPacketName, double &p_dData, const char *p_szRQ = NULL, BOOL p_bRealData = TRUE)
					{	return m_pIChartAddIn->OnPreRealData( p_szPacketName, p_dData, p_szRQ, p_bRealData);	}
	virtual BOOL	OnPreIndexedRealData( const char *p_szPacketName, const int p_nIndex, double &p_dData, BOOL p_bRealData = TRUE)
					{	return m_pIChartAddIn->OnPreIndexedRealData( p_szPacketName, p_nIndex, p_dData, p_bRealData);	}
	virtual BOOL	OnPreRealPacketEnd( const char *p_szRQ = NULL, BOOL p_bRealData = TRUE)
					{	return m_pIChartAddIn->OnPreRealPacketEnd( p_szRQ, p_bRealData);	}
	virtual BOOL	OnPreNMTRClock( __int64 &p_nRealClockSecs)
					{	return m_pIChartAddIn->OnPreNMTRClock( p_nRealClockSecs);	}

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnCmdMsg(			UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	virtual BOOL	OnAddInMsg( const char *p_szAddInItemName, const long p_lParam)
					{	return m_pIChartAddIn->OnAddInMsg( p_szAddInItemName, p_lParam);	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID)
					{	return m_pIChartAddIn->OnAddInToolCommand(	p_eCommandID);	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
					{	return m_pIChartAddIn->OnAddInToolCommand(	p_eCommandID, p_hDC, nFlags, point, p_dwMouseActionID);	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam)
					{	return m_pIChartAddIn->OnAddInToolCommand(	p_eCommandID, p_szParam);	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2)
					{	return m_pIChartAddIn->OnAddInToolCommand(	p_eCommandID, p_szParam1, p_szParam2);	}
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
					{	return m_pIChartAddIn->OnAddInToolCommand(	p_eCommandID, p_szParam, p_lParam);	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const IAddInDll *p_pParam)
					{	return m_pIChartAddIn->OnAddInToolCommand(	p_eCommandID, p_pParam);	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam)
					{	return m_pIChartAddIn->OnAddInToolCommand(	p_eCommandID, p_lParam);	}
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam)
					{	return m_pIChartAddIn->OnAddInToolCommand(	p_eCommandID, p_lParam, p_szParam);	}
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3)
					{	return m_pIChartAddIn->OnAddInToolCommand(	p_eCommandID, p_lParam, p_szParam1, p_szParam2, p_szParam3);	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
					{	return m_pIChartAddIn->OnAddInToolCommand(	p_eCommandID, p_lParam1, p_lParam2);	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2, const long p_lParam3, const long p_lParam4, const long p_lParam5, const long p_lParam6, const long p_lParam7, const long p_lParam8, const long p_lParam9)
					{	return m_pIChartAddIn->OnAddInToolCommand(	p_eCommandID, p_lParam1, p_lParam2, p_lParam3, p_lParam4, p_lParam5, p_lParam6, p_lParam7, p_lParam8, p_lParam9);	}
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID)
					{	return m_pIChartAddIn->OnAddInToolCommand(	p_lResult, p_eCommandID);	}
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, CIndicatorInfo *p_pParam);

	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam)
					{	return m_pIChartAddIn->OnAddInToolCommand(	p_lResult, p_eCommandID, p_szParam);	}
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, CObject *p_pParam)
					{	return m_pIChartAddIn->OnAddInToolCommand(	p_lResult, p_eCommandID, p_szParam1, p_pParam);	}
	// 복수종목 지원을 위해 추가 : 복수종목 - ojtaso (20070125)
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3, CObject *p_pParam)
					{	return TRUE;	}
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam)
					{	return m_pIChartAddIn->OnAddInToolCommand(	p_lResult, p_eCommandID, p_lParam);	}
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
					{	return m_pIChartAddIn->OnAddInToolCommand(	p_lResult, p_eCommandID, p_lParam1, p_lParam2);	}
protected:
	virtual BOOL	OnKeyDown(			UINT nChar, UINT nRepCnt, UINT nFlags)
					{	return m_pIChartAddIn->OnKeyDown(			nChar, nRepCnt, nFlags);	}
	virtual BOOL	OnKeyUp(			UINT nChar, UINT nRepCnt, UINT nFlags)
					{	return m_pIChartAddIn->OnKeyUp(				nChar, nRepCnt, nFlags);	}
	virtual BOOL	OnMouseWheel(		UINT nFlags, short zDelta, POINT pt, const DWORD p_dwMouseActionID)
					{	return m_pIChartAddIn->OnMouseWheel(		nFlags, zDelta, pt, p_dwMouseActionID);	}
	virtual BOOL	OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
					{	return m_pIChartAddIn->OnMouseMove(		p_hDC, nFlags, point, p_dwMouseActionID, p_hbmpMainOSB);	}
	virtual BOOL	OnLButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
					{	return m_pIChartAddIn->OnLButtonDown(		p_hDC, nFlags, point, p_dwMouseActionID);	}
	virtual BOOL	OnLButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
					{	return m_pIChartAddIn->OnLButtonUp(			p_hDC, nFlags, point, p_dwMouseActionID);	}
	virtual BOOL	OnLButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
					{	return m_pIChartAddIn->OnLButtonDblClk(		p_hDC, nFlags, point, p_dwMouseActionID);	}
	virtual BOOL	OnRButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
					{	return m_pIChartAddIn->OnRButtonDown(		p_hDC, nFlags, point, p_dwMouseActionID);	}
	virtual BOOL	OnRButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
					{	return m_pIChartAddIn->OnRButtonUp(			p_hDC, nFlags, point, p_dwMouseActionID);	}
	virtual BOOL	OnRButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
					{	return m_pIChartAddIn->OnRButtonDblClk(		p_hDC, nFlags, point, p_dwMouseActionID);	}
};

#endif // !defined(AFX_IWADDINDLL_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_)
