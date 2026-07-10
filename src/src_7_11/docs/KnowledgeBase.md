# KnowledgeBase

- 생성일: 2026-07-07
- 목적: 분석·리뷰·디버깅 과정에서 얻은 설계 의도, 트러블슈팅 히스토리, 주의사항, 도메인 지식을 누적한다.

## 개요

src_7_11/platform은 AxisBuilder 맵/화면 컴파일러+에디터 도구체인으로, awWcc가 핵심 VBScript/Python 컴파일러 DLL. 런타임은 ibks/Wizard의 axWizard.ocx가 컴파일된 .map 바이너리를 로드하고, 스크립트는 src_7_11/platform/dll/vbs (또는 ibks/dll/vbs)의 axisvbs.dll (CEngineWrapper + CScriptEngine for VB, CPythonEngine for Python)로 실행.

## 목차

1. Module Ownership Boundaries (모듈 소유권 경계)
2. Dual-Tree VBS Source Management (이원화된 소스 트리 관리)
3. Compile-Time Event Handler Wrapping (컴파일 시 Sub-End-Sub 래핑)
4. Mixed-Language Limitation (맵별 단일 언어 제한)
5. Indentation-Stripping Bug (들여쓰기 제거 버그 수정)
6. Diagnostic Infrastructure (디버그 인프라)
7. First End-to-End Verification (Python OnStart 최초 성공, 2026-07-08)
8. Explicit pythonMode Architecture (추측 방식 → 명시적 플래그 방식, 2026-07-08)
9. Python Authoring Notes (Python 스크립트 작성 시 주의사항, 계속 갱신)

---

## 1. Module Ownership Boundaries (2026-07-07)

**확인 방법**: 각 프로젝트의 .vcxproj AdditionalDependencies 검사.

- **Builder-only** (Wizard가 링크하지 않음): awWcc (맵 컴파일러), awBuild, awDlg, awTool, awUser, awCommon, awSock, awObject.
- **Shared** (Builder와 Wizard 모두 링크): axislib, axisform.
  - Builder 측: awObject/awDlg/awTool/awCommon이 링크 → 폼 에디팅/미리보기용.
  - Wizard 측: 런타임 렌더링용.
- **Wizard-only** (Builder는 링크하지 않음): axisvbs (스크립트 실행 엔진 — CScriptEngine for VB, CPythonEngine for Python).
  - axisvbs.dll이 빌드 출력 폴더에 나타나는 이유: 모든 모듈이 동일 출력 디렉토리를 공유하기 때문이지, AxisBuilder.exe가 실제로 로드하는 것은 아님.

## 2. Dual-Tree VBS Source Management (2026-07-07)

**상황**: 동일한 vbs 엔진 코드가 두 곳에 존재:
- `ibks/dll/vbs/` (ibks 트리)
- `src_7_11/platform/dll/vbs/` (src_7_11 트리)

**컴파일 타임 링크**: ibks/Wizard/Screen.cpp는 `#include "../dll/vbs/engineWrapper.h"` (quoted-include, 상대 경로)로 컴파일 시점에 ibks/dll/vbs를 참조.

**실제 배포 DLL**: D:\util\HTS\IBK_SMART\exe\axisvbs.dll는 src_7_11/platform/dll/vbs/axisvbs10.vcxproj에서 빌드됨 (파일 크기+타임스탬프 일치 확인, 및 Wizard.vcxproj Release 설정이 절대 경로 `D:\src\IBKS\src\src_7_11\platform\dll\vbs\Release\axisvbs.lib` 명시적 링크).

**이원화 원인**: 클래스 인터페이스(engineWrapper.h 등)가 비가상 메소드이고 바이트 동일하여 ABI 호환성 문제 없음. 그러나 혼동의 원인.

**결정 (2026-07-07)**: **ibks/dll을 참조/마스터 트리로 지정**. src_7_11/platform/dll/vbs는 ibks/dll/vbs와 동기화 유지. 오늘 isPythonScript() 감지 로직이 유일한 실제(공백 제외) 차이였으며 동기화 완료 (둘 다 `line.TrimLeft(); line.Find("def ")==0 || line.Find("import ")==0` 사용, 문자열 어디든지 검색 아님).

## 3. Compile-Time Event Handler Wrapping (2026-07-07)

**메커니즘**: src_7_11/platform/builder/awWcc/mapbld.cpp의 `CCompile::mapResStrBuilding()` (per-line 콜백, 컴파일 드라이버가 각 라인마다 호출):
1. 에디터(awBuild/build.cpp)가 각 이벤트 스크립트 텍스트 주위에 삽입한 고정 마커 토큰 인식:
   - `_AW_ONSTART_AW_`, `_AW_ONCLICK_AW_`, `_AW_ONENDSUB_AW_` 등 (Maptbl.hxx의 scriptTbl/fEventTbl에 정의).
2. `writeScript()` 호출로 컴파일된 스크립트 blob에 직접 작성:
   - VBScript: `"Sub AX_SUB_OnStart_AX_()"` / `"End Sub"`, 또는 제어별 `"Sub AX_<ControlSymbol>_On<Event>_AX_()"`
3. **Python 인식 추가 (2026-07-07)**: Sub-End-Sub 래핑 보류(Compile.h의 새 멤버 `m_pyAwait`/`m_pyBlock`/`m_pyWrap`에 저장, compile.cpp의 `mapMemoryAllocation()`에서 초기화):
   - 이벤트 블록의 첫 실제 내용 라인까지 대기.
   - 첫 라인이 `def ` 또는 `import `로 시작하면: 전체 블록을 Python으로 취급, Sub-End-Sub 래핑 완전히 스킵.
   - 사용자는 정확한 네이밍 컨벤션 `CScript::getProcName()` (ibks/Wizard/Script.cpp)과 정확히 일치하는 `def AX_<Symbol>_On<Event>_AX_():` 작성 필요.
   - 빈 블록(마커 직후 ONENDSUB, 실제 내용 없음): VBScript 래퍼 작성 (안전장치).

## 4. Mixed-Language Limitation (2026-07-07)

