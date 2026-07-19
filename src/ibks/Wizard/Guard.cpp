// Guard.cpp: implementation of the CGuard class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <afxadv.h>
#include "excel9.h"
#include "Wizard.h"
#include "Guard.h"
#include "Client.h"
#include "Script.h"
#include "Stream.h"
#include "Dde.h"
#include "Tips.h"
#include "Dll.h"
#include "Mouse.h"
#include "Objects.h"
#include "xsystem.h"
#include "Printer.h"
#include "../h/axisvar.h"
#include "../h/axisanm.h"
#include "../h/axiserr.h"
#include "../h/axstring.h"
#include "../dll/form/fmgrid.h"
#include <future>
#include <thread>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	HISTORYs	100		// history entry # 20 -> 100 (091217 ibk)
#define IDC_HAND	MAKEINTRESOURCE(32649)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define DF_NOENC1

CGuard::CGuard()
{
	m_app     = AfxGetApp();
	m_sock    = new CWnd();
	m_certify = NULL;
	m_xecure  = NULL;
	m_symbol  = NULL;
	m_loginx  = NULL;
	m_dialog  = NULL;
	m_secure  = NULL;
	m_secure2 = NULL;
//	m_interest = NULL;

	m_dde     = new CDde(this);
	m_draw    = new CAxisDraw();

	m_mapN.Empty();
	m_user.Empty();
	m_wait    = 0;
	m_flash   = 0;
	m_term    = 0;
	m_status  = WS_FCB;
	m_uname   = _T("Anonymous");

	m_absS   = 0;
	ZeroMemory(m_incS, sizeof(m_incS));
	ZeroMemory(m_excS, sizeof(m_excS));

	RemoveRegistry();

	m_verpass.Empty();
	m_alive = false;

	m_vers.RemoveAll();

	m_xsystem = new CxSystem(this);
	m_login   = NULL;
	m_objects = NULL;

	m_trigger = (trgGROUP|trgVRS);
	m_font    = 9;
	m_resize  = true;	// change false to true (20080730)

	m_vid     = 0;
	m_tips    = new CTips(this);
	m_hTrace  = NULL;

	m_CODEs   = 6;				// default size of symbol
	m_master.Empty();

	for (int ii = winK_NORM; ii < winK_END; ii++)
		m_major.Add(ii);
	m_cursors.RemoveAll();
	m_rtm.RemoveAll();
	m_interests.RemoveAll();

	m_fds      = NULL;
	axFDSValue = NULL;

	ZeroMemory(m_encID, sizeof(m_encID));
}

CGuard::~CGuard()
{
	int	key;
	CWorks*	works;
	POSITION pos;

	for (pos = m_clients.GetStartPosition(); pos; ) 
	{
		m_clients.GetNextAssoc(pos, key, works);
		delete works;
	}
	m_clients.RemoveAll();

	CString	name;
	CCode*	code;

	for (pos = m_codex.GetStartPosition(); pos; )
	{
		m_codex.GetNextAssoc(pos, name, (CObject*&)code);
		delete code;
	}
	m_codex.RemoveAll();

	m_push.RemoveAll();
	m_secmt.RemoveAll();

	RemoveRegistry();

	if (m_xecure)
		delete m_xecure;
	if (m_certify)
		delete m_certify; 
	delete m_sock;

	delete m_dde;
	delete m_draw;

	if (m_symbol)
		AfxFreeLibrary(m_symbol);
	if (m_loginx)
		AfxFreeLibrary(m_loginx);
	if (m_dialog)
		AfxFreeLibrary(m_dialog);
	if (m_secure)
		AfxFreeLibrary(m_secure);
	if (m_secure2)
		AfxFreeLibrary(m_secure2);
//	if (m_interest)
//		AfxFreeLibrary(m_interest);

	Cvers*	vers;
	for (pos = m_vers.GetStartPosition(); pos; )
	{
		m_vers.GetNextAssoc(pos, name, (void*&)vers);
		delete vers;
	}
	m_vers.RemoveAll();

	if (m_objects)
		delete m_objects;
	delete m_xsystem;
	delete m_tips;
	m_major.RemoveAll();

	HCURSOR	cur;
	WORD	wkey;
	for (pos = m_cursors.GetStartPosition(); pos; )
	{
		m_cursors.GetNextAssoc(pos, wkey, (void *&)cur);
		::DeleteObject(cur);
	}
	m_cursors.RemoveAll();

	CObject* ob;
	for (pos = m_rtm.GetStartPosition(); pos; )
	{
		m_rtm.GetNextAssoc(pos, wkey, (CObject *&)ob);
		delete ob;
	}
	m_rtm.RemoveAll();

	if (m_fds)
		AfxFreeLibrary(m_fds);
}

int CGuard::Initial(CWnd* control)
{
	control->GetParent()->GetWindowText(m_keys);
	if (m_keys.IsEmpty())
	{
		AfxGetMainWnd()->GetWindowText(m_keys);
		if (m_keys.IsEmpty())
			m_keys = AXISKEY;
	}
	((CWizardApp*)AfxGetApp())->SetRegistry(m_keys);

	m_root = m_app->GetProfileString(ENVIRONMENT, ROOTDIR, "");

	{
		CString slogTrace;
		slogTrace.Format(_T("[logintrace][%s]<%d> m_root read from registry =[%s]"), _T(__FUNCTION__), __LINE__, m_root.GetString());
		OutputDebugString(slogTrace);
	}

	if (m_root.IsEmpty())
		return -1;

	//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	// [KJS] ������ ó��...  ����׽� ������ ó���ϸ� ó�� �ȵ�...
	//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
#ifndef _DEBUG
	auto future = std::async([&]() {	
#endif // !1

		m_xecure = new CWnd();
		if (!m_xecure->CreateControl(_T("AxisXecure.XecureCtrl.IBK2019"), NULL, 0, CRect(0, 0, 0, 0), control, 0))
		{
			m_slog.Format("[logintrace][%s]<%d> m_xecure CreateControl fail err=[%d]", __FUNCTION__, __LINE__, GetLastError());
			OutputDebugString(m_slog);
			delete m_xecure;
			m_xecure = NULL;
		}
		else
		{
			m_slog.Format("[logintrace][%s]<%d> m_xecure CreateControl success", __FUNCTION__, __LINE__);
			OutputDebugString(m_slog);
		}
#ifndef _DEBUG
	});
#endif // !1
//	//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

	if (!m_sock->CreateControl(_T("AxisSock.SockCtrl.IBK2019"), NULL, WS_VISIBLE, CRect(0, 0, 0, 0), control, -1))
	{
		CString slogTrace;
		slogTrace.Format(_T("[logintrace][%s]<%d> m_sock CreateControl fail err=[%d]"), _T(__FUNCTION__), __LINE__, GetLastError());
		OutputDebugString(slogTrace);
		return -1;
	}

	m_certify = new CWnd();
	if (!m_certify->CreateControl(_T("AxisCertify.CertifyCtrl.IBK2019"), NULL, 0, CRect(0, 0, 0, 0), control, 0))
	{
		m_slog.Format("[logintrace][%s]<%d> m_certify CreateControl fail err=[%d]", __FUNCTION__, __LINE__, GetLastError());
		OutputDebugString(m_slog);
		delete m_certify;
		m_certify = NULL;
	}
	else
	{
		m_slog.Format("[logintrace][%s]<%d> m_certify CreateControl success", __FUNCTION__, __LINE__);
		OutputDebugString(m_slog);
	}


	m_dde->Create(NULL, NULL, 0, CRect(0,0,0,0), control, 0);
	m_parent = control;

	m_vtcode.Format(_T("%s\\%s\\%s"), m_root.GetString(), TABDIR, VTCODE);
	m_repository.Format(_T("%s\\%s\\Repository"), m_root.GetString(), TABDIR);
	m_userini.Format(_T("%s\\%s\\%s"), m_root.GetString(), TABDIR, AXISUSER);
	m_modals.Format(_T("%s\\%s\\%s\\modal.ini"), m_root.GetString(), USRDIR, m_uname.GetString());

	{
		CString slogTrace;
		slogTrace.Format(_T("[logintrace][%s]<%d> m_vtcode=[%s] m_repository=[%s] m_userini=[%s]"), _T(__FUNCTION__), __LINE__, m_vtcode.GetString(), m_repository.GetString(), m_userini.GetString());
		OutputDebugString(slogTrace);
	}

	CString	tmps;
// updateXXX_202202
	char	twb[2048];

	GetEnvironmentVariable("path", twb, sizeof(twb));
	tmps.Format(_T("%s;%s\\%s;%s\\%s"), twb, m_root.GetString(), RUNDIR, m_root.GetString(), DEVDIR);
	SetEnvironmentVariable("path", tmps);
	LOG_OUTP(3, "[LOADLIB", "AxisLogin", __FUNCTION__);
	m_loginx = AfxLoadLibrary("AxisLogin.dll");
	if (m_loginx == NULL)
	{
		axLogin		= NULL;
		axLoginData	= NULL;
		axLoginLedger	= NULL;
		axLedger	= NULL;
		axLedgerEx	= NULL;
		axGetLedger	= NULL;
		axSetLedger	= NULL;
		axGetLedgerEx	= NULL;
		axGetLedgerData	= NULL;
	}
	else
	{
		axLogin		= (void* (APIENTRY*)(void*))GetProcAddress(m_loginx, _T("axLogin"));
		axLoginData	= (void* (APIENTRY*)(int))GetProcAddress(m_loginx, _T("axLoginData"));
		axLoginLedger   = (void  (APIENTRY*)(void*))GetProcAddress(m_loginx, _T("axLoginLedger"));
		axLedger        = (void* (APIENTRY*)(void*))GetProcAddress(m_loginx, _T("axLedger"));
		axLedgerEx      = (void  (APIENTRY*)(void*, void*))GetProcAddress(m_loginx, _T("axLedgerEx"));
		axGetLedger     = (void* (APIENTRY*)(void*, int, int))GetProcAddress(m_loginx, _T("axGetLedger"));
		axSetLedger     = (void  (APIENTRY*)(void*, int, int, void*))GetProcAddress(m_loginx, _T("axSetLedger"));
		axGetLedgerEx   = (void* (APIENTRY*)(void*, int))GetProcAddress(m_loginx, _T("axGetLedgerEx"));
		axGetLedgerData = (void* (APIENTRY*)(void*, void*, int))GetProcAddress(m_loginx, _T("axGetLedgerData"));
		m_login = (CCmdTarget *)axLogin(NULL);
	}

	m_objects = new CObjects(m_root);
	m_tips->Create(control);

	isMenu(true);
#ifndef _DEBUG
//	//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//	// [KJS] ������ ���������� ��ٸ�
//	//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	const std::chrono::milliseconds wtime(50);
	while (future.wait_for(wtime) != std::future_status::ready)
		TRACE("Thread runing");
//	//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
#endif
	return 0;
}

void CGuard::Startup()
{
	if (m_alive)	return;

	void	(APIENTRY* axCreateEx)(void* root);
	LOG_OUTP(3, "[LOADLIB", "AxisCode", __FUNCTION__);
	m_symbol = AfxLoadLibrary("AxisCode.dll");
	if (m_symbol == NULL)
	{
		axCreateEx  = NULL;
		axGetCode   = NULL;
		axGetName   = NULL;
		axGetUpInfo = NULL;
		axGetInfo   = NULL;
	}
	else
	{
		axCreateEx  = (void (APIENTRY*)(void*))GetProcAddress(m_symbol, _T("axCreateEx"));
		axGetCode   = (bool (APIENTRY*)(int, char*, char*, int, CPoint))GetProcAddress(m_symbol, _T("axGetCode"));
		axGetName   = (bool (APIENTRY*)(int, char*, char*, int*))GetProcAddress(m_symbol, _T("axGetName"));
		axGetUpInfo = (bool (APIENTRY*)(char*, char*, char*, int*))GetProcAddress(m_symbol, _T("axGetUpInfo"));
		axGetInfo   = (int  (APIENTRY*)(int, char*, char*, int*))GetProcAddress(m_symbol, _T("axGetInfo"));
	}

	if (axCreateEx)
		axCreateEx((void*)m_root.operator LPCTSTR());

	LOG_OUTP(3, "[LOADLIB", "AxisDialog", __FUNCTION__);
	m_dialog = AfxLoadLibrary("AxisDialog.dll");
	if (m_dialog == NULL)
		axDialog = NULL;
	else
		axDialog = (void* (APIENTRY*)(int, char*, char*, char*, char*))GetProcAddress(m_dialog, _T("axDialog"));

	CString	tmps = _T("");

	tmps.Format(_T("%s\\%s\\%s"), m_root.GetString(), TABDIR, PALETTE);
	m_palette = new CAxisPalette((char *)tmps.operator LPCTSTR());
	m_flash  = GetPrivateProfileInt(GENERALSN, FLASHSN, 99, tmps);
	int maxCurs  = GetPrivateProfileInt(GENERALSN, "Cursors", 0, tmps);

	m_wtype = m_app->GetProfileInt(ENVIRONMENT, WNDINFO, szMAP);
	m_wtype = HIWORD(m_wtype);

	m_wait = m_app->GetProfileInt(WORKSTATION, TRANTMO, 0);
	m_wait *= 1000;

	char	tmpb[1024]{};
	GetPrivateProfileString(SYSTEMSN, "Pass", "", tmpb, sizeof(tmpb), m_vtcode);
	m_verpass = tmpb;
	m_verpass.MakeUpper();

	char*	(APIENTRY* axGetMaster)(char* root, int* ncode);
	LOG_OUTP(3, "[LOADLIB", "AxisCodx", __FUNCTION__);
	HINSTANCE hCode = AfxLoadLibrary("AxisCodx.dll");
	if (hCode)
	{
		axGetMaster = (char* (APIENTRY*)(char*, int*))GetProcAddress(hCode, _T("axGetMaster"));
		if (axGetMaster)
			m_master = axGetMaster((char *)m_root.operator LPCTSTR(), &m_CODEs);
		AfxFreeLibrary(hCode);
	}


	CString	text, name;
	int	idx;

	text.Format(_T("%s\\%s\\axis.ini"), m_root.GetString(), TABDIR);
	GetPrivateProfileString(_T("Portfolio"), "Symbol", "", tmpb, sizeof(tmpb), text);
	text = tmpb;

	while (!text.IsEmpty())
	{
		idx = text.Find(',');
		if (idx == -1)
		{
			name = text;
			text.Empty();
		}
		else
		{
			name = text.Left(idx++);
			text = text.Mid(idx);
		}
		name.Trim();
		if (name.IsEmpty())
			continue;
		m_interests.SetAt(name, NULL);
	}

	LoadSecuritys();
	m_alive = true;

	// load cursor
	int	ii;
	HCURSOR	hcur = nullptr;
	WORD	wkey;
	for (POSITION pos = m_cursors.GetStartPosition(); pos; )
	{
		m_cursors.GetNextAssoc(pos, wkey, (void *&)hcur);
		::DeleteObject(hcur);
	}
	m_cursors.RemoveAll();

	for (ii = 0; ii < 5;  ii++)
	{
		switch (ii)
		{
		case 0:		// CS_NONE : normal
			hcur = ::LoadCursor(NULL, IDC_ARROW);
			break;
		case 1:		// CS_SORTd : sort_descending
			tmps.Format(_T("%s\\%s\\axsort_dsc.cur"), m_root.GetString(), IMAGEDIR);
			hcur = ::LoadCursorFromFile(tmps);
			break;
		case 2:		// CS_SORTa : sort_ascending
			tmps.Format(_T("%s\\%s\\axsort_asc.cur"), m_root.GetString(), IMAGEDIR);
			hcur = ::LoadCursorFromFile(tmps);
			break;
		case 3:		// CS_POP : not cursor
			continue;
		case 4:		// CS_IBEAM
			hcur = ::LoadCursor(NULL, IDC_IBEAM);
			break;
		}
		if (hcur != nullptr) m_cursors.SetAt(ii, hcur);
	}

	for (ii = 0; ii < maxCurs; ii++)	// user-defined cursor
	{
		switch (ii)
		{
		case 0:
			hcur = ::LoadCursor(NULL, IDC_HAND);
			break;
		default:
			tmps.Format(_T("%s\\%s\\axuser%d.cur"), LPCTSTR(m_root), IMAGEDIR, ii);//+1);
			hcur = ::LoadCursorFromFile(tmps);
			break;
		}
		if (hcur != nullptr) m_cursors.SetAt(CS_CURSOR+ii, hcur);
	}
}

void CGuard::LoadHistory()
{
	int	idx;
	CString	name, text, arrays, tmps;
	CCode*	code;
	char	tmpb[512]{};

	SendAxis(MAKEWPARAM(userNAME, 0), (LPARAM)tmpb);
	tmps = CString(tmpb, strlen(tmpb));

	m_userini.Format(_T("%s\\%s\\%s\\%s"), m_root.GetString(), USRDIR, tmps.GetString(), AXISUSER);
	GetPrivateProfileString(SYSTEMSN, SAVECODE, "", tmpb, sizeof(tmpb), m_userini);
	arrays = tmpb;

	while (!arrays.IsEmpty())
	{
		idx = arrays.Find(',');
		if (idx == -1)
		{
			name = arrays;
			arrays.Empty();
		}
		else
		{
			name = arrays.Left(idx++);
			arrays = arrays.Mid(idx);
		}
		name.Trim();
		if (name.IsEmpty()) continue;

		GetPrivateProfileString(SYSTEMSN, name, "", tmpb, sizeof(tmpb), m_userini);
		text = tmpb;
		code = new CCode();

		while (!text.IsEmpty())
		{
			idx = text.Find(',');
			if (idx == -1)
			{
				tmps = text;
				text.Empty();
			}
			else
			{
				tmps = text.Left(idx++);
				text = text.Mid(idx);
			}
			tmps.Trim();
			if (!tmps.IsEmpty())
			{
				if (code->m_codes.Add(tmps) >= HISTORYs)
					break;
			}
		}
		if (code->m_codes.GetSize())
			m_codex.SetAt(name, code);
		else
			delete code;
	}
}

void CGuard::Cleanup()
{
	if (m_alive)
	{
		CString	name, namex, text, arrays;
		CCode*	code = NULL;
		int	kospi = 0;

		arrays.Empty();
		for (POSITION pos = m_codex.GetStartPosition(); pos; )
		{
			m_codex.GetNextAssoc(pos, name, (CObject*&)code);
			if (code->m_self)
			{
				if (!arrays.IsEmpty())
					arrays += ',';
				arrays += name;

				text.Empty();
				for (int key = 0; key < code->m_codes.GetSize() && key < HISTORYs; key++)
				{
					if (GetName(codeNAME, code->m_codes.GetAt(key), namex, kospi)
						|| GetName(whichNAME, code->m_codes.GetAt(key), namex, kospi))
					{
						if (!text.IsEmpty())
							text += ',';
						text += code->m_codes.GetAt(key);
					}
				}
				WritePrivateProfileString(SYSTEMSN, name, text, m_userini);
			}
			delete code;
		}
		WritePrivateProfileString(SYSTEMSN, SAVECODE, arrays, m_userini);
		m_codex.RemoveAll();

		SetPush(false);
		delete m_palette;
	}
	m_sock->InvokeHelper(DI_CLOSE, DISPATCH_METHOD, VT_EMPTY, NULL, VTS_NONE);

	if (m_xecure)
	{
		delete m_xecure;
		m_xecure = NULL;
	}
	m_alive = false;
}

int CGuard::Attach(CWnd *view, int type, int key)
{
	CWorks* works;
	bool	istrue = false;

	switch (key)
	{
	case WK_NORM:
		key = winK_NORM;
		break;
	case WK_POPUP:
		key = winK_POPUP;
		break;
	default:
		if (key < winK_NORM || key > winK_END)
			key = winK_NORM;
		else if (m_clients.Lookup(key, works))
		{				// already attached
			if (works->m_type == type)
				return key;
			Detach(key);
			istrue = true;
		}
		else	istrue = true;

		break;
	}

	m_sync.Lock();
	if (!istrue)
	{
		int	ii;

		istrue = (key & winK_POPUP) ? true : false;
		for (ii = 0; ii < m_major.GetSize(); ii++)
		{
			key = m_major.GetAt(ii);
			if (!m_clients.Lookup(key, works))
			{
				if (istrue && key & winK_POPUP)
					break;
				if (!istrue && !(key & winK_POPUP))
					break;
			}
		}

		if (ii == m_major.GetSize())
		{
			m_sync.Unlock();
			return WK_NONE;
		}
	}

	switch (type & ~vtypeMSK)
	{
	case vtypeNRM:
		works = (CWorks *)new CClient(this, view, key, type);
		break;
	case vtypeDLL:
	case vtypeGRX:
		works = (CWorks *)new CDll(this, view, key, type);
		break;
	case vtypeVBX:
	default:
		m_sync.Unlock();
		return WK_NONE;
	}

	m_clients.SetAt(key, works);
	m_sync.Unlock();

	return key;
}

UINT OnPurge(void* lpvoid)
{
	MSG	msg;

	for (; ((CClient *)lpvoid)->isPurge(); )
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	Sleep(50);
	delete (CWorks*)lpvoid;
	return 0;
}

void CGuard::Detach(int key)
{
	CWorks* works;

	m_sync.Lock();
	if (m_clients.Lookup(key, works))
	{
		for (int ii = 0; ii < m_major.GetSize(); ii++)
		{
			if (key == m_major.GetAt(ii))
			{
				m_major.RemoveAt(ii);
				m_major.Add(key);
				break;
			}
		}
		m_clients.RemoveKey(key);
		if (works->isWorks() && ((CClient *)works)->isPurge())
			AfxBeginThread(OnPurge, (void*)works);
		else
			delete works;
	}
	m_sync.Unlock();
}

int CGuard::GetWorks()
{
	int	key;
	CWorks* works;

	for (int ii = 0; ii < m_major.GetSize(); ii++)
	{
		key = m_major.GetAt(ii);
		if (!(key & winK_POPUP) && !m_clients.Lookup(key, works))
			return key;
	}
	return -1;
}

