// SockCtrl.cpp : CSockCtrl ActiveX 컨트롤 클래스의 구현입니다.

#include "pch.h"
#include "framework.h"
#include "Sock.h"
#include "SockCtrl.h"
#include "SockPropPage.h"
#include "afxdialogex.h"

#include "Stream.h"
#include "Dgram.h"
#include "Wsh.h"
#include "../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CSockCtrl, COleControl)

// 메시지 맵입니다.

BEGIN_MESSAGE_MAP(CSockCtrl, COleControl)
	ON_MESSAGE(WFM_EVENT, OnFireEvent)
	ON_MESSAGE(WFM_PUSH, OnFirePush)
	ON_MESSAGE(WFM_AXIS, OnFireAxis)
	ON_MESSAGE(WFM_STAT, OnFireStat)
	ON_MESSAGE(WFM_WRITE, OnFireWrite)
	ON_MESSAGE(WFM_VERS, OnFireVersion)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
//	ON_WM_CREATE()
END_MESSAGE_MAP()

// 디스패치 맵입니다.

BEGIN_DISPATCH_MAP(CSockCtrl, COleControl)
	DISP_FUNCTION(CSockCtrl, "OpenSock", OpenSock, VT_BOOL, VTS_BSTR VTS_BSTR VTS_I4)
	DISP_FUNCTION(CSockCtrl, "CloseSock", CloseSock, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CSockCtrl, "WriteStream", WriteStream, VT_BOOL, VTS_I4 VTS_I4)
	DISP_FUNCTION(CSockCtrl, "OpenDgram", OpenDgram, VT_BOOL, VTS_I4)
	DISP_FUNCTION(CSockCtrl, "CloseDgram", CloseDgram, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CSockCtrl, "WriteDgram", WriteDgram, VT_BOOL, VTS_BSTR VTS_I4 VTS_I4 VTS_BOOL)
	DISP_FUNCTION(CSockCtrl, "StartAxis", StartAxis, VT_BOOL, VTS_I4 VTS_I4)
	DISP_FUNCTION(CSockCtrl, "GetEnum", GetEnum, VT_I4, VTS_I4 VTS_I4 VTS_I4)
END_DISPATCH_MAP()

// 이벤트 맵입니다.

BEGIN_EVENT_MAP(CSockCtrl, COleControl)
	EVENT_CUSTOM("OnStream", OnStream, VTS_I4  VTS_I4  VTS_I4)
END_EVENT_MAP()

// 속성 페이지입니다.

// TODO: 필요에 따라 속성 페이지를 추가합니다.  카운트도 이에 따라 증가해야 합니다.
BEGIN_PROPPAGEIDS(CSockCtrl, 1)
	PROPPAGEID(CSockPropPage::guid)
END_PROPPAGEIDS(CSockCtrl)

// 클래스 팩터리와 GUID를 초기화합니다.

IMPLEMENT_OLECREATE_EX(CSockCtrl, "AxisSock.SockCtrl.IBK2019",
	0x7136b426,0x24ff,0x4e0c,0xbf,0x12,0x6b,0xf8,0xff,0x88,0x9d,0x75)

// 형식 라이브러리 ID와 버전입니다.

IMPLEMENT_OLETYPELIB(CSockCtrl, _tlid, _wVerMajor, _wVerMinor)

// 인터페이스 ID입니다.

const IID IID_DSock = {0x891ac554,0xebec,0x4e4c,{0x84,0xeb,0xe5,0xbc,0xdd,0xf5,0xab,0x08}};
const IID IID_DSockEvents = {0x77abd00d,0xa694,0x48fb,{0xa9,0x79,0xa6,0x9b,0x72,0xb8,0x1a,0x83}};

// 컨트롤 형식 정보입니다.

static const DWORD _dwSockOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CSockCtrl, IDS_SOCK, _dwSockOleMisc)

// CSockCtrl::CSockCtrlFactory::UpdateRegistry -
// CSockCtrl에 대한 시스템 레지스트리 항목을 추가하거나 제거합니다.

BOOL CSockCtrl::CSockCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_SOCK,
			IDB_SOCK,
			afxRegApartmentThreading,
			_dwSockOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


// CSockCtrl::CSockCtrl - 생성자

CSockCtrl::CSockCtrl()
{
	InitializeIIDs(&IID_DSock, &IID_DSockEvents);

	m_rsm = (CWsh*)AfxBeginThread(RUNTIME_CLASS(CWsh), THREAD_PRIORITY_NORMAL, 0, 0);
	m_wsh = (CWsh*)AfxBeginThread(RUNTIME_CLASS(CWsh), THREAD_PRIORITY_NORMAL, 0, 0);
	m_wsh->BeginRTM();

	m_stream = new CStream(m_rsm, m_wsh);
	m_dgram = new CDgram(m_rsm, m_wsh);
}

// CSockCtrl::~CSockCtrl - 소멸자

