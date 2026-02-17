// Key.cpp: implementation of the CKey class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "imm.h"
#include "Wizard.h"
#include "Key.h"
#include "Client.h"
#include "Mouse.h"
#include "Script.h"
#include "Stream.h"
#include "History.h"

#include "../h/axiserr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	FMX_OK		0
#define	FMX_OB		1
#define	FMX_END		2

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKey::CKey(CClient* client)
{
	m_client = client;
	m_guard  = client->m_guard;
}

CKey::~CKey()
{
}

void CKey::OnKey(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_SHIFT:
	case VK_CONTROL:
	case VK_CAPITAL:
	case VK_NUMLOCK:
		return;
	default:
//		if (m_client->m_status & S_WAIT)
		if (m_guard->IsWait(m_client))
		{
			MessageBeep(MB_OK);
			return;
		}
		break;
	}

	switch (m_client->m_status & S_FIND)
	{
	case S_FIND:
		switch (wParam)
		{
		case VK_RETURN:
			m_client->History(true);
			return;
		case VK_BACK:
		case VK_DELETE:
			break;
		default:
			if (!((CHistory *)m_client->m_child)->OnKey(wParam))
				m_client->History(false);
			return;
		}
		break;
	default:
		break;
	}

	if (m_client->m_status & (S_SHIFT|S_CONTROL) &&
		(m_client->m_cbox.m_key != m_client->m_current.m_key ||
		m_client->m_cbox.m_idx != m_client->m_current.m_idx))
		m_client->ClearShift();

	CScreen	*screen;
	CfmBase* form;
	if (!SetOnKeys(screen, form, true) && !screen)
		return;

	int	result;

	int	trxK = 0;

	switch (wParam)
	{
	case VK_TAB:
		if (OnKeyCell(screen, form, wParam))
			return;
		if (lParam)
			GoPrevForm();
		else
			GoNextForm();
		return;
	case VK_BACK:
	case VK_DELETE:
	case VK_LEFT:
	case VK_RIGHT:
		break;
	case VK_INSERT:
		m_client->m_status ^= S_INS;
		return;			
	case VK_UP:
	case VK_DOWN:
		if (form && form->m_form->kind == FM_EDIT)
		{
			if (form->IsEnable() && form->m_form->properties & (PR_CODE|PR_SPIN))
			{
				CString	text;
				int	value;

				form->GetEnum(text, value);
				if (!text.IsEmpty())
					break;
			}

			if (wParam == VK_DOWN)
				GoNextForm();
			else
				GoPrevForm();
			return;
		}
		break;

	case VK_ESCAPE:
		return;

	case VK_RETURN:
		if (form && OnKey(screen, wParam, form))
			return;
		if (OnKeyCell(screen, form, wParam))
			return;

		if (screen->m_mapH->options & OP_CR2TB)
		{
			if (!form)	return;

			if (form->IsEnable())
			{
				if (form->m_form->attr & FA_SEND)
				{
					if ((form->m_form->attr2 & SA_SEND) == SA_THIS)
						m_client->m_stream->InStream(screen);
					else
						m_client->m_stream->InStream();
					return;
				}

				if (form->m_form->kind == FM_BUTTON && (form->m_form->type == BTN_PUSH || form->m_form->type == BTN_USER)
					&& form->IsEnable() && m_client->m_vm->IsAvailable(screen, evClick, form))
				{
					m_client->m_vms.scroll = SB_NS;
					m_client->m_vm->OnClick(screen, form);
					return;
				}
				else if (form->m_form->kind == FM_GRID && form->m_form->iok == EIO_OUTPUT && form->m_form->isEdit != FI_NONE
					&& m_client->m_vm->IsAvailable(screen, evClick, form))
				{
					m_client->m_vms.scroll = SB_NS;
					m_client->m_vm->OnClick(screen, form);
					return;
				}
			}
			GoNextForm();
			return;
		}

		if (m_client->m_vm->IsAvailable(screen, evKey))
		{
			OnKey(screen, wParam);
			return;
		}

		if (form == NULL)
		{
			m_client->m_stream->InStream(true);
			return;
		}

// updateXXX_2022XX
		if (form->IsEnable())
		{
			if (form->m_form->kind == FM_EDIT)
			{
				form->OnKey(wParam, result);
				if (result != RC_NOP)
					return;
			}

			if (!(lParam & 0x1000000))
			{
				switch (form->m_form->kind)
				{
				case FM_MEMO:
					form->OnKey(wParam, result);
					return;
				case FM_GRID:
					if (m_client->m_vm->IsAvailable(screen, evClick, form))
					{
						m_client->m_vms.scroll = SB_IM;
						m_client->m_vm->OnClick(screen, form);
						return;
					}
					break;
				default:
					break;
				}
			}
		}

		if (screen->m_mapH->mapM == MM_MENU)
			return;

		switch (form->m_form->kind)
		{
		case FM_BUTTON:
			switch (form->m_form->type)
			{
			case BTN_PUSH:
			case BTN_USER:
				if (!form->IsEnable() || !m_client->m_vm->IsAvailable(screen, evClick, form) || !(form->m_form->properties & PR_CHECKED))
					break;

				m_client->m_vm->OnClick(screen, form);
				return;
			default:
				break;
			}
			break;
		default:
			break;
		}

		m_client->m_stream->InStream(true);
		return;

	case VK_F1: case VK_F2: case VK_F3: case VK_F4:
	case VK_F5: case VK_F6: case VK_F7: case VK_F8:
	case VK_F9: case VK_F10: case VK_F11: case VK_F12:
		if (screen->m_mapH->typeH == TH_KOSCOM)
		{
			if (isKoscom(wParam))
			{
				m_client->m_stream->InStream();
				return;
			}
		}

		if (wParam == VK_F1)
		{
			m_client->GetAtScreen(screen);
			 if (screen->m_mapH->helpN != NOVALUE)
				 m_guard->RunHelp((char *)screen->m_mapH->helpN);
			return;
		}
	case VK_PAUSE:
		trxK = -1;
		break;
	case VK_HOME:
		trxK = trxK_HOME;
		break;
	case VK_PRIOR:
		trxK = trxK_PGUP;
		break;
	case VK_NEXT:
		trxK = trxK_PGDN;
		break;
	case VK_END:
		trxK = trxK_END;
		break;
	default:
		return;
	}

	if (trxK)
	{
		if (form && OnKey(screen, wParam, form))
			return;

		if (trxK != -1 && screen->m_trxS & trxK)
		{
			screen->m_trxK = trxK;
			m_client->m_stream->InStream(screen, true);
			return;
		}
		if (m_client->isKeyButton(wParam))
			return;

		if (m_client->m_vm->IsAvailable(screen, evKey))
		{
			OnKey(screen, wParam);
			return;
		}
	}

	if (form == NULL || !form->IsEnable())
		return;

	result = 0;
	if (GetKeyState(VK_SHIFT) & 0x8000)
		result++;
	if (GetKeyState(VK_CONTROL) & 0x8000)
		result += 2;

	switch (result)
	{
	case 1:
		result = RC_SHIFT;
		break;
	case 2:
		result = RC_CONTROL;
		break;
	case 3:
		result = RC_CTRLSHIFT;
		break;
	default:
		result = 0;
		break;
	}

	form->OnKey(wParam, result);
	switch (result)
	{
	case RC_CHANGED:
		form->IsChanged();
		if (m_client->m_vm->IsAvailable(screen, evChange, form))
			m_client->m_vm->OnChange(screen, form);
		return;
	case RC_ITEM:
		if (m_client->m_vm->IsAvailable(screen, evClick, form))
		{
			m_client->m_vms.scroll = SB_IM;
			m_client->m_vm->OnClick(screen, form);
			m_client->m_vms.scroll = SB_NS;
		}
		return;
	case RC_NOP:
// updateXXX_2022XX	
		switch (wParam)
		{
		case VK_BACK:
		case VK_DELETE:
			if (m_client->m_vm->IsAvailable(screen, evChar, form))
				m_client->m_vm->OnCharChange(screen, form);
			if (wParam == VK_BACK)
				m_client->AutoHistory(form);
			break;
		default:
			break;
		}
		return;
	case RC_SEARCH:
		m_client->SelfHistory(form);
		return;
	case RC_SHIFT:
		m_client->m_cbox = m_client->m_current;
		m_client->m_status |= S_SHIFT;
		break;
	case RC_CTRLSHIFT:
		m_client->m_cbox = m_client->m_current;
		m_client->m_status |= S_SHIFT|S_CONTROL;
		break;
	case RC_CONTROL:
		m_client->m_cbox = m_client->m_current;
		m_client->m_status |= S_CONTROL;
		break;
	default:
		if (wParam == VK_BACK)
			m_client->History(false);
		break;
	}
	screen->OnClick(form, result);
}

