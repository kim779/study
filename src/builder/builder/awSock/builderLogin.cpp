// download.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "builderLogin.h"
#include "wSock.h"

#include "../h/mapvar.h"
#include "../h/mainvar.h"
#include "../awWcc/libwcc.h"
//#include "../../h/axisvar.h"


// CDownload

IMPLEMENT_DYNAMIC(CbuilderLogin, CWnd)

CbuilderLogin::CbuilderLogin(CString UserID, CString pass, CString domain)
{
	m_sock = NULL;
	m_alive = false;
	m_done = false;
	m_UserID = UserID;
	m_pass = pass;
	m_domain = domain;

	ZeroMemory(&m_sndH, L_rsmH);
}

CbuilderLogin::~CbuilderLogin()
{
	if (m_sock)
	{
		m_sock->Close();
		delete m_sock;
		m_sock = NULL;
	}
}

BEGIN_MESSAGE_MAP(CbuilderLogin, CWnd)
	ON_MESSAGE(WM_READDATA, OnReadData)
	ON_MESSAGE(WM_OPENEDSOCK, OnOpenedSock)
END_MESSAGE_MAP()

bool CbuilderLogin::Create()
{
	CWnd::Create(NULL, NULL, WS_CHILD, CRect(0), AfxGetMainWnd(), 2);
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOACTIVATE);

	if (!initialize())
	{
		PostMessage(WM_CLOSE);
		return false;
	}
	return true;
}

bool CbuilderLogin::initialize()
{
	if (m_sock)
	{
		m_sock->Close();
		if (m_sock)
			delete m_sock;
	}

	m_sock = new CwSock(this);
	if (!m_sock->Open())
	{
		MessageBox(_T("Can't open Socket"), _T("ERROR"), MB_OK);

		m_sock->Close();
		if (m_sock)
			delete m_sock;
		m_sock = NULL;
		return false;
	}

	m_rcvH = (struct _rsmH *)m_sock->m_recvB;
	return true;
}

LRESULT CbuilderLogin::OnReadData(WPARAM wParam, LPARAM lParam)
{
	CFile	file;
	CString	tmps;
	int	datl = wParam;
	int	rcod = 0;
	CString strErr("");

	if (datl < L_rsmH)
		return 0;

	struct	_rsmH*	rcvH;
	rcvH = (struct _rsmH *)lParam;

	if (rcvH->resK == resk_LOGIN)
	{
		if (rcvH->rcod != '0')
		{
			char* pBuffer = (char*)rcvH + L_rsmH;
			for (size_t ii = 0; ii < 80; ii++)
			{
				strErr.AppendFormat("%c", pBuffer[ii]);
				if (pBuffer[ii] == NULL)
					break;
			}
		}
		else
		{
			TRACE("success\n");
		}
		AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_BUILDERLOGIN, rcvH->rcod), (LPARAM)strErr.operator LPCSTR());
	}
	return 0;
}

LRESULT CbuilderLogin::OnOpenedSock(WPARAM wParam, LPARAM lParam)
{
	if (lParam)
	{
		m_alive = true;
		LoginStart();
	}
	else
	{
		//setProgress(100, 3);
		//finish(true);
	}
	return 0;
}

BOOL CbuilderLogin::LoginStart()
{
	struct	_rsmH* prsmH;
	struct i_login* loginI;
	int len;

	len = L_rsmH + sizeof(struct i_login);
	char *pBuffer = new char[len + 1];
	FillMemory(pBuffer, len, ' ');
	prsmH = (struct _rsmH *)pBuffer;
	loginI = (struct i_login*)(pBuffer + L_rsmH);

	CString sDat;
	sDat.Format("%08d", sizeof(struct i_login));
	//prsmH->dirF = dirF_OUTB;
	//prsmH->resC = resC_ASCII;
	prsmH->resK = resk_LOGIN;
	//prsmH->resF = resF_ONLY;
	//CopyMemory(prsmH->usid, m_UserID, min(sizeof(prsmH->usid), m_UserID.GetLength()));
	//prsmH->mode = 'W';
	//prsmH->rcod = '0';

	CopyMemory(prsmH->datL, sDat, min(sizeof(prsmH->datL), sDat.GetLength()));
	CopyMemory(loginI->usid, m_UserID, min(sizeof(loginI->usid), m_UserID.GetLength()));
	CopyMemory(loginI->pswd, m_pass, min(sizeof(loginI->pswd), m_pass.GetLength()));
	//CopyMemory(loginI->domain, m_domain, min(sizeof(loginI->domain), m_domain.GetLength()));

	BOOL bRet = m_sock->Write(pBuffer, len);
	// false일 경우	
	//if (!m_sock->Write(pBuffer, len))
	//	OfflineMode();
	//else
	//	SetTimer(100, 3000, NULL);

	delete[] pBuffer;
	return bRet;
}

void CbuilderLogin::finish(bool close)
{
	if (close && m_sock)
		m_sock->Close();

	m_done = true;
	if (IsWindow(GetSafeHwnd()))
		PostMessage(WM_USER);
}