# 마이그레이션 명세서 — 소켓 데이터 → 화면 드로잉 파이프라인

## 문서 목적

**배경:** 연말 HTS 플랫폼 전면 교체 예정 (axis/axwizard 폐기, 새 프로세스/플랫폼으로 이전). 이 문서는 "코드베이스 이해"가 아니라 **새 플랫폼 개발팀이 참고할 수 있는 정확한 사양서**를 목표로 함. 애매한 추정은 배제하고, 실측(로그) 또는 소스 직접 확인으로 검증된 내용만 기록.

**범위:** 소켓으로 원시 바이트가 들어와서, 화면에 값이 그려지기까지의 전체 파이프라인과 그 안의 모든 규칙. `@docs/WizardArchitecture.md`(클래스 계층), `@docs/RealtimeCodeIndex_Investigation.md`(RTM 매칭 상세)와 상호보완 관계 — 이 문서는 "파이프라인 전체 흐름과 프로토콜"에 집중.

**진행상태:** 🔍 작성 중 — 소켓 수신→재조립→파싱→필드쓰기(`SetDataNRM`)까지 실측 로그(`[WIZARD][DATA][0]~[5]`)로 검증 완료. 그리드(`FM_GRID`) 파싱, `FCC`/`RCC`/`SCC` 제어코드 정확한 의미, `SetDataOOP`/`TAB`류 나머지 포맷은 다음 조사 대상 (9절 참고).

**2026-07-25 실측 검증:** AxisChaser(패킷캡처 도구)로 원시 바이트를 직접 대조해서 `_axisH` 구조체 필드 순서(msgK/stat/auxs/winK/unit)가 실제 와이어 바이트와 1:1로 정확히 일치함을 확인함. 코드분석+로그+원시패킷캡처 3중 검증됨.

---

## 0. 전체 파이프라인 개요

```
[서버] TCP 소켓
    │  (조각날 수 있음, statCON 플래그로 표시)
    ▼
CWizardCtrl::OnAxis(int type, char* pBytes, int variant)      ← 연결 생명주기(FEV_OPEN/FEV_RUN)
    │
CWizardCtrl::OnAxis(struct _axisH* axisH, char* pBytes, int nBytes)   ← 패킷 1건 진입점
    │  axisH->winK로 대상 CClient/CDll(works) 탐색, 없으면 서버 지시로 자동 생성 가능
    │  auxsMAP 플래그면 서버가 화면 전환을 지시할 수 있음
    ▼
works->OnStream(axisH, pBytes, nBytes)     [CWorks::OnStream, Works.cpp:59]
    │  statCON 비트로 조각 재조립 (m_axisB에 누적)
    │  마지막 조각이면:
    ▼
works->OnAxis(axisH, 재조립된pBytes, nBytes)   [가상함수, CClient/CDll이 실제 구현]
    │  ※ 필드 단위 파싱 상세 — 다음 조사 대상
    ▼
CScreen 필드에 값 반영 (WriteData 등)
    ▼
WM_PAINT → CAxisForm::DrawForm → CfmBase::Draw(dc)   ← 실제 화면 렌더링
```

실시간 시세(RTM) 갱신은 별도 경로(`CGuard::OnAlert`→`DoRTM`)를 타며 `@docs/RealtimeCodeIndex_Investigation.md`에 상세 기록됨 — 이 문서에는 개요만 요약(6절).

---

## 1. 와이어 프로토콜 — 패킷 헤더 `_axisH`

**정의:** `h/axis.h:24`

```cpp
struct _axisH {
    unsigned char msgK;    // 메시지 종류
    unsigned char stat;    // 상태 (조각전송 플래그 statCON 포함)
    unsigned char auxs;    // 부가상태 (auxsCLOSE, auxsMAP 등)

    unsigned char winK;    // 대상 창(작업영역) key — axAttach가 배정한 그 key
    unsigned char unit;    // 유닛 창
    unsigned char trxK;    // TR key (수신측)
    unsigned char trxS;    // TR key (송신측)

    char svcN[4];          // 서비스명
    char trxC[8];          // TR 코드 (화면/맵 코드)
    char datL[5];          // 뒤따르는 데이터 길이 (문자열)
};
// L_axisH = sizeof(struct _axisH) = 24바이트 고정 헤더
```

**모든 클라이언트-서버 패킷은 이 24바이트 헤더 + 가변 페이로드 구조**입니다. 새 플랫폼에서 소켓을 파싱하려면 이 구조를 그대로 재현해야 함.

### `msgK` (메시지 종류) — `h/axis.h:44~`

| 값 | 상수 | 의미 |
|---|---|---|
| 0x20 | `msgK_AXIS` | 일반 AXIS 메시지 |
| 0x21 | `msgK_HTM` | HTML 메시지 |
| 0x22 | `msgK_TAB` | 탭 구분 메시지 |
| 0x24 | `msgK_SVC` | 서비스 콜 |
| 0x25 | `msgK_APC` | 승인 콜 |
| 0x26 | `msgK_CTRL` | 컨트롤 데이터 |
| 0x27 | `msgK_UPF` | 파일 업로드 |
| 0x28 | `msgK_DNF` | 파일 다운로드 |
| 0x30 | `msgK_RSM` | 리소스 요청 (맵 파일 등, `CGuard::RequestMAPs`가 씀) |

### `auxs` 부가상태 (확인된 것)

| 플래그 | 의미 | 근거 |
|---|---|---|
| `auxsCLOSE` | 이 창을 닫으라는 서버 지시 | `WizardCtrl.cpp:1098` |
| `auxsMAP` | 서버가 화면(맵) 전환을 지시 — `works->Attach(mapN, true)` 트리거 | `WizardCtrl.cpp:1104-1109` |
| `auxsFDS` | FDS(시세?) 값 포함 여부 — `Guard.cpp`의 `Write`/`Invoke`에서 암호화 분기와 함께 사용 | Guard.cpp 다수 |

---

## 2. 패킷 조각 재조립 규칙 — `CWorks::OnStream`

**근거:** `Works.cpp:59-94`

서버는 큰 응답을 여러 TCP 조각으로 나눠 보낼 수 있습니다. `axisH.stat`의 **`statCON`** 비트가 "뒤에 이어지는 조각이 더 있다"를 뜻합니다.

