// build.cpp: implementation of the CBuild class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "awBuild.h"
#include "build.h"

#include "../mapvar.h"
#include "../mapform.h"
#include "../awWcc/libWcc.h"
#include "../awCommon/CellItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const char toPath[10][5]    = {"AXIS", "BP1", "BP2", "BP3", "BP4", "BP5", "BP6", "BP7", "FEP"};
const char mapMethod[4][5]  = {"FORM", "MENU", "SEND", "PASS"};
const char fontStyle[5][12] = {"", "NORMAL", "ITALIC", "BOLD", "BOLD_ITALIC"};
const char keyMap[21][12]   = {"NOTMAPPING", "PAGEUP", "PAGEDN", "HOME", "END", "F2", "F3", "F4",
				"F5", "F6", "F7", "F8", "F9", "F11", "F12", "NUMPAD+", "NUMPAD-", "NUMPAD/", "NUMPAD*", "KEYUP", "KEYDOWN"};
const char mapKind[3][16]   = {"NORMAL", "TEMPLATE", "PROCEDURES"};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBuild::CBuild()
{
	m_bOpen = false;
	m_buf  = _T("");

	m_mapH  = (struct _mapH    *) 0;
	m_pFormItem = NULL;
	m_grR   = (struct _fgraph  *) 0;
}

CBuild::~CBuild()
{
	if (m_bOpen)
		m_file.Close();
	if (m_pFormItem)
		delete m_pFormItem;
}

int CBuild::GenerateSource(bool alarm, struct _mapH* mapH)
{
	CString	tmps = _T("");
	int	len  = strlen(mapH->mapN);

	if (len <= 0 || len < L_MAPN)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, "Map Name Incorrect Error!!", "Message", MB_ICONEXCLAMATION | MB_OK);
		return (-1);
	}

	if (strlen(mapH->mapdesc) <= 0)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, "Map Description Error!! ", "Message", MB_ICONEXCLAMATION | MB_OK);
		return (-1);
	}

	wccSetTempPath(tmps, mapH->mapN);
	m_bOpen = (m_file.Open(tmps, CFile::modeCreate|CFile::modeWrite)) ? true : false;
	if (!m_bOpen)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, "\nCan't create Source.\n\nMapName Error!",
					"Message", MB_ICONEXCLAMATION|MB_OK);
		return (-1);
	}
	m_bSrc = false;

	m_mapH  = mapH;

	if (m_pFormItem)
		delete m_pFormItem;
	m_pFormItem = new CFormItem(mapH);

	generateHeader();
	newline(false);
	generateForm();
	newline(false);
	generateResource();
	m_file.Close();
	m_bOpen = false;
	mapH->source   = true;
	mapH->modified = false;

	if (alarm)
	{
		tmps.Format("\nCreated Source.\n\nMapName : %s", m_mapH->mapN);
		::MessageBox(AfxGetMainWnd()->m_hWnd, tmps, "Message", MB_ICONEXCLAMATION|MB_OK);
	}
	return 0;
}

CString CBuild::GetSource(bool bNotResource, struct _mapH* mapH)
{
	CString	tmps = _T("");
	int	len  = strlen(mapH->mapN);

	m_bOpen = false;
	m_bSrc = true;
	
	m_mapH  = mapH;
	if (m_pFormItem)
		delete m_pFormItem;
	m_pFormItem = new CFormItem(mapH);
	m_sSrc.Empty();

	generateHeader();
	newline(false);
	generateForm();
	newline(false);
	if (!bNotResource)
		generateResource();
	mapH->source   = true;
	mapH->modified = false;

	return m_sSrc;
}

void CBuild::generateHeader()
{
	int	offs;
	bool	first;
	CString	tmps;

	m_buf = "GLOBAL DIVISION";
	newline(false);
	brace(true);

	m_buf.Format("MAPNAME(%s)", m_mapH->mapN);
	newline(false);

	m_buf.Format("MAPSIZE(%d, %d)", m_mapH->height, m_mapH->width);
	newline(false);

	m_buf.Format("PYTHON(%d)", m_mapH->pythonMode);
	newline(false);

//	m_buf.Format("VERSION(%03d)", m_mapH->vers+1);	// on batch compile (temporary)
	m_buf.Format("VERSION(%03d)", m_mapH->vers);
	newline(false);

	m_buf.Format("MAPKIND(%s)", mapKind[m_mapH->mapK]);
	newline(false);

	m_buf.Format("MAPGROUP(%d)", m_mapH->mapGroup);
	newline(false);

	m_buf.Format("MAPDESC(\"%s\")", m_mapH->mapdesc);
	newline(false);

	if (strlen(m_mapH->publicR) == 0)
		strcpy_s(m_mapH->publicR, 512, ";");
	m_buf.Format("MAPPUBLIC(\"%s\")", m_mapH->publicR);
	newline(false);

	if (m_mapH->fName[0] != NULL)
	{
		m_buf.Format("FONT(\"%s\", %d, %s)", m_mapH->fName, m_mapH->fPoint, fontStyle[m_mapH->fStyle]);
		newline(false);
	}

	first = true;
	if (m_mapH->options)
	{
		m_buf = "OPTIONS(";
		if (m_mapH->options & OP_OOP)
		{
			if (!first)	addOR(m_buf);
			m_buf += "FMS";
			first = false;
		}
		if (m_mapH->options & OP_CLS)
		{
			if (!first)	addOR(m_buf);
			m_buf += "FORMAT";
			first = false;
		}
		if (m_mapH->options & OP_CNV)
		{
			if (!first)	addOR(m_buf);
			m_buf += "CODECNV";
			first = false;
		}
		if (m_mapH->options & OP_FLASH)
		{
			if (!first)	addOR(m_buf);
			m_buf += "FLASH";
			first = false;
		}
		if (m_mapH->options & OP_ENC)
		{
			if (!first)	addOR(m_buf);
			m_buf += "ENCODE";
			first = false;
		}
		if (m_mapH->options & OP_TABS)
		{
			if (!first)	addOR(m_buf);
			m_buf += "TABSEP";
			first = false;
		}
		if (m_mapH->options & OP_CR2TB)
		{
			if (!first)	addOR(m_buf);
			m_buf += "CR2TAB";
			first = false;
		}
		if (m_mapH->options & OP_DROP)
		{
			if (!first)	addOR(m_buf);
			m_buf += "DROP";
			first = false;
		}
		if (m_mapH->options & OP_USER)
		{
			if (!first)	addOR(m_buf);
			m_buf += "USER";
			first = false;
		}
		
		if (m_mapH->options & OP_INTER)
		{
			if (!first)	addOR(m_buf);
			m_buf += "INTER";
			first = false;
		}
		if (m_mapH->options & OP_CERTIFY)
		{
			if (!first)	addOR(m_buf);
			m_buf += "CERTIFY";
			first = false;
		}
		if (m_mapH->options & OP_OOP2)
		{
			if (!first)	addOR(m_buf);
			m_buf += "FMS2";
			first = false;
		}

		if (m_mapH->options & OP_DOMINO)
		{
			if (!first)	addOR(m_buf);
			m_buf += "DOMINO";
			first = false;
		}

		if (m_mapH->options & OP_XFONT)
		{
			if (!first)	addOR(m_buf);
			m_buf += "XFONT";
			first = false;
		}

		if (m_mapH->options & OP_SAVE)
		{
			if (!first)	addOR(m_buf);
			m_buf += "SAVE";
			first = false;
		}
		m_buf += ")";
		newline(false);
	}

	if (m_mapH->caption[0] != NULL)
	{
		m_buf.Format("TITLE(\"%s\")", m_mapH->caption);
		newline(false);
	}

	m_buf.Format("TOPATH(%s)", toPath[m_mapH->toPath]);
	newline(false);

	m_buf.Format("METHOD(%s)", mapMethod[m_mapH->mapM]);
	newline(false);

	if (m_mapH->trxC[0] != NULL)
	{
		m_buf.Format("TRXCODE(\"%s\")", m_mapH->trxC);
		newline(false);
	}

	m_buf = "MAPTYPE(";
	switch(m_mapH->type)
	{
	case VW_NORM:
		m_buf += "NORM";	break;
	case VW_SCROLL:
		m_buf += "SCROLL";	break;
	case VW_RESIZE:
		m_buf += "RESIZE";	break;
	case VW_WND:
		m_buf += "WND";		break;
	case VW_FIXED:
		m_buf += "FIXED";	break;
	case VW_FEX:
		m_buf += "FIXEDEX";	break;
		break;
	}
	m_buf += ")";
	newline(false);

	m_buf = "HEADER(";
	switch (m_mapH->typeH) 
	{
	case TH_NOP:
		m_buf += "NOP";		break;
	case TH_KOSCOM:
		m_buf += "KOSCOM";	break;
	case TH_SCUP:
		m_buf += "SCUP";	break;
	case TH_4702:
		m_buf += "4702";	break;
	case TH_4702Ex:
		m_buf += "4702_";	break;
	case TH_TUXEDO:
		m_buf += "TUXEDO";	break;
	case TH_TOPEND:
		m_buf += "TOPEND";	break;
	case TH_LEDGER:
		m_buf += "LEDGER";	break;
	default:
		break;
	}
	m_buf += ")";
	newline(false);

	if (m_mapH->helpN[0] != NULL)
	{
		m_buf.Format("HELPFILE(\"%s\")", m_mapH->helpN);
		newline(false);
	}

	if (m_mapH->trxH[0] != NULL)
	{
		m_buf.Format("TRXH(\"%s\")", m_mapH->trxH);
		newline(false);
	}

	if (m_mapH->repeatV != 0)
	{
		m_buf.Format("REPEATTIME(%d)", m_mapH->repeatV);
		newline(false);
	}

	if (m_mapH->waitV != 0)
	{
		m_buf.Format("WAITTIMEOUT(%d)", m_mapH->waitV);
		newline(false);
	}

	switch (m_mapH->imageK)
	{
	case IK_BRUSH:
		m_buf.Format("BGROUND(BRUSH, %d)", m_mapH->bkClr);
		newline(false);		break;
	case IK_BMP:
		tmps = (char *)m_mapH->imageV;
		offs = tmps.ReverseFind('\\');;
		if (offs != -1)
			tmps = tmps.Mid(offs+1);
		m_buf.Format("BGROUND(IMAGE, \"%s\")", tmps);
		newline(false);		break;
	case IK_NOP:
	default:
		break;
	}
	brace(false);
}

void CBuild::generateForm()
{
	m_buf = "FORM DIVISION";
	newline(false);
	brace(true);

	for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++) 
	{
		switch (m_pFormItem->getFormPoint(ii)->kind)
		{
		case FM_SYS:
			srcSys(ii);		break;
		case FM_LABEL:
			srcLabel(ii);		break;
		case FM_BOX:
			srcBox(ii);		break;
		case FM_GROUP:
			srcGroup(ii);		break;
		case FM_TAB:
			srcTab(ii);		break;
		case FM_UTAB:
			srcUsrTab(ii);		break;
		case FM_PANEL:
			srcPanel(ii);		break;
		case FM_BUTTON:
			srcButton(ii);		break;
		case FM_AVI:
			srcAVI(ii);		break;
		case FM_BROWSER:
			srcBrowser(ii);		break;
		case FM_CONTROL:
			srcControl(ii);		break;
		case FM_EDIT:
		case FM_OUT:
			srcEdit(ii);		break;
		case FM_MEMO:
			srcMemo(ii);		break;
		case FM_COMBO:
			srcCombo(ii);		break;
		case FM_GRAPH:
			srcGraph(ii);		break;
		case FM_GRID:
			srcGrid(ii);		break;
		case FM_OBJECT:
			srcObject(ii);
			break;
		case FM_TABLE:
			srcTable(ii);		break;
		case FM_TREE:
			srcTreeView(ii);	break;
		case FM_SHEET:
			srcSheet(ii);		break;
		default:
			continue;
		}
		writeEtc(ii);
	}

	brace(false);
}

