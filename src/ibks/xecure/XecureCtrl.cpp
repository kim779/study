// XecureCtrl.cpp : CXecureCtrl ActiveX 컨트롤 클래스의 구현입니다.

#include "pch.h"
#include "framework.h"
#include "Xecure.h"
#include "XecureCtrl.h"
#include "XecurePropPage.h"
#include "afxdialogex.h"
#include "Log.h"

void WriteLog(LPCSTR log, ...)
{
#if 0
	TRY
	{
		CString slog;
	slog.Format("[AXIS][WriteLog] [%s]\n", log);
	//OutputDebugString(slog);

		FILE* fp;
		fopen_s(&fp, "C://openapi.log", "a+");
		if (!fp) return;

		const CTime time = CTime::GetCurrentTime();
		fprintf(fp, (LPCSTR)time.Format("[%Y-%m-%d %H:%M:%S] "));

		va_list argptr;
		va_start(argptr, log);
		vfprintf(fp, log, argptr);
		va_end(argptr);
		fprintf(fp, "\n");

		fclose(fp);
	}
		CATCH(CMemoryException, e)
	{

	}
	END_CATCH
#endif
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define XECURE_PSWD	"qwer1234"
#define	XECURE_CONF	"xc_conf.ini"

IMPLEMENT_DYNCREATE(CXecureCtrl, COleControl)

// 메시지 맵입니다.

BEGIN_MESSAGE_MAP(CXecureCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()

// 디스패치 맵입니다.

BEGIN_DISPATCH_MAP(CXecureCtrl, COleControl)
	DISP_FUNCTION(CXecureCtrl, "Encrypt", Encrypt, VT_BOOL, VTS_I4 VTS_I4)
	DISP_FUNCTION(CXecureCtrl, "Decrypt", Decrypt, VT_BOOL, VTS_I4 VTS_I4)
	DISP_FUNCTION(CXecureCtrl, "Xecure", Xecure, VT_I4, VTS_I4 VTS_I4)
END_DISPATCH_MAP()

// 이벤트 맵입니다.

BEGIN_EVENT_MAP(CXecureCtrl, COleControl)
END_EVENT_MAP()

// 속성 페이지입니다.

// TODO: 필요에 따라 속성 페이지를 추가합니다.  카운트도 이에 따라 증가해야 합니다.
BEGIN_PROPPAGEIDS(CXecureCtrl, 1)
	PROPPAGEID(CXecurePropPage::guid)
END_PROPPAGEIDS(CXecureCtrl)

// 클래스 팩터리와 GUID를 초기화합니다.

IMPLEMENT_OLECREATE_EX(CXecureCtrl, "AxisXecure.XecureCtrl.IBK2019",
	0x5526eb80,0xfe01,0x4fc4,0x91,0x94,0x8f,0xb8,0x06,0xd6,0xad,0x29)

// 형식 라이브러리 ID와 버전입니다.

IMPLEMENT_OLETYPELIB(CXecureCtrl, _tlid, _wVerMajor, _wVerMinor)

// 인터페이스 ID입니다.

const IID IID_DXecure = {0x316efc7f,0x38e6,0x4ce6,{0x96,0x8e,0x8d,0xb2,0x34,0xd8,0xc2,0x31}};
const IID IID_DXecureEvents = {0xeb73b3f7,0x1f77,0x40db,{0x82,0xf3,0x57,0x6c,0x90,0x61,0x6f,0xb4}};

// 컨트롤 형식 정보입니다.

static const DWORD _dwXecureOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CXecureCtrl, IDS_XECURE, _dwXecureOleMisc)

// CXecureCtrl::CXecureCtrlFactory::UpdateRegistry -
// CXecureCtrl에 대한 시스템 레지스트리 항목을 추가하거나 제거합니다.

BOOL CXecureCtrl::CXecureCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_XECURE,
			IDB_XECURE,
			afxRegApartmentThreading,
			_dwXecureOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


// CXecureCtrl::CXecureCtrl - 생성자

CXecureCtrl::CXecureCtrl()
{
	InitializeIIDs(&IID_DXecure, &IID_DXecureEvents);

	m_enc = stepENC::encHELLO;

	_TCHAR	path[MAX_PATH] = { 0, };

	int rc = GetWindowsDirectory(path, MAX_PATH);
	CString text = CString(path, rc);
	rc = text.Find(_T(":\\"));
	if (rc != -1)
		text = text.Left(rc + 1);

	text = _T("c:\\xecure.trc");
	m_log = new CLog((char*) text.GetString());
}

// CXecureCtrl::~CXecureCtrl - 소멸자

CXecureCtrl::~CXecureCtrl()
{
	if (m_log != nullptr)
		delete m_log;
}

// CXecureCtrl::OnDraw - 그리기 함수

void CXecureCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& /* rcInvalid */)
{
	if (!pdc)
		return;

	// TODO: 다음 코드를 사용자가 직접 작성한 그리기 코드로 바꾸십시오.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}

// CXecureCtrl::DoPropExchange - 지속성 지원

void CXecureCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 지속적인 사용자 지정 속성 모두에 대해 PX_ functions를 호출합니다.
}


// CXecureCtrl::OnResetState - 컨트롤을 기본 상태로 다시 설정

void CXecureCtrl::OnResetState()
{
	COleControl::OnResetState();  // DoPropExchange에 들어 있는 기본값을 다시 설정합니다.

	// TODO: 여기에서 다른 모든 컨트롤의 상태를 다시 설정합니다.
}

