#if !defined(AFX_INTERFACE_OF_TRCOMM_ACC8_9_BY_JUNOK_LEE_8B1A__INCLUDED_)
#define AFX_INTERFACE_OF_TRCOMM_ACC8_9_BY_JUNOK_LEE_8B1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// #include "../../_Include/ITrCommMng.h"
#include "IBaseDefine.h"

//  int nID = 2;
// 	IAUTrCommManager* pTrCommMng = (IAUTrCommManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);

// 개념 : 
// ITrCommSite를 구현할 곳은 데이터를 받는 Client부분이며
// ITrCommManager를 구현할 곳은 데이터를 제공하는 Server부분이다.
//
// 만든이  : 이준옥 2005.11.02(수)
// History : 2005.11.02 인터페이스 정의
//

DECLARE_INTERFACE(ITrCommSite)
{
	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD(RecieveData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, UINT dwTRDateLen) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
    STDMETHOD(StatusChange) (int nState) PURE;
};
typedef  ITrCommSite FAR * LPITrCommSite;

DECLARE_INTERFACE(IAUTrCommManager)
{
	// 메인의 소켓핸들을 세팅한다.
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(void, SetSocket)(HWND hSocket) PURE;

	// 메인이 디폴트로 생성한 소켓의 핸들.
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(HWND, GetSocketHandle)(LPCSTR szType="MAIN") PURE;

	// ---------------------------------------------------------------
	// 생성된 개별 TRComm 과 관련된 부분. start
	// ---------------------------------------------------------------
	// 기존의 CTRComm을 내부에서 처리해주고 있으며, 생성된 CTRComm과의 인터페이스
	// 개별 TrComm Create.
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD(Advise)(ITrCommSite* pSite=NULL) PURE;

	// Unadvise를 콜하면 생성된 개별 TrComm이 Destroy된다.
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD(Unadvise)() PURE;

	// 콜백함수를 바꾸려고 할 때 사용
	// I-PARAM : 새로운 ITrCommSite*
	// O-PARAM : 기존에 등록된 ITrCommSite*.
	STDMETHOD_(ITrCommSite*, ChangeCallback)(ITrCommSite* pSite) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(BOOL, Send2Server)(LPCSTR szData, int nLen, BOOL bWait=FALSE) PURE;

	// TR타입이 TYPE400이 아닐 경우도 가능.
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(BOOL, RealSend)(HWND hWnd, LPVOID pData, int nLen, DWORD dwType) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(void, SetTR)(LPCSTR szTR, BOOL bCert = FALSE) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(void, SetTimeout)(int nValue) PURE;

	//
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(void, SetEnc)(BOOL bValue) PURE;
	// ---------------------------------------------------------------
	// 생성된 개별 TRComm 과 관련된 부분. end
	// ---------------------------------------------------------------
};

#endif // AFX_INTERFACE_OF_TRCOMM_ACC8_9_BY_JUNOK_LEE_8B1A__INCLUDED_
