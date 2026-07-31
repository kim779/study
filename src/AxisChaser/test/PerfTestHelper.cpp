// PerfTestHelper.cpp
// Purpose: Helper functions for AxisChaser performance testing
// Usage: Inject into ChildView.cpp for testing (remove after testing)

#include <windows.h>
#include <stdio.h>

// Performance measurement utilities

class PerfCounter {
private:
    LARGE_INTEGER m_liStart;
    LARGE_INTEGER m_liFreq;

public:
    PerfCounter() {
        QueryPerformanceFrequency(&m_liFreq);
        QueryPerformanceCounter(&m_liStart);
    }

    long long ElapsedMs() const {
        LARGE_INTEGER liNow;
        QueryPerformanceCounter(&liNow);
        return (liNow.QuadPart - m_liStart.QuadPart) * 1000 / m_liFreq.QuadPart;
    }

    void Reset() {
        QueryPerformanceCounter(&m_liStart);
    }
};

// Inject into addTrace() to measure Truncate performance
void MeasureTruncatePerformance(CRichEditCtrlEx* pTrace) {
#ifdef _DEBUG
    static PerfCounter counter;

    int beforeLen = pTrace->GetTextLength();

    if (beforeLen > 50 * 1024 * 1024) {
        counter.Reset();

        int docLen = pTrace->GetTextLength();
        pTrace->SetSel(0, docLen / 2);
        pTrace->ReplaceSel("");

        long long deltaMs = counter.ElapsedMs();
        int afterLen = pTrace->GetTextLength();

        CString slog;
        slog.Format("[PERF][TRUNCATE] Before: %d bytes -> After: %d bytes, Elapsed: %lld ms\n",
            beforeLen, afterLen, deltaMs);
        OutputDebugString(slog);
    }
#endif
}

// Inject into OnReceive() to measure batch processing delay
class BatchPerfCounter {
private:
    LARGE_INTEGER m_liFreq;
    LARGE_INTEGER m_liLastCall;
    int m_callCount;

public:
    BatchPerfCounter() : m_callCount(0) {
        QueryPerformanceFrequency(&m_liFreq);
        QueryPerformanceCounter(&m_liLastCall);
    }

    void RecordCall() {
#ifdef _DEBUG
        m_callCount++;
        LARGE_INTEGER liNow;
        QueryPerformanceCounter(&liNow);
        long long deltaMs = (liNow.QuadPart - m_liLastCall.QuadPart) * 1000 / m_liFreq.QuadPart;

        if (deltaMs > 50) {  // Log if delay > 50ms
            CString slog;
            slog.Format("[PERF][BATCH] Call #%d, Delta: %lld ms\n", m_callCount, deltaMs);
            OutputDebugString(slog);
        }

        m_liLastCall = liNow;
#endif
    }
};

// Inject into CopyData() to monitor queue depth
void MonitorQueueDepth(int queueSize, DWORD dataType) {
#ifdef _DEBUG
    static int maxQueueSize = 0;

    if (queueSize > maxQueueSize) {
        maxQueueSize = queueSize;

        CString typeStr;
        switch (dataType) {
            case 0: typeStr = "RTM"; break;
            case 1: typeStr = "SND"; break;
            case 2: typeStr = "RCV"; break;
            default: typeStr = "OTHER"; break;
        }

        CString slog;
        slog.Format("[QUEUE] Type: %s, Size: %d (max: %d)\n", typeStr, queueSize, maxQueueSize);
        OutputDebugString(slog);
    }
#endif
}

// Inject into addTrace() to measure addTrace() itself
class AddTracePerfCounter {
private:
    LARGE_INTEGER m_liFreq;
    LARGE_INTEGER m_liStart;

public:
    AddTracePerfCounter() {
        QueryPerformanceFrequency(&m_liFreq);
        QueryPerformanceCounter(&m_liStart);
    }

    void LogElapsed(const char* context) const {
#ifdef _DEBUG
        LARGE_INTEGER liNow;
        QueryPerformanceCounter(&liNow);
        long long deltaMs = (liNow.QuadPart - m_liStart.QuadPart) * 1000 / m_liFreq.QuadPart;

        if (deltaMs > 10) {  // Log if > 10ms
            CString slog;
            slog.Format("[PERF][ADDTRACE] %s: %lld ms\n", context, deltaMs);
            OutputDebugString(slog);
        }
#endif
    }
};

// ============================================================
// Memory leak detection helper
// ============================================================

void CheckMemoryLeaks() {
#ifdef _DEBUG
    static DWORD lastVirtualSize = 0;

    HANDLE hProc = GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS pmc;

    if (GetProcessMemoryInfo(hProc, &pmc, sizeof(pmc))) {
        DWORD virtualSize = pmc.PagefileUsage;

        if (lastVirtualSize > 0) {
            long long delta = (long long)virtualSize - lastVirtualSize;
            if (delta > 1024 * 1024) {  // Alert if > 1MB increase
                CString slog;
                slog.Format("[MEM] Virtual size increased: %lld MB (now: %d MB)\n",
                    delta / (1024 * 1024), virtualSize / (1024 * 1024));
                OutputDebugString(slog);
            }
        }

        lastVirtualSize = virtualSize;
    }
#endif
}

// ============================================================
// Test data generator (for manual testing without AXIS)
// ============================================================

