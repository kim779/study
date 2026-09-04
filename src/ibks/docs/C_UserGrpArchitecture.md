---
project: ibks
category: architecture
status: 작성됨
updated: 2026-08-28
---

# C_UserGrp 아키텍처

## 목차

- [개요](#개요)
- [모듈 구조](#모듈-구조)
- [계층 설계](#계층-설계)
- [주요 클래스 및 책임](#주요-클래스-및-책임)
- [그래프 엔진](#그래프-엔진)
- [메시지 흐름](#메시지-흐름)
- [데이터 흐름](#데이터-흐름)
- [문제점 및 개선 대상](#문제점-및-개선-대상)

---

## 개요

**C_UserGrp**는 MFC 기반의 OLE/COM 라인/바 차트 컨트롤 DLL로, 사용자 정의 그래프를 렌더링하는
동적 라이브러리입니다.

- **출력물**: `C_UserGrp.dll`
- **빌드 타입**: MFC Dynamic Library (MultiByte, v142 Toolset)
- **OLE 서버**: COM 객체로 등록 (MIDL)
- **인터페이스**: OLE Dispatch Interface (IMainWnd)
- **C++ 표준**: C++17 (std::unique_ptr, std::shared_ptr 사용)

---

## 모듈 구조

### 계층도

```
┌─────────────────────────────────────┐
│    CC_UserGrpApp (CWinApp)          │  <- DLL Entry, OLE Server
├─────────────────────────────────────┤
│ CMainWnd (CWnd)                     │  <- OLE Dispatch Interface
│  - Property/Method Bridge           │
├─────────────────────────────────────┤
│ CCtrlWnd (CWnd)                     │  <- Control Panel
├─────────────────────────────────────┤
│ CGrpWnd (CWnd)                      │  <- Graph Rendering Window
│  - Data Container                   │
│  - Rendering Manager                │
├─────────────────────────────────────┤
│ Graph Engine (CGrp_*)               │
│  - CGrp_Base (추상 기본 클래스)     │
│  - CGrp_Line (선형 차트)            │
│  - CGrp_Bar (막대 차트)             │
│  - CGrp_VBar (수직 막대 차트)       │
│  - CGrp_Data (데이터 컨테이너)      │
├─────────────────────────────────────┤
│ Utilities                           │
│  - CrossLine (십자선)               │
│  - ToolTip (도구 팁)                │
│  - Ctrl (제어 로직)                 │
├─────────────────────────────────────┤
│ External Libraries                  │
│  - MFC Core                         │
│  - Windows API                      │
│  - AXIS Libraries                   │
└─────────────────────────────────────┘
```

### 파일 구성

| 파일명 | 역할 |
|--------|------|
| **C_UserGrp.h/cpp** | DLL 메인 진입점, CC_UserGrpApp 정의 |
| **MainWnd.h/cpp** | OLE 차트 컨트롤 윈도우, Dispatch Interface |
| **GrpWnd.h/cpp** | 그래프 렌더링 윈도우, 데이터 관리 |
| **CtrlWnd.h/cpp** | 제어 패널 윈도우 |
| **Ctrl.h/cpp** | 제어 로직 |
| **Grp_Base.h/cpp** | 그래프 기본 클래스 (공통 렌더링) |
| **Grp_Line.h/cpp** | 선형 차트 구현 |
| **Grp_Bar.h/cpp** | 막대 차트 구현 |
| **Grp_VBar.h/cpp** | 수직 막대 차트 구현 |
| **Grp_Data.h/cpp** | 데이터 컨테이너/저장소 |
| **CrossLine.h/cpp** | 십자선 렌더링 |
| **ToolTip.h/cpp** | 도구 팁 표시 |
| **libctrl.cpp** | OLE 라이브러리 함수 |

---

## 계층 설계

### 1. **DLL Application Layer (CC_UserGrpApp)**

```cpp
class CC_UserGrpApp : public CWinApp {
  // C++11 std::shared_ptr 사용
  std::shared_ptr<CImageList> m_pImgCtrl;
  
  // OLE 서버 초기화
  // 폰트, 펜, 브러시 관리
  // 샘플 데이터 제공
};
```

**책임**:
- OLE 객체 팩토리 등록
- 전역 그래픽 리소스 (CFont, CPen, CBrush, CImageList) 관리
- 샘플 데이터 제공 (GetSampleData)
- 애플리케이션 초기화/종료

**특징**:
- C++17 지원 (`std::shared_ptr` 사용)
- 메모리 안전성 강화

---

### 2. **OLE Control Layer (CMainWnd)**

```cpp
class CMainWnd : public CWnd {
  std::unique_ptr<CGrpWnd> m_pGrpWnd;     // Graph window
  std::unique_ptr<CCtrlWnd> m_pCtrlWnd;   // Control window
  
  // OLE Dispatch Interface
  // Property: show, visible, RTSCode
  // Method: GetProperties, SetProperties, Clear, SetData, SetShowLine, SetRTS
};
```

**책임**:
- OLE 디스패치 메서드 구현
- CGrpWnd, CCtrlWnd 소유 및 관리 (unique_ptr)
- 메시지 라우팅 (OnUser, OnCtrl, OnGrp, OnPreview)
- RTSCode 프로퍼티 관리

**Dispatch Interface**:
```
Properties:
  - show (BOOL)
  - visible (BOOL)
  - RTSCode (BSTR)

Methods:
  - GetProperties() -> BSTR
  - SetProperties(BSTR str)
  - Clear()
  - SetData(LPCTSTR str)
  - SetShowLine(short nLine, BOOL bShow)
  - SetRTS(BOOL bShow)
```

---

### 3. **Graph Rendering Layer (CGrpWnd)**

```cpp
class CGrpWnd : public CWnd {
  // 그래프 렌더링 윈도우
  // 데이터 저장소
  // 렌더링 엔진 관리
};
```

**책임**:
- 그래프 데이터 저장 및 관리
- 그래프 객체 (CGrp_*) 관리
- Windows 메시지 처리 (WM_PAINT, WM_MOUSEMOVE 등)
- 십자선(CrossLine) 표시

---

### 4. **Control Layer (CCtrlWnd)**

```cpp
class CCtrlWnd : public CWnd {
  // 제어 패널 윈도우
  // 사용자 입력 처리
};
```

**책임**:
- UI 제어 요소 렌더링
- 사용자 입력 처리 (마우스, 키보드)
- CMainWnd와의 메시지 통신

---

### 5. **Graph Engine (CGrp_* Classes)**

#### CGrp_Base (추상 기본 클래스)

```cpp
class CGrp_Base {
  virtual void DrawGraph(CDC *pDC) = 0;        // 순수 가상
  virtual void DrawChart(CDC* pDC) { }         // 선택 구현
  virtual bool CalculateMinMax() = 0;          // 최대/최소값 계산
  virtual bool IsChangeMinMax(bool bShift) = 0;
  virtual void GetMinMaxValue(double& dMin, double& dMax) = 0;
};
```

**공통 책임**:
- 데이터 최대/최소 계산
- 좌표 변환
- 눈금(Tick) 그리기
- 텍스트 렌더링 (DisplayRect, GetDisplayPosHead 등)
- Excel 내보내기 데이터 (GetExcelHead, GetExcelData)

#### CGrp_Line (선형 차트)

```cpp
class CGrp_Line : public CGrp_Base {
  virtual void DrawGraph(CDC *pDC) override;
  virtual bool CalculateMinMax() override;
};
```

**책임**:
- 선형 차트 그리기
- 선의 스타일/색상 관리
- 데이터 점 렌더링

#### CGrp_Bar (막대 차트)

```cpp
class CGrp_Bar : public CGrp_Base {
  virtual void DrawGraph(CDC *pDC) override;
  virtual bool CalculateMinMax() override;
};
```

**책임**:
- 막대 차트 그리기
- 막대 색상/크기 관리

#### CGrp_VBar (수직 막대 차트)

```cpp
class CGrp_VBar : public CGrp_Base {
  virtual void DrawGraph(CDC *pDC) override;
  virtual bool CalculateMinMax() override;
};
```

**책임**:
- 수직 막대 차트 그리기

#### CGrp_Data (데이터 컨테이너)

```cpp
class CGrp_Data {
  // 차트 데이터 저장소
  // 데이터 파싱, 검증, 접근
};
```

**책임**:
- 차트 데이터 저장
- 데이터 파싱 및 포맷 검증
- 범위 계산 및 쿼리

---

### 6. **Utility Layer**

#### CrossLine
- 십자선 렌더링 및 추적

#### ToolTip
- 마우스 오버 시 도구 팁 표시

#### Ctrl
- 각종 제어 로직

---

## 주요 클래스 및 책임

### CC_UserGrpApp

| 멤버 | 용도 |
|------|------|
| `m_pImgCtrl` | 제어 아이콘 이미지 리스트 (std::shared_ptr) |
| `m_pView` | 부모 뷰 포인터 |
| `m_hDlg` | 다이얼로그 라이브러리 핸들 |

**주요 메서드**:
- `GetSampleData()` - 샘플 차트 데이터 반환
- `CreateImage()` - 이미지 리스트 생성
- `GetFont(), GetPen(), GetBrush()` - 그래픽 리소스
- `GetColor()` - 색상 조회

### CMainWnd

| 멤버 | 용도 |
|------|------|
| `m_pGrpWnd` | 그래프 윈도우 (unique_ptr) |
| `m_pCtrlWnd` | 제어 윈도우 (unique_ptr) |
| `m_param` | 차트 파라미터 |
| `m_RTSCode` | 실시간 시세 종목코드 |
| `m_bCtrl` | 제어 활성화 플래그 |

**Dispatch 메서드**:
- `GetProperties() / SetProperties()` - OLE 인터페이스
- `Clear()` - 데이터 초기화
- `SetData(LPCTSTR str)` - 데이터 설정
- `SetShowLine(short nLine, BOOL bShow)` - 선 표시/숨김
- `SetRTS(BOOL bShow)` - 실시간 시세 활성화

**메시지 핸들러**:
- `OnCreate()` - CGrpWnd, CCtrlWnd 생성
- `OnSize()` - 윈도우 리사이징
- `OnUser()` - 사용자 정의 메시지
- `OnCtrl()` - 제어 메시지
- `OnGrp()` - 그래프 메시지
- `OnPreview()` - 미리보기 메시지

### CGrpWnd

**책임**:
- 그래프 렌더링 (WM_PAINT)
- 마우스 이벤트 (WM_LBUTTONDOWN, WM_MOUSEMOVE)
- 그래프 객체 (CGrp_Line, CGrp_Bar 등) 관리
- CrossLine 관리

### CGrp_Base

**공통 메서드**:
- `DrawGraph(CDC *pDC)` - 차트 그리기 (가상)
- `CalculateMinMax()` - 데이터 범위 계산
- `IsChangeMinMax()` - 범위 변화 감지
- `DrawDateBottom()` - 날짜 축 그리기
- `DrawVerticalNumber()` - 수직 숫자 표시
- `DrawTickHorizon()` - 수평 눈금 그리기

---

## 메시지 흐름

### OLE Dispatch 메서드 호출

```
Host Application (VB, C#, VBS)
    │
    ├─> IMainWnd::SetData(LPCTSTR str)
    │   └─> CMainWnd::SetData() -> CGrpWnd::SetData()
    │       └─> CGrp_Data::ParseData()
    │           └─> CGrpWnd::InvalidateRect() -> WM_PAINT
    │
    ├─> IMainWnd::GetProperties()
    │   └─> CMainWnd::GetProperties() -> 문자열 반환
    │
    ├─> IMainWnd::SetShowLine(nLine, bShow)
    │   └─> CMainWnd::SetShowLine()
    │       └─> CGrpWnd::SetShowLine()
    │           └─> 해당 라인 표시/숨김
    │
    └─> IMainWnd::SetRTS(bShow)
        └─> CMainWnd::SetRTS()
            └─> 실시간 시세 갱신 활성화
```

### Windows 메시지 처리

```
CMainWnd::WndProc
    ├─> WM_CREATE
    │   ├─> CreateWindow(CGrpWnd)
    │   └─> CreateWindow(CCtrlWnd)
    │
    ├─> WM_SIZE
    │   ├─> MoveWindow(CGrpWnd, 새로운 크기)
    │   └─> MoveWindow(CCtrlWnd, 새로운 크기)
    │
    ├─> WM_PAINT
    │   ├─> CGrpWnd::OnPaint()
    │   │   ├─> CGrp_Base::DrawGraph()
    │   │   ├─> DrawTickBottom()
    │   │   └─> DrawName()
    │   │
    │   └─> CCtrlWnd::OnPaint()
    │
    └─> Custom Messages
        ├─> OnUser (사용자 정의)
        ├─> OnCtrl (제어)
        ├─> OnGrp (그래프)
        └─> OnPreview (미리보기)
```

### Graph Rendering Pipeline

```
CGrpWnd::OnPaint()
    │
    ├─> CDC* pDC = BeginPaint()
    │
    ├─> for each CGrp_* in m_arGrp
    │   ├─> CalculateMinMax()  // 범위 계산
    │   ├─> DrawGraph()        // 차트 그리기
    │   ├─> DrawTickBottom()   // 눈금
    │   └─> DrawName()         // 라벨
    │
    ├─> CrossLine::Draw()      // 십자선
    ├─> ToolTip::Draw()        // 도구 팁
    │
    └─> EndPaint()
```

---

## 데이터 흐름

### 데이터 입력 및 렌더링

```
SetData(LPCTSTR str)  (OLE Method)
    │
    ├─> CGrp_Data::ParseData(str)
    │   ├─> 문자열 파싱
    │   ├─> 데이터 유효성 검증
    │   └─> 내부 버퍼에 저장
    │
    ├─> CGrpWnd::InvalidateRect()
    │   └─> WM_PAINT 메시지 발생
    │
    └─> CGrpWnd::OnPaint()
        ├─> CGrp_Line/Bar/VBar::DrawGraph()
        ├─> 축/눈금 렌더링
        └─> 화면 업데이트
```

### 마우스 상호작용

```
User Mouse Move
    │
    └─> CGrpWnd::OnMouseMove()
        ├─> 현재 위치에서 데이터 값 계산
        ├─> CrossLine::Update()
        ├─> ToolTip::Show()
        └─> InvalidateRect() -> WM_PAINT
```

### 실시간 시세 (RTS) 갱신

```
SetRTS(BOOL bShow)
    │
    ├─> m_RTSCode 저장
    ├─> 외부 시세 소스에서 데이터 수신
    ├─> SetData() 호출 (새 데이터)
    │   └─> 렌더링 업데이트
    │
    └─> 타이머로 주기적 갱신 (추정)
```

---

## 그래프 타입 다형성

### 그래프 종류별 렌더링

```
CGrp_Base (추상 기본)
├─> CGrp_Line  (선형 차트)
│   ├─ 데이터 점들을 선으로 연결
│   └─ 선 스타일/너비/색상 지원
│
├─> CGrp_Bar   (수평 막대 차트)
│   ├─ 각 데이터값을 막대로 표현
│   └─ 막대 색상/크기 조절
│
└─> CGrp_VBar  (수직 막대 차트)
    ├─ 수직 막대로 데이터 표현
    └─ 양수/음수 색상 구분

CGrp_Data (데이터 저장소)
└─ 모든 그래프 타입이 공유하는 데이터 컨테이너
```

---

## 문제점 및 개선 대상

### 1. **메모리 관리**
   - unique_ptr / shared_ptr 사용으로 안전성 향상
   - 단, 원시 포인터도 여전히 사용 중
   
   **개선 방안**: 모든 포인터를 스마트 포인터로 전환

### 2. **그래프 렌더링 성능**
   - 매 프레임마다 OnPaint에서 CalculateMinMax 호출
   - 대량 데이터 시 UI 프리징 가능
   
   **개선 방안**: 
   - 캐시된 범위 값 사용
   - 비동기 렌더링

### 3. **Thread 안전성**
   - 멀티 스레드 환경에서 데이터 접근 미보호
   
   **개선 방안**: CriticalSection 도입

### 4. **외부 의존성**
   - axisfire.h 강결합
   
   **개선 방안**: 의존성 주입 패턴

### 5. **에러 처리**
   - 데이터 파싱 실패 시 처리 로직 불명확
   
   **개선 방안**: 명확한 예외 처리 및 로깅

---

## 히스토리

- **초기 작성**: 2026-08-27
- **마지막 갱신**: 2026-08-27
