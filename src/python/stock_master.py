"""
종목코드마스터(hjcode3.dat/upcode.dat)와 테마/그룹사(THEMCD.INI/GROPCD.INI)를 읽어
업종/테마/그룹사별 종목 카테고리를 만든다. StockPickerDialog가 사용.

구조체 레이아웃과 분류 조건은 D:\\src\\IBKS\\src\\H\\jmcode.h(hjcodex)와
D:\\src\\IBKS\\src\\DLL\\AXISCODE-운용\\JongmukDlg.cpp(SetTree() 등)에서 확인한
실제 코드 기준이다.
"""
import os
import struct

TAB_DIR = r"C:\IBK_OPENAPI\tab"

_HJCODEX_FIELDS = [
    "code", "hnam", "enam", "ecng", "jsiz", "symb", "size", "jjug",
    "kpgb", "kosd", "ssgb", "ucmd", "itgb", "wsgb", "jqty", "star",
    "unio", "jchk", "elwf", "sjjs", "kqpg", "uc10", "uc20",
    "sec1", "sec2", "sec3", "sec4", "sec5", "acdl", "acdm", "acds",
    "nxtf", "fill",
]
_HJCODEX_FMT = "<12s40s40s1s1s7s1s1s1s1s1s1s1s1s1s1s1s1s1s10s1s1s1s1s1s1s1s1s6s6s6s1s19s"
_HJCODEX_SIZE = struct.calcsize(_HJCODEX_FMT)  # 170바이트, 실 파일 크기로 검증됨

KOSPI, KOSDAQ = 0, 10  # hjcodex.kosd (jmcode.h jmKOSPI/jmKOSDAQ)

# jmcode.h의 AS-IS/TO-BE 업종코드 주석표와 JongmukDlg.cpp SetTree()의 acdm/acds 비교
# case문을 함께 참고해 만든 코드->라벨. 실제 hjcode3.dat 데이터로 acdm 값을 뽑아보면
# 코스닥은 acds 수준(QGS09P~QGS30P 등)의 세분류 코드가 acdm에도 그대로 쓰이고 있어
# 두 표를 합쳐서 정리했다. 매핑에 없는 코드는 원본 코드를 그대로 라벨로 보여준다.
SECTOR_LABELS = {
    # 코스피
    "KGS01P": "음식료품", "KGS02P": "섬유,의복", "KGS03P": "종이,목재",
    "KGS04P": "화학", "KGS05P": "의약품", "KGS06P": "비금속광물",
    "KGS07P": "철강및금속", "KGS08P": "기계", "KGS09P": "전기,전자",
    "KGS19P": "의료정밀", "KGS10P": "운수장비", "KGS11P": "유통업",
    "KGS20P": "전기가스업", "KGS12P": "건설업", "KGS13P": "운수창고",
    "KGS21P": "통신업", "KGS14P": "금융업", "KGS15P": "은행",
    "KGS16P": "증권", "KGS17P": "보험", "KGS22P": "서비스업",
    "KGS18P": "제조업",
    # 코스닥
    "QGG01P": "코스닥", "QGZ01P": "코스닥 대형주", "QGZ02P": "코스닥 중형주",
    "QGZ03P": "코스닥 소형주", "QGS35P": "중소기업지수", "QGS36P": "제조",
    "QGS37P": "유통서비스", "QGS38P": "건설", "QGS39P": "금융",
    "QGS01P": "기타서비스", "QGS32P": "코스닥 IT", "QGS42P": "오락,문화",
    "QGS02P": "제조", "QGS03P": "건설", "QGS04P": "유통",
    "QGS31P": "운송", "QGS05P": "금융", "QGS49P": "사업서비스",
    "QGS34P": "오락,문화", "QGS06P": "통신방송서비스", "QGS07P": "IT S/W서비스",
    "QGS08P": "IT H/W", "QGS09P": "음식료,담배", "QGS10P": "섬유,의류",
    "QGS11P": "종이,목재", "QGS12P": "출판,매체복제", "QGS13P": "화학",
    "QGS33P": "제약", "QGS14P": "비금속", "QGS15P": "금속",
    "QGS16P": "기계,장비", "QGS17P": "일반전기전자", "QGS18P": "의료,정밀기기",
    "QGS19P": "운송장비,부품", "QGS20P": "기타 제조", "QGS65P": "종합건설",
    "QGS66P": "전문건설", "QGS68P": "도매", "QGS75P": "금융",
    "QGS81P": "전문기술", "QGS87P": "오락,문화", "QGS21P": "통신서비스",
    "QGS22P": "방송서비스", "QGS23P": "인터넷", "QGS24P": "디지털컨텐츠",
    "QGS25P": "소프트웨어", "QGS26P": "컴퓨터서비스", "QGS27P": "통신장비",
    "QGS28P": "정보기기", "QGS29P": "반도체", "QGS30P": "IT부품",
    "QGG03P": "코스닥 우량기업부", "QGG04P": "코스닥 벤처기업부",
    "QGG05P": "코스닥 중견기업부", "QGG06P": "코스닥 기술성장기업부",
}


def _cstr(raw):
    return raw.split(b"\x00")[0].decode("cp949", errors="ignore").strip()


