// MSocketCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wsock.h"
#include "Stream.h"
#include "Dgram.h"
#include "Wsh.h"
#include "../h/axisfire.h"
#include "../h/mapvar.H"
#include "../h/axisrsm.h"
#include "MSocktCtrl.h"

// CMSocketCtrl

IMPLEMENT_DYNAMIC(CwSock, CWnd)

CString AFXAPI FORMAT(const char* formatString, ...)
{
	CString	s;

	va_list	valist;
	va_start(valist, formatString);
	s.FormatV(formatString, valist);
	va_end(valist);
	
	s.TrimLeft();
	s.TrimRight();
	return s;
}


CwSock::CwSock(CWnd *pWnd) : CWnd()
{
	m_wnd   = pWnd;
	m_alive = false;
	m_pSock = NULL;
	m_recvB = NULL;
	Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), pWnd, 0);	
}

CwSock::~CwSock()
{
	if (m_recvB)
		delete[] m_recvB;		

	if (m_pSock)
	{
		m_pSock->DestroyWindow();
		delete m_pSock;
	}
}


BEGIN_MESSAGE_MAP(CwSock, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_MESSAGE(WFM_EVENT, OnFireEvent) 
	ON_MESSAGE(WFM_ALERT, OnFireAlert)
	ON_MESSAGE(WFM_PUSH, OnFirePush)
	ON_MESSAGE(WFM_AXIS, OnFireAxis)
	ON_MESSAGE(WFM_STAT, OnFireStat)
	ON_MESSAGE(WFM_WRITE, OnFireWrite)
	ON_MESSAGE(WFM_VERS, OnFireVersion)
END_MESSAGE_MAP()

// CCwSocketCtrl 메시지 처리기입니다.


/////////////////////////////////////////////////////////////////////////////
// CwSock::DoPropExchange - Persistence support

/*
void CwSock::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);
}
*/

/////////////////////////////////////////////////////////////////////////////
// CwSock::OnResetState - Reset control to default state

/*
void CwSock::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange
}
*/

/////////////////////////////////////////////////////////////////////////////
// CwSock message handlers

/*
LRESULT CwSock::OnFireEvent(WPARAM wParam, LPARAM lParam)
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
*/

LRESULT CwSock::OnFireEvent(WPARAM wParam, LPARAM lParam)
{
	long	nBytes = HIWORD(wParam);
	char*   pBytes = (char*)lParam;

	switch(wParam)
	{
	case FEV_OPEN:
		if (nBytes == 0) //success
		{
			m_alive = true;
			m_wnd->SendMessage(WM_OPENEDSOCK, FEV_OPEN, 1);
		}
		else
		{
			m_wnd->PostMessage(WM_OPENEDSOCK, 0, 0);
			m_rtnStr = _T("Socket Error!");
		}
		break;

	case FEV_RUN:
		break;
	case FEV_AXIS:
		if (nBytes)
		{
//			CopyMemory(m_recvB, pBytes, nBytes);
//			m_wnd->SendMessage(WM_READDATA, nBytes, (LPARAM)m_recvB);
			//m_wnd->SendMessage(WM_READDATA, 0, nBytes);
		}
		break;
	case FEV_CLOSE:
		m_wnd->SendMessage(WM_OPENEDSOCK, FEV_CLOSE, 0);
		break;
	case FEV_ERROR:
		break;
	}

	return 0;
}

LRESULT CwSock::OnFireAlert(WPARAM wParam, LPARAM lParam)
{
//	OnStream(FEV_ANM, lParam, (long)wParam);
	return 0;
}

LRESULT CwSock::OnFirePush(WPARAM wParam, LPARAM lParam)
{
	//OnStream(FEV_PUSH, lParam, (long)wParam);
	return 0;
}

LRESULT CwSock::OnFireAxis(WPARAM wParam, LPARAM lParam)
{
	if (wParam)
	{
		m_wnd->SendMessage(WM_READDATA, wParam, lParam);
	}
	return 0;
}

LRESULT CwSock::OnFireStat(WPARAM wParam, LPARAM lParam)
{
//	OnStream(FEV_STAT, lParam, (long)wParam);
	return 0;
}

LRESULT CwSock::OnFireWrite(WPARAM wParam, LPARAM lParam)
{
	//m_stream->WriteData((char *)lParam, (int)wParam);
	return 0;
}

LRESULT CwSock::OnFireVersion(WPARAM wParam, LPARAM lParam)
{
//	OnStream(FEV_VERS, lParam, (long)wParam);
	return 0;
}



int CwSock::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_pSock = new CMSocktCtrl();
	m_pSock->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), this, 10);
	((CMSocktCtrl*)m_pSock)->SetWnd(this);
	CString sPath, sData;

	DWORD dw = 512;
	::GetCurrentDirectory(dw, sPath.GetBuffer(512));
	sPath.ReleaseBuffer();
	sPath += FORMAT("\\workshop.ini");

	GetPrivateProfileString("Environment", "RootDir", "", sData.GetBuffer(256), 256, sPath);
	sData.ReleaseBuffer();
	TRIM(sData);
	m_root = sData;
	sData.Empty();

	GetPrivateProfileString("SERVER", "IP", "", m_sIP.GetBuffer(256), 256, sPath);
	m_sIP.ReleaseBuffer();

	GetPrivateProfileString("SERVER", "PORT", "", sData.GetBuffer(256), 256, sPath);
	sData.ReleaseBuffer();
	m_Port = atoi(sData);

	return 0;
}


