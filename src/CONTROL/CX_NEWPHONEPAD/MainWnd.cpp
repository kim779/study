// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_NEWPHONEPAD.h"
#include "MainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib, "lib/EAPIClientLib.lib")

#ifdef _DEBUG
#pragma comment(lib, "../../AXIS/Debug/axis.lib")
#else
#pragma comment(lib, "../../AXIS/Release/axis.lib")
#endif

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
		_snprintf_s(buf, sizeof(buf), _TRUNCATE, "MSG_%d", nMsgID);
		buf[sizeof(buf) - 1] = '\0';
		return buf;
	}
	}
}

static const char* SZ(const char* p) { return p ? p : ""; }

CMainWnd* CMainWnd::s_pDlg = NULL;
/////////////////////////////////////////////////////////////////////////////
// CMainWnd

CMainWnd::CMainWnd(CWnd* pParent, _param* pParam)
{
	EnableAutomation();
}

CMainWnd::~CMainWnd()
{
}

void CMainWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_MESSAGE(WM_APP_LOG, &CMainWnd::OnAppLog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
	DISP_FUNCTION(CMainWnd, "DoSomething", DoSomething, VT_BOOL, VTS_I2 VTS_BSTR)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CMainWnd, "EAPIConnect", dispidEAPIConnect, _EAPIConnect, VT_BOOL, VTS_BSTR VTS_I2)
	DISP_FUNCTION_ID(CMainWnd, "EAPIDisconnect", dispidEAPIDisconnect, _EAPIDisconnect, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CMainWnd, "EAPILogin", dispidEAPILogin, _EAPILogin, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CMainWnd, "EAPILogout", dispidEAPILogout, _EAPILogout, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CMainWnd, "EAPIPhonePad", dispidEAPIPhonePad, _EAPIPhonePad, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CMainWnd, "SendMSgToMain", dispidSendMSgToMain, _SendMSgToMain, VT_EMPTY, VTS_BSTR)
	DISP_PROPERTY_EX_ID(CMainWnd, "sRes", dispidsRes, GetsRes, SetsRes, VT_BSTR)
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {F2D3913A-1BCD-4EAB-980C-78555FDB3AAF}
static const IID IID_IMainWnd =
{ 0xf2d3913a, 0x1bcd, 0x4eab, { 0x98, 0xc, 0x78, 0x55, 0x5f, 0xdb, 0x3a, 0xaf } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

//서버요청에 대한 결과
DWORD CMainWnd::CB_Resp(int nMessageID, char* pszUniqueNO, char* pszDeviceType,
	char* pszResult, char* pszCause, char* pszCauseStr, HWND hWnd, HANDLE hHnd)
{
	if (!s_pDlg) return 0;

	char buf[1024];
	_snprintf_s(
		buf,
		sizeof(buf),
		_TRUNCATE,
		"[cx_newphonepad][%s]<%d>[RESP] %s(%d) Result=%s, Cause=%s, Msg=%s", __FUNCTION__, __LINE__,
		GetMsgName(nMessageID),
		nMessageID,
		SZ(pszResult),
		SZ(pszCause),
		SZ(pszCauseStr));
	buf[sizeof(buf) - 1] = '\0';
	s_pDlg->PostLogFromCallback(buf);

	// Login response
	if (nMessageID == 1101) {
		if (pszResult && pszResult[0] == '1') {
			s_pDlg->SetStatus("Logged In");
		}
		else
		{
			int nCause = pszCause ? atoi(pszCause) : 0;
			char detail[512];
			switch (nCause) {
			case 1017:
				_snprintf_s(detail, sizeof(detail), _TRUNCATE,
					"[cx_newphonepad][%s]<%d>[LOGIN FAIL] Cause=%d DEVICE_NOT_FOUND - Device ID not found on server", __FUNCTION__, __LINE__, nCause);
				s_pDlg->SetStatus(detail);
				break;
			case 1018:
				_snprintf_s(detail, sizeof(detail), _TRUNCATE,
					"[cx_newphonepad][%s]<%d>[LOGIN FAIL] Cause=%d DEVICE_MULTIPLE_FOUND - Multiple devices found", __FUNCTION__, __LINE__, nCause);
				s_pDlg->SetStatus(detail);
				break;
			case 1019:
				_snprintf_s(detail, sizeof(detail), _TRUNCATE,
					"[cx_newphonepad][%s]<%d>[LOGIN FAIL] Cause=%d HR_EXT_NOT_FOUND - No extension in HR (ID-only login)", __FUNCTION__, __LINE__, nCause);
				s_pDlg->SetStatus(detail);
				break;
			case 1020:
				_snprintf_s(detail, sizeof(detail), _TRUNCATE,
					"[cx_newphonepad][%s]<%d>[LOGIN FAIL] Cause=%d HR_EXT_NB_FOUND - Extension exists in another HR", __FUNCTION__, __LINE__, nCause);
				s_pDlg->SetStatus(detail);
				break;
			default:
				_snprintf_s(detail, sizeof(detail), _TRUNCATE,
					"[cx_newphonepad][%s]<%d>[LOGIN FAIL] Cause=%d %s", __FUNCTION__, __LINE__, nCause, SZ(pszCauseStr));
				s_pDlg->SetStatus(detail);
				break;
			}
			detail[sizeof(detail) - 1] = '\0';
			s_pDlg->PostLogFromCallback(detail);
		}
	}
	// Logout response
	else if (nMessageID == 1102) {
		s_pDlg->SetStatus("[cx_newphonepad]Logged Out");
	}
	// PhonePad response
	else if (nMessageID == 1120) {
		char pp[256];
		_snprintf_s(pp, sizeof(pp), _TRUNCATE, "[cx_newphonepad][PHONEPAD-RESP] Result=%s (%s)",
			SZ(pszResult), (pszResult && pszResult[0] == '1') ? "SUCCESS" : "FAIL");
		pp[sizeof(pp) - 1] = '\0';
		s_pDlg->PostLogFromCallback(pp);
	}

	return 0;
}

// Event Callback  실시간 상태 변화 이벤트
DWORD CMainWnd::CB_Event(int nMessageID, char* pszCallID1, char* pszCallID2,
	char* pszANI, char* pszDNIS, char* pszCallType, char* pszCallDirection,
	char* pszPrevStatus, char* pszCallStatus, char* pszDeviceID,
	HWND hWnd, HANDLE hHnd)
{
	if (!s_pDlg) return 0;

	char buf[1024];
	_snprintf_s(buf, sizeof(buf), _TRUNCATE,
		"[cx_newphonepad][EVENT] %s(%d) CallID=%s, ANI=%s, DNIS=%s, Dir=%s, %s->%s",
		GetMsgName(nMessageID), nMessageID,
		SZ(pszCallID1), SZ(pszANI), SZ(pszDNIS),
		SZ(pszCallDirection), SZ(pszPrevStatus), SZ(pszCallStatus));
	buf[sizeof(buf) - 1] = '\0';
	s_pDlg->PostLogFromCallback(buf);

	return 0;
}

DWORD CMainWnd::CB_UUI(int nMessageID, char* pszCallID, char* pszUUIData,
	char* pszResult, char* pszCause, char* pszCauseStr, HWND hWnd, HANDLE hHnd)
{
	if (!s_pDlg) return 0;

	char buf[2048];

	if (nMessageID == 1314) {
		// EVT_UC_DATA - PhonePad result
		_snprintf_s(buf, sizeof(buf), _TRUNCATE,
			"[cx_newphonepad][UC-DATA] %s(%d) CallID=%s\r\n"
			"          DATA=[%s]\r\n"
			"          Result=%s, Cause=%s",
			GetMsgName(nMessageID), nMessageID,
			SZ(pszCallID), SZ(pszUUIData),
			SZ(pszResult), SZ(pszCause));
	}
	else {
		// EVT_UUI_DATA
		_snprintf_s(buf, sizeof(buf), _TRUNCATE,
			"[cx_newphonepad][UUI] %s(%d) CallID=%s, Data=[%s], Result=%s",
			GetMsgName(nMessageID), nMessageID,
			SZ(pszCallID), SZ(pszUUIData), SZ(pszResult));
	}
	buf[sizeof(buf) - 1] = '\0';
	s_pDlg->PostLogFromCallback(buf);

	return 0;
}
// CMainWnd 메시지 처리기
/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers

BOOL CMainWnd::DoSomething(short type, BSTR sval)
{
	// TODO: Add your dispatch handler code here
	CString slog;
	slog.Format("%d %s", type, sval);
	
	return TRUE;
}


VARIANT_BOOL CMainWnd::_EAPIConnect(BSTR ip, SHORT port)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
	m_nPort = port;
	m_strIP.Format("%s", ip);

	CStringA ipA(m_strIP);
	BOOL bRtn = EAPIConnect(ipA.GetString(), m_nPort);

	CString slog;
	slog.Format("[cx_newphonepad][%s]<%d> bRtn=[%d]", __FUNCTION__, __LINE__, bRtn);
	OutputDebugString(slog);

	return VARIANT_TRUE;
}

