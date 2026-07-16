# Workflow: 아키텍처 분석 (`architecture-analysis`)

## 목적

프로젝트의 구조(DLL 관계, Layer, Module, Thread, Message Flow 등)를 분석하고 문서화한다.

## 자동 선택 트리거

- "아키텍처 분석해줘", "구조가 어떻게 돼", "이 프로젝트 구조 파악", "모듈 관계 보여줘"

## 공통 절차

[WORKFLOWS.md](WORKFLOWS.md)의 공통 1~5단계를 먼저 수행한다.

## Agent 실행 순서

| 순서 | Agent | 목적 | 실행 방식 |
|---|---|---|---|
| 1 | `architecture` | DLL/Layer/Module/Thread/Message Flow/Dependency/리팩터링 후보 분석 | 순차 (단독 수행 가능) |
| 2 | `documentation` | 분석 결과를 문서 체계에 정리, Mermaid 다이어그램 반영 | architecture 이후 |

## 완료 후 절차

- 관련 문서 갱신: `docs/Architecture.md`, `docs/ArchitectureReview.md`, `docs/Dependency.md`
- `docs/KnowledgeBase.md` 갱신: 설계 의도 관련 발견 사항 기록
- 결과 요약: 구조 개요 + 주요 발견(리스크/리팩터링 후보) 하이라이트

## 산출물

- `docs/Architecture.md`, `docs/ArchitectureReview.md`, `docs/Dependency.md`

## 주의사항

- 코드를 수정하지 않는다.
- 확실하지 않은 구조적 판단은 "추정"으로 표기한다.