bool CKey::OnKeyCell(CScreen* screen, CfmBase* form, WPARAM wParam)
{
	m_client->History(false);
	if (form != NULL && form->m_form->kind == FM_GRID)
	{
		int	result = 0;

		if (GetKeyState(VK_SHIFT) & 0x8000)
			result = RC_SHIFT;
		form->OnKey(wParam, result);

		switch (result)
		{
		case RC_CHANGED:
			form->IsChanged();
			if (m_client->m_vm->IsAvailable(screen, evChange, form))
				m_client->m_vm->OnChange(screen, form);
			[[fallthrough]];
		case RC_OK:
			return true;
		default:
			break;
		}
	}
	return false;
}

void CKey::OnIME(WPARAM wParam, LPARAM lParam)
{
	if (wParam && lParam & GCS_DELTASTART)
	{
		OnKey(VK_DELETE, 0);
		return;
	}

//	if (m_client->m_status & S_WAIT)
	if (m_guard->IsWait(m_client))
	{
		MessageBeep(MB_OK);
		return;
	}

	if (m_client->m_status & (S_SHIFT|S_CONTROL) &&
		(m_client->m_cbox.m_key != m_client->m_current.m_key ||
		m_client->m_cbox.m_idx != m_client->m_current.m_idx))
		m_client->ClearShift();

	bool moving = (lParam & GCS_RESULTSTR) ? true : false;
	if (wParam == 0 && !moving)
	{
		OnKey(VK_DELETE, 0);
		return;
	}

	CScreen	*screen{};
	CfmBase* form{};
	if (!SetOnKeys(screen, form, true) || (form && !form->IsEnable()))
		return;

	OnChar(screen, form, wParam, moving);
}

