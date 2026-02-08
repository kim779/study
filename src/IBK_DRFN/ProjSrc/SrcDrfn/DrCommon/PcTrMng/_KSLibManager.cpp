// PcTrMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PcTrMng.h"

//#include "../../../../CommonSrc/LinkDataManager.h"
//#include "../../../../Linkmanager/LinkManager.h"

//@신영제거#include "../../../../Builder/LinkInfoMng/KSLinkInfoMng.h"
//@신영제거#include "../../../../Controls/GDIMgr/ColorMap.h"
//@신영제거#include "../../../../Common/CommHeaderDef.h"

//@신영제거extern CString KSLink_GetFileHeader();		// 삼성에서 암호화된 아이디를 얻기위해 외부링크.
//@신영제거extern CString KSLink_GetLoginInfo(CString strTag);
//@신영제거extern int     KSLink_GetAccountCount();
//@신영제거extern CString KSLink_GetAccount(int nIndex);
//@신영제거extern CString KSLink_GetAccInfo(CString strAccount, int nType);
//@신영제거extern LOGIN_ACC_INFO* KSLink_GetAccInfo(CString strAccount);
//@신영제거extern CColorMap* GetColorMap();

CKSLibManager::CKSLibManager()
{
	m_pColorMap = NULL;
}

BOOL CKSLibManager::Lib_KSLink_SetLinkTagData( LPCTSTR sTag, LPCTSTR sValue)
{
//@신영제거::KSLink_SetLinkTagData(sTag, sValue);
	return TRUE;
}

BOOL CKSLibManager::Lib_KSLink_SetFormLinkTagData(CString sTag, CString sValue)
{
//@신영제거::KSLink_SetFormLinkTagData(sTag, sValue);
	return TRUE;
}

CString CKSLibManager::Lib_KSLink_GetFileHeader()
{
//@신영제거return ::KSLink_GetFileHeader();
	return "";
}

CString CKSLibManager::Lib_KSLink_GetLoginInfo(CString strTag)
{
//@신영제거return ::KSLink_GetLoginInfo(strTag);
	return "";
}

int CKSLibManager::Lib_KSLink_GetAccountCount()
{
//@신영제거return ::KSLink_GetAccountCount();
	return 0;
}

CString CKSLibManager::Lib_KSLink_GetAccount(int nIndex)
{
//@신영제거return ::KSLink_GetAccount(nIndex);
	return "";
}

BYTE CKSLibManager::Lib_CommRequestData(HWND hFormWnd, LONG _pRqCommInfo, BYTE* lpData, int nDataLen, 
					 BOOL bDownLoad, int nProgressType, BOOL bWaitCursor)
{
//@신영제거	REQUEST_COMMINFO* pRqCommInfo = (REQUEST_COMMINFO*)_pRqCommInfo;
//@신영제거	return ::CommRequestData(hFormWnd, pRqCommInfo, lpData, nDataLen, bDownLoad, nProgressType, bWaitCursor);
	return (BYTE)1;
}

CString CKSLibManager::Lib_KSLink_GetAccInfo(CString strAccount, int nType)
{
//@신영제거	return ::KSLink_GetAccInfo(strAccount, nType);
	return "";
}

long CKSLibManager::Lib_KSLink_GetAccInfo(CString strAccount)
{
//@신영제거	return (long)::KSLink_GetAccInfo(strAccount);
	return 0l;
}

COLORREF CKSLibManager::Lib_KSLink_GetColor(short nIndex, BOOL bUseOrgIndex)
{
//@신영제거
//	if(!m_pColorMap)
//		m_pColorMap = (long)::GetColorMap();
//
//	if(!m_pColorMap) return(RGB(255,255,255));
//
//	CColorMap* pColorMap = (CColorMap*)m_pColorMap;
//	return pColorMap->GetColor(nIndex);
	return RGB(0,0,0);
}


long CKSLibManager::Lib_KSLink_GetEnvInfo(int nType)
{

//@신영제거	return ::KSLink_GetEnvInfo(nType);
	return 0l;
}