struct TestDataGenerator {
    static void GenerateRTMData(char* buffer, int bufferSize, int seqNo) {
        // Format: KEY\tFIELD1\tVALUE1\tFIELD2\tVALUE2\n
        snprintf(buffer, bufferSize,
            "TEST_RTM_%04d\t"
            "PRICE\t12345.50\t"
            "VOLUME\t1000000\t"
            "BID\t12345.00\t"
            "ASK\t12345.99\t"
            "TIME\t153045\n",
            seqNo);
    }

    static void GenerateSNDData(char* buffer, int bufferSize, int seqNo) {
        // Format: Binary hex dump (20 bytes per line)
        snprintf(buffer, bufferSize,
            "SND_%04d: 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F 50 51 52 53 54",
            seqNo);
    }

    static void GenerateRCVData(char* buffer, int bufferSize, int seqNo) {
        snprintf(buffer, bufferSize,
            "RCV_%04d: 55 56 57 58 59 5A 61 62 63 64 65 66 67 68 69 6A 6B 6C 6D 6E",
            seqNo);
    }
};

// ============================================================
// Injection points in ChildView.cpp
// ============================================================

/*
// In CChildView::addTrace() - line 781
LRESULT CChildView::addTrace(CString dat, int kind, CString boldSub) {
    // ... existing code ...

    // ADD THIS AT LINE ~803 (before Truncate)
    AddTracePerfCounter perfCounter;

    // ... existing code ...

    // ADD THIS AT LINE ~807 (after Truncate)
    MeasureTruncatePerformance(&m_trace);
    perfCounter.LogElapsed("addTrace");

    // ... rest of existing code ...
}

// In CChildView::CopyData() - line 1034
void CChildView::CopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) {
    // ... existing code line 1042-1047 ...

    // ADD THIS BEFORE QUEUE CHECK
    MonitorQueueDepth(m_que.GetSize(), flag);

    if ((flag == x_RTMs && m_que.GetSize() > 200) ||
        (flag == x_SNDs && m_que.GetSize() > 500) ||
        (flag == x_RCVs && m_que.GetSize() > 500))
    {
        // ADD THIS LOG
        CString slog;
        slog.Format("[QUEUE][DROP] Type: %d, QueueSize: %d\n", flag, m_que.GetSize());
        OutputDebugString(slog);
        return;
    }

    // ... rest of existing code ...
}

// In CChildView::OnReceive() - line 1065
LRESULT CChildView::OnReceive(WPARAM wParam, LPARAM lParam) {
    // ADD THIS AT START
    BatchPerfCounter batchPerf;

    const int MAX_PER_CALL = 20;
    int processed = 0;

    CQue* que;
    while (m_que.GetSize() && processed < MAX_PER_CALL) {
        // ADD THIS BEFORE PROCESSING
        batchPerf.RecordCall();

        m_sync.Lock();
        que = (CQue *) m_que.GetAt(0);
        m_que.RemoveAt(0);
        m_sync.Unlock();
        OnRCVData(MAKEWPARAM(que->m_nBytes, que->m_flag), (LPARAM) que->m_pBytes);
        delete que;
        processed++;
    }

    // ... rest of existing code ...

    // ADD THIS AT END
    CheckMemoryLeaks();

    return 0;
}
*/

// ============================================================
// Usage example: Create a test program to send data to AxisChaser
// ============================================================

/*
// Compile separately as TestSender.exe
#include <windows.h>
#include <stdio.h>
#include <thread>
#include <chrono>

struct _exeCDSS {
    DWORD flag;
    DWORD len;
};

void SendTestData(HWND hChaserWnd, DWORD dataType, int dataCount, int delayMs) {
    for (int i = 0; i < dataCount; i++) {
        char buffer[2048];
        _exeCDSS* cdss = (_exeCDSS*)buffer;
        cdss->flag = dataType;

        char* data = buffer + sizeof(_exeCDSS);

        // Generate different data based on type
        switch (dataType) {
            case 1: // RTM
                sprintf_s(data, 2000, "TEST_RTM_%04d\tPRICE\t12345.50\tVOLUME\t1000000\n", i);
                break;
            case 2: // SND
                sprintf_s(data, 2000, "SND_%04d: 41 42 43 44 45 46\n", i);
                break;
            case 3: // RCV
                sprintf_s(data, 2000, "RCV_%04d: 55 56 57 58 59 5A\n", i);
                break;
        }

        cdss->len = strlen(data) + 1;

        COPYDATASTRUCT cds;
        cds.dwData = 0;
        cds.cbData = sizeof(_exeCDSS) + cdss->len;
        cds.lpData = buffer;

        SendMessage(hChaserWnd, WM_COPYDATA, 0, (LPARAM)&cds);

        if (delayMs > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }
}

int main() {
    HWND hChaser = FindWindow(NULL, "AxisChaser");
    if (!hChaser) {
        printf("AxisChaser not found\n");
        return 1;
    }

    printf("Found AxisChaser, starting stress test...\n");

    // Spawn multiple threads to send data
    std::thread t1(SendTestData, hChaser, 1, 1000, 5);  // RTM 1000x with 5ms delay
    std::thread t2(SendTestData, hChaser, 2, 500, 10);   // SND 500x with 10ms delay
    std::thread t3(SendTestData, hChaser, 3, 500, 10);   // RCV 500x with 10ms delay

    t1.join();
    t2.join();
    t3.join();

    printf("Test complete\n");
    return 0;
}
*/
