# Workflow: 코드 리뷰 (`code-review`)

## 목적

코드 변경 또는 기존 코드의 품질/구조/설계를 검토하고 피드백을 제공한다. **코드를
수정하지 않는다.**

## 자동 선택 트리거

- "코드 리뷰해줘", "이 변경 괜찮아?", "이 PR 봐줘", "이 코드 검토해줘"

## 공통 절차

[WORKFLOWS.md](WORKFLOWS.md)의 공통 1~5단계를 먼저 수행한다.

## Agent 실행 순서

| 순서 | Agent | 목적 | 실행 방식 |
|---|---|---|---|
| 1 | `architecture` | 구조/의존성 관점 검토 | mentor와 병렬 |
| 1 | `mentor` | SRP/디자인 패턴/유지보수성/기술 부채 관점 검토 | architecture와 병렬 |
| 2 | `performance` | 성능에 영향 있는 변경일 경우에만 추가 | 선택적 |
| 3 | `documentation` | 리뷰에서 나온 후속 과제를 Todo.md에 반영 | 순차 |

**`coding` Agent는 호출하지 않는다** — 리뷰는 수정이 아니다. 수정이 필요하면 리뷰 결과를
바탕으로 `bug-fix`, `refactor`, `feature` Workflow 중 적합한 것을 사용자에게 제안한다.

## 완료 후 절차

- 관련 문서 갱신: `docs/ArchitectureReview.md`, `docs/Todo.md`
- `docs/KnowledgeBase.md` 갱신: 리뷰 과정에서 드러난 설계 의도/제약 기록
- 결과 요약: 발견 사항을 심각도 순으로 정리해 보고 (실제 수정 여부는 사용자 결정에 맡김)

## 산출물

- 리뷰 결과 보고 (심각도별 발견 사항 + 근거 + 제안)
- 갱신된 `docs/ArchitectureReview.md`, `docs/Todo.md`

## 주의사항

- 절대 소스 코드를 직접 수정하지 않는다.
- 확실하지 않은 지적은 "추정"임을 명시한다.
