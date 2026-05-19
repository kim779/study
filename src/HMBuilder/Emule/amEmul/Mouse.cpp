// Mouse.cpp: implementation of the CMouse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mouse.h"
#include "Client.h"
#include "Script.h"
#include "Stream.h"
#include "Key.h"
#include "../dll/form/amgrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMouse::CMouse(CClient* client)
{
	m_client = client;
	m_guard  = client->m_guard;

	m_caret.m_key = -1;
	m_caret.m_idx = -1;

	m_capture = false;
	m_repeat  = -1;
	m_type    = MS_NB;

	m_mouse.m_key = -1;
	m_mouse.m_idx = -1;

	m_hover.m_key = -1;
	m_hover.m_idx = -1;
}

CMouse::~CMouse()
{
}

void CMouse::OnDown(CPoint point, WPARAM wParam)
{
	if (m_capture)	return;

	if (m_client->m_status & S_WAIT)
	{
		m_type = MS_NB;
		m_mouse.m_idx = -1;
		return;
	}

	m_capture = true;
	switch (wParam & (MK_LBUTTON|MK_MBUTTON|MK_RBUTTON))
	{
	case MK_MBUTTON:
		m_type = MS_MB;
		break;
	case MK_RBUTTON:
		m_type = MS_RB;
		break;
	case MK_LBUTTON:
	default:
		m_type = MS_LB;
		break;
	}
	m_client->m_vms.button = m_type;

	int	shift;
	switch (wParam & (MK_CONTROL|MK_SHIFT))
	{
	case MK_CONTROL|MK_SHIFT:
		shift = RC_CTRLSHIFT;
		m_client->m_vms.shift = SS_CSS;
		break;
	case MK_SHIFT:
		shift = RC_SHIFT;
		m_client->m_vms.shift = SS_SS;
		break;
	case MK_CONTROL:
		shift = RC_CONTROL;
		m_client->m_vms.shift = SS_CS;
		break;
	default:
		shift = 0;
		m_client->m_vms.shift = SS_NS;
		break;
	}

	switch (m_type)
	{
	case MS_LB:
		if (L_Down(point, shift) == mouseERR)
		{
			m_capture = false;
			return;
		}
		break;
	case MS_RB:
	case MS_MB:
		if (X_Down(point) == mouseERR)
		{
			m_capture = false;
			return;
		}
		break;
	default:
		m_capture = false;
		return;
	}

	if (m_capture)
		SetCapture(m_client->m_view->m_hWnd);
	else if (m_mouse.m_idx != -1)
	{
		CamBase* form;
		form = m_screen->GetAtForm(m_mouse.m_idx);
		form->OnLButton(false, point, shift);
	}
	
	
}

void CMouse::OnUp(CPoint point, WPARAM wParam)
{
	if (!m_capture)	return;

	::ReleaseCapture();
	m_capture = false;

	switch (m_type)
	{
	case MS_LB:
		if (L_Up(point) == mouseIN && !(m_client->m_status & S_WAIT))
			OnClick();
		break;
	case MS_MB:
	case MS_RB:
		X_Up(point);
		break;
	default:
		break;
	}
}

bool CMouse::OnRepeat(CPoint point)
{
	if (!m_capture || m_repeat < 0 || m_client->m_status & (S_WAIT|S_LOCK))
	{
		m_repeat = -1;
		return false;
	}

	CScreen	*screen;
	point  += m_client->m_offset;

	if (!m_client->GetAtScreen(screen, m_mouse.m_key))
	{
		m_repeat = -1;
		return false;
	}
	if (m_mouse.m_idx < 0)
	{
		m_repeat = -1;
		return false;
	}

	int	result;
	bool	rc;

	CamBase *form = screen->GetAtForm(m_mouse.m_idx);
	rc = form->OnRepeat(point, result);
	switch (result)
	{
	case RC_SPIN:
		if (form->IsChanged() && m_client->m_vm->IsAvailable(m_screen, evChange, form))
			m_client->m_vm->OnChange(m_screen, form);
	default:
		break;
	}
	return rc;
}

