# 실시간 시세(RTM) 종목코드 역인덱스 도입 조사

## 배경 / 목표

`CGuard::DoRTM`(Guard.cpp:5898~5982)이 실시간 틱 1건마다 열려있는 **모든 화면**(`m_clients` 전체)을 순회하며 "이 화면이 이 종목에 관심있나"를 검사한다 (`@docs/KnowledgeBase.md` 참고 — 2026-07-19 발견). 화면을 많이 띄운 상태에서 틱이 몰릴 때 UI가 버벅이는 원인으로 추정됨.

**개선 방향:** 종목코드 → 관심 화면 역인덱스를 `CGuard`에 두고, 틱마다 전체순회 대신 인덱스 조회로 대체.

**관건:** 화면의 "종목코드 필드" 값이 바뀌는 모든 지점에서 인덱스를 갱신해야 함 — 누락 시 "시세가 조용히 안 뜨는" 회귀 발생 (성능 문제보다 훨씬 위험).

**목표 일정:** 2026-07-22 저녁까지 실제 소스 적용 (사용자가 그 전까지 직접 소스 리뷰 + 디버깅 시간 가짐).

---

## 확인된 사실

### 1. "종목코드 필드"의 식별 방법 — `FA_FLASH` 속성 플래그 (2026-07-20 정정, 실측 확인 완료)

**정정:** 최초 조사 때는 `PR_CODE`(`FM_EDIT : has code list`, form.h:361)가 식별 플래그라고 봤으나, 이는 **코드조회 팝업(콤보리스트) UI 기능** 표시일 뿐 실시간 매칭과 무관한 것으로 확인됨(`CGuard::SetCode`가 쓰는 목적은 이 팝업 연동, 다른 용도).

**실제 정답:** `form->m_form->attr & FA_FLASH`(`flash key field`, form.h:282)가 설정된 필드가 "이 화면의 실시간 매칭 키 필드"다. 그리고 이 필드는 **`FM_EDIT`이 아니라 `FM_OUT`(kind=9, output)일 수 있다** — 사용자가 눈으로 보는/조회에 쓰는 Edit 필드(예: `IB999987`의 `1301`, `FM_EDIT`, 조회용)와 실시간 키로 쓰이는 필드(같은 맵의 `1021`, `FM_OUT`, `FA_FLASH`)가 **서로 다른 별개의 필드**인 경우가 있다. `1021`은 서버 조회 응답으로 시장구분+종목코드 조합값(예: `A005930`)을 받는다.

**실측 로그 (2026-07-20, `IB999987`, 화면 오픈 시점):**
```
[WIZARD][RTM][DEBUG][ATTACH] mapN=IB999987 name=1021 kind=9 code=
```
(`code=`가 비어있는 건 화면이 막 열린 시점이라 아직 TR 조회 응답이 안 와서임 — 정상. 응답 도착 후 다음 틱에서 자가치유(`OnAlert`의 `m_lastCode` 비교)가 실제값으로 채워줌.)

axisBuilder에서 `1021`의 Attributes 다이얼로그로 직접 확인 — `FLASH` 체크박스가 켜져 있고, 이 체크를 켜야 실시간 갱신이 동작하는 것을 실제 토글 테스트로 확인함(사용자 직접 검증).

**따라서 `CScreen::Parse()`의 attach 로직은 `FA_FLASH` 여부 + `kind`가 `FM_GRID`/`FM_TABLE`/`FM_CONTROL`이 아닌 것(=`CScreen::OnAlert`의 default 분기와 동일 기준)으로 판별해야 하며, `FM_EDIT`으로 kind를 좁혀서 판별하면 `FM_OUT` 키 필드를 놓친다.** (최초 구현 시도 때 `case FM_EDIT:`으로 좁혀서 테스트했다가 실제 키 필드(`FM_OUT`)를 못 잡아서 한 차례 혼선 있었음 — 위 실측으로 해결.)

~~근거(정정 전, 참고용 — `PR_CODE`는 실시간 매칭과 무관하므로 이 예시는 폐기): `CGuard::SetCode(CScreen* screen, ...)` (Guard.cpp:2305)는 `PR_CODE` 필드를 찾아 코드조회 팝업과 연동하는 별개 기능이다.~~

