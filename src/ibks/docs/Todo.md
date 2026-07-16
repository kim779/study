# ibks 프로젝트 진행 과제 (Todo)

## 문서 목적

ibks 프로젝트의 현재 진행 상황, 미완료 항목, 향후 작업을 추적합니다.

---

## 1. VBScript → Python 엔진 전환 프로젝트 (2026-06-08 ~ )

### 진행 현황 요약

| 단계 | 작업 | 상태 | 완료일 | 담당 |
|------|------|------|--------|------|
| 1 | pythonEngine.h / engineWrapper.h 헤더 | ✓ 완료 | 2026-06-08 | Developer |
| 2 | pythonEngine.cpp 전체 구현 | ✓ 완료 | 2026-06-09 | Developer |
| 3 | engineWrapper.cpp - 자동 감지 방식 | ✓ 완료 | 2026-06-09 | Developer |
| 4 | axisvbs / axwizard 빌드 성공 | ✓ 완료 | 2026-06-09 | Developer |
| 5 | 사이드카 .py 파일로 Python 동작 확인 | ✓ 완료 | 2026-06-09 | Developer |
| 6 | 크래시 수정 (재진입/소멸자/AddRef) | ✓ 완료 | 2026-06-09 | Developer |
| 7 | 바이너리 마커 제거 (사이드카 없이 .map 직접 로드) | ✓ 완료 | 2026-06-10 | Developer |
| 8 | axisbuilder PY 버튼 / Python 하이라이팅 | ✓ 완료 | 2026-06-11 | Developer |
| 9 | mapload.cpp - 맵 소스에서 pythonMode 자동감지 | ✓ 완료 | 2026-06-11 | Developer |
| **10** | **axisbuilder → Python 스크립트 작성 → 빌드 → axwizard 실행 확인** | **⏳ 진행 중** | - | Developer |
| 11 | Screen.cpp TEST 블록(사이드카 .py 로드, ~400줄) 제거 | ⬜ 미착수 | - | Developer |
| 12 | VBScript → Python 일괄 변환 도구 | ⬜ 미착수 | - | Developer |

### 단계 10: 실운영 테스트 (현재 진행 중)

**작업:**
1. axisbuilder 실행
2. 기존 .map 파일 열기
3. 스크립트 편집창에서 [PY] 버튼 클릭
4. Python 스크립트 작성 (기존 VBS 대체)
5. 빌드 (ScpKind=PYTHON 기록)
6. axwizard에서 빌드된 .map 실행
7. Python 엔진 동작 확인

**예상 검증 항목:**
- [ ] Python 스크립트 로드 성공
- [ ] Screen/System/Login/Info COM 객체 접근
- [ ] 이벤트 핸들러(OnClick, OnTimer) 정상 동작
- [ ] 오류 발생 시 MessageBox 표시
- [ ] 메모리 누수 없음 (Process Monitor)

**현재 상태:**
```
예상 시작일: 2026-07-16 (다음 작업)
예상 완료일: 2026-07-17
담당자: Developer
```

---

### 단계 11: TEST 블록 제거

**작업:**
- Screen.cpp의 TEST 블록 제거 (~400줄)
  - 사이드카 .py 파일 로드 코드
  - 레지스트리 전역 설정 코드
  - 디버그용 로깅

**목표:**
- 프로덕션 코드 정리
- 배포 이미지 슬림화

**우선순위:** 높음

**예상 일정:** 단계 10 완료 후

---

### 단계 12: VBScript → Python 일괄 변환 도구

**작업:**
1. 5,391개 맵 소스 파일 분석
2. VBS 패턴 → Python 패턴 매핑
3. 자동 변환 스크립트 개발
4. 변환 결과 검증

**고려사항:**
- [ ] VBS 대소문자 무시 → Python 구분 처리
- [ ] Variant 타입 → Python 타입 변환
- [ ] 비정상 VBS 코드 처리 (문법 오류, 비표준)
- [ ] 변환 로그 (실패 파일, 주의 사항)

