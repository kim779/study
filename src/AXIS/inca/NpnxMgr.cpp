// NpnxMgr.cpp: implementation of the CNpnxMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NpnxMgr.h"
#include "Winsvc.h"
#include <io.h>

#pragma warning(disable : 4996)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#pragma warning (disable : 26495)
#pragma warning (disable : 6386)
#pragma warning (disable : 6387)
#pragma warning (disable : 6011)

CNpnxMgr::CNpnxMgr()
:m_strCustomerID(_T("")) , m_strCurrentPath(_T("")), m_strModulePath(_T(""))
{
	m_hWnd = NULL;
	m_dwhwnd = 0;
	m_bOutMessage = FALSE;
	

	lpfnCloseNPN = NULL;
}

CNpnxMgr::~CNpnxMgr()
{
	OutputDebugString("~CNpnxMgr()");
	if (m_hModule != NULL)
	{
		FreeLibrary(m_hModule);
		m_hModule = NULL;
	}
	OutputDebugString("~CNpnxMgr() End");
}

BOOL CNpnxMgr::isAdmin()
{
	// Have to add code for win 9x user
	
	// When 98se user
	OSVERSIONINFOEX osvi{};
	BOOL bOsVersionInfoEx{};

	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	// If that fails, try using the OSVERSIONINFO structure.

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
		{
			if(m_bOutMessage)
				AfxMessageBox("보안 프로그램은 관리자권한이 필요합니다.\n보안프로그램을 실행하실려면 관리자 권한으로 실행을 하셔야 합니다.");
			return FALSE;
		}
	}

	switch (osvi.dwPlatformId)
	{
		case VER_PLATFORM_WIN32_WINDOWS:
			if(osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
			{
				//printf("Microsoft Windows 98 ");
				//if (osvi.szCSDVersion[1] == 'A')
					return true;
			}
			if(osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
			{
				//printf("Microsoft Windows Me ");
				//if (osvi.szCSDVersion[1] == 'A')
					return true;
			}
	}

	// When more 2K
	HANDLE		schSCManager;

	schSCManager = OpenSCManager(
				NULL,
				NULL,
				GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE
				);

	if(schSCManager == NULL){
		const DWORD dw = GetLastError();
		if(m_bOutMessage)
			AfxMessageBox("보안 프로그램은 관리자권한이 필요합니다.\n보안프로그램을 실행하실려면 관리자 권한으로 실행을 하셔야 합니다.");
		return false;
		//ExitProcess(dw);
	}		

	return true;
}

void CNpnxMgr::Init(BOOL bOutMessage)
{
	m_bOutMessage = bOutMessage;
	if(!isAdmin())
		return;
	//Dll Load
	TCHAR szDllPath[MAX_PATH] = {0, }, szFolderPath[MAX_PATH] = {0, };

	const TCHAR szPath[1024] = {0, };
	GetModuleFileName(NULL, szFolderPath, 1024);
	char *ptr = (char*)strrchr(szFolderPath, '\\');
	*ptr = 0;

	/*
	if(ptr != NULL)
	{
		delete ptr;
		ptr = NULL;
	}*/


	OutputDebugString(szFolderPath);
	wsprintf(szDllPath, "%s\\npnx.dll", szFolderPath);
	if(access(szDllPath, 00) == -1)
	{
		GetSystemDirectory(szFolderPath, MAX_PATH);

		wsprintf(szDllPath, "%s\\npnx.dll", szFolderPath);

		if(access(szDllPath, 00) == -1)
		{
			return;
		}
	}

	m_hModule = LoadLibrary("npnx.dll");

	if(m_hModule == NULL)
	{
		OutputDebugString("m_hModule == NULL");
		return;
	}else
	{
		OutputDebugString("m_hModule != NULL");
	}

	OutputDebugString("[npnx]1");
	lpfnGoNPN = NULL;
	lpfnGoNPN = (BOOL(*)(char *szID))
		GetProcAddress(m_hModule, "GoNPN");
	
	OutputDebugString("[npnx]2");
	lpfnGoNPNEx = NULL;
	lpfnGoNPNEx = (BOOL(*)(char *szID, char *szCurrentPath))
		GetProcAddress(m_hModule, "GoNPNEx");

	OutputDebugString("[npnx]3");
	lpfnIsExecute = NULL;
	lpfnIsExecute = (BOOL(*)())
		GetProcAddress(m_hModule, "IsExecute");

	OutputDebugString("[npnx]4");
	lpfnCloseNPN = NULL;
	lpfnCloseNPN = (BOOL(*)())
		GetProcAddress(m_hModule, "CloseNPN");

	OutputDebugString("[npnx]5");
	lpfnCopyModule = NULL;
	lpfnCopyModule = (BOOL(*)(char *szCurrentPath))
		GetProcAddress(m_hModule, "CopyModule");

	OutputDebugString("[npnx]6");
	lpfnGoNPNQuick = NULL;
	lpfnGoNPNQuick = (BOOL(*)(char *szID, char *szCurrentPath))
		GetProcAddress(m_hModule, "GoNPNQuick");

	OutputDebugString("[npnx]7");
	lpfnGoNPNCheck = NULL;
	lpfnGoNPNCheck = (BOOL(*)(char *szID, char *szCurrentPath))
		GetProcAddress(m_hModule, "GoNPNCheck");

	OutputDebugString("[npnx]8");
	lpfnSetHwnd = NULL;
	lpfnSetHwnd = (void(*)(DWORD dwHWND))
		GetProcAddress(m_hModule, "SetHwnd");

	if ( (lpfnGoNPN == NULL) ||
		 (lpfnGoNPNEx == NULL) ||
		 (lpfnIsExecute == NULL) ||
		 (lpfnCloseNPN == NULL) ||
		 (lpfnCopyModule == NULL) ||
		 (lpfnSetHwnd == NULL))
	{
		OutputDebugString("Dll load is failed.");
		return;
	}
//실행

	OutputDebugString("[npnx]9");
	
	if(m_hWnd != NULL)
	{
		lpfnSetHwnd((DWORD)m_hWnd);
	}
	else
	{
		OutputDebugString("[NpnxMgr] HWND == NULL");

		if( m_dwhwnd != 0 )
			lpfnSetHwnd(m_dwhwnd);
	}

	OutputDebugString("[npnx]10");
	if(!m_strModulePath.IsEmpty())
	{
		if (lpfnCopyModule != NULL)
		{
			if(!m_strModulePath.IsEmpty())
			{
				if(lpfnCopyModule(m_strModulePath.GetBuffer(0)) != TRUE)
				{
					OutputDebugString("Copy Module Failed.");
				}
			}
			else
			{
				OutputDebugString(m_strModulePath);
			}
		}
	}

	OutputDebugString("[npnx]11");
	if(m_strCurrentPath.IsEmpty())
	{
		OutputDebugString("[npnx]12");
		if (!m_strCustomerID.IsEmpty())
		{
			if (lpfnGoNPN != NULL)
			{
				if (lpfnGoNPN(m_strCustomerID.GetBuffer(0)) != TRUE)			
					OutputDebugString("Execution of Netizen failed.");
			}
		}else
		{
			AfxMessageBox("기업 아이디를 입력하여 주십시오");
		}
	}else
	{
		OutputDebugString("[npnx]13");
		if (!m_strCustomerID.IsEmpty())
		{
			if (lpfnGoNPNEx != NULL)
			{
				OutputDebugString("path");
				OutputDebugString(m_strCustomerID);
				OutputDebugString(m_strCurrentPath);
				m_strCurrentPath += "\\";
				if (lpfnGoNPNEx(m_strCustomerID.GetBuffer(0), m_strCurrentPath.GetBuffer(0)) != TRUE)
					OutputDebugString("Execution of Netizen failed.");
			}
		}else
		{
			AfxMessageBox("기업 아이디를 입력하여 주십시오");
		}
	}
}

void CNpnxMgr::CloseNetizen()
{
	if (lpfnCloseNPN != NULL)
	{
		lpfnCloseNPN();
	}
	else
	{
		OutputDebugString("lpfnCloseNPN == NULL");
	}
}

BOOL CNpnxMgr::IsExecute()
{
	m_hModule = LoadLibrary("npnx.dll");

	if(m_hModule == NULL)
	{
		OutputDebugString("m_hModule == NULL");
		return FALSE;
	}else
	{
		OutputDebugString("m_hModule != NULL");
	}

	lpfnIsExecute = NULL;
	lpfnIsExecute = (BOOL(*)())
		GetProcAddress(m_hModule, "IsExecute");

	if (lpfnIsExecute != NULL)
	{
		if (lpfnIsExecute() == FALSE)			
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CNpnxMgr::Quick(BOOL bOutMessage)
{
	m_bOutMessage = bOutMessage;
	if(!isAdmin())
		return;
	//Dll Load
	TCHAR szDllPath[MAX_PATH] = {0, }, szFolderPath[MAX_PATH] = {0, };

	const TCHAR szPath[1024] = {0, };
	GetModuleFileName(NULL, szFolderPath, 1024);
	char *ptr = (char*)strrchr(szFolderPath, '\\');
	*ptr = 0;

	/*
	if(ptr != NULL)
	{
		delete ptr;
		ptr = NULL;
	}*/


	OutputDebugString(szFolderPath);
	wsprintf(szDllPath, "%s\\npnx.dll", szFolderPath);
	if(access(szDllPath, 00) == -1)
	{
		GetSystemDirectory(szFolderPath, MAX_PATH);

		wsprintf(szDllPath, "%s\\npnx.dll", szFolderPath);

		if(access(szDllPath, 00) == -1)
		{
			return;
		}
	}

	m_hModule = LoadLibrary("npnx.dll");

	if(m_hModule == NULL)
	{
		OutputDebugString("m_hModule == NULL");
		return;
	}else
	{
		OutputDebugString("m_hModule != NULL");
	}

	OutputDebugString("[npnx]1");
	lpfnGoNPN = NULL;
	lpfnGoNPN = (BOOL(*)(char *szID))
		GetProcAddress(m_hModule, "GoNPN");
	
	OutputDebugString("[npnx]2");
	lpfnGoNPNEx = NULL;
	lpfnGoNPNEx = (BOOL(*)(char *szID, char *szCurrentPath))
		GetProcAddress(m_hModule, "GoNPNEx");

	OutputDebugString("[npnx]3");
	lpfnIsExecute = NULL;
	lpfnIsExecute = (BOOL(*)())
		GetProcAddress(m_hModule, "IsExecute");

	OutputDebugString("[npnx]4");
	lpfnCloseNPN = NULL;
	lpfnCloseNPN = (BOOL(*)())
		GetProcAddress(m_hModule, "CloseNPN");

	OutputDebugString("[npnx]5");
	lpfnCopyModule = NULL;
	lpfnCopyModule = (BOOL(*)(char *szCurrentPath))
		GetProcAddress(m_hModule, "CopyModule");

	OutputDebugString("[npnx]6");
	lpfnGoNPNQuick = NULL;
	lpfnGoNPNQuick = (BOOL(*)(char *szID, char *szCurrentPath))
		GetProcAddress(m_hModule, "GoNPNQuick");

	OutputDebugString("[npnx]7");
	lpfnGoNPNCheck = NULL;
	lpfnGoNPNCheck = (BOOL(*)(char *szID, char *szCurrentPath))
		GetProcAddress(m_hModule, "GoNPNCheck");

	OutputDebugString("[npnx]8");
	lpfnSetHwnd = NULL;
	lpfnSetHwnd = (void(*)(DWORD dwHWND))
		GetProcAddress(m_hModule, "SetHwnd");

	if ( (lpfnGoNPN == NULL) ||
		 (lpfnGoNPNEx == NULL) ||
		 (lpfnIsExecute == NULL) ||
		 (lpfnCloseNPN == NULL) ||
		 (lpfnCopyModule == NULL) ||
		 (lpfnSetHwnd == NULL))
	{
		OutputDebugString("Dll load is failed.");
		return;
	}
//실행

	OutputDebugString("[npnx]9");
	
	if(m_hWnd != NULL)
	{
		lpfnSetHwnd((DWORD)m_hWnd);
	}
	else
	{
		OutputDebugString("[NpnxMgr] HWND == NULL");

		if( m_dwhwnd != 0 )
			lpfnSetHwnd(m_dwhwnd);
	}

	OutputDebugString("[npnx]10");
	if(!m_strModulePath.IsEmpty())
	{
		if (lpfnCopyModule != NULL)
		{
			if(!m_strModulePath.IsEmpty())
			{
				if(lpfnCopyModule(m_strModulePath.GetBuffer(0)) != TRUE)
				{
					OutputDebugString("Copy Module Failed.");
				}
			}
			else
			{
				OutputDebugString(m_strModulePath);
			}
		}
	}

	OutputDebugString("[npnx]11");
	if(m_strCurrentPath.IsEmpty())
	{
		OutputDebugString("[npnx]12");
		if (!m_strCustomerID.IsEmpty())
		{
			if (lpfnGoNPN != NULL)
			{
				if (lpfnGoNPN(m_strCustomerID.GetBuffer(0)) != TRUE)			
					OutputDebugString("Execution of Netizen failed.");
			}
		}else
		{
			OutputDebugString("기업 아이디를 입력하여 주십시오");
		}
	}else
	{
		OutputDebugString("[npnx]13");
		if (!m_strCustomerID.IsEmpty())
		{
			if (lpfnGoNPNQuick != NULL)
			{
				OutputDebugString("path");
				OutputDebugString(m_strCustomerID);
				OutputDebugString(m_strCurrentPath);
				m_strCurrentPath += "\\";
				if (lpfnGoNPNQuick(m_strCustomerID.GetBuffer(0), m_strCurrentPath.GetBuffer(0)) != TRUE)
					OutputDebugString("Execution of Netizen failed.");
			}
		}else
		{
			AfxMessageBox("기업 아이디를 입력하여 주십시오");
		}
	}
}

void CNpnxMgr::Check(BOOL bOutMessage)
{
	m_bOutMessage = bOutMessage;
	if(!isAdmin())
		return;
	m_hModule = LoadLibrary("npnx.dll");

	if(m_hModule == NULL)
	{
		OutputDebugString("m_hModule == NULL");
	}else
	{
		OutputDebugString("m_hModule != NULL");
	}

	lpfnGoNPN = NULL;
	lpfnGoNPN = (BOOL(*)(char *szID))
		GetProcAddress(m_hModule, "GoNPN");
	
	lpfnGoNPNEx = NULL;
	lpfnGoNPNEx = (BOOL(*)(char *szID, char *szCurrentPath))
		GetProcAddress(m_hModule, "GoNPNEx");

	lpfnIsExecute = NULL;
	lpfnIsExecute = (BOOL(*)())
		GetProcAddress(m_hModule, "IsExecute");

	lpfnCloseNPN = NULL;
	lpfnCloseNPN = (BOOL(*)())
		GetProcAddress(m_hModule, "CloseNPN");

	lpfnCopyModule = NULL;
	lpfnCopyModule = (BOOL(*)(char *szCurrentPath))
		GetProcAddress(m_hModule, "CopyModule");

	lpfnGoNPNQuick = NULL;
	lpfnGoNPNQuick = (BOOL(*)(char *szID, char *szCurrentPath))
		GetProcAddress(m_hModule, "GoNPNQuick");

	lpfnGoNPNCheck = NULL;
	lpfnGoNPNCheck = (BOOL(*)(char *szID, char *szCurrentPath))
		GetProcAddress(m_hModule, "GoNPNCheck");

	lpfnSetHwnd = NULL;
	lpfnSetHwnd = (void(*)(DWORD dwHWND))
		GetProcAddress(m_hModule, "SetHwnd");

	if ( (lpfnGoNPN == NULL) ||
		 (lpfnGoNPNEx == NULL) ||
		 (lpfnIsExecute == NULL) ||
		 (lpfnCloseNPN == NULL) ||
		 (lpfnCopyModule == NULL) ||
		 (lpfnSetHwnd == NULL))
	{
		OutputDebugString("Dll load is failed.");
	}

//Dll 실행
	if(m_hWnd != NULL)
	{
		lpfnSetHwnd((DWORD)m_hWnd);
	}
	else
	{
		OutputDebugString("[execnpnx] HWND == NULL");
	}

	if(!m_strModulePath.IsEmpty())
	{
		if (lpfnCopyModule != NULL)
		{
			if(!m_strModulePath.IsEmpty())
			{
				if(lpfnCopyModule(m_strModulePath.GetBuffer(0)) != TRUE)
				{
					OutputDebugString("Copy Module Failed.");
				}
			}else
			{
				OutputDebugString(m_strModulePath);
			}
		}
	}


	if (!m_strCustomerID.IsEmpty())
	{
		if (lpfnGoNPNQuick != NULL)
		{
			OutputDebugString("path");
			OutputDebugString(m_strCustomerID);
			OutputDebugString(m_strCurrentPath);
			m_strCurrentPath += "\\";
			if (lpfnGoNPNCheck(m_strCustomerID.GetBuffer(0), m_strCurrentPath.GetBuffer(0)) != TRUE)			
				OutputDebugString("Execution of Netizen failed.");
		}
	}
}

#pragma warning (default : 26495)
#pragma warning (default : 6386)
#pragma warning (default : 6387)
#pragma warning (default : 6011)