---
project: ibks
category: knowledge-base
status: 완료
updated: 2026-08-28
---

# ibks 프로젝트 지식 베이스


## 목차

- [문서 목적](#문서-목적)
- [1. 설계 의도](#1-설계-의도)
  - [VBScript → Python 엔진 전환의 이유](#vbscript-python-엔진-전환의-이유)
  - [CEngineWrapper의 역할](#cenginewrapper의-역할)
- [2. 아키텍처 설계 상세](#2-아키텍처-설계-상세)
  - [왜 AddObject 버퍼링을 했는가?](#왜-addobject-버퍼링을-했는가)
  - [왜 바이너리 마커를 제거해야 하는가?](#왜-바이너리-마커를-제거해야-하는가)
  - [왜 재진입(Re-entrance) 방어가 필요한가?](#왜-재진입re-entrance-방어가-필요한가)
- [3. 트러블슈팅 이력](#3-트러블슈팅-이력)
  - [버그 1: LoadLibrary error 126](#버그-1-loadlibrary-error-126)
  - [버그 2: C4996 Py_SetPythonHome deprecated](#버그-2-c4996-py_setpythonhome-deprecated)
  - [버그 3: 모든 화면에서 Python 동작](#버그-3-모든-화면에서-python-동작)
  - [버그 4: INFO 객체 오류 (800a01a8)](#버그-4-info-객체-오류-800a01a8)
  - [버그 5: 프로그램 크래시 (PyDict_GetItem)](#버그-5-프로그램-크래시-pydict_getitem)
  - [버그 6: SYSTEM is not defined](#버그-6-system-is-not-defined)
- [4. 중요한 설계 결정](#4-중요한-설계-결정)
  - [1. COM 객체 래핑 방식](#1-com-객체-래핑-방식)
  - [2. 엔진 생성 시점](#2-엔진-생성-시점)
  - [3. 바이너리 마커 처리 위치](#3-바이너리-마커-처리-위치)
- [5. Python 환경 설정](#5-python-환경-설정)
  - [sys.path 구성](#syspath-구성)
  - [표준 라이브러리 사용](#표준-라이브러리-사용)
- [6. COM 객체 명칭 규약](#6-com-객체-명칭-규약)
  - [대소문자 규칙](#대소문자-규칙)
  - [VBS → Python 변환 매핑](#vbs-python-변환-매핑)
- [7. 성능 최적화 팁](#7-성능-최적화-팁)
  - [1. LoadScript 캐싱](#1-loadscript-캐싱)
  - [2. GC 최적화](#2-gc-최적화)
  - [3. 대량 AddObject 최적화](#3-대량-addobject-최적화)
- [8. 알려진 제한사항](#8-알려진-제한사항)
  - [1. 스레드 안전성](#1-스레드-안전성)
  - [2. 상속/클래스 정의 불가](#2-상속클래스-정의-불가)
  - [3. 모듈 import 제한](#3-모듈-import-제한)
- [9. 배포 체크리스트](#9-배포-체크리스트)
  - [DLL 배포](#dll-배포)
  - [설정 확인](#설정-확인)
- [10. 다음 단계 (미완료)](#10-다음-단계-미완료)
- [11. 실시간 데이터(RTM) 갱신 흐름과 Input 필드 버그 (2026-07-16)](#11-실시간-데이터rtm-갱신-흐름과-input-필드-버그-2026-07-16)
  - [배경](#배경)
  - [전체 흐름](#전체-흐름)
  - [발견한 문제](#발견한-문제)
  - [수정 내용](#수정-내용)
  - [관련 파일](#관련-파일)
- [12. axwizard 소스 분석 중 발견한 문서-코드 드리프트 (2026-07-13)](#12-axwizard-소스-분석-중-발견한-문서-코드-드리프트-2026-07-13)
  - [12.1 엔진 선택 방식이 "순수 자동감지"에서 "빌드 시 확정 + 자동감지 폴백"으로 진화](#121-엔진-선택-방식이-순수-자동감지에서-빌드-시-확정-자동감지-폴백으로-진화)
  - [12.2 텍스트 스캔 폴백(`isPythonScript()`) 자체도 더 엄격해짐](#122-텍스트-스캔-폴백ispythonscript-자체도-더-엄격해짐)
  - [12.3 재진입(reentrancy) 방어 코드(`m_inException`)가 현재 코드에 없음](#123-재진입reentrancy-방어-코드m_inexception가-현재-코드에-없음)
  - [12.4 PendingObject 버퍼링 메커니즘은 그대로 유지됨 (기존 설계 확인됨)](#124-pendingobject-버퍼링-메커니즘은-그대로-유지됨-기존-설계-확인됨)
- [13. Ctrl+C/V 클립보드 단축키가 외부에서 차단됨 — Ctrl+1/2/3 폴백으로 우회 (2026-07-29)](#13-ctrlcv-클립보드-단축키가-외부에서-차단됨-ctrl123-폴백으로-우회-2026-07-29)
  - [증상](#증상)
  - [조사 경과 (axlog로 진단, `docs/DebugLogGuide.md` 4절 클립보드 로그 활용)](#조사-경과-axlog로-진단-docsdebuglogguidemd-4절-클립보드-로그-활용)
  - [결론](#결론)
  - [적용한 우회책 — `Wizard/Key.cpp::CKey::OnKey`](#적용한-우회책-wizardkeycppckeyonkey)
- [14. COnTimer는 죽은 코드 — 실제 타이머는 전부 평범한 WM_TIMER (2026-07-29)](#14-contimer는-죽은-코드-실제-타이머는-전부-평범한-wm_timer-2026-07-29)
  - [배경](#배경-1)
  - [확인된 사실](#확인된-사실)
  - [설계 의도로 추정되는 것](#설계-의도로-추정되는-것)
  - [되돌려진 이유로 추정되는 것 (증거 기반 추정, 미확정)](#되돌려진-이유로-추정되는-것-증거-기반-추정-미확정)
  - [교훈](#교훈)
  - [관련 파일](#관련-파일-1)
- [15. Screen.Send() 중복/연쇄 억제 메커니즘 (2026-07-29, 실측 트레이스 기반)](#15-screensend-중복연쇄-억제-메커니즘-2026-07-29-실측-트레이스-기반)
  - [배경](#배경-2)
  - [확인된 사실 — Send/OnChange 무한연쇄를 막는 5중 안전장치](#확인된-사실-sendonchange-무한연쇄를-막는-5중-안전장치)
  - [실측으로 확정됨 (2026-07-29, `[OnTRAN-gate]`/`[MakeStream-waitSN-drop]` 로그 추가 후 재현)](#실측으로-확정됨-2026-07-29-ontran-gatemakestream-waitsn-drop-로그-추가-후-재현)
  - [부수 발견 — OnStart 실행 전에 이미 Send가 나갈 수 있음](#부수-발견-onstart-실행-전에-이미-send가-나갈-수-있음)
  - [관련 파일](#관련-파일-2)
- [16. FlashGrid RTM 로그의 포맷스트링 타입 불일치로 인한 크래시 (2026-08-20)](#16-flashgrid-rtm-로그의-포맷스트링-타입-불일치로-인한-크래시-2026-08-20)
  - [증상](#증상-1)
  - [원인](#원인-1)
  - [수정](#수정)
  - [교훈](#교훈-1)
  - [관련 파일](#관련-파일-3)
- [17. 화면 대기(모래시계) 커서 메커니즘 — Wizard는 커서를 직접 그리지 않는다 (2026-08-26)](#17-화면-대기모래시계-커서-메커니즘-wizard는-커서를-직접-그리지-않는다-2026-08-26)
  - [배경](#배경-3)
  - [전체 체인 — Wizard(스크립트/네트워크) → 호스트 EXE(실제 SetCursor)](#전체-체인-wizard스크립트네트워크-호스트-exe실제-setcursor)
  - [Wizard가 "대기 상태"로 들어가는 트리거 5가지](#wizard가-대기-상태로-들어가는-트리거-5가지)
  - [종합화면에서 부분(창)마다 커서가 다르게 바뀌는 이유](#종합화면에서-부분창마다-커서가-다르게-바뀌는-이유)
  - [발견 1 — TranTimeout 워치독은 Send()에만 있고 Service()/Wait 속성에는 없음](#발견-1-trantimeout-워치독은-send에만-있고-servicewait-속성에는-없음)
  - [발견 2 — 실사용 사례: IB000157/IB000130의 Screen.Service (워치독 없는 패턴)](#발견-2-실사용-사례-ib000157ib000130의-screenservice-워치독-없는-패턴)
  - [발견 3 — 죽은 코드: ChildFrm/SChild의 axWAIT 처리 (로직 반전 + break 누락, 호출자 없음)](#발견-3-죽은-코드-childfrmschild의-axwait-처리-로직-반전-break-누락-호출자-없음)
  - [관련 파일](#관련-파일-4)
- [18. 추가 자료](#18-추가-자료)
  - [참고 문서](#참고-문서)
  - [외부 참고](#외부-참고)

---

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

**2026-07-27 갱신 — 수정 잠정 보류(주석 처리로 원복):** 위 미검증 리스크가 실제로 마음에 걸려서, 이 `iok == EIO_INPUT` continue 처리를 코드에서 다시 주석 처리해 원래 동작(입력전용 필드도 실시간 갱신 대상)으로 되돌려놓은 상태다. 이게 "버그"인지 "특정 화면이 의도적으로 기대는 동작"인지 아직 확신이 없어서 — 확실한 근거 없이 성급하게 되돌리지 않겠다는 판단. 대신 `Wizard/Screen.cpp`의 `UpdateRTM`(FM_EDIT/FM_OUT 분기)에 진단 로그(`axlog(LOG_RTM, "[UpdateRTM-write] name=... iok=... isInput=...")`, `docs/DebugLogGuide.md` 참고)를 추가해서, 장중에 실제로 `isInput=1`인 필드가 얼마나/어떤 화면에서 실시간 갱신되는지 먼저 관찰하기로 함. **현재 코드 상태는 수정 적용 전(버그 재현 가능)이다** — 이 절 앞부분의 "수정 내용"/"검증" 문구는 2026-07-16 당시 상황 기록이며, 현재 코드와 다르다는 점에 주의. 로그로 실사용 화면 중 영향받는 사례가 있는지 확인한 뒤 최종 방향(재적용 여부, 혹은 화면별 예외 처리)을 정할 예정.

### 관련 파일

| 파일 | 역할 |
|------|------|
| `Wizard/Guard.cpp` | 실시간 데이터 수신/파싱(`OnAlert`), 클라이언트 브로드캐스트(`DoRTM`) |
| `Wizard/Client.cpp` | 화면 단위 전파(`OnAlert`) |
| `Wizard/Screen.cpp` | 심볼 매칭(`OnAlert`), 필드 갱신(`UpdateRTM`) — 이번 수정 위치 |
| `dll/form/fmEdit.cpp` | 실제 데이터 반영/그리기(`WriteData`, `Draw`) |
| `builder/ChildView.cpp` | 빌더에서 `iorder` 부여 로직(`isValidIOOrder`) |

---

## 12. axwizard 소스 분석 중 발견한 문서-코드 드리프트 (2026-07-13)

`ibks/Wizard/` 전체 아키텍처 분석 프로젝트(`@docs/WizardArchitecture.md`) 진행 중, 초기 설계 문서(`@docs/python_engine_260608.md`, 2026-06-08~11 작성)와 **현재 `dll/vbs/engineWrapper.cpp` / `pythonEngine.h` 실제 코드가 달라진 부분**을 발견함. 아래는 그 상세 내용 (기존 기록은 삭제하지 않고 이 절로 보강).

### 12.1 엔진 선택 방식이 "순수 자동감지"에서 "빌드 시 확정 + 자동감지 폴백"으로 진화

기존 문서(1절 "엔진 선택 방식", 3절 버그#3): `isPythonScript()`가 스크립트 텍스트 어디든 `"def "`/`"import "`가 있으면(Find() >= 0) Python으로 판단한다고 기록됨.

**현재 코드 (`Wizard/Screen.cpp` `CScreen::Parse()`):**
```cpp
m_vbe->LoadScript(text, m_mapH->pythonMode);   // 빌드 시 확정된 pythonMode를 명시적으로 전달
```
`m_mapH->pythonMode`는 `builder/awBuild/mapload.cpp`가 **빌드 시점**에 자동감지해 맵 바이너리에 기록한 값(python_engine_260608.md 9단계 작업, 완료됨). 즉 런타임 자동감지는 이제 "1차 판단"이 아니라 이 값이 없을 때(-1)의 **폴백**이다.

`dll/vbs/engineWrapper.cpp::LoadScript(scripts, scpKind)`:
```cpp
bool usePython = (scpKind == -1) ? isPythonScript(scripts) : (scpKind != 0);
```

### 12.2 텍스트 스캔 폴백(`isPythonScript()`) 자체도 더 엄격해짐

기존: `scripts.Find("def ") >= 0` — 텍스트 어디에 있든 매치 (문자열/주석 안의 "def "도 오탐 가능).

현재: 줄 단위로 순회하며 `line.TrimLeft()` 후 **줄의 맨 앞이 `"def "` 또는 `"import "`로 시작**하는지만 검사. 오탐 가능성을 줄이는 방향의 개선.

### 12.3 재진입(reentrancy) 방어 코드(`m_inException`)가 현재 코드에 없음

기존 문서(2절, 3절 버그#5): `MessageBox` 재진입으로 인한 `PyDict_GetItem` 크래시를 `m_inException` 플래그로 방어했다고 기록됨.

**현재 `dll/vbs/pythonEngine.h`에는 `m_inException` 필드 자체가 없음.** `engineWrapper.cpp::IsAvailable()`의 주석:
> "Nested calls (e.g. Screen.Send(0) inside OnClick synchronously reaching OnSend) are safe in CPython on the same thread, so no re-entrancy guard is needed here - matches VBScript's behavior below."

의도적으로 제거된 것으로 보이나, **실제 재진입 크래시 시나리오(MessageBox 모달 → 메시지 루프 재진입 → DoProcedure 재호출)로 재검증되지는 않은 상태**. 과거 크래시 심각도(버그#5)를 고려하면 실운영 테스트(`python_engine_260608.md` 10단계) 시 최우선 회귀 확인 대상.

### 12.4 PendingObject 버퍼링 메커니즘은 그대로 유지됨 (기존 설계 확인됨)

`CScreen::Parse()`는 `m_vbe->Initialize()` 직후, 아직 VBS/Python이 결정되기 전에 `AddObject("Screen", ...)`, `AddObject("System", ...)` 등을 여러 차례 호출한다. `CEngineWrapper`는 이를 `std::vector<PendingObject>`에 버퍼링해뒀다가 `LoadScript()`에서 엔진이 확정되는 순간(`ensureEngine()`) 일괄 등록한다 — 기존 문서 2절 "AddObject 버퍼링" 설계와 동일한 메커니즘이 현재도 유지되고 있음을 코드로 확인.

**교훈:** 설계 문서는 작성 시점의 스냅샷이며, 이후 리팩토링으로 세부 구현이 계속 진화할 수 있다. 특히 크래시 방어 코드처럼 "왜 있는지" 불분명해 보이는 코드가 문서에는 있는데 실제 코드에는 없을 경우, 삭제된 것이 의도적 개선인지 실수인지 코드만으로는 단정할 수 없으므로 실동작 테스트로 확인 필요.

---

## 13. Ctrl+C/V 클립보드 단축키가 외부에서 차단됨 — Ctrl+1/2/3 폴백으로 우회 (2026-07-29)

### 증상

HTS 화면의 입력필드(`CfmEdit`)에서 **Ctrl+C(복사)/Ctrl+V(붙여넣기)가 전혀 동작하지 않음**. 사용자가 직접 발견해서 보고.

### 조사 경과 (axlog로 진단, `docs/DebugLogGuide.md` 4절 클립보드 로그 활용)

1. **일반 문자 키(예: 그냥 C, 또는 Shift+C)는 완벽히 정상 동작** — `CKey::OnKey` → `CKey::OnChar` 파이프라인 자체는 문제 없음이 로그로 확인됨.
2. **Ctrl 키를 누르고 있는 동안은 `CKey::OnKey`에 wParam=17(VK_CONTROL) 로그가 계속 정상적으로 찍힘.**
3. **그 상태에서 C(또는 V)를 누르는 순간, 그 키의 WM_KEYDOWN 자체가 `CKey::OnKey`에 전혀 도달하지 않음.** (로그가 안 찍히는 게 아니라, 함수 진입 자체가 없음 — Wizard 코드가 받아서 무시하는 게 아니라 메시지 자체가 안 옴)
4. `AXIS/axis.rc`의 `IDR_MAINFRAME ACCELERATORS` 테이블 확인 → Ctrl+C/V 항목 없음.
5. `AXIS/MainFrm.cpp`의 `SetWindowsHookEx(WH_GETMESSAGE, KeyboardProc, ...)` 훅과 `RegisterHotKey` 사용처 확인 → C/V 관련 처리 없음.
6. AXIS에 내장된 안랩 보안SDK(ASTx/STSDKEX, `MainFrm.cpp`의 `initASTx()`)를 사용자가 직접 `return false`로 강제 비활성화 후 재현 → **증상 동일** (이미 코드상 `pcAOS = false; //test astx`로 호출 자체가 스킵되고 있었음에도 동일).
7. 작업관리자에서 `GnAhnLab.exe` 프로세스 종료 후 재현 → **증상 동일** (단, AXIS가 이미 떠있는 상태에서 죽인 거라 완전히 확정적인 테스트는 아님 — 후킹이 프로세스 시작 시점에 한 번 주입되는 방식이면 이미 늦음).
8. `tasklist /m`으로 AXIS.exe(32비트)의 로드된 DLL 목록 조회 시도 → `ntdll.dll`/`wow64*.dll` 외에는 아무것도 안 보임(정상 프로세스라면 다 보여야 함) — anti-tampering으로 프로세스가 보호되고 있는 것으로 추정.
9. `AppInit_DLLs` 레지스트리(`HKLM\SOFTWARE\...\Windows`, 32/64비트 뷰 둘 다) 확인 → 비어있고 `LoadAppInit_DLLs=0` — 이 경로의 전역 DLL 주입은 아님.

### 결론

**Wizard/AXIS 소스 문제가 아님.** OS 메시지 큐 단계에서 Ctrl+C/V 조합만 선택적으로 삼켜지는 것으로 보이며, 가장 유력한 원인은 시스템 전역 저수준 키보드 후크(WH_KEYBOARD_LL 방식) 기반의 보안/DLP 에이전트 — 안랩(`GnAhnLab.exe`/`GnAgent.exe`/`GnPlugin.exe`/`GnStart.exe`) 또는 SYSTEM 권한으로 떠있는 `I3GMainSvc.exe` 계열로 추정되나, 정확한 원인 프로세스는 확정하지 못함 (완전 확정하려면 AXIS를 완전히 종료 → 후보 프로세스/서비스 전부 정지 → AXIS 재시작 → 재현 여부 확인하는 절차가 필요하나, 이번엔 여기까지 진행하지 않고 실용적 우회로 마무리함).

### 적용한 우회책 — `Wizard/Key.cpp::CKey::OnKey`

기존 `Copy$Paste(WPARAM, CScreen*, CfmBase*)` 함수(코드 3=복사/24=잘라내기/22=붙여넣기)는 전혀 문제가 없으므로, 이걸 그대로 호출하는 대체 단축키를 추가:

| 단축키 | 동작 |
|---|---|
| `Ctrl+1` | 복사 |
| `Ctrl+2` | 붙여넣기 |
| `Ctrl+3` | 잘라내기 |

처음엔 `Ctrl+Insert`/`Shift+Insert`/`Shift+Delete`(Windows 레거시 클립보드 단축키, Ctrl+C/V/X와 겹치지 않는 VK 코드라 후킹을 피할 가능성이 높다고 판단해서 선택)로 구현했으나, 사용자의 키보드가 미니 키보드라 Insert 키가 없어서 **Ctrl+1/2/3으로 재변경**. `AXIS/MainFrm.cpp::PreTranslateMessage`의 기존 Ctrl+조합(`Ctrl+Shift+F`, `Ctrl+Shift+X` 등) 확인 결과 숫자키 1/2/3과 충돌 없음.

**교훈:** "우리 코드가 특정 키 이벤트를 처리 안 한다"와 "그 키 이벤트 자체가 우리 코드까지 도달하지 않는다"는 증상은 똑같아 보이지만 원인이 완전히 다르다. 후자는 axlog처럼 진입점(`OnKey`/`OnChar`) 맨 앞에 로그를 심어서 "함수가 호출되는지 자체"를 확인해야 구분 가능하다 — 이번 건은 `CKey::OnKey` 진입 로그가 Ctrl+C/V 조합에서만 아예 안 찍히는 것으로 외부 차단임을 확정했다.

---

## 14. COnTimer는 죽은 코드 — 실제 타이머는 전부 평범한 WM_TIMER (2026-07-29)

### 배경

`OnTimer.cpp`에 축된 axlog 커버리지를 넓히던 중(`docs/DebugLogGuide.md` 10절), 사용자가 `CClient::SetTimer(UINT id, UINT elapse)`(Client.cpp:4192)를 직접 보고 `COnTimer` 클래스 관련 코드가 통째로 주석 처리돼 있다는 걸 발견함.

### 확인된 사실

`COnTimer` 관련 코드가 **세 군데 모두** 주석 처리돼 있음:
- `Client.h:99` — `m_OnTimer` 멤버 선언 자체가 주석
- `Client.cpp` 생성자 — `m_OnTimer = NULL;` 주석
- `Client.cpp` 소멸자 — 정리 로직(`m_OnTimer->PostThreadMessage(WM_QUIT, ...)` 등) 통째로 주석
- `Client.cpp::SetTimer` — `AfxBeginThread(RUNTIME_CLASS(COnTimer), ...)` 생성부 주석, 대신 `m_view->KillTimer(id); m_view->SetTimer(id, elapse, NULL);`만 실행

`Wizard.vcxproj`엔 `OnTimer.cpp`/`.h`가 빌드 대상으로 포함돼 있어 **컴파일은 되지만, 실제로 인스턴스화하는 곳이 코드 전체에 단 한 군데도 없다.** 실제 타이머 흐름은 전부 평범한 MFC `SetTimer` → `WM_TIMER` → `Event.cpp`의 `CallProc`에서 `wParam`(`TM_RTM`/`TM_WAIT`/`TM_REPBN`/`TM_REPTR`/`TM_VB`/`TM_VBx`)으로 분기하는 방식.

git 로그로는 언제/왜 되돌렸는지 확인 불가 — 이 저장소는 `ffd9e2e4 wizard 260217`(2026-02-17) 커밋 하나로 전체가 한 번에 임포트되어, 그 이전 히스토리 자체가 없음.

### 설계 의도로 추정되는 것

`COnTimer`는 생산자-소비자 큐(`m_que`+`m_keys`(중복제거)+`m_section`(임계구역)+`m_event`) 위에서 별도 워커스레드(`Run()`, 표준 메시지펌프 안 씀 — 완전 커스텀 루프)가 `DoParse()`로 메인스레드 창에 **동기 `SendMessage(WM_USER+11, key)`**를 보내는 구조. `WM_TIMER`는 Windows 메시지 중 최하위 우선순위라 메시지큐가 비어있을 때만 합성되는데, 이 프로그램처럼 RTM으로 메시지큐가 계속 바쁜 트레이딩 화면에서는 `WM_TIMER`가 계속 밀릴 수 있음 — `COnTimer`는 이걸 우회해서 `SendMessage`로 즉시 전달하려 한 시도로 추정됨.

### 되돌려진 이유로 추정되는 것 (증거 기반 추정, 미확정)

소멸자의 주석 처리된 코드:
```cpp
m_OnTimer->PostThreadMessage(WM_QUIT, NULL, NULL);
// HANDLE hThread = m_OnTimer->m_hThread;
// if (WaitForSingleObject(hThread, 500) == WAIT_TIMEOUT)
//     TerminateThread(hThread, 0);
```
`Run()`은 메시지펌프를 전혀 안 도는 커스텀 루프인데 `PostThreadMessage(WM_QUIT)`로 종료시키려 했다 — **이 스레드는 애초에 WM_QUIT를 못 받는 구조**라 500ms 타임아웃 후 최후수단으로 위험한 `TerminateThread`까지 준비돼 있었음. 게다가 `DoParse`의 `SendMessage`가 동기 블로킹인데, 만약 UI 스레드가 종료 중 이 워커스레드의 `WaitForSingleObject`를 기다리는 동안(=메시지펌프 정지 상태) 워커스레드가 그 `SendMessage`의 응답을 기다리고 있었다면 교착상태(deadlock) 조건이 성립함. 종료 시점에 멈추거나 크래시나는 문제를 겪고 근본 수정 대신 통째로 되돌린 것으로 보임(생성자/소멸자/생성지점 세 군데를 일관되게 지운 정황).

### 교훈

**"클래스가 빌드에 포함되고 내부 로직이 그럴듯해 보인다"가 "실제로 쓰이고 있다"를 보장하지 않는다.** `axlog`를 추가하기 전에 실제 호출 경로(인스턴스화 지점)를 먼저 확인했어야 함 — 이번엔 로그를 다 넣고 나서야 죽은 코드라는 걸 알게 됨(`DebugLogGuide.md` 10절에 이 사실을 명시해둠). 그리고 나중에 플랫폼 마이그레이션 시 "스레드 간 동기 SendMessage + 부정확한 종료 핸드셰이크" 패턴은 조용히 죽지 않고 데드락/크래시로 이어지기 쉬우니 재도입 시 주의할 지점으로 기록.

### 관련 파일

| 파일 | 역할 |
|---|---|
| `Wizard/OnTimer.h/cpp` | `COnTimer` 정의 — 빌드는 되나 미사용 |
| `Wizard/Client.h:99`, `Client.cpp`(생성자/소멸자/`SetTimer`) | `m_OnTimer` 관련 코드가 전부 주석 처리된 실제 위치 |
| `Wizard/Event.cpp` | 실제 타이머 처리 경로 — `CallProc`의 `WM_TIMER` 분기 |
| `@docs/DebugLogGuide.md` 10절 | `COnTimer`에 추가한 axlog 태그 카탈로그(현재 미발화 상태 명시) |

---

## 15. Screen.Send() 중복/연쇄 억제 메커니즘 (2026-07-29, 실측 트레이스 기반)

### 배경

`IB1208` 화면을 열었을 때의 axlog 실측 트레이스(`docs/DebugLogGuide.md` 4/6절 태그 활용)에서, 서브맵(`IB120810`)의 `1301` 필드 `OnChange`(`AX_1301_OnChange_AX_`)가 연속 2번 호출됐는데도 실제 소켓 송신(`[0-MakeStream-send]`/`CGuard::Write`)은 **1번만** 일어난 것을 발견함. "OnChange가 여러 번 타도 그 안의 `Screen.Send(0)`이 매번 다 나가지는 않는 것 같다"는 관찰에서 출발한 조사.

### 확인된 사실 — Send/OnChange 무한연쇄를 막는 5중 안전장치

1. **`CfmBase::IsChanged()` — read-and-clear dirty bit** (`CScreen::OnChange`, `Screen.cpp:1807`): `if (form->IsChanged()) { ...스크립트 호출... }` 구조. `IsChanged()`를 호출하는 순간 내부 변경플래그가 리셋됨 — 즉 **필드값이 실제로 다시 바뀌지 않는 한, 같은 필드에 대해 `OnChange()`가 또 호출돼도 스크립트가 다시 실행되지 않는다.**
2. **`CScreen::OnTRAN()` — Send 직전 전체 필드 재검증 패스** (`Screen.cpp:1748`): `Screen.Send()`가 실제로 `CStream::InStream()`을 타면, `MakeStream()` 호출 전에 **화면의 모든 입력필드를 순회하며 `OnChange(idx, byKey)`를 한 번씩 다시 호출**한다(1791줄, `if (!OnChange(ii, byKey)) return false;`). "Send 누르기 직전에 아직 반영 안 된 변경사항을 마저 커밋"하는 의도로 보이며, 위 1번의 dirty-bit 덕분에 이미 처리된 필드는 여기서 다시 스크립트를 타지 않는다.
3. **`CStream::m_lock` — 비재진입 락** (`Stream.cpp:58`, `InStream(CScreen*, bool, CString)`): `if (m_lock || !screen->OnTRAN(byKey)) return false;` — `MakeStream()`~소켓 `Write()` 구간 동안 `m_lock=true`로 걸어두므로, 그 안에서 스크립트가 재귀적으로 또 `Screen.Send()`를 불러도 즉시 `false`로 튕겨나간다. `CStream`은 `CClient`당 1개(`m_stream`)라 같은 클라이언트의 어느 화면에서 걸어도 전부 공유되는 락이다.
4. **`CxScreen::_Send()` 자체 가드** (`xscreen.cpp:371`): `if (m_screen->m_client->m_vm->m_script) return; // ignore dup` — `CScript::m_script`(현재 실행 중인 스크립트 콜백이 `byKey`로 트리거된 것인지 나타내는 플래그, `OnChange`/`OnClick`에서 `m_script = byKey`로 세팅됨)가 설정된 상태에서 또 `_Send`가 불리면 무시.
5. **`screen->m_state & waitSN` — 응답 대기 중 재송신 차단** (`Stream.cpp:1361`, `CStream::MakeStream` 맨 첫 줄): `if (screen->m_state & waitSN) return;`. `InStream()`이 송신에 성공하면 그 직후 `CClient::WaitState(screen, true)`가 `screen->m_state |= waitSN`을 세팅하고(`Client.cpp:1931`), 그 TR의 응답(`OutStream`)이 도착해야 `Stream.cpp:185` 등에서 `waitSN`이 다시 꺼진다. 이 사이에 `Screen.Send()`가 또 호출되면 `OnTRAN()` 검증까지는 정상 통과해도(3번 락과 달리 이건 `OnTRAN` **밖에서** 체크됨) `MakeStream()` 맨 앞에서 아무 로그도 없이(패킷 조립·`OnSend` 관련 코드에 도달하기 전) 조용히 리턴된다.

### 실측으로 확정됨 (2026-07-29, `[OnTRAN-gate]`/`[MakeStream-waitSN-drop]` 로그 추가 후 재현)

위 5개 중 실제 트레이스(서브맵 `IB120810`의 `1301` 필드가 초기화 중 연속 2번 `OnChange`된 케이스)에서 두 번째 시도를 막은 것은 **5번(`waitSN`)이었다** — `Screen.cpp:1802`에 추가한 `[OnTRAN-gate]` 로그가 두 번째 호출에서도 `onSendResult=1 m_return=1`(즉 `OnTRAN()` 자체는 정상적으로 `true`를 반환)로 찍혔는데도 그 뒤 `[0-MakeStream-send]` 배너가 안 나온 것으로 확정. 1~4번(dirty-bit/필드재검증/`m_lock`/`m_script` 가드)은 전부 `OnTRAN()` **내부**에서 걸리는 것들이라 `OnTRAN()`이 `true`를 반환한 이 케이스에서는 관여하지 않았다는 뜻. 실제 흐름:

```
OnChange(1301) #1 → Screen.Send(0) → InStream() → OnTRAN()=true → MakeStream() 실제 전송 → WaitState(screen,true) → screen->m_state |= waitSN
(그 사이 원인 미상의 두 번째 WriteData로 1301이 다시 dirty)
OnChange(1301) #2 → Screen.Send(0) → InStream() → OnTRAN()=true (1~4번 가드 전부 통과, IsChanged가 이 시점엔 다시 true였던 것으로 보임)
                  → MakeStream() 진입하자마자 `screen->m_state & waitSN`이 아직 true(첫 TR 응답이 아직 안 옴) → 조용히 return
```

**교훈:** `m_lock`(3번)은 "지금 이 순간 소켓에 쓰는 중"만 막는 아주 짧은 재진입 락이라 두 호출처럼 간격이 있는 케이스는 못 막는다. 실질적으로 "같은 화면이 이전 TR 응답을 아직 못 받았으면 새 Send를 버린다"는 더 넓은 범위의 안전장치는 `waitSN`이 담당한다. 원인이 된 "왜 1301이 두 번 dirty가 됐는지"(어느 코드가 두 번째 `WriteData`를 했는지)는 아직 미조사 — 서브맵 attach 초기화 경로(`CScreen::Parse`의 `FA_FLASH` 등록 블록 등) 쪽을 다음에 확인해볼 만하다.

### 부수 발견 — OnStart 실행 전에 이미 Send가 나갈 수 있음

`CClient::Attach`의 타이밍 로그 순서(`screen->Parse` 완료 → 이번 자동 OnChange/Send 연쇄 → `SetFont` → `OnStart(script)`)를 보면, **`Parse()` 단계에서 필드에 기본값이 채워지며 발생하는 `OnChange`는 화면의 `AX_SUB_OnStart_AX_`가 실행되기도 전에 이미 스크립트를 트리거하고 서버로 조회를 보낼 수 있다.** 스크립트 작성자가 "OnStart에서 초기화 로직을 다 마친 뒤 조회한다"고 가정하고 코드를 짜면 실제 실행 순서와 어긋날 수 있으므로, VBS/Python 변환 작업이나 신규 스크립트 작성 가이드에 반영할 가치가 있음.

### 관련 파일

| 파일 | 역할 |
|---|---|
| `Wizard/Screen.cpp:1748` | `CScreen::OnTRAN` — Send 직전 전체 필드 재검증 루프 |
| `Wizard/Screen.cpp:1807` | `CScreen::OnChange` — `IsChanged()` dirty-bit 체크 지점 |
| `Wizard/Stream.cpp:58` | `CStream::InStream(CScreen*, ...)` — `m_lock` 재진입 방지 |
| `Wizard/xscreen.cpp:366` | `CxScreen::_Send` — `m_script` 플래그 기반 dup 가드 |
| `Wizard/Screen.cpp:1802` | `CScreen::OnTRAN` 끝부분 — `[OnTRAN-gate]` 로그(2026-07-29 추가), `onSendResult`/`m_return` 값 노출 |
| `Wizard/Stream.cpp:1359` | `CStream::MakeStream` — 실제 송신 시점, `[0-MakeStream-send] ------ map=... tr=... ------` 배너 로그(2026-07-29 추가, `docs/DebugLogGuide.md` 6절) |
| `Wizard/Stream.cpp:1361` | `CStream::MakeStream` 맨 첫 줄 — `waitSN` 체크, `[MakeStream-waitSN-drop]` 로그(2026-07-29 추가) — 실제 두 번째 Send를 막은 지점 |
| `Wizard/Client.cpp:1927` | `CClient::WaitState` — 송신 성공 직후 `screen->m_state |= waitSN` 세팅 |

---

## 16. FlashGrid RTM 로그의 포맷스트링 타입 불일치로 인한 크래시 (2026-08-20)

### 증상

`CScreen::FlashGrid`(그리드 RTM 실시간 갱신, `@docs/MigrationSpec_SocketToDrawing.md` 8.11.4절)의 axlog 진단 로그(`[FlashGrid-write]`, 2026-08-19 추가, `@docs/DebugLogGuide.md` 6절)가 찍히는 순간 프로그램이 죽는 현상을 사용자가 실측 로그로 발견. Visual Studio에서 `h/axlog.h`의 `axlog()` 구현부, `CString funcLine;` 줄(포맷 문자열 조립이 끝난 뒤 다음 `CString`을 새로 만드는 지점)에서 예외로 멈추는 것까지 확인됨 — 즉 크래시 지점 자체는 `axlog` 내부처럼 보이지만 실제 원인은 그 직전 호출부에 있었음.

### 원인

`Wizard/Screen.cpp`(`CScreen::FlashGrid`)의 로그 호출:

```cpp
axlog(LOG_RTM, "[FlashGrid-write] name=%.16s code=%s row=%d col=%.16s old=[%.32s] new=[%.32s]",
    (char*)form->m_form->name, code.GetString(), ii, form->GetName(idx), text.GetString(), string.GetString());
```

`col=%.16s`(문자열 포인터를 기대하는 포맷 지정자)에 넘긴 `form->GetName(idx)`가 실제로는 `int`를 반환한다:

```cpp
// dll/form/fmBase.h:171
virtual int GetName(int col = -1) { return m_rts; }
// dll/form/fmGrid.h:205 (CfmGrid override, 실제 런타임 타입)
int GetName(int col = -1);
```

`CString::FormatV`가 가변인자를 포맷 문자열대로 순서대로 소비하는데, `%s` 자리에서 정수값을 포인터로 오인해 그 값을 주소로 역참조 — 대부분 유효하지 않은 메모리 영역이라 힙/스택이 오염되거나 접근위반이 발생한다. 크래시가 `FormatV` 호출 그 순간이 아니라 **다음 힙 할당 지점(`axlog.h`의 후속 `CString funcLine;` 생성자)에서 지연 발현**된 것도 이 유형의 버그(포맷스트링 타입 불일치로 인한 메모리 오염)의 전형적인 특징 — 죽는 줄과 원인 줄이 다르게 보여 진단이 헷갈리기 쉽다.

**참고 — 같은 함수(1036행) `fms->Lookup(form->GetName(idx), string)`은 정상이다.** `fms`(`CdataSet*`)는 필드코드를 정수 키로 쓰는 구조라 `GetName()`이 `int`를 반환하는 게 거기서는 맞는 용법. 문제는 오직 axlog 호출에서 그 정수를 `%s` 포맷에 그대로 넣은 것뿐 — 같은 반환값이라도 소비하는 쪽(정수 키 조회 vs 문자열 포맷)에 따라 옳고 그름이 갈리는 사례.

### 수정

`col=%.16s` → `col=%d`로 포맷 지정자를 실제 타입(`int`)에 맞게 교정(사용자 직접 적용, 2026-08-20). 로그 문구 외 다른 로직은 변경 없음:

```cpp
axlog(LOG_RTM, "[FlashGrid-write] name=%.16s code=%s row=%d col=%d old=[%.32s] new=[%.32s]",
    (char*)form->m_form->name, code.GetString(), ii, form->GetName(idx), text.GetString(), string.GetString());
```

같은 시점에 추가된 `[FlashSemi-write]`(Screen.cpp:1118)/`[ScrollRTM-insert]`(Screen.cpp:1186)는 `GetName()`을 쓰지 않아 이 버그와 무관함을 확인함.

### 교훈

`axlog`/`printf` 계열 가변인자 함수에서 크래시가 나면, **크래시가 찍힌 줄이 아니라 그 직전에 호출된 가변인자 함수(및 그 인자 타입)부터 의심**해야 한다 — 포맷 지정자와 실제 인자 타입이 어긋나면 컴파일러가 잡아주지 못하고(가변인자는 타입체크 대상 밖), 오염된 메모리가 다음 할당 시점에야 크래시로 드러나는 경우가 흔하다. 특히 이름이 `GetName()`인 함수라도 실제로 문자열을 반환한다고 가정하지 말 것 — 이 코드베이스에서는 `int`(내부 식별자 `m_rts`)를 반환하는 동명 함수가 실재한다.

### 관련 파일

| 파일 | 역할 |
|---|---|
| `Wizard/Screen.cpp:1048-1049` | `CScreen::FlashGrid` — 버그가 있던 axlog 호출, 수정 완료 |
| `dll/form/fmBase.h:171` | `CfmBase::GetName(int col=-1)` — `int m_rts` 반환 (기반 클래스 기본 구현) |
| `dll/form/fmGrid.h:205` | `CfmGrid::GetName(int col=-1)` — `int` 반환 오버라이드 (`FlashGrid`에서 실제 호출되는 런타임 타입) |
| `h/axlog.h` | `axlog()` 구현 — `msg.FormatV(fmt, args)` 직후 `CString funcLine` 생성 시점에 오염된 메모리로 인한 크래시가 지연 발현된 지점 |
| `@docs/DebugLogGuide.md` 6절 | `[FlashGrid-write]` 태그 설명 (2026-08-19 추가 당시 기록, 이번 버그 발생 시점) |
| `@docs/MigrationSpec_SocketToDrawing.md` 8.11.4절 | `FlashGrid`/`FlashSemi` 메커니즘 상세 |

---

## 17. 화면 대기(모래시계) 커서 메커니즘 — Wizard는 커서를 직접 그리지 않는다 (2026-08-26)

### 배경

종합화면 "조회시 약 1분 헹" 증상 조사(`[TR-RTT]`/`[CTRL-RTT]`/`[SVC-RTT]`/`[ServiceEx-wait]` 로그 추가 작업, `@docs/DebugLogGuide.md` 6절)와 같은 맥락에서, 사용자가 "종합화면의 특정 부분에 커서가 옮겨가면 그 화면의 상황에 맞게 커서가 바뀐다"는 걸 관찰하고 axwizard 소스 중 정확히 어떤 상황에서 대기(모래시계) 커서가 뜨는지 전수조사를 요청함. 모래시계가 실제로 어떤 내부 상태를 반영하는지 알면, 헹이 발생하는 순간을 커서 상태만 보고도 "지금 어느 채널이 걸려있는지" 추정할 수 있어 진단에 도움이 됨.

### 전체 체인 — Wizard(스크립트/네트워크) → 호스트 EXE(실제 SetCursor)

**핵심 발견: `axwizard.ocx`(Wizard.dll) 자체에는 `::SetCursor()`/`LoadCursor()` 호출이 전혀 없다**(`Wizard/` 전체 grep으로 확인). Wizard는 오직 "지금 대기 상태다/아니다"만 판단해서 COM Fire 이벤트(`waitPAN`, `h/axisfire.h:89`)로 호스트 EXE에 알릴 뿐이고, 실제로 마우스 커서를 `IDC_WAIT`로 바꾸는 코드는 전부 호스트 EXE(`AXIS.exe`, `AXIS/` 트리)에 있다.

```
[Wizard.dll — "대기 상태" 판정]
CClient::WaitState(screen, timeout)          Client.cpp:1927
    screen->m_state |= waitSN
    m_units.SetAt(screen->m_key, ...)        (그 작업영역 안에서 "아직 응답 안 온 unit" 집합)
    m_status |= S_WAIT
    if (timeout && m_guard->m_wait > 0)
        m_view->SetTimer(TM_WAIT, m_guard->m_wait, NULL)   ← 유일한 자동 복구 안전장치(발견1 참고)
    m_guard->PostAxis(MAKEWPARAM(waitPAN, m_key), true)    ← COM Fire 이벤트로 호스트에 통지
            │
            ▼  (COM OnFire, 프로세스 경계를 넘어 호스트 EXE로)
[AXIS.exe — 실제 커서 교체]
CMainFrame::OnFireXXX 의 case waitPAN:
    if (lParam) beginWait(HIWORD(wParam));   else endWait(HIWORD(wParam));
            │
            ▼
CMainFrame::beginWait(key)                   MainFrm.cpp:12162
    key에 대응하는 CChildFrame/CSChild/CMPop/CGPop을 찾아 그 창의 m_cursor = 1
    (key == m_activeKey면 그 자리에서 즉시 ::SetCursor(IDC_WAIT)도 호출)
            │
            ▼  (이후 사용자가 마우스를 그 창 위로 움직일 때마다 Windows가 재문의)
WM_SETCURSOR
CChildFrame::OnSetCursor / CSChild::OnSetCursor / CGPop::OnSetCursor
    if (m_cursor == 1) { ::SetCursor(IDC_WAIT); return TRUE; }
```

반대 방향(`WaitDone`)도 대칭 — `m_units`가 완전히 빌 때(그 작업영역의 모든 unit이 응답을 받았을 때)만 `S_WAIT`를 끄고 `PostAxis(waitPAN, key, false)` → `endWait(key)` → `m_cursor = 0`.

### Wizard가 "대기 상태"로 들어가는 트리거 5가지

1. **`Screen.Send(target)`/엔터키 조회 등** — `CStream::InStream()` 두 오버로드가 성공적으로 송신하면 항상 `WaitState(screen 또는 NULL, timeout=true)` 호출. **유일하게 `TranTimeout` 워치독이 걸리는 경로**(발견1).
2. **`Screen.Service(trN, data, len, mode)`** — `mode`에 `US_PASS`(0x04) 비트가 없으면 `CGuard::Service()`가 `CClient::WaitState()`를 거치지 않고 **직접** `screen->m_state |= waitSN` + `PostAxis(waitPAN,...)`를 실행 → 모래시계는 뜨지만 **워치독은 안 걸림**(발견1). `mode`에 `US_PASS`가 있으면 대기상태 진입 자체를 안 함(fire-and-forget, 커서 변화 없음).
3. **`Screen.ServiceEx(trN, data, len, mode, timeout)`** — 내부적으로 항상 `_Service(...,mode|US_PASS)`를 호출하므로 위 2번의 "대기상태 진입" 자체가 일어나지 않음(**모래시계가 안 뜬다**). 대신 `xscreen.cpp`의 자체 `PeekMessage` 루프로 UI 스레드를 직접 블로킹 — "커서는 안 바뀌는데 화면(다른 조작)은 멈추는" 유형의 헹이 이 경로에서 나올 수 있음(`@docs/DebugLogGuide.md`의 `[ServiceEx-wait]` 참고).
4. **`Screen.Wait` 스크립트 속성**(`CxScreen::_getWait`/`_setWait`, `xscreen.cpp:155`) — 스크립트가 `Screen.Wait = True`로 직접 강제 진입 가능. `WaitState(screen, timeout=false)`로 호출되므로 **역시 워치독 없음** — 스크립트가 실수로 `False`로 되돌리지 않으면(조건 분기 누락, 예외로 인한 조기 리턴 등) 이론상 무한정 모래시계가 뜬 채로 남을 수 있음.
5. `CGuard::Approve`/`UploadFile`/`DownloadFile` 등 다른 `CGuard::Write` 계열도 각자 `waitSN`/`PostAxis`를 직접 세팅하는 유사한 코드가 있음(`Guard.cpp` 3286/3358/3479행대 — `[CTRL-send]`/`[SVC-send]` 로그를 추가했던 바로 그 자리들과 같은 함수군) — 개별 화면 정책까지 전부 확인하지는 않았으나 패턴은 2번과 동일한 것으로 보임.

### 종합화면에서 부분(창)마다 커서가 다르게 바뀌는 이유

`waitPAN`의 key는 `CClient::m_key`(작업영역/창 단위)이고, `beginWait`/`endWait`가 `m_cursor` 플래그를 세팅하는 대상은 **그 key에 대응하는 개별 창(`CChildFrame`/`CSChild`/`CMPop`/`CGPop`) 하나씩**이다. 종합화면이 여러 개의 별도 작업영역(창)으로 구성돼 있다면, 그중 응답을 기다리는 중인 작업영역의 창만 `m_cursor=1`이 되고, 마우스가 창 경계를 넘어갈 때마다 Windows가 그 창에 `WM_SETCURSOR`를 새로 물어보므로 — 다른 부분(다른 작업영역의 창)으로 마우스를 옮기면 그 창 자신의 `m_cursor`(대개 0, 대기 없음)를 따라 즉시 화살표로 돌아온다. 사용자가 관찰한 "특정 부분에 커서가 옮겨가면 그 화면의 상황에 맞게 바뀐다"는 정확히 이 개별 창별 `m_cursor` 플래그 때문이다.

### 발견 1 — TranTimeout 워치독은 Send()에만 있고 Service()/Wait 속성에는 없음

`m_guard->m_wait`는 레지스트리 `HKCU\...\Workstation\TranTimeout`(초 단위, `h/axisvar.h:145`)에서 읽어 ms로 변환한다(`Guard.cpp:394`, 값이 없으면 기본 0=워치독 없음). `WaitState()`의 `if (timeout && m_guard->m_wait > 0) m_view->SetTimer(TM_WAIT, m_guard->m_wait, NULL)`가 유일한 자동 복구 지점 — `TM_WAIT` 타이머가 만료되면 `Event.cpp`의 `case TM_WAIT: client->WaitDone(NULL, true); client->m_guard->SetGuide(AE_TIMEOUT, client->m_key);`가 그 작업영역의 **모든** unit의 `waitSN`을 강제로 꺼버리고 `WM_GUIDE`로 안내(`AE_TIMEOUT`=22)를 띄운다.

**`TranTimeout`이 예컨대 60(초)로 설정돼 있다면, 이게 "약 1분 후에 풀어주는" 증상과 정확히 일치할 수 있다** — 서버가 어떤 이유로든 응답을 안/못 주는 상황에서 이 클라이언트 측 워치독이 60초 뒤 강제로 화면을 풀어주는 정상 동작일 가능성이 있음. **다만 이건 어디까지나 `Screen.Send()` 계열에만 적용된다** — 아래처럼 `Screen.Service()`/`Screen.Wait` 경로는 이 워치독 자체가 안 걸리므로, 만약 실제 헹의 원인이 이쪽이라면 60초에도 안 풀리고 그보다 훨씬 오래갈 수 있다.

**확인 방법:** 레지스트리 `HKEY_CURRENT_USER\Software\IBK투자증권MAC\AXIS Workstation V04.00\Workstation\TranTimeout` 값을 확인하면 현재 설정된 워치독 시간(초)을 알 수 있다. 값이 없거나 0이면 `Send()` 경로조차 워치독 없이 무한 대기한다.

### 발견 2 — 실사용 사례: IB000157/IB000130의 Screen.Service (워치독 없는 패턴)

이번 종합화면 CTRL-RTT 조사에서 이미 등장했던 두 서브맵(`IB000157`, `IB000130`)의 `.map` 바이너리를 직접 추출해 확인한 결과, 둘 다 동일하게:
```vbscript
SCREEN.Service "pidomyst", data, DataLen.Data, &H02
```
를 호출한다. `&H02` = `US_OOP`(0x02)이고 `US_PASS`(0x04)가 아니므로 — 위 "트리거 2"에 해당, **모래시계는 뜨지만 워치독(TranTimeout)이 전혀 안 걸리는** 패턴이다. 다만 호출 위치가 `_AW_ONCLOSE_AW_`(화면을 닫을 때, 설정 저장 여부를 묻는 `MsgBox` 확인 이후) — 즉 **화면을 여는/조회하는 시점이 아니라 닫는 시점**에 실행되므로, 이번에 캡처된 "화면 열자마자 발생하는 조회 헹"의 직접 원인일 가능성은 낮다. 그러나 "서버가 `pidomyst` 서비스에 응답을 안 주면 닫기 확인 후 화면이 워치독 없이 무한정 멈출 수 있다"는 별개의 잠재 위험으로 기록해둘 가치가 있음 — 종합화면을 **닫을 때**도 비슷한 헹이 보고되면 1차 의심 지점.

`Screen.Wait` 속성(트리거 4)을 이 종합화면 계열 스크립트가 실제로 쓰는지는 이번 조사에서는 확인하지 않았음(미조사).

### 발견 3 — 죽은 코드: ChildFrm/SChild의 axWAIT 처리 (로직 반전 + break 누락, 호출자 없음)

`CChildFrame::OnAXIS`/`CSChild::OnAXIS`/`CGPop::OnAXIS`에 `WM_AXIS`(`axWAIT` 서브명령, `axMsg.hxx:64`)로 커서를 직접 바꾸는 **또 다른 경로**가 남아있다:
```cpp
// ChildFrm.cpp:336, SChild.cpp:84 — 실제 살아있는 beginWait/endWait와 로직이 반대!
case axWAIT:
    m_cursor = (int) lParam;
    if (m_cursor)  ::SetCursor(IDC_ARROW);   // m_cursor=1(대기)인데 화살표?!
    else           ::SetCursor(IDC_WAIT);    // m_cursor=0(정상)인데 모래시계?!
                                              // ChildFrm은 break 없이 axLINKEDIT로 그대로 흘러들어감
```
`GPop.cpp:98`은 로직 자체는 정상(`m_cursor`가 참이면 `IDC_WAIT`)이지만, 이 세 곳 다 **`axWAIT`를 `PostMessage`/`SendMessage`로 실제 보내는 호출자가 코드베이스 전체에 단 한 곳도 없다**(전수 grep 확인) — 즉 현재는 완전히 도달 불가능한 죽은 코드다. 실제 라이브 경로는 `beginWait`/`endWait`가 `m_cursor`를 직접 멤버 대입하고 `OnSetCursor`(`WM_SETCURSOR`)가 그 값을 읽는 방식뿐이다. `COnTimer`(`@docs/KnowledgeBase.md` 14절)와 같은 유형의 leftover — 나중에 이 메시지 경로를 되살리는 리팩터링을 한다면 `ChildFrm.cpp`/`SChild.cpp`의 반전된 로직과 누락된 `break`부터 고쳐야 함.

### 관련 파일

| 파일 | 역할 |
|---|---|
| `Wizard/Client.cpp:1927` | `CClient::WaitState` — waitSN 세팅 + TranTimeout 워치독 타이머 무장 + waitPAN 발사 |
| `Wizard/Client.cpp:1956` | `CClient::WaitDone` — waitSN 해제, `m_units` 전부 비면 waitPAN(false) 발사 |
| `Wizard/Stream.cpp:44,58` | `CStream::InStream` 두 오버로드 — Send() 성공 시 `WaitState` 호출 지점 |
| `Wizard/Guard.cpp:3071` | `CGuard::Service` — `US_PASS` 없으면 `WaitState`를 거치지 않고 직접 waitSN/waitPAN 세팅(워치독 미적용) |
| `Wizard/xscreen.cpp:68,155-166` | `CxScreen::_getWait`/`_setWait` — 스크립트 노출 `Screen.Wait` 속성 |
| `Wizard/xscreen.cpp:992` | `CxScreen::_ServiceEx` — 항상 `US_PASS`로 우회, 대신 자체 `PeekMessage` 대기루프(`@docs/DebugLogGuide.md`의 `[ServiceEx-wait]` 참고) |
| `Wizard/Event.cpp:284-292` | `WM_TIMER`의 `TM_WAIT` 핸들러 — 워치독 만료 시 강제 `WaitDone` + `AE_TIMEOUT` 안내 |
| `Wizard/Guard.cpp:394` | `m_wait = GetProfileInt(WORKSTATION, TRANTMO, 0) * 1000` — 레지스트리 `TranTimeout`(초) 로드 |
| `h/axisfire.h:89` | `waitPAN` COM Fire 이벤트 정의 |
| `AXIS/MainFrm.cpp:12162,12199` | `CMainFrame::beginWait`/`endWait` — waitPAN 수신, 창별 `m_cursor` 세팅 |
| `AXIS/ChildFrm.cpp:384`, `SChild.cpp:532`, `GPop.cpp:202` | `OnSetCursor`(`WM_SETCURSOR`) — 실제 `::SetCursor(IDC_WAIT)` 호출 지점(라이브 경로) |
| `AXIS/ChildFrm.cpp:331`, `SChild.cpp:79`, `GPop.cpp:90` | `OnAXIS`의 `axWAIT` 분기 — 죽은 코드, ChildFrm/SChild는 로직도 반전 |
| `map_src/IB/IB0/IB000157`, `IB000130` | `_AW_ONCLOSE_AW_`에서 `SCREEN.Service "pidomyst",...,&H02` 사용(워치독 없는 실사용 사례) |
| `@docs/DebugLogGuide.md` 6절 | `[SVC-send]`/`[SVC-RTT]`/`[ServiceEx-wait]` — 이번 조사와 같은 맥락에서 추가된 msgK_SVC 진단 로그 |

---

## 18. 추가 자료

### 참고 문서
- `@docs/Architecture.md` - 모듈 구조
- `@docs/Dependency.md` - 라이브러리 의존성
- `@docs/python_engine_260608.md` - 프로젝트 상세 기록
- `@docs/CallGraph.md` - 함수 호출 흐름
- `@docs/WizardArchitecture.md` - axwizard 클래스 계층/이벤트 흐름 상세 분석 (2026-07-13)
- `@docs/DebugLogGuide.md` - axlog 태그 카탈로그 (이번 조사에 사용한 클립보드 로그 포함)

### 외부 참고
- [Python C API 문서](https://docs.python.org/3.11/c-api/)
- [COM 및 Automation](https://docs.microsoft.com/en-us/windows/win32/com/com-technical-reference)
- [MFC 문서](https://docs.microsoft.com/en-us/cpp/mfc/mfc-desktop-applications)

---

**최종 수정:** 2026-08-26
**기여자:** Documentation Agent
