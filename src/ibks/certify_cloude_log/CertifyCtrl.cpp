// CertifyCtrl.cpp : CCertifyCtrl ActiveX 컨트롤 클래스의 구현입니다.

#include "pch.h"
#include "framework.h"
#include "Certify.h"
#include "CertifyCtrl.h"
#include "CertifyPropPage.h"
#include "afxdialogex.h"

#include "guide.h"
#include "issue.h"
#include "invalid.h"
#include "overpass.h"
#include "countpass.h"
//#include "passInput.h"
#include "ExitPass.h"

#include "../h/axis.h"
#include "../h/axiserr.h"
#include "../h/axisfire.h"
#include "../h/axisvar.h"

//#include "../../IBK/H/axislog.h"

#define AXLOG_MODULE_TAG "CERTIFY"
#include "../h/axlog.h"

#pragma	comment(lib, "CaLib/SKComdIF")
#pragma	message("Automatically linking with SKComdIF library")

#define DF_DEV
#define DEV_CLOUDE_SERVER  "twas.signkorea.com"
#define REAL_CLOUDE_SERVER "cert.signkorea.com"
#define DEV_AGREEMENT_URL "https://tweb.signkorea.com:8700/notice/html/conditionsOfUse.txt"
#define REAL_AGREEMENT_URL "https://center.signkorea.com:8700/notice/html/conditionsOfUse.txt"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	TM_MSG	10000

IMPLEMENT_DYNCREATE(CCertifyCtrl, COleControl)

// 메시지 맵입니다.

BEGIN_MESSAGE_MAP(CCertifyCtrl, COleControl)
	ON_WM_TIMER()
	ON_MESSAGE(WM_USER, OnMessage)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()

// 디스패치 맵입니다.

BEGIN_DISPATCH_MAP(CCertifyCtrl, COleControl)
	DISP_FUNCTION(CCertifyCtrl, "OnCertify", OnCertify, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION(CCertifyCtrl, "Certify", Certify, VT_BOOL, VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CCertifyCtrl, "CertifyErr", CertifyErr, VT_BOOL, VTS_I4 VTS_I4)
	DISP_FUNCTION(CCertifyCtrl, "CertifyId", CertifyId, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CCertifyCtrl, "CertifyEx", CertifyEx, VT_BOOL, VTS_I4 VTS_I4)
	DISP_FUNCTION(CCertifyCtrl, "CertifyFull", CertifyFull, VT_I4, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CCertifyCtrl, "CertifyName", CertifyName, VT_I4, VTS_I4)
	DISP_FUNCTION_ID(CCertifyCtrl, "CertifyCloud", dispidCertifyCloud, CertifyCloud, VT_I4, VTS_I4)
END_DISPATCH_MAP()

// 이벤트 맵입니다.

BEGIN_EVENT_MAP(CCertifyCtrl, COleControl)
	EVENT_CUSTOM("OnFire", OnFire, VTS_I4  VTS_I4  VTS_I4)
END_EVENT_MAP()

// 속성 페이지입니다.

// TODO: 필요에 따라 속성 페이지를 추가합니다.  카운트도 이에 따라 증가해야 합니다.
BEGIN_PROPPAGEIDS(CCertifyCtrl, 1)
	PROPPAGEID(CCertifyPropPage::guid)
END_PROPPAGEIDS(CCertifyCtrl)

// 클래스 팩터리와 GUID를 초기화합니다.

IMPLEMENT_OLECREATE_EX(CCertifyCtrl, "AxisCertify.CertifyCtrl.IBK2019",
	0x3277f0b4,0xb8be,0x42aa,0x9b,0x58,0x2f,0xa8,0x88,0x86,0xac,0x64)

// 형식 라이브러리 ID와 버전입니다.

IMPLEMENT_OLETYPELIB(CCertifyCtrl, _tlid, _wVerMajor, _wVerMinor)

// 인터페이스 ID입니다.

const IID IID_DCertify = {0x45b73aba,0xcdd7,0x4cb0,{0xa9,0xf3,0xd5,0x38,0x6e,0x3c,0xfa,0xcd}};
const IID IID_DCertifyEvents = {0x605e9aa3,0xdfb7,0x4ab1,{0xbd,0x82,0xcd,0xa3,0x2d,0xa4,0xf5,0x0a}};

// 컨트롤 형식 정보입니다.

static const DWORD _dwCertifyOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CCertifyCtrl, IDS_CERTIFY, _dwCertifyOleMisc)

// CCertifyCtrl::CCertifyCtrlFactory::UpdateRegistry -
// CCertifyCtrl에 대한 시스템 레지스트리 항목을 추가하거나 제거합니다.

void FileLog(CString slog)
{
	OutputDebugString(slog);
}

BOOL CCertifyCtrl::CCertifyCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: 컨트롤이 아파트 모델 스레딩 규칙을 준수하는지
	// 확인합니다. 자세한 내용은 MFC Technical Note 64를
	// 참조하십시오. 컨트롤이 아파트 모델 규칙에
	// 맞지 않는 경우 다음에서 여섯 번째 매개 변수를 변경합니다.
	// afxRegApartmentThreading을 0으로 설정합니다.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_CERTIFY,
			IDB_CERTIFY,
			afxRegApartmentThreading,
			_dwCertifyOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


// CCertifyCtrl::CCertifyCtrl - 생성자

CCertifyCtrl::CCertifyCtrl()
{
	InitializeIIDs(&IID_DCertify, &IID_DCertifyEvents);

	m_ca = caNO;
	m_user = _T("");
	m_name = _T("cn=");
	ZeroMemory(m_encpass, sizeof(m_encpass));

	m_auto = false;
	m_calogon = false;
	m_certifys = _T("AXIS Certify !!!");
	m_msg = _T("");

	m_emaps.RemoveAll();

	m_bDev = FALSE;
	m_bCloudeUse = FALSE;
	m_bCloudeInit = FALSE;
}

// CCertifyCtrl::~CCertifyCtrl - 소멸자

CCertifyCtrl::~CCertifyCtrl()
{
	ZeroMemory(m_encpass, sizeof(m_encpass));
	m_user = _T("");
	m_name = _T("");
	m_emaps.RemoveAll();
}

// CCertifyCtrl::OnDraw - 그리기 함수

void CCertifyCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& /* rcInvalid */)
{
	if (!pdc)
		return;

	// TODO: 다음 코드를 사용자가 직접 작성한 그리기 코드로 바꾸십시오.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}

// CCertifyCtrl::DoPropExchange - 지속성 지원

void CCertifyCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 지속적인 사용자 지정 속성 모두에 대해 PX_ functions를 호출합니다.
}


// CCertifyCtrl::OnResetState - 컨트롤을 기본 상태로 다시 설정

void CCertifyCtrl::OnResetState()
{
	COleControl::OnResetState();  // DoPropExchange에 들어 있는 기본값을 다시 설정합니다.

	// TODO: 여기에서 다른 모든 컨트롤의 상태를 다시 설정합니다.
}

// CCertifyCtrl 메시지 처리기
LRESULT CCertifyCtrl::OnMessage(WPARAM wParam, LPARAM lParam)
{
	axlog(LOG_CERTIFY, "OnMessage cmd=%d hi=%d m_ca=%d", LOWORD(wParam), HIWORD(wParam), (int)m_ca);
	switch (LOWORD(wParam))
	{
	case checkPass:
		if (HIWORD(wParam))	// remove certificate
			removeCertificate();
		else
			return checkPasswd(CString((char*)lParam));
	case rebootAxis:
		OnFire(FEV_CA, MAKELONG(closeCA, HIWORD(wParam)), 0);
		break;
	case encryptPass:
		OnFire(FEV_CA, MAKELONG(encryptCA, HIWORD(wParam)), lParam);
		return (long)lParam;
	}
	return 0;
}

