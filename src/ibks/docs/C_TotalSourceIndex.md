# C_Total 소스 인덱스

## 목차

- [개요](#개요)
- [Core Application](#core-application)
- [Main Control Window](#main-control-window)
- [Panel Subsystem](#panel-subsystem)
- [Data Management](#data-management)
- [Utilities](#utilities)

---

## 개요

C_Total 프로젝트는 총 **13개 소스 파일**로 구성되어 있으며, DLL 생성 시 CONTROL/Common의
SavedHead.cpp와 MxTrace.cpp도 포함됩니다.

| 분류 | 파일 수 | 역할 |
|------|--------|------|
| **Core** | 3 | DLL 진입점, OLE 서버 |
| **Main Window** | 3 | OLE 차트 컨트롤 윈도우 |
| **Panels** | 4 | 제어/입력 패널 |
| **Data** | 2 | 데이터 검증, 파일 I/O |
| **Utilities** | 1 | 메시지 처리 |

---

## Core Application

### C_Total.h

| 항목 | 설명 |
|------|------|
| **역할** | DLL 메인 헤더, 애플리케이션 클래스 정의 |
| **주요 클래스** | `CC_TotalApp` (CWinApp 상속) |
| **주요 멤버** | m_strRoot, m_hGMainLib, m_hGDlgLib, m_pImg* (CImageList 배열) |
| **주요 메서드** | Create(), GetFont(), GetPen(), GetBrush(), GetImage() |
| **의존성** | axisfire.h, axisvar.h, axisgwin.h, defInfo.h |
| **사용 API** | MFC OLE/COM, Windows GDI |
| **관련 파일** | C_Total.cpp, resource.h |

**핵심 코드**:
```cpp
class CC_TotalApp : public CWinApp {
  // DLL 애플리케이션 객체
  // 전역 리소스 (폰트, 펜, 브러시, 이미지) 관리
  // OLE 서버 초기화 (COleObjectFactory::RegisterAll)
};
```

---

### C_Total.cpp

| 항목 | 설명 |
|------|------|
| **역할** | CC_TotalApp 구현, DLL 진입점 |
| **주요 함수** | CC_TotalApp::InitInstance(), CC_TotalApp 생성자/소멸자 |
| **주요 처리** | OLE 서버 등록, 라이브러리 해제 |
| **의존성** | C_Total.h, MainWnd.h, DataChk.h, stdafx.h |
| **사용 API** | MFC OLE, FreeLibrary() |
| **관련 파일** | C_Total.h, MainWnd.h |

**핵심 로직**:
- `COleObjectFactory::RegisterAll()` - OLE 객체 팩토리 등록
- 리소스 이미지 로드 및 폰트 생성
- 라이브러리 핸들(GMainLib, GDlgLib) 정리

---

### libTotal.cpp

| 항목 | 설명 |
|------|------|
| **역할** | OLE/COM 라이브러리 함수 구현 (Export) |
| **주요 함수** | DLL_CreateEx(), DLL_Delete(), DLL_SetProperties(), GetProperties(), SetProperties() 등 |
| **주요 처리** | OLE 객체 생성/삭제, Dispatch 메서드 래퍼 |
| **의존성** | C_Total.h, MainWnd.h |
| **사용 API** | MFC COM/OLE |
| **관련 파일** | C_Total.def (모듈 정의) |

**예상 구현**:
```cpp
extern "C" LPVOID PASCAL EXPORT DLL_CreateEx(LPVOID pParent, LPVOID pInfo) {
  // CMainWnd 생성 및 반환
}
extern "C" void PASCAL EXPORT DLL_Delete(LPVOID pWnd) {
  // 윈도우 삭제
}
```

---

## Main Control Window

### MainWnd.h

| 항목 | 설명 |
|------|------|
| **역할** | OLE 차트 컨트롤 윈도우 헤더, Dispatch Interface |
| **주요 클래스** | `CMainWnd` (CWnd 상속, OLE Dispatch) |
| **주요 멤버** | m_iDtUnit, m_iDtIndex, m_pwndChart, m_arData, m_arLen, m_mapPanel, m_pEnvInfo |
| **주요 메서드** | GetProperties(), SetProperties(), RequestTR(), RequestTR2(), Config() |
| **Dispatch 메서드** | GetProperties, SetProperties, RequestTR, RequestTR2, Config, GetTotalDay, GetDisplayDay, GetSelectTime, SetSelectTime, GetSelectPrice, SetTimeLine, RemoveTimeLine, SetOrderMode, sMarket(propput) |
| **의존성** | DataChk.h, ExtraMsg.h, fileIO.h, PnCtrl.h, PnInput.h, PnWndCombo.h, PnBaseWnd.h |
| **사용 API** | MFC OLE Dispatch, Windows Messages |
| **관련 파일** | MainWnd.cpp, C_Total.h |

**멤버 변수 분류**:
- **데이터 단위**: m_iDtUnit, m_iDtIndex (일/주/월)
- **윈도우 포인터**: m_pwndChart, m_pwndCtrl, m_pwndInput, m_pwndView
- **문자열 정보**: m_strDay, m_strCode, m_strJName, m_strMarket
- **버퍼**: m_arData (CArray), m_arLen, m_pacRegion, m_pacGraph, m_pcGDat
- **Panel 관리**: m_mapPanel (CMap<int, struct _pninfo*>)
- **환경**: m_pEnvInfo, m_pExtraMsg

---

### MainWnd.cpp

| 항목 | 설명 |
|------|------|
| **역할** | CMainWnd 구현, OLE 인터페이스 메서드 및 UI 로직 |
| **주요 메서드** | OnCreate(), OnDestroy(), OnSize(), ResizeWnd(), CreatePn(), DelPanels() |
| **주요 OLE 메서드** | GetProperties(), SetProperties(), RequestTR(), RequestTR2(), Config() |
| **파일 I/O** | ReadEnv(), ReadGrp(), ReadData(), WriteEnv(), WriteGrp(), WriteDat() |
| **동기화** | SyncMainChart(), SyncMap(), IndexChange() |
| **의존성** | MainWnd.h, DataChk.h, fileIO.h, ExtraMsg.h, PnCtrl.h 등 |
| **사용 API** | MFC Message Map, Windows GDI |
| **관련 파일** | MainWnd.h, C_Total.h |

**핵심 흐름**:
1. `OnCreate()` - Panel 생성 (CreatePn)
2. `OnSize()` - 윈도우 리사이징 (ResizeWnd)
3. `RequestTR()` - 외부 데이터 요청 (SendRequest)
4. `SetProperties()` - Panel 구성 변경 (SetGrpAtDat)

---

### ExtraMsg.h / ExtraMsg.cpp

| 항목 | 설명 |
|------|------|
| **역할** | 추가 메시지 처리, 콜백 메커니즘 |
| **주요 클래스** | `CExtraMsg` (자체 클래스) |
| **주요 메서드** | 메시지 후처리, 이벤트 콜백 등 (구체적 구현은 소스 확인 필요) |
| **의존성** | MainWnd.h, stdafx.h |
| **사용 API** | Windows Messages, MFC |
| **관련 파일** | ExtraMsg.h, MainWnd.h |

---

## Panel Subsystem

### PnBaseWnd.h

| 항목 | 설명 |
|------|------|
| **역할** | Panel 윈도우 기반 클래스, 공통 렌더링 및 이벤트 처리 |
| **주요 클래스** | `CPnBaseWnd` (CWnd 상속), `CPnSlider` (CSliderCtrl 상속) |
| **주요 멤버** | m_ctrlID, m_clrBack, m_clrFrame, m_arComs (컴포넌트 배열) |
| **주요 메서드** | SetCtrlID(), GetCtlrID(), SetBackColor(), SetFrameColor() |
| **렌더링** | DrawBackground(), DrawFrame(), DrawButton() |
| **이벤트** | OnMouseWheel(), OnMouseMove(), OnLButtonDown() 등 |
| **의존성** | afxtempl.h (CArray), stdafx.h |
| **사용 API** | MFC Windows, GDI drawing |
| **관련 파일** | PnBaseWnd.cpp, PnCtrl.h, PnInput.h, PnWndCombo.h |

**CPnSlider**:
- CSliderCtrl 상속
- 마우스 휠 이벤트 처리 (OnMouseWheel)

---

### PnCtrl.h / PnCtrl.cpp

| 항목 | 설명 |
|------|------|
| **역할** | 제어 패널 컨트롤 (CPnBaseWnd 상속) |
| **주요 클래스** | `CPnCtrl` |
| **주요 멤버** | (PnBaseWnd 상속) |
| **주요 메서드** | 컨트롤 요소 렌더링 및 이벤트 처리 |
| **의존성** | PnBaseWnd.h, stdafx.h |
| **사용 API** | MFC Windows |
| **관련 파일** | PnCtrl.h, PnBaseWnd.h, MainWnd.h |

---

### PnInput.h / PnInput.cpp

| 항목 | 설명 |
|------|------|
| **역할** | 입력 패널 (CPnBaseWnd 상속) |
| **주요 클래스** | `CPnInput` |
| **주요 멤버** | (PnBaseWnd 상속) |
| **주요 메서드** | 입력 필드 렌더링 및 키보드 입력 처리 |
| **의존성** | PnBaseWnd.h, stdafx.h |
| **사용 API** | MFC Windows |
| **관련 파일** | PnInput.h, PnBaseWnd.h, MainWnd.h |

---

### PnWndCombo.h / PnWndCombo.cpp

| 항목 | 설명 |
|------|------|
| **역할** | 콤보박스 윈도우 (CPnBaseWnd 상속) |
| **주요 클래스** | `CPnWndCombo` |
| **주요 멤버** | (PnBaseWnd 상속) |
| **주요 메서드** | 드롭다운 아이템 렌더링 및 선택 처리 |
| **의존성** | PnBaseWnd.h, stdafx.h |
| **사용 API** | MFC Windows |
| **관련 파일** | PnWndCombo.h, PnBaseWnd.h, MainWnd.h |

---

## Data Management

### DataChk.h

| 항목 | 설명 |
|------|------|
| **역할** | 데이터 검증 및 파일 재생성 |
| **주요 클래스** | `CDataChk` |
| **주요 멤버** | m_bVol, m_bName, m_iFPoint, m_iDtCount, m_iBongKind, m_aiPMA[], m_aiVMA[], m_strFileName 등 |
| **주요 메서드** | CheckData(), MakeDat(), MakeEnv(), MakeGrp(), ReMakeData() |
| **의존성** | stdafx.h |
| **사용 API** | File I/O, Windows |
| **관련 파일** | DataChk.cpp, fileIO.h, MainWnd.h |

**핵심 역할**:
- 차트 데이터 일관성 검증
- .env, .grp, .dat 파일 생성/갱신
- 색상 팔레트 로드

---

### DataChk.cpp

| 항목 | 설명 |
|------|------|
| **역할** | CDataChk 구현 |
| **주요 함수** | CheckData(), MakeDat(), MakeEnv(), MakeGrp(), ReMakeData() |
| **주요 처리** | 파일 경로 분석, 데이터 포맷 검증, 환경/그룹/데이터 파일 생성 |
| **의존성** | DataChk.h, fileIO.h, stdafx.h |
| **사용 API** | CFile, File I/O |
| **관련 파일** | DataChk.h, fileIO.h, MainWnd.h |

---

### fileIO.h

| 항목 | 설명 |
|------|------|
| **역할** | 파일 입출력 인터페이스 (ENV, GRP, DAT 파일) |
| **주요 함수** | 예상 구현: ReadEnv(), WriteEnv(), ReadGrp(), WriteGrp(), ReadData(), WriteData() |
| **파일 형식** | .env (환경), .grp (그룹), .dat (데이터) |
| **의존성** | stdafx.h |
| **사용 API** | File I/O |
| **관련 파일** | fileIO.cpp, MainWnd.h, DataChk.h |

---

### fileIO.cpp

| 항목 | 설명 |
|------|------|
| **역할** | 파일 I/O 구현 |
| **주요 함수** | 파일 읽기/쓰기 함수 |
| **주요 처리** | 파일 경로 확인, 데이터 직렬화/역직렬화 |
| **의존성** | fileIO.h, stdafx.h |
| **사용 API** | CFile, Windows File API |
| **관련 파일** | fileIO.h, DataChk.h, MainWnd.h |

---

## Utilities

### StdAfx.h / StdAfx.cpp

| 항목 | 설명 |
|------|------|
| **역할** | 프리컴파일된 헤더 (PCH) |
| **주요 Include** | MFC core (afxwin.h, afxext.h, afxole.h, afxdisp.h 등) |
| **주요 Include** | AXIS 헤더 (../../chart_dll/mxtrace.h) |
| **의존성** | Windows SDK, MFC SDK |
| **사용 API** | MFC 전체 |
| **관련 파일** | StdAfx.cpp |

---

### SavedHead.cpp (CONTROL/Common)

| 항목 | 설명 |
|------|------|
| **역할** | 공통 유틸리티 함수 |
| **주요 함수** | 파일 경로 처리, 문자열 유틸 등 |
| **의존성** | 없음 (독립 유틸) |
| **사용 API** | 기본 C++ 라이브러리 |
| **관련 파일** | CONTROL/Common/SavedHead.cpp (외부) |

**참고**: 소스 코드 직접 포함 (링크 의존성 없음)

---

### MxTrace.cpp (chart_dll)

| 항목 | 설명 |
|------|------|
| **역할** | 트레이싱/로깅 유틸리티 |
| **주요 함수** | 디버그 로그, 추적 정보 출력 |
| **의존성** | 없음 (독립 유틸) |
| **사용 API** | Windows OutputDebugString 등 |
| **관련 파일** | D:\src\IBKS\src\CONTROL\MxTrace.cpp (외부) |

**참고**: 소스 코드 직접 포함 (링크 의존성 없음)

---

## 자동 생성 파일

| 파일명 | 생성 도구 | 역할 |
|--------|----------|------|
| C_Total_h.h | MIDL (C_Total.odl) | OLE Type Library 헤더 |
| C_Total_i.c | MIDL (C_Total.odl) | OLE Interface ID 상수 |
| C_Total.tlb | MIDL (C_Total.odl) | Type Library 바이너리 |
| Resource.h | Visual Studio | 리소스 ID 헤더 |
| C_Total.rc | 리소스 컴파일러 | 리소스 파일 |

---

## 파일 구성 요약

```
C_Total/
├── C_Total.h/cpp           <- DLL 진입점, CC_TotalApp
├── MainWnd.h/cpp           <- OLE 차트 컨트롤
├── ExtraMsg.h/cpp          <- 추가 메시지 처리
├── DataChk.h/cpp           <- 데이터 검증
├── fileIO.h/cpp            <- 파일 I/O
├── PnBaseWnd.h/cpp         <- Panel 기반 클래스
├── PnCtrl.h/cpp            <- 제어 패널
├── PnInput.h/cpp           <- 입력 패널
├── PnWndCombo.h/cpp        <- 콤보 윈도우
├── libTotal.cpp            <- OLE 라이브러리 함수
├── StdAfx.h/cpp            <- PCH
├── C_Total.odl             <- OLE Type Library 정의
├── C_Total.def             <- Module Definition (Export)
├── C_Total.rc              <- 리소스 파일
├── C_Total.vcxproj         <- Visual Studio 프로젝트
└── docs/
    └── Architecture.md     <- 아키텍처 문서
```

---

## 히스토리

- **초기 작성**: 2026-08-27
- **마지막 갱신**: 2026-08-27