void CGuard::SetPush(bool push)
{
	CClient* client;
	int	key;

	CString	pushN = _T("");
	for (POSITION pos = m_clients.GetStartPosition(); pos; )
	{
		m_clients.GetNextAssoc(pos, key, (CWorks*&)client);
		if (client->isWorks() && client->isPush(pushN))
		{
			if (!push)
				return;
			break;
		}
	}

	int	datL = L_anmH;
	char	datB[1024];

	struct	_anmH	*anmH = (struct _anmH *) datB;
	ZeroMemory(anmH, datL);

	anmH->anmK = anmK_PUSH;
	anmH->anmF = push ? 1 : 0;

	if (!pushN.IsEmpty())
	{
		CFile	fileH;
		char	path[128];

		wsprintf(path, _T("%s/%s/%s"), m_root.GetString(), USRDIR, pushN.GetString());
		if (fileH.Open(path, CFile::modeRead|CFile::typeBinary))
		{
			datL = min((int)fileH.GetLength(), sizeof(datB)-L_anmH);
			fileH.Read(&datB[L_anmH], datL);
			fileH.Close();

			wsprintf(path, "%05d", datL);
			CopyMemory(anmH->datL, path, sizeof(anmH->datL));
			datL += L_anmH;
		}
	}
	BOOL	retv;
	m_sock->InvokeHelper(DI_DWRITE, DISPATCH_METHOD, VT_BOOL, (void*)&retv,
				(BYTE *) VTS_BSTR VTS_I4 VTS_I4 VTS_BOOL, m_ips, datB, datL, TRUE);
}

void CGuard::OnPush(CString pushs)
{
	CClient* client;
	int	key;

	xTRACE(x_RTMs, (char*)pushs.operator LPCTSTR());

	m_sync.Lock();
	for (POSITION pos = m_clients.GetStartPosition(); pos; )
	{
		m_clients.GetNextAssoc(pos, key, (CWorks*&)client);
		if (client->isWorks() && client->m_status & S_LOAD)
			client->OnPush(pushs);
	}
	m_sync.Unlock();
	SetPush(false);
}

void CGuard::OnAlert(char* pBytes, int nBytes)
{
	struct	_rtmH*	rtmH;
	int	datL, idx;
	CString	text, code, tmps;
	CString	string, updates;
	CStringArray*	rtmk;
	CMapStringToOb	quote;		// parsed data for MAP
	CMapStringToOb	scroll;	
	CMapStringToString scrolls;	// string data for DLL, CONTROL
	POSITION	pos;

	CdataSet*	rts;
	CObArray*	obs;
	bool	info = true;

	quote.RemoveAll();
	scroll.RemoveAll();
	scrolls.RemoveAll();
	while (nBytes > L_rtmH)
	{
		rtmH = (struct _rtmH *) pBytes;
		datL = atoi(CString(rtmH->datL, sizeof(rtmH->datL)));
		if (datL <= 0)
			break;

		nBytes -= L_rtmH;
		pBytes += L_rtmH;

		if (nBytes < datL)
			break;

		text = CString(pBytes, datL);
		switch (rtmH->rtmK)
		{
		case rtmK_INFO:
			if (text.Find("\t\t") == -1)
			{
				rtmk = new CStringArray;
				for ( ; !text.IsEmpty(); )
				{
					idx = text.Find('\t');
					if (idx == -1)
					{
						tmps = text;
						text = _T("");
					}
					else
					{
						tmps = text.Left(idx++);
						text = text.Mid(idx);
					}
					rtmk->Add(tmps);
					m_slog.Format("\r\n [%c] [%s] ", rtmH->datK, tmps);
					OutputDebugString(m_slog);
				}
				if (rtmk->GetSize() > 0)
				{
					CObject* ob;

					if (m_rtm.Lookup(rtmH->datK, (CObject *&)ob))
						delete ob;
					m_rtm.SetAt(rtmH->datK, rtmk);
				}
				else
					delete rtmk;
			}
			break;

		case rtmK_DATA:
			if (!m_rtm.Lookup(rtmH->datK, (CObject *&)rtmk))
				break;

			idx = text.Find('\t');
			if (idx == -1)
				break;
			code  = text.Left(idx++);

			info  = false;
			// process previous data
			if (rtmH->stat & stat_SCR)
			{
				bool	doing = true;

m_slog.Format("\r\n -----  onalert  code= [%s] scroll cnt =[%d] ", code, scroll.GetSize());
//OutputDebugString(m_slog);
				
				if (scroll.Lookup(code, (CObject *&)obs))
				{

m_slog.Format("\r\n onalert  code= [%s] scroll cnt =[%d] obs cnt = [%d]", code, scroll.GetSize(), obs->GetSize());
//OutputDebugString(m_slog);

					if (obs->GetSize() > 0)
						doing = false;
				}

				if (doing && quote.Lookup(code, (CObject *&)rts))
				{
					if (rts->IsOn())
					{
						updates = MakeString(rts);
						DoRTM(code, 0, rts, NULL, updates);
					}
					rts->SetOn(false);
				}
			}
			else
			{
				if (scroll.Lookup(code, (CObject *&)obs) && obs->GetSize() > 0)
				{
					if (!quote.Lookup(code, (CObject *&)rts))
						rts = NULL;
					if (!scrolls.Lookup(code, updates))
						updates = _T("");
					DoRTM(code, stat_SCR, rts, obs, updates);
					
					rts->SetOn(false);
					for (int ii = 0; ii < obs->GetSize(); ii++)
						delete obs->GetAt(ii);
					obs->RemoveAll();
				}
			}

			if (rtmH->stat & stat_SCR)
			{
				int	index;
				CdataSet* fms{};
				char	*ptr = NULL;
				char	*token, *datb = (char *)(pBytes + idx);

				if (!scroll.Lookup(code, (CObject *&)obs))
				{
					obs = new CObArray;
					obs->RemoveAll();
				}
				if (!quote.Lookup(code, (CObject *&)fms))
					fms = new CdataSet;
				if (!scrolls.Lookup(code, updates))
					updates = _T("");

				rts = new CdataSet;
				string = _T("");

				token = strtokx(datb, '\t', &ptr);
				for (int ii = 0; token && ii < rtmk->GetSize(); ii++)
				{
					index = atoi(rtmk->GetAt(ii));
					if (!rts->IsOn())
					{
						rts->SetOn();
						fms->SetOn();
					}
					rts->SetAt(index, token);
					fms->SetAt(index, token);
					tmps.Format(_T("%s\t%s\t"), LPCTSTR(rtmk->GetAt(ii)), token);
					string += tmps;
					token = strtokx(NULL, '\t', &ptr);
				}

				if (rts->IsOn())
				{
					string += "\n";
					if (obs->GetSize() > 0)
					{
						obs->InsertAt(0, rts);
						updates.Insert(0, string);
					}
					else
					{
						obs->Add(rts);
						updates = string;
					}
				}
				else	delete rts;

				quote.SetAt(code, fms);
				scroll.SetAt(code, obs);
				scrolls.SetAt(code, updates);
			}
			else
			{
				char	*ptr = NULL;
				char	*token, *datb = (char *)(pBytes + idx);

				if (!quote.Lookup(code, (CObject *&)rts))
					rts = new CdataSet;

				token = strtokx(datb, '\t', &ptr);
				for (int ii = 0; token && ii < rtmk->GetSize(); ii++)
				{
					if (!rts->IsOn())
						rts->SetOn();
					rts->SetAt(atoi(rtmk->GetAt(ii)), token);
					token = strtokx(NULL, '\t', &ptr);
				}
				quote.SetAt(code, rts);
			}
			break;
		default:
			TRACE("RTM format error !\n");
			break;
		}

		nBytes -= datL;
		pBytes += datL;
	}

	if (!info)
	{
m_slog.Format("\r\n !!!!!!  onalert  scroll cnt =[%d]  quote cnt =[%d]", scroll.GetSize(), quote.GetSize());
//OutputDebugString(m_slog);
		for (pos = scroll.GetStartPosition(); pos; )
		{
			scroll.GetNextAssoc(pos, code, (CObject *&)obs);
			if (obs->GetSize() > 0)
			{
				if (!quote.Lookup(code, (CObject *&)rts))
					rts = NULL;
				if (!scrolls.Lookup(code, updates))
					updates = _T("");
				DoRTM(code, stat_SCR, rts, obs, updates);
				for (int ii = 0; ii < obs->GetSize(); ii++)
					delete obs->GetAt(ii);
				if (rts)
				{
					delete rts;
					quote.RemoveKey(code);
				}
			}
			delete obs;
		}
		scroll.RemoveAll();
		scrolls.RemoveAll();

		for (pos = quote.GetStartPosition(); pos; )
		{
			quote.GetNextAssoc(pos, code, (CObject *&)rts);
			if (rts->IsOn())
			{
				updates = MakeString(rts);
				DoRTM(code, 0, rts, NULL, updates);
			}
			delete rts;
		}
		quote.RemoveAll();
	}
}

void CGuard::OnAlert(CString code, char* pBytes, int nBytes, int stat)
{
	CString	text, tmps, data;
	CdataSet fms;
	CObArray obs;
	int	pos;

	text = CString(pBytes, nBytes);
	data = text;
	obs.RemoveAll();
	while (!text.IsEmpty())
	{
		pos = text.Find('\n');
		if (pos != -1)
		{
			tmps = text.Left(pos++);
			text = text.Mid(pos);
		}
		else
		{
			tmps = text;
			text = _T("");
		}
		tmps.Trim();
		if (!tmps.IsEmpty())
		{
			SetString((char *)tmps.operator LPCTSTR(), NULL, NULL, &fms);
			if (fms.IsOn())
			{
				if (obs.GetSize() > 0)
					obs.InsertAt(0, &fms);
				else
					obs.Add(&fms);
			}
		}
	}
	DoRTM(code, stat, &fms, &obs, data);
	/*
	if (!fms.IsOn())
		return;
	m_dde->OnAlert(code, text);

	obs.Add(&fms);
	m_sync.Lock();
	for (pos = m_clients.GetStartPosition(); pos; )
	{
		m_clients.GetNextAssoc(pos, key, works);
		if (works->m_status & S_LOAD)
		{
			if (works->isWorks())
				((CClient *)works)->OnAlert(code, text, NULL, &obs, stat);
			else
				works->OnAlert(code, text, stat);
		}
	}
	obs.RemoveAll();
	m_sync.Unlock();
	*/
}

void CGuard::OnNotice(char* pBytes, int nBytes)
{
	int	key;
	CString	tmps, text;
	CdataSet major, minor, fms;
	char*	datb;

	text = CString(pBytes, nBytes);
 	tmps = text;
	if (text.Find("w  ") != -1 && text.Find("quote") != -1)	// �ü������ǿ� ������
	{
		CWorks* works;

		m_sync.Lock();
		for (POSITION pos = m_clients.GetStartPosition(); pos; )
		{
			m_clients.GetNextAssoc(pos, key, works);
			if (works->m_status & S_LOAD)
			{
				if (!works->isWorks())
					works->OnNotice(tmps);
			}
		}
		m_sync.Unlock();
		return;
	}

	key = text.Find('\r');
	if (key != -1)
	{
		datb = pBytes;
		datb[key++] = NULL;
		SetString(datb, &major, &minor, &fms);
		text = text.Mid(key);
		pBytes += key;
	}

	key = text.Find('\n');
	if (key != -1)
	{
		datb = pBytes;
		datb[key++] = NULL;
		SetString(datb, NULL, &minor, &fms);
		text = text.Mid(key);
		pBytes += key;
	}

//	datb = pBytes;
	datb = (char *)text.operator LPCTSTR();
	SetString(datb, NULL, NULL, &fms);
	if (!fms.IsOn())
		return;

	CWorks* works;

	m_sync.Lock();
	for (POSITION pos = m_clients.GetStartPosition(); pos; )
	{
		m_clients.GetNextAssoc(pos, key, works);
		if (works->m_status & S_LOAD)
		{
			if (works->isWorks())
				((CClient *)works)->OnNotice(major, minor, fms, tmps);
			else
				works->OnNotice(tmps);
		}
	}
	m_sync.Unlock();
}

void CGuard::SetString(char* datb, CdataSet* major, CdataSet* minor, CdataSet* fms)
{
	int	sym;
	bool	is_symbol = true;
	char	*token, *ptr = NULL;

	token = strtokx(datb, '\t', &ptr);
	while (token)
	{
		if (is_symbol)
			sym = atoi(token);
		else
		{
			if (sym > 0 && sym < maxSYM)
			{
				if (major)
				{
					major->SetOn();
					major->SetAt(sym, token);
				}
				if (minor)
				{
					minor->SetOn();
					minor->SetAt(sym, token);
				}
				if (fms)
				{
					fms->SetOn();
					fms->SetAt(sym, token);
				}
			}
		}
		is_symbol = !is_symbol;
		token = strtokx(NULL, '\t', &ptr);
	}
}

bool CGuard::GetVirtualTR(char *mapN)
{
	char	tmpb[L_MAPN+1]{};
	CString	path;

	if (strcmp(&mapN[L_SGID+L_SELC], "00"))
		return false;
	path.Format(_T("%s\\%s\\axis.ini"), m_root.GetString(), TABDIR);
	GetPrivateProfileString(_T("Select"), CString(&mapN[L_SGID], L_SELC), "", tmpb, sizeof(tmpb), path);
	if (strlen(tmpb) != L_MAPN-L_SGID)
		return false;
	CopyMemory(&mapN[L_SGID], tmpb, L_MAPN-L_SGID);
	return true;
}

void CGuard::SetVersion(CString mapN)
{
	if (!m_mapN.CompareNoCase(mapN))
		m_mapN = _T("");
}

void CGuard::UpdateVersion(CString name)
{
	Cvers*	vers;

	if (m_vers.Lookup(name, (void *&)vers))
	{
		CStdioFile file;
		CString	path, text;

		vers->m_request = false;
		path.Format(_T("%s/%s/infoRSC"), m_root.GetString(), TABDIR);
		if (file.Open(path, CFile::modeWrite))
		{
			file.SeekToEnd();
			text.Format(_T("%s %04d %s %d %s\n"), name.GetString(), 0, vers->m_vers.GetString(), 0, vers->m_path.GetString());
			file.WriteString(text);
			file.Close();
		}		
	}
}

void CGuard::UpdateVers(char *verB, int verL, bool runtime)
{
	CString	name, text;
	Cvers*	vers;
	struct _verM *verM = (struct _verM *) verB;

	text.Empty();
	for (int ii = 0; ii < verL; ii++, verM++)
	{
		name = CString(verM->name);
		name.MakeUpper();
		if (atoi(CString(verM->vers, L_VERS)) == 0)
		{
			m_vers.RemoveKey(name);
			name.Format(_T("%s/%s/%s"), m_root.GetString(), verM->path, name.GetString());
			DeleteFile(name);
			continue;
		}

		if (m_vers.Lookup(name, (void*&)vers))
		{
			if (atoi(CString(verM->vers, L_VERS)) > atoi(vers->m_vers))
			{
				vers->m_vers = CString(verM->vers, L_VERS);
				vers->m_request = true;
			}
			else	continue;
		}
		else
		{
			vers = new Cvers;
			vers->m_path = CString(verM->path);
			vers->m_vers = CString(verM->vers, L_VERS);
			vers->m_request = runtime ? true : verM->request;
			m_vers.SetAt(name, vers);
		}

		if (runtime)
		{
			if (!text.IsEmpty())
				text += '\t';
			text += name;
		}
	}
	if (runtime && !text.IsEmpty())
		m_parent->SendMessage(WM_VERS, text.GetLength(), (LPARAM)text.operator LPCTSTR());
}

bool CGuard::isVersion(CString mapN)
{
	if (!(m_term & flagVER) || !mapN.Mid(L_SGID+L_SELC).CollateNoCase(_AXUS_))
		return GetMapH(mapN) ? true : false;

	mapN.MakeUpper();
//	if (m_verpass.Find(mapN.Left(L_TGID)) != -1 || m_verpass.Find(mapN.Left(L_SGID)) != -1)
//		return true;

	Cvers*	vers;
	if (!m_vers.Lookup(mapN, (void*&)vers))
		return true;		// false->true : for engineer

	if (vers->m_request)
		RequestMAPs(mapN, vers);

	return true;
}

bool CGuard::isVersion(CString mapN, CString prefix)
{
	if (!(m_term & flagVER))
		return true;

	return true;	// prohibit runtime download of DLL (080228)

	CString	fileN;
	Cvers*	vers;

	mapN += prefix;
	mapN.MakeUpper();
	if (m_vers.Lookup(mapN, (void *&)vers))
	{
		if (!vers->m_request)
			return true;
	}
	else	return true;		// false->true : for engineer

/*	fileN.Format("%s\\%s\\%s%s", m_root, DEVDIR, mapN, prefix);
	if (GetVersion(fileN, vers))
	{
		int	idx;
		if (vers.GetLength() > L_VER)
			vers = vers.Left(L_VER);
		idx = vers.ReverseFind('.');
		if (idx != -1)
			vers = vers.Mid(++idx);
		if (version == atoi(vers))
			return true;
	}
*/
	RequestMAPs(mapN, vers);
	return true;
}

bool CGuard::isLevel(CString mapN, CString prefix)
{
	CString	fileN;

	fileN.Format(_T("%s\\%s\\%s%s"), LPCTSTR(m_root), DEVDIR, LPCTSTR(mapN), LPCTSTR(prefix));

	DWORD 	size, value = 0;
	size = GetFileVersionInfoSize(fileN.GetString(), &value);
	if (size <= 0)
		return true;

	char 	*vers;
	VS_FIXEDFILEINFO *vs;

	value = 0;
	vers = new char[size];
	if (GetFileVersionInfo (fileN.GetString(), value, size, (void *)vers))
	{
		if (VerQueryValue(vers, "\\", (void**)&vs, (UINT*)&value))
		{
			if (m_absS < vs->dwFileVersionLS)
			{
				delete [] vers;
				SetGuide(AE_ESECURITY);
				return false;
			}
		}
	}

	delete [] vers;
	return true;
}

