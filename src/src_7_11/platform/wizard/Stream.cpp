// Stream.cpp: implementation of the CStream class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Wizard.h"
#include "Stream.h"
#include "Client.h"
#include "Script.h"
#include "xscreen.h"
#include "../h/axisrsm.h"
#include "../h/axiserr.h"
#include "../dll/form/fmgrid.h"
#include "../dll/form/fmradio.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStream::CStream(CClient* client)
{
	m_client = client;
	m_guard  = client->m_guard;

	m_lock   = false;
}

CStream::~CStream()
{
}

bool CStream::InStream(bool byKey)
{
	if (!m_client->OnTRAN(byKey))
		return false;

	m_lock = true;
	m_sndL = 0;
	MakeStream(byKey);

	if (m_sndL > 0 && m_guard->RouteTR(m_client))
	{
		m_lock = false;
		m_client->WaitState(NULL, true);
		return true;
	}
	m_lock = false;
	return false;
}

bool CStream::InStream(CScreen* screen, bool byKey)
{
	if (screen->isUob())
	{
		while (m_client->GetAtScreen(screen, screen->m_parent))
		{
			if (!screen->isUob())
			{
				if (screen->m_mapH->options & OP_OOP)
					break;
				return false;
			}
		}
	}

	if (!screen->OnTRAN(byKey))
		return false;

	m_lock = true;
	m_sndL = 0;
	MakeStream(screen);

	if (m_sndL > 0 && m_guard->RouteTR(m_client))
	{
		m_lock = false;
		m_client->WaitState(screen, true);
		return true;
	}
	m_lock = false;
	return false;
}

void CStream::OutStream(struct _axisH* axisH, char* datB, int datL)
{
	struct _auxH*	auxH;
	CString	guide = _T("");

	if (axisH->stat & statAUX)
	{
		auxH = (struct _auxH *)datB;
		datL -= L_auxH;
		if (datL < 0)
			return;
		datB += L_auxH;
		datL -= auxH->datL;
		if (datL < 0)
			return;

		guide = CString(datB, auxH->datL);
		datB += auxH->datL;
	}

	int	index;
	CfmBase* form;
	CScreen* screen = NULL;
	bool	wait = true;

	switch (axisH->msgK)
	{
	case msgK_HTM:
		datB[datL] = '\0';
		m_guard->SendAxis(MAKEWPARAM(htmlVIEW, axisH->winK), (LPARAM)datB);
		break;
	case msgK_AXIS:
	case msgK_TAB:
		if (!GetScreen(screen, axisH))
			break;

		if (axisH->unit != unitMAIN && axisH->auxs & auxsMAP && axisH->trxC[0] > ' ')
		{
			CString	mapN = CString(axisH->trxC, L_MAPN);
			if (!m_guard->GetMapH(mapN))
			{
				m_guard->SetGuide(AE_CHOICE, m_client->m_key);
				break;
			}
			CScreen* parent;
			m_client->GetAtScreen(parent, screen->m_parent);
			parent->SetObject(screen->m_object, (char *)mapN.operator LPCTSTR());
		}
		else if (axisH->stat & statCLS)
		{
			if (screen->m_mapH->options & OP_CLS)
				screen->ClearForm(CLR_ALL);
			else
				screen->ClearForm(CLR_OUTPUT);
			screen->ResetSystemForm();
		}
		else if (screen->m_mapH->typeH == TH_LEDGER && screen->m_mapH->options & OP_CLS)
		{
			screen->Reset();
			screen->ResetSystemForm();
		}

		screen->m_trxS = axisH->trxS;

		switch (axisH->msgK)
		{
		case msgK_AXIS:
			if (axisH->auxs & auxsOOP)		// symbol data
				SetDataOOP(screen, datB, datL);
			else
				SetDataNRM(screen, datB, datL);
			break;
		case msgK_TAB:
			SetDataTAB(screen, datB, datL);
			break;
		}
		screen->DominoForm(false);
		m_guard->SetCode(screen);

		wait = false;
		screen->m_state &= ~waitSN;
		m_client->m_vm->OnReceive(screen);
		screen->GuideLedger();
		break;

	case msgK_SVC:
		if (GetScreen(screen, axisH) && !screen->m_xscreen->OnService(datB, datL))
		{
			screen->m_state &= ~waitSN;
			m_client->m_vm->OnService(screen, datB, datL);
			m_client->WaitDone(screen, true, false);
		}
		return;

	case msgK_APC:
		if (GetScreen(screen, axisH))
			m_client->m_vm->OnApprove(screen, MAKEWORD(axisH->trxK, axisH->trxS), datB, datL);
		return;

	case msgK_CTRL:
		if (GetScreen(screen, axisH, index))
		{
			screen->m_state &= ~waitSN;
			form = screen->GetAtForm(index);
			if (form && form->m_form->kind == FM_CONTROL)
				form->WriteData(CString(datB, datL));
			m_client->WaitDone(screen, true, false);
		}
		return;

	case msgK_UPF:
	case msgK_DNF:
		if (GetScreen(screen, axisH))
		{
			screen->m_state &= ~waitSN;
			screen->OnFile((axisH->msgK == msgK_UPF) ? TRUE : FALSE, datB, datL);
			m_client->WaitDone(screen, true, false);
		}
		return;

	case msgK_ERR:
		datB[datL] = '\0';
		m_guard->SetGuide(datB, m_client->m_key);
		break;

	default:
		return;
	}

	if (axisH->stat & statAUX)
	{
		if (!(axisH->stat & statNOC) && screen != NULL)
		{
			int	idx;
			if (screen->FindForm(CString(auxH->cursor, sizeof(auxH->cursor)), idx))
				m_client->SetCPos(screen, idx, false);
		}

		if (auxH->type != typeFM || !SetGuide(screen, guide))
			m_guard->SetGuide(auxH->type, guide, m_client->m_key);
	}

	if (axisH->stat & statNOC)
		m_client->m_status |= S_CURSOR;
	m_client->SetCPos(false);
	m_client->WaitDone(screen, true, wait);

	if (screen && axisH->stat & statREP)
		m_client->RepeatTR(screen);
}

bool CStream::SetGuide(CScreen* screen, CString guide, COLORREF rgb)
{
	CfmBase* form = NULL;

	if (screen == NULL)
		m_client->GetAtScreen(screen);
	if (!screen->FindForm("GUIDE", form) && !screen->isMain())
	{
		m_client->GetAtScreen(screen);
		screen->FindForm("GUIDE", form);
	}

	if (form != NULL)
	{
		if (rgb != -1)
			form->SetFgColor(rgb);
		form->WriteData(guide);
		return true;
	}
	return false;
}

void CStream::GetDataH(CScreen* screen, char* sysB, int& sysL)
{
	int	datH = 0;
	switch (screen->m_mapH->typeH)
	{
	case TH_SCUP:
	case TH_4702:
	case TH_KOSCOM:
		break;
	case TH_LEDGER:
		datH = screen->m_ledgerL;
		FillMemory(sysB, datH, ' ');
		screen->GetLedger(sysB);
		break;
	default:
		break;
	}

	sysL += datH;
}

