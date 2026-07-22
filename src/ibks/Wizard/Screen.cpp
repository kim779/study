// Screen.cpp: implementation of the CScreen class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "Wizard.h"
#include "Screen.h"
#include "Client.h"
#include "Script.h"
#include "Stream.h"
#include "key.h"
#include "xscreen.h"
#include "xsystem.h"
#include "misc.h"

#include "../h/axisvar.h"
#include "../h/axiserr.h"
#include "../h/axstring.h"
#include "../dll/form/fmctrl.h"
#include "../dll/form/fmgrid.h"
#include "../dll/form/fmtable.h"
#include "../dll/vbs/engineWrapper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScreen::CScreen(CClient* client, CRect rect)
	: CAxisForm()
{
	m_client = client;
	m_rect   = rect;
	m_guard  = client->m_guard;
	m_vbe    = new CEngineWrapper(m_guard->GetParent());

	m_mapH    = NULL;
	m_view    = m_client->m_view;
	m_palette = m_guard->m_palette;
	m_draw    = m_guard->m_draw;
	m_root    = m_guard->m_root;

	m_row    = 0;
	m_tabL   = 0;
	m_ioL    = 0;
	m_state  = 0;
	m_push   = NULL;
	m_tabR   = NULL;
	m_ioR    = NULL;
	m_parent = 0;
	m_trxK   = 0;
	m_trxS   = 0;
	m_object = 0;
	m_notice = NULL;
	m_profit = NULL;

	m_ledger  = NULL;
	m_ledgerL = 0;
	m_xscreen = NULL;
	m_cells.RemoveAll();
	m_notices.RemoveAll();

	m_ins.RemoveAll();
	m_outs.RemoveAll();

	m_icells.RemoveAll();
	m_ocells.RemoveAll();

	m_objects.RemoveAll();
}

CScreen::~CScreen()
{
#ifdef DF_RTM_INDEX  //CScreen::~CScreen - RTM index detach
	{
		WORD	flashKey;
		CString	flashCode;
		for (POSITION pos = m_lastCodes.GetStartPosition(); pos; )
		{
			m_lastCodes.GetNextAssoc(pos, flashKey, flashCode);
			m_guard->UpdateCodeIndex(this, flashCode, _T(""));
		}
	}
#endif

	m_flashObs.RemoveAll();
	m_revObs.RemoveAll();
	m_blink.RemoveAll();

	if (IsWindow(m_client->m_view->GetSafeHwnd()))
	{
		m_client->m_view->HideCaret();
		if (m_state & reptxSN)
			m_client->m_view->KillTimer(TM_REPTR+m_key);
//		m_client->m_view->KillTimer(TM_VB+m_key);
	}

//	m_client->m_vm->OnClose(this);

	if (m_mapH && m_client)
	{
		for (int ii = 0; ii < m_mapH->formN; ii++)
		{
			switch (m_formR[ii].kind)
			{
			case FM_OBJECT:
				break;
			default:
				continue;
			}
			m_client->RemoveAtScreen(m_formR[ii].keys);
		}
	}
	m_client->RemoveTips(this);

	if (m_tabR != NULL)
		delete[] m_tabR;
	if (m_ioR != NULL)
		delete[] m_ioR;
	if (m_profit != NULL)
		delete m_profit;


	CString	name;
	CObject* object;
	POSITION pos;

	for (pos = m_cells.GetStartPosition(); pos; )
	{
		m_cells.GetNextAssoc(pos, name, object);
		((CPtrArray *)object)->RemoveAll();
		delete object;
	}
	m_cells.RemoveAll();
	m_notices.RemoveAll();

	m_vbe->UnloadScript();

	if (m_ledger)
		delete m_ledger;
	if (m_xscreen)
		delete m_xscreen;
	CpubControl* objectx;
	for (pos = m_objects.GetStartPosition(); pos; )
	{
		m_objects.GetNextAssoc(pos, name, (void *&)objectx);
		delete objectx->m_target;
		AfxFreeLibrary(objectx->m_dll);
		delete objectx;
	}
	m_objects.RemoveAll();

	delete m_vbe;

	ClearTranInfo();
}

bool CScreen::Attach(CString mapN)
{
	if (mapN.GetLength() != L_MAPN)
		return false;

	if (!m_guard->isVersion(mapN))
		return false;

	CString tmps = m_guard->PathMAP(mapN);
	if (!LoadMAP(tmps))
		return false;

	if (!(m_guard->m_term & flagACN))
	{
		if (m_guard->m_term & flagXXX && (m_mapH->options & (OP_CERTIFY|OP_ENC)))
		{
			m_guard->SetGuide(AE_ESECURITY);
			return false;
		}
	}

	if (m_tabR != NULL)
		delete[] m_tabR;
	m_tabR = new int[m_mapH->formN];
	m_tabL = 0;
	ZeroMemory(m_tabR, sizeof(int) * m_mapH->formN);

	if (m_ioR != NULL)
		delete[] m_ioR;
	m_ioR = new int[m_mapH->formN];
	m_ioL = 0;
	ZeroMemory(m_ioR, sizeof(int) * m_mapH->formN);

	if (m_mapH->typeH == TH_LEDGER)
	{
		m_ledger  = m_guard->GetLedger((void *)m_mapH);
		m_ledgerL = atoi(m_guard->GetLedger(m_ledger, getSIZE));
	}

	return true;
}

int compare(const void *arg1, const void *arg2)
{
	return *(int *)arg1 - *(int *)arg2;
}

bool CScreen::Parse(bool resize, bool fix)
{
	if (m_push != NULL)
	{
		m_push = NULL;
		m_guard->SetPush(false);
	}

	m_state  = 0;
	m_notice = m_sales = NULL;

	CRect	mainRc, unitRc;

	mainRc = isMain() ? CRect(0, 0, m_mapH->width, m_mapH->height) : m_rect;
	LoadForm(mainRc);

	CScreen* screen;
	CfmBase* form;
	float	hr, vr;
	CString	text, string;
	char	twb[512];
	bool	flash = false;

	m_vbe->Initialize(CString(m_mapH->mapN, L_MAPN));
	screen    = this;
	m_xscreen = new CxScreen(this);
	m_vbe->AddObject(_T("Screen"), m_xscreen, SCRIPTITEM_GLOBALMEMBERS);
	m_vbe->AddObject(_T("System"), m_guard->m_xsystem);
	if (m_guard->m_login)
		m_vbe->AddObject(_T("Login"),  m_guard->m_login);
	if (m_ledger)
	{
		m_guard->SetLedger(m_ledger, -1, m_client->m_key, (char *)m_client->m_mapN.operator LPCTSTR());
		m_vbe->AddObject(_T("Ledger"), m_ledger);
	}
	if (m_mapH->publicR != NOVALUE)
	{
		int	pos;
		CString	name;
		CCmdTarget* control;

		text = (char *)m_mapH->publicR;
		for ( ; !text.IsEmpty(); )
		{
			pos = text.Find('\t');
			if (pos != -1)
			{
				name = text.Left(pos);
				text = text.Mid(pos+1);
			}
			else
			{
				name = text;
				text = _T("");
			}

			name.Trim();
			if (name.IsEmpty()) continue;

			control = m_client->GetControl(name);
			if (control)
				m_vbe->AddObject(name, control);
		}
	}

	CObArray fmTabs;

	fmTabs.RemoveAll();
	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		form = GetAtForm(ii);
		if (!form) continue;
		if (form->m_form->kind == FM_COMBO)
		{
			if (form->m_form->properties & PR_ACCN)
			{
				if (m_guard->m_term & flagACN)
					form->SetEnum("", 0);
				else if (m_guard->GetCombo((char *)form->m_form->name, text))
					form->WriteAll(text, false);
			}
			form->IsChanged();
		}

		Order(form, ii);
		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			if (isMain())
				unitRc = CRect((short)form->m_form->left, (short)form->m_form->top, (short)form->m_form->right, (short)form->m_form->bottom);
			else
			{
				hr = (float)mainRc.Width() / (float)m_mapH->width;
				vr = (float)mainRc.Height() / (float)m_mapH->height;
				if (form->m_form->attr2 & GO_FIX)
					vr = m_client->m_vR;

				unitRc.left   = int(mainRc.left+((short)form->m_form->left)*hr);
				unitRc.top    = int(mainRc.top+((short)form->m_form->top)*vr);
				unitRc.right  = int(mainRc.left+((short)form->m_form->right)*hr);
				unitRc.bottom = int(mainRc.top+((short)form->m_form->bottom)*vr);
			}

			if (m_client->SetAtScreen(screen, unitRc, &m_strR[form->m_form->vals[0]], -1, m_key))
			{
				form->m_form->keys = screen->m_key;
				screen->m_object   = ii;
				screen->m_parent   = m_key;
				screen->Parse(resize, (form->m_form->attr2 & GO_FIX) ? true : false);
			}
			break;

		case FM_BUTTON:
		case FM_MEMO:
		case FM_EDIT:
		case FM_COMBO:
			break;

		case FM_CONTROL:
			((CfmCtrl *)form)->SetFCB((m_guard->m_status & WS_FCB) ? true : false, m_guard->m_flash);
			if (form->m_form->attr & FA_COMMA)
				m_notices.Add(form);
			break;

		case FM_GRID:
			// pre-defined columns
			string.Format(_T("%s\\%s\\grid.ini"), m_root.GetString(), TABDIR);
			text.Format(_T("%s$%s"), LPCTSTR(CString(m_mapH->mapN, L_MAPN)), (char *)form->m_form->name);
			GetPrivateProfileString("GRID", text, "", twb, sizeof(twb), string);
			string = twb;
			if (!string.IsEmpty())
			{
				form->m_form->attr |= FA_DOMINO;
				SetCells(form, string, false, true);
			}

			if (form->m_form->attr & FA_ENUM)	// user-defined columns
			{
				GetPrivateProfileString("GRID", text, "", twb, sizeof(twb), m_guard->m_modals);
				text = twb;
				if (!text.IsEmpty())
					SetCells(form, text, false);
			}
			else if (m_profit == NULL)
				m_guard->SetProfit(this, form, m_profit);

			if (form->m_form->vals[2] != NOVALUE)
			{
				form->m_form->vals[2] = (DWORD)&m_strR[form->m_form->vals[2]];
				text = (char *)form->m_form->vals[2];
				if (text.Find("$$") == 0)
				{
					form->m_form->vals[2] += 2;
					m_notice = form;
				}
				else if (text.Find("$?") == 0)
					m_sales = form;
				else if (text.Find("$*") == 0)
					m_push = form;
			}
			break;

		case FM_UTAB:
			if (form->m_form->attr & FA_ENUM)	// user-defined columns
			{
				text.Format(_T("%s$%s"), LPCTSTR(CString(m_mapH->mapN, L_MAPN)), (char *)form->m_form->name);
				GetPrivateProfileString("UTAB", text, "", twb, sizeof(twb), m_guard->m_modals);
				text = twb;
				if (!text.IsEmpty())
				{
					form->SetEnum(text, 0);
					fmTabs.Add(form);
				}
			}
			break;

		default:
			break;
		}

		if (form->m_form->attr & FA_FLASH)
		{
			if (form->m_form->iorder != (WORD)-1)
				m_flashObs.SetAt(form->m_form->iorder, form);
			else if (form->m_form->kind == FM_CONTROL)
				m_flashObs.SetAt(m_mapH->formN+ii, form);
			flash = true;

#ifdef DF_RTM_INDEX  //CScreen::Parse - RTM index attach (mirrors CScreen::OnAlert's default-branch classification)
			switch (form->m_form->kind)
			{
			case FM_GRID:
			case FM_TABLE:
			case FM_CONTROL:
				break;
			default:
				if (form->m_form->iorder != (WORD)-1)
				{
					form->ReadData(text);
					text.TrimRight();
					m_guard->UpdateCodeIndex(this, _T(""), text);
					m_lastCodes.SetAt(form->m_form->iorder, text);

					CString dbg;
					dbg.Format("[WIZARD][RTM][DEBUG][ATTACH] mapN=%s name=%s kind=%d code=%s\n",
						CString(m_mapH->mapN, L_MAPN).GetString(), (char*)form->m_form->name, form->m_form->kind, text.GetString());
					OutputDebugString(dbg);
				}
				break;
			}
#endif

		}

		m_vbe->AddObject(form->GetSymbolName(),
			(form->m_form->kind == FM_CONTROL) ? (CCmdTarget *)((CfmCtrl*)form)->m_ctrl : form);
	}

	if (m_mapH->scriptR != NOVALUE)
	{
		CString	pubs;

		text = CString(m_scriptR, m_mapH->strR - m_mapH->scriptR);

		getExternalScript(text, pubs);

		if (!pubs.IsEmpty())
		{
			int	pos;
			CString	name;
			CCmdTarget* control;

			for ( ; !pubs.IsEmpty(); )
			{
				pos = pubs.Find('\t');
				if (pos != -1)
				{
					name = pubs.Left(pos);
					pubs = pubs.Mid(pos+1);
				}
				else
				{
					name = pubs;
					pubs = _T("");
				}

				name.Trim();
				if (name.IsEmpty())
					continue;

				control = m_client->GetControl(name);
				if (control)
					m_vbe->AddObject(name, control);
			}
		}
		{
			CString dbg;
			dbg.Format("[WIZARD][SCRIPT][DEBUG] text.GetLength()=%d\n", text.GetLength());
			OutputDebugString(dbg);
			dbg.Format("[WIZARD][SCRIPT][DEBUG] text=[%s]\n", text.GetString());
			OutputDebugString(dbg);
		}
		if (!m_vbe->LoadScript(text, m_mapH->pythonMode))
		{
			CStringArray* errs = m_vbe->GetErrorMessages();
			CString dbg;
			dbg.Format("[WIZARD][SCRIPT][DEBUG] LoadScript FAILED, errCount=%d\n", errs ? (int)errs->GetSize() : -1);
			OutputDebugString(dbg);
			if (errs)
			{
				for (int di = 0; di < errs->GetSize(); di++)
				{
					dbg.Format("[WIZARD][SCRIPT][DEBUG] err[%d]=%s\n", di, errs->GetAt(di).GetString());
					OutputDebugString(dbg);
				}
			}
		}
		else
		{
			OutputDebugString("[WIZARD][SCRIPT][DEBUG] LoadScript OK\n");
		}
	}
	for (int ii = 0; ii < fmTabs.GetSize(); ii++)
	{
		form = (CfmBase *)fmTabs.GetAt(ii);
		if (m_client->m_vm->IsAvailable(this, evClick, form))
			m_client->m_vm->OnClick(this, form);
	}
	fmTabs.RemoveAll();

	if (m_push != NULL)
		m_guard->SetPush(true);

	switch (m_mapH->imageK)
	{
	case IK_BRUSH:
		if (isMain())
		{
			if (m_mapH->imageV & 0x02000000)
				m_client->SetBK(m_mapH->imageV);
			else
				m_client->SetBK(m_guard->GetRGB(m_mapH->imageV));
		}
		else if (m_client->GetAtScreen(screen, m_parent))
		{
			form = screen->GetAtForm(m_object);
			if (form) form->SetBkColor(m_mapH->imageV);
		}
		break;
	case IK_BMP:
		if (isMain())
			m_client->SetBK((char *)&m_strR[m_mapH->imageV]);
		else if (m_client->GetAtScreen(screen, m_parent))
		{
			form = screen->GetAtForm(m_object);
			if (form) form->WriteData((char *)&m_strR[m_mapH->imageV]);
		}
		break;
	default:
		if (!isMain() && m_client->GetAtScreen(screen, m_parent))
		{
			form = screen->GetAtForm(m_object);
			if (form) form->ClearData();
		}
		break;
	}

	if (resize)
	{
		if (!m_guard->m_resize && !(m_client->m_status & S_FIX))
			SetFont(m_guard->m_resize, m_client->m_font);
		m_client->GetAtScreen(screen);
		if (fix)
		{
			hr = m_client->m_hR;
			vr = m_client->m_vR;
		}
		else
		{
			hr = (float)m_client->m_rect.Width() / (float)screen->m_mapH->width;
			vr = (float)m_client->m_rect.Height() / (float)screen->m_mapH->height;
		}
		if (IsResizable())
			vr = m_client->m_vR;
		ResizeForm(hr, vr, true);
	}

	if (isUob())
	{
		if (!isMain())
		{
			screen = this;
			while (m_client->GetAtScreen(screen, screen->m_parent))
			{
				if (!screen->isUob())
				{
					if (screen->m_mapH->options & OP_OOP)
						m_mapH->options |= OP_OOP;
					if (screen->m_mapH->options & OP_CR2TB)
						m_mapH->options |= OP_CR2TB;
					break;
				}
			}
		}
		if (flash)	m_mapH->options |= OP_FLASH;
	}

	if (m_mapH->options & OP_FLASH)
		m_client->m_status |= S_FLASH;

	if (m_client->m_offset.cx || m_client->m_offset.cy)
		AdjustForm(-m_client->m_offset);

	if (m_mapH->options & OP_DROP)
		m_client->Register(NULL);

	return true;
}