void CMouse::OnDblClick(CPoint point, WPARAM wParam)
{
	if (m_client->m_status & S_WAIT)
		return;

	int idx = WhichForm(point);
	if (idx < 0)
		return;

	CamBase *form = m_screen->GetAtForm(idx);
	if (!form->IsVisible() || !IsEnable(form))
		return;

	if (m_client->m_current == m_mouse)
	{// if cursor position force moved, ignore DblClick action for cursor position 041221
		switch (form->m_form->kind)
		{
		case FM_COMBO:
			ComboBox(point, true, false);
			break;
		case FM_GRID:
		case FM_GRIDEX:
		//case FM_MEMO:
			if (form->m_form->iok == EIO_OUTPUT)
				break;
		case FM_EDIT:
//		case FM_TREE:
			form->SetFocus(true);
			break;
		}
	}

	int	result;
	form->OnDblClick(point, result);

	switch (result)
	{
	case RC_CHANGED:
		form->IsChanged();
		if (m_client->m_vm->IsAvailable(m_screen, evChange, form))
			m_client->m_vm->OnChange(m_screen, form);
		return;
	case RC_HEAD:
		m_client->m_vms.scroll = SB_HD;
		break;
	case RC_FOLDER:
		m_client->m_vms.scroll = SB_FD;
		break;
	case RC_ITEM:
		m_client->m_vms.scroll = SB_IM;
		break;
	case RC_NOP:
		if (!IsSelf(form))
			break;
		if (m_client->m_current == m_mouse)
		{
			m_client->m_cbox.m_idx = idx;
			m_client->m_cbox.m_key = m_screen->m_key;
		}
		break;
	default:
		m_client->m_vms.scroll = SB_NS;
		break;
	}

	if (m_client->m_vm->IsAvailable(m_screen, evDblClk, form))
		m_client->m_vm->OnDblClick(m_screen, form);
}

bool CMouse::OnWheel(CPoint point, WPARAM wParam)
{
	int	idx;
	bool	fBool = false;
	CScreen	*screen = m_screen;

	if (m_client->m_status & S_WAIT)
		return false;

	int	delta = (short)HIWORD(wParam) / 120;

	if (m_client->m_status & S_LOCK)
		return false;
	idx = WhichForm(point);

	if (screen == NULL || idx < 0)
		return false;

	int	result;
	CamBase *form = screen->GetAtForm(idx);

	fBool = form->OnMouseWheel(delta, result);
	screen->OnClick(form, result);

	return fBool;
}

void CMouse::OnMove(CPoint point, WPARAM wParam)
{
	if (m_client->m_status & S_WAIT)
		return;

	int	result;
	CamBase *form;
	CScreen	*screen;
	result = RC_NOP;

	if (!m_capture)
	{
		if (m_hover.m_key >= 0 && m_hover.m_idx >= 0)
		{
			if (m_screen->m_key != m_hover.m_key)
			{
				if (!m_client->GetAtScreen(screen, m_hover.m_key))
					return;
			}
			else
				screen = m_screen;

			form = screen->GetAtForm(m_hover.m_idx);
			if (form)
			{
				form->OnDrag(point, result);
				if (result == RC_HOVER)
					return;
			}
		}

		int idx = WhichForm(point);
		if (idx != -1)
		{
			form = m_screen->GetAtForm(idx);
			form->OnDrag(point, result);
			if (result == RC_HOVER)
			{
				m_hover.m_key = m_screen->m_key;
				m_hover.m_idx = idx;
			}
			else
			{
				m_hover.m_key = -1;
				m_hover.m_idx = -1;
			}
		}
		return;
	}

	if (m_mouse.m_idx < 0 || !m_client->GetAtScreen(screen, m_mouse.m_key))
		return;

	form = screen->GetAtForm(m_mouse.m_idx);
	if (form)
	{
		form->OnDrag(point, result);
		point  -= m_client->m_offset;
		m_point = point;
		screen->OnClick(form, result);
	}
}