int CStream::GetDataNRM(CScreen* screen, char* iosB, bool tab, bool skip)
{
	CfmBase*	form;
	struct _cellR	*cell;
	CString		text;

	int	idx, rowN, nCols;
	int	formL;
	int	iosL = 0;

	if (!skip)
		GetDataH(screen, iosB, iosL);

	for (int ii = 0; ii < screen->m_ioL; ii++)
	{
		form = screen->GetAtForm(screen->m_ioR[ii]);
		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			CScreen* uob;
			if (m_client->GetAtScreen(uob, form->m_form->keys) && uob->isUob())
				iosL += GetDataNRM(uob, &iosB[iosL], tab, true);
			continue;
		case FM_MEMO:
			if (form->m_form->iok != EIO_OUTPUT)
			{
				form->ReadAll(text);
				formL = text.GetLength();

				if (!tab)
				{
					wsprintf(&iosB[iosL], "%05d", formL);
					iosL += L_FILED;
				}

				if (iosL + formL > maxIOs)
				{
					m_guard->SetGuide(AE_MAXIO, m_client->m_key);
					return 0;
				}
				CopyMemory(&iosB[iosL], (char *)text.operator LPCTSTR(), formL);
				iosL += formL;
				if (tab) iosB[iosL++] = '\t';
			}
			continue;

		case FM_EDIT:
			if (form->m_form->attr & FA_SKIP)
				continue;

			form->ReadData(text);
			text = CString((char *)text.operator LPCTSTR(), text.GetLength());
			if (form->m_form->attr & FA_ENUM && form->m_form->attr & FA_PASSWD)
			{
				CString	string = _T("");

				if (form->m_form->keys != NOVALUE)
				{
					CfmBase* base;
					if (screen->FindForm(&screen->m_strR[form->m_form->keys], base))
					{
						base->ReadData(string);
						string = CString((char *)string.operator LPCTSTR(), string.GetLength());
					}
				}
				text = m_guard->Secure(text, string);
			}

			if (tab)
			{
				text.TrimRight();
				text += '\t';
			}
			formL = text.GetLength();
			CopyMemory(&iosB[iosL], text, formL);
			iosL += formL;
			continue;

		case FM_BUTTON:
			switch (form->m_form->type)
			{
			case BTN_RADIO:
			case BTN_CHECK:
				switch (form->m_form->iok)
				{
				case EIO_INPUT:
				case EIO_INOUT:
					break;
				case EIO_NOP:
				default:
					continue;
				}
				break;
			default:
				continue;
			}
			break;
		case FM_GRID:
			if (form->m_form->vals[0] == NOVALUE)
				continue;

			if (form->m_form->attr2 & GO_HEADER)
			{
				form->GetEnum(text, formL, 99);
				CopyMemory(&iosB[iosL], text, text.GetLength());
				iosL += text.GetLength();
			}

			nCols = HIWORD(form->m_form->vals[1]);
			rowN = ((CfmGrid *)form)->GetValidRows();
			if (rowN <= 0)
				continue;

			bool	edit;

			edit = false;
			for (idx = 0; idx < rowN; idx++)
			{
				cell = (struct _cellR *) &screen->m_cellR[form->m_form->vals[0]];
				if (form->m_form->attr2 & GO_CUD)
				{
					form->ReadData(text, false, 0, idx);
					if (text.GetLength() && text.GetAt(0) == ' ')
						continue;
				}
				for (int kk = 0; kk < nCols; kk++, cell++)
				{
					switch (cell->iok)
					{
					case EIO_INPUT:
					case EIO_INOUT:
						if (cell->attr & FA_SKIP)
							continue;
						edit = true;
						break;
					default:
						continue;
					}

					form->ReadData(text, false, kk, idx);
					if (tab)
					{
						text.TrimRight();
						text += '\t';
					}

					formL = text.GetLength();
					if (iosL + formL > maxIOs)
					{
						m_guard->SetGuide(AE_MAXIO, m_client->m_key);
						return 0;
					}
					CopyMemory(&iosB[iosL], text, formL);
					iosL += formL;
				}
				if (tab && edit) iosB[iosL++] = '\n';
			}
			if (tab && edit) iosB[iosL++] = '\r';
			continue;
		case FM_COMBO:
		case FM_SHEET:
			if (form->m_form->attr & FA_SKIP || !form->m_form->size)
				continue;
			break;
		case FM_TREE:
			if (form->m_form->attr & FA_SKIP || !form->m_form->size)
				continue;

			form->ReadAll(text);
			formL = text.GetLength();
			wsprintf(&iosB[iosL], "%02d", formL/form->m_form->size);
			iosL += 2;

			CopyMemory(&iosB[iosL], text, formL);
			iosL += formL;
			if (tab)
				iosB[iosL++] = '\t';
			continue;
		case FM_CONTROL:
			if (form->m_form->attr & FA_SKIP)
				continue;

			switch (form->m_form->iok)
			{
			case EIO_INPUT:
			case EIO_INOUT:
				form->ReadData(text);
				formL = text.GetLength();
				CopyMemory(&iosB[iosL], text, formL);
				iosL += formL;
				if (tab)
					iosB[iosL++] = (form->m_form->attr & FA_ENUM) ? '\r' : '\t';
				break;
			}
			continue;
		default:
			continue;
		}

		form->ReadData(text);
		text = CString((char *)text.operator LPCTSTR(), text.GetLength());

		if (tab)
		{
			text.TrimRight();
			text += '\t';
		}
		formL = text.GetLength();
		CopyMemory(&iosB[iosL], text, formL);
		iosL += formL;
	}

	return iosL;
}

