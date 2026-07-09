// Client.cpp: implementation of the CClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "imm.h"
#include "Wizard.h"
#include "Client.h"
#include "Script.h"
#include "Stream.h"
#include "Key.h"
#include "Mouse.h"
#include "History.h"
#include "../h/axiserr.h"
#include "../h/axisvar.h"
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

	HANDLE instance = (HANDLE)GetWindowLongPtr(m_view->GetSafeHwnd(), GWLP_HINSTANCE);
	FARPROC	callproc = (FARPROC)MakeProcInstance(CallProc, instance);
	m_callproc = (FARPROC)SetWindowLongPtr(m_view->GetSafeHwnd(), GWLP_WNDPROC, (LONG_PTR)callproc);

	m_keys.RemoveAll();
	m_screen.RemoveAll();
	m_control.RemoveAll();

	SetBK();
	m_hR = m_vR = 1.0;
}

CClient::~CClient()
{
	m_guard->SetRepository(this);

	SetInitial(false);
	m_dominos.RemoveAll();

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
}

void CClient::SetInitial(bool reset)
{
	History(false);

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

	m_sync.Lock();

	CString	domino;
	char	tmapN[L_MAPN+1];

	if (maps.GetLength() < L_MAPN)
		return false;
	domino = maps.Mid(L_MAPN);
	maps = maps.Left(L_MAPN);
	CopyMemory(tmapN, (char *)maps.operator LPCSTR(), L_MAPN);
	tmapN[L_MAPN] = '\0';

	CScreen* screen;
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
		char	twb[L_MAPN+1];

		CopyMemory(twb, m_mapN, L_MAPN);
		twb[L_MAPN] = '\0';
		if (m_guard->GetVirtualTR(twb) && !strcmp(twb, tmapN))
			equal = true;
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

		m_mapN = maps;
		screen->Parse();
		OnResize(m_rect.Width(), m_rect.Height());
		m_guard->SendAxis(MAKEWPARAM(titleVIEW, m_key), (LPARAM)screen->GetString(screen->m_mapH->caption).operator LPCSTR());

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

		m_font = screen->m_mapH->fPoint;
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

	if (key != -1)						// disable trigger only for internal trigger
		m_status |= S_TRIX;
	if (name.IsEmpty())
	{
		if (GetScreen(screen) && m_current.m_idx != -1)
		{
			form = screen->GetAtForm(m_current.m_idx);
			if (!(form->m_form->attr & FA_NOR))
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
		if (form->IsChanged())
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
			text = form->GetSymbolName();
			return TRUE;
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
	int	key;
	CString	name;

	key = text.GetAt(0);
	text.Delete(0);

	if (key == m_key)
		return;
	key = text.Find('\t');
	if (key == -1)
		return;
	name = text.Left(key++);
	text = text.Mid(key);
	OnTrigger(name, text, -1);
}

void CClient::SetPalette(bool force)
{
	CScreen* screen;

	if (GetAtScreen(screen))
		screen->SetPalette(force);
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
	tmps.Format("%s\\%s\\%s", m_guard->m_root, IMAGEDIR, bitmapN);

	FreeBK(false);
	m_bitmap = (HBITMAP)LoadImage(NULL, tmps, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
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
	CString	text;
	char	wb[64];
	DWORD	value;

	text.Format("%s/%s/%s", m_guard->m_root, TABDIR, PALETTE);
	value = GetPrivateProfileString(GENERALSN, _T("Back"), "", wb, sizeof(wb), text);
	text = CString(wb, value);
	text.TrimLeft(); text.TrimRight();
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

bool CClient::SetObject(int key, CScreen*& screen, CRect fmRect, char* mapN)
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

		if (m_mouse->m_caret.m_key == screen->m_key)
			m_mouse->m_caret.m_key = -1;

		delete screen;
	}

	if (mapN == NULL)
		return false;

	return SetAtScreen(screen, fmRect, mapN, key);
}

bool CClient::SetObject(CScreen* main, int key, char *mapN)
{
	CScreen* screen;
	int	parent, object;
	bool	cursor = false;
	bool	focus  = false;

	m_sync.Lock();
	CfmBase* form = main->GetAtForm(key);

	if (GetAtScreen(screen, form->m_form->keys))
	{
		if (!memcmp(screen->m_mapH->mapN, mapN, L_MAPN))
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
	}
	else
	{
		parent = main->m_key;
		object = key;
		key = -1;
	}
	SaveDomino();

	CRect	unitRc;
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

	bool success = SetObject(key, screen, unitRc, mapN);

	if (success)
	{
		form->m_form->keys = screen->m_key;
		screen->m_parent   = parent;
		screen->m_object   = object;
		screen->Parse(true);
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

	return success;
}

void CClient::GetKeyObject(CScreen* screen, CfmBase*& form)
{
	CScreen* main;
	GetAtScreen(main, screen->m_parent);
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

	CSize	size;
	CScreen* screen;

	size = ((CScrollView*)m_view)->GetScrollPosition();
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

	if ((m_type & vtypeMSK) != vtypeRSZ)
	{
		if (Height && Width)
		{
			if (!GetAtScreen(main))
				return;

			hRatio = (float)Width / (float)main->m_mapH->width;
			vRatio = (float)Height / (float)main->m_mapH->height;
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
		screen->ResizeForm(hRatio, vRatio);
	}
}

void CClient::OnFocus(BOOL focus, HWND hWnd)
{
	m_focus = focus;
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
				form->OnDrag(CPoint(-10, -10), result);
			}
		}

		if (m_current.m_idx != -1 && GetScreen(screen))
			screen->SetForm(m_current.m_idx, focus ? true : false);

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

void CClient::OnControl(HWND hWnd)
{
	int	key;
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
}

bool CClient::OnControl()
{
	int	key;
	CScreen *screen;

	for (int idx = 0; idx < m_magic; idx++)
	{
		if (GetAtScreen(screen, idx) && screen->OnControl(NULL, key))
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
		if (form->m_form->kind == FM_OBJECT && GetAtScreen(screen, m_mouse->m_caret.m_key))
		{
			if (m_mouse->m_caret.m_idx != -1 && m_mouse->m_caret.m_idx < screen->m_mapH->formN)
				form = screen->GetAtForm(m_mouse->m_caret.m_idx);
			else	return;
		}
		if (form->IsChanged())
			m_vm->OnChange(screen, form);
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
		if (form->IsChanged())
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
			CfmBase* form;
			CRect	fmRect;

			GetKeyObject(screen, form);
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

	GetAtScreen(main, screen->m_parent);
	if (!main->IsFormVisible(form))
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
			if (!isVisible(screen, object) || !screen->IsFormVisible(base))
				return false;
		}
		else if (!screen->IsFormVisible(base))
			return false;
	}
	return true;
}

void CClient::OnPrint(CDC* pDC)
{
	CRect	clientRc, metaRc;
	HENHMETAFILE	metaFile;
	CMetaFileDC*	metaDC;

	CDC*	pcDC;
	int	inx, iny;
	int	cx, cy, numerator;

	if (pDC->IsPrinting())
		m_view->RedrawWindow();

	pcDC = m_view->GetDC();
	inx = pcDC->GetDeviceCaps(LOGPIXELSX);
	iny = pcDC->GetDeviceCaps(LOGPIXELSY);
	m_view->ReleaseDC(pcDC);

	numerator = 2540;
	if (GetVersion() < 0x80000000)
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

	int	status;
	status = m_guard->m_app->GetProfileInt(ENVIRONMENT, STATUS, 0);
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
	CScreen *screen;
	m_sync.Lock();
	m_status |= S_LOCK;

	for (int idx = 0; idx < m_magic; idx++)
	{
		if (GetAtScreen(screen, idx))
			screen->OnPush(pushs);
	}

	m_status &= ~S_LOCK;
	m_sync.Unlock();
}

void CClient::OnAlert(CString code, CdataSet* fms, CObArray* obs, int stat, struct _alertR* alertR)
{
	if (!(m_status & S_FLASH))
		return;

	CScreen* screen;
	m_sync.Lock();
	m_status |= S_LOCK;

	for (int idx = 0; idx < m_magic; idx++)
	{
		if (GetAtScreen(screen, idx) && screen->isFlash())
		{
			if (screen->OnAlert(code, fms, obs, stat, alertR))
				m_vm->OnProcedure(screen, "OnAlert");
			m_vm->OnAlert(screen, code, _T(""), stat);	// maybe does not run : return true
		}
	}

	m_status &= ~S_LOCK;
	m_sync.Unlock();
}

void CClient::OnNotice(CdataSet& major, CdataSet& minor, CdataSet& fms, CString text)
{
	CScreen* screen;
	m_sync.Lock();
	m_status |= S_LOCK;

	for (int idx = 0; idx < m_magic; idx++)
	{
		if (GetAtScreen(screen, idx) && screen->isNotice())
		{
			if (screen->OnNotice(major, minor, fms, text))
				m_vm->OnProcedure(screen, "OnNotice");
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
	int	key, value, idx = 0;
	int	form, parent = screen->m_key;

	if (screen->isFlash() || screen->m_mapH->options & OP_OOP)
		keys[idx++] = screen->m_key;

	for (POSITION pos = m_keys.GetStartPosition(); pos; )
	{
		m_keys.GetNextAssoc(pos, key, value);
		if (parent == GetScreenKey(key, form))
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

	CString	text;
	CScreen* screen;
	CfmBase* form;

	int	idx, jj, kk;
	int	nCols, nRows;
	struct	_cellR *cell;
	CString	name, tmps;

	for (idx = 0; idx < m_magic; idx++)
	{
		if (!GetAtScreen(screen, idx))
			continue;
		for (int ii = 0; ii < screen->m_mapH->formN; ii++)
		{
			form = screen->GetAtForm(ii);
			if (form->m_form->attr & FA_NOR)
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
						form->ReadData(text, false, jj, kk);
						text.TrimRight();
						if (text.IsEmpty())
							continue;

						tmps.Format("%d$%d", jj, kk);
						name = form->GetSymbolName();
						name += tmps;
						name = name + form->GetSymbolName(cell[jj].name);
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
				m_dominos.SetAt(form->GetSymbolName(), text);
		}
	}
}

int CClient::RestoreDomino(bool force)
{
	int	dominoN;
	CScreen	*screen;
	CMapStringToString codes;

	codes.RemoveAll();
	m_guard->GetCode(codes);
	if (m_dominos.IsEmpty() && codes.IsEmpty())
		return 0;

	dominoN = 0;
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
					tmps.Format("%d$%d", ii, kk);
					name = form->GetSymbolName();
					name += tmps;
					name = name + form->GetSymbolName(cell[ii].name);
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

		if (dominos.Lookup(form->GetSymbolName(), text))
		{
			form->WriteData(text);
			if (form->IsChanged())
			{
				m_vm->OnChange(screen, form);
				dominoN++;
			}
		}
	}
	return dominoN;
}

void CClient::CodeDomino(CScreen* screen, CMapStringToString& dominos)
{
	CString	text;
	CfmBase	*form;

	if (dominos.IsEmpty())
		return;

	for (int idx = 0; idx < screen->m_mapH->formN; idx++)
	{
		form = screen->GetAtForm(idx);
		if (form->m_form->attr & FA_NOR)
			continue;
		if (!(form->m_form->properties & PR_CODE))
		{
			m_guard->GetCode(form->GetSymbolName(), text);
			if (text.IsEmpty())
				continue;
		}

		switch (form->m_form->iok)
		{
		case EIO_INPUT:
		case EIO_INOUT:
			if (dominos.Lookup(form->GetSymbolName(), text))
			{
				form->WriteData(text);
				if (form->IsChanged())
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
		screen->m_state |= waitSN;
	else
	{
		for (int idx = 0; idx < m_magic; idx++)
		{
			if (!GetAtScreen(screen, idx))
				continue;

			if (screen->m_skip || screen->m_mapH->mapM == MM_MENU)
				continue;
			screen->m_state |= waitSN;
		}
	}

	m_status |= S_WAIT;
	if (timeout && m_guard->m_wait > 0)
		m_view->SetTimer(TM_WAIT, m_guard->m_wait, NULL);

	m_guard->PostAxis(MAKEWPARAM(waitPAN, m_key), true);
}

void CClient::WaitDone(CScreen* screen, bool timeout, bool wait)
{
	int	ii;

	if (screen)
	{
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
	}

	m_status &= ~S_WAIT;
	if (timeout && m_guard->m_wait > 0)
		m_view->KillTimer(TM_WAIT);

	m_guard->PostAxis(MAKEWPARAM(waitPAN, m_key), false);
}

void CClient::RepeatTR(CScreen* screen)
{
	int	repeatV;

	repeatV = screen->m_mapH->repeatV;
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
		default:
			m_status &= ~S_HAN;
			break;
		}
		break;
	default:
		return;
	}

	ChangeIME(true);
}

void CClient::ChangeIME(bool ime)
{
	HIMC	hIMC;
	DWORD	dwConversion, dwSentence;

	hIMC = ImmGetContext(m_view->m_hWnd);
	if (!ImmGetConversionStatus(hIMC, &dwConversion, &dwSentence))
		return;

	if (ime)
	{
		DWORD	dwConvSave;
		dwConvSave = dwConversion;
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
		return;
	}

	m_status &= ~(S_HAN|S_FULL);
	if (dwConversion & IME_CMODE_HANGEUL)
		m_status |= S_HAN;
	if (dwConversion & IME_CMODE_FULLSHAPE)
		m_status |= S_FULL;

	m_guard->PostAxis(MAKEWPARAM(hangulPAN, m_key), m_status & S_HAN);
	m_guard->PostAxis(MAKEWPARAM(doublePAN, m_key), m_status & S_FULL);
}

bool CClient::RunMenu(CScreen* screen, CfmBase* form, CPoint point)
{
	CScreen* main;
	if (!GetAtScreen(main))
		return false;

	int	index, pos;
	char	*ptr, tmpb[1024];;

	CStringArray	items;
	CString		tmps, mapN, path, maps;

	m_guard->SendAxis(MAKEWPARAM(nameVIEW, m_key), (LPARAM)tmpb);
	maps = CString(tmpb, L_MAPN);
	items.RemoveAll();

	CMenu *popM = new CMenu();
	popM->CreatePopupMenu();
	m_guard->GetSignName(tmps);
	path.Format("%s\\%s\\%s\\%s", m_guard->m_root, USRDIR, tmps, maps.Left(L_SELC));

	index = -1;
	DWORD	rc = GetPrivateProfileSection(maps, tmpb, sizeof(tmpb), path);
	if (rc <= 0)
	{
		path.Format("%s\\%s\\%s", m_guard->m_root, MTBLDIR, maps.Left(L_SELC));
		rc = GetPrivateProfileSection(maps, tmpb, sizeof(tmpb), path);
	}
	ptr = tmpb;
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

		index = (int)items.Add(mapN);
		popM->AppendMenu(MF_STRING|MF_ENABLED, menuBASE+index, tmps);
	}

	if (index >= 0 && m_guard->GetString(AS_LINK, tmps))
	{
		index++;
		popM->AppendMenu(MF_SEPARATOR, 0);
		popM->AppendMenu(MF_STRING|MF_ENABLED, menuLINK, tmps);
	}

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
				{
					if (++index > 0)
					{
						pos++;
						popM->AppendMenu(MF_SEPARATOR, 0);
					}
					popM->AppendMenu(MF_STRING|MF_ENABLED, menuXLS, tmps);
				}
			}
			if (form->m_form->kind == FM_GRID)
			{
				if (form->m_form->attr & FA_ENUM && m_guard->GetString(AS_ITEM, tmps))
				{
					if (++index > 0 && pos == 0)
						popM->AppendMenu(MF_SEPARATOR, 0);
					popM->AppendMenu(MF_STRING|MF_ENABLED, menuGRID, tmps);
				}
			}
			break;
		default:
			break;
		}
	}

	if (index < 0)
	{
		delete popM;
		return false;
	}


	m_view->ClientToScreen(&point);
	index = popM->TrackPopupMenu(TPM_LEFTALIGN|TPM_RETURNCMD, point.x, point.y, m_view);
	delete popM;

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
				SetCode(tmps);

			mapN += (m_guard->m_status & WS_LINK) ? "/t/d" : "/s/t/d";
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

	case menuLINK:
		m_string = maps;
		m_guard->PostAxis(MAKEWPARAM(linkVIEW, 0), (LPARAM)m_string.operator LPCTSTR());
		return true;

	default:
		return false;
	}

	return true;
}

