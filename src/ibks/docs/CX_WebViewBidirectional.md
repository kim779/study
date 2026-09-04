# CX_WebView 양방향 통신(WebMessage) 현황 조사

## 목차

- [문서 목적](#문서-목적)
- [1. 배경](#1-배경)
- [2. 컨트롤 구조 개요](#2-컨트롤-구조-개요)
- [3. 현재 스크립트/Wizard에 노출된 API](#3-현재-스크립트wizard에-노출된-api)
- [4. WebView2 표준 양방향 메시징 메커니즘](#4-webview2-표준-양방향-메시징-메커니즘)
- [5. 실제 코드 상태 — 이미 절반 배선되어 있음](#5-실제-코드-상태--이미-절반-배선되어-있음)
- [6. 참고 — SendTR/OnMessage(WM_USER)는 다른 채널이다](#6-참고--sendtronmessagewm_user는-다른-채널이다)
- [7. Gap 분석](#7-gap-분석)
- [8. 구현 방향 (제안, 미구현)](#8-구현-방향-제안-미구현)
- [9. 관련 파일](#9-관련-파일)
- [10. 다음 단계 / 미확인 사항](#10-다음-단계-미확인-사항)

---

## 문서 목적

HTS가 향후 AI 등 웹 콘텐츠와 양방향으로 통신해야 할 가능성을 대비해, 현재 `CONTROL/CX_WebView`(EdgeView/WebView2 기반 웹 뷰어 OCX)가 호스트(Wizard/HTS)와 로드된 웹페이지 사이에 실제로 어떤 통신 수단을 갖고 있는지 소스 코드로 확인하고, 표준 WebView2 양방향 메시징 메커니즘 대비 무엇이 있고 무엇이 빠졌는지 정리합니다. 아직 구현 작업은 시작하지 않았고, 현재 상태 파악 + 격차(gap) 분석까지만 다룹니다.

---

## 1. 배경

현재 `Screen.Navigate(url)`류의 단방향 URL 로드만 가능하고, 웹페이지가 HTS로 메시지를 보내거나 HTS가 이미 로드된 웹페이지에 메시지를 보내는 기능은 없는 것으로 파악됨(사용자 확인). HTS가 AI 기능을 웹링크 방식으로 도입한다면 이 양방향 통신이 선행되어야 할 것으로 예상되어, 우선 현재 코드 상태부터 확인함.

---

## 2. 컨트롤 구조 개요

```
CX_WebView.ocx (COM 진입점, dispatch/event map 자체는 없음)
  └─ CMainWnd            (dispatch map 선언은 있으나 내용이 비어있음 — MainWnd.cpp:44 BEGIN_DISPATCH_MAP 안에 항목 없음)
        └─ CEdgeWnd       (WebView2 기반 — 실제 브라우저 렌더링 + 스크립트 API 노출)
        └─ CIEWnd         (레거시 Internet Explorer 기반 — WebMessage류 배선 전혀 없음, Navigate만 존재)
```

`CEdgeWnd`(`EdgeWnd.cpp/.h`)가 `ICoreWebView2`/`ICoreWebView2Controller`/`ICoreWebView2Environment`를 직접 들고 있는 실질적인 WebView2 래퍼이고, 스크립트(Wizard/맵)에 노출되는 dispatch 메서드도 `CMainWnd`가 아니라 `CEdgeWnd`/`CIEWnd`에 각각 개별로 선언되어 있음(`DECLARE_DISPATCH_MAP` — 두 클래스 다 자체 소유).

**미조사:** `CAxBrowser : public CHtmlView`(`AxBrowser.h`)와 `ViewComponent.cpp/.h`, `ComponentBase.h`의 정확한 역할(레거시 MSHTML 뷰인지, EdgeWnd 내부의 `m_components` 플러그인 구조인지)은 이번 조사 범위 밖 — 메시징 게이트 자체와 무관해 보여 건너뜀.

---

## 3. 현재 스크립트/Wizard에 노출된 API

`EdgeWnd.cpp`의 `DISP_FUNCTION_ID` 전체(106~109행) — 이게 전부다:

| 메서드 | dispid | 역할 |
|---|---|---|
| `Navigate(BSTR sUrl)` | 1 | URL 이동 |
| `GoBack()` | 2 | 뒤로가기 |
| `GoForward()` | 3 | 앞으로가기 |
| `Navigate2(SHORT igubn, BSTR sUrl)` | 4 | URL 이동(변형) |

`IEWnd.cpp`도 `Navigate` 하나뿐(56행). **메시지를 보내는 메서드, 웹페이지로부터 메시지를 받았을 때 스크립트로 쏘아줄 이벤트(`DECLARE_EVENT_MAP`/`FireEvent`) — 둘 다 없음.**

---

## 4. WebView2 표준 양방향 메시징 메커니즘

Microsoft WebView2 SDK가 공식 제공하는 호스트↔웹페이지 메시징 표준 — 이 프로젝트가 이미 참조 중인 `Microsoft.Web.WebView2` NuGet(1.0.3595.46, `CONTROL/CX_WebView/packages/`)에 포함된 API:

| 방향 | 네이티브(C++/COM) 쪽 | 웹페이지(JS) 쪽 |
|---|---|---|
| 호스트 → 웹 | `ICoreWebView2::PostWebMessageAsString(LPCWSTR)` / `PostWebMessageAsJson(LPCWSTR)` | `window.chrome.webview.addEventListener('message', e => { ... })` |
| 웹 → 호스트 | `ICoreWebView2::add_WebMessageReceived(handler)` 이벤트 | `window.chrome.webview.postMessage(data)` |

이건 WebView2가 기본 제공하는 정식 메커니즘이라, 사용자가 예상한 "이미 표준화된 송수신 기법"이 정확히 이것에 해당함.

---

## 5. 실제 코드 상태 — 이미 절반 배선되어 있음

`CEdgeWnd::OnCreateCoreWebView2ControllerCompleted()`(`EdgeWnd.cpp:395`) 안에 위 두 API가 **이미 코드로 존재**한다:

- **`add_WebMessageReceived` 핸들러 등록됨** (`EdgeWnd.cpp:426-439`):
  ```cpp
  webView->add_WebMessageReceived(Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>(
      [this](ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
          wil::unique_bstr message;
          args->TryGetWebMessageAsString(&message);
          CString strMsg(message.get());
          m_slog.Format("[EDGE] add_WebMessageReceived  strMsg =[%s] ", strMsg);
          OutputDebugString(m_slog);
          // TODO: 실제 처리 로직 추가   ← 여기서 끝. 받은 메시지를 아무 데도 전달하지 않음
          return S_OK;
      }).Get(), nullptr);
  ```
  웹페이지가 `postMessage`를 호출하면 문자열은 수신되어 `OutputDebugString`으로 로그만 찍히고 버려짐 — 소비하는 곳이 전혀 없음.

- **`PostWebMessageAsString` 호출부는 있으나 통째로 주석 처리됨** (`EdgeWnd.cpp:444-460`, `FrameNavigationCompleted` 콜백 안):
  ```cpp
  // 페이지 로딩 완료 후 메시지 전송
  /*LPCWSTR msg = L"{ \"type\": \"notify\", \"data\": \"Hello from native!\" }";
  HRESULT hr = m_webView->PostWebMessageAsString(msg);
  if (FAILED(hr)) { TRACE(_T("PostWebMessageAsString 실패! hr=0x%08X\n"), hr); }*/
  ```
  실제로 호출되는 코드 경로가 없어 지금은 절대 실행되지 않음.

**결론:** WebView2 API 자체는 양방향을 지원하고, 누군가 과거에 그 배선을 절반쯤 깔아두었다(receive 훅은 등록됐지만 처리 로직 없음, send 호출은 작성됐지만 비활성화). 다만 3절에서 확인했듯 **이 두 훅이 OCX의 스크립트/Wizard 노출 인터페이스(dispatch/event map)와는 전혀 연결되어 있지 않아서**, 실사용 관점에서는 "일방적인 navigate뿐"이라는 사용자 관찰이 정확함.

---

## 6. 참고 — SendTR/OnMessage(WM_USER)는 다른 채널이다

`EdgeWnd.h`에 `LRESULT SendTR(CString strName, BYTE type, CString strData, BYTE key)`(`EdgeWnd.cpp:1044`)와 `afx_msg long OnMessage(WPARAM, LPARAM)`(`ON_MESSAGE(WM_USER, OnMessage)`, `EdgeWnd.cpp:97`)가 있어 얼핏 통신 채널처럼 보이지만, **이건 웹페이지(JS)와의 통신이 아니라 `CEdgeWnd`↔부모 윈도우(Wizard `CScreen`/`CClient`) 사이의 통신**이다:

- `SendTR`은 `m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, ...), (LPARAM)buff)` 형태로 부모(Wizard)에게 TR 요청을 올려보내는 용도(예: `SendTR("PMHOWURL", NULL, strData, 1)`, `EdgeWnd.cpp:1016`).
- `OnMessage`는 반대로 부모가 `WM_USER`로 보내는 명령(맵명/루트경로/사용자ID 조회 등, `mapDLL`/`variantDLL`/`eventDLL` 계열 `MAKEWPARAM` 서브코드)을 받는 핸들러.

이 패턴은 `WizardDependency.md` §④에서 정리한 "`.map`/스크립트가 이름 지정하는 동적 플러그인 DLL"이 부모(Wizard)와 통신하는 기존 관례(`DllProc WM_USER cmd=...`, `DebugLogGuide.md` §7)와 동일한 계열 — **즉 CX_WebView는 이미 "컨트롤↔Wizard" 축의 통신 채널은 갖고 있고, 이번에 부족한 건 "컨트롤↔웹페이지(JS)" 축**이라는 게 명확히 구분된다. 향후 양방향 메시징을 구현할 때, 웹페이지가 보낸 메시지를 처리할 위치를 "Wizard로 SendTR류로 올려보낼지" 아니면 "Screen 스크립트로 이벤트만 쏘고 처리는 스크립트에 맡길지"를 결정할 때 이 기존 축을 참고할 수 있음.

---

## 7. Gap 분석

| 항목 | 필요한 것 | 현재 상태 |
|---|---|---|
| 웹→호스트 수신 | `add_WebMessageReceived` 훅 | ✅ 있음(등록됨) |
| 웹→호스트 처리 | 받은 메시지를 스크립트/상위로 전달 | ❌ TODO만 있고 없음 |
| 웹→호스트 알림(스크립트) | `DECLARE_EVENT_MAP`+`FireEvent`로 Wizard 스크립트에 이벤트 발생 | ❌ 없음 |
| 호스트→웹 전송 API(네이티브) | `PostWebMessageAsString`/`Json` 호출 | 🟡 코드는 있으나 주석처리, 미호출 |
| 호스트→웹 전송 API(스크립트 노출) | `Screen.PostMessage(data)` 류의 dispatch 메서드 | ❌ 없음(`Navigate` 계열만 존재) |
| JS 쪽 수신 | `window.chrome.webview.addEventListener('message', ...)` | (웹페이지 몫 — 사용자가 만들 테스트 페이지에서 확인 예정) |
| JS 쪽 송신 | `window.chrome.webview.postMessage(data)` | (웹페이지 몫 — 동일) |

---

## 8. 구현 방향 (제안, 미구현)

**웹 → 호스트 (받기)**
1. `add_WebMessageReceived` 핸들러(`EdgeWnd.cpp:426`)의 TODO 자리에서, 받은 문자열을 어딘가로 전달 — 후보 두 가지:
   - a) 6절의 기존 축을 재사용해 `m_pParent->SendMessage(WM_USER, ...)`로 Wizard에 올려보냄(기존 `SendTR`과 같은 패턴)
   - b) `CEdgeWnd` 자체에 새 이벤트(`DECLARE_EVENT_MAP`, 예: `eventidOnWebMessage`)를 추가해 `FireEvent`로 스크립트(`AX_*_OnWebMessage_AX_`류)에 직접 전달 — Wizard의 다른 이벤트 발행 패턴(`CScript`, `WizardArchitecture.md` §4 명명규칙)과 일관성 있는 방식
2. 어느 쪽이든 `.odl`에도 새 이벤트/메서드를 등록해야 함 — 기존 메모리 `[[feedback_ocx_new_method_needs_odl]]`("OCX 신규 메서드는 .odl도 필수") 그대로 적용됨. dispatch만 추가하고 `.odl` 반영을 빠뜨리면 조용히 실패함.

**호스트 → 웹 (보내기)**
1. `EdgeWnd.h`에 새 dispatch 메서드 추가(예: `void PostMessage(BSTR sMsg)`), `DISP_FUNCTION_ID`로 등록, `.odl` 반영.
2. 그 구현에서 `m_webView->PostWebMessageAsString(...)` 호출 — 지금 주석 처리된 코드(444~460행)를 참고해 살리되, `FrameNavigationCompleted` 콜백 안이 아니라 **스크립트가 원하는 시점에 호출 가능한 별도 메서드**로 분리해야 함(지금 위치는 "페이지 로딩 완료 시 자동 발송" 테스트코드 흔적으로 보임).

---

## 9. 관련 파일

| 파일 | 역할 |
|---|---|
| `CONTROL/CX_WebView/EdgeWnd.h/cpp` | WebView2 래퍼 — 이번 조사의 핵심, `add_WebMessageReceived`/`PostWebMessageAsString` 코드 위치 |
| `CONTROL/CX_WebView/IEWnd.h/cpp` | 레거시 IE 기반 대안 — 메시징 배선 없음, `Navigate`만 |
| `CONTROL/CX_WebView/MainWnd.h/cpp` | dispatch map 선언은 있으나 비어있음 |
| `CONTROL/CX_WebView/CX_WebView.h/cpp/odl` | OCX 최상위 진입점, 신규 dispatch/event 추가 시 `.odl` 반영 필요 |
| `@docs/WizardDependency.md` §④ | CX_* 계열 컨트롤이 Wizard에 꽂히는 공통 진입점(`axCreate`) 및 기존 WM_USER 통신 관례 |
| `@docs/DebugLogGuide.md` §7 | `DllProc WM_USER cmd=...` — 6절에서 언급한 기존 컨트롤↔Wizard 통신축의 로그 태그 |

---

## 10. 다음 단계 / 미확인 사항

- 사용자가 직접 테스트 Java 웹페이지를 만들어 `window.chrome.webview.postMessage(...)`를 호출하면, 지금 상태로도 `EdgeWnd.cpp:432`의 `OutputDebugString` 로그가 DebugView에 찍히는지부터 1차 검증 예정(코드 수정 없이 확인 가능한 부분).
- `CAxBrowser`/`ViewComponent`/`ComponentBase`의 정확한 역할 — 이번 조사에서 메시징 게이트와 무관해 보여 건너뜀, 향후 구현 단계에서 `EdgeWnd`가 이 컴포넌트들과 어떻게 엮이는지 재확인 필요할 수 있음.
- 8절의 "웹→호스트 처리를 어디로 보낼지"(SendTR류 vs 신규 이벤트) 방향은 아직 결정된 바 없음 — 실제 구현 착수 시 사용자와 논의 필요.
- AI 웹링크 활용 시나리오에서 보안(어떤 origin/URL에서 온 메시지를 신뢰할지, `add_WebMessageReceived`의 `args`에서 origin 검증 가능 여부)은 이번 조사에서 다루지 않음 — 구현 단계에서 검토 필요.

---

**최종 수정:** 2026-09-04
**작성 방식:** `EdgeWnd.cpp/.h` 소스 직접 확인, WebView2 SDK 표준 API 지식 대조
**상태:** 1차 현황조사 완료 — 구현은 미착수