mouse_t CMouse::L_Down(CPoint& point, int shift)
{
	m_mouse.m_idx = -1;
	m_mouse.m_key = m_screen->m_key;

	int idx = WhichForm(point);
	if (idx < 0)
	{
		if (m_client->m_status & (S_SHIFT|S_CONTROL))
			m_client->ClearShift();
		return mouseNO;
	}

	if (m_client->m_status & (S_SHIFT|S_CONTROL) &&
		(m_client->m_cbox.m_key != m_screen->m_key || m_client->m_cbox.m_idx != idx))
		m_client->ClearShift();

	CamBase* form = m_screen->GetAtForm(idx);
	if (!form->IsVisible() || !IsEnable(form))
		return mouseERR;

	bool	focus = true;
	switch (form->m_form->kind)
	{
	case FM_BUTTON:
	case FM_CHECK:
	case FM_RADIO:
		switch (form->m_form->type)
		{
		case BTN_PUSH:
		case BTN_USER:
			if (!(form->m_form->properties & PR_CHECKED))
				focus = false;
			break;
		case BTN_RADIO:
			break;
		default:
			break;
		}
	/*case FM_MEMO:
		if (form->m_form->iok == EIO_OUTPUT)
		{
			form->OnLButton(true, point, shift);
			m_mouse.m_idx = idx;
			break;
		}*/
//	case FM_TREE:
	case FM_COMBO:
		m_mouse.m_idx = idx;
		if (focus && m_mouse != m_client->m_current)
		{
			m_client->OnFocus(m_screen, idx, true);
			if (m_client->m_status & S_WAIT)
				return mouseERR;
		}

		if (form->m_form->kind == FM_COMBO)
		{
			m_client->m_cbox.m_key = m_mouse.m_key;
			m_client->m_cbox.m_idx = m_mouse.m_idx;
			ComboBox(point, true);
			shift = 0;
			break;
		}
		form->OnLButton(true, point, shift);
		break;

	case FM_EDIT:
		m_mouse.m_idx = idx;
		form->OnLButton(true, point, shift);
		if (focus && m_mouse != m_client->m_current)
			m_client->OnFocus(m_screen, idx, true);
		break;

	case FM_GRID:
	case FM_GRIDEX:
		if (focus && ((CamGrid *)form)->GetValidRows() > 0
			&& (form->m_form->iok == EIO_INPUT || form->m_form->iok == EIO_INOUT))
		{
			m_mouse.m_idx = idx;
			if (m_mouse == m_client->m_current)
				m_client->OnChange(m_screen);
			form->OnLButton(true, point, shift);
			if (m_mouse != m_client->m_current)
			{
				m_client->OnChange(m_screen);
				m_client->OnFocus(m_screen, idx, true);
			}
			break;
		}
//	case FM_SHEET:
	case FM_OUT:
	case FM_LABEL:
	case FM_IMAGEVW:
	case FM_TABLE:
	case FM_BROWSER:
		form->OnLButton(true, point, shift);
		m_mouse.m_idx = idx;
		break;
		/*
	case FM_TAB:
		form->OnLButton(true, point, shift);
		if (shift >= 0)
		{
			if (!m_client->isVisible(m_client->m_current.m_key, m_client->m_current.m_idx))
				m_client->m_keyx->OnHome(false);

			m_mouse.m_idx = idx;
			if (!OnClick())
				break;
			m_mouse.m_idx = -1;
			m_type = MS_NB;
			return mouseNO;
		}
		m_mouse.m_idx = idx;
		return mouseIN;
		*/
	case FM_TAB:
		form->OnLButton(true, point, shift);
		if (shift >= 0)
		{
			m_mouse.m_idx = idx;
			OnClick();

			m_mouse.m_idx = -1;
			m_type = MS_NB;
			return mouseNO;
		}
		m_mouse.m_idx = idx;
		return mouseIN;
	default:
		return mouseERR;
	}
	m_client->m_status &= ~(S_SHIFT|S_CONTROL);

	CString	tmps;
	switch (shift)
	{
	case RC_CHANGED:
		m_client->m_vms.scroll = SB_IM;
		form->IsChanged();
		if (m_client->m_vm->IsAvailable(m_screen, evChange, form))
			m_client->m_vm->OnChange(m_screen, form);
		return mouseNO;
	case RC_SCRUP:
	case RC_SCRDN:
	case RC_PAGEUP:
	case RC_PAGEDN:
	case RC_FIRST:
		m_screen->OnClick(form, shift);
		return mouseNO;

	case RC_SPIN:
		if (form->IsChanged() && m_client->m_vm->IsAvailable(m_screen, evChange, form))
			m_client->m_vm->OnChange(m_screen, form);
	case RC_UP:
	case RC_DN:
	case RC_LEFT:
	case RC_RIGHT:
		m_repeat = 0;
		point -= m_client->m_offset;
		m_point = point;
		m_client->m_view->KillTimer(TM_REPBN);
		m_client->m_view->SetTimer(TM_REPBN, 100, NULL);
		m_client->m_vms.scroll = SB_NS;
		return mouseIN;

	case RC_ITEM:
		m_client->m_vms.scroll = SB_IM;
		switch (form->m_form->kind)
		{
		case FM_GRID:
		case FM_GRIDEX:
			if (m_mouse != m_client->m_current && form->m_form->isEdit != FI_NONE)
				m_client->SetCPos(m_screen, idx);
			break;
		case FM_TABLE:
			m_client->SetCPos(m_screen, idx);
		default:
			break;
		}
		break;
	case RC_HEAD:
		m_client->m_vms.scroll = SB_HD;
		switch (form->m_form->kind)
		{
		case FM_TABLE:
			m_client->SetCPos(m_screen, idx);
		default:
			break;
		}
		return mouseIN;
	case RC_FOLDER:
		m_client->m_vms.scroll = SB_FD;
		return mouseIN;
	case RC_SHIFT:
		m_client->m_cbox.m_idx = idx;
		m_client->m_cbox.m_key = m_screen->m_key;
		m_client->m_status |= S_SHIFT;
		break;
	case RC_CTRLSHIFT:
		m_client->m_cbox.m_idx = idx;
		m_client->m_cbox.m_key = m_screen->m_key;
		m_client->m_status |= S_SHIFT|S_CONTROL;
		break;
	case RC_CONTROL:
		m_client->m_cbox.m_idx = idx;
		m_client->m_cbox.m_key = m_screen->m_key;
		m_client->m_status |= S_CONTROL;
		break;
	case RC_SEND:
		m_screen->m_trxK = 0;
		m_client->m_stream->InStream("", m_screen, true);
		return mouseIN;
	case RC_OUT:
		m_client->m_vms.scroll = SB_NS;
		return mouseOUT;
	default:
		m_client->m_vms.scroll = SB_NS;
		if (form->m_form->kind == FM_GRID || form->m_form->kind == FM_GRIDEX)
			return mouseIN;
		break;
	}

	if (/*!(form->m_form->attr & FA_DRAG) || */m_client->m_vm->IsAvailable(m_screen, evClick, form))
		return mouseIN;

	CString	text;
	int	row, col;
//	struct _precellR	*cell;
	m_guard->m_dragbys.Empty();

	switch (form->m_form->kind)
	{
	case FM_GRID:
	case FM_GRIDEX:
		row = ((CamGrid *)form)->GetCellRow();
		if (((CamGrid *)form)->GetValidRows() < row)
			return mouseIN;

		col = ((CamGrid *)form)->GetCellColumn();
		if (--col < 0 || --row < 0)
			return mouseIN;

//		cell = (struct _precellR *) &m_screen->m_cellR[form->m_form->vals[0]];

/*		if (!(cell[col].attr & FA_DRAG))
			return mouseIN;
*/
		return mouseIN;
		
		//if (/*cell[col].attr & FA_REF &&*/ cell[col].vals[0] != NOVALUE)
		//	col = cell[col].vals[0];

		//form->ReadData(text, false, col, row);
		//text.TrimRight();
		//if (text.IsEmpty())
		//	return mouseIN;

		//m_guard->m_dragbys = form->GetSymbolName(cell[col].name);
		//m_guard->m_dragbys += "\t" + text;
		break;
/*
	case FM_TREE:
		form->ReadAll(text);
		if (text.GetLength() < form->m_form->size)
			return mouseIN;

		m_guard->m_dragbys = form->GetSymbolName();
		while (true)
		{
			m_guard->m_dragbys += '\t';
			m_guard->m_dragbys += text.Left(form->m_form->size);
			text = text.Mid(form->m_form->size);
			if (text.GetLength() < form->m_form->size)
				break;
		}
		break;

	case FM_TAB:
		if (shift >= 0 && form->m_form->attr & FA_DRAG)
		{
			form->ReadData(m_guard->m_dragbys);
			m_guard->m_dragbys.Insert(L_MAPN, '\t');
			break;
		}
		return mouseIN;
*/
	default:
		/*
		if (/*form->m_form->attr & FA_REF && form->m_form->vals[2] != NOVALUE)
		{
			idx = form->m_form->vals[2];
			form = m_screen->GetAtForm(idx);
		}
		*/
		form->ReadData(text);
		text.TrimRight();
		if (true/*text.IsEmpty()*/)
			return mouseIN;

		m_guard->m_dragbys = form->GetSymbolName();
		m_guard->m_dragbys += "\t" + text;
		break;
	}

	m_client->Register(NULL, true);
	m_guard->OnDrag(m_client->m_key, m_guard->m_dragbys);
	return mouseERR;
}

