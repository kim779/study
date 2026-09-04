---
project: ibks
category: note
status: 작성됨
updated: 2026-08-28
---

# axisGData 캔들(OHLC) 구성 방식

## 목차

- [개요](#개요)
- [주기 구분 (일/주/월/분/초/틱)](#주기-구분-일주월분초틱)
- [과거 봉: 서버가 만들어서 내려줌](#과거-봉-서버가-만들어서-내려줌)
- [실시간 봉: 클라이언트가 직접 만듦](#실시간-봉-클라이언트가-직접-만듦)
- [핵심 구조체](#핵심-구조체)
- [코드 근거](#코드-근거)
- [히스토리](#히스토리)

---

## 개요

`C_Total`이 `axisGMain.dll`(`AxisGMain_GDlgDependency.md` 참고)에 위임하는 차트 렌더링의
데이터 계층은 `axisGData.lib`(`CONTROL/ibk_chart_dll_20220831/chart_dll/gData/`)가 담당한다.
질문: "차트 데이터를 조회할 때 월/일/분/초 구분이 있고, 그 데이터를 기반으로 시고저종(OHLC)을
만들어서 캔들을 구성하는가?" — 답은 **경우에 따라 다르다**:

- **이미 지나간(확정된) 봉**: 서버(TR 응답)가 이미 시가/고가/저가/종가를 계산해서 내려주고,
  클라이언트(`COrgData::AttachGraphData`)는 그 값을 그대로 파싱만 한다.
- **지금 진행 중인(미확정) 봉**: 서버는 체결가(현재가) 하나만 실시간으로 내려주고,
  클라이언트(`COrgData::UpdateRTM`)가 그 현재가로 종가를 갱신하고 고가/저가와 비교해서
  직접 캔들을 만들어 나간다.

---

## 주기 구분 (일/주/월/분/초/틱)

TR 요청/응답 헤더 `_dataH`(`axisgenv.h:644`)에 주기 관련 필드가 있다:

```cpp
struct _dataH {
	...
	char		dunit;    // 일주월... (시장/종목 구분과 얽혀 있음, GU_CODE/GU_INDEX/GU_FUTURE 등)
	unsigned char	dindex;   // Data Index — 실제 주기(일/주/월/분/틱)
	char		lgap[4];  // n분, n틱
	char		ltic[4];  // last tick count
	...
};
```

`COrgData`(`OrgData.h`) 쪽 멤버로 보면 더 명확하다:

```cpp
int	m_iDtUnit;    // Data Unit - 주식/지수/선물/옵션/해외 등 시장 구분 (GU_CODE, GU_INDEX, GU_FUTURE...)
int	m_iDtIndex;   // Data Index - 일, 주, 월, 분, 틱 (GI_DAY, GI_WEEK, GI_MONTH, GI_MINUTE, GI_TICK)
int	m_iMGap;      // xxx: xx분 차트 / 9xx: xx초 차트 (분·초는 이 필드 하나로 구분)
int	m_iTGap;      // xxx: xx틱 차트
```

즉 **`m_iDtUnit`은 "무엇을"(종목/지수/선물/옵션/해외)**, **`m_iDtIndex`는 "어떤 주기로"(일/주/월/분/틱)**
를 나타내는 서로 다른 축이다. "초봉"은 별도 인덱스가 아니라 `m_iMGap` 값이 900 이상(9xx)이면
초 단위로 해석하는 방식으로 분봉과 같은 축에서 구분된다.

---

## 과거 봉: 서버가 만들어서 내려줌

`COrgData::AttachGraphData()`(`OrgData.cpp:118`)가 TR 응답 버퍼를 파싱하는 함수다. 데이터는
`'\t'`로 구분된 토큰이 한 줄(`'\n'`)에 한 봉씩 오고(`_dataH` 뒤에 이어짐, `axisgenv.h`의
"OOP data format" 주석 참고), 시장/주기 조합별로 필드 개수와 순서가 다르지만 **패턴은 동일**하다
— 날짜/시간 다음에 시가·고가·저가·종가가 그대로 개별 필드로 온다:

**주식 & (일봉|주봉|월봉)** — `m_iDtUnit==GU_CODE && (GI_DAY|GI_WEEK|GI_MONTH)`, 10개 필드:

```cpp
switch (jj) {
case 0: date(yy/mm/dd) = token;         // yyyymmdd
case 1: time(hh/mm/ss) = token;         // hhmmss
case 2: pgbData->m_iOpen  = atoi(token); // 시가
case 3: pgbData->m_iHigh  = atoi(token); // 고가
case 4: pgbData->m_iLow   = atoi(token); // 저가
case 5: pgbData->m_iClose = atoi(token); // 종가
case 6: pgbData->m_dVolume   = atof(token); // 거래량
case 7: pgbData->m_dTrdMoney = atof(token); // 거래대금
case 8: pgbData->m_dwCheck = atoi(token);   // check flag(권리락 등)
case 9: pgbData->m_wRate   = atoi(token);   // check rate
}
```

다른 분기들(`GU_FOREIGN`=해외 6필드, `GU_INDEX&&GI_DAY`=업종일봉, `GU_CFUTURE`/`GU_FFUTURE`=채권·상품선물
9필드, 그리고 분/초/틱을 포함한 나머지 전부를 담당하는 마지막 `else` 8필드)도 필드 개수만
다를 뿐 **"시가→고가→저가→종가" 순서로 이미 계산된 값이 온다는 원칙은 동일**하다. 클라이언트는
`atoi(token)`으로 문자열을 숫자로 바꿔 `CGrpBasic`에 대입할 뿐, 어떤 형태로든 시고저종을
직접 계산(예: 원시 체결가들을 모아서 max/min 구하기)하는 코드는 `AttachGraphData` 안에 없다.

---

## 실시간 봉: 클라이언트가 직접 만듦

`COrgData::UpdateRTM(bool& rbIncrease)`(`OrgData.cpp:709`)는 실시간 체결가(`m_pDataMgr->GetCurr()`,
현재가)가 갱신될 때마다 호출된다. 여기서부터는 서버가 봉을 만들어주지 않고, **클라이언트가
직접 마지막 봉을 갱신하거나 새 봉을 추가**한다.

### 분봉 (`GI_MINUTE`)

```cpp
if (iCheckGap > 0 || iCheckGap < -7200 || pgbEnd->m_bDummy)
{
    // 새 분 구간 시작 → 새 봉 생성, O=H=L=C=현재가로 초기화
    pgbNew->m_iClose = atoi(m_pDataMgr->GetCurr());
    pgbNew->m_iOpen = pgbNew->m_iHigh = pgbNew->m_iLow = pgbNew->m_iClose;
}
else
{
    // 같은 분 구간 → 기존 봉 갱신
    pgbEnd->m_iClose = atoi(m_pDataMgr->GetCurr());
    if (pgbEnd->m_iClose > pgbEnd->m_iHigh) pgbEnd->m_iHigh = pgbEnd->m_iClose;
    else if (pgbEnd->m_iClose < pgbEnd->m_iLow) pgbEnd->m_iLow = pgbEnd->m_iClose;
}
```

`iCheckGap`은 현재 실시간 시각(`m_pDataMgr->GetCTIM()`)과 마지막 봉의 시각을 초 단위로 뺀
값 — 분 경계를 넘었으면 새 봉, 아니면 같은 봉 갱신. **고가/저가는 "현재가가 기존 고가보다
크면 고가 갱신, 작으면 저가 갱신"이라는 전형적인 실시간 캔들 갱신 로직으로 클라이언트가
직접 계산**한다.

### 틱봉 (`GI_TICK`)

분봉과 동일한 원리, 다만 시간 대신 틱 카운트(`m_iLastTick` vs `m_iTGap`)로 봉 경계를 판단한다.

### 주봉/월봉 (`GI_WEEK`/`GI_MONTH`)

당일 진행 중인 주/월봉의 종가·고가·저가를 실시간 체결가로 계속 갱신(새 봉 생성 로직은 별도,
날짜가 바뀔 때 `m_bDateAdd` 플래그로 처리).

### 일봉 (`GI_DAY`)

**정정** — 캔들 갱신용 두 번째 `switch(m_iDtIndex)`에는 `GI_DAY`가 명시적 케이스로 없고,
대신 `default:` 케이스(주석 `// GI_DAY`)가 그 역할을 한다(`OrgData.cpp:936`). 여기서
거래량/거래대금/미결제약정을 먼저 갱신한 뒤, `m_iDtUnit`(시장 구분)별로 다시 분기한다.

**`GU_CODE`(일반 주식, 최종 `default` 분기, `OrgData.cpp:978`)** — 분봉/틱봉과 달리 클라이언트가
직접 고가/저가를 비교·계산하지 않고, **실시간 시세 필드에 이미 들어있는 당일 시가/고가/저가를
그대로 가져다 쓴다**:

```cpp
pgbEnd->m_iClose = atoi(m_pDataMgr->GetCurr());  // 종가 = 현재가
pgbEnd->m_iOpen  = atoi(m_pDataMgr->GetSiga());  // 시가 = 실시간 시세의 당일 시가
pgbEnd->m_iHigh  = atoi(m_pDataMgr->GetKoga());  // 고가 = 실시간 시세의 당일 고가
pgbEnd->m_iLow   = atoi(m_pDataMgr->GetJega());  // 저가 = 실시간 시세의 당일 저가
```

`GetSiga()`/`GetKoga()`/`GetJega()`는 클라이언트가 계산한 값이 아니라 실시간 시세 패킷 자체에
포함된 필드(거래소가 이미 계산해서 매 틱마다 같이 내려주는 당일 시가/고가/저가)를 그대로 담아둔
것 — 즉 **일봉(일반 주식)의 당일 진행 중 고가/저가는 분봉/틱봉과 달리 클라이언트가 max/min
비교로 만드는 게 아니라 거래소가 실시간으로 계산해서 보내주는 값을 그대로 반영**한다는 점이
분봉/틱봉과의 핵심 차이다.

**`GU_INDEX`(업종/지수, `OrgData.cpp:942`)**: 분봉과 같은 방식으로 클라이언트가 직접
`if(close>high) high=close` 비교를 한다.

**`GU_FFUTURE`(상품선물, `OrgData.cpp:957`)**: 실시간 시세에 저가(`GetJega`)가 유효하면
(`>0`) 시가/고가/저가를 시세 필드 그대로 반영, 없으면(0 이하) `GU_INDEX`처럼 클라이언트가
직접 비교해서 갱신 — 두 방식이 혼재.

정리하면 "일봉 실시간 갱신"은 시장 구분에 따라 **①시세 패킷의 시가/고가/저가를 그대로 복사**
하는 방식(일반 주식)과 **②클라이언트가 현재가와 비교해서 직접 계산**하는 방식(지수, 그리고
선물 중 시세 필드가 비어있는 경우)이 나뉘어 있다.

---

## 핵심 구조체

### `CGrpBasic` — 봉 하나 (`GrpBasic.h`)

```cpp
class CGrpBasic {
public:
	int	m_iOpen;    // 시가
	int	m_iHigh;    // 고가
	int	m_iLow;     // 저가
	int	m_iClose;   // 종가
	double	m_dVolume;    // 거래량
	double	m_dTrdMoney;  // 거래대금
	int	m_iMgjy;    // 미결제약정(선물/옵션)
	struct _gIndex {
		struct _date { short yy; unsigned char mm, dd; } date;
		struct _time { unsigned char hh, mm, ss; } time;
	} m_index;
	// + 외국인/기관/개인 매매동향 필드 다수
};
```

### `COrgData` — 종목 하나의 전체 봉 배열 (`OrgData.h`)

`CArray<CGrpBasic*, CGrpBasic*> m_arDataQue`로 봉들을 순서대로 보관. `m_iDtUnit`/`m_iDtIndex`/
`m_iMGap`/`m_iTGap`로 이 배열이 어떤 시장·주기의 데이터인지 표시.

---

## 코드 근거

- `_dataH` 정의: `CONTROL/ibk_chart_dll_20220831/h/axisgenv.h:644-668`
- `COrgData` 멤버(`m_iDtUnit`/`m_iDtIndex`/`m_iMGap`/`m_iTGap`): `.../gData/OrgData.h:33-40`
- `CGrpBasic` 정의: `.../gData/GrpBasic.h`
- 과거 봉 파싱: `.../gData/OrgData.cpp` `COrgData::AttachGraphData()` (118줄~), 특히
  일/주/월봉 분기 199~243줄, 마지막 `else`(분/초/틱 등 나머지) 366줄대
- 실시간 봉 갱신: `.../gData/OrgData.cpp` `COrgData::UpdateRTM()` (709줄~), 분봉 860줄,
  틱봉 889줄, 주/월봉 925줄, 일봉(`default`, `GU_CODE`) 936~984줄

---

## 히스토리

- **초기 작성**: 2026-08-27
- **2026-08-27 보강**: 일봉(`GI_DAY`) 실시간 갱신 로직 확인 — `default` 케이스로 처리되며,
  일반 주식(`GU_CODE`)은 시세 패킷의 시가/고가/저가를 그대로 복사, 지수(`GU_INDEX`)는
  클라이언트가 직접 비교 계산한다는 차이를 반영.
