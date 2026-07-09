# Architecture

- 생성일: 2026-07-07 / 최초 분석 갱신: 2026-07-07
- 목적: src_7_11 프로젝트의 DLL 관계, Layer/Module 구조, Thread 구조, Message Flow를 기록한다.
- 분석 범위: appl, h, rm, axis, control, dll, platform (전체 1,624개 대상 파일, 9개 배치로 분석)

## 전체 개요

src_7_11은 IBKS 증권 거래 플랫폼(Windows/MFC/COM 기반)의 소스 트리로, 다음과 같은 최상위 모듈로 구성된다.

| 모듈 | 성격 | 파일 수(추정) |
|---|---|---|
| axis | 메인 UI 애플리케이션 (MDI EXE) | 178 |
| control | 화면 컨트롤 DLL 8종 (COM/ActiveX) | 203 |
| dll | 코드조회/로그인/암호화/UI 컴포넌트 DLL 군 | 404 |
| dll/axissm | 거래 화면 UI 컴포넌트 DLL (5계층) | 168 |
| platform/builder | 화면(Map) 빌더 도구 (awBuild/awDlg/Components 등) | 729 |
| platform/dll | Form/Graphics/Script Engine DLL (axisform, axislib, axisvbs) | 191 |
| platform/wizard, sock | 화면 렌더링·통신 OCX 컨트롤 | 89 |
| appl | 개별 업무 화면 DLL (이자, 채권 등) | 49 |
| h | 공용 프로토콜/도메인 헤더 | 29 |
| rm | 리소스 관리 유틸리티 | 32 |

