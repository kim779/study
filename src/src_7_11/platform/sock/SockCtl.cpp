// SockCtl.cpp : Implementation of the CSockCtrl ActiveX Control class.

#include "stdafx.h"
#include "Sock.h"
#include "SockCtl.h"
#include "SockPpg.h"
#include "Stream.h"
#include "Dgram.h"
#include "Wsh.h"
#include "../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSockCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSockCtrl, COleControl)
	//{{AFX_MSG_MAP(CSockCtrl)
	ON_MESSAGE(WFM_EVENT, OnFireEvent)
	ON_MESSAGE(WFM_ALERT, OnFireAlert)
	ON_MESSAGE(WFM_PUSH, OnFirePush)
	ON_MESSAGE(WFM_AXIS, OnFireAxis)
	ON_MESSAGE(WFM_STAT, OnFireStat)
	ON_MESSAGE(WFM_WRITE, OnFireWrite)
	ON_MESSAGE(WFM_VERS, OnFireVersion)
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CSockCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CSockCtrl)
	DISP_FUNCTION(CSockCtrl, "OpenSock", OpenSock, VT_BOOL, VTS_BSTR VTS_BSTR VTS_I4)
	DISP_FUNCTION(CSockCtrl, "CloseSock", CloseSock, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CSockCtrl, "WriteStream", WriteStream, VT_BOOL, VTS_I4 VTS_I4)
	DISP_FUNCTION(CSockCtrl, "OpenDgram", OpenDgram, VT_BOOL, VTS_I4)
	DISP_FUNCTION(CSockCtrl, "CloseDgram", CloseDgram, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CSockCtrl, "WriteDgram", WriteDgram, VT_BOOL, VTS_BSTR VTS_I4 VTS_I4 VTS_BOOL)
	DISP_FUNCTION(CSockCtrl, "StartAxis", StartAxis, VT_BOOL, VTS_I4 VTS_I4)
	DISP_FUNCTION(CSockCtrl, "GetEnum", GetEnum, VT_I4, VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CSockCtrl, COleControl)
	//{{AFX_EVENT_MAP(CSockCtrl)
	EVENT_CUSTOM("OnStream", OnStream, VTS_I4  VTS_I4  VTS_I4)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CSockCtrl, 1)
	PROPPAGEID(CSockPropPage::guid)
END_PROPPAGEIDS(CSockCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

// {5D0FDBF9-A1AE-46E0-BA4C-477FA94C8358}
IMPLEMENT_OLECREATE_EX(CSockCtrl, "AxisSock.SockCtrl.ERGATE",
	0xdca192b2, 0x9e5e, 0x4e97, 0x88, 0xe4, 0xf3, 0xfe, 0x92, 0x9d, 0xe4, 0x27);


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CSockCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DSock =
		{ 0x425b9540, 0x5811, 0x4ff2, { 0xb0, 0x54, 0x41, 0x7d, 0x84, 0x8, 0xba, 0xa5 } };
const IID BASED_CODE IID_DSockEvents =
		{ 0xdf8b333e, 0x3e0b, 0x4ccd, { 0x94, 0xd, 0x32, 0x8e, 0x22, 0xd4, 0x19, 0xf4 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwSockOleMisc =
	OLEMISC_INVISIBLEATRUNTIME |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CSockCtrl, IDS_SOCK, _dwSockOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CSockCtrl::CSockCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CSockCtrl

BOOL CSockCtrl::CSockCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

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


/////////////////////////////////////////////////////////////////////////////
// CSockCtrl::CSockCtrl - Constructor

CSockCtrl::CSockCtrl()
{
	InitializeIIDs(&IID_DSock, &IID_DSockEvents);

	m_rsm = (CWsh *)AfxBeginThread(RUNTIME_CLASS(CWsh), THREAD_PRIORITY_NORMAL, 0, 0);
	m_wsh = (CWsh *)AfxBeginThread(RUNTIME_CLASS(CWsh), THREAD_PRIORITY_NORMAL, 0, 0);

	m_stream = new CStream(m_rsm, m_wsh, m_wsh->m_home);
	m_dgram  = new CDgram(m_rsm, m_wsh);
}


/////////////////////////////////////////////////////////////////////////////
// CSockCtrl::~CSockCtrl - Destructor

CSockCtrl::~CSockCtrl()
{
	HANDLE	hThread;

	delete m_dgram;
	delete m_stream;

	hThread = m_rsm->m_hThread;
	m_rsm->Cleanup();
	if (WaitForSingleObject(hThread, 500) == WAIT_TIMEOUT)
		TerminateThread(hThread, 0);

	hThread = m_wsh->m_hThread;
	m_wsh->Cleanup();
	if (WaitForSingleObject(hThread, 500) == WAIT_TIMEOUT)
		TerminateThread(hThread, 0);
}


/////////////////////////////////////////////////////////////////////////////
// CSockCtrl::OnDraw - Drawing function

void CSockCtrl::OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
}


/////////////////////////////////////////////////////////////////////////////
// CSockCtrl::DoPropExchange - Persistence support

void CSockCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);
}


