#include "stdafx.h"
#include "axis.h"
#include "cpuUse.h"
#include <atlbase.h>		// for CRegKey use
#include <Windows.h>

#pragma pack(push,8)
#include "perCounters.h"
#pragma pack(pop)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define SYSTEM_OBJECT_INDEX			2		// 'System' object
#define PROCESS_OBJECT_INDEX			230		// 'Process' object
#define PROCESSOR_OBJECT_INDEX			238		// 'Processor' object
#define TOTAL_PROCESSOR_TIME_COUNTER_INDEX	240		// '% Total processor time' counter (valid in WinNT under 'System' object)
#define PROCESSOR_TIME_COUNTER_INDEX		6		// '% processor time' counter (for Win2K/XP)

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


CCpuUse::CCpuUse()
{
	m_bFirstTime = true;
	m_lnOldValue = 0;
	memset(&m_OldPerfTime100nSec, 0, sizeof(m_OldPerfTime100nSec));
}

CCpuUse::~CCpuUse()
{
}

//=================================================================================================
//	GetCpuUsage returns the system-wide cpu usage.
//	Since we calculate the cpu usage by two samplings, the first
//	call to GetCpuUsage() returns 0 and keeps the values for the next
//	sampling.
//  Read the comment at the beginning of this file for the formula.
//=================================================================================================
int CCpuUse::GetCpuUsage()
{
	static PLATFORM Platform = GetPlatform();
	
	if (m_bFirstTime)
		enablePerformaceCounters();

	// Cpu usage counter is 8 byte length.
	CPerfCounters<LONGLONG> PerfCounters;
	char szInstance[256] = {0};

	//		On windows NT, cpu usage counter is '% Total processor time'
	//		under 'System' object. However, in Win2K/XP Microsoft moved
	//		that counter to '% processor time' under '_Total' instance
	//		of 'Processor' object.
	//		Read 'INFO: Percent Total Performance Counter Changes on Windows 2000'
	//		Q259390 in MSDN.

	DWORD dwObjectIndex{};
	DWORD dwCpuUsageIndex{};
	switch (Platform)
	{
	case WINNT:
		dwObjectIndex = SYSTEM_OBJECT_INDEX;
		dwCpuUsageIndex = TOTAL_PROCESSOR_TIME_COUNTER_INDEX;
		break;
	case WIN2K_XP:
	case WIN_VISTA:
		dwObjectIndex = PROCESSOR_OBJECT_INDEX;
		dwCpuUsageIndex = PROCESSOR_TIME_COUNTER_INDEX;
		strcpy_s(szInstance, 256, "_Total");
		break;
	default:
		dwObjectIndex = PROCESSOR_OBJECT_INDEX;
		dwCpuUsageIndex = PROCESSOR_TIME_COUNTER_INDEX;
		strcpy_s(szInstance, 256, "_Total");
		//return -1;
	}

	int			CpuUsage = 0;
	LONGLONG		lnNewValue = 0;
	PPERF_DATA_BLOCK	pPerfData = NULL;
	LARGE_INTEGER		NewPerfTime100nSec = {0};

	lnNewValue = PerfCounters.GetCounterValue(&pPerfData, dwObjectIndex, dwCpuUsageIndex, szInstance);
	NewPerfTime100nSec = pPerfData->PerfTime100nSec;

	if (m_bFirstTime)
	{
		m_bFirstTime = false;
		m_lnOldValue = lnNewValue;
		m_OldPerfTime100nSec = NewPerfTime100nSec;
		return 0;
	}

	const LONGLONG lnValueDelta = lnNewValue - m_lnOldValue;
	const double DeltaPerfTime100nSec = (double)NewPerfTime100nSec.QuadPart - (double)m_OldPerfTime100nSec.QuadPart;

	m_lnOldValue = lnNewValue;
	m_OldPerfTime100nSec = NewPerfTime100nSec;

	const double a = (double)lnValueDelta / DeltaPerfTime100nSec;

	const double f = (1.0 - a) * 100.0;
	CpuUsage = (int)(f + 0.5);	// rounding the result
	if (CpuUsage < 0)
		return 0;
	return CpuUsage;
}

