// IWAddInToolMgr.h: interface for the CIWAddInToolMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWADDINTOOLMGR_H__E1D8DADE_E2D2_4234_902E_A4931D02D9E3__INCLUDED_)
#define AFX_IWADDINTOOLMGR_H__E1D8DADE_E2D2_4234_902E_A4931D02D9E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../AddInManagerImp.h"		// for CAddInManagerImp
#include "../_IWUnknown.h"				// for CIWUnknown
#include "_IAddInToolMgr.h"				// for IAddInToolMgr
#include "_IChartOCX.h"					// for IChartOCX

class CIWAddInToolMgr : public CIWUnknown, public IAddInToolMgr
{
// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER( IAddInToolMgr, CIWAddInToolMgr, CAddInManagerImp, m_pAddInManager)

// (2006/2/12 - Seung-Won, Bae) Register/Unregister AddIn Event Map
public:
	virtual BOOL	RegisterEventMap( const IChartAddIn::EAddInEventType p_eAddInEventType, IAddInDllBase *p_pIAddInDllBase, const BOOL p_bOverExclusive);
	virtual BOOL	UnregisterEventMap( const IChartAddIn::EAddInEventType p_eAddInEventType, IAddInDllBase *p_pIAddInDllBase, const BOOL p_bOverExclusive);

// (2006/1/17 - Seung-Won, Bae) Keyboard & Mouse Exclusive Setting
protected:
	virtual BOOL	GetKeyboardExclusive(		IAddInDllBase *p_pIAddInDllBase);
	virtual BOOL	IsKeyboardExclusive(		IAddInDllBase *p_pIAddInDllBase);
	virtual BOOL	ReleaseKeyboardExclusive(	IAddInDllBase *p_pIAddInDllBase);
protected:
	virtual BOOL	GetAllMouseExclusive(		IAddInDllBase *p_pIAddInDllBase);	// return TRUE in one more Exclusive Created
	virtual BOOL	IsAllMouseExclusive(		IAddInDllBase *p_pIAddInDllBase);	// return TRUE in one more Exclusive Setted
	virtual BOOL	ReleaseAllMouseExclusive(	IAddInDllBase *p_pIAddInDllBase);	// return TRUE in one more Exclusive Released
protected:
	virtual BOOL	GetLButtonExclusive(		IAddInDllBase *p_pIAddInDllBase);
	virtual BOOL	IsLButtonExclusive(			IAddInDllBase *p_pIAddInDllBase);
	virtual BOOL	ReleaseLButtonExclusive(	IAddInDllBase *p_pIAddInDllBase);
protected:
	virtual BOOL	GetRButtonExclusive(		IAddInDllBase *p_pIAddInDllBase);
	virtual BOOL	IsRButtonExclusive(			IAddInDllBase *p_pIAddInDllBase);
	virtual BOOL	ReleaseRButtonExclusive(	IAddInDllBase *p_pIAddInDllBase);
protected:
	virtual BOOL	GetMouseMoveExclusive(		IAddInDllBase *p_pIAddInDllBase);
	virtual BOOL	IsMouseMoveExclusive(		IAddInDllBase *p_pIAddInDllBase);
	virtual BOOL	ReleaseMouseMoveExclusive(	IAddInDllBase *p_pIAddInDllBase);
protected:
	virtual BOOL	GetMouseWheelExclusive(		IAddInDllBase *p_pIAddInDllBase);
	virtual BOOL	IsMouseWheelExclusive(		IAddInDllBase *p_pIAddInDllBase);
	virtual BOOL	ReleaseMouseWheelExclusive(	IAddInDllBase *p_pIAddInDllBase);

// (2004.10.11, 배승원) AddIn DLL을 관리한다.
public:
	// (2006/1/7 - Seung-Won, Bae) Interface for Spacial Interface of AddIn DLL
	virtual IUnknown *	GetAddInCustomInterface( const char *p_szAddInModule);
	// (2006/8/25 - Seung-Won, Bae) Check AddIn Loaded (DLL:IDS)
	virtual BOOL	IsAddInLoaded( const char *p_szAddInItem);

// (2004.10.11, 배승원) AddIn DLL에 OCX Event를 전달한다.
protected:
	// (2006/10/4 - Seung-Won, Bae) On Real Data
	virtual void	OnRealData( const char *p_szPacketName, double p_dData, const char *p_szRQ = NULL);
	virtual void	OnIndexedRealData( const char *p_szPacketName, const int p_nIndex, double p_dData);
	// (2006/10/4 - Seung-Won, Bae) On Real Packet End
	virtual void	OnRealPacketEnd( const char *p_szRQ = NULL);
	// (2006/9/27 - Seung-Won, Bae) On Packet Truncated.
	virtual void	OnPacketTruncated( const int p_nDataCount);
	//>> 갭보정 - vntsorl(20090217)
	virtual void	OnPacketDataMultiItem( const char * p_szRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// (2008/10/22 - Seung-Won, Bae) for Real Filtering
	//	Do not return FALSE. if you do, the real data can not be delivered to other AddIn and OCX.
	//	You can change the Real Data Value. it will be set to chart.
	virtual BOOL	OnPreRealData( const char *p_szPacketName, double &p_dData, const char *p_szRQ = NULL);
	virtual BOOL	OnPreIndexedRealData( const char *p_szPacketName, const int p_nIndex, double &p_dData);
	virtual BOOL	OnPreRealPacketEnd( const char *p_szRQ = NULL);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
protected:
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual	BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam);
	virtual	BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const IAddInDllBase *p_pParam);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3);
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2);
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID);
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam);
};

#endif // !defined(AFX_IWADDINTOOLMGR_H__E1D8DADE_E2D2_4234_902E_A4931D02D9E3__INCLUDED_)
