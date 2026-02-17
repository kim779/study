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


#pragma	comment(lib, "CaLib/SKComdIF")
#pragma	message("Automatically linking with SKComdIF library")

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

CString CCertifyCtrl::getStatus()
{//enum { caNO, caNOx, caOK, caRUN, caPWD, caPWDa, caOKx } m_ca;
	switch (m_ca)
	{
	case caNO:
		return "caNO";
	case caNOx:
		return "caNOx";
	case caOK:
		return "caOK";
	case caRUN:
		return "caRUN";
	case caPWD:
		return "caPWD";
	case caPWDa:
		return "caPWDa";
	case caOKx:
		return "caOKx";
	default:
		return "";
	}
	return "";
}

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


/*
*자기 자신이 ontimer에서 sendmessage 하거나 내부의 공동인증비번 화면에서  사용됨

checkPass - 1로 값을 보내줬을때 removeCertificate 호출하지만 하는것은 없음
                    0 으로 CPassInput 다이알로그(공동인증비번 입력화면)에서 보내주며 비번인증할때..
rebootAxis - 5회이상 오류나서 모듈 닫으려고 할때 쓴다
encryptPass - 실제로 사용되지는 않음
*/
// CCertifyCtrl 메시지 처리기
LRESULT CCertifyCtrl::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
		case checkPass:   
		{
			if (HIWORD(wParam))	// remove certificate
			{
				m_slog.Format("[certify][CCertifyCtrl::OnMessage] [checkPass] removeCertificate\r\n");
				OutputDebugString(m_slog);
				removeCertificate();
			}
			else
			{
				m_slog.Format("[certify][CCertifyCtrl::OnMessage] [checkPass] checkPasswd\r\n");
				OutputDebugString(m_slog);
				return checkPasswd(CString((char*)lParam));
			}
		}
		case rebootAxis:
		{
			m_slog.Format("[certify][CCertifyCtrl::OnMessage] [rebootAxis]\r\n");
			OutputDebugString(m_slog);
			OnFire(FEV_CA, MAKELONG(closeCA, HIWORD(wParam)), 0);
		}
			break;
		case encryptPass:
		{
			m_slog.Format("[certify][CCertifyCtrl::OnMessage] [encryptPass]\r\n");
			OutputDebugString(m_slog);
			OnFire(FEV_CA, MAKELONG(encryptCA, HIWORD(wParam)), lParam);
			return (long)lParam;
		}
	}
	return 0;
}

