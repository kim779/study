# C_UserGrp: Data Flow Architecture

## Overview

C_UserGrp (사용자 정의 그래프 DLL)는 외부로부터 구조화된 데이터를 수신하여 메모리 저장소에 저장한 후, 동적 그래프 표현으로 렌더링하는 아키텍처입니다. 특히 **메모리 기반 데이터 구조**와 **실시간 갱신** 메커니즘이 특징입니다.

주요 클래스: `CMainWnd`, `CGrpWnd`, `CCtrlWnd`, `CGrp_Data`

---

## 1. Data Reception (데이터 수신)

### 1.1 Primary Message Channel: CM_GRP (Graph Messages)

**진입점**: `CGrpWnd::OnGrp(WPARAM wParam, LPARAM lParam)` (GrpWnd.cpp Line 150)

```cpp
// GrpWnd.cpp (Message Map)
BEGIN_MESSAGE_MAP(CGrpWnd, CWnd)
    // ...
    ON_MESSAGE(CM_GRP, OnGrp)
END_MESSAGE_MAP()

LONG CGrpWnd::OnGrp(WPARAM wParam, LPARAM lParam)
{
    switch (LOWORD(wParam))
    {
    case GRP_AttachData:
        return DispatchData(wParam, lParam);
    case GRP_RealTime:
        RealTimeData((struct _alertR*)lParam);
        break;
    // ...
    }
}
```

**메시지 구조**:
- `LOWORD(wParam)`: 그래프 명령 (GRP_AttachData, GRP_RealTime, GRP_Control, etc.)
- `HIWORD(wParam)`: 세부 옵션
- `lParam`: 데이터 포인터 또는 구조체

---

### 1.2 Data Dispatch and Routing

**메서드**: `CGrpWnd::DispatchData(WPARAM wParam, LPARAM lParam)` (GrpWnd.cpp Line 467)

```cpp
int CGrpWnd::DispatchData(WPARAM wParam, LPARAM lParam)
{
    // Route based on HIWORD(wParam)
    switch (HIWORD(wParam))
    {
    case HEADER_DATA:       // 헤더(메타데이터) 수신
        // 종목 정보, 필드 이름 등
        break;
    
    case GRAPH_DATA:        // 그래프 데이터 수신
        // m_pDataInfo->AttachGraphData() 호출
        return m_pDataInfo->AttachGraphData(
            (char*)lParam, 
            nHead,          // 헤더 크기
            nData,          // 데이터 크기
            iSize,          // 레코드 크기
            dSize           // 필드 개수
        );
    
    case REALTIME_DATA:     // 실시간 데이터 수신 (미사용 경로)
        break;
    }
}
```

---

### 1.3 Real-Time Data Channel

**진입점**: `CGrpWnd::RealTimeData(struct _alertR* alert)` (GrpWnd.cpp Line 599)

**데이터 소스**:
- AXIS 플랫폼 Alert 구조체 (실시간 시세 푸시)
- 또는 `OnUser()` 메시지로부터 받은 구조체

**처리 흐름**:
```cpp
void CGrpWnd::RealTimeData(struct _alertR* alert)
{
    for (int ii = 0; ii < m_nInfo; ii++)
    {
        // 각 종목에 대해
        CString sCode = m_pDataInfo->m_pInfo[ii]->RtsCode;
        CString sSymbol = m_pDataInfo->m_pInfo[ii]->RtsSymbol;
        
        if (alert->code == sCode)  // 일치하는 종목?
        {
            CString sTime = ExtractTime(alert->...);
            CString sVal = ExtractValue(alert->...);
            
            // 실시간 데이터 저장
            iResult = m_pDataInfo->RealTimeData(ii, sTime, sVal, bInsert);
            
            // 그래프 갱신
            InvalidateRect(...);  // 다시 그리기 요청
        }
    }
}
```

---

### 1.4 External API Channels

**공개 메서드** (MainWnd.h):

