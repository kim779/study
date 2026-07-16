---
name: update-docs
description: 이전 분석 이후 변경된 파일만 다시 분석하여 Architecture/Dependency/SourceIndex/CallGraph/ArchitectureReview 등 기존 문서를 삭제 없이 부분 갱신한다. "문서 최신화", "변경사항만 반영해줘" 요청 시 사용.
---

# update-docs

## 목적

코드가 변경된 이후 문서 전체를 재생성하지 않고, **변경된 부분만** 문서에 반영하여
최신 상태를 유지한다.

## 입력

- 프로젝트 루트 경로
- 기존 `docs/` 문서 일체
- 변경 범위 판단 근거 (git 저장소면 `git status`/`git diff`, 아니면 파일 mtime과
  문서 내 "마지막 분석일" 메타데이터 비교)

## 수행 절차

1. 공통 사전 점검
   1. `CLAUDE.md` 읽기
   2. `docs/` 폴더 존재 확인 (없으면 init-project 먼저 실행하도록 안내하고 중단)
   3. `Architecture.md`, `Dependency.md`, `KnowledgeBase.md`, `SourceIndex.md`,
      `CallGraph.md`, `Build.md`, `Todo.md`, (있으면) `ArchitectureReview.md`를 모두 읽는다.
2. 변경된 파일 목록을 산출한다.
   - git 저장소인 경우: 마지막 분석 커밋(문서에 기록되어 있으면) 이후 `git diff --name-only`
   - git이 아닌 경우: 각 문서에 기록된 "마지막 분석일"과 파일 mtime 비교
3. 변경된 파일에 한해서만 analyze-project / dependency-map / source-index / call-graph의
   분석 절차를 적용한다 (전체 재분석 금지).
4. 산출된 새 분석 결과를 기존 문서와 **줄 단위/섹션 단위로 비교**하여 달라진 부분만 교체한다.
   - 변경되지 않은 섹션은 그대로 둔다.
   - 삭제된 코드 요소는 문서에서 바로 지우지 않고 "(삭제됨 — 코드에서 확인되지 않음)"으로 표시한다.
5. 각 문서 상단의 "마지막 갱신일"(또는 커밋 해시) 메타데이터를 갱신한다.
6. 이번 실행에서 갱신된 문서와 변경 내역을 요약 보고한다.

## 출력

- 변경분이 반영된 `docs/*.md` (기존 파일 제자리 갱신, 신규 파일 생성 없음)

## 주의사항

- 전체 재분석/재작성을 하지 않는다 — 반드시 변경분만 반영한다.
- 어떤 문서도 삭제하지 않는다.
- 변경 범위를 판단할 근거가 전혀 없다면(git도 없고 메타데이터도 없다면) 사용자에게
  전체 재분석(analyze-project) 여부를 확인한다.