def load_stocks(tab_dir=TAB_DIR):
    # hjcode3.dat: 헤더 없이 hjcodex(170바이트) 구조체가 그대로 나열된 파일.
    path = os.path.join(tab_dir, "hjcode3.dat")
    with open(path, "rb") as f:
        data = f.read()
    n = len(data) // _HJCODEX_SIZE
    stocks = []
    for i in range(n):
        rec = dict(zip(_HJCODEX_FIELDS, struct.unpack(
            _HJCODEX_FMT, data[i * _HJCODEX_SIZE:(i + 1) * _HJCODEX_SIZE])))
        code = _cstr(rec["code"])
        if code.startswith("A"):
            code = code[1:]
        if not code:
            continue
        stocks.append({
            "code": code,
            "name": _cstr(rec["hnam"]),
            "kosd": rec["kosd"][0],
            "kpgb": rec["kpgb"][0],
            "unio": rec["unio"][0],
            "size": rec["size"][0],
            "acdm": _cstr(rec["acdm"]),
            "ssgb": rec["ssgb"][0],  # 소속구분: 0=일반, 그외 ELW/ETF/ETN/신주인수권 등
        })
    return stocks


def _load_folder_ini(path, code_section):
    # THEMCD.INI/GROPCD.INI 공통 포맷:
    #   [FOLDER] 11=2차전지(LFP/리튬인산철)               ;503  (표시명;일련번호)
    #   [TMCODE 또는 GRCODE] 503=코드1;코드2;...;
    import configparser
    if not os.path.isfile(path):
        return {}
    cp = configparser.ConfigParser(strict=False)
    cp.read(path, encoding="cp949")
    result = {}
    if "FOLDER" not in cp or code_section not in cp:
        return result
    for value in cp["FOLDER"].values():
        if ";" not in value:
            continue
        name, folder_id = value.rsplit(";", 1)
        name = name.strip()
        folder_id = folder_id.strip()
        codes_raw = cp[code_section].get(folder_id, "")
        codes = [c for c in codes_raw.split(";") if c]
        if name and codes:
            result[name] = codes
    return result


def load_themes(tab_dir=TAB_DIR):
    return _load_folder_ini(os.path.join(tab_dir, "THEMCD.INI"), "TMCODE")


def load_groups(tab_dir=TAB_DIR):
    return _load_folder_ini(os.path.join(tab_dir, "GROPCD.INI"), "GRCODE")


def build_categories(tab_dir=TAB_DIR, stocks=None):
    # StockPickerDialog 트리에 채울 카테고리: {카테고리명: {하위명: [(코드, 종목명), ...]}}
    if stocks is None:
        stocks = load_stocks(tab_dir)
    by_code = {s["code"]: s["name"] for s in stocks}

    def pairs(codes):
        return [(c, by_code.get(c, "")) for c in codes]

    tree = {}
    tree["전종목"] = {"전체": pairs([s["code"] for s in stocks])}

    kospi = [s for s in stocks if s["kosd"] == KOSPI]
    tree["코스피"] = {
        "종합": pairs([s["code"] for s in kospi]),
        "대형주": pairs([s["code"] for s in kospi if s["size"] == 2]),
        "중형주": pairs([s["code"] for s in kospi if s["size"] == 3]),
        "소형주": pairs([s["code"] for s in kospi if s["size"] == 4]),
    }

    kosdaq = [s for s in stocks if s["kosd"] == KOSDAQ]
    tree["코스닥"] = {
        "종합": pairs([s["code"] for s in kosdaq]),
        "대형주": pairs([s["code"] for s in kosdaq if s["size"] == 2]),
        "중형주": pairs([s["code"] for s in kosdaq if s["size"] == 3]),
        "소형주": pairs([s["code"] for s in kosdaq if s["size"] == 4]),
    }

    # 코스피200: kpgb in (1,2,3) / 코스피100: (2,3) / 코스피50: (3) - 누적 포함 관계
    tree["코스피200"] = {"전체": pairs([s["code"] for s in kospi if s["kpgb"] in (1, 2, 3)])}
    tree["KRX100"] = {"전체": pairs([s["code"] for s in stocks if s["unio"] == 1])}

    def sector_group(pool):
        groups = {}
        for s in pool:
            if not s["acdm"]:
                continue
            label = SECTOR_LABELS.get(s["acdm"], s["acdm"])
            groups.setdefault(label, []).append(s["code"])
        return {label: pairs(codes) for label, codes in groups.items()}

    tree["코스피업종"] = sector_group(kospi)
    tree["코스닥업종"] = sector_group(kosdaq)

    tree["테마"] = {name: pairs(codes) for name, codes in load_themes(tab_dir).items()}
    tree["그룹사"] = {name: pairs(codes) for name, codes in load_groups(tab_dir).items()}

    return tree


# "전종목/코스피/코스닥/코스피200/KRX100"은 너무 넓어서 "관련종목" 후보로는 의미가 없어 제외
_BROAD_CATEGORIES = {"전종목", "코스피", "코스닥", "코스피200", "KRX100"}


def find_related(code, categories=None, tab_dir=TAB_DIR):
    # 종목코드 하나가 속한 업종/테마/그룹사 카테고리만 뽑아 반환한다(build_categories와 같은 형태).
    # 예: {"코스피업종": {"운수장비": [...]}, "테마": {"조선": [...]}}
    if categories is None:
        categories = build_categories(tab_dir)
    result = {}
    for cat_name, sub in categories.items():
        if cat_name in _BROAD_CATEGORIES:
            continue
        for sub_name, pairs in sub.items():
            if any(c == code for c, _ in pairs):
                result.setdefault(cat_name, {})[sub_name] = pairs
    return result


if __name__ == "__main__":
    cats = build_categories()
    for cat, sub in cats.items():
        print(f"[{cat}]")
        for name, pairs in sub.items():
            print(f"  {name}: {len(pairs)}종목")