void CClient::ClearShift()
{
	CScreen* screen;

	m_status &= ~(S_SHIFT|S_CONTROL);
	if (GetAtScreen(screen, m_cbox.m_key))
	{
		CfmBase* form = screen->GetAtForm(m_cbox.m_idx);
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
	switch (mode)
	{
	case RC_SEARCH:
		items = m_guard->GetCodes();
		size.cx = 180;
		size.cy = 160;
		m_status |= S_FIND;
		break;
	case RC_NOP:
		self = false;
	case RC_CODE:
		count = GetCodes(form->GetSymbolName(), items, self);
		if (count == 0)
			return;
		size.cx = (items.Find(' ') == -1) ? 80 : 180;
		size.cy = count < 9 ? count*13+6 : 120;
		m_status |= self ? S_CHILD : S_FIND;
		break;
	default:
		return;
	}

	CRect	cRc;
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
	if (!((CHistory *)m_child)->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, wRc, m_view, 0))
	{
		m_status &= ~(S_CHILD|S_FIND);
		delete m_child;
	}
}

void CClient::History(bool apply)
{
	if (m_status & (S_CHILD|S_FIND))
	{
		CString	text;

		m_status &= ~(S_CHILD|S_FIND);
		if (apply && ((CHistory *)m_child)->GetItem(text))
		{
			CScreen* screen;
			if (GetAtScreen(screen, m_cbox.m_key))
			{
				CfmBase* form = screen->GetAtForm(m_cbox.m_idx);
				form->WriteData(text);
				if (form->IsChanged())
				{
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
		}
		delete m_child;
	}
}

void CClient::SelfHistory(CfmBase* form)
{
	CString	text;

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
			if (isdigit(text[ii]))
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
	if (form->m_form->properties & PR_LIST && !(form->m_form->attr & FA_PROTECT))
	{
		CString	text;

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

		next.TrimLeft();
		next.TrimRight();
		if (next.GetLength() == L_MAPN)
			return;
	}

	next.Empty();
}

bool CClient::isNext(CScreen* screen, int key)
{
	CfmBase* form;

	form = screen->GetAtForm(key);
	if (form->m_form->kind != FM_GRID || form->m_form->vals[2] == NOVALUE)
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

	CString	maps, tmps;

	tmps.Empty();
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
			CfmBase* form;

			form = screen->GetAtForm(m_form.m_idx);
			form->SetCurSel(m_cell);
		}
	}
}