void CBuild::generateResource()
{
	CString	tmps;

	m_buf = "RESOURCE DIVISION";
	newline(false);
	brace(true);

	m_buf.Empty();
	newline(false);

	if (m_mapH->onDeclaration && m_mapH->onDeclarationN[0] != NULL)
	{
		m_buf = "_AW_ONDECLARE_AW_";
		newline(false);
		writeScriptData(m_mapH->onDeclarationN);
		newline(false);
	}

	if (m_mapH->onInDeclaration && m_mapH->onInDeclarationN[0] != NULL)
	{
		m_buf = "_AW_ONINDECLARE_AW_";
		newline(false);
		writeScriptData(m_mapH->onInDeclarationN);
		newline(false);
		m_buf = "_AW_ONENDSUB_AW_";
		newline(false);
	}

	if (m_mapH->onStart && m_mapH->onStartN[0] != NULL)
	{
		m_buf = "_AW_ONSTART_AW_";
		newline(false);
		writeScriptData(m_mapH->onStartN);
		newline(false);
		m_buf = "_AW_ONENDSUB_AW_";
		newline(false);
	}

	if (m_mapH->onSend && m_mapH->onSendN[0] != NULL)
	{
		m_buf = "_AW_ONSEND_AW_";
		newline(false);
		writeScriptData(m_mapH->onSendN);
		newline(false);
		m_buf = "_AW_ONENDSUB_AW_";
		newline(false);
	}

	if (m_mapH->onReceive && m_mapH->onReceiveN[0] != NULL)
	{
		m_buf = "_AW_ONRECEIVE_AW_";
		newline(false);
		writeScriptData(m_mapH->onReceiveN);
		newline(false);
		m_buf = "_AW_ONENDSUB_AW_";
		newline(false);
	}

	if (m_mapH->onAlert && m_mapH->onAlertN[0] != NULL)
	{
		m_buf = "_AW_ONALERT_AW_";
		newline(false);
		writeScriptData(m_mapH->onAlertN);
		newline(false);
		m_buf = "_AW_ONENDSUB_AW_";
		newline(false);
	}

	if (m_mapH->onService && m_mapH->onServiceN[0] != NULL)
	{
		m_buf = "_AW_ONSERVICE_AW_";
		newline(false);
		writeScriptData(m_mapH->onServiceN);
		newline(false);
		m_buf = "_AW_ONENDSUB_AW_";
		newline(false);
	}

	if (m_mapH->onFile && m_mapH->onFileN[0] != NULL)
	{
		m_buf = "_AW_ONFILE_AW_";
		newline(false);
		writeScriptData(m_mapH->onFileN);
		newline(false);
		m_buf = "_AW_ONENDSUB_AW_";
		newline(false);
	}

	if (m_mapH->onSelect && m_mapH->onSelectN[0] != NULL)
	{
		m_buf = "_AW_ONSELECT_AW_";
		newline(false);
		writeScriptData(m_mapH->onSelectN);
		newline(false);
		m_buf = "_AW_ONENDSUB_AW_";
		newline(false);
	}

	if (m_mapH->onTimer && m_mapH->onTimerN[0] != NULL)
	{
		m_buf = "_AW_ONTIMER_AW_";
		newline(false);
		writeScriptData(m_mapH->onTimerN);
		newline(false);
		m_buf = "_AW_ONENDSUB_AW_";
		newline(false);
	}

	if (m_mapH->onFocus && m_mapH->onFocusN[0] != NULL)
	{
		m_buf = "_AW_ONFOCUS_AW_";
		newline(false);
		writeScriptData(m_mapH->onFocusN);
		newline(false);
		m_buf = "_AW_ONENDSUB_AW_";
		newline(false);
	}

	if (m_mapH->onClose && m_mapH->onCloseN[0] != NULL)
	{
		m_buf = "_AW_ONCLOSE_AW_";
		newline(false);
		writeScriptData(m_mapH->onCloseN);
		newline(false);
		m_buf = "_AW_ONENDSUB_AW_";
		newline(false);
	}

	if (m_mapH->onDevice && m_mapH->onDeviceN[0] != NULL)
	{
		m_buf = "_AW_ONDEVICE_AW_";
		newline(false);
		writeScriptData(m_mapH->onDeviceN);
		newline(false);
		m_buf = "_AW_ONENDSUB_AW_";
		newline(false);
	}

	if (m_mapH->onApprove && m_mapH->onApproveN[0] != NULL)
	{
		m_buf = "_AW_ONAPPROVE_AW_";
		newline(false);
		writeScriptData(m_mapH->onApproveN);
		newline(false);
		m_buf = "_AW_ONENDSUB_AW_";
		newline(false);
	}

	if (m_mapH->onKey && m_mapH->onKeyN[0] != NULL)
	{
		m_buf = "_AW_ONKEY_AW_";
		newline(false);
		writeScriptData(m_mapH->onKeyN);
		newline(false);
		m_buf = "_AW_ONENDSUB_AW_";
		newline(false);
	}

	/*if (m_mapH->onTrigger && m_mapH->onTriggerN[0] != NULL)
	{
		m_buf = "_AW_ONTRIGGER_AW_";
		newline(false);
		writeScriptData(m_mapH->onTriggerN);
		newline(false);
		m_buf = "_AW_ONENDSUB_AW_";
		newline(false);
	}*/

	HANDLE	hFile;
	DWORD	scpSize = 0;
	DWORD	dwReadLen;
	char*	sbuff;
	BOOL	bRc;
	bool	edit;

	char*	click      = new char[1024*32];
	char*	dblclk     = new char[1024*32];
	char*	change     = new char[1024*32];
	char*	charchange = new char[1024*32];
	char*	keychange = new char[1024*32];

	for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
	{
		if (m_pFormItem->getFormStr(ii, "name").IsEmpty())
			continue;

		edit = false;
		if (m_pFormItem->getFormPoint(ii)->isEdit != IE_NOP)
			edit = true;

		hFile = CreateFile(m_pFormItem->getFormStr(ii, "scpN"), GENERIC_READ, FILE_SHARE_READ, 
				NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			if (!edit)
				continue;
		}
		else
		{
			scpSize = GetFileSize(hFile, NULL);
			if (scpSize == 0xFFFFFFFF)
			{
				CloseHandle(hFile);
				if (!edit)
					continue;
			}
			else
			{
				sbuff = NULL;
				sbuff = new char[scpSize+1];

				dwReadLen = 0;
				bRc = ReadFile(hFile, sbuff, scpSize, &dwReadLen, NULL);
				CloseHandle(hFile);
				if (!bRc || scpSize != dwReadLen)
				{
					delete[] sbuff;
					if (!edit)
					{
AfxMessageBox("XError [b9]");
						continue;
					}
				}
				else
				{
					click[0]      = NULL;
					dblclk[0]     = NULL;
					change[0]     = NULL;
					charchange[0] = NULL;
					keychange[0]  = NULL;

					if (m_pFormItem->getFormPoint(ii)->onClick != -1)
					{
						CopyMemory(click, &sbuff[m_pFormItem->getFormPoint(ii)->onClick], m_pFormItem->getFormPoint(ii)->onClickL);
						click[m_pFormItem->getFormPoint(ii)->onClickL] = NULL;
					}
					if (m_pFormItem->getFormPoint(ii)->onDblClk != -1)
					{
						CopyMemory(dblclk, &sbuff[m_pFormItem->getFormPoint(ii)->onDblClk], m_pFormItem->getFormPoint(ii)->onDblClkL);
						dblclk[m_pFormItem->getFormPoint(ii)->onDblClkL] = NULL;
					}
					if (m_pFormItem->getFormPoint(ii)->onChange != -1)
					{
						CopyMemory(change, &sbuff[m_pFormItem->getFormPoint(ii)->onChange], m_pFormItem->getFormPoint(ii)->onChangeL);
						change[m_pFormItem->getFormPoint(ii)->onChangeL] = NULL;
					}
					if (m_pFormItem->getFormPoint(ii)->onChar != -1)
					{
						CopyMemory(charchange, &sbuff[m_pFormItem->getFormPoint(ii)->onChar], m_pFormItem->getFormPoint(ii)->onCharL);
						charchange[m_pFormItem->getFormPoint(ii)->onCharL] = NULL;
					}
					if (m_pFormItem->getFormPoint(ii)->onKey != -1)
					{
						CopyMemory(keychange, &sbuff[m_pFormItem->getFormPoint(ii)->onKey], m_pFormItem->getFormPoint(ii)->onKeyL);
						keychange[m_pFormItem->getFormPoint(ii)->onKeyL] = NULL;
					}

					delete[] sbuff;
				}
			}
		}

		m_buf.Format("_AW_SCRIPT_AW_(%s)", m_pFormItem->getFormStr(ii, "name"));
		newline(false);

		int nKind = m_pFormItem->getFormPoint(ii)->kind;

		/*if (m_pFormItem->getFormPoint(ii)->isEdit == IE_EDIT || m_pFormItem->getFormPoint(ii)->isEdit == IE_CORGB || m_pFormItem->getFormPoint(ii)->isEdit == IE_CORGB2)
		{
			m_buf = "ONEDIT";
			newline(false);

			m_buf.Empty();
			switch (m_pFormItem->getFormPoint(ii)->isEdit)
			{
			case IE_EDIT:
				m_buf.Format("EDIT(\"%s\")", m_pFormItem->getFormStr(ii, "editS"));
				break;
			case IE_CORGB:
				m_buf.Format("IFRGB(%d, %d)", HIWORD(m_pFormItem->getFormPoint(ii)->onEdit), LOWORD(m_pFormItem->getFormPoint(ii)->onEdit));
				break;
			case IE_NOP:
			default:
				break;
			}
			newline(false);
		}*/

		if (click[0] != NULL && m_pFormItem->getFormPoint(ii)->onClickL != 0 && nKind != FM_COMBO)
		{
			m_buf = "_AW_ONCLICK_AW_";
			newline(false);
			writeDataToFile(click);
			m_buf = "_AW_ONENDSUB_AW_";
			newline(false);
		}

		if (dblclk[0] != NULL && m_pFormItem->getFormPoint(ii)->onDblClkL != 0 && nKind != FM_COMBO)
		{
			m_buf = "_AW_ONDBLCLK_AW_";
			newline(false);
			writeDataToFile(dblclk);
			m_buf = "_AW_ONENDSUB_AW_";
			newline(false);
		}

		if (change[0] != NULL && m_pFormItem->getFormPoint(ii)->onChangeL != 0
			&& (nKind == FM_EDIT || nKind == FM_COMBO
			|| nKind == FM_GRID || nKind == FM_MEMO || nKind == FM_CONTROL))
		{
			m_buf = "_AW_ONCHANGE_AW_";
			newline(false);
			writeDataToFile(change);
			m_buf = "_AW_ONENDSUB_AW_";
			newline(false);
		}

		if (charchange[0] != NULL && m_pFormItem->getFormPoint(ii)->onCharL != 0 
			&& (nKind == FM_EDIT || nKind == FM_GRID))
		{
			m_buf = "_AW_ONCHARCHANGE_AW_";
			newline(false);
			writeDataToFile(charchange);
			m_buf = "_AW_ONENDSUB_AW_";
			newline(false);
		}

		if (keychange[0] != NULL && m_pFormItem->getFormPoint(ii)->onKeyL != 0 
			&& (nKind == FM_GRID || nKind == FM_EDIT))
		{
			m_buf = "_AW_ONKEYCHANGE_AW_";
			newline(false);
			writeDataToFile(keychange);
			m_buf = "_AW_ONENDSUB_AW_";
			newline(false);
		}
		m_buf = "_AW_ENDSCRIPT_AW_";
		newline(false);
		newline(false);
	}
	delete[] click;
	delete[] dblclk;
	delete[] change;
	delete[] charchange;
	delete[] keychange;

	brace(false);
	newline(false);
}

void CBuild::srcSys(int fN)
{
	writeFile("SYS");
	space(2);
	symbolRect(fN);
	space(2);
	switch (m_pFormItem->getFormPoint(fN)->type) 
	{
	case SYS_DATE:
		m_buf = "TP_DATE";		break;
	case SYS_TIME:
		m_buf = "TP_TIME";		break;
	case SYS_TERM:
		m_buf = "TP_TERM";		break;
	default:
		m_buf.Empty();			break;
	}
	addOR(m_buf);
	alignment(m_pFormItem->getFormPoint(fN)->alignment);
	if (m_pFormItem->getFormPoint(fN)->fStyle)
	{
		space(2);
		m_buf.Format("FONT(\"%s\", %d, %s)", m_pFormItem->getFormStr(fN, "fName"), 
			m_pFormItem->getFormPoint(fN)->fPoint, 
			fontStyle[m_pFormItem->getFormPoint(fN)->fStyle]);
		writeFile(m_buf);
	}
	space(2);
	attributes(m_pFormItem->getFormPoint(fN)->attr);
	space(2);
	color(m_pFormItem->getFormPoint(fN)->pRGB, m_pFormItem->getFormPoint(fN)->tRGB);
	groupN(fN);
}

void CBuild::srcLabel(int fN)
{
	writeFile("LABEL");
	space(2);
	symbolRect(fN);
	space(2);
	if (!m_pFormItem->getFormStr(fN, "str").IsEmpty())
	{
		m_buf.Format("'%s'", m_pFormItem->getFormStr(fN, "str"));
		writeFile(m_buf);
		space(2);
	}
	m_buf.Empty();
	alignment(m_pFormItem->getFormPoint(fN)->alignment);
	if (m_pFormItem->getFormPoint(fN)->fStyle)
	{
		space(2);
		m_buf.Format("FONT(\"%s\", %d, %s)", m_pFormItem->getFormStr(fN, "fName"), m_pFormItem->getFormPoint(fN)->fPoint, fontStyle[m_pFormItem->getFormPoint(fN)->fStyle]);
		writeFile(m_buf);
	}
	space(2);
	attributes(m_pFormItem->getFormPoint(fN)->attr);
	space(2);
	color(m_pFormItem->getFormPoint(fN)->pRGB, m_pFormItem->getFormPoint(fN)->tRGB);
	groupN(fN);
}

