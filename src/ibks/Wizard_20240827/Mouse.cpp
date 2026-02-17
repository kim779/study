// Mouse.cpp: implementation of the CMouse class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "Wizard.h"
#include "Mouse.h"
#include "Client.h"
#include "Script.h"
#include "Stream.h"
#include "Key.h"
#include "History.h"
#include "../dll/form/fmgrid.h"

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

	m_idx     = -1;
	m_capture = false;
	m_repeat  = -1;
	m_type    = MS_NB;

	m_mouse.m_key = -1;
	m_mouse.m_idx = -1;

	m_hover.m_key = -1;
	m_hover.m_idx = -1;

	m_move.m_key  = -1;
	m_move.m_idx  = -1;

	m_screen = NULL;
}

CMouse::~CMouse()
{
}

void CMouse::OnDown(CPoint point, WPARAM wParam)
{
	if (m_capture)	return;

	// Win10 IME
	m_client->ClearIME();

//	if (m_client->m_status & S_WAIT)
	if (m_guard->IsWait(m_client))
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
		if (L_Down(point, shift) == mouse_t::mouseERR)
		{
			m_capture = false;
			return;
		}
		break;
	case MS_RB:
	case MS_MB:
		if (X_Down(point) == mouse_t::mouseERR)
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
		CfmBase* form;
		form = m_screen->GetAtForm(m_mouse.m_idx);
		if (form) form->OnLButton(false, point, shift);
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
//		if (L_Up(point) == mouseIN && !(m_client->m_status & S_WAIT))
		if (L_Up(point) == mouse_t::mouseIN && !m_guard->IsWait(m_client))
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
//	if (!m_capture || m_repeat < 0 || m_client->m_status & (S_WAIT|S_LOCK))
	if (!m_capture || m_repeat < 0 || (m_client->m_status & S_LOCK) || m_guard->IsWait(m_client))
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

	CfmBase *form = screen->GetAtForm(m_mouse.m_idx);
	if (!form) return false;

	rc = form->OnRepeat(point, result);
	switch (result)
	{
	case RC_SPIN:
		form->IsChanged();
		if (m_client->m_vm->IsAvailable(m_screen, evChange, form))
			m_client->m_vm->OnChange(m_screen, form);
		[[fallthrough]];
	default:
		break;
	}
	return rc;
}

