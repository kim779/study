// IWAddInDll050531.h: interface for the CIWAddInDll050531 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWADDINDLL050531_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_)
#define AFX_IWADDINDLL050531_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. 주의!	OCX에서 이용되는 AddIn측  표준 Interface를 각 Version별 Interface에 맞춰주는 Wrapper입니다.
//		표준 AddIn Interface가 변경될 때마다 함께 전 Version의 AddIn Interface Wrapper가 각 Version에 맞도록 수정되어야 합니다.
//		이때, 수정되는 기능을 전파하여 연계된 AddIn이 이용할 수 있도록 하게 되면, 관련 AddIn도 함께 버전처리 되어야 하고,
//			그렇지 않는 경우에는 Wrapper에서 해당 Version의 AddIn Interface에 연결시키지 않음으로써 독립적으로 버전처리 될 수 있도록 하는 것입니다.
//////////////////////////////////////////////////////////////////////

// (2004.10.07, 배승원) Addin DLL의 Interface Wrapper를 정의한다.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../IWUnknown.h"					// for CIWUnknown
#include "../IAddInDll.h"					// for IAddInDll
#include "./IChartAddIn050531.h"	// IChartAddIn050531

class CIWAddInDll050531 : public CIWUnknown, public IAddInDll
{

// [04/10/07]
public:
	CIWAddInDll050531( const char *p_szAddInModule, HMODULE p_hAddInDll);
protected:
	virtual ~CIWAddInDll050531();

// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()


// (2004.10.07, 배승원) Addin DLL의 Module Name과 Handle을 관리하여 DLL Interface를 호출할 수 있도록 한다.
protected:
	HMODULE		m_hAddInDll;
	CString		m_strModuleName;


// (2004.10.07, 배승원) AddIn Interface Wrapper의 Version Value를 관리한다.
typedef enum EIVERSION	{ IVERSION = _IWRAPPER_DECIMAL( 050531)	};

// (2004.10.07, 배승원) AddIn Interface Wrapper Selector Routine을 제공한다.
//		AddIn Manager의 LoadAddIn()에서 Loading된 DLL의 Addin Version에 맞는 Wrapper를 생성하는 Routine이다.
//		특히 Addin Manager는 항상 최종 Version의 Selector Routine을 호출하여 적절한 Wrapper Object를 구할 수 있도록 한다.
public:
	static IAddInDll *CreateAddInDLLIWrapper( const char *p_szAddInModule, HMODULE p_hAddInDll, int p_nAddInIVersion);


//[04/10/07] AddIn DLL을 초기화시키는 Interface를 제공한다.
// (2004.10.08, 배승원) 초기화시에 IAddInManager(ChartOcx Interface Gateway)를 전달하여
//		DLL의 AddIn Interface를 확보한다.
protected:
	IChartAddIn050531 *	m_pIChartAddIn;
public:
	virtual BOOL		InitAddInDll( IAddInManager *p_pIAddInManager);

// [04/10/11] AddIn DLL의 이름을 조회하는 Interface를 제공한다.
public:
	virtual const char *	GetAddInDllName( void)	{	return m_strModuleName;	}

// [04/10/11] AddIn Object의 Interface를 제공하는 Interface를 제공한다.
public:
	virtual IUnknown *		GetIChartAddIn( void);

// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
public:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData)
					{	return m_pIChartAddIn->InvokeAddIn( p_nCommandType, p_lData);	}
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData)	// (2005/9/2 - Seung-Won, Bae) for String Data
					{	return m_pIChartAddIn->InvokeAddInStr( p_szCommandName, p_szData);	}

public:
	// [04/10/11] 공유를 전달하는 Interface를 제공한다.
	// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare)
					{	m_pIChartAddIn->OnGSharedDataChange( p_szGShareKey, p_szGShareValue, p_bStartShare);	}
	virtual void	OnLSharedDataChange( const char *p_szLShareKey, const char *p_szLShareValue)
					{	m_pIChartAddIn->OnLSharedDataChange( p_szLShareKey, p_szLShareValue);	}
	// (2005/8/10 - Seung-Won, Bae) use in Only Hanwha
	virtual void	OnSharedDataChange( const char *p_szShareName, const char *p_szSharedData, BOOL p_bStartShare)
					{	m_pIChartAddIn->OnSharedDataChange( p_szShareName, p_szSharedData, p_bStartShare);	}

	// [04/10/11] 외부 ToolBar의 Message를 전달하여 AddIn DLL에서도 처리할 수 있도록 한다.
	virtual void	OnExtTool( WPARAM wParam, LPARAM lParam)
					{	m_pIChartAddIn->OnExtTool( wParam, lParam);	}
	// [04/10/11] AddIn DLL들에게 ResetChart 상황을 알려 처리되도록 한다.
	virtual void	OnResetChart( void)
					{	m_pIChartAddIn->OnResetChart();	}
	// [04/10/11] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawBeforeGraph( CDC *p_pDC)
					{	m_pIChartAddIn->OnDrawBeforeGraph( p_pDC);	}
	// [04/12/25] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawAfterGraph( CDC *p_pDC)
					{	m_pIChartAddIn->OnDrawAfterGraph( p_pDC);	}
	// [04/10/13] TR의 수신을 AddIn DLL들에게 알려 대응토록 한다.
	virtual void	OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset)
					{	m_pIChartAddIn->OnPacketData( p_szPacketNames, p_bWithReset);	}
	// [04/12/07] TR이나 Real의 수신에 의해 Packet Data가 Update되었음을 AddIn DLL들에 알려 지표등을 처리토록 한다.
	virtual void	OnUpdatePacketData( BOOL p_bTrUpdate)
					{	m_pIChartAddIn->OnUpdatePacketData( p_bTrUpdate);	}

