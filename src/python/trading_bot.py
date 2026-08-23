import sys
import os
import shutil
import subprocess
import ctypes
import winreg
import time
import socket
import sqlite3
import re
from PyQt5.QAxContainer import QAxWidget
from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QGroupBox, QPushButton, QLabel, QLineEdit, QTextEdit, QFormLayout,
    QComboBox, QGridLayout, QMessageBox, QTableWidget, QTableWidgetItem,
    QHeaderView, QAbstractItemView, QDateTimeEdit, QCheckBox, QScrollArea,
    QTabWidget, QDialog, QTreeWidget, QTreeWidgetItem, QDateEdit
)
from PyQt5.QtCore import QDateTime, QDate, QTime, QTimer, Qt
from datetime import datetime
import matplotlib
matplotlib.rcParams['font.family'] = 'Malgun Gothic'
matplotlib.rcParams['axes.unicode_minus'] = False  # 한글 폰트 사용 시 마이너스 기호 깨짐 방지
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg
from matplotlib.figure import Figure
import matplotlib.dates
from backtest_ma_cross import sma_series, find_trades
from backtest_daily import (
    load_daily, sma as daily_sma, cross_signals,
    simulate as daily_simulate, today_signal as daily_today_signal
)

OCX_GUID = "{CDADD338-C7AB-4977-B65D-8E988B5958E3}"

# IBKSConnector.ocx가 내부적으로 셀프등록하는 의존 컨트롤들 (AxisUtil.cpp의 OCX[] 배열과 동일)
# 등록경로가 엉뚱한 폴더로 바뀌는 문제를 여러 번 겪어서, 초기화 시 실제 로드경로를 눈으로 바로 확인하기 위함.
DEPENDENT_OCX_PROGIDS = {
    "AxisWizard": "AxisWizard.WizardCtrl.IBK2019",
    "AxisSock":   "AxisSock.SockCtrl.IBK2019",
    "AxisCertify": "AxisCertify.CertifyCtrl.IBK2019",
    "AxisXecure": "AxisXecure.XecureCtrl.IBK2019",
}

# 접속서버 목록 (IP, 이름) - 포트는 UI의 svr_port 값 공용 사용
SERVER_LIST = [
    ("211.255.204.104", "UAT"),
    ("211.255.204.70", "BP10"),
    ("211.255.204.71", "BP11"),
    ("211.255.204.72", "BP12"),
    ("211.255.204.73", "BP13"),
    ("211.255.204.74", "BP14"),
    ("211.255.204.75", "BP15"),
    ("211.255.204.76", "BP16"),
    ("211.255.204.77", "BP17"),
    ("211.255.204.78", "BP18"),
    ("211.255.204.79", "BP19"),
    ("211.255.204.33", "BP20"),
    ("211.255.204.34", "BP21"),
    ("211.255.204.35", "BP22"),
    ("211.255.204.36", "BP23"),
    ("211.255.204.37", "BP24"),
    ("211.255.204.38", "BP25"),
    ("211.255.204.39", "BP26"),
    ("211.255.204.57", "BP27"),
    ("211.255.204.58", "BP28"),
    ("211.255.204.59", "BP29"),
]

# TR key constants - must match Common.h exactly
TK_TR1001 = 1   # 주식 시세조회
TK_TR1002 = 2   # 주식 조회(OOP)
TK_TR1003 = 3   # 주식 조회(OOP)
TK_TR1004 = 6   # 주식 실시간 다중종목 등록
TK_TR1005 = 16  # 주식 일/주/월별 시세조회
TK_TR1006 = 18  # 주요 시장지표(지수/금리) 조회
TK_TR1007 = 21  # 차트(일/주/월봉 캔들) 조회
TK_TR1201 = 4   # 주식 주문
TK_TR1203 = 5   # 주식 주문 시장구분
TK_TR1211 = 7   # 주식 체결/미체결조회
TK_TR1221 = 8   # 주식 잔고조회
TK_TR1231 = 9   # 주식 주문옵션조회
TK_TR1801 = 10  # 지수관련 종목시세
TK_TR1802 = 11  # 지수관련 oop시세
TK_TR1803 = 12  # 지수관련 종목현황
TK_TR1804 = 13  # 지수관련 시세
TK_TR3001 = 14  # 선물옵션 시세조회
TK_TR3002 = 15  # 선물옵션 oop조회(OOP)
TK_TR3201 = 17  # 선물옵션 주문
TK_TR3211 = 19  # 선물옵션 체결/미체결조회
TK_TR3221 = 20  # 선물옵션 잔고조회
TK_TR3222 = 46  # 선물옵션 잔고조회(실시간)
TK_TR1222 = 45  # 주식 잔고조회(실시간)
TK_TR3232 = 47  # 선물옵션 미결제조회
TK_TR3411 = 48  # 옵션 만기일 관련 시세
TK_TR8001  = 50 # 고객정보조회
TK_TR1223  = 51 # 주식 잔고조회(시장구분)
TK_GETCODE = 32 # 종목코드 목록조회 (GetCode)
TK_GREEKS1 = 150 # 옵션 그릭스(민감도) 조회용 키1
TK_GREEKS2 = 151 # 옵션 그릭스(민감도) 조회용 키2

# 파이썬 TR명 -> OCX가 실제로 서버에 보내는 트랜잭션 코드(IBKSConnectorCtl.cpp의 SendTR/ledger->svcd 리터럴).
# TR4xxx 계열은 공통 게이트웨이(piboPBxQ)를 타고 이 코드는 페이로드 안쪽(ledger->svcd)에 실린다.
TR_WIRE_NAME = {
    "TR1001": "pibo1003", "TR1002": "pooppoop", "TR1003": "pooppoop",
    "TR1004": "pooppoop", "TR1005": "pooppoop", "TR1006": "pino2901",
    "TR1007": "GOOPHOOP", "TR1201": "pibosodr", "TR1203": "pibosodr",
    "TR1211": "piboschg", "TR1221": "pibosjgo", "TR1222": "pibosjgo",
    "TR1223": "pibosjg2", "TR1231": "SONAQ200", "TR1801": "pibo5101",
    "TR1802": "pibo5102", "TR1803": "pibo5103", "TR1804": "pibo5104",
    "TR2001": "bo292101", "TR3001": "pibo3002", "TR3002": "pooppoop",
    "TR3003": "pooppoop", "TR3201": "pibofodr", "TR3211": "pihofchg",
    "TR3221": "pibofjgo", "TR3222": "pibofjgo", "TR3231": "SONBQ101",
    "TR3232": "SSLBQ033", "TR3411": "PIBO4013", "TR4003": "SONBQ114",
    "TR4004": "SONBT709", "TR4101": "SACMT238", "TR4201": "SONBQ740",
    "TR4202": "SONBQ504", "TR4221": "SONBQ105", "TR4223": "SONDQ401",
    "TR4224": "SONBQ123", "TR4303": "SSLAQ826", "TR4401": "SDPBQ001",
    "TR4403": "SONDQ304", "TR4501": "SDPKT360", "TR4502": "SDPDT901",
    "TR4503": "SDPKT300", "TR4511": "SDPNQ002", "TR4512": "SDPBQ905",
    "TR4521": "SDPNQ060", "TR4522": "SDPNQ031", "TR4523": "SACMQ913",
    "TR4524": "SBPGT339", "TR4525": "SDPNQ180", "TR4526": "SACAQ519",
    "TR4527": "SACMQ229", "TR8001": "pihocust",
}

# TR1002(NXT/통합 시세조회) 요청 시 1777(장운영구분) 뒤에 붙이는 요청 필드번호 목록
# 순서 = 응답에서 값이 오는 순서와 동일하다고 가정 (실측 후 확정 필요)
TR1002_SISE_FIELDS = ["2023", "2033", "2029", "2030", "2031", "2024", "2027", "2041", "2061"]
TR1002_SISE_LABELS = ["현재가", "등락율", "시가", "고가", "저가", "전일대비", "거래량", "매도잔량", "매수잔량"]

# TR1005(종목 일/주/월별 시세) 요청 필드 목록 - AxisChaser 실측(Send_Rev.ini) 그대로
TR1005_DAILY_FIELDS = ["12021", "12302", "12029", "12030", "12031", "12023", "12024", "12033",
                        "12027", "12028", "12036", "12363", "12367", "12390", "12399", "12338",
                        "12119", "12118", "12037"]

# TR1006(주요 시장지표) 지수 코드 라벨. 지수 레코드는 87바이트 고정폭:
# 코드(5)+지수명+지수(023)+대비(024)+등락률(033)+거래량,천(027)+대금,백만(028)+거래종목수(256)+상승(252)+하락(255)
# 필드번호는 [2901] 화면 Grid Column Edit Dialog 실측으로 확인.
TR1006_INDEX_LABELS = {
    "K0001": "코스피(종합)",
    "K5001": "KRX100",
    "K2001": "코스피200",
    "ET181": "KTOP30",
    "KQ001": "코스닥(종합)",
    "KQ047": "코스닥150",
    "ST050": "코리아밸류업지수",
}
TR1006_INDEX_RE = re.compile(
    r'([A-Z][A-Z0-9]{4})([가-힣A-Za-z0-9]+)\s+([\d.]+)\s+(\d+\.\d+)([+-][\d.]+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)'
)
# 금리 구간: "CP(91일)"부터 TJ코드 시작 전까지, "구분명 금리[+-]?대비" 반복
TR1006_RATE_RE = re.compile(r'([가-힣A-Za-z0-9()]+?)\s+(\d+\.\d{2})([+-]?)\s*(\d+\.\d{2})')
# 기관/외국인 동향: TJ+8자리코드+순매수+매수+매도. 코드 앞4자리=시장(01코스피/02선물), 뒤4자리=투자자(0000기관/0900외국인)
TR1006_FLOW_RE = re.compile(r'TJ(\d{8})\s+([+-]{1,2}\d+)\s+(\d+)\s+(\d+)')
TR1006_FLOW_LABELS = {
    "01000001": "코스피 기관",
    "01090001": "코스피 외국인",
    "02000000": "선물 기관",
    "02090000": "선물 외국인",
}

# TR1005 필드 라벨 - [2011]일자별시세 실측값과 1:1 대조로 확인(2026-07-29, 005930 07-28행 기준).
# 12023(전일대비), 12036(외인보유)는 값 앞에 등락구분으로 보이는 숫자 한자리가 더 붙어 나옴(미해결).
# 12037은 대응하는 값을 못 찾음(레코드에 값 자체가 안 옴).
TR1005_DAILY_LABELS = ["일자", "시가", "고가", "저가", "종가", "전일대비", "등락률", "거래량",
                        "거래대금", "체결강도", "외인보유", "외인비중", "기관", "외국인", "신용비율",
                        "매수체결", "매도체결", "차이", "12037"]

# TR1007(차트/캔들 조회) 응답 캔들 레코드 필드. AxisChaser 실측(GOOPHOOP) 확인 완료.
# 레코드는 "일자\t시간(공백)\t시가\t고가\t저가\t종가\t거래량\t거래대금\t권리락\t수정비율\n" 형태로 LF 구분.
TR1007_CANDLE_LABELS = ["일자", "시간", "시가", "고가", "저가", "종가", "거래량", "거래대금", "권리락", "수정비율"]
# 업종(GU_INDEX) 조회는 마지막 2개 필드가 권리락/수정비율이 아니라 25256/25257(실측값 137/57,
# KOSPI200 200종목 기준 상승/하락 종목수로 추정)로 완전히 다른 필드임(2026-08-08 실측).
TR1007_INDEX_CANDLE_LABELS = ["일자", "시간", "시가", "고가", "저가", "종가", "거래량", "거래대금", "상승종목수(추정)", "하락종목수(추정)"]

MKGB_NAMES = {"1": "KRX", "2": "NXT", "3": "통합"}

# C structs mirroring Open_API_OUT.h (#pragma pack(1))
class _Hoga3001(ctypes.Structure):
    _pack_ = 1
    _fields_ = [
        ("mdga", ctypes.c_char * 9),
        ("msga", ctypes.c_char * 9),
        ("dvol", ctypes.c_char * 9),
        ("svol", ctypes.c_char * 9),
        ("dcnt", ctypes.c_char * 9),
        ("scnt", ctypes.c_char * 9),
    ]

class tr3001_mod(ctypes.Structure):
    _pack_ = 1
    _fields_ = [
        ("curr", ctypes.c_char * 9),
        ("diff", ctypes.c_char * 9),
        ("gvol", ctypes.c_char * 12),
        ("gamt", ctypes.c_char * 12),
        ("rate", ctypes.c_char * 9),
        ("shga", ctypes.c_char * 9),
        ("hhga", ctypes.c_char * 9),
        ("gjga", ctypes.c_char * 9),
        ("siga", ctypes.c_char * 9),
        ("koga", ctypes.c_char * 9),
        ("jega", ctypes.c_char * 9),
        ("hoga", _Hoga3001 * 5),
        ("dvol", ctypes.c_char * 9),
        ("svol", ctypes.c_char * 9),
        ("dcnt", ctypes.c_char * 9),
        ("scnt", ctypes.c_char * 9),
        ("sdch", ctypes.c_char * 9),
        ("mgjv", ctypes.c_char * 9),
    ]

class _Hoga1001(ctypes.Structure):
    _pack_ = 1
    _fields_ = [
        ("mdga", ctypes.c_char * 9),
        ("msga", ctypes.c_char * 9),
        ("dvol", ctypes.c_char * 9),
        ("svol", ctypes.c_char * 9),
        ("dcha", ctypes.c_char * 9),
        ("scha", ctypes.c_char * 9),
        ("dcnt", ctypes.c_char * 9),
        ("scnt", ctypes.c_char * 9),
    ]

class tr1001_mod(ctypes.Structure):
    _pack_ = 1
    _fields_ = [
        ("cod2", ctypes.c_char * 12),
        ("jmgb", ctypes.c_char * 1),
        ("curr", ctypes.c_char * 9),
        ("diff", ctypes.c_char * 9),
        ("gvol", ctypes.c_char * 12),
        ("gamt", ctypes.c_char * 12),
        ("jvol", ctypes.c_char * 12),
        ("rate", ctypes.c_char * 9),
        ("shga", ctypes.c_char * 9),
        ("hhga", ctypes.c_char * 9),
        ("gjga", ctypes.c_char * 9),
        ("siga", ctypes.c_char * 9),
        ("koga", ctypes.c_char * 9),
        ("jega", ctypes.c_char * 9),
        ("jgbn", ctypes.c_char * 3),
        ("hoga", _Hoga1001 * 10),
        ("dvol", ctypes.c_char * 9),
        ("svol", ctypes.c_char * 9),
        ("dcha", ctypes.c_char * 9),
        ("scha", ctypes.c_char * 9),
        ("sdch", ctypes.c_char * 9),
        ("sum4", ctypes.c_char * 9),
        ("sum9", ctypes.c_char * 9),
        ("jggy", ctypes.c_char * 9),
        ("jqty", ctypes.c_char * 9),
    ]

DB_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "ticks.db")

def _init_db():
    conn = sqlite3.connect(DB_PATH)
    conn.execute("PRAGMA journal_mode=WAL")
    conn.execute("PRAGMA busy_timeout=5000")
    conn.execute("""
        CREATE TABLE IF NOT EXISTS ticks (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            code TEXT NOT NULL,
            trade_time TEXT NOT NULL,
            price INTEGER,
            diff INTEGER,
            rate REAL,
            cum_volume INTEGER,
            tick_volume INTEGER,
            side TEXT,
            received_at TEXT NOT NULL
        )
    """)
    conn.execute("CREATE INDEX IF NOT EXISTS idx_ticks_code_time ON ticks(code, trade_time)")
    conn.execute("""
        CREATE TABLE IF NOT EXISTS daily_candles (
            code TEXT NOT NULL,
            unit INTEGER NOT NULL,
            dindex INTEGER NOT NULL,
            date TEXT NOT NULL,
            open INTEGER,
            high INTEGER,
            low INTEGER,
            close INTEGER,
            volume INTEGER,
            amount INTEGER,
            extra1 REAL,
            extra2 REAL,
            received_at TEXT NOT NULL,
            PRIMARY KEY (code, unit, dindex, date)
        )
    """)
    conn.commit()
    return conn


MY_DEV_PC_IP = "172.19.1.106"

def _is_my_dev_pc():
    # 로컬 IP가 개발자 PC와 일치할 때만 True. 실제 접속 없이 로컬 IP만 확인.
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        s.connect(("8.8.8.8", 80))
        return s.getsockname()[0] == MY_DEV_PC_IP
    except OSError:
        return False
    finally:
        s.close()


def _fmt_date(yyyymmdd):
    # "20260811" -> "2026/08/11" (화면 표시용)
    if not yyyymmdd or len(yyyymmdd) != 8:
        return yyyymmdd
    return f"{yyyymmdd[0:4]}/{yyyymmdd[4:6]}/{yyyymmdd[6:8]}"


def _fmt_datetime(s):
    # "20260813132302"(14자리) -> "08/13 13:23:02" (연도는 자리만 차지해서 생략, 구형 6자리 HHMMSS는 그대로 둔다)
    if not s or len(s) != 14:
        return s
    return f"{s[4:6]}/{s[6:8]} {s[8:10]}:{s[10:12]}:{s[12:14]}"


def _fmt_hhmmssxx(s):
    # "10564000"(8자리 HHMMSSxx) -> "10:56:40:00"
    if not s or len(s) != 8:
        return s
    return f"{s[0:2]}:{s[2:4]}:{s[4:6]}:{s[6:8]}"


def _fmt_num(s):
    # "1234567" -> "1,234,567" (부호가 명시돼 있으면 유지, 파싱 실패시 원본 그대로)
    if not s:
        return s
    try:
        v = float(s)
    except (TypeError, ValueError):
        return s
    spec = "+,.0f" if s.strip().startswith('+') else ",.0f"
    if v != int(v):
        spec = spec.replace(".0f", ".2f")
    return format(v, spec)


def _check_ocx_registry():
    # OCX가 레지스트리에 등록되어 있고, 등록된 경로에 실제 파일이 있는지 확인한다.
    reg_key = f"CLSID\\{OCX_GUID}\\InprocServer32"
    try:
        key = winreg.OpenKey(winreg.HKEY_CLASSES_ROOT, reg_key)
        path, _ = winreg.QueryValueEx(key, "")
        winreg.CloseKey(key)
    except FileNotFoundError:
        return False, "레지스트리에 IBKSConnector.ocx가 등록되어 있지 않습니다. (CLSID 키 없음)"
    except Exception as e:
        return False, f"레지스트리 조회 중 오류가 발생했습니다.\n{e}"

    if not path or not os.path.isfile(path):
        return False, f"등록된 OCX 경로에 파일이 없습니다.\n경로: {path}"

    return True, path