void CGuard::RequestMAPs(CString mapN, Cvers* vers)
{
	CString	path;
	struct	_fileH	fileH;

	m_section.Lock();
	SetGuide(AE_MUPDATE);
	m_mapN = mapN;
	path.Format(_T("%s/%s"), vers->m_path.GetString(), mapN.GetString());
	
	ZeroMemory(&fileH, sizeof(struct _fileH));
	fileH.fileK = fileK_MAP;
	CopyMemory(fileH.fileN, (char *)path.operator LPCTSTR(), path.GetLength());	
	if (Write(msgK_RSM, "", (char *)&fileH, sizeof(fileH), 0, true))
	{
		MSG	msg;
		ULONGLONG	elapseV = GetTickCount64();

		while (!m_mapN.IsEmpty())
		{
			if (GetTickCount64() - elapseV > 5000)
				break;

			if (!PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
				continue;

			switch (msg.message)
			{
			case WM_CHAR:
			case WM_KEYDOWN:
			case WM_KEYUP:
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MOUSEMOVE:
			case WM_MOUSEWHEEL:
				break;
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			default:
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				break;
			}
		}
	}

	m_mapN = _T("");
	SetGuide(AE_CLEAR);
	m_section.Unlock();
}

void CGuard::PushData(CfmBase* form, CString keys, int row, int column)
{
	CString	text = _T("");

	form->ReadData(text, false, -1, column, row);
	text.TrimRight();

	if (keys.IsEmpty())
		keys = (char *)form->m_form->name;
	if (text.IsEmpty())
		m_push.RemoveKey(keys);
	else
		m_push.SetAt(keys, text);
}

void CGuard::PushData(CString text)
{
	CString	keys;
	int	idx;

	idx = text.Find('\t');
	if (idx != -1)
	{
		keys = text.Left(idx++);
		text = text.Mid(idx);

		text.TrimRight();
		if (text.IsEmpty())
			m_push.RemoveKey(keys);
		else
			m_push.SetAt(keys, text);
	}
}

void CGuard::PopData(CfmBase* form, CString keys, int row, int column)
{
	CString	text;

	if (keys.IsEmpty())
		keys = (char *)form->m_form->name;

	if (m_push.Lookup(keys, text))
		form->WriteData(text, true, column, row);
}

void CGuard::PopData(CString keys, CString& text)
{
	if (!m_push.Lookup(keys, text))
		text.Empty();
}

bool CGuard::GetMapH(CString mapN, char* mapH)
{
	if (mapN.GetLength() != L_MAPN)
		return false;

	CString pathN = PathMAP(mapN);
	CFile	fileH;
	if (!fileH.Open(pathN, CFile::modeRead|CFile::typeBinary))
		return false;

	int	readL, mapL = (int)fileH.GetLength();
	char *mapB = nullptr, *wb = new char[mapL];
	readL = fileH.Read(wb, mapL);
	if (readL != mapL)
	{
		delete[] wb;
		fileH.Close();
		return false;
	}

	if (!strncmp(&wb[iAXIS], sKEY, iKEY))
	{
		mapB = new char[readL * 4];
		mapL = lz4dec((char*)&wb[iAXIS + iKEY], (readL - iAXIS - iKEY), mapB, readL * 4);
		if (mapL <= 0 || mapL < L_MAPH)
		{
			delete[] wb;
			delete[] mapB;
			return false;
		}
		CopyMemory(mapB, wb, iAXIS);
		delete[] wb;
	}
	else
		mapB = wb;

	if (mapH)
		CopyMemory(mapH, mapB, L_MAPH);

	delete[] mapB;
	fileH.Close();
	return true;
}

void CGuard::Sign(int signK, char* signB, int signL, CString& dns, bool shop)
{
	CString	user, text = _T(""), string;
	struct _signR*	signR;

	/*CString strresult;
	CString stmp;
	char ctemp[sizeof(struct _signR) + 1]{};
	memcpy(ctemp, signB, sizeof(struct _signR));
	for (int ii = 0; ii < sizeof(struct _signR); ii++)
	{
		if (ctemp[ii] == 0)
			stmp = "0";
		else if (ctemp[ii] == 1)
			stmp = "1";
		else if (ctemp[ii] == 20)
			stmp = " ";
		else
			stmp.Format("%s", (char*)&ctemp[ii]);
		strresult += stmp;
	}*/

	signR = (struct _signR*) signB;
	if (!dns.IsEmpty())
		m_user.Empty();

	if (signR->absS)
		m_absS = signR->absS;

	int	idx;
	switch (signR->mask & maskSEC)  
	{//�հ� ������ ��Ģ�� ���ߴ°� ����.
	 //or  ,  and,  xor ���߿� �ϳ� �ϱ�� ������ �����ذ� �����ڷ� �ް�
	//���� incS ���� ������ �����ڷ� ��Ʈ �����Ѵ�.
	//�迭�� ���� 1, 0 ���� ��Ʈ�����ؼ� �־�д�.
	case maskOR:
		for (idx = 0; idx < sizeof(signR->incS); idx++)
		{
			m_incS[idx] |= signR->incS[idx];
			m_excS[idx] |= signR->excS[idx];
		}
		break;
	case maskAND:
		for (idx = 0; idx < sizeof(signR->incS); idx++)
		{
			m_incS[idx] &= signR->incS[idx];
			m_excS[idx] &= signR->excS[idx];
		}
		break;
	case maskXOR:
		for (idx = 0; idx < sizeof(signR->incS); idx++)
		{
			m_incS[idx] ^= signR->incS[idx];
			m_excS[idx] ^= signR->excS[idx];
		}
		break;
	case maskNO:
	default:
		break;
	}

	idx = L_signR;
	signR->guide[sizeof(signR->guide)-1] = '\0';

	switch (signR->signK)
	{
	case signON:
		m_status |= WS_SIGN;
		RemoveRegistry();
		AddRegistry(&signB[idx], signL-idx, dns);
		break;
	case signOX:
		m_status &= ~WS_SIGN;
		RemoveRegistry();
		if (axLogin)
			axLogin("");
		SendAxis(MAKEWPARAM(userINFO, 0), (LPARAM)(char *)text.operator LPCTSTR());
		[[fallthrough]];
	default:
		return;
	}

	if (axLogin)
		text = CString((char *)axLogin(signB));
	if (!text.IsEmpty())
		SendAxis(MAKEWPARAM(userINFO, 0), (LPARAM)(char *)text.operator LPCTSTR());

	user = CString(signR->sign, sizeof(signR->sign));
	idx  = user.Find('\0');
	if (idx != -1)
		user = user.Left(idx);
	user.TrimRight();

	text = CString(signR->info, sizeof(signR->info));
	text.TrimRight();
	for ( ; !text.IsEmpty(); )
	{
		idx = text.Find('\t');
		if (idx != -1)
		{
			string = text.Left(idx);
			text   = text.Mid(idx+1);
		}
		else
		{
			string = text;
			text.Empty();
		}

		idx = string.Find('=');
		if (idx <= 0)
			continue;

		switch (string.GetAt(idx-1))
		{
		case 't':
		case 'T':
			if (m_app->GetProfileInt(WORKSTATION, "ServerTime", 0))
			{
				tm	ltime{};
				SYSTEMTIME st;
				time_t	tmx = (time_t)atoi(string.Mid(idx + 1));
				errno_t	eno = localtime_s(&ltime, &tmx);
// KJS  2022/11/04  ���� ����....
				if (eno == 0)
				{
					GetLocalTime(&st);
					st.wYear = ltime.tm_year + 1900;
					st.wMonth = ltime.tm_mon + 1;
					st.wDay = ltime.tm_mday;
					st.wDayOfWeek = ltime.tm_wday;

					st.wHour = ltime.tm_hour;
					st.wMinute = ltime.tm_min;
					st.wSecond = ltime.tm_sec;
					SetLocalTime(&st);
				}
			}
			break;

		case 'u':
		case 'U':
			user = string.Mid(idx+1);
			break;
		}
	}

	m_status |= WS_SELF;
	if (!m_user.IsEmpty() && m_user.CompareNoCase(user))
		m_status &= ~WS_SELF;
	m_user = user;

	m_uname = CString(signR->name, sizeof(signR->name));
	idx = m_uname.Find('\0');
	if (idx != -1)
		m_uname = m_uname.Left(idx);
	m_uname.TrimRight();

	if (m_uname.IsEmpty())
		m_uname = _T("Anonymous");
	user.Format(_T("%s|%s"), m_user.GetString(), m_uname.GetString());
	m_app->WriteProfileString(WORKSTATION, SIGNONID, user);
	m_modals.Format(_T("%s\\%s\\%s\\modal.ini"), m_root.GetString(), USRDIR, m_uname.GetString());

	m_term = signR->flag;
	m_app->WriteProfileString(WORKSTATION, TERMNAME, CString(signR->termN, sizeof(signR->termN)));

	text = CString(signR->mapN, sizeof(signR->mapN));
	idx = text.Find('\0');
	if (idx != -1)
		text = text.Left(idx);
	text.TrimRight();
	if (text.GetLength() == L_MAPN)
		m_app->WriteProfileString(WORKSTATION, INITMAP, text);
	else
		m_app->WriteProfileString(WORKSTATION, INITMAP, "");

	idx = max(atoi(CString(signR->trx, sizeof(signR->trx))), 0);
	m_app->WriteProfileInt(WORKSTATION, TRANTMO, idx);
	 
	idx = max(atoi(CString(signR->usage, sizeof(signR->usage))), 0);
	m_app->WriteProfileInt(WORKSTATION, USAGETMO, idx);

	idx = max(atoi(CString(signR->idle, sizeof(signR->idle))), 0);
	m_app->WriteProfileInt(WORKSTATION, IDLETMO, idx);

	text = CString(signR->service, sizeof(signR->service));
	idx = text.Find('\0');
	if (idx != -1)
		text = text.Left(idx);
	text.TrimRight();
	m_app->WriteProfileString(WORKSTATION, SERVICEID, text);

	text = CString(signR->menu, sizeof(signR->menu));
	idx = text.Find('\0');
	if (idx != -1)
		text = text.Left(idx);
	text.TrimRight();
	if (!shop)
	{
		isMenu(false);
		LoadMenu(text);
	}
}

void CGuard::AddRegistry(char* datB, int datL, CString& dns)
{
	int	idx, ii, size;
	CString	entry, name;
	struct	_regH*	regH;

	while (datL > sizeof(struct _regH))
	{
		regH = (struct _regH *) datB;
		datB += sizeof(struct _regH);
		datL -= sizeof(struct _regH);

		switch (regH->regK)
		{
		case regK_COMBO:
			m_combo.RemoveAll();
			size = atoi(CString(regH->regL, sizeof(regH->regL)));
			for (ii = 0; ii < size; ii++)
			{
				entry = datB;
				datB += entry.GetLength();
				datL -= entry.GetLength();
				datB++;	datL--;

				idx = entry.Find('\t');
				if (idx == -1)
					continue;
				name = entry.Left(idx++);
				name.TrimRight();
				if (name.IsEmpty())
					continue;
				entry = entry.Mid(idx);
				if (entry.IsEmpty())
					continue;
				m_combo.SetAt(name, entry);
			}
			break;
		case regK_ACCNO:
			size = atoi(CString(regH->regL, sizeof(regH->regL)));
			for (ii = 0; ii < size; ii++)
			{
				entry = datB;
				datB += entry.GetLength();
				datL -= entry.GetLength();
				datB++;	datL--;

				idx = entry.Find('\t');
				if (idx == -1)
					continue;
				name = entry.Left(idx++);	// account
				name.TrimRight();
				if (name.IsEmpty())
					continue;
				entry = entry.Mid(idx);		// password

				lz4enc(entry);
				m_account.SetAt(name, entry);

LOG_OUTP(3, "[axwizard][loadacc]", entry, __FUNCTION__);

				m_accno.Add(name);
			}
			break;
		case regK_CA:
			size = atoi(CString(regH->regL, sizeof(regH->regL)));
			if (size > 0)
			{
				// ignore regH->regL due to extend sizeof _caH
				idx = sizeof(struct _caH);
				dns = CString(datB, idx);
				datB += idx;
				datL -= idx;
			}
			/*
			dns = CString(datB, size);
			datB += size;
			datL -= size;
			*/
			break;
		default:
			return;
		}
	}
}

void CGuard::RemoveRegistry()
{
	m_combo.RemoveAll();
	m_account.RemoveAll();
	m_accno.RemoveAll();

	m_stock_accs.Empty();
	m_future_accs.Empty();
}

void CGuard::GetSignOn(CString& sign, bool state)
{
	if (!state || m_status & WS_SIGN)
	{
		CString	tmps;
		tmps = m_app->GetProfileString(WORKSTATION, SIGNONID, _T(""));
		sign = GetByString(tmps, '|', false);
	}
	else
		sign.Empty();
}

void CGuard::GetBooth(CString& booth)
{
	CString	text;

	text = m_app->GetProfileString(ENVIRONMENT, BOOTH, _T(""));
	booth = GetByString(text, '|');
}

void CGuard::GetBranch(CString& branch)
{
	branch.Format("%d", m_app->GetProfileInt(WORKSTATION, BRANCH, 0));
}

void CGuard::GetTerm(CString& term)
{
	term = m_app->GetProfileString(WORKSTATION, TERMNAME, _T(""));
}

void CGuard::GetHistory(CString name, CString& history, bool tab)
{
	CCode*	code;

	history.Empty();

	if (m_codex.Lookup(name, (CObject*&)code))
	{
		for (int ii = 0; ii < code->m_codes.GetSize(); ii++)
		{
			history += code->m_codes.GetAt(ii);
			if (tab)
				history += '\t';
		}
	}
}

void CGuard::GetCode(CString name, CString& text)
{
	CCode*	code;

	if (m_codex.Lookup(name, (CObject*&)code) && code->m_self && code->m_codes.GetSize())
		text = code->m_codes.GetAt(0);
	else
		text.Empty();
}

void CGuard::GetCode(CMapStringToString& domino, CClient* client)
{
	CString	name;
	CCode*	code;

	if (client && client->GetCode(domino))
		return;

	for (POSITION pos = m_codex.GetStartPosition(); pos; )
	{
		m_codex.GetNextAssoc(pos, name, (CObject*&)code);
		if (code->m_self && code->m_codes.GetSize())
			domino.SetAt(name, code->m_codes.GetAt(0));
	}
}

void CGuard::GetCode(CString& domino)
{
	CString	name;
	CCode*	code;

	domino.Empty();
	for (POSITION pos = m_codex.GetStartPosition(); pos; )
	{
		m_codex.GetNextAssoc(pos, name, (CObject*&)code);
		if (code->m_self && code->m_codes.GetSize())
		{
			domino += name;
			domino += '\t';
			domino += code->m_codes.GetAt(0);
			domino += '\n';
		}
	}
}

void CGuard::SetAccounts(CString entry)
{
	int	idx;

	idx = entry.Find('\t');
	if (idx == -1)
	{
		for (idx = 0; idx < m_accno.GetSize(); idx++)
		{
			if (!m_accno.GetAt(idx).Compare(entry))
			{
				m_accno.RemoveAt(idx);
				break;
			}
		}
		m_account.RemoveKey(entry);
	}
	else
	{
		CString	keys, tmps;

		keys = entry.Left(idx);
		if (!m_account.Lookup(keys, tmps))
			m_accno.Add(keys);
		tmps = entry.Mid(++idx);

LOG_OUTP(3, "[axwizard][loadacc]", tmps, __FUNCTION__);

		lz4enc(tmps);
		m_account.SetAt(keys, tmps);
	}
}

void CGuard::GetAccounts(CString& account)
{
	CString	key, text;

	account.Empty();
//	if (m_term & flagACN)
//		return;

	for (int ii = 0; ii < m_accno.GetSize(); ii++)
	{
		key = m_accno.GetAt(ii);
		if (m_account.Lookup(key, text))
		{
			lz4dec(text);

			account += key;
			account += '\t';
			account += text;
			account += '\n';
		}
	}
}

void CGuard::GetAcName(CString& account, BOOL full)
{
	CString	key, name;

	account.Empty();
//	if (m_term & flagACN)
//		return;
	m_slog.Format("account=[%s] full=[%d]", account, full);
	LOG_OUTP(3, "[axwizard][loadacc]", m_slog, __FUNCTION__);

	for (int ii = 0; ii < m_accno.GetSize(); ii++)
	{
		key = m_accno.GetAt(ii);

		account += key;
		account += '\t';

		//LOG_OUTP(3, "[axwizard][loadacc][key]", key, __FUNCTION__);

		if (full)
		{
			if (m_account.Lookup(key, name))
			{
				lz4dec(name);
				account += name;
			}
		}
		else if (GetAcName(key, name))
			account += name;

		account += '\n';
	}

	LOG_OUTP(3, "[axwizard][loadacc] account = ", account, __FUNCTION__);
}

bool CGuard::GetAcPass(CString key, CString& pass)
{
	if (!m_account.Lookup(key, pass))
		return false;

	lz4dec(pass);
	int idx = pass.Find('\t');
	if (idx != -1)
		pass = pass.Left(idx);
	return true;
}

bool CGuard::GetAcName(CString key, CString& name, bool full)
{
	if (!m_account.Lookup(key, name))
		return false;
	lz4dec(name);

	if (full)
		return true;

	int idx = name.Find('\t');
	if (idx != -1)
	{
		name = name.Mid(++idx);
		idx = name.Find('\t');
		if (idx != -1)
			name = name.Left(idx);
	}
	else
		name.Empty();

	return true;
}

void CGuard::xAlert(CClient* client, CScreen* screen)
{
	if (!m_alive)	return;

	int	sndL;
	char	sndB[1024];
	struct _axisH*	axisH;

	axisH = (struct _axisH*) sndB;
	ZeroMemory(axisH, L_axisH);

	axisH->msgK = msgK_MAPX;
	axisH->winK = client->m_key;

	sndL = client->xAlert(screen, &sndB[L_axisH]);
	if (sndL == 0)
	{
m_slog.Format("[mapkey] sndL=0  client->m_key=[%d] ", client->m_key);
LOG_OUTP(3, "axwizard", __FUNCTION__, m_slog);
		return;
	}

	CString	tmps, strdata;
	tmps.Format("%05d", sndL);
	CopyMemory(axisH->datL, tmps, tmps.GetLength());

	tmps.Empty();
	for (int ii = 0; ii < sndL; ii++)
	{
		tmps.Format("%d,", (unsigned char)*&sndB[L_axisH + ii]);
		strdata += tmps;
	}
	m_slog.Format("[mapkey] client->m_key=[%d] sndB=[%s] sndL=[%d] m_key=[%d] key= [%s]", client->m_key, sndB, sndL,  axisH->winK , strdata);
	LOG_OUTP(3, "axwizard", __FUNCTION__, m_slog);

	Write(sndB, L_axisH+sndL, false);
}

BOOL CGuard::Xecure(int helper, char* pBytes, int& nBytes)
{
	if (m_xecure == NULL)
		return FALSE;
	BOOL	retv;
	m_xecure->InvokeHelper(helper, DISPATCH_METHOD, VT_BOOL, (void*)&retv,
						(BYTE *) VTS_I4 VTS_I4, pBytes, &nBytes);
	return retv;
}

CString	CGuard::Secure(CString src, CString keys, int mode, bool enc)
{
	if (m_secure == NULL)
	{
		char	wb[64];
		CString	string;

		string.Format(_T("%s\\%s\\axis.ini"), m_root.GetString(), TABDIR);
		GetPrivateProfileString("Secure", "file", "", wb, sizeof(wb), string);
		string = wb;
		if (string.IsEmpty())
			return src;

		string.Format(_T("%s\\%s"), m_root.GetString(), wb);
		LOG_OUTP(3, "[LOADLIB", string, __FUNCTION__);
		m_secure = AfxLoadLibrary(string);
		if (m_secure == NULL)
		{
			axEncrypt = NULL;
			axDecrypt = NULL;
		}
		else
		{
			axEncrypt = (char* (APIENTRY*)(char*, char*, int))GetProcAddress(m_secure, _T("axEncrypt"));
			axDecrypt = (char* (APIENTRY*)(char*, char*, int))GetProcAddress(m_secure, _T("axDecrypt"));
		}
	}

	if (enc && axEncrypt)
	{
		int	len;
		
		len = src.GetLength();
		src.TrimRight();
		src = axEncrypt((char *)src.operator LPCTSTR(), (char *)keys.operator LPCTSTR(), mode);
		src += CString(' ', len - src.GetLength());
	}
	else if (!enc && axDecrypt)
	{
		int	len;
		
		len = src.GetLength();
		src.TrimRight();
		src = axDecrypt((char *)src.operator LPCTSTR(), (char *)keys.operator LPCTSTR(), mode);
		src += CString(' ', len - src.GetLength());
	}

	return src;
}

CString	CGuard::Secure2(CString src)
{
	if (m_secure2 == NULL)
	{
		CString	string;

		string.Format(_T("%s\\dev\\cx_sha256.dll"), (char*) m_root.GetString());
		LOG_OUTP(3, "[LOADLIB", string, __FUNCTION__);
		m_secure2 = AfxLoadLibrary(string);
		if (m_secure2 == NULL)
			axEncryptSHA = NULL;
		else
			axEncryptSHA = (char* (APIENTRY*)(char*, int))GetProcAddress(m_secure2, _T("axEncrypt"));
	}

	if (axEncryptSHA)
	{
		char*	pp;
		int	srcL;

		srcL = src.GetLength();
		pp   = axEncryptSHA((char *)src.operator LPCTSTR(), srcL);
		src  = CString(pp);
		return src;
	}
	return _T("");
}

bool CGuard::GetCode(int kind, CString& code, int type, CPoint pt, bool valid)
{
	if (axGetCode)
	{
		char	wb[128] = {0, };
		
		if (valid)
			strcpy_s(wb, sizeof(wb), code);
		if ((*axGetCode)(kind, (char*)(m_uname.GetString()), wb, type, pt))
		{
			code = wb;
			return true;
		}
	}

	return false;
}

bool CGuard::GetName(int kind, CString code, CString& name, int& type)
{
	if (axGetName)
	{
		char	wb[128];

		if ((*axGetName)(kind, (char*)code.operator LPCTSTR(), wb, &type))
		{
			name = wb;
			return true;
		}
	}

	return false;
}

bool CGuard::GetName(int kind, CString code, CString& name)
{
	int	type;

	if (kind == codeNAME || kind == whichNAME)
	{
		int	idx;

		if (code.GetLength() == m_CODEs)
		{
			idx = m_master.Find(code+MASTERs);
			if (idx != -1)
			{
				name = m_master.Mid(idx);
				idx = name.Find('\t');
				if (idx != -1)
					name = name.Left(idx);
				name = name.Mid(code.GetLength());
				name.TrimLeft();
				return true;
			}
		}
	}

	return GetName(kind, code, name, type);
}

bool CGuard::GetBizInfo(CString code, CString& bizcode, CString& bizname, int& market)
{
	if (axGetUpInfo)
	{
		char	bizcd[64];
		char	biznm[64];

		ZeroMemory(bizcd, sizeof(bizcd));
		ZeroMemory(biznm, sizeof(biznm));
		if ((*axGetUpInfo)((char*)code.operator LPCTSTR(), bizcd, biznm, &market))
		{
			bizcode = bizcd;
			bizname = biznm;
			return true;
		}
	}

	return false;
}

int CGuard::GetCodeInfo(int kind, CString code, CString& info, int& value)
{
	int	rc = 0;

	if (axGetInfo)
	{
		char	wb[1024];

		ZeroMemory(wb, sizeof(wb));
		rc = (*axGetInfo)(kind, (char *)code.operator LPCTSTR(), wb, &value);
		info = CString(wb);
	}
	return rc;
}

CString CGuard::PathMAP(CString mapN, bool force)
{
	CString pathN;
	if (mapN.Mid(L_SGID+L_SELC).CompareNoCase(_AXUS_))
	{
		if (force)
		{
			pathN.Format(_T("%s\\%s\\%s"), LPCTSTR(m_root), MAPDIR, LPCTSTR(mapN.Left(L_SGID)));
			if (GetFileAttributes(pathN) == 0xffffffff)
				CreateDirectory(pathN, NULL);

			pathN += "\\";
			pathN += mapN.Left(L_TGID);
			if (GetFileAttributes(pathN) == 0xffffffff)
				CreateDirectory(pathN, NULL);
		}
		pathN.Format(_T("%s\\%s\\%s\\%s\\%s"), LPCTSTR(m_root), MAPDIR,
				LPCTSTR(mapN.Left(L_SGID)), LPCTSTR(mapN.Left(L_TGID)), LPCTSTR(mapN));
	}
	else
		pathN.Format(_T("%s\\%s\\%s\\%s"), LPCTSTR(m_root), USRDIR, LPCTSTR(m_uname), LPCTSTR(mapN));


	return pathN;
}

CString	CGuard::GetByString(CString& src, char by, bool trim)
{
	CString	tmps;

	int idx = src.Find(by);
	if (idx == -1)
	{
		tmps = src;
		src.Empty();
	}
	else
	{
		tmps = src.Left(idx++);
		if (idx < src.GetLength())
			src = src.Mid(idx);
		else
			src.Empty();
	}
	if (trim)
		tmps.TrimRight();
	return tmps;
}

void CGuard::SetCode(CScreen* screen, bool recur, bool self)
{
	CString	text;
	CfmBase* form;

	for (int ii = 0; ii < screen->m_mapH->formN; ii++)
	{
		form = screen->GetAtForm(ii);
// updateXXX_2022XX	
		if (!form || form->m_form->kind != FM_EDIT)
			continue;
		if (form->m_form->properties & PR_CODE && self)
		{
			form->ReadData(text);
			SetCode((char*)form->m_form->name, text, self, (form->m_form->properties & PR_CODEx) ? true : false);
			if (!form->IsVisible() && !screen->isMain() &&
					screen->m_client->GetAtScreen(screen, screen->m_parent))
				SetCode(screen, true, true);
			else
				self = false;
			continue;
		}
		if (!recur && form->m_form->properties & PR_LIST)
		{
			form->ReadData(text);
			if (form->IsVisible() && form->IsEnable())
				SetCode((char*)form->m_form->name, text, false);
		}
	}
}

void CGuard::SetCode(CString text)
{
	int	idx;

	idx = text.Find('\t');
	if (idx != -1)
		SetCode(text.Left(idx), text.Mid(idx+1));
}

bool CGuard::SetCode(CString name, CString text, bool self, bool found)
{
	text.TrimRight();
	if (text.IsEmpty())
		return false;

	CCode*	code;

	if (!m_codex.Lookup(name, (CObject*&)code))
	{
		code = new CCode();
		code->m_self = self;
		m_codex.SetAt(name, code);
	}

	for (int ii = 0; ii < code->m_codes.GetSize(); ii++)
	{
		if (!text.CompareNoCase(code->m_codes.GetAt(ii)))
		{
			if (found)
				return true;
			code->m_codes.RemoveAt(ii);
			break;
		}
	}

	switch (code->m_codes.GetUpperBound())
	{
	case HISTORYs-1:
		code->m_codes.InsertAt(0, text);
		code->m_codes.RemoveAt(HISTORYs);
		break;
	case -1:
		code->m_codes.Add(text);
		break;
	default:
		code->m_codes.InsertAt(0, text);
		break;
	}
	SendAxis(MAKEWPARAM(newHISTORY, 0), (LPARAM)name.operator LPCTSTR());
	return true;
}

CString CGuard::SetCode(CClient* client)
{
	int	idx;
	char	wb[64]{};
	CStringArray	syms;
	CString	text, path, codes = _T("");

	syms.RemoveAll();
	path.Format(_T("%s/%s/symbols.ini"), m_root.GetString(), TABDIR);
	for (idx = 1; ; idx++)
	{
		text.Format("%d", idx);
		GetPrivateProfileString("Symbols", text, "", wb, sizeof(wb), path);
		if (strlen(wb) <= 0)
			break;

		syms.Add(wb);
	}

	codes.Empty();
	if (syms.GetSize() > 0)
	{
		CfmBase* form{};
		CScreen* screen{};

		for (idx = 0; idx < client->m_magic; idx++)
		{
			if (!client->GetAtScreen(screen, idx))
				continue;

			for (int ii = 0; ii < syms.GetSize(); ii++)
			{
				if (screen->FindForm(syms.GetAt(ii), form))
				{
					form->ReadData(text);
					text.TrimLeft();
					codes += syms.GetAt(ii);
					codes += '\t';
					codes += text;
					codes += '\n';
				}
			}
		}
	}

	return codes;
}

void CGuard::RemoveCode(CString text, bool reload)
{
	int	idx;
	bool	all = false;
	CCode*	code;
	CString	name = _T(""), codes = _T("");

	idx = text.Find('\t');
	if (idx != -1)
	{
		name  = text.Left(idx);
		codes = text.Mid(idx+1);
	}

	if (codes.IsEmpty())
		all = true;

	if (!m_codex.Lookup(name, (CObject*&)code))
		return;

	if (all)
	{
		code->m_codes.RemoveAll();
		m_codex.RemoveKey(name);
		if (reload)
			SendAxis(MAKEWPARAM(newHISTORY, 0), (LPARAM)name.operator LPCTSTR());
		return;
	}

	for (int ii = 0; ii < code->m_codes.GetSize(); ii++)
	{
		if (!codes.CompareNoCase(code->m_codes.GetAt(ii)))
		{
			code->m_codes.RemoveAt(ii);
			if (reload)
				SendAxis(MAKEWPARAM(newHISTORY, 0), (LPARAM)name.operator LPCTSTR());
			break;
		}
	}
}

void CGuard::RunHelp(char *helpN)
{
	if (strlen(helpN) == 0)
		return;

	CString	cmdN, curN;
	curN = cmdN = _T("");
	curN.Format(_T("%s\\%s"), m_root.GetString(), HELPDIR);
	cmdN.Format(_T(" %s\\%s"), curN.GetString(), helpN);

	DWORD	attr = GetFileAttributes(cmdN);
	if (attr == 0xffffffff)
	{
		SendAxis(MAKEWPARAM(htmlVIEW, 0), (LPARAM)helpN);
		return;
	}

	STARTUPINFO		si;
	PROCESS_INFORMATION	pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;

	char	pathN[256] = { 0, };
	if (!GetWindowsDirectory(pathN, sizeof(pathN)))
		return;
	strcat_s(pathN, sizeof(pathN), _T("\\hh.exe"));

	CreateProcess(
		pathN,			// application name
		(char *)cmdN.GetString(),// command line
		NULL,			// process attribute
		NULL,			// thread attribute
		FALSE,			// is inherit handle
		0,			// creation flags
		NULL,			// environment
		(char *)curN.GetString(),// current directory
		&si,			// STARTUPINFO
		&pi);			// PROCESS_INFORMATION
}

void CGuard::SetRepository(CClient* client)
{
	CScreen* screen;
	for (int idx = 0; idx < client->m_magic; idx++)
	{
		if (client->GetAtScreen(screen, idx))
			SetRepository(screen);
	}
}

void CGuard::SetRepository(class CScreen* screen)
{
	if (!(screen->m_mapH->options & OP_SAVE))
		return;

	CScreen* objs;
	CfmBase* form;
	int	value;
	CString	section;
	CString	text = _T(""), tmps = _T("");

	section = CString(screen->m_mapH->mapN, L_MAPN);
	WritePrivateProfileSection(section, "", m_repository);

	for (int ii = 0; ii < screen->m_mapH->formN; ii++)
	{
		form = screen->GetAtForm(ii);
		if (!form || form->m_form->attr & FA_NOR)
			continue;

		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			if (screen->m_client->GetAtScreen(objs, form->m_form->keys))
				text = CString(objs->m_mapH->mapN, L_MAPN);
			break;
		case FM_TAB:
			if (form->m_form->attr & FA_DRAG)
				form->ReadAll(text);
			else
				text.Format("%d", form->GetCurSel());
			break;
		case FM_SHEET:
		case FM_UTAB:
			text.Format("%d|", form->GetCurSel());
			form->GetEnum(tmps, value);
		//	if (value == ET_USER)
		//		text += tmps;
			break;
		default:
			continue;
		}

		text.TrimRight();
		if (!text.IsEmpty())
		{
			WritePrivateProfileString(section, (char *)form->m_form->name, text, m_repository);
			text.Empty();
		}
	}
}

