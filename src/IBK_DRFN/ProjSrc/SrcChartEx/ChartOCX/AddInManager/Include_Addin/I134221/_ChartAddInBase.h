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

// (2006/2/12 - Seung-Won, Bae) Register AddIn Event Map
#define DECLARE_ADDIN_EVENT_MAP()																				\
protected:																										\
	static const EAddInEventType	m_eRegisteredEventMap[ EAE_END + 1];										\
protected:																										\
	virtual const EAddInEventType *	GetRegisteredEventMap( void) const	{	return m_eRegisteredEventMap;	}

// (2006/2/12 - Seung-Won, Bae) Define AddIn Event Map
#define BEGIN_ADDIN_EVENT_MAP( CChartAddInImp)												\
const IChartAddIn::EAddInEventType CChartAddInImp::m_eRegisteredEventMap[ EAE_END + 1] =	\
{
#define ADDINEVENT( EventType)		\
	EAE_##EventType
#define ONADDINEVENT( EventType)	\
	ADDINEVENT( EventType),
#define END_ADDIN_EVENT_MAP()	\
	EAE_END,					\
};

interface IAddInToolMgr;
interface IChartManager;
interface IChartOCX;
interface IGraphInfoManager;
interface IPacketListManager;
interface IPacketManager;
interface IXScaleManager;

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

// (2008/1/22 - Seung-Won, Bae) for Multi-Language
protected:
	HWND				m_hOcxWnd;

// (2009/9/14 - Seung-Won, Bae) for World-On
protected:
	CChartInfo::CHART_MODE	m_eChartMode;

// (2006/1/17 - Seung-Won, Bae) Manage IAddInDll for AddIn Tool Exclusive
protected:
	IAddInDllBase *	m_pIAddInDllBase;

// (2004.10.11, 배승원) OCX로부터 주요 Manager의 Interface를 구하여 관리한다.
protected:
	IPacketListManager *	m_pIPacketListManager;
	IPacketManager *		m_pIDefaultPacketManager;
	IChartManager *			m_pIChartManager;
	IGraphInfoManager *		m_pIGraphInfoManager;
	IAddInToolMgr *			m_pIAddInToolMgr;
	IXScaleManager *		m_pIXScaleManager;
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
public:
	virtual int						RegisterEventMap( const BOOL p_bOverExclusive);
	virtual int						UnregisterEventMap( void);

// (2007/3/4 - Seung-Won, Bae) Notify OCX Destroying to Addin for Reset OCX's Interface
protected:
	BOOL			m_bOcxDestroyed;
protected:
	virtual void	OnOcxDestroyWindow( void)							{	m_bOcxDestroyed = TRUE;	}

//////////////////////////////////////////////////////////////////////
// Event Routines
//////////////////////////////////////////////////////////////////////