int CCpuUse::GetCpuUsage(LPCTSTR pProcessName)
{
	static PLATFORM Platform = GetPlatform();
	
	if (m_bFirstTime)
		enablePerformaceCounters();

	// Cpu usage counter is 8 byte length.
	CPerfCounters<LONGLONG> PerfCounters;
	char szInstance[256] = {0};


	const DWORD dwObjectIndex = PROCESS_OBJECT_INDEX;
	const DWORD dwCpuUsageIndex = PROCESSOR_TIME_COUNTER_INDEX;
	strcpy_s(szInstance, 256, pProcessName);

	int		CpuUsage = 0;
	LONGLONG	lnNewValue = 0;
	PPERF_DATA_BLOCK pPerfData = NULL;
	LARGE_INTEGER	NewPerfTime100nSec = {0};

	lnNewValue = PerfCounters.GetCounterValue(&pPerfData, dwObjectIndex, dwCpuUsageIndex, szInstance);
	NewPerfTime100nSec = pPerfData->PerfTime100nSec;

	if (m_bFirstTime)
	{
		m_bFirstTime = false;
		m_lnOldValue = lnNewValue;
		m_OldPerfTime100nSec = NewPerfTime100nSec;
		return 0;
	}

	const LONGLONG lnValueDelta = lnNewValue - m_lnOldValue;
	const double DeltaPerfTime100nSec = (double)NewPerfTime100nSec.QuadPart - (double)m_OldPerfTime100nSec.QuadPart;

	m_lnOldValue = lnNewValue;
	m_OldPerfTime100nSec = NewPerfTime100nSec;

	const double a = (double)lnValueDelta / DeltaPerfTime100nSec;

	CpuUsage = (int) (a*100);
	if (CpuUsage < 0)
		return 0;
	return CpuUsage;
}


int CCpuUse::GetCpuUsage(DWORD dwProcessID)
{
	
	static PLATFORM Platform = GetPlatform();
	
	if (m_bFirstTime)
		enablePerformaceCounters();

	// Cpu usage counter is 8 byte length.
	CPerfCounters<LONGLONG> PerfCounters;


	const DWORD dwObjectIndex = PROCESS_OBJECT_INDEX;
	const DWORD dwCpuUsageIndex = PROCESSOR_TIME_COUNTER_INDEX;

	int		CpuUsage = 0;
	LONGLONG	lnNewValue = 0;
	PPERF_DATA_BLOCK pPerfData = NULL;
	LARGE_INTEGER	NewPerfTime100nSec = {0};

	lnNewValue = PerfCounters.GetCounterValueForProcessID(&pPerfData, dwObjectIndex, dwCpuUsageIndex, dwProcessID);
	NewPerfTime100nSec = pPerfData->PerfTime100nSec;

	if (m_bFirstTime)
	{
		m_bFirstTime = false;
		m_lnOldValue = lnNewValue;
		m_OldPerfTime100nSec = NewPerfTime100nSec;
		return 0;
	}

	const LONGLONG lnValueDelta = lnNewValue - m_lnOldValue;
	const double DeltaPerfTime100nSec = (double)NewPerfTime100nSec.QuadPart - (double)m_OldPerfTime100nSec.QuadPart;

	m_lnOldValue = lnNewValue;
	m_OldPerfTime100nSec = NewPerfTime100nSec;

	const double a = (double)lnValueDelta / DeltaPerfTime100nSec;

	CpuUsage = (int) (a*100);
	if (CpuUsage < 0)
		return 0;
	return CpuUsage;
}



PLATFORM CCpuUse::GetPlatform()
{
//	OSVERSIONINFO osvi;
	OSVERSIONINFOEX osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	//if (!GetVersionEx(&osvi))
	if (!GetVersion(&osvi))
		return UNKNOWN;

	switch (osvi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_WINDOWS:
		return WIN9X;
	case VER_PLATFORM_WIN32_NT:
		if (osvi.dwMajorVersion == 4)
			return WINNT;
		else if (osvi.dwMajorVersion == 6)
		{
			return WIN_VISTA;
		}
		else
		{
			return WIN2K_XP;
		}
	}
	return UNKNOWN;
}

BOOL CCpuUse::enablePerformaceCounters(BOOL bEnable)
{
	if (GetPlatform() != WIN2K_XP)
		return TRUE;

	CRegKey regKey;
	if (regKey.Open(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\PerfOS\\Performance") != ERROR_SUCCESS)
		return FALSE;

	regKey.SetValue(!bEnable, "Disable Performance Counters");
	regKey.Close();

	if (regKey.Open(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\PerfProc\\Performance") != ERROR_SUCCESS)
		return FALSE;

	regKey.SetValue(!bEnable, "Disable Performance Counters");
	regKey.Close();

	return TRUE;
}

