#include "stdafx.h"
#include "axis.h"
#include "sysInfo.h"
//#include <process.h>

//#pragma comment(lib,"Ws2_32.lib")
//#pragma comment(lib,"Version.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static BOOL GetVersion(OSVERSIONINFOEX* os) {
	HMODULE hMod{};
	RtlGetVersion_FUNC func{};

	OSVERSIONINFOEXW o{};
	OSVERSIONINFOEXW* osw = &o;

	hMod = LoadLibrary(TEXT("ntdll.dll"));
	if (hMod)
	{
		func = (RtlGetVersion_FUNC)GetProcAddress(hMod, "RtlGetVersion");
		if (func == 0)
		{
			FreeLibrary(hMod);
			return FALSE;
		}
		ZeroMemory(osw, sizeof(*osw));
		osw->dwOSVersionInfoSize = sizeof(*osw);
		func(osw);

		os->dwBuildNumber = osw->dwBuildNumber;
		os->dwMajorVersion = osw->dwMajorVersion;
		os->dwMinorVersion = osw->dwMinorVersion;
		os->dwPlatformId = osw->dwPlatformId;
		os->dwOSVersionInfoSize = sizeof(*os);
	}
	else
		return FALSE;
	FreeLibrary(hMod);
	return  TRUE;
}

CSysInfo::CSysInfo()
{
	OSVERSIONINFOEX osvi;
	GetVersion(&osvi);
	/*
	if (GetVersion() < 0x80000000)
		m_bWindowsNT = TRUE;
	else
		m_bWindowsNT = FALSE;
  */

	if (osvi.dwMajorVersion == 4)
		m_bWindowsNT = TRUE;
	else 
		m_bWindowsNT = FALSE;

	m_dwServicePack = 0;
	m_dwWinMajor = 0;
	m_dwWinMinor = 0;
	m_dwBuildNumber = 0;
	m_dwServicePack = 0;
	m_sOSType = "";

	m_sCPUIdentifier = "";
	m_sCPUVendorIdentifier = "";
	m_sCPUNameString = "";
	m_dwCPUSpeed = 0;

	m_pGlobalMemoryStatusEx = NULL;
	m_dwMemoryLoad = 0;
	m_dwAvailVirtual = 0;
	m_dwTotalVirtual = 0;
	m_dwAvailPageFile = 0;
	m_dwTotalPageFile = 0;
	m_dwAvailPhys = 0;
	m_dwTotalPhys = 0;

	m_sIEVersion.Empty();
}

CSysInfo::~CSysInfo()
{

}

void CSysInfo::Init()
{
	osType();	cpuType();	memoryType();	ieVersion();
}

CString CSysInfo::GetWindowInfo()
{
	return m_sOSType;
}

CString CSysInfo::GetCPUInfo()
{
	return m_sCPUNameString;
}

CString CSysInfo::GetMemoryInfo()
{
	CString tmpS;
	tmpS.Format("%dMB RAM",(int)(m_dwTotalPhys/1048576));
	return tmpS;
}

CString CSysInfo::GetIEInfo()
{
	return m_sIEVersion;
}

