// CWC_FileSync.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CWC_FileSync.h"


#define DF_LIMIT 5
// CWC_FileSync

IMPLEMENT_DYNAMIC(CWC_FileSync, CWnd)

CWC_FileSync::CWC_FileSync()
{

}

CWC_FileSync::~CWC_FileSync()
{
    DeleteCriticalSection(&csMapHandle);
}


BEGIN_MESSAGE_MAP(CWC_FileSync, CWnd)
    ON_WM_CREATE()
END_MESSAGE_MAP()



// CWC_FileSync 메시지 처리기


void CWC_FileSync::cs_Lock()
{
#ifdef DF_GLOVAL_CS
    EnterCriticalSection(&csMapHandle);
#else
    m_cs.Lock();
#endif
}

void CWC_FileSync::cs_Unlock()
{
#ifdef DF_GLOVAL_CS
    LeaveCriticalSection(&csMapHandle);
#else
      m_cs.Unlock();
#endif
}

UINT th_synWriteFile(LPVOID lparam)
{
    CWC_FileSync* cs_val = (CWC_FileSync*)lparam;
    CString _tlog;

_tlog.Format(_T("\r\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! [%d] !!!!!!!!!!!!!!!!!!!!!"), cs_val->m_que.GetCount());
OutputDebugString(_tlog);

    int _tval = 0;

  //  CSingleLock synclock(&cs_val->m_event);
    while (1)
    {
      //  synclock.Lock(1000);
       // synclock.Unlock();
     //   Sleep(1000);
        cs_val->cs_Lock();
        if (!cs_val->m_bUseing)
        {
            cs_val->cs_Unlock();
            cs_val->StartNextThread();
            break;
        }

_tlog.Format(_T("\r\n---> while que=[%d]<---"), cs_val->m_que.GetCount());
OutputDebugString(_tlog);

        _tval++;
        if (_tval > DF_LIMIT)
        {
            cs_val->m_bUseing = FALSE;
            cs_val->cs_Unlock();
            cs_val->StartNextThread();
            break;
        }
        Sleep(1000);
        cs_val->cs_Unlock();
    }
    return 0;
}

void CWC_FileSync::StartNextThread()
{
    if (1)
    {
        if (!m_que.IsEmpty())
        {
            void* nextParam = m_que.GetAt(0);
            m_que.RemoveAt(0);
            //uintptr_t hthread = _beginthreadex(0, 0, (_beginthreadex_proc_type)th_synWriteFile, nextParam, 0, 0);
            AfxBeginThread(th_synWriteFile, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
            m_bUseing = true;
        }
    }
    else
    {
        if (!m_thQueue.empty())
        {
            void* nextParam = m_thQueue.front();
            m_thQueue.pop();
            //uintptr_t hthread = _beginthreadex(0, 0, (_beginthreadex_proc_type)th_synWriteFile, nextParam, 0, 0);
            AfxBeginThread(th_synWriteFile, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
            m_bUseing = true;
        }
    }
 
}

BOOL CWC_FileSync::CheckThreadUsing()
{
    cs_Lock();
    if (m_bUseing)
    {
        m_thQueue.push((void*)this);
        cs_Unlock();
        return TRUE;
    }

    m_bUseing = TRUE;
    cs_Unlock();
    return FALSE;
}

void CWC_FileSync::synWritePrivateProfileString(CString sSec, CString sItem, CString sVal, CString sPath)
{
    // EnterCriticalSection(&g_CriticalSection);
    if (CheckThreadUsing())
        return;

    //m_event.SetEvent();
    _slog.Format(_T("\r\n--->que=[%d]<---"), m_que.GetCount());
    OutputDebugString(_slog);
     AfxBeginThread(th_synWriteFile, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
    //uintptr_t hthread = _beginthreadex(0, 0, (_beginthreadex_proc_type)th_synWriteFile, (void*)this, 0, 0);
}

int CWC_FileSync::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

#ifdef DF_GLOVAL_CS 
    InitializeCriticalSection(&csMapHandle);
#endif
    return 0;
}
