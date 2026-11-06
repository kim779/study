# WinDbg

## 목차

- [1. 문서 목적](#1-문서-목적)
- [2. 환경 설정](#2-환경-설정)
  - [2.1 디버거 경로](#21-디버거-경로)
  - [2.2 심볼 경로](#22-심볼-경로)
  - [2.3 32비트 덤프 다루기 (WOW64)](#23-32비트-덤프-다루기-wow64)
- [3. 덤프 수집](#3-덤프-수집)
  - [3.1 크래시와 행의 차이](#31-크래시와-행의-차이)
  - [3.2 수집 방법별 정리](#32-수집-방법별-정리)
  - [3.3 덤프가 0KB로 생성될 때](#33-덤프가-0kb로-생성될-때)
- [4. 분석 절차](#4-분석-절차)
  - [4.1 크래시 덤프](#41-크래시-덤프)
  - [4.2 행 덤프](#42-행-덤프)
  - [4.3 자주 쓰는 명령](#43-자주-쓰는-명령)
- [5. 스택 읽는 법](#5-스택-읽는-법)
  - [5.1 한 줄의 구성](#51-한-줄의-구성)
  - [5.2 스레드 헤더의 구성](#52-스레드-헤더의-구성)
  - [5.3 걸린 스레드와 쉬는 스레드 구분](#53-걸린-스레드와-쉬는-스레드-구분)
  - [5.4 맨 위 대기 함수로 대기 대상 판단](#54-맨-위-대기-함수로-대기-대상-판단)
- [6. 사례 #1: 시작 시 인쇄 스풀러 대기로 로그인 창 미표시 (2026-11-03)](#6-사례-1-시작-시-인쇄-스풀러-대기로-로그인-창-미표시-2026-11-03)
  - [6.1 증상](#61-증상)
  - [6.2 !analyze -v 결과와 함정](#62-analyze--v-결과와-함정)
  - [6.3 0번 스레드 스택 해석](#63-0번-스레드-스택-해석)
  - [6.4 나머지 스레드](#64-나머지-스레드)
  - [6.5 결론](#65-결론)
  - [6.6 고객 PC 조치](#66-고객-pc-조치)
  - [6.7 소스 수정](#67-소스-수정)
  - [6.8 덤프로 알 수 없었던 것](#68-덤프로-알-수-없었던-것)
- [7. 덤프 크기 축소 (whdump)](#7-덤프-크기-축소-whdump)
- [8. 남은 과제](#8-남은-과제)

---

## 1. 문서 목적

AXIS(HTS)의 크래시·행 덤프를 cdb/WinDbg로 분석하는 방법과 실제 사례를 누적 기록한다.
새 사례가 생기면 6장 뒤에 `사례 #N`으로 추가한다.

## 2. 환경 설정

### 2.1 디버거 경로

| 용도 | 경로 |
|---|---|
| 64비트 cdb (WOW64 덤프 포함 범용) | `C:\Program Files (x86)\Windows Kits\8.1\Debuggers\x64\cdb.exe` |
| 32비트 cdb (32비트로 받은 덤프 전용) | `C:\Program Files (x86)\Windows Kits\8.1\Debuggers\x86\cdb.exe` |

실행 예:

```cmd
cdb.exe -z "D:\src\IBKS\err\axis.DMP" -y "SRV*D:\symbol*https://msdl.microsoft.com/download/symbols;D:\symstore"
```

### 2.2 심볼 경로

| 경로 | 내용 |
|---|---|
| `SRV*D:\symbol*https://msdl.microsoft.com/download/symbols` | MS 공개 심볼(ntdll, rpcrt4, winspool, comdlg32 등). 인터넷 연결 필요 |
| `D:\symstore` | 우리 빌드 PDB(axis 등). 빌드 후 이벤트의 `symstore add`로 자동 등록 |

- MS 심볼 서버를 넣어야 `winspool!Ordinal222` 대신 `winspool!SendRecvBidiData`처럼 실제 함수명이 나온다.
- 출력에 `GetUrlPageData2 (WinHttp) failed: 12030`이 보이면 인터넷 연결이 막혀 MS 심볼을 받지 못한 것이다.
- 우리 모듈이 `axis!Axis_State+0xce31d`처럼 **오프셋이 매우 크게** 나오면 PDB가 없는 것이다. 가장 가까운 export 이름을 빌린 것이라 함수명으로 믿으면 안 된다.
- **배포하는 빌드마다 PDB를 symstore에 등록해야 한다.** 사례 #1의 axis.exe(2026-09-11 09:17, timestamp `6AA348AE`)는 PDB가 없어 axis 프레임의 함수명을 얻지 못했다.

### 2.3 32비트 덤프 다루기 (WOW64)

axis.exe는 32비트다. 64비트 작업관리자로 덤프를 받으면 64비트 껍데기(wow64) 스택만 보인다.

```text
.load wow64exts
!sw                 ; prompt becomes 0:000:x86>
```

- 처음부터 **32비트 작업관리자**(`C:\Windows\SysWOW64\taskmgr.exe`)로 받으면 `!sw`가 필요 없다.
- x86 cdb로 64비트 작업관리자 덤프를 열면 `.effmach x86` 오류가 난다. 이 경우 x64 cdb + `!sw`를 쓴다.

## 3. 덤프 수집

### 3.1 크래시와 행의 차이

| 구분 | 크래시 | 행 |
|---|---|---|
| 의미 | 예외로 비정상 종료 | 프로세스는 살아 있으나 UI가 반응 없음 |
| 자동 덤프 | whdump(SetUnhandledExceptionFilter), WER LocalDumps | **없음** (Windows는 행에서 덤프를 남기지 않음) |
| 분석 핵심 | 예외 발생 위치(`.ecxr`) | 멈춘 스레드가 무엇을 기다리는지 |
| `!analyze -v` | 유효 | **결과가 엉뚱함** (가짜 `80000003` 예외로 보고) |

행의 세 가지 유형:

| 유형 | 상태 | CPU |
|---|---|---|
| 동기 대기형 | 외부(서비스·네트워크·다른 프로세스) 응답을 타임아웃 없이 기다림 | 0% |
| 데드락형 | 스레드끼리 서로 잠금을 기다림 | 0% |
| 무한 루프형 | 계속 실행되지만 끝나지 않음 | 높음 |

### 3.2 수집 방법별 정리

| 방법 | 대상 | 비고 |
|---|---|---|
| whdump (axis 내장) | 크래시 | `MiniDumpWriteDump` + 콜백으로 Windows 모듈 메모리 제외 |
| WER LocalDumps | 크래시 | `HKLM\SOFTWARE\Microsoft\Windows\Windows Error Reporting\LocalDumps\axis.exe` (DumpFolder REG_EXPAND_SZ, DumpType=2, DumpCount=5). whdump가 예외를 처리하면 동작하지 않음 |
| 작업관리자 → 덤프 파일 만들기 | 행 | 저장 위치 `%LOCALAPPDATA%\Temp\axis.DMP`. 32비트 작업관리자 권장 |
| ProcDump `procdump -ma axis.exe` | 행 | 실패 시 오류 메시지가 나와 원인 파악이 쉬움 |
| ProcDump `procdump -h axis.exe` | 행 자동 | 창이 5초 이상 무응답일 때. **창이 생기기 전 행은 못 잡음** |
| AxisAgent | 행 자동 | 창 응답 감시 기반. `CMainFrame` 이후 실행되어 **시작 단계 행은 못 잡음** |

- 행 덤프는 **10~30초 간격으로 2~3개** 받는다. 매번 같은 위치면 그 지점에서 멈춰 있다고 확신할 수 있다.
- 외부 서비스를 기다리는 행이면 **상대 프로세스 덤프도 같이** 받는다(사례 #1이면 spoolsv.exe).

### 3.3 덤프가 0KB로 생성될 때

덤프 크기는 프로세스가 정상이든 아니든 같다(정상 axis도 수백 MB). **0KB는 작성 실패**다.

- 사내 PC는 DRM(Document Safer)·보안 모듈이 다른 프로세스 메모리 읽기나 파일 쓰기를 막는 경우가 많다.
- 확인 순서: 관리자 권한 32비트 작업관리자로 재시도 → ProcDump로 재시도(오류 메시지 확인) → 보안 담당에 예외 요청.
- 사례 #1에서 고객 PC 덤프는 213MB로 정상이었고, 사내 PC에서 정상 axis를 덤프하면 0KB였다.

## 4. 분석 절차

### 4.1 크래시 덤프

```text
!analyze -v
.ecxr               ; switch to exception context
kn                  ; stack at the exception
```

### 4.2 행 덤프

```text
.load wow64exts
!sw
~0kn                ; main(UI) thread first
~*kn                ; all threads
!locks              ; if any thread waits on a critical section
lm                  ; loaded modules (security/hook DLLs etc.)
```

1. 0번(UI) 스레드 스택을 아래에서 위로 읽는다.
2. 아래에서 처음 나오는 우리 모듈(`axis`, `cx_*`, `ax*`)이 진입점이다.
3. 그 바로 위의 시스템 API가 "우리가 부른 것"이다.
4. 맨 위 대기 함수로 무엇을 기다리는지 판단한다(5.4).
5. 다른 스레드가 잠금을 기다리면 데드락 여부를 `!locks`로 본다.

### 4.3 자주 쓰는 명령

| 명령 | 용도 |
|---|---|
| `~` | 스레드 목록 |
| `~0kn` / `~*kn` / `~*kn 10` | 0번 / 전체 / 전체(상위 10프레임) 스택 |
| `~3s` | 현재 스레드를 3번으로 전환 |
| `!runaway` | 스레드별 CPU 사용 시간 (무한 루프형 구분) |
| `!locks` | 잠긴 CriticalSection과 소유 스레드 |
| `lm`, `lmvm 모듈` | 모듈 목록, 모듈 상세(버전·경로·timestamp) |
| `ub 주소 L8` | 리턴 주소 직전 역어셈블 (어떤 함수를 call 했는지) |
| `!sym noisy; .reload /f axis.exe` | PDB 불일치 진단 |

## 5. 스택 읽는 법

### 5.1 한 줄의 구성

```text
18 00e9fa1c 0107af7b axis!Axis_State+0xce31d
│  │        │        └ module!function+offset : where this frame is executing
│  │        └ RetAddr : return address in the caller (next line below)
│  └ ChildEBP : stack frame base of this function
└ frame number : 00 = most recent (top), larger = earlier caller
```

- 실행 순서는 **아래(큰 번호)에서 위(00)**.
- 맨 위(00)는 덤프 순간 스레드가 있던 위치다. 대기 시스템 콜이면 그 다음은 커널 안이라 사용자 덤프에는 나오지 않는다.
- `WARNING: Stack unwind information not available` 아래 프레임은 심볼이 없어 틀릴 수 있다.

### 5.2 스레드 헤더의 구성

```text
0  Id: 2d28.ce4 Suspend: 0 Teb: 09447000 Unfrozen
```

| 항목 | 의미 |
|---|---|
| `0` | 디버거 스레드 번호 |
| `2d28.ce4` | 프로세스 ID . 스레드 ID |
| `Suspend: 0` | `SuspendThread`로 정지된 횟수. **0이면 정지 아님** |
| `Teb` | 스레드 정보 블록 주소 |
| `Unfrozen` | 디버거에서 얼렸는지(`~Nf`). 프로그램 동작과 무관 |

정지(Suspended)와 대기(Waiting)는 다르다. 행의 대부분은 **대기**이며 `Suspend: 0`으로 나온다.

### 5.3 걸린 스레드와 쉬는 스레드 구분

둘 다 맨 위는 대기 함수다. **그 아래가 무엇인지**로 구분한다.

| 구분 | 대기 함수 아래 | 예 |
|---|---|---|
| 쉬는 스레드 | 작업을 기다리는 루프 | `TppWorkerThread`(스레드 풀), `combase!CRpcThread::WorkerLoop`(COM) |
| 걸린 스레드 | 우리 코드에서 시작된 긴 호출 사슬 | axis → mfc → comdlg32 → winspool → rpcrt4 |

### 5.4 맨 위 대기 함수로 대기 대상 판단

| 맨 위 함수 | 기다리는 대상 |
|---|---|
| `NtAlpcSendWaitReceivePort` + `rpcrt4` | 다른 프로세스(서비스)의 RPC 응답. 바로 아래 모듈로 상대를 판단 (winspool → 스풀러) |
| `NtWaitForSingleObject` / `WaitForMultipleObjects` | 이벤트·스레드·뮤텍스 |
| `RtlpWaitOnCriticalSection` | CriticalSection (`!locks`로 소유자 확인) |
| `NtUserMessageCall` / `SendMessage` | 다른 스레드의 창이 메시지를 처리하기를 기다림 |
| `NtDelayExecution` | `Sleep` |
| `NtWaitForWorkViaWorkerFactory` | 스레드 풀 유휴 (정상) |

## 6. 사례 #1: 시작 시 인쇄 스풀러 대기로 로그인 창 미표시 (2026-11-03)

### 6.1 증상

- 고객 PC(Windows 11 24H2, 빌드 26100, 8코어)에서 axis.exe 프로세스는 있는데 로그인 창이 뜨지 않음.
- 로그는 `[axis][initinstance]  m_regkey=[...]`까지만 기록되고 `[axis]Initinstance 1`은 기록되지 않음.
- 처음 의심은 레지스트리·보안 프로그램이었으나 덤프 결과 인쇄 스풀러였다.
- 덤프: `D:\src\IBKS\err\axis.DMP` (213MB, 64비트 작업관리자로 수집, 프로세스 시작 10초 후, 부팅 후 2시간 38분)

### 6.2 !analyze -v 결과와 함정

```text
ExceptionCode: 80000003 (Break instruction exception)
FAILURE_BUCKET_ID: STATUS_BREAKPOINT_80000003_wow64.dll!whNtAlpcSendWaitReceivePort
```

- `80000003`은 예외 없는 행 덤프에 cdb가 붙이는 **가짜 예외**다.
- `wow64!...`, `LdrpInitializeProcess` 스택은 64비트 껍데기다. 실제 axis 스택은 `!sw` 후에 보인다.
- **행 덤프에서는 `!analyze -v`를 결론으로 쓰지 않는다.**

### 6.3 0번 스레드 스택 해석

```text
0:000:x86> ~0kn
00 ntdll!NtAlpcSendWaitReceivePort+0xc
01 rpcrt4!LRPC_BASE_CCALL::DoSendReceive+0x137
02 rpcrt4!LRPC_CCALL::SendReceive+0x5a
03 rpcrt4!NdrClientCall2+0xbb2
04 rpcrt4!NdrClientCall4+0x14
05 winspool!SendRecvBidiData+0x5b
06 bidispl!TBidiSpl::MultiSendRecv+0x100
07 bidispl!TBidiSpl::SendRecv+0x7a
08 IppCommon!...IppPrinterInfoHandler::CheckIsIppPrinterPortValid+0x298
09 IppCommon!...IppPrinterInfoHandler::CreatePrinterInfoEntryForLocalPrinter+0x1b7
0a IppCommon!...IppPrinterInfoHandler::CreatePrinterInfoEntry+0x5e
0b IppCommon!...IppPrinterInfoHandler::RetrieveOrCreatePrinterInfoEntry+0xa1
0c IppCommon!...IppPrinterInfoHandler::GetPrinterInfo+0xeb
0d IppCommon!...IppPrinterInfoHandler::IsModernPrinter+0x1c
0e IppCommon!IsModernPrinter+0x4f
0f winspool!RegeneratePdcBeforeSplwow64+0xaa
10 winspool!DocumentPropertiesW+0xa5
11 comdlg32!PrintGetDevMode+0x1a
12 comdlg32!PrintReturnDefault+0x70
13 comdlg32!PrintDlgX+0x133
14 comdlg32!PrintDlgA+0x5b
15 mfc140+0x22dea6
16 mfc140+0x1e6718
17 mfc140+0x1e64c4
18 axis!Axis_State+0xce31d
19 axis!Axis_State+0x22cb
1a axis+0x1053c
1b mfc140+0x2a1480
1c axis!Axis_State+0xfcae8
1d kernel32!BaseThreadInitThunk+0x19
1e ntdll!__RtlUserThreadStart+0x2b
1f ntdll!_RtlUserThreadStart+0x1b
```

| # | 함수 | 해석 |
|---|---|---|
| 1f~1d | ntdll / kernel32 | 스레드 시작 |
| 1c | axis (추정: CRT 진입점 → WinMain) | 프로그램 진입 |
| 1b | mfc140 (추정: `AfxWinMain`) | MFC 메인. `InitInstance` 호출 |
| 1a | axis (추정: `CAxisApp::InitInstance`) | 로그가 `m_regkey`에서 끊긴 함수 |
| 19 | axis (추정: `CMainFrame::CMainFrame`) | `new CMainFrame` |
| 18 | axis (추정: `CPrinterSettings::CopyDefaultMfcPrinter`) | `MainFrm.cpp:1024`에서 호출 |
| 17 | mfc140 (추정: `CWinApp::GetPrinterDeviceDefaults`) | 기본 프린터 조회 |
| 16 | mfc140 (추정: `CWinApp::UpdatePrinterSelection`) | 기본 프린터 정보가 없으면 새로 받음 |
| 15 | mfc140 (추정: `CPrintDialog` 계열) | `PD_RETURNDEFAULT`로 `PrintDlgA` 호출 |
| 14 | `comdlg32!PrintDlgA` | 인쇄 대화상자 API (axis는 MBCS라 A 버전) |
| 13 | `comdlg32!PrintDlgX` | A/W 공통 처리 |
| 12 | `comdlg32!PrintReturnDefault` | **대화상자 없이 기본 프린터만 반환하는 모드** (`PD_RETURNDEFAULT` 확정) |
| 11 | `comdlg32!PrintGetDevMode` | 기본 프린터의 DEVMODE(용지·방향 등) 획득 |
| 10 | `winspool!DocumentPropertiesW` | 드라이버에게 DEVMODE를 받는 표준 API |
| 0f | `winspool!RegeneratePdcBeforeSplwow64` | **32비트 앱 전용 경로.** Splwow64 중계 전 프린터 기능 정보(PDC) 재생성 판단 |
| 0e~0d | `IppCommon!IsModernPrinter` | 기본 프린터가 IPP 기반 최신 프린터인지 판별 |
| 0c~09 | `GetPrinterInfo` → `RetrieveOrCreatePrinterInfoEntry` → `CreatePrinterInfoEntryForLocalPrinter` | 캐시에 없어서(첫 실행) 로컬 프린터 정보 항목을 새로 생성 |
| 08 | **`CheckIsIppPrinterPortValid`** | **포트가 살아 있는 IPP 포트인지 실제 통신으로 확인** |
| 07~06 | `bidispl!TBidiSpl::SendRecv` / `MultiSendRecv` | Bidi(양방향) 질의 전송 |
| 05 | `winspool!SendRecvBidiData` | 질의를 스풀러 서비스(spoolsv.exe)로 전달 |
| 04~01 | `rpcrt4` (NdrClientCall → LRPC SendReceive) | 로컬 RPC 요청 후 **타임아웃 없이** 응답 대기 |
| 00 | **`ntdll!NtAlpcSendWaitReceivePort`** | 커널 ALPC 대기. **스레드가 여기서 잠들어 있음** |

axis·mfc140 프레임 이름은 PDB가 없어 소스 흐름에 맞춘 추정이다. 근거는 초기화 중 MFC가 `PrintDlg`를 부르는 경로가 `CopyDefaultMfcPrinter` 하나뿐이라는 점이다. Windows 프레임은 MS 심볼로 확인된 이름이다.

### 6.4 나머지 스레드

| 스레드 | 스택 | 정체 | 상태 |
|---|---|---|---|
| 0 | 위 표 | axis 메인(UI) | **스풀러 RPC 응답 대기** |
| 1~4, 6~8 | `TppWorkerThread` → `NtWaitForWorkViaWorkerFactory` | Windows 스레드 풀 | 유휴(정상) |
| 5 | `combase!CRpcThread::WorkerLoop` → `CROIDTable::WorkerThreadLoop` → `WaitForMultipleObjects` | COM 관리 스레드(`CoInitialize`가 생성) | 유휴(정상) |

- 9개 스레드 모두 `Suspend: 0`.
- axis가 만든 스레드는 0번뿐이다. `CMainFrame` 생성 중 멈춰서 axis 자체 스레드(시세·통신 등)는 아직 시작 전이었다.
- 잠금을 기다리는 스레드가 없으므로 데드락이 아니다.

### 6.5 결론

**동기 대기형 행.** HTS 시작 → `CMainFrame` 생성자에서 기본 프린터 조회 → Windows가 32비트 앱 경로에서 IPP 포트 유효성을 스풀러에 질의 → 스풀러 무응답 → UI 스레드가 타임아웃 없이 대기.
창이 생성되기 전이라 "(응답 없음)" 표시도 없이 프로세스만 남았다.

추가 확인:

- 고객 PC 기본 프린터는 Microsoft Print to PDF였다. 특정 네트워크 프린터가 아니라 **스풀러 자체가 무응답**이었다.
- `Get-CimInstance Win32_Printer`도 결과 없이 끝나 스풀러 이상이 확인됐다.
- 스풀러 재시작 후 로그인 창이 정상 표시되어 원인이 확정됐다.

### 6.6 고객 PC 조치

관리자 cmd:

```cmd
net stop spooler
del /q /f %systemroot%\System32\spool\PRINTERS\*
net start spooler
```

- `net stop`이 실패하면 `taskkill /f /im spoolsv.exe` 후 이어서 실행한다.
- `del`은 대기 중인 인쇄 작업(.SPL/.SHD)만 지운다. 이 파일은 디스크에 남아서 **재부팅으로는 해결되지 않을 수 있다**. 3줄을 모두 실행하도록 안내한다.
- 직접 멈춘 스풀러는 인쇄를 시도해도 자동으로 다시 시작되지 않는다(`net start` 또는 재부팅 필요).

배포용 bat (`HTS_spool.bat`, 오른쪽 클릭 → 관리자 권한으로 실행):

```bat
@echo off
net session >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Right-click this file and choose "Run as administrator".
    pause
    exit /b 1
)

echo Stopping Print Spooler...
net stop spooler /y
taskkill /f /im spoolsv.exe >nul 2>&1

echo Clearing pending print jobs...
del /q /f "%systemroot%\System32\spool\PRINTERS\*"

echo Starting Print Spooler...
net start spooler

echo.
echo Done. Please restart the HTS.
pause
```

주의: 사내 PC에서 메모장으로 저장하면 DRM(`<DOCUMENT SAFER V2010 R2>` 헤더)으로 암호화되어 cmd에서 실행되지 않는다(창이 열렸다 바로 닫힘). DRM 해제 또는 DRM 없는 PC에서 작성해야 한다.

### 6.7 소스 수정

시작 시 프린터 조회를 빼고 첫 인쇄·첫 프린터 설정 때 조회하도록 지연 초기화했다.

`AXIS/MainFrm.cpp:1024` (CP949)

```cpp
	m_printsetup = new CPrinterSettings();
	//m_printsetup->CopyDefaultMfcPrinter();
```

`AXIS/PrinterSettings.cpp` `PrinterSetup()`, `SetThisPrinter()` 앞부분

```cpp
	if (!m_hDevMode)
		CopyDefaultMfcPrinter();
```

| 경로 | 호출 | 가드 |
|---|---|---|
| 화면 인쇄 | `printImg` → 1초 타이머 `TM_PRNIMG` → `printOper` → `SetThisPrinter` | O |
| 프린터 설정 메뉴 | `ID_APP_PRINT_SETUP` → `PrinterSetup` | O |
| `RestorePrinter` | `printOper` 끝 | 불필요 (`m_hDevMode` 미사용) |

테스트 결과 (2026-11-03):

| 테스트 | 결과 |
|---|---|
| `net stop spooler` 상태에서 HTS 시작 | 로그인 창 정상 표시 |
| 프린터 설정 메뉴 | 열림, 속성의 인쇄 방향 가로 |
| 화면 인쇄 | 가로로 실제 출력 |

한계: 스풀러가 막힌 PC에서 **인쇄를 시도하면** 그 시점에 같은 이유로 멈춘다(`printOper`의 `CreatePrinterDC`도 같은 조회를 함). 시작과 로그인은 보호된다.

### 6.8 덤프로 알 수 없었던 것

- **스풀러가 왜 응답하지 않았는지**는 axis 덤프로 알 수 없다. 재발하면 axis.exe와 **spoolsv.exe 덤프를 같이** 받는다(관리자 작업관리자 → 세부 정보 → spoolsv.exe).
- 부팅 후 2시간 38분 시점에도 막혀 있었으므로, 재부팅해도 남는 원인(쌓인 인쇄 작업 파일 또는 드라이버)일 가능성이 있다.
- 커널 드라이버 쪽은 사용자 모드 덤프에 보이지 않는다. 필요하면 `fltmc`(필터 드라이버 목록)와 Process Monitor의 Duration을 함께 본다.

## 7. 덤프 크기 축소 (whdump)

| 항목 | 내용 | 상태 |
|---|---|---|
| Windows 모듈 메모리 제외 | `AXIS/whdump.cpp`에 `DumpCallback` 추가. `C:\Windows\` 모듈의 `ModuleWriteDataSeg`/`ModuleWriteCodeSegs` 제거 | 적용·검증 완료 (3.1MB → 0, 스택 정상) |
| 대형 전역 배열 | `g_tickSlots`(MainFrm.cpp:143), `g_dlltickSlots`(cx_shared CMainWnd.cpp:16) 각 `TickSnapshot[4096]`이 `.data`에 있어 `MiniDumpWithDataSegs`로 통째 포함(합계 약 131MB) | **적용·검증 완료 (2026-11-03).** `VirtualAlloc` 대신 기능 자체를 끔: MainFrm.h `DF_MAIN_RTS` 비활성(`DF_MAIN_RTS1`), cx_shared 틱 저장소 코드 전부 제거. 덤프 279MB → **15MB**, `.ecxr; kn`으로 `cx_shared!CMainWnd::OnCreate+0x134` 정상 확인 |
| DF_MAIN_RTS 비활성 부작용 | `Axis_State`, `Axis_GetMainWnd` export와 AxisAgent 멤버가 블록 안에 있어 LNK2019(`_Axis_State` in whdump.obj) 발생 | 해결: export와 AxisAgent 멤버를 `DF_MAIN_RTS` 블록 밖으로 이동. `Axis_IsMainRTS`는 사용처 없어 삭제 |
| 테스트 크래시 코드 | cx_shared `CMainWnd::OnCreate`의 `memcpy(pnull, ...)` | 검증 후 삭제 완료 |
| 운영 빌드 대조 | 운영 PC는 whdump 콜백 없이도 덤프가 작았음. WinDbg에서 `x axis!CMainFrame::DrawFrame`은 검색되고 `x axis!g_tickSlots`는 없음 → 운영 빌드에는 대형 배열이 원래 없었음 | 확인 (2026-11-03) |

**결론:** 덤프 크기의 대부분(약 260MB)은 개발 중이던 `DF_MAIN_RTS` 틱 저장소 전역 배열이었다. whdump 콜백 효과는 약 3MB다. 앞으로 덤프가 갑자기 커지면 `.data`/`.bss`에 큰 전역 배열이 추가됐는지부터 본다.

**심볼 확인 요령:** `x 모듈!변수`가 아무것도 출력하지 않으면 "변수 없음"과 "PDB 못 찾음"을 구분해야 한다. `lm m 모듈`로 `(pdb symbols)` 여부를 보고, 확실히 있는 함수(`x axis!CMainFrame::DrawFrame`)를 대조 검색한다.

## 8. 남은 과제

- **AxisAgent 시작 단계 행 감지:** 현재 `CMainFrame::CreateAgentProcess`에서 실행되어 사례 #1처럼 그 전에 멈추면 감시자가 없다. 또한 창 응답(`SendMessageTimeout`, `IsHungAppWindow`) 기반이라 창이 없으면 감지하지 못한다. → 실행 시점을 앞당기고, "시작 후 N초 안에 로그인 창이 없으면 덤프" 감시를 검토.
- **배포 빌드 PDB 관리:** 배포하는 모든 axis.exe의 PDB를 symstore에 등록.
- **사내 PC 덤프 0KB 원인 확인:** 32비트 작업관리자(관리자 권한) → ProcDump 순으로 재시도.
