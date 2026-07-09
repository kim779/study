// Connector.cpp : implementation file
//

#include "stdafx.h"
#include <afxsock.h>
#include "RushTester.h"
#include "Connector.h"
#include "./h/axis.h"
#include "./h/axisfire.h"
#include "./h/axisvar.h"
#include "./h/axisanm.h"
#include "./h/Grid.h"
#include "axErr.h"

//#define DEBUG_CONNECT

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TID_QUERY		200
#define	TID_RESPONSEWAIT	500
#define	auxsCAx		0x80		// 공인인증비밀번호를 재입력/재확인하는 TR구분자(WTS와 axWTS간의 약속)

#define ID_SISEONLY	"SISEONLY_LOGIN"

/////////////////////////////////////////////////////////////////////////////
// CConnector


CConnector::CConnector(CWnd* pParent, int idx)
{
	m_nAbsIndex = idx;
	m_gridAppendMode = false;
	m_gridNextMode = true;
	
	m_connectInfo.m_timeTryConnect	= 0; 
	m_connectInfo.m_timeConnected	= 0;
	m_connectInfo.m_timeLastQuery	= 0;
	m_connectInfo.m_timeLastOub	= 0;
	m_connectInfo.m_timeLastAlert	= 0;

	m_sock = NULL;
	m_xecure = NULL;
	m_certify = NULL;
	m_mode = mtNO;
	m_loginKind = lkUser;
	m_hts	= "D";	// 접속 HTS ("D" 대투, "H" 하나)
	m_autoCaPwd = FALSE;	// 공인인증 비밀번호 자동접속(주문등에서 자동입력)
	m_ca = FALSE;	// 공인인증 or 시세전용 접속

	m_pLedger = NULL;
	m_order = FALSE;
	m_parent = pParent;
}

CConnector::~CConnector()
{
	if (m_sock != NULL) delete m_sock;
	if (m_xecure != NULL) delete m_xecure;
	if (m_certify != NULL) delete m_certify; 
}


BEGIN_MESSAGE_MAP(CConnector, CWnd)
	//{{AFX_MSG_MAP(CConnector)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CConnector, CConnector) 
        ON_EVENT(CConnector, -1, 1, OnFireStream, VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


/////////////////////////////////////////////////////////////////////////////
// CConnector message handlers

int CConnector::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CConnector::OnDestroy() 
{
	Close();

	CWnd::OnDestroy();
}


BOOL CConnector::CreateWnd(int nID)
{
	if (m_sock != NULL) return TRUE;
	if (!CreateEx(0, NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), m_parent, nID, NULL))
	{
		return FALSE;
	}

	m_sock = new CWnd();
	if (!m_sock->CreateControl("AxisSock.SockCtrl.AX2010", NULL, 0, CRect(0), this, 0))
	{
		delete m_sock; m_sock = NULL;
		return FALSE;
	}
	return TRUE;
}

BOOL CConnector::Open(char* ip, int port, char* id, char* pwd, char* capwd, char* opt)
{
	Close();
	clearLoginData();

	m_mode	= mtNO;
	m_sIp	= ip;
	m_nPort	= port;
	m_sId	= id;
	m_sPwd	= pwd;
	m_sCaPwd= capwd;
	m_sOpt	= opt;

	m_autoCaPwd = FALSE;
	m_ca = FALSE;

	return OpenSock(_T("c:\\"), m_sIp, m_nPort);
}

void CConnector::Close()
{
	if (m_nResponseWaitTime > 0)
	{
		for (int ii = 0; ii < m_parQueryData->GetSize(); ii++)
		{
			KillTimer(TID_QUERY + ii);
			KillTimer(TID_RESPONSEWAIT + ii);
		}
	}
	else
	{
		KillTimer(TID_QUERY);
	}

	CloseSock();
	//Sleep(10);

	m_loginKind = lkUser;
	m_mode = mtNO;

	m_sId = "";
	m_sPwd = "";
	m_sCaPwd = "";
	m_ca = FALSE;
	m_order = FALSE;
	m_term = 0;

	m_accounts.RemoveAll();

	if (m_xecure != NULL) 
	{
		delete m_xecure;
		m_xecure = NULL;
	}
	if (m_certify != NULL)
	{
		delete m_certify;
		m_certify = NULL;
	}
}

BOOL CConnector::OpenSock(LPCTSTR home, LPCTSTR ip, long port)
{
	if (m_sock == NULL) return FALSE;

	BOOL retB;
	m_sock->InvokeHelper(DI_OPEN, DISPATCH_METHOD, VT_BOOL, &retB, 
		(BYTE *)VTS_BSTR VTS_BSTR VTS_I4, home, ip, port);
	return retB;
}

void CConnector::CloseSock()
{
	if (m_sock == NULL) return;
	m_sock->InvokeHelper(DI_CLOSE, DISPATCH_METHOD, VT_EMPTY, NULL, VTS_NONE);
}

BOOL CConnector::StartAxis(char* key, long mode)
{
	BOOL retB;
	m_sock->InvokeHelper(DI_AXIS, DISPATCH_METHOD, VT_BOOL, &retB,
		(BYTE*)VTS_I4 VTS_I4, key, mode);
	return retB;
}

