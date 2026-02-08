#include "stdafx.h"
#include "../../Inc/ExOCXDLL.h"

ULONG CExOCXDLL::XCallBack::AddRef() { return S_OK;}
ULONG CExOCXDLL::XCallBack::Release(){ return S_OK;}
HRESULT CExOCXDLL::XCallBack::QueryInterface(THIS_ REFIID riid, LPVOID FAR * lplpObj){ return S_OK;}

HRESULT CExOCXDLL::XCallBack::RecieveData(HWND hTrComm, LPCSTR szTR, LPCSTR szMsgCode,LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, CallBack)
	return pThis->RecieveData(hTrComm, szTR, szMsgCode, szMessage, aTRData, dwTRDateLen);
}

HRESULT CExOCXDLL::RecieveData(HWND hTrComm, LPCSTR szTR, LPCSTR szMsgCode,LPCSTR szMessage, LPVOID aTRData, long dwTRDateLen)
{
	return 0L;
}

HRESULT CExOCXDLL::XCallBack::StatusChange(int nState)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, CallBack)
	return S_OK;
}

