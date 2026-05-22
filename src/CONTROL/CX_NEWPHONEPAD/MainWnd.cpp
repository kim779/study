// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_NEWPHONEPAD.h"
#include "MainWnd.h"
#include "CL_ENC.h"
#include "../../H/TickStore.h"

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

#define TM_EVENT_STATUS 9898



static const char* GetMsgName(int nMsgID)
{
	switch (nMsgID) {
		// Response
	case 1101: return "RES_LOGIN(로그인응답)";
	case 1102: return "RES_LOGOUT(로그아웃응답)";
	case 1103: return "RES_ADDLOGIN";
	case 1111: return "RES_MAKE_CALL(발신응답)";
	case 1112: return "RES_ANSWER_CALL(수신응답)";
	case 1113: return "RES_CALL_RELEASE(통화종료응답)";
	case 1114: return "RES_HOLD_CALL(보류응답)";
	case 1115: return "RES_RETRIEVE_CALL(보류복원응답)";
	case 1117: return "RES_TRANSFER(통화전환응답)";
	case 1118: return "RES_CONFERENCE(회의통화응답)";
	case 1119: return "RES_CONSULTATION(상담통화응답)";
	case 1120: return "RES_PHONEPAD(폰패드응답 1=성공";
	case 1175: return "RES_GET_UCDATA";
		// Event
	case 1202: return "EVT_CONNECTED(통화연결)";
	case 1203: return "EVT_RELEASE(통화종료)";
	case 1204: return "EVT_HOLD(통화보류)";
	case 1205: return "EVT_RETRIEVE(보류복원)";
	case 1206: return "EVT_TRANSFER(통화전환완료)";
	case 1207: return "EVT_CONFERENCE(회의통화연결)";
	case 1208: return "EVT_ALERTING(벨울림)";
	case 1209: return "EVT_OUTGOING(발신중)";
	case 1305: return "EVT_IN_SERVICE(로그인성공)";  
	case 1306: return "EVT_LOGOUT(인증해제이벤트)";
	case 1307: return "EVT_DIVERTED";
	case 1308: return "EVT_OFFERED(수화기들림)";
	case 1313: return "EVT_UUI_DATA(UUI데이터이벤트)";
	case 1314: return "EVT_UC_DATA(계좌번호 수신 성공)";    
	case 1315: return "EVT_REMOVE_PARTY(회의참가자제거)";
	case 1316: return "EVT_CONSULT_CALL(상담통화요청)";
	case 1317: return "EVT_CONSULT_INITIATED(상담통화시작)";
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

	m_Param.key = ((struct _param*)pParam)->key;
	m_Param.name = CString(((struct _param*)pParam)->name, ((struct _param*)pParam)->name.GetLength());
	m_Param.rect = CRect(((struct _param*)pParam)->rect.left, ((struct _param*)pParam)->rect.top, ((struct _param*)pParam)->rect.right, ((struct _param*)pParam)->rect.bottom);
	m_Param.fonts = CString(((struct _param*)pParam)->fonts, ((struct _param*)pParam)->fonts.GetLength());
	m_Param.point = ((struct _param*)pParam)->point;
	m_Param.style = ((struct _param*)pParam)->style;
	m_Param.tRGB = ((struct _param*)pParam)->tRGB;
	m_Param.pRGB = ((struct _param*)pParam)->pRGB;
	m_Param.options = CString(((struct _param*)pParam)->options, ((struct _param*)pParam)->options.GetLength());
}

void CMainWnd::parseOptions()
{
	int	idx = 0, pos = 0;
	CString	keys, text, tmps, strtemp;
	CString sOption{};
	sOption = m_Param.options;

	tmps = _T("/ ");
	keys = _T("dpbschlutoargmxwfyvk");

	for (int ii = 0; ii < keys.GetLength(); ii++)
	{
		tmps.SetAt(1, keys[ii]);
		idx = sOption.Find(tmps);
		if (idx < 0)
			continue;

		idx += 2;
		pos = sOption.Find('/', idx);

		text = (pos < 0) ? sOption.Mid(idx) : sOption.Mid(idx, pos - idx);
		text.TrimLeft();

		switch (keys[ii])
		{
		case 'u':
		{
		
		}
		break;
		case 't':
		{
		
		}
		break;
		case 'k':
		{
		
		}
		break;
		case 'd':
		{
	
		}
		break;
		case 'v':
		{
		
		}
		break;
		case 'y':
		{
		
		}
		break;
		case 'f':
		{
	
		}
		break;
		}
	}
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
	ON_WM_TIMER()
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
	DISP_FUNCTION(CMainWnd, "DoSomething", DoSomething, VT_BOOL, VTS_I2 VTS_BSTR)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CMainWnd, "EAPIConnect", dispidEAPIConnect, _EAPIConnect, VT_BOOL, VTS_BSTR VTS_I2)
	DISP_FUNCTION_ID(CMainWnd, "EAPIDisconnect", dispidEAPIDisconnect, _EAPIDisconnect, VT_BOOL, VTS_NONE)
	DISP_FUNCTION_ID(CMainWnd, "EAPILogin", dispidEAPILogin, _EAPILogin, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CMainWnd, "EAPILogout", dispidEAPILogout, _EAPILogout, VT_BOOL, VTS_NONE)
	DISP_FUNCTION_ID(CMainWnd, "EAPIPhonePad", dispidEAPIPhonePad, _EAPIPhonePad, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CMainWnd, "SendMSgToMain", dispidSendMSgToMain, _SendMSgToMain, VT_EMPTY, VTS_BSTR)
	DISP_PROPERTY_EX_ID(CMainWnd, "sRes", dispidsRes, GetsRes, SetsRes, VT_BSTR)
	DISP_PROPERTY_EX_ID(CMainWnd, "sState", dispidsState, GetsState, SetsState, VT_BSTR)
	DISP_PROPERTY_EX_ID(CMainWnd, "blogin", dispidblogin, Getblogin, Setblogin, VT_I2)
	DISP_PROPERTY_EX_ID(CMainWnd, "sMSGID", dispidsMSGID, GetsMSGID, SetsMSGID, VT_BSTR)
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

	CString slog{};
	char buf[1024];
	_snprintf_s(
		buf,
		sizeof(buf),
		_TRUNCATE,
		"[cx_newphonepad][서버요청결과] %s(%d) Result=%s, Cause=%s, Msg=%s", 
		GetMsgName(nMessageID),
		nMessageID,
		SZ(pszResult),
		SZ(pszCause),
		SZ(pszCauseStr));
	buf[sizeof(buf) - 1] = '\0';
	s_pDlg->m_sMsgID.Format("%d", nMessageID);
	s_pDlg->PostLogFromCallback(buf);

	// Login response
	if (nMessageID == 1101) {
		s_pDlg->m_sMsgID.Format("%d", nMessageID);
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
					"[로그인실패] Cause=%d DEVICE_NOT_FOUND - Device ID not found on server", nCause);
				s_pDlg->SetStatus(detail);
				break;
			case 1018:
				_snprintf_s(detail, sizeof(detail), _TRUNCATE,
					"[로그인실패] Cause=%d DEVICE_MULTIPLE_FOUND - Multiple devices found", nCause);
				s_pDlg->SetStatus(detail);
				break;
			case 1019:
				_snprintf_s(detail, sizeof(detail), _TRUNCATE,
					"[cx_newphonepad][로그인실패] Cause=%d HR_EXT_NOT_FOUND - No extension in HR (ID-only login)", nCause);
				s_pDlg->SetStatus(detail);
				break;
			case 1020:
				_snprintf_s(detail, sizeof(detail), _TRUNCATE,
					"[로그인실패] Cause=%d HR_EXT_NB_FOUND - Extension exists in another HR", nCause);
				s_pDlg->SetStatus(detail);
				break;
			default:
				_snprintf_s(detail, sizeof(detail), _TRUNCATE,
					"[로그인실패] Cause=%d %s",  nCause, SZ(pszCauseStr));
				s_pDlg->SetStatus(detail);
				break;
			}
			detail[sizeof(detail) - 1] = '\0';
			s_pDlg->PostLogFromCallback(detail);
		}
	}
	// Logout response
	else if (nMessageID == 1102) {
		s_pDlg->SetStatus("Logged out");
		s_pDlg->m_iLoginState = LOGOUT;
		s_pDlg->m_sMsgID.Format("%d", nMessageID);
	}
	// PhonePad response
	else if (nMessageID == 1120) {
		char pp[256];
		_snprintf_s(pp, sizeof(pp), _TRUNCATE, "[cx_newphonepad][PHONEPAD-RESP] Result=%s (%s)",
			SZ(pszResult), (pszResult && pszResult[0] == '1') ? "SUCCESS" : "FAIL");
		pp[sizeof(pp) - 1] = '\0';
		s_pDlg->m_sMsgID.Format("%d", nMessageID);
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
	s_pDlg->m_sMsgID.Format("%d", nMessageID);
	if (nMessageID == 1305)
	{
		s_pDlg->m_iLoginState = LOGINED;
	}
	return 0;
}

