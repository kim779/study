#include "stdafx.h"
#include "whdump.h"

LPCTSTR MiniDumper::m_szAppName;
#pragma warning (disable : 26495)
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
    CString slog;
    slog.Format("[axis][MiniDumper][TopLevelFilter] [%s]\n", Axis::sFiller);
    OutputDebugString(slog);
    if (Axis::sFiller.Find("destroying") >= 0)
    {
        OutputDebugString("[axis][MiniDumper][TopLevelFilter] --------- bDestroying is true  return ");
        return 0;
    }
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
            file.Format("%s\\%s\\%s\\Crashlog", Axis::home, USRDIR, Axis::user);

            CFileFind ff;
            if (!ff.FindFile(file))
                ::CreateDirectory(file, NULL);

            CString strCurTimeLogfile;
            strCurTimeLogfile.Format(_T("%s\\%s_%s%dh%dm%ds.dmp"), file, Axis::userID , strDay, oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());

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
                const BOOL bOK = pDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL);
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