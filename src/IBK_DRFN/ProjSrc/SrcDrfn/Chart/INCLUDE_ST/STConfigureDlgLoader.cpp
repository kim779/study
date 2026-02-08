// STConfigureDlgLoader.cpp: implementation of the CSTConfigureDlgLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "STConfigureDlgLoader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSTConfigureDlgLoader::CSTConfigureDlgLoader()
{
	m_hSTConfigureDlgLoader = NULL;
	m_fnShowConfigureDlg = NULL;
	m_fnShowConfigureDlgOnWizard = NULL;
	m_fnWriteConfigure = NULL;
	m_fnSetPassword = NULL;
	m_fnSetUserID = NULL;
	m_fnSetExpiredDate = NULL;
	m_fnGetPassword = NULL;
	m_fnGetUserID = NULL;
	m_fnGetExpiredDate = NULL;	
	m_fnSetUsePassword = NULL;
	m_fnSetUseUserID = NULL;
	m_fnSetUseExpiredDate = NULL;
	m_fnGetUsePassword = NULL;
	m_fnGetUseUserID = NULL;
	m_fnGetUseExpiredDate = NULL;
	m_strSTConfigureDlgLoader = "STConfigureDlg.dll";
	LoadSTConfigureDlgLoader();
}

CSTConfigureDlgLoader::~CSTConfigureDlgLoader()
{
	UnloadSTConfigureDlgLoader();
}

