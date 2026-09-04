---
project: ibks
category: protocol
status: 완료
updated: 2026-08-28
---

# 로그인 시점 파일 패치(File Patch) 프로토콜


## 목차

- [문서 목적](#문서-목적)
- [1. 전체 아키텍처 — 2단계 구조](#1-전체-아키텍처-2단계-구조)
- [2. 로그인 시점 흐름](#2-로그인-시점-흐름)
- [3. AXIS 브랜치 vs RSC 브랜치](#3-axis-브랜치-vs-rsc-브랜치)
- [4. 관련 파일 전체 카탈로그](#4-관련-파일-전체-카탈로그)
  - [4.1 infoAXIS / infoAXIS.new — 실행파일(exe 폴더) 매니페스트](#41-infoaxis--infoaxisnew-실행파일exe-폴더-매니페스트)
  - [4.2 infoRSC / infoRSC.new / infoRSC.tmp — 리소스 매니페스트](#42-inforsc--inforscnew--inforsctmp-리소스-매니페스트)
  - [4.3 updateAXIS / updateRSC — 다운로드 큐 파일 (임시, 세션 중에만 존재)](#43-updateaxis--updatersc-다운로드-큐-파일-임시-세션-중에만-존재)
  - [4.4 updateX — 진행상황 추적 파일 (임시, INI, 완료 시 비워짐)](#44-updatex-진행상황-추적-파일-임시-ini-완료-시-비워짐)
  - [4.5 openapi_whitelist.txt — 패치 대상 필터](#45-openapi_whitelisttxt-패치-대상-필터)
- [5. MakeUpdateList() 판정 로직](#5-makeupdatelist-판정-로직)
  - [5.1 AXIS 브랜치 판정 흐름 (Wsh.cpp:699-808)](#51-axis-브랜치-판정-흐름-wshcpp699-808)
  - [5.2 RSC 브랜치 판정 흐름 (Wsh.cpp:810-998)](#52-rsc-브랜치-판정-흐름-wshcpp810-998)
- [6. m_runDown(런타임 재확인) 메커니즘과 defer](#6-m_rundown런타임-재확인-메커니즘과-defer)
- [7. 실측 로그 예시와 axlog 태그 카탈로그](#7-실측-로그-예시와-axlog-태그-카탈로그)
- [8. 알려진 미확인/미해결 사항](#8-알려진-미확인미해결-사항)
- [9. 관련 문서](#9-관련-문서)

---

## 문서 목적

로그인할 때(또는 세션 중) "이 파일을 패치받아야 하는지"를 어디서 어떻게 판단하고, 관련 파일들(`infoAXIS`/`infoRSC`/`updateAXIS`/`updateRSC`/`updateX` 등)이 각각 무슨 역할이며 언제 생성·소비·삭제되는지를 코드로 확인한 내용을 정리합니다. `ibks/sock/Wsh.cpp`(`CWsh::MakeUpdateList()`가 핵심)와 `ibks/sock/SockCtrl.cpp`(`CSockCtrl::StartAxis()`)를 실측 로그(axlog `LOG_FILEPATCH` 카테고리)와 대조하며 확인했습니다.

**배경:** 접속할 때 파일을 많이 패치받는 것 같다는 사용자 관찰에서 출발한 조사. `MakeUpdateList()`의 두 브랜치(AXIS/RSC)에 axlog(`[MakeUpdateList-*-axis]`/`[MakeUpdateList-*-rsc]`)를 대칭적으로 추가해 실측 로그로 검증했습니다(2026-08-27~28).

---

## 1. 전체 아키텍처 — 2단계 구조

파일 버전 체크는 서로 다른 두 레이어에서, 서로 다른 타이밍에 일어납니다.

```
[레이어 1] Wizard(axWizard.ocx) — 화면 단위, 지연(lazy) 방식
    CScreen::Parse() / CClient 화면 attach 시점마다
        CGuard::isVersion(mapN) 로 로컬 캐시(m_vers) 확인
            → 캐시에 "이 화면 구버전"이라고 표시돼 있으면(request=true)
              CGuard::RequestMAPs() 로 그 화면 하나만 동기(5초 타임아웃) 요청

[레이어 2] 소켓 OCX(AxSock.ocx, CWsh) — 로그인 시점, 일괄(batch) 방식
    로그인 시 CSockCtrl::StartAxis() 가 서버에서 전체 매니페스트(infoAXIS.new/infoRSC.new)를
    한 번에 받아 로컬 매니페스트(infoAXIS/infoRSC)와 통째로 비교(MakeUpdateList()) →
    필요한 파일들을 한 번에 큐잉해서 순서대로 다운로드(DoRequest())
```

레이어 2(`MakeUpdateList()`)가 처리를 마치면서, `.map` 화면 중 일부를 "지금 안 받고 화면 열 때 받도록" 레이어 1에 위임할 수 있습니다(`WFM_VERS` 메시지로 `CGuard::UpdateVers()`에 전달, 6절 참고) — 두 레이어가 완전히 분리된 게 아니라 이 지점에서 연결됩니다.

---

## 2. 로그인 시점 흐름

```
CSockCtrl::StartAxis(key, mode)                         [SockCtrl.cpp:268]
    mode > 0 이면 runtime=true 로 표시하고 mode = -mode 로 반전
    case modeRM:
        SetDownloadMode(false, runtime ? 1 : 0)          → m_runDown = runtime
        m_stream->RunStream(ssM_RM)
        GetUpdateList()                                  [Wsh.cpp:524] — 중단된 패치 재개 시도
            성공(재개할 게 있음) → DoRequest()
            실패(재개할 거 없음) → RunStream::RunVersion(NULL, 0)
                                        │
                                        ▼ (서버에 매니페스트 요청, RSM 파일전송으로 응답)
                              서버가 infoAXIS.new / infoRSC.new 를 통째로 내려줌
                                        │  (Wsh.cpp:191-223, resK_REQ/REQ2, MakeDir()로 .new 파일 기록)
                                        ▼ (파일 수신 완료 시점, 마지막 조각 도착 시)
                              CWsh::MakeUpdateList()      [Wsh.cpp:685] ← 실제 "패치 필요 여부" 판정
                                        │
                                        ├─ 뭔가 큐잉됨(true) → DoRequest() 로 실제 다운로드 시작
                                        └─ 큐잉 없음(false) → infoAXIS.new/infoRSC.new 를 그대로
                                                              infoAXIS/infoRSC 로 rename, 종료
```

`MakeUpdateList()`는 **AXIS 브랜치를 먼저 처리하고 나서(같은 함수 안에서 `m_state`로 분기), 그 다음 호출 때(재귀 아님 — RSC 매니페스트가 별도로 도착한 뒤) RSC 브랜치를 처리**하는 구조입니다(`m_state == stFlag::stAXIS`가 참이면 AXIS 브랜치, 아니면 RSC 브랜치 — `Wsh.cpp:699`). `CWsh::GetUpdateList()`(`Wsh.cpp:524-543`)가 `m_state`를 `stNONE`/`stDONE`이면 `stAXIS`로, 아니면 `stRSC`로 전환시켜 순서를 보장합니다.

---

## 3. AXIS 브랜치 vs RSC 브랜치

| 구분 | AXIS 브랜치 | RSC 브랜치 |
|---|---|---|
| 로컬 매니페스트 | `tab/infoAXIS` | `tab/infoRSC` |
| 서버 매니페스트 | `tab/infoAXIS.new` | `tab/infoRSC.new` |
| 큐 파일 | `tab/updateAXIS` | `tab/updateRSC` |
| 다운로드 목적지 | **항상 `exe`(RUNDIR) 폴더 고정** | **항목마다 제각각** (`image`/`icss/...`/`tab`/`map/...`/`dev/...`) |
| 매니페스트 컬럼 수 | 3개 (`name vers size`) | 5개 (`name vers key size path`) |
| 버전 비교 기준 | `vers`(정수) | `key`(12자리 timestamp 문자열, `vers`는 비교에 안 쓰임 — 8절 참고) |
| `.map` 화면 지연(defer) 개념 | 없음 | 있음 (`verMAP` + `m_runDown`, 6절) |
| 화이트리스트 필터 | `IsPatchWhitelisted(home, RUNDIR, name)` | `IsPatchWhitelisted(home, path, name)` |

---

## 4. 관련 파일 전체 카탈로그

모두 `{home}/tab/` 아래에 있습니다(`TABDIR="tab"`, `h/axisvar.h:33`).

### 4.1 `infoAXIS` / `infoAXIS.new` — 실행파일(exe 폴더) 매니페스트

**포맷 (공백 구분 3컬럼):** `name vers size`

```
AXIS.EXE                         000000000325 0004702160
```

| 컬럼 | 예시값 | 의미 |
|---|---|---|
| name | `AXIS.EXE` | `exe` 폴더에 배치될 파일명 |
| vers | `000000000325` → 325 | 버전 번호(정수, 빌드번호로 추정) — **AXIS 브랜치는 이 값으로만 최신여부 비교** |
| size | `0004702160` → 4,702,160 | 파일 크기(바이트). **로컬(`infoAXIS`) 파싱 시엔 읽기만 하고 버리며, 서버(`infoAXIS.new`) 파싱 시에만 사용** — `size==0`이면 "삭제 지시"로 해석, 그 외엔 다운로드 바이트 합산에 사용 |

- `infoAXIS` = 지금 로컬에 뭐가 깔려있다고 기록된 상태(직전 패치 완료 시점의 스냅샷)
- `infoAXIS.new` = 이번 로그인에서 서버가 내려준 "현재 서버 기준 최신 목록" — 비교 후 그대로(또는 큐잉된 파일들만 빠진 채로) `infoAXIS`를 덮어씀

### 4.2 `infoRSC` / `infoRSC.new` / `infoRSC.tmp` — 리소스 매니페스트

**포맷 (공백 구분 5컬럼):** `name vers key size path`

예시(이번 세션 실측 `queue-rsc` 로그에서 역산, `vers` 값 자체는 로그에 안 찍혀 있어 `?`로 표기 — 8절 참고):

```
opcode2.dat ? 001787870276 809212 tab
지지선근접반등.xml ? 001330671799 2112 icss/finddata/other/itemexcavation/유망종목발굴
```

| 컬럼 | 예시값 | 의미 |
|---|---|---|
| name | `opcode2.dat` | 파일명 |
| vers | (미확인, 8절) | 정수 필드. 로컬 파싱 시 `Citem::m_vers`에 저장은 되지만 **최신여부 비교에는 전혀 안 쓰임** |
| key | `001787870276` | 12자리 timestamp 문자열(`L_VERS=12`, `h/axis.h:153,156` — 필드 주석이 실제로 `// time stamp`). **최신여부 비교는 오직 이 값으로만 함**: `atoi(서버key) <= 로컬key` 이면 up-to-date |
| size | `809212` | 파일 크기(바이트). `0`이면 삭제 지시 |
| path | `tab` | 다운로드 목적지 폴더(exe 아님, 상대경로) — `map/`로 시작하면 화면파일, `dev/`로 시작하면 실행모듈로 특수 분류(6절), 그 외는 일반 리소스 |

- `infoRSC.tmp` = 이번 세션에서 "새로 확정된 infoRSC"를 조립 중인 임시 파일. `MakeUpdateList()` 루프 중 **이미 최신인 항목**의 원본 라인을 여기 즉시 적어두고(`Wsh.cpp:910`), 큐잉된 항목은 **다운로드가 실제로 완료되는 순간마다**(`Update()`, `Wsh.cpp:471-475`) 하나씩 추가로 적힘. 전부 끝나면 이 파일이 `infoRSC`로 rename됨(`Wsh.cpp:500-513, 982-995`).

### 4.3 `updateAXIS` / `updateRSC` — 다운로드 큐 파일 (임시, 세션 중에만 존재)

`Wsh.cpp:25-26`: `#define snAXIS "updateAXIS"`, `#define snRSC "updateRSC"`.

- **생성:** `MakeUpdateList()`가 시작하면서 `CFile::modeCreate|CFile::modeWrite`로 항상 새로 엶(`Wsh.cpp:727, 867`) — 즉 매 로그인마다 덮어써짐.
- **활용:** 이번에 큐잉된(다운로드할) 항목만 한 줄씩 기록됨(AXIS: `name@vers` 형태, RSC: 서버 매니페스트 원본 라인 그대로). 이 파일은 **재접속 시 중단된 패치를 이어받기 위한 용도** — `GetUpdateList()`(`Wsh.cpp:524`)가 이 파일을 다시 읽어서 `m_list`를 재구성함.
- **삭제:**
  - 이번에 큐잉된 게 **0개**면, `MakeUpdateList()` 자신이 끝나면서 바로 삭제(`Wsh.cpp:996`, AXIS는 `806`).
  - 큐잉된 게 **있으면**, 그 파일들 다운로드가 **전부 완료된 시점**(`Update()`, `Wsh.cpp:514-515`)에 삭제됨.
  - 즉 결과적으로 로그인 세션이 끝날 때는 항상 존재하지 않는 상태 — 사용자께서 짐작하신 "로그인시 만들어지고 활용되고 지워지나요?"가 정확히 맞습니다.

### 4.4 `updateX` — 진행상황 추적 파일 (임시, INI, 완료 시 비워짐)

INI 포맷, 섹션 `[updateAXIS]`/`[updateRSC]` 각각에 키 2개:

| 키 | 의미 |
|---|---|
| `count` | 이번에 큐잉된 전체 개수 (`MakeUpdateList()`가 씀) |
| `done` | 지금까지 실제로 받아 완료한 개수 (`Update()`가 파일 하나 끝날 때마다 갱신) |

**용도 — 중단 복구 판단.** `GetUpdateList()`(`Wsh.cpp:550-569`)가 재접속 시:
1. `done == count`면 "이미 다 받은 것"으로 보고 재개 안 함
2. `updateX` 파일의 마지막 수정시각이 **60초보다 오래되면** 죽은 세션으로 간주하고 재개 포기 (안전장치)
3. 둘 다 통과해야 실제로 이어받기 시도

**완료 시 비워짐 — 히스토리가 남지 않습니다.** 성공이든 실패든 마지막 파일 처리 시점(`Update()`, `Wsh.cpp:517-518`)에 `WritePrivateProfileSection(section, "", path)`로 해당 섹션의 키를 **전부 지웁니다**. 사용자께서 보신 `[updateAXIS]`/`[updateRSC]` 섹션 헤더만 있고 값이 없는 상태는, **패치가 있었든 없었든 완료 후엔 항상 이 모양**입니다 — "패치가 있었다면 히스토리가 남았을 것"이라는 추측은 틀렸고, 애초에 이 파일은 로그가 아니라 순수 실시간 진행률 트래커라 완료 즉시 리셋됩니다. 세션 도중(다운로드가 진행 중인 그 순간)에만 값이 채워진 모습을 볼 수 있습니다.

### 4.5 `openapi_whitelist.txt` — 패치 대상 필터

`{home}/exe/openapi_whitelist.txt`. 없으면 필터링 OFF(전체 허용, 기존 동작), 있으면 `FOLDER/FILENAME` 또는 `FOLDER/*` 형식의 화이트리스트만 통과(`Wsh.cpp:634-682`, `IsPatchWhitelisted()`). 프로세스당 1회만 로드해서 static 캐시(`[Whitelist-load]` 로그가 세션당 딱 한 번만 찍히는 이유). 2026-08-17 작업(`project_openapi_patch_whitelist` 메모리 참고)으로 도입됨 — OPEN API 최소배포 환경에서 불필요한 패치를 걸러내기 위함.

**⚠️ "파일은 있는데 안이 비어있음(또는 유효한 줄이 0개)"이면 전체 패치가 잠깁니다 — "파일 없음"과 결과가 정반대입니다.**

```cpp
// Wsh.cpp:670-682
if (!active)        // 파일 자체가 없음 → 필터링 OFF
    return true;

...
for (entries 순회)   // active=true인데 entries가 0개면 이 루프가 통째로 스킵됨
    if (일치) return true;
return false;        // → 결과적으로 모든 파일이 여기로 떨어짐 = 전부 거부
```

- **파일이 아예 없음** (`active=false`) → `return true` (필터링 없음, 전체 허용) — 로그: `[Whitelist-load] not found(...) - filtering OFF, full behavior`
- **파일은 있는데 유효한 줄이 0개** (전부 빈 줄이거나 `#` 주석, `active=true`이지만 `entries.GetSize()==0`) → 모든 파일에 대해 `entries` 순회 루프가 한 번도 못 돌고 바로 `return false` → **전체 패치 완전 잠금**. 로그: `[Whitelist-load] path=... entries=0`(entries가 0으로 찍힘 — 이걸로 "파일 없음"과 구분 가능)
- 이 상태에서는 큐잉될 뻔한 파일마다 `[MakeUpdateList-skip-whitelist-axis]`/`[MakeUpdateList-skip-whitelist-rsc]`가 전부 찍힙니다(원래 `queue-*`로 갔어야 할 게 전부 이쪽으로 빠짐).
- **단, 삭제(`delete-axis`/`delete-rsc`, 서버 size=0 지시)는 화이트리스트 체크보다 코드상 먼저 일어나므로 이 잠금과 무관하게 그대로 실행됩니다** — 화이트리스트가 막는 건 "새로 받는 것"뿐이고, "서버가 지우라고 한 로컬 파일 삭제"는 막지 못합니다.

즉 화이트리스트 파일을 만들어두고 항목을 아직 안 채웠거나, 오타/형식 오류로 모든 줄이 파싱에 실패하면(`#`으로 시작하거나 빈 줄로 처리되는 경우 포함) — 의도치 않게 "이 배포본은 어떤 파일도 패치 안 받는다"는 상태가 조용히 만들어집니다. 운영 시 화이트리스트를 도입/수정할 때는 반드시 `[Whitelist-load] ... entries=N`의 `N`이 기대한 개수인지 확인해야 합니다.

---

## 5. MakeUpdateList() 판정 로직

### 5.1 AXIS 브랜치 판정 흐름 (`Wsh.cpp:699-808`)

```
for 서버 매니페스트(infoAXIS.new)의 각 줄:
    로컬(infoAXIS)에 같은 name이 있고 서버vers <= 로컬vers  → skip-axis (최신, 넘어감)
    else if 서버size == 0                                  → delete-axis (로컬 파일 삭제)
    else if 화이트리스트 통과 못함                          → skip-whitelist-axis
    else                                                    → queue-axis (다운로드 큐에 추가)
```

### 5.2 RSC 브랜치 판정 흐름 (`Wsh.cpp:810-998`)

```
for 서버 매니페스트(infoRSC.new)의 각 줄:
    서버size == 0                                          → delete-rsc (즉시 로컬 삭제, continue)

    path가 "map/" 또는 "dev/"로 시작하면 verM[count]에 이름/키/경로 기록해두고
    version = verMAP 또는 verDEV로 표시 (그 외 경로는 version=verNONE, verM 대상 아님)

    로컬(infoRSC)에 같은 name+path가 있고 서버key <= 로컬key  → skip-rsc (최신)
                                                                (version!=verNONE이면 verM 카운트만 증가)
    else:
        switch(version):
            verMAP && m_runDown==true  → defer-rsc  (지금 안 받음, verM[].request=true 로 표시만)
            verMAP && m_runDown==false → (fall-through) 아래 verDEV와 동일하게 즉시 큐잉 대상
            verDEV                     → verM 카운트 증가, 즉시 큐잉 대상
            verNONE                    → 바로 아래 화이트리스트/큐잉 체크로

        화이트리스트 통과 못하면 → skip-whitelist-rsc
        통과하면                → queue-rsc (다운로드 큐에 추가)
```

**핵심:** `map/`·`dev/`가 아닌 경로(예: `image`, `icss/...`, `tab`)는 애초에 `verM[]`/`WFM_VERS` 캐시 갱신 대상이 아닙니다 — 그냥 "필요하면 지금 바로 받는" 일반 리소스입니다. 오직 `map/`(화면)과 `dev/`(실행모듈)만 Wizard의 버전 캐시 시스템과 연결됩니다.

---

## 6. m_runDown(런타임 재확인) 메커니즘과 defer

`m_runDown`은 `CWsh::SetDownloadMode(axisOnly, runtime)`가 그대로 저장하는 플래그(`Wsh.h:66`, 주석 `// runtime download`)이고, 호출부는 `CSockCtrl::StartAxis()`입니다:

```cpp
// SockCtrl.cpp:268-284
bool runtime = false;
if (mode > 0) { mode = -mode; runtime = true; }   // 양수 mode로 호출 = "세션 중 재확인"
...
case modeRM:
    m_rsm->SetDownloadMode(false, runtime ? 1 : 0);   // → m_runDown = runtime
```

- **최초 로그인(음수/기본 mode) → `m_runDown=false`**: `.map` 화면도 다른 리소스와 똑같이 즉시 큐잉되어 로그인 시점에 다 받습니다.
- **세션 중 재확인(양수 mode) → `m_runDown=true`**: out-of-date `.map` 화면은 지금 받지 않고(`defer-rsc`), `verM[].request=true`로 표시만 해서 `WFM_VERS` 메시지로 `CGuard::UpdateVers()`(Wizard 쪽, `Guard.cpp:1266`)에 전달합니다. 이후 사용자가 그 화면을 실제로 열 때 `CScreen::Parse()`→`CGuard::isVersion(mapN)`이 이 캐시를 보고 `RequestMAPs()`로 그 화면 하나만 개별 요청합니다(`Guard.cpp:1402`).

**미확인 사항:** `StartAxis()`를 정확히 언제/어디서 양수 `mode`로 호출하는지(즉 `m_runDown=true`가 실제로 트리거되는 시나리오)는 이번 조사에서 추적하지 못했습니다 — 8절 참고.

---

## 7. 실측 로그 예시와 axlog 태그 카탈로그

카테고리: `LOG_FILEPATCH` (`h/axlog.h`). 전부 `CWsh::MakeUpdateList()` 내부(`Wsh.cpp`).

| 태그 | 브랜치 | 의미 |
|---|---|---|
| `[Whitelist-load]` | 공통 | 화이트리스트 파일 로드 결과(세션당 1회) |
| `[MakeUpdateList-local-axis]` | AXIS | 로컬 `infoAXIS` 항목 수 |
| `[MakeUpdateList-server-axis]` | AXIS | 서버 `infoAXIS.new` 항목 수 |
| `[MakeUpdateList-skip-axis]` | AXIS | 이미 최신이라 건너뜀 |
| `[MakeUpdateList-delete-axis]` | AXIS | 서버 size=0 → 로컬 삭제 |
| `[MakeUpdateList-skip-whitelist-axis]` | AXIS | 화이트리스트에 없어서 제외 |
| `[MakeUpdateList-queue-axis]` | AXIS | 다운로드 큐에 추가 |
| `[MakeUpdateList-summary-axis]` | AXIS | `queuedCount`/`totalBytes` |
| `[MakeUpdateList-local-rsc]` | RSC | 로컬 `infoRSC` 항목 수 |
| `[MakeUpdateList-server-rsc]` | RSC | 서버 `infoRSC.new` 항목 수 |
| `[MakeUpdateList-delete-rsc]` | RSC | 서버 size=0 → 로컬 삭제 |
| `[MakeUpdateList-skip-rsc]` | RSC | 이미 최신 (localKey/serverKey 비교값 포함) |
| `[MakeUpdateList-defer-rsc]` | RSC | `.map`이면서 `m_runDown=true` → 지금 안 받고 화면 열 때로 미룸 |
| `[MakeUpdateList-skip-whitelist-rsc]` | RSC | 화이트리스트에 없어서 제외 |
| `[MakeUpdateList-queue-rsc]` | RSC | 다운로드 큐에 추가 |
| `[MakeUpdateList-summary-rsc]` | RSC | 아래 참고 |

**`summary-rsc` 6개 인자 (2026-08-27 실측 예시, 로그인 직후 전체 캡처):**

```
[MakeUpdateList-summary-rsc] queuedCount=43 totalBytes=5322881 verMTotal=5875 upToDateCount=5875 deferredCount=0 devPendingCount=0
```

| 인자 | 값 | 의미 |
|---|---|---|
| `queuedCount` | 43 | 지금 바로 다운로드될 파일 수 (`m_list.GetSize()`) |
| `totalBytes` | 5,322,881 | 그 43개 파일의 총 바이트 |
| `verMTotal` | 5875 | `map/`+`dev/` 경로 항목 중 `verM[]`(버전캐시)에 등록된 총 개수 = 아래 3개 합 |
| `upToDateCount` | 5875 | 그중 이미 최신 (`skip-rsc`) |
| `deferredCount` | 0 | 그중 화면 열 때로 미뤄진 것 (`defer-rsc`) |
| `devPendingCount` | 0 | 그중 즉시 큐잉된 `dev/` 항목 (또는 `m_runDown=false`인 `map/`) |

**이 예시가 보여주는 것:** `verMTotal == upToDateCount`(5875=5875), `deferredCount`/`devPendingCount` 둘 다 0 — 이 세션은 화면(.map)·실행모듈(dev) 전부 100% 최신이었고 패치가 필요 없었습니다. 실제로 받은 43개(총 5.08MB)는 전부 `map/`·`dev/`와 무관한 일반 리소스(`tab/` 아래 종목코드 마스터 테이블 `opcode.dat`/`mocode.dat`/`wocode.dat` 등, `icss/...` 아래 전략검색 xml·캔들패턴 bmp)였습니다.

---

## 8. 알려진 미확인/미해결 사항

- **RSC `vers` 컬럼(두 번째 필드)의 정확한 용도** — 로컬 파싱 시 `Citem::m_vers`에 저장은 되지만, 최신여부 비교(`skip-rsc`)는 오직 `key`(세 번째 필드)로만 이뤄져서 실제로 어디 쓰이는지 코드상 확인 안 됨. 현재 axlog에도 이 값을 안 찍고 있어 실측값 자체도 없음 — 필요하면 `-rsc` 로그들에 `vers=` 필드를 추가 가능.
- **`GetUpdateList()`가 재개 시 파싱하는 `key`(정수) 필드**(`Wsh.cpp` 재개 파싱부, `"%s %d %d %d %s"`)도 파싱만 되고 이후 코드에서 실제 사용되는지 미확인.
- **`m_runDown=true`가 실제로 언제 트리거되는지** — `StartAxis()`가 양수 `mode`로 호출되는 실제 시나리오(어떤 이벤트가 "세션 중 재확인"을 유발하는지)를 아직 추적하지 못함. 이걸 알아야 `defer-rsc`가 실사용 중 실제로 얼마나 자주 발생하는지 가늠 가능.
- **AXIS 브랜치의 `RUNDIR/name@vers` 목적지 포맷** — 다운로드 대상 경로에 `@버전번호`가 붙는 이유(파일명에 버전을 실어 스테이징 후 나중에 rename하는 방식으로 추정)는 `MakeDir()`/파일쓰기 쪽 코드까지 더 봐야 확정 가능, 이번엔 미조사.
- **`infoAXIS.new`/`infoRSC.new` 자체가 서버에서 내려오는 시점의 상세 RSM 프로토콜**(`MakeDir()`, `resK_REQ`/`resK_REQ2` 분기)은 이번 문서에서 개요만 다뤘고 세부 조각전송 규칙은 `@docs/MigrationSpec_SocketToDrawing.md` 2절(패킷 조각 재조립)과 사실상 동일한 매커니즘으로 보이나 1:1 대조 검증은 안 함.

---

## 9. 관련 문서

- `@docs/DebugLogGuide.md` — axlog 전체 태그 카탈로그(`LOG_FILEPATCH` 추가 예정)
- `@docs/MigrationSpec_SocketToDrawing.md` 2절 — 패킷 조각 재조립 규칙(RSM 파일전송과 유사한 메커니즘)
- `@docs/KnowledgeBase.md` — 관련 설계의도/트러블슈팅 누적 기록
- `project_openapi_patch_whitelist`(사용자 메모리) — `openapi_whitelist.txt` 도입 배경(2026-08-17)

---

**최종 수정:** 2026-08-28
**작성 방식:** `sock/Wsh.cpp`/`SockCtrl.cpp` 코드 직접 확인 + axlog 실측 로그 대조
**상태:** 1차 작성 — 8절 미확인 사항은 추가 조사 필요