void CScreen::Order(CfmBase* form, int order)
{
	if (form->m_form->index != (WORD)-1 && form->m_form->index < m_mapH->formN)
	{
		m_tabR[form->m_form->index] = order;
		m_tabL++;
	}

	if (form->m_form->iorder != (WORD)-1 && form->m_form->iorder < m_mapH->formN)
	{
		m_ioR[form->m_form->iorder] = order;
		m_ioL++;
	}
}

void CScreen::ReAttach(CRect rect)
{
	m_rect = rect;
	ReloadForm(m_rect);

	CScreen* screen;
	CfmBase* form;
	float	hr, vr;

	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		form = GetAtForm(ii);
		if (form && form->m_form->kind == FM_OBJECT && m_client->GetAtScreen(screen, form->m_form->keys))
		{
			hr = (float)m_rect.Width() / (float)m_mapH->width;
			vr = (float)m_rect.Height() / (float)m_mapH->height;

			rect.left   = int(m_rect.left+((short)form->m_form->left)*hr);
			rect.top    = int(m_rect.top+((short)form->m_form->top)*vr);
			rect.right  = int(m_rect.left+((short)form->m_form->right)*hr);
			rect.bottom = int(m_rect.top+((short)form->m_form->bottom)*vr);

			screen->ReAttach(rect);
		}
	}
	m_client->GetAtScreen(screen);
	hr = (float)m_client->m_rect.Width() / (float)screen->m_mapH->width;
	vr = (float)m_client->m_rect.Height() / (float)screen->m_mapH->height;
	ResizeForm(hr, vr, true);
}

void CScreen::OnVirtual(char* mapN, int& tab)
{
	tab = NOVALUE;
	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		if (m_formR[ii].kind != FM_TAB)
			continue;
		if (m_formR[ii].vals[2]  >= m_mapH->formN)
			continue;

		int	pageL;
		int	index = m_formR[ii].vals[2];
		int	offs = m_formR[ii].vals[0];
		char	*pageN = &m_strR[offs];
		for (int kk = 0; kk < (int)m_formR[ii].keys; kk++)
		{
			if (!memcmp(&mapN[L_SGID], &pageN[L_SGID], L_SELC))
			{
				m_formR[ii].checked = kk;
				m_formR[index].vals[0] = offs;
				break;
			}
			pageL = strlen(pageN);
			pageN += pageL;
			offs  += pageL;
			pageN++;
			offs++;
		}
		tab = ii;
		break;
	}
}

void CScreen::SetForm(int idx, bool focus)
{
	if (idx < 0 || idx >= m_mapH->formN)
		return;

	CfmBase* form = GetAtForm(idx);
	if (form) form->SetFocus(focus);

	m_client->ClearIME();
	if (focus) m_client->ChangeIME(this, idx);
}

bool CScreen::SetObject(int key, char *mapN, bool force)
{
	if (key < 0 || key >= m_mapH->formN)
		return false;

	return m_client->SetObject(this, key, mapN, force);
}

bool CScreen::isPush(CString& pushN)
{
	if (!isPush())
		return false;

	pushN.Empty();
	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		if (m_formR[ii].kind != FM_EDIT)
			continue;
		if (m_formR[ii].vals[0] == NOVALUE)
			continue;
		pushN = (char *)&m_strR[m_formR[ii].vals[0]];
		break;
	}
	return true;
}

void CScreen::OnPush(CString pushs)
{
	if (!isPush())
		return;

	CString	text;
	int	result;

	CfmBase* form = m_push;

	struct	_cellR *cell = (struct _cellR *) &m_cellR[form->m_form->vals[0]];
	int nCols = HIWORD(form->m_form->vals[1]);

	int rowN = form->GetSize();

	CString	cmps;
	int	tRGB = 0, pRGB = 0;
	bool	colorF;

	for (; !pushs.IsEmpty(); )
	{
		result = pushs.Find('\n');
		if (result == -1)
		{
			text = pushs;
			pushs.Empty();
		}
		else
		{
			text = pushs.Left(result++);
			pushs = pushs.Mid(result);
		}
		if (text.IsEmpty())
			continue;

		colorF = false;
		result = text.Find(0x1b);
		if (result != -1)
		{
			colorF = true;
			cmps = text.Mid(1, 3);
			tRGB = atoi(cmps);
			cmps = text.Mid(4, 3);
			pRGB = atoi(cmps);
			text = text.Mid(7);
		}

		for (int idx = 0; idx < nCols; idx++)
		{
			if (text.IsEmpty())
				break;
			result = text.Find('\t');
			if (result == -1)
			{
				cmps = text;
				text.Empty();
			}
			else
			{
				cmps = text.Left(result++);
				text = text.Mid(result);
			}

			if (colorF)
			{
				form->SetFgColor(tRGB);
				form->SetBkColor(pRGB);
			}
			form->WriteData(cmps, true, idx, m_row);
		}
		if (++m_row >= rowN)
			m_row = 0;

		text = _T("");
		for (int idx = 0; idx < nCols; idx++)
		{
			form->WriteData(text, true, idx, m_row);
			if (colorF)
			{
				form->SetFgColor(cell[idx].tRGB);
				form->SetBkColor(cell[idx].pRGB);
			}
		}
	}
}

bool CScreen::OnAlert(CString code, CString update, CdataSet* fms, CObArray* obs, int stat, struct _alertR* alertR)
{
	if (m_flashObs.IsEmpty())
		return false;

	WORD	key;
	CfmBase* form;
	CString	text;
	bool	flash;

	m_color = flash = false;
	m_tick  = GetTickCount64();
	if (fms == NULL)
		fms = (CdataSet *)obs->GetAt(0);
	for (POSITION pos = m_flashObs.GetStartPosition(); pos; )
	{
		m_flashObs.GetNextAssoc(pos, key, (CObject *&)form);
		switch (form->m_form->kind)
		{
		case FM_GRID:
			if (FlashGrid(key, code, fms))
				flash = true;
			break;
		case FM_TABLE:
			if (FlashSemi(key, fms, code))
				flash = true;
			break;
		case FM_CONTROL:
			text = code + "\t" + update;
			text.Insert(0, stat);
			form->WriteAll(text);
			if (alertR)
				form->SetEnum(_T(""), (int)alertR, 253);
			break;
		default:
			form->ReadData(text);
			text.TrimRight();

			CString dbg;
			dbg.Format("[WIZARD][RTM][DEBUG] compare mapN=%s field=%s vs code=%s match=%d\n",
				CString(m_mapH->mapN, L_MAPN).GetString(), text.GetString(), code.GetString(), !text.Compare(code));
			OutputDebugString(dbg);

#ifdef DF_RTM_INDEX  //CScreen::OnAlert
			// 자가갱신: 매 틱마다 어차피 읽는 이 값을 이용해 인덱스를 최신 상태로 유지.
			// 별도 쓰기지점 후킹 없이, 값이 바뀐 걸 발견하면 그때 인덱스를 갱신함.
			// key(iorder)별로 추적 - 화면 하나에 flash key 필드가 여러 개(다중 코드) 있어도 서로 안 섞이게.
			{
				CString prevCode;
				if (!m_lastCodes.Lookup(key, prevCode))
					prevCode = _T("");
				if (text != prevCode)
				{
					m_guard->UpdateCodeIndex(this, prevCode, text);
					m_lastCodes.SetAt(key, text);
				}
			}
#endif

			if (!text.Compare(code))
			{
				flash = true;
				UpdateRTM(key+1, code, update, fms, obs, stat, alertR);
			}
			break;
		}
	}
	if (m_color)
		DominoForm();
	return flash;
}

