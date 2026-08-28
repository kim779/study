# C_Total: Data Flow Architecture

## Overview

C_Total (차트 종합 분석 DLL)는 다양한 데이터 소스로부터 실시간 시세 데이터를 수신하고, 환경 정보와 함께 저장한 후, 내장 차트 컨트롤(m_pwndChart)을 통해 렌더링하는 아키텍처를 갖습니다.

주요 클래스: `CMainWnd`, `CExtraMsg`, `CC_TotalApp`

---

## 1. Data Reception (데이터 수신)

### 1.1 Message-Based Data Channels

#### **Channel: GEV_SISE (Realtime Market Data)**

**진입점**: `CMainWnd::OnSiseEvent(WPARAM wParam, LPARAM lParam)`

```cpp
// MainWnd.h (Line 197)
afx_msg LRESULT OnSiseEvent(WPARAM wParam, LPARAM lParam);

// MainWnd.cpp (Line 948)
LRESULT CMainWnd::OnSiseEvent(WPARAM wParam, LPARAM lParam)
{
    return m_pExtraMsg->SiseEvent(wParam, lParam);  // Delegate to CExtraMsg
}
```

**메시지 구조**:
- `LOWORD(wParam)`: 시세 이벤트 타입 (siseChgPalette, SISE_TR_DATA, siseRTMData, siseData, siseFlash, siseFont)
- `HIWORD(wParam)`: 세부 커맨드 (siseCurr, siseCode, siseDate, sisePivot, siseStock, siseDollar)
- `lParam`: 데이터 포인터 또는 구조체

**주요 데이터 타입**:
- `SISE_TR_DATA`: 거래소 데이터 (struct _trData*)
  ```cpp
  struct _trData {
      char *pcData[0];        // Raw data buffer
      int iLen[0];            // Data length
      int iSiseLen[0];        // Output length (set by ReceiveData)
  };
  ```
- `siseRTMData`: 실시간 마켓 데이터 (raw char* buffer)

**처리 방식**: `CExtraMsg::SiseEvent()` 호출
- `SISE_TR_DATA`: `ReceiveData()` 호출하여 데이터 파싱
- `siseRTMData`: `realtime()` 호출하여 실시간 갱신

---

#### **Channel: GEV_INPUT (User Input & Control)**

**진입점**: `CMainWnd::OnInputEvent(WPARAM wParam, LPARAM lParam)`

```cpp
// MainWnd.h (Line 196)
afx_msg LRESULT OnInputEvent(WPARAM wParam, LPARAM lParam);

// MainWnd.cpp (Line 938)
LRESULT CMainWnd::OnInputEvent(WPARAM wParam, LPARAM lParam)
{
    return m_pExtraMsg->InputEvent(wParam, lParam);
}
```

**메시지 구조**:
- `LOWORD(wParam)`: 입력 이벤트 타입 (INPUT_GET_POS_COMMENT, INPUT_CHANGE, INPUT_DATA)
- `HIWORD(wParam)`: 세부 커맨드 (GET_TR_INFO, getTRInfo2, GET_TOTAL_DAY, getDisplayDay, etc.)
- `lParam`: 헤더 포인터 또는 커맨드 데이터

**외부 호출처**:
- `PnBaseWnd` / `PnWndCombo`: 패널 컨트롤에서 UI 변경 이벤트 → `SendMessage(GEV_INPUT, ...)`
- 외부 ActiveX 클라이언트: `RequestTR()` / `RequestTR2()` 공개 메서드 호출

---

#### **Channel: GEV_CHART (Chart Control Events)**

**진입점**: `CMainWnd::OnChartEvent(WPARAM wParam, LPARAM lParam)`

**주요 용도**:
- 차트 윈도우(`m_pwndChart`)로부터 환경 정보 조회
- 예: `REQUEST_CUR_ENV` → 현재 차트 환경 정보(_envInfo*) 반환

```cpp
// ExtraMsg.cpp (Line 50)
if (m_pMainWnd->m_pwndChart)
    envinfo = (_envInfo*)m_pMainWnd->m_pwndChart->SendMessage(
        GEV_CHART, 
        MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 
        0
    );
```

---

### 1.2 Timer-Based Refresh

**진입점**: `CMainWnd::OnTimer(UINT nIDEvent)`

```cpp
// MainWnd.h (Line 171)
afx_msg void OnTimer(UINT nIDEvent);

// MainWnd.cpp (Line 1110)
void CMainWnd::OnTimer(UINT nIDEvent) 
{
    // Timer-driven updates (implementation details context-dependent)
    CWnd::OnTimer(nIDEvent);
}
```

**용도**: 주기적 데이터 갱신 또는 UI 리프레시

---

### 1.3 External API (TR Request)

**공개 메서드**: 
- `BOOL RequestTR(LPCTSTR str)` - TR 요청
- `BOOL RequestTR2(LPCTSTR str)` - 확장 TR 요청