**제한사항**: 단일 맵/화면은 여러 이벤트 핸들러 간 VBScript와 Python을 혼합할 수 없음.

**이유**: 
- 컴파일 타임: 화면의 모든 스크립트 섹션(declare/procedure 섹션, OnStart, OnSend, 제어별 OnClick/OnChange/등)이 ONE 평탄 텍스트 blob으로 연결됨 (컴파일된 `_mapH`의 scriptR..strR 영역).
- 런타임: ibks/Wizard/Screen.cpp::Parse()가 이 전체 blob을 하나의 CString으로 추출, **한 번에** `m_vbe->LoadScript(text)` 호출.
- `CEngineWrapper::isPythonScript()` (src_7_11/platform/dll/vbs/engineWrapper.cpp 또는 ibks/dll/vbs/engineWrapper.cpp): 전체 blob을 스캔, `def `/`import `로 시작하는 라인을 찾으면 전체 blob을 CPythonEngine으로 라우팅 → VBScript 섹션(남은 VBScript Sub 선언 등)이 Python 구문 오류 유발.

**현재 해결책**: 각 맵/화면은 완전히 한 언어여야 함. 다른 화면(별도 CScreen 인스턴스, 각각 독립 CEngineWrapper)은 서로 다른 언어 가능 (언어 감지는 per-screen).

**미래 개선**: 전체 blob 대신 per-event-block 엔진 디스패치 필요 (Todo 참고).

## 5. Indentation-Stripping Bug (2026-07-07)

**버그 위치**: src_7_11/platform/builder/awWcc/misc.cpp의 `CCompile::mapSourceFileLoad()` (raw 중간 소스 파일을 m_text[] 라인 배열로 읽기 전에 토큰화):
- 윈도우 핸들로 컴파일 시(`hwnd != NULL`) 각 라인의 선행 공백 무조건 제거: `text = &text[ii]` (ii = 첫 non-space/tab 인덱스).
- 발생: "Build" 대화형 명령(MainFrm.cpp가 this 전달), 배치 빌드(awDlg/BATCH.CPP가 더미 non-null `(CWnd*)1` 전달).
- **VBScript**: 무해 (공백 불민감).
- **Python**: 들여쓰기 파괴 → `IndentationError: expected an indented block`.

**수정 (2026-07-07)**: 
- `rawText` 포인터 도입, strip 이전에 캡처.
- 최종 저장에만 사용: `m_text[pLine].text = rawText` (실제 저장/토큰화 내용).
- 기존 stripped `text` 변수: 모든 결정 로직(공백 라인 스킵, `&` 연속 라인 감지, RESOURCE DIVISION 키워드)에 사용 → 20+ 년된 VBScript 파싱 거동 회귀 방지.

**검증**: 컴파일된 .map 파일의 scriptR..strR 바이트 범위를 hex-dump(`xxd -s <scriptR> -l <len> <mapfile>`) — 이제 표준 빠른 검증 기법.

## 6. Diagnostic Infrastructure (2026-07-07)

모든 디버깅 추적은 OutputDebugString (DebugView++로 확인):

- **[WIZARD][SCRIPT][DEBUG]** 
  - ibks/Wizard/Screen.cpp: LoadScript 성공/실패 + 오류 메시지, LoadScript에 전달된 정확한 text 내용/길이.
  - src_7_11/platform/dll/vbs/scriptEngine.cpp의 getIDOfProcedure(): 조회된 procedure 이름, GetIDsOfNames의 HRESULT.

- **[WIZARD][PYTHON][DEBUG]**
  - src_7_11/platform/dll/vbs/pythonEngine.cpp의 LoadScript(): PyRun_String에 전달된 최종 문자열 (0x0E/0x0C 바이너리 마커 제거 후).

- **[AWWCC][DEBUG]**
  - src_7_11/platform/builder/awWcc/libWcc.cpp: wccAllocateMemory/wccBuildObject 진입점, 빌드 버전 문자열 태그(2026-07-07-1 — fresh awWcc.dll과 stale one 구분, 실행 중 프로세스는 DLL hot-reload 불가, AxisBuilder.exe 재시작 필요).
  - mapbld.cpp의 writeScript(): 각 문자열 작성 + 누적 m_scriptN.
  - misc.cpp의 mapBuildObjectFile(): 저장 시 최종 m_scriptN/scriptR/strR 헤더값.

**주의**: 이 추적들은 근본 원인 분석 필수였음 (특히: 컴파일러 내부 버퍼 계산 vs 컴파일된 파일 실제 바이트 vs 런타임이 받은 것 비교). Python 기능이 안정화되면 제거 또는 debug-only 플래그 뒤로 이동 필요 (Todo 참고).

## 7. First End-to-End Verification (2026-07-08)

**절차**: awWcc 리빌드(MSBuild, VS2019 Professional) → AxisBuilder 재시작 → 동일 맵 재컴파일 → HTS에서 실행.

**1차 시도 실패 (예상된 결과, 새 버그 아님)**:
- 빌드 로그(`[AWWCC][DEBUG]`)상 컴파일은 정상: `Sub Testpro(ival)...End Sub`(VBScript, 공용 프로시저), `def AX_SUB_OnStart_AX_():` (Python, Sub/End Sub 래핑 스킵 + 들여쓰기 보존 확인), `Sub AX_BUTTON1_OnClick_AX_()...End Sub`, `Sub AX_cb1_OnChange_AX_()...End Sub` (둘 다 VBScript) 가 각각 올바른 형태로 작성됨.
- 런타임에서 `PyRun_String src=[Sub Testpro(ival)...` 로 blob 전체가 Python에 전달되어 `invalid syntax (<string>, line 1)` 발생.
- 원인: 이 테스트 화면이 **섹션 4(Mixed-Language Limitation)** 를 위반 — Testpro 선언부/OnClick/OnChange는 VBScript, OnStart만 Python으로 혼재되어 있었고, `isPythonScript()`가 blob 어딘가에서 `def `를 찾아 전체를 Python으로 라우팅했기 때문. **awWcc 컴파일러 자체는 정상 동작**한 것으로 확인.