void CScreen::UpdateRTM(int key, CString code, CString update, CdataSet* fms, CObArray* obs, int stat, struct _alertR* alertR)
{
	CfmBase* form;
	CString	text, string;
	struct	_flashK	flashK;

	if (m_sales && (stat & alert_SCR))
		ScrollRTM(obs);

	if (isUob() && !isFlash())
		m_color = false;

	for ( ; key < m_ioL; key++)
	{
		form = GetAtForm(m_ioR[key]);
		if (!form) continue;

		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			CScreen* uob;
			if (m_client->GetAtScreen(uob, form->m_form->keys) && uob->isUob() && !uob->isFlash())
			{
				uob->m_tick = m_tick;
				uob->UpdateRTM(0, code, update, fms, obs, stat, alertR);
			}
			continue;
		case FM_EDIT:
		case FM_OUT:
			//if (form->m_form->iok == EIO_INPUT)   //Edit  input no RTS writing //testcode
			//	continue;
			break;
		case FM_GRID:
			if (isUob() && !isFlash())
				FlashGrid(key, code, fms);
			continue;
		case FM_TABLE:
			if (!(form->m_form->attr & FA_FLASH))
				FlashSemi(key, fms);
			continue;
		case FM_CONTROL:
			if (!(form->m_form->attr & FA_FLASH))
			{
				text = update;
				text.Insert(0, stat);
				form->WriteAll(text);
				if (alertR)
					form->SetEnum(_T(""), (int)alertR, 253);
			}
			continue;
		default:
			continue;
		}

		if (form->m_form->attr & FA_FLASH)
			return;

		if (!fms->Lookup(form->GetName(), string))
			continue;

		form->ReadData(text);
		if (form->m_form->properties & PR_SUM)
		{
			int	idx;
			double	value;

			value = atof(text);
			value += atof(string);

			string.Format("%f", value);
			idx = string.Find('.');
			if (idx != -1)
				string = string.Left(idx);
		}
		else if (!isFlash(form->m_form->attr2 & SA_CORGB, string, text))
			continue;

		if (form->m_form->isEdit == IE_CORGB)
			m_color = true;

		if (m_guard->m_status & WS_FCB)
		{
			if (!m_revObs.Lookup(m_ioR[key], flashK))
			{
				flashK.index = m_ioR[key];
				flashK.key = 0;
				flashK.color = form->GetBkColor();
			}
			flashK.time = m_tick;

			form->SetBkColor(m_guard->m_flash);
			form->WriteData(string);
			m_revObs.SetAt(m_ioR[key], flashK);
		}
		else	form->WriteData(string);
	}

	if (isUob() && !isFlash() && m_color)
		DominoForm();
}

bool CScreen::FlashGrid(int key, CString code, CdataSet *fms)
{
	int	gridK, baseK;
	int	nCols, nRows;
	struct	_cellR *cell;
	struct	_flashK	flashK;
	CDWordArray	flashs;

	CfmBase* form;
	DWORD	flash;
	CString	text, string;

	baseK = GetFlashBaseKey(m_ioR[key]);
	form  = GetAtForm(m_ioR[key]);
	if (!form) return false;

	cell  = (struct _cellR *) &m_cellR[form->m_form->vals[0]];
	nCols = HIWORD(form->m_form->vals[1]);
	nRows = ((CfmGrid *)form)->GetValidRows();
	if (nRows <= 0)	return false;

	m_sync.Lock();
	gridK = -1;
	flash = form->m_form->checked;
	if (flash && cell->attr & FA_FLASH)
		flashs.Add(MAKELONG(0, form->m_form->checked));
	flashs.Add(MAKELONG(flash, nCols));

	for (int ii = 0; ii < nRows; ii++)
	{
		for (int kk = 0; kk < flashs.GetSize(); kk++)
		{
			flash = flashs.GetAt(kk);
			form->ReadData(text, false, -1, LOWORD(flash), ii);
			text.TrimRight();

			if (text.Compare(code))
				continue;

			cell = (struct _cellR *) &m_cellR[form->m_form->vals[0] + LOWORD(flash)];
			for (int idx = LOWORD(flash); idx < HIWORD(flash); idx++, cell++)
			{
				if (!fms->Lookup(form->GetName(idx), string))
					continue;

				form->ReadData(text, false, -1, idx, ii);
				if (!isFlash(cell->attr2 & SA_CORGB, string, text))
					continue;

				if (cell->isEdit == IE_CORGB)
					m_color = true;

				gridK = baseK + ii*nCols+idx;
				if (m_guard->m_status & WS_FCB)
				{
					if (!m_revObs.Lookup(gridK, flashK))
					{
						flashK.index = m_ioR[key];
						flashK.key = baseK;
						flashK.color = form->GetBkColor(idx, ii);
					}
					flashK.time = m_tick;

					form->SetBkColor(m_guard->m_flash, idx, ii);
					form->WriteData(string, true, idx, ii);
					m_revObs.SetAt(gridK, flashK);
				}
				else	form->WriteData(string, true, idx, ii);
			}
		}
	}
	flashs.RemoveAll();

	if (gridK != -1)
		OnProfit(form);
	m_sync.Unlock();

	return (gridK == -1) ? false : true;
}

bool CScreen::FlashSemi(int key, CdataSet *fms, CString code)
{
	int	baseK, semiK;
	CString	text, string;

	struct	_cellR *cell;
	struct	_flashK	flashK;

	CfmBase* form = GetAtForm(m_ioR[key]);
	if (!form || form->m_form->iok == EIO_NOP)
		return false;

	if (!code.IsEmpty())
	{
		form->ReadData(text, false, -1, 0, form->m_form->checked);
		text.TrimRight();

		if (text.Compare(code))
			return false;
	}

	baseK = GetFlashBaseKey(m_ioR[key]);
	cell = (struct _cellR *) &m_cellR[form->m_form->vals[0]];

	semiK = -1;
	int nRows = LOWORD(form->m_form->vals[1]);
	for (int ii = 0; ii < nRows; ii++)
	{
		if (!fms->Lookup(form->GetName(ii), string))
			continue;

		form->ReadData(text, false, -1, 0, ii);
		if (!isFlash(cell[ii].attr2 & SA_CORGB, string, text))
			continue;

		if (cell[ii].isEdit == IE_CORGB)
			m_color = true;

		semiK = baseK + ii;
		if (m_guard->m_status & WS_FCB)
		{
			if (!m_revObs.Lookup(semiK, flashK))
			{
				flashK.index = m_ioR[key];
				flashK.key = baseK;
				flashK.color = form->GetBkColor(0, ii);
			}
			flashK.time = m_tick;

			form->SetBkColor(m_guard->m_flash, 0, ii);
			form->WriteData(string, true, 0, ii);
			m_revObs.SetAt(semiK, flashK);
		}
		else	form->WriteData(string, true, 0, ii);
	}
	return (semiK == -1) ? false : true;
}

void CScreen::ScrollRTM(CObArray* obs)
{
	CdataSet* fms;
	CString	text, string, rows;
	struct	_cellR *cell;
	bool	skip, colorF = false, insertF = false;

	CfmBase* form = m_sales;
	int nCols = HIWORD(form->m_form->vals[1]);

	CPtrArray* grid;
	if (form->m_form->attr & FA_ENUM && m_cells.Lookup((char *)form->m_form->name, (CObject *&)grid))
		nCols = grid->GetSize();
	else
		grid = NULL;

	string = _T("");
	for (int ii = 0; ii < obs->GetSize(); ii++)
	{
		fms = (CdataSet *)obs->GetAt(ii);
		cell = (struct _cellR *) &m_cellR[form->m_form->vals[0]];
		skip = true; rows = _T("");
		for (int idx = 0; idx < nCols; idx++, cell++)
		{
			if (grid != NULL)
				cell = (struct _cellR *)grid->GetAt(idx);

			if (fms->Lookup(form->GetName(idx), text))
			{
				skip = false;
				rows += text;
			}
			rows += '\t';

			if (cell->isEdit == IE_CORGB)
				colorF = true;
		}
		if (!skip)
		{
			insertF = true;
			string += rows;
			string += '\n';
		}
	}
	if (insertF)
	{
		if (form->m_form->attr2 & GO_TOP)
			((CfmGrid *)form)->InsertRows(0, string);
		else
			((CfmGrid *)form)->InsertRows(-1, string);
	}

	if (colorF)
		DominoForm();
}

bool CScreen::OnNotice(CdataSet& major, CdataSet& minor, CdataSet& fms, CString notices)
{
	CfmBase* form;
	CString	text, string;
	struct	_flashK	flashK;

	int	ii, name;
	bool	match;

	for (ii = 0; ii < m_notices.GetSize(); ii++)
	{
		form = (CfmBase *)m_notices.GetAt(ii);
		form->SetEnum(notices, 0, 0);
	}

	if (major.IsOn())
	{
		match = false;
		ULONGLONG tick = GetTickCount64();

		for (ii = 0; ii < m_mapH->formN; ii++)
		{
			form = GetAtForm(ii);
			if (!form) continue;

			if (major.Lookup(form->GetName(), string))
			{
				form->ReadData(text);
				text.TrimRight();

				if (!text.IsEmpty())
				{
					if (text.Compare(string))
						return false;
					match = true;
				}
			}
		}

		if (!match)	return false;

		m_color = false;
		for (ii = 0; ii < m_ioL; ii++)
		{
			form = GetAtForm(m_ioR[ii]);
			if (!form) continue;
			switch (form->m_form->kind)
			{
			case FM_OUT:
			case FM_EDIT:
				name = form->GetName();
				if (!major.Lookup(name, string) && fms.Lookup(name, string))
					break;
				continue;
			case FM_TABLE:
				FlashSemi(ii, &fms);
				continue;
			default:
				continue;
			}

			form->ReadData(text);
			if (form->m_form->properties & PR_SUM)
			{
				int	idx;
				double	value;

				value = atof(text);
				value += atof(string);

				string.Format("%f", value);
				idx = string.Find('.');
				if (idx != -1)
					string = string.Left(idx);
			}
			else if (!isFlash(form->m_form->attr2 & SA_CORGB, string, text))
				continue;

			if (form->m_form->isEdit == IE_CORGB)
				m_color = true;

			if (m_guard->m_status & WS_FCB)
			{
				if (!m_revObs.Lookup(m_ioR[ii], flashK))
				{
					flashK.index = m_ioR[ii];
					flashK.key = 0;
					flashK.color = form->GetBkColor();
				}
				flashK.time = tick;

				form->SetBkColor(m_guard->m_flash);
				form->WriteData(string);
				m_revObs.SetAt(m_ioR[ii], flashK);
			}
			else	form->WriteData(string);
		}
		if (m_color)	DominoForm();
	}

	if (!minor.IsOn())
		return false;
	if (m_notice == NULL)
		return true;

	int	nCols, nRows, idx, kk;
	struct	_cellR *cell;

	form = m_notice;
	if (form->m_form->kind == FM_GRID)
		nRows = ((CfmGrid *)form)->GetValidRows();
	else
		nRows = ((CfmTable *)form)->GetValidRows();
	match = false;

	nCols = HIWORD(form->m_form->vals[1]);

	m_sync.Lock();
	for (kk = 0; kk < nRows; kk++)
	{
		for (idx = 0; idx < nCols; idx++)
		{
			if (minor.Lookup(form->GetName(idx), string))
			{
				form->ReadData(text, false, -1, idx, kk);
				text.TrimRight();

				if (text.Compare(string))
				{
					match = false;
					break;
				}
				else	match = true;
			}
		}

		if (match)	break;
		else if (idx == nCols)
		{
			m_sync.Unlock();
			return false;
		}
	}

	if (nRows == 0)
	{
		for (idx = 0; idx < nCols; idx++)
		{
			if (minor.Lookup(form->GetName(idx), string))
				break;
		}

		if (idx == nCols)
		{
			m_sync.Unlock();
			return false;
		}
	}

	bool	flash = false;

	name = atoi((char *)form->m_form->vals[2]);
	if (match)
	{
		if (fms.Lookup(name, text) && !atoi(text))
		{
			flash = true;
			match = false;
			((CfmGrid *)form)->RemoveRow(kk);
		}
	}
	else if (!fms.Lookup(name, text) || atoi(text))
	{
		if (form->m_form->attr2 & GO_TOP)
		{
			((CfmGrid *)form)->InsertRow(0);
			kk = 0;
		}
		else
		{
			((CfmGrid *)form)->InsertRow(-1);
			kk = ((CfmGrid *)form)->GetValidRows();
			if (--kk < 0)	kk = 0;
		}
		match = true;
	}

	if (match)
	{
		bool color = false;

		cell = (struct _cellR *) &m_cellR[form->m_form->vals[0]];
		for (idx = 0; idx < nCols; idx++, cell++)
		{
			if (fms.Lookup(form->GetName(idx), text))
			{
				if (cell->isEdit == IE_CORGB)
					color = true;
				flash = true;
				form->WriteData(text, true, idx, kk);
			}
		}
		if (color)	form->SetDomino();
	}

	if (flash)
		OnProfit(form);
	m_sync.Unlock();
	return flash;
}