**내부 흐름** (2026-08-27 코드 확인 후 정정 — 아래가 실제 경로):
1. `SendRequest()`/`SendRequest2()`/`SendRequestS()`가 `_userTH`(trc="GOOPHOOP", stat=US_OOP)
   헤더 + 입력/차트 헤더를 조립해 `m_pwndView`(부모/호스트 윈도우)에 `WM_USER(invokeTRx, ...)`로
   전송 위임 (C_Total 자신은 소켓을 갖지 않음)
2. 응답은 호스트가 `WM_USER(DLL_OUBx, _extTHx*)`로 되돌려주고, `CMainWnd::OnMessage()` →
   `Oub()`가 처리 — `_zcom_head`로 분할 전송 재조립 후 `m_TrData`(`_trData`)에 저장
3. 이후 `m_pwndChart`(GEV_CHART/CHART_TR_DATA)와 자기 자신(GEV_SISE/SISE_TR_DATA)에 전달

**참고**: `OnSiseEvent()`/`ReceiveData()`(`CExtraMsg`)는 이 TR 요청/응답 경로가 아니라 별개의
실시간 시세 채널(1.1절 GEV_SISE)입니다 — TR 요청/응답 상세 구조체(`_userTH`/`_extTHx`/
`_zcom_head`/`_trData`)와 전체 시퀀스는 [C_TotalTRProtocol.md](C_TotalTRProtocol.md) 참고.

---

## 2. Data Storage (데이터 저장)

### 2.1 Primary Storage Structure

**클래스**: `CExtraMsg` (파일: ExtraMsg.h/cpp)

**멤버 변수**:
```cpp
class CExtraMsg  
{
    CString m_code;     // 현재시간 코드
    CString m_date;     // 기준일자
    CString m_sahg;     // 상한가
    CString m_hahg;     // 하한가
    CString m_jjga;     // 정중가
    CString m_stock;    // 누적거래량
    CString m_start;    // 장시작시간
    
    CString m_curr;     // 현재가
    CString m_udyl;     // 등락
    
    class CMainWnd *m_pMainWnd;  // 부모 참조
    CC_TotalApp *m_pApp;         // 애플리케이션 인스턴스
};
```

**저장 생명주기**:
1. **초기화**: 생성자에서 CString 객체 초기화 (자동)
2. **갱신**: `ReceiveData()` / `realtime()` 호출 시 파싱해서 할당
3. **정리**: 소멸자에서 자동 정리

---

### 2.2 Chart-Level Storage

**클래스**: `CMainWnd` (파일: MainWnd.h/cpp)

**멤버 변수** (Line 83-86):
```cpp
CArray<char*, char*>            m_arData;      // Raw data buffers
CArray<int, int>                m_arLen;       // Buffer lengths
struct _trData                  m_TrData;      // TR data (double-buffer)
_envInfo*                       m_pEnvInfo;    // Environment info
```

**_envInfo 구조체** (외부 정의):
```cpp
struct _envInfo {
    struct {
        struct {
            int dwTotCnt;   // Total day count
            int dwDisCnt;   // Display count
        } aaDayInfo[4][15]; // [unit][index]
        char cToday;        // Today flag
    } datainfo;
};
```

**데이터 흐름**:
1. `ReceiveData(char* pcData, int iLen)`: 
   - 수신 버퍼에서 토큰 파싱 (탭 구분)
   - `m_code`, `m_date`, `m_sahg`, `m_hahg`, `m_jjga`, `m_stock` 저장
   - 반환값: 파싱된 데이터 길이

2. `realtime(char* pData)`: 
   - 실시간 데이터 처리 (m_curr, m_udyl 갱신)

---

### 2.3 File-Backed Persistence

**저장소 경로**:
```cpp
CString m_strEnvPath;   // 환경 정보 파일 경로
CString m_strGrpPath;   // 그룹 정보 파일 경로
CString m_strDatPath;   // 차트 데이터 파일 경로
```

**I/O 메서드**:
- `ReadEnv()`: 환경 정보 로드
- `ReadGrp()`: 그룹 설정 로드
- `ReadData()`: 차트 데이터 로드
- `WriteEnv()`: 환경 정보 저장
- `WriteGrp()`: 그룹 설정 저장
- `WriteDat()`: 차트 데이터 저장

---

## 3. Rendering (렌더링)

### 3.1 Paint Entry Point

**진입점**: `CMainWnd::OnPaint()`

```cpp
// MainWnd.h (Line 206)
afx_msg void OnPaint();

// MainWnd.cpp (Line 2469)
void CMainWnd::OnPaint()
{
    // Note: Actual rendering delegated to m_pwndChart
}
```

---

### 3.2 Chart Control Rendering

**위임 대상**: `m_pwndChart` (CWnd*)

- C_Total은 **차트 렌더링 로직을 외부 차트 컨트롤에 위임**
- 내부에서 OnPaint()는 주로 **데이터 검증 및 상태 동기화** 역할