BOOL CConnector::SetCertify(BOOL Active)
{
	if (!(m_term & flagCAX) && Active)
	{
		if (!m_certify)
		{
			m_certify = new CWnd();
			if (!m_certify->CreateControl("AxisCertify.CertifyCtrl.AX2010", NULL, 0, CRect(0, 0, 0, 0), this, 0))
			{
				delete m_certify;
				m_certify = NULL;
				sendErr(AE_ECERTIFY_CREATE, "공인인증모듈생성오류");
				return FALSE;
			}
		}

		//if (xcertify)
		//{
		//	  m_certify->InvokeHelper(DI_CAEX, DISPATCH_METHOD, VT_BOOL, (void*)&retv, (BYTE *)(VTS_I4 VTS_I4), NULL, NULL);
		//	  if (!retv)
		//	   return FALSE;
		//}

		if (m_accounts.GetSize() > 0)
		{
			int pos;
			CString val, accounts;
			accounts.Empty();
			for (int ii = 0; ii < m_accounts.GetSize(); ii++)
			{
				val = m_accounts.GetAt(ii);
				pos = val.Find("\t");
				if (pos != -1)
				{
					val = val.Left(++pos);
				}
				accounts += val;
			}

			BOOL	retv;
			m_certify->InvokeHelper(DI_CAEX, DISPATCH_METHOD, VT_BOOL, (void*)&retv, 
				(BYTE *)(VTS_I4 VTS_I4), (long)(LPTSTR)(LPCTSTR)accounts, 0);
			if (!retv)
				return FALSE;
		}

	}
	else if (m_certify)
	{	
		delete m_certify;
		m_certify = NULL;
	}
	return TRUE;
}

BOOL CConnector::Xecure(int helper, char* pBytes, int& nBytes)
{
	BOOL retB;
	m_xecure->InvokeHelper(helper, DISPATCH_METHOD, VT_BOOL, &retB,
		(BYTE *)VTS_I4, VTS_I4, pBytes, nBytes);
	return retB;
}

BOOL CConnector::Certify(char* pBytes, int& nBytes, char* maps)
{
	if (!m_ca)
		return FALSE;

	BOOL	retv;
	m_certify->InvokeHelper(DI_CA, DISPATCH_METHOD, VT_BOOL, (void*)&retv,
						(BYTE *)(VTS_I4 VTS_I4 VTS_I4), pBytes, &nBytes, maps);
	return retv;
}

BOOL CConnector::WriteStream(char* pBytes, long nBytes)
{
	BOOL retB = FALSE;

	if (::IsWindow(m_sock->m_hWnd))
		m_sock->InvokeHelper(DI_WRITE, DISPATCH_METHOD, VT_BOOL, &retB, 
			(BYTE *)VTS_I4 VTS_I4, pBytes, nBytes);
	return retB;
}

void CConnector::OnFireStream(int type_, int pBytes, int nBytes)
{
	switch(type_)
	{
	case FEV_ANM:
		OnAlert(type_, (char*)pBytes);
		break;
	case FEV_AXIS:
		OnRead((char*)pBytes, nBytes);
		break;
	case FEV_CLOSE:
		m_mode = mtNO;
		OnDisconnectMsg(0, 0);
		break;
	case FEV_ERROR:
		break;
	case FEV_PUSH:
		break;
	case FEV_SIZE:
	case FEV_STAT:
		break;
	case FEV_CA:
		OnCertify((char*)pBytes, nBytes);
		break;
	default:
		OnAxis(type_, (char*)pBytes, nBytes);
		break;
	}
}

void CConnector::OnAlert(int type, CString text)
{
	int	key;
	CString	code;

	key = text.Find('\t');
	if (key == -1)
		return;

//	code = text.Left(key++);
//	text = text.Mid(key);

	if (type & stat_TICK)
	{
		type &= stat_TICK;
	}

	OnAlertMsg((long)(LPSTR)(LPCTSTR)text, text.GetLength());
}

void CConnector::OnPush(char* pBytes, int nBytes)
{
	//OutputDebugString("AX   OnPush\n");
}

void CConnector::OnRead(char* pBytes, int nBytes)
{
	struct _axisH *axisH;
	int	axisL, totalL;
	char	*chain;

	totalL = nBytes;
	while (nBytes >= L_axisH)
	{
		axisH = (struct _axisH*) pBytes;
		pBytes += L_axisH;
		nBytes -= L_axisH;

		axisL = atoi(CString(axisH->datL, sizeof(axisH->datL)));
		if (axisL > nBytes)
			break;

		chain = pBytes;
		pBytes += axisL;
		nBytes -= axisL;

		if (axisH->stat & statENC && !Xecure(DI_DEC, chain, axisL))
		{
			continue;
		}
		switch (axisH->msgK)
		{
		case msgK_SIGN: // SignOn
			if (m_mode == mtSIGN)
			{
#ifdef DEBUG_CONNECT
CString sTest;
sTest.Format("AX msgK_SIGN m_mode == mtSIGN");
OutputDebugString(sTest);
#endif
				OnSign(axisH->msgK, chain, axisL);
				break;
			}
		case msgK_AXIS:
		case msgK_TAB:
		case msgK_HTM:
		case msgK_ERR:
			OnAxisOubs(axisH, chain, axisL, totalL);
			break;
		case msgK_ENC:
		case msgK_XCA:
			OnXecure(axisH->unit, chain, axisL);
			break;
		case msgK_RSM:
			break;
		case msgK_TICK:
			OnNotice(chain, axisL); 
			break;
		case msgK_POP:
			break;
		case msgK_ARM:
			break;
		case msgK_AUX:
			OnAlert(0, CString(chain, axisL));
			break;
		case msgK_DIAL:
			sendDialogPAN(axisH->winK, chain);
			break;
		}
	}
}

