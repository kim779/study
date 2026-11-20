---
project: ibks
category: worklog
status: 진행중
updated: 2026-11-19
---

# 작업 일지 (하루 성과 / 다음 계획)

## 목차

- [사용법](#사용법)
- [다음 할 일 (누적)](#다음-할-일-누적)
- [2026-11-19 (목)](#2026-11-19-목)
- [2026-11-18 (수)](#2026-11-18-수)
- [2026-11-17 (화)](#2026-11-17-화)
- [2026-11-16 (월)](#2026-11-16-월)
- [2026-11-12 (목)](#2026-11-12-목)
- [2026-11-11 (수)](#2026-11-11-수)
- [2026-11-10 (화)](#2026-11-10-화)
- [2026-11-09 (월)](#2026-11-09-월)
- [2026-11-06 (목)](#2026-11-06-목)

---

## 사용법

- 하루가 끝날 때 맨 위에 날짜 절을 하나 추가한다. 최신 날짜가 위로 온다.
- 날짜 절에는 **한 일**과 **다음 할 일**을 적는다. 자세한 내용은 해당 과제 문서에 두고 여기서는 링크만 건다.
- **다음 할 일 (누적)** 은 아직 끝나지 않은 일만 남긴다. 끝난 항목은 체크한 뒤 그날 절의 "한 일"로 옮기고 여기서 지운다.

---

## 다음 할 일 (누적)

### AxisAgent / HTS 감시 ([AxisAgent.md](AxisAgent.md))

- [ ] 운영 소스 반영 전에 diff 확인 (오늘 이전부터 커밋 안 된 변경이 섞여 있음; `AxisAgent.rc`는 UTF-16)
- [ ] 배포 빌드의 PDB를 symstore에 등록 (axis.exe, cx_shared/form 등 DLL)
- [ ] 개발기 배포: `axis.exe` + `AxisAgent.exe`를 함께 (Agent만 새것이면 `MAPS=(unknown)`, 시작 유예 미동작). `AxisAgent.exe`는 수동 복사(A-19)
- [ ] 사무실 단위 테스트
  - [ ] 저사양 PC에서 화면 복원이 60초 안에 끝나는지 (60초 직후 Hung 덤프가 생기면 유예 연장)
  - [ ] `[M5] … 정상 복귀 (cnt n -> 0)`의 평소 n 분포 → 30초 기준 적정성
  - [ ] 장중 `MAIN[core]` 평소 수준
  - [ ] 각 PC `<HTS>\exe\ping\`의 `monitorlog_*.txt`, `dump_analysis_*.ini`, `*.dmp` 7일 안에 회수
- [ ] 남은 과제: A-1, A-2, A-6, A-8, A-9, A-10, A-12, A-18, A-20

### IB771000 종합시황 — 브릿지경제 추가

- [ ] 서버팀 작업 요청/확인: 브릿지경제 `GUBN=18`, BULRU 분류 코드를 룰대로 반영
- [ ] 서버 반영 후 조회 테스트: 전체 / 언론사별 / 카테고리 / 브릿지 노드 단독, 기존 언론사 결과 누락 없는지
- [ ] 버전 이행 테스트: v4 ini에서 이력 유지 + 브릿지만 자동 체크, 저장 후 v5로 기록돼 재자동체크 안 되는지
- [ ] `tab\newstab`이 로그인 파일 패치 배포 대상인지 확인
- [ ] 배포: `newstab`과 `MapDlg`(VERSION 5) 반드시 동시 배포 (MapDlg만 먼저 나가면 v5 저장돼 브릿지 자동 체크 안 됨)
- [ ] (선택) `MapDlg.cpp` 3963줄 주석의 원래 값 정정 (`11111101…` → 실제 `10111101…`)

### HTS 송신 보안 — 메모리 변조 대응 (클라이언트 한정)

- [ ] **(최우선) `NOENC.TXT` 운영 빌드 차단**: `Guard.cpp` `CGuard::IsNoEncMode()`에 `#ifdef _DEBUG` 등 제한이 없어 고객 PC exe 폴더에 빈 파일만 있으면 모든 송신 암호화가 꺼짐 → Release에서는 항상 `FALSE`
- [ ] **조회전용 로그인 전 구간 평문** (2026-11-16 확인): 서버가 조회전용 로그인 응답 `_signR::flag`에 `0x30`(flagENX+flagCAX)을 내림(일반은 `0x09` ENC+CA) → 키 교환 생략(WizardCtrl.cpp:1010), `m_xecure` 삭제(960), 송신 조건 `!(m_term & flagENX)`가 false → 맵(OP_ENC 포함)·DLL·SVC·CTRL 송신 전부 평문
  - [ ] 서버팀 확인: 조회전용에 ENX를 내리는 게 의도된 정책인지, 조회전용 세션(termN 끝자리 1, menu HTS010)에서 AXISENCX 키 교환·암호 TR 처리 가능한지
  - [ ] 결정: A 서버가 조회전용 flag를 `0x21`(ENC+CAX)로 / B 클라이언트가 ENX 무시(904·960·1010·송신 조건, 테스트 서버 선검증 필수) / C 조회전용에서 OP_ENC 화면 송신 차단 또는 개인정보 화면을 조회전용 메뉴에서 제외
- [ ] **DataGuard.dll 모듈화** (`ibks/DataGuard/DataGuard/`, MFC 일반 DLL·자동화 없음·MultiByte, 출력 `src/release/DataGuard.dll`) — 4-3·4-4·USERID·`SessionClearKind`까지 완료(2026-11-17), 현재 `DG_VERSION 2`
  - [ ] 배포: `axWizard.ocx` + `DataGuard.dll` **반드시 동시** (새 ocx는 version 2 요청 → 옛 DLL이면 NULL → fail-open으로 검사 전부 꺼짐. 옛 ocx + 새 DLL은 정상)
  - [ ] BLOCK 전환 판단: 운영 LOG 기간 동안 `[DG][Common]`(ID 불일치) 0건 확인. 나오면 usid에 로그인 ID가 아닌 값을 넣는 화면 → 화면별 확인
  - [ ] 2차: CTRL/DLL/`ServiceEx` 송신 경로 적용(지금은 Stream.cpp `TH_LEDGER` 맵 송신만), 수신 검증(`DG_RECV`), DLL 무결성 확인(서명/해시), 해시 재생성 경로를 로그인으로 제한(`AddRegistry(datB,datL)` Dll.cpp:116·Event.cpp:372), 직원 단말 예외
  - [ ] 세션 후보 추가 검토: `[AddRegistry] COMBO[n]` 목록 로그 확인, infox IP 필드 ↔ ledger `pcip`/`puip`(NAT 오탐 우려), `name`(본문에 이름 싣는 서비스 있을 때만). `termN`은 접속마다 바뀌어 제외
  - [ ] 운영 전 로그 정리: `[DG][Check] val=`(계좌 평문), `[AddRegistry] ACCNO account=`(Wizard Guard.cpp ~1979), `[LEDGERCHK]`, Guard.cpp LOG_OUTP 비밀번호 출력 제거. DLL `DG_SHAPE_LOG`(송신 평문 덤프)는 반드시 미정의(`#define DG_SHAPE_LOG1`) 상태로 배포. Stream.cpp `[DG] result` 라벨 `missmatch=` → `result=`(값은 불일치 개수가 아니라 0/1/2 결과 코드)
  - [ ] (선택) Wizard Guard.cpp 30~31줄 bcrypt 주석 삭제, DLL 규칙 테이블 위 옛 주석(`test rule: map puts own account into ledger fil2`) 정리
- [ ] **`CClient::sendInterGroup` 하드코딩 후속** (2026-11-17 소스에서 삭제 완료)
  - [ ] 삭제분 커밋. 같은 코드가 남은 사본 `Wizard_20240827`, `Wizard_test` 처리 여부 결정
  - [ ] 2023-01-17 `[kjs]` 변경 이력 확인, 하드코딩 계정(`rhk62`)·계좌·비밀번호가 실고객 것인지 확인 → 실계정이면 비밀번호 변경
- [ ] **계좌·비밀번호 노출 / 테스트 잔재** (2026-11-18 발견, 운영 배포본 포함 여부 확인 필요)
  - [ ] `APPL/IB0000A1(운영)/MapForm.cpp:1347` `ChangePasswordNotifyToMain`: `적용` 때마다 `계좌\t비밀번호\t…`를 `OutputDebugString`으로 평문 출력(DebugView로 누구나 열람) → **11-18 소스 주석 처리 완료**, IB0000A1 배포 남음
  - [ ] `CONTROL/CX_DATATRANS/MainWnd.cpp:361` `OnLButtonDblClk`: `SetAccountInfo("AN10|00110000014|zPwd|0423|")` 고정 계좌·비밀번호 → 컨트롤 더블클릭 시 동작. 운영 배포 여부 확인 후 제거
- [ ] **(잠복 결함) 계좌 목록 재수신(`accountDLL`) 형식 불일치** — 지금은 실행되지 않아 운영 영향 없음, 이번 DataGuard 수정과 무관한 기존 결함
  - 수신: Wizard `Dll.cpp:116`/`Event.cpp:372` → `CGuard::AddRegistry(datB,datL,cbonly=false)`(Guard.cpp 6016)가 `m_accno`/`m_account`를 **먼저 비운 뒤** `AddRegistry(…, dns)`(1914)로 파싱. 기대 형식은 로그인 응답과 같은 `_regH{regK 1B, regL 4B}` + `계좌\t비밀번호\0` × N (`regK_ACCNO 0x02`, h/axis.h:242)
  - 송신 ① `IB0000A1/MapForm.cpp:879` `processSave`: 저장 TR(`pidoaccn` 'U') 응답의 `_acInfo` 배열(59B 고정: accn 11 + acnm 45 + ownf 1 + fill 2, **비밀번호 없음**)을 헤더 없이 그대로 전송 → 첫 바이트 `0x20`을 regK로 읽고 `UNKNOWN regK=0x20 - parsing stopped` → **계좌 목록 0건**(계좌 콤보·비밀번호 사용 화면 막힘, 재로그인해야 복구)
  - 지금 안 터지는 이유: `OnOK`가 저장 TR 송신 직후 `CloseDll()`(992)로 화면을 닫아 응답이 와도 `processSave`가 실행되지 않음 → 우연히 막혀 있음. `CloseDll` 위치를 옮기거나 지우면 바로 재현(11-18 `CloseDll` 주석 처리로 재현 확인, `datL=1593` = 59 × 27)
  - 송신 ② `APPL/AxMapForm/AxMapForm.cpp:725·759` `SetAccountInfo`/`SetAccountINIT`: `0x02` + **개수 1바이트**(regL은 4자리 문자열이어야 함) → 역시 형식 불일치. 호출하는 화면은 못 찾음(IBXXXX08 시세포착설정 포함)
  - 두 송신자 모두 Wizard 형식이 바뀐 뒤 갱신 안 된 옛 프로토콜로 보임. 수정한다면 IB0000A1은 879줄 `SendMessage(accountDLL)` 삭제가 가장 안전(이미 `ChangePasswordNotifyToMain`(WM_USER+5555)으로 계좌·비밀번호를 건별 통지 중). 급하지 않음
- [ ] 서버팀 한 줄 확인: fil2 뒤쪽 12바이트(228~239) 클라이언트 사용 가능 여부, `term`/`usid`/`pbid`를 요청값으로 믿는지
- [ ] ENCODE 누락 맵 전수 조사: `HEADER(LEDGER)`인데 `OPTIONS`에 `ENCODE` 없는 맵 목록 → 고객 데이터 서비스인지 판단 (예: 직원용 IB9999AA/AC `SSLEQ450`)
- [ ] `svcd` 기준 평문 송신 목록 수집: `grep -A1 LEDGERCHK | grep -B1 "enc(statENC)=0"`

### 해외주식 — 미국 23시간 거래 (연합인포맥스 IMAX, 12/7(월) 시작)

- 개발서버 접속: `exe\IMAX\ini\sysenv.ini`를 인포맥스 제공본으로 교체(원본은 `sysenv_.ini`) → 재접속 시 `IMAX\bin` DLL이 TEST 버전으로 자동 갱신. 테스트 종목은 `D:\src\IBKS\testcode\미국23시간_TEST_종목코드.xlsx` 30종목(AMX/NYS/NAS, 예 KORU·SOXL·TQQQ)만
- 운영시간(ET 고정): Overnight 21:00~04:00(신규) / 거래 중단 20:00~21:00 / Pre·정규·After 기존. 한국시간 동절기 Overnight **11:00~18:00**, 중단 10:00~11:00. 서머타임(2027-03-14~11-07)엔 한국시간 전체 −1시간(길이 동일) → 시간 판정은 ET 기준 또는 DST 반영 필수
- [x] 11-17 확인: KORU 야간 체결·거래량 있음, 비대상(AA) 거래량 0 → 설정 정상. 일봉은 Overnight 시작 시 다음 거래일(11/17)로 전환
- [x] 11-18 확인(KORU): ET 20:00 체결 멈춤(마지막 19:59:41) → ET 21:00 재개(21:00:52). 21:00에 전일종가 6.67→6.48(11/17 종가), 시고저·거래량 초기화, 등락률 6.48 기준 → **Overnight = 다음 거래일(11/18)의 첫 세션**. 분차트에 Overnight 봉 나옴(After 봉은 분차트에 안 나왔음)
- [ ] 18:00(화면 18:15) Pre 전환·시세 연속성
- [ ] **(고민 중) 날짜 기준 불일치**: 일봉은 거래일(11/18), 분봉은 현지 달력 시각(`11/17 21:01`) → 거래일 하루가 분봉에서 이틀(전날 21:00 ~ 당일 20:00)에 걸침. 분봉 일자 구분·"당일" 필터·분봉→일봉 합성·일자별 분봉 조회 범위에 영향 가능. 우리가 해외 분봉을 쓰게 되면 거래일 기준 재그룹 필요
- [ ] 인포맥스 회신: ① 세션 표시가 Overnight·거래중단 중에도 `AFter`(대상·비대상 동일, IMAX DLL에 Overnight 라벨 문자열 없음) ② Overnight 시작 시 다음 거래일 전환이 의도인지 + 분봉에 거래일 정보가 있는지·분봉 조회 일자 기준(현지일/거래일)·분봉 화면 일자 구분 기준 ③ After 시세 대비가 당일 정규장 종가가 아닌 전일종가 기준(7.08, +0.41 = 6.67 기준)인지 ④ (참고) 전일거래량 57.2M vs After 종료 시 당일 거래량 17.6M 집계 범위 차이
- [ ] 업무부서 확인: 12/7부터 야간 **주문**도 받는지 (받으면 주문 가능시간·세션 처리 등 우리 쪽 작업 발생, 시세만이면 확인으로 끝)

### 티커 뉴스 — 브릿지경제 추가 (axiscm + sm)

- [ ] `DLL/axiscm/AxCommon.cpp:709` `newsSource[]`의 `"뉴스웨이",` 뒤에 `"브릿지경제",` (반드시 맨 뒤 — 저장값=목록위치+1)
- [ ] `DLL/sm/Pane.cpp:3468` 다음 줄 `case 16: index = 18; break;` (저장값 16 → GUBN 18, GUBN은 7710과 같은 가정)
- [ ] 설정 화면(`axiscp` `SetTickInfo`) 목록 16개 표시 확인, `axiscm.dll`+`sm.dll` 동시 배포

### VBScript 지원 중단 대응 ([VBScriptDeprecation.md](VBScriptDeprecation.md))

- [ ] **(배포 전 필수)** `ibks/dll/vbs/scriptEngine.cpp` `Initialize`의 테스트 코드 `CLSIDFromProgID(L"VBScript1", ...)` 블록 제거 — 남기면 모든 PC에서 스크립트 미동작
- [ ] HTS 쪽 안내: 프로세스당 1회(`static bool`) 메시지 + 실패 로그. 화면 열 때마다 띄우지 않음
- [ ] AxisAgent 활성화 경로(시작 시 검사 + `VB체크등록` 버튼)를 Win11 24H2 이상에서 실측: Sysnative DISM + runas, exit 코드, 켠 뒤 재실행 필요 여부
- [ ] AxisAgent에서 "아니오" 선택 시 다시 묻는 주기 결정 (지금은 HTS 실행 때마다)
- [ ] 메시지박스를 화면 오픈 중에 띄우면 느려지는 현상 원인 설명
- [ ] 회사 보고(2단계 일정·증상·단기/장기 대응), 아이티젠 제안서의 맵 스크립트 처리 방안 확인, 직접 로드 대안 법무·보안 확인 여부 결정

### astxManager.dll — ASTx 모듈화 ([astxManager_construction.txt](../astxManager/astxManager/astxManager_construction.txt))

- [ ] installASTx 경로 실측 (ASTx 미설치 PC/VM): `AM_Install` → 설치 후 `AOSRESTART` 재시작까지
- [ ] 6단계: MainFrm의 fallback 코드를 `AstxClient`로 이동, MainFrm ASTx 함수는 UI·정책만 남김
- [ ] 7단계: 두 콜백(`STSDKEX_EventCallback`, `AKSDK_Callback`) 스레드 ID 확인 → AstxClient static 콜백 + `PostMessage`로 MainFrm 전달 (pParam 값은 Post 전에 복사)
- [ ] 8단계: MainFrm.h의 SDK include/lib를 AstxClient로 이동, `Err2Str`·`g_pIAstxAkSDK` 정리
- [ ] `MainFrm.cpp` 336 `WriteLog()`에 넣은 테스트용 `OutputDebugString` 3줄 배포 전 제거 여부 결정 (원래 본문 `#if 0`이라 no-op 함수, 켜면 176곳 출력)
- [ ] astxManager 프로젝트 Debug|Win32 문자 집합 MultiByte로 변경 (지금 Unicode → Debug 빌드 시 `IAstxMkd8A` 타입 오류)
- [ ] 배포: `axis.exe` + `astxManager.dll` 함께 (DLL 없으면 기존 경로로 동작하므로 반대 경우는 안전)

### IB771000 종합시황 — 하단 Edge(WebView2) 뷰어 방어 로직

- [ ] 프로세스 종료(kind=0·1) 복구 재확인 1회 (IE 폴백 추가 후, kind=0 env 재생성 실패 시 IE로 가는지 포함)
- [ ] env 미준비 재시도 경로(`env NOT ready -> retry`) 확인: PC 재부팅 직후·저사양 PC 첫 실행 — 5초 안에 env가 안 되면 이제 IE로 전환됨(why=2)
- [ ] 배포: `BrowserWindowEdge.cpp/.h` + `MapDlg.cpp/.h` (브릿지 작업과 같은 DLL이므로 newstab과 함께 나감)
- [ ] 참고(범위 외): `NavigationStarting`이 http:// 링크 차단, `Resize()` 콤마 연산자, `Navigate()`의 무의미한 Event 대기, kind=0 화면 2개일 때 env 2회 생성(무해). `UploadEdgeEnv` 판단은 axis.ini `[EDGE]` 하루 1회 캐시(당일 런타임 제거·손상 시 그날은 Edge 시도 → 이제 IE 폴백으로 흡수)

---

## 2026-11-19 (목)

### 한 일

**VBScript 지원 중단 대응** ([VBScriptDeprecation.md](VBScriptDeprecation.md) 신규)

- MS 일정 정리: Win11 24H2부터 FOD(기본 활성) → 2027년 무렵 기본 비활성 → 제거(미정). 설정 경로 `시스템 > 선택적 기능 > VBSCRIPT`, `DISM /Online /Add-Capability /CapabilityName:VBSCRIPT~~~~`
- 엔진 생성 실패 실측(ProgID를 `VBScript1`로 바꿔 재현): 수정 전에는 첫 스크립트 맵에서 HTS 크래시(`AddObject`의 `m_as` NULL) → `scriptEngine.cpp` NULL 방어 4곳 추가 후 크래시 없음, 화면은 열리고 스크립트 조회만 안 됨
- 화면(CScreen)마다 엔진을 새로 만드는 구조라 실패 메시지박스가 화면마다 뜨고 프로세스가 느려짐 → 메시지박스 제거로 해소
- AxisAgent: 시작 시 VBScript 검사 스레드(`CLSIDFromProgID` + `CoCreateInstance`, 경로는 Release 전에 읽음) + 꺼져 있으면 Sysnative DISM runas. `VB체크등록` 버튼으로 수동 검사(중복 실행 방지 플래그). 이 PC(Win10)에서 `사용 가능 path=…SysWOW64\vbscript.dll` 확인
- 직접 로드 대안(`LoadLibrary` → `DllGetClassObject`) 검토: 가능하나 라이선스·패치·오탐 문제로 비상용 보류

**127 로드 오류 진단 + 모듈 종속 검사 도구**

- 원인: 새로 빌드한 axWizard.ocx 옆에 운영 axisvbs.dll(CEngineWrapper 없음)을 둬서 정적 import 누락(127) → COM이 레지스트리 CLSID 경로의 운영 위자드를 대신 띄움
- `tools/peimp.py` 작성(같은 폴더 DLL의 export와 import 비교), [WinDbg.md](WinDbg.md) 8장에 사용법 기록
- 같은 기능을 AxisAgent `모듈종속` 버튼으로 C++ 이식(`LoadLibraryEx(AS_IMAGE_RESOURCE)`, Python 불필요) — 동작 확인
- AxisAgent resource.h ID 중복(DEPEND/DEPEND2/VBCHECK, `_APS_NEXT_CONTROL_VALUE`) 정리

**astxManager.dll — ASTx 모듈화** ([명세서](../astxManager/astxManager/astxManager_construction.txt))

- `ibks/astxManager` 일반 Win32 DLL(MFC 미사용, MultiByte) 생성·Release 빌드: export 14개, StSdkEx.dll ordinal 11개가 axis.exe와 동일
- axis.exe 쪽 `AstxClient` 클래스(AXIS/AstxClient.h/.cpp, 플랫폼 중립·SDK 헤더 불필요) + MainFrm 멤버 `m_astx`. 없으면 기존 코드로 fallback
- MainFrm 연결 1~5단계 적용·로그 확인: 로드 / init·uninit / PB·FW / AK / 설치확인·IsASTxRunning
  - 종료 순서가 `AK stop → Stop(PB·FW·PCSCAN) → Uninit`으로 바뀜(기존은 SDK 종료 뒤 AK 해제)
  - IsASTxRunning(강제 종료 조건)은 DLL 초기화 후에만 DLL 사용 → 로그인 직후·30초 타이머 모두 `on[0]` 유지

**기타**

- 직원/고객 ID 숫자 필터 위치 확인: `AXIS/Dtconnect.cpp:715`, `CertLogin.cpp:822` (`IsNumber(m_user)`; MainFrm 232 pibostaf는 호출부 주석으로 죽은 코드)

### 다음 할 일

- astxManager 6~8단계(fallback·콜백 이관), installASTx 실측
- VBScript: `VBScript1` 테스트 코드 제거, Win11 실측
- 누적 "HTS 송신 보안" 배포 전 항목

---

## 2026-11-18 (수)

### 한 일

**DataGuard — `SessionClearKind` 검증 완료**

- 실제 화면에서 `accountDLL` 경로를 타는 곳이 없어(IB0000A1 `적용`은 응답 전에 화면 닫힘) 테스트 PC에서만 `IB0000A1 OnOK`의 `CloseDll()`을 임시 주석 처리해 강제 실행
- `[DG] session clear kind=1 count=2` → 계좌(kind 1)만 지우고 USERID·JUNO 2건 유지 확인. 이후 SACMT289 `Kind=3 chk res=1` 유지
- IB170900도 `SACMT289`를 보내며 offset 390이 주민번호(`vlen=15 chk res=1`) → 같은 규칙으로 커버됨
- 계좌 재등록은 IB0000A1 데이터 형식 불일치로 이 경로에서 원래 동작하지 않음(누적 "잠복 결함" 참고). 로그인 경로의 재등록(1984)은 11-16에 검증됨

**발견 (누적 할 일에 기록)**

- 잠복 결함: `accountDLL` 형식 불일치 → 경로가 열리면 Wizard 계좌 목록 0건
- IB0000A1 비밀번호 평문 `OutputDebugString`, CX_DATATRANS 고정 계좌·비밀번호

### 다음 할 일

- (완료) IB0000A1 `CloseDll()` 원복·재빌드. 비밀번호 평문 `OutputDebugString`(1347~1348)도 주석 처리 → IB0000A1 배포 시 반영
- 미국 23시간: 시간대별 확인 → 인포맥스 회신, 업무부서에 야간 주문 여부 확인
- 누적 "HTS 송신 보안": 운영 전 로그 정리 범위에 IB0000A1·CX_DATATRANS 추가 검토

---

## 2026-11-17 (화)

### 한 일

**DataGuard.dll — 4-3 · 4-4 · BLOCK 검증** (`DataGuard/DataGuard/Guard.cpp` CP949, `Wizard/Stream.cpp`, `Wizard/Guard.cpp/.h`, `h/DataGuardApi.h`, 사용자 적용)

- 4-3 `Check()` 연결: Stream.cpp `TH_LEDGER` 송신에서 `m_dg->Check(lg->svcd, …, DG_SEND)` → `[DG] result`. `DG_BLOCK`이면 `m_sndL -= L_axisH`로 그 화면 송신만 빼고 `SetGuide("Data verification failed.")`
- SACMT289 규칙 `{ "SACMT289", DG_LOG, 1, {{390,24,DG_JUNO}} }`: 처음 `n=2`(+AcntNo 414)는 AcntNo가 비어 있어 `dg=1` 오탐 → `n=1`. 주민번호는 실제 `vlen=15`로 들어옴(문제 없음)
- SAMFQ014 테스트 규칙(fil2 228 계좌)은 IB820899가 fil2를 비워 보내 오탐 → 삭제
- 검증: 정상 `own=1 dg=0` / 송신 직전 메모리 변조 `own=0 dg=1` / 규칙을 `DG_BLOCK`·off 392로 바꿔 `dg=2 BLOCK` + 송신 차단, 다른 화면 정상 → 규칙 원복 확인
- 4-4: Wizard inline 검사(`ACCCHK`/`protSvcd`/`DF_ACCCHK_BLOCK`)와 `AccHash*`·`m_accHash`·`m_accSalt`, Wizard의 bcrypt 링크 제거 → DLL만 남김, 빌드·동작 확인
- `[DG][Check]` 로그 안전화: `data=[%s]`(평문 + 길이 초과 읽기) 제거, 값은 계좌 종류만 출력(나머지 `-`). 사용자 확인용 전체 덤프는 `#ifdef DG_SHAPE_LOG`로 묶고 평소에는 꺼둠(`#define DG_SHAPE_LOG1`)

**DataGuard — 사용자 ID 검사 추가 (`DG_USERID`, `DG_VERSION 2`)**

- 로그인 데이터 검토: 로그인 응답 `sign`(h12589)이 login.dll을 거쳐 ledger `usid`(offset 23, 16B)에 들어감(libLogin.cpp:32, ledgerx.cpp:558) → 모든 원장 송신 같은 위치라 서비스 공통 검사로 적합
- DLL: 서비스 규칙과 별도로 `g_common[] = {{23,16,DG_USERID}}`, `g_commonMode = DG_LOG`. 송신마다 돌기 때문에 **불일치일 때만** `[DG][Common]` 로그. 세션에 그 종류가 없으면(로그인 전) 건너뜀(`HasKind`). 결과는 가장 강한 mode
- **svcd 공백이면 공통 검사 제외**: IB200088(주식실시간체결, `TRXCODE("pihoSchg")`, `HEADER(LEDGER)`이지만 svcd·usid 공백)이 `vlen=0` 오탐 → 정보계 TR로 보고 제외. svcd가 있는데 usid가 빈 경우는 계속 불일치로 잡음
- Wizard signON: juno 다음에 `signR->sign` → `SessionAdd(DG_USERID)`
- `SessionClearKind(kind)`를 `DG_API` 끝에 추가: 계좌 목록 재수신(`AddRegistry(…, cbonly=false)`, Guard.cpp 6040)이 `SessionClear()`로 주민번호·ID까지 지우던 문제 → `SessionClearKind(DG_ACCOUNT)`로 계좌만 교체
- 검증: `[DG] load OK … ver=2 size=28`(함수 포인터 5개 포함 크기) / 계정계 송신(SAMFQ014·SFCAT006·SACMQ528·SONAQ135·SDPAQ066·SONAQ201) 전부 통과 / IB200088 제외 확인 / usid 변조 → `[DG][Common] TR=SONAQ135 … kind=2 vlen=6 own=0 mode=1` 탐지

**발견: `CClient::sendInterGroup` 하드코딩 송신** (`Wizard/Client.cpp` CP949, −90줄, 사용자 삭제)

- 그리드 오른쪽 클릭 메뉴(`AS_PORTFOLIO`, Client.cpp:2248)를 만들 때마다 `if (1)` 블록이 다른 사용자 ID(`rhk62`)·고정 계좌·`SACMT238`·고정 비밀번호를 담은 ledger를 `ServiceEx("pidomyst", …)`로 보내고 있었음. 원래 동작은 `else`의 관심그룹("MY") 조회. 주석 `[2023/01/17] [kjs]`, git은 `ffd9e2e4`(2026-02-17, 트리 최초 등록)부터 존재
- 운영 `C:\IBKS\IBK투자증권 HTS\exe\axWizard.ocx`(2026-06-05, 667,600B)에는 해당 문자열 없음(ASCII·UTF-16 모두) → **운영 고객 영향 없음**. 개발 빌드에만 있었음
- `if (1)` ~ `else` 삭제 후 빌드 ocx에서 `rhk62`·계좌·`SACMT238` 0건, `pidomyst` 1건 확인. `ServiceEx` 경로라 DataGuard로는 못 잡는 송신이었음

### 다음 할 일

- 계좌 재조회 후 주민번호 유지 확인(`session clear kind=1`, SACMT289 `own=1`)
- 누적 "HTS 송신 보안": 운영 전 로그 정리, ocx+DLL 동시 배포 준비, `sendInterGroup` 이력·계정 확인
- 누적 "IB771000": 브릿지 서버 작업 → 테스트 → 동시 배포

---

## 2026-11-16 (월)

### 한 일

**DataGuard.dll — 3단계 ~ 4-2단계** (`ibks/DataGuard/DataGuard/Guard.cpp`, `Wizard/Guard.cpp/.h`, 사용자 적용)

- 3단계 `Guard.cpp`(DLL): 규칙 테이블 `Field{off,len,kind}`/`Rule{svc,mode,n,f[4]}`(`n`=사용 필드 수), 세션 저장소(salt SHA-256, 해시 앞 8자리·개수만 로그), `Check`, `DG_GetApi`. 빌드 25,600B x86, export `DG_GetApi` 하나 확인 → `D:\util\HTS\IBK_SMART\exe` 배포
- 4-1 Wizard 로드: `CGuard::Initial`에서 exe 폴더 전체 경로로 `LoadLibrary` → `DG_GetApi(DG_VERSION)`, 실패 시 **fail-open**(`m_dg == NULL`이면 검사 전부 생략, 사용자 결정 — 테스트 기간 충분히). 소멸자에서 `FreeLibrary`
  - 경로 버그 2회: `else dgPath = "DataGuard.dll"` / `else dgPath += …` 때문에 폴더 경로로 로드 → err=126 → `else` 제거로 해결. 이 과정에서 fail-open 경로(로드 실패해도 HTS 정상)도 확인됨
  - HTS 실행 중에는 ocx가 교체되지 않아 옛 버전이 돌던 것도 원인 중 하나 → HTS 종료 후 복사
- 4-2 세션 전달: `AccHashAdd/Remove`, `m_accHash.RemoveAll()` 2곳에 `SessionAdd/Remove/Clear` → 로그 `[DG] session clear` 1회 + `add count=1..38`이 `[ACCHASH]` 38건과 짝으로 나옴
- 사고: VS가 CP949 `Guard.h`를 UTF-8로 저장해 한글 주석 11줄 깨짐 → git checkout + iconv 절차로 복구(+3줄, 한글 정상). CP949 파일은 VS에서 "한국어(949)"로 저장할 것
- 발견: Stream.cpp `//#ifdef DF_ACCCHK_BLOCK`가 주석 처리돼 inline 차단이 항상 동작 → IB820899 오차단 (누적 할 일)

**SACMT289(IB820850) 본문 분석 — 첫 타깃 서비스**

- 맵 IOORDER 기준 입력 블록 258B: INA(5) TrxTp(1) **Rmnno(24, `LOGIN.Juno`=주민번호)** AcntNo(20, 스크립트에서 값 안 넣음) Pwd(8) INB(200). ledger 384 + 258 = 642 = 로그 `[Xecure] nBytesIn=642`로 offset 검증
- 민감 데이터는 계좌가 아니라 주민번호(data offset 390) → 세션 종류 `DG_JUNO` 추가안. 주민번호는 login.dll과 같은 출처인 `_signR::infox` 탭 7번 필드(덤프로 13자리 확인)
- 4-3(`Check()` 연결) + SACMT289 규칙 코드 제시 — 적용·검증은 다음

**평문 송수신 보고서 대응**

- 보고서의 "IB820850 평문"은 IB820850이 로그인 직후 자동으로 보내는 서브맵 **IB820899(`SAMFQ014`, OPTIONS에 ENCODE 없음)** 로 추정 → `OPTIONS(ENCODE|CR2TAB)`로 수정, 통과 확인(사용자). `SAMDT002` 맵도 OP_ENC 추가 후 통과
- 클라이언트 암호화 판단은 서비스 성격이 아니라 맵 `OPTIONS(ENCODE)`/호출 시 `US_ENC` 플래그 하나뿐. 같은 조회(Q) 서비스도 맵마다 ENCODE 유무가 다름 = 누락
- **조회전용 로그인은 전부 평문**: 로그인 응답 덤프 비교 — flag 조회전용 `0x30`(ENX+CAX) vs 일반 `0x09`(ENC+CA), termN 끝자리 1/2, menu HTS010/HTS001, 나머지 동일. 디버거의 `m_term` "0"/"9"는 `unsigned char`라 문자 `'0'`(48)과 `'\t'`(9)였음
- `m_term` 세팅 경로: `msgK_SIGN`/`SIGNx` → `CWizardCtrl::OnSign`(853, `[OnSign-response]` 로그에 flag) → `CGuard::Sign`(Guard.cpp:1853) → 플래그 보정 후 WizardCtrl.cpp:906 → 키 교환 판단 1010

**IB771000 — Edge 실패 시 IE 폴백** (`BrowserWindowEdge.cpp` CP949, `.h`, `MapDlg.cpp/.h` CP949, +79/−13)

- 원인: `m_bUsingEdgeBrowser`는 런타임 설치 여부(`GetAvailableCoreWebView2BrowserVersionString`, axis.ini `[EDGE]` 하루 1회)만 보고, 실제 생성 실패 시 흰 화면으로 멈춤. IE 컨트롤 `m_pBrowser`는 Edge 사용 시에도 숨겨진 채 항상 생성돼 있음
- 이전 메모 정정: `IsProgramInstalled`가 한글 표시명을 비교한다는 건 오기(레지스트리 열거는 주석, 한글은 ini 키 이름일 뿐)
- `BrowserWindowEdge::NotifyFail(why)` → 부모에 `WM_EDGE_FAILED`(WM_USER+0x771) Post. why 1 env 생성 실패(`!m_bEnvCreating`이면 대기 없이 즉시) / 2 env 5초(100ms×50) 미완료 / 3 컨트롤러 생성 실패(**E_ABORT 제외**) / 4 WebView 획득 실패 / 5 컨트롤러 요청 실패. `IsWindow(m_hWnd)` 가드
- `CMapDlg::OnEdgeFailed`: 한 번만 전환, Edge 위치를 IE에 복사, Edge 숨김, `m_bUsingEdgeBrowser = FALSE`, 현재 기사 `Navigate2`. 창 생성 실패는 `AfxMessageBox` 제거하고 바로 IE
- 검증: why=1(env 콜백 `result`를 디버거로 E_FAIL) → IE / why=3 → IE, 화면 다시 열면 Edge 정상 / 열자마자 닫기 3회 `hr=0x80004004` 후 NotifyFail 없음(첫 적용 때 why=3 오판 재현 → E_ABORT 조건 추가로 해결) / 정상 경로 회귀·IE 상태 기사 클릭·인쇄·리사이즈 정상
- 관찰: IB771000 화면을 모두 닫으면 DLL이 내려가 정적 env도 초기화 → 다음 열 때 env 새로 생성(실패한 PC도 다음에 다시 Edge 시도)

### 다음 할 일

- 누적 "HTS 송신 보안": 4-3 + SACMT289 소스 적용까지 완료(저녁) → 배포본 갱신 확인 → 로그인·IB820850·IB820899 로그 검증, 조회전용 ENX 정책 서버팀 확인
- 누적 "IB771000": 브릿지 서버 작업 → 테스트 → newstab + MapDlg/BrowserWindowEdge 동시 배포

---

## 2026-11-12 (목)

### 한 일

**계좌 변조 검증 — 프로토타입 완료** (`Wizard/Guard.cpp/.h`, `Stream.cpp` — 키워드 `ACCHASH`, `ACCCHK`)

- `m_account`(계좌→비밀번호·계좌명), `m_accno`(계좌 순서 목록) 역할 확인. 로그인 응답 `AddRegistry` `regK_ACCNO`에서 채워짐
- 해시 목록 `m_accHash` 추가: 영숫자만 남겨 정규화 + 프로세스별 16바이트 salt(`BCryptGenRandom`) + SHA-256. 추가(`AddRegistry` ACCNO·`SetAccounts`)/삭제/전체삭제 지점에 연결 → 로그인 로그 `[ACCHASH] add` 38건 = `ACCNO count=38` 확인. 계좌는 11자리라 fil2 12바이트 칸에 들어감
- `CGuard::AccHashCheck` + `[LEDGERCHK]` 블록에서 fil2 228~239(`lg->fil2 + 37`) 대조 → IB170900(`SAMFQ014`) 테스트: 타인값 `own=0`, 본인 계좌 `own=1`
- 로그 인자 순서 오류(tr/acc 뒤바뀜) 수정
- 보호 svcd 목록 + 차단 코드 제시(`own=0` 또는 보호 서비스인데 필러 비어 있으면 BLOCK, `m_sndL -= L_axisH` 롤백 + `SetGuide`) → 차단 후 다른 조회 정상 확인
- `m_sndL -= L_axisH` 의미: MakeStream이 axisH(+24)를 먼저 확정하고 본문 길이는 성공 시 마지막에 더하므로, 중단 시 axisH만 빼면 이 화면이 버퍼에서 빠짐(여러 화면 동시 송신 시 다음 프레임 위치 보호)

**axisH 문의 정리**

- `msgK_CTRL`: 맵 안 컨트롤 TR 송수신(송신 Guard.cpp:3691, 수신 Stream.cpp:231 → 해당 FM_CONTROL `WriteData`). MakeStream을 안 거쳐서 `[ACCCHK]` 대상 아님
- `msgK_MAPX`: 클라이언트→서버 전용, 화면 닫힘/맵 변경 시 해당 key들의 실시간 등록 해제(Guard `xAlert`, Dll `xAlert`, Dde `OnExit`). 수신 처리 없음
- `statAUX`(0x80): 수신 시 axisH와 본문 사이에 `auxH`(cursor 8 + type 1 + datL 1) + 안내 메시지가 있음. type `'1'` GUIDE 폼, `'2'` 시스템, `'3'` 경고창, `'4'` 확인창
- `ACTF`: 클라이언트 axisH에 해당 필드 없음(서버 쪽 명칭으로 추정, 출처 확인 필요)

**DataGuard.dll 모듈 설계·착수**

- 방향(사용자 결정): 별도 DLL `DataGuard`, 세션 검증 데이터를 계좌에 한정하지 않고 종류별로 Wizard→DLL 전달, 장기적으로는 ledger 필러가 아니라 **서비스별 본문 I/O 구조의 민감 필드를 DLL이 직접 판정**(맵 수정 불필요, "필러만 두고 본문 변경" 한계 해소). DLL 함수는 서비스명 + 데이터 포인터만 받음
- 인터페이스: export는 `DG_GetApi(version)` 하나 → `DG_API` 함수 테이블(size/version, append-only). `SessionClear/Add/Remove(kind,value)`, `Check(svc, data, len, dir)` → `DG_PASS/LOG/BLOCK`. 규칙 테이블은 DLL 내장(offset은 ledger 포함 data 시작 기준 → 현재 fil2 228 테스트를 규칙 한 줄로 재현), 차단 여부는 규칙 `mode`가 결정
- 프로젝트: 처음 자동화 포함으로 생성 → 자동화 빼고 MFC 일반 DLL로 재생성, Release|Win32 MultiByte·`DATAGUARD_EXPORTS`, 빌드 확인. 공용 헤더는 프로젝트 `DataGuard.h`와 이름 충돌 피해 `h/DataGuardApi.h`
- `Guard.cpp` 코드는 제시까지 (내일 추가)

### 다음 할 일

- 누적 할 일 "HTS 송신 보안 → DataGuard 3단계"부터: `Guard.cpp` 추가 → 빌드 → export 확인 → Wizard 연결

---

## 2026-11-11 (수)

### 한 일

**송신 암호화 여부 로그** (`Wizard/Stream.cpp`, `Guard.cpp` — 검색 키워드 `SENDENC`)

- 송신 경로 4곳에 요청 플래그와 실제 결과 기록: 맵 `[0-MakeStream-enc]`(`OP_ENC`), Service `[SVC-send]`, DLL 화면 `[DLL-send]`(신규, `CGuard::Write`), 맵 내 컨트롤 `[CTRL-send]` (`US_ENC`)
- 관계 정리: `US_ENC`/`OP_ENC`=요청, `statENC`=결과. 결과 = 요청 && !`flagENX`(서버 로그인 응답의 암호화 금지) && !`NOENC.TXT` && Xecure 성공
- 첫 로그에서 전부 `enc=0` → exe 폴더(`D:\util\HTS\IBK_SMART\exe`)의 `NOENC.TXT` 때문 → 이름 변경 후 `req=1`이면 모두 `enc=1` 확인
- 평문 송신 TR(`req=0`): DLL `pibfenca` `GOOPPOOP` `PIBOTICK` `pibfstup` `pibo2018` `pidouini` `PIDOMYST`, 맵 `pmho1001` `pihoscpw` `pibotrrt` `pidogood` `pibo1711` `pihswbbs` `pibo7700` `pibo2018`, 그리고 `piboPBxQ`는 IB820899만 평문(IB8208A0/50/75/88은 암호화)

**와이어샤크 패킷 분석**

- 프레임 = fmH(12) + [axisH(24) + 본문]. axisH는 TR 세션(`ssM=01`)에만 있고 시세(`ssM=08`)는 실시간 레코드가 바로 옴
- 압축(`fmH.stat & 0x02`)은 종류별 규칙이 아니라 프레임별: 클라이언트 송신은 "압축해서 줄면 압축", 시세도 압축/비압축 혼재 → 서버 기준은 미확인
- 처리 순서 확인: 본문 암호화 → axisH 포함 통째 압축 → fmH. 그래서 압축 프레임은 TR 코드도 안 보임, 암호화된 송신은 대부분 비압축
- LZW(9→13비트, LSB, 사전 257부터) 디코더로 압축 송신 패킷을 풀어 axisH `stat=0x00`(평문) 확인. 스크립트는 세션 scratchpad `lzw.py`
- 서버 IP는 211.255.204.38 / .70 두 대 확인

**메모리 변조(타인 개인정보 조회) 대응 논의**

- 원리: 같은 사용자 권한이면 프로세스 메모리 읽기/쓰기 가능(치트엔진과 동일). 암호화(Xecure/HTTPS)는 회선만 보호, 암호화 전 평문 변조는 못 막음
- 근본 대책은 서버의 세션 기준 소유 검증이지만 단기간 어려워 **클라이언트 한정**으로 진행하기로 함
- `[LEDGERCHK]` 로그 추가 (`Stream.cpp` 1495, 인증서 처리 후·Xecure 직전): svcd/usid/pbid/term/brno/odrf/fil1/fil2 (epwd·본문은 기록 안 함)
- `SAMDT002`의 정체 = ledger `svcd`(서비스코드, 맵 TxHead). IB820899 `SAMDT002`, IB170900 `SAMFQ014` 모두 `odrf=1`(주문)인데 평문 송신
- `pbid`/`term`은 첫 송신엔 공백, 응답 ledger가 화면 ledger를 덮어써서 이후 송신에 서버 값이 그대로 실려 감 → 클라이언트가 바꿀 수 있는 값
- 맵 스크립트 `SCREEN.SetLedger pos,len,val` / `GetLedger(pos,len)`로 fil2 접근 가능 확인 (송신까지 전달)
- fil2(191~239) 서버 처리: 191 그대로, **192~195 화면번호(svcN)로 덮어씀**, 196~199 그대로, **200~207 공백화**, 208~209 그대로, **210~227 공백화**, **228~239(12바이트) 그대로**
- 설계 방향: 로그인 계좌 해시 목록(Wizard) + 보호 화면은 fil2 228~239에 본문과 같은 계좌를 **평문**으로 넣음 + `Xecure` 직전에 해시 목록과 대조. 화면 값 변조(치트엔진 "전부 바꾸기")는 차단, "필러만 빼고 본문 변경"·후킹(검사 후 변조)은 못 막음(한계로 수용)
- 발견: `NOENC.TXT` 우회가 운영 빌드에도 살아 있음 (누적 할 일 최우선)

### 다음 할 일

- 위 [다음 할 일 (누적)](#다음-할-일-누적)의 "HTS 송신 보안" — 로그인 계좌 목록 수신 위치 파악부터

---

## 2026-11-10 (화)

### 한 일

**IB771000 — 하단 Edge(WebView2) 뷰어 방어 로직** (`BrowserWindowEdge.cpp` CP949 +149/−35, `.h`, `MapDlg.cpp`)

모든 수정은 `[edge]` 로그로 재현하고 다시 확인함. 진단 로그는 남겨 둠.

- **컨트롤러 중복 생성(②) 수정**: `m_bCreating` 플래그. 생성 중에 `Init`이 다시 들어오면 URI만 바꿔 두고, 생성이 끝나면 그 URI로 이동. 완료 콜백·요청 실패 시 해제 → 화면 열 때 create 1회 확인
- **렌더러 종료(kind=1) 자동 복구**: `RESULT_CODE_KILLED` 화면 대신 `Reload` (3회 제한, 탐색 성공 시 횟수 초기화) → 다시 그려지는 것 확인
- **브라우저 프로세스 종료(kind=0) 자동 재생성**: 까만 화면 원인. controller/webview/env를 비우고 `InitInstance` → `InitWebView`. env 미준비면 100ms 타이머로 재시도(최대 50회). 정적 `m_bEnvCreating`으로 env 중복 생성 방지 → 화면 1개·2개, 여러 차례 종료 모두 복구 확인
- **응답 없음(kind=2)**: MessageBox 대신 `Reload`
- **MessageBox 3곳을 로그로 대체**: kind=2, 런타임 없음(`ERROR_FILE_NOT_FOUND`), `CheckFailure`
- **잠재 null 참조 크래시 제거**
  - 완료 콜백에서 `get_CoreWebView2`가 실패하면 controller를 비우고 빠져나감
  - `Settings`를 `wil::com_ptr`로 바꿈 (참조 누수 해소)
  - DevTools receiver 2곳에 null 체크
- **[인쇄]/[인쇄 미리보기]가 Edge에서 동작하지 않던 문제**: IE 경로만 있었음. `ShowPrintUI`(ICoreWebView2_16, 없으면 `window.print()`)를 추가해 인쇄창이 뜨는 것까지 확인. Edge 인쇄에는 기존 IE의 사용자 머리글/바닥글이 없음
- **확인만 함 (문제 없음)**
  - 열자마자 닫기: `E_ABORT` 완료 콜백이 dtor보다 먼저 옴. dtor 이후 콜백 없음
  - 화면 하나 닫고 다시 열기: env 재사용
  - 연속 클릭: 서버 TR을 한 번 거치므로 탐색이 겹치지 않음
  - 폰트 콤보 동작

### 다음 할 일

- 위 [다음 할 일 (누적)](#다음-할-일-누적)의 IB771000 두 묶음 (Edge 회귀 확인 → 브릿지 서버 작업 → 동시 배포)

---

## 2026-11-09 (월)

### 한 일

**IB771000 종합시황 — 브릿지경제 추가** (`MapDlg.cpp` CP949, +19/−10)

- `tab\newstab`: `COUNT=17`, `[NEWS_17]`(브릿지경제, GUBN=18 가정), `[BULRU_17]`(뉴스웨이와 같은 8분류) 추가 → UI 확인
- `VERSION` 4→5, 이력 로딩 조건 `atoi(version) >= 2` (기존엔 4 누락 → v4 사용자 이력 무시, 매번 전체 체크되던 버그 함께 해소)
- 버전 4 이하 사용자는 브릿지경제 자동 체크
- 뉴스웨이 미조회 버그 수정: 서버는 마스크 비트로 조회 범위를 정하는데 "전체"/"언론사별" 마스크에 idx16 비트가 빠져 있었음 → 수정 후 조회 확인
- 브릿지 idx17 비트까지 반영: 전체/카테고리 `11111101111111111100`(3878/3923/4033/4043/4194), 언론사별 `10111101111111111100`(3963). 4194의 "전체" 판단 비교값은 카테고리 루트 마스크와 일치해야 함
- 서버에 아직 없는 idx17 비트를 보내도 문제없음 확인 (서버는 뉴스웨이까지만 읽고 나머지 비트는 무시)

**IB771000 — 하단 Edge(WebView2) 뷰어 점검**

- `BrowserWindowEdge` 콜백 구조 점검: 방어 로직은 파일 미존재 시 `TM_HTMLFILE` 2초 1회 재시도뿐
- 의심 지점 4개 도출: ① env 미준비 시 조용히 skip ② 컨트롤러 생성 중 재진입 → 중복 생성 ③ 탐색 실패 무처리 ④ ProcessFailed 복구 코드 주석 처리
- `[edge]` 진단 로그 9곳 추가 (BrowserWindowEdge.cpp ①~⑦, MapDlg.cpp TM_HTMLFILE else / selectTab IsWindowVisible else), CP949·diff 검증
- 첫 로그로 **②번 재현 확인**: 화면 열기 `Init(about:blank)` 생성 완료 전에 `selectTab`의 첫 기사 `Init` → `create controller request` 2회, 두 번째 완료 시 `old≠0`으로 덮어씀

### 다음 할 일

- 위 [다음 할 일 (누적)](#다음-할-일-누적)의 IB771000 항목 (서버 작업 → 조회·버전 이행 테스트 → 동시 배포)

---

## 2026-11-06 (목)

### 한 일

**AxisAgent** — 상세: [AxisAgent.md](AxisAgent.md) 과제 목록 A-4~A-30, 변경 이력

- A-5 로그 스레드 안전: 대기열 + `PostMessage`(`m_bLogPosted`), UI 스레드에서만 리스트 갱신, 중지 해제 시 다시 표시
- A-7 크래시 덤프 분석: 예외 순간 컨텍스트로 StackWalk → `CMainWnd::OnCreate`부터 `initShared`까지 소스 줄 단위로 풀림
- 덤프-화면 대조(A-25): HTS `SetLastMaps`에서 `[MAP] open/close/change | now=[…]` 전송, 덤프 ini에 `MAPS=`
- 덤프 ini에 `TRIGGER=`(사유, MAIN/CPU, 카운트) (A-26)
- 덤프 정책(A-27): CpuSpike는 30초 연속일 때만, `cpudump`(기본 1), 첫 `[MAP]` 후 60초는 CpuSpike·Hung 덤프 안 함, NoResponse는 항상
- `[M5]` 로그에 `MAIN/CPU/cnt=n/30`, 연속이 끊기면 `정상 복귀`
- `dump_analysis_YYYYMMDD.ini` 날짜별 + 7일 삭제 (A-28)
- UI(A-29): SymPath 상단 이동, 리스트 더블클릭 상세보기(RichEdit), 버튼 리사이즈 등록, SymPath 직접 입력 저장
- A-4, A-11은 코드상 이미 해소된 것 확인
- 덤프 해석 사례: IB201100 열기 직후 4초 CPU 부하, 시작 시 화면 복원 부하(`NtGdiSaveDC` ← `CfmBase` 생성자의 `setFontSize` GDI 호출)
- 관찰(A-30): form DLL `CfmBase::getDC()`의 `while (dc == nullptr)` 무한 루프 위험

**기타**

- 저장소 `.gitignore`에 `*.dmp`/`*.DMP` 추가
- IB771000 브릿지경제 추가 사전 분석: 트리는 `tab\newstab`으로 동적 생성, 기존 사용자 자동 체크에는 코드 수정 필요 (위 "다음 할 일" 참고)

### 다음 할 일

- 위 [다음 할 일 (누적)](#다음-할-일-누적) 참고