void CBuild::srcBox(int fN)
{
	writeFile("BOX");
	space(2);
	symbolRect(fN);
	space(2);
	color(m_pFormItem->getFormPoint(fN)->pRGB, m_pFormItem->getFormPoint(fN)->tRGB);
	space(2);
	m_buf.Format("LINEW(%d)", m_pFormItem->getFormPoint(fN)->size);
	writeFile(m_buf);
	space(2);
	switch (m_pFormItem->getFormPoint(fN)->type)
	{
	case BX_2CLR:
		m_buf = "BX_2CLR";	break;
	case BX_BORDER:
		m_buf = "BX_BORDER";	break;
	case BX_LINE:
	default:
		m_buf = "BX_LINE";	break;
	}
	writeFile(m_buf);
	space(2);
	attributes(m_pFormItem->getFormPoint(fN)->attr);
	groupN(fN);
}

void CBuild::srcGroup(int fN)
{
	writeFile("GROUP");
	space(2);
	symbolRect(fN);
	space(2);

	if (!m_pFormItem->getFormStr(fN, "str").IsEmpty())
	{
		m_buf.Format("'%s'", m_pFormItem->getFormStr(fN, "str"));
		writeFile(m_buf);
		space(2);
	}

	if (m_pFormItem->getFormPoint(fN)->fStyle)
	{
		space(2);
		m_buf.Format("FONT(\"%s\", %d, %s)", m_pFormItem->getFormStr(fN, "fName"), m_pFormItem->getFormPoint(fN)->fPoint, fontStyle[m_pFormItem->getFormPoint(fN)->fStyle]);
		writeFile(m_buf);
	}
	space(2);
	attributes(m_pFormItem->getFormPoint(fN)->attr);
	space(2);
	m_pFormItem->getFormPoint(fN)->pRGB = -1;
	color(m_pFormItem->getFormPoint(fN)->pRGB, m_pFormItem->getFormPoint(fN)->tRGB);
	groupN(fN);
}

void CBuild::srcTab(int fN)
{
	writeFile("TAB");
	space(2);
	symbolRect(fN);
	space(2);

	m_buf.Format("INDEX(%d)", m_pFormItem->getFormPoint(fN)->checked);
	writeFile(m_buf);	space(2);
	m_buf.Format("COUNT(%d)", m_pFormItem->getFormPoint(fN)->keys);
	writeFile(m_buf);	space(2);
	m_buf.Format("TABH(%d)", m_pFormItem->getFormPoint(fN)->size);
	writeFile(m_buf);	space(2);
	if (!m_pFormItem->getFormStr(fN, "auxS").IsEmpty())
	{
		m_buf.Format("OBJECT(\"%s\")", m_pFormItem->getFormStr(fN, "auxS"));
		writeFile(m_buf);	space(2);
	}

	switch (m_pFormItem->getFormPoint(fN)->type)
	{
	case TAB_NONE:
		writeFile("TAB_NO3D");		break;
	case TAB_OBLIQUE:
		writeFile("TAB_OBLIQUE");	break;
	case TAB_RECT:
		writeFile("TAB_RECT");		break;
	case TAB_IMAGE:
		writeFile("TAB_IMAGE");		break;
	case TAB_BORDER:
	default:
		writeFile("TAB_3D");		break;
	}
	space(2);

	attributes(m_pFormItem->getFormPoint(fN)->attr);
	space(2);
	m_buf.Empty();
	if (m_pFormItem->getFormPoint(fN)->fStyle)
	{
		space(2);
		m_buf.Format("FONT(\"%s\", %d, %s)", m_pFormItem->getFormStr(fN, "fName"), m_pFormItem->getFormPoint(fN)->fPoint, fontStyle[m_pFormItem->getFormPoint(fN)->fStyle]);
		writeFile(m_buf);
	}
	space(2);
	color(m_pFormItem->getFormPoint(fN)->pRGB, m_pFormItem->getFormPoint(fN)->tRGB);
	space(2);

	if (m_pFormItem->getFormPoint(fN)->type == TAB_IMAGE)
	{
		CString	tmps = m_pFormItem->getFormStr(fN, "str2");
		int	offs = tmps.ReverseFind('\\');
		if (offs != -1)
			tmps = tmps.Mid(offs+1);
		m_buf.Format("TABIMAGE(\"%s\")", tmps);
		writeFile(m_buf);
		space(2);
	}

	if (m_pFormItem->getFormPoint(fN)->spRGB == 0 && m_pFormItem->getFormPoint(fN)->stRGB == 0)
	{
		m_pFormItem->getFormPoint(fN)->spRGB = (m_pFormItem->getFormPoint(fN)->type == TAB_NONE) ? 193 : m_pFormItem->getFormPoint(fN)->pRGB;
		m_pFormItem->getFormPoint(fN)->stRGB   = (m_pFormItem->getFormPoint(fN)->type == TAB_NONE) ? 203 : m_pFormItem->getFormPoint(fN)->tRGB;
	}
	m_buf.Format("FCOLOR(%d, %d)", m_pFormItem->getFormPoint(fN)->spRGB, m_pFormItem->getFormPoint(fN)->stRGB);
	writeFile(m_buf);
	space(2);

	if (m_pFormItem->getFormPoint(fN)->keys != 0)
	{
		CString	tmps, data;
		int ii = 0;
		m_buf = "CAPTION(";
		data  = m_pFormItem->getFormStr(fN, "dat");
		for (ii = 0; ii < (int)m_pFormItem->getFormPoint(fN)->keys; ii++)
		{
			tmps.Format("\"%s\"", Parser(data, "\t"));
			if (ii < (int)m_pFormItem->getFormPoint(fN)->keys - 1)
				tmps += ", ";
			
			m_buf += tmps;
		}
		m_buf += ")";
		writeFile(m_buf);	space(2);
		
		m_buf = "UNIT(";
		data = m_pFormItem->getFormStr(fN, "str");
		for (ii = 0; ii < (int)m_pFormItem->getFormPoint(fN)->keys; ii++)
		{
			tmps.Format("\"%s\"", Parser(data, "\t"));
			if (ii < (int)m_pFormItem->getFormPoint(fN)->keys - 1)
				tmps += ", ";
			
			m_buf += tmps;
		}
		m_buf += ")";
		writeFile(m_buf);

		for (ii = 0; ii < (int)m_pFormItem->getFormPoint(fN)->keys; ii++)
		{
			CString temp = _T("");
			int nCnt = 0;
			for (int jj = 0; jj < m_pFormItem->getFormCount(); jj++)
			{
				if (m_pFormItem->getFormPoint(jj)->tabN == m_pFormItem->getFormPoint(fN) && m_pFormItem->getFormPoint(jj)->pageN == ii)
				{
					CString tmp;
					nCnt++;
					tmp.Format(", %d", jj);
					temp +=tmp;
				}
			}
			m_buf.Format("  PAGE(%d, %d%s)", ii, nCnt, temp);
			writeFile(m_buf);
		}
	}
	groupN(fN);	
}

void CBuild::srcUsrTab(int fN)
{
	writeFile("USRTAB");
	space(2);
	symbolRect(fN);
	space(2);

	if (m_pFormItem->getFormPoint(fN)->properties & PR_CHECKED)
	{
		writeFile("FOCUS");
		space(2);
		m_buf.Format("INDEX(%d)", m_pFormItem->getFormPoint(fN)->checked);
		writeFile(m_buf);	space(2);
	}

	m_buf.Format("COUNT(%d)", m_pFormItem->getFormPoint(fN)->keys);
	writeFile(m_buf);	space(2);
	m_buf.Empty();
	alignment(m_pFormItem->getFormPoint(fN)->alignment);
	space(2);
	switch (m_pFormItem->getFormPoint(fN)->type)
	{
	case TAB_NONE:
		writeFile("TAB_NO3D");		break;
	case TAB_OBLIQUE:
		writeFile("TAB_OBLIQUE");	break;
	case TAB_RECT:
		writeFile("TAB_RECT");		break;
	case TAB_IMAGE:
		writeFile("TAB_IMAGE");		break;
	case TAB_BORDER:
	default:
		writeFile("TAB_3D");		break;
	}
	space(2);

	attributes(m_pFormItem->getFormPoint(fN)->attr);
	space(2);
	m_buf.Empty();
	if (m_pFormItem->getFormPoint(fN)->fStyle)
	{
		space(2);
		m_buf.Format("FONT(\"%s\", %d, %s)", m_pFormItem->getFormStr(fN, "fName"), m_pFormItem->getFormPoint(fN)->fPoint, fontStyle[m_pFormItem->getFormPoint(fN)->fStyle]);
		writeFile(m_buf);
	}
	space(2);
	color(m_pFormItem->getFormPoint(fN)->pRGB, m_pFormItem->getFormPoint(fN)->tRGB);
	space(2);

	if (m_pFormItem->getFormPoint(fN)->type == TAB_IMAGE)
	{
		CString	tmps = m_pFormItem->getFormStr(fN, "str2");
		int	offs = tmps.ReverseFind('\\');
		if (offs != -1)
			tmps = tmps.Mid(offs+1);

		m_buf.Format("TABIMAGE(\"%s\")", tmps);
		writeFile(m_buf);
		space(2);
	}

	if (m_pFormItem->getFormPoint(fN)->spRGB == 0 && m_pFormItem->getFormPoint(fN)->stRGB == 0)
	{
		m_pFormItem->getFormPoint(fN)->spRGB = (m_pFormItem->getFormPoint(fN)->type == TAB_NONE) ? 193 : m_pFormItem->getFormPoint(fN)->pRGB;
		m_pFormItem->getFormPoint(fN)->stRGB   = (m_pFormItem->getFormPoint(fN)->type == TAB_NONE) ? 203 : m_pFormItem->getFormPoint(fN)->tRGB;
	}
	m_buf.Format("FCOLOR(%d, %d)", m_pFormItem->getFormPoint(fN)->spRGB, m_pFormItem->getFormPoint(fN)->stRGB);
	writeFile(m_buf);
	space(2);

	if (m_pFormItem->getFormPoint(fN)->keys != 0)
	{
		CString	tmps, data;

		m_buf = "CAPTION(";
		data  = m_pFormItem->getFormStr(fN, "dat");
		int ii = 0;
		for (ii = 0; ii < (int)m_pFormItem->getFormPoint(fN)->keys; ii++)
		{
			tmps.Format("\"%s\"", Parser(data, "\t"));
			if (ii < (int)m_pFormItem->getFormPoint(fN)->keys - 1)
				tmps += ", ";

			m_buf += tmps;
		}
		m_buf += ")";
		writeFile(m_buf);	space(2);

		m_buf = "TABINFO(";
		data = m_pFormItem->getFormStr(fN, "str");
		for (ii = 0; ii < (int)m_pFormItem->getFormPoint(fN)->keys; ii++)
		{
			tmps.Format("\"%s\"", Parser(data, "\t"));
			if (ii < (int)m_pFormItem->getFormPoint(fN)->keys - 1)
				tmps += ", ";

			m_buf += tmps;
		}
		m_buf += ")";
		writeFile(m_buf);
	}
	groupN(fN);
}

void CBuild::srcPanel(int fN)
{
	writeFile("PANEL");
	space(2);
	symbolRect(fN);
	space(2);
	if (!(m_pFormItem->getFormPoint(fN)->properties & PR_IMAGE) && !m_pFormItem->getFormStr(fN, "str").IsEmpty())
	{
		m_buf.Format("'%s'", m_pFormItem->getFormStr(fN, "str"));
		writeFile(m_buf);
		space(2);
	}

	switch (m_pFormItem->getFormPoint(fN)->type)
	{
	case PN_BORDER:
		switch (m_pFormItem->getFormPoint(fN)->borders) 
		{
		case BD_NONE:
			m_buf = "TP_NONE";	break;
		case BD_RAISED:
			m_buf = "TP_RAISED";	break;
		case BD_SUNKEN:
			m_buf = "TP_SUNKEN";	break;
		case BD_BUMP:
			m_buf = "TP_BUMP";	break;
		case BD_ETCHED:
			m_buf = "TP_ETCHED";	break;
		default:
			m_buf.Empty();		break;
		}
		writeFile(m_buf);
		break;

	case PN_LINE:
		m_buf.Format("LINECOLOR(%d)", m_pFormItem->getFormPoint(fN)->bRGB);
		writeFile(m_buf);
		break;

	case PN_NONE:
		break;
	}
	space(2);
	writeAuxiliary(fN);
	color(m_pFormItem->getFormPoint(fN)->pRGB, m_pFormItem->getFormPoint(fN)->tRGB);
	space(2);
	attributes(m_pFormItem->getFormPoint(fN)->attr);
	space(2);
	if ((m_pFormItem->getFormPoint(fN)->properties & PR_IMAGE) ||
		(m_pFormItem->getFormPoint(fN)->properties & PR_IMAGETEXT))
	{
		CString	tmps = m_pFormItem->getFormStr(fN, "str2");
		int	offs = tmps.ReverseFind('\\');
		if (offs != -1)
			tmps = tmps.Mid(offs+1);

		m_buf.Format("\"%s\"", tmps);
		writeFile(m_buf);
	}
	if (!(m_pFormItem->getFormPoint(fN)->properties & PR_IMAGE))
	{
		m_buf.Empty();
		if (!(m_pFormItem->getFormPoint(fN)->properties & PR_IMAGETEXT))
		{
			alignment(m_pFormItem->getFormPoint(fN)->alignment);
			space(2);
		}
		if (m_pFormItem->getFormPoint(fN)->fStyle != 0)
		{
			m_buf.Format("FONT(\"%s\", %d, %s)", m_pFormItem->getFormStr(fN, "fName"), m_pFormItem->getFormPoint(fN)->fPoint, fontStyle[m_pFormItem->getFormPoint(fN)->fStyle]);
			writeFile(m_buf);
		}
	}
	groupN(fN);
}

