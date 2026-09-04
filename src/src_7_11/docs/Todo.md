---
project: src_7_11
category: todo
status: 작성됨
updated: 2026-08-21
---

# Todo


## 목차

- [TODO](#todo)
  - [완료 (2026-07-08)](#완료-2026-07-08)
  - [진행 중 — AxisBuilder Python 지원 안정화 (2026-07-08, 계획: purrfect-wandering-blossom.md)](#진행-중-axisbuilder-python-지원-안정화-2026-07-08-계획-purrfect-wandering-blossommd)
  - [완료 (2026-07-08, Phase 6 이후 추가 개선)](#완료-2026-07-08-phase-6-이후-추가-개선)
  - [완료 (2026-07-13)](#완료-2026-07-13)
  - [완료 (2026-07-14)](#완료-2026-07-14)
  - [완료 (2026-07-10)](#완료-2026-07-10)
  - [즉시 수행 대상](#즉시-수행-대상)
  - [미래 개선 (우선순위: 중)](#미래-개선-우선순위-중)
  - [장기 계획 (우선순위: 낮음)](#장기-계획-우선순위-낮음)

---

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

### 완료 (2026-07-13)

- [x] **`OnTimerX(id)` "Go to Line"(Ctrl+G) 이동 버그 수정 (주말 노클샘 작업, 대고객 기능)** — `ScriptBar::getAllScript()`가 각 이벤트 블록에 `idx` 상수를 부여하고 `MainFrm.cpp`가 `idx/13`, `abs(idx%13)+1`로 디코딩하는 13진법 스킴을 쓰는데, `OnTimerX`에 배정된 `idx=-26`이 `Declaration`이 이미 쓰던 값과 중복됨 + `OnTimerX`의 이벤트 코드(`35`)는 13진법 나머지(최대 13)로 표현 자체가 불가능해서 어떤 값을 넣어도 정확히 찾아갈 수 없었음. 40진법으로 확장해 `getAllScript()`의 idx 상수 16개(`-13~-27` → `-40~-52`, `-74`, `-80`, `-81`) 전부 재계산 + `MainFrm.cpp:878`의 디코딩 공식(`/13`,`%13`→`/40`,`%40`) 변경. 같은 함수의 컨트롤별 스크립트 인코딩(`idx = ii*13+N`)도 같은 디코더를 공유하므로 `ii*40+N`으로 함께 수정(빠뜨리면 컨트롤별 스크립트 이동이 깨짐). 추가로, 요청한 줄 번호가 어떤 블록에도 안 걸리면 `idx`가 기본값 `-3`으로 남아 `setLinePos()`가 엉뚱한 컨트롤 인덱스로 해석해 `ERROR_INVALID_PARAMETER`("매개 변수가 틀립니다") 메시지 박스가 뜨던 문제도 발견 — `ID_EDIT_GOTO` 핸들러에서 요청 줄 번호를 전체 스크립트 줄 수 범위로 클램프하도록 수정(범위 밖이면 가장 가까운 처음/끝 줄로 이동). `getAllScript()`/`setLinePos()`는 이 핸들러 한 곳에서만 쓰이는 것을 전수 확인(사이드 이펙트 없음).
- [x] **스크립트 에디터에 줄 번호 표시 추가** — `CCrystalTextView::DrawMargin()`/`GetMarginWidth()`(`Components/crysedit_src/CCrystalTextView.cpp`)에 줄 번호 텍스트 렌더링 추가. 기존엔 북마크/브레이크포인트 아이콘만 그리던 좌측 여백에 줄 번호를 우측 정렬로 함께 표시. 여백 폭을 `20 + GetCharWidth()*6`(최대 6자리)으로 확장. `CVBScriptEdit`가 이 컴포넌트의 유일한 파생 클래스라 스크립트 에디터 전용으로 영향 범위 한정됨(다른 뷰어 없음, 확인됨). `SetTimerX`의 "Go to Line" 줄 번호 충돌 버그(아래 항목)를 사용자가 직접 재현/검증하기 위해 먼저 추가.
- [x] **"Go to Line" 클램프 이후에도 남아있던 `ERROR_INVALID_PARAMETER` 재발/불규칙 재현 버그 근본 수정** — 위 40진법 클램프 수정 이후에도 메시지 박스가 사라지지 않고 오히려 라인 번호에 따라 들쭉날쭉하게 재현됨. 원인은 `ScriptBar::getLineCount(CString str)`가 **빈 줄을 세지 않는** 로직이었던 것 — `getAllScript()`가 이벤트 블록의 경계(어느 라인이 어느 이벤트에 속하는지)를 판정할 때 전부 이 함수를 기준으로 계산하는데, `MainFrm.cpp`의 `ID_EDIT_GOTO`가 넘기는 `dlg.m_lineNum`은 에디터 화면에 실제로 보이는 물리적 라인 번호(빈 줄 포함)라 단위가 서로 달랐음. 스크립트에 빈 줄이 하나도 없으면 두 값이 우연히 일치해 정상 동작하고, 빈 줄이 있으면 그 지점부터 어긋나 엉뚱한 블록/라인으로 계산되어 `setLinePos()`가 잘못된 위치를 다루면서 에러 발생 — 라인 번호가 아니라 "그 라인 이전에 빈 줄이 몇 개 있었는가"에 따라 실패 여부가 달라졌던 것. `getLineCount()`를 빈 줄도 포함해 순수하게 `\n` 개수+1로 세도록 수정. 이 함수는 `getAllScript()` 내부 16곳과 `MainFrm.cpp`의 클램프 계산 1곳, 총 2곳에서만 쓰이는 것을 grep으로 전수 확인(다른 부작용 없음). 사용자 확인 완료: "잘 되는거 같습니다 이제 에러 박스가 사라졌내요."

### 완료 (2026-07-14)

- [x] **Procedures(라이브러리, `#load`/`@load`용) 맵에 Python/VBScript 전환 버튼 추가** — 일반 Map은 `ScriptBar`의 `m_pythonBtn`으로 `pythonMode`를 직접 켤 수 있는데, Procedures 맵은 `ScriptBar`를 안 쓰고 `ChildView`가 별도의 `CVBScriptEdit`(`m_pSCEdit`)를 직접 띄우는 구조라 이런 토글 버튼 자체가 없었음. 게다가 저장 경로도 달라서(`CChildFrame::SaveProcedures()`가 `CreateFile`/`WriteFile`로 직접 씀) 유일한 자동감지 로직(`CMapLoad::SaveScriptFile()`의 `def `/`import ` 스캔)이 이 경로에서는 전혀 실행되지 않고, 별도 프로세스인 `awWcc.exe`가 컴파일 시점에만 사후적으로 감지함 — 그 결과 Procedures 맵에 Python 코드를 처음 입력하는 시점엔 `pythonMode`가 항상 `false`라 주석기호/키워드/전역객체 대소문자 자동교정이 전부 VBScript 기준으로 잘못 동작했고, 딱 한 번 컴파일 후 맵을 닫았다 다시 열어야만(디스크에 저장된 `pythonMode=true`를 재로드) 정상화되던 구조적 문제였음(2026-07-09 IB999999 테스트에서 "재빌드+재시작 후 확인"으로 우회했던 게 이 문제였음). `ChildView`에 `CNFBtn m_pyBtn`을 신규 추가(`ID_CV_PYBTN`)해서 `ScriptBar`의 PY/VB 버튼과 동일하게 클릭 시 `m_mapH->pythonMode`를 즉시 토글 + `m_pSCEdit->SetPythonMode()` 즉시 반영하도록 함(`ChildView.cpp::changeKind()`에서 생성, `OnPyBtnClick()` 신규, `OnSize()`에서 상단 20px를 버튼 자리로 확보). `MainFrm.cpp`의 `ID_USR_REFRESHCTRL`(맵 리로드 시 재동기화 지점)에도 버튼 캡션/체크 동기화 추가. 컴파일러(`awWcc/mapbld.cpp`)의 Declaration 처리(case 18 `ONDECLARE`)는 애초에 `pythonMode`와 무관하게 원문 그대로 통과시키고, `@load` 병합 로직(`axform.cpp::getExternalScript()`/`getScripts()`)도 컴파일된 텍스트를 그대로 복사하는 방식이라 컴파일 결과물 자체는 문제없었음 — 문제는 순전히 에디터 UI에 Python 모드를 켤 방법이 없었다는 점. 사용자 확인 완료: "childview 에 토글버튼이 잘 들어갔습니다."

### 완료 (2026-07-10)

- [x] **`fetchError()`에 런타임 에러 전체 트레이스백(줄번호) 표시 추가** — `SyntaxError`는 `.text` 속성으로 문제 줄을 보여줬지만(2026-07-09), `AttributeError`/`TypeError` 등 일반 런타임 예외는 줄번호 정보가 전혀 표시되지 않아 원인 파악이 어려웠음. `pythonEngine.cpp::fetchError()`(`ibks/dll/vbs`, `src_7_11/platform/dll/vbs` 양쪽)에 Python `traceback` 모듈(`traceback.format_exception()`)로 전체 콜스택을 포맷해 메시지에 덧붙이는 로직 추가. 이 기능으로 `IB12010B`의 `InitOption()` 내부 두 번째 `'int' object has no attribute 'strip'` 버그 위치를 바로 특정함.
- [x] **`IB12010B` `InitOption()`의 `medoFee`/`medoTax` int/str 버그 (2번째 사례)** — `if feeGubn == 1: medoFee = 1; medoTax = 1`에서 정수 리터럴을 그대로 대입(VBScript Variant 원본을 직역), 바로 아래 `medoFee.strip()`/`medoTax.strip()` 호출에서 `AttributeError` 발생. `medoFee = "1"` / `medoTax = "1"`로 문자열화하여 수정(사용자가 직접 수정, 2026-07-10). `medoFee=1`류 버그가 `InitOption()` 안에 최소 2곳 있었던 사례 — 문자열로 쓰이는 전역변수에 정수 리터럴을 대입하는 다른 VBScript 원본 코드가 더 있는지 재확인 권장.
- [x] **Python 모드 스크립트 편집기에서 Tab 키가 탭 문자 대신 스페이스 4칸을 삽입하도록 수정** — `CCrystalEditView::OnEditTab()`(베이스 클래스)이 Tab 키 입력 시 항상 리터럴 탭 문자(`_T("\t")`)를 삽입하도록 하드코딩되어 있어, Python은 들여쓰기가 문법이라 탭/스페이스가 섞이면 `TabError: inconsistent use of tabs and spaces`가 발생함(VBScript는 들여쓰기를 아예 안 보므로 기존엔 문제없었음). `CVBScriptEdit::OnEditTab()`(`VBScriptEdit.h/cpp`)를 새로 오버라이드해 `m_bPythonMode`일 때만 스페이스 4칸을 삽입하도록 분기 추가, `m_bPythonMode`가 false(VBScript 모드)이거나 오버라이트 모드일 때는 기존 `CCrystalEditView::OnEditTab()`을 그대로 호출해 기존 VBScript 화면 편집에는 영향 없음. `m_bOvrMode`/`DeleteCurrentSelection()`이 베이스 클래스 private 멤버라 직접 접근 불가 → `GetOverwriteMode()`/`ReplaceSelection()` public 메서드로 대체.
- [x] **View 메뉴 "Edit Source"에 `Alt+F1` 단축키 추가** — `axisWork.rc` 액셀러레이터 테이블에 `VK_F1, ID_VIEW_SOURCE, VIRTKEY, ALT` 추가, 메뉴 텍스트도 `Edit Source\tAlt+F1`로 갱신. 기존 `ID_VIEW_SOURCE` 핸들러를 그대로 재사용.
- [x] **`IB120100`+`IB12010B`(부모+임베디드) 반복 open/close 시 `AxisObject_dealloc`에서 크래시 (재현 가능, 확정 수정)** — 스크립트의 모든 `def` 함수가 `__globals__`로 `m_globals` 딕셔너리를 참조하고 그 딕셔너리도 함수들을 값으로 담아 순환 참조가 형성됨. `CPythonEngine::UnloadScript()`의 `Py_DECREF(m_globals)` 한 번만으로는 refcount가 0이 안 되어(cyclic GC가 나중에 수거해야 함), `~CScreen()`이 바로 다음 줄에서 `delete m_xscreen`(Screen 객체 실제 구현체)을 실행해버림. Python 순환 GC가 나중에(다음 스크립트 로드 시 등) 뒤늦게 그 죽은 순환구조를 수거하면서, 이미 `delete`된 `m_xscreen`을 가리키는 댕글링 `IDispatch*`(`AxisObject::m_dispatch`)에 `Release()`를 호출해 힙이 깨지고 크래시(`SOFTWARE_NX_FAULT`, 가비지 주소로 점프). 반복적인 화면 열기/닫기로 GC 트리거 타이밍과 겹쳐야 재현되어 간헐적이었음. 수정: `CPythonEngine::Initialize()`/`UnloadScript()`(`ibks/dll/vbs`, `src_7_11/platform/dll/vbs` 양쪽)에서 `Py_DECREF(m_globals)` 직후 `PyGC_Collect()`를 강제 호출 — `m_xscreen`/`m_ledger` 등이 삭제되기 전에 순환 참조를 즉시 끊도록 순서 고정. 사용자가 반복 테스트로 재현 안 됨 확인.

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
