# 디버그 로그 가이드 (axlog)

## 문서 목적

2026-07-26 세션에서 도입한 중앙 로깅 인프라(`h/axlog.h`)와, 그날 하루 동안 추가한 모든 진단 로그의 위치·의미·태그를 정리합니다.
DebugView(또는 DebugView++)의 "Find" 창에 아래 태그를 넣어 검색하면서 화면을 열고 닫아보는 용도로 사용합니다.

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

모든 태그 앞에 `[WIZARD][카테고리][DEBUG]`가 공통으로 붙습니다. 예: `[WIZARD][INIT][DEBUG] [CScreen::Parse:214] CScreen::Parse mapN=IB120100 ...`

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

**실측 경험(2026-07-26):** 이 단계에서 지연이 있다면 대부분 `LoadForm`(컨트롤 개수/그리드 크기에 비례) 또는 `LoadScript-block`(로그 자체의 오버헤드 포함, 3절 참고)에서 발생했음.

---

## 3. 스크립트 엔진 내부 (LOG_SCRIPT — 평소 꺼둠)

| 태그 | 위치 | 의미 |
|---|---|---|
| `text=[...]` / `text.GetLength()` | `Screen.cpp` (`CScreen::Parse`) | 스크립트 원문 전체 덤프. **화면 열 때마다(서브맵 포함) 매번 실행되므로 무거움 — 필요할 때만 켤 것.** |
| `LoadScript OK` / `LoadScript FAILED` | `Screen.cpp` | 스크립트 로드 성공/실패, 실패 시 에러 목록 |
| `[...]<...> type =[...]` (`CScreen::DoSomething`) | `Screen.cpp` | axisform.dll → Wizard 콜백. **모든 타입(doPUSH/doFOCUS/doCHANGE/doPORTFOLIO 등)에 대해 호출마다 찍힘 — 그리드가 있는 화면에서 스크롤/갱신 시 매우 자주 찍힐 수 있음.** |
| `getIDOfProcedure(...)` | `dll/vbs/scriptEngine.cpp` (VBS 엔진, `CScriptEngine`) | 스크립트 안에 특정 프로시저 이름이 있는지 `GetIDsOfNames`로 조회. **`IsAvailable()`/`DoProcedure()` 양쪽에서 호출되므로 VBS 화면의 모든 이벤트마다 실행됨 — 캐싱 없음(최적화 여지로 남겨둠).** |
| `DoProcedure(1)` / `DoProcedure(2)` | `dll/vbs/engineWrapper.cpp` | `CEngineWrapper`가 VBS/Python 중 어느 엔진으로 프로시저를 호출했는지 |
| `CxScreen::_Send` / `CxScreen::_Send InStream +Nms` | `xscreen.cpp` | 스크립트의 `Screen.Send(target)` 호출과, 그로 인한 `CStream::InStream` 소요시간. **`CKey::OnKey`의 InStream 로그(4절)와 최종 목적지가 동일 — 사람이 엔터친 것과 스크립트가 보낸 것을 구분해서 볼 수 있음.** |
| `CxSystem::_Trigger` | `xsystem.cpp` | 스크립트의 `System.Trigger(procs, params, maps)` 호출 — 다른 화면/프로시저를 원격으로 실행시킴(`CGuard::OnProcedure`로 위임) |

---

## 4. 입력 이벤트 (마우스/키보드)

| 태그 | 위치 | 의미 |
|---|---|---|
| `CMouse::OnDown` | `Mouse.cpp` | 마우스 좌클릭 좌표 |
| `CKey::OnKey` | `Key.cpp` | 눌린 키코드, 소속 `client_key`, `isWait`(응답대기 중이라 키가 씹히는 상황인지) |
| `CKey::OnKey VK_RETURN->InStream (FA_SEND) +Nms` | `Key.cpp` | 엔터키로 `FA_SEND` 필드 눌렀을 때 `CStream::InStream` 소요시간 — TR 송신이 동기 블로킹인지 확인용 |

---

## 5. 소켓 데이터 파이프라인 (번호 순서대로 실행됨)

