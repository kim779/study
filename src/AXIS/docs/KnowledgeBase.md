# KnowledgeBase

- 생성일: 2026-07-14
- 목적: AXIS 프로젝트 분석/디버깅 과정에서 얻은 설계 의도, 트러블슈팅 히스토리, 주의사항, 도메인 지식을 누적 기록한다.
- 배경: 이 프로젝트 분석은 비정상종료(크래시) 원인 분석을 위해 지속적으로 이어갈 endless 작업이다.

## 개요

AXIS 프로젝트 아키텍처 분석 (2026-07-14, architecture Agent).
상세 구조는 [Architecture.md](Architecture.md), [Dependency.md](Dependency.md), [ArchitectureReview.md](ArchitectureReview.md) 참조.

## 목차

1. [AXIS 설계 특징](#axis-설계-특징)
2. [Crash 분석 체크리스트](#crash-분석-체크리스트)
3. [공통 주의사항](#공통-주의사항)

## AXIS 설계 특징

### 1. CAxisApp 싱글톤 패턴의 안전성

**설계 패턴**:
```cpp
CAxisApp theApp;  // axis.cpp:88 - 전역 싱글톤
// MFC 프레임워크에서 자동으로 InitInstance()/ExitInstance() 호출
```

**안전성 보장 메커니즘**:
- MFC WinMain() 루프가 CAxisApp 생명주기 관리
- 스레드 안전성: 메인 스레드에서만 UI 생성/삭제
- 이중 인스턴스 방지: CAxInstance::IsFirstInstance() 체크

**주의**:
- ExitInstance()는 반드시 호출되어야 함 (메모리 정리)
- delete 연산은 명시적으로 추적 필요

### 2. MFC Message Pump 의존성

**특징**:
- MFC Message Loop는 메인 스레드 독점
- Modal Dialog는 별도 Message Pump 스택

**Thread 안전성**:
- Worker Thread (PhoneThread, ExcelThread)는 PostMessage() 사용
- UI 업데이트는 메인 스레드로만 라우팅
- DDX/DDV (Dialog Data Exchange)는 메인 스레드에서만 호출

**잠재 위험**:
- Worker Thread에서 직접 UI 객체 접근 → Crash
- PostMessage() 순서 뒤섞임 → Race condition

### 3. Thread-Safe Grid 컨트롤

**동기화 전략**:
- Excel Export Thread (grid/fxGrid.cpp:492)
- CCriticalSection (MFC 뮤텍스)
- 우선순위: THREAD_PRIORITY_HIGHEST (Excel 내보내기 우선)

**메커니즘**:
```cpp
AfxBeginThread(RunExcelThread, this, THREAD_PRIORITY_HIGHEST)
// -> RunExcelThread() 내부에서 CCriticalSection::Lock()
```

**잠재 위험**:
- Lock 타임아웃 없음 → Deadlock 가능
- Grid 업데이트 중 스레드 종료 → 미정의 동작

### 4. COM Initialization Hazard

**실제 코드 확인됨** (axis.cpp):
```cpp
// axis.cpp:327 (InitInstance 내부)
CoInitialize(NULL);   // COINIT_APARTMENTTHREADED, NOT CoInitializeEx/MULTITHREADED

// axis.cpp:698 (ExitInstance 내부)
CoUninitialize();
```

**주의**:
- `CoInitialize(NULL)`은 STA(Single-Threaded Apartment)로 초기화한다. Worker Thread(PhoneThread, ExcelThread)에서 COM 객체를 사용한다면, 해당 스레드도 별도로 CoInitialize/CoInitializeEx를 호출해야 하며 메인 스레드와 다른 아파트먼트 모델을 쓰면 marshaling 문제가 발생할 수 있다.
- COM 객체 수명 관리: Release() 쌍 필수

**잠재 위험**:
- Worker Thread에서 CoInitialize 없이 COM 객체 호출 → 실패 (0x800401xx 계열 CoInitialize 관련 에러)
- CoUninitialize 누락 → 메모리 누수
- STA 스레드 간 COM 포인터 직접 공유 → 크래시 (marshaling 필요)

### 5. Socket Event Handling Pattern

**초기화**:
```cpp
AfxSocketInit()  // axis.cpp:190
```

**특징**:
- MFC CAsyncSocket 기반 (비동기)
- Message Pump와 통합
- Worker Thread에서 실행 가능 (MESSAGE_PUMP 분리 필요)

**잠재 위험**:
- Socket 이벤트 손실 (Message Queue 오버플로우)
- Thread 종료 후 OnReceive() 콜백 → Crash
- Receive Buffer 부족 → 데이터 손실

## Crash 분석 체크리스트

### A. InitInstance 순서 검증

```
[필수] Axis::Initialize()
  - 설정 파일 로드 성공?
  - 기본 자원 할당 성공?

[필수] SetOtherCore() [CLI/Registry]
  - 레지스트리 접근 권한 있는가?
  - Legacy 설정값 호환성?

[필수] AfxSocketInit() (axis.cpp:190)
  - Winsock 버전 (2.2 이상)?
  - 네트워크 드라이버 정상?

[필수] CAxInstance::IsFirstInstance()
  - 중복 실행 감지 정상?
  - 윈도우 클래스 등록 성공?

[필수] CoInitialize(NULL) (axis.cpp:327)
  - 메인 스레드에서 한 번만 호출되는가?
  - Worker Thread는 별도로 초기화하는가?

[필수] CMainFrame::LoadFrame()
  - 자식 윈도우 생성 순서?
  - Dialog Template 리소스 존재?

[필수] SCCORE_B.DLL 동적 로드 (axis.cpp:727 부근)
  - DLL 경로 올바른가?
  - DLL 없을 때 fail-safe 동작?
```

### B. ExitInstance 정리 여부 검증

```
[필수] 로그 파일 정리
  - File Handle 모두 Close?
  - Pending Write 완료?

[필수] m_instance->UnsetClass()
  - 윈도우 클래스 등록 해제?
  - 다른 프로세스 윈도우 영향?

[필수] delete m_instance
  - nullptr 체크?
  - 소멸자 호출 완료?

[필수] CoUninitialize() (axis.cpp:698)
  - COM 객체 모두 Release()?
  - CoInitialize 호출과 반드시 짝을 이루는가?

[필수] Axis::Release()
  - 전역 자원 할당 해제?
  - 역순 정리?
```

### C. Memory Leak 주요 의심 지점

| 객체 | 생성 위치 | 삭제 위치 | 위험도 |
|------|---------|---------|--------|
| `m_instance` | InitInstance() | ExitInstance() | 높음 |
| `m_mapHelper[]` | CMainFrame::OnCreate() (추정) | ~CMainFrame() (추정) | 중간 |
| `m_axMisc` | CMainFrame::OnCreate() (추정) | ~CMainFrame() (추정) | 중간 |
| `m_axGuide` | CMainFrame::OnCreate() (추정) | ~CMainFrame() (추정) | 중간 |
| Excel Export Buffer | RunExcelThread() | 스레드 종료 | 중간 |
| COM 객체 | 다양 | Release() 호출 | 높음 |

**검증 방법**:
- ~CMainFrame() 소멸자에서 모든 new 삭제 확인
- delete vs Release() 일관성 체크

### D. Thread Synchronization 위험 지점

| 위험 상황 | 원인 | 대응 |
|----------|------|------|
| Deadlock | CCriticalSection 중첩 Lock | Lock 순서 문서화 + 재진입 불가 체크 |
| Race Condition | PostMessage() 순서 뒤섞임 | Message ID + 순서 검증 |
| Worker Thread Crash | UI 객체 직접 접근 | PostMessage() 강제 |
| Socket Event 손실 | Message Queue 오버플로우 | Event Buffer 증설 + 로깅 추가 |

## 공통 주의사항

### 향후 Crash 분석 프로세스

1. **MiniDump 수집**:
   - D:\symbol\axis.pdb (PDB 보관)
2. **Call Stack 복원**:
   - WinDbg: `.sympath D:\symbol`
   - `!analyze -v`
3. **크래시 카테고리화**:
   - InitInstance 단계: 체크리스트 A 적용
   - 주 루프 단계: Thread/COM/Socket 검증
   - ExitInstance 단계: 체크리스트 B 적용

## 크래시 사례 로그

### 사례 #1 — 2026-07-14, khs779_71410h32m27s.dmp

- **덤프 위치**: `D:\src\IBKS\err\crash\khs779_71410h32m27s.dmp` (User Mini Dump, 132MB)
- **분석 결과**: `D:\src\IBKS\err\crash\khs779_71410h32m27s.analyze.txt` (cdb `!analyze -v` 원본 출력)
- **덤프 시각**: 2026-07-14 10:32:27 (UTC+9), 프로세스 uptime 2:56:23
- **예외**: Access Violation (0xc0000005), 주소 `0x2343e000` 읽기 시도 — NULL이 아닌 유효하지 않은 포인터라 **use-after-free/댕글링 포인터 의심** (단순 널체크 누락이 아님)
- **크래시 지점**: `VCRUNTIME140!memcpy+0x51d`, 호출자는 `axis+0xdd6c8` (axis.exe 내부, 함수명 미상 — 아래 PDB 불일치 참고)

**콜스택 요약 (스레드 시작 → 크래시 순)**:
```
AfxWinMain → CWinThread::Run (메인 메시지 루프)
 → DispatchMessage → axissm!CBasebar0::OnTimer (WM_TIMER, 시세 폴링 타이머)
 → CBasebar0::Next → CBasebar0::ChangeMap → axis+0x890e1 (axis.exe, 함수명 미상)
 → (OLE 컨트롤 이벤트 체인: axisform!CfmEdit::_Trigger → CfmCtrl::WriteData
    → axWizard(OCX, 함수명 미상, export만 있음) → CX_CODECTRL!axCreate 등)
 → SendMessage 재귀 → axis+0x5a70 (axis.exe, 함수명 미상)
 → (VBScript 엔진 체인: axisvbs!CScriptEngine::DoProcedure → vbscript!InvokeDispatch
    → mfc140!COleDispatchImpl::Invoke — VBScript 매크로가 COM Dispatch로 호출됨)
 → axWizard(export만) → SendMessage 재귀
 → mfc140!CWnd::WindowProc → axis+0x8f4c8 (axis.exe, 함수명 미상)
 → axis+0xdd6c8 (axis.exe, 함수명 미상) → memcpy → CRASH
```

**해석**: 시세 갱신 타이머(`CBasebar0::OnTimer`)에서 시작된 호출이 axWizard(OCX)/axisvbs(VBScript 엔진)로 구성된
스크립트/매크로 이벤트 체인을 거쳐 다시 axis.exe 내부로 재진입하고, 그 안에서 memcpy가 유효하지 않은
포인터를 읽다가 죽었다. 재진입 깊이(SendMessage 중첩 2회 이상)와 VBScript 콜백 경유 정황을 보면,
[[project_axis_crash_analysis]]에서 지목한 "COM 객체 생명주기"·"Worker/Timer 재진입" 리스크와
정확히 일치하는 패턴 — 타이머 콜백 도중 스크립트 이벤트가 만든 객체/버퍼가 해제된 뒤에도
axis.exe 쪽 코드가 그 포인터를 계속 참조했을 가능성이 높다 (추정, axis.exe 심볼 불일치로 확정 불가).

**소스 추적 결과 (2026-07-14, axis.exe 심볼 없이 소스 코드로 역추적)**:
```
CBasebar0::OnTimer (DLL/sm/Basebar0.cpp:112, TM_CON)
 → Next() → ChangeMap() (DLL/sm/Basebar0.cpp:1088) — 자동전환바(종목/화면 순환)
 → (*m_axiscall)(AXI_CHANGEVIEW, ...) 콜백
 → axiscall() (AXIS/MainFrm.cpp:722, case AXI_CHANGEVIEW)
 → CMapHelper::ChangeChild() (AXIS/MapHelper.cpp:2632) — Wizard 스크린 생성/전환
 → CfmCtrl::WriteData() (ibks/dll/form/fmCtrl.cpp:280)
     - _extTHx.data = 로컬 CString 파라미터의 버퍼 포인터
     - m_ctrl->SendMessage(WM_USER, DLL_OUBx, &extTHx) — 동기 호출
 → CX_CODECTRL(운영)/ControlWnd.cpp:190 (DLL_OUBx 핸들러)
     - CString str = CString(exth->data) → SetEditData()
     - Wizard 스크린의 VBScript OnChange 등 이벤트를 동기 재귀 실행
 → (VBScript → axWizard/axisform 경유, WriteData/_Trigger 재귀 2~3회 중첩 — 스택에서 확인됨)
 → mfc140!CWnd::InvokeHelper (OLE 메서드 호출) → axis.exe 내부(axis+0xab76c→+0x13819e→+0x8f4c8→+0xdd6c8, 함수명 미상)
 → memcpy 크래시
```
**해석**: 자동전환바 타이머 → Wizard 스크린 전환 → 컨트롤 값 전달 → VBScript 이벤트가
여러 겹 재귀 실행되는 도중 axis.exe 내부 memcpy가 무효 포인터를 읽음. 재진입 깊이와
동기 SendMessage 중첩 구조 자체가 근본 리스크로 보임 (아래 "COM 객체 생명주기"/
"Worker/Timer 재진입" 항목과 일치).

**메모리 직접 확인 시도 — 한계**: 크래시 주소(0x2343e000)와 스택에 반복 등장하는
포인터값(0x2343dfc4) 부근을 `!address`/`db`로 조회했으나 **이 덤프에 해당 메모리 영역이
아예 캡처되어 있지 않음** ("info not present at the target"). 이 덤프가 User Mini Dump
(레지스터+스택+일부 메모리)라서 힙 영역은 대부분 빠짐. **향후 개선**: 덤프 생성 시
`MiniDumpWithFullMemory` 또는 최소 `MiniDumpWithPrivateReadWriteMemory` 플래그를 추가하면
이런 힙 포인터 문제를 실제로 들여다볼 수 있음 (현재 whdump.cpp 플래그는
MiniDumpNormal+WithDataSegs+WithHandleData+WithThreadInfo 뿐 — [[project_ib202200_crash]] 참고).

**⚠️ 중요 — PDB 불일치 발견**:
- `D:\symbol\axis.pdb`는 이 크래시의 axis.exe와 **매칭되지 않는 PDB**임 (cdb: `mismatched pdb`).
- 이 덤프의 axis.exe 시그니처: Timestamp `Thu Jun 11 12:59:18 2026` (`6A2A3296`), CheckSum `00483312`,
  요구 PDB 서명 `AXIS.pdb\743FE41DCE8F43E295BE04502CE0F28D1\AXIS.pdb`.
- `D:\symbol`은 symstore 트랜잭션 구조(`000Admin` 등)가 아니라 **평면 폴더**라서 과거 빌드의 PDB를
  GUID별로 보관/조회할 수 없음 — 이번 빌드에 해당하는 PDB가 유실된 상태로 추정.
- 그 결과 콜스택에서 axis.exe 자체 프레임(`axis+0xdd6c8`, `axis+0x8f4c8`, `axis+0x5a70`, `axis+0x890e1`,
  `axis+0x3550c`, `axis+0x5a70`, `axis+0x13819e`, `axis+0x183b88`)은 함수명/라인 번호를 알 수 없음.
  axWizard.ocx, CX_CODECTRL.dll, axSock.ocx, ChartCommonAddIn.dll도 동일하게 심볼 없음(export만 존재).
- **다음 크래시 분석의 정확도를 높이려면**: (1) symstore.exe로 빌드마다 GUID 버전 관리되는 진짜 심볼
  스토어를 구축할 것 ([[project_ib202200_crash]]에서 이미 필요성 확인됨, 아직 미착수), (2) 최소한
  배포 시점마다 axis.exe와 axis.pdb를 타임스탬프/체크섬과 함께 보관할 것.

### 사례 #2 — 2026-08-27, HTS 종료 시 cx_shared.dll 언로드 후 참조 (해결됨)

- **덤프 위치**: `C:\IBKS\Dumps\axis.exe.<PID>.dmp` (WER LocalDumps, Full Dump) — 2026-08-27 하루에만 09:28/09:29/10:06/11:06/13:11경 5건(PID 13428/13532/21688/23532/24452) 연속 발생, 사용자가 직접 `cdb.exe !analyze -v`로 그중 1건을 분석해 결과를 붙여넣음.
- **예외**: `BAD_INSTRUCTION_PTR` / `APPLICATION_FAULT_BAD_INSTRUCTION_PTR_SOFTWARE_NX_FAULT` — 실행 포인터가 **이미 언로드된 DLL의 코드 영역**으로 점프.
- **BUCKET_ID**: `APPLICATION_FAULT_BAD_INSTRUCTION_PTR_SOFTWARE_NX_FAULT_BAD_IP_axis!CAxisApp::ExitInstance+74`
- **증상**: HTS 종료(창 닫기/시스템 종료 명령) 시에만 발생. 조회/거래 등 정상 사용 중에는 재현되지 않음.

**콜스택 요약**:
```
AfxWinMain → CWinThread::Run (메시지 펌프)
 → DefFrameProcA → CFrameWnd::OnSysCommand (WM_SYSCOMMAND, 창닫기)
 → axis!CMainFrame::OnClose+0x1011
 → axis!CAxisApp::ExitInstance+0x74   (axis.cpp:692, m_instance->UnsetClass(...) 부근)
 → (윈도우 메시지 재귀 디스패치: ntdll!KiUserCallbackDispatcher → user32!__fnDWORD
    → DispatchClientMessage → UserCallWinProcCheckWow → user32!_InternalCallWinProc)
 → <Unloaded_cx_shared.dll>+0x3283   ← CRASH (언로드된 DLL 코드로 점프)
```

**원인 (소스 확인 완료)**:
1. `CMainFrame::initShared()`(MainFrm.cpp:32741~)가 시작 시 `cx_shared.dll`을 `LoadLibrary`하고, export 함수 `axCreate()`(`cx_shared.cpp:130`)를 호출해 **`WS_CHILD` 자식창**(`CMainWnd`, cx_shared.dll 안에 정의)을 메인프레임(`this`)의 자식으로 생성한다. 이 `CWnd*`는 `m_pSharedMemory`(MainFrm.h:1729)에 저장된다.
2. `CMainFrame::OnClose()`(MainFrm.cpp:3177~3181, 수정 전)는 종료 처리 맨 끝, `CMDIFrameWnd::OnClose()`를 호출하기 직전에 **`m_pSharedMemory`(자식창)를 전혀 건드리지 않은 채 `FreeLibrary(m_hSharedLib)`만 호출**했다 — 즉 `cx_shared.dll`의 코드 페이지가 프로세스 주소공간에서 즉시 해제되는데, 그 코드 안에 WNDPROC/vtable이 있는 자식창(`m_pSharedMemory`)은 **여전히 살아있고 메인프레임의 자식으로 붙어있는 상태**였다.
3. 바로 다음 줄의 `CMDIFrameWnd::OnClose()` 호출이 메인프레임의 `DestroyWindow()`로 이어지고(또는 `CAxisApp::ExitInstance()`의 후속 정리 과정에서), Windows가 자식창 파괴를 연쇄적으로 처리하며 그 자식창(`m_pSharedMemory`)에 메시지(`WM_DESTROY`류)를 보낸다. 하지만 그 WNDPROC 코드는 방금 `FreeLibrary`로 언매핑된 메모리라 — `user32!_InternalCallWinProc`가 그 주소로 점프하는 순간 `BAD_INSTRUCTION_PTR`로 죽는다.
4. `CMainWnd`(cx_shared.dll)에 `PostNcDestroy()` 오버라이드가 없어 자기소멸(`delete this`)도 하지 않으므로, 애초에 이 객체/창을 정리하는 코드 자체가 프로젝트 전체에 존재하지 않았다(`m_pSharedMemory`는 `axCreate()` 호출 지점 외엔 null 대입/delete가 전혀 없었음, `MainFrm.cpp` 전수 grep으로 확인).

**수정 (2026-08-27, `MainFrm.cpp::OnClose`, `FreeLibrary(m_hSharedLib)` 직전에 추가)**:
```cpp
if (m_pSharedMemory)
{
	m_pSharedMemory->DestroyWindow();
	delete m_pSharedMemory;
	m_pSharedMemory = nullptr;
}

if (m_hSharedLib)
{
	FreeLibrary(m_hSharedLib);
	m_hSharedLib = nullptr;
}
```
`DestroyWindow()`(자식창을 부모로부터 분리 + WM_DESTROY 처리, 이 시점엔 `cx_shared.dll`이 아직 로드돼 있어 정상 실행됨) → `delete`(C++ 소멸자 호출, 역시 아직 로드된 상태라 안전) → 그 다음에야 `FreeLibrary`. 순서를 지키면 `FreeLibrary` 시점에는 그 DLL 코드를 참조하는 창/객체가 프로세스에 하나도 남지 않으므로, 이후 어떤 경로로 메시지가 재귀되든 언로드된 메모리로 점프할 길이 없다.

**교훈 — 외부 DLL을 자식창으로 심는 패턴의 종료 순서 원칙**: `LoadLibrary` + `axCreate`류 팩토리로 만든 자식창은, 그 창이 "누구의 코드로 그려지는지"(WNDPROC/vtable의 소속 모듈)가 부모 코드 입장에서는 안 보이는 블랙박스다. **자식창을 만든 DLL을 언로드하기 전에는 반드시 그 창을 먼저 `DestroyWindow`(+필요시 `delete`)해야 한다** — "어차피 부모창이 곧 파괴되니 자식도 같이 정리되겠지"라고 순서를 맡기면, "부모 파괴 연쇄가 자식에 메시지를 보내는 시점"과 "DLL이 이미 언로드된 시점"의 순서가 뒤바뀔 수 있다. 이번 사례는 정확히 그 뒤바뀜이 원인이었다. `WizardArchitecture.md`의 `CDll`(외부 DLL 로드형 작업영역, 동일한 `axCreate`류 패턴)이나 유사한 플러그인식 자식창을 쓰는 다른 코드(`CX_MarketPicker` 등도 `axCreate` 패턴 사용 확인됨)에도 같은 리스크가 있을 수 있어 점검 가치 있음(미착수).

**재현 조건 참고**: 이 시점까지 5건 모두 오늘(2026-08-27) 하루에 몰려 발생 — cx_shared.dll이 정상적으로 `axCreate`까지 성공한 세션에서 HTS를 종료할 때마다 사실상 결정적(deterministic)으로 재현되는 것으로 보임(경쟁조건이 아니라 항상 이 순서로 실행되는 코드 경로 버그였기 때문). 수정 후 재발 여부는 다음 종료 테스트로 확인 필요.

**추가 조치 (2026-08-27, `cx_shared.dll::CMainWnd`) — 미사용 워커스레드 비활성화:**
`CMainWnd`는 `std::thread m_workerThread`를 멤버로 갖고 `OnCreate()`에서 `Axis_IsMainRTS()`일 때 `StartWorkerThread()`로 기동시키는 코드가 있었다. 이 스레드가 살아있는 상태에서 `CMainWnd` 객체가 소멸되면(`std::thread`가 `joinable()`인 채로 소멸자에 들어가면) `std::terminate()`가 호출되어 위 크래시와는 별개의 즉시 프로세스 종료가 발생할 수 있는 잠재 위험이 있었다. 사용자 확인 결과 이 워커스레드 기능은 테스트 코드로 실사용하지 않을 것이라 판단해, `OnCreate()`의 `StartWorkerThread()` 호출(사용자가 직접 주석 처리)과 필자가 안전장치로 추가했던 `OnDestroy()`의 `StopWorkerThread()` 호출을 **둘 다 주석 처리**해 기능 자체를 비활성화했다(짝을 맞춤 — 시작하지 않는 스레드를 정지시키는 코드만 남겨둘 이유가 없음).

---

### 사례 #3 — 2026-08-27, AxisCodx.dll 언로드 후 참조 (해결됨)

- **덤프 위치**: 사례 #2와 같은 배치(`C:\IBKS\Dumps`)의 5건 중 가장 최근(13:11경, PID 24452) — 나머지 4건은 전부 cx_shared.dll(사례 #2)이었으나 이 1건만 다른 모듈이 원인으로 확인됨.
- **예외**: 사례 #2와 동일한 `BAD_INSTRUCTION_PTR`/`APPLICATION_FAULT_BAD_INSTRUCTION_PTR_SOFTWARE_NX_FAULT` 계열 — 언로드된 DLL 코드로 점프.
- **크래시 모듈 확정**: 폴트 주소 `0x72a53283` = `AxisCodx.dll`의 언로드 시점 베이스 `0x72a40000` + 오프셋 `0x13283`. `AxisCode.dll`(e, 별개 모듈)의 베이스 범위(`0x72a80000~0x72b9e000`)와는 겹치지 않아, "AxisCode"가 아니라 **"AxisCodx"(x)**로 확정됨.

**콜스택 요약** (사례 #2와 달리 mfc140 공개 심볼로 완전히 해석됨, PDB 불일치 없음):
```
AfxWinMain → CWinThread::Run (메시지 펌프)
 → WM_SYSCOMMAND → ... → axis!CMainFrame::OnClose+0x1011
 → mfc140!CFrameWnd::OnClose+0x1f9
 → mfc140!CWnd::DestroyWindow+0x68
 → (윈도우 메시지 재귀 디스패치, 사례 #2와 동일 패턴)
 → <Unloaded_AxisCodx.dll>+0x13283   ← CRASH
```

**원인 (소스 확인 완료, 사례 #2와는 다른 메커니즘)**:
1. `ibks/dll/form/axform.cpp`의 `CAxisForm`(모든 Wizard 화면·서브맵의 기반 클래스, `CScreen : public CAxisForm`)이 **생성자에서 `AxisCodx.dll`을 개별적으로 `AfxLoadLibrary`하고, 소멸자에서 개별적으로 `AfxFreeLibrary`했다.** `m_hCode`/`axGetCategory`/`axGetSpinData`/`axGetDelta`가 전부 **인스턴스 멤버**였다 — 즉 화면(메인화면+서브맵) 하나가 열릴 때마다 이 DLL을 각자 로드하고, 닫힐 때 각자 해제하는 구조였다.
2. `AfxLoadLibrary`/`FreeLibrary`는 프로세스 전역 참조카운트 방식이라 다른 화면이 참조를 들고 있으면 실제 언로드는 안 되지만, **HTS 종료 시 열려있던 모든 화면이 한꺼번에(연쇄적으로) 파괴되는 구간**에서, 어느 한 화면(A)의 소멸자가 우연히 마지막 참조를 놓아 DLL이 실제로 언로드되는 바로 그 타이밍에, **아직 파괴 처리 중인 다른 화면(B)**의 컨트롤(예: 스핀버튼 `CfmEdit`, `GetSpinData()`/`GetCode()`/`GetDelta()` 경유)이 **자기 자신의(이미 무효해진 주소를 가리키는) 함수포인터를 통해 호출**을 시도하면, 그 주소가 이미 언매핑된 메모리라 크래시한다.
3. `lmu`(언로드모듈 목록)에서 `AxisCodx.dll`이 서로 다른 베이스 주소로 두 번 나타난 것도, 여러 화면이 세션 도중 각자 독립적으로 로드/해제를 반복했다는 것과 일치한다(cx_shared.dll처럼 세션당 1회만 로드되는 패턴과 대비됨). `Wizard/Guard.cpp:409-418`에도 `AxisCodx.dll`을 로드하는 코드가 있으나, 이건 세션 시작 시 1회성으로 로드 후 같은 함수 안에서 즉시 `AfxFreeLibrary`하는 구조라 이번 종료-시점 크래시와는 무관함을 확인.

**수정 (2026-08-27, `ibks/dll/form/axform.h`, `axform.cpp`)** — 인스턴스별 로드/해제를 **static 공유 + 참조카운트**로 변경:
```cpp
// axform.h (private)
static HINSTANCE	m_hCode;
static bool		(APIENTRY* axGetCategory)(char*, int, int, char*, bool);
static int		(APIENTRY* axGetSpinData)(DWORD, int, int, char*, double, bool);
static double		(APIENTRY* axGetDelta)(DWORD, double, bool);
static int		m_hCodeRefCount;
```
```cpp
// axform.cpp, 생성자
if (m_hCodeRefCount == 0)
{
	m_hCode = AfxLoadLibrary("AxisCodx.dll");
	// ... GetProcAddress 3개 (기존과 동일)
}
m_hCodeRefCount++;

// 소멸자
if (--m_hCodeRefCount == 0 && m_hCode)
{
	AfxFreeLibrary(m_hCode);
	m_hCode = NULL;
	axGetCategory = NULL; axGetSpinData = NULL; axGetDelta = NULL;
}
```
화면이 하나라도 살아있는 한 참조카운트가 0이 되지 않으므로 DLL이 계속 로드 상태로 유지되고, 진짜 마지막 화면이 파괴될 때만 해제된다 — "다른 화면이 아직 쓰는 중인데 내가 마지막 참조를 놓아버리는" 경합이 원천적으로 사라진다.

**재배포 필수사항**: `CAxisForm`의 인스턴스 크기(레이아웃)가 바뀌므로, `CScreen`(axWizard.ocx/Wizard.dll)이 이 클래스를 상속하는 이상 **`axisform.dll`과 `axWizard.ocx`를 반드시 함께 Clean+Rebuild하여 함께 재배포**해야 한다(부분빌드 시 두 모듈의 `CAxisForm` 레이아웃 인식이 어긋나 새로운 크래시가 날 수 있음 — `AxisformArchitecture.md` 8절 규칙과 동일한 이유).

**교훈 — 사례 #2와의 대비**: 사례 #2(cx_shared.dll)는 "창 하나의 파괴 순서"가 틀린 문제였지만, 이번은 "여러 인스턴스가 참조카운트된 자원 하나를 공유"하는데 **그 자원의 로드/해제 단위(화면별)가 실제 공유 단위(프로세스 전체)와 어긋나 있던** 문제다. `AfxLoadLibrary`/`FreeLibrary`가 참조카운트를 관리해준다고 해서 "여러 인스턴스가 각자 로드/해제해도 안전하다"고 가정하면 안 된다 — 카운트가 정확히 0이 되는 순간 실제 언로드가 일어나고, 그 시점이 "마지막으로 그 자원을 쓸 코드가 다 정리된 시점"과 정확히 일치한다는 보장이 없으면(특히 여러 객체가 동시다발적으로 파괴되는 종료 시퀀스에서) 이번과 같은 경합이 생긴다. 이런 패턴을 쓰는 다른 플러그인/헬퍼 DLL이 더 있는지(`lmu`에서 확인된 ~35개 동적 로드 DLL 목록 중 점검 가치 있는 것들) 점검 가치 있음(미착수).

**재현 조건 참고**: 종합화면처럼 화면(서브맵 포함)을 여러 개 동시에 띄운 세션에서 HTS를 종료할 때 재현 가능성이 높은 것으로 추정됨(경쟁 조건이므로 사례 #2처럼 결정적이지는 않을 수 있음). 수정 후 재발 여부는 다음 종료 테스트로 확인 필요.

## 기록 목록

| 날짜 | 항목 | 상태 |
|------|------|------|
| 2026-07-14 | AXIS 아키텍처 분석 완료 (architecture Agent) | 완료 |
| 2026-07-14 | Crash 분석 체크리스트 작성 | 완료 |
| 2026-07-14 | COM 초기화 함수 오기재(CoInitializeEx→CoInitialize) 정정 | 완료 |
| 2026-07-14 | 크래시 사례 #1 분석 (khs779_71410h32m27s.dmp) — memcpy AV, axis.pdb 불일치 확인 | 완료 (axis.exe 함수명은 미상) |
| 2026-07-14 | 사례 #1 사용자 WinDbg GUI 교차검증 — 동일 결과(khs779_71410h32m27s.windbg_log.txt) | 완료 |
| 2026-08-27 | 크래시 사례 #2 분석+수정 (HTS 종료 시 cx_shared.dll 언로드 후 참조, BAD_INSTRUCTION_PTR) — MainFrm.cpp::OnClose에 DestroyWindow 순서 수정 + CMainWnd 미사용 워커스레드 비활성화 | 완료 (재현 테스트 대기) |
| 2026-08-27 | 크래시 사례 #3 분석+수정 (HTS 종료 시 AxisCodx.dll 언로드 후 참조, BAD_INSTRUCTION_PTR) — CAxisForm의 인스턴스별 로드/해제를 static 참조카운트 공유로 변경 (ibks/dll/form/axform.h·cpp) | 완료 (axisform.dll+axWizard.ocx 재빌드/재배포 및 재현 테스트 대기) |
| - | MainFrame 멤버(m_mapHelper/m_axMisc/m_axGuide) 실제 생성/소멸 위치 검증 | 추후 검증 |
| - | Grid Excel Export Thread 안전성 테스트 | 추후 검증 |
| - | axis.exe/axis.pdb symstore GUID 버전관리 체계 구축 | 미착수 |
