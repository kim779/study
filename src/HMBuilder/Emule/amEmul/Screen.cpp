// Screen.cpp: implementation of the CScreen class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Screen.h"
#include "Client.h"
#include "Script.h"
#include "Stream.h"
#include "key.h"
#include "xscreen.h"
#include "xsystem.h"

#include "../h/axisvar.h"
#include "../h/axiserr.h"
#include "../h/axstring.h"
#include "../dll/form/amctrl.h"
#include "../dll/form/amgrid.h"
#include "../dll/form/amtable.h"
#include "../dll/lua/scriptEngine.h"
#include "../dll/lua/ledgerx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScreen::CScreen(CClient* client, CRect rect)
	: CAmForm()
{
	m_client = client;
	m_rect   = rect;
	m_guard  = client->m_guard;
	m_vbe    = new CScriptEngine(m_guard->GetParent());
	m_vbe->m_clientKey	= m_client->m_key;
	m_vbe->m_screenKey	= 0;

	m_mapH    = NULL;
	m_view    = m_client->m_view;
//	m_palette = m_guard->m_palette;
//	m_draw    = m_guard->m_draw;
	m_root    = m_guard->m_root;

	m_pAmDraw  = m_guard->m_pdraw;
	m_pPalette = m_guard->m_palette;

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
}

CScreen::~CScreen()
{	
	m_flashObs.RemoveAll();
	m_revObs.RemoveAll();
	m_blink.RemoveAll();
	
	if (m_client->m_view->GetSafeHwnd())
	{
		m_client->m_view->HideCaret();
		if (m_state & reptxSN)
			m_client->m_view->KillTimer(TM_REPTR+m_key);
		m_client->m_view->KillTimer(TM_VB+m_key);
	}

	
	m_client->m_vm->OnClose(this);
	
	/*
	if (m_mapH && m_client)
	{
		for (int ii = 0; ii < m_mapH->formN; ii++)
		{
//			switch (m_formR[ii].kind)
			switch (m_vFORM[ii]->kind)
			{
			case FM_OBJECT:
				break;
			default:
				continue;
			}
			m_client->RemoveAtScreen(m_vFORM[ii]->keys);
		}
	}
	*/
	
	if (m_tabR != NULL)
		delete[] m_tabR;
	if (m_ioR != NULL)
		delete[] m_ioR;
	if (m_profit != NULL)
		delete m_profit;

	CString	name;
	CObject *object;

	for (POSITION pos = m_cells.GetStartPosition(); pos; )
	{
		m_cells.GetNextAssoc(pos, name, object);
		((CPtrArray *)object)->RemoveAll();
		delete object;
	}
	m_cells.RemoveAll();
	m_notices.RemoveAll();

	if (m_ledger)
		delete m_ledger;
	if (m_xscreen)
		delete m_xscreen;
	delete m_vbe;
	
}

bool CScreen::Attach(CString mapN)
{
//	if (mapN.GetLength() != L_MAPN)
//		return false;

	if (!m_guard->isVersion(mapN))
		return false;
	CString tmps;
	tmps = mapN;
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

	if (m_mapH->options & OP_LEDGER)
	{
		m_ledger  = new CLedger;
		m_ledgerL = m_ledger->m_size;
	}
	return true;
}

int compare(const void *arg1, const void *arg2)
{
	return *(int *)arg1 - *(int *)arg2;
}

bool CScreen::Parse(bool resize)
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
	//mainRc = CRect(0, 0, m_mapH->width, m_mapH->height);
	LoadForm(mainRc);

	CScreen* screen;
	CamBase* form;
	float	hr, vr;
	float	ratio = (float)(m_mapH->resolution / 100.0);
	CString	text;

	bool	flash = false;

	m_vbe->Initialize();
	screen    = this;
	m_xscreen = new CxScreen(this);

	m_vbe->m_screenKey = m_key;		// Screen Key Setting
	if (m_guard->m_login)
		m_vbe->AddObject(_T("LOGIN"),  (CCmdTarget *)m_guard->m_login);

	if (m_ledger)
		m_vbe->AddObject(_T("LEDGER"), (CCmdTarget *)m_ledger);
	
	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		form = GetAtForm(ii);
		if (form->m_form->kind == FM_COMBO)
		{
/*			if (form->m_form->properties & PR_ACCN)
			{
				if (m_guard->m_term & flagACN)
					form->SetEnum("", 0);
				else if (m_guard->GetCombo(form->GetSymbolName(), text))
					form->WriteAll(text, false);
			}
			*/
			form->IsChanged();
		}

		Order(form, ii);
		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			
			if (isMain())
			{
				hr = ((float)mainRc.Width() / (float)m_mapH->width) * ratio;
				vr = ((float)mainRc.Height() / (float)m_mapH->height) * ratio;
				unitRc = CRect(form->m_form->left * hr, form->m_form->top * vr, form->m_form->right * hr, form->m_form->bottom * vr);

			}
			else
			{
				hr = ((float)mainRc.Width() / (float)m_mapH->width);// * ratio;
				vr = ((float)mainRc.Height() / (float)m_mapH->height);// * ratio;
				//hr = (float)(m_mapH->resolution / 100.0);
				//vr = (float)(m_mapH->resolution / 100.0);

				unitRc.left   = int(mainRc.left+form->m_form->left*hr);
				unitRc.top    = int(mainRc.top+form->m_form->top*vr);
				unitRc.right  = int(mainRc.left+form->m_form->right*hr);
				unitRc.bottom = int(mainRc.top+form->m_form->bottom*vr);
			}
			
		
			if (!GetFileExist(form->m_form->str))
				break;

			if (m_client->SetAtScreen(screen, unitRc, form->m_form->str, -1, m_key))
			{
				form->m_form->keys = screen->m_key;
				screen->m_object   = ii;
				screen->m_parent   = m_key;
				screen->Parse(resize);
			}
			
			break;

		case FM_BUTTON:
		case FM_RADIO:
		case FM_CHECK:
		//case FM_MEMO:
		case FM_EDIT:
		case FM_COMBO:
			break;

		case FM_CONTROL:
			((CamCtrl *)form)->SetFCB((m_guard->m_status & WS_FCB) ? true : false, m_guard->m_flash);
			if (form->m_form->attr & FA_COMMA)
				m_notices.Add(form);
			break;

		case FM_GRID:
		case FM_GRIDEX:
/*			if (form->m_form->attr & FA_ENUM)
			{
				char	twb[256];

				text.Format("%s$%s", CString(m_mapH->mapN, L_MAPN), form->GetSymbolName());
				GetPrivateProfileString("GRID", text, "", twb, sizeof(twb), m_guard->m_modals);
				text = twb;
				if (!text.IsEmpty())
					SetCells(form, text, false);
			}
			else */
			if (m_profit == NULL)
				m_guard->SetProfit(this, form, m_profit);

//			if (form->m_form->vals[2] != NOVALUE)
			if (!CString(form->m_form->str).IsEmpty())
			{
				//form->m_form->vals[2] = (DWORD)&m_strR[form->m_form->vals[2]];
				text = (char *)form->m_form->str;	//form->m_form->vals[2];
				if (text.Find("$$") == 0)
				{
					//????????????????????????
					//form->m_form->vals[2] += 2;
					m_notice = form;
				}
				else if (text.Find("$?") == 0)
					m_sales = form;
				else if (text.Find("$*") == 0)
					m_push = form;
			}
			break;
		}
		/*
		if (form->m_form->attr & FA_FLASH)
		{
			if (form->m_form->iorder != (WORD)-1)
				m_flashObs.SetAt(form->m_form->iorder, form);
			else if (form->m_form->kind == FM_CONTROL)
				m_flashObs.SetAt(m_mapH->formN+ii, form);
			flash = true;
		}
		*/

		if (form->m_form->kind != FM_CONTROL)
		{
			m_vbe->AddObject(form->GetSymbolName(),
				(form->m_form->kind == FM_CONTROL) ? (CCmdTarget *)((CamCtrl*)form)->m_ctrl : form);
		}
	}

	if (m_push != NULL)
		m_guard->SetPush(true);

	switch (m_mapH->bgK)
	{
	case IK_BRUSH:
		if (isMain())
		{
//			if (m_mapH->imageV & 0x02000000)
//				m_client->SetBK(m_mapH->imageV);
//			else
				m_client->SetBK(m_guard->GetRGB(m_mapH->bgClr));
		}
		else if (m_client->GetAtScreen(screen, m_parent))
		{
			form = screen->GetAtForm(m_object);
			form->SetBkColor(m_mapH->bgClr);
		}
		break;
	case IK_BMP:
		if (isMain())
			m_client->SetBK(m_mapH->bgFileN);
		else if (m_client->GetAtScreen(screen, m_parent))
		{
			form = screen->GetAtForm(m_object);
			form->WriteData(m_mapH->bgFileN);
		}
		break;
	default:
		if (!isMain() && m_client->GetAtScreen(screen, m_parent))
		{
			form = screen->GetAtForm(m_object);
			form->ClearData();
		}
		break;
	}

	if (resize)
	{
		if (!m_guard->m_resize && !(m_client->m_status & S_FIX))
			SetFont(m_guard->m_resize, m_client->m_font);
		m_client->GetAtScreen(screen);
		hr = (float)m_client->m_rect.Width() / (float)screen->m_mapH->width;
		vr = (float)m_client->m_rect.Height() / (float)screen->m_mapH->height;
		ResizeForm(hr, vr);
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
/*
	if (m_mapH->options & OP_DROP)
		m_client->Register(NULL);
*/
	return true;
}

void CScreen::Order(CamBase* form, int order)
{
	if (form->m_form->index != (WORD)-1 && form->m_form->index < m_mapH->formN)
	{
		m_tabR[form->m_form->index] = order;
		m_tabL++;
	}

	if (form->m_form->iorder != (WORD)-1 && form->m_form->iorder < m_mapH->formN)
	{
		Msg("form iorder  [%d]    order [%d]", form->m_form->iorder, order);
		m_ioR[form->m_form->iorder] = order;
		m_ioL++;
	}
}

void CScreen::ReAttach(CRect rect)
{
	m_rect = rect;
	ReloadForm(m_rect);

	CScreen* screen;
	CamBase* form;
	float	hr, vr;
	float	ratio = (float)(m_mapH->resolution / 100.0);

	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		form = GetAtForm(ii);
		if (form->m_form->kind == FM_OBJECT && m_client->GetAtScreen(screen, form->m_form->keys))
		{
			hr = ((float)m_rect.Width() / (float)m_mapH->width) * ratio;
			vr = ((float)m_rect.Height() / (float)m_mapH->height) * ratio;
			//hr = (float)m_mapH->resolution / 100.0;
			//vr = (float)m_mapH->resolution / 100.0;

			rect.left   = int(m_rect.left+form->m_form->left*hr);
			rect.top    = int(m_rect.top+form->m_form->top*vr);
			rect.right  = int(m_rect.left+form->m_form->right*hr);
			rect.bottom = int(m_rect.top+form->m_form->bottom*vr);

			screen->ReAttach(rect);
		}
	}
	m_client->GetAtScreen(screen);
	hr = (float)m_client->m_rect.Width() / (float)screen->m_mapH->width;
	vr = (float)m_client->m_rect.Height() / (float)screen->m_mapH->height;
	ResizeForm(hr, vr);
}

void CScreen::OnVirtual(char* mapN, int& tab)
{
	tab = NOVALUE;
/*
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
			pageL = (int)strlen(pageN);
			pageN += pageL;
			offs  += pageL;
			pageN++;
			offs++;
		}
		tab = ii;
		break;
	}
*/
}

