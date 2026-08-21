# axwizard(Wizard) 소스 분석


## 목차

- [문서 목적](#문서-목적)
- [1. 클래스 계층 구조](#1-클래스-계층-구조)
- [2. 초기화/연결 흐름](#2-초기화연결-흐름)
- [3. CScreen::Parse() 내 스크립트 엔진 초기화 순서 (Screen.cpp:198~)](#3-cscreenparse-내-스크립트-엔진-초기화-순서-screencpp198)
- [4. 이벤트 → 스크립트 프로시저 이름 매핑 (Script.cpp::getProcName)](#4-이벤트-스크립트-프로시저-이름-매핑-scriptcppgetprocname)
- [5. dll/vbs/engineWrapper.cpp 현재 구현 (2026-07-13 코드 확인, 문서 드리프트 주의)](#5-dllvbsenginewrappercpp-현재-구현-2026-07-13-코드-확인-문서-드리프트-주의)
- [6. 관련 파일](#6-관련-파일)
- [7. 전체 클래스 레퍼런스 (2026-07-25, `Wizard/` 폴더 57개 파일 전수 확인)](#7-전체-클래스-레퍼런스-2026-07-25-wizard-폴더-57개-파일-전수-확인)
  - [7.1 진입점 / ActiveX 컨트롤 계층](#71-진입점-activex-컨트롤-계층)
  - [7.2 세션 / 작업영역 관리 계층](#72-세션-작업영역-관리-계층)
  - [7.3 화면 / 스크립트 노출 객체 계층](#73-화면-스크립트-노출-객체-계층)
  - [7.4 이벤트 / 입력 처리 계층](#74-이벤트-입력-처리-계층)
  - [7.5 통신 / 외부 연동 계층](#75-통신-외부-연동-계층)
  - [7.6 부가 UI / 유틸리티 계층](#76-부가-ui-유틸리티-계층)
  - [7.7 작은 헬퍼/데이터 구조체 (Misc.h)](#77-작은-헬퍼데이터-구조체-misch)
- [8. 다음 분석 대상 (미착수)](#8-다음-분석-대상-미착수)
- [9. 관련 문서](#9-관련-문서)

---

## 문서 목적

HTS 핵심 구성요소인 `ibks/Wizard/` (axwizard, ActiveX 컨트롤 DLL) 자체의 구조를 분석/이해합니다.
`@docs/Architecture.md`가 Python 엔진 전환 프로젝트 관점의 3계층 개요라면, 이 문서는 axwizard 내부 클래스 계층과 이벤트 흐름을 더 상세히 다룹니다.

---

## 1. 클래스 계층 구조

```
CWizardApp (Wizard.cpp)
  └ COleControlModule — DLL 진입점, ActiveX 등록(DllRegisterServer/DllUnregisterServer)

CWizardCtrl (WizardCtrl.h/cpp)
  └ COleControl 상속 — 실제 ActiveX 컨트롤 (호스트 EXE가 생성하는 객체)
      · Dispatch: RunAxis, axTR, axAttach/axDetach, axWorkShop, axWorkName, axWorkView, axWizard, axInvoke
      · Event: OnFire (호스트 EXE로 이벤트 전달, FireEvent(eventidOnFire, ...))
      · OnCreate() 시 CGuard 생성 + m_guard->Initial(this)

CGuard (Guard.h/cpp) — 세션 전체를 관장하는 허브(god object)
  · 통신: m_sock(소켓 OCX 핸들), Write/Login/Service/Approve/UploadFile/DownloadFile
  · 보안: Xecure/Certify/Secure/Secure2 (암호화, 인증서)
  · 작업 관리: m_clients (CMap<int,int,CWorks*,CWorks*>, key→작업영역), Attach()/Detach()/GetClient()
  · 부가 객체: m_objects(CObjects*), m_dde(CDde*), m_xsystem(CxSystem*), m_tips(CTips*), m_palette(CAxisPalette*)
  · 계정/코드마스터: m_account, m_master, GetCode/SetCode 계열

CClient : CWorks (Client.h/cpp) — 작업영역 1개 (윈도우/탭 단위)
  · m_screen (CMap<int,int,CScreen*,CScreen*>) — 이 작업영역에 로드된 화면들
  · m_vm (CScript*) — 이벤트→스크립트 프로시저 브릿지
  · m_keyx(CKey*), m_mouse(CMouse*), m_stream(CStream*)
  · 이벤트 라우팅 진입점: OnMouse/OnKey/OnChange/OnTRAN/OnAlert/OnNotice/OnFocus 등

CScreen : CAxisForm (Screen.h/cpp) — 화면(.map) 1개 인스턴스
  · m_vbe (CEngineWrapper*) — 스크립트 엔진 래퍼 (VBS/Python, dll/vbs/engineWrapper.h)
  · m_xscreen (CxScreen*) — 스크립트에 "Screen" 이름으로 노출되는 화면 객체 모델
  · m_client, m_guard — 상위 참조
  · Attach()/Parse() — .map 로드, 컨트롤 생성, 스크립트 엔진 초기화 (아래 3절 참고)
  · Flash/Blink/Notice/Profit(손익) 처리, RTM(실시간) 갱신(UpdateRTM)

CScript (Script.h/cpp) — C++ 이벤트 ↔ 스크립트 프로시저 브릿지 (상태 없음, CClient::m_vm)
  · evStart/evClick/evChange/evTimer/evReceive 등 이벤트 상수 (evXXX = 1~15, evClick 이후 100~108은 컨트롤 이벤트)
  · getProcName(event, symbol) — 이벤트를 프로시저명 문자열로 매핑 (4절 참고)
  · On*(screen, form, ...) 각 함수: IsAvailable() 체크 후 screen->m_vbe->DoProcedure() 호출
```

---

## 2. 초기화/연결 흐름

```
호스트 EXE (axisbuilder로 만든 HTS)
  1. CWizardCtrl 인스턴스 생성 → OnCreate() → CGuard 생성 + Initial()
  2. axAttach(view, type, key) → CGuard::Attach() → 키별 CClient 생성 (m_clients에 등록)
  3. axWorkShop(key, maps, size, only) → CWorks::Attach(maps) → CScreen 생성 → CScreen::Parse()
       (Parse() 안에서 컨트롤 파싱 + 스크립트 엔진 초기화, 3절 참고)
  4. RunAxis(mode, ...) → 로그인/TR 세션 시작 (m_guard->Login/Startup, 소켓 OCX로 서버 연결)
```

이후 사용자 조작(클릭/키/타이머) 또는 서버 응답(TR)이 발생하면:

```
Windows 메시지 / TR 수신
  → CClient의 On* 핸들러 (OnMouse/OnKey/OnTRAN/OnAlert 등)
  → CScript::On*(screen, form) 호출
      → getProcName(event, symbol)로 프로시저명 결정
      → screen->m_vbe->IsAvailable(procs) 로 존재 확인
      → screen->m_vbe->DoProcedure(procs, ...) 로 실제 스크립트 실행
```

---

## 3. CScreen::Parse() 내 스크립트 엔진 초기화 순서 (Screen.cpp:198~)

```cpp
m_vbe->Initialize(mapN);                                  // 엔진 래퍼 초기화 (아직 VBS/Python 미결정)
m_xscreen = new CxScreen(this);
m_vbe->AddObject("Screen", m_xscreen, SCRIPTITEM_GLOBALMEMBERS);
m_vbe->AddObject("System", m_guard->m_xsystem);
if (m_guard->m_login)  m_vbe->AddObject("Login", m_guard->m_login);
if (m_ledger)           m_vbe->AddObject("Ledger", m_ledger);
// publicR 목록의 이름들 → m_client->GetControl(name)으로 찾아 AddObject
// ... 폼(FM_OBJECT/FM_BUTTON/FM_EDIT/FM_GRID/...) 순회하며 각 컨트롤을 심볼명으로 AddObject
// scriptR 섹션이 있으면:
m_vbe->LoadScript(text, m_mapH->pythonMode);               // 여기서 비로소 VBS/Python 엔진 확정+생성
```

**핵심:** `AddObject()`는 `LoadScript()`보다 먼저 여러 번 호출되지만, 이 시점엔 아직 VBS 엔진인지 Python 엔진인지 결정되지 않은 상태다. `CEngineWrapper`는 이를 `PendingObject` 벡터에 버퍼링해뒀다가, `LoadScript()`에서 엔진 종류가 확정되는 순간(`ensureEngine()`) 일괄 등록한다. (`@docs/python_engine_260608.md`에 기록된 "INFO 객체 오류" 버그의 근본 해결책과 동일 메커니즘 — 현재도 유지되고 있음을 코드로 확인.)

---

## 4. 이벤트 → 스크립트 프로시저 이름 매핑 (Script.cpp::getProcName)

맵소스(VBS/Python) 안에서 정의해야 하는 함수명은 다음 규칙을 따른다.

**화면 단위 이벤트 (심볼 없음):**

| 이벤트 | 프로시저명 |
|---|---|
| evStart | `AX_SUB_OnStart_AX_` |
| evFocus | `AX_SUB_OnFocus_AX_` |
| evClose | `AX_SUB_OnClose_AX_` |
| evSend | `AX_SUB_OnSend_AX_` |
| evReceive | `AX_SUB_OnReceive_AX_` |
| evService | `AX_SUB_OnService_AX_` |
| evApprove | `AX_SUB_OnApprove_AX_` |
| evDevice | `AX_SUB_OnDevice_AX_` |
| evTimer | `AX_SUB_OnTimer_AX_` |
| evTimerX | `AX_SUB_OnTimerX_AX_` |
| evSelect | `AX_SUB_OnSelect_AX_` |
| evFile | `AX_SUB_OnFile_AX_` |
| evAlert | `AX_SUB_OnAlert_AX_` |
| evSize | `AX_SUB_OnSize_AX_` |
| evKey (심볼 없을 때) | `AX_SUB_OnKey_AX_` |

**컨트롤 단위 이벤트 (심볼 = 컨트롤명, 맵소스에서 정의한 이름):**

| 이벤트 | 프로시저명 패턴 |
|---|---|
| evKey (심볼 있을 때) | `AX_%s_OnKey_AX_` |
| evClick | `AX_%s_OnClick_AX_` |
| evChange | `AX_%s_OnChange_AX_` |
| evDblClk | `AX_%s_OnDblClick_AX_` |
| evMouseDn | `AX_%s_OnMouseDown_AX_` |
| evMouseUp | `AX_%s_OnMouseUp_AX_` |
| evChar | `AX_%s_OnCharChange_AX_` |
| evSetFocus | `AX_%s_OnSetFocus_AX_` |
| evMouseEnter | `AX_%s_OnMouseEnter_AX_` |
| evMouseLeave | `AX_%s_OnMouseLeave_AX_` |

예: `BUTTON0` 컨트롤의 클릭 이벤트 → 함수명 `AX_BUTTON0_OnClick_AX_` (VBS `Sub`/Python `def` 모두 이 이름으로 정의).

---

## 5. dll/vbs/engineWrapper.cpp 현재 구현 (2026-07-13 코드 확인, 문서 드리프트 주의)

`@docs/python_engine_260608.md` 작성 당시(2026-06-08~11) 설계와 **현재 코드가 달라진 부분**이 있어 별도 기록한다 (docs는 병합 원칙이라 KnowledgeBase.md 12절에도 동일 내용 기록).

- **엔진 선택이 이제 2단계다.** `CScreen::Parse()`가 `m_vbe->LoadScript(text, m_mapH->pythonMode)`로 **빌드 시점에 확정된 `pythonMode` 플래그**(axisbuilder의 mapload.cpp가 빌드 시 자동감지, 9단계 작업)를 명시적으로 전달한다. `engineWrapper.cpp::LoadScript(scripts, scpKind)`는 `scpKind != -1`이면 이 명시적 값을 그대로 쓰고, `scpKind == -1`일 때만 텍스트 스캔 폴백(`isPythonScript()`)을 쓴다.
- **텍스트 스캔 폴백도 더 엄격해졌다.** 기존 문서는 `scripts.Find("def ") >= 0`(텍스트 어디든 포함)였지만, 현재 `isPythonScript()`는 **줄 단위로 순회하며 각 줄의 선행 공백을 제거한 뒤 줄의 맨 앞이 `def ` 또는 `import `로 시작하는지**를 검사한다. 문자열/주석 안에 우연히 "import"가 들어간 경우의 오탐을 줄이는 방향으로 개선된 것으로 보임.
- **재진입(reentrancy) 방어 코드가 보이지 않는다.** KnowledgeBase.md에 기록된 버그#5(PyDict_GetItem 크래시)의 해결책이었던 `m_inException` 플래그가 현재 `pythonEngine.h`에 없다. `engineWrapper.cpp::IsAvailable()`의 주석에 "Nested calls ... are safe in CPython on the same thread, so no re-entrancy guard is needed here"라고 명시되어 있어 **의도적으로 제거된 것으로 보이나, 실제 크래시 재현 테스트로 재검증되지는 않음** — 실운영 테스트(python_engine 문서 10단계) 시 최우선 확인 대상으로 남겨둔다.

---

## 6. 관련 파일

| 파일 | 역할 |
|------|------|
| `Wizard/WizardCtrl.h/cpp` | ActiveX 컨트롤 진입점, Dispatch/Event 맵 |
| `Wizard/Guard.h/cpp` | 세션 허브 (통신/보안/작업관리) |
| `Wizard/Client.h/cpp` | 작업영역(CClient), 이벤트 라우팅 |
| `Wizard/Screen.h/cpp` | 화면 인스턴스, 스크립트 엔진 초기화 (Parse()) |
| `Wizard/Script.h/cpp` | 이벤트→프로시저명 매핑, DoProcedure 호출 |
| `Wizard/xscreen.h/cpp` | 스크립트에 노출되는 Screen 객체 모델 |
| `dll/vbs/engineWrapper.h/cpp` | VBS/Python 엔진 선택·위임 |
| `dll/vbs/pythonEngine.h/cpp` | Python 엔진 (Python C API) |
| `dll/vbs/scriptEngine.h/cpp` | VBS 엔진 (Windows Script Host) |

---

## 7. 전체 클래스 레퍼런스 (2026-07-25, `Wizard/` 폴더 57개 파일 전수 확인)

`Wizard/` 폴더의 모든 클래스를 역할별로 정리. 1~5절에서 이미 상세히 다룬 6개 핵심 클래스(CWizardApp, CWizardCtrl, CGuard, CClient, CScreen, CScript)는 표에서 파일 위치만 표시하고 설명은 생략 — 위 절 참고.

### 7.1 진입점 / ActiveX 컨트롤 계층

| 클래스 | 파일 | 역할 |
|---|---|---|
| `CWizardApp` | Wizard.h/cpp | DLL 진입점 (1절 참고) |
| `CWizardCtrl` | WizardCtrl.h/cpp | ActiveX 컨트롤 본체 (1절 참고) |
| `CWizardPropPage : COlePropertyPage` | WizardPropPage.h/cpp | 디자인타임 속성 페이지 — MFC ActiveX 마법사가 생성한 보일러플레이트, 커스텀 로직 없음 |

### 7.2 세션 / 작업영역 관리 계층

| 클래스 | 파일 | 역할 |
|---|---|---|
| `CGuard` | Guard.h/cpp | 세션 허브 (1절 참고) |
| `CWorks` | Works.h/cpp | 작업영역의 **추상 기반 클래스**. `CClient`/`CDll`이 상속. `S_*` 상태 플래그(S_LOAD, S_FLASH, S_LOCK 등) 정의, `Attach`/`OnAxis`/`OnAlert`/`OnDomino` 등 가상함수로 공통 인터페이스 제공. `m_drop`(COleDrop)로 OLE 드래그앤드롭도 여기서 관리 |
| `CClient : CWorks` | Client.h/cpp | 실제 화면 작업영역 (1절 참고) |
| `CDll : CWorks` | Dll.h/cpp | **DLL 기반 작업영역** — CClient(화면 기반)의 대안 경로로 보임. `m_dll`(별도 로드된 DLL 핸들), `m_screens`(CMapWordToPtr)로 자체 화면 목록 관리. `LoadLibrary`로 외부 DLL을 동적 로드해서 작업영역처럼 다룸(`Attach`가 `HINSTANCE m_instance` 사용) — 용도는 미조사 (예: 특정 화면군을 별도 DLL로 배포하는 확장 메커니즘 추정) |

### 7.3 화면 / 스크립트 노출 객체 계층

| 클래스 | 파일 | 역할 |
|---|---|---|
| `CScreen : CAxisForm` | Screen.h/cpp | 화면 인스턴스 (1, 3절 참고) |
| `CxScreen : CCmdTarget` | xscreen.h/cpp | 스크립트에서 `Screen` 이름으로 보이는 **COM 디스패치 래퍼**. `_SetData`류가 아니라 `_ChangeTR`/`_Send`/`_Service`/`_CreateObject`/`_SetTimer`/`_SetTimerX`/`_GetCode`/`_Print`/`_UploadFile`/`_DownloadFile` 등 **60개 이상의 dispatch 메서드**로 화면 제어 전체 노출. `CScreen` 1개당 1개 생성(`Screen.cpp:223`, `m_xscreen = new CxScreen(this)`) |
| `CxSystem : CCmdTarget` | xsystem.h/cpp | 스크립트에서 `System` 이름으로 보이는 COM 디스패치 래퍼. `_GetUserID`/`_Trigger`/`_Encrypt`/`_Decrypt`/`_Menu`/`_PlaySound`/`_CheckPasswd` 등 시스템/보안 레벨 기능 노출. `CGuard` 1개당 1개(세션 전체에서 공유) |

### 7.4 이벤트 / 입력 처리 계층

| 클래스 | 파일 | 역할 |
|---|---|---|
| `CScript` | Script.h/cpp | 이벤트→프로시저 매핑 (1, 4절 참고) |
| `CKey` | Key.h/cpp | 키보드 입력 처리. `OnKey`/`OnChar`/`OnIME`, 필드 간 이동(`GoNextForm`/`GoPrevForm`/`NextForm`), 셀 단위 편집(`OnKeyCell`). RTM 조사(`RealtimeCodeIndex_Investigation.md`)에서 언급된 `CfmEdit::UpdateData` 호출의 상위 진입점이 여기 있을 가능성 높음(교차검증 필요) |
| `CMouse` | Mouse.h/cpp | 마우스 입력 처리. `OnDown`/`OnUp`/`OnDblClick`/`OnWheel`/`ComboBox`, 클릭한 위치의 폼 판별(`WhichForm`) |
| `COnTimer : CWinThread` | OnTimer.h/cpp | **타이머 전용 워커 스레드**(`AfxBeginThread`로 생성, `Client.cpp:4180`). `Dispatch(key)`로 타이머 이벤트를 큐(`CDWordArray m_que`)에 넣고 메인 스레드와 `CEvent`로 동기화. Wizard에서 확인된 몇 안 되는 실제 멀티스레드 지점 — KnowledgeBase.md의 "멀티스레드 고려사항" 절과 연결지어 재검토 가치 있음 |

### 7.5 통신 / 외부 연동 계층

| 클래스 | 파일 | 역할 |
|---|---|---|
| `CStream` | Stream.h/cpp | TR 스트림 처리 (상세 미조사, `Client.h`에 `m_stream`으로 참조됨) |
| `CDde : CWnd` | Dde.h/cpp | DDE(Dynamic Data Exchange) 통신 — 엑셀 등 외부 프로그램과 실시간 데이터 연동. `CGuard::m_dde`로 세션당 1개 |
| `COleDrop : COleDropTarget` | OleDrop.h/cpp | OLE 드래그앤드롭 수신 (MFC 표준 클래스 상속). `CWorks::m_drop`으로 작업영역마다 1개 |
| `COleDropEx : IDropTarget` | OleDropEx.h/cpp | OLE 드래그앤드롭 수신의 **COM 저수준 직접 구현판**. `CMap<HWND, ..., CWorks*, ...>`로 여러 창의 드롭 대상을 한 인스턴스가 관리 — `COleDrop`과 두 가지 구현이 공존하는 이유는 미조사 |
| `_Application`/`_Workbook`/`Workbooks`/`Range` | excel9.h/cpp | Excel COM 자동화용 **자동생성 래퍼**(ClassWizard 산출물). 커스텀 로직 없음, 엑셀 내보내기 기능(`CGuard::MakeXlsFile` 등)에서 사용 |

### 7.6 부가 UI / 유틸리티 계층

| 클래스 | 파일 | 역할 |
|---|---|---|
| `CObjects` | Objects.h/cpp | 이름→COM 객체 팩토리. `CreateObject(name, target)`로 외부 DLL을 동적 로드해서 `CCmdTarget` 생성 — 맵소스에서 커스텀 COM 컨트롤을 이름으로 참조할 때 쓰는 것으로 추정 |
| `CHistory : CListBox` | History.h/cpp | 입력 필드의 코드 히스토리 드롭다운 UI(리스트박스). `CGuard::m_codex`(코드 히스토리 데이터)를 화면에 보여주는 뷰 역할 |
| `CTips : CWnd` | Tips.h/cpp | 툴팁 표시. `CGuard::m_tips`로 세션당 1개, `RelayEvent`로 여러 `CClient`의 마우스 이벤트를 릴레이받아 툴팁 표시 여부 결정 |
| `CPrinter` | Printer.h/cpp | 프린터 설정 저장/복원 (MFC 표준 유틸리티에 가까움, Wizard 고유 로직 적음) |
| `CLog` | Log.h/cpp | 파일 기반 통신 로그 기록(`Trace(msg, len, dir)`) — 3절/4절에서 다룬 `OutputDebugString` 기반 디버그 로그와는 별개로, **송수신 패킷을 파일로 남기는** 운영 로그 메커니즘 |

### 7.7 작은 헬퍼/데이터 구조체 (Misc.h)

| 클래스 | 역할 |
|---|---|
| `CCaret` | 화면 내 커서 위치(폼 key + 인덱스) 표현. `CClient::m_current`/`m_default`/`m_cbox` 등에서 광범위하게 사용 |
| `CComboN` | 콤보박스 항목 하나 (`m_entry`) |
| `CCode` | 종목코드 입력 히스토리 목록 (`m_codes`, `CGuard::m_codex`의 값 타입) |
| `CdataSet` | 실시간 시세 데이터셋 — 고정배열 `DWORD m_data[maxSYM]`에 필드별 값 저장, RTM 흐름(`DoRTM`)에서 핵심적으로 사용됨 |
| `CpubControl` | DLL 핸들 + CCmdTarget 포인터 쌍 |

---

## 8. 다음 분석 대상 (미착수)

- CGuard 초기화/로그인 시퀀스 상세 (RunAxis mode별 분기, Xecure/Certify 흐름)
- CClient 이벤트 라우팅 상세 (OnMouse/OnKey/OnTRAN → CScript 호출 지점) — `CKey`/`CMouse`가 1차 수신하는 것으로 보이나 `CClient`로 넘어가는 정확한 지점 미확인
- `CDll`(작업영역의 DLL 기반 대안 경로)의 실제 사용처 — 어떤 화면/기능이 CClient 대신 CDll을 쓰는지
- `COleDrop`과 `COleDropEx` 두 드래그앤드롭 구현이 공존하는 이유
- RTM(실시간) 갱신 흐름은 이미 `@docs/KnowledgeBase.md` 11절, `@docs/RealtimeCodeIndex_Investigation.md`에 기록됨 (참고)

---

## 9. 관련 문서

- `@docs/Architecture.md` - Python 엔진 전환 프로젝트 관점 3계층 개요
- `@docs/python_engine_260608.md` - VBS→Python 전환 상세 기록 (초기 설계, 일부 드리프트 있음 → 5절 참고)
- `@docs/KnowledgeBase.md` - 버그/설계 지식 베이스 (12절에 이 문서의 드리프트 발견 내용 반영)
- `@docs/RealtimeCodeIndex_Investigation.md` - RTM 종목코드 역인덱스 조사, `CKey`/`fmEdit` 입력 경로 상세
