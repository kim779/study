// Guard.cpp: implementation of the CGuard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
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
#include "../h/axisrsm.h"
#include "../h/axisanm.h"
#include "../h/axiserr.h"
#include "../h/axstring.h"
#include "../dll/form/fmgrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	HISTORYs	32		// history entry #

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuard::CGuard()
{
	m_app     = AfxGetApp();
	m_sock    = new CWnd();
	m_certify = NULL;
	m_xecure  = NULL;
	m_symbol  = NULL;
	m_secure  = NULL;

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
	m_resize  = false;

	m_vid     = 0;
	m_tips    = new CTips(this);
	m_hTrace  = NULL;

	m_CODEs   = 6;				// default size of symbol
	m_master.Empty();

	for (int ii = winK_NORM; ii < winK_RMN; ii++)
		m_major.Add(ii);

	m_certifyVer = caVER1;
	m_otp.Empty();

	m_cast = NULL;
	m_addOnly = false;
	m_rtm.RemoveAll();
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
	if (m_cast)
		delete m_cast;
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

	m_vers.RemoveAll();

	if (m_objects)
		delete m_objects;
	delete m_xsystem;
	delete m_tips;

	m_major.RemoveAll();

	CObject* ob;
	WORD	wkey;
	for (pos = m_rtm.GetStartPosition(); pos; )
	{
		m_rtm.GetNextAssoc(pos, wkey, (CObject *&)ob);
		delete ob;
	}
	m_rtm.RemoveAll();
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
	if (m_root.IsEmpty())
		return -1;

	if (!m_sock->CreateControl("AxisSock.SockCtrl.ERGATE", NULL, 0, CRect(0), control, 0))
		return -1;

	m_dde->Create(NULL, NULL, 0, CRect(0), control, 0);
	m_parent = control;

	m_vtcode.Format("%s\\%s\\%s", m_root, TABDIR, VTCODE);
	m_repository.Format("%s\\%s\\Repository", m_root, TABDIR);
	m_userini.Format("%s\\%s\\%s", m_root, TABDIR, AXISUSER);
	m_modals.Format("%s\\%s\\%s\\modal.ini", m_root, USRDIR, m_uname);

	CString	tmps;
	char	twb[256];

	GetEnvironmentVariable("path", twb, sizeof(twb));
	tmps.Format("%s;%s\\%s;%s\\%s", twb, m_root, RUNDIR, m_root, DEVDIR);
	SetEnvironmentVariable("path", tmps);

	m_loginx = AfxLoadLibrary("AxisLogin.dll");
	if (m_loginx == NULL)
	{
		axLogin         = NULL;
		axLoginLedger   = NULL;
		axLedger        = NULL;
		axLedgerEx      = NULL;
		axGetLedger     = NULL;
		axSetLedger     = NULL;
		axGetLedgerEx   = NULL;
		axGetLedgerData = NULL;
	}
	else
	{
		axLogin         = (void* (APIENTRY*)(void*))GetProcAddress(m_loginx, _T("axLogin"));
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
	return 0;
}

void CGuard::Startup()
{
	if (m_alive)	return;

	void	(APIENTRY* axCreateEx)(void* root);
	m_symbol = AfxLoadLibrary("AxisCode.dll");
	if (m_symbol == NULL)
	{
		axCreateEx = NULL;
		axGetCode  = NULL;
		axGetName  = NULL;
	}
	else
	{
		axCreateEx = (void (APIENTRY*)(void*))GetProcAddress(m_symbol, _T("axCreateEx"));
		axGetCode  = (bool (APIENTRY*)(int, char*, char*, int, CPoint))GetProcAddress(m_symbol, _T("axGetCode"));
		axGetName  = (bool (APIENTRY*)(int, char*, char*, int*))GetProcAddress(m_symbol, _T("axGetName"));
	}
	if (axCreateEx)
		axCreateEx((void*)m_root.operator LPCTSTR());

	m_dialog = AfxLoadLibrary("AxisDialog.dll");
	if (m_dialog == NULL)
		axDialog = NULL;
	else
		axDialog = (void* (APIENTRY*)(int, char*, char*, char*))GetProcAddress(m_dialog, _T("axDialog"));

	CString	tmps;
	tmps.Format("%s\\%s\\%s", m_root, TABDIR, PALETTE);

	m_palette = new CAxisPalette((char *)tmps.operator LPCTSTR());

	m_flash  = GetPrivateProfileInt(GENERALSN, FLASHSN, 99, tmps);
	m_wtype = m_app->GetProfileInt(ENVIRONMENT, WNDINFO, szMAP);
	m_wtype = HIWORD(m_wtype);

	m_wait = m_app->GetProfileInt(WORKSTATION, TRANTMO, 0);
	m_wait *= 1000;

	char	tmpb[256];
	GetPrivateProfileString(SYSTEMSN, "Pass", "", tmpb, sizeof(tmpb), m_vtcode);
	m_verpass = tmpb;
	m_verpass.MakeUpper();

	int	idx;
	CString	name, text, arrays;
	CCode*	code;

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
		name.TrimLeft();
		name.TrimRight();
		if (name.IsEmpty())
			continue;

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
			tmps.TrimLeft();
			tmps.TrimRight();
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

	char*	(APIENTRY* axGetMaster)(char* root, int* ncode);
	HINSTANCE hCode = AfxLoadLibrary("AxisCodx.dll");
	if (hCode)
	{
		axGetMaster = (char* (APIENTRY*)(char*, int*))GetProcAddress(hCode, _T("axGetMaster"));
		if (axGetMaster)
		{
			m_master = axGetMaster((char *)m_root.operator LPCTSTR(), &m_CODEs);
			if (!m_master.IsEmpty())
				m_master.Insert(0, '\t');
		}
		AfxFreeLibrary(hCode);
	}

	LoadSecuritys();
	m_alive = true;

	if (m_cast) m_cast->ReloadFMT(m_root);
}

void CGuard::Cleanup()
{
	if (m_alive)
	{
		CString	name, namex, text, arrays;
		CCode*	code;
		int	kospi;

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

		wsprintf(path, "%s/%s/%s", m_root, USRDIR, pushN);
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
	CStringArray*	rtmk;
	CMapStringToOb	quote;		// parsed data for MAP
	CStringArray	codes;		// code array (on addOnly option)
	CObArray	quotes;		// data array (on addOnly option)
	CMapStringToOb	scroll;	
	POSITION	pos;

	CdataSet*	rts;
	CObArray*	obs;
	bool	info = true;

	quote.RemoveAll();
	codes.RemoveAll();
	quotes.RemoveAll();
	scroll.RemoveAll();
	for (; nBytes > L_rtmH; )
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
			m_fsection.Lock();
			if (m_filter.Lookup(rtmH->datK, idx))
			{
				m_fsection.Unlock();
				break;
			}
			m_fsection.Unlock();

			if (!m_rtm.Lookup(rtmH->datK, (CObject *&)rtmk))
				break;

			idx = text.Find('\t');
			if (idx == -1)
				break;
			code = text.Left(idx++);

			info = false;
			// process previous data
			if (rtmH->stat & stat_SCR)
			{
				bool	doing = true;

				if (scroll.Lookup(code, (CObject *&)obs))
				{
					if (obs->GetSize() > 0)
						doing = false;
				}

				if (doing)
				{
					if (m_addOnly)
					{
						for (int ii = 0; ii < quotes.GetSize(); )
						{
							if (!code.CompareNoCase(codes.GetAt(ii)))
							{
								rts  = (CdataSet *)quotes.GetAt(ii);
								if (rts->IsOn())
									DoRTM(code, 0, rts, NULL);
								delete rts;
								codes.RemoveAt(ii);
								quotes.RemoveAt(ii);
								continue;
							}
							ii++;
						}
					}
					else if (quote.Lookup(code, (CObject *&)rts))
					{
						if (rts->IsOn())
							DoRTM(code, 0, rts, NULL);
						rts->SetOn(false);
					}
				}
			}
			else
			{
				if (scroll.Lookup(code, (CObject *&)obs) && obs->GetSize() > 0)
				{
					if (!quote.Lookup(code, (CObject *&)rts))
						rts = NULL;
					DoRTM(code, stat_SCR, rts, obs);
					
					rts->SetOn(false);
					for (int ii = 0; ii < obs->GetSize(); ii++)
						delete obs->GetAt(ii);
					obs->RemoveAll();
				}
			}

			if (rtmH->stat & stat_SCR)
			{
				int	index;
				CdataSet* fms;
				char	*ptr = NULL;
				char	*token, *datb = (char *)(pBytes + idx);

				if (!scroll.Lookup(code, (CObject *&)obs))
				{
					obs = new CObArray;
					obs->RemoveAll();
				}
				if (!quote.Lookup(code, (CObject *&)fms))
					fms = new CdataSet;

				rts = new CdataSet;
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
					token = strtokx(NULL, '\t', &ptr);
				}

				if (rts->IsOn())
				{
					if (obs->GetSize() > 0)
						obs->InsertAt(0, rts);
					else
						obs->Add(rts);
				}
				else	delete rts;

				quote.SetAt(code, fms);
				scroll.SetAt(code, obs);
			}
			else
			{
				char	*ptr = NULL;
				char	*token, *datb = (char *)(pBytes + idx);

				if (m_addOnly)
				{
					codes.Add(code);
					rts = new CdataSet;
					quotes.Add(rts);
				}
				else
				{
					if (!quote.Lookup(code, (CObject *&)rts))
						rts = new CdataSet;
					quote.SetAt(code, rts);
				}
				token = strtokx(datb, '\t', &ptr);
				for (int ii = 0; token && ii < rtmk->GetSize(); ii++)
				{
					if (!rts->IsOn())
						rts->SetOn();
					rts->SetAt(atoi(rtmk->GetAt(ii)), token);
					token = strtokx(NULL, '\t', &ptr);
				}
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
		for (pos = scroll.GetStartPosition(); pos; )
		{
			scroll.GetNextAssoc(pos, code, (CObject *&)obs);
			if (obs->GetSize() > 0)
			{
				if (!quote.Lookup(code, (CObject *&)rts))
					rts = NULL;
				DoRTM(code, stat_SCR, rts, obs);
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

		if (m_addOnly)
		{
			for (int ii = 0; ii < quotes.GetSize(); ii++)
			{
				code = codes.GetAt(ii);
				rts  = (CdataSet *)quotes.GetAt(ii);
				if (rts->IsOn())
					DoRTM(code, 0, rts, NULL);
				delete rts;
			}
			codes.RemoveAll();
			quotes.RemoveAll();
		}
		else
		{
			for (pos = quote.GetStartPosition(); pos; )
			{
				quote.GetNextAssoc(pos, code, (CObject *&)rts);
				if (rts->IsOn())
					DoRTM(code, 0, rts, NULL);
				delete rts;
			}
			quote.RemoveAll();
		}
	}
}

void CGuard::OnAlert(CString code, char* pBytes, int nBytes, int stat)
{
	int	key;
	POSITION pos;
	CWorks* works;
	CdataSet fms;
	CObArray obs;

	SetString(pBytes, NULL, NULL, &fms);
	if (!fms.IsOn())
		return;

	m_sync.Lock();
	m_dde->OnAlert(code, &fms);
	obs.Add(&fms);
	for (pos = m_clients.GetStartPosition(); pos; )
	{
		m_clients.GetNextAssoc(pos, key, works);
		if (works->m_status & S_LOAD)
		{
			if (works->isWorks())
				((CClient *)works)->OnAlert(code, NULL, &obs, stat);
		//	else
		//		works->OnAlert(code, text, stat);
		}
	}
	obs.RemoveAll();
	m_sync.Unlock();
}

void CGuard::OnNotice(char* pBytes, int nBytes, bool recur)
{
	int	key;
	CString	tmps, string, text;
	CdataSet major, minor, fms;
	char*	datb;

	text = CString(pBytes, nBytes);
	tmps = text;
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
			if (works->m_key == winK_RMN)
				continue;

			if (works->isWorks())
				((CClient *)works)->OnNotice(major, minor, fms, tmps);
			else
				works->OnNotice(tmps);
		}
	}

	if (recur && GetClient(winK_RMN, works))
	{
		text = works->OnNotice(tmps);
		if (!text.IsEmpty())
			OnNotice((char *)text.operator LPCTSTR(), text.GetLength(), false);
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
	char	tmpb[L_MAPN+1];
	CString	path;

	if (strcmp(&mapN[L_SGID+L_SELC], "00"))
		return false;
	path.Format("%s\\%s\\axis.ini", m_root, TABDIR);
	GetPrivateProfileString(_T("Select"), CString(&mapN[L_SGID], L_SELC), "", tmpb, sizeof(tmpb), path);
	if (strlen(tmpb) != L_MAPN-L_SGID)
		return false;
	CopyMemory(&mapN[L_SGID], tmpb, L_MAPN-L_SGID);
	return true;
}

void CGuard::SetVersion(char *mapN)
{
	if (!memcmp((char *)m_mapN.operator LPCTSTR(), mapN, L_MAPN))
		m_mapN = _T("");
}

void CGuard::UpdateVersion(char *mapN, int ver)
{
	CString	maps, vers;

	maps = CString(mapN, L_MAPN);
	vers.Format("%d", ver);
	m_vers.SetAt(maps, vers);
}

void CGuard::UpdateVers(char *verB, int verL)
{
	CString	maps, vers;
	struct _verM *verM = (struct _verM *) verB;

	for (int ii = 0; ii < verL; ii++)
	{
		maps = CString(verM->name);
		vers.Format("%d", verM->ver);
		m_vers.SetAt(maps, vers);
		verM++;
	}
}

bool CGuard::isVersion(CString mapN)
{
	struct	_mapH	mapH;

	if (!(m_term & flagVER) || !mapN.Mid(L_SGID+L_SELC).CollateNoCase(_AXUS_))
		return GetMapH(mapN) ? true : false;

	mapN.MakeUpper();
	if (m_verpass.Find(mapN.Left(L_TGID)) != -1 || m_verpass.Find(mapN.Left(L_SGID)) != -1)
		return true;

	int	ver = 0;
	CString	tmps;
	if (m_vers.Lookup(mapN, tmps))
		ver = atoi(tmps);
	else	return false;

	if (!ver || (GetMapH(mapN, (char *)&mapH) && mapH.vers == ver))
		return true;

	RequestMAPs(mapN);
	return true;
}

bool CGuard::isVersion(CString mapN, CString prefix)
{
	if (!(m_term & flagVER))
		return true;

	CString	fileN, vers;
	int	version = 0;

	if (m_vers.Lookup(mapN, vers))
	{
		version = atoi(vers);
		if (!version)
			return true;
	}
	else	return false;

	fileN.Format("%s\\%s\\%s%s", m_root, DEVDIR, mapN, prefix);
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

	RequestMAPs(mapN + prefix);
	return true;
}

bool CGuard::isLevel(CString mapN, CString prefix)
{
	return true;	// for test

	//
	CString	fileN;

	fileN.Format("%s\\%s\\%s%s", m_root, DEVDIR, mapN, prefix);

	DWORD 	size, value;
	size = GetFileVersionInfoSize((char *)fileN.operator LPCTSTR(), &value);
	if (size <= 0)
		return true;

	char 	*vers;
	VS_FIXEDFILEINFO *vs;

	vers = new char[size];
	if (GetFileVersionInfo ((char *)fileN.operator LPCTSTR(), value, size, (void *)vers))
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

void CGuard::RequestMAPs(CString mapN)
{
	CString	path;
	struct	_fileH	fileH;

	m_section.Lock();
	SetGuide(AE_MUPDATE);
	m_mapN = mapN;
	path.Format("%s/%s/%s/%s", MAPDIR, mapN.Left(L_SGID), mapN.Left(L_TGID), mapN);
	
	ZeroMemory(&fileH, sizeof(struct _fileH));
	fileH.fileK = fileK_MAP;
	CopyMemory(fileH.fileN, (char *)path.operator LPCTSTR(), path.GetLength());	
	if (Write(msgK_RSM, "", (char *)&fileH, sizeof(fileH)))
	{
		MSG	msg;
		DWORD elapseV = GetTickCount();

		while (!m_mapN.IsEmpty())
		{
			if (GetTickCount() - elapseV > 5000)
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
	CString	text;
	form->ReadData(text, false, column, row);
	text.TrimRight();

	if (keys.IsEmpty())
		keys = form->GetSymbolName();
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
		keys = form->GetSymbolName();

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

	int mapL = (int)fileH.GetLength();
	if (mapL < L_MAPH)
	{
		fileH.Close();
		return false;
	}

	if (mapH)
		fileH.Read(mapH, L_MAPH);
	fileH.Close();
	return true;
}

void CGuard::Sign(int signK, char* signB, int signL, CString& dns, bool shop)
{
	CString	user, text, string;
	struct _signR*	signR;

	signR = (struct _signR*) signB;
	if (!dns.IsEmpty())
		m_user.Empty();

	if (signR->absS)
		m_absS = signR->absS;

	int	idx;
	switch (signR->mask & maskSEC)
	{
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
			{
				tm	ltime;
				SYSTEMTIME st;
				errno_t	err;
				
				idx = atoi(string.Mid(idx+1));
				err = localtime_s(&ltime, (time_t*)&idx);
				if (err != 0)
					break;

				GetLocalTime(&st);
				st.wYear      = ltime.tm_year+1900;
				st.wMonth     = ltime.tm_mon+1;
				st.wDay       = ltime.tm_mday;
				st.wDayOfWeek = ltime.tm_wday;

				st.wHour   = ltime.tm_hour;
				st.wMinute = ltime.tm_min;
				st.wSecond = ltime.tm_sec;
				SetLocalTime(&st);
			}
			break;

		case 'u':
		case 'U':
			user = string.Mid(idx+1);
			break;

		case 'o':
		case 'O':
			m_otp = string.Mid(idx+1);
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
	user.Format("%s|%s", m_user, m_uname);
	m_app->WriteProfileString(WORKSTATION, SIGNONID, user);
	m_modals.Format("%s\\%s\\%s\\modal.ini", m_root, USRDIR, m_uname);

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
	int	idx;
	CString	entry, name;
	struct	_regH*	regH;

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
			for (ii = 0; ii < regH->regL; ii++)
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
				m_account.SetAt(name, entry);
				m_accno.Add(name);
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

void CGuard::RemoveRegistry()
{
	m_combo.RemoveAll();
	m_account.RemoveAll();
	m_accno.RemoveAll();
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
		m_account.SetAt(keys, entry.Mid(++idx));
	}
}

void CGuard::GetAccounts(CString& account)
{
	CString	key, text;

	account.Empty();
	if (m_term & flagACN)
		return;

	for (int ii = 0; ii < m_accno.GetSize(); ii++)
	{
		key = m_accno.GetAt(ii);
		if (m_account.Lookup(key, text))
		{
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
	if (m_term & flagACN)
		return;

	for (int ii = 0; ii < m_accno.GetSize(); ii++)
	{
		key = m_accno.GetAt(ii);

		account += key;
		account += '\t';
		if (full)
		{
			if (m_account.Lookup(key, name))
				account += name;
		}
		else if (GetAcName(key, name))
			account += name;

		account += '\n';
	}
}

bool CGuard::GetAcPass(CString key, CString& pass)
{
	if (!m_account.Lookup(key, pass))
		return false;

	int idx = pass.Find('\t');
	if (idx != -1)
		pass = pass.Left(idx);
	return true;
}

bool CGuard::GetAcName(CString key, CString& name)
{
	if (!m_account.Lookup(key, name))
		return false;

	int idx = name.Find('\t');
	if (idx != -1)
	{
		name = name.Mid(++idx);
		idx = name.Find('\t');
		if (idx != -1)
		{
			CString	text;

			text = name.Mid(idx+1);
			name = name.Left(idx);
			idx  = text.Find('\t');
			if (idx != -1)
			{
				text = text.Left(idx);
				if (!text.IsEmpty())
					name = text;
			}
		}
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
	if (sndL == 0)	return;

	CString	tmps;
	tmps.Format("%05d", sndL);
	CopyMemory(axisH->datL, tmps, tmps.GetLength());

	Write(sndB, L_axisH+sndL, false);

	if (m_cast)
	{
		for (int idx = 0; idx < client->m_magic; idx++)
			m_cast->RegisterRTM(client->m_key, idx, "");
	}
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

CString	CGuard::Secure(CString src, CString keys, int mode)
{
	if (m_secure == NULL)
	{
		char	wb[64];
		CString	string;

		string.Format("%s\\%s\\axis.ini", m_root, TABDIR);
		GetPrivateProfileString("Secure", "file", "", wb, sizeof(wb), string);
		string = wb;
		if (string.IsEmpty())
			return src;

		string.Format("%s\\%s", m_root, wb);
		m_secure = AfxLoadLibrary(string);
		if (m_secure == NULL)
			axEncrypt = NULL;
		else
			axEncrypt = (char* (APIENTRY*)(char*, char*, int))GetProcAddress(m_secure, _T("axEncrypt"));
	}

	if (axEncrypt)
	{
		int	len;
		
		len = src.GetLength();
		src.TrimRight();
		src = axEncrypt((char *)src.operator LPCTSTR(), (char *)keys.operator LPCTSTR(), mode);
		src += CString(' ', len - src.GetLength());
		return src;
	}

	return src;
}

bool CGuard::GetCode(int kind, CString& code, int type, CPoint pt, bool valid)
{
	if (axGetCode)
	{
		char	wb[128];
		
		if (valid)
			strcpy_s(wb, sizeof(wb), code);
		if ((*axGetCode)(kind, (char*)m_uname.operator LPCTSTR(), wb, type, pt))
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

		idx = m_master.Find('\t'+code+MASTERs);
		if (idx != -1)
		{
			name = m_master.Mid(idx);
			name.TrimLeft();
			idx = name.Find('\t');
			if (idx != -1)
				name = name.Left(idx);
			name = name.Mid(code.GetLength());
			name.TrimLeft();
			return true;
		}
	}

	return GetName(kind, code, name, type);
}

CString CGuard::PathMAP(CString mapN, bool force)
{
	CString pathN;
	if (mapN.Mid(L_SGID+L_SELC).CompareNoCase(_AXUS_))
	{
		if (force)
		{
			pathN.Format("%s\\%s\\%s", m_root, MAPDIR, mapN.Left(L_SGID));
			if (GetFileAttributes(pathN) == 0xffffffff)
				CreateDirectory(pathN, NULL);

			pathN += "\\";
			pathN += mapN.Left(L_TGID);
			if (GetFileAttributes(pathN) == 0xffffffff)
				CreateDirectory(pathN, NULL);
		}
		pathN.Format("%s\\%s\\%s\\%s\\%s", m_root, MAPDIR,
				mapN.Left(L_SGID), mapN.Left(L_TGID), mapN);
	}
	else
		pathN.Format("%s\\%s\\%s\\%s", m_root, USRDIR, m_uname, mapN);


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
		if (form->m_form->properties & PR_CODE && self)
		{
			form->ReadData(text);
			SetCode(form->GetSymbolName(), text);
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
				SetCode(form->GetSymbolName(), text, false);
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

bool CGuard::SetCode(CString name, CString text, bool self)
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
	return true;
}

void CGuard::RunHelp(char *helpN)
{
	if (strlen(helpN) == 0)
		return;

	CString	cmdN, curN;
	curN = cmdN = _T("");
	curN.Format("%s\\%s", m_root, HELPDIR);
	cmdN.Format(" %s\\%s", curN, helpN);

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

	char	pathN[256];
	GetWindowsDirectory(pathN, sizeof(pathN));
	strcat_s(pathN, sizeof(pathN), "\\hh.exe");

	CreateProcess(
		pathN,			// application name
		(char *)cmdN.operator LPCTSTR(),// command line
		NULL,			// process attribute
		NULL,			// thread attribute
		FALSE,			// is inherit handle
		0,			// creation flags
		NULL,			// environment
		(char *)curN.operator LPCTSTR(),// current directory
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
	CString	text, tmps;

	text.Empty();
	section = CString(screen->m_mapH->mapN, L_MAPN);
	WritePrivateProfileSection(section, "", m_repository);

	for (int ii = 0; ii < screen->m_mapH->formN; ii++)
	{
		form = screen->GetAtForm(ii);
		if (form->m_form->attr & FA_NOR)
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
			if (value == ET_USER)
				text += tmps;
			break;
		default:
			continue;
		}

		text.TrimRight();
		if (!text.IsEmpty())
		{
			WritePrivateProfileString(section, form->GetSymbolName(), text, m_repository);
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

	char	twb[256];
	int	idx, saveN = 0;

	section = CString(screen->m_mapH->mapN, L_MAPN);

	for (int key = 0; key < screen->m_mapH->formN; key++)
	{
		form = screen->GetAtForm(key);
		if (form->m_form->attr & FA_NOR)
			continue;

		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			if (screen->m_client->GetAtScreen(objs, form->m_form->keys))
			{
				GetPrivateProfileString(section, form->GetSymbolName(), "", twb, sizeof(twb), m_repository);
				text = twb;
				text.TrimRight();
				if (text.GetLength() == L_MAPN)
					screen->SetObject(objs->m_object, (char *)text.operator LPCTSTR());
			}
			continue;
		case FM_TAB:
			if (form->m_form->attr & FA_DRAG)
			{
				GetPrivateProfileString(section, form->GetSymbolName(), "", twb, sizeof(twb), m_repository);
				text = twb;
				if (!text.IsEmpty())
					form->WriteAll(text);
				continue;
			}
			form->SetCurSel(GetPrivateProfileInt(section, form->GetSymbolName(), 0, m_repository));
			continue;
		case FM_SHEET:
		case FM_UTAB:
			GetPrivateProfileString(section, form->GetSymbolName(), "", twb, sizeof(twb), m_repository);
			text = twb;
			idx  = text.Find('|');
			if (idx == -1)
				continue;
			tmps = text.Mid(idx+1);
			tmps.TrimRight();
			if (!tmps.IsEmpty())
				form->SetEnum(tmps, 0);
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
	char*	sendB;
	struct	_axisH*	axisH;
	struct	_userTH* userth;
	CWorks*	works = NULL;

	sendB = new char[L_axisH+nBytes+256];
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

	xTRACE(x_SNDs, sendB, L_axisH+nBytes);

	if (!(m_term & flagCAX) && userth->stat & US_CA)
	{
		if (!Certify(&sendB[L_axisH], nBytes, works ? works->m_mapN : ""))
		{
			delete [] sendB;
			return FALSE;
		}
		axisH->auxs |= auxsCA;
	}

	if (!(m_term & flagENX) && userth->stat & US_ENC)
	{
		if (!Xecure(DI_ENC, &sendB[L_axisH], nBytes))
		{
			SetGuide(AE_CSECURE, key);
			delete [] sendB;
			return FALSE;
		}
		axisH->stat |= statENC;
	}

	CString	text;
	text.Format("%05d", nBytes);
	CopyMemory(axisH->datL, (char *)text.operator LPCTSTR(), sizeof(axisH->datL));

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

BOOL CGuard::Write(int msgK, CString trxC, char* datB, int datL, int key)
{
	char*	sendB;
	struct	_axisH*	axisH;
	CWorks*	works;
	CString	text;

	sendB = new char[L_axisH+datL];
	axisH = (struct _axisH *) sendB;
	ZeroMemory(axisH, L_axisH);

	axisH->msgK = msgK;
	axisH->winK = key;
	if (GetClient(key, works) && !works->m_mapN.IsEmpty())
		CopyMemory(axisH->svcN, works->m_mapN.Mid(L_SGID, L_SELC), sizeof(axisH->svcN));
	CopyMemory(axisH->trxC, trxC, trxC.GetLength());
	text.Format("%05d", datL);
	CopyMemory(axisH->datL, (char *)text.operator LPCSTR(), sizeof(axisH->datL));
	CopyMemory(&sendB[L_axisH], datB, datL);

	if (Write(sendB, L_axisH+datL, false))
	{
		delete [] sendB;
		return TRUE;
	}

	delete [] sendB;
	return FALSE;
}

BOOL CGuard::Login(char* datB, int datL, bool xecure)
{
	CString	text;
	char*	sendB;
	struct	_axisH*	axisH;

	sendB = new char[L_axisH+datL+256];
	axisH = (struct _axisH *) sendB;
	ZeroMemory(axisH, L_axisH);

	text = _T("AXLOGON");
	axisH->msgK = msgK_SIGN;
	CopyMemory(axisH->trxC, text, text.GetLength());
	CopyMemory(&sendB[L_axisH], datB, datL);
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

	sendB = new char[L_axisH+datL+256];
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

	xTRACE(x_SNDs, sendB, L_axisH+datL);

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
	if (key != -1)
	{
		axisH->winK = LOBYTE(key);
		axisH->unit = HIBYTE(key);
	}
	else
	{
		axisH->stat = statNEW;
		axisH->trxK = screen->m_client->m_key;
		axisH->trxS = screen->m_key;
		CopyMemory(axisH->trxC, map, map.GetLength());
	}

	CopyMemory(axisH->svcN, CString(screen->m_mapH->mapN, L_MAPN).Mid(L_SGID, L_SELC), sizeof(axisH->svcN));
	text.Format("%05d", datL);
	CopyMemory(axisH->datL, (char *)text.operator LPCSTR(), sizeof(axisH->datL));
	CopyMemory(&sendB[L_axisH], datB, datL);

	xTRACE(x_SNDs, datB, datL);

	BOOL	retv;
	m_sock->InvokeHelper(DI_DWRITE, DISPATCH_METHOD, VT_BOOL, (void*)&retv,
				(BYTE *)(VTS_BSTR VTS_I4 VTS_I4 VTS_BOOL), ip, sendB, L_axisH+datL, FALSE);
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

	if (!file.Open(fileN, CFile::modeRead|CFile::typeBinary))
	{
		delete [] sendB;
		return 0;
	}

	int	rc, size;
	CString	text;
	struct	_fileH*	fileH;

	size = (int)file.GetLength();
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
	rc = file.Read(&sendB[L_axisH+datL], maxIOs-L_axisH-datL);
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
	
	text.Format("%05d", rc);
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
		if (!Xecure(DI_ENC, &sendB[L_axisH], datL))
		{
			SetGuide(AE_CSECURE, axisH->winK);
			delete [] sendB;
			return 0;
		}
		axisH->stat |= statENC;
	}

	text.Format("%05d", datL);
	CopyMemory(axisH->datL, (char *)text.operator LPCTSTR(), sizeof(axisH->datL));

	if (Write(sendB, L_axisH+datL, true))
	{
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
		if (!Xecure(DI_ENC, &sendB[L_axisH], datL))
		{
			SetGuide(AE_CSECURE, axisH->winK);
			delete [] sendB;
			return FALSE;
		}
		axisH->stat |= statENC;
	}

	text.Format("%05d", datL);
	CopyMemory(axisH->datL, (char *)text.operator LPCTSTR(), sizeof(axisH->datL));

	if (Write(sendB, L_axisH+datL, true))
	{
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
	char*	sendB;
	struct	_axisH*	axisH;
	struct	_userTH* userth;
	CWorks*	works;
	CScreen* screen;
	int	index, keyx;
	INT_PTR	pos;
	CString	string;

	char*	p = strchr(pBytes, '\t');
	if (p == NULL)
		return FALSE;

	pos = p - pBytes;
	if (pos == 0)
	{
		p = strchr(pBytes+1, '\t');
		if (p == NULL)
			return FALSE;
		else
			pos = p - pBytes;
	}
	string = CString(pBytes, (int)pos);
	pos++;
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

	keyx = ((CClient *)works)->GetKey(userth->key, (int)pos, index);
	if (keyx < 0)
		return FALSE;

	sendB  = new char[L_axisH+nBytes+256];
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

	xTRACE(x_SNDs, sendB, L_axisH+nBytes);

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
		if (!Xecure(DI_ENC, &sendB[L_axisH], nBytes))
		{
			SetGuide(AE_CSECURE, key);
			delete [] sendB;
			return FALSE;
		}
		axisH->stat |= statENC;
	}

	CString	text;
	text.Format("%05d", nBytes);
	CopyMemory(axisH->datL, (char *)text.operator LPCTSTR(), sizeof(axisH->datL));

	if (Write(sendB, L_axisH+nBytes, true))
	{
		if (key && !(userth->stat & US_PASS))
		{
		//	screen->m_state |= waitSN;
			PostAxis(MAKEWPARAM(waitPAN, key), true);
		}
		delete [] sendB;
		return TRUE;
	}

	delete [] sendB;
	return FALSE;
}

BOOL CGuard::Write(char* pBytes, int nBytes, bool trace)
{
	if (trace)
		xTRACE(x_SNDs, pBytes, nBytes);

	BOOL	retv;
	m_sock->InvokeHelper(DI_WRITE, DISPATCH_METHOD, VT_BOOL, (void*)&retv,
						(BYTE *)(VTS_I4 VTS_I4), pBytes, nBytes);
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

LRESULT CGuard::SendAxis(int msg, int key, LPARAM lParam)
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

	wsprintf(tmb, "%s\\%s\\%s", m_root, MTBLDIR, SECMSG);
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
		{0}
	};

	char	wb[256];
	int	cx, cy, pos;
	CString	text;

	CopyMemory(wb, (char *)maps.operator LPCSTR(), maps.GetLength());
	wb[maps.GetLength()] = '\0';
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
			if (cx && cy && isLevel(maps, controls[ii].prefix))	// && isVersion(maps, controls[ii].prefix))
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
	default:
		break;
	}
	return vtypeNRM;
}

bool CGuard::GetVersion(CString fileN, CString& vers)
{
	DWORD 	tw, DWval;
	DWval = GetFileVersionInfoSize((char *)fileN.operator LPCTSTR(), &tw);
	if (DWval <= 0)	return false;

	int	ii, jj;
	char 	*VerInfo, *Vpointer;
	VerInfo = new char[DWval +1];
	ZeroMemory(VerInfo, DWval+1);

	if (!GetFileVersionInfo ((char *)fileN.operator LPCTSTR(), tw, DWval+1, (LPVOID)VerInfo))
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
		for (ii = (int)strlen("FileVersion"), jj = 0; ii < static_cast<int>(DWval); ii++)
		{
			if (Vpointer[ii] == '.' || isdigit(Vpointer[ii])) 
				VerInfo[jj++] = Vpointer[ii];
			else	break;
		}
	}
	else if (Vpointer = strstr(VerInfo, "ProductVersion")) 
	{
		for (ii = (int)strlen("ProductVersion"), jj = 0; ii < static_cast<int>(DWval); ii++)
		{
			if (Vpointer[ii] == '.' || isdigit(Vpointer[ii]))
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
				tmps += CString(' ', m_CODEs-tmps.GetLength());
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
		case 's':
			search = true;
			break;
		case 'K':
			force = true;
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
			CRect	rect = form->GetRect();
			works->m_view->ClientToScreen(&rect);
			userwh.pos.x = rect.left;
			userwh.pos.y = rect.bottom;
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
			return (int)SendAxis(MAKEWPARAM(renVIEW, works->m_key), (LPARAM)maps.operator LPCTSTR());
		}
		return AtView(works, maps, domino, force);
	}

	if (search)
	{
		bool	isvirtual, isfloating;
		char	mapN[L_MAPN+1];

		CopyMemory(mapN, maps, L_MAPN);
		mapN[L_MAPN] = '\0';
		isvirtual = GetVirtualTR(mapN);

		isfloating = works->isFloating();
		for (POSITION posx = m_clients.GetStartPosition(); posx; )
		{
			m_clients.GetNextAssoc(posx, idx, works);
			if (works->m_status & S_WORKS)
				continue;
			if (m_vid != works->m_vid || (userwh.group && userwh.group != works->m_group))
				continue;
			if (isfloating != works->isFloating())
				continue;
			if (works->m_mapN.CompareNoCase(maps) == 0)
				return AtView(works, maps, domino, force);

			if (isvirtual)
			{
				char	mapb[L_MAPN+1];

				CopyMemory(mapb, works->m_mapN, L_MAPN);
				mapb[L_MAPN] = '\0';
				if (GetVirtualTR(mapb) && CString(mapN).CompareNoCase(mapb) == 0)
				{
					maps += domino;
					return (int)SendAxis(MAKEWPARAM(renVIEW, works->m_key), (LPARAM)maps.operator LPCTSTR());
				}
			}
		}
	}

	if (type != typeVIEW && !userwh.key)
		userwh.key = WK_POPUP;

	text = CString((char *)&userwh, L_userWH) + domino;
	return (int)SendAxis(MAKEWPARAM(newVIEW, key), (LPARAM)text.operator LPCTSTR());
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

	text.Format("#%s", form->GetSymbolName());
	GetPrivateProfileString("GRID Controls", text, "", twb, sizeof(twb), m_vtcode);
	tmps = twb; tmps += ' ';

	text.Format("$%s", form->GetSymbolName());
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
			if (!text.CompareNoCase(form->GetSymbolName(cell[idx].name)))
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
		profit->m_name = form->GetSymbolName();

		profit->m_fees    = NULL;
		profit->m_taxs    = NULL;
		profit->m_moneys  = NULL;
		profit->m_charges = NULL;

		profit->m_fee    = 0;
		profit->m_tax    = 0;
		profit->m_money  = 0;
		profit->m_charge = false;

		text.Format("?%s", form->GetSymbolName());
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
		syms.Format("%s$%s", maps, text);
		GetPrivateProfileString("GRID", syms, "", wb, sizeof(wb), m_modals);
		text = wb;
		text.TrimRight();
		return (bool)!text.IsEmpty();
	}

	syms.Format("%s$%s", maps, text.Left(pos));
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

void CGuard::SetPalette(bool force)
{
	if (m_palette)
	{
		CString	tmps;
		CWorks* works;
		int	key;

		tmps.Format("%s\\%s\\%s", m_root, TABDIR, PALETTE);
		m_palette->SetPalette((char *)tmps.operator LPCTSTR(), force);
		for (POSITION pos = m_clients.GetStartPosition(); pos; )
		{
			m_clients.GetNextAssoc(pos, key, works);
			if (works->m_status & S_LOAD)
				works->SetPalette(force);
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
		if (GetClient(key, works) && works->m_status & S_LOAD)
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

BOOL CGuard::Certify(BOOL force, BOOL certify, BOOL xcertify)
{
	return TRUE;
	/*
	if (certify)
	{
		if (!m_certify)
		{
			m_certify = new CWnd();
			if (!m_certify->CreateControl("AxisCertify.CertifyCtrl.1", NULL, 0, CRect(0, 0, 0, 0), m_parent, 0))
			{
				delete m_certify;
				m_certify = NULL;
				if (!force)
					SetGuide(AE_ESECURE);
				return TRUE;
			}
		}

		if (certify && xcertify)
		{
			BOOL	retv;
			m_certify->InvokeHelper(DI_CAEX, DISPATCH_METHOD, VT_BOOL, (void*)&retv, NULL);
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
	*/
}

int CGuard::OnCertify(char* pBytes, int nBytes)
{
	return 0;
	/*
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
	*/
}

BOOL CGuard::CertifyErr(char* pBytes, int nBytes)
{
	return TRUE;
	/*
	if (!m_certify)
		return FALSE;

	BOOL	retv;
	m_certify->InvokeHelper(DI_CAERR, DISPATCH_METHOD, VT_BOOL, (void*)&retv,
						(BYTE *)(VTS_I4 VTS_I4), pBytes, nBytes);
	return retv;
	*/
}

BOOL CGuard::Certify(char* pBytes, int& nBytes, CString maps)
{
	return TRUE;
	/*
	if (!m_certify || !(m_status & WS_SELF))
	{
		SetGuide(AE_ECERTIFY);
		CertifyErr("", -1);
		return FALSE;
	}

	BOOL	retv;
	if (m_certifyVer == caVER2)
	{
		m_certify->InvokeHelper(DI_CAX, DISPATCH_METHOD, VT_BOOL, (void*)&retv,
						(BYTE *)(VTS_I4 VTS_I4 VTS_I4), pBytes, &nBytes, (char *)maps.operator LPCTSTR());
	}
	else
		m_certify->InvokeHelper(DI_CA, DISPATCH_METHOD, VT_BOOL, (void*)&retv, (BYTE *)(VTS_I4 VTS_I4), pBytes, &nBytes);

	return retv;
	*/
}

void CGuard::CertifyId(char* pBytes, bool retry)
{
	/*
	Certify(TRUE, TRUE);
	if (m_certify)
		m_certify->InvokeHelper(DI_CAID, DISPATCH_METHOD, VT_EMPTY, NULL, (BYTE *)(VTS_I4), pBytes);
	if (retry)
		Certify(FALSE, m_term & flagCA);
	*/
}

void CGuard::GetCetifyInfo(CString& infos)
{
	infos = _T("");
	/*
	bool	emergency = false;

	if (m_term & flagCA && m_term & flagCAX)
		emergency = true;
	infos.Format("%c\t%c", emergency ? 'Y' : 'N', m_certify ? 'Y' : 'N'); 
	*/
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

	int	vRows, nCols;
	CString	dests, string;

	nCols = format.GetLength();
	text.Remove('\r');
	vRows = text.Replace('\n', '\r');
	if (vRows == 0)
		return;

	if (!fileN.IsEmpty() && !userdefine)
	{
		dests.Format("%s\\%s\\%s.xls", m_root, USRDIR, fileN);
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
		SetGuide(AE_STARTXLS);
		return;
	}

	bool	except = false;
	COleVariant FilenameSave(dests), Fileformat((short)33), covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
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
		for (int row = 0; row < vRows; )
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

			for (int col = 0; col < nCols; col++)
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

void CGuard::MakeXlsFile(CString text, bool visible, CString fileN, bool userdefine)
{
	CString	dests, tmps, string;

	if (!fileN.IsEmpty() && !userdefine)
	{
		dests.Format("%s\\%s\\%s.xls", m_root, USRDIR, fileN);
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
	tmps.Format("%s.txt", dests.Left(idx));

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
	COleVariant FilenameSave(dests), Fileformat((short)33), covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
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
	int	offs;
	CString	dests, tmps, string;

	if (!fileN.IsEmpty())
	{
		offs = fileN.ReverseFind('.');
		if (offs == -1)
		{
			dests.Format("%s.xls", fileN);
			tmps.Format("%s.txt", fileN);
		}
		else
		{
			dests.Format("%s.xls", fileN.Left(offs));
			tmps = fileN;
		}
	}
	else
	{
		CFileDialog fDlg(FALSE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST,
				"WorkSheet Files(*.txt)|*.txt|All Files(*.*)|*.*|");
		if (fDlg.DoModal() != IDOK)
			return FALSE;

		tmps   = fDlg.GetPathName();
		string = fDlg.GetFileExt();

		offs = tmps.ReverseFind('.');
		if (offs == -1)
			string = "txt";
		else
		{
			string = tmps.Mid(offs+1);
			tmps   = tmps.Left(offs);
			if (string.IsEmpty())
				string = "txt";
		}

		dests.Format("%s.xls", tmps);
		tmps += ".";
		tmps += string;
	}
	AfxGetApp()->DoWaitCursor(TRUE);

	_Application	excel;
	if (!excel.CreateDispatch("Excel.Application"))
	{
		AfxGetApp()->DoWaitCursor(FALSE);
		SetGuide(AE_ECONVERT);
		return FALSE;
	}

	bool	except = false;
	COleVariant FilenameSave(dests), Fileformat((short)33), covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
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
		::MessageBox(NULL, e->m_strDescription, e->m_strSource, MB_OK|MB_ICONERROR|MB_SYSTEMMODAL);
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
	int	offs;
	CString	dests, tmps, string;

	if (!fileN.IsEmpty())
	{
		offs = fileN.ReverseFind('.');
		if (offs == -1)
		{
			dests.Format("%s.txt", fileN);
			tmps.Format("%s.xls", fileN);
		}
		else
		{
			dests.Format("%s.txt", fileN.Left(offs));
			tmps = fileN;
		}
	}
	else
	{
		CFileDialog fDlg(FALSE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST,
				"WorkSheet Files(*.xls)|*.xls|All Files(*.*)|*.*|");
		if (fDlg.DoModal() != IDOK)
			return FALSE;

		tmps   = fDlg.GetPathName();
		string = fDlg.GetFileExt();

		offs = tmps.ReverseFind('.');
		if (offs == -1)
			string = "xls";
		else
		{
			string = tmps.Mid(offs+1);
			tmps   = tmps.Left(offs);
			if (string.IsEmpty())
				string = "xls";
		}

		dests.Format("%s.txt", tmps);
		tmps += ".";
		tmps += string;
	}
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
		::MessageBox(NULL, e->m_strDescription, e->m_strSource, MB_OK|MB_ICONERROR|MB_SYSTEMMODAL);
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

bool CGuard::Modal(int kind, CString keys, CString& currents, CString defs)
{
	if (axDialog)
	{
		char*	ptr = (char *)axDialog(kind, (char *)keys.operator LPCTSTR(), (char *)currents.operator LPCTSTR(),
							(char *)defs.operator LPCTSTR());
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

BOOL CGuard::GetCombo(CString key, CString& combo, bool parse)
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
	struct	_fileH*	fileH;

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
			SetVersion(axisH->trxC);
		break;
	case fileK_VER:
		datL /= sizeof(struct _verM);
		if (datB <= 0)
			break;
		UpdateVers(datB, datL);
	default:
		break;
	}
}

BOOL CGuard::LoadMenu(CString menus)
{
	BOOL	rc;
	CFileFind ff;
	CString	string;

	string.Format("%s\\%s\\*.menu", m_root, TABDIR);
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

		string.Format("%s\\%s\\%s", m_root, RUNDIR, MENUDAT);
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
		pMenu = &wb[sizeof(struct _menuR)*64];
		for (idx = 0; idx < 64; idx++, menuR++)
		{
			if (menuR->name[0] == '\0')
				break;

			string = CString(menuR->name);
			string.MakeLower();
			if (!string.Compare(menus))
			{
				string.Format("%s\\%s\\%s", m_root, TABDIR, AXISMENU);
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

	path.Format("%s\\%s\\%s", m_root, RUNDIR, MENUDAT);
	if (CFile::GetStatus(path, status))
	{
		if ((status.m_attribute & (CFile::readOnly|CFile::hidden)) != CFile::hidden)
		{
			status.m_attribute |= CFile::hidden;
			status.m_attribute &= ~CFile::readOnly;
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
		int	size;

		size = (int)file.GetLength();
		if (size > sizeof(struct _menuH))
		{
			file.Seek(size-sizeof(struct _menuH), CFile::begin);
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

void CGuard::AddRegistry(char* datB, int datL)
{
	if (datL <= 0)
		return;

	CString	string;

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

	path.Format("%s/%s", m_root, fileH->fileN);
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

	file.Write(&datB[sizeof(struct _fileH)], value);
	file.Close();

	switch (fileH->fileK)
	{
	case fileK_XLS:
		break;
	case fileK_MAP:
		if (fileH->fileF == fileF_FIR || fileH->fileF == fileF_ONLY)
		{
			struct _mapH*	mapH;
			mapH = (struct _mapH *)datB;
			UpdateVersion((char *)mapH->mapN, mapH->vers);
		}
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
		Fileformat((short)33),
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
			size = (int)strlen(trace);
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
			CopyMemory(pp, (char *)text.operator LPCSTR(), size);
			pp[size] = '\0';
			GlobalUnlock(hg);
			SetClipboardData(CF_TEXT, hg);
		}
		CloseClipboard();
	}
}

bool CGuard::GetClipboard(CString& text)
{
	text = _T("");

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

	path.Format("%s\\%s\\axstring.dat", m_root, MTBLDIR);
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

	for ( ; true; )
	{
		if (fileN.IsEmpty() || force)
		{
			CFileDialog fDlg(FALSE, ext, fileN, OFN_HIDEREADONLY, filter);
			if (fDlg.DoModal() != IDOK)
				return _T("");

			fileN = fDlg.GetPathName();
			ext   = fDlg.GetFileExt();
		}

		int	offs = fileN.ReverseFind('.');
		if (offs == -1)
			ext = "xls";
		else
		{
			ext   = fileN.Mid(offs+1);
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
				string.Format("이 위치에  \'%s\' 파일이 있습니다. 기존의 파일을(를) 바꾸시겠습니까?", fileN);

			int	rc = ::MessageBox(NULL, string, "Excel", MB_YESNOCANCEL|MB_ICONINFORMATION);
			switch (rc)
			{
			case IDYES:
				if (!DeleteFile(fileN))
				{
					if (GetString(AS_USING, tmps))
						string.Format(tmps, fileN);
					else
						string.Format("\'%s\' 파일이 사용중입니다. 사용중인 파일을 닫고 다시 시도해 주십시오.", fileN);
					::MessageBox(NULL, string, "Excel", MB_OK|MB_ICONINFORMATION);
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
	char	wb[32];

	SendAxis(MAKEWPARAM(userPASS, 0), (LPARAM)wb);
	return CString(wb, (int)strlen(wb));
}

bool CGuard::OpenCast(CString listP)
{
	if (!m_cast)
		m_cast = new CCast(this, m_parent, (char *)m_root.operator LPCTSTR());

	if (m_cast->InitialCast())
		return false;

	return true;
}

void CGuard::CloseCast(int key)
{
	if (m_cast)
	{
		m_cast->ExitCast();
	}
}

void CGuard::FilterRTM(CString filter)
{
	int	idx;
	CString	entry;

	m_fsection.Lock();
	m_filter.RemoveAll();
	idx = filter.Find('\n');
	if (idx != -1)
		filter = filter.Left(idx);
	while (!filter.IsEmpty())
	{
		idx = filter.Find('\t');
		if (idx == -1)
		{
			entry  = filter;
			filter = _T("");
		}
		else
		{
			entry  = filter.Left(idx++);
			filter = filter.Mid(idx);
		}

		if (entry.GetLength() > 0)
			m_filter.SetAt(entry.GetAt(0), idx);
	}
	m_fsection.Unlock();
}

void CGuard::SetGridOption(int key, CString options)
{
	CWorks*	works;

	if (key != -1)
	{
		if (GetClient(key, works) && (works->m_status & S_LOAD) && !works->isWorks())
			((CDll *)works)->OnTrigger(options);
		return;
	}

	for (POSITION pos = m_clients.GetStartPosition(); pos; )
	{
		m_clients.GetNextAssoc(pos, key, works);
		if ((works->m_status & S_LOAD) && !works->isWorks())
			((CDll *)works)->OnTrigger(options);
	}
}

void CGuard::LogMsg(DWORD tick, CString mapN)
{
	static int count = 0;

	if (count >= 100) return;

	FILE* debug;
	errno_t	err;
	SYSTEMTIME stime;

	err = fopen_s(&debug, "c:\\debugX1.txt", "a+");
	if (err != 0)
		return;

	GetLocalTime(&stime);
	fprintf(debug, "[%02d:%02d:%02d.%03d][%02d] = tick[%ld], map[%s]\n",
			stime.wHour, stime.wMinute, stime.wSecond, stime.wMilliseconds, count++, tick, mapN);
	fflush(debug);
	fclose(debug);
}

void CGuard::DoRTM(CString code, int stat, CdataSet* fms, CObArray* obs, bool from_cast)
{
	int	key;
	POSITION pos;
	CWorks* works;
//	CString	data;

	if (fms == NULL || !fms->IsOn())
		return;

	if (stat & stat_SCR)
	{
		if (obs == NULL || (obs && (obs->GetSize() <= 0 || (obs->GetSize() > 0 && !((CdataSet *)obs->GetAt(0))->IsOn()))))
	//	if (!obs && (obs->GetSize() <= 0 || (obs->GetSize() > 0 && !((CdataSet *)obs->GetAt(0))->IsOn())))
			return;
	}

	m_sync.Lock();
	m_dde->OnAlert(code, fms);
	m_alertR.code = code;
	m_alertR.stat = stat;
	if (stat & stat_SCR)
	{
		int	idx, size = (int)obs->GetSize();

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
				m_alertR.ptr[ii] = ((CdataSet *)(obs->GetAt(idx+ii)))->GetData();
			if (idx == 0)
				break;
		}
	}
	else
	{
		m_alertR.size = 1;
		m_alertR.ptr[0] = fms->GetData();
	}

	for (pos = m_clients.GetStartPosition(); pos; )
	{
		m_clients.GetNextAssoc(pos, key, works);
		if (works->m_status & S_LOAD)
		{
			if (works->isWorks())
				((CClient *)works)->OnAlert(code, fms, obs, stat, &m_alertR);
			else
				((CDll *)works)->OnAlert((void *)&m_alertR);
		}
	}
	m_sync.Unlock();
}

CString CGuard::MakeString(CdataSet* fms)
{
	CString	text, string, data;

	text = _T("");
	for (int ii = 0; ii < maxSYM; ii++)
	{
		if (fms->Lookup(ii, data))
		{
			string.Format("%03d\t%s\t", ii, data);
			text += string;
		}
	}
	return text;
}

int CGuard::GetDataCount()
{
	long	value;

	m_sock->InvokeHelper(DI_ENUM, DISPATCH_METHOD, VT_I4, (void*)&value,
						(BYTE*)VTS_I4 VTS_I4 VTS_I4, NULL, NULL, enumQUEUE);
	return value;
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