### 2. 읽기측(매칭) — `CScreen::OnAlert`

`Screen.cpp:755~804`. 화면 로드 시(`Parse()`) `FA_FLASH` 속성이 붙은 필드들이 `m_flashObs`에 등록됨. 틱이 오면 `m_flashObs`를 순회하는데, `FM_GRID`/`FM_TABLE`/`FM_CONTROL`이 아닌 나머지("default" 분기, 대개 `PR_CODE`가 붙은 `FM_EDIT`)는:

```cpp
default:
    form->ReadData(text);      // 캐시된 값이 아니라 매번 라이브로 읽음
    text.TrimRight();
    if (!text.Compare(code))   // 틱의 종목코드와 비교
    {
        flash = true;
        UpdateRTM(key+1, code, update, fms, obs, stat, alertR);
    }
    break;
```

**중요:** 화면에 "현재 코드"를 저장해두는 멤버 변수가 없다. 매 틱마다 필드 UI 값을 직접 읽어서 비교한다. 역인덱스를 만들면 이 `ReadData`+`Compare` 자체가 없어지고 인덱스 조회로 대체된다.

### 3. 쓰기측(값이 바뀌는 경로) — 최소 3갈래 확인됨

**(a) 사용자 직접 입력** — `dll/form/fmEdit.cpp`의 키보드 입력 처리 경로 (아직 상세 미조사).

**(b) 도미노(화면간 코드연동) — 두 함수 확인**

- `CClient::RestoreDomino(CScreen*, CMapStringToString&, bool)` (Client.cpp:1766) — 범용 필드 동기화. `PR_CODE` 여부와 무관하게 폼 이름으로 `dominos` 맵을 조회해서 `form->WriteData(text)` 호출.
- `CClient::CodeDomino(CScreen*, CMapStringToString&)` (Client.cpp:1845) — `PR_CODE`가 아닌 필드는 `m_guard->GetCode()`로 코드값을 채우고, `PR_CODE` 필드 자체는 `dominos` 맵에서 찾아 `form->WriteData(text)`.
- 두 함수 다 `form->WriteData(text)` 직후 `form->IsChanged(); m_vm->OnChange(screen, form);`을 호출함 — 즉 "필드 값이 바뀜"과 "스크립트 OnChange 이벤트 발생"이 같은 지점에서 일어남.
- 트리거 지점: `CClient::OnDomino(CString domino)` (Client.cpp:477) → `SaveDomino()` + `RestoreDomino(true)`. 언제 `OnDomino`가 호출되는지는 아직 미추적 (포커스 전환/화면전환 시점으로 추정, 확인 필요).

**(c) `fmEdit::WriteData`의 `PR_CODE` 특수분기 — 기대와 다름**

`dll/form/fmEdit.cpp:1261~1296`:
```cpp
void CfmEdit::WriteData(CString data, bool redraw, int col, int row)
{
    if (m_form->properties & PR_CODE)
    {
        int pos = data.Find('\n');
        if (pos != -1)          // 개행 포함 = 카테고리+코드목록 특수 포맷
        {
            m_category = data.Left(pos);
            ...
            return;              // 여기서 조기 리턴
        }
        // pos == -1 (평범한 코드 한 개 쓰기)이면 여기서 안 걸리고 아래로 흘러감!
    }
    // 일반 필드와 동일한 공통 경로 (makeData, TrimRight 등)
    ...
}
```

**즉 "이 필드에 종목코드 하나를 쓰는" 가장 흔한 경우는 이 특수분기를 타지 않고 모든 Edit 필드와 동일한 공통 경로로 흘러간다.** 기대했던 "PR_CODE 쓰기의 단일 후킹 지점"이 아니었음 — `WriteData`는 컨트롤 타입별로 19개 파일(`dll/form/fm*.cpp`)에 흩어진 가상함수라 이 자체가 만능 후킹 지점이 되기 어렵다.

---

## 미해결 질문 → 답변 완료 (2026-07-20)

