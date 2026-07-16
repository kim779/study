# ibks 프로젝트 지식 베이스

## 문서 목적

ibks 프로젝트의 분석, 디버깅, 개발 과정에서 얻은 지식을 누적하고 재사용 가능한 형태로 정리합니다.
삭제하지 않고 계속 병합합니다.

---

## 1. 설계 의도

### VBScript → Python 엔진 전환의 이유

**배경:**
- 기존: Windows Script Host 기반 VBScript (COM 자동화)
- 현재: Python 3.11.6 기반 새 엔진
- 목표: 스크립트 언어 현대화, 커뮤니티 확장성 향상, 5,391개 맵 소스의 마이그레이션

**핵심 결정:**
1. **인터페이스 호환성 유지** - Screen/System/Login/Info COM 객체 그대로 사용
2. **자동 엔진 선택** - `def` / `import` 감지로 런타임 자동 선택
3. **점진적 전환** - 기존 VBS 스크립트는 계속 동작, Python은 새 화면부터 지원

### CEngineWrapper의 역할

```
기존 Wizard/Builder 코드 "수정 불필요"
        ↓
CEngineWrapper (추상화 계층)
        ↓
┌─ CScriptEngine (VBS) ─ 기존 화면
└─ CPythonEngine (Python) ─ 새 화면
```

**이점:**
- 기존 화면 스크립트 0개 수정
- 새 화면 자유롭게 Python 사용
- 점진적 마이그레이션 가능

---

## 2. 아키텍처 설계 상세

### 왜 AddObject 버퍼링을 했는가?

**문제 상황:**
```
순서 1: Initialize() → CEngineWrapper 생성 (엔진 아직 미생성)
순서 2: AddObject("Screen", disp) → 엔진 없음, 실패!
순서 3: LoadScript(code) → 엔진 생성
```

**해결책: PendingObject 버퍼**
```
AddObject: m_pendingObjects 배열에 저장
LoadScript: 엔진 생성 후 일괄 AddObject 호출
```

**코드 예시:**
```cpp
// engineWrapper.cpp
struct PendingObject {
    CString name;
    IDispatch* disp;
};

void CEngineWrapper::AddObject(const char* name, IDispatch* disp) {
    if (!m_engine) {
        // 엔진 미생성 → 버퍼에 저장
        PendingObject po;
        po.name = name;
        po.disp = disp;
        m_pendingObjects.Add(po);
        return;
    }
    // 엔진 생성됨 → 직접 등록
    m_engine->AddObject(name, disp);
}

void CEngineWrapper::LoadScript(const char* code) {
    if (!m_engine) {
        // 엔진 선택 및 생성
        if (isPythonScript(code)) {
            m_engine = new CPythonEngine();
        } else {
            m_engine = new CScriptEngine();
        }
        m_engine->Initialize();
        
        // 버퍼된 객체 일괄 등록
        for (auto& po : m_pendingObjects) {
            m_engine->AddObject(po.name, po.disp);
        }
        m_pendingObjects.RemoveAll();
    }
    m_engine->LoadScript(code);
}
```

### 왜 바이너리 마커를 제거해야 하는가?

**발견:**
- .map 파일의 스크립트 섹션: `\x0E [ScriptKind] \x0C [스크립트 내용]`
- VBS: 이 마커를 무시하고 스크립트만 파싱
- Python: SyntaxError 발생

**원인:**
- Windows Script Host의 AddCode: 마커를 자동으로 처리
- Python의 PyRun_String: 순수 텍스트만 파싱

**해결:**
```cpp
// pythonEngine.cpp
void CPythonEngine::LoadScript(const char* rawData) {
    // 바이너리 마커 제거
    CString cleaned;
    int len = strlen(rawData);
    for (int i = 0; i < len; i++) {
        if (rawData[i] == '\x0E') {
            // 마커 시작 → 끝(\x0C)까지 스킵
            while (i < len && rawData[i] != '\x0C') i++;
            i++; // \x0C 스킵
        } else {
            cleaned += rawData[i];
        }
    }
    
    // 정제된 스크립트 실행
    PyRun_String(cleaned, Py_file_input, m_globals, m_locals);
}
```