void CClient::SetCode(CString& codes)
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
			if (form->m_form->properties & PR_CODE)
			{
				form->ReadData(text);
				text.TrimLeft();
				if (!text.IsEmpty())
				{
					codes += form->GetSymbolName();
					codes += '\t';
					codes += text;
					codes += '\n';
				}
			}
		}
	}
	GetCodes(codes);
}

bool CClient::SetCode(CScreen* screen, CfmBase* form, CString& codes, bool force)
{
	int	row, col;

	row = ((CfmGrid *)form)->GetCellRow();
	if (((CfmGrid *)form)->GetValidRows() < row)
		return false;

	col = ((CfmGrid *)form)->GetCellColumn();
	if (--col < 0 || --row < 0)
		return false;

	if (!GetDragCell(screen, form, col, force))
		return false;

	CString	text;
	struct	_cellR*	cell;

	form->ReadData(text, false, col, row);
	cell = (struct _cellR *) &screen->m_cellR[form->m_form->vals[0]];

	text.TrimLeft();
	if (text.IsEmpty())
		return false;

	codes = form->GetSymbolName(cell[col].name);
	codes += '\t';
	codes += text;	codes += '\n';

	if (force)
		GetCodes(codes);

	return true;
}

bool CClient::GetDragCell(CScreen* screen, CfmBase* form, int& col, bool force)
{
	CPtrArray*	grid;
	int nCols = HIWORD(form->m_form->vals[1]);

	if (form->m_form->attr & FA_ENUM && screen->m_cells.Lookup(form->GetSymbolName(), (CObject *&)grid))
		nCols = (int)grid->GetSize();
	else
		grid = NULL;

	int	idx = col;
	struct	_cellR*	cell;

	cell = (struct _cellR *) &screen->m_cellR[form->m_form->vals[0]];
	for (col = 0; col < nCols; col++, cell++)
	{
		if (grid != NULL)
			cell = (struct _cellR *)grid->GetAt(col);
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
	CFileDialog fDlg(FALSE, "txt", path.IsEmpty() ? form->GetSymbolName() : path.operator LPCTSTR(),
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
			fileN.Format("EX%s(%s)", CString(&screen->m_mapH->mapN[L_SGID], L_SELC), form->GetSymbolName());
		else
			fileN.Empty();
		m_guard->MakeXlsFile(GetFormat(screen, form), text, visible, fileN);
	}
	else
		m_guard->MakeXlsFile(GetFormat(screen, form), text, visible, path, true);
}

void CClient::Export(CScreen* screen, CfmBase* form, bool visible, CString data)
{
	int	offs;
	CString	text, path, string, ext;
	
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

	if (!ext.CompareNoCase("xls") || !ext.CompareNoCase("csv"))
	{
		m_guard->MakeXlsFile(GetFormat(screen, form), text, visible, path, true);
		return;
	}

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

		if (form->m_form->attr & FA_ENUM && screen->m_cells.Lookup(form->GetSymbolName(), (CObject *&)grid))
			nCols = (int)grid->GetSize();
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
					form->ReadData(tmps, true, kk, idx);
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
		CFileDialog fDlg(FALSE, "txt", form->GetSymbolName(), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
								"Text Files(*.txt)|*.txt|All Files(*.*)|*.*|");

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
	m_guard->OnPrint(m_string + text, form->GetSymbolName(), screen->GetString(form->m_form->fName), form->m_form->fPoint);
}

bool CClient::SetCaption(CString maps)
{
	CScreen* screen;

	for (int ii = 0; ii < m_magic; ii++)
	{
		if (GetAtScreen(screen, ii) && !maps.CompareNoCase(CString(screen->m_mapH->mapN, L_MAPN)))
		{
			m_guard->SendAxis(MAKEWPARAM(titleVIEW, m_key), (LPARAM)screen->GetString(screen->m_mapH->caption).operator LPCSTR());
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
	case dpGREAT:
		if (!GetAtScreen(screen, screen->m_parent))
			return false;
	case dpGRAND:
		if (!GetAtScreen(screen, screen->m_parent))
			return false;
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
	HINSTANCE hinst;
	
	path.Format("%s/%s/%s.dll", m_guard->m_root, DEVDIR, name);
	hinst = AfxLoadLibrary(path);
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
	int	key;
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
		fonts = screen->m_fonts;
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
			return FALSE;

		m_rect.left   = 0;
		m_rect.right  = width;
		m_rect.top    = 0;
		m_rect.bottom = height;
		m_guard->SendAxis(sizeVIEW, m_key, MAKEWPARAM(cx, cy));
	}

	m_font = point;
	m_hR = hR; m_vR = vR;
	for (POSITION pos = m_screen.GetStartPosition(); pos; )
	{
		m_screen.GetNextAssoc(pos, key, screen);
		if (resize)
			screen->ResizeForm(m_hR, m_vR);
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
					domino.SetAt(form->GetSymbolName(), text);
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
	path.Format("%s\\%s\\axis.ini", m_guard->m_root, TABDIR);
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
	path.Format("%s\\%s\\axis.ini", m_guard->m_root, TABDIR);
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
				text = screen->GetString(form->m_form->onHint);
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
				form->ReadData(text, false, MAKELONG(idx-1, point.y), -3);
				break;
			case FM_TABLE:
			//	form->ReadData(text, false, -3, idx-1);
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
			switch (form->m_form->kind)
			{
			case FM_COMBO:
				if (form->m_form->properties & PR_ACCN)
				{
					if (!(m_guard->m_term & flagACN) && m_guard->GetCombo(form->GetSymbolName(), text))
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
		char	twb[256];

		text.Format("%s$%s", CString(screen->m_mapH->mapN, L_MAPN), form->GetSymbolName());
		GetPrivateProfileString("GRID", text, "", twb, sizeof(twb), m_guard->m_modals);
		tmps = twb;
		tmps.TrimLeft();
		tmps.TrimRight();
		index = tmps.GetLength();

		text.Empty();
		struct _cellR*	cell;
		cell = (struct _cellR *) &screen->m_cellR[form->m_form->vals[0]];
		int nCols = HIWORD(form->m_form->vals[1]);

		for (idx = form->m_form->type; idx < nCols; idx++)
		{
			text += form->GetSymbolName(cell[idx].name);
			text += '\t';
			text += (cell[idx].head  == NOVALUE) ? "HEAD ?" : screen->GetString(cell[idx].head);
			text += '\t';

			if (cell[idx].properties & PR_VISIBLE && index == 0)
			{
				tmps += form->GetSymbolName(cell[idx].name);
				tmps += '|';
			}
		}

		defs.Empty();
		for (idx = form->m_form->type; idx < nCols; idx++)
		{
			if (!(cell[idx].properties & PR_VISIBLE))
				continue;

			defs += form->GetSymbolName(cell[idx].name);
			defs += '|';
		}

		if (m_guard->Modal(columnED, text, tmps, defs))
		{
			text.Format("%s$%s", CString(screen->m_mapH->mapN, L_MAPN), form->GetSymbolName());
			WritePrivateProfileString("GRID", text, tmps, m_guard->m_modals);
			screen->SetCells(form, tmps);
			m_stream->InStream(screen);
		}
	}
}

int CClient::GetKey(int skey, int ukey, int index)
{
	int	key, keyx;

	key = MAKELONG(MAKEWORD(skey, ukey), index);
	for (int ii = 0xfe; ii >= 0; ii--)
	{
		if (m_keys.Lookup(ii, keyx))
		{
			if (key == keyx)
				return ii;
			continue;
		}
		else
		{
			m_keys.SetAt(ii, key);
			return ii;
		}
	}

	return -1;
}

int CClient::GetScreenKey(int key, int& index)
{
	int	skey;

	if (m_keys.Lookup(key, skey))
	{
		index = HIWORD(skey);
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

	int	nCols;
	CString	format;
	CPtrArray*	grid;
	struct _cellR	*cell;

	nCols = HIWORD(form->m_form->vals[1]);
	if (form->m_form->attr & FA_ENUM && screen->m_cells.Lookup(form->GetSymbolName(), (CObject *&)grid))
		nCols = (int)grid->GetSize();
	else
		grid = NULL;

	cell = (struct _cellR *) &screen->m_cellR[form->m_form->vals[0]];
	for (int kk = 0; kk < nCols; kk++, cell++)
	{
		if (grid != NULL)
			cell = (struct _cellR *)grid->GetAt(kk);
		if (form->IsVisible(kk))
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