long CCertifyCtrl::OnCertify(long pBytes, long nBytes)
{
	axlog(LOG_CERTIFY, "OnCertify ENTER m_ca=%d pBytes=%s nBytes=%ld", (int)m_ca, pBytes ? "non-null" : "NULL", nBytes);
	if (pBytes == NULL)
	{
		m_ca = caNO;
		m_name = _T("cn=");
		axlog(LOG_CERTIFY, "OnCertify pBytes==NULL -> m_ca=caNO, reset");
		return NULL;
	}

	CString	refs, auths, msg = _T("");
	int	idx, retv, caL = *(int*)nBytes;
	int	ecode = -1, result = 0;
	bool	reissue, query = true;
	bool	retry = false;
	CString	text;
	struct	_caH* caH;

	retv = -1;
	*(int*)nBytes = retv;
	axlog(LOG_CERTIFY, "OnCertify switch(m_ca=%d) caL=%d", (int)m_ca, caL);
	switch (m_ca)
	{
	case caPWDa:
		retv--;
		m_msg = CString((char*)pBytes, caL);
		SetTimer(TM_MSG, 200, NULL);
		m_ca = caRUN;
		*(int*)nBytes = retv;
		axlog(LOG_CERTIFY, "OnCertify caPWDa -> m_ca=caRUN, SetTimer(TM_MSG)");
		return NULL;

	case caNO:
		ZeroMemory(&m_context, sizeof(APP_CONTEXT));
		ZeroMemory(&m_contextNew, sizeof(SD_API_CONTEXT_NEW));
		if (caL <= 0)
			return NULL;
		if (sk_if_cert_InitContextApp(&m_context, NULL, 0) == -1)
		{
			axlog(LOG_CERTIFY, "OnCertify caNO sk_if_cert_InitContextApp FAILED");
			MessageBox(_T("공인인증 사용시 오류가 발생할 수 있사오니 고객센터(1588-0030, 1544-0050)로 문의바랍니다."),
				_T("공인인증 초기화 오류"), MB_OKCANCEL | MB_SYSTEMMODAL | MB_ICONINFORMATION);
			return NULL;
		}

		caH = (struct _caH*)pBytes;
		m_name = CString(caH->dns, sizeof(caH->dns));
		// 필수 화면
		text = CString(caH->map, sizeof(caH->map));
		idx = text.Find(_T('\0'));
		if (idx != -1)
			text = text.Left(idx);
		for (; !text.IsEmpty(); )
		{
			m_emaps.SetAt(text.Left(L_MAPN), NULL);
			if (text.GetLength() > L_MAPN)
				text = text.Mid(L_MAPN);
			else
				text.Empty();
		}
		//
		text = CString(caH->ecode, sizeof(caH->ecode));
		if (text.GetAt(0) == 'S')
			ecode = atoi( (const char *)(text.Mid(1)).GetString() );
		else if ( text.GetAt(0) == 'E' || atoi((const char*)(text.GetString())) )
			msg = m_name;

		idx = m_name.Find(_T('\0'));
		if (idx != -1)
			m_name = m_name.Left(idx);
		m_name.TrimRight();

		reissue = false;
		axlog(LOG_CERTIFY, "OnCertify caNO ecode=%d msgEmpty=%d name=%.40s", ecode, msg.IsEmpty(), m_name.GetString());
		if (ecode >= 0 || !msg.IsEmpty())
		{
			idx = 0;
			switch (ecode)
			{
			case 1:	// 폐지
			{
				CGuide guide(typeREVOKE);
				idx = guide.DoModal();
				reissue = true;
			}
			break;
			case 2:	// 정지
			{
				CGuide guide(typeSUSPEND);
				idx = guide.DoModal();
				reissue = true;
			}
			break;
			case 3:	// 만료
			{
				CGuide guide(typeEXPIRE);
				idx = guide.DoModal();
				reissue = true;
			}
			break;
			case 4:	// 미등록
			{
				CIssue issue;
				idx = issue.DoModal();
			}
			break;
			case 5:	// 신규발급
			{
				CInvalid invalid;
				idx = invalid.DoModal();
			}
			break;
			case 20:// 비밀번호 5회오류
			{
				COverPass passover;
				idx = passover.DoModal();
				reissue = true;
			}
			break;
			case 9:	// 상태불분명
				msg = m_name;
			default:
				if (!guideMsg((msgNO)ecode, msg))
					return NULL;
				break;
			}

			axlog(LOG_CERTIFY, "OnCertify caNO ecode=%d dialog idx=%d reissue=%d", ecode, idx, reissue);
			if (idx > 0)
			{
				switch (idx)
				{
				case IDC_ISSUE:
					certify(reissue);
					*(int*)nBytes = -2;
					return NULL;

				case IDC_OTHER:
					otherCertificate();
					*(int*)nBytes = -2;
				default:
					return NULL;
				}
			}
			query = false;
		}

	case caPWD:
		axlog(LOG_CERTIFY, "OnCertify caPWD fallthrough m_ca==caPWD? %d query=%d", m_ca == caPWD, query);
		if (m_ca == caPWD)
		{   //ID로그인  - 공동비번 틀리면 여기로 온다
			retry = true;
			struct	_pwdR* pwdR = (struct _pwdR*)pBytes;
			switch (pwdR->ret[0])
			{
			case 'X':
				retry = false;
			case '0':
				break;
			default:
				msg = CString(pwdR->msg, sizeof(pwdR->msg)); msg.Trim();
				guideMsg((msgNO)-1, msg);
				return NULL;
			}
			//죽는 경우가 생기면
			
			char* pdata = new char[2];
			memset(pdata, 0x00, 2);
			memcpy(pdata, (char*)&pwdR->pwdn[0], 1);
			CString sdata;
			sdata.Format("%s", pdata); sdata.TrimRight();
			idx = atoi(sdata);
			
			//idx = atoi((const char*)pwdR->pwdn[0]); // CString(pwdR->pwdn, sizeof(pwdR->pwdn)) );

			CCountPass countDlg(idx, retry);     // 공동오류 횟수 다이알로그
			int countDlgRet = countDlg.DoModal();
			axlog(LOG_CERTIFY, "OnCertify caPWD CCountPass cnt=%d retry=%d dlgRet=%d", idx, retry, countDlgRet);
			switch (countDlgRet)
			{
			case IDOK:
				break;
			case IDC_ISSUE:
				certify(true);
				*(int*)nBytes = -2;
				return NULL;

			case IDC_OTHER:
				otherCertificate();
				*(int*)nBytes = -2;
			default:
				return NULL;
			}
		}

		if (query)
		{
			for (; true; )
			{
				result = queryDn(m_name, (int*)nBytes, retry);
				axlog(LOG_CERTIFY, "OnCertify caPWD/caNO query=true queryDn result=%d", result);
				switch (result)
				{
				case rspNULL:
					return NULL;

				case rspPASSWD:
					m_ca = caPWD;
					m_string = _T("pswd\t");
					*(int*)nBytes = m_string.GetLength();
					strcpy_s(m_pBytes, sizeof(m_pBytes), (char*)m_string.GetString());
					return (long)m_pBytes;

				case rspOTHER:
					otherCertificate();
					*(int*)nBytes = -2;
					return NULL;

				case rspISSUE:
				case rspREISSUE:
					certify(result == rspREISSUE);
					*(int*)nBytes = -2;
					return NULL;

				case rspOK:
					break;
				}
				break;
			}
		}

	case caNOx:
		if (!sign())
		{
			axlog(LOG_CERTIFY, "OnCertify caNOx sign() FAILED");
			sk_if_cert_static_context_release();
			return NULL;
		}
		m_ca = caOK;
		axlog(LOG_CERTIFY, "OnCertify caNOx sign() OK -> m_ca=caOK");
		break;

	case caOK:
		m_ca = caRUN;
		*(int*)nBytes = 0;
		axlog(LOG_CERTIFY, "OnCertify caOK -> m_ca=caRUN, savePasswd()");

		savePasswd();
		return NULL;

	case caOKx:
		if (caL <= 0)
			return NULL;

		caH = (struct _caH*)pBytes;
		m_name = CString(caH->dns, sizeof(caH->dns));
		// 필수 화면
		text = CString(caH->map, sizeof(caH->map));
		idx = text.Find(_T('\0'));
		if (idx != -1)
			text = text.Left(idx);
		for (; !text.IsEmpty(); )
		{
			m_emaps.SetAt(text.Left(L_MAPN), NULL);
			if (text.GetLength() > L_MAPN)
				text = text.Mid(L_MAPN);
			else
				text.Empty();
		}
		//
		idx = m_name.Find(_T('\0'));
		if (idx != -1)
			m_name = m_name.Left(idx);
		m_name.TrimRight();

		m_ca = caRUN;
		*(int*)nBytes = 0;
		axlog(LOG_CERTIFY, "OnCertify caOKx -> m_ca=caRUN name=%.40s", m_name.GetString());
		return NULL;

	default:
		axlog(LOG_CERTIFY, "OnCertify default (unhandled m_ca=%d)", (int)m_ca);
		return NULL;
	}

	axlog(LOG_CERTIFY, "OnCertify EXIT falling through, returning m_pBytes m_nBytes=%d", m_nBytes);
	*(int*)nBytes = m_nBytes;
	return (long)m_pBytes;
}