### 왜 재진입(Re-entrance) 방어가 필요한가?

**문제 상황:**
```
Python 스크립트 실행 중:
  result = Screen.MessageBox("오류가 발생했습니다")
           ↓
  IDispatch::Invoke (MessageBox)
           ↓
  CScreen::MessageBox (Windows API)
           ↓
  ::MessageBox() - 모달 다이얼로그 표시
           ↓
  사용자가 OK 클릭, 메시지 루프 실행
           ↓
  다른 이벤트 발생 (예: 타이머)
           ↓
  DoProcedure 호출 → Python 다시 실행!
           ↓
  Python GIL 재진입 → 크래시!
```

**해결책:**
```cpp
// pythonEngine.cpp
class CPythonEngine {
private:
    bool m_inException;  // 예외 처리 중 플래그
};

void CPythonEngine::DoProcedure() {
    if (m_inException) {
        // 재진입 방지 - 조용히 무시
        return;
    }
    
    m_inException = true;
    try {
        PyObject_Call(...);
    } catch (...) {
        // 예외 처리
    }
    m_inException = false;
}
```

---

## 3. 트러블슈팅 이력

### 버그 1: LoadLibrary error 126

**증상:**
- axisvbs.dll 로드 실패
- "코드 126: 지정된 모듈을 찾을 수 없습니다"

**원인:**
- python311.dll이 HTS exe 폴더에 없음
- axisvbs.dll이 로드될 때 python311.dll을 찾을 수 없음

**해결:**
```
C:\Users\IBKS\AppData\Local\Programs\Python\Python311-32\python311.dll
        ↓ 복사
HTS exe 폴더\python311.dll
```

**학습:**
- Windows DLL 검색 경로: exe 폴더 → system32 → 환경변수 PATH
- Python311-32는 PATH에 없으므로 수동 복사 필수

---

### 버그 2: C4996 Py_SetPythonHome deprecated

**증상:**
```
warning C4996: 'Py_SetPythonHome': deprecated ...
```

**원인:**
- Python 3.11에서 PyConfig API로 변경
- Py_SetPythonHome: Python 3.10 이전 API

**해결:**
```cpp
// Before (Python 3.10)
Py_SetPythonHome(L"C:\\Python311-32");

// After (Python 3.11+)
PyConfig config;
PyConfig_InitPythonConfig(&config);
config.home = PyUnicode_FromWideChar(L"C:\\Python311-32", -1);
Py_InitializeFromConfig(&config);
```

**학습:**
- Python API는 메이저 버전마다 변경 가능
- 공식 문서 `python --version` 확인 후 코드 작성

---

### 버그 3: 모든 화면에서 Python 동작

**증상:**
- VBS 스크립트가 있는 기존 화면도 Python으로 실행됨
- INFO 객체 오류 등으로 인해 동작 불가

**원인:**
- 초기 버전: 레지스트리 전역 설정 방식 사용
- "PYTHON 모드 ON" → 모든 화면에 적용

**해결:**
- 스크립트 내용 자동 감지 방식으로 변경
- `def ` 또는 `import ` 포함 시만 CPythonEngine 사용
- 그 외 → CScriptEngine 사용 (기존 VBS 유지)

**코드:**
```cpp
bool CPythonEngine::isPythonScript(const char* code) {
    CString str(code);
    return (str.Find("def ") >= 0 || str.Find("import ") >= 0);
}
```

**학습:**
- 점진적 마이그레이션: 전체 ON/OFF보다 자동 감지가 효율적
- 사용자 의도 존중: 수동 설정보다 자동 결정

---

### 버그 4: INFO 객체 오류 (800a01a8)

**증상:**
- `INFO is not defined` 오류
- 기존 VBS 화면에서 발생