void CConnector::OnNotice(char* pBytes, int nBytes)
{
	OnNoticeMsg((long)pBytes, nBytes);
}

void CConnector::OnCertify(char* pBytes, int nBytes)
{
	CString	text;
	switch (LOWORD(nBytes))
	{
	case invokeCA:
		Write(msgK_XCA, "AXISENCA", pBytes, HIWORD(nBytes));
		break;
	case encryptCA:
		/*
		text = pBytes;
		if (!text.IsEmpty())
		{
			int	idx = text.Find('\t');
			if (idx == -1)
				idx = text.GetLength();
			text = m_guard->Secure(text.Left(idx), (idx < text.GetLength()) ? text.Mid(idx+1) : _T(""), HIWORD(nBytes));
			strcpy(pBytes, text);
		}
		*/
		break;
	case guideCA:
		/*
		if (pBytes)
			m_guard->SetGuide(pBytes);
		else
			m_guard->SetGuide(HIWORD(nBytes));
		*/
		break;
	case closeCA:
		/*
		PostMessage(WM_USER, MAKEWPARAM(closeAXIS, true));
		*/
		break;
	case htmlCA:
		/*
		m_guard->SendAxis(MAKEWPARAM(htmlVIEW, 0), (LPARAM)pBytes);
		*/
		break;
	}	
}

void CConnector::OnSign(int signK, char *pBytes, int nBytes)
{
	if (nBytes < L_signR)
		return;

	CString	dns;
	struct	_signR *sign;

	sign = (struct _signR *) pBytes;
	sign->guide[sizeof(sign->guide)-1] = '\0';
	m_term = sign->flag;
	if (m_term & flagENX)
	{
		m_term &= ~flagENC;
	}

	if (m_term & flagCAX)
	{
		m_term &= ~flagCA;
	}

	switch (sign->signK)
	{
	case signON:
	case signOX:
		dns = CString(sign->name, sizeof(sign->name));
		readSignR(pBytes);
		addRegistry(&pBytes[L_signR], nBytes - L_signR, dns);
		//m_guard->Sign(signK, (char *)sign, nBytes, dns);
		break;
	case signER:
		{
			//Close();
			CString sMsg;
			sMsg.Format("%s", sign->guide);
			sendErr(AE_SIGNERR, sMsg);
		}
		return;
	}

	if (!SetCertify(m_term & flagCA))
		return;

	if (!(m_term & flagCAX) && sign->flag & flagCA)
	{
		m_mode = mtCA;
		m_ca = TRUE;
		if (DoCertify((char*)(LPCTSTR)dns, dns.GetLength()) != -1)
			return;

		//Close();
		sendErr(AE_ECERTIFY_CANCEL, "인증서 선택이 취소되었습니다.");
		return;
	} else
		m_ca = FALSE;

	RunXecure();
}

void CConnector::OnXecure(int encK, char *pBytes, int nBytes)
{
	switch (encK)
	{
	case encERR:
		if (m_mode == mtCA && CertifyErr(pBytes, nBytes))
			break;
		TRACE(CString(pBytes, nBytes));
		TRACE("\n");
		break;
	case encENC:
		if (m_mode == mtCA)
		{
			switch (DoCertify(pBytes, nBytes))
			{
			case -1:
				//Close();
				sendErr(AE_ECERTIFY_CANCEL, "사용자 인증서 선택을 확인하십시요");

				break;
			case 0:
				RunXecure();
				break;
			}
			break;
		}

		switch (DoXecure(pBytes, nBytes))
		{
		case -1:
			;//Sleep(250);
		case 0:
			Run();
			break;
		default:
			break;
		}

		break;
	}
}

BOOL CConnector::OnAxis(int type, char* pBytes, int variant)
{
	BOOL	retv;
	int	nBytes;
	CString	text, tmps;

	switch (type)
	{
	case FEV_OPEN:
		if (variant)
		{
			//Close();
			sendErr(variant, "Fail to connect");
			return TRUE;
		}

		text = "AXIS";
		pBytes = (char *)(LPCTSTR)text;
		nBytes = modeWS/*modeWTS*/;

		if (StartAxis(pBytes, nBytes))
			;//Sleep(0);
		if (m_loginKind == lkSiseOnly)
		{
			Run();
			return  TRUE;
		}

		retv = SignOn();
		break;
	case FEV_RUN:
		/*
		if (m_mode == mtNO)
			return TRUE;

		if (variant)
		{
			m_mode = mtNO;
			return TRUE;
		}
		nBytes = -1;
		*/
		break;
	default:
		return TRUE;
	}
	//retv = StartAxis(pBytes, nBytes);

	return retv;
}

void CConnector::OnAxisOubs(struct _axisH* axisH, char *pBytes, int nBytes, int totalL)
{
	char	mapN[L_MAPN+1];
	CopyMemory(mapN, axisH->trxC, L_MAPN);
	mapN[L_MAPN] = '\0';

/*
CString s;
s.Format("AX mapN:%s\n", mapN); OutputDebugString(s);
s.Format("AX stat:%d\n", (int)axisH->winK); OutputDebugString(s);
s.Format("AX unit:%d\n", (int)axisH->unit); OutputDebugString(s);
s.Format("AX svcN4:%s\n", (int)axisH->svcN); OutputDebugString(s);
s.Format("AX Length:%d\n", nBytes); OutputDebugString(s);
s.Format("AX Data:%s\n", pBytes); OutputDebugString(s);
if (nBytes > 1200) s.Format("AX Data2:%s\n", pBytes + 1200); OutputDebugString(s);
*/
	OnOubMsg((long)axisH, (long)nBytes, totalL);
}