전체적으로 axis(메인 EXE) → dll/axissm, dll/*, platform/dll, platform/wizard 등 다수의 DLL을 로드하여 동작하는 **플러그인형 MDI 증권 거래 시스템** 구조이다. 화면 정의는 platform/builder에서 만든 Map 문서를 axisvbs(Script Engine)/axisform(Form Control)이 런타임에 해석하여 렌더링한다.

---

## 1. appl + h + rm

### 계층 구조 (3-Tier)
**Application Layer (appl)**
- TB205001 (이자/금리): MFC DLL, Export `axCreate`, 클래스 10+개 (CInterestPanel, CNButton, CMapWnd, CPanel, CGpEdit, CGroupNameDlg, testwnd)
- TB900100 (채권/Unbond): MFC DLL, Export `axCreate`, 클래스 8+개 (CUnBond, CNButton, CPanel, CMapWnd, SEdit, CXLAutomation, CXLEzAutomation)

**Domain/Data Layer (h)**
- 30개 헤더파일, 136개 구조체 정의
- 금융: jmcode.h(증권), comcode.h(상품), tjmcode.h(T-형 증권)
- 통신: axis.h(AXIS 프로토콜), axisfire.h(런타임 이벤트), axisrsm.h(리소스 메시지)
- UI: form.h(Form Control), Grid.h, axisgenv.h(45+ Chart 구조체), axisgwin.h, axismgrp.h
- 설정: menudef.h, controlMSG.h, UserDefineMessage.h

**Resource Manager (rm)**
- axisrm: MFC Application, 클래스 5개 (CAxisrmApp, CCommLine, CResDlg, CTransDlg, CAboutDlg)
- 기능: 리소스 업로드/다운로드, 파일 전송, 다중 플랫폼(Win32, x64)

### 동시성
- CCriticalSection 1개 (CUnBond.m_section), 스레드: h_thread/h_Allthread (CUnBond), 함수: FillListdata_thread(int), FilAllList_thread()
- 용도: UI 블로킹 방지, 비동기 리스트 데이터 로딩. 위험도 중간(생성/해제 코드 미발견, 동기화 범위 불명확)

### COM/OLE/MFC
- Excel Automation (CXLAutomation, CXLEzAutomation), Event Sink(DECLARE_EVENTSINK_MAP)
- Message Map 24회 사용, CListCtrl/CDateTimeCtrl/CIPAddressCtrl/CProgressCtrl 등 사용

### 디자인 패턴 (추정)
Singleton(CAxisrmApp 등 CWinApp), Command(axCreate export), Observer(Message Map/Event Sink), Wrapper(CXLAutomation), MemDC(CAxMemDC)

### 구조적 문제
1. h/ 와 rm/h/ 헤더 중복(약 18개) — 싱크 문제 가능성
2. 스레드 관리 불명확 (CCriticalSection 1개로 다중 배열 보호 여부 미확인)
3. Excel Automation 예외 처리 미확인
4. CXLAutomation GlobalLock/Unlock 쌍 매칭, CArray 생명주기 확인 필요

(세부 분석은 이번 세션의 임시 스크래치 파일에서 병합됨 — 영구 보관 경로 아님)

---

## 2. axis (메인 UI 애플리케이션)

MFC MDI 기반 증권 거래 UI 애플리케이션. 177개 파일(cpp 78, h 99), 클래스 ~100개.

### 계층 구조
```
CAxisApp
  └─ CMainFrame (MDI Frame, ~730행, 200+ 멤버변수 — God Class)
      ├─ CAxisDoc/CAxisView 계층 (CAxTabView, CAxScrollView)
      ├─ CChildFrame (MDI Child), CSChild (SDI Frame)
      ├─ CFormManager (화면 동적 생성 Factory)
      ├─ Dialogs (CConnect, CPassDlg 등 25-30개)
      ├─ Pop-ups (CGPop, CMPop)
      ├─ UI Components (CNTab, CNTable, CLinkbar 등 50+개)
      └─ CNSocket, Crypt, axMisc 등 지원 클래스
```

### Message Flow
1. `CAxisApp::InitInstance()` → AfxSocketInit() → CoInitialize() → AfxOleInit() → CMainFrame 생성/로드 → `pMainFrame->Start(m_user)`
2. `CConnect`(로그인) → 서버 연결 검증 → 진행률 표시
3. `CNSocket` → OnConnect/OnReceive(m_flow, m_step 상태머신)/OnClose
4. `CMainFrame` 메시지 처리: BEGIN_MESSAGE_MAP, Custom handlers(OnAXIS, OnUSER, OnCHILDMSG, OnCHASER), Event Sink Map(COM)

### Thread & 동시성
- Main Thread(메시지 루프), Async Socket Thread(CNSocket 콜백), Timer(CMainFrame::OnTimer)
- 동기화: CCriticalSection(m_sync, CUseCriticalSection RAII), Mutex(CAxInstance 중복 실행 방지)
- 잠재 경쟁 상태: CMainFrame 다수 멤버가 UI/소켓 콜백 스레드에서 동시 접근, 명시적 동기화 제한적, PostMessage로 일부 완화

### COM/OLE/MFC
- CoInitialize/AfxOleInit, IDispatch(CImpIDispatch), Event Sink Map, IWebBrowserApp

### 디자인 패턴
Singleton(CAxisApp, CAxInstance), Factory(CFormManager, GetNewView), Observer(Message/Event Sink Map), Command(OnCommand/funcKey), Callback(IExceptionHandler, CNSocket), RAII(CUseCriticalSection), Strategy/State Machine(CNSocket 수신 프로토콜 STX/ETX), Bridge(CImpIDispatch), Facade(CMainFrame)

### 구조적 문제
1. **God Class**: CMainFrame(~730행, 200+ 멤버) — 책임 과다(UI/이벤트/통신/설정/세션)
2. 강한 결합: 대화상자/컴포넌트 직접 소유
3. 약한 응집도: 비즈니스 로직-UI 로직 혼재
4. 순환 Include 가능성(MainFrm.h가 다수 헤더 포함, 대화상자가 다시 MainFrm.h 포함)
5. 메모리 누수 가능성: 포인터 기반 관리, 스마트 포인터 미사용
6. 동시성: 소켓 콜백-UI 스레드 데이터 공유, 명시적 동기화 부족

(파일별 클래스 목록 등 세부 내용은 이번 세션의 임시 스크래치 파일에서 병합됨)

---

## 3. control (화면 컨트롤 DLL 8종)

모든 8개 DLL이 동일한 COM/ActiveX + MFC 제어 프레임워크를 공유한다.

```
[COM Interface Layer] DllCanUnloadNow/DllGetClassObject/... , axCreate(CWnd*, void*) -> CWnd* (Factory)
        v
[MFC Control Layer] CControlwnd/CControlWnd(CWnd), DECLARE_MESSAGE_MAP/DISPATCH_MAP/INTERFACE_MAP, 자동생성 IDL
        v
[Utility/Helper Layer] CMemDC(더블버퍼링), CParam, 암호화/파일전송/데이터 모델
```

| DLL | 역할 | 주요 클래스 | 특징 | 스레드 |
|---|---|---|---|---|
| fx_gridEX | 고급 그리드 | CfxGrid, CfxIGrid, CMemDC | 셀 병합, Excel 내보내기, OLE 드래그드롭 | 없음 |
| CX_FilsSearch | 파일 검색 | CControlwnd+IDispatch | CFileDialog 래퍼 | 없음 |
| CX_POP | 팝업 정보 | CPopInfo(CDialogEx), CMTable | 슬라이드 애니메이션, Timer(100ms) | 없음 |
| fx_codectrl | 코드 입력 자동완성 | CCodeEdit(CEdit+Thread), CSubList | 자동완성 | **CWinThread + HANDLE Event** |
| CX_griddata | 금융 데이터 그리드 | CControlwnd+CCodeData | _ledgerH/_balst 구조체 | 없음 |
| CX_FileRead | 파일 읽기 | CControlwnd | 파일 I/O 래퍼 | 없음 |
| cx_fileupdown | 파일 업/다운 | CControlwnd+Crypt | SEED/SHA256/Base64, 40KB 청킹 | 암시적(m_bSending 플래그) |
| fx_AGdlg | AG 주문 대화상자 | CControlWnd | - | 없음 |

### 동시성
- fx_codectrl만 명시적 스레드(CWinThread + HANDLE Event, 뮤텍스 아님) — 공유 상태 경쟁 가능성
- cx_fileupdown은 m_bSending 플래그 기반 비동기 모델 — 원자성 보장 안 됨
- 나머지 6개는 단일 스레드 + Timer

### 구조적 문제
동기화 부족(fx_codectrl, cx_fileupdown), 파일전송 재시도/타임아웃 미구현, 하드코딩 상수(maxMERGE=20, ChunkSize=40KB), 로케일 미지원

---

## 4. dll/axissm (거래 화면 UI 컴포넌트, 5계층)

168개 파일, 클래스/구조체 150+개. axissm.dll은 MFC Extension DLL 기반 UI 컴포넌트 라이브러리.

```
Layer 1 Foundation   : CGlobal (비트맵/색상/팔레트/폰트 리소스 Singleton)
Layer 2 Basic Ctrl   : CAxButton, CAxStatic, CAxScrollBar, CHBand
Layer 3 Extended Ctrl: CXedit, CXComboBox, CXListCtrl, CXDateBox, fxSpin
Layer 4 Dialogs      : CTSetup, COrderSetup, CLink, CHotkey, CMenuSearch
Layer 5 Main Comp    : CBb(보드), CSmain(거래메인바), CTMenu(메뉴바), CoolDialogBar, Dbar0/1/2
```

### 동시성
- iMenu.cpp: `AfxBeginThread(create_LinkFile, this)` — 링크 파일 생성 스레드
- 명시적 CriticalSection/Mutex 미사용 → CGlobal(static) 스레드 안전성 미보장

### 디자인 패턴
Callback(axiscall 함수포인터), Global Resource Manager/Singleton(static CGlobal), Adapter(CXedit 등), Template Method(CSetupDialog 파생), Composition(CBb), Factory(make_Ctrl 추정)

### 구조적 이슈
전역 상태(static CGlobal) 경쟁 위험, 헤더 절대경로(../../axis/, ../../h/) 의존, 깊은 상속, RAII 미사용

---

## 5. dll (axissm 제외 — AxisCode/ctrl/login/xecure/axisDialog/axiscodx/axiscore/RushTester)

~220개 파일, 클래스/구조체 ~110개.

```
Application (RushTester, axiscore)
  v
Business Logic (AxisCode 코드검색, axiscodx 마스터, login 인증)
  v
UI Component (axisDialog, ctrl: Tree/List/Combo)
  v
Security (xecure: LEA 암호화)
```

### 동시성
Message-loop 기반 동기 모델. 명시적 Thread 생성 미검출(CreateThread/AfxBeginThread 없음). afxmt.h 포함되나 미사용 관찰.

### COM/OLE
ActiveX(CXecureCtrl/COleControl), Automation(RushTester Excel wrapper, login CCmdTarget, xecure DllGetClassObject)

### 디자인 패턴
Singleton(CGlobal in AxisCode), Factory(void* 핸들 export), Template Method(CDialog 파생), Observer(WndProc), Adapter(CXecureCtrl→LEA C API), Facade(CXLAutomation), MVC-like, Command(stConnectInfo/_FCC/_RCC/_SCC)

### 구조적 문제
God Class(CGlobal), void* 핸들 타입 안전성 부족, Include 경로 불일관, Export 정의 방식 혼용(.def/__declspec), 비동기 미지원(I/O 블로킹 가능)

---

## 6. platform/builder/awDlg + Components

129개 파일, 클래스 ~78개. UI 컴포넌트 라이브러리(Dialog/PropertySheet 계층 + 고급 MFC 컨트롤).

- **awDlg**: CDialog 파생(28개: CAddrDlg, CLogin, CPreview 등), CPropertyPage(4), CPropertySheet(2: COptionsSheet, CMapSheet), CWnd 파생(4)
- **Components**: CListBox/CComboBox/CEdit 파생(7), CListCtrl 파생(3), CTreeCtrl 파생(5: CTreeFileCtrl, CServerTreeCtrl 등), CMenu(BCMenu), CRichEditCtrl 파생(3), CCrystalTextView(Crystal Edit 프레임워크, Open-Source)

### 동시성
- FileTreeCtrl: `AfxBeginThread(MonitoringThread,...)` + CEvent(m_TerminateEvent) — 파일 변경 모니터링
- ServerTreeCtrl: CCriticalSection m_section

### COM
FileTreeCtrl만: IMalloc, IShellFolder (Shell API, 파일 아이콘/속성 조회)

### 디자인 패턴
Observer(FileTreeCtrl 모니터링), Command(MFC 메시지맵 전체), Decorator(추정: ColorCombo 등), Singleton(추정: CSystemImageList), MVC-like(OptionsSheet+PropertyPage), Factory(추정: FileTreeCtrl GetIconIndex)

### DLL 의존성
awdlg.dll → awsock.lib, awbuild.lib, awwcc.lib, awcommon.lib, axislib.lib, awobject.lib, awuser.lib

### 기술 부채
VC60 레거시 props, Crystal Edit 라이선스 확인 필요, MFC OLE 헤더 실사용 여부 확인 필요

---

## 7. platform/builder (awDlg, Components 제외 — awBuild/awCommon/awWcc/awUser/awSock/awObject/awTool)

95개 파일, 클래스 ~55개.

```
Tier 1 Core     : awWcc(CCompile 스크립트 컴파일러) -> awBuild(CMapLoad Map 파서)
Tier 2 UI Tools : awTool(Ctl* 19개 도형 클래스), awCommon(유틸)
Tier 3 Business : awUser(사용자 객체 UI), awSock(CwSock 소켓), awObject(CShow 렌더링)
```

### 동시성
Thread 생성 2곳(PreViewBar CWinThread, FileTreeCtrl AfxBeginThread), CCriticalSection 2곳(LoadThread.h, ServerTreeCtrl.h). 115개 DECLARE_MESSAGE_MAP.
**위험**: awSock/CwSock의 OnFireEvent가 콜백 스레드에서 실행 — UI 스레드 크로스 처리 불명시

### COM/MFC
CwSock: DECLARE_EVENTSINK_MAP(Winsock 콜백), 8-state State Machine(stIDLE, stCONNECTED 등), AFX_EXT_CLASS export 90개

### 디자인 패턴
Singleton/Application(CAwBuildApp 등), Facade(CMapLoad), Strategy(Ctl* 19개 동일 인터페이스), State Machine(CwSock), Observer(Message Map), Template Method(CCompile::mapTokenParsing), Factory(CShow DECLARE_DYNCREATE)

### 코드 품질 이슈
xTreeCtrl 중복 정의(awCommon ↔ awSock), 스레드 안전 미흡(awSock 콜백-UI 크로스), 외부 struct 강결합(../../h/), HANDLE 명시적 해제 필요, 예외처리 미사용(bool/HRESULT 반환만)

설계 의도: Map 문서 기반 UI 빌드 시스템(awBuild/awWcc가 핵심)

---

## 8. platform/dll (axisform / axislib / axisvbs)

~90개 파일, 클래스 ~55개. 3개 MFC Extension DLL.

1. **axisform.dll (Form Control Layer)**: CAxisForm(CCmdTarget) 통합관리자 + 50+ 컨트롤(CfmBase→CfmEdit/CfmButton/CfmGrid/CfmTab 등 다형성), Cbrowser(IE CHtmlView)/CbrowserX(WebView2). Dispatch 자동화(DISP_PROPERTY_EX/DISP_FUNCTION). 동시성 없음.
2. **axislib.dll (Graphics Utility)**: CAxisDraw(그래픽 객체 풀: m_penObs/m_brushObs/m_pointFontObs/m_bmpObs), CAxisPalette. **동시성 보호 없음(위험)**.
3. **axisvbs.dll (Script Engine)**: VBS(CScriptEngine/COleDispatchDriver + CScriptSite/IActiveScriptSite, CoCreateInstance(IID_IActiveScript)), Python(CPythonEngine, Python.h C API, PyObject* 수동관리), CEngineWrapper(자동 언어 선택), CVbGuide(CWinThread, 에러 시 MB_SYSTEMMODAL)

### 디자인 패턴
Factory(CAxisForm DYNCREATE), Strategy(CEngineWrapper), Object Pool(CAxisDraw), Adapter(Cbrowser/CbrowserX), Wrapper/Facade(CScriptEngine/CPythonEngine), Bridge(CScriptSite↔CScriptEngine)

### 구조적 위험요소
1. **CAxisDraw 동시성 보호 부재(높음)**: 여러 폼의 동시 SetFont/SetBrush 호출 시 Race Condition 가능
2. Python C API 메모리 관리(중간): PyObject* 수동 관리, Py_DECREF 누락 위험
3. "AxisCodx.dll" 문자열 하드코딩(낮음~중간)
4. COM 에러 체크 미흡(낮음), CVbGuide MB_SYSTEMMODAL로 전체 시스템 모달(낮음)

---

## 9. platform/wizard + sock + h + h_우선백업 + ver + rm

150개 파일, 클래스 ~60개, 구조체 ~120개.

```
UI/Application (ver.exe, rm.exe, OCX 호스트)
    v
Business Logic (CGuard - 핵심 조정자, 340+ 메서드, God Class)
    - CWorks, CClient, CScreen, CScript(VBS 엔진), Cast/Dde/Dll/mapping 등
    v
Network Layer (CStream/TCP, CDgram/UDP, CWsh/Worker Thread)
    v
Protocol & Data (h/ - 50+ struct)
```

- **wizard(axWizard.ocx, 56파일)**: OCX ActiveX, CGuard(God Class, 400+ 메서드) — 화면 렌더링/데이터 바인딩/트리거·프로시저/세션 관리. 의존: axislib.lib, axisform.lib, axisvbs.lib
- **sock(axSock.ocx, 27파일)**: CStream(TCP), CDgram(UDP), CWsh(Worker Thread) — 메시지 큐 기반 비동기 처리
- **h/(21파일)**: axis.h(_axisH 프로토콜), form.h, Graph.h, common.h — 순환 의존성 없음
- **h_우선백업/(18파일)**: 백업/미사용 추정
- **ver, rm**: 버전/리소스 관리 EXE

### 동시성
CWinThread(CWsh Worker), Windows ThreadPool API(CThpool), AfxBeginThread(레거시) 혼용. CCriticalSection 12+(Guard의 m_sync/m_section/m_fsection), CEvent 1개(CWsh). **Lock 순서 역전(deadlock) 위험**.

### COM/MFC
wizard/sock: IDispatch(ActiveX), OleDropEx(IDropTarget 직접구현), 4종 export(DllCanUnloadNow 등). 링크: ws2_32.lib, imm32.lib, version.lib, winmm.lib, axislib/axisform/axisvbs.lib

### 디자인 패턴
Module(OCX 독립모듈), Facade(CGuard/CSockCtrl), Observer(EventMap), Thread Pool(CThpool), Producer-Consumer(CMQue+CWsh), Singleton(추정: CWizardApp theApp)

### 주요 위험요소 (우선순위)
1. **God Class(CGuard)** — 심각. 로그인/화면/데이터/세션/트리거/프로시저/Excel/클립보드 등 과도한 책임, 340+ public 메서드. 리팩터링 1순위 후보.
2. 강한 결합도 — 심각. Guard→CClient/CScreen/CWorks/CDde/CObjects 의존성 체인
3. Thread Safety 검증 필요 — 중간. CCriticalSection 3종 용도 불명확, Lock 순서 미정의
4. RAII 미준수 — 중간. Smart Pointer 미사용, 직접 new/delete
5. 스레드 모델 혼용 — 낮음. ThreadPool + AfxBeginThread 동시 사용

---

## 종합 리팩터링 후보 (우선순위)

1. **CGuard (platform/wizard)** — God Class, 340+ 메서드. Authentication/FormManager/SessionManager 등으로 분할 검토
2. **CMainFrame (axis)** — God Class, ~730행/200+ 멤버
3. **CGlobal (dll/axissm, dll/AxisCode)** — 여러 모듈에서 동일 이름의 전역 리소스 매니저 반복 등장, 스레드 안전성 미보장
4. **CAxisDraw (platform/dll/lib)** — 그래픽 객체 풀에 동시성 보호 없음
5. **xTreeCtrl 중복 정의 (platform/builder/awCommon ↔ awSock)** — 동일 코드 중복

## 다음 분석 단계 (권장)
- Call Graph: 진입점(WinMain/DllMain)→ThreadProc/Socket Callback/Message Handler 추적 (call-graph 스킬)
- Architecture Review: 위 God Class/강결합/동시성 리스크에 대한 세부 리뷰 (architecture-review 스킬)
- SourceIndex 세부화: 현재는 모듈/배치 단위 요약이며, 파일 단위 전체 표는 추후 보강 필요
