---
project: ibks
category: other
status: 완료
updated: 2026-08-21
---

# VBScript -> Python Engine 전환 프로젝트 (2026-06-08 ~)


## 목차

- [목표](#목표)
- [관련 경로](#관련-경로)
- [전체 아키텍처](#전체-아키텍처)
- [엔진 선택 방식](#엔진-선택-방식)
- [Python 환경](#python-환경)
  - [배포 시 필수 파일 (HTS exe 폴더에 복사)](#배포-시-필수-파일-hts-exe-폴더에-복사)
- [수정된 파일 목록](#수정된-파일-목록)
  - [axisvbs.dll (ibks/dll/vbs/)](#axisvbsdll-ibksdllvbs)
  - [axwizard (ibks/Wizard/)](#axwizard-ibkswizard)
  - [axisbuilder (builder/)](#axisbuilder-builder)
- [버그 수정 이력](#버그-수정-이력)
- [CPythonEngine 핵심 구조](#cpythonengine-핵심-구조)
  - [바이너리 마커 제거 (중요)](#바이너리-마커-제거-중요)
- [COM 객체 명칭 (Python 대소문자 주의)](#com-객체-명칭-python-대소문자-주의)
- [axisbuilder Python 모드 사용법](#axisbuilder-python-모드-사용법)
- [진행 현황](#진행-현황)
  - [9단계 상세 (2026-06-11)](#9단계-상세-2026-06-11)
- [다음 작업](#다음-작업)

---

## 목표

axisvbs.dll 내부의 VBScript 엔진(CScriptEngine)을 Python 엔진(CPythonEngine)으로 교체.
Wizard/Builder 쪽 코드는 수정 없이 엔진만 교체 가능하도록 동일한 public 인터페이스 유지.

---

## 관련 경로

| 경로 | 설명 |
|------|------|
| `ibks/dll/vbs/` | axisvbs.dll - VBS/Python 엔진 DLL |
| `ibks/Wizard/` | axwizard - 화면 변환 도구, scriptEngine 사용 |
| `builder/` | axisbuilder - 화면 파일 생성 도구 |
| `D:\src\IBKS\map_src\IB\` | 맵 소스 파일 5,391개 (VBScript) |

---

## 전체 아키텍처

```
axisbuilder (화면 파일 생성)
    |
axwizard (MFC 화면 구성)
    +-- Screen.cpp  -> CEngineWrapper Initialize / AddObject / LoadScript
    +-- Script.cpp  -> 이벤트 발생 시 DoProcedure() 호출
            |
    axisvbs.dll
        +-- CEngineWrapper  (스크립트 내용으로 VBS/Python 자동 선택)
        +-- CScriptEngine   (기존 VBS 엔진 - COM 기반)
        +-- CPythonEngine   (신규 Python 엔진 - Python C API)
```

---

## 엔진 선택 방식

레지스트리 전역 설정 방식 → **스크립트 내용 자동 감지** 방식으로 변경.

```cpp
// engineWrapper.cpp isPythonScript()
return (scripts.Find("def ") >= 0 || scripts.Find("import ") >= 0);
```

- `def ` 또는 `import ` 포함 → CPythonEngine 사용
- 그 외 → CScriptEngine 사용 (기존 VBS 동작 유지)

엔진 생성은 `LoadScript()` 시점으로 지연 (AddObject 호출도 버퍼에 보관 후 일괄 전달).

---

## Python 환경

- Python 3.11.6 32비트 설치 경로: `C:\Users\IBKS\AppData\Local\Programs\Python\Python311-32`
- include: `...\Python311-32\include`
- lib: `...\Python311-32\libs\python311.lib`
- axisvbs.vcxproj에 Debug/Release 모두 경로 등록 완료

### 배포 시 필수 파일 (HTS exe 폴더에 복사)

| 파일 | 출처 |
|------|------|
| `python311.dll` | `C:\Users\IBKS\AppData\Local\Programs\Python\Python311-32\` |
| `vcruntime140.dll` | `C:\Users\IBKS\AppData\Local\Programs\Python\Python311-32\` |

> 누락 시 axisvbs.dll 로드 실패 (LoadLibrary error 126)

---

## 수정된 파일 목록

### axisvbs.dll (ibks/dll/vbs/)

| 파일 | 변경 내용 |
|------|----------|
| `pythonEngine.h` | 신규 - CPythonEngine 선언 |
| `pythonEngine.cpp` | 신규 - CPythonEngine 전체 구현 |
| `engineWrapper.h` | 신규 - CEngineWrapper 선언, PendingObject 구조체 |
| `engineWrapper.cpp` | 신규 - 스크립트 내용 기반 자동 엔진 선택, 지연 초기화 |
| `axisvbs.vcxproj` | Python include/lib 경로 추가, 신규 파일 등록 |

### axwizard (ibks/Wizard/)

| 파일 | 변경 내용 |
|------|----------|
| `Screen.h` | m_vbe 타입: CScriptEngine* -> CEngineWrapper* |
| `Screen.cpp` | CEngineWrapper 사용, .py 사이드카 파일 로드 (TEST 블록) |
| `Script.cpp` | include: scriptEngine.h -> engineWrapper.h |
| `xscreen.cpp` | include: scriptEngine.h -> engineWrapper.h |

### axisbuilder (builder/)

| 파일 | 변경 내용 |
|------|----------|
| `builder/h/mapform.H` | `bool pythonMode` 필드 추가 |
| `builder/h/keywords.h` | `szPythonKeywords`, `szPythonConstants` 추가 |
| `builder/VBScriptEdit.h` | `m_bPythonMode`, `SetPythonMode()` 추가 |
| `builder/VBScriptEdit.cpp` | Python 모드: `#` 주석, Python 키워드 하이라이팅 |
| `builder/scriptWnd.h` | `m_pythonBtn`, `OnPythonClick()`, `SetPythonMode()` 추가 |
| `builder/scriptWnd.cpp` | [PY] 버튼 추가, Initialize 시 pythonMode 반영 |
| `builder/awWcc/BinaryMngr.h` | `#define PYTHON 2` 추가 |
| `builder/awWcc/Compile.cpp` | pythonMode 기반 ScpKind 설정 |

---

## 버그 수정 이력

| 날짜 | 증상 | 원인 | 수정 |
|------|------|------|------|
| 06-08 | LoadLibrary error 126 | python311.dll 없음 | exe 폴더에 DLL 복사 |
| 06-08 | C4996 Py_SetPythonHome deprecated | Python 3.11 API 변경 | PyConfig API로 교체 |
| 06-08 | 모든 화면에서 Python 동작 | 레지스트리 전역 설정 | 스크립트 내용 자동 감지로 변경 |
| 06-08 | INFO 객체 오류 (800a01a8) | AddObject가 LoadScript 이후 호출됨 | PendingObject 버퍼링으로 해결 |
| 06-09 | 프로그램 크래시 (PyDict_GetItem) | MessageBox 재진입 + 소멸자 미정리 | IsAvailable 재진입 방어, ~CPythonEngine 정리, GetIDispatch(TRUE) |
| 06-09 | SYSTEM is not defined | Python 대소문자 구분 (VBS는 무시) | 스크립트에서 System/Screen 사용 |

---

## CPythonEngine 핵심 구조

- `AxisObject`: CCmdTarget(IDispatch)를 Python 객체로 래핑
- `AxisMethod`: 속성/메서드 접근 시 IDispatch::Invoke 호출
- `AxisObject_setattro`: 속성 대입 시 DISPATCH_PROPERTYPUT (`obj.Data = value`)
- `AddObject()`: GetIDispatch(TRUE)로 IDispatch 획득 후 AxisObject 생성
- `LoadScript()`: 바이너리 마커(\x0E[kind]\x0C) 제거 후 PyRun_String 실행
- `DoProcedure()`: m_globals dict에서 함수 찾아 PyObject_Call

### 바이너리 마커 제거 (중요)

.map 바이너리 스크립트 섹션에는 `\x0E [kind] \x0C` 마커가 포함됨.
VBScript는 무시하지만 Python은 SyntaxError 발생 → LoadScript에서 제거 처리.

---

## COM 객체 명칭 (Python 대소문자 주의)

| Python에서 쓸 이름 | AddObject 등록명 | 설명 |
|---|---|---|
| `Screen` | `"Screen"` | 현재 화면 |
| `System` | `"System"` | 시스템 정보 |
| `Login` | `"Login"` | 로그인 정보 |
| `Info` | `"Info"` | INI 파일 접근 |
| 컨트롤명 | 맵소스 정의명 | 예: `usid`, `BUTTON0` |

> VBScript는 대소문자 무시, Python은 구분. 변환 시 주의.

---

## axisbuilder Python 모드 사용법

1. 스크립트 편집창에서 `[PY]` 버튼 클릭 → Python 모드 ON
2. Python 키워드 하이라이팅 적용 (`def`, `import`, `class` 등)
3. `#` 주석 사용 가능
4. 빌드 시 ScpKind=PYTHON(2) 기록
5. axwizard가 `def ` / `import ` 감지 → 자동으로 CPythonEngine 사용

---

## 진행 현황

| 단계 | 작업 | 상태 |
|------|------|------|
| 1 | pythonEngine.h / engineWrapper.h 헤더 작성 | 완료 |
| 2 | pythonEngine.cpp 전체 구현 | 완료 |
| 3 | engineWrapper.cpp - 자동 감지 방식 | 완료 |
| 4 | axisvbs / axwizard 빌드 성공 | 완료 |
| 5 | 사이드카 .py 파일로 Python 동작 확인 | 완료 |
| 6 | 크래시 수정 (재진입/소멸자/AddRef) | 완료 |
| 7 | 바이너리 마커 제거 (사이드카 없이 .map 직접 로드) | 완료 (axisvbs.dll 2026-06-10 빌드) |
| 8 | axisbuilder PY 버튼 / Python 하이라이팅 | 완료 (AxisBuilder.exe 2026-06-11 빌드) |
| 9 | mapload.cpp - 맵 소스에서 pythonMode 자동 감지 | **완료 (2026-06-11)** |
| 10 | axisbuilder에서 Python 스크립트 작성 → 빌드 → axwizard 실행 확인 | **미착수 (다음 작업)** |
| 11 | Screen.cpp TEST 블록(사이드카 .py 로드) 제거 | 미착수 |
| 12 | VBScript → Python 일괄 변환 도구 | 미착수 |

### 9단계 상세 (2026-06-11)

`builder/awBuild/mapload.cpp::BuildXmlResource` 수정:
- `mapData`(form 이벤트) + `mapObject`(컨트롤별 이벤트) 순회
- `def ` 또는 `import ` 발견 시 `m_mapH->pythonMode = true` 자동 설정
- awBuild.dll 재빌드 완료 (exe 폴더 직접 출력, 2026-06-11 20:34)

---

## 다음 작업

1. axisbuilder 실행 → .map 파일 열기 → PY 버튼 클릭 → Python 스크립트 작성 → 빌드
2. axwizard에서 빌드된 .map 실행 → Python 엔진 동작 확인
3. Screen.cpp TEST 블록(사이드카 .py 로드, ~400번 줄) 제거
4. VBScript → Python 변환 스크립트 개발