**2차 시도 성공**: 화면을 순수 Python(모든 이벤트 블록 Python)으로 맞춰 재테스트 → `Screen.SetMessage("Python OnStart OK")` 실행, 흰색 토스트 메시지 정상 표시/소멸.

**부가 확인**: `Screen.SetMessage`는 Python 전용 API가 아니라, `platform/wizard/Wizard.odl`에 `[id(14)] void SetMessage(BSTR msg);` 로 선언된 `CScreen`(`platform/wizard/xscreen.cpp/h`의 `_SetMessage`) COM Dispatch 메서드. VBScript(`Screen.SetMessage "msg"`)와 Python(`Screen.SetMessage("msg")`) 양쪽에서 동일한 dispid=14를 호출하는 것으로, 언어별 특수 기능이 아니라 기존 Screen 객체 모델을 Python에서도 그대로 쓸 수 있음을 보여주는 사례.

**결론**: awWcc 컴파일 시점 Python 인식(Sub 래핑 스킵 + 들여쓰기 보존) + axisvbs 런타임(CPythonEngine COM dispatch) 전체 파이프라인이 **순수 Python 화면 기준으로 최초 End-to-End 검증 완료**. 혼합 언어 화면은 여전히 섹션 4의 제약을 받음(미해결, Todo의 Per-Event-Block Engine Dispatch 항목 참고).

## 8. Explicit pythonMode Architecture (2026-07-08)

**배경**: 섹션 4(Mixed-Language Limitation)의 근본 원인은 "코드 내용을 스캔해서 Python인지 추측"하는 방식 자체였다. 화면 하나에 VBScript와 Python 이벤트 블록이 섞이면 `isPythonScript()`/컴파일러 휴리스틱이 오판할 수 있었다 (섹션 7의 1차 시도 실패가 실제 사례).

**변경 내용**:
1. **바이너리 구조체(`_mapH`)에 `pythonMode` 필드 추가** — `src_7_11/platform/h/mapform.h`(및 `h/`, `rm/h/`, `platform/h_우선백업/`, `ibks/h/mapform.h` 동기화). 기존 `BYTE rsv[32]`(미사용 예비 공간)의 첫 1바이트를 재활용해 `BYTE pythonMode; BYTE rsv[31];`로 변경 — 구조체 크기·이후 필드 오프셋 불변, **기존 배포 .map 파일과 100% 바이너리 호환** (실제 배포 화면 `D:\util\HTS\IBK_SMART\map\IB\IB2\IB200011`의 해당 오프셋을 hex-dump로 0인 것 확인).
2. **에디터 측(`platform/builder/MAPFORM.H`)에는 이미 `pythonMode` 필드와 자동감지 로직이 존재했다** — `awBuild/mapLoad.cpp::CMapLoad::SaveScriptFile()`이 스크립트 저장 시 `def `/`import `를 포함하면 `m_mapH->pythonMode = true`로 설정(한 번 켜지면 꺼지지 않는 단방향). 다만 이 값을 컴파일러로 넘기는 코드가 빠져 있었음(미완성 상태로 방치돼 있었던 것으로 추정).
3. **`awBuild/build.cpp::CBuild::generateHeader()`에 `PYTHON(%d)` 지시어 출력 추가** — 기존 `MAPNAME`/`MAPSIZE`와 같은 방식으로 `m_mapH->pythonMode` 값을 컴파일 소스 텍스트에 직렬화.
4. **`awWcc/Maptbl.hxx`에 `"PYTHON"` 전역 지시어(flow=21) 추가**, **`awWcc/mapbld.cpp::mapGlobalStrBuilding()`에 `case 21`** 추가 — 소스의 `PYTHON(1)`을 읽어 컴파일러 측 `m_mapH.pythonMode`에 반영.
5. **`awWcc/mapbld.cpp::mapResStrBuilding()`의 이벤트 블록 첫 줄 `def `/`import ` 스캔 로직 삭제**, `m_mapH.pythonMode` 값을 직접 참조하도록 교체 — 화면 전체 단위로 Sub/End Sub 래핑 여부를 **명시적으로** 결정 (더 이상 블록별 추측 없음).

**Phase 4 완료 (2026-07-08)**: `CEngineWrapper::LoadScript(CString scripts, int scpKind = -1)`로 시그니처 확장 — `scpKind`가 `-1`이면 기존 `isPythonScript()` 텍스트 스캔(레거시 fallback), `0`/`1`이면 각각 VBScript/Python으로 명시 선택. `ibks/Wizard/Screen.cpp`가 `m_vbe->LoadScript(text, m_mapH->pythonMode)`로 호출하도록 변경(`src_7_11/platform/wizard/Screen.cpp`도 동일하게 변경했으나 이 트리가 실제 배포에 쓰이는지는 미확인). `[WIZARD][ENGINE][DEBUG]` 로그로 `scpKind` 값과 `source=explicit pythonMode` 여부를 확인 가능.

**Phase 2 완료 (2026-07-08)**: 최초 계획 문서(`purrfect-wandering-blossom.md`)는 PY 토글 버튼 위치를 `awDlg/mapScript.h/cpp`(`CMapScript`, PropertyPage)로 명시했으나, 실제 구현 중 `MainFrm.cpp`를 추적한 결과 사용자가 실제로 쓰는 스크립트 창은 `platform/builder/ScriptBar.h/cpp`(`CScriptBar`, `axisWork.vcxproj` 소속, `MainFrm.cpp`가 `"Script window"`라는 이름으로 도킹 패널로 생성)임을 확인 — `CMapScript`는 별개의 더 제한적인(이벤트 5종만 지원) 에디터로, 이번 작업 대상에서 제외. `ScriptBar.cpp::OnPyBtnClick()`가 `m_mapH->pythonMode` 토글 + `m_pythonBtn` 체크 상태 갱신 + `m_editScript.SetPythonMode()` 호출(키워드셋/주석문자 `'`↔`#` 전환) + `modified=true`만 수행하며, 스크립트 텍스트 자체는 건드리지 않는다(자동 변환/삭제 없음).