**원인:**
```
순서 1: Initialize() → CEngineWrapper 생성
순서 2: AddObject("INFO", disp) → m_pendingObjects 저장
순서 3: LoadScript(vbs_code) → CScriptEngine 생성
순서 4: CScriptEngine::AddObject 호출되지 않음!
```

실제로는 CScriptEngine에 INFO를 등록하려 했으나, PendingObject 버퍼링 로직이 불완전

**해결:**
- PendingObject 버퍼링 로직 수정
- LoadScript 호출 시 엔진 타입 확인 후 일괄 등록

**코드:**
```cpp
void CEngineWrapper::LoadScript(const char* code) {
    if (!m_engine) {
        // 엔진 선택
        if (isPythonScript(code)) {
            m_engine = new CPythonEngine();
        } else {
            m_engine = new CScriptEngine();
        }
        m_engine->Initialize();
        
        // 중요: 엔진 타입에 관계없이 모든 버퍼된 객체 등록
        for (auto& po : m_pendingObjects) {
            m_engine->AddObject(po.name, po.disp);  // ← 이 줄 필수
        }
        m_pendingObjects.RemoveAll();
    }
    m_engine->LoadScript(code);
}
```

---

### 버그 5: 프로그램 크래시 (PyDict_GetItem)

**증상:**
- MessageBox 표시 후 프로그램 크래시
- 스택 추적: PyDict_GetItem → IsAvailable 체크

**원인:**
```
1. Python 스크립트 실행 중 MessageBox 호출
2. MessageBox 후 다른 이벤트 발생 (재진입)
3. CPythonEngine::IsAvailable() 검사 중 PyDict_GetItem 호출
4. 파이썬 인터프리터 상태 불일치 → 크래시
```

**해결:**
1. 재진입 방어 플래그 추가
2. 소멸자에서 Python 정리 추가
3. GetIDispatch(TRUE) 추가 참조

```cpp
class CPythonEngine {
private:
    bool m_inException;
    
    ~CPythonEngine() {
        if (m_globals) PyDecRef(m_globals);
        if (m_locals) PyDecRef(m_locals);
        Py_FinalizeEx();  // ← 중요
    }
};

void CPythonEngine::DoProcedure() {
    if (m_inException) return;  // 재진입 방지
    m_inException = true;
    // ... 실행
    m_inException = false;
}

void CPythonEngine::IsAvailable() {
    if (m_inException) return false;  // 재진입 시 false 반환
    // ... 정상 체크
}
```

---

### 버그 6: SYSTEM is not defined

**증상:**
- Python 스크립트에서 `SYSTEM.MessageBox(...)` 호출 시 NameError 발생
- VBS에서는 정상 동작

**원인:**
- VBS: 대소문자 무시 (SYSTEM = System = system)
- Python: 대소문자 구분 (SYSTEM ≠ System)

**해결:**
```python
# 잘못된 Python 코드 (VBS 호환)
SYSTEM.MessageBox("msg")  # NameError

# 올바른 Python 코드
System.MessageBox("msg")  # OK
```

**학습:**
- VBS → Python 변환 시 대소문자 정규화 필수
- AddObject 등록명: "System" (대문자 S, 소문자 ystem)
- Python에서 정확히 `System` 사용

---

## 4. 중요한 설계 결정

### 1. COM 객체 래핑 방식

**선택:** AxisObject(Python 타입) 방식

**장점:**
- Python 스크립트에서 자연스러운 문법 사용 (obj.method())
- IDispatch::Invoke 오버헤드 최소화
- 속성/메서드 동적 바인딩

**단점:**
- CPython 내부 구조 의존성 (tp_getattro 등)
- 순환참조 관리 필요 (PyGC_Collect)

### 2. 엔진 생성 시점

**선택:** LoadScript 지연 생성

**대안:**
- Initialize에서 미리 생성
- 첫 호출 시 생성

