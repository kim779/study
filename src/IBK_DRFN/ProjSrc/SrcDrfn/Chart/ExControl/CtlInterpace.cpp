#include "stdafx.h"
#include "../../Inc/ExOCXDLL.h"

ULONG  CExOCXDLL::XPlatformObj::AddRef() { return S_OK;}
ULONG CExOCXDLL::XPlatformObj::Release(){ return S_OK;}
HRESULT CExOCXDLL::XPlatformObj::QueryInterface(THIS_ REFIID riid, LPVOID FAR * lplpObj){ return S_OK;}

long CExOCXDLL::XPlatformObj::PFGetBase(LPCTSTR szKey) 
{
	AZMETHOD_PROLOGUE(CExOCXDLL, PlatformObj)
	return pThis->Platform_GetBase(szKey);
}

long CExOCXDLL::XPlatformObj::PFGetKey(LPCTSTR szKey)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, PlatformObj)
	return pThis->Platform_GetKey(szKey);
}

void CExOCXDLL::XPlatformObj::PFSetKey(LPCTSTR szKey, long dwKey)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, PlatformObj)
	pThis->Platform_SetKey(szKey, dwKey);
}

void CExOCXDLL::XPlatformObj::PFInitialize(long dwMainDllObj, long dwBaseDesk,
										long dwRscMng, CStringList* pAllProperties)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, PlatformObj)
	pThis->Platform_Initialize(dwMainDllObj, dwBaseDesk, dwRscMng, pAllProperties);
}

void CExOCXDLL::XPlatformObj::PFSetProperty(STDATAITEM *stData, int nDataCnt)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, PlatformObj)
	pThis->Platform_SetProperty(stData, nDataCnt);
}

long CExOCXDLL::XPlatformObj::PFGetState(int nStatus, long dwData1, long dwData2)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, PlatformObj)
	return pThis->Platform_GetState(nStatus, dwData1, dwData2);
}

long CExOCXDLL::XPlatformObj::PFSetState(int nStatus, long dwData1, long dwData2)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, PlatformObj)
	return pThis->Platform_SetState(nStatus, dwData1, dwData2);
}

LRESULT CExOCXDLL::XPlatformObj::PFWindowProc(UINT message, WPARAM wParam, LPARAM lParam, int &nContinue)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, PlatformObj)
	return pThis->Platform_WindowProc(message, wParam, lParam, nContinue);
}

LRESULT CExOCXDLL::XPlatformObj::PFPreTranslateMessage(MSG* pMsg)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, PlatformObj)
	return pThis->Platform_PreTranslateMessage(pMsg);
}

ULONG CExOCXDLL::XWndObj::AddRef() { return S_OK;}
ULONG CExOCXDLL::XWndObj::Release(){ return S_OK;}
HRESULT CExOCXDLL::XWndObj::QueryInterface(THIS_ REFIID riid, LPVOID FAR * lplpObj){ return S_OK;}

void CExOCXDLL::XWndObj::OnSize(UINT nType, int cx, int cy)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, WndObj)
	pThis->Platform_OnSize(nType, cx, cy);
}

