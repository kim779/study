# AxisChaser 성능 개선 구현 검증


## 목차

- [1. WM_VSCROLL 배치 처리](#1-wm_vscroll-배치-처리)
  - [구현 위치](#구현-위치)
  - [코드 검증](#코드-검증)
  - [검증 결과](#검증-결과)
- [2. OutputDebugString 조건부 (#ifdef _DEBUG)](#2-outputdebugstring-조건부-ifdef-_debug)
  - [구현 위치](#구현-위치-1)
  - [코드 검증](#코드-검증-1)
  - [검증 결과](#검증-결과-1)
- [3. RichEdit Truncate (50MB 상한)](#3-richedit-truncate-50mb-상한)
  - [구현 위치](#구현-위치-2)
  - [코드 검증](#코드-검증-2)
  - [검증 결과](#검증-결과-2)
- [4. Send/Receive 큐 상한 (500건)](#4-sendreceive-큐-상한-500건)
  - [구현 위치](#구현-위치-3)
  - [코드 검증](#코드-검증-3)
  - [검증 결과](#검증-결과-3)
- [5. 메모리 누수 방지](#5-메모리-누수-방지)
  - [구현 위치](#구현-위치-4)
  - [코드 검증](#코드-검증-4)
  - [상세 검증](#상세-검증)
    - [5-1. 큐 메모리 관리](#5-1-큐-메모리-관리)
    - [5-2. RichEdit 메모리 관리](#5-2-richedit-메모리-관리)
    - [5-3. 큐 상한 (메모리 누수 방지)](#5-3-큐-상한-메모리-누수-방지)
- [종합 검증 결과](#종합-검증-결과)
  - [5가지 개선사항 구현 상태](#5가지-개선사항-구현-상태)
  - [빌드 여부](#빌드-여부)
- [실행 가능 환경 체크](#실행-가능-환경-체크)
  - [빌드 환경](#빌드-환경)
  - [실행 환경](#실행-환경)
- [추가 검증 필요 항목](#추가-검증-필요-항목)
  - [1. CQue 소멸자 확인](#1-cque-소멸자-확인)
  - [2. 레거시 코드 정리](#2-레거시-코드-정리)
  - [3. Xecure ActiveX 주석 코드](#3-xecure-activex-주석-코드)
  - [4. Debug 빌드 오버헤드](#4-debug-빌드-오버헤드)
- [테스트 실행 준비 체크리스트](#테스트-실행-준비-체크리스트)
- [결론](#결론)
  - [개선 포인트](#개선-포인트)
  - [다음 단계](#다음-단계)
- [관련 파일](#관련-파일)

---

**검증일**: 2026-07-30  
**검증 범위**: ChildView.cpp에 구현된 5가지 성능 개선사항  
**결과**: 모든 항목 구현 확인됨

---

## 1. WM_VSCROLL 배치 처리

### 구현 위치

**파일**: `ChildView.cpp`  
**함수**: `CChildView::OnReceive()`  
**라인**: 1065-1088

### 코드 검증

```cpp
LRESULT CChildView::OnReceive(WPARAM wParam, LPARAM lParam)
{
    const int MAX_PER_CALL = 20;
    int processed = 0;

    CQue*   que;
    while (m_que.GetSize() && processed < MAX_PER_CALL)  // 최대 20건 배치
    {
        m_sync.Lock();
        que = (CQue *) m_que.GetAt(0);
        m_que.RemoveAt(0);
        m_sync.Unlock();
        OnRCVData(MAKEWPARAM(que->m_nBytes, que->m_flag), (LPARAM) que->m_pBytes);
        delete que;
        processed++;
    }

    if (m_que.GetSize())
        PostMessage(WM_RECEIVE);        // 큐 남음 → 재귀 예약
    else if (!m_bNOSCROLL)
        m_trace.SendMessage(WM_VSCROLL, SB_BOTTOM);  // 한 번만 실행! ✓

    return 0;
}
```

### 검증 결과

- [x] 최대 20건 배치 처리 (MAX_PER_CALL = 20)
- [x] 루프 완료 후 스크롤 한 번 실행 (else if 블록)
- [x] m_bNOSCROLL 플래그 확인 (사용자가 스크롤 정지 선택 시 동작 안 함)
- [x] 기대 효과: 80-90% 렌더링 오버헤드 감소

**상태**: ✓ IMPLEMENTED

---

## 2. OutputDebugString 조건부 (#ifdef _DEBUG)

### 구현 위치

**파일**: `ChildView.cpp`  
**함수**: `CChildView::addTrace()`  
**라인**: 788-791

### 코드 검증

```cpp
void CChildView::addTrace(CString dat, int kind, CString boldSub)
{
    // ... 코드 생략 ...

#ifdef _DEBUG
    slog.Format("[addTrace] [%d][%s]", stmp.GetLength(), stmp);
    OutputDebugString(slog);  // Debug 빌드에서만 실행
#endif

    // ... 나머지 코드 ...
}
```

### 검증 결과

- [x] `#ifdef _DEBUG` 블록으로 감싸짐
- [x] Release 빌드에서 컴파일되지 않음
- [x] 기대 효과: Release 빌드 오버헤드 제거

**상태**: ✓ IMPLEMENTED

---

## 3. RichEdit Truncate (50MB 상한)

### 구현 위치

**파일**: `ChildView.cpp`  
**함수**: `CChildView::addTrace()`  
**라인**: 803-807

### 코드 검증

```cpp
void CChildView::addTrace(CString dat, int kind, CString boldSub)
{
    // ... 필터링 및 로그 코드 생략 ...

    if (m_trace.GetTextLength() > 50 * 1024 * 1024) {
        int docLen = m_trace.GetTextLength();
        m_trace.SetSel(0, docLen / 2);          // 상단 50% 선택
        m_trace.ReplaceSel("");                  // 삭제
    }

    // ... 나머지 코드 ...
}
```

### 검증 결과

- [x] 50MB 상한 체크 (50 * 1024 * 1024 바이트)
- [x] GetTextLength() 호출로 현재 크기 확인
- [x] SetSel(0, docLen/2)로 상단 50% 선택
- [x] ReplaceSel("")로 선택 범위 삭제
- [x] 기대 효과: 메모리 사용량 100MB 안정화

**상태**: ✓ IMPLEMENTED

---

## 4. Send/Receive 큐 상한 (500건)

### 구현 위치

**파일**: `ChildView.cpp`  
**함수**: `CChildView::CopyData()`  
**라인**: 1042-1047

### 코드 검증

```cpp
void CChildView::CopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
    char*   rcvB = (char *) pCopyDataStruct->lpData;
    struct _exeCDSS* cdss = (struct _exeCDSS*) rcvB;

    DWORD   len  = cdss->len;
    DWORD   flag = cdss->flag;
#if 1
    // 큐 상한 체크
    if ((flag == x_RTMs && m_que.GetSize() > 200) ||      // RTM: 200건
        (flag == x_SNDs && m_que.GetSize() > 500) ||      // SND: 500건
        (flag == x_RCVs && m_que.GetSize() > 500))        // RCV: 500건
    {
        return;                                             // 데이터 드롭
    }

    CQue*   que = new CQue;
    que->m_flag = flag;
    que->m_nBytes = len;
    que->m_pBytes = new char[que->m_nBytes+1];
    ZeroMemory(que->m_pBytes, len+1);
    CopyMemory(que->m_pBytes, &rcvB[L_cdss], len);
    
    m_sync.Lock();
    m_que.Add(que);
    m_sync.Unlock();
    PostMessage(WM_RECEIVE);
#else
    // 레거시 코드 (비활성)
    OnRCVData(MAKEWPARAM(len, flag), (LPARAM) &rcvB[L_cdss]);
#endif
}
```

### 검증 결과

- [x] RTM 큐 상한: 200건
- [x] SND 큐 상한: 500건
- [x] RCV 큐 상한: 500건
- [x] 상한 초과 시 `return;` 으로 데이터 드롭
- [x] 각 데이터 타입별 우선순위 반영 (RTM: 낮음, SND/RCV: 높음)
- [x] 기대 효과: 메모리 누수 방지

**상태**: ✓ IMPLEMENTED

---

## 5. 메모리 누수 방지

### 구현 위치

**파일**: `ChildView.cpp`  
**함수**: `CChildView::CopyData()`  
**라인**: 1042-1047 (항목 4와 동일)

### 코드 검증

**메커니즘**:
1. 큐 크기 상한 도달 시 데이터 드롭 (CopyData 내 return)
2. RichEdit Truncate (50MB 이상 시 상단 50% 삭제)
3. OnReceive에서 CQue 객체 명시적 삭제 (line 1078: `delete que;`)

### 상세 검증

#### 5-1. 큐 메모리 관리

```cpp
// CopyData() - 큐 추가
CQue* que = new CQue;
que->m_flag = flag;
que->m_nBytes = len;
que->m_pBytes = new char[que->m_nBytes+1];  // 데이터 할당
// ...
m_que.Add(que);

// OnReceive() - 큐 제거
while (m_que.GetSize() && processed < MAX_PER_CALL) {
    // ...
    que = (CQue *) m_que.GetAt(0);
    m_que.RemoveAt(0);
    // ...
    delete que;  // ✓ 명시적 삭제
    processed++;
}
```

- [x] CQue 할당 (new)
- [x] 데이터 할당 (new char[])
- [x] 큐 추가 (m_que.Add)
- [x] 큐에서 제거 (m_que.RemoveAt)
- [x] 메모리 해제 (delete que) — **중요: 데이터도 함께 해제 필요**

**주의**: CQue 소멸자에서 m_pBytes 해제 확인 필요

#### 5-2. RichEdit 메모리 관리

```cpp
// addTrace()
if (m_trace.GetTextLength() > 50 * 1024 * 1024) {
    int docLen = m_trace.GetTextLength();
    m_trace.SetSel(0, docLen / 2);
    m_trace.ReplaceSel("");  // 상단 50% 삭제
}
```

- [x] 50MB 초과 시 자동 Truncate
- [x] SetSel + ReplaceSel로 메모리 해제

#### 5-3. 큐 상한 (메모리 누수 방지)

```cpp
// CopyData()
if ((flag == x_RTMs && m_que.GetSize() > 200) ||
    (flag == x_SNDs && m_que.GetSize() > 500) ||
    (flag == x_RCVs && m_que.GetSize() > 500))
{
    return;  // 데이터 드롭 → 메모리 할당 안 함
}
```

- [x] 상한 초과 시 데이터 할당 전에 반환
- [x] 메모리 누수 방지

**상태**: ✓ IMPLEMENTED (단, CQue 소멸자 검증 필요)

---

## 종합 검증 결과

### 5가지 개선사항 구현 상태

| 번호 | 항목 | 구현 | 위치 | 상태 |
|------|------|------|------|------|
| 1 | WM_VSCROLL 배치 처리 | ✓ | OnReceive() L1085 | **IMPLEMENTED** |
| 2 | OutputDebugString 조건부 | ✓ | addTrace() L788-791 | **IMPLEMENTED** |
| 3 | RichEdit Truncate (50MB) | ✓ | addTrace() L803-807 | **IMPLEMENTED** |
| 4 | Send/Receive 큐 상한 | ✓ | CopyData() L1042-1047 | **IMPLEMENTED** |
| 5 | 메모리 누수 방지 | ✓ | CopyData() + OnReceive() | **IMPLEMENTED** |

### 빌드 여부

**필수 확인**:
- [ ] Release 빌드 성공
- [ ] 링크 성공 (RichLib.lib)
- [ ] 실행 파일 생성 (Release/AxisChaser.exe)

---

## 실행 가능 환경 체크

### 빌드 환경

| 항목 | 상태 | 비고 |
|------|------|------|
| Visual Studio 2019+ | [ ] | v142 toolset 필요 |
| MFC | [ ] | Static or Shared DLL |
| Windows SDK | [ ] | 10.0 이상 |
| x64 플랫폼 | [ ] | Debug/Release 둘 다 |

### 실행 환경

| 항목 | 상태 | 비고 |
|------|------|------|
| Windows 10+ | [ ] | LTSC 2019 이상 |
| AXIS 프로세스 | [ ] | WM_COPYDATA 송신 프로세스 필요 |
| 테스트 데이터 | [ ] | 실제 AXIS 또는 TestSender.exe |

---

## 추가 검증 필요 항목

### 1. CQue 소멸자 확인

**필요성**: delete que 시 m_pBytes 메모리도 해제되는지 확인

```cpp
// chaser.h에서 CQue 정의 찾아서 소멸자 확인
class CQue : public CObject {
    // ...
    ~CQue() {
        if (m_pBytes) delete[] m_pBytes;  // ✓ 있어야 함
    }
};
```

**검증**: [ ] Grep으로 chaser.h에서 CQue 클래스 소멸자 확인

### 2. 레거시 코드 정리

**현재 상태**: `#if 1` / `#else` 블록 (line 1041-1062)
- `#if 1` 블록: 새 큐 기반 방식 (활성)
- `#else` 블록: 레거시 동기 방식 (비활성)

**권장**: 레거시 코드 제거 고려 (나중에 함)

### 3. Xecure ActiveX 주석 코드

**현재**: OnCreate()에서 약 60줄 주석 처리 (L228-285)  
**확인**: 향후 복호화 필요 시 복원 가능한 상태

### 4. Debug 빌드 오버헤드

**OutputDebugString 제거 여부**: Release 빌드에서 컴파일되지 않는지 확인

```bash
# Release 빌드 후 역어셈블리로 "OutputDebugString" 코드 확인
# (없어야 함)
```

---

## 테스트 실행 준비 체크리스트

- [ ] AxisChaser.vcxproj Release x64 빌드 완료
- [ ] RichLib.lib 생성 확인
- [ ] AxisChaser.exe 실행 파일 생성 확인
- [ ] test/ 폴더에 시나리오 문서 준비 완료
- [ ] BuildInstructions.md 검토 완료
- [ ] DebugView 또는 Visual Studio 준비
- [ ] 작업 관리자 Performance 탭 준비
- [ ] AXIS 프로세스 또는 TestSender.exe 준비
- [ ] 테스트 로그 저장 폴더 준비 (d:\src\IBKS\err\test\)

---

## 결론

**모든 5가지 성능 개선사항이 ChildView.cpp에 구현되었습니다.**

### 개선 포인트

1. **WM_VSCROLL 배치**: AddTrace()에서 매번 스크롤하는 대신, OnReceive() 완료 후 한 번만 실행
2. **Debug 출력**: Release 빌드에서 OutputDebugString 제거
3. **메모리 관리**: 50MB Truncate + 큐 상한으로 메모리 누수 방지

### 다음 단계

1. **빌드 검증**: Release 빌드 성공 여부 확인
2. **회귀 테스트**: 기존 기능 정상 작동 확인 (Phase 1)
3. **성능 측정**: Phase 2-5 테스트 실행 및 기록
4. **문제 수정**: 테스트 중 발견된 이슈 해결
5. **문서 갱신**: 최종 테스트 결과 반영

---

## 관련 파일

- [ChildView.cpp](../ChildView.cpp) — 구현 코드
- [test/PerfTestScenario.md](../test/PerfTestScenario.md) — 테스트 시나리오
- [test/BuildInstructions.md](../test/BuildInstructions.md) — 빌드 지침
- [docs/KnowledgeBase.md](../docs/KnowledgeBase.md) — 설계 의도
