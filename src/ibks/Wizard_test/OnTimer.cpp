// OnTimer.cpp : implementation file
//

#include "pch.h"
#include "Wizard.h"
#include "OnTimer.h"

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

		DoParse(key);
	}
	return 0;
}

void COnTimer::Dispatch(int key)
{
	if (!m_alive)
		return;

	int	value;

	m_section.Lock();
	if (!m_keys.Lookup(key, value))
	{
		m_que.Add(key);
		m_keys.SetAt(key, key);
	}
	m_section.Unlock();
	m_event.SetEvent();
}

void COnTimer::Startup(CWnd* view)
{
	m_view  = view;
	m_alive = true;
}

void COnTimer::Cleanup(bool exit)
{
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
		m_view->SendMessage(WM_USER+11, key);
	return true;
}