**Phase 5 검증 완료 (2026-07-08)**: Python 화면(PY 버튼 눌린 상태로 컴파일)과 VBScript 화면 2건(신규 테스트 맵 + 기존 배포 맵 `실시간잔고`)을 각각 재컴파일 후 HTS에서 로드 — DebugView++ 로그 전부 `source=explicit pythonMode`로 확인(`scpKind=1→Python`, `scpKind=0→VBScript`), 레거시 텍스트 스캔 fallback 경로를 타지 않음을 확인. 기존 VBScript 화면 회귀 없음.

**여전히 남는 제약**: `pythonMode`는 여전히 **화면(맵) 전체 단위**다. 한 화면 안에서 이벤트 블록별로 언어를 섞어 쓰는 것은 이번 변경으로도 지원되지 않는다 (Todo.md "Per-Event-Block Engine Dispatch" 참고, 별도 향후 과제).

**이벤트 핸들러 자동 래핑 추가 (2026-07-08, 후속 개선)**: 최초 구현(위 4단계)까지는 `pythonMode=true`여도 컴파일러가 `def AX_..._AX_():` 헤더를 자동 생성해주지 않아, 사용자가 이벤트 박스마다 이 줄을 정확한 철자로 직접 써야 했다(VBScript는 `Sub`/`End Sub`을 컴파일러가 자동으로 감싸주는 것과 비대칭). `awWcc/mapbld.cpp::mapResStrBuilding()`을 수정해 VBScript와 동일하게 동작하도록 개선:
- 이벤트 블록 시작 시 `m_pyWrap`(예: `"Sub AX_BUTTON0_OnClick_AX_()"`)을 `"def AX_BUTTON0_OnClick_AX_():"`로 변환해 컴파일러가 자동으로 씀.
- 블록이 끝날 때까지(`ONENDSUB`) 사용자가 입력한 각 줄 앞에 4-space 들여쓰기를 자동으로 붙여서 씀(`writeScript()`가 호출당 한 줄+개행을 기록하는 구조라 줄 단위 prefix 삽입으로 충분).
- 결과: 이벤트 박스에는 이제 본문만 쓰면 된다 (예: `Testpro("1")` 한 줄). `def AX_..._AX_():` 헤더나 함수명 철자를 사용자가 신경 쓸 필요 없음 — VBScript에서 `Testpro "1"` 한 줄만 쓰던 것과 동일한 경험.
- **주의(호환성)**: 이 변경 이후로는 이벤트 박스 본문에 사용자가 직접 `def AX_..._AX_():`를 쓰면 안 된다(컴파일러가 자동 생성한 헤더 안에 중첩 함수로 잘못 들어가게 됨). 이 변경 이전(2026-07-08 오전)에 수동으로 `def`를 써서 테스트했던 화면(OnStart 테스트 맵 등)은 `def` 줄을 지우고 본문만 남긴 뒤 재컴파일해야 한다. `_declaration_` 섹션(전역 함수 정의, 예: `Testpro`)은 이 자동 래핑 대상이 아니므로 계속 사용자가 `def Testpro(ival):`을 직접 작성한다(VBScript의 `Sub Testpro(ival) ... End Sub`와 동일한 위치).

**빈 본문 안전장치 추가 (2026-07-08, 후속 개선 2)**: 기존 VBScript 화면을 Python으로 옮기는 과정에서, 아직 변환 못 한 줄을 `#`로 주석 처리만 해두고 실제 코드가 없는 이벤트 박스가 흔히 생긴다. Python은 `def f():` 다음에 주석/빈 줄만 있고 실제 문장(statement)이 하나도 없으면 `IndentationError: expected an indented block after function definition`로 컴파일 자체가 실패한다(전체 화면의 스크립트가 하나의 Python 모듈로 한번에 컴파일되므로, 화면 안의 어느 박스 하나만 이래도 전체가 깨짐). 이를 막기 위해 `mapbld.cpp::mapResStrBuilding()`의 `case 17`(ONENDSUB)이 Python 모드의 모든 이벤트 블록 끝에 `    pass`를 자동으로 한 줄 추가하도록 변경 — 실제 코드가 있는 블록에서는 그냥 무해한 마지막 줄이 되고(Python은 함수 끝에 `pass`가 있어도 아무 영향 없음), 주석만 있거나 완전히 빈 블록에서는 이 `pass`가 유효한 문장이 되어 컴파일이 통과한다. (완전히 빈 블록—마커 사이에 내용이 전혀 없는 경우—도 이제 `def` 헤더가 정상적으로 생성되도록 함께 수정: 이전에는 이 경로가 VBScript용 `Sub ...()` 텍스트를 그대로 써버리는 버그가 있었음.)

## 9. Python Authoring Notes (2026-07-08~, 계속 갱신)

Python 문법에 익숙하지 않은 상태로 화면을 작성하며 발견된 특이사항을 모아둔다. Python/VBScript를 함께 다뤄야 하는 만큼, VBScript와 다른 점 위주로 기록.

**1. 함수 정의 문법**
```python
def Testpro(ival):
    ed1.Data = ival
    out1.Data = ival
```
- `Sub ... End Sub` → `def 이름(매개변수):` 한 줄, 별도 종료 키워드 없음
- **들여쓰기가 문법이다**: VBScript는 들여쓰기가 무의미하지만 Python은 들여쓰기로 블록을 구분한다. 안 맞으면 `IndentationError`.

**2. 이벤트 핸들러는 이제 본문만 작성하면 됨 (2026-07-08 개선 이후)**
- 컴파일러가 `def AX_<컨트롤심볼>_On<이벤트>_AX_():` 헤더를 자동으로 생성해준다 (섹션 8 참고). 이벤트 박스(OnClick/OnChange/OnStart 등)에는 VBScript 때처럼 본문만 쓰면 됨. 예: `Testpro("1")` 한 줄.
- 단, `_declaration_` 섹션(전역 함수를 정의하는 곳)은 예외 — 여기 정의하는 함수는 이름을 자유롭게 정하므로(예: `Testpro`) 사용자가 `def Testpro(ival):`을 직접 작성해야 한다 (VBScript의 `Sub Testpro(ival) ... End Sub`와 동일한 위치/역할).

