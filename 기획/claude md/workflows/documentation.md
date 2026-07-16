# Workflow: 문서 생성/최신화 (`documentation`)

## 목적

프로젝트 문서를 코드 현황과 일치하도록 생성하거나 최신화한다.

## 자동 선택 트리거

- "문서 만들어줘", "문서화해줘", "문서 최신화", "보고서 생성해줘"

## 공통 절차

[WORKFLOWS.md](WORKFLOWS.md)의 공통 1~5단계를 먼저 수행한다 (이 Workflow에서는 4~5단계
자체가 핵심 작업과 상당 부분 겹친다).

## Agent 실행 순서

| 순서 | Agent | 목적 | 실행 방식 |
|---|---|---|---|
| 1 | `documentation` | 필요한 Skill(source-index/dependency-map/call-graph/generate-report/
  update-docs) 호출 및 문서 갱신 | 순차 (단독 수행 가능) |
| 2 | `knowledge` | 문서화 과정에서 드러난 지식을 KnowledgeBase.md에 반영 (선택적) | documentation 이후 |

## 완료 후 절차

- 관련 문서 갱신: 요청 범위에 해당하는 `docs/*.md` 전체 (삭제 금지, 변경분만 반영)
- `docs/KnowledgeBase.md` 갱신
- 결과 요약: 갱신된 문서 목록과 주요 변경 내용

## 산출물

- 갱신/생성된 `docs/*.md` 문서 (요청 범위에 따라 Architecture/Dependency/SourceIndex/
  CallGraph/ProjectReport 등)

## 주의사항

- 코드를 수정하지 않는다.
- 기존 문서의 서술을 임의로 재작성하지 않고 변경분만 반영한다.