int CStream::GetDataOOP(CScreen* screen, char* iosB, bool skip)
{
	CfmBase*	form;
	struct _cellR	*cell;

	int	idx, nRow, nRows, vRows, nCols;
	int	iosL = 0;

	char	*symN;
	CString	text;
	int	formL,   symL;

	if (!skip)
		GetDataH(screen, iosB, iosL);

	for (int ii = 0; ii < screen->m_ioL; ii++)
	{
		form = screen->GetAtForm(screen->m_ioR[ii]);
		symN = (char *)form->GetSymbolName().operator LPCSTR();
		symL = (int)strlen(symN);

		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			CScreen* uob;
			if (m_client->GetAtScreen(uob, form->m_form->keys) && uob->isUob())
				iosL += GetDataOOP(uob, &iosB[iosL], true);
			continue;
		case FM_MEMO:
		case FM_BROWSER:
			switch (form->m_form->iok)
			{
			case EIO_INPUT:
			case EIO_INOUT:
				form->ReadAll(text);
				formL = text.GetLength();

				CopyMemory(&iosB[iosL], symN, symL); iosL += symL;
				iosB[iosL++] = FS;
				wsprintf(&iosB[iosL], "%05d", formL);
				iosL += L_FILED;

				if (iosL + formL > maxIOs)
				{
					m_guard->SetGuide(AE_MAXIO, m_client->m_key);
					return 0;
				}
				CopyMemory(&iosB[iosL], (char *)text.operator LPCTSTR(), formL);
				iosL += formL;
				iosB[iosL++] = '\t';

				if (form->m_form->iok == EIO_INPUT)
					break;
			case EIO_OUTPUT:
				CopyMemory(&iosB[iosL], symN, symL); iosL += symL;
				iosB[iosL++] = '\t';
				break;
			}

			continue;
		case FM_EDIT:
			if (form->m_form->attr & FA_SKIP)
				continue;

			switch (form->m_form->iok)
			{
			case EIO_INPUT:
			case EIO_INOUT:
				form->ReadData(text);
				if (form->m_form->attr & FA_ENUM && form->m_form->attr & FA_PASSWD)
				{
					CString	string = _T("");

					if (form->m_form->keys != NOVALUE)
					{
						CfmBase* base;
						if (screen->FindForm(&screen->m_strR[form->m_form->keys], base))
						{
							base->ReadData(string);
							string = CString((char *)string.operator LPCTSTR(), string.GetLength());
						}
					}
					text = m_guard->Secure(text, string);
				}
				formL = text.GetLength();
				CopyMemory(&iosB[iosL], symN, symL); iosL += symL;
				iosB[iosL++] = FS;
				CopyMemory(&iosB[iosL], text, formL); iosL += formL;
				iosB[iosL++] = '\t';

				if (form->m_form->iok == EIO_INPUT)
					break;
			case EIO_OUTPUT:
				CopyMemory(&iosB[iosL], symN, symL); iosL += symL;
				iosB[iosL++] = '\t';
				break;
			}
			continue;

		case FM_OUT:
			if (form->m_form->attr & FA_SKIP)
				continue;
			form->m_form->iok = EIO_OUTPUT;
			break;
		case FM_BUTTON:
			switch (form->m_form->type)
			{
			case BTN_RADIO:
			case BTN_CHECK:
				break;
			default:
				continue;
			}
			break;
		case FM_TABLE:
			if (form->m_form->iok == EIO_NOP)
				continue;
			if (form->m_form->vals[0] == NOVALUE)
				continue;

			text.Empty();
			cell = (struct _cellR *) &screen->m_cellR[form->m_form->vals[0]];

			nRows = LOWORD(form->m_form->vals[1]);
			for (nRow = 0; nRow < nRows; nRow++)
			{
				if (cell[nRow].attr & FA_SKIP)
					continue;

				text += form->GetSymbolName(cell[nRow].name);
				text += '\t';
			}
			CopyMemory(&iosB[iosL], text, text.GetLength());
			iosL += text.GetLength();
			continue;

		case FM_GRID:
			if (form->m_form->vals[0] == NOVALUE)
				continue;

			iosB[iosL++] = '$';
			CopyMemory(&iosB[iosL], symN, symL); iosL += symL;
			iosB[iosL++] = FS;

			text.Empty();
			form->GetEnum(text, formL, 99);
			CopyMemory(&iosB[iosL], text, text.GetLength());
			iosL += text.GetLength();

			text.Empty();
			CPtrArray*	grid;

			nCols = HIWORD(form->m_form->vals[1]);
			if (screen->m_cells.Lookup(form->GetSymbolName(), (CObject *&)grid))
				nCols = (int)grid->GetSize();
			else
				grid = NULL;

			cell = (struct _cellR *) &screen->m_cellR[form->m_form->vals[0]];
			vRows = ((CfmGrid *)form)->GetValidRows();
			for (idx = 0; idx < nCols; idx++, cell++)
			{
				if (grid != NULL)
					cell = (struct _cellR *)grid->GetAt(idx);
				else if (idx >= form->m_form->type && !(cell->properties & PR_VISIBLE))
					continue;
				if (cell->iok == EIO_NOP || cell->attr & FA_SKIP)
					continue;

				symN = (char *)form->GetSymbolName(cell->name).operator LPCSTR();
				symL = (int)strlen(symN);

				CopyMemory(&iosB[iosL], symN, symL);
				iosL += symL;

				switch (cell->iok)
				{
				case EIO_INPUT:
				case EIO_INOUT:
					int	kk;
					for (kk = 0; kk < vRows; kk++)
					{
						iosB[iosL++] = FS;
						form->ReadData(text, false, idx, kk);
						formL = text.GetLength();
						if (iosL + formL > maxIOs)
						{
							m_guard->SetGuide(AE_MAXIO, m_client->m_key);
							return 0;
						}
						CopyMemory(&iosB[iosL], text, formL);
						iosL += formL;
					}
					if (cell->iok == EIO_INOUT && vRows > 0)
					{
						iosB[iosL++] = '\n';
						CopyMemory(&iosB[iosL], symN, symL);
						iosL += symL;
					}
				default:
					iosB[iosL++] = '\n';
					break;
				}
			}
			iosB[iosL++] = '\t';
			continue;

		case FM_COMBO:
		case FM_SHEET:
			if (form->m_form->attr & FA_SKIP)
				continue;
			break;
		case FM_TREE:
			if (form->m_form->attr & FA_SKIP)
				continue;

			form->ReadAll(text);
			formL = text.GetLength();
			CopyMemory(&iosB[iosL], symN, symL); iosL += symL;
			iosB[iosL++] = FS;

			wsprintf(&iosB[iosL], "%02d", form->m_form->size ? formL/form->m_form->size : formL);
			iosL += 2;

			CopyMemory(&iosB[iosL], text, formL);
			iosL += formL;
			iosB[iosL++] = '\t';
			continue;
		case FM_CONTROL:
			if (form->m_form->attr & FA_SKIP)
				continue;

			if (form->m_form->attr & FA_ENUM)
			{
				iosB[iosL++] = '$';
				CopyMemory(&iosB[iosL], symN, symL); iosL += symL;
				iosB[iosL++] = FS;
				form->ReadData(text);
				CopyMemory(&iosB[iosL], text, text.GetLength());
				iosL += text.GetLength();
				iosB[iosL++] = '\t';
				continue;
			}
			break;
		default:
			continue;
		}

		switch (form->m_form->iok)
		{
		case EIO_INPUT:
		case EIO_INOUT:
			form->ReadData(text);
			formL = text.GetLength();

			CopyMemory(&iosB[iosL], symN, symL); iosL += symL;
			iosB[iosL++] = FS;
			CopyMemory(&iosB[iosL], text, formL); iosL += formL;
			iosB[iosL++] = '\t';

			if (form->m_form->iok == EIO_INPUT)
				break;
		case EIO_OUTPUT:
			CopyMemory(&iosB[iosL], symN, symL); iosL += symL;
			iosB[iosL++] = '\t';
			break;
		}

	}
	return iosL;
}

