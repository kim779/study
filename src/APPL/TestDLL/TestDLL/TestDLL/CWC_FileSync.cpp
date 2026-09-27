// CWC_FileSync.cpp: 구현 파일
//

#include "pch.h"
#include "CWC_FileSync.h"


#define DF_LIMIT 5
// CWC_FileSync

IMPLEMENT_DYNAMIC(CWC_FileSync, CWnd)

CMQue::CMQue()
{
 
}

CMQue::~CMQue()
{
    OutputDebugString("\r\n--------------------------------[filesync] CMQue 소멸자(destructor)---------------------------------------\r\n");
}

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
    CString _tlog, stmp, stemp;
    CStringW wstrSec, wstrItem, wstrVal, wstrFile;

_tlog.Format(_T("\r\n[filesync]  th_synWriteFile start  cs_val = [%p]   m_que cnt = [%d] !!!!!!!!!!!!!!!!!!!!!"), cs_val, cs_val->m_que.GetCount());
OutputDebugString(_tlog);

    int _tval = 0;

  //  CSingleLock synclock(&cs_val->m_event);
    while (1)
    {
        cs_val->cs_Lock();
        if (!cs_val->m_bUseing)
        {
            cs_val->cs_Unlock();
            cs_val->StartNextThread();
            break;
        }

     //   _tval++;
        //CMQue* pque = (CMQue * )cs_val->m_que.GetAt(0);
        CMQue* pque = (CMQue*)(cs_val->m_que.GetAt(0));
_tlog.Format(_T("\r\n[filesync]--->cs_val = [%p]   this in que =[%p]  file=[%s]<---"), cs_val, pque->m_pParentWnd, pque->m_strFile);
OutputDebugString(_tlog);
_tlog.Format("%d", cs_val->m_que.GetCount());
stmp.Format("%lu", GetTickCount64());
stemp.Format("%p", cs_val);
        WritePrivateProfileString(stmp, stemp, _tlog, pque->m_strFile);
       
       
       // if (_tval > 1)
        {
            cs_val->m_que.RemoveAt(0);
            cs_val->m_bUseing = FALSE;
            cs_val->cs_Unlock();

            _tlog.Format(_T("\r\n[filesync]---> 큐 삭제 cs_val = [%p]   this in que cnd=[%d]<---"), cs_val, cs_val->m_que.GetCount());
            OutputDebugString(_tlog);

            _tlog.Format("%d", cs_val->m_que.GetCount());
            stmp.Format("%lu", GetTickCount64());
            stemp.Format("delete = [%p]", cs_val);

            cs_val->convert_ansi_to_unicode(wstrSec, (LPCSTR)(LPCTSTR)stmp, stmp.GetLength());
            cs_val->convert_ansi_to_unicode(wstrItem, (LPCSTR)(LPCTSTR)stemp, stemp.GetLength());
            cs_val->convert_ansi_to_unicode(wstrVal, (LPCSTR)(LPCTSTR)_tlog, _tlog.GetLength());
            cs_val->convert_ansi_to_unicode(wstrFile, (LPCSTR)(LPCTSTR)pque->m_strFile, pque->m_strFile.GetLength());

            WritePrivateProfileStringW(wstrSec, wstrItem, wstrVal, wstrFile);

            cs_val->StartNextThread();
            break;
        }
        cs_val->cs_Unlock();
        Sleep(1000);
    }
    return 0;
}

void CWC_FileSync::StartNextThread()
{
    if (!m_que.IsEmpty())
    {
        //void* nextParam = m_que.GetAt(0);
        CMQue* pque = (CMQue*)m_que.GetAt(0);
    
        uintptr_t hthread = _beginthreadex(0, 0, (_beginthreadex_proc_type)th_synWriteFile, pque->m_pParentWnd, 0, 0);
        //AfxBeginThread(th_synWriteFile, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
        cs_Lock();
        m_bUseing = true;
        cs_Unlock();
    }
}

BOOL CWC_FileSync::CheckThreadUsing()
{
_slog.Format(_T("\r\n[filesync]--->[%s]  que=[%d] m_bUseing=[%d]<---"), __FUNCTION__,  m_que.GetCount(), m_bUseing);
OutputDebugString(_slog);

    cs_Lock();
    if (m_bUseing)
    {
       // m_thQueue.push((void*)this);
        std::unique_ptr<CMQue> que = std::make_unique<CMQue>();
        que.get()->m_pParentWnd = this;
        que.get()->m_strFile = m_strFile;
        que.get()->m_strSec = m_strSec;
        que.get()->m_strItem = m_strItem;
        que.get()->m_strVal = m_strVal;
        m_que.Add((CObject*)que.release());
        cs_Unlock();
        return TRUE;
    }

    std::unique_ptr<CMQue> que = std::make_unique<CMQue>();
    que.get()->m_pParentWnd = this;
    que.get()->m_strFile = m_strFile;
    que.get()->m_strSec = m_strSec;
    que.get()->m_strItem = m_strItem;
    que.get()->m_strVal = m_strVal;
    m_que.Add((CObject*)que.release());

    m_bUseing = TRUE;
    cs_Unlock();
    return FALSE;
}

void CWC_FileSync::synWritePrivateProfileString(CString sSec, CString sItem, CString sVal, CString sPath)
{
    // EnterCriticalSection(&g_CriticalSection);
    _slog.Format(_T("\r\n[filesync]--->클릭을 했다   que cnt =[%d]     m_bUseing=[%d]<---"), m_que.GetCount(), m_bUseing);
    m_strFile = sPath;
    m_strSec = sSec;
    m_strItem = sItem;
    m_strVal = sVal;
    OutputDebugString(_slog);

    if (CheckThreadUsing())
        return;

    //m_event.SetEvent();
     //AfxBeginThread(th_synWriteFile, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
    uintptr_t hthread = _beginthreadex(0, 0, (_beginthreadex_proc_type)th_synWriteFile, (void*)this, 0, 0);
}

int CWC_FileSync::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  여기에 특수화된 작성 코드를 추가합니다.
#ifdef DF_GLOVAL_CS 
    InitializeCriticalSection(&csMapHandle);
#endif
    return 0;
}

DWORD CWC_FileSync::convert_ansi_to_unicode(CStringW& swUnicode, const char* ansi, const int isize)
{
    if(1)
    {
        CStringW str1;
        str1 = CA2W(ansi, CP_ACP);
        swUnicode = str1;
    }
    else
    {
        DWORD dError = 0;
        wstring wstr;
        do
        {
            if ((nullptr == ansi) || (0 == isize))
            {
                dError = ERROR_INVALID_PARAMETER;
                break;
            }

            wstr.clear();

            //메모리 확보
            int ilen = MultiByteToWideChar(CP_ACP, 0, ansi, static_cast<int>(isize), nullptr, 0);
            if (ilen == 0)
            {
                dError = GetLastError();
                break;
            }

            wstr.resize(ilen);
            //변환
            if (0 == MultiByteToWideChar(CP_ACP, 0, ansi, static_cast<int>(isize), const_cast<wchar_t*>(wstr.c_str()), static_cast<int>(wstr.size())))
            {
                dError = GetLastError();
                break;
            }
            else
            {
                swUnicode = wstr.c_str();
            }
        } while (false);

        return dError;
    }
}