#### **SetData(LPCTSTR str)**
```cpp
// COM dispatch interface
afx_msg void SetData(LPCTSTR str);

// 호출: 외부 클라이언트에서 직렬화 포맷으로 데이터 전달
// 형식: "[header]\t[data1]\t[data2]..."
```

**처리**: MainWnd → GrpWnd → DispatchData → CGrp_Data::AttachGraphData

#### **Clear()**
```cpp
afx_msg void Clear();  // 모든 데이터 초기화
```

**처리**: MainWnd → GrpWnd → ClearGrp()

#### **SetRTS(BOOL bShow) / SetRTSCode(BSTR newVal)**
```cpp
afx_msg void SetRTS(BOOL bShow);              // 실시간 데이터 토글
afx_msg void SetRTSCode(BSTR newVal);         // 실시간 심볼 설정
```

---

### 1.5 Timer-Based Updates

**진입점**: `CGrpWnd::OnTimer(UINT nIDEvent)` (GrpWnd.cpp Line 244)

**용도**:
- 주기적 데이터 갱신 폴링
- 애니메이션 프레임 업데이트
- 자동 스크롤 처리

---

### 1.6 Control Panel Events

**경로**: `CCtrlWnd` → `SendMessage(CM_GRP, ...)` → `CGrpWnd::OnGrp`

**이벤트 유형**:
- `ChgDispScale`: 디스플레이 스케일 변경 (Zoom In/Out)
- `ChgDisplay`: 표시 범위 변경 (스크롤)
- `ChgShift`: 시프트 동작 (데이터 이동)
- `ExcelData`: 엑셀 내보내기 요청
- `GetScrData`: 스크린 데이터 조회

---

## 2. Data Storage (데이터 저장)

### 2.1 Primary Storage: CGrp_Data Class

**파일**: Grp_Data.h / Grp_Data.cpp

**멤버 변수** (Grp_Data.h Line 40-51):

```cpp
class CGrp_Data  
{
public:
    // 메타데이터 (종목 정보)
    std::vector<std::unique_ptr<_ItemInfo>>  m_pInfo;    // 종목 배열
    int                                     m_nInfo;     // 종목 개수
    bool                                    m_bRealTime; // 실시간 여부
    CArray <CString, CString>               m_arItemRts; // 실시간 심볼 배열

private:
    // 실제 데이터 저장소
    std::map<int, std::shared_ptr<struct _cgNValx>>  _mData;  // 맵 기반 (인덱스)
    std::vector<std::shared_ptr<struct _cgNValx>>    _vData;  // 벡터 기반 (시간순)
    
    int  m_szFrame;     // 1 Frame Size (1 Que Size)
    int  m_dIndex;      // 데이터 인덱스
    int  m_dGap;        // 데이터 갭
    int  m_dCount;      // 값 개수
};
```

---

### 2.2 Item Metadata Structure: _ItemInfo

```cpp
struct _ItemInfo
{
    CString name;           // 종목명 또는 필드명
    CString RtsCode;        // 실시간 코드 (예: "005930")
    CString RtsSymbol;      // 실시간 심볼 (예: "A005930")
    int     RtsSign;        // 실시간 부호 여부
    int     digit;          // 소수점 자릿수
    double  power;          // 10^digit
    int     region;         // 영역 (레이아웃)
    int     pivot;          // 피벗 포인트 (기준값)
};
```

**생명주기**:
- **초기화**: `AttachGraphData()` 헤더 파싱 시 생성
- **갱신**: `RealTimeData(int index, ...)` 호출 시 해당 인덱스만 갱신
- **정리**: 소멸자에서 자동 정리

---

### 2.3 Graph Data Structure: _cgNValx

```cpp
struct _cgNValx  // Chart Graph Numeric Value X (추정 구조)
{
    CString     sKey;       // 키 (시간 등)
    std::vector<double>    values;   // 각 필드별 수치
    // 또는 다른 형식
};
```

**저장소 이중화**:
- `_mData` (map): 빠른 룩업 (인덱스 → 데이터)
- `_vData` (vector): 시간순 순회 (렌더링용)

