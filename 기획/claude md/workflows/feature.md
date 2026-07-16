# Workflow: 신규 기능 개발 (`feature`)

## 목적

새로운 기능을 기존 아키텍처와 일관되게 설계·구현하고 검증·문서화한다.

## 자동 선택 트리거

- "추가해줘", "기능을 만들어줘", "구현해줘", "새 화면/모듈/API 필요"

## 공통 절차

[WORKFLOWS.md](WORKFLOWS.md)의 공통 1~5단계를 먼저 수행한다.

## Agent 실행 순서

| 순서 | Agent | 목적 | 실행 방식 |
|---|---|---|---|
| 1 | `architecture` | 기능을 어느 계층/모듈에 둘지, 기존 구조와의 정합성 검토 | 순차 |
| 2 | `coding` | 기능 구현 (애매하면 architecture에 재확인) | 순차 |
| 3 | `testing` | 신규 기능에 대한 테스트 시나리오/코드 작성 | 순차 |
| 4 | `documentation` | Architecture/SourceIndex/CallGraph 등 반영 | 순차 |
| - | `performance` | 성능에 민감한 기능일 경우에만 추가 | 선택적, coding 이후 병렬 검토 가능 |

## 완료 후 절차

- 관련 문서 갱신: `docs/Architecture.md`, `docs/SourceIndex.md`, `docs/CallGraph.md`(흐름
  변경 시), `docs/Todo.md`(후속 개선 과제)
- `docs/KnowledgeBase.md` 갱신: 기능의 목적/설계 의도/사용 API 기록
- 결과 요약: 구현 범위 → 설계 근거 → 테스트 결과 → 문서 반영 내역

## 산출물

- 신규/변경된 소스 코드
- 테스트 코드 또는 시나리오
- 갱신된 관련 문서

## 주의사항

- 요청 범위를 벗어난 부가 기능을 임의로 추가하지 않는다.
- 기존 아키텍처를 우회해야 한다면 반드시 `architecture` Agent의 검토를 거친다.
