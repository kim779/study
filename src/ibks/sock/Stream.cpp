// Stream.cpp : implementation file
//

#include "pch.h"
#include "Sock.h"
#include "Compress.h"
#include "Wsh.h"
#include "Stream.h"
#include "../h/axisrsm.h"

#ifdef _DEBUG
#include "Log.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStream

CStream::CStream(CWsh* rsm, CWsh* wsh)
{
	m_idx    = 0;
	m_flow   = _frameF_;
	m_nBytesToResponse = 0;

	m_chainL = 0;
	m_chain  = (char *) 0;
	m_alive  = false;

	m_rsm    = rsm;
	m_wsh    = wsh;
	m_zip    = new CCompress();
	m_lock   = new CSingleLock(&m_event);

	m_ssm    = ssM_RM;
#ifdef	_DEBUG
	m_log = new CLog("axsock.trc");
#endif
}

CStream::~CStream()
{
	CloseStream();

	delete m_lock;
	delete m_zip;

#ifdef	_DEBUG
	delete m_log;
#endif
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CStream, CAsyncSocket)
	//{{AFX_MSG_MAP(CStream)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CStream member functions

void CStream::OnClose(int nErrorCode) 
{
#ifdef	_DEBUG
	m_log->Trace("OnClose signal...", 0, dirSTR);
#endif

	m_alive = false;
	if (m_ssm == ssM_RM)
		m_rsm->Dispatch(ssM_CLOSE, 0, 0);
	else
		m_wsh->Dispatch(ssM_CLOSE, 0, 0);
	CAsyncSocket::OnClose(nErrorCode);
}

void CStream::OnConnect(int nErrorCode) 
{
#ifdef	_DEBUG
	m_log->Trace("OnConnect signal...", 0, dirSTR);
#endif

	m_alive = true;
	m_wsh->Dispatch(ssM_OPEN, 0, nErrorCode);
	CAsyncSocket::OnConnect(nErrorCode);
}

void CStream::OnSend(int nErrorCode) 
{
#ifdef	_DEBUG
	m_log->Trace("OnSend signal...", 0, dirSTR);
#endif
	if (nErrorCode)
	{
		OnClose(nErrorCode);
		return;
	}

	m_event.SetEvent();
	CAsyncSocket::OnSend(nErrorCode);
}

void CStream::OnReceive(int nErrorCode) 
{
	if (nErrorCode)
	{
		OnClose(nErrorCode);
		return;
	}

	m_frameL = Receive(m_frameB, sizeof(m_frameB));

#ifdef	_DEBUG
	m_log->Trace(m_frameB, m_frameL, dirRCV);
#endif

	switch (m_frameL)
	{
	case 0:
		OnClose(0);
		return;
	case SOCKET_ERROR:			// maybe does not happened
		return;
	default:
		break;
	}

	for (int ii = 0; ii < m_frameL; ii++)
	{
		switch (m_flow)
		{
		case _frameF_:			// start flag
			switch ((BYTE)m_frameB[ii])
			{
			case fmF_FS:
				m_flow = (fmFlow) m_idx;
				m_rcvB[m_idx++] = m_frameB[ii];
				break;
			default:
				m_idx = 0;
				break;
			}
			continue;

		case _frameH_:			// fmH
			m_rcvB[m_idx++] = m_frameB[ii];
			if (m_idx < L_fmH)
				continue;

			fmH = (_fmH *) m_rcvB;
			m_ptr = (char *) &m_rcvB[L_fmH];
			CopyMemory(m_ptr, fmH->datL, sizeof(fmH->datL));
			m_ptr[sizeof(fmH->datL)] = '\0';
			m_rcvL = atoi(m_ptr);
			m_idx = 0;
			if (m_rcvL == 0)
			{
				m_flow = (fmH->stat & stat_BCC) ? _frameBCC_ : _frameE_;
				break;
			}

			m_flow = _frameD_;
			break;

		case _frameD_:			// Data
			m_ptr[m_idx++] = m_frameB[ii];
			if (m_idx >= m_rcvL)
				m_flow = (fmH->stat & stat_BCC) ? _frameBCC_ : _frameE_;
			break;

		case _frameBCC_:		// BCC
			BYTE	bcc = 0;
			for (int jj = 0; jj < m_rcvL; jj++)
				bcc ^= m_ptr[jj];

			bcc |= 0x20;
			if ((BYTE)m_frameB[ii] != bcc)
			{
				strcpy_s(m_rcvB, sizeof(m_rcvB), "AXIS/BCC ERROR ... Check server program");
				m_flow = _frameF_;
				m_idx = 0;
				continue;
			}
			m_flow = _frameE_;
			break;
		}
		if (m_flow == _frameE_)
		{
			m_flow = _frameF_;
			m_idx = 0;
			if (!ParseFrame())
			{
				OnClose(0);
				return;
			}
		}
	}
}