**차트 컨트롤과의 통신**:
```cpp
// MainWnd.cpp (Line 50) in ExtraMsg::InputEvent()
if (m_pMainWnd->m_pwndChart)
    envinfo = (_envInfo*)m_pMainWnd->m_pwndChart->SendMessage(
        GEV_CHART, 
        MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 
        0
    );
```

---

### 3.3 Data-to-Rendering Pipeline

**흐름**:

```
Data Reception
    ↓
OnSiseEvent / OnInputEvent
    ↓
CExtraMsg::{SiseEvent, InputEvent}
    ↓
ReceiveData() / realtime()
    ↓
CExtraMsg member vars (m_code, m_date, etc.) ← STORAGE
    ↓
CMainWnd members (m_arData, m_TrData, m_pEnvInfo) ← CACHED
    ↓
SendMessage(GEV_CHART, ...) → m_pwndChart
    ↓
External Chart Control Rendering
```

---

### 3.4 Performance Optimizations

1. **Double Buffering**: m_TrData 구조체로 수신/처리 버퍼 분리
2. **Lazy Update**: OnTimer 또는 명시적 메시지로만 업데이트
3. **Data Caching**: m_arData/m_arLen 배열로 이전 상태 유지

---

## 4. Message Flow Diagram

```
External Data Source (증권사 실시간 시세)
    ↓
    ↓ SendMessage(GEV_SISE, MAKEWPARAM(siseRTMData, ...), pcData)
    ↓
CMainWnd::OnSiseEvent()
    ↓
CExtraMsg::SiseEvent()
    ├─→ case SISE_TR_DATA: ReceiveData(pcData, iLen)
    │       └─→ Parse tokens → Store in m_code, m_date, m_sahg, etc.
    │
    └─→ case siseRTMData: realtime(pcData)
            └─→ Update m_curr, m_udyl

User Input (패널 컨트롤)
    ↓
    ↓ SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, ...), ...)
    ↓
CMainWnd::OnInputEvent()
    ↓
CExtraMsg::InputEvent()
    ├─→ case INPUT_DATA: GetInputTRInfo(pHeader)
    │       └─→ Format header for external request
    │
    └─→ case INPUT_CHANGE: Handle unit/gap/rotation change

Timer / Chart Event
    ↓
OnTimer / OnChartEvent
    ↓
CMainWnd direct update or m_pwndChart message

Rendering
    ↓
OnPaint (minimal) → Delegate to m_pwndChart
```

---

## 5. Data Type Reference

### 5.1 _trData (Transaction Data)

**정정 (2026-08-27, 실제 정의로 교체)** — 실제 정의는 `axisgwin.h:129`:

```cpp
struct _trData
{
	int	iLen[2];
	int	iSiseLen[2];
	char*	pcData[2];
};
```

2-슬롯 배열(`m_iTRIndex`로 0/1 선택, 다단계 TR 요청 지원). 상세 사용법(어느 분기가 어느 슬롯을
채우는지)은 [C_TotalTRProtocol.md](C_TotalTRProtocol.md)의 "응답(수신) 구조체" 참고.

### 5.2 _envInfo (Environment Info)

```cpp
struct _envInfo {
    struct {
        struct {
            DWORD dwTotCnt;     // Total available days
            DWORD dwDisCnt;     // Display-able days
        } aaDayInfo[4][15];     // [GU_* unit][GI_* index]
        char cToday;
    } datainfo;
};
```

### 5.3 Output Format (ReceiveData Token Order)

파싱 순서 (코드 유형별):
- **주식 (GU_CODE)**: TIME, NAME, JGGA, DIFF, RATE, GVOL, MDHG, MSHG, SIGA, KOGA, JEGA, CODE, CDAY, SAHG, HAHG, JJGA, STCK, START
- **지수 (GU_INDEX)**: 유사 구조 (O*_* 상수 사용)
- **선물/옵션/외환/상품**: 각각의 필드 매핑

---

## 6. Thread Safety Considerations

- **CString**: MFC 내장, 스레드 안전 (Copy-On-Write)
- **CArray**: 단일 스레드 가정, 동기화 필요시 호출부에서 처리
- **CExtraMsg**: 멤버 변수들이 각 데이터 수신 시 덮어쓰기 → 동시성 문제 가능

---

## 7. Key Takeaways

1. **분산 메시지 기반**: 모든 데이터 입출력이 SendMessage() 기반
2. **임시 저장소**: CExtraMsg는 마지막 수신 데이터만 유지
3. **차트 컨트롤 위임**: 실제 렌더링은 외부 m_pwndChart에 위임
4. **다중 데이터 채널**: GEV_SISE, GEV_INPUT, GEV_CHART 분리
5. **파일 기반 상태 복원**: ReadEnv/ReadGrp/ReadData로 영속성 확보

