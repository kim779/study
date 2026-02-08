#if !defined(AFX_HANARO_IRUNSTATEMANAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_)
#define AFX_HANARO_IRUNSTATEMANAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_

// #include "../../_Include/IRunStateMng.h"
#include "IBaseDefine.h"

//  int nID = 38;
// 	IRunStateManager* pMng = (IRunStateManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IRunStateManager* pMng = (IRunStateManager*)AfxGetPctrInterface(nID);

// 만든이  : 이준옥 2005.11.14(월)
// History : 2005.11.14 인터페이스
// PCTR에 실행상태알려주기 기능

// ex... 관심 데이터를 얻어올 수 있는 시점에 Call...
// #include "../../_Include/IRunStateMng.h"
// IRunStateManager* pMng = (IRunStateManager*)AfxGetPctrInterface(38);
// pMng->NotifyState("GWANSIM", 1, NULL);

#define  MaxRunState			5

enum
{
	RunState_End=0,
	RunState_Start,
	RunState_Run,
};

enum 
{
	RunModule_GwanLib=0,
	RunModule_AutoTrade,
};

#define		RunState_Gwansim		"GWANSIM"
#define     RunState_AUTOTRADE		"AUTOTRADEMONITOR"

DECLARE_INTERFACE(IRunStateManager)
{
	// szKey 가능목록
	// "MAIN"
	// "GWANSIM"
	// dwState  1 : Start
	//          0 : End
	// dwStateData 필요한 추가정보를 넘길경우.
	// Return : 해당 처리를 하면  TRUE, 아니면 FALSE
	STDMETHOD_(BOOL, NotifyState)(LPCSTR szKey, long dwState, long dwStateData=NULL) PURE;
};

#endif // AFX_HANARO_IRUNSTATEMANAGER_H_73649_BY_JUNOK_LEE_98B1A__INCLUDED_


