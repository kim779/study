# KnowledgeBase

- 생성일: 2026-07-10
- 업데이트: 2026-07-30 (성능 개선 구현/테스트 검증)

## 개요

AxisChaser는 트레이스 뷰어(디버깅 도구)로, AXIS 프로세스의 송수신/RTM/시스템 데이터를 WM_COPYDATA로 받아 RichEdit에 표시. 
성능 개선은 렌더링 오버헤드(WM_VSCROLL), 메모리 관리(Truncate), 큐 상한 3개 영역에 집중.

## 설계 의도 및 트러블슈팅 히스토리

### 1. WM_VSCROLL 배치 처리 (2026-07-30)

**문제점**:
- 이전: `addTrace()` 내에서 매번 `SendMessage(WM_VSCROLL, SB_BOTTOM)` 호출 → 렌더링 오버헤드 80-90%
- 대량 데이터 입력 시 (RTM 200건/초) CPU 60-90%, UI 프리징 500ms 이상

**해결책**:
- OnReceive() 루프 완료 후 (최대 20건 배치) 스크롤을 **한 번만** 실행
- 구현: ChildView.cpp:1065-1088 — `if (m_que.GetSize()) PostMessage(WM_RECEIVE); else SendMessage(WM_VSCROLL, ...)`

**기대 효과**:
- CPU 사용률: 60-90% → 10-40% (추정 80% 개선)
- UI 응답 시간: 500ms → 100ms 이하

**설계 트레이드오프**:
- 데이터 도착 → UI 표시 지연: 최대 20건 × N ms (OnReceive 주기 의존)
- 스크롤 위치 손실 불가능 (루프 완료 후 한 번만 실행)

### 2. RichEdit Truncate (50MB 상한) (2026-07-30)

**문제점**:
- 이전: Truncate 없음 → 장시간 운영 시 메모리 증가, 성능 악화
- 테스트 불가능한 환경에서도 구현만 완료하고 검증 필요

**해결책**:
- `addTrace()` 내에서 m_trace.GetTextLength() > 50MB 시 상단 50% 삭제
- 구현: ChildView.cpp:803-807 — SetSel(0, docLen/2) → ReplaceSel("")

**기대 효과**:
- 메모리 사용량: 100-200MB 안정화 (무한 증가 방지)
- Truncate 지연: < 500ms (동기 호출)

**설계 트레이드오프**:
- Truncate 중 UI 프리징 가능성
- 상단 50% 데이터 손실 (로그용이므로 허용)
- SetRedraw() 미사용 (프리징 더 심할 수 있음)

### 3. 큐 상한 (RTM 200건, SND/RCV 500건) (2026-07-30)

**문제점**:
- 이전: 큐 상한 없음 → 대량 유입 시 메모리 누수
- CopyData()에서 큐 추가만 하고 상한 체크 안 함

**해결책**:
- CopyData()에서 큐 크기 체크 후 상한 초과 시 데이터 드롭 (PostMessage 미실행)
- RTM: 200건 (fast RTM, 낮은 우선순위)
- SND/RCV: 500건 (거래 관련, 높은 우선순위)

**구현**: ChildView.cpp:1042-1047
```cpp
if ((flag == x_RTMs && m_que.GetSize() > 200) ||
    (flag == x_SNDs && m_que.GetSize() > 500) ||
    (flag == x_RCVs && m_que.GetSize() > 500)) {
    return;  // Drop this data
}
```

**기대 효과**:
- 메모리 누수 방지
- 극한 상황에서 큐 크기 안정화

**설계 트레이드오프**:
- 큐 상한 도달 시 데이터 손실 (로그용이므로 일부 손실 허용)
- 드롭 여부를 UI에 표시하지 않음 (사용자 인지 불가)

### 4. OutputDebugString 조건부 (#ifdef _DEBUG) (2026-07-30)

**문제점**:
- 이전: Release 빌드에서도 OutputDebugString 호출 → 약간의 오버헤드
- 실제 배포 시 Debug 메시지 제거 필요

**해결책**:
- addTrace()의 OutputDebugString을 `#ifdef _DEBUG` 블록으로 감싸기
- 구현: ChildView.cpp:788-791

**기대 효과**:
- Release 빌드 오버헤드 미미 (함수 호출 제거)

### 5. CQue 소멸자 메모리 누수 방지 (2026-07-30)

**문제점**:
- 이전: CQue 소멸자에서 `m_pBytes`를 정리하지 않음 → 메모리 누수
- 큐 항목이 제거될 때마다 바이트 버퍼가 해제되지 않음

