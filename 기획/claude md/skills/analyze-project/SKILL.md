---
name: analyze-project
description: C/C++ Windows 프로젝트(*.cpp, *.h, *.idl, *.vcxproj 등)를 정적으로 전수 분석하여 클래스/함수/네임스페이스/동시성 프리미티브/COM·MFC·ATL·STL·WinAPI 사용/디자인 패턴을 추출하고 Architecture.md, Dependency.md, SourceIndex.md에 반영한다. "프로젝트 분석해줘", "코드베이스 파악" 요청 시 사용.
---

# analyze-project

## 목적

프로젝트 전체를 정적으로 분석하여 구조(클래스/함수/네임스페이스), 의존성, 동시성 사용,
Windows/COM/MFC/ATL 관련 요소, 디자인 패턴 사용 현황을 추출하고 문서화한다.

## 입력

- 프로젝트 루트 경로
- 분석 대상 확장자: `*.cpp *.c *.h *.hpp *.idl *.rc *.vcxproj *.props *.targets *.json *.xml *.def *.sln`

## 수행 절차

1. 공통 사전 점검
   1. `CLAUDE.md` 읽기
   2. `docs/` 폴더 존재 확인 (없으면 init-project 실행을 먼저 권장)
   3. `docs/Architecture.md`, `docs/Dependency.md`, `docs/SourceIndex.md`가 있으면 먼저 읽는다.
   4. 기존 문서 내용을 최대한 재사용하고, 삭제하지 않는다.
   5. 가능하면 이전 분석 시점 이후 변경된 파일만 우선 재분석한다 (git이 있으면 `git status`/`git diff` 활용, 없으면 mtime 비교).
2. 대상 파일을 확장자별로 수집한다.
3. 파일별로 다음 항목을 추출한다.
   - 클래스, 구조체, 네임스페이스, 전역 변수
   - 함수 시그니처, Export 함수(`__declspec(dllexport)`, `.def` 파일), Import 함수/DLL
   - 정적 라이브러리(`.lib`) 링크 여부
   - Include 관계 (상대/절대 경로 정규화)
   - 동시성: Thread 생성(`CreateThread`, `std::thread`, `AfxBeginThread` 등), `Mutex`,
     `CriticalSection`, `std::atomic` / interlocked 함수
   - COM (`IUnknown`, `CoCreateInstance`, `.idl`), ATL (`CComObject`, `CComPtr` 등), MFC
     (`CWnd`, `CDialog`, 메시지 맵 `BEGIN_MESSAGE_MAP`)
   - STL 컨테이너/알고리즘 사용, WinAPI 호출
   - 예외 처리(`try/catch`, `__try/__except`, `CException`)
   - 디자인 패턴 후보: Singleton, Factory, Observer, Command (명명 규칙·구조적 특징으로 추정, 근거 명시)
4. 추출 결과를 기존 `Architecture.md` / `Dependency.md` / `SourceIndex.md`와 비교하여
   **변경/신규분만 반영**한다. 기존 서술은 유지하고, 사라진 항목은 삭제하지 말고
   `(코드에서 확인되지 않음 — 확인 필요)`로 표시한다.
5. 분석 결과 요약(파일 수, 클래스 수, 함수 수, 주요 발견 사항)을 보고한다.

## 출력

- `docs/Architecture.md` (갱신)
- `docs/Dependency.md` (갱신)
- `docs/SourceIndex.md` (갱신)

## 주의사항

- 기존 문서의 서술을 임의로 재작성하지 말고, 변경분만 반영한다.
- 확실하지 않은 디자인 패턴 판정은 추정임을 명시한다.
- 파일이 매우 많은 경우 배치 단위로 나누어 처리하고 중간 진행 상황을 보고한다.
- 이미 존재하는 문서를 삭제하지 않는다.
