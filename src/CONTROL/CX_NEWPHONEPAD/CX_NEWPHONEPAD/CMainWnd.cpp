// CMainWnd.cpp: 구현 파일
//

#include "pch.h"
#include "CX_NEWPHONEPAD.h"
#include "CMainWnd.h"

#pragma comment(lib, "lib/EAPIClientLib.lib")
// CMainWnd

IMPLEMENT_DYNAMIC(CMainWnd, CWnd)

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
		_snprintf_s(buf, sizeof(buf), _TRUNCATE, "MSG_%d", nMsgID);
		buf[sizeof(buf) - 1] = '\0';
		return buf;
	}
	}
}

static const char* SZ(const char* p) { return p ? p : ""; }

CMainWnd* CMainWnd::s_pDlg = NULL;

CMainWnd::CMainWnd(CWnd* pParent, _param* pParam)
{

}

CMainWnd::~CMainWnd()
{
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WM_APP_LOG, &CMainWnd::OnAppLog)
END_MESSAGE_MAP()


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
				s_pDlg->SetStatus("Login Failed: DEVICE_NOT_FOUND");
				break;
			case 1018:
				_snprintf_s(detail, sizeof(detail), _TRUNCATE,
					"[cx_newphonepad][%s]<%d>[LOGIN FAIL] Cause=%d DEVICE_MULTIPLE_FOUND - Multiple devices found", __FUNCTION__, __LINE__, nCause);
				s_pDlg->SetStatus("Login Failed: DEVICE_MULTIPLE");
				break;
			case 1019:
				_snprintf_s(detail, sizeof(detail), _TRUNCATE,
					"[cx_newphonepad][%s]<%d>[LOGIN FAIL] Cause=%d HR_EXT_NOT_FOUND - No extension in HR (ID-only login)", __FUNCTION__, __LINE__, nCause);
				s_pDlg->SetStatus("Login Failed: HR_EXT_NOT_FOUND");
				break;
			case 1020:
				_snprintf_s(detail, sizeof(detail), _TRUNCATE,
					"[cx_newphonepad][%s]<%d>[LOGIN FAIL] Cause=%d HR_EXT_NB_FOUND - Extension exists in another HR", __FUNCTION__, __LINE__, nCause);
				s_pDlg->SetStatus("Login Failed: HR_EXT_NB_FOUND");
				break;
			default:
				_snprintf_s(detail, sizeof(detail), _TRUNCATE,
					"[cx_newphonepad][%s]<%d>[LOGIN FAIL] Cause=%d %s", __FUNCTION__, __LINE__, nCause, SZ(pszCauseStr));
				s_pDlg->SetStatus("Login Failed");
				break;
			}
			detail[sizeof(detail) - 1] = '\0';
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
		_snprintf_s(pp, sizeof(pp), _TRUNCATE, "[PHONEPAD-RESP] Result=%s (%s)",
			SZ(pszResult), (pszResult && pszResult[0] == '1') ? "SUCCESS" : "FAIL");
		pp[sizeof(pp) - 1] = '\0';
		s_pDlg->PostLogFromCallback(pp);
	}

	return 0;
}

// Event Callback
DWORD CMainWnd::CB_Event(int nMessageID, char* pszCallID1, char* pszCallID2,
	char* pszANI, char* pszDNIS, char* pszCallType, char* pszCallDirection,
	char* pszPrevStatus, char* pszCallStatus, char* pszDeviceID,
	HWND hWnd, HANDLE hHnd)
{
	if (!s_pDlg) return 0;

	char buf[1024];
	_snprintf_s(buf, sizeof(buf), _TRUNCATE,
		"[EVENT] %s(%d) CallID=%s, ANI=%s, DNIS=%s, Dir=%s, %s->%s",
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
			"[UC-DATA] %s(%d) CallID=%s\r\n"
			"          DATA=[%s]\r\n"
			"          Result=%s, Cause=%s",
			GetMsgName(nMessageID), nMessageID,
			SZ(pszCallID), SZ(pszUUIData),
			SZ(pszResult), SZ(pszCause));
	}
	else {
		// EVT_UUI_DATA
		_snprintf_s(buf, sizeof(buf), _TRUNCATE,
			"[UUI] %s(%d) CallID=%s, Data=[%s], Result=%s",
			GetMsgName(nMessageID), nMessageID,
			SZ(pszCallID), SZ(pszUUIData), SZ(pszResult));
	}
	buf[sizeof(buf) - 1] = '\0';
	s_pDlg->PostLogFromCallback(buf);

	return 0;
}
// CMainWnd 메시지 처리기
void CMainWnd::SetParam(_param* pParam)
{
	m_Param.key = pParam->key;
	m_Param.name = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	//m_clrForeColor = m_Param.tRGB = pParam->tRGB;
	//m_clrBackColor = m_Param.pRGB = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());
/*
	m_sHideTradeAgent.Empty();

	m_sOption.Empty();

	int	idx = 0, pos = 0;
	CString	keys, text, tmps, strtemp;
	CString sOption{};
	sOption = m_Param.options;

	tmps = _T("/ ");
	keys = _T("wt");

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
		case 'w':
		{
			if (OptionParser(sOption, "/w") == 3)
				m_sHideTradeAgent = "1";
		}
		break;
		case 't':
		{
#ifndef DF_TEST_MODE
			if (!(long)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0L))
#endif
			{
				m_sOption = OptionParser(sOption, "/t");
			}
		}
		break;
		}
	}*/
}

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	BOOL bRtn = EAPIInitialize(CB_Resp, CB_Event, CB_UUI, GetSafeHwnd(), NULL);

	CString slog;
	slog.Format("[cx_newphonepad][%s]<%d>  bRtn = [%d]  CB_Resp=[%x]  CB_Event=[%x] CB_UUI=[%x]", __FUNCTION__, __LINE__,   bRtn, CB_Resp, CB_Event, CB_UUI);
	OutputDebugString(slog); 

	return 0;
}

LRESULT CMainWnd::OnAppLog(WPARAM wParam, LPARAM lParam)
{
	CString* pStr = reinterpret_cast<CString*>(lParam);
	if (pStr) {
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