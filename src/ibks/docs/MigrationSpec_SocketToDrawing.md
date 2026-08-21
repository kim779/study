# 마이그레이션 명세서 — 소켓 데이터 → 화면 드로잉 파이프라인










## 목차

- [문서 목적](#문서-목적)
- [0. 전체 파이프라인 개요](#0-전체-파이프라인-개요)
  - [0.1. 단순 TR 송수신 — 함수 호출 스택 정리 (2026-08-18)](#01-단순-tr-송수신-함수-호출-스택-정리-2026-08-18)
- [1. 와이어 프로토콜 — 패킷 헤더 `_axisH`](#1-와이어-프로토콜-패킷-헤더-_axish)
  - [`msgK` (메시지 종류) — `h/axis.h:44~70` (전수 확인, 2026-07-31 갱신)](#msgk-메시지-종류-haxish4470-전수-확인-2026-07-31-갱신)
  - [`auxs` 부가상태 (확인된 것)](#auxs-부가상태-확인된-것)
- [2. 패킷 조각 재조립 규칙 — `CWorks::OnStream`](#2-패킷-조각-재조립-규칙-cworksonstream)
- [3. 창(작업영역) 식별 규칙 — `key`/`type`](#3-창작업영역-식별-규칙-keytype)
  - [`key` — 작업영역 슬롯 식별자 (한 바이트 범위)](#key-작업영역-슬롯-식별자-한-바이트-범위)
  - [`type` — 화면 종류 + 표시방식 비트마스크 (한 바이트, 상하위 니블 분리)](#type-화면-종류-표시방식-비트마스크-한-바이트-상하위-니블-분리)
  - [`unit` — 작업영역 안의 화면(스크린) 식별자 (2026-07-30 확인)](#unit-작업영역-안의-화면스크린-식별자-2026-07-30-확인)
- [4. 화면(맵) 로드/조립 규칙 — `CScreen::Parse()`](#4-화면맵-로드조립-규칙-cscreenparse)
  - [4.1 서브맵 임베딩 규칙 (FM_OBJECT)](#41-서브맵-임베딩-규칙-fm_object)
  - [4.2 그리드 특수역할 마커 — `vals[2]`의 `$$`/`$?`/`$*` (Screen.cpp:372-385, 2026-08-14 확인)](#42-그리드-특수역할-마커-vals2의-screencpp372-385-2026-08-14-확인)
- [5. 컨트롤은 실제 윈도우가 아님 — 가상 컨트롤 구조](#5-컨트롤은-실제-윈도우가-아님-가상-컨트롤-구조)
- [6. 입력 이벤트 → 원시 메시지 후킹 규칙 — 윈도우 서브클래싱](#6-입력-이벤트-원시-메시지-후킹-규칙-윈도우-서브클래싱)
  - [`CallProc`이 처리하는 메시지 전체 목록 (27종, `Event.cpp` 실측)](#callproc이-처리하는-메시지-전체-목록-27종-eventcpp-실측)
- [7. 이벤트 → 스크립트 실행 규칙](#7-이벤트-스크립트-실행-규칙)
  - [7.1 프로시저 명명 규칙](#71-프로시저-명명-규칙)
  - [7.2 실행 게이팅 — `IsAvailable`](#72-실행-게이팅-isavailable)
  - [7.3 실측 확인된 사실 — 서브맵도 독립적으로 이벤트를 가짐](#73-실측-확인된-사실-서브맵도-독립적으로-이벤트를-가짐)
- [8. 실시간 시세(RTM) 매칭 규칙 — 요약](#8-실시간-시세rtm-매칭-규칙-요약)
- [8.5. 데이터 처리부 상세 — `CStream` (Stream.cpp)](#85-데이터-처리부-상세-cstream-streamcpp)
  - [`msgK` 종류 — `OutStream`까지 도달하는 것만 (TR 요청-응답 계열 하위 집합, `CStream::OutStream` 실측)](#msgk-종류-outstream까지-도달하는-것만-tr-요청-응답-계열-하위-집합-cstreamoutstream-실측)
  - [필드 단위 파싱 알고리즘 — `SetDataNRM` (Stream.cpp:1474, `msgK_AXIS`의 기본 포맷)](#필드-단위-파싱-알고리즘-setdatanrm-streamcpp1474-msgk_axis의-기본-포맷)
  - [인밴드 제어코드 (FCC/RCC/SCC) — 2026-08-18 재정정: 셋이 서로 다른 용도(속성/드롭다운데이터/색상)](#인밴드-제어코드-fccrccscc-2026-08-18-재정정-셋이-서로-다른-용도속성드롭다운데이터색상)
    - [`FCC` — `CC_*` 플래그 전체 목록 (2026-08-18 확인, `h/axis.h:288-293`)](#fcc-cc_-플래그-전체-목록-2026-08-18-확인-haxish288-293)
    - [`RCC` — 드롭다운/트리 데이터 채우기, 그리고 이름이 같은 두 함수](#rcc-드롭다운트리-데이터-채우기-그리고-이름이-같은-두-함수)
- [8.6. 그리드 셀 데이터 파싱 — `SetCells`/`SetTable` (Stream.cpp)](#86-그리드-셀-데이터-파싱-setcellssettable-streamcpp)
  - [`SetCells` (Stream.cpp:2444) — 일반 그리드](#setcells-streamcpp2444-일반-그리드)
  - [`SetTable` (Stream.cpp:2647) — GO_TABLE 방식, 인밴드 제어코드 처리 위치](#settable-streamcpp2647-go_table-방식-인밴드-제어코드-처리-위치)
  - [사용자 정의 컬럼 변형 — `SetCells2`/`SetTable2`](#사용자-정의-컬럼-변형-setcells2settable2)
- [8.7. 레코드 헤더(`SetDataH`) — 원장(Ledger) 블록의 실제 위치 (2026-07-26 실측 확인)](#87-레코드-헤더setdatah-원장ledger-블록의-실제-위치-2026-07-26-실측-확인)
  - [원장 헤더 구조체 `_ledgerH` — 금융권 표준 "공통전문헤더" 포맷 (h/ledger.h)](#원장-헤더-구조체-_ledgerh-금융권-표준-공통전문헤더-포맷-hledgerh)
- [8.8. 송신측 TCP write 프레이밍 — 한 번의 write에 여러 `_axisH` 메시지가 묶일 수 있음 (2026-07-30 실측 확인)](#88-송신측-tcp-write-프레이밍-한-번의-write에-여러-_axish-메시지가-묶일-수-있음-2026-07-30-실측-확인)
- [8.9. 페이로드 암호화/복호화 — `CGuard::Xecure` (2026-07-31 실측 확인)](#89-페이로드-암호화복호화-cguardxecure-2026-07-31-실측-확인)
  - [확정된 규칙](#확정된-규칙)
  - [개발용 진단 스위치 — `NOENC.TXT`](#개발용-진단-스위치-noenctxt)
  - [로그 대응표](#로그-대응표)
- [8.10. IBKSConnector OOP 요청 페이로드 문법 — `pooppoop`/`GOOPHOOP` 계열 (2026-08-14 확인)](#810-ibksconnector-oop-요청-페이로드-문법-pooppoopgoophoop-계열-2026-08-14-확인)
  - [배경 — 이 절이 다루는 범위](#배경-이-절이-다루는-범위)
  - [1) 단일값(비그리드) 조회 — 특수문자 없음](#1-단일값비그리드-조회-특수문자-없음)
  - [2) 그리드(다중 레코드) 조회 — `$` 마커](#2-그리드다중-레코드-조회-마커)
  - [3) 캔들/시계열 조회 — `?` 마커](#3-캔들시계열-조회-마커)
  - [4) 공통 요약 — 세 문법을 관통하는 규칙](#4-공통-요약-세-문법을-관통하는-규칙)
  - [5) 확인 범위와 다음 단계](#5-확인-범위와-다음-단계)
  - [6) TR1007(`GOOPHOOP`) 실제 와이어 바이트 최초 확인 + `snapFields[]` 확장 사례 (2026-08-19)](#6-tr1007goophoop-실제-와이어-바이트-최초-확인--snapfields-확장-사례-2026-08-19)
- [8.11. 그리드 실시간 갱신 3종 마커 — `$?`/`$$`/`$*` (2026-08-17 확인)](#811-그리드-실시간-갱신-3종-마커-2026-08-17-확인)
  - [8.11.1. `$?` — `CScreen::ScrollRTM` (순수 삽입, 매칭 없음)](#8111-cscreenscrollrtm-순수-삽입-매칭-없음)
  - [8.11.2. `$$` — `CScreen::OnNotice` 후반부 (키매칭 upsert/삭제)](#8112-cscreenonnotice-후반부-키매칭-upsert삭제)
  - [8.11.3. `$*` — `_anmH`라는 완전히 별도의 소켓 채널 (신규 발견)](#8113-_anmh라는-완전히-별도의-소켓-채널-신규-발견)
  - [8.11.4. `FlashGrid`/`FlashSemi` — `vals[2]` 마커와 무관한, 훨씬 흔한 4번째 그리드 실시간 경로 (2026-08-19 신규 발견)](#8114-flashgridflashsemi-vals2-마커와-무관한-훨씬-흔한-4번째-그리드-실시간-경로-2026-08-19-신규-발견)
- [8.12. Wizard 자체 OOP 왕복 — `GetDataOOP`/`GetDataOOP2`(요청) + `SetDataOOP`(응답) (2026-08-18 확인)](#812-wizard-자체-oop-왕복-getdataoopgetdataoop2요청-setdataoop응답-2026-08-18-확인)
  - [배경 — 8.10절(IBKSConnector)과는 별개인, 두 번째 OOP 구현](#배경-810절ibksconnector과는-별개인-두-번째-oop-구현)
  - [핵심 발견 — 요청과 응답이 같은 순서표(`m_ioR`)를 공유해서 정렬을 보장한다 (사용자 가설 확인+정정)](#핵심-발견-요청과-응답이-같은-순서표m_ior를-공유해서-정렬을-보장한다-사용자-가설-확인정정)
  - [요청 조립 — `GetDataOOP`(OOP1)/`GetDataOOP2`(OOP2)](#요청-조립-getdataoopoop1getdataoop2oop2)
  - [응답 파싱 — `SetDataOOP` (Stream.cpp:1900)](#응답-파싱-setdataoop-streamcpp1900)
  - [실측 검증 — 단일값(비그리드) OOP 요청/응답 왕복 1건 전체 (2026-08-19, `IB999986`/`pooppoop`)](#실측-검증-단일값비그리드-oop-요청응답-왕복-1건-전체-2026-08-19-ib999986pooppoop)
- [8.13. `SetDataNRM2`/`SetDataTAB2` — 사용자 컬럼 커스터마이즈 변형 (2026-08-18 확인)](#813-setdatanrm2setdatatab2-사용자-컬럼-커스터마이즈-변형-2026-08-18-확인)
  - [왜 "2"가 붙은 버전이 따로 있는가](#왜-2가-붙은-버전이-따로-있는가)
  - [`m_ioR[]` 대신 `m_outs[]`를 순회 — 그리고 `NULL` 항목이라는 새로운 케이스](#m_ior-대신-m_outs를-순회-그리고-null-항목이라는-새로운-케이스)
  - [`SetDataNRM2` vs `SetDataNRM` — 필드별 처리 차이는 "크기 출처"뿐](#setdatanrm2-vs-setdatanrm-필드별-처리-차이는-크기-출처뿐)
  - [`SetDataTAB2` vs `SetDataTAB` — 그리드 처리가 다르다 (신규 발견)](#setdatatab2-vs-setdatatab-그리드-처리가-다르다-신규-발견)
- [8.14. 그리드 필드 헤더 — `_gridHi`/`_gridHo` (`h/Grid.h`, 2026-08-19 확인)](#814-그리드-필드-헤더-_gridhi_gridho-hgridh-2026-08-19-확인)
  - [배경 — 9절 "GetEnum/SetEnum 슬롯 98/99" 미해결 항목 해소](#배경-9절-getenumsetenum-슬롯-9899-미해결-항목-해소)
  - [두 구조체 — 요청은 `_gridHi`(110바이트), 응답은 `_gridHo`(103바이트)](#두-구조체-요청은-_gridhi110바이트-응답은-_gridho103바이트)
  - [어디서 만들고 어디서 소비하는가](#어디서-만들고-어디서-소비하는가)
  - [실측 검증 — `IB999988`(체결그리드) 요청 헤더 바이트 1:1 대조 (2026-08-19)](#실측-검증-ib999988체결그리드-요청-헤더-바이트-11-대조-2026-08-19)
  - [`GetDataOOP`(OOP1)의 FM_GRID 전체 문법 — 요청측 (코드 인용)](#getdataoopoop1의-fm_grid-전체-문법-요청측-코드-인용)
  - [이 헤더 메커니즘은 OOP 전용이 아니다 — `GO_HEADER`로 게이팅되는 범용 그리드 프로토콜](#이-헤더-메커니즘은-oop-전용이-아니다-go_header로-게이팅되는-범용-그리드-프로토콜)
  - [RTM 실시간 삽입 방향(`GO_TOP`)과의 관계 — 별개 메커니즘임을 주의](#rtm-실시간-삽입-방향go_top과의-관계-별개-메커니즘임을-주의)
- [8.15. RCC 콤보 항목수(`ccl`) 필드값이 실제 전송 데이터보다 작음 — 서버측 데이터 버그로 확인 (2026-08-21)](#815-rcc-콤보-항목수ccl-필드값이-실제-전송-데이터보다-작음-서버측-데이터-버그로-확인-2026-08-21)
  - [배경](#배경-3)
  - [확인된 사실](#확인된-사실-1)
  - [결론 — 클라이언트는 정상, 서버가 내려주는 `ccl` 값이 실제 데이터 개수와 불일치](#결론-클라이언트는-정상-서버가-내려주는-ccl-값이-실제-데이터-개수와-불일치)
  - [관련 파일](#관련-파일-4)
- [9. 다음 조사 대상 (미완료)](#9-다음-조사-대상-미완료)
- [10. 관련 문서](#10-관련-문서)

---

## 문서 목적

**배경:** 연말 HTS 플랫폼 전면 교체 예정 (axis/axwizard 폐기, 새 프로세스/플랫폼으로 이전). 이 문서는 "코드베이스 이해"가 아니라 **새 플랫폼 개발팀이 참고할 수 있는 정확한 사양서**를 목표로 함. 애매한 추정은 배제하고, 실측(로그) 또는 소스 직접 확인으로 검증된 내용만 기록.

**범위:** 소켓으로 원시 바이트가 들어와서, 화면에 값이 그려지기까지의 전체 파이프라인과 그 안의 모든 규칙. `@docs/WizardArchitecture.md`(클래스 계층), `@docs/RealtimeCodeIndex_Investigation.md`(RTM 매칭 상세)와 상호보완 관계 — 이 문서는 "파이프라인 전체 흐름과 프로토콜"에 집중.

**진행상태:** 🔍 작성 중 — **①단순 TR 송수신 카테고리는 완결(2026-08-18~19)**: 소켓 수신→재조립→파싱→필드쓰기(`SetDataNRM`/`NRM2`/`SetDataOOP`/`TAB`/`TAB2`, 그리드 `SetCells`/`SetTable`, 원장 `SetDataH`), 송신 조립(`GetDataNRM`/`NRM2`/`OOP`/`OOP2`), 인밴드 제어코드(FCC/RCC/SCC, `CC_*` 전체), 송수신 암호화(`CGuard::Xecure`, 8.9절)까지 전부 소스+실측 로그로 검증 완료(§0.1에 호출 스택 정리). 나머지 카테고리(②실시간/③체결통보/④로그인/⑤파일패치)와 `MAPX`/`DIAL` msgK, `_anmH` 채널 상세는 다음 조사 대상 (9절 참고).

**2026-07-25 실측 검증:** AxisChaser(패킷캡처 도구)로 원시 바이트를 직접 대조해서 `_axisH` 구조체 필드 순서(msgK/stat/auxs/winK/unit)가 실제 와이어 바이트와 1:1로 정확히 일치함을 확인함. 코드분석+로그+원시패킷캡처 3중 검증됨.

**2026-07-31 실측 검증:** 암호화/복호화(`CGuard::Xecure`, 8.9절) 전체 흐름과 `msgK` 전체 값 목록(1절)을 axlog+실제 통신 캡처로 확정. 개발용 `NOENC.TXT` 스위치로 "서버가 클라이언트의 암호화 선언을 그대로 따라간다"는 사실도 확인.

**2026-08-14 확인:** `IBKSConnector`(OCX) 소스에서 OOP 포맷(`trxC=pooppoop/GOOPHOOP` 계열) TR의 **요청 페이로드 조립 문법**을 확정(8.10절 신설) — 단일값/그리드/캔들 세 가지 문법과 `$`/`?` 마커, `grid_i`/`grid_o` 페이징 구조체까지 소스 코드로 직접 확인. 단, 이건 클라이언트→서버 요청 쪽만 확인된 것이고, Wizard(서버 응답 수신) 쪽의 `SetDataOOP` 파싱은 여전히 미조사(9절 참고) — 서로 다른 절반.

**2026-08-17 확인:** 그리드 실시간 갱신 마커 `$?`/`$$`/`$*`(8.11절 신설) 전체 추적 완료. 그중 `$*`가 RTM(`_rtmH`)도 TR(`_axisH`)도 아닌 **완전히 별도의 소켓 이벤트/헤더(`FEV_PUSH`, `_anmH`)를 쓰는 제3의 채널**이라는 걸 신규 발견 — `msgK_ARM`/`msgK_AUX`(1절, 미조사 상태였던 것)와의 관계는 아직 미확인.

**2026-08-21 확인:** RCC 콤보 항목수 필드(`ccl`)와 실제 전송 데이터 개수 불일치를 실사용 화면(`bo425401`/`JNAM`)에서 발견, axlog+원시 바이트 덤프 이중 검증으로 **서버측 데이터 버그**로 확정(8.15절 신설) — `unsigned char` 1바이트로 대규모 목록을 표현하는 RCC 프로토콜 자체의 구조적 한계도 함께 드러남.

---

## 0. 전체 파이프라인 개요

**정정(2026-08-18):** 아래 다이어그램에서 이름이 같은 `CWizardCtrl::OnAxis`가 두 번 나오는데, 예전 버전은 이 둘을 세로로 나란히 그려서 마치 하나가 다른 하나를 순서대로 호출하는 것처럼 보였다 — **사실이 아니다.** 둘은 시그니처(반환형·인자)가 다른 완전히 별개의 오버로드이고, 서로를 호출하지도 않으며, `OnFireEvent`(소켓 OCX 이벤트의 최상위 분배점, WizardCtrl.cpp:203)의 **서로 다른 분기**에서 각자 도달한다:

```cpp
void CWizardCtrl::OnFireEvent(int type, char* pBytes, int nBytes)
{
    switch (type)
    {
    case FEV_ANM:   OnAlert(pBytes, nBytes);   break;   // RTM
    case FEV_PUSH:  OnPush(pBytes, nBytes);    break;   // $* 채널(8.11.3절)
    case FEV_AXIS:  OnRead(pBytes, nBytes);    break;   // ★ 여기를 거쳐야 두 번째 OnAxis에 도달
    case FEV_ERROR: ...
    case FEV_CLOSE: ...
    case FEV_STAT:
    case FEV_SIZE:  ...
    case FEV_VERS:  ...
    case FEV_CA:    ...
    default:        OnAxis(type, pBytes, nBytes);   break;   // ★ 첫 번째 OnAxis — "그 외 나머지 이벤트"
    }
}
```

**① `BOOL OnAxis(int type, char* pBytes, int variant)`(WizardCtrl.cpp:783)** — `OnFireEvent`의 **`default:` 분기**, 즉 위 목록에 명시적으로 안 잡힌 나머지 이벤트 타입 전부가 여기로 온다. 실제로 의미 있는 처리를 하는 건 그중 `FEV_OPEN`(=0, "axis connected")과 `FEV_RUN`(=2, "axis/workstation start")뿐 — 나머지 `type`은 그냥 `return TRUE`. **사용자가 짐작하신 그대로, 이건 TR 데이터가 아니라 소켓 OCX 자신의 연결 생명주기 이벤트다.** `FEV_OPEN`/`FEV_RUN` 처리 안에서 `m_guard->m_sock->InvokeHelper(DI_AXIS, ...)`를 호출하는데, 이게 소켓 컨트롤에게 "로그인/초기 핸드셰이크용 바이트 수를 읽어달라"고 요청하는 것으로 보인다(`nBytes`에 `modeWS`/`modeRM`/`modeRMx` 같은 고정 상수를 실어보냄) — 즉 **TR 프로토콜(`_axisH`)이 시작되기 전, 소켓이 막 연결됐을 때 최초 핸드셰이크 단계를 여는 함수**다.

**② `void OnAxis(struct _axisH* axisH, char* pBytes, int nBytes)`(WizardCtrl.cpp:1064)** — **사용자가 짐작하신 대로 일반 TR 조회 응답의 최초 통로가 맞다.** 다만 도달 경로가 `OnFireEvent`가 아니라 `case FEV_AXIS: OnRead(pBytes, nBytes)`를 거친다: `OnRead`(WizardCtrl.cpp:686)가 `_axisH` 헤더를 직접 파싱해서(`while (nBytes >= L_axisH) { axisH = (struct _axisH*)pBytes; ... }`) **`msgK`값으로 1차 분기**하고, `msgK_AXIS`/`msgK_TAB`/`msgK_HTM`/`msgK_SVC`/`msgK_APC`/`msgK_CTRL`/`msgK_UPF`/`msgK_DNF`(1절 표의 "OutStream 계열" 부분집합)만 이 두 번째 `OnAxis`로 넘어온다. `msgK_SIGN`류는 `OnStream`으로, `msgK_ENC`/`msgK_XCA`는 `OnXecure`로, `msgK_TICK`/`msgK_POP`/`msgK_ARM`/`msgK_AUX`/`msgK_DIAL`은 각자 다른 함수로 — 전부 `OnRead`의 스위치 안에서 갈라지고, 이 두 번째 `OnAxis`는 그 갈래 중 하나일 뿐이다(1절 "두 계층의 msgK 분기" 문단과 정확히 같은 내용, 실제 소스 위치가 여기다).

**요약 — 두 함수의 관계는 "형제"이지 "부모-자식"이 아니다:** 둘 다 `OnFireEvent`(또는 그 아래 `OnRead`)라는 같은 조상에서 갈라져 나온 서로 다른 가지이고, 이름이 같은 건 오버로드일 뿐 호출 관계가 아니다. 아래 다이어그램은 이 구조를 정확히 반영해 다시 그림:

```
[서버] TCP 소켓
    │  (조각날 수 있음, statCON 플래그로 표시)
    ▼
CWizardCtrl::OnFireEvent(int type, char* pBytes, int nBytes)   ← 소켓 OCX 이벤트 최상위 분배점
    │
    ├─ type=FEV_OPEN/FEV_RUN(그 외 미분류 이벤트 전부) ─▶ OnAxis(int type, ...)   ← 연결 생명주기(①)
    │
    └─ type=FEV_AXIS ─▶ OnRead(pBytes, nBytes)     ← _axisH 헤더 파싱 + msgK 1차 분기
                             │  statCON 조각 재조립 없음(그건 다음 단계), statENC면 여기서 복호화(8.9절)
                             │  msgK_AXIS/TAB/HTM/SVC/APC/CTRL/UPF/DNF만 아래로:
                             ▼
                         OnAxis(struct _axisH* axisH, char* pBytes, int nBytes)   ← 패킷 1건 진입점(②)
                             │  axisH->winK로 대상 CClient/CDll(works) 탐색, 없으면 서버 지시로 자동 생성 가능
                             │  auxsMAP 플래그면 서버가 화면 전환을 지시할 수 있음
                             ▼
                         works->OnStream(axisH, pBytes, nBytes)     [CWorks::OnStream, Works.cpp:59]
                             │  statCON 비트로 조각 재조립 (m_axisB에 누적)
                             │  마지막 조각이면:
                             ▼
                         works->OnAxis(axisH, 재조립된pBytes, nBytes)   [가상함수, CClient/CDll이 실제 구현]
                             │  ※ 필드 단위 파싱 상세 — 8.5절 이하
                             ▼
                         CScreen 필드에 값 반영 (WriteData 등)
                             ▼
                         WM_PAINT → CAxisForm::DrawForm → CfmBase::Draw(dc)   ← 실제 화면 렌더링
```

**새 플랫폼 설계 시사점:** 이름이 같은 오버로드가 서로 다른 진입점이라는 사실 자체가, 원본 설계자가 "소켓 컨트롤의 연결 상태 이벤트"와 "TR 데이터 패킷"을 의도적으로 별개의 개념으로 분리했다는 근거다. 새 플랫폼에서도 (a) 연결 수립/핸드셰이크 상태머신과 (b) `_axisH` 프레임 파싱 루프를 하나의 함수로 뭉치지 말고 처음부터 분리해서 설계하는 게, 이 코드베이스가 실제로 걸어온 구조와 일치한다.

**추가로 이번에 `OnRead`를 전수 확인하며 §1/§9의 미해결 항목 일부가 풀림:**
- **`msgK_ARM`(0x92)** — `OnFire(FEV_AXIS, MAKEWPARAM(alarmPAN, false), (LPARAM)텍스트)`로 라우팅됨. `msgK_POP`(0x91, 모달리스 다이얼로그)과 **같은 `alarmPAN` UI 타깃**을 쓰되 두 번째 인자가 `true`(POP) vs `false`(ARM)로 갈림 — 두 msgK가 UI상 같은 패널을 공유하는 변형 관계로 보인다(정확한 `false` 플래그의 의미는 `alarmPAN` 처리부 쪽 추가 확인 필요).
- **`msgK_AUX`(0x93)** — `OnAlert(0, chain, axisL)`(3-인자 오버로드, WizardCtrl.cpp:641)로 라우팅됨. 이 함수는 페이로드를 `<코드>\t<나머지>`로 텍스트 파싱한 뒤 `m_guard->OnAlert(code, pBytes, nBytes, type=0)`을 호출한다 — **`FEV_ANM`(RTM, `OnAlert(pBytes,nBytes)` 2-인자 오버로드)과는 다른 시그니처**이므로, 8.11.3절에서 미해결로 남겨뒀던 "`msgK_AUX`가 `_anmH`/RTM과 같은 채널인지"는 **"아니다, 최소 클라이언트 진입점(`CWizardCtrl`)에서는 별도 오버로드로 갈라진다"**까지는 확인됨. 다만 `CGuard::OnAlert`의 2-인자/4-인자 두 오버로드가 내부적으로 같은 `DoRTM` 파이프라인으로 합류하는지는 이번 조사 범위 밖(추가 확인 필요, 9절 반영).
- **`msgK_AXIS`/`msgK_TAB`에 숨어있던 특수 `winK` 라우팅** — `winK_FMX`(0x1e, "AXIS/Frame TRx service")/`winK_DDE`(0x1f, "DDE service")면 일반 화면 라우팅(②) 대신 `OnAux()`를 거쳐 각각 `OnFire(FEV_FMX,...)`/`m_guard->OnDDE(...)`로 빠진다 — 즉 같은 msgK라도 `winK`값에 따라 "화면에 그리는 일반 TR"과 "DDE/외부프레임 연동용 TR"로 완전히 다른 목적지로 갈릴 수 있다. `CDde`(`WizardArchitecture.md` §7.5)로 이어지는 연결고리가 이번에 확정됨.

실시간 시세(RTM) 갱신은 별도 경로(`CGuard::OnAlert`→`DoRTM`)를 타며 `@docs/RealtimeCodeIndex_Investigation.md`에 상세 기록됨 — 이 문서에는 개요만 요약(6절).

### 0.1. 단순 TR 송수신 — 함수 호출 스택 정리 (2026-08-18)

로그를 함수 이름으로 따라갈 때 참고용. **이 문서 안에서 이미 상세를 다룬 함수는 절 번호만 표기**하고 여기서는 반복 설명하지 않는다 — 아래는 어느 함수가 어느 함수를 부르는지의 "지도"만 제공한다. 각 줄은 `함수명(File.cpp:줄번호)` 형식.

**① 수신(서버→클라이언트, 응답/TR 데이터가 화면에 반영되기까지):**
```
CWizardCtrl::OnFireEvent(WizardCtrl.cpp:203)                 [소켓 OCX 이벤트 최상위 분배점, 0절]
  └─ type=FEV_AXIS
      CWizardCtrl::OnRead(WizardCtrl.cpp:686)                [_axisH 헤더 파싱 루프, msgK 1차 분기, statENC 복호화(8.9절)]
        └─ msgK_AXIS/TAB/HTM/SVC/APC/CTRL/UPF/DNF
            CWizardCtrl::OnAxis(_axisH*)(WizardCtrl.cpp:1064)    [winK로 대상 창 탐색, auxsMAP 화면전환, auxsCLOSE, 0절]
              └─ CWorks::OnStream(Works.cpp:59)                 [statCON 조각 재조립, 2절]
                    └─ works->OnAxis(가상함수)
                        ├─ CClient::OnAxis(Client.cpp:366)      [일반 작업영역 — 실제로 하는 일은 아래 OutStream 위임뿐]
                        │     └─ CStream::OutStream(Stream.cpp:90)   [msgK별 2차 분기, 8.5절]
                        │           ├─ msgK_AXIS, auxsOOP=0, m_outs 없음  → SetDataNRM(Stream.cpp:1524)
                        │           ├─ msgK_AXIS, auxsOOP=0, m_outs 있음  → SetDataNRM2(Stream.cpp:1714)      [8.13절]
                        │           ├─ msgK_AXIS, auxsOOP=1               → SetDataOOP(Stream.cpp:1900)      [8.12절]
                        │           ├─ msgK_TAB,  m_outs 없음              → SetDataTAB(Stream.cpp:2095)
                        │           ├─ msgK_TAB,  m_outs 있음              → SetDataTAB2(Stream.cpp:2269)    [8.13절]
                        │           │     (위 4개 함수 공통) SetDataH(Stream.cpp:2449)  [레코드/원장 헤더, 8.7절]
                        │           │     (FM_GRID 필드 만나면) SetCells(2484)/SetCells2/SetTable(2698)/SetTable2  [8.6절]
                        │           │     (FCC/RCC/SCC 만나면) ParseCC/ParseCC2 → ParseFCC/SetCC, ParseRCC(×2), ParseSCC  [8.5절]
                        │           └─ form->WriteData(...) / WriteAll(...)   [axisform.dll, CfmBase 파생]
                        └─ CDll::OnAxis(Dll.cpp:530)            [DLL 기반 작업영역 — OutStream 안 탐, WM_USER로 외부 DLL에 그대로 위임, 9절 미조사]
CScreen 필드 값 반영 완료
  └─ WM_PAINT → CAxisForm::DrawForm → CfmBase::Draw(dc)         [실제 화면 렌더링]
```

**② 송신(클라이언트→서버, 스크립트/사용자 조작이 TR을 쏘기까지):**
```
스크립트 Screen.Send(target) / 사용자 조회버튼 클릭
  └─ CxScreen::_Send(xscreen.cpp:366)
      └─ CStream::InStream(CScreen*, bool, CString)(Stream.cpp:58)
            │  m_lock 재진입 체크
            └─ CScreen::OnTRAN(byKey)(Screen.cpp:1748)          [Send 직전 전체 필드 재검증, KnowledgeBase.md §15]
                  └─ CStream::MakeStream(CScreen*, CString)(Stream.cpp:1394)
                        │  screen->m_state & waitSN 체크 — 이전 응답 안 왔으면 조용히 drop(8절/KnowledgeBase §15)
                        │  screen->m_mapH->options 로 분기:
                        ├─ OP_OOP,  OP_OOP2=0  → GetDataOOP(Stream.cpp:822)     [8.12절]
                        ├─ OP_OOP2=1           → GetDataOOP2(Stream.cpp:1092)   [8.12절]
                        ├─ !OP_OOP, GetTranInfo()=true  → GetDataNRM2(Stream.cpp:579)
                        └─ !OP_OOP, GetTranInfo()=false → GetDataNRM(Stream.cpp:333)
                              │  (위 4개 함수 공통, TH_LEDGER면) GetDataH(Stream.cpp:298)   [8.7절, SetDataH의 송신측 대응]
                              │  (OP_CERTIFY면) m_guard->Certify(...)
                              │  (OP_ENC면) m_guard->Xecure(DI_ENC,...)   [8.9절]
                        └─ CGuard::RouteTR(CClient*)(Guard.cpp:2874)
                              └─ CGuard::Write(char*, int, bool)(Guard.cpp:3479)  [실제 소켓 write, 8.8절 — 한 write에 여러 화면 프레임 배치 가능]
```

**참고 — 송신측 함수명에 `Get`, 수신측 함수명에 `Set` 접두어가 붙는 명명 규칙이 그대로 대칭을 이룬다:** `GetDataNRM`↔`SetDataNRM`, `GetDataOOP`↔`SetDataOOP`, `GetDataH`↔`SetDataH`. `SetDataTAB`/`SetDataTAB2`만 짝이 되는 `GetDataTAB`이 없는데, 이는 `msgK_TAB`이 `OP_TABS` 옵션에서만 파생되고 별도의 `GetData*` 조립 경로 없이 `GetDataNRM`(tab 플래그 인자로 구분)이 그대로 쓰이기 때문으로 보인다(`MakeStream`의 `axisH->msgK = msgK_TAB; tab = true;` 분기, `GetDataNRM(screen, ..., tab)` 호출부 참고) — 새 플랫폼 설계 시 "요청 조립기는 4갈래(NRM/OOP/OOP2/+tab플래그)인데 응답 파서는 5갈래(NRM/NRM2/OOP/TAB/TAB2)"라는 비대칭이 있다는 점에 유의.

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

### `msgK` (메시지 종류) — `h/axis.h:44~70` (전수 확인, 2026-07-31 갱신)

| 값 | 상수 | 의미 | 처리 경로 |
|---|---|---|---|
| 0x20 | `msgK_AXIS` | 일반 AXIS 메시지 (TR 조회 요청/응답) | `OutStream` (요청-응답, 8.5절) — **단, `winK`가 `winK_FMX`(0x1e)/`winK_DDE`(0x1f)면 일반 화면 라우팅 대신 `OnAux()`→`OnFire(FEV_FMX,...)`/`CGuard::OnDDE`로 별도 처리(2026-08-18 확인, 0절 참고)** |
| 0x21 | `msgK_HTM` | HTML 메시지 | `OutStream` |
| 0x22 | `msgK_TAB` | 탭 구분 메시지 | `OutStream` (`winK_FMX`/`winK_DDE` 예외는 위와 동일) |
| 0x24 | `msgK_SVC` | 서비스 콜 | `OutStream` |
| 0x25 | `msgK_APC` | 승인 콜 | `OutStream` |
| 0x26 | `msgK_CTRL` | 컨트롤(`FM_CONTROL`) 데이터 직접 갱신 | `OutStream` — **실측 확인(2026-08-19): IBKSConnector의 `GOOPHOOP`(TR1007, OOP 캔들조회, 8.10절 6항)도 이 `msgK`로 나간다** — `msgK_AXIS`가 아님, TR 요청이 곧 `msgK_AXIS`라는 가정은 성립하지 않음 |
| 0x27 | `msgK_UPF` | 파일 업로드 | `OutStream` |
| 0x28 | `msgK_DNF` | 파일 다운로드 | `OutStream` |
| 0x30 | `msgK_RSM` | 리소스 요청 (맵 파일 등, `CGuard::RequestMAPs`가 씀) | `CGuard::OnRsm` (OutStream 안 탐) |
| 0x40 | `msgK_RTM` | 실시간 시세 데이터 | `CGuard::OnAlert`→`DoRTM` (RTM 전용 경로, `@docs/RealtimeCodeIndex_Investigation.md`) |
| 0x50 | `msgK_MAPX` | 맵(화면) 전환 지시 | 미조사 |
| 0x80 | `msgK_ENC` | 암호화 키 데이터 | `CWizardCtrl::OnXecure` |
| 0x81 | `msgK_XCA` | 인증서(Certify) 데이터 | `CWizardCtrl::OnXecure` |
| 0x82 | `msgK_SIGN` | 로그인/로그아웃(sign on/off) | `OnStream`(`OutStream`이 아니라 별도 스트림 진입점) |
| 0x83 | `msgK_SIGNx` | 로그인/로그아웃(인증서 로그인) | `OnStream` |
| 0x90 | `msgK_TICK` | **"tick pane(notice...)"** — 서버가 임의 시점에 미는 브로드캐스트 알림(체결통보류). TR 요청-응답이 아니라 요청 없이 도착 | `CGuard::OnNotice` → `CScreen::OnNotice`(`$$` 마킹 그리드, 2026-07-31 확인) |
| 0x91 | `msgK_POP` | 모달리스 다이얼로그(ASCII 컨트롤 데이터) | `OnFire(FEV_AXIS, MAKEWPARAM(alarmPAN,...))` |
| 0x92 | `msgK_ARM` | 알람 메시지 | **`OnFire(FEV_AXIS, MAKEWPARAM(alarmPAN, false), ...)`(2026-08-18 확인)** — `msgK_POP`과 같은 `alarmPAN` UI 타깃, 두 번째 인자만 `false`로 다름(0절 참고) |
| 0x93 | `msgK_AUX` | AUX 실시간 메시지 | **`CWizardCtrl::OnAlert(0, chain, axisL)`(3-인자 오버로드, 2026-08-18 확인)** — `<코드>\t<데이터>` 텍스트 파싱 후 `CGuard::OnAlert(code,...,type=0)` 호출. RTM(`FEV_ANM`)과는 다른 오버로드로 갈라짐(0절 참고), `CGuard::OnAlert` 내부 합류 여부는 미확인 |
| 0x94 | `msgK_DIAL` | 확인 다이얼로그(`axisH.winK`가 세부 다이얼로그 타입) | 미조사 |
| 0x99 | `msgK_ERR` | 에러 메시지 | `OutStream` |

**중요 — 두 계층의 `msgK` 분기가 있다.** `CWizardCtrl::OnRead`(WizardCtrl.cpp:719-768)가 소켓에서 막 재조립한 모든 프레임에 대해 **1차로** `msgK`를 검사하고, `AXIS`/`TAB`/`HTM`/`SVC`/`APC`/`CTRL`/`UPF`/`DNF`/`ERR` 계열만 `CWizardCtrl::OnAxis`를 거쳐 `CStream::OutStream`(8.5절 표)까지 도달한다. `SIGN`/`SIGNx`/`RSM`/`ENC`/`XCA`/`TICK`/`POP` 등은 **`OutStream`을 아예 타지 않고** `OnRead`의 switch 안에서 곧바로 별도 함수로 갈라진다 — 즉 8.5절의 `OutStream` 표는 "TR 요청-응답 계열"만 다루는 하위 집합이고, 이 표가 실제 전체 `msgK` 공간이다. 새 플랫폼의 소켓 디스패처는 이 두 계층 구조(전체 `msgK` 1차 분기 → 그중 TR 계열만 2차로 요청/응답 매칭)를 그대로 반영해야 한다.

### `auxs` 부가상태 (확인된 것)

| 플래그 | 의미 | 근거 |
|---|---|---|
| `auxsMAP` (0x01) | 서버가 화면(맵) 전환을 지시 — `works->Attach(mapN, true)` 트리거 | `WizardCtrl.cpp:1104-1109` |
| `auxsFDS` (0x02) | FDS(시세?) 값 포함 여부 — `Guard.cpp`의 `Write`/`Invoke`에서 암호화 분기와 함께 사용 | Guard.cpp 다수 |
| `auxsOOP` (0x10) | **이 프레임이 OOP 포맷("symbol data interface")임을 표시.** 요청 쪽이 `axisH->auxs |= auxsOOP`로 세팅(`GetDataOOP`/`GetDataOOP2` 사용 시, 8.12절), 응답도 서버가 이 비트를 그대로 실어서 돌려줌 — `OutStream`이 `SetDataOOP` vs `SetDataNRM`/`NRM2` 분기 기준으로 씀(8.12절). **2026-08-19 실측 로그로 확인**: 요청/응답 모두 `auxs=16` | `h/axis.h:91` |
| `auxsCLOSE` (0x20) | 이 창을 닫으라는 서버 지시 | `WizardCtrl.cpp:1098` |

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

### 4.2 그리드 특수역할 마커 — `vals[2]`의 `$$`/`$?`/`$*` (Screen.cpp:372-385, 2026-08-14 확인)

**⚠️ 주의 — 8.10절의 `$`/`?` 마커와 이름만 같을 뿐 완전히 다른 메커니즘이다.** 8.10절은 **와이어 프로토콜**(클라이언트→서버 OOP TR 요청 페이로드) 레벨의 마커였고, 이 절은 **맵소스 파싱 시점**(화면이 열릴 때 `CScreen::Parse()`가 `.map` 바이너리를 읽는 순간)에만 작동하는 **UI 역할 지정** 마커다. 소스 위치도 다르다(하나는 `IBKSConnector`, 하나는 `Wizard/Screen.cpp`) — 우연히 `$`라는 같은 특수문자를 재사용했을 뿐 서로 무관하다.

`CScreen::Parse()`의 `FM_GRID` 케이스(352-386행)에서, 그리드 컨트롤의 `vals[2]`(맵소스 빌드 시 지정하는 부가 문자열 값)을 검사해서 그 그리드가 세 가지 특수 역할 중 하나를 맡는지 판별한다:

```cpp
if (form->m_form->vals[2] != NOVALUE)
{
    form->m_form->vals[2] = (DWORD)&m_strR[form->m_form->vals[2]];
    text = (char *)form->m_form->vals[2];
    if (text.Find("$$") == 0)
    {
        form->m_form->vals[2] += 2;
        m_notice = form;
    }
    else if (text.Find("$?") == 0)
        m_sales = form;
    else if (text.Find("$*") == 0)
        m_push = form;
}
```

| 마커 | 대상 멤버 | 실제 소비하는 함수 | 역할 |
|---|---|---|---|
| `$$` | `m_notice` | `CScreen::OnNotice()` (Screen.cpp:1188) | `msgK_TICK`(0x90, 1절) 브로드캐스트 알림(체결통보 등, 요청 없이 서버가 미는 데이터)을 표시하는 그리드로 지정 |
| `$?` | `m_sales` | `CScreen::UpdateRTM()`/`ScrollRTM()` (Screen.cpp:897, 1132) | RTM 틱 수신 시 새 행을 스크롤 삽입하는 관심종목형 확장 그리드로 지정 |
| `$*` | `m_push` | `CScreen::OnPush()` (Screen.cpp:719) | 푸시 메시지 전용 그리드로 지정 |

**정정(2026-08-14):** 이전 대화에서 이 마커를 `text.Find("$") == 0`(홑따옴표 `$` 하나)로 설명한 적이 있는데, 실제 소스는 **`"$$"`(더블 달러)**다. 단순 `$`로 시작하고 `$$`/`$?`/`$*` 중 아무것도 아닌 `vals[2]` 값은 이 세 역할 중 어디에도 매칭되지 않고 그냥 지나간다.

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

**정정(2026-07-31):** 위 ①(`CWizardCtrl::OnAxis`)이 실제로는 "첫 분기점"이 아니다 — 더 앞단인 `CWizardCtrl::OnRead`(소켓에서 막 재조립한 프레임을 받는 지점, 복호화도 여기서 일어남·8.9절)에 `msgK` 전체를 보는 1차 switch가 있고, 이 표에서 다루는 ①~④ 체인은 그중 TR 요청-응답 계열(`AXIS`/`TAB`/`HTM`/`SVC`/`APC`/`CTRL`/`UPF`/`DNF`)만 `OnAxis`로 넘겨받은 것이다. 전체 `msgK` 공간과 1차/2차 분기 구조는 1절 표 참고.

### `msgK` 종류 — `OutStream`까지 도달하는 것만 (TR 요청-응답 계열 하위 집합, `CStream::OutStream` 실측)

> 전체 `msgK` 값 목록(`SIGN`/`TICK`/`ENC` 등 `OutStream`을 안 타는 것 포함)은 1절 `msgK` 표 참고 — 이 표는 그중 `OutStream`까지 도달하는 것들의 세부 처리만 다룬다.

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

### 인밴드 제어코드 (FCC/RCC/SCC) — 2026-08-18 재정정: 셋이 서로 다른 용도(속성/드롭다운데이터/색상)

`h/axis.h:280,295,316` — 데이터 스트림 안에 인쇄 불가능한 ASCII 제어문자를 마커로 심어서 일반 필드값과 구분. **2026-07-26에 "셋 다 색상이 아니라 속성 제어"라고 정정했던 것을, `h/axis.h`의 구조체 정의와 `SCC`의 실제 구현(`ParseSCC`)을 마저 확인하고 나서 다시 정정한다 — 셋은 하나로 뭉뚱그릴 수 없고, 서로 완전히 다른 세 가지 일을 한다.**

| 마커 | 값 | 실제 용도 |
|---|---|---|
| `FCC` | 0x1A | **필드/셀 속성 토글.** 아래 "`CC_*` 플래그 전체 목록" 참고 |
| `RCC` | 0x1B | **드롭다운/트리 목록 데이터 채우기.** 속성 제어가 아니라 **콤보박스 항목이나 트리 항목 텍스트 자체를 서버가 실시간으로 내려보내는 것** — 아래 "RCC 상세" 참고 |
| `SCC` | 0x1C | **정말로 색상 지정이다.** `_SCC` 구조체 정의 자체에 `// SET color command`라는 주석이 있고(`h/axis.h:317`), `ParseSCC`(Stream.cpp:2998)가 `form->SetBkColor(scc->back)`/`SetFgColor(scc->text)`를 직접 호출한다 — **2026-07-26의 "색상 아님" 정정은 SCC에는 틀렸던 것으로 정정.** (FCC가 색상이 아니라는 부분은 여전히 맞음 — FCC는 아래 `CC_*` 표대로 속성 토글이지 색상과 무관.) |

값이 일반 텍스트/숫자 필드에 나올 수 없는 제어문자 범위(0x1A~0x1C)라 마커로 안전하게 구분 가능 — 새 플랫폼 파서도 이 바이트들을 필드 데이터와 구분해서 스킵/해석해야 함.

#### `FCC` — `CC_*` 플래그 전체 목록 (2026-08-18 확인, `h/axis.h:288-293`)

```cpp
struct _FCC { unsigned char fcc; unsigned char scc; unsigned char rcc; };   // scc="set control command", rcc="reset control command" (이름이 헷갈리지만 SCC/RCC 마커와 무관 — 같은 이니셜을 재사용한 필드명일 뿐)

#define CC_SET   0x01   // focus
#define CC_VIS   0x02   // visible
#define CC_PRO   0x04   // protect
#define CC_MAND  0x08   // mandatory
#define CC_SEND  0x10   // send
#define CC_ENB   0x20   // enable
```

`ParseFCC`(Stream.cpp:2938)가 `scc`/`rcc` 두 바이트에서 **XOR로 "이번에 실제로 바뀌는 비트"만 골라낸 뒤, scc 쪽에 속한 비트는 SET, rcc 쪽에 속한 비트는 RESET**으로 `SetCC()`에 넘긴다 — 한 레코드로 "이 플래그들은 켜고, 저 플래그들은 끈다"를 동시에 표현하는 인코딩:
```cpp
cc = (fcc->scc ^ fcc->rcc) & fcc->scc;   // scc에만 있는 비트 = 켤 것
SetCC(form, cc, true, col, row);
cc = (fcc->scc ^ fcc->rcc) & fcc->rcc;   // rcc에만 있는 비트 = 끌 것
SetCC(form, cc, false, col, row);
```

`SetCC()`(Stream.cpp:2951)의 실제 효과:

| 플래그 | 효과 |
|---|---|
| `CC_PRO` | `FA_PROTECT` 속성 토글 — 보호(읽기전용) |
| `CC_MAND` | `FA_MAND` 속성 토글 — 필수입력 |
| `CC_SEND` | `FA_SEND` 속성 토글 — 값 변경 시 자동전송 대상 여부 |
| `CC_SET` | `FA_REF` 속성 토글(SET일 때만) — 정확한 UI 의미 미확인, 주석상 "focus"로 추정 |
| `CC_VIS` | `form->SetVisible()` (그리드/테이블이고 `(row,col)` 둘 다 지정된 경우는 예외적으로 건너뜀 — 셀 단위 표시제어는 이 경로가 아닌 것으로 보임, 화면 전체 필드의 보이기/숨김만 처리하는 듯) |
| `CC_ENB` | `FM_GRID`면 `CfmGrid::SetCellEnable(row,col,...)`(셀 단위), 그 외는 `form->SetEnable()`(필드 전체) |

#### `RCC` — 드롭다운/트리 데이터 채우기, 그리고 이름이 같은 두 함수

**정정:** 기존에 "행 단위 속성 제어"라고 적었던 게 부정확했다. 실제로 `_RCC` 구조체(`h/axis.h:296`, `name[16]` + `ccs`(타입) + `ccl`(뒤따르는 항목 수))가 담는 건 **콤보박스/트리 컨트롤에 채울 항목 목록 자체(텍스트 데이터)**이지, FCC 같은 on/off 속성이 아니다. `rcc->ccs`가 `csCOMBO`(0x01)/`csTREE`(0x02) 중 뭘 채우는지 결정한다.

**⚠️ 이름이 같은 함수가 클래스만 다르게 두 개 있다(2026-08-18 확인, `OnAxis` 사례와 같은 패턴):**

| 함수 | 호출 경로 | 대상 |
|---|---|---|
| `CStream::ParseRCC(screen, datB)` (Stream.cpp:3011) | `CStream::ParseCC` → `SetDataNRM`/`SetDataTAB`이 일반 필드 스트림을 순회하다 만남 | **화면의 일반 필드**(그리드 아닌) 하나의 콤보/트리 항목을 통째로 채움 |
| `CScreen::ParseRCC(form, datB, index)` (Screen.cpp:3211) | `CStream::SetTable`(8.6절)이 그리드 셀 단위로 순회하다 만남 — `form->GetEnum()`으로 `rcc->name`을 **컬럼 인덱스로 역조회**한 뒤 `((CfmGrid*)form)->SetCellItems(row, col, ...)` | **그리드의 특정 셀** 하나의 콤보 항목만 채움 |

즉 "그리드 안이냐 밖이냐"로 완전히 다른 클래스의 동명 함수가 호출된다 — 새 플랫폼에서 RCC를 구현할 때도 "필드 전체용"과 "그리드 셀용" 두 갈래를 구분해야 한다. `csTREE`는 항목을 그대로 UI에 채우는 것 외에 `USRDIR`(`m_guard->m_root` 하위)에 파일을 쓰는 부수효과도 있는 것으로 보이나 상세는 이번 조사 범위 밖(9절 반영).

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

## 8.10. IBKSConnector OOP 요청 페이로드 문법 — `pooppoop`/`GOOPHOOP` 계열 (2026-08-14 확인)

### 배경 — 이 절이 다루는 범위

8.9절까지는 `trxC=piboPBxQ`(`US_ENC`, 일반 AXIS 포맷)류를 다뤘다. 이 절은 **`trxC`가 `pooppoop`/`GOOPHOOP` 등인 `US_OOP` 포맷 TR의 요청 페이로드를 클라이언트(IBKSConnector OCX)가 어떻게 조립하는지**를 다룬다. `MigrationSpec` 9절에 오래 남아있던 `SetDataOOP`(서버 응답을 Wizard가 파싱하는 쪽, `Stream.cpp`)와는 **반대 방향(클라이언트→서버 요청 조립)이며 소스 위치도 다르다** — `Wizard/` 안이 아니라 `HTS_OpenAPI(운영)/IBKSConnector/IBKSConnectorCtl.cpp`에 있다. 즉 OOP 프로토콜의 "요청 조립" 절반은 이번에 확정됐고, "응답 파싱"(`SetDataOOP`) 절반은 여전히 미확인이다(9절 참고).

**⚠️ 주의 — 4.2절의 `$$`/`$?`/`$*`(그리드 특수역할 마커)와 이름만 같을 뿐 무관한 별개 메커니즘이다.** 저건 맵소스 파싱 시점(`CScreen::Parse()`)의 UI 역할 지정이고, 이 절은 와이어 프로토콜(요청 페이로드) 레벨의 마커다.

**중요 — 이 문법은 `trxC` 자체와 무관하게, `_axisH` 헤더(24바이트) 다음에 오는 페이로드 안에서 작동하는 문법이다.** 즉 1절의 `_axisH` 구조(헤더 24바이트+`datL`만큼의 페이로드), 8.9절의 암호화 규칙(헤더 이후 페이로드만 암호화)은 OOP 포맷에도 동일하게 적용되고, 이 절은 그 페이로드 **내부**의 문법만 다룬다.

**근거:** `HTS_OpenAPI(운영)/IBKSConnector/IBKSConnectorCtl.cpp`의 `S_TR1002`/`S_TR1003`/`S_TR3002`/`S_TR3003`(2341~2505행)과 `IBKSConnector_test/IBKSConnectorCtl.cpp`의 `S_TR1007`(2527~2662행, 원본 CP949 주석 iconv로 확인) — 전부 실제 소스 코드 직접 확인(로그/추정 아님).

### 1) 단일값(비그리드) 조회 — 특수문자 없음

`S_TR1002`(주식 조회), `S_TR3002`(선물옵션 조회):
```cpp
sdat.Format("1301\x7f%s\t%s\t", code, data);
return SendTR("pooppoop", key, US_OOP, (LPCSTR)sdat, sdat.GetLength(), &CIBKSConnectorCtrl::C_TR1002);
```

**문법:** `<필드코드>\x7f<값>\t<필드코드2>\t<필드코드3>...\t`

- `1301` = "종목코드" 필드코드. 여기에 값을 채우고 싶으면 `\x7f`(0x7f, DEL 문자)로 필드코드와 값을 붙인다: `1301\x7f005930`.
- 값 없이 "이 필드코드를 조회해달라"만 요청할 때는 필드코드만 탭(`\t`, 0x09)으로 나열한다.
- `data`(함수 인자명은 파일마다 `data`/`symb`로 다르게 불리지만 실체는 같음, Python `dynamicCall`에서는 `TR1002(int, QString, QString)`의 세 번째 인자)는 **조회하고 싶은 필드코드들을 탭으로 이어붙인 문자열**이다. "심볼 하나"가 아니라 "필드코드 목록"이다.
- 맨 끝에 항상 trailing 탭이 하나 더 붙는다(`Format(...%s\t)`의 마지막 `\t`).

**실측 예시(사용자가 앞서 캡처한 로그)와 대조:**
```
preview=[1301024110	1034	1022	1023	1024	1033	1027	1025	1026	1029	1030	1031	1021	1310	1311	1312	1313	1316	131...
```
- `1301024110` = `1301` + `\x7f`(비인쇄 문자라 화면엔 안 보임) + `024110`(종목코드) — `\x7f`가 프린트 안 되는 문자라 로그 미리보기에서는 필드코드와 종목코드가 그냥 붙어 보인 것.
- 그 뒤 `1034\t1022\t1023\t1024...`는 조회를 요청한 필드코드 목록 — 실제로 이 18개 목록(`1034,1022,1023,1024,1033,1027,1025,1026,1029,1030,1031,1021,1310,1311,1312,1313,1316,1318`)은 `S_TR1007`이 내부적으로 쓰는 `snapFields[]` 배열과 정확히 동일하다(아래 3절 참고) — "스냅샷 18필드"라는 이름으로 여러 TR 호출부에서 재사용되는 공통 필드 세트로 보인다.

**사용자 질문에 대한 답:** "`1301 0x7f 실제종목코드 0x09 심볼 0x09` — 종목코드는 1301 값이고 심볼에 해당하는 데이터 조회시 사용" — **골격은 정확히 맞다.** 다만 "심볼"이라는 이름 때문에 오해할 수 있는데, 실제로는 단일 심볼이 아니라 **조회하고 싶은 필드코드들을 탭으로 이어붙인 문자열 전체**가 그 자리에 들어간다(위 로그 예시처럼 18개가 한 번에 들어갈 수 있음). 마지막 탭은 종결자다.

### 2) 그리드(다중 레코드) 조회 — `$` 마커

`S_TR1003`(주식 그리드 조회, 예: 체결/호가 등 여러 행이 필요한 조회), `S_TR3003`(선물옵션 버전):
```cpp
sin.Format("1301\x7f%s\t10302\x7f%d\t$10310\x7f", code, type);   // 3003은 "30301.../$33310" 또는 "40301.../$43310"
sout = columns; sout.TrimRight(); sout.Replace('\t', '\n'); sout += "\n\t";

vector<char> buff(ilen+glen+olen);
memcpy(&buff[0],         sin,  ilen);
memcpy(&buff[ilen],      &gin, glen);   // grid_i 이진 구조체
memcpy(&buff[ilen+glen], sout, olen);

return SendTR("pooppoop", key, US_OOP, buff.data(), buff.size(), &CIBKSConnectorCtrl::C_TR1003);
```

**문법:** `<일반필드=값...>\t $<그리드시작필드코드>\x7f [grid_i 이진구조체] [컬럼목록, \n구분]\n\t`

- `$10310`(3003은 `$33310`/`$43310`) — **필드코드 앞에 `$`가 붙으면 "여기부터는 단일 값이 아니라 그리드(표) 형태로 응답을 만들어달라"는 마커**다. 1)의 문법과 필드코드/값 부분은 동일하되, 그리드 시작 지점에만 `$`가 붙는다.
- `$필드코드\x7f` 바로 뒤에는 **텍스트가 아니라 이진 구조체 `grid_i`(`grid_i.h`)가 그대로 붙는다** — 정렬/페이징 요청 정보:
  ```cpp
  class grid_i {
      char vrow[2];   // 보이는 행수
      char nrow[4];   // 요청 행수
      char vflg[1];   // 뷰 플래그
      char gdir[1];   // 정렬방향(그리드)
      char sdir[1];   // 정렬방향(부가)
      char scol[16];  // 정렬 기준 컬럼명
      char ikey[1];   // 입력 키(연속조회 시 setIKEY(2))
      char page[4];   // 페이지 번호
      char save[80];  // 연속조회 이어보기 토큰 — 응답 grid_o.save를 그대로 되돌려보냄
  };
  ```
- `grid_i` 바이너리 다음에 **원하는 컬럼(필드) 목록**이 텍스트로 이어진다 — 호출자가 넘긴 `columns`(탭구분)를 개행(`\n`)구분으로 바꾸고 끝에 `\n\t`를 붙인 형태.
- **연속조회(페이징) 메커니즘:** 응답에는 `grid_o`(같은 헤더, `IsNext()`/`GetNKey()` 제공)가 돌아온다. 다음 페이지가 필요하면 `S_TR1003`의 `nkey` 인자에 직전 응답의 `grid_o.save`(80바이트)를 그대로 넣어서 재호출 → `gin.setGRIDO(gout)`으로 `grid_i.save`에 그대로 반영됨. 즉 **`save` 80바이트가 서버-클라이언트 간 "이어보기 토큰"**이고, 새 플랫폼도 이 필드를 불투명한 opaque 토큰으로 왕복시켜주기만 하면 됨(내용 해석 불필요).

### 3) 캔들/시계열 조회 — `?` 마커

`S_TR1007`(차트/캔들 조회, `trxC=GOOPHOOP`, `IBKSConnector_test/IBKSConnectorCtl.cpp:2527`):
```cpp
sendS += "1301"; sendS += (char)0x7f; sendS += code; sendS += "\t";
for (18개 snapFields) sendS += 필드코드 + "\t";     // 종목이면 그대로, 업종(GU_INDEX)이면 "2" 접두(21301, 21034...)

sendS += "1777"; sendS += (char)0x7f; sendS += mkgubn; sendS += "\t";

sendS += (dunit==2) ? "?25500" : "?5500";           // ★ 캔들 마커 — $가 아니라 ?
sendS += (char)0x7f;
// _dataH(136바이트 이진 헤더): count[6] + dummy[6] + dkind + dkey + pday[8] + dunit + dindex + lgap[4] + ltic[4]
//                              + option1 + option2 + rcode[16] + ikey + xpos + page[4] + save[80]

for (캔들필드 10개) sendS += 필드코드 + "\n";        // 시가/고가/저가/종가/거래량/거래대금/권리락/수정비율 등
sendS += "\t";

return SendTR("GOOPHOOP", key, US_OOP, sendS.data(), sendS.size(), &CIBKSConnectorCtrl::C_TR1007);
```

**문법:** 2)와 뼈대는 동일(`특수문자+필드코드\x7f` → 이진헤더 → 필드목록\n구분\n\t)이지만 **마커 문자가 `$`가 아니라 `?`**다(`5500`/`25500` — 종목은 접두어 없이, 업종은 `2` 접두). 이진 헤더도 `grid_i`가 아니라 TR1007 전용 136바이트 `_dataH`(요청 건수/기준일/일봉·주봉·월봉 구분 등)로 다르다.

**주의 — 업종(GU_INDEX) vs 종목(GU_CODE) 전체 접두 규칙:** `dunit==2`(업종)이면 코드값 자체는 그대로(3자리, `00` 접두 없음)이지만 **필드코드/캔들마커 전부에 `2`가 접두**된다(`1301`→`21301`, `5500`→`25500`, 캔들필드 `5302`→`25302` 등). `docs/RealtimeCodeIndex_Investigation.md`류 조사에서 마주칠 수 있는 `2`로 시작하는 필드코드들이 이 규칙과 관련 있을 가능성이 있다 — 교차 확인 가치 있음.

### 4) 공통 요약 — 세 문법을 관통하는 규칙

```
[일반 필드코드\x7f값 / 필드코드만 ...]\t  [마커?][그리드또는캔들시작 필드코드]\x7f  [이진 헤더구조체]  [원하는필드/컬럼목록, \n구분]\n\t
```

| 구분 | 마커 | 이진 헤더 | 헤더 크기 | 뒤따르는 목록 |
|---|---|---|---|---|
| 단일값 (TR1002/3002) | 없음 | 없음 | - | (없음, 앞부분 필드코드 나열이 곧 요청 전부) |
| 그리드 (TR1003/3003) | `$` | `grid_i` | `sizeof(grid_i)` | 컬럼 목록 |
| 캔들/시계열 (TR1007) | `?` | `_dataH` | 136바이트 | 캔들 필드 목록(10개) |

공통 구분자:
- `\x7f`(0x7f, DEL) — 필드코드와 그 값을 붙일 때
- `\t`(0x09) — 필드/파라미터 구분
- `\n`(0x0a) — 그리드/캔들 문법에서만, 목록 항목 구분
- 페이로드 끝은 항상 `\t`로 종결

### 5) 확인 범위와 다음 단계

**확인 완료:** TR1002, TR1003, TR3002, TR3003, TR1007 — 5개 TR의 요청 조립 문법.

**미확인 (다음 관찰 대상):**
- `$`/`?` 외에 다른 마커 문자가 더 존재하는지 (예: 파일업로드/원장류 등 다른 US_OOP TR)
- 이 문법을 쓰는 다른 그리드성 OOP TR이 더 있는지 — `IBKSConnectorCtl.cpp`에서 `US_OOP`로 검색하면 후보를 추가로 찾을 수 있음
- Wizard(서버) 쪽에서 이 요청을 받아 실제로 어떻게 파싱/응답을 만드는지(`SetDataOOP`, 9절의 기존 미확인 항목과 동일 — 이 절이 다룬 건 클라이언트 쪽 절반뿐)

**진행 방법(사용자 제안, 2026-08-14):** AxisChaser로 새로운 그리드성 TR을 캡처할 때마다, "같은 TR의 비그리드 버전(있다면)"이나 "필드코드만 나열한 단순 요청"과 바이트 단위로 대조해서 **어느 필드코드 앞에 특수문자가 붙었는지**만 찾으면 위 4)의 문법이 그대로 적용될 가능성이 높다. 새로 확인되는 대로 이 절에 TR을 추가할 것.

### 6) TR1007(`GOOPHOOP`) 실제 와이어 바이트 최초 확인 + `snapFields[]` 확장 사례 (2026-08-19)

`D:\util\HTS\IBK_SMART\Dev\Send_Rev.ini`(AxisChaser 트레이스 로그)에 남긴 실제 `GOOPHOOP` 캡처를, 그동안 소스코드만으로 기록했던 위 `S_TR1007` 문법과 바이트 단위로 처음 대조함.

**`_axisH` 헤더(1절) 실측 디코딩 — `[Send Data 319 Bytes][Win 53][Unit 254]`:**

```
26 00 10 35 FE 00 00 39 39 39 39 47 4F 4F 50 48 4F 4F 50 30 30 32 39 35
```

| 필드 | 바이트 | 값 | 비고 |
|---|---|---|---|
| `msgK` | `26` | `0x26` = **`msgK_CTRL`**(`h/axis.h:50`) | ⚠️ `msgK_AXIS`(0x20)가 아니다 — GOOPHOOP은 `msgK_CTRL`로 나간다는 게 이번에 실측으로 처음 확인됨. 1절 표엔 GOOPHOOP/TR1007의 실제 `msgK`가 기록돼 있지 않았음(소스로만 문법을 확인했을 뿐 와이어 검증은 안 됨) — 이번 캡처로 채워짐 |
| `stat` | `00` | 0 | |
| `auxs` | `10` | `0x10` = `auxsOOP` | `US_OOP` 선언과 일치 |
| `winK` | `35` | 53 | 캡처 배너의 `[Win 53]`과 일치 |
| `unit` | `FE` | 254 | 캡처 배너의 `[Unit 254]`와 일치 |
| `trxK`/`trxS` | `00 00` | 0/0 | |
| `svcN[4]` | `39 39 39 39` | `"9999"` | |
| `trxC[8]` | `47 4F 4F 50 48 4F 4F 50` | `"GOOPHOOP"` | |
| `datL[5]` | `30 30 32 39 35` | `"00295"` | 319(전체) − 24(헤더) = 295 — 정확히 일치, 헤더 디코딩 자체의 정합성 검증됨 |

**요청 페이로드도 8.10절 문법 그대로 확인됨:** `1301\x7f005930\t` 뒤로 스냅샷 필드 18개(`1034\t1022\t...\t1318\t`)가 그대로, 이어서 `1777\x7f{mkgubn}\t`, `?5500\x7f`+136바이트 `_dataH`, 그리고 캔들필드 10개(`5302\n5034\n...\n5310\n`)까지 — 소스 기반 문법과 실측 바이트가 완전히 일치.

**신규 미션 — 필드코드 `1900`/`1901`/`1902` 추가 (KRX 애프터장 16:00-20:00 개편 대응):** 이 미션은 `[[project_krx_pre_after]]`(2026 하반기 KRX 프리/애프터장 개편, 8월초 개발완료→9월초 오픈)의 일부로, 차트 조회가 확장된 세션 시간 정보를 받아와야 하는 요건이다. 후보는 두 곳이었다(위 4절 "공통 요약" 참고): **스냅샷 블록**(단순 필드코드 나열, `\t` 구분)과 **캔들 블록**(그리드/캔들 마커 뒤, `\n` 구분). 필드코드 사전(`h/axisgwin.h`)을 확인한 결과 셋 다 "시세 마스터"(스냅샷) 계열 필드명 공간에 속해 있고 캔들 필드(`5xxx`대)와는 번호 자체가 다른 영역이라 — **스냅샷 블록에 추가하는 게 맞다고 판단.**

**필드 의미 확정(2026-08-19, 사용자 확인):**

| 필드코드 | 의미 | 값 예시 |
|---|---|---|
| `1900` | 장시작시간 | `0800` |
| `1901` | 장마감시간 | `2000` |
| `1902` | 애프터시작시간 | `1540` |

레거시 소스 두 곳의 기록과 대조: `axisgwin.h`(`OJ_GUBN="1901"`="소속구분")는 **틀린 정보**였음이 확인됨(다른 문맥에서 같은 필드번호를 재사용한 것으로 추정, 정정 필요). `IBK_DRFN/ChartItemStock.cpp` 주석(1900=예상장시작시간, 1901=예상장운영시간종료)은 방향은 맞았으나 1902(애프터시작시간)는 언급이 없었음. `1902=1540`은 KRX의 기존 "시간외단일가"(15:40~16:00) 시작 시각과 일치 — 이번에 신설되는 16:00~20:00 연속거래 애프터장의 시작이 아니라, **기존부터 있던 시간외단일가 세션의 시작을 가리키는 것으로 보임**(확정은 아님, 서버 응답 실측 후 재확인 필요).

**실제 적용 위치 — `IBKSConnector`가 아니라 HTS 자체 차트 엔진(`CONTROL/ibk_chart_dll_20220831/chart_dll`):** 처음에는 `HTS_OpenAPI(운영)/IBKSConnector_test/IBKSConnectorCtl.cpp`(OPEN API, 8.10절 본문의 `S_TR1007`)를 수정했었으나, 사용자가 실제로 원했던 대상은 **HTS 자체가 쓰는 `chart_dll/C_Total`**이었음이 확인되어 IBKSConnector 쪽 변경은 되돌렸다(OPEN API는 HTS 결과를 보고 나중에 별도 반영 예정). `chart_dll`은 GOOPHOOP을 구현한 두번째, 독립적인 클라이언트로, IBKSConnector와 프로토콜(마커/구분자)은 동일하지만 요청 조립 방식이 완전히 다르다(플랫 문자열 연결이 아니라 커스텀 윈도우 메시지로 여러 하위 컴포넌트에 위임하는 구조). 수정은 사용자가 직접 적용함(2026-08-19), 확정된 수정 지점 4곳:

| # | 파일 | 내용 |
|---|---|---|
| 1 | `h/axisgwin.h` | `1900`/`1902`용 이름 상수 추가(`1901`은 기존 `OJ_GUBN`, 의미가 틀렸다는 점 주석 정정 필요) |
| 2 | `chart_dll/C_Total/PnInput.cpp::MakeInputSymbol()` `GU_CODE` 케이스 | 요청 문자열에 3필드 추가 — **입력패널이 있는 일반 차트에서 실제로 도는 경로**(`SendRequest()`가 `m_pwndInput` 존재 시 이쪽을 호출) |
| 3 | `chart_dll/C_Total/ExtraMsg.cpp::GetInputTRInfo()` `GU_CODE` 케이스 | 요청 문자열에 3필드 추가 — `m_pwndInput`이 없는(입력패널 꺼진 레이아웃) 폴백 경로 |
| 4 | `chart_dll/C_Total/ExtraMsg.cpp::ReceiveData()` | **응답 파싱 — 반드시 같이 고쳐야 하는 지점.** 아래 참고 |

**4번이 핵심 — 응답측 필드개수와 "캔들 데이터 시작 오프셋"이 강하게 결합돼 있음:** `ReceiveData(char* pcData, int iLen)`는 스냅샷 블록을 순서대로 토큰화하며 소비하는 두 개의 고정 루프(`for(ii<11)`+`for(ii<7)`=18개, `GetToken()`으로 `\t`/`\n`까지 순차 소비)로 돼 있고, 이 함수의 리턴값(`index`, 소비한 총 바이트수)이 `MainWnd.cpp`를 거쳐 `m_TrData.iSiseLen[0]`에 담겨 **`axisGMain.dll`(별도 DLL, `chart_dll/gMain/`)의 `PnChart.cpp`로 그대로 전달**된다:

```cpp
// gMain/PnChart.cpp, CHART_TR_DATA 핸들러
if (!pTD || !pTD->pcData[0] || pTD->iLen[0] - pTD->iSiseLen[0] == 0 || pTD->iSiseLen[0] == 0)
    { ...ObjectClear()... return FALSE; }
if (m_pObjMgr)
    int iDispDay = m_pObjMgr->DispatchGraphData(pTD);   // 실제 OHLC 봉 생성 엔진(ObjMgr.cpp)
```

`pTD->iLen[0] - pTD->iSiseLen[0]`가 "캔들 데이터의 실제 바이트 길이"로 계산되어 `DispatchGraphData`(진짜 봉 생성 로직, `chart_dll/gMain/ObjMgr.cpp`)에 넘어간다. 즉 **요청 필드를 18→21개로 늘렸는데 `ReceiveData()`의 루프를 그대로 두면, `iSiseLen[0]`이 실제보다 3필드만큼 작게 계산되어 `DispatchGraphData`가 스냅샷 값의 꼬리 일부를 캔들 데이터로 착각하고 첫 봉을 깨뜨린다** — 사용자가 처음부터 정확히 우려했던 지점이고, 코드 추적으로 그 우려가 사실임이 확인됨. 수정 방향은 두번째 루프(`ii<7`)를 `ii<10`으로 늘려 1900/1901/1902 값도 소비하도록 하고(신규 멤버변수 `m_field1900`/`m_field1901`/`m_field1902`에 저장, `ExtraMsg.h`), `index`가 21개 전부를 반영해 자동으로 정확해지게 하는 것.

**IBKSConnector 쪽은 왜 이 문제가 없었나(대조 참고):** `IBKSConnectorCtl.cpp`의 `C_TR1007`은 원시 바이트를 그대로 `FireOnRecvData()`로 패스스루할 뿐 고정 위치 파싱이 없고, 소비자인 `python/trading_bot.py`도 정규식(`(20\d{6})\t\t`)으로 캔들행을 찾는 방식이라 스냅샷 필드 개수 변화에 영향을 안 받는다(`trading_bot.py:2904-2916`). 반면 HTS 자체 `chart_dll`은 실제로 화면에 그려야 하니 고정 위치/오프셋 기반 파싱(`ReceiveData`)이 있고, 그래서 요청 필드 개수 변경이 수신측 코드 변경을 강제한다 — **같은 프로토콜(GOOPHOOP)의 두 클라이언트 구현이 "수신 데이터를 실제로 쓰는지 여부"에 따라 완전히 다른 강건성(robustness) 특성을 가진다**는 게 이번에 확인된 흥미로운 대조점.

**미해결 — 애프터장(16:00-20:00) 개장이 차트 엔진에 미칠 영향:** `gMain/ObjMgr.cpp`, `DataFormat.cpp`(실제 봉 생성/일자 버켓팅 로직)를 세션시간 관련 하드코딩(15:30/18:00/20:00 등) 기준으로 검색했으나 **아무 것도 안 나옴** — 즉 이 차트 엔진이 애초에 "세션 인식"이 없이 서버가 주는 타임스탬프를 그대로 그리는 구조일 가능성이 있다. 이 경우 애프터장이 열려도 `DispatchGraphData` 자체는 안 건드려도 될 수 있으나, **실시간 틱→분봉 합성 경로(RTM 수신 시 봉을 만드는 로직)는 아직 못 찾음** — 만약 거기에 암묵적인 장마감 컷오프가 있다면 애프터장 틱이 오늘 일봉에 안 붙는 위험이 있음. 사용자가 별도로 조사 중.

**다음 확인 단계(미완료):** 실제 재조회 후 캡처해서 (a) 응답 탭 개수가 21개+α로 늘어났는지, (b) `1901` 위치의 값이 "소속구분"류(숫자 0/1/2 등 소분류 코드) 형태인지 "HH:MM" 시각 형태인지 관찰해 두 소스 중 어느 쪽이 맞는지 판별, (c) `1902` 위치에 실제 값이 오는지(빈 문자열이면 서버가 이 필드를 모른다는 뜻) 확인 필요.

---

## 8.11. 그리드 실시간 갱신 3종 마커 — `$?`/`$$`/`$*` (2026-08-17 확인)

**배경:** `CScreen::Parse()`(4절)의 FM_GRID 처리 중 `form->m_form->vals[2]`(빌더 UI의 "Variant" 속성)에 특수 문자열이 들어있으면 그 그리드를 화면당 딱 1개씩 `m_notice`/`m_sales`/`m_push` 중 하나로 등록해두는 코드가 있었음(문서에 등장은 했으나 실제 사용처는 이번에 처음 추적). 이 3개는 **서로 완전히 다른 실시간 갱신 메커니즘**이며, 사용자가 실제 HTS 화면(체결 그리드, 시가총액순위 그리드)과 대조하며 검증함.

**주의 — 이 3종이 "그리드 실시간 갱신"의 전부가 아니다.** `vals[2]`가 비어있는 그리드도 실시간으로 갱신될 수 있다 — 종목코드 매칭으로 기존 행의 셀만 고쳐쓰는 `FlashGrid`/`FlashSemi`라는 4번째(이자 실제로는 더 흔한) 경로가 따로 있으며, `vals[2]`와는 완전히 무관하게 그리드 필드 자체의 `FA_FLASH` 속성만으로 작동한다. 상세는 아래 8.11.4절.

| 마커 | 저장 필드 | 데이터 출처 | 갱신 방식 | 용도(추정) |
|---|---|---|---|---|
| `$?` | `m_sales` | RTM(`_rtmH`, 6절 요약) 스크롤버퍼 | 무조건 `CfmGrid::InsertRow` — 기존 행 매칭 없음 | 체결 틱커(스크린샷으로 실측 확인; `IB999988`의 `11310` 그리드 필드가 `vals[2]="$?"`로 확인됨, 8.14절 참고) |
| `$$` | `m_notice` | RTM(`_rtmH`) | 키 매칭 → 있으면 셀 갱신 or 상태플래그 꺼지면 행삭제, 없으면 행삽입 (upsert) | 미체결/주문 리스트류 (추정, 실측화면 미확인) |
| `$*` | `m_push` | **별도 채널(`_anmH`, 아래 8.11.3)** | 고정크기 원형버퍼, 커서(`m_row`) 위치에 덮어쓰기 | 대량 배치푸시/뉴스크롤류 (추정, 실측화면 미확인) |

### 8.11.1. `$?` — `CScreen::ScrollRTM` (순수 삽입, 매칭 없음)

RTM 파이프라인(1절/6절)에서 종목별로 스크롤형(`stat_SCR`, 아래 참고) 데이터를 버퍼링했다가 한꺼번에 플러시하는 구조:

```
CGuard::OnAlert(code, pBytes, nBytes, stat)   [Guard.cpp:1030]
  rtmK_INFO 패킷: 종목의 필드번호 순서표(rtmk, CStringArray)를 캐시
  rtmK_DATA 패킷: 값 파싱, 종목코드별 CObArray*(scroll맵, obs)에 CdataSet 버퍼링(InsertAt(0,...))
    → 같은 종목 "일반" 시세갱신이 오거나, 이번 소켓패킷 처리가 끝나는 시점에
      DoRTM(code, stat_SCR, rts, obs, updates) 로 일괄 플러시
        ↓
CScreen::UpdateRTM (Screen.cpp:897)
  if (m_sales && (stat & alert_SCR))  ScrollRTM(obs);
        ↓
CScreen::ScrollRTM(CObArray* obs)   [Screen.cpp:1132]
  obs 안 CdataSet들을 컬럼이름으로 조회해 탭구분 멀티라인 문자열로 조립
  (form->m_form->attr2 & GO_TOP) ? InsertRows(0,...) : InsertRows(-1,...)
        ↓
CfmGrid::InsertRows()   [dll/form/fmGrid.cpp:5016, axisform.dll]
  줄바꿈으로 쪼개 insertRow()로 실제 삽입, 기존 행은 밀려남
```

`stat_SCR = 0x10 // scroll data (grid, graph)` (`h/axisanm.h:59`) — 와이어 RTM 헤더(`rtmH->stat`)에 실제로 실리는 비트, 서버가 "이건 스크롤형 데이터"라고 명시적으로 표시함. **행 매칭 로직이 전혀 없음** — 종목코드나 특정 row를 찾는 코드가 `ScrollRTM` 안에 없음(실측 확인).

### 8.11.2. `$$` — `CScreen::OnNotice` 후반부 (키매칭 upsert/삭제)

`OnNotice(CdataSet& major, CdataSet& minor, CdataSet& fms, CString notices)`(Screen.cpp:1188)의 후반부(1288~1397):

```cpp
// 1. minor(키 데이터셋)로 기존 행 중 일치하는 행 탐색
for (kk=0; kk<nRows; kk++)
    for (idx=0; idx<nCols; idx++)
        if (minor.Lookup(form->GetName(idx), string) && text.Compare(string)==0) match=true;

// 2. vals[2]에 남은 숫자("$$201" -> Parse()가 "$$" 2글자 skip -> "201")가
//    "삭제여부" 판단용 필드번호
name = atoi((char*)form->m_form->vals[2]);

if (match && fms.Lookup(name,text) && !atoi(text))
    ((CfmGrid*)form)->RemoveRow(kk);          // 상태플래그 꺼짐 -> 행 삭제
else if (!match && (!fms.Lookup(name,text) || atoi(text)))
    ((CfmGrid*)form)->InsertRow(GO_TOP ? 0 : -1);  // 새 키 -> 행 삽입

if (match)
    for (idx=0; idx<nCols; idx++)
        form->WriteData(text, true, idx, kk);  // 그 행 셀들 갱신
```

`major`는 화면 레벨 컨텍스트 매칭(같은 함수 앞부분, 1203~1286행), `minor`는 그리드 행 단위 키 매칭 — 2단계 스코프 구조. 실측 화면(어떤 맵이 `$$`를 쓰는지)은 아직 미확인 — 동작 특성(키 매칭+상태기반 삭제)상 미체결/주문상태 리스트류로 추정.

### 8.11.3. `$*` — `_anmH`라는 완전히 별도의 소켓 채널 (신규 발견)

**핵심 발견: `$*`는 RTM(`_rtmH`)도 TR(`_axisH`)도 아닌 제3의 독립 프로토콜을 씀.** 소켓 OCX가 쏘는 이벤트 자체가 분리되어 있음:

```cpp
// WizardCtrl.cpp:203, CWizardCtrl::OnFireEvent
case FEV_ANM:   OnAlert(pBytes, nBytes);   break;   // RTM (_rtmH)
case FEV_PUSH:  OnPush(pBytes, nBytes);    break;   // ← $* 전용 채널
case FEV_AXIS:  OnRead(pBytes, nBytes);    break;   // TR (_axisH)
```

**수신 파싱** (`CWizardCtrl::OnPush`, WizardCtrl.cpp:663) — 자체 헤더 `_anmH` 사용:
```cpp
struct _anmH* anmH = (struct _anmH*)pBytes;
switch (anmH->anmK) {
case anmK_ALIVE: break;                              // 하트비트
case anmK_PUSH:  m_guard->OnPush(CString(&pBytes[L_anmH], anmL)); break;
}
```

**구독 요청(클라이언트→서버)** — `CGuard::SetPush(bool push)`(Guard.cpp:715). `$*` 그리드가 있는 화면이 열리면(`CScreen::Parse()`, Screen.cpp:511) `push=true`로 호출됨:
```cpp
// CScreen::isPush(pushN) — 화면의 첫 FM_EDIT 필드값을 "토픽 이름"으로 사용
anmH->anmK = anmK_PUSH;
anmH->anmF = push ? 1 : 0;                // 구독/해지 플래그
if (!pushN.IsEmpty()) {
    // USRDIR/{pushN} 로컬 파일을 읽어 페이로드에 실어 같이 전송 (동기화용으로 추정)
    fileH.Open(path, ...); fileH.Read(&datB[L_anmH], datL);
}
m_sock->InvokeHelper(DI_DWRITE, ...);     // ★ 일반 DI_WRITE(axisH 경로)가 아닌 별도 디스패치
```

**수신 후 처리** — `CGuard::OnPush(CString pushs)`(Guard.cpp:763): 열려있는 모든 `CClient`(`S_LOAD` 상태)에 `client->OnPush(pushs)`로 브로드캐스트 → `CClient::OnPush`(Client.cpp:1502) → `screen->OnPush(pushs)`. 처리 후 **`SetPush(false)`로 즉시 자동 구독해지** — 지속 스트리밍이 아니라 1회성 배치 수신+해지 패턴.

**그리드 반영** — `CScreen::OnPush(CString pushs)`(Screen.cpp:719): `$?`(InsertRow로 밀어내기)와 달리 **고정 크기 원형버퍼**:
```cpp
form->WriteData(cmps, true, idx, m_row);   // 커서(m_row) 위치에 씀
if (++m_row >= rowN) m_row = 0;            // 끝까지 가면 처음으로 순환
// 바로 다음 칸은 미리 공백으로 지워둠 (다음 삽입 예정 위치 표시)
```
`\x1b`(ESC) 이스케이프로 라인별 전경/배경 RGB 직접 지정 가능(`text.Mid(1,3)`/`Mid(4,3)` = 3자리 RGB 코드) — 뉴스크롤/공지사항처럼 서버가 색상 포함된 대량 텍스트를 한 번에 밀어주는 용도로 추정.

**미해결:** `$$`/`$*`를 실제로 쓰는 맵 화면 예시 미확인(`$?`는 체결그리드로 실측 확인됨). `_anmH`/`anmK_*` 상수 전체 목록, `USRDIR/{pushN}` 파일의 실제 포맷과 용도는 여전히 미확인. **이 채널이 1절의 `msgK_ARM`(0x92)/`msgK_AUX`(0x93)와 같은 것인지는 2026-08-18에 부분 확인됨(0절 참고)** — `CWizardCtrl` 레벨에서는 `msgK_AUX`가 `OnAlert(0,...)`(3-인자), `$*`(`_anmH`/`FEV_PUSH`)는 `OnPush(...)`로 서로 다른 함수에 도달해 **최소 이 계층에서는 별개 채널임이 확인**됐으나, 더 아래(`CGuard`) 레벨에서 합류하는지는 여전히 미확인.

### 8.11.4. `FlashGrid`/`FlashSemi` — `vals[2]` 마커와 무관한, 훨씬 흔한 4번째 그리드 실시간 경로 (2026-08-19 신규 발견)

**위 3종(`$?`/`$$`/`$*`)은 전부 `vals[2]`(빌더의 "Variant" 속성)에 값이 있어야만 작동한다.** 하지만 실측(`IB211500`, 거래량순위 화면, `208300` 그리드 필드)에서 **`vals[2]`가 완전히 비어있는(`NOVALUE`) 그리드도 실시간으로 갱신되는 것**이 확인됨 — 종목코드가 일치하는 **기존 행을 찾아 그 행의 컬럼들만 고쳐쓰고, 행을 새로 삽입/삭제하지는 않는** 방식. `vals[2]`와 무관하므로 4절/8.11절의 마커 표에는 애초에 안 잡히는, 별도의 경로다.

**핵심은 `CScreen::OnAlert`(805행)의 `switch (form->m_form->kind)` 분기 자체다** — `FM_GRID`/`FM_TABLE`은 `vals[2]` 마커(`m_notice`/`m_sales`/`m_push`)와 전혀 상관없이, **그 그리드 필드 자체가 `FA_FLASH` 속성으로 `m_flashObs`에 등록만 되어 있으면** 매 틱마다 무조건 호출된다:

```cpp
// Screen.cpp:822, CScreen::OnAlert
switch (form->m_form->kind)
{
case FM_GRID:
    if (FlashGrid(key, code, fms))  flash = true;   break;
case FM_TABLE:
    if (FlashSemi(key, fms, code))  flash = true;   break;
case FM_CONTROL: ...
default:                                             // 단일 필드(FA_FLASH) 매칭 — 8절 요약, RealtimeCodeIndex_Investigation.md
```

`CScreen::FlashGrid(int key, CString code, CdataSet *fms)`(Screen.cpp:994)의 동작:

1. `form->m_form->checked`(`builder/h/form.h`엔 "default selected index"로 정의돼 있으나 그리드에서는 **종목코드가 들어있는 컬럼의 인덱스**로 재활용됨 — `PR_CODE`가 코드조회 팝업용으로, `FA_FLASH`가 단일필드 RTM 매칭용으로 재활용됐던 것과 같은 패턴, `RealtimeCodeIndex_Investigation.md` 참고)를 기준으로 **그리드의 모든 행을 순회**(`for (ii = 0; ii < nRows; ii++)`)하며 그 컬럼의 값을 `ReadData`로 읽어 `code`와 `text.Compare` 비교.
2. 일치하는 행을 찾으면, `FA_FLASH` 속성이 붙은 컬럼들만(전체 컬럼이 아니라) `fms`(이번 틱의 필드셋)에서 값을 찾아 `isFlash()`로 "실제로 값이 바뀌었는지" 확인한 뒤 `form->WriteData(string, true, idx, ii)`로 그 셀만 갱신.
3. `m_guard->m_status & WS_FCB`(깜빡임 효과 켜짐)면 `SetBkColor(m_guard->m_flash, ...)`로 배경색을 잠깐 바꾸고 `m_revObs`에 원래 색+시각을 저장해뒀다가 나중에 되돌리는 "플래시" 효과까지 처리.
4. **행 삽입/삭제는 전혀 없음** — 오직 `WriteData`(셀 갱신)뿐. `FlashSemi`(`FM_TABLE`용, 1074행)도 구조는 동일(코드 매칭 → 매칭된 셀만 갱신)하되 그리드가 아니라 `_cellR` 배열 기반의 고정 테이블이라는 차이만 있음.

**이번 로그(`IB211500`)와 대조:** 화면을 열 때 `SetDataOOP`가 `key=5 name=208300 kind=11 GRID textLen=3586`로 **전체 22행을 한 번에 스냅샷 로드**(8.14절과 동일한 OOP 그리드 로딩 경로 — 신규 발견 아님, 8.12/8.14절 메커니즘 재사용)한 뒤, 이후 시세가 바뀔 때마다 위 `FlashGrid`가 종목코드로 행을 찾아 등락률/현재가 등 해당 컬럼만 고쳐쓰는 것으로 설명된다. `$?`(체결그리드, 8.11.1절)와 정확히 대비되는 지점 — **같은 "그리드에 RTM이 온다"는 현상이 화면(맵)마다 전혀 다른 두 메커니즘 중 하나로 구현되어 있고, 그 둘을 가르는 유일한 스위치가 `vals[2]` 유무다.**

| | `$?`/`$$`/`$*` (8.11.1~3절) | `FlashGrid`/`FlashSemi` (이 절) |
|---|---|---|
| 트리거 조건 | `vals[2]`에 마커 문자열 존재 | 그리드 필드 자체의 `FA_FLASH` 속성 (`vals[2]`와 무관) |
| 저장 위치 | `m_notice`/`m_sales`/`m_push` (화면당 최대 1개씩) | `m_flashObs`에 등록된 모든 `FM_GRID`/`FM_TABLE` (화면당 여러 개 가능) |
| 행 단위 동작 | 삽입(`$?`)/upsert-or-삭제(`$$`)/원형버퍼 덮어쓰기(`$*`) | **기존 행 검색 후 셀만 갱신, 행 자체는 불변** |
| 코드 매칭 컬럼 지정 | (해당없음, 행 전체를 다루므로 컬럼 매칭 개념 없음) | `form->m_form->checked` (코드 컬럼 인덱스) |
| 실측 화면 | `IB999988`(체결, `$?`) | `IB211500`(거래량순위, `208300` 필드, 마커 없음) |

**참고 — 로그가 "짧아 보이는" 것에 대한 오해 해소:** `[5-SetDataOOP-write] key=5 name=208300 kind=11 GRID textLen=3586 preview=[M.A052690	1	052690	 한전기술	+10]`에서 `textLen=3586`이 실제 전체 크기(22행×컬럼 전부)이고, `preview=[...]`는 `axlog`의 `%.64s` 포맷 때문에 **첫 64글자만 화면에 찍은 것**이다(8.14절의 `getDataOOP`/`SetDataOOP` 로그 전부 동일한 `%.64s` 관례 — `Stream.cpp:2009`). `form->WriteAll(text)`에는 3586바이트 전체가 그대로 넘어가므로 데이터 유실은 없다 — 로그 한 줄의 미리보기 길이 제한일 뿐이다.

---

## 8.12. Wizard 자체 OOP 왕복 — `GetDataOOP`/`GetDataOOP2`(요청) + `SetDataOOP`(응답) (2026-08-18 확인)

### 배경 — 8.10절(IBKSConnector)과는 별개인, 두 번째 OOP 구현

8.10절은 **`HTS_OpenAPI(운영)/IBKSConnector`**(별도 OCX, `Wizard/` 바깥)가 OOP TR 요청을 조립하는 문법이었다. 이번에 확인한 건 **`Wizard/Stream.cpp` 안에 있는, IBKSConnector와 완전히 별개인 Wizard 자체의 OOP 요청/응답 구현**이다 — `GetDataOOP`(Stream.cpp:822)/`GetDataOOP2`(Stream.cpp:1092)가 요청을, `SetDataOOP`(Stream.cpp:1900)가 응답을 담당한다. 즉 이 프로토콜을 말하는 클라이언트가 최소 두 종류(IBKSConnector, Wizard 자신) 있다는 뜻이고, 둘 다 서버 입장에서는 같은 `US_OOP` 계열 TR을 주고받는 것으로 보이나 클라이언트 쪽 조립 코드는 중복 구현이다.

**어떤 `.map`이 이 경로를 타는가:** `msgK`는 여전히 `msgK_AXIS`(0x20) 그대로다 — **별도 msgK 값이 아니라, 맵 빌드 옵션 `OP_OOP`/`OP_OOP2`(`h/mapform.h:124,140`)가 켜진 화면만 `MakeStream`이 `GetDataOOP`/`GetDataOOP2`로 요청을 만들고, 그 요청에 `axisH->auxs |= auxsOOP` 비트를 실어 보낸다** (Stream.cpp:1436-1443). 수신측은 `OutStream`의 `msgK_AXIS` 분기에서 이 `auxsOOP` 비트만 보고 `SetDataOOP` vs `SetDataNRM`/`SetDataNRM2`로 갈라진다(Stream.cpp:164-165):
```cpp
case msgK_AXIS:
    if (axisH->auxs & auxsOOP)      // symbol data
        SetDataOOP(screen, datB, datL);
    else { ... SetDataNRM/NRM2 ... }
```

### 핵심 발견 — 요청과 응답이 같은 순서표(`m_ioR`)를 공유해서 정렬을 보장한다 (사용자 가설 확인+정정)

**질문하신 가설("송신할 때 탭으로 붙여서 올린 심볼들의 값을 그 순서에 맞게 다시 탭으로 붙여서 내려주지 않았을까")은 골격은 정확히 맞고, 다만 그 "순서"가 어디서 오는지는 조금 다르다.** 클라이언트가 요청을 만들 때 즉석에서 정한 순서가 아니라, **`.map` 빌드 시점에 이미 고정된 필드 순서표 `screen->m_ioR[]`를, 요청 조립(`GetDataOOP`)과 응답 파싱(`SetDataOOP`) 양쪽이 똑같이 `for (ii = 0; ii < screen->m_ioL; ii++)` 루프로 순회**한다(둘 다 이 정확한 형태의 루프를 씀, Stream.cpp:837/1107/1915 대조 확인). 서버는 요청에서 "이 이름의 필드를 원한다"는 선언만 보고, 응답 페이로드에는 **필드 이름 없이 값만 탭으로 나열**해서 돌려준다 — 클라이언트가 그 값을 다시 같은 `m_ioR` 순서로 하나씩 소비하며 제자리를 찾는 구조다. 즉 "정렬이 맞는 이유"는 서버가 순서를 지켜서 보내주기 때문이 아니라(그건 전제일 뿐), **클라이언트 쪽 요청 조립과 응답 파싱이 같은 순서표를 공유하도록 설계되어 있기 때문**이다 — 이 순서표(`.map`의 필드 정의 순서)가 새 플랫폼에서도 요청/응답 양쪽에 반드시 동일하게 보존돼야 한다는 뜻.

### 요청 조립 — `GetDataOOP`(OOP1)/`GetDataOOP2`(OOP2)

`m_ioR[]` 순회 중 필드 종류(`kind`)/입출력속성(`iok`)별로 텍스트를 이어붙인다. OOP1과 OOP2는 **필드명-값 구분자만 다르고 골격은 동일**:

| 필드 종류 | `iok` | 조립 형태 (OOP1, `FS`=`0x7f`) | 조립 형태 (OOP2, `iFS`=`'='`/`uFS`=`'@'`) |
|---|---|---|---|
| `FM_EDIT`/`FM_OUT`/일반 | `EIO_INPUT` | `이름` + `FS` + `값` + `\t` | `이름` + `iFS`(`=`) + `값` + `\t` |
| 〃 | `EIO_INOUT` | `이름`+`FS`+`값`+`\t` 뒤에 `이름`+`\t` 한 번 더(값요청도 겸함) | `이름`+`uFS`(`@`)+`값`+`\t` 뒤에 `이름`+`\t` 한 번 더 |
| 〃 | `EIO_OUTPUT` | `이름` + `\t` (값 없이 이름만 — "이 필드 값을 응답으로 달라"는 선언) | 동일 |
| `FM_MEMO`/`FM_BROWSER` | 위와 동일하되 값 앞에 `%05d` 형식 5자리 길이 프리픽스(`L_FILED`) 추가 | | |
| `FM_TABLE` | - | 컬럼명들을 `\t`로만 나열(값 없음, 마커도 없음) | |
| `FM_GRID` | - | `$` + `이름` + `FS` + `GetEnum(...,99)` 결과(연속조회 상태 블롭, 아래 참고) + 컬럼별 `이름`(+INPUT/INOUT이면 행수만큼 `FS`+값 반복)+`\n` ... + `\t` | `이름` + `$` (마커 위치가 이름 **뒤**로 바뀜) + 나머지는 유사 |
| `FM_CONTROL`(`FA_ENUM`) | - | `$` + `이름` + `FS` + 값 + `\t` | 동일 |
| `FM_OBJECT` | - | 재귀 호출(서브맵도 같은 규칙) | 동일 |

**8.10절(IBKSConnector)과 겹치는 부분:** 필드명-값 구분자 `FS = 0x7f`가 **IBKSConnector가 쓰던 `\x7f`(DEL)와 정확히 같은 값**이고(Stream.h:13), 그리드 마커도 똑같이 `$`를 쓴다 — 서버가 기대하는 와이어 바이트 자체는 두 클라이언트 구현이 동일한 계약을 따르고 있다는 근거. 다만 IBKSConnector는 그리드 페이징 정보를 별도의 이진 구조체(`grid_i`, 8.10절)로 붙이는 반면, Wizard 자체 구현은 그런 고정 구조체가 안 보이고 `form->GetEnum(text, formL, 99)`이라는, 폼 컨트롤 자체에 저장된 opaque 블롭(추정: 연속조회/스크롤 상태 — 정확한 내부 포맷은 `CfmGrid::GetEnum`/`SetEnum` 쪽 소스라 이번 조사 범위 밖, 미확인)을 그대로 읽어 붙인다는 점이 다르다.

### 응답 파싱 — `SetDataOOP` (Stream.cpp:1900)

`SetDataH`(8.7절, 원장 헤더)로 시작하는 것까지 `SetDataNRM`과 동일. 이후 `m_ioR[]`를 순회하며 **필드 종류별로 다른 규칙**:

| 필드 종류 | 파싱 방식 |
|---|---|
| `FM_EDIT`/`FM_COMBO`/`FM_MEMO`/`FM_BUTTON`/`FM_BROWSER`/`FM_OUT` | **탭(`\t`) 구분** — `text.Find('\t')`까지를 그 필드의 값으로 소비. `iok`가 `EIO_OUTPUT`/`EIO_INOUT`인 필드만 값을 받고(`EIO_INPUT`은 건너뜀), `FA_SKIP` 필드도 건너뜀 |
| `FM_GRID` | `form->GetEnum(dats, pos, 98)`/`SetEnum(text, pos, 99)`로 opaque 블롭을 먼저 소비(요청 때 보낸 98/99 슬롯과 대칭 — 정확한 의미는 미확인) → 이후 데이터는 **OOP2면 `gFS`(0x1b, ESC) 한 바이트, OOP1이면 `"\r\t"` 두 바이트**를 찾아 그 앞까지를 그리드 블롭으로 통째로 `form->WriteAll(text)`에 전달 — **`SetCells`/`SetTable`을 전혀 안 탐**(8.6절과 무관한, 그리드 컨트롤 자체가 내부 파싱하는 완전히 다른 경로) |
| `FM_TABLE` | `screen->m_cellR[...]`의 컬럼 정의를 순회하며 `\t` 구분으로 값을 모아 `form->WriteAll(dats)` |
| `FM_CONTROL`(`FA_ENUM`) | `FM_GRID`와 동일하게 `gFS`/`"\r\t"` 구분자 사용, 아니면 일반 필드처럼 `\t` 구분 |
| `FM_OBJECT` | 재귀 호출(서브맵) |

**OOP1 vs OOP2 요약(전 구간):**

| 구분자 역할 | OOP1 (`OP_OOP`만) | OOP2 (`OP_OOP2`) |
|---|---|---|
| 요청 필드명-값 (INPUT) | `FS` = `0x7f` | `iFS` = `'='` |
| 요청 필드명-값 (INOUT) | `FS` = `0x7f` | `uFS` = `'@'` |
| 그리드 마커 위치 | `$이름` (마커가 앞) | `이름$` (마커가 뒤) |
| 응답 그리드/ENUM 블롭 종결자 | `"\r\t"` (2바이트) | `gFS` = `0x1b` (1바이트) |

**새 플랫폼 설계 시사점:**
1. 새 플랫폼은 `.map` 빌드 옵션(`OP_OOP`/`OP_OOP2`)을 반드시 이관해야 하고, 요청 조립기와 응답 파서가 **동일한 필드 순서표**를 참조하도록 설계해야 한다 — 순서표가 요청/응답 양쪽에서 어긋나면 값이 다른 필드로 밀려 들어가는, IB204200 그리드 오염 사례(고정폭 `SetCells`, 8.6절)와 같은 유형의 정렬 버그가 OOP 경로에서도 똑같이 발생할 수 있다.
2. OOP1/OOP2는 **바이트 값 하나(구분자 문자)가 프로토콜 버전을 가르는 유일한 차이**다 — 새 플랫폼은 이 두 프로토콜을 완전히 별개로 재구현하기보다, 구분자 상수 4개(`FS`/`iFS`/`uFS`/`gFS`)를 옵션값으로 뽑아내는 하나의 파서로 통합하는 게 자연스럽다.
3. `GetEnum(...,98)`/`SetEnum(...,99)` 슬롯이 정확히 뭘 담는지(추정: 그리드 연속조회 상태, IBKSConnector의 `grid_i.save`와 유사한 역할일 가능성)는 이번 조사로 확정하지 못했다 — **다음 조사 대상**(9절에 반영).

### 실측 검증 — 단일값(비그리드) OOP 요청/응답 왕복 1건 전체 (2026-08-19, `IB999986`/`pooppoop`)

axlog 실측 캡처(`NOENC.TXT` 평문 모드)로 위 규칙들을 소스 분석이 아니라 실제 와이어 바이트로 재확인:

```
[0-Write-send]  msgK=32 datL=65  preview=[1301005930	1301	17772	2023	2302	2025	2026	2303	1377	2320	12374	]
CGuard::Write(2) len=89 hdr0=0x20 hdr3=0x28                    <- 89 = L_axisH(24) + datL(65), 정확히 일치
[1-OnAxis-raw]  msgK=32 auxs=16 nBytes=54                      <- auxs=16=auxsOOP, 응답도 요청과 같은 비트로 옴
[4-OutStream-parse] preview=[005930	273500	355500	-0	-0	191500	1	273500	5846278608	]
```

**확인된 것:**
- `auxsOOP`(0x10)이 요청/응답 양쪽에 동일하게 실림 — 요청측 `axisH->auxs |= auxsOOP`(`GetDataOOP` 호출 게이트)와 수신측 `OutStream`의 `auxsOOP` 분기 조건이 실제로 대칭임을 확인
- 요청 페이로드에서 `1301`(종목코드)이 **값 있는 토큰 1개 + 값 없는 토큰 1개, 총 두 번** 나옴 — 위 요청 조립 표의 `EIO_INOUT`(값 방출 후 `EIO_OUTPUT`으로 fallthrough) 규칙이 실제 와이어에 정확히 그대로 나타남을 실측으로 재확인
- `CGuard::Write`의 `len`이 `L_axisH + datL`과 정확히 일치 — 헤더/페이로드 경계 계산이 실측과 어긋나지 않음(8.8절 규칙과 정합)

**정정(2026-08-19) — "필드 10개인데 응답 9개" 불일치는 처음 판독이 틀렸던 것, 실제로는 개수가 정확히 맞는다:**

요청 미리보기 `1301005930	1301	17772	2023	...`를 육안으로 다시 보면 `17772`가 필드코드 하나처럼 보이지만, **`\x7f`(DEL, 인쇄 불가 문자)가 안 보이는 채로 중간에 끼어있는 것**이다 — `1301005930`이 `1301`+`\x7f`+`005930`이었던 것과 완전히 같은 패턴으로, `17772` = `1777`+`\x7f`+`2` (필드코드 `1777`에 값 `2`를 실은 것)다. 즉 요청은:

| 순번 | 원문 토큰 | 실제 의미 |
|---|---|---|
| 1 | `1301` + `\x7f` + `005930` | `1301`(종목코드, `EIO_INOUT`) — 입력값 |
| 2 | `1301` | `1301`의 fallthrough 재방출 — 출력요청 |
| 3 | `1777` + `\x7f` + `2` | `1777`(시장구분 등으로 추정, `EIO_INPUT` 전용) — 입력값. **§8.10의 TR1007 예시에 나온 그 `1777`+`\x7f`+`mkgubn` 필드와 동일 필드코드** — OOP 계열 여러 TR이 공유하는 파라미터로 보임 |
| 4~11 | `2023`/`2302`/`2025`/`2026`/`2303`/`1377`/`2320`/`12374` | 값 없는 순수 `EIO_OUTPUT` 필드 8개 |

**`1777`은 `EIO_INPUT` 전용이라 fallthrough가 없다** — `1301`(INOUT)처럼 "이름+값" 다음에 "이름만" 재방출되지 않고 딱 한 번만 나온다. 그리고 `SetDataOOP`의 응답 파싱 로직(위 "응답 파싱" 표)은 `iok`가 `EIO_OUTPUT`/`EIO_INOUT`인 필드만 응답값을 소비하고 순수 `EIO_INPUT`은 `key++; continue;`로 완전히 건너뛴다 — 즉 **`1777`은 응답값을 아예 받지 않는 필드**다. 그래서 실제 값을 주고받는 필드는 `1301`(1개) + 출력전용 8개 = **9개**이고, 응답의 `\t` 9개와 정확히 일치한다. `datL=54`도 이 9개 값 그대로 설명됨 — 미스터리가 아니라 처음 판독에서 `\x7f`를 하나 놓친 것이었다.

**교훈:** OOP 페이로드를 로그 미리보기(preview)만으로 읽을 때는, 숫자로만 이루어진 토큰이 나오면 항상 "이 안에 `\x7f`가 숨어있을 수 있다"를 의심해야 한다 — `\x7f`는 화면에 아무 자국도 안 남기기 때문에 `1777`+`\x7f`+`2`와 순수 필드코드 `17772`가 육안으로는 구분이 안 된다. 확실히 구분하려면 로그 자체에 구분자를 표시하거나(`\x7f`를 다른 가시문자로 치환해서 출력), `SetDataOOP`처럼 소스의 정확한 `m_ioR` 순회 규칙과 대조해서 세어야 한다.

**추가 조치:** 향후 이런 로그를 필드 단위로 바로 확인할 수 있도록, `SetDataNRM`의 `[5-SetDataNRM-write]`(8.5절)와 같은 패턴으로 **`SetDataOOP`의 4개 필드-write 지점(FM_OUT류/FM_GRID/FM_TABLE/FM_CONTROL)에 `[5-SetDataOOP-write]` axlog를 추가함**(`Stream.cpp`, 2026-08-19). 다음에 OOP 로그를 캡처하면 각 필드의 `key`/`name`/`kind`/`iok`/실제 값이 한 줄씩 그대로 찍혀서, 이번처럼 수동으로 `\x7f` 위치를 되짚을 필요가 없어짐.

---

## 8.13. `SetDataNRM2`/`SetDataTAB2` — 사용자 컬럼 커스터마이즈 변형 (2026-08-18 확인)

### 왜 "2"가 붙은 버전이 따로 있는가

8.5절에서 이미 짚었듯, `OutStream`은 화면에 사용자 커스터마이즈 상태(`screen->m_outs.GetSize() > 0`)가 있으면 `SetDataNRM` 대신 `SetDataNRM2`를, `SetDataTAB` 대신 `SetDataTAB2`를 부른다. **"커스터마이즈"란 사용자가 그리드/화면의 컬럼 순서를 바꾸거나 특정 컬럼을 숨긴 상태**를 말하며, 이 상태가 `screen->m_outs`(타입: `CtranItem` 배열)에 저장돼 있다.

### `m_ioR[]` 대신 `m_outs[]`를 순회 — 그리고 `NULL` 항목이라는 새로운 케이스

`SetDataNRM`/`SetDataTAB`이 `.map` 빌드 시 고정된 필드 순서표 `screen->m_ioR[]`를 순회하는 것과 달리, `SetDataNRM2`/`SetDataTAB2`는 **사용자가 재배열한 순서표 `screen->m_outs[]`**를 순회한다. 각 항목(`CtranItem`)은 `m_form`(대상 컨트롤, 또는 **`NULL`**)·`m_size`(그 필드가 소비할 바이트 수 — `.map`의 고정 크기가 아니라 **커스터마이즈된 크기**일 수 있음)·`m_col`(그리드/테이블 컬럼 인덱스)을 담는다.

```cpp
item = (CtranItem *)screen->m_outs.GetAt(key);
form = item->m_form;
if (ParseCC2(screen, &axisB[idx], form, idx))   // FCC/RCC/SCC는 이미 해석된 form 포인터를 바로 씀 (ParseCC처럼 다시 찾지 않음)
    continue;
if (form == NULL)              // ★ 사용자가 숨긴 컬럼 — 서버는 바이트를 여전히 보내지만 어디에도 안 씀
{
    idx += item->m_size;       // (NRM2) 고정폭만큼 그냥 건너뜀
    // 또는 (TAB2) 다음 '\t'까지 찾아서 건너뜀
    key++; continue;
}
```

**이건 8.6절에서 이미 확인한 `SetCells2`/`SetTable2`의 "숨긴 컬럼도 바이트는 소비" 규칙이, 그리드 안쪽뿐 아니라 화면 전체 필드 레벨에서도 똑같이 적용된다는 뜻이다** — 사용자가 화면의 어떤 필드든 숨기면, 서버는 그 필드 데이터를 계속 보내고 클라이언트는 그 바이트만큼 커서를 밀어내되 아무 데도 쓰지 않는다. 서버는 사용자의 UI 커스터마이즈 상태를 전혀 모른다는 뜻이기도 하다 — 정렬/숨김은 순수 클라이언트 로컬 기능이고, 서버는 항상 `.map` 원본 필드 순서대로 전체 데이터를 보낸다.

### `SetDataNRM2` vs `SetDataNRM` — 필드별 처리 차이는 "크기 출처"뿐

나머지 필드 종류별 파싱 로직(EDIT/OUT 고정폭, MEMO/BROWSER 길이프리픽스, GRID의 `GO_HEADER`/`GO_FLEX`/`GO_TABLE` 분기, CONTROL 등)은 `SetDataNRM`과 사실상 동일하다. 유일한 실질적 차이는 EDIT/OUT류가 폭을 `form->m_form->size`(맵 고정값) 대신 `item->m_size`(커스터마이즈 반영값)에서 가져온다는 것, 그리고 그리드 파싱을 `SetCells`/`SetTable`이 아니라 **`SetCells2`/`SetTable2`**(8.6절, 컬럼 매핑 인지 버전)로 위임한다는 것뿐이다. `FM_TABLE`도 `item->m_col`로 지정된 정확한 컬럼 하나에만 `WriteData(text, true, -1, item->m_col)`를 호출 — `SetDataNRM`이 컬럼을 순서대로 쭉 이어붙이는 것과 달리 컬럼 단위로 정확히 타겟팅한다.

### `SetDataTAB2` vs `SetDataTAB` — 그리드 처리가 다르다 (신규 발견)

필드 구분자(`\t`)가 값의 경계라는 기본 문법은 동일하지만, **`FM_GRID` 케이스가 다르다.** `SetDataTAB`(8.5절 인용 코드)은 그리드 데이터를 통째로 `form->WriteAll(text)`에 넘기고 `SetCells` 호출은 명시적으로 주석처리돼 있었다(2026-07-26 확인 사항). 반면 **`SetDataTAB2`는 `WriteAll(text)` 호출 전에 `SetCells2(screen, form, text)`를 먼저 호출한다:**
```cpp
pos = text.Find('\r');
if (pos != -1) text = text.Left(pos++); else pos = text.GetLength();
SetCells2(screen, form, text);   // ★ TAB(v1)엔 없던 호출
form->WriteAll(text);
```
즉 **컬럼이 사용자에 의해 숨겨졌거나 재배열된 그리드는, TAB 포맷이라도 `SetCells2`가 먼저 컬럼 매핑을 반영한 뒤에야 그리드 컨트롤에 넘어간다** — v1(커스터마이즈 없음)에서는 그리드 컨트롤 스스로 탭을 쪼개 처리했지만, v2(커스터마이즈 있음)는 `SetCells2`가 컬럼 재배열/숨김을 먼저 개입시켜야 하므로 이 추가 호출이 필요해진 것으로 보인다.

**새 플랫폼 설계 시사점:** "사용자가 컬럼을 재배열/숨겼는가"는 서버와 무관한 순수 클라이언트 상태이므로, 새 플랫폼은 이 상태를 서버 프로토콜과 완전히 분리된 **클라이언트 로컬 레이어**(수신한 원본 필드 스트림을 항상 `.map` 순서대로 먼저 파싱한 뒤, 그 결과를 사용자 컬럼 설정에 따라 재배열/필터링하는 2단계 구조)로 구현하는 게, 지금처럼 파서 함수 자체를 이중화(`NRM`/`NRM2`, `TAB`/`TAB2`, `Cells`/`Cells2`, `Table`/`Table2`)하는 것보다 유지보수에 유리할 것으로 보인다.

---

## 8.14. 그리드 필드 헤더 — `_gridHi`/`_gridHo` (`h/Grid.h`, 2026-08-19 확인)

### 배경 — 9절 "GetEnum/SetEnum 슬롯 98/99" 미해결 항목 해소

8.12절 요청 조립 표에서 `FM_GRID`(OOP1) 행에 적어뒀던 "`GetEnum(...,99)` 결과(연속조회 상태 블롭 — 정확한 내부 포맷은 `CfmGrid::GetEnum`/`SetEnum` 쪽 소스라 미확인)"가 이번에 실제 캡처 로그(`IB999988`, 체결그리드)와 `dll/form/fmGridx.cpp`의 `getInformation()`/`setInformation()` 구현을 대조해서 완전히 풀렸다. opaque가 아니라 `h/Grid.h`에 정의된 고정 레이아웃 구조체였다.

### 두 구조체 — 요청은 `_gridHi`(110바이트), 응답은 `_gridHo`(103바이트)

`h/Grid.h`(`#pragma pack(1)`, 전부 아스키 텍스트 필드 — 진짜 이진수가 아니라 자릿수 고정 문자열):

```cpp
// INBOUND format (클라이언트 -> 서버, 요청)
struct	_gridHi	{
	char	visible[2];		// 화면에 보이는 행수
	char	rows[4];		// 최대 행수(버퍼 크기)
	char	type;			// '0'=fixed GRID, '1'=flexible GRID(GO_FLEX)
	char	dir;			// 실시간 갱신 삽입방향: '1'=top, '2'=bottom
	char	sort;			// '0'=disable, '1'=ascending, '2'=descending
	char	symbol[16];		// 정렬기준 컬럼명
	char	key;			// 이벤트키: '0'=enter '1'=pgup '2'=pgdn '3'=scrup '4'=scrdn '5'=first/end '6'=sortcolumn
	char	page[4];
	char	save[80];		// 연속조회 토큰(직전 응답의 save를 그대로 echo)
};					// 2+4+1+1+1+16+1+4+80 = 110바이트

// OUTBOUND format (서버 -> 클라이언트, 응답)
struct	_gridHo	{
	char	mode;			// '0'=update, '1'=insert top, '2'=append tail
	char	sort;			// '0'/'1'/'2' (요청과 동일 의미)
	char	symbol[16];
	char	stat;			// bit OR: statPREV=0x01, statNEXT=0x02, statSORT=0x04
	char	page[4];
	char	save[80];		// 다음 연속조회 시 그대로 되돌려보낼 토큰
};					// 1+1+16+1+4+80 = 103바이트
```

`IBKSConnector`(8.10절)의 `grid_i`/`grid_o`와 필드 구성이 사실상 동형(가시/최대행수, 정렬방향/컬럼, 연속조회키, `save` 토큰)이지만 **바이트 배치와 크기가 다른 별개 구조체**다 — Wizard 자체 OOP 구현(8.12절)이 IBKSConnector 코드를 그대로 재사용한 게 아니라 독자적으로 다시 구현한 것이라는 근거가 하나 더 늘었다.

### 어디서 만들고 어디서 소비하는가

`CfmGrid`(`dll/form/fmGridx.cpp`)의 `GetEnum`/`SetEnum`이 `func` 인자로 두 구조체를 왕복시킨다:

| 호출 | 위치 | 동작 |
|---|---|---|
| `form->GetEnum(text, formL, 99)` | 요청 조립측(`GetDataNRM`/`GetDataNRM2`/`GetDataOOP`/`GetDataOOP2`) | `CfmGrid::getInformation(text)` 호출 — 현재 그리드 상태로 `_gridHi` 110바이트 문자열을 즉석 조립(`fmGridx.cpp:5928`, `sprintf`형 `Format("%02d%04d%d%d%d%-16s%d%04d%-80s", ...)`) |
| `form->GetEnum(dats, pos, 98)` | 응답 파싱측(`SetDataNRM`/`SetDataNRM2`/`SetDataTAB`/`SetDataTAB2`/`SetDataOOP`) | `case 98: value = sizeof(_gridHo);` — 텍스트가 아니라 **크기(103)만** 돌려줌(`fmGridx.cpp:6609`) |
| `form->SetEnum(text, pos, 99)` | 응답 파싱측, 위 98 직후 | `CfmGrid::setInformation(text)` 호출 — 방금 얻은 크기(103)만큼 `text` 앞부분을 `{1,1,16,1,4,80}` 순서로 잘라 `mode`(→`_SetDataPosition`, 그리기 위치 결정)/`sort`/`symbol`(정렬컬럼 찾아 `m_sortc` 갱신)/`stat`/`page`/`save`(→`m_inits`, 다음 연속조회용) 순서로 반영(`fmGridx.cpp:5855`) |

응답측 파싱(예: `SetDataOOP`, Stream.cpp:1988-1993)은 `GetEnum(...,98)`으로 **크기만 미리 물어본 뒤** 그만큼을 `SetEnum(...,99)`에 통째로 넘기는 2단계 호출 — "몇 바이트인지 먼저 확인 후 그만큼 소비"하는 패턴이다.

### 실측 검증 — `IB999988`(체결그리드) 요청 헤더 바이트 1:1 대조 (2026-08-19)

이번에 캡처하신 `[0-Write-send]` 요청 원문 중 그리드 필드(`11310`) 구간:

```
$11310\x7f 110200110 [16칸 공백] 00000 [80칸 공백]
```

`_gridHi`(110바이트) 필드 순서대로 쪼개면 한 글자도 안 어긋나고 정확히 들어맞는다:

| 바이트 | 값 | 필드 | 의미 |
|---|---|---|---|
| 1-2 | `11` | `visible` | 화면에 11행이 보임 |
| 3-6 | `0200` | `rows` | 최대 200행 버퍼 |
| 7 | `1` | `type` | flexible grid(`GO_FLEX`) |
| 8 | `1` | `dir` | 실시간 갱신 시 **top(위쪽) 삽입** |
| 9 | `0` | `sort` | 정렬 비활성 |
| 10-25 | (공백16) | `symbol` | 정렬 기준 컬럼 없음 |
| 26 | `0` | `key` | enter(최초 조회) |
| 27-30 | `0000` | `page` | 첫 페이지 |
| 31-110 | (공백80) | `save` | 연속조회 토큰 없음(최초 요청이라 아직 없음) |

`dir=1`(top 삽입)이 바로 사용자님이 말씀하신 "체결그리드라서 실시간으로 위에 row가 insert 됩니다"와 맞아떨어진다 — 다만 이건 **이 OOP 요청 헤더에 실린 값일 뿐**이고, 실제 실시간 삽입 동작 자체는 이 TR 왕복과는 다른 채널(RTM, 8.11절)에서 일어난다는 점은 마지막 소절에서 따로 짚는다.

**추가 실측 — `save` 토큰 왕복 확인 (2026-08-19, 같은 화면 재조회 캡처):** 위 표는 화면을 막 열었을 때의 최초 조회라 `save`가 전부 공백이었다. 같은 `IB999988`을 다시 조회한 캡처에서는 `$11310` 뒤 `_gridHi`의 `save` 자리에 `A005930...`으로 시작하는 내용이 채워져 있었다 — 직전 응답의 `_gridHo.save`가 `CfmGrid::setInformation()`에 의해 `m_inits` 멤버에 저장됐다가(`fmGridx.cpp:5882`), 다음 요청 조립 시 `getInformation()`이 그 `m_inits`를 그대로 다시 꺼내 쓰는 것(`fmGridx.cpp:5939`)으로 확인된다 — "어디서 만들고 어디서 소비하는가" 표에서 설명한 요청↔응답 `save` 왕복이 이론이 아니라 실제로 도는 것을 캡처로 검증한 셈이다. 다만 그 안에 담긴 값 자체(예: `A005930`이 정확히 무엇을 인코딩하는지)의 서버측 의미는 클라이언트 소스만으로는 알 수 없다 — 마지막으로 받은 레코드의 종목코드/시각/거래량 등을 실어 델타 조회 커서로 쓰는 것으로 추정되나 미확정.

**응답측 `_gridHo`도 바이트 단위로 실측 확인됨 (2026-08-19, `IB211500`/거래량순위):**

`[4-OutStream-parse]`가 찍는 원문 미리보기는 `Stream.cpp:93`에서 `previewL = (datL < 60) ? datL : 60`으로 **60바이트 캡**이다(`SetDataOOP`쪽 필드별 로그의 `%.64s`와는 다른 별개의 상한 — 헷갈리기 쉬우니 주의). 캡처된 60바이트:

```
0\t0\t1\t1\t22\t   000               F0001                          
```

앞의 `0\t0\t1\t1\t22\t`(11바이트)는 그리드 앞의 출력전용 필드 5개 값(`200301`~`200305`) — 뒤이은 `[5-SetDataOOP-write]` 로그의 `key=0~4` 값과 정확히 일치한다. 그 뒤 49바이트가 그리드 필드(`208300`)의 `_gridHo` 응답 헤더 앞부분이며, `_gridHo`(1+1+16+1+4+80) 순서로 정확히 쪼개진다:

| 바이트 | 값 | 필드 | 의미 |
|---|---|---|---|
| 1 | `0` | `mode` | update(행 삽입/삭제 없이 갱신) |
| 2 | `0` | `sort` | 정렬 비활성 |
| 3-18 | `0`+공백15 | `symbol` | `sort=0`이라 무의미(내용 자체는 참고용) |
| 19 | `F`(0x46) | `stat` | 이 필드만 아스키 숫자가 아니라 **원시 비트마스크 1바이트** — `0x46 = 0x40(기본/정상거래) + 0x04(statSORT) + 0x02(statNEXT)` |
| 20-23 | `0001` | `page` | 1페이지 (요청측 `_gridHi.page`는 0-베이스로 보였는데 응답은 1-베이스로 보임 — 확정은 아니고 추가 샘플 필요) |
| 24-49(부분) | 공백 | `save` | 80바이트 중 앞 26바이트만 미리보기에 잡힘(전부 공백으로 보이나 60바이트 상한 때문에 끝까지는 확인 불가) |

`stat` 필드가 다른 필드들(전부 `%0Nd` 형태의 아스키 숫자 문자열)과 달리 **원시 바이트 값**이라는 게 이번에 새로 확인된 점 — 파싱할 때 `atoi()`가 아니라 문자 하나를 그대로 비트 OR로 읽어야 한다(`h/Grid.h`의 `statPREV=0x01`/`statNEXT=0x02`/`statSORT=0x04` 정의와 대조).

### `GetDataOOP`(OOP1)의 FM_GRID 전체 문법 — 요청측 (코드 인용)

`Stream.cpp:962-1024`, 지금까지 8.12절엔 요약만 있었는데 실제 조립 순서를 코드로 그대로 옮기면:

```cpp
case FM_GRID:
    iosB[iosL++] = '$';
    CopyMemory(&iosB[iosL], symN, symL); iosL += symL;   // 필드명(예: 11310)
    iosB[iosL++] = FS;                                   // 0x7f

    form->GetEnum(text, formL, 99);                       // _gridHi 110바이트
    CopyMemory(&iosB[iosL], text, text.GetLength());
    iosL += text.GetLength();

    for (각 컬럼 cell)
    {
        CopyMemory(&iosB[iosL], symN, symL); iosL += symL;   // 컬럼명(예: 11034)
        switch (cell->iok)
        {
        case EIO_INPUT:
        case EIO_INOUT:
            for (각 화면 표시행)                              // vRows번 반복
            {
                iosB[iosL++] = FS;
                form->ReadData(text, false, -1, idx, kk);      // 그 셀의 현재값
                CopyMemory(&iosB[iosL], text, ...);
            }
            if (EIO_INOUT) { iosB[iosL++]='\n'; 컬럼명 재출력; }
            [[fallthrough]];
        default:                                           // EIO_OUTPUT은 이 default로 바로 떨어짐
            iosB[iosL++] = '\n';
            break;
        }
    }
    iosB[iosL++] = '\t';                                   // 그리드 필드 전체 종결
```

캡처된 요청의 컬럼 목록 부분(`11034\n2023\n11024\n11032\n2027\n`, 5개 컬럼 모두 순수 출력전용)이 정확히 위 `default` 분기(컬럼명+`\n`만 반복, 값 없음)와 일치한다. 8.12절 표에는 "컬럼명(+INPUT/INOUT이면 값 반복)+`\n`"로 요약만 돼 있었는데, 이번에 그 괄호 안 조건의 정확한 조립 순서(값을 몇 번 반복하는지, INOUT일 때만 컬럼명을 왜 두 번 쓰는지)까지 코드로 확정됐다.

### 이 헤더 메커니즘은 OOP 전용이 아니다 — `GO_HEADER`로 게이팅되는 범용 그리드 프로토콜

`GetEnum(...,99)`/`GetEnum(...,98)`+`SetEnum(...,99)` 호출 쌍은 `Stream.cpp`를 전수 검색해보면 **OOP 계열만이 아니라 `GetDataNRM`/`GetDataNRM2`/`SetDataNRM`/`SetDataNRM2`/`SetDataTAB`/`SetDataTAB2`의 `FM_GRID` 분기에도 전부 나온다.** 즉 `_gridHi`/`_gridHo` 헤더는 OOP 프로토콜의 특징이 아니라 **그리드가 있는 화면이면 어떤 송수신 방식이든 공통으로 쓰는 부속 프로토콜**이다.

한 가지 비대칭이 확인된다: `GetDataNRM`/`SetDataNRM`/`SetDataNRM2`/`SetDataTAB`/`SetDataTAB2`는 전부 `if (form->m_form->attr2 & GO_HEADER)`로 감싸서, **맵 빌드 시 그 그리드에 "헤더 포함" 속성이 켜져 있을 때만** 이 헤더를 넣거나 읽는다. 반면 `GetDataOOP`/`GetDataOOP2`/`SetDataOOP`의 `FM_GRID` 분기는 `GO_HEADER` 체크 없이 **항상** 헤더를 싣고 읽는다 — OOP 프로토콜을 쓰는 그리드는 무조건 이 헤더가 실린다는 뜻이고, 실측 캡처(`IB999988`)도 이와 일치한다.

### RTM 실시간 삽입 방향(`GO_TOP`)과의 관계 — 별개 메커니즘임을 주의

`_gridHi.dir`/`_gridHo.mode`가 "위/아래 삽입"을 표현하지만, 8.11.1절에서 이미 확인한 실시간 시세(RTM) 삽입 방향은 **완전히 다른 속성**(`form->m_form->attr2 & GO_TOP`, `CScreen::ScrollRTM`이 읽음)으로 결정된다. 둘 다 개념은 "새 행을 위/아래 어디에 넣을지"로 같지만:

- `GO_TOP`(맵 빌드 시 고정, 화면 열려있는 동안 절대 안 바뀜) → RTM 채널(`$?`/`$$`, 8.11절)이 읽음
- `_gridHi.dir`/`_gridHo.mode`(TR 요청/응답마다 실려오는 세션 상태값) → OOP/TR 그리드 헤더(이 절)가 다룸

이번 캡처에서 `dir=1`(top)과 사용자 관찰(체결이 위에 쌓임)이 우연히 같은 방향을 가리켰을 뿐, **이 TR 왕복 자체가 실시간 삽입을 일으키는 경로는 아니다** — 화면이 처음 열릴 때 이 OOP 조회로 스냅샷(현재 보이는 9~11행)을 채우고, 그 이후의 실시간 갱신은 8.11절의 RTM 마커 경로로 넘어간다고 보는 게 맞다. 두 메커니즘이 서로 다른 코드 경로임을 명확히 구분해야 새 플랫폼에서 "그리드 방향 하나만 구현하면 되겠지"라는 실수를 피할 수 있다.

**확정(2026-08-19):** `IB999988`의 `11310` 그리드 필드를 빌더에서 직접 확인한 결과 `vals[2]="$?"` — 즉 8.11.1절의 `m_sales`/`CScreen::ScrollRTM` 경로다(무조건 `InsertRow`, 기존 행 매칭 없음). 이걸로 이 화면의 전체 그림이 완성된다: **화면 오픈 시 이 절의 OOP 조회(`pooppoop`, `SetDataOOP`)가 스냅샷을 한 번 채우고, 이후 매 체결마다 8.11.1절의 RTM `ScrollRTM` 경로가 행을 통째로 위에 삽입(전체 row insert, 셀 단위 갱신 아님)**하는 구조 — "row 전체가 insert"라는 실측 관찰이 `$$`(키매칭 upsert, 8.11.2절)가 아니라 `$?`(무조건 삽입) 경로임을 그대로 뒷받침한다.

---

## 8.15. RCC 콤보 항목수(`ccl`) 필드값이 실제 전송 데이터보다 작음 — 서버측 데이터 버그로 확인 (2026-08-21)

### 배경

8.5절 "인밴드 제어코드" 절에서 다룬 `RCC`(0x1B, 드롭다운/트리 데이터 채우기)와 관련해, 실사용 화면에서 "콤보 드롭다운에 항목이 몇 개 안 들어간다"는 증상이 보고됨. `CStream::ParseRCC`(Stream.cpp:3025)의 `csCOMBO` 케이스에는 그동안 axlog가 전혀 없어서(`@docs/DebugLogGuide.md` 6절), `[ParseRCC]`/`[ParseRCC-combo]` 태그를 신규 추가(2026-08-21)한 뒤 실측 캡처로 원인을 완전히 규명함.

### 확인된 사실

`bo425401`(종합시세표/업종별시세 화면), 콤보 필드 `JNAM`(종목명) 조회 캡처(`datL=3758`) 기준:

1. **와이어 데이터에는 283개의 (종목코드+종목명) 쌍이 전부 정상적으로, 끝까지 안 빠지고 실려 있음** — `S0A`(TKG휴켐스)부터 `SM8`(TIGER 반도체TOP10)까지 순서대로 확인.
2. **그런데 RCC 헤더의 `ccl`(항목개수, `unsigned char`, `h/axis.h:296-301`) 필드값은 `27`.** axlog(`[ParseRCC-combo] name=JNAM ccl=27 textLen=375 ...`)와 원시 패킷 16진수 덤프(AxisChaser `Send_Rev.ini`)를 바이트 단위로 직접 대조해서 교차검증 완료:
   ```
   1B 01 4A 4E 41 4D 00×12 1B 53 30 41 54 4B 47 ...
   └rcc └ccs └────name[16]("JNAM"+패딩)────┘ └ccl  └"S0ATKG휴켐스..." (첫 항목)
   (=0x1B)  (=csCOMBO)                      (=0x1B=27, 10진수)
   ```
   `_RCC` 구조체 시작으로부터 정확히 오프셋 18(`rcc`+`ccs`+`name[16]` = 1+1+16 = 18바이트 뒤)에 위치한 `ccl` 바이트의 실제 값이 `0x1B`(=27, 10진수)로, axlog가 메모리에서 읽어 출력한 값과 정확히 일치 — 로그값이 아니라 서버가 소켓에 실제로 써서 보낸 값임이 원시 바이트로 확정됨.
3. `CStream::ParseRCC`는 설계대로 `ccl`(=27)개만 읽고 멈추므로, **27번째 항목(`S18`, 신한지주)까지만 콤보에 채워지고 나머지 256개(`S19`기아 이후 ~`SM8`까지)는 파싱되지 않고 버려짐** — 사용자가 실제 화면에서 관찰한 "신한지주까지만 보이고 그 뒤가 없다"는 증상과 정확히 일치.
4. RCC 헤더의 컨트롤명(`JNAM`)이 페이로드 전체에서 단 1회만 등장 — 즉 283개가 여러 RCC 블록으로 나뉘어 온 게 아니라 **하나의 RCC 블록 안에 283개 분량의 데이터가 통째로 실려 있는데, 그 블록 자신의 개수 필드에는 27이라는 별개의 숫자가 적혀 있는 상태.**

**참고(미확정 추정):** `ccl` 값(0x1B=27)이 공교롭게도 바로 앞의 `rcc` 마커 바이트(0x1B) 값과 동일함. 위치가 명확히 다른 별개 바이트라 클라이언트가 혼동해서 잘못 읽은 건 아니지만, 서버측 개수 계산 로직이 이 값을 어딘가에서 잘못 재사용하고 있을 가능성도 배제하지 못함 — 서버 코드 확인 없이는 우연인지 원인인지 판단 불가.

### 결론 — 클라이언트는 정상, 서버가 내려주는 `ccl` 값이 실제 데이터 개수와 불일치

`CStream::ParseRCC`는 프로토콜 설계대로 정확히 동작하고 있다 — `ccl`을 신뢰하고 그만큼만 읽는 구현 자체는 문제가 없다. 원인은 **서버(`bo425401` TR 혹은 그 백엔드 로직)가 `ccl` 필드에 실제 전송 항목 수(283)가 아닌 다른 값(27)을 계산해 넣고 있는 것**으로 판단됨 — **서버측 확인/수정이 필요한 사항**.

클라이언트 측에서 `ccl`을 무시하고 버퍼 끝까지 강제로 읽는 방어적 수정은 채택하지 않음 — `ccl`이 정확한 다른 정상 TR들에서는 다음 필드의 데이터를 콤보 항목으로 잘못 삼켜버리는 새로운 위험을 만들 수 있기 때문. 새 플랫폼에서 RCC 프로토콜을 재구현할 때도, **1바이트(`ccl`, 최대 255) 자체가 대규모 목록(수백 개 단위)을 안전하게 표현하기엔 근본적으로 좁다**는 점과, 서버가 실제 전송량과 다른 개수를 선언할 수 있다는 이번 사례를 함께 감안할 필요가 있음(예: 수신측에서 `ccl`을 다 소진한 뒤에도 버퍼에 더 읽을 수 있는 NUL-종단 문자열이 남아있으면 최소한 경고 로그를 남기는 정도의 방어는 검토 가치 있음).

### 관련 파일

| 파일 | 역할 |
|---|---|
| `Wizard/Stream.cpp:3025-3088` | `CStream::ParseRCC` — `csCOMBO` 처리, `[ParseRCC]`/`[ParseRCC-combo]` axlog 추가 지점(2026-08-21) |
| `h/axis.h:296-301` | `_RCC` 구조체 정의(`rcc`/`ccs`/`name[16]`/`ccl`) — `ccl`이 `unsigned char`라 최대 255가 물리적 한계 |
| `dll/form/fmCombo.cpp:774-846` | `CfmCombo::WriteAll` — RCC로 조립된 탭구분 문자열을 실제 콤보 항목(`m_datas`/`m_displays`)으로 분해 |
| `@docs/DebugLogGuide.md` 6절 | `[ParseRCC]`/`[ParseRCC-combo]` 태그 설명 |

---

## 9. 다음 조사 대상 (미완료)

- ~~`IB999986`(`pooppoop`) 단일값 OOP 조회의 요청/응답 필드 개수 불일치~~ — **확인 완료(2026-08-19), 8.12절 "실측 검증" 참고.** 처음 판독에서 `\x7f`(DEL) 하나를 놓쳐서 필드코드 하나(`1777`)를 못 보고 지나친 것이었고, 실제로는 개수가 정확히 맞음. 재발 방지로 `SetDataOOP`에 `[5-SetDataOOP-write]` axlog 4곳 추가함(`Stream.cpp`, 2026-08-19) — 다음 캡처부터는 필드 단위로 바로 확인 가능
- ~~`ParseSCC`/`SetCC`의 `CC_*` 플래그 전체 목록~~ — **확인 완료(2026-08-18), 8.5절 "인밴드 제어코드" 참고.** `CC_SET`/`CC_VIS`/`CC_PRO`/`CC_MAND`/`CC_SEND`/`CC_ENB` 6개 전부 비트값+효과 확정, FCC의 scc/rcc XOR 기반 SET/RESET 동시 인코딩 방식도 확정. 덤으로 **SCC가 실제로는 색상 지정 명령이라는 게 재확인**되며 2026-07-26 정정("색상 아님")이 SCC에 한해서는 틀렸음이 드러남
- ~~`SetDataOOP`~~ — **확인 완료(2026-08-18), 8.12절 참고.** Wizard 자체의 OOP 요청(`GetDataOOP`/`GetDataOOP2`)+응답(`SetDataOOP`) 왕복 전체를 `Stream.cpp` 안에서 확정 — 8.10절(IBKSConnector)과는 별개 구현이며, 필드 순서 정렬은 양쪽이 공유하는 `m_ioR[]` 순회로 보장됨을 확인
- ~~`SetDataNRM2`/`SetDataTAB2`~~ — **확인 완료(2026-08-18), 8.13절 참고.** 사용자 컬럼 재배열/숨김 상태(`screen->m_outs`)를 반영하는 변형. `TAB2`는 `TAB`(v1)과 달리 그리드에서 `SetCells2`를 실제로 호출한다는 차이도 발견
- ~~`CfmGrid::GetEnum`/`SetEnum`의 슬롯 98/99가 담는 opaque 블롭의 정확한 의미~~ — **확인 완료(2026-08-19), 8.14절 참고.** `h/Grid.h`의 `_gridHi`(요청, 110바이트)/`_gridHo`(응답, 103바이트) 구조체로 확정. `IB999988` 체결그리드 요청 헤더를 바이트 단위로 대조 검증(`visible`/`rows`/`type`/`dir`/`sort`/`symbol`/`key`/`page`/`save` 전부 일치). OOP 전용이 아니라 `GetDataNRM`/`NRM2`/`SetDataNRM`/`NRM2`/`TAB`/`TAB2`에도 `GO_HEADER` 게이팅으로 공통 사용된다는 것도 확인
- ~~`SetDataH`~~ — **확인 완료, 8.7절 참고.** 이 항목은 8.7절 작성 이후에도 지워지지 않고 남아있던 stale 항목이었음(2026-08-18 정리) — `TH_LEDGER`일 때 원장 블록 스킵+`SetLedger` 위임 규칙, `_ledgerH` 384바이트 구조까지 확정됨
- **`SetTable`엔 있고 `SetCells`엔 없는 FCC/RCC/SCC 처리 — 왜 GO_TABLE 방식만 셀별 동적 속성제어를 지원하는지 설계의도** — 관련 메커니즘(그리드 셀 전용 `CScreen::ParseRCC` vs 화면 전체용 `CStream::ParseRCC`, 8.5절)은 2026-08-18에 확정됐으나 "왜 SetCells 쪽엔 이 지원이 아예 없는지"는 여전히 미확인
- **(신규, 2026-08-18, 8.5절) RCC의 `csTREE`가 `USRDIR` 하위에 파일을 쓰는 부수효과의 정확한 포맷/용도** — 존재는 확인(`CStream::ParseRCC`), 상세 미조사
- `WM_USER` 커스텀 메시지의 정확한 용도
- ~~TR 요청(사용자가 조회 버튼 누르는 것) → 소켓 송신 경로~~ — **확인 완료(2026-07-30), 8.8절 참고.** `RouteTR`이 `CGuard::Write(char*, int, bool)`로 최종 소켓 전송하며, 한 번의 write에 여러 화면(unit)의 `_axisH` 프레임이 배치로 묶일 수 있음
- `CDll::OnAxis`(DLL 기반 작업영역)의 실제 파싱 로직 — **`CClient`와 다르다는 것 자체는 확인됨(2026-07-31)**: `CStream::OutStream`/`SetDataNRM`을 전혀 안 타고 받은 바이트를 그대로 `WM_USER`로 로드된 DLL에 던진다(`Dll.cpp:530`, `[CDll-OnAxis-raw]` 로그 추가, `@docs/DebugLogGuide.md` 7절). 다만 그 DLL 내부의 실제 파싱 로직 자체는 Wizard 소스 밖이라 여전히 미조사 — 실사용 사례: `9524`(이벤트 데이터 조회, 기획부) 화면이 이 경로를 탐
- **(신규, 2026-08-19) `CDll`의 두번째 유력 사용처 후보 — `CONTROL/ibk_chart_dll_20220831/chart_dll/gView/axisGView.dll`.** KRX 애프터장 대응(1900/1901/1902 필드, 위 6항) 작업 중 `gCom`(`AxisGCom.dll`)이 `chart_dll/C_Total`이 아니라 `chart_dll/gView/grpView.cpp`가 `LoadLibrary("axisGCom.dll")`/`LoadLibrary("axisGMain.dll")`로 동적 로드하는, `C_Total`과는 독립적인 **세번째 GOOPHOOP 클라이언트 구현**임을 발견. `axisGView.def`가 export하는 유일한 함수명이 `axCreate` 하나뿐인데, 이건 `CDll::Attach`(`Wizard/Dll.cpp:454`)가 찾는 첫번째 진입점 이름과 정확히 일치 — `CDll::Attach`의 `name.Format("%s\\%s\\%s.dll", root, DEVDIR, tmapN)`(`tmapN`=화면의 8자리 맵코드) 규칙상 실제 배포 파일명은 맵코드 이름으로 리네임될 것으로 추정되어, 소스 검색만으로는 `axisGView.dll`이 정확히 어느 맵코드에 대응하는지 확인 불가(이 환경엔 배포된 HTS 폴더 자체가 없어 실물 대조도 불가능했음). `vtypeGRX`(0x04, "그래픽")가 `CDll`로 라우팅되는 두 `type` 중 하나라는 기존 기록과 정황상 맞아떨어짐 — **차트/그래프 화면이 `CDll` 경로의 원래 설계 의도였을 가능성.** 정확한 맵코드는 실제 배포 환경(`axscreenmanage.ini` 등) 확인 필요, 사용자 확인 대기 중.
- **(신규, 2026-08-19) `gCom`(`AxisGCom.dll`)의 `MakeInputSymbol()`이 이미 오래전부터 `OJ_GUBN`(1901)을 GOOPHOOP 요청에 포함하고 있었음** — `C_Total`/IBKSConnector의 18개 필드와 달리 `gView` 경로는 19개(`...OJ_START, OJ_GUBN`, `gCom/PnInput.cpp:1361`). `axisgwin.h` 주석("소속구분")과 이번에 확인된 새 의미("장마감시간", 위 6항)가 상충하는데, 만약 `gView`가 실제로 라이브 화면에서 이 필드를 이미 쓰고 있다면 **KRX 서버측 개편으로 1901의 반환값이 바뀌는 순간 `gView` 쪽 화면에서 조용한 회귀(잘못된 데이터 표시)가 생길 위험**이 있음 — `gView`의 실사용처가 확인되는 대로 재검토 필요.
- **(신규, 2026-08-17, 8.11.3절) `$*`(`m_push`) 전용 채널 `_anmH`/`FEV_PUSH`** — 구조(요청 SetPush/수신 OnPush/원형버퍼 반영)는 코드로 확정됐으나: (1) `anmK_*` 상수 전체 목록 미확인(`anmK_ALIVE`/`anmK_PUSH`만 확인됨), (2) `USRDIR/{pushN}` 로컬 파일의 실제 포맷/용도 미확인, (3) 1절의 `msgK_ARM`(0x92)/`msgK_AUX`(0x93)와 이 `_anmH` 채널의 관계는 **`CWizardCtrl` 레벨에서는 별개 함수로 갈라짐이 2026-08-18 확인됨(0절 참고)** — 더 아래 `CGuard` 레벨 합류 여부만 남음, (4) `$$`/`$*`를 실제로 쓰는 맵 화면 예시 미확인(`$?`만 체결그리드로 실측됨)
- **(신규, 2026-08-18, 0절) `CGuard::OnAlert`의 2-인자(`FEV_ANM`/RTM 전용)/4-인자(`msgK_AUX`가 씀) 두 오버로드가 내부적으로 같은 `DoRTM` 파이프라인에 합류하는지** — `CWizardCtrl` 레벨까지는 갈라짐이 확인됐으나 `Guard.cpp` 안에서 최종적으로 합쳐지는지는 미확인
- **(신규, 2026-08-18, 0절) `msgK_ARM`의 `MAKEWPARAM(alarmPAN, false)` — `false` 플래그가 `msgK_POP`(`true`)와 정확히 무엇을 다르게 처리하는지** — `alarmPAN` 수신측(호스트 EXE 쪽 UI) 처리 로직은 Wizard 소스 밖이라 미조사
- **(신규, 2026-08-18, 0절) `winK_FMX`(0x1e)/`winK_DDE`(0x1f)의 `OnAux()` 분기, `OnFire(FEV_FMX,...)`/`CGuard::OnDDE`가 실제로 어떤 기능(Frame TRx 서비스, DDE 연동)에 쓰이는지** — 존재와 라우팅 지점만 확인, 상세 미조사

---

## 10. 관련 문서

- `@docs/WizardArchitecture.md` — 클래스 계층, 7절에 전체 클래스 레퍼런스
- `@docs/RealtimeCodeIndex_Investigation.md` — RTM 종목코드 매칭 상세
- `@docs/AxisformArchitecture.md` — 컨트롤(`CfmBase` 24종) 렌더링 레이어 상세
- `@docs/KnowledgeBase.md` — 트러블슈팅/설계의도 누적 기록
