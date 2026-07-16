# ibks 소스 파일 색인

## 문서 목적

ibks 프로젝트의 주요 소스 파일을 역할, 클래스/함수, 관련 파일, 사용 API별로 색인화합니다.

---

## 1. 모듈 별 파일 구성

### 1.1 axisvbs.dll - 스크립트 엔진 DLL

#### Python 엔진 (신규)

| 파일 | 역할 | 주요 클래스/함수 | 관련 파일 | 사용 API | 상태 |
|------|------|-----------------|----------|---------|------|
| `pythonEngine.h` | CPythonEngine 선언 | CPythonEngine | pythonEngine.cpp | Python C API | 완료 |
| `pythonEngine.cpp` | Python 엔진 구현 | CPythonEngine, AxisObject, AxisMethod | engineWrapper.cpp | Python.h, IDispatch | 완료 |
| `engineWrapper.h` | 엔진 추상화 선언 | CEngineWrapper, PendingObject | engineWrapper.cpp, Screen.h | Python C API | 완료 |
| `engineWrapper.cpp` | 자동 선택 로직 | CEngineWrapper::LoadScript(), isPythonScript() | scriptEngine.h, pythonEngine.h | IDispatch | 완료 |

#### VBS 엔진 (기존)

| 파일 | 역할 | 주요 클래스 | 관련 파일 | 사용 API | 상태 |
|------|------|-----------|----------|---------|------|
| `scriptEngine.h` | CScriptEngine 선언 | CScriptEngine | scriptEngine.cpp | COM, IDispatch | 유지 |
| `scriptEngine.cpp` | VBS 엔진 구현 | CScriptEngine | engineWrapper.cpp | Windows Script Host | 유지 |

#### 빌드 설정

| 파일 | 역할 | 포함 내용 | 상태 |
|------|------|----------|------|
| `axisvbs.vcxproj` | 프로젝트 설정 | Python include/lib 경로, 신규 파일 등록 | 완료 |
| `axisvbs.def` | DLL export 정의 | CEngineWrapper export | 필요시 업데이트 |

---

### 1.2 axwizard (ibks/Wizard/) - 화면 런타임

#### 핵심 파일

| 파일 | 역할 | 주요 클래스/함수 | 관련 파일 | 사용 API | 상태 |
|------|------|-----------------|----------|---------|------|
| `Screen.h` | 화면 컨트롤 선언 | CScreen | Screen.cpp, engineWrapper.h | MFC, IDispatch | 수정 완료 |
| `Screen.cpp` | 화면 컨트롤 구현 | CScreen::Initialize(), AddObject(), LoadScript() | Script.cpp, engineWrapper.h | MFC, COM | 수정 완료 |
| `Script.h` | 스크립트 이벤트 선언 | CScript | Script.cpp | - | 유지 |
| `Script.cpp` | 이벤트 핸들러 구현 | CScript::OnClick(), DoProcedure() | engineWrapper.h | IDispatch | 수정 완료 |
| `xscreen.cpp` | 확장 화면 구현 | xscreen::* | Screen.h, engineWrapper.h | MFC | 수정 완료 |

#### UI 지원 파일

| 파일 | 역할 | 주요 함수 | 관련 파일 |
|------|------|---------|----------|
| `Objects.h/cpp` | 화면 객체 관리 | CObjects | Screen.cpp |
| `Dde.h/cpp` | DDE 통신 | CDde | Wizard.cpp |
| `Log.h/cpp` | 로깅 | CLog | 전역 |
| `Misc.h/cpp` | 유틸리티 | Misc::* | 전역 |
| `Printer.h/cpp` | 인쇄 기능 | CPrinter | - |
| `Tips.h/cpp` | 팁 표시 | CTips | - |

#### 리소스 파일

| 파일 | 용도 |
|------|------|
| `Wizard.rc` | 다이얼로그, 아이콘, 메뉴 |
| `Wizard.idl` | COM 인터페이스 정의 |
| `Resource.h` | 리소스 ID |

---

### 1.3 axisbuilder (builder/) - 화면 편집기