void CKey::OnChar(WPARAM wParam, LPARAM lParam)
{
	if (!(lParam & 0xff0000))
		return;				// Specifies the scan code, ignore IME code

	switch (wParam)
	{
	case VK_TAB:
	case VK_RETURN:
		return;
	default:
//		if (m_client->m_status & S_WAIT)
		if (m_guard->IsWait(m_client))
		{
			MessageBeep(MB_OK);
			return;
		}
		break;
	}

	if (m_client->m_status & (S_SHIFT|S_CONTROL) &&
		(m_client->m_cbox.m_key != m_client->m_current.m_key ||
		m_client->m_cbox.m_idx != m_client->m_current.m_idx))
		m_client->ClearShift();

	CScreen	*screen;
	CfmBase* form;
	int	ii, result;

	if (!SetOnKeys(screen, form, false) || (form && !form->IsEnable()))
		return;

	if (wParam > 0xa0 || wParam < 0x20)
	{
		Copy$Paste(wParam, screen, form);
		return;
	}

	switch ((lParam >> 16) & 0xff)
	{
	case 0x53:
		m_guard->m_status &= ~WS_GUIDE;
		if (form->IsAutoThousand())
		{
			for (ii = 0; ii < 3; ii++)
			{
				if (m_client->m_status & S_INS)
					form->InsertData('0', true, result);
				else
					form->UpdateData('0', true, result);

				switch (result)
				{
				case RC_FULL:
					if (m_client->m_vm->IsAvailable(screen, evChar, form))
						m_client->m_vm->OnCharChange(screen, form);
					OnChange(screen, m_client->m_current.m_idx, true);
					break;
				case RC_CHANGED:
					if (form->IsChanged() && m_client->m_vm->IsAvailable(screen, evChange, form))
						m_client->m_vm->OnChange(screen, form);
					if (m_client->m_vm->IsAvailable(screen, evChar, form))
						m_client->m_vm->OnCharChange(screen, form);
					break;
				case RC_OVERFLOW:
				case RC_NODELETE:
					break;
				default:
					if (m_client->m_vm->IsAvailable(screen, evChar, form))
						m_client->m_vm->OnCharChange(screen, form);
					break;
				}
			}
			m_client->History(false);
		}
		else
			OnChar(screen, form, wParam, true);
		break;

	case 0x4a:
	case 0x4e:
	case 0x37:
	case 0x35:
		if (form->OnKey(wParam, result) > 0)
			return;
		if (m_client->isKeyButton(wParam))
			return;
	default:
		m_guard->m_status &= ~WS_GUIDE;
		OnChar(screen, form, wParam, true);
		break;
	}

	if (m_guard->m_status & WS_GUIDE)
	{
		m_guard->m_status &= ~WS_GUIDE;
		return;
	}
	m_guard->SetGuide(AE_CLEAR, m_client->m_key);
}

