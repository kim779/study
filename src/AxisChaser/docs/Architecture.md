# Architecture


## 목차

- [개요](#개요)
- [클래스 구조](#클래스-구조)
- [프로세스 간 통신(IPC) 및 동시성 구조](#프로세스-간-통신ipc-및-동시성-구조)
- [메시지/데이터 흐름](#메시지데이터-흐름)
- [Include 관계 (핵심)](#include-관계-핵심)
- [성능 최적화 설계 (2026-07-30 이후)](#성능-최적화-설계-2026-07-30-이후)
  - [배경: RTM 대량 유입 시 성능 문제](#배경-rtm-대량-유입-시-성능-문제)
  - [개선 설계 (5가지 최적화)](#개선-설계-5가지-최적화)
    - [1. WM_VSCROLL 배치 처리](#1-wm_vscroll-배치-처리)
    - [2. RichEdit Truncate (50MB 상한)](#2-richedit-truncate-50mb-상한)
    - [3. 데이터 큐 상한 설정](#3-데이터-큐-상한-설정)
    - [4. OutputDebugString 조건부 실행](#4-outputdebugstring-조건부-실행)
    - [5. 메모리 누수 방지 (CQue 소멸자)](#5-메모리-누수-방지-cque-소멸자)
  - [성능 측정 기준](#성능-측정-기준)
  - [상세 분석](#상세-분석)
- [리팩터링 후보 (추정 — 근거 명시)](#리팩터링-후보-추정-근거-명시)

---

- 생성일: 2026-07-10
- 목적: 프로젝트의 클래스/모듈 구조, 계층, 의존 관계를 기록한다.

## 개요

AxisChaser는 MFC 기반 Windows 실행 파일(단일 SDI 스타일, Doc/View 미사용 - CFrameWnd
+ CWnd 조합)로, AXIS 트레이딩 워크스테이션 프로세스가 주고받는 송수신 데이터·RTM(실시간)
데이터·시스템 트레이스·리포트 데이터를 다른 프로세스로부터 `WM_COPYDATA`로 전달받아
헥사덤프/텍스트 형태로 색상·폰트를 입혀 RichEdit 컨트롤에 표시하는 **트레이스 뷰어(디버깅
도구)**다. 이름의 "Chaser"는 전광판 이펙트가 아니라 AXIS 통신 데이터를 "쫓아가며(추적)"
보여주는 트레이스 도구를 의미하는 것으로 확인됨.

## 클래스 구조

| 클래스 | 베이스 | 역할 |
|---|---|---|
| `CAxisChaserApp` | `CWinApp` | 앱 진입점. 단일 인스턴스 뮤텍스(`IsFirstInstance`), 명령행 파싱, 레지스트리 설정 관리 |
| `CCommParam` | `CCommandLineInfo` | 명령행 인자 파싱(`-c`: 클래스명, `-r`: 대상 레지스트리 키) |
| `CMainFrame` | `CFrameWnd` | 메인 프레임. 툴바(`CToolBar24`) + `CChildView` 호스팅, 창 위치/크기 저장 |
| `CChildView` | `CWnd` | 핵심 뷰. 트레이스 표시, 필터/검색 UI, 데이터 파싱·큐잉 |
| `CQue` | `CObject` | 수신 데이터 1건을 담는 큐 항목(`m_flag`, `m_nBytes`, `m_pBytes`) |
| `COptions` | `CDialog` | 옵션 다이얼로그(표시 대상 코드 목록 관리, `chaser.ini` 연동) |
| `CFontSetDlg` | `CDialog` | 카테고리별(전체/SND·RCV/RTM/SYS/REPORT) 폰트 설정 |
| `CTextColorDlg` | `CDialog` | 카테고리별 텍스트 색상 설정 |
| `CColor` | (없음) | RGB ↔ HLS 색상 변환 유틸리티 (독일어 주석, 외부 유틸 코드로 추정) |
| `CEnBitmap` | `CBitmap` | 32비트 비트맵 로드/그레이스케일/비활성화 이미지 가공 |
| `C32BitImageProcessor` 및 파생(`CImageNormal`,`CImageGrayer`,`CImageHigh`) | - | 픽셀 단위 이미지 프로세싱 전략 (Strategy 패턴 후보) |
| `CToolBar24` | `CToolBar` | 24비트 컬러 툴바 이미지 처리(`CEnBitmap` 활용) |
| `CRichEditCtrlEx`, `CRTFBuilder` 등 | `CRichEditCtrl` 등 | (RichLib 서브프로젝트) RTF 스트림 빌더 — `<<`, `>>` 연산자로 색상/폰트 서식 문자열 구성 |

## 프로세스 간 통신(IPC) 및 동시성 구조

- 실제 소켓/네트워크 코드는 AxisChaser 내부에 없음. 데이터 원본은 별도 프로세스(AXIS
  워크스테이션 본체)이며, `WM_COPYDATA` 메시지로 전달받는다(`CMainFrame::OnCopyData` →
  `CChildView::CopyData`).
- 별도 워커 스레드(`CreateThread`/`AfxBeginThread`)는 사용하지 않는다. 대신 UI 스레드
  내에서 **큐 + 커스텀 메시지 펌핑**으로 비동기 처리를 흉내낸다:
  1. `CopyData()`가 수신 바이트를 `CQue`로 감싸 `m_que`(`CObArray`)에 추가 (RTM 데이터는
     큐 200건 초과 시 드롭)
  2. `CCriticalSection m_sync`로 큐 추가/제거 구간만 보호 (진짜 멀티스레드 경쟁이 아니라
     안전을 위한 방어적 락으로 보임 — 전부 UI 스레드에서 실행되는 듯하나, `PostMessage`
     비동기 처리와 결합되어 재진입 가능성 대비용으로 추정)
  3. `PostMessage(WM_RECEIVE)` → `CChildView::OnReceive`가 한 번에 최대 20건(`MAX_PER_CALL`)
     을 꺼내 `OnRCVData()` 호출, 큐에 남은 항목이 있으면 다시 `PostMessage`로 재귀 예약
     (UI 응답성 유지를 위한 배치 처리)
- `SetTimer(TM_STAYONTOP, 1000, ...)` — 시작 시 1초 뒤 "항상 위" 상태를 자동 해제하는
  1회성 타이머.

## 메시지/데이터 흐름

```
[AXIS 프로세스] --WM_COPYDATA--> CMainFrame::OnCopyData
                                      │
                                      ▼
                          CChildView::CopyData (큐 적재, m_sync로 보호)
                                      │ PostMessage(WM_RECEIVE)
                                      ▼
                          CChildView::OnReceive (최대 20건 배치 처리)
                                      │
                                      ▼
                          CChildView::OnRCVData (데이터 종류별 분기)
              x_SNDs/x_RCVs(송수신) ─┬─ x_RTMs(실시간) ─┬─ x_STRs(시스템) ─┬─ x_CONs(리포트)
                                      │                  │                 │
                                      ▼                  ▼                 ▼
                                  hex dump          parseData()      ReportParse()
                                      └──────────────────┴─────────────────┘
                                                    ▼
                                          CChildView::addTrace()
                                     (필터/키워드/바이트범위 적용 후
                                      CRTFBuilder로 색상·폰트 서식 구성)
                                                    ▼
                                        CRichEditCtrlEx (m_trace) 출력
```

- `x_SNDs`/`x_RCVs`: `../H/axis.h`의 `_axisH` 헤더 구조체를 해석, `statENC` 플래그가
  설정된 경우 `Xecure()`로 복호화를 시도하나, **현재 `Xecure()` 구현은 주석 처리되어
  `false`만 반환**(ActiveX 컨트롤 `AxisXecure.XecureCtrl.IBK2019` 연동 코드 전체가
  `CChildView::OnCreate` 내부에 주석 처리된 상태 — 죽은 코드, 삭제하지 않고 확인 필요로
  남겨둠).
- `x_RTMs`/`x_CONs`: 탭/개행 구분 텍스트를 `parseData()`/`ReportParse()`로 키-값 파싱 후
  포맷팅.

## Include 관계 (핵심)

- `ChildView.cpp`가 `../H/axisfire.h`, `../H/axis.h` — 워크스페이스 루트의 AXIS 공용
  프로토콜 헤더(`d:\src\IBKS\src\H\`)에 의존. 이 헤더들은 AxisChaser 트리 밖에 있음.
- `ChildView.h`가 `RichLib/RichEditCtrlEx.h`를 include — RichLib은 별도 `.vcxproj`를
  가진 서브 프로젝트이며, 빌드 산출물(`RichLib.lib`)을 링크하는 구조(자세한 내용은
  [Dependency.md](Dependency.md) 참고).

## 성능 최적화 설계 (2026-07-30 이후)

### 배경: RTM 대량 유입 시 성능 문제

AxisChaser는 AXIS 프로세스로부터 RTM(실시간 시세) 200~1000건/초, SND/RCV 10~100건/초로
대량의 트레이스 데이터를 수신한다. 개선 전에는 다음 5가지 원인으로 CPU 60-90%, UI 프리징
500ms 이상 발생:

1. **WM_VSCROLL 매 호출 동기 렌더링**: `addTrace()` 내에서 매번 `SendMessage(WM_VSCROLL, 
   SB_BOTTOM)` 호출 → 렌더링 오버헤드 80-90%
2. **OutputDebugString 오버헤드**: Debug 빌드 + 디버거 연결 시 1-10ms/호출
3. **RichEdit 무제한 누적**: 장시간 운영 시 100MB 이상 축적, 성능 선형 악화
4. **데이터 큐 무제한**: CopyData()에서 상한 체크 없음 → 극한 상황에 메모리 폭발
5. **배치 처리 부재**: UI 스레드 장시간 점유

### 개선 설계 (5가지 최적화)

#### 1. WM_VSCROLL 배치 처리

**설계**:
- `OnReceive()` 루프에서 최대 20건(MAX_PER_CALL)을 처리한 뒤, 큐가 남아있어 다시
  `PostMessage(WM_RECEIVE)`로 재유입되는 경우에도 매 호출마다 `SendMessage(WM_VSCROLL,
  SB_BOTTOM)`을 정확히 한 번씩 실행 (addTrace() 호출 횟수와 무관하게 1:1)
- 구현: `ChildView.cpp:1065-1088`
- 큐가 계속 채워지는 대량 유입 상황에서도 매 배치(최대 20건)마다 스크롤이 갱신되므로,
  실시간성(최신 데이터가 계속 보이는 것)을 유지하면서 addTrace() 호출당 리드로우이던 것을
  배치당 1회로 줄임 (최초 구현에서는 큐가 완전히 빌 때만 스크롤하도록 되어 있어 대량 유입
  지속 시 화면이 갱신되지 않는 회귀가 있었음 — 수정 완료)

**기대 효과**:
- 렌더링 호출: 100+회/배치 → 1회/배치 (99% 감소)
- CPU: 60-90% → 10-40% (추정 80% 개선)
- UI 응답 시간: 500ms → 100ms 이하

**트레이드오프**:
- 데이터 도착 → UI 표시 지연: 최대 20건 × OnReceive 주기
- 스크롤 위치 손실 불가능 (루프 완료 후 정확히 한 번만)

#### 2. RichEdit Truncate (50MB 상한)

**설계**:
- `addTrace()` 내에서 텍스트 길이 체크
- 50MB 도달 시 상단 50% 삭제: `SetSel(0, docLen/2)` → `ReplaceSel("")`
- 구현: `ChildView.cpp:803-807`

**기대 효과**:
- 메모리 사용량: 100-200MB 안정화 (무한 증가 방지)
- Truncate 지연: < 500ms (동기 호출, 드문 빈도)

**트레이드오프**:
- Truncate 중 UI 프리징 가능 (~500ms, 50MB 도달 시에만)
- 상단 50% 로그 데이터 손실 (로그 도구이므로 허용)

#### 3. 데이터 큐 상한 설정

**설계**:
- `CopyData()` 진입 시 큐 크기별 상한 체크
- RTM: 200건 (높은 빈도, 낮은 우선순위)
- SND/RCV: 500건 (거래 관련, 높은 우선순위)
- 구현: `ChildView.cpp:1042-1047`

**기대 효과**:
- 메모리 누수 방지 (극한 상황)
- 큐 크기 안정화

**트레이드오프**:
- 큐 상한 도달 시 데이터 드롭 (로그용이므로 일부 손실 허용)
- 드롭 여부를 UI에 표시하지 않음 (사용자 인지 불가)

#### 4. OutputDebugString 조건부 실행

**설계**:
- `addTrace()` 내 OutputDebugString을 `#ifdef _DEBUG` 블록으로 감싸기
- Release 빌드에서 디버그 출력 제거
- 구현: `ChildView.cpp:788-791`

**기대 효과**:
- Release 빌드 오버헤드 제거 (미미하지만 누적)

#### 5. 메모리 누수 방지 (CQue 소멸자)

**설계**:
- CQue 소멸자에서 `delete[] m_pBytes` 실행
- 기존: 구현되지 않음 → 메모리 누수 가능성

**기대 효과**:
- 메모리 누수 방지

### 성능 측정 기준

| 지표 | 개선 전 | 개선 후 | 개선율 |
|------|--------|--------|--------|
| CPU (RTM 200건/초) | 60-90% | 20-40% | 50-70% |
| UI 응답 시간 | 500ms | 100ms | 80% |
| 메모리 증가율 (10분) | 선형 증가 → 100+MB | 50MB 안정 | 무한→유한 |
| 큐 크기 (극한) | 무제한 | RTM: 200, SND/RCV: 500 | 무한→유한 |

### 상세 분석

자세한 설계 의도, 트러블슈팅 히스토리, 동시성 모델은 
[KnowledgeBase.md](KnowledgeBase.md#설계-의도-및-트러블슈팅-히스토리) 참고.

---

## 리팩터링 후보 (추정 — 근거 명시)

- `CChildView::OnRCVData` (`ChildView.cpp:379`) — 약 330줄, 데이터 종류 4가지(SND/RCV,
  RTM, SYSTRACE, REPORT)를 하나의 switch 안에서 모두 처리. SRP 위반 후보, 종류별 함수로
  분리 여지 있음.
- `OnCreate`(`ChildView.cpp:198`) 내 Xecure ActiveX 등록/생성 코드 전체(약 60줄)가
  주석 처리된 채 남아있음 — 삭제하지 않고 사용자 확인 필요로 표시.
- `#ifdef DF_MBCS` 분기(`ChildView.cpp:499-581`)가 `RichEditCtrlEx.h`에서 항상
  `#define DF_MBCS`로 고정되어 있어, `#else` 분기는 사실상 죽은 코드로 추정.
- `CColor` 클래스(독일어 주석, 1999년 작성)는 AxisChaser 내에서 실제 사용처가 코드상
  확인되지 않음 — 별도 유틸리티를 가져다 쓴 것으로 보이며 사용 여부 확인 필요.