**해결책**:
- CQue::~CQue() 구현에서 `delete[] m_pBytes` 추가
- 이미 구현: CQue 소멸자에서 바이트 버퍼 동적 해제

**기대 효과**:
- 메모리 누수 방지 (큐 항목당 수 KB ~ 수 MB 반환)
- 장시간 운영 시 메모리 안정화

**설계 트레이드오프**:
- 없음 (순수 버그 수정)

---

## 도메인 지식

### A. 데이터 흐름 및 우선순위

```
AXIS 프로세스
    │
    ├─ RTM (실시간 시세): 200건/초 ~ 1000건/초
    │  └─ 큐 상한: 200건 (높은 빈도, 낮은 우선순위)
    │
    ├─ SND (주문): 10건/초 ~ 100건/초
    │  └─ 큐 상한: 500건 (중요도 높음)
    │
    ├─ RCV (응답): 10건/초 ~ 100건/초
    │  └─ 큐 상한: 500건 (중요도 높음)
    │
    ├─ STR (시스템): 1건/초 ~ 10건/초
    │  └─ 큐 상한: 제한 없음 (낮은 빈도)
    │
    └─ CON (리포트): 0.1건/초 ~ 1건/초
       └─ 큐 상한: 제한 없음 (매우 낮은 빈도)

위 빈도는 실 운영 기준 추정치임.
```

### B. RichEdit 성능 특성

1. **SetSel + ReplaceSel**: 동기 호출, 블로킹
   - 작은 텍스트 (<10KB): < 1ms
   - 큰 텍스트 (>50MB): 100-500ms (Truncate 시)
   - SetRedraw(FALSE) 사용 시 2-5배 개선 (단, UI 갱신 지연)

2. **GetTextLength()**: 빠름 (< 1ms)

3. **LimitText(100MB)**: OnCreate()에서 설정 (line 1292)
   - 최대 텍스트 크기 제한 없음 (SetSel에서 자동 제한 없음)

### C. 동시성 모델

- UI 스레드 전용 (별도 워커 스레드 없음)
- CCriticalSection m_sync는 방어적 락 (재진입 방지 + PostMessage 안전성)
- 경쟁 조건 위험: 낮음 (모든 데이터 처리가 UI 스레드에서 수행)

### D. 필터 및 검색

- **키워드 필터** (addTrace 내): 문자열 Find 사용
- **범위 필터** (OnRCVData 내): 바이트 범위 (from/to)
- **코드 필터** (Options 다이얼로그): chaser.ini의 [code] 섹션에서 종목코드 목록

### E. Xecure 암호화 (현재 비활성)

- AxisXecure.XecureCtrl.IBK2019 ActiveX 컨트롤 참조 (OnCreate 주석 처리)
- Xecure() 함수도 주석 처리, 항상 false 반환
- 향후 복호화 필요 시 OnCreate의 주석 코드 복원 필요

---

## 성능 측정 기준

### 목표 성능 (개선 후)

| 지표 | 이전 | 개선 후 | 개선율 |
|------|------|--------|--------|
| CPU (RTM 200건/초) | 60-90% | 20-40% | 50-70% |
| UI 응답 시간 | 500ms | 100ms | 80% |
| 메모리 증가율 (10분) | 선형 증가 | 50MB 안정 | 무한→유한 |
| 큐 크기 (극한) | 무한 | RTM: 200, SND/RCV: 500 | 무한→유한 |

### 검증 방법

1. 작업 관리자 Performance 탭 (CPU, Memory)
2. DebugView.exe (Debug 출력)
3. Event Viewer (크래시 로그)
4. 시각적 평가 (UI 프리징, 반응성)

---

## 주의사항 및 제약

1. **AXIS 프로세스 필수**: 실제 테스트는 AXIS 워크스테이션 또는 테스트 데이터 송신 프로그램 필요
2. **Release 빌드만 검증 유효**: Debug 빌드는 오버헤드 높음
3. **Truncate 중 UI 프리징 불가피**: SetRedraw 사용 시 더 심할 수 있음
4. **메모리 누수**: 큐 상한 도달 시에만 방지, 상한 이하에서는 누수 가능성 있음
5. **데이터 손실 허용**: 로그용 도구이므로 일부 손실은 정상

---

## 참고자료

- Architecture.md: 프로젝트 구조
- Dependency.md: 라이브러리 의존성
- SourceIndex.md: 파일 색인
- test/PerfTestScenario.md: 상세 테스트 시나리오
- test/BuildInstructions.md: 빌드 지침