**3. COM 객체 속성 읽기는 `()` 호출이 필요, 쓰기는 필요 없음 (비대칭 — Python 엔진만의 특이점)**
- 원인: `src_7_11/platform/dll/vbs/pythonEngine.cpp`의 `AxisObject_getattro()`가 속성을 바로 값으로 반환하지 않고, `AxisMethod`라는 "호출 대기" 래퍼 객체를 반환한다 (`AxisMethod_call`이 실제 COM Invoke를 수행). 반면 `AxisObject_setattro()`는 대입을 바로 COM PUT으로 처리한다.
- **읽기**: `cb1.Data` 만 쓰면 값이 아니라 `<axisvbs.AxisMethod object at 0x...>` 가 나온다. 반드시 `cb1.Data()` 처럼 괄호를 붙여 "호출"해야 실제 값을 얻는다.
- **쓰기**: `ed1.Data = ival` 는 괄호 없이 그대로 동작한다 (VBScript와 동일).
- 실수 사례: `Screen.SetMessage(str(cb1.Data))` → 항상 `<axisvbs.AxisMethod object at ...>` 문자열이 표시됨. 올바른 코드: `Screen.SetMessage(str(cb1.Data()))`.

**4. VBScript 내장 함수는 Python에 없다**
- `MsgBox` 등은 VBScript 스크립트 엔진 자체의 내장 함수이며, `axisvbs`가 Python용으로 별도 등록해준 적이 없다 (`pythonEngine.cpp`에 `MsgBox` 참조 없음 확인).
- 괄호 없이 `MsgBox ival` 처럼 쓰는 것도 VBScript 문법이라 Python에서는 `SyntaxError`.
- 메시지 표시가 필요하면 `Screen.SetMessage(...)` (COM으로 노출된, 언어 무관 API, 섹션 7 참고)를 사용한다.

**5. 소스에 없는 예시**: 위 항목들은 실제 테스트 화면(`D:\src\IBKS\기획\axibuilder python\new builder src\IB999987`)에서 발견된 것이며, 앞으로 새로운 특이사항이 발견되면 이 섹션에 이어서 추가한다.

**5--2. `#load` include 지시어의 Python용 대체 문법 `@load` 추가 (2026-07-08)**
- 배경: `#load IB999901`처럼 다른 맵의 스크립트/공개변수를 불러오는 지시어가, Python 모드에서는 에디터가 `#`를 주석 색으로 칠해버려 "이게 진짜 주석인지 아닌지" 혼동을 유발함(실제로는 주석이 아님 — 섹션 5-3 참고). 사용자 요청으로 Python 모드 전용의, 주석 기호와 겹치지 않는 새 문법을 추가.
- 수정: `CAxisForm::getExternalScript()`(`ibks/dll/form/axform.cpp`, `src_7_11/platform/dll/form/axform.cpp` 동일 적용 — `axisform.vcxproj` 소속, Builder/Wizard 공유 모듈)가 `"#LOAD "` 탐색에 실패하면 `"@LOAD "`도 추가로 탐색하도록 변경. 기존 `#load` 문법은 완전히 그대로 유지(하위 호환) — VBScript 화면은 계속 `#load`, Python 화면은 `@load` 사용 권장.
- 예: `@load IB999901` — 에디터에서 일반 텍스트 색으로 표시되어(주석 아님이 시각적으로 명확) Python 스크립트와 구분됨. 실행에는 어차피 영향 없음(이 블록은 `getRoutine()`이 스크립트 엔진에 전달되기 전에 통째로 잘라내므로 Python/VBScript 어느 쪽도 이 텍스트를 직접 실행하지 않음).
- 재빌드 대상: `axisform.dll`(Builder와 Wizard 양쪽에서 링크하므로, 두 실행파일 모두 재빌드 필요).

**5--3. `#load`가 진짜 주석이 아님을 실전 확인 (2026-07-08)**
- 사용자가 Python 모드에서 `#load IB999901`을 실제로 지워보니 컴파일 결과가 달라짐(포함되던 내용이 안 들어옴) → `#load`가 단순 주석이 아니라 실제로 동작하는 지시어임을 실전으로 확인. 섹션 5--0/8의 "#load는 Python 엔진에 도달하기 전에 별도 처리되어 영향 없다"는 결론과 일치 — 단, "영향 없다"는 built-in 여부(functional correctness)에 대한 것이었고, 이번에 확인한 건 "화면에는 주석처럼 보이지만 실제로는 살아있는 지시어"라는 별개의(관련되지만 다른) 사실.
- 부가 발견: `IB999901`(공용 라이브러리 맵, 순수 VBScript, If/End If 다수)의 112번째 줄 `End If  '`이 실전에서 만난 "unterminated string literal (line ...)" 에러의 정확한 원인이었음(맵 소스 직접 대조로 확인). `IB999987`의 `OnSend` 박스가 이 라이브러리의 `SetMKGubn777()`(457번째 줄)을 호출하고 있어, `#load IB999901`을 그냥 지우면 컴파일은 되지만 `SetMKGubn777()` 호출 시 `NameError`가 날 것으로 예상됨.
- **마이그레이션 시사점**: `#load`/`@load`로 공용 라이브러리를 불러오는 화면은, 그 라이브러리 맵 자체도 Python으로 전환해야 완전히 동작함. 이런 공용 라이브러리 맵(`IB999901` 등)은 여러 화면에서 재사용되므로, 우선순위 높은 전환 대상 후보로 별도 추적 필요(Todo.md 참고).