int CGuard::GetRepository(CClient* client)
{
	CScreen* screen;
	int	saveN = 0;

	for (int idx = 0; idx < client->m_magic; idx++)
	{
		if (client->GetAtScreen(screen, idx))
			saveN += GetRepository(screen);
	}
	return saveN;
}

int CGuard::GetRepository(CScreen* screen)
{
	if (!(screen->m_mapH->options & OP_SAVE))
		return 0;

	CScreen* objs;
	CfmBase* form;
	CString	text, tmps;
	CString	section;

	char	twb[256]{};
	int	idx, saveN = 0;

	section = CString(screen->m_mapH->mapN, L_MAPN);

	for (int key = 0; key < screen->m_mapH->formN; key++)
	{
		form = screen->GetAtForm(key);
		if (!form || form->m_form->attr & FA_NOR)
			continue;

		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			if (screen->m_client->GetAtScreen(objs, form->m_form->keys))
			{
				GetPrivateProfileString(section, (char *)form->m_form->name, "", twb, sizeof(twb), m_repository);
				text = twb;
				text.TrimRight();
				if (text.GetLength() == L_MAPN)
					screen->SetObject(objs->m_object, (char *)text.operator LPCTSTR());
			}
			continue;
		case FM_TAB:
			if (form->m_form->attr & FA_DRAG)
			{
				GetPrivateProfileString(section, (char *)form->m_form->name, "", twb, sizeof(twb), m_repository);
				text = twb;
				if (!text.IsEmpty())
					form->WriteAll(text);
				continue;
			}
			form->SetCurSel(GetPrivateProfileInt(section, (char *)form->m_form->name, 0, m_repository));
			continue;
		case FM_SHEET:
		case FM_UTAB:
			GetPrivateProfileString(section, (char *)form->m_form->name, "", twb, sizeof(twb), m_repository);
			text = twb;
			idx  = text.Find('|');
			if (idx == -1)
				continue;
			tmps = text.Mid(idx+1);
			tmps.TrimRight();
		//	if (!tmps.IsEmpty())
		//		form->SetEnum(tmps, 0);
			form->SetCurSel(atoi(text.Left(idx)));
			switch (form->m_form->kind)
			{
			case FM_TREE:
			case FM_SHEET:
				saveN++;
				break;
			}
			continue;
		default:
			continue;
		}
	}
	return saveN;
}

void CGuard::SetTriggerMode(BOOL vrs, int mode)
{
	if (vrs)
	{
		m_trigger &= ~0xf0;
		switch (mode)
		{
		case 1:
			m_trigger |= trgALLS;	break;
		default:
			m_trigger |= trgVRS;	break;
		}
		return;
	}

	m_trigger &= ~0x0f;
	switch (mode)
	{
	case 1:
		m_trigger |= trgALL;	break;
	case 2:
		m_trigger |= trgTHIS;	break;
	default:
		m_trigger |= trgGROUP;	break;
	}
}

void CGuard::OnTrigger(CWorks* works, CString text)
{
	int	idx;

	idx = text.Find('\t');
	if (idx != -1)
	{
		CString	name;

		name = text.Left(idx++);
		text = text.Mid(idx);

		idx = text.Find('\r');
		if (idx != -1)
		{
			int	key;

			key  = atoi(text.Mid(idx+1));
			text = text.Left(idx);
			works->OnTrigger(name, text, key);
			return;
		}
		works->OnSave(name, text);
		OnTrigger(works, (char *)name.operator LPCTSTR(), text);
	}
}

void CGuard::OnTrigger(CWorks* works, char* formN, CString text)
{
	if (m_status & WS_TRIX)
		return;

	int	key;
	CWorks* client;
	POSITION pos;

	m_status |= WS_TRIX;
	for (pos = m_clients.GetStartPosition(); pos; )
	{
		m_clients.GetNextAssoc(pos, key, client);
		if (client->isTrigger(works->m_key, formN, text))
		{
			m_status &= ~WS_TRIX;
			return;
		}
	}

	if (!works->m_group && (m_trigger & 0x0f) != trgALL)
	{
		m_status &= ~WS_TRIX;
		return;
	}

	for (pos = m_clients.GetStartPosition(); pos; )
	{
		m_clients.GetNextAssoc(pos, key, client);
		if (client->m_status & S_LOAD && key != works->m_key)
		{
			switch (m_trigger & 0xf0)
			{
			case trgVRS:
				if (client->m_vid != works->m_vid)
					continue;
				[[fallthrough]];
			default:
				break;
			}

			switch (m_trigger & 0x0f)
			{
			case trgALL:
				break;
			case trgGROUP:
				if (client->m_group == works->m_group || client->m_group == -1)
					break;
				[[fallthrough]];
			case trgTHIS:
				continue;
			}

			if (!(client->m_status & S_WORKS))
				client->OnTrigger(formN, text, -1);
		}
	}
	m_status &= ~WS_TRIX;
}

void CGuard::OnProcedure(CString proc, CString param, CString maps, int major, int minor)
{
	CWorks*	works;

	if (m_clients.Lookup(major, works))
		works->OnProcedure(proc, param, minor);
	else
	{
		for (POSITION pos = m_clients.GetStartPosition(); pos; )
		{
			m_clients.GetNextAssoc(pos, major, works);
			if (maps.IsEmpty() || !works->m_mapN.CompareNoCase(maps))
				works->OnProcedure(proc, param);
		}
	}
}

bool CGuard::IsPopup(CString mapN)
{
	int	key;
	CWorks* works;

	for (POSITION pos = m_clients.GetStartPosition(); pos; )
	{
		m_clients.GetNextAssoc(pos, key, works);
		if (!works->m_mapN.CompareNoCase(mapN) && works->m_key & WK_POPUP)
			return true;
	}
	return false;
}

bool CGuard::RouteTR(CClient* client)
{
	client->m_status &= ~S_CURSOR;
	CString stmp, strresult;
	
	for (int ii = L_axisH; ii < client->m_stream->m_sndL; ii++)
	{
		switch (client->m_stream->m_sndB[ii])
		{
			case 0x09:
				stmp = "^";
				break;
			case 0x0A:
				stmp = "&";
				break;
			case 0x7f:
				stmp = "*";
				break;
			default:
				stmp.Format("%.1s", (char*)&(client->m_stream->m_sndB[ii]));
				break;
		}
		strresult += stmp;
	}
	if (!Write(client->m_stream->m_sndB, client->m_stream->m_sndL, false))
	{
		SetGuide(AE_SERVERX, client->m_key);
		return false;
	}

	SetGuide(AE_CLEAR, client->m_key);
	return true;
}

BOOL CGuard::Write(char* pBytes, int nBytes, int key)
{
	m_slog.Format("\r\n[wizard][guard]<%d>[%s] len =[%d] pBytes=[%.50s]",__LINE__, __FUNCTION__, nBytes, pBytes);
	OutputDebugString(m_slog);
// updateXXX_2022XX
	if (nBytes > maxIOs)
	{
		SetGuide(AE_MAXIO, key);
		return FALSE;
	}

	char*	sendB;
	struct	_axisH*	axisH;
	struct	_userTH* userth;
	CWorks*	works = NULL;

	sendB = new char[L_axisH+nBytes+2048];	// 1024->2048 : for FDS
	userth = (struct _userTH *)pBytes;
	axisH = (struct _axisH *) sendB;
	ZeroMemory(axisH, L_axisH);

	axisH->msgK = msgK_AXIS;
	axisH->winK = key;
	axisH->unit = userth->key;

	CopyMemory(axisH->trxC, userth->trc, sizeof(userth->trc));
	CopyMemory(&sendB[L_axisH], &pBytes[L_userTH], nBytes);

	if (GetClient(key, works) && !works->m_mapN.IsEmpty())
		CopyMemory(axisH->svcN, works->m_mapN.Mid(L_SGID, L_SELC), sizeof(axisH->svcN));

	if (userth->stat & US_OOP)
		axisH->auxs |= auxsOOP;
	if (userth->stat & US_XRTM)
		axisH->trxS |= trxK_XRTM;

	if (!(m_term & flagCAX) && userth->stat & US_CA)
	{
		if (!Certify(&sendB[L_axisH], nBytes, works ? works->m_mapN : _T("")))
		{
			delete [] sendB;
			return FALSE;
		}
		axisH->auxs |= auxsCA;
	}

	if (!(m_term & flagENX) && userth->stat & US_ENC)
	{
		if (GetFdsValue(CString(axisH->trxC, L_TRXC), &sendB[L_axisH], nBytes))
			axisH->auxs |= auxsFDS;

#ifndef DF_NOENC
		if (!Xecure(DI_ENC, &sendB[L_axisH], nBytes))
		{
			SetGuide(AE_CSECURE, key);
			delete [] sendB;
			return FALSE;
		}
		axisH->stat |= statENC;
#endif
	}

	CString	text;
	text.Format(_T("%05d"), nBytes);
	CopyMemory(axisH->datL, text.GetString(), sizeof(axisH->datL));

	if (Write(sendB, L_axisH+nBytes, true))
	{
		if (key && !(userth->stat & US_PASS))
			PostAxis(waitPAN, key, true);
		delete [] sendB;
		return TRUE;
	}

	delete [] sendB;
	return FALSE;
}

BOOL CGuard::Write(int msgK, CString trxC, char* datB, int datL, int key, bool trace)
{
	char*	sendB;
	struct	_axisH*	axisH;
	CWorks*	works;
	CString text;

	sendB = new char[L_axisH+datL+1024];	// 1024 : for FDS
	axisH = (struct _axisH *) sendB;
	ZeroMemory(axisH, L_axisH);

	axisH->msgK = msgK;
	axisH->winK = key;
	if (GetClient(key, works) && !works->m_mapN.IsEmpty())
		CopyMemory(axisH->svcN, works->m_mapN.Mid(L_SGID, L_SELC), sizeof(axisH->svcN));
	CopyMemory(axisH->trxC, trxC, trxC.GetLength());
	text.Format(_T("%05d"), datL);
	CopyMemory(axisH->datL, (char *)text.operator LPCSTR(), sizeof(axisH->datL));
	CopyMemory(&sendB[L_axisH], datB, datL);

	if (msgK == msgK_XCA)	// for FDS
	{
		if (GetFdsValue(CString(axisH->trxC, L_TRXC), &sendB[L_axisH], datL))
			axisH->auxs |= auxsFDS;
	}

	if (Write(sendB, L_axisH+datL, trace))
	{
		delete [] sendB;
		return TRUE;
	}

	delete [] sendB;
	return FALSE;
}

/* 20171101 for signUSERc
BOOL CGuard::Login(char* datB, int datL, bool xecure)
{
	CString	text;
	char*	sendB;
	struct	_axisH*	axisH;

	sendB = new char[L_axisH+datL+2048];	// 256 -> 2048 : for FDS
	axisH = (struct _axisH *) sendB;
	ZeroMemory(axisH, L_axisH);

	text = _T("AXLOGON");
	axisH->msgK = msgK_SIGN;
	CopyMemory(axisH->trxC, text, text.GetLength());
	CopyMemory(&sendB[L_axisH], datB, datL);
	if (GetFdsValue(_T("AXLOGON"), &sendB[L_axisH], datL))
		axisH->auxs |= auxsFDS;
	if (xecure)
	{
		if (!Xecure(DI_ENC, (char *)&sendB[L_axisH], datL))
		{
			delete [] sendB;
			return FALSE;
		}
		axisH->stat |= statENC;
	}

	text.Format("%05d", datL);
	CopyMemory(axisH->datL, (char *)text.operator LPCTSTR(), sizeof(axisH->datL));
	if (Write(sendB, L_axisH+datL, false))
	{
		delete [] sendB;
		return TRUE;
	}

	delete [] sendB;
	return FALSE;
}
*/

BOOL CGuard::Login(int mode, char* datB, int datL, bool xecure)
{
	CString	trN, text;
	char*	sendB;
	struct	_axisH*	axisH;

	sendB = new char[L_axisH+datL+2048];	// 256 -> 2048 : for FDS
	axisH = (struct _axisH *) sendB;
	ZeroMemory(axisH, L_axisH);

	if (mode == signUSER)
	{
		trN = _T("AXLOGONE");
		axisH->msgK = msgK_SIGN;
	}
	else
	{
		trN = _T("AXLOGONC");
		axisH->msgK = msgK_SIGNx;
	}
	CopyMemory(axisH->trxC, trN, trN.GetLength());
	CopyMemory(&sendB[L_axisH], datB, datL);
	if (GetFdsValue(trN, &sendB[L_axisH], datL))
		axisH->auxs |= auxsFDS;

	if (xecure)
	{
		if (!Xecure(DI_ENC, (char *)&sendB[L_axisH], datL))
		{
			delete [] sendB;
			return FALSE;
		}
		axisH->stat |= statENC;
	}

	text.Format("%05d", datL);
	CopyMemory(axisH->datL, (char *)text.operator LPCTSTR(), sizeof(axisH->datL));
	if (Write(sendB, L_axisH+datL, false))
	{
		delete [] sendB;
		return TRUE;
	}

	delete [] sendB;
	return FALSE;
}

BOOL CGuard::Service(CScreen* screen, CString trxC, char* datB, int datL, int mode)
{
	char*	sendB;
	struct	_axisH*	axisH;

	sendB = new char[L_axisH+datL+2048];	// 1024 -> 2048 : for FDS
	axisH = (struct _axisH *) sendB;
	ZeroMemory(axisH, L_axisH);

	axisH->msgK = msgK_SVC;
	axisH->winK = screen->m_client->m_key;
	axisH->unit = screen->m_key;
	CopyMemory(axisH->trxC, trxC, trxC.GetLength());
	CopyMemory(axisH->svcN, CString(screen->m_mapH->mapN, L_MAPN).Mid(L_SGID, L_SELC), sizeof(axisH->svcN));

	CopyMemory(&sendB[L_axisH], datB, datL);
	if (mode & US_OOP)
		axisH->auxs |= auxsOOP;
	if (mode & US_XRTM || !(screen->m_mapH->options & OP_FLASH))
		axisH->trxS |= trxK_XRTM;
	if (!(m_term & flagCAX) && mode & US_CA)
	{
		if (!Certify(&sendB[L_axisH], datL))
		{
			delete [] sendB;
			return FALSE;
		}
		axisH->auxs |= auxsCA;
	}

	if (!(m_term & flagENX) && mode & US_ENC)
	{
		if (GetFdsValue(CString(axisH->trxC, L_TRXC), &sendB[L_axisH], datL))
			axisH->auxs |= auxsFDS;

		if (!Xecure(DI_ENC, &sendB[L_axisH], datL))
		{
			SetGuide(AE_CSECURE, axisH->winK);
			delete [] sendB;
			return FALSE;
		}
		axisH->stat |= statENC;
	}

	CString	text;
	text.Format("%05d", datL);
	CopyMemory(axisH->datL, (char *)text.operator LPCTSTR(), sizeof(axisH->datL));

	if (Write(sendB, L_axisH+datL, true))
	{
		screen->m_client->m_units.SetAt(screen->m_key, screen->m_key);
		if (!(mode & US_PASS))
		{
			screen->m_state |= waitSN;
			PostAxis(MAKEWPARAM(waitPAN, axisH->winK), true);
		}
		delete [] sendB;
		return TRUE;
	}

	delete [] sendB;
	return FALSE;
}

