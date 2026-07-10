# Todo

- 생성일: 2026-07-07
- 목적: src_7_11 프로젝트에서 남은 작업, 확인 필요 항목을 추적한다.

## TODO

### 완료 (2026-07-08)

- [x] **awWcc 리빌드 → 맵 재컴파일 → HTS Python OnStart 테스트** — 순수 Python 화면 기준 End-to-End 검증 성공 (`Screen.SetMessage("Python OnStart OK")` 정상 실행). 혼합 언어 화면은 여전히 실패(예상된 동작, 아래 Mixed-Language 항목 참고). 상세: KnowledgeBase.md 섹션 7.

### 진행 중 — AxisBuilder Python 지원 안정화 (2026-07-08, 계획: purrfect-wandering-blossom.md)

추측 방식(def/import 텍스트 스캔) → 명시적 pythonMode 플래그 방식으로 전환하는 작업. 상세: KnowledgeBase.md 섹션 8.

- [x] Phase 1: `_mapH` 구조체(5개 파일 + ibks/h)에 `pythonMode` 필드 추가, 기존 배포 .map 파일과 바이너리 호환성 검증
- [x] Phase 3: awWcc 컴파일러(Maptbl.hxx, mapbld.cpp)를 `pythonMode` 명시적 플래그 기준으로 교체
- [x] Phase 2(일부): awBuild/build.cpp에 `PYTHON(%d)` 지시어 직렬화 추가 — 기존에 존재하던 에디터 측 자동감지(mapLoad.cpp)와 컴파일러를 연결하는 누락된 고리였음
- [x] Phase 3 테스트: 순수 Python 화면(`D:\src\IBKS\기획\axibuilder python\new builder src\IB999987`) 재컴파일 성공 확인 (Testpro/OnClick/OnChange 전부 Python 전환)
- [x] Phase 4: 런타임(`ibks/Wizard/Screen.cpp`, `engineWrapper.cpp`)이 컴파일된 `pythonMode`를 읽어 엔진을 선택하도록 변경 완료 (`LoadScript(scripts, scpKind)` 신규 파라미터, `scpKind=-1`은 레거시 텍스트 스캔 fallback으로 유지). DebugView++ `[WIZARD][ENGINE][DEBUG]` 로그로 `source=explicit pythonMode` 확인.
- [x] Phase 2(나머지): 에디터 UI에 PY 토글 버튼 추가 — 최초 계획은 `awDlg/mapScript.h/cpp`였으나, 실제 활성 에디터가 `platform/builder/ScriptBar.h/cpp`(`CScriptBar`, `axisWork.vcxproj` 소속)임을 확인해 이쪽에 구현. `CVBScriptEdit`에 Python 키워드/주석문자(`#`) 구문강조 추가(`keywords.h`, `VBScriptEdit.h/cpp`). "Python 에디터 Auto-Capitalize 이슈"는 이 작업으로 해결됨.
- [x] Phase 5: 회귀 테스트(기존 VBScript 화면 2건 재컴파일) + Python 화면(PY 버튼 상태로 재컴파일) 최종 검증 — 3개 화면 모두 DebugView++ 로그에서 `source=explicit pythonMode`로 정확히 구분됨 확인 (2026-07-08).
- [ ] Phase 6: 문서 정리(진행 중), `OutputDebugString` 제거/게이팅 항목과 통합 검토

### 완료 (2026-07-08, Phase 6 이후 추가 개선)

- [x] **이벤트 박스 자동 def 래핑** — `awWcc/mapbld.cpp::mapResStrBuilding()`이 Python 모드에서도 VBScript처럼 `def AX_..._AX_():` 헤더와 본문 들여쓰기를 자동 생성하도록 개선. 이벤트 박스에 본문만 쓰면 됨. 상세: KnowledgeBase.md 섹션 8.
  - **재작업 필요**: 이 변경 이전에 수동으로 `def AX_..._AX_():`를 직접 쓴 테스트 화면(OnStart 테스트 맵 등)은 그 줄을 지우고 본문만 남긴 뒤 재컴파일해야 함. `_declaration_` 섹션(전역 함수)은 영향 없음(계속 직접 `def 함수명():` 작성).
