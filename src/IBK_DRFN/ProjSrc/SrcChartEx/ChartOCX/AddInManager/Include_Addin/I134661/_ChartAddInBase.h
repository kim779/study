// ChartAddInBase.h: interface for the CChartAddInBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTADDINBASE_H__DDC8A6DF_434B_4790_898D_B98EDE523730__INCLUDED_)
#define AFX_CHARTADDINBASE_H__DDC8A6DF_434B_4790_898D_B98EDE523730__INCLUDED_

// (2004.10.08, 배승원) Chart AddIn Object의 Base Class를 정의한다.
// (2004.10.18, 배승원) Chart AddIn Object Base Class도 Chart Module과의 Interface를 정의하는 부분이므로,
//		Chart쪽에서 AddIn Module로 관리하면서 유지보수한다.	따라서, Version 번호를 결정하여 관리한다.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../_IWUnknown.h"			// for CIWUnknown
#include "_IChartAddIn.h"			// for IChartAddIn
#include "_IChartOCX.h"				// for IChartOCX

class CChartAddInBase : public CIWUnknown, public IChartAddIn
{
protected:
	CChartAddInBase( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CChartAddInBase();

// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()

// (2004.10.08, 배승원) Chart OCX Interface를 관리하여, 이후 타 Object도 조회할 수 있도록 한다.
protected:
	IChartOCX *			m_pIChartOCX;

// (2004.10.25, 배승원) Chart OCX Control Window Pointer를 관리하여 Windows Interface를 이용할 수 있도록 한다.
protected:
	CWnd *				m_pChartCWnd;

// (2006/1/17 - Seung-Won, Bae) Manage IAddInDll for AddIn Tool Exclusive
protected:
	IAddInDllBase *	m_pIAddInDllBase;

// (2004.10.11, 배승원) OCX로부터 주요 Manager의 Interface를 구하여 관리한다.
protected:
	IPacketManager *	m_pIPacketManager;
	IChartManager *		m_pIChartManager;
	IGraphInfoManager *	m_pIGraphInfoManager;
	IAddInToolMgr *		m_pIAddInToolMgr;
// (2004.10.13, 배승원) 기본 Manager가 등록되어 있는지 확인하는 Interface를 제공한다.
protected:
	virtual BOOL		HasFullManager( void);

// [05/07/08] AddIn 전용 Interface를 제공한다.
//		*. 주의. 반듯이 전용 Interface Pointer로 Converting후 IUnknown의 Pointer로 변환하여 전달한다.
protected:
	virtual IUnknown *	GetAddInCustomInterface( void)				{	return NULL;	}

// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
protected:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData)	{	return FALSE;	}
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData)	{	return FALSE;	}	// (2005/9/2 - Seung-Won, Bae) for String Data

// (2006/2/12 - Seung-Won, Bae) Register AddIn Event Map
protected:
	BOOL								m_bOverExclusive;
protected:
	virtual const EAddInEventType *		GetRegisteredEventMap( void) const = 0;
	virtual IAddInDllBase::EAddInEventType	GetGlobalAddInEventType( const long p_eAddInEventType) const	{	return IAddInDllBase::EAddInEventType::EAE_NOT_DEFINED;	}
public:
	virtual int						RegisterEventMap( const BOOL p_bOverExclusive);
	virtual int						UnregisterEventMap( void);

//////////////////////////////////////////////////////////////////////
// Event Routines
//////////////////////////////////////////////////////////////////////

// (2004.10.11, 배승원) 기본적인 Interface를 Default로 제공한다. (Derived Class에서 재정의하지 않아도 오류가 나지 않도록)
protected:
	// [04/10/11] 공유를 전달하는 Interface를 제공한다.
	// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare)	{}
	// (2005/11/21 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnLSharedDataChange( const char *p_szLShareKey, const char *p_szLShareValue)	{}
	// (2005/8/10 - Seung-Won, Bae) use in Only Hanwha
	virtual void	OnSharedDataChange( const char *p_szShareName, const char *p_szSharedData, BOOL p_bStartShare)	{}

	// [04/10/11] AddIn DLL들에게 ResetChart 상황을 알려 처리되도록 한다.
	virtual void	OnResetChart( void)	{}
	// [04/10/11] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawBeforeGraph( CDC *p_pDC)	{}
	// [04/12/25] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawAfterGraph( CDC *p_pDC)	{}
	// [04/10/13] TR의 수신을 AddIn DLL들에게 알려 대응토록 한다.
	virtual void	OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset)	{}
	// [04/12/07] TR이나 Real의 수신에 의해 Packet Data가 Update되었음을 AddIn DLL들에 알려 지표등을 처리토록 한다.
	virtual void	OnUpdatePacketData( BOOL p_bTrUpdate)	{}