BOOL CCertifyCtrl::Certify(long pBytes, long nBytes, long infos)
{
	axlog(LOG_CERTIFY, "Certify(TR-sign) ENTER m_ca=%d nBytes=%ld", (int)m_ca, nBytes);
	switch (m_ca)
	{
	case caNO:
		axlog(LOG_CERTIFY, "Certify(TR-sign) m_ca==caNO -> TRUE (no-op)");
		return TRUE;
	case caRUN:
		break;
	default:
		axlog(LOG_CERTIFY, "Certify(TR-sign) m_ca=%d not caRUN -> FEV_CA guideCA/AE_ECERTIFY", (int)m_ca);
		OnFire(FEV_CA, MAKELONG(guideCA, AE_ECERTIFY), 0);
		return FALSE;
	}

	char	encpass[32 + 1];
	UString	src, des;

	ZeroMemory(encpass, sizeof(encpass));
	CString maps = CString((char*)infos, L_MAPN);
	BOOL mustCertify = isMustCertify(maps);
	axlog(LOG_CERTIFY, "Certify(TR-sign) maps=%.8s m_auto=%d mustCertify=%d m_bCloudeUse=%d", maps.GetString(), m_auto, mustCertify, m_bCloudeUse);
	if (m_auto && !mustCertify)  //로그인시 공동인증자동 체크했을 경우
		CopyMemory(encpass, m_encpass, sizeof(encpass));
	else	//로그인시 공동인증자동 체크 안했을 경우 공동인증입력창을 작은걸로 받는다
	{
		if (!m_bCloudeUse)
		{
			CString	 pass;
			pass = checkPasswd();   //주문낼때 여기 타고 온다 작은 공인인증창 팝업
			axlog(LOG_CERTIFY, "Certify(TR-sign) checkPasswd() empty=%d", pass.IsEmpty());
			if (pass.IsEmpty())
			{
				if (m_ca != caPWDa)
					m_ca = caRUN;
				return FALSE;
			}
			sk_if_GetEncryptedPassword((char*)pass.operator LPCTSTR(), m_encpass);
			CopyMemory(encpass, m_encpass, sizeof(encpass));
			pass = _T("");
		}
	}

	if (m_bCloudeUse)
	{
		int iret = Cloude_ConTraction_sign(pBytes, nBytes);
		axlog(LOG_CERTIFY, "Certify(TR-sign) cloud Cloude_ConTraction_sign iret=%d", iret);
		return  iret == 1 ? TRUE :  FALSE;
	}
	else
	{
		m_sync.Lock();
		m_nBytes = *(int*)nBytes;
		src.length = m_nBytes;
		src.value = (unsigned char*)pBytes;
		if (sk_if_cert_SignData_notEncode(&m_context, encpass, &src, &des, NULL))
		{
			m_sync.Unlock();
			ZeroMemory(encpass, sizeof(encpass));
			axlog(LOG_CERTIFY, "Certify(TR-sign) local sk_if_cert_SignData_notEncode FAILED errorCode=%d", sk_if_GetLastErrorCode());
			OnFire(FEV_CA, MAKELONG(guideCA, AE_ECERTIFY), 0);
			return FALSE;
		}

		pBytes += m_nBytes;
		m_nBytes += des.length;

		*(int*)nBytes = m_nBytes;
		CopyMemory((void*)pBytes, des.value, des.length);
		sk_if_cert_MemFree(des.value);
		ZeroMemory(encpass, sizeof(encpass));
		m_sync.Unlock();
		axlog(LOG_CERTIFY, "Certify(TR-sign) local sign OK signedLen=%d", m_nBytes);
		return TRUE;
	}
}

BOOL CCertifyCtrl::CertifyErr(long pBytes, long nBytes)
{
	enum { eNONE = 0, eHTS = 1, eSIGN = 2 } eKind;

	axlog(LOG_CERTIFY, "CertifyErr ENTER nBytes=%ld m_ca=%d", nBytes, (int)m_ca);
	eKind = eNONE;
	if (nBytes <= 0)
		return (int)eKind;

	m_ca = caNO;
	switch (((char*)pBytes)[0])
	{
	case 'h': case 'H':
		eKind = eHTS;
		break;
	case 's': case 'S':
		eKind = eSIGN;
		break;
	default:
		return (int)eKind;
	}

	int	idx;
	CString	string = CString((char*)(pBytes + 1), nBytes - 1);

	CString	eCode = string.Left(4);
	string = string.Mid(4);

	int eCodeX = atoi((const char*)eCode.GetString());

	axlog(LOG_CERTIFY, "CertifyErr eKind=%d(%s) eCodeX=%d", (int)eKind, eKind == eHTS ? "HTS" : "SIGN", eCodeX);
	switch (eCodeX)
	{
	case 2001:
		if (eKind == eSIGN) guideMsg(msg1);		break;
	case 2008:
	case 4444:
	case 1:		// 폐지
		if (eKind == eSIGN)
		{
			CGuide guide(typeREVOKE);
			idx = guide.DoModal();
			switch (idx)
			{
			case IDC_ISSUE:
				certify(true);
				break;
			case IDC_OTHER:
				otherCertificate();
			default:
				removeCertificate();
				return FALSE;
			}
		}
		break;

	case 2424:
		if (eKind == eSIGN) guideMsg(msg2);		break;
	case 1405:
		if (eKind == eHTS)  guideMsg(msg4);		break;
	case 8888:
		if (eKind == eHTS)  guideMsg(msg0, string);	break;
	case 8889:
		if (eKind == eHTS)  guideMsg(msg8);		break;
	case 8899:
		if (eKind == eHTS)  guideMsg(msg5);		break;
	case 8989:
		if (false && eKind == eHTS)
			removeCertificate();
		break;
	case 3532:	// 비밀번호 over
		if (eKind == eSIGN)
		{
			COverPass passover;
			idx = passover.DoModal();

			switch (idx)
			{
			case IDC_ISSUE:
				certify(true);
				break;
			case IDC_OTHER:
				otherCertificate();
			default:
				removeCertificate();
				return FALSE;
			}
		}
		break;
	case 9999:	// SignKorea 접속오류
	case 9998:	// 입력값 불일치
		if (eKind == eSIGN)  guideMsg(msg0, string);	break;
	default:
		switch (eKind)
		{
		case eSIGN:
			guideMsg(msg3);	break;
		case eHTS:
			guideMsg(msg0, string);
		default:
			break;
		}
		break;
	}

	if (eKind == eSIGN)
	{
		switch (eCodeX)
		{
		case 2001:
		case 2008:
		case 1:
		case 3532:
			removeCertificate();
			break;
		default:
			break;
		}
	}
	sk_if_cert_static_context_release();

	axlog(LOG_CERTIFY, "CertifyErr EXIT eKind=%d eCodeX=%d msg=%.60s", (int)eKind, eCodeX, string.GetString());
	TRACE("[%c][%d][%s]\n", eKind == eHTS ? 'H' : 'S', eCodeX, string);
	return FALSE;
}

// id(12) + pass(10) + info(10) + cpass(30) + ip(15) + mac(16)
// info[1] : 주문자동서명, info[2] : 공인인증사용
//ID 로그인시 최초 
void CCertifyCtrl::CertifyId(long pBytes)
{
	int	idx;
	CString	info, pass;
	//	char* nAuto = _T("주문시매번비밀번호입력한다");
	//	char* yAuto = _T("주문시매번비밀번호입력하지않는다");

	m_user = CString((char*)pBytes, 12);
	info = CString((char*)(pBytes + 22), 10);
	pass = CString((char*)(pBytes + 32), 30);
	m_user.TrimRight();
	idx = pass.Find(_T('\0'));
	if (idx != -1)
		pass = pass.Left(idx);
	//	pass.TrimRight();	// 공인인증 비밀번호 마지막에 ' '(space)가 있는 경우가 있어 TrimRight 제외
	FillMemory((char*)(pBytes + 32), 30, ' ');

	if (!m_calogon)
	{
		sk_if_SetPasswordEncMode(1);
		sk_if_GetEncryptedPassword((char*)pass.operator LPCTSTR(), m_encpass);
		pass = _T("");
	}
	m_calogon = false;

	m_auto = (info.GetAt(1) == '1') ? true : false;
	m_certifys.Format(_T("%s"), m_user.GetString());
	//	m_certifys.Format("%s(%s)=%s", m_user, "사용자", m_auto ? yAuto : nAuto);
	axlog(LOG_CERTIFY, "CertifyId EXIT userLen=%d m_auto=%d", m_user.GetLength(), m_auto);
}