1. **`IsChanged()`가 공통 훅이 될 수 있나?** → **아니오.** `fmEdit.cpp:1894`의 `IsChanged()`는 `m_changed` 플래그를 읽고 리셋하는 getter일 뿐, 이벤트/콜백이 아님. 후킹 지점으로 못 씀.
2. **`CClient::OnDomino`는 언제 호출되나?** → `Guard.cpp:4029-4032` `CGuard::AtView(force=true)` → 새 화면을 도미노 코드와 함께 열 때만 호출(예: 관심종목 더블클릭 → 상세화면 열기). 화면 전환 시점 전용, 빈도 낮음.
3. **사용자 직접 타이핑이 `WriteData`를 거치나?** → **아니오.** `fmEdit.cpp`엔 `OnChar`/`OnKeyDown`이 없고, 키 입력은 `CfmEdit::UpdateData(int key, ...)`(fmEdit.cpp:1384)가 `m_strR` 버퍼를 직접 조작. `WriteData`와 별개 경로.
4. **TR 응답이 코드 필드를 건드릴 수 있나?** → **네.** `Stream.cpp`(`SetDataOOP` 등)엔 코드필드 관련 특수 분기가 없어서 일반 필드와 동일하게 `form->WriteData()` 호출.

**종합:** 코드 변경 경로가 최소 4갈래(직접 타이핑=`UpdateData` / 도미노=`WriteData` / TR응답=`WriteData` / 스크립트 SetData=`WriteData`)이고 방식도 제각각이라 개별 후킹은 비현실적 — 자가갱신(self-heal on read) 채택이 맞는 방향이었음이 재확인됨.

**단, 자가갱신 방식 자체의 구조적 한계 발견 (2026-07-20):** 자가갱신은 `CScreen::OnAlert`가 매 틱마다 호출되는 것에 편승하는 방식인데, 이는 **현재 shadow 모드(전체순회 유지)에서만 안전**하다. 실제 컷오버(전체순회 → 인덱스 기반 라우팅) 이후에는, 화면이 코드를 변경했는데 그 즉시 인덱스에 반영 안 된 상태에서 예전 코드의 틱이 더 이상 안 오면 그 화면은 영영 재인덱싱 안 될 수 있음 (커버리지 gap, 무한 지연 가능).

**대안으로 채택/적용한 것: Attach/Detach 훅 (2026-07-20 구현 완료)**
- 자가갱신을 대체하는 게 아니라 **보강** — "화면 열기/닫기" 케이스는 attach/detach로 지연 없이 확정 처리하고, "화면은 열려있는데 코드값만 바뀌는" 케이스는 여전히 자가갱신에 의존.
- **Attach** — `CScreen::Parse()`, `FA_FLASH` 등록 블록 안(Screen.cpp:390-404)에 추가. `OnAlert`와 동일한 kind 분류(`FM_GRID`/`FM_TABLE`/`FM_CONTROL`이 아닌 나머지)로 판별해서 화면 로드 시점에 바로 `UpdateCodeIndex(this, "", 코드)` 호출.
- **Detach** — `~CScreen()` 소멸자 맨 앞(Screen.cpp:77-81)에 `UpdateCodeIndex(this, m_lastCode, "")` 추가. 이걸로 화면이 닫혀도 `m_codeIndex`에 댕글링 `CScreen*` 포인터가 안 남게 됨 (수정 전엔 detach 호출이 전무해서, 나중에 인덱스를 실제 라우팅에 쓰기 시작하면 use-after-free 크래시로 이어질 수 있는 잠재 위험이었음 — `m_codeIndex` 자체가 이번 조사 중 새로 추가된 구조라 이 위험도 이번에 같이 생겼다가 같이 없어진 것).
- 둘 다 `#ifdef DF_RTM_INDEX` 안에서만 컴파일되어 실제 배포 동작(전체순회)엔 영향 없음.
- 실측 검증 완료: 화면 오픈 시 `[ATTACH] mapN=IB999987 name=1021 kind=9 code=` 로그로 attach 정상 동작 확인.