void CBuild::srcButton(int fN)
{
	writeFile("BUTTON");
	space(2);
	symbolRect(fN);
	space(2);
	if (!(m_pFormItem->getFormPoint(fN)->properties & PR_IMAGE) && !m_pFormItem->getFormStr(fN, "str").IsEmpty()) 
	{
		m_buf.Format("'%s'", m_pFormItem->getFormStr(fN, "str"));
		writeFile(m_buf);
		space(2);
	}

	switch (m_pFormItem->getFormPoint(fN)->type)
	{
	case BTN_PUSH:
		writeFile("TP_PUSH");
		writeAuxiliary(fN);
		break;

	case BTN_RADIO:
	case BTN_CHECK:
		if (m_pFormItem->getFormPoint(fN)->type == BTN_RADIO)
			m_buf = "TP_RADIO";
		else
			m_buf = "TP_CHECK";
		addOR(m_buf);
		switch (m_pFormItem->getFormPoint(fN)->iok)
		{
		case EIO_INPUT:
			m_buf += "EDIT_INPUT";	break;
		case EIO_INOUT:
			m_buf += "EDIT_INOUT";	break;
		case EIO_NOP:
		default:
			m_buf += "EDIT_NOP";	break;
		}
		writeFile(m_buf);
		writeAuxiliary(fN);
		space(2);
		attributes(m_pFormItem->getFormPoint(fN)->attr);
		if (m_pFormItem->getFormPoint(fN)->type == BTN_RADIO && !m_pFormItem->getFormStr(fN, "auxS").IsEmpty())
		{
			space(2);
			m_buf.Format("GROUP(%s)", m_pFormItem->getFormStr(fN, "auxS"));
			writeFile(m_buf);
		}
		break;

	case BTN_USER:
		writeFile("TP_USER");
		writeAuxiliary(fN);
		space(2);
		if (m_pFormItem->getFormPoint(fN)->keys > 0)
		{
			m_buf.Format("KEYMAP(%s)", keyMap[m_pFormItem->getFormPoint(fN)->keys]);
			writeFile(m_buf);
		}
		break;

	default:
		break;
	}
	space(2);
	m_buf.Empty();

	if (!(m_pFormItem->getFormPoint(fN)->properties & PR_IMAGE))
	{
		space(2);
		if (m_pFormItem->getFormPoint(fN)->type == BTN_PUSH || m_pFormItem->getFormPoint(fN)->type == BTN_USER)
		{
			if (!(m_pFormItem->getFormPoint(fN)->properties & PR_IMAGETEXT))
			{
				alignment(m_pFormItem->getFormPoint(fN)->alignment);
				space(2);
			}
		}

		if (m_pFormItem->getFormPoint(fN)->fStyle != 0)
		{
			m_buf.Format("FONT(\"%s\", %d, %s)", m_pFormItem->getFormStr(fN, "fName"), m_pFormItem->getFormPoint(fN)->fPoint, fontStyle[m_pFormItem->getFormPoint(fN)->fStyle]);
			writeFile(m_buf);
		}
	}

	if ((m_pFormItem->getFormPoint(fN)->type == BTN_PUSH || m_pFormItem->getFormPoint(fN)->type == BTN_USER) && (m_pFormItem->getFormPoint(fN)->properties & PR_CHECKED))
	{
		space(2);
		writeFile("FOCUS");
	}

	color(m_pFormItem->getFormPoint(fN)->pRGB, m_pFormItem->getFormPoint(fN)->tRGB);
	space(2);
	attributes(m_pFormItem->getFormPoint(fN)->attr);
	space(2);
	if (m_pFormItem->getFormPoint(fN)->properties & PR_IMAGE || m_pFormItem->getFormPoint(fN)->properties & PR_IMAGETEXT)
	{
		CString	tmps = m_pFormItem->getFormStr(fN, "str2");
		int	offs = tmps.ReverseFind('\\');
		if (offs != -1)
			tmps = tmps.Mid(offs+1);

		m_buf.Format("\"%s\"", tmps);
		writeFile(m_buf);
		space(2);
	}
	taborder(m_pFormItem->getFormPoint(fN)->index);
	if ((m_pFormItem->getFormPoint(fN)->type == BTN_RADIO || m_pFormItem->getFormPoint(fN)->type == BTN_CHECK)
		&& m_pFormItem->getFormPoint(fN)->iok != EIO_NOP)
		ioorder(m_pFormItem->getFormPoint(fN)->iorder);
	groupN(fN);
}

void CBuild::srcAVI(int fN)
{
	writeFile("AVI");
	space(2);
	symbolRect(fN);
	space(2);

	CString	tmps = m_pFormItem->getFormStr(fN, "str2");
	int	offs = tmps.ReverseFind('\\');;
	if (offs != -1)
		tmps = tmps.Mid(offs+1);
	m_buf.Format("\"%s\"", tmps);
	writeFile(m_buf);
	space(2);
	attributes(m_pFormItem->getFormPoint(fN)->attr);
	groupN(fN);
}

void CBuild::srcObject(int fN)
{
	CRect	fmRc;

	writeFile("OBJECT");
	space(2);
	fmRc.SetRect(m_pFormItem->getFormPoint(fN)->left, m_pFormItem->getFormPoint(fN)->top, m_pFormItem->getFormPoint(fN)->right, m_pFormItem->getFormPoint(fN)->bottom);
	symbolRect(fN, fmRc);
	space(2);
	attributes(m_pFormItem->getFormPoint(fN)->attr);	space(2);
	if (m_pFormItem->getFormPoint(fN)->attr2 & GO_FIX)
	{
		writeFile("RESIZABLE");
		space(2);
	}
	m_buf.Format("\"%s\"", m_pFormItem->getFormStr(fN, "str"));
	writeFile(m_buf);
	if (m_pFormItem->getFormPoint(fN)->bFixedSize)
		writeFile("FIXEDSIZE");

	taborder(m_pFormItem->getFormPoint(fN)->index);
	ioorder(m_pFormItem->getFormPoint(fN)->iorder);
	groupN(fN);
}

void CBuild::srcGraph(int fN)
{
	writeFile("GRAPH");
	space(2);
	symbolRect(fN);
	space(2);

	m_buf.Format("DATE(%d)", m_pFormItem->getFormPoint(fN)->size);
	writeFile(m_buf);	space(2);
	m_pFormItem->getFormPoint(fN)->fStyle = (BYTE)max((int)m_pFormItem->getFormPoint(fN)->fStyle, 1);
	m_buf.Format("FONT(\"%s\", %d, %s)", m_pFormItem->getFormStr(fN, "fName"), m_pFormItem->getFormPoint(fN)->fPoint, fontStyle[m_pFormItem->getFormPoint(fN)->fStyle]);
	writeFile(m_buf);	space(2);
	color(m_pFormItem->getFormPoint(fN)->pRGB, m_pFormItem->getFormPoint(fN)->tRGB);
	space(2);
	if (!m_pFormItem->getFormStr(fN, "auxS").IsEmpty())
	{
		m_buf.Format("REF(%s)", m_pFormItem->getFormStr(fN, "auxS"));
		writeFile(m_buf);	space(2);
	}
	if (!m_pFormItem->getFormStr(fN, "str").IsEmpty())
	{
		m_buf.Format("\"%s\"", m_pFormItem->getFormStr(fN, "str"));
		writeFile(m_buf);
	}
	space(2);
	attributes(m_pFormItem->getFormPoint(fN)->attr);
	ioorder(m_pFormItem->getFormPoint(fN)->iorder);
	groupN(fN);
}

void CBuild::srcEdit(int fN)
{
	switch (m_pFormItem->getFormPoint(fN)->kind) 
	{
	case FM_EDIT:
		writeFile("EDIT");	break;
	case FM_OUT:
		writeFile("OUT");	break;
	default:
		return;
	}
	space(2);
	symbolRect(fN);
	space(2);

	m_buf.Format("LEN(%d)", m_pFormItem->getFormPoint(fN)->size);
	writeFile(m_buf);
	space(2);
	dataType(m_pFormItem->getFormPoint(fN)->type);
	m_buf.Empty();
	if (m_pFormItem->getFormPoint(fN)->kind == FM_EDIT)
	{
		addOR(m_buf);
		if (m_pFormItem->getFormPoint(fN)->iok == EIO_INPUT)
			m_buf += "EDIT_INPUT";
		else
			m_buf += "EDIT_INOUT";
	}
	else
	{
		addOR(m_buf);
		alignment(m_pFormItem->getFormPoint(fN)->alignment);
		addOR(m_buf);
	}
	writeFile(m_buf);

	if (m_pFormItem->getFormPoint(fN)->type == IO_DIGIT)
		m_pFormItem->getFormPoint(fN)->attr |= FA_XFLOAT;

	if (m_pFormItem->getFormPoint(fN)->attr & FA_XFLOAT)
		m_pFormItem->getFormPoint(fN)->offs = 0;

	if (m_pFormItem->getFormPoint(fN)->offs)
		m_pFormItem->getFormPoint(fN)->attr |= FA_FLOAT;
	else
		m_pFormItem->getFormPoint(fN)->attr &= ~FA_FLOAT;

	attributes(m_pFormItem->getFormPoint(fN)->attr);
	subattributes(m_pFormItem->getFormPoint(fN)->attr, m_pFormItem->getFormPoint(fN)->attr2);
	if (m_pFormItem->getFormPoint(fN)->properties & PR_CODE)
	{
		addOR(m_buf);
		m_buf += "CODE";
		writeFile(m_buf);
		space(2);
	}
	if (m_pFormItem->getFormPoint(fN)->properties & PR_SUM)
	{
		addOR(m_buf);
		m_buf += "SUM";
		writeFile(m_buf);
		space(2);
	}
	if (m_pFormItem->getFormPoint(fN)->kind == FM_EDIT &&
		m_pFormItem->getFormPoint(fN)->properties & PR_LIST)
	{
		addOR(m_buf);
		m_buf += "LIST";
		writeFile(m_buf);
		space(2);
	}
	if (m_pFormItem->getFormPoint(fN)->kind == FM_EDIT &&
		m_pFormItem->getFormPoint(fN)->properties & PR_EDIT)
	{
		addOR(m_buf);
		m_buf += "SHOWFORM";
		writeFile(m_buf);
		space(2);
	}
	if (m_pFormItem->getFormPoint(fN)->vals0 > 0)
	{
		space(2);
		m_buf.Format("SPIN(%d)", m_pFormItem->getFormPoint(fN)->keys);
		writeFile(m_buf);
		space(2);
		if (m_pFormItem->getFormPoint(fN)->vals0 > 1)
		{
			m_buf.Format("SPINTYPE(%d)", m_pFormItem->getFormPoint(fN)->vals0);
			writeFile(m_buf);
			space(2);
		}
	}
	else
		m_pFormItem->getFormPoint(fN)->properties &= ~PR_SPIN;

	m_buf.Empty();
	if (m_pFormItem->getFormPoint(fN)->fStyle)
	{
		space(2);
		m_buf.Format("FONT(\"%s\", %d, %s)", m_pFormItem->getFormStr(fN, "fName"), m_pFormItem->getFormPoint(fN)->fPoint, fontStyle[m_pFormItem->getFormPoint(fN)->fStyle]);
		writeFile(m_buf);
	}
	space(2);
	color(m_pFormItem->getFormPoint(fN)->pRGB, m_pFormItem->getFormPoint(fN)->tRGB);
	space(2);
	
	if (m_pFormItem->getFormPoint(fN)->kind == FM_OUT)
	{
		switch (m_pFormItem->getFormPoint(fN)->iok)
		{
		case PN_BORDER:
			switch (m_pFormItem->getFormPoint(fN)->borders) 
			{
			case BD_NONE:
				m_buf = "TP_NONE";	break;
			case BD_RAISED:
				m_buf = "TP_RAISED";	break;
			case BD_SUNKEN:
				m_buf = "TP_SUNKEN";	break;
			case BD_BUMP:
				m_buf = "TP_BUMP";	break;
			case BD_ETCHED:
				m_buf = "TP_ETCHED";	break;
			default:
				m_buf.Empty();		break;
			}
			writeFile(m_buf);
			break;
			
		case PN_LINE:
			m_buf.Format("LINECOLOR(%d)", m_pFormItem->getFormPoint(fN)->bRGB);
			writeFile(m_buf);
			break;
			
		case PN_NONE:
			break;
		}
		space(2);
	}
	else if (m_pFormItem->getFormPoint(fN)->kind == FM_EDIT)
	{
		switch (m_pFormItem->getFormPoint(fN)->borders)
		{
		case BD_NONE:	// sunken <--> none
			m_buf = "TP_NONE";	break;
		case BD_SUNKEN:	// none <--> sunken
			m_buf = "TP_SUNKEN";	break;
		default:
			m_buf.Empty();		break;
		}
		writeFile(m_buf);
		space(2);
	}
	
	if (m_pFormItem->getFormPoint(fN)->kind == FM_EDIT)
	{
		if (!m_pFormItem->getFormStr(fN, "str").IsEmpty())
		{
			m_buf.Format("INIT(\"%s\")", m_pFormItem->getFormStr(fN, "str"));
			writeFile(m_buf);	space(2);
		}

		if (m_pFormItem->getFormPoint(fN)->initC)
			writeFile("CURSOR");
	}
	space(2);
	if (m_pFormItem->getFormPoint(fN)->kind == FM_EDIT)
	{
		if (!m_pFormItem->getFormStr(fN, "str2").IsEmpty() && 
			(m_pFormItem->getFormPoint(fN)->attr & FA_ENUM))
		{
			m_buf.Format("ENCRYPT(\"%s\")", m_pFormItem->getFormStr(fN, "str2"));
			writeFile(m_buf);	space(2);
		}
	}
	space(2);
	if (m_pFormItem->getFormPoint(fN)->kind == FM_OUT || m_pFormItem->getFormPoint(fN)->kind == FM_EDIT)
	{
		if (m_pFormItem->getFormPoint(fN)->offs)
		{
			space(2);
			m_buf.Format("FLOAT(%d)", m_pFormItem->getFormPoint(fN)->offs);
			writeFile(m_buf);
		}
	}
	space(2);
	if (!m_pFormItem->getFormStr(fN, "auxS").IsEmpty())
	{
		m_buf.Format("DOM(%s)", m_pFormItem->getFormStr(fN, "auxS"));
		writeFile(m_buf);
	}

	if (!m_pFormItem->getFormStr(fN, "refS").IsEmpty())
	{
		m_buf.Format("REFERENCE(%s)", m_pFormItem->getFormStr(fN, "refS"));
		writeFile(m_buf);
	}

	switch (m_pFormItem->getFormPoint(fN)->isEdit) 
	{
	case IE_EDIT:
		space(2);
		m_buf.Format("EDITFORMAT(\"%s\")", m_pFormItem->getFormPoint(fN)->editS);
		writeFile(m_buf);
		m_buf.Empty();
		break;
	case IE_CORGB:
		space(2);
		m_buf.Format("IFRGB(%d, %d)", HIWORD(m_pFormItem->getFormPoint(fN)->onEdit),
			LOWORD(m_pFormItem->getFormPoint(fN)->onEdit));
		writeFile(m_buf);
		m_buf.Empty();
		break;
	case IE_NOP:
	default:
		break;
	}
	
	taborder(m_pFormItem->getFormPoint(fN)->index);
	ioorder(m_pFormItem->getFormPoint(fN)->iorder);
	groupN(fN);
}