void CScreen::SetForm(int idx, bool focus)
{
	if (idx < 0 || idx >= m_mapH->formN)
		return;

	CamBase* form = GetAtForm(idx);
	form->SetFocus(focus);

	if (focus)
		m_client->ChangeIME(this, idx);
}

bool CScreen::SetObject(int key, char *mapN)
{
	if (key < 0 || key >= m_mapH->formN)
		return false;

	return m_client->SetObject(this, key, mapN);
}

bool CScreen::isPush(CString& pushN)
{
	if (!isPush())
		return false;

	pushN.Empty();
	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		if (m_vFORM[ii]->kind != FM_EDIT)
			continue;
//		if (m_vFORM[ii]->vals[0] == NOVALUE)
		if (CString(m_vFORM[ii]->str).IsEmpty())
			continue;
		pushN = m_vFORM[ii]->str;	//(char *)&m_strR[m_vFORM[ii]->vals[0]];
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

	CamBase* form = m_push;

//	struct	_cellR *cell;
	vector <struct _cellR*>* pCell;
	
//	cell = (struct _precellR *) &m_cellR[form->m_form->vals[0]];

	pCell = m_MGRID[form->m_form->name];

	int nCols = form->m_form->col;	//HIWORD(form->m_form->vals[1]);
	int rowN = form->GetSize();

	CString	cmps;
	int	tRGB, pRGB;
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
				form->SetFgColor((*pCell)[idx]->tRGB/*cell[idx].tRGB*/);
				form->SetBkColor((*pCell)[idx]->tRGB/*cell[idx].pRGB*/);
			}
		}
	}
}

bool CScreen::OnAlert(CString code, CdataSet* fms, CObArray* obs, int stat, struct _alertR* alertR)
{
	if (m_flashObs.IsEmpty())
		return false;

	WORD	key;
	CamBase* form;
	CString	text;
	bool	flash;

	m_color = flash = false;
	m_tick  = GetTickCount();
	if (fms == NULL)
		fms = (CdataSet *)obs->GetAt(0);
	for (POSITION pos = m_flashObs.GetStartPosition(); pos; )
	{
		m_flashObs.GetNextAssoc(pos, key, (CObject *&)form);
		switch (form->m_form->kind)
		{
		case FM_GRID:
		case FM_GRIDEX:
			if (FlashGrid(key, code, fms))
				flash = true;
			break;
		case FM_TABLE:
			if (FlashSemi(key, fms, code))
				flash = true;
			break;
		case FM_CONTROL:
			if (alertR)
				form->SetEnum(_T(""), (int)alertR, 253);
			break;
		default:
			form->ReadData(text);
			text.TrimRight();

			if (!text.Compare(code))
			{
				flash = true;
				UpdateRTM(key+1, code, fms, obs, stat, alertR);
			}
			break;
		}
	}
	if (m_color)
		DominoForm();
	return flash;
}

