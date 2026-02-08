// IChartAddIn050531.h: interface for the IChartAddIn050531 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICHARTADDIN050531_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_)
#define AFX_ICHARTADDIN050531_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. 주의!	050531 Version의 OCX에서 이용되는 AddIn측 Interface로 기능 추가만 가능합니다.
//		기능 삭제와 Type 변경은 허용하지 않습니다.
//		Interface 순서 변경 조차 허용되지 않습니다.
//		허용되지 않는 편집은 050531 Version의 모든 AddIn Module과 관련 Site OCX가
//			함께 수정되어 내려가는 것이 전제되어야 합니다.
// *. 050531 Version Info
//		AddIn Module				Site			Manager
//		ChartCustomAddIn			한화			배승원
//		ChartAssistIndexAddIn		한화			오근문
//////////////////////////////////////////////////////////////////////

// (2004.10.08, 배승원) Chart AddIn Object의 Interface를 정의한다.
//		AddIn DLL 참조 Interface로 Macro Interface를 정의한다.
#if IChartAddInVer < 050531
	#define	IChartAddIn		IChartAddIn050531
	#undef	IChartAddInVer
	#define	IChartAddInVer	050531
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

interface IChartAddIn050531 : public IUnknown  
{
protected:
	virtual ~IChartAddIn050531()	{}

// (2004.10.13, 배승원) 기본 Manager가 등록되어 있는지 확인하는 Interface를 제공한다.
public:
	virtual BOOL	HasFullManager( void) = 0;

// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
public:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData) = 0;
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData) = 0;	// (2005/9/2 - Seung-Won, Bae) for String Data

// [05/07/08] AddIn 전용 Interface를 제공한다.
//		*. 주의. 반듯이 전용 Interface Pointer로 Converting후 IUnknown의 Pointer로 변환하여 전달한다.
public:
	virtual IUnknown *	GetAddInCustomInterface( void) = 0;

public:
	// [04/10/11] 공유를 전달하는 Interface를 제공한다.
	// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare) = 0;
	// (2005/11/21 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnLSharedDataChange( const char *p_szLShareKey, const char *p_szLShareValue) = 0;
	// (2005/8/10 - Seung-Won, Bae) use in Only Hanwha
	virtual void	OnSharedDataChange( const char *p_szShareName, const char *p_szSharedData, BOOL p_bStartShare) = 0;

	// [04/10/11] 외부 ToolBar의 Message를 전달하여 AddIn DLL에서도 처리할 수 있도록 한다.
	virtual void	OnExtTool( WPARAM wParam, LPARAM lParam) = 0;
	// [04/10/11] AddIn DLL들에게 ResetChart 상황을 알려 처리되도록 한다.
	virtual void	OnResetChart( void) = 0;
	// [04/10/11] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawBeforeGraph( CDC *p_pDC) = 0;
	// [04/12/25] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawAfterGraph( CDC *p_pDC) = 0;
	// [04/10/13] TR의 수신을 AddIn DLL들에게 알려 대응토록 한다.
	virtual void	OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset) = 0;
	// [04/12/07] TR이나 Real의 수신에 의해 Packet Data가 Update되었음을 AddIn DLL들에 알려 지표등을 처리토록 한다.
	virtual void	OnUpdatePacketData( BOOL p_bTrUpdate) = 0;

// (2004.10.22, 배승원) AddIn DLL에게 Mouse Event를 전달한다.
//		Return Value가 FALSE인 경우에는 이후 처리 Routine을 진행하지 않는다.
public:
	// [04/10/11] Left Mouse Button Double Click Event를 처리하는 Interface를 제공한다.
	virtual BOOL	OnLButtonDblClk(	UINT nFlags, CPoint &point) = 0;
	// [04/10/22] Mouse Moving Event를 처리하는 Interface를 제공한다.
	virtual BOOL	OnMouseMove(		UINT nFlags, CPoint &point) = 0;
	// [04/10/25] Left Mouse Button Down Event를 처리하는 Interface를 제공한다.
	virtual BOOL	OnLButtonDown(		UINT nFlags, CPoint &point) = 0;
	// [04/10/25] Left Mouse Button Down Event를 처리하는 Interface를 제공한다.
	virtual BOOL	OnLButtonUp(		UINT nFlags, CPoint &point) = 0;
	// [04/10/29] Mouse Capture Release Event를 AddIn DLL에게 전달하도록 한다.
	//		주의! OnCaptureChanged()는 SetCapture()/RelaseCapture()에 의한 경우 외에도,
	//			Focus 나 Activate, ToolTip, TrackMenu등에 의해서도 수시로 (SetCapture없이)
	//			또한 자기 자신을 다시 SetCapture()하는 경우에도 호출됨을 유의한다.
	virtual void	OnCaptureChanged(	CWnd *pWnd) = 0;

// (2004.10.29, 배승원) AddIn DLL에게 그밖의 Windows Event를 전달한다.
public:
	// [04/10/29] Timer Event를 처리하는 Interface를 제공한다.
	virtual void	OnTimer( UINT p_uTimerID) = 0;
	// [04/11/09] KeyBoard Down Event를 처리하는 Interface를 제공한다.
	//		Return Value가 FALSE인 경우에는 이후 처리 Routine을 진행하지 않는다.
	virtual BOOL	OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
	// [04/11/09] KeyBoard Up Event를 처리하는 Interface를 제공한다.
	//		Return Value가 FALSE인 경우에는 이후 처리 Routine을 진행하지 않는다.
	virtual BOOL	OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags) = 0;

// (2005/2/4 - Seung-Won, Bae) Real 관련 Event를 전달한다.
// (2005/8/16 - Seung-Won, Bae) Not used in Hanwha
public:
	// [04/10/14] Real의 수신을 전달한다.
	//				속도 관계로 DRDS의 Data를 그대로 Pass한다.
	//				또한 FALSE를 Return할 경우 다음 Module에게 (PacketData에게도) 더 이상 Real을 전달하지 않는다.
	virtual BOOL	OnDrdsData( WPARAM p_wParam, LPARAM p_lParam) = 0;
	// [04/10/15] Real Packet의 수신이 끝났음을 AddIn DLL에게 알려 처리토록 한다.
	virtual void	OnDrdsPacketEnd( WPARAM p_wParam, LPARAM p_lParam) = 0;
// (2005/8/16 - Seung-Won, Bae) used in Only Hanwha
public:
	// 1. Notify for Every Real Item
	virtual BOOL	OnRealData( WORD p_wRealRecordID, WORD p_wRealItemID, const char *p_szRealKeyCode, long p_lPacketAddress, double dData) = 0;
	// 2. Notify for Every Real Record End
	virtual void	OnRealRedcordEnd( WORD p_wRealRecordID, const char *p_szRealKeyCode) = 0;
};

#endif // !defined(AFX_ICHARTADDIN050531_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_)