void CBuild::srcCombo(int fN)
{
	writeFile("COMBO");
	space(2);
	symbolRect(fN);
	space(2);

	switch (m_pFormItem->getFormPoint(fN)->borders)
	{
	case BD_NONE:	// 3D
		m_buf = "TP_NONE";	break;
	case BD_SUNKEN:	// line
		m_buf = "TP_SUNKEN";	break;
	default:
		m_buf.Empty();		break;
	}
	if (!m_buf.IsEmpty())
	{
		writeFile(m_buf);
		space(2);
	}

	m_buf.Format("LEN(%d)", m_pFormItem->getFormPoint(fN)->size);
	writeFile(m_buf);	space(2);
	m_buf.Format("COUNT(%d)", LOWORD(m_pFormItem->getFormPoint(fN)->keys));
	writeFile(m_buf);	space(2);
	m_buf.Format("VCOUNT(%d)", HIWORD(m_pFormItem->getFormPoint(fN)->keys));
	writeFile(m_buf);	space(2);
	m_buf.Format("INDEX(%d)", m_pFormItem->getFormPoint(fN)->checked);
	writeFile(m_buf);	space(2);
	if (!m_pFormItem->getFormStr(fN, "str2").IsEmpty())
	{
		m_buf.Format("CVAR(%s)", m_pFormItem->getFormStr(fN, "str2"));
		writeFile(m_buf);	space(2);
	}

	m_buf.Format("MATCHKEY(%s)", keyMap[m_pFormItem->getFormPoint(fN)->onEdit]);
	writeFile(m_buf);
	space(2);

	dataType(m_pFormItem->getFormPoint(fN)->type);
	m_buf.Empty();
	addOR(m_buf);
	if (m_pFormItem->getFormPoint(fN)->iok == EIO_INPUT)
		m_buf += "EDIT_INPUT";
	else
		m_buf += "EDIT_INOUT";
	writeFile(m_buf);
	attributes(m_pFormItem->getFormPoint(fN)->attr);
	subattributes(m_pFormItem->getFormPoint(fN)->attr, m_pFormItem->getFormPoint(fN)->attr2);
	if (m_pFormItem->getFormPoint(fN)->properties & PR_CODE)
	{
		addOR(m_buf);
		m_buf += "CODE";
		writeFile(m_buf);
		space(2);
	}

	if (m_pFormItem->getFormPoint(fN)->fStyle)
	{
		space(2);
		m_buf.Format("FONT(\"%s\", %d, %s)", m_pFormItem->getFormStr(fN, "fName"), m_pFormItem->getFormPoint(fN)->fPoint, fontStyle[m_pFormItem->getFormPoint(fN)->fStyle]);
		writeFile(m_buf);
	}
	space(2);
	color(m_pFormItem->getFormPoint(fN)->pRGB, m_pFormItem->getFormPoint(fN)->tRGB);
	space(2);
	if (LOWORD(m_pFormItem->getFormPoint(fN)->keys) != 0)
	{
		CString	tmps, data;
		int	count = LOWORD(m_pFormItem->getFormPoint(fN)->keys);
		int ii = 0;
		m_buf = "DISPLAY(";
		data = m_pFormItem->getFormStr(fN, "dat");
		for (ii = 0; ii < count; ii++)
		{
			tmps.Format("\"%s\"", Parser(data, "\t"));
			if (ii < count - 1)
				tmps += ", ";

			m_buf += tmps;
		}
		m_buf += ")";
		writeFile(m_buf);	space(2);

		m_buf = "DATA(";
		data = m_pFormItem->getFormStr(fN, "str");
		for (ii = 0; ii < count; ii++)
		{
			tmps.Format("\"%s\"", Parser(data, "\t"));
			if (ii < count - 1)
				tmps += ", ";

			m_buf += tmps;
		}
		m_buf += ")";
		writeFile(m_buf);	space(2);
	}

	if (m_pFormItem->getFormPoint(fN)->initC)
	{
		space(2);
		writeFile("CURSOR");
	}

	switch (m_pFormItem->getFormPoint(fN)->isEdit) 
	{
	case IE_CORGB:
		break;
	case IE_EDIT:
	case IE_NOP:
	default:
		if (m_pFormItem->getFormPoint(fN)->editS)
		{
			space(2);
			m_buf.Format("EDITFORMAT(\"%s\")", m_pFormItem->getFormPoint(fN)->editS);
			writeFile(m_buf);
			m_buf.Empty();
		}
		break;
	}
	taborder(m_pFormItem->getFormPoint(fN)->index);
	ioorder(m_pFormItem->getFormPoint(fN)->iorder);
	groupN(fN);
}

void CBuild::srcGrid(int fN)
{
	writeFile("GRID");
	space(2);
	symbolRect(fN);
	space(2);

	struct	_formR	*formR = m_pFormItem->getFormPoint(fN);
	struct	_repR*	repR   = (struct _repR *)formR->auxR;

	m_buf.Format("VISIBLEROW(%d)", HIWORD(formR->keys));
	writeFile(m_buf);	space(2);
	m_buf.Format("CELLH(%d)", LOWORD(formR->keys));
	writeFile(m_buf);	space(2);
	m_buf.Format("MATRIX(%d, %d)", repR->rows, repR->cols);
	writeFile(m_buf);	space(2);
	m_buf.Format("FIX(%d)", (formR->attr2 & GO_MULTI) ? 0 : formR->type);
	writeFile(m_buf);	space(2);
	m_buf.Format("LINECOLOR(%d)", formR->bRGB);
	writeFile(m_buf);	space(2);
	m_buf.Format("FOCUSCOLOR(%d)", formR->onEdit);
	writeFile(m_buf);	space(2);
	m_buf.Format("FOCUSKIND(%d)", formR->isEdit);
	writeFile(m_buf);	space(2);
	if (formR->attr2 & GO_AUTOSIZE)
	{
		writeFile("AUTOSIZE");	space(2);
	}

	if (formR->fStyle)
	{
		space(2);
		m_buf.Format("FONT(\"%s\", %d, %s)", m_pFormItem->getFormStr(fN, "fName"), formR->fPoint, fontStyle[formR->fStyle]);
		writeFile(m_buf);
		space(2);
	}

	if (!m_pFormItem->getFormStr(fN, "str").IsEmpty())
	{
		m_buf.Format("VARIANT(\"%s\")", m_pFormItem->getFormStr(fN, "str"));
		writeFile(m_buf);	space(2);
	}

	if (!m_pFormItem->getFormStr(fN, "str2").IsEmpty())
	{
		m_buf.Format("GRSHARE(\"%s\")", m_pFormItem->getFormStr(fN, "str2"));
		writeFile(m_buf);	space(2);
	}

	if (formR->attr2 & GO_ROW)
	{
		m_buf.Format("ROWC(%d, %d)", formR->spRGB, formR->stRGB);
		writeFile(m_buf);	space(2);
	}

	m_buf.Empty();
	if (formR->attr2 & GO_MULTI)
	{
		addOR(m_buf);
		m_buf += "MULTI";
	}
	if (formR->attr2 & GO_MERGE)
	{
		addOR(m_buf);
		m_buf += "MERGE";
	}
	if (formR->attr2 & GO_ALWAYS)
	{
		addOR(m_buf);
		m_buf += "SELALWAYS";
	}
	if (formR->attr2 & GO_FLEX)
	{
		addOR(m_buf);
		m_buf += "FLEXIBLE";
	}
	if (formR->attr2 & GO_VSCR)
	{
		addOR(m_buf);
		m_buf += "VSCROLL";
	}
	if (formR->attr2 & GO_HSCR)
	{
		addOR(m_buf);
		m_buf += "HSCROLL";
	}
	if (formR->attr2 & GO_TOP)
	{
		addOR(m_buf);
		m_buf += "INS_TOP";
	}
	if (formR->attr2 & GO_XHEAD)
	{
		addOR(m_buf);
		m_buf += "NOHEAD";
	}
	if (formR->attr2 & GO_FIX)
	{
		addOR(m_buf);
		m_buf += "GRFIX";
	}
	if (formR->attr2 & GO_X3D)
	{
		addOR(m_buf);
		m_buf += "NO3D";
	}
	if (formR->attr2 & GO_3DHEAD)
	{
		addOR(m_buf);
		m_buf += "ONLYHEAD3D";
	}
	if (formR->attr2 & GO_ROW)
	{
		addOR(m_buf);
		m_buf += "RROW";
	}
	if (formR->attr2 & GO_XVLINE)
	{
		addOR(m_buf);
		m_buf += "XVLINE";
	}
	if (formR->attr2 & GO_XHLINE)
	{
		addOR(m_buf);
		m_buf += "XHLINE";
	}
	if (formR->attr2 & GO_PAGE)
	{
		addOR(m_buf);
		m_buf += "PAGE";
	}
	if (formR->attr2 & GO_HEADER)
	{
		addOR(m_buf);
		m_buf += "HEADER";
	}
	if (formR->attr2 & GO_TABLE)
	{
		addOR(m_buf);
		m_buf += "TABLE";
	}
	if (formR->attr2 & GO_THUMB)
	{
		addOR(m_buf);
		m_buf += "THUMB";
	}
	if (formR->attr2 & GO_CUD)
	{
		addOR(m_buf);
		m_buf += "CUD";
	}
	if (formR->attr2 & GO_KEY)
	{
		addOR(m_buf);
		m_buf += "KEYFOCUS";
	}
	if (formR->attr2 & GO_EXPAND)
	{
		addOR(m_buf);
		m_buf += "GRFLEXEXPAND";
	}
	if (formR->attr2 & GO_VALID)
	{
		addOR(m_buf);
		m_buf += "GRVALID";
	}
	if (formR->attr2 & GO_FTEXT)
	{
		addOR(m_buf);
		m_buf += "GRFTEXT";
	}
	if (formR->attr2 & GO_XXLS)
	{
		addOR(m_buf);
		m_buf += "GRXXSL";
	}
	if (formR->attr & FA_WRAP)
	{
		addOR(m_buf);
		m_buf += "LEAVE";
	}
	if (formR->attr & FA_ENUM)
	{
		addOR(m_buf);
		m_buf += "CELLEDIT";
	}
	if (formR->attr & FA_XDELETE)
	{
		addOR(m_buf);
		m_buf += "CXDELETE";
	}
	if (formR->attr2 & GO_XSIZE)
	{
		addOR(m_buf);
		m_buf += "GRXSIZE";
	}
	if (formR->attr2 & GO_AUTO)
	{
		addOR(m_buf);
		m_buf += "GRAUTOWIDTH";
	}

	writeFile(m_buf);
	space(2);
	m_buf.Empty();
	writeColumns(repR);
	formR->properties |= PR_ENABLE;
	taborder(formR->index);
	ioorder(formR->iorder);
	groupN(fN);
}