```cpp
void CWorks::OnStream(_axisH *axisH, char *pBytes, int nBytes)
{
    // 1) 누적 버퍼(m_axisB)에 계속 이어붙임
    //    - 최초 호출: 헤더(L_axisH바이트) + 첫 페이로드
    //    - 이후 호출: 페이로드만 계속 append
    ...
    if (!(axisH->stat & statCON))   // 2) "더 이상 조각 없음" = 완성
    {
        // 3) 메시지 루프를 펌핑하며 대기 (S_ING 상태 플래그가 꺼질 때까지)
        // 4) 재조립된 완전한 메시지를 가상함수 OnAxis로 전달
        OnAxis((struct _axisH*)m_axisB, &m_axisB[L_axisH], m_axisL-L_axisH);
        m_axisL = 0;
        delete [] m_axisB;
    }
}
```

**새 플랫폼 구현 시 필수 규칙:**
- 헤더는 첫 조각에만 붙어있고, 후속 조각은 순수 페이로드만 옴 (재조립 시 헤더 중복 주의)
- `statCON` 비트가 켜져있는 동안은 절대 파싱 시도하지 말고 버퍼링만 할 것
- 재조립 도중 메시지 루프를 계속 펌핑함 (UI 프리징 방지 목적으로 보임) — 새 플랫폼이 비동기/논블로킹이면 이 부분은 불필요할 수 있음

---

## 3. 창(작업영역) 식별 규칙 — `key`/`type`

**출처:** `Wizard/Guard.cpp` `CGuard::Attach`, `h/axis.h`, `h/axisfire.h`

### `key` — 작업영역 슬롯 식별자 (한 바이트 범위)

```cpp
winK_NORM = 0x20   // 일반창 시작
winK_POPUP = 0x80  // 팝업창 시작
winK_END  = 0xff
```
호출자가 `WK_NORM`(0x20)/`WK_POPUP`(0x80)을 넘기면 "빈 슬롯 아무거나 배정해줘" 요청이고, `CGuard::Attach`가 `m_major` 배열을 순회해 실제 빈 key를 찾아 배정. 구체적 정수를 넘기면 "이 key로 붙여달라" 지정 요청.

`_axisH.winK`가 바로 이 key와 동일한 값 — **서버가 보내는 패킷도 이 key로 어느 창을 대상으로 하는지 지정**합니다.

### `type` — 화면 종류 + 표시방식 비트마스크 (한 바이트, 상하위 니블 분리)

```
하위 니블(0x0F, "종류"):
  vtypeERR=0x00(에러) vtypeNRM=0x01(일반맵→CClient) vtypeVBX=0x02(레거시,미지원)
  vtypeDLL=0x03(DLL→CDll) vtypeGRX=0x04(그래픽) vtypeHTM=0x05(HTML)

상위 니블(0xF0=vtypeMSK, "표시방식"):
  vtypeSCR=0x10(스크롤형) vtypeFIX=0x20(고정형) vtypeRSZ=0x30(리사이즈-고정)
  vtypeWND=0x40(정적창) vtypeFEX=0x50(고정-비스크롤) vtypeREX=0x60(리사이즈-고정 vR)
```

`CGuard::Attach`의 `switch (type & ~vtypeMSK)`로 하위니블만 보고 `CClient`(vtypeNRM) 또는 `CDll`(vtypeDLL/vtypeGRX) 생성, `vtypeVBX`는 명시적으로 거부(`WK_NONE` 리턴).

**실측 예시(2026-07-25 로그):** `type=97`(0x61) = `vtypeREX`(0x60,상위) + `vtypeNRM`(0x01,하위) → 일반 맵 화면, 리사이즈-고정형.

### `unit` — 작업영역 안의 화면(스크린) 식별자 (2026-07-30 확인)

`winK`이 작업영역(창) 하나를 식별한다면, 그 작업영역 안에 대표화면 + 임베디드 서브맵(`FM_OBJECT`, 4.1절 참고)이 여러 개 동시에 존재할 수 있고, 이들도 각자 독립적으로 TR을 송수신한다. 이걸 구분하는 필드가 `_axisH.unit`이다.

`h/axis.h`:
```cpp
#define unitMAIN 0x00   // 대표(메인) 화면 -- trxC에 MAP명이 실림, MAP 전환 가능
#define unitSUB  0xff   // 아직 정수 키가 없어 trxC 문자열로 화면을 찾아야 하는 경우
                        // 그 외 값 = 그 화면의 실제 unit window key
```

`Stream.cpp::GetScreen`의 실제 매칭 로직: `if (axisH->unit == screen->m_key) return true;` — 즉 **`unit`은 `CScreen::m_key`를 그대로 실어 보낸 값**이다. `CClient::SetAtScreen`(대표화면/서브맵 임베딩 공용, `Client.cpp:735`)이 `key == -1`로 호출되면 `for (key=0; key<m_magic; key++)`로 빈 슬롯을 찾아 순번을 배정한다 — 대표화면은 항상 0(`unitMAIN`), 서브맵들은 배정 순서대로 1, 2, 3...을 받는다.

**실측 예시(2026-07-29~30 로그, `IB120800`이 대표화면, `IB120810`이 그 안의 서브맵):**
```
[0-MakeStream-send] ------ map=IB120810 tr=pooppoop ------ winK=35 unit=1   <- 서브맵(m_key=1)
[0-MakeStream-send] ------ map=IB120800 tr=piboPBxQ ------ winK=35 unit=0   <- 대표화면(m_key=0=unitMAIN)
```

**주의 — `unit`은 `msgK`에 따라 인코딩이 다르다.** 위 규칙(`unit == screen->m_key`)은 `msgK_AXIS`/`msgK_TAB`(일반 TR 송수신)에만 적용된다. `msgK_CTRL`(0x26, `FM_CONTROL` 필드 직접 갱신)은 전혀 다른 조회 경로(`CStream::GetScreen(screen, axisH, index, ukey)` 오버로드)를 타며, 내부적으로 `CClient::GetScreenKey(axisH->unit, index, ukey)`가 `CClient::m_keys`라는 별도 룩업테이블에서 `unit` 한 바이트에 패킹된 (화면키+컨트롤인덱스+ukey) 조합을 풀어낸다(`Client.cpp:3769`). 실측 로그에서 본 `unit=253`(`PIDOSETa`)/`unit=254`(`pidomemo`, 둘 다 `msgK=38=0x26=msgK_CTRL`)이 이 경로다 — **`screen->m_key` 값이 아니라 별도 룩업키이므로, 새 플랫폼에서 두 msgK 그룹을 동일한 방식으로 디코딩하면 안 된다.**

---

## 4. 화면(맵) 로드/조립 규칙 — `CScreen::Parse()`

**근거:** `Wizard/Screen.cpp:210~` (실측 로그로 검증됨)

화면 하나가 열릴 때 5단계로 조립됩니다:

1. **`LoadForm(mainRc)`** — `.map` 바이너리 파싱, 컨트롤(24종, `CfmBase` 파생) 실제 생성. `axisform.dll`의 `CAxisForm::LoadForm`이 `kind` 값으로 팩토리 분기(`switch(kind){case FM_EDIT: new CfmEdit(...); ...}`)
2. **스크립트 엔진 준비** — `m_vbe->Initialize()`, 아직 VBS/Python 미확정
3. **전역 객체 등록** — `Screen`(`CxScreen`)/`System`(`CxSystem`)/`Login`/`Ledger` 등을 스크립트에 노출 (`AddObject`, 엔진 미확정이라 버퍼링됨)
4. **폼 전체 순회** (`m_mapH->formN`개) — 컨트롤별 특수처리 + **`FM_OBJECT`(임베디드 서브맵)면 재귀적으로 `Parse()` 재호출** + `FA_FLASH`(실시간 대상) 필드 등록 + 컨트롤별 스크립트 노출
5. **`LoadScript()`** — 스크립트 실제 로드, VBS/Python 엔진 확정, 3단계에서 버퍼링된 객체 일괄 등록

### 4.1 서브맵 임베딩 규칙 (FM_OBJECT)

**실측 검증 (2026-07-25):** 메인화면 1개 열었더니 `Parse` 로그가 4번 찍힘 (`IB120100`+서브맵 `IB12010A`/`IB12010B`/`IB120120`).

서브맵은 **별도 창이 아니라, 부모 화면의 좌표계 안에서 스케일링된 부분 영역(`unitRc`)에 그려짐**:
```cpp
hr = (float)mainRc.Width() / (float)m_mapH->width;
vr = (float)mainRc.Height() / (float)m_mapH->height;
unitRc.left   = int(mainRc.left + ((short)form->m_form->left)*hr);
unitRc.top    = int(mainRc.top + ((short)form->m_form->top)*vr);
...
screen->Parse(resize, ...);   // 재귀 호출, 새 CScreen이지만 같은 창(view) 공유
```
**중요:** `CScreen`은 자기 소유의 윈도우(`view`)가 없음 — 항상 `m_client->m_view`(소속 CClient의 창)를 공유. 창(윈도우) 1개 : 작업영역(CClient) 1개 : 화면(CScreen) N개(메인+서브) 관계.

---

## 5. 컨트롤은 실제 윈도우가 아님 — 가상 컨트롤 구조

**근거:** `dll/form/fmBase.h:124` — `class CfmBase : public CCmdTarget` (⚠️ `CWnd` 아님)

24종 컨트롤(`CfmEdit`/`CfmGrid`/`CfmButton`/...)은 **진짜 Win32 자식 윈도우가 아니라, 부모 창(view) 위에 좌표 기반으로 그려지는 논리적 객체**입니다.

| 메커니즘 | 방식 |
|---|---|
| 그리기 | `CfmBase::Draw(CDC* dc)` — 부모의 DC에 직접 그림 (오너드로우) |
| 클릭 판정 | `CMouse::WhichForm(CScreen*, CPoint pt)` — 좌표로 어느 컨트롤인지 직접 계산 (Windows가 자동 라우팅 안 해줌) |
| 포커스 추적 | `CCaret`(`m_key`, `m_idx`) — Win32 `SetFocus`가 아니라 직접 변수로 관리 |

**새 플랫폼 설계 시사점:** 컨트롤 개수가 많은 화면(그리드 등)에서 네이티브 위젯을 화면 요소 개수만큼 만들면 안 되고, 좌표 기반 커스텀 렌더링 + 수동 히트테스트 방식을 채택한 이유가 성능(리소스 절약)으로 추정됨.

---

## 6. 입력 이벤트 → 원시 메시지 후킹 규칙 — 윈도우 서브클래싱

**근거:** `Works.cpp:35,40-41`, `Client.cpp:54-56`, `Event.cpp:14`

```cpp
// 1) 창 생성 시 소유자(CClient*) 저장
SetWindowLong(hwnd, GWL_USERDATA, (long)this);

// 2) 원래 윈도우 프로시저를 CallProc로 교체 (서브클래싱), 원본은 백업
m_callproc = (FARPROC)SetWindowLong(hwnd, GWL_WNDPROC, (LONG)callproc);

// 3) 소멸 시 원본 프로시저로 복원
if (m_callproc) SetWindowLong(hwnd, GWL_WNDPROC, (LONG)m_callproc);
```

`Event.cpp`의 `CallProc(hwnd, msg, wParam, lParam)`가 그 창의 **모든** Win32 메시지를 대신 받아서 `GetWindowLong(hwnd, GWL_USERDATA)`로 복원한 `CClient*`를 통해 하위 객체로 위임합니다.

### `CallProc`이 처리하는 메시지 전체 목록 (27종, `Event.cpp` 실측)

| 카테고리 | 메시지 | 위임 대상 |
|---|---|---|
| 생명주기 | `WM_DESTROY` | `client->OnClose()` |
| 그리기 | `WM_PAINT`, `WM_ERASEBKGND` | `client->OnDraw()` |
| 크기/스크롤 | `WM_SIZE`, `WM_VSCROLL`, `WM_HSCROLL`, `WM_SETCURSOR` | |
| 포커스 | `WM_SETFOCUS`, `WM_KILLFOCUS` | |
| 마우스클릭 | `WM_LBUTTONDOWN`/`MBUTTONDOWN`/`RBUTTONDOWN`, `*UP`, `WM_LBUTTONDBLCLK` | `client->m_mouse->OnDown/OnUp/OnDblClick` |
| 마우스이동 | `WM_MOUSEMOVE`, `WM_MOUSEWHEEL`, `WM_MOUSELEAVE` | |
| 키보드 | `WM_CHAR`, `WM_KEYDOWN` | `client->m_keyx->OnChar/OnKey` |
| IME | `WM_IME_CHAR`, `WM_IME_COMPOSITION`, `WM_IME_NOTIFY` | 한글 조합입력 |
| 타이머 | `WM_TIMER` | `wParam`(타이머ID)로 재분기 — `TM_RTM`/`TM_WAIT`/`TM_REPBN`/`TM_REPTR`/`TM_VB`/`TM_VBx` |
| 커스텀 | `WM_USER` | Wizard 자체 정의 메시지 (미상세조사) |
| 오너드로우 | `WM_MEASUREITEM`, `WM_DRAWITEM` | 커스텀 리스트박스 등 |

---

## 7. 이벤트 → 스크립트 실행 규칙

**근거:** `Wizard/Script.cpp::getProcName`, 실측 로그(2026-07-25)

### 7.1 프로시저 명명 규칙