BOOL CCertifyCtrl::CertifyEx(long pBytes, long nBytes)
{
	axlog(LOG_CERTIFY, "CertifyEx ENTER pBytes=%s", pBytes ? "non-null" : "NULL");
	if (pBytes == NULL)
	{
		if (MessageBox(_T("공인인증 절차를 수행할 수 없습니다.\r\n공인인증을 사용하지 않고 진행을 계속하시겠습니까?"),
			_T("공인인증 미사용 동의 확인"), MB_OKCANCEL | MB_SYSTEMMODAL | MB_ICONINFORMATION) == IDOK)
			return TRUE;

		axlog(LOG_CERTIFY, "CertifyEx user declined -> FEV_CA closeCA (terminate AXIS)");
		OnFire(FEV_CA, MAKELONG(closeCA, true), 0);	// terminate AXIS
		return FALSE;
	}

	return TRUE;
}

bool CCertifyCtrl::sign()
{
	UString src, des;

	src.length = m_certifys.GetLength();
	src.value = (unsigned char*)m_certifys.operator LPCTSTR();  //m_certifys  는  로그인 시도 하는 ID이다.
	if (sk_if_cert_SignData(&m_context, NULL, &src, &des))
	{
		axlog(LOG_CERTIFY, "sign() sk_if_cert_SignData FAILED errorCode=%d", sk_if_GetLastErrorCode());
		return false;
	}

	CString	keys = _T("key\t");
	m_nBytes = keys.GetLength();
	CopyMemory(m_pBytes, keys, m_nBytes);

	CopyMemory(&m_pBytes[m_nBytes], des.value, des.length);
	m_nBytes += des.length;

	sk_if_cert_MemFree(des.value);
	axlog(LOG_CERTIFY, "sign() OK m_nBytes=%d", m_nBytes);
	return true;
}

int CCertifyCtrl::queryDn(CString dn_name, int* nBytes, bool retry)
{
	CWnd* pWnd;
	BOOL	success = FALSE;

	axlog(LOG_CERTIFY, "queryDn ENTER retry=%d hasCachedEncpass=%d", retry, m_encpass[0] != NULL);
	pWnd = AfxGetMainWnd();
	sk_if_SetWrongPasswordLimit(1);
	m_contextNew.sd.bOldStorage = 0x00;
	m_contextNew.sd.bForceAllCaSearchMode = 1;
	strcpy_s(m_contextNew.sd.szUserId, E_BUFLEN, (char*)dn_name.GetString());
	ZeroMemory(m_contextNew.sd.szOldPasswd, sizeof(m_contextNew.sd.szOldPasswd));
	sk_if_SetKeySaferMode(1);  //9 안내받음  1기존 (IBKs 내부방화벽에서 9번관련 막힌다함..)
	if (retry || m_encpass[0] == NULL)
	{
		if (pWnd && IsWindow(pWnd->GetSafeHwnd()))
			sk_if_DialogModalMode(pWnd->GetSafeHwnd());
		success = sk_if_CertSetSelectExt(&m_contextNew, CONTEXT_SELECT2, SEARCH_ALLMEDIA);
	}
	else
	{
		CopyMemory(m_contextNew.sd.szOldPasswd, m_encpass, min(sizeof(m_encpass), sizeof(m_contextNew.sd.szOldPasswd)));
		success = sk_if_CertSetSelect(&m_contextNew.sd);
	}

	axlog(LOG_CERTIFY, "queryDn CertSetSelect(Ext) success=%d errorCode=%d", success, success ? 0 : sk_if_GetLastErrorCode());
	if (!success)
	{
		switch (sk_if_GetLastErrorCode())
		{
		case 2001:		// 만료
			sk_if_cert_static_context_release();
			{
				CGuide guide(typeEXPIRE);
				switch (guide.DoModal())
				{
				case IDC_OTHER:
					return rspOTHER;
				case IDC_ISSUE:
					return rspREISSUE;
				default:
					break;
				}
				return rspNULL;
			}

		case 2501:		// 취소
			guideMsg(msg7);
			sk_if_cert_static_context_release();
			return rspNULL;

		case 2500:		// 인증서 없슴
		{
			CInvalid invalid;
			switch (invalid.DoModal())
			{
			case IDC_OTHER:
				return rspOTHER;
			case IDC_ISSUE:
				return rspISSUE;
			case IDOK:
				return queryDn(dn_name, nBytes);
			default:
				break;
			}
			return rspNULL;
		}
		case 1001:		// 인증서 서명오류 (5회)
		case 2417:
			return rspPASSWD;

		case 2508:		// 인증서 갱신 선택
			guideMsg(msg19);
			sk_if_cert_static_context_release();
			return rspNULL;

		default:
			guideMsg(msg12);
			break;
		}
	}

	if (sk_if_cert_preset_context(&m_context, &m_contextNew.sd) < 0)
	{
		axlog(LOG_CERTIFY, "queryDn sk_if_cert_preset_context FAILED errorCode=%d", sk_if_GetLastErrorCode());
		MessageBox(sk_if_GetLastErrorMsg(), _T("공인인증-Preset 실패"), MB_OK | MB_SYSTEMMODAL | MB_ICONINFORMATION);
		return rspNULL;
	}
	axlog(LOG_CERTIFY, "queryDn EXIT rspOK");
	return rspOK;
}

void CCertifyCtrl::savePasswd()
{
	if (m_context.pInterfaceContext == NULL)
		return;

	CString	pass;

	pass = CString(m_context.pInterfaceContext->szOldPasswd, sizeof(m_context.pInterfaceContext->szOldPasswd));
	sk_if_GetEncryptedPassword((char*)pass.operator LPCTSTR(), m_encpass);
	pass = _T("");
}

BOOL CCertifyCtrl::checkPasswd(CString pass)
{
	CString	text;
	UString	src, des;
	char	encpass[32 + 1];

	m_sync.Lock();
	ZeroMemory(encpass, sizeof(encpass));
	sk_if_GetEncryptedPassword((char*)pass.operator LPCTSTR(), encpass);

	src.length = 0;
	src.value = NULL;
	if (sk_if_cert_SignData_notEncode(&m_context, encpass, &src, &des, NULL))
	{
		int errCode = sk_if_GetLastErrorCode();
		axlog(LOG_CERTIFY, "checkPasswd(CString) FAILED errorCode=%d", errCode);
		if (errCode == 2417)
		{
			m_ca = caPWDa;
			m_string = _T("pswd\t");
			OnFire(FEV_CA, MAKELONG(invokeCA, m_string.GetLength()), (long)(char*)m_string.operator LPCTSTR());
		}
		ZeroMemory(encpass, sizeof(encpass));
		m_sync.Unlock();
		return FALSE;
	}

	sk_if_cert_MemFree(des.value);
	ZeroMemory(encpass, sizeof(encpass));
	m_sync.Unlock();
	axlog(LOG_CERTIFY, "checkPasswd(CString) OK");
	return TRUE;
}

////주문시
CString CCertifyCtrl::checkPasswd()
{
	CString	text, pass = _T("");
	UString	src, des;
	char	encpass[32 + 1];
	int	pswdL;

	m_sync.Lock();
	ZeroMemory(encpass, sizeof(encpass));
	pswdL = sizeof(m_context.pInterfaceContext->szOldPasswd);
	ZeroMemory(m_context.pInterfaceContext->szOldPasswd, pswdL);

	src.length = 0;
	src.value = NULL;

	sk_if_SetKeySaferMode(1); //9 안내받음  1기존 (IBKs 내부방화벽에서 9번관련 막힌다함..)

	if (sk_if_cert_SignData_notEncode(&m_context, encpass, &src, &des, NULL))
	{
		int errCode = sk_if_GetLastErrorCode();
		axlog(LOG_CERTIFY, "checkPasswd() FAILED errorCode=%d", errCode);
		if (errCode == 2417)
		{
			m_ca = caPWDa;
			m_string = _T("pswd\t");
			OnFire(FEV_CA, MAKELONG(invokeCA, m_string.GetLength()), (long)(char*)m_string.operator LPCTSTR());
		}
		ZeroMemory(encpass, sizeof(encpass));
		m_sync.Unlock();
		return pass;
	}
	sk_if_cert_MemFree(des.value);

	ZeroMemory(encpass, sizeof(encpass));
	pass = CString(m_context.pInterfaceContext->szOldPasswd, pswdL);
	m_sync.Unlock();
	axlog(LOG_CERTIFY, "checkPasswd() OK");
	return pass;
}

bool CCertifyCtrl::certify(bool reissue)
{
	axlog(LOG_CERTIFY, "certify(reissue=%d) -> FEV_CA htmlCA newissue.jsp", reissue);
	m_string = _T("http://www.ibks.com/LoadService.jsp?url=/customer/certificate/newissue.jsp");
	OnFire(FEV_CA, MAKELONG(htmlCA, 0), (long)(char*)m_string.operator LPCTSTR());
	return true;
}

