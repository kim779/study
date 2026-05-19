// Stream.cpp: implementation of the CStream class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "Wizard.h"
#include "Stream.h"
#include "Client.h"
#include "Script.h"
#include "xscreen.h"
#include "../h/axisrsm.h"
#include "../h/axiserr.h"
#include "../dll/form/amgrid.h"
#include "../dll/form/amradio.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static char nGubn = 13;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStream::CStream(CClient* client)
{
	m_client = client;
	m_guard  = client->m_guard;
	m_lock   = false;
	clear();
	m_Deque.clear();
}

CStream::~CStream()
{
	m_Deque.clear();
	clear();
}

bool CStream::clear()
{
	m_lock   = false;
	
	m_MScreen.clear();

	return true;
}

bool CStream::DataClear(CString sTrName)
{
	return true;
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


bool CStream::InStream(CString sTr, CScreen* screen, bool byKey)
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
	MakeStream(sTr, screen);

	if (m_sndL > 0 && m_guard->RouteTR(m_client))
	{
		m_lock = false;
		m_client->WaitState(screen, true);
		return true;
	}

	// 
	string strservice = string(sTr);
	deque<string>:: iterator it = find(m_Deque.begin(), m_Deque.end(), strservice);
	if (it == m_Deque.end())
	{
		m_Deque.push_back(strservice);
		m_MScreen[strservice] = screen;
	}

	m_lock = false;
	return false;
}

void CStream::OutStream(struct _axisH* axisH, char* datB, int datL)
{
	struct _auxH*	auxH;
	CString	guide = _T("");
	CString sTrace = CString(datB, datL);

//	Msg("%s", sTrace);
//	CString sOut = FORMAT("[OUTPUT] %s", sTrace);
//	AfxGetMainWnd()->SendMessage(WM_USER+ 700, MAKEWPARAM(50, sTrace.GetLength()), (LPARAM)sTrace.operator LPCSTR());

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
	CamBase* form;
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
		
		m_guard->SetCode(screen);
		wait = false;
		screen->m_state &= ~waitSN;
		screen->DominoForm(false);
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


	if (m_Deque.size() > 0)
	{
		string sservice = m_Deque[0];
		class CScreen * pScreen;
		pScreen = m_MScreen[sservice];

		m_Deque.pop_front();
		m_MScreen.erase(sservice);

		if (pScreen == NULL)
			return;
		
		InStream(sservice.data(), pScreen);
	}

}

bool CStream::SetGuide(CScreen* screen, CString guide, COLORREF rgb)
{
	CamBase* form = NULL;

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

	if (screen->m_mapH->options & OP_LEDGER)
	{
		datH = screen->m_ledgerL;
		FillMemory(sysB, datH, ' ');
		screen->GetLedger(sysB);
	}
	sysL += datH;
}