BOOL CGuard::Approve(CScreen* screen, int key, CString ip, CString map, char* datB, int datL)
{
	char*	sendB;
	struct	_axisH*	axisH;
	CString	text;

	sendB = new char[L_axisH+datL];
	axisH = (struct _axisH *) sendB;
	ZeroMemory(axisH, L_axisH);

	axisH->msgK = msgK_APC;
	axisH->trxK = screen->m_client->m_key;
	axisH->trxS = screen->m_key;
	if (key != -1)
	{
		axisH->winK = LOBYTE(key);
		axisH->unit = HIBYTE(key);
	}
	else
	{
		axisH->stat = statNEW;
	//	axisH->trxK = screen->m_client->m_key;
	//	axisH->trxS = screen->m_key;
		CopyMemory(axisH->trxC, map, map.GetLength());
	}

	CopyMemory(axisH->svcN, CString(screen->m_mapH->mapN, L_MAPN).Mid(L_SGID, L_SELC), sizeof(axisH->svcN));
	text.Format(_T("%05d"), datL);
	CopyMemory(axisH->datL, (char *)text.operator LPCSTR(), sizeof(axisH->datL));
	CopyMemory(&sendB[L_axisH], datB, datL);

	xTRACE(x_SNDs, sendB, L_axisH+datL);

	ip.Trim();
	BOOL	retv;
	m_sock->InvokeHelper(DI_DWRITE, DISPATCH_METHOD, VT_BOOL, (void*)&retv,
				(BYTE *) VTS_BSTR VTS_I4 VTS_I4 VTS_BOOL, ip, sendB, L_axisH+datL, FALSE);
	delete [] sendB;
	return retv;
}

long CGuard::UploadFile(CScreen* screen, CString trxC, char* datB, int datL, int mode, CString fileN, int offset)
{
	char*	sendB;
	CFile	file;
	struct	_axisH*	axisH;

	sendB = new char[maxIOs];
	axisH = (struct _axisH *) sendB;
	ZeroMemory(axisH, L_axisH);

	axisH->msgK = msgK_UPF;
	axisH->winK = screen->m_client->m_key;
	axisH->unit = screen->m_key;
	CopyMemory(axisH->trxC, trxC, trxC.GetLength());
	CopyMemory(axisH->svcN, CString(screen->m_mapH->mapN, L_MAPN).Mid(L_SGID, L_SELC), sizeof(axisH->svcN));
	CopyMemory(&sendB[L_axisH], datB, datL);
	if (mode & US_OOP)
		axisH->auxs |= auxsOOP;
	if (mode & US_XRTM || !(screen->m_mapH->options & OP_FLASH))
		axisH->trxS |= trxK_XRTM;

	if (!file.Open(fileN, CFile::modeRead|CFile::typeBinary))
	{
		delete [] sendB;
		return 0;
	}

	int	rc, size;
	CString	text;
	struct	_fileH*	fileH;

	size = (int) file.GetLength();
	if (size <= offset)
	{
		file.Close();
		delete [] sendB;
		return 0;
	}

	fileH = (struct _fileH *)&sendB[L_axisH+datL];
	ZeroMemory(fileH, L_fileH);
	fileH->fileK = fileK_FILE;
	CopyMemory(fileH->fileN, fileN, fileN.GetLength());

	datL += L_fileH;
	file.Seek(offset, CFile::begin);
	rc = file.Read(&sendB[L_axisH+datL], maxIOs-L_axisH-datL-1024);	// maxIOs-L_axisH-datL -> maxIOs-L_axisH-datL-1024 : for FDS
	file.Close();
	if (rc <= 0)
	{
		delete [] sendB;
		return 0;
	}

	if (offset == 0)
		fileH->fileF = fileF_FIR;
	else if (offset + rc < size)
		fileH->fileF = fileF_MID;
	if (offset + rc >= size)
		fileH->fileF = (fileH->fileF == fileF_FIR) ? fileF_ONLY : fileF_LAS;
	
	text.Format(_T("%05d"), rc);
	CopyMemory(fileH->datL, (char *)text.operator LPCTSTR(), sizeof(fileH->datL));

	datL += rc;
	if (!(m_term & flagCAX) && mode & US_CA)
	{
		if (!Certify(&sendB[L_axisH], datL))
		{
			delete [] sendB;
			return 0;
		}
		axisH->auxs |= auxsCA;
	}

	if (!(m_term & flagENX) && mode & US_ENC)
	{
		if (GetFdsValue(CString(axisH->trxC, L_TRXC), &sendB[L_axisH], datL))
			axisH->auxs |= auxsFDS;

		if (!Xecure(DI_ENC, &sendB[L_axisH], datL))
		{
			SetGuide(AE_CSECURE, axisH->winK);
			delete [] sendB;
			return 0;
		}
		axisH->stat |= statENC;
	}

	text.Format(_T("%05d"), datL);
	CopyMemory(axisH->datL, (char *)text.operator LPCTSTR(), sizeof(axisH->datL));

	if (Write(sendB, L_axisH+datL, true))
	{
		screen->m_client->m_units.SetAt(screen->m_key, screen->m_key);
		if (!(mode & US_PASS))
		{
			screen->m_state |= waitSN;
			PostAxis(MAKEWPARAM(waitPAN, axisH->winK), true);
		}
		delete [] sendB;
		return rc;
	}

	delete [] sendB;
	return 0;
}

BOOL CGuard::DownloadFile(CScreen* screen, CString trxC, char* datB, int datL, int mode, CString fileN)
{
	char*	sendB;
	struct	_axisH*	axisH;

	sendB = new char[maxIOs];
	axisH = (struct _axisH *) sendB;
	ZeroMemory(axisH, L_axisH);

	axisH->msgK = msgK_DNF;
	axisH->winK = screen->m_client->m_key;
	axisH->unit = screen->m_key;
	CopyMemory(axisH->trxC, trxC, trxC.GetLength());
	CopyMemory(axisH->svcN, CString(screen->m_mapH->mapN, L_MAPN).Mid(L_SGID, L_SELC), sizeof(axisH->svcN));
	CopyMemory(&sendB[L_axisH], datB, datL);
	if (mode & US_OOP)
		axisH->auxs |= auxsOOP;
	if (mode & US_XRTM || !(screen->m_mapH->options & OP_FLASH))
		axisH->trxS |= trxK_XRTM;

	CString	text;
	struct	_fileH*	fileH;

	fileH = (struct _fileH *)&sendB[L_axisH+datL];
	ZeroMemory(fileH, L_fileH);
	fileH->fileK = fileK_FILE;
	CopyMemory(fileH->fileN, fileN, fileN.GetLength());
	datL += L_fileH;
	
	if (!(m_term & flagCAX) && mode & US_CA)
	{
		if (!Certify(&sendB[L_axisH], datL))
		{
			delete [] sendB;
			return FALSE;
		}
		axisH->auxs |= auxsCA;
	}

	if (!(m_term & flagENX) && mode & US_ENC)
	{
		if (GetFdsValue(CString(axisH->trxC, L_TRXC), &sendB[L_axisH], datL))
			axisH->auxs |= auxsFDS;

		if (!Xecure(DI_ENC, &sendB[L_axisH], datL))
		{
			SetGuide(AE_CSECURE, axisH->winK);
			delete [] sendB;
			return FALSE;
		}
		axisH->stat |= statENC;
	}

	text.Format(_T("%05d"), datL);
	CopyMemory(axisH->datL, (char *)text.operator LPCTSTR(), sizeof(axisH->datL));

	if (Write(sendB, L_axisH+datL, true))
	{
		screen->m_client->m_units.SetAt(screen->m_key, screen->m_key);
		if (!(mode & US_PASS))
		{
			screen->m_state |= waitSN;
			PostAxis(MAKEWPARAM(waitPAN, axisH->winK), true);
		}
		delete [] sendB;
		return TRUE;
	}

	delete [] sendB;
	return FALSE;
}

BOOL CGuard::Invoke(char* pBytes, int nBytes, int key)
{
	if (nBytes > maxIOs)
	{
		SetGuide(AE_MAXIO, key);
		return FALSE;
	}

	char*	sendB;
	struct	_axisH*	axisH;
	struct	_userTH* userth;
	CWorks*	works;
	CScreen* screen;
	int	pos, index;
	int	keyx;
	CString	string;

	string = CString(pBytes, L_userTH+nBytes);
	pos    = string.Find('\t');
	switch (pos)
	{
	case -1:
		return FALSE;
	case 0:
		pos = string.Mid(1).Find('\t');
		if (pos == -1)
			return FALSE;
		pos++;
		[[fallthrough]];
	default:
		break;
	}

	string = string.Left(pos++);
	pBytes += pos;
	userth  = (struct _userTH *)pBytes;

	if (!GetClient(key, works) || !works->isWorks())
		return FALSE;

	if (!((CClient *)works)->GetAtScreen(screen, userth->key))
		return FALSE;
	
	if (userth->stat & US_KEY)
	{
		pos = string.GetAt(0);
		string.Delete(0);
	}
	else	pos = 0;

	if (!screen->FindForm(string, index))
		return FALSE;

	keyx = ((CClient *)works)->GetKey(userth->key, pos, index);
	if (keyx < 0)
		return FALSE;

	sendB  = new char[L_axisH+nBytes+2048];		// 1024 -> 2048 : for FDS
	axisH  = (struct _axisH *) sendB;
	ZeroMemory(axisH, L_axisH);

	axisH->msgK = msgK_CTRL;
	axisH->winK = key;
	axisH->unit = keyx;

	CopyMemory(axisH->trxC, userth->trc, sizeof(userth->trc));
	CopyMemory(axisH->svcN, CString(screen->m_mapH->mapN, L_MAPN).Mid(L_SGID, L_SELC), sizeof(axisH->svcN));
	CopyMemory(&sendB[L_axisH], &pBytes[L_userTH], nBytes);

	if (userth->stat & US_OOP)
		axisH->auxs |= auxsOOP;
	if (userth->stat & US_XRTM || !(screen->m_mapH->options & OP_FLASH))
		axisH->trxS |= trxK_XRTM;

	if (!(m_term & flagCAX) && userth->stat & US_CA)
	{
		if (!Certify(&sendB[L_axisH], nBytes))
		{
			delete [] sendB;
			return FALSE;
		}
		axisH->auxs |= auxsCA;
	}

	if (!(m_term & flagENX) && userth->stat & US_ENC)
	{
		if (GetFdsValue(CString(axisH->trxC, L_TRXC), &sendB[L_axisH], nBytes))
			axisH->auxs |= auxsFDS;
#ifndef DF_NOENC
		if (!Xecure(DI_ENC, &sendB[L_axisH], nBytes))
		{
			SetGuide(AE_CSECURE, key);
			delete [] sendB;
			return FALSE;
		}
		axisH->stat |= statENC;
#endif
	}

	CString	text;
	text.Format(_T("%05d"), nBytes);
	CopyMemory(axisH->datL, (char *)text.operator LPCTSTR(), sizeof(axisH->datL));

	if (Write(sendB, L_axisH+nBytes, true))
	{
		screen->m_client->m_units.SetAt(keyx, keyx);
		if (key && !(userth->stat & US_PASS))
			PostAxis(MAKEWPARAM(waitPAN, key), true);
		delete [] sendB;
		return TRUE;
	}

	delete [] sendB;
	return FALSE;
}

BOOL CGuard::Write(char* pBytes, int nBytes, bool trace)
{
// updateXXX_2022XX	
OutputDebugString("\r\n--------------");
m_slog.Format("\r\n[wizard][guard]<%d>[%s] len =[%d] pBytes=[%x][%x]", __LINE__, __FUNCTION__, nBytes, *pBytes, *(pBytes  + 3));
OutputDebugString(m_slog);
OutputDebugString("\r\n============");
	if (nBytes > L_axisH + maxIOs)
	{
		SetGuide(AE_MAXIO, ((struct _axisH *)pBytes)->winK);
		return FALSE;
	}

	if (trace)
		xTRACE(x_SNDs, pBytes, nBytes);

	BOOL	retv;
	m_sock->InvokeHelper(DI_WRITE, DISPATCH_METHOD, VT_BOOL, (void*)&retv,
						(BYTE *) VTS_I4 VTS_I4, pBytes, nBytes);
	return retv;
}

void CGuard::PostAxis(int msg, int key, LPARAM lParam)
{
	int	keys;
	CWorks* works;

	for (POSITION pos = m_clients.GetStartPosition(); pos; )
	{
		m_clients.GetNextAssoc(pos, keys, works);
		if (works->isYours(key))
		{
			((CDll *)works)->m_dll->PostMessage(WM_USER, MAKEWPARAM(MAKEWORD(DLL_AXIS, msg), key), lParam);
			return;
		}
	}
	PostAxis(MAKEWPARAM(msg, key), lParam);
}

int CGuard::SendAxis(int msg, int key, LPARAM lParam)
{
	int	keys;
	CWorks* works;

	for (POSITION pos = m_clients.GetStartPosition(); pos; )
	{
		m_clients.GetNextAssoc(pos, keys, works);
		if (works->isYours(key))
			return ((CDll *)works)->m_dll->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(DLL_AXIS, msg), key), lParam);
	}
	return SendAxis(MAKEWPARAM(msg, key), lParam);
}

bool CGuard::CheckSecurity(CString mapN, char* curN)
{
	struct _mapH	mapH, gmapH;

	if (mapN.GetLength() != L_MAPN)
		return false;

	if (!GetMapH(mapN, (char*)&mapH))
	{
		SetGuide(AE_CHOICE);
		return false;
	}

	if (mapN.CompareNoCase(CString((char *)mapH.mapN, L_MAPN)))
	{
		SetGuide(AE_CHOICE);
		return false;
	}

	if (strlen(curN) && memcmp(curN, mapN, L_TGID))
	{
		char	twb[L_MAPN+1];
		FillMemory(twb, L_MAPN, '0');
		CopyMemory(twb, mapN, L_TGID);
		twb[L_MAPN] = '\0';
		if (GetMapH(twb, (char*)&gmapH))
		{
			if (!CheckSecurity(&gmapH))
				return false;
		}
	}

	return CheckSecurity(&mapH);
}


bool CGuard::CheckSecurity(struct _mapH* mapH)
{
	if (m_absS && m_absS < mapH->absS)
	{
		SetGuide(AE_ESECURITY);
		return false;
	}

	int	iBit, kk;
	DWORD	terms, maps, masks, ieSecurity;

	terms = maps = 0;
	for (kk = 0; kk < L_SECB; kk++)
	{
		terms <<= 8;
		terms |= (BYTE)m_incS[kk];

		maps <<= 8;
		maps |= (BYTE)mapH->incS[kk];
	}

	ieSecurity = terms & maps;
	if (!ieSecurity && terms && maps)
	{
		iBit = 0;
		for (masks = 0x80000000; masks; masks >>= 1)
		{
			if (terms & masks)
			{
				if (!(maps & masks))
				{
					SecurityErrorMsg(iBit);
					return false;
				}
			}
			iBit++;
		}
	}

	terms = maps = 0;
	for (kk = 0; kk < L_SECB; kk++)
	{
		terms <<= 8;
		terms |= (BYTE)m_excS[kk];

		maps <<= 8;
		maps |= (BYTE)mapH->excS[kk];
	}

	if (!maps)
		return true;

	iBit = 32;
	ieSecurity = terms ^ maps;
	for (masks = 0x80000000; masks; masks >>= 1, iBit++)
	{
		if (!(maps & masks))
		{
			continue;
		}

		if (ieSecurity & masks)
		{
			SecurityErrorMsg(iBit);
			return false;
		}
	}
	return true;
}

void CGuard::SecurityErrorMsg(int secNo)
{
	CString	guide;
	if (m_secmt.Lookup(secNo, guide))
		SetGuide(guide);
	else
		SetGuide(AE_ESECURITM);
	return;
}