void CConnector::RunXecure()
{
	if (m_term & flagENC)
	{
		m_mode = mtXEC;
		if (DoXecure(NULL, 0) != -1)
			return;
	}
	Run();
}

int CConnector::DoCertify(char* pBytes, int nBytes)
{
	if (!m_certify)
		return -1;

	long	retv;
	m_certify->InvokeHelper(DI_ONCA, DISPATCH_METHOD, VT_I4, (void*)&retv,
						(BYTE *)(VTS_I4 VTS_I4), pBytes, &nBytes);
	if (nBytes > 0)
	{
		if (Write(msgK_XCA, "AXISENCA", (char *)retv, nBytes))
			return nBytes;
		return -1;
	}
	return nBytes;
}

int CConnector::DoXecure(char* pBytes, int nBytes)
{
	if (m_xecure == NULL)
	{
		m_xecure = new CWnd();
		if (!m_xecure->CreateControl("AxisXecure.XecureCtrl.AX2010", NULL, 0, CRect(0, 0, 0, 0), this, 0))
		{
			delete m_xecure;
			m_xecure = NULL;
			sendErr(AE_CSECURE_CREATE, "보안모듈생성오류");
			return -1;
		}
	}

	long	retv;
	m_xecure->InvokeHelper(DI_XEC, DISPATCH_METHOD, VT_I4, (void*)&retv,
						(BYTE *) VTS_I4 VTS_I4, pBytes, &nBytes);

	switch (nBytes)
	{
	case 0:
		return 0;
	case -1:
		break;
	default:
		if (Write(msgK_ENC, "AXISENCX", (char *)retv, nBytes))
			return nBytes;
		break;
	}
	return -1;
}

void CConnector::Run()
{
	CString infos, sInfoX;	// 계좌정보 + '\n' + mapN + '\n' + INFOX
	infos.Empty();
	for (int ii = 0; ii < m_accounts.GetSize(); ii++)
		infos += m_accounts.GetAt(ii);
	sInfoX.Format("\n%s\n%s", m_mapN, m_infoX);
	infos += sInfoX;

	m_mode = mtRUN;	
	OnRunMsg(MAKEWPARAM(0, infos.GetLength()), (long)(LPTSTR)(LPCTSTR)infos);
}


BOOL CConnector::SignOn()
{
	m_accounts.RemoveAll();
	if (m_sId.IsEmpty())
		return FALSE;

	char wb[128];
	CString ip;
	CString m_uuid;
#if 1 // Prucential
	struct	_signM {
		char	user[30];
		char	pass[16];
		char	uuid[36];
		char    uuidrec[1];
		char	lcip[15];
	} signM;
#endif
#if 0 // Taiwan
	struct	_signM {
		char	user[12];
		char	pass[8];
		char	dats[10];
		char    info[64];	//[256];
	} signM;
#endif
	FillMemory(&signM, sizeof(struct _signM), ' ');
	CopyMemory(signM.user, m_sId,  m_sId.GetLength());
	CopyMemory(signM.pass, m_sPwd, m_sPwd.GetLength());
	CString uuid;

	CopyMemory(signM.uuid, m_suuid, 36);
	CopyMemory(signM.uuidrec, "N", 1);
	CopyMemory(signM.lcip, "10.150.17.157", 13);

	int nBytes = sizeof(struct _signM);
	CopyMemory(wb, &signM, nBytes);
	wb[nBytes] = '\0';
	
//	SetCertify(TRUE);  //test

//	m_certify->InvokeHelper(DI_CAID, DISPATCH_METHOD, VT_EMPTY, NULL, (BYTE *)(VTS_I4), &wb);
//	int iresult = !(m_term & flagENX);
	
	m_mode = mtSIGN;
	return Write(msgK_SIGN, "AXLOGON", (char *)wb, nBytes);
}

BOOL CConnector::Write(int msgK, CString trxC, char* datB, int datL, int key)
{
	char*	sendB;
	struct	_axisH*	axH;

	sendB = new char[L_axisH+datL];
	axH = (struct _axisH *) sendB;
	ZeroMemory(axH, L_axisH);

	axH->msgK = msgK;
	axH->winK = key;
	CopyMemory(axH->trxC, trxC, trxC.GetLength());
	sprintf(axH->datL, "%05d", datL);
	CopyMemory(&sendB[L_axisH], datB, datL);

	if (m_ca && !(m_term & flagCAX) && axH->auxs & auxsCA)
	{
		if (m_loginKind == lkSiseOnly)
		{
			delete [] sendB;
			return FALSE;
		}

		if (!Certify(&sendB[L_axisH], datL, NULL))
		{
			sendErr(AE_ECERTIFY, "사용자 인증서 확인에 실패했습니다");
			delete [] sendB;
			return FALSE;
		}
	}
	
	if (!(m_term & flagENX) && axH->stat & statENC)
	{
		/*
		if (m_loginKind == lkSiseOnly)
		{
			delete [] sendB;
			return FALSE;
		}
		*/
		if (!Xecure(DI_ENC, &sendB[L_axisH], datL))
		{
			sendErr(AE_CSECURE, "암호화에 실패했습니다");
			delete [] sendB;
			return FALSE;
		}
	}

	if (WriteStream(sendB, L_axisH+datL))
	{
		delete [] sendB;
		return TRUE;
	}

	delete [] sendB;
	return FALSE;
}

