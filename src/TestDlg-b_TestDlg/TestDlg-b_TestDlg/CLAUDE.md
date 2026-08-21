# CLAUDE.md — TestDlg-b_TestDlg


## 목차

- [빌드 & 실행](#빌드-실행)
- [아키텍처](#아키텍처)
  - [핵심 모듈](#핵심-모듈)
  - [스레드 흐름 (Production_td 경로)](#스레드-흐름-production_td-경로)
  - [Race Condition 데모](#race-condition-데모)
  - [CPU 친화도](#cpu-친화도)
  - [MiniDump (whdump)](#minidump-whdump)
- [주요 매크로 / 분기](#주요-매크로-분기)
- [주의사항](#주의사항)

---

MFC C++ 멀티스레드 실험/학습용 테스트 프로그램.
Race Condition 재현, CPU 친화도 제어, MiniDump 생성 등을 실습하는 Sandbox.

## 빌드 & 실행

```
솔루션: TestDlg.sln
구성:   Debug|x64 (또는 Release|x64)
실행:   TestDlg.exe  (레지스트리 등 외부 의존 없음)
```

## 아키텍처

### 핵심 모듈

| 파일 | 역할 |
|------|------|
| `whdump.cpp/h` | MiniDumper — `SetUnhandledExceptionFilter` + `MiniDumpWriteDump`; 덤프는 `exe위치\dump\` 에 저장 |
| `CDlg_Thread.cpp/h` | 메인 스레드 다이얼로그; Production_td 워커 스레드 관리, CPU 친화도 제어, Race Demo 진입점 |
| `CThreadWnd.cpp/h` | 데이터 수신·렌더링용 자식 윈도우 (최대 4개); `Dispatch()` 로 데이터 수신 |
| `CTD_test.cpp/h` | `CWinThread` 상속 Race 데모 스레드 (TD_WRITER / TD_READER) |
| `CProducer_TD.cpp/h` | CThreadWnd 에 연결된 프로듀서 스레드 |
| `CSRWSR.cpp/h` | Slim Reader/Writer Lock 래퍼 |
| `cs_TLS.cpp/h` | Thread Local Storage 유틸 |

### 스레드 흐름 (Production_td 경로)

```
CDlg_Thread::OnBnClickedBtnTest()
  └─ AfxBeginThread(Production_td)   // 워커 스레드 시작
       └─ while(m_balive)
            └─ SendDataToThWnd()
                 └─ CThreadWnd[0..n]::Dispatch()  // 각 윈도우에 전달
```

- 종료: `m_balive = false` → 워커가 `SelfSetEvent()` → `m_event_TrdDlg` 시그널
- `WaitForSingleObject(m_event_TrdDlg, INFINITE)` 로 종료 대기 (5초 타이머 fallback: ID 9898)

### Race Condition 데모

| 버튼 | 함수 | 동작 |
|------|------|------|
| BTN_TEST3 | `StartRaceDemo(false)` | Lock 없음 → CArray 재할당 중 크래시 유발 |
| BTN_TEST4 | `StartRaceDemo(true)` | CCriticalSection 보호 → 안전 |

- `CTD_test::Run()` 에서 TD_WRITER: `Add + RemoveAll(>1000)`, TD_READER: `GetAt(n-1)`
- Lock 없을 때 Writer의 `RemoveAll` 도중 Reader가 `GetAt` → AV 크래시

### CPU 친화도

- `Affinity.ini` (exe 옆) 에 `[PROCESS] CORE=1;2;3` / `[THREAD] CORE=1;2` 형식으로 저장
- 체크박스 IDC_CHECK1~8: 프로세스 코어, IDC_CHECK9~16: 스레드 코어

### MiniDump (whdump)

- `MiniDumper` 생성자에서 `SetUnhandledExceptionFilter` 등록
- 덤프 타입: `MiniDumpNormal | WithDataSegs | WithHandleData | WithThreadInfo | WithUnloadedModules | WithCodeSegs`
- 저장 경로: `<exe폴더>\dump\<월일>_<시분초>.dmp`

## 주요 매크로 / 분기

| 매크로 | 위치 | 의미 |
|--------|------|------|
| `DF_USETHREAD` | CDlg_Thread.h | 스레드 관련 코드 활성화 (기본 ON) |
| `DF_memory1` | CDlg_Thread.cpp | ON: 스택 버퍼 재사용, OFF: heap new/delete |
| `DF_memory11` | CDlg_Thread.cpp | 현재 활성 분기 표시용 (실제 분기 아님) |

## 주의사항

- `m_event_TrdDlg` 이벤트는 `AfxBeginThread` **이후** `CreateEvent`로 생성됨  
  → 워커가 먼저 `SelfSetEvent` 호출하면 이벤트 핸들이 아직 없어 레이스 발생 가능
- `OnBnClickedBtnTest` 내 `WaitForSingleObject(INFINITE)` 는 UI 스레드를 블록함  
  → 타이머(5초) fallback으로 강제 해제
- `SendDataToThWnd` 내 Lock(`m_cs`) 은 현재 주석 처리 상태 — Race 실험 중
- 영문 주석만 사용 (한글/유니코드 주석 → 컴파일 에러)