void CCertifyCtrl::otherCertificate()
{
	axlog(LOG_CERTIFY, "otherCertificate -> FEV_CA htmlCA etc_entry.jsp");
	m_string = _T("http://www.ibks.com/LoadService.jsp?url=/customer/certificate/etc_entry.jsp");
	OnFire(FEV_CA, MAKELONG(htmlCA, 0), (long)(char*)m_string.operator LPCTSTR());
}

void CCertifyCtrl::removeCertificate()
{
	//	if (strlen(m_contextNew.sd.szUserId) > 0)
	//		sk_if_RemoveFromMediaExt(&m_contextNew);
}

bool CCertifyCtrl::guideMsg(msgNO msgno, CString guide, CString title)
{
	struct	_msg {
		char	no;
		bool	confirm;
		char* title;
		char* msg;
	};

	struct	_msg	msg[] = {
		{  0, false, NULL,		NULL },
		{  1, false, "",		"사용하신 인증서는 만료된 인증서입니다." },
		{  2, false, "",		"사용하신 인증서는 IBK투자증권에서 사용하실 수 없는 인증서입니다.\r\n신규발급 : 다시 로그인 후 인증서 선택창에서 취소를 선택하여 주십시오." },
		{  3, false, "",		"전자서명을 실패하였습니다. 고객센터에 문의하여 주십시오.\r\n(고객센터 1588-0030, 1544-0050)" },
		{  4, false, "",		"사용하신 인증서는 IBK투자증권에 등록되지 않은 인증서입니다.\r\n신규발급 : 다시 로그인 후 인증서 선택창에서 취소를 선택하여 주십시오." },
		{  5, false, "",		"인증서를 재발급하기 위하여 당사 고객센터(1588-0030, 1544-0050)를 통하여\r\n본인여부를 확인하신 후 인증서를 재발급 받으시기 바랍니다." },
		{  6, false, "",		"인증서 발급을 취소했습니다." },
		{  7, false, "인증서 선택 취소","인증서 선택을 취소하였습니다" },
		{  8, false, "",		"입력하신 전자서명 비밀번호가 맞지 않습니다. 전자서명 비밀번호가 5회오류로 인하여\r\n더 이상 전자서명을 수행할 수 없습니다.\r\n고객센터를 통해 본인여부를 확인하신 후 인증서를 재발급 받으시기 바랍니다.\r\n(고객센터 1588-0030, 1544-0050)" },
		{  9, false, "",		"전자서명 비밀번호가 틀립니다" },
		{ 10, false, "",		"고객님의 인증서가 정상적으로 발급(재발급) 되었습니디.\r\n다시 로그인하시기 바랍니다.\r\n기타 궁금하신 사항은 고객센터로 문의하시기 바랍니다.\r\n(고객센터 1588-0030, 1544-0050)" },
		{ 11, true,  "",		"인증서가 정상적으로 발급되었습니다.\r\n인증서의 변경된 내용을 안전하게 보관하기 위해 백업절차를 진행합니다.\r\n백업이 완료된후 플로피디스켓(또는 이동매체)은 윈도우의 디스크오류검사기를 이용하여\r\n정상인지 확인하신 후, 안전하게 보관하시기 바랍니다.\r\n\r\n인증서 백업을 위해 백업용 플로피디스켓(또는 이동매체)을 준비하여 주십시오."},
		{ 12, false, "기타오류",	"알 수 없는 오류가 발생했습니다.\r\n고객센터(1588-0030, 1544-0050)로 문의하시기 바랍니다." },
		{ 13, false, "",		"사용자 인증 정보 취득에 실패하였습니다. 고객센터로 문의 바랍니다.\r\n(고객센터 1588-0030, 1544-0050)" },
		{ 14, false, "",		"사용자 정보 인증에 실패하였습니다. 고객센터로 문의 바랍니다.\r\n(고객센터 1588-0030, 1544-0050)" },
		{ 15, false, "",		"시용자 부가정보 입력을 취소했습니다." },
		{ 16, false, "",		"타기관 인증서 등록을 취소했습니다." },
		{ 17, false, "",		"전자서명에 실패하였습니다." },
		{ 18, false, "",		"타기관 인증서 등록이 정상 처리 되었습니다. 프로그램을 재시작합니다." },
		{ 19, false, "",		"인증서 갱신후 다시 이용바랍니다." },
	};

	UINT	type = MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL;

	axlog(LOG_CERTIFY, "guideMsg msgno=%d customGuide=%d", (int)msgno, !guide.IsEmpty());
	if (guide.IsEmpty())
	{
		if ((int)msgno < 0 || (int)msgno >= sizeof(msg) || msg[msgno].msg == NULL)
			return false;
		guide = msg[msgno].msg;
		if (msg[msgno].confirm)
			type = MB_OKCANCEL | MB_ICONINFORMATION | MB_SYSTEMMODAL;
		title = msg[msgno].title;
	}
	if (title.IsEmpty())
		title = _T("IBK투자증권");

	if (MessageBox(guide, title, type) == IDCANCEL)
		return false;

	return true;
}

BOOL CCertifyCtrl::isMustCertify(CString maps)
{
	void* ptr;

	return m_emaps.Lookup(maps, ptr);
}

void CCertifyCtrl::OnTimer(UINT nIDEvent)
{
	COleControl::OnTimer(nIDEvent);
	
	KillTimer(nIDEvent);
	if (nIDEvent != TM_MSG)
		return;

	struct	_pwdR* pwdR;
	CString	string;

	pwdR = (struct _pwdR*)(char*)m_msg.operator LPCTSTR();
	axlog(LOG_CERTIFY, "OnTimer(TM_MSG) pwdR->ret=%c", pwdR->ret[0]);
	switch (pwdR->ret[0])
	{
	case 'X':
		{
		CExitPass exitpass;
		exitpass.DoModal();
		SendMessage(WM_USER, MAKEWPARAM(checkPass, 1));
		SendMessage(WM_USER, MAKEWPARAM(rebootAxis, true));
		}
		return;
	case '0':
		string.Format(_T("전자서명 비밀번호 %c회 오류입니다"), pwdR->pwdn[0]);
		break;
	default:
		string = CString(pwdR->msg, sizeof(pwdR->msg));
		string.TrimLeft();
		string.TrimRight();
		break;
	}
	MessageBox(string, _T("IBK투자증권"), MB_ICONINFORMATION | MB_OK);
}

