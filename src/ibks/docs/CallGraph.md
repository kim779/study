# ibks 함수 호출 흐름 분석


## 목차

- [문서 목적](#문서-목적)
- [1. 전체 호출 흐름도](#1-전체-호출-흐름도)
- [2. 세부 호출 흐름](#2-세부-호출-흐름)
  - [2.1 초기화 단계](#21-초기화-단계)
  - [2.2 AddObject 단계 (객체 등록)](#22-addobject-단계-객체-등록)
  - [2.3 LoadScript 단계 (스크립트 실행)](#23-loadscript-단계-스크립트-실행)
  - [2.4 DoProcedure 단계 (함수 호출)](#24-doprocedure-단계-함수-호출)
  - [2.5 Python 스크립트 → COM 메서드 호출](#25-python-스크립트-com-메서드-호출)
  - [2.6 가비지 컬렉션 (순환참조 해결)](#26-가비지-컬렉션-순환참조-해결)
- [3. 엔진 선택 로직 (핵심)](#3-엔진-선택-로직-핵심)
- [4. 예외 처리 흐름](#4-예외-처리-흐름)
  - [Python 예외](#python-예외)
  - [VBS 예외](#vbs-예외)
- [5. axisbuilder 호출 흐름](#5-axisbuilder-호출-흐름)
  - [편집 모드](#편집-모드)
  - [빌드 모드](#빌드-모드)
- [6. 데이터 전달 경로](#6-데이터-전달-경로)
  - [Screen.SetData 예시](#screensetdata-예시)
  - [반환값 처리](#반환값-처리)
- [7. 멀티 스레드 고려사항](#7-멀티-스레드-고려사항)
  - [현재 구조](#현재-구조)
- [8. 성능 프로파일링 포인트](#8-성능-프로파일링-포인트)
- [9. 관련 문서](#9-관련-문서)

---

## 문서 목적

ibks 프로젝트의 진입점(axwizard, axisbuilder)부터 시작하는 주요 호출 흐름을 Mermaid 다이어그램으로 추적합니다.

---

## 1. 전체 호출 흐름도

```mermaid
graph TD
    A["axwizard (화면 런타임)"] -->|1. Initialize| B["Screen::Initialize"]
    B -->|2. 엔진 생성| C["CEngineWrapper::Create"]
    C -->|isPythonScript검사| D{"def/import<br/>존재?"}
    D -->|Yes| E["CPythonEngine::Initialize<br/>Py_Initialize"]
    D -->|No| F["CScriptEngine::Initialize<br/>WSH 초기화"]
    
    B -->|3. 컨트롤 등록| G["Screen::AddObject<br/>PendingObject 버퍼"]
    G -->|4. 버퍼 저장| H["m_pendingObjects"]
    
    A -->|이벤트발생| I["Script.cpp<br/>OnClick, OnTimer 등"]
    I -->|5. 스크립트 로드| J["Screen::LoadScript"]
    J -->|6. 자동 감지| K["CEngineWrapper::LoadScript"]
    K -->|isPythonScript| L{"def/import<br/>존재?"}
    L -->|Yes| M["CPythonEngine::LoadScript<br/>바이너리 마커 제거<br/>PyRun_String 실행<br/>버퍼된 객체 일괄 등록"]
    L -->|No| N["CScriptEngine::LoadScript<br/>VBS 파싱"]
    
    I -->|7. 핸들러 호출| O["Script::DoProcedure<br/>함수명 전달"]
    O -->|8. 엔진 호출| P["CEngineWrapper::DoProcedure"]
    P -->|엔진 확인| Q{"Python<br/>또는<br/>VBS?"}
    Q -->|Python| R["CPythonEngine::DoProcedure<br/>m_globals dict 검색<br/>PyObject_Call 실행"]
    Q -->|VBS| S["CScriptEngine::DoProcedure<br/>WSH 엔진 호출"]
    
    R -->|9. COM 메서드 호출| T["AxisObject_setattro<br/>또는<br/>AxisMethod_call"]
    S -->|9. COM 메서드 호출| T
    T -->|10. IDispatch::Invoke| U["Screen 객체/컨트롤<br/>속성/메서드"]
    U -->|11. 결과 반환| V["VARIANT"]
    V -->|12. Python으로 변환| W["PyObject"]
    W -->|13. 결과 반환| X["Script.cpp 처리"]
    
    style A fill:#99ff99
    style E fill:#ffcccc
    style F fill:#ffcccc
    style M fill:#ffcccc
    style N fill:#ffcccc
    style R fill:#ffcccc
    style S fill:#ffcccc
    style U fill:#99ccff
```

---

## 2. 세부 호출 흐름

### 2.1 초기화 단계

```mermaid
graph LR
    A["axwizard::DllMain"] -->|프로세스 시작| B["axwizard 로드"]
    B -->|COM 등록| C["IUnknown::QueryInterface"]
    C -->|Screen 인스턴스| D["CScreen 생성"]
    D -->|Initialize 호출| E["CEngineWrapper::Create"]
    E -->|파이썬 감지 시작| F["isPythonScript 확인"]
    F -->|def/import 없음| G["CScriptEngine::Initialize<br/>Windows Script Host 초기화"]
    F -->|def/import 있음| H["CPythonEngine::Initialize<br/>Py_Initialize<br/>PyConfig_InitPythonConfig"]
    H -->|Python 환경 설정| I["sys.path 설정"]
    I -->|모듈 검색 경로| J["맵소스 경로 추가"]
    
    style E fill:#ffcccc
    style F fill:#ffff99
    style G fill:#ccccff
    style H fill:#ffcccc
```

### 2.2 AddObject 단계 (객체 등록)

```mermaid
graph LR
    A["Screen::Initialize"] -->|Screen 객체| B["Screen::AddObject<br/>name='Screen'"]
    B -->|엔진 미생성| C["PendingObject 버퍼"]
    C -->|저장| D["m_pendingObjects"]
    
    A -->|System 객체| E["Screen::AddObject<br/>name='System'"]
    E -->|버퍼| F["m_pendingObjects"]
    
    A -->|컨트롤 객체| G["Screen::AddObject<br/>name='usid'<br/>name='BUTTON0'"]
    G -->|버퍼| H["m_pendingObjects"]
    
    D -->|LoadScript 호출 후| I["CEngineWrapper::LoadScript<br/>일괄 등록"]
    F -->|엔진 생성 후| I
    H -->|객체 준비 후| I
    I -->|CPythonEngine::AddObject| J["AxisObject 래핑<br/>Python dict에 등록"]
    I -->|CScriptEngine::AddObject| K["WSH dict에 등록"]
    
    style C fill:#ffff99
    style D fill:#ffff99
    style I fill:#ccccff
    style J fill:#ffcccc
    style K fill:#ccccff
```

### 2.3 LoadScript 단계 (스크립트 실행)

```mermaid
graph LR
    A["Screen::LoadScript<br/>binary .map 데이터"]
    A -->|바이너리 스크립트| B["CEngineWrapper::LoadScript"]
    B -->|마커 검사| C{"바이너리 마커<br/>\\x0E[kind]\\x0C<br/>존재?"}
    
    C -->|Yes| D["마커 제거<br/>클린 텍스트"]
    C -->|No| E["그냥 진행"]
    
    D -->|isPythonScript| F{"def<br/>또는<br/>import?"}
    E -->|isPythonScript| F
    
    F -->|Yes| G["CPythonEngine::LoadScript"]
    F -->|No| H["CScriptEngine::LoadScript"]
    
    G -->|PyRun_String| I["스크립트 컴파일<br/>및 실행"]
    H -->|WSH AddCode| J["VBS 등록"]
    
    I -->|예외처리| K["PyErr_Print<br/>MessageBox"]
    J -->|예외처리| L["WSH Error<br/>MessageBox"]
    
    I -->|PendingObject<br/>등록| M["CPythonEngine::AddObject<br/>정제된 객체 일괄 등록"]
    
    style B fill:#ccccff
    style D fill:#ffff99
    style F fill:#ffff99
    style G fill:#ffcccc
    style H fill:#ccccff
    style M fill:#99ff99
```

### 2.4 DoProcedure 단계 (함수 호출)

```mermaid
graph LR
    A["사용자 이벤트"] -->|OnClick 등| B["Script::OnClick"]
    B -->|함수명| C["CEngineWrapper::DoProcedure<br/>procName='OnClick'"]
    
    C -->|엔진 확인| D{"엔진<br/>타입?"}
    
    D -->|Python| E["CPythonEngine::DoProcedure<br/>m_globals dict 검색"]
    D -->|VBS| F["CScriptEngine::DoProcedure<br/>WSH dict 검색"]
    
    E -->|함수 찾음| G["PyObject_Call<br/>인자: 없음"]
    E -->|함수 없음| H["PyErr: NameError"]
    
    F -->|함수 찾음| I["WSH InvokeMethod"]
    F -->|함수 없음| J["WSH Error"]
    
    H -->|예외 처리| K["try-except<br/>또는<br/>MessageBox"]
    J -->|예외 처리| L["Error 핸들러"]
    
    G -->|실행 중| M["def OnClick():<br/>Screen.SetData('label', 'value')"]
    M -->|Python → IDispatch| N["AxisObject_setattro"]
    
    style C fill:#ccccff
    style D fill:#ffff99
    style E fill:#ffcccc
    style F fill:#ccccff
    style N fill:#99ff99
```

### 2.5 Python 스크립트 → COM 메서드 호출

```mermaid
graph LR
    A["Python 스크립트"] -->|Screen.SetData| B["AxisObject<br/>attribute access"]
    B -->|tp_setattro<br/>호출| C["Python 타입 슬롯"]
    C -->|속성명 'SetData'| D["AxisObject_setattro<br/>처리"]
    D -->|DISPATCH_PROPERTYPUT<br/>또는<br/>DISPATCH_METHOD| E["IDispatch::Invoke"]
    E -->|Screen COM 객체| F["CScreen::SetData<br/>실제 메서드 호출"]
    F -->|값 처리| G["화면 업데이트"]
    
    style B fill:#ffcccc
    style C fill:#ffcccc
    style D fill:#99ff99
    style E fill:#99ccff
    style F fill:#99ccff
```

### 2.6 가비지 컬렉션 (순환참조 해결)

```mermaid
graph LR
    A["Python 함수 호출<br/>COM 객체 참조"] -->|참조 순환| B["COM 객체 → IDispatch<br/>↓<br/>Python AxisObject<br/>↓<br/>다시 COM 참조"]
    B -->|순환 감지| C["PyGC_Collect<br/>강제 GC 실행"]
    C -->|생명주기 관리| D["메모리 해제"]
    D -->|안전한 소멸자| E["~CPythonEngine<br/>Py_FinalizeEx"]
    
    style C fill:#ffff99
    style E fill:#99ff99
```

---

## 3. 엔진 선택 로직 (핵심)

```mermaid
graph TD
    A["LoadScript 호출"] -->|스크립트 텍스트| B["CEngineWrapper::LoadScript"]
    B -->|바이너리 마커 제거| C["클린 스크립트"]
    C -->|ispythonscript| D["isPythonScript<br/>함수 호출"]
    D -->|패턴 검사| E["'def ' 또는<br/>'import ' 포함?"]
    E -->|포함 → Python| F["CPythonEngine<br/>사용"]
    E -->|미포함 → VBS| G["CScriptEngine<br/>사용"]
    F -->|첫 실행 시만| H["CPythonEngine::Initialize<br/>Py_Initialize"]
    G -->|첫 실행 시만| I["CScriptEngine::Initialize<br/>WSH 초기화"]
    H -->|이후 호출| J["PyRun_String<br/>스크립트 재실행 X<br/>함수만 호출"]
    I -->|이후 호출| K["WSH DoProcedure<br/>함수만 호출"]
    
    style E fill:#ffff99
    style F fill:#ffcccc
    style G fill:#ccccff
```

---

## 4. 예외 처리 흐름

### Python 예외

```mermaid
graph LR
    A["PyRun_String<br/>또는<br/>PyObject_Call"] -->|예외 발생| B["PyErr_Occurred"]
    B -->|Error 있음| C["PyErr_Print<br/>에러 출력"]
    C -->|DisplayException| D["Screen 재진입<br/>방어 체크"]
    D -->|재진입 아님| E["MessageBox<br/>에러 표시"]
    D -->|재진입 중| F["로그 기록만"]
    E -->|사용자 확인| G["흐름 계속"]
    F -->|조용히 처리| G
    
    style B fill:#ffff99
    style D fill:#ffff99
    style E fill:#ff9999
```

### VBS 예외

```mermaid
graph LR
    A["CScriptEngine<br/>메서드 호출"] -->|예외 발생| B["WSH IActiveScriptSite<br/>OnScriptError"]
    B -->|Error 있음| C["문맥 정보 수집"]
    C -->|라인 번호 등| D["MessageBox<br/>VBS 오류 표시"]
    D -->|사용자 확인| E["흐름 계속"]
    
    style B fill:#ffff99
    style D fill:#ff9999
```

---

## 5. axisbuilder 호출 흐름

### 편집 모드

```mermaid
graph LR
    A["axisbuilder.exe<br/>시작"] -->|맵 파일 열기| B["mapload.cpp<br/>BuildXmlResource"]
    B -->|폼 스크립트 순회| C["mapData 이벤트<br/>mapObject 이벤트"]
    C -->|def/import 검사| D{"Python<br/>스크립트?"}
    D -->|Yes| E["pythonMode=true<br/>자동 설정"]
    D -->|No| F["pythonMode=false<br/>유지"]
    E -->|저장| G["mapform.pythonMode"]
    F -->|저장| G
    G -->|편집 시| H["VBScriptEdit"]
    H -->|pythonMode 확인| I{"pythonMode<br/>ON?"}
    I -->|Yes| J["Python 키워드 하이라이팅<br/>h/keywords.h 사용"]
    I -->|No| K["VB 키워드 하이라이팅"]
    J -->|UI| L["scriptWnd [PY] 버튼<br/>토글 표시"]
    K -->|UI| L
    
    style B fill:#99ccff
    style E fill:#99ff99
    style J fill:#ffcccc
```

### 빌드 모드

```mermaid
graph LR
    A["axisbuilder 빌드"] -->|컴파일| B["Compile.cpp<br/>스크립트 처리"]
    B -->|pythonMode 확인| C{"pythonMode<br/>ON?"}
    C -->|Yes| D["ScpKind = PYTHON<br/>값=2"]
    C -->|No| E["ScpKind = VBS<br/>값=1"]
    D -->|바이너리 생성| F[".map 파일<br/>ScpKind=2 기록"]
    E -->|바이너리 생성| G[".map 파일<br/>ScpKind=1 기록"]
    F -->|빌드 완료| H["axisbuilder → axwizard<br/>전달"]
    G -->|빌드 완료| H
    
    style C fill:#ffff99
    style D fill:#99ff99
    style E fill:#99ccff
```

---

## 6. 데이터 전달 경로

### Screen.SetData 예시

```
Python 스크립트:
  Screen.SetData("fieldname", "value")
           ↓
  AxisObject_setattro("SetData", value)
           ↓
  IDispatch::Invoke(DISPATCH_METHOD)
           ↓
  CScreen::SetData(fieldname, value)
           ↓
  컨트롤 찾기 (Objects 컬렉션)
           ↓
  컨트롤.SetData(value)
           ↓
  화면 업데이트 요청
           ↓
  WM_PAINT 메시지
           ↓
  CScreen::OnPaint()
           ↓
  화면 렌더링
```

### 반환값 처리

```
CScreen::GetValue() → VARIANT
           ↓
  Python 타입 변환
           ↓
  PyObject (int/str/float/bool 등)
           ↓
  Python 함수에 반환
           ↓
  result = Screen.GetValue("field")
```

---

## 7. 멀티 스레드 고려사항

### 현재 구조

```
Main Thread (MFC 메시지 루프)
  ├─ Screen::OnClick
  ├─ DoProcedure → Python/VBS
  └─ 화면 렌더링 (OnPaint)

Worker Thread (선택사항)
  ├─ 실시간 데이터 수신
  └─ Screen.SetData(...) 호출
      ↓ 위험: GIL 락 필요
```

**주의:** CPythonEngine은 GIL(Global Interpreter Lock)을 사용하므로, Worker 스레드에서 호출 시 `PyGILState_Ensure/Release` 필수.

---

## 8. 성능 프로파일링 포인트

| 단계 | 오버헤드 | 최적화 여지 |
|------|---------|-----------|
| Py_Initialize | ~100-200ms | 1회만 (지연 초기화) |
| LoadScript (컴파일) | ~50-200ms | 1회만 (코드 캐싱) |
| DoProcedure | ~1-10ms | 호출마다 dict 검색 |
| AxisObject_setattro | ~0.1-1ms | IDispatch::Invoke 오버헤드 |
| 화면 렌더링 | ~10-50ms | UI 최적화 필요 |

---

## 9. 관련 문서

- `@docs/Architecture.md` - 모듈 계층 구조
- `@docs/SourceIndex.md` - 소스 파일별 함수 색인
- `@docs/python_engine_260608.md` - Python 엔진 전환 상세 기록