| 이벤트 | 프로시저명 패턴 | 비고 |
|---|---|---|
| 화면단위(OnStart/OnClose/OnSend/OnReceive/OnAlert/OnTimer 등) | `AX_SUB_On{이벤트}_AX_` | 심볼 없음 |
| 컨트롤단위(OnClick/OnChange/OnDblClick 등) | `AX_{컨트롤명}_On{이벤트}_AX_` | 예: `AX_BUTTON0_OnClick_AX_` |
| 스크립트 내부 임의 서브루틴 호출 | 이름 그대로 (`Screen.Proc("이름", data)`) | 실측: `Send_f`, `OnClear`, `f_vSendUiData` 등 — 표준 이벤트명이 아닌 개발자 정의 함수명 |

### 7.2 실행 게이팅 — `IsAvailable`

```cpp
CString procs = getProcName(evClick, form->GetSymbolName());
if (screen->m_vbe->IsAvailable(procs))   // 스크립트에 이 이름의 함수가 "정의되어 있어야만"
    screen->m_vbe->DoProcedure(procs);
```
**빌더의 스크립트 편집창 드롭다운(OnStart/OnSend/OnReceive/OnAlert/OnService/OnFile/OnSelect/OnTimer/OnTimerX/OnFocus/OnClose 등)에서 실제로 코드를 채운 이벤트만 이 게이트를 통과합니다.** 빈 채로 두면 `IsAvailable`이 false라 아예 호출 자체가 안 됨(로그도 안 찍힘) — 성능 최적화 + 새 플랫폼에서도 "정의 안 된 이벤트는 스킵"해야 정확히 동일 동작.

### 7.3 실측 확인된 사실 — 서브맵도 독립적으로 이벤트를 가짐

화면 1개(메인+서브맵 3개) 열었을 때 `AX_SUB_OnStart_AX_`가 2번만 찍힘(4개 화면 중 2개만 실제로 정의) — **각 서브맵은 자기 자신의 독립적인 이벤트 핸들러 세트를 가지며, 메인화면과 무관하게 개별적으로 게이팅됨.**

---

## 8. 실시간 시세(RTM) 매칭 규칙 — 요약

상세는 `@docs/RealtimeCodeIndex_Investigation.md` 참고. 핵심만 요약:

- 화면의 "종목코드 필드"는 이름 규칙이 아니라 **`FA_FLASH` 속성 플래그**로 식별 (`FM_EDIT`일 수도 `FM_OUT`일 수도 있음 — 조회용 필드와 실시간 키 필드가 별개인 경우 있음)
- 매칭은 **캐시 없이 매 틱마다 라이브로 필드값을 읽어서(`ReadData`) 비교**하는 방식 (`CScreen::OnAlert`)
- 코드값이 바뀌는 경로가 4갈래(직접입력/도미노/TR응답/스크립트SetData)라 개별 후킹이 비현실적 — 원본 구현은 "매 틱 전체 화면 순회 후 비교"라는 O(N) 방식을 씀 (성능 이슈로 별도 개선 작업 진행 중)

**새 플랫폼 설계 시사점:** 새로 설계한다면 처음부터 "종목코드 → 관심 화면"의 역인덱스를 유지하는 구조로 만드는 게, 지금 겪고 있는 O(N) 문제를 원천적으로 피하는 길.

---

## 8.5. 데이터 처리부 상세 — `CStream` (Stream.cpp)

**호출 체인 (전체, 정정판):**
```
소켓 도착
  → CWizardCtrl::OnAxis(_axisH*, ...)   ① winK로 대상 창 탐색, 서버지시 새창생성/닫기
      → works->OnStream(...)             ② CWorks::OnStream, 조각 재조립(statCON)
          → works->OnAxis(...)           ③ 가상함수 — CClient::OnAxis는 한 줄 위임(m_stream->OutStream 호출)
              → m_stream->OutStream(...) ④ CStream::OutStream — 실제 분기/파싱 시작
```
①~③은 라우팅/재조립만 하고, **④에서 처음으로 데이터 내용에 따라 분기**한다. 반대 방향(클라이언트→서버 송신)은 `CGuard::Write`/`Login`/`Service`이 담당하고, `CClient::OnWrite`에서 보이는 `m_stream->InStream(screen)`이 그 카운터파트로 추정됨(필드 변경 시 자동 전송) — `Out`/`In` 네이밍은 서버 기준(서버에서 나가는/서버로 들어가는)으로 추정.

### `msgK` 전체 종류 (9개 + RSM, `CStream::OutStream` 실측)

| msgK | 값 | 처리 | 공통꼬리(커서복원/재조회) 통과 여부 |
|---|---|---|---|
| `msgK_AXIS` | 0x20 | 메인 데이터 — `SetDataOOP`/`SetDataNRM`/`SetDataNRM2` 분기, 끝에 `OnReceive` 발생 | O |
| `msgK_HTM` | 0x21 | HTML 뷰로 그대로 전달 | X (break 없이 처리 후 흐름 이어짐) |
| `msgK_TAB` | 0x22 | 탭구분 데이터 — `SetDataTAB`/`SetDataTAB2` | O |
| `msgK_SVC` | 0x24 | 서비스콜 — `_xscreen->OnService()` 우선, 아니면 스크립트 `OnService` | X (return) |
| `msgK_APC` | 0x25 | 승인콜 — 스크립트 `OnApprove` | X (return) |
| `msgK_CTRL` | 0x26 | `FM_CONTROL` 필드에 직접 `WriteData` | X (return) |
| `msgK_UPF`/`msgK_DNF` | 0x27/0x28 | 파일 업/다운로드 — `screen->OnFile()` | X (return) |
| `msgK_ERR` | - | 에러 메시지 → `SetGuide`로 안내 표시 | O |
| default | - | 무시 | - |

**공통꼬리(msgK_AXIS/TAB/ERR만 해당):** 커서 위치 복원(`statNOC`), 추가 안내메시지, 대기상태 해제, **`statREP` 플래그 있으면 `RepeatTR`(자동 재조회) 트리거**.

### 필드 단위 파싱 알고리즘 — `SetDataNRM` (Stream.cpp:1474, `msgK_AXIS`의 기본 포맷)

```
1. SetDataH()로 레코드 헤더 파싱 (skip=true면 생략, 재귀 호출 시 사용)
2. idx가 끝날 때까지 반복:
   a. ParseCC()로 현재 바이트가 "제어코드"(FCC/RCC/SCC)인지 확인
      - 제어코드면: 색상/서식 지정으로 추정되는 처리 후 continue (필드 데이터 아님)
      - 아니면: key(필드 순번)를 얻음
   b. key를 screen->m_ioR[key]에 대입 → 실제 CfmBase*(폼) 획득
   c. 폼 종류(kind)별로 다른 길이/파싱 규칙 적용 (아래 표)
```