- [x] **빈/주석뿐인 이벤트 박스 자동 `pass` 삽입** — 실전 화면(IB999988, 실시간잔고) 변환 중 발견: VBScript 줄을 `#`로 주석만 처리하고 아직 Python 코드가 없는 박스가 있으면 `IndentationError: expected an indented block` 발생(화면 전체가 한 Python 모듈로 컴파일되므로 박스 하나만 이래도 전체 컴파일 실패). `mapbld.cpp` case 17(ONENDSUB)이 Python 모드 블록 끝에 `    pass`를 항상 자동으로 붙이도록 수정. 완전히 빈 블록(내용 자체가 없는 경우)의 `def` 헤더 생성 누락 버그도 함께 수정. 상세: KnowledgeBase.md 섹션 8.
- [x] **`#load` include 지시어는 Python 모드와 무관 (조사만, 코드 변경 없음)** — `Sub AX_SUB_include_AX_() ... End Sub` 래핑은 컴파일러가 만드는 실행 코드가 아니라, Wizard 런타임에서 `CAxisForm::getRoutine()`(공유 모듈 `ibks/dll/form/axform.cpp`)이 스크립트 본문에서 통째로 잘라내 별도 처리하는 마커였음 — Python 엔진에 도달하기 전에 제거되므로 손댈 필요 없음.
- [x] **Python 에러 메시지에 실제 소스 줄 표시** — `pythonEngine.cpp::fetchError()`가 `SyntaxError`의 `.text` 속성(문제가 된 소스 줄)을 꺼내 메시지 박스에 같이 표시하도록 개선(`ibks/dll/vbs`, `src_7_11/platform/dll/vbs` 동일 적용, axisvbs 리빌드 필요).
- [x] **`#load` Python용 대체 문법 `@load` 추가** — `CAxisForm::getExternalScript()`(`ibks/dll/form/axform.cpp`, `src_7_11/platform/dll/form/axform.cpp`, `axisform.vcxproj`)가 `@LOAD `도 인식하도록 수정, 기존 `#load` 하위호환 유지. 상세: KnowledgeBase.md 섹션 5--2.
- [ ] **공용 라이브러리 맵(`IB999901` 등)의 Python 전환** — `#load`/`@load`로 재사용되는 라이브러리 맵은 그 자체도 Python으로 바꿔야 이를 참조하는 화면이 완전히 동작함. `IB999987`이 `IB999901`의 `SetMKGubn777()`을 호출하는데 현재 `#load` 제거 상태라 `NameError` 예상됨 — 다음 작업 후보. 상세: KnowledgeBase.md 섹션 5--3.
- [x] **(변환 가이드, 버그 아님) 크로스맵 `Screen.Proc("맵.함수명", data)` 호출은 대소문자를 정확히 맞춰야 함** (2026-07-10, `IB12010B`/`IB120100` 사례) — 부모 맵 `IB120100`이 `SCREEN.Proc "objX.Send_f", ""`(대문자 S)로 자식 맵의 프로시저를 호출하는데, 자식 맵(`IB12010B`)의 Python 함수는 원본 VBScript 그대로 `def send_f():`(소문자 s)로 변환돼 있어서 이름이 안 맞았음. VBScript는 대소문자 구분이 없어 항상 같은 Sub를 찾았지만, `CPythonEngine::DoProcedure(procs, data, count)`는 `getFunction()`이 실패하면 `if (!func) return true; // match VBS behavior`로 **에러 없이 조용히 아무 일도 안 하고 성공한 것처럼 넘어가서** 증상이 "에러도 없이 그냥 안 됨"으로만 보였음. 해결: 자식 맵의 함수명을 호출하는 쪽 문자열(`Send_f`)과 정확히 맞춤. **앞으로 크로스맵 프로시저를 Python으로 옮길 때는 호출부(`Screen.Proc`/`#load`/`@load`로 참조하는 다른 맵)의 문자열 대소문자를 먼저 확인하고 그대로 함수명에 반영할 것.** 같은 파일에 `SCREEN.Proc "main.Change_MKGubn_Main", cxMarket.sMarket`도 있어 그 맵(`main`) 변환 시 동일하게 주의 필요.
- [x] **에디터의 전역 객체 이름 자동 대문자 강제변환 버그 수정** — `VBScriptEdit.cpp::IsSymbol()`이 `PUBLIC.INI`에 대문자로 등록된 전역 객체(SCREEN, SYSTEM 등)를 Python 모드에서도 강제로 전부 대문자화해서 `Screen`→`SCREEN`처럼 바꿔버려 `NameError` 유발하던 문제. Python 모드에서는 원래 타이핑한 대소문자를 유지하도록 수정. 상세: KnowledgeBase.md 섹션 9-5-0.
- [x] **Procedures(라이브러리) 맵 에디터의 pythonMode 미적용 버그 수정** (2026-07-09) — `MAPKIND(PROCEDURES)` 맵(예: `IB999999` 테스트 라이브러리)은 `ScriptBar`의 편집기 대신 `ChildView.cpp::changeKind()`가 별도의 `CVBScriptEdit` 인스턴스(`m_pSCEdit`)를 생성하는데, `SetPythonMode()` 호출이 누락되어 있었음. 1차로 `changeKind()`(`ChildView.cpp:3819`)에 추가했으나 이 시점은 `CChildFrame::OnCreate()`에서 `m_mapH`가 `ZeroMemory`된 직후라 `pythonMode`가 아직 파일에서 로드되기 전(`false`)이라 효과 없었음. 실제 맵 로딩 후 갱신되는 `ID_USR_REFRESHCTRL` 핸들러(`MainFrm.cpp:3616`, `ScriptBar`도 이 시점에 동일하게 재적용)에도 `m_pSCEdit->SetPythonMode()` 재호출을 추가해 해결. `Screen` 등 전역객체 대소문자 자동교정도 Procedures 맵에서 정상 동작 확인.
- [x] **Python 스크립트에 한글 포함 시 "지정되지 않은 오류"(COM 예외) 발생 수정** (2026-07-09) — `pythonEngine.cpp::LoadScript()`(`ibks/dll/vbs`, `src_7_11/platform/dll/vbs` 양쪽)가 컴파일된 스크립트 원문(CP949/ANSI)을 변환 없이 그대로 `PyRun_String()`에 전달하고 있었음. `PyRun_String`은 소스를 UTF-8로 가정하므로 한글 포함 스크립트는 파싱 단계에서 깨짐(같은 파일의 `ansiToPy`/`cstrToPy`는 이미 `MultiByteToWideChar(CP_ACP,...)`로 정확히 처리 중이었는데 `LoadScript()`만 누락). 새 헬퍼 `ansiToUtf8()`(CP949→wide→UTF-8) 추가로 해결. 빌드 시 `windows.h`의 `max` 매크로가 `std::max(...)`를 깨뜨려(`std::` + 매크로 텍스트치환으로 구문오류) 컴파일 에러 발생 → `(std::max)(...)` 형태로 우회. axisvbs.dll 재빌드 후 사용자 확인 완료.
- [ ] **`AxisMethod_call()` COM Invoke 실패 시 예외 미전파** (2026-07-09 발견, 미수정) — `pythonEngine.cpp`의 `AxisMethod_call()`(`Screen.SetMessage()` 등 COM 메서드 호출부)이 `IDispatch::Invoke()` 실패 시 Python 예외를 세팅하지 않고 조용히 `None`을 반환함. 나중에 COM 호출 실패의 원인 파악이 어려울 수 있음 — 우선순위 낮음, 발견만 기록.
- [x] **`Screen.Send(0)`이 버튼 OnClick 안에서 호출되면 `OnSend()`가 실행되지 않는 버그 수정** (2026-07-09) — `CEngineWrapper::IsAvailable()`(`ibks/dll/vbs`, `src_7_11/platform/dll/vbs` 양쪽 `engineWrapper.cpp`)에 Python 모드에서만 걸려있던 재진입 가드(`if (m_py->m_invoke) return false;`)가 원인. `BUTTON0.OnClick` 자체가 이미 Python 프로시저 실행 중(`m_invoke=true`)인 상태에서 그 안에서 `Screen.Send(0)`을 호출하면, 호출 체인(`_Send()`→`InStream()`→`CScreen::OnTRAN()`→`CScript::OnSend()`) 끝의 `IsAvailable("AX_SUB_OnSend_AX_")`가 무조건 `false`를 반환해 OnSend 이벤트박스가 없는 것처럼 처리되어 버림. VBScript 쪽(`m_vbs`)엔 이런 가드가 애초에 없었음(비대칭). CPython은 같은 스레드에서의 중첩 호출(재귀적 `PyObject_Call`)을 안전하게 지원하므로 가드 제거. `Send(0)` 안에서 `OnSend()`를 우회해서 버튼 클릭 핸들러에 로직을 직접 넣는 임시 우회가 더 이상 필요 없음.
- [x] **`Screen.Proc("맵.프로시저", "")`(인자 없는 크로스맵 프로시저 호출)이 Python에서 "takes 0 positional arguments but 1 was given" 에러 발생** (2026-07-09) — `CPythonEngine::DoProcedure(CString procs, CString data, int count)`(양쪽 트리 `pythonEngine.cpp`)의 콤마 분리 루프가 `data=""`이어도 무조건 `arrs`에 빈 문자열 1개를 추가해버려(`for(;;) { ... } else { arrs.Add(tmp); break; }`), 인자 0개로 호출돼야 할 프로시저가 인자 1개(빈 문자열)로 호출됨. VBScript 쪽(`scriptEngine.cpp:284`, `for ( ; !data.IsEmpty(); )`)은 반복 조건 자체에 빈 문자열 체크가 있어 `data=""`이면 인자 0개로 정확히 처리 — 이 차이 때문에 VBScript에선 문제없던 `SCREEN.Proc "objX.OnClear", ""` 같은 호출이 Python 변환 후 깨짐. 동일하게 `for ( ; !tmp.IsEmpty(); )`로 루프 조건을 바꿔서 해결.