int CStream::GetDataOOP2(CScreen* screen, char* iosB, bool skip)
{
	CfmBase*	form;
	struct _cellR	*cell;

	int	idx, nRow, nRows, vRows, nCols;
	int	iosL = 0;

	char	*symN;
	CString	text, tmps, inbs;
	int	formL,   symL;

	if (!skip)
		GetDataH(screen, iosB, iosL);

	for (int ii = 0; ii < screen->m_ioL; ii++)
	{
		form = screen->GetAtForm(screen->m_ioR[ii]);
		symN = (char *)form->GetSymbolName().operator LPCSTR();
		symL = (int)strlen(symN);

		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			CScreen* uob;
			if (m_client->GetAtScreen(uob, form->m_form->keys) && uob->isUob())
				iosL += GetDataOOP2(uob, &iosB[iosL], true);
			continue;
		case FM_MEMO:
		case FM_BROWSER:
			switch (form->m_form->iok)
			{
			case EIO_INPUT:
			case EIO_INOUT:
				form->ReadAll(text);
				formL = text.GetLength();

				CopyMemory(&iosB[iosL], symN, symL); iosL += symL;
				iosB[iosL++] = (form->m_form->iok == EIO_INPUT) ? iFS : uFS;
				wsprintf(&iosB[iosL], "%05d", formL);
				iosL += L_FILED;

				if (iosL + formL > maxIOs)
				{
					m_guard->SetGuide(AE_MAXIO, m_client->m_key);
					return 0;
				}
				CopyMemory(&iosB[iosL], (char *)text.operator LPCTSTR(), formL);
				iosL += formL;
				iosB[iosL++] = '\t';
				break;
			case EIO_OUTPUT:
				CopyMemory(&iosB[iosL], symN, symL); iosL += symL;
				iosB[iosL++] = '\t';
				break;
			}

			continue;
		case FM_EDIT:
			if (form->m_form->attr & FA_SKIP)
				continue;

			switch (form->m_form->iok)
			{
			case EIO_INPUT:
			case EIO_INOUT:
				form->ReadData(text);
				if (form->m_form->attr & FA_ENUM && form->m_form->attr & FA_PASSWD)
				{
					CString	string = _T("");

					if (form->m_form->keys != NOVALUE)
					{
						CfmBase* base;
						if (screen->FindForm(&screen->m_strR[form->m_form->keys], base))
						{
							base->ReadData(string);
							string = CString((char *)string.operator LPCTSTR(), string.GetLength());
						}
					}
					text = m_guard->Secure(text, string);
				}
				formL = text.GetLength();
				CopyMemory(&iosB[iosL], symN, symL); iosL += symL;
				iosB[iosL++] = (form->m_form->iok == EIO_INPUT) ? iFS : uFS;
				CopyMemory(&iosB[iosL], text, formL); iosL += formL;
				iosB[iosL++] = '\t';
				break;
			case EIO_OUTPUT:
				CopyMemory(&iosB[iosL], symN, symL); iosL += symL;
				iosB[iosL++] = '\t';
				break;
			}
			continue;

		case FM_OUT:
			if (form->m_form->attr & FA_SKIP)
				continue;
			form->m_form->iok = EIO_OUTPUT;
			break;
		case FM_BUTTON:
			switch (form->m_form->type)
			{
			case BTN_RADIO:
			case BTN_CHECK:
				break;
			default:
				continue;
			}
			break;
		case FM_TABLE:
			if (form->m_form->iok == EIO_NOP)
				continue;
			if (form->m_form->vals[0] == NOVALUE)
				continue;

			text.Empty();
			cell = (struct _cellR *) &screen->m_cellR[form->m_form->vals[0]];

			nRows = LOWORD(form->m_form->vals[1]);
			for (nRow = 0; nRow < nRows; nRow++)
			{
				if (cell[nRow].attr & FA_SKIP)
					continue;

				text += form->GetSymbolName(cell[nRow].name);
				text += '\t';
			}
			CopyMemory(&iosB[iosL], text, text.GetLength());
			iosL += text.GetLength();
			continue;

		case FM_GRID:
			if (form->m_form->vals[0] == NOVALUE)
				continue;

			CopyMemory(&iosB[iosL], symN, symL); iosL += symL;
			iosB[iosL++] = '$';

			text.Empty();
			form->GetEnum(text, formL, 99);
			CopyMemory(&iosB[iosL], text, text.GetLength());
			iosL += text.GetLength();

			text.Empty();
			CPtrArray*	grid;

			nCols = HIWORD(form->m_form->vals[1]);
			if (screen->m_cells.Lookup(form->GetSymbolName(), (CObject *&)grid))
				nCols = (int)grid->GetSize();
			else
				grid = NULL;

			vRows = ((CfmGrid *)form)->GetValidRows();

			int	kk;
			formL = 0;
			for (kk = -1; kk < vRows; kk++)
			{
				inbs.Empty();
				cell = (struct _cellR *) &screen->m_cellR[form->m_form->vals[0]];
				for (idx = 0; idx < nCols; idx++, cell++)
				{
					if (grid != NULL)
						cell = (struct _cellR *)grid->GetAt(idx);
					else if (idx >= form->m_form->type && !(cell->properties & PR_VISIBLE))
						continue;
					if (cell->iok == EIO_NOP || cell->attr & FA_SKIP)
						continue;

					switch (cell->iok)
					{
					case EIO_INPUT:
					case EIO_INOUT:
						if (kk == -1)
						{
							formL++;
							text += (cell->iok == EIO_INPUT) ? '1' : '3';
							text += form->GetSymbolName(cell->name);
							text += '\t';
							continue;
						}
						break;
					case EIO_OUTPUT:
						if (kk == -1)
						{
							formL++;
							text += '2';
							text += form->GetSymbolName(cell->name);
							text += '\t';
						}
					default:
						continue;
					}
					form->ReadData(tmps, false, idx, kk);
					inbs += tmps;
					inbs = '\t';
				}
				if (kk == -1)
				{
					tmps.Format("%02d%s", formL, text);
					formL = tmps.GetLength();
					if (iosL + formL > maxIOs)
					{
						m_guard->SetGuide(AE_MAXIO, m_client->m_key);
						return 0;
					}
					CopyMemory(&iosB[iosL], tmps, formL);
					iosL += formL;
					continue;
				}
				if (inbs.IsEmpty())
					break;
				formL = inbs.GetLength();
				if (iosL + formL > maxIOs)
				{
					m_guard->SetGuide(AE_MAXIO, m_client->m_key);
					return 0;
				}
				CopyMemory(&iosB[iosL], inbs, formL);
				iosL += formL;
				iosB[iosL++] = '\n';
			}
			iosB[iosL++] = gFS;
			continue;

		case FM_COMBO:
		case FM_SHEET:
			if (form->m_form->attr & FA_SKIP)
				continue;
			break;
		case FM_TREE:
			if (form->m_form->attr & FA_SKIP)
				continue;

			form->ReadAll(text);
			formL = text.GetLength();
			CopyMemory(&iosB[iosL], symN, symL); iosL += symL;
			iosB[iosL++] = iFS;

			wsprintf(&iosB[iosL], "%02d", form->m_form->size ? formL/form->m_form->size : formL);
			iosL += 2;

			CopyMemory(&iosB[iosL], text, formL);
			iosL += formL;
			iosB[iosL++] = '\t';
			continue;
		case FM_CONTROL:
			if (form->m_form->attr & FA_SKIP)
				continue;

			if (form->m_form->attr & FA_ENUM)
			{
				CopyMemory(&iosB[iosL], symN, symL); iosL += symL;
				iosB[iosL++] = '$';
				form->ReadData(text);
				CopyMemory(&iosB[iosL], text, text.GetLength());
				iosL += text.GetLength();
				iosB[iosL++] = gFS;
				continue;
			}
			break;
		default:
			continue;
		}

		switch (form->m_form->iok)
		{
		case EIO_INPUT:
		case EIO_INOUT:
			form->ReadData(text);
			formL = text.GetLength();

			CopyMemory(&iosB[iosL], symN, symL); iosL += symL;
			iosB[iosL++] = (form->m_form->iok == EIO_INPUT) ? iFS : uFS;
			CopyMemory(&iosB[iosL], text, formL); iosL += formL;
			iosB[iosL++] = '\t';
			break;
		case EIO_OUTPUT:
			CopyMemory(&iosB[iosL], symN, symL); iosL += symL;
			iosB[iosL++] = '\t';
			break;
		}

	}
	return iosL;
}