void CGuard::LoadSecuritys()
{
	char	tmb[60*32*2];
	char	*secM;

	HANDLE	hSecFile;
	DWORD	NumOfBytesRead;
	BOOL	fBool;

	wsprintf(tmb, _T("%s\\%s\\%s"), m_root.GetString(), MTBLDIR, SECMSG);
	hSecFile = CreateFile(tmb,
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

	if (hSecFile == INVALID_HANDLE_VALUE)
		return;

	fBool = ReadFile(hSecFile, tmb, sizeof(tmb), &NumOfBytesRead, NULL);
	CloseHandle(hSecFile);
	if (!fBool || NumOfBytesRead != sizeof(tmb))
		return;

	secM = tmb;
	CString	guideM;
	for (int ii = 0; ii < 32*2; ii++)
	{
		if (strlen(secM))
		{
			guideM = secM;
			m_secmt.SetAt(ii, guideM);
		}
		secM += 60;
	}
}

int CGuard::WorkView(CString maps, int* size) 
{
	struct	{
		char*	section;
		char*	prefix;
		int	type;
	} controls[] = {
		{ "VBX Controls", ".ocx", vtypeVBX },
		{ "DLL Controls", ".dll", vtypeDLL },
		{ "GRX Controls", ".dll", vtypeGRX },
		{0}
	};

	struct	{
		char*	desc;
		int	vtype;
	} vtypes[] = {
		{ "Normal",	0 },
		{ "Scroll",	vtypeSCR },
		{ "Resizable",	vtypeRSZ },
		{ "Static",	vtypeWND },
		{ "Fixed",	vtypeFIX },
		{ "FixedEx",	vtypeFEX },
		{ "ResizableEx",vtypeREX },
		{0}
	};

	char	wb[256];
	int	cx, cy, pos;
	CString	text;

	CopyMemory(wb, (char *)maps.operator LPCSTR(), maps.GetLength()); wb[maps.GetLength()] = '\0';
	if (GetVirtualTR(wb))
		maps = wb;

	for (int ii = 0; controls[ii].section; ii++)
	{
		if (GetPrivateProfileString(controls[ii].section, maps, "", wb, sizeof(wb), m_vtcode))
		{
			cx = cy = 0;
			text = wb;
			text.TrimLeft(); text.TrimRight();
			pos = text.FindOneOf(" \t");
			if (pos != -1)
			{
				cx   = atoi(text.Left(pos));
				text = text.Mid(pos+1);
				text.TrimLeft();
			}
			pos = text.FindOneOf(" \t");
			if (pos != -1)
			{
				cy = atoi(text.Left(pos));
				text = text.Mid(pos+1);
				text.TrimLeft();
			}

			pos = text.FindOneOf(" ;\t");
			if (pos != -1)
				text = text.Left(pos);
			if (cx && cy && isLevel(maps, controls[ii].prefix) && isVersion(maps, controls[ii].prefix))
			{
				*(int *)size = MAKELONG(cx, cy);
				for (int jj = 0; vtypes[jj].desc; jj++)
				{
					if (!text.CompareNoCase(vtypes[jj].desc))
						return (controls[ii].type|vtypes[jj].vtype);
				}
				return controls[ii].type;
			}
			return vtypeERR;
		}
	}

	struct	_mapH	mapH;

	if (!isVersion(maps) || !GetMapH(maps, (char *)&mapH))
	{
		SetGuide(AE_CHOICE);
		return vtypeERR;
	}
	if (!CheckSecurity(&mapH))
		return vtypeERR;

	*(int *)size = MAKELONG(mapH.width, mapH.height);

	switch (mapH.type)
	{
	case VW_SCROLL:
		return (vtypeNRM|vtypeSCR);
	case VW_RESIZE:
		return (vtypeNRM|vtypeRSZ);
	case VW_WND:
		return (vtypeNRM|vtypeWND);
	case VW_FIXED:
		return (vtypeNRM|vtypeFIX);
	case VW_FEX:
		return (vtypeNRM|vtypeFEX);
	case VW_REX:
		return (vtypeNRM|vtypeREX);
	default:
		break;
	}
	return vtypeNRM;
}

bool CGuard::GetVersion(CString fileN, CString& vers)
{
	DWORD 	tw = 0, DWval = 0;
	DWval = GetFileVersionInfoSize((char *)fileN.operator LPCTSTR(), &tw);
	if (DWval <= 0)	return false;

	int	ii, jj;
	char 	*VerInfo, *Vpointer;
	VerInfo = new char[DWval +1]{ 0, };

	tw = 0;
	if (!GetFileVersionInfo (fileN.GetString(), tw, DWval+1, (LPVOID)VerInfo))
	{
		delete [] VerInfo;
		return false;
	}

	for (ii = 0, jj = 0; ii < static_cast<int>(DWval); ii++)
	{
		if (VerInfo[ii])
			VerInfo[jj++] = VerInfo[ii];
	}
	VerInfo[jj] = '\0';

	if (Vpointer = strstr(VerInfo, "FileVersion")) 
	{
		for (ii = strlen("FileVersion"), jj = 0; ii < static_cast<int>(DWval); ii++)
		{
// updateXXX_2022XX
			if (Vpointer[ii] == '.' || isdigit(static_cast<unsigned char>(Vpointer[ii]))) 
				VerInfo[jj++] = Vpointer[ii];
			else	break;
		}
	}
	else if (Vpointer = strstr(VerInfo, "ProductVersion")) 
	{
		for (ii = strlen("ProductVersion"), jj = 0; ii < static_cast<int>(DWval); ii++)
		{
// updateXXX_2022XX
			if (Vpointer[ii] == '.' || isdigit(static_cast<unsigned char>(Vpointer[ii])))
				VerInfo[jj++] = Vpointer[ii];
			else	break;
		}
	}

	vers = CString(VerInfo, jj);
	delete [] VerInfo;

	return true;
}

int CGuard::GetCodes(CString name, CString& text, bool self)
{
	CCode*	code;

	int	size;
	CString	tmps;

	size = 0;
	text.Empty();

	if (!m_codex.Lookup(name, (CObject*&)code))
		return size;

	for (int ii = 0; ii < code->m_codes.GetSize(); ii++)
	{
		tmps = code->m_codes.GetAt(ii);
		if (self)
		{
			if (!code->m_self || !GetName(whichNAME, tmps, name))
				continue;
			if (tmps.GetLength() < m_CODEs)
				tmps += ' ';
			tmps += MASTERs;
			tmps += name;
		}
		text += tmps;
		text += '\t';
		size++;
	}
	return size;
}

void CGuard::SetGuide(int type, CString guide, int key)
{
	CString	text;

	switch (type)
	{
	case typeFM:
	case typeSM:
		SetGuide(guide, key);
		break;
	case typeERR:
		::MessageBox(NULL, guide, GetString(AS_ERROR, text) ? text : _T("ERROR"), MB_OK|MB_ICONERROR|MB_SYSTEMMODAL);
		break;
	case typeCON:
		type = guide.GetAt(0);
		SetGuide(guide.Mid(1), key, type);
		break;
	case typeNO:
	default:
		break;
	}
}

int CGuard::OpenView(CWorks* works, CString maps, int type, int pos)
{
	struct	_userWH	userwh;

	if (maps.GetLength() < L_MAPN)
		return 0;

	ZeroMemory(&userwh, L_userWH);
	CopyMemory(userwh.maps, maps, L_MAPN);

	userwh.type  = type;
	userwh.pos.x = -1;
	userwh.pos.y = pos;

	int	idx, key;
	char	*keys;
	bool	search, force, find;
	CString	tmps, text, domino;

	domino.Empty();
	m_dragbys.Empty();
	search = force = find = false;

	tmps = "/ ";
	keys = _T("ctSsKkPpd");

	for (int ii = 0; keys[ii]; ii++)
	{
		tmps.SetAt(1, keys[ii]);
		idx = maps.Find(tmps);
		if (idx < 0)
			continue;

		idx += 2;
		pos = maps.Find('/', idx);

		text = (pos < 0) ? maps.Mid(idx) : maps.Mid(idx, pos-idx);
		text.TrimLeft();

		switch (keys[ii])
		{
		case 'c':
			m_dragbys = CString(userwh.maps, L_MAPN);
			break;
		case 't':
			userwh.group = atoi(text);
			if (!userwh.group)
				userwh.group = works->m_group;
			break;
		case 'S':
			force = true;
			[[fallthrough]];
		case 's':
			search = true;
			break;
		case 'K':
			force = true;
			[[fallthrough]];
		case 'k':
			userwh.key = atoi(text);
			break;
		case 'p':
			if (!text.IsEmpty() && text.GetAt(0) == '?')
			{
				find = true;
				break;
			}
			userwh.pos.y = atoi(text);
			break;
		case 'P':
			idx = atoi(text);
			userwh.pos.x = LOWORD(idx);
			userwh.pos.y = HIWORD(idx);
			break;
		case 'd':
			domino = text;
			break;
		}
	}

	if (find)
	{
		CScreen* screen = ((CClient *)works)->m_mouse->GetScreen();
		if (screen)
		{
			CfmBase* form = screen->GetAtForm(((CClient *)works)->m_mouse->m_mouse.m_idx);
			if (form)
			{
				CRect	rect = form->GetRect();
				works->m_view->ClientToScreen(&rect);
				userwh.pos.x = max(0, rect.left);
				userwh.pos.y = max(0, rect.bottom);
			}
		}
	}

	if (!m_dragbys.IsEmpty())
	{
		m_dragbys += domino;
		PostAxis(MAKEWPARAM(renVIEW, works->m_key), (LPARAM)m_dragbys.operator LPCTSTR());
		return works->m_key;
	}

	key = works->m_key;
	maps = maps.Left(L_MAPN);
	if (GetClient(userwh.key, works))
	{
		if (works->m_mapN.CompareNoCase(maps.Left(L_MAPN)))
		{
			maps += domino;
			return SendAxis(MAKEWPARAM(renVIEW, works->m_key), (LPARAM)maps.operator LPCTSTR());
		}
		return AtView(works, maps, domino, force);
	}

	if (search)
	{
		bool	isvirtual;
		char	mapN[L_MAPN+1];

		CopyMemory(mapN, maps, L_MAPN);
		mapN[L_MAPN] = '\0';
		isvirtual = GetVirtualTR(mapN);

		for (POSITION posx = m_clients.GetStartPosition(); posx; )
		{
			m_clients.GetNextAssoc(posx, idx, works);
			if (works->m_status & S_WORKS)
				continue;
			if (m_vid != works->m_vid || (userwh.group && userwh.group != works->m_group))
				continue;

			if (isvirtual)
			{
				char	mapb[L_MAPN+1];

				CopyMemory(mapb, works->m_mapN, L_MAPN); mapb[L_MAPN] = '\0';
				if (GetVirtualTR(mapb) && CString(mapN).CompareNoCase(mapb) == 0)
				{
					maps += domino;
					return SendAxis(MAKEWPARAM(renVIEW, works->m_key), (LPARAM)maps.operator LPCTSTR());
				}
			}

			if (works->m_mapN.CompareNoCase(maps) == 0)
			{
				if (!works->isWorks())
					force = true;
				return AtView(works, maps, domino, force);
			}
		}
	}

	if (type != typeVIEW && !userwh.key)
		userwh.key = WK_POPUP;

	text = CString((char *)&userwh, L_userWH) + domino;
	return SendAxis(MAKEWPARAM(newVIEW, key), (LPARAM)text.operator LPCTSTR());
}

int CGuard::AtView(CWorks* works, CString maps, CString domino, bool force)
{
	if (force)
		works->OnDomino(domino);
	else
	{
		maps += domino;
		works->Attach(maps, false);
		works->m_view->RedrawWindow();
	}

	WINDOWPLACEMENT	wp;
	if (works->m_view->GetParent()->GetWindowPlacement(&wp) && wp.showCmd == SW_SHOWMINIMIZED)
	{
		wp.showCmd = SW_RESTORE;
		works->m_view->GetParent()->SetWindowPlacement(&wp);
		return works->m_key;
	}

	works->m_view->SetFocus();
	if (works->m_view->GetParent()->GetParent())
		PostMessage(works->m_view->GetParent()->GetParent()->GetSafeHwnd(), WM_MDIACTIVATE, (WPARAM)works->m_view->GetParent()->GetSafeHwnd(), 0);
	else
		works->m_view->GetParent()->SetFocus();
	return works->m_key;
}

void CGuard::SetProfit(CScreen* screen, CfmBase* form, CProfit*& profit)
{
	CString	tmps, text;
	char	twb[256];

	text.Format("#%s", (char *)form->m_form->name);
	GetPrivateProfileString("GRID Controls", text, "", twb, sizeof(twb), m_vtcode);
	tmps = twb; tmps += ' ';

	text.Format("$%s", (char *)form->m_form->name);
	GetPrivateProfileString("GRID Controls", text, "", twb, sizeof(twb), m_vtcode);
	tmps += twb;

	tmps.TrimRight();
	tmps.TrimLeft();
	if (tmps.IsEmpty())
		return;

	profit = new CProfit();

	char	sum;
	int	idx, nCols;
	CString	keys;
	struct _cellR	*cell;

	while (!tmps.IsEmpty())
	{
		idx = tmps.Find(' ');
		if (idx == -1)
		{
			text = tmps;
			tmps.Empty();
		}
		else
		{
			text = tmps.Left(idx++);
			tmps = tmps.Mid(idx);
			tmps.TrimLeft();
		}
		idx = text.FindOneOf("#$.");
		if (idx == -1)
			continue;
		sum = text.GetAt(idx);
		keys = text.Left(idx++);
		if (keys.IsEmpty())
			continue;
		text = text.Mid(idx);


		nCols = HIWORD(form->m_form->vals[1]);
		cell = (struct _cellR *) &screen->m_cellR[form->m_form->vals[0]];

		for (idx = 0; idx < nCols; idx++)
		{
			if (!text.CompareNoCase((char *)cell[idx].name))
			{
				if (sum == '#')
					profit->m_cell.SetAt(keys.GetAt(0), idx);
				else
				{
					CfmBase* base;

					if (screen->FindForm(keys, base))
					{
						if (sum == '$')
							profit->m_sum.SetAt(idx, base);
						else
							profit->m_sum.SetAt(idx|floatSUM, base);
					}
				}
				break;
			}
		}
	}

	if (!profit->m_cell.GetCount() && !profit->m_sum.GetCount())
	{
		delete profit;
		profit = NULL;
	}
	else
	{
		profit->m_name = (char *)form->m_form->name;

		profit->m_fees    = NULL;
		profit->m_taxs    = NULL;
		profit->m_moneys  = NULL;
		profit->m_charges = NULL;

		profit->m_fee    = 0;
		profit->m_tax    = 0;
		profit->m_money  = 0;
		profit->m_charge = false;

		text.Format("?%s", (char *)form->m_form->name);
		GetPrivateProfileString("GRID Controls", text, "", twb, sizeof(twb), m_vtcode);
		tmps = twb;

		while (!tmps.IsEmpty())
		{
			idx = tmps.Find(' ');
			if (idx == -1)
			{
				text = tmps;
				tmps.Empty();
			}
			else
			{
				text = tmps.Left(idx++);
				tmps = tmps.Mid(idx);
				tmps.TrimLeft();
			}
			idx = text.Find('?');
			if (idx == -1)
				continue;
			keys = text.Left(idx++);
			if (keys.IsEmpty())
				continue;
			text = text.Mid(idx);

			switch (keys.GetAt(0))
			{
			case 'I':
				screen->FindForm(text, profit->m_charges);
				break;
			case 'f':
				screen->FindForm(text, profit->m_fees);
				break;
			case 't':
				screen->FindForm(text, profit->m_taxs);
				break;
			case 'm':
				screen->FindForm(text, profit->m_moneys);
				break;
			case 'F':
				profit->m_fee = atof(text);
				break;
			case 'T':
				profit->m_tax = atof(text);
				break;
			case 'M':
				profit->m_money = atof(text);
				break;
			}
		}
	}
}

bool CGuard::SetCells(CString maps, CString& text)
{
	int	pos;
	char	wb[256];
	CString	syms, cells, defs;

	pos = text.Find('\n');
	if (pos == -1)
	{
		syms.Format(_T("%s$%s"), maps.GetString(), text.GetString());
		GetPrivateProfileString("GRID", syms, "", wb, sizeof(wb), m_modals);
		text = wb;
		text.TrimRight();
		return (bool)!text.IsEmpty();
	}

	syms.Format(_T("%s$%s"), maps.GetString(), LPCTSTR(text.Left(pos)));
	text = text.Mid(pos+1);
	pos  = text.Find('\n');
	if (pos == -1)
	{
		cells = _T("");
		defs  = _T("");
	}
	else
	{
		cells = text.Mid(pos+1);
		text  = text.Left(pos);
	}

	pos = cells.Find('\n');
	if (pos == -1)
		defs = _T("");
	else
	{
		defs  = cells.Mid(pos+1);
		cells = cells.Left(pos);
	}

	cells.TrimLeft(); cells.TrimRight();
	GetPrivateProfileString("GRID", syms, "", wb, sizeof(wb), m_modals);
	if (cells.IsEmpty() && strlen(wb))
		cells = wb;

	if (Modal(columnED, text, cells, defs))
	{
		WritePrivateProfileString("GRID", syms, cells, m_modals);
		text = cells;
		return true;
	}
	
	return false;
}

void CGuard::SetPalette()
{
	if (m_palette)
	{
		CString	tmps;
		CWorks* works;
		int	key;

		tmps.Format(_T("%s\\%s\\%s"), m_root.GetString(), TABDIR, PALETTE);
		m_palette->SetPalette((char *)tmps.operator LPCTSTR());
		for (POSITION pos = m_clients.GetStartPosition(); pos; )
		{
			m_clients.GetNextAssoc(pos, key, works);
			if (works->m_status & S_LOAD)
				works->SetPalette();
		}
	}
}

void CGuard::SetFCB(bool blink, int flash)
{
	int	key;
	CWorks* works;

	for (POSITION pos = m_clients.GetStartPosition(); pos; )
	{
		m_clients.GetNextAssoc(pos, key, works);
		if (works->m_status & S_LOAD)
			works->SetFCB(blink, flash);
	}
}

BOOL CGuard::SetFont(int point, bool resize, int key)
{
	CWorks*	works;

	m_resize = resize;
	if (key != -1)
	{
		if (GetClient(key, works) && (works->m_status & S_LOAD))
			return works->SetFont(point, resize);
		return FALSE;
	}

	m_font = point;
	if (m_resize)
		return TRUE;

	for (POSITION pos = m_clients.GetStartPosition(); pos; )
	{
		m_clients.GetNextAssoc(pos, key, works);
		if (works->m_status & S_LOAD)
			works->SetFont(point, resize);
	}
	return TRUE;
}

BOOL CGuard::Certify(BOOL force, BOOL certify, BOOL xcertify, BOOL xserver)
{
	if (certify)
	{
		BOOL	retv;
		CString	text;

		if (!m_certify)
		{
			m_certify = new CWnd();
			if (!m_certify->CreateControl("AxisCertify.CertifyCtrl.IBK2019", NULL, 0, CRect(0, 0, 0, 0), m_parent, 0))
			{
				delete m_certify;
				m_certify = NULL;
				if (!force)
				{
					AfxMessageBox("Xecure create fail");
					SetGuide(AE_ECERTCTRL);
				}
				return TRUE;
			}
		}

		if (certify && xcertify && !xserver)
		{
			m_certify->InvokeHelper(DI_CAEX, DISPATCH_METHOD, VT_BOOL, (void*)&retv, (BYTE *)(VTS_I4 VTS_I4), NULL, NULL);
			if (!retv)
				return FALSE;
		}

		if (!force && m_accno.GetSize() > 0)
		{
			text.Empty();
			for (int ii = 0; ii < m_accno.GetSize(); ii++)
			{
				text += m_accno.GetAt(ii);
				text += '\t';
			}
			m_certify->InvokeHelper(DI_CAEX, DISPATCH_METHOD, VT_BOOL, (void*)&retv, (BYTE *)(VTS_I4 VTS_I4), (long)text.operator LPCTSTR(), 0);
		}
	}
	else if (m_certify)
	{
		delete m_certify;
		m_certify = NULL;
	}
	return TRUE;
}

int CGuard::OnCertify(char* pBytes, int nBytes)
{
	if (!m_certify)
		return -1;

	long	retv;
	m_certify->InvokeHelper(DI_ONCA, DISPATCH_METHOD, VT_I4, (void*)&retv, (BYTE *)(VTS_I4 VTS_I4), pBytes, &nBytes);
	if (nBytes > 0)
	{
		if (m_term & flagXCS || Write(msgK_XCA, "AXISENCA", (char *)retv, nBytes))
			return nBytes;
		return -1;
	}
	return nBytes;
}

BOOL CGuard::CertifyErr(char* pBytes, int nBytes)
{
	if (!m_certify)
		return FALSE;

	BOOL	retv;
	m_certify->InvokeHelper(DI_CAERR, DISPATCH_METHOD, VT_BOOL, (void*)&retv,
						(BYTE *)(VTS_I4 VTS_I4), pBytes, nBytes);
	return retv;
}

BOOL CGuard::Certify(char* pBytes, int& nBytes, CString maps)
{
	if (!m_certify || !(m_status & WS_SELF))
	{
		SetGuide(AE_ECERTIFY);
		return FALSE;
	}

	BOOL	retv;
	m_certify->InvokeHelper(DI_CA, DISPATCH_METHOD, VT_BOOL, (void*)&retv,
					(BYTE *)(VTS_I4 VTS_I4 VTS_I4), pBytes, &nBytes, (char *)maps.operator LPCTSTR());
	return retv;
}

void CGuard::CertifyId(char* pBytes, bool retry)
{
	Certify(TRUE, TRUE);
	if (m_certify)
		m_certify->InvokeHelper(DI_CAID, DISPATCH_METHOD, VT_EMPTY, NULL, (BYTE *)(VTS_I4), pBytes);
	if (retry)
		Certify(FALSE, m_term & flagCA);
}

void CGuard::GetCetifyInfo(CString& infos)
{
	bool	emergency = false;

	if (m_term & flagCA && m_term & flagCAX)
		emergency = true;
	infos.Format("%c\t%c", emergency ? 'Y' : 'N', m_certify ? 'Y' : 'N'); 
}

void CGuard::OnDrag(int key, CString text)
{
#ifdef	_DEBUG
	if (AfxGetMainWnd() == NULL)
		return;
#endif
	COleDataSource	source;
	CSharedFile	sf(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	text.Insert(0, char(key));
	sf.Write(text, text.GetLength());
	source.CacheGlobalData(CF_TEXT, sf.Detach());
	source.DoDragDrop();
}

void CGuard::MakeXlsFile(CString format, CString text, bool visible, CString fileN, bool userdefine)
{
	if (format.IsEmpty())
	{
		MakeXlsFile(text, visible, fileN, userdefine);
		return;
	}

	int	row, col;
	int	vRows, nCols;
	CString	dests, string;

	nCols = format.GetLength();
	text.Remove('\r');
	vRows = text.Replace('\n', '\r');
	if (vRows == 0)
		return;

	if (!fileN.IsEmpty() && !userdefine)
	{
		dests.Format(_T("%s\\%s\\%s.xls"), LPCTSTR(m_root), USRDIR, LPCTSTR(fileN));
		fileN = dests;
	}
	else
	{
		string = "xls";
		if (fileN.IsEmpty() || !userdefine)
			fileN.Empty();
	}

	dests = GetFileName(fileN, "WorkSheet Files(*.xls)|*.xls|All Files(*.*)|*.*|", string);
	if (dests.IsEmpty())
		return;

	_Application	excel;
	if (!excel.CreateDispatch("Excel.Application"))
	{
		SetGuide(AE_SAVE);
		return;
	}

	bool	except = false;
	COleVariant FilenameSave(dests), Fileformat((short)-4143), covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	TRY
	{
		Workbooks	Wbs;
		_Workbook	Wb;
		Range		Rng;
		LPDISPATCH	lpDisp;
		COleSafeArray	sa;
		DWORD	numElements[2];
		long	index[2];
		COleVariant	var;

		CString	tmps;
		int	idx;
		const	int	alphas = 26;		// alphabet character #

		lpDisp = excel.GetWorkbooks();
		Wbs.AttachDispatch(lpDisp);

		/*
		CFileFind finder;
		if (finder.FindFile(dests))
		{
			lpDisp = Wbs.Open(dests, COleVariant((short)2),
					covOptional, covOptional, covOptional, covOptional, covOptional, covOptional,
					covOptional, covOptional, covOptional, covOptional, covOptional);
			Wb.AttachDispatch(lpDisp);
		}
		else
		*/
		{
			lpDisp = Wbs.Add(covOptional);
			Wb.AttachDispatch(lpDisp);
			Wb.SaveAs(FilenameSave, COleVariant((long)-4143), covOptional, covOptional, covOptional, COleVariant((short)FALSE),
					FALSE, covOptional, covOptional, covOptional, covOptional);
		}

		for (int col = 0; col < nCols; col++)
		{
			if (col < alphas)
			{
				string.Format("%c1", (char)('A'+col));
				tmps.Format("%c%d", (char)('A'+col), vRows);
			}
			else
			{
				string.Format("%c%c1", (char)('@'+col/alphas), (char)('A'+col%alphas));
				tmps.Format("%c%c%d", (char)('@'+col/alphas), (char)('A'+col%alphas), vRows);
			}
			Rng = excel.GetRange(COleVariant((LPCTSTR)string, VT_BSTR), COleVariant((LPCTSTR)tmps, VT_BSTR));

			switch (format.GetAt(col))
			{
			case 'T':						// text format
				Rng.SetNumberFormat(COleVariant("@"));
				break;
			default:
				break;
			}
		}

		numElements[0] = 1;
		numElements[1] = nCols;
		sa.Create(VT_VARIANT, 2, numElements);

		index[0] = 0;
		for (row = 0; row < vRows; )
		{
			row++;
			string.Format("A%d", row);
			if (nCols <= alphas)
				tmps.Format("%c%d", (char)('@'+nCols), row);		// for 'A' index
			else
				tmps.Format("%c%c%d", (char)('@'+nCols/alphas), (char)('@'+nCols%alphas), row);
			Rng = excel.GetRange(COleVariant((LPCTSTR)string, VT_BSTR), COleVariant((LPCTSTR)tmps, VT_BSTR));

			idx = text.Find('\r');
			string = text.Left(idx++);
			text = text.Mid(idx);
			if (string.IsEmpty())
				continue;

			for (col = 0; col < nCols; col++)
			{
				idx = string.Find('\t');
				if (idx == -1)
				{
					tmps = string;
					string.Empty();
				}
				else
				{
					tmps = string.Left(idx++);
					string = string.Mid(idx);
				}
				index[1] = col;
				sa.PutElement(index, COleVariant(tmps));
			}
			Rng.SetValue(sa);
		}
		sa.Clear();
		sa.Detach();

		Rng = Rng.GetEntireColumn();
		Rng.AutoFit();
		Rng.ReleaseDispatch();

		Wb.SetSaved(FALSE);
		Wb.Save();

		if (!visible)
			Wbs.Close();
	}
	CATCH(COleDispatchException, e)
	{
		except = true;
	//	::MessageBox(NULL, e->m_strDescription, e->m_strSource, MB_OK|MB_ICONERROR|MB_SYSTEMMODAL);
	}
	END_CATCH;

	if (!visible || except)
		excel.Quit();
	else
		excel.SetVisible(visible);
	excel.ReleaseDispatch();
}

void CGuard::MakeXlsFile(CString format, CString text)
{
	if (format.IsEmpty())
		return;

	int	row, col;
	int	vRows, nCols;
	CString	string;

	nCols = format.GetLength();
	text.Remove('\r');
	vRows = text.Replace('\n', '\r');
	if (vRows == 0)
		return;

	_Application	excel;
	if (!excel.CreateDispatch("Excel.Application"))
	{
		SetGuide(AE_SAVE);
		return;
	}

	bool	except = false;
	COleVariant  Fileformat((short)-4143), covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	TRY
	{
		Workbooks	Wbs;
		_Workbook	Wb;
		Range		Rng;
		LPDISPATCH	lpDisp;
		COleSafeArray	sa;
		DWORD	numElements[2];
		long	index[2];
		COleVariant	var;

		CString	tmps;
		int	idx;
		const	int	alphas = 26;		// alphabet character #

		lpDisp = excel.GetWorkbooks();
		Wbs.AttachDispatch(lpDisp);

		lpDisp = Wbs.Add(covOptional);
		Wb.AttachDispatch(lpDisp);
	//	Wb.SaveAs(FilenameSave, COleVariant((long)-4143), covOptional, covOptional, covOptional, COleVariant((short)FALSE),
	//			FALSE, covOptional, covOptional, covOptional, covOptional);
		excel.SetVisible(true);

		for (col = 0; col < nCols; col++)
		{
			if (col < alphas)
			{
				string.Format("%c1", (char)('A'+col));
				tmps.Format("%c%d", (char)('A'+col), vRows);
			}
			else
			{
				string.Format("%c%c1", (char)('@'+col/alphas), (char)('A'+col%alphas));
				tmps.Format("%c%c%d", (char)('@'+col/alphas), (char)('A'+col%alphas), vRows);
			}
			Rng = excel.GetRange(COleVariant((LPCTSTR)string, VT_BSTR), COleVariant((LPCTSTR)tmps, VT_BSTR));

			switch (format.GetAt(col))
			{
			case 'T':						// text format
				Rng.SetNumberFormat(COleVariant("@"));
				break;
			default:
				break;
			}
		}

		numElements[0] = 1;
		numElements[1] = nCols;
		sa.Create(VT_VARIANT, 2, numElements);

		index[0] = 0;
		for (row = 0; row < vRows; )
		{
			row++;
			string.Format("A%d", row);
			if (nCols <= alphas)
				tmps.Format("%c%d", (char)('@'+nCols), row);		// for 'A' index
			else
				tmps.Format("%c%c%d", (char)('@'+nCols/alphas), (char)('@'+nCols%alphas), row);
			Rng = excel.GetRange(COleVariant((LPCTSTR)string, VT_BSTR), COleVariant((LPCTSTR)tmps, VT_BSTR));

			idx = text.Find('\r');
			string = text.Left(idx++);
			text = text.Mid(idx);
			if (string.IsEmpty())
				continue;

			for (col = 0; col < nCols; col++)
			{
				idx = string.Find('\t');
				if (idx == -1)
				{
					tmps = string;
					string.Empty();
				}
				else
				{
					tmps = string.Left(idx++);
					string = string.Mid(idx);
				}
				index[1] = col;
				sa.PutElement(index, COleVariant(tmps));
			}
			Rng.SetValue(sa);
		}
		sa.Clear();
		sa.Detach();

		Rng = Rng.GetEntireColumn();
		Rng.AutoFit();
		Rng.ReleaseDispatch();

	//	Wb.SetSaved(FALSE);
	//	Wb.Save();
	}
	CATCH(COleDispatchException, e)
	{
		except = true;
	//	::MessageBox(NULL, e->m_strDescription, e->m_strSource, MB_OK|MB_ICONERROR|MB_SYSTEMMODAL);
	}
	END_CATCH;

	if (except)
		excel.Quit();
	excel.ReleaseDispatch();
}

void CGuard::MakeXlsFile(CString text, bool visible, CString fileN, bool userdefine)
{
	CString	dests, tmps, string;

	if (!fileN.IsEmpty() && !userdefine)
	{
		dests.Format(_T("%s\\%s\\%s.xls"), LPCTSTR(m_root), USRDIR, LPCTSTR(fileN));
		fileN = dests;
	}
	else
	{
		string = "xls";
		if (fileN.IsEmpty() || !userdefine)
			fileN.Empty();
	}

	dests = GetFileName(fileN, "WorkSheet Files(*.xls)|*.xls|All Files(*.*)|*.*|", string);
	if (dests.IsEmpty())
		return;

	int	idx = dests.ReverseFind('.');
	if (idx == -1)
		idx = dests.GetLength();
	tmps.Format(_T("%s.txt"), LPCTSTR(dests.Left(idx)));

	CFile	fileH;
	if (!fileH.Open(tmps, CFile::modeCreate|CFile::modeWrite))
	{
		SetGuide(AE_EFILE);
		return;
	}
	fileH.Write(text.operator LPCTSTR(), text.GetLength());
	fileH.Close();

	_Application	excel;
	if (!excel.CreateDispatch("Excel.Application"))
	{
		SetGuide(AE_SAVE);
		return;
	}

	bool	except = false;
	COleVariant FilenameSave(dests), Fileformat((short)-4143), covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	TRY
	{
		Workbooks	Wbs;
		_Workbook	Wb;
		LPDISPATCH	lpDisp;

		lpDisp = excel.GetWorkbooks();
		Wbs.AttachDispatch(lpDisp);
		lpDisp = Wbs.Open(tmps,
			covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional);

		Wb.AttachDispatch(lpDisp);
		Wb.SetSaved(FALSE);
		Wb.SaveAs(FilenameSave, Fileformat, covOptional, covOptional, covOptional, covOptional,
				7, covOptional, covOptional, covOptional, covOptional);

		if (!visible)
			Wbs.Close();
		DeleteFile(tmps);
	}
	CATCH(COleDispatchException, e)
	{
		except = true;
	//	::MessageBox(NULL, e->m_strDescription, e->m_strSource, MB_OK|MB_ICONERROR|MB_SYSTEMMODAL);
	}
	END_CATCH;

	if (!visible || except)
		excel.Quit();
	else
		excel.SetVisible(visible);
	excel.ReleaseDispatch();
}

BOOL CGuard::MakeTxtToXls(CString fileN, bool visible, bool deleteSrc)
{
	int	idx;
	CString	dests, tmps, string;

	if (fileN.IsEmpty())
	{
		CFileDialog fDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST,
				"Text Files(*.txt)|*.txt|All Files(*.*)|*.*|");
		if (fDlg.DoModal() != IDOK)
			return FALSE;

		fileN = fDlg.GetPathName();
	}

	idx = fileN.ReverseFind('.');
	if (idx == -1)
	{
		dests.Format(_T("%s.xls"), LPCTSTR(fileN));
		tmps.Format(_T("%s.txt"), LPCTSTR(fileN));
	}
	else
	{
		dests.Format(_T("%s.xls"), LPCTSTR(fileN.Left(idx)));
		tmps = fileN;
	}

	string = "xls";
	dests = GetFileName(dests, "WorkSheet Files(*.xls)|*.xls|All Files(*.*)|*.*|", string);
	if (dests.IsEmpty())
		return FALSE;

	AfxGetApp()->DoWaitCursor(TRUE);

	_Application	excel;
	if (!excel.CreateDispatch("Excel.Application"))
	{
		AfxGetApp()->DoWaitCursor(FALSE);
		SetGuide(AE_ECONVERT);
		return FALSE;
	}

	bool	except = false;
	COleVariant FilenameSave(dests), Fileformat((short)-4143), covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	TRY
	{
		Workbooks	Wbs;
		_Workbook	Wb;
		LPDISPATCH	lpDisp;

		lpDisp = excel.GetWorkbooks();
		Wbs.AttachDispatch(lpDisp);
		lpDisp = Wbs.Open(tmps,
			covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional);

		Wb.AttachDispatch(lpDisp);
		Wb.SetSaved(TRUE);
		Wb.SaveAs(FilenameSave, Fileformat, covOptional, covOptional, covOptional, covOptional,
				7, covOptional, covOptional, covOptional, covOptional);

		if (!visible)
			Wbs.Close();
		if (deleteSrc)
			DeleteFile(tmps);
	}
	CATCH(COleDispatchException, e)
	{
		except = true;
	//	::MessageBox(NULL, e->m_strDescription, e->m_strSource, MB_OK|MB_ICONERROR|MB_SYSTEMMODAL);
	}
	END_CATCH;

	if (!visible || except)
		excel.Quit();
	else
		excel.SetVisible(visible);

	excel.ReleaseDispatch();
	AfxGetApp()->DoWaitCursor(FALSE);
	return TRUE;
}

BOOL CGuard::MakeXlsToTxt(CString fileN, bool visible, bool deleteSrc)
{
	int	idx;
	CString	dests, tmps, string;

	if (fileN.IsEmpty())
	{
		CFileDialog fDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST,
				"WorkSheet Files(*.xls)|*.xls|All Files(*.*)|*.*|");
		if (fDlg.DoModal() != IDOK)
			return FALSE;

		fileN = fDlg.GetPathName();
	}

	idx = fileN.ReverseFind('.');
	if (idx == -1)
	{
		dests.Format(_T("%s.txt"), LPCTSTR(fileN));
		tmps.Format(_T("%s.xls"), LPCTSTR(fileN));
	}
	else
	{
		dests.Format(_T("%s.txt"), LPCTSTR(fileN.Left(idx)));
		tmps = fileN;
	}

	string = "txt";
	dests = GetFileName(dests, "Text Files(*.txt)|*.txt|All Files(*.*)|*.*|", string);

	AfxGetApp()->DoWaitCursor(TRUE);

	_Application	excel;
	if (!excel.CreateDispatch("Excel.Application"))
	{
		AfxGetApp()->DoWaitCursor(FALSE);
		SetGuide(AE_ECONVERT);
		return FALSE;
	}

	bool	except = false;
	COleVariant FilenameSave(dests), Fileformat((short)-4158), covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	TRY
	{
		Workbooks	Wbs;
		_Workbook	Wb;
		LPDISPATCH	lpDisp;

		lpDisp = excel.GetWorkbooks();
		Wbs.AttachDispatch(lpDisp);
		lpDisp = Wbs.Open(tmps,
			covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional);

		Wb.AttachDispatch(lpDisp);
		Wb.SetSaved(TRUE);
		Wb.SaveAs(FilenameSave, Fileformat, covOptional, covOptional, covOptional, covOptional,
				1, covOptional, covOptional, covOptional, covOptional);
		Wb.SetSaved(TRUE);
		if (!visible)
			Wbs.Close();
		if (deleteSrc)
			DeleteFile(tmps);
	}
	CATCH(COleDispatchException, e)
	{
		except = true;
	//	::MessageBox(NULL, e->m_strDescription, e->m_strSource, MB_OK|MB_ICONERROR|MB_SYSTEMMODAL);
	}
	END_CATCH;

	if (!visible || except)
		excel.Quit();
	else
		excel.SetVisible(visible);

	excel.ReleaseDispatch();
	AfxGetApp()->DoWaitCursor(FALSE);
	return TRUE;
}

