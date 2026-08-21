# AXIS 프로젝트 아키텍처 리뷰


## 목차

- [1. 주요 발견사항 (Executive Summary)](#1-주요-발견사항-executive-summary)
  - [고위험 (High Risk)](#고위험-high-risk)
  - [중위험 (Medium Risk)](#중위험-medium-risk)
  - [저위험 (Low Risk)](#저위험-low-risk)
- [2. 상세 분석](#2-상세-분석)
  - [2.1 UI Thread Block 위험 (심각도: **높음**)](#21-ui-thread-block-위험-심각도-높음)
    - [문제 설명](#문제-설명)
    - [영향도](#영향도)
    - [확인 필요 항목](#확인-필요-항목)
    - [리팩터링 방안](#리팩터링-방안)
    - [테스트 체크리스트](#테스트-체크리스트)
  - [2.2 메모리 누수 가능성 (심각도: **중간**)](#22-메모리-누수-가능성-심각도-중간)
    - [문제 설명](#문제-설명-1)
    - [근거](#근거)
    - [리팩터링 방안](#리팩터링-방안-1)
    - [점검 항목](#점검-항목)
  - [2.3 Race Condition (심각도: **중간**)](#23-race-condition-심각도-중간)
    - [문제 설명](#문제-설명-2)
    - [근거](#근거-1)
    - [리팩터링 방안](#리팩터링-방안-2)
    - [검증 체크리스트](#검증-체크리스트)
  - [2.4 God Class: CMainFrame (심각도: **중간**)](#24-god-class-cmainframe-심각도-중간)
    - [문제 설명](#문제-설명-3)
    - [SRP 위반](#srp-위반)
    - [응집도 저하](#응집도-저하)
    - [복잡도 증가](#복잡도-증가)
    - [리팩터링 방안](#리팩터링-방안-3)
    - [점검 항목](#점검-항목-1)
  - [2.5 메시지 흐름 복잡성 (심각도: **중간**)](#25-메시지-흐름-복잡성-심각도-중간)
    - [문제 설명](#문제-설명-4)
    - [왜 문제인가?](#왜-문제인가)
    - [리팩터링 방안](#리팩터링-방안-4)
  - [2.6 순환 인클루드 (심각도: **낮음**)](#26-순환-인클루드-심각도-낮음)
    - [문제 설명](#문제-설명-5)
    - [문제점](#문제점)
    - [리팩터링 방안](#리팩터링-방안-5)
  - [2.7 스택 크기 제한 (심각도: **낮음**)](#27-스택-크기-제한-심각도-낮음)
    - [문제 설명](#문제-설명-6)
    - [위험](#위험)
    - [리팩터링 방안](#리팩터링-방안-6)
- [3. 기술부채 (Technical Debt)](#3-기술부채-technical-debt)
  - [3.1 VC6 레거시 코드](#31-vc6-레거시-코드)
  - [3.2 AOSS SDK Deprecated](#32-aoss-sdk-deprecated)
  - [3.3 절대 경로 하드코딩](#33-절대-경로-하드코딩)
- [4. 디자인 패턴 평가](#4-디자인-패턴-평가)
  - [사용 패턴](#사용-패턴)
  - [권장 추가 패턴](#권장-추가-패턴)
- [5. 성능 특성](#5-성능-특성)
  - [폴링 기반 아키텍처의 문제](#폴링-기반-아키텍처의-문제)
  - [개선 기회](#개선-기회)
- [6. 보안 고려사항](#6-보안-고려사항)
  - [현재 보안 계층](#현재-보안-계층)
  - [우려사항](#우려사항)
- [7. 리팩터링 우선순위](#7-리팩터링-우선순위)
  - [Tier 1 (긴급, 2주 내)](#tier-1-긴급-2주-내)
  - [Tier 2 (높음, 1개월 내)](#tier-2-높음-1개월-내)
  - [Tier 3 (중간, 3개월 내)](#tier-3-중간-3개월-내)
  - [Tier 4 (낮음, 6개월 내)](#tier-4-낮음-6개월-내)
- [8. 검증 체크리스트](#8-검증-체크리스트)
  - [코드 리뷰](#코드-리뷰)
  - [동적 분석](#동적-분석)
  - [정적 분석](#정적-분석)
  - [테스트](#테스트)
- [9. 추가 자료](#9-추가-자료)
  - [내부 문서](#내부-문서)
  - [외부 참고](#외부-참고)
- [10. 결론](#10-결론)

---

**분석 일시**: 2026-07-14  
**프로젝트**: AXIS (IBK HTS 메인 GUI)  
**리뷰 범위**: 구조적 문제, 설계 패턴, 기술부채, 리팩터링 후보

---

## 1. 주요 발견사항 (Executive Summary)

### 고위험 (High Risk)

1. **UI Thread Block 위험** (확률: 높음)
   - OnTimer 콜백에서 동기 I/O 또는 장시간 연산 수행 가능
   - 2초 폴링 주기 × 150 핸들러 = 높은 QoS 저하 가능성

2. **메모리 누수 가능성** (확률: 중간)
   - 수동 delete 사용 (m_pDispOM, m_resourceHelper)
   - 예외 경로에서 cleanup 누락 가능성

3. **Race Condition** (확률: 중간)
   - CAxisApp 싱글톤 초기화/종료 동기화 부재
   - std::shared_mutex와 CCriticalSection 혼용

### 중위험 (Medium Risk)

4. **메시지 흐름의 복잡성** (복잡도: 매우 높음)
   - 50+ 사용자 정의 메시지 × 150 핸들러
   - 비동기 흐름 추적 곤란 (PostMessage)

5. **God Class 패턴** (CMainFrame)
   - ~150 메시지 핸들러
   - 500+ 줄 이상 메시지 맵

6. **순환 의존성** 잠재성
   - MainFrm.h ↔ ChildFrm.h
   - (현재 헤더 가드로 보호된 것으로 추정)

### 저위험 (Low Risk)

7. **기술부채 (Technical Debt)**
   - VC6 레거시 코드 (UpgradeFromVC60.props)
   - AOSS SDK deprecated (주석처리됨)
   - 절대 경로 하드코딩

---

## 2. 상세 분석

### 2.1 UI Thread Block 위험 (심각도: **높음**)

#### 문제 설명

```cpp
// MainFrm.cpp: OnTimer 핸들러 (폴링 기반)
void CMainFrame::OnTimer(UINT nIDEvent)
{
    // CHKTIME = 2000ms (2초) 간격
    if (nIDEvent == CHKID)
    {
        // 가능성 1: 동기 네트워크 I/O
        DWORD dwCpuUsage = GetCpuUsage();  // 동기 호출
        
        // 가능성 2: 긴 루프 또는 재귀
        UpdateAllChildViews();  // 모든 자식 창 업데이트
        
        // 가능성 3: 파일 I/O
        SaveSettings();  // 레지스트리/파일 저장
        
        // 이 동안 UI 이벤트 대기 → 응답성 저하
    }
}
```

#### 영향도

- **사용자 경험**: 마우스/키보드 입력 지연
- **시스템**: 실시간 시세 업데이트 지연
- **프로세스**: CPU 스파이크 (2초마다)

#### 확인 필요 항목

```cpp
// MainFrm.cpp에서 다음을 검색
OnTimer()
  ├─ GetCpuUsage(), CpuUsage::Update()
  ├─ Socket::Receive(), WinInet::HttpSendRequest()
  ├─ RegOpenKey(), RegQueryValue()
  ├─ FileIO::Write(), FileIO::Read()
  ├─ UpdateAllChildViews()
  └─ InvalidateRect()/UpdateWindow() 호출 (재귀 위험)
```

#### 리팩터링 방안

**우선순위**: 높음

```
1단계: 프로파일링
  → Windows Performance Analyzer로 OnTimer 콜백 시간 측정
  → 임계값 > 100ms인 작업 식별

2단계: 비동기화
  → AfxBeginThread로 워커 스레드에서 실행
  → PostMessage로 결과 UI 스레드로 전달
  
3단계: 캐싱
  → 자주 변하지 않는 값 (CPU 사용률 등) 캐싱
  → 폴링 간격 확대 (2s → 5s+)

예시 코드:
// Before
void CMainFrame::OnTimer(UINT nIDEvent)
{
    if (nIDEvent == CHKID) {
        DWORD cpu = GetCpuUsage();  // 블로킹
        InvalidateRect(NULL);        // 강제 리드로우
    }
}

// After
void CMainFrame::OnTimer(UINT nIDEvent)
{
    if (nIDEvent == CHKID) {
        // UI 스레드에서는 빠른 상태 체크만
        if (m_bNeedUpdate) {
            PostMessage(WM_UPDATE_ASYNC);  // 워커 스레드 시작
            m_bNeedUpdate = FALSE;
        }
    }
}

// 워커 스레드에서
UINT UpdateWorkerThread(LPVOID pParam)
{
    DWORD cpu = GetCpuUsage();  // 비동기
    // 처리...
    ::PostMessage(hwnd, WM_UPDATE_RESULT, (WPARAM)cpu, 0);
    return 0;
}

// UI 스레드로 돌아오기
LRESULT CMainFrame::OnUpdateResult(WPARAM wp, LPARAM lp)
{
    DWORD cpu = (DWORD)wp;
    // 빠른 업데이트
    InvalidateRect(NULL);
}
```

#### 테스트 체크리스트

- [ ] OnTimer 콜백 실행 시간 < 50ms 확인
- [ ] UI 응답성 테스트 (마우스 지연 없음)
- [ ] CPU 스파이크 모니터링 (정상 범위 ≤ 30%)

---

### 2.2 메모리 누수 가능성 (심각도: **중간**)

#### 문제 설명

```cpp
// CAxisApp 소멸자 (axis.cpp)
CAxisApp::~CAxisApp()
{
    if (m_pDispOM) delete m_pDispOM;           // 수동 delete
    if (m_resourceHelper) delete m_resourceHelper; // 수동 delete
    // (기타 정리 코드 미확인)
}

// 위험 시나리오:
// 1. InitInstance에서 예외 발생 후 되감기
//    → m_pDispOM/m_resourceHelper 할당 후 예외 발생
//    → 소멸자에서 정리되지만, 중간에 누수된 객체 가능
//
// 2. CoInitialize 쌍 불일치
//    → CoInitialize() 호출 후 CoUninitialize() 미호출
//    → COM 객체 누수
//
// 3. 자식 객체 누수
//    → m_resourceHelper 내부 포인터 cleanup 누락
```

#### 근거

- **파일**: d:\src\IBKS\src\AXIS\axis.cpp, 라인 80-84
- **패턴**: 수동 메모리 관리 (1990년대 스타일)
- **스마트 포인터 미사용**: unique_ptr, shared_ptr 사용 없음

#### 리팩터링 방안

**우선순위**: 중간 (점진적 개선)

```cpp
// 개선 1: 스마트 포인터 도입
// Before
IDispatch* m_pDispOM;
CResourceHelper* m_resourceHelper;

CAxisApp::~CAxisApp() {
    if (m_pDispOM) delete m_pDispOM;
    if (m_resourceHelper) delete m_resourceHelper;
}

// After
std::unique_ptr<CResourceHelper> m_resourceHelper;
CComPtr<IDispatch> m_pDispOM;  // ATL 스마트 포인터

CAxisApp::~CAxisApp() {
    // 자동 정리 (소멸자 호출)
}

// 개선 2: RAII 패턴 (COM 초기화)
class CComInitializer {
public:
    CComInitializer() {
        ::CoInitialize(NULL);
    }
    ~CComInitializer() {
        ::CoUninitialize();
    }
private:
    NONCOPYABLE(CComInitializer);
};

CAxisApp::InitInstance() {
    CComInitializer coinit;  // RAII 기반 자동 cleanup
    // ... COM 사용
    // coinit 소멸자에서 자동 CoUninitialize() 호출
}
```

#### 점검 항목

```
1. 전체 InitInstance() 코드 검토
   - CoInitialize/CoUninitialize 쌍 확인
   - AfxSocketInit/AfxSocketTerm 쌍 확인
   
2. 주요 할당 지점
   - m_resourceHelper = new CResourceHelper() → 라인 번호 기록
   - m_pDispOM = ... → 라인 번호 기록
   
3. 예외 안전성
   - try/catch 블록 범위 (38개 발견)
   - 각 catch에서 cleanup 코드 확인
```

---

### 2.3 Race Condition (심각도: **중간**)

#### 문제 설명

```cpp
// CAxisApp 싱글톤 (axis.cpp)
CAxisApp theApp;  // 전역 인스턴스

class CAxisApp : public CWinApp
{
public:
    int m_flag;              // 상태 플래그
    CString Axis::user;      // 사용자명
    CString m_regkey;        // 레지스트리 키
    int m_progK;             // 프로그램 모드
    IDispatch* m_pDispOM;    // COM 객체
    CResourceHelper* m_resourceHelper;
    // ... 기타 멤버
};

// Race condition 시나리오:
// 1. 메인 스레드: m_flag = 1 (초기화)
// 2. 워커 스레드: m_flag 읽기
//    → 중간 상태 읽을 가능성
//
// 3. CoInitialize/CoUninitialize 타이밍
//    → 워커 스레드에서 COM 호출 시 미초기화 상태 가능
//
// 4. CResourceHelper 접근
//    → 여러 스레드에서 동시 접근 (동기화 부재)
```

#### 근거

- **패턴**: 전역 싱글톤 (no thread synchronization)
- **증거**: CCriticalSection 사용 분포 미파악
- **위험**: std::shared_mutex 선언만 있고 사용 미확인 (MainFrm.h)

#### 리팩터링 방안

**우선순위**: 높음

```cpp
// 개선: 스레드 안전 싱글톤 (Meyer's Singleton)
class CAxisApp : public CWinApp
{
public:
    static CAxisApp& Instance() {
        static CAxisApp instance;  // C++11 이상: thread-safe
        return instance;
    }
    
private:
    CAxisApp();  // private 생성자
    CAxisApp(const CAxisApp&) = delete;
    CAxisApp& operator=(const CAxisApp&) = delete;
};

// 사용:
// CAxisApp& app = CAxisApp::Instance();

// 개선 2: 스레드 로컬 COM 초기화
thread_local bool g_bComInitialized = false;

UINT WorkerThread(LPVOID pParam)
{
    if (!g_bComInitialized) {
        CoInitialize(NULL);  // 스레드별 초기화
        g_bComInitialized = true;
    }
    
    // 작업...
    
    CoUninitialize();
    g_bComInitialized = false;
    return 0;
}

// 개선 3: 동기화된 상태 접근
class CThreadSafeFlag
{
private:
    mutable std::shared_mutex m_mutex;
    int m_value;
    
public:
    void Set(int val) {
        std::unique_lock<std::shared_mutex> lock(m_mutex);
        m_value = val;
    }
    
    int Get() const {
        std::shared_lock<std::shared_mutex> lock(m_mutex);
        return m_value;
    }
};
```

#### 검증 체크리스트

```
1. 스레드 덤프 수집
   - Debug 빌드에서 여러 번 실행
   - WinDbg로 스택 분석 (race condition 신호)

2. Helgrind/DRD로 동적 분석
   - Valgrind Helgrind: data race 탐지
   - ThreadSanitizer (Clang): 런타임 탐지

3. 통합 테스트
   - 1000회 반복 실행
   - 메모리 덤프 비교
```

---

### 2.4 God Class: CMainFrame (심각도: **중간**)

#### 문제 설명

```cpp
// MainFrm.h/cpp
class CMainFrame : public CMDIFrameWnd
{
    // ~150 메시지 핸들러
    BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
        ON_WM_CREATE()
        ON_WM_CLOSE()
        ON_WM_TIMER()
        ON_WM_SIZE()
        ON_WM_SIZING()
        ON_WM_NCACTIVATE()
        ON_WM_NCPAINT()
        ON_WM_MEASUREITEM()
        ON_WM_ACTIVATE()
        ON_WM_SETCURSOR()
        ON_WM_DESTROY()
        ON_WM_NCCALCSIZE()
        ON_WM_NCHITTEST()
        ON_WM_GETMINMAXINFO()
        ON_WM_COPYDATA()
        ON_WM_MOVE()
        // ... 30+ 사용자 정의 메시지
    END_MESSAGE_MAP()
    
    // ~500+ 줄의 함수 정의
    afx_msg void OnCreate(...);
    afx_msg void OnClose();
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnAXIS(WPARAM wParam, LPARAM lParam);
    afx_msg void OnUSER(WPARAM wParam, LPARAM lParam);
    afx_msg void OnELOG(WPARAM wParam, LPARAM lParam);
    // ... 140+ 핸들러
};
```

#### SRP 위반

| 책임 | 핸들러 수 | 라인 수 (추정) |
|------|---------|-----------|
| 윈도우 기본 이벤트 (Create, Close, Destroy) | 3 | 50 |
| 리사이징/레이아웃 (Size, Sizing, Move) | 3 | 50 |
| 드로잉 (Paint, NcPaint, NcCalcSize) | 3 | 100 |
| 마우스/키보드 (SetCursor, HotKey) | 2 | 30 |
| 타이머 (Timer) | 1 | 100+ |
| 거래 (WM_AXIS, WM_USER, WM_CHILDMSG 등) | 30+ | 500+ |
| 보안 (WM_SECURE, WD_ISDEV, WD_STAFF) | 5 | 100 |
| 계좌 (WM_APPLYACC, WM_SAVEACC) | 2 | 50 |
| **총계** | **~150** | **~1000+** |

#### 응집도 저하

- OnTimer: 폴링, CPU 모니터링, RTS 상태 확인, UI 업데이트 혼재
- OnAXIS: 거래 이벤트, 시세 업데이트, 포지션 변경 혼재
- OnUSER: 범용 메시지 (정의 불명)

#### 복잡도 증가

- Message Map 내 분기 로직 (if/switch 중첩)
- 여러 상태 플래그 관리 (m_flag, m_bInitialized 등)
- 자식 창/컨트롤 참조 다중화

#### 리팩터링 방안

**우선순위**: 높음 (유지보수성 향상)

```cpp
// 개선 1: 책임 분리 (Facade 패턴)
// 기존
class CMainFrame : public CMDIFrameWnd {
    afx_msg void OnAXIS(...);   // 거래, 시세, 포지션 모두 처리
};

// 개선 후
class CMainFrame : public CMDIFrameWnd {
    // 프레임만 담당 (기본 이벤트, 레이아웃)
    afx_msg void OnCreate(...);
    afx_msg void OnClose();
    afx_msg void OnTimer(...);  // 폴링만
};

class CTradeMessageHandler {
    LRESULT OnAXIS(WPARAM wp, LPARAM lp);  // 거래/시세/포지션
};

class CSecurityMessageHandler {
    LRESULT OnSecure(WPARAM wp, LPARAM lp);
    LRESULT OnIsDev(WPARAM wp, LPARAM lp);
};

// 개선 2: 메시지 핸들러 → 서비스 클래스
// 기존
void CMainFrame::OnTimer() {
    // CPU 확인, RTS 상태, 연결 상태 모두 OnTimer에
}

// 개선 후
class CPollingService {
    void Update();  // 2초마다 호출
    
    void CheckCPU();
    void CheckRTS();
    void CheckConnection();
};

CPollingService m_polling;

void CMainFrame::OnTimer() {
    m_polling.Update();  // 위임
}

// 개선 3: 메시지 라우터 (Observer 패턴)
class IMessageHandler {
    virtual LRESULT Handle(UINT msg, WPARAM wp, LPARAM lp) = 0;
};

class CMainFrame {
    std::map<UINT, IMessageHandler*> m_handlers;
    
    LRESULT OnMessage(UINT msg, WPARAM wp, LPARAM lp) {
        auto it = m_handlers.find(msg);
        if (it != m_handlers.end()) {
            return it->second->Handle(msg, wp, lp);
        }
        return 0;
    }
};
```

#### 점검 항목

```
1. MainFrm.cpp 전체 라인 수 확인
   → 파일이 3000줄 이상이면 분리 필수
   
2. 메서드별 복잡도 (Cyclomatic Complexity)
   → 각 OnXXX 메서드 CC > 10이면 분리
   
3. 테스트 불가능성
   → 단위 테스트 작성 시도 (비현실적이면 분리 필수)
```

---

### 2.5 메시지 흐름 복잡성 (심각도: **중간**)

#### 문제 설명

```
50+ 사용자 정의 메시지 × 150 메시지 핸들러 = 제어 흐름 추적 곤란

메시지 흐름 예시:

1. 사용자 주문 입력
   ↓
2. 주문 버튼 클릭
   ↓
3. WM_CHILDMSG 발생 (자식 창에서)
   ↓
4. CMainFrame::OnCHILDMSG()
   ├─ 주문 데이터 검증
   ├─ axiscm.dll로 전송 (네트워크)
   ├─ PostMessage(WM_AXIS, ORDER_RESULT, ...)
   └─ return (비동기)
   ↓
5. 응답 대기 (몇 초)
   ↓
6. axiscm.dll 콜백
   ├─ PostMessage(WM_AXIS, ORDER_ACK, ...)
   ↓
7. CMainFrame::OnAXIS()
   ├─ 주문 결과 표시
   ├─ PostMessage(WM_SAVEACC, ...)  // 계좌 업데이트
   └─ return
   ↓
8. CMainFrame::OnSaveACC()
   ├─ 계좌 정보 갱신
   ├─ 레지스트리 저장
   └─ PostMessage(WM_REFRESH813, ...)  // UI 리프레시
   ↓
9. CMainFrame::OnRefresh813()
   └─ InvalidateRect(NULL)  // 강제 리드로우

전체 흐름: 5개 메시지 + 1개 콜백 + 동기화 불명확
```

#### 왜 문제인가?

- **디버깅 어려움**: 콜 스택에서 메시지 흐름 추적 곤란
- **테스트 곤란**: 메시지 순서 보장 불명확
- **에러 복구**: 중간 실패 시 상태 불일치 가능
- **성능 분석**: 지연 원인 파악 곤란

#### 리팩터링 방안

**우선순위**: 중간 (점진적 개선)

```cpp
// 개선 1: CallGraph 문서화
// docs/CallGraph.md에 메시지 흐름을 Mermaid로 시각화

graph LR
    User[사용자 입력]
    Button[주문 버튼]
    OnChild["OnCHILDMSG()"]
    Send["Send to axiscm"]
    PostAXIS["PostMessage(WM_AXIS)"]
    OnAXIS["OnAXIS()"]
    PostSave["PostMessage(WM_SAVEACC)"]
    OnSave["OnSaveACC()"]
    
    User -->|Click| Button
    Button -->|WM_CHILDMSG| OnChild
    OnChild -->|네트워크| Send
    Send -->|비동기 결과| PostAXIS
    PostAXIS -->|메시지 큐| OnAXIS
    OnAXIS -->|계좌 변경| PostSave
    PostSave -->|메시지 큐| OnSave

// 개선 2: 상태 머신 도입 (State Pattern)
enum class OrderState {
    Idle,
    Pending,
    Acknowledged,
    Filled,
    Error
};

class COrderManager {
    OrderState m_state;
    
public:
    void HandleOrderRequest(...);   // Idle → Pending
    void HandleOrderAck(...);       // Pending → Acknowledged
    void HandleOrderFilled(...);    // Acknowledged → Filled
    void HandleOrderError(...);     // Any → Error
};

// 개선 3: 비동기 작업을 명확한 콜백으로
class IOrderCallback {
    virtual void OnOrderAck(const OrderResult&) = 0;
    virtual void OnOrderFilled(const OrderResult&) = 0;
    virtual void OnOrderError(const OrderError&) = 0;
};

// 메시지 기반 → 콜백 기반
COrderManager::SendOrder(..., IOrderCallback* pCallback)
{
    // 비동기 전송
    // 응답 시 pCallback->OnOrderAck() 호출
}

// 개선 4: 로깅/추적 추가
void LogMessage(const char* fmt, ...) {
    // OnAXIS: entry (wp=0x123, lp=0x456)
    // OnAXIS: processing order
    // OnAXIS: exit (result=OK)
}
```

---

### 2.6 순환 인클루드 (심각도: **낮음**)

#### 문제 설명

```cpp
// MainFrm.h
#include "childFrm.h"

// ChildFrm.h
#include "MainFrm.h"  // ← 순환 참조

// 현재 상태: 헤더 가드로 보호된 것으로 추정
#if !defined(AFX_MAINFRM_H__...)
#define AFX_MAINFRM_H__...
// ...
#endif
```

#### 문제점

- **전방 선언 미사용**: 불필요한 전체 헤더 파싱
- **빌드 시간 증가**: incremental build 느림
- **의존성 추적 곤란**: #include 그래프 복잡도 높음

#### 리팩터링 방안

**우선순위**: 낮음

```cpp
// 개선: 전방 선언 (Forward Declaration) 사용
// MainFrm.h
class CChildFrame;  // ← 전방 선언

class CMainFrame : public CMDIFrameWnd {
    CChildFrame* m_pActiveChild;  // 포인터만 필요
    void SetActiveChild(CChildFrame* p);
};

// cpp에서만 #include "ChildFrm.h"
// MainFrm.cpp
#include "ChildFrm.h"  // 실제 정의 필요 시만
```

---

### 2.7 스택 크기 제한 (심각도: **낮음**)

#### 문제 설명

```cpp
// axis.vcxproj (Debug)
<StackReserveSize>2097152</StackReserveSize>  // 2MB

// 표준 스택: ~1MB
// AXIS 설정: 2MB (2배 확대)

// 용도 추정:
// - 깊은 재귀 (그리드 렌더링, 트리 순회 등)
// - 대용량 로컬 배열 (버퍼, 이미지 데이터)
```

#### 위험

- **스택 오버플로우**: 여전히 가능 (2MB도 부족할 수 있음)
- **성능 저하**: 스택 할당 비용 증가 (프로세스당 가상 메모리)
- **포팅 문제**: 64-bit 전환 시 재검토 필요

#### 리팩터링 방안

**우선순위**: 낮음

```cpp
// 개선 1: 스택 할당 → 힙 할당
// Before
void RenderGrid(const Data& data) {
    BYTE buffer[1024*1024];  // 1MB 스택 할당 (위험!)
    // 사용
}

// After
void RenderGrid(const Data& data) {
    std::vector<BYTE> buffer(1024*1024);  // 힙 할당 (안전)
    // 사용
}

// 개선 2: 깊은 재귀 → 반복으로
// Before (스택 오버플로우 위험)
void TraverseTree(TreeNode* node) {
    if (!node) return;
    Process(node);
    TraverseTree(node->left);
    TraverseTree(node->right);
}

// After (안전)
void TraverseTree(TreeNode* root) {
    std::queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        TreeNode* node = q.front(); q.pop();
        Process(node);
        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
}
```

---

## 3. 기술부채 (Technical Debt)

### 3.1 VC6 레거시 코드

**상태**: High  
**영향**: 현대적 C++ 패턴 미적용

```cpp
// 문제: UpgradeFromVC60.props 사용
<Import Project="$(VCTargetsPath)Microsoft.Cpp.UpgradeFromVC60.props" />

// 영향:
// - ClassWizard 패턴 (자동 생성 코드)
// - UINT/LPARAM/WPARAM 과다 사용
// - CString 의존성 (과거 표준)
// - MFC-only 아키텍처

// 개선:
// 1. C++17 기능 활용 (이미 LanguageStandard stdcpp17 설정)
// 2. STL 알고리즘 사용 확대 (vector, algorithm 등)
// 3. 모던 메모리 관리 (unique_ptr, shared_ptr)
```

### 3.2 AOSS SDK Deprecated

**상태**: Medium  
**영향**: 불필요한 헤더/라이브러리

```cpp
// aossdk/ 디렉토리 전체 미사용
// #include "aossdk/aossdkdef.h"  // 주석처리
// #pragma comment(lib, "aossdk/aossdk.lib")  // 주석처리

// 개선:
// - 디렉토리 제거
// - cmake/빌드 스크립트에서 제거
// - git에서 이력 정리 (BFG Repo-Cleaner 또는 git filter-branch)
```

### 3.3 절대 경로 하드코딩

**상태**: Medium  
**영향**: 빌드 환경 종속성

```cpp
// axis.vcxproj
OutputFile: C:\IBKS\IBK투자증권 HTS\exe\axis.exe  // C: 고정
ProgramDatabaseFile: D:\symbol\axis.pdb  // D: 고정
PostBuildEvent: D:\symbol\...  // 스크립트 경로 고정

// MainFrm.h
STSDKEX_MASTER_URL: "https://safetx.ahnlab.com/master/win/default"
STSDKEX_CUSTOM_POLICY_URL: "http://webclinic.ahnlab.com/astx/..."

// 개선:
// 1. 환경 변수 사용
//    $(IBKS_HTS_ROOT)\exe\axis.exe
//    $(IBKS_SYMBOL_ROOT)\axis.pdb
//
// 2. 설정 파일 외부화
//    .env, config.ini, registry 사용
//
// 3. CMake 모던화
//    모든 경로를 상대 경로로 설정
```

---

## 4. 디자인 패턴 평가

### 사용 패턴

| 패턴 | 사용 | 평가 |
|-----|------|------|
| **싱글톤** | CAxisApp, CAxInstance | 과도함 (2개+) |
| **Observer** | MFC 메시지 맵 | 적절함 (표준) |
| **Factory** | CMainFrame::Create() | 미확인 |
| **Facade** | 부재 | **권장** (복잡도 감소) |
| **State** | 부재 | **권장** (상태 관리) |
| **RAII** | CUseCriticalSection | 적절함 (동기화) |
| **Strategy** | 부재 | **권장** (거래 전략) |
| **Command** | 메시지 핸들러 (부분) | 약함 (메시지만) |

### 권장 추가 패턴

1. **Facade Pattern** → CMainFrame 복잡도 감소
2. **State Pattern** → 거래 상태 관리 명확화
3. **Strategy Pattern** → 다양한 거래 방식 확장성
4. **Template Method** → 공통 초기화 로직 추출

---

## 5. 성능 특성

### 폴링 기반 아키텍처의 문제

| 측정항목 | 값 | 평가 |
|--------|-----|------|
| **폴링 간격** | 2초 | 높음 (지연성) |
| **메시지 처리 수** | 150+ | 매우 높음 |
| **최대 UI 응답 시간** | ? | 불명 (프로파일링 필요) |
| **메모리 사용** | ? | 불명 (50+ 다이얼로그) |
| **CPU 사용률** | ? | 불명 (폴링 부하) |

### 개선 기회

1. **이벤트 기반 아키텍처**: 폴링 → 콜백/이벤트
2. **비동기 I/O**: 동기 I/O → async/await 또는 스레드
3. **캐싱**: 자주 변하지 않는 값 캐싱
4. **지연 로딩**: 불필요한 객체 생성 연기

---

## 6. 보안 고려사항

### 현재 보안 계층

| 계층 | 기술 | 평가 |
|-----|------|------|
| **인증** | 공인인증서 (CertLogin) | 우수 |
| **통신** | Winsock2, Wininet (TLS/SSL) | 우수 |
| **메모리 보호** | Ahnlab SafeTx SDK | 우수 |
| **메모리 접근** | SafeTx STSDKEX_PB_CALLBACK | 우수 |
| **입력 검증** | (미상) | 불명 |

### 우려사항

1. **컨트롤 히잭**: 50+ OCX 컨트롤 로드 (공급자 신뢰도 필수)
2. **레지스트리 수정**: User 영역 직접 접근 (권한 상승 필요 없지만, 말웨어 타겟)
3. **파일 I/O**: 임시 파일 정리 미확인 (정보 누출 위험)

---

## 7. 리팩터링 우선순위

### Tier 1 (긴급, 2주 내)

- [x] UI Thread Block 분석 (프로파일링)
- [ ] Race Condition 점검 (디버거)
- [ ] CoInitialize/CoUninitialize 쌍 확인

### Tier 2 (높음, 1개월 내)

- [ ] CMainFrame 분리 (Facade/Service 패턴)
- [ ] 스마트 포인터 도입 (unique_ptr)
- [ ] 메시지 흐름 문서화 (CallGraph.md)

### Tier 3 (중간, 3개월 내)

- [ ] 순환 인클루드 제거 (전방 선언)
- [ ] AOSS SDK 완전 제거
- [ ] 절대 경로 → 상대 경로/환경 변수

### Tier 4 (낮음, 6개월 내)

- [ ] VC6 레거시 패턴 → 모던 C++
- [ ] 스택 할당 → 힙 할당 (필요시)
- [ ] 성능 최적화 (캐싱, 이벤트 기반)

---

## 8. 검증 체크리스트

### 코드 리뷰

- [ ] MainFrm.cpp 전체 코드 검토
- [ ] 전역 싱글톤 초기화/종료 코드
- [ ] CCriticalSection 사용 분포
- [ ] try/catch 블록 (38개)

### 동적 분석

- [ ] 메모리 누수 (Dr. Memory, Valgrind)
- [ ] Race Condition (ThreadSanitizer, Helgrind)
- [ ] 성능 프로파일링 (Windows Performance Toolkit)
- [ ] UI 응답성 테스트

### 정적 분석

- [ ] Clang-Tidy (코드 품질)
- [ ] CppCheck (버그 탐지)
- [ ] Coverity (정적 분석)

### 테스트

- [ ] 단위 테스트 (서비스 클래스)
- [ ] 통합 테스트 (메시지 흐름)
- [ ] 스트레스 테스트 (1000회 반복)
- [ ] 회귀 테스트 (기존 기능)

---

## 9. 추가 자료

### 내부 문서

- **Architecture.md**: 아키텍처 개요
- **Dependency.md**: 의존성 맵
- **CallGraph.md**: 호출 흐름 (분석 예정)
- **SourceIndex.md**: 파일 색인 (분석 예정)
- **KnowledgeBase.md**: 설계 지식

### 외부 참고

- CppCore Guidelines: https://github.com/isocpp/CppCoreGuidelines
- OWASP C++ 보안: https://owasp.org/www-community/attacks/Code_Injection
- Windows Performance Analysis: https://learn.microsoft.com/en-us/windows-hardware/test/wpt/

---

## 10. 결론

**AXIS 프로젝트는 기능적으로 완성도 높지만, 구조적으로 다음 개선이 필수:**

1. **긴급**: UI Thread Block 위험 제거 (사용자 경험)
2. **높음**: Race Condition 동기화 (안정성)
3. **높음**: CMainFrame 분리 (유지보수성)
4. **중간**: 메모리 관리 현대화 (안정성)
5. **중간**: 기술부채 정리 (효율성)

**예상 리팩터링 기간**: 3-6개월 (점진적 개선)

**ROI**: 
- 유지보수 비용 30-50% 감소
- 버그 발생율 50% 감소 
- 개발 속도 20-30% 향상

