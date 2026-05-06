#include "stdafx.h"
#include "EAPISample.h"
#include "EAPISampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//-------------------------------------------------------
// Message Name
//-------------------------------------------------------
static const char* GetMsgName(int nMsgID)
{
	switch (nMsgID) {
	// Response
	case 1101: return "RES_LOGIN";
	case 1102: return "RES_LOGOUT";
	case 1103: return "RES_ADDLOGIN";
	case 1111: return "RES_MAKE_CALL";
	case 1112: return "RES_ANSWER_CALL";
	case 1113: return "RES_CALL_RELEASE";
	case 1114: return "RES_HOLD_CALL";
	case 1115: return "RES_RETRIEVE_CALL";
	case 1117: return "RES_TRANSFER";
	case 1118: return "RES_CONFERENCE";
	case 1119: return "RES_CONSULTATION";
	case 1120: return "RES_PHONEPAD";
	case 1175: return "RES_GET_UCDATA";
	// Event
	case 1202: return "EVT_CONNECTED";
	case 1203: return "EVT_RELEASE";
	case 1204: return "EVT_HOLD";
	case 1205: return "EVT_RETRIEVE";
	case 1206: return "EVT_TRANSFER";
	case 1207: return "EVT_CONFERENCE";
	case 1208: return "EVT_ALERTING";
	case 1209: return "EVT_OUTGOING";
	case 1306: return "EVT_LOGOUT";
	case 1307: return "EVT_DIVERTED";
	case 1308: return "EVT_OFFERED";
	case 1313: return "EVT_UUI_DATA";
	case 1314: return "EVT_UC_DATA";
	case 1315: return "EVT_REMOVE_PARTY";
	case 1316: return "EVT_CONSULT_CALL";
	case 1317: return "EVT_CONSULT_INITIATED";
	default:
		{
			static char buf[32];
			_snprintf(buf, sizeof(buf), "MSG_%d", nMsgID);
			buf[sizeof(buf)-1] = '\0';
			return buf;
		}
	}
}

static const char* SZ(const char* p) { return p ? p : ""; }

//-------------------------------------------------------
CEAPISampleDlg* CEAPISampleDlg::s_pDlg = NULL;

CEAPISampleDlg::CEAPISampleDlg(CWnd* pParent)
	: CDialog(CEAPISampleDlg::IDD, pParent)
	, m_strIP(_T("127.0.0.1"))
	, m_nPort(5511)
	, m_strID(_T(""))
	, m_strDevice(_T(""))
	, m_strUnique(_T(""))
	, m_strPPDialNum(_T("7007111"))
	, m_strPPCustData(_T("{\"IVRPARAM\": \"DATA\"}"))
{
	s_pDlg = this;
}

void CEAPISampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IP,          m_strIP);
	DDX_Text(pDX, IDC_EDIT_PORT,        m_nPort);
	DDX_Text(pDX, IDC_EDIT_ID,          m_strID);
	DDX_Text(pDX, IDC_EDIT_DEVICE,      m_strDevice);
	DDX_Text(pDX, IDC_EDIT_UNIQUE,      m_strUnique);
	DDX_Text(pDX, IDC_EDIT_PP_DIALNUM,  m_strPPDialNum);
	DDX_Text(pDX, IDC_EDIT_PP_CUSTDATA, m_strPPCustData);
	DDX_Control(pDX, IDC_EDIT_LOG,      m_editLog);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_stStatus);
}

BEGIN_MESSAGE_MAP(CEAPISampleDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_CONNECT,        &CEAPISampleDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT,     &CEAPISampleDlg::OnBnClickedBtnDisconnect)
	ON_BN_CLICKED(IDC_BTN_LOGIN,          &CEAPISampleDlg::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_BTN_LOGOUT,         &CEAPISampleDlg::OnBnClickedBtnLogout)
	ON_BN_CLICKED(IDC_BTN_PHONEPAD,       &CEAPISampleDlg::OnBnClickedBtnPhonepad)
	ON_BN_CLICKED(IDC_BTN_PHONEPAD_CHECK, &CEAPISampleDlg::OnBnClickedBtnPhonepadCheck)
	ON_BN_CLICKED(IDC_BTN_CLEAR_LOG,      &CEAPISampleDlg::OnBnClickedBtnClearLog)
	ON_MESSAGE(WM_APP_LOG,                &CEAPISampleDlg::OnAppLog)
END_MESSAGE_MAP()

//-------------------------------------------------------
BOOL CEAPISampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	BOOL bRtn = EAPIInitialize(CB_Resp, CB_Event, CB_UUI, GetSafeHwnd(), NULL);
	CString strLog;
	strLog.Format(_T("[INIT] EAPIInitialize : %s"), bRtn ? _T("OK") : _T("FAIL"));
	AppendLog(strLog);
	SetStatus(bRtn ? "Initialized" : "Init Failed");

	return TRUE;
}

void CEAPISampleDlg::OnDestroy()
{
	EAPIDisconnect();
	EAPIFinalize();
	CDialog::OnDestroy();
}

