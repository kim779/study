---
project: src_7_11
category: dependency
status: 작성됨
updated: 2026-08-21
---

# Dependency


## 목차

- [런타임 DLL 로드 개요](#런타임-dll-로드-개요)
- [모듈별 Include/링크 의존성](#모듈별-include링크-의존성)
  - [appl + h + rm](#appl-h-rm)
  - [axis (메인 EXE)](#axis-메인-exe)
  - [control (8개 DLL 공통)](#control-8개-dll-공통)
  - [dll/axissm](#dllaxissm)
  - [dll (axissm 제외)](#dll-axissm-제외)
  - [platform/builder (awDlg/Components 포함 전체)](#platformbuilder-awdlgcomponents-포함-전체)
  - [platform/dll (axisform/axislib/axisvbs)](#platformdll-axisformaxislibaxisvbs)
  - [platform/wizard + sock + h](#platformwizard-sock-h)
- [구조적 의존성 이슈](#구조적-의존성-이슈)
- [외부 SDK/라이브러리 총괄](#외부-sdk라이브러리-총괄)
- [다음 단계](#다음-단계)

---

- 생성일: 2026-07-07 / 최초 분석 갱신: 2026-07-07
- 목적: Header Include, DLL/LIB, COM, 외부 SDK, MFC/ATL, Windows API 의존성을 기록한다.

## 런타임 DLL 로드 개요

```
axis.exe (메인 UI)
  ├─ dll/axissm.dll        (거래 화면 UI 컴포넌트, 5계층)
  ├─ dll/AxisCode.dll      (코드 검색/조회)
  ├─ dll/login.dll         (로그인/인증/원장)
  ├─ dll/xecure.dll        (LEA 암호화 ActiveX)
  ├─ dll/axisDialog.dll    (공용 대화상자)
  ├─ dll/axiscodx.dll      (마스터/카테고리 데이터)
  ├─ dll/axiscore.dll
  ├─ platform/dll/axisform.dll  (Form Control Layer)
  ├─ platform/dll/axislib.dll   (Graphics Utility)
  ├─ platform/dll/axisvbs.dll   (Script Engine: VBS/Python)
  ├─ platform/wizard/axWizard.ocx (화면 렌더링/세션 관리 OCX)
  ├─ platform/sock/axSock.ocx    (TCP/UDP 통신 OCX)
  ├─ control/*.dll (8종 화면 컨트롤: fx_gridEX, CX_POP, fx_codectrl 등)
  └─ appl/*.dll (업무 화면: TB205001, TB900100 등)

platform/builder (axisBuilder, 화면 Map 편집기 — 별도 실행 도구)
  ├─ awBuild.lib / awWcc.lib   (Map 파서 / 스크립트 컴파일러)
  ├─ awCommon.lib, awTool.lib, awUser.lib, awSock.lib, awObject.lib
  └─ awDlg.dll (Dialog/PropertySheet), Components(*.lib) (Tree/List/Edit 컨트롤)
```

## 모듈별 Include/링크 의존성

### appl + h + rm
- Include 방향(DAG, 순환 없음): `appl(TB205001, TB900100) → h(30개 헤더) → Windows SDK/MFC`
- 외부 의존: MFC(Dynamic), Windows SDK(GDI/File I/O/Registry), OLE/COM(Excel Automation), Socket(이벤트 기반)
- 주의: h/ 와 rm/h/ 에 약 18개 헤더 중복 존재 — 동기화 여부 확인 필요

### axis (메인 EXE)
- Include: axis.h→stdafx.h/resource.h/UnhandledException/ExceptionHandler.h/MainFrm.h 등, MainFrm.h→afxtempl.h/afxmt.h/TlHelp32.h + 로컬 헤더 50+
- 링크: winmm.lib (멀티미디어)
- 외부 DLL 로드: axissm.dll, axisform.dll, axislib.dll (../dll, ../platform/dll)
- COM/OLE: CoInitialize(), AfxOleInit(), IDispatch(CImpIDispatch), Event Sink Map, IWebBrowserApp
- WinAPI: AfxSocketInit/CAsyncSocket(소켓), GetModuleFileName/TlHelp32(프로세스), CreateWindow/FindWindow/SetWindowHook(윈도우), GDI(CreateDC/BitBlt), Registry(SetRegistryKey), 파일 I/O, PlaySound/timeGetTime(멀티미디어)

### control (8개 DLL 공통)
- 내부: `../../h/axisfire.h` (공유 프레임워크 헤더)
- 외부: MFC(afxwin/afxole/afxext), Windows GDI, COM/RPC
- 특수: cx_fileupdown만 stdint.h(SHA256/SEED)
- DLL 링크: 명시적 .lib 없음(MFC/OLE 동적 링크)

### dll/axissm
- 내부: `../../axis/axmsg.hxx`, `../../axis/resource.h`, `../../h/axisvar.h`, `../../h/axisfire.h`
- 외부: MFC(afxwin/afxext 핵심, afxole/afxsock/afxcmn 선택적), WinAPI(Imm32.lib — IME), mmsystem.h
- 상위 모듈: axisBuilder, axiscore.dll 등

### dll (axissm 제외)
- xecure: `LEA/(Win32|x64)/dllLEA.lib` (암호화), LEA/include/lea.h, config.h
- login: `skPcinfo.lib` (SKT 인증서), `<AFXTEMPL.H>`, ledger.h
- axiscore: `version.lib`
- AxisCode: `../../h/axisfire.h`, `../../h/jmCode.h`
- RushTester: `h/axis*.h`, `excel9.h` (Excel OLE 래퍼)
- Export 함수(대표): axGetCode/axGetName/axCreateEx(AxisCode), axLogin/axLoginLedger(login), axDialog(axisDialog), axGetMaster/axGetCategory(axiscodx), axEncrypt(xecure)
- Circular Include: 검출 안 됨(모두 include guard/pragma once)

### platform/builder (awDlg/Components 포함 전체)
- awDlg.vcxproj 링크: `awsock.lib, awbuild.lib, awwcc.lib, awcommon.lib, axislib.lib, awobject.lib, awuser.lib`
- Components(FileTreeCtrl): `<afxmt.h>`(MFC Threading), `<shlobj.h>`(Shell API), `<lm.h>`(Network API), COM(IMalloc, IShellFolder)
- awSock/CwSock: Winsock2, DECLARE_EVENTSINK_MAP
- 내부 참조: `../awCommon/`, `../Components/`, `../../dll/lib/axislib.h`, `../../h/mapform.h`, `../../h/mapvar.h`, `../form_w.h`, `../workio.h`
- Open-Source 포함: Crystal Edit(crysedit_src/) — 라이선스 확인 필요
- 순환 Include: 관찰 기반 없음(하향식)

### platform/dll (axisform/axislib/axisvbs)
- form/ → `lib/axislib.h` (CAxisDraw, CAxisPalette)
- vbs/ → 공유 헤더(`../../h/`)
- COM: IActiveScript, IActiveScriptSite, IDispatch (Windows Script Host)
- Python 3.x: `<Python.h>` (C API)
- GDI: CDC/CPen/CBrush/CFont/CBitmap
- WebView2: ICoreWebView2Environment/Controller
- WinMM: mciSendCommand (AVI 재생)
- 동적 로드(하드코딩 문자열 주의): `"AxisCodx.dll"` (form/axform.cpp)

### platform/wizard + sock + h
- 링크: `ws2_32.lib`(소켓), `imm32.lib`, `version.lib`, `winmm.lib`
- 내부 DLL 의존: `axislib.lib, axisform.lib, axisvbs.lib`
- COM: IDispatch(ActiveX 컨트롤), IDropTarget(OleDropEx 직접구현)
- Export 4종(공통): DllCanUnloadNow, DllGetClassObject, DllRegisterServer, DllUnregisterServer
- h_우선백업/: 백업/미사용 추정 — 실제 참조 여부 확인 필요

## 구조적 의존성 이슈

1. **헤더 중복**: `h/` ↔ `rm/h/` (~18개), `platform/h/` ↔ `platform/h_우선백업/` (백업 추정) — 동기화 여부 확인 필요
2. **경로 표기 불일관**: 절대 상대경로(`../../h/`) vs 상대경로 혼용 (dll 계열 전반)
3. **Export 방식 불일관**: `.def` 파일 / `__declspec(dllexport)` / `AFX_EXT_CLASS` 혼용
4. **동적 로드 하드코딩**: `"AxisCodx.dll"` 문자열 리터럴 — 배포 경로/이름 변경 시 로드 실패 위험
5. **순환 Include**: 각 배치에서 개별적으로는 발견되지 않았으나, axis ↔ dll/axissm ↔ platform/dll 간 교차 참조(`../../axis/*.hxx`, `../../h/*.h`)가 많아 전체 트리 기준 순환 여부는 call-graph/전체 include 그래프 재검증 필요

## 외부 SDK/라이브러리 총괄

| 구분 | 항목 |
|---|---|
| UI 프레임워크 | MFC (전 모듈 공통), ATL(부분) |
| COM/OLE | IDispatch, IActiveScript, IShellFolder, IDropTarget, Excel Automation |
| 암호화 | LEA(xecure), SHA256/Base64/SEED(cx_fileupdown) |
| 스크립트 엔진 | VBScript(IActiveScript), Python 3.x(Python.h) |
| 브라우저 | IE(CHtmlView), WebView2(ICoreWebView2) |
| 네트워크 | Winsock2(ws2_32.lib), CAsyncSocket |
| 인증 | SKT 인증서(skPcinfo.lib) |
| 기타 시스템 | Imm32(IME), version.lib, winmm.lib, Shell API(shlobj.h, lm.h) |

## 다음 단계
- dependency-map 스킬로 Mermaid 그래프 생성 및 axis↔dll↔platform 교차 참조 상세 검증
- h/ vs rm/h/, platform/h/ vs platform/h_우선백업/ 중복 헤더의 실제 diff 확인