mouse_t CMouse::X_Down(CPoint& point)
{
	if (m_client->m_status & (S_SHIFT|S_CONTROL))
		m_client->ClearShift();

	m_mouse.m_idx = -1;

	m_mouse.m_key = m_screen->m_key;
	m_mouse.m_idx = WhichForm(point);

	if (m_mouse.m_idx < 0)
		return mouseNO;

	CamBase *form = m_screen->GetAtForm(m_mouse.m_idx);
	if (!form->IsVisible() || !IsEnable(form))
		return mouseERR;

	int	result;
	if (m_type == MS_RB)
		form->OnRButton(true, point, result);
	else
		form->OnMButton(true, point, result);

	switch (result)
	{
	case RC_SCRUP:
	case RC_SCRDN:
		m_client->m_vms.scroll = (result == RC_SCRUP) ? SB_UP : SB_DN;
		break;
	case RC_PAGEUP:
	case RC_PAGEDN:
		m_client->m_vms.scroll = (result == RC_PAGEUP) ? SB_PU : SB_PD;
		break;
	case RC_LEFT:
	case RC_RIGHT:
	case RC_UP:
	case RC_DN:
		m_client->m_vms.scroll = SB_NS;
		break;
	case RC_ITEM:
		m_client->m_vms.scroll = SB_IM;
		break;
	case RC_HEAD:
		m_client->m_vms.scroll = SB_HD;
		break;
	default:
		m_client->m_vms.scroll = SB_NS;
		break;
	}
	return mouseIN;
}