---

### 2.4 Storage Life Cycle

#### **초기화 (Initialization)**

```cpp
// AttachGraphData() 호출
int CGrp_Data::AttachGraphData(char* data, int nHead, int nData, int iSize, int dSize)
{
    // 1. 헤더 파싱 → m_pInfo 생성
    InitInfo(nHead);
    
    // 2. 데이터 파싱 → _mData, _vData 채우기
    ParseDataRecords(data, nData);
    
    // 3. 정규화 (Min/Max 계산, 스케일링)
    NormalizeData();
    
    return _vData.size();
}
```

#### **실시간 갱신 (Real-Time Update)**

```cpp
// RealTimeData() 호출
int CGrp_Data::RealTimeData(int index, CString sTime, CString sVal, bool bInsert)
{
    if (bInsert)
    {
        // 새 레코드 삽입 (벡터 끝에 추가)
        auto newVal = CreateNValx(sTime, sVal);
        _vData.push_back(newVal);
        _mData[_vData.size()-1] = newVal;
    }
    else
    {
        // 기존 레코드 갱신 (마지막 항목)
        if (!_vData.empty())
            UpdateNValx(_vData.back(), sVal);
    }
    
    return _vData.size();
}
```

#### **정리 (Cleanup)**

```cpp
~CGrp_Data()
{
    // std::unique_ptr / std::shared_ptr 자동 정리
    // m_pInfo, _mData, _vData 모두 소멸
}
```

---

### 2.5 Graph Container: CGrpWnd

**클래스**: CGrpWnd (GrpWnd.h/cpp)

**멤버 변수** (GrpWnd.h Line 62-92):

```cpp
class CGrpWnd : public CWnd
{
public:
    // 데이터 저장소
    std::unique_ptr<class CGrp_Data>          m_pDataInfo;  // 주 데이터
    std::array<std::unique_ptr<class CGrp_Base>, MAX_GRAPH>  m_pGrpInfo;  // 각 그래프
    std::array<std::unique_ptr<struct _RgnInfo>, MAX_REGION> m_pRgnInfo;  // 영역 배치
    
    // 레이아웃 정보
    int           m_RgnCnt;        // 영역 개수
    int           m_GrpCnt;        // 그래프 개수
    int           m_totalDay;      // 전체 데이터 행 수
    int           m_dispPos;       // 디스플레이 시작 위치
    int           m_dispEnd;       // 디스플레이 끝 위치
    int           m_dispDay;       // 디스플레이 범위
    
    // 미니맥스 (스케일링)
    std::vector<std::unique_ptr<struct _NMinMaxChk>>  m_pMinMaxChk;
    int           m_MinMaxCnt;
};
```

---

## 3. Rendering (렌더링)

### 3.1 Paint Entry Point

**진입점**: `CGrpWnd::OnPaint()` (GrpWnd.cpp Line 225)

```cpp
void CGrpWnd::OnPaint()
{
    CPaintDC dc(this);
    
    // 1. 더블 버퍼링 사용 (성능 최적화)
    CDC mdc;
    mdc.CreateCompatibleDC(&dc);
    
    // 2. 그래프 그리기
    DrawGraph(&mdc);
    
    // 3. 화면에 비트블릿
    dc.BitBlt(0, 0, m_ObjRect.Width(), m_ObjRect.Height(), 
              &mdc, 0, 0, SRCCOPY);
}
```

---

### 3.2 Graph Drawing Pipeline

**메서드 호출 순서**:

```
OnPaint()
    ↓
DrawGraph(CDC* pDC)  [GrpWnd.cpp Line 808]
    ↓
    ├─→ DrawEmpty(pDC, true)        // 배경 초기화
    │       └─→ DrawFrameRect()      // 프레임 그리기
    │
    ├─→ for (int ii = 0; ii < m_GrpCnt; ii++)
    │       m_pGrpInfo[ii]->DrawGraph(pDC);  // 각 그래프 그리기
    │
    └─→ DrawEmpty(pDC, false)       // 틱/라벨 그리기
            └─→ DrawTicks()         // 눈금 그리기
            └─→ DrawLabels()        // 레이블 그리기
```