**5--1. `RootDir` 미해결로 `public.ini`(자동완성/대소문자교정 데이터) 로딩 실패 (발견 및 우회, 2026-07-08)**
- 증상: 새 로컬 빌드 폴더(`D:\src\IBKS\src\release`)에서 스크립트 에디터의 키워드 자동 대소문자 교정, `.` 멤버 자동완성 드롭다운이 전혀 동작하지 않음.
- 원인: `MainFrm.cpp:3128`이 `workshop.ini`의 `[Environment]` `PROJECTNAME` 값을 읽는데, 이 키가 없으면 기본값 `"AXIS"`를 사용(`GetPrivateProfileString(ENVIRONMENT, "PROJECTNAME", "AXIS", ...)`). 이 값이 비어있지 않으면 `ID_USR_GETROOTDIR` 핸들러(`MainFrm.cpp:3348`)가 `workshop.ini`의 `RootDir`를 직접 읽는 대신 **레지스트리**(`AfxGetApp()->GetProfileString(ENVIRONMENT, ROOTDIR)`, `setRegistryKey("AXIS")`로 설정된 키 하위)에서 읽으려 하는데, 신규 개발 환경에는 이 레지스트리 값이 등록된 적이 없어 빈 문자열을 반환 → `ScriptBar::LoadAutoList()`가 `public.ini`를 찾지 못함(`root` 부분이 빈 경로 `\tab\public.ini`가 됨).
- `GetPrivateProfileString` API 자체나 `workshop.ini` 파일 내용은 문제없음(PowerShell로 동일 API 직접 호출해 정상 반환 확인).
- 해결(사용자 로컬 설정): `workshop.ini`의 `[Environment]` 섹션에 `PROJECTNAME=`(빈 값)을 추가하면 `m_sProjectName`이 빈 문자열이 되어 레지스트리 대신 `workshop.ini`의 `RootDir`를 직접 읽는 분기를 타게 됨 — 코드 변경 아님, 로컬 설정 파일만 수정.
- 진단용 로그 `[AXISWORK][ROOTDIR][DEBUG]`(`MainFrm.cpp`), `[AXISWORK][AUTOLIST][DEBUG]`(`ScriptBar.cpp`) 추가함(Todo.md 진단 제거 목록에 포함, 안정화 확인 후 정리).
- **해결 확인 완료(2026-07-08)**: `workshop.ini`에 `PROJECTNAME=` 추가 후 로그가 `ini branch: ... err=0 value=[D:\src\IBKS\src\release]`로 정상 출력, 에디터에서 `SCREEN.` 대문자 변환 및 자동완성 정상 동작 확인.

**5-0. 에디터가 전역 객체 이름을 자동으로 대문자로 강제 변환하는 문제 (발견 및 수정, 2026-07-08)**
- 증상: Python 모드에서 `screen`(소문자)이라고 타이핑하고 식별자를 끝내는 순간(공백/괄호 등), 에디터가 자동으로 `SCREEN`(전부 대문자)으로 바꾸고 색을 입힌다.
- 원인 추적: `PUBLIC.INI`(예: `D:\util\HTS\IBK_SMART\tab\PUBLIC.INI`)의 `[CONTROLS]` 섹션에 `23 = SCREEN;화면`처럼 전역 엔진 객체(SCREEN, SYSTEM 등)가 전부 대문자로 등록되어 있고, `ScriptBar.cpp::LoadAutoList()`가 이 파일을 읽어 `CVBScriptEdit::m_ScriptStrMap`에 그 대문자 키 그대로 채운다. `VBScriptEdit.cpp::IsSymbol()`이 입력값을 `MakeUpper()`로 무조건 대문자화한 뒤 이 맵에서 찾고, 찾으면 원래 대소문자로 복원하지 않은 채 그대로 반환한다(반면 폼에 실제 등록된 컨트롤 심볼은 정확한 등록 대소문자로 복원됨 — 이 경로는 문제없음). `ParseLine()`이 이 값으로 편집 버퍼를 직접 덮어쓴다(`CopyMemory`).
- 문제가 되는 이유: VBScript는 대소문자를 구분하지 않아 `SCREEN`으로 강제 변환돼도 무해했지만(오히려 옛 관례상 자연스러움), Python 엔진의 전역 객체는 `Screen.cpp`의 `AddObject(_T("Screen"), ...)`로 **정확히 대소문자를 구분해서** 등록되어 있어(`Screen`이지 `SCREEN`이 아님), 강제 대문자 변환이 실제로 `NameError: name 'SCREEN' is not defined`를 유발할 수 있는 진짜 버그였다(IB999988 BUTTON1 OnClick에서 `screen.send(0)` 실전 확인, 이후 대문자 강제변환 자체도 재현됨).
- 수정: `VBScriptEdit.cpp::IsSymbol()`에서 `m_ScriptStrMap` 매치 시, `m_bPythonMode`이면 대문자화 이전의 원래 입력 그대로 복원하도록 변경 — Python 모드에서는 사용자가 타이핑한 대소문자를 그대로 유지하면서 색상 강조(및 멤버 자동완성)만 적용됨. VBScript 모드는 기존 동작(강제 대문자화) 그대로 유지.
- **개선(2026-07-08, 후속)**: `m_ScriptStrMap`으로 매치되는 전역 엔진 객체는 `Screen`/`System`/`Login`/`Ledger` 딱 4개뿐이고, 이들의 정확한 등록 철자는 `ibks/Wizard/Screen.cpp`의 `AddObject()` 호출에 리터럴로 고정되어 있음(`AddObject(_T("Screen"), ...)` 등). 단순히 원본 타이핑을 복원하는 대신, Python 모드에서 이 4개에 한해 **정확한 철자로 강제 교정**하도록 `IsSymbol()`에 고정 매핑 테이블(`{"SCREEN","Screen"}, {"SYSTEM","System"}, {"LOGIN","Login"}, {"LEDGER","Ledger"}`)을 추가 — 대소문자를 아무렇게나 입력해도 폼 컨트롤 심볼처럼 정확한 이름으로 자동 교정됨. (폼에 정의된 일반 컨트롤 심볼은 이 4개와 별개로 `formItem.getFormCount()` 루프에서 이미 항상 정확한 등록 이름으로 복원되고 있었음 — 이번 변경 대상 아님.)