void CScreen::OnClear()
{
	int	key, row, col;
	struct	_flashK	flashK;

	if (m_revObs.IsEmpty())
		return;

	m_sync.Lock();
	ULONGLONG tickV = GetTickCount64();

	for (POSITION pos = m_revObs.GetStartPosition(); pos; )
	{
		m_revObs.GetNextAssoc(pos, key, (struct _flashK &)flashK);
		if (!flashK.time || (tickV - flashK.time) < 1000)
			continue;

		flashK.time = 0;
		m_revObs.SetAt(key, flashK);

		CfmBase* form = GetAtForm(flashK.index);
		if (form && flashK.key != 0)
		{
			key -= flashK.key;
			row = key / HIWORD(form->m_form->vals[1]);
			col = key % HIWORD(form->m_form->vals[1]);

			form->SetBkColor(flashK.color, col, row);
		}
		else
			form->SetBkColor(flashK.color);
	}
	m_sync.Unlock();
}

void CScreen::Blink()
{
	int	key, color;
	struct	_blink	blink;

	if (m_blink.IsEmpty())
		return;

	m_sync.Lock();
	ULONGLONG tickV = GetTickCount64();

	for (POSITION pos = m_blink.GetStartPosition(); pos; )
	{
		m_blink.GetNextAssoc(pos, key, (struct _blink &)blink);
		if (tickV - blink.time < 500)
			continue;

		switch (blink.form->m_form->kind)
		{
		case FM_GRID:
			color = blink.form->GetBkColor(HIWORD(blink.key), LOWORD(blink.key));
			blink.form->SetBkColor(blink.color, HIWORD(blink.key), LOWORD(blink.key));
			break;
		case FM_PANEL:
			color = blink.form->GetFgColor();
			blink.form->SetFgColor(blink.color);
			break;
		default:
			color = blink.form->GetBkColor();
			blink.form->SetBkColor(blink.color);
			break;
		}
		blink.time  = tickV;
		blink.color = color;
		m_blink.SetAt(key, blink);
	}

	m_sync.Unlock();
}

void CScreen::Blink(CfmBase* form, BOOL enable, int color)
{
	int	key;
	struct	_blink	blink;

	if (!FindForm((char *)form->m_form->name, key))
		return;

	m_sync.Lock();

	if (!m_blink.Lookup(key, blink))
	{
		if (enable)
		{
			if (form->m_form->kind == FM_PANEL)
			{
				if (color >= 0)
					form->SetFgColor(color);
				blink.color = form->m_form->tRGB;
				blink.rgb   = form->m_form->tRGB;
			}
			else
			{
				if (color >= 0)
					form->SetBkColor(color);
				blink.color = form->m_form->pRGB;
				blink.rgb   = form->m_form->pRGB;
			}
			blink.form  = form;
			blink.time  = GetTickCount64();
			m_blink.SetAt(key, blink);
			m_client->SetBlink();
		}
	}
	else if (!enable)
	{
		if (form->m_form->kind == FM_PANEL)
			form->SetFgColor(blink.rgb);
		else
			form->SetBkColor(blink.rgb);
		m_blink.RemoveKey(key);
	}

	m_sync.Unlock();
}

void CScreen::Blink(CfmBase* form, int key, BOOL enable, int color)
{
	int	idx, clr;
	int	row, col, nCols;
	struct	_blink	blink;

	if (!FindForm((char *)form->m_form->name, idx))
		return;

	row = LOWORD(key);
	col = HIWORD(key);
	nCols = HIWORD(form->m_form->vals[1]);
	idx   = GetFlashBaseKey(idx) + row*nCols+col;
	m_sync.Lock();
	if (!m_blink.Lookup(idx, blink))
	{
		if (enable)
		{
			clr = form->GetBkColor(col, row);
			if (color >= 0)
				form->SetBkColor(color, col, row);
			blink.form  = form;
			blink.key   = key;
			blink.time  = GetTickCount64();
			blink.color = clr;
			blink.rgb   = clr;
			m_blink.SetAt(idx, blink);
			m_client->SetBlink();
		}
	}
	else if (!enable)
	{
		form->SetBkColor(blink.rgb, col, row);
		m_blink.RemoveKey(idx);
	}

	m_sync.Unlock();
}

void CScreen::ResetRTM(char* name)
{
	int	key, row, col;
	struct	_flashK	flashK;
	CDWordArray	keys;

	if (m_flashObs.IsEmpty() || m_revObs.IsEmpty())
		return;

	for (POSITION pos = m_revObs.GetStartPosition(); pos; )
	{
		m_revObs.GetNextAssoc(pos, key, (struct _flashK &)flashK);
		if (flashK.key != 0)
		{
			CfmBase* form = GetAtForm(flashK.index);
			if (!form || strcmp((char *)form->m_form->name, name))
				continue;

			key -= flashK.key;
			row = key / HIWORD(form->m_form->vals[1]);
			col = key % HIWORD(form->m_form->vals[1]);

			form->SetBkColor(flashK.color, col, row);
			keys.Add(key);
		}
	}

	for (key = 0; key < keys.GetSize(); key++)
		m_revObs.RemoveKey(keys.GetAt(key));
	keys.RemoveAll();
}

int CScreen::GetFlashBaseKey(int index)
{
	int	key = m_mapH->formN;

	for (int ii = 0; ii < index; ii++)
	{
		switch (m_formR[ii].kind)
		{
		case FM_TABLE:
			if (m_formR[ii].iok == EIO_NOP)
				continue;
			[[fallthrough]];
		case FM_GRID:
			break;
		default:
			continue;
		}

		key += LOWORD(m_formR[ii].vals[1]) * HIWORD(m_formR[ii].vals[1]);
	}

	return key;
}

bool CScreen::isFlash(int isEdit, CString string, CString text)
{
	double	value, cval;

	if (!GetValue(isEdit, string, value))
		return string.Compare(text) ? true : false;

	if (!GetValue(isEdit, text, cval))
		return string.Compare(text) ? true : false;

	return (value == cval) ? false : true;
}

bool CScreen::GetValue(int isEdit, CString text, double &value)
{
	int	ii, index;
	int	conVal = 0;
	bool	sign = false;

	text.TrimLeft();
	text.TrimRight();

	if (text.IsEmpty())
	{
		value = 0;
		return true;
	}

	ii = 0;
	if (text[ii] & 0x80)
		conVal = (text[ii++] & 0xff) << 8;
	conVal |= text[ii] & 0xff;

	index = 0;
	switch (conVal)
	{
	case 0xa1e3:
	case 0xa1e8:
	case 0xa3ab:
		index++;
		[[fallthrough]];
	case '+':
		index++;
		break;

	case 0xa1e5:
	case 0xa1e9:
	case 0xa3ad:
		index++;
		[[fallthrough]];
	case '-':
		sign = true;
		index++;
		break;
	case 0:
		value = 0;
		return true;
	default:
		if (conVal < '0' || conVal > '9')
			return false;
		break;
	}

	for (; index < text.GetLength(); index++)
	{
		if (!_istspace(text[index]) && text[index] != '0')
			break;
	}

	for (ii = index; ii < text.GetLength(); ii++)
	{
		switch (text[ii])
		{
		case '.':
		case ',':
			continue;
		default:
//			if ((text[ii] & 0x80) || !isdigit(text[ii]))
//			if (text[ii] < '0' || text[ii] > '9')
			if (!isdigit(static_cast<unsigned char>(text[ii])))
				return false;
			continue;
		}
	}

	value = atof(text.Mid(index));

	switch (isEdit)
	{
	case SA_COSUP:
		break;
	default:
		if (sign) value = -value;
		break;
	}
	return true;
}

void CScreen::ChangeTR(char *trxC)
{
	FillMemory(m_mapH->trxC, L_TRXC, ' ');
	for (int ii = 0; ii < L_TRXC; ii++)
	{
		if (trxC[ii] <= ' ')
			break;
		m_mapH->trxC[ii] = trxC[ii];
	}
}

bool CScreen::isKeyButton(WPARAM wParam)
{
	CfmBase* form;

	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		form = GetAtForm(ii);
		if (!form || form->m_form->kind != FM_BUTTON)
			continue;
		switch (form->m_form->type)
		{
		case BTN_USER:
			if (form->m_form->keys != wParam)
				break;

			if (form->IsEnable() && m_client->m_vm->IsAvailable(this, evClick, form))
			{
				m_client->m_vm->OnClick(this, form);
				return true;
			}
			[[fallthrough]];
		default:
			break;
		}
	}
	return false;
}

bool CScreen::OnTRAN(bool byKey)
{
	int	guide;
	CfmBase* form;

	m_skip = true;
	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		form = GetAtForm(ii);
		if (!form) continue;

		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			CScreen* uob;
			if (m_client->GetAtScreen(uob, form->m_form->keys) && uob->isUob())
			{
				if (!uob->OnTRAN(byKey))
					return false;
			}
			continue;
		case FM_GRID:
			if (form->m_form->iok == EIO_OUTPUT)
				continue;
			[[fallthrough]];
		case FM_EDIT:
		case FM_COMBO:
		case FM_MEMO:
			break;
		default:
			continue;
		}

		//if (!(form->m_form->attr & FA_SKIP))  //��ŵ�� ���ִ� 
		{
			if (!form->IsValid(guide))
			{
				m_client->SetCPos(this, ii);
				m_guard->SetGuide(guide, m_client->m_key);
				return false;
			}
		}

		if (!OnChange(ii, byKey))
		{
			m_client->SetCPos(this, ii);
			return false;
		}
	}

	m_xscreen->m_return = TRUE;
	if (m_client->m_vm->OnSend(this) && m_xscreen->m_return)
	{
		m_skip = false;
		return true;
	}
	return false;
}