#### Python 모드 지원 (신규/수정)

| 파일 | 역할 | 변경 내용 | 상태 |
|------|------|----------|------|
| `h/mapform.H` | 맵 폼 구조 | bool pythonMode 필드 추가 | 완료 |
| `h/keywords.h` | 키워드 정의 | Python 키워드/상수 추가 | 완료 |
| `VBScriptEdit.h` | 에디터 선언 | m_bPythonMode, SetPythonMode() | 완료 |
| `VBScriptEdit.cpp` | 에디터 구현 | Python 키워드 하이라이팅 | 완료 |
| `scriptWnd.h` | 스크립트 창 선언 | m_pythonBtn, OnPythonClick() | 완료 |
| `scriptWnd.cpp` | 스크립트 창 구현 | [PY] 버튼 UI, Initialize 반영 | 완료 |

#### 빌드/컴파일

| 파일 | 역할 | 변경 내용 | 상태 |
|------|------|----------|------|
| `awWcc/BinaryMngr.h` | 바이너리 관리 | #define PYTHON 2 추가 | 완료 |
| `awWcc/Compile.cpp` | 컴파일 로직 | pythonMode 기반 ScpKind 설정 | 완료 |
| `awBuild/mapload.cpp` | 맵 로더 | pythonMode 자동감지 로직 (2026-06-11) | 완료 |

#### 레이아웃/UI

| 파일 | 역할 | 관련 파일 |
|------|------|----------|
| `MainFrm.h/cpp` | 메인 프레임 | 전체 UI |
| `ChildView.h/cpp` | 자식 뷰 | 맵 렌더링 |
| `ViewBar.h/cpp` | 뷰 바 | UI 도구모음 |

---

## 2. 계층 별 파일 분류

### 계층 1: 엔진 (axisvbs.dll)

```
ibks/dll/vbs/
  ├── pythonEngine.h/cpp          (CPythonEngine)
  ├── engineWrapper.h/cpp         (CEngineWrapper, 자동 선택)
  ├── scriptEngine.h/cpp          (CScriptEngine, VBS)
  └── axisvbs.vcxproj
```

**책임:**
- Python/VBS 스크립트 실행
- COM 객체(IDispatch) 래핑
- 엔진 선택 및 생명주기 관리

---

### 계층 2: 런타임 (axwizard)

```
ibks/Wizard/
  ├── Screen.h/cpp               (화면 컨트롤)
  ├── Script.h/cpp               (이벤트 핸들러)
  ├── xscreen.cpp                (확장 화면)
  ├── Objects.h/cpp              (객체 관리)
  ├── Log.h/cpp                  (로깅)
  └── Wizard.rc/.idl/.def
```

**책임:**
- MFC 윈도우 생성 및 렌더링
- 이벤트 처리
- 엔진(axisvbs.dll) 호출

---

### 계층 3: 편집기 (axisbuilder)

```
builder/
  ├── h/mapform.H                (pythonMode 필드)
  ├── h/keywords.h               (Python 키워드)
  ├── VBScriptEdit.h/cpp         (에디터)
  ├── scriptWnd.h/cpp            ([PY] 버튼)
  ├── MainFrm.h/cpp              (메인 UI)
  └── awWcc/
  │   ├── BinaryMngr.h           (PYTHON=2)
  │   └── Compile.cpp            (ScpKind 설정)
  └── awBuild/
      └── mapload.cpp            (pythonMode 자동감지)
```

**책임:**
- .map 파일 편집
- 화면/스크립트 합성
- Python/VBS 모드 구분

---

## 3. 주요 클래스 색인

### Python 엔진 클래스

| 클래스 | 파일 | 용도 | 주요 메서드 |
|--------|------|------|-----------|
| **CPythonEngine** | pythonEngine.h/cpp | Python 스크립트 실행 | Initialize(), LoadScript(), DoProcedure(), AddObject() |
| **AxisObject** | pythonEngine.cpp | IDispatch → Python 래퍼 | tp_getattro, tp_setattro, tp_call (Python type) |
| **AxisMethod** | pythonEngine.cpp | IDispatch 메서드 호출 | PyObject_Call (Python type) |
| **CEngineWrapper** | engineWrapper.h/cpp | VBS/Python 자동 선택 | LoadScript(), isPythonScript(), Create() |
| **PendingObject** | engineWrapper.h | AddObject 버퍼 | (구조체) |

