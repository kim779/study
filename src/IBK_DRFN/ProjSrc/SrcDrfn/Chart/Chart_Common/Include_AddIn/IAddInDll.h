// IAddInDll.h: interface for the IAddInDll class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IADDINDLL_H__BB3A45D2_5941_4D70_AAEB_1DF7AAC43A35__INCLUDED_)
#define AFX_IADDINDLL_H__BB3A45D2_5941_4D70_AAEB_1DF7AAC43A35__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. 주의!	AddIn 기능이 있는 OCX에서만 이용됩니다.
//		각 AddIn들을 OCX별 무관하게 이용할 수 있도록 지원하기 위한 표준 Interface로
//			Interface 순서 변경만 가능합니다.
//		단, 기능 추가(확장)과 삭제 및 Type 변경은 전 AddIn Version들의 IWAddInDllXXXXXX들이
//			연계되는 IChartAddInXXXXXX에 맞도록 조정하는 작업이 있은 다음에나 가능하다.
//		모두 OCX내에서만 이용되는 Interface로 수정시 연계된 AddIn이 함께 내려갈 필요는 없다.
// *. AddIn Version Info
//		AddIn OCX Site
//			동양 GZone
//////////////////////////////////////////////////////////////////////

// (2004.10.07, 배승원) Addin DLL의 Export되어야 하는 Interface를 정의한다.
//		1. Addin DLL이 구버전 이라 하더라도 이 Interface로 호출되기 때문에,
//			이를 계승받은 Interface Wrapper Class를 정의하여 구버전이 호환되도록 수정한다.
//		2. 따라서 이 Interface를 수정할 경우, Version 수치가 오늘 일자가 아니면,
//			새로운 Interface Wrapper를 등록하여 이용하고,
//			구버전의 Interface Wrapper들은 이에 대해 호환되도록 모두 수정해 줘야 한다.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

interface IAddInManager;
interface IAddInDll : public IUnknown  
{
protected:
	virtual ~IAddInDll()	{};

// [04/10/07] AddIn DLL을 초기화시키는 Interface를 제공한다.
// (2004.10.08, 배승원) 초기화시에 IAddInManager(ChartOcx Interface Gateway)를 전달하여
//		DLL의 AddIn Interface를 확보한다.
public:
	virtual BOOL	InitAddInDll( IAddInManager *p_pIAddInManager) = 0;

// [04/10/11] AddIn DLL의 이름을 조회하는 Interface를 제공한다.
public:
	virtual const char *	GetAddInDllName( void) = 0;

// [04/10/11] AddIn Object의 Interface를 제공하는 Interface를 제공한다.
public:
	virtual IUnknown *		GetIChartAddIn( void) = 0;

// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
public:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData) = 0;
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData) = 0;	// (2005/9/2 - Seung-Won, Bae) for String Data
	
// [04/10/14] Event를 전달하는 Interface를 제공한다.
public:
	// [04/10/11] Global 공유를 전달하는 Interface를 제공한다.
	// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare) = 0;
	// (2005/11/21 - Seung-Won, Bae) Notify Local Shared Data Changed to AddIn
	virtual void	OnLSharedDataChange( const char *p_szLShareKey, const char *p_szLShareValue) = 0;
	// (2005/8/10 - Seung-Won, Bae) use in Only Hanwha
	virtual void	OnSharedDataChange( const char *p_szShareName, const char *p_szSharedData, BOOL p_bStartShare = FALSE) = 0;
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
	// [04/10/25] Left Mouse Button Up Event를 처리하는 Interface를 제공한다.
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
	virtual BOOL	OnDrdsData( WPARAM p_wParam, LPARAM p_lParam)		= 0;
	// [04/10/15] Real Packet의 수신이 끝났음을 AddIn DLL에게 알려 처리토록 한다.
	virtual void	OnDrdsPacketEnd( WPARAM p_wParam, LPARAM p_lParam)	= 0;
// (2005/8/16 - Seung-Won, Bae) used in Only Hanwha
public:
	// 1. Notify for Every Real Item
	virtual BOOL	OnRealData( WORD p_wRealRecordID, WORD p_wRealItemID, const char *p_szRealKeyCode, long p_lPacketAddress, double dData) = 0;
	// 2. Notify for Every Real Record End
	virtual void	OnRealRedcordEnd( WORD p_wRealRecordID, const char *p_szRealKeyCode) = 0;
};

#endif // !defined(AFX_IADDINDLL_H__BB3A45D2_5941_4D70_AAEB_1DF7AAC43A35__INCLUDED_)