bool CScreen::OnChange(int idx, bool byKey)
{
	CfmBase* form = GetAtForm(idx);
	if (!form || !m_client->m_vm->IsAvailable(this, evChange, form))
		return true;

	switch (form->m_form->kind)
	{
	case FM_MEMO:
	case FM_EDIT:
	case FM_COMBO:
	case FM_TREE:
	case FM_SHEET:
	case FM_GRID:
		if (form->IsChanged())
		{
			if (!m_client->m_vm->OnChange(this, form, byKey))
				return false;
		}
		break;
	default:
		break;
	}

	return true;
}

void CScreen::OnClick(CfmBase* form, int result)
{
	CScreen* screen = this;

	while (screen->isUob() && !screen->isMain())
	{
		if (!m_client->GetAtScreen(screen, screen->m_parent))
			return;			// maybe does not happen
	}

	switch (result)
	{
	case RC_SCRUP:
		m_client->m_vms.scroll = SB_UP;
		if (screen->m_trxS & trxK_SCUP)
			screen->m_trxK = trxK_SCUP;
		break;
	case RC_SCRDN:
		m_client->m_vms.scroll = SB_DN;
		if (screen->m_trxS & trxK_SCDN)
			screen->m_trxK = trxK_SCDN;
		break;
	case RC_PAGEUP:
		m_client->m_vms.scroll = SB_PU;
		if (screen->m_trxS & trxK_PGUP)
			screen->m_trxK = trxK_PGUP;
		break;
	case RC_PAGEDN:
		m_client->m_vms.scroll = SB_PD;
		if (screen->m_trxS & trxK_PGDN)
			screen->m_trxK = trxK_PGDN;
		break;
	case RC_FIRST:
		m_client->m_vms.scroll = SB_FR;
		if (screen->m_trxS & trxK_FIRST)
			screen->m_trxK = trxK_FIRST;
		break;
	default:
		return;
	}

	if (m_client->m_vm->IsAvailable(this, evClick, form))
	{
		m_client->m_vm->OnClick(this, form);
		m_client->m_vms.scroll = SB_NS;
	}

	if (screen->m_trxK || screen->m_mapH->options & OP_OOP || form->m_form->attr2 & GO_HEADER)
		m_client->m_stream->InStream(screen, true);
}

void CScreen::SetCells(CfmBase* form, CString text, bool update, bool grid_ini)
{
	struct	_cellR *cell;
	cell = (struct _cellR *) &m_cellR[form->m_form->vals[0]];
	int nCols = HIWORD(form->m_form->vals[1]);

	int	idx;
	CString	tmps, name;

	CPtrArray*	grid = new CPtrArray();
	name = text;

	if (grid_ini)
	{
		while (!text.IsEmpty())
		{
			idx = text.Find('|');
			if (idx == -1)
			{
				tmps = text;
				text.Empty();
			}
			else
			{
				tmps = text.Left(idx++);
				if (!tmps.Compare("-"))
					break;
				text = text.Mid(idx);
			}
			for (idx = 0; idx < nCols; idx++)
			{
				if (!strcmp((char *)cell[idx].name, tmps))
				{
					grid->Add(&cell[idx]);
					break;
				}
			}
		}
	}
	else
	{
		for (idx = 0; idx < form->m_form->type; idx++)
			grid->Add(&cell[idx]);

		while (!text.IsEmpty())
		{
			idx = text.Find('|');
			if (idx == -1)
			{
				tmps = text;
				text.Empty();
			}
			else
			{
				tmps = text.Left(idx++);
				if (!tmps.Compare("-"))
					break;
				text = text.Mid(idx);
			}
			for (idx = form->m_form->type; idx < nCols; idx++)
			{
				if (!strcmp((char *)cell[idx].name, tmps))
				{
					grid->Add(&cell[idx]);
					break;
				}
			}
		}
	}

	m_sync.Lock();

	CObject	*object;
	if (m_cells.Lookup((char *)form->m_form->name, object))
		delete object;

	if (grid->GetSize() > form->m_form->type)
		m_cells.SetAt((char *)form->m_form->name, grid);
	else			// set default cells
	{
		m_cells.RemoveKey((char *)form->m_form->name);
		name.Empty();
		delete grid;
	}
	ResetRTM((char *)form->m_form->name);
	form->SetEnum(name, update, grid_ini ? 200 : -1);
	m_sync.Unlock();
}

void CScreen::Reset()
{
	CFile	file;
	CString path = m_guard->PathMAP(CString(m_mapH->mapN, L_MAPN));
	if (!file.Open(path, CFile::typeBinary|CFile::modeRead))
		return;

	DWORD mapL = (DWORD)file.GetLength();
	if (mapL < L_MAPH)
	{
		file.Close();
		return;
	}

	char* mapB = new char[mapL];
	if (file.Read(mapB, mapL) != mapL)
	{
		delete [] mapB;
		file.Close();
		return;
	}
	file.Close();

	struct	_mapH* mapH = (struct _mapH *) mapB;

	struct	_formR* formR;
	if (mapH->formR != NOVALUE)
		formR = (struct _formR *) &mapB[mapH->formR];
	else
		return;

	struct	_cellR* cellR = nullptr;
	if (mapH->cellR != NOVALUE)
		cellR = (struct _cellR *) &mapB[mapH->cellR];

	CfmBase* form;

	for (int ii = 0; ii < mapH->formN; ii++, formR++)
	{
		switch (formR->kind)
		{
		case FM_BUTTON:
			switch (formR->type)
			{
			case BTN_CHECK:
			case BTN_RADIO:
				break;
			default:
				continue;
			}
			[[fallthrough]];
		case FM_EDIT:
		case FM_COMBO:
		case FM_OUT:
		case FM_MEMO:
		case FM_TABLE:
			form = GetAtForm(ii);
			if (!form) break;

			form->m_form->attr = formR->attr;
			form->SetBkColor(formR->pRGB);
			form->SetFgColor(formR->tRGB);
			form->SetEnable(formR->properties & PR_ENABLE ? true : false);
			form->SetVisible(formR->properties & PR_VISIBLE ? true : false);
			break;
		case FM_GRID:
		{
			if (!(formR->attr2 & GO_TABLE))
				continue;
			form = GetAtForm(ii);
			if (!form) break;
			form->SetVisible(formR->properties & PR_VISIBLE ? true : false);

			if (formR->vals[0] == NOVALUE)
				continue;

			int	nRows, nCols;
			struct	_cellR* cell = nullptr;

			nRows = LOWORD(formR->vals[1]);
			nCols = HIWORD(formR->vals[1]);
			cell  = (struct _cellR *) &cellR[formR->vals[0]];

			int	idx, kk;

			for (kk = 0; kk < nRows; kk++)
			{
				((CfmGrid *)form)->SetCellRow(kk+1);
				for (idx = 0; idx < nCols; idx++)
				{
					if (formR->attr2 & GO_ROW)
					{
						if (kk % 2)
							form->SetBkColor(m_valueR[formR->vals[3]+1], idx, kk);
						else
							form->SetBkColor(m_valueR[formR->vals[3]], idx, kk);
					}
					else	form->SetBkColor(cell[idx].pRGB, idx, kk);

					form->SetFgColor(cell[idx].tRGB, idx, kk);
					form->SetAttributes(cell[idx].attr & FA_REF, false, idx, kk);
					form->SetAttributes(cell[idx].attr & ~FA_REF, true, idx, kk);
				}
			}
		}
			[[fallthrough]];
		default:
			continue;
		}
	}

	delete[] mapB;
}

void CScreen::GuideLedger()
{
	if (m_ledger == NULL)
		return;

	CString	etype, ecode, msg;
	COLORREF rgb{};

	etype = m_guard->GetLedger(m_ledger, getEtype);
	if (atoi(etype) < 0)
		return;
	ecode = m_guard->GetLedger(m_ledger, getEcode);
	msg = m_guard->GetLedger(m_ledger, getMsg);
	msg.TrimRight();

	if (etype.GetAt(0) == '0' && ecode.GetAt(0) == '0')
		rgb = PALETTERGB(0, 0, 0);
	else
		rgb = PALETTERGB(255, 0, 0);

	msg.TrimRight();
	bool guide = m_client->m_stream->SetGuide(this, msg, rgb);

	if (!(m_guard->m_status & WS_NOMSG) && etype.GetAt(0) == '1')
		::MessageBox(m_view->GetSafeHwnd(), msg, m_guard->GetString(AS_ERROR, etype) ? etype : _T("ERROR"), MB_OK);
	else if (!guide)
		m_guard->SetGuide(msg, m_client->m_key);
}

void CScreen::SetLedger(char* ledger)
{
	if (m_ledger)
		m_guard->SetLedger(m_ledger, 0, -1, ledger);
}

void CScreen::GetLedger(char* ledger)
{
	if (m_ledger)
	{
		CString	text;

		text = m_guard->GetLedger(m_ledger, 0, -1);
		CopyMemory(ledger, text, text.GetLength());
	}
}

void CScreen::SetLedger(int pos, int length, CString item)
{
	if (m_ledger)
		m_guard->SetLedger(m_ledger, pos, length, (char *)item.operator LPCTSTR());
}

CString CScreen::GetLedger(int pos, int length)
{
	if (m_ledger)
		return m_guard->GetLedger(m_ledger, pos, length);

	return _T("");
}