### VBS 엔진 클래스

| 클래스 | 파일 | 용도 | 주요 메서드 |
|--------|------|------|-----------|
| **CScriptEngine** | scriptEngine.h/cpp | VBS 스크립트 실행 | Initialize(), LoadScript(), DoProcedure(), AddObject() |

### 화면 클래스

| 클래스 | 파일 | 용도 | 주요 메서드 |
|--------|------|------|-----------|
| **CScreen** | Screen.h/cpp | 화면 컨트롤 | Initialize(), AddObject(), LoadScript(), Render() |
| **CScript** | Script.h/cpp | 이벤트 핸들러 | OnClick(), OnTimer(), DoProcedure() |
| **CObjects** | Objects.h/cpp | 객체 컬렉션 | Add(), Find(), Remove() |

---

## 4. 중요 함수 색인

### CPythonEngine 핵심 함수

| 함수 | 파일 | 역할 |
|------|------|------|
| `CPythonEngine::Initialize()` | pythonEngine.cpp | Python 인터프리터 초기화 |
| `CPythonEngine::LoadScript()` | pythonEngine.cpp | 바이너리 마커 제거 후 스크립트 실행 |
| `CPythonEngine::DoProcedure()` | pythonEngine.cpp | 함수 호출 (m_globals dict 검색) |
| `CPythonEngine::AddObject()` | pythonEngine.cpp | IDispatch → AxisObject 생성 |
| `AxisObject_setattro()` | pythonEngine.cpp | 속성 대입 (DISPATCH_PROPERTYPUT) |
| `AxisMethod_call()` | pythonEngine.cpp | 메서드 호출 (IDispatch::Invoke) |

### CEngineWrapper 핵심 함수

| 함수 | 파일 | 역할 |
|------|------|------|
| `CEngineWrapper::LoadScript()` | engineWrapper.cpp | 엔진 자동 선택 (isPythonScript) |
| `isPythonScript()` | engineWrapper.cpp | def/import 감지 |
| `CEngineWrapper::Create()` | engineWrapper.cpp | 지연 엔진 생성 |

### CScreen 핵심 함수

| 함수 | 파일 | 역할 |
|------|------|------|
| `CScreen::Initialize()` | Screen.cpp | 화면 초기화, 엔진 생성 |
| `CScreen::AddObject()` | Screen.cpp | 엔진에 COM 객체 등록 |
| `CScreen::LoadScript()` | Screen.cpp | 스크립트 로드 |

---

## 5. 파일 간 호출 관계

```
┌─ Screen.cpp::Initialize()
│   └─ CEngineWrapper::Create()
│       ├─ isPythonScript() → CPythonEngine::Initialize()
│       └─ CScriptEngine::Initialize()
│
├─ Screen.cpp::AddObject()
│   └─ CEngineWrapper::AddObject()
│       ├─ CPythonEngine::AddObject() → AxisObject 생성
│       └─ CScriptEngine::AddObject()
│
├─ Screen.cpp::LoadScript()
│   └─ CEngineWrapper::LoadScript()
│       ├─ 바이너리 마커 제거
│       ├─ CPythonEngine::LoadScript() → PyRun_String()
│       └─ CScriptEngine::LoadScript()
│
└─ Script.cpp::OnClick()
    └─ CEngineWrapper::DoProcedure()
        ├─ CPythonEngine::DoProcedure() → PyObject_Call()
        └─ CScriptEngine::DoProcedure()
```

---

## 6. 외부 의존 API

### Python C API