mouse_t CMouse::L_Up(CPoint& point)
{
	int	result = 0;

	if (m_mouse.m_idx < 0)
		return mouseERR;

	CamBase	*form;
	if (m_mouse.m_key != m_screen->m_key)
	{
		CScreen *screen;
		m_client->GetAtScreen(screen, m_mouse.m_key);
		if (screen && m_mouse.m_idx < screen->m_mapH->formN)
		{
			form = screen->GetAtForm(m_mouse.m_idx);
			form->OnLButton(false, point, result);
		}
		return mouseNO;
	}

	int idx = WhichForm(point);
	if (idx < 0 || m_mouse.m_idx != idx)
	{
		form = m_screen->GetAtForm(m_mouse.m_idx);
		form->OnLButton(false, point, result);
		return mouseOUT;
	}

	form = m_screen->GetAtForm(idx);
	switch (form->m_form->kind)
	{
//	case FM_TAB:
	case FM_TAB:
		form->OnLButton(false, point, result);
		return mouseNO;
	case FM_BUTTON:
	case FM_CHECK:
	case FM_RADIO:
	case FM_EDIT:
	//case FM_MEMO:
	case FM_GRID:
	case FM_GRIDEX:
	case FM_OUT:
	case FM_COMBO:
	case FM_LABEL:
	case FM_IMAGEVW:
	case FM_TABLE:
//	case FM_TREE:
//	case FM_SHEET:
	case FM_BROWSER:
		form->OnLButton(false, point, result);
		break;
	default:
		return mouseERR;
	}

	switch (result)
	{
	case RC_NOP:
		if (!IsSelf(form))
			break;
	case RC_CODE:
		m_client->m_cbox.m_idx = idx;
		m_client->m_cbox.m_key = m_screen->m_key;
		return mouseNO;
	default:
		break;
	}
	return mouseIN;
}

