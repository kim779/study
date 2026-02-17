// glbDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axisglb.h"
#include "glbDlg.h"
#include "socket.h"

#include "../../h/axis.h"
#include "../../h/axisfm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TM_CONNECT	260
#define	TM_RECEIVE	261
#define	TMI_RECEIVE	3000

/////////////////////////////////////////////////////////////////////////////
// CGlbDlg dialog


CGlbDlg::CGlbDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGlbDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGlbDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_sock = NULL;
	m_wait = 2000;
	m_write = 250;
	m_staff = false;

	m_data.Empty();
	m_returns.Empty();

	m_connect = false;
	m_on = false;
}

CGlbDlg::~CGlbDlg()
{
	clearSocket();
	m_ips.RemoveAll();
	m_ports.RemoveAll();
}

void CGlbDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGlbDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGlbDlg, CDialog)
	//{{AFX_MSG_MAP(CGlbDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGlbDlg message handlers

long CGlbDlg::OnUser(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case sm_CLOSE:
		m_connect = false;
		if (m_sock)
		{
			m_sock->Close();
			delete m_sock;
			m_sock = NULL;
		}
		break;

	case sm_CONNECT:
		m_connect = true;
		KillTimer(TM_CONNECT);
		setGuide(_T("최적서버를 찾는 중입니다."));
		sendTR();
		SetTimer(TM_RECEIVE, TMI_RECEIVE, NULL);
		break;

	case sm_RECEIVE:
		KillTimer(TM_CONNECT);
		KillTimer(TM_RECEIVE);
		dispatch((char *)lParam, (int)HIWORD(wParam));
		break;
	
	case sm_NEXTSOCK:
		m_iip++;
		if (m_iip >= m_ips.GetSize())
			m_iip = 0;
		if (m_iip == m_sip)
			m_iport++;
		connect();
		break;
	}
	return 0;
}

BOOL CGlbDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if (!loadServer(m_path, m_staff))
	{
		MessageBox("접속 서버 정보가 없습니다.", "자동접속", MB_ICONWARNING);
		CDialog::OnCancel();
		return TRUE;
	}

	m_on = true;
	connect();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGlbDlg::OnTimer(UINT nIDEvent) 
{
	CDialog::OnTimer(nIDEvent);

	switch (nIDEvent)
	{
	case TM_CONNECT:
		KillTimer(TM_CONNECT);
		break;
	case TM_RECEIVE:
		KillTimer(TM_RECEIVE);
		break;
	default:
		return;
	}

	m_connect = false;
	if (m_sock)
	{
		m_sock->Close();
		delete m_sock;
		m_sock = NULL;
	}
	PostMessage(WM_USER, MAKEWPARAM(sm_NEXTSOCK, 0));
}

void CGlbDlg::OnCancel() 
{
	if (m_on && MessageBox("자동 접속 중입니다.\n\n취소하시겠습니까?", "자동접속", MB_ICONSTOP|MB_OKCANCEL) != IDOK)
		return;

	m_on = false;
	KillTimer(TM_CONNECT);
	KillTimer(TM_RECEIVE);
	CDialog::OnCancel();
}

void CGlbDlg::Init(char* path, char* sndB, int sndL, bool staff)
{
	m_path  = path;
	m_data  = CString(sndB, sndL);
	m_staff = staff;
}

void CGlbDlg::connect()
{
	if (m_iport >= m_ports.GetSize())
	{
		m_on = false;
		if (!m_connect)
		{
			KillTimer(TM_CONNECT);
			KillTimer(TM_RECEIVE);
			if (!m_url.IsEmpty())
			{
				m_returns = m_url;
				CDialog::OnOK();
			}
			else
			{
				MessageBox("연결실패! 잠시 후 이용바랍니다..", "자동접속", MB_ICONWARNING);
				CDialog::OnCancel();
			}
		}
		return;
	}

	if (m_sock)
	{
		m_sock->Close();
		delete m_sock;
		m_sock = NULL;
	}

TRACE("start(%d) ip(%s) port(%d)\n", m_sip, m_ips.GetAt(m_iip), m_ports.GetAt(m_iport));
	m_sock = new Csocket(this);
	if (m_sock->Open(m_ips.GetAt(m_iip), (int)m_ports.GetAt(m_iport)))
	{
		setGuide(_T("서버 연결중........."));
		SetTimer(TM_CONNECT, m_wait, NULL);
	}
	else
	{
		delete m_sock;
		m_sock = NULL;
		MessageBox("소켓 생성오류", "소켓오류", MB_ICONSTOP);
		CDialog::OnCancel();
	}
}