### 완료 (2026-07-10)

- [x] **`fetchError()`에 런타임 에러 전체 트레이스백(줄번호) 표시 추가** — `SyntaxError`는 `.text` 속성으로 문제 줄을 보여줬지만(2026-07-09), `AttributeError`/`TypeError` 등 일반 런타임 예외는 줄번호 정보가 전혀 표시되지 않아 원인 파악이 어려웠음. `pythonEngine.cpp::fetchError()`(`ibks/dll/vbs`, `src_7_11/platform/dll/vbs` 양쪽)에 Python `traceback` 모듈(`traceback.format_exception()`)로 전체 콜스택을 포맷해 메시지에 덧붙이는 로직 추가. 이 기능으로 `IB12010B`의 `InitOption()` 내부 두 번째 `'int' object has no attribute 'strip'` 버그 위치를 바로 특정함.
- [x] **`IB12010B` `InitOption()`의 `medoFee`/`medoTax` int/str 버그 (2번째 사례)** — `if feeGubn == 1: medoFee = 1; medoTax = 1`에서 정수 리터럴을 그대로 대입(VBScript Variant 원본을 직역), 바로 아래 `medoFee.strip()`/`medoTax.strip()` 호출에서 `AttributeError` 발생. `medoFee = "1"` / `medoTax = "1"`로 문자열화하여 수정(사용자가 직접 수정, 2026-07-10). `medoFee=1`류 버그가 `InitOption()` 안에 최소 2곳 있었던 사례 — 문자열로 쓰이는 전역변수에 정수 리터럴을 대입하는 다른 VBScript 원본 코드가 더 있는지 재확인 권장.
- [x] **Python 모드 스크립트 편집기에서 Tab 키가 탭 문자 대신 스페이스 4칸을 삽입하도록 수정** — `CCrystalEditView::OnEditTab()`(베이스 클래스)이 Tab 키 입력 시 항상 리터럴 탭 문자(`_T("\t")`)를 삽입하도록 하드코딩되어 있어, Python은 들여쓰기가 문법이라 탭/스페이스가 섞이면 `TabError: inconsistent use of tabs and spaces`가 발생함(VBScript는 들여쓰기를 아예 안 보므로 기존엔 문제없었음). `CVBScriptEdit::OnEditTab()`(`VBScriptEdit.h/cpp`)를 새로 오버라이드해 `m_bPythonMode`일 때만 스페이스 4칸을 삽입하도록 분기 추가, `m_bPythonMode`가 false(VBScript 모드)이거나 오버라이트 모드일 때는 기존 `CCrystalEditView::OnEditTab()`을 그대로 호출해 기존 VBScript 화면 편집에는 영향 없음. `m_bOvrMode`/`DeleteCurrentSelection()`이 베이스 클래스 private 멤버라 직접 접근 불가 → `GetOverwriteMode()`/`ReplaceSelection()` public 메서드로 대체.

