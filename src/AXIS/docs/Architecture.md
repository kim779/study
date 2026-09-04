---
project: AXIS
category: architecture
status: 작성됨
updated: 2026-08-21
---

# AXIS 프로젝트 아키텍처


## 목차

- [프로젝트 규모](#프로젝트-규모)
- [계층 구조 (Layered Architecture)](#계층-구조-layered-architecture)
- [스레드 구조 (Thread Architecture)](#스레드-구조-thread-architecture)
  - [메인 스레드 (UI Thread)](#메인-스레드-ui-thread)
  - [워커 스레드](#워커-스레드)
    - [1. CPhoneThread (전화 패드 관련)](#1-cphonethread-전화-패드-관련)
    - [2. 기타 워커 스레드 (추정)](#2-기타-워커-스레드-추정)
  - [동기화 메커니즘](#동기화-메커니즘)
  - [Race Condition 위험 지점 (확인 필요)](#race-condition-위험-지점-확인-필요)
- [메시지 흐름 (Message Flow)](#메시지-흐름-message-flow)
  - [입력 계열 (Input Events)](#입력-계열-input-events)
  - [타이머 기반 폴링](#타이머-기반-폴링)
  - [예외/크래시 처리 흐름](#예외크래시-처리-흐름)
- [전역 상태 및 싱글톤 패턴](#전역-상태-및-싱글톤-패턴)
  - [1. CAxisApp (애플리케이션 싱글톤)](#1-caxisapp-애플리케이션-싱글톤)
  - [2. CAxInstance (중복 실행 방지 싱글톤)](#2-caxinstance-중복-실행-방지-싱글톤)
  - [3. 전역 변수들 (MainFrm.h 추정)](#3-전역-변수들-mainfrmh-추정)
  - [4. 레지스트리 기반 설정 관리](#4-레지스트리-기반-설정-관리)
- [클래스 계층 구조 (선별)](#클래스-계층-구조-선별)
  - [MFC 프레임워크 기반 클래스](#mfc-프레임워크-기반-클래스)
  - [커스텀 컨트롤 및 헬퍼 클래스](#커스텀-컨트롤-및-헬퍼-클래스)
- [COM/ActiveX 통합](#comactivex-통합)
  - [COM 초기화](#com-초기화)
  - [Dispatch Object Model (IDispatch)](#dispatch-object-model-idispatch)
  - [ActiveX/OCX 컨트롤](#activexocx-컨트롤)
- [초기화/종료 시퀀스 (Initialization/Termination)](#초기화종료-시퀀스-initializationtermination)
  - [시작 시퀀스](#시작-시퀀스)
  - [종료 시퀀스](#종료-시퀀스)
- [주요 발견사항](#주요-발견사항)
  - [1. 메시지 기반 아키텍처의 복잡성](#1-메시지-기반-아키텍처의-복잡성)
  - [2. 스레드 모델 불명확](#2-스레드-모델-불명확)
  - [3. 전역 싱글톤 의존성](#3-전역-싱글톤-의존성)
  - [4. COM/ActiveX 복잡도](#4-comactivex-복잡도)
  - [5. 메모리 관리](#5-메모리-관리)
  - [6. 리소스 정리 불완전](#6-리소스-정리-불완전)
  - [7. 보안 관련 복잡도](#7-보안-관련-복잡도)
- [알려진 제약사항 및 기술부채](#알려진-제약사항-및-기술부채)
- [성능/스케일 특성](#성능스케일-특성)
  - [폴링 기반 아키텍처](#폴링-기반-아키텍처)
  - [메모리 사용](#메모리-사용)
  - [네트워크 I/O](#네트워크-io)
- [권장 분석 우선순위](#권장-분석-우선순위)
- [참고 문서](#참고-문서)

---

**프로젝트 개요**: AXIS는 IBK 투자증권 HTS(Home Trading System) 메인 GUI 애플리케이션으로, 주식거래 주문, 계좌조회, 실시간 시세, 매매알림, 관리자 기능 등을 제공하는 Windows MFC 기반 MDI(Multiple Document Interface) 애플리케이션입니다.

**분석 일시**: 2026-07-14  
**대상 버전**: Visual Studio 2019 (ToolsVersion 15.0, PlatformToolset v142)  
**대상 플랫폼**: Win32 (x86)  
**MFC 방식**: Dynamic MFC (DLL 링크)

---

## 프로젝트 규모

- **소스 파일**: ~133 개 .cpp + 100+ .h 파일 (기본 AXIS 폴더)
- **서브폴더**:
  - `grid/` - 격자 컨트롤 (fxGrid, InPlaceEdit, InPlaceDate, InPlaceCombo, excel9, DropTarget 등 ~8개)
  - `ASTx/` - Ahnlab SafeTx SDK (보안 관련)
  - `aossdk/` - AOSS SDK (구 보안 SDK, 대부분 disabled)
  - `inca/` - INCA 소프트웨어 래퍼
  - `MacExec/` - 매매 매크로 실행
  - `Test/` - 테스트 코드
  - `UnhandledException/` - 예외 처리 (예: CriticalSection.h, ExceptionHandler.h)
  - `res/` - 리소스 관련 파일
  - `kings/` - KApi2 래퍼
  - `cloude/` - 클라우드 인터페이스
  - `packages/` - cpprestsdk (NuGet)
- **전체 클래스 수**: ~150+ 개 (CMainFrame, CAxisApp, CAxisDoc, CAxisView, CChildFrame, ~130개 이상 다양한 다이얼로그/컨트롤)
- **주요 프로토콜/메시지**: 50+ 개 사용자 정의 메시지 (WM_USER + offset 방식)
- **컴파일 표준**: C++17 (LanguageStandard stdcpp17)
- **동시성 기본**: CCriticalSection (RAII 래퍼), CWinThread 기반 스레드, `shared_mutex` (STL)

---

## 계층 구조 (Layered Architecture)

```
┌─────────────────────────────────────────────────────┐
│         UI Layer (MFC Presentation)                 │
│  ┌────────────────────────────────────────────────┐ │
│  │ Frame Window: CMainFrame (CMDIFrameWnd)       │ │
│  │  - ~150 메시지 핸들러 (OnTimer, OnCreate 등)    │ │
│  │  - ~30+ 사용자 정의 메시지 (WM_USER + offset) │ │
│  │  - 자식 문서/뷰 관리 (MDI)                    │ │
│  │  - 타이머 기반 폴링 (CHKID=100, CHKTIME=2000) │ │
│  └────────────────────────────────────────────────┘ │
│                      ↓                               │
│  ┌──────────────────────────────────────────────┐   │
│  │ View/Dialog Layer:                          │   │
│  │  - CAxisView (문서 뷰)                       │   │
│  │  - CAxisDoc (MFC 문서 모델)                  │   │
│  │  - 50+ 모달/모드리스 다이얼로그            │   │
│  │    (DuplicateLoginConfirmDlg, SecureDlg 등) │   │
│  │  - Custom Grid Controls (fxGrid)             │   │
│  └──────────────────────────────────────────────┘   │
│                      ↓                               │
│  ┌──────────────────────────────────────────────┐   │
│  │ Custom Controls Layer:                      │   │
│  │  - NGrid, NTable (테이블 그리드)             │   │
│  │  - BmpButton, BtnST, ImgBtn (버튼)          │   │
│  │  - NTab, NProgressCtrl (탭, 프로그레스)      │   │
│  │  - NDib, NPalette (이미지 처리)              │   │
│  │  - TTip, MyToolTip (도구팁)                 │   │
│  └──────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────┘
         ↓         ↓         ↓         ↓
┌──────────────────────────────────────────────────────┐
│      Service/Module Layer (Business Logic)           │
│  ┌────────────────────────────────────────────────┐  │
│  │ Core Services:                                │  │
│  │  - axInstance: 중복 실행 방지 (싱글톤)        │  │
│  │  - ResourceHelper: 리소스 관리                 │  │
│  │  - GDIHelper, NDib: 그래픽 처리               │  │
│  │  - CpuUsage, CpuUse, sysInfo: 시스템 정보    │  │
│  │  - Alarm: 알람/경고 시스템                     │  │
│  │  - Dtconnect: 서버 연결 관리                   │  │
│  │  - Setup, SetupC: 사용자 설정 관리             │  │
│  │  - AccountConfig, TotalAccount: 계좌 관리     │  │
│  │  - EmailNotify 관련 클래스들                  │  │
│  │  - Chaser: 명령어 추적(디버깅 도구)          │  │
│  └────────────────────────────────────────────────┘  │
│  ┌────────────────────────────────────────────────┐  │
│  │ Data Models:                                  │  │
│  │  - dtInfo: 데이터 타입 정보                     │  │
│  │  - Conclusion, EConclusion: 체결 정보          │  │
│  │  - BaseMP, TotalAcc: 계좌 모델                │  │
│  │  - MacExec: 매매 매크로 모델                    │  │
│  └────────────────────────────────────────────────┘  │
│  ┌────────────────────────────────────────────────┐  │
│  │ Utility/Helper:                               │  │
│  │  - fname: 파일명 처리                          │  │
│  │  - IhUtils: 유틸리티 함수                      │  │
│  │  - MapHelper: 맵 처리                         │  │
│  │  - XZip, XUnzip: 압축 처리                    │  │
│  │  - XCaption: 캡션 커스터마이징                │  │
│  │  - Helper: 범용 헬퍼 함수                     │  │
│  └────────────────────────────────────────────────┘  │
└──────────────────────────────────────────────────────┘
         ↓         ↓         ↓         ↓
┌──────────────────────────────────────────────────────┐
│         DLL/Library Layer (External Services)        │
│  ┌────────────────────────────────────────────────┐  │
│  │ AXIS Internal DLLs (동일 회사 개발):           │  │
│  │  - axiscm.lib (증권사 통신 모듈)               │  │
│  │  - axissm.lib (서버 관리 모듈)                 │  │
│  │  - axiscp.lib (컨트롤 팩 모듈)                 │  │
│  │  (위 3개는 LinkLib 단계에서 결합)              │  │
│  └────────────────────────────────────────────────┘  │
│  ┌────────────────────────────────────────────────┐  │
│  │ Third-party DLLs (Runtime 로드):              │  │
│  │  - ASTx/StSdkEx.lib (Ahnlab SafeTx SDK)        │  │
│  │  - cpprestsdk (NuGet, REST/JSON)               │  │
│  │  - 기타 ActiveX 컨트롤                        │  │
│  └────────────────────────────────────────────────┘  │
│  ┌────────────────────────────────────────────────┐  │
│  │ Windows API/SDK:                              │  │
│  │  - Imm32.lib (IME 입력)                       │  │
│  │  - msimg32.lib (알파 합성 이미지)              │  │
│  │  - wininet.lib (인터넷 연결)                   │  │
│  │  - Netapi32.lib, Winmm.lib (네트워크, 멀티미디어) │
│  │  - iphlpapi.lib (IP 헬퍼 API)                 │  │
│  │  - Winsock2, WinCrypt (소켓, 암호화)          │  │
│  │  - TOOLHELP32 (프로세스 모니터링)              │  │
│  └────────────────────────────────────────────────┘  │
│  ┌────────────────────────────────────────────────┐  │
│  │ Third-party SDKs:                             │  │
│  │  - INCA (증권사 소프트웨어 래퍼)               │  │
│  │  - ASTx/SafeTx (보안 SDK - Ahnlab)             │  │
│  │  - AOSS SDK (구 보안, deprecated)              │  │
│  └────────────────────────────────────────────────┘  │
└──────────────────────────────────────────────────────┘
```

---

## 스레드 구조 (Thread Architecture)

### 메인 스레드 (UI Thread)

```
WinMain (MFC 초기화)
  ↓
AfxWinApp::InitInstance()
  - CoInitialize() (COM 초기화)
  - Enable3dControls() (MFC 3D 컨트롤)
  - AfxOleInit() (OLE/ActiveX 초기화)
  - AfxSocketInit() (Winsock 초기화)
  - STSDKEX 초기화 (Ahnlab SafeTx 콜백 등록)
  - CMainFrame 생성 및 표시
  ↓
CMainFrame::OnCreate()
  - InitialUpdate()
  - 각종 자식 창 생성 (CChildFrame, 다이얼로그)
  - SetTimer(CHKID, CHKTIME=2000ms) - 폴링 타이머
  ↓
메시지 루프 (GetMessage/TranslateMessage/DispatchMessage)
  - ~150 OnTimer, OnClose, OnSize 등 메시지 핸들러
  - PostMessage/SendMessage 기반 통신
  ↓
종료 시퀀스
  - OnClose → OnDestroy
  - CMainFrame::~CMainFrame()
```

### 워커 스레드

#### 1. CPhoneThread (전화 패드 관련)
- **생성**: CMainFrame에서 AfxBeginThread로 생성 (추정)
- **역할**: 전화 시스템/폰 패드 메시지 처리
- **메시지 맵**: BEGIN_MESSAGE_MAP(CPhoneThread, CWinThread) (메시지 핸들러 미정의)
- **종료**: ExitInstance() 처리
- **동기화**: (코드상 명시적 동기화 없음 — 추정: 이벤트/포스트메시지 사용)

#### 2. 기타 워커 스레드 (추정)
- **다이얼로그 기반 모드리스 창**: 별도 스레드에서 메시지 루프 실행 가능성 (CDialog::DoModal 비모달 사용 시)
- **타이머 콜백**: OnTimer 핸들러는 UI 스레드에서 동기 실행 (UI Thread Block 위험)

### 동기화 메커니즘

1. **CCriticalSection (RAII 래퍼)**
   - 위치: d:\src\IBKS\src\AXIS\CriticalSection.h
   - 패턴: `CUseCriticalSection cs(m_criticalSection)` (스택 기반 RAII)
   - 용도: 공유 데이터 보호 (추정: 글로벌 상태, 캐시 등)
   - **제약**: 재귀적 잠금 불가 (EnterCriticalSection 단순 사용)

2. **std::shared_mutex** (C++17, MainFrm.h 선언)
   - 용도: 추정상 읽기-쓰기 동시성 제어
   - 우려: 메인 스레드에서 차단 가능성 (UI Thread Block)

3. **PostMessage 기반 비동기 통신**
   - 예: WM_AXIS, WM_USER, WM_ELOG, WM_TRAYEVENT 등 ~30+ 사용자 정의 메시지
   - 방식: 워커 → UI 스레드로 PostMessage, UI 스레드에서 처리

### Race Condition 위험 지점 (확인 필요)

- **전역 상태**: CAxisApp 싱글톤 (m_instance, m_flag, Axis::user, m_regkey 등)
  - 초기화/종료 시 동기화 부재 가능성
- **리소스 헬퍼**: m_resourceHelper 동적 할당/삭제 (OnCreate, 소멸자)
  - 스레드 안전성 미확인
- **CCriticalSection 사용 분포**: 전체 코드에서 실제 사용 현황 미확인
  - 모든 공유 데이터가 보호되는지 검토 필요

---

## 메시지 흐름 (Message Flow)

### 입력 계열 (Input Events)

```
사용자 입력 (마우스, 키보드, 타이머)
  ↓
Windows 메시지 큐
  ↓
MFC 메시지 맵 디스패치 (DispatchMessage)
  ↓
CMainFrame::BEGIN_MESSAGE_MAP
  ├─ ON_WM_CREATE → CMainFrame::OnCreate()
  ├─ ON_WM_CLOSE → CMainFrame::OnClose()
  ├─ ON_WM_TIMER → CMainFrame::OnTimer()
  │   └─ 폴링 기반 상태 업데이트 (2초 간격)
  ├─ ON_WM_SIZE, ON_WM_SIZING → 윈도우 리사이징
  ├─ ON_WM_NCACTIVATE, ON_WM_NCPAINT → 프레임 드로잉
  ├─ ON_WM_ACTIVATE → 활성화/비활성화
  ├─ ON_WM_COPYDATA → IPC 메시지
  ├─ ON_MESSAGE(WM_AXIS, ...) → 거래/시세 업데이트
  ├─ ON_MESSAGE(WM_USER, ...) → 범용 사용자 메시지
  ├─ ON_MESSAGE(WM_CHILDMSG, ...) → 자식 창 메시지
  ├─ ON_MESSAGE(WM_CHASER, ...) → 명령어 추적
  ├─ ON_MESSAGE(WM_ELOG, ...) → 로그/에러
  ├─ ON_MESSAGE(WM_TRAYEVENT, ...) → 시스템 트레이
  ├─ ON_MESSAGE(WM_APPLYACC, ...) → 계좌 변경
  ├─ ON_MESSAGE(WM_SAVEACC, ...) → 계좌 저장
  ├─ ON_MESSAGE(WM_AXISCLOSE, ...) → 종료 신호
  ├─ ON_MESSAGE(WM_PHONEPAD, ...) → 전화 패드
  ├─ ON_MESSAGE(WM_HOTKEY, ...) → 핫키
  ├─ ON_MESSAGE(WM_SECURE, ...) → 보안 관련
  ├─ ON_MESSAGE(WD_MINICLOSE, ...) → 미니 종료
  ├─ ON_MESSAGE(WM_IBK, ...) → 외부 호출 (Wine)
  ├─ ON_MESSAGE(WX_HOTTRADE, ...) → 핫트레이드
  ├─ ON_MESSAGE(WD_TOP10, ...) → TOP10 뷰
  └─ ... (30+ 메시지)
  ↓
메시지 핸들러 실행 (OnXXX 메서드)
  ├─ 상태 변경
  ├─ 자식 창/컨트롤 업데이트
  ├─ 다른 창으로 PostMessage 발송
  └─ 데이터 저장 (레지스트리, 파일)
```

### 타이머 기반 폴링

```
SetTimer(CHKID=100, CHKTIME=2000ms)
  ↓
2초 마다 WM_TIMER 발생
  ↓
CMainFrame::OnTimer(UINT nIDEvent)
  ├─ if (nIDEvent == CHKID)
  │   ├─ CPU 사용률 확인 (CpuUsage)
  │   ├─ RTS (실시간 시세) 상태 확인
  │   ├─ 연결 상태 모니터링
  │   ├─ 자식 창 업데이트 (InvalidateRect, UpdateWindow)
  │   └─ 필요시 재연결/복구 로직
  └─ 다른 타이머: 각 다이얼로그/컨트롤별 타이머
```

### 예외/크래시 처리 흐름

```
비정상 조건 발생
  ↓
IExceptionHandler 인터페이스
  ├─ CExceptionHandler::SetConnectionPoint(IExceptionHandler *pPoint)
  └─ ExceptionInformation(CString strInfo) 콜백
  ↓
try/catch 블록 (38개 발견)
  └─ 또는 __try/__except (SEH)
  ↓
MiniDump 생성 (whdump.h 추정)
  └─ D:\symbol\axis.pdb (PDB 저장)
  ↓
Ahnlab SafeTx 콜백
  ├─ STSDKEX_EventCallback(long lCode, ...)
  ├─ STSDKEX_PB_CALLBACK_ABNORMAL_MEMORY_ACCESS
  ├─ STSDKEX_PB_CALLBACK_EXCEPTION_PROCESS
  └─ (추정: 보안 위반 감지 후 프로세스 종료)
```

---

## 전역 상태 및 싱글톤 패턴

### 1. CAxisApp (애플리케이션 싱글톤)

```cpp
CAxisApp theApp;  // 단일 인스턴스

// 주요 멤버
- m_instance: 중복 실행 방지
- m_flag: 상태 플래그
- Axis::user: 로그인 사용자명
- m_regkey: 레지스트리 키
- m_progK: 프로그램 모드 (MD_DEV, PK_DEV 등)
- m_pDispOM: Dispatch Object Model (COM)
- m_hSCSK: ? (불명 — 확인 필요)
- m_logPath: 로그 파일 경로
- m_resourceHelper: 리소스 관리자 (동적 할당)
- m_forceIP, m_forcePort: 강제 서버 IP/포트

// 초기화/종료 (axis.cpp)
CAxisApp::CAxisApp()
  - 모든 멤버 초기화
  
CAxisApp::~CAxisApp()
  - if (m_pDispOM) delete m_pDispOM
  - if (m_resourceHelper) delete m_resourceHelper
  - (기타 정리 미확인)
```

**우려**: 소멸자에서 수동 delete를 사용 → 스마트 포인터 미사용 → 예외 경로에서 누수 가능

### 2. CAxInstance (중복 실행 방지 싱글톤)

- 위치: d:\src\IBKS\src\AXIS\axInstance.h/cpp
- 역할: 동일 프로세스 중복 실행 방지 (Windows 뮤텍스/이름있는 이벤트)
- 패턴: 레지스트리 키 기반 유니크 ID 생성

### 3. 전역 변수들 (MainFrm.h 추정)

```cpp
// 다양한 전역 및 static 변수
// (정확한 목록은 헤더 파일 전체 읽기 필요)
```

### 4. 레지스트리 기반 설정 관리

- **로드 시점**: InitInstance() 또는 OnCreate()
- **저장 시점**: 사용자 설정 변경 시 (Setup.cpp, SetupC.cpp)
- **구조**: HKEY_CURRENT_USER\Software\... (표준 Windows 레지스트리)
- **우려**: 동시 읽기/쓰기 시 일관성 문제

---

## 클래스 계층 구조 (선별)

### MFC 프레임워크 기반 클래스

```
CWinApp
  └─ CAxisApp (axis.h/cpp)
       ├─ InitInstance() 오버라이드
       ├─ ExitInstance() 오버라이드
       └─ 멤버: 전역 상태 (싱글톤)

CMDIFrameWnd
  └─ CMainFrame (MainFrm.h/cpp)
       ├─ BEGIN_MESSAGE_MAP: ~150 메시지 핸들러
       ├─ CREATE_TOOLBAR, 상태 바 관리
       ├─ MDI 자식 창 관리 (CChildFrame)
       ├─ 멤버: 타이머, 각종 서비스 포인터
       └─ 소멸자: ~CMainFrame()

CMDIChildWnd
  └─ CChildFrame (ChildFrm.h/cpp)
       └─ 개별 문서 뷰 호스팅

CDocument
  └─ CAxisDoc (axisDoc.h/cpp)
       └─ 데이터 모델 (MDI 문서)

CView
  ├─ CAxisView (axisView.h/cpp)
  │   └─ 기본 뷰
  ├─ CScrollView
  │   └─ CAxisScrollView (axScrollView.h/cpp)
  │       └─ 스크롤 가능 뷰
  └─ CTabCtrl 기반
      └─ CAxTabView (AxTabView.h/cpp)
          └─ 탭 기반 뷰

CDialog
  ├─ CSecureDlg (보안 로그인)
  ├─ CDuplicateLoginConfirmDlg (중복 로그인 확인)
  ├─ CConfigDlg (설정)
  ├─ CSetupC (고급 설정)
  ├─ CAccountConfig (계좌 설정)
  ├─ CEmergencyDlg (긴급상황)
  ├─ CStopLoss (손절매)
  ├─ CAlarm (알람)
  ├─ CLockDlg (잠금)
  ├─ CProgressDlg (진행률)
  ├─ CTestInfoDlg (테스트 정보)
  └─ ... (50+ 다이얼로그)

CWinThread
  └─ CPhoneThread (PhoneThread.h/cpp)
       ├─ InitInstance()
       ├─ Run()
       └─ ExitInstance()

CListCtrl, CHeaderCtrl 기반
  └─ NGrid, NTable, fxGrid 커스텀 그리드 컨트롤
```

### 커스텀 컨트롤 및 헬퍼 클래스

```
GDI 그래픽 관련
  - CGDIHelper (GDIHelper.cpp)
  - CNDib (NDib.cpp) - DIB 비트맵
  - CNPalette (NPalette.cpp) - 팔레트
  - CMapHelper (MapHelper.h) - 맵 렌더링

버튼 및 UI 컨트롤
  - CBmpButton (BmpButton.cpp) - 비트맵 버튼
  - CBtnST (BtnST.cpp) - 스타일 버튼
  - CShapeButton (ShapeButton.cpp) - 모양 버튼
  - CImgBtn (ImgBtn.cpp) - 이미지 버튼
  - CNTab (NTab.cpp) - 탭 컨트롤
  - CNProgressCtrl (NProgressCtrl.cpp) - 프로그레스
  - CNExpand (NExpand.cpp) - 확장/축약

데이터 모델/정보
  - CDtInfo (dtInfo.cpp) - 데이터 타입 정보
  - CConclusion (Conclusion.cpp) - 체결 정보
  - CEConclusion (EConclusion.cpp) - 확장 체결 정보
  - CBaseMP (BaseMP.cpp) - 기본 매매 포지션
  - CTotalAccount (TotalAccount.cpp) - 통합 계좌
  - CTotalAcc (TotalAcc.cpp) - 계좌 정보
  - CAccountConfig (AccountConfig.cpp) - 계좌 설정

시스템/유틸
  - CResourceHelper (ResourceHelper.cpp) - 리소스 관리
  - CCpuUsage (CpuUsage.cpp) - CPU 사용률
  - CCpuUse (CpuUse.cpp) - CPU 사용 통계
  - CSysInfo (sysInfo.cpp) - 시스템 정보
  - CAxInstance (axInstance.cpp) - 중복 실행 방지
  - CHelpOK (HelpOK.cpp) - 도움말

알림/이벤트
  - CAlarm (Alarm.cpp) - 알람
  - CImageNotice (ImageNotice.cpp) - 이미지 알림
  - CKobaElwNotify (KobaElwNotify.cpp) - 공시 알림
  - CEmailNotify (추정, 파일명으로)

매매/거래
  - CMacExec (MacExec.cpp) - 매매 매크로 실행
  - CStopLoss (Stoploss.cpp) - 손절매
  - CContGrid (ContGrid.cpp) - 계약 그리드

파일/압축
  - CXZip (XZip.cpp) - ZIP 압축
  - CXUnzip (XUnzip.cpp) - ZIP 해제
  - CFname (fname.cpp) - 파일명 처리

보안
  - CSecureDlg (SecureDlg.cpp) - 보안 로그인
  - CLockPass (LockPass.cpp) - 패스워드 잠금
  - CCertLogin (CertLogin.cpp) - 인증서 로그인
  - CCertErrDialog (CertErrDialog.cpp) - 인증서 에러
  - CDlgPB (DlgPB.cpp) - 보안 정책 다이얼로그

네트워크
  - CDtconnect (Dtconnect.cpp) - 데이터 연결
  - CNSocket (NSocket.cpp) - 소켓 래퍼
  - CUploadFile (CUploadFile.cpp) - 파일 업로드

기타
  - CChaser (Chaser.cpp) - 명령어 추적(디버깅 도구)
  - CNewsViewer (NewsViewer.cpp) - 뉴스 뷰어
  - CPhoneThread (PhoneThread.cpp) - 전화 스레드
  - CFirstJob (FirstJob.cpp) - 초기화 작업
  - CMDebug (MDebug.cpp) - 디버그 유틸
```

---

## COM/ActiveX 통합

### COM 초기화

```cpp
// axis.cpp: InitInstance()에서
AfxOleInit();  // OLE/ActiveX 초기화

// MainFrm.cpp: CMainFrame::OnCreate()에서
CoInitialize(NULL);  // COM 초기화 (IME, 각종 COM 객체)
```

### Dispatch Object Model (IDispatch)

```cpp
// CAxisApp 멤버
IDispatch* m_pDispOM;  // 외부 자동화 클라이언트용 (추정)

// 자신 설정 예상 위치
// OnCreate 또는 Initialize 메서드에서
// CoCreateInstance로 생성 및 SetConnectionPoint
```

### ActiveX/OCX 컨트롤

- **INCA 소프트웨어**: npenkAppInstall5WIN.h → INCA 증권사 API 래퍼 (OCX 로드 가능성)
- **기타 OCX**: Alarm, 차트, 그리드 등이 ActiveX일 가능성
- **CCustomOccManager** (추정): OCX 래퍼 매니저 (이름에서 유추)

---

## 초기화/종료 시퀀스 (Initialization/Termination)

### 시작 시퀀스

```
1. WinMain (MFC 프레임워크)
   └─ theApp.Run()
       ↓
2. CAxisApp::InitInstance()
   ├─ Enable3dControls() (MFC 3D 컨트롤)
   ├─ CoInitialize(NULL) (COM)
   ├─ AfxOleInit() (OLE/ActiveX)
   ├─ AfxSocketInit() (Winsock)
   ├─ CAxInstance 생성 (중복 실행 방지)
   ├─ 레지스트리 설정 로드
   ├─ m_resourceHelper = new CResourceHelper() (동적 할당)
   ├─ m_pDispOM 초기화 (COM 객체)
   ├─ STSDKEX_EventCallback 등록 (Ahnlab SafeTx)
   ├─ CFirtsJob 실행 (사용자정의 초기화)
   ├─ CMainFrame 생성 및 표시
   └─ return TRUE
       ↓
3. CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
   ├─ CMDIFrameWnd::OnCreate(...)
   ├─ CMainFrame::InitialUpdate()
   ├─ SetTimer(CHKID, CHKTIME)
   ├─ 각종 자식 창 생성 (CChildFrame)
   ├─ 도구 모음, 상태 바 초기화
   ├─ m_pDocTemplate 설정
   └─ return 0
       ↓
4. 메시지 루프 (GetMessage → TranslateMessage → DispatchMessage)
```

### 종료 시퀀스

```
1. 사용자 종료 신호 (X 버튼, Alt+F4, WM_QUIT)
   ↓
2. CMainFrame::OnClose()
   ├─ 저장 확인 (m_pCurrentDoc)
   ├─ DestroyWindow()
   └─ PostQuitMessage(0)
       ↓
3. CMainFrame::OnDestroy()
   ├─ KillTimer(CHKID)
   ├─ 자식 창 정리
   ├─ CMDIFrameWnd::OnDestroy()
       ↓
4. CAxisApp::ExitInstance()
   ├─ if (m_pDispOM) delete m_pDispOM
   ├─ if (m_resourceHelper) delete m_resourceHelper
   ├─ 레지스트리 설정 저장
   ├─ CoUninitialize() (COM 정리, 추정)
   ├─ AfxSocketTerm() (Winsock 정리, 추정)
   └─ return 0
       ↓
5. 메시지 루프 종료 (GetMessage 반환 0)
   └─ WinMain 반환 (프로세스 종료)
```

**우려사항**:
- CoInitialize/CoUninitialize 쌍 확인 필요
- 동적 할당 객체(m_pDispOM, m_resourceHelper)의 예외 안전성
- 워커 스레드 정리 순서 (지금 명시적 정보 없음)

---

## 주요 발견사항

### 1. 메시지 기반 아키텍처의 복잡성
- **50+ 사용자 정의 메시지** (WM_USER + offset)
- **150 메시지 핸들러** (OnTimer, OnCreate, OnClose 등)
- **흐름 추적 어려움**: PostMessage의 비동기 특성으로 인한 복잡한 제어 흐름

### 2. 스레드 모델 불명확
- CPhoneThread 외 워커 스레드 구조 미파악
- 동기화 메커니즘 분산 (CCriticalSection, std::shared_mutex 혼용)
- UI Thread Block 위험: OnTimer에서 동기 I/O 또는 장시간 연산 가능성

### 3. 전역 싱글톤 의존성
- CAxisApp, CAxInstance 등 여러 싱글톤
- 초기화/종료 순서 의존성 (CoInitialize → ... → CoUninitialize)
- 멀티스레드 환경에서 race condition 위험

### 4. COM/ActiveX 복잡도
- 여러 COM 객체 (m_pDispOM, OCX 컨트롤 등)
- 자동화(IDispatch) 인터페이스 미상세
- 스택/힙 불일치 가능성 (COM 객체 생명주기)

### 5. 메모리 관리
- 수동 delete 사용 (m_pDispOM, m_resourceHelper)
- 스마트 포인터 미사용
- 예외 경로에서 누수 위험 (try/catch 사용 38회)

### 6. 리소스 정리 불완전
- 레지스트리 핸들 정리 확인 필요
- 소켓/네트워크 연결 정리 (AfxSocketTerm 확인 필요)
- 임시 파일 정리 (압축/해제 후 cleanup)

### 7. 보안 관련 복잡도
- Ahnlab SafeTx SDK (STSDKEX_EventCallback)
- INCA 소프트웨어 래퍼
- 구 AOSS SDK (deprecated, 여전히 코드에 남아있음)

---

## 알려진 제약사항 및 기술부채

1. **VC6 업그레이드 유산**
   - 파일 구조: vcxproj에 `UpgradeFromVC60.props` import
   - 레거시 코드 스타일: MFC ClassWizard 패턴

2. **스택 예약 크기 (Debug)**
   - StackReserveSize=2097152 (2MB) 명시
   - 목적: 깊은 재귀/대용량 로컬 배열 처리
   - 위험: 스택 오버플로우 가능성 여전히 높음

3. **디버그 정보 저장**
   - PDB 경로: D:\symbol\axis.pdb (절대 경로)
   - Post-Build: symstore.exe로 기호 저장
   - 빌드 머신 환경 종속성 (D:\ 드라이브 가정)

4. **멀티 바이트 문자셋 (MBCS)**
   - CharacterSet: MultiByte (CP949/EUC-KR 가정)
   - 유니코드 마이그레이션 미실시

5. **코드 분석 설정**
   - Release: CodeAnalysis 비활성화
   - Debug: RunCodeAnalysis=false
   - 정적 분석 미실시 (Clang-Tidy, CppCheck 등)

---

## 성능/스케일 특성

### 폴링 기반 아키텍처
- **타이머 간격**: 2초 (CHKTIME=2000ms)
- **메시지 큐 부하**: 높음 (150 핸들러 × 폴링 빈도)
- **UI Thread 응답성**: 폴링 콜백에서 장시간 작업 시 블로킹

### 메모리 사용
- **동적 할당**: 각 다이얼로그 생성 시 힙 할당
- **컨트롤 수**: 50+ 다이얼로그 × 평균 10+ 컨트롤/다이얼로그 = 500+ 윈도우 객체
- **캐시**: 미상세 (레지스트리 캐싱 가정)

### 네트워크 I/O
- **Winsock 기반**: wininet.lib 사용 (HTTP/FTP)
- **비동기 vs 동기**: OnTimer 콜백에서 동기 I/O 가능성 (블로킹 위험)

---

## 권장 분석 우선순위

1. **(높음)** MainFrm.cpp 메시지 핸들러 전체 분석 → 제어 흐름 맵 작성
2. **(높음)** 전역 싱글톤 (CAxisApp, CAxInstance) 초기화/종료 코드 추적
3. **(중간)** CCriticalSection 사용 분포 및 동기화 모범 사례 검토
4. **(중간)** COM/ActiveX 객체 생명주기 (CoInitialize/CoUninitialize 쌍 확인)
5. **(중간)** 워커 스레드 구조 (CPhoneThread, 기타 스레드) 상세 분석
6. **(낮음)** 레거시 코드 (AOSS SDK, VC6 패턴) 정리/제거

---

## 참고 문서

- 파일 목록: `docs/SourceIndex.md` (분석 예정)
- 의존성 맵: `docs/Dependency.md`
- 구조 리뷰: `docs/ArchitectureReview.md`
- 호출 흐름: `docs/CallGraph.md` (분석 예정)
- 지식베이스: `docs/KnowledgeBase.md`