// (2004.10.29, 배승원) AddIn DLL에게 그밖의 Windows Event를 전달한다.
protected:
	// [04/10/29] Timer Event를 처리하는 Interface를 제공한다.
	virtual void	OnTimer( UINT p_uTimerID)	{}
	// [04/10/29] Mouse Capture Release Event를 AddIn DLL에게 전달하도록 한다.
	//		주의! OnCaptureChanged()는 SetCapture()/RelaseCapture()에 의한 경우 외에도,
	//			Focus 나 Activate, ToolTip, TrackMenu등에 의해서도 수시로 (SetCapture없이)
	//			또한 자기 자신을 다시 SetCapture()하는 경우에도 호출됨을 유의한다.
	virtual void	OnCaptureChanged(	CWnd *pWnd)	{}

// (2005/2/4 - Seung-Won, Bae) Real 관련 Event를 전달한다.
// (2005/8/16 - Seung-Won, Bae) Not used in Hanwha
protected:
	// [04/10/14] Real의 수신을 전달한다.
	//				속도 관계로 DRDS의 Data를 그대로 Pass한다.
	virtual void	OnDrdsData( WPARAM p_wParam, LPARAM p_lParam)		{}
	// [04/10/15] Real Packet의 수신이 끝났음을 AddIn DLL에게 알려 처리토록 한다.
	virtual void	OnDrdsPacketEnd( WPARAM p_wParam, LPARAM p_lParam)	{}
// (2005/8/16 - Seung-Won, Bae) used in Only Hanwha
protected:
	// 1. Notify for Every Real Item
	virtual void	OnRealData( WORD p_wRealRecordID, WORD p_wRealItemID, const char *p_szRealKeyCode, long p_lPacketAddress, double dData)	{}
	// 2. Notify for Every Real Record End
	virtual void	OnRealRedcordEnd( WORD p_wRealRecordID, const char *p_szRealKeyCode)													{}

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnCmdMsg(			UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)									{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID)																		{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)	{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam)												{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)							{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const CBlockBehaviorData *p_pParam)									{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const CStringList *p_pParam)											{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const CStyleData *p_pParam)											{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const IAddInDllBase *p_pParam)										{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam)													{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam)							{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3)	{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)							{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2, const long p_lParam3, const long p_lParam4, const long p_lParam5, const long p_lParam6, const long p_lParam7, const long p_lParam8, const long p_lParam9)	{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, CWnd *p_pWnd, CPoint &point)											{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID)														{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam)								{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam, CObject *p_pParam)			{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam)								{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)			{	return FALSE;	}
protected:
	virtual BOOL	OnKeyDown(			UINT nChar, UINT nRepCnt, UINT nFlags)																	{	return FALSE;	}
	virtual BOOL	OnKeyUp(			UINT nChar, UINT nRepCnt, UINT nFlags)																	{	return FALSE;	}
	virtual BOOL	OnMouseWheel(		UINT nFlags, short zDelta, CPoint pt, const DWORD p_dwMouseActionID)									{	return FALSE;	}
	virtual BOOL	OnMouseMove(		CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)									{	return FALSE;	}
	virtual BOOL	OnLButtonDown(		CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)									{	return FALSE;	}
	virtual BOOL	OnLButtonUp(		CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)									{	return FALSE;	}
	virtual BOOL	OnLButtonDblClk(	CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)									{	return FALSE;	}
	virtual BOOL	OnRButtonDown(		CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)									{	return FALSE;	}
	virtual BOOL	OnRButtonUp(		CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)									{	return FALSE;	}
	virtual BOOL	OnRButtonDblClk(	CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)									{	return FALSE;	}
};

#endif // !defined(AFX_CHARTADDINBASE_H__DDC8A6DF_434B_4790_898D_B98EDE523730__INCLUDED_)