//-------------------------------------------------------
// Connect / Disconnect
//-------------------------------------------------------
void CEAPISampleDlg::OnBnClickedBtnConnect()
{
	UpdateData(TRUE);
	CStringA ipA(m_strIP);
	BOOL bRtn = EAPIConnect(ipA.GetString(), m_nPort);

	CString strLog;
	strLog.Format(_T("[CONNECT] %s:%d => %s"), m_strIP, m_nPort, bRtn ? _T("OK") : _T("FAIL"));
	AppendLog(strLog);
	if (bRtn) SetStatus("Connected");
}

void CEAPISampleDlg::OnBnClickedBtnDisconnect()
{
	EAPIDisconnect();
	AppendLog(_T("[DISCONNECT] OK"));
	SetStatus("Disconnected");
}

//-------------------------------------------------------
// Login / Logout
//-------------------------------------------------------
void CEAPISampleDlg::OnBnClickedBtnLogin()
{
	UpdateData(TRUE);
	CStringA idA(m_strID);
	CStringA devA(m_strDevice);
	CStringA uniA(m_strUnique);

	BOOL bRtn = EAPILogin(idA.GetString(), devA.GetString(), uniA.GetString(), "");

	CString strLog;
	strLog.Format(_T("[LOGIN] ID=%s, Device=%s => %s"), m_strID, m_strDevice, bRtn ? _T("Sent") : _T("FAIL"));
	AppendLog(strLog);
	if (bRtn) SetStatus("Login requested...");
}

void CEAPISampleDlg::OnBnClickedBtnLogout()
{
	UpdateData(TRUE);
	CStringA idA(m_strID);
	CStringA devA(m_strDevice);
	CStringA uniA(m_strUnique);

	BOOL bRtn = EAPILogout(idA.GetString(), devA.GetString(), uniA.GetString());

	CString strLog;
	strLog.Format(_T("[LOGOUT] => %s"), bRtn ? _T("Sent") : _T("FAIL"));
	AppendLog(strLog);
	if (bRtn) SetStatus("Logout requested...");
}

//-------------------------------------------------------
// PhonePad
//-------------------------------------------------------
void CEAPISampleDlg::OnBnClickedBtnPhonepad()
{
	UpdateData(TRUE);
	CStringA dialA(m_strPPDialNum);
	CStringA custA(m_strPPCustData);

	// CallID=NULL => Active TALKING call
	// SvcCode="" (empty, as per real log)
	BOOL bRtn = EAPIPhonePad(NULL, "", custA.GetString(), dialA.GetString());

	CString strLog;
	strLog.Format(_T("[PHONEPAD] Dial=%s, CustData=%s => %s"),
		m_strPPDialNum, m_strPPCustData, bRtn ? _T("Sent") : _T("FAIL"));
	AppendLog(strLog);
}

void CEAPISampleDlg::OnBnClickedBtnPhonepadCheck()
{
	UpdateData(TRUE);
	CStringA dialA(m_strPPDialNum);
	CStringA custA(m_strPPCustData);

	// CallID="check" => No Call object, direct send
	BOOL bRtn = EAPIPhonePad("check", "", custA.GetString(), dialA.GetString());

	CString strLog;
	strLog.Format(_T("[PHONEPAD-CHECK] Dial=%s, CustData=%s => %s"),
		m_strPPDialNum, m_strPPCustData, bRtn ? _T("Sent") : _T("FAIL"));
	AppendLog(strLog);
}

//-------------------------------------------------------
// Log
//-------------------------------------------------------
void CEAPISampleDlg::OnBnClickedBtnClearLog()
{
	m_editLog.SetWindowText(_T(""));
}

void CEAPISampleDlg::AppendLog(const CString& strMsg)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString strLine;
	strLine.Format(_T("[%02d:%02d:%02d.%03d] %s\r\n"),
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, strMsg);

	int nLen = m_editLog.GetWindowTextLength();
	m_editLog.SetSel(nLen, nLen);
	m_editLog.ReplaceSel(strLine);
}

LRESULT CEAPISampleDlg::OnAppLog(WPARAM wParam, LPARAM lParam)
{
	CString* pStr = reinterpret_cast<CString*>(lParam);
	if (pStr) {
		AppendLog(*pStr);
		delete pStr;
	}
	return 0;
}

void CEAPISampleDlg::PostLogFromCallback(const char* pszMsg)
{
	if (!pszMsg) return;
	CString* pStr = new CString(pszMsg);
	PostMessage(WM_APP_LOG, 0, reinterpret_cast<LPARAM>(pStr));
}

void CEAPISampleDlg::SetStatus(const char* pszStatus)
{
	if (m_stStatus.GetSafeHwnd())
		m_stStatus.SetWindowText(CString(pszStatus));
}

//-------------------------------------------------------
// DLL Callbacks (static, called from DLL thread)
//-------------------------------------------------------