void CScreen::OnProfit(CfmBase* form)
{
	if (m_profit == NULL || m_profit->m_name.CompareNoCase((char *)form->m_form->name))
		return;

	CfmBase* base;
	int	nCols, nRows;

	nCols = HIWORD(form->m_form->vals[1]);
	nRows = ((CfmGrid *)form)->GetValidRows();

	if (nRows <= 0)
	{
		for (POSITION pos = m_profit->m_sum.GetStartPosition(); pos; )
		{
			m_profit->m_sum.GetNextAssoc(pos, nCols, base);
			base->WriteData("");
		}
		return;
	}

	double	Buy, Value, Profit;
	int	dir, unit, value, profit, rate, cost, fee, tax, type, mi_price, mo_price, x_fee, x_tax, x_money;

	dir = unit = value = profit = rate = cost = fee = tax = type = mi_price = mo_price = x_fee = x_tax = x_money = -1;
	m_profit->m_cell.Lookup('D', dir);
	m_profit->m_cell.Lookup('V', value);
	m_profit->m_cell.Lookup('P', profit);
	m_profit->m_cell.Lookup('R', rate);
	m_profit->m_cell.Lookup('U', unit);
	m_profit->m_cell.Lookup('f', cost);
	m_profit->m_cell.Lookup('F', fee);
	m_profit->m_cell.Lookup('T', tax);
	m_profit->m_cell.Lookup('K', type);
	m_profit->m_cell.Lookup('M', mi_price);
	m_profit->m_cell.Lookup('m', mo_price);
	m_profit->m_cell.Lookup('E', x_fee);
	m_profit->m_cell.Lookup('X', x_tax);
	m_profit->m_cell.Lookup('N', x_money);

	Buy = Value = Profit = 0;

	CString	text, format;
	int	buy = 0, price = 0, amount = 0;
	double	tmpv, sum, v_price, v_amount, v_tax, v_unit, v_charge;

	if (m_profit->m_fees)
	{
		m_profit->m_fees->ReadData(text);
		m_profit->m_fee = atof(text);
	}
	if (m_profit->m_taxs)
	{
		m_profit->m_taxs->ReadData(text);
		m_profit->m_tax = atof(text);
	}
	if (m_profit->m_moneys)
	{
		m_profit->m_moneys->ReadData(text);
		m_profit->m_money = atof(text);
	}
	if (m_profit->m_charges)
	{
		m_profit->m_charges->ReadData(text);
		m_profit->m_charge = atoi(text) ? true : false;
	}

	if (m_profit->m_cell.Lookup('B', buy) && m_profit->m_cell.Lookup('C', price) &&	m_profit->m_cell.Lookup('A', amount))
	{
		v_unit = 1.;
		format = _T("%.f");
		for (int ii = 0; ii < nRows; ii++)
		{
			v_charge = 0;
			form->ReadData(text, false, -1, price, ii);
			sum = v_price = atof(text);
			form->ReadData(text, false, -1, amount, ii);
			tmpv = atof(text);
			sum *= tmpv;
			if (sum == 0)	continue;

			v_amount = tmpv;
			if (unit != -1)
			{
				form->ReadData(text, false, -1, unit, ii);
				v_unit = atof(text);
				sum *= v_unit;
				v_price *= v_unit;
			}

			form->ReadData(text, false, -1, buy, ii);
			tmpv = atof(text);
			Buy += tmpv;

			v_tax = 0;
			if (type != -1)
			{
				int	v_type;
				double	mx_tax;

				form->ReadData(text, false, -1, type, ii);
				v_type = text.GetAt(0);
				text.Delete(0);
				format.Format("%%.0%df", atoi(text));

				switch (v_type)
				{
				case '1':		// ETF
					break;
				default:
					if (x_tax != -1)
					{
						form->ReadData(text, false, -1, x_tax, ii);
						mx_tax = atof(text);
					}
					else
						mx_tax = m_profit->m_tax;

					text.Format("%.f", v_amount * v_price * mx_tax);
					v_tax = atof(text);
					if (tax != -1)
						form->WriteData(text, true, tax, ii);
					break;
				}
			}

			if (cost != -1 || fee != -1 || m_profit->m_charge)
			{
				double	mx_fee, mx_money;

				if (x_fee != -1)
				{
					form->ReadData(text, false, -1, x_fee, ii);
					mx_fee = atof(text);
				}
				else
					mx_fee = m_profit->m_fee;

				if (x_money != -1)
				{
					form->ReadData(text, false, -1, x_money, ii);
					mx_money = atof(text);
				}
				else
					mx_money = m_profit->m_money;

				v_charge = v_amount * v_price;
				if (v_charge < mx_money)
					text.Format("%.f", mx_money * mx_fee / 10. - 0.5);
				else if (v_charge * mx_fee < 10.)
					text.Empty();
				else
					text.Format("%.f", v_charge * mx_fee / 10. - 0.5);
				v_charge = atof(text) * 10.;

				if (tmpv < mx_money)
					text.Format("%.f", mx_money * mx_fee / 10. - 0.5);
				else if (tmpv * mx_fee < 10.)
					text.Empty();
				else
					text.Format("%.f", tmpv * mx_fee / 10. - 0.5);
				v_charge += atof(text) * 10.;

				if (fee != -1)
				{
					text.Format("%.f", v_charge);
					form->WriteData(text, true, fee, ii);
				}

				v_charge += v_tax;
				if (cost != -1)
				{
					text.Format("%.f", v_charge);
					form->WriteData(text, true, cost, ii);
				}
				if (m_profit->m_charge)
					sum -= v_charge;
				if (mi_price != -1 && mo_price != -1)
				{
					text.Format(format, v_charge / (v_amount * v_unit));
					v_charge = atof(text);
					form->ReadData(text, false, -1, mi_price, ii);
					v_charge += atof(text);
					text.Format(format, v_charge);
					form->WriteData(text, true, mo_price, ii);
				}
			}

			if (value != -1)
			{
				if (sum)
				{
					text.Format("%.f", sum);
					form->WriteData(text, true, value, ii);
				}
				else	form->WriteData("", true, value, ii);
			}
			Value += sum;

			if (dir != -1)
			{
				form->ReadData(text, false, -1, dir, ii);
				if (text.Find(_T("매수")) != -1 || text.Find('S') != -1)
					sum = tmpv + (tmpv - (sum + m_profit->m_charge ? v_charge * 2 : 0));
			}

			sum -= tmpv;
			if (profit != -1)
			{
				text.Format(sum > 0 ? "+%.f" : "%.f", sum);
				form->WriteData(text, true, profit, ii);
			}
			if (tmpv)
				Profit += sum;

			if (rate != -1 && tmpv)
			{
				sum *= 100;
				sum /= tmpv;
				text.Format(sum > 0 ? "+%.02f" : "%.02f", sum);
				form->WriteData(text, true, rate, ii);
			}
		}
	}

	int	idx;
	if (nRows > 0)
	{
		struct	_cellR *cell;

		cell  = (struct _cellR *) &m_cellR[form->m_form->vals[0]];
		for (int ii = 0; ii < nCols; ii++)
		{
			if (cell[ii].attr & FA_ZEROSUP && (cell[ii].attr2 & SA_ZEROSUP) == SA_ASTER &&
						cell[ii].attr & FA_REF && cell[ii].vals[0] != NOVALUE)
			{
				for (int kk = 0; kk < nRows; kk++)
				{
					form->ReadData(text, false, -1, ii, kk);
					tmpv = atof(text);
					if (amount != -1)
					{
						form->ReadData(text, false, -1, amount, kk);
						tmpv *= atof(text);
					}
					if (dir != -1)
					{
						form->ReadData(text, false, -1, dir, kk);
						if (text.Find(_T("매수")) != -1 || text.Find('S') != -1)
							tmpv = -tmpv;
					}
					if (tmpv)
					{
						text.Format(tmpv > 0 ? "+%.04f" : "%.04f", tmpv);
						form->WriteData(text, true, cell[ii].vals[0], kk);
					}
					else	form->WriteData("", true, cell[ii].vals[0], kk);
				}
			}
		}
	}

	bool	floating;
	for (POSITION pos = m_profit->m_sum.GetStartPosition(); pos; )
	{
		m_profit->m_sum.GetNextAssoc(pos, idx, base);
		if (idx & floatSUM)
		{
			floating = true;
			idx &= ~floatSUM;
		}
		else	floating = false;

		if (idx == rate)
		{
			if (Buy)			// Profit && Buy
			{
				sum = Profit * 100. / Buy;
				text.Format(sum > 0 ? "+%.02f" : "%.02f", sum);
				base->WriteData(text);
			}
			else	base->WriteData("");

			continue;
		}

		if (idx == buy)
			sum = Buy;
		if (idx == value)
			sum = Value;
		else
		{
			sum = 0;
			for (int ii = 0; ii < nRows; ii++)
			{
				form->ReadData(text, false, -1, idx, ii);
				sum += atof(text);
			}
		}

		switch (base->m_form->isEdit)
		{
		case IE_CORGB:
			if (!sum)
				text.Empty();
			else if (floating)
				text.Format(sum > 0 ? "+%.04f" : "%.04f", sum);
			else
				text.Format(sum > 0 ? "+%.f" : "%.f", sum);
			break;
		default:
			if (floating)
				text.Format("%.04f", sum);
			else
				text.Format("%.f", sum);
			break;
		}
		base->WriteData(text);
	}
}

void CScreen::EnableForm(bool enable)
{
	CfmBase* form;

	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		form = GetAtForm(ii);
		if (form) form->SetEnable(enable);
	}
}

void CScreen::SetPalette()
{
	CScreen* screen;
	CfmBase* form;
	COLORREF rgb{};
	int	value;
	struct	_cellR *cell;

	if (isMain() && m_mapH->imageK == IK_BRUSH)
	{
		if (!(m_mapH->imageV & 0x02000000) && m_guard->m_palette->Lookup(m_mapH->imageV, rgb))
			m_client->SetBK(m_guard->GetRGB(m_mapH->imageV), true);
	}

	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		form = GetAtForm(ii);
		if (!form) continue;

		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			if (m_client->GetAtScreen(screen, form->m_form->keys))
			{
				if (screen->m_mapH->imageK == IK_BRUSH && !(screen->m_mapH->imageV & 0x02000000))
				{
					if (m_guard->m_palette->Lookup(screen->m_mapH->imageV, rgb))
						form->SetBkColor(rgb);
				}
				screen->SetPalette();
			}
			continue;

		case FM_GRID:
			cell = (struct _cellR *)&m_cellR[form->m_form->vals[0]];
			for (value = 0 ; value < HIWORD(form->m_form->vals[1]); value++, cell++)
			{
				if (!(cell->hpRGB & 0x02000000))
				{
					if (m_guard->m_palette->Lookup(cell->hpRGB, rgb))
						form->SetBkColor(rgb, value, -1);
				}
				if (!(cell->pRGB & 0x02000000))
				{
					if (m_guard->m_palette->Lookup(cell->pRGB, rgb))
					{
						for (int jj = 0; jj < LOWORD(form->m_form->vals[1]); jj++)
							form->SetBkColor(rgb, value, jj);
					}
				}
			}

			if (form->m_form->bRGB != NOVALUE && !(form->m_form->bRGB & 0x02000000))
			{
				if (m_guard->m_palette->Lookup(form->m_form->bRGB, rgb))
					form->SetEnum(_T(""), rgb, 98);
			}

			if (form->m_form->attr2 & GO_ROW && form->m_form->vals[3] != NOVALUE)
			{
				DWORD	fRGB, sRGB;

				value = form->m_form->vals[3];
				fRGB = sRGB = 0xffffffff;
				if (!(m_valueR[value] & 0x02000000))
					m_guard->m_palette->Lookup(m_valueR[value], fRGB);
				if (!(m_valueR[value+1] & 0x02000000))
					m_guard->m_palette->Lookup(m_valueR[value+1], sRGB);
				if (fRGB != 0xffffffff || sRGB != 0xffffffff)
					((CfmGrid *)form)->SetRowColor(fRGB, sRGB);
			}

			if ((form->m_form->isEdit == FI_RBAR || form->m_form->isEdit == FI_RREC)
				&& form->m_form->onEdit != NOVALUE && !(form->m_form->onEdit & 0x02000000))
			{
				if (m_guard->m_palette->Lookup(form->m_form->onEdit, rgb))
					form->SetBkColor(rgb, 0, -2);
			}
			continue;

		case FM_TABLE:
			if (form->m_form->bRGB != NOVALUE && !(form->m_form->bRGB & 0x02000000))
			{
				if (m_guard->m_palette->Lookup(form->m_form->bRGB, rgb))
					form->SetEnum(_T(""), rgb, 98);
			}
			cell = (struct _cellR *)&m_cellR[form->m_form->vals[0]];
			for (value = 0; value < LOWORD(form->m_form->vals[1]); value++, cell++)
			{
				if (!(cell->hpRGB & 0x02000000) && m_guard->m_palette->Lookup(cell->hpRGB, rgb))
					form->SetBkColor(rgb, -1, value);
				if (!(cell->pRGB & 0x02000000) && m_guard->m_palette->Lookup(cell->pRGB, rgb))
					form->SetBkColor(rgb, 0, value);
			}
			continue;

		case FM_CONTROL:
			form->SetBkColor(rgb);
			continue;

		case FM_TAB:
		case FM_UTAB:
			if (form->m_form->type == TAB_IMAGE && form->m_form->vals[2] != NOVALUE)
			{
				form->SetEnum(_T(""), 1, 100);
				if (form->m_form->vals[3] != NOVALUE)
				{
					if (!(m_valueR[form->m_form->vals[3]+1] & 0x02000000))
						if (m_guard->m_palette->Lookup(m_valueR[form->m_form->vals[3]+1], rgb))
							form->SetFgColor(rgb, 0);
				}
				if (!(form->m_form->tRGB & 0x020000) && m_guard->m_palette->Lookup(form->m_form->tRGB, rgb))
					form->SetFgColor(rgb);
				continue;
			}
			if (form->m_form->vals[3] != NOVALUE)
			{
				if (!(m_valueR[form->m_form->vals[3]] & 0x02000000))
					if (m_guard->m_palette->Lookup(m_valueR[form->m_form->vals[3]], rgb))
						form->SetBkColor(rgb, 0);
				if (!(m_valueR[form->m_form->vals[3]+1] & 0x02000000))
					if (m_guard->m_palette->Lookup(m_valueR[form->m_form->vals[3]+1], rgb))
						form->SetFgColor(rgb, 0);
			}
			if (!(form->m_form->pRGB & 0x020000) && m_guard->m_palette->Lookup(form->m_form->pRGB, rgb))
				form->SetBkColor(rgb);
			if (!(form->m_form->tRGB & 0x020000) && m_guard->m_palette->Lookup(form->m_form->tRGB, rgb))
				form->SetFgColor(rgb);
			continue;

		case FM_PANEL:
		case FM_OUT:
			if (form->m_form->bRGB != NOVALUE && !(form->m_form->bRGB & 0x02000000))
			{
				if (m_guard->m_palette->Lookup(form->m_form->bRGB, rgb))
					form->SetEnum(_T(""), rgb, 98);
			}
			[[fallthrough]];
		default:
			if (form->m_form->pRGB & 0x02000000)
				continue;
			break;
		}
	
		if (m_guard->m_palette->Lookup(form->m_form->pRGB, rgb))
			form->SetBkColor(rgb);
	}
}