### 즉시 수행 대상

- [ ] **OutputDebugString 진단 제거/게이팅 (2026-07-07 미루기)** — `[WIZARD][SCRIPT][DEBUG]`, `[WIZARD][PYTHON][DEBUG]`, `[AWWCC][DEBUG]`, `[WIZARD][ENGINE][DEBUG]`(2026-07-08 Phase 4 추가, 엔진 선택 근거 확인용) 태그 추적이 근본 원인 분석에 필수였으나, Python 기능 안정화 후 제거 또는 compile-time debug 플래그 뒤로 이동할 것.
  - 파일: src_7_11/platform/builder/awWcc/libWcc.cpp, mapbld.cpp, misc.cpp; src_7_11/platform/dll/vbs/scriptEngine.cpp, pythonEngine.cpp, engineWrapper.cpp; ibks/dll/vbs/engineWrapper.cpp; ibks/Wizard/Screen.cpp; src_7_11/platform/builder/ScriptBar.cpp(`[AXISWORK][AUTOLIST][DEBUG]`, 2026-07-08 추가, public.ini 로딩 진단용); src_7_11/platform/builder/MainFrm.cpp(`[AXISWORK][ROOTDIR][DEBUG]`, 2026-07-08 추가, RootDir 레지스트리/ini 분기 진단용 — 원인 규명 후에도 당분간 유지 권장, workshop.ini PROJECTNAME 설정에 따라 동작이 크게 갈리므로).
  - 위험도: 낮음 (단순 로그, 기능 영향 없음, 개발 빌드만 출력).

