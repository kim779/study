// PcTrMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PcTrMng.h"

LONG CGateManager::GetInterface(int nID, long dwUnUse)
{
	if(nID<0 || nID>=MAX_INTERFACE)
		return 0L;

	switch(nID)
	{
		case 9: //UUID_IAUGwanManager
			if(theApp.m_uaInterface[nID].pInterface==NULL )
			{
				//(1) 관심Manager요청
				theApp.m_uaInterface[nID].pInterface = theApp.m_pPctrWinixLoader->PTMGetInferface(1, NULL);
			}
			break;
		case 11:
			if(theApp.m_uaInterface[11].pInterface==NULL )
			{
//				theApp.m_uaInterface[11].pInterface = theApp.m_OrderLoader.OrderMng_GetInferface(0);
			}
			break;
		case 26:
			if(theApp.m_uaInterface[nID].pInterface==NULL)
			{
				theApp.m_pTLMngLoader->LoadLib();
				theApp.m_uaInterface[nID].pInterface = theApp.m_pTLMngLoader->TLM_GetInferface((long)theApp.m_uaInterface[0].pInterface);
			}
		case 27:
			if(theApp.m_uaInterface[nID].pInterface==NULL)
			{
//				theApp.m_pECWMngLoader->LoadLib();
//				theApp.m_uaInterface[nID].pInterface = theApp.m_pECWMngLoader->ECW_GetInferface((long)theApp.m_uaInterface[0].pInterface);
			}
			break;
		case 50: //UUID_IPartnerSvrManager:
			if(theApp.m_uaInterface[nID].pInterface==NULL )
			{
				theApp.m_uaInterface[nID].pInterface = theApp.m_pPtMngLoader->PTMGetInferface(0, NULL);
			}
			break;
		case 51: //UUID_IChartBozoMsgManager:
			if(theApp.m_uaInterface[nID].pInterface==NULL )
			{
				theApp.m_uaInterface[nID].pInterface = theApp.m_pPtMngLoader->PTMGetInferface(1, NULL);
				theApp.m_pPtMngLoader->PTMSetPcTrInterface(UUID_IGateManager, (long)&theApp.m_GateMng);
			}
			break;
		default: break;
	}
	
	return theApp.m_uaInterface[nID].pInterface;
}