DWORD CMainWnd::CB_UUI(int nMessageID, char* pszCallID, char* pszUUIData,
	char* pszResult, char* pszCause, char* pszCauseStr, HWND hWnd, HANDLE hHnd)
{
	if (!s_pDlg) return 0;

	char buf[2048];

	if (nMessageID == 1314) {      //사용자로 부터 계좌비번 수신받음
		// EVT_UC_DATA - PhonePad result
		_snprintf_s(buf, sizeof(buf), _TRUNCATE,
			"[cx_newphonepad][계좌번호수신] %s(%d) CallID=%s  --- "
			"          DATA=[%s] --- "
			"          Result=%s, Cause=%s",
			GetMsgName(nMessageID), nMessageID,
			SZ(pszCallID), SZ(pszUUIData),
			SZ(pszResult), SZ(pszCause));

			CString stmp;
			stmp.Format("%s", pszUUIData);

			CString stoken{};
			int pos = stmp.Find(_T("HTS^"));
			if (pos >= 0)
			{
				pos += 4;
				int end = stmp.Find(_T("\""), pos);
				if (end > pos)
				{
					stoken = stmp.Mid(pos, end - pos);
					s_pDlg->m_sAccPass = Aes256DecryptBase64(stoken);
					s_pDlg->m_sAccPass.TrimRight();
					s_pDlg->SendPassToMap();
					s_pDlg->m_sState = "고객입력완료";
					s_pDlg->SetStatus(s_pDlg->m_sState);
				}
			}
			
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
	// {"IVRPARAM": "HTS^"}
	slog.Format("%d %s", type, sval);
	return TRUE;
}

void CMainWnd::SendPassToMap()
{
	m_sRes = m_sAccPass;
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnClick)), (LPARAM)m_Param.name.GetString()); //계좌비번 맵으로
	m_sAccPass.Empty();
	m_sRes.Empty();
}