long CSTConfigureDlgLoader::LoadSTConfigureDlgLoader()
{
	if(m_hSTConfigureDlgLoader) return FALSE;
	CString strMessage;	
	m_hSTConfigureDlgLoader  = (HINSTANCE) ::LoadLibrary(m_strSTConfigureDlgLoader);
	if( m_hSTConfigureDlgLoader == NULL)
	{
		strMessage.Format("%s을 Load할 수 없습니다.",m_strSTConfigureDlgLoader);
		AfxMessageBox(strMessage);
		return FALSE;
	}
	m_fnShowConfigureDlg = (_DLL_ShowConfigureDlg)::GetProcAddress(m_hSTConfigureDlgLoader,"fnShowConfigureDlg");
	if(m_fnShowConfigureDlg == NULL)
	{
		strMessage.Format("%s에서 fnShowConfigureDlg 함수를 찾을 수 없습니다.",m_strSTConfigureDlgLoader);
		AfxMessageBox(strMessage);
		return FALSE;
	}
	
	m_fnShowConfigureDlgOnWizard = (_DLL_ShowconfigureDlgOnWizard)::GetProcAddress(m_hSTConfigureDlgLoader,"fnShowConfigureDlgOnWizard");
	if(m_fnShowConfigureDlgOnWizard == NULL)
	{
		strMessage.Format("%s에서 fnShowConfigureDlgOnWizard 함수를 찾을 수 없습니다.",m_strSTConfigureDlgLoader);
		AfxMessageBox(strMessage);
		return FALSE;
	}

	m_fnWriteConfigure = (_DLL_WriteConfigure)::GetProcAddress(m_hSTConfigureDlgLoader,"fnWriteConfigure");
	if(m_fnWriteConfigure == NULL)
	{
		strMessage.Format("%s에서 fnWriteConfigure 함수를 찾을 수 없습니다.",m_strSTConfigureDlgLoader);
		AfxMessageBox(strMessage);
		return FALSE;
	}

	m_fnSetPassword = (_DLL_SetPassword)::GetProcAddress(m_hSTConfigureDlgLoader,"fnSetPassword");
	if(m_fnSetPassword == NULL)
	{
		strMessage.Format("%s에서 fnSetPassword 함수를 찾을 수 없습니다.",m_strSTConfigureDlgLoader);
		AfxMessageBox(strMessage);
		return FALSE;
	}

	m_fnSetUserID = (_DLL_SetUserID)::GetProcAddress(m_hSTConfigureDlgLoader,"fnSetUserID");
	if(m_fnSetUserID == NULL)
	{
		strMessage.Format("%s에서 fnSetPassword 함수를 찾을 수 없습니다.",m_strSTConfigureDlgLoader);
		AfxMessageBox(strMessage);
		return FALSE;
	}

	m_fnSetExpiredDate = (_DLL_SetExpiredDate)::GetProcAddress(m_hSTConfigureDlgLoader,"fnSetExpiredDate");
	if(m_fnSetExpiredDate == NULL)
	{
		strMessage.Format("%s에서 fnSetExpiredDate 함수를 찾을 수 없습니다.",m_strSTConfigureDlgLoader);
		AfxMessageBox(strMessage);
		return FALSE;
	}

	m_fnGetPassword = (_DLL_GetPassword)::GetProcAddress(m_hSTConfigureDlgLoader,"fnGetPassword");
	if(m_fnGetPassword == NULL)
	{
		strMessage.Format("%s에서 fnGetPassword 함수를 찾을 수 없습니다.",m_strSTConfigureDlgLoader);
		AfxMessageBox(strMessage);
		return FALSE;
	}

	m_fnGetUserID = (_DLL_GetUserID)::GetProcAddress(m_hSTConfigureDlgLoader,"fnGetUserID");
	if(m_fnGetUserID == NULL)
	{
		strMessage.Format("%s에서 fnGetUserID 함수를 찾을 수 없습니다.",m_strSTConfigureDlgLoader);
		AfxMessageBox(strMessage);
		return FALSE;
	}
	
	m_fnGetExpiredDate = (_DLL_GetExpiredDate)::GetProcAddress(m_hSTConfigureDlgLoader,"fnGetExpiredDate");
	if(m_fnGetExpiredDate == NULL)
	{
		strMessage.Format("%s에서 fnGetExpiredDate 함수를 찾을 수 없습니다.",m_strSTConfigureDlgLoader);
		AfxMessageBox(strMessage);
		return FALSE;
	}

	m_fnSetUsePassword = (_DLL_SetUsePassword)::GetProcAddress(m_hSTConfigureDlgLoader,"fnSetUsePassword");
	if(m_fnSetUsePassword == NULL)
	{
		strMessage.Format("%s에서 fnSetUsePassword 함수를 찾을 수 없습니다.",m_strSTConfigureDlgLoader);
		AfxMessageBox(strMessage);
		return FALSE;
	}

	m_fnSetUseUserID = (_DLL_SetUseUserID)::GetProcAddress(m_hSTConfigureDlgLoader,"fnSetUseUserID");
	if(m_fnSetUseUserID == NULL)
	{
		strMessage.Format("%s에서 fnSetUseUserID 함수를 찾을 수 없습니다.",m_strSTConfigureDlgLoader);
		AfxMessageBox(strMessage);
		return FALSE;
	}

	m_fnSetUseExpiredDate = (_DLL_SetUseExpiredDate)::GetProcAddress(m_hSTConfigureDlgLoader,"fnSetUseExpiredDate");
	if(m_fnSetUseExpiredDate == NULL)
	{
		strMessage.Format("%s에서 fnSetUseExpiredDate 함수를 찾을 수 없습니다.",m_strSTConfigureDlgLoader);
		AfxMessageBox(strMessage);
		return FALSE;
	}

	m_fnGetUsePassword = (_DLL_GetUsePassword)::GetProcAddress(m_hSTConfigureDlgLoader,"fnGetUsePassword");
	if(m_fnGetUsePassword == NULL)
	{
		strMessage.Format("%s에서 fnGetUsePassword 함수를 찾을 수 없습니다.",m_strSTConfigureDlgLoader);
		AfxMessageBox(strMessage);
		return FALSE;
	}

	m_fnGetUseUserID = (_DLL_GetUseUserID)::GetProcAddress(m_hSTConfigureDlgLoader,"fnGetUseUserID");
	if(m_fnGetUseUserID == NULL)
	{
		strMessage.Format("%s에서 fnGetUseUserID 함수를 찾을 수 없습니다.",m_strSTConfigureDlgLoader);
		AfxMessageBox(strMessage);
		return FALSE;
	}

	m_fnGetUseExpiredDate = (_DLL_GetUseExpiredDate)::GetProcAddress(m_hSTConfigureDlgLoader,"fnGetUseExpiredDate");
	if(m_fnGetUseExpiredDate == NULL)
	{
		strMessage.Format("%s에서 fnGetUseExpiredDate 함수를 찾을 수 없습니다.",m_strSTConfigureDlgLoader);
		AfxMessageBox(strMessage);
		return FALSE;
	}
	return TRUE;
}

