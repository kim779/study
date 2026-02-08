#if !defined(AFX_CPUUSE_H__62616DB7_C90F_46DE_B8D6_03F52E925F2E__INCLUDED_)
#define AFX_CPUUSE_H__62616DB7_C90F_46DE_B8D6_03F52E925F2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

#define MAX_RAW_VALUES          20

const char szCounterName[] = "\\Processor(_Total)\\% Processor Time";

typedef enum
{
	WINNT, WIN2K_XP, WIN9X, WIN_VISTA, UNKNOWN
} PLATFORM;

class CCpuUse  
{
public:
	CCpuUse();
	virtual ~CCpuUse();

public:
	int GetCpuUsage();
	int GetCpuUsage(LPCTSTR pProcessName);
	int GetCpuUsage(DWORD dwProcessID);
	PLATFORM	GetPlatform();

private:
	BOOL		enablePerformaceCounters(BOOL bEnable = TRUE);
	

private:
	bool		m_bFirstTime;
	LONGLONG	m_lnOldValue ;
	LARGE_INTEGER	m_OldPerfTime100nSec;
};

#endif // !defined(AFX_CPUUSE_H__62616DB7_C90F_46DE_B8D6_03F52E925F2E__INCLUDED_)