LRESULT CMainWnd::OnAppLog(WPARAM wParam, LPARAM lParam)
{
	CString* pStr = reinterpret_cast<CString*>(lParam);
	if (pStr) {
		//m_sRes.Format("%s", pStr);
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk)), (LPARAM)m_Param.name.GetString());
		SetStatus(*pStr);
		delete pStr;
	}
	return 0;
}

void CMainWnd::PostLogFromCallback(const char* pszMsg)
{
	if (!pszMsg) return;
	CString* pStr = new CString(pszMsg);
	PostMessage(WM_APP_LOG, 0, reinterpret_cast<LPARAM>(pStr));
}

void CMainWnd::SetStatus(const char* pszStatus)
{
	CString slog;
	slog.Format("[cx_newphonepad][%s]<%d> pszStatus=[%s]", __FUNCTION__, __LINE__, pszStatus);
	OutputDebugString(slog);
}

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	BOOL bRtn = EAPIInitialize(CB_Resp, CB_Event, CB_UUI, GetSafeHwnd(), NULL);

	CString slog;
	slog.Format("[cx_newphonepad][%s]<%d>  bRtn = [%d]  CB_Resp=[%x]  CB_Event=[%x] CB_UUI=[%x]", __FUNCTION__, __LINE__, bRtn, CB_Resp, CB_Event, CB_UUI);
	OutputDebugString(slog);
	return 0;
}