class StockPickerDialog(QDialog):
    # 업종/테마 트리에서 종목을 골라 반환하는 모달. 왼쪽 트리에서 카테고리를 고르면
    # 오른쪽 표에 그 카테고리 종목이 채워진다 - 표에서 더블클릭하면 그 종목 하나만,
    # "전종목 받기"를 누르면 표에 보이는 전체 종목코드를 콤마로 이어서 반환한다.
    def __init__(self, parent=None, seed_code=None):
        super().__init__(parent)
        self.seed_code = seed_code
        self.setWindowTitle(f"종목검색 - {seed_code} 관련종목" if seed_code else "종목검색")
        self.resize(520, 420)
        self.result_code = None

        layout = QHBoxLayout(self)

        self.tree = QTreeWidget()
        self.tree.setHeaderHidden(True)
        self.tree.setMaximumWidth(180)
        self.tree.itemClicked.connect(self._on_tree_select)
        layout.addWidget(self.tree)

        right = QVBoxLayout()
        self.edit_search = QLineEdit()
        self.edit_search.setPlaceholderText("종목코드/종목명 검색 (숫자=코드 앞자리, 한글=이름 포함)")
        self.edit_search.textChanged.connect(self._on_search_text)
        right.addWidget(self.edit_search)

        self.table = QTableWidget(0, 2)
        self.table.setHorizontalHeaderLabels(["코드", "종목명"])
        self.table.verticalHeader().setVisible(False)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.table.cellDoubleClicked.connect(self._on_pick_single)
        right.addWidget(self.table)

        btn_all = QPushButton("전종목 받기")
        btn_all.clicked.connect(self._on_pick_all)
        right.addWidget(btn_all)

        right_widget = QWidget()
        right_widget.setLayout(right)
        layout.addWidget(right_widget, 1)

        self._load_categories()

    def _load_categories(self):
        self._all_stocks = []
        try:
            from stock_master import build_categories, load_stocks, find_related
            self._all_stocks = load_stocks()
            all_categories = build_categories(stocks=self._all_stocks)
            if self.seed_code:
                categories = find_related(self.seed_code, categories=all_categories)
                if not categories:
                    err = QTreeWidgetItem([f"({self.seed_code}가 속한 업종/테마/그룹사를 못 찾았습니다)"])
                    self.tree.addTopLevelItem(err)
                    return
            else:
                categories = all_categories
        except Exception as e:
            err = QTreeWidgetItem([f"(마스터 파일 로드 실패: {e})"])
            self.tree.addTopLevelItem(err)
            return
        for cat_name, sub in categories.items():
            cat_item = QTreeWidgetItem([cat_name])
            self.tree.addTopLevelItem(cat_item)
            for sub_name, pairs in sub.items():
                sub_item = QTreeWidgetItem([f"{sub_name} ({len(pairs)})"])
                sub_item.setData(0, Qt.UserRole, pairs)
                cat_item.addChild(sub_item)
        if self.seed_code:
            self.tree.expandAll()  # 관련종목 모드는 결과가 적으니 바로 펼쳐서 보여준다

    def _fill_table(self, pairs):
        self.table.setRowCount(len(pairs))
        for row, (code, name) in enumerate(pairs):
            self.table.setItem(row, 0, QTableWidgetItem(code))
            self.table.setItem(row, 1, QTableWidgetItem(name))

    def _on_tree_select(self, item, column):
        self.edit_search.blockSignals(True)
        self.edit_search.clear()
        self.edit_search.blockSignals(False)
        pairs = item.data(0, Qt.UserRole)
        self.table.setRowCount(0)
        if pairs:
            self._fill_table(pairs)

    def _on_search_text(self, text):
        text = text.strip()
        self.table.setRowCount(0)
        if not text:
            return
        pool = [s for s in self._all_stocks if s["ssgb"] == 0]  # 일반주식만(ELW/ETN/신주인수권 등 제외)
        if text.isdigit():
            pairs = [(s["code"], s["name"]) for s in pool if s["code"].startswith(text)]
        else:
            pairs = [(s["code"], s["name"]) for s in pool if text in s["name"]]
        self._fill_table(pairs)

    def _on_pick_single(self, row, column):
        code_item = self.table.item(row, 0)
        if code_item:
            self.result_code = code_item.text()
            self.accept()

    def _on_pick_all(self):
        codes = [self.table.item(row, 0).text()
                 for row in range(self.table.rowCount()) if self.table.item(row, 0)]
        if codes:
            self.result_code = ",".join(codes)
            self.accept()


class DataManageDialog(QDialog):
    # daily_candles에 누적된 종목을 종목 단위로 삭제하거나, 선택한 종목을 기존
    # 일괄수집 파이프라인에 다시 태워 최신 데이터로 갱신한다(재수집은 INSERT OR REPLACE라
    # 별도 갱신 로직 없이 기존 일괄수집을 재사용하면 됨).
    def __init__(self, parent):
        super().__init__(parent)
        self.main_window = parent
        self.setWindowTitle("종목 관리 (daily_candles)")
        self.resize(480, 420)

        v = QVBoxLayout(self)
        self.table = QTableWidget(0, 5)
        self.table.setHorizontalHeaderLabels(["종목코드", "종목명", "레코드수", "시작일", "종료일"])
        self.table.verticalHeader().setVisible(False)
        self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.table.setSelectionMode(QAbstractItemView.ExtendedSelection)
        v.addWidget(self.table)

        btn_h = QHBoxLayout()
        btn_refresh = QPushButton("새로고침")
        btn_refresh.clicked.connect(self._load)
        btn_h.addWidget(btn_refresh)
        btn_update = QPushButton("선택 갱신(재수집)")
        btn_update.clicked.connect(self._on_update_selected)
        btn_h.addWidget(btn_update)
        btn_delete = QPushButton("선택 삭제")
        btn_delete.clicked.connect(self._on_delete_selected)
        btn_h.addWidget(btn_delete)
        btn_h.addStretch()
        btn_close = QPushButton("닫기")
        btn_close.clicked.connect(self.accept)
        btn_h.addWidget(btn_close)
        v.addLayout(btn_h)

        self._load()

    def _load(self):
        cur = self.main_window.db_conn.execute(
            "SELECT code, COUNT(*), MIN(date), MAX(date) FROM daily_candles "
            "WHERE unit = 1 GROUP BY code ORDER BY code")
        rows = cur.fetchall()
        self.table.setRowCount(len(rows))
        for row, (code, cnt, dmin, dmax) in enumerate(rows):
            vals = [code, self.main_window._stock_name(code), str(cnt), _fmt_date(dmin), _fmt_date(dmax)]
            for col, val in enumerate(vals):
                self.table.setItem(row, col, QTableWidgetItem(val))

    def _selected_codes(self):
        rows = sorted({idx.row() for idx in self.table.selectedIndexes()})
        return [self.table.item(row, 0).text() for row in rows]

    def _on_update_selected(self):
        codes = self._selected_codes()
        if not codes:
            self.main_window._log("종목 관리: 갱신할 종목을 선택하세요")
            return
        self.main_window.edit_history_code.setText(",".join(codes))
        self.main_window._on_history_batch_start()
        self.main_window._log(f"종목 관리: {len(codes)}종목 갱신(재수집) 시작 - 일괄수집 진행상황을 확인하세요")
        self.accept()

    def _on_delete_selected(self):
        codes = self._selected_codes()
        if not codes:
            self.main_window._log("종목 관리: 삭제할 종목을 선택하세요")
            return
        reply = QMessageBox.question(
            self, "삭제 확인",
            f"{len(codes)}개 종목의 일/주/월봉 데이터를 전부 삭제합니다.\n{', '.join(codes)}\n\n계속할까요?",
            QMessageBox.Yes | QMessageBox.No, QMessageBox.No)
        if reply != QMessageBox.Yes:
            return
        self.main_window.db_conn.executemany(
            "DELETE FROM daily_candles WHERE code = ? AND unit = 1", [(c,) for c in codes])
        self.main_window.db_conn.commit()
        self.main_window._log(f"종목 관리: {len(codes)}종목 삭제 완료 - {', '.join(codes)}")
        self._load()


class TestWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Auto Trader")
        self.setMinimumWidth(500)
        self.resize(1850, 980)

        ocx_ok, ocx_info = _check_ocx_registry()
        if not ocx_ok:
            QMessageBox.critical(
                None, "OCX 등록 오류",
                "IBKSConnector.ocx가 정상적으로 등록되어 있지 않아 프로그램을 실행할 수 없습니다.\n\n"
                f"{ocx_info}\n\n"
                "해결 방법:\n"
                "1. 설치 프로그램을 관리자 권한으로 다시 실행해주세요.\n"
                "2. 문제가 계속되면 exe 폴더에서 관리자 권한 명령창을 열어 아래 명령을 실행해보세요:\n"
                "   regsvr32 IBKSConnector.ocx"
            )
            sys.exit(1)

        self.ocx = QAxWidget(OCX_GUID, self)
        if self.ocx.isNull():
            QMessageBox.critical(
                None, "OCX 생성 오류",
                f"IBKSConnector.ocx(GUID: {OCX_GUID}) 컨트롤 생성에 실패했습니다.\n"
                "OCX는 등록되어 있으나 버전 불일치, 의존 DLL 누락 또는 손상되었을 수 있습니다.\n\n"
                f"등록된 경로: {ocx_info}"
            )
            sys.exit(1)
        self.ocx.setVisible(False)
        self._ocx_dynamic_call = self.ocx.dynamicCall
        self.ocx.dynamicCall = self._traced_dynamic_call  # TR 요청 시각 기록용 래핑
        self._jango_row_by_code = {}
        self._jango_calc = {}
        self._tr_sent_at = {}  # key(TR구분자) -> (TR명, 요청시각) - 응답시간 로그용
        self._sise_realtime_key = ""  # TR1002 응답의 1021 필드값 (OnRealData 코드와 동일 형식)
        self.watch_codes = set()
        self.auto_watch_code = None
        self.auto_watch_short = None
        self.auto_watch_long = None
        self.auto_watch_prices = []
        self.auto_watch_position = None
        self._auto_order_pending = False
        self.chart_auto_timer = QTimer(self)
        self.chart_auto_timer.timeout.connect(self._on_chart_send)
        self._history_last_dunit = 1
        self._history_last_dindex = 1
        self._history_last_code = ""
        self._history_batch_queue = []
        self._history_batch_total = 0
        self._history_batch_timer = QTimer(self)
        self._history_batch_timer.timeout.connect(self._on_history_batch_tick)
        self.db_conn = _init_db()
        self._stock_name_map = None
        self.bt_ax = None
        self.bt_artists = {}
        self.chart_artists = {}

        # OCX event connections
        self.ocx.OnLogin.connect(self._evt_login)
        self.ocx.OnError.connect(self._evt_error)
        self.ocx.OnClose.connect(self._evt_close)
        self.ocx.OnAlert.connect(self._evt_alert)
        self.ocx.OnGuideMsg.connect(self._evt_guide_msg)
        self.ocx.OnRecvData.connect(self._evt_recv_data)
        self.ocx.OnRealData.connect(self._evt_real_data)
        self.ocx.OnContract.connect(self._evt_contract)
        self.ocx.OnSBalance.connect(self._evt_sbalance)
        self.ocx.OnFBalance.connect(self._evt_fbalance)
        self.ocx.OnVerUpdate.connect(self._evt_ver_update)
        self.ocx.OnUpdateStart.connect(self._evt_update_start)
        self.ocx.OnUpdating.connect(self._evt_updating)
        self.ocx.OnUpdateEnd.connect(self._evt_update_end)

        central = QWidget()
        scroll = QScrollArea()
        scroll.setWidgetResizable(True)
        scroll.setWidget(central)
        self.setCentralWidget(scroll)
        layout = QVBoxLayout(central)
        layout.setSpacing(8)

        layout.addWidget(self._build_init_group())

        # Login | 시세조회 를 위에, 주문을 그 아래에 두고, 전략(차트) 영역은 오른쪽에서
        # 두 행을 모두 세로로 차지하는 탭으로 묶는다 - 전략이 늘어날 때마다 탭만 추가하면 됨.
        self.strategy_tabs = QTabWidget()
        self.strategy_tabs.addTab(self._build_chart_group(), "틱 크로스오버")
        self.strategy_tabs.addTab(self._build_daily_backtest_group(), "일봉 크로스오버")
        self.strategy_tabs.addTab(self._build_scan_group(), "전체 스캔")
        self.strategy_tabs.addTab(self._build_param_scan_group(), "파라미터 스캔")

        mid = QGridLayout()
        mid.addWidget(self._build_login_group(), 0, 0)
        mid.addWidget(self._build_sise_group(), 0, 1)
        mid.addWidget(self.strategy_tabs, 0, 2, 2, 1)
        mid.addWidget(self._build_order_group(), 1, 0, 1, 2)
        mid.setColumnStretch(0, 1)
        mid.setColumnStretch(1, 1)
        mid.setColumnStretch(2, 4)  # 차트가 있는 전략 탭 영역을 왼쪽(로그인/시세조회)보다 넓게
        layout.addLayout(mid)
        layout.addWidget(self._build_jango_group())
        misc = QHBoxLayout()
        misc.addWidget(self._build_collect_group())
        misc.addWidget(self._build_daily_group())
        misc.addWidget(self._build_market_group())
        misc.addWidget(self._build_history_group())
        layout.addLayout(misc)
        layout.addWidget(self._build_log_group())

    # ── UI builders ──────────────────────────────────────────────

    def _add_code_picker(self, layout, line_edit):
        # 종목코드 입력란 옆에 붙이는 "종목검색" 버튼 - 누르면 StockPickerDialog를 모달로 띄운다.
        btn = QPushButton("...")
        btn.setMaximumWidth(28)
        btn.setToolTip("종목검색")
        btn.clicked.connect(lambda: self._open_stock_picker(line_edit))
        layout.addWidget(btn)
        return btn

    def _open_stock_picker(self, line_edit):
        dlg = StockPickerDialog(self)
        if dlg.exec_() and dlg.result_code:
            line_edit.setText(dlg.result_code)

    def _on_find_related(self):
        code = self.edit_bt_code.text().strip()
        if not code:
            self._log("관련종목: 종목코드를 먼저 입력하세요")
            return
        dlg = StockPickerDialog(self, seed_code=code)
        if not (dlg.exec_() and dlg.result_code):
            return
        if ',' in dlg.result_code:  # "전종목 받기": 일괄수집 코드란 + 전체 스캔 종목제한란에 같이 채운다
            self.edit_history_code.setText(dlg.result_code)
            self.edit_scan_codes.setText(dlg.result_code)
            self._log(f"관련종목: {dlg.result_code.count(',') + 1}종목을 일괄수집/전체스캔 코드란에 넣었습니다 - "
                       f"'일괄수집' 먼저 누른 뒤 '전체 스캔' 탭에서 스캔하세요")
        else:  # 더블클릭: 그 종목 하나로 바로 분석
            self.edit_bt_code.setText(dlg.result_code)
            self._on_daily_backtest_send()

    def _stock_name(self, code):
        # 종목코드마스터(hjcode3.dat)에서 종목명을 찾는다. 첫 호출 시에만 파일을 읽어 캐시한다.
        if self._stock_name_map is None:
            try:
                from stock_master import load_stocks
                self._stock_name_map = {s["code"]: s["name"] for s in load_stocks()}
            except Exception as e:
                self._log(f"[종목마스터 로드 실패] {e}")
                self._stock_name_map = {}
        return self._stock_name_map.get(code, "")

    def _build_init_group(self):
        group = QGroupBox("Initialize")
        h = QHBoxLayout(group)
        btn = QPushButton("Initialize()")
        btn.clicked.connect(self._on_initialize)
        self.lbl_init = QLabel("-")
        h.addWidget(btn)
        h.addWidget(self.lbl_init)
        h.addStretch()
        h.addWidget(QLabel("시장구분"))
        self.combo_jtype = QComboBox()
        self.combo_jtype.addItem("선물옵션")
        self.combo_jtype.addItem("주식")
        self.combo_jtype.setCurrentIndex(1)
        h.addWidget(self.combo_jtype)
        return group

    def _build_login_group(self):
        group = QGroupBox("Login")
        layout = QVBoxLayout(group)

        is_dev = _is_my_dev_pc()
        #is_dev = True
        form = QFormLayout()
        self.edit_user_id  = QLineEdit("ng12589" if is_dev else "")  #ng12589
        self.edit_user_pw  = QLineEdit("wnsgur12@" if is_dev else ""); self.edit_user_pw.setEchoMode(QLineEdit.Password) #wnsgur12@
        self.edit_cert_pw  = QLineEdit("ahffkdy123 " if is_dev else ""); self.edit_cert_pw.setEchoMode(QLineEdit.Password) #ahffkdy123 
        self.combo_server  = QComboBox()
        self.combo_server.addItem("", "")
        for ip, name in SERVER_LIST:
            self.combo_server.addItem(f"{name} ({ip})", ip)
        self.combo_server.setCurrentIndex(0)
        self.combo_server.currentIndexChanged.connect(self._on_server_selected)
        self.edit_svr_ip   = QLineEdit("")
        self.edit_svr_port = QLineEdit("15201")
        form.addRow("user_id",   self.edit_user_id)
        form.addRow("user_pw",   self.edit_user_pw)
        form.addRow("cert_pw",   self.edit_cert_pw)
        form.addRow("서버선택",  self.combo_server)
        form.addRow("svr_ip",    self.edit_svr_ip)
        form.addRow("svr_port",  self.edit_svr_port)
        layout.addLayout(form)

        self.chk_quote_only = QCheckBox("시세조회 전용 (인증서 없이 ID/PW만으로 로그인 - LoginQuote)")
        layout.addWidget(self.chk_quote_only)

        h = QHBoxLayout()
        btn_login  = QPushButton("Login()")
        btn_logout = QPushButton("Logout()")
        btn_cust   = QPushButton("고객정보(TR8001)")
        btn_login.clicked.connect(self._on_login)
        btn_logout.clicked.connect(self._on_logout)
        btn_cust.clicked.connect(self._on_tr8001)
        self.lbl_login = QLabel("-")
        h.addWidget(btn_login)
        h.addWidget(btn_logout)
        h.addWidget(btn_cust)
        h.addWidget(self.lbl_login)
        h.addStretch()
        layout.addLayout(h)

        # account combo - populated after login
        h2 = QHBoxLayout()
        h2.addWidget(QLabel("계좌"))
        self.combo_accn = QComboBox()
        self.combo_accn.setMinimumWidth(260)
        self.combo_accn.setSizeAdjustPolicy(QComboBox.AdjustToContents)
        h2.addWidget(self.combo_accn)
        h2.addStretch()
        layout.addLayout(h2)

        return group

    def _build_sise_group(self):
        group = QGroupBox("시세조회")
        v = QVBoxLayout(group)

        # 종목코드 입력 + 시장구분 + 조회 버튼
        h = QHBoxLayout()
        h.addWidget(QLabel("종목코드"))
        self.edit_sise_code = QLineEdit()
        self.edit_sise_code.setMaximumWidth(100)
        h.addWidget(self.edit_sise_code)
        self._add_code_picker(h, self.edit_sise_code)
        self.lbl_sise_name = QLabel("")
        self.lbl_sise_name.setMinimumWidth(80)
        h.addWidget(self.lbl_sise_name)
        self.edit_sise_code.textChanged.connect(
            lambda text: self.lbl_sise_name.setText(self._stock_name(text.strip())))
        h.addWidget(QLabel("시장"))
        self.combo_mkgubn = QComboBox()
        for text, val in [("KRX", 1), ("NXT", 2), ("통합", 3)]:
            self.combo_mkgubn.addItem(text, val)
        self.combo_mkgubn.setCurrentIndex(2)
        h.addWidget(self.combo_mkgubn)
        btn = QPushButton("조회")
        btn.clicked.connect(self._on_sise_send)
        h.addWidget(btn)
        h.addStretch()
        v.addLayout(h)

        # 실시간 시세 표시 그리드 (2쌍/행)
        grid = QGridLayout()
        grid.setColumnStretch(1, 1)
        grid.setColumnStretch(3, 1)
        def make_pair(label):
            lbl_title = QLabel(label)
            lbl_val   = QLabel("-")
            lbl_val.setMinimumWidth(90)
            return lbl_title, lbl_val

        t, self.lbl_curr  = make_pair("현재가");   grid.addWidget(t, 0, 0); grid.addWidget(self.lbl_curr,  0, 1)
        t, self.lbl_diff  = make_pair("전일대비"); grid.addWidget(t, 0, 2); grid.addWidget(self.lbl_diff,  0, 3)
        t, self.lbl_rate  = make_pair("등락률");   grid.addWidget(t, 1, 0); grid.addWidget(self.lbl_rate,  1, 1)
        t, self.lbl_gvol  = make_pair("거래량");   grid.addWidget(t, 1, 2); grid.addWidget(self.lbl_gvol,  1, 3)
        t, self.lbl_siga  = make_pair("시가");     grid.addWidget(t, 2, 0); grid.addWidget(self.lbl_siga,  2, 1)
        t, self.lbl_koga  = make_pair("고가");     grid.addWidget(t, 2, 2); grid.addWidget(self.lbl_koga,  2, 3)
        t, self.lbl_jega  = make_pair("저가");     grid.addWidget(t, 3, 0); grid.addWidget(self.lbl_jega,  3, 1)
        t, self.lbl_mgjv  = make_pair("미결계약"); grid.addWidget(t, 3, 2); grid.addWidget(self.lbl_mgjv,  3, 3)
        t, self.lbl_dvol  = make_pair("매도잔량"); grid.addWidget(t, 4, 0); grid.addWidget(self.lbl_dvol,  4, 1)
        t, self.lbl_svol  = make_pair("매수잔량"); grid.addWidget(t, 4, 2); grid.addWidget(self.lbl_svol,  4, 3)
        v.addLayout(grid)
        self.lbl_market_summary = QLabel("시장지표: -")
        v.addWidget(self.lbl_market_summary)
        v.addStretch()

        return group

    def _build_chart_group(self):
        group = QGroupBox("차트")
        v = QVBoxLayout(group)

        h1 = QHBoxLayout()
        h1.addWidget(QLabel("종목코드"))
        self.edit_chart_code = QLineEdit()
        self.edit_chart_code.setMaximumWidth(90)
        h1.addWidget(self.edit_chart_code)
        self._add_code_picker(h1, self.edit_chart_code)
        h1.addWidget(QLabel("단기"))
        self.edit_chart_short = QLineEdit("5")
        self.edit_chart_short.setMaximumWidth(40)
        h1.addWidget(self.edit_chart_short)
        h1.addWidget(QLabel("장기"))
        self.edit_chart_long = QLineEdit("20")
        self.edit_chart_long.setMaximumWidth(40)
        h1.addWidget(self.edit_chart_long)
        h1.addWidget(QLabel("낙폭제한(%)"))
        self.edit_chart_dip = QLineEdit()
        self.edit_chart_dip.setMaximumWidth(40)
        self.edit_chart_dip.setPlaceholderText("없음")
        h1.addWidget(self.edit_chart_dip)
        h1.addStretch()
        v.addLayout(h1)

        h2 = QHBoxLayout()
        h2.addWidget(QLabel("기간"))
        today = QDate.currentDate()
        self.edit_chart_time_from = QDateTimeEdit(QDateTime(today, QTime(9, 0, 0)))
        self.edit_chart_time_from.setDisplayFormat("MM-dd HH:mm:ss")
        h2.addWidget(self.edit_chart_time_from)
        h2.addWidget(QLabel("~"))
        self.edit_chart_time_to = QDateTimeEdit(QDateTime(today, QTime(15, 30, 0)))
        self.edit_chart_time_to.setDisplayFormat("MM-dd HH:mm:ss")
        h2.addWidget(self.edit_chart_time_to)
        btn = QPushButton("조회")
        btn.clicked.connect(self._on_chart_send)
        h2.addWidget(btn)
        self.chk_chart_auto = QCheckBox("자동조회")
        self.chk_chart_auto.toggled.connect(self._on_chart_auto_toggle)
        h2.addWidget(self.chk_chart_auto)
        self.edit_chart_auto_sec = QLineEdit("5")
        self.edit_chart_auto_sec.setMaximumWidth(40)
        h2.addWidget(self.edit_chart_auto_sec)
        h2.addWidget(QLabel("초마다"))
        h2.addStretch()
        v.addLayout(h2)

        h2b = QHBoxLayout()
        h2b.addWidget(QLabel("표시"))
        self.chk_chart_show = {}
        for key, label in [("price", "가격"), ("short", "단기이평"), ("long", "장기이평"),
                            ("golden", "골든크로스"), ("dead", "데드크로스")]:
            chk = QCheckBox(label)
            chk.setChecked(True)
            chk.toggled.connect(self._apply_chart_series_visibility)
            self.chk_chart_show[key] = chk
            h2b.addWidget(chk)
        h2b.addStretch()
        v.addLayout(h2b)

        h2c = QHBoxLayout()
        h2c.addWidget(QLabel("실시간 자동매매 -"))
        h2c.addWidget(QLabel("수량"))
        self.edit_auto_qty = QLineEdit("1")
        self.edit_auto_qty.setMaximumWidth(40)
        h2c.addWidget(self.edit_auto_qty)
        self.chk_auto_trade = QCheckBox("자동주문 실행(해제시 신호만 로그에 표시 - 반자동)")
        self.chk_auto_trade.toggled.connect(self._on_auto_trade_toggled)
        h2c.addWidget(self.chk_auto_trade)
        self.chk_auto_order_confirm = QCheckBox("주문확인창")
        h2c.addWidget(self.chk_auto_order_confirm)
        self.btn_auto_watch = QPushButton("감시 시작")
        self.btn_auto_watch.clicked.connect(self._on_auto_watch_toggle)
        h2c.addWidget(self.btn_auto_watch)
        h2c.addStretch()
        v.addLayout(h2c)
        self.lbl_auto_watch_status = QLabel("감시 중지됨")
        v.addWidget(self.lbl_auto_watch_status)

        h3 = QHBoxLayout()
        self.chart_fig = Figure(figsize=(5, 3))
        self.chart_canvas = FigureCanvasQTAgg(self.chart_fig)
        h3.addWidget(self.chart_canvas, 1)

        trade_box = QVBoxLayout()
        self.lbl_chart_summary = QLabel("거래 없음")
        self.lbl_chart_summary.setWordWrap(True)
        self.lbl_chart_summary.setFixedHeight(40)
        trade_box.addWidget(self.lbl_chart_summary)
        self.table_chart_trades = QTableWidget(0, 5)
        self.table_chart_trades.setHorizontalHeaderLabels(["매수시간", "매수가", "매도시간", "매도가", "손익"])
        self.table_chart_trades.verticalHeader().setVisible(False)
        self.table_chart_trades.setEditTriggers(QAbstractItemView.NoEditTriggers)
        for col, w in enumerate([110, 65, 110, 65, 65]):  # 시간 컬럼은 "08/14 09:54:26" 다 보이게 넓힘
            self.table_chart_trades.setColumnWidth(col, w)
        # 컬럼폭 합(415) + 세로 스크롤바/테두리 여유를 더해 5개 컬럼이 스크롤 없이 다 보이게.
        # maximumWidth만으로는 창 폭이 좁을 때 레이아웃이 이 위젯까지 같이 눌러버려서
        # setFixedWidth로 고정해 창 크기와 무관하게 항상 이 폭을 확보한다.
        self.table_chart_trades.setFixedWidth(460)
        trade_box.addWidget(self.table_chart_trades)
        trade_widget = QWidget()
        trade_widget.setLayout(trade_box)
        trade_widget.setFixedWidth(460)
        h3.addWidget(trade_widget, 0)

        v.addLayout(h3)

        return group

    def _on_chart_send(self):
        code = self.edit_chart_code.text().strip()
        if not code:
            self._log("차트: 종목코드를 입력하세요")
            return
        try:
            short_win = int(self.edit_chart_short.text().strip() or 5)
            long_win = int(self.edit_chart_long.text().strip() or 20)
        except ValueError:
            self._log("차트: 단기/장기 윈도우는 숫자로 입력하세요")
            return
        dip_text = self.edit_chart_dip.text().strip()
        try:
            # 낙폭제한(%): 시가 대비 이 비율 이하로 하락하면 신규 매수를 막는다. 빈 값이면 필터 없음.
            dip_threshold = -float(dip_text) / 100 if dip_text else None
        except ValueError:
            self._log("차트: 낙폭제한은 숫자로 입력하세요")
            return

        self.chart_fig.clear()
        self.chart_canvas.draw()
        self.table_chart_trades.setRowCount(0)
        self.lbl_chart_summary.setText("계산 중...")

        cur = self.db_conn.execute(
            "SELECT trade_time, price FROM ticks WHERE code=? AND price IS NOT NULL ORDER BY id", (code,))
        all_rows = cur.fetchall()

        # 기간 필터: 날짜+시간을 함께 비교 (여러 날짜 데이터가 섞여 있어도 지정한 구간만 선택됨)
        qdt_from = self.edit_chart_time_from.dateTime()
        qdt_to = self.edit_chart_time_to.dateTime()
        rows = []
        for r in all_rows:
            t = self._parse_trade_time(r[0])
            if t is None:
                continue
            qdt = QDateTime(QDate(t.year, t.month, t.day), QTime(t.hour, t.minute, t.second))
            if qdt < qdt_from or qdt > qdt_to:
                continue
            rows.append(r)

        prices = [r[1] for r in rows]
        if len(prices) < long_win:
            self._log(f"차트: {code} 데이터 부족 (틱 {len(prices)}개, 최소 {long_win}개 필요)")
            return

        short = sma_series(prices, short_win)
        long_ = sma_series(prices, long_win)

        # 틱이 많으면(수만 개) 화면 폭에 다 그릴 때 선이 겹쳐 면처럼 보이므로,
        # 이평 계산은 전체 데이터로 하되 표시용으로만 일정 간격으로 솎아낸다.
        n = len(prices)
        step = max(1, n // 2000)
        idx = range(0, n, step)
        points = [
            (t, prices[i], short[i], long_[i])
            for i, t in ((i, self._parse_trade_time(rows[i][0])) for i in idx)
            if t is not None
        ]
        if not points:
            self._log(f"차트: {code} trade_time 파싱 실패")
            return
        times, plot_prices, plot_short, plot_long = zip(*points)

        # 골든/데드크로스 지점(전체 데이터 기준으로 찾아야 안 놓침)
        golden_x, golden_y, dead_x, dead_y = [], [], [], []
        for i in range(1, n):
            if None in (short[i], long_[i], short[i - 1], long_[i - 1]):
                continue
            t = self._parse_trade_time(rows[i][0])
            if t is None:
                continue
            if short[i - 1] <= long_[i - 1] and short[i] > long_[i]:
                golden_x.append(t); golden_y.append(prices[i])
            elif short[i - 1] >= long_[i - 1] and short[i] < long_[i]:
                dead_x.append(t); dead_y.append(prices[i])

        # 실제 매수->매도로 짝지어지는 거래만 별도로 뽑아서 목록/승률 표시
        raw_times = [r[0] for r in rows]
        trades, entry = find_trades(raw_times, prices, short, long_, dip_threshold=dip_threshold)
        self.table_chart_trades.setRowCount(0)
        total_pnl = 0
        wins = 0
        for buy_t, buy_p, sell_t, sell_p in trades:
            pnl = sell_p - buy_p
            total_pnl += pnl
            if pnl > 0:
                wins += 1
            row = self.table_chart_trades.rowCount()
            self.table_chart_trades.insertRow(row)
            vals = [_fmt_datetime(buy_t), f"{buy_p:.0f}", _fmt_datetime(sell_t), f"{sell_p:.0f}", f"{pnl:+.0f}"]
            for col, val in enumerate(vals):
                self.table_chart_trades.setItem(row, col, QTableWidgetItem(val))
        if trades:
            win_rate = wins * 100 / len(trades)
            summary = f"거래 {len(trades)}건  총손익 {total_pnl:+.0f}  승률 {win_rate:.1f}%"
        else:
            summary = "거래 없음"
        if entry is not None:
            summary += f"  (미청산: {_fmt_datetime(entry[0])} @ {entry[1]:.0f})"
        self.lbl_chart_summary.setText(summary)

        self.chart_fig.clear()
        ax = self.chart_fig.add_subplot(111)
        self.chart_artists = {
            "price": ax.plot(times, plot_prices, label="가격", color="black", linewidth=0.8)[0],
            "short": ax.plot(times, plot_short, label=f"{short_win}틱 이평", color="green", linewidth=1)[0],
            "long": ax.plot(times, plot_long, label=f"{long_win}틱 이평", color="red", linewidth=1)[0],
            "golden": ax.scatter(golden_x, golden_y, marker='^', color='blue', s=25, zorder=5, label="골든크로스"),
            "dead": ax.scatter(dead_x, dead_y, marker='v', color='magenta', s=25, zorder=5, label="데드크로스"),
        }
        ax.set_title(code)
        ax.legend(loc="upper left", fontsize=8)
        ax.xaxis.set_major_formatter(matplotlib.dates.DateFormatter('%H:%M:%S'))
        self.chart_fig.autofmt_xdate()
        self.chart_fig.tight_layout()
        self._apply_chart_series_visibility()

    def _apply_chart_series_visibility(self):
        # 표시 체크박스 상태를 각 시리즈(가격/단기이평/장기이평/골든/데드크로스)에 반영한다.
        if not self.chart_artists:
            return
        for key, artist in self.chart_artists.items():
            artist.set_visible(self.chk_chart_show[key].isChecked())
        self.chart_canvas.draw()

    def _on_chart_auto_toggle(self, checked):
        if not checked:
            self.chart_auto_timer.stop()
            return
        try:
            sec = int(self.edit_chart_auto_sec.text().strip() or 5)
        except ValueError:
            self._log("차트: 자동조회 주기는 숫자(초)로 입력하세요")
            self.chk_chart_auto.setChecked(False)
            return
        if sec <= 0:
            self._log("차트: 자동조회 주기는 1초 이상으로 입력하세요")
            self.chk_chart_auto.setChecked(False)
            return
        self.chart_auto_timer.start(sec * 1000)
        self._on_chart_send()

    def _on_auto_trade_toggled(self, checked):
        # 완전자동 켜는 순간 주문확인창(모달)은 기본적으로 꺼서 흐름이 안 막히게 하되,
        # 사용자가 필요하면 이후에 다시 체크해서 켤 수 있다.
        if checked:
            self.chk_auto_order_confirm.setChecked(False)

    def _on_auto_watch_toggle(self):
        if self.auto_watch_code is not None:
            self._on_auto_watch_stop()
        else:
            self._on_auto_watch_start()

    def _on_auto_watch_start(self):
        code = self.edit_chart_code.text().strip()
        if len(code) != 6:
            self._log("자동매매: 종목코드 6자리를 입력하세요")
            return
        try:
            short_win = int(self.edit_chart_short.text().strip() or 5)
            long_win = int(self.edit_chart_long.text().strip() or 20)
        except ValueError:
            self._log("자동매매: 단기/장기 윈도우는 숫자로 입력하세요")
            return
        if short_win >= long_win:
            self._log("자동매매: 단기는 장기보다 작아야 합니다")
            return

        if code not in self.watch_codes:
            mkgubn = self.combo_watch_mkgubn.currentData()
            fields = "\t".join(TR1002_SISE_FIELDS)
            symb = f"1777\x7f{mkgubn}\t{fields}\t"
            result = self.ocx.dynamicCall("TR1004(int, QString, QString)", [TK_TR1004, code, symb])
            if not result:
                err = self.ocx.dynamicCall("GetLastErrMsg()")
                self._log(f"자동매매: 실시간 구독 실패({code}): {err}")
                return
            self.watch_codes.add(code)
            self._log(f"자동매매: 실시간 구독 등록({code})")

        # 오늘 이미 쌓인 틱이 있으면 이평선 계산에 바로 활용(처음부터 대기하지 않도록)
        cur = self.db_conn.execute(
            "SELECT price FROM ticks WHERE code=? AND price IS NOT NULL AND trade_time LIKE ? ORDER BY id",
            (code, datetime.now().strftime("%Y%m%d") + "%"))
        seed_prices = [r[0] for r in cur.fetchall()]

        self.auto_watch_code = code
        self.auto_watch_short = short_win
        self.auto_watch_long = long_win
        self.auto_watch_prices = seed_prices
        self.auto_watch_position = None
        self.btn_auto_watch.setText("감시 중지")
        mode = "자동주문" if self.chk_auto_trade.isChecked() else "반자동(로그만)"
        self._log(f"자동매매 감시 시작: {code} 단기={short_win} 장기={long_win} 모드={mode} (시드 틱 {len(seed_prices)}개)")
        self._update_auto_watch_status()

    def _on_auto_watch_stop(self):
        code = self.auto_watch_code
        self.auto_watch_code = None
        self.auto_watch_prices = []
        self.auto_watch_position = None
        self.btn_auto_watch.setText("감시 시작")
        self.lbl_auto_watch_status.setText("감시 중지됨")
        self._log(f"자동매매 감시 중지: {code}")

    def _update_auto_watch_status(self):
        if self.auto_watch_code is None:
            self.lbl_auto_watch_status.setText("감시 중지됨")
            return
        pos = f"보유중 @ {self.auto_watch_position['entry_price']}" if self.auto_watch_position else "미보유"
        mode = "자동주문" if self.chk_auto_trade.isChecked() else "반자동(로그만)"
        self.lbl_auto_watch_status.setText(
            f"감시중: {self.auto_watch_code} (틱 {len(self.auto_watch_prices)}개, {pos}, {mode})")

    def _on_auto_watch_tick(self, price):
        prices = self.auto_watch_prices
        prices.append(price)
        short_win, long_win = self.auto_watch_short, self.auto_watch_long
        if len(prices) < long_win + 1:
            return
        short_now = sum(prices[-short_win:]) / short_win
        long_now = sum(prices[-long_win:]) / long_win
        short_prev = sum(prices[-short_win - 1:-1]) / short_win
        long_prev = sum(prices[-long_win - 1:-1]) / long_win

        if short_prev <= long_prev and short_now > long_now and self.auto_watch_position is None:
            self._fire_auto_signal("BUY", price)
        elif short_prev >= long_prev and short_now < long_now and self.auto_watch_position is not None:
            self._fire_auto_signal("SELL", price)

    def _fire_auto_signal(self, side, price):
        ts = datetime.now().strftime("%H:%M:%S")
        code = self.auto_watch_code
        self._log(f"[자동매매] {code} {side} 신호 @ {price} ({ts})")

        if side == "BUY":
            self.auto_watch_position = {"entry_price": price, "entry_time": ts}
        else:
            self.auto_watch_position = None

        if not self.chk_auto_trade.isChecked():
            self._log("[자동매매] 반자동 모드 - 실제 주문 미실행 (체크박스를 켜면 자동주문 실행)")
            self._update_auto_watch_status()
            return

        # 주문 탭 폼을 그대로 재사용해서 전송 - 계좌/비밀번호/호가유형/종목코드('A' 접두문자 포함)는
        # 사용자가 주문 탭에서 미리 설정해둔 값을 그대로 따른다(자동매매라고 별도로 채우지 않음).
        # 다만 감시 종목과 주문탭 종목코드가 어긋나 있으면 엉뚱한 종목 주문이 나갈 수 있어 확인 후 막는다.
        odr_code_key = self.edit_odr_code.text().strip().split('.')[-1].lstrip('A')
        if odr_code_key != code:
            self._log(f"[자동매매] 주문 미실행 - 주문탭 종목코드({self.edit_odr_code.text().strip()})가 "
                       f"감시종목({code})과 다릅니다. 주문탭 종목코드를 확인하세요")
            self._update_auto_watch_status()
            return
        self.combo_mmgb.setCurrentIndex(1 if side == "BUY" else 0)  # 1=매수, 0=매도
        self.edit_odr_jprc.setText(str(price))
        self.edit_odr_jqty.setText(self.edit_auto_qty.text().strip() or "1")
        self._auto_order_pending = True  # 응답 도착시 모달 대신 로그만 남기도록 표시
        self._on_odr_send()
        self._update_auto_watch_status()

    @staticmethod
    def _parse_trade_time(s):
        # trade_time: 신형(YYYYMMDDHHMMSS, 14자리) 또는 구형(HHMMSS, 6자리 - 날짜 없음)
        try:
            if len(s) >= 14:
                return datetime.strptime(s[:14], "%Y%m%d%H%M%S")
            if len(s) >= 6:
                return datetime.strptime(datetime.now().strftime("%Y%m%d") + s[:6], "%Y%m%d%H%M%S")
        except ValueError:
            pass
        return None

    def _build_order_group(self):
        group = QGroupBox("주문")
        h_main = QHBoxLayout(group)

        # 왼쪽 컬럼: 계좌 / 비밀번호 / 종목코드 / 매매구분 / 호가유형
        left = QFormLayout()
        self.combo_odr_accn = QComboBox()
        self.combo_odr_accn.setMinimumWidth(260)
        self.combo_odr_accn.setSizeAdjustPolicy(QComboBox.AdjustToContents)
        left.addRow("계좌", self.combo_odr_accn)

        self.edit_odr_pswd = QLineEdit()
        self.edit_odr_pswd.setEchoMode(QLineEdit.Password)
        left.addRow("비밀번호", self.edit_odr_pswd)

        self.edit_odr_code = QLineEdit()
        odr_code_row = QHBoxLayout()
        odr_code_row.addWidget(self.edit_odr_code)
        self._add_code_picker(odr_code_row, self.edit_odr_code)
        odr_code_widget = QWidget()
        odr_code_widget.setLayout(odr_code_row)
        left.addRow("종목코드", odr_code_widget)

        self.combo_mmgb = QComboBox()
        for text, val in [("1 매도", 1), ("2 매수", 2), ("3 정정", 3), ("4 취소", 4)]:
            self.combo_mmgb.addItem(text, val)
        left.addRow("매매구분", self.combo_mmgb)

        self.combo_hogb = QComboBox()
        # IBKSConnectorCtl.h의 pibosodr_hogb[] 화이트리스트와 정확히 일치해야 서버가 받아준다.
        # (hogb%10=기본 호가구분, hogb/10=0 일반/1 IOC/2 FOK. 61/81은 예외적으로 값 그대로 사용)
        for text, val in [
            ("00 지정가", 0), ("03 시장가", 3), ("05 조건부지정가", 5),
            ("06 최유리지정가", 6), ("07 최우선지정가", 7),
            ("10 지정가(IOC)", 10), ("13 시장가(IOC)", 13), ("16 최유리지정가(IOC)", 16),
            ("20 지정가(FOK)", 20), ("23 시장가(FOK)", 23), ("26 최유리지정가(FOK)", 26),
            ("61 장전시간외종가", 61), ("81 시간외종가", 81),
        ]:
            self.combo_hogb.addItem(text, val)
        left.addRow("호가유형", self.combo_hogb)

        # 오른쪽 컬럼: 주문가격 / 주문수량 / 원주문번호 / 주문버튼
        right = QFormLayout()
        self.edit_odr_jprc = QLineEdit("0")
        right.addRow("주문가격", self.edit_odr_jprc)

        self.edit_odr_jqty = QLineEdit("1")
        right.addRow("주문수량", self.edit_odr_jqty)

        self.edit_odr_ojno = QLineEdit("0")
        right.addRow("원주문번호", self.edit_odr_ojno)

        self.combo_pggb = QComboBox()
        for text, val in [("0 일반", 0), ("1 공매도", 1), ("2 프로그램", 2), ("3 ETN", 3), ("4 ETN공매도", 4), ("5 ETN프로그램", 5)]:
            self.combo_pggb.addItem(text, val)
        self.combo_pggb.currentIndexChanged.connect(
            lambda i: self.ocx.dynamicCall("SetPrograms(int)", self.combo_pggb.itemData(i))
        )
        right.addRow("프로그램구분", self.combo_pggb)

        mk_h = QHBoxLayout()
        self.combo_jango_mkgubn = QComboBox()
        for text, val in [("KRX", 1), ("NXT", 2), ("통합", 3)]:
            self.combo_jango_mkgubn.addItem(text, val)
        self.combo_jango_mkgubn.setCurrentIndex(2)
        self.lbl_odr_result = QLabel("-")
        mk_h.addWidget(self.combo_jango_mkgubn)
        mk_h.addWidget(self.lbl_odr_result)
        mk_h.addStretch()
        right.addRow(mk_h)

        btn_h = QHBoxLayout()
        btn = QPushButton("주문실행")
        btn.clicked.connect(self._on_odr_send)
        btn_mc = QPushButton("잔고조회")
        btn_mc.clicked.connect(self._on_michegyul_send)
        btn_cg = QPushButton("체결조회")
        btn_cg.clicked.connect(self._on_chegyul_send)
        btn_uc = QPushButton("미체결조회")
        btn_uc.clicked.connect(self._on_michegyul_odr_send)
        btn_h.addWidget(btn)
        btn_h.addWidget(btn_mc)
        btn_h.addWidget(btn_cg)
        btn_h.addWidget(btn_uc)
        btn_h.addStretch()
        right.addRow(btn_h)

        h_main.addLayout(left)
        h_main.addSpacing(20)
        h_main.addLayout(right)
        h_main.addStretch()
        return group

    def _build_jango_group(self):
        group = QGroupBox("잔고")
        v = QVBoxLayout(group)
        v.setContentsMargins(4, 4, 4, 4)
        v.setSpacing(2)

        self.jango_tabs = QTabWidget()
        v.addWidget(self.jango_tabs)

        jango_widget = QWidget()
        jv = QVBoxLayout(jango_widget)
        jv.setContentsMargins(0, 0, 0, 0)
        jv.setSpacing(2)

        h = QHBoxLayout()
        btn_jango_scan = QPushButton("보유종목 스캔(일봉 크로스오버)")
        btn_jango_scan.clicked.connect(self._on_jango_scan)
        h.addWidget(btn_jango_scan)
        h.addStretch()
        jv.addLayout(h)

        cols = ["종목코드", "종목명", "잔고수량", "가능수량", "현재가", "평가금액", "평가손익", "수익률(%)"]
        self.table_jango = QTableWidget(0, len(cols))
        self.table_jango.setHorizontalHeaderLabels(cols)
        self.table_jango.verticalHeader().setVisible(False)
        self.table_jango.verticalHeader().setDefaultSectionSize(18)
        self.table_jango.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table_jango.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.table_jango.horizontalHeader().setStretchLastSection(True)
        self.table_jango.horizontalHeader().setSectionResizeMode(1, QHeaderView.Stretch)
        self.table_jango.setFixedHeight(140)  # 헤더 + 6줄 고정
        self.table_jango.cellDoubleClicked.connect(self._on_jango_double_clicked)
        jv.addWidget(self.table_jango)
        self.jango_tabs.addTab(jango_widget, "잔고")

        self.table_chegyul = self._build_chegyul_table()
        self.jango_tabs.addTab(self.table_chegyul, "체결")

        self.table_michegyul = self._build_chegyul_table()
        self.jango_tabs.addTab(self.table_michegyul, "미체결")

        return group

    def _build_chegyul_table(self):
        # TR1211(주식 체결/미체결조회) 응답 그리드를 그대로 표로 보여준다.
        cols = ["주문번호", "원주문번호", "시장", "종목코드", "종목명", "구분",
                "주문가", "주문량", "체결가", "체결량", "미체결", "상태", "시간"]
        t = QTableWidget(0, len(cols))
        t.setHorizontalHeaderLabels(cols)
        t.verticalHeader().setVisible(False)
        t.verticalHeader().setDefaultSectionSize(18)
        t.setEditTriggers(QAbstractItemView.NoEditTriggers)
        t.setSelectionBehavior(QAbstractItemView.SelectRows)
        t.setFixedHeight(140)
        return t

    def _add_jango_row(self, code, remain, avail, maip, curr, eval_amt, rate):
        try:
            pnl = f"{float(eval_amt) - float(maip):.0f}"
        except ValueError:
            pnl = "-"
        code_key = code.split('.')[-1].lstrip('A')  # "M.A010140"처럼 시장접두사가 붙는 경우 대응
        row = self.table_jango.rowCount()
        self.table_jango.insertRow(row)
        vals = [code, self._stock_name(code_key), _fmt_num(remain), _fmt_num(avail),
                _fmt_num(curr), _fmt_num(eval_amt), _fmt_num(pnl), rate]
        for col, val in enumerate(vals):
            self.table_jango.setItem(row, col, QTableWidgetItem(val))
        self._jango_row_by_code[code_key] = row
        # 실시간 재계산용 기준값 저장 (서버가 계산한 초기 평가금액을 기준점으로 삼아
        # 현재가 변동분만큼만 보정 -> 수수료/세금을 몰라도 근사치가 아닌 정확한 값 유지)
        try:
            # curr(현재가)의 +/-는 등락 방향 표시일 뿐 실제 부호가 아니므로 양수로 취급
            self._jango_calc[code_key] = {
                'remain': float(remain), 'maip': float(maip),
                'base_curr': float(curr.lstrip('+-')), 'base_eval': float(eval_amt),
            }
        except ValueError:
            pass

    def _update_jango_row(self, code, curr=None, eval_amt=None, pnl=None, rate=None):
        row = self._jango_row_by_code.get(code.split('.')[-1].lstrip('A'))
        if row is None:
            return
        fmt = [(4, _fmt_num(curr) if curr is not None else None),
               (5, _fmt_num(eval_amt) if eval_amt is not None else None),
               (6, _fmt_num(pnl) if pnl is not None else None),
               (7, rate)]
        for col, val in fmt:
            if val is not None:
                self.table_jango.setItem(row, col, QTableWidgetItem(val))

    def _recalc_jango_on_price(self, code, new_curr_s):
        # OnRealData로 들어온 현재가로 잔고 종목의 평가금액/평가손익/수익률을 실시간 재계산
        code_key = code.split('.')[-1].lstrip('A')
        calc = self._jango_calc.get(code_key)
        if calc is None:
            return
        try:
            # 023(현재가)의 +/-는 실제 부호가 아니라 등락 방향 표시라서 가격은 항상 양수로 취급
            new_curr = float(new_curr_s.lstrip('+-'))
        except ValueError:
            return
        remain = calc['remain']
        maip = calc['maip']
        new_eval = calc['base_eval'] + (new_curr - calc['base_curr']) * remain
        pnl = new_eval - maip
        rate = pnl * 100 / maip if maip else 0.0
        self._update_jango_row(code_key, curr=f"{new_curr:.0f}", eval_amt=f"{new_eval:.0f}",
                                pnl=f"{pnl:.0f}", rate=f"{rate:.2f}")

    def _on_jango_double_clicked(self, row, col):
        item = self.table_jango.item(row, col)
        if item is None:
            return
        text = item.text().strip()
        if col == 0:  # 종목코드 -> 주문 종목코드 ('A' 접두문자 포함 그대로)
            self.edit_odr_code.setText(text)
        elif col == 4:  # 현재가 -> 주문가격 (부호와 콤마 제거)
            self.edit_odr_jprc.setText(text.lstrip('+-').replace(',', ''))

    def _build_collect_group(self):
        group = QGroupBox("데이터수집 (틱 저장)")
        h = QHBoxLayout(group)
        h.addWidget(QLabel("구독종목(콤마구분)"))
        self.edit_watch_codes = QLineEdit()
        h.addWidget(self.edit_watch_codes)
        self._add_code_picker(h, self.edit_watch_codes)
        h.addWidget(QLabel("시장"))
        self.combo_watch_mkgubn = QComboBox()
        for text, val in [("KRX", 1), ("NXT", 2), ("통합", 3)]:
            self.combo_watch_mkgubn.addItem(text, val)
        self.combo_watch_mkgubn.setCurrentIndex(2)
        h.addWidget(self.combo_watch_mkgubn)
        btn = QPushButton("구독시작")
        btn.clicked.connect(self._on_watch_start)
        h.addWidget(btn)
        btn_stop = QPushButton("구독중지")
        btn_stop.clicked.connect(self._on_watch_stop)
        h.addWidget(btn_stop)
        self.lbl_watch_status = QLabel("구독중: 0개")
        h.addWidget(self.lbl_watch_status)
        h.addStretch()
        return group

    def _build_log_group(self):
        group = QGroupBox("Log")
        v = QVBoxLayout(group)
        h = QHBoxLayout()
        btn_clear = QPushButton("로그 지우기")
        btn_clear.clicked.connect(lambda: self.log_edit.clear())
        h.addWidget(btn_clear)
        h.addStretch()
        v.addLayout(h)
        self.log_edit = QTextEdit()
        self.log_edit.setReadOnly(True)
        self.log_edit.setMinimumHeight(150)
        v.addWidget(self.log_edit)
        return group

    # ── button handlers ──────────────────────────────────────────

    def _log(self, msg):
        ts = datetime.now().strftime("%H:%M:%S")
        self.log_edit.append(f"[{ts}] {msg}")
        ctypes.windll.kernel32.OutputDebugStringW(f"[IBKS][{ts}] {msg}")

    def _log_err(self, err):
        self._log(f"  오류: {err}")
        if err and "제한" in err:
            if self.chk_auto_trade.isChecked() and not self.chk_auto_order_confirm.isChecked():
                return  # 자동매매 중(주문확인창 꺼짐)엔 흐름을 막지 않도록 로그만 남긴다
            QMessageBox.warning(self, "요청 제한", err)

    def _on_initialize(self):
        result = self.ocx.dynamicCall("Initialize()")
        self.lbl_init.setText(str(result))
        self._log(f"Initialize() => {result}")
        self._log(f"OCX path: {self._get_ocx_path()}")
        for name, progid in DEPENDENT_OCX_PROGIDS.items():
            self._log(f"  {name} path: {self._get_ocx_path_by_progid(progid)}")
        if not result:
            err = self.ocx.dynamicCall("GetLastErrMsg()")
            self._log(f"  Initialize 실패: {err}")
            QMessageBox.critical(
                None, "초기화 실패",
                "Initialize()가 실패했습니다 — IBKSConnector.ocx가 필요한 컴포넌트(axWizard/axSock/axCertify/axXecure)를\n"
                "자체 등록하는 과정에서 문제가 발생했을 가능성이 높습니다.\n\n"
                f"상세: {err}\n\n"
                "해결 방법:\n"
                "1. 이 프로그램을 관리자 권한으로 다시 실행해주세요.\n"
                "2. 관리자 권한으로도 계속 실패하면 exe 폴더의 해당 OCX 파일이 손상/누락되지 않았는지 확인하세요."
            )
            sys.exit(1)

    def _get_ocx_path(self):
        reg_key = f"CLSID\\{OCX_GUID}\\InprocServer32"
        try:
            key = winreg.OpenKey(winreg.HKEY_CLASSES_ROOT, reg_key)
            path, _ = winreg.QueryValueEx(key, "")
            winreg.CloseKey(key)
            return path
        except Exception as e:
            return f"(registry read failed: {e})"

    def _get_ocx_path_by_progid(self, progid):
        # ProgID -> CLSID -> InprocServer32 순으로 조회 (C++ CLSIDFromProgID와 동일한 경로)
        try:
            clsid_key = winreg.OpenKey(winreg.HKEY_CLASSES_ROOT, f"{progid}\\CLSID")
            clsid, _ = winreg.QueryValueEx(clsid_key, "")
            winreg.CloseKey(clsid_key)
            path_key = winreg.OpenKey(winreg.HKEY_CLASSES_ROOT, f"CLSID\\{clsid}\\InprocServer32")
            path, _ = winreg.QueryValueEx(path_key, "")
            winreg.CloseKey(path_key)
            return path
        except Exception as e:
            return f"(registry read failed: {e})"

    def _on_login(self):
        user_id  = self.edit_user_id.text()
        user_pw  = self.edit_user_pw.text()
        cert_pw  = self.edit_cert_pw.text()
        svr_ip   = self.edit_svr_ip.text()
        svr_port = int(self.edit_svr_port.text() or "0")

        if not svr_ip.strip():
            QMessageBox.warning(self, "서버 미선택", "서버를 선택하거나 svr_ip를 입력하세요.")
            return

        if self.chk_quote_only.isChecked():
            result = self.ocx.dynamicCall(
                "LoginQuote(QString, QString, QString, int)",
                user_id, user_pw, svr_ip, svr_port
            )
            self.lbl_login.setText(str(result))
            self._log(f"LoginQuote({user_id}, ***, {svr_ip}, {svr_port}) => {result}")
            return

        result = self.ocx.dynamicCall(
            "Login(QString, QString, QString, QString, int)",
            user_id, user_pw, cert_pw, svr_ip, svr_port
        )
        self.lbl_login.setText(str(result))
        self._log(f"Login({user_id}, ***, ***, {svr_ip}, {svr_port}) => {result}")

    def _on_logout(self):
        self.ocx.dynamicCall("Logout()")
        self.lbl_login.setText("-")
        self._log("Logout() called")

    def _on_server_selected(self, idx):
        ip = self.combo_server.itemData(idx)
        if ip:
            self.edit_svr_ip.setText(ip)

    def _on_sise_send(self):
        code = self.edit_sise_code.text().strip()
        mkgubn = self.combo_mkgubn.currentData()  # 1=KRX 2=NXT 3=통합
        for lbl in (self.lbl_curr, self.lbl_diff, self.lbl_rate, self.lbl_gvol,
                    self.lbl_siga, self.lbl_koga, self.lbl_jega, self.lbl_mgjv,
                    self.lbl_dvol, self.lbl_svol):
            lbl.setText("-")
        if self.combo_jtype.currentIndex() == 0:  # 선물옵션
            if len(code) != 8:
                self._log("선물옵션 종목코드는 8자리입니다.")
                return
            result = self.ocx.dynamicCall("TR3001(int, QString)", TK_TR3001, code)
        else:  # 주식
            if len(code) != 6:
                self._log("주식 종목코드는 6자리입니다.")
                return
            # KRX/NXT/통합 전부 TR1002로 통일 (1777 + 요청필드 목록 전송)
            # -> KRX 결과를 기존 TR1001 값과 직접 비교해서 파싱 검증용
            fields = "\t".join(TR1002_SISE_FIELDS)
            symb = f"1777\x7f{mkgubn}\t{fields}\t"
            result = self.ocx.dynamicCall(
                "TR1002(int, QString, QString)", [TK_TR1002, code, symb])
        self._log(f"시세조회 요청({code}, 시장={mkgubn}) => {result}")
        if not result:
            err = self.ocx.dynamicCall("GetLastErrMsg()")
            self._log_err(err)

    def _on_watch_start(self):
        mkgubn = self.combo_watch_mkgubn.currentData()  # 1=KRX 2=NXT 3=통합
        # TR1001은 시장구분 파라미터가 없어 KRX 실시간만 등록된다.
        # TR1002를 종목별로 반복 호출하면 서버가 매 호출마다 이전 등록을 새 등록으로
        # 덮어써서 마지막 종목만 실시간이 살아있는 문제가 있었다 -> 다중종목 등록5
        # TR1004(콤마구분 종목코드 전체를 한 번에 전송)로 전환.
        fields = "\t".join(TR1002_SISE_FIELDS)
        codes = [c.strip() for c in self.edit_watch_codes.text().split(',') if c.strip()]
        new_codes = []
        for code in codes:
            if len(code) != 6:
                self._log(f"구독 실패(종목코드 6자리 아님): {code}")
                continue
            if code in self.watch_codes:
                continue
            new_codes.append(code)

        if not new_codes:
            self.lbl_watch_status.setText(f"구독중: {len(self.watch_codes)}개")
            return

        symb = f"1777\x7f{mkgubn}\t{fields}\t"
        codes_str = ",".join(new_codes)
        result = self.ocx.dynamicCall(
            "TR1004(int, QString, QString)", [TK_TR1004, codes_str, symb])
        if result:
            self.watch_codes.update(new_codes)
            self._log(f"구독 등록: {codes_str} (시장={mkgubn})")
        else:
            err = self.ocx.dynamicCall("GetLastErrMsg()")
            self._log(f"구독 실패({codes_str}): {err}")
        self.lbl_watch_status.setText(f"구독중: {len(self.watch_codes)}개")

    def _on_watch_stop(self):
        # 서버에 등록 해제를 요청하는 TR은 아직 없어서, 로컬에서만 watch_codes에서
        # 빼서 이후 OnRealData가 와도 틱을 저장하지 않게 하고, 다시 구독시작을 누르면
        # 새 종목으로 취급되어 TR1004가 재전송되게 한다.
        codes = [c.strip() for c in self.edit_watch_codes.text().split(',') if c.strip()]
        removed = [c for c in codes if c in self.watch_codes]
        for c in removed:
            self.watch_codes.discard(c)
        if removed:
            self._log(f"구독 중지: {','.join(removed)}")
        else:
            self._log("구독 중지: 대상 없음 (구독중인 종목이 아님)")
        self.lbl_watch_status.setText(f"구독중: {len(self.watch_codes)}개")

    def _build_daily_group(self):
        group = QGroupBox("일/주/월별 시세조회")
        h = QHBoxLayout(group)
        h.addWidget(QLabel("종목코드"))
        self.edit_daily_code = QLineEdit()
        self.edit_daily_code.setMaximumWidth(90)
        h.addWidget(self.edit_daily_code)
        self._add_code_picker(h, self.edit_daily_code)
        h.addWidget(QLabel("구분"))
        self.combo_daily_period = QComboBox()
        for text, val in [("일", "0"), ("주", "1"), ("월", "2")]:
            self.combo_daily_period.addItem(text, val)
        h.addWidget(self.combo_daily_period)
        h.addWidget(QLabel("시장"))
        self.combo_daily_mkgubn = QComboBox()
        for text, val in [("KRX", 1), ("NXT", 2), ("통합", 3)]:
            self.combo_daily_mkgubn.addItem(text, val)
        self.combo_daily_mkgubn.setCurrentIndex(2)
        h.addWidget(self.combo_daily_mkgubn)
        btn = QPushButton("조회")
        btn.clicked.connect(self._on_daily_send)
        h.addWidget(btn)
        h.addStretch()
        return group

    def _on_daily_send(self):
        code = self.edit_daily_code.text().strip()
        if not code:
            self._log("일자별시세: 종목코드를 입력하세요")
            return
        period = self.combo_daily_period.currentData()  # 0=일 1=주 2=월 ($1250{period} 마커에 대응)
        mkgubn = self.combo_daily_mkgubn.currentData()
        base_date = datetime.now().strftime("%Y%m%d")
        fields = "\t".join(TR1005_DAILY_FIELDS)
        symb = f"{period}\x7f{base_date}\t1777\x7f{mkgubn}\t{fields}\t"
        result = self.ocx.dynamicCall(
            "TR1005(int, QString, QString)", [TK_TR1005, code, symb])
        self._log(f"일자별시세 요청({code}, 구분={period}, 시장={mkgubn}) => {result}")
        if not result:
            err = self.ocx.dynamicCall("GetLastErrMsg()")
            self._log_err(err)

    def _build_market_group(self):
        group = QGroupBox("주요 시장지표(TR1006) 조회")
        h = QHBoxLayout(group)
        btn = QPushButton("조회")
        btn.clicked.connect(self._on_market_send)
        h.addWidget(btn)
        h.addStretch()
        return group

    def _on_market_send(self):
        result = self.ocx.dynamicCall("TR1006(int, QString)", [TK_TR1006, "1"])
        self._log(f"시장지표 요청 => {result}")
        if not result:
            err = self.ocx.dynamicCall("GetLastErrMsg()")
            self._log_err(err)

    def _build_history_group(self):
        group = QGroupBox("차트(일/주/월봉) 조회")
        h = QHBoxLayout(group)
        h.addWidget(QLabel("대상"))
        self.combo_history_unit = QComboBox()
        for text, val in [("종목", 1), ("업종", 2)]:
            self.combo_history_unit.addItem(text, val)
        h.addWidget(self.combo_history_unit)
        h.addWidget(QLabel("코드"))
        self.edit_history_code = QLineEdit()
        self.edit_history_code.setMaximumWidth(90)
        h.addWidget(self.edit_history_code)
        self._add_code_picker(h, self.edit_history_code)
        h.addWidget(QLabel("구분"))
        self.combo_history_period = QComboBox()
        for text, val in [("일", 1), ("주", 2), ("월", 3)]:
            self.combo_history_period.addItem(text, val)
        h.addWidget(self.combo_history_period)
        h.addWidget(QLabel("건수"))
        self.edit_history_count = QLineEdit("200")
        self.edit_history_count.setMaximumWidth(50)
        h.addWidget(self.edit_history_count)
        h.addWidget(QLabel("시장"))
        self.combo_history_mkgubn = QComboBox()
        for text, val in [("KRX", 1), ("NXT", 2), ("통합", 3)]:
            self.combo_history_mkgubn.addItem(text, val)
        self.combo_history_mkgubn.setCurrentIndex(2)
        h.addWidget(self.combo_history_mkgubn)
        btn = QPushButton("조회")
        btn.clicked.connect(self._on_history_send)
        h.addWidget(btn)
        btn_batch = QPushButton("일괄수집")
        btn_batch.setToolTip("코드란에 콤마(,)로 여러 개를 넣으면 1.2초 간격으로 순서대로 조회해 DB에 저장합니다")
        btn_batch.clicked.connect(self._on_history_batch_start)
        h.addWidget(btn_batch)
        btn_manage = QPushButton("종목 관리")
        btn_manage.setToolTip("daily_candles에 쌓인 종목을 선택해서 갱신(재수집)하거나 삭제합니다")
        btn_manage.clicked.connect(self._on_open_data_manage)
        h.addWidget(btn_manage)
        self.lbl_history_batch = QLabel("")
        h.addWidget(self.lbl_history_batch)
        h.addStretch()
        return group

    def _on_open_data_manage(self):
        DataManageDialog(self).exec_()

    def _on_history_send(self):
        code = self.edit_history_code.text().strip()
        if not code:
            self._log("차트조회: 코드를 입력하세요")
            return
        self._request_history(code)

    def _request_history(self, code):
        dunit = self.combo_history_unit.currentData()  # 1=종목 2=업종
        dindex = self.combo_history_period.currentData()  # 1=일 2=주 3=월
        count = self.edit_history_count.text().strip() or "200"
        mkgubn = self.combo_history_mkgubn.currentData()
        pday = datetime.now().strftime("%Y%m%d")
        symb = f"{dunit}\t{dindex}\x7f{pday}\t{count}\t1777\x7f{mkgubn}\t"
        self._history_last_dunit = dunit
        self._history_last_dindex = dindex
        self._history_last_code = code
        result = self.ocx.dynamicCall(
            "TR1007(int, QString, QString)", [TK_TR1007, code, symb])
        self._log(f"차트조회 요청({code}, 대상={dunit}, 구분={dindex}, 건수={count}, 시장={mkgubn}) => {result}")
        if not result:
            err = self.ocx.dynamicCall("GetLastErrMsg()")
            self._log_err(err)

    def _on_history_batch_start(self):
        # eTR1007 요청 제한(1건/1000ms)을 넘지 않도록 1.2초 간격 QTimer로 순차 조회한다.
        codes = [c.strip() for c in self.edit_history_code.text().split(',') if c.strip()]
        if not codes:
            self._log("차트 일괄수집: 코드를 콤마(,)로 구분해 입력하세요")
            return
        self._history_batch_queue = codes
        self._history_batch_total = len(codes)
        self._on_history_batch_tick()
        self._history_batch_timer.start(1200)

    def _on_history_batch_tick(self):
        if not self._history_batch_queue:
            self._history_batch_timer.stop()
            self.lbl_history_batch.setText("수집 완료")
            return
        code = self._history_batch_queue.pop(0)
        done = self._history_batch_total - len(self._history_batch_queue)
        self.lbl_history_batch.setText(f"수집중: {done}/{self._history_batch_total} ({code})")
        self._request_history(code)

    def _save_history_records(self, code, dunit, dindex, records):
        # extra1/extra2 의미: 종목=권리락/수정비율, 업종=상승종목수(추정)/하락종목수(추정)
        now = datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")
        rows = []
        for vals in records:
            try:
                rows.append((
                    code, dunit, dindex, vals[0],
                    int(vals[2]), int(vals[3]), int(vals[4]), int(vals[5]),
                    int(vals[6]), int(vals[7]), float(vals[8]), float(vals[9]),
                    now,
                ))
            except (ValueError, IndexError):
                continue
        if not rows:
            return 0
        try:
            self.db_conn.executemany(
                "INSERT OR REPLACE INTO daily_candles "
                "(code, unit, dindex, date, open, high, low, close, volume, amount, extra1, extra2, received_at) "
                "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
                rows
            )
            self.db_conn.commit()
        except sqlite3.Error as e:
            self._log(f"캔들 저장 오류({code}): {e}")
            return 0
        return len(rows)

    def _build_daily_backtest_group(self):
        group = QGroupBox("일봉 백테스트 (daily_candles 기반)")
        v = QVBoxLayout(group)

        h1 = QHBoxLayout()
        h1.addWidget(QLabel("종목코드"))
        self.edit_bt_code = QLineEdit()
        self.edit_bt_code.setMaximumWidth(90)
        h1.addWidget(self.edit_bt_code)
        self._add_code_picker(h1, self.edit_bt_code)
        self.lbl_bt_name = QLabel("")
        self.lbl_bt_name.setMinimumWidth(60)
        h1.addWidget(self.lbl_bt_name)
        self.edit_bt_code.textChanged.connect(
            lambda text: self.lbl_bt_name.setText(self._stock_name(text.strip())))
        btn_related = QPushButton("관련종목")
        btn_related.setToolTip("이 종목이 속한 업종/테마/그룹사에서 다른 종목 찾기")
        btn_related.clicked.connect(self._on_find_related)
        h1.addWidget(btn_related)
        h1.addWidget(QLabel("단기"))
        self.edit_bt_short = QLineEdit("5")
        self.edit_bt_short.setMaximumWidth(40)
        h1.addWidget(self.edit_bt_short)
        h1.addWidget(QLabel("장기"))
        self.edit_bt_long = QLineEdit("20")
        self.edit_bt_long.setMaximumWidth(40)
        h1.addWidget(self.edit_bt_long)
        self.chk_bt_regime = QCheckBox("코스피(001)")
        h1.addWidget(self.chk_bt_regime)
        self.edit_bt_regime_days = QLineEdit("20")
        self.edit_bt_regime_days.setMaximumWidth(40)
        h1.addWidget(self.edit_bt_regime_days)
        h1.addWidget(QLabel("일선 필터"))
        btn = QPushButton("분석")
        btn.clicked.connect(self._on_daily_backtest_send)
        h1.addWidget(btn)
        h1.addStretch()
        v.addLayout(h1)

        h1b = QHBoxLayout()
        h1b.addWidget(QLabel("차트 기간"))
        self.edit_bt_date_from = QDateEdit(QDate.currentDate().addMonths(-3))
        self.edit_bt_date_from.setCalendarPopup(True)
        self.edit_bt_date_from.setDisplayFormat("yyyy-MM-dd")
        h1b.addWidget(self.edit_bt_date_from)
        h1b.addWidget(QLabel("~"))
        self.edit_bt_date_to = QDateEdit(QDate.currentDate())
        self.edit_bt_date_to.setCalendarPopup(True)
        self.edit_bt_date_to.setDisplayFormat("yyyy-MM-dd")
        h1b.addWidget(self.edit_bt_date_to)
        self.chk_bt_zoom = QCheckBox("기간으로 확대")
        self.chk_bt_zoom.toggled.connect(lambda checked: self._apply_bt_date_zoom())
        self.edit_bt_date_from.dateChanged.connect(lambda d: self._apply_bt_date_zoom())
        self.edit_bt_date_to.dateChanged.connect(lambda d: self._apply_bt_date_zoom())
        h1b.addWidget(self.chk_bt_zoom)
        h1b.addStretch()
        v.addLayout(h1b)

        h1c = QHBoxLayout()
        h1c.addWidget(QLabel("표시"))
        self.chk_bt_show = {}
        for key, label in [("close", "종가"), ("short", "단기선"), ("long", "장기선"),
                            ("buy", "매수"), ("sell", "매도")]:
            chk = QCheckBox(label)
            chk.setChecked(True)
            chk.toggled.connect(self._apply_bt_series_visibility)
            self.chk_bt_show[key] = chk
            h1c.addWidget(chk)
        h1c.addStretch()
        v.addLayout(h1c)

        h2 = QHBoxLayout()
        self.bt_fig = Figure(figsize=(6, 2.2))
        self.bt_canvas = FigureCanvasQTAgg(self.bt_fig)
        h2.addWidget(self.bt_canvas, 3)

        trade_box = QVBoxLayout()
        self.lbl_bt_today = QLabel("오늘 신호: -")
        trade_box.addWidget(self.lbl_bt_today)
        self.lbl_bt_summary = QLabel("거래 없음")
        trade_box.addWidget(self.lbl_bt_summary)
        self.table_bt_trades = QTableWidget(0, 6)
        self.table_bt_trades.setHorizontalHeaderLabels(["매수일", "매수가", "매도일", "매도가", "수익률", "상태"])
        self.table_bt_trades.verticalHeader().setDefaultSectionSize(18)
        self.table_bt_trades.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table_bt_trades.setMaximumHeight(110)
        for col, w in enumerate([70, 60, 70, 60, 60, 60]):
            self.table_bt_trades.setColumnWidth(col, w)
        trade_box.addWidget(self.table_bt_trades)
        trade_widget = QWidget()
        trade_widget.setLayout(trade_box)
        h2.addWidget(trade_widget, 2)

        v.addLayout(h2)
        return group

    def _on_daily_backtest_send(self):
        code = self.edit_bt_code.text().strip()
        if not code:
            self._log("일봉 백테스트: 종목코드를 입력하세요")
            return
        try:
            short_win = int(self.edit_bt_short.text().strip() or 5)
            long_win = int(self.edit_bt_long.text().strip() or 20)
            regime_days = int(self.edit_bt_regime_days.text().strip() or 20)
        except ValueError:
            self._log("일봉 백테스트: 단기/장기 이평, 코스피 필터 일수는 숫자로 입력하세요")
            return

        self.bt_fig.clear()
        self.bt_canvas.draw()
        self.table_bt_trades.setRowCount(0)
        self.lbl_bt_today.setText("계산 중...")
        self.lbl_bt_summary.setText("계산 중...")

        stock = load_daily(self.db_conn, code, unit=1, dindex=1)
        if len(stock) < long_win + 1:
            self._log(f"일봉 백테스트: {code} 데이터 부족(레코드 {len(stock)}건, 최소 {long_win + 1}건 필요) "
                       f"- 차트조회로 먼저 모아주세요")
            return

        closes = [r["close"] for r in stock]
        short_ma = daily_sma(closes, short_win)
        long_ma = daily_sma(closes, long_win)
        signals = cross_signals(short_ma, long_ma)

        regime_ok = None
        if self.chk_bt_regime.isChecked():
            kospi = load_daily(self.db_conn, "001", unit=2, dindex=1)
            if len(kospi) < regime_days + 1:
                self._log("일봉 백테스트: 코스피(001) 데이터가 부족해 필터 없이 진행합니다 "
                           "- 차트조회(대상=업종, 코드=001)로 먼저 모아주세요")
            else:
                kospi_closes = [r["close"] for r in kospi]
                kospi_ma = daily_sma(kospi_closes, regime_days)
                kospi_regime_by_date = {
                    r["date"]: (kospi_ma[i] is not None and kospi_closes[i] > kospi_ma[i])
                    for i, r in enumerate(kospi)
                }
                regime_ok = [kospi_regime_by_date.get(r["date"], False) for r in stock]

        trades, position = daily_simulate(stock, signals, regime_ok=regime_ok)
        sig = daily_today_signal(stock, signals, position, regime_ok=regime_ok)
        hold_state = f"보유중(진입 {_fmt_date(position['entry_date'])} @ {position['entry_price']:.0f})" if position else "미보유"
        today_date = _fmt_date(stock[-1]['date'])
        if sig == "BUY":
            today_text = f"오늘({today_date}) 신호: 매수 (내일 시가 매수 검토) - 현재 {hold_state}"
        elif sig == "SELL":
            today_text = f"오늘({today_date}) 신호: 매도 (내일 시가 매도 검토) - 현재 {hold_state}"
        else:
            today_text = f"오늘({today_date}) 신호: 없음(관망) - 현재 {hold_state}"
        self.lbl_bt_today.setText(today_text)

        self.table_bt_trades.setRowCount(0)
        for t in trades:
            row = self.table_bt_trades.rowCount()
            self.table_bt_trades.insertRow(row)
            vals = [_fmt_date(t["entry_date"]), f"{t['entry_price']:.0f}", _fmt_date(t["exit_date"]),
                    f"{t['exit_price']:.0f}", f"{t['return'] * 100:+.2f}%", "완료"]
            for col, val in enumerate(vals):
                self.table_bt_trades.setItem(row, col, QTableWidgetItem(val))
        if position:  # 아직 청산 안 된 현재 보유 포지션도 마지막 줄에 표시
            row = self.table_bt_trades.rowCount()
            self.table_bt_trades.insertRow(row)
            vals = [_fmt_date(position["entry_date"]), f"{position['entry_price']:.0f}", "-", "-", "-", "보유중"]
            for col, val in enumerate(vals):
                self.table_bt_trades.setItem(row, col, QTableWidgetItem(val))

        buy_hold = (closes[-1] - closes[0]) / closes[0] * 100
        if trades:
            wins = [t for t in trades if t["return"] > 0]
            cum = 1.0
            for t in trades:
                cum *= (1 + t["return"])
            avg = sum(t["return"] for t in trades) / len(trades)
            summary = (f"거래 {len(trades)}건  승률 {len(wins) / len(trades) * 100:.1f}%  "
                       f"평균수익률 {avg * 100:+.2f}%  누적수익률 {(cum - 1) * 100:+.2f}%  "
                       f"(단순보유 {buy_hold:+.2f}%)")
        else:
            summary = f"거래 없음  (단순보유 {buy_hold:+.2f}%)"
        self.lbl_bt_summary.setText(summary)
        self._log(f"일봉 백테스트({code}, 단기={short_win}, 장기={long_win}, "
                   f"필터={'ON(' + str(regime_days) + '일)' if regime_ok else 'OFF'}) => {summary} / {today_text}")

        dates = [datetime.strptime(r["date"], "%Y%m%d") for r in stock]
        buy_x = [datetime.strptime(t["entry_date"], "%Y%m%d") for t in trades]
        buy_y = [t["entry_price"] for t in trades]
        sell_x = [datetime.strptime(t["exit_date"], "%Y%m%d") for t in trades]
        sell_y = [t["exit_price"] for t in trades]
        if position:  # 아직 청산 안 된 현재 보유 포지션도 매수 마커로 표시
            buy_x.append(datetime.strptime(position["entry_date"], "%Y%m%d"))
            buy_y.append(position["entry_price"])

        self.bt_fig.clear()
        ax = self.bt_fig.add_subplot(111)
        self.bt_artists = {
            "close": ax.plot(dates, closes, label="종가", color="black", linewidth=0.8)[0],
            "short": ax.plot(dates, short_ma, label=f"{short_win}일선", color="green", linewidth=1)[0],
            "long": ax.plot(dates, long_ma, label=f"{long_win}일선", color="red", linewidth=1)[0],
            "buy": ax.scatter(buy_x, buy_y, marker='^', color='blue', s=30, zorder=5, label="매수"),
            "sell": ax.scatter(sell_x, sell_y, marker='v', color='magenta', s=30, zorder=5, label="매도"),
        }
        ax.set_title(code)
        ax.legend(loc="upper left", fontsize=7)
        ax.xaxis.set_major_formatter(matplotlib.dates.DateFormatter('%y-%m-%d'))
        self.bt_fig.autofmt_xdate()
        self.bt_fig.tight_layout()
        self.bt_ax = ax
        self._apply_bt_series_visibility()
        self._apply_bt_date_zoom()

    def _apply_bt_date_zoom(self):
        # 이동평균/신호는 항상 전체 데이터로 계산된 상태 그대로 두고, x축 보기 범위만 좁힌다
        # (먼저 데이터를 잘라서 다시 계산하면 구간 시작부에 룩백이 부족해 결과가 달라진다).
        if self.bt_ax is None:
            return
        if self.chk_bt_zoom.isChecked():
            d_from = datetime.combine(self.edit_bt_date_from.date().toPyDate(), datetime.min.time())
            d_to = datetime.combine(self.edit_bt_date_to.date().toPyDate(), datetime.min.time())
            self.bt_ax.set_xlim(d_from, d_to)
        else:
            self.bt_ax.autoscale(enable=True, axis='x')
        self.bt_canvas.draw()

    def _apply_bt_series_visibility(self):
        # 표시 체크박스 상태를 각 시리즈(종가/단기선/장기선/매수/매도)에 반영한다.
        if not self.bt_artists:
            return
        for key, artist in self.bt_artists.items():
            artist.set_visible(self.chk_bt_show[key].isChecked())
        self.bt_canvas.draw()

    def _build_scan_group(self):
        group = QGroupBox("전체 종목 스캔 (오늘의 매수/매도 후보)")
        v = QVBoxLayout(group)

        h0 = QHBoxLayout()
        h0.addWidget(QLabel("종목 제한(비우면 전체)"))
        self.edit_scan_codes = QLineEdit()
        self.edit_scan_codes.setPlaceholderText("콤마로 구분된 종목코드 - 비우면 daily_candles 전체")
        h0.addWidget(self.edit_scan_codes)
        self._add_code_picker(h0, self.edit_scan_codes)
        v.addLayout(h0)

        h = QHBoxLayout()
        h.addWidget(QLabel("단기"))
        self.edit_scan_short = QLineEdit("5")
        self.edit_scan_short.setMaximumWidth(40)
        h.addWidget(self.edit_scan_short)
        h.addWidget(QLabel("장기"))
        self.edit_scan_long = QLineEdit("20")
        self.edit_scan_long.setMaximumWidth(40)
        h.addWidget(self.edit_scan_long)
        self.chk_scan_regime = QCheckBox("코스피(001)")
        h.addWidget(self.chk_scan_regime)
        self.edit_scan_regime_days = QLineEdit("20")
        self.edit_scan_regime_days.setMaximumWidth(40)
        h.addWidget(self.edit_scan_regime_days)
        h.addWidget(QLabel("일선 필터"))
        self.chk_scan_state = QCheckBox("상태기반 포함(오늘 신호 없어도 상승국면 표시)")
        h.addWidget(self.chk_scan_state)
        btn = QPushButton("스캔")
        btn.clicked.connect(self._on_scan_send)
        h.addWidget(btn)
        h.addStretch()
        v.addLayout(h)

        self.lbl_scan_summary = QLabel("스캔 결과 없음")
        v.addWidget(self.lbl_scan_summary)

        self.table_scan = QTableWidget(0, 5)
        self.table_scan.setHorizontalHeaderLabels(["종목코드", "종목명", "신호", "상태", "갭%"])
        self.table_scan.verticalHeader().setVisible(False)
        self.table_scan.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table_scan.cellDoubleClicked.connect(self._on_scan_row_select)
        v.addWidget(self.table_scan)
        lbl_scan_hint = QLabel("행 더블클릭 -> 파라미터 스캔 탭에서 이 종목의 단기/장기 조합 비교")
        v.addWidget(lbl_scan_hint)
        return group

    def _on_scan_send(self):
        codes_text = self.edit_scan_codes.text().strip()
        if codes_text:
            codes = [c.strip() for c in codes_text.split(',') if c.strip()]
            label = f"선택 {len(codes)}종목"
        else:
            cur = self.db_conn.execute(
                "SELECT DISTINCT code FROM daily_candles WHERE unit = 1 AND dindex = 1 ORDER BY code")
            codes = [r[0] for r in cur.fetchall()]
            label = "전체"
        if not codes:
            self._log("전체 스캔: daily_candles에 종목 데이터가 없습니다 - 차트조회/일괄수집으로 먼저 모아주세요")
            return
        self._run_scan(codes, label)

    def _on_jango_scan(self):
        codes = list(self._jango_row_by_code.keys())
        if not codes:
            self._log("보유종목 스캔: 잔고에 종목이 없습니다 - 잔고조회를 먼저 해주세요")
            return
        self.strategy_tabs.setCurrentIndex(2)  # 전체 스캔 탭
        self._run_scan(codes, "보유종목")

    def _run_scan(self, codes, label):
        try:
            short_win = int(self.edit_scan_short.text().strip() or 5)
            long_win = int(self.edit_scan_long.text().strip() or 20)
            regime_days = int(self.edit_scan_regime_days.text().strip() or 20)
        except ValueError:
            self._log("전체 스캔: 단기/장기 이평, 코스피 필터 일수는 숫자로 입력하세요")
            return

        self.table_scan.setRowCount(0)
        self.lbl_scan_summary.setText("스캔 중...")
        QApplication.processEvents()

        use_regime = self.chk_scan_regime.isChecked()
        kospi_regime_by_date = {}
        if use_regime:
            kospi = load_daily(self.db_conn, "001", unit=2, dindex=1)
            if len(kospi) < regime_days + 1:
                self._log("전체 스캔: 코스피(001) 데이터가 부족해 필터 없이 진행합니다")
                use_regime = False
            else:
                kospi_closes = [r["close"] for r in kospi]
                kospi_ma = daily_sma(kospi_closes, regime_days)
                kospi_regime_by_date = {
                    r["date"]: (kospi_ma[i] is not None and kospi_closes[i] > kospi_ma[i])
                    for i, r in enumerate(kospi)
                }

        use_state = self.chk_scan_state.isChecked()
        self.table_scan.setRowCount(0)
        buy_count = sell_count = state_count = 0
        no_data_codes = []
        for code in codes:
            stock = load_daily(self.db_conn, code, unit=1, dindex=1)
            if len(stock) < long_win + 1:
                no_data_codes.append(code)
                continue
            closes = [r["close"] for r in stock]
            short_ma = daily_sma(closes, short_win)
            long_ma = daily_sma(closes, long_win)
            signals = cross_signals(short_ma, long_ma)
            regime_ok = [kospi_regime_by_date.get(r["date"], False) for r in stock] if use_regime else None
            _, position = daily_simulate(stock, signals, regime_ok=regime_ok)
            sig = daily_today_signal(stock, signals, position, regime_ok=regime_ok)
            gap_pct = (short_ma[-1] - long_ma[-1]) / long_ma[-1] * 100

            if sig is None and use_state and short_ma[-1] > long_ma[-1]:
                sig = "상태:상승국면"
                state_count += 1
            elif sig is None:
                continue
            elif sig == "BUY":
                buy_count += 1
            else:
                sell_count += 1

            row = self.table_scan.rowCount()
            self.table_scan.insertRow(row)
            state = f"보유중 @ {position['entry_price']:.0f}" if position else "미보유"
            for col, val in enumerate([code, self._stock_name(code), sig, state, f"{gap_pct:.2f}"]):
                self.table_scan.setItem(row, col, QTableWidgetItem(val))

        self.lbl_scan_summary.setText(
            f"스캔({label}) {len(codes)}종목 중 매수후보 {buy_count}개, 매도후보 {sell_count}개"
            + (f", 상승국면 {state_count}개" if use_state else ""))
        if no_data_codes:
            self._log(f"스캔({label}): 데이터 부족으로 제외된 종목 - {', '.join(no_data_codes)} "
                       f"(일괄수집으로 먼저 일봉을 모아주세요)")
        self._log(f"스캔({label}, 단기={short_win}, 장기={long_win}, "
                   f"필터={'ON(' + str(regime_days) + '일)' if use_regime else 'OFF'}) "
                   f"=> {len(codes)}종목 중 매수 {buy_count} / 매도 {sell_count}"
                   + (f" / 상승국면 {state_count}" if use_state else ""))

    def _on_scan_row_select(self, row, column):
        # 전체 스캔 표에서 종목을 더블클릭하면 파라미터 스캔 탭으로 넘어가
        # 그 종목의 단기/장기 조합별 백테스트를 바로 비교해볼 수 있게 한다.
        code = self.table_scan.item(row, 0).text()
        self.edit_pscan_code.setText(code)
        self.chk_pscan_regime.setChecked(self.chk_scan_regime.isChecked())
        self.edit_pscan_regime_days.setText(self.edit_scan_regime_days.text())
        self.strategy_tabs.setCurrentIndex(3)  # 파라미터 스캔 탭
        self._on_param_scan_send()

    def _build_param_scan_group(self):
        group = QGroupBox("파라미터 스캔 (단기/장기 이평 조합별 백테스트 비교)")
        v = QVBoxLayout(group)

        h1 = QHBoxLayout()
        h1.addWidget(QLabel("종목코드"))
        self.edit_pscan_code = QLineEdit()
        self.edit_pscan_code.setMaximumWidth(90)
        h1.addWidget(self.edit_pscan_code)
        self._add_code_picker(h1, self.edit_pscan_code)
        self.lbl_pscan_name = QLabel("")
        self.lbl_pscan_name.setMinimumWidth(60)
        h1.addWidget(self.lbl_pscan_name)
        self.edit_pscan_code.textChanged.connect(
            lambda text: self.lbl_pscan_name.setText(self._stock_name(text.strip())))
        h1.addWidget(QLabel("단기"))
        self.edit_pscan_short_from = QLineEdit("3")
        self.edit_pscan_short_from.setMaximumWidth(35)
        h1.addWidget(self.edit_pscan_short_from)
        h1.addWidget(QLabel("~"))
        self.edit_pscan_short_to = QLineEdit("15")
        self.edit_pscan_short_to.setMaximumWidth(35)
        h1.addWidget(self.edit_pscan_short_to)
        h1.addWidget(QLabel("step"))
        self.edit_pscan_short_step = QLineEdit("1")
        self.edit_pscan_short_step.setMaximumWidth(30)
        h1.addWidget(self.edit_pscan_short_step)
        h1.addWidget(QLabel("장기"))
        self.edit_pscan_long_from = QLineEdit("15")
        self.edit_pscan_long_from.setMaximumWidth(35)
        h1.addWidget(self.edit_pscan_long_from)
        h1.addWidget(QLabel("~"))
        self.edit_pscan_long_to = QLineEdit("60")
        self.edit_pscan_long_to.setMaximumWidth(35)
        h1.addWidget(self.edit_pscan_long_to)
        h1.addWidget(QLabel("step"))
        self.edit_pscan_long_step = QLineEdit("5")
        self.edit_pscan_long_step.setMaximumWidth(30)
        h1.addWidget(self.edit_pscan_long_step)
        h1.addStretch()
        v.addLayout(h1)

        h2 = QHBoxLayout()
        self.chk_pscan_regime = QCheckBox("코스피(001)")
        h2.addWidget(self.chk_pscan_regime)
        self.edit_pscan_regime_days = QLineEdit("20")
        self.edit_pscan_regime_days.setMaximumWidth(40)
        h2.addWidget(self.edit_pscan_regime_days)
        h2.addWidget(QLabel("일선 필터"))
        btn = QPushButton("스캔")
        btn.clicked.connect(self._on_param_scan_send)
        h2.addWidget(btn)
        h2.addStretch()
        v.addLayout(h2)

        self.lbl_pscan_summary = QLabel("스캔 결과 없음")
        v.addWidget(self.lbl_pscan_summary)

        self.table_pscan = QTableWidget(0, 6)
        self.table_pscan.setHorizontalHeaderLabels(
            ["단기", "장기", "거래횟수", "승률(%)", "평균수익률(%)", "누적수익률(%)"])
        self.table_pscan.verticalHeader().setVisible(False)
        self.table_pscan.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table_pscan.setSortingEnabled(True)
        self.table_pscan.cellDoubleClicked.connect(self._on_pscan_row_select)
        v.addWidget(self.table_pscan)
        lbl_hint = QLabel("행 더블클릭 -> 일봉 크로스오버 탭에서 해당 조합의 매수/매도 날짜 상세 확인")
        v.addWidget(lbl_hint)
        return group

    def _on_param_scan_send(self):
        code = self.edit_pscan_code.text().strip()
        if not code:
            self._log("파라미터 스캔: 종목코드를 입력하세요")
            return
        try:
            short_from = int(self.edit_pscan_short_from.text().strip())
            short_to = int(self.edit_pscan_short_to.text().strip())
            short_step = int(self.edit_pscan_short_step.text().strip())
            long_from = int(self.edit_pscan_long_from.text().strip())
            long_to = int(self.edit_pscan_long_to.text().strip())
            long_step = int(self.edit_pscan_long_step.text().strip())
            regime_days = int(self.edit_pscan_regime_days.text().strip() or 20)
        except ValueError:
            self._log("파라미터 스캔: 단기/장기 범위·step, 코스피 필터 일수는 숫자로 입력하세요")
            return

        stock = load_daily(self.db_conn, code, unit=1, dindex=1)
        max_long = long_to
        if len(stock) < max_long + 1:
            msg = (f"{code} 데이터 부족(레코드 {len(stock)}건, 장기 최대값 {max_long} -> 최소 {max_long + 1}건 필요) "
                   f"- 장기 범위를 줄이거나 차트조회로 데이터를 더 모아주세요")
            self.table_pscan.setRowCount(0)
            self.lbl_pscan_summary.setText(msg)
            self._log(f"파라미터 스캔: {msg}")
            return
        closes = [r["close"] for r in stock]

        regime_ok = None
        if self.chk_pscan_regime.isChecked():
            kospi = load_daily(self.db_conn, "001", unit=2, dindex=1)
            if len(kospi) < regime_days + 1:
                self._log("파라미터 스캔: 코스피(001) 데이터가 부족해 필터 없이 진행합니다")
            else:
                kospi_closes = [r["close"] for r in kospi]
                kospi_ma = daily_sma(kospi_closes, regime_days)
                kospi_regime_by_date = {
                    r["date"]: (kospi_ma[i] is not None and kospi_closes[i] > kospi_ma[i])
                    for i, r in enumerate(kospi)
                }
                regime_ok = [kospi_regime_by_date.get(r["date"], False) for r in stock]

        self.table_pscan.setSortingEnabled(False)
        self.table_pscan.setRowCount(0)
        self.lbl_pscan_summary.setText("스캔 중...")
        QApplication.processEvents()

        combo_count = 0
        for short_win in range(short_from, short_to + 1, short_step):
            short_ma = daily_sma(closes, short_win)
            for long_win in range(long_from, long_to + 1, long_step):
                if short_win >= long_win:
                    continue
                combo_count += 1
                long_ma = daily_sma(closes, long_win)
                signals = cross_signals(short_ma, long_ma)
                trades, _ = daily_simulate(stock, signals, regime_ok=regime_ok)
                if not trades:
                    continue
                wins = [t for t in trades if t["return"] > 0]
                cum = 1.0
                for t in trades:
                    cum *= (1 + t["return"])
                avg = sum(t["return"] for t in trades) / len(trades)
                row = self.table_pscan.rowCount()
                self.table_pscan.insertRow(row)
                vals = [short_win, long_win, len(trades), len(wins) / len(trades) * 100,
                        avg * 100, (cum - 1) * 100]
                for col, v_ in enumerate(vals):
                    item = QTableWidgetItem()
                    item.setData(Qt.DisplayRole, round(v_, 2) if isinstance(v_, float) else v_)
                    self.table_pscan.setItem(row, col, item)

        self.table_pscan.setSortingEnabled(True)
        self.table_pscan.sortItems(5, Qt.DescendingOrder)  # 누적수익률 기준 내림차순
        buy_hold = (closes[-1] - closes[0]) / closes[0] * 100
        self.lbl_pscan_summary.setText(
            f"{code} - {combo_count}개 조합 중 거래 발생 {self.table_pscan.rowCount()}개 "
            f"(열 헤더 클릭으로 재정렬) / 단순보유(전체기간) {buy_hold:+.2f}%")
        self._log(f"파라미터 스캔({code}, 단기={short_from}~{short_to}/{short_step}, "
                   f"장기={long_from}~{long_to}/{long_step}) => {combo_count}개 조합 스캔 완료")

    def _on_pscan_row_select(self, row, column):
        # 파라미터 스캔 표에서 조합을 더블클릭하면 일봉 크로스오버 탭으로 넘어가
        # 그 조합의 매수/매도 날짜 상세(차트+거래표)를 바로 보여준다.
        short_win = self.table_pscan.item(row, 0).text()
        long_win = self.table_pscan.item(row, 1).text()
        self.edit_bt_code.setText(self.edit_pscan_code.text().strip())
        self.edit_bt_short.setText(short_win)
        self.edit_bt_long.setText(long_win)
        self.chk_bt_regime.setChecked(self.chk_pscan_regime.isChecked())
        self.edit_bt_regime_days.setText(self.edit_pscan_regime_days.text())
        self.strategy_tabs.setCurrentIndex(1)  # 일봉 크로스오버 탭
        self._on_daily_backtest_send()

    # ── OCX event handlers ───────────────────────────────────────

    def _evt_login(self, b_login, msg):
        status = "OK" if b_login else "FAIL"
        self.lbl_login.setText(status)
        self._log(f"[EVT] OnLogin({status}) {msg}")

        if b_login:
            home = self.ocx.dynamicCall("GetHome()")
            self._log(f"  OCX 홈 경로: {home}")
            self._load_accounts()

    def _load_accounts(self):
        # GetAccounts() returns "계좌번호\t계좌명\n계좌번호\t계좌명" format
        raw = self.ocx.dynamicCall("GetAccounts()")
        self.combo_accn.clear()
        self.combo_odr_accn.clear()
        if not raw:
            self._log("GetAccounts() => (empty)")
            return
        lines = [l.strip() for l in raw.split("\n") if l.strip()]
        self._log(f"GetAccounts() => {len(lines)}개 수신:")
        for line in lines:
            parts = line.split('\t')
            acno = parts[0] if parts else line
            acnm = parts[1] if len(parts) > 1 else ""
            display = f"{acno[:3]} {acno[3:5]} {acno[5:]}  {acnm}".strip() if len(acno) >= 5 else line
            self._log(f"  계좌={acno}({len(acno)}자리) 명={acnm}")
            self.combo_accn.addItem(display, acno)
            self.combo_odr_accn.addItem(display, acno)

    def _evt_error(self, msg):
        self._log(f"[EVT] OnError: {msg}")
        QMessageBox.warning(self, "알림", msg)

    def _evt_close(self):
        self.lbl_login.setText("-")
        self._log("[EVT] OnClose: connection closed")

    def _evt_alert(self, code, msg):
        self._log(f"[EVT] OnAlert [{code:04d}] {msg}")

    def _evt_guide_msg(self, key, msg):
        self._log(f"[EVT] OnGuideMsg [{key}] {msg}")

    def _evt_contract(self, msg):
        # format: key<delim>value<delim>key<delim>value... (delim = \r \t \n)
        self._log(f"[체결통보] {repr(msg)}")
        parts = [p for p in msg.replace('\r', '\t').replace('\n', '\t').split('\t') if p != '']
        m = {}
        it = iter(parts)
        for k in it:
            v = next(it, "")
            m[k] = v
        self._log(
            f"  구분={m.get('988','-')} 계좌={m.get('901','-')} 종목={m.get('907','-')} "
            f"매매={m.get('912','-')} 가격={m.get('916','-')} 구분={m.get('925','-')} "
            f"주문번호={m.get('904','-')} 원주문={m.get('905','-')} "
            f"주문수량={m.get('974','-')} 체결수량={m.get('909','-')} 미체결수량={m.get('931','-')}"
        )

        # 다른 사용자 계좌의 체결통보도 같이 내려올 수 있어서, 현재 선택된 계좌와
        # 일치할 때만 이후 계산(잔고 갱신 등) 로직으로 진입시킨다.
        selected_acno = (self.combo_odr_accn.currentData() or "").strip()
        contract_acno = m.get('901', '').strip()
        if contract_acno != selected_acno:
            return

        # 접수/체결/취소 어느 쪽이든 가능수량(xqty, 매도가능/청산가능)에 영향을 주므로
        # 상태 구분 없이 재조회한다. 실시간 시세와 무관하게 주문/체결 시점에만 오는
        # 저빈도 이벤트라 OnSBalance 대신 여기서 잔고를 다시 조회해도 안전하다.
        self._on_michegyul_send()

    def _evt_sbalance(self, data):
        # 실제 필드 포맷이 화면 설정(m_arField)에 따라 달라지는 레거시 프로토콜이라
        # 신뢰할 수 있는 매핑을 알 수 없어 무시한다 (2026-07-21 논의).
        # 잔고 갱신은 OnContract(체결통보) 수신 시 TR1221/TR3221 재조회로 처리한다.
        pass

    def _evt_fbalance(self, data):
        # 위 OnSBalance와 동일한 이유로 무시한다.
        pass

    def _evt_ver_update(self):
        self._log("[업데이트] OnVerUpdate: exe\\axis\\ -> exe\\ 복사 및 OCX 재등록 시작")
        home = self.ocx.dynamicCall("GetHome()")
        exe_dir  = os.path.join(home, "exe")
        axis_dir = os.path.join(exe_dir, "axis")

        if os.path.isdir(axis_dir):
            # IBKSCONNECTOR.OCX 는 직접 빌드한 버전 사용 - 서버 버전으로 덮어쓰지 않음
            SKIP = {"IBKSCONNECTOR.OCX"}
            OCX_FILES = {"AXWIZARD.OCX", "AXXECURE.OCX", "AXSOCK.OCX", "AXCERTIFY.OCX"}
            for fname in os.listdir(axis_dir):
                if fname.upper() in SKIP:
                    self._log(f"  skip: {fname}")
                    continue
                src = os.path.join(axis_dir, fname)
                dst = os.path.join(exe_dir, fname)
                try:
                    shutil.copy2(src, dst)
                    self._log(f"  copy: {fname}")
                except Exception as e:
                    self._log(f"  copy FAIL: {fname} - {e}")

            for ocx in OCX_FILES:
                ocx_path = os.path.join(exe_dir, ocx)
                if os.path.exists(ocx_path):
                    r = subprocess.call(["regsvr32", "/s", ocx_path])
                    self._log(f"  regsvr32 {ocx} => {'OK' if r == 0 else f'FAIL({r})'}")

            try:
                shutil.rmtree(axis_dir)
                self._log(f"  axis\\ 폴더 삭제 완료")
            except Exception as e:
                self._log(f"  axis\\ 삭제 실패: {e}")
        else:
            self._log(f"  axis\\ 폴더 없음 ({axis_dir})")

        self._log("[업데이트] 완료 - 프로그램을 재시작합니다")
        QMessageBox.information(self, "업데이트 완료", "파일 업데이트가 완료되었습니다.\n확인을 누르면 프로그램이 재시작됩니다.")
        # ActiveX 컨트롤을 먼저 명시적으로 해제해야 재시작 시 임시폴더(_MEI) 삭제가 안정적으로 됨
        self.ocx.clear()
        time.sleep(0.5)
        subprocess.Popen([sys.executable] + sys.argv)
        QApplication.quit()

    def _evt_update_start(self):
        self._log("[업데이트] 파일 업데이트 시작...")

    def _evt_updating(self, filename, percent):
        self._log(f"[업데이트] {filename} {percent}%")

    def _evt_update_end(self):
        self._log("[업데이트] 파일 업데이트 완료")

    def _on_tr8001(self):
        acno = self.combo_accn.currentData() or ""
        if not acno.strip():
            self._log("계좌를 선택하세요")
            return
        result = self.ocx.dynamicCall("TR8001(int, QString)", TK_TR8001, acno)
        self._log(f"고객정보조회 => {result}")
        if not result:
            self._log_err(self.ocx.dynamicCall('GetLastErrMsg()'))

    def _on_odr_send(self):
        acno  = self.combo_odr_accn.currentData() or ""
        pswd  = self.edit_odr_pswd.text()
        code  = self.edit_odr_code.text().strip()
        mmgb  = self.combo_mmgb.currentData()
        hogb  = self.combo_hogb.currentData()
        jprc  = int(self.edit_odr_jprc.text() or "0")
        jqty  = int(self.edit_odr_jqty.text() or "0")
        ojno  = int(self.edit_odr_ojno.text() or "0")

        if self.combo_jtype.currentIndex() == 0:  # 선물옵션
            result = self.ocx.dynamicCall(
                "TR3201(int, int, QString, QString, int, QString, int, int, int, int)",
                [TK_TR3201, mmgb, acno, pswd, ojno, code, jqty, jprc, hogb, 0])
        else:  # 주식
            mkgb = self.combo_jango_mkgubn.currentData()  # 1=KRX 2=NXT 3=통합
            result = self.ocx.dynamicCall(
                "TR1203(int, int, QString, QString, int, QString, int, int, int, int, int)",
                [TK_TR1203, mmgb, acno, pswd, ojno, code, jqty, jprc, hogb, 0, mkgb])

        self._log(f"주문({code} {self.combo_mmgb.currentText()} {jqty}@{jprc}) => {result}")
        if not result:
            err = self.ocx.dynamicCall("GetLastErrMsg()")
            self._log_err(err)

    def _on_michegyul_send(self):
        acno = self.combo_odr_accn.currentData() or ""
        pswd = self.edit_odr_pswd.text()
        if not acno.strip():
            self._log("계좌를 선택하세요")
            return
        self._jngo_acno = acno
        self._jngo_pswd = pswd
        self.table_jango.setRowCount(0)
        self._jango_row_by_code.clear()
        self._jango_calc.clear()
        if self.combo_jtype.currentIndex() == 0:  # 선물옵션
            result = self.ocx.dynamicCall(
                "TR3221(int, QString, QString, QString)",
                [TK_TR3221, acno, pswd, ""])
            r2 = self.ocx.dynamicCall("TR3222(int, QString, QString)", [TK_TR3222, acno, pswd])
            self._log(f"TR3222(실시간잔고등록) => {r2}")
            if not r2:
                self._log_err(self.ocx.dynamicCall('GetLastErrMsg()'))
        else:  # 주식
            mkgb = self.combo_jango_mkgubn.currentData()  # 1=KRX 2=NXT 3=통합
            self._jngo_mkgb = mkgb
            result = self.ocx.dynamicCall(
                "TR1223(int, QString, QString, int, int, QString)",
                [TK_TR1223, acno, pswd, 0, mkgb, ""])
            r2 = self.ocx.dynamicCall("TR1222(int, QString, QString, int)", [TK_TR1222, acno, pswd, 0])
            self._log(f"TR1222(실시간잔고등록) => {r2}")
            if not r2:
                self._log_err(self.ocx.dynamicCall('GetLastErrMsg()'))
        self._log(f"잔고조회 => {result}")
        if not result:
            self._log_err(self.ocx.dynamicCall('GetLastErrMsg()'))

    def _on_chegyul_send(self):
        acno = self.combo_odr_accn.currentData() or ""
        pswd = self.edit_odr_pswd.text()
        if not acno.strip():
            self._log("계좌를 선택하세요")
            return
        self._jngo_acno = acno
        self._jngo_pswd = pswd
        self.table_chegyul.setRowCount(0)
        if self.combo_jtype.currentIndex() == 0:  # 선물옵션
            result = self.ocx.dynamicCall(
                "TR3211(int, QString, QString, int, int, QString, int, int, QString)",
                [TK_TR3211, acno, pswd, 0, 1, "", 0, 0, ""])
        else:  # 주식
            result = self.ocx.dynamicCall(
                "TR1211(int, QString, QString, QString, int, int, int, int, QString)",
                [TK_TR1211, acno, pswd, "", 0, 0, 0, 1, ""])
        self._chegyul_mode = "체결"
        self._log(f"체결조회 => {result}")
        if not result:
            self._log_err(self.ocx.dynamicCall('GetLastErrMsg()'))

    def _on_michegyul_odr_send(self):
        acno = self.combo_odr_accn.currentData() or ""
        pswd = self.edit_odr_pswd.text()
        if not acno.strip():
            self._log("계좌를 선택하세요")
            return
        self._jngo_acno = acno
        self._jngo_pswd = pswd
        self.table_michegyul.setRowCount(0)
        if self.combo_jtype.currentIndex() == 0:  # 선물옵션 dlgb=2
            result = self.ocx.dynamicCall(
                "TR3211(int, QString, QString, int, int, QString, int, int, QString)",
                [TK_TR3211, acno, pswd, 2, 1, "", 0, 0, ""])
        else:  # 주식 dlgb=2
            result = self.ocx.dynamicCall(
                "TR1211(int, QString, QString, QString, int, int, int, int, QString)",
                [TK_TR1211, acno, pswd, "", 0, 0, 2, 1, ""])
        self._chegyul_mode = "미체결"
        self._log(f"미체결조회 => {result}")
        if not result:
            self._log_err(self.ocx.dynamicCall('GetLastErrMsg()'))

    @staticmethod
    def _parse_signed_int(s):
        s = s.strip()
        if s[:1] in ('+', '-'):
            s = s[1:] if s[:1] == '+' else s
        try:
            return int(s)
        except ValueError:
            return None

    @staticmethod
    def _parse_price_int(s):
        # 023(현재가)의 +/-는 실제 부호가 아니라 전일대비 등락 방향 표시라서
        # 가격 자체는 항상 양수로 취급해야 한다 (diff는 _parse_signed_int로 별도 처리).
        s = s.strip()
        if s[:1] in ('+', '-'):
            s = s[1:]
        try:
            return int(s)
        except ValueError:
            return None

    @staticmethod
    def _parse_tick_volume(raw):
        # 032(체결량): 부호(+/-)가 있으면 매수/매도 체결 구분, 없으면 맨 앞자리는 체결강도 표시라서 버림
        if not raw:
            return None, None
        if raw[0] in ('+', '-'):
            side, digits = raw[0], raw[1:]
        else:
            side, digits = None, raw[1:]
        try:
            return side, int(digits)
        except ValueError:
            return side, None

    def _save_tick(self, code, m):
        side, tick_volume = self._parse_tick_volume(m.get("032", ""))
        now = datetime.now()
        try:
            self.db_conn.execute(
                "INSERT INTO ticks (code, trade_time, price, diff, rate, cum_volume, tick_volume, side, received_at) "
                "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)",
                (
                    code,
                    now.strftime("%Y%m%d") + m.get("034", ""),
                    self._parse_price_int(m.get("023", "")),
                    self._parse_signed_int(m.get("024", "")),
                    float(m["033"]) if m.get("033", "").strip() else None,
                    self._parse_signed_int(m.get("027", "")),
                    tick_volume,
                    side,
                    now.strftime("%Y-%m-%d %H:%M:%S.%f"),
                )
            )
            self.db_conn.commit()
        except (ValueError, sqlite3.Error) as e:
            self._log(f"틱 저장 오류({code}): {e}")

    def _evt_real_data(self, data):
        # format: 종목코드\t필드번호\t값\t필드번호\t값\t...
        parts = data.split('\t')
        if len(parts) < 3:
            return

        # 실시간 코드 형식: KRX="A005930", NXT="N.A005930", 통합="M.A005930"
        # '.' 기준 마지막 조각만 취한 뒤 앞의 'A'를 떼면 세 경우 모두 순수 종목코드만 남음
        code = parts[0].split('.')[-1].lstrip('A')

        # key=value 딕셔너리로 변환
        m = {}
        it = iter(parts[1:])
        for k in it:
            v = next(it, "")
            m[k] = v

        if code in self._jango_calc and "023" in m:
            self._recalc_jango_on_price(code, m["023"])

        if code in self.watch_codes and "034" in m:
            self._save_tick(code, m)

        if code == self.auto_watch_code and "034" in m:
            price = self._parse_price_int(m.get("023", ""))
            if price is not None:
                self._on_auto_watch_tick(price)

        if code != self.edit_sise_code.text().strip().lstrip('A'):
            return  # 현재 조회 중인 종목만 처리(시세조회 화면 갱신은 여기까지)

        if "034" in m:  # 체결시간 있으면 체결 데이터
            self.lbl_curr.setText(m.get("023", "-").lstrip('+-') or "-")
            self.lbl_diff.setText(m.get("024", "-"))
            self.lbl_rate.setText(m.get("033", "-"))
            self.lbl_gvol.setText(m.get("027", "-"))
            self.lbl_siga.setText(m.get("029", "-").lstrip('+-') or "-")
            self.lbl_koga.setText(m.get("030", "-").lstrip('+-') or "-")
            self.lbl_jega.setText(m.get("031", "-").lstrip('+-') or "-")
            self.lbl_mgjv.setText(m.get("201", "-"))
        elif "111" in m:  # 동시호가 구간엔 034(체결) 대신 111(예상체결가)만 옴
            self.lbl_curr.setText(m.get("111", "-").lstrip('+-') or "-")

        if "040" in m:  # 호가시간 있으면 호가 데이터
            self.lbl_dvol.setText(m.get("101", "-"))
            self.lbl_svol.setText(m.get("106", "-"))

    def _traced_dynamic_call(self, sig, *args):
        # TRxxxx 요청은 보낸 시각을 key(TR구분자)로 기록해뒀다가, 응답이 오는 _evt_recv_data에서
        # 걸린 시간을 계산해 로그로 남긴다.
        name = sig.split("(", 1)[0]
        if re.match(r"^TR\d+$", name):
            params = args[0] if len(args) == 1 and isinstance(args[0], (list, tuple)) else args
            if params and isinstance(params[0], int):
                self._tr_sent_at[params[0]] = (name, time.monotonic())
        return self._ocx_dynamic_call(sig, *args)

    def _evt_recv_data(self, key, dptr, size, b_next, nkey):
        sent = self._tr_sent_at.pop(key, None)
        if sent:
            name, t0 = sent
            wire = TR_WIRE_NAME.get(name)
            label = f"{name}({wire})" if wire else name
            self._log(f"[응답시간] {label} {(time.monotonic() - t0) * 1000:.0f}ms")
        self._log(f"[EVT] OnRecvData key={key} dptr={dptr} size={size} bNext={b_next} nkey={nkey}")
        if dptr == 0 or size <= 0:
            return
        try:
            # copy bytes immediately - dptr memory may be freed after this call returns
            addr = dptr & 0xFFFFFFFF
            raw = ctypes.string_at(addr, size)
            def s(b): return b.decode('cp949', errors='ignore').strip()
            if key == TK_TR3001:
                mod = tr3001_mod.from_buffer_copy(raw)
                self.lbl_curr.setText(s(mod.curr))
                self.lbl_diff.setText(s(mod.diff))
                self.lbl_rate.setText(s(mod.rate))
                self.lbl_gvol.setText(s(mod.gvol))
                self.lbl_siga.setText(s(mod.siga))
                self.lbl_koga.setText(s(mod.koga))
                self.lbl_jega.setText(s(mod.jega))
                self.lbl_mgjv.setText(s(mod.mgjv))
                self._log(f"  [선물시세] 현재가={s(mod.curr)} 전일대비={s(mod.diff)}"
                          f" 등락률={s(mod.rate)} 거래량={s(mod.gvol)}"
                          f" 시가={s(mod.siga)} 고가={s(mod.koga)} 저가={s(mod.jega)}"
                          f" 미결={s(mod.mgjv)}")
            elif key == TK_TR1001:
                mod = tr1001_mod.from_buffer_copy(raw)
                self.lbl_curr.setText(s(mod.curr))
                self.lbl_diff.setText(s(mod.diff))
                self.lbl_rate.setText(s(mod.rate))
                self.lbl_gvol.setText(s(mod.gvol))
                self.lbl_siga.setText(s(mod.siga))
                self.lbl_koga.setText(s(mod.koga))
                self.lbl_jega.setText(s(mod.jega))
                self.lbl_mgjv.setText("-")
                self._log(f"  [주식시세] 현재가={s(mod.curr)} 전일대비={s(mod.diff)}"
                          f" 등락률={s(mod.rate)} 거래량={s(mod.gvol)}"
                          f" 시가={s(mod.siga)} 고가={s(mod.koga)} 저가={s(mod.jega)}")
            elif key in (TK_TR3201, TK_TR1203):
                # 주문번호[6] 원주문번호[6] 에러메시지[80] (nrec[4] 이후)
                jmno = raw[4:10].decode('cp949', errors='ignore').strip()
                ojno = raw[10:16].decode('cp949', errors='ignore').strip()
                emsg = raw[16:96].decode('cp949', errors='ignore').strip()
                label = "선물주문" if key == TK_TR3201 else "주식주문"
                self.lbl_odr_result.setText(f"주문번호:{jmno}")
                self._log(f"  [{label}] 주문번호={jmno} 원주문번호={ojno} 메시지={emsg}")
                if self._auto_order_pending and not self.chk_auto_order_confirm.isChecked():
                    # 자동매매가 낸 주문은 기본적으로 모달 팝업 없이 로그로만 결과를 남긴다
                    # ("주문확인창" 체크박스를 켜두면 자동주문도 수동과 동일하게 팝업으로 확인 가능)
                    self._auto_order_pending = False
                    if jmno and jmno != "000000":
                        self._log(f"[자동매매] {label} 접수 - 주문번호: {jmno}")
                    else:
                        self._log(f"[자동매매] {label} 실패 - {emsg or '주문이 거부되었습니다.'}")
                    return
                self._auto_order_pending = False
                if jmno and jmno != "000000":
                    QMessageBox.information(self, f"{label} 접수", f"주문번호: {jmno}\n{emsg}")
                else:
                    QMessageBox.warning(self, f"{label} 실패", emsg or "주문이 거부되었습니다.")
            elif key == TK_TR3221:  # 선물 잔고 (header: acno[11]+nrec[4]=15, grid=149)
                GRID = 149
                nrec = int(raw[11:15].decode('cp949', errors='ignore').strip() or "0")
                self._log(f"  [선물잔고] {nrec}건")
                for i in range(nrec):
                    g = raw[15 + i*GRID: 15 + (i+1)*GRID]
                    if len(g) < GRID: break
                    if not s(g[0:8]):  # 종목코드 빈 레코드는 건너뛴다
                        continue
                    self._log(f"    {s(g[0:8])} {s(g[38:44])} 잔고={s(g[44:54])} 가능={s(g[54:64])} 평단={s(g[64:74])} 현재={s(g[74:84])} 평가금액={s(g[94:109])} 수익률={s(g[124:134])}% 매입금액={s(g[134:149])}")
                    self._add_jango_row(s(g[0:8]), s(g[44:54]), s(g[54:64]), s(g[134:149]), s(g[74:84]), s(g[94:109]), s(g[124:134]))
                if b_next:
                    acno = getattr(self, '_jngo_acno', '')
                    pswd = getattr(self, '_jngo_pswd', '')
                    self.ocx.dynamicCall("TR3221(int, QString, QString, QString)",
                                        [TK_TR3221, acno, pswd, nkey])
            elif key == TK_TR1223:  # 주식 잔고(시장구분) (header: acno[11]+nrec[4]=15, grid=194)
                GRID = 194
                nrec = int(raw[11:15].decode('cp949', errors='ignore').strip() or "0")
                self._log(f"  [주식잔고] {nrec}건")
                for i in range(nrec):
                    g = raw[15 + i*GRID: 15 + (i+1)*GRID]
                    if len(g) < GRID: break
                    if not s(g[0:12]):  # 종목코드 빈 레코드는 건너뛴다
                        continue
                    self._log(f"    {s(g[0:12])} 잔고={s(g[54:64])} 가능={s(g[64:74])} 평단={s(g[74:84])} 현재={s(g[99:109])} 평가금액={s(g[129:144])} 수익률={s(g[159:169])}% 매입금액={s(g[84:99])}")
                    self._add_jango_row(s(g[0:12]), s(g[54:64]), s(g[64:74]), s(g[84:99]), s(g[99:109]), s(g[129:144]), s(g[159:169]))
                if b_next:
                    acno = getattr(self, '_jngo_acno', '')
                    pswd = getattr(self, '_jngo_pswd', '')
                    mkgb = getattr(self, '_jngo_mkgb', 1)
                    self.ocx.dynamicCall("TR1223(int, QString, QString, int, int, QString)",
                                        [TK_TR1223, acno, pswd, 0, mkgb, nkey])
            elif key == TK_TR3211:  # 선물 체결/미체결 (header: acno[11]+nrec[4]=15, grid=179)
                GRID = 179
                mode = getattr(self, '_chegyul_mode', '체결')
                nrec = int(raw[11:15].decode('cp949', errors='ignore').strip() or "0")
                self._log(f"  [선물{mode}] {nrec}건")
                for i in range(nrec):
                    g = raw[15 + i*GRID: 15 + (i+1)*GRID]
                    if len(g) < GRID: break
                    self._log(f"    주문={s(g[6:12])} {s(g[18:26])} {s(g[56:64])} 주문가={s(g[84:95])} 주문량={s(g[95:102])} 체결가={s(g[106:117])} 체결량={s(g[117:124])} 미체결={s(g[130:137])} [{s(g[144:152])}]")
                if b_next:
                    acno = getattr(self, '_jngo_acno', '')
                    pswd = getattr(self, '_jngo_pswd', '')
                    dlgb = 2 if mode == "미체결" else 0
                    self.ocx.dynamicCall(
                        "TR3211(int, QString, QString, int, int, QString, int, int, QString)",
                        [TK_TR3211, acno, pswd, dlgb, 1, "", 0, 0, nkey])
            elif key == TK_TR1211:  # 주식 체결/미체결 (header: acno[11]+nrec[4]=15, grid=265)
                GRID = 265
                mode = getattr(self, '_chegyul_mode', '체결')
                table = self.table_chegyul if mode == "체결" else self.table_michegyul
                nrec = int(raw[11:15].decode('cp949', errors='ignore').strip() or "0")
                self._log(f"  [주식{mode}] {nrec}건")
                for i in range(nrec):
                    g = raw[15 + i*GRID: 15 + (i+1)*GRID]
                    if len(g) < GRID: break
                    mkgb = s(g[20:21])
                    juno, ojno, cod2 = s(g[0:10]), s(g[10:20]), s(g[21:33])
                    hnam, gubn = s(g[33:73]), s(g[73:93])
                    oprc, oqty, dprc, dqty = s(g[133:145]), s(g[145:157]), s(g[157:169]), s(g[169:181])
                    wqty, stat, time_ = s(g[193:205]), s(g[237:257]), s(g[257:265])
                    self._log(f"    주문={juno} 원주문={ojno} 시장={MKGB_NAMES.get(mkgb, mkgb)} 종목={cod2} {hnam} {gubn} 주문가={oprc} 주문량={oqty} 체결가={dprc} 체결량={dqty} 미체결={wqty} [{stat}] 시간={time_}")
                    row = table.rowCount()
                    table.insertRow(row)
                    vals = [juno, ojno, MKGB_NAMES.get(mkgb, mkgb), cod2, hnam, gubn,
                            oprc, oqty, dprc, dqty, wqty, stat, _fmt_hhmmssxx(time_)]
                    for col, val in enumerate(vals):
                        table.setItem(row, col, QTableWidgetItem(val))
                if b_next:
                    acno = getattr(self, '_jngo_acno', '')
                    pswd = getattr(self, '_jngo_pswd', '')
                    dlgb = 2 if mode == "미체결" else 0
                    self.ocx.dynamicCall(
                        "TR1211(int, QString, QString, QString, int, int, int, int, QString)",
                        [TK_TR1211, acno, pswd, "", 0, 0, dlgb, 1, nkey])
            elif key == TK_TR8001:  # 고객정보 (pihocust, mod: gubn[1])
                gubn = raw[0:1].decode('cp949', errors='ignore').strip()
                self._log(f"  [고객정보] gubn={gubn!r} raw={raw[:32].hex(' ')}")
            elif key == TK_TR1002:  # 주식 시세(NXT/통합) - 응답이 고정바이너리 아닌 평문 텍스트
                text = raw.decode('cp949', errors='ignore')
                self._log(f"  [TR1002 시세] raw={text!r}")
                # 맨 앞 토큰 = 종목코드(실시간 코드와 동일 형식, M.A005930 등). 빈 필드가 있을 수
                # 있어서(예: 매도/매수잔량 미제공) 빈 문자열을 걸러내면 안 됨 - 위치가 밀려버림.
                tokens = text.replace('\n', '\t').split('\t')
                realtime_key, vals = (tokens[0], tokens[1:]) if tokens else ("", [])
                self._sise_realtime_key = realtime_key
                pairs = ", ".join(f"{lbl}={v}" for lbl, v in zip(TR1002_SISE_LABELS, vals))
                self._log(f"  [TR1002 파싱시도] code={realtime_key} {pairs}")
                # 현재가,등락율,시가,고가,저가,전일대비,거래량,매도잔량,매수잔량 순
                # 등락률/전일대비는 방향(+-)이 의미가 있어 부호를 유지하고, 나머지는 부호를 뗀다.
                widgets = [self.lbl_curr, self.lbl_rate, self.lbl_siga, self.lbl_koga,
                           self.lbl_jega, self.lbl_diff, self.lbl_gvol, self.lbl_dvol, self.lbl_svol]
                keep_sign = {self.lbl_rate, self.lbl_diff}
                for w, v in zip(widgets, vals):
                    w.setText(v if w in keep_sign else (v.lstrip('+-') or "-"))
            elif key == TK_TR1004:  # 다중종목 실시간 등록 응답 - 코드별로 레코드가 이어붙어 옴
                text = raw.decode('cp949', errors='ignore')
                self._log(f"  [TR1004 응답] size={size}")
                # 레코드 시작점 = 종목코드 토큰(M.A005930, N.A005930, A441800 등) + 탭.
                # 그 앞에는 화면 헤더용 고정 필드가 붙어있어서 코드 패턴으로 잘라낸다.
                matches = list(re.finditer(r'((?:[A-Z]\.)?A\d{6})\t', text))
                for i, m in enumerate(matches):
                    code = m.group(1).split('.')[-1].lstrip('A')
                    start = m.end()
                    end = matches[i + 1].start() if i + 1 < len(matches) else len(text)
                    vals = text[start:end].rstrip('\t').split('\t')
                    pairs = ", ".join(f"{lbl}={v}" for lbl, v in zip(TR1002_SISE_LABELS, vals))
                    self._log(f"  [TR1004 파싱] code={code} {pairs}")
            elif key == TK_TR1005:  # 일/주/월별 시세조회 응답
                # 레코드 시작점 = 일자(12021, 8자리 20xxxxxx) 필드 + 탭. 그 앞에는 화면 헤더용
                # 고정 필드가 섞여 있어서(TR1004와 동일 문제) 코드 패턴 대신 일자 패턴으로 잘라낸다.
                text = raw.decode('cp949', errors='ignore')
                matches = list(re.finditer(r'(?:^|\t)(20\d{6})\t', text))
                self._log(f"  [TR1005 응답] size={size} 레코드수={len(matches)}")
                for i, m in enumerate(matches[:5]):
                    start = m.start(1)
                    end = matches[i + 1].start(1) if i + 1 < len(matches) else len(text)
                    vals = [v for v in text[start:end].split('\t') if v != '']
                    pairs = ", ".join(f"{lbl}={v}" for lbl, v in zip(TR1005_DAILY_LABELS, vals))
                    self._log(f"  [TR1005 레코드] {pairs}")
                if len(matches) > 5:
                    self._log(f"  [TR1005] ... 외 {len(matches) - 5}건 더 있음")
            elif key == TK_TR1006:  # 주요 시장지표(지수/금리) 응답
                text = raw.decode('cp949', errors='ignore')
                self._log(f"  [TR1006 응답] size={size}")
                idx_by_code = {}
                for m in TR1006_INDEX_RE.finditer(text):
                    code, _name, price, diff, rate, vol, amt, ntot, nup, ndn = m.groups()
                    label = TR1006_INDEX_LABELS.get(code, code)
                    idx_by_code[code] = (label, price, rate)
                    self._log(f"  [TR1006 지수] {label} 현재가={price} 전일대비={diff} 등락률={rate}% "
                              f"거래량(천)={vol} 거래대금(백만)={amt} 거래종목수={ntot} 상승={nup} 하락={ndn}")
                kospi = idx_by_code.get("K0001")
                kosdaq = idx_by_code.get("KQ001")
                if kospi and kosdaq:
                    self.lbl_market_summary.setText(
                        f"코스피 {kospi[1]}({kospi[2]}%)   코스닥 {kosdaq[1]}({kosdaq[2]}%)")
                rate_start = text.find("CP(91")
                rate_end = text.find("TJ", rate_start) if rate_start >= 0 else -1
                if rate_start >= 0 and rate_end >= 0:
                    for m in TR1006_RATE_RE.finditer(text[rate_start:rate_end]):
                        gubn, rate, sign, diff = m.groups()
                        self._log(f"  [TR1006 금리] {gubn} 금리={rate} 전일대비={sign}{diff}")
                for m in TR1006_FLOW_RE.finditer(text):
                    code, netbuy, buy, sell = m.groups()
                    label = TR1006_FLOW_LABELS.get(code, code)
                    self._log(f"  [TR1006 매매동향] {label} 순매수={netbuy} 매수={buy} 매도={sell}")
                dump_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "tr1006_raw.bin")
                with open(dump_path, "wb") as f:
                    f.write(raw)
            elif key == TK_TR1007:  # 차트(일/주/월봉 캔들) 응답
                # 캔들 레코드는 "일자\t\t시가\t...\t수정비율\n" 형태로, TR1005/TR1004와 동일하게
                # 앞부분에 스냅샷 필드가 섞여 있어서 "일자+빈시간필드" 이중탭 패턴으로 레코드 시작점을 찾는다.
                text = raw.decode('cp949', errors='ignore')
                matches = list(re.finditer(r'(20\d{6})\t\t', text))
                labels = TR1007_INDEX_CANDLE_LABELS if self._history_last_dunit == 2 else TR1007_CANDLE_LABELS
                # 스냅샷/dataH 에코 안에도 우연히 "날짜+이중탭" 패턴이 하나 섞여 나올 수 있는데(값이
                # "6296,38" 처럼 쉼표가 섞이는 등 깨져 있음), 시가~거래대금이 순수 숫자가 아니면 걸러낸다.
                records = []
                for i, m in enumerate(matches):
                    start = m.start(1)
                    end = matches[i + 1].start(1) if i + 1 < len(matches) else len(text)
                    vals = text[start:end].rstrip('\n').split('\t')
                    if len(vals) < 10 or not all(re.fullmatch(r'\d+', v) for v in vals[2:8]):
                        continue
                    records.append(vals)
                self._log(f"  [TR1007 응답] size={size} 레코드수={len(records)}")
                for vals in records[:5]:
                    pairs = ", ".join(f"{lbl}={v}" for lbl, v in zip(labels, vals))
                    self._log(f"  [TR1007 레코드] {pairs}")
                if len(records) > 5:
                    self._log(f"  [TR1007] ... 외 {len(records) - 5}건 더 있음")
                saved = self._save_history_records(
                    self._history_last_code, self._history_last_dunit, self._history_last_dindex, records)
                self._log(f"  [TR1007] DB 저장: {saved}건 (code={self._history_last_code})")
            else:
                self._log(f"  [RecvData] key={key} (미처리)")
        except Exception as e:
            self._log(f"  [RecvData 파싱 오류] {e}")


app = QApplication(sys.argv)
window = TestWindow()
window.show()
sys.exit(app.exec_())