**우선순위:** 중간

**예상 소요 시간:** 1-2주

**참고:**
```python
# VBS 패턴 예시
If x > 10 Then
    SCREEN.SETDATA "field", "value"
    SYSTEM.MESSAGEBOX "msg"
End If

# Python 패턴 (예상)
if x > 10:
    Screen.SetData("field", "value")
    System.MessageBox("msg")
```

---

## 2. 실운영 배포 계획

### 배포 대상

| 대상 | 규모 | 예상 일정 |
|------|------|----------|
| 내부 테스트 | 1-2개 화면 | 2026-07-16 |
| 베타 테스트 | 50-100개 화면 | 2026-07-20 |
| 정식 배포 | 5,391개 화면 (전체) | 2026-08-31 |

### 배포 체크리스트

- [ ] axisvbs.dll Release 빌드 완료
- [ ] axwizard.dll Release 빌드 완료
- [ ] python311.dll, vcruntime140.dll 복사 (HTS exe 폴더)
- [ ] QA 테스트: 기존 VBS 화면 정상 작동 확인
- [ ] QA 테스트: Python 화면 정상 작동 확인
- [ ] 성능 테스트: 메모리 누수, CPU 부하
- [ ] 사용자 교육: Python 문법, 마이그레이션 가이드

---

## 3. 알려진 버그 및 미처리 항목

### 높은 우선순위