void CConnector::GetledgerH(struct _ledgerH* pLedgerH, int igubn)
{
	_ledgerH ledger;
	CString strtemp;
	if(igubn == 0)
	{
		strtemp = "FITS-HTS";
		memcpy(pLedgerH->anam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());  

		strtemp = "FITSUsrNmSO";
		memcpy(pLedgerH->snam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());

		strtemp = "selectUsrNm";
		memcpy(pLedgerH->fnam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());
	}
	else if(igubn == 1)
	{
		strtemp = "FITS-HTS";
		memcpy(pLedgerH->anam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());  

		strtemp = "FITSComAttGrpSO"; 
		memcpy(pLedgerH->snam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());

		strtemp = "selectAttGrpList";
		memcpy(pLedgerH->fnam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());
	}
	else if(igubn == 2)
	{
		strtemp = "FITS-HTS";
		memcpy(pLedgerH->anam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());  

		strtemp = "FITSComLoginSO"; 
		memcpy(pLedgerH->snam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());

		strtemp = "menuLog";
		memcpy(pLedgerH->fnam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());
	}
}

BOOL CConnector::SendTr(struct _axisH* axH, char* datB, int datL, int igubn)
{
	if (m_mode != mtRUN) 
	{
		return FALSE;
	}

	char* sendB;
	int nDatL = datL;
	sendB = new char[L_axisH+datL+256 + sizeof(struct  _ledgerH)];
	ZeroMemory(sendB, L_axisH+datL+256+ sizeof(struct  _ledgerH));
	CopyMemory(sendB, axH, L_axisH);

	struct  _ledgerH ledgerH;
	FillMemory(&ledgerH, sizeof(struct  _ledgerH), ' ');
	GetledgerH(&ledgerH, igubn);

	CopyMemory(&sendB[L_axisH], &ledgerH, sizeof(struct  _ledgerH));
	CopyMemory(&sendB[L_axisH + sizeof(struct  _ledgerH)], datB, datL);

	struct _axisH* pAxH = (struct _axisH*)sendB;
	if (m_ca && (pAxH->auxs & auxsCA || pAxH->auxs & auxsCAx))
	{
		pAxH->auxs |= auxsCA;
		if (m_loginKind == lkSiseOnly)
		{
			delete [] sendB;
			return FALSE;
		}

		CString mapInfo;
		if (pAxH->auxs & auxsCAx)
		{
			pAxH->auxs ^= auxsCAx;
			mapInfo = "WTSWTS";
		}else
			mapInfo = "";

		if (!Certify(&sendB[L_axisH], datL, (LPTSTR)(LPCTSTR)mapInfo))
		{
			sendErr(AE_ECERTIFY, "사용자 인증서 확인에 실패했습니다");
			delete [] sendB;
			return FALSE;
		}
	}

	if (!(m_term & flagENX) && pAxH->stat & statENC)
	{
		if (m_loginKind == lkSiseOnly)
		{
			delete [] sendB;
			return FALSE;
		}

		if (!Xecure(DI_ENC, &sendB[L_axisH], datL))
		{
			sendErr(AE_CSECURE, "암호화에 실패했습니다");
			delete [] sendB;
			return FALSE;
		}
	}

	if (nDatL != datL)
	{
		nDatL = datL;
	}

	CString sLen;
	sLen.Format("%05d", nDatL+sizeof(struct  _ledgerH));
	strncpy(&pAxH->datL[0], sLen, sLen.GetLength());

	BOOL retB;
	retB = WriteStream(sendB, L_axisH+datL+sizeof(struct  _ledgerH));
	delete [] sendB;

	return retB;
}

void CConnector::sendErr(int ErrCode, CString ErrText)
{
	OnErrorMsg(ErrCode, (long)(LPTSTR)(LPCTSTR)ErrText);
}

void CConnector::sendDialogPAN(int winK, CString DialogMsg)
{
	OnDialogPanMsg(winK, (long)(LPTSTR)(LPCTSTR)DialogMsg);
}

BOOL CConnector::CertifyErr(char* pBytes, int nBytes)
{
	if (!m_certify)
		return FALSE;

	BOOL	retv;
	m_certify->InvokeHelper(DI_CAERR, DISPATCH_METHOD, VT_BOOL, (void*)&retv,
						(BYTE *)(VTS_I4 VTS_I4), pBytes, nBytes);
	return retv;
}

void CConnector::clearLoginData()
{
	for (int ii = 0; ii < SIGNON_CNT; ii++)
		m_signOn[ii].Empty();
}

