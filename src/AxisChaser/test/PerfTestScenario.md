# AxisChaser 성능 개선 테스트 시나리오

**생성일**: 2026-07-30  
**목적**: WM_VSCROLL 배치 처리, RichEdit Truncate, 큐 상한 등 5가지 성능 개선사항의 검증

---

## 개선사항 요약

| 번호 | 항목 | 구현 위치 | 기대 효과 |
|------|------|---------|---------|
| 1 | **WM_VSCROLL 배치 처리** | `OnReceive()` (line 1065-1088) | 렌더링 오버헤드 80-90% 감소 |
| 2 | **OutputDebugString 조건부** | `addTrace()` (line 788-791) | Release 빌드에서 디버그 메시지 제거 |
| 3 | **RichEdit Truncate (50MB)** | `addTrace()` (line 803-807) | 시간 경과에 따른 성능 악화 방지 |
| 4 | **Send/Receive 큐 상한 (500건)** | `CopyData()` (line 1042-1047) | 메모리 누수 방지 |
| 5 | **메모리 누수 방지** | `CopyData()` 큐 상한 도달 시 | 데이터 드롭으로 메모리 증가 제한 |

---

## 테스트 계획

### Phase 1: 회귀 테스트 (기존 기능 정상 작동)

**목표**: 개선사항 구현이 기존 기능을 깨지 않았는지 확인

#### TC-1.1: 기본 데이터 수신 및 표시

**조건**:
- AxisChaser 실행
- AXIS 프로세스에서 WM_COPYDATA로 수신 데이터 전송 (RTM 100건, SND 50건, RCV 50건 혼합)

**기대 결과**:
- UI에 텍스트/색상/폰트 정상 표시
- 필터/검색 기능 정상 작동
- 스크롤 위치 맨 아래로 자동 이동

**검증 방법**:
- 화면 시각적 확인
- 필터 입력 후 해당 데이터만 표시되는지 확인
- 스크롤 위치 확인

#### TC-1.2: 옵션 다이얼로그

**조건**:
- 옵션 메뉴에서 카테고리별 필터 설정 (SND/RCV/RTM/SYS/REPORT)
- 폰트/색상 설정 변경

**기대 결과**:
- 설정 항목 저장됨
- 재시작 후 설정 유지

**검증 방법**:
- 설정값 변경 후 chaser.ini 파일 확인
- AxisChaser 재시작 후 설정 유지 확인

#### TC-1.3: 로그 파일 저장

**조건**:
- "Log SND/RCV" 체크
- 데이터 수신

**기대 결과**:
- Send_Rev.ini 파일에 데이터 기록됨

**검증 방법**:
- 로그 파일 열기 버튼 클릭 후 파일 내용 확인

---

### Phase 2: WM_VSCROLL 배치 처리 검증

**목표**: OnReceive()가 한 루프에서 최대 20건을 처리한 후 스크롤을 한 번만 실행하는지 확인

#### TC-2.1: 중간 크기 배치 (RTM 50건/초)

**조건**:
- RTM 데이터 50건/초 입력 (OnReceive 루프 3회 정도)
- 기간: 5초

**기대 결과**:
- 모든 데이터 표시됨
- UI 반응 시간: < 100ms (개선 전 추정 500ms → 개선 후 100ms)
- CPU 사용률: 10-30%

**검증 방법**:
- 작업 관리자 Performance 탭에서 CPU/메모리 모니터링
- 화면 프레임 율 체감적 평가 (끊김 없음)

#### TC-2.2: 대량 배치 (RTM 200건/초)

**조건**:
- RTM 데이터 200건/초 입력 (OnReceive 루프 10회 정도)
- 기간: 10초

**기대 결과**:
- 모든 데이터 표시됨
- UI 반응 시간: < 100ms
- CPU 사용률: 20-40% (개선 전 추정 80-90% → 개선 후)
- 메모리 안정적 (Truncate로 100MB 유지)

**검증 방법**:
- 작업 관리자에서 CPU/메모리 추이 기록
- Truncate 발생 여부 확인 (Debug 출력 또는 데이터 크기 체크)

**측정 도구**:
```cpp
// ChildView.cpp addTrace()에 임시 성능 로그 추가 (테스트 후 제거)
#ifdef _DEBUG
static LARGE_INTEGER liLast = {0};
LARGE_INTEGER liNow;
QueryPerformanceCounter(&liNow);
if (liLast.QuadPart > 0) {
    long long delta = (liNow.QuadPart - liLast.QuadPart) / 10000; // ms
    if (delta > 50) {
        CString slog;
        slog.Format("[PERF] addTrace delay: %lld ms\n", delta);
        OutputDebugString(slog);
    }
}
liLast = liNow;
#endif
```

---

### Phase 3: RichEdit Truncate 검증 (50MB 상한)

**목표**: RichEdit 텍스트 크기가 50MB를 초과하면 상단 50% 삭제 후 성능 유지

