#include "stdafx.h"
#include "whdump.h"

LPCTSTR MiniDumper::m_szAppName;
#pragma warning (disable : 26495)

static void WriteCrashSummaryIni(
    LPCTSTR dumpFilePath,
    struct _EXCEPTION_POINTERS* pExceptionInfo)
{
    if (dumpFilePath == NULL || pExceptionInfo == NULL)
        return;

    CString iniPath;
    iniPath.Format(_T("%s\\ping\\dump_analysis.ini"), Axis::home);

    // ping 폴더 없으면 생성
    CString pingDir;
    pingDir.Format(_T("%s\\ping"), Axis::home);
    ::CreateDirectory(pingDir, NULL);

    // 섹션명은 dump 파일명만 사용
    CString section = dumpFilePath;
    int pos = section.ReverseFind(_T('\\'));
    if (pos >= 0)
        section = section.Mid(pos + 1);

    SYSTEMTIME st;
    ::GetLocalTime(&st);

    CString value;
    value.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),
        st.wYear, st.wMonth, st.wDay,
        st.wHour, st.wMinute, st.wSecond);
    ::WritePrivateProfileString(section, _T("TIME"), value, iniPath);

    ::WritePrivateProfileString(section, _T("DUMPFILE"), dumpFilePath, iniPath);

    value.Format(_T("0x%08X"), pExceptionInfo->ExceptionRecord->ExceptionCode);
    ::WritePrivateProfileString(section, _T("EXCEPTION_CODE"), value, iniPath);

    value.Format(_T("0x%p"), pExceptionInfo->ExceptionRecord->ExceptionAddress);
    ::WritePrivateProfileString(section, _T("EXCEPTION_ADDR"), value, iniPath);

    value.Format(_T("%lu"), ::GetCurrentThreadId());
    ::WritePrivateProfileString(section, _T("THREAD_ID"), value, iniPath);

    value.Format(_T("%lu"), ::GetCurrentProcessId());
    ::WritePrivateProfileString(section, _T("PROCESS_ID"), value, iniPath);
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
//LONG MiniDumper::TopLevelFilter(struct _EXCEPTION_POINTERS* pExceptionInfo)
//{
//    LONG retval = EXCEPTION_CONTINUE_SEARCH;
//    const HWND hParent = NULL;      // find a better value for your app
//    CString slog;
//    slog.Format("[Axis_State][MiniDumper][TopLevelFilter] [%s]   Axis_state=[%d] \n", Axis::sFiller, Axis_State());
//    OutputDebugString(slog);
//    if (Axis::sFiller.Find("destroying") >= 0)
//    {
//        OutputDebugString("[axis][MiniDumper][TopLevelFilter] --------- bDestroying is true  return ");
//        return 0;
//    }
//
//    if(Axis_State() >= AXIS_STATE_ONCLOSE)
//    {
//        OutputDebugString("[axis][MiniDumper][TopLevelFilter] --------- AXIS_STATE_ONCLOSE is true  return ");
//        return 0;
//    }
//
//    // firstly see if dbghelp.dll is around and has the function we need
//    // look next to the EXE first, as the one in System32 might be old
//    // (e.g. Windows 2000)
//    HMODULE hDll = NULL;
//
//    hDll = ::LoadLibrary("DBGHELP.DLL");
//
//    LPCTSTR szResult = NULL;
//
//
//
//    if (hDll)
//    {
//        MINIDUMPWRITEDUMP pDump = (MINIDUMPWRITEDUMP)::GetProcAddress(hDll, "MiniDumpWriteDump");
//       
//        if (pDump)
//        {
//            COleDateTime oTime;
//            oTime = COleDateTime::GetCurrentTime();
//            CString strDay;
//            strDay.Format(_T("%d%d"), oTime.GetMonth(), oTime.GetDay());
//           
//            CString file;
//            file.Format("%s\\%s\\%s\\Crashlog", Axis::home, USRDIR, Axis::user);
//
//            CFileFind ff;
//            if (!ff.FindFile(file))
//                ::CreateDirectory(file, NULL);
//
//            CString strCurTimeLogfile;
//            strCurTimeLogfile.Format(_T("%s\\%s_%s%dh%dm%ds.dmp"), file, Axis::userID , strDay, oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());
//
//            char szScratch[_MAX_PATH]{};
//            HANDLE hFile = ::CreateFile(strCurTimeLogfile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
//                FILE_ATTRIBUTE_NORMAL, NULL);
//
//            if (hFile != INVALID_HANDLE_VALUE)
//            {
//                _MINIDUMP_EXCEPTION_INFORMATION ExInfo;
//
//                ExInfo.ThreadId = ::GetCurrentThreadId();
//                ExInfo.ExceptionPointers = pExceptionInfo;
//                ExInfo.ClientPointers = NULL;
//
//                // write the dump
//                const BOOL bOK = pDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL);
//                if (bOK)
//                {
//                    szResult = szScratch;
//                    retval = EXCEPTION_EXECUTE_HANDLER;
//                }
//                else
//                {
//                    sprintf_s(szScratch, _T("1 Failed to save dump file to '%s' (error %d)"), (LPSTR)(LPCTSTR)file, GetLastError());
//                    szResult = szScratch;
//                }
//                ::CloseHandle(hFile);
//            }
//            else
//            {
//     
//            }
//        }
//        else
//        {
//      
//        }
//    }
//    else
//    {
//        szResult = _T("DBGHELP.DLL not found");
//    }
//
//    if (szResult)
//    {
//      
//    }
//    return retval;
//}

