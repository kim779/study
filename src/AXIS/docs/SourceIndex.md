---
project: AXIS
category: source-index
status: 작성됨
updated: 2026-08-21
---

# AXIS 프로젝트 소스 색인 (Source Index)


## 목차

- [파일 분류 및 색인](#파일-분류-및-색인)
  - [1. MFC 프레임워크 핵심 (Framework Core)](#1-mfc-프레임워크-핵심-framework-core)
  - [2. 동기화 및 예외 처리 (Synchronization & Exception Handling)](#2-동기화-및-예외-처리-synchronization-exception-handling)
  - [3. 워커 스레드 (Worker Threads)](#3-워커-스레드-worker-threads)
  - [4. 싱글톤 및 전역 관리 (Singletons & Global Management)](#4-싱글톤-및-전역-관리-singletons-global-management)
  - [5. 데이터 모델 (Data Models)](#5-데이터-모델-data-models)
  - [6. 커스텀 컨트롤 (Custom Controls)](#6-커스텀-컨트롤-custom-controls)
    - [그리드 컨트롤](#그리드-컨트롤)
    - [버튼 및 기본 컨트롤](#버튼-및-기본-컨트롤)
  - [7. 다이얼로그 (Dialogs - 60+ 개)](#7-다이얼로그-dialogs---60-개)
    - [핵심 다이얼로그](#핵심-다이얼로그)
  - [8. 거래/주문 처리 (Trading/Order)](#8-거래주문-처리-tradingorder)
  - [9. 시스템 정보 및 모니터링 (System Info & Monitoring)](#9-시스템-정보-및-모니터링-system-info-monitoring)
  - [10. 알림 및 이벤트 (Notifications & Events)](#10-알림-및-이벤트-notifications-events)
  - [11. 그래픽 및 렌더링 (Graphics & Rendering)](#11-그래픽-및-렌더링-graphics-rendering)
  - [12. 파일 처리 (File Operations)](#12-파일-처리-file-operations)
  - [13. 보안 및 인증 (Security & Authentication)](#13-보안-및-인증-security-authentication)
  - [14. 네트워크 (Network)](#14-네트워크-network)
  - [15. 특수 기능 (Special Features)](#15-특수-기능-special-features)
  - [16. 레이아웃 및 UI 유틸 (Layout & UI Utils)](#16-레이아웃-및-ui-유틸-layout-ui-utils)
  - [17. 리소스 및 초기화 (Resource & Initialization)](#17-리소스-및-초기화-resource-initialization)
  - [18. 기타 다이얼로그 및 도구 (Misc)](#18-기타-다이얼로그-및-도구-misc)
- [파일 통계](#파일-통계)
  - [전체 요약](#전체-요약)
  - [주요 파일 (Top 10 복잡도)](#주요-파일-top-10-복잡도)
- [의존성 관계 요약](#의존성-관계-요약)
  - [상향식 의존성 (Bottom-up)](#상향식-의존성-bottom-up)
  - [순환 의존성 위험](#순환-의존성-위험)
- [빌드 순서 제안](#빌드-순서-제안)
  - [단계 1: 기반 (Foundations)](#단계-1-기반-foundations)
  - [단계 2: 핵심 앱 (Core App)](#단계-2-핵심-앱-core-app)
  - [단계 3: 서비스 (Services)](#단계-3-서비스-services)
  - [단계 4: 모델 (Models)](#단계-4-모델-models)
  - [단계 5: 컨트롤 (Controls)](#단계-5-컨트롤-controls)
  - [단계 6: 다이얼로그 & 뷰 (Dialogs & Views)](#단계-6-다이얼로그-뷰-dialogs-views)
  - [단계 7: 특수 기능 (Special Features)](#단계-7-특수-기능-special-features)
- [분석 가능성 (Analyzability)](#분석-가능성-analyzability)
  - [높음 (Easy to analyze)](#높음-easy-to-analyze)
  - [중간 (Medium)](#중간-medium)
  - [낮음 (Hard to analyze)](#낮음-hard-to-analyze)
- [테스트 전략](#테스트-전략)
  - [단위 테스트 (Unit Test) 가능성](#단위-테스트-unit-test-가능성)
- [참고](#참고)

---

**분석 일시**: 2026-07-14  
**대상 범위**: d:\src\IBKS\src\AXIS 디렉토리의 주요 .cpp/.h 파일

---

## 파일 분류 및 색인

### 1. MFC 프레임워크 핵심 (Framework Core)

| 파일 | 역할 | 주요 클래스 | 관련 파일 | 의존성 |
|------|------|----------|---------|-------|
| **axis.h** | 애플리케이션 헤더 | CAxisApp (싱글톤) | axis.cpp, MainFrm.h | MFC, Windows.h |
| **axis.cpp** | 애플리케이션 진입점 | CAxisApp 구현 | axis.h, MainFrm.h, FirstJob.h | MFC, AfxOleInit(), CoInitialize() |
| **MainFrm.h** | 메인 프레임 헤더 | CMainFrame (MDI) | MainFrm.cpp, ChildFrm.h | afxmt.h, atlbase.h, CriticalSection.h |
| **MainFrm.cpp** | 메인 프레임 구현 | CMainFrame 메시지 맵 (150+) | MainFrm.h, PhonePad.h, ExceptionHandler.h | whdump.h, ASTx SDK |
| **ChildFrm.h** | 자식 프레임 헤더 | CChildFrame (MDI Child) | ChildFrm.cpp, MainFrm.h | MFC |
| **ChildFrm.cpp** | 자식 프레임 구현 | CChildFrame 메시지 맵 | ChildFrm.h, MainFrm.h | MFC |
| **axisDoc.h** | 문서 헤더 | CAxisDoc | axisDoc.cpp, MainFrm.h | MFC |
| **axisDoc.cpp** | 문서 구현 | CAxisDoc (데이터 모델) | axisDoc.h | MFC, CString |
| **axisView.h** | 뷰 헤더 | CAxisView | axisView.cpp | MFC |
| **axisView.cpp** | 뷰 구현 | CAxisView (렌더링) | axisView.h, axisDoc.h | MFC |
| **axScrollView.h** | 스크롤 뷰 헤더 | CAxisScrollView | axScrollView.cpp | MFC |
| **axScrollView.cpp** | 스크롤 뷰 구현 | CAxisScrollView | axScrollView.h | MFC, CScrollView |
| **AxTabView.h** | 탭 뷰 헤더 | CAxTabView | AxTabView.cpp | MFC |
| **AxTabView.cpp** | 탭 뷰 구현 | CAxTabView (탭 기반) | AxTabView.h, CTabCtrl | MFC |

**라인 수 추정**: 총 5000+ 줄  
**복잡도**: 높음 (메시지 맵 중심)  
**유지보수성**: 중간 (God Class 우려)

---

### 2. 동기화 및 예외 처리 (Synchronization & Exception Handling)

| 파일 | 역할 | 주요 클래스 | 관련 파일 | 의존성 |
|------|------|----------|---------|-------|
| **CriticalSection.h** | RAII 동기화 | CCriticalSection, CUseCriticalSection | MainFrm.h (포함) | Windows.h, <Windows.h> |
| **ExceptionHandler.h** | 예외 처리 인터페이스 | IExceptionHandler, CExceptionHandler | ExceptionHandler.cpp, MainFrm.cpp | MFC |
| **ExceptionHandler.cpp** | 예외 처리 구현 | CExceptionHandler (싱글톤) | ExceptionHandler.h | MFC |
| **whdump.h** | MiniDump 유틸 | (함수 기반) | MainFrm.cpp | Windows.h, DbgHelp.lib |

**라인 수**: 200 줄  
**복잡도**: 낮음  
**우려사항**: RAII 패턴이 코드 전체에 적용되는지 미확인

---

### 3. 워커 스레드 (Worker Threads)

| 파일 | 역할 | 주요 클래스 | 관련 파일 | 의존성 |
|------|------|----------|---------|-------|
| **PhoneThread.h** | 전화 스레드 헤더 | CPhoneThread (CWinThread) | PhoneThread.cpp | MFC |
| **PhoneThread.cpp** | 전화 스레드 구현 | CPhoneThread (Run, InitInstance, ExitInstance) | PhoneThread.h, MainFrm.h | MFC |
| **PhonePad.h** | 전화 패드 헤더 | (미상) | MainFrm.h 포함 | MFC |

**라인 수**: 100 줄  
**복잡도**: 낮음  
**우려사항**: 메시지 맵이 비어있음 (Run()만 구현)

---

### 4. 싱글톤 및 전역 관리 (Singletons & Global Management)

| 파일 | 역할 | 주요 클래스 | 관련 파일 | 의존성 |
|------|------|----------|---------|-------|
| **axInstance.h** | 중복 실행 방지 | CAxInstance | axInstance.cpp | Windows.h, Registry |
| **axInstance.cpp** | CAxInstance 구현 | 뮤텍스/이벤트 기반 | axInstance.h | Windows.h |
| **ResourceHelper.h** | 리소스 관리 헤더 | CResourceHelper | ResourceHelper.cpp | MFC |
| **ResourceHelper.cpp** | 리소스 관리 구현 | CResourceHelper (동적 할당) | ResourceHelper.h | MFC, GDI |

**라인 수**: 500 줄  
**복잡도**: 중간  
**우려사항**: Race Condition, 메모리 누수 가능성

---

### 5. 데이터 모델 (Data Models)

| 파일 | 역할 | 주요 클래스 | 관련 파일 | 의존성 |
|------|------|----------|---------|-------|
| **dtInfo.h** | 데이터 타입 정보 | CDtInfo | dtInfo.cpp | MFC |
| **dtInfo.cpp** | CDtInfo 구현 | 데이터 정의/변환 | dtInfo.h | MFC, CString |
| **Conclusion.h** | 체결 정보 헤더 | CConclusion | Conclusion.cpp | MFC |
| **Conclusion.cpp** | 체결 정보 구현 | CConclusion (주문/체결) | Conclusion.h | MFC |
| **EConclusion.h** | 확장 체결 정보 | CEConclusion | EConclusion.cpp | MFC |
| **EConclusion.cpp** | CEConclusion 구현 | 체결 세부 정보 | EConclusion.h | MFC |
| **BaseMP.h** | 기본 매매 포지션 | CBaseMP | BaseMP.cpp | MFC |
| **BaseMP.cpp** | CBaseMP 구현 | 포지션 관리 | BaseMP.h | MFC |
| **TotalAccount.h** | 통합 계좌 | CTotalAccount | TotalAccount.cpp | MFC |
| **TotalAccount.cpp** | CTotalAccount 구현 | 계좌 통합 정보 | TotalAccount.h | MFC |
| **TotalAcc.h** | 계좌 정보 | CTotalAcc | TotalAcc.cpp | MFC |
| **TotalAcc.cpp** | CTotalAcc 구현 | 계좌 세부 정보 | TotalAcc.h | MFC |

**라인 수**: 1000+ 줄  
**복잡도**: 중간  
**범위**: 거래/계좌 핵심 데이터

---

### 6. 커스텀 컨트롤 (Custom Controls)

#### 그리드 컨트롤

| 파일 | 역할 | 주요 클래스 | 관련 파일 | 의존성 |
|------|------|----------|---------|-------|
| **grid/fxGrid.h** | FX 그리드 헤더 | CFXGrid | grid/fxGrid.cpp | MFC, CListCtrl |
| **grid/fxGrid.cpp** | FX 그리드 구현 | CFXGrid (다중 선택, 편집) | grid/fxGrid.h, grid/InPlaceEdit.h | MFC, GDI |
| **grid/InPlaceEdit.h** | 인플레이스 에디트 | CInPlaceEdit | grid/InPlaceEdit.cpp | MFC |
| **grid/InPlaceEdit.cpp** | CInPlaceEdit 구현 | 셀 편집 컨트롤 | grid/InPlaceEdit.h | MFC |
| **grid/InPlaceDate.h** | 인플레이스 날짜 | CInPlaceDate | grid/InPlaceDate.cpp | MFC |
| **grid/InPlaceDate.cpp** | CInPlaceDate 구현 | 날짜 선택 컨트롤 | grid/InPlaceDate.h | MFC |
| **grid/InPlaceCombo.h** | 인플레이스 콤보 | CInPlaceCombo | grid/InPlaceCombo.cpp | MFC |
| **grid/InPlaceCombo.cpp** | CInPlaceCombo 구현 | 드롭다운 셀 | grid/InPlaceCombo.h | MFC |
| **NGrid.h** | NGrid 클래스 | CNGrid | NGrid.cpp | MFC, CListCtrl |
| **NGrid.cpp** | CNGrid 구현 | 확장 그리드 | NGrid.h | MFC, GDI |
| **NTable.h** | 테이블 클래스 | CNTable | NTable.cpp | MFC |
| **NTable.cpp** | CNTable 구현 | 테이블 렌더링 | NTable.h | MFC, GDI |
| **grid/DropTarget.h** | 드래그 드롭 | CDropTarget | grid/DropTarget.cpp | MFC, IOleDropTarget |
| **grid/DropTarget.cpp** | CDropTarget 구현 | 드롭 처리 | grid/DropTarget.h | MFC |
| **grid/excel9.h** | Excel9 호환성 | CExcel9 | grid/excel9.cpp | MFC |
| **grid/excel9.cpp** | CExcel9 구현 | Excel 형식 호환 | grid/excel9.h | MFC |

**라인 수**: 2000+ 줄  
**복잡도**: 높음 (GDI 렌더링, 메시지 맵)  
**범위**: 데이터 시각화 (시세, 포지션, 주문)

#### 버튼 및 기본 컨트롤

| 파일 | 역할 | 주요 클래스 | 관련 파일 | 의존성 |
|------|------|----------|---------|-------|
| **BmpButton.h** | 비트맵 버튼 | CBmpButton | BmpButton.cpp | MFC, CButton |
| **BmpButton.cpp** | CBmpButton 구현 | 이미지 버튼 렌더링 | BmpButton.h | MFC, GDI |
| **BtnST.h** | 스타일 버튼 | CBtnST | BtnST.cpp | MFC |
| **BtnST.cpp** | CBtnST 구현 | 3D 버튼 스타일 | BtnST.h | MFC, GDI |
| **ShapeButton.h** | 모양 버튼 | CShapeButton | ShapeButton.cpp | MFC |
| **ShapeButton.cpp** | CShapeButton 구현 | 원형, 다각형 버튼 | ShapeButton.h | MFC, GDI |
| **ImgBtn.h** | 이미지 버튼 | CImgBtn | ImgBtn.cpp | MFC |
| **ImgBtn.cpp** | CImgBtn 구현 | 이미지 버튼 | ImgBtn.h | MFC, GDI |
| **NTab.h** | 탭 컨트롤 | CNTab | NTab.cpp | MFC, CTabCtrl |
| **NTab.cpp** | CNTab 구현 | 커스텀 탭 | NTab.h | MFC, GDI |
| **NProgressCtrl.h** | 프로그레스 | CNProgressCtrl | NProgressCtrl.cpp | MFC |
| **NProgressCtrl.cpp** | CNProgressCtrl 구현 | 진행률 표시 | NProgressCtrl.h | MFC, GDI |
| **CNoteWnd.h** | 노트 윈도우 | CNoteWnd | CNoteWnd.cpp | MFC |
| **CNoteWnd.cpp** | CNoteWnd 구현 | 노트 출력 | CNoteWnd.h | MFC |
| **CSlideWnd.h** | 슬라이드 윈도우 | CSlideWnd | CSlideWnd.cpp | MFC |
| **CSlideWnd.cpp** | CSlideWnd 구현 | 슬라이드 애니메이션 | CSlideWnd.h | MFC, GDI |

**라인 수**: 1500+ 줄  
**복잡도**: 중간 (GDI 기본)

---

### 7. 다이얼로그 (Dialogs - 60+ 개)

#### 핵심 다이얼로그

| 파일 | 역할 | 주요 클래스 | 관련 파일 | 의존성 |
|------|------|----------|---------|-------|
| **SecureDlg.h** | 보안 로그인 | CSecureDlg | SecureDlg.cpp, MainFrm.h | MFC, CDialog |
| **SecureDlg.cpp** | CSecureDlg 구현 | 사용자 인증 | SecureDlg.h | MFC, Winsock2, ASTx |
| **CertLogin.h** | 인증서 로그인 | CCertLogin | CertLogin.cpp | MFC |
| **CertLogin.cpp** | CCertLogin 구현 | 공인인증서 인증 | CertLogin.h | MFC, Wincrypt |
| **DuplicateLoginConfirmDlg.h** | 중복 로그인 | CDuplicateLoginConfirmDlg | DuplicateLoginConfirmDlg.cpp | MFC |
| **DuplicateLoginConfirmDlg.cpp** | 구현 | 중복 로그인 확인 | DuplicateLoginConfirmDlg.h | MFC |
| **ConfigDlg.h** | 기본 설정 | CConfigDlg | ConfigDlg.cpp, MainFrm.h | MFC |
| **ConfigDlg.cpp** | CConfigDlg 구현 | 사용자 설정 | ConfigDlg.h | MFC, Registry |
| **SetupC.h** | 고급 설정 | CSetupC | SetupC.cpp, MainFrm.h | MFC |
| **SetupC.cpp** | CSetupC 구현 | 고급 설정 (계좌, 서버) | SetupC.h | MFC, Registry |
| **AccountConfig.h** | 계좌 설정 | CAccountConfig | AccountConfig.cpp | MFC |
| **AccountConfig.cpp** | CAccountConfig 구현 | 계좌별 설정 | AccountConfig.h | MFC, dtInfo |
| **LockDlg.h** | 잠금 다이얼로그 | CLockDlg | LockDlg.cpp | MFC |
| **LockDlg.cpp** | CLockDlg 구현 | 패스워드 잠금 | LockDlg.h | MFC |
| **LockPass.h** | 패스워드 관리 | CLockPass | LockPass.cpp | MFC |
| **LockPass.cpp** | CLockPass 구현 | 패스 저장/검증 | LockPass.h | MFC, Wincrypt |

**라인 수**: 3000+ 줄 (모든 다이얼로그 포함 시 10000+ 줄)  
**복잡도**: 중간  
**범위**: UI 입력 처리, 설정 관리

---

### 8. 거래/주문 처리 (Trading/Order)

| 파일 | 역할 | 주요 클래스 | 관련 파일 | 의존성 |
|------|------|----------|---------|-------|
| **MacExec.h** | 매매 매크로 | CMacExec | MacExec.cpp | MFC |
| **MacExec.cpp** | CMacExec 구현 | 자동 주문 매크로 | MacExec.h | MFC, MainFrm.h |
| **MacExecDlg.h** | 매크로 설정 | CMacExecDlg | MacExecDlg.cpp | MFC |
| **MacExecDlg.cpp** | CMacExecDlg 구현 | 매크로 UI | MacExecDlg.h | MFC |
| **Stoploss.h** | 손절매 | CStopLoss | Stoploss.cpp | MFC |
| **Stoploss.cpp** | CStopLoss 구현 | 손절매 로직 | Stoploss.h | MFC, MainFrm.h |
| **ContGrid.h** | 계약 그리드 | CContGrid | ContGrid.cpp | MFC, NGrid |
| **ContGrid.cpp** | CContGrid 구현 | 계약/주문 표시 | ContGrid.h | MFC, GDI |
| **Conclusion.h** | 체결 정보 | CConclusion | (위 데이터 모델 참고) | MFC |
| **Dtconnect.h** | 데이터 연결 | CDtconnect | Dtconnect.cpp | MFC |
| **Dtconnect.cpp** | CDtconnect 구현 | 서버 연결 관리 | Dtconnect.h | MFC, Winsock2 |

**라인 수**: 1000+ 줄  
**복잡도**: 높음 (주문 로직 복잡)

---

### 9. 시스템 정보 및 모니터링 (System Info & Monitoring)

| 파일 | 역할 | 주요 클래스 | 관련 파일 | 의존성 |
|------|------|----------|---------|-------|
| **CpuUsage.h** | CPU 사용률 | CCpuUsage | CpuUsage.cpp | Windows.h, pdh.h |
| **CpuUsage.cpp** | CCpuUsage 구현 | Performance Counter | CpuUsage.h | Windows API |
| **CpuUse.h** | CPU 사용 통계 | CCpuUse | CpuUse.cpp | MFC |
| **CpuUse.cpp** | CCpuUse 구현 | CPU 통계 표시 | CpuUse.h | MFC, GDI |
| **sysInfo.h** | 시스템 정보 | CSysInfo | sysInfo.cpp | Windows.h |
| **sysInfo.cpp** | CSysInfo 구현 | 시스템 조회 | sysInfo.h | Windows API |
| **perCounters.h** | 성능 카운터 | CPerformanceCounters | (추정) | Windows.h, pdh.h |
| **whdump.h** | 미니덤프 | (함수 기반) | MainFrm.cpp | Windows.h, DbgHelp.lib |

**라인 수**: 500 줄  
**복잡도**: 낮음  
**범위**: 시스템 모니터링

---

### 10. 알림 및 이벤트 (Notifications & Events)

| 파일 | 역할 | 주요 클래스 | 관련 파일 | 의존성 |
|------|------|----------|---------|-------|
| **Alarm.h** | 알람 | CAlarm | Alarm.cpp | MFC |
| **Alarm.cpp** | CAlarm 구현 | 알람/경고 표시 | Alarm.h | MFC, MainFrm.h |
| **ImageNotice.h** | 이미지 알림 | CImageNotice | ImageNotice.cpp | MFC |
| **ImageNotice.cpp** | CImageNotice 구현 | 이미지 팝업 | ImageNotice.h | MFC, GDI |
| **KobaElwNotify.h** | 공시 알림 | CKobaElwNotify | KobaElwNotify.cpp | MFC |
| **KobaElwNotify.cpp** | CKobaElwNotify 구현 | 공시 알림 수신 | KobaElwNotify.h | MFC, Winsock2 |
| **dlgMisuAlarm.h** | 체결 알람 | CdlgMisuAlarm | dlgMisuAlarm.cpp | MFC |
| **dlgMisuAlarm.cpp** | CdlgMisuAlarm 구현 | 체결 알림 UI | dlgMisuAlarm.h | MFC |

**라인 수**: 500 줄  
**복잡도**: 낮음

---

### 11. 그래픽 및 렌더링 (Graphics & Rendering)

| 파일 | 역할 | 주요 클래스 | 관련 파일 | 의존성 |
|------|------|----------|---------|-------|
| **NDib.h** | DIB 비트맵 | CNDib | NDib.cpp | MFC, GDI |
| **NDib.cpp** | CNDib 구현 | 비트맵 조작 | NDib.h | MFC, GDI |
| **NPalette.h** | 팔레트 | CNPalette | NPalette.cpp | MFC |
| **NPalette.cpp** | CNPalette 구현 | 색상 팔레트 관리 | NPalette.h | MFC, GDI |
| **GDIHelper.h** | GDI 헬퍼 | CGDIHelper | GDIHelper.cpp | GDI |
| **GDIHelper.cpp** | CGDIHelper 구현 | GDI 유틸 (선, 원 등) | GDIHelper.h | GDI |
| **MapHelper.h** | 맵 렌더링 | CMapHelper | (구현 미상) | GDI |
| **NExpand.h** | 확장/축약 | CNExpand | NExpand.cpp | MFC, GDI |
| **NExpand.cpp** | CNExpand 구현 | 트리 확장 애니메이션 | NExpand.h | MFC, GDI |

**라인 수**: 800 줄  
**복잡도**: 중간 (GDI 기술 필요)

---

### 12. 파일 처리 (File Operations)

| 파일 | 역할 | 주요 클래스 | 관련 파일 | 의존성 |
|------|------|----------|---------|-------|
| **XZip.h** | ZIP 압축 | CXZip | XZip.cpp | (외부 라이브러리) |
| **XZip.cpp** | CXZip 구현 | 압축 | XZip.h | zlib (추정) |
| **XUnzip.h** | ZIP 해제 | CXUnzip | XUnzip.cpp | (외부 라이브러리) |
| **XUnzip.cpp** | CXUnzip 구현 | 해제 | XUnzip.h | zlib (추정) |
| **fname.h** | 파일명 처리 | CFname | fname.cpp | MFC |
| **fname.cpp** | CFname 구현 | 경로, 파일명 파싱 | fname.h | MFC, CString |
| **CUploadFile.h** | 파일 업로드 | CUploadFile | CUploadFile.cpp | MFC |
| **CUploadFile.cpp** | CUploadFile 구현 | 파일 업로드 | CUploadFile.h | MFC, Winsock2 |
| **Infofile.h** | 정보 파일 | CInfofile | Infofile.cpp | MFC |
| **Infofile.cpp** | CInfofile 구현 | 레지스트리/파일 I/O | Infofile.h | MFC, Registry |

**라인 수**: 600 줄  
**복잡도**: 낮음-중간

---

### 13. 보안 및 인증 (Security & Authentication)

| 파일 | 역할 | 주요 클래스 | 관련 파일 | 의존성 |
|------|------|----------|---------|-------|
| **CertErrDialog.h** | 인증서 에러 | CCertErrDialog | CertErrDialog.cpp | MFC |
| **CertErrDialog.cpp** | CCertErrDialog 구현 | 인증서 에러 처리 | CertErrDialog.h | MFC |
| **CertLogin.h** | 인증서 로그인 | CCertLogin | (위 다이얼로그 참고) | MFC, Wincrypt |
| **passdlg.h** | 패스워드 입력 | CPassDlg | passdlg.cpp | MFC |
| **passdlg.cpp** | CPassDlg 구현 | 패스워드 입력 UI | passdlg.h | MFC |
| **PassEdit.h** | 패스워드 에디트 | CPassEdit | PassEdit.cpp | MFC, CEdit |
| **PassEdit.cpp** | CPassEdit 구현 | 마스킹 에디트 | PassEdit.h | MFC |

**라인 수**: 400 줄  
**복잡도**: 낮음

---

### 14. 네트워크 (Network)

| 파일 | 역할 | 주요 클래스 | 관련 파일 | 의존성 |
|------|------|----------|---------|-------|
| **NSocket.h** | 소켓 래퍼 | CNSocket | NSocket.cpp | MFC, Winsock2 |
| **NSocket.cpp** | CNSocket 구현 | TCP/UDP 소켓 | NSocket.h | Winsock2 |
| **custsite.h** | 커스텀 사이트 | CCustomOccManager | custsite.cpp | MFC, OLE |
| **custsite.cpp** | CCustomOccManager 구현 | OCX 호스팅 | custsite.h | MFC, OleCtl |

**라인 수**: 300 줄  
**복잡도**: 중간 (Winsock2)

---

### 15. 특수 기능 (Special Features)

| 파일 | 역할 | 주요 클래스 | 관련 파일 | 의존성 |
|------|------|----------|---------|-------|
| **APIHookEx.h** | API 후킹 | CAPIHookEx | APIHookEx.cpp | Windows.h |
| **APIHookEx.cpp** | CAPIHookEx 구현 | 함수 후킹 (디버깅) | APIHookEx.h | Windows API |
| **Chaser.h** | 명령어 추적 | CChaser | Chaser.cpp | MFC |
| **Chaser.cpp** | CChaser 구현 | 거래 명령 추적 (디버깅 도구) | Chaser.h | MFC, MainFrm.h |
| **MDebug.h** | 디버그 | CMDebug | MDebug.cpp | MFC |
| **MDebug.cpp** | CMDebug 구현 | 디버그 출력 | MDebug.h | MFC |
| **KApi2Launcher.h** | KApi2 실행 | CKApi2Launcher | KApi2Launcher.cpp | MFC |
| **KApi2Launcher.cpp** | CKApi2Launcher 구현 | 외부 프로세스 실행 | KApi2Launcher.h | Windows API |
| **FirstJob.h** | 초기 작업 | CFirstJob | (구현 미상) | MFC |
| **NewsViewer.h** | 뉴스 뷰어 | CNewsViewer | NewsViewer.cpp | MFC |
| **NewsViewer.cpp** | CNewsViewer 구현 | 뉴스 표시 | NewsViewer.h | MFC, IE/WebView |

**라인 수**: 500 줄  
**복잡도**: 낮음-중간

---

### 16. 레이아웃 및 UI 유틸 (Layout & UI Utils)

| 파일 | 역할 | 주요 클래스 | 관련 파일 | 의존성 |
|------|------|----------|---------|-------|
| **BkWnd.h** | 백그라운드 윈도우 | CBkWnd | BkWnd.cpp | MFC |
| **BkWnd.cpp** | CBkWnd 구현 | 배경 이미지 렌더링 | BkWnd.h | MFC, GDI |
| **XCaption.h** | 커스텀 캡션 | CXCaption | (구현 미상) | MFC |
| **button.h** | 버튼 | Cbutton | button.cpp | MFC |
| **button.cpp** | Cbutton 구현 | 기본 버튼 | button.h | MFC |
| **TTip.h** | 도구팁 | CTTip | TTip.cpp | MFC |
| **TTip.cpp** | CTTip 구현 | 도구팁 표시 | TTip.h | MFC |
| **MyToolTip.h** | 커스텀 도구팁 | CMyToolTip | (구현 미상) | MFC |
| **NProgressCtrl.h** | 프로그레스 | (위 컨트롤 참고) | NProgressCtrl.cpp | MFC |

**라인 수**: 400 줄  
**복잡도**: 낮음

---

### 17. 리소스 및 초기화 (Resource & Initialization)

| 파일 | 역할 | 주요 클래스 | 관련 파일 | 의존성 |
|------|------|----------|---------|-------|
| **ResourceHelper.h** | 리소스 관리 | (위 싱글톤 참고) | ResourceHelper.cpp | MFC, GDI |
| **RSCinfo.h** | 리소스 정보 | CRSCinfo | RSCinfo.cpp | MFC |
| **RSCinfo.cpp** | CRSCinfo 구현 | 리소스 로드 | RSCinfo.h | MFC |
| **PrinterSettings.h** | 프린터 설정 | CPrinterSettings | PrinterSettings.cpp | MFC |
| **PrinterSettings.cpp** | CPrinterSettings 구현 | 프린터 설정 | PrinterSettings.h | MFC, WinSpool |

**라인 수**: 300 줄  
**복잡도**: 낮음

---

### 18. 기타 다이얼로그 및 도구 (Misc)

| 파일 | 역할 | 주요 클래스 | 관련 파일 | 의존성 |
|------|------|----------|---------|-------|
| **TOP10View.h** | TOP10 뷰 | CTOP10View | TOP10View.cpp | MFC |
| **TOP10View.cpp** | CTOP10View 구현 | TOP10 순위표 | TOP10View.h | MFC, NGrid |
| **TOP10Dialog.h** | TOP10 다이얼로그 | CTOP10Dialog | TOP10Dialog.cpp | MFC |
| **TOP10Dialog.cpp** | CTOP10Dialog 구현 | TOP10 다이얼로그 | TOP10Dialog.h | MFC |
| **EmergencyDlg.h** | 긴급 상황 | CEmergencyDlg | EmergencyDlg.cpp | MFC |
| **EmergencyDlg.cpp** | CEmergencyDlg 구현 | 긴급 종료 확인 | EmergencyDlg.h | MFC |
| **ProgressDlg.h** | 진행률 다이얼로그 | CProgressDlg | ProgressDlg.cpp | MFC |
| **ProgressDlg.cpp** | CProgressDlg 구현 | 진행 표시 | ProgressDlg.h | MFC, NProgressCtrl |
| **Environment.h** | 환경 변수 | (헤더만) | - | (구현 미상) |
| **axmsg.hxx** | 메시지 정의 | (컴파일 타임 상수) | axis.cpp | C++ Preprocessor |
| **stdafx.h** | PCH | (모든 파일 포함) | StdAfx.cpp | MFC, Windows.h, STL |
| **StdAfx.cpp** | PCH 생성 | (empty) | stdafx.h | (PCH 생성만) |
| **whdump.h** | 미니덤프 | (함수 기반) | MainFrm.cpp | Windows.h, DbgHelp.lib |

**라인 수**: 500 줄  
**복잡도**: 낮음

---

## 파일 통계

### 전체 요약

| 카테고리 | 파일 수 | 예상 라인 수 | 복잡도 |
|---------|-------|-----------|--------|
| MFC 프레임워크 | 12 | 5000+ | 높음 |
| 동기화/예외 처리 | 4 | 200 | 낮음 |
| 워커 스레드 | 3 | 100 | 낮음 |
| 싱글톤/관리 | 4 | 500 | 중간 |
| 데이터 모델 | 12 | 1000+ | 중간 |
| 커스텀 컨트롤 | 25+ | 3500+ | 높음 |
| 다이얼로그 | 60+ | 10000+ | 중간 |
| 거래/주문 | 11 | 1000+ | 높음 |
| 시스템 정보 | 8 | 500 | 낮음 |
| 알림/이벤트 | 8 | 500 | 낮음 |
| 그래픽/렌더링 | 9 | 800 | 중간 |
| 파일 처리 | 10 | 600 | 낮음 |
| 보안/인증 | 7 | 400 | 낮음 |
| 네트워크 | 4 | 300 | 중간 |
| 특수 기능 | 11 | 500 | 낮음 |
| 레이아웃/UI | 9 | 400 | 낮음 |
| 리소스/초기화 | 5 | 300 | 낮음 |
| 기타 | 15+ | 500 | 낮음 |
| **총합** | **~200+** | **~30000+** | **중간-높음** |

### 주요 파일 (Top 10 복잡도)

1. **MainFrm.cpp** - ~1500 줄, 150+ 메시지 핸들러 (God Class)
2. **fxGrid.cpp** - ~800 줄, GDI 렌더링 복잡
3. **SetupC.cpp** - ~700 줄, 레지스트리 관리 복잡
4. **ConfigDlg.cpp** - ~600 줄, 설정 저장/로드
5. **SecureDlg.cpp** - ~500 줄, 인증 로직 복잡
6. **Chaser.cpp** - ~400 줄, 거래 추적
7. **ResourceHelper.cpp** - ~400 줄, 리소스 관리
8. **GDIHelper.cpp** - ~400 줄, GDI 유틸
9. **CpuUsage.cpp** - ~300 줄, Performance API
10. **NGrid.cpp** - ~300 줄, 그리드 렌더링

---

## 의존성 관계 요약

### 상향식 의존성 (Bottom-up)

```
Windows API (Winsock2, GDI, Wincrypt, etc.)
  ↓
MFC (CWnd, CDialog, CFrameWnd, etc.)
  ↓
Core Classes (CAxisApp, CMainFrame, CAxisDoc)
  ↓
Services (ResourceHelper, ExceptionHandler, etc.)
  ↓
Models & Controls (CDtInfo, CFXGrid, etc.)
  ↓
Dialogs & Views (CConfigDlg, CAxisView, etc.)
```

### 순환 의존성 위험

- **MainFrm.h ↔ ChildFrm.h** (헤더 가드로 보호된 것으로 추정)
- **MainFrm.h ↔ 다양한 다이얼로그** (메시지 핸들러 콜백)

---

## 빌드 순서 제안

### 단계 1: 기반 (Foundations)

1. stdafx.h, stdafx.cpp (PCH)
2. CriticalSection.h (동기화)
3. ExceptionHandler.h/cpp (예외 처리)
4. axis.h

### 단계 2: 핵심 앱 (Core App)

5. axis.cpp (CAxisApp)
6. axInstance.h/cpp (중복 실행 방지)
7. MainFrm.h, MainFrm.cpp
8. ChildFrm.h, ChildFrm.cpp
9. axisDoc.h/cpp
10. axisView.h/cpp, axScrollView.h/cpp, AxTabView.h/cpp

### 단계 3: 서비스 (Services)

11. ResourceHelper.h/cpp
12. CpuUsage.h/cpp, sysInfo.h/cpp
13. Dtconnect.h/cpp
14. PhoneThread.h/cpp

### 단계 4: 모델 (Models)

15. dtInfo.h/cpp
16. BaseMP.h/cpp, Conclusion.h/cpp
17. TotalAccount.h/cpp, TotalAcc.h/cpp

### 단계 5: 컨트롤 (Controls)

18. grid/* (NGrid.h/cpp, fxGrid.h/cpp, 등)
19. 버튼 및 기본 컨트롤 (BtnST, BmpButton, 등)

### 단계 6: 다이얼로그 & 뷰 (Dialogs & Views)

20. SecureDlg.h/cpp, CertLogin.h/cpp
21. ConfigDlg.h/cpp, SetupC.h/cpp
22. ... (60+ 다이얼로그)

### 단계 7: 특수 기능 (Special Features)

23. Chaser.h/cpp, NewsViewer.h/cpp
24. APIHookEx.h/cpp

---

## 분석 가능성 (Analyzability)

### 높음 (Easy to analyze)

- 개별 컨트롤 클래스 (BmpButton, NDib, etc.)
- 데이터 모델 (CDtInfo, CConclusion, etc.)
- 유틸리티 (fname, IhUtils, etc.)

### 중간 (Medium)

- 다이얼로그 (설정 저장/로드 로직)
- 그리드 컨트롤 (GDI 렌더링)
- 거래/주문 처리

### 낮음 (Hard to analyze)

- **MainFrm.cpp** (150+ 메시지 핸들러, 제어 흐름 복잡)
- 전체 메시지 흐름 (PostMessage 비동기)
- 초기화/종료 시퀀스 (싱글톤 의존성)

---

## 테스트 전략

### 단위 테스트 (Unit Test) 가능성

| 카테고리 | 가능성 | 예시 |
|---------|--------|------|
| 유틸리티 | 높음 | fname, IhUtils, GDIHelper |
| 모델 | 중간 | CDtInfo (순수 데이터), CConclusion |
| 서비스 | 중간 | CpuUsage (외부 API 목킹 필요) |
| 다이얼로그 | 낮음 | MFC 메시지 루프 의존성 |
| 프레임 | 낮음 | MainFrm (전역 상태 의존성) |

---

## 참고

- **Architecture.md**: 전체 구조 및 계층
- **Dependency.md**: 링크/런타임 의존성
- **ArchitectureReview.md**: 구조적 문제 및 개선 방안
- **CallGraph.md**: 호출 흐름 (분석 예정)
- **KnowledgeBase.md**: 설계 지식 및 트러블슈팅

