import sys
from PyQt5.QAxContainer import QAxWidget
from PyQt5.QtWidgets import QApplication, QMainWindow

class IBKConnector(QMainWindow):
    def __init__(self):
        super().__init__()
        self.ocx = QAxWidget("{CDADD338-C7AB-4977-B65D-8E988B5958E3}", self)
        result = self.ocx.dynamicCall("Initialize()")
        print("Initialize:", result)

app = QApplication(sys.argv)
window = IBKConnector()
sys.exit(app.exec_())