void CConnector::readSignR(char* datB)
{
	clearLoginData();
	if (datB == NULL || strlen(datB) <= 0)
	{
		return;
	}

	int	pos;
	CString	string, text;
	struct	_signR*	signR = (struct _signR *)datB;

	m_order = (signR->flag & flagACN) ? FALSE : TRUE;

	text = CString(signR->sign, sizeof(signR->sign));
	pos  = text.Find('\0');
	if (pos != -1)
		text = text.Left(pos);
	text.TrimRight();
	m_AppUser.Format("%s", text);

	text = CString(signR->termN, sizeof(signR->termN));
	pos  = text.Find('\0');
	if (pos != -1)
		text = text.Left(pos);
	text.TrimRight();
	m_AppTerm.Format("%s", text);

/* m_mapN
 * [0] : 부가정보 입력창        (DH000091)
 * [1] : 보안카드 입력창        (DH000092)
 * [2] : 계좌비밀번호 변경창    (DH000093)
 * [3] : 비밀번호변경창         (DH621400)
 */
	m_mapN.Format("%s", CString(signR->mapN, sizeof(signR->mapN)));
	m_infoX.Format("%s", CString(signR->infox, sizeof(signR->infox)));
	text.Format("%s", m_infoX);
	if (text.Find('\t') == -1)
	{
		text = CString(signR->sign, sizeof(signR->sign));
		pos  = text.Find('\0');
		if (pos != -1)
			text = text.Left(pos);
		text.TrimRight();
		m_signOn[dtUser].Format("%s", text);

		text = CString(signR->name, sizeof(signR->name));
		pos  = text.Find('\0');
		if (pos != -1)
			text = text.Left(pos);
		text.TrimRight();
		if (text.IsEmpty())
			text = _T("Anonymous");
		m_signOn[dtName].Format("%s", text);
		m_order = (signR->flag & flagACN) ? FALSE : TRUE;

		return;
	}

	if (m_hts == "D")
	{
		// 대투
		for (int ii = 0; !text.IsEmpty(); ii++)
		{
			pos = text.Find('\t');
			if (pos != -1)
			{
				string = text.Left(pos);
				text   = text.Mid(pos+1);
			}
			else
			{
				string = text;
				text   = _T("");
			}
			m_signOn[ii].Format("%s", string);
		}
	}
	else
	{
		//하나
		for (int ii = 0; !text.IsEmpty(); ii++)
		{
			pos = text.Find('\t');
			if (pos != -1)
			{
				string = text.Left(pos);
				text   = text.Mid(pos+1);
			}
			else
			{
				string = text;
				text   = _T("");
			}
			m_signOn[ii].Format("%s", string);
		}
	}
};

void CConnector::addRegistry(char* datB, int datL, CString& dns)
{
	int idx;
	CString entry, name, account, accounts;
	struct _regH* regH;

	for (int ii; datL > sizeof(struct _regH); )
	{
		regH = (struct _regH *) datB;
		datB += sizeof(struct _regH);
		datL -= sizeof(struct _regH);

		switch (regH->regK)
		{
		case regK_COMBO:
			for (ii = 0; ii < regH->regL; ii++)
			{
				entry = datB;
				datB += entry.GetLength();
				datL -= entry.GetLength();
				datB++; datL--;
			}
			break;
		case regK_ACCNO:
			for (ii = 0; ii < regH->regL; ii++)
			{
				entry = datB;
				datB += entry.GetLength();
				datL -= entry.GetLength();
				datB++; datL--;

				idx = entry.Find('\t');
				if (idx == -1)
					continue;
				name = entry.Left(idx++); // account
				name.TrimRight();
				if (name.IsEmpty())
					continue;
				entry = entry.Mid(idx);  // password

				account.Format("%s%c%s%c", name, '\t', entry, '\t');
				m_accounts.Add(account);
			}
			break;
		case regK_CA:
			dns = CString(datB, regH->regL);
			datB += regH->regL;
			datL -= regH->regL;
			break;
		default:
			return;
		}
	}
}

CString CConnector::getLocalIP()
{
	char szHostName[40] = {0};
	CString sDefault = "127.000.000.001";


	if(::gethostname(szHostName, sizeof(szHostName)) != 0)
		return sDefault;
	if(lstrcmp(szHostName, "") == 0)
		return sDefault;
	HOSTENT FAR* lphostent = ::gethostbyname(szHostName);
	if(lphostent == NULL)
		return sDefault;
	sprintf(szHostName, "%s", inet_ntoa(*((struct in_addr*)lphostent->h_addr)));
	return szHostName;
}

CString CConnector::getMacAddress()
{
	/*
	IP_ADAPTER_INFO AdapterInfo[16];	// Allocate information
						// for up to 16 NICs
	DWORD dwBufLen = sizeof(AdapterInfo);	// Save memory size of buffer

	DWORD dwStatus = GetAdaptersInfo(	// Call GetAdapterInfo
	AdapterInfo,				// [out] buffer to receive data
	&dwBufLen);				// [in] size of receive data buffer
	ASSERT(dwStatus == ERROR_SUCCESS);	// Verify return value is
						// valid, no buffer overflow

	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; // Contains pointer to
						// current adapter info
	do {
		CString sAddr, sIp;
		sAddr.Format("%.2X%.2X%.2X%.2X%.2X%.2X    ", 
			pAdapterInfo->Address[0], pAdapterInfo->Address[1], pAdapterInfo->Address[2],
			pAdapterInfo->Address[3], pAdapterInfo->Address[4], pAdapterInfo->Address[5]);

		return sAddr;

		pAdapterInfo = pAdapterInfo->Next;	// Progress through
						// linked list
	}
	while(pAdapterInfo);                    // Terminate if last adapter
*/
	return "";
}

void CConnector::DoConnect(CString Ip, int Port, CString Opt)
{
	BOOL retB;

	m_connectInfo.m_timeTryConnect = CTime::GetCurrentTime();
	SendMsgToParent(WL_TRYCONNECT);
	retB = Open((LPTSTR)(LPCTSTR)Ip, Port, (LPTSTR)(LPCTSTR)m_sId, (LPTSTR)(LPCTSTR)m_sPwd, 
		(LPTSTR)(LPCTSTR)m_sCaPwd, (LPTSTR)(LPCTSTR)Opt);

	//if (!retB) AfxMessageBox("Fail to Connect To Server");
}

