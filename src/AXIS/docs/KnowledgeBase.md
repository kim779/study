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

## 기록 목록

| 날짜 | 항목 | 상태 |
|------|------|------|
| 2026-07-14 | AXIS 아키텍처 분석 완료 (architecture Agent) | 완료 |
| 2026-07-14 | Crash 분석 체크리스트 작성 | 완료 |
| 2026-07-14 | COM 초기화 함수 오기재(CoInitializeEx→CoInitialize) 정정 | 완료 |
| 2026-07-14 | 크래시 사례 #1 분석 (khs779_71410h32m27s.dmp) — memcpy AV, axis.pdb 불일치 확인 | 완료 (axis.exe 함수명은 미상) |
| 2026-07-14 | 사례 #1 사용자 WinDbg GUI 교차검증 — 동일 결과(khs779_71410h32m27s.windbg_log.txt) | 완료 |
| - | MainFrame 멤버(m_mapHelper/m_axMisc/m_axGuide) 실제 생성/소멸 위치 검증 | 추후 검증 |
| - | Grid Excel Export Thread 안전성 테스트 | 추후 검증 |
| - | axis.exe/axis.pdb symstore GUID 버전관리 체계 구축 | 미착수 |