void CKey::OnChar(CScreen* screen, CfmBase* form, int key, bool moving)
{
	int	result;
	bool	change = false;

	if (m_client->m_status & S_INS)
		form->InsertData(key, moving, result);
	else
		form->UpdateData(key, moving, result);

	switch (result)
	{
	case RC_NOP:
		if (m_client->m_vm->IsAvailable(screen, evChar, form))
			m_client->m_vm->OnCharChange(screen, form);
		m_client->AutoHistory(form);
		return;
	case RC_SEARCH:
		m_client->SelfHistory(form);
		return;
	case RC_CHANGED:
		if (m_client->m_vm->IsAvailable(screen, evChar, form))
			m_client->m_vm->OnCharChange(screen, form);
		if (form->IsChanged() && m_client->m_vm->IsAvailable(screen, evChange, form))
			m_client->m_vm->OnChange(screen, form);
		break;
	case RC_OVERFLOW:
	case RC_NODELETE:
		break;
	case RC_FULL:
		if (m_client->m_vm->IsAvailable(screen, evChar, form))
			m_client->m_vm->OnCharChange(screen, form);
		[[fallthrough]];
	case RC_CBCLOSE:
		if (form->m_form->kind == FM_COMBO && form->m_form->properties & PR_ACCN && !(m_guard->m_term & flagACN))
		{
			CString	data, text;

			form->ReadData(data);
			text = CString((char *)form->m_form->name) + '\t' + data;
			screen->m_guard->SetCombo(text);
		}
		OnChange(screen, m_client->m_current.m_idx, true);
		break;
	default:
		if (m_client->m_vm->IsAvailable(screen, evChar, form))
			m_client->m_vm->OnCharChange(screen, form);
		break;
	}

	m_client->History(false);
}

bool CKey::SetOnKeys(CScreen*& screen, CfmBase*& form, bool reset)
{
	if (!m_client->GetScreen(screen))
	{
		screen = NULL;
		return false;
	}

	if (m_client->m_current.m_idx < 0)
	{
		form = NULL;
		return false;
	}
	
	form = screen->GetAtForm(m_client->m_current.m_idx);
	m_client->m_mouse->m_screen = screen;
	m_client->m_mouse->m_mouse  = m_client->m_current;
	if (reset)
		m_guard->SetGuide(AE_CLEAR, m_client->m_key);
	return true;
}

