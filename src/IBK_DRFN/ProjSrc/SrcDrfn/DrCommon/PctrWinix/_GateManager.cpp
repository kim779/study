// PcTrMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PctrWinix.h"

LONG CGateManager::GetInterface(int nID, long dwUnUse)
{
	if(nID<0 || nID>=MAX_Partner_INTERFACE)
		return 0L;
	
	return theApp.m_uaInterface[nID].pInterface;
}

