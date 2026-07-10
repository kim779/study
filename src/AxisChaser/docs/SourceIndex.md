# SourceIndex

- 생성일: 2026-07-10
- 목적: 파일 단위로 역할, 주요 클래스/함수, 사용 API, 의존 모듈을 색인화한다.

## 파일 목록

| 파일 | 역할 | 주요 클래스/함수 | 의존 |
|---|---|---|---|
| `AxisChaser.cpp/.h` | 앱 진입점 | `CAxisChaserApp`(`InitInstance`, `IsFirstInstance`), `CCommParam`(`ParseParam`), `CAboutDlg` | `MainFrm.h`, `chaser.h` |
| `MainFrm.cpp/.h` | 메인 프레임 | `CMainFrame`(`OnCreate`, `OnCopyData`, `OnChaserDisplay`, `OnClose`) | `EnBitmap.h`, `ChildView.h`, `chaser.h` |
| `ChildView.cpp/.h` | 핵심 뷰(트레이스 표시/파싱/큐잉/검색) | `CChildView`(`CopyData`, `OnReceive`, `OnRCVData`, `addTrace`, `parseData`, `ReportParse`, `SearchData`/`SearchUP`, `loadOptions`, `Xecure`), `CQue` | `RichLib/RichEditCtrlEx.h`, `chaser.h`, `TextColorDlg.h`, `FontSetDlg.h`, `Options.h`, `../H/axis.h`, `../H/axisfire.h` |
| `chaser.h` | 공용 상수 정의 | 메시지 ID(`WM_AXIS`,`WM_RECEIVE`), 데이터 타입 코드(`x_STRs`,`x_RTMs`,`x_SNDs`,`x_RCVs`,`x_CONs`), 레지스트리 키 문자열 | - |
| `Options.cpp/.h` | 옵션 다이얼로그 | `COptions`(`OnInitDialog`, `loadOptions`, `saveOptions`, `OnAdd`, `OnRemove`) | `chaser.ini` |
| `FontSetDlg.cpp/.h` | 카테고리별 폰트 설정 다이얼로그 | `CFontSetDlg`(`loadInfo`, `saveInfo`, `ChangeFont`, `OnSetallfont`/`OnSetsnrfont`/`OnSetrtmfont`/`OnSetsysfont`/`OnSetrptfont`) | 레지스트리(`TRACESETTING`) |
| `TextColorDlg.cpp/.h` | 카테고리별 텍스트 색상 설정 다이얼로그 | `CTextColorDlg`(`loadInfo`, `saveInfo`, `ChangeColor`, `OnCtlColor`) | 레지스트리(`TRACESETTING`) |
| `Color.cpp/.h` | RGB ↔ HLS 색상 변환 유틸 | `CColor`(`SetHue`/`SetSaturation`/`SetLuminance`, `ToRGB`/`ToHLS`) | 없음(독립 유틸, 사용처 미확인) |
| `EnBitmap.cpp/.h` | 32비트 비트맵 이미지 가공, 컬러 툴바 | `CEnBitmap`(`LoadImage`, `GrayImage`, `MakeDisabled`/`MakeNormal`/`MakeNotActive`), `C32BitImageProcessor`+파생(`CImageNormal`,`CImageGrayer`,`CImageHigh`), `CToolBar24`(`SetFullColorImage`, `UpdateBk`) | `afxtempl.h` |
| `StdAfx.cpp/.h` | PCH | - | `afxwin.h` 등 |
| `resource.h` | 리소스 ID 정의 | - | `AxisChaser.rc` |
| `RichLib/RichEditCtrlEx.cpp/.h` | (서브프로젝트) RTF 스트림 빌더 + RichEdit 확장 | `CRichEditCtrlEx`, `CRTFBuilder`(연산자 `<<`/`>>`), `CFontList`, `CColorList`, `CManip` 파생(`CStringManip`,`CIntManip`,`CBoolManip`,`CControlManip`) | `atlbase.h`, `<list>`, `<stack>` |

## 비고

- `Release/` 디렉토리는 빌드 산출물(포함하지 않음).
- `RichLib/.vs/`, `RichLib/Backup/`은 IDE/백업 산출물(분석 대상 아님).
