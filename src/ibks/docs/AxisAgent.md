# AxisAgent 개선 과제

## 목차

- [1. 개요](#1-개요)
- [2. 과제 목록](#2-과제-목록)
- [3. 상세](#3-상세)
  - [A-12. MonitorLoop 스레드 수 집계가 매초 시스템 전체를 스냅샷함](#a-12-monitorloop-스레드-수-집계가-매초-시스템-전체를-스냅샷함)
- [4. 변경 이력](#4-변경-이력)

---

## 1. 개요

AxisAgent는 HTS(axis.exe)의 크래시와 행을 감시해 덤프를 뜨는 별도 exe다. `CMainFrame::CreateAgentProcess`가 실행한다.

- 소스: `D:\src\IBKS\src\AxisAgent\AxisAgent\AxisAgentDlg.cpp`
- 핵심 흐름: `MonitorLoop` → `CreateDump` → `AnalyzeDump`
- 덤프 분석 방법과 사례: [WinDbg.md](WinDbg.md)

이 문서는 정리·보완할 항목을 모아 둔다. 수정은 항목별로 코드 제안을 먼저 검토한 뒤 반영한다.

## 2. 과제 목록

| # | 항목 | 분류 | 상태 |
|---|---|---|---|
| A-1 | 시작 단계 행 감지: 로그인 창이 뜨기 전에 멈추면 감시자가 없다. 실행 시점을 앞당기고 "시작 후 N초 안에 로그인 창 없음 → 덤프"를 검토 | 기능 공백 | 미착수 |
| A-2 | 이벤트 이름 불일치 (`AgentEnd_` vs `PingStop_`) | 결함 | 미착수 |
| A-3 | `AGENT_LOG_MONITOR` 매크로 불일치 (`pch.h`가 `AGENT_LOG_MONITOR1`이라 모니터 로그가 파일/DebugView에 안 남음) | 결함 | 완료 (2026-11-05) |
| A-4 | 덤프 재시도 무한 루프 | 결함 | 코드상 해소 (2026-11-06 확인): `MAX_DUMP_FAIL=3`, `m_dumpFailCount`는 성공 시에만 0으로 돌아가고 `[D8]`에서는 리셋되지 않음 → 3회 연속 실패 후 그 세션은 덤프 중단. 실패 재현 테스트는 안 함 |
| A-5 | `m_allLogs` 잠금 없이 접근 | 결함 | 완료 (2026-11-06): `AddLog`는 `m_logQueue`(mutex)에 넣고 `m_bLogPosted`로 `WM_AGENT_LOG`를 1회만 Post(실패 시 플래그 복구), `OnAgentLog`(UI)가 swap 후 `m_allLogs`/리스트 반영. 워커 스레드의 `m_chkStop.GetCheck()` 교차 SendMessage 제거. 중지 해제 시 `OnBnClickedChkStop` → `RefreshLogList` 추가. 실행 확인(PING/MONITOR, 덤프분석 로그, 중지 해제 후 키인 로그 표시) |
| A-6 | `m_bShow`일 때 `ShouldStop`이 항상 false | 결함 | 미착수 |
| A-7 | `AnalyzeDump` 컨텍스트 선택: 크래시 덤프인데도 ThreadList 컨텍스트(덤프 작성 시점, whdump 핸들러 내부)를 써서 `TOP=NtGetContextThread`, CALLER 공란. cdb `.ecxr` 기준 실제 지점은 `cx_shared+0x1824`(`h12589_11520h54m51s.dmp`) | 결함 | 완료 (2026-11-06): 예외 스레드면 `MINIDUMP_EXCEPTION_STREAM::ThreadContext` 사용, `[AN6] context=exception record` 로그. 재분석 확인: `[TOP] CMainWnd::OnCreate+0x134 (cx_shared CMainWnd.cpp:114)` → mfc140 → user32 → `axCreate+0x10F` → `CMainFrame::initShared+0x472 (MainFrm.cpp:32813)`까지 풀림. user32 프레임은 PDB가 없어 가까운 export 이름(`AddClipboardFormatListener`, `GetDC` 등)으로 표시됨 |
| A-8 | `CL_ENC.cpp` AES 키 하드코딩 | 보안 | 미착수 |
| A-9 | 테스트 코드 잔존 (dumptest / BtnTest / Decacc) | 정리 | 미착수 |
| A-10 | 시작할 때마다 COM 등록 | 정리 | 미착수 |
| A-11 | `MainFrm.h`의 `m_hAgentProcess` / `m_dwAgentPid` / `m_pingMs` 초기화 안 됨. `{}`로 초기화해야 `CloseAgent`의 `CloseHandle`이 쓰레기 값을 쓰지 않음 | 결함 (axis 쪽) | 해소 (2026-11-06 확인): 현재 사용 분기(`DF_MAIN_RTS` 꺼짐)의 선언은 `HWND m_hAgentWnd{}`, `DWORD m_dwAgentPid{}`, `HANDLE m_hAgentProcess{}`, `int m_pingMs{}`로 초기화됨. `#ifdef DF_MAIN_RTS` 블록 안에 초기화 없는 중복 선언이 남아 있어 그 매크로를 켜면 중복 멤버로 빌드 오류 |
| A-12 | `MonitorLoop` 스레드 수 집계가 매초 시스템 전체 스레드를 스냅샷함 | 성능 | 미착수 |
| A-13 | CPU 로그에 기준 표기 (`CPU[all]` 전체 코어, `MAIN[core]` 코어 1개 기준) | 정리 | 완료 (2026-11-05) |
| A-14 | 덤프 한도 리셋: `m_dumpCount`가 리셋되지 않아 세션당 3개 후 영구 중단 → 응답 정상 + CPU 정상이면 `[D8]` 리셋, `s_logged`를 `m_bDumpLimitLogged`로 변경, `DUMP_INTERVAL_MS` 1000→3000 | 결함 | 완료, 드래그 테스트 확인 |
| A-15 | 덤프 크기 축소: Agent에 `DumpCallback`이 없어 90MB (C:\Windows 모듈이 65MB). whdump와 같은 콜백 적용, ntdll 데이터는 `!locks`용으로 유지 | 성능 | 완료: 89.7MB → 21.2MB. cdb에서는 콜스택이 끝까지 풀림. 단 Agent 자체 `AnalyzeDump`는 `IMGSF50Filter.dll`(MarkAny, C:\Windows\System32, PDB 없음) 프레임에서 끊김(A-20). `!locks`는 90MB 덤프에서도 동일하게 부분 결과(힙 미포함)라 회귀 아님 |
| A-20 | `AnalyzeDump`의 `StackWalk64`가 PDB 없는 x86 FPO 프레임에서 쓰레기 주소(`0xFFFFFFFF8A011F38` = ESI 값)로 끊김. 콜백(시스템 DLL 코드 제외) 영향인지 미확정 | 결함 | 미착수 |
| A-16 | 세션 전체 덤프 상한 없음: CpuSpike는 덤프 중 대상 프로세스가 정지돼 다음 샘플 CPU가 떨어지므로 매번 `[D8]` 리셋 → 반복 부하 시 덤프 무제한. `MAX_DUMP_TOTAL`(리셋 안 됨) 필요 | 결함 | 완료: `MAX_DUMP_TOTAL=10`, `[D6] total=` 로그 동작 확인(2026-11-06). 10개 도달 시 `[D0] max total` 동작은 미검증 |
| A-22 | CpuSpike 연속 덤프 불가: 덤프 중(약 1초) HTS 전체 스레드가 정지돼 다음 샘플 `MAIN[core]`가 80% 미만 → `cpuSpikeCount=0` → `[D8]` 리셋. 결과적으로 CPU 행은 항상 1개만 찍혀 비교 불가. 덤프와 겹친 샘플은 CPU 판정에서 제외해야 함 | 결함 | 완료: 드래그 테스트에서 약 3.7초 간격으로 덤프 3개(각 21MB), 이후 `[D0] max`, CPU가 내려가면 `[D8]`. 1차 구현은 덤프가 생략돼도 건너뛰기가 켜져 `cpuSpike=3`이 고착되는 버그가 있었음 → `m_dumpSeq` 증가로 실제 덤프 여부를 판단하도록 수정. (2026-11-06부터 CpuSpike 판정은 30초 연속으로 변경, A-27) |
| A-23 | 시작 시점 앞당김(A-1 일부): axis `CMainFrame::Initialize()`(OnCreate 내부)에서 `CloseAgent()` 직후 `CreateAgentProcess()` 호출, ini(use/show)는 함수 내부에서 읽음, `axSTART` 호출 제거 (사용자 반영). 생성자~OnCreate 앞부분은 여전히 감시 밖 | 기능 공백 | 완료 (axis) |
| A-24 | 사용자 Crashlog 폴더 자동 스캔 기능 제거: `Initialize()` 시점엔 `Axis::user`가 비어 `/d` 경로가 무의미. "덤프분석"(`IDC_BTN_DUMPANS`) 버튼과 `ScanAndAnalyzeMainCrashDumps`/`GetMainCrashDumpDir`/`IsDumpAlreadyAnalyzed`, `m_sUserpath` 삭제. 크래시 덤프는 "파일분석"(`IDC_BTN_DUMPOPEN`)으로 선택 분석. axis 쪽 `/d` 인자는 그대로 둠(무시됨) | 정리 | 완료 (2026-11-05) |
| A-21 | ping 폴더 무한 누적 (2026-11-05 기준 4.6GB). `MonitorLoop` 시작 시 `CleanupPingFolder(7)`: 수정시각 7일 초과 파일 삭제, `[C0]`/`[C1]` 로그. (2026-11-06부터 `dump_analysis.ini` 예외 제거, A-28) | 정리 | 완료: 첫 실행에서 36개 삭제, 4,552MB 확보 (4.6GB → 107MB) |
| A-17 | `HUNG`만 연속인 행은 덤프 안 됨: UI 스레드가 다른 스레드 창에 `SendMessage`하고 기다리는 중이면 들어오는 `WM_NULL`은 처리하므로 NORESP가 안 오름 | 기능 공백 | 적용·빌드 완료 (`hungCount>=2` → `"Hung"` 덤프, 한 루프 1덤프 우선순위 NoResponse>Hung>CpuSpike, `[M9]`는 `[M8] <원인> trigger`로 통합). 2026-11-06 확인: HTS 시작 시 화면 복원 중 Hung 덤프가 매번 발생(`hung=2~6`, `noResp=0`) → 첫 `[MAP]` 후 60초 유예 적용(A-27). 완료 |
| A-18 | `[D5] ok=1 err=87`: 성공해도 이전 `GetLastError` 값을 찍어 혼동 | 정리 | 미착수 |
| A-19 | 빌드 출력(`src\release\AxisAgent.exe`)이 HTS 폴더로 복사되지 않음. 수동 복사 필요 | 배포 | 참고 |
| A-25 | 덤프-화면 대조: HTS `CMainFrame::SetLastMaps`(AXI_LASTMAPS 단일 통로)에서 `[MAP] open/close/change … \| now=[열린 화면]`을 Agent로 전송, Agent는 최신 목록을 보관해 덤프 ini에 `MAPS=` 기록(수동 분석에는 기록 안 함) | 기능 | 완료 (2026-11-06, 로그·ini 확인) |
| A-26 | 덤프 ini에 `TRIGGER=<사유> mainCpu[core] procCpu[all] cpuSpike noResp hung` 기록 (`CreateDump(reason, trigger)`) | 기능 | 완료 (2026-11-06, ini 확인) |
| A-27 | 덤프 정책: CpuSpike는 MAIN 80% 초과 30초 연속(`CPU_SUSTAINED_SAMPLES`)일 때만, `tab\AXISAI.ini [Axisagent] cpudump`(기본 1, 0이면 끔). 첫 `[MAP]` 후 60초(`STARTUP_GRACE_MS`)는 CpuSpike·Hung 덤프 안 함(`[M8] … ignored` 1회 기록). NoResponse는 항상 덤프. `[M5]`에 `MAIN/CPU/cnt=n/30`, 연속이 끊기면 `정상 복귀` | 정책 | 완료 (2026-11-06, `[M5] cnt` 표시 확인). 사무실 단위 테스트에서 유예 60초 적정성 확인 필요 |
| A-28 | `dump_analysis.ini` 무한 누적 → 날짜별 `dump_analysis_YYYYMMDD.ini`, 7일 삭제 대상 포함 | 정리 | 완료 (2026-11-06) |
| A-29 | UI: SymPath 상단 이동, 리스트 더블클릭 상세보기(RichEdit, `AfxInitRichEdit2`), 덤프분석/스탑/리스트삭제 리사이즈 등록, SymPath 직접입력 저장·`...` 덧붙이기, 로그 대기열(A-5) | UI | 완료 (2026-11-06) |
| A-30 | (HTS form DLL, 관찰) `CfmBase::getDC()`의 `while (dc == nullptr) dc = GetDC();` — GDI 핸들 고갈 시 CPU 100% 무한 루프. NoResponse 덤프로 잡히며 `TOP`이 `GetDC` 근처면 의심 | 위험 (참고) | 관찰만, 수정 안 함 |

## 3. 상세

### A-12. MonitorLoop 스레드 수 집계가 매초 시스템 전체를 스냅샷함

**위치:** `AxisAgentDlg.cpp` `MonitorLoop()`의 `CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0)` 블록

**현재 동작**

- `TH32CS_SNAPTHREAD`는 PID 인자를 무시하고 **시스템의 모든 스레드(보통 수천 개)**를 스냅샷한다.
- 그 결과를 `Thread32First/Next`로 전부 훑으면서 `th32OwnerProcessID == m_parentPid`인 것만 센다.
- 이 작업을 `MONITOR_INTERVAL`(1000ms)마다, 즉 **1초에 한 번** 한다.

**결과값의 쓰임**

- `nThreadCount`는 로그의 `THR=` 항목으로만 나간다.
  - `[monitor][상태변화]`: HUNG 또는 NORESP 값이 바뀔 때
  - `[monitor][1분요약]`: 60초마다
- 행 판정([M4]), CPU 임계치([M5]), 덤프 트리거([M8]) 조건에는 쓰이지 않는다.
- `sendMsg`에도 들어가지만 `SendToParent`가 주석 처리돼 있다.

즉 실제로 필요한 건 대략 1분에 한 번인데, 60번을 계산하고 59번을 버린다.

**스레드 수의 진단 가치 (유지할 이유)**

- 1분 요약의 THR가 계속 늘어나면 스레드 누수를 의심할 수 있다.
- 행이 난 시점에 스레드가 급증했는지 함께 볼 수 있다.

**수정 제안 (검토용)**

1. 로그를 쓸 때만 센다.
2. `TH32CS_SNAPPROCESS`의 `PROCESSENTRY32.cntThreads`를 쓴다. 대상 PID 한 줄만 찾으면 되므로 스레드를 하나씩 셀 필요가 없다.

```cpp
// Thread count of one process via process snapshot (cntThreads), -1 on failure
static int CountProcessThreads(DWORD pid)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
		return -1;

	int nCount = -1;
	PROCESSENTRY32 pe = {};
	pe.dwSize = sizeof(pe);
	if (Process32First(hSnap, &pe))
	{
		do
		{
			if (pe.th32ProcessID == pid)
			{
				nCount = (int)pe.cntThreads;
				break;
			}
		} while (Process32Next(hSnap, &pe));
	}
	CloseHandle(hSnap);
	return nCount;
}
```

```cpp
// MonitorLoop: count threads only when a log line is written
DWORD nowTick = GetTickCount();
const bool bStateChanged = (bHung != m_bLastHung || noResponseCount != m_lastNoResponseCount);
const bool bSummaryDue   = (nowTick - m_lastMonitorSummaryTick >= 60000);

int nThreadCount = -1;	// -1 = not measured this tick
if (bStateChanged || bSummaryDue)
	nThreadCount = CountProcessThreads(m_parentPid);
```

**주의할 점**

- `SendToParent(sendMsg, ...)`를 다시 살리면 매초 THR 값이 필요해진다. 그때는 (1)을 적용하지 말고 (2)만 적용한다.
- 스레드 수가 급증하는 순간을 즉시 로그로 남기려면, 상태변화 조건에 "직전 값 대비 일정 개수 이상 증가"를 추가해야 한다. 이 경우에는 매초 측정이 필요하므로 (2)만 적용한다.
- 스냅샷 실패 시 기존처럼 `GetLastError()`를 로그로 남긴다.

## 4. 변경 이력

| 날짜 | 내용 |
|---|---|
| 2026-11-05 | 문서 작성. 메모리에만 있던 과제 A-1~A-11을 옮기고, MonitorLoop 스레드 수 집계 비효율(A-12) 추가 |
| 2026-11-05 | A-3, A-13, A-14 완료. 드래그 테스트에서 `[M9]` → 덤프 → `AnalyzeDump`(메인스레드 TID 일치, `[MAIN_UI]` 분류) → `[D8]` 정상 확인. A-15 적용(테스트 대기), A-16~A-19 추가. 참고: 평상시(장중) `MAIN[core]`가 40~49%로 80% 임계치 여유가 작음 |
| 2026-11-06 | A-5 완료 (로그 대기열 + PostMessage, 중지 해제 시 재표시) |
| 2026-11-06 | A-7 완료 (크래시 덤프는 예외 컨텍스트로 StackWalk). UI: 덤프분석/스탑/리스트삭제 버튼을 리사이즈 레이아웃에 등록. SymPath: 직접 입력값도 "덤프분석" 시 ini 저장(`SaveSymPath`), `...`은 덧붙이기(중복 제외), ini 읽기 버퍼 1024. 저장소 `.gitignore`에 `*.dmp`/`*.DMP` 추가 |
| 2026-11-06 | HTS 화면 이력 로그 1단계: axis `CMainFrame::SetLastMaps`(AXI_LASTMAPS 단일 통로, ChildFrm/SChild의 SetMapName·OnClose)에서 `[MAP] open/close/change … \| now=[열린 화면 목록]`을 `SendLogToAgent`. 2단계 적용: Agent `OnCopyData`가 `[MAP]` 줄의 `now=`를 `m_lastMaps`(`m_mapsLock`)에 보관, `CreateDump` 시작 시 복사 → 덤프 성공 후 해당 섹션에 `MAPS=`(없으면 `(unknown)`). 수동 "덤프분석"에는 기록 안 함(현재 목록이 과거 덤프와 무관). 1·2단계 모두 사용자 테스트로 `[MAP]` 로그와 ini `MAPS=` 기록 확인 (2026-11-06). 참고: `WriteLog`는 `#if 0`으로 운영에서 무동작 |
| 2026-11-06 | UI: SymPath 줄을 필터 줄 아래(맨 위)로 이동, 리스트와 하단 버튼 사이에 상세보기 RichEdit(`IDC_EDIT_DETAIL`, RichEdit20A, 읽기전용, 3줄+세로 스크롤) 추가. 리스트 더블클릭(`OnNMDblclkListLog`) 시 시간/유형/메시지 전체 표시. `InitInstance`에 `AfxInitRichEdit2()` 추가 |
| 2026-11-06 | `dump_analysis.ini` → 날짜별 `dump_analysis_YYYYMMDD.ini`(`GetDumpAnalysisIniPath`), `CleanupPingFolder` 예외 제거로 다른 로그처럼 7일 경과 시 삭제. 기존 단일 `dump_analysis.ini`도 마지막 수정 후 7일 지나면 삭제됨 |
| 2026-11-06 | 덤프 ini에 `TRIGGER=<사유> mainCpu[core]=… procCpu[all]=… cpuSpike/noResp/hung` 추가 (`CreateDump(reason, trigger)`). 사례: IB201100 열기 3초 후 MAIN 95.4%로 CpuSpike 덤프, 약 4초 만에 정상화. 스택은 CoreMessaging(PDB 없음, `MsgRelease`는 근접 export명) 메시지 처리 중 한 순간이라 원인 함수 특정 불가 |
| 2026-11-06 | CpuSpike 덤프 정책: ① 첫 `[MAP]` 수신 후 60초(`STARTUP_GRACE_MS`)는 화면 복원으로 보고 덤프 안 함 ② 세션당 1개(`MAX_CPU_DUMP_TOTAL`) ③ `<home>	ab\AXISAI.ini [Axisagent] cpudump=1`일 때만 허용(기본 0, 매번 읽어 즉시 반영). 건너뛸 때 `[M8] CpuSpike ignored: <사유>` 1회 기록. NoResponse/Hung 덤프는 영향 없음 |
| 2026-11-06 | CpuSpike 보완: 세션 1개 상한을 썼어도 CPU 초과가 30초 연속(`CPU_SUSTAINED_SAMPLES`)이면 덤프 (메시지를 처리하면서 CPU를 계속 쓰는 경우 대비). cpudump/시작 60초/부하당 3개/세션 10개 규칙은 그대로 |
| 2026-11-06 | CpuSpike 규칙 단순화: 세션 1개 상한 제거, CPU 초과 30초 연속(`CPU_SUSTAINED_SAMPLES`)일 때만 덤프. 짧은 부하는 `[M5]` 로그만. cpudump(기본 0)·첫 `[MAP]` 후 60초 유예는 유지 |
| 2026-11-06 | `cpudump` 기본값 1로 변경 (값이 없으면 켜짐, `cpudump=0`일 때만 끔). 30초 연속 부하는 사실상 멈춘 것으로 보고 모든 PC에서 수집 |
| 2026-11-06 | `[M5]` 로그에 `MAIN[core]`/`CPU[all]`/`cnt=n/30` 표시, 80% 이하로 떨어져 연속이 끊기면 `[M5] CPU 정상 복귀 (cnt n -> 0)` 기록. 관찰: 15:01 HTS 시작 시 화면 복원 중 `IsHungAppWindow`로 Hung 덤프 2개(37MB, TOP=NtUserCreateWindowEx/SetScrollInfo) — Hung은 시작 60초 유예 대상 아님. 15:02:08~42 드래그 부하(MAIN 약 90%)는 중간에 끊겨 30초 연속 미달로 덤프 없음 |
| 2026-11-06 | 시작 60초 유예를 Hung에도 적용(첫 `[MAP]` 후 60초, 화면 복원 중 `IsHungAppWindow` 오탐 방지, `[M8] Hung ignored` 1회 기록). NoResponse는 유예 없음. 근거: 14:57/15:01 두 번의 시작 모두 Hung 덤프 2개씩(`hung=2~6`, `noResp=0`, `cpuSpike=5~8`) |