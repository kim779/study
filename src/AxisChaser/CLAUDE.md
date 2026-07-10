# CLAUDE.md (AxisChaser)

## 프로젝트 개요

MFC 기반 Windows 실행 파일(Application). AXIS 트레이딩 워크스테이션 프로세스가
`WM_COPYDATA`로 전달하는 송수신 데이터/RTM(실시간)/시스템 트레이스/리포트 데이터를 받아
헥사덤프·파싱 텍스트로 색상/폰트를 입혀 RichEdit 컨트롤에 보여주는 **트레이스 뷰어(디버깅
도구)**. "Chaser"는 통신 데이터를 추적해 보여준다는 의미. 자세한 구조는
[docs/Architecture.md](docs/Architecture.md) 참고.

## 빌드 방법

RichLib 서브프로젝트(`RichLib/RichLib.vcxproj`)를 먼저 빌드해 `Debug/RichLib.lib` 또는
`Release/RichLib.lib`를 생성한 뒤 `AxisChaser.vcxproj`를 빌드해야 링크된다. 자세한 내용은
[docs/Dependency.md](docs/Dependency.md) 참고.

## 코딩 규칙

(TODO)
