// Client.cpp: implementation of the CClient class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "imm.h"
#include "mmsystem.h"
#include "Wizard.h"
#include "Client.h"
#include "Script.h"
#include "Stream.h"
#include "Key.h"
#include "Mouse.h"
#include "History.h"
#include "xSystem.h"
#include "xScreen.h"
#include "../h/axiserr.h"
#include "../h/axisvar.h"
#include "../dll/form/fmctrl.h"
#include "../dll/form/fmgrid.h"
#include "../h/axstring.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClient::CClient(CGuard* guard, CWnd* view, int key, int type) : CWorks(guard, view, key, type)
{
	m_focus  = TRUE;
	m_magic  = 0;
	m_bitmap = NULL;

	m_keyx   = new CKey(this);
	m_mouse  = new CMouse(this);
	m_stream = new CStream(this);
	m_vm     = new CScript(guard);

	m_tips.RemoveAll();
	m_offset = CSize(0, 0);

	m_bool   = false;
	m_value  = 0;
	m_string = _T("");

	m_default.m_idx = -1;
	m_mapN.Empty();

	HANDLE instance = (HANDLE)GetWindowLong(m_view->GetSafeHwnd(), GWL_HINSTANCE);
	FARPROC	callproc = (FARPROC)MakeProcInstance(CallProc, instance);
	m_callproc = (FARPROC)SetWindowLong(m_view->GetSafeHwnd(), GWL_WNDPROC, (LONG)callproc);

	m_keys.RemoveAll();
	m_screen.RemoveAll();
	m_control.RemoveAll();
	m_units.RemoveAll();
	m_timers.RemoveAll();

	SetBK();
	m_hR = m_vR = 1.0;

//	m_OnTimer = NULL;
}

CClient::~CClient()
{
	m_guard->SetRepository(this);

	SetInitial(false);
	m_dominos.RemoveAll();

/*	if (m_OnTimer)
	{
		m_OnTimer->PostThreadMessage(WM_QUIT, NULL, NULL);
	//	HANDLE	hThread = m_OnTimer->m_hThread;
	//	if (WaitForSingleObject(hThread, 500) == WAIT_TIMEOUT)
	//		TerminateThread(hThread, 0);
	}
*/
	delete m_keyx;
	delete m_mouse;
	delete m_stream;
	delete m_vm;

	m_tips.RemoveAll();

	CString	keys;
	CpubControl* control;
	for (POSITION pos = m_control.GetStartPosition(); pos; )
	{
		m_control.GetNextAssoc(pos, keys, (void*&)control);
		delete control->m_target;
		AfxFreeLibrary(control->m_dll);
		delete control;
	}
	m_control.RemoveAll();
	m_units.RemoveAll();
}

void CClient::SetInitial(bool reset)
{
	History(false);
	StopAVI();

	if (m_status & (S_FLASH|S_BLINK))
	{
		m_status &= ~(S_FLASH|S_BLINK);
		if (reset)
			m_view->KillTimer(TM_RTM);
	}

	CScreen* screen;
	if (GetAtScreen(screen))
		m_guard->xAlert(this, screen);

	m_status = 0;
	if (reset)
	{
		m_status = S_ING;
		m_guard->SetGuide(AE_CLEAR, m_key);

		if (m_current.m_idx != -1)
		{
			if (GetScreen(screen))
				screen->SetForm(m_current.m_idx, false);
			m_current.m_idx = -1;
		}

		WaitDone(NULL, true);
	}

	m_sync.Lock();
	for (int ii = 0; ii < m_magic; ii++)
	{
		if (GetAtScreen(screen, ii))
			delete screen;
	}


	int	key, tid;
	for (POSITION pos = m_timers.GetStartPosition(); pos; )
	{
		m_timers.GetNextAssoc(pos, key, tid);
		m_view->KillTimer(key);
	}
	m_timers.RemoveAll();

//	if (m_OnTimer)
//		m_OnTimer->Cleanup(!reset);

	FreeBK(reset);

	m_magic  = 0;
	m_screen.RemoveAll();
	m_sync.Unlock();

	m_default.m_idx = -1;
	m_default.m_key = 0;
	m_current.m_idx = -1;
	m_current.m_key = 0;
	m_cbox.m_key    = 0;

	m_offset = CSize(0, 0);
	m_mapN.Empty();

	m_mouse->m_caret.m_key = -1;
}

bool CClient::Attach(CString maps, bool only, bool fix)
{
	if ((m_status & S_WAIT) && !only)
		return false;

	m_guard->SetCursor(0, m_view);
	m_sync.Lock();

	char	tmapN[L_MAPN+1]{};

	if (maps.GetLength() < L_MAPN)
	{
		m_sync.Unlock();
		return false;
	}

	CString domino = maps.Mid(L_MAPN);
	maps = maps.Left(L_MAPN);
	CopyMemory(tmapN, (char *)maps.GetString(), L_MAPN); tmapN[L_MAPN] = '\0';

	CScreen* screen{};
	bool	equal = false;
	bool	isvirtual = m_guard->GetVirtualTR(tmapN);

	m_hR = m_vR = 1.0;
	if (!(m_guard->m_term & flagACN))
	{
		struct	_mapH	mapH;

		if (!m_guard->GetMapH(tmapN, (char *)&mapH))
		{
			m_sync.Unlock();
			return false;
		}

		if (m_guard->m_term & flagXXX && (mapH.options & (OP_CERTIFY|OP_ENC)))
		{
			m_sync.Unlock();
			m_guard->SetGuide(AE_ESECURITY);
			return false;
		}
	}

	if (isvirtual && !m_mapN.IsEmpty())
	{
		char	twb[L_MAPN+1]{};
		CopyMemory(twb, m_mapN, L_MAPN); twb[L_MAPN] = '\0';
		if (m_guard->GetVirtualTR(twb) && !strcmp(twb, tmapN))
		{
			equal = true;
			m_view->GetClientRect(&m_rect);
		}
	}

	m_guard->SetRepository(this);
	m_dominos.RemoveAll();
	if (!equal)
	{
		if (domino.IsEmpty())
			m_guard->GetCode(m_dominos, this);

		if (!m_guard->isVersion(tmapN))
		{
			m_sync.Unlock();
			return false;
		}

		if (!m_guard->CheckSecurity(tmapN, (char *)m_mapN.operator LPCTSTR()))
		{
			m_sync.Unlock();
			return false;
		}

		if (m_offset.cx || m_offset.cy)
			((CScrollView *)m_view)->ScrollToPosition(CPoint(0, 0));

		SetInitial();

		if (fix)
			m_status |= S_FIX;
		if (!SetAtScreen(screen, m_rect, tmapN))
		{
			m_status &= ~S_ING;
			m_sync.Unlock();
			return false;
		}

		m_mouse->m_hover.m_key = -1;
		m_mouse->m_hover.m_idx = -1;

		m_mouse->m_move.m_key = -1;
		m_mouse->m_move.m_idx = -1;

		m_mapN = maps;
		screen->Parse();
		OnResize(m_rect.Width(), m_rect.Height());
		m_guard->SendAxis(MAKEWPARAM(titleVIEW, m_key), screen->m_mapH->caption);

		if (!isvirtual && !only)
		{
			m_guard->GetRepository(this);
			if (domino.IsEmpty())
				RestoreDomino();
			else
			{
				SaveDomino(domino);
				RestoreDomino(true);
			}
		}

		CString	string;

		string.Format(_T("%s\\%s\\axis.ini"), m_guard->m_root.GetString(), TABDIR);
		m_font = screen->m_mapH->fPoint;
		GetPrivateProfileString("Font", "apply", "", tmapN, sizeof(tmapN), string);
		string = tmapN;
		if (!atoi(string))
			SetFont(m_font, m_guard->m_resize, true);
		else
			SetFont(m_guard->m_font, m_guard->m_resize, true);

		OnStart(screen);

		if (m_current.m_idx == -1)
		{
			m_keyx->OnHome(true);
			m_current = m_default;
		}

		if (m_focus && m_current.m_idx != -1)
		{
			GetAtScreen(screen, m_current.m_key);
			screen->SetForm(m_current.m_idx, true);
		}
	}

	if (isvirtual)
	{
		int	key;

		if (!only)
		{
			if (!domino.IsEmpty())
				SaveDomino(domino);

			m_guard->GetRepository(this);
			if (!equal)
				RestoreDomino(m_dominos.IsEmpty() ? false : true);
			else if (!m_dominos.IsEmpty())
				RestoreDomino(true);
		}

		for (POSITION pos = m_screen.GetStartPosition(); pos; )
		{
			m_screen.GetNextAssoc(pos, key, screen);
			m_vm->OnSelect(screen, maps.Mid(L_SGID, L_SELC));
		}

		if (equal)
		{
			m_status &= ~S_ING;
			m_sync.Unlock();
			return true;
		}
	}

	m_status |= S_LOAD;
	m_status &= ~S_ING;

	AddTips();
	PlayAVI();

	if (!only && !(m_status & S_WAIT))
		CheckJumpByRule();

	if (m_status & (S_FLASH|S_BLINK))
		m_view->SetTimer(TM_RTM, TMI_RTM, NULL);
	m_sync.Unlock();

	return true;
}

void CClient::OnAxis(struct _axisH* axisH, char* pBytes, int nBytes)
{
	m_stream->OutStream(axisH, pBytes, nBytes);
}

void CClient::OnTrigger(CString name, CString text, int key)
{
	CScreen* screen;
	CfmBase* form;
	CString	string;

	if (m_status & (S_WAIT|S_TRIX|S_XTRI))
		return;

	if (key != -1)
		m_status |= S_TRIX;
	if (name.IsEmpty())
	{
		if (GetScreen(screen) && m_current.m_idx != -1)
		{
			form = screen->GetAtForm(m_current.m_idx);
			if (form && !(form->m_form->attr & FA_NOR))
				OnWrite(screen, form, text);
		}
		m_status &= ~S_TRIX;
		return;
	}

	for (int idx = 0; idx < m_magic; idx++)
	{
		if (GetAtScreen(screen, idx) && screen->m_key != key && screen->FindForm(name, form))
		{
			if (form->m_form->attr & FA_NOR)
				continue;

			OnWrite(screen, form, text);
			if (key == -1)	break;
		}
	}
	m_status &= ~S_TRIX;
}

void CClient::OnWrite(CScreen* screen, CfmBase* form, CString text)
{
	CString	string;

	form->ReadData(string);
	if (string.CompareNoCase(text))
	{
		form->WriteData(text);
		form->IsChanged();

		if (form->m_form->kind != FM_CONTROL)
		{
			m_vm->OnChange(screen, form);
			if ((form->m_form->attr & (FA_SEND|FA_MAND)) == FA_SEND)
				m_stream->InStream(screen);
		}
	}
}

void CClient::OnProcedure(CString proc, CString param, int key)
{
	CScreen* screen;

	if (m_status & S_WAIT)
		return;

	if (GetAtScreen(screen, key))
		m_vm->Procedure(screen, proc, param);
}