CSockCtrl::~CSockCtrl()
{
	HANDLE	hThread = (HANDLE) 0;

	delete m_dgram;
	delete m_stream;

	m_wsh->TerminateRTM();
	hThread = m_rsm->m_hThread;
	m_rsm->Cleanup();
	if (WaitForSingleObject(hThread, 500) == WAIT_TIMEOUT)
		TerminateThread(hThread, 0);

	hThread = m_wsh->m_hThread;
	m_wsh->Cleanup();
	if (WaitForSingleObject(hThread, 500) == WAIT_TIMEOUT)
		TerminateThread(hThread, 0);
}

// CSockCtrl::OnDraw - 그리기 함수

void CSockCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& /* rcInvalid */)
{
	if (!pdc)
		return;

	// TODO: 다음 코드를 사용자가 직접 작성한 그리기 코드로 바꾸십시오.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}

// CSockCtrl::DoPropExchange - 지속성 지원

void CSockCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 지속적인 사용자 지정 속성 모두에 대해 PX_ functions를 호출합니다.
}


// CSockCtrl::OnResetState - 컨트롤을 기본 상태로 다시 설정

void CSockCtrl::OnResetState()
{
	COleControl::OnResetState();  // DoPropExchange에 들어 있는 기본값을 다시 설정합니다.

	// TODO: 여기에서 다른 모든 컨트롤의 상태를 다시 설정합니다.
}

// CSockCtrl 메시지 처리기
LONG CSockCtrl::OnFireEvent(WPARAM wParam, LPARAM lParam)
{
	CMQue* pMQ;

	switch (wParam)
	{
	case FEV_ERROR:
	case FEV_SIZE:
		OnStream(wParam, lParam, 0);
		break;
	case FEV_ANM:
		pMQ = (CMQue*)lParam;
		OnStream(wParam, (long)pMQ->m_pBytes, pMQ->m_nBytes);
		break;
	default:
		OnStream(wParam, 0, lParam);
		break;
	}
	return 0;
}

LONG CSockCtrl::OnFirePush(WPARAM wParam, LPARAM lParam)
{
	OnStream(FEV_PUSH, lParam, wParam);
	return 0;
}

LONG CSockCtrl::OnFireAxis(WPARAM wParam, LPARAM lParam)
{
	OnStream(FEV_AXIS, lParam, wParam);
	return 0;
}

LONG CSockCtrl::OnFireStat(WPARAM wParam, LPARAM lParam)
{
	OnStream(FEV_STAT, lParam, wParam);
	return 0;
}

LONG CSockCtrl::OnFireWrite(WPARAM wParam, LPARAM lParam)
{
	m_stream->WriteData((char*)lParam, wParam);
	return 0;
}

LONG CSockCtrl::OnFireVersion(WPARAM wParam, LPARAM lParam)
{
	OnStream(FEV_VERS, lParam, wParam);
	return 0;
}

BOOL CSockCtrl::OpenSock(LPCTSTR home, LPCTSTR ip, long port)
{
	CloseSock();

	m_rsm->Startup(this, (char*)home);
	m_wsh->Startup(this, (char*)home);
	return m_stream->OpenStream((char*)ip, port);
}

void CSockCtrl::CloseSock()
{
	m_stream->CloseStream();
}

BOOL CSockCtrl::WriteStream(long pBytes, long nBytes)
{
	return m_stream->WriteData((char*)pBytes, nBytes);
}

BOOL CSockCtrl::OpenDgram(long port)
{
	CloseDgram();

	return m_dgram->OpenDgram(port);
}

void CSockCtrl::CloseDgram()
{
	m_dgram->CloseDgram();
}

BOOL CSockCtrl::WriteDgram(LPCTSTR ip, long pBytes, long nBytes, BOOL route)
{
	return m_dgram->WriteData((char*)ip, (char*)pBytes, nBytes, route);
}

BOOL CSockCtrl::StartAxis(long key, long mode)
{
	bool	runtime = false;

	if (mode > 0)
	{
		mode = -mode;
		runtime = true;
	}

	switch (mode)
	{
	case modeRMx:
		m_rsm->SetDownloadMode(true);
		[[fallthrough]];
	case modeRM:
		m_rsm->SetDownloadMode(false, runtime ? 1 : 0);
		m_stream->RunStream(ssM_RM);
		if (m_rsm->GetUpdateList())
			return m_rsm->DoRequest();
		return m_stream->RunVersion(NULL, 0);
	case modeWS:
		m_rsm->SetDownloadMode(false, runtime ? 1 : 0);
		return m_stream->RunStream(ssM_WS);
	case modeWM:
		return m_stream->RunStream(ssM_WM);
	default:					// 3rd P/F version manager
		m_stream->RunStream(ssM_RM);
		return m_stream->RunVersion((char*)key, mode);
	}
}

long CSockCtrl::GetEnum(long pBytes, long nBytes, long mode)
{
	switch (mode)
	{
	case enumRTS:
		return m_wsh->GetQueuingCount();
	}

	return 0;
}

