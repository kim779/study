---
name: call-graph
description: 프로그램 진입점(main/WinMain/DllMain/AfxWinMain)부터 ThreadProc/Socket Callback/Message Handler를 거치는 주요 호출 흐름을 추적하여 CallGraph.md에 Mermaid 흐름도로 기록한다. "실행 흐름 추적", "이 함수가 어디서 호출되는지" 요청 시 사용.
---

# call-graph

## 목적

프로그램의 실행 흐름(진입점 → 주요 함수 호출 경로)을 추적하여 동작을 이해하기 쉽게 문서화한다.

## 입력

- 프로젝트 루트 경로
- 가능하면 `docs/SourceIndex.md`, `docs/Architecture.md` (선행 분석 결과 재사용)

## 수행 절차

1. 공통 사전 점검
   1. `CLAUDE.md` 읽기
   2. `docs/` 폴더 확인, `docs/CallGraph.md`가 있으면 먼저 읽는다.
   3. `docs/SourceIndex.md`를 읽을 수 있으면 먼저 읽어 진입점/주요 함수 위치를 빠르게 파악한다.
   4. 기존 내용을 최대한 재사용하고 삭제하지 않는다.
2. 진입점을 찾는다: `main`, `WinMain`, `DllMain`, `AfxWinMain`.
3. 진입점에서 시작하여 다음을 추적한다 (가능한 범위까지, 과도하게 깊지 않게 핵심 경로 위주로).
   - Thread 생성 지점과 `ThreadProc`(또는 람다/멤버 함수) 진입 흐름
   - Socket Callback (예: `WSAAsyncSelect`, `select`, IOCP 콜백, `boost::asio` 핸들러)
   - Message Handler (`WndProc`, MFC 메시지 맵 항목, COM 이벤트 핸들러)
   - 위 지점들에서 이어지는 주요 함수 호출 체인 (비즈니스 로직 핵심 경로 위주)
4. 각 흐름을 Mermaid `flowchart` 또는 `sequenceDiagram`으로 표현한다. 흐름이 너무 복잡하면
   상위 수준(모듈/서브시스템) 다이어그램과 세부 다이어그램으로 나눈다.
5. 기존 `CallGraph.md`와 비교하여 변경/신규 흐름만 반영하고, 기존 다이어그램은 유지한다.

## 출력

- `docs/CallGraph.md`

## 주의사항

- 모든 호출을 다 그리려 하지 말고 핵심 진입점과 주요 분기점 위주로 추적한다 (가독성 우선).
- 정적 분석의 한계상 함수 포인터/가상 호출/콜백 등록은 "추정 경로"로 표시한다.
- 기존 다이어그램/서술을 삭제하지 않는다.