// (2004.10.11, 배승원) AddIn DLL에 OCX Event를 전달한다.
protected:
	// [04/10/11] 공유를 전달하는 Interface를 제공한다.
	// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare)	{}
	// (2005/11/21 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnLSharedDataChange( const char *p_szLShareKey, const char *p_szLShareValue)	{}
	// [04/10/11] AddIn DLL들에게 ResetChart 상황을 알려 처리되도록 한다.
	virtual void	OnResetChart( void)	{}
	// [04/10/11] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawBeforeGraph( HDC p_hDC)	{}
	// [04/12/25] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawAfterGraph( HDC p_hDC)	{}
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)	{}
	// [04/10/29] Timer Event를 처리하는 Interface를 제공한다.
	virtual void	OnTimer( UINT p_uTimerID)	{}
	// [04/10/29] Mouse Capture Release Event를 AddIn DLL에게 전달하도록 한다.
	//		주의! OnCaptureChanged()는 SetCapture()/RelaseCapture()에 의한 경우 외에도,
	//			Focus 나 Activate, ToolTip, TrackMenu등에 의해서도 수시로 (SetCapture없이)
	//			또한 자기 자신을 다시 SetCapture()하는 경우에도 호출됨을 유의한다.
	virtual void	OnCaptureChanged( HWND p_hWnd, const HBITMAP p_hbmpMainOSB)	{}	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
	// (2006/9/25 - Seung-Won, Bae) On Reset MainBlock
	virtual void	OnResetMainBlock( void)								{}
	// (2006/9/25 - Seung-Won, Bae) On User IndicatorInfo Loaded
	virtual void	OnUserIndiatorConditionLoaded( void)				{}
	// (2006/9/25 - Seung-Won, Bae) On ChartItem Changed.
	virtual void	OnChartItemChanged( void)							{}
	// (2006/9/25 - Seung-Won, Bae) On GraphData Changed.
	virtual void	OnGraphDataChanged( void)							{}
	// (2006/9/25 - Seung-Won, Bae) On Block Removed.
	virtual void	OnBlockRemoved( const int p_nRow, const int p_nColumn)	{}
	// (2006/9/25 - Seung-Won, Bae) On Graph Added.
	virtual void	OnGraphAdded( const char *p_szGraphName, const char *p_szIndicatorName)			{}
	// (2006/9/25 - Seung-Won, Bae) On Graph Added.
	virtual void	OnManualRealData( const char *p_szPacketName, const double p_dData, const int p_nOption)	{}
	// (2006/9/27 - Seung-Won, Bae) On Context Menu.
	virtual void	OnContextMenu( HWND p_hWnd, POINT &p_pt)			{}
	// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
	virtual void	OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName)			{}
	// (2006/9/27 - Seung-Won, Bae) On Packet Truncated.
	virtual void	OnPacketTruncated( const int p_nDataCount)			{}
	// (2006/9/27 - Seung-Won, Bae) On Block Changed.
	virtual void	OnBlockChanged( const CBlockBaseData::BLOCKBEHAVIOR p_eChangeType, const int p_nSourceRow, const int p_nSourceColumn, const int p_nTargetRow, const int p_nTargetColumn)	{}
	// (2006/11/24 - Seung-Won, Bae) On Tool Command
	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption)	{}
	// (2007/2/8 - Seung-Won, Bae) On Real Data
	virtual void	OnRealData( const char *p_szPacketName, double p_dData, const char *p_szRQ)			{}
	virtual void	OnIndexedRealData( const char *p_szPacketName, const int p_nIndex, double p_dData)		{}
	// (2006/10/4 - Seung-Won, Bae) On Real Packet End
	virtual void	OnRealPacketEnd( const char *p_szRQ)								{}
	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	virtual void	OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap)	{}
	virtual void	OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap)	{}
	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	virtual RECT	DrawCursor( HDC p_hDC)								{	RECT rct = { 0, 0, 0, 0};	return rct;		}
	// (2007/1/18 - Seung-Won, Bae) Notify Recalculation.
	virtual void	OnRecalculationComplete( void)						{}
	// (2008/10/22 - Seung-Won, Bae) for Real Filtering
	//	Do not return FALSE. if you do, the real data can not be delivered to other AddIn and OCX.
	//	You can change the Real Data Value. it will be set to chart.
	virtual BOOL	OnPreRealData( const char *p_szPacketName, double &p_dData, const char *p_szRQ = NULL)	{	return TRUE;	}
	virtual BOOL	OnPreIndexedRealData( const char *p_szPacketName, const int p_nIndex, double &p_dData)	{	return TRUE;	}
	virtual BOOL	OnPreRealPacketEnd( const char *p_szRQ = NULL)											{	return TRUE;	}

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnCmdMsg(			UINT nID, int nCode, ICmdUI *p_pICmdUI)																	{	return FALSE;	}
	virtual BOOL	OnAddInMsg(			const char *p_szAddInItemName, const long p_lParam)														{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID)																		{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)	{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam)												{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2)						{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)							{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const IAddInDllBase *p_pParam)										{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam)													{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam)							{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3)	{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)							{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2, const long p_lParam3, const long p_lParam4, const long p_lParam5, const long p_lParam6, const long p_lParam7, const long p_lParam8, const long p_lParam9)	{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID)														{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, IGraphInfo *p_pIGraphInfo)							{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam)								{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam, CObject *p_pParam)			{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam)								{	return FALSE;	}
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)			{	return FALSE;	}
protected:
	virtual BOOL	OnKeyDown(						UINT nChar, UINT nRepCnt, UINT nFlags)																	{	return FALSE;	}
	virtual BOOL	OnKeyUp(						UINT nChar, UINT nRepCnt, UINT nFlags)																	{	return FALSE;	}
	virtual BOOL	OnMouseWheel(					UINT nFlags, short zDelta, POINT pt, const DWORD p_dwMouseActionID)										{	return FALSE;	}
	virtual BOOL	OnMouseMove(					HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)		{	return FALSE;	}	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
	virtual BOOL	OnLButtonDown(					HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)									{	return FALSE;	}
	virtual BOOL	OnLButtonUp(					HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)									{	return FALSE;	}
	virtual BOOL	OnLButtonDblClk(				HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)									{	return FALSE;	}
	virtual BOOL	OnRButtonDown(					HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)									{	return FALSE;	}
	virtual BOOL	OnRButtonUp(					HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)									{	return FALSE;	}
	virtual BOOL	OnRButtonDblClk(				HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)									{	return FALSE;	}
};

#endif // !defined(AFX_CHARTADDINBASE_H__DDC8A6DF_434B_4790_898D_B98EDE523730__INCLUDED_)
