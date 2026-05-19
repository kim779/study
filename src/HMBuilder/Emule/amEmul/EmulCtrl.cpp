// EmulCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EmulCtrl.h"
#include "Client.h"
#include "Key.h"
#include "../h/axisrsm.h"
#include "../h/axisanm.h"
#include "../h/axiserr.h"
#include "MSocktCtrl.h"
// CEmulCtrl

IMPLEMENT_DYNAMIC(CEmulCtrl, CWnd)

CEmulCtrl::CEmulCtrl()
{
	m_mode   = mtNO;
	m_xtype  = xtWS;
	m_pSocket = new CMSocktCtrl();
	m_pGuard = new CGuard(m_pSocket);
}

CEmulCtrl::~CEmulCtrl()
{
	SAFE_DELETEW(m_pSocket);
}


BEGIN_MESSAGE_MAP(CEmulCtrl, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
//	ON_MESSAGE(WM_SOCK, OnStream)
	ON_MESSAGE(WM_SOCK, OnFireEvent)
	ON_MESSAGE(WM_USER, OnFireAxis)
	ON_MESSAGE(WM_GUIDE, OnGuide)
	ON_MESSAGE(WM_ANM, OnFireAlert)
	ON_MESSAGE(WM_LUASCRIPT, OnLuaSend)
	ON_MESSAGE(WM_LUAOBJECT, OnLuaObj)
	ON_MESSAGE(WM_LUAGETNAME, OnLuaGetName)
	ON_MESSAGE(WM_LUAGETMAIN, OnLuaGetMain)	
END_MESSAGE_MAP()


LRESULT CEmulCtrl::OnFireAxis(WPARAM wParam, LPARAM lParam)
{
	OnFire(FEV_AXIS, wParam, lParam);
	return 0;
}

LRESULT CEmulCtrl::OnGuide(WPARAM wParam, LPARAM lParam)
{
	if (lParam)
		OnFire(FEV_ERROR, wParam, lParam);
	else
		OnFire(FEV_GUIDE, LOWORD(wParam), HIWORD(wParam));
	return 0;
}

LRESULT CEmulCtrl::OnFireAlert(WPARAM wParam, LPARAM lParam)
{
	OnFire(FEV_ANM, wParam, lParam);
	return 0;
}

// 루아 스크립트 메시지
LRESULT CEmulCtrl::OnLuaSend(WPARAM wParam, LPARAM lParam)
{
	if (m_pGuard)
	{
		switch(HIWORD(wParam))
		{
		case 0:			//	LUA_SEND
			m_pGuard->LuaSend(wParam, lParam);
			break;

		case 20:		//	LUA_VERIFY
			m_pGuard->xTRACE(x_LUAs, (char *)lParam);
			break;
		case 5:		//	LUA_PUSH
		case 6:		//	LUA_POP
			m_pGuard->LuaSystem(wParam, lParam);
			break;
		}
	}
	return 0;
}

// 루아 GetObject 메시지
LRESULT CEmulCtrl::OnLuaObj(WPARAM wParam, LPARAM lParam)
{
	if (m_pGuard)
		m_pGuard->LuaGetObj(wParam, lParam);
	return 0;
}
// GetName
LRESULT CEmulCtrl::OnLuaGetName(WPARAM wParam, LPARAM lParam)
{
	if (m_pGuard)
		m_pGuard->LuaGetName(wParam, lParam);
	return 0;
}

LRESULT CEmulCtrl::OnLuaGetMain(WPARAM wParam, LPARAM lParam)
{
	if (m_pGuard)
		m_pGuard->LuaGetMain(wParam, lParam);
	return 0;
}

LRESULT CEmulCtrl::OnFireEvent(WPARAM wParam, LPARAM lParam)
{
	// 소켓에서 받는것 처리 ...
	int type = LOWORD(wParam);
	int nBytes = HIWORD(wParam);
	char* pBytes = (char*)lParam;

	switch (type)
	{
	case FEV_ANM:
		OnAlert(pBytes, nBytes);
		break;
	case FEV_PUSH:
		OnPush(pBytes, nBytes);
		break;
	case FEV_AXIS:
		OnRead(pBytes, nBytes);
		break;
	case FEV_ERROR:
		OnFire(type, 0, (LPARAM)pBytes);
		if (m_mode == mtSIGN)
			OnFire(FEV_AXIS, MAKEWPARAM(loginERR, axisERR), 0);
		break;
	case FEV_CLOSE:
		m_mode = mtNO;
		OnFire(FEV_CLOSE, 0, 0);
		break;
	case FEV_STAT:
	case FEV_SIZE:
		if (m_mode != mtRUN)
			OnFire(type, nBytes, (LONG_PTR)pBytes);
		break;
	case FEV_VERS:
		m_pGuard->UpdateVers(pBytes, nBytes);
		break;
	case FEV_CA:
//		OnCertify(pBytes, nBytes);
		break;
	default:
		OnAxis(type, pBytes, nBytes);
		break;
	}

	
	return 0;
}

/*
// CEmulCtrl 메시지 처리기입니다.
LRESULT CEmulCtrl::OnStream(WPARAM wParam, LPARAM lParam) 
{ 
	::AfxGetMainWnd()->SendMessage(WM_USER + 1000, wParam, lParam);

	return 0;
}
*/

int CEmulCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	if (m_pSocket->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), this, 0))
	{
		((CMSocktCtrl*)m_pSocket)->SetWnd(this);
		m_pGuard->Initial(this);
	}
	Run();
	return 0;
}

