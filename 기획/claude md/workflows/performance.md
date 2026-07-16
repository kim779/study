# Workflow: 성능 개선 (`performance`)

## 목적

성능 병목을 분석하고, 근본 원인에 기반한 개선을 적용·검증한다.

## 자동 선택 트리거

- "느려요", "성능 개선", "병목", "CPU/메모리 많이 써요", "응답이 느립니다"

## 공통 절차

[WORKFLOWS.md](WORKFLOWS.md)의 공통 1~5단계를 먼저 수행한다.

## Agent 실행 순서

| 순서 | Agent | 목적 | 실행 방식 |
|---|---|---|---|
| 1 | `performance` | CPU/Memory/Thread/Lock/Cache/Socket/I·O 병목 분석 | 순차 |
| 2 | `architecture` | 병목의 구조적 원인 확인 (필요 시) | 선택적, performance 이후 |
| 3 | `coding` | 개선안 구현 | 순차 |
| 4 | `testing` | 성능/회귀 검증 (Stress Test 포함) | 순차 |
| 5 | `documentation` | 결과 반영 | 순차 |

## 완료 후 절차

- 관련 문서 갱신: `docs/PerformanceReport.md`, 구조적 원인이 있었다면 `docs/ArchitectureReview.md`
- `docs/KnowledgeBase.md` 갱신: 병목 원인과 개선 방식, 트레이드오프 기록
- 결과 요약: 병목 원인 → 개선 내용 → 측정/추정 효과 → 남은 리스크

## 산출물

- 개선된 소스 코드
- 성능 검증 결과 (실측 또는 추정, 반드시 구분하여 명시)
- 갱신된 관련 문서

## 주의사항

- 실측 데이터가 없으면 개선 효과를 단정하지 않고 "추정"으로 표기한다.
- 성능을 위해 가독성/유지보수성을 크게 희생해야 하는 경우 그 트레이드오프를 명시하고
  사용자 확인을 받는다.