bool CKey::OnKey(CScreen* screen, WPARAM wParam, CfmBase* form)
{
	if (::GetKeyState(VK_CONTROL) < 0 && ::GetKeyState(VK_SHIFT) < 0)
		m_client->m_vms.shift = SS_CSS;
	else if (::GetKeyState(VK_CONTROL) < 0)
		m_client->m_vms.shift = SS_CS;
	else if (::GetKeyState(VK_SHIFT) < 0)
		m_client->m_vms.shift = SS_SS;
	else
		m_client->m_vms.shift = SS_NS;

	m_client->m_vms.key = wParam;

	if (form != NULL)
	{
		if (form->IsEnable() && m_client->m_vm->IsAvailable(screen, evKey, form))
		{
			m_client->m_vm->OnKey(screen, form);
			return true;
		}
		return false;
	}

	if (m_client->m_vm->IsAvailable(screen, evKey))
		m_client->m_vm->OnKey(screen);

	return true;
}

bool CKey::isKoscom(WPARAM wParam)
{
	switch (wParam)
	{
	case VK_F1:			// REGS
		break;
	case VK_F2:			// INQY
		break;
	case VK_F3:			// MODY
		break;
	case VK_F4:			// CANC
		break;
	case VK_F5:			// CONF
		break;
	case VK_F6:			// PREVS
		break;
	case VK_F7:			// CONT
		break;
	default:
		return false;
	}
	return true;
}

bool CKey::OnChange(CScreen* screen, int ev, bool byKey)
{
	int	guide;
	CfmBase* form = screen->GetAtForm(ev);

	m_client->m_status &= ~S_CURSOR;
	if (!(form->m_form->attr & FA_VALIDX) && !form->IsValid(guide))
	{
		m_guard->SetGuide(guide, m_client->m_key);
		return false;
	}

	m_client->m_status &= ~S_CURSOR;
	if (byKey && !(form->m_form->attr & FA_SEND))
	{
		if (!screen->OnChange(ev, false))
			return false;
	}
	else if (!screen->OnChange(ev, byKey))
			return false;


	if (!byKey)
	{
		if (m_client->m_status & S_CURSOR)
		{
			m_client->m_status &= ~S_CURSOR;
			return false;
		}
		return true;
	}

	if (!(m_client->m_status & S_CURSOR))
	{
		if (form->m_form->attr & FA_WRAP)
			GoNextForm(false);
	}
	m_client->m_status &= ~S_CURSOR;
	m_client->m_view->UpdateWindow();

	if (form->m_form->attr & FA_SEND)
	{
		if ((form->m_form->attr2 & SA_SEND) == SA_THIS)
			m_client->m_stream->InStream(screen, byKey);
		else
			m_client->m_stream->InStream(byKey);
	}
	return true;
}

void CKey::GoNextForm(bool byKey)
{
	CScreen* screen;
	CScreen* prevs;
	CfmBase* form;

	if (!SetOnKeys(screen, form, false) ||!screen || !form)
		return;

	if (byKey && !OnChange(screen, m_client->m_current.m_idx, false))
		return;

	int	idx;
	if (screen->m_mapH->options & OP_INTER)
	{
		idx = NextForm(screen, form, false);
		if (idx != -1)
			m_client->SetCPos(screen, idx, false);
		return;
	}

	int	tabX;
	int tab = form->m_form->index + 1;

	for (int ii = 0; ii < m_client->m_magic; )
	{
		tabX = NextForm(screen, tab, false);
		switch (tabX)
		{
		case FMX_OK:
			break;
		case FMX_OB:
			prevs = screen;
			if (!m_client->GetAtScreen(screen, screen->m_formR[screen->m_tabR[tab]].keys))
			{
				ii++;	tab++;
				continue;
			}
			m_client->GetKeyObject(screen, form);
			if (prevs->IsFormVisible(form) && form->IsVisible())
			{
				tab = 0;
				continue;
			}
			[[fallthrough]];
		case FMX_END:
			ii++;
			m_client->GetNextScreen(screen, tab);
			continue;
		}

		m_client->SetCPos(screen, screen->m_tabR[tab], false);
		return;
	}
}