bool CGuard::CreateObject(CString name, CCmdTarget*& target)
{
	return m_objects->CreateObject(name, target);
}

void CGuard::OnDDE(CString text, int key)
{
	m_dde->OnService(text, key);
}

void CGuard::OnDDE()
{
	m_dde->OnDDE();
}

char* CGuard::Modal(int kind, CString text)
{
	int	idx;
	CString	keys, currents, defs;

	keys.Empty();
	currents.Empty();
	defs.Empty();

	idx = text.Find('\t');
	if (idx != -1)
	{
		keys = text.Left(idx++);
		text = text.Mid(idx);
	}
	else	return NULL;

	idx = text.Find('\t');
	if (idx != -1)
	{
		currents = text.Left(idx++);
		text = text.Mid(idx);
	}
	else	return NULL;

	idx = text.Find('\t');
	if (idx != -1)
		defs = text.Left(idx);
	else
		defs = text;

	Modal(kind, keys, currents, defs);
	return (char*)currents.GetString();
}

bool CGuard::Modal(int kind, CString keys, CString& currents, CString defs)
{
	if (axDialog)
	{
		char*	ptr = (char *)axDialog(kind, (char *)m_root.operator LPCTSTR(), (char *)keys.operator LPCTSTR(),
						(char *)currents.operator LPCTSTR(), (char *)defs.operator LPCTSTR());
		if (ptr)
		{
			currents = CString(ptr);
			return true;
		}
	}

	return false;
}

CCmdTarget* CGuard::GetLedger(void* mapH)
{
	if (axLedger)
		return (CCmdTarget *)axLedger(mapH);
	return NULL;
}

CString CGuard::GetLedger(CCmdTarget* ledger, int pos, int length)
{
	if (axGetLedger)
	{
		char*	ptr = (char *)axGetLedger((void *)ledger, pos, length);
		if (ptr)
			return CString(ptr);
	}
	return _T("");
}

CString CGuard::GetLedger(CCmdTarget* ledger, int id)
{
	if (axGetLedgerEx)
	{
		char*	ptr = (char *)axGetLedgerEx((void *)ledger, id);
		if (ptr)
			return CString(ptr);
	}
	return _T("");
}

CString CGuard::GetLedger(CCmdTarget* ledger, char* data, int id)
{
	if (axGetLedgerData)
	{
		char*	ptr = (char *)axGetLedgerData((void *)ledger, (void *)data, id);
		if (ptr)
			return CString(ptr);
	}
	return _T("");
}

void CGuard::SetLedger(CCmdTarget* ledger, int pos, int length, char* data)
{
	if (axSetLedger)
		axSetLedger((void *)ledger, pos, length, data);
}

void CGuard::SetLedger(CCmdTarget* ledger, CString data)
{
	if (axLedgerEx)
		axLedgerEx(ledger, (void *)data.operator LPCTSTR());
}

void CGuard::Ledger(char* ledger)
{
	if (axLoginLedger)
		axLoginLedger((void *)ledger);
}

CString CGuard::GetLoginData(int id)
{
	if (axLoginData)
	{
		char*	ptr = (char *)axLoginData(id);
		CString stmp;
		stmp.Format("%s", ptr);
	//	AfxMessageBox(stmp);
		stmp.TrimRight();
		if (!stmp.IsEmpty())
			return stmp;
	//	if (ptr)
	//		return CString(ptr);
	}
	return _T("");
}

int CGuard::GetKey(int key)
{
	int	keys;
	CWorks* works;

	for (POSITION pos = m_clients.GetStartPosition(); pos; )
	{
		m_clients.GetNextAssoc(pos, keys, works);
		if (works->isYours(key))
			return works->m_key;
	}
	return key;
}

BOOL CGuard::GetCombo(char* key, CString& combo, bool parse)
{
	if (!m_combo.Lookup(key, combo))
		return FALSE;

	if (parse)
	{
		CString	text, string;
		int	offs, size;

		text = combo;
		size = text.Find('\t');
		if (size != -1)
		{
			combo = text.Left(size) + '\t';
			text  = text.Mid(size+1);
			for ( ; !text.IsEmpty(); )
			{
				offs = text.Find('\t');
				if (offs != -1)
				{
					string = text.Left(offs);
					text   = text.Mid(offs+1);
				}
				else
				{
					string = text;
					text   = _T("");
				}

				combo += string.Left(size);
				combo += '\t';
				combo += string.Mid(size);
				combo += '\n';
			}
		}

	}
	return TRUE;
}

void CGuard::SetCombo(CString text)
{
	CString	keys, combo;
	int	offs;

	offs = text.Find('\t');
	if (offs == -1)
		return;
	
	keys = text.Left(offs);
	text = text.Mid(offs+1);
	if (m_combo.Lookup(keys, combo))
	{
		offs = combo.Find('\t');
		if (offs != -1)
		{
			combo = combo.Mid(offs);
			combo.Insert(0, text);
			m_combo.SetAt(keys, combo);
		}
	}
}

void CGuard::OnRsm(struct _axisH* axisH, char* datB, int datL)
{
	CString	text;
	int	value;
	struct	_fileH*	fileH;

	xTRACE(x_RCVs, (char *)axisH, L_axisH+datL);

	fileH = (struct _fileH *)datB;
	datB += sizeof(struct _fileH);
	datL -= sizeof(struct _fileH);
	
	switch (fileH->fileK)
	{
	case fileK_MAP:
	case fileK_FILE:
	case fileK_XLS:
		OnFile(fileH, datB, datL);
		if (!(axisH->stat & statCON) && fileH->fileK == fileK_MAP
				&& (fileH->fileF == fileF_LAS || fileH->fileF == fileF_ONLY))
		{
			text = CString(fileH->fileN, sizeof(fileH->fileN));
			text.TrimRight();
			value = text.ReverseFind('/');
			if (value != -1)
				text = text.Mid(++value);
			value = text.Find('\0');
			if (value != -1)
				text = text.Left(value);
			SetVersion(text);
		}
		break;
	case fileK_VER:
		datL /= L_verM;
		if (datB <= 0)
			break;
		UpdateVers(datB, datL, true);
		break;
	case fileK_ERR:
		value = atoi(CString(fileH->datL, sizeof(fileH->datL)));
		text  = CString(datB, min(value, datL));
		SetGuide(text);

		text = CString(fileH->fileN, sizeof(fileH->fileN));
		text.TrimRight();
		value = text.ReverseFind('/');
		if (value != -1)
			text = text.Mid(++value);
		value = text.Find('\0');
		if (value != -1)
			text = text.Left(value);
		SetVersion(text);
		break;
	default:
		break;
	}
}