void CStream::MakeStream(bool byKey)
{
	CScreen* screen;

	for (int idx = 0; idx < m_client->m_magic; idx++)
	{
		if (!m_client->GetAtScreen(screen, idx))
			continue;

		if (screen->m_skip || screen->m_mapH->mapM == MM_MENU)
			continue;

		if (screen->isUob())
			continue;

		MakeStream(screen);
	}
}

void CStream::MakeStream(CScreen* screen)
{
	if (screen->m_state & waitSN)
		return;			// maybe does not happen except for script bug

	struct _axisH*	axisH;

	int	axisL = 0;
	axisH = (struct _axisH*) &m_sndB[m_sndL];
	ZeroMemory(axisH, L_axisH);

	axisH->msgK = msgK_AXIS;
	axisH->winK = m_client->m_key;
	axisH->unit = screen->m_key;
	axisH->trxK = screen->m_trxK;
	screen->m_trxK = 0;

	bool	tab = false;
	if (screen->m_mapH->options & OP_TABS)
	{
		axisH->msgK = msgK_TAB;
		tab = true;
	}

	if (screen->m_mapH->options & OP_CNV)
		axisH->stat |= statCNV;
	CopyMemory(axisH->trxC, screen->m_mapH->trxC, L_TRXC);
	CopyMemory(axisH->svcN, CString(screen->m_mapH->mapN, L_MAPN).Mid(L_SGID, L_SELC), sizeof(axisH->svcN));

	m_sndL += L_axisH;

	if (screen->m_mapH->options & OP_OOP)
	{
		axisH->auxs |= auxsOOP;
		if (screen->m_mapH->options & OP_OOP2)
			axisL = GetDataOOP2(screen, &m_sndB[m_sndL]);
		else
			axisL = GetDataOOP(screen, &m_sndB[m_sndL]);
	}
	else
		axisL = GetDataNRM(screen, &m_sndB[m_sndL], tab);

	m_guard->xTRACE(x_SNDs, &m_sndB[m_sndL-L_axisH], L_axisH+axisL);

	if (!(m_guard->m_term & flagCAX) && screen->m_mapH->options & OP_CERTIFY)
	{
		if (!m_guard->Certify(&m_sndB[m_sndL], axisL, CString(screen->m_mapH->mapN, L_MAPN)))
		{
			m_sndL -= L_axisH;
			if (!SetGuide(screen, ""))
				m_guard->SetGuide(AE_CLEAR, m_client->m_key);
			return;
		}
		axisH->auxs |= auxsCA;
	}

	if (!(m_guard->m_term & flagENX) && screen->m_mapH->options & OP_ENC)
	{
		if (!m_guard->Xecure(DI_ENC, &m_sndB[m_sndL], axisL))
		{
			m_sndL -= L_axisH;
			SetGuide(screen, "");
			m_guard->SetGuide(AE_CSECURE, m_client->m_key);
			return;
		}
		axisH->stat |= statENC;
	}
 
	CString	text;
	text.Format("%05d", axisL);
	CopyMemory(axisH->datL, (char *)text.operator LPCTSTR(), sizeof(axisH->datL));
	m_sndL += axisL;
}

bool CStream::GetScreen(CScreen*& screen, struct _axisH* axisH)
{
	for (int ii = 0; ii < m_client->m_magic; ii++)
	{
		if (!m_client->GetAtScreen(screen, ii))
			continue;

		switch (axisH->unit)
		{
		case unitSUB:
			if (!memcmp(axisH->trxC, screen->m_mapH->trxC, L_TRXC))
			{
				axisH->trxC[0] = ' ';
				axisH->unit = screen->m_key;
				return true;
			}
			break;
		case unitMAIN:
		default:
			if (axisH->unit == screen->m_key)
				return true;
			break;
		}
	}

	return false;
}

BOOL CStream::GetScreen(class CScreen*& screen, struct _axisH* axisH, int& index)
{
	int	key = m_client->GetScreenKey(axisH->unit, index);
	if (key < 0)
		return FALSE;
	return m_client->GetAtScreen(screen, key);
}

int CStream::SetDataNRM(CScreen* screen, char* axisB, int axisL, bool skip)
{
	int	idx = 0;
	if (!skip && !SetDataH(screen, &axisB[idx], idx))
		return axisL;

	CfmBase* form;
	int	formL, row;
	int	nRows, nCols;
	CString	text;
	bool	isSup;
	struct _cellR*	cell;

	for (int key = 0; idx < axisL; )
	{
		if (ParseCC(screen, &axisB[idx], key, idx))
			continue;

		if (key >= screen->m_ioL)
			return idx;

		isSup = true;
		form = screen->GetAtForm(screen->m_ioR[key]);

		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			CScreen* uob;
			if (m_client->GetAtScreen(uob, form->m_form->keys) && uob->isUob())
				idx += SetDataNRM(uob, &axisB[idx], axisL - idx, true);
			break;

		case FM_BUTTON:
			switch (form->m_form->iok)
			{
			case EIO_OUTPUT:
			case EIO_INOUT:
				((CfmRadio *)form)->SetChecked(axisB[idx] > '0' ? true : false);
				idx++;
				break;
			default:
				break;
			}
			break;

		case FM_MEMO:
			if (form->m_form->iok == EIO_INPUT)
				break;

			if (L_FILED > axisL - idx)
				return axisL;
			formL = atoi(CString(&axisB[idx], L_FILED));
			if (formL > axisL - idx)
				return axisL;
			idx += L_FILED;

			form->WriteAll(CString(&axisB[idx], formL));
			form->IsChanged();
			idx += formL;
			break;

		case FM_BROWSER:
			if (form->m_form->iok != EIO_OUTPUT)
				break;

			if (L_FILED > axisL - idx)
				return axisL;
			formL = atoi(CString(&axisB[idx], L_FILED));
			if (formL > axisL - idx)
				return axisL;
			idx += L_FILED;

			form->WriteAll(CString(&axisB[idx], formL));
			idx += formL;
			break;

		case FM_COMBO:
		case FM_EDIT:
			if (form->m_form->iok == EIO_INPUT)
				break;
			isSup = false;
		case FM_OUT:
			if (form->m_form->attr & FA_SKIP)
				break;

			formL = form->m_form->size;
			if (formL > axisL - idx)
				return axisL;

			form->WriteData(CString(&axisB[idx], formL));
			if (!isSup)	form->IsChanged();

			idx += formL;
			break;

		case FM_GRID:
			if (form->m_form->vals[0] == NOVALUE)
				break;

			if (form->m_form->attr2 & GO_HEADER)
			{
				formL = 0;
				form->GetEnum(text, formL, 98);
				if (formL > axisL - idx)
					return axisL;

				form->SetEnum(CString(&axisB[idx], formL), formL, 99);
				idx += formL;
			}

			nCols = HIWORD(form->m_form->vals[1]);
			if (form->m_form->attr2 & GO_FLEX)
			{
				nRows = atoi(CString(&axisB[idx], L_FLEX));
				idx += L_FLEX;
			}
			else	nRows = LOWORD(form->m_form->vals[1]);

			if (form->m_form->attr2 & GO_TABLE)
				SetTable(screen, key, nCols, nRows, &axisB[idx], axisL, idx);
			else
				SetCells(screen, form, nRows, nCols, axisB, idx, axisL);

			screen->OnProfit(form);
			form->IsChanged();
			break;

		case FM_TABLE:
			if (form->m_form->iok == EIO_NOP)
				break;
			if (form->m_form->vals[0] == NOVALUE)
				break;

			text.Empty();
			cell = (struct _cellR *) &screen->m_cellR[form->m_form->vals[0]];

			nRows = LOWORD(form->m_form->vals[1]);
			for (row = 0; row < nRows && idx < axisL; row++)
			{
				if (cell[row].attr & FA_SKIP)
					continue;

				formL = cell[row].size;
				if (formL > axisL - idx)
					return axisL;

				text += CString(&axisB[idx], formL);
				text += '\t';
				idx += formL;
			}
			form->WriteAll(text);
			break;
		case FM_CONTROL:
			if (form->m_form->attr & FA_SKIP)
				break;

			switch (form->m_form->iok)
			{
			case EIO_INOUT:
			case EIO_OUTPUT:
				formL = form->m_form->size;
				if (!formL || formL > axisL - idx)
					formL = axisL - idx;

				form->WriteData(CString(&axisB[idx], formL));
				form->IsChanged();
				idx += formL;
				break;
			}
			break;
		}
		key++;
	}
	return idx;
}