**선택한 이유:**
- Initialize: 아직 스크립트가 없음 (Python/VBS 판단 불가)
- LoadScript: 스크립트 로드 시점, 정확히 판단 가능

### 3. 바이너리 마커 처리 위치

**선택:** LoadScript 호출 시

**대안:**
- mapload.cpp (Builder)에서 미리 제거
- Screen.cpp에서 제거

**선택한 이유:**
- axisvbs.dll이 책임져야 할 일 (내부 구현)
- Builder/Screen은 모르고 호출만 하면 됨
- 독립성 유지

---

## 5. Python 환경 설정

### sys.path 구성

```python
>>> import sys
>>> sys.path
[
    'C:\\Users\\IBKS\\AppData\\Local\\Programs\\Python\\Python311-32\\lib',
    'C:\\Users\\IBKS\\AppData\\Local\\Programs\\Python\\Python311-32\\libs',
    'D:\\src\\IBKS\\map_src\\IB',  # ← pythonEngine.cpp에서 추가
    ...
]
```

**목적:**
- 사용자 정의 .py 모듈을 맵소스 경로에서 로드 가능

```cpp
// pythonEngine.cpp
PyConfig_AddPath(config, L"D:\\src\\IBKS\\map_src\\IB");
```

### 표준 라이브러리 사용

```python
# Python 스크립트에서 표준 라이브러리 사용 가능
import os
import sys
import json
import re
import datetime
```

---

## 6. COM 객체 명칭 규약

### 대소문자 규칙

| 객체명 | 등록 (AddObject) | Python 코드 | 설명 |
|--------|-----------------|-----------|------|
| Screen | "Screen" | `Screen.SetData()` | 현재 화면 |
| System | "System" | `System.MessageBox()` | 시스템 함수 |
| Login | "Login" | `Login.GetUserID()` | 로그인 정보 |
| Info | "Info" | `Info.GetValue()` | INI 파일 접근 |
| 컨트롤 | 맵소스 정의명 | `usid.GetValue()` | 스크린 컨트롤 |

**주의:** "SYSTEM"이 아니라 "System" (파스칼 케이싱)

### VBS → Python 변환 매핑

```vbscript
' VBS (대소문자 무시)
SCREEN.SETDATA "field", "value"
SYSTEM.MESSAGEBOX "msg"
```

```python
# Python (대소문자 구분)
Screen.SetData("field", "value")
System.MessageBox("msg")
```

---

## 7. 성능 최적화 팁

### 1. LoadScript 캐싱

현재: LoadScript 호출마다 PyRun_String 실행

최적화:
```cpp
void CPythonEngine::LoadScript(const char* code) {
    static std::unordered_map<std::string, PyCodeObject*> cache;
    
    // 코드 해시 계산
    std::string hash = ComputeHash(code);
    
    if (cache.find(hash) != cache.end()) {
        // 캐시된 코드 사용
        PyObject_CallObject(cache[hash], ...);
    } else {
        // 새 코드 컴파일
        PyCodeObject* compiled = Py_CompileString(code, "...", Py_file_input);
        cache[hash] = compiled;
        PyEval_EvalCode(compiled, ...);
    }
}
```

### 2. GC 최적화

현재: 매 DoProcedure마다 PyGC_Collect()

최적화:
```cpp
void CPythonEngine::DoProcedure() {
    // ...
    
    // 일정 횟수마다만 GC 실행
    m_callCount++;
    if (m_callCount % 100 == 0) {
        PyGC_Collect();
    }
}
```

### 3. 대량 AddObject 최적화

현재: 개별 호출

```python
for obj in large_list:
    Screen.AddObject(obj)  # 1000번 호출
```

최적화: 배치 처리
```cpp
void CPythonEngine::AddObjects(const ObjectArray& objs) {
    // 한 번에 등록
    for (auto& obj : objs) {
        AddObject(obj.name, obj.disp);
    }
    PyGC_Collect();  // 한 번만 실행
}
```

---

## 8. 알려진 제한사항