void CBuild::srcMemo(int fN)
{
	writeFile("MEMO");
	space(2);
	symbolRect(fN);
	space(2);

	switch(m_pFormItem->getFormPoint(fN)->borders)
	{
	case BD_NONE:
		m_buf = "TP_NONE";	break;
	case BD_RAISED:
	default:
		m_buf = "TP_RAISED";	break;
	}
	writeFile(m_buf);
	space(2);

	dataType(IO_MIXED);
	m_buf.Empty();
	space(2);

	switch (m_pFormItem->getFormPoint(fN)->iok)
	{
	case EIO_INPUT:
		writeFile("EDIT_INPUT");	break;
	case EIO_INOUT:
		writeFile("EDIT_INOUT");	break;
	case EIO_OUTPUT:
		writeFile("EDIT_OUTPUT");	break;
	}

	m_buf.Empty();
	if (m_pFormItem->getFormPoint(fN)->fStyle)
	{
		space(2);
		m_buf.Format("FONT(\"%s\", %d, %s)", m_pFormItem->getFormStr(fN, "fName"), m_pFormItem->getFormPoint(fN)->fPoint, fontStyle[m_pFormItem->getFormPoint(fN)->fStyle]);
		writeFile(m_buf);
	}
	space(2);
	color(m_pFormItem->getFormPoint(fN)->pRGB, m_pFormItem->getFormPoint(fN)->tRGB);
	space(2);
	attributes(m_pFormItem->getFormPoint(fN)->attr);
	space(2);

	if (m_pFormItem->getFormPoint(fN)->initC)
	{
		writeFile("CURSOR");
		space(2);
	}

	m_buf.Format("CHARS(%d)", m_pFormItem->getFormPoint(fN)->keys);
	writeFile(m_buf);	space(2);
	writeFile("FA_HAN");
	taborder(m_pFormItem->getFormPoint(fN)->index);
	ioorder(m_pFormItem->getFormPoint(fN)->iorder);
	groupN(fN);
}

void CBuild::srcTable(int fN)
{
	struct	_repR*	repR;

	writeFile("TABLE");
	space(2);
	symbolRect(fN);
	space(2);

	repR = (struct _repR *)m_pFormItem->getFormPoint(fN)->auxR;
	repR->cols = 1;
	m_buf.Format("MATRIX(%d, %d)", repR->rows, repR->cols);
	writeFile(m_buf);	space(2);
	m_buf.Format("LINECOLOR(%d)", m_pFormItem->getFormPoint(fN)->bRGB);
	writeFile(m_buf);	space(2);
	m_buf.Format("CELLH(%d)", m_pFormItem->getFormPoint(fN)->keys);
	writeFile(m_buf);	space(2);
	if (m_pFormItem->getFormPoint(fN)->attr2 & GO_AUTOSIZE)
	{
		writeFile("AUTOSIZE");	space(2);
	}

	if (m_pFormItem->getFormPoint(fN)->fStyle)
	{
		space(2);
		m_buf.Format("FONT(\"%s\", %d, %s)", m_pFormItem->getFormStr(fN, "fName"), m_pFormItem->getFormPoint(fN)->fPoint, fontStyle[m_pFormItem->getFormPoint(fN)->fStyle]);
		writeFile(m_buf);
	}

	m_buf.Empty();
	if (m_pFormItem->getFormPoint(fN)->attr2 & GO_MULTI)
	{
		addOR(m_buf);
		m_buf += "MULTI";
	}
	if (m_pFormItem->getFormPoint(fN)->attr2 & GO_XHEAD)
	{
		addOR(m_buf);
		m_buf += "NOHEAD";
	}
	if (m_pFormItem->getFormPoint(fN)->attr2 & GO_XVLINE)
	{
		addOR(m_buf);
		m_buf += "XVLINE";
	}
	if (m_pFormItem->getFormPoint(fN)->attr2 & GO_XHLINE)
	{
		addOR(m_buf);
		m_buf += "XHLINE";
	}
	if (m_pFormItem->getFormPoint(fN)->attr & FA_XDELETE)
	{
		addOR(m_buf);
		m_buf += "CXDELETE";
	}
	if (m_pFormItem->getFormPoint(fN)->properties & PR_CHECKED)
	{
		addOR(m_buf);
		m_buf += "ISFOCUS";
	}
	writeFile(m_buf);
	space(2);
	m_buf.Empty();
	switch (m_pFormItem->getFormPoint(fN)->iok)
	{
	case EIO_OUTPUT:
		writeFile("EDIT_OUTPUT");	break;
	case EIO_NOP:
	default:
		writeFile("EDIT_NOP");		break;
	}
	groupN(fN);
	space(2);
	writeRows(repR);
	ioorder(m_pFormItem->getFormPoint(fN)->iorder);
}

void CBuild::srcResizeBar(int fN)
{
	writeFile("RESIZE");
	space(2);
	symbolRect(fN);
	space(2);
	m_buf.Format("SPLIT(%d, %d)", m_pFormItem->getFormPoint(fN)->type, m_pFormItem->getFormPoint(fN)->iok);
	writeFile(m_buf);
	taborder(m_pFormItem->getFormPoint(fN)->index);
	groupN(fN);
}

void CBuild::srcTreeView(int fN)
{
	writeFile("TREEVIEW");
	space(2);
	symbolRect(fN);
	space(2);

	m_buf.Format("LEN(%d)", m_pFormItem->getFormPoint(fN)->size);
	writeFile(m_buf);
	space(2);
	dataType(IO_ALPHA);

	attributes(m_pFormItem->getFormPoint(fN)->attr);
	space(2);
	m_buf.Empty();
	if (m_pFormItem->getFormPoint(fN)->fStyle)
	{
		space(2);
		m_buf.Format("FONT(\"%s\", %d, %s)", m_pFormItem->getFormStr(fN, "fName"), m_pFormItem->getFormPoint(fN)->fPoint, fontStyle[m_pFormItem->getFormPoint(fN)->fStyle]);
		writeFile(m_buf);
	}
	space(2);
	if (m_pFormItem->getFormPoint(fN)->properties & PR_CHECKED)
	{
		writeFile("MULTISEL");
		space(2);
	}
	color(m_pFormItem->getFormPoint(fN)->pRGB, m_pFormItem->getFormPoint(fN)->tRGB);
	ioorder(m_pFormItem->getFormPoint(fN)->iorder);
	groupN(fN);
}

void CBuild::srcSheet(int fN)
{
	writeFile("SHEET");
	space(2);
	symbolRect(fN);
	space(2);

	m_buf.Format("LEN(%d)", m_pFormItem->getFormPoint(fN)->size);
	writeFile(m_buf);
	space(2);
	m_buf.Format("COUNT(%d)", m_pFormItem->getFormPoint(fN)->keys);
	writeFile(m_buf);       space(2);
	writeFile("EDIT_INPUT");space(2);
	dataType(IO_ALPHA);

	addOR(m_buf);
	switch (m_pFormItem->getFormPoint(fN)->type)
	{
	case SHT_SHEET:
		m_buf += "SHT_SHEET";		break;
	case SHT_BORDER:
	default:
		m_buf += "SHT_BORDER";		break;
	}
	addOR(m_buf);
	alignment(m_pFormItem->getFormPoint(fN)->alignment);
	space(2);
	attributes(m_pFormItem->getFormPoint(fN)->attr);
	space(2);
	m_buf.Empty();
	if (m_pFormItem->getFormPoint(fN)->fStyle)
	{
		space(2);
		m_buf.Format("FONT(\"%s\", %d, %s)", m_pFormItem->getFormStr(fN, "fName"), m_pFormItem->getFormPoint(fN)->fPoint, fontStyle[m_pFormItem->getFormPoint(fN)->fStyle]);
		writeFile(m_buf);
	}
	space(2);
	color(m_pFormItem->getFormPoint(fN)->pRGB, m_pFormItem->getFormPoint(fN)->tRGB);
	ioorder(m_pFormItem->getFormPoint(fN)->iorder);
	groupN(fN);
}

void CBuild::srcUserOb(int fN)
{
	writeFile("USEROB");
	space(2);
	symbolRect(fN);
	space(2);
	attributes(m_pFormItem->getFormPoint(fN)->attr);	space(2);
	m_buf.Format("\"%s\"", m_pFormItem->getFormStr(fN, "str"));
	writeFile(m_buf);
}

void CBuild::srcBrowser(int fN)
{
	writeFile("BROWSER");
	space(2);
	symbolRect(fN);
	space(2);

	switch (m_pFormItem->getFormPoint(fN)->iok)
	{
	case EIO_NOP:
	default:
		writeFile("EDIT_NOP");		break;
	case EIO_OUTPUT:
		writeFile("EDIT_OUTPUT");	break;
	}

	if (!m_pFormItem->getFormStr(fN, "str").IsEmpty())
	{
		space(2);
		m_buf.Format("URL(\"%s\")", m_pFormItem->getFormStr(fN, "str"));
		writeFile(m_buf);
	}

	if (m_pFormItem->getFormPoint(fN)->properties & PR_CHECKED)
	{
		space(2);
		writeFile("HASTOOL");
	}
	space(2);
	attributes(m_pFormItem->getFormPoint(fN)->attr);
	if (m_pFormItem->getFormPoint(fN)->iok == EIO_OUTPUT)
		ioorder(m_pFormItem->getFormPoint(fN)->iorder);
	groupN(fN);
}

void CBuild::srcControl(int fN)
{
	CRect	fmRc;

	writeFile("CONTROL");
	space(2);
	fmRc.SetRect(m_pFormItem->getFormPoint(fN)->left, m_pFormItem->getFormPoint(fN)->top, m_pFormItem->getFormPoint(fN)->right, m_pFormItem->getFormPoint(fN)->bottom);
	symbolRect(fN, fmRc);
	space(2);

	attributes(m_pFormItem->getFormPoint(fN)->attr);	space(2);
	m_buf.Format("\"%s\"", m_pFormItem->getFormStr(fN, "str"));
	writeFile(m_buf);	space(2);
	m_buf.Format("\"%s\"", m_pFormItem->getFormStr(fN, "str2"));
	writeFile(m_buf);

	space(2);
	m_buf.Format("LEN(%d)", m_pFormItem->getFormPoint(fN)->size);
	writeFile(m_buf);

	space(2);
	switch (m_pFormItem->getFormPoint(fN)->iok)
	{
	case EIO_INPUT:
		writeFile("EDIT_INPUT");	break;
	case EIO_INOUT:
		writeFile("EDIT_INOUT");	break;
	case EIO_OUTPUT:
		writeFile("EDIT_OUTPUT");	break;
	case EIO_NOP:
	default:
		writeFile("EDIT_NOP");		break;
	}

	if (m_pFormItem->getFormPoint(fN)->fStyle)
	{
		space(2);
		m_buf.Format("FONT(\"%s\", %d, %s)", m_pFormItem->getFormStr(fN, "fName"), m_pFormItem->getFormPoint(fN)->fPoint, fontStyle[m_pFormItem->getFormPoint(fN)->fStyle]);
		writeFile(m_buf);
	}

	space(2);
	color(m_pFormItem->getFormPoint(fN)->pRGB, m_pFormItem->getFormPoint(fN)->tRGB);

	taborder(m_pFormItem->getFormPoint(fN)->index);
	if (m_pFormItem->getFormPoint(fN)->iok != EIO_NOP)
		ioorder(m_pFormItem->getFormPoint(fN)->iorder);
	groupN(fN);
}

void CBuild::symbolRect(int fN, CRect fmRc)
{
	if (fmRc.IsRectEmpty())
	{
		m_buf.Format("%s(%d, %d, %d, %d)", m_pFormItem->getFormStr(fN, "name"), m_pFormItem->getFormPoint(fN)->left, m_pFormItem->getFormPoint(fN)->top,
				m_pFormItem->getFormPoint(fN)->right, m_pFormItem->getFormPoint(fN)->bottom);
	}
	else
		m_buf.Format("%s(%d, %d, %d, %d)", m_pFormItem->getFormStr(fN, "name"), fmRc.left, fmRc.top, fmRc.right, fmRc.bottom);

	writeFile(m_buf);
}

