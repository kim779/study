---
project: ibks
category: architecture
status: 작성됨
updated: 2026-08-28
---

# C_Total 아키텍처

## 목차

- [개요](#개요)
- [모듈 구조](#모듈-구조)
- [계층 설계](#계층-설계)
- [주요 클래스 및 책임](#주요-클래스-및-책임)
- [메시지 흐름](#메시지-흐름)
- [데이터 관리](#데이터-관리)
- [문제점 및 개선 대상](#문제점-및-개선-대상)

---

## 개요

**C_Total**은 MFC 기반의 OLE/COM 차트 컨트롤 DLL로, 다양한 금융 차트 타입(일/주/월 등)을 지원하는
동적 라이브러리입니다.

- **출력물**: `C_TOTAL.DLL`
- **빌드 타입**: MFC Dynamic Library (MultiByte, v142 Toolset)
- **OLE 서버**: COM 객체로 등록 (`COleObjectFactory::RegisterAll()`)
- **인터페이스**: OLE Dispatch Interface (IMainWnd)

---

## 모듈 구조

### 계층도

```
┌─────────────────────────────────────┐
│    CC_TotalApp (CWinApp)            │  <- DLL Entry, OLE Server
├─────────────────────────────────────┤
│ CMainWnd (CWnd)                     │  <- OLE Dispatch Interface
│  - Panel Manager                    │
│  - Data Cache                       │
│  - File I/O                         │
├─────────────────────────────────────┤
│ Panel Subsystem                     │
│  - CPnCtrl (제어 패널)              │
│  - CPnInput (입력 패널)             │
│  - CPnWndCombo (콤보 윈도우)        │
│  - CPnBaseWnd (기반 클래스)         │
├─────────────────────────────────────┤
│ Data Management                     │
│  - CDataChk (데이터 검증)           │
│  - fileIO (파일 I/O)                │
│  - CExtraMsg (메시지 처리)          │
├─────────────────────────────────────┤
│ External Libraries                  │
│  - MFC Core                         │
│  - MFC OLE/COM                      │
│  - Windows API                      │
│  - AXIS 라이브러리                  │
└─────────────────────────────────────┘
```

### 파일 구성

| 파일명 | 역할 |
|--------|------|
| **C_Total.h/cpp** | DLL 메인 진입점, CC_TotalApp 정의 |
| **MainWnd.h/cpp** | OLE 차트 컨트롤 윈도우, 메인 로직 |
| **DataChk.h/cpp** | 데이터 검증 및 준비 |
| **fileIO.h/cpp** | 환경파일(ENV), 그룹파일(GRP), 데이터파일(DAT) 로드/저장 |
| **ExtraMsg.h/cpp** | 추가 메시지 처리 및 콜백 |
| **PnBaseWnd.h/cpp** | Panel 기반 클래스 |
| **PnCtrl.h/cpp** | 제어 패널 컨트롤 |
| **PnInput.h/cpp** | 입력 패널 |
| **PnWndCombo.h/cpp** | 콤보박스 윈도우 |
| **libTotal.cpp** | OLE 라이브러리 함수 |
| **SavedHead.cpp** | 공통 헤더 함수 (CONTROL/Common 에서 포함) |
| **MxTrace.cpp** | 트레이싱 유틸리티 |

---

## 계층 설계

### 1. **DLL Application Layer (CC_TotalApp)**

```cpp
class CC_TotalApp : public CWinApp
{
  // OLE 서버 초기화
  // 폰트, 펜, 브러시, 이미지 리스트 관리
  // 리소스 경로 관리 (Root, User, Tab, Gex 디렉토리)
};
```

**책임**:
- OLE 객체 팩토리 등록
- 전역 그래픽 리소스 (CFont, CPen, CBrush, CImageList) 관리
- 애플리케이션 초기화/종료

---

### 2. **OLE Control Layer (CMainWnd)**

```cpp
class CMainWnd : public CWnd
{
  // OLE Dispatch Interface 제공
  // Panel 생성 및 관리
  // Data 캐싱
  // File I/O
  // TR(거래) 요청 처리
};
```

**책임**:
- OLE 디스패치 메서드 구현 (GetProperties, SetProperties, RequestTR 등)
- Panel 생성, 삭제, 리사이징
- 파일 기반 환경/그룹/데이터 로드/저장
- 인덱스 변경, 차트 동기화

---

### 3. **Panel Subsystem**

**CPnBaseWnd** - 모든 Panel의 기반 클래스:
- 마우스 이벤트 처리
- 컴포넌트 드로잉 (버튼, 색상, 너비 조절 등)
- 상태 관리

**CPnCtrl** - 제어 패널:
- 컨트롤 요소 배치
- 사용자 입력 처리

**CPnInput** - 입력 패널:
- 데이터 입력 필드

**CPnWndCombo** - 콤보 윈도우:
- 드롭다운 선택

---

### 4. **Data Management Layer**

**CDataChk**:
- 종목 데이터 검증
- 환경/그룹/데이터 파일 생성
- 색상 팔레트 로드

**fileIO**:
- `.env` 파일 (환경 설정)
- `.grp` 파일 (그룹 정보)
- `.dat` 파일 (차트 데이터)

**CExtraMsg**:
- 메시지 후처리
- 콜백 처리

---

## 주요 클래스 및 책임

### CC_TotalApp

| 멤버 | 용도 |
|------|------|
| `m_strRoot` | 애플리케이션 루트 경로 |
| `m_strUserID / m_strUserName` | 사용자 정보 |
| `m_strTabDir / m_strUserDir / m_strGexDir` | 데이터 디렉토리 |
| `m_hGMainLib / m_hGDlgLib` | 동적 로드 라이브러리 핸들 |
| `m_pImg*` | 이미지 리스트 (Date, Time, Options, Ctrl 등) |

**주요 메서드**:
- `Create()` - CMainWnd 생성 반환
- `GetFont(), GetPen(), GetBrush()` - 그래픽 리소스 관리
- `GetImage()` - 이미지 리스트 반환

### CMainWnd

| 멤버 | 용도 |
|------|------|
| `m_iDtUnit / m_iDtIndex` | 데이터 단위 (일/주/월) 및 인덱스 |
| `m_pwndChart / m_pwndCtrl / m_pwndInput / m_pwndView` | 서브윈도우 포인터 |
| `m_arData / m_arLen` | 데이터 버퍼 배열 |
| `m_mapPanel` | Panel 정보 맵 |
| `m_pEnvInfo` | 환경 정보 구조체 |
| `m_pExtraMsg` | ExtraMsg 포인터 |

**주요 메서드**:
- `GetProperties() / SetProperties()` - OLE 인터페이스
- `RequestTR() / RequestTR2()` - 데이터 요청
- `ReadEnv(), ReadGrp(), ReadData()` - 파일 로드
- `WriteEnv(), WriteGrp(), WriteDat()` - 파일 저장
- `CreatePn(), DelPanels()` - Panel 관리
- `SyncMainChart(), SyncMap()` - 데이터 동기화

### CDataChk

**책임**:
- 데이터 일관성 검증
- 환경/그룹/데이터 파일 재구성 (ReMakeData)
- 색상 팔레트 초기화

### CPnBaseWnd

**책임**:
- Panel 렌더링 (DrawBackground, DrawFrame, DrawButton)
- 마우스 이벤트 처리 (OnMouseWheel, OnMouseMove, OnLButtonDown)
- 컴포넌트 검색 및 상태 관리

---

## 메시지 흐름

### OLE Dispatch 메서드 호출

```
Host Application (VB, C#)
    │
    ├─> IMainWnd::GetProperties()
    │   └─> CMainWnd::GetMapInfo() -> 문자열 반환
    │
    ├─> IMainWnd::SetProperties(BSTR str)
    │   └─> CMainWnd::SetGrpAtDat(char *info) -> Panel 재구성
    │
    ├─> IMainWnd::RequestTR(BSTR str)
    │   └─> CMainWnd::SendRequest() -> 외부 TR 요청
    │
    ├─> IMainWnd::GetSelectTime()
    │   └─> CMainWnd::GetMapInfo() 반환
    │
    └─> IMainWnd::Config()
        └─> 환경 다이얼로그 표시
```

### Windows 메시지 처리

```
CMainWnd::WndProc
    ├─> WM_CREATE -> OnCreate()
    ├─> WM_SIZE -> ResizeWnd()
    ├─> WM_PAINT -> OnPaint()
    ├─> WM_MOUSEMOVE -> Panel 에서 처리
    ├─> WM_LBUTTONDOWN -> Panel 에서 처리
    └─> Custom Message (Oub) -> 외부 이벤트 처리
```

### Panel 이벤트

```
User Input (마우스 클릭 등)
    │
    ├─> CPnBaseWnd::OnMouseMove()
    │   └─> SearchCom(CPoint) -> 컴포넌트 찾기
    │
    ├─> CPnBaseWnd::OnLButtonDown()
    │   ├─> 컴포넌트 타입별 처리
    │   └─> InvalidateCom() -> 화면 업데이트
    │
    └─> Panel 재렌더링
```

---

## 데이터 관리

### 파일 구조

```
User Directory/
├── *.env  (환경 파일)
│   └─ Panel 레이아웃, 폰트, 색상 설정
├── *.grp  (그룹 파일)
│   └─ 그래프 그룹 정보
└── *.dat  (데이터 파일)
    └─ 차트 렌더링 데이터
```

### Data Flow

```
RequestTR() (OLE 메서드)
    │
    ├─> SendRequest() / SendRequest2()
    │   └─> 외부 데이터 소스에 요청
    │
    └─> 응답 데이터
        ├─> m_arData 버퍼에 캐싱
        ├─> CDataChk::MakeDat() -> .dat 파일 생성
        └─> Panel 재렌더링 (InvalidateRect)
```

### 데이터 구조체

```cpp
struct _param {
  // 차트 설정 파라미터
};

struct _envInfo {
  // 환경 정보 (Panel 레이아웃, 폰트)
};

struct _pninfo {
  int whsize;     // Panel 높이/너비
  WORD info;      // Panel 상태 정보
};

struct _trData {
  // TR 응답 데이터
};
```

---

## 문제점 및 개선 대상

### 1. **강한 결합도**
   - CMainWnd가 모든 Panel, Data, File I/O를 직접 관리
   - Panel 생성/삭제 로직이 복잡하고 분산됨
   
   **개선 방안**: PanelFactory 패턴 도입, 책임 분리

### 2. **메모리 관리**
   - 수동 포인터 관리 (char*, CWnd* 등)
   - 동적 라이브러리 핸들 (m_hGMainLib, m_hGDlgLib) 해제 확인 필요
   
   **개선 방안**: std::unique_ptr / std::shared_ptr 전환

### 3. **파일 I/O 에러 처리**
   - ReadEnv(), ReadGrp(), ReadData() 반환값만 확인
   - 실패 시 대체 방안 불명확
   
   **개선 방안**: 명확한 예외 처리 또는 상세 로깅

### 4. **Thread 안전성**
   - 파일 I/O 중 UI 스레드 블로킹 가능
   - 동시 호출 시 m_arData 버퍼 경쟁 조건 가능성
   
   **개선 방안**: 비동기 파일 I/O, CriticalSection 도입

### 5. **외부 의존성**
   - AXIS 라이브러리 (axisgwin.h, axisvar.h, axisfire.h) 강결합
   - CONTROL/Common 모듈도 강결합
   
   **개선 방안**: Dependency Injection 또는 인터페이스 계층 추가

---

## 히스토리

- **초기 작성**: 2026-08-27
- **마지막 갱신**: 2026-08-27
