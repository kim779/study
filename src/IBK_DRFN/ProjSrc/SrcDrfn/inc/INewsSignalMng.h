#if !defined(AFX_INTERFACE_OF_NEWSSIGNALMNG_H__320E_1464_BY_JUNOK_LEE_8B1A__INCLUDED_)
#define AFX_INTERFACE_OF_NEWSSIGNALMNG_H__320E_1464_BY_JUNOK_LEE_8B1A__INCLUDED_

// 만든이  : 이준옥 2008.04.18금
// History : 2008.04.18금 인터페이스 정의

// #include "../../_Include/INewsSignal.h"
//#include "./ISignalMng.h"

//#include <AFXTEMPL.H>
#include "IBaseDefine.h"
#define UUID_INewsSignalManager			45

//	ISignalManager* pSigMng = (ISignalManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	ISignalManager* pSigMng = (ISignalManager*)AfxGetPctrInterface(UUID_INewsSignalManager);

// 만든이  : 이준옥 2006.01.02(월)
// History : 2006.01.02 인터페이스 정의
//

//typedef  int  (WINAPI* FpSetLibOpen)(CWnd*);
//typedef  void (WINAPI* FpSetLibClose)();
//typedef  void (WINAPI* FpGetSignalData)(CString strType, CMapStringToPtr &mapSignalSet) ;
//typedef  void (WINAPI* FpSetSignalData)(CString strType, CMapStringToPtr *pmapSignalSet) ;
//typedef  void (WINAPI* FpGetFilterData)(long& stFilterInfo) ;
//typedef  void (WINAPI* FpSetFilterData)(long* pstFilterInfo) ;
//typedef  int  (WINAPI* FpConvertRealData)(int nPacketName, LPSTR &sRealData) ;
//
//DECLARE_INTERFACE(ISignalManager)
//{
//	//# DRDS에서 Signal DLL을로드한 후 세팅하는 경우
//	STDMETHOD_(BOOL, SetLibHandle)(HMODULE) PURE;
//
//	//# Open / Close! => 반드시 Open 후 Colse!
//	STDMETHOD_(int, _DLL_SetLibOpen)(CWnd* pwndMain) PURE;
//	STDMETHOD_(void, _DLL_SetLibClose)() PURE;
//
//	//# 신호 설정 관련 함수!
//	STDMETHOD_(void, _DLL_GetSignalData)(CString strType, CMapStringToPtr &mapSignalSet) PURE;
//	STDMETHOD_(void, _DLL_SetSignalData)(CString strType, CMapStringToPtr *pmapSignalSet) PURE;
//
//	STDMETHOD_(void, _DLL_GetFilterData)(long& stFilterInfo) PURE;
//	STDMETHOD_(void, _DLL_SetFilterData)(long* pstFilterInfo) PURE;
//
//	//# DRDS 외부 함수
//	STDMETHOD_(int, ConvertRealData)(int nPacketName, LPSTR &sRealData) PURE;
//
//};

DECLARE_INTERFACE(INewsSignalManagerSite)
{
};

DECLARE_INTERFACE(INewsSignalManager)
{
//	STDMETHOD_(BOOL, IsNewsSignalLoaded)() PURE;

	// 현재 Advise된 카운트 리턴.
	//cType :	A -> 기본 타입.
	//			E -> 유진 소리마치 메인Type;
	STDMETHOD_(LONG, AdviseNewsSignal)(char cType, LONG dwTypeValue) PURE;

	// UnAdvise후 Advise되어있는 카운트 리턴.
	STDMETHOD_(BOOL, UnAdviseNewsSignal)(LONG dwAdviseKey) PURE;

	STDMETHOD_(int, GetAdviseCount)() PURE;

	// bAdvise 신호를 받을 것인지(1), 말것인지(0)
	STDMETHOD_(int, SetNewsSignal)(BOOL bAdvise, CWnd* pScreenWnd, CString strType, CString strKey) PURE;

	//SetSiganl대신 SetNewsSignal사용한다.
	//STDMETHOD_(int,  SetSiganl)(BOOL bAdvise, CWnd* pwndMain, CString strType, CString strKey) PURE;

	STDMETHOD_(void, GetSignalData)(CString strType, CMapStringToPtr &mapSignalSet) PURE;

	STDMETHOD_(void, GetFilterData)(long &stFilterInfo) PURE;
	
	STDMETHOD_(void, SetSignalData)(CString strType, CMapStringToPtr *pmapSignalSet) PURE;

	STDMETHOD_(void, SetFilterData)(long *pstFilterInfo) PURE;


	STDMETHOD_(int,  GetSiganlTable)(CString strType, CStringArray &saSignalTable ) PURE;

	STDMETHOD_(HWND, GetDummyHwnd)() PURE;
};

#define WM_GET_BROD				(WM_USER+605)

#endif //AFX_INTERFACE_OF_NEWSSIGNALMNG_H__320E_1464_BY_JUNOK_LEE_8B1A__INCLUDED_
