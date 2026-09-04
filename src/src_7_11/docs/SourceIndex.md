---
project: src_7_11
category: source-index
status: 작성됨
updated: 2026-08-21
---

# SourceIndex


## 목차

- [appl + h + rm](#appl-h-rm)
- [axis (메인 EXE)](#axis-메인-exe)
- [control (8개 화면 컨트롤 DLL)](#control-8개-화면-컨트롤-dll)
- [dll/axissm (거래 화면 UI 컴포넌트, 5계층)](#dllaxissm-거래-화면-ui-컴포넌트-5계층)
- [dll (axissm 제외)](#dll-axissm-제외)
- [platform/builder/awDlg + Components](#platformbuilderawdlg-components)
- [platform/builder (awDlg, Components 제외)](#platformbuilder-awdlg-components-제외)
- [platform/dll (axisform / axislib / axisvbs)](#platformdll-axisform-axislib-axisvbs)
- [platform/wizard + sock + h + h_우선백업 + ver + rm](#platformwizard-sock-h-h_우선백업-ver-rm)
- [통계 요약](#통계-요약)
- [다음 단계](#다음-단계)

---

- 생성일: 2026-07-07 / 최초 분석 갱신: 2026-07-07
- 목적: cpp/h 파일 단위 역할, 주요 클래스/함수, 관련 파일, 사용 API, 의존 모듈을 표로 색인한다.

> **주의**: 프로젝트 규모(1,624개 대상 파일)로 인해 이번 1차 분석은 **모듈/디렉토리 단위** 요약 색인이다.
> 개별 파일 단위 상세 표는 추후 `update-docs` 또는 `source-index` 스킬로 하위 디렉토리별 재실행하여 보강해야 한다.

## appl + h + rm

| 경로 | 역할 | 주요 클래스/함수 | 관련 파일 | 사용 API | 의존 모듈 |
|---|---|---|---|---|---|
| appl/TB205001/* | 이자/금리 MFC DLL | CInterestPanel, CNButton, CMapWnd, CPanel, CGpEdit, CGroupNameDlg, testwnd, axCreate() | h/*.h | MFC, GDI | axis 런타임 |
| appl/TB900100/* | 채권/Unbond MFC DLL | CUnBond, CNButton, CPanel, CMapWnd, SEdit, CXLAutomation, CXLEzAutomation, axCreate() | h/*.h | MFC, Excel COM Automation | axis 런타임 |
| h/*.h (30개) | 도메인/프로토콜/UI 구조체 정의 | jmcode.h, comcode.h, tjmcode.h, axis.h, axisfire.h, axisrsm.h, form.h, Grid.h, axisgenv.h, axisgwin.h, axismgrp.h, menudef.h, controlMSG.h, UserDefineMessage.h 등 | appl/*, rm/* | - | - |
| rm/* | 리소스 관리자(axisrm) | CAxisrmApp, CCommLine, CResDlg, CTransDlg, CAboutDlg | h/* (중복 포함) | MFC, 파일 I/O, 소켓 | - |

## axis (메인 EXE)

| 경로 | 역할 | 주요 클래스/함수 | 관련 파일 | 사용 API | 의존 모듈 |
|---|---|---|---|---|---|
| axis/axis.h, axis.cpp | 애플리케이션 진입점/초기화 | CAxisApp, InitInstance() | MainFrm.h, resource.h, UnhandledException/ExceptionHandler.h | MFC(CWinApp), COM(CoInitialize/AfxOleInit), Socket(AfxSocketInit) | - |
| axis/MainFrm.h, MainFrm.cpp | 메인 프레임(God Class, ~730행) | CMainFrame, OnAXIS/OnUSER/OnCHILDMSG/OnCHASER, OnTimer | axisDoc, axisView, ChildFrm, 대화상자 25+개 | MFC(CMDIFrameWnd), Event Sink Map | - |
| axis/axisDoc.*, axisView.* | 문서/뷰 | CAxisDoc(CDocument), CAxisView/CAxTabView/CAxScrollView(CView) | MainFrm | MFC | - |
| axis/ChildFrm.* | MDI 자식 프레임 | CChildFrame | MainFrm | MFC | - |
| axis/axInstance.* | 중복 실행 방지 싱글톤 | CAxInstance(Mutex 기반) | axis.cpp | WinAPI(Mutex) | - |
| axis/connect.* | 로그인 대화상자 | CConnect | NSocket | MFC | - |
| axis/NSocket.* | 비동기 소켓 통신 | CNSocket(CAsyncSocket), OnConnect/OnReceive/OnClose, 상태머신(m_flow, m_step) | connect | Winsock(afxsock.h) | - |
| axis/component/*, 기타 UI | 커스텀 UI 컴포넌트(~50개) | CNTab, CNTable, CMTable, CfxGridEx, CLinkbar, CXCaption, CGPop, CMPop 등 | MainFrm | MFC, GDI | - |
| axis/UnhandledException/* | 크래시/예외 처리 | IExceptionHandler, CExceptionHandler, CriticalSection(RAII), CrashHandler, SymbolEngine | axis.cpp | WinAPI(TlHelp32, DbgHelp 추정) | - |
| axis/Crypt.*, axMisc.*, NDib.*, NPalette.*, AxBitmap.*, FormManager.* | 유틸리티(암호화/비트맵/팔레트/폼관리) | Crypt(SHA256/Base64), CAxMisc, CNDib, CNPalette, CAxBitmap, CFormManager | MainFrm | GDI | - |

## control (8개 화면 컨트롤 DLL)

| 경로 | 역할 | 주요 클래스/함수 | 관련 파일 | 사용 API | 의존 모듈 |
|---|---|---|---|---|---|
| control/fx_gridEX/* | 고급 그리드 컨트롤 DLL | CfxGrid, CfxIGrid, CMemDC, axCreate() | h/axisfire.h | MFC, GDI, OLE 드래그드롭 | - |
| control/CX_FilsSearch/* | 파일 검색 컨트롤 DLL | CControlwnd, IDispatch, axCreate() | h/axisfire.h | MFC, CFileDialog | - |
| control/CX_POP/* | 팝업 정보 컨트롤 DLL | CPopInfo(CDialogEx), CMTable, axCreate() | h/axisfire.h | MFC, Timer | - |
| control/fx_codectrl/* | 코드 입력 자동완성 컨트롤 DLL | CCodeEdit(CEdit+Thread), CSubList, axCreate() | h/axisfire.h | MFC, CWinThread, HANDLE Event | - |
| control/CX_griddata/* | 금융 데이터 그리드 DLL | CControlwnd, CCodeData, axCreate() | h/axisfire.h | MFC | - |
| control/CX_FileRead/* | 파일 읽기 DLL | CControlwnd, axCreate() | h/axisfire.h | MFC, 파일 I/O | - |
| control/cx_fileupdown/* | 파일 업/다운로드(암호화) DLL | CControlwnd, Crypt, axCreate() | h/axisfire.h | MFC, SEED/SHA256/Base64, stdint.h | - |
| control/fx_AGdlg/* | AG 주문 대화상자 DLL | CControlWnd, axCreate() | h/axisfire.h | MFC | - |

## dll/axissm (거래 화면 UI 컴포넌트, 5계층)

| 경로 | 역할 | 주요 클래스/함수 | 관련 파일 | 사용 API | 의존 모듈 |
|---|---|---|---|---|---|
| dll/axissm/* (Layer1 Foundation) | 리소스 관리 | CGlobal | stdafx.h | MFC, GDI | - |
| dll/axissm/* (Layer2 Basic Controls) | 기본 컨트롤 | CAxButton, CAxStatic, CAxScrollBar, CHBand | Layer1 | MFC | - |
| dll/axissm/* (Layer3 Extended Controls) | 확장 컨트롤 | CXedit, CXComboBox, CXListCtrl, CXDateBox, fxSpin | Layer2 | MFC | - |
| dll/axissm/* (Layer4 Dialogs) | 설정 대화창 | CTSetup, COrderSetup, CLink, CHotkey, CMenuSearch | Layer3 | MFC | - |
| dll/axissm/* (Layer5 Main Components) | 메인 UI 컴포넌트 | CBb, CSmain, CTMenu, CoolDialogBar, Dbar0/1/2 | 전 계층, ../../axis/*, ../../h/* | MFC, IME(Imm32.lib) | axis, h |

## dll (axissm 제외)

| 경로 | 역할 | 주요 클래스/함수 | 관련 파일 | 사용 API | 의존 모듈 |
|---|---|---|---|---|---|
| dll/AxisCode/* | 코드 검색/조회 DLL | axGetCode, axGetName, axCreateEx, axGetLink, axGetMiniJisu, axCodeList, CGlobal | ../../h/axisfire.h, jmCode.h | MFC | h |
| dll/ctrl/* | Tree/List/Combo 유틸 컨트롤 라이브러리 | AFX_EXT_CLASS 컨트롤군 | - | MFC | - |
| dll/RushTester/* | Excel 연동 테스트 앱 | CXLAutomation(Excel COM wrapper) | h/axis*.h, excel9.h | MFC, OLE Automation | h |
| dll/login/* | 로그인/인증/원장 | axLogin, axLoginLedger, axLedger, axGetLedger, axSetLedger, axGetLedgerEx, ledger.h | AFXTEMPL.H | MFC, skPcinfo.lib(SKT 인증서) | - |
| dll/xecure/* | LEA 암호화 ActiveX | CXecureCtrl(COleControl), axEncrypt | LEA/include/lea.h, config.h | ActiveX, LEA/(Win32|x64)/dllLEA.lib | - |
| dll/axisDialog/* | 공용 대화상자 | axDialog(void* axDialog(int kind, char *root, ...)) | - | MFC | - |
| dll/axiscodx/* | 마스터/카테고리 데이터 | axGetMaster, axGetCategory, axGetSpinData, axGetDelta | - | MFC | - |
| dll/axiscore/* | 코어 버전/기반 | - | - | version.lib | - |

## platform/builder/awDlg + Components

| 경로 | 역할 | 주요 클래스/함수 | 관련 파일 | 사용 API | 의존 모듈 |
|---|---|---|---|---|---|
| platform/builder/awDlg/* | Dialog/PropertySheet 기반 UI 모듈 | CAddrDlg, CComboItem, CAttrDlg, CLogin, CPreview, CSettingDlg, CBaseInfoPage, CDefaultPage, CScriptPage, CMapScript, COptionsSheet, CMapSheet, CFolderBrowser, CSepWnd, CSingleProperty, CPane, CHBBtn | ../Components/*, ../../dll/lib/axislib.h | MFC, OLE | awSock, awBuild, awWcc, awCommon, axislib, awObject, awUser |
| platform/builder/Components/* | 고급 MFC 컨트롤 컴포넌트 | CAutoCmplLB, CCheckComboBox, CWEdit, CEditList, CToolTipListCtrl, CPopListCtrl, CTreeFileCtrl, CServerTreeCtrl, CMultiSelTree, CSymbolTreeCtrl, CPCTreeCtrl, BCMenu, CVBScriptEdit, CTWScriptEdit, CResultEditCtrl, CCrystalTextView, CResizeDlg, CEditReplaceDlg, CFindTextDlg, CSizingControlBar, CMDITabs, CSplashWnd, CTips | ../awCommon/*, ../awUser/DownLoad.h | MFC, Shell API(shlobj.h, lm.h), COM(IMalloc, IShellFolder) | awCommon, awUser |

## platform/builder (awDlg, Components 제외)

| 경로 | 역할 | 주요 클래스/함수 | 관련 파일 | 사용 API | 의존 모듈 |
|---|---|---|---|---|---|
| platform/builder/awWcc/* | 스크립트 컴파일러 엔진 | CCompile, mapTokenParsing | ../../h/mapform.h, mapvar.h | MFC | - |
| platform/builder/awBuild/* | Map 문서 파서/빌더 | CMapLoad, CAwBuildApp | awWcc | MFC | awWcc |
| platform/builder/awCommon/* | 공용 유틸 컴포넌트 | xTreeCtrl(중복 정의 발견) | - | MFC | - |
| platform/builder/awTool/* | 도형 그리기 도구 (19개 Ctl*) | Ctl* 클래스군 | ../form_w.h | MFC | - |
| platform/builder/awUser/* | 사용자 객체 UI | CusrObject(CDialog) | awSock | MFC | awSock |
| platform/builder/awSock/* | 소켓 통신 | CwSock(CWnd, DECLARE_EVENTSINK_MAP, 8-state State Machine) | ../workio.h | MFC, Winsock2 | - |
| platform/builder/awObject/* | 객체 렌더링 | CShow(CObject, DECLARE_DYNCREATE) | CAxisPalette, CAxisDraw, CAxisForm(전방선언, 미정의) | MFC | - |

## platform/dll (axisform / axislib / axisvbs)

| 경로 | 역할 | 주요 클래스/함수 | 관련 파일 | 사용 API | 의존 모듈 |
|---|---|---|---|---|---|
| platform/dll/form/* | Form Control Layer DLL (axisform.dll) | CAxisForm(CCmdTarget), CfmBase, CfmEdit, CfmButton, CfmGrid, CfmTab, Cbrowser(CHtmlView), CbrowserX(WebView2) | lib/axislib.h | MFC, IE ActiveX, WebView2(ICoreWebView2), Dispatch(DISP_PROPERTY_EX) | axislib, AxisCodx.dll(동적로드) |
| platform/dll/lib/* | Graphics Utility DLL (axislib.dll) | CAxisDraw(Object Pool), CAxisPalette | - | MFC, GDI | - |
| platform/dll/vbs/* | Script Engine DLL (axisvbs.dll) | CScriptEngine(COleDispatchDriver), CScriptSite(IActiveScriptSite), CPythonEngine, CEngineWrapper, CVbGuide(CWinThread) | ../../h/* | COM(IActiveScript, IDispatch), Python.h C API, WinMM | h |

## platform/wizard + sock + h + h_우선백업 + ver + rm

| 경로 | 역할 | 주요 클래스/함수 | 관련 파일 | 사용 API | 의존 모듈 |
|---|---|---|---|---|---|
| platform/wizard/* | axWizard.ocx - 화면/데이터/세션 관리 OCX | CGuard(God Class, 400+ 메서드), CWorks, CClient, CScreen, CScript, CDde, CObjects | platform/h/*, platform/dll/lib, form, vbs | COM(IDispatch), MFC(COleControl), Thread Pool(CThpool) | axislib, axisform, axisvbs |
| platform/sock/* | axSock.ocx - TCP/UDP 통신 OCX | CStream(TCP), CDgram(UDP), CWsh(Worker Thread), CMQue | platform/h/* | COM(IDispatch), MFC(COleControl), ws2_32.lib | - |
| platform/h/* (21개) | 프로토콜/UI 구조체 정의 | axis.h(_axisH), form.h, Graph.h, common.h | wizard, sock | - | - |
| platform/h_우선백업/* (18개) | 백업/미사용 추정 헤더 | - | - | - | (백업/미사용 추정) |
| platform/ver/* | 버전 관리 EXE | axisver.cpp | - | version.lib | - |
| platform/rm/* | 리소스 관리 EXE | axisrm.cpp | - | - | - |

## 통계 요약

| 배치 | 파일 수(추정) | 클래스/구조체 수(추정) |
|---|---|---|
| appl+h+rm | 113 | 39 클래스 + 136 구조체 |
| axis | 177 | ~100 |
| control | 120+ | 40+ |
| dll/axissm | 168 | 150+ |
| dll(axissm 제외) | ~220 | ~110 |
| platform/builder/awDlg+Components | 129 | ~78 |
| platform/builder(나머지) | 95 | ~55 |
| platform/dll | ~90 | ~55 |
| platform/wizard+sock+h 등 | 150 | ~60 클래스 + ~120 구조체 |
| **합계(대략)** | **~1,260~1,624** | **~700+** |

## 다음 단계
- `source-index` 또는 `update-docs` 스킬을 하위 디렉토리별로 재실행하여 파일 단위(1행=1파일) 세부 표로 보강
- 특히 axis(177개), dll/axissm(168개), dll 나머지(~220개)는 파일 수가 많아 우선순위 높음