BOOL CClient::GetAt(CString name, CString& text)
{
	CScreen* screen;
	CfmBase* form;

	if (name.IsEmpty())
	{
		if (GetScreen(screen) && m_current.m_idx != -1)
		{
			form = screen->GetAtForm(m_current.m_idx);
			if (form)
			{
				text = CString((char *)form->m_form->name);
				return TRUE;
			}
		}
		return FALSE;
	}

	for (int idx = 0; idx < m_magic; idx++)
	{
		if (GetAtScreen(screen, idx) && screen->FindForm(name, form))
		{
			form->ReadData(text);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CClient::GetData(CString& name, CString& tmps)
{
	int	pos = -1;
	CScreen* screen;

	pos = name.Find('\t');
	if (pos != -1)
	{
		CString	procs = name.Mid(pos+1);
		for (int idx = 0; idx < m_magic; idx++)
		{
			if (GetAtScreen(screen, idx) && m_vm->OnProcedure(screen, procs))
				break;
		}
		name = name.Left(pos);
	}
	return TRUE;
}

void CClient::OnDomino(CString domino)
{
	if (m_status & S_WAIT)
		return;

	SaveDomino(domino);
	if (RestoreDomino(true))
		CheckJumpByRule();
}

void CClient::OnDrop(CPoint point, CString text, CWnd* pWnd)
{
	int key = text.GetAt(0);
	text.Delete(0);

	if (key == m_key)
		return;

	key = text.Find('\t');
	if (key == -1)
		return;

	CString name = text.Left(key++);
	text = text.Mid(key);
	OnTrigger(name, text, -1);
}

void CClient::OnMouse(CString infos, BOOL left)
{
	int	pos = -1;
	CString	tmps = _T(""), maps = _T(""), data = _T("");
	CPoint	point = CPoint(0, 0);

	for (int ii = 0; ii < 3; ii++)
	{
		pos = infos.Find('\n');
		if (pos != -1 && ii != 2)
		{
			tmps  = infos.Left(pos);
			infos = infos.Mid(pos+1);
		}
		else
			tmps = infos;

		switch (ii)
		{
		case 0:		// mapN
			pos = tmps.Find('\t');
			if (pos != -1)
				maps = tmps.Left(pos);
			else	maps = tmps;
			break;
		case 1:		// point
			pos = tmps.Find('\t');
			if (pos != -1)
			{
				point.x = atoi(tmps.Left(pos));
				point.y = atoi(tmps.Mid(pos+1));
			}
			break;
		case 2:		// data
			data = tmps + "\n";
			break;
		}
	}

	CScreen* main;
	if (!GetAtScreen(main))
		return;

	char	tmpb[1024]{};

	CStringArray	items;
	CString		mapN, path;

	m_guard->SendAxis(MAKEWPARAM(nameVIEW, m_key), (LPARAM)tmpb);
	maps = CString(tmpb, L_MAPN);
	items.RemoveAll();

	CMenu *popM = new CMenu();
	if (!popM->CreatePopupMenu())
	{
		delete popM;
		return;
	}
	m_guard->GetSignName(tmps);
	path.Format(_T("%s\\%s\\%s\\%s"), LPCTSTR(m_guard->m_root), USRDIR, LPCTSTR(tmps), LPCTSTR(maps.Left(L_SELC)));

	int index = -1;
	DWORD	rc = GetPrivateProfileSection(maps, tmpb, sizeof(tmpb), path.GetString());
	if (rc <= 0)
	{
		path.Format(_T("%s\\%s\\%s"), LPCTSTR(m_guard->m_root), MTBLDIR, LPCTSTR(maps.Left(L_SELC)));
		rc = GetPrivateProfileSection(maps, tmpb, sizeof(tmpb), path.GetString());
	}
	char* ptr = tmpb;
	for ( ; rc > 0; ptr++, rc--)
	{
		tmps = ptr;
		ptr += tmps.GetLength();
		rc   -= tmps.GetLength();
		if (tmps.GetLength() <= L_MAPN)
		{
			if (tmps.Find('-') == 0)
				popM->AppendMenu(MF_SEPARATOR, 0);
			continue;
		}
		mapN = tmps.Left(L_MAPN);
		tmps = tmps.Mid(L_MAPN);
		pos  = tmps.Find('=');
		if (pos != -1)
			tmps = tmps.Mid(pos+1);

		index = items.Add(mapN);
		popM->AppendMenu(MF_STRING|MF_ENABLED, menuBASE+index, tmps);
	}

	if (index >= 0)
	{
		index++;
		popM->AppendMenu(MF_SEPARATOR, 0);
	}

	if (m_guard->GetString(AS_LINK, tmps))
	{
		index++;
		popM->AppendMenu(MF_STRING|MF_ENABLED, menuLINK, tmps);
	}

	if (index < 0)
	{
		delete popM;
		return;
	}

	index = popM->TrackPopupMenu(TPM_LEFTALIGN|TPM_RETURNCMD, point.x, point.y, m_view);
	delete popM;

	if (index >= menuBASE)
	{
		index -= menuBASE;
		if (index < items.GetSize())
		{
			mapN = items.GetAt(index);
			mapN += "/s/t/d";
			mapN += data;
			m_guard->OpenView(this, mapN, typeVIEW);
		}
		items.RemoveAll();
		return;
	}

	items.RemoveAll();

	if (index == menuLINK)
	{
		m_string = maps;
		m_guard->PostAxis(MAKEWPARAM(linkVIEW, 0), (LPARAM)m_string.operator LPCTSTR());
	}
}

void CClient::SetPalette()
{
	CScreen* screen;

	if (GetAtScreen(screen))
		screen->SetPalette();
}

void CClient::FreeBK(bool reset)
{
	if (m_bitmap)
	{
		DeleteObject(m_bitmap);
		m_bitmap = NULL;
	}
	if (reset)
		SetBK();
}

void CClient::SetBK(char *bitmapN, bool update)
{
	CString	tmps;
	tmps.Format(_T("%s\\%s\\%s"), m_guard->m_root.GetString(), IMAGEDIR, bitmapN);

	FreeBK(false);
	m_bitmap = (HBITMAP)LoadImage(NULL, tmps.GetString(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (update)
		m_view->Invalidate();
}

void CClient::SetBK(COLORREF rgb, bool update)
{
	if (rgb != m_bkground)
	{
		FreeBK(false);
		m_bkground = rgb;
		if (update)
			m_view->Invalidate();
	}
}

void CClient::SetBK()
{
	char	wb[64]{};
	
	CString	text; text.Format(_T("%s/%s/%s"), m_guard->m_root.GetString(), TABDIR, PALETTE);
	DWORD	value = GetPrivateProfileString(GENERALSN, _T("Back"), "", wb, sizeof(wb), text.GetString());
	text = CString(wb, value); text.Trim();
	if (value <= 0 || text.IsEmpty())
		m_bkground = GetSysColor(COLOR_BTNFACE);
	else
	{
		value = atoi(text);
		if (value & 0x02000000)
			m_bkground = value;
		else
			m_bkground = m_guard->GetRGB(value);
	}
}

void CClient::CheckJumpByRule(CScreen* screen)
{
	bool	main = false;
	if (screen == NULL)
	{
		main = true;
		GetAtScreen(screen);
	}

	switch (screen->m_mapH->mapM)
	{
	case MM_SEND:
		if (main)
			m_stream->InStream(true);
		else
			m_stream->InStream(screen, true);
		break;
	case MM_FORM:
	case MM_MENU:
	default:
		break;
	}
}

bool CClient::SetAtScreen(CScreen*& screen, CRect rect, char* mapN, int key, int parent)
{
//	CString	tmpx;
	screen = new CScreen(this, rect);
	if (!screen->Attach(mapN))
	{
		delete screen;
		return false;
	}

	if (key == -1)
	{
		CScreen* tmps;

		for (key = 0; key < m_magic; key++)
		{
			if (!m_screen.Lookup(key, tmps) && key > parent)
				break;
		}

		if (key >= m_magic)
		{
			int	magic = 0;

			for (POSITION pos = m_screen.GetStartPosition(); pos; )
			{
				m_screen.GetNextAssoc(pos, key, tmps);
				magic = max(magic, key);
			}
			if (++magic < m_magic)
				key = magic;
			else
				key = m_magic++;
		}
	}

	m_screen.SetAt(key, screen);
	screen->m_key = key;

	m_slog.Format("[mapkey][%.8s] m_key=[%d] magic=[%d] ", screen->m_mapH->mapN, screen->m_key, m_magic);
	//OutputDebugString(m_slog);
	LOG_OUTP(3, "axwizard", __FUNCTION__, m_slog);
	

	return true;
}

void CClient::RemoveAtScreen(int key)
{
	CScreen* screen;

	if (!GetAtScreen(screen, key))
		return;

	m_screen.RemoveKey(key);
	if (screen->m_key == m_current.m_key)
	{
		m_current.m_key = 0;
		m_current.m_idx = -1;
	}
	if (screen->m_key == m_default.m_key)
	{
		m_default.m_key = 0;
		m_default.m_idx = -1;
	}

	delete screen;
}

void CClient::OnClose(CScreen* screen)
{
	if (screen)
		m_vm->OnClose(screen);
	else
	{
		for (int ii = 0; ii < m_magic; ii++)
		{
			if (GetAtScreen(screen, ii))
				m_vm->OnClose(screen);
		}
	}
}

bool CClient::SetObject(int key, CScreen*& screen, CRect fmRect, char* mapN, int parent)
{
	if (GetAtScreen(screen, key))
	{
		m_screen.RemoveKey(key);

		if (key == m_default.m_key)
		{
			m_default.m_key = 0;
			m_default.m_idx = -1;
		}

		if (m_mouse->m_hover.m_key == screen->m_key)
		{
			m_mouse->m_hover.m_key = -1;
			m_mouse->m_hover.m_idx = -1;
		}

		if (m_mouse->m_move.m_key == screen->m_key)
		{
			m_mouse->m_move.m_key = -1;
			m_mouse->m_move.m_idx = -1;
		}

		if (m_mouse->m_caret.m_key == screen->m_key)
			m_mouse->m_caret.m_key = -1;

		OnClose(screen);
		delete screen;
	}

	if (mapN == NULL)
		return false;

	return SetAtScreen(screen, fmRect, mapN, key, parent);
}

bool CClient::SetObject(CScreen* main, int key, char *mapN, bool force)
{
	CScreen* screen;
	int	parent = 0, object = 0;
	bool	cursor = false;
	bool	focus  = false;

	m_sync.Lock();
	CfmBase* form = main->GetAtForm(key);
	if (!form)
	{
		m_sync.Unlock();
		return false;
	}

	if (GetAtScreen(screen, form->m_form->keys))
	{
		if (!force && !memcmp(screen->m_mapH->mapN, mapN, L_MAPN))
		{
			m_sync.Unlock();
			return true;
		}

		WhereAmi(screen, cursor, focus);
		if (focus)	SetCPos(main, key);
		m_guard->xAlert(this, screen);

		parent = screen->m_parent;
		object = screen->m_object;
		key = screen->m_key;
		if (key == m_mouse->m_move.m_key)
		{
			m_mouse->m_screen = NULL;
			m_mouse->m_move.m_key = -1;
			m_mouse->m_move.m_idx = -1;
		}
	}
	else
	{
		parent = main->m_key;
		object = key;
		key = -1;
	}
	SaveDomino();

	CRect	unitRc;
	/*
	if (main->isMain())
		unitRc = CRect(form->m_form->left, form->m_form->top, form->m_form->right, form->m_form->bottom);
	else
	{
		float hr = (float)main->m_rect.Width() / (float)main->m_mapH->width;
		float vr = (float)main->m_rect.Height() / (float)main->m_mapH->height;

		unitRc.left   = int(main->m_rect.left+form->m_form->left*hr);
		unitRc.top    = int(main->m_rect.top+form->m_form->top*vr);
		unitRc.right  = int(main->m_rect.left+form->m_form->right*hr);
		unitRc.bottom = int(main->m_rect.top+form->m_form->bottom*vr);
	}
	*/
	unitRc = form->GetRect(true);
	bool success = SetObject(key, screen, unitRc, mapN, parent);

	if (success)
	{
		form->m_form->keys = screen->m_key;
		screen->m_parent   = parent;
		screen->m_object   = object;
		screen->Parse(true, (form->m_form->attr2 & GO_FIX) ? true : false);
		if (m_type & vtypeMSK)
			screen->SetFont(m_guard->m_resize, m_font);

		if (screen->m_mapH->options & OP_DOMINO)
			RestoreDomino(screen, m_dominos);
		else
			CodeDomino(screen, m_dominos);
		OnStart(screen);
	}
	unitRc = form->GetRect();
	m_view->InvalidateRect(unitRc);

	if (cursor)
	{
		m_default.m_key = 0;
		m_default.m_idx = -1;
		m_keyx->OnHome(true);
	}

	if (success)
	{
		AddTips();
		if (focus)	SetCPos(true);
	}
	m_sync.Unlock();
//	m_mouse->OnCursor();

	return success;
}

void CClient::GetKeyObject(CScreen* screen, CfmBase*& form)
{
	CScreen* main;

	if (GetAtScreen(main, screen->m_parent))
		form = main->GetAtForm(screen->m_object);
}

void CClient::GetNextScreen(CScreen*& screen, int& key)
{
	if (screen->isMain())
	{
		key = 0;
		return;
	}

	key = screen->m_object;
	GetAtScreen(screen, screen->m_parent);
	key = screen->m_formR[key].index;
	key++;
}

void CClient::GetPrevScreen(CScreen*& screen, int& key)
{
	if (screen->isMain())
		key = screen->m_tabL;
	else
	{
		key = screen->m_object;
		GetAtScreen(screen, screen->m_parent);
		key = screen->m_formR[key].index;
		if (key == 0)
			key = screen->m_tabL;
	}
	if (key) key--;
}

void CClient::OnScroll()
{
	if ((m_type & vtypeMSK) != vtypeSCR)
		return;

	CScreen* screen;

	CSize size = ((CScrollView*)m_view)->GetScrollPosition();
	if (size == m_offset)
		return;

	for (int ii = 0; ii < m_magic; ii++)
	{
		if (GetAtScreen(screen, ii))
			screen->AdjustForm(-size);
	}
	m_offset = size;
}

void CClient::OnResize(int Width, int Height)
{
	CScreen* screen;
	CScreen* main;

	History(false);

	switch (m_type & vtypeMSK)
	{
	case vtypeFIX:
	case vtypeWND:
	case vtypeFEX:
		return;
	default:
		break;
	}

	float hRatio = 1.0, vRatio = 1.0;

//	if ((m_type & vtypeMSK) != vtypeRSZ)				// 2006_04_06
	if (m_guard->m_resize || (m_type & vtypeMSK) != vtypeRSZ)	// 2006_04_06
	{
		if (Height && Width)
		{
			if (!GetAtScreen(main))
				return;

			hRatio = (float)Width / (float)main->m_mapH->width;
			vRatio = (float)Height / (float)main->m_mapH->height;
		}
		if ((m_type & vtypeMSK) == vtypeREX)
		{
			vRatio = m_vR;
			hRatio = (float)max(1.0, hRatio);
		}

		m_rect.left   = 0;
		m_rect.right  = Width;
		m_rect.top    = 0;
		m_rect.bottom = Height;
	}

	for (int ii = 0; ii < m_magic; ii++)
	{
		if (!GetAtScreen(screen, ii))
			continue;
		screen->ResizeForm(hRatio, vRatio, (m_type & vtypeMSK) != vtypeREX);
	}
}

void CClient::OnFocus(BOOL focus, HWND hWnd)
{
	m_focus = focus;
	if (m_focus)
		m_guard->ClearFocus(m_key);

	if (m_status & S_LOAD)
	{
		CScreen* screen;
	
		if (!focus)
		{
			if (m_mouse->m_capture && ::IsWindow(hWnd) && hWnd != m_guard->GetParent()->GetParent()->GetSafeHwnd())
			{
				m_mouse->m_capture = false;
				::ReleaseCapture();
			}
			if (m_mouse->m_hover.m_key >= 0 && m_mouse->m_hover.m_idx >= 0 &&
							GetAtScreen(screen, m_mouse->m_hover.m_key))
			{
				int	result;
				CfmBase* form = screen->GetAtForm(m_mouse->m_hover.m_idx);
				if (form)
					form->OnDrag(CPoint(-10, -10), result);
			}
			if (m_mouse->m_move.m_key >= 0 && m_mouse->m_move.m_idx >= 0 &&
							GetAtScreen(screen, m_mouse->m_move.m_key))
			{
				CfmBase* form = screen->GetAtForm(m_mouse->m_move.m_idx);
				if (form)
					m_vm->OnMouseLeave(screen, form);
			}
		}

		if (m_current.m_idx != -1 && GetScreen(screen))
			screen->SetForm(m_current.m_idx, focus ? true : false);
	//
	/*
		if (m_current.m_idx != -1 && GetScreen(screen))
		{
			CfmBase* form = screen->GetAtForm(m_current.m_idx);
			if (form->m_form->kind == FM_CONTROL)
			{
				CScreen* screenMain;
				if (focus && GetAtScreen(screenMain))
				{
					CPoint pt;
					GetCursorPos(&pt);

					screenMain->m_view->ScreenToClient(&pt);
					if (!ControlAtPt(screenMain, form, pt))
						screen->SetForm(m_current.m_idx, focus ? true : false);
				}
			}
			else
				screen->SetForm(m_current.m_idx, focus ? true : false);
		}
	*/

		if (focus)
		{
			if (hWnd && ::IsChild(m_view->GetSafeHwnd(), hWnd))
			{
				ChangeIME(true);
				return;
			}

			for (int key = 0; key < m_magic; key++)
			{
				if (GetAtScreen(screen, key))
					m_vm->OnFocus(screen);
			}
			ChangeIME(true);
		}
	}
}

void CClient::OnControl(HWND hWnd, bool focus)
{
	int	key = -1;
	CScreen *screen;

	for (int idx = 0; idx < m_magic; idx++)
	{
		if (GetAtScreen(screen, idx) && screen->OnControl(hWnd, key))
		{
			if (m_current.m_key != screen->m_key ||	m_current.m_idx != key)
			{
				idx = screen->m_key;
				if (GetScreen(screen))
					screen->SetForm(m_current.m_idx, false);
				m_current.m_key = idx;
				m_current.m_idx = key;
			}
			return;
		}
	}

	/*
	for (int idx = 0; idx < m_magic; idx++)
	{
		if (GetAtScreen(screen, idx) && screen->OnControl(hWnd, key))
		{
			if (m_current.m_key != screen->m_key ||	m_current.m_idx != key)
			{
				idx = screen->m_key;
				if (GetScreen(screen))
					screen->SetForm(m_current.m_idx, false);
				m_current.m_key = idx;
				m_current.m_idx = key;
			}
			return;
		}
	}

	if (m_focus && !(m_status & (S_CHILD|S_FIND|S_COMBO)))
	{
		//m_view->SetFocus();
		CfmCtrl* ctrl = NULL;
		CScreen* screenMain;
		if (GetAtScreen(screenMain))
			ctrl = FindControl(screenMain, hWnd);

		if (ctrl != NULL && ctrl->m_form->iok == EIO_NOP)
		{
			if (focus == false)
				OnFocus(FALSE, hWnd);
		}
		else
			m_view->SetFocus();
	}
	*/
}

bool CClient::OnControl()
{
	int	key = -1;
	CScreen *screen;

	for (int idx = 0; idx < m_magic; idx++)
	{
		if (GetAtScreen(screen, idx) && screen->OnControl(NULL, key, true))
			return true;
	}
	return false;
}

void CClient::OnFocus(CScreen* screen, int idx, bool event)
{
	CCaret	caret = m_current;

	SetCPos(screen, idx, event);

	if (GetAtScreen(screen, caret.m_key))
	{
		if (caret.m_idx < 0 || caret.m_idx >= screen->m_mapH->formN || caret == m_current)
			return;

		CfmBase* form = screen->GetAtForm(caret.m_idx);
		if (form)
		{
			if (form->m_form->kind == FM_OBJECT && GetAtScreen(screen, m_mouse->m_caret.m_key))
			{
				if (m_mouse->m_caret.m_idx != -1 && m_mouse->m_caret.m_idx < screen->m_mapH->formN)
					form = screen->GetAtForm(m_mouse->m_caret.m_idx);
				else	return;
			}
			if (form && form->IsChanged())
				m_vm->OnChange(screen, form);
		}
	}
}

void CClient::OnChange(CScreen* screen)
{
	CCaret	caret = m_current;

	if (GetAtScreen(screen, caret.m_key))
	{
		if (caret.m_idx < 0 || caret.m_idx >= screen->m_mapH->formN)
			return;

		CfmBase* form = screen->GetAtForm(caret.m_idx);
		if (form && form->IsChanged())
			m_vm->OnChange(screen, form);
	}
}

void CClient::OnDraw(CDC* pDC)
{
	if (pDC->IsPrinting())
	{
		OnPrint(pDC);
		return;
	}

	OnPaint(pDC, false);
	return;
}

void CClient::OnPaint(CDC* pDC, bool isPrint)
{
	CScreen	*screen;

	m_sync.Lock();

	for (int ii = 0; ii < m_magic; ii++)
	{
		if (!GetAtScreen(screen, ii))
			continue;

		if (!screen->isMain())
		{
			CfmBase* form = nullptr;
			CRect	fmRect;

			GetKeyObject(screen, form);
			if (!form) continue;

			fmRect = form->GetRect();
			if (!pDC->RectVisible(&fmRect))
				continue;

			if (!form->IsVisible() || !isVisible(screen, form))
			{
				screen->DrawForm(NULL);
				continue;
			}
		}

		screen->DrawForm(pDC);
	}

	m_sync.Unlock();
}

bool CClient::isVisible(CScreen* screen, CfmBase* form)
{
	CScreen* main;

	if (!form->IsVisible())
		return false;
	if (!GetAtScreen(main, screen->m_parent) || !main->IsFormVisible(form))
		return false;

	if (main->isMain())
		return true;

	GetKeyObject(main, form);
	return isVisible(main, form);
}

bool CClient::isVisible(int key, int idx)
{
	CScreen* screen;

	if (GetAtScreen(screen, key) && idx != -1)
	{
		CfmBase	*object, *base;

		base = screen->GetAtForm(idx);
		if (!screen->isMain())
		{
			GetKeyObject(screen, object);
			if (!isVisible(screen, object) || !base || (base && !screen->IsFormVisible(base)))
				return false;
		}
		else if (!screen->IsFormVisible(base))
			return false;
	}
	return true;
}

#include <VersionHelpers.h>

void CClient::OnPrint(CDC* pDC)
{
	CRect	clientRc, metaRc;
	HENHMETAFILE	metaFile;
	CMetaFileDC*	metaDC;

	int	cx, cy, numerator;

	if (pDC->IsPrinting())
		m_view->RedrawWindow();

	CDC* pcDC = m_view->GetDC();
	int  inx = pcDC->GetDeviceCaps(LOGPIXELSX);
	int  iny = pcDC->GetDeviceCaps(LOGPIXELSY);
	m_view->ReleaseDC(pcDC);

	numerator = 2540;
	if (!IsWindows8OrGreater())
	{
		cx = GetSystemMetrics(SM_CXFULLSCREEN);
		if (cx <= 800)
			numerator = 3860;
		else if (cx <= 1024)
			numerator = 3020;
		else
			numerator = 2670;
	}

	m_view->GetClientRect(clientRc);
	cx = MulDiv(clientRc.Width(), numerator, inx);
	cy = MulDiv(clientRc.Height(), numerator, iny);

	metaRc = CRect(0, 0, cx, cy);
	metaDC = new CMetaFileDC();
	if (!metaDC->CreateEnhanced(NULL, NULL, metaRc, NULL))
	{
		delete metaDC;
		return;
	}

	int status = m_guard->m_app->GetProfileInt(ENVIRONMENT, STATUS, 0);;
	metaDC->m_bPrinting = status & stBlackPrint;
	metaDC->m_hAttribDC = pDC->m_hAttribDC;
	OnPaint(metaDC, true);
	metaFile = metaDC->CloseEnhanced();

	if (pDC->IsPrinting())
	{
		PRINTDLG	printDlg;
		bool		vpaper = true;
		if (m_guard->m_app->GetPrinterDeviceDefaults(&printDlg))
		{
			CPrintDialog	dlg(FALSE);
			dlg.m_pd.hDevMode = printDlg.hDevMode;
			LPDEVMODE pDevMode = (LPDEVMODE) ::GlobalLock(dlg.m_pd.hDevMode);
			if (pDevMode)
			{
				if (pDevMode->dmOrientation == DMORIENT_LANDSCAPE)
					vpaper = false;
				::GlobalUnlock(dlg.m_pd.hDevMode);
			}
		}

		CScreen* screen;
		if (vpaper && GetAtScreen(screen) && screen->m_mapH->width < screen->m_mapH->height)
			vpaper = false;

		const int	space = 50;
		pDC->SetMapMode(MM_LOMETRIC);
		if (vpaper)
			clientRc = CRect(space/2, -space/2, pDC->GetDeviceCaps(HORZSIZE)*10-space, pDC->GetDeviceCaps(VERTSIZE)*(-6)+space);
		else
			clientRc = CRect(space/2, -space/2, pDC->GetDeviceCaps(HORZSIZE)*10-space, pDC->GetDeviceCaps(VERTSIZE)*(-10)+space);
	}

	pDC->PlayMetaFile(metaFile, clientRc);
	DeleteEnhMetaFile(metaFile);
	delete metaDC;
}

void CClient::SetCPos(bool event)
{
	if (!event)
	{
		if (m_status & S_CURSOR)
		{
			m_status &= ~S_CURSOR;
			return;
		}
	}

	if (m_default.m_idx == -1)
		return;

	if (m_focus)
	{
		CScreen* screen;
		if (m_current.m_idx != -1)
		{
			if (GetScreen(screen))
				screen->SetForm(m_current.m_idx, false);
		}

		if (GetAtScreen(screen, m_default.m_key))
			screen->SetForm(m_default.m_idx, true);
	}
	m_current = m_default;
}

void CClient::SetCPos(CScreen *screen, int pos, bool event)
{
	/* 20080329
	if (pos < 0 || pos >= screen->m_mapH->formN)
		return;

	CScreen*  active;
	if (m_focus)
	{
		if (GetScreen(active) && m_current.m_idx != -1)
			active->SetForm(m_current.m_idx, false);

		screen->SetForm(pos, true);
	}
	m_current.m_key = screen->m_key;
	m_current.m_idx = pos;
	*/

	if (pos < 0 || pos >= screen->m_mapH->formN)
		return;

	CScreen*  active;
	CCaret	caret = m_current;

	m_current.m_key = screen->m_key;
	m_current.m_idx = pos;
	if (m_focus)
	{
		if (GetAtScreen(active, caret.m_key) && caret.m_idx != -1)
			active->SetForm(caret.m_idx, false);

		screen->SetForm(pos, true);
	}

	if (!event)
		m_status |= S_CURSOR;
}

bool CClient::isPush(CString &pushN)
{
	CScreen *screen;
	for (int idx = 0; idx < m_magic; idx++)
	{
		if (GetAtScreen(screen, idx) && screen->isPush(pushN))
			return true;
	}
	return false;
}

void CClient::OnPush(CString pushs)
{
	m_sync.Lock();
	m_status |= S_LOCK;

	CScreen *screen;
	for (int idx = 0; idx < m_magic; idx++)
	{
		if (GetAtScreen(screen, idx))
			screen->OnPush(pushs);
	}

	m_status &= ~S_LOCK;
	m_sync.Unlock();
}

void CClient::OnAlert(CString code, CString update, CdataSet* fms, CObArray* obs, int stat, struct _alertR* alertR)
{
	if (!(m_status & S_FLASH))
		return;

	m_sync.Lock();
	m_status |= S_LOCK;

	CScreen* screen;
	for (int idx = 0; idx < m_magic; idx++)
	{
		if (GetAtScreen(screen, idx) && screen->isFlash())
		{
			CString dbg;
			dbg.Format("[WIZARD][RTM][DEBUG] visit mapN=%s screen=%p code=%s\n",
			CString(screen->m_mapH->mapN, L_MAPN).GetString(), screen, code.GetString());
			OutputDebugString(dbg);
			if (screen->OnAlert(code, update, fms, obs, stat, alertR))
				m_vm->OnProcedure(screen, "OnAlert");
			m_vm->OnAlert(screen, code, update, stat);
		}
	}

	m_status &= ~S_LOCK;
	m_sync.Unlock();
}

void CClient::OnNotice(CdataSet& major, CdataSet& minor, CdataSet& fms, CString text)
{
	m_sync.Lock();
	m_status |= S_LOCK;

	CScreen* screen;
	for (int idx = 0; idx < m_magic; idx++)
	{
		if (GetAtScreen(screen, idx) && screen->isNotice())
		{
			if (screen->OnNotice(major, minor, fms, text))
				m_vm->OnProcedure(screen, _T("OnNotice"));
		}
	}

	m_status &= ~S_LOCK;
	m_sync.Unlock();
}

void CClient::OnClear()
{
	m_sync.Lock();

	CScreen* screen;
	for (int idx = 0; idx < m_magic; idx++)
	{
		if (GetAtScreen(screen, idx))
		{
			if (m_status & S_FLASH)
				screen->OnClear();
			screen->Blink();
		}
	}
	m_sync.Unlock();
}

void CClient::SetBlink()
{
	if (m_status & (S_FLASH|S_BLINK))
		return;

	m_status |= S_BLINK;
	if (m_status & S_LOAD)
		m_view->SetTimer(TM_RTM, TMI_RTM, NULL);
}

int CClient::xAlert(CScreen* screen, char* keys)
{
	int	key, ukey, value, idx = 0;
	int	form, parent = screen->m_key;

	if (screen->isFlash() || screen->m_mapH->options & OP_OOP)
		keys[idx++] = screen->m_key;

	for (POSITION pos = m_keys.GetStartPosition(); pos; )
	{
		m_keys.GetNextAssoc(pos, key, value);
		if (parent == GetScreenKey(key, form, ukey))
		{
			keys[idx++] = key;
			m_keys.RemoveKey(key);
		}
	}

	for (key = 0; key < m_magic; key++)
	{
		if (GetAtScreen(screen, key) && !screen->isMain() &&
						screen->m_parent == parent)
			idx += xAlert(screen, &keys[idx]);
	}
	return idx;
}

void CClient::WhereAmi(CScreen* screen, bool& cursor, bool& focus)
{
	int	parent = screen->m_key;

	if (m_default.m_key == screen->m_key)
		cursor = true;
	if (m_current.m_key == screen->m_key)
		focus = true;
	for (int key = 0; key < m_magic; key++)
	{
		if (GetAtScreen(screen, key) && !screen->isMain() &&
						screen->m_parent == parent)
			WhereAmi(screen, cursor, focus);
	}
}

void CClient::SetRepository(CScreen* screen)
{
	int	parent = screen->m_key;

	m_guard->SetRepository(screen);
	for (int key = 0; key < m_magic; key++)
	{
		if (GetAtScreen(screen, key) && !screen->isMain() && screen->m_parent == parent)
			SetRepository(screen);
	}
}

void CClient::GetRepository(CScreen* screen)
{
	int	parent = screen->m_key;

	m_guard->GetRepository(screen);
	for (int key = 0; key < m_magic; key++)
	{
		if (GetAtScreen(screen, key) && !screen->isMain() && screen->m_parent == parent)
			GetRepository(screen);
	}
}

void CClient::SaveDomino(CString domino)
{
	m_dominos.RemoveAll();

	int	idx;
	CString	keys, text;

	while (!domino.IsEmpty())
	{
		idx = domino.Find('\n');
		if (idx == -1)
		{
			text = domino;
			domino.Empty();
		}
		else
		{
			text = domino.Left(idx++);
			domino = domino.Mid(idx);
		}

		idx = text.Find('\t');
		if (idx != -1)
		{
			keys = text.Left(idx++);
			text = text.Mid(idx);
			m_dominos.SetAt(keys, text);
		}
	}
}

void CClient::SaveDomino()
{
	m_dominos.RemoveAll();
	m_guard->GetCode(m_dominos);

	CString	text{};
	CScreen* screen;
	CfmBase* form;

	int	idx, jj, kk;
	int	nCols = 0, nRows = 0;
	struct	_cellR *cell{};
	CString	name{}, tmps{};

	for (idx = 0; idx < m_magic; idx++)
	{
		if (!GetAtScreen(screen, idx))
			continue;
		for (int ii = 0; ii < screen->m_mapH->formN; ii++)
		{
			form = screen->GetAtForm(ii);
			if (!form || (form->m_form->attr & FA_NOR))
				continue;
			switch (form->m_form->kind)
			{
			case FM_MEMO:
				continue;
			case FM_GRID:
				cell = (struct _cellR *) &screen->m_cellR[form->m_form->vals[0]];
				nCols = HIWORD(form->m_form->vals[1]);
				nRows = LOWORD(form->m_form->vals[1]);

				for (jj = 0; jj < nCols; jj++)
				{
					switch (cell[jj].iok)
					{
					case EIO_INPUT:
					case EIO_INOUT:
						break;
					default:
						continue;
					}

					for (kk = 0; kk < nRows; kk++)
					{
						form->ReadData(text, false, -1, jj, kk);
						text.TrimRight();
						if (text.IsEmpty())
							continue;

						tmps.Format(_T("%d$%d"), jj, kk);
						name = (char *)form->m_form->name;
						name += tmps;
						name = name + (char *)cell[jj].name;
						m_dominos.SetAt(name, text);
					}
				}
				continue;
			default:
				switch (form->m_form->iok)
				{
				case EIO_INPUT:
				case EIO_INOUT:
					break;
				default:
					continue;
				}
				break;
			}

			form->ReadData(text);
			text.TrimRight();
			if (!text.IsEmpty())
				m_dominos.SetAt((char *)form->m_form->name, text);
		}
	}
}

int CClient::RestoreDomino(bool force)
{
	CScreen	*screen;
	CMapStringToString codes;

	codes.RemoveAll();
	m_guard->GetCode(codes);
	if (m_dominos.IsEmpty() && codes.IsEmpty())
		return 0;

	int dominoN = 0;
	for (int idx = 0; idx < m_magic; idx++)
	{
		if (GetAtScreen(screen, idx))
		{
			if (force || (screen->m_mapH->options & OP_DOMINO))
				dominoN += RestoreDomino(screen, m_dominos, force);
			else
				dominoN += RestoreDomino(screen, codes, force);
		}
	}
	m_dominos.RemoveAll();
	return dominoN;
}

int CClient::RestoreDomino(CScreen* screen, CMapStringToString& dominos, bool force)
{
	int	nCols, nRows;
	struct	_cellR *cell;
	CString	text, name, tmps;

	int	ii, kk;
	CfmBase	*form;

	if (dominos.IsEmpty())
		return 0;

	int	dominoN = 0;

	for (int idx = 0; idx < screen->m_mapH->formN; idx++)
	{
		form = screen->GetAtForm(idx);
		if (!force && form->m_form->attr & FA_NOR)
			continue;
		switch (form->m_form->kind)
		{
		case FM_MEMO:
			continue;
		case FM_GRID:
			cell = (struct _cellR *) &screen->m_cellR[form->m_form->vals[0]];
			nCols = HIWORD(form->m_form->vals[1]);
			nRows = LOWORD(form->m_form->vals[1]);

			for (ii = 0; ii < nCols; ii++)
			{
				switch (cell[ii].iok)
				{
				case EIO_INPUT:
				case EIO_INOUT:
					break;
				default:
					continue;
				}

				for (kk = 0; kk < nRows; kk++)
				{
					tmps.Format(_T("%d$%d"), ii, kk);
					name = (char *)form->m_form->name;
					name += tmps;
					name = name + (char *)cell[ii].name;
					if (dominos.Lookup(name, text))
					{
						form->WriteData(text, false, ii, kk);
						dominoN++;
					}
				}
			}
			continue;
		default:
			switch (form->m_form->iok)
			{
			case EIO_INPUT:
			case EIO_INOUT:
				break;
			default:
				continue;
			}
			break;
		}

		if (dominos.Lookup((char *)form->m_form->name, text))
		{
			if (form->m_form->kind == FM_CONTROL)
				form->WriteData(text, true, -2);
			else
				form->WriteData(text);
			form->IsChanged();
			m_vm->OnChange(screen, form);
			dominoN++;
		}
	}
	return dominoN;
}

void CClient::CodeDomino(CScreen* screen, CMapStringToString& dominos)
{
	CString	text{};
	CfmBase	*form = NULL;

	if (dominos.IsEmpty())
		return;

	for (int idx = 0; idx < screen->m_mapH->formN; idx++)
	{
		form = screen->GetAtForm(idx);
		if (!form || form->m_form->attr & FA_NOR)
			continue;
		if (!(form->m_form->properties & PR_CODE))
		{
			m_guard->GetCode((char *)form->m_form->name, text);
			if (text.IsEmpty())
				continue;
		}

		switch (form->m_form->iok)
		{
		case EIO_INPUT:
		case EIO_INOUT:
			if (dominos.Lookup((char *)form->m_form->name, text))
			{
				form->WriteData(text);
				form->IsChanged();
				m_vm->OnChange(screen, form);
			}
			break;
		}
	}
}

bool CClient::OnTRAN(bool byKey)
{
	CScreen* screen;
	for (int ii = 0; ii < m_magic; ii++)
	{
		if (!GetAtScreen(screen, ii) || screen->isUob())
			continue;
		if (!screen->OnTRAN(byKey) && screen->isMain())
			return false;
	}
	return true;
}

bool CClient::isKeyButton(WPARAM wParam)
{
	CScreen* screen;

	if (GetScreen(screen) && screen->isKeyButton(wParam))
		return true;

	for (int ii = 0; ii < m_magic; ii++)
	{
		if (!GetAtScreen(screen, ii))
			continue;
		if (screen->isKeyButton(wParam))
			return true;
	}
	return false;
}

void CClient::WaitState(CScreen* screen, bool timeout)
{
	if (screen)
	{
		screen->m_state |= waitSN;
		m_units.SetAt(screen->m_key, screen->m_key);
	}
	else
	{
		for (int idx = 0; idx < m_magic; idx++)
		{
			if (!GetAtScreen(screen, idx))
				continue;

			if (screen->m_skip || screen->m_mapH->mapM == MM_MENU)
				continue;
			if (screen->isUob())
				continue;
			screen->m_state |= waitSN;
			m_units.SetAt(screen->m_key, screen->m_key);
		}
	}

	m_status |= S_WAIT;
	if (timeout && m_guard->m_wait > 0)
		m_view->SetTimer(TM_WAIT, m_guard->m_wait, NULL);
	m_guard->PostAxis(MAKEWPARAM(waitPAN, m_key), true);
}

void CClient::WaitDone(CScreen* screen, bool timeout, bool wait, int key)
{
	int	ii;

	if (screen)
	{
		if (key < 0)
			key = screen->m_key;
		m_units.RemoveKey(key);

		if (wait)
			screen->m_state &= ~waitSN;
		for (ii = 0; ii < m_magic; ii++)
		{
			if (!GetAtScreen(screen, ii))
				continue;
			if (screen->isUob())
				continue;
			if (screen->m_state & waitSN)
				return;
		}
	}
	else if (key >= 0)
		m_units.RemoveKey(key);
	else
	{
		for (ii = 0; ii < m_magic; ii++)
		{
			if (!GetAtScreen(screen, ii))
				continue;
			if (screen->isUob())
				continue;
			screen->m_state &= ~waitSN;
		}
		m_units.RemoveAll();
	}


	if (m_units.GetCount() <= 0)
	{
		m_status &= ~S_WAIT;
		if (timeout && m_guard->m_wait > 0)
			m_view->KillTimer(TM_WAIT);
		m_guard->PostAxis(MAKEWPARAM(waitPAN, m_key), false);
	}
	else
	{
		int	key, unit;
		for (POSITION pos = m_units.GetStartPosition(); pos; )
			m_units.GetNextAssoc(pos, key, unit);
	}

//	m_mouse->OnCursor();
}

void CClient::RepeatTR(CScreen* screen)
{
	int repeatV = screen->m_mapH->repeatV;
	if (repeatV <= TMI_REPTR)
		repeatV = TMI_REPTR;

	screen->m_state |= reptxSN;
	m_view->SetTimer(TM_REPTR + screen->m_key, repeatV, NULL);
}

void CClient::OnRepeat(int key)
{
	CScreen* screen;

	if (GetAtScreen(screen, key))
	{
		screen->m_state &= ~reptxSN;
		m_stream->InStream(screen);
	}
}

void CClient::ChangeIME(CScreen* screen, int idx)
{
	switch (screen->m_formR[idx].kind)
	{
	case FM_EDIT:
	case FM_COMBO:
	case FM_MEMO:
		switch (screen->m_formR[idx].type)
		{
		case IO_HAN:
			m_status |= S_HAN;
			break;
		case IO_MIXED:
		case IO_ANY:
			if (screen->m_formR[idx].attr & FA_HAN)
			{
				m_status |= S_HAN;
				break;
			}
			[[fallthrough]];
		default:
			m_status &= ~S_HAN;
			break;
		}
	/*	if (screen->m_formR[idx].attr & FA_PASSWD)
			m_status |= S_PASSWD;
		else
			m_status &= ~S_PASSWD;	*/
		break;
	default:
		return;
	}

	ChangeIME(true);
}

void CClient::ChangeIME(bool ime)
{
	DWORD	dwConversion{}, dwSentence{};

	HIMC hIMC = ImmGetContext(m_view->m_hWnd);
	if (!ImmGetConversionStatus(hIMC, &dwConversion, &dwSentence))
		return;

	if (ime)
	{
		DWORD dwConvSave = dwConversion;
		if (m_status & S_HAN)
			dwConversion |= IME_CMODE_HANGEUL;
		else
			dwConversion &= ~IME_CMODE_HANGEUL;

		if (m_status & S_FULL)
			dwConversion |= IME_CMODE_FULLSHAPE;
		else
			dwConversion &= ~IME_CMODE_FULLSHAPE;
		if (dwConversion != dwConvSave)
			::ImmSetConversionStatus(hIMC, dwConversion, dwSentence);
	
	/*	if (m_status & S_PASSWD)
			::ImmSetConversionStatus(hIMC, IME_CMODE_NOCONVERSION, dwSentence);
		else if (dwConversion != dwConvSave)
			::ImmSetConversionStatus(hIMC, dwConversion, dwSentence);
	*/	return;
	}

/*	if (m_status & S_PASSWD)
		::ImmSetConversionStatus(hIMC, IME_CMODE_NOCONVERSION, dwSentence);
	else
		::ImmSetConversionStatus(hIMC, dwConversion, dwSentence);
*/
	m_status &= ~(S_HAN|S_FULL);
	if (dwConversion & IME_CMODE_HANGEUL)
		m_status |= S_HAN;
	if (dwConversion & IME_CMODE_FULLSHAPE)
		m_status |= S_FULL;

	m_guard->PostAxis(MAKEWPARAM(hangulPAN, m_key), m_status & S_HAN);
	m_guard->PostAxis(MAKEWPARAM(doublePAN, m_key), m_status & S_FULL);
}

void CClient::ClearIME()
{
	HIMC hImc = ImmGetContext(m_view->m_hWnd);
	//	ImmSetCompositionString(hImc, SCS_SETSTR, NULL, 0, NULL, 0);
	ImmNotifyIME(hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
	ImmReleaseContext(m_view->m_hWnd, hImc);
}



bool CClient::RunMenu(CScreen* screen, CfmBase* form, CPoint point)
{
	CScreen* main;
	if (!GetAtScreen(main))
		return false;

	char	*ptr{}, tmpb[2048]{};

	CStringArray	items;
	CString		tmps, mapN, path, maps, domino;

	int pos = (m_status & S_WORKS) ? m_guard->GetOwner(m_key) : m_key;
	m_guard->SendAxis(MAKEWPARAM(nameVIEW, pos), (LPARAM)tmpb);
	maps = CString(tmpb, L_MAPN);
	items.RemoveAll();

	CMenu *popM = new CMenu();
	if (!popM->CreatePopupMenu())
	{
		delete popM;
		return false;
	}
	m_guard->GetSignName(tmps);
	path.Format(_T("%s\\%s\\%s\\%s"), LPCTSTR(m_guard->m_root), USRDIR, LPCTSTR(tmps), LPCTSTR(maps.Left(L_SELC)));

	int index = -1;
	int	rc = GetPrivateProfileSection(maps, tmpb, sizeof(tmpb), path);
	if (rc <= 0)
	{
		path.Format(_T("%s\\%s\\%s"), LPCTSTR(m_guard->m_root), MTBLDIR, LPCTSTR(maps.Left(L_SELC)));
		rc = GetPrivateProfileSection(maps, tmpb, sizeof(tmpb), path);
	}
	if (rc == sizeof(tmpb)-2)
	{
		for (rc--; --rc > 0; )
		{
			if (tmpb[rc] == '\0')
			{
				rc++;
				break;
			}
		}
	}
	ptr = tmpb;
	for ( ; rc > 0; ptr++, rc--)
	{
		tmps = ptr;
		ptr += tmps.GetLength();
		rc  -= tmps.GetLength();
		if (tmps.GetLength() <= L_MAPN)
		{
			if (tmps.Find('-') == 0)
				popM->AppendMenu(MF_SEPARATOR, 0);
			continue;
		}
		mapN = tmps.Left(L_MAPN);
		tmps = tmps.Mid(L_MAPN);
		pos  = tmps.Find('=');
		if (pos != -1)
			tmps = tmps.Mid(pos+1);

		index = items.Add(mapN);
		popM->AppendMenu(MF_STRING|MF_ENABLED, menuBASE+index, tmps);
	}

	if (index >= 0)
	{
		index++;
		popM->AppendMenu(MF_SEPARATOR, 0);
	}

	if (m_guard->GetString(AS_INIT, tmps))
	{
		index++;
		popM->AppendMenu(MF_STRING|MF_ENABLED, menuINIT, tmps);
	}

	if (m_guard->GetString(AS_COPY, tmps))
	{
		index++;
		popM->AppendMenu(MF_STRING|MF_ENABLED, menuCOPY, tmps);
	}

	if (!(main->m_mapH->options & OP_XPRINT) && m_guard->GetString(AS_PRINT, tmps))
	{
		index++;
		popM->AppendMenu(MF_STRING|MF_ENABLED, menuPRINT, tmps);
	}

	CMenu*	subM = NULL;
	CString	text;
	//xxxxxxxxxxxxxxxxxxxxxxxxxxxx [2023/01/17] [kjs]수정  
	// groupKey, groupName  
	std::vector<std::pair<int, CString>> groups;

	text = _T("");
	if (form)
	{
		switch (form->m_form->kind)
		{
		case FM_GRID:
		case FM_TABLE:
			pos = 0;
			if (m_guard->GetString(AS_2XLS, tmps))
			{
				if (form->m_form->kind == FM_TABLE
					|| (form->m_form->kind == FM_GRID && !(form->m_form->attr2 & GO_XXLS) && ((CfmGrid *)form)->GetValidRows() > 0))
					popM->AppendMenu(MF_STRING|MF_ENABLED, menuXLS, tmps);
			}
			if (form->m_form->kind == FM_GRID)
			{
				if ((form->m_form->attr & FA_ENUM) && m_guard->GetString(AS_ITEM, tmps))
					popM->AppendMenu(MF_STRING|MF_ENABLED, menuGRID, tmps);
				if (m_guard->GetString(AS_PORTFOLIO, tmps))
				{
					form->GetEnum(text, pos, 253);
					if (!text.IsEmpty())
					{
						index++;
						popM->AppendMenu(MF_SEPARATOR, 0);
						subM = new CMenu();
						subM->CreatePopupMenu();
						popM->AppendMenu(MF_STRING|MF_ENABLED|MF_POPUP, (UINT)subM->m_hMenu, tmps);

						//xxxxxxxxxxxxxxxxxxxxxxxxxxxx [2023/01/17] [kjs]수정
						sendInterGroup(screen, groups);						
						
						int ii = 0;
						for (auto item : groups)
						{
							subM->AppendMenu(MF_STRING | MF_ENABLED, menuGROUP + ii, item.second);
							ii++;
						}
						//xxxxxxxxxxxxxxxxxxxxxxxxxxxx
						subM->AppendMenu(MF_SEPARATOR, 0);
						if (m_guard->GetString(AS_NEWGROUP, tmps))
							subM->AppendMenu(MF_STRING|MF_ENABLED, menuNGROUP, tmps);
					}
				}
			}
			break;
		default:
			break;
		}
	}

	if (m_guard->GetString(AS_LINK, tmps))
	{
		index++;
		popM->AppendMenu(MF_STRING|MF_ENABLED, menuLINK, tmps);
	}

	if (index < 0)
	{
		delete popM;
		return false;
	}


	m_view->ClientToScreen(&point);
	index = popM->TrackPopupMenu(TPM_LEFTALIGN|TPM_RETURNCMD, point.x, point.y, m_view);
	if (subM)
		delete subM;
	delete popM;

	if (index >= menuGROUP)
	{
		index -= menuGROUP;
		if (index < (int)groups.size())
		{
			//xxxxxxxxxxxxxxxxxxxxxxxxxxxx [2023/01/17] [kjs]수정
			auto item = groups.at(index);
			domino.Format("IB202201/t/s/dappendITEMS\t%02d\t%s\n", item.first, text.Trim());	
			m_guard->OpenView(this, domino, typeVIEW);
			//xxxxxxxxxxxxxxxxxxxxxxxxxxxx	
		}
		groups.clear();
		return true;
	}

	if (index >= menuBASE)
	{
		index -= menuBASE;
		if (index < items.GetSize())
		{
			mapN = items.GetAt(index);
			tmps.Empty();
			if (form && form->m_form->kind == FM_GRID)
				SetCode(screen, form, tmps, true);

			if (tmps.IsEmpty())
				SetCode(tmps, true);

			mapN += "/t/d";	// "/s/t/d"; 20150731 IBK 요청에 의해 /s 삭제
			mapN += tmps;
			m_guard->OpenView(this, mapN, typeVIEW);
		}
		items.RemoveAll();
		return true;
	}

	items.RemoveAll();

	switch (index)
	{
	case menuXLS:
		MakeXlsFile(screen, form, true);
		return true;
	case menuTXT:
		return true;
	case menuGRID:
		EditColumns(screen, form);
		return true;
	case menuINIT:
		m_guard->PostAxis(MAKEWPARAM(initVIEW, m_key));
		return true;
	case menuCOPY:
		m_guard->PostAxis(MAKEWPARAM(copyVIEW, m_key));
		return true;
	case menuPRINT:
		m_guard->PostAxis(MAKEWPARAM(printVIEWx, m_key));
		return true;
	case menuLINK:
		m_string = maps;
		m_guard->PostAxis(MAKEWPARAM(linkVIEW, 0), (LPARAM)m_string.operator LPCTSTR());
		return true;
	case menuNGROUP:
		domino.Format("IB202201/dappendGROUP\t%s", maps);
		m_guard->OpenView(this, domino, typeVIEW);
		return true;
	default:
		return false;
	}

	return true;
}

int CClient::WhichForm(CScreen* screen, CPoint pt)
{
	int	ii, onPoint, myPoint, youPoint;

	onPoint = myPoint = youPoint = -1;
	for (ii = screen->m_mapH->formN-1; ii >= 0; ii--)
	{
		CfmBase *form = screen->GetAtForm(ii);
		if (!screen->IsFormVisible(form) || !form->IsPointInRect(pt))
			continue;

		switch (form->m_form->kind)
		{
		case FM_SYS:
		case FM_BOX:
		case FM_GROUP:
		case FM_AVI:
		case FM_OBJECT:
			if (youPoint == -1)
				youPoint = ii;
			break;

		case FM_LABEL:
			if (onPoint == -1)
				onPoint = ii;
			break;
		case FM_EDIT:
		case FM_MEMO:
		case FM_BUTTON:
		case FM_COMBO:
		case FM_OUT:
		case FM_GRID:
		case FM_TAB:
		case FM_UTAB:
		case FM_TABLE:
		case FM_TREE:
		case FM_SHEET:
		case FM_BROWSER:
			return ii;

		case FM_PANEL:
			if (myPoint == -1)
				myPoint = ii;
			break;
		}
	}

	ii = (onPoint != -1) ? onPoint : (myPoint != -1) ? myPoint : youPoint;
	return ii;
}

void CClient::ClearShift()
{
	CScreen* screen;

	m_status &= ~(S_SHIFT|S_CONTROL);
	if (GetAtScreen(screen, m_cbox.m_key))
	{
		CfmBase* form = screen->GetAtForm(m_cbox.m_idx);
		if (form)
			form->ClearData(true);
	}
}

void CClient::History(CfmBase* form, int mode)
{
	if (m_status & (S_CHILD|S_FIND))
		return;

	CRect	wRc;
	CSize	size;

	wRc = form->GetRect();
	size.cx = wRc.Width();
	size.cy = wRc.Height();

	bool	self;
	CString	items;
	int	count, height;

	self = true;
	height = size.cy;

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// [KJS]	   combo 사이즈 변경 17->18
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	switch (mode)
	{
	case RC_SEARCH:
		items = m_guard->GetCodes();
		size.cx = 180;
		size.cy = 15*18;	// 160;
		m_status |= S_FIND;
		break;
	case RC_NOP:
		self = false;
	case RC_CODE:
		count = GetCodes((char *)form->m_form->name, items, self);
		if (count == 0)
			return;
		size.cx = (items.Find(' ') == -1) ? 80 : 180;
	//	size.cy = count < 9 ? count*13+6 : 120;
		size.cy = count <= 20 ? count*18 : 20*18;	// 140;
		m_status |= self ? S_CHILD : S_FIND;
		break;
	default:
		return;
	}
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

	CRect	cRc(0,0,0,0);
	m_view->GetClientRect(&cRc);

	if (wRc.top + size.cy + height < cRc.Height() || wRc.top - size.cy < height)
		wRc.top += height;
	else
	{
		wRc.top -= size.cy;
		wRc.top += 4;
	}
	wRc.right  = wRc.left + size.cx;
	wRc.bottom = min(wRc.top + size.cy, cRc.bottom);

	m_child   = new CHistory(this, items); 
	if (!((CHistory *)m_child)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, wRc, m_view, 223)) //test codelist
	{
		m_status &= ~(S_CHILD | S_FIND);
		delete m_child;
	}	
}

void CClient::History(bool apply)
{
	if (m_status & (S_CHILD|S_FIND))
	{
		CString	text{};

		m_status &= ~(S_CHILD|S_FIND);
		if (apply && ((CHistory *)m_child)->GetItem(text))
		{
			CScreen* screen;
			if (GetAtScreen(screen, m_cbox.m_key))
			{
				CfmBase* form = screen->GetAtForm(m_cbox.m_idx);
				if (!form) return;
				form->WriteData(text);
				form->IsChanged();
				m_vm->OnChange(screen, form);
				if (form->m_form->attr & FA_SEND)
				{
					if ((form->m_form->attr2 & SA_SEND) == SA_THIS)
						m_stream->InStream(screen);
					else
						m_stream->InStream();
				}
			}
		}
		delete m_child;
	}
}

void CClient::SelfHistory(CfmBase* form)
{
	CString	text{};

	form->ReadData(text);		// self-history mode reset
	text.TrimRight();
	if (text.IsEmpty())
	{
		History(false);
		return;
	}

	if (form->m_form->size > m_guard->m_CODEs && !(m_status & S_FIND))
	{
		int	ii;
		for (ii = 0; ii < text.GetLength(); ii++)
		{
// updateXXX_2022XX			
			if (isdigit(static_cast<unsigned char>(text[ii])))
				continue;
			if (text[ii] & 0x80 || m_guard->isCodes(text))
				break;
		}

		if (ii == text.GetLength())
			return;
	}

	m_cbox = m_current;
	History(form, RC_SEARCH);
	((CHistory *)m_child)->FindItem(text, true);
}

void CClient::AutoHistory(CfmBase* form)
{
// updateXXX_2022XX	
	if (form->m_form->kind != FM_EDIT)
		return;

	if (form->m_form->properties & PR_LIST && !(form->m_form->attr & FA_PROTECT))
	{
		CString	text{};

		form->ReadData(text);
		text.TrimRight();
		if (!text.IsEmpty())
		{
			m_cbox = m_current;
			History(form, RC_NOP);
			if (m_status & S_CHILD)
				((CHistory *)m_child)->FindItem(text, true);
			return;
		}
	}
	History(false);
}

void CClient::GetNext(CString& next)
{
	int	idx;

	while (!m_next.IsEmpty())
	{
		idx = m_next.Find('|');
		if (idx == -1)
		{
			next = m_next;
			m_next.Empty();
		}
		else
		{
			next = m_next.Left(idx++);
			m_next = m_next.Mid(idx);
		}

		next.Trim();
		if (next.GetLength() == L_MAPN)
			return;
	}

	next.Empty();
}

bool CClient::isNext(CScreen* screen, int key)
{
	CfmBase* form = screen->GetAtForm(key);
	if (!form || form->m_form->kind != FM_GRID || form->m_form->vals[2] == NOVALUE)
		return false;

	CString	text = (char *)form->m_form->vals[2];

	if (text.Find('$') || text.GetLength() <= L_MAPN)
		return false;

	switch (text.GetAt(1))
	{
	case '>':
		m_form.m_idx = -1;
		break;
	case '<':
		m_form.m_idx = key;
		break;
	default:
		return false;
	}

	CString	maps, tmps = _T("");

	if (!SetCode(screen, form, tmps, false))
		return false;

	text = text.Mid(2);
	maps = text + "|" + m_mapN;

	if (m_form.m_idx != -1)
	{
		m_form.m_idx = key;
		m_cell = form->GetCurSel();
		m_next = maps;
		m_codes = tmps;

		SetNext();
		return true;
	}

	key = maps.Find('|');
	text = maps.Left(key++);
	maps = maps.Mid(key);

	text += tmps;
	key = m_guard->GetWorks();
	if (key < 0)	return false;
	
	m_guard->SendAxis(MAKEWPARAM(renVIEW, key), (LPARAM)text.operator LPCTSTR());

	CClient* client;
	if (m_guard->GetClient(key, (CWorks*&)client))
	{
		client->m_form.m_idx = -1;
		client->m_next = maps;
		client->m_codes = tmps;
	}
	return true;
}

void CClient::SetNext()
{
	CString	text;

	GetNext(text);
	if (!isNext() && m_form.m_idx == -1)
	{
		m_guard->PostAxis(MAKEWPARAM(delVIEW, m_key), (LPARAM)0);
		return;
	}

	text += m_codes;
	m_guard->SendAxis(MAKEWPARAM(renVIEW, m_key), (LPARAM)text.operator LPCTSTR());

	if (!isNext())
	{
		CScreen* screen;

		if (GetAtScreen(screen, m_form.m_key))
		{
			CfmBase* form = screen->GetAtForm(m_form.m_idx);
			if (form)
				form->SetCurSel(m_cell);
		}
	}
}

void CClient::SetCode(CString& codes, bool link)
{
	CString	text;
	CfmBase* form;
	CScreen* screen;

	codes.Empty();
	for (int idx = 0; idx < m_magic; idx++)
	{
		if (!GetAtScreen(screen, idx))
			continue;

		for (int ii = 0; ii < screen->m_mapH->formN; ii++)
		{
			form = screen->GetAtForm(ii);
			if (form && form->m_form->properties & PR_CODE)
			{
				form->ReadData(text);
				text.TrimLeft();
				if (!text.IsEmpty())
				{
					codes += (char*)form->m_form->name;
					codes += '\t';
					codes += text;
					codes += '\n';
				}
			}
		}
	}
	GetCodes(codes);
	if (link)
		codes += m_guard->SetCode(this);
}

bool CClient::SetCode(CScreen* screen, CfmBase* form, CString& codes, bool force)
{
	int row = ((CfmGrid *)form)->GetCellRow();
	if (((CfmGrid *)form)->GetValidRows() < row)
		return false;

	int col = ((CfmGrid *)form)->GetCellColumn();
	if (--col < 0 || --row < 0)
		return false;

	if (!GetDragCell(screen, form, col, force))
		return false;

	CString	text;
	form->ReadData(text, false, -1, col, row); text.TrimLeft();
	if (text.IsEmpty()) return false;

	struct	_cellR* cell = (struct _cellR *) &screen->m_cellR[form->m_form->vals[0]];

	codes = (char *)cell[col].name;
	codes += '\t';
	codes += text;	codes += '\n';

	if (force) GetCodes(codes);

	return true;
}

bool CClient::GetDragCell(CScreen* screen, CfmBase* form, int& col, bool force)
{
	int	nCols = HIWORD(form->m_form->vals[1]);
	int	idx = col;

	struct	_cellR* cell = (struct _cellR *) &screen->m_cellR[form->m_form->vals[0]];
	for (col = 0; col < nCols; col++, cell++)
	{
		if (cell->attr & FA_DRAG)
		{
			if (!force && col != idx)
				return false;

			if (cell->attr & FA_REF && cell->vals[0] != NOVALUE)
				col = cell->vals[0];
			return true;
		}
	}
	return false;
}

void CClient::OnStart(CScreen* screen)
{
	int	parent = screen->m_key;

	m_vm->OnStart(screen);
	for (int key = 0; key < m_magic; key++)
	{
		if (GetAtScreen(screen, key) && !screen->isMain() &&
						screen->m_parent == parent)
			OnStart(screen);
	}
}

void CClient::OnSend(CScreen* screen)
{
	int	parent = screen->m_key;

	if (screen->m_mapH->mapM != MM_MENU)
		m_stream->InStream(screen);

	for (int key = 0; key < m_magic; key++)
	{
		if (GetAtScreen(screen, key) && !screen->isMain() &&
					!screen->isUob() && screen->m_parent == parent)
			OnSend(screen);
	}
}

void CClient::MakeFile(CScreen* screen, CfmBase* form, CString path, bool head, bool append)
{
	CString	text = _T("");

	switch (form->m_form->kind)
	{
	case FM_TABLE:
	case FM_GRID:
		form->ReadAll(text, true, head);
		break;
	default:
		return;
	}

	if (text.IsEmpty())
		return;

	CString	fileN;
	CFile	file;
	UINT	flag = CFile::modeCreate|CFile::typeBinary|CFile::modeWrite;
	CFileDialog fDlg(FALSE, "txt", path.IsEmpty() ? (char *)form->m_form->name : path.operator LPCTSTR(),
				OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "Text Files(*.txt)|*.txt|All Files(*.*)|*.*|");
	if (fDlg.DoModal() != IDOK)
		return;
	fileN = fDlg.GetPathName();
	if (append)
		flag |= CFile::modeNoTruncate;
	if (file.Open(fileN, flag))
	{
		if (append)
			file.SeekToEnd();
		file.Write(text, text.GetLength());
		file.Close();
	}
}

void CClient::MakeXlsFile(CScreen* screen, CfmBase* form, bool visible, CString path, bool head)
{
	CString	text;

	switch (form->m_form->kind)
	{
	case FM_GRID:
	case FM_TABLE:
		form->ReadAll(text, true, head);
		break;
	default:
		return;
	}

	if (text.IsEmpty())
		return;

	CString	fileN;

	if (path.IsEmpty())
	{
		if (visible)
			fileN.Format(_T("EX%s(%s)"), LPCTSTR(CString(&screen->m_mapH->mapN[L_SGID], L_SELC)), (char *)form->m_form->name);
		else
			fileN.Empty();
		m_guard->MakeXlsFile(GetFormat(screen, form), text, visible, fileN);
	}
	else
		m_guard->MakeXlsFile(GetFormat(screen, form), text, visible, path, true);
}

void CClient::MakeXlsFileEx(CScreen* screen, CfmBase* form, bool head)
{
	if (form->m_form->kind != FM_GRID)
		return;

	CString	text;

	form->ReadAll(text, true, head);
	if (text.IsEmpty())
		return;

	m_guard->MakeXlsFile(GetFormat(screen, form), text);
}

void CClient::MakeXlsFile2(CScreen* screen, CfmBase* form, bool visible, CString path, bool head)
{
	if (form->m_form->kind != FM_GRID)
		return;

	CString	text;
	int	value = head ? 1 : 0;

	form->GetEnum(text, value, 1);
	if (text.IsEmpty())
		return;

	CString	fileN;

	if (path.IsEmpty())
	{
		if (visible)
			fileN.Format(_T("EX%s(%s)"), LPCTSTR(CString(&screen->m_mapH->mapN[L_SGID], L_SELC)), (char *)form->m_form->name);
		else
			fileN.Empty();
		m_guard->MakeXlsFile(text, visible, fileN);
	}
	else
		m_guard->MakeXlsFile(text, visible, path, true);
}

void CClient::Export(CScreen* screen, CfmBase* form, bool visible, CString data)
{
	int	offs = 1;
	CString	text, path, string, ext{};
	
	text.Empty();
	form->ReadAll(text, true, true);
	if (text.IsEmpty())
		return;

	offs = data.Find(FS);
	if (offs != -1)
	{
		path = data.Left(offs++);
		data = data.Mid(offs);
		offs = data.Find(FS);
		if (offs != -1)
		{
			string = data.Left(offs++);
			data = data.Mid(offs);
			data += '\n';
		}
		else
		{
			string = data;
			data.Empty();
		}
	}
	else
	{
		path = data;
		string.Empty();
		data.Empty();
	}
	text.Insert(0, string);		// top string
	text += data;			// tail string

	path.MakeLower();
	if (path.Find(".xls") != -1)
	{
		ext = "xls";
		string = _T("Worksheet Files (*.xls)|*.xls|Text Files (*.txt)|*.txt|CSV Files (*.csv)|*.csv|");
	}
	else if (path.Find(".csv") != -1)
	{
		ext = "csv";
		string = _T("CSV Files (*.csv)|*.csv|Worksheet Files (*.xls)|*.xls|Text Files (*.txt)|*.txt|");
	}
	else
	{
		ext = "txt";
		string = _T("Text Files (*.txt)|*.txt|Worksheet Files (*.xls)|*.xls|CSV Files (*.csv)|*.csv|");
	}

	path = m_guard->GetFileName(path, string, ext);
	if (path.IsEmpty())
		return;

	if (!ext.CompareNoCase("xls"))
	{
		m_guard->MakeXlsFile(GetFormat(screen, form), text, visible, path, true);
		return;
	}
// updateXXX_2022XX	
	if (!ext.CompareNoCase("csv"))
		text.Replace(_T('\t'), _T(','));

	CFile	file;
	if (file.Open(path, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite))
	{
		file.Write(text, text.GetLength());
		file.Close();
		if (visible)
			ShellExecute(NULL, "open", path, NULL, NULL, SW_SHOW);
	}
}

void CClient::OnPrint(CScreen* screen, CfmBase* form, bool print)
{
	CString	text, tmps;

	text.Empty();
	switch (form->m_form->kind)
	{
	case FM_MEMO:
		form->ReadAll(text);
		break;
	case FM_GRID:
		if (form->m_form->vals[0] == NOVALUE)
			return;

		CPtrArray*	grid;
		struct _cellR	*cell;
		int	idx, space, rowN, nCols;

		rowN = ((CfmGrid *)form)->GetValidRows();
		nCols = HIWORD(form->m_form->vals[1]);

		if (form->m_form->attr & (FA_ENUM|FA_DOMINO) && screen->m_cells.Lookup((char *)form->m_form->name, (CObject *&)grid))
			nCols = grid->GetSize();
		else
			grid = NULL;

		for (idx = -2; idx < rowN; idx++)
		{
			cell = (struct _cellR *) &screen->m_cellR[form->m_form->vals[0]];
			for (int kk = 0; kk < nCols; kk++, cell++)
			{
				if (grid != NULL)
					cell = (struct _cellR *)grid->GetAt(kk);
				if (cell->properties & PR_VISIBLE)
				{
					form->ReadData(tmps, true, -1, kk, idx);
					text += tmps;

					space = idx < 0 ? max(0, cell->size+4 - tmps.GetLength()) : 4;
					text += CString(' ', space);
				}
			}
			text.TrimRight(' ');
			text += '\n';
			if (idx < 0)	idx++;
		}
		break;
	default:
		return;
	}

	if (m_string.IsEmpty() && text.IsEmpty())
		return;

	if (!print)
	{
		CFileDialog fDlg(FALSE, "txt", (char *)form->m_form->name, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
								_T("Text Files(*.txt)|*.txt|All Files(*.*)|*.*|"));
		CFile	file;
		if (fDlg.DoModal() == IDOK &&
				file.Open(fDlg.GetPathName(), CFile::modeCreate|CFile::typeBinary|CFile::modeWrite))
		{
			file.Write(m_string, m_string.GetLength());
			file.Write(text, text.GetLength());
			file.Close();
		}
		return;
	}
	m_guard->OnPrint(m_string + text, (char *)form->m_form->name, (char *)form->m_form->fName, form->m_form->fPoint);
}

bool CClient::SetCaption(CString maps)
{
	CScreen* screen;

	for (int ii = 0; ii < m_magic; ii++)
	{
		if (GetAtScreen(screen, ii) && !maps.CompareNoCase(CString(screen->m_mapH->mapN, L_MAPN)))
		{
			m_guard->SendAxis(MAKEWPARAM(titleVIEW, m_key), screen->m_mapH->caption);
			return true;
		}
	}
	return false;
}

bool CClient::FindScreen(int parent, CString& name, CScreen*& screen, bool only)
{
	switch (parent)
	{
	case dpMAIN:
		if (!GetAtScreen(screen))
			return false;
		[[fallthrough]];
	case dpGREAT:
		if (!GetAtScreen(screen, screen->m_parent))
			return false;
		[[fallthrough]];
	case dpGRAND:
		if (!GetAtScreen(screen, screen->m_parent))
			return false;
		[[fallthrough]];
	case dpPARENT:
		if (!GetAtScreen(screen, screen->m_parent))
			return false;
		break;
	case dpCURRENT:
	default:
		break;
	}

	CfmBase* form;
	CString	tmps = name;

	while (true)
	{
		tmps = m_guard->GetByString(name, '.');
		if (name.IsEmpty())
		{
			name = tmps;
			if (only)
			{
				if (!screen->FindForm(tmps, form))
					return false;
				if (!GetAtScreen(screen, form->m_form->keys))
					return false;
			}
			return true;
		}
		if (!tmps.CompareNoCase("main"))
			return GetAtScreen(screen) ? true : false;

		if (!screen->FindForm(tmps, form))
			return false;
		if (!GetAtScreen(screen, form->m_form->keys))
			return false;
	}

	name = tmps;
	return true;
}

CCmdTarget* CClient::GetControl(CString name)
{
	CpubControl* control;

	if (m_control.Lookup(name, (void*&)control))
		return control->m_target;

	CString	path;
	
	path.Format(_T("%s/%s/%s.dll"), m_guard->m_root.GetString(), DEVDIR, name.GetString());
LOG_OUTP(3, "[LOADLIB", path, __FUNCTION__);
	HINSTANCE hinst = AfxLoadLibrary(path); 
	if (hinst)
	{
		CCmdTarget*	target;
		CCmdTarget*	(APIENTRY* axCreate)(void);

		axCreate = (CCmdTarget* (APIENTRY*)(void))GetProcAddress(hinst, _T("axCreate"));
		if (axCreate != NULL)
		{
			target = (*axCreate)();
			if (target)
			{
				control = new CpubControl;
				control->m_dll    = hinst;
				control->m_target = target;
				m_control.SetAt(name, control);
				return target;
			}
		}
		AfxFreeLibrary(hinst);
	}

	return NULL;
}

bool CClient::isChild(int parent, CScreen* screen)
{
	if (screen->isMain())
		return false;

	if (screen->m_parent == parent)
		return true;

	if (GetAtScreen(screen, screen->m_parent))
		return isChild(parent, screen);
	return false;
}

void CClient::SetFCB(bool blink, int flash)
{
	int	key;
	CScreen* screen;

	for (POSITION pos = m_screen.GetStartPosition(); pos; )
	{
		m_screen.GetNextAssoc(pos, key, screen);
		screen->SetFCB(blink, flash);
	}
}

void CClient::SetControl(int key, CString text)
{
	CScreen* screen;

	if (key != 0xffff)
	{
		if (GetAtScreen(screen, key))
			screen->SetControl(text);
		return;
	}

	for (POSITION pos = m_screen.GetStartPosition(); pos; )
	{
		m_screen.GetNextAssoc(pos, key, screen);
		screen->SetControl(text);
	}
}

BOOL CClient::SetFont(int point, bool resize, bool attach)
{
	CScreen* screen;
	float	hR, vR;

	if (resize && (m_status & S_FIX) || (!attach && m_font == point))
		return TRUE;

	hR = vR = 1.0;
	if (resize && GetAtScreen(screen))
	{
		CDC*	dc;
		CRect	rect;
		CString	fonts;
		float	hRx, vRx;
		int	ndc, width, height, cx, cy;
		TEXTMETRIC	tm;

		dc   = m_view->GetDC();
		ndc  = dc->SaveDC();
		fonts = (char *)screen->m_mapH->fName;
		m_guard->m_draw->SetFont(dc, (char *)fonts.operator LPCTSTR(), point, false, FW_NORMAL);
		dc->GetOutputTextMetrics(&tm);
		width  = tm.tmAveCharWidth;
		height = tm.tmHeight;

		m_guard->m_draw->SetFont(dc, (char *)fonts.operator LPCTSTR(), screen->m_mapH->fPoint, false, FW_NORMAL);
		dc->GetOutputTextMetrics(&tm);

		hR = (float)width  / (float)tm.tmAveCharWidth;
		vR = (float)height / (float)tm.tmHeight;

		dc->RestoreDC(ndc);
		m_view->ReleaseDC(dc);

		width  = (int)(screen->m_mapH->width * hR);
		height = (int)(screen->m_mapH->height * vR);

		if (!(m_type & vtypeMSK))	// normal view
			hRx = vRx = 1.0;
		else
		{
			m_view->GetClientRect(&rect);
			if (attach || rect.Width() <= GetSystemMetrics(SM_CXSMICON) || rect.Height() <= GetSystemMetrics(SM_CYCAPTION))
				rect.SetRect(0, 0, screen->m_mapH->width, screen->m_mapH->height);

			hRx = (float)rect.Width()  / (float)screen->m_mapH->width;
			vRx = (float)rect.Height() / (float)screen->m_mapH->height;
			hRx /= m_hR;
			vRx /= m_vR;
		}
		cx = (int)(width * hRx + 0.9);
		cy = (int)(height * vRx + 0.9);

		if (cx >= GetSystemMetrics(SM_CXSCREEN) || cy >= GetSystemMetrics(SM_CYSCREEN))
			return FALSE;			// 20081117
		else
		{
			m_rect.left   = 0;
			m_rect.right  = width;
			m_rect.top    = 0;
			m_rect.bottom = height;
			m_guard->SendAxis(sizeVIEW, m_key, MAKEWPARAM(cx, cy));
		}
	}

	m_font = point;
	m_hR = hR; m_vR = vR;
/*	for (POSITION pos = m_screen.GetStartPosition(); pos; )
	{
		m_screen.GetNextAssoc(pos, key, screen);
		if (resize)
			screen->ResizeForm(m_hR, m_vR, true);
		screen->SetFont(resize, point);
	}
//	modify for loop as below
*/
	for (int ii = 0; ii < m_magic; ii++)
	{
		if (!GetAtScreen(screen, ii))
			continue;
		if (resize)
			screen->ResizeForm(m_hR, m_vR, true);
		screen->SetFont(resize, point);
	}

	return TRUE;
}

void CClient::OnDevice()
{
	int	key;
	CScreen* screen;

	for (POSITION pos = m_screen.GetStartPosition(); pos; )
	{
		m_screen.GetNextAssoc(pos, key, screen);
		m_vm->OnDevice(screen);
	}
}

BOOL CClient::OnKey(int key)
{
	CString	param;
	CScreen* screen;

	param.Format("%d", key);
	m_guard->m_xsystem->m_userKey = FALSE;
	if (GetScreen(screen))
		m_vm->Procedure(screen, _T("OnAxUserKey"), param);
	return m_guard->m_xsystem->m_userKey;
}

void CClient::SetLedger(CString maps)
{
	int	key;
	CScreen* screen;

	for (POSITION pos = m_screen.GetStartPosition(); pos; )
	{
		m_screen.GetNextAssoc(pos, key, screen);
		if (screen->m_mapH->typeH == TH_LEDGER)
			m_guard->SetLedger(screen->m_ledger, maps);
	}
}

bool CClient::GetCode(CMapStringToString& domino)
{
	int	key;
	CScreen* screen;
	CfmBase* form;
	CString	text;

	for (POSITION pos = m_screen.GetStartPosition(); pos; )
	{
		m_screen.GetNextAssoc(pos, key, screen);
		for (int ii = 0; ii < screen->m_mapH->formN; ii++)
		{
			form = screen->GetAtForm(ii);
			if (form->m_form->properties & PR_CODE)
			{
				form->ReadData(text);
				text.TrimRight();
				if (!text.IsEmpty())
					domino.SetAt((char*)form->m_form->name, text);
			}
		}
	}
	return (domino.GetCount() > 0);
}

int CClient::GetCodes(CString syms, CString& items, bool self)
{
	char	wb[128];
	CString	path, string;
	int	count = 0;

	items = _T("");
	path.Format(_T("%s\\%s\\axis.ini"), LPCTSTR(m_guard->m_root), TABDIR);
	count = GetPrivateProfileString(SAVECODE, syms, "", wb, sizeof(wb), path);
	if (count > 0)
	{
		int	pos;
		CString	text;

		syms  = CString(wb, count);
		count = 0;
		for ( ; !syms.IsEmpty(); )
		{
			pos = syms.Find(',');
			if (pos != -1)
			{
				string = syms.Left(pos);
				syms   = syms.Mid(pos+1);
			}
			else
			{
				string = syms;
				syms   = _T("");
			}
			count += m_guard->GetCodes(string, text, self);
			items += text;
		}
	}
	else
		count = m_guard->GetCodes(syms, items, self);

	return count;
}

void CClient::GetCodes(CString& codes)
{
	char	wb[128];
	int	idx;
	CString	path, string;
	CString	syms, text;
	CMapStringToString domino;

	domino.RemoveAll();
	path.Format(_T("%s\\%s\\axis.ini"), LPCTSTR(m_guard->m_root), TABDIR);
	for ( ; !codes.IsEmpty(); )
	{
		idx = codes.Find('\n');
		if (idx == -1)
		{
			string = codes;
			codes.Empty();
		}
		else
		{
			string = codes.Left(idx++);
			codes  = codes.Mid(idx);
		}

		idx = string.Find('\t');
		if (idx == -1)
			continue;

		text   = string.Left(idx++);
		string = string.Mid(idx);
		if (!string.FindOneOf("aA"))
			string.Delete(0, 1);
		GetPrivateProfileString("Link", text, text, wb, sizeof(wb), path);

		text = wb;
		for ( ; !text.IsEmpty(); )
		{
			idx = text.Find(',');
			if (idx != -1)
			{
				syms = text.Left(idx++);
				text = text.Mid(idx);
			}
			else
			{
				syms = text;
				text.Empty();
			}
			syms.TrimLeft();
			syms.TrimRight();
			if (!syms.IsEmpty())
				domino.SetAt(syms, string);
		}
	}

	codes.Empty();
	for (POSITION pos = domino.GetStartPosition(); pos; )
	{
		domino.GetNextAssoc(pos, syms, text);
		codes += syms;
		codes += '\t';
		codes += text;
		codes += '\n';
	}
}

void CClient::AddTips()
{
	int	key;
	CScreen* screen;

	m_tips.RemoveAll();
	for (int ii = 0; ii < m_magic; ii++)
	{
		if (!GetAtScreen(screen, ii))
			continue;
		
		for (int kk = 0; kk < screen->m_mapH->formN; kk++)
		{
			if (screen->m_formR[kk].isHint != IH_NOP && screen->m_formR[kk].onHint != NOVALUE)
			{
				switch (screen->m_formR[kk].kind)
				{
				case FM_GRID:
				case FM_TABLE:
					key = MAKELONG(screen->m_mapH->formN+kk, MAKEWORD(screen->m_key, m_key));
					break;
				default:
					key = MAKELONG(kk, MAKEWORD(screen->m_key, m_key));
					break;
				}
				m_tips.SetAt(key, screen->GetAtForm(kk));
			}
		}
	}
}

void CClient::RemoveTips(CScreen* screen)
{
	int	key;

	if (screen->m_mapH == NULL)
		return;

	for (int ii = 0; ii < screen->m_mapH->formN; ii++)
	{
		if (screen->m_formR[ii].isHint != IH_NOP && screen->m_formR[ii].onHint != NOVALUE)
		{
			switch (screen->m_formR[ii].kind)
			{
			case FM_GRID:
			case FM_TABLE:
				key = MAKELONG(screen->m_mapH->formN+ii, MAKEWORD(screen->m_key, m_key));
				break;
			default:
				key = MAKELONG(ii, MAKEWORD(screen->m_key, m_key));
				break;
			}
			m_tips.RemoveKey(key);
		}
	}
}

int CClient::LookTips(CPoint point, CString& text)
{
	CScreen *screen;
	CfmBase *form, *object;
	int	key, idx;

	screen = NULL;
	text = _T("");
	point += m_offset;

	for (POSITION pos = m_tips.GetStartPosition(); pos; )
	{
		m_tips.GetNextAssoc(pos, key, form);
		if (!GetAtScreen(screen, LOBYTE(HIWORD(key))))
			continue;
		if (!screen->isMain())
		{
			GetKeyObject(screen, object);
			if (!isVisible(screen, object))
				continue;
		}
		if (!screen->IsFormVisible(form))
			continue;

		switch (idx = form->IsPointInRect(point))
		{
		case P_OUT:	// OUT
			break;
		case P_IN:	// IN
			if (form->m_form->kind != FM_GRID && form->m_form->kind != FM_TABLE)
			{
				text = (char*)form->m_form->onHint;
				if (form->m_form->isHint == IH_REF)
				{
					if (screen && screen->FindForm(text, form))
						form->ReadData(text, true);
				}
				return key;
			}
			return -1;
		default:	// GRID, TABLE
			switch (form->m_form->kind)
			{
			case FM_GRID:
				form->ReadData(text, false, -1, MAKELONG(idx-1, point.y), -3);
				break;
			case FM_TABLE:
			//	form->ReadData(text, false, -1, -3, idx-1);
				break;
			}
			key += point.y/3;
			return key;
		}
	}

	return -1;
}

void CClient::OnAccount()
{
	CfmBase* form;
	CScreen* screen;
	CString	text;

	for (int idx = 0; idx < m_magic; idx++)
	{
		if (!GetAtScreen(screen, idx))
			continue;

		for (int ii = 0; ii < screen->m_mapH->formN; ii++)
		{
			form = screen->GetAtForm(ii);
			if (!form) continue;

			switch (form->m_form->kind)
			{
			case FM_COMBO:
				if (form->m_form->properties & PR_ACCN)
				{
					if (!(m_guard->m_term & flagACN) && m_guard->GetCombo((char *)form->m_form->name, text))
						form->WriteAll(text, true);
				}
				if (form->IsChanged())
					m_vm->OnChange(screen, form);
				break;

			case FM_CONTROL:
				form->SetEnum("", 0);
				break;

			default:
				continue;
			}
		}
	}
}

void CClient::EditColumns(CScreen* screen, CfmBase* form)
{
	if (form->m_form->vals[0] != NOVALUE)
	{
		int	index, idx;
		CString	text, defs, tmps;
		char	twb[256]{};

		text.Format(_T("%s\\%s\\grid.ini"), m_guard->m_root.GetString(), TABDIR);
		defs.Format(_T("%s$%s"), LPCTSTR(CString(screen->m_mapH->mapN, L_MAPN)), (char *)form->m_form->name);
		GetPrivateProfileString("GRID", defs, "", twb, sizeof(twb), text);
		defs = twb;

		text.Format(_T("%s$%s"), LPCTSTR(CString(screen->m_mapH->mapN, L_MAPN)), (char *)form->m_form->name);
		GetPrivateProfileString("GRID", text, "", twb, sizeof(twb), m_guard->m_modals);
		tmps = twb;
		tmps.Trim();
		index = tmps.GetLength();
		if (index == 0)
		{
			tmps  = defs;
			index = tmps.GetLength();
		}

		text.Empty();
		struct _cellR*	cell = (struct _cellR *) &screen->m_cellR[form->m_form->vals[0]];
		int nCols = HIWORD(form->m_form->vals[1]);

		for (idx = form->m_form->type; idx < nCols; idx++)
		{
			text += (char *)cell[idx].name;
			text += '|';
			text += (cell[idx].head  == NOVALUE) ? "HEAD ?" : (char *)cell[idx].head;
			text += '|';

			if (cell[idx].properties & PR_VISIBLE && index == 0)
			{
				tmps += (char *)cell[idx].name;
				tmps += '|';
			}
		}

		if (defs.IsEmpty())
		{
			for (idx = form->m_form->type; idx < nCols; idx++)
			{
				if (!(cell[idx].properties & PR_VISIBLE))
					continue;

				defs += (char *)cell[idx].name;
				defs += '|';
			}
		}

		if (m_guard->Modal(columnED, text, tmps, defs))
		{
			text.Format(_T("%s$%s"), LPCTSTR(CString(screen->m_mapH->mapN, L_MAPN)), (char *)form->m_form->name);
			WritePrivateProfileString(_T("GRID"), text, tmps, m_guard->m_modals);
			screen->SetCells(form, tmps);
			m_stream->InStream(screen);
		}
	}
}

void CClient::EditUTab(CScreen* screen, CfmBase* form)
{
	int	ii, index;
	CString	text, defs, currents, tmps{};
	char	twb[256];

	text.Format(_T("%s$%s"), LPCTSTR(CString(screen->m_mapH->mapN, L_MAPN)), (char *)form->m_form->name);
	GetPrivateProfileString("UTAB", text, "", twb, sizeof(twb), m_guard->m_modals);
	currents = twb; currents.Trim();
	index = currents.GetLength();

	text.Empty();
	char*	map	= &screen->m_strR[form->m_form->vals[0]];
	char*	caption	= &screen->m_strR[form->m_form->vals[1]];
	for (ii = 0; ii < (int)form->m_form->keys; ii++)
	{
		tmps = map;
		text += tmps.Mid(2);	text += '|';
		text += caption;	text += '|';

		map += strlen(map); map++;
		caption += strlen(caption); caption++;

		if (index == 0 && tmps.GetAt(0) == '1')
		{
			currents += tmps.Mid(2);
			currents += '|';
		}
	}

	defs.Empty();
	map	= &screen->m_strR[form->m_form->vals[0]];
	for (ii = 0; ii < (int)form->m_form->keys; ii++)
	{
		tmps = map;
		if (tmps.GetAt(0) == '1')
		{
			defs += tmps.Mid(2);
			defs += '|';
		}
		map += strlen(map); map++;
	}

	if (m_guard->Modal(tabED, text, currents, defs))
	{
		text.Format(_T("%s$%s"), LPCTSTR(CString(screen->m_mapH->mapN, L_MAPN)), (char *)form->m_form->name);
		WritePrivateProfileString("UTAB", text, currents, m_guard->m_modals);
		form->SetEnum(currents, 0);
		if (m_vm->IsAvailable(screen, evClick, form))
			m_vm->OnClick(screen, form);
	}
}

int CClient::GetKey(int skey, int ukey, int index)
{
	int	key, keyx;

	key = MAKELONG(MAKEWORD(skey, ukey), index);

m_slog.Format("[mapkey] skey=[%d] ukey=[%d] index=[%d] key=[%d] ", skey, ukey, index, key);
LOG_OUTP(3, "axwizard", __FUNCTION__, m_slog);

	for (int ii = 0xfe; ii >= 0; ii--)
	{
		if (m_keys.Lookup(ii, keyx))
		{
			if (key == keyx)
			{
m_slog.Format("[mapkey] key == keyx  cnt=[%d]  key=[%d]   ii=[%d]", m_keys.GetCount(), key, ii);
LOG_OUTP(3, "axwizard", __FUNCTION__, m_slog);
				return ii;
			}
			continue;
		}
		else
		{
			m_keys.SetAt(ii, key);
m_slog.Format("[mapkey]   cnt=[%d]  key=[%d]  ii=[%d]", m_keys.GetCount(), key, ii);
LOG_OUTP(3, "axwizard", __FUNCTION__, m_slog);
			return ii;
		}
	}

	return -1;
}

int CClient::GetScreenKey(int key, int& index, int& ukey)
{
	int	skey;

	if (m_keys.Lookup(key, skey))
	{
		index = HIWORD(skey);
		ukey  = HIBYTE(LOWORD(skey));
		skey  = LOBYTE(LOWORD(skey));
	}
	else
		skey = -1;

	return skey;
}

CString CClient::GetFormat(CScreen* screen, CfmBase* form)
{
	if (form->m_form->kind != FM_GRID)
		return _T("");

	CString	format = _T("");
	CPtrArray*	grid;
	struct _cellR	*cell;

	int nCols = HIWORD(form->m_form->vals[1]);
	if (form->m_form->attr & (FA_ENUM|FA_DOMINO) && screen->m_cells.Lookup((char *)form->m_form->name, (CObject *&)grid))
		nCols = grid->GetSize();
	else
		grid = NULL;

	cell = (struct _cellR *) &screen->m_cellR[form->m_form->vals[0]];
	for (int kk = 0; kk < nCols; kk++, cell++)
	{
		if (grid != NULL)
			cell = (struct _cellR *)grid->GetAt(kk);
		if (grid || (!grid && form->IsVisible(kk)))
		{
			switch (cell->type)
			{
			case IO_NUM:
			case IO_DIGIT:
				format += ' ';
				break;
			case IO_ALPHA:
			case IO_HAN:
			case IO_MIXED:
			case IO_ANY:
			default:
				format += 'T';
				break;
			}
		}
	}
	return format;
}

bool CClient::isPurge()
{
	CScreen *screen;

	for (int idx = 0; idx < m_magic; idx++)
	{
		if (GetAtScreen(screen, idx))
		{
			if (screen->isPurge())
				return true;
		}
	}
	return false;
}

void CClient::SetTabStop(int key, BOOL tabstop, CString forms)
{
	CScreen* screen;

	if (GetAtScreen(screen, key))
	{
		CfmBase* form;
		if (screen->FindForm(forms, form))
		{
			if (tabstop)
				form->m_form->attr &= ~FA_PASS;
			else
				form->m_form->attr |= FA_PASS;
		}
	}
}

void CClient::SetControlRect(void* param)
{
	CScreen* screen;

	struct	_rectR* rectR = (struct _rectR *)param;
	if (GetAtScreen(screen, rectR->key))
	{
		CfmBase* form;
		for (int ii = 0; ii < screen->m_mapH->formN; ii++)
		{
			form = screen->GetAtForm(ii);
			if (!form || form->m_form->kind != FM_CONTROL)
				continue;

			if ((int)((CfmCtrl *)form)->m_ctrl == rectR->ctrl)
			{
				form->SetRect(CRect(rectR->left, rectR->top, rectR->right, rectR->bottom), CRect(0,0,0,0));
				break;
			}
		}
	}
}

CString CClient::GetSaveData()
{
	int	key;
	CfmBase* form;
	CScreen* screen;
	CString	text, string, tmps;

	string.Empty();
	for (POSITION pos = m_screen.GetStartPosition(); pos; )
	{
		m_screen.GetNextAssoc(pos, key, screen);
		text.Empty();
		for (int ii = 0; ii < screen->m_mapH->formN; ii++)
		{
			form = screen->GetAtForm(ii);
			if (form && form->m_form->attr & FA_SAVE)
			{
				form->ReadData(tmps);
				text += form->GetSymbolName();
				text += (TCHAR)FS;
				text += tmps;
				text += '|';
			}
		}
		if (!text.IsEmpty())
		{
			tmps.Format(_T("%s=%s\n"), LPCTSTR(CString(screen->m_mapH->mapN, L_MAPN)), text.GetString());
			string += tmps;
		}
	}
	return string;
}

void CClient::SetSaveData(CString data)
{
	CString	text, string;
	CString	maps, syms;
	CMapStringToOb	obs;
	CMapStringToString* arr = nullptr;

	for ( ; !data.IsEmpty(); )
	{
		text = getString('\n', data);
		maps = getString('=', text);
		if (maps.IsEmpty())
			continue;

		arr = new CMapStringToString;
		arr->RemoveAll();
		for ( ; !text.IsEmpty(); )
		{
			string = getString('|', text);
			syms = getString(FS, string);
			arr->SetAt(syms, string);
		}
		obs.SetAt(maps, arr);
	}

	CfmBase* form;
	CScreen* screen;
	for (int ii = 0; ii < m_magic; ii++)
	{
		if (GetAtScreen(screen, ii) && obs.Lookup(CString(screen->m_mapH->mapN, L_MAPN), (CObject *&)arr))
		{
			for (POSITION pos = arr->GetStartPosition(); pos; )
			{
				arr->GetNextAssoc(pos, syms, text);
				if (screen->FindForm(syms, form))
				{
					form->WriteData(text);
					form->IsChanged();
					m_vm->OnChange(screen, form);
				}
			}
		}
	}

	for (POSITION pos = obs.GetStartPosition(); pos; )
	{
		obs.GetNextAssoc(pos, maps, (CObject *&)arr);
		arr->RemoveAll();
		delete arr;
	}
	obs.RemoveAll();
}

CString	CClient::getString(char ch, CString& string)
{
	CString	text{};

	int idx = string.Find(ch);
	if (idx != -1)
	{
		text = string.Left(idx);
		string = string.Mid(++idx);
	}
	else
	{
		text = string;
		string.Empty();
	}

	return text;
}

BOOL CClient::SetFocus(int key, bool action, CWnd* pWnd)
{
	CScreen* screen;
	CfmBase* form;

	if (GetAtScreen(screen, key))
	{
		for (int ii = 0; ii < screen->m_mapH->formN; ii++)
		{
			form = screen->GetAtForm(ii);
			if (!form || form->m_form->kind != FM_CONTROL)
				continue;

			if (((CfmCtrl *)form)->m_ctrl == pWnd)
			{
				CCaret	caret = m_current;

				m_current.m_key = key;
				m_current.m_idx = ii;
				if ((action || m_focus) && (caret.m_key != -1 && caret.m_idx != -1)
					&& (caret.m_key != key || caret.m_idx != ii))
				{
					CScreen* active;
					if (GetAtScreen(active, caret.m_key))
						active->SetForm(caret.m_idx, false);
				}

				m_mouse->m_screen = screen;
				m_mouse->m_mouse  = m_current;
				if (action)
				{
					if (!m_focus)
					{
						m_focus = TRUE;
						m_guard->ClearFocus(m_key);
					}
					screen->SetForm(m_current.m_idx, true);
				}
				return m_focus;
			}
		}
	}

	return FALSE;
}

bool CClient::ControlAtPt(CScreen* screen, CfmBase* fmExcept, CPoint pt)
{
	CfmBase* form;
	CfmCtrl* ctrl;
	CScreen* obj;

	for (int ii = screen->m_mapH->formN-1; ii >= 0; ii--)
	{
		form = screen->GetAtForm(ii);

		if (form == NULL || !screen->IsFormVisible(form) || !form->IsPointInRect(pt))
			continue;

		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			if (GetAtScreen(obj, form->m_form->keys) && ControlAtPt(obj, fmExcept, pt))
				return true;
			break;

		case FM_CONTROL:
			ctrl = (CfmCtrl*)form; 
			if (!ctrl->IsVisible() || !m_mouse->IsEnable(ctrl) || 
			    !ctrl->IsFocus(ctrl->m_ctrl->GetSafeHwnd()) || fmExcept == ctrl)
				break;
			else
				return true;

		default:
			break;
		}
	}

	return false;
}

CfmCtrl* CClient::FindControl(CScreen* screen, HWND hWnd)
{
	CfmBase* form;
	CfmCtrl* ctrl;
	CScreen* obj;

	for (int ii = screen->m_mapH->formN-1; ii >= 0; ii--)
	{
		form = screen->GetAtForm(ii);

		if (form == NULL || !screen->IsFormVisible(form))
			continue;

		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			if (GetAtScreen(obj, form->m_form->keys))
			{
				ctrl = FindControl(obj, hWnd);
				if (ctrl != NULL)
					return ctrl;
			}
			break;

		case FM_CONTROL:
			ctrl = (CfmCtrl*)form;
			if (ctrl->m_ctrl->GetSafeHwnd() == hWnd)
				return ctrl;
		default:
			break;
		}
	}

	return NULL;
}

void CClient::OnNotifyMCI(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case MCI_NOTIFY_SUCCESSFUL:
		if (m_status & S_AVI)
			PlayAVI(false);
		break;
	case MCI_NOTIFY_SUPERSEDED:
	case MCI_NOTIFY_ABORTED:
	case MCI_NOTIFY_FAILURE:
		break;
	}
	return;
}

void CClient::PlayAVI(bool start)
{
	CScreen* screen;

	if (start)	StopAVI();
	for (int idx = 0; idx < m_magic; idx++)
	{
		if (!GetAtScreen(screen, idx))
			continue;
		if (screen->PlayAVI(!start))
		{
			m_status |= S_AVI;
			break;
		}
	}
}

void CClient::StopAVI()
{
	if (!(m_status & S_AVI))
		return;

	CScreen* screen;
	for (int idx = 0; idx < m_magic; idx++)
	{
		if (!GetAtScreen(screen, idx))
			continue;
		screen->StopAVI();
		m_status &= ~S_AVI;
	}
}

void CClient::MeasureItem(LPMEASUREITEMSTRUCT lp, bool history)
{
	if (history)
	{
		if (m_child)
			((CHistory *)m_child)->MeasureItemX(lp);
		return;
	}

	CScreen* screen;
	if (GetScreen(screen) && m_current.m_idx != -1)
	{
		CString	text;

		CfmBase* form = screen->GetAtForm(m_current.m_idx);
		if (form)
			form->SetEnum(text, (int)lp, 55);
	}
}

void CClient::DrawItem(LPDRAWITEMSTRUCT lp, bool history)
{
	if (history)
	{
		if (m_child)
			((CHistory *)m_child)->DrawItemX(lp);
		return;
	}

	CScreen* screen;
	if (GetScreen(screen) && m_current.m_idx != -1)
	{
		CString	text;

		CfmBase* form = screen->GetAtForm(m_current.m_idx);
		if (form)
			form->SetEnum(text, (int)lp, 56);
	}
}

void CClient::SetTimer(UINT id, UINT elapse)
{
	/*
	if (!m_OnTimer)
	{
		m_OnTimer = (COnTimer *)AfxBeginThread(RUNTIME_CLASS(COnTimer), THREAD_PRIORITY_NORMAL, 0, 0);
		m_OnTimer->Startup(m_view);
		m_OnTimer->SetEvent();
	}*/

	m_view->KillTimer(id);
	m_view->SetTimer(id, elapse, NULL);
}

//******************************************************[23/01/17][kjs] 함수추가
void CClient::sendInterGroup(CScreen* screen, std::vector<std::pair<int, CString>>& arr)
{
	struct	_uinfo {
		char	gubn[2];	// 업무구분
					// "MY" : 관심종목
					// "UL" : 사용자 데이터 목록 
					// "UD" : 사용자 데이터
		char	dirt[1];	// 전송방향 'U' : pc==>host, 'D' : host==>pc, 'X' : host==>pc (conversion), 'R' : 재조회
		char	cont[1];	// 연속구분 'F':First, 'M':Middle, 'L':First&Last, 'C':Cancel
		char	name[80];	// file name
		char	nblc[5];	// 블럭개수, 파일크기(max:64k)
		char	retc[1];	// return value 'O':정상 'E':에러
		char	emsg[40];	// error message
	};

	struct	_ginfo {
		char	gnox[2];	// group number
		char	gnam[20];	// group name
		char	jrec[4];	// _jinfo count
	};

	struct	_updn {
		_uinfo	uinfo;
		_ginfo	ginfo;
	};

	struct  _glistex {
		char    ngrs[2];           /* New Group Seqn       */
		char    ogrs[2];           /* Old Group Seqn       */
		char	gnam[30];
	};

	struct  _gfoldex {
		char	nrec[4];	// Group Count
		struct	_glistex glist[200];
	};

	const int sz_uinfo = sizeof(struct _uinfo);

	CString data{};

	
	if (1)
	{
		
		struct	_ledgerH {
			char	size[6]{};	// 0	전문길이		I	S	X	전문전체(Size-6)
			char	type[1]{};	// 6	전문구분		I	S		B:계정계, I:정보계, F:파일송수신, N:Notice 자동갱신(R:정보계, D:계정계)
			char	encf[2]{};	// 7	압축구분		I/O	S	X	전문압축/암호화구분(00:압축,암호화 안함)
			char	dptr[3]{};	// 9	데이터 옵셋		I/O	S		입/출력 데이타가 시작되는 위치(공통헤더를 포함)
			char	svcd[8]{};	// 12	서비스코드		I	Cf	●	서비스코드               ♥ TxHead(3) of Map
			char	cono[3]{};	// 20	회사번호		I	S		회사번호 "991"
			char	usid[16]{};	// 23	사용자ID		I	C		트랜잭션을 발생시킨 사용자 고유정보 (예:사용자 접속ID)
			char	cogb[2]{};	// 39	접속분류코드		I	S		접속 접속매체 (HT : 자체HTS)
			char	fep1[12]{};	// 41	접속채널번호		I		X	접속 순차번호(3)+일련번호(9)
			char	pbid[16]{};	// 53	트랜잭션추적ID		I	Cf	X	전문 추적ID ('@'로시작)  ♥ 로그인 성공시 받은값 유지
			char	puip[12]{};	// 69	공인IP			I	S		공인IP (168.126.63.1의 경우 "168126063001" 형태)
			char	pcip[12]{};	// 81	사설IP			I	C		사설IP (168.126.63.1의 경우 "168126063001" 형태)
			char	dept[3]{};	// 93	처리지점 번호		I			처리PC가 위치한 지점
			char	brno[3]{};	// 96	지점번호		I			계좌가 속한 지점
			char	term[8]{};	// 99	단말번호		I	Cm	●	매체코드 (직원 : 001 고객 : 002)
			char	lang[1]{};	// 107	언어구분		I	S		K:한글, E:영문, C:중국, J:일본
			char	time[9]{};	// 108	AP처리시각		O			요청 처리시각 (HHMMSSmmm)
			char	ecod[4]{};	// 117	메시지코드		O			오류코드 :(정상코드: 0000 ~ 0999, 비정상코드 : 1000 ~ 9999, 요청시 Space로 채움)
			char	eknd[1]{};	// 121	출력메시지구분		O			
			char	fep2[1]{};	// 122	압축요청구분		I		X	
			char	fkey[4]{};	// 123	기능키목록		I/O	Cm	●	입력: 기능키(1바이트) ♥ TxHead(2) of Map (5:확인 7:연속 C:전송)
			char	rcnt[4]{};	// 127	요청 레코드 개수	I	Cm	●	일반적으로 "0000"을 입력
			char	fil1[6]{};	// 131	Filler			-		X	
			char	next[1]{};	// 137	연속여부		I/O	C		연속 데이터 유무 (Y:연속있음, 이외의 값:연속없음)
			char	nkey[18]{};	// 138	연속키			I/O	C	●	연속처리를 위한 키값
			char	usiz[2]{};	// 156	가변시스템정보길이	I		X	IBK 고정	
			char	hsiz[2]{};	// 158	가변헤더정보길이	I/O		X	사용 : epwd와 함께 사용
			char	msiz[2]{};	// 160	가변메시지길이		O		X	성공 및 오류메시지 (요청시 "00"입력)
			char	fep3[10]{};	// 162	Reserved for  FEP		S	X	
			char	mkty[1]{};	// 172	시장구분			Cfm	●	1:현물 2:선옵 3:기타    ♥TxHead(0) of MAP
			char	odrf[1]{};	// 173	주문구분			Cfm	●	1:주문 2:조회 3:이체    ♥TxHead(1) of MAP
			char	evid[4]{};	// 174	Reserved for  I/F			X	
			char	svrm[12]{};	// 178	서버IP				S		서버IP (168.126.63.1의 경우 "168126063001" 형태)
			char	skip[1]{};	// 190	Flag for I/F
			char	fil2[49]{};	// 191	Filler					X	
			char	eact[1]{};	// 240	에러구분					에러구분(0:상태바 1:메세지박스)
			char	emsg[99]{};	// 241	오류메세지					코드(4)+메시지(94)(9999 오류입니다 !)
			char	epwd[44]{};	// 340	sha256암호화 된 계좌비밀번호
		};
		#define	L_ledgerH	sizeof(struct _ledgerH)		// 384 bytes

		struct _ledgerH ledger;
		m_guard->Ledger((char*)&ledger);

		CString strUser;
		strUser = "rhk62";

		CString acc, pass, sSvcn, strData;
		acc = "00110016877";
		sSvcn = "SACMT238";

		CopyMemory(&ledger.svcd, (LPCTSTR)sSvcn, sizeof(ledger.svcd));
		CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
		CopyMemory(&ledger.brno, acc.Left(3), 3);
		CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

		ledger.fkey[0] = 'C';
		ledger.mkty[0] = '1';
		ledger.odrf[0] = '1';

		strData = CString((char*)&ledger, L_ledgerH);


		struct _chkPwdMid {
			char in[5];
			char acctNo[20];
			char password[8];
			//char zTrxTp[1];
		};
#define L_chkPwdMid	sizeof(struct _chkPwdMid)

		struct _chkPwdMid mid;
		FillMemory(&mid, L_chkPwdMid, ' ');
		CString acnt = acc;

		memset(&mid, ' ', sizeof(mid));
		memcpy(mid.in, "00001", 5);
		memcpy(mid.acctNo, acc, acc.GetLength());
		memcpy(mid.password, "0423", 4);	//2013.13.23 KSJ 일방향암호화 추가

		strData += CString((char*)&mid, L_chkPwdMid);

		data = screen->m_xscreen->ServiceEx("pidomyst", strData, strData.GetLength(), US_PASS, 60000);
	}
	else
	{
		int	sendL = 0;
		std::string sendB;
		sendB.resize(16 * 1024);
		const int isz_updn = sz_uinfo;
		struct _updn* updn = (struct _updn*)&sendB[sendL];
		sendL += isz_updn;

		FillMemory((char*)updn, isz_updn, ' ');
		CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
		updn->uinfo.dirt[0] = 'U';
		updn->uinfo.cont[0] = 'g';
		CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
		updn->uinfo.retc[0] = 'O';

		data = screen->m_xscreen->ServiceEx("pidomyst", sendB.data(), sendL, US_PASS, 1000);
	}
	

	if (!data.IsEmpty() && data.GetLength() > 0)
	{
		struct	_gfoldex* group = (struct _gfoldex*)data.GetString();
		const	int	cnt = atoi(CString(group->nrec, sizeof(group->nrec)));
		if (cnt > 0)
		{
			CString	tmps, tmpx;
			for (int ii = 0, ival = 0; ii < cnt; ii++)
			{
				tmps = CString(group->glist[ii].gnam, sizeof(group->glist[ii].gnam));
				ival = atoi(CString(group->glist[ii].ogrs, sizeof(group->glist[ii].ogrs)));
				if (tmps.IsEmpty())
					continue;
				arr.emplace_back(ival, tmps);				
			}
		}
	}
}