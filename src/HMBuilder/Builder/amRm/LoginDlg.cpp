// LoginDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "amRm.h"
#include "LoginDlg.h"
#include "afxdialogex.h"

#include "../mSock/wSock.h"
#include "../h/axisrsm.h"
#include "../h/axisfire.h"
#include "../h/mapvar.h"

// CLoginDlg 대화 상자입니다.

//IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/): CDialog(CLoginDlg::IDD, pParent)
{

	m_pswd = _T("");
	m_userid = _T("");
	m_desc = _T("");

	m_state  = sNONE;
	m_socket = (class CwSock *) NULL;
	m_sendB  = (char *) NULL;
	m_recvB  = (char *) NULL;
	m_guide = _T("");

	m_loginOK = false;

	AfxInitRichEdit2();
}

CLoginDlg::~CLoginDlg()
{
	if (m_socket)	delete m_socket;
	if (m_sendB)	delete[] m_sendB;
	if (m_recvB)	delete[] m_recvB;

}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PSWD, m_pswd);
	DDV_MaxChars(pDX, m_pswd, 16);
	DDX_Text(pDX, IDC_USERID, m_userid);
	DDV_MaxChars(pDX, m_userid, 16);
	DDX_Text(pDX, IDC_GUIDE, m_guide);
	DDV_MaxChars(pDX, m_guide, 80);
	DDX_Text(pDX, IDC_DESC, m_desc);
	DDV_MaxChars(pDX, m_desc, 1024);
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginDlg)
	ON_BN_CLICKED(IDC_LOGIN, &CLoginDlg::OnClickedLogin)
	// }} AFX_MSG_MAP

	ON_MESSAGE(WM_OPENEDSOCK, OnSocketSession)
	ON_MESSAGE(WM_READDATA, OnSocketData)

	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLoginDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CLoginDlg 메시지 처리기입니다.
BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_sendB = new char[szBUFF];
	m_recvB = new char[szBUFF];
	
	m_SRH   = (struct _rsmH *)&m_sendB[0];
	m_RRH   = (struct _rsmH *)&m_recvB[0];
	m_login = (struct _login *)&m_sendB[L_rsmH];

	m_socket = new CwSock(this); m_state = sCREATE;

	int	nLen;
	TCHAR	wb[1024]; GetCurrentDirectory(sizeof(wb), wb);
	CString	path; path.Format("%s/%s", wb, "workshop.ini");

	nLen = GetPrivateProfileString("SERVER", "IP", _T(""), wb, sizeof(wb), path);
	CopyMemory(m_ips, wb, nLen); m_ips[nLen] = 0x00;

	nLen = GetPrivateProfileString("SERVER", "PORT", _T(""), wb, sizeof(wb), path);
	m_port = atoi(CString(wb, nLen));

	return TRUE;	// return TRUE unless you set the focus to a control
			// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CLoginDlg::OnClickedLogin()
{
	UpdateData(true);

	if (m_userid.IsEmpty() || m_pswd.IsEmpty())
	{
		guideMsg("입력항목을 확인바랍니다.");
		return;
	}

	BOOL	rc;

	switch (m_state)
	{
	case sCREATE:
		rc = m_socket->Open(m_ips, m_port);
		if (!rc)
		{
			guideMsg("Fail to Connect!"); return;
		}
		break;
	case sOPEN:
		if (!sendData())
		{
			guideMsg("Fail to Transmit Data."); return;
		}
		break;
	default:
		guideMsg("Fail to Connect.");
		break;
	}
}

void CLoginDlg::OnBnClickedOk()
{
	UpdateData(true);

	if (m_desc.IsEmpty())
	{
		guideMsg("Update Description is Empty!!");
		return;
	}

	close();
	CDialog::OnOK();
}


void CLoginDlg::OnBnClickedCancel()
{
	m_loginOK = false; m_userid = _T(""); m_pswd = _T(""); close();

	CDialog::OnCancel();
}

LRESULT CLoginDlg::OnSocketSession(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case FEV_OPEN:
		if (lParam)
		{
			m_state = sOPEN;
			OnClickedLogin();
			return 0;
		}
		break;
	case FEV_CLOSE:
		close();
		break;
	}
	return 0;
}

LRESULT CLoginDlg::OnSocketData(WPARAM wParam, LPARAM lParam)
{
	if (m_state != sOPEN)
		return 0;

	int	nBytes = LOWORD(wParam);
	char*	pBytes = (char*)lParam;

	CopyMemory(m_recvB, pBytes, nBytes);
	if (toInt(m_RRH->erno, sizeof(m_RRH->erno)) != 0)
	{
		guideMsg(CString(m_RRH->errmsg, sizeof(m_RRH->errmsg)));
		close(); return 0;
	}

	guideMsg("Login OK!!"); m_loginOK = true;
	return 0;
}

bool CLoginDlg::sendData()
{
	CString	tmpx;

	ZeroMemory(m_sendB, szBUFF);

	m_SRH->dirF = dirF_OUTB;
	m_SRH->resK = resK_LOGIN;
	m_SRH->resC = resC_ASCII;
	m_SRH->resF = resF_ONLY;

	tmpx.Format("%08d", sizeof(struct _login));
	CopyMemory(m_SRH->datL, (char *) tmpx.operator LPCSTR(), sizeof(m_SRH->datL));

	CopyMemory(m_login->usid, (char *)m_userid.operator LPCSTR(), min(sizeof(m_login->usid), m_userid.GetLength()));
	CopyMemory(m_login->pswd, (char *)m_pswd.operator LPCSTR(), min(sizeof(m_login->pswd), m_pswd.GetLength()));

	int	sendL = L_rsmH + sizeof(struct _login);
	if (!m_socket->Write(m_sendB, sendL))
		return false;

	return true;
}

void CLoginDlg::close()
{
	if (m_state == sOPEN)
		m_socket->Close();

	m_state = sCREATE;
}

int CLoginDlg::toInt(char* pBytes, int plen)
{
	CString	string = CString(pBytes, plen);
	return atoi((const char*) string.operator LPCSTR());
}

void CLoginDlg::guideMsg(CString string)
{
	m_guide = string; UpdateData(false);
}