**남은 잔여 리스크(경미, 당장 조치 불필요):** Attach는 `Parse()` 시점(TR 조회 응답 도착 전)에 실행되므로 최초 등록 시 코드값이 비어있을 수 있음(위 로그의 `code=` 공란). TR 응답 도착 후 다음 실시간 틱이 오면 기존 자가갱신이 채워줌 — shadow 모드(전체순회 유지)에서는 이 다음 틱이 반드시 오므로 문제 없음.

**추가 수정 (2026-07-20) — `m_lastCode` 단일변수 → 필드별 추적으로 변경:** 화면 하나에 `FA_FLASH` 기본분기 필드가 여러 개(예: 관심종목처럼 여러 종목을 동시에 추적하는 화면) 있을 수 있다는 점을 고려해, `CString m_lastCode`(화면당 1개) 대신 `CMap<WORD, WORD, CString, CString&> m_lastCodes`(필드의 `iorder`를 키로, 필드별로 마지막 관측값 추적)로 변경함. 이걸 안 하면: 자가갱신이 서로 다른 필드의 값을 뒤섞어 비교하게 되고, Detach 시 마지막으로 처리된 필드의 코드만 인덱스에서 빠지고 나머지는 댕글링으로 남게 됨. Attach/Detach/자가갱신 3곳 모두 `m_lastCodes`(필드별 키) 기준으로 통일함. (`Wizard/Screen.h:126`, `Screen.cpp:77-89`(Detach), `398-413`(Attach), `835-847`(자가갱신))

**부수 수정 — Attach의 kind 분류에서 `FM_EDIT` 제외 로직 제거:** 테스트 중 한때 `case FM_EDIT:`을 제외 목록에 추가했었는데, `OnAlert`의 실제 매칭 로직은 `FM_EDIT`을 배제하지 않으므로(GRID/TABLE/CONTROL만 명시적으로 분리) 이 예외가 있으면 다른 맵에서 진짜 키 필드가 `FM_EDIT` 타입일 경우 Attach가 누락될 위험이 있었음. `OnAlert`와 완전히 동일한 분류 기준으로 되돌림.

---

## 관련 파일 (이번 조사에서 읽은 곳)

| 파일 | 관련 내용 |
|---|---|
| `Wizard/Screen.cpp:755-804` | `OnAlert` — 읽기측 매칭 로직 |
| `Wizard/Screen.cpp:198-454` | `Parse()` — `m_flashObs` 등록 (화면 로드 시점) |
| `Wizard/Guard.cpp:5898-5982` | `DoRTM` — 개선 대상, 전체순회 지점 |
| `Wizard/Guard.cpp:2305-2334` | `SetCode(CScreen*, ...)` — `PR_CODE` 필드 식별 예시 (실시간 매칭과는 무관한 것으로 판명, 참고용) |
| `Wizard/Client.cpp:477` | `OnDomino` — 도미노 트리거 진입점 |
| `Wizard/Client.cpp:1741-1878` | `RestoreDomino`/`CodeDomino` — 도미노 실제 쓰기 |
| `dll/form/fmEdit.cpp:1261-1296` | `CfmEdit::WriteData`의 `PR_CODE` 분기 (일반 케이스는 미해당) |
| `dll/form/fmBase.h:215,222` | `WriteData`/`IsChanged` 가상함수 선언 |
| `builder/h/form.h:282,361` | `FA_FLASH`("flash key field") vs `PR_CODE`("has code list") 정의 — 실시간 키 식별은 전자 |
| `Wizard/Screen.cpp:77-81` | `~CScreen()` — RTM 인덱스 Detach 훅 (2026-07-20 추가) |
| `Wizard/Screen.cpp:390-404` | `Parse()` FA_FLASH 등록 블록 — RTM 인덱스 Attach 훅 (2026-07-20 추가) |
| `Wizard/Guard.h:129`, `Guard.cpp:6001-6030` | `m_codeIndex`, `UpdateCodeIndex()` — 인덱스 자료구조/유지 함수 |

## 3단계 설계: 실제 컷오버 (2026-07-20 설계, 미구현)