#### TC-3.1: Truncate 동작 확인

**조건**:
- 대량 데이터를 계속 입력해 RichEdit 텍스트 크기 50MB 도달
- 기간: 5분 이상

**기대 결과**:
- 50MB 도달 후 상단 50% 자동 삭제
- Truncate 중 UI 프리징 < 500ms
- 메모리 사용량 200MB 이상 증가하지 않음

**검증 방법**:
- `m_trace.GetTextLength()` 값 기록 (Debug 출력)
- Truncate 실행 전후 시간 측정
- 작업 관리자에서 메모리 사용량 추이 기록

**검증 코드** (addTrace() 내에 임시 추가):
```cpp
// Before truncate
int beforeLen = m_trace.GetTextLength();
LARGE_INTEGER liStart;
QueryPerformanceCounter(&liStart);

if (m_trace.GetTextLength() > 50 * 1024 * 1024) {
    int docLen = m_trace.GetTextLength();
    m_trace.SetSel(0, docLen / 2);
    m_trace.ReplaceSel("");
}

LARGE_INTEGER liEnd;
QueryPerformanceCounter(&liEnd);
long long deltaMs = (liEnd.QuadPart - liStart.QuadPart) / 10000;
int afterLen = m_trace.GetTextLength();

CString slog;
slog.Format("[TRUNCATE] Before: %d -> After: %d, Time: %lld ms\n", 
    beforeLen, afterLen, deltaMs);
OutputDebugString(slog);
```

#### TC-3.2: 스크롤 위치 손실 검증

**조건**:
- Truncate 중 사용자가 스크롤 시도
- 데이터 계속 입력

**기대 결과**:
- 스크롤 위치 변경되나, UI 크래시 없음
- 데이터 손실 없음 (Truncate는 상단 50% 삭제)

**검증 방법**:
- 화면 시각적 확인 (크래시/프리징 없음)
- 디버그 로그에서 예외 확인

---

### Phase 4: 큐 상한 검증 (RTM 200건, SND/RCV 500건)

**목표**: 큐 크기 초과 시 데이터 드롭 확인 및 메모리 누수 없음

#### TC-4.1: RTM 큐 상한 (200건)

**조건**:
- RTM 데이터 500건/초 입력 (OnReceive 처리 속도보다 빠름)
- 기간: 5초

**기대 결과**:
- m_que 크기 200건 이상 증가 안 함
- 일부 데이터 드롭됨 (예상)
- 메모리 누수 없음

**검증 방법**:
- CopyData()에 로그 추가:
```cpp
if ((flag == x_RTMs && m_que.GetSize() > 200)) {
    CString slog;
    slog.Format("[QUEUE] RTM DROP: queSize=%d\n", m_que.GetSize());
    OutputDebugString(slog);
    return;  // Drop this data
}
```
- 작업 관리자에서 메모리 안정성 확인

#### TC-4.2: SND/RCV 큐 상한 (500건)

**조건**:
- SND 데이터 300건/초 입력
- 기간: 5초

**기대 결과**:
- m_que 크기 500건 이상 증가 안 함
- 메모리 누수 없음

**검증 방법**:
- CopyData()에 로그 추가 (위와 동일)
- 작업 관리자 메모리 추이 기록

---

### Phase 5: Stress Test (극한 시나리오)

**목표**: 모든 데이터 타입을 동시에 대량 입력할 때 크래시/프리징 없음 확인

#### TC-5.1: 통합 Stress Test

**조건**:
- RTM 200건/초
- SND 200건/초
- RCV 200건/초
- STR 50건/초
- CON 10건/초
- 기간: 10분 이상

**기대 결과**:
- 크래시 없음
- 메모리 증가량 < 200MB (Truncate로 100MB 유지)
- UI 응답성 유지 (프리징 < 100ms)
- 데이터 손실: 예상 (큐 상한으로 인한 드롭)

**검증 방법**:
- 작업 관리자에서 CPU/메모리/디스크 모니터링
- Event Viewer에서 크래시 로그 확인
- AxisChaser 자체 Debug 출력 확인

**모니터링 코드** (sendTestData 함수 추가):

```cpp
// 테스트 프로세스에서 AxisChaser로 데이터 송신
void SendTestData(HWND hChaserWnd, DWORD dataType, int count) {
    struct _exeCDSS {
        DWORD flag;
        DWORD len;
    };
    
    for (int i = 0; i < count; i++) {
        char buffer[1024];
        _exeCDSS* cdss = (_exeCDSS*)buffer;
        cdss->flag = dataType;
        
        // Dummy data
        char* data = buffer + sizeof(_exeCDSS);
        sprintf_s(data, 1000, "TEST_DATA_%d_%d\n", dataType, i);
        cdss->len = strlen(data) + 1;
        
        COPYDATASTRUCT cds;
        cds.dwData = 0;
        cds.cbData = sizeof(_exeCDSS) + cdss->len;
        cds.lpData = buffer;
        
        SendMessage(hChaserWnd, WM_COPYDATA, 0, (LPARAM)&cds);
    }
}
```