BOOL CEmulCtrl::axTR(char* pBytes, long nBytes)
{
//	return m_pGuard->Write(pBytes, nBytes, winK_FMX);
	return TRUE;
}

long CEmulCtrl::axAttach(CWnd* view, long type, long key) 
{
	return m_pGuard->Attach((CWnd*)view, type, key);
}

void CEmulCtrl::axDetach(long key) 
{
	m_pGuard->Detach(key);
}

BOOL CEmulCtrl::axWorkShop(long key, CString maps, long size, BOOL only) 
{
	CWorks*	works;

	if (!m_pGuard->GetClient(key, works))
		return FALSE;

	int aaa = LOWORD(size);
	aaa = HIWORD(size);

	works->m_view->GetClientRect(&works->m_rect);
	works->m_rect.right  = works->m_rect.left + LOWORD(size);
	works->m_rect.bottom = works->m_rect.top + HIWORD(size);
	return works->Attach(maps, only ? true : false);
}

void CEmulCtrl::Run()
{
//	CString ip = "121.126.21.14";
//	CString port = "15105";	
//	RunAxis(loginSHOP, (LONG_PTR)ip.GetBuffer(0), 15101);	
//	Login();
		
	m_mode = mtRUN;
	m_pGuard->Startup();
	//OnFireAxis(runAXIS, 0);
}


long CEmulCtrl::axWorkView(LPCTSTR maps, long* size) 
{
	return m_pGuard->WorkView(maps, (int *)size);
}


long CEmulCtrl::axRunView(CWnd *view, LPCTSTR maps, int nID, long*size)
{
	int key;
	long nType = axWorkView(maps, size);
	key = axAttach(view, nType, nID);
	axWorkShop(key, maps, *size, false);

	return key;
}


void CEmulCtrl::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_pGuard->Cleanup();
	
	SAFE_DELETEW(m_pSocket);
	delete m_pGuard;
}