**전제:** 1~2단계(shadow 검증 + attach/detach 보강)가 끝났으니, 다음은 `DoRTM`이 전체순회 대신 `m_codeIndex`를 실제로 써서 화면 방문 범위를 줄이는 것. 이건 운영 트레이딩 화면에 직접 영향을 주는 변경이라, 실시간 데이터로 검증 가능한 시점(장중)에 신중하게 넣어야 함.

### 설계 원칙 — 매칭 로직은 그대로, "몇 개 화면을 부르느냐"만 바꾼다

`CScreen::OnAlert` 내부의 필드별 매칭 로직(`m_flashObs` 순회, `text.Compare(code)`, `UpdateRTM(key+1, ...)` 호출)은 **손대지 않는다.** 바뀌는 건 오직 "`OnAlert`를 몇 개의 화면에 대해 호출하느냐" 뿐:

- **지금:** `CClient::OnAlert`(Client.cpp:1496)가 그 클라이언트에 열린 **모든** `isFlash()` 화면에 대해 `OnAlert` 호출
- **컷오버 후:** `m_codeIndex[code]`에 등록된 화면들에 대해서만 `OnAlert` 호출

### 변경 지점 후보

`CClient::OnAlert`가 클라이언트별로 화면을 순회하면서 스크립트 훅(`m_vm->OnProcedure`, `m_vm->OnAlert`)도 같이 호출하고 있어서, 단순히 `DoRTM`에서 인덱스 조회 결과로 직접 화면을 호출하면 이 스크립트 훅 호출 경로가 빠질 위험이 있음. 두 가지 방식 검토 필요:

1. `CClient::OnAlert`는 그대로 두되, 그 안의 화면순회 루프만 "그 클라이언트 소속이면서 `m_codeIndex[code]`에도 있는 화면"으로 제한
2. `DoRTM`이 `m_codeIndex[code]`로 화면 목록을 직접 얻고, 각 화면의 `m_client`를 통해 스크립트 훅까지 동일하게 호출하도록 재구성

**중복 호출 주의:** 화면 하나가 같은 코드에 대해 `m_flashObs` 안에 필드를 2개 이상 갖고 있어도(이론상 드물지만 배제 못 함) `m_codeIndex[code]` 배열엔 그 화면이 여러 번 들어갈 수 있음(현재 `UpdateCodeIndex`는 중복 방지 안 함). 같은 화면에 `OnAlert`를 두 번 부르면 틀린 결과가 나오진 않지만(멱등) 낭비이므로, 컷오버 시점에 화면 포인터 기준 중복 제거를 추가하는 게 좋음.

### 안전한 롤아웃 순서 (제안)

1. **화면 단위 shadow 검증 추가** — **완료 (2026-07-23)**, 아래 상세 참고.
2. **런타임 킬스위치 확보** — **완료 (2026-07-23)**, 아래 상세 참고. 레지스트리 방식 채택.
3. **컷오버 적용 + 병행 기간** — 위 킬스위치를 켠 상태로 실제 배포하되, 초기엔 "인덱스로 걸러진 화면 목록"과 "전체순회로 찾은 화면 목록"을 여전히 둘 다 계산해서 다르면 경고 로그를 남기는 이중 모드로 일정 기간(예: 1~2주 장중) 운영.
4. **전체순회 제거** — 병행 기간 동안 불일치가 없었으면 전체순회 코드/이중계산 제거, 인덱스 단독 운영으로 전환.

### ①단계 실행 결과 (2026-07-23)

**포인터 로그 추가로 "IB999919가 매 틱 두 번 방문되는" 의문 해소:** `visit`(Client.cpp:1510)과 `compare`(Screen.cpp:831) 로그에 `screen=%p`를 추가해서 확인한 결과, 두 번의 방문이 서로 **다른 `CScreen*` 포인터**였음을 확인 — 한 화면이 flash 필드 2개를 가진 게 아니라, **대표화면(마스터 화면) 안에 같은 맵(`IB999919`)이 임베디드 형태로 2개 들어있는 구조**였다(사용자 확인). `m_codeIndex`/`m_lastCodes` 둘 다 `CScreen*` 인스턴스 단위로 동작하므로 이 케이스에 별도 설계 변경 불필요 — 기존 설계가 이미 정확히 처리하고 있음이 실측으로 확인됨.