void CScreen::UpdateRTM(int key, CString code, CdataSet* fms, CObArray* obs, int stat, struct _alertR* alertR)
{
	CamBase* form;
	CString	text, string;
	struct	_flashK	flashK;

	if (m_sales && (stat & alert_SCR))
		ScrollRTM(obs);

	if (isUob() && !isFlash())
		m_color = false;

	for ( ; key < m_ioL; key++)
	{
		form = GetAtForm(m_ioR[key]);
		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			CScreen* uob;
			if (m_client->GetAtScreen(uob, form->m_form->keys) && uob->isUob() && !uob->isFlash())
			{
				uob->m_tick = m_tick;
				uob->UpdateRTM(0, code, fms, obs, stat, alertR);
			}
			continue;
		case FM_EDIT:
		case FM_OUT:
			break;
		case FM_GRID:
		case FM_GRIDEX:
			if (isUob() && !isFlash())
				FlashGrid(key, code, fms);
			continue;
		case FM_TABLE:
//			if (!(form->m_form->attr & FA_FLASH))
//				FlashSemi(key, fms);
			continue;
		case FM_CONTROL:
/*			if (!(form->m_form->attr & FA_FLASH))
			{
				if (alertR)
					form->SetEnum(_T(""), (int)alertR, 253);
			}
			*/
			continue;
		default:
			continue;
		}

//		if (form->m_form->attr & FA_FLASH)
//			return;

		if (!fms->Lookup(form->GetName(), string))
			continue;

		form->ReadData(text);
		/*
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
		else*/ 
		if(!isFlash(form->m_form->attr2 & SA_CORGB, string, text))
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
	vector <struct _cellR*>* pCell;
	struct	_flashK	flashK;
	CDWordArray	flashs;
	CPtrArray* grid;

	CamBase* form;
	DWORD	flash;
	CString	text, string;

	baseK = GetFlashBaseKey(m_ioR[key]);
	form  = GetAtForm(m_ioR[key]);

	//cell  = //(struct _precellR *) &m_cellR[form->m_form->vals[0]];
	nCols = form->m_form->col;		//HIWORD(form->m_form->vals[1]);
	nRows = ((CamGrid *)form)->GetValidRows();
	if (nRows <= 0)	return false;

	m_sync.Lock();
	if (/*!(form->m_form->attr & FA_ENUM) ||*/ !m_cells.Lookup(form->GetSymbolName(), (CObject *&)grid))
		grid = NULL;

	gridK = -1;
	flash = form->m_form->checked;
//	if (flash && cell->attr & FA_FLASH)
//		flashs.Add(MAKELONG(0, form->m_form->checked));
	flashs.Add(MAKELONG(flash, nCols));

	for (int ii = 0; ii < nRows; ii++)
	{
		for (int kk = 0; kk < flashs.GetSize(); kk++)
		{
			flash = flashs.GetAt(kk);
			form->ReadData(text, false, LOWORD(flash), ii);
			text.TrimRight();

			if (text.Compare(code))
				continue;

	//		cell = (struct _cellR *) &m_cellR[form->m_form->vals[0] + LOWORD(flash)];
	//		cell = m_vCELL[LOWORD(flash)];
			pCell = m_MGRID[form->m_form->name];
			cell = (*pCell)[LOWORD(flash)];

			for (int idx = LOWORD(flash); idx < HIWORD(flash); idx++/*, cell++*/)
			{
				if (grid != NULL)
				{
					if (idx < grid->GetSize())
						cell = (struct _cellR *)grid->GetAt(idx);
					else	break;
				}

				if (!fms->Lookup(form->GetName(idx), string))
					continue;

				form->ReadData(text, false, idx, ii);
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

bool CScreen::FlashSemi(int key, CdataSet* fms, CString code)
{
	int	baseK, semiK;
	CString	text, string;

	struct	_cellR *cell = NULL;
	vector <struct _cellR*>* pCell;
	struct	_flashK	flashK;

	CamBase* form = GetAtForm(m_ioR[key]);
	if (form->m_form->iok == EIO_NOP)
		return false;

	if (!code.IsEmpty())
	{
		CString	text;
		form->ReadData(text, false, 0, form->m_form->checked);
		text.TrimRight();

		if (text.Compare(code))
			return false;
	}

	baseK = GetFlashBaseKey(m_ioR[key]);
	//cell = m_vCELL[0];	//(struct _cellR *) &m_cellR[form->m_form->vals[0]];
	pCell = m_MGRID[form->m_form->name];
	

	semiK = -1;
	int nRows = form->m_form->row;	//LOWORD(form->m_form->vals[1]);
	for (int ii = 0; ii < nRows; ii++)
	{
		if (!fms->Lookup(form->GetName(ii), string))
			continue;

		form->ReadData(text, false, 0, ii);
		if (!isFlash((*pCell)[ii]->attr2 & SA_CORGB, string, text))
			continue;

		if ((*pCell)[ii]->isEdit == IE_CORGB)
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
	vector <struct _cellR*>* pCell;
	bool	skip, colorF = false, insertF = false;

	CamBase* form = m_sales;
	int nCols = form->m_form->col;	//HIWORD(form->m_form->vals[1]);

	CPtrArray* grid;
	if (/*form->m_form->attr & FA_ENUM &&*/ m_cells.Lookup(form->GetSymbolName(), (CObject *&)grid))
		nCols = (int)grid->GetSize();
	else
		grid = NULL;

	string = _T("");
	for (int ii = 0; ii < obs->GetSize(); ii++)
	{
		fms = (CdataSet *)obs->GetAt(ii);
		//cell = m_vCELL[0];	//(struct _cellR *) &m_cellR[form->m_form->vals[0]];
		pCell = m_MGRID[form->m_form->name];
		
		skip = true; rows = _T("");
		for (int idx = 0; idx < nCols; idx++/*, cell++*/)
		{
			cell = (*pCell)[idx];

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
			((CamGrid *)form)->InsertRows(1, string);
		else
			((CamGrid *)form)->InsertRows(-1, string);
	}

	if (colorF)
		DominoForm();
}

bool CScreen::OnNotice(CdataSet& major, CdataSet& minor, CdataSet& fms, CString notices)
{
	CamBase* form;
	CString	text, string;
	struct	_flashK	flashK;

	int	ii, name;
	bool	match;

	for (ii = 0; ii < m_notices.GetSize(); ii++)
	{
		form = (CamBase *)m_notices.GetAt(ii);
		form->SetEnum(notices, 0, 0);
	}

	if (major.IsOn())
	{
		DWORD	tick;

		match = false;
		tick = GetTickCount();

		for (ii = 0; ii < m_mapH->formN; ii++)
		{
			form = GetAtForm(ii);
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
/*			if (form->m_form->properties & PR_SUM)
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
			else*/ 
			if (!isFlash(form->m_form->attr2 & SA_CORGB, string, text))
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
	vector <struct _cellR*>* pCell;
	
	CPtrArray* grid;

	form = m_notice;
	if (form->m_form->kind == FM_GRID || form->m_form->kind == FM_GRIDEX)
		nRows = ((CamGrid *)form)->GetValidRows();
	else
		nRows = ((CamTable *)form)->GetValidRows();
	match = false;

	nCols = form->m_form->col;	//HIWORD(form->m_form->vals[1]);

	m_sync.Lock();
	if (/*!(form->m_form->attr & FA_ENUM) ||*/ !m_cells.Lookup(form->GetSymbolName(), (CObject *&)grid))
		grid = NULL;

	pCell = m_MGRID[form->m_form->name];
	for (kk = 0; kk < nRows; kk++)
	{
		//cell  = m_vCELL[0];	//(struct _precellR *) &m_cellR[form->m_form->vals[0]];
		for (idx = 0; idx < nCols; idx++/*, cell++*/)
		{
			if (grid != NULL)
			{
				if (idx == grid->GetSize())
				{
					idx = nCols;
					break;
				}
				cell = (struct _cellR *)grid->GetAt(idx);
			}
			if (minor.Lookup(form->GetName(idx), string))
			{
				form->ReadData(text, false, idx, kk);
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
		//cell  = m_vCELL[0];	//(struct _precellR *) &m_cellR[form->m_form->vals[0]];
		pCell = m_MGRID[form->m_form->name];
		for (idx = 0; idx < nCols; idx++/*, cell++*/)
		{
			if (grid != NULL)
			{
				if (idx == grid->GetSize())
				{
					m_sync.Unlock();
					return false;
				}
				cell = (struct _cellR *)grid->GetAt(idx);
			}
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

	name = atoi(form->m_form->str);		//atoi((char *)form->m_form->vals[2]);
	if (match)
	{
		if (fms.Lookup(name, text) && !atoi(text))
		{
			flash = true;
			match = false;
			((CamGrid *)form)->RemoveRow(kk);
		}
	}
	else if (!fms.Lookup(name, text) || atoi(text))
	{
		if (form->m_form->attr2 & GO_TOP)
		{
			((CamGrid *)form)->InsertRow(0);
			kk = 0;
		}
		else
		{
			((CamGrid *)form)->InsertRow(-1);
			kk = ((CamGrid *)form)->GetValidRows();
			if (--kk < 0)	kk = 0;
		}
		match = true;
	}

	if (match)
	{
		bool color = false;

	//	cell = m_vCELL[0];	//(struct _precellR *) &m_cellR[form->m_form->vals[0]];
		for (idx = 0; idx < nCols; idx++, cell++)
		{
			if (grid != NULL)
			{
				if (idx < grid->GetSize())
					cell = (struct _cellR *)grid->GetAt(idx);
				else	break;
			}

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
	DWORD	tickV = GetTickCount();

	for (POSITION pos = m_revObs.GetStartPosition(); pos; )
	{
		m_revObs.GetNextAssoc(pos, key, (struct _flashK &)flashK);
		if (!flashK.time || (tickV - flashK.time) < 1000)
			continue;

		flashK.time = 0;
		m_revObs.SetAt(key, flashK);

		CamBase* form = GetAtForm(flashK.index);
		if (flashK.key != 0)
		{
			key -= flashK.key;
			row = key / form->m_form->col;	//HIWORD(form->m_form->vals[1]);
			col = key % form->m_form->col;	//HIWORD(form->m_form->vals[1]);

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
	DWORD	tickV = GetTickCount();

	for (POSITION pos = m_blink.GetStartPosition(); pos; )
	{
		m_blink.GetNextAssoc(pos, key, (struct _blink &)blink);
		if (tickV - blink.time < 500)
			continue;

		color = blink.form->GetBkColor();
		blink.form->SetBkColor(blink.color);

		blink.time  = tickV;
		blink.color = color;
		m_blink.SetAt(key, blink);
	}

	m_sync.Unlock();
}

void CScreen::Blink(CamBase* form, BOOL enable, int color)
{
	int	key;
	struct	_blink	blink;

	if (!FindForm(form->GetSymbolName(), key))
		return;

	m_sync.Lock();

	if (!m_blink.Lookup(key, blink))
	{
		if (enable)
		{
			if (color >= 0)
				form->SetBkColor(color);
			blink.form  = form;
			blink.time  = GetTickCount();
			blink.color = form->m_form->pRGB;
			m_blink.SetAt(key, blink);
			m_client->SetBlink();
		}
	}
	else if (!enable)
	{
		m_blink.RemoveKey(key);
		form->SetBkColor(form->m_form->pRGB);
	}

	m_sync.Unlock();
}

void CScreen::ResetRTM(CString name)
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
			CamBase* form = GetAtForm(flashK.index);
			if (name.CompareNoCase(form->GetSymbolName()))
				continue;

			key -= flashK.key;
			row = key / form->m_form->col;	//HIWORD(form->m_form->vals[1]);
			col = key % form->m_form->col;	//HIWORD(form->m_form->vals[1]);

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
		switch (m_vFORM[ii]->kind)
		{
		case FM_TABLE:
			if (m_vFORM[ii]->iok == EIO_NOP)
				continue;
		case FM_GRID:
		case FM_GRIDEX:
			break;
		default:
			continue;
		}

//		key += LOWORD(m_vFORM[ii]->vals[1]) * HIWORD(m_vFORM[ii]->vals[1]);
		key += m_vFORM[ii]->col * m_vFORM[ii]->row;	
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
	bool	sign, contrast;

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
	sign = false;
	contrast = IsContrast(conVal);

	switch (conVal)
	{
	case '+':
		index++;
		break;
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

	if (contrast)	index++;

	for (; index < text.GetLength(); index++)
	{
		if (!isspace(text[index]) && text[index] != '0')
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
			if (!isdigit(text[ii]))
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
	CamBase* form;

	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		form = GetAtForm(ii);
		if (form->m_form->kind != FM_BUTTON ||
			form->m_form->kind != FM_CHECK  ||
			form->m_form->kind != FM_RADIO)
			continue;
		switch (form->m_form->type)
		{
		case BTN_USER:
			if (form->m_form->keys != wParam)
				break;

			if (form->IsEnable() && m_client->m_vm->IsAvailable(this, evClick, form))
			{
				if (form->m_form->properties & PR_CHECKED)
					m_client->SetCPos(this, ii, false);
				m_client->m_vm->OnClick(this, form);
				return true;
			}
		default:
			break;
		}
	}
	return false;
}

bool CScreen::OnTRAN(bool byKey)
{
	int	guide;
	CamBase* form;

	m_skip = true;
	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		form = GetAtForm(ii);
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
		case FM_GRIDEX:
			if (form->m_form->iok == EIO_OUTPUT)
				continue;
		case FM_EDIT:
		case FM_COMBO:
		//case FM_MEMO:
			break;
		default:
			continue;
		}

		if (!form->IsValid(guide))
		{
			m_client->SetCPos(this, ii);
			m_guard->SetGuide(guide, m_client->m_key);
			return false;
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
	CamBase* form = GetAtForm(idx);
	if (!m_client->m_vm->IsAvailable(this, evChange, form))
		return true;

	switch (form->m_form->kind)
	{
	//case FM_MEMO:
	case FM_EDIT:
	case FM_COMBO:
//	case FM_TREE:
//	case FM_SHEET:
	case FM_GRID:
	case FM_GRIDEX:
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

void CScreen::OnClick(CamBase* form, int result)
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
		m_client->m_stream->InStream("", screen, true);
}

void CScreen::SetCells(CamBase* form, CString text, bool update)
{
	struct	_cellR *cell = NULL;
	vector <struct _cellR*>* pCell;
	//cell = m_vCELL[0];	//(struct _precellR *) &m_cellR[form->m_form->vals[0]];
	pCell = m_MGRID[form->m_form->name];
//cell = (*pCell)[0];

	int nCols = form->m_form->col;	//HIWORD(form->m_form->vals[1]);

	int	idx;
	CString	tmps, name;

	CPtrArray*	grid = new CPtrArray();
	for (idx = 0; idx < form->m_form->type; idx++)
		grid->Add(&(*pCell)[idx]);

	name = text;
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
			if (!strcmp(form->GetSymbolName(idx/*cell[idx].name*/), tmps))
				grid->Add(&(*pCell)[idx]);
		}
	}

	m_sync.Lock();

	CObject	*object;
	if (m_cells.Lookup(form->GetSymbolName(), object))
		delete object;

	if (grid->GetSize() > form->m_form->type)
		m_cells.SetAt(form->GetSymbolName(), grid);
	else			// set default cells
	{
		m_cells.RemoveKey(form->GetSymbolName());
		name.Empty();
		delete grid;
	}
	ResetRTM(form->GetSymbolName());
	form->SetEnum(name, update);
	m_sync.Unlock();
}

void CScreen::GuideLedger()
{
	if (m_ledger == NULL)
		return;
}

void CScreen::SetLedger(char* ledger)
{
	if (m_ledger)
		m_ledger->SetLedger(ledger);
}

void CScreen::GetLedger(char* ledger)
{
	if (m_ledger)
		m_ledger->GetLedger(ledger);
}

void CScreen::OnProfit(CamBase* form)
{
	if (m_profit == NULL || m_profit->m_name.CompareNoCase(form->GetSymbolName()))
		return;

	CamBase* base;
	int	nCols, nRows;

	nCols = form->m_form->col;	//HIWORD(form->m_form->vals[1]);
	nRows = ((CamGrid *)form)->GetValidRows();

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
	int	buy, price, amount;
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
			form->ReadData(text, false, price, ii);
			sum = v_price = atof(text);
			form->ReadData(text, false, amount, ii);
			tmpv = atof(text);
			sum *= tmpv;
			if (sum == 0)	continue;

			v_amount = tmpv;
			if (unit != -1)
			{
				form->ReadData(text, false, unit, ii);
				v_unit = atof(text);
				sum *= v_unit;
				v_price *= v_unit;
			}

			form->ReadData(text, false, buy, ii);
			tmpv = atof(text);
			Buy += tmpv;

			v_tax = 0;
			if (type != -1)
			{
				int	v_type;
				double	mx_tax;

				form->ReadData(text, false, type, ii);
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
						form->ReadData(text, false, x_tax, ii);
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
					form->ReadData(text, false, x_fee, ii);
					mx_fee = atof(text);
				}
				else
					mx_fee = m_profit->m_fee;

				if (x_money != -1)
				{
					form->ReadData(text, false, x_money, ii);
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
					double	mx_tmp, mx_charge;

					form->ReadData(text, false, mi_price, ii);
					mx_tmp = atof(text);
					text.Format(format, v_charge / (v_amount * v_unit));
					mx_charge = atof(text);
					if (dir != -1)
					{
						form->ReadData(text, false, dir, ii);
						if (text.Find(_T("매도")) != -1 || text.Find('S') != -1)
							mx_charge = -mx_charge;
					}
					mx_tmp += mx_charge;
					text.Format(format, mx_tmp);
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
				form->ReadData(text, false, dir, ii);
				if (text.Find(_T("매도")) != -1 || text.Find('S') != -1)
					sum = tmpv + (tmpv - (sum + (m_profit->m_charge ? v_charge * 2 : 0)));
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
		struct	_cellR *cell = NULL;
		vector <struct _cellR*>* pCell;

		//cell  = m_vCELL[0];	//(struct _precellR *) &m_cellR[form->m_form->vals[0]];
		pCell = m_MGRID[form->m_form->name];
		for (int ii = 0; ii < nCols; ii++)
		{
			if (cell[ii].attr & FA_ZEROSUP && ((*pCell)[ii]->attr2 & SA_ZEROSUP) == SA_ASTER)// &&
						/*cell[ii].attr & FA_REF &&*/ /*cell[ii].vals[0] != NOVALUE*///)
			{
				for (int kk = 0; kk < nRows; kk++)
				{
					form->ReadData(text, false, ii, kk);
					tmpv = atof(text);
					if (amount != -1)
					{
						form->ReadData(text, false, amount, kk);
						tmpv *= atof(text);
					}
					if (dir != -1)
					{
						form->ReadData(text, false, dir, kk);
						if (text.Find(_T("매도")) != -1 || text.Find('S') != -1)
							tmpv = -tmpv;
					}
					if (tmpv)
					{
						text.Format(tmpv > 0 ? "+%.04f" : "%.04f", tmpv);
						form->WriteData(text, true, ii/*cell[ii].vals[0]*/, kk);
					}
					else	form->WriteData("", true, ii/*cell[ii].vals[0]*/, kk);
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
				form->ReadData(text, false, idx, ii);
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
	CamBase* form;

	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		form = GetAtForm(ii);
		form->SetEnable(enable);
	}
}

void CScreen::SetPalette(bool force)
{
	CScreen* screen;
	CamBase* form;
	COLORREF rgb;
	int	value;
	struct	_cellR *cell = NULL;
	vector <struct _cellR*>* pCell = NULL;

	if (isMain() && m_mapH->bgK == IK_BRUSH)
	{
		if (/*!(m_mapH->imageV & 0x02000000) && */m_guard->m_palette->Lookup(m_mapH->bgClr, rgb))
			m_client->SetBK(m_guard->GetRGB(m_mapH->bgClr), true);
	}

	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		form = GetAtForm(ii);
		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			if (m_client->GetAtScreen(screen, form->m_form->keys))
			{
				if (screen->m_mapH->bgK == IK_BRUSH /*&& !(screen->m_mapH->imageV & 0x02000000)*/)
				{
					if (m_guard->m_palette->Lookup(screen->m_mapH->bgClr, rgb))
						form->SetBkColor(rgb);
				}
				screen->SetPalette(force);
			}
			continue;

		case FM_GRID:
		case FM_GRIDEX:
			CPtrArray* grid;
			vector <struct _cellR*>* pCell;
			if (/*!(form->m_form->attr & FA_ENUM) ||*/ !m_cells.Lookup(form->GetSymbolName(), (CObject *&)grid))
				grid = NULL;

	//		cell = m_vCELL[0];	//(struct _precellR *)&m_cellR[form->m_form->vals[0]];
			pCell = m_MGRID[form->m_form->name];

			for (value = 0; value < form->m_form->col; value++/*, cell++*/)
			{
				cell = (*pCell)[value];
				if (grid != NULL)
				{
					if (value < grid->GetSize())
						cell = (struct _cellR *)grid->GetAt(value);
					else	break;
				}

				if (!(cell->hpRGB & 0x02000000) && m_guard->m_palette->Lookup(cell->hpRGB, rgb))
					form->SetBkColor(rgb, value, -1);
				if (!(cell->htRGB & 0x02000000) && m_guard->m_palette->Lookup(cell->htRGB, rgb))
					form->SetFgColor(rgb, value, -1);

				if (force)
				{
					for (int jj = 0; jj < ((CamGrid *)form)->GetValidRows(); jj++)
						form->SetFgColor(-1, value, jj);
				}
			}

			if (form->m_form->attr2 & GO_ROW /*&& form->m_form->vals[3] != NOVALUE*/)
			{
				DWORD	fRGB, sRGB;

				//value = form->m_form->vals[3];
				fRGB = sRGB = 0xffffffff;
//				if (!(m_valueR[value] & 0x02000000))
				m_guard->m_palette->Lookup(form->m_form->stRGB, fRGB);
//				if (!(m_valueR[value+1] & 0x02000000))
				m_guard->m_palette->Lookup(form->m_form->spRGB, sRGB);
				
				if (fRGB != 0xffffffff || sRGB != 0xffffffff)
					((CamGrid *)form)->SetRowColor(fRGB, sRGB);
			}

			if ((form->m_form->isEdit == FI_RBAR || form->m_form->isEdit == FI_RREC)
				&& form->m_form->onEdit != NOVALUE && !(form->m_form->onEdit & 0x02000000))
			{
				if (m_guard->m_palette->Lookup(form->m_form->onEdit, rgb))
					form->SetBkColor(rgb, 0, -2);
			}
			form->SetEnum(_T(""), 0, 200);
			continue;

		case FM_TABLE:
//			cell = m_vTCELL[0];	//(struct _precellR *)&m_cellR[form->m_form->vals[0]];
			pCell = m_MTABLE[form->m_form->name];
			for (value = 0; value < form->m_form->row/*LOWORD(form->m_form->vals[1])*/; value++, cell++)
			{
				if (!(cell->hpRGB & 0x02000000) && m_guard->m_palette->Lookup(cell->hpRGB, rgb))
					form->SetBkColor(rgb, -1, value);
				if (!(cell->pRGB & 0x02000000) && m_guard->m_palette->Lookup(cell->pRGB, rgb))
					form->SetBkColor(rgb, 0, value);
				if (force)
					form->SetFgColor(-1, 0, value);
			}
			continue;

		case FM_CONTROL:
			form->SetBkColor(rgb);
			continue;

//		case FM_TAB:
		case FM_TAB:
			if (form->m_form->type == TAB_IMAGE /*&& form->m_form->vals[2] != NOVALUE*/)
			{
				form->SetEnum(_T(""), 0, 200);
				continue;
			}
			if (form->m_form->spRGB != 0)
			{
				if (!(form->m_form->spRGB & 0x02000000))
					if (m_guard->m_palette->Lookup(form->m_form->spRGB, rgb))	//m_valueR[form->m_form->vals[3]], rgb))
						form->SetBkColor(rgb, 0);
			}
			else
			{
				if (form->m_form->type != TAB_NONE && !(form->m_form->pRGB & 0x020000))
					if (m_guard->m_palette->Lookup(form->m_form->pRGB, rgb))
						form->SetBkColor(rgb, 0);
			}
			break;

		case FM_EDIT:
		case FM_COMBO:
			form->SetEnum(_T(""), 0, 200);
		default:
			break;
		}
	
		if (!(form->m_form->pRGB & 0x02000000) && m_guard->m_palette->Lookup(form->m_form->pRGB, rgb))
			form->SetBkColor(rgb);
		if (force)
			form->SetFgColor(-1);
	}
}

void CScreen::SetControl(CString data)
{
	int	pos = data.Find('\t');
	if (pos == -1)
		return;

	CamBase* form;
	if (FindForm(data.Left(pos), form))
	{
		data = data.Mid(pos+1);
		pos  = data.Find('\t');
		if (pos == -1)
			pos = data.GetLength();
		else
			form->SetFgColor(atoi(data.Mid(pos+1)));
		form->WriteData(data.Left(pos));
		m_client->m_status &= ~S_CURSOR;
		pos = FindForm(form);
		if (pos >= 0)
			OnChange(pos, false);
	}
}

void CScreen::SetFCB(bool blink, int flash)
{
	CamCtrl* ctrl;

	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		if (m_vFORM[ii]->kind != FM_CONTROL)
			continue;

		ctrl = (CamCtrl *)GetAtForm(ii);
		ctrl->SetFCB(blink, flash);
	}
}

void CScreen::OnFile(BOOL upload, char* datB, int datL)
{
	int	pos = 0;
	bool	error = false;

	if ((m_mapH->options & OP_LEDGER) && m_ledger && datL >= m_ledgerL)
		pos = m_ledgerL;

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
	CamBase* form;

	if (FindForm(syms, form) && form->m_form->kind == FM_CONTROL)
	{
		switch (ev)
		{
		case evOnClick:
			m_client->m_vm->OnClick(this, form);	break;
		case evOnChange:
			m_client->m_vm->OnChange(this, form);	break;
		case evOnDblClk:
			m_client->m_vm->OnDblClick(this, form);
		default:
			break;
		}
	}
}

long CScreen::DoSomething(int type, CamBase* form, WPARAM wParam, LPARAM lParam)
{
	CScreen* screen;
	CString	string, text;
	int	value, row, column;

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
				m_guard->OnTrigger(m_client, (char *)form->GetSymbolName().operator LPCSTR(), text);
		}
		else
		{
			if (!string.IsEmpty())
				m_client->OnTrigger(string, text, m_key);
			else
				m_client->OnTrigger(form->GetSymbolName(), text, m_key);
		}
		break;

	case doKEY:
		form->OnKey((int)lParam, value);
		OnClick(form, value);
		break;

	case doEXCEL:
		m_client->MakeXlsFile(this, form, LOWORD(wParam) ? true : false, (char *)lParam, HIWORD(wParam) ? true : false);
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
		//switch (form->m_form->kind)
		{
			/*
		case FM_TAB:
			form->ReadAll(text);
			// MANIE if (m_guard->Modal(tabED, form->GetSymbolName(), text))
			//	form->WriteAll(text);
			break;
			*/
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
			screen->LuaSend((int)wParam, (LPCSTR)lParam);
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

	case doGETFORM:
		screen = this;
		string = form->GetSymbolName();
		if (m_client->FindScreen(dpCURRENT, string, screen, true))
		{
			CString strCtrlName = string + (LPTSTR)lParam;
			screen->m_vbe->m_subScreenKey = screen->m_key;
			screen->m_vbe->m_pLua2 = m_vbe->m_pLua;
			screen->m_vbe->m_bParent = true;
			luabind::object* objj = &screen->m_vbe->LuaGetObject((char *)lParam);
			return (long)&screen->m_vbe->m_pObj;
			//return (long)&m_vbe->LuaGetObject((char *)lParam);
			//return (long)&obj;
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
					m_client->RemoveAtScreen(form->m_form->keys);
					form->Refresh();
				}
				else
					screen->SetObject(value, (char *)lParam);
			}
			else
				m_client->SetObject(this, value, (char *)lParam);
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
			&& m_client->isVisible(m_key, value) && form->IsEnable())
			m_client->SetCPos(screen, value, false);
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
		value  = form->m_form->iorder;
		if (value >= 0)
			m_client->m_stream->ParseFCC(this, value, (_FCC*)lParam, column, row);
		break;

	case doCOLUMN:
		m_client->EditColumns(this, form);
		break;

	default:
		break;
	}

	return 0;
}

int CScreen::DoComboBox(CamBase* form)
{
	int	idx;
	
	idx = FindForm(form);
/*
	if (form->m_form->properties & PR_ACCN)
	{
		CString	data, text;

		form->ReadData(data);
		text = form->GetSymbolName() + '\t' + data;
		m_guard->SetCombo(text);
	}
	*/
	m_client->m_keyx->OnChange(this, idx, true);
	return 0;
}

void CScreen::DoProc(CString procs, CamBase* form, CString data, int count)
{
	CString	string;
	CScreen* screen = this;
	
	string = form->GetSymbolName();
	if (m_client->FindScreen(dpCURRENT, string, screen, true))
		m_client->m_vm->Procedure(screen, procs, data, count);
}

bool CScreen::OnControl(HWND hWnd, int& key)
{
	CamBase* form;

	for (key = 0; key < m_mapH->formN; key++)
	{
		form = GetAtForm(key);
		if (form && form->IsFocus(hWnd))
			return true;
	}
	return false;
}

bool CScreen::isPurge()
{
	return false;
		//m_vbe->m_invoke;
}

///////////////////////////////////////////////////////////////
// LUA Method
//  -- FORM
void CScreen::LuaSend(long target, CString strTR)
{
	m_xscreen->LuaSend(target, strTR);
}

lua_State* CScreen::LuaGetObj()
{
	return m_vbe->m_pLua;
}

void CScreen::setFormName(CString sFormName)
{	
	if (sFormName.IsEmpty())
		m_vbe->m_formName = m_mapH->mapN;
	else
		m_vbe->m_formName = sFormName;
}

void CScreen::setFormMain(CScreen* pScreen)
{	
	m_vbe->m_pParent = pScreen->m_vbe;
	// sub에서 상위 form
	pScreen->m_vbe->m_pLua2 = m_vbe->m_pLua;
}

void CScreen::setParentObj(lua_State* L)
{	
	//m_vbe->m_object = obj;
	m_vbe->setParentObj(L);
}

//  -- SYSTEM
void CScreen::SetPopData(CString data)
{
	m_vbe->m_pushData = data;
}
////////////////////////////////////////////////////////////////////


bool CScreen::GetFileExist(CString sName)
{
	CFileFind finder;
	CString sRoot, ss;

	sRoot.Format("%s\\exe\\workshop.ini", m_guard->m_root);
	GetPrivateProfileString("Environment", "Emul_Path", "", ss.GetBuffer(256), 256, sRoot); 
	ss.ReleaseBuffer();
	
	ss.MakeReverse();
	int nPos = ss.Find('\\');

	if (nPos == 0)
		ss = ss.Mid(1, ss.GetLength());		

	ss.MakeReverse();

	CString fpath = FORMAT("%s\\Form\\%s", ss, sName);	
	BOOL bResult = finder.FindFile(fpath.operator LPCSTR());
	finder.Close();
	
	
	return bResult == TRUE ? true: false;
}