BOOL CEmulCtrl::RunAxis(long mode, LONG_PTR pBytes, long nBytes) 
{
	BOOL	retv = TRUE;

	switch (mode)
	{
	case loginSHOP:
		m_xtype = xtRM;
		break;
	case loginAXIS:
		m_xtype = xtWS;
		break;
	case loginAXISx:
		m_xtype = xtXEC;
		break;

	case signUSER:
		switch (m_mode)
		{
		case mtCON:
			break;
		case mtRUN:
			m_pGuard->CertifyId((char *)pBytes, true);
		case mtNO:
		default:
			return TRUE;
		}
		m_pGuard->CertifyId((char *)pBytes);
		if (m_pGuard->Login((char *)pBytes, nBytes, m_xtype == xtXEC))
		{
			m_mode = mtSIGN;
			return TRUE;
		}
		return FALSE;

	case listenUDP:
		if (nBytes < 0)
		{
//			m_pGuard->m_sock->InvokeHelper(DI_DCLOSE, DISPATCH_METHOD, VT_EMPTY, NULL, VTS_NONE);
			((CMSocktCtrl*)m_pSocket)->CloseDgram();
			return TRUE;
		}
		m_pGuard->m_ips = (char *)pBytes;
//		m_pGuard->m_sock->InvokeHelper(DI_DOPEN, DISPATCH_METHOD, VT_BOOL, (void*) &retv,
//									(BYTE *)VTS_I4, nBytes);

		((CMSocktCtrl*)m_pSocket)->OpenDgram(nBytes);
		return retv;

	case signUSERx:
		m_pGuard->Startup();
		return TRUE;

	case listenCAST:
		if (nBytes < 0)
		{
			m_pGuard->CloseCast();
			return TRUE;
		}
		return m_pGuard->OpenCast((char*) pBytes);
	default:
		return FALSE;
	}

	if (nBytes == 0 || (pBytes && !strcmp((char *)pBytes, "0.0.0.0")))
	{
		m_pGuard->Startup();
		PostMessage(WM_USER, runAXIS);
		return TRUE;
	}

	m_pGuard->Cleanup();
	if (nBytes < 0)
		return TRUE;

//	m_pGuard->m_sock->InvokeHelper(DI_OPEN, DISPATCH_METHOD, VT_BOOL, (void*) &retv,
//			(BYTE *)(VTS_BSTR VTS_BSTR VTS_I4), m_pGuard->m_root, (char *)pBytes, nBytes);

	retv = ((CMSocktCtrl*)m_pSocket)->OpenSock(m_pGuard->m_root,(char*)pBytes, nBytes);

	return retv;
}


LONG_PTR CEmulCtrl::axWizard(long kind, LONG_PTR variant) 
{
	CWorks* works;
	static CString	tmps;

	switch (LOWORD(kind))
	{
	case loadACC:
		m_pGuard->GetAccounts(tmps);
		return (LONG_PTR)(char *)tmps.operator LPCTSTR();
	case saveACC:
		m_pGuard->SetAccounts((char *)variant);
		break;
	case setGID:
		if (m_pGuard->GetClient(LOWORD(variant), works))
			works->SetGID(short(HIWORD(variant)));
		break;
	case getFDC:
		tmps.Empty();
		if (m_pGuard->GetClient(HIWORD(kind), works))
			works->GetAt((char *)variant, tmps);	
		return (LONG_PTR)(char *)tmps.operator LPCTSTR();
	case setFDC:
		if (HIWORD(kind) & 0xff00)
		{
			if (m_pGuard->GetClient(HIWORD(kind) & 0x00ff, works))
			{
				int	idx;
				tmps = (char *)variant;
				if ((idx = tmps.Find('\t')) != -1)
					m_pGuard->OnTrigger(works, (char *)tmps.Left(idx).operator LPCTSTR(), tmps.Mid(idx+1));
			}
			break;
		}
		if (m_pGuard->GetClient(HIWORD(kind), works))
		{
			int	idx;
			tmps = (char *)variant;
			if ((idx = tmps.Find('\t')) != -1)
				works->OnTrigger(tmps.Left(idx), tmps.Mid(idx+1), -1);
			else
				works->OnTrigger(_T(""), tmps, -1);
		}
		break;
	case setFCB:
		if (variant)
			m_pGuard->m_status |= WS_FCB;
		else
			m_pGuard->m_status &= ~WS_FCB;
		m_pGuard->SetFCB(variant ? true : false, m_pGuard->m_flash);
		break;
	case setPAL:
		m_pGuard->SetPalette(HIWORD(kind) ? true : false);
		break;
	case devEV:
		if (m_pGuard->GetClient(HIWORD(kind), works) && works->isWorks())
			((CClient *)works)->OnDevice();
		break;
	case homeEV:
		if (m_pGuard->GetClient(HIWORD(kind), works) && works->isWorks())
			((CClient *)works)->m_keyx->OnHome(false);
		break;
	case setVID:
		if (!LOWORD(variant))
			m_pGuard->m_vid = short(HIWORD(variant));
		else if (m_pGuard->GetClient(LOWORD(variant), works))
			works->m_vid = short(HIWORD(variant));
		break;
	case setTRG:
		m_pGuard->SetTriggerMode(HIWORD(kind), HIWORD(variant));
		break;
	case setFONT:
		return m_pGuard->SetFont(HIWORD(variant));
	case encPASS:
		tmps = (char *)variant;
		if (!tmps.IsEmpty())
		{
			int	idx = tmps.Find('\t');
			if (idx == -1)
				idx = tmps.GetLength();
			tmps = m_pGuard->Secure(tmps.Left(idx), (idx < tmps.GetLength()) ? tmps.Mid(idx+1) : _T(""), HIWORD(kind));
		}
		return (long)tmps.operator LPCTSTR();
	case setSCN:
		//if (m_pGuard->GetClient(HIWORD(kind), works) && works->isWorks())
		//	((CClient *)works)->SetLedger((char *)variant);
		break;
	case setFONTx:
		return m_pGuard->SetFont(HIWORD(variant), true, (short)HIWORD(kind));
	case applyACC:
		m_pGuard->OnAccount();
		break;
	case getCA:
		m_pGuard->GetCetifyInfo(tmps);
		return (LONG_PTR)(char *)tmps.operator LPCTSTR();
	case setFLOAT:
		if (m_pGuard->GetClient(HIWORD(kind), works))
			works->SetFloating(variant ? true : false);
		break;
	case setCAv:
		m_pGuard->m_certifyVer = HIWORD(kind);
		break;
	case setLINK:
		if (variant)
			m_pGuard->m_status |= WS_LINK;
		else
			m_pGuard->m_status &= ~WS_LINK;
		break;
	case optSOCK:
		m_pGuard->OptionRTM(LOWORD(variant) ? true : false);
		break;
	case filterRTM:
		m_pGuard->FilterRTM((char *)variant);
		if (m_pGuard->m_cast)
			m_pGuard->m_cast->FilterRTM((char*) variant);
		break;
	case setINTERVAL:
		if (m_pGuard->m_cast)
			m_pGuard->m_cast->SetInterval((int)variant);
		break;
	case setGOPTION:
		m_pGuard->SetGridOption((short)HIWORD(kind), (char *)variant);
		break;
	case setNDC:
		m_pGuard->PushData((char *)variant);
		break;
	case setTRACE:
		m_pGuard->xTRACE(HIWORD(kind), (HWND)variant);
		break;
	}
	return 0;
}