**컨트롤 종류별 파싱 규칙:**

| kind | 길이 결정 방식 | 비고 |
|---|---|---|
| `FM_EDIT`/`FM_COMBO`/`FM_OUT` | **고정길이** — `.map` 정의의 `form->m_form->size` | `EIO_INPUT`(입력전용) 필드는 **건너뜀** — 서버 데이터로 안 덮어씀 |
| `FM_MEMO`/`FM_BROWSER` | **가변길이, 길이 프리픽스** — 앞에 `L_FILED`바이트짜리 ASCII 숫자로 뒤따르는 실데이터 길이 표시 | `FM_BROWSER`는 `EIO_OUTPUT`일 때만 |
| `FM_BUTTON`(라디오) | **1바이트** — `'0'`이면 미체크, 그 외 체크 | `EIO_OUTPUT`/`EIO_INOUT`만 |
| `FM_GRID` | 헤더(옵션, `GO_HEADER`) + 행수(고정 or `GO_FLEX`면 가변길이 프리픽스) + 셀데이터 → `SetTable`/`SetCells`에 위임 | 그리드 자체가 복합 포맷, 별도 상세조사 필요 |
| `FM_TABLE` | `screen->m_cellR[...]` 사전정의 배열의 행별 고정 크기만큼 순회 | `EIO_NOP`이면 스킵 |
| `FM_CONTROL` | `EIO_INOUT`/`EIO_OUTPUT`일 때 정의크기(0이면 "남은 전체") | |
| `FM_OBJECT` | **재귀 호출** — 같은 스트림 안에 서브화면(uob) 데이터가 이어붙어 있음 | `SetDataNRM(서브screen, ...)` 재귀 |

**입력전용 필드 보호 규칙이 여기도 적용됨:** RTM(실시간) 경로에서 확인했던 "`EIO_INPUT` 필드는 서버 데이터로 절대 안 덮어씀" 규칙이, TR 응답 파싱(`SetDataNRM`)에서도 동일하게 지켜짐 — 사용자가 입력 중인 필드는 어떤 경로로 와도 보호됨. **새 플랫폼에서 반드시 동일하게 지켜야 할 규칙.**

**정확한 판단 기준은 `kind`가 아니라 `iok`(입출력 속성):** `FM_EDIT`/`FM_COMBO`도 `iok`가 `EIO_OUTPUT`/`EIO_INOUT`이면 `FM_OUT`과 동일한 경로로 서버 데이터를 그대로 받는다(2026-07-25 실측 확인, Stream.cpp:1569-1587). 오직 순수 `EIO_INPUT`일 때만 스킵됨. 즉 "EDIT류는 안 받고 OUT류만 받는다"가 아니라 **"iok가 INPUT인 필드만 예외적으로 제외된다"**가 정확한 규칙.

**부수 발견 — `IsChanged()` 호출 여부도 갈림:** `FM_EDIT`/`FM_COMBO`(비입력전용)는 서버 데이터 반영 후 `form->IsChanged()`가 호출되어 "변경됨" 상태가 되지만(스크립트 `OnChange` 연계 가능), 순수 `FM_OUT`은 이 호출이 없음(`isSup` 플래그로 분기). 출력겸용 입력창과 단순 표시필드를 구분해서 처리하는 것으로 보임 — 새 플랫폼에서 "서버 데이터 반영 시 OnChange를 트리거할지"를 컨트롤 종류(EDIT/COMBO vs OUT)로 판단해야 함.

### 인밴드 제어코드 (FCC/RCC/SCC) — 2026-07-26 정정: 색상 아니라 "속성 제어"

`h/axis.h:280,295,316` — 데이터 스트림 안에 인쇄 불가능한 ASCII 제어문자를 마커로 심어서 일반 필드값과 구분:

| 마커 | 값 | 실제 용도 (`CStream::ParseFCC`/`SetCC`, Stream.cpp:2887-2928 실측 확인) |
|---|---|---|
| `FCC` | 0x1A | **필드/셀 속성을 동적으로 토글하는 제어코드.** `SetCC()`가 `FA_PROTECT`(보호/읽기전용), `FA_MAND`(필수입력), `FA_SEND`(자동전송), 보이기(`SetVisible`), 활성화 여부를 서버 응답 도중에 즉석으로 바꿀 수 있음. ~~색상 지정으로 추정~~ → **오判정, 색상 아님.** |
| `RCC` | 0x1B | **이름으로 지정된 컬럼**(`rcc->name`, `form->GetEnum()`으로 컬럼 인덱스 역조회)에 대해 행 단위 속성 제어 (`CScreen::ParseRCC`, Screen.cpp:3205) |
| `SCC` | 0x1C | `FCC`와 유사한 구조로 셀 단위 속성 제어(`ParseSCC`), 상세 미조사 |

**중요 — 새 플랫폼 설계 시사점:** 이 마커들은 그림 그리듯 색상을 입히는 게 아니라, **서버가 실시간으로 "이 필드를 지금부터 읽기전용으로 바꿔라/숨겨라/필수입력으로 만들어라" 같은 UI 상태를 원격 제어하는 메커니즘**입니다. 새 플랫폼에서 그리드/폼 컴포넌트가 이런 동적 속성 변경을 지원하지 않으면, 서버가 의도한 UI 동작(예: 조건 충족 시 필드 잠금)이 재현되지 않습니다.

값이 일반 텍스트/숫자 필드에 나올 수 없는 제어문자 범위(0x1A~0x1C)라 마커로 안전하게 구분 가능 — 새 플랫폼 파서도 이 3바이트 값을 필드 데이터와 구분해서 스킵/해석해야 함.

---

## 8.6. 그리드 셀 데이터 파싱 — `SetCells`/`SetTable` (Stream.cpp)

`FM_GRID`(`GO_TABLE` 미설정 시 `SetCells`, 설정 시 `SetTable` — 8.5절의 `SetDataNRM` FM_GRID 분기 참고)의 셀 데이터 포맷.

### `SetCells` (Stream.cpp:2444) — 일반 그리드

```
for row in nRows:
    for col in nCols:
        if cell[col].attr & FA_SKIP: continue        // 스킵 컬럼은 바이트 소비 안 함
        if cell[col].iok != EIO_INOUT/EIO_OUTPUT:
            continue                                  // ★ INPUT 전용 컬럼은 와이어에 아예 안 실림(바이트 자체가 없음)
        formL = cell[col].size                        // 고정길이(컬럼 정의값)
        text += 그값 + '\t'
    text += '\n'
form->WriteAll(text)   // 탭+개행 구분 문자열 통째로 컨트롤에 전달, 파싱은 컨트롤 몫
```