| ID | 내용 | 상태 | 담당 | 예상 일정 |
|----|----|------|------|----------|
| BUG-001 | Python 인터프리터 메모리 해제 (Py_FinalizeEx) | ✓ 완료 | Dev | 2026-06-09 |
| BUG-002 | GC 순환참조 (PyGC_Collect) | ✓ 완료 | Dev | 2026-06-09 |
| BUG-003 | 재진입(Re-entrance) 방어 | ✓ 완료 | Dev | 2026-06-09 |
| BUG-004 | 바이너리 마커(\x0E[\x0C) 제거 | ✓ 완료 | Dev | 2026-06-10 |

### 중간 우선순위

| ID | 내용 | 상태 | 담당 | 예상 일정 |
|----|----|------|------|----------|
| TASK-001 | Python 스크립트 캐싱 | ⬜ 미착수 | Dev | 2026-08 |
| TASK-002 | Worker 스레드 GIL 관리 | ⬜ 미착수 | Dev | 2026-08 |
| TASK-003 | Python 모듈 라이브러리 구축 | ⬜ 미착수 | Dev | 2026-09 |

### 낮은 우선순위

| ID | 내용 | 상태 | 담당 | 예상 일정 |
|----|----|------|------|----------|
| FEATURE-001 | Python 타입 힌트(Type Hints) | ⬜ 미착수 | Dev | TBD |
| FEATURE-002 | Python asyncio 지원 | ⬜ 미착수 | Dev | TBD |
| FEATURE-003 | NumPy/Pandas 통합 | ⬜ 미착수 | Dev | TBD |

---

## 4. 문서 작업

### 필수 문서 (완료)

- [x] Architecture.md - 모듈 구조 및 계층 (2026-07-15)
- [x] Dependency.md - 라이브러리 의존성 (2026-07-15)
- [x] SourceIndex.md - 소스 파일 색인 (2026-07-15)
- [x] CallGraph.md - 함수 호출 흐름도 (2026-07-15)
- [x] KnowledgeBase.md - 지식 베이스 (2026-07-15)
- [x] Build.md - 빌드 가이드 (2026-07-15)
- [x] python_engine_260608.md - Python 엔진 전환 기록 (2026-06-08 ~ 2026-06-11)

### 예정된 문서

- [ ] ProjectReport.md - 프로젝트 전체 요약 (2026-07-15)
- [ ] ArchitectureReview.md - 구조적 문제점 및 리팩터링 (2026-07-20)
- [ ] PerformanceProfile.md - 성능 분석 (2026-08)
- [ ] TestingStrategy.md - 테스트 전략 (2026-08)
- [ ] ReleaseNotes.md - 릴리즈 노트 (2026-08)

---

## 5. 테스트 계획

### 단위 테스트 (Unit Tests)

- [ ] CPythonEngine::LoadScript 테스트
- [ ] CPythonEngine::DoProcedure 테스트
- [ ] CEngineWrapper::isPythonScript 테스트
- [ ] AxisObject 래핑 테스트
- [ ] 바이너리 마커 제거 테스트

**도구:** Google Test, C++ Unit Testing Framework

### 통합 테스트 (Integration Tests)

- [ ] axisvbs.dll + axwizard.dll 호환성
- [ ] Python 스크립트 + COM 객체 상호작용
- [ ] VBS 스크립트 + Python 스크립트 혼용

**도구:** Manual Testing + QA 검증

### 성능 테스트 (Performance Tests)

- [ ] LoadScript 시간 측정 (컴파일)
- [ ] DoProcedure 호출 시간 측정
- [ ] 메모리 사용량 프로파일링
- [ ] GC 순환참조 정리 확인

**도구:** Process Monitor, Profiler

### 회귀 테스트 (Regression Tests)

- [ ] 기존 VBS 화면 100% 호환
- [ ] 레이아웃 변경 없음
- [ ] 성능 저하 없음 (<10%)

---

## 6. 리스크 분석

### 높은 리스크

| 리스크 | 영향 | 확률 | 완화 방안 |
|--------|------|------|----------|
| Python 메모리 누수 | 프로세스 크래시 | 중간 | PyGC_Collect, 정기 모니터링 |
| GIL 경합(Contention) | 성능 저하 | 중간 | 단일 스레드 권장, 문서화 |
| VBS → Python 변환 오류 | 기능 손실 | 높음 | 자동 검증 도구, QA 테스트 |

### 중간 리스크

| 리스크 | 영향 | 확률 | 완화 방안 |
|--------|------|------|----------|
| 써드파티 라이브러리 호환 | 기능 제한 | 낮음 | pip로 버전 관리 |
| Python 버전 업그레이드 | 호환성 문제 | 낮음 | 버전 고정(3.11.6), 정기 검토 |

---

## 7. 마일스톤

| 마일스톤 | 목표 | 예상 날짜 | 상태 |
|---------|------|----------|------|
| **M1** | Python 엔진 기본 구현 | 2026-06-11 | ✓ 완료 |
| **M2** | 실운영 테스트 완료 | 2026-07-17 | ⏳ 진행 중 |
| **M3** | 베타 배포 (50-100 화면) | 2026-07-20 | ⬜ 예정 |
| **M4** | 일괄 변환 도구 완성 | 2026-08-15 | ⬜ 예정 |
| **M5** | 정식 배포 (5,391 화면) | 2026-08-31 | ⬜ 예정 |

---

## 8. 팀 역할 및 책임

| 역할 | 담당자 | 책임 |
|------|--------|------|
| **Developer** | - | Python 엔진 구현, 단계 10-12 |
| **QA** | - | 통합 테스트, 회귀 테스트, 성능 테스트 |
| **DevOps** | - | 빌드 배포, 배포 인프라 |
| **Documentation** | - | 문서 관리, KnowledgeBase 유지 |

---

## 9. 커뮤니케이션

### 정기 회의

- **주 1회 진행 상황 회의** (월 14:00)
- **이슈 발생 시 긴급 회의**

### 보고 항목

- 주간 진행률 (%)
- 발견된 버그 및 해결 현황
- 리스크 및 대응 방안
- 다음 주 일정

---

## 10. 변경 이력

| 날짜 | 변경 내용 | 작성자 |
|------|----------|--------|
| 2026-07-15 | 초안 작성 | Documentation Agent |
| - | - | - |

---

## 관련 문서

- `@docs/Architecture.md` - 모듈 구조
- `@docs/python_engine_260608.md` - 상세 기록
- `@docs/KnowledgeBase.md` - 지식 베이스
