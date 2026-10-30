import sys
import os
import shutil
import subprocess
import ctypes
import winreg
import time
import json
from PyQt5.QAxContainer import QAxWidget
from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QGroupBox, QPushButton, QLabel, QLineEdit, QTextEdit, QFormLayout,
    QComboBox, QGridLayout, QMessageBox, QTableWidget, QTableWidgetItem,
    QHeaderView, QAbstractItemView, QTabWidget, QScrollArea, QCheckBox
)
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QColor
from datetime import datetime
from decimal import Decimal

OCX_GUID = "{CDADD338-C7AB-4977-B65D-8E988B5958E3}"

SVR_PORT = 15201  # 서버 포트 (화면 비노출)

# Build target - keep in sync with OCX IBKSConnectorCtl.h `_type`
#   "normal"  : domestic customer   (domestic only)
#   "foreign" : overseas developer  (+ overseas group incl. exchange/pre-settlement tabs)
#   "admin"   : internal test       (+ server select, chaser, TR8001, login save)
USER_TYPE = "foreign"
IS_FOREIGN = USER_TYPE in ("foreign", "admin")
IS_ADMIN   = USER_TYPE == "admin"

LOGIN_SAVE_FILE = "login.dat"   # saved next to the exe, DPAPI encrypted (internal only)


class _DATA_BLOB(ctypes.Structure):
    _fields_ = [("cbData", ctypes.c_uint32), ("pbData", ctypes.POINTER(ctypes.c_char))]


def _dpapi(data: bytes, protect: bool) -> bytes:
    # CryptProtectData/CryptUnprotectData: only the same Windows user on the same PC can decrypt
    buf = ctypes.create_string_buffer(data, len(data))
    src = _DATA_BLOB(len(data), ctypes.cast(buf, ctypes.POINTER(ctypes.c_char)))
    dst = _DATA_BLOB()
    fn = ctypes.windll.crypt32.CryptProtectData if protect else ctypes.windll.crypt32.CryptUnprotectData
    if not fn(ctypes.byref(src), None, None, None, None, 0, ctypes.byref(dst)):
        raise OSError("DPAPI failed")
    try:
        return ctypes.string_at(dst.pbData, dst.cbData)
    finally:
        ctypes.windll.kernel32.LocalFree(dst.pbData)


SERVER_LIST = [
    ("211.255.204.104", "UAT"),
]

# TR key constants - must match Common.h exactly
TK_TR1001 = 1   # 주식 시세조회
TK_TR1002 = 2   # 주식 조회(OOP)
TK_TR1003 = 3   # 주식 조회(OOP)
TK_TR1201 = 4   # 주식 주문
TK_TR1203 = 5   # 주식 주문 시장구분
TK_TR1211 = 7   # 주식 체결/미체결조회
TK_TR1221 = 8   # 주식 잔고조회
TK_TR1231 = 9   # 주식 주문옵션조회
TK_TR1801 = 10  # 지수관련 종목시세
TK_TR1802 = 11  # 지수관련 oop시세
TK_TR1803 = 12  # 지수관련 종목현황
TK_TR1804 = 13  # 지수관련 시세
TK_TR1222 = 45  # 주식 잔고조회(실시간)
TK_TR8001  = 50 # 고객정보조회
TK_TR1223  = 51 # 주식 잔고조회(시장구분)
TK_GETCODE = 32 # 종목코드 목록조회 (GetCode)
TK_TR5002 = 160  # 해외주식 체결/미체결조회
TK_TR5000 = 161  # 해외증권 예수금조회
TK_TR5001 = 162  # 해외주식 잔고조회
TK_TR2002 = 163  # 주식 거래내역
TK_TR5003 = 164  # 해외주식 거래내역
TK_TR5005 = 165  # 환전 적용환율조회
TK_TR5006 = 166  # 외화환전(단건) 실행 (trxtp=1, 실제 체결)
TK_TR5006_EST = 170  # 외화환전 예상금액 조회 (trxtp=0) - 응답 구분용 별도 key
TK_TR5010A = 167  # 환전 중계은행조회 (TR5006 전에 필수)
TK_TR5007 = 168  # 환전 기업은행 환율조회 (SDPKT183)
TK_TR5004 = 169  # 환전 국민은행 환율조회 (SDPKT169)
TK_TR5011 = 171  # 해외주식 가정산조회 (COSCQ002, 개발자 추가 dispid 85)

# TR1002(NXT/통합 시세조회) 요청 시 1777(장운영구분) 뒤에 붙이는 요청 필드번호 목록
# 순서 = 응답에서 값이 오는 순서와 동일하다고 가정 (실측 후 확정 필요)
TR1002_SISE_FIELDS = ["2023", "2033", "2029", "2030", "2031", "2024", "2027", "2041", "2061"]
TR1002_SISE_LABELS = ["현재가", "등락율", "시가", "고가", "저가", "전일대비", "거래량", "매도잔량", "매수잔량"]

MKGB_NAMES = {"1": "KRX", "2": "NXT", "3": "통합"}
OVS_BNSTP_NAMES = {"%": "전체", "1": "매도", "2": "매수", "8": "단주매도", "9": "단주취소"}

# C structs mirroring Open_API_OUT.h (#pragma pack(1))
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


class TestWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        title_tag = {"admin": " [내부테스트용]", "foreign": " [해외개발자용]"}.get(USER_TYPE, "")
        self.setWindowTitle("IBKConnector OCX Test" + title_tag)
        self.setMinimumWidth(1000)
        # 로그창까지 보이도록 화면 작업영역 높이에 맞춘다
        avail = QApplication.primaryScreen().availableGeometry()
        self.setGeometry(avail.x() + (avail.width() - min(1400, avail.width())) // 2,
                         avail.y() + 30, min(1400, avail.width()), avail.height() - 40)

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
        self._jango_row_by_code = {}
        self._jango_calc = {}
        self._sise_realtime_key = ""  # TR1002 응답의 1021 필드값 (OnRealData 코드와 동일 형식)

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
        layout.setSpacing(6)

        layout.addWidget(self._build_init_group())

        # Login | 시세조회 좌우 분할
        mid = QWidget()
        mid_h = QHBoxLayout(mid)
        mid_h.setContentsMargins(0, 0, 0, 0)
        mid_h.addWidget(self._build_login_group())
        if IS_ADMIN:                     # domestic quote panel: internal only
            mid_h.addWidget(self._build_sise_group())
        layout.addWidget(mid)

        # 주문(국내) | 해외거래 좌우 분할
        odr = QWidget()
        odr_h = QHBoxLayout(odr)
        odr_h.setContentsMargins(0, 0, 0, 0)
        odr_h.addWidget(self._build_order_group())
        if IS_FOREIGN:                   # overseas group: foreign developer + internal
            odr_h.addWidget(self._build_overseas_group())
        layout.addWidget(odr)

        layout.addWidget(self._build_log_group())

        if IS_ADMIN:
            self._load_login_info()      # after log group exists (uses _log)

    # ── UI builders ──────────────────────────────────────────────

    def _build_init_group(self):
        group = QGroupBox("Initialize")
        h = QHBoxLayout(group)
        btn = QPushButton("Initialize()")
        btn.clicked.connect(self._on_initialize)
        self.lbl_init = QLabel("-")
        h.addWidget(btn)
        h.addWidget(self.lbl_init)
        h.addStretch()
        return group

    def _build_login_group(self):
        group = QGroupBox("Login")
        layout = QVBoxLayout(group)

        form = QFormLayout()
        self.edit_user_id  = QLineEdit("")
        self.edit_user_pw  = QLineEdit(""); self.edit_user_pw.setEchoMode(QLineEdit.Password)
        self.edit_cert_pw  = QLineEdit(""); self.edit_cert_pw.setEchoMode(QLineEdit.Password)
        form.addRow("user_id",   self.edit_user_id)
        form.addRow("user_pw",   self.edit_user_pw)
        form.addRow("cert_pw",   self.edit_cert_pw)
        if IS_FOREIGN:                   # server combo: foreign (UAT by name) + internal (all, with IP)
            self.combo_server  = QComboBox()
            self.combo_server.addItem("", "")
            for ip, name in SERVER_LIST:
                # IP is shown only in the internal build
                self.combo_server.addItem(f"{name} ({ip})" if IS_ADMIN else name, ip)
            # default: foreign -> UAT, internal -> blank (GLB)
            uat_idx = self.combo_server.findText("UAT")
            self.combo_server.setCurrentIndex(uat_idx if not IS_ADMIN and uat_idx >= 0 else 0)
            svr_h = QHBoxLayout()
            svr_h.addWidget(self.combo_server, 1)              # half width
            svr_h.addWidget(QLabel("미선택시 GLB"), 1)
            form.addRow("서버선택",  svr_h)
        if IS_FOREIGN:                   # direct IP input: foreign + internal
            self.combo_server.currentIndexChanged.connect(self._on_server_selected)
            self.edit_svr_ip   = QLineEdit("")
            form.addRow("svr_ip",    self.edit_svr_ip)
        layout.addLayout(form)

        if IS_ADMIN:                     # quote-only login (LoginQuote): internal only
            self.chk_quote_only = QCheckBox("시세조회 전용 (인증서 없이 ID/PW만으로 로그인 - LoginQuote)")
            layout.addWidget(self.chk_quote_only)

        h = QHBoxLayout()
        btn_login  = QPushButton("Login()")
        btn_logout = QPushButton("Logout()")
        btn_full   = QPushButton("전체파일 다시받기")
        btn_full.setToolTip("tab\\infoAXIS, infoRSC를 지우고 프로그램을 재실행합니다. 다음 로그인 때 전체 파일을 다시 받습니다.")
        btn_login.clicked.connect(self._on_login)
        btn_logout.clicked.connect(self._on_logout)
        btn_full.clicked.connect(self._on_full_redownload)
        self.lbl_login = QLabel("-")
        h.addWidget(btn_login)
        h.addWidget(btn_logout)
        if IS_ADMIN:
            btn_cust = QPushButton("고객정보(TR8001)")
            btn_cust.clicked.connect(self._on_tr8001)
            h.addWidget(btn_cust)
            self.chk_save_login = QCheckBox("접속정보 저장")
            self.chk_save_login.setToolTip("로그인 성공 시 ID/PW/인증서PW를 암호화(DPAPI)해 exe 폴더에 저장합니다. 해제하면 즉시 지웁니다.")
            self.chk_save_login.toggled.connect(self._on_save_login_toggled)
            h.addWidget(self.chk_save_login)
        h.addWidget(self.lbl_login)
        h.addStretch()
        h.addWidget(btn_full)        # far right, away from Login() to avoid misclicks
        layout.addLayout(h)

        if IS_ADMIN:
            h_chaser = QHBoxLayout()
            btn_chaser_show = QPushButton("체이서 보기")
            btn_chaser_show.setToolTip("AxisChaser.exe를 실행해 이 프로그램의 송수신 프로토콜을 실시간으로 확인합니다")
            btn_chaser_hide = QPushButton("체이서 닫기")
            btn_chaser_show.clicked.connect(self._on_show_chaser)
            btn_chaser_hide.clicked.connect(self._on_hide_chaser)
            h_chaser.addWidget(btn_chaser_show)
            h_chaser.addWidget(btn_chaser_hide)
            h_chaser.addStretch()
            layout.addLayout(h_chaser)

        return group

    def _build_sise_group(self):
        group = QGroupBox("시세조회(국내주식)")
        v = QVBoxLayout(group)

        # 종목코드 입력 + 시장구분 + 조회 버튼
        h = QHBoxLayout()
        h.addWidget(QLabel("종목코드"))
        self.edit_sise_code = QLineEdit()
        self.edit_sise_code.setMaximumWidth(100)
        h.addWidget(self.edit_sise_code)
        h.addWidget(QLabel("시장"))
        self.combo_mkgubn = QComboBox()
        for text, val in [("KRX", 1), ("NXT", 2), ("통합", 3)]:
            self.combo_mkgubn.addItem(text, val)
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

        return group

    def _build_order_group(self):
        group = QGroupBox("국내")
        v = QVBoxLayout(group)

        # 계좌/비밀번호는 탭 공통 - 해외거래 쪽과 같은 구성
        acc_h = QHBoxLayout()
        acc_h.addWidget(QLabel("계좌"))
        self.combo_odr_accn = QComboBox()
        self.combo_odr_accn.setMinimumWidth(260)
        self.combo_odr_accn.setSizeAdjustPolicy(QComboBox.AdjustToContents)
        acc_h.addWidget(self.combo_odr_accn)
        acc_h.addWidget(QLabel("비밀번호"))
        self.edit_odr_pswd = QLineEdit()
        self.edit_odr_pswd.setEchoMode(QLineEdit.Password)
        acc_h.addWidget(self.edit_odr_pswd)
        acc_h.addStretch()
        v.addLayout(acc_h)

        tabs = QTabWidget()
        v.addWidget(tabs)
        tabs.addTab(self._build_odr_order_tab(), "주문")
        tabs.addTab(self._build_odr_chegyul_tab(), "체결")
        tabs.addTab(self._build_odr_michegyul_tab(), "미체결")
        tabs.addTab(self._build_odr_jango_tab(), "잔고")
        tabs.addTab(self._build_odr_gorae_tab(), "거래내역(TR2002)")
        return group

    def _build_odr_order_tab(self):
        w = QWidget()
        h_main = QHBoxLayout(w)

        left = QFormLayout()
        self.edit_odr_code = QLineEdit()
        left.addRow("종목코드", self.edit_odr_code)
        lbl_code_hint = QLabel("ex) A005930")     # stock code needs the leading 'A'
        lbl_code_hint.setStyleSheet("color: gray;")
        left.addRow("", lbl_code_hint)

        self.combo_mmgb = QComboBox()
        for text, val in [("1 매도", 1), ("2 매수", 2), ("3 정정", 3), ("4 취소", 4)]:
            self.combo_mmgb.addItem(text, val)
        left.addRow("매매구분", self.combo_mmgb)

        self.combo_hogb = QComboBox()
        for text, val in [("00 지정가", 0), ("03 시장가", 3), ("05 조건부", 5),
                          ("06 최유리", 6), ("10 지정가(IOC)", 10), ("20 지정가(FOK)", 20)]:
            self.combo_hogb.addItem(text, val)
        left.addRow("호가유형", self.combo_hogb)

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
        self.combo_odr_mkgubn = QComboBox()
        for text, val in [("KRX", 1), ("NXT", 2), ("통합", 3)]:
            self.combo_odr_mkgubn.addItem(text, val)
        self.lbl_odr_result = QLabel("-")
        mk_h.addWidget(self.combo_odr_mkgubn)
        mk_h.addWidget(self.lbl_odr_result)
        mk_h.addStretch()
        right.addRow(mk_h)

        btn = QPushButton("주문실행")
        btn.clicked.connect(self._on_odr_send)
        right.addRow(btn)

        h_main.addLayout(left)
        h_main.addSpacing(20)
        h_main.addLayout(right)
        h_main.addStretch()
        return w

    def _build_chegyul_columns(self):
        return ["주문번호", "원주문번호", "종목", "주문가", "주문량", "체결가", "체결량", "미체결량", "메시지/시간"]

    def _build_odr_chegyul_tab(self):
        w = QWidget()
        v = QVBoxLayout(w)
        h = QHBoxLayout()
        btn = QPushButton("조회")
        btn.clicked.connect(self._on_chegyul_send)
        h.addWidget(btn)
        h.addStretch()
        v.addLayout(h)

        cols = self._build_chegyul_columns()
        self.table_chegyul = QTableWidget(0, len(cols))
        self.table_chegyul.setHorizontalHeaderLabels(cols)
        self.table_chegyul.verticalHeader().setVisible(False)
        self.table_chegyul.verticalHeader().setDefaultSectionSize(18)
        self.table_chegyul.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table_chegyul.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.table_chegyul.horizontalHeader().setStretchLastSection(True)
        self.table_chegyul.setFixedHeight(140)
        v.addWidget(self.table_chegyul)
        return w

    def _build_odr_michegyul_tab(self):
        w = QWidget()
        v = QVBoxLayout(w)
        h = QHBoxLayout()
        btn = QPushButton("조회")
        btn.clicked.connect(self._on_michegyul_odr_send)
        h.addWidget(btn)
        h.addStretch()
        v.addLayout(h)

        cols = self._build_chegyul_columns()
        self.table_michegyul = QTableWidget(0, len(cols))
        self.table_michegyul.setHorizontalHeaderLabels(cols)
        self.table_michegyul.verticalHeader().setVisible(False)
        self.table_michegyul.verticalHeader().setDefaultSectionSize(18)
        self.table_michegyul.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table_michegyul.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.table_michegyul.horizontalHeader().setStretchLastSection(True)
        self.table_michegyul.setFixedHeight(140)
        v.addWidget(self.table_michegyul)
        return w

    def _build_odr_jango_tab(self):
        w = QWidget()
        v = QVBoxLayout(w)
        v.setContentsMargins(4, 4, 4, 4)
        v.setSpacing(2)

        h = QHBoxLayout()
        h.addWidget(QLabel("시장"))
        self.combo_jango_mkgubn = QComboBox()
        for text, val in [("KRX", 1), ("NXT", 2), ("통합", 3)]:
            self.combo_jango_mkgubn.addItem(text, val)
        h.addWidget(self.combo_jango_mkgubn)
        btn = QPushButton("조회")
        btn.clicked.connect(self._on_michegyul_send)
        h.addWidget(btn)
        h.addStretch()
        v.addLayout(h)

        cols = ["종목코드", "잔고수량", "가능수량", "현재가", "평가금액", "수익률(%)"]
        self.table_jango = QTableWidget(0, len(cols))
        self.table_jango.setHorizontalHeaderLabels(cols)
        self.table_jango.verticalHeader().setVisible(False)
        self.table_jango.verticalHeader().setDefaultSectionSize(18)
        self.table_jango.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table_jango.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.table_jango.horizontalHeader().setStretchLastSection(True)
        for col, width in enumerate([80, 60, 60, 80, 100]):    # code wide enough, qty columns narrow
            self.table_jango.setColumnWidth(col, width)
        self.table_jango.setFixedHeight(140)  # 헤더 + 6줄 고정
        self.table_jango.cellDoubleClicked.connect(self._on_jango_double_clicked)
        v.addWidget(self.table_jango)

        return w

    def _build_odr_gorae_tab(self):
        w = QWidget()
        v = QVBoxLayout(w)

        h = QHBoxLayout()
        h.addWidget(QLabel("시작일"))
        self.edit_odr_gorae_sdate = QLineEdit()
        self.edit_odr_gorae_sdate.setPlaceholderText("YYYYMMDD")
        h.addWidget(self.edit_odr_gorae_sdate)
        h.addWidget(QLabel("종료일"))
        self.edit_odr_gorae_edate = QLineEdit()
        self.edit_odr_gorae_edate.setPlaceholderText("YYYYMMDD (비우면 오늘)")
        h.addWidget(self.edit_odr_gorae_edate)
        h.addWidget(QLabel("조회구분"))
        self.combo_odr_gorae_type = QComboBox()
        for text, val in [("전체", "0"), ("입출금", "1"), ("입출고", "2"), ("매매", "3"),
                           ("매수", "E"), ("매도", "F"), ("기타", "4"), ("담보대출", "5")]:
            self.combo_odr_gorae_type.addItem(text, val)
        h.addWidget(self.combo_odr_gorae_type)
        h.addWidget(QLabel("종목코드"))
        self.edit_odr_gorae_code = QLineEdit()
        h.addWidget(self.edit_odr_gorae_code)
        btn = QPushButton("조회")
        btn.clicked.connect(self._on_odr_gorae_send)
        h.addWidget(btn)
        h.addStretch()
        v.addLayout(h)

        cols = ["거래일자", "적요명", "종목명", "거래수량", "거래단가", "거래금액", "수수료", "세금합계", "처리시각"]
        self.table_odr_gorae = QTableWidget(0, len(cols))
        self.table_odr_gorae.setHorizontalHeaderLabels(cols)
        self.table_odr_gorae.verticalHeader().setVisible(False)
        self.table_odr_gorae.verticalHeader().setDefaultSectionSize(18)
        self.table_odr_gorae.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table_odr_gorae.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.table_odr_gorae.horizontalHeader().setStretchLastSection(True)
        self.table_odr_gorae.setFixedHeight(140)
        v.addWidget(self.table_odr_gorae)

        return w

    def _build_overseas_group(self):
        group = QGroupBox("해외거래")
        v = QVBoxLayout(group)

        # 계좌/비밀번호는 탭 공통 - 탭마다 다시 고를 필요 없게 그룹 상단에 하나만 둔다.
        acc_h = QHBoxLayout()
        acc_h.addWidget(QLabel("계좌"))
        self.combo_ovs_accn = QComboBox()
        self.combo_ovs_accn.setEditable(True)  # 직접 타이핑해서 다른 계좌번호로 테스트 가능하게
        self.combo_ovs_accn.setMinimumWidth(260)
        self.combo_ovs_accn.setSizeAdjustPolicy(QComboBox.AdjustToContents)
        acc_h.addWidget(self.combo_ovs_accn)
        acc_h.addWidget(QLabel("비밀번호"))
        self.edit_ovs_pswd = QLineEdit()
        self.edit_ovs_pswd.setEchoMode(QLineEdit.Password)
        acc_h.addWidget(self.edit_ovs_pswd)
        acc_h.addStretch()
        v.addLayout(acc_h)

        tabs = QTabWidget()
        v.addWidget(tabs)

        tabs.addTab(self._build_ovs_jango_tab(), "잔고(TR5001)")
        tabs.addTab(self._build_ovs_chegyul_tab(), "체결내역(TR5002)")
        tabs.addTab(self._build_ovs_gorae_tab(), "거래내역(TR5003)")
        tabs.addTab(self._build_ovs_yesugeum_tab(), "예수금(TR5000)")
        if IS_FOREIGN:
            tabs.addTab(self._build_ovs_hwanjeon_tab(), "환전(TR5005/5006)")
            tabs.addTab(self._build_ovs_gajungsan_tab(), "가정산(TR5011)")
        return group

    def _build_ovs_jango_tab(self):
        w = QWidget()
        v = QVBoxLayout(w)

        h = QHBoxLayout()
        h.addWidget(QLabel("기준"))
        self.combo_ovs5001_today = QComboBox()
        self.combo_ovs5001_today.addItem("당일", "1")
        self.combo_ovs5001_today.addItem("일별", "2")
        h.addWidget(self.combo_ovs5001_today)
        h.addWidget(QLabel("조회일자"))
        self.edit_ovs5001_date = QLineEdit()
        self.edit_ovs5001_date.setPlaceholderText("YYYYMMDD (비우면 오늘)")
        h.addWidget(self.edit_ovs5001_date)
        h.addWidget(QLabel("조회구분"))
        self.combo_ovs5001_qrytp = QComboBox()
        for text, val in [("전체", "0"), ("홍콩", "1"), ("중국", "2"), ("미국", "3")]:
            self.combo_ovs5001_qrytp.addItem(text, val)
        h.addWidget(self.combo_ovs5001_qrytp)
        btn = QPushButton("조회")
        btn.clicked.connect(self._on_ovs_jango_send)
        h.addWidget(btn)
        h.addStretch()
        v.addLayout(h)

        cols = ["종목코드", "종목명", "통화", "보유수량", "매도가능수량", "현재가", "외화평가금액", "외화평가손익", "수익률"]
        self.table_ovs_jango = QTableWidget(0, len(cols))
        self.table_ovs_jango.setHorizontalHeaderLabels(cols)
        self.table_ovs_jango.verticalHeader().setVisible(False)
        self.table_ovs_jango.verticalHeader().setDefaultSectionSize(18)
        self.table_ovs_jango.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table_ovs_jango.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.table_ovs_jango.horizontalHeader().setStretchLastSection(True)
        self.table_ovs_jango.setFixedHeight(140)
        v.addWidget(self.table_ovs_jango)

        return w

    def _build_ovs_chegyul_tab(self):
        w = QWidget()
        v = QVBoxLayout(w)
        grid = QGridLayout()

        self.edit_ovs5002_code = QLineEdit()
        self.edit_ovs5002_edate = QLineEdit()
        self.edit_ovs5002_edate.setPlaceholderText("YYYYMMDD (비우면 오늘)")
        # OCX S_TR5002: 이 값은 시작일(SrtDt), 종료일(EndDt)은 OCX가 항상 오늘로 보낸다 (개발자 2026-10 수정)

        self.combo_ovs5002_mm = QComboBox()
        for text, val in [("전체", "%"), ("매도", "1"), ("매수", "2"), ("단주매도", "8"), ("단주취소", "9")]:
            self.combo_ovs5002_mm.addItem(text, val)

        self.combo_ovs5002_gubn = QComboBox()
        for text, val in [("전체", "0"), ("미체결", "1"), ("체결", "2")]:
            self.combo_ovs5002_gubn.addItem(text, val)

        self.combo_ovs5002_type = QComboBox()
        for text, val in [("전체", "A"), ("접수전", "2"), ("접수완료", "0"), ("거부", "4")]:
            self.combo_ovs5002_type.addItem(text, val)

        self.combo_ovs5002_market = QComboBox()
        for text, val in [("전체", "99"), ("NYSE", "81"), ("NASDAQ", "82"), ("AMEX", "87"),
                           ("홍콩", "71"), ("상해", "72"), ("심천", "73")]:
            self.combo_ovs5002_market.addItem(text, val)

        self.combo_ovs5002_ordtype = QComboBox()
        for text, val in [("지정가", "00"), ("시장가", "03"), ("MOC", "B1"), ("LOC", "B2"),
                           ("TWAP", "A2"), ("VWAP", "A3"), ("Limit TWAP", "A4"), ("Limit VWAP", "A5")]:
            self.combo_ovs5002_ordtype.addItem(text, val)

        self.combo_ovs5002_sort = QComboBox()
        self.combo_ovs5002_sort.addItem("정순", 0)
        self.combo_ovs5002_sort.addItem("역순", 1)

        fields_row1 = [("종목코드", self.edit_ovs5002_code), ("조회시작일", self.edit_ovs5002_edate),
                       ("매매구분", self.combo_ovs5002_mm), ("체결여부", self.combo_ovs5002_gubn)]
        fields_row2 = [("접수상태", self.combo_ovs5002_type), ("등록시장", self.combo_ovs5002_market),
                       ("주문타입", self.combo_ovs5002_ordtype), ("정렬", self.combo_ovs5002_sort)]
        for row, fields in enumerate([fields_row1, fields_row2]):
            for col, (label, widget) in enumerate(fields):
                grid.addWidget(QLabel(label), row, col*2)
                grid.addWidget(widget, row, col*2+1)

        btn = QPushButton("조회")
        btn.clicked.connect(self._on_ovs_chegyul_send)
        grid.addWidget(btn, 2, 0, 1, 2)
        v.addLayout(grid)

        cols = ["주문번호", "원주문번호", "종목", "구분", "주문량", "체결량", "미체결량",
                "주문가", "체결가", "주문시각", "체결시각", "거부사유"]
        self.table_ovs_chegyul = QTableWidget(0, len(cols))
        self.table_ovs_chegyul.setHorizontalHeaderLabels(cols)
        self.table_ovs_chegyul.verticalHeader().setVisible(False)
        self.table_ovs_chegyul.verticalHeader().setDefaultSectionSize(18)
        self.table_ovs_chegyul.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table_ovs_chegyul.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.table_ovs_chegyul.horizontalHeader().setStretchLastSection(True)
        self.table_ovs_chegyul.setFixedHeight(140)
        v.addWidget(self.table_ovs_chegyul)

        return w

    def _build_ovs_gorae_tab(self):
        w = QWidget()
        v = QVBoxLayout(w)

        h = QHBoxLayout()
        h.addWidget(QLabel("시작일"))
        self.edit_ovs5003_sdate = QLineEdit()
        self.edit_ovs5003_sdate.setPlaceholderText("YYYYMMDD (비우면 1주일 전)")
        h.addWidget(self.edit_ovs5003_sdate)
        h.addWidget(QLabel("종료일"))
        self.edit_ovs5003_edate = QLineEdit()
        self.edit_ovs5003_edate.setPlaceholderText("YYYYMMDD (비우면 오늘)")
        h.addWidget(self.edit_ovs5003_edate)
        h.addWidget(QLabel("종목코드"))
        self.edit_ovs5003_code = QLineEdit()
        h.addWidget(self.edit_ovs5003_code)
        v.addLayout(h)

        h2 = QHBoxLayout()
        h2.addWidget(QLabel("거래구분"))
        self.combo_ovs5003_gubn = QComboBox()
        for text, val in [("전체", "00"), ("출납", "10"), ("입출금", "11"), ("입출고", "12"),
                           ("외화입출금", "13"), ("환전", "14"), ("매매내역", "20"),
                           ("대금결제", "21"), ("유가결제", "22"), ("과세내역", "30")]:
            self.combo_ovs5003_gubn.addItem(text, val)
        h2.addWidget(self.combo_ovs5003_gubn)
        h2.addWidget(QLabel("주식구분"))
        self.combo_ovs5003_stktp = QComboBox()
        for text, val in [("전체", "0"), ("온주", "1"), ("소수점", "2"), ("적립식", "3")]:
            self.combo_ovs5003_stktp.addItem(text, val)
        h2.addWidget(self.combo_ovs5003_stktp)
        btn = QPushButton("조회")
        btn.clicked.connect(self._on_ovs_gorae_send)
        h2.addWidget(btn)
        # 연속조회: 응답의 b_next=True면 활성화, 누르면 받은 nkey로 다음 페이지를 이어서 조회
        self.btn_ovs5003_next = QPushButton("다음 조회")
        self.btn_ovs5003_next.setEnabled(False)
        self.btn_ovs5003_next.clicked.connect(self._on_ovs_gorae_next)
        h2.addWidget(self.btn_ovs5003_next)
        self.lbl_ovs5003_next = QLabel("")
        h2.addWidget(self.lbl_ovs5003_next)
        h2.addStretch()
        v.addLayout(h2)

        cols = ["거래일자", "적요명", "종목명", "통화", "수량", "단가", "외화금액", "원화금액", "시장명", "처리시각"]
        self.table_ovs_gorae = QTableWidget(0, len(cols))
        self.table_ovs_gorae.setHorizontalHeaderLabels(cols)
        self.table_ovs_gorae.verticalHeader().setVisible(False)
        self.table_ovs_gorae.verticalHeader().setDefaultSectionSize(18)
        self.table_ovs_gorae.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table_ovs_gorae.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.table_ovs_gorae.horizontalHeader().setStretchLastSection(True)
        self.table_ovs_gorae.setFixedHeight(140)
        v.addWidget(self.table_ovs_gorae)

        return w

    def _build_ovs_gajungsan_tab(self):
        w = QWidget()
        v = QVBoxLayout(w)

        h = QHBoxLayout()
        h.addWidget(QLabel("주문일"))
        self.edit_ovs5011_date = QLineEdit()
        self.edit_ovs5011_date.setPlaceholderText("YYYYMMDD (비우면 오늘)")
        h.addWidget(self.edit_ovs5011_date)
        h.addWidget(QLabel("종목코드"))
        self.edit_ovs5011_code = QLineEdit()
        self.edit_ovs5011_code.setPlaceholderText("비우면 전체")
        h.addWidget(self.edit_ovs5011_code)
        btn = QPushButton("조회")
        btn.clicked.connect(self._on_ovs_gajungsan_send)
        h.addWidget(btn)
        # 연속조회: 응답의 b_next=True면 활성화, 누르면 받은 nkey로 다음 페이지를 이어서 조회
        self.btn_ovs5011_next = QPushButton("다음 조회")
        self.btn_ovs5011_next.setEnabled(False)
        self.btn_ovs5011_next.clicked.connect(self._on_ovs_gajungsan_next)
        h.addWidget(self.btn_ovs5011_next)
        self.lbl_ovs5011_next = QLabel("")
        h.addWidget(self.lbl_ovs5011_next)
        h.addStretch()
        v.addLayout(h)

        cols = ["매매구분", "종목번호", "종목명", "체결수량", "평균가", "체결금액", "수수료", "정산금액", "통화", "시장명"]
        self.table_ovs_gajungsan = QTableWidget(0, len(cols))
        self.table_ovs_gajungsan.setHorizontalHeaderLabels(cols)
        self.table_ovs_gajungsan.verticalHeader().setVisible(False)
        self.table_ovs_gajungsan.verticalHeader().setDefaultSectionSize(18)
        self.table_ovs_gajungsan.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table_ovs_gajungsan.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.table_ovs_gajungsan.horizontalHeader().setStretchLastSection(True)
        self.table_ovs_gajungsan.setFixedHeight(140)
        v.addWidget(self.table_ovs_gajungsan)

        return w

    def _build_ovs_yesugeum_tab(self):
        w = QWidget()
        v = QVBoxLayout(w)

        h = QHBoxLayout()
        btn = QPushButton("조회")
        btn.clicked.connect(self._on_ovs_yesugeum_send)
        h.addWidget(btn)
        h.addStretch()
        v.addLayout(h)

        # 원화 기준 요약 (COSAQ027 out3)
        sum_h = QHBoxLayout()
        def make_sum(label):
            sum_h.addWidget(QLabel(label))
            lbl = QLabel("-")
            lbl.setMinimumWidth(90)
            sum_h.addWidget(lbl)
            return lbl
        self.lbl_ovs_wondps = make_sum("원화예수금")
        self.lbl_ovs_wonout = make_sum("원화출금가능금액")
        self.lbl_ovs_wonconv = make_sum("원화환산거래금액")
        self.lbl_ovs_bnsmgn = make_sum("매매증거금")
        sum_h.addStretch()
        v.addLayout(sum_h)

        # 통화별 상세 (COSAQ027 out2 반복)
        cols = ["통화", "외화예수금", "외화주문가능금액", "외화주문금액", "외화담보금액",
                "외화환전가능금", "미수금", "외화상환금액"]
        self.table_ovs_yesugeum = QTableWidget(0, len(cols))
        self.table_ovs_yesugeum.setHorizontalHeaderLabels(cols)
        self.table_ovs_yesugeum.verticalHeader().setVisible(False)
        self.table_ovs_yesugeum.verticalHeader().setDefaultSectionSize(18)
        self.table_ovs_yesugeum.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table_ovs_yesugeum.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.table_ovs_yesugeum.horizontalHeader().setStretchLastSection(True)
        self.table_ovs_yesugeum.setFixedHeight(120)
        v.addWidget(self.table_ovs_yesugeum)

        return w

    def _build_ovs_hwanjeon_tab(self):
        w = QWidget()
        v = QVBoxLayout(w)
        form = QFormLayout()

        # TR5010A: 중계은행(SDPWQ002)만 조회해서 응답을 그대로 돌려준다 (다음 조회는 자동으로 안 함).
        # 은행코드는 OCX 내부에도 저장되며 TR5006이 이 값을 쓴다.
        bank_h = QHBoxLayout()
        self.combo_hwj_bank_crcy = QComboBox()
        for c in ["USD", "AUD", "HKD", "CNY"]:
            self.combo_hwj_bank_crcy.addItem(c, c)
        bank_h.addWidget(self.combo_hwj_bank_crcy)
        btn_bank = QPushButton("중계은행조회(TR5010A)")
        btn_bank.clicked.connect(self._on_hwj_bank_send)
        bank_h.addWidget(btn_bank)
        self.lbl_hwj_bank = QLabel("-")
        bank_h.addWidget(self.lbl_hwj_bank)
        bank_h.addStretch()
        form.addRow("중계은행", bank_h)

        # 중계은행 응답을 보고 해당 은행 버튼을 사용자가 직접 누른다 (003 기업 -> TR5007, 004 국민 -> TR5004).
        # 통화는 위 중계은행 통화 콤보 값을 같이 쓴다.
        brate_h = QHBoxLayout()
        btn_ibk = QPushButton("기업환율조회(TR5007)")
        btn_ibk.clicked.connect(lambda: self._on_hwj_bankrate_send("TR5007", TK_TR5007))
        brate_h.addWidget(btn_ibk)
        btn_kb = QPushButton("국민환율조회(TR5004)")
        btn_kb.clicked.connect(lambda: self._on_hwj_bankrate_send("TR5004", TK_TR5004))
        brate_h.addWidget(btn_kb)
        self.lbl_hwj_bankrate = QLabel("-")
        brate_h.addWidget(self.lbl_hwj_bankrate)
        brate_h.addStretch()
        form.addRow("은행환율", brate_h)

        self.combo_hwj_bnstp = QComboBox()
        self.combo_hwj_bnstp.addItem("외화매수", "01")
        self.combo_hwj_bnstp.addItem("외화매도", "02")
        self.combo_hwj_bnstp.setMaximumWidth(100)
        form.addRow("매매구분", self.combo_hwj_bnstp)

        crcy_h = QHBoxLayout()
        self.combo_hwj_crcy_from = QComboBox()
        self.combo_hwj_crcy_to = QComboBox()
        # TR5006 대상통화(출금) -> 환전통화(입금). 매매구분에 따라 KRW 쪽을 고정한다
        # (외화매수: KRW -> 외화, 외화매도: 외화 -> KRW)
        self.combo_hwj_bnstp.currentIndexChanged.connect(self._on_hwj_bnstp_changed)
        self._on_hwj_bnstp_changed()
        crcy_h.addWidget(QLabel("출금"))
        crcy_h.addWidget(self.combo_hwj_crcy_from)
        crcy_h.addWidget(QLabel("→ 입금"))
        crcy_h.addWidget(self.combo_hwj_crcy_to)
        # TR5005 입력 환율: 위 은행환율 결과(매도율/매입율)를 보고 사용자가 직접 넣는다
        crcy_h.addWidget(QLabel("기준환율"))
        self.edit_hwj_gdrate = QLineEdit("")
        self.edit_hwj_gdrate.setMaximumWidth(100)
        crcy_h.addWidget(self.edit_hwj_gdrate)
        btn_rate = QPushButton("적용환율조회(TR5005)")
        btn_rate.clicked.connect(self._on_hwj_rate_send)
        crcy_h.addWidget(btn_rate)
        crcy_h.addWidget(QLabel("적용환율은 10초간 유지됩니다."))
        crcy_h.addStretch()
        form.addRow("통화코드", crcy_h)

        rate_h = QHBoxLayout()
        self.edit_hwj_rate = QLineEdit("0.00")
        self.edit_hwj_rate.setReadOnly(True)
        rate_h.addWidget(self.edit_hwj_rate)
        self.lbl_hwj_ratetime = QLabel("(최종조회시간: 00:00:00)")
        rate_h.addWidget(self.lbl_hwj_ratetime)
        rate_h.addWidget(QLabel("우대율"))
        self.lbl_hwj_prvlg = QLabel("-")
        rate_h.addWidget(self.lbl_hwj_prvlg)
        rate_h.addStretch()
        form.addRow("적용환율", rate_h)

        EDIT_W = 100
        self.edit_hwj_rate.setMaximumWidth(EDIT_W)

        amt_h = QHBoxLayout()
        self.edit_hwj_fcurramt = QLineEdit("0.00")
        self.edit_hwj_fcurramt.setMaximumWidth(EDIT_W)
        amt_h.addWidget(self.edit_hwj_fcurramt)
        btn_est = QPushButton("환전예상조회(TR5006)")
        btn_est.clicked.connect(lambda: self._send_tr5006(TK_TR5006_EST, "0"))
        amt_h.addWidget(btn_est)
        amt_h.addStretch()
        form.addRow("환전금액(외화)", amt_h)

        # 예상조회 응답값을 받은 형식 그대로 표시하고, 실제 환전(trxtp=1) 때 이 값을 그대로 보낸다
        won_h = QHBoxLayout()
        won_h.addWidget(QLabel("원화"))
        self.edit_hwj_wonamt = QLineEdit("")
        self.edit_hwj_wonamt.setReadOnly(True)
        self.edit_hwj_wonamt.setMinimumWidth(140)
        won_h.addWidget(self.edit_hwj_wonamt)
        won_h.addWidget(QLabel("외화"))
        self.edit_hwj_fcurr_res = QLineEdit("")
        self.edit_hwj_fcurr_res.setReadOnly(True)
        self.edit_hwj_fcurr_res.setMinimumWidth(140)
        won_h.addWidget(self.edit_hwj_fcurr_res)
        btn_exec = QPushButton("외화 환전(TR5006 실행)")
        btn_exec.clicked.connect(self._on_hwj_exec_send)
        won_h.addWidget(btn_exec)
        won_h.addStretch()
        form.addRow("예상금액", won_h)

        v.addLayout(form)

        result_h = QHBoxLayout()
        result_h.addWidget(QLabel("처리결과"))
        self.lbl_hwj_result = QLabel("-")
        result_h.addWidget(self.lbl_hwj_result)
        result_h.addWidget(QLabel("거래번호"))
        self.lbl_hwj_trno = QLabel("-")
        result_h.addWidget(self.lbl_hwj_trno)
        result_h.addStretch()
        v.addLayout(result_h)
        v.addStretch()

        # 응답 값을 마우스로 드래그 선택/복사할 수 있게 한다 (예: 증권사 매도율 -> 기준환율)
        for lbl in (self.lbl_hwj_bank, self.lbl_hwj_bankrate, self.lbl_hwj_prvlg,
                    self.lbl_hwj_result, self.lbl_hwj_trno):
            lbl.setTextInteractionFlags(Qt.TextSelectableByMouse)

        return w

    def _add_ovs_yesugeum_row(self, crcy, dps, ordable, ordamt, pldg, mxchg, rcvbl, fund):
        row = self.table_ovs_yesugeum.rowCount()
        self.table_ovs_yesugeum.insertRow(row)
        for col, val in enumerate([crcy, dps, ordable, ordamt, pldg, mxchg, rcvbl, fund]):
            self.table_ovs_yesugeum.setItem(row, col, QTableWidgetItem(val))

    def _add_ovs_jango_row(self, code, name, crcy, hld, sellable, curr, eval_amt, eval_pflt, rate):
        row = self.table_ovs_jango.rowCount()
        self.table_ovs_jango.insertRow(row)
        for col, val in enumerate([code, name, crcy, hld, sellable, curr, eval_amt, eval_pflt, rate]):
            self.table_ovs_jango.setItem(row, col, QTableWidgetItem(val))

    def _add_chegyul_row(self, mode, jmno, ojno, jong, jprc, jqty, cprc, cqty, michegyul, msg):
        table = self.table_michegyul if mode == "미체결" else self.table_chegyul
        row = table.rowCount()
        table.insertRow(row)
        for col, val in enumerate([jmno, ojno, jong, jprc, jqty, cprc, cqty, michegyul, msg]):
            table.setItem(row, col, QTableWidgetItem(val))

    def _add_odr_gorae_row(self, tdate, smry, isunm, qty, uprc, amt, cmsn, tax, ttime):
        row = self.table_odr_gorae.rowCount()
        self.table_odr_gorae.insertRow(row)
        for col, val in enumerate([tdate, smry, isunm, qty, uprc, amt, cmsn, tax, ttime]):
            self.table_odr_gorae.setItem(row, col, QTableWidgetItem(val))

    def _add_ovs_gorae_row(self, tdate, smry, isunm, crcy, qty, uprc, fcurr, won, mkt, ttime):
        row = self.table_ovs_gorae.rowCount()
        self.table_ovs_gorae.insertRow(row)
        for col, val in enumerate([tdate, smry, isunm, crcy, qty, uprc, fcurr, won, mkt, ttime]):
            self.table_ovs_gorae.setItem(row, col, QTableWidgetItem(val))

    @staticmethod
    def _fmt_num(v):
        # "+000000000038.4000" -> "38.40", "+000000000000003" -> "3" (sign kept only when negative)
        try:
            d = Decimal(v)
        except Exception:
            return v
        if d == d.to_integral_value() and "." not in v:
            return f"{int(d):,}"
        txt = f"{d:,.4f}".rstrip("0")
        return txt + "0" * max(0, 2 - len(txt.split(".")[1]))   # at least 2 decimals

    def _add_ovs_gajungsan_row(self, vals):
        # server mixes detail rows with "▶ 소계"(per-symbol subtotal) and a blank total row at the end
        is_total = not vals[0] and not vals[1]
        is_summary = is_total or "소계" in vals[0]
        if is_total:
            vals[0] = "합계"
        for col in range(3, 8):                     # 체결수량 ~ 정산금액
            vals[col] = self._fmt_num(vals[col])
        row = self.table_ovs_gajungsan.rowCount()
        self.table_ovs_gajungsan.insertRow(row)
        for col, val in enumerate(vals):
            item = QTableWidgetItem(val)
            if col >= 3 and col <= 7:
                item.setTextAlignment(Qt.AlignRight | Qt.AlignVCenter)
            if is_summary:
                item.setBackground(QColor(230, 230, 230))
            self.table_ovs_gajungsan.setItem(row, col, item)

    def _add_ovs_chegyul_row(self,jmno, ojno, jong, gubn, jqty, cqty, michegyul, jprc, cprc, otime, ctime, rjt):
        row = self.table_ovs_chegyul.rowCount()
        self.table_ovs_chegyul.insertRow(row)
        for col, val in enumerate([jmno, ojno, jong, gubn, jqty, cqty, michegyul, jprc, cprc, otime, ctime, rjt]):
            self.table_ovs_chegyul.setItem(row, col, QTableWidgetItem(val))

    def _add_jango_row(self, code, remain, avail, maip, curr, eval_amt, rate):
        row = self.table_jango.rowCount()
        self.table_jango.insertRow(row)
        for col, val in enumerate([code, remain, avail, curr, eval_amt, rate]):
            self.table_jango.setItem(row, col, QTableWidgetItem(val))
        code_key = code.lstrip('A')
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

    def _update_jango_row(self, code, curr=None, eval_amt=None, rate=None):
        row = self._jango_row_by_code.get(code.lstrip('A'))
        if row is None:
            return
        for col, val in [(3, curr), (4, eval_amt), (5, rate)]:
            if val is not None:
                self.table_jango.setItem(row, col, QTableWidgetItem(val))

    def _recalc_jango_on_price(self, code, new_curr_s):
        # OnRealData로 들어온 현재가로 잔고 종목의 평가금액/수익률을 실시간 재계산
        code_key = code.lstrip('A')
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
        rate = (new_eval - maip) * 100 / maip if maip else 0.0
        self._update_jango_row(code_key, curr=f"{new_curr:.0f}", eval_amt=f"{new_eval:.0f}", rate=f"{rate:.2f}")

    def _on_jango_double_clicked(self, row, col):
        item = self.table_jango.item(row, col)
        if item is None:
            return
        text = item.text().strip()
        if col == 0:  # 종목코드 -> 주문 종목코드 ('A' 접두문자 포함 그대로)
            self.edit_odr_code.setText(text)
        elif col == 3:  # 현재가 -> 주문가격 (부호는 등락 표시이므로 제거)
            self.edit_odr_jprc.setText(text.lstrip('+-'))

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
            QMessageBox.warning(self, "요청 제한", err)

    def _check_result(self, result):
        # None = COM 호출 자체가 실패(OCX 메서드 시그니처 불일치 등) → OCX 내부 메시지는 이전 것이라 조회하지 않는다
        if result is None:
            self._log("  오류: OCX 호출 실패 (인터페이스 불일치 - 인자 개수/타입 확인 필요)")
        elif not result:
            self._log_err(self.ocx.dynamicCall('GetLastErrMsg()'))

    def _on_initialize(self):
        result = self.ocx.dynamicCall("Initialize()")
        self.lbl_init.setText(str(result))
        self._log(f"Initialize() => {result}")
        self._log(f"OCX path: {self._get_ocx_path()}")
        if not result:
            self._log_err(self.ocx.dynamicCall("GetLastErrMsg()"))

    def _get_ocx_path(self):
        reg_key = f"CLSID\\{OCX_GUID}\\InprocServer32"
        try:
            key = winreg.OpenKey(winreg.HKEY_CLASSES_ROOT, reg_key)
            path, _ = winreg.QueryValueEx(key, "")
            winreg.CloseKey(key)
            return path
        except Exception as e:
            return f"(registry read failed: {e})"

    def _on_login(self):
        user_id  = self.edit_user_id.text()
        user_pw  = self.edit_user_pw.text()
        cert_pw  = self.edit_cert_pw.text()
        if IS_FOREIGN:
            # 직접 입력한 svr_ip가 있으면 그것을, 없으면 서버콤보의 IP(화면 비노출)를 쓴다.
            typed_ip = self.edit_svr_ip.text().strip()
            svr_ip   = typed_ip or self.combo_server.currentData()
            svr_name = "직접입력" if typed_ip else (self.combo_server.currentText().split(" (")[0] or "glb")   # log name only, no IP
        else:
            svr_ip, svr_name = "", "glb"    # customer: OCX resolves the production server via glb
        svr_port = SVR_PORT
        self._login_svr_ip = svr_ip         # saved with login info when OnLogin succeeds

        # svr_ip가 비어 있으면 OCX가 glb로 서버 IP를 조회한다.
        if IS_ADMIN and self.chk_quote_only.isChecked():
            result = self.ocx.dynamicCall(
                "LoginQuote(QString, QString, QString, int)",
                user_id, user_pw, svr_ip, svr_port
            )
            self.lbl_login.setText(str(result))
            self._log(f"LoginQuote({user_id}, ***, 서버={svr_name}) => {result}")
            return

        result = self.ocx.dynamicCall(
            "Login(QString, QString, QString, QString, int)",
            user_id, user_pw, cert_pw, svr_ip, svr_port
        )
        self.lbl_login.setText(str(result))
        self._log(f"Login({user_id}, ***, ***, 서버={svr_name}) => {result}")

    def _on_logout(self):
        self.ocx.dynamicCall("Logout()")
        self.lbl_login.setText("-")
        self._log("Logout() called")

    def _on_server_selected(self, idx):
        # 콤보 서버를 고르면 IP를 화면에 채우지 않고 직접입력란을 비운다 (IP 비노출)
        if self.combo_server.itemData(idx):
            self.edit_svr_ip.clear()

    def _on_sise_send(self):
        code = self.edit_sise_code.text().strip()
        mkgubn = self.combo_mkgubn.currentData()  # 1=KRX 2=NXT 3=통합
        for lbl in (self.lbl_curr, self.lbl_diff, self.lbl_rate, self.lbl_gvol,
                    self.lbl_siga, self.lbl_koga, self.lbl_jega, self.lbl_mgjv,
                    self.lbl_dvol, self.lbl_svol):
            lbl.setText("-")
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

    # ── OCX event handlers ───────────────────────────────────────

    def _evt_login(self, b_login, msg):
        status = "OK" if b_login else "FAIL"
        self.lbl_login.setText(status)
        self._log(f"[EVT] OnLogin({status}) {msg}")

        if b_login:
            home = self.ocx.dynamicCall("GetHome()")
            self._log(f"  OCX 홈 경로: {home}")
            self._load_accounts()
            if IS_ADMIN and self.chk_save_login.isChecked():
                self._save_login_info()

    def _load_accounts(self):
        # GetAccounts() returns "계좌번호\t계좌명\n계좌번호\t계좌명" format
        raw = self.ocx.dynamicCall("GetAccounts()")
        self.combo_odr_accn.clear()
        if IS_FOREIGN:
            self.combo_ovs_accn.clear()
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
            self.combo_odr_accn.addItem(display, acno)
            if IS_FOREIGN:
                self.combo_ovs_accn.addItem(display, acno)

    def _evt_error(self, msg):
        self._log(f"[EVT] OnError: {msg}")

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

        # 접수/체결/취소 어느 쪽이든 가능수량(xqty, 매도가능)에 영향을 주므로
        # 상태 구분 없이 재조회한다. 실시간 시세와 무관하게 주문/체결 시점에만 오는
        # 저빈도 이벤트라 OnSBalance 대신 여기서 잔고를 다시 조회해도 안전하다.
        self._on_michegyul_send()

    def _evt_sbalance(self, data):
        # 실제 필드 포맷이 화면 설정(m_arField)에 따라 달라지는 레거시 프로토콜이라
        # 신뢰할 수 있는 매핑을 알 수 없어 무시한다 (2026-07-21 논의).
        # 잔고 갱신은 OnContract(체결통보) 수신 시 TR1223 재조회로 처리한다.
        pass

    def _login_save_path(self):
        return os.path.join(os.path.dirname(self._get_ocx_path()), LOGIN_SAVE_FILE)

    def _save_login_info(self):
        info = {"id": self.edit_user_id.text(), "pw": self.edit_user_pw.text(),
                "cert": self.edit_cert_pw.text(), "svr": getattr(self, "_login_svr_ip", ""), "save": True}
        try:
            with open(self._login_save_path(), "wb") as f:
                f.write(_dpapi(json.dumps(info).encode("utf-8"), True))
            self._log("[접속정보] 저장 완료 (DPAPI 암호화)")
        except Exception as e:
            self._log(f"[접속정보] 저장 실패: {e}")

    def _load_login_info(self):
        path = self._login_save_path()
        try:
            with open(path, "rb") as f:
                blob = f.read()
            if not blob:
                return
            info = json.loads(_dpapi(blob, False).decode("utf-8"))
        except FileNotFoundError:
            return
        except Exception as e:      # other PC/user, or corrupted -> start empty
            self._log(f"[접속정보] 불러오기 실패: {e}")
            return
        self.edit_user_id.setText(info.get("id", ""))
        self.edit_user_pw.setText(info.get("pw", ""))
        self.edit_cert_pw.setText(info.get("cert", ""))
        svr = info.get("svr", "")
        idx = self.combo_server.findData(svr) if svr else 0
        if idx > 0:
            self.combo_server.setCurrentIndex(idx)      # one of SERVER_LIST
        else:
            self.edit_svr_ip.setText(svr)               # direct input IP (or empty = glb)
        self.chk_save_login.setChecked(bool(info.get("save")))
        self._log("[접속정보] 저장된 접속정보 불러옴")

    def _on_save_login_toggled(self, checked):
        if checked:
            return                  # saved on next successful login
        try:
            open(self._login_save_path(), "wb").close()     # clear file contents
            self._log("[접속정보] 저장 해제 - 파일 내용 삭제")
        except Exception as e:
            self._log(f"[접속정보] 파일 삭제 실패: {e}")

    def _on_full_redownload(self):
        # home = parent of the OCX folder (<home>\exe\IBKSConnector.ocx), same rule as _evt_ver_update
        exe_dir = os.path.dirname(self._get_ocx_path())
        tab_dir = os.path.join(os.path.dirname(exe_dir), "tab")
        targets = [os.path.join(tab_dir, f) for f in ("infoAXIS", "infoRSC")]

        if QMessageBox.question(self, "전체파일 다시받기",
                                "버전 파일(infoAXIS, infoRSC)을 삭제하고 프로그램을 재실행합니다.\n"
                                "재실행 후 로그인하면 전체 파일을 다시 받습니다.\n\n진행할까요?") != QMessageBox.Yes:
            return

        for path in targets:
            try:
                if os.path.exists(path):
                    os.remove(path)
                    self._log(f"[전체받기] 삭제: {path}")
                else:
                    self._log(f"[전체받기] 없음(건너뜀): {path}")
            except OSError as e:
                self._log(f"[전체받기] 삭제 실패: {path} ({e})")
                QMessageBox.warning(self, "전체파일 다시받기", f"삭제 실패:\n{path}\n{e}")
                return

        # relaunch self: frozen exe -> the exe itself, IDE run -> python + this script
        if getattr(sys, "frozen", False):
            args = [sys.executable]
        else:
            args = [sys.executable, os.path.abspath(__file__)]
        self._log(f"[전체받기] 재실행: {subprocess.list2cmdline(args)}")
        env = dict(os.environ, PYINSTALLER_RESET_ENVIRONMENT="1")
        subprocess.Popen(args, env=env, cwd=os.path.dirname(args[-1]))

        self.ocx.clear()                        # release OCX/DLL file locks
        QApplication.quit()

    def _evt_ver_update(self):
        self._log("[업데이트] OnVerUpdate: axisver에 복사/재실행 위임")
        # GetHome()은 로그인 완료 전엔 빈 값인데 OnVerUpdate는 로그인 도중에 온다.
        # 그래서 레지스트리에 등록된 OCX 위치(<home>\exe\IBKSConnector.ocx)에서 경로를 구한다.
        exe_dir = os.path.dirname(self._get_ocx_path())
        home    = os.path.dirname(exe_dir)
        axisver = os.path.join(exe_dir, "axisver.exe")

        if not getattr(sys, "frozen", False) or not os.path.exists(axisver):
            self._log(f"[업데이트] axisver 위임 불가 (frozen={getattr(sys, 'frozen', False)}, axisver={os.path.exists(axisver)})")
            QMessageBox.information(self, "업데이트", "업데이트 파일을 받았습니다.\n프로그램 종료 후 다시 실행해주세요.")
            return

        try:
            # axisver runs "<root>\exe\<apname>", so pass self path relative to exe dir
            rel = os.path.relpath(sys.executable, exe_dir)
        except ValueError:   # different drive
            QMessageBox.information(self, "업데이트", "업데이트 파일을 받았습니다.\n프로그램 종료 후 다시 실행해주세요.")
            return

        args = [axisver,
                "/c", str(int(self.winId())),   # WM_CLOSE target
                "/x", str(os.getpid()),         # wait/kill target (new axisver)
                "/d", home,                     # root (parent of exe)
                "/a", rel,                      # relaunch target
                "/u"]                           # MUST be last flag
        self._log(f"[업데이트] axisver 실행: {subprocess.list2cmdline(args)}")
        # 재실행되는 exe가 이 프로세스의 _MEI 임시폴더를 재사용하지 않고 새로 시작하도록 한다
        # (PyInstaller 6.9+, 미지정 시 "Failed to load Python DLL" 발생)
        env = dict(os.environ, PYINSTALLER_RESET_ENVIRONMENT="1")
        subprocess.Popen(args, env=env)

        self.ocx.clear()                        # release OCX/DLL file locks
        QApplication.quit()

    def _evt_update_start(self):
        self._log("[업데이트] 파일 업데이트 시작...")

    def _evt_updating(self, filename, percent):
        self._log(f"[업데이트] {filename} {percent}%")

    def _evt_update_end(self):
        self._log("[업데이트] 파일 업데이트 완료")

    def _on_tr8001(self):
        acno = self.combo_odr_accn.currentData() or ""
        if not acno.strip():
            self._log("국내 계좌를 선택하세요")
            return
        result = self.ocx.dynamicCall("TR8001(int, QString)", TK_TR8001, acno)
        self._log(f"고객정보조회 => {result}")
        if not result:
            self._log_err(self.ocx.dynamicCall('GetLastErrMsg()'))

    def _on_show_chaser(self):
        result = self.ocx.dynamicCall("ShowChaser()")
        self._log(f"체이서 실행 => {result}")
        if not result:
            self._log_err(self.ocx.dynamicCall('GetLastErrMsg()'))

    def _on_hide_chaser(self):
        self.ocx.dynamicCall("HideChaser()")
        self._log("체이서 종료 요청")

    def _on_odr_send(self):
        acno  = self.combo_odr_accn.currentData() or ""
        pswd  = self.edit_odr_pswd.text()
        code  = self.edit_odr_code.text().strip()
        mmgb  = self.combo_mmgb.currentData()
        hogb  = self.combo_hogb.currentData()
        jprc  = int(self.edit_odr_jprc.text() or "0")
        jqty  = int(self.edit_odr_jqty.text() or "0")
        ojno  = int(self.edit_odr_ojno.text() or "0")

        mkgb = self.combo_odr_mkgubn.currentData()  # 1=KRX 2=NXT 3=통합
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
        result = self.ocx.dynamicCall(  # dlgb=2
            "TR1211(int, QString, QString, QString, int, int, int, int, QString)",
            [TK_TR1211, acno, pswd, "", 0, 0, 2, 1, ""])
        self._chegyul_mode = "미체결"
        self._log(f"미체결조회 => {result}")
        if not result:
            self._log_err(self.ocx.dynamicCall('GetLastErrMsg()'))

    def _on_ovs_chegyul_send(self):
        acno = self.combo_ovs_accn.currentData() or self.combo_ovs_accn.currentText().strip()
        pswd = self.edit_ovs_pswd.text()
        if not acno.strip():
            self._log("계좌를 선택하세요")
            return
        code = self.edit_ovs5002_code.text()
        edate = self.edit_ovs5002_edate.text()
        mm = self.combo_ovs5002_mm.currentData()
        gubn = self.combo_ovs5002_gubn.currentData()
        rtype = self.combo_ovs5002_type.currentData()
        market = self.combo_ovs5002_market.currentData()
        ordtype = self.combo_ovs5002_ordtype.currentData()
        sort = self.combo_ovs5002_sort.currentData()
        self.table_ovs_chegyul.setRowCount(0)
        result = self.ocx.dynamicCall(
            "TR5002(int, QString, QString, QString, QString, QString, QString, QString, QString, QString, int, QString)",
            [TK_TR5002, acno, pswd, edate, mm, gubn, rtype, code, market, ordtype, sort, ""])
        self._log(f"해외체결내역조회(TR5002) => {result}")
        self._check_result(result)

    def _on_ovs_yesugeum_send(self):
        acno = self.combo_ovs_accn.currentData() or self.combo_ovs_accn.currentText().strip()
        pswd = self.edit_ovs_pswd.text()
        if not acno.strip():
            self._log("계좌를 선택하세요")
            return
        self.table_ovs_yesugeum.setRowCount(0)
        result = self.ocx.dynamicCall(
            "TR5000(int, QString, QString)",
            [TK_TR5000, acno, pswd])
        self._log(f"해외예수금조회(TR5000) => {result}")
        if not result:
            self._log_err(self.ocx.dynamicCall('GetLastErrMsg()'))

    def _on_ovs_jango_send(self):
        acno = self.combo_ovs_accn.currentData() or self.combo_ovs_accn.currentText().strip()
        pswd = self.edit_ovs_pswd.text()
        if not acno.strip():
            self._log("계좌를 선택하세요")
            return
        edate = self.edit_ovs5001_date.text()
        today = self.combo_ovs5001_today.currentData()
        qrytp = self.combo_ovs5001_qrytp.currentData()
        self.table_ovs_jango.setRowCount(0)
        # 참고: TR5001은 OCX의 ODL 선언(5개: key,acc,pswd,date,nkey)이 실제 구현
        # (7개: key,acc,pswd,sDate,today,qryTp,nkey)과 안 맞는다 - OCX ODL 수정 전까지는 None 반환.
        result = self.ocx.dynamicCall(
            "TR5001(int, QString, QString, QString, QString, QString, QString)",
            [TK_TR5001, acno, pswd, edate, today, qrytp, ""])
        self._log(f"해외잔고조회(TR5001) => {result}")
        self._check_result(result)

    def _on_odr_gorae_send(self):
        acno = self.combo_odr_accn.currentData() or ""
        pswd = self.edit_odr_pswd.text()
        if not acno.strip():
            self._log("계좌를 선택하세요")
            return
        sdate = self.edit_odr_gorae_sdate.text()
        edate = self.edit_odr_gorae_edate.text()
        qtype = self.combo_odr_gorae_type.currentData()
        code = self.edit_odr_gorae_code.text()
        self.table_odr_gorae.setRowCount(0)
        result = self.ocx.dynamicCall(
            "TR2002(int, QString, QString, QString, QString, QString, QString, QString)",
            [TK_TR2002, acno, pswd, sdate, edate, qtype, code, ""])
        self._log(f"주식거래내역조회(TR2002) => {result}")
        self._check_result(result)

    def _on_ovs_gorae_send(self):
        acno = self.combo_ovs_accn.currentData() or self.combo_ovs_accn.currentText().strip()
        pswd = self.edit_ovs_pswd.text()
        if not acno.strip():
            self._log("계좌를 선택하세요")
            return
        # 다음 조회 때 같은 조건으로 nkey만 바꿔 보내야 하므로 조회 조건을 저장해 둔다
        self._ovs5003_args = [acno, pswd,
                              self.edit_ovs5003_sdate.text(), self.edit_ovs5003_edate.text(),
                              self.edit_ovs5003_code.text(), "0",
                              self.combo_ovs5003_gubn.currentData(), self.combo_ovs5003_stktp.currentData()]
        self.table_ovs_gorae.setRowCount(0)
        self._send_tr5003("")

    def _on_ovs_gorae_next(self):
        self._send_tr5003(self._ovs5003_nkey)

    def _send_tr5003(self, nkey):
        # nkey는 OCX가 18바이트 고정으로 복사하므로 받은 값을 strip 없이 그대로 넘긴다
        self.btn_ovs5003_next.setEnabled(False)
        self.lbl_ovs5003_next.setText("")
        result = self.ocx.dynamicCall(
            "TR5003(int, QString, QString, QString, QString, QString, QString, QString, QString, QString)",
            [TK_TR5003] + self._ovs5003_args + [nkey])
        self._log(f"해외거래내역조회(TR5003, {'다음 nkey=' + repr(nkey) if nkey else '첫 조회'}) => {result}")
        self._check_result(result)

    def _on_ovs_gajungsan_send(self):
        acno = self.combo_ovs_accn.currentData() or self.combo_ovs_accn.currentText().strip()
        pswd = self.edit_ovs_pswd.text()
        if not acno.strip():
            self._log("계좌를 선택하세요")
            return
        # OCX copies Date as fixed 8 bytes, so never send it empty
        date = self.edit_ovs5011_date.text().strip() or datetime.now().strftime("%Y%m%d")
        # 다음 조회 때 같은 조건으로 nkey만 바꿔 보내야 하므로 조회 조건을 저장해 둔다
        self._ovs5011_args = [acno, pswd, date, self.edit_ovs5011_code.text().strip()]
        self.table_ovs_gajungsan.setRowCount(0)
        self._send_tr5011("")

    def _on_ovs_gajungsan_next(self):
        self._send_tr5011(self._ovs5011_nkey)

    def _send_tr5011(self, nkey):
        # nkey는 OCX가 18바이트 고정으로 복사하므로 받은 값을 strip 없이 그대로 넘긴다
        self.btn_ovs5011_next.setEnabled(False)
        self.lbl_ovs5011_next.setText("")
        result = self.ocx.dynamicCall(
            "TR5011(int, QString, QString, QString, QString, QString)",
            [TK_TR5011] + self._ovs5011_args + [nkey])
        self._log(f"해외가정산조회(TR5011, 주문일={self._ovs5011_args[2]}, "
                  f"{'다음 nkey=' + repr(nkey) if nkey else '첫 조회'}) => {result}")
        self._check_result(result)

    def _on_hwj_bank_send(self):
        crcy = self.combo_hwj_bank_crcy.currentData()
        self.lbl_hwj_bank.setText("-")
        result = self.ocx.dynamicCall("TR5010A(int, QString)", [TK_TR5010A, crcy])
        self._log(f"중계은행조회(TR5010A, {crcy}) => {result}")
        self._check_result(result)

    def _on_hwj_bankrate_send(self, method, key):
        crcy = self.combo_hwj_bank_crcy.currentData()
        self.lbl_hwj_bankrate.setText("-")
        result = self.ocx.dynamicCall(f"{method}(int, QString)", [key, crcy])
        self._log(f"은행환율조회({method}, {crcy}) => {result}")
        self._check_result(result)

    def _on_hwj_rate_send(self):
        acno = self.combo_ovs_accn.currentData() or self.combo_ovs_accn.currentText().strip()
        pswd = self.edit_ovs_pswd.text()
        if not acno.strip():
            self._log("계좌를 선택하세요")
            return
        gdrate = self.edit_hwj_gdrate.text().strip()
        if not gdrate:
            self._log("기준환율을 입력하세요 (은행환율 조회 결과 참고)")
            return
        bnstp = self.combo_hwj_bnstp.currentData()  # 01:외화매수 02:외화매도
        self.edit_hwj_rate.setText("")
        self.lbl_hwj_prvlg.setText("-")
        result = self.ocx.dynamicCall(
            "TR5005(int, QString, QString, QString, QString)",
            [TK_TR5005, acno, pswd, gdrate, bnstp])
        self._log(f"적용환율조회(TR5005, 기준환율={gdrate}, 구분={bnstp}) => {result}")
        self._check_result(result)

    def _on_hwj_bnstp_changed(self):
        FOREIGN = ["USD", "HKD", "CNY", "AUD"]
        buy = self.combo_hwj_bnstp.currentData() == "01"
        # 매매구분을 바꿔도 고르던 외화는 유지
        fx = next((c.currentData() for c in (self.combo_hwj_crcy_from, self.combo_hwj_crcy_to)
                   if c.currentData() in FOREIGN), "USD")
        fixed, free = ((self.combo_hwj_crcy_from, self.combo_hwj_crcy_to) if buy
                       else (self.combo_hwj_crcy_to, self.combo_hwj_crcy_from))
        fixed.clear()
        fixed.addItem("KRW", "KRW")
        fixed.setEnabled(False)
        free.clear()
        for c in FOREIGN:
            free.addItem(c, c)
        free.setCurrentIndex(FOREIGN.index(fx))
        free.setEnabled(True)

    def _send_tr5006(self, key, trxtp, rt_won="", rt_target=""):
        # TR5006(key, acc, pswd, 대상통화, 환전통화, 적용환율, 우대율, 환전금액(외화), 매매구분, 처리구분, 원화금액, 외화금액)
        # 주의: OCX가 적용환율/우대율을 15자 고정으로 복사하므로 TR5005 응답값을 받은 형식 그대로 넘긴다.
        acno = self.combo_ovs_accn.currentData() or self.combo_ovs_accn.currentText().strip()
        pswd = self.edit_ovs_pswd.text()
        if not acno.strip():
            self._log("계좌를 선택하세요")
            return
        app = self.edit_hwj_rate.text().strip()
        prv = self.lbl_hwj_prvlg.text().strip()
        if not app or prv == "-":
            self._log("적용환율조회(TR5005)를 먼저 하세요")
            return
        crcy_from = self.combo_hwj_crcy_from.currentData()
        crcy_to = self.combo_hwj_crcy_to.currentData()
        amt = self.edit_hwj_fcurramt.text().strip()
        bnstp = self.combo_hwj_bnstp.currentData()  # 01:외화매수 02:외화매도
        if key == TK_TR5006_EST:
            self.edit_hwj_wonamt.setText("")
            self.edit_hwj_fcurr_res.setText("")
        self.lbl_hwj_result.setText("-")
        self.lbl_hwj_trno.setText("-")
        result = self.ocx.dynamicCall(
            "TR5006(int, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString)",
            [key, acno, pswd, crcy_from, crcy_to, app, prv, amt, bnstp, trxtp, rt_won, rt_target])
        label = "환전예상" if trxtp == "0" else "외화환전 실행"
        self._log(f"{label}(TR5006, {crcy_from}->{crcy_to}, 금액={amt}, 구분={bnstp}, trxtp={trxtp}) => {result}")
        self._check_result(result)

    def _on_hwj_exec_send(self):
        rt_won = self.edit_hwj_wonamt.text().strip()
        rt_target = self.edit_hwj_fcurr_res.text().strip()
        if not rt_won or not rt_target:
            self._log("환전예상조회를 먼저 하세요")
            return
        msg = (f"실제 환전이 진행됩니다 (취소 불가).\n\n"
               f"{self.combo_hwj_crcy_from.currentData()} -> {self.combo_hwj_crcy_to.currentData()}\n"
               f"원화 {rt_won} / 외화 {rt_target}\n\n계속할까요?")
        if QMessageBox.question(self, "외화 환전", msg,
                                QMessageBox.Yes | QMessageBox.No, QMessageBox.No) != QMessageBox.Yes:
            self._log("외화환전 취소")
            return
        self._send_tr5006(TK_TR5006, "1", rt_won, rt_target)

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

        if not IS_ADMIN or code != self.edit_sise_code.text().strip().lstrip('A'):
            return  # 현재 조회 중인 종목만 처리(시세조회 화면 갱신은 여기까지, 시세 패널은 내부용에만 있음)

        if "034" in m:  # 체결시간 있으면 체결 데이터
            self.lbl_curr.setText(m.get("023", "-").lstrip('+-') or "-")
            self.lbl_diff.setText(m.get("024", "-"))
            self.lbl_rate.setText(m.get("033", "-"))
            self.lbl_gvol.setText(m.get("027", "-"))
            self.lbl_siga.setText(m.get("029", "-"))
            self.lbl_koga.setText(m.get("030", "-"))
            self.lbl_jega.setText(m.get("031", "-"))
            self.lbl_mgjv.setText(m.get("201", "-"))
        elif "111" in m:  # 동시호가 구간엔 034(체결) 대신 111(예상체결가)만 옴
            self.lbl_curr.setText(m.get("111", "-").lstrip('+-') or "-")

        if "040" in m:  # 호가시간 있으면 호가 데이터
            self.lbl_dvol.setText(m.get("101", "-"))
            self.lbl_svol.setText(m.get("106", "-"))

    def _evt_recv_data(self, key, dptr, size, b_next, nkey):
        self._log(f"[EVT] OnRecvData key={key} dptr={dptr} size={size} bNext={b_next} nkey={nkey}")
        if dptr == 0 or size <= 0:
            return
        try:
            # copy bytes immediately - dptr memory may be freed after this call returns
            addr = dptr & 0xFFFFFFFF
            raw = ctypes.string_at(addr, size)
            def s(b): return b.decode('cp949', errors='ignore').strip()
            if key == TK_TR1001:
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
            elif key == TK_TR1203:
                # 주문번호[6] 원주문번호[6] 에러메시지[80] (nrec[4] 이후)
                jmno = raw[4:10].decode('cp949', errors='ignore').strip()
                ojno = raw[10:16].decode('cp949', errors='ignore').strip()
                emsg = raw[16:96].decode('cp949', errors='ignore').strip()
                label = "주식주문"
                self.lbl_odr_result.setText(f"주문번호:{jmno}")
                self._log(f"  [{label}] 주문번호={jmno} 원주문번호={ojno} 메시지={emsg}")
                if jmno and jmno != "000000":
                    QMessageBox.information(self, f"{label} 접수", f"주문번호: {jmno}\n{emsg}")
                else:
                    QMessageBox.warning(self, f"{label} 실패", emsg or "주문이 거부되었습니다.")
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
            elif key == TK_TR5010A:
                # SDPWQ002 응답: tr5010_mid[6] + out5[5](건수) + tr5010_rec(bnkcode[3] + bnkname[40])
                cnt = s(raw[6:11])
                bank, bnm = s(raw[11:14]), s(raw[14:54])
                self.lbl_hwj_bank.setText(f"{bank} {bnm}")
                self._log(f"  [중계은행] 건수={cnt} 은행코드={bank} 은행명={bnm}")
            elif key == TK_TR5007:
                # SDPKT183 응답: tr5007_mid[29] + Out[5] + tr5007_rec
                r = raw[29 + 5:]
                sell, buy = s(r[17:28]), s(r[28:39])
                self.lbl_hwj_bankrate.setText(f"[기업] 증권사매도율 {sell} / 증권사매입율 {buy}")
                self._log(f"  [기업환율] 시각={s(r[0:17])} 증권사매도율={sell} 증권사매입율={buy}"
                          f" 중계은행={s(r[39:42])} {s(r[42:82])} 응답코드={s(r[82:86])} {s(r[86:186])}"
                          f" IF관리번호={s(r[186:196])}")
            elif key == TK_TR5004:
                # SDPKT169 응답: tr5004_mid[29] + Out[5] + tr5004_rec
                r = raw[29 + 5:]
                sell, buy = s(r[17:28]), s(r[28:39])
                self.lbl_hwj_bankrate.setText(f"[국민] 증권사매도율 {sell} / 증권사매입율 {buy}")
                self._log(f"  [국민환율] 시각={s(r[0:17])} 증권사매도율={sell} 증권사매입율={buy}"
                          f" 기준환율={s(r[39:50])} 매입환율={s(r[50:61])} 매도환율={s(r[61:72])}"
                          f" 중계은행={s(r[72:75])} {s(r[75:115])} 응답코드={s(r[115:119])} {s(r[119:219])}")
            elif key == TK_TR5005:
                # SDPDQ099 응답: tr5005_mid[82] + count[5] + AppXchrat[15] + PrvlgRat[15]
                cnt, app, prv = s(raw[82:87]), s(raw[87:102]), s(raw[102:117])
                self.edit_hwj_rate.setText(app)
                self.lbl_hwj_prvlg.setText(prv)
                self.lbl_hwj_ratetime.setText(f"(최종조회시간: {datetime.now().strftime('%H:%M:%S')})")
                self._log(f"  [적용환율] 건수={cnt} 적용환율={app} 우대율={prv}")
            elif key in (TK_TR5006_EST, TK_TR5006):
                # SDPDT080 응답: tr5006_mid[263] + Out[5] + tr5006_rec
                r = raw[263 + 5:]
                trno, won, fcur, app = s(r[0:10]), s(r[76:92]), s(r[92:109]), s(r[109:124])
                wcbal, wfbal, ffbal, fcbal = s(r[10:26]), s(r[26:42]), s(r[42:59]), s(r[59:76])
                label = "환전예상" if key == TK_TR5006_EST else "외화환전"
                if key == TK_TR5006_EST:
                    # 예상금액은 rec가 아니라 mid 에코의 WonAmt[16]/FcurrAmt[17]에 온다.
                    # 이 두 값이 실제 환전(trxtp=1) 때 rtWon/rtTarget으로 그대로 돌아간다.
                    m_won, m_fcur = s(raw[181:197]), s(raw[197:214])
                    self.edit_hwj_wonamt.setText(m_won)
                    self.edit_hwj_fcurr_res.setText(m_fcur)
                    self.lbl_hwj_result.setText(f"[{label}] 원화 {m_won} / 외화 {m_fcur}")
                else:
                    self.lbl_hwj_result.setText(f"[{label}] 원화 {won} / 외화 {fcur} @ {app}")
                self.lbl_hwj_trno.setText(trno or "-")
                self._log(f"  [{label}] 거래번호={trno} 원화금액={won} 외화금액={fcur} 적용환율={app}"
                          f" 원화잔고={wcbal} 원화환전={wfbal} 외화환전={ffbal} 외화잔고={fcbal}")
                # 응답 앞부분(tr5006_mid 에코)도 확인용으로 남긴다 - 결과가 0으로 올 때 원인 파악용
                self._log(f"    [mid] 처리구분={s(raw[16:17])} 환전금액={s(raw[163:181])}"
                          f" 원화금액={s(raw[181:197])} 외화금액={s(raw[197:214])}"
                          f" 응답코드={s(raw[214:217])} 대고객환율={s(raw[217:232])} 거래일={s(raw[255:263])}"
                          f" 건수={s(raw[263:268])} size={size}")
            elif key == TK_TR1211:  # 주식 체결/미체결 (header: acno[11]+nrec[4]=15, grid=265)
                GRID = 265
                mode = getattr(self, '_chegyul_mode', '체결')
                nrec = int(raw[11:15].decode('cp949', errors='ignore').strip() or "0")
                self._log(f"  [주식{mode}] {nrec}건")
                for i in range(nrec):
                    g = raw[15 + i*GRID: 15 + (i+1)*GRID]
                    if len(g) < GRID: break
                    mkgb = s(g[20:21])
                    jong = f"[{MKGB_NAMES.get(mkgb, mkgb)}] {s(g[21:33])} {s(g[33:73])}".strip()
                    msg = s(g[237:257]) or s(g[257:265])
                    self._add_chegyul_row(
                        mode, s(g[0:10]), s(g[10:20]), jong,
                        s(g[133:145]), s(g[145:157]), s(g[157:169]), s(g[169:181]),
                        s(g[193:205]), msg)
                if b_next:
                    acno = getattr(self, '_jngo_acno', '')
                    pswd = getattr(self, '_jngo_pswd', '')
                    dlgb = 2 if mode == "미체결" else 0
                    self.ocx.dynamicCall(
                        "TR1211(int, QString, QString, QString, int, int, int, int, QString)",
                        [TK_TR1211, acno, pswd, "", 0, 0, dlgb, 1, nkey])
            elif key == TK_TR5002:  # 해외 체결/미체결 (COSAQ005, mid[93]+count[5]+rec[1030]*n)
                MID_LEN = 93
                REC = 1030
                nrec = int(raw[MID_LEN:MID_LEN+5].decode('cp949', errors='ignore').strip() or "0")
                self._log(f"  [해외체결내역] {nrec}건")
                for i in range(nrec):
                    g = raw[MID_LEN+5 + i*REC: MID_LEN+5 + (i+1)*REC]
                    if len(g) < REC: break
                    bnstp = s(g[400:401])
                    jong = f"{s(g[109:121])} {s(g[10:50])}".strip()
                    self._add_ovs_chegyul_row(
                        s(g[0:10]), s(g[50:60]), jong, OVS_BNSTP_NAMES.get(bnstp, bnstp),
                        s(g[283:299]), s(g[322:338]), s(g[461:477]),
                        s(g[440:461]), s(g[500:521]), s(g[562:571]), s(g[571:580]),
                        s(g[920:1020]))
                if b_next:
                    self._log(f"    (다음 페이지 있음: nkey={nkey} - 현재 OCX가 이어보기 커서를 안 받아서 자동 연속조회는 아직 안 됨)")
            elif key == TK_TR5000:  # 해외증권 예수금 (COSAQ027, mid[37] + count1+out1[275]*count1 + count2+out2[216]*count2 + count3+out3[64] + ...)
                MID_LEN = 37
                pos = MID_LEN
                # count1(요청 통화 상세 건수) - 0이면 out1[275] 블록 자체가 통째로 생략되고 온다
                count1 = int(raw[pos:pos+5].decode('cp949', errors='ignore').strip() or "0")
                pos += 5 + count1 * 275  # out1은 아직 화면에 안 씀 - count1건만큼 건너뛰기만 함
                count2 = int(raw[pos:pos+5].decode('cp949', errors='ignore').strip() or "0")
                pos += 5
                self.table_ovs_yesugeum.setRowCount(0)
                for i in range(count2):
                    o = raw[pos + i*216: pos + (i+1)*216]
                    if len(o) < 216: break
                    self._add_ovs_yesugeum_row(
                        s(o[40:43]), s(o[60:77]), s(o[77:94]), s(o[111:128]),
                        s(o[128:145]), s(o[162:179]), s(o[179:199]), s(o[199:216]))
                pos += count2 * 216
                pos += 5  # count3
                o3 = raw[pos:pos+64]
                self.lbl_ovs_wondps.setText(s(o3[0:16]))
                self.lbl_ovs_wonout.setText(s(o3[16:32]))
                self.lbl_ovs_wonconv.setText(s(o3[32:48]))
                self.lbl_ovs_bnsmgn.setText(s(o3[48:64]))
                self._log(f"  [해외예수금] 통화 {count2}건")
            elif key == TK_TR5001:  # 해외주식 잔고 (COSAQ003, mid[57]+count[5]+rec[469]*n)
                MID_LEN = 57
                REC = 469
                nrec = int(raw[MID_LEN:MID_LEN+5].decode('cp949', errors='ignore').strip() or "0")
                self.table_ovs_jango.setRowCount(0)
                for i in range(nrec):
                    g = raw[MID_LEN+5 + i*REC: MID_LEN+5 + (i+1)*REC]
                    if len(g) < REC: break
                    self._add_ovs_jango_row(
                        s(g[100:112]), s(g[0:100]), s(g[137:177]), s(g[121:137]),
                        s(g[209:225]), s(g[289:304]), s(g[319:336]), s(g[336:353]), s(g[304:316]))
                self._log(f"  [해외잔고] {nrec}건")
            elif key == TK_TR2002:  # 주식 거래내역 (mid[63]+out1[614]+count[5]+rec[440]*n)
                MID_LEN = 63
                OUT1_LEN = 614
                pos = MID_LEN + OUT1_LEN
                nrec = int(raw[pos:pos+5].decode('cp949', errors='ignore').strip() or "0")
                pos += 5
                REC = 440
                for i in range(nrec):
                    g = raw[pos + i*REC: pos + (i+1)*REC]
                    if len(g) < REC: break
                    self._add_odr_gorae_row(
                        s(g[0:8]), s(g[8:48]), s(g[158:198]), s(g[52:68]),
                        s(g[198:211]), s(g[68:84]), s(g[84:100]), s(g[227:243]), s(g[431:440]))
                self._log(f"  [주식거래내역] {nrec}건")
            elif key == TK_TR5003:  # 해외주식 거래내역 (COSAQ208, mid[75]+count[5]+AcntNm[40]+out[5]+rec[981]*n)
                MID_LEN = 75
                pos = MID_LEN + 5 + 40 + 5  # count(5) + AcntNm(40) + out(5)
                # 레코드 건수는 앞의 count(5)가 아니라 rec 바로 앞의 out(5) 필드다
                nrec = int(raw[pos-5:pos].decode('cp949', errors='ignore').strip() or "0")
                REC = 981
                # tr5003_rec 오프셋 (Open_API_OUT.h): SmryNm 338:378, Position 217:239, Uprc 683:698,
                # MktNm 805:845, TrxTime 878:887
                for i in range(nrec):
                    g = raw[pos + i*REC: pos + (i+1)*REC]
                    if len(g) < REC: break
                    self._add_ovs_gorae_row(
                        s(g[0:8]), s(g[338:378]), s(g[59:99]), s(g[99:102]),
                        s(g[217:239]), s(g[683:698]), s(g[102:119]), s(g[119:135]),
                        s(g[805:845]), s(g[878:887]))
                self._ovs5003_nkey = nkey if b_next else ""
                self.btn_ovs5003_next.setEnabled(bool(b_next))
                self.lbl_ovs5003_next.setText("다음 데이터 있음" if b_next else "마지막")
                self._log(f"  [해외거래내역] {nrec}건 (누적 {self.table_ovs_gorae.rowCount()}건)"
                          f" bNext={b_next} nkey={nkey!r}")
            elif key == TK_TR5011:  # 해외주식 가정산 (COSCQ002, mid[68]+out[5]+rec[489]*n)
                pos = 68 + 5
                nrec = int(raw[pos-5:pos].decode('cp949', errors='ignore').strip() or "0")
                REC = 489
                # tr5011_mod offsets (Open_API_OUT.h): bnstpnm 20:30, IsuNo 30:42, Hname 54:154,
                # ExecQty 154:170, avrPrice 210:228, exepr 228:245, cms 245:262, adjs 262:279,
                # Mktnm 353:393, crcy 393:396
                for i in range(nrec):
                    g = raw[pos + i*REC: pos + (i+1)*REC]
                    if len(g) < REC: break
                    self._add_ovs_gajungsan_row([
                        s(g[20:30]), s(g[30:42]), s(g[54:154]), s(g[154:170]), s(g[210:228]),
                        s(g[228:245]), s(g[245:262]), s(g[262:279]), s(g[393:396]), s(g[353:393])])
                self._ovs5011_nkey = nkey if b_next else ""
                self.btn_ovs5011_next.setEnabled(bool(b_next))
                self.lbl_ovs5011_next.setText("다음 데이터 있음" if b_next else "마지막")
                self._log(f"  [해외가정산] {nrec}건 (누적 {self.table_ovs_gajungsan.rowCount()}건)"
                          f" bNext={b_next} nkey={nkey!r} size={len(raw)}")
            elif key == TK_TR8001:  # 고객정보 (pihocust, mod: gubn[1])
                gubn = raw[0:1].decode('cp949', errors='ignore').strip()
                self._log(f"  [고객정보] gubn={gubn!r} raw={raw[:32].hex(' ')}")
            elif key == TK_TR1002:  # 주식 시세(NXT/통합) - 응답이 고정바이너리 아닌 평문 텍스트
                text = raw.decode('cp949', errors='ignore')
                self._log(f"  [TR1002 시세] raw={text!r}")
                # 맨 앞 토큰 = 1021(종목코드) 필드값. 실시간 코드와 동일 형식(M.A005930 등)이라
                # OnRealData 매칭용 키로 저장해두고, 나머지 토큰만 요청 필드 순서대로 매핑
                tokens = [v for v in text.replace('\n', '\t').split('\t') if v != '']
                realtime_key, vals = (tokens[0], tokens[1:]) if tokens else ("", [])
                self._sise_realtime_key = realtime_key
                pairs = ", ".join(f"{lbl}={v}" for lbl, v in zip(TR1002_SISE_LABELS, vals))
                self._log(f"  [TR1002 파싱시도] code={realtime_key} {pairs}")
                # 현재가,등락율,시가,고가,저가,전일대비,거래량,매도잔량,매수잔량 순
                widgets = [self.lbl_curr, self.lbl_rate, self.lbl_siga, self.lbl_koga,
                           self.lbl_jega, self.lbl_diff, self.lbl_gvol, self.lbl_dvol, self.lbl_svol]
                for w, v in zip(widgets, vals):
                    w.setText(v)
            else:
                self._log(f"  [RecvData] key={key} (미처리)")
        except Exception as e:
            self._log(f"  [RecvData 파싱 오류] {e}")


app = QApplication(sys.argv)
window = TestWindow()
window.show()
sys.exit(app.exec_())