**핵심 규칙 — 일반 필드와 다른 점:** `SetDataNRM`의 EDIT/OUT 필드는 "입력전용이면 읽되 스킵"이었지만, **그리드 컬럼은 입력전용이면 애초에 서버가 그 바이트 자체를 안 보냅니다** (컬럼이 와이어 포맷에서 통째로 빠짐, `continue`가 `idx` 증가 없이 실행됨). 새 플랫폼에서 그리드 파서를 만들 때 이 차이를 놓치면 컬럼이 밀리는 버그가 남.

### `SetTable` (Stream.cpp:2647) — GO_TABLE 방식, 인밴드 제어코드 처리 위치

`SetCells`와 기본 구조는 같으나, **셀 하나하나를 읽기 전에 `FCC`/`RCC`/`SCC` 여부를 먼저 검사**한다:
```cpp
while (idx + skip < datL)
{
    switch (datB[skip])
    {
    case FCC: ParseFCC(screen, form, ..., col=kk, row=ii); skip += L_FCC; continue;
    case RCC: skip += screen->ParseRCC(form, ..., row=ii); continue;
    case SCC: ParseSCC(screen, form, ..., col=kk, row=ii); skip += L_SCC; continue;
    }
    // 제어코드가 아니면 비로소 실제 셀 값 읽기
    text += CString(&datB[skip], cell->size);
    skip += cell->size;
    break;
}
```
**이게 FCC/RCC/SCC가 (col,row) 좌표와 함께 파싱되는 근거** — 그리드/테이블 컨텍스트에서 "몇 번째 행, 몇 번째 열의 속성을 바꿔라"는 지시로 쓰인다는 게 코드로 확정됨. `SetCells`(GO_TABLE 미설정)에는 이 제어코드 검사 루프가 없음 — GO_TABLE 방식에서만 셀별 동적 속성제어가 지원되는 것으로 보임(추가확인 필요).

### 사용자 정의 컬럼 변형 — `SetCells2`/`SetTable2`

컬럼을 사용자가 재배열/숨김 설정한 경우(`screen->m_ocells`에 컬럼 매핑 정보 존재 시) 쓰이는 변형. `CtranItem::m_col < 0`이면 "서버는 이 컬럼 데이터를 여전히 보내지만 사용자 설정상 숨겨진 컬럼"이라는 뜻으로, **바이트는 소비하되(`idx += citem->m_size`) 텍스트에는 안 넣음** — `SetCells`(입력전용 컬럼은 바이트 자체가 없음)와는 다른 종류의 "스킵"이라 새 플랫폼에서 혼동 주의.

---

## 8.7. 레코드 헤더(`SetDataH`) — 원장(Ledger) 블록의 실제 위치 (2026-07-26 실측 확인)

**핵심 발견:** 화면의 맵 타입(`m_mapH->typeH`)이 `TH_LEDGER`인 경우, 페이로드 구조는 다음과 같습니다:

```
[axisH 헤더 24바이트] + [원장(Ledger) 블록, screen->m_ledgerL 바이트] + [일반 TR 필드 데이터 (8.5절 SetDataNRM)]
```

`CStream::SetDataH`(Stream.cpp:2410, `SetDataNRM`이 매 레코드 시작 시 제일 먼저 호출)가 이 분기를 담당:

```cpp
bool CStream::SetDataH(CScreen* screen, char* datB, int& datH)
{
    datH = 0;
    switch (screen->m_mapH->typeH)
    {
    case TH_LEDGER:
        datH += screen->m_ledgerL;        // 원장 블록 크기만큼 idx를 밀어냄
        screen->SetLedger(datB);           // 원장 블록을 외부 DLL(dll/login)에 그대로 전달
        skip = atoi(m_guard->GetLedger(screen->m_ledger, getOK));  // 처리결과 재확인
        if (skip == 0) return false;       // 실패 시 전체 파싱 중단
        if (skip < 0) datH += -skip;       // 추가 스킵량
        break;
    // TH_KOSCOM/TH_SCUP/TH_4702는 별도 처리 필요해보이나 현재 분기는 비어있음(추가조사 필요)
    }
    return true;
}
```

**원장(Ledger) 자체는 Wizard가 파싱하지 않음 — 별도 DLL(`dll/login/`)의 블랙박스:** `screen->SetLedger(datB)`는 원장 블록의 바이트를 그대로 외부 DLL(`axSetLedger`/`axLedger` 등 함수포인터, `dll/login/ledger.h`·`ledgerx.cpp`)에 넘길 뿐, Wizard 자체는 그 내부구조를 해석하지 않습니다. 이후 화면단 필요 시 `GetLedger(ledger, pos, length)`/`GetLedger(ledger, id)`로 위치/ID 기반 조회만 함.

### 원장 헤더 구조체 `_ledgerH` — 금융권 표준 "공통전문헤더" 포맷 (h/ledger.h)

기본은행/증권 호스트 인터페이스의 전형적인 공통 헤더 포맷(전문길이/서비스코드/사용자ID/지점번호/단말번호/기능키/연속키/오류코드/오류메시지 등). 주요 필드:

| 필드 | 오프셋 | 크기 | 의미 |
|---|---|---|---|
| `size` | 0 | 6 | 전문 전체 길이 |
| `type` | 6 | 1 | 전문구분(B:계정계, I:정보계, F:파일송수신, N:Notice) |
| `svcd` | 12 | 8 | 서비스코드 |
| `usid` | 23 | 16 | 사용자ID |
| `pbid` | 53 | 16 | 트랜잭션추적ID |
| `term` | 99 | 8 | 단말번호 |
| `ecod` | 117 | 4 | 메시지코드(오류코드, 0000~0999 정상/1000~9999 비정상) |
| `fkey` | 123 | 4 | 기능키(5:확인 7:연속 C:전송) |
| `next` | 137 | 1 | 연속여부(Y:연속있음) |
| `nkey` | 138 | 18 | 연속처리 키값 |
| `mkty`/`odrf` | 172/173 | 1/1 | 시장구분(1:현물 2:선옵 3:기타) / 주문구분(1:주문 2:조회 3:이체) |
| `emsg` | 241 | 99 | 오류메시지(코드4+메시지94) |

**⚠️ 코드베이스 내 구조체 정의 불일치 발견:** `h/ledger.h`(공용 헤더, **340바이트**, `epwd` 필드 없음)와 `Wizard/Client.cpp:4243`(함수 내부 로컬 재정의, **384바이트**, `epwd[44]`—sha256 암호화 계좌비밀번호 필드 추가로 있음)가 서로 다릅니다. `epwd`가 최근에 추가된 필드인데 공용 헤더(`h/ledger.h`)에는 아직 반영이 안 된 것으로 보임 — 실제 원장 데이터가 340/384바이트 중 어느 쪽인지는 `screen->m_ledgerL`을 로그로 실측해서 확인 필요(로그 추가함, 아래 참고).