// updateXX_20160503
long CCertifyCtrl::CertifyFull(long pInB, long pInL, long pOutB, long pOutL)
{
//	m_bCloudeUse = CheckCloude();
	axlog(LOG_CERTIFY, "CertifyFull ENTER m_bCloudeUse=%d m_ca=%d pInL=%ld", m_bCloudeUse, (int)m_ca, pInL);
	if (m_bCloudeUse)   //클라우드 로그인
	{
		m_slog.Format("[CERTIFY] DEV = [%d] [%s]", m_bDev, "!!!!!!!  클라우드 로그인 !!!!!!");
		//--------------------------------------------------------------------------------------------------------------------------------
		//클라우드 초기화
		int	pswdL, rc = 0;
		switch (m_ca)
		{
		case caNO:
		{
			InitCloude();
			//CloudConfig config;
			//memset(&config, 0x00, sizeof(CloudConfig));

			//config.CUSTOMER_ID = "SS0068";
			//if (m_bDev)
			//{
			//	config.SITE_CODE[0] = "U1MwMDY4XzA2";
			//	config.SERVER_HOST = DEV_CLOUDE_SERVER;
			//	config.AGREEMENT_URL = DEV_AGREEMENT_URL;
			//}
			//else
			//{
			//	config.SITE_CODE[0] = "U1MwMDY4X0FYSVNfRA==";
			//	config.SERVER_HOST = REAL_CLOUDE_SERVER;
			//	config.AGREEMENT_URL = REAL_AGREEMENT_URL;
			//}
			//
			//config.VERSION = "1.0.0";
			//config.SERVER_PORT = 8500;

			////타임아웃 기간을 설정합니다 1~10초 (msec 단위)
			//config.TIMEOUT_MSEC = 3000;

			//sk_if_Set_CloudConfig(config);
			//sk_if_DialogModalMode(m_hWnd); //모달 모드

//--------------------------------------------------------------------------------------------------------------------------------
			//APP_CONTEXT	m_context;
			//SD_API_CONTEXT_NEW m_contextNew;

			int rc = 0;
			SD_API_CONTEXT_NEW Context;
			memset(&Context, 0x00, sizeof(Context)); //구조체 선언
			memset(&m_context, 0, sizeof(APP_CONTEXT));
			sk_if_cert_InitContextApp(&m_context, NULL, (int)false); //초기화
			
			////sk_if_SetPolicyFilter(0, NULL); //인증서 OID 필터 기능 적용
			////sk_if_SetExipreCheckSkip(TRUE); //인증서 갱신안내 스킵옵션
			////sk_if_Set_Show_OnlyValidateCloudCert_flag(1); //클라우드 인증서 유효한것만 보이기(컴퓨터시간 기준)
			////sk_if_SetKeySaferMode(nflag); //키보드보안 모듈 연동	

			////클라우드 인증서 선택 기능
			////클라우드 인증서 선택을 성공하면 해당 구조체에
			////인증서 정보와 개인키, 임시 비밀번호가 담깁니다
			////사용 완료시까지 임시 비밀번호와 개인키 정보를 초기화 해서는 안됩니다
			////성공시 0, 실패시 0이와에 값이 반환됩니다

			////선택 옵션
			//// SELECT_CLOUDCERT_OPTION_NOSEARCHLOCAL	1 - Dn 입력한 인증서 로컬저장소에서 검색하지 않겠다   
			//// SELECT_CLOUDCERT_OPTION_NOCERT_OUT		4 - 클라우드에 인증서가 없으면 바로 에러 반환 (에러코드 2500)
			rc = sk_if_CloudCertSetSelectExt(&Context, 0);
			axlog(LOG_CERTIFY, "CertifyFull caNO/cloud sk_if_CloudCertSetSelectExt rc=%d", rc);
			if (rc == 0)
			{

			}
			else
			{
				memset(m_contextNew.sd.szDN, 0x00, sizeof(m_contextNew.sd.szDN));
				int errorCode = sk_if_GetLastErrorCode();
				if (rc == -2) //-2면 연결끊기를 한것이다.
				{
					*(int*)pOutL = -3;
					m_slog.Format("[CERTIFY]  sk_if_CloudCertSetSelectExt rc. = [%d]  errorCode=[%d]", rc, errorCode);
					return errorCode;
				}
				
				*(int*)pOutL = -3;
				m_slog.Format("[CERTIFY]  sk_if_CloudCertSetSelectExt rc.. = [%d]  errorCode=[%d]", rc, errorCode);

				return errorCode;
			}

			//Context.sd.bOldStorage == 7 은 클라우드 저장소에 있는 인증서임을 표시합니다
			////이뒤로 서명
			sk_if_cert_preset_context(&m_context, &Context.sd);

			CString plain, strResult;
			UString p1, p2, p3;

			plain = "abcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()-=";

			p1.value = (unsigned char*)LPCTSTR(plain);
			p1.length = plain.GetLength();

			//p1 = 원문 , p2 결과값 , p3 = R값 결과
			memset(&p3, 0x00, sizeof(UString));
			rc = sk_if_cert_SignDataWithR(&m_context, "", &p1, &p2, &p3);
			axlog(LOG_CERTIFY, "CertifyFull caNO/cloud sk_if_cert_SignDataWithR rc=%d", rc);

			if (rc) {
				m_slog.Format("[CERTIFY]  sk_if_cert_SignDataWithR 오류 rc = [%d]  errorCode=[%d]", rc, sk_if_GetLastErrorCode());
			
				return sk_if_GetLastErrorCode();
			}
			else
			{
				CopyMemory((void*)pOutB, p2.value, p2.length);
				*(int*)pOutL = p2.length;
				//sk_if_cert_MemFree(p2.value);//추후에 해줘야 할수도

				if (m_ca == caNO)
					m_ca = caOKx;
				m_calogon = true;
				savePasswd();

				memcpy(&m_cpContext, (APP_CONTEXT * )&m_context, sizeof(APP_CONTEXT));
				memcpy(&m_cpContextNew, (SD_API_CONTEXT_NEW*)&Context.sd, sizeof(SD_API_CONTEXT_NEW));
				sk_if_cert_preset_context(&m_cpContext, &m_cpContextNew.sd);
			}

			memcpy(&m_contextNew, &Context, sizeof(SD_API_CONTEXT_NEW));
			axlog(LOG_CERTIFY, "CertifyFull caNO/cloud EXIT OK m_ca=%d", (int)m_ca);
			return 0;
		}
		break;
		case caRUN:
			pswdL = sizeof(m_context.pInterfaceContext->szOldPasswd);
			ZeroMemory(m_context.pInterfaceContext->szOldPasswd, pswdL);
			axlog(LOG_CERTIFY, "CertifyFull caRUN/cloud -> Cloude_Full_sign()");
			return Cloude_Full_sign(pOutB, pOutL);
			break;
		default:
			axlog(LOG_CERTIFY, "CertifyFull cloud default (unhandled m_ca=%d)", (int)m_ca);
			OnFire(FEV_CA, MAKELONG(guideCA, AE_ECERTIFY), 0);
			return -1;
		}
	}
	else  //공동인증서
	{
		UString src, des;
		char	encpass[32 + 1];
		int	pswdL, rc = 0;
		CWnd* pWnd;
		BOOL	success = FALSE;

		*(int*)pOutL = 0;
		ZeroMemory(encpass, sizeof(encpass));
		switch (m_ca)
		{
		case caNO:
			ZeroMemory(&m_context, sizeof(APP_CONTEXT));
			ZeroMemory(&m_contextNew, sizeof(SD_API_CONTEXT_NEW));
			if (sk_if_cert_InitContextApp(&m_context, NULL, 0) == -1)
			{
				rc = sk_if_GetLastErrorCode();
				MessageBox(_T("공인인증 사용시 오류가 발생할 수 있사오니 고객센터(1588-0030, 1544-0050)로 문의바랍니다."),
					_T("공인인증 초기화 오류"), MB_OKCANCEL | MB_SYSTEMMODAL | MB_ICONINFORMATION);
				return rc;
			}

			m_name = _T("");
			pWnd = AfxGetMainWnd();
			sk_if_SetWrongPasswordLimit(1);
			m_contextNew.sd.bOldStorage = 0x00;
			m_contextNew.sd.bForceAllCaSearchMode = 1;
			strcpy_s(m_contextNew.sd.szUserId, E_BUFLEN, (char*)m_name.GetString());
			ZeroMemory(m_contextNew.sd.szOldPasswd, sizeof(m_contextNew.sd.szOldPasswd));
			sk_if_SetKeySaferMode(1);  //9 안내받음  1기존 (IBKs 내부방화벽에서 9번관련 막힌다함..)
			if (pWnd && IsWindow(pWnd->GetSafeHwnd()))
				sk_if_DialogModalMode(pWnd->GetSafeHwnd());
			success = sk_if_CertSetSelectExt(&m_contextNew, CONTEXT_SELECT2, SEARCH_ALLMEDIA);
			axlog(LOG_CERTIFY, "CertifyFull caNO/local sk_if_CertSetSelectExt success=%d", success);
			if (!success)
			{
				rc = sk_if_GetLastErrorCode();
				axlog(LOG_CERTIFY, "CertifyFull caNO/local errorCode=%d", rc);
				switch (rc)
				{
				case 2501:		// 취소
					sk_if_cert_static_context_release();
					*(int*)pOutL = -3;
					break;
				case 1001:		// 인증서 서명오류 (5회)
				case 2417:
					*(int*)pOutL = -4;
					break;

				default:
					*(int*)pOutL = -2;
					break;
				}
				return rc;
			}

			if (sk_if_cert_preset_context(&m_context, &m_contextNew.sd) < 0)
			{
				rc = sk_if_GetLastErrorCode();
				MessageBox(sk_if_GetLastErrorMsg(), _T("공인인증-Preset 실패"), MB_OK | MB_SYSTEMMODAL | MB_ICONINFORMATION);
				*(int*)pOutL = -2;
				return rc;
			}
			break;
		case caRUN:
			pswdL = sizeof(m_context.pInterfaceContext->szOldPasswd);
			ZeroMemory(m_context.pInterfaceContext->szOldPasswd, pswdL);
			break;
		default:
			axlog(LOG_CERTIFY, "CertifyFull local default (unhandled m_ca=%d)", (int)m_ca);
			OnFire(FEV_CA, MAKELONG(guideCA, AE_ECERTIFY), 0);
			return -1;
		}

		m_sync.Lock();

		src.length = pInL;
		src.value = (unsigned char*)pInB;
		if (sk_if_cert_SignData(&m_context, encpass, &src, &des))  //이체등 공동인증서 입력
		{
			m_sync.Unlock();
			rc = sk_if_GetLastErrorCode();
			axlog(LOG_CERTIFY, "CertifyFull local sk_if_cert_SignData FAILED rc=%d", rc);
			switch (rc)
			{
			case 1000:	// 취소
				*(int*)pOutL = -3;
				break;
			case 2417:	// 패스워드 오류
				*(int*)pOutL = -4;
				break;
			default:
				*(int*)pOutL = -2;
				break;
			}

			return rc;
		}

		*(int*)pOutL = des.length;
		CopyMemory((void*)pOutB, des.value, des.length);
		sk_if_cert_MemFree(des.value);
		m_sync.Unlock();

		if (m_ca == caNO)
			m_ca = caOKx;
		m_calogon = true;
		savePasswd();

		axlog(LOG_CERTIFY, "CertifyFull local EXIT OK m_ca=%d signedLen=%ld", (int)m_ca, *(int*)pOutL);
		return 0;
	}
}