// (2004.10.22, 배승원) AddIn DLL에게 Mouse Event를 전달한다.
//		Return Value가 FALSE인 경우에는 이후 처리 Routine을 진행하지 않는다.
public:
	// [04/10/11] Left Mouse Button Double Click Event를 처리하는 Interface를 제공한다.
	virtual BOOL	OnLButtonDblClk(	UINT nFlags, CPoint &point)
					{	return m_pIChartAddIn->OnLButtonDblClk(	nFlags, point);	}
	// [04/10/22] Mouse Moving Event를 처리하는 Interface를 제공한다.
	virtual BOOL	OnMouseMove(		UINT nFlags, CPoint &point)
					{	return m_pIChartAddIn->OnMouseMove(		nFlags, point);	}
	// [04/10/25] Left Mouse Button Down Event를 처리하는 Interface를 제공한다.
	virtual BOOL	OnLButtonDown(		UINT nFlags, CPoint &point)
					{	return m_pIChartAddIn->OnLButtonDown(	nFlags, point);	}
	// [04/10/25] Left Mouse Button Down Event를 처리하는 Interface를 제공한다.
	virtual BOOL	OnLButtonUp(		UINT nFlags, CPoint &point)
					{	return m_pIChartAddIn->OnLButtonUp(		nFlags, point);	}
	// [04/10/29] Mouse Capture Release Event를 AddIn DLL에게 전달하도록 한다.
	virtual void	OnCaptureChanged(	CWnd *pWnd)
					{	m_pIChartAddIn->OnCaptureChanged( pWnd);	}

// (2004.10.29, 배승원) AddIn DLL에게 그밖의 Windows Event를 전달한다.
public:
	// [04/10/29] Timer Event를 처리하는 Interface를 제공한다.
	virtual void	OnTimer( UINT p_uTimerID)
					{	m_pIChartAddIn->OnTimer( p_uTimerID);	}
	// [04/11/09] KeyBoard Down Event를 처리하는 Interface를 제공한다.
	//		Return Value가 FALSE인 경우에는 이후 처리 Routine을 진행하지 않는다.
	virtual BOOL	OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags)
					{	return m_pIChartAddIn->OnKeyDown( nChar, nRepCnt, nFlags);	}
	// [04/11/09] KeyBoard Up Event를 처리하는 Interface를 제공한다.
	//		Return Value가 FALSE인 경우에는 이후 처리 Routine을 진행하지 않는다.
	virtual BOOL	OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags)
					{	return m_pIChartAddIn->OnKeyUp( nChar, nRepCnt, nFlags);	}

// (2005/2/4 - Seung-Won, Bae) Real 관련 Event를 전달한다.
// (2005/8/16 - Seung-Won, Bae) Not used in Hanwha
public:
	// [04/10/14] Real의 수신을 전달한다.
	//				속도 관계로 DRDS의 Data를 그대로 Pass한다.
	//				또한 FALSE를 Return할 경우 다음 Module에게 (PacketData에게도) 더 이상 Real을 전달하지 않는다.
	virtual BOOL	OnDrdsData( WPARAM p_wParam, LPARAM p_lParam)
					{	return m_pIChartAddIn->OnDrdsData( p_wParam, p_lParam);	}
	// [04/10/15] Real Packet의 수신이 끝났음을 AddIn DLL에게 알려 처리토록 한다.
	virtual void	OnDrdsPacketEnd( WPARAM p_wParam, LPARAM p_lParam)
					{	m_pIChartAddIn->OnDrdsPacketEnd( p_wParam, p_lParam);	}
// (2005/8/16 - Seung-Won, Bae) used in Only Hanwha
public:
	// 1. Notify for Every Real Item
	virtual BOOL	OnRealData( WORD p_wRealRecordID, WORD p_wRealItemID, const char *p_szRealKeyCode, long p_lPacketAddress, double dData)
					{	return m_pIChartAddIn->OnRealData( p_wRealRecordID, p_wRealItemID, p_szRealKeyCode, p_lPacketAddress, dData);	}
	// 2. Notify for Every Real Record End
	virtual void	OnRealRedcordEnd( WORD p_wRealRecordID, const char *p_szRealKeyCode)
					{	m_pIChartAddIn->OnRealRedcordEnd( p_wRealRecordID, p_szRealKeyCode);	}
};

#endif // !defined(AFX_IWADDINDLL050531_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_)