void CScreen::SetControl(CString data)
{
	int	pos = data.Find('\t');
	if (pos == -1)
		return;

	CfmBase* form;
	if (FindForm(data.Left(pos), form))
	{
		data = data.Mid(pos+1);
		pos  = data.Find('\t');
		if (pos == -1)
			pos = data.GetLength();
		else
			form->SetFgColor(atoi(data.Mid(pos+1)));
// updateXXX_202202	
		if (form->m_form->kind == FM_CONTROL)
			form->WriteData(data.Left(pos), true, data.Left(pos).GetLength());
		else
			form->WriteData(data.Left(pos));
		m_client->m_status &= ~S_CURSOR;
		pos = FindForm(form);
		if (pos >= 0)
			OnChange(pos, false);
	}
}

void CScreen::SetFCB(bool blink, int flash)
{
	CfmCtrl* ctrl;

	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		if (m_formR[ii].kind != FM_CONTROL)
			continue;

		ctrl = (CfmCtrl *)GetAtForm(ii);
		if (ctrl) ctrl->SetFCB(blink, flash);
	}
}

void CScreen::OnFile(BOOL upload, char* datB, int datL)
{
	int	pos = 0;
	bool	error = false;

	if (m_mapH->typeH == TH_LEDGER && datL >= m_ledgerL)
	{
		if (!atoi(m_guard->GetLedger(m_ledger, datB, getOK)))
			error = true;
		pos = m_ledgerL;
	}

	if (!error && datL >= (int)(pos+L_fileH) && !upload)
	{
		CFile	file;
		CString	text;
		struct	_fileH*	fileH;

		fileH = (struct _fileH *)&datB[pos];
		if (fileH->fileF == fileF_FIR || fileH->fileF == fileF_ONLY)
			DeleteFile(fileH->fileN);
		pos += L_fileH;
		if (file.Open(fileH->fileN, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite|CFile::modeNoTruncate))
		{
			file.SeekToEnd();
			file.Write(&datB[pos], datL-pos);
			file.Close();
		}
	}
	m_client->m_vm->OnFile(this, upload, datB, datL);
}

void CScreen::DoEvent(int ev, CString syms)
{
	CfmBase* form;

	if (FindForm(syms, form) && form->m_form->kind == FM_CONTROL)
	{
		switch (ev)
		{
		case evOnClick:
			m_client->m_vm->OnClick(this, form);	break;
		case evOnChange:
			m_client->m_vm->OnChange(this, form);	break;
		case evOnDblClk:
			m_client->m_vm->OnDblClick(this, form);	break;
		default:
			break;
		}
	}
}

CString getSomethins(int type)
{
	switch (type)
	{
	case doPUSH:
		return "doPUSH";
	case doPUSHX:
		return "doPUSHX";
	case doPOP:
		return "doPOP";
	case doPOPX:
		return "doPOPX";
	case doRELOAD:
		return "doRELOAD";
	case doBLINK:
		return "doBLINK";
	case doTRIGGER:
		return "doTRIGGER";
	case doKEY:
		return "doKEY";
	case doEXCEL:
		return "doEXCEL";
	case doEXCELx:
		return "doEXCELx";
	case doEXCEL2:
		return "doEXCEL2";
	case doTEXT:
		return "doPOP";
	case doEXPORT:
		return "doEXPORT";
	case doPRINT:
		return "doPRINT";
	case doHITPOS:
		return "doHITPOS";
	case doMODAL:
		return "doMODAL";
	case doCLEAR:
		return "doCLEAR";
	case doSEND:
		return "doSEND";
	case doRSEND:
		return "doRSEND";
	case doOBJECT:
		return "doOBJECT";
	case doIGNORE:
		return "doIGNORE";
	case doENABLE:
		return "doENABLE";
	case doFOCUS:
		return "doFOCUS";
	case doCHANGE:
		return "doCHANGE";
	case doIME:
		return "doIME";
	case doATTR:
		return "doATTR";
	case doRADIO:
		return "doRADIO";
	case doSETFOCUS:
		return "doSETFOCUS";
	case doCURSOR:
		return "doCURSOR";
	case doHTS:
		return "doHTS";
	case doSIZE:
		return "doSIZE";
	case doVERS:
		return "doVERS";
	case doCOMBO:
		return "doCOMBO";
	case doRATIO:
		return "doRATIO";
	case doBACK:
		return "doBACK";
	case doPORTFOLIO:
		return "doPORTFOLIO";
	case doDECOMPRESS:
		return "doDECOMPRESS";
	case 99:
		return "xTrace";
	default:
		return "default";
	}
}

long CScreen::DoSomething(int type, CfmBase* form, WPARAM wParam, LPARAM lParam, int wpL, int lpL)
{
	CScreen* screen;
	CString	string;
	static CString text;
	int	value = 0, row = 0, column = 0;

	m_slog.Format("[WIZARD][SCRIPT][%s]<%d> type =[%s]", __FUNCTION__, __LINE__,getSomethins(type));
	OutputDebugString(m_slog);

	switch (type)
	{
	case doPUSH:
		m_guard->PushData(form, (char *)lParam);
		break;

	case doPUSHX:
		row    = LOWORD(wParam);
		column = HIWORD(wParam);
		m_guard->PushData(form, (char *)lParam, row, column);
		break;

	case doPOP:
		m_guard->PopData(form, (char *)lParam);
		break;

	case doPOPX:
		row    = LOWORD(wParam);
		column = HIWORD(wParam);
		m_guard->PopData(form, (char *)lParam, row, column);
		break;

	case doRELOAD:
		if (form->m_form->kind == FM_OBJECT)
		{
			screen = this;
			string = form->GetSymbolName();
			if (m_client->FindScreen(dpCURRENT, string, screen, true))
				screen->ReloadForm(screen->m_rect);
			break;
		}
		break;

	case doBLINK:
		if (form->m_form->kind == FM_GRID)
			Blink(form, MAKELONG(LOWORD(wParam), LOBYTE(HIWORD(wParam))), HIBYTE(HIWORD(wParam)) ? TRUE : FALSE, (int)lParam); 
		else
			Blink(form, wParam ? TRUE : FALSE, (int)lParam); 
		break;

	case doTRIGGER:
		string = (char *)lParam;
		form->ReadData(text);
		if (wParam)
		{
			if (!string.IsEmpty())
				m_guard->OnTrigger(m_client, (char *)string.operator LPCTSTR(), text);
			else
				m_guard->OnTrigger(m_client, (char *)form->m_form->name, text);
		}
		else
		{
			if (!string.IsEmpty())
				m_client->OnTrigger(string, text, m_key);
			else
				m_client->OnTrigger((char *)form->m_form->name, text, m_key);
		}
		break;

	case doKEY:
		form->OnKey((int)lParam, value);
		OnClick(form, value);
		break;

	case doEXCEL:
		m_client->MakeXlsFile(this, form, LOWORD(wParam) ? true : false, (char *)lParam, HIWORD(wParam) ? true : false);
		break;

	case doEXCELx:
		m_client->MakeXlsFileEx(this, form, HIWORD(wParam) ? true : false);
		break;

	case doEXCEL2:
		m_client->MakeXlsFile2(this, form, LOWORD(wParam) ? true : false, (char *)lParam, HIWORD(wParam) ? true : false);
		break;

	case doTEXT:
		m_client->MakeFile(this, form, (char *)lParam, HIWORD(wParam) ? true : false, LOWORD(wParam) ? true : false);
		break;

	case doEXPORT:
		m_client->Export(this, form, LOWORD(wParam) ? true : false, (char *)lParam);
		break;

	case doPRINT:
		m_client->OnPrint(this, form, true);
		break;

	case doHITPOS:
		return m_client->m_vms.scroll;

	case doMODAL:
		switch (form->m_form->kind)
		{
		case FM_UTAB:
			m_client->EditUTab(this, form);
			break;
		case FM_GRID:
			m_client->EditColumns(this, form);
			break;
		}
		break;

	case doCLEAR:
		screen = this;
		string = form->GetSymbolName();
		if (m_client->FindScreen(dpCURRENT, string, screen, true))
			screen->ClearForm((int)wParam);
		break;

	case doSEND:
		screen = this;
		string = form->GetSymbolName();
		if (m_client->FindScreen(dpCURRENT, string, screen, true))
		{
			if (m_client->m_vm->m_script)
				return 0;	// ignore dup

			m_client->m_stream->InStream(screen, false, wParam ? (char *)wParam : "");
		}
		break;

	case doRSEND:
		screen = this;
		string = form->GetSymbolName();
		if (m_client->FindScreen(dpCURRENT, string, screen, true))
		{
			if (m_client->m_vm->m_script)
				return 0;	// ignore dup

			m_client->OnSend(screen);
		}
		break;

	case doOBJECT:
		screen = this;
		string = form->GetSymbolName();
		if (screen->FindForm(string, value))
		{
			if (m_client->FindScreen(dpCURRENT, string, screen))
			{
				string = (char *)lParam;
				if (string.IsEmpty())
				{
					m_client->OnClose(this);
					m_client->RemoveAtScreen(form->m_form->keys);
					form->Refresh();
				}
				else
					screen->SetObject(value, (char *)lParam, wParam ? true : false);
			}
			else
				m_client->SetObject(this, value, (char *)lParam, wParam ? true : false);
		}
		break;

	case doIGNORE:
		return (m_client->m_status & S_IGNORE) ? 1 : 0;

	case doENABLE:
		screen = this;
		string = form->GetSymbolName();
		if (m_client->FindScreen(dpCURRENT, string, screen, true))
			screen->EnableForm(wParam ? true : false);
		break;

	case doFOCUS:
		screen = this;
		string = form->GetSymbolName();
		value  = FindForm(form);
		if (value >= 0 && m_client->FindScreen(dpCURRENT, string, screen)
			&& m_client->isVisible(screen->m_key, value) && form->IsEnable())
		{
			m_client->SetCPos(screen, value, false);
		}
		break;

	case doCHANGE:
		m_client->m_status &= ~S_CURSOR;
		value = FindForm(form);
		if (value >= 0)
			OnChange(value, false);
		break;

	case doIME:
		if (wParam)
			m_client->m_status |= S_HAN;
		else
			m_client->m_status &= ~S_HAN;
		m_client->ChangeIME(true);
		break;

	case doATTR:
		row    = LOWORD(wParam);
		column = HIWORD(wParam);
		m_client->m_stream->ParseFCC(this, form, (_FCC*)lParam, column, row);
		break;

	case doRADIO:
		if (form && form->m_form->kind == FM_BUTTON && form->m_form->type == BTN_RADIO)
		{
			CfmBase* base;
			int	tab = form->m_form->index;

			for (int ii = 0; ii < m_tabL; ii++)
			{
				if (wParam)		// next
					tab++;
				else
					tab--;

				if (tab < 0 || tab >= m_tabL)
					break;

				base = GetAtForm(m_tabR[tab]);
				if (base && base->m_form->kind == FM_BUTTON && base->m_form->type == BTN_RADIO
					&& base->IsEnable() && base->m_form->keys == form->m_form->keys)
				{
					screen = this;
					string = base->GetSymbolName();
					if (m_client->FindScreen(dpCURRENT, string, screen) && screen->FindForm(string, value))
						m_client->SetCPos(screen, value, false);
					break;
				}
			}
		}
		break;

	case doSETFOCUS:
		if (m_client->m_vm->IsAvailable(this, evSetFocus, form))
		{
			m_client->m_vm->OnSetFocus(this, form);
			return 1;
		}
		break;

	case doCURSOR:
		if (!m_client->m_focus)
			wParam = CS_NONE;
		m_guard->SetCursor(wParam, m_view);
		break;

	case doHTS:
		value = m_guard->m_app->GetProfileInt(WORKSTATION, "HTS", 0);
		return value;

	case doSIZE:
		screen = this;
		if (m_client->GetAtScreen(screen, form->m_form->keys))
		{
			CPoint	pt;
			CRect	rect = form->GetRect(true);
			float hRatio = (float)rect.Width() / (float)screen->m_mapH->width;
			float vRatio = (float)rect.Height() / (float)screen->m_mapH->height;

			screen->m_rect = rect;
			pt = CPoint(rect.left, rect.top);
			for (int ii = 0; ii < screen->m_mapH->formN; ii++)
			{
				CfmBase* base = screen->GetAtForm(ii);
				ASSERT_VALID(base);
				if (wParam)
					base->Adjust(pt, (float)max(1.0, hRatio), vRatio);
				else
					base->Adjust(pt, hRatio, vRatio);
				base->Resize(screen->m_hRatio, screen->m_vRatio, wParam ? false : true, true);
				if (base->m_form->kind == FM_OBJECT)
					screen->DoSomething(doSIZE, base, wParam);
			}
		}
		break;

	case doVERS:
		return m_guard->isVersion((char *)lParam, ".dll");

	case doCOMBO:
		if (wParam)
			m_client->m_status |= S_COMBO;
		else
			m_client->m_status &= ~S_COMBO;
		break;

	case doRATIO:
		if (wParam)
			text.Format("%.f", m_client->m_hR);
		else
			text.Format("%.f", m_client->m_vR);
		return (long)text.operator LPCTSTR();

	case doBACK:
		if (m_client->GetAtScreen(screen, form->m_form->keys))
			return screen->IsRectVisible((CDC *)wParam);
		return TRUE;

	case doPORTFOLIO:
		return m_guard->IsPortfolioSymbol((char *)lParam);

	case doDECOMPRESS:
		return m_guard->lz4dec((char *)(wParam), wpL, (char *)lParam, lpL);

	case 99:
		m_guard->xTRACE(x_STRs, (char *)lParam);
		break;
	default:
		break;
	}

	return 0;
}

