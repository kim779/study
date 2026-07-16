# Workflow: 릴리즈 (`release`)

## 목적

배포 전 산출물(DLL/EXE)의 무결성과 배포 요건을 검증하고 체크리스트를 완성한다.

## 자동 선택 트리거

- "배포 준비", "릴리즈 체크", "배포해도 되나요", "버전 올려서 배포"

## 공통 절차

[WORKFLOWS.md](WORKFLOWS.md)의 공통 1~5단계를 먼저 수행한다.

## Agent 실행 순서

| 순서 | Agent | 목적 | 실행 방식 |
|---|---|---|---|
| 1 | `release` | DLL Export/Version/PDB/VC Runtime/의존성/누락 DLL 검증 | 순차 |
| 2 | `testing` | 최종 검증 필요 시 회귀/스모크 테스트 | 선택적 |
| 3 | `coding` | 배포 차단 이슈 발견 시 수정 | 조건부 |
| 4 | `documentation` | 체크리스트/Build.md 반영 | 순차 |

## 완료 후 절차

- 관련 문서 갱신: `docs/ReleaseChecklist.md`, `docs/Build.md`
- `docs/KnowledgeBase.md` 갱신: 배포 관련 특이사항(환경별 이슈 등) 기록
- 결과 요약: 체크리스트 통과/실패 항목, 배포 가능 여부에 대한 명확한 판단

## 산출물

- `docs/ReleaseChecklist.md`
- 배포 차단 이슈가 있었다면 수정된 코드

## 주의사항

- 확인 불가능한 항목을 임의로 통과 처리하지 않는다 — "확인 불가"로 명시한다.
- 배포 가능 여부는 명확히 Yes/No로 결론짓고 근거를 함께 제시한다.
