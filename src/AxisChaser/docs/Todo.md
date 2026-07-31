# Todo

- 생성일: 2026-07-10
- 업데이트: 2026-07-30 (성능 개선 테스트 전략 수립)

## 개요

AxisChaser 성능 개선(WM_VSCROLL 배치 처리, RichEdit Truncate, 큐 상한) 후속 테스트 및 최적화 항목

## 현재 진행 중

### 테스트 (2026-07-30)

**상태**: Test 시나리오 작성 완료, 빌드/실행 대기

- [x] PerfTestScenario.md 작성 (5개 Phase, 총 12개 TC)
- [x] PerfTestHelper.cpp 작성 (성능 측정 헬퍼)
- [x] TestResultTemplate.md 작성
- [x] BuildInstructions.md 작성
- [ ] Phase 1 테스트 실행 (회귀 - 30분)
  - [ ] TC-1.1: 기본 데이터 수신/표시
  - [ ] TC-1.2: 옵션 다이얼로그
  - [ ] TC-1.3: 로그 파일 저장
- [ ] Phase 2 테스트 실행 (WM_VSCROLL - 20분)
  - [ ] TC-2.1: 중간 크기 배치 (50건/초)
  - [ ] TC-2.2: 대량 배치 (200건/초)
- [ ] Phase 3 테스트 실행 (Truncate - 30분)
  - [ ] TC-3.1: Truncate 동작 확인
  - [ ] TC-3.2: 스크롤 위치 손실
- [ ] Phase 4 테스트 실행 (큐 상한 - 20분)
  - [ ] TC-4.1: RTM 큐 상한 (200건)
  - [ ] TC-4.2: SND/RCV 큐 상한 (500건)
- [ ] Phase 5 테스트 실행 (Stress - 60분)
  - [ ] TC-5.1: 통합 Stress (10분 이상)

## 목차

### 테스트 관련

1. **성능 개선 검증** (2026-07-30~)
   - [test/PerfTestScenario.md](../test/PerfTestScenario.md) — 상세 테스트 시나리오
   - [test/BuildInstructions.md](../test/BuildInstructions.md) — 빌드 지침
   - [test/PerfTestHelper.cpp](../test/PerfTestHelper.cpp) — 성능 측정 코드

2. **미결 이슈**
   - Xecure ActiveX 컨트롤 코드 주석 처리 상태 확인 필요 (ChildView.cpp:228-285)
   - `#ifdef DF_MBCS` 분기 사실성 확인 (RichEditCtrlEx.h 항상 정의됨)
   - CColor 클래스 사용 여부 확인

### 추후 최적화

1. **구조 리팩터링** (우선순위: MEDIUM)
   - OnRCVData() 함수 분리 (330줄, 4가지 데이터 타입 처리)
     - Propose: OnRCVData_SendReceive(), OnRCVData_RTM(), OnRCVData_Systrace(), OnRCVData_Report()
   - 데이터 종류별 전용 클래스 도입 (CRTMHandler, CSDNRCVHandler 등)

2. **메모리 관리** (우선순위: MEDIUM)
   - CQue 자동해제 (smart pointer 도입)
   - 큐 상한 도달 시 로그 추가 (현재 조용히 드롭)

3. **동시성 강화** (우선순위: LOW)
   - CCriticalSection 범위 최소화 (현재 최소화됨)
   - Read-Write Lock 고려 (향후 워커 스레드 도입 시)

4. **UI 응답성** (우선순위: LOW)
   - SetRedraw(FALSE/TRUE) 도입 고려 (Truncate 중)
   - 비동기 Truncate 스레드 고려 (극도로 대량 입력 시)
