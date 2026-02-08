#if !defined(AFX_INTERFACE_FOR_SUPPORT_KSORI_LIBRARY_BY_JUNOK_LEE_B1A__INCLUDED_)
#define AFX_INTERFACE_FOR_SUPPORT_KSORI_LIBRARY_BY_JUNOK_LEE_B1A__INCLUDED_

// #include "../../_Include/IKSLibMng.h"
#include "IBaseDefine.h"

//#ifndef UUID_IKSLibManager
//#define UUID_IKSLibManager		50
//#endif

// 	IKSLibManager* pMng = (UUID_IKSLibManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, 17, 0);
//	IKSLibManager* pMng = (UUID_IKSLibManager*)AfxGetPctrInterface(nID);


DECLARE_INTERFACE(IKSLibManager)
{
	STDMETHOD_(BOOL, Lib_KSLink_SetLinkTagData)( LPCTSTR sTag, LPCTSTR sValue) PURE ;
	STDMETHOD_(BOOL, Lib_KSLink_SetFormLinkTagData)(CString sTag, CString sValue ) PURE;

	STDMETHOD_(CString, Lib_KSLink_GetFileHeader)() PURE;
	STDMETHOD_(CString, Lib_KSLink_GetLoginInfo)(CString strTag) PURE;
	STDMETHOD_(int,		Lib_KSLink_GetAccountCount)() PURE;
	STDMETHOD_(CString, Lib_KSLink_GetAccount)(int nIndex) PURE;
	//STDMETHOD_(BYTE,	Lib_CommRequestData)(HWND hFormWnd, REQUEST_COMMINFO* pRqCommInfo, BYTE* lpData, int nDataLen, 
	//				 BOOL bDownLoad=FALSE, int nProgressType=-1, BOOL bWaitCursor=TRUE) PURE;
	STDMETHOD_(BYTE,	Lib_CommRequestData)(HWND hFormWnd, LONG pRqCommInfo, BYTE* lpData, int nDataLen, 
					 BOOL bDownLoad=FALSE, int nProgressType=-1, BOOL bWaitCursor=TRUE) PURE;

	// 소리마치의 칼라정보 구하기.
	// 유진에서 사용함.
	STDMETHOD_(COLORREF, Lib_KSLink_GetColor)(short nIndex, BOOL bUseOrgIndex = FALSE ) PURE;
	STDMETHOD_(long, Lib_KSLink_GetEnvInfo)(int nType) PURE;

	// FX차트 주문 연동 - onlyojt (20091204)
	STDMETHOD_(BOOL, Lib_KSLink_GetOrderLineInfo)(CString& strAcctNo, CString& strItemCode, CObArray* pArrOpenPosition, CObArray* pArrPendingOrder, CWnd* pWnd) PURE;
	//<< 20100113 by kmlee 체결잔고 라이브러리 해제.
	STDMETHOD_(BOOL, Lib_KSLink_SetGridCheJanLibFree)(CWnd* pWnd) PURE;
	//>>>
};

//#include "../../Common/KHResource.h"
#endif //AFX_INTERFACE_FOR_SUPPORT_KSORI_LIBRARY_BY_JUNOK_LEE_B1A__INCLUDED_