void CKey::GoPrevForm()
{
	CScreen* screen;
	CScreen* prevs;
	CfmBase* form;

	if (!SetOnKeys(screen, form, false))
		return;

	if (!OnChange(screen, m_client->m_current.m_idx, false))
		return;

	int	idx;
	if (screen->m_mapH->options & OP_INTER)
	{
		idx = NextForm(screen, form, true);
		if (idx != -1)
			m_client->SetCPos(screen, idx, false);
		return;
	}

	int	tabX;
	int tab = form->m_form->index - 1;

	for (int ii = 0; ii < m_client->m_magic; )
	{
		tabX = NextForm(screen, tab, true);
		switch (tabX)
		{
		case FMX_OK:
			break;
		case FMX_OB:
			prevs = screen;
			if (!m_client->GetAtScreen(screen, screen->m_formR[screen->m_tabR[tab]].keys))
			{
				ii++;	tab--;
				continue;
			}
			m_client->GetKeyObject(screen, form);
			if (prevs->IsFormVisible(form) && form->IsVisible())
			{
				tab = screen->m_tabL - 1;
				continue;
			}
			[[fallthrough]];
		case FMX_END:
			ii++;
			m_client->GetPrevScreen(screen, tab);
			continue;
		}

		m_client->SetCPos(screen, screen->m_tabR[tab], false);
		return;
	}
}

int CKey::NextForm(CScreen* screen, int& tab, bool back)
{
	int	idx;
	CfmBase* form;

	for (int ii = 0; ii < screen->m_tabL; ii++)
	{
		if (tab < 0)
		{
			if (screen->isMain())
				tab = screen->m_tabL - 1;
			else
				return FMX_END;
		}
		else if (tab >= screen->m_tabL)
		{
			if (screen->isMain())
				tab = 0;
			else
				return FMX_END;
		}
		idx = screen->m_tabR[tab];

		form = screen->GetAtForm(idx);
		if (!form) continue;

		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			return FMX_OB;
		case FM_BUTTON:
			switch (form->m_form->type)
			{
			case BTN_PUSH:
			case BTN_USER:
				if (form->m_form->properties & PR_CHECKED && inFocus(screen, form))
					return FMX_OK;
				break;
			case BTN_RADIO:
				if (!(form->m_form->properties & PR_CHECKED))
					break;
				[[fallthrough]];
			case BTN_CHECK:
				switch (form->m_form->iok)
				{
				case EIO_INPUT:
				case EIO_INOUT:
					if (inFocus(screen, form))
						return FMX_OK;
					break;
				}
				break;
			}
			break;
		case FM_MEMO:
		case FM_GRID:
		case FM_CONTROL:
			if (form->m_form->iok == EIO_OUTPUT)
				break;
			[[fallthrough]];
		case FM_EDIT:
		case FM_COMBO:
			if (inFocus(screen, form))
				return FMX_OK;
			break;
		default:
			break;
		}
		if (back)
			tab--;
		else
			tab++;
	}

	return FMX_END;
}