---

### 3.3 Individual Graph Rendering

**클래스**: `CGrp_Base` (파생: CGrp_Bar, CGrp_Line, CGrp_VBar)

**메서드**: `virtual void DrawGraph(CDC* pDC)`

각 그래프 유형이 자신의 그리기 방식 구현:

- **CGrp_Bar**: 막대 그래프
- **CGrp_Line**: 선 그래프
- **CGrp_VBar**: 수직 막대 그래프

**공통 렌더링 단계**:
1. 데이터 범위 검증 (m_dispPos ~ m_dispEnd)
2. 좌표 변환 (데이터 → 픽셀)
3. 스케일링 (Min/Max 정규화)
4. 그리기 (GDI 또는 DC 메서드)

---

### 3.4 Region Layout Management

**구조체**: `_RgnInfo` (GrpWnd.h)

```cpp
struct _RgnInfo  // Region Info
{
    int tkHeight;
    struct _tick tick[3];       // left/right/bottom
    CRect gpRect;               // graph rect (렌더링 영역)
};
```

**할당**: `CGrpWnd::AssignRegion(int nRgn)` (GrpWnd.cpp)
- 각 그래프가 차지할 CRect 영역 설정
- 윈도우 리사이즈 시 자동 재계산

---

### 3.5 Real-Time Data Rendering

**갱신 트리거**: `CGrpWnd::RealTimeData(struct _alertR* alert)`

1. 데이터 저장 (m_pDataInfo->RealTimeData)
2. 화면 갱신 요청 (InvalidateRect)
3. OnPaint() 다시 호출 → 최신 데이터로 그리기

---

### 3.6 Performance Optimizations

1. **Double Buffering**: 메모리 DC에 그린 후 화면에 한 번에 출력
2. **Dirty Region Tracking**: 변경된 영역만 다시 그리기
3. **Indexed Data**: _mData 맵으로 빠른 수정 접근
4. **Vectorized Storage**: _vData로 순차 렌더링 최적화

---

## 4. Message Flow Diagram

```
External Data Source (Active-X Client / AXIS Platform)
    ↓
    ├─ COM Method: SetData(str) 
    │   └─→ MainWnd::SetData()
    │       └─→ GrpWnd::SendMessage(CM_GRP, GRP_AttachData, ...)
    │
    ├─ SendMessage(CM_GRP, GRP_RealTime, (LPARAM)pAlertR)
    │   └─→ CGrpWnd::OnGrp()
    │       └─→ case GRP_RealTime: RealTimeData(pAlertR)
    │
    └─ SendMessage(CM_GRP, GRP_AttachData, (LPARAM)pData)
        └─→ CGrpWnd::OnGrp()
            └─→ case GRP_AttachData: DispatchData()
                └─→ m_pDataInfo->AttachGraphData(pData, ...)

Data Storage
    ↓
CGrp_Data::{AttachGraphData, RealTimeData}
    ├─→ InitInfo()          // 헤더 파싱 → m_pInfo 생성
    ├─→ ParseDataRecords()  // 데이터 파싱 → _mData, _vData 채우기
    └─→ NormalizeData()     // Min/Max 계산 및 스케일링

Rendering
    ↓
CGrpWnd::OnPaint()
    ├─→ CreateCompatibleDC()    // 더블 버퍼링
    ├─→ DrawGraph(mdc)
    │   ├─→ DrawEmpty(true)     // 배경
    │   ├─→ for (m_pGrpInfo[ii]->DrawGraph) // 각 그래프
    │   └─→ DrawEmpty(false)    // 틱/라벨
    └─→ BitBlt()                // 화면에 출력

User Interaction (Control Panel)
    ↓
CCtrlWnd::OnLButtonDown/OnHScroll/OnTimer
    └─→ SendMessage(CM_GRP, MAKEWPARAM(GRP_Control, ChgDisplay), pos)
        └─→ CGrpWnd::OnGrp()
            ├─→ ChangeDisplay(pos)   // m_dispPos, m_dispDay 갱신
            └─→ InvalidateRect()     // 다시 그리기
```

