// MSocketCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MSocktCtrl.h"
#include "Stream.h"
#include "Dgram.h"
#include "Wsh.h"
#include "../h/axisfire.h"

// CMSocketCtrl

IMPLEMENT_DYNAMIC(CMSocktCtrl, CWnd)

CMSocktCtrl::CMSocktCtrl() : CWnd()
{
	m_rsm = (CWsh *)AfxBeginThread(RUNTIME_CLASS(CWsh), THREAD_PRIORITY_NORMAL, 0, 0);
	m_wsh = (CWsh *)AfxBeginThread(RUNTIME_CLASS(CWsh), THREAD_PRIORITY_NORMAL, 0, 0);

	m_stream = new CStream(m_rsm, m_wsh, m_wsh->m_home);
	m_dgram  = new CDgram(m_rsm, m_wsh);	
}

CMSocktCtrl::~CMSocktCtrl()
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


BEGIN_MESSAGE_MAP(CMSocktCtrl, CWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WFM_EVENT, OnFireEvent) 
	ON_MESSAGE(WFM_ALERT, OnFireAlert)
	ON_MESSAGE(WFM_PUSH, OnFirePush)
	ON_MESSAGE(WFM_AXIS, OnFireAxis)
	ON_MESSAGE(WFM_STAT, OnFireStat)
	ON_MESSAGE(WFM_WRITE, OnFireWrite)
	ON_MESSAGE(WFM_VERS, OnFireVersion)
END_MESSAGE_MAP()

// CMSocketCtrl 메시지 처리기입니다.


/////////////////////////////////////////////////////////////////////////////
// CMSocktCtrl::DoPropExchange - Persistence support

/*
void CMSocktCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);
}
*/

/////////////////////////////////////////////////////////////////////////////
// CMSocktCtrl::OnResetState - Reset control to default state

/*
void CMSocktCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange
}
*/

/////////////////////////////////////////////////////////////////////////////
// CMSocktCtrl message handlers


LRESULT CMSocktCtrl::OnFireEvent(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case FEV_OPEN:
		m_pEmule->SendMessage(WFM_EVENT, wParam, lParam);
		break;
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

LRESULT CMSocktCtrl::OnFireAlert(WPARAM wParam, LPARAM lParam)
{
	m_pEmule->SendMessage(WFM_ALERT, wParam, lParam);	
//	OnStream(FEV_ANM, lParam, (long)wParam);
	return 0;
}

LRESULT CMSocktCtrl::OnFirePush(WPARAM wParam, LPARAM lParam)
{
	m_pEmule->SendMessage(WFM_PUSH, wParam, lParam);	
//	OnStream(FEV_PUSH, lParam, (long)wParam);
	return 0;
}

LRESULT CMSocktCtrl::OnFireAxis(WPARAM wParam, LPARAM lParam)
{
//	OnStream(FEV_AXIS, lParam, (long)wParam);
	m_pEmule->SendMessage(WFM_AXIS, wParam, lParam);
	return 0;
}

LRESULT CMSocktCtrl::OnFireStat(WPARAM wParam, LPARAM lParam)
{
//	OnStream(FEV_STAT, lParam, (long)wParam);
	m_pEmule->SendMessage(WFM_STAT, wParam, lParam);
	return 0;
}

LRESULT CMSocktCtrl::OnFireWrite(WPARAM wParam, LPARAM lParam)
{
	m_stream->WriteData((char *)lParam, (int)wParam);
	return 0;
}

LRESULT CMSocktCtrl::OnFireVersion(WPARAM wParam, LPARAM lParam)
{
	//OnStream(WFM_VERS, lParam, (long)wParam);
	m_pEmule->SendMessage(WFM_STAT, wParam, lParam);
	return 0;
}

BOOL CMSocktCtrl::OpenSock(LPCTSTR home, LPCTSTR ip, long port) 
{
	CloseSock();

	m_rsm->Startup(this, (char *)home);
	m_wsh->Startup(this, (char *)home);
	return m_stream->OpenStream((char *)ip, port);
}

void CMSocktCtrl::CloseSock() 
{
	m_stream->CloseStream();
}

BOOL CMSocktCtrl::WriteStream(char* pBytes, long nBytes) 
{
	return m_stream->WriteData((char *)pBytes, nBytes);
}

BOOL CMSocktCtrl::OpenDgram(long port) 
{
	CloseDgram();
	return m_dgram->OpenDgram(port);
}

void CMSocktCtrl::CloseDgram() 
{
	m_dgram->CloseDgram();
}

BOOL CMSocktCtrl::WriteDgram(LPCTSTR ip, LONG_PTR pBytes, long nBytes, BOOL route) 
{
	return m_dgram->WriteData((char *)ip, (char *)pBytes, nBytes, route);
}

BOOL CMSocktCtrl::StartAxis(LONG_PTR key, long mode) 
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

long CMSocktCtrl::GetEnum(LONG_PTR pBytes, long nBytes, long mode) 
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


int CMSocktCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	return 0;
}