int CStream::SetDataOOP(CScreen* screen, char* axisB, int axisL, bool skip)
{
	int idx = 0;

	if (!skip && !SetDataH(screen, &axisB[idx], idx))
		return axisL;

	CfmBase* form;
	struct	_cellR* cell;
	int	pos, index;
	bool	isSup;

	CString	text, dats;
	text = CString(&axisB[idx], axisL - idx);

	for (int key = 0; idx < axisL; )
	{
		if (ParseCC(screen, &axisB[idx], key, idx))
			continue;

		if (key >= screen->m_ioL)
			return idx;

		isSup = true;
		form = screen->GetAtForm(screen->m_ioR[key]);

		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			CScreen* uob;
			if (m_client->GetAtScreen(uob, form->m_form->keys) && uob->isUob())
				pos = SetDataOOP(uob, &axisB[idx], axisL - idx, true);
			else
				pos = 0;

			break;

		case FM_COMBO:
		case FM_EDIT:
		case FM_MEMO:
			isSup = false;
		case FM_BUTTON:
		case FM_BROWSER:
			switch (form->m_form->iok)
			{
			case EIO_OUTPUT:
			case EIO_INOUT:
				break;
			default:
				key++;
				continue;
			}
		case FM_OUT:
			if (form->m_form->attr & FA_SKIP)
			{
				key++;
				continue;
			}
			pos = text.Find('\t');
			if (pos != -1)
				text = text.Left(pos++);
			else
				pos = text.GetLength();

			if (form->m_form->kind == FM_MEMO || form->m_form->kind == FM_BROWSER)
				form->WriteAll(text);
			else
				form->WriteData(text);

			if (!isSup)	form->IsChanged();

			break;

		case FM_GRID:
			if (form->m_form->vals[0] == NOVALUE || form->m_form->iok == EIO_INPUT)
			{
				key++;
				continue;
			}
			pos = 0;
			form->GetEnum(dats, pos, 98);
			if (pos > axisL - idx)
				return axisL;

			form->SetEnum(text, pos, 99);

			text = text.Mid(pos);
			if (screen->m_mapH->options & OP_OOP2)
				index = text.Find(gFS);
			else
				index = text.Find("\r\t");
			if (index == -1)
				pos += text.GetLength();
			else
			{
				text = text.Left(index);
				pos += index + ((screen->m_mapH->options & OP_OOP2) ? 1 : 2);
			}

			form->WriteAll(text);
			screen->OnProfit(form);
			form->IsChanged();
			break;

		case FM_TABLE:
			if (form->m_form->iok == EIO_NOP || form->m_form->vals[0] == NOVALUE)
			{
				key++;
				continue;
			}

			pos = 0;
			dats.Empty();
			cell = (struct _cellR *) &screen->m_cellR[form->m_form->vals[0]];

			int	ii;
			for (ii = 0; ii < LOWORD(form->m_form->vals[1]); ii++, cell++)
			{
				if (cell->attr & FA_SKIP)
					continue;
				index = text.Find('\t');
				if (index == -1)
				{
					dats += text;
					pos += text.GetLength();
					break;
				}
				dats += text.Left(++index);
				text = text.Mid(index);
				pos += index;
			}
			form->WriteAll(dats);
			break;

		case FM_CONTROL:
			if (form->m_form->attr & FA_SKIP)
			{
				key++;
				continue;
			}

			switch (form->m_form->iok)
			{
			case EIO_INOUT:
			case EIO_OUTPUT:
				if (form->m_form->attr & FA_ENUM)
				{
					if (screen->m_mapH->options & OP_OOP2)
						index = text.Find(gFS);
					else
						index = text.Find("\r\t");
					if (index == -1)
						pos += text.GetLength();
					else
					{
						text = text.Left(index);
						pos += index + ((screen->m_mapH->options & OP_OOP2) ? 1 : 2);
					}
				}
				else
				{
					pos = text.Find('\t');
					if (pos != -1)
						text = text.Left(pos++);
					else
						pos = text.GetLength();
				}
				form->WriteData(text);
				form->IsChanged();
				break;
			default:
				pos = 0;
				break;
			}
			break;
		default:
			key++;
			continue;
		}

		key++;
		idx += pos;
		text = CString(&axisB[idx], axisL - idx);
	}

	return idx;
}