**실측 결과 (2026-07-26 확인 완료):** `IB12010`(계좌잔고 성격 화면) 조회 시 `[SetDataH] typeH=6 ledgerL=384` 로그로 확인됨 — **실제 운영 데이터는 384바이트(`epwd` 포함) 버전이 맞다.** 즉 `h/ledger.h`(340바이트, 공용헤더)가 구버전이고, `Client.cpp` 로컬 정의(384바이트)가 현재 실제 프로토콜과 일치. **새 플랫폼에서는 384바이트 버전(epwd 포함)을 기준으로 구현해야 함.** `typeH=6`이 `TH_LEDGER` 상수값으로 확인됨(추가 매핑 정보는 axisfm.h/axisfire.h류에서 `TH_LEDGER` define 값 대조 필요).

---

## 8.8. 송신측 TCP write 프레이밍 — 한 번의 write에 여러 `_axisH` 메시지가 묶일 수 있음 (2026-07-30 실측 확인)

`CGuard::Write(char* pBytes, int nBytes, bool trace)`가 실제 소켓 송신 직전의 **유일한 합류 지점**임을 확인함(`RouteTR`(일반 TR)과 `CGuard::UploadFile`(파일 업로드) 둘 다 최종적으로 이 함수 하나를 거침). 여기에 로그를 달아보니, **한 번의 `Write()` 호출(=한 번의 TCP 송신)이 서로 다른 화면(unit) 앞으로 가는 `_axisH` 프레임을 여러 개 이어붙여서 보낼 수 있다**는 게 실측으로 확인됨.

**원인:** `CStream::MakeStream(bool byKey)`(단일 화면용 `MakeStream(CScreen*, CString)`과는 다른 오버로드)는 `m_client`에 속한 **모든 화면을 순회**하며(`skip`/`MM_MENU`/`isUob()` 제외) 화면마다 `MakeStream(screen)`을 호출해 **같은 송신버퍼(`m_sndB`)에 계속 이어붙인다.** `m_sndL`(누적 오프셋)은 이 배치 시작 시점에 딱 한 번만 0으로 리셋되고, 그 뒤 화면 개수만큼 `_axisH`+데이터 프레임이 순서대로 쌓인 뒤, **딱 한 번의 `RouteTR`/`Write()`로 전부 한꺼번에 전송**된다. 이 배치 경로는 스크립트의 `Screen.Send(targetALL)`(`CxScreen::_Send`의 `targetALL` case → 인자 없는 `InStream()` 호출)로 진입한다.

**실측 예시(2026-07-30):** 화면 안에 `IB282300`(unit=0)/`IB282310`(unit=1) 두 화면이 동시에 조회를 시도한 경우:
```
[0-MakeStream-send] ------ map=IB282300 tr=poopoop ------ winK=47 unit=0
[0-MakeStream-send] ------ map=IB282310 tr=poopoop ------ winK=47 unit=1
[0-Write-send] #0 winK=47 unit=0 msgK=32 trxC=poopoop datL=... preview=[...]
[0-Write-send] #1 winK=47 unit=1 msgK=32 trxC=poopoop datL=... preview=[...]
CGuard::Write(2) len=315 ...      <- 두 프레임 합쳐서 한 번에 315바이트 전송
```
AxisChaser는 이 315바이트짜리 TCP 페이로드를 각 프레임의 `datL`(헤더의 5자리 ASCII 길이 필드) 경계로 스스로 파싱해서 `[Send Data 87 Bytes][Unit 0]`/`[Send Data 228 Bytes][Unit 1]` 두 개의 별도 항목으로 나눠 보여준다 — **Wizard 쪽에서 보면 한 번의 소켓 write이지만, 논리적으로는 독립된 여러 메시지가 프레임 단위로 이어붙여진 것**이다.

**새 플랫폼 설계 시사점:** 소켓 수신 파서를 만들 때 "한 번의 recv/read가 정확히 하나의 논리 메시지"라고 가정하면 안 된다. TCP 스트림 특성상 여러 프레임이 한 번의 시스템 콜로 도착하거나(반대로 하나의 프레임이 여러 번에 걸쳐 도착하거나, 4절의 `statCON` 재조립 규칙 참고) 할 수 있으므로, 항상 `_axisH` 헤더의 `datL` 필드로 프레임 경계를 직접 계산하며 루프를 도는 파서가 필요하다 — 이번 확인으로 송신측도 동일한 원칙이 적용됨을 알았으니, 신규 구현체는 송수신 양쪽 모두 "고정 24바이트 헤더 + `datL`만큼의 페이로드"를 한 단위로 보고 반복 파싱하는 구조로 설계해야 한다.

---

## 8.9. 페이로드 암호화/복호화 — `CGuard::Xecure` (2026-07-31 실측 확인)

**실측 캡처 (IB140300, `SRGSQ145` 조회 — 결산분배금 내역):**
```
[0-MakeStream-send] map=IB140300 tr=piboPBxQ winK=39 unit=0
[0-GetDataNRM-field] name=zPwd kind=7 value=[***]                (평문 조립, L_axisH 이후 페이로드)
[Xecure] helper=ENC nBytesIn=434 nBytesOut=496 retv=1              <- 암호화: 434B -> 496B
[0-Write-send] ... datL=496 (암호문 그대로 전송)
CGuard::Write(2) len=520 hdr0=0x20 hdr3=0x27                       <- 520 = L_axisH(24) + 496, 정확히 일치
...
[Xecure] helper=DEC nBytesIn=912 nBytesOut=860 retv=1              <- 응답 복호화: 912B -> 860B
[1-OnAxis-raw] nBytes=860                                          <- 이후 파이프라인은 평문 기준으로 정상 진행
```

### 확정된 규칙