mouse_t CMouse::X_Up(CPoint& point)
{
	if (m_client->isNext())
	{
		m_client->SetNext();
		return mouseNO;
	}

	int idx = WhichForm(point);
	CamBase *form = NULL;
	if (idx != -1)
	{
		if (m_client->isNext(m_screen, idx))
			return mouseNO;
	}

	form = m_screen->GetAtForm(idx);
	if (m_type == MS_RB && form)
	{
		m_client->m_string = form->GetSymbolName();
		if (m_client->m_vm->Procedure(m_screen, "OnRClick", form->GetSymbolName()))
			return mouseNO;
	}
	if (m_client->RunMenu(m_screen, form, point) || form == NULL)
		return mouseNO;

	m_mouse.m_key = m_screen->m_key;

	if (!form->IsVisible() || !IsEnable(form))
		return mouseERR;

	int	result;
	if (m_type == MS_RB)
		form->OnRButton(true, point, result);
	else
		form->OnMButton(true, point, result);

	if (idx != m_mouse.m_idx)
	{
		m_mouse.m_idx = idx;
		return mouseOUT;
	}
	return mouseIN;
}

bool CMouse::OnClick()
{
	if (m_mouse.m_idx < 0)
		return false;

//	if (!m_client->m_vm->IsAvailable(m_screen, evClick, m_screen->GetAtForm(m_mouse.m_idx)))
//		return false;

	m_client->m_vm->OnClick(m_screen, m_screen->GetAtForm(m_mouse.m_idx));
	return true;
}

void CMouse::ComboBox(CPoint point, bool down, bool click)
{
	CScreen	*screen;

	m_mouse.m_key = m_client->m_cbox.m_key;
	m_mouse.m_idx = m_client->m_cbox.m_idx;
	m_client->GetAtScreen(screen, m_mouse.m_key);
	CamBase *form = screen->GetAtForm(m_mouse.m_idx);

	int	result;
	if (click)
		form->OnLButton(down, point, result);
	else
		form->OnDblClick(point, result);

	switch (result)
	{
	case RC_ITEM:
		m_client->m_keyx->OnChange(screen, m_mouse.m_idx, true);
		m_type = MS_NB;
		break;
	case RC_OUT:
	case RC_CBCLOSE:
		m_type = MS_NB;
		break;
	case RC_CBSPREAD:
		break;
	default:
		break;
	}
}

