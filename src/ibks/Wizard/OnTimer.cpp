// OnTimer.cpp : implementation file
//

#include "pch.h"
#include "Wizard.h"
#include "OnTimer.h"

#include "../h/axlog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COnTimer

IMPLEMENT_DYNCREATE(COnTimer, CWinThread)

COnTimer::COnTimer()
{
	m_alive = false;
}

COnTimer::~COnTimer()
{
	Cleanup(true);
}

BOOL COnTimer::InitInstance()
{
	return TRUE;
}

int COnTimer::ExitInstance()
{
	Cleanup(true);
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(COnTimer, CWinThread)
	//{{AFX_MSG_MAP(COnTimer)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COnTimer message handlers

int COnTimer::Run()
{
	int	key;
	CSingleLock	syncLock(&m_event);

	axlog(LOG_EVENT, "COnTimer::Run thread started tid=%d", (int)GetCurrentThreadId());

	while (m_alive)
	{
		if (m_que.GetUpperBound() < 0)
		{
			syncLock.Lock(3000);
			syncLock.Unlock();
			continue;
		}
		m_section.Lock();
		key = (int)m_que.GetAt(0);
		m_que.RemoveAt(0);
		m_keys.RemoveKey(key);
		m_section.Unlock();

		axlog(LOG_EVENT, "COnTimer::Run dequeued key=%d tid=%d", key, (int)GetCurrentThreadId());
		DoParse(key);
	}

	axlog(LOG_EVENT, "COnTimer::Run thread exiting tid=%d", (int)GetCurrentThreadId());
	return 0;
}

void COnTimer::Dispatch(int key)
{
	if (!m_alive)
	{
		axlog(LOG_EVENT, "COnTimer::Dispatch key=%d ignored (not alive) tid=%d", key, (int)GetCurrentThreadId());
		return;
	}

	int	value;
	bool	dup;

	m_section.Lock();
	dup = m_keys.Lookup(key, value) ? true : false;
	if (!dup)
	{
		m_que.Add(key);
		m_keys.SetAt(key, key);
	}
	m_section.Unlock();
	axlog(LOG_EVENT, "COnTimer::Dispatch key=%d dup=%d queLen=%d tid=%d", key, dup, (int)m_que.GetSize(), (int)GetCurrentThreadId());
	m_event.SetEvent();
}

void COnTimer::Startup(CWnd* view)
{
	axlog(LOG_INIT, "COnTimer::Startup view=%p tid=%d", view, (int)GetCurrentThreadId());
	m_view  = view;
	m_alive = true;
}

void COnTimer::Cleanup(bool exit)
{
	axlog(LOG_INIT, "COnTimer::Cleanup exit=%d queLen=%d tid=%d", exit, (int)m_que.GetSize(), (int)GetCurrentThreadId());

	if (exit)
		m_alive = false;

	m_section.Lock();
	m_que.RemoveAll();
	m_keys.RemoveAll();
	m_section.Unlock();
	if (!exit)
		m_event.SetEvent();
}

bool COnTimer::DoParse(int key)
{
	if (m_alive)
	{
		ULONGLONG t0 = GetTickCount64();
		m_view->SendMessage(WM_USER+11, key);
		axlog(LOG_EVENT, "COnTimer::DoParse key=%d SendMessage +%dms tid=%d", key, (int)(GetTickCount64()-t0), (int)GetCurrentThreadId());
	}
	return true;
}