BOOL CGuard::LoadMenu(CString menus)
{
	BOOL	rc;
	CFileFind ff;
	CString	string;

	string.Format(_T("%s\\%s\\*.menu"), LPCTSTR(m_root), TABDIR);
	if (ff.FindFile(string))
	{
		for ( ; true; )
		{
			BOOL	rc = ff.FindNextFile();
			string = ff.GetFilePath();
			DeleteFile(string);
			if (!rc)
				break;
		}
		ff.Close();
	}

	rc = FALSE;
	if (!menus.IsEmpty())
	{
#pragma pack(1)
		struct	_menuR {
			char	name[12];
			int	size;
		};
#pragma pack()
		CFile	file;
		CFileStatus status;
		int	idx;
		char	*wb, *pMenu;
		struct	_menuR* menuR;

		string.Format(_T("%s\\%s\\%s"), LPCTSTR(m_root), RUNDIR, MENUDAT);
		if (!file.Open(string, CFile::modeRead))
		{
			SendAxis(MAKEWPARAM(menuAXIS, 0));
			return rc;
		}
		idx = (int)file.GetLength();
		wb  = new char[idx];
		file.Read(wb, idx);
		file.Close();

		menus.MakeLower();
		menuR = (struct _menuR*)wb;
		pMenu = &wb[sizeof(struct _menuR)*128];
		for (idx = 0; idx < 128; idx++, menuR++)
		{
			if (menuR->name[0] == '\0')
				break;

			string = CString(menuR->name);
			string.MakeLower();
			if (!string.Compare(menus))
			{
				string.Format(_T("%s\\%s\\%s"), LPCTSTR(m_root), TABDIR, AXISMENU);
				if (file.Open(string, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
				{
					rc = TRUE;
					file.Write(pMenu, menuR->size);
					file.Close();
				}
				break;
			}
			pMenu += menuR->size;
		}
		delete[] wb;
	}
	SendAxis(MAKEWPARAM(menuAXIS, 0));
	return rc;
}

void CGuard::isMenu(bool force)
{
	CString	path;
	CFile	file;
	CFileStatus status;

	path.Format(_T("%s\\%s\\%s"), LPCTSTR(m_root), RUNDIR, MENUDAT);
	if (CFile::GetStatus(path, status))
	{
		if ((status.m_attribute & (CFile::Attribute::readOnly|CFile::Attribute::hidden)) != CFile::Attribute::hidden)
		{
			status.m_attribute |= CFile::Attribute::hidden;
			status.m_attribute &= ~CFile::Attribute::readOnly;
			CFile::SetStatus(path, status);
		}
	}

	if (file.Open(path, CFile::modeReadWrite|CFile::typeBinary))
	{
#pragma pack(1)
		struct	_menuH	{
			unsigned char	mm;		// 0xfe
			unsigned long	time;
		} menuH;
#pragma pack()
		ULONGLONG sizeul = file.GetLength();

		if (sizeul > sizeof(struct _menuH))
		{
			file.Seek(sizeul - sizeof(struct _menuH), CFile::begin);
			file.Read(&menuH, sizeof(struct _menuH));
			if (menuH.mm != 0xfe)
			{
				if (!force)
				{
					menuH.mm    = 0xfe;
					menuH.time  = (unsigned long)time(0);
					menuH.time += 3;
					file.Write(&menuH, sizeof(struct _menuH));
				}
			}
			else if (force)
			{
				menuH.time -= (unsigned long)status.m_mtime.GetTime();
				if (menuH.time > 0 && menuH.time <= 3)
					force = false;
			}
		}
		else	force = true;

		file.Close();
		if (force)
			CFile::Remove(path);
	}
}

void CGuard::AddRegistry(char* datB, int datL, bool cbonly)
{
	if (datL <= 0)
		return;

	CString	string;

	if (cbonly)
	{
		m_combo.RemoveAll();
		AddRegistry(datB, datL, string);
		return;
	}
	m_accno.RemoveAll();
	m_account.RemoveAll();
	AddRegistry(datB, datL, string);
	PostAxis(MAKEWPARAM(accnINFO, 0));
}

void CGuard::OnAccount()
{
	CWorks*	works;
	int	key;

	for (POSITION pos = m_clients.GetStartPosition(); pos; )
	{
		m_clients.GetNextAssoc(pos, key, (CWorks*&)works);
		if (!(works->m_status & S_LOAD) || (works->m_status & S_WORKS))
			continue;
		if (works->isWorks())
			((CClient *)works)->OnAccount();
		else
			((CDll *)works)->m_dll->PostMessage(WM_USER, MAKEWPARAM(DLL_ACCOUNT, 0));
	}
}

void CGuard::OnFile(struct _fileH* fileH, char* datB, int datL)
{
	CFile	file;
	CString	path;
	int	value;

	value = atoi(CString(fileH->datL, sizeof(fileH->datL)));
	if (value > datL)
		return;

	path.Format(_T("%s/%s"), m_root.GetString(), fileH->fileN);
	switch (fileH->fileF)
	{
	case fileF_LAS:
	case fileF_MID:
		if (!file.Open(path, CFile::modeCreate|CFile::modeNoTruncate|CFile::typeBinary|CFile::modeWrite))
			return;
		file.SeekToEnd();
		break;
	case fileF_ONLY:
	case fileF_FIR:
	default:
		if (!file.Open(path, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite))
			return;
		break;
	}

	file.Write(datB, value);
	file.Close();

	switch (fileH->fileK)
	{
	case fileK_XLS:
		break;
	case fileK_MAP:
		if (fileH->fileF == fileF_FIR || fileH->fileF == fileF_ONLY)
		{
			int	idx;

			idx = path.ReverseFind('/');
			if (idx != -1)
				path = path.Mid(++idx);
			UpdateVersion(path);
		}
		[[fallthrough]];
	default:
		return;
	}

	CString	excelN;
	
	value = path.ReverseFind('.');
	excelN = (value == -1) ? path : path.Left(value);

	if (path.CompareNoCase(excelN + ".txt"))
		MoveFile(path, excelN + ".txt");
	path = excelN + ".txt";

	excelN += ".xls";

	_Application	excel;
	SetGuide(AE_CNVXLS, 0);
	if (!excel.CreateDispatch("Excel.Application"))
	{
		SetGuide(AE_ERRXLS, 0);
		return;
	}

	COleVariant
		FilenameSave(excelN), 
		Fileformat((short)-4143),
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	TRY 
	{
		Workbooks	Wbs;
		_Workbook	Wb;
		LPDISPATCH	lpDisp;
		
		lpDisp = excel.GetWorkbooks();
		Wbs.AttachDispatch(lpDisp);
		lpDisp = Wbs.Open(path,
			covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional);

		Wb.AttachDispatch(lpDisp);
		Wb.SetSaved(TRUE);
		Wb.SaveAs(FilenameSave, Fileformat, covOptional, covOptional,
			covOptional, covOptional, 7, covOptional,
			covOptional, covOptional, covOptional);
		Wbs.Close();
		DeleteFile(path);
	} 
	CATCH(COleDispatchException, e) 
	{ 
		path = "Excel-File ERROR : "+e->m_strDescription;
		SetGuide(path, 0);
	} 
	END_CATCH; 

	excel.Quit();
	excel.ReleaseDispatch();
}

void CGuard::xTRACE(int type, char* trace, int size)
{
	if (m_hTrace)
	{
		switch (type)
		{
		case x_STRs:
		case x_RTMs:
			size = strlen(trace);
			break;
		}
		::SendMessage(m_hTrace, m_trace, MAKEWPARAM(size, type), (LPARAM)trace);
	}
}

BOOL CGuard::OnPrint(CString text, CString name, CString fName, int fPoint)
{
	CDC	printDC;

	CPrinter* printer = new CPrinter(m_app);
	printer->CopyDefaultMfcPrinter();

	printer->SetThisPrinter();
	m_app->CreatePrinterDC(printDC);
	if (!printDC)
	{
		delete printer;
		return FALSE;
	}

	SetMapMode(printDC.m_hDC, MM_TEXT);
	DOCINFO docinfo = { sizeof(DOCINFO), name, NULL };
 	if (!StartDoc(printDC.m_hDC, &docinfo))
	{
		printDC.DeleteDC();
		delete printer;
		return FALSE;
	}

	int	paperX = GetDeviceCaps(printDC.m_hDC, HORZRES);
	int	paperY = GetDeviceCaps(printDC.m_hDC, VERTRES);
	int	printX = GetDeviceCaps(printDC.m_hDC, LOGPIXELSX);
	int	printY = GetDeviceCaps(printDC.m_hDC, LOGPIXELSY);
	int	pageGapx, pageGapy;

	CFont	Font, *font;
	CRect	pRc;

	pageGapx = pageGapy = 0;
	pRc.SetRect(pageGapx, pageGapy, paperX, paperY);

	int	fontS = fPoint*10*printX/LOGPIXEL_X;
	Font.CreatePointFont(fontS, fName);
	font = printDC.SelectObject(&Font);
	if (StartPage(printDC.m_hDC))
	{
		printDC.DrawText(text, &pRc, 0);
		EndPage(printDC.m_hDC);
	}

	EndDoc(printDC.m_hDC);
	printer->RestorePrinter();

	printDC.SelectObject(font);
	DeleteObject(Font);
	printDC.DeleteDC();
	delete printer;
	return TRUE;
}

void CGuard::SetClipboard(CString text)
{
	if (m_parent->OpenClipboard())
	{
		int	size = text.GetLength();
		EmptyClipboard();
		HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, size+1);
		if (hg != NULL)
		{
			char*	pp = (char *)GlobalLock(hg);
			if (pp != (char *) 0)
			{
				CopyMemory(pp, (char*)text.operator LPCSTR(), size); pp[size] = '\0';
			}
			GlobalUnlock(hg);
			SetClipboardData(CF_TEXT, hg);
		}
		CloseClipboard();
	}
}

bool CGuard::GetClipboard(CString& text)
{
	text.Empty();

	if (m_parent->OpenClipboard())
	{
		HGLOBAL hg = GetClipboardData(CF_TEXT);
		if (hg != NULL)
		{
			text = (char *)GlobalLock(hg);
			GlobalUnlock(hg);
		}
		CloseClipboard();
		return hg ? true : false;
	}
	return false;
}

bool CGuard::GetString(int code, CString& text)
{
	CString	tmps, path;
	char	tmpb[128];

	path.Format(_T("%s\\%s\\axstring.dat"), m_root.GetString(), MTBLDIR);
	tmps.Format("%04d", code);
	GetPrivateProfileString("String Tables", tmps, "", tmpb, sizeof(tmpb), path);
	text = tmpb;
	text.TrimRight();
	return text.IsEmpty() ? false : true;
}

CString CGuard::GetFileName(CString fileN, CString filter, CString& ext, bool force)
{
	DWORD	attr;
	CString	tmps, string;

	for (; true; )
	{
		if (fileN.IsEmpty() || force)
		{
			CFileDialog fDlg(FALSE, ext, fileN, OFN_HIDEREADONLY, filter);
			if (fDlg.DoModal() != IDOK)
				return _T("");

			fileN = fDlg.GetPathName();
			ext = fDlg.GetFileExt();
		}

		int	offs = fileN.ReverseFind('.');
		if (offs == -1)
			ext = "xls";
		else
		{
			ext = fileN.Mid(offs + 1);
			fileN = fileN.Left(offs);
			if (ext.IsEmpty())
				ext = "xls";
		}

		fileN += ".";
		fileN += ext;

		attr = GetFileAttributes(fileN);
		if (attr != 0xffffffff && !(attr & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (GetString(AS_FILE, tmps))
				string.Format(tmps, fileN);
			else
				string.Format(_T("�� ��ġ��  \'%s\' ������ �ֽ��ϴ�. ������ ������(��) �ٲٽðڽ��ϱ�?"), fileN.GetString());

			int	rc = ::MessageBox(NULL, string, "Excel", MB_YESNOCANCEL | MB_ICONINFORMATION);
			switch (rc)
			{
			case IDYES:
				if (!DeleteFile(fileN))
				{
					if (GetString(AS_USING, tmps))
						string.Format(tmps, fileN);
					else
						string.Format(_T("\'%s\' ������ ������Դϴ�. ������� ������ �ݰ� �ٽ� �õ��� �ֽʽÿ�."), fileN.GetString());
					::MessageBox(NULL, string, "Excel", MB_OK | MB_ICONINFORMATION);
					return _T("");
				}
				break;

			case IDNO:
				fileN.Empty();
				continue;

			case IDCANCEL:
				return _T("");
			}
		}
		break;
	}

	return fileN;
}

CString CGuard::GetLoginPass()
{
	char	wb[32] = { 0, };

	SendAxis(MAKEWPARAM(userPASS, 0), (LPARAM)wb);
	return CString(wb, strlen(wb));
}

CString CGuard::GetCAPass()
{
	char	wb[64];

	SendAxis(MAKEWPARAM(userCPASS, 0), (LPARAM)wb);
	return CString(wb, strlen(wb));
}

int CGuard::GetFocus()
{
	int	key;
	CClient* client;

	for (POSITION pos = m_clients.GetStartPosition(); pos; )
	{
		m_clients.GetNextAssoc(pos, key, (CWorks *&)client);
		if (client->isWorks() && client->m_focus)
			return key;
	}
	return 0;
}

int CGuard::GetOwner(int key)
{
	int	keyx;
	CWorks* works;

	for (POSITION pos = m_clients.GetStartPosition(); pos; )
	{
		m_clients.GetNextAssoc(pos, keyx, works);
		if (works->isYours(key))
			return keyx;
	}
	return key;
}

bool CGuard::GetCode(CString name, CString& code, bool up)
{
	int	ii;
	CCode*	codex;

	if (!m_codex.Lookup(name, (CObject*&)codex))
		return false;


	for (ii = 0; ii < codex->m_codes.GetSize(); ii++)
	{
		if (!code.CompareNoCase(codex->m_codes.GetAt(ii)))
			break;
	}

	if (ii >= codex->m_codes.GetSize())
		return false;

	if (up)
	{
		ii--;
		if (ii < 0)
			ii = codex->m_codes.GetUpperBound(); 
	}
	else
	{
		ii++;
		if (ii > codex->m_codes.GetUpperBound())
			ii = 0;
	}

	code = codex->m_codes.GetAt(ii);
	return true;
}

void CGuard::SetCursor(int id, CWnd* pWnd)
{
	HCURSOR	hcur;

	if (m_cursors.Lookup((WORD)id, (void *&)hcur))
	{
		if (hcur == NULL)
			hcur = ::LoadCursor(NULL, IDC_ARROW);
		if (hcur && hcur != ::GetCursor())
			::SetCursor(hcur);
	}

	if (pWnd)
	{
		TRACKMOUSEEVENT	csTME;

		csTME.cbSize = sizeof(csTME);
		csTME.dwFlags = id ? TME_LEAVE : TME_CANCEL;
		csTME.hwndTrack = pWnd->GetSafeHwnd();
		::_TrackMouseEvent(&csTME);
	}
}

HCURSOR CGuard::GetCursor(int id)
{
	HCURSOR	hcur;

	if (m_cursors.Lookup((WORD)id, (void *&)hcur))
		return hcur;
	return NULL;
}

void CGuard::ClearFocus(int key)
{
	int	keyx;
	CWorks*	works;

	for (POSITION pos = m_clients.GetStartPosition(); pos; )
	{
		m_clients.GetNextAssoc(pos, keyx, works);
		if (keyx != key && (works->m_status & S_LOAD) && works->isWorks())
			((CClient *)works)->m_focus = FALSE;
	}
}

void CGuard::InternalTrigger(CClient* client, CString procs, CString param, CString maps, BOOL setfocus)
{
	CWorks*	works;
	int	key, owner;

	if (maps.IsEmpty() || !(client->m_status & S_WORKS))
		return;
	
	owner = GetOwner(client->m_key);
	for (POSITION pos = m_clients.GetStartPosition(); pos; )
	{
		m_clients.GetNextAssoc(pos, key, works);
	//	if (works == client || !(works->m_status & S_WORKS))
	//		continue;
		if (!(works->m_status & S_WORKS))
			continue;
		if (!works->m_mapN.CompareNoCase(maps) && owner == GetOwner(works->m_key))
		{
			if (setfocus)
			{
				works->m_view->SetFocus();
				if (works == client)
					continue;
			//	ClearFocus(works->m_key);
			}
			works->OnProcedure(procs, param);
		}
	}
}

BOOL CGuard::IsWait(CClient* client)
{
	if (client->m_status & S_WAIT)
		return TRUE;

	if (client->m_status & S_WORKS)
	{
		CWorks* works;
		int	owner = GetOwner(client->m_key);

		if (m_clients.Lookup(owner, works))
		{
			if (((CDll*)works)->IsWait(0))
				return TRUE;
			return ((CDll*)works)->IsWait();
		}
	}
	return FALSE;
}

void CGuard::DoRTM(CString code, int stat, CdataSet* fms, CObArray* obs, CString updates)
{
	CString dbg;
	dbg.Format("[WIZARD][RTM][DEBUG] tick code=%s clients=%d\n", code.GetString(), (int)m_clients.GetCount());
	OutputDebugString(dbg);

#ifdef DF_RTM_INDEX   //CGuard::DoRTM
	{
		void*	ptr;
		int	idxCount = 0;
		if (m_codeIndex.Lookup(code, ptr))
			idxCount = (int)((CPtrArray*)ptr)->GetSize();
		CString dbgIdx;
		dbgIdx.Format("[WIZARD][RTM][DEBUG][INDEX] code=%s indexCount=%d\n", code.GetString(), idxCount);
		OutputDebugString(dbgIdx);
	}
#endif

	int	key;
	POSITION pos;
	CWorks* works;
	CString	data;

	if (fms == NULL || !fms->IsOn())
		return;

	if (stat & stat_SCR)
	{
		if (obs == NULL || (obs && (obs->GetSize() <= 0 || (obs->GetSize() > 0 && !((CdataSet *)obs->GetAt(0))->IsOn()))))
			return;
	}

	data = code + "\t" + updates;
	xTRACE(x_RTMs, (char*)data.operator LPCTSTR());

/*	if (stat & stat_TICK)
		m_parent->SendMessage(WM_ANM, stat & stat_TICK, (long)updates.operator LPCTSTR());
	else
		m_parent->SendMessage(WM_ANM, stat, (long)data.operator LPCTSTR());
*/
	if (stat & stat_SCR)
	{
		if (fms == NULL)
		{
			key  = updates.Find('\n');
			data = updates.Left(key);
		}
		else	data = MakeString(fms, false);
	}
	else	data = updates;
	m_dde->OnAlert(code, data);

	m_sync.Lock();
	m_alertR.code = code;
	m_alertR.stat = stat;
	if (stat & stat_SCR)
	{
		int	idx, size = obs->GetSize();

		while (true)
		{
			if (size > maxREC)
			{
				m_alertR.size = maxREC;
				size -= maxREC;
				idx = size;
			}
			else
			{
				m_alertR.size = size;
				idx = 0;
			}
			for (int ii = 0; ii < m_alertR.size; ii++)
				m_alertR.ptr[ii] = (DWORD)((CdataSet *)obs->GetAt(idx+ii))->GetData();
			if (idx == 0)
				break;
		}
	}
	else
	{
		m_alertR.size = 1;
		m_alertR.ptr[0] = (DWORD)fms->GetData();
	}

	m_parent->SendMessage(WM_ANM, 0, (LPARAM)&m_alertR);		// Array �������� ����
	for (pos = m_clients.GetStartPosition(); pos; )
	{
		m_clients.GetNextAssoc(pos, key, works);
		if (works->m_status & S_LOAD)
		{
			if (works->isWorks())
				((CClient *)works)->OnAlert(code, updates, fms, obs, stat, &m_alertR);
			else
			{
				works->OnAlert(code, updates, stat);
				((CDll *)works)->OnAlert((void *)&m_alertR);
			}
		}
	}
	m_sync.Unlock();
}

#ifdef DF_RTM_INDEX  //CGuard::UpdateCodeIndex
void CGuard::UpdateCodeIndex(CScreen* screen, CString oldCode, CString newCode)
{
	void*		ptr;
	CPtrArray*	arr;

	if (!oldCode.IsEmpty() && m_codeIndex.Lookup(oldCode, ptr))
	{
		arr = (CPtrArray*)ptr;
		for (int ii = 0; ii < arr->GetSize(); ii++)
		{
			if (arr->GetAt(ii) == screen)
			{
				arr->RemoveAt(ii);
				break;
			}
		}
	}

	if (!newCode.IsEmpty())
	{
		if (!m_codeIndex.Lookup(newCode, ptr))
		{
			arr = new CPtrArray();
			m_codeIndex.SetAt(newCode, arr);
		}
		else
			arr = (CPtrArray*)ptr;
		arr->Add(screen);
	}
}
#endif

BOOL CGuard::IsPortfolioSymbol(CString name)
{
	void*	ptr;

	return m_interests.Lookup(name, (void *&)ptr);
}

//bool CGuard::ReadGroup(CWnd* wnd, CStringArray& arr)
//{
//	arr.RemoveAll();
//	if (axReadGroup)
//	{
//		CString	text = axReadGroup((int)wnd, (char *)m_uname.operator LPCTSTR());
//		if (!text.IsEmpty())
//		{
//			int	idx;
//			CString	string;
//
//			for ( ; !text.IsEmpty(); )
//			{
//				idx = text.Find('\t');
//				if (idx != -1)
//				{
//					string = text.Left(idx++);
//					text = text.Mid(idx);
//				}
//				else
//				{
//					string = text;
//					text.Empty();
//				}
//				if (!string.IsEmpty())
//					arr.Add(string);
//			}
//
//			if (arr.GetSize() > 0)
//				return true;
//		}
//	}
//	return false;
//}
//
//bool CGuard::WriteCode(CWnd* wnd, CString group, CString code)
//{
//	if (axWriteCode)
//	{
//		if (axWriteCode((int)wnd, group.IsEmpty() ? true : false, (char *)m_uname.operator LPCTSTR(),
//				(char *)group.operator LPCTSTR(), (char *)code.operator LPCTSTR()))
//		{
//			OnProcedure(_T("OnPortfolio"), _T("ok"), _T(""));
//			return true;
//		}
//	}
//
//
///*	if (!code.IsEmpty())
//	{
//		OnProcedure(_T("OnPortfolio"), _T("ok"), _T(""));
//		return true;
//	}*/		
//	return false;
//}

bool CGuard::GetFdsValue(CString trN, char* datB, int& datL)
{
	if (m_fds == NULL)
	{
		CString	path;

		path.Format(_T("%s\\%s\\CX_FDS.dll"), LPCTSTR(m_root), DEVDIR);
		LOG_OUTP(3, "[LOADLIB", path, __FUNCTION__);
		m_fds = AfxLoadLibrary(path);
		if (m_fds == NULL)
			return false;

		axFDSValue = (int (APIENTRY*)(char*, char*, int, char*))GetProcAddress(m_fds, _T("axFDSValue"));
	}

	if (axFDSValue)
	{
		char	fdsB[1024*2];

		ZeroMemory(fdsB, sizeof(fdsB));
		int	rc = (*axFDSValue)((char *)trN.operator LPCTSTR(), datB, datL, fdsB);
		if (rc > 0)
		{
			struct	_fdsR*	fdsR;
			char*	ptr = new char[datL];

			CopyMemory(ptr, datB, datL);
			fdsR = (struct _fdsR *)datB;
			ZeroMemory(fdsR, L_FDSR);
			fdsR->fdsL = rc;
			CopyMemory(&datB[L_FDSR], ptr, datL);
			datL += L_FDSR;
			CopyMemory(&datB[datL], fdsB, rc);
			datL += rc;
			delete[] ptr;
			return true;
		}
	}
	return false;
}

CString CGuard::MakeString(CdataSet* fms, bool newline)
{
	CString	text, string, data;

	text = _T("");
	for (int ii = 0; ii < maxSYM; ii++)
	{
		if (fms->Lookup(ii, data))
		{
			string.Format(_T("%03d\t%s\t"), ii, LPCTSTR(data));
			text += string;
		}
	}
	if (newline)
		text += "\n";
	return text;
}

size_t CGuard::strcspnx(const char* s, const char reject)
{
	unsigned int length = 0;
	while (*s != '\0' && *s != reject)
	{
		s++;
		length++;
	}
	return length;
}

char* CGuard::strtokx(char* str, const char delim, char** start)
{
	char *sbegin, *send;
	char *save = "";

	if (str != NULL)
		sbegin = str;
	else
		sbegin = *start;
		
	// calculate starting point of token
	//sbegin += strspnx(sbegin, delim);

	if (*sbegin == '\0')
	{
		save = "";
		return NULL;
	}
	
	// calulate ending point of token
	send = sbegin + strcspnx(sbegin, delim);

	if (*send != '\0')
		*send++ = '\0';
		
	save = send;

	*start = save;
	return sbegin;
}

long CGuard::CertifyFull(CString srcB, int srcL, char* desB, int& desL)
{
	desL = 0;
	Certify(TRUE, TRUE);
	if (!m_certify)
		return -1;

	long	retv, outL;
	m_certify->InvokeHelper(DI_CAFULL, DISPATCH_METHOD, VT_I4, (void*)&retv,
			(BYTE *)(VTS_I4 VTS_I4 VTS_I4 VTS_I4), (char*) srcB.operator LPCTSTR(), srcL, desB, &outL);
	desL = outL;
	return retv;
}

long CGuard::CertifyName(char* datB)
{
	Certify(TRUE, TRUE);
	if (!m_certify)
		return 0;

	long	retv;
	m_certify->InvokeHelper(DI_CANAME, DISPATCH_METHOD, VT_I4, (void*)&retv, (BYTE *)(VTS_I4), datB);
	return retv;
}

// 
// password encrypt / lz4
//
bool CGuard::lz4enc(CString& src)
{
	//int	cmpL = 0;
	//char	cmpB[1024]{ 0, };

	//cmpL = m_lz4.compress((char *) src.GetString(), src.GetLength(), cmpB, sizeof(cmpB));
	//if (cmpL > 0)
	//{
	//	for (int ii = 0; ii < cmpL; ii++)
	//		cmpB[ii] += 0x20;

	//	src = CString(cmpB, cmpL);
	//}

	return true;
}

bool CGuard::lz4dec(CString& src)
{
	//int	dmpL = 0;
	//char	dmpB[1024]{ 0, };

	//dmpL = src.GetLength();
	//strcpy_s(dmpB, sizeof(dmpB), (char *) src.GetString());
//	for (int ii = 0; ii < dmpL; ii++)
//		dmpB[ii] -= 0x20;
//	src = CString(dmpB, dmpL);



	//dmpL = m_lz4.decompress((char*)src.GetString(), src.GetLength(), dmpB, sizeof(dmpB));
	//if (dmpL > 0)
	//	src = CString(dmpB, dmpL);

	return true;
}

int CGuard::lz4dec(char* inB, int inL, char* outB, int outL)
{
	if (!inB || !outB || inL <= 0 || outL <= 0)
		return -1;

	return m_lz4.decompress(inB, inL, outB, outL);
}

int CGuard::CertifyCloude(LONG gubn)
{
	if (!m_certify)
		return 0;

	long	retv{};
	m_certify->InvokeHelper(DI_CLOUD, DISPATCH_METHOD, VT_I4, (void*)&retv, (BYTE*)(VTS_I4 ), gubn);
	return retv;
}