int CScreen::DoComboBox(CfmBase* form)
{
	int	idx;
	
	idx = FindForm(form);
	if (idx >= 0)
	{
		if (form->m_form->kind == FM_COMBO && (form->m_form->properties & PR_ACCN))
		{
			CString	data, text;

			form->ReadData(data);
			text = CString((char *)form->m_form->name) + '\t' + data;
			m_guard->SetCombo(text);
		}
		m_client->m_keyx->OnChange(this, idx, true);
	}
	return 0;
}

void CScreen::DoProc(CString procs, CfmBase* form, CString data, int count)
{
	CString	string;
	CScreen* screen = this;

	string = form->GetSymbolName();
	if (m_client->FindScreen(dpCURRENT, string, screen, true))
		m_client->m_vm->Procedure(screen, procs, data, count);
}

bool CScreen::GetHistory(CString name, CString& codes, bool up)
{
	return m_guard->GetCode(name, codes, up);
}

BOOL CScreen::IsResizable()
{
	return ((m_client->m_type & vtypeMSK) == vtypeREX);
}

int CScreen::GetHeight()
{
	CScreen* screen;

	if (m_client->GetAtScreen(screen))
		return screen->m_mapH->height;
	return 0;
}

int CScreen::GetWidth()
{
	CScreen* screen;

	if (m_client->GetAtScreen(screen))
		return screen->m_mapH->width;
	return 0;
}

bool CScreen::OnControl(HWND hWnd, int& key, bool force)
{
	CfmBase* form;

	for (key = 0; key < m_mapH->formN; key++)
	{
		form = GetAtForm(key);
		if (form && form->IsFocus(hWnd))
		{
			if (force && form->m_form->kind == FM_COMBO)
			{
				m_client->m_view->SetFocus();
				return false;
			}
			return true;
		}
	}
	return false;
}

int CScreen::ParseRCC(CfmBase* form, char* datB, int index)
{
	int	skipL, indx;
	struct	_RCC*	rcc;

	skipL = L_RCC;
	rcc = (struct _RCC *) datB;

	for (indx = 0; indx < sizeof(rcc->name); indx++)
	{
		switch (rcc->name[indx])
		{
		case '\0':
		case '\t':
		case ' ':
			break;
		default:
			continue;
		}
		break;
	}

	int	col;
	CString	text;

	col = 0;
	form->GetEnum(CString(rcc->name, col), col);
	if (col < 0)
	{
		for (indx = 0; indx < rcc->ccl; indx++)
		{
			skipL += strlen(datB);
			datB = &datB[strlen(datB)];
			datB++; skipL++;
		}
		return skipL;
	}

	switch (rcc->ccs)
	{
	case csCOMBO:
		if (rcc->ccl <= 0)
		{
			((CfmGrid *)form)->SetCellItems(index, col, _T(""));
			break;
		}

		datB = &datB[L_RCC];

		text = datB;
		text += '\t';
		for (indx = 0; indx < rcc->ccl; indx++)
		{
			text += datB;
			text += '\t';

			skipL += strlen(datB);
			datB = &datB[strlen(datB)];
			datB++; skipL++;
		}
		((CfmGrid *)form)->SetCellItems(index, col, text);
		break;
	default:
		break;
	}

	return skipL;
}

bool CScreen::isPurge()
{
// updateXXX_202202	
	if (m_vbe->m_invoke)
		m_vbe->Close();
	return m_vbe->m_invoke;
}

bool CScreen::GetTranInfo(CString trx)
{
	CString	path;

	ClearTranInfo();
	path.Format(_T("%s/%s/%s.trx"), LPCTSTR(m_root), TRXDIR, LPCTSTR(trx));
	if (::GetFileAttributes((char *)path.operator LPCTSTR()) == 0xFFFFFFFF)
		return false;

	GetTranInfo(path, m_ins, m_icells, true);
	GetTranInfo(path, m_outs, m_ocells, false);

	if (m_ins.GetSize() <= 0 || m_outs.GetSize() <= 0)
		return false;
	return true;
}

void CScreen::GetTranInfo(CString path, CPtrArray& forms, CMapStringToOb& cells, bool input)
{
	CString	str, syms;
	char	wb[256] = { 0, }, twb[32] = { 0, };
	CfmBase* base;
	CtranItem *item, *citem;
	CPtrArray* arr;
	CString	section = input ? _T("IN") : _T("OUT");

	for (int ii = 1; ; ii++)
	{
		str.Format("%d", ii);
		GetPrivateProfileString(section, str, "", wb, sizeof(wb), path);
		if (strlen(wb) <= 0)
			break;

		str  = CString(wb);
		item = new CtranItem;
		sscanf_s((char *)str.operator LPCTSTR(), "%s %d %d %s", twb, _countof(twb), &item->m_size, &item->m_type, wb, _countof(wb));
		syms = CString(twb, strlen(twb));
		if (syms.Find("##") == 0)
			syms = syms.Mid(2);
		if (FindForm(syms, base))
			item->m_form = base;
		else if (!input)
		{	// find output-symbol in table
			int	value;

			for (int jj = 0; jj < m_mapH->formN; jj++)
			{
				if (m_formR[jj].kind == FM_TABLE)
				{
					base = GetAtForm(jj);
					base->GetEnum(syms, value, 109);
					if (value >= 0)
					{
						item->m_form = base;
						item->m_col  = value;
						break;
					}
				}
			}
		}
		forms.Add(item);
		if (base && (base->m_form->kind == FM_GRID))
		{
			arr = new CPtrArray;
			for (int jj = 1; ; jj++)
			{
				str.Format("%d", ii*1000+jj);
				GetPrivateProfileString(section, str, "", wb, sizeof(wb), path);
				if (strlen(wb) <= 0)
					break;

				str   = CString(wb);
				citem = new CtranItem;
				sscanf_s((char *)str.operator LPCTSTR(), "%s %d %d %s", twb, _countof(twb), &citem->m_size, &citem->m_type, wb, _countof(wb));
				str = twb;
				citem->m_col = 0;
				base->GetEnum(str, citem->m_col);
				arr->Add(citem);
			}
			cells.SetAt(syms, arr);
		}
	}
}

void CScreen::ClearTranInfo()
{
	int	ii;
	CString	name;
	CPtrArray* arr;

	for (ii = 0; ii < m_ins.GetSize(); ii++)
		delete m_ins.GetAt(ii);
	m_ins.RemoveAll();

	for (ii = 0; ii < m_outs.GetSize(); ii++)
		delete m_outs.GetAt(ii);
	m_outs.RemoveAll();

	for (POSITION pos = m_icells.GetStartPosition(); pos; )
	{
		m_icells.GetNextAssoc(pos, name, (CObject *&)arr);
		for (ii = 0; ii < arr->GetSize(); ii++)
			delete arr->GetAt(ii);
		arr->RemoveAll();
		delete arr;
	}
	m_icells.RemoveAll();

	for (POSITION pos = m_ocells.GetStartPosition(); pos; )
	{
		m_ocells.GetNextAssoc(pos, name, (CObject *&)arr);
		for (int ii = 0; ii < arr->GetSize(); ii++)
			delete arr->GetAt(ii);
		arr->RemoveAll();
		delete arr;
	}
	m_ocells.RemoveAll();
}

int CScreen::CreateObject(CString name, CCmdTarget*& target)
{
	CpubControl* object;

	name.MakeUpper();
	if (m_objects.Lookup(name, (void *&)object))
	{
		target = object->m_target;
		return 0;
	}

	CString	path;
	HINSTANCE hinst;

	path.Format(_T("%s/%s/%s.dll"), m_root.GetString(), DEVDIR, name.GetString());
	LOG_OUTP(3, "[LOADLIB", path, __FUNCTION__);
	hinst = AfxLoadLibrary(path);
	if (hinst)
	{
		CCmdTarget* (APIENTRY* axCreate)(void);
		CCmdTarget* (APIENTRY* axCreateEx)(void*);

		axCreate = (CCmdTarget* (APIENTRY*)(void))GetProcAddress(hinst, _T("axCreate"));
		if (axCreate != NULL)
		{
			target = (*axCreate)();
			if (target)
			{
				object = new CpubControl;
				object->m_dll    = hinst;
				object->m_target = target;
				m_objects.SetAt(name, object);
				return 1;
			}
		}
		else
		{
			axCreateEx = (CCmdTarget* (APIENTRY*)(void*))GetProcAddress(hinst, _T("axCreateEx"));
			if (axCreateEx != NULL)
			{
				target = (*axCreateEx)((void*)m_root.operator LPCTSTR());
				if (target)
				{
					object = new CpubControl;
					object->m_dll    = hinst;
					object->m_target = target;
					m_objects.SetAt(name, object);
					return 1;
				}
			}
		}
		AfxFreeLibrary(hinst);
	}

	target = NULL;
	return -1;
}

//////////////////////////////////////////////////////////////////////////////
//	transaction information
//

CtranItem::CtranItem()
{
	m_form = NULL;	// for form
	m_size = 0;
	m_type = 1;	// IO_NUM
	m_col  = -1;	// for cell
}

CtranItem::~CtranItem()
{
}