void CwSock::OnDestroy()
{
	CWnd::OnDestroy();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_alive)
		((CMSocktCtrl*)m_pSock)->CloseSock();
}


bool CwSock::Open(CString ips, int port)
{
	CString	root;
	BOOL	retvalue = false;

//	m_sock = new CWnd();
//	if (!m_sock->CreateControl(_T("AxisSock.SockCtrl.KTB64"), NULL, 0, CRect(0), this, 1))
//	{
//		AfxMessageBox(_T("Create AxSock.ocx : failed"));
//		return false;
//	}

	if (ips.IsEmpty())
		ips = m_sIP;
		//		ips = (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETIPS);
	if (port < 0)
		port = WRK_PORT;

	if (ips.IsEmpty())
	{
//		m_wnd->SendMessage(WM_STATE, stADDRESS, (LPARAM) 0);
		return false;
	}

//	root = (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR);
//	m_sock->InvokeHelper(DI_OPEN, DISPATCH_METHOD, VT_BOOL, (void *)&retvalue, (BYTE*)(VTS_BSTR VTS_BSTR VTS_I4), root, ips, port);
	if (!((CMSocktCtrl*)m_pSock)->OpenSock(m_root, ips, port))	
	{
		m_alive = false;
		return false;
	}
	/*
	if (m_recvB)
		delete[] m_recvB;

	m_recvB = new char[MAX_IO];
	ZeroMemory(m_recvB, MAX_IO);
	*/
	return true;
}

BOOL CwSock::Write(char* pBytes, int nBytes)
{
	if (!m_alive)
		return FALSE;

//	BOOL	retvalue;
//	ZeroMemory(m_recvB, MAX_IO);
	return ((CMSocktCtrl*)m_pSock)->WriteStream(pBytes, nBytes);
	//m_sock->InvokeHelper(DI_WRITE, DISPATCH_METHOD, VT_BOOL, (void*)&retvalue, (BYTE*)(VTS_I8 VTS_I4), (LONG_PTR)pBytes, nBytes);
}


void CwSock::Close()
{
	if (m_alive)
		((CMSocktCtrl*)m_pSock)->CloseSock();
		//m_sock->InvokeHelper(DI_CLOSE, DISPATCH_METHOD, VT_EMPTY, (void*)NULL, NULL);
}

void CwSock::Login(CString sUserID, CString sPass)
{
	/*
	struct	_rsmH* prsmH;
	struct _login* plog;
	int len;
	
	len = L_rsmH + sizeof(struct _login);
	char *pBuffer = new char[len];
	FillMemory(pBuffer, len, ' ');
	prsmH = (struct _rsmH *)pBuffer;
	plog  = (struct _login*)(pBuffer + L_rsmH);



//	ZeroMemory(prsmH, L_rsmH);
//	ZeroMemory(plog, sizeof(struct _login));

	CString sDat;
	sDat.Format("%08d",  sizeof(struct _login));
	prsmH->dirF = dirF_OUTB;
	prsmH->resC = resC_ASCII;
	prsmH->resK = resK_LOGIN;
	prsmH->resF = resF_ONLY;
	

//	strcpy(plog->usid, sUserID);
//	strcpy(plog->pswd, sPass);
	sprintf(prsmH->datL, "%8s", sDat);
	sprintf(plog->usid, "%-16s", sUserID);
	sprintf(plog->pswd, "%-16s", sPass);


	Write(pBuffer, len);	
//	delete[] pBuffer;
*/
}
