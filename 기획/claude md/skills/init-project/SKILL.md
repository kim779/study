---
name: init-project
description: C/C++ Windows 프로젝트를 Claude Harness 문서 구조(docs/ 하위 7종 문서, 표준 디렉토리)로 초기화한다. 새 프로젝트를 시작하거나 docs 폴더/CLAUDE.md가 없을 때, 또는 사용자가 "프로젝트 초기화"를 요청할 때 사용한다.
---

# init-project

## 목적

프로젝트를 Claude Harness 구조(문서 체계 + 표준 디렉토리)로 초기화하여, 이후의
analyze-project / dependency-map / architecture-review / source-index / call-graph /
knowledge-base / update-docs / generate-report Skill들이 일관된 기반 위에서 동작하도록 만든다.

이 Skill은 "초기화 전용"이다. 실제 코드 분석(클래스/함수/의존성 추출 등)은 수행하지 않는다.

## 입력

- 프로젝트 루트 경로 (기본값: 현재 작업 디렉토리)
- (선택) 프로젝트 이름, 한 줄 설명 — 없으면 사용자에게 묻거나 TODO로 남긴다.

## 수행 절차

1. 공통 사전 점검
   1. 프로젝트 루트에 `CLAUDE.md`가 있는지 확인하고, 있으면 읽는다.
   2. `docs/` 폴더 존재 여부를 확인한다.
   3. `docs/` 안에 `Architecture.md`, `Dependency.md`, `KnowledgeBase.md`,
      `SourceIndex.md`, `CallGraph.md`, `Build.md`, `Todo.md`가 이미 있으면 먼저 읽는다.
   4. 이미 존재하는 문서·설정 파일은 절대 삭제하거나 덮어쓰지 않는다. 최대한 재사용한다.
2. 표준 디렉토리 생성 (이미 존재하는 디렉토리는 건드리지 않는다)
   - `docs/`, `src/`, `include/`, `build/`, `test/`, `scripts/`, `tools/`
3. `docs/` 내 기본 문서를 생성한다. **이미 존재하는 파일은 절대 건드리지 않는다.**
   존재하지 않는 문서만 아래 최소 템플릿으로 새로 만든다.
   - 제목, 생성일, 한 줄 목적 설명
   - "## 개요", "## 목차" 등 뼈대 섹션
   - 아직 분석 전이므로 본문은 `(분석 전 — analyze-project 등 실행 필요)`로 표시
4. `CLAUDE.md`가 없는 경우에만, 프로젝트 개요 / 빌드 방법 / 코딩 규칙 자리표시자를 담은
   최소 골격을 생성한다. 이미 있으면 절대 수정하지 않는다.
5. 프로젝트 성격(라이브러리 / 실행 파일 / 서비스 / DLL 등)을 코드에서 바로 판단할 수 없으면
   문서에 TODO로 표시하고, 필요 시 사용자에게 확인을 요청한다.
6. 생성한 항목과 이미 존재하여 건너뛴 항목을 구분하여 목록으로 요약 보고한다.

## 출력

- `docs/Architecture.md`
- `docs/Dependency.md`
- `docs/KnowledgeBase.md`
- `docs/SourceIndex.md`
- `docs/CallGraph.md`
- `docs/Build.md`
- `docs/Todo.md`
- (없었을 경우) `CLAUDE.md`
- 표준 디렉토리 (`src/`, `include/`, `build/`, `test/`, `scripts/`, `tools/`)

## 주의사항

- 이미 존재하는 파일/폴더는 절대 덮어쓰거나 삭제하지 않는다.
- 실제 코드 분석은 하지 않는다. 초기화 이후 analyze-project를 실행하도록 안내한다.
- 프로젝트 성격을 추측으로 단정하지 말고 불확실하면 TODO/질문으로 남긴다.
