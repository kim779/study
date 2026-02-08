// IAddInToolMgr.h: interface for the IAddInToolMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IADDINTOOLMGR_H__6E38A144_2F84_457F_A8F7_C884D9396662__INCLUDED_)
#define AFX_IADDINTOOLMGR_H__6E38A144_2F84_457F_A8F7_C884D9396662__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "_AddInVer.h"					// for AddIn Version Define
#include "_IChartAddIn.h"				// for IChartAddIn

enum EAICommandType;
interface IAddInDllBase;
interface IAddInToolMgr : public IUnknown
{
protected:
	virtual ~IAddInToolMgr()	{}

// (2006/2/12 - Seung-Won, Bae) Register/Unregister AddIn Event Map
public:
	virtual BOOL	RegisterEventMap( const IChartAddIn::EAddInEventType p_eAddInEventType, IAddInDllBase *p_pIAddInDllBase, const BOOL p_bOverExclusive) = 0;
	virtual BOOL	UnregisterEventMap( const IChartAddIn::EAddInEventType p_eAddInEventType, IAddInDllBase *p_pIAddInDllBase, const BOOL p_bOverExclusive) = 0;

// (2006/1/17 - Seung-Won, Bae) Keyboard & Mouse Exclusive Setting
public:
	virtual BOOL	GetKeyboardExclusive(		IAddInDllBase *p_pIAddInDllBase) = 0;
	virtual BOOL	IsKeyboardExclusive(		IAddInDllBase *p_pIAddInDllBase) = 0;
	virtual BOOL	ReleaseKeyboardExclusive(	IAddInDllBase *p_pIAddInDllBase) = 0;
public:
	virtual BOOL	GetAllMouseExclusive(		IAddInDllBase *p_pIAddInDllBase) = 0;	// return TRUE in one more Exclusive Created
	virtual BOOL	IsAllMouseExclusive(		IAddInDllBase *p_pIAddInDllBase) = 0;	// return TRUE in one more Exclusive Setted
	virtual BOOL	ReleaseAllMouseExclusive(	IAddInDllBase *p_pIAddInDllBase) = 0;	// return TRUE in one more Exclusive Released
public:
	virtual BOOL	GetLButtonExclusive(		IAddInDllBase *p_pIAddInDllBase) = 0;
	virtual BOOL	IsLButtonExclusive(			IAddInDllBase *p_pIAddInDllBase) = 0;
	virtual BOOL	ReleaseLButtonExclusive(	IAddInDllBase *p_pIAddInDllBase) = 0;
public:
	virtual BOOL	GetRButtonExclusive(		IAddInDllBase *p_pIAddInDllBase) = 0;
	virtual BOOL	IsRButtonExclusive(			IAddInDllBase *p_pIAddInDllBase) = 0;
	virtual BOOL	ReleaseRButtonExclusive(	IAddInDllBase *p_pIAddInDllBase) = 0;
public:
	virtual BOOL	GetMouseMoveExclusive(		IAddInDllBase *p_pIAddInDllBase) = 0;
	virtual BOOL	IsMouseMoveExclusive(		IAddInDllBase *p_pIAddInDllBase) = 0;
	virtual BOOL	ReleaseMouseMoveExclusive(	IAddInDllBase *p_pIAddInDllBase) = 0;
public:
	virtual BOOL	GetMouseWheelExclusive(		IAddInDllBase *p_pIAddInDllBase) = 0;
	virtual BOOL	IsMouseWheelExclusive(		IAddInDllBase *p_pIAddInDllBase) = 0;
	virtual BOOL	ReleaseMouseWheelExclusive(	IAddInDllBase *p_pIAddInDllBase) = 0;


// (2004.10.11, 배승원) AddIn DLL을 관리한다.
public:
	// (2006/1/7 - Seung-Won, Bae) Interface for Spacial Interface of AddIn DLL
	virtual IUnknown *	GetAddInCustomInterface( const char *p_szAddInModule) = 0;


// (2005/2/4 - Seung-Won, Bae) Real 관련 Event를 전달한다.
// (2005/8/16 - Seung-Won, Bae) Not used in Hanwha
public:
	// [04/10/14] Real의 수신을 전달한다.
	//				속도 관계로 DRDS의 Data를 그대로 Pass한다.
	virtual void	OnDrdsData( WPARAM p_wParam, LPARAM p_lParam)		= 0;
	// [04/10/15] Real Packet의 수신이 끝났음을 AddIn DLL에게 알려 처리토록 한다.
	virtual void	OnDrdsPacketEnd( WPARAM p_wParam, LPARAM p_lParam)	= 0;

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
public:
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const IAddInDllBase *p_pParam) = 0;
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam) = 0;
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3) = 0;
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID) = 0;
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam) = 0;
};

#endif // !defined(AFX_IADDINTOOLMGR_H__6E38A144_2F84_457F_A8F7_C884D9396662__INCLUDED_)