VARIANT_BOOL CMainWnd::_EAPIDisconnect()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString slog;
	BOOL bret = EAPIDisconnect();
	slog.Format("%s", bret ? "커넥션해제 성공" : "커넥션해제 실패");
	SetStatus(slog);

	CString* pStr = new CString(slog);
	PostMessage(WM_APP_LOG, 0, reinterpret_cast<LPARAM>(pStr));

	if (bret)
		return VARIANT_TRUE;
	else
		return VARIANT_FALSE;
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
}

VARIANT_BOOL CMainWnd::_EAPIConnect(BSTR ip, SHORT port)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
	CString slog;

	m_nPort = port;
	m_strIP.Format("%s", ip);
	m_strIP.TrimRight();
	CStringA ipA(m_strIP);
	BOOL bRtn = EAPIConnect(ipA.GetString(), m_nPort);

	slog.Format("%s", bRtn ? "커넥션 성공" : "커넥션 실패");
	SetStatus(slog);

	CString* pStr = new CString(slog);
	PostMessage(WM_APP_LOG, 0, reinterpret_cast<LPARAM>(pStr));

	if(bRtn)
		return VARIANT_TRUE;
	else
		return VARIANT_FALSE;
}

LRESULT CMainWnd::OnAppLog(WPARAM wParam, LPARAM lParam)
{
	CString* pStr = reinterpret_cast<CString*>(lParam);
	if (pStr) {
		CTime time;
		time = CTime::GetCurrentTime();
		m_sRes.Format("[%02d:%02d:%02d]%s", time.GetHour(), time.GetMinute(), time.GetSecond(),(LPCTSTR)(*pStr));
		m_sRes.Replace("[cx_newphonepad]", "");

		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk)), (LPARAM)m_Param.name.GetString());  //메시지
		
		CString slog;
		slog.Format("[cx_newphonepad] pszStatus=[%s]",  (LPCTSTR)(*pStr));
		OutputDebugString(slog);

		//m_sRes.Empty();

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
	m_sState.Format("%s", pszStatus);
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange)), (LPARAM)m_Param.name.GetString()); //프로세스 상태
}

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	s_pDlg = this;
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	BOOL bRtn = EAPIInitialize(CB_Resp, CB_Event, CB_UUI, GetSafeHwnd(), NULL);

	CString slog;
	slog.Format("[cx_newphonepad][%s]<%d>  bRtn = [%d]  CB_Resp=[%x]  CB_Event=[%x] CB_UUI=[%x]", __FUNCTION__, __LINE__, bRtn, CB_Resp, CB_Event, CB_UUI);
	OutputDebugString(slog);

	m_sState.Format("%s", bRtn ? "초기화 성공" : "초기화 실패");
	SetTimer(TM_EVENT_STATUS, 1000, nullptr);
	return 0;
}