### 1. 스레드 안전성

**현재:** 단일 메인 스레드만 지원

**이유:** Python GIL(Global Interpreter Lock)

**해결:** Worker 스레드에서 호출 시
```cpp
PyGILState_STATE state = PyGILState_Ensure();
try {
    // Python 호출
} finally {
    PyGILState_Release(state);
}
```

### 2. 상속/클래스 정의 불가

**제한:** Python 스크립트에서 클래스 정의 불가

```python
# 불가능
class MyClass(Screen):
    def Method(self):
        pass
```

**이유:** AxisObject가 동적 라핑, 상속 체계가 없음

**해결:** 함수 정의로 대체
```python
# 가능
def MyMethod():
    Screen.SetData("field", "value")
```

### 3. 모듈 import 제한

**현재:** 표준 라이브러리만 가능

```python
import os, sys, json  # OK
import numpy          # 설치 필요
```

**해결:** pip install로 패키지 추가
```bash
D:\Python311-32\Scripts\pip.exe install numpy
```

---

## 9. 배포 체크리스트

### DLL 배포

```
exe/
  ├── python311.dll          ← 필수 (누락 시 error 126)
  ├── vcruntime140.dll       ← 필수 (누락 시 초기화 실패)
  ├── msvcp140.dll           ← 선택 (stdlib 사용 시 필요)
  ├── axisvbs.dll            ← 신규
  ├── axwizard.dll           ← 수정
  └── [기타 기존 DLL]
```

### 설정 확인

```
Python 경로 (axisvbs.vcxproj):
  IncludeDirectories:
    C:\Users\IBKS\AppData\Local\Programs\Python\Python311-32\include

  AdditionalLibraryDirectories:
    C:\Users\IBKS\AppData\Local\Programs\Python\Python311-32\libs

  AdditionalDependencies:
    python311.lib
```

---

## 10. 다음 단계 (미완료)

| 작업 | 담당 | 상태 |
|------|------|------|
| Screen.cpp TEST 블록 제거 | Developer | 단계 11 |
| VBScript → Python 일괄 변환 도구 | Developer | 단계 12 |
| 실운영 테스트 (5,391개 맵) | QA | 미착수 |
| Python 모듈 라이브러리 구축 | Developer | 미착수 |

---

## 11. 실시간 데이터(RTM) 갱신 흐름과 Input 필드 버그 (2026-07-16)

### 배경

Wizard가 실시간 시세를 수신해서 화면의 Edit 컨트롤(`dll/form/fmEdit.cpp`)에 데이터를 쓰기까지의 전체 흐름을 추적하고, 그 과정에서 "입력전용(Input) 필드도 실시간 데이터로 덮어써지는" 문제를 발견/수정함.

### 전체 흐름

```
소켓 수신 (실시간 tick, code=종목코드)
  → CGuard::OnAlert (Wizard/Guard.cpp:1024)
    → CGuard::DoRTM (Wizard/Guard.cpp:5898)
      → CClient::OnAlert (Wizard/Client.cpp:1496)   [열려있는 각 화면에 전파]
        → CScreen::OnAlert (Wizard/Screen.cpp:755)
            ★ text.Compare(code) — 화면의 종목코드 필드 == 실시간 데이터 심볼 비교 (line 793)
          → CScreen::UpdateRTM (Wizard/Screen.cpp:806)
            → fms->Lookup(필드명) 으로 값 조회
            → CfmEdit::WriteData (dll/form/fmEdit.cpp:1261)
              → invalidateRect → WM_PAINT → CfmEdit::Draw (dll/form/fmEdit.cpp:603)
```

- 화면 로드 시 `FA_FLASH` 속성(맵소스 "실시간" 체크)이 붙은 필드는 `m_flashObs`에 등록됨 (Screen.cpp:378-384). 이 중 "종목코드" 역할 필드가 `CScreen::OnAlert`의 `default:` 분기에서 실시간 데이터의 심볼과 비교됨.
- 일치하면 `UpdateRTM`이 그 다음 필드(`key+1`)부터 순회하며 각 필드명을 실시간 데이터셋(`fms`)에서 조회해 `WriteData` 호출.

