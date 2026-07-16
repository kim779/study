---
name: architecture-review
description: 구조적 문제(SRP 위반, God Class, 순환 Include/의존성, 강한 결합, 낮은 응집도, Lock 경쟁, UI Thread Block, 메모리 누수 가능성, RAII/Smart Pointer 준수 여부)를 분석하여 ArchitectureReview.md에 리팩터링 후보와 함께 기록한다. "구조 리뷰", "리팩터링 대상 찾아줘" 요청 시 사용.
---

# architecture-review

## 목적

프로젝트의 구조적 품질 문제를 식별하고, 근거와 함께 리팩터링 후보를 제시한다.

## 입력

- 프로젝트 루트 경로
- 가능하면 `docs/Architecture.md`, `docs/Dependency.md`, `docs/SourceIndex.md` (선행 분석 결과 재사용)

## 수행 절차

1. 공통 사전 점검
   1. `CLAUDE.md` 읽기
   2. `docs/` 폴더 확인, `docs/ArchitectureReview.md`가 있으면 먼저 읽는다.
   3. `docs/Architecture.md`, `docs/Dependency.md`, `docs/SourceIndex.md`를 읽을 수 있으면
      먼저 읽어 재분석 비용을 줄인다 (없으면 analyze-project / dependency-map 선행 실행을 권장).
   4. 기존 `ArchitectureReview.md` 내용을 최대한 재사용하고 삭제하지 않는다.
2. 다음 항목을 근거(파일/라인/클래스명)와 함께 분석한다.
   - SRP 위반 후보, God Class (책임/멤버/함수 수가 과도한 클래스)
   - 순환 Include, 순환 의존성 (dependency-map 결과 재사용 가능하면 활용)
   - 강한 결합 / 낮은 응집도 (모듈 간 상호 참조 빈도로 추정)
   - Lock 경쟁 가능성 (동일 Mutex/CriticalSection에 대한 광범위한 대기 지점)
   - UI Thread Block 가능성 (메시지 핸들러/이벤트 콜백 내 동기 I/O, Sleep, 장시간 루프)
   - 메모리 누수 가능성 (`new`/`malloc` 대비 대응하는 해제 누락, 예외 경로에서 해제 누락)
   - RAII 준수 여부, Smart Pointer(`unique_ptr`/`shared_ptr`/`CComPtr` 등) 사용 비율 vs raw pointer
3. 각 발견 항목을 심각도(높음/중간/낮음)와 함께 기록하고, 확실하지 않은 추정은 "추정"임을 명시한다.
4. 리팩터링 후보 목록을 우선순위와 함께 정리한다.
5. 기존 `ArchitectureReview.md`와 비교하여 변경/신규 발견 사항만 반영하고, 해결된 항목은
   삭제하지 말고 "해결됨"으로 표시한다.

## 출력

- `docs/ArchitectureReview.md`

## 주의사항

- 확인되지 않은 추정은 반드시 "추정"으로 표기한다 (정적 분석만으로 런타임 동작을 단정하지 않는다).
- 심각도와 근거(파일 경로 등)를 반드시 함께 제시한다.
- 기존 문서를 삭제하지 않고, 해결된 항목은 상태만 갱신한다.
