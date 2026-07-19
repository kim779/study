# 실시간 시세(RTM) 종목코드 역인덱스 도입 조사

## 배경 / 목표

`CGuard::DoRTM`(Guard.cpp:5898~5982)이 실시간 틱 1건마다 열려있는 **모든 화면**(`m_clients` 전체)을 순회하며 "이 화면이 이 종목에 관심있나"를 검사한다 (`@docs/KnowledgeBase.md` 참고 — 2026-07-19 발견). 화면을 많이 띄운 상태에서 틱이 몰릴 때 UI가 버벅이는 원인으로 추정됨.

**개선 방향:** 종목코드 → 관심 화면 역인덱스를 `CGuard`에 두고, 틱마다 전체순회 대신 인덱스 조회로 대체.

**관건:** 화면의 "종목코드 필드" 값이 바뀌는 모든 지점에서 인덱스를 갱신해야 함 — 누락 시 "시세가 조용히 안 뜨는" 회귀 발생 (성능 문제보다 훨씬 위험).

**목표 일정:** 2026-07-22 저녁까지 실제 소스 적용 (사용자가 그 전까지 직접 소스 리뷰 + 디버깅 시간 가짐).

---

## 확인된 사실

### 1. "종목코드 필드"의 식별 방법 — `PR_CODE` 속성 플래그

화면(.map)의 필드 중 `FM_EDIT` 타입이고 `form->m_form->properties & PR_CODE`가 설정된 필드가 "이 화면의 종목코드 필드"다. 이건 **맵소스 설계 시점(빌더)에 지정되는 플래그**이지, 필드 이름 관례가 아니다.

근거: `CGuard::SetCode(CScreen* screen, ...)` (Guard.cpp:2305)
```cpp
for (int ii = 0; ii < screen->m_mapH->formN; ii++)
{
    form = screen->GetAtForm(ii);
    if (!form || form->m_form->kind != FM_EDIT)
        continue;
    if (form->m_form->properties & PR_CODE && self)
    {
        form->ReadData(text);
        SetCode((char*)form->m_form->name, text, self, ...);
        ...
    }
}
```

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

## 미해결 질문 (다음 조사 대상)

1. **`fmEdit::WriteData`의 일반 경로(1298줄 이후) 끝에서 `IsChanged()`나 다른 공통 콜백이 불리는지** — 있다면 그게 더 나은 후킹 후보. `fmBase.h:222`의 `virtual bool IsChanged(bool reset=true) { return true; }`가 서브클래스에서 어떻게 오버라이드되는지 아직 안 봄.
2. **`CClient::OnDomino`가 실제로 언제 호출되는지** — 포커스 전환마다? 코드 입력 엔터 시? 빈도가 얼마나 되는지가 인덱스 갱신 비용 산정에 중요.
3. **사용자가 키보드로 직접 코드를 입력하는 경로**(`fmEdit.cpp`의 `OnChar`/`OnKeyDown` 계열)가 최종적으로 `WriteData`를 거치는지, 아니면 별도 경로로 필드 버퍼를 직접 건드리는지.
4. **TR 응답으로 필드가 채워지는 경로**(`Stream.cpp`)가 `PR_CODE` 필드를 건드릴 수 있는지 — 있다면 이것도 4번째 쓰기 경로.
5. **대안**: 매 write 지점을 다 찾아 후킹하는 대신, `CScreen::OnAlert`가 어차피 매 틱마다 `PR_CODE` 필드를 `ReadData`로 읽는다는 점을 이용해 — **"인덱스를 실시간으로 100% 정확하게 유지"하는 대신 "일정 주기로(예: 코드 변경 감지 시 지연 없이, 혹은 도미노/도착 이벤트 시점마다) 인덱스를 갱신"하는 절충안**도 고려 가치 있음. 완벽한 이벤트 기반 갱신이 너무 위험하면, 첫 구현은 "화면 Attach/Detach 시점 + Domino 발생 시점"만 잡고 시작하는 것도 방법.

---

## 관련 파일 (이번 조사에서 읽은 곳)

| 파일 | 관련 내용 |
|---|---|
| `Wizard/Screen.cpp:755-804` | `OnAlert` — 읽기측 매칭 로직 |
| `Wizard/Screen.cpp:198-454` | `Parse()` — `m_flashObs` 등록 (화면 로드 시점) |
| `Wizard/Guard.cpp:5898-5982` | `DoRTM` — 개선 대상, 전체순회 지점 |
| `Wizard/Guard.cpp:2305-2334` | `SetCode(CScreen*, ...)` — `PR_CODE` 필드 식별 예시 |
| `Wizard/Client.cpp:477` | `OnDomino` — 도미노 트리거 진입점 |
| `Wizard/Client.cpp:1741-1878` | `RestoreDomino`/`CodeDomino` — 도미노 실제 쓰기 |
| `dll/form/fmEdit.cpp:1261-1296` | `CfmEdit::WriteData`의 `PR_CODE` 분기 (일반 케이스는 미해당) |
| `dll/form/fmBase.h:215,222` | `WriteData`/`IsChanged` 가상함수 선언 |

## 관련 문서

- `@docs/WizardArchitecture.md` — 전체 아키텍처
- `@docs/KnowledgeBase.md` 11절 — RTM 흐름 최초 기록, 12절 — 이 조사의 발단이 된 문서-코드 드리프트