VARIANT_BOOL CMainWnd::_EAPILogin(BSTR sID, BSTR sDvcID, BSTR sUnqID, BSTR sOtpion)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_strID.Format("%s", sID);
	m_strID.TrimRight();
	m_strDevice.Format("%s", sDvcID);
	m_strDevice.TrimRight();
	m_strUnique.Format("%s", sUnqID);
	m_strUnique.TrimRight();

	CStringA idA(m_strID);
	CStringA devA(m_strDevice);
	CStringA uniA(m_strUnique);

	BOOL bRtn = EAPILogin(idA.GetString(), devA.GetString(), uniA.GetString(), "");

	CString slog;
	slog.Format("[cx_newphonepad][%s]<%d>  bRtn = [%d] ", __FUNCTION__, __LINE__, bRtn);
	OutputDebugString(slog);

	m_sState.Format("%s", bRtn ? "로그인" : "로그인 실패");
	SetTimer(TM_EVENT_STATUS, 1000, nullptr);

	if (bRtn)
		return VARIANT_TRUE;
	else
		return VARIANT_FALSE;
}


VARIANT_BOOL CMainWnd::_EAPILogout()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CStringA idA(m_strID);
	CStringA devA(m_strDevice);
	CStringA uniA(m_strUnique);

	BOOL bRtn = EAPILogout(idA.GetString(), devA.GetString(), uniA.GetString());

	CString slog;
	slog.Format("[cx_newphonepad][%s]<%d>  bRtn = [%d] ", __FUNCTION__, __LINE__, bRtn);
	OutputDebugString(slog);

	m_sState.Format("%s", bRtn ? "로그아웃" : "로그아웃 실패");
	SetTimer(TM_EVENT_STATUS, 1000, nullptr);

	if (bRtn)
		return VARIANT_TRUE;
	else
		return VARIANT_FALSE;
}


void CMainWnd::OnDestroy()
{
	EAPIDisconnect();
	EAPIFinalize();
	CWnd::OnDestroy();
}


VARIANT_BOOL CMainWnd::_EAPIPhonePad(BSTR sCallID, BSTR sSvcCode, BSTR sCustData, BSTR sDialNumber)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strCallID;
	strCallID.Format("%s", sCallID);
	strCallID.TrimRight();
	if (strCallID.IsEmpty())
		strCallID = "check";

	m_strPPCustData.Format("%s", sCustData);
	m_strPPCustData.TrimRight();
	m_strPPDialNum.Format("%s", sDialNumber);
	m_strPPDialNum.TrimRight();
	//{"IVRPARAM": "HTS^"}
	if(m_strPPCustData.GetLength() == 0)
		m_strPPCustData.Format("{\"IVRPARAM\": \"%s\"}", "HTS^");
	
	CStringA dialA(m_strPPDialNum);
	CStringA custA(m_strPPCustData);  

	// CallID="check" => No Call object, direct send
	BOOL bRtn = EAPIPhonePad(strCallID, "", custA.GetString(), dialA.GetString());

	CString slog;
	CTime time;
	time = CTime::GetCurrentTime();
	m_sRes.Format("[%02d:%02d:%02d]계좌비번요청 결과 = [%d] 고유번호=[%s] 다이알넘버=[%s] ", time.GetHour(), time.GetMinute(), time.GetSecond() ,bRtn, m_strPPCustData, m_strPPDialNum );
	OutputDebugString(slog);
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk)), (LPARAM)m_Param.name.GetString());  //메시지

	m_sState.Format("%s", bRtn ? "고객입력대기중" : "고개입력요청 실패");
	SetTimer(TM_EVENT_STATUS, 1000, nullptr);
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.

	if (bRtn)
		return VARIANT_TRUE;
	else
		return VARIANT_FALSE;
}
#include "../../H/TickStore.h"
void CMainWnd::_SendMSgToMain(BSTR sMsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString stmp, stemp;
	stmp.Format("%s", sMsg);
	stmp.TrimRight();
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


void CMainWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
		case TM_EVENT_STATUS:
		{
			KillTimer(nIDEvent);
			SetStatus(m_sState);
		}
	break;
	}
	CWnd::OnTimer(nIDEvent);
}

BSTR CMainWnd::GetsMSGID()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
	strResult = m_sMsgID.TrimRight();
	return strResult.AllocSysString();
}


void CMainWnd::SetsMSGID(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: 여기에 속성 처리기 코드를 추가합니다.
}

BSTR CMainWnd::GetsState()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
	strResult = m_sState.TrimRight();
	return strResult.AllocSysString();
}


void CMainWnd::SetsState(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_sState.Format("%s", newVal);
	m_sState.TrimRight();
	// TODO: 여기에 속성 처리기 코드를 추가합니다.
}


SHORT CMainWnd::Getblogin()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
	return m_iLoginState;
}


void CMainWnd::Setblogin(SHORT newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 속성 처리기 코드를 추가합니다.
}
