// LoadMyNetFindLib.cpp: implementation of the CLoadMyNetFindLib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyNetFindLibLoader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyNetFindLibLoader::CMyNetFindLibLoader()
{
#ifdef _DEBUG
	m_szMyNetFindLib = "\\BinD\\MyNetFindLib.dll";
#else
	m_szMyNetFindLib = "\\Bin\\MyNetFindLib.dll";
#endif	
	m_hMyNetFindLib = NULL;
	m_hSelfWnd = NULL;
	m_hWndParent = NULL;
}

CMyNetFindLibLoader::~CMyNetFindLibLoader()
{
	UnloadMyNetFindLib();
}


/*
int CLoadMyNetFindLib::GetFindDefine(char *pFullFilePath, int nSize, LPSTR &lpData)//(CString strFullFilePath, LPSTR &lpData)
{
	if(!LoadMyNetFindLib())	return NULL;

	_DLL_GetFindDefine fnGetFindDefine;
	fnGetFindDefine = (_DLL_GetFindDefine)::GetProcAddress(m_hMyNetFindLib,"_DLL_GetFindDefine");
	if(fnGetFindDefine == NULL)
	{
		AfxMessageBox("_GetFindDefine함수를 찾을 수 없습니다.");
		return NULL;
	}
	return fnGetFindDefine(m_hSelfWnd, pFullFilePath, nSize, lpData);
}
*/


int CMyNetFindLibLoader::GetFindDefine_TR(char *pFullFilePath, int nSize, int &nTrIndex, LPSTR &lpData)
{
	if(!LoadMyNetFindLib())	return NULL;

	_DLL_GetFindDefine_TR fnGetFindDefine_TR;
	fnGetFindDefine_TR = (_DLL_GetFindDefine_TR)::GetProcAddress(m_hMyNetFindLib,"_DLL_GetFindDefine_TR");
	if(fnGetFindDefine_TR == NULL)
	{
		AfxMessageBox("_GetFindDefine_TR함수를 찾을 수 없습니다.");
		return NULL;
	}
	return fnGetFindDefine_TR(m_hSelfWnd, pFullFilePath, nSize, nTrIndex, lpData);
}


int CMyNetFindLibLoader::GetConditionList(int nType, BOOL bGetAlert, LPSTR &lpName, LPSTR &lpPath)
//int CLoadMyNetFindLib::GetConditionList(int nType, BOOL bGetAlert, LPSTR &lpData)//(int nType, LPSTR &lpData)
{
	if(!LoadMyNetFindLib())	return NULL;
	_DLL_GetConditionList fnGetConditionList;
	fnGetConditionList = (_DLL_GetConditionList)::GetProcAddress(m_hMyNetFindLib,"_DLL_GetConditionList_Path");
	if(fnGetConditionList == NULL)
	{
		AfxMessageBox("_GetConditionList함수를 찾을 수 없습니다.");
		return NULL;
	}
	return fnGetConditionList( nType, bGetAlert, lpName, lpPath);
}


BOOL CMyNetFindLibLoader::LoadMyNetFindLib()
{
	if(m_hMyNetFindLib) return TRUE;
	if(!m_hSelfWnd||!m_hWndParent)
	{
		AfxMessageBox("m_hSelfWnd or m_hWndParent is Null..");
		return FALSE;
	}
	m_hMyNetFindLib  = (HINSTANCE) ::LoadLibrary((LPCTSTR)(m_strRootPath+m_szMyNetFindLib));
	if( m_hMyNetFindLib == NULL)
	{
		AfxMessageBox("MyNetFindLib DLL을 Load할 수 없습니다.");
		return FALSE;
	}
	_DLL_SetLibOpen	fnSetLibOpen;
	fnSetLibOpen = (_DLL_SetLibOpen)::GetProcAddress(m_hMyNetFindLib,"_DLL_SetLibOpen");
	if(fnSetLibOpen == NULL)
	{
		AfxMessageBox("_DLL_SetLibOpen 함수를 찾을 수 없습니다.");
		return FALSE;
	}
	fnSetLibOpen(m_hSelfWnd, m_hWndParent);

	return TRUE;
}

BOOL CMyNetFindLibLoader::UnloadMyNetFindLib()
{
	if(!m_hMyNetFindLib) return TRUE;

	_DLL_SetLibClose SetLibClose;
	SetLibClose = (_DLL_SetLibClose)::GetProcAddress(m_hMyNetFindLib,"_DLL_SetLibClose");
	if(SetLibClose == NULL)
	{
		AfxMessageBox("_DLL_SetLibClose 함수를 찾을 수 없습니다.");
		return FALSE;
	}
	SetLibClose(m_hSelfWnd);

	BOOL bResult = NULL;
	bResult = FreeLibrary(m_hMyNetFindLib);
	if(bResult)
	{
		m_hMyNetFindLib = NULL;
		return TRUE;
	}
	return FALSE;
}
