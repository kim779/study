#if !defined(AFX_SYSINFO_H__3E1D4AEC_8665_47D1_B4E4_6365D1BE273D__INCLUDED_)
#define AFX_SYSINFO_H__3E1D4AEC_8665_47D1_B4E4_6365D1BE273D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#define W95_s	"Windows 95"
#define W98_s	"Windows 98"
#define WME_s	"Windows ME"
#define NT4_s	"Windows NT 4.0"
#define WKS_s	" Workstation"
#define WXP_s	"Windows XP"
#define WVISTA_s "Windows Vista"
#define XHE_s	" Home Edition"
#define XPR_s	" Professional Edition"
#define W2K_s	"Windows 2000"
#define PRO_s	" Professional"
#define DOM_s	" Domain Controller"
#define SER_s	" Server"
#define ADV_s	" Advanced"
#define DTC_s	" DataCenter"
#define TER_s	" Terminal"
#define BOF_s	" BackOffice"
#define W80_s	"Windows 8.0"
#define W81_s	"Windows 8.1"
#define W10_s	"Windows 10"

typedef struct _MYOSVERSIONINFOEXA 
{
	DWORD dwOSVersionInfoSize;
	DWORD dwMajorVersion;
	DWORD dwMinorVersion;
	DWORD dwBuildNumber;
	DWORD dwPlatformId;
	TCHAR szCSDVersion[128];
	WORD  wServicePackMajor;
	WORD  wServicePackMinor;
	WORD  wSuiteMask;
	BYTE  wProductType;
	BYTE  wReserved;
} MYOSVERSIONINFOEXA, *MYPOSVERSIONINFOEXA, *MYLPOSVERSIONINFOEXA;

typedef MYOSVERSIONINFOEXA MYOSVERSIONINFOEX;
typedef MYPOSVERSIONINFOEXA MYPOSVERSIONINFOEX;
typedef MYLPOSVERSIONINFOEXA MYLPOSVERSIONINFOEX;

typedef struct _MYMEMORYSTATUSEX 
{
	DWORD dwLength; 
	DWORD dwMemoryLoad; 
	DWORDLONG ullTotalPhys; 
	DWORDLONG ullAvailPhys; 
	DWORDLONG ullTotalPageFile; 
	DWORDLONG ullAvailPageFile; 
	DWORDLONG ullTotalVirtual; 
	DWORDLONG ullAvailVirtual; 
	DWORDLONG ullAvailExtendedVirtual;
} MYMEMORYSTATUSEX, *LPMYMEMORYSTATUSEX;
typedef BOOL (WINAPI *pGlobalMemoryStatusEx)(LPMYMEMORYSTATUSEX lpBuffer);

#define MY_VER_SERVER_NT					0x80000000
#define MY_VER_WORKSTATION_NT					0x40000000
#define MY_VER_SUITE_SMALLBUSINESS				0x00000001
#define MY_VER_SUITE_ENTERPRISE					0x00000002
#define MY_VER_SUITE_BACKOFFICE					0x00000004
#define MY_VER_SUITE_COMMUNICATIONS				0x00000008
#define MY_VER_SUITE_TERMINAL					0x00000010
#define MY_VER_SUITE_SMALLBUSINESS_RESTRICTED			0x00000020
#define MY_VER_SUITE_EMBEDDEDNT					0x00000040
#define MY_VER_SUITE_DATACENTER					0x00000080
#define MY_VER_SUITE_SINGLEUSERTS				0x00000100
#define MY_VER_SUITE_PERSONAL					0x00000200
#define MY_VER_SUITE_BLADE					0x00000400

#define MY_VER_NT_WORKSTATION					0x0000001
#define MY_VER_NT_DOMAIN_CONTROLLER				0x0000002
#define MY_VER_NT_SERVER					0x0000003


#define CPU_CONFIGURATION_KEY "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"

//=================================================================================================
class CSysInfo  
{
public:
	CSysInfo();
	virtual ~CSysInfo();

public:
	CString GetIEInfo();
	void ieVersion();
	void	Init();

	CString	GetWindowInfo();
	CString	GetCPUInfo();
	CString	GetMemoryInfo();
	CString GetWindowVersion();
	
private:
	void	osType();
	void	cpuType();
	void	memoryType();
	BOOL	GetWinVer(DWORD &dwMajor,DWORD &dwMinor);

	BOOL	isWindowsNT() { return m_bWindowsNT; };

private:
	BOOL	m_bWindowsNT;
	DWORD	m_dwWinMajor;
	DWORD	m_dwWinMinor;
	DWORD	m_dwBuildNumber;
	DWORD	m_dwServicePack;
	CString	m_sOSType;
	CString m_sWinVer;

	CString m_sCPUIdentifier;
	CString m_sCPUVendorIdentifier;
	CString	m_sCPUNameString;
	DWORD	m_dwCPUSpeed;
		
	pGlobalMemoryStatusEx m_pGlobalMemoryStatusEx;
	DWORD	m_dwMemoryLoad;
	DWORD64 m_dwAvailVirtual;
	DWORD64 m_dwTotalVirtual;
	DWORD64 m_dwAvailPageFile;
	DWORD64 m_dwTotalPageFile;
	DWORD64 m_dwAvailPhys;
	DWORD64 m_dwTotalPhys;

	CString	m_sIEVersion;
};

#endif // !defined(AFX_SYSINFO_H__3E1D4AEC_8665_47D1_B4E4_6365D1BE273D__INCLUDED_)