long CCertifyCtrl::CertifyName(long pBytes)
{
	int	rc = 0;

	rc = strlen(m_contextNew.sd.szDN);
	CopyMemory((char*)pBytes, m_contextNew.sd.szDN, rc);
	axlog(LOG_CERTIFY, "CertifyName dnLen=%d", rc);
	return rc;
}

BOOL CCertifyCtrl::CheckCloude()
{
	char chfile[500]{};
	GetModuleFileName(nullptr, chfile, 260);

	CString spath, stmp;
	spath.Format("%s", chfile);
	spath.TrimRight();

	int iFind = spath.Find("exe");
	spath = spath.Left(iFind);
	m_root = spath;
	spath += "tab\\DEV_CLOUDE.ini";

	CFileFind   finder;
	if (finder.FindFile(spath))
		m_bDev = TRUE;
	else
		m_bDev = FALSE;

	m_slog.Format("[CERTIFY] CheckCloude  m_bDev = [%d]  ", m_bDev);
	FileLog(m_slog);

	/*memset(chfile, 0x00, 500);
	GetPrivateProfileString("CLOUDELOGIN", "USE", "0", chfile, sizeof(chfile), spath);

	stmp.Format("%s", chfile);
	stmp.TrimRight();

	if (stmp == "1")
		return TRUE;
	else
		return FALSE;*/

	axlog(LOG_CERTIFY, "CheckCloude EXIT always-TRUE (ini-based branch commented out) m_bDev=%d", m_bDev);
	return TRUE;
}

int CCertifyCtrl::Cloude_Full_sign(long pOutB, long pOutL)
{
	SD_API_CONTEXT_NEW tContext; //구조체 선언(클라우드 인증서 정보를 담을)
	memset(&tContext, 0x00, sizeof(SD_API_CONTEXT_NEW));
	int rc = 0;
	axlog(LOG_CERTIFY, "Cloude_Full_sign ENTER bOldStorage=%d", m_contextNew.sd.bOldStorage);

	CString plain, strResult;
	UString p1, p2, p3;

	//간편비밀번호를 다시 입력 받고 싶다면
	if (m_contextNew.sd.bOldStorage == 7)
	{
		memcpy(&tContext, &m_contextNew, sizeof(SD_API_CONTEXT_NEW));
		memset(&tContext.sd.szOldPasswd, 0x00, sizeof(tContext.sd.szOldPasswd)); //비밀번호 부분만 초기화

		rc = sk_if_CloudCertSetSelectExt(&tContext, 1);
		axlog(LOG_CERTIFY, "Cloude_Full_sign re-select sk_if_CloudCertSetSelectExt rc=%d", rc);

		if (rc == 0)
		{
			memset(&m_contextNew, 0x00, sizeof(SD_API_CONTEXT_NEW));
			memcpy(&m_contextNew, &tContext, sizeof(SD_API_CONTEXT_NEW));
			sk_if_cert_preset_context(&m_context, &m_contextNew.sd);

		}
		else
		{
			int errorCode = sk_if_GetLastErrorCode();

			if (errorCode == 2501)   //일부러 취소했을때
				*(int*)pOutL = -3;
			else
				*(int*)pOutL = -2;
			return -1;
		}
		memset(&tContext, 0x00, sizeof(SD_API_CONTEXT_NEW));
	}

	plain = "abcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()-=";
	p1.value = (unsigned char*)LPCTSTR(plain);
	p1.length = plain.GetLength();

	//p1 = 원문 , p2 결과값 , p3 = R값 결과
	memset(&p3, 0x00, sizeof(UString));
	rc = sk_if_cert_SignDataWithR(&m_context, "", &p1, &p2, &p3);
	axlog(LOG_CERTIFY, "Cloude_Full_sign sk_if_cert_SignDataWithR rc=%d", rc);

	if (rc) {
		m_slog.Format("[CERTIFY]  전자서명 오류 sk_if_cert_SignDataWithR rc = [%d]  errorCode=[%d]", rc, sk_if_GetLastErrorCode());
		FileLog(m_slog);
		CString emsg;
		emsg = sk_if_GetLastErrorMsg();
		int iret = sk_if_GetLastErrorCode();
		*(int*)pOutL = -2;
		return iret;
	}
	else
	{
 		CopyMemory((void*)pOutB, p2.value, p2.length);
		*(int*)pOutL = p2.length;
		//sk_if_cert_MemFree(p2.value); //추후에 해줘야 할수도
		if (m_ca == caNO)
			m_ca = caOKx;
		m_calogon = true;
		savePasswd();

		axlog(LOG_CERTIFY, "Cloude_Full_sign EXIT OK m_ca=%d signedLen=%d", (int)m_ca, p2.length);
		return 1;
	}
}

int CCertifyCtrl::Cloude_ConTraction_sign(long pOutB, long pOutL)
{
	int rc = 0;
	axlog(LOG_CERTIFY, "Cloude_ConTraction_sign ENTER bOldStorage=%d", m_contextNew.sd.bOldStorage);
	CString plain, strResult;
	UString p1, p2, p3;

	plain = "abcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()-=";

	p1.value = (unsigned char*)LPCTSTR(plain);
	p1.length = plain.GetLength();

	//p1 = 원문 , p2 결과값 , p3 = R값 결과
	memset(&p2, 0x00, sizeof(UString));
	memset(&p3, 0x00, sizeof(UString));

	//클라우드 저장소에 저장된 인증서인데 임시 비밀번호가 없다면
	//다시 받는다
	if (m_contextNew.sd.bOldStorage == 7 &&
		(m_contextNew.sd.szOldPasswd[0] == NULL ||
			strlen(m_contextNew.sd.szOldPasswd) <= 0))
	{
		SD_API_CONTEXT_NEW tContext; //구조체 선언(클라우드 인증서 정보를 담을)
		memset(&tContext, 0x00, sizeof(SD_API_CONTEXT_NEW));

		memcpy(&tContext, &m_contextNew, sizeof(SD_API_CONTEXT_NEW));
		memset(&tContext.sd.szOldPasswd, 0x00, sizeof(tContext.sd.szOldPasswd));

		rc = sk_if_CloudCertSetSelectExt(&tContext, 1);
		axlog(LOG_CERTIFY, "Cloude_ConTraction_sign re-select sk_if_CloudCertSetSelectExt rc=%d", rc);
		if (rc == 0)
		{
			memset(&m_contextNew, 0x00, sizeof(SD_API_CONTEXT_NEW));
			memcpy(&m_contextNew, &tContext, sizeof(SD_API_CONTEXT_NEW));
			sk_if_cert_preset_context(&m_context, &m_contextNew.sd);
		}
		else
		{
			int errorCode = sk_if_GetLastErrorCode();
			m_slog.Format("[CERTIFY]  축약서명 오류 sk_if_CloudCertSetSelectExt rc = [%d]  errorCode=[%d]", rc, sk_if_GetLastErrorCode());
			FileLog(m_slog);
			m_sync.Unlock();
			return  -2;
		}

		memset(&tContext, 0x00, sizeof(SD_API_CONTEXT_NEW));
	}

	m_sync.Lock();
	m_nBytes = *(int*)pOutL;
	p1.length = m_nBytes;
	p1.value = (unsigned char*)pOutB;

	if (sk_if_cert_SignData_notEncode(&m_cpContext, "", &p1, &p2, NULL))
	{
		m_slog.Format("[CERTIFY] 축약서명 오류 Ecod =[%s]", sk_if_GetLastErrorMsg());
		FileLog(m_slog);
		axlog(LOG_CERTIFY, "Cloude_ConTraction_sign sk_if_cert_SignData_notEncode FAILED");
		m_sync.Unlock();
		return  -2;
	}
	else
	{
		pOutB += m_nBytes;
		m_nBytes += p2.length;

		*(int*)pOutL = m_nBytes;
		CopyMemory((void*)pOutB, p2.value, p2.length);
		sk_if_cert_MemFree(p2.value);
		m_sync.Unlock();
		axlog(LOG_CERTIFY, "Cloude_ConTraction_sign EXIT OK signedLen=%d", m_nBytes);
		return 1;
	}
	return 0;
}

