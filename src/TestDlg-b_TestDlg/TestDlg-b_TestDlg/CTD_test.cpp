// CTD_test.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CTD_test.h"


// CTD_test

IMPLEMENT_DYNCREATE(CTD_test, CWinThread)

CTD_test::CTD_test()
	: m_role(TD_WRITER), m_pArr(nullptr), m_pLock(nullptr), m_bUseLock(false), m_bAlive(false)
{
}

CTD_test::~CTD_test()
{
}

BOOL CTD_test::InitInstance()
{
	m_slog.Format("[@][%s]", __FUNCTION__);
	m_data = 1;
	OutputDebugString(m_slog);
	return TRUE;
}

int CTD_test::ExitInstance()
{
	// TODO:  여기에서 각 스레드에 대한 정리를 수행합니다.
	m_slog.Format("[@][%s]", __FUNCTION__);
	OutputDebugString(m_slog);
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CTD_test, CWinThread)
END_MESSAGE_MAP()


// CTD_test 메시지 처리기


int CTD_test::Run()
{
	if (!m_pArr)
		return 0;

	while (m_bAlive)  // m_bAlive is set by StartRaceDemo before ResumeThread
	{
		if (m_role == TD_WRITER)
		{
			if (m_bUseLock && m_pLock) m_pLock->Lock();

			CString s;
			s.Format("item_%06d", m_data++);
			m_pArr->Add(s);
			if (m_pArr->GetSize() > 1000)
				m_pArr->RemoveAll();  // realloc triggers here

			if (m_bUseLock && m_pLock) m_pLock->Unlock();
		}
		else  // TD_READER
		{
			if (m_bUseLock && m_pLock) m_pLock->Lock();

			int n = m_pArr->GetSize();
			if (n > 0)
			{
				CString val = m_pArr->GetAt(n - 1);  // crash here if writer reallocated
				m_data++;
			}

			if (m_bUseLock && m_pLock) m_pLock->Unlock();
		}
		Sleep(0);  // yield CPU to maximize race probability
	}

	return 0;
}