BOOL CEmulCtrl::OnAxis(int type, char* pBytes, int variant)
{
//	BOOL	retv;
	int	nBytes;
	CString	text, tmps;

	switch (type)
	{
	case FEV_OPEN:
		if (variant)
		{
			OnFire(type, 0, variant);
			return TRUE;
		}

		Sleep(250);	// wait for establish
		nBytes = (m_xtype == xtRM) ? modeWS : modeRM;
		m_mode = mtCON;
		break;

	case FEV_RUN:
		if (m_mode == mtNO)
			return TRUE;

		if (variant)
		{
			m_mode = mtNO;
			OnFire(type, 0, variant);
			return TRUE;
		}
		nBytes = modeWS;
		break;

	default:
		return TRUE;
	}
//	m_pGuard->m_sock->InvokeHelper(DI_AXIS, DISPATCH_METHOD, VT_BOOL, (void*)&retv,
//							(BYTE*)(VTS_I8 VTS_I4), pBytes, nBytes);


	BOOL bflag = ((CMSocktCtrl*)m_pSocket)->StartAxis((LONG_PTR)pBytes, nBytes);

	/*
	if (m_xtype == xtXEC && type == FEV_RUN)
	{
		m_mode = mtXEC;
		OnFire(FEV_GUIDE, 0, AE_SECURE);
		if (Xecure(NULL, 0))
			return TRUE;

		m_xtype = xtWS;
		m_mode  = mtCON;
	}
	*/

	OnFire(type, 0, variant);
	return bflag;
}

void CEmulCtrl::OnAlert(char* pBytes, int nBytes)
{
	m_pGuard->OnAlert(pBytes, nBytes);
}

