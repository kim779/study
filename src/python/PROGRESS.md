# IBK 증권 OpenAPI Python 테스트 프로젝트 진행 사항

## 프로젝트 개요

IBK증권 OCX 커넥터(`ibkconnector.ocx`, GUID `{CDADD338-C7AB-4977-B65D-8E988B5958E3}`)를
PyQt5 `QAxWidget`으로 제어하는 테스트 프로그램.

- **파일**: `src/python/test_ocx.py`
- **빌드 결과**: `src/python/dist/test_ocx.exe`
- **서버**: 211.255.204.75:15201

---

## 완료된 작업

### [2026-06-28] 기본 UI 및 OCX 연동 완성

#### 구현 완료 기능

| 기능 | TR | 상태 |
|------|----|------|
| 초기화 | `Initialize()` | ✅ |
| 로그인/로그아웃 | `Login()` / `Logout()` | ✅ |
| 계좌 목록 조회 | `GetAccounts()` | ✅ |
| 주식 시세조회 | TR1001 (key=1) | ✅ |
| 선물옵션 시세조회 | TR3001 (key=14) | ✅ |
| 주식 주문 | TR1201 (key=4) | ✅ |
| 선물옵션 주문 | TR3201 (key=17) | ✅ |
| 주식 잔고조회 | TR1221 (key=8) | ✅ |
| 선물옵션 잔고조회 | TR3221 (key=20) | ✅ |
| 주식 체결조회 | TR1211 (key=7) | ✅ |
| 선물옵션 체결조회 | TR3211 (key=19) | ✅ |
| 실시간 시세 수신 | `OnRealData` | ✅ |
| 이진 패킷 파싱 | `OnRecvData` + ctypes | ✅ |

#### OCX 이벤트 처리

| 이벤트 | 처리 내용 |
|--------|-----------|
| `OnLogin` | 로그인 결과 표시, 계좌 콤보 자동 로드 |
| `OnError` | 로그에 오류 메시지 출력 |
| `OnClose` | 연결 종료 상태 표시 |
| `OnAlert` | 알림 코드/메시지 출력 |
| `OnGuideMsg` | 안내 메시지 출력 |
| `OnRecvData` | TR별 바이너리 파싱 (ctypes.Structure) |
| `OnRealData` | 탭구분 실시간 데이터 파싱 |

#### UI 구성

- Initialize 그룹: 초기화 버튼 + 시장구분 콤보 (주식/선물옵션)
- Login 그룹: 계좌/비밀번호/서버IP/포트 입력 + 계좌 콤보
- 시세조회 그룹: 종목코드 입력 + 실시간 시세 표시 (현재가/전일대비/등락률/거래량/시가/고가/저가/미결계약/호가잔량)
- 주문 그룹: 계좌/비밀번호/종목코드/매매구분/호가유형/가격/수량/원주문번호 + 잔고조회/체결조회
- 로그 그룹: 타임스탬프 로그 + OutputDebugStringW

#### 바이너리 패킷 레이아웃

```
헤더: acno[11] + nrec[4] = 15바이트
그리드 크기:
  TR3001 = 가변(모드 구조체)
  TR1221 = 194바이트/건  (주식 잔고)
  TR3221 = 149바이트/건  (선물 잔고)
  TR3211 = 179바이트/건  (선물 체결)
  TR1211 = 222바이트/건  (주식 체결)
```

---

## [2026-06-28] 개발환경 정비 (노트북 ↔ 다른 PC git 동기화)

### symstore PDB 관리

- **문제**: 빌드후 이벤트의 symstore 경로가 PC마다 달라 git 충돌
- **해결**: PostBuildEvent를 `.vcxproj`에서 제거 → 각 PC의 `.vcxproj.user`로 이동
  - `.vcxproj.user`는 `.gitignore`의 `*.user` 규칙으로 git 제외
  - 대상 프로젝트: `IBKSConnector`, `TestDlg`, `IB202200`
- **symstore 경로**: `C:\Program Files (x86)\Debugging Tools for Windows (x86)\symstore.exe`
- **심볼 저장소**: `D:\symstore` (없으면 수동 생성 필요)
- **WinDbg 심볼 경로**: `.sympath srv*D:\symcache*D:\symstore;srv*D:\symcache*https://msdl.microsoft.com/download/symbols`

### NuGet 패키지 경로 문제 (TestDlg)

- **문제**: `NuGet.Config`에 다른 PC 경로 하드코딩(`C:\Users\IBKS\...`) → 노트북에서 패키지 못 찾음
- **해결 1**: `NuGet.Config`를 nuget.org 소스로 교체
- **해결 2**: 패키지 실체는 `AXIS\packages\`에 있고 vcxproj는 `../packages/` 참조
  → `TestDlg-b_TestDlg\packages` → `AXIS\packages` junction 생성으로 해결
  ```
  mklink /J "F:\src\IBK\src\TestDlg-b_TestDlg\packages" "F:\src\IBK\src\AXIS\packages"
  ```
  ※ 다른 PC에서 pull 후 동일하게 junction 생성 필요 (gitignore로 동기화 안 됨)

---

## 관련 C++ 프로젝트 현황

### IBKSConnector_test (MFC OCX 본체)

- 위치: `src/HTS_OpenAPI(운영)/IBKSConnector_test/`
- **최근 변경** (uncommitted):
  - `IBKSConnector_h.h`: MIDL 컴파일러 버전 8.01.0622 → 8.01.0626 업데이트
  - `DECLSPEC_XFGVIRT` 매크로 추가 (Control Flow Guard XFG 지원)
  - `IBKSConnector_i.c`: 동일 버전 업데이트

### AxisChaser (MFC 뷰어)

- 위치: `src/AxisChaser/`
- MFC SDI 텍스트 뷰어 (RichEdit 기반)
- axis 통신 헤더 사용 (`axishdr`, `_exeCDSS`)

---

## 미완료 / 향후 작업

- [ ] 실시간 구독 등록/해제 UI (현재 TR 요청 시 자동 구독만)
- [ ] 체결/잔고 조회 결과를 테이블 위젯으로 표시
- [ ] 다중 종목 실시간 모니터링
- [ ] GREEKS 데이터 (TK_GREEKS1=150, TK_GREEKS2=151) 처리
- [ ] GetCode (TK_GETCODE=32) 종목코드 조회 기능
- [ ] PyInstaller 빌드 검증 (test_ocx.spec)

---

## 메모

- `GetAccounts()` 반환: `계좌번호\t계좌명\n...` (계좌번호 11자리 슬라이싱)
- 주식 실시간 코드에 `A` prefix 붙어 옴 → `lstrip('A')` 후 비교
- `dptr` 포인터는 콜백 반환 후 해제되므로 `ctypes.string_at(addr & 0xFFFFFFFF, size)`로 즉시 복사 필수
- Python 환경: PyQt5 + pywin32 (32비트 필수, OCX가 32비트)