| 태그 | 위치 | 의미 |
|---|---|---|
| `[0-MakeStream-send]` | `Stream.cpp` (`MakeStream`) | 송신 패킷(`_axisH`) 조립 시점 |
| `[1-OnAxis-raw]` | `WizardCtrl.cpp` | 소켓에서 막 수신한 원시 패킷(조각일 수 있음) |
| `[2-OnStream-reassemble]` | `Works.cpp` (`CWorks::OnStream`) | 패킷 조각 재조립 진행(`statCON` 비트 기준) |
| `[3-CClient-OnAxis-reassembled]` | `Client.cpp` | 재조립 완료된 패킷이 `CClient`에 도달 |
| `[4-OutStream-parse]` | `Stream.cpp` (`OutStream`) | `msgK` 종류별 분기 시작, 데이터 미리보기 |
| `[SetDataH]` / `[SetDataH-ledger]` | `Stream.cpp` | 레코드헤더 파싱, `TH_LEDGER`인 경우 원장 블록(384바이트) 파싱 |
| `[5-SetDataNRM-write]` | `Stream.cpp` (`SetDataNRM`) | TR 응답 필드 단위 쓰기(`FM_EDIT`/`FM_OUT` 등) |
| `[6-SetCells-...]` | `Stream.cpp` (`SetCells`) | 그리드 셀 단위 파싱(진입/셀값/스킵/완료) |
| `[UpdateRTM-write]` | `Screen.cpp` (`UpdateRTM`) | 실시간 시세로 필드 갱신 시 `EIO_INPUT` 여부 |
| `[WIZARD][RTM][DEBUG][ATTACH]` / `[MISMATCH]` | `Screen.cpp` (`DF_RTM_INDEX` 빌드시만) | RTM 종목코드 역인덱스 attach/검증 로그 — 별도 조사(`RealtimeCodeIndex_Investigation.md`) 참고 |

---

## 6. DLL 기반 화면 (CDll)

| 태그 | 위치 | 의미 |
|---|---|---|
| `CDll::Attach maps=... domino=...` | `Dll.cpp` | DLL 기반 작업영역 attach 시작 |
| `loaded via axCreate / axCreateEx / axCreateX` | `Dll.cpp` | 세 가지 DLL 진입점 중 실제로 성공한 것 |
| `DllProc WM_USER cmd=...` | `Dll.cpp` | 로드된 DLL이 부모(Wizard)에게 서비스 요청(자식→부모 콜백) |
| `DllProc WM_SIZE propagate` | `Dll.cpp` | 부모 리사이즈를 자식 DLL 윈도우에 수동 전파 |

---

## 7. 내일(화면 열고 닫으며 확인할 때) 체크리스트

1. **화면 열기 지연 확인** — `[CClient::Attach-timing]`을 보고 어느 단계가 큰지 먼저 본다. `LoadForm`이 크면 컨트롤/그리드 크기 문제, `LoadScript-block`이 크면 `LOG_SCRIPT`를 꺼봐서 로그 자체 오버헤드인지 구분.
2. **엔터 조회 vs 스크립트 조회 구분** — `CKey::OnKey VK_RETURN->InStream`과 `CxScreen::_Send InStream`을 비교해서, 사람이 누른 건지 스크립트가 보낸 건지 구분.
3. **그리드 있는 화면 상시 반응성** — `LOG_SCRIPT` 켜고 스크롤/갱신하면서 `CScreen::DoSomething`(특히 `doPORTFOLIO`) 빈도 확인. 너무 잦으면 그 자체가 부담일 수 있음.
4. **화면 닫을 때 정상 정리되는지** — `CScreen::dtor` 로그의 주소가 `ctor`와 잘 짝지어지는지, 서브맵 개수만큼 소멸 로그가 나오는지 확인.
5. **RTM 화면(장중)** — `[UpdateRTM-write]`에서 `isInput=1`인 필드가 실제로 찍히는지 확인(미해결 이슈, `KnowledgeBase.md` 11절/`RealtimeCodeIndex_Investigation.md` 참고).

---

## 8. 관련 문서

- `@docs/MigrationSpec_SocketToDrawing.md` — 소켓→파싱→렌더링 파이프라인 명세서 (이 로그들이 검증한 내용의 원본)
- `@docs/WizardArchitecture.md` — 클래스 계층 전체
- `@docs/KnowledgeBase.md` 11~12절 — RTM 흐름, 문서-코드 드리프트 이력
- `@docs/RealtimeCodeIndex_Investigation.md` — RTM 역인덱스, `EIO_INPUT` 미해결 이슈
