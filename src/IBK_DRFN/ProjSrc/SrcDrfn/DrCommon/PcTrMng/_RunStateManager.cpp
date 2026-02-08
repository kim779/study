// PcTrMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PcTrMng.h"

CRunStateManager::CRunStateManager()
{
	for(int i=0; i<MaxRunState; i++) m_dwInterface[i]=NULL;
}

CRunStateManager::~CRunStateManager()
{
}

BOOL CRunStateManager::NotifyState(LPCSTR szKey, long dwState, long dwStateData)
{
// 	if(strcmp(szKey, RunState_Gwansim)==0)
// 	{
// 		if(dwState==RunState_Start)
// 		{
// 			// theApp.m_GwanMng.CreateGwanWnd();
// 			theApp.m_GwanMng.ConnectGwansimLib(TRUE);
// 			//{{ Test
// 			//DSBM_RUNSTATE Data;long dwLong=NULL;
// 			//theApp.m_OrderMonMng.SetState(&Data, dwLong);
// 			//}} Test
// 			return TRUE;
// 		}
// 	}
// 	else 
	if(strcmp(szKey, RunState_AUTOTRADE)==0)
	{
		if(dwState==RunState_Start || dwState==RunState_End)
		{
			m_dwInterface[RunModule_AutoTrade] = dwStateData;
			return TRUE;
		}
	}

	return FALSE;
}

