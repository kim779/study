
// ThreadpoolWork Class : 헤더 파일
//
// callback function type : void NTAPI taskHandler(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_WORK Work)
//
#pragma once

#include <WinBase.h>

const	LONG	maxWITEM = 10;

class CThpool
{
public:
	CThpool()
	{
		for (int ii = 0; ii < maxWITEM; ii++)
			m_pWorkItem[ii] = NULL;

		m_pCallbackEnv = NULL;
		m_pPoolEnv = NULL;

		m_errorNo = 0;

		SYSTEM_INFO sysInfo; GetSystemInfo(&sysInfo);
		m_coreCount = sysInfo.dwNumberOfProcessors;
		m_threadCount = m_coreCount;	// default thread# = core#
	};
	~CThpool()
	{
		exitThreadEnv();
	};

private:
	TP_WORK*		m_pWorkItem[maxWITEM];
	TP_CALLBACK_ENVIRON*	m_pCallbackEnv;
	TP_POOL*		m_pPoolEnv;

	LONG			m_coreCount;
	LONG			m_threadCount;
	DWORD			m_errorNo;

public:
	LONG	getCoreCount()	{ return m_coreCount; }
	LONG	getThreadCount(){ return m_threadCount; }
	DWORD	getErrorNumber(){ return m_errorNo; }

	bool	initThreadEnv(int threadNum = -1)
	{
		if (m_pCallbackEnv != NULL) return true;

		m_pCallbackEnv = (TP_CALLBACK_ENVIRON*) new char[sizeof(TP_CALLBACK_ENVIRON)];

		InitializeThreadpoolEnvironment(m_pCallbackEnv);

		m_pPoolEnv = CreateThreadpool(NULL);
		if (m_pPoolEnv == NULL)
		{
			m_errorNo = GetLastError();
			delete [] m_pCallbackEnv; m_pCallbackEnv = NULL;
			return false;
		}

		if (threadNum != -1) m_threadCount = threadNum;
		SetThreadpoolThreadMinimum(m_pPoolEnv, m_threadCount);
		SetThreadpoolThreadMaximum(m_pPoolEnv, m_threadCount);

		SetThreadpoolCallbackPool(m_pCallbackEnv, m_pPoolEnv);
		return true;
	};

	bool	exitThreadEnv()
	{
		if (m_pPoolEnv != NULL)
		{
			CloseThreadpool(m_pPoolEnv);
			m_pPoolEnv = NULL;
		}

		if (m_pCallbackEnv != NULL)
		{
			DestroyThreadpoolEnvironment(m_pCallbackEnv);
			delete [] m_pCallbackEnv;
			m_pCallbackEnv = NULL;
		}

		return true;
	};

	int	createThreadWork(PTP_WORK_CALLBACK pfnwk, VOID* context)
	{
		int idx = 0;
		for (; idx < maxWITEM; idx++)
		{
			if (m_pWorkItem[idx] == NULL)
				break;
		}

		if (idx >= maxWITEM) return -1;

		m_pWorkItem[idx] = CreateThreadpoolWork(pfnwk, context, m_pCallbackEnv);
		if (m_pWorkItem[idx] == NULL)
			return -1;
		return idx;
	}

	inline bool runThreadWork(int idx)
	{
		if (idx < 0  || idx > maxWITEM)
			return false;
		if (m_pWorkItem[idx] == NULL)
			return false;

		SubmitThreadpoolWork(m_pWorkItem[idx]);
		return true;

	}

	bool	closeThreadWork(int idx)
	{
		if (0 < idx || idx > maxWITEM)
			return false;
		if (m_pWorkItem[idx] == NULL)
			return false;

		CloseThreadpoolWork(m_pWorkItem[idx]);
		m_pWorkItem[idx] = NULL;

		return true;
	}
};