int CStream::SetDataTAB(CScreen* screen, char* axisB, int axisL, bool skip)
{
	int	idx = 0;

	if (!skip && (idx >= axisL || !SetDataH(screen, &axisB[idx], idx)))
		return axisL;

	int	pos, index;
	bool	isSup;
	struct	_cellR*	cell;

	CfmBase* form;
	CString	text, dats;
	text = CString(&axisB[idx], axisL - idx);

	for (int key = 0; idx < axisL; )
	{
		if (ParseCC(screen, &axisB[idx], key, idx))
			continue;

		if (key >= screen->m_ioL)
			return idx;

		form = screen->GetAtForm(screen->m_ioR[key]);

		isSup = true;
		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			CScreen* uob;
			if (m_client->GetAtScreen(uob, form->m_form->keys) && uob->isUob())
				pos = SetDataTAB(uob, &axisB[idx], axisL - idx, true);
			else
				pos = 0;
			break;

		case FM_COMBO:
		case FM_EDIT:
		case FM_MEMO:
			isSup = false;
		case FM_BUTTON:
		case FM_BROWSER:
			switch (form->m_form->iok)
			{
			case EIO_INOUT:
			case EIO_OUTPUT:
				break;
			default:
				key++;
				continue;
			}
		case FM_OUT:
			if (form->m_form->attr & FA_SKIP)
			{
				key++;
				continue;
			}

			pos = text.Find('\t');
			if (pos != -1)
				text = text.Left(pos++);
			else
				pos = text.GetLength();

			if (form->m_form->kind == FM_MEMO || form->m_form->kind == FM_BROWSER)
				form->WriteAll(text);
			else
				form->WriteData(text);

			if (!isSup)	form->IsChanged();

			break;
		case FM_TABLE:
			if (form->m_form->iok == EIO_NOP)
			{
				key++;
				continue;
			}

			pos = 0;
			dats.Empty();
			cell = (struct _cellR *) &screen->m_cellR[form->m_form->vals[0]];

			int	ii;
			for (ii = 0; ii < LOWORD(form->m_form->vals[1]); ii++, cell++)
			{
				if (cell->attr & FA_SKIP)
					continue;
				index = text.Find('\t');
				if (index == -1)
				{
					dats += text;
					pos += text.GetLength();
					break;
				}
				dats += text.Left(++index);
				text = text.Mid(index);
				pos += index;
			}
			form->WriteAll(dats);
			break;

		case FM_GRID:
			if (form->m_form->vals[0] == NOVALUE || form->m_form->iok == EIO_INPUT)
			{
				key++;
				continue;
			}

			if (form->m_form->attr2 & GO_HEADER)
			{
				pos = 0;
				form->GetEnum(dats, pos, 98);
				if (pos > axisL - idx)
					return axisL;

				form->SetEnum(text, pos, 99);
				text = text.Mid(pos);
			}

			pos = text.Find('\r');
			if (pos != -1)
				text = text.Left(pos++);
			else
				pos = text.GetLength();

			SetCells(screen, form, text);
			form->WriteAll(text);
			screen->OnProfit(form);
			form->IsChanged();
			break;

		case FM_CONTROL:
			if (form->m_form->attr & FA_SKIP)
			{
				key++;
				continue;
			}

			switch (form->m_form->iok)
			{
			case EIO_INOUT:
			case EIO_OUTPUT:
				pos = text.Find((form->m_form->attr & FA_ENUM) ? '\r' : '\t');
				if (pos != -1)
					text = text.Left(pos++);
				else
					pos = text.GetLength();
				form->WriteData(text);
				form->IsChanged();
				break;
			default:
				pos = 0;
				break;
			}
			break;
		default:
			key++;
			continue;
		}

		key++;
		idx += pos;
		text = CString(&axisB[idx], axisL - idx);
	}
	return idx;
}

bool CStream::SetDataH(CScreen* screen, char* datB, int& datH)
{
	int	skip;

	datH = 0;
	switch (screen->m_mapH->typeH)
	{
	case TH_KOSCOM:
	case TH_SCUP:
	case TH_4702:
		break;
	case TH_LEDGER:
		datH += screen->m_ledgerL;
		screen->SetLedger(datB);
		skip = atoi(m_guard->GetLedger(screen->m_ledger, getOK));
		if (skip == 0)
			return false;
		if (skip < 0)
			datH += -skip;
		break;
	default:
		break;
	}
	return true;
}

void CStream::SetCells(CScreen* screen, CfmBase* form, int nRows, int nCols, char* axisB, int& idx, int axisL)
{
	int	ii, col, row, formL;
	CMap	<int, int, CString, CString&>	xgrid;

	CString	text;
	struct _cellR*	cell;
	CPtrArray* grid = NULL;

	if (form->m_form->attr & FA_ENUM)
		screen->m_cells.Lookup(form->GetSymbolName(), (CObject *&)grid);
	cell = (struct _cellR *)&screen->m_cellR[form->m_form->vals[0]];

	text.Empty();

	for (row = 0; row < nRows && idx < axisL; row++)
	{
		for (col = 0; col < nCols && idx < axisL; col++)
		{
			if (cell[col].attr & FA_SKIP)
				continue;

			switch (cell[col].iok)
			{
			case EIO_INOUT:
			case EIO_OUTPUT:
				formL = cell[col].size;
				if (formL <= 0)
				{
					if (grid == NULL)
					{
						text += '\t';
						continue;
					}
					formL = 0;
				}
				break;
			case EIO_NOP:
			default:
				continue;
			}

			if (formL > axisL - idx)
			{
				idx = axisL;
				break;			// return false;
			}

			if (grid)
			{
				for (int ii = 0; ii < grid->GetSize(); ii++)
				{
					if (((struct _cellR*)grid->GetAt(ii))->name == cell[col].name)
					{
						xgrid.SetAt(ii, CString(&axisB[idx], formL));
						break;
					}
				}
			}
			else
			{
				text += CString(&axisB[idx], formL);
				text += '\t';
			}
			idx += formL;
		}

		if (grid)
		{
			CString	tmps;

			for (col = ii = 0; ii < xgrid.GetCount(); col++)
			{
				if (xgrid.Lookup(col, tmps))
				{
					text += tmps;
					text += '\t';
					ii++;
				}
			}
			xgrid.RemoveAll();
		}
		text += '\n';
	}

	form->WriteAll(text);
}

void CStream::SetCells(CScreen* screen, CfmBase* form, CString& text)
{
	int	nCols, nRows;
	int	ii, col, row;
	CMap	<int, int, CString, CString&>	xgrid;

	CString	tmps, forms, cells;
	struct _cellR*	cell;
	CPtrArray* grid = NULL;

	if (form->m_form->attr & FA_ENUM)
		screen->m_cells.Lookup(form->GetSymbolName(), (CObject *&)grid);
	if (!grid)	return;

	nCols = HIWORD(form->m_form->vals[1]);
	nRows = LOWORD(form->m_form->vals[1]);

	tmps = text;
	text.Empty();
	cell = (struct _cellR *)&screen->m_cellR[form->m_form->vals[0]];

	for (row = 0; row < nRows && !tmps.IsEmpty(); row++)
	{
		ii = tmps.Find('\n');
		if (ii == -1)
		{
			forms = tmps;
			tmps.Empty();
		}
		else
		{
			forms = tmps.Left(ii++);
			tmps = tmps.Mid(ii);
		}

		for (col = 0; col < nCols && !forms.IsEmpty(); col++)
		{
			if (cell[col].attr & FA_SKIP)
				continue;

			switch (cell[col].iok)
			{
			case EIO_INOUT:
			case EIO_OUTPUT:
				break;
			case EIO_NOP:
			default:
				continue;
			}

			ii = forms.Find('\t');
			if (ii == -1)
			{
				cells = forms;
				forms.Empty();
			}
			else
			{
				cells = forms.Left(ii++);
				forms = forms.Mid(ii);
			}
			for (int ii = 0; ii < grid->GetSize(); ii++)
			{
				if (((struct _cellR*)grid->GetAt(ii))->name == cell[col].name)
				{
					xgrid.SetAt(ii, cells);
					break;
				}
			}
		}

		for (col = ii = 0; ii < xgrid.GetCount(); col++)
		{
			if (xgrid.Lookup(col, cells))
			{
				text += cells;
				text += '\t';
				ii++;
			}
		}
		xgrid.RemoveAll();
		text += '\n';
	}
}