void CSysInfo::osType()
{
// 	BOOL bOSINFOEXWorking = FALSE;
// 
// 	OSVERSIONINFO osinfo;
// 	ZeroMemory(&osinfo, sizeof(OSVERSIONINFO));
// 	osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
// 	GetVersionEx((LPOSVERSIONINFO)&osinfo);
// 
// 	m_dwWinMajor = osinfo.dwMajorVersion;
// 	m_dwWinMinor = osinfo.dwMinorVersion;
// 	m_dwBuildNumber = osinfo.dwBuildNumber;
// 
// 	if (!isWindowsNT())
// 	{
// 		// Windows 9x/ME.
// 		if (m_dwWinMinor==0)  { m_sOSType=_T(W95_s); };
// 		if (m_dwWinMinor==10) { m_sOSType=_T(W98_s); };
// 		if (m_dwWinMinor==90) { m_sOSType=_T(WME_s); };
// 		m_sOSType += osinfo.szCSDVersion;
// 		return;
// 	};
// 
// 	// Get Service Pack number
// 	int pack = -1; 
// 	CString sSvcPack = osinfo.szCSDVersion;
// 	TCHAR szNum[10];
// 	for (int nn = 1; nn <= 9; nn++)
// 	{
// 		_stprintf(szNum, _T("%d"), nn);
// 		pack = sSvcPack.Find(szNum);
// 		if (pack >= 0) 
// 			break;
// 	};
// 	if (pack >= 0)
// 		m_dwServicePack = nn;
// 
// 	// Check for OSVERSIONINFOEX-compatibility
// 	// It works only on Windows NT 4.0 Service Pack 6 or better
// 	if ((m_dwWinMajor >= 5) || (m_dwServicePack >= 6))
// 	{
// 		bOSINFOEXWorking=TRUE;
// 	};
// 
// 	if (m_dwWinMajor == 4)
// 	{
// 		m_sOSType = _T(NT4_s);				// Windows NT 4.0
// 	}
// 	else if (m_dwWinMajor==5)
// 	{
// 		if (m_dwWinMinor == 0) { m_sOSType = _T(W2K_s); };  // Windows 2000
// 		if (m_dwWinMinor == 1) { m_sOSType = _T(WXP_s); };  // Windows XP
// 	}
// 	else if (m_dwWinMajor == 6)
// 	{
// 		m_sOSType = _T(WVISTA_s);				// vista
// 	}
// 
// 	if (bOSINFOEXWorking)
// 	{
// 		MYOSVERSIONINFOEX osvi;
// 		BOOL bOsVersionInfoEx;
// 		ZeroMemory(&osvi, sizeof(MYOSVERSIONINFOEX));
// 		osvi.dwOSVersionInfoSize = sizeof(MYOSVERSIONINFOEX);
// 		bOsVersionInfoEx=GetVersionEx((LPOSVERSIONINFO)&osvi);
// 
// 		if (bOsVersionInfoEx)
// 		{
// 			if (osvi.wProductType == MY_VER_NT_WORKSTATION)
// 			{
// 				if ((osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion == 1))
// 				{
// 					// Windows XP
// 					if (osvi.wSuiteMask & MY_VER_SUITE_PERSONAL)
// 						{ m_sOSType += _T(XHE_s); }
// 					else
// 						{ m_sOSType+=_T(XPR_s); };
// 				};
// 
// 				if ((osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion == 0))
// 					m_sOSType += _T(PRO_s);		// Windows 2000 Professional
// 				if (osvi.dwMajorVersion == 4)
// 					m_sOSType += _T(WKS_s);		// Windows NT 4.0 Workstation
// 			};
// 
// 			if ((osvi.wProductType==MY_VER_NT_SERVER) || (osvi.wProductType==MY_VER_NT_DOMAIN_CONTROLLER))
// 			{
// 				if (osvi.dwMajorVersion==5)
// 				{
// 					if (osvi.wSuiteMask & MY_VER_SUITE_ENTERPRISE) { m_sOSType += _T(ADV_s); };
// 					if (osvi.wSuiteMask & MY_VER_SUITE_DATACENTER) { m_sOSType += _T(DTC_s); };
// 					if (osvi.wSuiteMask & MY_VER_SUITE_TERMINAL) { m_sOSType += _T(TER_s); };
// 					if (osvi.wSuiteMask & MY_VER_SUITE_BACKOFFICE) { m_sOSType += _T(BOF_s); };
// 					m_sOSType += _T(SER_s);
// 				};
// 				if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0) { m_sOSType += _T(SER_s); };
// 			};
// 
// 			if (osvi.wProductType==MY_VER_NT_DOMAIN_CONTROLLER)
// 				{ m_sOSType += _T(DOM_s); };
// 		};
// 	};
// 
// 	if (m_dwServicePack > 0)
// 	{
// 		m_sOSType += _T(" ");
// 		m_sOSType += osinfo.szCSDVersion;
// 	};
	LONG lResult{};
	HKEY hKey{};
	TCHAR keyValue[5]; 
	memset(keyValue, 0, sizeof(keyValue));
	DWORD dwType = REG_SZ; 
	DWORD dwSize = sizeof(keyValue);

	lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), 0, KEY_QUERY_VALUE, &hKey);

	if(lResult != ERROR_SUCCESS)
	{
		m_sOSType = _T("NOTFOUND");
		return;
	}

	lResult = RegQueryValueEx(hKey, _T("CurrentVersion"), 0, &dwType, (LPBYTE)keyValue, &dwSize);
	if(lResult == ERROR_SUCCESS && dwSize > 0)
	{
		m_sOSType = keyValue;

		m_sWinVer = keyValue;

		if(m_sOSType == "5.0")
		{
			m_sOSType = _T(W2K_s);
		}
		else if(m_sOSType == "5.1")
		{
			m_sOSType = _T(WXP_s);
		}
		else if(m_sOSType == "5.2")
		{
			m_sOSType = _T("WINDOWS SERVER 2003");
		}
		else if(m_sOSType == "6.0")
		{
			m_sOSType = _T(WVISTA_s);
		}
		else if(m_sOSType == "6.1")
		{
			m_sOSType = _T("WINDOWS 7");
		}
		else if(m_sOSType == "6.2")
		{
			m_sOSType = _T(W80_s);
		}
		else if(m_sOSType == "6.3")
		{
			m_sOSType = _T(W81_s);

			dwType = REG_DWORD;
			DWORD dwData{};
			dwSize = sizeof(dwData);

			lResult = RegQueryValueEx(hKey, _T("CurrentMajorVersionNumber"), 0, &dwType, (LPBYTE)(&dwData), &dwSize);
			
			if(lResult == ERROR_SUCCESS && dwSize > 0)
			{				
				if(dwData == 10)
				{
					m_sOSType = _T("WINDOWS 10");

					m_sWinVer = "10.0";
				}
			}
		}
		else if(m_sOSType == "10.0")
		{
			m_sOSType = _T("WINDOWS 10");
		}
	}

	RegCloseKey(hKey);
}

