# Workflow 색인 (전역 개발 표준)

이 디렉토리는 "Workflow" 계층의 정의를 담는다. Workflow는 Claude Code의 네이티브
primitive가 아니다(Skill/Agent와 달리 도구가 직접 인식하지 않는다). 대신
`~/.claude/CLAUDE.md`의 전역 정책이 Claude에게 "개발 작업 요청 시 이 디렉토리에서
적절한 Workflow를 찾아 project-manager Agent를 통해 실행하라"고 지시함으로써
아래 계층을 실질적으로 구현한다.

```text
User
  ↓ (요청 분석 → 적절한 Workflow 자동 선택)
Workflow   (이 디렉토리, *.md)
  ↓ (Workflow가 정의한 Agent 순서대로 호출)
Agent      (~/.claude/agents/*.md, project-manager가 오케스트레이션)
  ↓ (Agent가 필요한 Skill 호출)
Skill      (~/.claude/skills/*.md)
  ↓
Result
```

## Workflow 목록 및 자동 선택 기준

| Workflow 파일 | 트리거 예시 | 관여 Agent |
|---|---|---|
| [bug-fix.md](bug-fix.md) | 버그 수정, 오류, 크래시, "안 돼요" | architecture, knowledge, coding, testing, documentation |
| [feature.md](feature.md) | 신규 기능, "추가해줘", "구현해줘" | architecture, coding, testing, documentation |
| [refactor.md](refactor.md) | 리팩터링, 구조 개선, "정리해줘" | architecture, mentor, coding, testing, documentation |
| [code-review.md](code-review.md) | 코드 리뷰, "이 변경 봐줘", "괜찮은지 검토" | architecture, mentor, performance(선택), documentation |
| [performance.md](performance.md) | 성능 개선, "느려요", 병목 | performance, architecture(선택), coding, testing, documentation |
| [documentation.md](documentation.md) | 문서 생성/최신화, "문서화해줘" | documentation, knowledge(선택) |
| [testing.md](testing.md) | 테스트 작성/검증, "테스트해줘" | testing, coding(조건부), documentation |
| [release.md](release.md) | 릴리즈, 배포 준비, "배포해도 되나" | release, testing(선택), coding(조건부), documentation |
| [architecture-analysis.md](architecture-analysis.md) | 아키텍처 분석, "구조가 어떻게 돼" | architecture, documentation |
| [dependency-analysis.md](dependency-analysis.md) | 의존성 분석, "뭘 링크하나", 외부 SDK 파악 | architecture, documentation |

## Workflow 공통 규칙

모든 Workflow는 다음 9단계를 따른다 (개별 Workflow 문서는 5단계까지의 공통 절차를
반복 서술하지 않고 이 문서를 참조한다).

1. `CLAUDE.md` 확인
2. 프로젝트 구조 확인
3. `docs/` 확인
4. `docs/KnowledgeBase.md` 확인
5. 필요하면 `update-docs` Skill 수행
6. 필요한 Agent 실행 (Workflow별로 정의된 순서/병렬 구성에 따름)
7. 관련 문서 갱신 (기존 문서 삭제 금지, 변경분만 반영)
8. `docs/KnowledgeBase.md` 갱신 (삭제 금지, 병합)
9. 결과 요약 보고

## Workflow 설계 원칙

- 하나의 Workflow는 하나의 명확한 목적만 가진다.
- Workflow는 재사용 가능해야 하며 다른 Workflow와 독립적이어야 한다.
- Workflow는 필요한 Agent만 호출한다 (과호출 금지).
- Workflow는 절대 코드를 직접 수정하지 않는다 — 반드시 Agent(주로 `coding`)를 통해 수행한다.
- Agent는 다른 Agent의 책임을 침범하지 않는다 (예: code-review Workflow는 `mentor`/`architecture`만
  호출하고 `coding`은 호출하지 않는다 — 리뷰는 수정이 아니다).

## Workflow 자동 선택 규칙

1. 사용자 요청을 분석해 위 표의 트리거와 매칭되는 Workflow를 자동 선택한다. 사용자가
   Workflow를 명시하지 않아도 항상 적용한다.
2. 정확히 일치하는 Workflow가 없으면, 가장 유사한 Workflow를 선택해 적용하되 차이점을
   보고한다.
3. 그래도 적절한 Workflow가 없으면 project-manager Agent가 임시로 Agent 조합을 판단해
   수행하고, 그 사실과 사용된 Agent 조합을 결과 보고에 명시한다.

## Workflow 확장 정책

- 새로운 Workflow는 기존 Agent/Skill을 최대한 재사용하여 정의한다. 기존 Workflow와
  목적이 중복되면 새로 만들지 않고 기존 Workflow를 보완한다.
- **반복 신규 유형 규칙**: 기존 Workflow 중 어느 것과도 잘 맞지 않는 새로운 유형의 작업
  요청이 들어오면, project-manager Agent는 이를 `docs/KnowledgeBase.md`의
  "미분류 작업 유형" 섹션에 (요청 유형, 발생 일자, 사용한 임시 Agent 조합)로 기록한다.
  같은/유사한 유형이 **3회 이상 누적**되면, 다음을 수행한다.
  1. 먼저 기존 Workflow 중 하나를 일반화/확장하여 처리 가능한지 검토한다.
  2. 적합한 기존 Workflow가 없다고 판단되면, 이 디렉토리에 새 Workflow 파일 생성을
     사용자에게 **제안**한다 (임의로 자동 생성하지 않고 사용자 확인을 받는다).
- 새 Workflow를 추가할 때는 이 `WORKFLOWS.md`의 표에도 행을 추가한다.
- 더 이상 쓰이지 않는 Workflow는 삭제하기보다 표에 "(사용 중단)"으로 표시하는 것을
  권장한다 (문서 삭제 금지 원칙과 동일한 정신).
