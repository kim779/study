// SubLoader.cpp: implementation of the CSubLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SubLoader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSubLoader::CSubLoader()
{
	m_hSTDll= NULL;
	m_fnSubInFuncST = NULL;
	m_fnInputs = NULL;
	m_lCntOfInput = 0;
	m_bCheckOfUseful = FALSE;
}

CSubLoader::~CSubLoader()
{
	UnloadDLL();
}

BOOL CSubLoader::LoadDLL(LPCTSTR lpSTFileName)
{
	m_strSTDll = lpSTFileName;
	m_hSTDll  = (HINSTANCE) ::LoadLibrary(m_strSTDll);
	if( m_hSTDll == NULL)
	{
		m_strError.Format("%s을 Load할 수 없습니다.",lpSTFileName);
		return FALSE;
	}
	m_fnSubInFuncST = (_DLL_SubInFuncST)::GetProcAddress(m_hSTDll,"fnSubInFuncST");
	if(m_fnSubInFuncST == NULL)
	{
		m_strError.Format("%s에서 fnSubInFuncST함수를 찾을 수 없습니다.",lpSTFileName);
		return FALSE;
	}
	m_fnInputs = (_DLL_GetInputs)::GetProcAddress(m_hSTDll,"fnGetInputs");
	if(m_fnInputs == NULL)
	{
		m_strError.Format("%s에서 fnGetInputs함수를 찾을 수 없습니다.",lpSTFileName);
		return FALSE;
	}
	CString strInput;
	for(m_lCntOfInput=0;m_lCntOfInput<10;m_lCntOfInput++)
	{
		strInput = m_fnInputs(m_lCntOfInput);
		if(strInput=="#") break;
		m_arrayInputs.Add(strInput);
	}	
	return TRUE;
}

BOOL CSubLoader::UnloadDLL()
{
	BOOL bResult = FALSE;
	bResult = FreeLibrary(m_hSTDll);
	if(bResult)
	{
		m_hSTDll = NULL;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

double CSubLoader::SubInFuncST(long lSTActor, long lIndex, BOOL bTemp0 /*= FALSE*/, BOOL bTemp1 /*= FALSE*/,BOOL bTemp2 /*= FALSE*/, BOOL bTemp3 /*= FALSE*/,BOOL bTemp4 /*= FALSE*/, BOOL bTemp5 /*= FALSE*/,BOOL bTemp6 /*= FALSE*/, BOOL bTemp7 /*= FALSE*/,BOOL bTemp8 /*= FALSE*/, BOOL bTemp9 /*= FALSE*/)
{
	if(!m_bCheckOfUseful)
	{
		long bTempAll = bTemp0+bTemp1+bTemp2+bTemp3+bTemp4+bTemp5+bTemp6+bTemp7+bTemp8+bTemp9;
		if(m_lCntOfInput!=bTempAll)
		{
			m_strError.Format("변수의 개수가 다릅니다. 확인하여 주십시오.");
			return 0.0;
		}
		m_bCheckOfUseful = TRUE;
	}	
	if(m_fnSubInFuncST)
	{
		return m_fnSubInFuncST(lSTActor,lIndex, bTemp0, bTemp1, bTemp2, bTemp3, bTemp4, bTemp5, bTemp6, bTemp7, bTemp8, bTemp9);
	}
	return 0.0;
}