void CSysInfo::cpuType()
{
	LONG	res;
	HKEY	newKey;

	res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T(CPU_CONFIGURATION_KEY), 0, KEY_EXECUTE, &newKey);
	if (ERROR_SUCCESS != res) 
		return;
	
	TCHAR keyValue[100]; 
	memset(keyValue, 0, sizeof(keyValue));
	DWORD dwType = REG_SZ; 
	DWORD dwSize = sizeof(keyValue);

	res = RegQueryValueEx(newKey, _T("Identifier"), NULL, &dwType, (LPBYTE)keyValue, &dwSize);
	if ((res == ERROR_SUCCESS) && (dwSize > 0)) 
	{
		m_sCPUIdentifier = keyValue; 
	};

	memset(keyValue, 0, sizeof(keyValue));
	dwType = REG_SZ;
	dwSize = sizeof(keyValue);

	res = RegQueryValueEx(newKey, _T("VendorIdentifier"), NULL, &dwType, (LPBYTE)keyValue, &dwSize);
	if ((res == ERROR_SUCCESS) && (dwSize > 0))
	{
		m_sCPUVendorIdentifier = keyValue;
	};

	memset(keyValue,0,100); dwType=REG_SZ; dwSize=100;

	res = RegQueryValueEx(newKey, _T("ProcessorNameString"), NULL, &dwType, (LPBYTE)keyValue,& dwSize);
	if ((res == ERROR_SUCCESS) && (dwSize > 0))
	{
		m_sCPUNameString = keyValue;
	};

	DWORD dwData=0;
	dwType = REG_DWORD;
	dwSize = sizeof(dwData);
	res = RegQueryValueEx(newKey, _T("~MHz"), NULL, &dwType, (LPBYTE)(&dwData), &dwSize);
	if ((res == ERROR_SUCCESS) && (dwSize > 0))
	{
		m_dwCPUSpeed = dwData;
	};
	RegCloseKey(newKey);
}