**5-1. COM 객체의 메서드/속성 이름(점 뒤)은 대소문자를 구분하지 않지만, 객체 이름 자체(점 앞)는 구분한다**
- `pythonEngine.cpp`의 `AxisObject_getattro()`/`AxisObject_setattro()`는 Python에서 받은 속성 이름 문자열을 그대로 `IDispatch::GetIDsOfNames()`에 넘긴다. COM 자동화(Automation)의 `GetIDsOfNames`는 관례적으로 대소문자를 구분하지 않으므로(VBScript와 동일한 관례), `Screen.SetMessage(x)`와 `Screen.setMessage(x)`는 동일하게 동작한다.
- **하지만 이건 점(`.`) 뒤의 메서드/속성 이름에만 해당한다.** `Screen`, `System`, `S1301` 같은 **전역 객체 이름 자체**는 `CPythonEngine::AddObject()`가 `PyDict_SetItemString(m_globals, names, ...)`로 등록한 이름(예: `ibks/Wizard/Screen.cpp`의 `m_vbe->AddObject(_T("Screen"), ...)` — 정확히 대문자 `Screen`)과 정확히 일치해야 하는 **일반 Python 전역변수 조회**라서 대소문자를 구분한다. `screen.send(0)`(소문자)는 `NameError: name 'screen' is not defined`가 난다(실전 확인, IB999988 BUTTON1 OnClick).
- 요약: **`Screen.SetMessage(x)` — 앞의 `Screen`은 정확히 맞춰야 하고, 뒤의 `SetMessage`는 대소문자 아무렇게나 써도 된다.**

**6. 임베디드 Python 실행 환경 (2026-07-08 확인, `pythonEngine.cpp` 기준)**
- Python 3.11 (32bit), 경로: `C:\Users\IBKS\AppData\Local\Programs\Python\Python311-32`
- `__builtins__`를 전체 그대로 노출(`PyDict_SetItemString(m_globals, "__builtins__", PyEval_GetBuiltins())`) — `len()`, `str()`, `int()`, `range()`, `list`, `dict` 등 표준 내장 함수/타입이 **전부 정상 사용 가능**한 진짜 CPython 인터프리터다 (VBScript 흉내가 아님).
- 스크립트는 `PyRun_String(src, Py_file_input, ...)`(모듈 단위 실행)으로 돌아가므로, **`import` 문도 실제로 동작**한다 (`import math`, `import re`, `import json` 등). 다만 이 환경에 어떤 패키지가 실제로 설치돼 있는지는 별도 확인 필요 — 표준 라이브러리 외 외부 패키지(`pip install`)는 미검증.
- 이벤트 핸들러별 인자 전달 규칙(`DoProcedure()` 참고, `procs` 이름으로 분기):
  - `AX_SUB_OnService_AX_(data, length)`, `AX_SUB_OnApprove_AX_(key, data, length)`, `AX_SUB_OnSelect_AX_(data)`, `AX_SUB_OnFile_AX_(upload, data, length)`, `AX_SUB_OnAlert_AX_(code, data, state)`, `AX_SUB_OnTimerX_AX_(id)` — VBScript 시절과 동일한 파라미터 순서/의미를 유지하도록 C++ 쪽에서 맞춰줌.
- VARIANT↔Python 타입 변환: `VT_BOOL↔bool`, `VT_I4/I2/I8↔int`, `VT_R4/R8↔float`, `VT_BSTR↔str`, `VT_EMPTY/VT_NULL↔None`, `VT_DISPATCH↔AxisObject`(COM 래퍼, 섹션 9-3 참고).

**7. VBScript → Python 문법 대응표**

일반 문법:

| VBScript | Python | 비고 |
|---|---|---|
| `' 주석` | `# 주석` | |
| `Dim x` | (생략, 그냥 `x = 값`) | Python은 변수 선언이 없음 |
| `If a Then ... ElseIf b Then ... Else ... End If` | `if a:\n    ...\nelif b:\n    ...\nelse:\n    ...` | 콜론(`:`)+들여쓰기 필수, `End If` 없음 |
| `For i = 1 To 10 Step 2 ... Next` | `for i in range(1, 11, 2):\n    ...` | `To 10`은 포함이지만 `range(1,11)`처럼 끝값+1 필요 |
| `For Each x In coll ... Next` | `for x in coll:\n    ...` | |
| `Do While cond ... Loop` / `While cond ... Wend` | `while cond:\n    ...` | |
| `Do Until cond ... Loop` | `while not cond:\n    ...` | |
| `Sub f(a) ... End Sub` | `def f(a):\n    ...` | 반환값 없는 함수 |
| `Function f(a) ... f = 결과 ... End Function` | `def f(a):\n    ...\n    return 결과` | `함수명 = 값` 대신 `return` |
| `a = b` (비교) | `a == b` | VBScript는 대입/비교 둘 다 `=`, Python은 `=`(대입) vs `==`(비교) 구분 |
| `a <> b` | `a != b` | |
| `Not a` / `a And b` / `a Or b` | `not a` / `a and b` / `a or b` | 소문자 |
| `True` / `False` | `True` / `False` | 철자 동일하지만 **Python은 대소문자 구분** — `true`/`false`는 오류 |
| `Null` / `Nothing` / `Empty` | `None` | 세 개념이 Python에서는 `None` 하나로 통일 |
| `s1 & s2` (문자열 연결) | `s1 + s2` 또는 `f"{s1}{s2}"` | |
| `On Error Resume Next` | `try:\n    ...\nexcept Exception:\n    pass` | |
| `Array(1, 2, 3)` | `[1, 2, 3]` | Python 리스트는 크기 자동 조절, `ReDim` 불필요 |
| `UBound(arr)` | `len(arr) - 1` | Python은 `len()`이 개수, VBScript `UBound`는 마지막 인덱스 |

문자열 함수 (⚠️ **VBScript는 1부터 시작하는 인덱스, Python은 0부터** — 변환 시 가장 흔한 실수 지점):

| VBScript | Python | 비고 |
|---|---|---|
| `Len(s)` | `len(s)` | |
| `Left(s, n)` | `s[:n]` | |
| `Right(s, n)` | `s[-n:]` | |
| `Mid(s, start, len)` | `s[start-1:start-1+len]` | VBScript `start`는 1부터 시작 → Python은 `start-1` |
| `Mid(s, start)` (길이 생략) | `s[start-1:]` | |
| `InStr(s, sub)` | `s.find(sub) + 1` (못 찾으면 Python은 `-1`, VBScript는 `0`) | 위치 값이 필요없고 포함 여부만 볼 땐 `sub in s` 권장 |
| `Replace(s, old, new)` | `s.replace(old, new)` | |
| `UCase(s)` / `LCase(s)` | `s.upper()` / `s.lower()` | |
| `Trim(s)` / `LTrim(s)` / `RTrim(s)` | `s.strip()` / `s.lstrip()` / `s.rstrip()` | |
| `CStr(x)` / `CInt(x)` / `CDbl(x)` | `str(x)` / `int(x)` / `float(x)` | |
| `MsgBox s` | (해당 없음) → `Screen.SetMessage(str(s))` 사용 | 섹션 9-4 참고, VBScript 내장 함수라 Python엔 없음 |