void CMouse::OnDblClick(CPoint point, WPARAM wParam)
{
//	if (m_client->m_status & S_WAIT)
	if (m_guard->IsWait(m_client))
		return;

	m_client->History(false);
	int idx = WhichForm(point);
	if (idx < 0)
		return;

	CfmBase *form = m_screen->GetAtForm(idx);
	if (!form || !form->IsVisible() || !IsEnable(form))
		return;

	if (m_client->m_current == m_mouse)
	{// if cursor position force moved, ignore DblClick action for cursor position 041221
		switch (form->m_form->kind)
		{
		case FM_COMBO:
			ComboBox(point, true, false);
			break;
		case FM_GRID:
		case FM_MEMO:
			if (form->m_form->iok == EIO_OUTPUT)
				break;
			[[fallthrough]];
		case FM_EDIT:
		case FM_TREE:
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
		[[fallthrough]];
	case RC_CODE:
		if (m_client->m_current == m_mouse)
		{
			m_client->m_cbox.m_idx = idx;
			m_client->m_cbox.m_key = m_screen->m_key;
			m_client->History(form, result);
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

//	if (m_client->m_status & S_WAIT)
	if (m_guard->IsWait(m_client))
		return false;

	int	delta = (short)HIWORD(wParam) / 120;
	switch (m_client->m_status & S_FIND)
	{
	case S_FIND:
		((CHistory *)m_client->m_child)->OnWheel(delta);
		return true;
	default:
		if (m_client->m_status & S_LOCK)
			return false;
		idx = WhichForm(point);
		break;
	}

	if (screen == NULL || idx < 0)
		return false;

	int	result;
	CfmBase *form = screen->GetAtForm(idx);
	if (!form) return false;

	fBool = form->OnMouseWheel(delta, result);
	screen->OnClick(form, result);

	if (fBool)	m_client->History(false);

	return fBool;
}

void CMouse::OnMove(CPoint point, WPARAM wParam)
{
//	if (m_client->m_status & S_WAIT)
	if (m_guard->IsWait(m_client))
		return;

	int	result, idx;
	CfmBase *form = nullptr;
	CScreen	*screen = nullptr;
	result = RC_NOP;

	if (!m_capture)
	{
		CCaret	caret = m_move;
		if (m_hover.m_key >= 0 && m_hover.m_idx >= 0)
		{
			if (m_screen->m_key != m_hover.m_key)
			{
				if (!m_client->GetAtScreen(screen, m_hover.m_key))
					return;
			}
			else	screen = m_screen;

			form = screen->GetAtForm(m_hover.m_idx);
			if (form)
			{
				form->OnDrag(point, result);
				if (result == RC_HOVER)
					return;
			}
		}
		else if (m_move.m_key >= 0 && m_move.m_idx >= 0)
		{
			if (m_screen->m_key != m_move.m_key)
			{
				if (!m_client->GetAtScreen(screen, m_move.m_key))
					return;
			}
			else	screen = m_screen;

			idx = WhichForm(screen, point);
			form = screen->GetAtForm(m_move.m_idx);
			if (form)
			{
				form->OnDrag(point, result);
				if (idx != -1 && idx == m_move.m_idx)
					if (result == RC_HOVER || result == RC_IN)
						return;
			}
		}

		m_move.m_key = -1;
		m_move.m_idx = -1;
		idx = WhichForm(point);
		if (idx != -1)
		{
			form = m_screen->GetAtForm(idx);
			if (form) form->OnDrag(point, result);
			if (result == RC_HOVER)
			{
				m_hover.m_key = m_screen->m_key;
				m_hover.m_idx = idx;
				m_move = m_hover;
			}
			else
			{
				m_hover.m_key = -1;
				m_hover.m_idx = -1;
				if (result == RC_IN)
				{
					m_move.m_key = m_screen->m_key;
					m_move.m_idx = idx;
				}
			}
		}
		if (caret != m_move && m_client->GetAtScreen(screen, caret.m_key))
		{
			CfmBase* base = screen->GetAtForm(caret.m_idx);
			if (base && m_client->m_vm->IsAvailable(screen, evMouseLeave, base))
				m_client->m_vm->OnMouseLeave(screen, base);
		}
		if (caret != m_move && m_move.m_key >= 0 && m_move.m_idx >= 0 && m_client->m_vm->IsAvailable(m_screen, evMouseEnter, form))
			m_client->m_vm->OnMouseEnter(m_screen, form);
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

void CMouse::OnCursor(bool reset, CPoint point)
{
	if (!m_client->m_focus || !(m_client->m_status & S_LOAD) || m_screen == NULL)
		return;

	if (reset)
	{
		m_guard->SetCursor(CS_NONE);
		return;
	}

	CfmBase *form;
	int	result = RC_NOP;

	if (point.x < 0 && point.y < 0)
	{
		POINT	pt;

		::GetCursorPos(&pt);
		m_client->m_view->ScreenToClient(&pt);
		point = CPoint(pt.x, pt.y);
	}

	if (m_move.m_idx != -1)
	{
		form = m_screen->GetAtForm(m_move.m_idx);
		if (form)
			form->OnDrag(point, result);
	}
}

mouse_t CMouse::L_Down(CPoint& point, int shift)
{
	m_client->History(false);
	m_mouse.m_idx = -1;
	m_mouse.m_key = m_screen->m_key;

	int idx = WhichForm(point);
	if (idx < 0)
	{
		if (m_client->m_status & (S_SHIFT|S_CONTROL))
			m_client->ClearShift();
		return mouse_t::mouseNO;
	}

	if (m_client->m_status & (S_SHIFT|S_CONTROL) &&
		(m_client->m_cbox.m_key != m_screen->m_key || m_client->m_cbox.m_idx != idx))
		m_client->ClearShift();

	CfmBase* form = m_screen->GetAtForm(idx);
	if (!form || !form->IsVisible() || !IsEnable(form))
		return mouse_t::mouseERR;

	bool	focus = true;
	switch (form->m_form->kind)
	{
	case FM_BUTTON:
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
		[[fallthrough]];
	case FM_MEMO:
		if (form->m_form->iok == EIO_OUTPUT)
		{
			form->OnLButton(true, point, shift);
			m_mouse.m_idx = idx;
			break;
		}
		[[fallthrough]];
	case FM_TREE:
	case FM_COMBO:
		m_mouse.m_idx = idx;
		if (focus && m_mouse != m_client->m_current)
		{
			m_client->OnFocus(m_screen, idx, true);
//			if (m_client->m_status & S_WAIT)
			if (m_guard->IsWait(m_client))
				return mouse_t::mouseERR;
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
		if (form->m_form->kind == FM_BUTTON && form->m_form->type == BTN_RADIO)
		{
			form->OnLButton(true, point, shift);
			m_mouse.m_idx = idx;
			m_type = MS_LB;
			OnClick();
		}
		break;

	case FM_EDIT:
		m_mouse.m_idx = idx;
		form->OnLButton(true, point, shift);
		if (focus && m_mouse != m_client->m_current)
			m_client->OnFocus(m_screen, idx, true);
		break;

	case FM_GRID:
		if (focus && ((CfmGrid *)form)->GetValidRows() > 0
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
		[[fallthrough]];
	case FM_SHEET:
	case FM_OUT:
	case FM_LABEL:
	case FM_PANEL:
	case FM_TABLE:
	case FM_BROWSER:
		form->OnLButton(true, point, shift);
		m_mouse.m_idx = idx;
		break;

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
			return mouse_t::mouseNO;
		}
		m_mouse.m_idx = idx;
		return mouse_t::mouseIN;

	case FM_UTAB:
		form->OnLButton(true, point, shift);
		if (shift >= 0)
		{
			m_mouse.m_idx = idx;
			OnClick();

			m_mouse.m_idx = -1;
			m_type = MS_NB;
			return mouse_t::mouseNO;
		}
		m_mouse.m_idx = idx;
		return mouse_t::mouseIN;
	default:
		return mouse_t::mouseERR;
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
		return mouse_t::mouseNO;
	case RC_SCRUP:
	case RC_SCRDN:
	case RC_PAGEUP:
	case RC_PAGEDN:
	case RC_FIRST:
		m_screen->OnClick(form, shift);
		return mouse_t::mouseNO;

	case RC_SPIN:
		form->IsChanged();
		if (m_client->m_vm->IsAvailable(m_screen, evChange, form))
			m_client->m_vm->OnChange(m_screen, form);
		[[fallthrough]];
	case RC_UPC:
	case RC_DNC:
	case RC_LEFT:
	case RC_RIGHT:
		m_repeat = 0;
		point -= m_client->m_offset;
		m_point = point;
		m_client->m_view->KillTimer(TM_REPBN);
		m_client->m_view->SetTimer(TM_REPBN, 100, NULL);
		m_client->m_vms.scroll = SB_NS;
		return mouse_t::mouseIN;

	case RC_ITEM:
		m_client->m_vms.scroll = SB_IM;
		switch (form->m_form->kind)
		{
		case FM_GRID:
			if (m_mouse != m_client->m_current && form->m_form->isEdit != FI_NONE)
				m_client->SetCPos(m_screen, idx);
			break;
		case FM_TABLE:
			m_client->SetCPos(m_screen, idx);
			[[fallthrough]];
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
			[[fallthrough]];
		default:
			break;
		}
		return mouse_t::mouseIN;
	case RC_FOLDER:
		m_client->m_vms.scroll = SB_FD;
		return mouse_t::mouseIN;
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
		m_client->m_stream->InStream(m_screen, true);
		return mouse_t::mouseIN;
	case RC_OUT:
		m_client->m_vms.scroll = SB_NS;
		return mouse_t::mouseOUT;
	default:
		m_client->m_vms.scroll = SB_NS;
		if (form->m_form->kind == FM_GRID)
			return mouse_t::mouseIN;
		break;
	}

	if (!(form->m_form->attr & FA_DRAG) || m_client->m_vm->IsAvailable(m_screen, evClick, form))
		return mouse_t::mouseIN;

	CString	text;
	int	row, col;
	bool	chkDrag = false;
	struct _cellR	*cell;
	m_guard->m_dragbys.Empty();

	switch (form->m_form->kind)
	{
	case FM_GRID:
		row = ((CfmGrid *)form)->GetCellRow();
		if (((CfmGrid *)form)->GetValidRows() < row)
			return mouse_t::mouseIN;

		col = ((CfmGrid *)form)->GetCellColumn();
		if (--col < 0 || --row < 0)
			return mouse_t::mouseIN;

		cell = (struct _cellR *) &m_screen->m_cellR[form->m_form->vals[0]];
// updateXX_20200615
//		if (!(cell[col].attr & FA_DRAG))
//			return mouse_t::mouseIN;
		chkDrag = (cell[col].attr & FA_DRAG) ? true : false;

		if (cell[col].attr & FA_REF && cell[col].vals[0] != NOVALUE)
			col = cell[col].vals[0];

		form->ReadData(text, false, -1, col, row);
		text.TrimRight();
		if (text.IsEmpty())
			return mouse_t::mouseIN;

		m_guard->m_dragbys = (char *)cell[col].name;
		m_guard->m_dragbys += "\t" + text;
// updateXX_20200615
		if (!chkDrag)
		{
			text.Format("%d\t%d", row, col); m_guard->m_dragbys += "\t" + text;
			m_client->m_vm->Procedure(m_screen, "OnGridClickedXX", m_guard->m_dragbys);
			return mouse_t::mouseIN;
		}
		m_client->m_vm->Procedure(m_screen, "OnGridDragClickedXX", m_guard->m_dragbys);
		break;
	case FM_TREE:
		form->ReadAll(text);
		if (text.GetLength() < form->m_form->size)
			return mouse_t::mouseIN;

		m_guard->m_dragbys = (char *)form->m_form->name;
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
		return mouse_t::mouseIN;
	default:
		if (form->m_form->attr & FA_REF && form->m_form->vals[2] != NOVALUE)
		{
			idx = form->m_form->vals[2];
			form = m_screen->GetAtForm(idx);
		}
		form->ReadData(text);
		text.TrimRight();
		if (text.IsEmpty())
			return mouse_t::mouseIN;

		m_guard->m_dragbys = (char *)form->m_form->name;
		m_guard->m_dragbys += "\t" + text;
		break;
	}

	m_client->Register(NULL, true);
	m_guard->OnDrag(m_client->m_key, m_guard->m_dragbys);
	return mouse_t::mouseERR;
}

mouse_t CMouse::X_Down(CPoint& point)
{
	if (m_client->m_status & (S_SHIFT|S_CONTROL))
		m_client->ClearShift();

	m_client->History(false);
	m_mouse.m_idx = -1;

	m_mouse.m_key = m_screen->m_key;
	m_mouse.m_idx = WhichForm(point);

	if (m_mouse.m_idx < 0)
		return mouse_t::mouseNO;

	CfmBase *form = m_screen->GetAtForm(m_mouse.m_idx);
	if (!form || !form->IsVisible() || !IsEnable(form))
		return mouse_t::mouseERR;

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
	case RC_UPC:
	case RC_DNC:
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
	return mouse_t::mouseIN;
}

mouse_t CMouse::L_Up(CPoint& point)
{
	int	result = 0;

	if (m_mouse.m_idx < 0)
		return mouse_t::mouseERR;

	CfmBase	*form;
	if (m_mouse.m_key != m_screen->m_key)
	{
		CScreen *screen;
		m_client->GetAtScreen(screen, m_mouse.m_key);
		if (screen && m_mouse.m_idx < screen->m_mapH->formN)
		{
			form = screen->GetAtForm(m_mouse.m_idx);
			if (form) form->OnLButton(false, point, result);
		}
		return mouse_t::mouseNO;
	}

	int idx = WhichForm(point);
	if (idx < 0 || m_mouse.m_idx != idx)
	{
		form = m_screen->GetAtForm(m_mouse.m_idx);
		if (form) form->OnLButton(false, point, result);
		return mouse_t::mouseOUT;
	}

	form = m_screen->GetAtForm(idx);
	if (!form)
		return mouse_t::mouseERR;

	switch (form->m_form->kind)
	{
	case FM_TAB:
	case FM_UTAB:
	//	form->OnLButton(false, point, result);
	//	return mouseNO;
		form->OnLButton(false, point, result);
		if (result >= 0)
		{
			m_mouse.m_idx = idx;
			OnClick();

			m_mouse.m_idx = -1;
			m_type = MS_NB;
		}
		return mouse_t::mouseNO;
	case FM_BUTTON:
		if (form->m_form->type == BTN_RADIO)
		{
			form->OnLButton(false, point, result);
			return mouse_t::mouseNO;
		}
		[[fallthrough]];
	case FM_EDIT:
	case FM_MEMO:
	case FM_GRID:
	case FM_OUT:
	case FM_COMBO:
	case FM_LABEL:
	case FM_PANEL:
	case FM_TABLE:
	case FM_TREE:
	case FM_SHEET:
	case FM_BROWSER:
		form->OnLButton(false, point, result);
		break;
	default:
		return mouse_t::mouseERR;
	}

	switch (result)
	{
	case RC_NOP:
		if (!IsSelf(form))
			break;
		[[fallthrough]];
	case RC_CODE:
		m_client->m_cbox.m_idx = idx;
		m_client->m_cbox.m_key = m_screen->m_key;
		m_client->History(form, result);
		return mouse_t::mouseNO;
	default:
		break;
	}
	return mouse_t::mouseIN;
}

mouse_t CMouse::X_Up(CPoint& point)
{
	if (m_client->isNext())
	{
		m_client->SetNext();
		return mouse_t::mouseNO;
	}

	int idx = WhichForm(point);
	CfmBase *form = NULL;
	if (idx != -1)
	{
		if (m_client->isNext(m_screen, idx))
			return mouse_t::mouseNO;
	}

	form = m_screen->GetAtForm(idx);
	if (m_type == MS_RB && form)
	{
		m_client->m_string = (char *)form->m_form->name;
		if (m_client->m_vm->Procedure(m_screen, "OnRClick", (char *)form->m_form->name))
			return mouse_t::mouseNO;
	}
	if (m_client->RunMenu(m_screen, form, point) || form == NULL)
		return mouse_t::mouseNO;

	m_mouse.m_key = m_screen->m_key;

	if (!form->IsVisible() || !IsEnable(form))
		return mouse_t::mouseERR;

	int	result;
	if (m_type == MS_RB)
		form->OnRButton(true, point, result);
	else
		form->OnMButton(true, point, result);

	if (idx != m_mouse.m_idx)
	{
		m_mouse.m_idx = idx;
		return mouse_t::mouseOUT;
	}
	return mouse_t::mouseIN;
}

bool CMouse::OnClick()
{
	if (m_mouse.m_idx < 0)
		return false;

	if (!m_client->m_vm->IsAvailable(m_screen, evClick, m_screen->GetAtForm(m_mouse.m_idx)))
		return false;

	m_client->m_vm->OnClick(m_screen, m_screen->GetAtForm(m_mouse.m_idx));
	return true;
}

void CMouse::ComboBox(CPoint point, bool down, bool click)
{
	CScreen	*screen;

	m_mouse.m_key = m_client->m_cbox.m_key;
	m_mouse.m_idx = m_client->m_cbox.m_idx;
	m_client->GetAtScreen(screen, m_mouse.m_key);
	CfmBase *form = screen->GetAtForm(m_mouse.m_idx);
	if (!form) return;

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
	m_idx = -1;
	m_client->GetScreen(m_screen);
	point += m_client->m_offset;

	CScreen* screen = nullptr;
	CfmBase* form;
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
	m_idx = -1;
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

int CMouse::WhichForm(CScreen* screen, CPoint pt)
{
	if (screen)
		return m_client->WhichForm(screen, pt);
	return -1;
}

int CMouse::WhichForm(CPoint pt)
{
	if (m_screen)
	{
		m_idx = m_client->WhichForm(m_screen, pt);
		return m_idx;
	}
	return -1;
}

bool CMouse::IsEnable(CfmBase* form)
{
	if (!form->IsEnable())
	{
		if (form->m_form->kind == FM_GRID)
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

bool CMouse::IsSelf(CfmBase* form)
{
	if (form->m_form->kind == FM_EDIT && form->m_form->properties & PR_LIST && !(form->m_form->attr & FA_PROTECT))
	{
		CString	text;

		form->ReadData(text);
		text.TrimRight();
		if (text.IsEmpty())
			return true;
	}
	return false;
}