1. **암호화 대상은 `_axisH` 헤더(24바이트) 이후 페이로드뿐 — 헤더 자체는 항상 평문이다.** `CStream::MakeStream`/`Guard.cpp`의 모든 `Xecure(DI_ENC, ...)` 호출은 `&sendB[L_axisH]`/`&m_sndB[m_sndL]`(헤더 다음 위치)부터 시작하는 버퍼를 넘긴다. `winK`/`unit`/`trxC`/`datL` 같은 라우팅 정보를 서버가 복호화 없이도 읽을 수 있어야 하므로 당연한 설계지만, 이번에 바이트 길이 계산으로 실측 확정됨: `CGuard::Write(2) len=520` = 헤더 24B + 암호문 496B, 오차 없음.
2. **암호화 여부는 헤더의 `stat` 필드 `statENC` 비트로 표시된다.** 송신측이 `Xecure(DI_ENC, ...)` 성공 시 `axisH->stat |= statENC`를 세팅하고, 수신측(`CWizardCtrl::OnRead`, `WizardCtrl.cpp:712`)은 `[1-OnAxis-raw]`로 넘기기 **전에** `axisH->stat & statENC`를 검사해 켜져 있으면 `Xecure(DI_DEC, ...)`부터 통과시킨다. 복호화 실패 시 그 프레임은 조용히 버려진다(`continue`, `[Xecure] decrypt FAILED` 로그 참고, `docs/DebugLogGuide.md` 12절).
3. **암호화는 전체 TR이 아니라 맵(화면) 단위로 켜고 끈다.** `CStream::MakeStream`의 게이트 조건은 `!(m_guard->m_term & flagENX) && screen->m_mapH->options & OP_ENC` — 즉 (a) 이 맵의 빌드 옵션에 `OP_ENC`가 설정돼 있어야 하고, (b) 로그인 시 받은 단말 권한 플래그(`m_term`)에 암호화 예외(`flagENX`) 비트가 없어야 한다. 두 조건 다 충족해야만 암호화 경로를 탄다 — 모든 TR이 무조건 암호화되는 게 아니다.
4. **`datL`(헤더의 5자리 ASCII 길이 필드)는 암호화가 걸리면 원문이 아니라 암호문 길이를 담는다.** 위 캡처에서 원문 434B가 아니라 암호문 496B가 `datL`/`Write` 길이에 그대로 반영됨 — 새 플랫폼 파서는 "헤더 다음에 `datL`만큼 읽고, `statENC`가 켜져 있으면 그 블록 전체를 복호화한 뒤에야 실제 필드 파싱(`SetDataNRM` 등)을 시작"하는 순서를 지켜야 한다.
5. **암호화/복호화 알고리즘 자체는 이번 조사로 확인 불가 — `AxisXecure.XecureCtrl.IBK2019`라는 서드파티 ActiveX 컨트롤(COM `InvokeHelper` 호출) 뒤에 완전히 캡슐화돼 있다.** `nBytesIn`→`nBytesOut` 증가폭(이번 예: 요청 434→496, +62B / 응답 912→860, -52B)은 매번 다를 수 있어 보이며(요청과 응답은 별개의 메시지라 증감폭이 대칭일 필요는 없음), 패딩+IV+MAC 등 정확한 구성은 블랙박스 안에 있어 바이트 길이 관찰만으로는 알고리즘을 특정할 수 없다. **새 플랫폼에서 동일 프로토콜을 구현하려면 이 컨트롤을 그대로 재사용하거나(COM 상호운용), 벤더/보안팀으로부터 실제 알고리즘·키 교환 방식을 별도로 전달받아야 한다** — 리버스엔지니어링 대상이 아니라 공식 스펙을 받아야 하는 영역.
6. **서버는 클라이언트가 보낸 요청의 `statENC` 비트를 보고 응답의 암호화 여부를 그대로 맞춘다.** 같은 화면/TR(IB140300, SRGSQ145)을 암호화 켠 상태와 끈 상태로 번갈아 조회해서 확인(2026-07-31, `docs/DebugLogGuide.md` 12절의 `NOENC.TXT` 개발용 스위치로 재현) — 요청에 `statENC`를 안 실으면(암호화 생략) 응답도 `stat=0`(평문)으로 오고, 요청에 `statENC`를 실으면 응답도 `stat=2`로 암호화돼서 온다. 즉 암호화 여부는 **세션/계정 단위 서버 정책이 아니라 매 요청마다 클라이언트가 선언한 값에 서버가 그대로 반응하는 요청-단위(per-request) 협상**이다 — 새 플랫폼도 "요청 시점에 암호화 여부를 결정해 `stat` 비트로 선언하면, 그 요청에 대한 응답은 동일한 방식으로 온다"고 가정하고 구현하면 된다.

### 개발용 진단 스위치 — `NOENC.TXT`

호스트 exe(AXIS.exe) 폴더에 `NOENC.TXT` 빈 파일을 두면(`CGuard::IsNoEncMode()`, 앱 재시작 불필요, 파일 유무를 매번 재확인) 위 규칙 6에 따라 요청·응답이 전부 평문으로 오가게 되어 로그에서 실제 필드 데이터를 바로 읽을 수 있다 — 이번 절의 확인들도 이 스위치로 재현한 것. 상세는 `docs/DebugLogGuide.md` 12절.

### 로그 대응표

`docs/DebugLogGuide.md` 12절에 상세 — `[Xecure]` 키워드로 DebugView 필터링하면 이 절의 모든 호출이 잡힌다.

---

## 9. 다음 조사 대상 (미완료)

- **`ParseSCC`/`SetCC`의 `CC_*` 플래그 전체 목록** — `CC_PRO`/`CC_MAND`/`CC_SEND`/`CC_VIS`/`CC_ENB`/`CC_SET` 등 확인됨(Stream.cpp:2900-2928), 전체 목록과 각각의 정확한 UI 효과는 추가 확인 여지
- **`SetDataOOP`/`SetDataNRM2`/`SetDataTAB`/`SetDataTAB2`** — `SetDataNRM` 외 4개 포맷 변형 상세 미조사
- **`SetDataH`** — 레코드 헤더 파싱 상세 미조사
- **`SetTable`엔 있고 `SetCells`엔 없는 FCC/RCC/SCC 처리** — 왜 GO_TABLE 방식만 셀별 동적 속성제어를 지원하는지 설계의도 미확인
- `WM_USER` 커스텀 메시지의 정확한 용도
- ~~TR 요청(사용자가 조회 버튼 누르는 것) → 소켓 송신 경로~~ — **확인 완료(2026-07-30), 8.8절 참고.** `RouteTR`이 `CGuard::Write(char*, int, bool)`로 최종 소켓 전송하며, 한 번의 write에 여러 화면(unit)의 `_axisH` 프레임이 배치로 묶일 수 있음
- `CDll::OnAxis`(DLL 기반 작업영역)의 실제 파싱 로직 — CClient와 다른지 동일한지

---

## 10. 관련 문서

- `@docs/WizardArchitecture.md` — 클래스 계층, 7절에 전체 클래스 레퍼런스
- `@docs/RealtimeCodeIndex_Investigation.md` — RTM 종목코드 매칭 상세
- `@docs/AxisformArchitecture.md` — 컨트롤(`CfmBase` 24종) 렌더링 레이어 상세
- `@docs/KnowledgeBase.md` — 트러블슈팅/설계의도 누적 기록
