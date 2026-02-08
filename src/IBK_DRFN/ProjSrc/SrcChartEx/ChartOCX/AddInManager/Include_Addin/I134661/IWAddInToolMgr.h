// IWAddInToolMgr.h: interface for the CIWAddInToolMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWADDINTOOLMGR_H__E1D8DADE_E2D2_4234_902E_A4931D02D9E3__INCLUDED_)
#define AFX_IWADDINTOOLMGR_H__E1D8DADE_E2D2_4234_902E_A4931D02D9E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../AddInManagerImp.h"		// for CAddInManagerImp
#include "..\_IWUnknown.h"				// for CIWUnknown
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

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
protected:
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const IAddInDllBase *p_pParam);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam);
};

#endif // !defined(AFX_IWADDINTOOLMGR_H__E1D8DADE_E2D2_4234_902E_A4931D02D9E3__INCLUDED_)