void CConnector::DoCloseConnect()
{
	Close();
}

void CConnector::SetUser(CString Id, CString Pwd, CString uuid)
{
	m_sId = Id;
	m_sPwd = Pwd;
	m_suuid = uuid;
}

void CConnector::InitQuery(CPtrArray* QueryData, int QueryInterval, int ResponseWaitTime)
{
	m_parQueryData   = QueryData;
	m_nQueryInterval = QueryInterval;
	m_nResponseWaitTime = ResponseWaitTime;
}


char* CConnector::GetTrData(int igubn)
{
	CString strtext, strdata;
//	((CWnd*)GetDlgItem(IDC_CB_TRLIST))->GetWindowText(strtext);
	//AfxMessageBox(strtext);

	if(igubn == 0)
	{
		struct mid
		{
			char userid[30];
		};

		char  *pbuf = NULL;
		char  *psub = NULL;

		psub = new char[sizeof(struct mid) + 1];
		memset(psub, 0, sizeof(struct mid) + 1);

		struct mid* pmid = new struct mid;
		FillMemory(pmid, sizeof(struct  mid), ' ');
		strdata.Format(_T("%s"), "fits01");
		strdata.TrimRight();
		memcpy(pmid->userid, (char*)strdata.operator LPCSTR(), strdata.GetLength());
		memcpy(psub, pmid, sizeof(struct mid));
		return psub;
	}
	else if(igubn == 1)
	{
		struct mid
		{
			char userid[30];
		};

		char  *pbuf = NULL;
		char  *psub = NULL;

		psub = new char[sizeof(struct mid) + 1];
		memset(psub, 0, sizeof(struct mid) + 1);
	
		struct mid* pmid = new struct mid;
		FillMemory(pmid, sizeof(struct  mid), ' ');
		strdata.Format(_T("%s"), m_sId.TrimRight());
		strdata.TrimRight();
		memcpy(pmid->userid, (char*)m_sId.operator LPCSTR(), m_sId.GetLength());
		memcpy(psub, pmid, sizeof(struct mid));
		return psub;
	}
	else if(igubn == 2)
	{
		struct usemapmid //사용한 화면
		{
			char userid[30];
			char mapnum[6];
		};
		CString strmap;
		char* psub = NULL;
		strmap = "000003";
		struct usemapmid  *pmid = new struct usemapmid;
		memset(pmid, 0, sizeof(struct usemapmid) + 1);
		memcpy(pmid->userid, m_sId, m_sId.GetLength());
		memcpy(pmid->mapnum, (LPSTR)(LPCTSTR)strmap, strmap.GetLength());
		psub = (char*)pmid;
		return psub;
	}

	return 0;
}

void CConnector::SetQueryData(int icnt, int igubn)
{
	m_iCount = icnt;
	m_iQueryGubn = igubn;
	if(m_iCount > 100)
		m_iCount = 100;
}

BOOL CConnector::DoQuery()
{
	if (m_mode == mtNO)
	{
		return FALSE;
	}
	SendMsgToParent(WL_ENABLEFALSE);
	for (int ii = 0; ii < m_iCount; ii++)
	{
		BOOL	retB;
		int	nBytes;
		char	buf[2048];	
		CString	sData;
		struct	_axisH axH;

		ZeroMemory(&axH, L_axisH);
		axH.msgK = msgK_AXIS;
		axH.auxs = 0;
		axH.winK = 0x20;
		axH.unit = 1 + ii;

		CopyMemory(axH.trxC, "PIBOPFRM", 8);
		sData.Format("%s", GetTrData(m_iQueryGubn));
		if(sData == "0" && sData.GetLength() == 1)
			break;

		nBytes = sData.GetLength();
		CopyMemory(buf, (LPCTSTR)sData, nBytes);
		buf[nBytes] = '\0';

		sData.Format("%05d", nBytes + + sizeof(struct  _ledgerH));
		strncpy(axH.datL, sData, sizeof(axH.datL) );
		//sprintf(axH.datL, "%05d", nBytes);

		m_connectInfo.m_timeLastQuery = CTime::GetCurrentTime();
		retB = SendTr(&axH, buf, nBytes, m_iQueryGubn);  //test
		TRACE("sendTr\n");
		Sleep(m_nQueryInterval);
	}

	SendMsgToParent(WL_ENABLETRUE);
	/*  //test
	if ((m_nQueryInterval > 0) && (m_nResponseWaitTime == 0))
	{
		SetTimer(TID_QUERY, m_nQueryInterval, NULL);
	}

	m_connectInfo.m_timeLastQuery = CTime::GetCurrentTime();
	*/
	return TRUE;
}

BOOL CConnector::DoQuery(int id)
{
	if (m_mode == mtNO)
	{
		return FALSE;
	}

	//m_sQryFile
	BOOL	retB;
	int	nBytes;
	char	buf[2048];	
	CString	sData;
	struct	_axisH axH;
	CQueryData* pQuery = (CQueryData*)m_parQueryData->GetAt(id);

	ZeroMemory(&axH, L_axisH);
	axH.msgK = msgK_AXIS;
	axH.auxs = (pQuery->m_bOop) ? auxsOOP : 0;
	axH.winK = 0x20;
	axH.unit = 1 + id;

	CopyMemory(axH.trxC, pQuery->m_trCode, 8);
	sData.Format("%s", pQuery->m_pData);

	nBytes = sData.GetLength();
	CopyMemory(buf, (LPCTSTR)sData, nBytes);
	buf[nBytes] = '\0';

	sData.Format("%05d", nBytes);
	strncpy(axH.datL, sData, sizeof(axH.datL));
	//sprintf(axH.datL, "%05d", nBytes);

	m_connectInfo.m_timeLastQuery = CTime::GetCurrentTime();
	// SendTr
	retB = SendTr(&axH, buf, nBytes);
	SendMsgToParent(WL_QUERY);

	if ((m_nQueryInterval > 0) && (m_nResponseWaitTime > 0))
		SetTimer(TID_RESPONSEWAIT + id, m_nResponseWaitTime * 1000, NULL);

	m_connectInfo.m_timeLastQuery = CTime::GetCurrentTime();
	return TRUE;
}