/*
1. CWizardCtrl::OnSign(int signK, char* pBytes, int nBytes)
1. CWizardCtrl::OnXecure(int encK, char* pBytes, int nBytes)
2.m_certify->InvokeHelper(DI_ONCA,..)  
위 방식으로 타고 온다. 

pBytes = nullptr로 해서 맨처음 시작할때 초기화 목적으로 보내거나 
로그인 하면서 인증해달라고 넘어온다.

암호화(xecure) m_mode == mtFlag::mtCA || m_mode == mtFlag::mtRUN 
과정에서도 넘어온다

*/
long CCertifyCtrl::OnCertify(long pBytes, long nBytes)
{
	m_slog.Format("[certify][CCertifyCtrl::OnCertify]  m_ca=[%s] nBytes=[%d]  pBytes=[%s]\r\n", getStatus(), *(int*)nBytes, (char*)pBytes);
	OutputDebugString(m_slog);

	if (pBytes == NULL)
	{
		m_ca = caNO;
		m_name = _T("cn=");
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
	switch (m_ca)
	{
	case caPWDa:
		retv--;
		m_msg = CString((char*)pBytes, caL);
		SetTimer(TM_MSG, 200, NULL);
		m_ca = caRUN;
		*(int*)nBytes = retv;
		return NULL;

	case caNO:
		ZeroMemory(&m_context, sizeof(APP_CONTEXT));
		ZeroMemory(&m_contextNew, sizeof(SD_API_CONTEXT_NEW));
		if (caL <= 0)
			return NULL;
		if (sk_if_cert_InitContextApp(&m_context, NULL, 0) == -1)
		{
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
		if (m_ca == caPWD)
		{
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

			char* pdata = new char[2];
			memset(pdata, 0x00, 2);
			memcpy(pdata, (char*)&pwdR->pwdn[0], 1);
			CString sdata;
			sdata.Format("%s", pdata); sdata.TrimRight();
			idx = atoi(sdata);

		//	idx = atoi((const char*)pwdR->pwdn[0]); // CString(pwdR->pwdn, sizeof(pwdR->pwdn)) );
			

			CCountPass countDlg(idx, retry);
			switch (countDlg.DoModal())
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
			sk_if_cert_static_context_release();
			return NULL;
		}
		m_ca = caOK;
		break;

	case caOK:
		m_ca = caRUN;
		*(int*)nBytes = 0;

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
		return NULL;

	default:
		return NULL;
	}

	*(int*)nBytes = m_nBytes;
	return (long)m_pBytes;
}

/*
BOOL CGuard::Certify(char* pBytes, int& nBytes, CString maps)
실제로 맵등 화면에서 공동인증을 하려거나 할때 사용된다.
*/
BOOL CCertifyCtrl::Certify(long pBytes, long nBytes, long infos)
{
	m_slog.Format("[certify][CCertifyCtrl::Certify]  m_ca=[%s]  pBytes=[%s] infos=[%s] \r\n", getStatus(), (unsigned char*)pBytes, CString((char*)infos, L_MAPN));
	OutputDebugString(m_slog);

	switch (m_ca)
	{
	case caNO:
		return TRUE;
	case caRUN:
		break;
	default:
		OnFire(FEV_CA, MAKELONG(guideCA, AE_ECERTIFY), 0);
		return FALSE;
	}

	char	encpass[32 + 1];
	UString	src, des;

	ZeroMemory(encpass, sizeof(encpass));
	CString maps = CString((char*)infos, L_MAPN);
	if (m_auto && !isMustCertify(maps))  //로그인시 공동인증자동 체크 
		CopyMemory(encpass, m_encpass, sizeof(encpass));
	else
	{
		CString	pass;
		pass = checkPasswd();   //주문낼때 여기 타고 온다
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
	BOOL bCloude = CheckCloude();
	if (bCloude)
	{
		int iret = Cloude_ConTraction_sign(pBytes, nBytes);
		return  iret == 1 ? TRUE : FALSE;
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
		return TRUE;
	}
}
/*
void CWizardCtrl::OnXecure(int encK, char* pBytes, int nBytes)
m_guard->CertifyErr(pBytes, nBytes)
m_certify->InvokeHelper(DI_CAERR, DISPATCH_METHOD, VT_BOOL, (void*)&retv,
						(BYTE *)(VTS_I4 VTS_I4), pBytes, nBytes);
*/
BOOL CCertifyCtrl::CertifyErr(long pBytes, long nBytes)
{
	m_slog.Format("[certify][CCertifyCtrl::CertifyErr]  m_ca=[%s]  nBytes=[%d] pBytes=[%s] \r\n", getStatus(), (char*)pBytes);
	OutputDebugString(m_slog);

	enum { eNONE = 0, eHTS = 1, eSIGN = 2 } eKind;

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

	TRACE("[%c][%d][%s]\n", eKind == eHTS ? 'H' : 'S', eCodeX, string);
	return FALSE;
}

// id(12) + pass(10) + info(10) + cpass(30) + ip(15) + mac(16)
// info[1] : 주문자동서명, info[2] : 공인인증사용
/*
CWizardCtrl::RunAxis(long mode, long pBytes, long nBytes)
switch (mode)
	{
	....
	case signUSER:
	case signUSERc:
		switch (m_mode)
		{
		case mtFlag::mtCON:
			break;
		case mtFlag::mtRUN:
			m_guard->CertifyId((char*)pBytes, true);
			return TRUE;
		case mtFlag::mtNO:
		default:
			return TRUE;
	}
	m_guard->CertifyId((char*)pBytes);
	if (m_guard->Login(mode, (char*)pBytes, nBytes, m_xtype == xtFlag::xtXEC))
.....
	m_certify->InvokeHelper(DI_CAID, DISPATCH_METHOD, VT_EMPTY, NULL, (BYTE *)(VTS_I4), pBytes);
*/
void CCertifyCtrl::CertifyId(long pBytes)
{
	OutputDebugString("[certify][CCertifyCtrl::CertifyId] -------------------------------------------------------------\r\n");
	m_slog.Format("[certify][CCertifyCtrl::CertifyId]  m_ca=[%s]  pBytes=[%62s] \r\n", getStatus(), (char*)pBytes);
	OutputDebugString(m_slog);

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

	m_slog.Format("[certify][CCertifyCtrl::CertifyId] m_calogon =[%d]  m_user=[%s]  info=[%s]  pass=[%s]\r\n", m_calogon, m_user, info, pass);
	OutputDebugString(m_slog);

	if (!m_calogon)
	{
		sk_if_SetPasswordEncMode(1);
		sk_if_GetEncryptedPassword((char*)pass.operator LPCTSTR(), m_encpass);

		m_slog.Format("[certify][CCertifyCtrl::CertifyId] m_encpass=[%s]\r\n", m_encpass);
		OutputDebugString(m_slog);

		pass = _T("");
	}
	m_calogon = false;

	m_auto = (info.GetAt(1) == '1') ? true : false;
	m_certifys.Format(_T("%s"), m_user.GetString());
	//	m_certifys.Format("%s(%s)=%s", m_user, "사용자", m_auto ? yAuto : nAuto);
}

/*
void CWizardCtrl::OnSign(int signK, char* pBytes, int nBytes)
...
if (!m_guard->Certify(FALSE, sign->flag & flagCA, sign->flag & flagCAX, sign->flag & flagXCS))
...
BOOL CGuard::Certify(BOOL force, BOOL certify, BOOL xcertify, BOOL xserver)
..
if (certify && xcertify && !xserver)
		{
			m_certify->InvokeHelper(DI_CAEX, DISPATCH_METHOD, VT_BOOL, (void*)&retv, (BYTE *)(VTS_I4 VTS_I4), NULL, NULL);

이거 개발기는 공동인증 안할때 여기로 온다.
*/
BOOL CCertifyCtrl::CertifyEx(long pBytes, long nBytes)
{
	m_slog.Format("[certify][CCertifyCtrl::CertifyEx]  m_ca=[%s] pBytes=[%s] \r\n", getStatus(), (char*)pBytes);
	OutputDebugString(m_slog);

	if (pBytes == NULL)
	{
		if (MessageBox(_T("공인인증 절차를 수행할 수 없습니다.\r\n공인인증을 사용하지 않고 진행을 계속하시겠습니까?"),
			_T("공인인증 미사용 동의 확인"), MB_OKCANCEL | MB_SYSTEMMODAL | MB_ICONINFORMATION) == IDOK)
			return TRUE;

		OnFire(FEV_CA, MAKELONG(closeCA, true), 0);	// terminate AXIS
		return FALSE;
	}

	return TRUE;
}

//클라우드 할때도 여기 타는지 확인한다.
bool CCertifyCtrl::sign()
{
	m_slog.Format("[certify][CCertifyCtrl::sign]  m_ca=[%s]  [%d] m_certifys=[%s]\r\n", getStatus(), m_certifys.GetLength(), m_certifys);
	OutputDebugString(m_slog);

	UString src, des;

	src.length = m_certifys.GetLength();
	src.value = (unsigned char*)m_certifys.operator LPCTSTR();
	if (sk_if_cert_SignData(&m_context, NULL, &src, &des))
		return false;

	CString	keys = _T("key\t");
	m_nBytes = keys.GetLength();
	CopyMemory(m_pBytes, keys, m_nBytes);

	CopyMemory(&m_pBytes[m_nBytes], des.value, des.length);
	m_nBytes += des.length;

	sk_if_cert_MemFree(des.value);
	return true;
}
/*
ID로그인 할때
OnCertify 에서  m_ca 가 case caNO:  일때 여기를 온다.  dn값을 알고 있다.
certifyID 하고 온다. 인증처리에 대한 결과를 dn값을 통해서 가져오는거 같다. 성공 실패등 알고 있다.

*/
int CCertifyCtrl::queryDn(CString dn_name, int* nBytes, bool retry)
{
	m_slog.Format("[certify][CCertifyCtrl::queryDn]  m_ca=[%s] dn_name=[%s] \r\n", getStatus(), dn_name);
	OutputDebugString(m_slog);

	CWnd* pWnd;
	BOOL	success = FALSE;

	pWnd = AfxGetMainWnd();
	sk_if_SetWrongPasswordLimit(1);
	m_contextNew.sd.bOldStorage = 0x00;
	m_contextNew.sd.bForceAllCaSearchMode = 1;
	strcpy_s(m_contextNew.sd.szUserId, E_BUFLEN, (char*)dn_name.GetString());
	ZeroMemory(m_contextNew.sd.szOldPasswd, sizeof(m_contextNew.sd.szOldPasswd));
	sk_if_SetKeySaferMode(1);
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

	if (!success)  
	{//ID 로그인 방식에서 공동인증서 검증 할때..  인증서 비번오류상황
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
		MessageBox(sk_if_GetLastErrorMsg(), _T("공인인증-Preset 실패"), MB_OK | MB_SYSTEMMODAL | MB_ICONINFORMATION);
		return rspNULL;
	}
	return rspOK;
}

void CCertifyCtrl::savePasswd()
{
	m_slog.Format("[certify][CCertifyCtrl::savePasswd]  m_ca=[%s] m_encpass=[%s] \r\n", getStatus(), m_encpass);
	OutputDebugString(m_slog);

	if (m_context.pInterfaceContext == NULL)
		return;

	CString	pass;

	pass = CString(m_context.pInterfaceContext->szOldPasswd, sizeof(m_context.pInterfaceContext->szOldPasswd));
	sk_if_GetEncryptedPassword((char*)pass.operator LPCTSTR(), m_encpass);

m_slog.Format("[certify][CCertifyCtrl::savePasswd]  pass[%d]=[%s] \r\n", pass.GetLength() , pass);
OutputDebugString(m_slog);
m_slog.Format("[certify][CCertifyCtrl::savePasswd]  m_encpass[%d]=[%s] \r\n", strlen(m_encpass), m_encpass);
OutputDebugString(m_slog);

	pass = _T("");
}

BOOL CCertifyCtrl::checkPasswd(CString pass)
{
	m_slog.Format("[certify][CCertifyCtrl::checkPasswd]  m_ca=[%s] pass =[%s]  \r\n", getStatus(), pass);
	OutputDebugString(m_slog);

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
		if (sk_if_GetLastErrorCode() == 2417)
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
	sk_if_SetKeySaferMode(1);
	if (sk_if_cert_SignData_notEncode(&m_context, encpass, &src, &des, NULL))
	{
		if (sk_if_GetLastErrorCode() == 2417)
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

	m_slog.Format("[certify][CCertifyCtrl::checkPasswd]  pass=[%s] \r\n", pass);
	OutputDebugString(m_slog);

	return pass;
}

bool CCertifyCtrl::certify(bool reissue)
{
	m_slog.Format("[certify][CCertifyCtrl::certify]  m_ca=[%s] reissue=[%d]\r\n", getStatus(), reissue);
	OutputDebugString(m_slog);

	m_string = _T("http://www.ibks.com/LoadService.jsp?url=/customer/certificate/newissue.jsp");
	OnFire(FEV_CA, MAKELONG(htmlCA, 0), (long)(char*)m_string.operator LPCTSTR());
	return true;
}

void CCertifyCtrl::otherCertificate()
{
	m_slog.Format("[certify][CCertifyCtrl::otherCertificate]  m_ca=[%s] \r\n", getStatus());
	OutputDebugString(m_slog);

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
	m_slog.Format("[certify][CCertifyCtrl::guideMsg]  m_ca=[%s] msgno=[%s] guide=[%s] title=[%s]\r\n", getStatus(), msgno, guide, title);
	OutputDebugString(m_slog);

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
	m_slog.Format("[certify][CCertifyCtrl::isMustCertify]  m_ca=[%s] maps=[%s] \r\n", getStatus(), maps);
	OutputDebugString(m_slog);

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
	BOOL bCloude = CheckCloude();
	m_slog.Format("[certify][CCertifyCtrl::CertifyFull]  m_ca=[%s] pInL=[%d] bCloude=[%d]\r\n", getStatus(), pInL, bCloude);
	OutputDebugString(m_slog);
	//bCloude = false; 
	if (bCloude)   //클라우드 로그인
	{
//--------------------------------------------------------------------------------------------------------------------------------
//클라우드 초기화
		int	pswdL, rc = 0;
		switch (m_ca)
		{
			case caNO:
			{
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
					config.SITE_CODE[0] = "U1MwMDY4X0FYSVNfRA==";
					config.SERVER_HOST = REAL_CLOUDE_SERVER;
					config.AGREEMENT_URL = REAL_AGREEMENT_URL;
				}

				config.VERSION = "1.0.0";
				config.SERVER_PORT = 8500;

				//타임아웃 기간을 설정합니다 1~10초 (msec 단위)
				config.TIMEOUT_MSEC = 3000;

				sk_if_Set_CloudConfig(config);
				sk_if_DialogModalMode(m_hWnd); //모달 모드

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
				if (rc == 0)
				{
					//MessageBox("성공", "클라우드 인증서 선택", MB_OK);
				}
				else
				{
			
					memset(m_contextNew.sd.szDN, 0x00, sizeof(m_contextNew.sd.szDN));
					if (rc == -2) //-2면 연결끊기를 한것이다.
					{
						*(int*)pOutL = -3;
						return 2501;
					}
				
					int errorCode = sk_if_GetLastErrorCode();
					*(int*)pOutL = -3;
					
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

				if (rc) {
					//MessageBox(sk_if_GetLastErrorMsg(), "전자서명 오류", MB_OK);
					return sk_if_GetLastErrorCode();
				}
				else
				{
					//	MessageBox("로그인 성공", "전자 서명", MB_OK);
					CopyMemory((void*)pOutB, p2.value, p2.length);
					*(int*)pOutL = p2.length;

					if (m_ca == caNO)
						m_ca = caOKx;
					m_calogon = true;
					savePasswd();

					memcpy(&m_appContext, (APP_CONTEXT*)&m_context, sizeof(APP_CONTEXT));
					memcpy(&m_SDAPIContext, (SD_API_CONTEXT_NEW*)&Context.sd, sizeof(SD_API_CONTEXT_NEW));
					sk_if_cert_preset_context(&m_appContext, &m_SDAPIContext.sd);
				}

				memcpy(&m_contextNew, &Context, sizeof(SD_API_CONTEXT_NEW));
			}
			break;
		case caRUN:
			pswdL = sizeof(m_context.pInterfaceContext->szOldPasswd);
			ZeroMemory(m_context.pInterfaceContext->szOldPasswd, pswdL);
			return Cloude_Full_sign( pOutB,  pOutL);
			break;
		default:
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
			sk_if_SetKeySaferMode(1);
			if (pWnd && IsWindow(pWnd->GetSafeHwnd()))
				sk_if_DialogModalMode(pWnd->GetSafeHwnd());
			success = sk_if_CertSetSelectExt(&m_contextNew, CONTEXT_SELECT2, SEARCH_ALLMEDIA);
			if (!success)
			{
				rc = sk_if_GetLastErrorCode();
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

		return 0;
	}
}
//저장되있는 DN값을 내리는 것이다 
long CCertifyCtrl::CertifyName(long pBytes)
{
	m_slog.Format("[certify][CCertifyCtrl::CertifyName]  m_ca=[%s] pBytes=[%s] \r\n", getStatus(), pBytes);
	OutputDebugString(m_slog);

	int	rc = 0;

	rc = strlen(m_contextNew.sd.szDN);
	CopyMemory((char*)pBytes, m_contextNew.sd.szDN, rc);
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
	spath += "tab\\axis.ini";

	int readL;
	memset(chfile, 0x00, 500);

	GetPrivateProfileString("MODE", "DEV", "0", chfile, sizeof(chfile), spath);
	stmp.Format("%s", chfile);
	stmp.TrimRight();
	if (stmp == "1")
		m_bDev = TRUE;
	else
		m_bDev = FALSE;

	memset(chfile, 0x00, 500);
	GetPrivateProfileString("CLOUDELOGIN", "USE", "0", chfile, sizeof(chfile), spath);

	stmp.Format("%s", chfile);
	stmp.TrimRight();

	if (stmp == "1")
		return TRUE;
	else
		return FALSE;
}

int CCertifyCtrl::Cloude_Full_sign(long pOutB, long pOutL)
{
	SD_API_CONTEXT_NEW tContext; //구조체 선언(클라우드 인증서 정보를 담을)
	memset(&tContext, 0x00, sizeof(SD_API_CONTEXT_NEW));
	int rc = 0;

	CString plain, strResult;
	UString p1, p2, p3;

	//간편비밀번호를 다시 입력 받고 싶다면
	if (m_contextNew.sd.bOldStorage == 7)
	{
		memcpy(&tContext, &m_contextNew, sizeof(SD_API_CONTEXT_NEW));
		memset(&tContext.sd.szOldPasswd, 0x00, sizeof(tContext.sd.szOldPasswd)); //비밀번호 부분만 초기화

		rc = sk_if_CloudCertSetSelectExt(&tContext, 1);

		if (rc == 0)
		{
			memset(&m_contextNew, 0x00, sizeof(SD_API_CONTEXT_NEW));
			memcpy(&m_contextNew, &tContext, sizeof(SD_API_CONTEXT_NEW));
			sk_if_cert_preset_context(&m_context, &m_contextNew.sd);

		}
		else
		{
			int errorCode = sk_if_GetLastErrorCode();
			MessageBox(sk_if_GetLastErrorMsg(), "전자 서명 오류", MB_OK);
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

	if (rc) {
		MessageBox(sk_if_GetLastErrorMsg(), "전자서명 오류", MB_OK);
		CString emsg;
		emsg = sk_if_GetLastErrorMsg();
		int iret = sk_if_GetLastErrorCode();
		*(int*)pOutL = -2;
		return iret;
	}
	else
	{
		//MessageBox("계좌이체 성공", "전자 서명", MB_OK);
		CopyMemory((void*)pOutB, p2.value, p2.length);
		*(int*)pOutL = p2.length;
		
		if (m_ca == caNO)
			m_ca = caOKx;
		m_calogon = true;
		savePasswd();

		return 1;
	}
}

int CCertifyCtrl::Cloude_ConTraction_sign(long pOutB, long pOutL)
{
	int rc = 0;
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
		if (rc == 0)
		{
			memset(&m_contextNew, 0x00, sizeof(SD_API_CONTEXT_NEW));
			memcpy(&m_contextNew, &tContext, sizeof(SD_API_CONTEXT_NEW));
			sk_if_cert_preset_context(&m_context, &m_contextNew.sd);
		}
		else
		{
			int errorCode = sk_if_GetLastErrorCode();
			MessageBox(sk_if_GetLastErrorMsg(), "축약서명 오류", MB_OK);
			m_sync.Unlock();
			return  -2;
		}

		memset(&tContext, 0x00, sizeof(SD_API_CONTEXT_NEW));
	}

	m_sync.Lock();
	m_nBytes = *(int*)pOutL;
	p1.length = m_nBytes;
	p1.value = (unsigned char*)pOutB;

	if (sk_if_cert_SignData_notEncode(&m_appContext, "", &p1, &p2, NULL))
	{
		m_slog.Format("축약서명 오류 Ecod =[%s]", sk_if_GetLastErrorMsg());
		MessageBox(m_slog, "IBK 투자증권", MB_OK);
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
		return 1;
	}
	return 0;
}

//CString CCertifyCtrl::checkPasswd()
//{
//	BOOL bCloude = CheckCloude();
//
//	m_slog.Format("[certify][CCertifyCtrl::checkPasswd]  m_ca=[%s] bCloude=[%d]\r\n", getStatus(), bCloude);
//	OutputDebugString(m_slog);
//
//	if (bCloude)
//	{
//		CString	text, pass = _T("");
//
//		SD_API_CONTEXT_NEW tContext; //구조체 선언(클라우드 인증서 정보를 담을)
//		memset(&tContext, 0x00, sizeof(SD_API_CONTEXT_NEW));
//		int rc = 0;
//
//		CString plain, strResult;
//		UString p1, p2, p3;
//
//		//간편비밀번호를 다시 입력 받고 싶다면
//		if (m_contextNew.sd.bOldStorage == 7)
//		{
//			memcpy(&tContext, &m_contextNew, sizeof(SD_API_CONTEXT_NEW));
//			memset(&tContext.sd.szOldPasswd, 0x00, sizeof(tContext.sd.szOldPasswd)); //비밀번호 부분만 초기화
//
//			rc = sk_if_CloudCertSetSelectExt(&tContext, 1);
//
//			if (rc == 0)
//			{
//				memset(&m_contextNew, 0x00, sizeof(SD_API_CONTEXT_NEW));
//				memcpy(&m_contextNew, &tContext, sizeof(SD_API_CONTEXT_NEW));
//				sk_if_cert_preset_context(&m_context, &m_contextNew.sd);
//
//			}
//			else
//			{
//				int errorCode = sk_if_GetLastErrorCode();
//				MessageBox(sk_if_GetLastErrorMsg(), "전자 서명 오류", MB_OK);
//				return "";
//				//if (errorCode == 2501)   //일부러 취소했을때
//				//	*(int*)pOutL = -3;
//				//else
//				//	*(int*)pOutL = -2;
//				//return -1;
//			}
//			memset(&tContext, 0x00, sizeof(SD_API_CONTEXT_NEW));
//		}
//
//		plain = "abcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()-=";
//		p1.value = (unsigned char*)LPCTSTR(plain);
//		p1.length = plain.GetLength();
//
//		//p1 = 원문 , p2 결과값 , p3 = R값 결과
//		memset(&p3, 0x00, sizeof(UString));
//		rc = sk_if_cert_SignDataWithR(&m_context, "", &p1, &p2, &p3);
//
//		if (rc) {
//			MessageBox(sk_if_GetLastErrorMsg(), "전자서명 오류", MB_OK);
//			return "";
//			/*CString emsg;
//			emsg = sk_if_GetLastErrorMsg();
//			int iret = sk_if_GetLastErrorCode();
//			*(int*)pOutL = -2;
//			return iret;*/
//		}
//		else
//		{
//			//MessageBox("계좌이체 성공", "전자 서명", MB_OK);
//		//	CopyMemory((void*)pOutB, p2.value, p2.length);
//		/*	*(int*)pOutL = p2.length;
//
//			if (m_ca == caNO)
//				m_ca = caOKx;
//			m_calogon = true;
//			savePasswd();
//
//			return 1;*/
//
//			pass = CString(tContext.sd.szOldPasswd, 33);
//			return pass;
//		}
//		/*		*/
//
//		return pass;
//	}
//	else
//	{
//		CString	text, pass = _T("");
//		UString	src, des;
//		char	encpass[32 + 1];
//		int	pswdL;
//
//		m_sync.Lock();
//		ZeroMemory(encpass, sizeof(encpass));
//		pswdL = sizeof(m_context.pInterfaceContext->szOldPasswd);
//		ZeroMemory(m_context.pInterfaceContext->szOldPasswd, pswdL);
//
//		src.length = 0;
//		src.value = NULL;
//		sk_if_SetKeySaferMode(1);
//		if (sk_if_cert_SignData_notEncode(&m_context, encpass, &src, &des, NULL))
//		{
//			if (sk_if_GetLastErrorCode() == 2417)
//			{
//				m_ca = caPWDa;
//				m_string = _T("pswd\t");
//				OnFire(FEV_CA, MAKELONG(invokeCA, m_string.GetLength()), (long)(char*)m_string.operator LPCTSTR());
//			}
//			ZeroMemory(encpass, sizeof(encpass));
//			m_sync.Unlock();
//			return pass;
//		}
//		sk_if_cert_MemFree(des.value);
//
//		ZeroMemory(encpass, sizeof(encpass));
//		pass = CString(m_context.pInterfaceContext->szOldPasswd, pswdL);
//		m_sync.Unlock();
//
//		m_slog.Format("[certify][CCertifyCtrl::checkPasswd]  pass=[%s] \r\n", pass);
//		OutputDebugString(m_slog);
//
//		return pass;
//	}
//}
