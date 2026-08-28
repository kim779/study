# C_UserGrp 소스 인덱스

## 목차

- [개요](#개요)
- [Core Application](#core-application)
- [OLE Control Interface](#ole-control-interface)
- [Graph Window & Rendering](#graph-window--rendering)
- [Graph Types (Engine)](#graph-types-engine)
- [Data Container](#data-container)
- [Control & Utilities](#control--utilities)

---

## 개요

C_UserGrp 프로젝트는 총 **14개 소스 파일**로 구성되어 있으며, 다형적 그래프 렌더링 엔진과
C++17 기반의 현대적 메모리 관리를 특징으로 합니다.

| 분류 | 파일 수 | 역할 |
|------|--------|------|
| **Core** | 2 | DLL 진입점, OLE 서버 |
| **OLE Interface** | 1 | Dispatch 윈도우 |
| **Graph System** | 3 | 렌더링 윈도우 및 기본 엔진 |
| **Graph Types** | 3 | 선형, 막대, 수직막대 차트 |
| **Data & Utilities** | 5 | 데이터, 십자선, 도구팁, 제어 |

---

## Core Application

### C_UserGrp.h

| 항목 | 설명 |
|------|------|
| **역할** | DLL 메인 헤더, 애플리케이션 클래스 정의 |
| **주요 클래스** | `CC_UserGrpApp` (CWinApp 상속) |
| **주요 멤버** | m_pView, m_pImgCtrl (std::shared_ptr), m_hDlg |
| **주요 메서드** | GetSampleData(), CreateImage(), GetFont(), GetPen(), GetBrush(), GetColor() |
| **특징** | C++17 std::shared_ptr 사용 |
| **의존성** | axisfire.h, resource.h |
| **사용 API** | MFC OLE/COM, Windows GDI, std::shared_ptr |
| **관련 파일** | C_UserGrp.cpp, resource.h |

**핵심 코드**:
```cpp
class CC_UserGrpApp : public CWinApp {
  std::shared_ptr<CImageList> m_pImgCtrl;  // 제어 아이콘
  CWnd *m_pView;                           // 부모 뷰
  HINSTANCE m_hDlg;                        // 다이얼로그 DLL
  
  // OLE 서버 초기화
  // 전역 리소스 관리
};
```

---

### C_UserGrp.cpp

| 항목 | 설명 |
|------|------|
| **역할** | CC_UserGrpApp 구현, DLL 진입점 |
| **주요 함수** | CC_UserGrpApp::InitInstance(), ExitInstance() |
| **주요 처리** | OLE 서버 등록, 이미지 생성, 라이브러리 정리 |
| **의존성** | C_UserGrp.h, MainWnd.h, stdafx.h |
| **사용 API** | MFC OLE, FreeLibrary() |
| **관련 파일** | C_UserGrp.h, MainWnd.h |

**핵심 로직**:
- OLE 객체 팩토리 등록 (COleObjectFactory::RegisterAll)
- 이미지 리스트 로드 및 생성
- 라이브러리 핸들 정리

---

### libctrl.cpp

| 항목 | 설명 |
|------|------|
| **역할** | OLE/COM 라이브러리 함수 구현 (Export) |
| **주요 함수** | DLL_Create(), DLL_Delete(), DLL_SetData(), GetProperties(), SetProperties() 등 |
| **주요 처리** | OLE 객체 생성/삭제, Dispatch 메서드 래퍼 |
| **의존성** | C_UserGrp.h, MainWnd.h |
| **사용 API** | MFC COM/OLE |
| **관련 파일** | C_UserGrp.def (모듈 정의) |

**예상 구현**:
```cpp
extern "C" LPVOID PASCAL EXPORT DLL_Create(LPVOID pParent, LPVOID pInfo) {
  // CMainWnd 생성 및 반환
}
extern "C" void PASCAL EXPORT DLL_Delete(LPVOID pWnd) {
  // 윈도우 삭제
}
```

---

## OLE Control Interface

### MainWnd.h

| 항목 | 설명 |
|------|------|
| **역할** | OLE 차트 컨트롤 윈도우 헤더, Dispatch Interface |
| **주요 클래스** | `CMainWnd` (CWnd 상속, OLE Dispatch) |
| **주요 멤버** | m_pGrpWnd (unique_ptr), m_pCtrlWnd (unique_ptr), m_param, m_RTSCode, m_bCtrl |
| **주요 메서드** | OnCreate(), OnDestroy(), OnSize(), Resize() |
| **Dispatch 메서드** | GetProperties, SetProperties, Clear, SetData, SetShowLine, SetRTS, GetRTSCode, SetRTSCode |
| **메시지 맵** | OnUser, OnCtrl, OnGrp, OnPreview |
| **특징** | std::unique_ptr 사용, OLE 자동화 |
| **의존성** | GrpWnd.h, CtrlWnd.h, _param 구조체 |
| **사용 API** | MFC OLE Dispatch, Windows Messages, std::unique_ptr |
| **관련 파일** | MainWnd.cpp, C_UserGrp.h |

**멤버 변수**:
```cpp
std::unique_ptr<CGrpWnd> m_pGrpWnd;      // 그래프 렌더링 윈도우
std::unique_ptr<CCtrlWnd> m_pCtrlWnd;    // 제어 패널 윈도우
CWnd *m_pParent;                          // 부모 윈도우
struct _param m_param;                    // 파라미터
CString m_RTSCode;                        // 실시간 시세 종목코드
bool m_bCtrl;                             // 제어 활성화 플래그
```

---

### MainWnd.cpp

| 항목 | 설명 |
|------|------|
| **역할** | CMainWnd 구현, OLE 인터페이스 메서드 및 UI 로직 |
| **주요 메서드** | OnCreate(), OnDestroy(), OnSize(), Resize() |
| **주요 OLE 메서드** | GetProperties(), SetProperties(), Clear(), SetData(), SetShowLine(), SetRTS() |
| **메시지 핸들러** | OnUser(), OnCtrl(), OnGrp(), OnPreview() |
| **의존성** | MainWnd.h, GrpWnd.h, CtrlWnd.h |
| **사용 API** | MFC Message Map, Windows Messages, GDI |
| **관련 파일** | MainWnd.h, C_UserGrp.h, GrpWnd.h |

**핵심 흐름**:
1. `OnCreate()` - CGrpWnd, CCtrlWnd 생성 (unique_ptr)
2. `OnSize()` - 서브윈도우 리사이징 (Resize)
3. `SetData()` - 데이터 설정 (CGrpWnd::SetData)
4. `OnGrp()` - 그래프 메시지 라우팅

---

## Graph Window & Rendering

### GrpWnd.h

| 항목 | 설명 |
|------|------|
| **역할** | 그래프 렌더링 윈도우, 데이터 관리, 그래프 객체 관리 |
| **주요 클래스** | `CGrpWnd` (CWnd 상속) |
| **주요 멤버** | m_arGrp (그래프 배열), m_pGrpData (CGrp_Data), m_pCrossLine, m_pToolTip |
| **주요 메서드** | OnPaint(), OnMouseMove(), OnLButtonDown(), SetData(), SetShowLine(), Clear() |
| **메시지 맵** | WM_PAINT, WM_MOUSEMOVE, WM_LBUTTONDOWN, WM_SIZE |
| **의존성** | Grp_Base.h, Grp_Line.h, Grp_Bar.h, Grp_VBar.h, Grp_Data.h, CrossLine.h, ToolTip.h |
| **사용 API** | MFC Windows, GDI drawing, CDC |
| **관련 파일** | GrpWnd.cpp, MainWnd.h |

**핵심 역할**:
- 그래프 렌더링 (OnPaint)
- 마우스 상호작용 (십자선, 도구팁)
- 그래프 객체 (CGrp_Line, CGrp_Bar 등) 생성/관리
- 데이터 파싱 및 저장 (SetData)

---

### GrpWnd.cpp

| 항목 | 설명 |
|------|------|
| **역할** | CGrpWnd 구현 |
| **주요 함수** | OnPaint(), OnMouseMove(), SetData(), SetShowLine(), Clear(), Resize() |
| **주요 처리** | 그래프 렌더링, 이벤트 처리, 데이터 업데이트 |
| **의존성** | GrpWnd.h, Grp_*.h, Grp_Data.h, CrossLine.h, ToolTip.h |
| **사용 API** | CDC, CRect, CPoint, InvalidateRect |
| **관련 파일** | GrpWnd.h, MainWnd.h |

**렌더링 파이프라인**:
```cpp
OnPaint() {
  for each CGrp_* in m_arGrp {
    CalculateMinMax();    // 범위 계산
    DrawGraph();          // 차트 그리기
    DrawTickBottom();     // 눈금
    DrawName();           // 라벨
  }
  CrossLine::Draw();      // 십자선
  ToolTip::Draw();        // 도구 팁
}
```

---

## Graph Types (Engine)

### Grp_Base.h

| 항목 | 설명 |
|------|------|
| **역할** | 모든 그래프 타입의 기본 클래스, 공통 렌더링 로직 |
| **주요 클래스** | `CGrp_Base` (추상 기본 클래스) |
| **주요 멤버** | m_DrawRect, m_RgnRect, m_Max, m_Min, m_tRGB, m_pRGB, m_TickInfo, m_LineWidth |
| **순수 가상 메서드** | DrawGraph(), CalculateMinMax(), IsChangeMinMax(), GetMinMaxValue() |
| **공통 메서드** | DisplayRect(), DrawTickHorizon(), DrawDateBottom(), DrawName(), FormatVariableComma() |
| **유틸리티** | GetTickCount(), calculateDisplay(), DrawVerticalString() |
| **의존성** | stdafx.h, CGrpWnd |
| **사용 API** | CDC, CWnd, 기본 C++ |
| **관련 파일** | Grp_Base.cpp, Grp_Line.h, Grp_Bar.h, Grp_VBar.h |

**추상 인터페이스**:
```cpp
class CGrp_Base {
  virtual void DrawGraph(CDC *pDC) = 0;           // 순수 가상
  virtual bool CalculateMinMax() = 0;             // 순수 가상
  virtual bool IsChangeMinMax(bool bShift) = 0;   // 순수 가상
  virtual void GetMinMaxValue(double& dMin, double& dMax) = 0;
};
```

---

### Grp_Base.cpp

| 항목 | 설명 |
|------|------|
| **역할** | CGrp_Base 구현, 공통 렌더링 로직 |
| **주요 함수** | DisplayRect(), DrawTickHorizon(), DrawDateBottom(), Resize(), FormatVariableComma() |
| **주요 처리** | 공통 렌더링 (눈금, 라벨, 3D 효과) |
| **의존성** | Grp_Base.h, stdafx.h |
| **사용 API** | CDC, GDI functions |
| **관련 파일** | Grp_Base.h, GrpWnd.h |

---

### Grp_Line.h / Grp_Line.cpp

| 항목 | 설명 |
|------|------|
| **역할** | 선형 차트 구현 |
| **주요 클래스** | `CGrp_Line` (CGrp_Base 상속) |
| **주요 메서드** | DrawGraph() override, CalculateMinMax() override |
| **특징** | 데이터 점들을 선으로 연결 |
| **의존성** | Grp_Base.h, Grp_Data.h |
| **사용 API** | CDC::Polyline(), CDC::MoveTo(), CDC::LineTo() |
| **관련 파일** | Grp_Line.h/cpp, GrpWnd.h |

---

### Grp_Bar.h / Grp_Bar.cpp

| 항목 | 설명 |
|------|------|
| **역할** | 막대 차트 구현 |
| **주요 클래스** | `CGrp_Bar` (CGrp_Base 상속) |
| **주요 메서드** | DrawGraph() override, CalculateMinMax() override |
| **특징** | 각 데이터를 수평 막대로 표현 |
| **의존성** | Grp_Base.h, Grp_Data.h |
| **사용 API** | CDC::Rectangle(), CDC::FillRect() |
| **관련 파일** | Grp_Bar.h/cpp, GrpWnd.h |

---

### Grp_VBar.h / Grp_VBar.cpp

| 항목 | 설명 |
|------|------|
| **역할** | 수직 막대 차트 구현 |
| **주요 클래스** | `CGrp_VBar` (CGrp_Base 상속) |
| **주요 메서드** | DrawGraph() override, CalculateMinMax() override |
| **특징** | 각 데이터를 수직 막대로 표현 |
| **의존성** | Grp_Base.h, Grp_Data.h |
| **사용 API** | CDC::Rectangle(), CDC::FillRect() |
| **관련 파일** | Grp_VBar.h/cpp, GrpWnd.h |

---

## Data Container

### Grp_Data.h

| 항목 | 설명 |
|------|------|
| **역할** | 차트 데이터 저장소 및 파싱 |
| **주요 클래스** | `CGrp_Data` |
| **주요 멤버** | 데이터 배열/컨테이너, 메타데이터 (범위, 개수 등) |
| **주요 메서드** | ParseData(), GetValue(), GetCount(), GetMinMax() |
| **의존성** | stdafx.h |
| **사용 API** | 기본 C++, CArray (MFC) |
| **관련 파일** | Grp_Data.cpp, Grp_Base.h, GrpWnd.h |

**데이터 포맷** (추정):
```
종목|날짜|시가|고가|저가|종가|거래량
```

---

### Grp_Data.cpp

| 항목 | 설명 |
|------|------|
| **역할** | CGrp_Data 구현 |
| **주요 함수** | ParseData(), GetValue(), GetCount() 등 |
| **주요 처리** | 문자열 파싱, 데이터 검증, 저장 |
| **의존성** | Grp_Data.h, stdafx.h |
| **사용 API** | CString, atof, strtok_s 등 |
| **관련 파일** | Grp_Data.h, GrpWnd.h |

---

## Control & Utilities

### CtrlWnd.h / CtrlWnd.cpp

| 항목 | 설명 |
|------|------|
| **역할** | 제어 패널 윈도우 |
| **주요 클래스** | `CCtrlWnd` (CWnd 상속) |
| **주요 메서드** | OnPaint(), OnMouseMove(), OnLButtonDown() 등 |
| **의존성** | stdafx.h |
| **사용 API** | MFC Windows, GDI |
| **관련 파일** | CtrlWnd.h/cpp, MainWnd.h |

**책임**:
- UI 제어 요소 렌더링 (버튼, 슬라이더 등)
- 사용자 입력 처리
- MainWnd로 메시지 전송

---

### Ctrl.h / Ctrl.cpp

| 항목 | 설명 |
|------|------|
| **역할** | 제어 로직 (데이터 모델 또는 비즈니스 로직) |
| **주요 클래스** | `CCtrl` (추정) |
| **주요 메서드** | 제어 로직, 상태 관리 |
| **의존성** | stdafx.h |
| **사용 API** | 기본 C++ |
| **관련 파일** | Ctrl.h/cpp, CtrlWnd.h, GrpWnd.h |

---

### CrossLine.h / CrossLine.cpp

| 항목 | 설명 |
|------|------|
| **역할** | 십자선 렌더링 및 위치 추적 |
| **주요 클래스** | `CCrossLine` (추정) |
| **주요 메서드** | Draw(), Update(), GetValue() |
| **특징** | 마우스 위치에 따라 십자선 표시, 데이터 값 표시 |
| **의존성** | stdafx.h, GrpWnd.h |
| **사용 API** | CDC, CPoint |
| **관련 파일** | CrossLine.h/cpp, GrpWnd.h |

---

### ToolTip.h / ToolTip.cpp

| 항목 | 설명 |
|------|------|
| **역할** | 도구 팁 (마우스 오버 시 정보 표시) |
| **주요 클래스** | `CToolTip` (추정) |
| **주요 메서드** | Show(), Hide(), SetText() |
| **특징** | 마우스 오버 시 데이터 값/라벨 표시 |
| **의존성** | stdafx.h, GrpWnd.h |
| **사용 API** | CDC, CRect |
| **관련 파일** | ToolTip.h/cpp, GrpWnd.h |

---

## 자동 생성 파일

| 파일명 | 생성 도구 | 역할 |
|--------|----------|------|
| C_UserGrp_h.h | MIDL (C_UserGrp.odl) | OLE Type Library 헤더 |
| C_UserGrp_i.c | MIDL (C_UserGrp.odl) | OLE Interface ID 상수 |
| C_UserGrp.tlb | MIDL (C_UserGrp.odl) | Type Library 바이너리 |
| Resource.h | Visual Studio | 리소스 ID 헤더 |
| C_UserGrp.rc | 리소스 컴파일러 | 리소스 파일 |

---

## 파일 구성 요약

```
C_UserGrp/
├── C_UserGrp.h/cpp         <- DLL 진입점, CC_UserGrpApp
├── MainWnd.h/cpp           <- OLE 차트 컨트롤
├── GrpWnd.h/cpp            <- 그래프 렌더링 윈도우
├── CtrlWnd.h/cpp           <- 제어 패널
├── Ctrl.h/cpp              <- 제어 로직
├── Grp_Base.h/cpp          <- 그래프 기본 클래스
├── Grp_Line.h/cpp          <- 선형 차트
├── Grp_Bar.h/cpp           <- 막대 차트
├── Grp_VBar.h/cpp          <- 수직 막대 차트
├── Grp_Data.h/cpp          <- 데이터 컨테이너
├── CrossLine.h/cpp         <- 십자선
├── ToolTip.h/cpp           <- 도구 팁
├── libctrl.cpp             <- OLE 라이브러리 함수
├── StdAfx.h/cpp            <- PCH
├── C_UserGrp.odl           <- OLE Type Library 정의
├── C_UserGrp.def           <- Module Definition (Export)
├── C_UserGrp.rc            <- 리소스 파일
├── C_UserGrp.vcxproj       <- Visual Studio 프로젝트
└── docs/
    └── Architecture.md     <- 아키텍처 문서
```

---

## 클래스 계층도

```
CWnd
├─> CMainWnd           (OLE Dispatch)
│   ├─ CGrpWnd         (렌더링)
│   └─ CCtrlWnd        (제어)
│
└─> CGrpWnd
    └─ CGrp_Base       (추상 기본)
       ├─> CGrp_Line   (선형 차트)
       ├─> CGrp_Bar    (막대 차트)
       └─> CGrp_VBar   (수직 막대 차트)

CWinApp
└─> CC_UserGrpApp
```

---

## 히스토리

- **초기 작성**: 2026-08-27
- **마지막 갱신**: 2026-08-27
