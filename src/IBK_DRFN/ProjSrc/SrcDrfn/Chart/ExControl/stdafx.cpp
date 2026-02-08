// stdafx.cpp : source file that includes just the standard includes
//	ExControl.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "../../inc/BaseDesk.h"			// CBaseDesk2ฐüทร
#include "../../inc/ExOCXDLL.h"
#include "../../inc/IBaseDefine.h"
#include "../../inc/IGateMng.h"
#include "../../inc/IMainExInfoMng.h"
#include "../../inc/IAUPlatformDispMng.h"
#include "../../inc/winhand_.h"


#ifdef _COMMFUNC
CArray<HANDLE, HANDLE>		m_arPermanentHandle;

BOOL CompareParamter(LPSTR lpszCompare, CString strParamter)
{
	int nIndex = 0;
	CString strParam;

	while(AfxExtractSubString(strParam, strParamter, nIndex++, _T(';')))
	{
		if(strParam.IsEmpty())
			continue;

		strParam.Replace(_T(" "), _T(""));
		if(!strParam.CompareNoCase(lpszCompare))
			return TRUE;
	}

	return FALSE;
}

void GetParamterValue(LPCTSTR lpszKey, CString& strValue, CString strParamter)
{
	int nIndex = 0;	
	CString strParam, strKey;
	
	while(AfxExtractSubString(strParam, strParamter, nIndex++, _T(';')))
	{
		if(strParam.IsEmpty())
			continue;
		
		AfxExtractSubString(strKey, strParamter, 0, _T('='));
		if(strKey.IsEmpty())
			continue;
		
		if(!strKey.CompareNoCase(lpszKey))
		{
			AfxExtractSubString(strValue, strParamter, 1, _T('='));
			return;
		}
	}
}

void SetPermanentHandle(HANDLE h, CObject* permOb)
{
// 	HWND hMainWnd;// = AfxGetMainWnd()->GetSafeHwnd();
// 	AfxGetHanaroMainWnd(hMainWnd);
// 
// 	IMainExInfoManager* pMainEx = (IMainExInfoManager*)::SendMessage(hMainWnd, RMSG_INTERFACE, 22, 0);
// 	if(!pMainEx)
// 		return;
// 
// 	COleVariant* pVar = pMainEx->GetData(4);
// 	if(!pVar)
// 		return;
// 
// 	AFX_MODULE_STATE* pState = (AFX_MODULE_STATE*)pVar->lVal;
// 	if(!pState)
// 		return;

	AFX_MODULE_STATE* pState = AfxGetAppModuleState();
	if(!pState)		return;

	AFX_MODULE_THREAD_STATE* pThread = pState->m_thread.GetData();
	char aDebug[MAX_PATH]={0,};
	if(pThread && pThread->m_pmapHWND)
	{
		pThread->m_pmapHWND->SetPermanent(h, permOb);
		m_arPermanentHandle.Add(h);

		sprintf(aDebug, "SetPermanentHandle %08X=%0X\n", permOb, h);
		OutputDebugString(aDebug);
	}
}

void RemovePermanentHandle(HANDLE h)
{
// 	HWND hMainWnd;// = AfxGetMainWnd()->GetSafeHwnd();
// 	AfxGetHanaroMainWnd(hMainWnd);
// 
// 	IMainExInfoManager* pMainEx = (IMainExInfoManager*)::SendMessage(hMainWnd, RMSG_INTERFACE, 22, 0);
// 	if(!pMainEx)
// 		return;
// 
// 	COleVariant* pVar = pMainEx->GetData(4);
// 	if(!pVar)
// 		return;
// 
// 	AFX_MODULE_STATE* pState = (AFX_MODULE_STATE*)pVar->lVal;
// 	if(!pState)
// 		return;

	AFX_MODULE_STATE* pState = AfxGetAppModuleState();
	if(!pState)		return;

	AFX_MODULE_THREAD_STATE* pThread = pState->m_thread.GetData();
	if(pThread && pThread->m_pmapHWND)
		pThread->m_pmapHWND->RemoveHandle(h);
}
#endif