**`[MISMATCH]` 자동 검증 로그 추가:** `CGuard::IsCodeIndexed(code, screen)`(Guard.h 선언, Guard.cpp `UpdateCodeIndex` 바로 아래 구현)를 추가하고, `CScreen::OnAlert`의 default 분기에서 `text.Compare(code)`가 일치(`flash=true`)하는 바로 그 순간 이 화면이 `m_codeIndex[code]`에도 들어있는지 확인해서, **없으면**(=매칭됐는데 인덱스가 놓친 경우, 컷오버 시 데이터 누락으로 이어질 유일한 위험 케이스) `[WIZARD][RTM][DEBUG][MISMATCH]`를 찍도록 함. 인덱스에 화면이 여분으로 더 들어있는 반대 방향(매칭 안 되는데 인덱스엔 있는 경우)은 성능 손해만 있고 데이터 누락은 없어 의도적으로 검사 대상에서 제외.

```cpp
// Guard.h (UpdateCodeIndex 선언 옆)
bool	IsCodeIndexed(CString code, class CScreen* screen);

// Guard.cpp (UpdateCodeIndex 정의 바로 아래)
bool CGuard::IsCodeIndexed(CString code, CScreen* screen)
{
	void*	ptr;
	if (!m_codeIndex.Lookup(code, ptr))
		return false;
	CPtrArray* arr = (CPtrArray*)ptr;
	for (int ii = 0; ii < arr->GetSize(); ii++)
		if (arr->GetAt(ii) == screen)
			return true;
	return false;
}

// Screen.cpp, CScreen::OnAlert default 분기, flash = true; 직후
#ifdef DF_RTM_INDEX
if (!m_guard->IsCodeIndexed(code, this))
{
	CString dbgMiss;
	dbgMiss.Format("[WIZARD][RTM][DEBUG][MISMATCH] mapN=%s screen=%p code=%s matched but NOT in index!\n",
		CString(m_mapH->mapN, L_MAPN).GetString(), this, code.GetString());
	OutputDebugString(dbgMiss);
}
#endif
```

**실측 결과 (2026-07-23, 장중):** 주문(매도/매수), 통합현재가, 실시간잔고, 코스피지수/삼성전자 현재가, 관심종목(전종목/코스피/코스닥/코스피200/KRX100 탭 다수) 등 화면을 다수 동시에 띄운 상태로 장시간 관찰 — **`[MISMATCH]` 0건.**

**참고:** 관심종목/차트처럼 `FM_GRID`/`FM_TABLE` 기반으로 종목을 다수 표시하는 화면은 애초에 `m_codeIndex`의 검증 대상이 아님 — `OnAlert`의 `FM_GRID`/`FM_TABLE` 분기는 `FlashGrid`/`FlashSemi`로 별도 처리되고, attach 시점 코드도 이 kind들은 명시적으로 skip한다(단일 "코드 필드"가 아니라 여러 종목을 동시에 표시하는 구조라 이번 역인덱스 설계 범위 밖). 사용자가 "관심종목/차트는 구조가 다르지만 육안상 as-is/to-be 동일해 보인다"고 확인한 것은 이 경로가 이번 shadow 작업으로 전혀 건드려지지 않았다는 뜻으로, 정상.

**결론:** ①단계(화면 단위 shadow 검증) 완료로 판단.

### ②단계 실행 결과 (2026-07-23) — 런타임 킬스위치(레지스트리)

기존 `WORKSTATION` 섹션 레지스트리 관례(`m_app->GetProfileInt(WORKSTATION, KEY, default)`, 실제 경로는 `HKCU\Software\IBK투자증권MAC\AXIS Workstation V04.00\Workstation`)를 그대로 따라서 새 키 하나 추가:

```cpp
// h/axisvar.h, WORKSTATION SECTION 블록
#define	RTMIDXCUT	"RTMIndexCutover"	// dword
						// 0 = full traversal (default, safe) / 1 = use m_codeIndex for RTM dispatch

// Guard.h, DF_RTM_INDEX 블록 (m_codeIndex 옆)
bool	m_rtmIndexCutover = false;	// runtime kill switch (registry: Workstation\RTMIndexCutover), read once at Startup()

// Guard.cpp, CGuard::Startup() (m_wait 읽는 곳 옆)
#ifdef DF_RTM_INDEX
	m_rtmIndexCutover = m_app->GetProfileInt(WORKSTATION, RTMIDXCUT, 0) ? true : false;
	CString dbgCut;
	dbgCut.Format("[WIZARD][RTM][DEBUG] RTMIndexCutover=%d (registry-read)\n", m_rtmIndexCutover);
	OutputDebugString(dbgCut);
#endif
```

프로그램 재시작 시점에만 반영(라이브 리로드 아님) — 지금 단계에서 충분하다고 판단, 나중에 HTS 설정 화면으로 옮길 때 다시 검토.

**실측:** 값을 DWORD가 아니라 QWORD(64비트)로 잘못 만들어서 처음엔 0으로 읽힘 → DWORD(32비트)로 재생성 후 정상적으로 `RTMIndexCutover=1` 읽히는 것 확인.

**주의 — 이 변수는 아직 아무 실제 동작도 바꾸지 않음.** ③단계(실제 컷오버 로직)를 구현할 때 `if (m_guard->m_rtmIndexCutover) { 인덱스경로 } else { 기존 전체순회 }`로 감싸는 스위치로 쓸 예정. 지금은 레지스트리 읽기 인프라만 준비되고 검증된 상태.

### 미결 사항 (③단계 구현 전 반드시 확정 필요)

- **[신규, 최우선] 자가갱신의 구조적 한계가 아직 안 닫혀 있음** — `CScreen::OnAlert`의 자가갱신은 "그 화면에 틱이 실제로 와야만" 인덱스를 갱신한다. 화면이 이미 열려있는 상태에서 코드값이 바뀌는 경로(직접입력=`CfmEdit::UpdateData`, 도미노=`WriteData`, TR응답=`WriteData`, 스크립트 SetData=`WriteData`, 3절 참고)는 전부 자가갱신과 무관하게 동작하므로, 코드가 바뀐 "새 코드"의 틱이 온다는 보장이 없다(그 새 코드를 아무도 안 보내면 자가갱신 자체가 트리거 안 됨). Attach/Detach는 "화면 열기/닫기"만 커버하지 "열린 채로 코드만 바뀌는" 케이스는 못 막는다. 지금은(전체순회 유지 중) 이게 안전한데, ③단계에서 실제로 인덱스 밖 화면의 `OnAlert` 호출을 skip하기 시작하면 이 화면은 새 코드에 대해 영영 인덱싱이 안 될 수 있다.
  - 완화 옵션 (택1 또는 조합, ③단계 착수 전 결정 필요):
    a. **주기적 재동기화(safety net)** — 예: N초마다 한 번씩은 skip 없이 전체순회를 강제로 한 번 돌려서 인덱스를 바로잡음. 컷오버의 성능이득은 유지하면서 "영영 안 고쳐짐"의 최대 지연시간을 N초로 제한.
    b. **코드 변경 시점 훅 추가** — `CfmEdit::UpdateData`/도미노/TR응답 경로에 직접 `UpdateCodeIndex` 호출 추가 (2026-07-20 조사 당시엔 경로가 여러 갈래라 개별 후킹이 비현실적이라 판단해 자가갱신으로 대체했음 — 재검토 여지 있음)
    c. **잔여 리스크 수용** — 실거래에서 "화면 열어둔 채로 다른 종목으로 바꾸는" 빈도/영향을 실측해서 허용 가능하면 그대로 진행
- 스크립트 훅(`m_vm->OnProcedure`/`OnAlert`) 호출을 컷오버 후에도 정확히 같은 타이밍/횟수로 보장하는 방법
- `m_codeIndex` 배열의 중복 등록 방지 여부(어차피 멱등이라 필수는 아니지만 성능상 정리하는 게 나음)

---

## 관련 문서

- `@docs/WizardArchitecture.md` — 전체 아키텍처
- `@docs/KnowledgeBase.md` 11절 — RTM 흐름 최초 기록, 12절 — 이 조사의 발단이 된 문서-코드 드리프트
