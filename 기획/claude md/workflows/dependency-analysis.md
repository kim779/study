# Workflow: 의존성 분석 (`dependency-analysis`)

## 목적

프로젝트의 내부(Include)/외부(DLL, LIB, SDK, WinAPI) 의존성을 분석하고 문서화한다.

## 자동 선택 트리거

- "의존성 분석해줘", "이 프로젝트가 뭘 링크하나요", "외부 SDK 뭐 쓰나", "순환 의존성 있나요"

## 공통 절차

[WORKFLOWS.md](WORKFLOWS.md)의 공통 1~5단계를 먼저 수행한다.

## Agent 실행 순서

| 순서 | Agent | 목적 | 실행 방식 |
|---|---|---|---|
| 1 | `architecture` | `dependency-map` Skill 활용해 Include/DLL/LIB/COM/외부 SDK/WinAPI 의존성 분석 | 순차 (단독 수행 가능) |
| 2 | `documentation` | Dependency.md 정리, Mermaid 그래프 반영 | architecture 이후 |

## 완료 후 절차

- 관련 문서 갱신: `docs/Dependency.md`
- `docs/KnowledgeBase.md` 갱신: 외부 SDK 버전/제약 등 코드로 드러나지 않는 사실 기록
- 결과 요약: 주요 외부 의존성 목록 + 순환 의존성/위험 요소 하이라이트

## 산출물

- `docs/Dependency.md`

## 주의사항

- 순환 의존성은 반드시 눈에 띄게 표시한다.
- 코드를 수정하지 않는다.