void CSysInfo::memoryType()
{
	BOOL bSuccess = FALSE;
	if (isWindowsNT() & (m_dwWinMajor >= 5))
	{
		// Windows 2000+. Use GlobalMemoryStatusEx to detect memory size more than 4GB
		MYMEMORYSTATUSEX MemStatEx;

		MemStatEx.dwLength = 0;
		MemStatEx.dwMemoryLoad = 0;
		MemStatEx.ullTotalPhys = 0;
		MemStatEx.ullAvailPhys = 0;
		MemStatEx.ullTotalPageFile = 0;
		MemStatEx.ullAvailPageFile = 0;
		MemStatEx.ullTotalVirtual = 0;
		MemStatEx.ullAvailVirtual = 0;
		MemStatEx.dwLength = sizeof(MemStatEx);

		HMODULE hmKernelDll{};
		m_pGlobalMemoryStatusEx = NULL;
		if ((hmKernelDll = ::LoadLibrary(_T("kernel32.dll"))) != NULL)
		{
			m_pGlobalMemoryStatusEx = (pGlobalMemoryStatusEx)::GetProcAddress(hmKernelDll,"GlobalMemoryStatusEx");
			if (m_pGlobalMemoryStatusEx!=NULL)
			{
				if ((*m_pGlobalMemoryStatusEx)(&MemStatEx))
				{
					m_dwMemoryLoad = MemStatEx.dwMemoryLoad;
					m_dwTotalPhys = MemStatEx.ullTotalPhys;
					m_dwAvailPhys = MemStatEx.ullAvailPhys;
					m_dwTotalPageFile = MemStatEx.ullTotalPageFile;
					m_dwAvailPageFile = MemStatEx.ullAvailPageFile;
					m_dwTotalVirtual = MemStatEx.ullTotalVirtual;
					m_dwAvailVirtual = MemStatEx.ullAvailVirtual;
					bSuccess = TRUE;
				};
			};
			::FreeLibrary(hmKernelDll);
		};
	}

	if (bSuccess) return;

	MEMORYSTATUS MemStat;

	MemStat.dwMemoryLoad = 0;
	MemStat.dwTotalPhys = 0;
	MemStat.dwAvailPhys = 0;
	MemStat.dwTotalPageFile = 0;
	MemStat.dwAvailPageFile = 0;
	MemStat.dwTotalVirtual = 0;
	MemStat.dwAvailVirtual = 0;
	MemStat.dwLength = sizeof(MemStat);

	GlobalMemoryStatus(&MemStat);
	m_dwMemoryLoad = MemStat.dwMemoryLoad;
	m_dwTotalPhys = MemStat.dwTotalPhys;
	m_dwAvailPhys = MemStat.dwAvailPhys;
	m_dwTotalPageFile = MemStat.dwTotalPageFile;
	m_dwAvailPageFile = MemStat.dwAvailPageFile;
	m_dwTotalVirtual = MemStat.dwTotalVirtual;
	m_dwAvailVirtual = MemStat.dwAvailVirtual;

}

void CSysInfo::ieVersion()
{
	CString sValue;
	HKEY hKey{};
	DWORD dwDisp{};
	long	lResult{};
	DWORD size{}, type{};
	
	RegCreateKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Internet Explorer", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisp);
	
	size = 1024;
	type = REG_SZ;
	lResult = RegQueryValueEx(hKey, "version", NULL, &type, (unsigned char*)sValue.GetBuffer(1024), &size);
	
	sValue.ReleaseBuffer();
	if (lResult == ERROR_SUCCESS)
	{
		m_sIEVersion = sValue;
		sValue.Empty();
	}
	RegCloseKey(hKey);
}

#define WIN_MAJOR_MAX		10
#define WIN_MAJOR_MIN		5

BOOL CSysInfo::GetWinVer(DWORD &dwMajor,DWORD &dwMinor)
{
	DWORD dwTypeMask{};
	DWORDLONG dwlConditionMask = 0;
	OSVERSIONINFOEX ovi{};

	dwMajor = 0;
	dwMinor = 0;

	for(int mjr = WIN_MAJOR_MAX; mjr >= WIN_MAJOR_MIN ; mjr--)
	{
		for(int mnr = 0; mnr <= 3 ; mnr++)
		{
			memset(&ovi, 0, sizeof(OSVERSIONINFOEX));
			ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

			ovi.dwMajorVersion = mjr;
			ovi.dwMinorVersion = mnr;

			dwlConditionMask = 0;
			VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_EQUAL);
			VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_EQUAL);

			dwTypeMask = VER_MAJORVERSION | VER_MINORVERSION;

			if(VerifyVersionInfo(&ovi, dwTypeMask, dwlConditionMask) == TRUE)
			{
				dwMajor = mjr;
				dwMinor = mnr;
				break;
			}
		}
	}

	return TRUE;
}

CString CSysInfo::GetWindowVersion()
{
	return m_sWinVer;
}