bool CStream::ParseFrame()
{
	switch (fmH->fmC)
	{
	case fmC_NRM:
		if (fmH->stat & stat_PACK)
		{
			BYTE	dbuf[maxIOs];
			m_rcvL = m_zip->decompress((BYTE *) m_ptr, m_rcvL, dbuf);
			CopyMemory(m_ptr, dbuf, m_rcvL);
		}

		if (m_chain)
		{
			char *chain = m_chain;
			m_chain = new char[m_chainL+m_rcvL+1];
			CopyMemory(m_chain, chain, m_chainL);
			delete [] chain;
		}
		else
		{
			m_chainL = 0;
			m_chain = new char[m_rcvL+1];
		}

		CopyMemory(&m_chain[m_chainL], m_ptr, m_rcvL);
		m_chainL += m_rcvL;
		break;
	case fmC_ERR:
	case fmC_ERR2:
		m_ptr[m_rcvL++] = '\0';
		m_wsh->Dispatch(ssM_ERROR, m_ptr, m_rcvL);

		if (fmH->fmC == fmC_ERR2)
			return false;
		return ResponseFM(fmC_ACK);
	case fmC_ACK:
		return true;
	case fmC_NAK:
		strcpy_s(m_rcvB, sizeof(m_rcvB), "AXIS/NAK ERROR ... Check server program");
		m_rcvL = strlen(m_rcvB);
		m_wsh->Dispatch(ssM_ERROR, m_rcvB, ++m_rcvL);
		return true;
	case fmC_PLL:
		return ResponseFM(fmC_PLL);
	default:
		return ResponseFM(fmC_NAK);
	}

	bool isSuccess = ResponseFM(fmC_ACK);
	if (!(fmH->stat & stat_CONF))
	{
		switch (fmH->ssM)
		{
		case ssM_RM:
			m_rsm->Dispatch(fmH->ssM, m_chain, m_chainL);
			if (m_chainL >= L_rsmH && ((struct _rsmH *)m_chain)->resC & resC_RSP)
			{
				ZeroMemory(m_chain, L_rsmH);
				((struct _rsmH *)m_chain)->resK = resK_RSP;
				WriteData(m_chain, L_rsmH);
			}
			break;
		case ssM_SN:
			m_rsm->Dispatch(fmH->ssM, m_chain, m_chainL);
			break;
		default:
			m_wsh->Dispatch(fmH->ssM, m_chain, m_chainL);
			break;
		}

		delete [] m_chain;
		m_chain = (char *) 0;
		m_chainL = 0;
	}
	return isSuccess;
}

bool CStream::RunStream(int ssm)
{
	_fmH *fmh = (_fmH *) m_pBytesToResponse;
	fmh->fmF[0] = fmF_FS;
	fmh->fmF[1] = fmF_FS;
	fmh->stat = stat_WS;
	fmh->stat = 0;
	fmh->ssM = m_ssm = ssm;
	FillMemory(fmh->rsvB, sizeof(fmh->rsvB), ' ');
	FillMemory(fmh->datL, sizeof(fmh->datL), '0');
	fmh->fmC = fmC_SSM;
	m_nBytesToResponse = L_fmH;

	return WriteBytes(m_pBytesToResponse, m_nBytesToResponse);
}

bool CStream::RunVersion(char* keys, int size)
{
	int	sndl;
	char*	sndb;
	bool	retv;

	sndl = L_rsmH;
	sndb = new char[L_rsmH+size];
	ZeroMemory(sndb, L_rsmH);

	((struct _rsmH *)sndb)->dirF = dirF_INB;
	((struct _rsmH *)sndb)->resK = resK_REQ;
	((struct _rsmH *)sndb)->resC = resC_INFO|resC_EXT;
	((struct _rsmH*)sndb)->rsvB[0] = rsvB_NEW;

	if (size > 0)
	{
		CopyMemory(&sndb[sndl], keys, size);
		sndl += size;
	}

	retv = WriteData(sndb, sndl);
	delete [] sndb;
	return retv;
}

