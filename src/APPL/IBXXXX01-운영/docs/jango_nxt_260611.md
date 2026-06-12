# 실시간잔고 NXT 종목코드 처리 수정 (2026-06-11)

## 모듈 개요

`Remain.cpp` / `Remain.h` — 실시간 잔고 계산 모듈 (`CRemain` 클래스)

---

## 배경

NXT 시장 도입으로 신규 체결 수신 시 종목코드가 기존 7바이트(`A000660`)에서
`N.A000660`, `M.A000660` 형태(9바이트)로 들어오는 케이스 추가.

## keyS 구조

`getKey()` (MapWnd.cpp:1732) 가 생성하는 keyS 레이아웃:

```
[jggb(2)] [codeS padded(LEN_ACODE=12)] [date(8)] [sygb(2)] |trkey
예: "00" + "N.A000660   " + "20240101" + "01" + "|1"
```

관련 상수 (DefineAll.h):
- `LEN_JGGB  = 2`
- `LEN_HCODE = 7`  (주식 종목코드 길이)
- `LEN_ACODE = 12` (코드 필드 전체 — keyS 에서 항상 12바이트 패딩)
- `LEN_DATE  = 8`

## 수정 내용 (Remain.cpp `CalRemainData`)

```cpp
// 기존
CString codeS = keyS.Mid(LEN_JGGB, LEN_HCODE);

// 수정 후
CString codeS;
if(keyS.Find("N.") >= 0 || keyS.Find("M.") >= 0)
    codeS = keyS.Mid(LEN_JGGB, LEN_HCODE + 2);  // 9바이트
else
    codeS = keyS.Mid(LEN_JGGB, LEN_HCODE);       // 7바이트
```

## 검토 결과

| 항목 | 판정 | 비고 |
|------|------|------|
| codeS 추출 로직 | 정상 | N.A000660 → Mid(2,9) 정확 |
| m_code 이후 사용 | 정상 | GetAt(0)=='J'/'Q'/'K'/'B'/'E' 모두 'N'이라 해당 없음 |
| keyS.Find("N.") 범위 | 실용상 문제 없음 | date/sygb 필드는 숫자라 오탐 불가 |

**선택적 개선안** (의도를 더 명확히):
```cpp
if(keyS.Mid(LEN_JGGB, 2) == "N." || keyS.Mid(LEN_JGGB, 2) == "M.")
```

## 기존 버그 (이번 수정과 무관)

`CalRemainData` 내 `dateS` 추출 오프셋이 잘못되어 있음:
```cpp
// 현재 (틀림 — 9번 위치)
CString dateS = keyS.Mid(LEN_JGGB + LEN_HCODE, LEN_DATE);
// 올바른 값 (14번 위치)
CString dateS = keyS.Mid(LEN_JGGB + LEN_ACODE, LEN_DATE);
```
`dateS`는 디버그 로그에만 사용되므로 기능 영향 없음.

## 체결 흐름 (MapWnd.cpp parsingNotice)

1. 일반 코드(`A000660`)로 CodeMap 조회
2. 없으면 `"N." + sJCode` 로 재조회
3. 없으면 `"M." + sJCode` 로 재조회
4. 그래도 없으면 신규 잔고 생성 (`bNewCode=true`) → `CalRemainData` 호출