BOOL CSTConfigureDlgLoader::UnloadSTConfigureDlgLoader()
{
	if(!m_hSTConfigureDlgLoader) return FALSE;
	BOOL bResult = FALSE;
	bResult = FreeLibrary(m_hSTConfigureDlgLoader);
	if(bResult)
	{
		m_hSTConfigureDlgLoader = NULL;
		m_fnShowConfigureDlg = NULL;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CSTConfigureDlgLoader::ShowConfigureDlg(HWND hParent, CISTControllerD *pISTController, LPCTSTR lpDLLName, int nType)
{
	if(m_fnShowConfigureDlg)
	{
		BOOL bRet = m_fnShowConfigureDlg(hParent,pISTController,lpDLLName,nType);
		return bRet;
	}
	return FALSE;
}

BOOL CSTConfigureDlgLoader::ShowConfigureDlgOnWizard(HWND hParent, CISTControllerD *pISTController, LPCTSTR lpDLLName, int nType, CRect* pRect)
{
	if(m_fnShowConfigureDlgOnWizard)
	{
		BOOL bRet = m_fnShowConfigureDlgOnWizard(hParent,pISTController,lpDLLName,nType, pRect);
		return bRet;
	}
	return FALSE;
}

BOOL CSTConfigureDlgLoader::WriteConfigure(HWND hParent,  CISTControllerD *pISTController, LPCTSTR lpFileName, int nType, int nMode)
{
	if(m_fnWriteConfigure)
	{
		return m_fnWriteConfigure(hParent,pISTController,lpFileName,nType,nMode);
	}
	return FALSE;
}

void CSTConfigureDlgLoader::SetPassword(LPCTSTR lpPassword)
{
	if(m_fnSetPassword)
	{
		m_fnSetPassword(lpPassword);
	}
}

void CSTConfigureDlgLoader::SetUserID(LPCTSTR lpUserID)
{
	if(m_fnSetUserID)
	{
		m_fnSetUserID(lpUserID);
	}
}

void CSTConfigureDlgLoader::SetExpiredDate(LPCTSTR lpExpiredDate)
{
	if(m_fnSetExpiredDate)
	{
		m_fnSetExpiredDate(lpExpiredDate);
	}
}

CString CSTConfigureDlgLoader::GetPassword()
{
	if(m_fnGetPassword)
	{
		return m_fnGetPassword();
	}
	return "";
}

CString CSTConfigureDlgLoader::GetUserID()
{
	if(m_fnGetUserID)
	{
		return m_fnGetUserID();
	}
	return "";
}

CString CSTConfigureDlgLoader::GetExpiredDate()
{
	if(m_fnGetExpiredDate)
	{
		return m_fnGetExpiredDate();
	}
	return "";
}

void CSTConfigureDlgLoader::SetUsePassword(BOOL bUse)
{
	if(m_fnSetUsePassword)
	{
		m_fnSetUsePassword(bUse);
	}
}

void CSTConfigureDlgLoader::SetUseUserID(BOOL bUse)
{
	if(m_fnSetUseUserID)
	{
		m_fnSetUseUserID(bUse);
	}
}

void CSTConfigureDlgLoader::SetUseExpiredDate(BOOL bUse)
{
	if(m_fnSetUseExpiredDate)
	{
		m_fnSetUseExpiredDate(bUse);
	}
}

BOOL CSTConfigureDlgLoader::GetUsePassword()
{
	if(m_fnGetUsePassword)
	{
		return m_fnGetUsePassword();
	}
	return FALSE;
}

BOOL CSTConfigureDlgLoader::GetUseUserID()
{
	if(m_fnGetUseUserID)
	{
		return m_fnGetUseUserID();
	}
	return FALSE;
}

BOOL CSTConfigureDlgLoader::GetUseExpiredDate()
{
	if(m_fnGetUseExpiredDate)
	{
		return m_fnGetUseExpiredDate();
	}
	return FALSE;
}