void CEmulCtrl::OnAlert(int type, char* pBytes, int nBytes)
{
	int	key;
	CString	code, text;

	text = CString(pBytes, nBytes);
//	m_pGuard->xTRACE(x_RTMs, (char*)text.operator LPCTSTR());

	key = text.Find('\t');
	if (key == -1)	return;

//	if (type & stat_TICK)
//		OnFire(FEV_ANM, type & stat_TICK, (long)text.Mid(key+1).operator LPCTSTR());
//	else
//		OnFire(FEV_ANM, type, (long)text.operator LPCTSTR());
	code = text.Left(key++);
	pBytes += key;
	nBytes -= key;
	m_pGuard->OnAlert(code, pBytes, nBytes, type);
}

void CEmulCtrl::OnPush(char* pBytes, int nBytes)
{
	int	anmL;
	struct _anmH*	anmH;

	if (nBytes < L_anmH)
		return;

	anmH = (struct _anmH *)pBytes;
	anmL = atoi(CString(anmH->datL, sizeof(anmH->datL)));
	if (anmL > int(nBytes - L_anmH))
		return;

	switch (anmH->anmK)
	{
	case anmK_ALIVE:
		break;
	case anmK_PUSH:
		m_pGuard->OnPush(CString(&pBytes[L_anmH], anmL));
		break;
	}
}


void CEmulCtrl::OnRead(char* pBytes, int nBytes)
{
	struct _axisH *axisH;
	int	axisL;
	char	*chain;
	CString	text;

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

		if (axisH->stat & statENC && !m_pGuard->Xecure(DI_DEC, chain, axisL))
		{
			OnFire(FEV_GUIDE, axisH->winK, AE_SSECURE);
			continue;
		}

		switch (axisH->msgK)
		{
		case msgK_SIGN:
			OnSign(axisH->msgK, chain, axisL);
			break;
		case msgK_RSM:
			m_pGuard->OnRsm(axisH, chain, axisL);
			break;
		case msgK_AXIS:
		case msgK_TAB:
			switch (axisH->winK)
			{
			case winK_FMX:
				if (OnAux(axisH, chain, axisL))
					OnFire(FEV_FMX, MAKEWPARAM(axisH->unit, axisL), (LPARAM)chain);
				continue;
			case winK_DDE:
				if (OnAux(axisH, chain, axisL))
					m_pGuard->OnDDE(CString(chain, axisL), axisH->unit);
				continue;
			default:
				break;
			}
		case msgK_HTM:
		case msgK_SVC:
		case msgK_APC:
		case msgK_CTRL:
		case msgK_UPF:
		case msgK_DNF:
			OnAxis(axisH, chain, axisL);
			break;
		case msgK_ENC:
		case msgK_XCA:
//			OnXecure(axisH->unit, chain, axisL);
			break;
		case msgK_TICK:
			text = CString(chain, axisL);
			m_pGuard->OnNotice(chain, axisL);
			OnFire(FEV_AXIS, MAKEWPARAM(noticePAN, true), (LPARAM)text.operator LPCTSTR());
			break;
		case msgK_POP:
			OnFire(FEV_AXIS, MAKEWPARAM(alarmPAN, true), (LPARAM)CString(chain, axisL).operator LPCTSTR());
			break;
		case msgK_ARM:
			OnFire(FEV_AXIS, MAKEWPARAM(alarmPAN, false), (LPARAM)CString(chain, axisL).operator LPCTSTR());
			break;
		case msgK_AUX:
			text = CString(chain, axisL);
			OnAlert(0, (char *)text.operator LPCTSTR(), axisL);
			break;
		case msgK_DIAL:
			OnFire(FEV_AXIS, MAKEWPARAM(dialogPAN, axisH->winK), (LPARAM)CString(chain, axisL).operator LPCTSTR());
			break;
		case msgK_ARMx:
			text = CString(chain, axisL);
			OnAlert(stat_SCR, (char *)text.operator LPCTSTR(), axisL);
			break;
		}
	}
}


