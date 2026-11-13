---
project: ibks
category: worklog
status: 진행중
updated: 2026-11-12
---

# 작업 일지 (하루 성과 / 다음 계획)

## 목차

- [사용법](#사용법)
- [다음 할 일 (누적)](#다음-할-일-누적)
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
- [ ] **DataGuard.dll 모듈화** (`ibks/DataGuard/DataGuard/`, MFC 일반 DLL·자동화 없음·MultiByte, 출력 `src/release/DataGuard.dll`)
  - [x] 1단계 프로젝트 생성·빌드, 2단계 공용 헤더 `ibks/h/DataGuardApi.h` + include
  - [ ] 3단계 `Guard.cpp` 추가(세션 저장소·규칙 테이블·Check·`DG_GetApi`) → 빌드 → `dumpbin /exports`로 `DG_GetApi` 확인
  - [ ] 4단계 Wizard 연결: CGuard에서 로드(전체 경로, 로드 실패 시 fail-closed 여부 결정) → `AccHash*` 호출을 `SessionAdd/Remove/Clear`로 교체 → Stream.cpp `[ACCCHK]` 인라인을 `Check()` 한 줄로 교체(BLOCK이면 `m_sndL -= L_axisH` 롤백)
  - [ ] 5단계 서비스 I/O 기준 규칙: SAMFQ014 본문 계좌 offset 확인 → 규칙의 `228`을 본문 offset으로 교체(맵 수정 불필요화). 서버팀 I/O 명세가 파일로 있는지 확인
  - [ ] 2차: 세션 데이터 종류 확대(USERID 등), CTRL/DLL 송신 경로 적용, 수신 검증(`DG_RECV`), DLL 무결성 확인(서명/해시), 해시 재생성 경로를 로그인으로 제한(`AddRegistry(datB,datL)` Dll.cpp:116·Event.cpp:372), 직원 단말 예외
  - [ ] 운영 전: `[DG] check`·`[ACCCHK]`·`[ACCHASH]` 로그의 계좌 평문 마스킹/제거, Guard.cpp LOG_OUTP 비밀번호 출력(1926/2094/2157) 제거
- [ ] 보호 대상 서비스(타깃 화면) 선정 — 아직 미정
- [ ] 서버팀 한 줄 확인: fil2 뒤쪽 12바이트(228~239) 클라이언트 사용 가능 여부, `term`/`usid`/`pbid`를 요청값으로 믿는지
- [ ] 평문 주문 서비스 확인: IB820899 `SAMDT002`, IB170900 `SAMFQ014` (둘 다 `odrf=1`, `OP_ENC` 없음) — 맵 옵션 누락 여부
- [ ] `svcd` 기준 평문 송신 목록 수집: `grep -A1 LEDGERCHK | grep -B1 "enc(statENC)=0"`

### 티커 뉴스 — 브릿지경제 추가 (axiscm + sm)

- [ ] `DLL/axiscm/AxCommon.cpp:709` `newsSource[]`의 `"뉴스웨이",` 뒤에 `"브릿지경제",` (반드시 맨 뒤 — 저장값=목록위치+1)
- [ ] `DLL/sm/Pane.cpp:3468` 다음 줄 `case 16: index = 18; break;` (저장값 16 → GUBN 18, GUBN은 7710과 같은 가정)
- [ ] 설정 화면(`axiscp` `SetTickInfo`) 목록 16개 표시 확인, `axiscm.dll`+`sm.dll` 동시 배포

### IB771000 종합시황 — 하단 Edge(WebView2) 뷰어 방어 로직

- [ ] 수정 후 회귀 확인 1회: 화면 열기 / 기사 클릭 / 프로세스 종료(kind=0·1) / 인쇄 실제 출력
- [ ] env 미준비 재시도 경로(`env NOT ready -> retry`) 확인: PC 재부팅 직후·저사양 PC 첫 실행 (배포 차단 사안 아님)
- [ ] 배포: `BrowserWindowEdge.cpp/.h` + `MapDlg.cpp` (브릿지 작업과 같은 DLL이므로 newstab과 함께 나감)
- [ ] 참고(범위 외): `IsProgramInstalled`가 한글 표시명 비교(영문 OS면 IE로 감), `NavigationStarting`이 http:// 링크 차단, `Resize()` 콤마 연산자, `Navigate()`의 무의미한 Event 대기, env 생성 실패 시 IE 폴백 없음, `MapDlg`의 `AfxMessageBox("edge create fail")`, kind=0 화면 2개일 때 env 2회 생성(무해)

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