int CKey::NextForm(CScreen* screen, CfmBase* form, bool back)
{
	int	idx, tab;

	tab = form->m_form->index;
	if (back)
		tab--;
	else
		tab++;

	for (int ii = 0; ii < screen->m_tabL; ii++)
	{
		if (tab < 0)
			tab = screen->m_tabL - 1;
		else if (tab >= screen->m_tabL)
			tab = 0;
		idx = screen->m_tabR[tab];

		form = screen->GetAtForm(idx);
		if (!form) continue;

		switch (form->m_form->kind)
		{
		case FM_BUTTON:
			switch (form->m_form->type)
			{
			case BTN_PUSH:
			case BTN_USER:
				if (form->m_form->properties & PR_CHECKED && inFocus(screen, form))
					return idx;
				break;
			case BTN_RADIO:
				if (!(form->m_form->properties & PR_CHECKED))
					break;
				[[fallthrough]];
			case BTN_CHECK:
				switch (form->m_form->iok)
				{
				case EIO_INPUT:
				case EIO_INOUT:
					if (inFocus(screen, form))
						return idx;
					break;
				}
				break;
			}
			break;
		case FM_MEMO:
		case FM_GRID:
		case FM_CONTROL:
			if (form->m_form->iok == EIO_OUTPUT)
				break;
			[[fallthrough]];
		case FM_EDIT:
		case FM_COMBO:
			if (inFocus(screen, form))
				return idx;
			[[fallthrough]];
		default:
			break;
		}
		if (back)
			tab--;
		else
			tab++;
	}

	return -1;
}

bool CKey::inFocus(CScreen* screen, CfmBase* form)
{
	if (!screen->IsFormVisible(form) || !form->IsVisible() || !form->IsEnable())
		return false;
	if (form->m_form->attr & (FA_PROTECT|FA_PASS))
		return false;
	return true;
}

void CKey::OnHome(bool start)
{
	CfmBase* form;
	CScreen* screen;
	CScreen* prevs;

	int	tab, type;

	if (!m_client->GetAtScreen(screen))
		return;

	tab = 0;
	for (int ii = 0; ii < m_client->m_magic; )
	{
		type = NextForm(screen, tab, false);
		switch (type)
		{
		case FMX_OK:
			break;
		case FMX_OB:
			prevs = screen;
			if (!m_client->GetAtScreen(screen, screen->m_formR[screen->m_tabR[tab]].keys))
			{
				ii++;	tab++;
				continue;
			}
			m_client->GetKeyObject(screen, form);
			if (prevs->IsFormVisible(form) && form->IsVisible())
			{
				tab = 0;
				continue;
			}
			[[fallthrough]];
		case FMX_END:
			ii++;
			m_client->GetNextScreen(screen, tab);
			continue;
		}

		if (start)
		{
			m_client->m_default.m_key = screen->m_key;
			m_client->m_default.m_idx = screen->m_tabR[tab];
		}
		else
		{
			if (SetOnKeys(prevs, form, false))
				OnChange(prevs, m_client->m_current.m_idx, false);
			m_client->SetCPos(screen, screen->m_tabR[tab], false);
		}
		return;
	}
}

void CKey::Copy$Paste(WPARAM wParam, CScreen* screen, CfmBase* form)
{
	CString	text;
	int	row, col;

	switch (form->m_form->kind)
	{
	case FM_EDIT:
// updateXXX_2022XX	
		if (form->m_form->attr & FA_PASSWD)
			return;
		row = col = 0;
		break;
	case FM_GRID:
		col = 0;
		form->GetEnum(text, col, 0);
		row = LOWORD(col);
		col = HIWORD(col);
		if (col > 0 && row > 0)
		{
			if (!(form->m_form->attr & FA_COMMA))
			{
				struct	_cellR* cell = (struct _cellR *)&screen->m_cellR[form->m_form->vals[0]];
				if (cell[col-1].attr & FA_PASSWD)
					return;
			}
			break;
		}
		[[fallthrough]];
	default:
		return;
	}

	row--; col--;
	switch (wParam)
	{
	case 3:			// Ctrl+C
	case 24:		// Ctrl+X
		form->ReadData(text, false, -1, col, row);
		text.TrimRight();
		if (!text.IsEmpty())
		{
			if (wParam == 24)
				form->WriteData(_T(""), true, col, row);
			m_guard->SetClipboard(text);
		}
		break;
	case 22:		// Ctrl+V
		if (m_guard->GetClipboard(text))
		{
			form->WriteData(text, true, col, row);
			form->SetFocus(true);
			m_client->m_vm->OnProcedure(screen, "OnPaste");
		}
		break;
	default:
		return;
	}
}
