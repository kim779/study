// CProducer_TD.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CProducer_TD.h"


#include "CThreadWnd.h"

// CProducer_TD
//쓰레드를 돌면서 다른 소비자 쓰레드(CWsh)에게 값을 넘긴다

IMPLEMENT_DYNCREATE(CProducer_TD, CWinThread)

CProducer_TD::CProducer_TD()
{
}

CProducer_TD::~CProducer_TD()
{
	m_alive = false;
	CString str;
	str.Format("\r\n[test][%d] [%s] 스레드클래스의 소멸자 ~CProducer_TD ", m_id, __FUNCTION__ );
	OutputDebugString(str);
	m_Event_end.SetEvent();
}

BOOL CProducer_TD::InitInstance()
{
	m_alive = true;
	m_Event_end.ResetEvent();
	
	
	return TRUE;
}

int CProducer_TD::ExitInstance()
{
	for (int ii = m_que.GetUpperBound(); ii >= 0; ii--)
		delete m_que.GetAt(ii);
	m_que.RemoveAll();

	CString str;
	str.Format("\r\n[test][%d][%s] 스레드 클래스의 ExitInstance", m_id,__FUNCTION__);
	OutputDebugString(str);
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CProducer_TD, CWinThread)
END_MESSAGE_MAP()


// CProducer_TD 메시지 처리기
void CProducer_TD::endThread()  
{
	CString str;
	m_alive = false;

	str.Format("\r\n[test][%d][%s]", m_id,__FUNCTION__);
	OutputDebugString(str);
}

int CProducer_TD::Run()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CMQue* pMQ;
	CSingleLock	syncLock(&m_event);
	CString str;
	
	while (m_alive)
	{
		if (m_que.GetUpperBound() <= 0)  //싱글락이 걸려있어도 m_que는 증가하는가..
		{
			//syncLock.Lock(3000);  //잠금을 얻을때까지 대기하겠다는 것이다.  여기서 이거 안하면 프로그램 닫을때 죽는다
			//syncLock.Unlock();
			//CString ss;
			//ss.Format("lock%d", GetTickCount());
			//((CThreadWnd*)m_pWnd)->MsgFromThread(ss);
			continue;
		}
		
	//m_section.Lock();
		pMQ = (CMQue*)m_que.GetAt(0);

		m_que.RemoveAt(0);
		str.Format("%s", pMQ->m_pBytes);
		((CThreadWnd*)m_pWnd)->MsgFromThread(str.Right(10));
		
		

		if (((CThreadWnd*)m_pWnd)->m_icnt != DF_MAXCNT - 1)
		{ 
			str.Format("[test][%d][%s]1 스레드 클래스 in threadnot DF_MAXCNT [%d] m_que=[%d]", m_id,__FUNCTION__,((CThreadWnd*)m_pWnd)->m_icnt, m_que.GetCount());
			OutputDebugString(str);
		}
		else
		{
			str.Format("[test][%d][%s]1 스레드 클래스 in thread m_icnt [%d] m_que=[%d]", m_id, __FUNCTION__,((CThreadWnd*)m_pWnd)->m_icnt, m_que.GetCount());
			OutputDebugString(str);
		}
	 //	m_section.Unlock();
		delete pMQ;  //지워주기 때문에 메모리릭이 안난다.
		
	//	m_event.SetEvent();

		str.Format("[test][%d][%s]2 스레드 클래스 in thread m_icnt [%d] m_que=[%d]", m_id,__FUNCTION__, ((CThreadWnd*)m_pWnd)->m_icnt, m_que.GetCount());
		OutputDebugString(str);
	/*	if (m_que.GetCount() > 100)
			Sleep(10);
		else if (m_que.GetCount() > 50)
			Sleep(50);
		else
			Sleep(75);*/
	}


	str.Format("\r\n[test][%d][%s] --- 스레드 클래스의 Run에서 리턴_ return success ", m_id,__FUNCTION__ );
	OutputDebugString(str);

	return 0;
}

void CProducer_TD::Dispatch(int ssm, char* pBytes, int nBytes)
{
	//m_section.Lock();
	CMQue* mque = new CMQue();
	mque->m_pBytes = new char[nBytes + 1];
	ZeroMemory(mque->m_pBytes, nBytes + 1);
	CopyMemory(mque->m_pBytes, pBytes, nBytes);
	mque->m_ssm = ssm;
	mque->m_nBytes = nBytes;
	
	CString str;

	

	str.Format("\r\n[test][%d][%s] size=[%d]", m_id,__FUNCTION__, m_que.GetCount());
	OutputDebugString(str);


	m_que.Add(mque);
	//delete mque;
//	m_section.Unlock();
//	test_stopcode();
	
	str.Format("\r\n[test][%d][%s] size=[%d]", m_id,__FUNCTION__,  m_que.GetCount());
	OutputDebugString(str);
}

void CProducer_TD::test_stopcode()
{
	CString str;

	str.Format("\r\n[test][%d][%s] size=[%d]", m_id, __FUNCTION__, m_que.GetCount());
	OutputDebugString(str);
	m_2section.Lock();
	/*HANDLE ehandle = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	switch (WaitForSingleObject(ehandle, INFINITE))
	{
	case WAIT_OBJECT_0:
	{
		str.Format("\r\n[test][%s] **********************   WAIT_OBJECT_0  *********************", __FUNCTION__);
		OutputDebugString(str);
	}
	break;
	case WAIT_FAILED:
	case WAIT_ABANDONED:
	{
		str.Format("\r\n[test][%s]     WAIT_FAILED    WAIT_ABANDONED error=[%d]", __FUNCTION__, GetLastError());
		OutputDebugString(str);
	}
	break;
	case WAIT_TIMEOUT:
	{
		str.Format("\r\n[test][%s]    CDlg_Thread의  WAIT_TIMEOUT", __FUNCTION__);
		OutputDebugString(str);
	}
	break;
	default:
	{
		str.Format("\r\n[test][%s] CDlg_Thread의  default", __FUNCTION__);
		OutputDebugString(str);
	}
	break;
	}*/
}