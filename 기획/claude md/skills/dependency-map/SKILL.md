---
name: dependency-map
description: 프로젝트의 Header Include, DLL/LIB, COM, 외부 SDK(OpenSSL, SQLite, WebView2, Boost), MFC/ATL, Windows API 의존성을 분석하여 Dependency.md에 Mermaid 그래프와 함께 문서화한다. "의존성 분석", "이 프로젝트가 뭘 링크하는지" 질문 시 사용.
---

# dependency-map

## 목적

프로젝트가 내부적으로(헤더 include, 모듈 간) 그리고 외부적으로(DLL, LIB, SDK, Windows API)
어떤 것에 의존하는지 문서화하여 빌드/이식/영향도 분석의 기준을 제공한다.

## 입력

- 프로젝트 루트 경로
- `*.vcxproj`, `*.props`, `*.targets`, `*.sln`, 소스/헤더 파일

## 수행 절차

1. 공통 사전 점검
   1. `CLAUDE.md` 읽기
   2. `docs/` 폴더 확인, `docs/Dependency.md`가 있으면 먼저 읽는다.
   3. 기존 내용을 최대한 재사용하고 삭제하지 않는다.
   4. 가능하면 이전 분석 이후 변경된 프로젝트/소스 파일만 재분석한다.
2. 내부 의존성 분석
   - 헤더 include 그래프 (파일 단위, 가능하면 모듈/디렉토리 단위로 집계)
   - 순환 include 여부 표시 (있으면 별도로 강조)
3. 외부 의존성 분석
   - `.vcxproj`/`.props`의 `AdditionalDependencies`, `AdditionalIncludeDirectories`에서
     DLL/LIB 목록 추출
   - COM 사용 여부 (`.idl`, `CoCreateInstance`, typelib 참조)
   - 알려진 외부 SDK 탐지: OpenSSL, SQLite, WebView2, Boost 등 (헤더 경로/라이브러리명 패턴 매칭)
   - MFC/ATL 사용 여부 (`Use of MFC` 프로젝트 설정, `afx*.h`/`atl*.h` include)
   - Windows API 계열 사용 (`windows.h`, `winsock2.h`, `wincrypt.h` 등 주요 헤더 기준 카테고리화)
4. 결과를 기존 `Dependency.md`와 비교하여 변경분만 반영한다. 사라진 의존성은 삭제하지 말고
   `(현재 코드에서 확인되지 않음)`으로 표시한다.
5. Mermaid `graph` 문법으로 모듈/DLL 간 의존 관계도를 생성한다 (노드가 너무 많으면 모듈 단위로 축약).

## 출력

- `docs/Dependency.md`

## 주의사항

- 순환 의존성은 반드시 눈에 띄게 표시한다 (예: ⚠️ 표기).
- Mermaid 다이어그램은 가독성을 위해 노드 수를 적절히 그룹화한다 (파일 단위가 너무 많으면 모듈 단위로).
- 기존 문서의 서술을 임의 재작성하지 말고 변경분만 반영한다.
