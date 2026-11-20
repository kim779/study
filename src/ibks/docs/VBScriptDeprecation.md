---
project: ibks
category: issue
status: 진행중
updated: 2026-11-20
---

# MS VBScript 지원 중단 대응

## 목차

- [1. 요약](#1-요약)
- [2. MS 일정](#2-ms-일정)
- [3. HTS 의존 지점](#3-hts-의존-지점)
- [4. 엔진 생성 실패 시 동작 (실측)](#4-엔진-생성-실패-시-동작-실측)
- [5. 적용한 수정](#5-적용한-수정)
- [6. 현장 사례와 조치](#6-현장-사례와-조치)
  - [6.1 Win11 고객 PC: RegExp만 생성 안 됨 (2026-11-20)](#61-win11-고객-pc-regexp만-생성-안-됨-2026-11-20)
  - [6.2 PC 쪽 조치 순서](#62-pc-쪽-조치-순서)
  - [6.3 RegExpRepair.bat](#63-regexprepairbat)
- [7. 검토한 대안](#7-검토한-대안)
- [8. 남은 과제](#8-남은-과제)
- [9. 변경 이력](#9-변경-이력)
- [관련 문서](#관련-문서)

---

## 1. 요약

- Windows 11부터 VBScript(`vbscript.dll`)가 단계적으로 꺼지고, 나중에는 제거됩니다. 2027년 무렵 기본 비활성으로 바뀝니다.
- HTS 맵 스크립트는 이 엔진을 Active Scripting(COM)으로 불러 씁니다. 엔진이 없으면 **화면은 뜨지만 스크립트 기반 조회와 이벤트가 동작하지 않습니다.**
- 수정 전에는 엔진 생성이 실패하면 **첫 스크립트 맵에서 HTS가 크래시**났습니다. 실측으로 확인했고, NULL 방어로 막았습니다.
- 단기 대책은 AxisAgent가 시작 시 VBScript 사용 가능 여부를 확인하고, 꺼져 있으면 DISM으로 켜도록 안내하는 것입니다.
- 근본 대책은 엔진 교체입니다. 진행 중인 Python 엔진 전환이나 2027 HTS 개편이 이에 해당합니다.

## 2. MS 일정

MS 공식 발표는 2024년 5월(Windows IT Pro Blog "VBScript deprecation: Timelines and next steps")입니다.

| 단계 | 시기 | 내용 | HTS 영향 |
|---|---|---|---|
| 1단계 | Windows 11 24H2부터 (현재) | 주문형 기능(FOD) "VBSCRIPT"로 바뀜. 기본 설치·활성 | 정상 동작. 단, 관리자나 보안정책이 끌 수 있음 |
| 2단계 | 2027년 무렵 | FOD 기본 비활성 | 새 PC나 재설치 PC에서 조회 불가 |
| 3단계 | 미정 | Windows에서 완전 제거 | DISM으로도 켤 수 없음. 엔진 교체 필수 |

- 2단계가 앞당겨질 수 있다는 보도가 있습니다(Neowin 등). MS는 관리자에게 미리 끄라고 권고하고 있습니다.
- 대상: `vbscript.dll`, `.vbs` 파일, wscript/cscript
- 대상 아님: Office VBA, VB6 런타임, VB.NET
- Windows 10은 이 계획의 대상이 아닙니다. 다만 지원이 끝나 Win11 전환이 진행 중입니다.

**Windows 설정에서 켜는 방법:** 설정 → 시스템 → 선택적 기능 → "기능 보기" → `VBSCRIPT` 검색 → 추가

**명령으로 켜는 방법:**
```
DISM /Online /Add-Capability /CapabilityName:VBSCRIPT~~~~
```
- 관리자 권한과 인터넷(Windows Update)이 필요합니다.
- 사내 WSUS 환경에서는 `0x800f0954`로 실패할 수 있습니다.

## 3. HTS 의존 지점

| 위치 | 내용 |
|---|---|
| `ibks/dll/vbs/scriptEngine.cpp` `Initialize` | `CLSIDFromProgID(L"VBScript")` → `CoCreateInstance(IID_IActiveScript)` → `IActiveScriptParse` → `SetScriptSite` → `InitNew` |
| `ibks/dll/vbs/engineWrapper.cpp` `ensureEngine` | pythonMode에 따라 Python과 VBScript 중 하나를 선택. VBScript 맵이면 `CScriptEngine` 생성 후 대기 중인 객체를 `AddObject` |
| `ibks/Wizard/Screen.cpp:43` | **화면(CScreen)마다** `CEngineWrapper`를 생성. 엔진은 화면 간에 공유되지 않음 |
| `ibks/Wizard/Screen.cpp:489` | 스크립트가 있는 맵만 `LoadScript` 호출 |
| `ibks/Wizard/Script.cpp` | 이벤트마다 `IsAvailable` → `DoProcedure` (64곳) |

- **두 단계 확인의 의미:**
  - `CLSIDFromProgID`는 레지스트리만 읽습니다.
  - `CoCreateInstance`가 실제로 `vbscript.dll`을 로드합니다.
  - 따라서 둘 다 성공해야 "엔진 사용 가능"입니다.
- **32비트 기준:** HTS가 32비트이므로 `Wow6432Node` 레지스트리와 `C:\Windows\SysWOW64\vbscript.dll`을 사용합니다.

## 4. 엔진 생성 실패 시 동작 (실측)

**테스트 방법:** `scriptEngine.cpp` `Initialize`에서 ProgID를 `L"VBScript1"`로 바꿔 실패를 재현했습니다.

| 상태 | 결과 |
|---|---|
| 수정 전 | 메시지박스를 닫은 직후 크래시. `ensureEngine` → `AddObject` → `m_as->AddNamedItem`(m_as NULL) Access Violation. 맵이 axis.exe 프로세스 안에서 실행되므로 HTS 전체가 종료됨 |
| NULL 방어 후 | 크래시 없음. 화면은 열림. 스크립트 기반 조회와 이벤트는 동작 안 함 |
| 실패 메시지박스를 화면 오픈 시마다 표시 | 화면마다 엔진을 새로 만들므로 열 때마다 표시됨. 프로세스가 눈에 띄게 느려짐. 메시지박스를 빼자 사라짐 (원인 설명은 별도 예정) |

**고객이 보게 될 증상:** "HTS는 뜨는데 조회가 안 된다"

## 5. 적용한 수정

### 5.1 scriptEngine.cpp NULL 방어 (ibks/dll/vbs, ASCII)

| 위치 | 내용 |
|---|---|
| `AddObject` | `if (m_as == NULL) return;` |
| `LoadScript` | `if (m_as == NULL \|\| m_asp == NULL) return false;` → `Screen.cpp`가 `LoadScript FAILED` 로그를 남김 |
| `getIDOfProcedure` | `m_dispatch` NULL 로그는 `m_as`가 있을 때만 출력. 엔진이 없으면 이벤트마다 로그가 쏟아지는 것을 방지 |
| `Close` | `if (m_as) m_as->Close();` |

`IsAvailable`/`DoProcedure`는 `getIDOfProcedure`의 NULL 확인을, `UnloadScript`는 기존 NULL 확인을 거치므로 수정이 필요 없습니다.

> **배포 전 필수:** `Initialize`에 넣은 테스트 코드(`CLSIDFromProgID(L"VBScript1", ...)` 블록)를 반드시 제거해야 합니다. 남겨 두면 모든 PC에서 스크립트가 동작하지 않습니다.

### 5.2 AxisAgent 시작 시 검사 (AxisAgent/AxisAgent/AxisAgentDlg.cpp, UTF-8 BOM)

| 함수 | 내용 |
|---|---|
| `IsVBScriptAvailable(path, cap)` | `CLSIDFromProgID` + `CoCreateInstance(IID_IUnknown)`. 로드된 DLL 경로는 `Release`/`CoUninitialize` **전에** 읽음 (CoUninitialize가 DLL을 언로드하기 때문) |
| `EnableVBScript(&err)` | `ShellExecuteEx("runas")`로 DISM 실행(UAC). 32비트 프로세스이므로 `%WINDIR%\Sysnative\dism.exe` 사용. 최대 15분 대기. 반환은 DISM 종료코드(0 성공, 3010 재부팅 필요) |
| `VBScriptCheckThreadProc` | 사용 가능하면 로그만 남김. 꺼져 있으면 확인창 → DISM → 결과 안내. 실패 시 설정 경로 안내. UAC 취소(1223)이면 메시지 없음 |
| `OnInitDialog` | MonitorThread 시작 후 별도 스레드로 실행 (모니터를 막지 않음) |

- 이 PC(Win10 LTSC 2019) 확인 결과: `[VBS] VBScript 사용 가능 path=...` 로그 출력
- 꺼짐 → 활성화 경로는 Win11 24H2 이상에서 확인 필요

## 6. 현장 사례와 조치

### 6.1 Win11 고객 PC: RegExp만 생성 안 됨 (2026-11-20)

| 항목 | 내용 |
|---|---|
| 증상 | VBScript 기능은 켜져 있고 다른 스크립트도 동작. IB000101에서 실시간 수신 때마다 **"개체가 필요합니다: 're1'"** (800A01A8) |
| 에러 위치 | `Onalert` 249행 `xval = re1.Replace(tDiff.GetTableCell(2), "")` |
| 실제 원인 | 화면 오픈 시 실행되는 전역 코드(`_AW_ONDECLARE_AW_`, 165행)의 `Set re1 = New RegExp`가 실패. 그 PC의 VBScript 구성(RegExp 부분)이 손상된 상태였음 |
| 해결 | Win11 설정 → 선택적 기능에서 **VBScript 제거 후 재설치** → 정상 |

**왜 엉뚱한 줄에서 에러가 보이나:**
```
화면 열기 → 전역 코드 실행 (scriptEngine.cpp SetScriptState(SCRIPTSTATE_CONNECTED))
   165: Set re1 = New RegExp     <- 실제 실패 지점 (VbGuide 첫 에러창)
   166~201: re2, gA, g_Arrdata ...  <- 실행 안 됨 (전역 코드는 에러 줄 이후를 건너뜀)
실시간 수신 → Onalert 호출 (Sub/Function 정의는 정상)
   249: re1.Replace(...)          <- "개체가 필요합니다: 're1'" (반복 표시)
```
- 전역 코드는 한 줄에서 런타임 에러가 나면 **그 아래 전역 코드를 모두 건너뜁니다.** Sub와 Function은 살아 있어서 "다른 VB는 된다"처럼 보입니다.
- 진짜 원인은 **화면을 열 때 처음 뜨는 에러창**(VbGuide)에 있습니다. 이후 실시간 에러창에 묻히기 쉬우므로, 문의가 오면 화면을 새로 열어 첫 창을 캡처받습니다.

**알게 된 점:**
- 엔진(`VBScript` ProgID) 생성이 되는 것과 RegExp가 되는 것은 별개입니다. **현재 AxisAgent 검사(`IsVBScriptAvailable`, 엔진 생성만 확인)로는 이 PC를 "정상"으로 판정합니다.**
- `map_src`에서 RegExp를 쓰는 맵은 **100개**입니다. 맵을 고치는 것보다 PC 쪽 복구가 현실적인 해결책입니다.
- RegExp는 `vbscript.dll` 안에 있습니다. CLSID는 `{3F4DACA4-160D-11D2-A8E9-00104B365C9F}`이고, HTS는 32비트라 `SysWOW64\vbscript.dll`을 씁니다.

**에러 문구별 판단:**

| 에러 | 의미 |
|---|---|
| 클래스가 정의되지 않았습니다: 'RegExp' (800A01FA) | RegExp 생성 실패 (이번 사례의 첫 에러로 추정) |
| ActiveX 구성 요소는 개체를 만들 수 없습니다 (800A01AD) | `CreateObject("VBScript.RegExp")` 생성 실패 |
| 개체가 필요합니다: 're1' (800A01A8) | 앞선 전역 코드 실패의 **결과**. 원인 아님 |
| Null을 잘못 사용했습니다 (800A005E) / 형식이 일치하지 않습니다 (800A000D) | RegExp는 정상. `Replace`에 넘긴 데이터 문제 |

### 6.2 PC 쪽 조치 순서

가벼운 것부터 진행하고, 단계마다 32비트 cscript로 RegExp 테스트를 다시 돌립니다.

| 단계 | 명령 | 권한 | 비고 |
|---|---|---|---|
| 0. 점검 | `C:\Windows\SysWOW64\cscript.exe //nologo re_test.vbs` | 일반 | 반드시 SysWOW64 cscript (HTS와 같은 32비트 엔진) |
| 1. HKCU 가로채기 제거 | `reg delete "HKCU\Software\Classes\CLSID\{3F4DACA4-...}" /f`, `reg delete "HKCU\Software\Classes\VBScript.RegExp" /f` | 일반 | 키가 있을 때만. 정상 PC에는 없음 |
| 2. 재등록 | `SysWOW64\regsvr32 vbscript.dll`, `System32\regsvr32 vbscript.dll` | 관리자 | HKLM 키가 TrustedInstaller 소유라 접근 거부가 정상일 수 있음 |
| 3. 시스템 파일 복구 | `DISM /Online /Cleanup-Image /RestoreHealth` → `sfc /scannow` | 관리자 | 10~30분. Win10 손상 시 주 수단 |
| 4. 기능 재설치 (Win11 24H2+) | `DISM /Online /Remove-Capability /CapabilityName:VBSCRIPT~~~~` → `/Add-Capability` | 관리자 | **이번 사례에서 효과 확인** (설정 화면으로 진행) |

- 이 PC 기준 레지스트리 권한: HKLM의 VBScript 엔진·RegExp CLSID 키는 소유자 SYSTEM, 쓰기는 TrustedInstaller만 가능, Administrators는 읽기 전용입니다. 그래서 다른 프로그램이 regsvr32로 덮어쓰기는 어렵고, 남는 가로채기 경로는 HKCU입니다.
- `vbscript.dll`을 다른 PC에서 복사해 등록하는 방법은 쓰지 않습니다(7장 참고).

### 6.3 RegExpRepair.bat

| 항목 | 내용 |
|---|---|
| 위치 | `src/tools/RegExpRepair.bat` (CP949, CRLF) |
| 실행 | 더블클릭 → 자동 관리자 권한 상승 → 0~4단계 순서 진행, 정상 판정 시 즉시 종료 |
| 사용자 확인 | 3단계(DISM/sfc), 4단계(기능 재설치)는 Y/N 확인 |
| 테스트 | 파일 끝 `:::` 줄에 넣은 VBS를 `%TEMP%\re_test.vbs`로 꺼내 32비트 cscript로 실행. `New RegExp`, `CreateObject`, `Replace("1,234won")=1234`를 확인하고 엔진 버전을 표시 |
| 로그 | `%TEMP%\RegExpRepair.log` (고객에게 받을 파일) |
| 의존성 | 없음. Windows 기본 cscript만 사용 (Python 불필요) |
| 확인 상태 | 테스트 스크립트는 이 PC(Win10, 엔진 5.8.16384)에서 OK 확인. 관리자 상승·1~4단계는 미실행(정상 PC라 진입 안 함). 4단계 기능 이름 파싱은 Win11 24H2에서 확인 필요 |

> AxisAgent에 같은 기능을 넣는 방안은 [[AxisAgent.md]] 과제로 검토합니다.

## 7. 검토한 대안

| 대안 | 결론 |
|---|---|
| `vbscript.dll`을 HTS 폴더에 두고 등록 없이 직접 로드 (`LoadLibrary` → `DllGetClassObject` → `CLSID_VBScript {B54F3741-5B07-11CF-A4B0-00AA004A55E8}`) | 기술적으로 가능하고, 코드는 `scriptEngine.cpp` 한 곳만 바꾸면 됨. 단, OS 구성요소라 **재배포 라이선스 위반 소지**가 있음. Windows에서 빠지면 **보안 패치도 끊김**. 백신·AppLocker 오탐 가능성도 있음. **비상용 임시 수단**으로만 보류하며, 법무·보안 확인이 필요함 |
| System32에 복사하고 regsvr32 등록 | 비권장. 관리자 권한이 필요하고, Windows 보호·업데이트와 충돌함 |
| 고객 PC의 VBScript를 코드로 끄기 | 비권장. 같은 PC의 다른 프로그램에 영향을 줌 |
| Python 엔진 전환 | 근본 대책. `engineWrapper` pythonMode 경로가 이미 있음 |
| 2027 HTS 개편(아이티젠)에 포함 | 2단계 시점과 오픈('27.09)이 겹침. 제안서에서 맵 스크립트 처리 방식을 반드시 확인해야 함 |

## 8. 남은 과제

- [ ] `scriptEngine.cpp`의 `VBScript1` 테스트 코드 제거 (배포 전 필수)
- [ ] HTS 쪽 안내 메시지: 프로세스당 1회(`static bool`) + 실패 로그(maps 포함). 화면을 열 때마다 띄우지 않음
- [ ] AxisAgent 활성화 경로를 Win11 24H2 이상에서 실측 (Sysnative + runas, DISM exit, 켠 뒤 재실행 필요 여부)
- [ ] AxisAgent에서 사용자가 "아니오"를 고른 경우 다시 묻는 주기 결정 (현재는 HTS를 실행할 때마다 물음)
- [ ] 메시지박스를 화면 오픈 중에 띄우면 느려지는 현상 원인 설명
- [ ] 회사 보고: 2단계 일정, 증상, 단기·장기 대응 (계기: MS 발표 일정 대응)
- [ ] 아이티젠 제안서에서 맵 스크립트(VBScript) 처리 방안 확인
- [ ] 직접 로드 대안의 법무·보안 확인 여부 결정
- [ ] AxisAgent 검사에 RegExp 생성 확인 추가 (엔진만 보면 6.1 사례를 놓침)
- [ ] `RegExpRepair.bat` 4단계를 Win11 24H2 PC에서 실측 (기능 이름 파싱, 제거와 추가 사이 재부팅 필요 여부)

## 9. 변경 이력

| 날짜 | 내용 |
|---|---|
| 2026-11-18 | 이슈 제기. MS 일정 확인, HTS 의존 지점 분석 |
| 2026-11-19 | 실패 시 크래시 실측 → NULL 방어 적용. AxisAgent 검사·활성화 코드 추가. 직접 로드 대안 검토. 문서 작성 |
| 2026-11-20 | Win11 고객 PC RegExp 생성 실패 사례(IB000101 're1' 에러) 원인 분석, 기능 재설치로 해결. `RegExpRepair.bat` 작성. 6장 추가(기존 6~8장 → 7~9장) |

## 관련 문서

- [[python_engine_260608.md]] — VBScript→Python 엔진 전환
- [[AxisAgent.md]] — AxisAgent 개선 과제
- [[HTS_Replatform_Proposal_Review.md]] — 2027 HTS 개편 제안 검토
