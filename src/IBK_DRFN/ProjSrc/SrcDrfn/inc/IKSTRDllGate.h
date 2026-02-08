#ifndef DRFN_DLLGATE__FOR_KOREASORIMARCH_7A16A6B2_C152_4b82_B0EA_0AB66003142C__HEADER__
#define DRFN_DLLGATE__FOR_KOREASORIMARCH_7A16A6B2_C152_4b82_B0EA_0AB66003142C__HEADER__

// 
// #include "../../inc/IKSTRDllGate.h"

#define UUID_IKSTRDllGate					0

const UINT RMSG_GATEINTERFACE = ::RegisterWindowMessage(_T("RMSG_GATEINTERFACE"));

DECLARE_INTERFACE(IKSTRDllGateSite)
{
	// 종목코드가 변경되었다는 것을 알린다.
	STDMETHOD_(void, OncmbJCodeSelChanged)(LPCSTR strMarket, LPCSTR strCode, LPCSTR strName) PURE;

	// 종목코드가 Drag&Drop 되었을 때 불린다.
	STDMETHOD_(void, SetCodeDragAndDrop)(CString strCode) PURE;

	// 부모 윈도우의 핸들이 변경되었음을 알린다.
	STDMETHOD_(long ,OnChangeChildFrameWindowHandle)( WPARAM wParam, LPARAM lParam) PURE;
	
	STDMETHOD_(void ,OnEuBtnClk)( UINT nIdIndex) PURE;
};

DECLARE_INTERFACE(IKSTRDllGate)
{
	STDMETHOD_(HWND, GetGateHWnd)() PURE;
	STDMETHOD_(void, RegisterSite)(IKSTRDllGateSite* pSite) PURE;
	STDMETHOD_(void, SetDllMain)(CWnd* pWnd) PURE;

	//-------------------------------------------------------------------
	STDMETHOD_(IDispatch*, GetControlDispatch)(LPCSTR szCtrlName) PURE;
	STDMETHOD_(COleDispatchDriver*, GetDrfnControl)(LPCSTR szCtrlName) PURE;
	STDMETHOD_(void, OpenScreen)(CString strScreen, CStringList *p_strData=NULL) PURE;
	STDMETHOD_(long, GetChildFrame)() PURE;
	STDMETHOD_(void, SetFrameMsgText)(LPCSTR szMsgCode, LPCSTR szMsg) PURE;
	STDMETHOD_(HWND, GetControlHwnd)(LPCSTR szCtrlName) PURE;
};



#endif // DRFN_DLLGATE__FOR_KOREASORIMARCH_7A16A6B2_C152_4b82_B0EA_0AB66003142C__HEADER__