### 발견한 문제

`CScreen::UpdateRTM`(Screen.cpp:833-835, FM_EDIT/FM_OUT 케이스)에는 필드의 `iok`(EIO_INPUT/EIO_INOUT/EIO_OUTPUT) 값을 확인하는 코드가 없었음. 반면 TR(화면조회) 응답 처리 경로인 `CStream::SetDataOOP`(Wizard/Stream.cpp:1895-1903)는 `iok`가 `EIO_OUTPUT`/`EIO_INOUT`일 때만 처리하고 `EIO_INPUT`은 명시적으로 skip함 — 실시간 경로와 TR 경로의 동작이 서로 달랐음.

**원인:** 빌더 쪽 `CChildView::isValidIOOrder()`(builder/ChildView.cpp:1704-1717)가 `fmEDIT`에 대해서는 `ioType`(input/output/both)과 무관하게 항상 `true`를 반환 → Input 전용 필드도 `iorder`를 부여받아 `m_ioR` 배열에 포함됨. 이후 런타임(`UpdateRTM`)에서 이를 걸러주는 필터가 없어서, 필드명이 우연히(혹은 의도적으로) 실시간 데이터셋의 필드명과 일치하면 사용자가 입력 중인 Input 필드 값도 tick 수신마다 덮어써질 수 있었음.

### 수정 내용

`Wizard/Screen.cpp` `CScreen::UpdateRTM` 함수, FM_EDIT/FM_OUT 케이스에 조건 추가:

```cpp
case FM_EDIT:
case FM_OUT:
    if (form->m_form->iok == EIO_INPUT)   // 입력전용은 실시간 갱신 제외
        continue;
    break;
```

`EIO_INOUT`(양방향)은 그대로 실시간 갱신 대상 유지 — "입력도 받고 실시간 시세도 반영"되는 설계는 의도된 동작이므로 배제하지 않음. 순수 `EIO_INPUT`만 제외.

**검증:** 2026-07-16 적용 후 정상 동작 확인.

**주의(미검증 리스크):** 기존 맵소스 중 특정 화면이 이 동작(Input 필드가 실시간으로 자동 프리필되는 것)에 의도적으로 의존하고 있었을 가능성은 전수조사되지 않음. 이상 동작 리포트 발생 시 이 변경을 1차 의심 지점으로 확인할 것.

### 관련 파일

| 파일 | 역할 |
|------|------|
| `Wizard/Guard.cpp` | 실시간 데이터 수신/파싱(`OnAlert`), 클라이언트 브로드캐스트(`DoRTM`) |
| `Wizard/Client.cpp` | 화면 단위 전파(`OnAlert`) |
| `Wizard/Screen.cpp` | 심볼 매칭(`OnAlert`), 필드 갱신(`UpdateRTM`) — 이번 수정 위치 |
| `dll/form/fmEdit.cpp` | 실제 데이터 반영/그리기(`WriteData`, `Draw`) |
| `builder/ChildView.cpp` | 빌더에서 `iorder` 부여 로직(`isValidIOOrder`) |

---

## 12. 추가 자료

### 참고 문서
- `@docs/Architecture.md` - 모듈 구조
- `@docs/Dependency.md` - 라이브러리 의존성
- `@docs/python_engine_260608.md` - 프로젝트 상세 기록
- `@docs/CallGraph.md` - 함수 호출 흐름

### 외부 참고
- [Python C API 문서](https://docs.python.org/3.11/c-api/)
- [COM 및 Automation](https://docs.microsoft.com/en-us/windows/win32/com/com-technical-reference)
- [MFC 문서](https://docs.microsoft.com/en-us/cpp/mfc/mfc-desktop-applications)

---

**최종 수정:** 2026-07-16
**기여자:** Documentation Agent
