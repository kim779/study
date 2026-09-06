---
project: ibks
category: guide
status: 완료
updated: 2026-08-30
---

# 디버그 로그 가이드 (axlog)


## 목차

- [전체 로그 키워드 목록 (DebugView 등록용)](#전체-로그-키워드-목록-debugview-등록용)
- [색인 — 태그/절 빠른 찾기](#색인-태그절-빠른-찾기)
  - [증상별 시작점](#증상별-시작점)
  - [추천 DebugView 필터/북마크 세트](#추천-debugview-필터북마크-세트)
  - [컨트롤 `kind` 코드 표 (참고용, `builder/h/form.h` 확정)](#컨트롤-kind-코드-표-참고용-builderhformh-확정)
- [문서 목적](#문서-목적)
- [1. axlog 인프라 개요](#1-axlog-인프라-개요)
- [2. 화면 열기 흐름 (CClient::Attach → CScreen::Parse)](#2-화면-열기-흐름-cclientattach-cscreenparse)
- [3. 스크립트 엔진 내부 (LOG_SCRIPT — 평소 꺼둠)](#3-스크립트-엔진-내부-log_script-평소-꺼둠)
- [4. 이벤트 → 스크립트 프로시저 디스패치 (CScript, 2026-07-29 추가)](#4-이벤트-스크립트-프로시저-디스패치-cscript-2026-07-29-추가)
- [5. 입력 이벤트 (마우스/키보드)](#5-입력-이벤트-마우스키보드)
- [6. 소켓 데이터 파이프라인 (번호 순서대로 실행됨)](#6-소켓-데이터-파이프라인-번호-순서대로-실행됨)
- [7. DLL 기반 화면 (CDll)](#7-dll-기반-화면-cdll)
- [8. 로그인/인증 · OLE 드래그앤드롭 · DDE (2026-07-28 추가)](#8-로그인인증-ole-드래그앤드롭-dde-2026-07-28-추가)
- [9. 내일(화면 열고 닫으며 확인할 때) 체크리스트](#9-내일화면-열고-닫으며-확인할-때-체크리스트)
- [10. 타이머 워커 스레드 (COnTimer, 2026-07-29 추가)](#10-타이머-워커-스레드-contimer-2026-07-29-추가)
- [10.5. 스크립트 타이머 (Screen.SetTimer/SetTimerX) — 실제 살아있는 경로 (2026-07-30 추가)](#105-스크립트-타이머-screensettimersettimerx-실제-살아있는-경로-2026-07-30-추가)
- [11. axisform.dll 컨트롤 레이어 로그 (LOG_AXISFORM, 2026-07-29 추가)](#11-axisformdll-컨트롤-레이어-로그-log_axisform-2026-07-29-추가)
- [12. 암호화/복호화 (Xecure, 2026-07-31 추가)](#12-암호화복호화-xecure-2026-07-31-추가)
  - [NOENC.TXT — 개발용 암호화 강제 비활성화 (2026-07-31 추가)](#noenctxt-개발용-암호화-강제-비활성화-2026-07-31-추가)
- [13. 관련 문서](#13-관련-문서)

---

## 전체 로그 키워드 목록 (DebugView 등록용)

이 문서에서 다루는 모든 로그 태그를 그대로 나열합니다. DebugView의 Highlight/Bookmark에 하나씩 추가해두는 용도. 각 태그의 상세 의미·주의사항은 아래 색인 표에서 절 번호를 찾아 본문 참고. `CxScreen::_`/`CxSystem::_`/`CScript::On`처럼 접두사만 적은 항목은 그 뒤에 붙는 여러 구체 메서드명(`_Send`/`_ChangeTR`/`OnClick`/`OnChange` 등)을 한 번에 잡기 위한 substring 필터임. 10절(`COnTimer`)은 죽은 코드라 실제로 안 찍히므로 목록에서 제외.

**카테고리 (필수, 1절)**
```
[WIZARD][INIT]
[WIZARD][EVENT]
[WIZARD][DATA]
[WIZARD][RTM]
[WIZARD][SCRIPT]
[WIZARD][AXISFORM]
```

**추가 카테고리 (2026-08-28~29, 이 문서 작성 이후 신설 — 상세는 각 문서 참고)**
```
[WIZARD][FILEPATCH]      ← 로그인시 파일패치, 상세: FilePatchProtocol.md
[WIZARD][LOGIN]          ← 로그인 응답 파싱(OnSign/AddRegistry/OnCertify), 상세: LoginSequence.md
[WIZARD][SOCK_SEND]      ← 소켓 저수준 송신(CGuard::Write), 상세: LoginSequence.md
[WIZARD][SOCK_RECEIVE]   ← 소켓 저수준 수신(OnRead, 로그인 포함 전체 메시지), 상세: LoginSequence.md
```

**화면 열기/구조 (2절)**
```
axAttach
CClient::Attach-enter
CClient::Attach-timing
CClient::Attach-total
CScreen::ctor
CScreen::dtor
CScreen::Parse
[SCREEN-TREE]
setFDC
```

**스크립트 엔진 내부 (3절, LOG_SCRIPT)**
```
LoadScript OK
LoadScript FAILED
DoSomething
getIDOfProcedure
DoProcedure(1)
DoProcedure(2)
CEngineWrapper::LoadScript
[Initialize]
[AddObject]
[IsAvailable]
[DoProcedure]
[fetchError]
CxScreen::_
CxSystem::_
```

**이벤트→스크립트 프로시저 디스패치 (4절)**
```
CScript::On
CScript::Procedure
```

**입력 이벤트 — 마우스/키보드/클립보드 (5절)**
```
CMouse::OnDown
CKey::OnKey
VK_RETURN->InStream
Ctrl+1/2/3
CKey::OnChar
blocked by IsWait
CKey::Copy$Paste
CGuard::SetClipboard
CGuard::GetClipboard
```

**소켓 데이터 파이프라인 (6절)**
```
[OnTRAN-gate]
[MakeStream-waitSN-drop]
[0-MakeStream-send]
[TR-RTT]
[CTRL-send]
[CTRL-RTT]
[SVC-send]
[SVC-RTT]
[ServiceEx-wait]
[0-GetDataNRM-field]
[0-GetDataNRM-grid]
[0-GetDataNRM-cell]
[0-Write-send]
[1-OnAxis-raw]
[2-OnStream-reassemble]
[3-CClient-OnAxis-reassembled]
[4-OutStream-parse]
[SetDataH]
[SetDataH-ledger]
[5-SetDataNRM-write]
[6-SetCells
[ParseRCC]
[ParseRCC-combo]
[UpdateRTM-write]
[FlashGrid-write]
[FlashSemi-write]
[ScrollRTM-insert]
[ATTACH]
[MISMATCH]
CGuard::Write
```

**CDll 기반 화면 (7절)**
```
CDll::Attach
loaded via axCreate
DllProc WM_USER
DllProc WM_SIZE
[CDll-OnAxis-raw]
```

**로그인/인증 · OLE 드래그앤드롭 · DDE (8절)**
```
CWizardCtrl::RunAxis
CGuard::Login
CGuard::Certify
CGuard::OnCertify
CGuard::CertifyId
COleDrop::OnDrop
COleDrop::Register
COleDropEx::Drop
COleDropEx::Register
COleDropEx::Revoke
CDde::OnDDE
CDde::OnAdvise
CDde::OnExit
```

**스크립트 타이머 (10.5절, 실제 동작 경로)**
```
CallProc WM_TIMER TM_VB
CClient::SetTimer
```

**axisform.dll 컨트롤 레이어 (11절, LOG_AXISFORM)**
```
CfmEdit::_Trigger
CfmEdit::Draw
CfmEdit::OnLButton
CfmEdit::SetFocus
CfmEdit::UpdateData
CfmEdit::InsertData
CfmEdit::getStartPos
CfmBase::ReadData
CfmBase::WriteData
```

**암호화/복호화 — Xecure, NOENC.TXT (12절)**
```
[Xecure]
IsNoEncMode
[Xecure-Nego]
```

---

## 색인 — 태그/절 빠른 찾기

전체를 읽지 않고 원하는 로그 태그나 상황부터 바로 찾기 위한 표. 상세 설명·조건·주의사항은 각 절 본문 참고.

| 절 | 주제 | 핵심 태그/키워드 |
|---|---|---|
| 1 | axlog 인프라, 카테고리 on/off | `LOG_INIT` / `LOG_EVENT` / `LOG_DATA` / `LOG_RTM` / `LOG_SCRIPT`(OFF) / `LOG_AXISFORM`(OFF) |
| 2 | 화면 열기(Attach→Parse) 타이밍 | `CClient::Attach-enter/timing/total`, `CScreen::ctor`/`dtor`, `CScreen::Parse`(`-timing`), `[SCREEN-TREE]`, `CWizardCtrl::axWizard setFDC` |
| 3 | 스크립트 엔진 내부(VBS/Python), `LOG_SCRIPT` | `getIDOfProcedure`, `DoProcedure(1)/(2)`(`LOG_EVENT`), `CEngineWrapper::LoadScript scpKind=`, Python `[Initialize]`/`[AddObject]`/`[IsAvailable]`/`[DoProcedure]`/`[fetchError]`, `CxScreen::_Send`/`_ChangeTR`/`_CreateWindow`/`_Proc`/`_SetTimer` 등, `CxSystem::_Trigger`/`_Trace`/`_CheckPasswd` 등 |
| 4 | 이벤트→스크립트 프로시저 디스패치(`CScript`) | `CScript::OnStart`/`OnFocus`/`OnClose`/`OnTimer`/`OnKey`/`OnClick`/`OnChange`/`OnProcedure`/`Procedure` 등 (`OnAlert`는 죽은코드, 로그 없음) |
| 5 | 입력 이벤트(마우스/키보드/클립보드) | `CMouse::OnDown`, `CKey::OnKey`(+`VK_RETURN->InStream`, +`Ctrl+1/2/3 fallback`), `CKey::OnChar`(+`blocked by IsWait`), `CKey::Copy$Paste`, `CGuard::SetClipboard`/`GetClipboard` |
| 6 | 소켓 데이터 파이프라인(송수신, 번호순 실행) | `[OnTRAN-gate]`, `[MakeStream-waitSN-drop]`, `[0-MakeStream-send]`, `[TR-RTT]`, `[CTRL-send]`, `[CTRL-RTT]`, `[SVC-send]`, `[SVC-RTT]`, `[ServiceEx-wait]`, `[0-GetDataNRM-field/grid/cell]`, `[0-Write-send]`, `[1-OnAxis-raw]`, `[2-OnStream-reassemble]`, `[3-CClient-OnAxis-reassembled]`, `[4-OutStream-parse]`, `[SetDataH]`/`[SetDataH-ledger]`, `[5-SetDataNRM-write]`, `[6-SetCells-*]`, `[ParseRCC]`/`[ParseRCC-combo]`, `[UpdateRTM-write]`, `[FlashGrid-write]`, `[FlashSemi-write]`, `[ScrollRTM-insert]`, `CGuard::Write(1)/(2)` |
| 7 | `CDll` 기반 화면(외부 DLL 로드형) | `CDll::Attach`, `loaded via axCreate/axCreateEx/axCreateX`, `DllProc WM_USER/WM_SIZE`, `[CDll-OnAxis-raw]`(`OutStream`을 안 타는 화면의 유일한 추적점) |
| 8 | 로그인/인증, OLE 드래그앤드롭, DDE | `CWizardCtrl::RunAxis`, `CGuard::Login`/`Certify`/`OnCertify`/`CertifyId`, `COleDrop::OnDrop`/`Register`, `COleDropEx::Drop`/`Register`/`Revoke`, `CDde::OnDDE`/`OnAdvise`/`OnExit` |
| 9 | 화면 열고닫으며 확인할 체크리스트 | (태그 없음, 진단 순서 안내) |
| 10 | `COnTimer` 워커스레드 — **죽은 코드, 실제로 안 찍힘** | `COnTimer::Startup`/`Dispatch`/`Run`/`DoParse`/`Cleanup` (참고용으로만 남김) |
| 10.5 | 스크립트 타이머(`Screen.SetTimer`/`SetTimerX`, 실제 동작 경로) | `CallProc WM_TIMER TM_VB`/`TM_VBx`, `CClient::SetTimer id=... result=...`(등록 성공여부) |
| 11 | axisform.dll 컨트롤 렌더링/입력(`LOG_AXISFORM`, OFF) | `CfmEdit::Draw`/`SetFocus`/`UpdateData`/`InsertData`/`getStartPos`/`_Trigger`/`OnLButton`, `CfmBase::ReadData`/`WriteData [combo]` |
| 12 | 암호화/복호화(Xecure), `NOENC.TXT` 개발스위치 | `CGuard::Initial m_xecure CreateControl`, `[Xecure] helper=ENC/DEC nBytesIn/nBytesOut/retv`, `[Xecure] helper=... SKIPPED`, `[Xecure] IsNoEncMode check path/result`, `[Xecure] decrypt FAILED`, `[Xecure-Nego]`(로그인 초반 AXISENCX 키교환, `CWizardCtrl::Xecure`류 — `[Xecure]`와 이름만 같고 별개 함수) |
| 13 | 관련 문서 링크 | (태그 없음) |

### 증상별 시작점

| 증상 | 먼저 볼 곳 |
|---|---|
| 특정 화면의 소켓 수신 로그(`[4-OutStream-parse]` 등)가 전혀 안 보임 | 7절 — `CDll` 기반 화면이면 `OutStream` 자체를 안 타고 `[CDll-OnAxis-raw]`만 찍힘 |
| `Screen.Send()`를 불렀는데 실제 송신이 안 됨 | 6절 `[OnTRAN-gate]`/`[MakeStream-waitSN-drop]` — 응답 대기중(`waitSN`)이면 조용히 버려짐 |
| 타이머가 한 번만 돌고 이후 멈춤/아예 안 됨 | 10.5절 "진단 순서" 4단계 체크리스트 |
| 로그에 평문이 아니라 암호화된 바이트만 보임 | 12절 `NOENC.TXT` — 개발용으로 요청/응답 암호화를 통째로 끌 수 있음 |
| Ctrl+C/V가 안 먹힘 | 5절 `Copy$Paste`(+`docs/KnowledgeBase.md` 13절, OS 레벨 차단 이슈) |
| 화면 열기가 느림 | 2절 `[CClient::Attach-timing]`으로 단계별 소요시간 확인 (9절 체크리스트 1번) |
| 스크립트 이벤트가 실행 안 됨 | 4절(디스패치 자체가 됐는지) → 3절 `IsAvailable`(프로시저가 정의됐는지) |
| 조회 중 화면이 통째로 수십초~1분씩 멈춤(`[TR-RTT]`/`[CTRL-RTT]`엔 그 정도로 큰 값이 안 보임) | 6절 `[SVC-RTT]`/`[ServiceEx-wait]` — `Screen.Service()`/`ServiceEx()`는 `msgK_SVC`라는 별도 채널을 쓰고 `ServiceEx`는 UI 스레드를 직접 블로킹하는 자체 메시지펌프 루프를 도므로, TR/CTRL RTT 로그에는 절대 안 잡힘 |

### 추천 DebugView 필터/북마크 세트

카테고리 필터(`[WIZARD][INIT/EVENT/DATA/SCRIPT]`, `[RTM]`)에 더해, 아래 태그들을 북마크/하이라이트로 추가해두면 상황별로 찾기 편하다.

**소켓/송수신 (6절)**
- `[0-Write-send]` 또는 `CGuard::Write` — 실제로 소켓에 나가는 바이트 원문(청크단위). `MakeStream`은 "보내려는 시도"까지고, 이건 "진짜 나갔다"의 최종 확인 지점.
- `[MakeStream-waitSN-drop]` — `MakeStream` 키워드에 이미 포함돼서 잡히지만, 따로 하이라이트해두면 "Send 호출했는데 응답대기(`waitSN`) 때문에 조용히 버려진" 케이스를 한눈에 구분 가능.
- `[CDll-OnAxis-raw]` — `CDll` 기반 화면(예: 9524)은 `OutStream`/`SetDataNRM` 계열 로그가 아예 안 찍히는데, 이게 그 화면들의 유일한 수신 흔적.

**암호화 (12절)**
- `[Xecure]` — 암/복호화 호출마다(`helper=ENC/DEC`, `nBytesIn/nBytesOut/retv`) 찍힘. `NOENC.TXT` 스위치 상태 확인(`IsNoEncMode check ... result=`)도 이걸로 잡힘.
- `[Xecure-Nego]` — 로그인 맨 처음 `AXISENCX` 키교환 협상 전용(`CWizardCtrl::Xecure`류). `[Xecure]`와 헷갈리기 쉬우니 필터를 분리해서 볼 것 — 협상 시작 판단→`DI_XEC` 결과→실제 송신→응답수신 순서로 찍힘.

**스크립트 디스패치 (3~4절) — "이벤트는 도는데 스크립트가 안 도네" 진단용**
- `CScript::On` — 어떤 이벤트가 어느 맵의 어느 프로시저(`AX_*_On*_AX_`)로 매핑됐는지(4절). `getIDOfProcedure`보다 한 단계 앞선 지점이라, 여기부터 보면 "디스패치 자체가 안 됐다"와 "디스패치는 됐는데 함수가 없다"(`getIDOfProcedure`/`IsAvailable`)를 구분하기 쉬움.
- `IsAvailable` — Python 엔진 쪽도 `[IsAvailable]` 태그로 찍혀서 VBS/Python 양쪽 다 이걸로 걸림.

**화면 열기/구조 (2절)**
- `CClient::Attach` — 열기 시작~단계별 시간~총 소요시간이 전부 이 키워드로 잡힘(`enter`/`timing`/`total`).
- `[SCREEN-TREE]` — 부모맵이 어느 서브맵을 어느 컨트롤에 임베딩했는지, 하이라이트해두면 화면 구조를 로그만으로 재구성 가능.

우선순위 추천: `[Xecure]`(그동안 전혀 안 잡히고 있던 영역)와 `CGuard::Write`(실제 송신 바이트 확인) 두 개가 가장 가치가 큼.

### 컨트롤 `kind` 코드 표 (참고용, `builder/h/form.h` 확정)

`[0-GetDataNRM-field]`/`[5-SetDataNRM-write]`/`[ATTACH]` 등 로그에 찍히는 `kind=` 숫자는 그 필드/컨트롤의 종류(`CfmBase` 파생 클래스, `@docs/AxisformArchitecture.md` 참고)를 나타낸다. 전체 21종:

| kind | 상수 | 종류 |
|---|---|---|
| 1 | `FM_SYS` | 시스템 정보 |
| 2 | `FM_LABEL` | 레이블 |
| 3 | `FM_BOX` | 박스/테두리 |
| 4 | `FM_GROUP` | 그룹박스 |
| 5 | `FM_PANEL` | 패널(3D) |
| 6 | `FM_BUTTON` | 버튼 |
| 7 | `FM_EDIT` | 입력필드 |
| 8 | `FM_COMBO` | 콤보박스 |
| 9 | `FM_OUT` | 출력전용 필드 |
| 10 | `FM_MEMO` | 멀티라인 텍스트 |
| 11 | `FM_GRID` | 그리드 |
| 12 | `FM_TABLE` | 테이블 |
| 13 | `FM_TREE` | 트리뷰 |
| 14 | `FM_AVI` | AVI 동영상 |
| 15 | `FM_CONTROL` | 임베디드 컨트롤(외부 OCX) |
| 16 | `FM_OBJECT` | 서브맵(unit MAP object) |
| 17 | `FM_TAB` | 탭 |
| 18 | `FM_UTAB` | 사용자 정의 탭 |
| 19 | `FM_SHEET` | 스프레드시트 스타일 |
| 20 | `FM_BROWSER` | 웹 브라우저 |
| 21 | `FM_GRIDEX` | 그리드Ex |

**주의:** `@docs/AxisformArchitecture.md` 5절의 24개 파생 클래스 목록(`CfmCheck`/`CfmRadio`/`CfmEditEx`/`CfmSysm` 포함)과 여기 21개 `kind` 숫자가 1:1로 안 맞는다 — 체크박스/라디오/EditEx 등 일부는 별도 `kind` 번호 없이 다른 `kind`(예: `FM_BUTTON`)에 속성 플래그로 얹혀 세분화되는 것으로 추정된다(미확인, 필요 시 `LoadForm`의 kind별 팩토리 분기 확인 요).

**실측 예시(2026-08-14, IB622300/IB622302 원장TR 비교):** 같은 이름의 필드(`AN15`/`csubb`)가 부모맵과 서브맵에서 서로 다른 `kind`로 정의된 경우가 실제로 확인됨(`AN15`: 부모=15/`FM_CONTROL`, 서브맵=7/`FM_EDIT`) — 서브맵은 부모와 완전히 독립적인 컨트롤 정의를 가진다는 기존 관찰(`@docs/WizardArchitecture.md`)과 일치.

---

## 문서 목적

2026-07-26 세션에서 도입한 중앙 로깅 인프라(`h/axlog.h`)와, 그날 하루 동안 추가한 모든 진단 로그의 위치·의미·태그를 정리합니다.
DebugView(또는 DebugView++)의 "Find" 창에 아래 태그를 넣어 검색하면서 화면을 열고 닫아보는 용도로 사용합니다.

**2026-07-27 갱신:** `dll/vbs/pythonEngine.cpp`(Python 엔진 내부, 그동안 로그가 전혀 없던 사각지대)와 `xscreen.cpp`/`xsystem.cpp`(`Screen`/`System` 스크립트 dispatch 객체, `_Send`/`_Trigger` 2곳만 있던 것을 나머지 주요 메서드로 확대)에 로그를 추가함 — 3절 표에 반영.

**2026-07-28 갱신:** 클립보드(Ctrl+C/X/V, `Key.cpp`/`Guard.cpp`), 로그인/인증(`RunAxis`/`Login`/`Certify`, `WizardCtrl.cpp`/`Guard.cpp`), OLE 드래그앤드롭(`OleDrop.cpp`/`OleDropEx.cpp`), DDE(`Dde.cpp`) — 로그가 전혀 없던 4개 영역에 추가. 5절, 8절(신설) 참고. `RunAxis`의 기존 평문 로그인데이터 출력(`m_slog`+`OutputDebugString`)은 axlog 전환하면서 보안상 제거함.

**2026-07-29 갱신:** Ctrl+C/V가 OS 레벨에서 외부(추정: 보안/DLP 에이전트)에 의해 통째로 막히는 현상을 axlog로 확정 진단(`docs/KnowledgeBase.md` 13절 참고) — `Key.cpp::CKey::OnKey`에 Ctrl+1(복사)/Ctrl+2(붙여넣기)/Ctrl+3(잘라내기) 폴백 단축키 추가, 5절에 반영. 그리고 그동안 로그가 없던 `Script.cpp`(이벤트→스크립트 프로시저 디스패치, `CScript`의 모든 `On*` 함수)에 로그 추가 — 신설 4절 참고(이후 절 번호 한 칸씩 밀림).

**2026-07-29 추가 갱신:** `OnTimer.cpp`(`COnTimer` 워커 스레드)에 로그 추가(10절 신설) — 단, `Client.cpp`/`Client.h`를 보니 `m_OnTimer` 생성 코드가 전부 주석 처리돼 있어 **현재 이 클래스는 실제로 인스턴스화되는 곳이 없는 죽은 코드**(`docs/KnowledgeBase.md` 14절 참고)이므로 이 로그들은 지금 빌드에서는 안 찍힘. 그리고 `axlog` 체계 밖에 있던 leftover 디버그 코드 4곳을 찾아 axlog로 전환: `dll/form/fmEdit.cpp`+`fmBase.cpp`(신규 카테고리 `LOG_AXISFORM`, 11절 신설), `Guard.cpp::Write`(민감데이터 노출 위험 있어 원문 미리보기 제거), `WizardCtrl.cpp::axWizard`의 `setFDC`(로그 문구 오탈자 `setFDS`→`setFDC` 정정), `dll/vbs/engineWrapper.cpp`의 엔진선택 로그(`LOG_SCRIPT`로 편입).

**2026-07-31 추가 갱신:** `CGuard::Xecure()`(Guard.cpp) — 그동안 로그가 전혀 없던 암호화/복호화 COM 컨트롤(`AxisXecure.XecureCtrl.IBK2019`) 호출 지점에 로그 추가. 상세는 12절(신설) 참고, 이후 절 번호 한 칸씩 밀림.

---

## 1. axlog 인프라 개요

- 위치: `h/axlog.h` — ASCII, header-only, `Wizard.dll`(axwizard)과 `axisvbs.dll` 양쪽에서 공유(inline이라 링크 의존성 없음).
- 사용법: `axlog(카테고리, "포맷문자열", 인자...)` — 내부적으로 매크로라서 **호출한 함수명/줄번호가 자동으로 `[함수명:줄번호]` 형태로 메시지 앞에 붙습니다.**
- 카테고리 on/off: `h/axlog.h` 맨 위의 `#define DF_LOG_XXX` 줄을 주석처리/해제 → **재빌드 필요** (axlog.h를 쓰는 두 DLL 모두: `axisvbs.dll` 먼저, `Wizard`(axwizard) 나중 — 기존 빌드순서와 동일).

| 카테고리 | 기본상태 | 용도 |
|---|---|---|
| `LOG_INIT` | ON | 화면/객체 생성·초기화, 화면 열기 단계별 타이밍 |
| `LOG_EVENT` | ON | 마우스/키보드 입력, 스크립트 프로시저 호출 |
| `LOG_DATA` | ON | 소켓 수신→파싱→필드쓰기 파이프라인 |
| `LOG_RTM` | ON | 실시간 시세(RTM) 매칭/쓰기 |
| `LOG_SCRIPT` | **OFF** | 스크립트 엔진 내부(DISPID 조회, DoSomething 콜백, 스크립트 원문 덤프) — **매 이벤트마다 호출되는 것들이 많아 평소엔 꺼두는 걸 권장.** 특정 상황 재현할 때만 켜서 확인. |
| `LOG_AXISFORM` | **OFF** | axisform.dll(`dll/form/`)의 컨트롤 렌더링/입력 레이어(`CfmEdit::Draw`/`UpdateData`/`SetFocus`, `CfmBase::ReadData`/`WriteData`) — **재도장/키입력마다 찍혀 카테고리 중 가장 빈도가 높음.** 2026-07-29 추가, 기존에 게이트 없이 무조건 찍히던 leftover 디버그 코드를 axlog로 전환한 것. |

모든 줄 앞에 `[WIZARD][카테고리]`가 공통으로 붙습니다(`[DEBUG]`는 2026-07-29 중복이라 판단해 제거). 예: `[WIZARD][INIT    ] [CScreen::Parse:214]                  CScreen::Parse mapN=IB120100 ...`

**가독성 정렬(2026-07-29 추가):** 카테고리명(`[%-8s]`, `AXISFORM`=8자 기준 고정폭)과 `[함수명:줄번호]`(`[%-42s]`, 42자 고정폭) 둘 다 왼쪽정렬 패딩을 넣어서, DebugView 같은 고정폭 폰트 뷰어에서 실제 메시지 내용이 항상 비슷한 컬럼에서 시작하도록 했다. 함수명이 42자보다 길면(드묾, 예: `CScriptEngine::getIDOfProcedure`) 그 줄만 밀리고 잘리지는 않음 — 폭을 더 넓히고 싶으면 `axlog.h`의 `%-42s`를 조정.

---

## 2. 화면 열기 흐름 (CClient::Attach → CScreen::Parse)

| 태그 | 위치 | 의미 |
|---|---|---|
| `axAttach` | `WizardCtrl.cpp` (`CWizardCtrl::axAttach`) | 호스트가 화면(작업영역) attach 요청 → 배정된 key |
| `[CClient::Attach-enter]` | `Client.cpp` | 화면 열기 시작, 맵 이름 |
| `[CClient::Attach-timing]` | `Client.cpp` | 단계별 누적 경과시간: `GetMapH`→`SetAtScreen`→`screen->Parse`→`SetFont`→`OnStart(script)` |
| `[CClient::Attach-total]` | `Client.cpp` | 화면 열기 전체 소요시간 |
| `CScreen::ctor` / `CScreen::dtor` | `Screen.cpp` | `CScreen` 인스턴스 생성/소멸 (주소 + 소속 `client_key` + 맵명) — 서브맵(FM_OBJECT) 몇 개가 실제로 만들어지는지 주소 단위로 추적 가능 |
| `CScreen::Parse` / `[CScreen::Parse-timing]` | `Screen.cpp` | 맵 하나(메인 또는 서브) 파싱 단계별 타이밍: `LoadForm`(바이너리+컨트롤생성) → `form-loop(incl. sub-Parse)`(폼순회, 서브맵 재귀 포함) → `LoadScript-block` → `TOTAL`. **재귀호출이라 서브맵마다 개별로 찍힘 — 부모의 TOTAL엔 자식들 시간이 이미 포함됨.** |
| `[SCREEN-TREE] parent=... child=... control=...` | `Screen.cpp` (`Parse`, `FM_OBJECT` 분기, 2026-07-29 추가) | 부모 맵의 어느 컨트롤(`control`, 맵소스 정의 심볼명)이 어느 서브맵(`child`)을 임베딩하는지 — 화면 구성 트리를 재구성하기 위한 태그. 서브맵 재귀 `Parse()` 호출 직전에 딱 1줄만 찍히므로, `[SCREEN-TREE]`로 필터링하면 다른 로그가 사이에 아무리 끼어도 화면의 부모-자식-컨트롤 관계만 순서대로 뽑아볼 수 있음 |
| `CWizardCtrl::axWizard setFDC` | `WizardCtrl.cpp` | 호스트/외부 컨트롤이 `kind`/`variant` 값으로 화면의 특정 속성을 설정하는 범용 진입점(`axWizard(kind, variant)`의 `setFDC` case). 로그에서 `IMAXSKIN`/`IMAXGROUP` 등이 보이는 건 이 경로로 들어오는 값. **2026-07-29 정정: 기존 로그 문구가 `setFDS`로 잘못 찍혀 있었음(실제 case 레이블은 `setFDC`) — axlog 전환하며 실제 case명으로 수정** |

**실측 경험(2026-07-26):** 이 단계에서 지연이 있다면 대부분 `LoadForm`(컨트롤 개수/그리드 크기에 비례) 또는 `LoadScript-block`(로그 자체의 오버헤드 포함, 3절 참고)에서 발생했음.

---

## 3. 스크립트 엔진 내부 (LOG_SCRIPT — 평소 꺼둠)

| 태그 | 위치 | 의미 |
|---|---|---|
| `text=[...]` / `text.GetLength()` | `Screen.cpp` (`CScreen::Parse`) | 스크립트 원문 전체 덤프. **화면 열 때마다(서브맵 포함) 매번 실행되므로 무거움 — 필요할 때만 켤 것.** |
| `LoadScript OK` / `LoadScript FAILED` | `Screen.cpp` | 스크립트 로드 성공/실패, 실패 시 에러 목록 |
| `[...]<...> type =[...]` (`CScreen::DoSomething`) | `Screen.cpp` | axisform.dll → Wizard 콜백. **모든 타입(doPUSH/doFOCUS/doCHANGE/doPORTFOLIO 등)에 대해 호출마다 찍힘 — 그리드가 있는 화면에서 스크롤/갱신 시 매우 자주 찍힐 수 있음.** |
| `getIDOfProcedure(...)` | `dll/vbs/scriptEngine.cpp` (VBS 엔진, `CScriptEngine`) | 스크립트 안에 특정 프로시저 이름이 있는지 `GetIDsOfNames`로 조회. **`IsAvailable()`/`DoProcedure()` 양쪽에서 호출되므로 VBS 화면의 모든 이벤트마다 실행됨 — 캐싱 없음(최적화 여지로 남겨둠).** |
| `DoProcedure(1)` / `DoProcedure(2)` | `dll/vbs/engineWrapper.cpp` | `CEngineWrapper`가 VBS/Python 중 어느 엔진으로 프로시저를 호출했는지. **정정(2026-07-29): 실제 카테고리는 `LOG_SCRIPT`가 아니라 `LOG_EVENT`(항상 ON)다** — 이 표에 다른 항목들과 같이 실려있어 헷갈리기 쉬우나 카테고리 게이트를 안 타므로 `LOG_SCRIPT`를 꺼도 계속 보인다 |
| `CEngineWrapper::LoadScript scpKind=...` | `dll/vbs/engineWrapper.cpp` | 빌드 시 확정된 `scpKind`(또는 -1일 때 텍스트 자동감지)로 VBS/Python 엔진을 최종 선택하는 지점. 2026-07-29 이전엔 `axlog` 매크로 없이 `dbg.Format`+`OutputDebugString`을 직접 호출하는 별개 코드였음(카테고리 게이트 없이 무조건 출력) — axlog로 전환하며 `LOG_SCRIPT`에 편입 |
| `[Initialize]` maps/alreadyInit | `dll/vbs/pythonEngine.cpp` (`CPythonEngine::Initialize`) | Python 인터프리터가 이미 떠있는 상태로 재사용됐는지(`alreadyInit=1`, `Py_Initialize`는 프로세스당 1회) 새로 초기화됐는지 |
| `[AddObject]` name/maps | `dll/vbs/pythonEngine.cpp` | `Screen`/`System`/컨트롤 등 COM 객체가 Python 전역에 실제로 등록되는 시점. `docs/python_engine_260608.md`의 "INFO 객체 오류" 류 버그(등록 누락/순서) 재발 시 1차 확인 지점 |
| `[IsAvailable]` procs/found | `dll/vbs/pythonEngine.cpp` | `getIDOfProcedure`의 Python판 — 특정 프로시저(`AX_*_On*_AX_`)가 정의돼 있는지 확인. **Python 화면의 모든 이벤트마다 실행**(VBS의 `getIDOfProcedure`와 동일한 빈도 특성) |
| `[DoProcedure]` procs/maps(/data) | `dll/vbs/pythonEngine.cpp` | `CEngineWrapper`가 아니라 실제 `PyObject_Call`을 감싸는 지점 — 어느 화면(`maps`)의 어느 프로시저가 실제로 실행됐는지. 실패 시 `FAILED procs=...` 별도 로그 |
| `[fetchError]` 메시지 원문 | `dll/vbs/pythonEngine.cpp` | **Python 예외 메시지 전체(트레이스백 포함)를 그대로 출력** — 이전엔 `CVbGuide` 팝업으로만 보였고 DebugView에는 전혀 안 남던 정보. 여러 줄(`\r\n`)로 찍힐 수 있음 |
| `CxScreen::_Send` / `CxScreen::_Send InStream +Nms` | `xscreen.cpp` | 스크립트의 `Screen.Send(target)` 호출과, 그로 인한 `CStream::InStream` 소요시간. **`CKey::OnKey`의 InStream 로그(5절)와 최종 목적지가 동일 — 사람이 엔터친 것과 스크립트가 보낸 것을 구분해서 볼 수 있음.** |
| `CxSystem::_Trigger` | `xsystem.cpp` | 스크립트의 `System.Trigger(procs, params, maps)` 호출 — 다른 화면/프로시저를 원격으로 실행시킴(`CGuard::OnProcedure`로 위임) |

**2026-07-27 추가 — `CxScreen`/`CxSystem`(`Screen`/`System` 스크립트 dispatch 객체) 나머지 주요 메서드:** 지금까지 `_Send`/`_Trigger` 2곳만 찍혀서 스크립트가 어떤 dispatch 메서드를 호출하는지 절반 이상 안 보이던 사각지대를 메움. 전부 `LOG_SCRIPT`(`CxSystem::_Exit`만 예외로 `LOG_EVENT`, 화면 이벤트가 아니라 종료 자체라 항상 보이게 함).

| 태그 | 위치 | 의미 |
|---|---|---|
| `CxScreen::_ChangeTR` | `xscreen.cpp` | `Screen.ChangeTR(trN)` — TR 코드 직접 변경 |
| `CxScreen::_ChangePath` | `xscreen.cpp` | `Screen.ChangePath(path)` — AXIS/BP1~7/FEP 경로 전환 |
| `CxScreen::_ChangeMap` | `xscreen.cpp` | `Screen.ChangeMap(mapN)` — 다른 맵으로 화면 전체 교체(`renVIEW`) |
| `CxScreen::_CreateWindow` | `xscreen.cpp` | `Screen.CreateWindow(type, param, data)` — 새 창 열기, 도미노 데이터 포함 여부 |
| `CxScreen::_RSend` | `xscreen.cpp` | `Screen.RSend(target)` — `_Send`와 달리 `OnSend`만 호출(TR 응답 대기 없음) |
| `CxScreen::_CloseWindow` (`maps=... screenKey=...` 포함, 2026-07-30 추가) | `xscreen.cpp` | `Screen.CloseWindow()` — 현재 창 닫기(`delVIEW`). `maps`는 이 호출을 실행한(=닫으라고 지시한) 화면 자신의 맵명 — 창(`client_key`) 전체가 닫히면 그 안의 모든 서브화면에 걸린 `TM_VB`/`TM_VBx` 타이머도 창과 함께 파괴되어 발화 없이 조용히 사라짐(10.5절 참고) |
| `CxScreen::_Service` | `xscreen.cpp` | `Screen.Service(trN, data, length, mode)` — `mode`를 스크립트가 넘긴 값 그대로 `CGuard::Service`에 전달한다. `mode`에 `US_PASS`(0x04)가 있으면 `waitSN` 게이트를 안 타고(일반 TR과 동시에 진행 가능, 모래시계도 안 뜸), 없으면 `waitSN`을 세우고 모래시계가 뜬다(단 `CClient::WaitState()`를 거치지 않아 TranTimeout 워치독은 안 걸림 — `KnowledgeBase.md` 17절 참고). 어느 쪽이든 `[SVC-send]`/`[SVC-RTT]`(6절)로 추적됨. **`_ServiceEx`(아래)는 이 `_Service`를 호출할 때 `mode`에 항상 `US_PASS`를 강제로 덧붙인다** — 즉 US_PASS 여부가 항상 그런 건 `_Service`가 아니라 `_ServiceEx`의 특징 |
| `CxScreen::_ServiceEx enter` / `[ServiceEx-wait]` (2026-08-26 추가) | `xscreen.cpp` | `Screen.ServiceEx(trN, data, length, mode, timeout)` — `_Service`를 내부 호출한 뒤 **UI 스레드를 직접 블로킹하는 커스텀 `PeekMessage` 루프**로 응답을 기다린다("종합화면 조회가 오래 멈춘다" 증상의 신규 용의 지점, 2026-08-26). `timeout==0`이면 대기 없이 즉시 리턴(비차단, fire-and-forget)하지만, **`timeout`이 양수면 그 값(ms)만큼, 음수면 무기한(창이 파괴될 때까지) 블로킹**한다. `enter` 로그로 어떤 스크립트가 이 API를 실제로 쓰는지/`timeout` 값이 얼마인지, `[ServiceEx-wait]`(entering/`exit=DONE`/`exit=TIMEOUT`) 로그로 실제 블로킹된 벽시계 시간을 직접 잰다. 응답 자체의 네트워크 왕복은 `[SVC-RTT]` 참고 |
| `CxScreen::_Proc` / `_ProcEx` (`maps=... screenKey=...` 포함, 2026-07-30 추가) | `xscreen.cpp` | `Screen.Proc(procs, data)` — PARENT/GRAND/MAIN 등 다른 깊이의 화면 프로시저 원격 호출. `maps`는 호출한 화면 자신의 맵명(대상이 아니라 발신자) |
| `CxScreen::_Approve` | `xscreen.cpp` | `Screen.Approve(key, ip, map, data, length)` — 승인 요청 |
| `CxScreen::_CreateObject` / `_CreateObjectEx` (`maps=... screenKey=...` 포함, 2026-07-30 추가) | `xscreen.cpp` | `Screen.CreateObject(name)` — 스크립트에 새 COM 객체 동적 등록. `maps`는 호출한 화면 자신의 맵명 |
| `CxScreen::_SetTimer` / `_SetTimerX` (`maps=... screenKey=...` 포함, 2026-07-30 추가) | `xscreen.cpp` | `Screen.SetTimer(...)` — 스크립트 타이머 등록. `screenKey`는 `Event.cpp`의 `WM_TIMER TM_VB(x)` 로그(10.5절)의 `screenKey`/`key`와 동일한 값이라, 등록 시점과 발화 시점 로그를 이 값으로 서로 대응시킬 수 있음 |
| `CxScreen::_UploadFile` / `_DownloadFile` | `xscreen.cpp` | 파일 송수신 요청(TR 기반) |
| `CxScreen::_SendTR` | `xscreen.cpp` | `Screen.SendTR(trN)` — `_Send`와 달리 특정 TR 하나만 지정 조회 |
| `CxSystem::_Navigate` | `xsystem.cpp` | `System.Navigate(url)` — 내장 브라우저로 URL 열기 |
| `CxSystem::_Push` | `xsystem.cpp` | `System.Push(name, data)` — 전역 데이터 스택에 저장(`Pop`으로 회수) |
| `CxSystem::_Trace` (2026-07-30 추가) | `xsystem.cpp` | `System.Trace(str)` — 스크립트가 임의 문자열을 남기는 디버그용 함수. 원래는 `CGuard::xTRACE()`가 외부 트레이스 뷰어(`m_hTrace`로 등록된 창, 예: AxisChaser)가 붙어있을 때만 `SendMessage`로 전달하고, 아무것도 안 붙어있으면 조용히 버려졌다 — axlog로 미러링해서 별도 뷰어 없이도 DebugView에서 항상 보이게 함. `CxSystem`은 화면(`CScreen`) 단위가 아니라 세션 전체에서 공유되는 객체라 `maps=`/`screenKey=`는 붙일 수 없음(어느 화면 스크립트가 호출했는지는 문자열 내용 자체로 구분해야 함) |
| `CxSystem::_Open` | `xsystem.cpp` | `System.Open(file)` — `ShellExecute`로 외부 파일/프로그램 실행 |
| `CxSystem::_ExcelToText` / `_TextToExcel` | `xsystem.cpp` | 엑셀 파일 변환 |
| `CxSystem::_Exit` (`LOG_EVENT`) | `xsystem.cpp` | `System.Exit(reboot)` — 프로그램 종료/재시작 요청 |
| `CxSystem::_CheckPasswd` | `xsystem.cpp` | `System.CheckPasswd(passwd)` — **비밀번호 원문은 로그에 안 남기고 일치 여부(`match`)만 기록** |

---

## 4. 이벤트 → 스크립트 프로시저 디스패치 (CScript, 2026-07-29 추가)

`Script.cpp`의 `CScript::On*` 함수들은 C++ 이벤트를 실제 스크립트 함수명(`AX_*_On*_AX_`, 명명규칙은 `docs/WizardArchitecture.md` 4절)으로 매핑하고 `screen->m_vbe->DoProcedure()`를 호출하는 최종 관문이다. VBS/Python 엔진 내부 로그(`LOG_SCRIPT`, 3절)는 각 엔진 구현체 안에 흩어져 있는 반면, 여기는 **엔진 종류와 무관하게 모든 이벤트 디스패치가 공통으로 거쳐가는 단일 지점**이라 "무슨 이벤트가 어느 화면의 어느 프로시저로 갔는지"를 한 줄로 확인할 수 있다. 전부 `LOG_EVENT`(기본 ON)로 분류 — 이벤트 발생 빈도만큼 찍히므로 마우스무브/타이머처럼 잦은 이벤트가 몰리는 화면에서는 양이 많을 수 있다.

| 태그 | 위치 | 의미 |
|---|---|---|
| `CScript::OnStart` / `OnFocus` / `OnClose` / `OnSend` / `OnDevice` / `OnTimer` / `OnSize` | `Script.cpp` | 화면단위 이벤트, `maps`(맵명)/`procs`(매핑된 함수명, 비어있으면 정의 안 됨) |
| `CScript::OnReceive` | `Script.cpp` | TR 응답 수신 후 스크립트 콜백 |
| `CScript::OnTimerX` | `Script.cpp` | `id`(타이머 식별자)까지 같이 찍힘 |
| `CScript::OnSelect` | `Script.cpp` | `selMaps`(선택된 맵명)까지 같이 찍힘 |
| `CScript::OnFile` | `Script.cpp` | `upload`(업로드/다운로드 방향) |
| `CScript::OnKey` | `Script.cpp` | 화면단위 `AX_SUB_OnKey_AX_` 또는 컨트롤단위 `AX_{심볼}_OnKey_AX_` 디스패치 — `Key.cpp`의 `CKey::OnKey`(4절 아님, 입력이벤트 5절)와는 다른 함수이니 혼동 주의 |
| `CScript::OnClick` / `OnChange` | `Script.cpp` | `byKey`(키보드로 발생했는지 마우스/스크립트로 발생했는지) 포함 |
| `CScript::OnDblClick` / `OnMouseDown` / `OnMouseUp` / `OnCharChange` / `OnSetFocus` / `OnMouseEnter` / `OnMouseLeave` | `Script.cpp` | 컨트롤 단위 이벤트, `maps`/`procs` |
| `CScript::OnProcedure` | `Script.cpp` | `Screen.Proc(...)`/`System.Trigger(...)` 등 스크립트가 임의 이름으로 다른 프로시저를 원격 호출할 때(예: `OnPaste`, 5절 클립보드 참고) |
| `CScript::Procedure` | `Script.cpp` | 데이터/카운트를 동반한 프로시저 호출 변형, `count` 포함 |

**`CScript::OnAlert`는 로그 대상에서 제외:** 함수 맨 앞이 `return true;`로 시작해 그 아래 전체가 도달 불가능한 죽은 코드다(의도적 비활성화로 보임, 이유 미상 — RTM 매 틱마다 호출되는 경로라 성능상 꺼둔 것으로 추정). 실행되지 않는 코드에 로그를 추가해도 찍히지 않으므로 넣지 않았다.

---

## 5. 입력 이벤트 (마우스/키보드)

| 태그 | 위치 | 의미 |
|---|---|---|
| `CMouse::OnDown` | `Mouse.cpp` | 마우스 좌클릭 좌표 |
| `CKey::OnKey` | `Key.cpp` | 눌린 키코드, 소속 `client_key`, `isWait`(응답대기 중이라 키가 씹히는 상황인지) |
| `CKey::OnKey VK_RETURN->InStream (FA_SEND) +Nms` | `Key.cpp` | 엔터키로 `FA_SEND` 필드 눌렀을 때 `CStream::InStream` 소요시간 — TR 송신이 동기 블로킹인지 확인용 |
| `CKey::OnKey Ctrl+1/2/3 -> Copy/Paste/Cut fallback` | `Key.cpp` | Ctrl+C/V/X가 OS 레벨에서 외부 차단당하는 문제(`docs/KnowledgeBase.md` 13절)의 우회책 — `Ctrl+1`=복사, `Ctrl+2`=붙여넣기, `Ctrl+3`=잘라내기, 기존 `Copy$Paste()` 재사용 |

**2026-07-28 추가 — 클립보드(Ctrl+C/X/V):** `FM_EDIT`은 네이티브 Windows 에디트박스가 아니라 `CfmBase : CCmdTarget`(커스텀 드로잉 컨트롤)이라, 클립보드 단축키가 OS가 자동으로 처리해주는 게 아니라 `CKey::OnChar`에서 제어문자(wParam)로 직접 구현돼 있다. "복사는 되는데 붙여넣기가 안 된다" 같은 증상 진단용으로 추가.

| 태그 | 위치 | 의미 |
|---|---|---|
| `CKey::OnChar` | `Key.cpp` | `OnChar` 진입 시 wParam/`client_key`/`isWait` — `blocked by IsWait` 로그가 같이 안 찍히면 대기상태 때문에 막힌 게 아님 |
| `CKey::OnChar blocked by IsWait` | `Key.cpp` | 화면이 서버 응답 대기(S_WAIT) 중이면 Ctrl+C/V 포함 모든 문자입력이 `MessageBeep`만 내고 무시됨 |
| `CKey::Copy$Paste` | `Key.cpp` | wParam(3=Ctrl+C, 24=Ctrl+X, 22=Ctrl+V)과 대상 필드의 kind/attr(예: `FA_PASSWD`면 애초에 함수 진입 직후 return) |
| `CKey::Copy$Paste Copy/Cut textLen=...` | `Key.cpp` | 복사/잘라내기 시 실제로 읽은 텍스트 길이 — 0이면 필드가 비어있어서 클립보드에 아무 것도 안 씀 |
| `CKey::Copy$Paste Paste got=... textLen=...` | `Key.cpp` | `got=0`이면 `CGuard::GetClipboard` 자체가 실패한 것(클립보드에 CF_TEXT 없음 또는 OpenClipboard 실패) — 필드 코드 문제가 아니라 클립보드 읽기 단계에서 막힌 것임을 구분 가능 |
| `CGuard::SetClipboard OpenClipboard=OK/FAILED` | `Guard.cpp` | `FAILED`면 `GetLastError()`도 같이 찍힘 — 다른 프로세스가 클립보드를 점유 중인 race condition 의심 지점 |
| `CGuard::GetClipboard OpenClipboard=OK hasData=... / FAILED` | `Guard.cpp` | `hasData=0`이면 클립보드는 열렸지만 CF_TEXT 포맷 데이터가 없음(예: 외부 앱이 이미지/커스텀 포맷만 올려둔 경우) |

---

## 6. 소켓 데이터 파이프라인 (번호 순서대로 실행됨)

| 태그 | 위치 | 의미 |
|---|---|---|
| `[OnTRAN-gate]` | `Screen.cpp` (`CScreen::OnTRAN`, 2026-07-29 추가) | `OnTRAN()` 자체가 통과됐는지(모든 필드 `IsValid`/`OnChange` 통과 + `OnSend` 스크립트훅 결과 + `m_return` 전부 참) 보여주는 로그. **주의(2026-07-29 실측 정정): 이 로그가 `onSendResult=1 m_return=1`로 찍혀도 실제 송신은 안 될 수 있다** — `OnTRAN()`이 참을 반환한 뒤에도 `CStream::InStream`의 `MakeStream()` 호출 자체는 일어나지만, `MakeStream()` 맨 첫 줄의 `waitSN`(이전 TR 응답 대기중) 체크에서 조용히 리턴될 수 있기 때문. 진짜 억제 지점은 아래 `[MakeStream-waitSN-drop]` 참고 |
| `[MakeStream-waitSN-drop]` | `Stream.cpp` (`CStream::MakeStream` 맨 첫 줄, 2026-07-29 추가) | **`Screen.Send()`가 실제로 억제되는 가장 흔한 지점.** 이 화면이 직전에 보낸 TR의 응답을 아직 못 받은 상태(`screen->m_state & waitSN`, `CClient::WaitState`가 송신 성공 직후 세팅하고 응답 도착 시 해제)에서 또 `Send()`가 호출되면, `OnTRAN()` 검증은 통과해도(`[OnTRAN-gate]` 참고) 패킷 조립 자체를 시작하기 전에 여기서 조용히 리턴됨 — 이 로그가 찍히면 `[0-MakeStream-send]` 배너는 절대 안 나옴 |
| `[0-MakeStream-send] ------ map=... tr=... ------` | `Stream.cpp` (`MakeStream`) | 송신 패킷(`_axisH`) 조립 시점. **2026-07-29부터 실제 전송되는 TR 코드(`trx`, 비어있으면 맵 기본 TR)를 맵명과 함께 눈에 띄는 구분선 형태로 출력** — "이 화면이 무슨 서비스를 쏘는지"를 한눈에 찾기 위함. 이 로그가 찍혔다는 건 `OnTRAN()` 검증도 통과하고 `waitSN`에도 안 걸려서 **진짜로 소켓에 나간다**는 뜻 |
| `[TR-RTT] map=... tr=... winK=... unit=... elapsed=Nms` | `Stream.cpp` (`CScreen::m_sendTick` 기록은 `MakeStream`, 계산/출력은 `OutStream`의 `screen->m_state &= ~waitSN;` 직후, 2026-08-25 추가) | **화면 단위 "보내고 나서 실제 응답이 올 때까지" 순수 왕복시간(RTT).** 기존 `InStream +Nms`류(3·5절)는 패킷을 조립해서 소켓에 쓰는 동기 구간만 재는 것이라 응답 대기시간은 포함하지 않았음 — 이 로그가 그 빈틈을 메운다. `[0-MakeStream-send]` 시점에 `screen->m_sendTick`(`GetTickCount64()`)을 저장해뒀다가, 같은 화면의 응답이 `OutStream`에서 `waitSN`을 해제하는 순간 그 값과의 차이를 찍고 `m_sendTick`을 0으로 리셋한다. `m_sendTick==0`(=이 화면이 보낸 요청이 없는데 응답만 도착한 것으로 보이는 비정상 케이스)이면 아예 로그를 안 남기도록 방어함. **"종합화면 조회가 가끔 1분 가까이 멈춘다" 증상 진단용**(2026-08-25) — 어느 화면의 `elapsed`가 비정상적으로 크게 찍히는지 보면, 그게 네트워크/서버 쪽 지연인지(이 값 자체가 큼) 아니면 OS/외부 프로세스가 블로킹하는 다른 구간인지(이 로그 자체는 정상 범위인데 다른 로그와의 시간 간격이 벌어짐, DebugView 타임스탬프로 확인) 구분할 수 있다 |
| `[CTRL-RTT] map=... tr=... winK=... unit=... elapsed=Nms` | `Stream.cpp` (`CGuard::m_ctrlSendTick` 기록은 `Guard.cpp::CGuard::Invoke`의 `Write()` 성공 직후, 계산/출력은 `OutStream`의 `case msgK_CTRL:` 진입 직후, 2026-08-25 추가) | **`[TR-RTT]`의 msgK_CTRL(0x26, 맵 내부 임베디드 컨트롤DLL이 쓰는 서비스, `pidomyst`/`pidomemo` 등) 버전.** `msgK_CTRL` 요청/응답은 `waitSN` 게이트를 안 타서(화면 하나가 여러 CTRL 요청을 동시에 진행 가능, `Stream.cpp:225` 주석 `//screen->m_state &= ~waitSN;` 참고) `[TR-RTT]`처럼 `CScreen`에 타임스탬프 하나만 저장하는 방식이 안 통한다 — 대신 `CGuard`에 `CMap<DWORD,DWORD,ULONGLONG,ULONGLONG&> m_ctrlSendTick`을 두고 `MAKELONG(winK,unit)`을 키로 여러 건을 동시에 들고 있다가, 응답이 오면 같은 키로 찾아서 경과시간을 찍고 제거한다. **종합화면처럼 임베디드 컨트롤을 여러 개 쓰는 화면에서 필드기반 TR(`[TR-RTT]`)은 멀쩡한데 CTRL쪽만 유독 오래 걸리는 경우를 가려내기 위한 것**(2026-08-25) |
| `[CTRL-send] map=... tr=... winK=... unit=...` | `Guard.cpp` (`CGuard::Invoke`, `Write()` 성공 직후, `[CTRL-RTT]`와 같은 시점, 2026-08-26 추가) | **CTRL 요청이 실제로 소켓에 나가는 순간의 송신순서 기록.** `[CTRL-RTT]`는 개별 요청의 경과시간만 보여주고 "몇 번째로 보냈는지"는 알려주지 않는데, 종합화면 재현 캡처에서 `IB000157`/`pooppoop`(`unit=240`)이 이틀 연속으로 캡처 마지막 줄에서 유독 크게(2156ms→1469ms) 찍히는 패턴을 조사하려면 "그 요청이 실제로 맨 나중에 보내져서 서버 큐잉으로 밀린 것인지" vs "보낸 순서와 무관하게 그 요청 자체가 원래 느린 것인지"를 구분해야 한다 — 이 로그로 송신 순서를, `[CTRL-RTT]`로 완료 순서/소요시간을 대조하면 구분 가능 |
| `[SVC-send] map=... tr=... winK=... unit=... mode=...` | `Guard.cpp` (`CGuard::Service`, `Write()` 성공 직후, 2026-08-26 추가) | **`msgK_SVC`(0x24, `Screen.Service`/`Screen.ServiceEx`) 요청의 송신 시점.** `mode`에 `US_PASS`가 섞여 있으면(`_ServiceEx`는 항상 이렇게 호출) `waitSN`을 안 세우므로 일반 TR과 동시에 진행될 수 있다 — `CScreen::m_svcSendTick`(TR용 `m_sendTick`과는 별도 필드, 이유는 아래 `[SVC-RTT]` 참고)에 전송시각을 기록 |
| `[SVC-RTT] map=... tr=... winK=... unit=... elapsed=Nms` | `Stream.cpp` (`CScreen::m_svcSendTick` 기록은 `CGuard::Service`, 계산/출력은 `OutStream`의 `case msgK_SVC:` 진입 직후, 2026-08-26 추가) | **`[TR-RTT]`의 msgK_SVC 버전.** `screen->m_sendTick`(TR용)을 그대로 재사용하지 않고 `m_svcSendTick`을 별도로 둔 이유: `_ServiceEx`가 내부적으로 `_Service`를 부를 때 항상 `mode\|US_PASS`를 넘겨서 `waitSN` 게이트를 의도적으로 우회한다(`CGuard::Service`의 `if (!(mode & US_PASS))` 참고) — 즉 Service 요청은 일반 TR과 동시에 진행 가능하도록 설계돼 있어서, "화면당 미결 요청은 하나뿐"이라는 `m_sendTick` 재사용의 전제가 깨진다. 두 요청이 겹치면 타임스탬프가 서로 덮어써져 엉뚱한 RTT가 찍힐 수 있어 필드를 분리함. **"종합화면 조회가 가끔 1분 가까이 멈춘다" 증상의 신규 용의 채널**(2026-08-26) — 아래 `[ServiceEx-wait]`와 짝을 이뤄, 네트워크 왕복시간(이 로그)과 실제 UI스레드 블로킹시간(`[ServiceEx-wait]`)을 구분해서 볼 수 있다 |
| `[ServiceEx-wait]` | `xscreen.cpp` (`CxScreen::_ServiceEx`, 2026-08-26 추가) | 3절 `CxScreen::_ServiceEx enter` 항목 참고 — `Screen.ServiceEx()`가 UI 스레드를 직접 블로킹하는 `PeekMessage` 대기루프에 실제로 들어갔는지, 얼마나 오래(`elapsed`) 블로킹했는지, 어떻게 빠져나왔는지(`exit=DONE`/`exit=TIMEOUT/WINDOW-GONE`)를 보여준다. 사용자가 제기한 "타 화면이 조회해온 값을 참조하는 순서가 역전되면 헹이 걸리지 않을까"라는 가설과 가장 직접적으로 맞아떨어지는 지점 — 이 API를 쓰는 스크립트가 아직 준비 안 된 값을 동기적으로 기다리면, 서버 응답이 늦어지는 동안 이 루프가 그대로 UI를 잡아먹는다 |
| `[0-GetDataNRM-field]` / `[0-GetDataNRM-grid]` / `[0-GetDataNRM-cell]` | `Stream.cpp` (`GetDataNRM`, 2026-07-30 추가) | **실제로 송신 버퍼에 실리는 필드값** — 받는 쪽 `[5-SetDataNRM-write]`/`[6-SetCells-...]`와 대칭되는 송신측 로그. `-field`는 일반 필드(`FM_EDIT`/`FM_MEMO`/`FM_BUTTON`/`FM_COMBO`/`FM_SHEET`/`FM_TREE`/`FM_CONTROL`) 하나당 한 줄, `-grid`는 그리드 필드 진입 시 `rows`/`cols` 요약, `-cell`은 그리드 셀 하나당 한 줄(받는 쪽 `SetCells-cell`과 대칭). **`FA_PASSWD` 속성이 붙은 필드/셀은 값 대신 `***`로 마스킹**(주문 화면의 거래비밀번호 필드 등 노출 방지). **`GetDataNRM2`/`GetDataOOP`/`GetDataOOP2` 경로는 아직 미적용** — `screen->GetTranInfo(trx)`가 참이거나 `OP_OOP` 맵이면 이 로그가 안 나옴(대신 아래 `[0-Write-send]`가 어차피 최종 바이트를 보여주므로 실사용에는 지장 없음). **마스킹 보강(2026-07-30):** 실측 중 `name=pwd`인 `FM_EDIT` 필드가 맵에 `FA_PASSWD` 속성이 안 걸려있어 평문(`value=[9999    ]`)으로 그대로 찍히는 걸 발견 — `isMaskedField()` 헬퍼를 추가해서 `FA_PASSWD` 플래그와 무관하게 **필드 이름에 `pwd`/`passwd`/`password`가 포함되면(대소문자 무관) 무조건 마스킹**하도록 5개 `-field` 로그 지점 전부 보강함. 단 `-cell`(그리드 셀)은 `cell->name`이 문자열 테이블 오프셋(`DWORD`)이라 이 로그에 찍히는 `name=`이 실제로는 그리드 전체의 이름이라(개별 컬럼명 아님) 이름 기반 매칭이 무의미해서 **`cell->attr & FA_PASSWD`만 적용된 채로 남아있음** — 그리드 안에 비밀번호 컬럼이 있고 그 컬럼에 `FA_PASSWD`가 안 걸려있다면 여전히 평문 노출 가능(알려진 잔여 갭). 어느 경우든 실제 와이어 바이트는 이 로그와 무관하게 맵의 `OP_ENC`/`OP_CERTIFY` 설정에 따라 암호화 여부가 결정됨(`[0-Write-send]` 참고) — 이 마스킹은 오직 axlog/DebugView 출력 자체의 노출을 막기 위한 것 |
| `[0-Write-send]` | `Guard.cpp` (`CGuard::Write(char*, int, bool)`, 2026-07-30 추가) | **어떤 빌더(`GetDataNRM`/`GetDataNRM2`/`GetDataOOP`/`GetDataOOP2`/`UploadFile`)를 거쳤든 상관없이, 실제로 소켓에 나가는 바이트를 전부 여기서 가로챈다.** `RouteTR`(일반 TR 송신)과 `CGuard::UploadFile`(파일 업로드) 둘 다 최종적으로 이 함수 하나로 모이는 게 확인된 유일한 지점 — 개별 빌더 함수를 일일이 계측하는 것보다 여기 하나가 "체이서의 Send Data 원문 덤프"와 가장 가깝게 대응한다. **`#0`, `#1`, ...로 번호가 매겨진 여러 줄이 찍힐 수 있다** — `Screen.Send(targetALL)`처럼 여러 화면(unit)의 요청을 한 번의 TCP 송신으로 묶어보내는 경우(`CStream::MakeStream(bool byKey)`가 `m_client`의 모든 화면을 순회하며 각 화면의 `_axisH` 프레임을 같은 버퍼에 이어붙이고 마지막에 한 번만 `Write()`), 헤더의 `datL`(5자리 ASCII 길이) 필드로 각 서브메시지 경계를 찾아 화면(unit)별로 한 줄씩 따로 로그를 남긴다 — 체이서가 이런 배치 전송을 여러 개의 "Send Data" 프레임으로 나눠 보여주는 것과 동일하게 맞춘 것(2026-07-30, 실측으로 `unit=0`/`unit=1` 두 화면이 한 번의 315바이트 전송에 묶여 있던 걸 확인 후 수정). `winK`/`unit`/`msgK`/`trxC`와 서브메시지 데이터 전체를 **100바이트씩 잘라 `chunk=N/전체` 여러 줄로** 출력한다(2026-07-30, 처음엔 120→400바이트 고정 상한이었다가 "어차피 로그도 많은데 상한 올리는 것보다 100바이트씩 쪼개서 전부 다 보이는 게 낫다"는 판단으로 청크 방식으로 변경 — 길이 제한 없이 전체 데이터가 잘리지 않고 다 나옴). **버그 수정(2026-07-30, 2차): 처음엔 NUL(0x00)만 의심해서 `.`으로 치환했으나(`%.*s`가 정밀도를 줘도 NUL에서 멈추는 표준 C 동작 대응), 실측 결과 진짜 원인은 그리드 tab모드 데이터에 실제로 섞여 들어가는 행구분 개행문자(`\r`/`\n`, `GetDataNRM`의 `iosB[iosL++] = '\n'`/`'\r'` 참고)였다 — DebugView가 이걸 진짜 줄바꿈으로 해석해서 로그 한 줄이 화면에서 둘로 쪼개지며 뒤쪽 내용(닫는 `]` 포함)이 통째로 안 보였음. NUL/`\r`/`\n` 세 가지를 전부 `.`으로 치환하도록 수정(그 외 바이트, 한글 CP949·탭 포함은 원본 그대로 통과).** **주의: 이 미리보기는 마스킹이 없다** — `GetDataNRM-field`가 `FA_PASSWD` 필드를 `***`로 가려도, 그 필드가 `FA_ENUM`과 함께 쓰이지 않은 단순 비밀번호 필드라면 실제 전송 바이트 자체가 평문일 수 있고 여기 그대로 찍힌다(단, 맵에 `OP_ENC`/`OP_CERTIFY`가 걸려 있으면 이 시점엔 이미 암호화된 뒤라 체이서 화면에서 보던 것과 동일하게 깨진 바이트로 보인다 — 즉 이 로그가 노출하는 정보는 체이서로 이미 볼 수 있는 것 이상은 아니다) |
| `[1-OnAxis-raw]` | `WizardCtrl.cpp` | 소켓에서 막 수신한 원시 패킷(조각일 수 있음) |
| `[2-OnStream-reassemble]` | `Works.cpp` (`CWorks::OnStream`) | 패킷 조각 재조립 진행(`statCON` 비트 기준) |
| `[3-CClient-OnAxis-reassembled]` | `Client.cpp` | 재조립 완료된 패킷이 `CClient`에 도달 |
| `[4-OutStream-parse]` | `Stream.cpp` (`OutStream`) | `msgK` 종류별 분기 시작, 데이터 미리보기 |
| `[SetDataH]` / `[SetDataH-ledger]` | `Stream.cpp` | 레코드헤더 파싱, `TH_LEDGER`인 경우 원장 블록(384바이트) 파싱 |
| `[5-SetDataNRM-write]` | `Stream.cpp` (`SetDataNRM`) | TR 응답 필드 단위 쓰기(`FM_EDIT`/`FM_OUT` 등) |
| `[6-SetCells-...]` | `Stream.cpp` (`SetCells`) | 그리드 셀 단위 파싱(진입/셀값/스킵/완료) |
| `[ParseRCC]` / `[ParseRCC-combo]` | `Stream.cpp` (`CStream::ParseRCC`, 2026-08-21 추가) | 콤보/트리 항목 목록을 채우는 RCC(인밴드 제어코드, `MigrationSpec_SocketToDrawing.md` 8.5절) 처리 지점. `[5-SetDataNRM-write]`는 콤보의 "현재 선택된 값" 하나(고정폭)만 잡고, 드롭다운 항목 목록 자체가 채워지는 이 경로는 별도라 지금까지 로그가 전혀 없었음. `[ParseRCC]`는 `rcc->name`(컨트롤명)으로 화면에서 폼을 못 찾아 데이터 전체가 조용히 버려지는 경우, `[ParseRCC-combo]`는 `csCOMBO` 케이스에서 실제로 `form->WriteAll(text)`로 항목 목록을 쓰기 직전 — `ccl`(항목 개수)/`textLen`/앞부분 미리보기(`%.100s`)를 출력. `csTREE`(트리 컨트롤, `USRDIR` INI 파일에 씀)는 아직 로그 미추가 |
| `[WIZARD][RTM][DEBUG] compare` | `Screen.cpp` (`CScreen::OnAlert`의 `default:` 분기) | 단일 필드(`FA_FLASH`, 그리드/테이블 아님)의 RTM 코드 매칭 비교 — `field=` vs `code=`, `match=`. **주의: `axlog`가 아니라 `OutputDebugString`을 직접 호출하는 leftover 코드라 `LOG_RTM` 카테고리 게이트를 안 탄다** — `LOG_RTM`을 꺼도 이 줄만은 계속 찍힘. 이름은 `[WIZARD][RTM]`이라 다른 axlog 줄들과 같아 보이지만 실제로는 이 화면(단일필드 매칭용)에만 존재하고, `FM_GRID`/`FM_TABLE`은 이 줄 대신 `[FlashGrid-write]`/`[FlashSemi-write]`/`[ScrollRTM-insert]`(아래, 2026-08-19 추가)를 본다 — 그리드가 있는 화면에서 이 `compare` 줄이 안 보인다고 "실시간이 안 온다"는 뜻이 아님, 애초에 그리드는 이 경로를 안 탐 |
| `[UpdateRTM-write]` | `Screen.cpp` (`UpdateRTM`) | 실시간 시세로 필드 갱신 시 `EIO_INPUT` 여부 |
| `[FlashGrid-write]` | `Screen.cpp` (`CScreen::FlashGrid`, 2026-08-19 추가) | `FM_GRID`가 `vals[2]` 마커(`$?`/`$$`/`$*`) 없이 `FA_FLASH`로만 등록된 경우의 실시간 셀 갱신(`MigrationSpec_SocketToDrawing.md` 8.11.4절) — 코드가 일치하는 행을 찾아 값이 바뀐 셀만 덮어씀. `row`/`col`/`old`/`new` 값을 함께 출력하므로 어느 종목의 어느 컬럼이 언제 바뀌었는지 이 한 줄로 추적 가능. 행 삽입/삭제는 없음(그런 경우는 `[ScrollRTM-insert]` 참고). **버그 이력(2026-08-20, 수정완료):** 추가 당시 `col=%.16s` 포맷 지정자에 `form->GetName(idx)`(실제로는 `int` 반환)를 그대로 넘겨 크래시가 났음 — `col=%d`로 수정됨, 상세는 `KnowledgeBase.md` 16절 참고 |
| `[FlashSemi-write]` | `Screen.cpp` (`CScreen::FlashSemi`, 2026-08-19 추가) | `FlashGrid`의 `FM_TABLE`(고정 테이블) 버전 — 컬럼이 아니라 `_cellR` 배열 기반의 고정 행 구조, 항상 0번 열에 씀 |
| `[ScrollRTM-insert]` | `Screen.cpp` (`CScreen::ScrollRTM`, 2026-08-19 추가) | `vals[2]="$?"`(`m_sales`) 그리드의 실시간 행 삽입(8.11.1절) — 코드 매칭 없이 무조건 `InsertRows`. `top=1`이면 위쪽 삽입(`GO_TOP`), `ticks`는 이번 소켓 패킷에 버퍼링되어 한 번에 플러시된 틱 개수 |
| `[WIZARD][RTM][DEBUG][ATTACH]` / `[MISMATCH]` | `Screen.cpp` (`DF_RTM_INDEX` 빌드시만) | RTM 종목코드 역인덱스 attach/검증 로그 — 별도 조사(`RealtimeCodeIndex_Investigation.md`) 참고 |
| `CGuard::Write(1)` / `CGuard::Write(2)` | `Guard.cpp` | 실제 소켓 송신 직전(`CGuard::Write` 두 오버로드). **2026-07-29 이전엔 `[wizard][guard]` 소문자 태그로 게이트 없이 무조건 출력됐고, `Write(1)`은 송신 바이트 원문을 최대 50자까지 그대로 찍고 있었음** — 로그인/주문 등 민감정보가 섞일 수 있어 axlog 전환하며 `len`/`key`만 남기고 원문 미리보기는 제거함(`RunAxis` 평문 로그 제거와 같은 이유). `Write(2)`는 원래도 헤더 바이트 2개(hex)만 찍던 거라 내용은 그대로 유지 |

---

## 7. DLL 기반 화면 (CDll)

| 태그 | 위치 | 의미 |
|---|---|---|
| `CDll::Attach maps=... domino=...` | `Dll.cpp` | DLL 기반 작업영역 attach 시작 |
| `loaded via axCreate / axCreateEx / axCreateX` | `Dll.cpp` | 세 가지 DLL 진입점 중 실제로 성공한 것 |
| `DllProc WM_USER cmd=...` | `Dll.cpp` | 로드된 DLL이 부모(Wizard)에게 서비스 요청(자식→부모 콜백) |
| `DllProc WM_SIZE propagate` | `Dll.cpp` | 부모 리사이즈를 자식 DLL 윈도우에 수동 전파 |
| `[CDll-OnAxis-raw] winK=... unit=... msgK=... stat=... nBytes=... statAUX=...` | `Dll.cpp` (`CDll::OnAxis`, 2026-07-31 추가) | **`CDll` 기반 작업영역의 유일한 수신 트레이스 지점.** `CClient::OnAxis`와 달리 `CStream::OutStream`/`SetDataNRM`/`SetCells`를 전혀 안 타고, 받은 바이트를 그대로 `WM_USER`로 로드된 DLL에 던진다 — 그래서 이 화면 유형은 `[4-OutStream-parse]`/`[5-SetDataNRM-write]` 같은 로그가 원천적으로 안 찍힌다. `[1-OnAxis-raw]`(`CWizardCtrl::OnAxis`)/`[2-OnStream-reassemble]`(`CWorks::OnStream`)까지는 `CClient`/`CDll` 공통 경로라 똑같이 찍히고, 그 다음 갈라지는 지점이 바로 여기다. AxisChaser에서는 정상적으로 보이는데 axlog에서 `[4-...]`/`[5-...]` 이후가 안 보이면 이 로그가 찍히는지부터 확인할 것 — 찍힌다면 그 화면은 `CDll` 기반이라 실제 필드 파싱은 이 DLL 내부에서 일어나며(현재 미조사, `@docs/MigrationSpec_SocketToDrawing.md` 9절), Wizard 쪽 axlog로는 더 깊이 들어갈 수 없다 |

---

## 8. 로그인/인증 · OLE 드래그앤드롭 · DDE (2026-07-28 추가)

지금까지 로그가 전혀 없던 사각지대 3곳. 전부 세션당 한두 번만 발생하는 저빈도 이벤트라 `LOG_INIT`으로 분류(항상 켜져 있음). 단, DDE는 실시간 시세를 Excel에 계속 흘려보내는 `CDde::OnAlert`/`Write`/`Service`/`OnClose` 같은 틱 단위 데이터 경로는 RTM과 동일하게 고빈도라서 **의도적으로 로그를 넣지 않았다** — 구독 시작/해제(`OnAdvise`)와 세션 초기화/종료만 커버.

| 태그 | 위치 | 의미 |
|---|---|---|
| `CWizardCtrl::RunAxis` | `WizardCtrl.cpp` | 호스트 EXE가 로그인/서명 등을 지시하는 진입점. `mode`값으로 분기(`loginSHOP`/`loginAXIS`/`loginAXISx`/`signUSER`/`signUSERc` 등). 기존에 `m_slog.Format`+`OutputDebugString`으로 평문 데이터(`pBytes`)까지 찍던 걸 axlog로 교체하면서 **평문 로그인 데이터는 더 이상 안 찍음**(보안) |
| `CGuard::Login` | `Guard.cpp` | 실제 로그인 TR(`AXLOGONE`/`AXLOGONC`) 전송 직전, mode/데이터길이/암호화여부 |
| `CGuard::Certify` | `Guard.cpp` | 공인인증서 컨트롤(`AxisCertify.CertifyCtrl.IBK2019`) 활성화/비활성화 요청, `hasCertifyCtrl`로 컨트롤이 이미 생성돼 있었는지 확인 가능 |
| `CGuard::Certify CreateControl(...) FAILED` | `Guard.cpp` | 인증서 컨트롤 자체 생성 실패 — 기존엔 `AfxMessageBox`로만 사용자에게 보였고 로그는 없었음 |
| `CGuard::OnCertify` | `Guard.cpp` | 인증서 컨트롤로부터 응답 수신 |
| `CGuard::CertifyId` | `Guard.cpp` | 인증서 ID 조회/재시도(`retry` 플래그) |
| `COleDrop::OnDrop` | `OleDrop.cpp` | (MFC `COleDropTarget` 기반, 구버전 경로) 실제 드롭 발생 시 CF_TEXT 존재여부/`m_works`의 `S_DROP` 상태(드롭 허용 여부) |
| `COleDrop::Register` | `OleDrop.cpp` | 작업영역에 드롭 타겟 등록 |
| `COleDropEx::Drop` | `OleDropEx.cpp` | (COM `IDropTarget` 직접구현, 신버전 경로) 드롭 좌표 + 대상 작업영역/`S_DROP`/텍스트길이 |
| `COleDropEx::Register` / `Revoke` | `OleDropEx.cpp` | 드롭 타겟 등록/해제, `hr`(`RegisterDragDrop` 결과) |
| `CDde::OnDDE DdeInitialize err=... (0=success)` | `Dde.cpp` | DDE 세션 초기화 — DDEML 관례상 **0이 성공**이라 다른 Win32 API와 반대이니 주의 |
| `CDde::OnAdvise item=... advise=...` | `Dde.cpp` | Excel 등 외부 앱이 특정 항목을 구독 시작(`advise=1`)/해제(`advise=0`)할 때 |
| `CDde::OnExit postCount=...` | `Dde.cpp` | DDE 세션 종료 시점, 그때 살아있던 구독(topic) 개수 |

---

## 9. 내일(화면 열고 닫으며 확인할 때) 체크리스트

1. **화면 열기 지연 확인** — `[CClient::Attach-timing]`을 보고 어느 단계가 큰지 먼저 본다. `LoadForm`이 크면 컨트롤/그리드 크기 문제, `LoadScript-block`이 크면 `LOG_SCRIPT`를 꺼봐서 로그 자체 오버헤드인지 구분.
2. **엔터 조회 vs 스크립트 조회 구분** — `CKey::OnKey VK_RETURN->InStream`과 `CxScreen::_Send InStream`을 비교해서, 사람이 누른 건지 스크립트가 보낸 건지 구분.
3. **그리드 있는 화면 상시 반응성** — `LOG_SCRIPT` 켜고 스크롤/갱신하면서 `CScreen::DoSomething`(특히 `doPORTFOLIO`) 빈도 확인. 너무 잦으면 그 자체가 부담일 수 있음.
4. **화면 닫을 때 정상 정리되는지** — `CScreen::dtor` 로그의 주소가 `ctor`와 잘 짝지어지는지, 서브맵 개수만큼 소멸 로그가 나오는지 확인.
5. **RTM 화면(장중)** — `[UpdateRTM-write]`에서 `isInput=1`인 필드가 실제로 찍히는지 확인(미해결 이슈, `KnowledgeBase.md` 11절/`RealtimeCodeIndex_Investigation.md` 참고).

---

## 10. 타이머 워커 스레드 (COnTimer, 2026-07-29 추가)

`OnTimer.cpp`의 `COnTimer : CWinThread`는 `WizardArchitecture.md` 7.4절에서 "Wizard 안에서 실제로 별도 OS 스레드로 도는 몇 안 되는 지점"으로 언급됐던 클래스다.

**⚠️ 2026-07-29 정정 — 아래 로그는 현재 빌드에서 절대 안 찍힌다.** `Client.h`/`Client.cpp`를 확인해보니 `m_OnTimer` 관련 코드(멤버 선언, 생성자 초기화, 소멸자 정리, `SetTimer()`의 `AfxBeginThread(RUNTIME_CLASS(COnTimer), ...)` 생성부)가 전부 주석 처리돼 있어 **`COnTimer`는 코드 전체에서 단 한 곳도 인스턴스화되지 않는 죽은 클래스**다(`OnTimer.cpp`는 빌드는 되지만 호출되는 곳이 없음). 실제 타이머는 전부 평범한 MFC `SetTimer`/`WM_TIMER`(`Event.cpp`의 `CallProc`에서 분기)로 처리된다. 상세 경위와 추정되는 설계의도/되돌려진 이유는 `docs/KnowledgeBase.md` 14절 참고.

아래는 (지금은 죽어있지만) 코드에 넣어둔 로그 설계 — 나중에 이 클래스가 다시 살아나거나 유사한 워커스레드를 재도입할 경우를 위해 남겨둔다. 설계 자체는: 메인 스레드(`Dispatch`)가 타이머 키를 큐에 넣고 이벤트를 세팅하면, 워커 스레드(`Run`)가 깨어나서 큐를 비우며 `DoParse`로 메인 스레드의 창(`m_view`)에 `SendMessage(WM_USER+11, key)`를 보내는 방식. `SendMessage`는 동기 호출이라 워커 스레드가 메인 스레드의 메시지 처리가 끝날 때까지 블록된다. 전부 스레드 ID(`tid`)를 같이 찍어서 메인 스레드 로그와 구분 가능하게 함.

| 태그 | 위치 | 의미 |
|---|---|---|
| `COnTimer::Startup` | `OnTimer.cpp` | 스레드에 대상 창(`view`) 연결, `m_alive=true`로 전환 |
| `COnTimer::Dispatch key=... dup=... queLen=...` | `OnTimer.cpp` | 메인 스레드가 타이머 키를 큐에 넣는 지점. `dup=1`이면 이미 대기 중인 키라 중복 추가 안 됨(디바운스) |
| `COnTimer::Dispatch ... ignored (not alive)` | `OnTimer.cpp` | 스레드가 이미 종료됐는데 `Dispatch`가 호출된 경우 — 종료 타이밍 경합 의심 시 확인 |
| `COnTimer::Run thread started/exiting` | `OnTimer.cpp` | 워커 스레드 생명주기, `tid` 확인용 |
| `COnTimer::Run dequeued key=...` | `OnTimer.cpp` | 워커 스레드가 큐에서 키를 꺼낸 시점 |
| `COnTimer::DoParse key=... SendMessage +Nms` | `OnTimer.cpp` | 워커→메인 스레드로 넘어가는 `SendMessage`의 실제 소요시간 — 이 값이 크면 메인 스레드가 다른 작업으로 막혀있다는 뜻 |
| `COnTimer::Cleanup exit=... queLen=...` | `OnTimer.cpp` | 큐 비우기(화면 전환/종료 시), `exit=1`이면 스레드 자체를 끝냄 |

---

## 10.5. 스크립트 타이머 (Screen.SetTimer/SetTimerX) — 실제 살아있는 경로 (2026-07-30 추가)

`COnTimer`(10절)는 죽은 코드이므로, 스크립트의 `Screen.SetTimer(interval, main)`/`Screen.SetTimerX(id, interval, main)`는 전부 평범한 MFC `SetTimer`/`WM_TIMER`를 탄다. `CxScreen::_SetTimer`(`xscreen.cpp:922`)가 `main=true`면 `TM_VB`(그 화면이 속한 작업영역의 공용 채널), `main=false`면 `TM_VB + m_screen->m_key`(그 CScreen 인스턴스 전용 채널, `SetTimerX`는 `TM_VBx` 계열)로 `CClient::SetTimer()`(`Client.cpp:4192`, 내부적으로 `m_view->KillTimer`+`m_view->SetTimer`만 함)를 호출한다. 실제 발화는 `Event.cpp::CallProc`의 `WM_TIMER` 케이스(284행~)에서 처리된다.

**핵심 함정 — `TM_VB`/`TM_VBx`는 원샷(one-shot)이다:** `Event.cpp`가 `WM_TIMER`를 받으면 스크립트 콜백을 부르기도 **전에** 무조건 `client->m_view->KillTimer(wParam)`부터 호출한다(315행 부근). 즉 엔진이 알아서 반복 실행해주지 않는다 — 주기적으로 동작해야 하는 타이머라면 **스크립트의 `AX_SUB_OnTimer_AX_`(또는 `OnTimerX`) 핸들러 맨 끝에서 `Screen.SetTimer`를 다시 호출해 재무장해야 한다.** 한 번만 호출해두고 반복 동작을 기대하면 "5초 뒤 딱 한 번만 되고 그 뒤로 안 되는" 것처럼 보인다.

**두 번째 함정 — `S_ING` 상태면 그 발화 자체가 조용히 버려진다:** `KillTimer`는 무조건 실행되지만, 실제 콜백 호출(`client->m_vm->OnTimer(screen)`)은 `!(client->m_status & S_ING) && client->GetAtScreen(screen, wParam - TM_VB)` 조건을 만족해야만 일어난다. `S_ING`은 그 작업영역(`CClient`)이 다른 화면을 Attach/로딩하는 중일 때 세팅되는 플래그(`Client.cpp:125`에서 set, `Client.cpp:261/343/350`에서 clear)다. 타이머가 발화하는 바로 그 순간 같은 클라이언트 안에서 다른 서브화면이 로딩 중이면, **타이머는 이미 `KillTimer`로 죽었는데 콜백은 실행 안 되는** 조용한 실패가 난다 — 기존 코드엔 이 경로에 로그가 전혀 없어서 DebugView에 어떤 흔적도 안 남았다.

**2026-07-30 추가한 로그 (`Event.cpp`, `LOG_EVENT`, 항상 ON):** 위 두 함정을 구분할 수 있도록 `WM_TIMER`의 `TM_VB`/`TM_VBx` 분기 진입 시 상태를 그대로 찍는다.

| 태그 | 위치 | 의미 |
|---|---|---|
| `CallProc WM_TIMER TM_VB wParam=... screenKey=... S_ING=... GetAtScreen=... client_key=...` | `Event.cpp` | `main=false`로 등록한 타이머(`Screen.SetTimer`) 발화 시점. `S_ING=1`이면 그 순간 클라이언트가 다른 화면 로딩 중이라 콜백이 버려진 것, `GetAtScreen=0`이면 `screenKey`(=`wParam-TM_VB`) 슬롯에 해당 화면이 더 이상 없는 것(예: 그 사이 화면이 재생성/재배치됨) — 두 경우 다 이 로그 다음에 `CScript::OnTimer`(4절) 로그가 안 찍힌다 |
| `CallProc WM_TIMER TM_VBx wParam=... key=... id=... S_ING=... GetAtScreen=... client_key=...` | `Event.cpp` | `Screen.SetTimerX`로 등록한 타이머 발화 시점, 동일한 두 함정 적용 |

**진단 순서:** 타이머가 "안 되는 것 같다"는 신고를 받으면 (1) 위 두 로그 중 하나가 애초에 찍히는지부터 본다 — **전혀 안 찍히면** `SetTimer`/`SetTimerX` 호출 자체가 안 됐거나(`CxScreen::_SetTimer`/`_SetTimerX`도 `LOG_SCRIPT`로 로그 남김, 3절 참고) `KillTimer`가 다른 경로로 먼저 취소된 것. (2) 찍히는데 `S_ING=1` 또는 `GetAtScreen=0`이면 그 발화 1회가 조용히 버려진 것. (3) `S_ING=0 GetAtScreen=1`로 정상 통과했는데도 `CScript::OnTimer`(4절, `maps=... procs=...`) 로그가 안 보이면 `IsAvailable(procs)`가 실패한 것 — 즉 그 맵 스크립트에 `AX_SUB_OnTimer_AX_` 서브루틴 자체가 정의돼 있지 않은 것. (4) 다 정상으로 한 번 찍혔는데 그 이후로 다시는 안 찍히면, 스크립트가 `OnTimer` 핸들러 안에서 `Screen.SetTimer`를 재호출(재무장)하지 않은 것 — 원샷 설계이므로 이게 가장 흔한 원인이다.

**세 번째 함정 — `::SetTimer()` 등록 자체가 조용히 실패할 수 있다 (2026-07-30 추가):** 위 세 가지로도 설명이 안 되는 사례 실측 — `_SetTimer`가 단 한 번만 호출됐고(재설정 아님), 클라이언트/화면이 닫히지도 않았는데(`_CloseWindow`/`dtor` 없음), 그 `screenKey`의 `WM_TIMER TM_VB`가 캡처 구간(10초 이상) 동안 단 한 번도 찍히지 않은 채로 다른 화면의 별개 타이머(`screenKey` 다름)만 정상 발화한 경우가 확인됨. `CClient::SetTimer(id, elapse)`(`Client.cpp:4192`)는 `m_view->SetTimer(id, elapse, NULL)`의 **반환값을 그동안 전혀 확인하지 않고 버리고 있었다** — Win32 `SetTimer()`는 실패 시(윈도우 핸들 무효, 프로세스당 타이머 개수 초과 등) 0을 반환하는데, 이 경우 애초에 `WM_TIMER`가 절대 안 온다. 등록 자체가 성공했는지 실패했는지 지금까지는 어디에도 로그가 없었음.

| 태그 | 위치 | 의미 |
|---|---|---|
| `CClient::SetTimer id=... elapse=... result=... isWindow=... client_key=...` | `Client.cpp` (`CClient::SetTimer`, 2026-07-30 추가) | 모든 `Screen.SetTimer`/`SetTimerX` 등록이 최종적으로 거치는 지점의 실제 Win32 `::SetTimer()` 반환값. **`result=0`이면 등록 자체가 실패한 것** — 그 뒤로 아무리 기다려도 `CallProc WM_TIMER`가 절대 안 뜬다. `isWindow=0`이면 그 시점에 `m_view` 핸들 자체가 이미 무효했다는 뜻(원인 쪽에 더 가까운 단서) |

`_SetTimer`(등록 요청, `xscreen.cpp`)와 `CClient::SetTimer`(실제 Win32 API 호출, `Client.cpp`) 사이에 시간차 없이 바로 이어지므로, DebugView에서 두 줄이 연달아 붙어 나오는지, 그리고 `result=0`인지를 확인하면 등록 실패 여부를 바로 확정할 수 있다.

---

## 11. axisform.dll 컨트롤 레이어 로그 (LOG_AXISFORM, 2026-07-29 추가)

`dll/form/fmEdit.cpp`(20곳)와 `fmBase.cpp`(2곳)에 있던 leftover 디버그 코드(`m_slog.Format`+`OutputDebugString` 직접 호출, 카테고리 게이트 없이 무조건 출력, 태그도 `[WIZARD][axisform]`/`[WIZARD][AXFORM]`/`[WIZARD][spin]`으로 제각각)를 axlog로 전환해서 `LOG_AXISFORM` 카테고리로 묶었다. **기본 OFF** — `CfmEdit::Draw`가 에디트 필드 하나 다시 그려질 때마다 무조건 호출되던 코드라, 켜면 카테고리 중 가장 시끄럽다. 켤 땐 `h/axlog.h`의 `// #define DF_LOG_AXISFORM` 주석을 풀고 axisform.dll부터 재빌드.

| 태그 | 위치 | 의미 |
|---|---|---|
| `CfmEdit::_Trigger` | `fmEdit.cpp` | 스크립트/외부에서 `Screen.Trigger` 등으로 이 필드를 건드릴 때 |
| `CfmEdit::Draw m_caret` / `m_fHeight` / `tmpS ... m_caretPos` / `setCaretPos` | `fmEdit.cpp` | 재도장 시 캐럿 표시 여부, 폰트 높이, 실제 표시 문자열, 캐럿 좌표 — **매 `WM_PAINT`마다** |
| `CfmEdit::OnLButton down=...` | `fmEdit.cpp` | 마우스 좌클릭(스핀버튼 등) 처리 진입 |
| `CfmEdit::SetFocus` | `fmEdit.cpp` | 포커스 진입/이탈, `m_strR`(현재 값)/`left` 좌표. **`m_strR=="a"`일 때 조기 `return`하는 분기가 있음(아래 KnowledgeBase 참고)** |
| `CfmEdit::UpdateData` (`start`/`[111]`/`[222]`/`[333]`/`whichHangulEdit`/`pre-assign`/`post-assign`/`end`) | `fmEdit.cpp` | 키 입력 한 글자가 `m_strR`에 반영되는 전체 과정을 단계별로 추적 — 한글 조합(`whichHangulEdit`) 분기 포함 |
| `CfmEdit::InsertData` | `fmEdit.cpp` | 삽입모드(`S_INS`) 키 입력 진입 |
| `CfmEdit::getStartPos start` / `end` | `fmEdit.cpp` | 가로 스크롤 표시 시작 위치(`m_stPos`) 재계산 — 텍스트가 필드 폭보다 길 때 |
| `CfmBase::ReadData [combo]` / `CfmBase::WriteData [combo]` | `fmBase.cpp` | 콤보 계열 컨트롤의 값 읽기/쓰기(변경 시에만 — `WriteData`는 `m_strR.Compare(data)`가 다를 때만 도달) |

**주의 — `CfmEdit::SetFocus`의 `m_strR=="a"` 분기(2026-07-29 발견, 미해결):** `fmEdit.cpp:1218` 부근에 `if (m_strR == "a") { ...; return; }`가 있어서, **필드의 현재 값이 정확히 문자열 `"a"`이면 `SetFocus()`가 포커스 처리 로직(`m_focus` 갱신, 캐럿 표시 등)을 전혀 안 하고 그냥 리턴한다.** 누군가 특정 상황을 재현하려고 임시로 넣은 디버그 트랩이 남은 것으로 보이는데, 실제로 어떤 필드가 값 "a"를 가질 수 있는 상황에서는 포커스가 정상 동작하지 않는 실제 동작상 결함이다. 이번 세션에서는 로그 포맷 변환 범위를 벗어나서 **건드리지 않고 그대로 뒀다** — 별도로 제거 여부를 결정할 것.

---

## 12. 암호화/복호화 (Xecure, 2026-07-31 추가)

`CGuard::Xecure(int helper, char* pBytes, int& nBytes)`(`Guard.cpp`)는 `AxisXecure.XecureCtrl.IBK2019` COM 컨트롤(`xecure/` 프로젝트, 별도 ActiveX DLL)을 감싸는 **암호화/복호화 전용 단일 진입점**이다. `helper`는 `DI_ENC`(0x01, 암호화)/`DI_DEC`(0x02, 복호화, `h/axisfire.h:361-362`) 둘 중 하나이고, `nBytes`는 참조 전달이라 호출 후 실제 암호문/평문 길이로 바뀔 수 있다.

**호출 경로 — 전부 이 한 함수로 모인다:**
- **암호화(`DI_ENC`)** — `Guard.cpp`에 6곳(`Write`(일반)/`Login`/`Service`/`UploadFile`/`DownloadFile`/`Invoke`, 각각 맵/TR의 `OP_ENC`류 옵션이 걸린 경우), `Stream.cpp:1464`(`CStream::MakeStream`, 화면 단위 조회 발송 직전, `[0-MakeStream-send]` 로그 직후 지점) — 총 7곳
- **복호화(`DI_DEC`)** — `WizardCtrl.cpp:712`(`CWizardCtrl::OnRead`, 소켓에서 막 조립한 `_axisH` 프레임의 `stat & statENC` 비트가 켜져 있을 때, `[1-OnAxis-raw]`로 넘기기 **전** 단계) — 1곳

기존엔 이 경로 전체에 로그가 전혀 없어서(컨트롤 생성 실패 시 원시 `OutputDebugString` 한 줄만 있었음) 암호화가 실제로 몇 번 타는지, 성공/실패율, 평문↔암호문 길이 변화조차 알 수 없었다. `CxScreen`/`CxSystem`에서 썼던 것과 같은 원칙(다수 호출부를 개별로 계측하지 않고 공용 진입점 하나에서 전부 커버)으로, `CGuard::Xecure()` 내부에만 로그를 추가해 총 8곳(암호화 7 + 복호화 1) 호출을 빠짐없이 잡는다.

| 태그 | 위치 | 의미 |
|---|---|---|
| `CGuard::Initial m_xecure CreateControl(...) OK` / `FAILED err=...` | `Guard.cpp` | 세션 시작 시 Xecure 컨트롤 생성 성공/실패(`LOG_INIT`). 기존에도 성공/실패 로그가 있었으나 `[logintrace]`+`OutputDebugString` 방식이라 axlog로 전환, 문구는 그대로 유지 |
| `[Xecure] helper=ENC/DEC SKIPPED (m_xecure is NULL)` | `Guard.cpp` (`CGuard::Xecure`) | 컨트롤 생성 자체가 실패했거나 아직 안 된 상태에서 암/복호화 요청이 들어온 경우 — 이 경우 항상 `FALSE`를 반환해 호출자 쪽의 `SetGuide(AE_CSECURE, ...)` 오류 처리로 이어진다 |
| `[Xecure] helper=ENC/DEC nBytesIn=... nBytesOut=... retv=...` | `Guard.cpp` (`CGuard::Xecure`) | 실제 `InvokeHelper` 호출 결과. `nBytesIn`(호출 전 길이)과 `nBytesOut`(호출 후 길이, 암호화 시 보통 늘어남)을 비교하면 이 컨트롤이 쓰는 암호화 방식의 블록 패딩 특성을 역으로 추정 가능. `retv=0`이면 암/복호화 자체가 실패한 것 |
| `[Xecure] decrypt FAILED trxC=... winK=... - packet dropped` | `WizardCtrl.cpp` (`CWizardCtrl::OnRead`) | 복호화 실패로 그 프레임 전체가 조용히 버려지는(`continue`) 유일한 지점 — 위 `[Xecure]` 체크포인트 로그는 실패 사실만 알려주고 어떤 TR/윈도우였는지는 안 나오므로, 실패 시 화면 식별용으로 별도 추가 |

**참고 — 코드 조사 중 발견한 사항(이번 작업과 무관, 손대지 않음):** `Guard.cpp`의 Xecure 컨트롤 생성 블록 바로 위(`CGuard::Initial`, 213-215행 부근)에 있는 주석 `// [KJS] ������ ó��... ����׽� ������ ó���ϸ� ó�� �ȵ�...`는 과거 어느 시점엔가 CP949→UTF-8 변환이 잘못 적용되어 이미 깨진(mojibake) 상태로 남아있다. 원본 한글 내용은 git 히스토리나 별도 백업 없이는 복구 불가 — 현재는 파일이 유효한 UTF-8(BOM 있음, `Guard.cpp`는 BOM 없음)로 저장돼 있는 것이 재확인됨(2026-07-31, `iconv -f UTF-8 -t UTF-8` 검증 + Grep으로 한글 정상 렌더링 확인). 즉 `Wizard/Guard.cpp`·`Wizard/WizardCtrl.cpp`는 더 이상 CP949가 아니라 UTF-8이므로 앞으로 이 두 파일은 `d:\src\IBKS\CLAUDE.md`의 iconv 왕복 절차 없이 Edit 도구로 직접 수정 가능하다(단, 다른 `.cpp`/`.h` 파일은 여전히 파일별로 `file` 확인 필수).

**클래스화 검토 결과(사용자 확인, 2026-07-31):** 위 로그로 확인 결과 `CGuard::Xecure()`가 이미 8곳 호출을 단일 진입점으로 묶고 있어 함수 단위 모듈화는 충분한 상태. 클래스로 한 번 더 감싸는 것의 이득(레거시 COM 의존성 격리)은 새 플랫폼이 C++/COM 스택을 그대로 이어받을 가능성이 낮다는 점에서 제한적이라고 판단 — 실제로 이식되는 자산은 코드가 아니라 `docs/MigrationSpec_SocketToDrawing.md` 8.9절에 정리한 프로토콜 스펙 쪽에 가깝다는 결론. **현재는 클래스화 보류, 필요성이 명확해지면 재검토.**

### NOENC.TXT — 개발용 암호화 강제 비활성화 (2026-07-31 추가)

암호화가 걸리면 `[0-Write-send]`/`[0-GetDataNRM-field]` 등 로그에 찍히는 실제 송신 바이트가 전부 암호문이라 평문을 보려면 매번 `[Xecure]` 로그의 길이 값을 대조하는 수고가 필요했다. 기존에도 `#ifndef DF_NOENC` 컴파일타임 매크로가 있었지만 7곳 중 2곳(`Write`(일반)/`Invoke`)에만 적용돼 있었고, 그마저 현재 `#define DF_NOENC1`(오탈자가 아니라 의도적으로 이름을 바꿔 무력화한 것으로 보임)라 사실상 죽어있었다. 재빌드 없이 즉시 켜고 끌 수 있는 런타임 스위치로 대체:

- `CGuard::IsNoEncMode()`(`Guard.cpp`) — **실행 중인 호스트 exe(AXIS.exe) 폴더**에 `NOENC.TXT` 파일이 있는지 매 호출마다(캐싱 없이) 확인. `m_root`(레지스트리 `ROOTDIR` 값)는 exe 폴더와 다를 수 있어서 **처음엔 `m_root` 기준으로 만들었다가 실측에서 안 먹히는 걸 확인하고 2026-07-31 exe 폴더 기준으로 수정함** — `GetModuleFileName(NULL, ...)`로 호스트 exe 경로를 구해 그 디렉터리를 사용. 파일을 두면 즉시 적용, 지우면 즉시 원복 — 앱 재시작 불필요.
- 암호화 게이트 7곳(`Guard.cpp` 6곳 + `Stream.cpp` 1곳) 전부에 `!IsNoEncMode() &&` 조건 추가 — 걸리면 `Xecure(DI_ENC, ...)` 호출 자체를 건너뛰고 `axisH->stat`의 `statENC` 비트도 안 켜진다(반쪽짜리로 껐다가 서버가 여전히 암호문으로 해석 시도하는 상황 방지).
- 복호화(`WizardCtrl.cpp:712`)는 건드리지 않음 — 수신 프레임의 `statENC` 비트를 그대로 따르므로, 서버가 응답을 암호화해서 보내면 그건 이 스위치와 무관하게 정상적으로 복호화된다.
- `[Xecure] IsNoEncMode check path=... result=...` 로그(`LOG_DATA`, 매 호출마다 항상 찍힘 — 진단 목적으로 성공/실패 무관하게 남김)로 실제로 어느 경로를 확인했는지, `NOENC.TXT`가 그 자리에 있었는지(`result=1`) 바로 확인 가능.

**실측 확인 완료(2026-07-31):** "클라이언트가 암호화를 안 하면 서버도 응답을 암호화 안 한다"는 가정이 같은 화면(IB140300/SRGSQ145)을 NOENC ON/OFF로 번갈아 조회한 캡처로 확정됨.
- NOENC ON(`result=1`): `[Xecure] helper=ENC`/`helper=DEC` 둘 다 아예 안 찍힘. 응답 쪽 `[1-OnAxis-raw] ... stat=0` — 서버도 평문으로 응답.
- NOENC OFF(`result=0`): 요청은 `helper=ENC`로 암호화되고, 응답은 `[1-OnAxis-raw] ... stat=2`(`h/axis.h`의 `statENC=0x02` 비트)로 암호화된 채 와서 `helper=DEC`로 정상 복호화.
- 결론: 서버는 클라이언트가 보낸 요청의 `stat`(`statENC` 비트) 여부를 보고 응답의 암호화 여부를 그대로 맞춰준다 — NOENC 모드에서는 요청/응답 전체가 평문으로 보이므로 로그 판독이 훨씬 쉬워졌다. (앱 재시작 없이 `NOENC.TXT` 파일만 지웠다 놨다 해도 즉시 반영되는 것도 같이 확인됨.)

### `[Xecure-Nego]` — 로그인 초반 암호화 채널 협상(AXISENCX), `[Xecure]`와는 별개 함수 (2026-08-31 추가)

**주의 — 이름이 겹쳐서 헷갈리기 쉽다.** 위의 `[Xecure]`는 `CGuard::Xecure()`(TR 페이로드 자체를 암/복호화하는 함수)의 로그이고, **여기 `[Xecure-Nego]`는 `CWizardCtrl::Xecure()`/`Xecure(char*,int)`/`OnXecure()`(로그인 맨 처음 암호화 키를 교환하는 함수, `LoginSequence.md`의 `AXISENCX` 단계)의 로그다 — 클래스도 역할도 다른 완전히 별개의 함수인데 이름만 같다.** 기존엔 이 협상 함수 쪽엔 로그가 전혀 없었음.

| 태그 | 위치 | 의미 |
|---|---|---|
| `[Xecure-Nego] enter flagENC=... m_xtype=...` | `WizardCtrl.cpp` (`CWizardCtrl::Xecure()`, 인자없는 오버로드) | 협상을 시작할지 판단하는 진입점 — `flagENC`(로그인 응답의 암호화단말 플래그)가 꺼져있으면 바로 `Run()`으로 건너뜀(`skipped -> Run()` 로그) |
| `[Xecure-Nego] DI_XEC nBytesIn=... nBytesOut=... retv=...` | `WizardCtrl.cpp` (`CWizardCtrl::Xecure(char*,int)`) | `AxisXecure.XecureCtrl`의 `DI_XEC`(키교환 전용, `CGuard::Xecure`가 쓰는 `DI_ENC`/`DI_DEC`와 다른 디스패치) 호출 결과. 입력이 `NULL,0`이라 컨트롤이 그 자리에서 새로 키교환값을 생성해서 돌려줌 |
| `[Xecure-Nego] AXISENCX send nBytes=...` | 〃 | **실제로 `AXISENCX` 메시지가 소켓에 나가는 순간** — `m_guard->Write(msgK_ENC, "AXISENCX", ...)` 직전 |
| `[Xecure-Nego] OnXecure encK=... nBytes=... m_mode=...` | `WizardCtrl.cpp` (`CWizardCtrl::OnXecure`) | 서버로부터 협상 응답(또는 이어지는 인증서 관련 이벤트)이 돌아왔을 때 진입점 — `encK`가 `encERR`/`encENC`/`encOK` 중 무엇인지로 이후 분기가 갈림 |

`m_guard->m_xecure`(COM 컨트롤 인스턴스)는 `CGuard::Xecure()`와 완전히 공유됨 — 즉 로그인 때 이 흐름으로 한 번 생성된 같은 컨트롤을, 이후 TR마다 `[Xecure]` 쪽이 계속 재사용해서 암/복호화한다.

---

## 13. 관련 문서

- `@docs/MigrationSpec_SocketToDrawing.md` — 소켓→파싱→렌더링 파이프라인 명세서 (이 로그들이 검증한 내용의 원본)
- `@docs/WizardArchitecture.md` — 클래스 계층 전체
- `@docs/KnowledgeBase.md` 11~12절 — RTM 흐름, 문서-코드 드리프트 이력
- `@docs/RealtimeCodeIndex_Investigation.md` — RTM 역인덱스, `EIO_INPUT` 미해결 이슈
