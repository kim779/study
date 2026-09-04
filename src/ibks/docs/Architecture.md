---
project: ibks
category: architecture
status: 완료
updated: 2026-08-21
---

# ibks 프로젝트 아키텍처


## 목차

- [문서 목적](#문서-목적)
- [1. 프로젝트 개요](#1-프로젝트-개요)
  - [규모](#규모)
  - [진행 상황](#진행-상황)
- [2. 모듈 구조 (3계층)](#2-모듈-구조-3계층)
- [3. 핵심 인터페이스](#3-핵심-인터페이스)
  - [CEngineWrapper (엔진 추상화)](#cenginewrapper-엔진-추상화)
  - [CPythonEngine (Python C API 기반)](#cpythonengine-python-c-api-기반)
  - [바이너리 마커 (중요)](#바이너리-마커-중요)
- [4. 모듈 간 의존성](#4-모듈-간-의존성)
  - [상위 계층 → 하위 계층](#상위-계층-하위-계층)
  - [외부 의존성](#외부-의존성)
- [5. 데이터 흐름 (이벤트 처리)](#5-데이터-흐름-이벤트-처리)
- [6. COM 객체 명칭 (Python 대소문자 구분 주의)](#6-com-객체-명칭-python-대소문자-구분-주의)
- [7. 파일 구성](#7-파일-구성)
  - [axisvbs.dll (ibks/dll/vbs/)](#axisvbsdll-ibksdllvbs)
  - [axwizard (ibks/Wizard/)](#axwizard-ibkswizard)
  - [axisbuilder (builder/)](#axisbuilder-builder)
- [8. 배포 체크리스트](#8-배포-체크리스트)
  - [필수 DLL (HTS exe 폴더에 배치)](#필수-dll-hts-exe-폴더에-배치)
  - [빌드 순서](#빌드-순서)
- [9. 알려진 문제 및 해결](#9-알려진-문제-및-해결)
- [10. 향후 작업](#10-향후-작업)
- [11. 관련 문서](#11-관련-문서)

---

## 문서 목적

ibks 프로젝트의 모듈 구조, 계층 관계, 데이터 흐름을 정의하고 DLL 간 인터페이스를 명확히 합니다.

---

## 1. 프로젝트 개요

### 규모
- **언어:** C++ (MFC), COM, Python C API, VBScript
- **모듈 수:** Wizard, Builder, DLL(vbs), 컨트롤 등 다중 컴포넌트
- **주요 프로젝트:** VBScript → Python Engine 전환 (진행 중, 2026-06-08 ~ )

### 진행 상황
| 단계 | 상태 | 날짜 |
|------|------|------|
| Python Engine 구현 | 완료 (10/12) | 2026-06-11 |
| mapload pythonMode 자동감지 | 완료 | 2026-06-11 |
| 실운영 검증 | 미착수 | - |
| VBS->Python 일괄변환 도구 | 미착수 | - |

---

## 2. 모듈 구조 (3계층)

```
┌─────────────────────────────────────────────────┐
│         axisbuilder (화면 파일 생성)             │
│     - 마크업 편집 → .map 바이너리 생성            │
│     - Python 모드 편집기 (PY 버튼)               │
│     - ScpKind 플래그 (VBS:1, PYTHON:2)          │
└──────────────────┬──────────────────────────────┘
                   │ .map (Screen/Script 정의)
                   ▼
┌─────────────────────────────────────────────────┐
│         axwizard (화면 런타임)                   │
│     - MFC 기반 윈도우 생성                       │
│     - Screen.cpp → CEngineWrapper 생성           │
│     - Script.cpp → DoProcedure() 호출            │
│     - 자동 엔진 선택 (VBS vs Python)             │
└──────────────────┬──────────────────────────────┘
                   │ LoadScript(), DoProcedure()
                   ▼
┌─────────────────────────────────────────────────┐
│       axisvbs.dll (스크립트 엔진)                │
│                                                 │
│  ┌─ CEngineWrapper (엔진 선택/초기화)           │
│  │   ├─ isPythonScript() - def/import 감지     │
│  │   └─ PendingObject - AddObject 버퍼링       │
│  │                                             │
│  ├─ CScriptEngine (기존 VBS 엔진, COM)         │
│  │   └─ Windows Script Host 기반               │
│  │                                             │
│  └─ CPythonEngine (신규 Python 엔진)           │
│      ├─ Python C API (Py_Initialize 등)       │
│      ├─ AxisObject - IDispatch 래핑            │
│      ├─ AxisMethod - 메서드/속성 호출          │
│      └─ 바이너리 마커(\x0E[\x0C) 제거         │
│                                                 │
│  ┌─ COM 객체 (IDispatch)                       │
│  │   ├─ Screen - 현재 화면                     │
│  │   ├─ System - 시스템 정보                   │
│  │   ├─ Login - 로그인 정보                    │
│  │   ├─ Info - INI 접근                        │
│  │   └─ 컨트롤 (usid, BUTTON0 등)             │
│  └─                                            │
└─────────────────────────────────────────────────┘
```

---

## 3. 핵심 인터페이스

### CEngineWrapper (엔진 추상화)

**목적:** VBS/Python 엔진을 런타임에 자동 선택하여 기존 코드(Screen/Script) 수정 불필요

**공개 인터페이스:**
```cpp
class CEngineWrapper {
    // Wizard에서 호출
    void Initialize();
    void AddObject(const char* name, IDispatch* disp);
    void LoadScript(const char* code);
    void DoProcedure(const char* procName);
    VARIANT GetVariable(const char* varName);
};
```

**선택 로직:**
- 스크립트 내용 스캔: `def ` 또는 `import ` 포함
- → CPythonEngine 사용
- → CScriptEngine 사용 (기존 VBS 유지)

**지연 초기화:**
- AddObject는 PendingObject 버퍼에 보관
- LoadScript 호출 시 엔진 생성 및 일괄 등록

### CPythonEngine (Python C API 기반)

**핵심 클래스:**

1. **AxisObject** (Python 래퍼)
   - CCmdTarget(IDispatch)를 Python 객체로 변환
   - tp_setattro: 속성 대입 시 DISPATCH_PROPERTYPUT
   - tp_call: 메서드 호출 시 IDispatch::Invoke

2. **AxisMethod** (메서드 호출)
   - IDispatch::Invoke 랩핑
   - 반환값 자동 변환 (VARIANT ↔ PyObject)

3. **LoadScript()**
   - 바이너리 마커(\x0E[kind]\x0C) 제거 (중요!)
   - PyRun_String으로 실행
   - Exception → Screen.MessageBox (재진입 방어)

4. **DoProcedure()**
   - m_globals dict에서 함수 검색
   - PyObject_Call로 실행
   - 파라미터 자동 변환

### 바이너리 마커 (중요)

.map 파일의 스크립트 섹션:
```
\x0E [ScriptKind] \x0C [script content]
```

- VBS는 무시하지만 Python은 SyntaxError 발생
- **LoadScript()에서 제거 처리 완료 (2026-06-10)**

---

## 4. 모듈 간 의존성

### 상위 계층 → 하위 계층

```
axisbuilder
  ├─ Python 키워드 하이라이팅 (builder/VBScriptEdit.cpp)
  ├─ [PY] 버튼 (builder/scriptWnd.cpp)
  └─ ScpKind 플래그 저장 (builder/awWcc/Compile.cpp)

axwizard
  ├─ axisvbs.dll 로드
  ├─ CEngineWrapper::Initialize()
  ├─ Screen.cpp: AddObject, LoadScript
  ├─ Script.cpp: DoProcedure
  └─ xscreen.cpp: 포함

axisvbs.dll
  ├─ Python 3.11.6 (python311.lib, python311.dll)
  ├─ Windows Script Host (기존 VBS)
  └─ COM 기반 IDispatch
```

### 외부 의존성

| 라이브러리 | 용도 | 포함 |
|-----------|------|------|
| python311.lib | Python C API 링크 | axisvbs.vcxproj |
| python311.dll | 런타임 (HTS exe 폴더) | ✓ 필수 |
| vcruntime140.dll | VC++ 런타임 (HTS exe 폴더) | ✓ 필수 |
| kernel32.lib | Win32 API | ✓ 기본 |
| ole32.lib | COM 기본 | ✓ 기본 |

---

## 5. 데이터 흐름 (이벤트 처리)

```
[사용자 이벤트 (클릭, 입력 등)]
        │
        ▼
  [axwizard] Script.cpp
        │ OnButtonClick() 등
        ▼
  DoProcedure("OnClick")
        │
        ▼
  [axisvbs.dll] CEngineWrapper
        │ 엔진 확인 (VBS vs Python)
        ▼
  [CPythonEngine] DoProcedure()
        │ m_globals 검색
        ▼
  PyObject_Call(함수)
        │
        ▼
  [Python 스크립트] def OnClick():
        │   ...
        │   Screen.SetData("label", value)  ← AddObject("Screen", ...)
        │   ...
        ▼
  [IDispatch::Invoke] DISPATCH_METHOD
        │
        ▼
  [axwizard] Screen 업데이트
        │
        ▼
  화면 렌더링
```

---

## 6. COM 객체 명칭 (Python 대소문자 구분 주의)

| 이름 | 등록명 | 설명 | 예시 |
|------|--------|------|------|
| `Screen` | `"Screen"` | 현재 화면 객체 | `Screen.SetData("field", val)` |
| `System` | `"System"` | 시스템 정보 | `System.MessageBox("msg")` |
| `Login` | `"Login"` | 로그인 정보 | `Login.GetUserID()` |
| `Info` | `"Info"` | INI 파일 | `Info.GetValue("section", "key")` |
| 컨트롤명 | 맵소스 정의명 | 스크린 컨트롤 | `usid.GetValue()`, `BUTTON0.Click()` |

> **주의:** VBScript는 대소문자 무시, Python은 구분!
> 기존 VBS `SCREEN.setdata()`를 `Screen.SetData()`로 변환 필요.

---

## 7. 파일 구성

### axisvbs.dll (ibks/dll/vbs/)

| 파일 | 역할 | 라인 수 |
|------|------|--------|
| `pythonEngine.h` | CPythonEngine 선언 | - |
| `pythonEngine.cpp` | CPythonEngine 전체 구현 | ~1000+ |
| `engineWrapper.h` | CEngineWrapper 선언, PendingObject 구조 | - |
| `engineWrapper.cpp` | 자동 엔진 선택, 지연 초기화 | ~300+ |
| `scriptEngine.h` | CScriptEngine 선언 (기존 VBS) | - |
| `scriptEngine.cpp` | CScriptEngine 구현 (기존 VBS) | - |
| `axisvbs.vcxproj` | 빌드 설정 | - |

### axwizard (ibks/Wizard/)

| 파일 | 변경 내용 |
|------|----------|
| `Screen.h` | m_vbe: CScriptEngine* → CEngineWrapper* |
| `Screen.cpp` | CEngineWrapper 사용, TEST 블록(사이드카 .py) |
| `Script.cpp` | engineWrapper.h 포함 |
| `xscreen.cpp` | engineWrapper.h 포함 |
| `Wizard.idl` | COM 인터페이스 정의 |

### axisbuilder (builder/)

| 파일 | 변경 내용 |
|------|----------|
| `h/mapform.H` | bool pythonMode 필드 |
| `h/keywords.h` | Python 키워드/상수 |
| `VBScriptEdit.h` | m_bPythonMode, SetPythonMode() |
| `VBScriptEdit.cpp` | Python 키워드 하이라이팅 |
| `scriptWnd.h` | [PY] 버튼, OnPythonClick() |
| `scriptWnd.cpp` | 버튼 UI, pythonMode 반영 |
| `awWcc/BinaryMngr.h` | #define PYTHON 2 |
| `awWcc/Compile.cpp` | pythonMode 기반 ScpKind 설정 |
| `awBuild/mapload.cpp` | pythonMode 자동감지 (2026-06-11) |

---

## 8. 배포 체크리스트

### 필수 DLL (HTS exe 폴더에 배치)

```
exe/
  python311.dll          (Python 런타임)
  vcruntime140.dll       (VC++ 런타임)
  axisvbs.dll            (스크립트 엔진)
  axwizard.dll           (화면 도구)
  [기타 기존 DLL]
```

> **누락 시 오류:** "LoadLibrary error 126"

### 빌드 순서

1. **axisvbs.dll** (python311.lib 필요)
2. **axwizard.dll** (axisvbs.dll 참조)
3. **axisbuilder.exe** (UI 도구)

---

## 9. 알려진 문제 및 해결

| 문제 | 원인 | 해결 |
|------|------|------|
| LoadLibrary error 126 | python311.dll 없음 | exe 폴더에 DLL 복사 |
| C4996 (Py_SetPythonHome) | Python 3.11 API 변경 | PyConfig API로 교체 |
| 모든 화면 Python 실행 | 레지스트리 전역 설정 | 스크립트 내용 자동감지 |
| INFO 객체 오류 (800a01a8) | AddObject가 LoadScript 이후 호출 | PendingObject 버퍼링 |
| 프로그램 크래시 (PyDict_GetItem) | 재진입 + 소멸자 미정리 | IsAvailable 방어, ~CPythonEngine 정리 |
| SYSTEM is not defined | Python 대소문자 구분 | System 사용 (대문자) |
| 바이너리 마커 SyntaxError | .map 파일의 \x0E[\x0C] | LoadScript에서 제거 |

---

## 10. 향후 작업

| 단계 | 작업 | 우선순위 |
|------|------|----------|
| 11 | Screen.cpp TEST 블록(사이드카 .py) 제거 | 높음 |
| 12 | VBScript → Python 일괄 변환 도구 | 중간 |
| - | 실운영 테스트 (5,391개 맵 소스) | 높음 |
| - | 추가 COM 객체 Python 래핑 | 중간 |

---

## 11. 관련 문서

- `@docs/python_engine_260608.md` - VBScript->Python 전환 상세 기록
- `@docs/Dependency.md` - 라이브러리 의존성 분석
- `@docs/SourceIndex.md` - 소스 파일 색인
- `@docs/CallGraph.md` - 함수 호출 흐름도
