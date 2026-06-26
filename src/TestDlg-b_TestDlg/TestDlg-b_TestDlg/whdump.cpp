#include "pch.h"
#include "whdump.h"

LPCTSTR MiniDumper::m_szAppName;
#pragma warning (disable : 26495)

CString GetDumpFolderPath()
{
    TCHAR szPath[MAX_PATH] = { 0 };
    // 실행 파일 경로 얻기
    GetModuleFileName(NULL, szPath, MAX_PATH);
    PathRemoveFileSpec(szPath); // 실행 파일 이름 제거 → 폴더만 남음

    // dump 폴더 붙이기
    PathAppend(szPath, _T("dump"));

    // 폴더 없으면 생성
    if (!PathFileExists(szPath))
    {
        ::CreateDirectory(szPath, NULL);
    }

    return szPath;
}

MiniDumper::MiniDumper(LPCTSTR szAppName)
{
    m_szAppName = _T("Application");
    CString slog;
    slog.Format("[axis][MiniDumper][MiniDumper] szAppName[%s]\n", szAppName);
    OutputDebugString(slog);
    ::SetUnhandledExceptionFilter(TopLevelFilter);
}
#pragma warning (default : 26495)
LONG MiniDumper::TopLevelFilter(struct _EXCEPTION_POINTERS* pExceptionInfo)
{
    LONG retval = EXCEPTION_CONTINUE_SEARCH;
    const HWND hParent = NULL;      // find a better value for your app
    
    // firstly see if dbghelp.dll is around and has the function we need
    // look next to the EXE first, as the one in System32 might be old
    // (e.g. Windows 2000)
    HMODULE hDll = NULL;

    hDll = ::LoadLibrary("DBGHELP.DLL");

    LPCTSTR szResult = NULL;



    if (hDll)
    {
        MINIDUMPWRITEDUMP pDump = (MINIDUMPWRITEDUMP)::GetProcAddress(hDll, "MiniDumpWriteDump");
       
        if (pDump)
        {
            COleDateTime oTime;
            oTime = COleDateTime::GetCurrentTime();
            CString strDay;
            strDay.Format(_T("%d%d"), oTime.GetMonth(), oTime.GetDay());
           
            CString file;
            file = GetDumpFolderPath();
            AfxMessageBox(file);
            CString strCurTimeLogfile;
            strCurTimeLogfile.Format(_T("%s_%s%dh%dm%ds.dmp"), file,  strDay, oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());

            char szScratch[_MAX_PATH]{};
            HANDLE hFile = ::CreateFile(strCurTimeLogfile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL, NULL);

            if (hFile != INVALID_HANDLE_VALUE)
            {
                _MINIDUMP_EXCEPTION_INFORMATION ExInfo;

                ExInfo.ThreadId = ::GetCurrentThreadId();
                ExInfo.ExceptionPointers = pExceptionInfo;
                ExInfo.ClientPointers = NULL;

                // write the dump
                MINIDUMP_TYPE dumpType = (MINIDUMP_TYPE)(
                    MiniDumpNormal |
                    MiniDumpWithDataSegs |
                    MiniDumpWithHandleData |
                    MiniDumpWithThreadInfo |
                    MiniDumpWithUnloadedModules |

                    MiniDumpWithCodeSegs);    // x64 CALL
                const BOOL bOK = pDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, dumpType, &ExInfo, NULL, NULL);
                if (bOK)
                {
                    szResult = szScratch;
                    retval = EXCEPTION_EXECUTE_HANDLER;
                }
                else
                {
                    sprintf_s(szScratch, _T("1 Failed to save dump file to '%s' (error %d)"), (LPSTR)(LPCTSTR)file, GetLastError());
                    szResult = szScratch;
                }
                ::CloseHandle(hFile);
            }
            else
            {
     
            }
        }
        else
        {
      
        }
    }
    else
    {
        szResult = _T("DBGHELP.DLL not found");
    }

    if (szResult)
    {
      
    }
    return retval;
}