---

## 5. Data Type Reference

### 5.1 _ItemInfo (종목 메타데이터)

```cpp
struct _ItemInfo
{
    CString name;           // "삼성전자" 또는 "시가"
    CString RtsCode;        // "005930" (실시간 코드)
    CString RtsSymbol;      // "A005930" (심볼)
    int RtsSign;            // 부호 여부 (양수/음수/백분율)
    int digit;              // 소수점 자릿수
    double power;           // 10^digit
    int region;             // 영역 인덱스 (0~MAX_REGION)
    int pivot;              // 기준값 (예: 피벗 포인트)
};
```

### 5.2 _cgNValx (차트 데이터 포인트)

```cpp
struct _cgNValx
{
    // 추정 구조 (정확한 정의는 헤더 미확인)
    CString sKey;                       // 시간/날짜 키
    std::vector<double> values;         // 각 필드 값
    // 또는: double open, high, low, close, volume; 등
};
```

### 5.3 _alertR (실시간 알림)

```cpp
struct _alertR  // AXIS Alert Real-Time (추정)
{
    CString code;           // 종목 코드
    CString symbol;         // 심볼
    CString time;           // 시간 (HH:MM:SS)
    double price;           // 가격
    int volume;             // 거래량
    int sign;               // 등락 방향 (+/-)
    // ... 기타 필드
};
```

### 5.4 _NMinMaxChk (스케일 정규화)

```cpp
struct _NMinMaxChk
{
    double dMin;            // 최소값
    double dMax;            // 최대값
    double dGap;            // 단계 간격
    int iCount;             // 눈금 개수
};
```

---

## 6. Control Panel Architecture

**클래스**: `CCtrlWnd` (CtrlWnd.h/cpp)

**역할**: 스크롤/줌 컨트롤 제공

**주요 메서드**:
- `UpdateScroll(bool bGetData = true)`: 스크롤바 상태 갱신
- `InitScroll(int pos, int end, int day, int total)`: 초기화
- `OnHScroll/OnMouseMove/OnTimer()`: 사용자 입력 처리

**메시지 패턴**:
```cpp
// CtrlWnd에서 사용자 이벤트 발생
CCtrlWnd::OnLButtonDown() 또는 OnHScroll()
    ↓
m_pParent->SendMessage(CM_GRP, MAKEWPARAM(GRP_Control, ChgDisplay), pos)
    ↓
CGrpWnd::OnGrp() 처리
    ├─→ m_dispPos = pos
    ├─→ RecalculateMinMax()
    └─→ InvalidateRect()  // 다시 그리기
```

---

## 7. Thread Safety Considerations

- **std::unique_ptr**: 단일 소유권, 동기화 불필요
- **std::vector / std::map**: 단일 스레드 가정
- **CString**: MFC 내장, Copy-On-Write 사용
- **CArray**: 단일 스레드 가정

**동시성 필요시**:
- 실시간 데이터 수신과 렌더링 분리
- Lock (CriticalSection/Mutex) 필요

---

## 8. Key Takeaways

1. **메시지 기반 아키텍처**: 모든 통신이 SendMessage() / PostMessage()
2. **메모리 저장소**: 파일이 아닌 메모리 (_mData, _vData) 중심
3. **렌더링 캡슐화**: CGrp_Base 파생 클래스로 그래프 유형별 독립 구현
4. **이중 저장소**: map (빠른 수정) + vector (시간순 순회) 병용
5. **실시간 갱신**: Alert 콜백으로 부분 업데이트 후 즉시 InvalidateRect
6. **레이아웃 관리**: _RgnInfo로 영역 분할 및 위치 결정
7. **스케일 정규화**: Min/Max 계산으로 데이터 크기 자동 정규화