---

## 회귀 위험 항목 (Risk Assessment)

| 위험 항목 | 심각도 | 근거 | 검증 방법 |
|---------|------|------|---------|
| **Truncate 중 UI 프리징** | HIGH | SetSel/ReplaceSel 동기 호출 | TC-3.1: 500ms 이하 |
| **스크롤 위치 손실** | MEDIUM | Truncate 중 m_bNOSCROLL 체크 무시 가능성 | TC-3.2: 시각적 확인 |
| **큐 드롭 데이터 손실** | LOW | 로그용 도구이므로 일부 손실 허용 | TC-4.1, 4.2: 드롭 로그 확인 |
| **메모리 누수** | HIGH | 큐 상한 미설정 시 메모리 누수 | TC-4.1, 4.2: 메모리 추이 |
| **멀티 스레드 경합** | MEDIUM | m_sync CCriticalSection 안전성 | TC-5.1: Stress test |

---

## 테스트 실행 순서

1. **Phase 1 (회귀)**: 30분
   - TC-1.1, 1.2, 1.3
2. **Phase 2 (WM_VSCROLL)**: 20분
   - TC-2.1, 2.2
3. **Phase 3 (Truncate)**: 30분
   - TC-3.1, 3.2
4. **Phase 4 (큐 상한)**: 20분
   - TC-4.1, 4.2
5. **Phase 5 (Stress)**: 60분
   - TC-5.1

**총 예상 시간**: 3시간

---

## 검증 기준 (Pass/Fail)

### Pass 조건

1. 모든 TC-1.x 통과 (회귀)
2. TC-2.1, 2.2: CPU 개선 전 대비 50% 이상 감소
3. TC-3.1: Truncate 지연 < 500ms
4. TC-4.1, 4.2: 큐 크기 상한 유지, 메모리 누수 없음
5. TC-5.1: 10분 연속 실행 후 크래시/프리징 없음

### Fail 조건

1. TC-1.x 중 하나 실패 (기능 손상)
2. TC-2.1, 2.2: CPU 개선 < 30%
3. TC-3.1: Truncate 지연 > 500ms 또는 크래시
4. TC-4.1, 4.2: 메모리 누수 또는 크래시
5. TC-5.1: 10분 이전에 크래시

---

## 검증 체크리스트

### 사전 준비

- [ ] AxisChaser.vcxproj Release 빌드 완료
- [ ] RichLib 서브프로젝트 빌드 완료 (RichLib.lib 생성)
- [ ] 작업 관리자 Performance 탭 준비
- [ ] Debug 출력 모니터링 (Visual Studio 또는 DebugView)
- [ ] 테스트용 AXIS 프로세스 또는 데이터 송신 프로그램 준비

### 테스트 실행

- [ ] Phase 1 완료 (회귀 테스트)
- [ ] Phase 2 완료 (WM_VSCROLL)
  - [ ] RTM 50건/초 성능 기록
  - [ ] RTM 200건/초 성능 기록
- [ ] Phase 3 완료 (Truncate)
  - [ ] 50MB 도달 시간 기록
  - [ ] Truncate 지연 시간 기록
  - [ ] 메모리 사용량 최댓값 기록
- [ ] Phase 4 완료 (큐 상한)
  - [ ] RTM 큐 드롭 로그 확인
  - [ ] SND/RCV 큐 드롭 로그 확인
- [ ] Phase 5 완료 (Stress)
  - [ ] 10분 연속 실행 완료
  - [ ] 최종 메모리 사용량 기록
  - [ ] 크래시 로그 없음 확인

### 사후 조치

- [ ] Debug 로그 제거 (임시 추가한 성능 측정 코드)
- [ ] 테스트 결과 문서화 (TestResult.md)
- [ ] 리스크 항목 재평가

---

## 주의사항

1. **AXIS 프로세스 필수**: AxisChaser는 WM_COPYDATA 수신 기반이므로, 실제 AXIS 프로세스 또는 테스트 데이터 송신 프로그램 필요
2. **Release 빌드**: OutputDebugString 조건부 확인을 위해 Release 빌드에서만 테스트
3. **심볼 경로**: Debug 심볼은 D:\symbol 또는 D:\symstore에 저장 (크래시 분석용)
4. **로그 보존**: 테스트 중 Debug 출력은 DebugView.exe로 캡처하여 보존

---

## 참고자료

- [Architecture.md](../docs/Architecture.md): 프로젝트 구조
- [KnowledgeBase.md](../docs/KnowledgeBase.md): 설계 의도
- [ChildView.cpp](../ChildView.cpp): 구현 코드
- [CLAUDE.md](../CLAUDE.md): 프로젝트 가이드
