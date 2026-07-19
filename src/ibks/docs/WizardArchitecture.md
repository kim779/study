# axwizard(Wizard) 소스 분석

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

## 7. 다음 분석 대상 (미착수)

- CGuard 초기화/로그인 시퀀스 상세 (RunAxis mode별 분기, Xecure/Certify 흐름)
- CClient 이벤트 라우팅 상세 (OnMouse/OnKey/OnTRAN → CScript 호출 지점)
- xscreen.cpp (CxScreen) — 스크립트에서 보이는 Screen 객체의 실제 메서드/프로퍼티 목록
- RTM(실시간) 갱신 흐름은 이미 `@docs/KnowledgeBase.md` 11절에 기록됨 (참고)

---

## 8. 관련 문서

- `@docs/Architecture.md` - Python 엔진 전환 프로젝트 관점 3계층 개요
- `@docs/python_engine_260608.md` - VBS→Python 전환 상세 기록 (초기 설계, 일부 드리프트 있음 → 5절 참고)
- `@docs/KnowledgeBase.md` - 버그/설계 지식 베이스 (12절에 이 문서의 드리프트 발견 내용 반영)