LONG CCertifyCtrl::CertifyCloud(LONG func)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
	//m_bCloudeUse = CheckCloude();
	axlog(LOG_CERTIFY, "CertifyCloud ENTER func=%ld", func);
	CheckCloude();
	InitCloude();

	int rc = 0;
	switch (func)
	{
		case 1:  //인증서 올리기
		{
			SD_API_CONTEXT_NEW Context;
			memset(&Context, 0x00, sizeof(Context));

			rc = sk_if_UploadPCtoCloud(&Context, 0);
			if (rc == 0)
				return rc;
			else
			{
				m_slog.Format("[CERTIFY] 클라우드로 인증서 올리기 오류 sk_if_UploadPCtoCloud rc = [%d]  errorCode=[%d] error  msg = [%s]",
					rc, sk_if_GetLastErrorCode(), sk_if_GetLastErrorMsg());
				FileLog(m_slog);
				return sk_if_GetLastErrorCode();
			}
		}
			break;
		case 2:  //인증서 내려받기
		{
			rc = sk_if_DownloadCloudtoPC(0);
			if (rc == 0)
				return rc;
			else
			{
				m_slog.Format("[CERTIFY] 클라우드에서 인증서리 내려받기 오류 sk_if_DownloadCloudtoPC rc = [%d]  errorCode=[%d] error  msg = [%s]",
					rc, sk_if_GetLastErrorCode(), sk_if_GetLastErrorMsg());
				FileLog(m_slog);
				return sk_if_GetLastErrorCode();
			}
		}
			break;
		case 3:  //간편비밀번호 변경
		{
			SD_API_CONTEXT_NEW Context;
			memset(&Context, 0x00, sizeof(Context));

			rc = sk_if_CertChangePin_inCloud(&Context, 0);
			if (rc == 0)
				return rc;
			else
			{
				m_slog.Format("[CERTIFY] 간편비밀번호 변경 오류 sk_if_CertChangePin_inCloud rc = [%d]  errorCode=[%d] error  msg = [%s]",
					rc, sk_if_GetLastErrorCode(), sk_if_GetLastErrorMsg());
				FileLog(m_slog);
				return sk_if_GetLastErrorCode();
			}
		}
		break;
		case 4:  //인증서 발급
		{
			SD_API_CONTEXT_NEW Context;
			memset(&Context, 0x00, sizeof(Context));

			int rc = 0;
			rc = sk_if_IssueCert_toCloud(&Context, 0);
			if (rc == 0)
				FileLog("[CERTIFY] 클라우드 인증서 발급");
			else
			{
				m_slog.Format("[CERTIFY] 클라우드 인증서 발급 오류 sk_if_IssueCert_toCloud rc = [%d]  errorCode=[%d] error  msg = [%s]",
					rc, sk_if_GetLastErrorCode(), sk_if_GetLastErrorMsg());
				FileLog(m_slog);
				return sk_if_GetLastErrorCode();
			}
			return 0;
		}
		break;
		case 5:  //인증서 갱신
		{
			SD_API_CONTEXT_NEW Context;
			memset(&Context, 0x00, sizeof(Context));

			int rc = 0;
			rc = sk_if_CertNew_toCloud(&Context, 0);
			if (rc == 0)
				FileLog("[CERTIFY] 클라우드 인증서 갱신");
			else
			{
				m_slog.Format("[CERTIFY] 클라우드 인증서 갱신 오류 sk_if_CertNew_toCloud rc = [%d]  errorCode=[%d] error  msg = [%s]",
					rc, sk_if_GetLastErrorCode(), sk_if_GetLastErrorMsg());
				FileLog(m_slog);
				return sk_if_GetLastErrorCode();
			}

			return 0;
		}
		break;
		case 6:  //인증서 삭제
		{
			int rc = 0;
			rc = sk_if_DeleteCert_inCloud(0);
			if (rc == 0)
				FileLog("[CERTIFY] 클라우드 인증서 삭제");
			else
			{
				m_slog.Format("[CERTIFY] 클라우드 인증서 삭제 오류 sk_if_DeleteCert_inCloud rc = [%d]  errorCode=[%d] error  msg = [%s]",
					rc, sk_if_GetLastErrorCode(), sk_if_GetLastErrorMsg());
				FileLog(m_slog);
				return sk_if_GetLastErrorCode();
			}
				
			return 0;
		}
		break;
		case 7: //연결확인
		{
			int rc = 0;
			rc = sk_if_Connected_CloudUser_Confirm(0); //1 : 화면을 보지않고 연결확인 된 것만 알겠다
			if (rc == 1)
				FileLog("[CERTIFY] 클라우드 연결 끊기 성공");

			if (rc < 0)
			{
				{
					m_slog.Format("[CERTIFY] 클라우드에 연결된 정보 확인 rc = [%d]  errorCode=[%d] error  msg = [%s]",
						rc, sk_if_GetLastErrorCode(), sk_if_GetLastErrorMsg());
					FileLog(m_slog);
					return sk_if_GetLastErrorCode();
				}
			}
			FileLog("[CERTIFY] 클라우드에 연결된 정보 확인");
	
			return 0;
		}
		break;
		case 8: //자동연결된 기기 조회
		{
			int rc = 0;
			rc = sk_if_Cloud_AutoConnected_Device(0);
			return 0;
		}
		break;
		case 9:  //회원탈퇴
		{
			int rc = 0;
			rc = sk_if_CloudUser_DeleteAccount(0);
			return 0;
		}
		break;
		case 10: //설정
		{
			return 0;
		}
		break;
		case 11: //클라우드 사용
		{
			m_bCloudeUse = TRUE;
			axlog(LOG_CERTIFY, "CertifyCloud func=11 -> m_bCloudeUse=TRUE");
			FileLog("[CERTIFY] 클라우드 사용");
			return 0;
		}
		break;
		case 12: //클라우드 미사용
		{
			m_bCloudeUse = FALSE;
			axlog(LOG_CERTIFY, "CertifyCloud func=12 -> m_bCloudeUse=FALSE");
			FileLog("[CERTIFY] 클라우드 미사용");
			return 0;
		}
		break;
		default:
		{
			axlog(LOG_CERTIFY, "CertifyCloud func=%ld unhandled -> 0", func);
			return 0;
		}
		break;
	}
	return 0;
}

void CCertifyCtrl::InitCloude()
{
	m_slog.Format("[CERTIFY]  InitCloude  m_bCloudeInit = [%d]  ", m_bCloudeInit);
	FileLog(m_slog);
	axlog(LOG_CERTIFY, "InitCloude ENTER m_bCloudeInit=%d m_bDev=%d", m_bCloudeInit, m_bDev);

	if (m_bCloudeInit)
	{
		axlog(LOG_CERTIFY, "InitCloude already initialized, skip");
		return;
	}

	CString slog;
	CloudConfig config;
	memset(&config, 0x00, sizeof(CloudConfig));

	config.CUSTOMER_ID = "SS0068";
	if (m_bDev)
	{
		config.SITE_CODE[0] = "U1MwMDY4XzA2";
		config.SERVER_HOST = DEV_CLOUDE_SERVER;
		config.AGREEMENT_URL = DEV_AGREEMENT_URL;
	}
	else
	{
		config.SITE_CODE[0] = "U1MwMDY4X0FYSVNfRDI=";  
											//U1MwMDY4X0FYSVNfRDI=   <-- 코드사인토큰 변경
											//"U1MwMDY4X0FYSVNfRA==";  //기존
		config.SERVER_HOST = REAL_CLOUDE_SERVER;
		config.AGREEMENT_URL = REAL_AGREEMENT_URL;
	}

	slog.Format("[CERTIFY] --InitCloude-- m_bDev=[%d]  ", m_bDev);
	FileLog(slog);

	config.VERSION = "1.0.0";
	config.SERVER_PORT = 8500;

	//타임아웃 기간을 설정합니다 1~10초 (msec 단위)
	config.TIMEOUT_MSEC = 3000;
	sk_if_Cloud_KeyPadUse(1);
	sk_if_Set_CloudConfig(config);
	sk_if_DialogModalMode(m_hWnd); //모달 모드
	int ret = sk_if_SetKeySaferMode(1);  //9 안내받음  1기존 (IBKs 내부방화벽에서 9번관련 막힌다함..)
	
	slog.Format("[CERTIFY] --InitCloude-- sk_if_SetKeySaferMode ret=[%d] ", ret);
	FileLog(slog);

	//해외전화 ARS 인증버튼 0:비활성화 1:활성화
	sk_if_Cloud_AbroadPhoneAuthenticationOnOff(1);

	m_bCloudeInit = TRUE;
	axlog(LOG_CERTIFY, "InitCloude EXIT m_bCloudeInit=TRUE serverHost=%s", m_bDev ? DEV_CLOUDE_SERVER : REAL_CLOUDE_SERVER);
}