BOOL CGlbDlg::sendTR()
{
	BOOL	rc;
	char*	sndB;
	int	size, sndL;

	Sleep(m_write);
	size = m_data.GetLength();
	sndL = L_fmH + L_axisH + m_data.GetLength();
	sndB = new char[sndL];

	ZeroMemory(sndB, sndL);
	struct _fmH* fmH = (struct _fmH *)sndB;
	fmH->fmF[0] = fmF_FS;
	fmH->fmF[1] = fmF_FS;
	fmH->fmC  = fmC_SSM;
	fmH->ssM  = ssM_WS;
	fmH->stat = stat_WS;
	sprintf(fmH->datL, "%05d", L_axisH + size);

	struct _axisH* axisH = (struct _axisH *)&sndB[L_fmH];
	axisH->msgK = msgK_AXIS;
	axisH->winK = winK_NORM;
//	CopyMemory(axisH->trxC, "pibowglb", sizeof(axisH->trxC));
	CopyMemory(axisH->trxC, "pibfwglb", sizeof(axisH->trxC));	// 20180220  조상님 요청으로 변경
	sprintf(axisH->datL, "%05d", size);

	CopyMemory(&sndB[L_fmH+L_axisH], m_data, size);
	rc = m_sock->Write(sndB, sndL);
	delete[] sndB;
	return rc;
}

void CGlbDlg::dispatch(char* pBytes, int nBytes)
{
	struct	_axisH*	axisH;

	pBytes += L_fmH;
	nBytes -= L_fmH;
	axisH = (struct _axisH *)pBytes;
	pBytes += L_axisH;
	nBytes -= L_axisH;
	nBytes = min(nBytes, atoi(CString(axisH->datL, sizeof(axisH->datL))));
	m_returns = CString(pBytes, nBytes);

	CDialog::OnOK();
}

void CGlbDlg::clearSocket()
{
	if (m_sock)
	{
		m_sock->Close();
		delete m_sock;
		m_sock = NULL;
	}
}

bool CGlbDlg::loadServer(CString path, bool staff)
{
	CString	keys, secs;
	char	wb[256];
	DWORD	rc;

	m_ips.RemoveAll();
	m_ports.RemoveAll();
	m_iip = m_iport = m_sip = 0;
	m_url.Empty();

	if (path.IsEmpty())
	{
		GetCurrentDirectory(sizeof(wb), wb);
		path += wb;
	}
	path += "\\axglb.ini";

	GetPrivateProfileString(_T("GLB"), _T("Wait"), "2000", wb, sizeof(wb), path);
	m_wait = atoi(wb);

	if (staff)
		secs = _T("Staff_Server_IP");
	else
		secs = _T("Server_IP");
	for (int ii = 0; ; ii++)
	{
		keys.Format("%02d", ii);
		rc = GetPrivateProfileString(secs, keys, "", wb, sizeof(wb), path);
		if (rc <= 0)
			break;

		m_ips.Add(wb);
	}

	if (staff)
		secs = _T("Staff_Server_PORT");
	else
		secs = _T("Server_PORT");
	for (ii = 0; ; ii++)
	{
		keys.Format("%02d", ii);
		rc = GetPrivateProfileString(_T("Server_PORT"), keys, "", wb, sizeof(wb), path);
		if (rc <= 0)
			break;

		m_ports.Add(atoi(wb));
	}

	GetPrivateProfileString(_T("GLB"), _T("Url"), "", wb, sizeof(wb), path);
	m_url = wb;

	GetPrivateProfileString(_T("GLB"), _T("SendSec"), "250", wb, sizeof(wb), path);
	m_write = atoi(wb);

	if (m_ips.GetSize() > 0 && m_ports.GetSize() > 0)
	{
	//	m_sip = GetTickCount() % m_ips.GetSize();
	//	m_iip = m_sip;
		return true;
	}

	return false;
}

void CGlbDlg::setGuide(CString guide)
{
	GetDlgItem(IDC_GUIDE)->SetWindowText(guide);
}