bool CStream::ResponseFM(unsigned char fmC)
{
	if ((fmH->stat & stat_RSP) || fmC == fmC_PLL)
	{
		_fmH *fmh = (_fmH *) m_pBytesToResponse;
		fmh->fmF[0] = fmF_FS;
		fmh->fmF[1] = fmF_FS;
		fmh->stat = stat_WS;
		fmh->stat = 0;
		fmh->ssM = ssM_WS;
		FillMemory(fmh->rsvB, sizeof(fmh->rsvB), ' ');
		FillMemory(fmh->datL, sizeof(fmh->datL), '0');
		fmh->fmC = fmC;
		m_nBytesToResponse = L_fmH;

		return WriteBytes(m_pBytesToResponse, m_nBytesToResponse);
	}
	return true;
}

bool CStream::WriteData(char *data, int ndat, bool wait)
{
	CString text;

	_fmH *fmh = (_fmH *) m_pBytesToData;
	fmh->fmF[0] = fmF_FS;
	fmh->fmF[1] = fmF_FS;
	fmh->stat = stat_WS;
	if (wait)
		fmh->stat |= stat_RSP;
	fmh->fmC = fmC_NRM;
	fmh->ssM = m_ssm;
	FillMemory(fmh->rsvB, sizeof(fmh->rsvB), ' ');

	int	nBytes;
	nBytes = m_zip->compress((BYTE *) data, ndat, (BYTE *) &m_pBytesToData[L_fmH]);
	if (nBytes >= ndat || nBytes <= 0)
	{
		nBytes = ndat;
		CopyMemory(&m_pBytesToData[L_fmH], data, nBytes);
	}
	else
		fmh->stat |= stat_PACK;

	text.Format(_T("%05d"), nBytes);
	CopyMemory(fmh->datL, (char*)text.GetString(), min(sizeof(fmh->datL), text.GetLength()));

	m_nBytesToData = L_fmH+nBytes;
	if (fmh->stat & stat_BCC)
	{
		unsigned char	bcc = 0;
		for (int ii = 0; ii < nBytes; ii++)
			bcc ^= m_pBytesToData[L_fmH+ii];
		m_pBytesToData[m_nBytesToData++] = bcc | 0x20;
	}
	return WriteBytes(m_pBytesToData, m_nBytesToData);
}

bool CStream::WriteBytes(char *pBytes, int nBytes)
{
#ifdef	_DEBUG
	m_log->Trace(pBytes, nBytes, dirSND);
#endif
	for (int ii = 0; ;)
	{
		int nWritten = Send(&pBytes[ii], nBytes);
		if (nWritten == SOCKET_ERROR)
		{
			switch (GetLastError())
			{
			case WSAEINPROGRESS:
			case WSAEWOULDBLOCK:
				break;
			default:
				return false;
			}
		}
		else
		{
			nBytes -= nWritten;
			if (nBytes == 0)
				break;
			ii += nWritten;
		}
		BOOL isSuccess = m_lock->Lock();
		m_lock->Unlock();
		if (!isSuccess)
			return false;
	}
	return true;
}

bool CStream::OpenStream(char* ip, int port)
{
#ifdef	_DEBUG
	char	msg[80];
	sprintf_s(msg, sizeof(msg), "Open(%s, %d)", ip, port);
	m_log->Trace(msg, 0, dirSTR);
#endif

	CloseStream();

	if (Create())
	{
		int	value;

		value = TRUE;
		SetSockOpt(SO_KEEPALIVE, &value, sizeof(value));
		value = TRUE;
		SetSockOpt(SO_REUSEADDR, &value, sizeof(value));
		value = maxIOs;
		SetSockOpt(SO_SNDBUF, &value, sizeof(value));
		value = maxIOs;
		SetSockOpt(SO_RCVBUF, &value, sizeof(value));
		value = TRUE;
		SetSockOpt(TCP_NODELAY, &value, sizeof(value), IPPROTO_TCP);
		if (!Connect((LPCTSTR)ip, (UINT)port))
		{
			switch (GetLastError())
			{
			case WSAEINPROGRESS:
			case WSAEWOULDBLOCK:
				break;
			default:
				return false;
			}
		}
		return true;
	}
	return false;
}

void CStream::CloseStream()
{
#ifdef	_DEBUG
	CString	name;
	UINT	port;

	if (GetPeerName(name, port))
	{
		char	msg[80];
		sprintf_s(msg, sizeof(msg), "Close(%s, %d)", name.GetString(), port);
		m_log->Trace(msg, 0, dirSTR);
	}
#endif

	m_alive = false;

	ShutDown(2);
	Close();
}