void CMouse::SaveMouse(CPoint &point, bool click)
{
	m_client->GetScreen(m_screen);
	point += m_client->m_offset;

	CScreen* screen;
	CamBase* form;
	int	ii;

	m_caret.m_key = -1;
	for (ii = m_client->m_magic; ii; ii--)
	{
		if (!m_client->GetAtScreen(screen, ii))
			continue;
		m_client->GetKeyObject(screen, form);
		if (m_client->isVisible(screen, form) && form->IsPointInRect(point))
		{
			if (!form->IsVisible())
				return;
			break;
		}
	}
	if (ii <= 0)
		m_client->GetAtScreen(screen);

	if (screen != m_screen)
	{
		m_caret = m_client->m_current;
		if (screen->isMain())
		{
			screen = m_screen;
			m_object.m_idx = m_screen->m_object;
			while (m_client->GetAtScreen(screen, screen->m_parent))
			{
				if (screen->isMain())
					break;
				m_object.m_idx = screen->m_object;
			}
		}
		else
			m_object.m_idx = screen->m_object;

		m_screen = screen;
		m_client->GetAtScreen(screen, screen->m_parent);
		if (click)
			m_client->SetCPos(screen, m_object.m_idx);
		m_object.m_key = screen->m_key;
	}
}

void CMouse::RestoreMouse(bool click)
{
	if (m_caret.m_key < 0)
		return;

	CScreen* screen;

	if (!m_client->GetScreen(screen))
		return;
	if (m_object.m_key == screen->m_key &&
		m_object.m_idx == m_client->m_current.m_idx)
	{
		if (click)
		{
			m_client->GetAtScreen(screen, m_caret.m_key);
			m_client->SetCPos(screen, m_caret.m_idx);
		}
		else
			m_client->m_current = m_caret;
	}

	m_caret.m_key = -1;
}

int CMouse::WhichForm(CPoint pt)
{
	int	ii, onPoint, myPoint, youPoint;

	onPoint = myPoint = youPoint = -1;
	for (ii = m_screen->m_mapH->formN-1; ii >= 0; ii--)
	{
		CamBase *form = m_screen->GetAtForm(ii);
		if (!m_screen->IsFormVisible(form) || !form->IsPointInRect(pt))
			continue;

		switch (form->m_form->kind)
		{
//		case FM_SYS:
		case FM_BOX:
		case FM_GROUP:
//		case FM_AVI:
		case FM_OBJECT:
			if (youPoint == -1)
				youPoint = ii;
			break;

		case FM_LABEL:
			if (onPoint == -1)
				onPoint = ii;
			break;
		case FM_EDIT:
		//case FM_MEMO:
		case FM_BUTTON:
		case FM_CHECK:
		case FM_RADIO:
		case FM_COMBO:
		case FM_OUT:
		case FM_GRID:
		case FM_GRIDEX:
//		case FM_TAB:
		case FM_TAB:
		case FM_TABLE:
//		case FM_TREE:
//		case FM_SHEET:
		case FM_BROWSER:
			return ii;

		case FM_IMAGEVW:
			if (myPoint == -1)
				myPoint = ii;
			break;
		}
	}

	ii = (onPoint != -1) ? onPoint : (myPoint != -1) ? myPoint : youPoint;
	return ii;
}

bool CMouse::IsEnable(CamBase* form)
{
	if (!form->IsEnable())
	{
		if (form->m_form->kind == FM_GRID || form->m_form->kind == FM_GRIDEX)
		{
			switch (form->m_form->iok)
			{
			case EIO_INPUT:
			case EIO_INOUT:
				return true;
			default:
				break;
			}
		}
		return false;
	}
	return true;
}

bool CMouse::IsSelf(CamBase* form)
{
	/*
	if (form->m_form->kind == FM_EDIT && form->m_form->properties & PR_LIST && !(form->m_form->attr & FA_PROTECT))
	{
		CString	text;

		form->ReadData(text);
		text.TrimRight();
		if (text.IsEmpty())
			return true;
	}
	*/
	return false;
}
