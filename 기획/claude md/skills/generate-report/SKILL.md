---
name: generate-report
description: 기존 docs 문서(Architecture, Dependency, SourceIndex, ArchitectureReview, KnowledgeBase 등)를 종합하여 프로젝트 개요/구조/규모/위험요소/기술부채/리팩터링 우선순위를 담은 ProjectReport.md를 생성한다. "전체 보고서 만들어줘", "프로젝트 요약 보고서" 요청 시 사용.
---

# generate-report

## 목적

프로젝트 전체를 아우르는 종합 보고서를 생성하여, 현재 상태와 리스크, 개선 방향을
한 문서에서 파악할 수 있게 한다.

## 입력

- `docs/Architecture.md`, `docs/Dependency.md`, `docs/SourceIndex.md`,
  `docs/ArchitectureReview.md`, `docs/KnowledgeBase.md`, `docs/CallGraph.md`,
  `docs/Build.md`, `docs/Todo.md` (존재하는 것만 사용)

## 수행 절차

1. 공통 사전 점검
   1. `CLAUDE.md` 읽기
   2. `docs/` 폴더 확인
   3. 위 문서들을 가능한 한 모두 읽는다. 없는 문서가 있으면 해당 섹션은
      "(선행 분석 없음 — 해당 Skill 실행 필요)"로 표시하고, 이 Skill이 새로 전체 분석을
      수행하지는 않는다 (다른 분석 Skill의 산출물을 종합하는 것이 목적).
   4. 기존 `docs/ProjectReport.md`가 있으면 읽고, 삭제하지 않는다.
2. 아래 항목을 기존 문서에서 종합하여 작성한다.
   - 프로젝트 개요 (목적, 주요 기능 — CLAUDE.md/Architecture.md 기반)
   - 디렉토리 구조 (트리 형태)
   - 아키텍처 요약 (Architecture.md 기반, 다이어그램 포함)
   - DLL/모듈 관계 (Dependency.md 기반, Mermaid 그래프)
   - 클래스 수, 함수 수, 코드 규모(파일 수, 대략적 라인 수) — SourceIndex.md 및 실측 집계
   - 주요 위험 요소 (ArchitectureReview.md 기반)
   - 기술 부채 목록
   - 리팩터링 우선순위 (심각도/영향 범위 기준 정렬)
   - 향후 개선 과제 (Todo.md 및 KnowledgeBase.md 반영)
3. 표와 Mermaid 다이어그램을 적극 활용하여 큰 프로젝트도 읽기 쉽게 구성한다.
4. 기존 `ProjectReport.md`가 있었다면 변경된 수치/내용만 갱신하고 나머지 서술은 유지한다.

## 출력

- `docs/ProjectReport.md`

## 주의사항

- 이 Skill은 원본 코드를 새로 분석하지 않는다 — 기존 docs 산출물을 종합하는 역할이다.
  선행 문서가 부족하면 어떤 Skill을 먼저 실행해야 하는지 명시적으로 안내한다.
- 수치(클래스 수, 함수 수 등)는 출처 문서를 명시하여 신뢰도를 표시한다.
- 기존 보고서를 삭제하지 않고 갱신한다.
