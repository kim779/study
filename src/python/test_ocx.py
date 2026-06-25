import sys
import ctypes
import winreg
from PyQt5.QAxContainer import QAxWidget
from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QGroupBox, QPushButton, QLabel, QLineEdit, QTextEdit, QFormLayout,
    QComboBox, QGridLayout
)
from datetime import datetime

OCX_GUID = "{CDADD338-C7AB-4977-B65D-8E988B5958E3}"

# TR key constants - must match Common.h exactly
TK_TR1001 = 1
TK_TR1002 = 2
TK_TR1003 = 3
TK_TR1201 = 4
TK_TR1211 = 7
TK_TR1221 = 8
TK_TR1231 = 9
TK_TR1801 = 10
TK_TR1802 = 11
TK_TR1803 = 12
TK_TR1804 = 13
TK_TR3001 = 14
TK_TR3002 = 15
TK_TR3201 = 17
TK_TR3211 = 19
TK_TR3221 = 20
TK_TR3222 = 46
TK_TR1222 = 45
TK_TR3232 = 47
TK_TR3411 = 48
TK_GETCODE = 32
TK_GREEKS1 = 150
TK_GREEKS2 = 151

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

class TestWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("IBKConnector OCX Test")
        self.setMinimumWidth(500)

        self.ocx = QAxWidget(OCX_GUID, self)
        self.ocx.setVisible(False)

        # OCX event connections
        self.ocx.OnLogin.connect(self._evt_login)
        self.ocx.OnError.connect(self._evt_error)
        self.ocx.OnClose.connect(self._evt_close)
        self.ocx.OnAlert.connect(self._evt_alert)
        self.ocx.OnGuideMsg.connect(self._evt_guide_msg)
        self.ocx.OnRecvData.connect(self._evt_recv_data)
        self.ocx.OnRealData.connect(self._evt_real_data)

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
        self.edit_user_id  = QLineEdit("khs779")
        self.edit_user_pw  = QLineEdit(); self.edit_user_pw.setEchoMode(QLineEdit.Password)
        self.edit_cert_pw  = QLineEdit(); self.edit_cert_pw.setEchoMode(QLineEdit.Password)
        self.edit_svr_ip   = QLineEdit("211.255.204.75")
        self.edit_svr_port = QLineEdit("15201")
        form.addRow("user_id",   self.edit_user_id)
        form.addRow("user_pw",   self.edit_user_pw)
        form.addRow("cert_pw",   self.edit_cert_pw)
        form.addRow("svr_ip",    self.edit_svr_ip)
        form.addRow("svr_port",  self.edit_svr_port)
        layout.addLayout(form)

        h = QHBoxLayout()
        btn_login  = QPushButton("Login()")
        btn_logout = QPushButton("Logout()")
        btn_login.clicked.connect(self._on_login)
        btn_logout.clicked.connect(self._on_logout)
        self.lbl_login = QLabel("-")
        h.addWidget(btn_login)
        h.addWidget(btn_logout)
        h.addWidget(self.lbl_login)
        h.addStretch()
        layout.addLayout(h)

        # account combo - populated after login
        h2 = QHBoxLayout()
        h2.addWidget(QLabel("계좌"))
        self.combo_accn = QComboBox()
        self.combo_accn.setMinimumWidth(200)
        h2.addWidget(self.combo_accn)
        h2.addStretch()
        layout.addLayout(h2)

        return group

    def _build_sise_group(self):
        group = QGroupBox("시세조회")
        v = QVBoxLayout(group)

        # 종목코드 입력 + 조회 버튼
        h = QHBoxLayout()
        h.addWidget(QLabel("종목코드"))
        self.edit_sise_code = QLineEdit()
        self.edit_sise_code.setMaximumWidth(100)
        h.addWidget(self.edit_sise_code)
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
        self.combo_odr_accn.setMinimumWidth(160)
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

        btn_h = QHBoxLayout()
        btn = QPushButton("주문실행")
        btn.clicked.connect(self._on_odr_send)
        self.lbl_odr_result = QLabel("-")
        btn_h.addWidget(btn)
        btn_h.addWidget(self.lbl_odr_result)
        btn_h.addStretch()
        right.addRow(btn_h)

        btn_h2 = QHBoxLayout()
        btn_mc = QPushButton("미체결조회")
        btn_mc.clicked.connect(self._on_michegyul_send)
        btn_cg = QPushButton("체결조회")
        btn_cg.clicked.connect(self._on_chegyul_send)
        btn_h2.addWidget(btn_mc)
        btn_h2.addWidget(btn_cg)
        btn_h2.addStretch()
        right.addRow(btn_h2)

        h_main.addLayout(left)
        h_main.addSpacing(20)
        h_main.addLayout(right)
        h_main.addStretch()
        return group

    def _build_log_group(self):
        group = QGroupBox("Log")
        v = QVBoxLayout(group)
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

    def _on_sise_send(self):
        code = self.edit_sise_code.text().strip()
        if self.combo_jtype.currentIndex() == 0:  # 선물옵션
            if len(code) != 8:
                self._log("선물옵션 종목코드는 8자리입니다.")
                return
            result = self.ocx.dynamicCall("TR3001(int, QString)", TK_TR3001, code)
        else:  # 주식
            if len(code) != 6:
                self._log("주식 종목코드는 6자리입니다.")
                return
            result = self.ocx.dynamicCall("TR1001(int, QString)", TK_TR1001, code)
        self._log(f"시세조회 요청({code}) => {result}")
        if not result:
            err = self.ocx.dynamicCall("GetLastErrMsg()")
            self._log(f"  오류: {err}")

    # ── OCX event handlers ───────────────────────────────────────

    def _evt_login(self, b_login, msg):
        status = "OK" if b_login else "FAIL"
        self.lbl_login.setText(status)
        self._log(f"[EVT] OnLogin({status}) {msg}")

        if b_login:
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
            self._log(f"  계좌={acno}({len(acno)}자리) 명={acnm} 4~5번째={acno[3:5] if len(acno)>=5 else '?'}")
            self.combo_accn.addItem(line)
            self.combo_odr_accn.addItem(line)

    def _evt_error(self, msg):
        self._log(f"[EVT] OnError: {msg}")

    def _evt_close(self):
        self.lbl_login.setText("-")
        self._log("[EVT] OnClose: connection closed")

    def _evt_alert(self, code, msg):
        self._log(f"[EVT] OnAlert [{code:04d}] {msg}")

    def _evt_guide_msg(self, key, msg):
        self._log(f"[EVT] OnGuideMsg [{key}] {msg}")

    def _on_odr_send(self):
        acno  = self.combo_odr_accn.currentText().split('\t')[0][:11]
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
        acno = self.combo_odr_accn.currentText().split('\t')[0][:11]
        pswd = self.edit_odr_pswd.text()
        if not acno.strip():
            self._log("계좌를 선택하세요")
            return
        self._jngo_acno = acno
        self._jngo_pswd = pswd
        if self.combo_jtype.currentIndex() == 0:  # 선물옵션
            result = self.ocx.dynamicCall(
                "TR3221(int, QString, QString, QString)",
                [TK_TR3221, acno, pswd, ""])
        else:  # 주식
            result = self.ocx.dynamicCall(
                "TR1221(int, QString, QString, int, QString)",
                [TK_TR1221, acno, pswd, 0, ""])
        self._log(f"미체결조회 => {result}")
        if not result:
            self._log(f"  오류: {self.ocx.dynamicCall('GetLastErrMsg()')}")

    def _on_chegyul_send(self):
        acno = self.combo_odr_accn.currentText().split('\t')[0][:11]
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
        self._log(f"체결조회 => {result}")
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
        if code != self.edit_sise_code.text().strip().lstrip('A'):
            return  # 현재 조회 중인 종목만 처리

        # key=value 딕셔너리로 변환
        m = {}
        it = iter(parts[1:])
        for k in it:
            v = next(it, "")
            m[k] = v

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
            elif key == TK_TR3221:  # 선물 미체결 (header: acno[11]+nrec[4]=15, grid=149)
                GRID = 149
                nrec = int(raw[11:15].decode('cp949', errors='ignore').strip() or "0")
                self._log(f"  [선물미체결] {nrec}건")
                for i in range(nrec):
                    g = raw[15 + i*GRID: 15 + (i+1)*GRID]
                    if len(g) < GRID: break
                    self._log(f"    {s(g[0:8])} {s(g[38:44])} 잔고={s(g[44:54])} 평단={s(g[64:74])} 현재={s(g[74:84])} 수익률={s(g[124:134])}%")
                if b_next:
                    acno = getattr(self, '_jngo_acno', '')
                    pswd = getattr(self, '_jngo_pswd', '')
                    self.ocx.dynamicCall("TR3221(int, QString, QString, QString)",
                                        [TK_TR3221, acno, pswd, nkey])
            elif key == TK_TR1221:  # 주식 미체결 (header: acno[11]+nrec[4]=15, grid=194)
                GRID = 194
                nrec = int(raw[11:15].decode('cp949', errors='ignore').strip() or "0")
                self._log(f"  [주식미체결] {nrec}건")
                for i in range(nrec):
                    g = raw[15 + i*GRID: 15 + (i+1)*GRID]
                    if len(g) < GRID: break
                    self._log(f"    {s(g[0:12])} 잔고={s(g[54:64])} 평단={s(g[74:84])} 현재={s(g[99:109])} 수익률={s(g[159:169])}%")
                if b_next:
                    acno = getattr(self, '_jngo_acno', '')
                    pswd = getattr(self, '_jngo_pswd', '')
                    self.ocx.dynamicCall("TR1221(int, QString, QString, int, QString)",
                                        [TK_TR1221, acno, pswd, 0, nkey])
            elif key == TK_TR3211:  # 선물 체결 (header: acno[11]+nrec[4]=15, grid=179)
                GRID = 179
                nrec = int(raw[11:15].decode('cp949', errors='ignore').strip() or "0")
                self._log(f"  [선물체결] {nrec}건")
                for i in range(nrec):
                    g = raw[15 + i*GRID: 15 + (i+1)*GRID]
                    if len(g) < GRID: break
                    self._log(f"    주문={s(g[6:12])} {s(g[18:26])} {s(g[56:64])} 주문가={s(g[84:95])} 주문량={s(g[95:102])} 체결가={s(g[106:117])} 체결량={s(g[117:124])} 미체결={s(g[130:137])} [{s(g[144:152])}]")
                if b_next:
                    acno = getattr(self, '_jngo_acno', '')
                    pswd = getattr(self, '_jngo_pswd', '')
                    self.ocx.dynamicCall(
                        "TR3211(int, QString, QString, int, int, QString, int, int, QString)",
                        [TK_TR3211, acno, pswd, 0, 1, "", 0, 0, nkey])
            elif key == TK_TR1211:  # 주식 체결 (header: acno[11]+nrec[4]=15, grid=222)
                GRID = 222
                nrec = int(raw[11:15].decode('cp949', errors='ignore').strip() or "0")
                self._log(f"  [주식체결] {nrec}건")
                for i in range(nrec):
                    g = raw[15 + i*GRID: 15 + (i+1)*GRID]
                    if len(g) < GRID: break
                    self._log(f"    주문={s(g[0:5])} {s(g[10:22])} {s(g[62:82])} 주문가={s(g[122:134])} 주문량={s(g[134:146])} 체결가={s(g[146:158])} 체결량={s(g[158:170])} 미체결={s(g[182:194])} [{s(g[194:214])}] {s(g[214:222])}")
                if b_next:
                    acno = getattr(self, '_jngo_acno', '')
                    pswd = getattr(self, '_jngo_pswd', '')
                    self.ocx.dynamicCall(
                        "TR1211(int, QString, QString, QString, int, int, int, int, QString)",
                        [TK_TR1211, acno, pswd, "", 0, 0, 0, 1, nkey])
            else:
                self._log(f"  [RecvData] key={key} (미처리)")
        except Exception as e:
            self._log(f"  [RecvData 파싱 오류] {e}")


app = QApplication(sys.argv)
window = TestWindow()
window.show()
sys.exit(app.exec_())