void CMainWnd::_EAPIDisconnect()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	EAPIDisconnect();
	SetStatus("Disconnected");
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
}


void CMainWnd::_EAPILogin(BSTR sID, BSTR sDvcID, BSTR sUnqID, BSTR sOtpion)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_strID.Format("%s", sID);
	m_strID.Format("%s", sDvcID);
	m_strID.Format("%s", sUnqID);

	CStringA idA(m_strID);
	CStringA devA(m_strDevice);
	CStringA uniA(m_strUnique);

	BOOL bRtn = EAPILogin(idA.GetString(), devA.GetString(), uniA.GetString(), "");

	CString slog;
	slog.Format("[cx_newphonepad][%s]<%d>  bRtn = [%d] ", __FUNCTION__, __LINE__, bRtn);
	OutputDebugString(slog);
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
}


void CMainWnd::_EAPILogout()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CStringA idA(m_strID);
	CStringA devA(m_strDevice);
	CStringA uniA(m_strUnique);

	BOOL bRtn = EAPILogout(idA.GetString(), devA.GetString(), uniA.GetString());

	CString slog;
	slog.Format("[cx_newphonepad][%s]<%d>  bRtn = [%d] ", __FUNCTION__, __LINE__, bRtn);
	OutputDebugString(slog);
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.

	if (bRtn)
		_EAPIDisconnect();
}


void CMainWnd::OnDestroy()
{
	EAPIDisconnect();
	EAPIFinalize();
	CWnd::OnDestroy();
}


void CMainWnd::_EAPIPhonePad(BSTR sCallID, BSTR sSvcCode, BSTR sCustData, BSTR sDialNumber)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strCallID;
	strCallID.Format("%s", sCallID);
	strCallID.TrimRight();
	if (strCallID.IsEmpty())
		strCallID = "check";

	m_strPPCustData.Format("%s", sCustData);
	m_strPPCustData.TrimRight();
	m_strPPCustData.Format("%s", sDialNumber);
	m_strPPCustData.TrimRight();

	CStringA dialA(m_strPPDialNum);
	CStringA custA(m_strPPCustData);  

	// CallID="check" => No Call object, direct send
	BOOL bRtn = EAPIPhonePad(strCallID, "", custA.GetString(), dialA.GetString());

	CString slog;
	slog.Format("[cx_newphonepad][%s]<%d>  bRtn = [%d] ", __FUNCTION__, __LINE__, bRtn);
	OutputDebugString(slog);
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
}
#include "../../H/TickStore.h"
void CMainWnd::_SendMSgToMain(BSTR sMsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString stmp, stemp;
	stmp.Format("%s", sMsg);

	CWnd* pwnd = Axis_GetMainWnd();
	bool bret = pwnd->SendMessage(WM_USER + 0x7001, 0, (LPARAM)stmp.operator LPCTSTR());
}


BSTR CMainWnd::GetsRes()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	strResult = m_sRes;
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.

	return strResult.AllocSysString();
}


void CMainWnd::SetsRes(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_sRes.Format("%s", newVal);
	m_sRes.TrimRight();
	// TODO: 여기에 속성 처리기 코드를 추가합니다.
}
