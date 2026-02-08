// InnerFuncLoader.cpp: implementation of the CInnerFuncLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InnerFuncLoader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInnerFuncLoader::CInnerFuncLoader()
{
	m_hSTInnerFunctionDll = NULL;
	m_fnLoadInnerFunction = NULL;
}

CInnerFuncLoader::~CInnerFuncLoader()
{
	UnloadInnerFuncDLL();
}

BOOL CInnerFuncLoader::LoadInnerFuncDLL(LPCTSTR lpInnerFuncDll)
{
	if(m_hSTInnerFunctionDll) return FALSE;
	m_strSTInnerFunctionDLL = lpInnerFuncDll;
	m_hSTInnerFunctionDll  = (HINSTANCE) ::LoadLibrary(m_strSTInnerFunctionDLL);
	if( m_hSTInnerFunctionDll == NULL)
	{
		m_strError.Format("%s을 Load할 수 없습니다.",m_strSTInnerFunctionDLL);
		return FALSE;
	}
	m_fnLoadInnerFunction = (_DLL_LoadInnerFunction)::GetProcAddress(m_hSTInnerFunctionDll,"fnLoadInnerFunction");
	if(m_fnLoadInnerFunction == NULL)
	{
		m_strError.Format("%s에서 fnLoadInnerFunction 함수를 찾을 수 없습니다.",m_strSTInnerFunctionDLL);
		return FALSE;
	}
	m_fnGetInputs = (_DLL_GetInputs)::GetProcAddress(m_hSTInnerFunctionDll,"fnGetInputs");
	if(m_fnGetInputs == NULL)
	{
		m_strError.Format("%s에서 fnGetInputs 함수를 찾을 수 없습니다.",m_strSTInnerFunctionDLL);
		return FALSE;
	}
	return TRUE;
}

BOOL CInnerFuncLoader::UnloadInnerFuncDLL()
{
	if(!m_hSTInnerFunctionDll) return FALSE;
	BOOL bResult = FALSE;
	bResult = FreeLibrary(m_hSTInnerFunctionDll);
	if(bResult)
	{
		m_hSTInnerFunctionDll = NULL;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

long CInnerFuncLoader::LoadInnerFunction(long lIndex)
{
	if(m_fnLoadInnerFunction)
	{
		return m_fnLoadInnerFunction(lIndex);
	}
	return NULL;	
}

BOOL CInnerFuncLoader::GetInputs(long lType, long lIndex, CString &rStrFuncName, CString &rStrInputs)
{
	if(m_fnGetInputs)
	{
		return m_fnGetInputs(lType, lIndex, rStrFuncName, rStrInputs);
	}
	return FALSE;
}