void CConnector::OnRunMsg(WPARAM wParam, LPARAM lParam)
{
	char* Data = (char*)lParam;
	long len = HIWORD(wParam);

	SendMsgToParent(WL_RUN);
	m_connectInfo.m_timeConnected = CTime::GetCurrentTime();
	//Sleep(10);
	//DoQuery();  //test
}

void CConnector::OnDisconnectMsg(WPARAM wParam, LPARAM lParam)
{
	short type = LOWORD(lParam);
	Close();
	SendMsgToParent(WL_DISCONNECT);
}

void CConnector::OnOubMsg(WPARAM wParam, LPARAM lParam, int nBytes)
{
	struct _auxH	*auxH;
	CString	guide;
	struct _axisH* axisH = (struct _axisH*)wParam;
	char* Data = (char*)((long)wParam + L_axisH);
	long len = lParam;

	if (m_nResponseWaitTime > 0)
	{
		KillTimer(TID_RESPONSEWAIT + axisH->unit - 1); // kill TID_RESPONSEWAIT
		SetTimer(TID_QUERY + (axisH->unit - 1), m_nQueryInterval, NULL); // settimer TID_QUERY
	}

	if (axisH->stat & statAUX)
	{
		auxH = (struct _auxH *)Data;
		len -= L_auxH;
		if (len < 0) return;

		Data += L_auxH;
		len -= auxH->datL;
		if (len < 0) return;

		guide = CString(Data, auxH->datL);
		Data += auxH->datL;
	}

	//int pos;
	CString sData(Data, len);
	
	m_connectInfo.m_timeLastOub = CTime::GetCurrentTime();
	SendMsgToParent(WL_RECEIVE, nBytes);
	/*
	switch (axisH->unit)
	{
	case 2:
		{
			
			pos = sData.Find('\t');
			if (pos < 0)
				return;
			
			CString sCode;
			sCode = sData.Left(pos++); sData = sData.Mid(pos);

			struct	_gridHo* gridHo;
			gridHo = (struct _gridHo*) &Data[pos];

			m_page = CString(gridHo->page, sizeof(gridHo->page));
			m_save = CString(gridHo->save, sizeof(gridHo->save));
			if (!(gridHo->stat & statNEXT)) m_gridNextMode = false;
		}
		break;
	}
	*/
}

void CConnector::OnAlertMsg(WPARAM wParam, LPARAM lParam)
{
	char* Data = (char*)wParam;
	long len = lParam;

	CString sData(Data, len);

	m_connectInfo.m_timeLastAlert = CTime::GetCurrentTime();
	SendMsgToParent(WL_ALERT);
}

void CConnector::OnNoticeMsg(WPARAM wParam, LPARAM lParam)
{
	char* Data = (char*)wParam;
	long len = lParam;
	CString sData(Data, len);

	SendMsgToParent(WL_NOTICE);
}

void CConnector::OnErrorMsg(WPARAM wParam, LPARAM lParam)
{
	long errCode = wParam;
	char* errText = (char*)lParam;

	CString sErr;
	sErr.Format("[%d][%s]", errCode, errText);
	SendMsgToParent(WL_ERROR, (LPARAM)sErr.operator LPCTSTR());
}

void CConnector::OnDialogPanMsg(WPARAM wParam, LPARAM lParam)
{
	long DialogCode = wParam;
	char* DialogText = (char*)lParam;

	CString sDialog;
	sDialog.Format("[%d][%s]", DialogCode, DialogText);
	//AfxMessageBox("OnDialogPan =>" + sDialog);
}

void CConnector::OnTimer(UINT nIDEvent) 
{
	if (m_nResponseWaitTime > 0)
	{
		if (nIDEvent < TID_RESPONSEWAIT)
		{
			// TID_QUERY:
			KillTimer(nIDEvent); // Kill TID_QUERY
			KillTimer(nIDEvent - TID_QUERY + TID_RESPONSEWAIT); // Kill TID_RESPONSEWAIT
			DoQuery(nIDEvent - TID_QUERY);
		}
		else
		{
			// TID_RESPONSEWAIT
			KillTimer(nIDEvent); // Kill TID_RESPONSEWAIT
			KillTimer(nIDEvent - TID_RESPONSEWAIT + TID_QUERY); // Kill TID_QUERY
			DoQuery(nIDEvent - TID_RESPONSEWAIT);
		}
	}
	else
	{
		switch(nIDEvent)
		{
		case TID_QUERY:
			KillTimer(nIDEvent);
			DoQuery();
			break;
		}

	}
	
	CWnd::OnTimer(nIDEvent);
}

void CConnector::SendMsgToParent(UINT msg, LPARAM lParam)
{
	GetParent()->SendMessage(WM_CONNECTOR_MSG, MAKEWPARAM(msg, m_nAbsIndex), lParam);
}

