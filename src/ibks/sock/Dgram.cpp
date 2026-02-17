// Dgram.cpp : implementation file
//

#include "pch.h"
#include "Sock.h"
#include "Wsh.h"
#include "Dgram.h"

#ifdef _DEBUG
#include "Log.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgram

CDgram::CDgram(CWsh* rsm, CWsh* wsh)
{
	m_rsm = rsm;
	m_wsh = wsh;
	m_ip.Empty();

#ifdef	_DEBUG
	m_log = new CLog("axudp.trc");
#endif
}

CDgram::~CDgram()
{
#ifdef	_DEBUG
	delete m_log;
#endif
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CDgram, CAsyncSocket)
	//{{AFX_MSG_MAP(CDgram)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CDgram member functions

void CDgram::OnReceive(int nErrorCode) 
{
	if (nErrorCode)
		return;

	CString	ip;
	UINT	port;
	m_nBytes = ReceiveFrom(m_pBytes, sizeof(m_pBytes), ip, port);

#ifdef	_DEBUG
	m_log->Trace(m_pBytes, m_nBytes, dirRCV);
#endif

	switch (m_nBytes)
	{
	case 0:
		return;
	case SOCKET_ERROR:			// maybe does not happened
		return;
	default:
		break;
	}

	_fmH	*fmH = (_fmH *) m_pBytes;
	if (m_nBytes <= L_fmH || fmH->fmF[0] != fmF_FS || fmH->fmF[1] != fmF_FS)
		return;

	switch (fmH->ssM)
	{
	case ssM_RM:
	case ssM_SN:
		m_rsm->Dispatch(fmH->ssM, &m_pBytes[L_fmH], m_nBytes-L_fmH);
		break;
	case ssM_UD:
		m_ip = ip;
	case ssM_AP:
	default:
		m_wsh->Dispatch(fmH->ssM, &m_pBytes[L_fmH], m_nBytes-L_fmH);
		break;
	}
}

bool CDgram::OpenDgram(int port)
{
#ifdef	_DEBUG
	char	msg[80];
	sprintf_s(msg, sizeof(msg), "Open(%d)", port);
	m_log->Trace(msg, 0, dirSTR);
#endif

	CloseDgram();

	m_ip.Empty();
	m_port = port;

	if (Create(port, SOCK_DGRAM, FD_READ|FD_WRITE))
	{
		int	value = TRUE;
		SetSockOpt(SO_REUSEADDR, &value, sizeof(value));
		value = maxIOs;
		SetSockOpt(SO_SNDBUF, &value, sizeof(value));
		value = maxIOs;
		SetSockOpt(SO_RCVBUF, &value, sizeof(value));
		return true;
	}
	return false;
}

bool CDgram::WriteData(char* ip, char* datB, int datL, BOOL route)
{
	CString	text;

	_fmH *fmh   = (_fmH *) m_frameB;
	fmh->fmF[0] = fmF_FS; 
	fmh->fmF[1] = fmF_FS;
	fmh->stat   = stat_WS;
	fmh->fmC    = fmC_NRM;
	fmh->ssM    = route ? ssM_UD : ssM_AP;
	FillMemory(fmh->rsvB, sizeof(fmh->rsvB), ' ');

	text.Format(_T("%05d"), datL);
	CopyMemory(fmh->datL, (char *)text.GetString(), sizeof(fmh->datL));
	CopyMemory(&m_frameB[L_fmH], datB, datL);
	m_frameL = L_fmH + datL;
#ifdef	_DEBUG
	m_log->Trace(m_frameB, m_frameL, dirSND);
#endif
	if (SendTo(m_frameB, m_frameL, m_port, ip) != SOCKET_ERROR)
	{
		if (route)
		{
			CString	tip = m_ip;
//			if (!tip.IsEmpty() && inet_addr(tip) != inet_addr(ip))
			if (!tip.IsEmpty() && tip.CompareNoCase(ip) != 0)
			{
				SendTo(m_frameB, m_frameL, m_port, tip);
				m_ip.Empty();
			}
		}
		return true;
	}

	return false;
}


void CDgram::CloseDgram()
{
	ShutDown(2);
	Close();
}