// Response Callback
DWORD CEAPISampleDlg::CB_Resp(int nMessageID, char* pszUniqueNO, char* pszDeviceType,
	char* pszResult, char* pszCause, char* pszCauseStr, HWND hWnd, HANDLE hHnd)
{
	if (!s_pDlg) return 0;

	char buf[1024];
	_snprintf(buf, sizeof(buf),
		"[RESP] %s(%d) Result=%s, Cause=%s, Msg=%s",
		GetMsgName(nMessageID), nMessageID,
		SZ(pszResult), SZ(pszCause), SZ(pszCauseStr));
	buf[sizeof(buf)-1] = '\0';
	s_pDlg->PostLogFromCallback(buf);

	// Login response
	if (nMessageID == 1101) {
		if (pszResult && pszResult[0] == '1') {
			s_pDlg->SetStatus("Logged In");
		} else {
			int nCause = pszCause ? atoi(pszCause) : 0;
			char detail[512];
			switch (nCause) {
			case 1017:
				_snprintf(detail, sizeof(detail),
					"[LOGIN FAIL] Cause=%d DEVICE_NOT_FOUND - Device ID not found on server", nCause);
				s_pDlg->SetStatus("Login Failed: DEVICE_NOT_FOUND");
				break;
			case 1018:
				_snprintf(detail, sizeof(detail),
					"[LOGIN FAIL] Cause=%d DEVICE_MULTIPLE_FOUND - Multiple devices found", nCause);
				s_pDlg->SetStatus("Login Failed: DEVICE_MULTIPLE");
				break;
			case 1019:
				_snprintf(detail, sizeof(detail),
					"[LOGIN FAIL] Cause=%d HR_EXT_NOT_FOUND - No extension in HR (ID-only login)", nCause);
				s_pDlg->SetStatus("Login Failed: HR_EXT_NOT_FOUND");
				break;
			case 1020:
				_snprintf(detail, sizeof(detail),
					"[LOGIN FAIL] Cause=%d HR_EXT_NB_FOUND - Extension exists in another HR", nCause);
				s_pDlg->SetStatus("Login Failed: HR_EXT_NB_FOUND");
				break;
			default:
				_snprintf(detail, sizeof(detail),
					"[LOGIN FAIL] Cause=%d %s", nCause, SZ(pszCauseStr));
				s_pDlg->SetStatus("Login Failed");
				break;
			}
			detail[sizeof(detail)-1] = '\0';
			s_pDlg->PostLogFromCallback(detail);
		}
	}
	// Logout response
	else if (nMessageID == 1102) {
		s_pDlg->SetStatus("Logged Out");
	}
	// PhonePad response
	else if (nMessageID == 1120) {
		char pp[256];
		_snprintf(pp, sizeof(pp), "[PHONEPAD-RESP] Result=%s (%s)",
			SZ(pszResult), (pszResult && pszResult[0] == '1') ? "SUCCESS" : "FAIL");
		pp[sizeof(pp)-1] = '\0';
		s_pDlg->PostLogFromCallback(pp);
	}

	return 0;
}

// Event Callback
DWORD CEAPISampleDlg::CB_Event(int nMessageID, char* pszCallID1, char* pszCallID2,
	char* pszANI, char* pszDNIS, char* pszCallType, char* pszCallDirection,
	char* pszPrevStatus, char* pszCallStatus, char* pszDeviceID,
	HWND hWnd, HANDLE hHnd)
{
	if (!s_pDlg) return 0;

	char buf[1024];
	_snprintf(buf, sizeof(buf),
		"[EVENT] %s(%d) CallID=%s, ANI=%s, DNIS=%s, Dir=%s, %s->%s",
		GetMsgName(nMessageID), nMessageID,
		SZ(pszCallID1), SZ(pszANI), SZ(pszDNIS),
		SZ(pszCallDirection), SZ(pszPrevStatus), SZ(pszCallStatus));
	buf[sizeof(buf)-1] = '\0';
	s_pDlg->PostLogFromCallback(buf);

	return 0;
}

// UUI Callback  (EVT_UUI_DATA=1313, EVT_UC_DATA=1314)
DWORD CEAPISampleDlg::CB_UUI(int nMessageID, char* pszCallID, char* pszUUIData,
	char* pszResult, char* pszCause, char* pszCauseStr, HWND hWnd, HANDLE hHnd)
{
	if (!s_pDlg) return 0;

	char buf[2048];

	if (nMessageID == 1314) {
		// EVT_UC_DATA - PhonePad result
		_snprintf(buf, sizeof(buf),
			"[UC-DATA] %s(%d) CallID=%s\r\n"
			"          DATA=[%s]\r\n"
			"          Result=%s, Cause=%s",
			GetMsgName(nMessageID), nMessageID,
			SZ(pszCallID), SZ(pszUUIData),
			SZ(pszResult), SZ(pszCause));
	} else {
		// EVT_UUI_DATA
		_snprintf(buf, sizeof(buf),
			"[UUI] %s(%d) CallID=%s, Data=[%s], Result=%s",
			GetMsgName(nMessageID), nMessageID,
			SZ(pszCallID), SZ(pszUUIData), SZ(pszResult));
	}
	buf[sizeof(buf)-1] = '\0';
	s_pDlg->PostLogFromCallback(buf);

	return 0;
}
