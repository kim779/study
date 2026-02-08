// PcTrMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PcTrMng.h"

/////////////////////////////////////////////////////////////////////////////
// CPCTRManager

CPCTRManager::CPCTRManager()
{
}

CPCTRManager::~CPCTRManager()
{
}

LPCSTR CPCTRManager::GetData(int nID)
{
	return theApp.m_MainInfoMng.GetDataString(nID);
}

BOOL CPCTRManager::SetData(int nID, LPCSTR szNewData)
{
	return theApp.m_MainInfoMng.SetDataString(nID, szNewData);
}

COleVariant* CPCTRManager::GetDataEx(int nID)
{
	return theApp.m_MainExinfoMng.GetData(nID);
}

BOOL CPCTRManager::SetDataEx(int nID, COleVariant* pNewData)
{
	return theApp.m_MainExinfoMng.SetData(nID, pNewData);
}

COleVariant* CPCTRManager::GetDataEx(LPCSTR szKey)
{
	return theApp.m_MainExinfoMng.GetData(szKey);
}

BOOL CPCTRManager::SetDataEx(LPCSTR szKey, COleVariant* pNewData)
{
	return theApp.m_MainExinfoMng.SetData(szKey, pNewData);
}

LONG CPCTRManager::GetTrData(char szTR, int nCount, LPCSTR szData, long dwKey, LPCSTR szKey)
{
	return 0L;
}

LONG CPCTRManager::Advise(int nKey, IPCTRSite* pSite)
{
	return 0L;
}

void CPCTRManager::UnAdvise(LONG pKey)
{
}

LONG CPCTRManager::AdviseEx(int nKey, IPCTRSite* pSite)
{
	return 0L;
}

void CPCTRManager::UnAdviseEx(LONG pKey)
{
}