LONG MiniDumper::TopLevelFilter(struct _EXCEPTION_POINTERS* pExceptionInfo)
{
    LONG retval = EXCEPTION_CONTINUE_SEARCH;
    CString slog;
    slog.Format("[Axis_State][MiniDumper][TopLevelFilter] [%s]   Axis_state=[%d] \n", Axis::sFiller, Axis_State());
    OutputDebugString(slog);

    if (Axis::sFiller.Find("destroying") >= 0)
    {
        OutputDebugString("[axis][MiniDumper][TopLevelFilter] --------- bDestroying is true  return ");
        return 0;
    }
    if (Axis_State() >= AXIS_STATE_ONCLOSE)
    {
        OutputDebugString("[axis][MiniDumper][TopLevelFilter] --------- AXIS_STATE_ONCLOSE is true  return ");
        return 0;
    }

    // exe 절대경로 기준으로 DbgHelp.dll 로드
    char exeDir[MAX_PATH] = { 0 };
    GetModuleFileNameA(NULL, exeDir, MAX_PATH);
    char* pSlash = strrchr(exeDir, '\\');
    if (pSlash) *pSlash = '\0';

    char dbgHelpPath[MAX_PATH] = { 0 };
    sprintf_s(dbgHelpPath, "%s\\DBGHELP.DLL", exeDir);

    HMODULE hDll = ::LoadLibraryA(dbgHelpPath);  // exe 옆 최신버전 우선
    if (!hDll)
    {
        OutputDebugString("[MiniDumper] exe 옆 DBGHELP.DLL 없음 - 시스템꺼 로드\n");
        hDll = ::LoadLibraryA("DBGHELP.DLL");
    }

    // 어떤 DLL 로드됐는지 로그
    if (hDll)
    {
        char loadedPath[MAX_PATH] = { 0 };
        GetModuleFileNameA(hDll, loadedPath, MAX_PATH);
        CString sDbgLog;
        sDbgLog.Format("[MiniDumper] DBGHELP.DLL 로드: [%s]\n", loadedPath);
        OutputDebugString(sDbgLog);
    }

    LPCTSTR szResult = NULL;
    char szScratch[_MAX_PATH] = { 0 };

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
            strCurTimeLogfile.Format(_T("%s\\%s_%s%dh%dm%ds.dmp"),
                file, Axis::userID, strDay,
                oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());

            HANDLE hFile = ::CreateFile(strCurTimeLogfile,
                GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
                CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

            if (hFile != INVALID_HANDLE_VALUE)
            {
                _MINIDUMP_EXCEPTION_INFORMATION ExInfo;
                ExInfo.ThreadId = ::GetCurrentThreadId();
                ExInfo.ExceptionPointers = pExceptionInfo;
                ExInfo.ClientPointers = NULL;

                // 덤프 타입 - 상세 정보 포함
                MINIDUMP_TYPE dumpType = (MINIDUMP_TYPE)(
                    MiniDumpNormal |
                    MiniDumpWithDataSegs |
                    MiniDumpWithHandleData |
                    MiniDumpWithThreadInfo |
                    MiniDumpWithUnloadedModules |
                    MiniDumpWithIndirectlyReferencedMemory | 
                    MiniDumpWithCodeSegs);    // x64 CALL

                const BOOL bOK = pDump(
                    GetCurrentProcess(), GetCurrentProcessId(),
                    hFile,
                    dumpType,
                    &ExInfo, NULL, NULL);

                if (bOK)
                {
                    // 파일 크기 확인
                    DWORD dwSize = ::GetFileSize(hFile, NULL);
                    sprintf_s(szScratch, "[MiniDumper] 덤프 성공 size=%lu bytes [%s]",
                        dwSize, (LPSTR)(LPCTSTR)strCurTimeLogfile);
                    OutputDebugString(szScratch);
                    szResult = szScratch;
                    retval = EXCEPTION_EXECUTE_HANDLER;
                }
                else
                {
                    DWORD err = GetLastError();
                    sprintf_s(szScratch, "[MiniDumper] 덤프 실패 err=%lu (0x%08X) path=[%s]",
                        err, err, (LPSTR)(LPCTSTR)strCurTimeLogfile);
                    OutputDebugString(szScratch);
                    szResult = szScratch;
                }

                ::CloseHandle(hFile);
            }
            else
            {
                DWORD err = GetLastError();
                sprintf_s(szScratch, "[MiniDumper] CreateFile 실패 err=%lu (0x%08X) path=[%s]",
                    err, err, (LPSTR)(LPCTSTR)strCurTimeLogfile);
                OutputDebugString(szScratch);
                szResult = szScratch;
            }
        }
        else
        {
            OutputDebugString("[MiniDumper] MiniDumpWriteDump GetProcAddress 실패\n");
        }

        ::FreeLibrary(hDll);
    }
    else
    {
        OutputDebugString("[MiniDumper] DBGHELP.DLL 로드 실패\n");
        szResult = _T("DBGHELP.DLL not found");
    }

    if (szResult)
    {
        OutputDebugString(szResult);
    }

    return retval;
}