void CBuild::attributes(DWORD attr)
{
	m_buf.Empty();
	if (attr & FA_COMMA)
	{
		addOR(m_buf);
		m_buf += "FA_COMMA";
	}
	if (attr & FA_RIGHT)
	{
		addOR(m_buf);
		m_buf += "FA_RIGHT";
	}
	if (attr & FA_PASSWD)
	{
		addOR(m_buf);
		m_buf += "FA_PASSWD";
	}
	if (attr & FA_ZEROSUP)
	{
		addOR(m_buf);
		m_buf += "FA_ZEROSUP";
	}
	if (attr & FA_SEND)
	{
		addOR(m_buf);
		m_buf += "FA_SEND";
	}
	if (attr & FA_PROTECT)
	{
		addOR(m_buf);
		m_buf += "FA_PROTECT";
	}
	if (attr & FA_WRAP)
	{
		addOR(m_buf);
		m_buf += "FA_WRAP";
	}
	if (attr & FA_MAND)
	{
		addOR(m_buf);
		m_buf += "FA_MAND";
	}
	if (attr & FA_HAN)
	{
		addOR(m_buf);
		m_buf += "FA_HAN";
	}
	if (attr & FA_SKIP)
	{
		addOR(m_buf);
		m_buf += "FA_SKIP";
	}
	if (attr & FA_CORGB)
	{
		addOR(m_buf);
		m_buf += "FA_CORGB";
	}
	if (attr & FA_DOMINO)
	{
		addOR(m_buf);
		m_buf += "FA_DOMINO";
	}
	if (attr & FA_REF)
	{
		addOR(m_buf);
		m_buf += "FA_REF";
	}
	if (attr & FA_DATE)
	{
		addOR(m_buf);
		m_buf += "FA_DATE";
	}
	if (attr & FA_ZERO)
	{
		addOR(m_buf);
		m_buf += "FA_ZERO";
	}
	if (attr & FA_FLASH)
	{
		addOR(m_buf);
		m_buf += "FA_FLASH";
	}
	if (attr & FA_HORS)
	{
		addOR(m_buf);
		m_buf += "FA_HORS";
	}
	if (attr & FA_FLOAT)
	{
		addOR(m_buf);
		m_buf += "FA_FLOAT";
	}
	if (attr & FA_XFLOAT)
	{
		addOR(m_buf);
		m_buf += "FA_XFLOAT";
	}
	if (attr & FA_UPPER)
	{
		addOR(m_buf);
		m_buf += "FA_UPPER";
	}
	if (attr & FA_DRAG)
	{
		addOR(m_buf);
		m_buf += "FA_DRAG";
	}
	if (attr & FA_NOR)
	{
		addOR(m_buf);
		m_buf += "FA_NOR";
	}
	if (attr & FA_ENUM)
	{
		addOR(m_buf);
		m_buf += "FA_ENUM";
	}
	if (attr & FA_XDELETE)
	{
		addOR(m_buf);
		m_buf += "FA_XDELETE";
	}
	if (attr & FA_CONTRAST)
	{
		addOR(m_buf);
		m_buf += "FA_CONTRAST";
	}
	if (attr & FA_PASS)
	{
		addOR(m_buf);
		m_buf += "FA_PASS";
	}
	writeFile(m_buf);
	m_buf.Empty();
}

void CBuild::subattributes(DWORD attr, DWORD attr2)
{
	m_buf.Empty();
	if (attr & FA_ZEROSUP)
	{
		switch (attr2 & SA_ZEROSUP)
		{
		case SA_SPACE:
			addOR(m_buf);
			m_buf += "SA_SPACE";
			break;
		case SA_ASTER:
			addOR(m_buf);
			m_buf += "SA_ASTER";
			break;
		case SA_ZERO:
			addOR(m_buf);
			m_buf += "SA_ZERO";
			break;
		default:
			break;
		}
	}

	if (attr & FA_CORGB)
	{
		switch (attr2 & SA_CORGB)
		{
		case SA_CORAW:
			addOR(m_buf);
			m_buf += "SA_CORAW";
			break;
		case SA_COSUP:
			addOR(m_buf);
			m_buf += "SA_COSUP";
			break;
		case SA_COSIGN:
			addOR(m_buf);
			m_buf += "SA_COSIGN";
			break;
		default:
			break;
		}
	}

	if (attr & FA_SEND)
	{
		switch (attr2 & SA_SEND)
		{
		case SA_ALL:
			addOR(m_buf);
			m_buf += "SA_ALL";
			break;
		case SA_THIS:
			addOR(m_buf);
			m_buf += "SA_THIS";
			break;
		default:
			break;
		}
	}

	if (attr & FA_DATE)
	{
		switch (attr2 & SA_DATE)
		{
		case SA_AUTO:
			addOR(m_buf);
			m_buf += "SA_AUTO";
			break;
		case SA_YYYYMM:
			addOR(m_buf);
			m_buf += "SA_YYYYMM";
			break;
		case SA_YYMM:
			addOR(m_buf);
			m_buf += "SA_YYMM";
			break;
		case SA_MMDD:
			addOR(m_buf);
			m_buf += "SA_MMDD";
			break;
		default:
			break;
		}
	}

	if (attr & FA_CONTRAST)
	{
		switch (attr2 & SA_CONTRAST)
		{
		case SA_TEXT:
			addOR(m_buf);
			m_buf += "SA_TEXT";
			break;
		case SA_PAINT:
			addOR(m_buf);
			m_buf += "SA_PAINT";
			break;
		default:
			break;
		}
	}

	writeFile(m_buf);
	m_buf.Empty();
}

void CBuild::color(DWORD pRGB, DWORD tRGB)
{
	CString	tmps;

	space(2);
	m_buf.Format("COLOR(%d, %d)", pRGB, tRGB);
	writeFile(m_buf);
	m_buf.Empty();
}

void CBuild::taborder(WORD idx)
{
	CString	tmps;

	space(2);
	m_buf.Format("TABORDER(%d)", idx);
	writeFile(m_buf);
	m_buf.Empty();
}

void CBuild::ioorder(WORD idx)
{
	CString	tmps;

	space(2);
	m_buf.Format("IOORDER(%d)", idx);
	writeFile(m_buf);
	m_buf.Empty();
}

void CBuild::groupN(WORD idx)
{
	if (m_pFormItem->getFormPoint(idx)->group)
	{
		CString	tmps;
		space(2);
		m_buf.Format("GROUPN(%d)", m_pFormItem->getIdxOfPtr(m_pFormItem->getFormPoint(idx)->group));
		writeFile(m_buf);
		m_buf.Empty();
	}
}

void CBuild::dataType(BYTE type)
{
	switch (type)
	{
	case IO_NUM:
		m_buf = "IO_NUM";	break;
	case IO_ALPHA:
		m_buf = "IO_ALPHA";	break;
	case IO_HAN:
		m_buf = "IO_HAN";	break;
	case IO_MIXED:
		m_buf = "IO_MIXED";	break;
	case IO_DIGIT:
		m_buf = "IO_DIGIT";	break;
	default:
		m_buf.Empty();		break;
	}
	writeFile(m_buf);
	m_buf.Empty();
}

void CBuild::alignment(BYTE align)
{
	switch (align)
	{
	case AL_LEFT:
		m_buf += "AL_LEFT";	break;
	case AL_RIGHT:
		m_buf += "AL_RIGHT";	break;
	case AL_TOP:
		m_buf += "AL_TOP";	break;
	case AL_BOTTOM:
		m_buf += "AL_BOTTOM";	break;
	case AL_TILE:
		m_buf += "AL_TILE";	break;
	case AL_STRETCH:
		m_buf += "AL_STRETCH";	break;
	case AL_CENTER:
	default:
		m_buf += "AL_CENTER";	break;
	}
	writeFile(m_buf);
	m_buf.Empty();
}

void CBuild::hintType(BYTE isHint)
{
	switch(isHint)
	{
	case IH_NOP:
		m_buf += "IH_NONE)";	break;
	case IH_STRING:
		m_buf += "IH_STRING)";	break;
	case IH_REF:
		m_buf += "IH_REF)";	break;
	case IH_MAP:
		m_buf += "IH_MAP)";	break;
	}
	writeFile(m_buf);
	m_buf.Empty();
}

void CBuild::writeEtc(int fN)
{
	bool	etc = false;
	int prop = m_pFormItem->getFormPoint(fN)->properties;

	m_buf.Empty();
	if (prop & PR_ENABLE)
	{
		if (etc)
		{
			newline(true);
			etc = true;
		}
		space(2);
		writeFile("ENABLE");
	}

	if (prop & PR_VISIBLE)
	{
		if (!etc)
		{
			newline(true);
			etc = true;
		}
		space(2);
		writeFile("VISIBLE");
	}

	if ((prop & PR_CHECKED) && m_pFormItem->getFormPoint(fN)->kind == FM_BUTTON 
		&& (m_pFormItem->getFormPoint(fN)->type == BTN_RADIO || m_pFormItem->getFormPoint(fN)->type == BTN_CHECK))
	{
		if (!etc)
		{
			newline(true);
			etc = true;
		}
		space(2);
		writeFile("CHECKED");
	}

	if ((prop & PR_VERT) && (m_pFormItem->getFormPoint(fN)->kind == FM_LABEL ||
		m_pFormItem->getFormPoint(fN)->kind == FM_BUTTON || m_pFormItem->getFormPoint(fN)->kind == FM_PANEL))
	{
		if (!etc)
		{
			newline(true);
			etc = true;
		}
		space(2);
		writeFile("VERTICAL");
	}

	if (prop & PR_ACCN && m_pFormItem->getFormPoint(fN)->kind == FM_COMBO)
	{
		if (!etc)
		{
			newline(true);
			etc = true;
		}
		space(2);
		writeFile("ACCCBO");
	}

	if (prop & PR_TRANSPARENT)
	{
		if (!etc)
		{
			newline(true);
			etc = true;
		}
		space(2);
		writeFile("TRANSPARENT");
	}
	if ((prop & PR_IMAGE)
		&& (m_pFormItem->getFormPoint(fN)->kind == FM_PANEL || m_pFormItem->getFormPoint(fN)->kind == FM_BUTTON))
	{
		if (!etc)
		{
			newline(true);
			etc = true;
		}
		space(2);
		writeFile("IMAGE");
	}

	if ((prop & PR_IMAGETEXT)
		&& (m_pFormItem->getFormPoint(fN)->kind == FM_PANEL || m_pFormItem->getFormPoint(fN)->kind == FM_BUTTON))
	{
		if (!etc)
		{
			newline(true);
			etc = true;
		}
		space(2);
		writeFile("IMAGETEXT");
	}

	if ((prop & PR_LINES) && m_pFormItem->getFormPoint(fN)->kind == FM_TREE)
	{
		if (!etc)
		{
			newline(true);
			etc = true;
		}
		space(2);
		writeFile("HASLINE");
	}

	if (prop & PR_HOVER)
	{
		if (!etc)
		{
			newline(true);
			etc = true;
		}
		space(2);
		writeFile("HOVER");
	}

	if (prop & PR_SORT && m_pFormItem->getFormPoint(fN)->kind == FM_GRID)
	{
		if (!etc)
		{
			newline(true);
			etc = true;
		}
		space(2);
		writeFile("SORT");
	}

	if (m_pFormItem->getFormPoint(fN)->kind != FM_GRID && 
		m_pFormItem->getFormPoint(fN)->kind != FM_TABLE &&
		m_pFormItem->getFormPoint(fN)->kind != FM_MEMO)
	{
		if (!etc)
		{
			newline(true);
			etc = true;
		}
		space(2);
		m_buf.Empty();
		m_buf = "HINTTYPE(";
		hintType(m_pFormItem->getFormPoint(fN)->isHint);
	}

	if (!m_pFormItem->getFormStr(fN, "help").IsEmpty())
	{
		if (!etc)
			newline(true);

		space(2);
		m_buf.Format("HELPTEXT(\"%s\")", m_pFormItem->getFormStr(fN, "help"));
		writeFile(m_buf);
	}
	newline(false);
}

void CBuild::writeAuxiliary(int fN)
{
	m_buf.Empty();
	if ((m_pFormItem->getFormPoint(fN)->properties & PR_IMAGE) || (m_pFormItem->getFormPoint(fN)->properties & PR_IMAGETEXT))
	{
		addOR(m_buf);
		if (m_pFormItem->getFormPoint(fN)->properties & PR_IMAGETEXT)
			m_buf += "IMAGETEXT";
		else
			m_buf += "IMAGE";
		addOR(m_buf);
		alignment(m_pFormItem->getFormPoint(fN)->alignImage);
	}
	space(2);
}