**8. 이 프로젝트 COM 연동 특유의 규칙 (재정리, 섹션 9-3과 동일 내용)**
- 속성 **읽기**: `obj.Prop()` — 반드시 괄호로 호출해야 실제 값. `obj.Prop`만 쓰면 `AxisMethod` 래퍼 객체가 나옴.
- 속성 **쓰기**: `obj.Prop = 값` — 괄호 없이 그대로 동작 (VBScript와 동일).
- 메서드 호출은 VBScript와 동일하게 `obj.Method(인자)` 형태.

**9. 실전 변환에서 실제로 만난 흔한 실수 사례 (`IB12010B`/`IB120100`, 2026-07-09~10)**

VBScript는 대소문자 무관 + `Variant` 자동 타입변환이라 대충 써도 동작하던 것들이, Python으로 옮기면 그대로 에러나 "조용한 오작동"으로 나타난 사례들. 변환 후 꼭 재확인할 체크리스트로 활용.

- **9-1. VBScript 함수/메서드 이름을 그대로 옮기면서 대소문자를 안 맞춤**
  - 예: `Replace(s, "0", "")` → `s.Replace("0", "")`(❌, VBScript 관성으로 대문자 R) 로 잘못 옮기면 `AttributeError: 'str' object has no attribute 'Replace'`. 올바른 표기는 `s.replace("0", "")`(소문자, 섹션 7 표 참고).
  - Python 표준 라이브러리 메서드는 전부 **소문자**(`.replace()`, `.strip()`, `.upper()`, `.find()` 등) — COM 객체의 메서드/속성(섹션 9-5-1)과 달리 대소문자를 반드시 정확히 맞춰야 함.
  - **함정**: `or`/`and`의 단락평가(short-circuit) 때문에, `if s.strip() == "" or s.Replace(...).strip() == 0:`처럼 잘못된 코드가 있어도 **왼쪽 조건이 먼저 참이 되는 동안은 에러가 안 나다가**, 나중에 `s`가 빈 문자열이 아니게 되는 순간(=오른쪽까지 평가되는 순간) 에러가 터진다. "처음엔 되다가 두 번째 호출부터 에러난다"는 증상이면 이 패턴을 의심할 것.

- **9-2. VBScript의 `Variant`는 숫자/문자열을 안 가리지만 Python은 타입을 구분함**
  - 예: `medoFee = 1`(VBScript, 정수 리터럴) → Python도 그대로 `medoFee = 1`로 옮기면, 이후 코드에서 `medoFee.strip()`처럼 **문자열 메서드를 호출하는 순간** `AttributeError: 'int' object has no attribute 'strip'`.
  - 원인: 해당 변수가 스크립트 나머지 부분에서 문자열로 취급된다면(`.strip()`, `+` 문자열 연결, `.Data =` 등), 애초에 대입할 때부터 **따옴표를 붙여 문자열로** 만들어야 함 (`medoFee = "1"`).
  - 변환 규칙: VBScript 원본에서 `x = 1`처럼 숫자 리터럴을 대입하는 줄을 보면, 그 변수가 이후 코드에서 문자열로도 쓰이는지(`Trim()`, `&` 연결, `.Data =` 등) 반드시 확인하고, 그렇다면 Python에서는 `x = "1"`로 옮길 것.

- **9-3. `a = b = c` 형태는 VBScript에서 체이닝 대입이 아니다**
  - `mesuFee = medoFee = medoTax = 0` 같은 줄은, VBScript에서 "3개 변수를 동시에 0으로 대입"하는 게 **아니라**, 맨 앞의 `=`만 대입이고 `medoFee = medoTax = 0`은 비교연산 체인(참/거짓)으로 해석됨 — 즉 원본 자체의 숨은 버그일 수 있다. Python은 반대로 `a = b = c = 0`이 정상적인 체이닝 대입이므로, 변환 시 원본의 "의도"(주석 등)를 보고 진짜 체이닝 대입으로 바꿀지, 원본 버그를 그대로 재현할지 사용자에게 확인이 필요하다.

- **9-4. 크로스맵 프로시저 이름은 호출부 문자열과 대소문자까지 정확히 일치해야 한다**
  - `SCREEN.Proc "다른맵.Send_f", ""`처럼 다른 맵의 프로시저를 이름(문자열)으로 호출하는 코드가 있으면, Python 쪽 함수 정의도 그 문자열과 **대소문자까지 정확히** 같아야 한다(`def Send_f():`, `def send_f():`가 아님).
  - VBScript는 대소문자 무관이라 원본에서는 전혀 문제가 없었지만, Python은 함수 이름을 정확한 문자열로 dict에서 찾기 때문에 하나라도 다르면 `getFunction()`이 실패한다.
  - **가장 위험한 점**: `CPythonEngine::DoProcedure(procs, data, count)`는 대상 함수를 못 찾으면 에러를 내지 않고 `return true`(VBS 동작 흉내)로 **조용히 성공한 것처럼 넘어간다.** 그래서 증상이 "에러도 없이 그냥 반응이 없다"로만 보인다 — 변환한 맵이 다른 맵에서 이름으로 호출되는 진입점(OnClear, Send_f 등)이 있다면, 호출하는 쪽 원본 VBScript의 정확한 철자를 먼저 확인할 것.
  - 상세: `docs/Todo.md` "크로스맵 Screen.Proc 호출은 대소문자를 정확히 맞춰야 함" 항목, 메모리 `project_python_engine_progress.md` 2026-07-10 기록 참고.