int CStream::GetDataNRM(CScreen* screen, char* iosB, bool tab, bool skip)
{
	CamBase*	form;
	struct _cellR	*cell;
	vector <struct _cellR*>* pCell;
	CString		text;
	vector<string> vInput, vBuffer, vList, vv, vOut;
	vector<string> vOutSymbol;

	int	idx, rowN, nCols;
	int	formL;
	int	iosL = 0;

	if (!skip)
		GetDataH(screen, iosB, iosL);

	Msg("");
	Msg("GetDataH %d", iosL);
	Msg("");

	screen->m_MLEN.clear();
	screen->m_vOUT.clear();
	
	split(vBuffer, screen->m_vINFO[5], is_any_of(""), token_compress_on);
	split(vList, vBuffer[0], is_any_of(""), token_compress_on);

	for each(string s in vList)
	{
		split(vv, s, is_any_of("="), token_compress_on);
		vInput.push_back(vv[0]);
	}

	vList.clear(); vv.clear();
	split(vList, vBuffer[1], is_any_of(""), token_compress_on);

	bool bflag = false;
	for each(string s in vList)
	{
		split(vv, s, is_any_of("="), token_compress_on);
		
		if (vv.size() < 2)
			continue;

vOut.push_back(vv[0]);
		if (vv[0].find("$") == -1)
		{
			screen->m_vOUT.push_back(vv[0]);
			bflag = false;
		}
		else 
		{
			if (!bflag)
			{
				screen->m_vOUT.push_back(vv[0]);
				bflag = true;
			}
		}
	}
		
	vv.clear(); vBuffer.clear();
	split(vBuffer, screen->m_vINFO[4], is_any_of(","), token_compress_on);

	int jj = 0;
	screen->m_vGridLen.clear();
	for each (string s in vBuffer)
	{
		if (s.find("$") != -1)
		{
			int n = 0;
			split(vv, s, is_any_of(":"), token_compress_off);
			for (int kk = 1; kk < (int)vv.size(); kk += 3)
			{
				if ((int)vv.size() > kk)
				{	
					n += atoi(vv[kk].data());
					//if (kk != 1)
					screen->m_vGridLen.push_back(atoi(vv[kk].data()));
				}
			}

			if ((int)screen->m_vOUT.size() > jj)
			{				
				trim(screen->m_vOUT[jj]);
				screen->m_MLEN[screen->m_vOUT[jj]] = FORMAT("%d", n);
			}
		}
		if (s.find("@") != -1)
		{
			int n = 0;
			split(vv, s, is_any_of(":"), token_compress_off);
			for (int kk = 1; kk < (int)vv.size(); kk += 4)
			{
				if ((int)vv.size() > kk)
				{	
					n += atoi(vv[kk].data());
					//if (kk != 1)
					screen->m_vGridLen.push_back(atoi(vv[kk].data()));
				}
			}

			if ((int)screen->m_vOUT.size() > jj)
			{				
				trim(screen->m_vOUT[jj]);
				screen->m_MLEN[screen->m_vOUT[jj]] = FORMAT("%d", n);
			}
		}
		else
		{

			split(vv, s, is_any_of(":"), token_compress_off);			
			if (vv.size() > 1)
			{
				if ((int)screen->m_vOUT.size() > jj)
				{
					trim(screen->m_vOUT[jj]);
					screen->m_MLEN[screen->m_vOUT[jj]] = vv[2];
				}
			}
		}
		jj++;
	}
	vv.clear();

	CString sName;
	
	for each(string sin in vInput)
	{
		screen->FindForm(CSTRING(sin), form);
		sName = CSTRING(sin);

		if (form == NULL)
			continue;

		bool bflag = false;
		for each(string s in vInput)
		{
			if (s.compare(sName) == 0)
			{
				form->m_form->iok = EIO_INPUT;
				bflag = true;
				break;
			}
		}

		if (!bflag)
			continue;
		
		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			CScreen* uob;
			if (m_client->GetAtScreen(uob, form->m_form->keys) && uob->isUob())
				iosL += GetDataNRM(uob, &iosB[iosL], tab, true);
			continue;
		/*case FM_MEMO:
			if (form->m_form->iok != EIO_OUTPUT)
			{
				form->ReadAll(text);
				formL = text.GetLength();

				if (!tab)
				{
					wsprintf(&iosB[iosL], "%05d", formL);
					iosL += L_FILEDs;
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
			continue;*/

		case FM_EDIT:
			if (form->m_form->attr & FA_SKIP)
				continue;
			form->ReadData(text);
			text = CString((char *)text.operator LPCTSTR(), text.GetLength());
			if (/*form->m_form->attr & FA_ENUM &&*/ form->m_form->attr & FA_PASSWD)
			{
				CString	string = _T("");

				if (form->m_form->keys != NOVALUE)
				{
					CamBase* base;
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
		case FM_CHECK:
		case FM_RADIO:
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
		case FM_GRIDEX:
			if (form->m_form->row == 0 || form->m_form->col == 0)
				continue;

			if (form->m_form->attr2 & GO_HEADER)
			{
				form->GetEnum(text, formL, 99);
				CopyMemory(&iosB[iosL], text, text.GetLength());
				iosL += text.GetLength();
			}

			nCols = form->m_form->col;		//HIWORD(form->m_form->vals[1]);
			rowN = ((CamGrid *)form)->GetValidRows();
			if (rowN <= 0)
				continue;

			bool	edit;

			edit = false;
			for (idx = 0; idx < rowN; idx++)
			{
				pCell = screen->m_MGRID[form->m_form->name];
			
//				cell = screen->m_vCELL[0];	//(struct _precellR *) &screen->m_cellR[form->m_form->vals[0]];
/*				
				if (form->m_form->attr2 & GO_CUD)
				{
					form->ReadData(text, false, 0, idx);
					if (text.GetLength() && text.GetAt(0) == ' ')
						continue;
				}
*/
				for (int kk = 0; kk < nCols; kk++/*, cell++*/)
				{
					cell = (*pCell)[kk];

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
//		case FM_SHEET:
			if (form->m_form->attr & FA_SKIP || !form->m_form->size)
				continue;
			break;
/*
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
*/
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
					iosB[iosL++] = /*(form->m_form->attr & FA_ENUM) ? '\r' :*/ '\t';
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

	CMapStringToString gridN;
	gridN.RemoveAll();
	for each (string sout in vOut)
	{
		CString str, smap;

		str = CSTRING(sout);
		screen->FindForm(str, form);

		if (form == NULL)
		{
			vector<string> vs;
			split(vs, sout, is_any_of("."), token_compress_off);
			str = CSTRING(vs[0]);
			screen->FindForm(str, form);

			if (form == NULL)
			{
				str.Remove('$');
				screen->FindForm(str, form);

				if (form == NULL)
					continue;
			}
		}

		switch (form->m_form->kind)
		{
		case FM_GRID:
		case FM_GRIDEX:
			if (gridN.Lookup(str, smap))
				continue;

			gridN.SetAt(str, str);
			if (form->m_form->iok != EIO_OUTPUT)
				continue;

			if (form->m_form->row == 0 || form->m_form->col == 0)
				continue;

			if (form->m_form->attr2 & GO_HEADER)
			{
				form->GetEnum(text, formL, 99);
				CopyMemory(&iosB[iosL], text, text.GetLength());
				iosL += text.GetLength();
			}
			break;

		default:
			break;
		}
	}
	return iosL;
}

int CStream::GetDataOOP(CScreen* screen, char* iosB, bool skip)
{
	screen->m_vOUT.clear();
	screen->m_MTR.clear();
	CString sGrid;
	CamBase*	form;
	struct _cellR	*cell;
	vector <struct _cellR*>* pCell;
	vector<string> vInput, vBuffer, vList, vv, vOut, vfor;
	vector<string> vOutSymbol;
	map<string, string> mMsymbol;

	split(vBuffer, screen->m_vINFO[5], is_any_of(""), token_compress_on);
	split(vList, vBuffer[0], is_any_of(""), token_compress_on);

	for each(string s in vList)
	{
		if (s.empty())	
			continue;

		split(vv, s, is_any_of("="), token_compress_on);
		trim(vv[0]);
		trim(vv[1]);
		vInput.push_back(vv[0]);
		vfor.push_back(vv[0]);

		// Grid ColumnÀÌ BothÀÏ °æ¿ì Áßº¹ Key·Î ¼¼ÆÃÀÌ ¾ÈµÊ - ¼öÁ¤
		int idx = vv[0].find("GRID");
		if (idx  > -1)
			mMsymbol[vv[0]+"_IN"] = vv[1];
		else
			mMsymbol[vv[0]] = vv[1];
	}
	vList.clear(); vv.clear();
	split(vList, vBuffer[1], is_any_of(""), token_compress_on);

	for each(string s in vList)
	{
		if (s.empty())
			continue;

		split(vv, s, is_any_of("="), token_compress_on);
		trim(vv[0]);
		trim(vv[1]);
		vOutSymbol.push_back(vv[1]);
		vOut.push_back(vv[0]);
		vfor.push_back(vv[0]);
		mMsymbol[vv[0]] = vv[1];	
	}

	int	idx, nRow, nRows, vRows, nCols;
	int	iosL = 0;

	char	*symN;
	CString	text;
	int	formL,   symL;

	if (!skip)
		GetDataH(screen, iosB, iosL);

//	for (int ii = 0; ii < screen->m_ioL; ii++)	
	for each(string si in vfor)
	{
		//form = screen->GetAtForm(screen->m_ioR[ii]);	
		screen->FindForm(CSTRING(si), form);

		if (form == NULL)
		{
			vector<string> vs;
			split(vs, si, is_any_of("."), token_compress_off);
			si = vs[0];	
			screen->FindForm(CSTRING(si), form);

			if (form == NULL)
			{
				CString str;
				str = CSTRING(si);
				str.Remove('$');
				screen->FindForm(str, form);

				if (form == NULL)
					continue;
			}
		}

		symN = (char *)form->GetSymbolName().operator LPCSTR();
		symL = (int)strlen(symN);

		bool bflag = false;
		for each(string s in vInput)
		{
			if (s.compare(symN) == 0)
			{
				form->m_form->iok = EIO_INPUT;
				bflag = true;
			}
		}

		CString sSymbol, sym;
		int jj = 0;
		sym = CString(symN);
		for each(string s in vOut)
		{
			if (s.compare(symN) == 0)
			{
				form->m_symbol = vOutSymbol[jj];
				form->m_form->iok = EIO_OUTPUT;
				bflag = true;
				screen->m_vOUT.push_back(s);
			}
			else if (s.find(".") != -1 && s.find("$") == -1) //(form->m_form->kind == FM_TABLE)
			{
				CString sTable = CSTRING(s);
				sTable = sTable.Left(sTable.Find("."));

	//			find(string(sTable), m_vOUT);
			

				if (sTable.CompareNoCase(symN) == 0)
				{
					pCell = screen->m_MTABLE[form->m_form->name];
					nRows = form->m_form->row;		//LOWORD(form->m_form->vals[1]);
					for (nRow = 0; nRow < nRows; nRow++)
					{
						if ((*pCell)[nRow]->attr & FA_SKIP)
							continue;

						sSymbol.Format("%s.%s", sTable, form->GetSymbolName(nRow));	
						vector<string>::iterator it = find(screen->m_vOUT.begin(), screen->m_vOUT.end(), string(sSymbol));
						if (it != screen->m_vOUT.end())
						{
							continue;
						}


						if (s.compare(sSymbol) == 0)	
						{
					//		form->m_symbol = vOutSymbol[jj];
							form->m_form->iok = EIO_OUTPUT;
							bflag = true;
							screen->m_vOUT.push_back(string(sSymbol));
							screen->m_MTR[string(sSymbol)] = form;
						}
					}
				}
			}
			else if (form->m_form->kind == FM_GRID || form->m_form->kind == FM_GRIDEX)
			{
				form->m_form->iok = EIO_OUTPUT;
				bflag = true;

				CString sOut = CString(symN);
				if (s.find("$") != -1)
				{
					CString sg = CSTRING(s);
					sg = sg.Left(sg.Find("."));

					CString osymbol = CSTRING(vOutSymbol[jj]);	
					osymbol = osymbol.Left(osymbol.Find("."));

					bool bbflag = false;
					for each (string ss in screen->m_vOUT)
					{
						if (ss.compare(string(sOut)) == 0)
						{
							bbflag = true;
							break;
						}
					}

					if (bbflag == false)
					{
						mMsymbol[string(sg)] = osymbol;
					}
				}		
			}
			jj++;
		}
		

		if (!bflag)
			continue;

		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			CScreen* uob;
			if (m_client->GetAtScreen(uob, form->m_form->keys) && uob->isUob())
				iosL += GetDataOOP(uob, &iosB[iosL], true);
			continue;
		//case FM_MEMO:
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
				iosL += L_FILEDs;

				if (iosL + formL > maxIOs)
				{
					m_guard->SetGuide(AE_MAXIO, m_client->m_key);
					return 0;
				}
				CopyMemory(&iosB[iosL], (char *)text.operator LPCTSTR(), formL);
				iosL += formL;
				iosB[iosL++] = nGubn;	//'\t';

				if (form->m_form->iok == EIO_INPUT)
					break;
			case EIO_OUTPUT:
				CopyMemory(&iosB[iosL], symN, symL); iosL += symL;
				iosB[iosL++] = nGubn;	//'\t';
				break;
			}

			continue;
		case FM_EDIT:
			if (form->m_form->attr & FA_SKIP)
				continue;
			break;

			// ÀÏ´Ü Á¦°Å....
			//switch (form->m_form->iok)
			//{
			//case EIO_INPUT:
			//case EIO_INOUT:
			//	form->ReadData(text);
			//	if (/*form->m_form->attr & FA_ENUM && */form->m_form->attr & FA_PASSWD)
			//	{
			//		CString	string = _T("");

			//		if (form->m_form->keys != NOVALUE)
			//		{
			//			CamBase* base;
			//			if (screen->FindForm(&screen->m_strR[form->m_form->keys], base))
			//			{
			//				base->ReadData(string);
			//				string = CString((char *)string.operator LPCTSTR(), string.GetLength());
			//			}
			//		}
			//		text = m_guard->Secure(text, string);
			//	}
			//	formL = text.GetLength();
			//	CopyMemory(&iosB[iosL], symN, symL); iosL += symL;
			//	iosB[iosL++] = FS;
			//	CopyMemory(&iosB[iosL], text, formL); iosL += formL;
			//	iosB[iosL++] = '\t';

			//	if (form->m_form->iok == EIO_INPUT)
			//		break;
			//case EIO_OUTPUT:
			//	CopyMemory(&iosB[iosL], symN, symL); iosL += symL;
			//	iosB[iosL++] = '\t';
			//	break;
			//}
			//continue;

		case FM_OUT:
			if (form->m_form->attr & FA_SKIP)
				continue;
			form->m_form->iok = EIO_OUTPUT;
			break;
		case FM_CHECK:
		case FM_RADIO:
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
			//if (form->m_form->vals[0] == NOVALUE)
			if (form->m_form->row == 0 || form->m_form->col == 0)
				continue;
		
		
			text.Empty();
			//cell = screen->m_vTCELL[0];	//(struct _precellR *) &screen->m_cellR[form->m_form->vals[0]];
			
			pCell = screen->m_MTABLE[form->m_form->name];
			nRows = form->m_form->row;		//LOWORD(form->m_form->vals[1]);
			for (nRow = 0; nRow < nRows; nRow++)
			{
				if ((*pCell)[nRow]->attr & FA_SKIP)
					continue;
				
				CString ss = FORMAT("%s.%s", form->m_form->name,  form->GetSymbolName(nRow));
				//string ss = form->GetSymbolName(nRow);
			
				bool bflag = false;
				for each (string s in screen->m_vOUT)
				{
					if (s.compare(string(ss)) == 0)	
						bflag = true;
				}
				
				if (bflag)
				{
					text += mMsymbol[string(ss)].data(); 	//cell[nRow].name);
					text += nGubn;	//'\t';
				}
			}
			CopyMemory(&iosB[iosL], text, text.GetLength());
			iosL += text.GetLength();
			continue;

		case FM_GRID:
		case FM_GRIDEX:
			//if (form->m_form->vals[0] == NOVALUE)
			{
				if (form->m_form->row == 0 || form->m_form->col == 0)
					continue;

				vector<string>::iterator it = find(screen->m_vOUT.begin(), screen->m_vOUT.end(), string(symN));
				if (it != screen->m_vOUT.end())
					continue;

				screen->m_vOUT.push_back(string(symN));	
				sGrid = FORMAT("$%s",symN);
				iosB[iosL++] = '$';
				symL = mMsymbol[string(sGrid)].size();
				CopyMemory(&iosB[iosL], mMsymbol[string(sGrid)].c_str(), symL); iosL += symL;
				iosB[iosL++] = FS;

				//////////////////////////////////////////////////////////
				/// trlayout 
				//////////////////////////////////////////////////////////
	//			pOrder->push_back(string(symN));
				screen->m_MTR[string(symN)] = form;

				text.Empty();
				form->GetEnum(text, formL, 99);
				CopyMemory(&iosB[iosL], text, text.GetLength());
				iosL += text.GetLength();

				text.Empty();
				CPtrArray*	grid;

				text = form->GetSymbolName();
				text.Empty();

				nCols = form->m_form->col;	//HIWORD(form->m_form->vals[1]);
				if (screen->m_cells.Lookup(form->GetSymbolName(), (CObject *&)grid))
					nCols = (int)grid->GetSize();
				else
					grid = NULL;

				//cell = screen->m_vCELL[0];	//(struct _precellR *) &screen->m_cellR[form->m_form->vals[0]];
			
				pCell = screen->m_MGRID[form->m_form->name];

				vRows = ((CamGrid *)form)->GetValidRows();

				//////////////////////////////////////////////////////
				CString colsymbol;
				// Tr_Layout ¼ø¼­ º¯°æ				
				string sTrlayout;
				vector<string> sTMP, sTRNAME, sTRORDER, sTRINORDER;
				map <string, string> ::iterator pIter;
				pIter = screen->m_LINKINFO.begin();

				// sTrlayout : Grid TR Layour Á¤º¸
				for (pIter = screen->m_LINKINFO.begin(); pIter != screen->m_LINKINFO.end(); pIter++)
				{
					sTrlayout = pIter -> second;
					if (sTrlayout.find(form->m_form->name))
						break;					
				}	
				int cnt = Split(sTMP, sTrlayout, 0x0e);		// Input / Output ºÐ¸®
				int inCnt = Split(sTRINORDER, sTMP[0], 0x0c);	// input ºÐ¸®
				int outCnt = Split(sTRORDER, sTMP[cnt-1], 0x0c);	// Output ºÐ¸®				
				/////////////////////////////////////////////////////////////
				
				// InputÀÏ°æ¿ì ¸ÕÀú ¼¼ÆÃ
				for (idx = 0; idx < nCols; idx++/*, cell++*/)
				{
					if (grid != NULL)
						cell = (struct _cellR *)grid->GetAt(idx);
					else 
						cell = (*pCell)[idx];

					if (idx >= form->m_form->type && !(cell->properties & PR_VISIBLE))
						continue;
					else if (/*cell->iok == EIO_NOP || */cell->attr & FA_SKIP)
						continue;

					// Input Symbol
					if (cell->iok == EIO_OUTPUT || cell->iok == EIO_NOP)
						continue;

					colsymbol.Format("$%s.%s_IN", form->GetSymbolName(), cell->name);
					colsymbol = CSTRING(mMsymbol[string(colsymbol)]);
					colsymbol = colsymbol.Mid(colsymbol.Find(".") + 1, colsymbol.GetLength());
					symL = colsymbol.GetLength();
					
					// Input Symbol
					if (!colsymbol.IsEmpty())
					{
						CopyMemory(&iosB[iosL], colsymbol.GetBuffer(0), symL);
						iosL += symL;
					}

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
						iosB[iosL++] = '\n';
						break;						
					}
				}
				
				// OutputÀÏ°æ¿ì ¸¶Áö¸· ÁÖ·ç·è ¼¼ÆÃ
				for (idx = 0; idx < nCols; idx++/*, cell++*/)
				{
					if (grid != NULL)
						cell = (struct _cellR *)grid->GetAt(idx);
					else 
						cell = (*pCell)[idx];


					if (idx >= form->m_form->type && !(cell->properties & PR_VISIBLE))
						continue;
					else if (/*cell->iok == EIO_NOP || */cell->attr & FA_SKIP)
						continue;

					if (cell->iok == EIO_INPUT)
						continue;

					// ±×¸®µå ÄÃ·³ ¼øÂ÷Àû ¼¼ÆÃ					
					/*symN = (char *)form->GetSymbolName(idx).operator LPCSTR();
					symL = (int)strlen(symN);
					CString colsymbol = FORMAT("%s.%s", sGrid, symN);
					colsymbol = CSTRING(mMsymbol[string(colsymbol)]);
					colsymbol = colsymbol.Mid(colsymbol.Find(".") + 1, colsymbol.GetLength());
					symL = colsymbol.GetLength();*/
					// SetOrderµî ¼øÂ÷ÀûÀÌÁö ¾ÊÀ» °æ¿ì ¶§¹®¿¡ º¯°æ
					colsymbol = sTRORDER[idx].data();
					colsymbol = colsymbol.Left(colsymbol.Find("="));
					colsymbol = CSTRING(mMsymbol[string(colsymbol)]);
					colsymbol = colsymbol.Mid(colsymbol.Find(".") + 1, colsymbol.GetLength());
					symL = colsymbol.GetLength();

					if (!colsymbol.IsEmpty())
					{
						CopyMemory(&iosB[iosL], colsymbol.GetBuffer(0), symL);
						iosL += symL;

						iosB[iosL++] = '\n';
					}					
				}
				iosB[iosL++] = nGubn;	//'\t';
			}
			continue;

		case FM_COMBO:
//		case FM_SHEET:
			if (form->m_form->attr & FA_SKIP)
				continue;
			break;
/*
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
*/
		case FM_CONTROL:
			if (form->m_form->attr & FA_SKIP)
				continue;
			/*
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
			*/
			break;
		default:
			continue;
		}
		string sSym;
		sSym = mMsymbol[string(symN)];
		symL = sSym.length();
		switch (form->m_form->iok)
		{
		case EIO_INPUT:
		case EIO_INOUT:
			form->ReadData(text);
			text.Trim();			
			formL = text.GetLength();
			
			CopyMemory(&iosB[iosL], sSym.data(), symL); iosL += symL;
			iosB[iosL++] = FS;
			CopyMemory(&iosB[iosL], text, formL); iosL += formL;
			iosB[iosL++] = nGubn;	//'\t';

			if (form->m_form->iok == EIO_INPUT)
				break;
		case EIO_OUTPUT:
			{
				CopyMemory(&iosB[iosL], sSym.data(), symL); iosL += symL;
				iosB[iosL++] = nGubn;	//'\t';
			}
			break;
		}
		screen->m_MTR[string(symN)] = form;		
	}

	return iosL;
}

int CStream::GetDataOOP2(CScreen* screen, char* iosB, bool skip)
{
	CamBase*	form;
	struct _cellR	*cell;
	vector <struct _cellR*>* pCell;

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
		//case FM_MEMO:
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
				iosL += L_FILEDs;

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
				if (/*form->m_form->attr & FA_ENUM &&*/ form->m_form->attr & FA_PASSWD)
				{
					CString	string = _T("");

					if (form->m_form->keys != NOVALUE)
					{
						CamBase* base;
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
		case FM_RADIO:
		case FM_CHECK:
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
			//if (form->m_form->vals[0] == NOVALUE)
			if (form->m_form->row == 0 || form->m_form->col == 0)
				continue;

			text.Empty();
			
			//cell = screen->m_vTCELL[0];		//(struct _precellR *) &screen->m_cellR[form->m_form->vals[0]];
			pCell = screen->m_MTABLE[form->m_form->name];

			nRows = form->m_form->row;	//LOWORD(form->m_form->vals[1]);
			for (nRow = 0; nRow < nRows; nRow++)
			{
				if ((*pCell)[nRow]->attr & FA_SKIP)
					continue;

				text += form->GetSymbolName(nRow);		//cell[nRow].name);
				text += '\t';
			}
			CopyMemory(&iosB[iosL], text, text.GetLength());
			iosL += text.GetLength();
			continue;

		case FM_GRID:
		case FM_GRIDEX:
			if (form->m_form->row == 0 || form->m_form->col == 0)
				continue;

			CopyMemory(&iosB[iosL], symN, symL); iosL += symL;
			iosB[iosL++] = '$';

			text.Empty();
			form->GetEnum(text, formL, 99);
			CopyMemory(&iosB[iosL], text, text.GetLength());
			iosL += text.GetLength();

			text.Empty();
			CPtrArray*	grid;

			nCols = form->m_form->col;	//HIWORD(form->m_form->vals[1]);
			if (screen->m_cells.Lookup(form->GetSymbolName(), (CObject *&)grid))
				nCols = (int)grid->GetSize();
			else
				grid = NULL;

			vRows = ((CamGrid *)form)->GetValidRows();

			int	kk;
			formL = 0;
			for (kk = -1; kk < vRows; kk++)
			{
				inbs.Empty();
				//cell = (struct _cellR *) &screen->m_cellR[form->m_form->vals[0]];
				//cell = screen->m_vCELL[0];
				pCell = screen->m_MGRID[form->m_form->name];
				for (idx = 0; idx < nCols; idx++, cell++)
				{
					cell = (*pCell)[idx];

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
							text += form->GetSymbolName(nCols);//cell->name);
							text += '\t';
							continue;
						}
						break;
					case EIO_OUTPUT:
						if (kk == -1)
						{
							formL++;
							text += '2';
							text += form->GetSymbolName(nCols);	//cell->name);
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
//		case FM_SHEET:
			if (form->m_form->attr & FA_SKIP)
				continue;
			break;
/*
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
*/
		case FM_CONTROL:
			if (form->m_form->attr & FA_SKIP)
				continue;
			/*
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
			*/
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
		
		MakeStream(screen->m_mapH->trxC, screen);
	}
}

void CStream::MakeStream(CString sTr, CScreen* screen)
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

	//********************************************************************************************************
	//********************************************************************************************************
	// Here My Work 
//	CString sTrace = "Input";
//	::AfxGetMainWnd()->SendMessage(WM_USER+700, MAKEWPARAM(WM_TRACE , sTrace.GetLength()), (LPARAM)sTrace.operator LPCTSTR());

	screen->m_vINFO.clear();
	clear();
	Msg("");
	Msg("%s", sTr);
	sTr.MakeUpper();
	
	string sInfo = screen->m_MTRINFO[string(sTr)];
	string sLink = screen->m_LINKINFO[string(sTr)];
	CString ss = CSTRING(sLink);
	if (sTr.IsEmpty() || sInfo.empty() || sLink.empty())
		return;
	
	vector <string> vBuffer, vinfo;
	split(vinfo, sInfo, is_any_of("\n"), token_compress_on);
	string sIn, sOut;

	int ii = 0;
	for each (string s in vinfo)
	{
		if (ii < 3)
		{
			split(vBuffer, s, is_any_of("="), token_compress_off);
			if (vBuffer.size() > 1 )
			{
				if (!vBuffer[1].empty())
					screen->m_vINFO.push_back(vBuffer[1]);
				vBuffer.clear();
			}
			
		}
		else if (s.find("IN") != -1)
		{
			split(vBuffer, s, is_any_of("="), token_compress_off);
			if (vBuffer.size() > 1 )
			{
				if (!vBuffer[1].empty())
					sIn.append(vBuffer[1]);	
				vBuffer.clear();
			}
		}
		else if (s.find("OUT") != -1)
		{
			split(vBuffer, s, is_any_of("="), token_compress_off);
			if (vBuffer.size() > 1 )
			{
				if (!vBuffer[1].empty())
					sOut.append(vBuffer[1]);		
				vBuffer.clear();
			}
		}
		ii++;
	}
	screen->m_vINFO.push_back(sIn);
	screen->m_vINFO.push_back(sOut);

	vBuffer.clear();
	CString s = ss.Mid(ss.Find(sTr) + sTr.GetLength() + 1, ss.GetLength());
	screen->m_vINFO.push_back(string(s));
	//*********************************************************************************
	//*********************************************************************************
	bool	tab = false;
	if (screen->m_mapH->options & OP_TABS)
	{
		axisH->msgK = msgK_TAB;
		tab = true;
	}

	if (screen->m_mapH->options & OP_CNV)
		axisH->stat |= statCNV;

	/// TrName Input...	
	CopyMemory(axisH->trxC, sTr.operator LPCSTR(), sTr.GetLength());
	CopyMemory(axisH->svcN, CString(screen->m_mapH->mapN, L_MAPN).Mid(L_SGID, L_SELC), sizeof(axisH->svcN));
	m_sndL += L_axisH;


	string sopt = screen->m_vINFO[2];
	to_upper(sopt);
	trim(sopt);
	
	char *popt[] = { "FID", "STRUCTURE", "TAB", "BINARY" };
	enum		   { eFID, eSTRUCTURE, eTAB, eBINARY };
	for (int ii = 0; ii < _countof(popt) ; ii++)
	{
		if (sopt.compare(popt[ii]) == 0)
		{
			switch (ii)
			{
			case eFID:
				axisH->auxs |= auxsOOP;
				axisL = GetDataOOP(screen, &m_sndB[m_sndL]);
				break;
			case eSTRUCTURE:
				axisL = GetDataNRM(screen, &m_sndB[m_sndL], tab);
				break;
			case eTAB:
				break;
			case eBINARY:
				break;
			default:
				break;
			}
		}
	}
	
/*
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
*/
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

	CString sTrace = CString(m_sndB[ii], m_sndL);	
	for (int ii = 0; ii < m_sndL; ii++)
	{
		TRACE("%c", m_sndB[ii]);
	}
	TRACE("\n");		
//	::AfxGetMainWnd()->SendMessage(WM_USER+700, MAKEWPARAM(WM_TRACE ,m_sndL), LPARAM(&m_sndB));
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

	Msg("%s", CString(axisB, axisL));

	CamBase* form;
	int	formL;	//, row;
	int	nRows, nCols;
	CString	text;
//	bool	isSup;
	struct _cellR*	cell			= NULL;
	vector <struct _cellR*>* pCell	= NULL;

	map <string, string> Mdata;

	formL = 0;
	for (int key = 0; idx < axisL; )
	{
		if (ParseCC(screen, &axisB[idx], key, idx))
			continue;

	//	if (key >= screen->m_ioL)
	//		return idx;

	//	if (key > m_VOUT)

		if ((int)screen->m_vOUT.size() <=  key)
			break;
	
		vector <string> v;
		split(v, screen->m_vOUT[key], is_any_of("."), token_compress_on);
		CString ss = CSTRING(v[0]);
		ss.Remove('$');
		screen->FindForm(ss, form);
		if (form)
		{
			if (form->m_form->kind == FM_GRID || form->m_form->kind == FM_GRIDEX)
			{

				if (form->m_form->row == 0 || form->m_form->col == 0)
					continue;

				if (form->m_form->attr2 & GO_HEADER)
				{
					formL = 0;
					form->GetEnum(text, formL, 98);
					if (formL > axisL - idx)
						return axisL;

					form->SetEnum(CString(&axisB[idx], formL), formL, 99);
					idx += formL;
				}

				nCols = form->m_form->col;		
				if (form->m_form->attr2 & GO_FLEX)
				{
					nRows = atoi(CString(&axisB[idx], L_FLEX));
					idx += L_FLEX;
				}
				else	nRows = form->m_form->row;		

				if (form->m_form->attr2 & GO_TABLE)
					SetTable(screen, key, nCols, nRows, &axisB[idx], axisL, idx);
				else
					SetCells(screen, form, nRows, nCols, axisB, idx, axisL);

				screen->OnProfit(form);
				form->IsChanged();
			}
			else
			{
				CString ss = screen->m_vOUT[key].data(); 
				TRIM(ss);
				formL = atoi(screen->m_MLEN[string(ss)].data());
				
				//if (formL > strlen(axisB))
				//	formL = strlen(axisB);
				int len = strlen(axisB);

				//Msg("%s", CString(&axisB[idx], formL));
				Msg("%s", CString(&axisB[idx], len));
				//Mdata[screen->m_vOUT[key]] = CString(&axisB[idx], formL);
				Mdata[screen->m_vOUT[key]] = CString(&axisB[idx], len);
				idx += formL;
			}
			key++;
		}
		else
		{
			CString ss = screen->m_vOUT[key].data(); 
			TRIM(ss);
			formL = atoi(screen->m_MLEN[string(ss)].data());
			Msg("%s", CString(&axisB[idx], formL));
			Mdata[screen->m_vOUT[key]] = CString(&axisB[idx], formL);
			idx += formL;
			key++;
		}
	}
	

	int ii = 0;
	for each (string s in screen->m_vOUT)
	{
		screen->FindForm(CSTRING(s), form);
		if (form)
		{
			form->m_form->iok = EIO_OUTPUT;			
			form->m_form->size = atoi(screen->m_MLEN[screen->m_vOUT[ii++]].data());
			form->WriteData(CSTRING(Mdata[s]));
		}
		else 
		{
			CString sName;
			int row = 0;
			for (int kk = 0; kk < screen->m_ioL; kk++)
			{
				form = screen->GetAtForm(screen->m_ioR[kk]);
				switch (form->m_form->kind)
				{
				case FM_TABLE:
					{
			//			if (form->m_form->iok == EIO_NOP)
			//				break;
						if (form->m_form->row == 0 || form->m_form->col == 0)
							continue;

						pCell = screen->m_MTABLE[form->m_form->name];
						nRows = form->m_form->row;	
						for (row = 0; row < nRows; row++)
						{
							if ((*pCell)[row]->attr & FA_SKIP)
								continue;
							
							sName.Format("%s.%s", form->m_form->name, CString((*pCell)[row]->name));							
							if (s.compare(sName) == 0)
							{
								(*pCell)[row]->size = atoi(screen->m_MLEN[screen->m_vOUT[ii]].data());
								form->WriteData(Mdata[s].data(), true, 2, row);
								form->SetDomino(false);
								//form->WriteAll(CSTRING(Mdata[s]));
							}						
						}
					}
					break;
				default:
					break;
				}
			}
		}
	}





	/*
	for (int key = 0; idx < axisL; )
	{
		if (ParseCC(screen, &axisB[idx], key, idx))
			continue;

		if (key >= screen->m_ioL)
			return idx;

		isSup = true;
		form = screen->GetAtForm(screen->m_ioR[key]);
		CString sName;

		int ct = 0;

		bool bflag = false;
		for each(string s in m_vOUT)
		{		
			sName = form->GetSymbolName();
			
			if ( s.compare(string(sName)) == 0)
			{
				bflag = true;
				form->m_form->iok = EIO_OUTPUT;
				form->m_form->size =  atoi(m_MLEN[m_vOUTSYMBOL[ct]].data());
			}
			else if (form->m_form->kind == FM_TABLE)
			{
				pCell = screen->m_MTABLE[form->m_form->name];
				nRows = form->m_form->row;	//LOWORD(form->m_form->vals[1]);
				for (row = 0; row < nRows && idx < axisL; row++)
				{
					if ((*pCell)[row]->attr & FA_SKIP)
						continue;

					sName = CString((*pCell)[row]->name);
					TRIM(sName);
					if (s.compare(sName) == 0)
					{
						(*pCell)[row]->iok = EIO_OUTPUT;
						(*pCell)[row]->size = atoi(m_MLEN[m_vOUTSYMBOL[ct]].data());
						bflag = true;
					}
				}
			}
			ct++;
		}

		if (!bflag)
		{
			key++;
			continue;
		}
		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			CScreen* uob;
			if (m_client->GetAtScreen(uob, form->m_form->keys) && uob->isUob())
				idx += SetDataNRM(uob, &axisB[idx], axisL - idx, true);
			break;

		case FM_CHECK:
		case FM_RADIO:
		case FM_BUTTON:
			switch (form->m_form->iok)
			{
			case EIO_OUTPUT:
			case EIO_INOUT:
				((CamRadio *)form)->SetChecked(axisB[idx] > '0' ? true : false);
				idx++;
				break;
			default:
				break;
			}
			break;

		case FM_MEMO:
			if (form->m_form->iok == EIO_INPUT)
				break;

			if (L_FILEDs > axisL - idx)
				return axisL;
			formL = atoi(CString(&axisB[idx], L_FILEDs));
			if (formL > axisL - idx)
				return axisL;
			idx += L_FILEDs;

			form->WriteAll(CString(&axisB[idx], formL));
			form->IsChanged();
			idx += formL;
			break;

		case FM_BROWSER:
			if (form->m_form->iok != EIO_OUTPUT)
				break;

			if (L_FILEDs > axisL - idx)
				return axisL;
			formL = atoi(CString(&axisB[idx], L_FILEDs));
			if (formL > axisL - idx)
				return axisL;
			idx += L_FILEDs;

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
			if (form->m_form->row == 0 || form->m_form->col == 0)
				continue;

			if (form->m_form->attr2 & GO_HEADER)
			{
				formL = 0;
				form->GetEnum(text, formL, 98);
				if (formL > axisL - idx)
					return axisL;

				form->SetEnum(CString(&axisB[idx], formL), formL, 99);
				idx += formL;
			}

			nCols = form->m_form->col;		//HIWORD(form->m_form->vals[1]);
			if (form->m_form->attr2 & GO_FLEX)
			{
				nRows = atoi(CString(&axisB[idx], L_FLEX));
				idx += L_FLEX;
			}
			else	nRows = form->m_form->row;		//LOWORD(form->m_form->vals[1]);

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
			if (form->m_form->row == 0 || form->m_form->col == 0)
				continue;

			text.Empty();
			//cell = screen->m_vCELL[0];//struct _precellR *) &screen->m_cellR[form->m_form->vals[0]];
			pCell = screen->m_MTABLE[form->m_form->name];
			nRows = form->m_form->row;	//LOWORD(form->m_form->vals[1]);
			for (row = 0; row < nRows && idx < axisL; row++)
			{
				if ((*pCell)[row]->attr & FA_SKIP)
					continue;

				formL = (*pCell)[row]->size;
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
	*/
	return idx;
}

int CStream::SetDataOOP(CScreen* screen, char* axisB, int axisL, bool skip)
{
	int idx = 0;
	vector <struct _cellR*>* pCell;
	if (!skip && !SetDataH(screen, &axisB[idx], idx))
		return axisL;

	CamBase* form;
	int	pos, index;
//	bool	isSup;

	CString	text, dats;
	text = CString(&axisB[idx], axisL - idx);
	map <string, string> Mdata;

	int ii = 0;
	for (int key = 0; idx < axisL; ii++)
	{
		if (ParseCC(screen, &axisB[idx], key, idx))
			continue;
		
//		if (key >= screen->m_ioL)
//			break;

		if (key  >= (int)screen->m_vOUT.size())
			break;

		screen->FindForm(CSTRING(screen->m_vOUT[key]), form);
		if (form)
		{
			if (form->m_form->kind == FM_GRID || form->m_form->kind == FM_GRIDEX)
			{
				if (form->m_form->row == 0 || form->m_form->col == 0 || form->m_form->iok == EIO_INPUT)
				{
					key++;
					continue;
				}
				pos = 0;
				form->GetEnum(dats, pos, 98);
				if (pos > axisL - idx)
					return axisL;

				form->SetEnum(text, pos, 99);

				//pos += 1;
				text = text.Mid(pos);
				if (screen->m_mapH->options & OP_OOP2)
					index = text.Find(gFS);
				else
					index = text.Find(nGubn);	//("\r\t");
			
				if (index == -1 || index == 0)
					pos += text.GetLength();
				else
				{
					text = text.Left(index);
					pos += index + ((screen->m_mapH->options & OP_OOP2) ? 1 : 2);
				}

				form->WriteAll(text);
				screen->OnProfit(form);
				form->IsChanged();

				idx += pos;
				if (axisL < idx)
					break;
				text = CString(&axisB[idx], axisL - idx);
				key++;
			}
			else 
			{
				pos = text.Find(nGubn);		//('\t');
				if (pos != -1)
					text = text.Left(pos++);
				else
					pos = text.GetLength();

				Msg("%s", text);
				Mdata[screen->m_vOUT[key]] = string(text);
				idx += pos;
				
				key++;
				text = CString(&axisB[idx], axisL - idx);
			}
		}
		else 
		{
			
				pos = text.Find(nGubn);		//('\t');
				if (pos != -1)
					text = text.Left(pos++);
				else
					pos = text.GetLength();

				Msg("%s", text);
				Mdata[screen->m_vOUT[key]] = string(text);
				idx += pos;
				
				key++;
				text = CString(&axisB[idx], axisL - idx);
				
		}
	}


	int nRows = 0;
	ii = 0;
	for each (string s in screen->m_vOUT)
	{
		screen->FindForm(CSTRING(s), form);
		if (form)
		{
			if (form->m_form->kind == FM_GRID || form->m_form->kind == FM_GRIDEX)
				continue;

			form->m_form->iok = EIO_OUTPUT;
			form->WriteData(CSTRING(Mdata[s]));
		}
		else 
		{
			CString sName;
			string ssymbol;
			int row = 0;
	//		for (int kk = 0; kk < screen->m_ioL; kk++)
			for each(string s in screen->m_vOUT)
			{
				ssymbol = s.substr(0, s.find(".", 1));
		
		//		form = screen->GetAtForm(screen->m_ioR[kk]);

				screen->FindForm(CSTRING(ssymbol), form);
				switch (form->m_form->kind)
				{
				case FM_TABLE:
					{
						if (form->m_form->iok == EIO_NOP)
							break;
						if (form->m_form->row == 0 || form->m_form->col == 0)
							continue;

						pCell = screen->m_MTABLE[form->m_form->name];
						nRows = form->m_form->row;	
						for (row = 0; row < nRows; row++)
						{
							if ((*pCell)[row]->attr & FA_SKIP)
								continue;

							sName = CString((*pCell)[row]->name);
							sName.Format("%s.%s", form->m_form->name, CString((*pCell)[row]->name));
							if (s.compare(sName) == 0)
							{
								form->WriteData(Mdata[s].data(), true, 2, row);
								form->SetDomino(false);
							}
								//form->WriteAll(CSTRING(Mdata[s]));
						}
					}
					break;
				default:
					break;
				}
			}
		}
	}




	/*

	int ii = 0;
	for (int key = 0; idx < axisL; ii++)
	{
		if (ParseCC(screen, &axisB[idx], key, idx))
			continue;
		if (key >= screen->m_ioL)
			return idx;

		isSup = true;
//		form = screen->GetAtForm(screen->m_ioR[key]);
		form = m_MTR[m_vOUT[key]];
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
		case FM_RADIO:
		case FM_CHECK:
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
			{
				Msg("%s", text);
				form->WriteData(text);
			}
			if (!isSup)	form->IsChanged();

			break;

		case FM_GRID:
			if (form->m_form->row == 0 || form->m_form->col == 0 || form->m_form->iok == EIO_INPUT)
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
			if (form->m_form->iok == EIO_NOP || form->m_form->row == 0 || form->m_form->col == 0)
			{
				key++;
				continue;
			}

			pos = 0;
			dats.Empty();
			//cell = screen->m_vTCELL[0];	//(struct _precellR *) &screen->m_cellR[form->m_form->vals[0]];
			pCell = screen->m_MTABLE[form->m_form->name];

			int	ii;
			for (ii = 0; ii < form->m_form->row; ii++)
			{
				if ((*pCell)[ii]->attr & FA_SKIP)
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
				///*
				//if (form->m_form->attr & FA_ENUM)
				//{
				//	if (screen->m_mapH->options & OP_OOP2)
				//		index = text.Find(gFS);
				//	else
				//		index = text.Find("\r\t");
				//	if (index == -1)
				//		pos += text.GetLength();
				//	else
				//	{
				//		text = text.Left(index);
				//		pos += index + ((screen->m_mapH->options & OP_OOP2) ? 1 : 2);
				//	}
				//}
				//else
				//*/
/*
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
	*/
	screen->m_vOUT.clear();
	screen->m_MTR.clear();
	return idx;
}

int CStream::SetDataTAB(CScreen* screen, char* axisB, int axisL, bool skip)
{
	int	idx = 0;
	vector <struct _cellR*>* pCell;
	if (!skip && (idx >= axisL || !SetDataH(screen, &axisB[idx], idx)))
		return axisL;

	int	pos, index;
	bool	isSup;
//	struct	_cellR*	cell;

	CamBase* form;
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
		//case FM_MEMO:
			isSup = false;
		case FM_BUTTON:
		case FM_CHECK:
		case FM_RADIO:
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

			if (/*form->m_form->kind == FM_MEMO ||*/ form->m_form->kind == FM_BROWSER)
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
			//cell = screen->m_vTCELL[0];		//(struct _precellR *) &screen->m_cellR[form->m_form->vals[0]];
			pCell = screen->m_MTABLE[form->m_form->name];
			int	ii;
			for (ii = 0; ii < form->m_form->row; ii++/*, cell++*/)
			{
				if ((*pCell)[ii]->attr & FA_SKIP)
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
		case FM_GRIDEX:
			if (form->m_form->row == 0 || form->m_form->col == 0 || form->m_form->iok == EIO_INPUT)
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
				pos = text.Find(/*(form->m_form->attr & FA_ENUM) ? '\r' : */'\t');
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

	if (screen->m_mapH->options & OP_LEDGER)
	{
		datH += screen->m_ledgerL;
		screen->SetLedger(datB);
	}
	return true;
}

void CStream::SetCells(CScreen* screen, CamBase* form, int nRows, int nCols, char* axisB, int& idx, int axisL)
{
	int	ii, col, row, formL;
	CMap	<int, int, CString, CString&>	xgrid;

	CString	text;
	struct _cellR*	cell = NULL;
	vector <struct _cellR*>* pCell;
	CPtrArray* grid = NULL;
	int record = screen->m_vGridLen[0];
	
	if (record != 0)
	{
		CString sRecord = CString(&axisB[idx], record);
		idx += record;
	
		record = atoi(sRecord);
		if (nRows < record)
		{
			((CamGrid*)form)->_SetRows(record);	
			nRows = record;
		}
	}

//	if (form->m_form->attr & FA_ENUM)
//		screen->m_cells.Lookup(form->GetSymbolName(), (CObject *&)grid);
	//cell = screen->m_vCELL[0];//(struct _precellR *)&screen->m_cellR[form->m_form->vals[0]];
	
	pCell = screen->m_MGRID[form->m_form->name];
	text.Empty();

	for (row = 0; row < nRows && idx < axisL; row++)
	{
		for (col = 0; col < nCols && idx < axisL; col++)
		{
			if ((*pCell)[col]->attr & FA_SKIP)
				continue;

			switch ((*pCell)[col]->iok)
			{
			case EIO_INOUT:
			case EIO_OUTPUT:
				formL = screen->m_vGridLen[col + 1];//(*pCell)[col]->size;
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
	/*			int record = m_vGridLen[0];
				if (record != 0)
				{
					CString sRecord = CString(&axisB[idx], record);
					record = atoi(sRecord);
				}*/

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
				Msg("%s", text);
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

void CStream::SetCells(CScreen* screen, CamBase* form, CString& text)
{
	int	nCols, nRows;
	int	ii, col, row;
	CMap	<int, int, CString, CString&>	xgrid;

	CString	tmps, forms, cells;
//	struct _cellR*	cell;
	vector <struct _cellR*>* pCell;
	CPtrArray* grid = NULL;

//	if (form->m_form->attr & FA_ENUM)
//		screen->m_cells.Lookup(form->GetSymbolName(), (CObject *&)grid);
	if (!grid)	return;

	nCols = form->m_form->col;				//HIWORD(form->m_form->vals[1]);
	nRows = form->m_form->row;				//LOWORD(form->m_form->vals[1]);

	tmps = text;
	text.Empty();
	//cell = screen->m_vCELL[0];	//(struct _precellR *)&screen->m_cellR[form->m_form->vals[0]];
	pCell = screen->m_MGRID[form->m_form->name];

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
			if ((*pCell)[col]->attr & FA_SKIP)
				continue;

			switch ((*pCell)[col]->iok)
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
				if (((struct _cellR*)grid->GetAt(ii))->name == (*pCell)[col]->name)
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
	CamBase* form;
	struct	_cellR* cell;
	vector <struct _cellR*>* pCell;
	CMap	<int, int, CString, CString&>	xgrid;
	CPtrArray* grid = NULL;

	text.Empty();
	form  = screen->GetAtForm(screen->m_ioR[key]);

//	if (form->m_form->attr & FA_ENUM)
//		screen->m_cells.Lookup(form->GetSymbolName(), (CObject *&)grid);

	int	skip = 0;
	for (int ii = 0; ii < nRows && idx + skip < datL; ii++)
	{
		//cell = screen->m_vTCELL[0];	//(struct _precellR *) &screen->m_cellR[form->m_form->vals[0]];
		pCell = screen->m_MTABLE[form->m_form->name];
		for (int kk = 0; kk < nCols && idx + skip < datL; kk++/*, cell++*/)
		{
			cell = (*pCell)[ii];

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
	CamBase* form;

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
		case FM_RADIO:
		case FM_CHECK:
			if (form->m_form->iok == EIO_NOP)
				continue;
		case FM_EDIT:
		case FM_COMBO:
		//case FM_MEMO:
			if (form->m_form->iok == EIO_INPUT) 
				continue;
		case FM_OUT:
		case FM_TABLE:
			return true;
		case FM_GRID:
		case FM_GRIDEX:
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
	CamBase* form = screen->GetAtForm(screen->m_ioR[key]);

	cc = (fcc->scc ^ fcc->rcc) & fcc->scc;
	if (cc & CC_SET)
		m_client->SetCPos(screen, screen->m_ioR[key], false);
	SetCC(form, cc, true, col, row);

	cc = (fcc->scc ^ fcc->rcc) & fcc->rcc;
	SetCC(form, cc, false, col, row);
}

void CStream::SetCC(CamBase* form, int cc, bool set, int col, int row)
{
	DWORD	attr = 0;

	if (cc & CC_PRO)
		attr |= FA_PROTECT;
//	if (cc & CC_MAND)
//		attr |= FA_MAND;
//	if (cc & CC_SEND)
//		attr |= FA_SEND;
//	if (cc & CC_SET && set)
//		attr |= FA_REF;

	form->SetAttributes(attr, set, col, row);

	if (cc & CC_VIS)
	{
		switch (form->m_form->kind)
		{
		case FM_TABLE:
		case FM_GRID:
		case FM_GRIDEX:
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
		case FM_GRIDEX:
			if (++row <= 0 || ++col <= 0)
				break;

			((CamGrid *)form)->SetCellEnable(row, col, set);
			break;
		default:
			form->SetEnable(set);
			break;
		}
	}
}

void CStream::ParseSCC(CScreen* screen, CamBase* form, _SCC* scc, int col, int row)
{
	if (form->m_form->kind == FM_GRID || form->m_form->kind == FM_GRIDEX)
	{
		((CamGrid *)form)->SetCellColumn(col+1);
		((CamGrid *)form)->SetCellRow(row+1);
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

	CamBase*	form;
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