void CEmulCtrl::OnSign(int signK, char *pBytes, int nBytes)
{
	if (nBytes < L_signR)
		return;

	CString	dns;
	struct	_signR *sign;

	sign = (struct _signR *) pBytes;
	sign->guide[sizeof(sign->guide)-1] = '\0';

	switch (sign->signK)
	{
	case signON:
	case signOX:
		dns = _T("");
		m_pGuard->Sign(signK, (char *)sign, nBytes, dns, m_xtype == xtRM);
		break;
	case signER:
		OnFire(FEV_ERROR, true, (LPARAM)sign->guide);
		if (m_mode == mtSIGN)
		{
			m_mode = mtCON;
			OnFire(FEV_AXIS, MAKEWPARAM(loginERR, signERR), 0);
		}
		else
			OnFire(FEV_AXIS, MAKEWPARAM(loginERR, axisERR), 0);
		return;
	}

	OnFire(FEV_ERROR, 0, (LPARAM)sign->guide);
	if (m_mode != mtSIGN)
		return;

	if (!m_pGuard->Certify(FALSE, sign->flag & flagCA, sign->flag & flagCAX))
	{
		m_mode = mtCON;
		OnFire(FEV_GUIDE, 0, AE_CLEAR);
		OnFire(FEV_AXIS, MAKEWPARAM(loginERR, certifyERR), 0);
		return;
	}

	if (sign->flag & flagCAX)
		sign->flag &= ~flagCA;
	if (sign->flag & flagENX)
		sign->flag &= ~flagENC;

	if (sign->flag & flagCA)
	{
		if (dns.IsEmpty())
		{
			m_mode = mtCON;
			OnFire(FEV_GUIDE, 0, AE_EDN);
			OnFire(FEV_AXIS, MAKEWPARAM(loginERR, certifyERR), 0);
			return;
		}

		m_mode = mtCA;
		OnFire(FEV_GUIDE, 0, AE_CERTIFY);

		CString	url;

		m_pGuard->OnCertify(NULL, 0);
		switch (m_pGuard->OnCertify((char *)dns.operator LPCTSTR(), dns.GetLength()))
		{
		case -1:
			m_mode = mtCON;
			OnFire(FEV_GUIDE, 0, AE_ECERTIFY);
			OnFire(FEV_AXIS, MAKEWPARAM(loginERR, certifyERR), 0);
			break;
		case -3:	// terminate AXIS...reboot
			OnFire(FEV_AXIS, MAKEWPARAM(loginERR, certifyERR), 0);
			PostMessage(WM_USER, MAKEWPARAM(closeAXIS, true));
			break;
		default:
			break;
		}
		return;
	}

//	Xecure();
}


bool CEmulCtrl::OnAux(struct _axisH* axisH, char* &pBytes, int& nBytes)
{
	m_pGuard->xTRACE(x_RCVs, (char *)axisH, L_axisH+nBytes);

	if (axisH->stat & statAUX)
	{
		struct _auxH*	auxH;

		auxH = (struct _auxH *)pBytes;
		nBytes -= L_auxH;
		if (nBytes < 0)
			return false;
		pBytes += L_auxH;
		nBytes -= auxH->datL;
		if (nBytes < 0)
			return false;

		pBytes += auxH->datL;
	}
	return true;
}

void CEmulCtrl::OnAxis(struct _axisH* axisH, char *pBytes, int nBytes)
{
	char	mapN[L_MAPN+1];
	CopyMemory(mapN, axisH->trxC, L_MAPN);
	mapN[L_MAPN] = '\0';

	CWorks* works;
	if (!m_pGuard->GetClient(axisH->winK, works))
	{
		if (axisH->stat & statNEW)
		{
			if (!axisH->winK)
				axisH->winK = m_pGuard->GetWorks();
			m_pGuard->SendAxis(MAKEWPARAM(renVIEW, axisH->winK), (LPARAM)mapN);
		}

		if (!m_pGuard->GetClient(axisH->winK, works))
		{
			m_pGuard->SetGuide((axisH->stat & statNEW) ? AE_EWINDOW : AE_XWINDOW);
			return;
		}
	}
	
	if (axisH->auxs & auxsCLOSE)
	{
		m_pGuard->PostAxis(MAKEWPARAM(delVIEW, axisH->winK), (LPARAM)0);
		return;
	}

	if (axisH->unit == unitMAIN && axisH->auxs & auxsMAP &&
			memcmp(mapN, works->m_mapN, L_MAPN) && mapN[0] > ' ')
	{
		if (!works->Attach(mapN, true))
			return;
	}

	m_pGuard->xTRACE(x_RCVs, (char *)axisH, L_axisH+nBytes);
	works->OnStream(axisH, pBytes, nBytes);
}