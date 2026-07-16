# Workflow: 리팩터링 (`refactor`)

## 목적

동작 변경 없이 구조적 품질(응집도, 결합도, SRP, 가독성)을 개선한다.

## 자동 선택 트리거

- "리팩터링해줘", "구조 개선", "정리해줘", "이 클래스 나눠줘"

## 공통 절차

[WORKFLOWS.md](WORKFLOWS.md)의 공통 1~5단계를 먼저 수행한다.

## Agent 실행 순서

| 순서 | Agent | 목적 | 실행 방식 |
|---|---|---|---|
| 1 | `architecture` | 리팩터링 대상의 현황과 후보 분석 (ArchitectureReview.md 활용) | 순차 |
| 2 | `mentor` | 설계 관점에서 방향성 검토 (선택적, 큰 구조 변경일 때 권장) | 선택적, architecture 이후 |
| 3 | `coding` | 실제 리팩터링 적용 (동작 불변 원칙) | 순차 |
| 4 | `testing` | 회귀 검증 (리팩터링 전후 동작 동일성 확인) | 순차 |
| 5 | `documentation` | 구조 변경 사항 반영 | 순차 |

## 완료 후 절차

- 관련 문서 갱신: `docs/Architecture.md`, `docs/ArchitectureReview.md`(해결된 항목 표시)
- `docs/KnowledgeBase.md` 갱신: 리팩터링 이유와 이전/이후 구조 차이 기록
- 결과 요약: 대상 → 변경 방식 → 동작 불변 검증 결과

## 산출물

- 리팩터링된 소스 코드
- 회귀 테스트 결과
- 갱신된 관련 문서

## 주의사항

- 기능 동작을 변경하지 않는다 (동작 변경이 필요하면 `feature` 또는 `bug-fix` Workflow로 전환).
- 요청받지 않은 범위까지 임의로 리팩터링하지 않는다.