### 미래 개선 (우선순위: 중)

- [ ] **CCrystalEdit 한글(DBCS) 커서 위치 오차** (2026-07-09 발견) — 스크립트 편집창(`CVBScriptEdit`/`CCrystalTextView`)에서 한글을 입력하면 커서가 실제 글자 위치보다 오른쪽으로 밀림. Python 작업과 무관한 기존 레거시 컴포넌트 버그로 추정.
  - 관련 코드: `CCrystalTextView.cpp` `TextToClient()`(1766행, 바이트수×`GetCharWidth()`로 픽셀 위치 계산 — `GetCharWidth()`는 영문 "X" 폭 기준), `ClientToText()`(1691행), DBCS 리드바이트 경계보정(1729행); `CCrystalEditView.cpp` `OnChar()`(269행, 타이핑 시 1바이트씩 삽입).
  - 원인 후보: 한글 글리프의 실제 렌더링 폭이 영문폭의 정확히 2배가 아니거나, 리드/트레일바이트 경계보정 로직이 타이핑 중에도 개입하며 오차 누적.
  - 사용자 판단(2026-07-09): 지금은 빌더의 DLL 컨트롤 사용 기능과 복잡한 대표화면 확인을 우선 진행, 이 이슈는 보류하되 **반드시 해결 필요** — 나중에 시간을 들여서라도 고칠 것. 부분 수정보다 **최신 에디트 컨트롤 클래스로 교체**하는 방안도 함께 검토(레거시 CCrystalEdit이 애초에 DBCS를 안전하게 지원하도록 설계되지 않았을 가능성).

- [ ] **Per-Event-Block Engine Dispatch (혼합 언어 지원)** — 현재는 단일 맵 내 전체 블록을 한 엔진으로 처리.
  - 목표: 같은 화면 내 일부 섹션은 VBScript, 일부는 Python.
  - 접근: Event Marker 경계마다 언어 감지, 각 블록을 별도로 로드/실행.
  - 영향: CEngineWrapper, CScriptEngine, CPythonEngine 인터페이스 재설계.
  - 우선순위: "필요할 때" (현재 단일 언어 제약으로 문제 보고된 적 없음).

### 장기 계획 (우선순위: 낮음)

- [ ] **이원화 소스 트리 통합 계획** — ibks/dll/vbs을 마스터로 지정(2026-07-07), src_7_11/platform/dll/vbs는 동기화 상태.
  - 미결: axislib, axisform도 이 원칙을 따를지? (vbs만 명시됨).
  - 장기 목표: 중복 제거, 단일 truth source.