void CStream::SetTable(CScreen* screen, int key, int nCols, int nRows, char* datB, int datL, int& idx)
{
	CString	text;
	CfmBase* form;
	struct	_cellR* cell;

	CMap	<int, int, CString, CString&>	xgrid;
	CPtrArray* grid = NULL;

	text.Empty();
	form  = screen->GetAtForm(screen->m_ioR[key]);

	if (form->m_form->attr & FA_ENUM)
		screen->m_cells.Lookup(form->GetSymbolName(), (CObject *&)grid);

	int	skip = 0;
	for (int ii = 0; ii < nRows && idx + skip < datL; ii++)
	{
		cell = (struct _cellR *) &screen->m_cellR[form->m_form->vals[0]];
		for (int kk = 0; kk < nCols && idx + skip < datL; kk++, cell++)
		{
			switch (cell->iok)
			{
			case EIO_INOUT:
			case EIO_OUTPUT:
				if (cell->attr & FA_SKIP)
					continue;
				break;
			default:
				continue;
			}

			while (idx + skip < datL)
			{
				switch (datB[skip])
				{
				case FCC:
					ParseFCC(screen, key, (_FCC *)&datB[skip], kk, ii);
					skip += L_FCC;
					continue;
				case RCC:
					skip += ParseRCC(screen, &datB[skip]);
					continue;
				case SCC:
					ParseSCC(screen, form, (_SCC *)&datB[skip], kk, ii);
					skip += L_SCC;
					continue;
				}
				if (cell->size <= datL-idx-skip)
				{
					if (grid)
					{
						for (int kk = 0; ii < grid->GetSize(); kk++)
						{
							if (((struct _cellR*)grid->GetAt(kk))->name == cell->name)
							{
								xgrid.SetAt(kk, CString(&datB[skip], cell->size));
								break;
							}
						}
					}
					else
					{
						text += CString(&datB[skip], cell->size);
						text += '\t';
					}
				}
				skip += cell->size;
				break;
			}
		}
		if (grid)
		{
			CString	tmps;

			for (int kk = 0, jj = 0; jj < xgrid.GetCount(); kk++)
			{
				if (xgrid.Lookup(kk, tmps))
				{
					text += tmps;
					text += '\t';
					jj++;
				}
			}
			xgrid.RemoveAll();
		}
		text += '\n';
	}
	idx += skip;
	form->WriteAll(text);
}

bool CStream::ParseCC(CScreen*& screen, char* datB, int& key, int& idx)
{
	switch (*datB)
	{
	case FCC:
		idx += L_FCC;
		if (!FormCC(screen, key))
			return false;

		ParseFCC(screen, key, (_FCC *)datB);
		break;
	case RCC:
		idx += ParseRCC(screen, datB);
		return true;
	case SCC:
		idx += L_SCC;
		if (!FormCC(screen, key))
			return false;

		ParseSCC(screen, screen->GetAtForm(screen->m_ioR[key]), (_SCC *)datB);
		break;

	default:
		return false;
	}

	return true;
}

bool CStream::FormCC(CScreen*& screen, int& key)
{
	CfmBase* form;

	for ( ; key < screen->m_ioL; key++)
	{
		form = screen->GetAtForm(screen->m_ioR[key]);
		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			CScreen* uob;
			if (m_client->GetAtScreen(uob, form->m_form->keys) && uob->isUob())
			{
				int	idx = 0;
				if (FormCC(uob, idx))
				{
					key    = idx;
					screen = uob;
					return true;
				}
			}
			continue;
		case FM_BUTTON:
			if (form->m_form->iok == EIO_NOP)
				continue;
		case FM_EDIT:
		case FM_COMBO:
		case FM_MEMO:
			if (form->m_form->iok == EIO_INPUT) 
				continue;
		case FM_OUT:
		case FM_TABLE:
			return true;
		case FM_GRID:
			return !(form->m_form->attr2 & GO_TABLE && !form->m_form->size);
		default:
			break;
		}
	}

	return false;
}

void CStream::ParseFCC(CScreen* screen, int key, _FCC* fcc, int col, int row)
{
	int	cc;
	CfmBase* form = screen->GetAtForm(screen->m_ioR[key]);

	cc = (fcc->scc ^ fcc->rcc) & fcc->scc;
	if (cc & CC_SET)
		m_client->SetCPos(screen, screen->m_ioR[key], false);
	SetCC(form, cc, true, col, row);

	cc = (fcc->scc ^ fcc->rcc) & fcc->rcc;
	SetCC(form, cc, false, col, row);
}

void CStream::SetCC(CfmBase* form, int cc, bool set, int col, int row)
{
	DWORD	attr = 0;

	if (cc & CC_PRO)
		attr |= FA_PROTECT;
	if (cc & CC_MAND)
		attr |= FA_MAND;
	if (cc & CC_SEND)
		attr |= FA_SEND;
	if (cc & CC_SET && set)
		attr |= FA_REF;

	form->SetAttributes(attr, set, col, row);

	if (cc & CC_VIS)
	{
		switch (form->m_form->kind)
		{
		case FM_TABLE:
		case FM_GRID:
			if (row >= 0 && col >= 0)
				break;
		default:
			form->SetVisible(set, col);
			break;
		}
	}

	if (cc & CC_ENB)
	{
		switch (form->m_form->kind)
		{
		case FM_GRID:
			if (++row <= 0 || ++col <= 0)
				break;

			((CfmGrid *)form)->SetCellEnable(row, col, set);
			break;
		default:
			form->SetEnable(set);
			break;
		}
	}
}

void CStream::ParseSCC(CScreen* screen, CfmBase* form, _SCC* scc, int col, int row)
{
	if (form->m_form->kind == FM_GRID)
	{
		((CfmGrid *)form)->SetCellColumn(col+1);
		((CfmGrid *)form)->SetCellRow(row+1);
	}

	if (scc->back != SC_NOP)
		form->SetBkColor(scc->back);
	if (scc->text != SC_NOP)
		form->SetFgColor(scc->text);
}

int CStream::ParseRCC(CScreen* screen, char* datB)
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

	int	idx, key, skey;
	CString	text, path, tmps;
	CString	group, item;

	CfmBase*	form;
	if (!screen->FindForm(CString(rcc->name, indx), form))
	{
		for (indx = 0; indx < rcc->ccl; indx++)
		{
			skipL += (int)strlen(datB);
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
			form->WriteAll("");
			break;
		}

		datB = &datB[L_RCC];

		text = datB;
		text += '\t';

		for (indx = 0; indx < rcc->ccl; indx++)
		{
			text += datB;
			text += '\t';

			skipL += (int)strlen(datB);
			datB = &datB[strlen(datB)];
			datB++; skipL++;
		}

		form->WriteAll(text);
		break;
	case csTREE:
		if (rcc->ccl <= 0)
			break;

		datB = &datB[L_RCC];
		path.Format("%s\\%s\\%s", m_guard->m_root, USRDIR, form->GetSymbolName());
		DeleteFile(path);

		key = 1;
		for (indx = 0; indx < rcc->ccl; indx++)
		{
			text = datB;
			idx = text.Find('\t');
			if (idx == -1)
				return skipL;
			group = text.Left(idx++);
			text = text.Mid(idx);
			group.TrimRight();

			if (!group.IsEmpty())
			{
				tmps.Format("%02d", key++);
				WritePrivateProfileString(USR_ROOT, tmps, group, path);
			}

			skey = 1;
			while (true)
			{
				idx = text.Find('\t');
				if (idx == -1)
					break;
				item = text.Left(idx++);
				text = text.Mid(idx);

				if (group.IsEmpty())
				{
					tmps.Format("%02d", key++);
					WritePrivateProfileString(USR_ROOT, tmps, item, path);
				}
				else
				{
					tmps.Format("%02d", skey++);
					WritePrivateProfileString(group, tmps, item, path);
				}
			}
			skipL += (int)strlen(datB);
			datB = &datB[strlen(datB)];
			datB++; skipL++;
		}

		form->SetEnum("", 0);
		break;
	default:
		break;
	}
	return skipL;
}