void CBuild::writeColumns(struct _repR* repR)
{
	CString	tmps;

	CCellItem *cellItem = new CCellItem(repR);
	for (int ii = 0; ii < repR->cols; ii++)
	{
		struct _cellR *cellR = cellItem->getCellPoint(ii);
		newline(true);
		writeFile("\t");
		m_buf.Format("COLINFO(\"%s\")", cellR->name);
		writeFile(m_buf);
		space(2);
		m_buf.Format("LEN(%d)", cellR->size);
		writeFile(m_buf);
		space(2);
		m_buf.Format("WIDTH(%d)", cellR->width);
		writeFile(m_buf);
		space(2);
		if (cellR->kind == CK_COMBO)
		{
			m_buf.Format("MATCHKEY(%s)", keyMap[cellR->vals2]);
			writeFile(m_buf);
			space(2);
		}
		if (cellR->head[0] != NULL)
		{
			m_buf.Format("HEAD(\"%s\")", cellR->head);
			writeFile(m_buf);
			space(2);
		}

		m_buf = "COLALIGN(";
		switch (cellR->alignH)
		{
		case AL_LEFT:
			m_buf += "AL_LEFT";          break;
		case AL_RIGHT:
			m_buf += "AL_RIGHT";         break;
		case AL_CENTER:
		default:
			m_buf += "AL_CENTER";        break;
		}
		m_buf += ", ";
		if (cellR->kind == CK_BUTTON && (cellR->properties & PR_IMAGE || cellR->properties & PR_IMAGETEXT))
		{
			switch (cellR->alignI)
			{
			case AL_CENTER:
				m_buf += "AL_CENTER";        break;
			case AL_TILE:
				m_buf += "AL_TILE";          break;
			case AL_STRETCH:
			default:
				m_buf += "AL_STRETCH";         break;
			}
		}
		else
		{
			switch (cellR->alignC)
			{
			case AL_CENTER:
				m_buf += "AL_CENTER";        break;
			case AL_LEFT:
				m_buf += "AL_LEFT";          break;
			case AL_RIGHT:
			default:
				m_buf += "AL_RIGHT";         break;
			}
		}
		m_buf += ")";
		writeFile(m_buf);
		
		space(2);
		m_buf = "COLHINTTYPE(";
		hintType(cellR->isHint);
		space(2);

		if (strlen(cellR->onHint))
		{
			m_buf.Format("COLHELPTEXT(\"%s\")", cellR->onHint);
			writeFile(m_buf);
			space(2);
		}

		m_buf.Format("HEADCOLOR(%d, %d)", cellR->hpRGB, cellR->htRGB);
		writeFile(m_buf);
		space(2);

		if (cellR->vals)
		{
			cellR->attr |= FA_FLOAT;
			m_buf.Format("FLOAT(%d)", cellR->vals);
			writeFile(m_buf);
			space(2);
		}
		else
			cellR->attr &= ~FA_FLOAT;

		dataType(cellR->type);
		addOR(m_buf);
		switch (cellR->iok)
		{
		case EIO_INPUT:
			m_buf += "EDIT_INPUT";	break;
		case EIO_INOUT:
			m_buf += "EDIT_INOUT";	break;
		case EIO_OUTPUT:
			m_buf += "EDIT_OUTPUT";	break;
		case EIO_NOP:
		default:
			m_buf += "EDIT_NOP";	break;
			break;
		}
		writeFile(m_buf);
		space(2);

		attributes(cellR->attr);
		subattributes(cellR->attr, cellR->attr2);
		space(2);
		color(cellR->pRGB, cellR->tRGB);
		space(2);
		if (cellR->auxS[0] != NULL)
		{
			m_buf.Format("DOM(%s)", cellR->auxS);
			writeFile(m_buf);
			space(2); m_buf.Empty();
		}

		if (cellR->refS[0] != NULL)
		{
			m_buf.Format("REFERENCE(%s)", cellR->refS);
			writeFile(m_buf);
			space(2); m_buf.Empty();
		}

		if (cellR->properties & PR_IMAGE)
		{
			space(2);
			writeFile("COLUMN_IMAGE");
		}

		if (cellR->properties & PR_IMAGETEXT)
		{
			space(2);
			writeFile("COLUMN_IMGTEXT");
		}

		if (cellR->properties & PR_VISIBLE)
		{
			space(2);
			writeFile("COLUMN_VISIBLE");
		}

		if (cellR->properties & PR_SUPRESS)
		{
			space(2);
			writeFile("SUPRESS");
		}

		if (cellR->properties & PR_EDIT)
		{
			space(2);
			writeFile("COLUMN_SHOWFORM");
		}

		if (cellR->properties & PR_SORT)
		{
			space(2);
			writeFile("COLUMN_SORT");
		}

		if (cellR->properties & PR_CHECKED)
		{
			space(2);
			writeFile("COLUMN_EXCHECKE");
		}

		if (cellR->properties & PR_BOLD)
		{
			space(2);
			writeFile("COLUMN_BOLD");
		}

		switch (cellR->kind)
		{
		case CK_BUTTON:
			space(2);
			writeFile("IBUTTON");	break;
		case CK_COMBO:
			space(2);	
			writeFile("ICOMBO");	break;
		case CK_CHECK:
			space(2);
			writeFile("ICHECK");	break;
		case CK_GRAPH:
			space(2);
			writeFile("IGRAPH");	break;
		case CK_NORMAL:
			space(2);
			writeFile("INORMAL");	break;
		default:
			break;
		}

		switch (cellR->isEdit) 
		{
		case IE_EDIT:
			space(2);
			m_buf.Format("EDITFORMAT(\"%s\")", cellR->editS);
			writeFile(m_buf);
			m_buf.Empty();
			break;
		case IE_CORGB:
			space(2);
			m_buf.Format("IFRGB(%d, %d)", HIWORD(cellR->onEdit),
				LOWORD(cellR->onEdit));
			writeFile(m_buf);
			m_buf.Empty();
			break;
		case IE_NOP:
		default:
			break;
		}
		
		space(2);
		if (cellR->kind == CK_COMBO && cellR->keys != 0)
		{
			m_buf.Format("COUNT(%d)", cellR->keys);
			writeFile(m_buf);	space(2);
			if (cellR->keys != 0)
			{
				CString	tmps, data;
				int	count = cellR->keys;
				
				m_buf = "DISPLAY(";
				data = cellR->dat;
				int jj = 0;
				for (jj = 0; jj < count; jj++)
				{
					tmps.Format("\"%s\"", Parser(data, "\t"));
					if (jj < count - 1)
						tmps += ", ";
					
					m_buf += tmps;
				}
				m_buf += ")";
				writeFile(m_buf);	space(2);
				
				m_buf = "DATA(";
				data = cellR->str;
				for (jj = 0; jj < count; jj++)
				{
					tmps.Format("\"%s\"", Parser(data, "\t"));
					if (jj < count - 1)
						tmps += ", ";
					
					m_buf += tmps;
				}
				m_buf += ")";
				writeFile(m_buf);	space(2);
				m_buf.Empty();
			}
		}

		if (cellR->kind == CK_CHECK && cellR->str2[0] != '\0')
		{
			m_buf.Format("CHECKSTR(\"%s\")", cellR->str2);
			writeFile(m_buf);	space(2);
			m_buf.Empty();
		}
		
		if (cellR->kind == CK_BUTTON)
		{
			if (!(cellR->properties & PR_IMAGE))
			{
				m_buf.Format("BUTTONSTR(\"%s\")", cellR->txt);
				writeFile(m_buf);	space(2);
				m_buf.Empty();
			}
			if (cellR->properties & PR_IMAGE || cellR->properties & PR_IMAGETEXT)
			{
				CString tmps = cellR->str3;
				int	offs = tmps.ReverseFind('\\');
				if (offs != -1)
					tmps = tmps.Mid(offs+1);
				m_buf.Format("BUTTONIMG(\"%s\")", tmps);
				writeFile(m_buf);	space(2);
				m_buf.Empty();
			}
		}
	}
	delete cellItem;
}

void CBuild::writeRows(struct _repR* repR)
{
	CString	tmps;

	CCellItem *cellItem = new CCellItem(repR);
	for (int ii = 0; ii < repR->rows; ii++)
	{
		struct _cellR *cellR = cellItem->getCellPoint(ii);
		newline(true);
		writeFile("\t");
		m_buf.Format("COLINFO(\"%s\")", cellR->name);
		writeFile(m_buf);
		space(2);
		m_buf.Format("LEN(%d)", cellR->size);
		writeFile(m_buf);
		space(2);
		m_buf.Format("WIDTH(%d)", cellR->width);
		writeFile(m_buf);
		space(2);
		if (cellR->head[0] != NULL)
		{
			m_buf.Format("HEAD(\"%s\")", cellR->head);
			writeFile(m_buf);
			space(2);
		}

		m_buf = "COLALIGN(";
		switch (cellR->alignH)
		{
		case AL_CENTER:
			m_buf += "AL_CENTER";        break;
		case AL_RIGHT:
			m_buf += "AL_RIGHT";         break;
		case AL_LEFT:
		default:
			m_buf += "AL_LEFT";          break;
		}
		m_buf += ", ";
		switch (cellR->alignC)
		{
		case AL_CENTER:
			m_buf += "AL_CENTER";        break;
		case AL_LEFT:
			m_buf += "AL_LEFT";          break;
		case AL_RIGHT:
		default:
			m_buf += "AL_RIGHT";         break;
		}
		m_buf += ")";
		writeFile(m_buf);
		space(2);

		m_buf.Format("HEADCOLOR(%d, %d)", cellR->hpRGB, cellR->htRGB);
		writeFile(m_buf);
		space(2);

		if (cellR->vals)
		{
			cellR->attr |= FA_FLOAT;
			m_buf.Format("FLOAT(%d)", cellR->vals);
			writeFile(m_buf);
			space(2);
		}
		else
			cellR->attr &= ~FA_FLOAT;

		dataType(cellR->type);
		attributes(cellR->attr);
		subattributes(cellR->attr, cellR->attr2);
		space(2);
		color(cellR->pRGB, cellR->tRGB);
		space(2);

		m_buf = "COLHINTTYPE(";
		hintType(cellR->isHint);
		space(2);

		if (strlen(cellR->onHint))
		{
			m_buf.Format("COLHELPTEXT(\"%s\")", cellR->onHint);
			writeFile(m_buf);
			m_buf.Empty();
			space(2);
		}

		if (cellR->auxS[0] != NULL)
		{
			m_buf.Format("DOM(%s)", cellR->auxS);
			writeFile(m_buf);
			m_buf.Empty();
			space(2);
		}

		if (cellR->refS[0] != NULL)
		{
			m_buf.Format("REFERENCE(%s)", cellR->refS);
			writeFile(m_buf);
			m_buf.Empty();
		}

		if (cellR->properties & PR_VISIBLE)
		{
			space(2);
			writeFile("COLUMN_VISIBLE");
		}

		switch (cellR->isEdit) 
		{
		case IE_EDIT:
			space(2);
			m_buf.Format("EDITFORMAT(\"%s\")", cellR->editS);
			writeFile(m_buf);
			m_buf.Empty();
			break;
		case IE_CORGB:
			space(2);
			m_buf.Format("IFRGB(%d, %d)", HIWORD(cellR->onEdit),
				LOWORD(cellR->onEdit));
			writeFile(m_buf);
			m_buf.Empty();
			break;
		case IE_NOP:
		default:
			break;
		}
	}
	delete cellItem;
}

void CBuild::writeFile(CString buf)
{
	if (m_bSrc)
	{
		m_sSrc += buf;
		return;
	}
	if (!m_bOpen || buf.IsEmpty())
	{
		if (!m_bOpen)
		AfxMessageBox("XError [b4]");
		return;
	}

CString	msg;
	TRY
	{
		m_file.Write((char *)buf.operator LPCTSTR(), buf.GetLength());
	}
	CATCH (CFileException, e)
	{
		msg.Format("XError [b6] [%d]", e->m_cause);
		AfxMessageBox(msg);
	}
	END_CATCH
}

void CBuild::writeDataToFile(char* buf)
{
	CString tmp = CString(buf, strlen(buf));
	int	len = 0;
	while(!tmp.IsEmpty())
	{
		CString sParse = Parser(tmp, "\n");
		sParse.Replace("\r", "");
		if (!sParse.IsEmpty())
		{
			m_buf = sParse;
			newline(false);
		}
		else
		{
			m_buf.Empty();
			newline(false);
		}
	}
	m_buf.Empty();
}

void CBuild::writeScriptData(char* dir)
{
	HANDLE	hFile;
	DWORD	scpSize = 0;
	DWORD	dwReadLen = 0;
	char*	sbuff = NULL;
	BOOL	bRc;

	hFile = CreateFile(dir, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return;

	scpSize = GetFileSize(hFile, NULL);
	if (scpSize == 0xFFFFFFFF)
	{
AfxMessageBox("XError [b2]");
		CloseHandle(hFile);
		return;
	}
	sbuff = new char[scpSize+1];
	bRc = ReadFile(hFile, sbuff, scpSize, &dwReadLen, NULL);
	CloseHandle(hFile);
	if (!bRc || scpSize != dwReadLen)
	{
AfxMessageBox("XError [b3]");
		delete[] sbuff;
		return;
	}
	sbuff[scpSize] = '\0';
	writeDataToFile(sbuff);
	delete[] sbuff;
}

void CBuild::addOR(CString& buf)
{
	buf += '|';
}

void CBuild::brace(bool begin)
{
	if (begin)
		writeFile("{");
	else
		writeFile("}");

	newline(false);
}

void CBuild::space(int count)
{
	CString	sp = CString(' ', count);
	writeFile(sp);
}

void CBuild::newline(bool iscontinue)
{
	if (!m_buf.IsEmpty())
	{
		writeFile(m_buf);
		m_buf.Empty();
	}

	if (iscontinue)
	{
		space(2);
		writeFile("&");
	}
	m_buf = (char)0x0d;
	m_buf += (char)0x0a;
	writeFile(m_buf);
}

CString CBuild::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
			return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

