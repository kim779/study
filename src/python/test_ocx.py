import sys
import os
import shutil
import subprocess
import ctypes
import winreg
import time
from PyQt5.QAxContainer import QAxWidget
from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QGroupBox, QPushButton, QLabel, QLineEdit, QTextEdit, QFormLayout,
    QComboBox, QGridLayout, QMessageBox, QTableWidget, QTableWidgetItem,
    QHeaderView, QAbstractItemView
)
from datetime import datetime

OCX_GUID = "{CDADD338-C7AB-4977-B65D-8E988B5958E3}"

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
TK_TR1201 = 4   # 주식 주문
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
TK_GETCODE = 32 # 종목코드 목록조회 (GetCode)
TK_GREEKS1 = 150 # 옵션 그릭스(민감도) 조회용 키1
TK_GREEKS2 = 151 # 옵션 그릭스(민감도) 조회용 키2

# TR1002(NXT/통합 시세조회) 요청 시 1777(장운영구분) 뒤에 붙이는 요청 필드번호 목록
# 순서 = 응답에서 값이 오는 순서와 동일하다고 가정 (실측 후 확정 필요)
TR1002_SISE_FIELDS = ["2023", "2033", "2029", "2030", "2031", "2024", "2027", "2041", "2061"]
TR1002_SISE_LABELS = ["현재가", "등락율", "시가", "고가", "저가", "전일대비", "거래량", "매도잔량", "매수잔량"]

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
        self.setWindowTitle("IBKConnector OCX Test")
        self.setMinimumWidth(500)
        self.resize(650, 750)

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
        self.setCentralWidget(central)
        layout = QVBoxLayout(central)
        layout.setSpacing(8)

        layout.addWidget(self._build_init_group())

        # Login | 시세조회 좌우 분할
        mid = QWidget()
        mid_h = QHBoxLayout(mid)
        mid_h.setContentsMargins(0, 0, 0, 0)
        mid_h.addWidget(self._build_login_group())
        mid_h.addWidget(self._build_sise_group())
        layout.addWidget(mid)

        layout.addWidget(self._build_order_group())
        layout.addWidget(self._build_jango_group())
        layout.addWidget(self._build_log_group())

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

        form = QFormLayout()
        self.edit_user_id  = QLineEdit("ng12589")
        self.edit_user_pw  = QLineEdit("wnsgur12@"); self.edit_user_pw.setEchoMode(QLineEdit.Password)
        self.edit_cert_pw  = QLineEdit("ahffkdy123 "); self.edit_cert_pw.setEchoMode(QLineEdit.Password)
        self.combo_server  = QComboBox()
        for ip, name in SERVER_LIST:
            self.combo_server.addItem(f"{name} ({ip})", ip)
        self.combo_server.currentIndexChanged.connect(self._on_server_selected)
        self.edit_svr_ip   = QLineEdit(SERVER_LIST[0][0])
        self.edit_svr_port = QLineEdit("15201")
        form.addRow("user_id",   self.edit_user_id)
        form.addRow("user_pw",   self.edit_user_pw)
        form.addRow("cert_pw",   self.edit_cert_pw)
        form.addRow("서버선택",  self.combo_server)
        form.addRow("svr_ip",    self.edit_svr_ip)
        form.addRow("svr_port",  self.edit_svr_port)
        layout.addLayout(form)

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
        left.addRow("종목코드", self.edit_odr_code)

        self.combo_mmgb = QComboBox()
        for text, val in [("1 매도", 1), ("2 매수", 2), ("3 정정", 3), ("4 취소", 4)]:
            self.combo_mmgb.addItem(text, val)
        left.addRow("매매구분", self.combo_mmgb)

        self.combo_hogb = QComboBox()
        for text, val in [("00 지정가", 0), ("03 시장가", 3), ("05 조건부", 5),
                          ("06 최유리", 6), ("10 지정가(IOC)", 10), ("20 지정가(FOK)", 20)]:
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

        btn_h = QHBoxLayout()
        btn = QPushButton("주문실행")
        btn.clicked.connect(self._on_odr_send)
        self.lbl_odr_result = QLabel("-")
        btn_h.addWidget(btn)
        btn_h.addWidget(self.lbl_odr_result)
        btn_h.addStretch()
        right.addRow(btn_h)

        btn_h2 = QHBoxLayout()
        btn_mc = QPushButton("잔고조회")
        btn_mc.clicked.connect(self._on_michegyul_send)
        btn_cg = QPushButton("체결조회")
        btn_cg.clicked.connect(self._on_chegyul_send)
        btn_uc = QPushButton("미체결조회")
        btn_uc.clicked.connect(self._on_michegyul_odr_send)
        btn_h2.addWidget(btn_mc)
        btn_h2.addWidget(btn_cg)
        btn_h2.addWidget(btn_uc)
        btn_h2.addStretch()
        right.addRow(btn_h2)

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

        cols = ["종목코드", "잔고수량", "현재가", "평가금액", "수익률(%)"]
        self.table_jango = QTableWidget(0, len(cols))
        self.table_jango.setHorizontalHeaderLabels(cols)
        self.table_jango.verticalHeader().setVisible(False)
        self.table_jango.verticalHeader().setDefaultSectionSize(18)
        self.table_jango.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.table_jango.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.table_jango.horizontalHeader().setStretchLastSection(True)
        self.table_jango.horizontalHeader().setSectionResizeMode(0, QHeaderView.Stretch)
        self.table_jango.setFixedHeight(140)  # 헤더 + 6줄 고정
        v.addWidget(self.table_jango)

        return group

    def _add_jango_row(self, code, remain, maip, curr, eval_amt, rate):
        row = self.table_jango.rowCount()
        self.table_jango.insertRow(row)
        for col, val in enumerate([code, remain, curr, eval_amt, rate]):
            self.table_jango.setItem(row, col, QTableWidgetItem(val))
        code_key = code.lstrip('A')
        self._jango_row_by_code[code_key] = row
        # 실시간 재계산용 기준값 저장 (서버가 계산한 초기 평가금액을 기준점으로 삼아
        # 현재가 변동분만큼만 보정 -> 수수료/세금을 몰라도 근사치가 아닌 정확한 값 유지)
        try:
            self._jango_calc[code_key] = {
                'remain': float(remain), 'maip': float(maip),
                'base_curr': float(curr), 'base_eval': float(eval_amt),
            }
        except ValueError:
            pass

    def _update_jango_row(self, code, curr=None, eval_amt=None, rate=None):
        row = self._jango_row_by_code.get(code.lstrip('A'))
        if row is None:
            return
        for col, val in [(2, curr), (3, eval_amt), (4, rate)]:
            if val is not None:
                self.table_jango.setItem(row, col, QTableWidgetItem(val))

    def _recalc_jango_on_price(self, code, new_curr_s):
        # OnRealData로 들어온 현재가로 잔고 종목의 평가금액/수익률을 실시간 재계산
        code_key = code.lstrip('A')
        calc = self._jango_calc.get(code_key)
        if calc is None:
            return
        try:
            new_curr = float(new_curr_s)
        except ValueError:
            return
        remain = calc['remain']
        maip = calc['maip']
        new_eval = calc['base_eval'] + (new_curr - calc['base_curr']) * remain
        rate = (new_eval - maip) * 100 / maip if maip else 0.0
        self._update_jango_row(code_key, curr=f"{new_curr:.0f}", eval_amt=f"{new_eval:.0f}", rate=f"{rate:.2f}")

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

    def _on_initialize(self):
        result = self.ocx.dynamicCall("Initialize()")
        self.lbl_init.setText(str(result))
        self._log(f"Initialize() => {result}")
        self._log(f"OCX path: {self._get_ocx_path()}")

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
        svr_ip   = self.edit_svr_ip.text()
        svr_port = int(self.edit_svr_port.text() or "0")

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
            self._log(f"  오류: {err}")

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

    def _evt_sbalance(self, data):
        # 주식 실시간 잔고 (TR1222 등록 후 수신). 필드 순서 추정치이며 실측 후 조정 필요:
        # 0.종목코드 1.종목명 2.결제잔고 3.주문잔량 4.매입평균가 5.매도가능잔고 6.현재가 7.평가금액 8.평가손익 9.평가수익률 10.매입금액 ...
        self._log(f"[EVT] OnSBalance raw={data!r}")
        f = data.split('\t')
        if len(f) < 10:
            return
        code = f[0].lstrip('A')
        self._update_jango_row(code, remain=f[2], avg_price=f[4], curr_price=f[6], rate=f[9])

    def _evt_fbalance(self, data):
        # 선물옵션 실시간 잔고 (TR3222 등록 후 수신). 필드 순서 추정치이며 실측 후 조정 필요:
        # 0.종목코드 1.종목명 2.구분 3.잔고수량 4.가능수량 5.평균가 6.현재가 7.평균가대비 8.평가금액 9.평가손익 10.수익률 11.매입금액 12.전일대비
        self._log(f"[EVT] OnFBalance raw={data!r}")
        f = data.split('\t')
        if len(f) < 11:
            return
        code = f[0].lstrip('A')
        self._update_jango_row(code, remain=f[3], avg_price=f[5], curr_price=f[6], rate=f[10])

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
            self._log(f"  오류: {self.ocx.dynamicCall('GetLastErrMsg()')}")

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
            result = self.ocx.dynamicCall(
                "TR1201(int, int, QString, QString, int, QString, int, int, int, int)",
                [TK_TR1201, mmgb, acno, pswd, ojno, code, jqty, jprc, hogb, 0])

        self._log(f"주문({code} {self.combo_mmgb.currentText()} {jqty}@{jprc}) => {result}")
        if not result:
            err = self.ocx.dynamicCall("GetLastErrMsg()")
            self._log(f"  오류: {err}")

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
                self._log(f"  오류: {self.ocx.dynamicCall('GetLastErrMsg()')}")
        else:  # 주식
            result = self.ocx.dynamicCall(
                "TR1221(int, QString, QString, int, QString)",
                [TK_TR1221, acno, pswd, 0, ""])
            r2 = self.ocx.dynamicCall("TR1222(int, QString, QString, int)", [TK_TR1222, acno, pswd, 0])
            self._log(f"TR1222(실시간잔고등록) => {r2}")
            if not r2:
                self._log(f"  오류: {self.ocx.dynamicCall('GetLastErrMsg()')}")
        self._log(f"잔고조회 => {result}")
        if not result:
            self._log(f"  오류: {self.ocx.dynamicCall('GetLastErrMsg()')}")

    def _on_chegyul_send(self):
        acno = self.combo_odr_accn.currentData() or ""
        pswd = self.edit_odr_pswd.text()
        if not acno.strip():
            self._log("계좌를 선택하세요")
            return
        self._jngo_acno = acno
        self._jngo_pswd = pswd
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
            self._log(f"  오류: {self.ocx.dynamicCall('GetLastErrMsg()')}")

    def _on_michegyul_odr_send(self):
        acno = self.combo_odr_accn.currentData() or ""
        pswd = self.edit_odr_pswd.text()
        if not acno.strip():
            self._log("계좌를 선택하세요")
            return
        self._jngo_acno = acno
        self._jngo_pswd = pswd
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
            self._log(f"  오류: {self.ocx.dynamicCall('GetLastErrMsg()')}")

    def _evt_real_data(self, data):
        # format: 종목코드\t필드번호\t값\t필드번호\t값\t...
        parts = data.split('\t')
        if len(parts) < 3:
            return

        # 주식은 실시간 코드에 'A' prefix 붙어 옴, 선물은 에디트/실시간 둘 다 'A' prefix
        # 양쪽 모두 lstrip('A') 후 비교
        code = parts[0].lstrip('A')

        # key=value 딕셔너리로 변환
        m = {}
        it = iter(parts[1:])
        for k in it:
            v = next(it, "")
            m[k] = v

        if code in self._jango_calc and "023" in m:
            self._recalc_jango_on_price(code, m["023"])

        if code != self.edit_sise_code.text().strip().lstrip('A'):
            return  # 현재 조회 중인 종목만 처리(시세조회 화면 갱신은 여기까지)

        if "034" in m:  # 체결시간 있으면 체결 데이터
            self.lbl_curr.setText(m.get("023", "-"))
            self.lbl_diff.setText(m.get("024", "-"))
            self.lbl_rate.setText(m.get("033", "-"))
            self.lbl_gvol.setText(m.get("027", "-"))
            self.lbl_siga.setText(m.get("029", "-"))
            self.lbl_koga.setText(m.get("030", "-"))
            self.lbl_jega.setText(m.get("031", "-"))
            self.lbl_mgjv.setText(m.get("201", "-"))

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
            elif key in (TK_TR3201, TK_TR1201):
                # 주문번호[6] 원주문번호[6] 에러메시지[80] (nrec[4] 이후)
                jmno = raw[4:10].decode('cp949', errors='ignore').strip()
                ojno = raw[10:16].decode('cp949', errors='ignore').strip()
                emsg = raw[16:96].decode('cp949', errors='ignore').strip()
                label = "선물주문" if key == TK_TR3201 else "주식주문"
                self.lbl_odr_result.setText(f"주문번호:{jmno}")
                self._log(f"  [{label}] 주문번호={jmno} 원주문번호={ojno} 메시지={emsg}")
            elif key == TK_TR3221:  # 선물 잔고 (header: acno[11]+nrec[4]=15, grid=149)
                GRID = 149
                nrec = int(raw[11:15].decode('cp949', errors='ignore').strip() or "0")
                self._log(f"  [선물잔고] {nrec}건")
                for i in range(nrec):
                    g = raw[15 + i*GRID: 15 + (i+1)*GRID]
                    if len(g) < GRID: break
                    self._log(f"    {s(g[0:8])} {s(g[38:44])} 잔고={s(g[44:54])} 평단={s(g[64:74])} 현재={s(g[74:84])} 평가금액={s(g[94:109])} 수익률={s(g[124:134])}% 매입금액={s(g[134:149])}")
                    self._add_jango_row(s(g[0:8]), s(g[44:54]), s(g[134:149]), s(g[74:84]), s(g[94:109]), s(g[124:134]))
                if b_next:
                    acno = getattr(self, '_jngo_acno', '')
                    pswd = getattr(self, '_jngo_pswd', '')
                    self.ocx.dynamicCall("TR3221(int, QString, QString, QString)",
                                        [TK_TR3221, acno, pswd, nkey])
            elif key == TK_TR1221:  # 주식 잔고 (header: acno[11]+nrec[4]=15, grid=194)
                GRID = 194
                nrec = int(raw[11:15].decode('cp949', errors='ignore').strip() or "0")
                self._log(f"  [주식잔고] {nrec}건")
                for i in range(nrec):
                    g = raw[15 + i*GRID: 15 + (i+1)*GRID]
                    if len(g) < GRID: break
                    self._log(f"    {s(g[0:12])} 잔고={s(g[54:64])} 평단={s(g[74:84])} 현재={s(g[99:109])} 평가금액={s(g[129:144])} 수익률={s(g[159:169])}% 매입금액={s(g[84:99])}")
                    self._add_jango_row(s(g[0:12]), s(g[54:64]), s(g[84:99]), s(g[99:109]), s(g[129:144]), s(g[159:169]))
                if b_next:
                    acno = getattr(self, '_jngo_acno', '')
                    pswd = getattr(self, '_jngo_pswd', '')
                    self.ocx.dynamicCall("TR1221(int, QString, QString, int, QString)",
                                        [TK_TR1221, acno, pswd, 0, nkey])
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
            elif key == TK_TR1211:  # 주식 체결/미체결 (header: acno[11]+nrec[4]=15, grid=222)
                GRID = 222
                mode = getattr(self, '_chegyul_mode', '체결')
                nrec = int(raw[11:15].decode('cp949', errors='ignore').strip() or "0")
                self._log(f"  [주식{mode}] {nrec}건")
                for i in range(nrec):
                    g = raw[15 + i*GRID: 15 + (i+1)*GRID]
                    if len(g) < GRID: break
                    self._log(f"    주문={s(g[0:5])} 종목={s(g[21:27])} {s(g[27:62])} {s(g[62:82])} 주문가={s(g[122:134])} 주문량={s(g[134:146])} 체결가={s(g[146:158])} 체결량={s(g[158:170])} 미체결={s(g[182:194])} [{s(g[194:214])}] {s(g[214:222])}")
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
                # 요청 필드 순서(TR1002_SISE_FIELDS)와 응답 값이 같은 순서로 온다고 가정
                vals = [v for v in text.replace('\n', '\t').split('\t') if v != '']
                pairs = ", ".join(f"{lbl}={v}" for lbl, v in zip(TR1002_SISE_LABELS, vals))
                self._log(f"  [TR1002 파싱시도] {pairs}")
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
