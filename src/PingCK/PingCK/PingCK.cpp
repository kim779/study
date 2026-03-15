// PingCK.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <windows.h>
#include <iphlpapi.h>
#include <stdio.h>
#pragma comment(lib, "iphlpapi.lib")

// 전역
char   g_logFile[MAX_PATH] = { 0 };
HANDLE g_hParent = NULL;
HANDLE g_hStopEvent = NULL;

// 로그 파일에 한 줄 기록
void WriteLog(const char* msg)
{
    FILE* fp = nullptr;
    fopen_s(&fp, g_logFile, "a");
    if (fp)
    {
        fputs(msg, fp);
        fclose(fp);
    }
}


// 종료 체크 (부모 죽음 or StopEvent)
bool ShouldStop()
{
    // 1. 부모 프로세스 죽었는지
    if (g_hParent)
    {
        if (WaitForSingleObject(g_hParent, 0) == WAIT_OBJECT_0)
            return true;
    }

    // 2. 부모가 정상 종료 신호 보냈는지
    if (g_hStopEvent)
    {
        if (WaitForSingleObject(g_hStopEvent, 0) == WAIT_OBJECT_0)
            return true;
    }

    return false;
}

void DebugLog(const char* fmt, ...)
{
    char msg[1024] = { 0 };
    va_list args;
    va_start(args, fmt);
    vsprintf_s(msg, sizeof(msg), fmt, args);
    va_end(args);

    char buf[1024] = { 0 };
    sprintf_s(buf, sizeof(buf), "[PingLog] %s", msg);
    OutputDebugStringA(buf);
}

// ping 스레드
DWORD WINAPI PingThread(LPVOID)
{
    SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };
    HANDLE hReadPipe, hWritePipe;
    CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
    SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA si = { sizeof(si) };
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = hWritePipe;
    si.hStdError = hWritePipe;

    PROCESS_INFORMATION pi = { 0 };
    char cmd[] = "ping 8.8.8.8 -t";
    CreateProcessA(NULL, cmd, NULL, NULL, TRUE,
        CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

    CloseHandle(hWritePipe);

    char buf[1024] = { 0 };
    char line[1024] = { 0 };
    int  linePos = 0;
    DWORD bytesRead;

    while (ReadFile(hReadPipe, buf, sizeof(buf) - 1, &bytesRead, NULL)
        && bytesRead > 0)
    {
        buf[bytesRead] = '\0';

        for (DWORD i = 0; i < bytesRead; i++)
        {
            char c = buf[i];
            if (c == '\r') continue;

            if (c == '\n')
            {
                line[linePos] = '\0';
                linePos = 0;
                if (strlen(line) == 0) continue;

                // 타임스탬프 붙여 로그 기록
                SYSTEMTIME st;
                GetLocalTime(&st);

                char output[2048] = { 0 };
                sprintf_s(output, sizeof(output),
                    "[%04d-%02d-%02d %02d:%02d:%02d] %s\n",
                    st.wYear, st.wMonth, st.wDay,
                    st.wHour, st.wMinute, st.wSecond,
                    line);

                WriteLog(output);

                // 종료 체크
                if (ShouldStop())
                {
                    WriteLog("[종료] 종료 신호 감지 - 자체 종료\n");
                    TerminateProcess(pi.hProcess, 0);
                    goto CLEANUP;
                }
            }
            else
            {
                if (linePos < (int)sizeof(line) - 1)
                    line[linePos++] = c;
            }
        }
    }

CLEANUP:
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hReadPipe);
    return 0;
}

int main(int argc, char* argv[])
{
    // 부모 PID 확인
    if (argc < 2) return 1;
    DebugLog(argv[1]);
    DWORD parentPid = (DWORD)atoi(argv[1]);

    // 부모 프로세스 핸들 획득
    g_hParent = OpenProcess(SYNCHRONIZE, FALSE, parentPid);
    if (!g_hParent) return 1;

    // Named Event 열기 (부모가 CreateEvent로 만든 것)
    char eventName[64] = { 0 };
    sprintf_s(eventName, "PingStop_%lu", parentPid);
    g_hStopEvent = OpenEvent(SYNCHRONIZE, FALSE, eventName);
    // StopEvent는 없어도 동작 (부모 죽음으로도 종료 가능)

    DebugLog(eventName);

    // 로그 파일 경로 설정
    SYSTEMTIME st;
    GetLocalTime(&st);

    // 본인 실행파일 경로
    char selfPath[MAX_PATH] = { 0 };
    GetModuleFileNameA(NULL, selfPath, MAX_PATH);

    // 폴더 추출
    char folder[MAX_PATH] = { 0 };
    strcpy_s(folder, selfPath);
    char* lastSlash = strrchr(folder, '\\');
    if (lastSlash) *lastSlash = '\0';

    // ping 폴더 경로
    char pingFolder[MAX_PATH] = { 0 };
    sprintf_s(pingFolder, "%s\\ping", folder);

    // ping 폴더 없으면 생성
    if (GetFileAttributesA(pingFolder) == INVALID_FILE_ATTRIBUTES)
    {
        CreateDirectoryA(pingFolder, NULL);
    }

    // 로그 파일 경로
    sprintf_s(g_logFile, MAX_PATH, "%s\\pinglog_%04d%02d%02d.txt",
        pingFolder, st.wYear, st.wMonth, st.wDay);

    // 네트워크 타입 로그
    char netLog[256] = { 0 };
    WriteLog(netLog);

    // ping 스레드 시작
    HANDLE hThread = CreateThread(NULL, 0, PingThread, NULL, 0, NULL);

    // 스레드 종료 대기
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    // 정리
    if (g_hStopEvent) CloseHandle(g_hStopEvent);
    if (g_hParent)    CloseHandle(g_hParent);

    return 0;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