| API | 파일 | 용도 |
|-----|------|------|
| `Py_Initialize()` | pythonEngine.cpp | 인터프리터 초기화 |
| `PyConfig_InitPythonConfig()` | pythonEngine.cpp | Python 3.11 설정 |
| `PyRun_String()` | pythonEngine.cpp | 스크립트 실행 |
| `PyDict_GetItem()` | pythonEngine.cpp | 함수 검색 |
| `PyObject_Call()` | pythonEngine.cpp | 함수 호출 |
| `PyGC_Collect()` | pythonEngine.cpp | 가비지 컬렉션 |

### COM/Windows API

| API | 사용 파일 | 용도 |
|-----|----------|------|
| `IDispatch::Invoke()` | pythonEngine.cpp, scriptEngine.cpp | 메서드/속성 호출 |
| `CoCreateInstance()` | axisvbs.vcxproj | COM 객체 생성 |
| `MessageBox()` | pythonEngine.cpp | 오류 표시 |
| `GetIDispatch()` | Screen.cpp | COM 객체 획득 |

### MFC API

| API | 사용 파일 | 용도 |
|-----|----------|------|
| `CCmdTarget` | Screen.h/cpp | MFC 명령 대상 |
| `CWnd` | Screen.h/cpp | MFC 윈도우 |
| `CDialogEx` | scriptWnd.cpp | MFC 다이얼로그 |

---

## 7. 빌드 의존성

### axisvbs.dll 빌드

**입력 파일:**
```
pythonEngine.h/cpp
engineWrapper.h/cpp
scriptEngine.h/cpp
axisvbs.vcxproj
```

**링크 라이브러리:**
```
python311.lib
ole32.lib
oleaut32.lib
kernel32.lib
user32.lib
```

**출력:**
```
axisvbs.dll
axisvbs.lib (import library)
```

### axwizard.dll 빌드 (axisvbs.dll 이후)

**입력 파일:**
```
Screen.h/cpp
Script.h/cpp
xscreen.cpp
Objects.h/cpp
Wizard.rc
Wizard.idl
```

**링크 라이브러리:**
```
axisvbs.lib
mfc*.lib
ole32.lib
```

**출력:**
```
axwizard.dll
```

### axisbuilder.exe 빌드

**입력 파일:**
```
VBScriptEdit.h/cpp
scriptWnd.h/cpp
mapform.H
keywords.h
MainFrm.h/cpp
```

**링크 라이브러리:**
```
awWcc.lib
awBuild.lib
mfc*.lib
```

**출력:**
```
axisbuilder.exe
```

---

## 8. 파일 크기 및 복잡도 (추정)

| 파일 | 라인 수 (추정) | 복잡도 | 주요 이슈 |
|------|--------|--------|----------|
| pythonEngine.cpp | ~1000+ | 높음 | GC 순환참조, 재진입 방어 |
| engineWrapper.cpp | ~300+ | 중간 | 엔진 선택 로직 |
| scriptEngine.cpp | ~600+ | 중간 | VBS 호환성 |
| Screen.cpp | ~800+ | 높음 | 이벤트 처리, 그리기 |
| Script.cpp | ~500+ | 중간 | 핸들러 등록 |
| VBScriptEdit.cpp | ~400+ | 중간 | 문법 하이라이팅 |

---

## 9. TEST/DEBUG 코드

### TEST 블록 (제거 예정)

| 파일 | 위치 | 내용 | 상태 |
|------|------|------|------|
| Screen.cpp | ~400줄 | 사이드카 .py 파일 로드 | 미제거 (단계 11) |

---

## 10. 알려진 제한사항

| 파일 | 제한사항 | 해결 |
|------|--------|------|
| pythonEngine.cpp | sys.path 설정 필요 | PyConfig API로 sys.path 설정 |
| CPythonEngine | 스레드 안전성 | GIL 관리 필요 (단일 스레드 추천) |
| AxisObject | 상속 불지원 | Python에서 상속 불가 |

---

## 11. 관련 문서

- `@docs/Architecture.md` - 모듈 구조 및 인터페이스
- `@docs/Dependency.md` - 라이브러리 의존성
- `@docs/python_engine_260608.md` - 전환 프로젝트 상세 기록
- `@docs/CallGraph.md` - 함수 호출 흐름도 (작성 예정)
