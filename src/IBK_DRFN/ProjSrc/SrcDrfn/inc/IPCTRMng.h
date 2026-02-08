#if !defined(AFX_PCTRMANAGER_OF_GDI_H__320E_14_BY_JUNOK_LEE_B1A__INCLUDED_)
#define AFX_PCTRMANAGER_OF_GDI_H__320E_14_BY_JUNOK_LEE_B1A__INCLUDED_

// #include "../../_Include/IPCTRMng.h"
#include "IBaseDefine.h"

//  int nID = 1;
// 	IPCTRManager* pPcTrMng = (IPCTRManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IPCTRManager* pMng = (IPCTRManager*)AfxGetPctrInterface(nID);


/*
	PCTrData는 문자열데이터만을 처리한다.
*/

//const UINT RMSG_GETPCTRDATA = ::RegisterWindowMessage("RMSG_GETPCTRDATA");
//const UINT RMSG_SETPCTRDATA = ::RegisterWindowMessage("RMSG_SETPCTRDATA");
//IBaseDefine.h에 정의되어있음.
// 
// 개념 : 각각의 데이터에 변경하지 안하을 고유 ID를 부여한다.
// IPCTRSite를 구현할 곳은 데이터를 받는 Client부분이며
// IPCTRManager를 구현할 곳은 데이터를 제공하는 Server부분이다.
//
// const UINT RMSG_PCTRGETDATA = ::RegisterWindowMessage("RMSG_PCTRGETDATA");
// 리턴값을 받아서 바로 처리해야 할 경우
//
// const UINT RMSG_PCTRSETDATA = ::RegisterWindowMessage("RMSG_PCTRSETDATA");
// 결과를 IPCTRSite에서 처리하고자 할 때 호출
//
// 만든이  : 이준옥 2005.11.02(수)
// History : 2005.11.02 인터페이스 정의
//

//	RMSG_GETPCTRDATA 
// WPARAM wp;
// LPARAM lp;
// wp : 'A':기본타입, 'E':서버TR같은 확장타
//	1 : 'A'일경우 IMainInfoManager에서 제공하는 데이터 제공.
//		 lp = int nID; 값.
//	2 : 'E'
//		 lp = STPCTR_GETDATAEX*
//		return값도 STPCTR_GETDATAEX*

//	RMSG_SETPCTRDATA 
// WPARAM wp;
// LPARAM lp;
// wp : 'A':기본타입, 'E':서버TR같은 확장타
//	1 : 'A'일경우 IMainInfoManager에서 제공하는 데이터 제공.
//		 lp = int nID; 값.
//	2 : 'E' 지원하지 않음.


// ID에 해당하는 데이타가 바뀌었을 경우 그 바뀐정도를 받고자 할 경우를 위해 지원
DECLARE_INTERFACE(IPCTRSite)
{
	STDMETHOD_(void, DataReceive)(long dwKey, long dwData)PURE;
};

DECLARE_INTERFACE(IPCTRManager)
{
	// ---------------------------------------------------------------------
	// I-PARAM : 
	// O-PARAM : 
	// ---------------------------------------------------------------------
	STDMETHOD_(LPCSTR, GetData)(int nID)PURE;

	// ---------------------------------------------------------------------
	// I-PARAM : 
	// O-PARAM : 
	// ---------------------------------------------------------------------
	STDMETHOD_(BOOL, SetData)(int nID, LPCSTR szNewData)PURE;

	// ---------------------------------------------------------------------
	// PCTR
	// I-PARAM : 
	// O-PARAM : 
	// ---------------------------------------------------------------------
	STDMETHOD_(COleVariant*, GetDataEx)(int nID)PURE;

	// ---------------------------------------------------------------------
	// I-PARAM : 
	// O-PARAM : 
	// ---------------------------------------------------------------------
	STDMETHOD_(BOOL, SetDataEx)(int nID, COleVariant* pNewData)PURE;

	// ---------------------------------------------------------------------
	// PCTR
	// I-PARAM : 
	// O-PARAM : 
	// ---------------------------------------------------------------------
	STDMETHOD_(COleVariant*, GetDataEx)(LPCSTR szKey)PURE;

	// ---------------------------------------------------------------------
	// I-PARAM : 
	// O-PARAM : 
	// ---------------------------------------------------------------------
	STDMETHOD_(BOOL, SetDataEx)(LPCSTR szKey, COleVariant* pNewData)PURE;

	// ---------------------------------------------------------------------
	// PCTR
	// I-PARAM : 
	// O-PARAM : 
	// ---------------------------------------------------------------------
	STDMETHOD_(LONG, GetTrData)(char szTR, int nCount, LPCSTR szData, long dwKeyPURE, LPCSTR szKey=NULL)PURE;

	// ---------------------------------------------------------------------
	// I-PARAM : nKey, IPCTRSite*
	// O-PARAM : Advise된 Key값. Key값은 Unadvise할 때 필요한 값임.
	// ---------------------------------------------------------------------
	STDMETHOD_(LONG, Advise)(int nKey, IPCTRSite* pSite)PURE;

	// ---------------------------------------------------------------------
	// I-PARAM : pKey : Advise시에 넘겨받은 키
	// O-PARAM : 
	// ---------------------------------------------------------------------
	STDMETHOD_(void, UnAdvise)(LONG pKey)PURE;

	// ---------------------------------------------------------------------
	// I-PARAM : nKey, IPCTRSite*
	// O-PARAM : Advise된 Key값. Key값은 Unadvise할 때 필요한 값임.
	// ---------------------------------------------------------------------
	STDMETHOD_(LONG, AdviseEx)(int nKey, IPCTRSite* pSite)PURE;

	// ---------------------------------------------------------------------
	// I-PARAM : pKey : Advise시에 넘겨받은 키
	// O-PARAM : 
	// ---------------------------------------------------------------------
	STDMETHOD_(void, UnAdviseEx)(LONG pKey)PURE;

};


#endif //AFX_PCTRMANAGER_OF_GDI_H__320E_14_BY_JUNOK_LEE_B1A__INCLUDED_