/////////////////////////////////////////////////////////////////////////////
// CSockCtrl::OnResetState - Reset control to default state

void CSockCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange
}


/////////////////////////////////////////////////////////////////////////////
// CSockCtrl message handlers

LRESULT CSockCtrl::OnFireEvent(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case FEV_ERROR:
	case FEV_SIZE:
		OnStream((int)wParam, lParam, 0);
		break;
	default:
		OnStream((int)wParam, 0, (long)lParam);
		break;
	}
	return 0;
}

LRESULT CSockCtrl::OnFireAlert(WPARAM wParam, LPARAM lParam)
{
	OnStream(FEV_ANM, lParam, (long)wParam);
	return 0;
}

LRESULT CSockCtrl::OnFirePush(WPARAM wParam, LPARAM lParam)
{
	OnStream(FEV_PUSH, lParam, (long)wParam);
	return 0;
}

LRESULT CSockCtrl::OnFireAxis(WPARAM wParam, LPARAM lParam)
{
	OnStream(FEV_AXIS, lParam, (long)wParam);
	return 0;
}

LRESULT CSockCtrl::OnFireStat(WPARAM wParam, LPARAM lParam)
{
	OnStream(FEV_STAT, lParam, (long)wParam);
	return 0;
}

LRESULT CSockCtrl::OnFireWrite(WPARAM wParam, LPARAM lParam)
{
	m_stream->WriteData((char *)lParam, (int)wParam);
	return 0;
}

LRESULT CSockCtrl::OnFireVersion(WPARAM wParam, LPARAM lParam)
{
	OnStream(FEV_VERS, lParam, (long)wParam);
	return 0;
}

BOOL CSockCtrl::OpenSock(LPCTSTR home, LPCTSTR ip, long port) 
{
	CloseSock();

	m_rsm->Startup(this, (char *)home);
	m_wsh->Startup(this, (char *)home);
	return m_stream->OpenStream((char *)ip, port);
}

void CSockCtrl::CloseSock() 
{
	m_stream->CloseStream();
}

BOOL CSockCtrl::WriteStream(LONG_PTR pBytes, long nBytes) 
{
	return m_stream->WriteData((char *)pBytes, nBytes);
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

BOOL CSockCtrl::WriteDgram(LPCTSTR ip, LONG_PTR pBytes, long nBytes, BOOL route) 
{
	return m_dgram->WriteData((char *)ip, (char *)pBytes, nBytes, route);
}

BOOL CSockCtrl::StartAxis(LONG_PTR key, long mode) 
{
	switch (mode)
	{
	case modeRM:
		m_stream->RunStream(ssM_RM);
		if (m_rsm->GetUpdateList())
			return m_rsm->DoRequest();
		return m_stream->RunVersion(NULL, 0);
	case modeWS:
		return m_stream->RunStream(ssM_WS);
	case modeWM:
		return m_stream->RunStream(ssM_WM);
	default:					// 3rd P/F version manager
		m_stream->RunStream(ssM_RM);
		return m_stream->RunVersion((char *)key, mode);
	}
}

long CSockCtrl::GetEnum(LONG_PTR pBytes, long nBytes, long mode) 
{
	switch (mode)
	{ 
	case enumQUEUE:
		return m_wsh->GetQueuingCount();

	default:
		break;
	}

	return 0;
}