// CXecureCtrl 메시지 처리기
BOOL CXecureCtrl::Encrypt(long pBytes, long nBytes)
{
	if (m_enc != stepENC::encRUN)
		return FALSE;

	m_sync.Lock();
	m_nBytes = *(int*)nBytes;

	if (m_nBytes > maxlen)
		return FALSE;


	if (XC_ENCODE(&m_ctx, (unsigned char*)m_pBytes, &m_nBytes, (unsigned char*)pBytes, (int) m_nBytes, NULL, 0, XC_TMODE_MSG) < 0)
	{
		m_sync.Unlock();
		return FALSE;
	}

	*(int*)nBytes = m_nBytes;
	CopyMemory((void*)pBytes, m_pBytes, m_nBytes);

	m_sync.Unlock();
	return TRUE;
}

BOOL CXecureCtrl::Decrypt(long pBytes, long nBytes)
{
	if (m_enc != stepENC::encRUN)
		return FALSE;

	m_sync.Lock();
	m_nBytes = *(int*)nBytes;

	if (m_nBytes > maxlen)
		return FALSE;


	if (XC_DECODE(&m_ctx, (unsigned char*)m_pBytes, &m_nBytes, (unsigned char*)pBytes, m_nBytes, NULL, 0) < 0)
	{
		m_log->Trace(m_pBytes, m_nBytes, dirDEC);
		m_sync.Unlock();
		return FALSE;
	}

	*(int*)nBytes = m_nBytes;
	CopyMemory((void*)pBytes, m_pBytes, m_nBytes);

	m_sync.Unlock();
	return TRUE;
}
//#include "../../IBK/H/axislog.h"
#include "../../H/axislog.h"
long CXecureCtrl::Xecure(long pBytes, long nBytes)
{
	int	rc, encL = *(int*)nBytes;
	*(int*)nBytes = -1;

	if (encL > maxlen)
		return 0;

	if (m_enc == stepENC::encRUN && (char*)pBytes == NULL && encL == 0)
		m_enc = stepENC::encHELLO;
	m_log->Trace("xecure_1", 0, dirSTR);

	char chfile[500]{};
	GetModuleFileName(nullptr, chfile, 260);
	CString slog;
	slog.Format("[xecure] Xecure dirSTR=[%s]  path=[%s] \r\n  ", dirSTR, chfile);
	OutputDebugString(slog);
	FileLog(slog);


	
	CString spath;
	spath.Format("%s", chfile);
	spath.TrimRight(); 

	int pos = spath.ReverseFind('\\'); // 마지막 백슬래시 위치 찾기
	if (pos != -1)
	{
		CString filename = spath.Mid(pos + 1); // 파일명 추출
		spath.Replace(filename, "xc_conf.ini"); // 파일명을 새 이름으로 바꿈
	}
	
	//spath.MakeUpper();
	//spath.Replace("AXIS.EXE", "xc_conf.ini");
	//spath.Replace("axis.exe", "xc_conf.ini");
	//spath.Replace("ibks_setup.exe", "xc_conf.ini");
	OutputDebugString("[XECURE]" + spath);
	FileLog(spath);


	//C:\IBKS\IBK투자증권 HTS\exe
	//spath.Format("%s", "C:\\IBKS\\IBK투자증권 HTS\\exe\\xc_conf.ini");

	switch (m_enc)
	{
	case stepENC::encHELLO:
		//WriteLog(slog);
		//WriteLog(spath);
		m_log->Trace("xecure_2", 0, dirSTR);
		//if (XC_INIT(XECURE_PSWD, XECURE_CONF, XC_SMODE_CLIENT) < 0)

		if (XC_INIT(XECURE_PSWD, (LPSTR)(LPCTSTR)spath, XC_SMODE_CLIENT) < 0)
		{
			m_log->Trace("xecure_2_2", 0, dirSTR);
			AfxMessageBox(_T("!!Xecure Init 실패!!"));
			return 0;
		}
		m_log->Trace("xecure_3", 0, dirSTR);

		ZeroMemory(&m_ctx, sizeof(XC_CTX));
		if (XC_ENCODE(&m_ctx, (unsigned char*)m_pBytes, &m_nBytes, NULL, 0, NULL, 0, XC_TMODE_KEY) < 0)
		{
			AfxMessageBox(_T("Init_Token 실패"));
			return 0;
		}
		m_log->Trace("xecure_4", 0, dirSTR);
		m_enc = stepENC::encOK;
		break;
	case stepENC::encOK:
		rc = XC_DECODE(&m_ctx, (unsigned char*)m_pBytes, &m_nBytes, (unsigned char*)pBytes, encL, NULL, 0);
		if (rc < 0)
		{
			if (rc == XC_MTYPE_NEWPROFILE)
			{
				if (XC_ENCODE(&m_ctx, (unsigned char*)m_pBytes, &m_nBytes, NULL, 0, NULL, 0, XC_TMODE_KEY) < 0)
				{
					AfxMessageBox(_T("Init_Token 실패 2"));
					return 0;
				}
				break;
			}
		}
		m_enc = stepENC::encRUN;
		*(int*)nBytes = 0;
		return 0;
	}

	*(int*)nBytes = m_nBytes;
	return (long) m_pBytes;
}

