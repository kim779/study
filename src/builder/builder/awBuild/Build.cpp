// Build.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "awBuild.h"
#include "Build.h"

#include "../h/mapvar.h"
#include "../h/mapform.h"
#include "../awWcc/libWcc.h"
#include "../h/fmProperties.h"

//#include "../awTool/awFormCtrl.h"
#include "../awTool/awObjectLoad.h"
//#include "../h/fmObject.h"

const char mapMethod[4][5]  = {"FORM", "MENU", "SEND", "PASS"};
const char fontStyle[5][12] = {"", "NORMAL", "ITALIC", "BOLD", "BOLD_ITALIC"};
const char keyMap[21][12]   = {"NOTMAPPING", "PAGEUP", "PAGEDN", "HOME", "END", "F2", "F3", "F4",
				"F5", "F6", "F7", "F8", "F9", "F11", "F12", "NUMPAD+", "NUMPAD-", "NUMPAD/", "NUMPAD*", "KEYUP", "KEYDOWN"};
const char mapKind[3][16]   = {"NORMAL", "TEMPLATE", "PROCEDURES"};

// CBuild
CBuild::CBuild()
{
	m_iTab = 0;
	m_open = false;
	m_mapH = (class mapForm *)NULL;
}

CBuild::~CBuild()
{
	if (m_open)
		m_file.Close();
}

int CBuild::GenerateSource(bool alarm, class mapForm* mapForm)
{
	CString	tmps, strFileName;
	int	len  = (int)strlen(mapForm->mapN);

	if (len <= 0 || len < L_MAPN)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("Map Name Incorrect Error!!"), _T("Message"), MB_ICONEXCLAMATION|MB_OK);
		return -1;
	}

	if (strlen(mapForm->mapdesc) <= 0)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("Map Description Error!!"), _T("Message"), MB_ICONEXCLAMATION|MB_OK);
		return -1;
	}

	strFileName.Format("%s.xml", mapForm->mapN);
	wccSetTempPath(tmps, (char*)strFileName.operator LPCSTR());
	m_open = (m_file.Open(tmps, CFile::modeCreate|CFile::modeWrite)) ? true : false;
	if (!m_open)
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("\nCan't create Source.\n\nMapName Error!"),
					_T("Message"), MB_ICONEXCLAMATION|MB_OK);
		return -1;
	}

	m_getsource = false;
	m_mapH  = mapForm;

	InitTab();
	writeWithNL(_T("<?xml version=\"1.0\" encoding=\"EUC-KR\"?>"));
	writeWithNL(_T("<AXIS>"));
	SetTab(true);
	generateHeader();
	newline();
	generateForm();
	newline();
	generateResource();
	generateTRlayout();
	newline();
	generateRTSSymbol();
	newline();
	generateLayoutEdit();
	newline();
	SetTab(false);
	writeFile(_T("</AXIS>"));
	m_file.Close();
	m_open = false;
	mapForm->source   = true;
	mapForm->modified = false;

	if (alarm)
	{
		tmps.Format(_T("\nCreated Source.\n\nMapName : %s"), m_mapH->mapN);
		::MessageBox(AfxGetMainWnd()->m_hWnd, tmps, _T("Message"), MB_ICONEXCLAMATION|MB_OK);
	}
	return 0;
}

CString CBuild::GetSource(bool notResource, class mapForm* mapForm)
{
	CString	tmps = _T("");

	m_open = false;
	m_getsource = true;
	
	m_mapH  = mapForm;

	m_source = _T("");
	InitTab();
	writeWithNL(_T("<?xml version=\"1.0\" encoding=\"EUC-KR\"?>"));
	writeWithNL(_T("<AXIS>"));
	SetTab(true);
	generateHeader();
	newline();
	generateForm();
	newline();
	generateResource();
	generateTRlayout();
	newline();
	generateRTSSymbol();
	newline();
	generateLayoutEdit();
	newline();
	SetTab(false);
	writeFile(_T("</AXIS>"));
	mapForm->source   = true;
	mapForm->modified = false;

	return m_source;
}

void CBuild::generateHeader()
{
	int	offs;
	bool	first;
	CString	tmps, buf;
	writeWithNL(_T("<GLOBAL_DIVISION>"));
	SetTab(true);
	buf.Format(_T("<MAPNAME>%s</MAPNAME>"), m_mapH->mapN);
	writeWithNL(buf);

	buf.Format(_T("<MEDIA>%d</MEDIA>"), m_mapH->media);
	writeWithNL(buf);

	buf.Format(_T("<MAPSIZE>%d, %d</MAPSIZE>"), m_mapH->height, m_mapH->width);
	writeWithNL(buf);

//	buf.Format(_T("VERSION(%03d)"), m_mapH->vers+1);	// on batch compile (temporary)
	buf.Format(_T("<VERSION>%03d</VERSION>"), m_mapH->vers);
	writeWithNL(buf);

	buf.Format(_T("<MAPKIND>%s</MAPKIND>"), mapKind[m_mapH->mapK]);
	writeWithNL(buf);

	buf.Format(_T("<MAPDESC>\"%s\"</MAPDESC>"), m_mapH->mapdesc);
	writeWithNL(buf);
	
	if (strlen(m_mapH->publicR) == 0)
		buf = _T("<MAPPUBLIC>\";\"</MAPPUBLIC>");
	else
		buf.Format(_T("<MAPPUBLIC>\"%s\"</MAPPUBLIC>"), m_mapH->publicR);
	writeWithNL(buf);

	if (m_mapH->fName[0] != NULL)
	{
		buf.Format(_T("<FONT>\"%s\", %d, %s</FONT>"), m_mapH->fName, m_mapH->fPoint, fontStyle[m_mapH->fStyle]);
		writeWithNL(buf);
	}

	first = true;
	if (m_mapH->options)
	{
		buf = _T("<OPTIONS>");
		if (m_mapH->options & OP_OOP)
		{
			if (!first)
				addOR(buf);
			buf += _T("FMS");
			first = false;
		}
		if (m_mapH->options & OP_CLS)
		{
			if (!first)
				addOR(buf);
			buf += _T("FORMAT");
			first = false;
		}
		if (m_mapH->options & OP_CNV)
		{
			if (!first)
				addOR(buf);
			buf += _T("CODECNV");
			first = false;
		}
		if (m_mapH->options & OP_FLASH)
		{
			if (!first)
				addOR(buf);
			buf += _T("FLASH");
			first = false;
		}
		if (m_mapH->options & OP_ENC)
		{
			if (!first)
				addOR(buf);
			buf += _T("ENCODE");
			first = false;
		}
		if (m_mapH->options & OP_TABS)
		{
			if (!first)
				addOR(buf);
			buf += _T("TABSEP");
			first = false;
		}
		if (m_mapH->options & OP_CR2TB)
		{
			if (!first)
				addOR(buf);
			buf += _T("CR2TAB");
			first = false;
		}
		if (m_mapH->options & OP_DROP)
		{
			if (!first)
				addOR(buf);
			buf += _T("DROP");
			first = false;
		}
		if (m_mapH->options & OP_USER)
		{
			if (!first)
				addOR(buf);
			buf += _T("USER");
			first = false;
		}
		
		if (m_mapH->options & OP_INTER)
		{
			if (!first)
				addOR(buf);
			buf += _T("INTER");
			first = false;
		}
		if (m_mapH->options & OP_CERTIFY)
		{
			if (!first)
				addOR(buf);
			buf += _T("CERTIFY");
			first = false;
		}
		if (m_mapH->options & OP_OOP2)
		{
			if (!first)
				addOR(buf);
			buf += _T("FMS2");
			first = false;
		}

		if (m_mapH->options & OP_DOMINO)
		{
			if (!first)
				addOR(buf);
			buf += _T("DOMINO");
			first = false;
		}

		if (m_mapH->options & OP_XFONT)
		{
			if (!first)
				addOR(buf);
			buf += _T("XFONT");
			first = false;
		}

		if (m_mapH->options & OP_SAVE)
		{
			if (!first)
				addOR(buf);
			buf += _T("SAVE");
			first = false;
		}
		buf += _T("</OPTIONS>");
		writeWithNL(buf);
	}

	if (m_mapH->caption[0] != NULL)
	{
		buf.Format(_T("<TITLE>\"%s\"</TITLE>"), m_mapH->caption);
		writeWithNL(buf);
	}

	buf = _T("<MAPTYPE>");
	switch (m_mapH->type)
	{
	case VW_NORM:
		buf += _T("NORM");	break;
	case VW_SCROLL:
		buf += _T("SCROLL");	break;
	case VW_RESIZE:
		buf += _T("RESIZE");	break;
	case VW_WND:
		buf += _T("WND");	break;
	case VW_FIXED:
		buf += _T("FIXED");	break;
	case VW_FEX:
		buf += _T("FIXEDEX");	break;
		break;
	}
	buf += _T("</MAPTYPE>");
	writeWithNL(buf);

	if (m_mapH->helpN[0] != NULL)
	{
		buf.Format(_T("<HELPFILE>\"%s\"</HELPFILE>"), m_mapH->helpN);
		writeWithNL(buf);
	}

	if (m_mapH->repeatV != 0)
	{
		buf.Format(_T("<REPEATTIME>%d</REPEATTIME>"), m_mapH->repeatV);
		writeWithNL(buf);
	}

	switch (m_mapH->imageK)
	{
	case IK_BRUSH:
		buf.Format(_T("<BGROUND>BRUSH, %d</BGROUND>"), m_mapH->bkClr);
		writeWithNL(buf);
		break;
	case IK_BMP:
		tmps = (char *)m_mapH->imageV;
		offs = tmps.ReverseFind('\\');;
		if (offs != -1)
			tmps = tmps.Mid(offs+1);
		buf.Format(_T("<BGROUND>IMAGE, \"%s\"</BGROUND>"), tmps);
		writeWithNL(buf);
		break;
	case IK_NOP:
	default:
		break;
	}
	SetTab(false);
	writeFile(_T("</GLOBAL_DIVISION>"));
}

void CBuild::generateForm()
{
	CString	buf;

	writeWithNL(_T("<FORM_DIVISION>"));
	SetTab(true);

	for (int ii = 0; ii < m_mapH->forms.GetCount(); ii++) 
	{
		CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(ii);;
		CformProp*	Prop = ObjLoad->getProp();
		switch (Prop->kind)
		{
		//case FM_SYS:
		//	srcSys(ii);		break;
		case fmLABEL:
			srcLabel(ii);		break;
		case fmBOX:
			srcBox(ii);		break;
		case fmGROUP:
			srcGroup(ii);		break;
		case fmTAB:
			srcTab(ii);		break;
		case fmIMVIEW:
			srcPanel(ii);		break;
		case fmBUTTON:
		case fmRADIO:
		case fmCHECK:
			srcButton(ii);		break;
		case fmBROWSER:
			srcBrowser(ii);		break;
		case fmOBJECT:
			srcObject(ii);		break;
		case fmEDIT:
		case fmOUTPUT:
			srcEdit(ii);		break;
		case fmMEMO:
			srcMemo(ii);		break;
		case fmCOMBO:
			srcCombo(ii);		break;
		case fmGRID:
			srcGrid(ii, fmGRID);		break;
		case fmGRIDEX:
			srcGrid(ii, fmGRIDEX);		break;
		case fmFORM:
			srcForm(ii);		break;
		case fmTABLE:
			srcTable(ii);		break;
		case fmTREE:
			srcTreeView(ii);	break;
		default:
			continue;
		}
		//writeEtc(ii);
	}

	SetTab(false);
	writeFile(_T("</FORM_DIVISION>"));
}

void CBuild::generateLayoutEdit()
{
	CString	buf, tmp, str;

	writeWithNL(_T("<LAYOUTEDIT_DIVISION>"));
	SetTab(true);
	/////////////////////////////////////////////
	if (m_mapH->maplayout != NULL)
	{
		CObjectLoad*	ObjLoad = NULL;
		CformProp*	Prop = NULL;
		CString		sKey, sIndex;
		struct _Layout* layout;
		for (int ii = 0; ii < m_mapH->forms.GetCount(); ii++)
		{
			ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(ii);
			Prop = ObjLoad->getProp();
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			if (m_mapH->maplayout->GetStartPosition() <= 0)
				continue;
			buf.Format("<%s>", Prop->name);
			for (POSITION pos = m_mapH->maplayout->GetStartPosition(); pos;)
			{
				m_mapH->maplayout->GetNextAssoc(pos, sIndex, sKey);
				Prop->layout->Lookup(sKey, (void*&)layout);
				//if (!sKey.CompareNoCase("Original"))
				//	continue;
				buf.AppendFormat("%s,%d,%d,%d,%d,%d^",
					sKey, layout->left, layout->top, layout->right,
					layout->bottom, layout->properties & stVISIBLE ? 1 : 0);

			}
			buf.Delete(buf.GetLength() - 1);
			buf.AppendFormat("</%s>", Prop->name);
			writeWithNL(buf);
			/////////////////////////////////////////////////////////////////////////////////////////////////////
		}
	}
	/////////////////////////////////////////////
	SetTab(false);
	writeFile(_T("</LAYOUTEDIT_DIVISION>"));
}

void CBuild::generateRTSSymbol()
{
	CString	buf, tmp, str;

	writeWithNL(_T("<RTS_DIVISION>"));
	SetTab(true);
	/////////////////////////////////////////////
	tmp = m_mapH->rtsinfo;
	str = parse(tmp, _T("^"));
	tmp = parse(str, _T("\t"));

	CString rtsinfo = CString(m_mapH->rtsinfo);
	while (!rtsinfo.IsEmpty())
	{
		int pos = rtsinfo.Find(_T("^"));
		if (pos != -1)
		{
			str = parse(rtsinfo, _T("^"));
			tmp = parse(str, _T(" "));
			if (!str.IsEmpty())
			{
				buf.Format(_T("<%s>%s %s</%s>"), tmp, tmp, str, tmp);
				writeWithNL(buf);
			}
		}
		else
			rtsinfo = _T("");
	}
	/////////////////////////////////////////////
	SetTab(false);
	writeFile(_T("</RTS_DIVISION>"));
}

void CBuild::generateTRlayout()
{
	CString	buf, tmp, str;

	writeWithNL(_T("<TRLAYOUT_DIVISION>"));
	SetTab(true);
	/////////////////////////////////////////////
	tmp = m_mapH->trinfo;
	str = parse(tmp, _T("^"));
	tmp = parse(str, _T("\t"));

	CString trinfo = CString(m_mapH->trinfo);
	while (!trinfo.IsEmpty())
	{
		int pos = trinfo.Find(_T("^"));
		if (pos != -1)
		{
			str = parse(trinfo, _T("^"));
			tmp = parse(str, _T("\t"));
			if (!str.IsEmpty())
			{
				buf.Format(_T("<%s>%s</%s>"), tmp, str, tmp);
				writeWithNL(buf);
			}
		}
		else
			trinfo = _T("");
	}
	/////////////////////////////////////////////
	SetTab(false);
	writeFile(_T("</TRLAYOUT_DIVISION>"));
}

void CBuild::generateResource()
{
	CString	tmps, buf;

	writeWithNL(_T("<RESOURCE_DIVISION>"));
	SetTab(true);

	if (m_mapH->onDeclaration && m_mapH->onDeclarationN[0] != NULL)
	{
		writeFile(_T("<ONDECLARE>"));
		writeScriptData(m_mapH->onDeclarationN);
		writeFile(_T("</ONDECLARE>"), false);
		writeFile(_T("\r\n"), false);
	}

	if (m_mapH->onInDeclaration && m_mapH->onInDeclarationN[0] != NULL)
	{
		writeFile(_T("<ONINDECLARE>"));
		writeScriptData(m_mapH->onInDeclarationN);
		writeFile(_T("</ONINDECLARE>"), false);
		writeFile(_T("\r\n"), false);
	}

	if (m_mapH->onStart && m_mapH->onStartN[0] != NULL)
	{
		writeFile(_T("<ONSTART>"));
		writeScriptData(m_mapH->onStartN);
		writeFile(_T("</ONSTART>"), false);
		writeFile(_T("\r\n"), false);
	}

	if (m_mapH->onSend && m_mapH->onSendN[0] != NULL)
	{
		writeFile(_T("<ONSEND>"));
		writeScriptData(m_mapH->onSendN);
		writeFile(_T("</ONSEND>"), false);
		writeFile(_T("\r\n"), false);
	}

	if (m_mapH->onReceive && m_mapH->onReceiveN[0] != NULL)
	{
		writeFile(_T("<ONRECEIVE>"));
		writeScriptData(m_mapH->onReceiveN);
		writeFile(_T("</ONRECEIVE>"), false);
		writeFile(_T("\r\n"), false);
	}

	if (m_mapH->onAlert && m_mapH->onAlertN[0] != NULL)
	{
		writeFile(_T("<ONALERT>"));
		writeScriptData(m_mapH->onAlertN);
		writeFile(_T("</ONALERT>"), false);
		writeFile(_T("\r\n"), false);
	}

	if (m_mapH->onService && m_mapH->onServiceN[0] != NULL)
	{
		writeFile(_T("<ONSERVICE>"));
		writeScriptData(m_mapH->onServiceN);
		writeFile(_T("</ONSERVICE>"), false);
		writeFile(_T("\r\n"), false);
	}

	if (m_mapH->onFile && m_mapH->onFileN[0] != NULL)
	{
		writeFile(_T("<ONFILE>"));
		writeScriptData(m_mapH->onFileN);
		writeFile(_T("</ONFILE>"), false);
		writeFile(_T("\r\n"), false);
	}

	if (m_mapH->onSelect && m_mapH->onSelectN[0] != NULL)
	{
		writeFile(_T("<ONSELECT>"));
		writeScriptData(m_mapH->onSelectN);
		writeFile(_T("</ONSELECT>"), false);
		writeFile(_T("\r\n"), false);
	}

	if (m_mapH->onTimer && m_mapH->onTimerN[0] != NULL)
	{
		writeFile(_T("<ONTIMER>"));
		writeScriptData(m_mapH->onTimerN);
		writeFile(_T("</ONTIMER>"), false);
		writeFile(_T("\r\n"), false);
	}

	if (m_mapH->onFocus && m_mapH->onFocusN[0] != NULL)
	{
		writeFile(_T("<ONFOCUS>"));
		writeScriptData(m_mapH->onFocusN);
		writeFile(_T("</ONFOCUS>"), false);
		writeFile(_T("\r\n"), false);
	}

	if (m_mapH->onClose && m_mapH->onCloseN[0] != NULL)
	{
		writeFile(_T("<ONCLOSE>"));
		writeScriptData(m_mapH->onCloseN);
		writeFile(_T("</ONCLOSE>"), false);
		writeFile(_T("\r\n"), false);
	}

	if (m_mapH->onDevice && m_mapH->onDeviceN[0] != NULL)
	{
		writeFile(_T("<ONDEVICE>"));
		writeScriptData(m_mapH->onDeviceN);
		writeFile(_T("</ONDEVICE>"), false);
		writeFile(_T("\r\n"), false);
	}

	if (m_mapH->onApprove && m_mapH->onApproveN[0] != NULL)
	{
		writeFile(_T("<ONAPPROVE>"));
		writeScriptData(m_mapH->onApproveN);
		writeFile(_T("</ONAPPROVE>"), false);
		writeFile(_T("\r\n"), false);
	}

	if (m_mapH->onKey && m_mapH->onKeyN[0] != NULL)
	{
		writeFile(_T("<ONKEY>"));
		writeScriptData(m_mapH->onKeyN);
		writeFile(_T("</ONKEY>"), false);
		writeFile(_T("\r\n"), false);
	}

	if (m_mapH->onChangeLayout && m_mapH->onChangeLayoutN[0] != NULL)
	{
		writeFile(_T("<ONCHANGELAYOUT>"));
		writeScriptData(m_mapH->onChangeLayoutN);
		writeFile(_T("</ONCHANGELAYOUT>"), false);
		writeFile(_T("\r\n"), false);
	}

	if (m_mapH->onFlicking && m_mapH->onFlickingN[0] != NULL)
	{
		writeFile(_T("<ONFLICKING>"));
		writeScriptData(m_mapH->onFlickingN);
		writeFile(_T("</ONFLICKING>"), false);
		writeFile(_T("\r\n"), false);
	}

	CFile	file;
	DWORD	scpSize;
	DWORD	readLen;
	char*	wb;

	char*	click      = new char[1024*32];
	char*	dblclk     = new char[1024*32];
	char*	change     = new char[1024*32];
	char*	charchange = new char[1024*32];
	char*	keychange  = new char[1024*32];

	

	for (int ii = 0; ii < m_mapH->forms.GetCount(); ii++)
	{
		CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(ii);
		CformProp*	Prop = (CformProp*)ObjLoad->getProp();
		if (Prop->name.IsEmpty())
			continue;

		if (!file.Open(Prop->scpN, CFile::modeRead|CFile::typeBinary))
			continue;
		else
		{
			scpSize = (DWORD)file.GetLength();
			if (scpSize <= 0)
			{
				file.Close();
				continue;
			}
			else
			{
				wb = new char[scpSize+1];
				readLen = file.Read(wb, scpSize);
				file.Close();
				if (scpSize != readLen)
				{
					delete[] wb;
					continue;
				}
				else
				{
					click[0]      = NULL;
					dblclk[0]     = NULL;
					change[0]     = NULL;
					charchange[0] = NULL;
					keychange[0]  = NULL;

					if (Prop->onClick != -1)
					{
						CopyMemory(click, &wb[Prop->onClick], Prop->onClickL);
						click[Prop->onClickL] = NULL;
					}
					if (Prop->onDblClk != -1)
					{
						CopyMemory(dblclk, &wb[Prop->onDblClk], Prop->onDblClkL);
						dblclk[Prop->onDblClkL] = NULL;
					}
					if (Prop->onChange != -1)
					{
						CopyMemory(change, &wb[Prop->onChange], Prop->onChangeL);
						change[Prop->onChangeL] = NULL;
					}
					if (Prop->onChar != -1)
					{
						CopyMemory(charchange, &wb[Prop->onChar], Prop->onCharL);
						charchange[Prop->onCharL] = NULL;
					}
					if (Prop->onKey != -1)
					{
						CopyMemory(keychange, &wb[Prop->onKey], Prop->onKeyL);
						keychange[Prop->onKeyL] = NULL;
					}
					delete[] wb;
				}
			}
		}

		//buf.Format(_T("<SCRIPT>"));
		//writeWithNL(buf);
		//SetTab(true);

		//buf.Format(_T("<NAME>%s</NAME>"), Prop->name);
		//writeWithNL(buf);

		int	kind = Prop->kind;
		if (click[0] != NULL && Prop->onClickL != 0 && kind != fmCOMBO)
		{
			buf.Format(_T("<%s_ONCLICK>"), Prop->name);
			writeFile(buf);
			writeDataToFile(click);
			buf.Format(_T("</%s_ONCLICK>"), Prop->name);
			writeFile(buf, false);
			writeFile(_T("\r\n"), false);
		}

		if (dblclk[0] != NULL && Prop->onDblClkL != 0 && kind != fmCOMBO)
		{
			buf.Format(_T("<%s_ONDBLCLK>"), Prop->name);
			writeFile(buf);
			writeDataToFile(dblclk);
			buf.Format(_T("</%s_ONDBLCLK>"), Prop->name);
			writeFile(buf, false);
			writeFile(_T("\r\n"), false);
		}

		if (change[0] != NULL && Prop->onChangeL != 0
			&& (kind == fmEDIT || kind == fmCOMBO || kind == fmGRID || kind == fmGRIDEX || kind == fmMEMO || kind == fmOBJECT))
		{
			buf.Format(_T("<%s_ONCHANGE>"), Prop->name);
			writeFile(buf);
			writeDataToFile(change);
			buf.Format(_T("</%s_ONCHANGE>"), Prop->name);
			writeFile(buf, false);
			writeFile(_T("\r\n"), false);
		}

		if (charchange[0] != NULL && Prop->onCharL != 0 && (kind == fmEDIT || kind == fmGRID || kind == fmGRIDEX))
		{
			buf.Format(_T("<%s_ONCHARCHANGE>>"), Prop->name);
			writeFile(buf);
			writeDataToFile(charchange);
			buf.Format(_T("</%s_ONCHARCHANGE>"), Prop->name);
			writeFile(buf, false);
			writeFile(_T("\r\n"), false);
		}

		if (keychange[0] != NULL && Prop->onKeyL != 0 && (kind == fmGRID || kind == fmGRIDEX || kind == fmEDIT))
		{
			buf.Format(_T("<%s_ONKEYCHANGE>>"), Prop->name);
			writeFile(buf);
			writeDataToFile(keychange);
			buf.Format(_T("</%s_ONKEYCHANGE>"), Prop->name);
			writeFile(buf, false);
			writeFile(_T("\r\n"), false);
		}

		//SetTab(false);
		//buf.Format(_T("</SCRIPT>"));
		//writeWithNL(buf);
	}
	delete[] click;
	delete[] dblclk;
	delete[] change;
	delete[] charchange;
	delete[] keychange;

	SetTab(false);
	writeWithNL(_T("</RESOURCE_DIVISION>"));
}

void CBuild::srcSys(int fN)
{
	CString	buf;

	writeWithNL(_T("<SYS>"));
	SetTab(true);
	symbolRect(fN);

	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(fN);
	CformProp*	Prop = (CformProp*)ObjLoad->getProp();

	writeFile(_T("<TYPE>"));
	switch (Prop->charType) 
	{
	case ctDATE:
		buf = _T("TP_DATE");		break;
	case ctTIME:
		buf = _T("TP_TIME");		break;
	case ctTERM:
		buf = _T("TP_TERM");		break;
	default:
		buf = _T("");			break;
	}
	addOR(buf);
	alignment(Prop->alignment, buf);
	attributes(Prop->attributes);
	writeFile(_T("</TYPE>"), false);
	writeFile(_T("\r\n"), false);
	if (Prop->fontStyle)
	{
		buf.Format(_T("<FONT>\"%s\", %d, %s</FONT>"), Prop->fontName, Prop->fontPoint, fontStyle[Prop->fontStyle]);
		writeWithNL(buf);
	}
	color(Prop->paintColor, Prop->textColor);
	groupN(fN);
	writeEtc(fN);
	SetTab(false);
	writeWithNL(_T("</SYS>"));
}

void CBuild::srcLabel(int fN)
{
	CString	buf;

	writeWithNL(_T("<LABEL>"));
	SetTab(true);
	symbolRect(fN);

	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(fN);
	CformProp*	Prop = (CformProp*)ObjLoad->getProp();

	if (!Prop->objClass.IsEmpty())
	{
		buf.Format(_T("<CLASS>'%s'</CLASS>"), Prop->objClass);
		writeWithNL(buf);
	}
	if (!Prop->strings.IsEmpty())
	{
		buf.Format(_T("<STRING>'%s'</STRING>"), Prop->strings);
		writeWithNL(buf);
	}
	buf.Format(_T("<LINECOLOR>%d</LINECOLOR>"), Prop->borderColor);
	writeWithNL(buf);
	writeFile(_T("<TYPE>"));
	buf = _T("");
	switch (Prop->charType)
	{
	case bxBORDER:
		switch (Prop->borderStyle)
		{
		case bsNONE:
			buf = _T("TP_NONE");	break;
		case bsRAISED:
			buf = _T("TP_RAISED");	break;
		case bsSUNKEN:
			buf = _T("TP_SUNKEN");	break;
		case bsBUMP:
			buf = _T("TP_BUMP");	break;
		case bsETCHED:
			buf = _T("TP_ETCHED");	break;
		case bsLINE:
			buf = _T("TP_LINE");	break;
		default:
			buf = _T("");		break;
		}
		writeFile(buf, false);
		break;
	}
	buf = _T("");
	alignment(Prop->alignment, buf);
	attributes(Prop->attributes);
	writeAuxiliary(fN);
	writeFile(_T("</TYPE>"), false);
	writeFile(_T("\r\n"), false);
	if (Prop->fontStyle)
	{
		buf.Format(_T("<FONT>\"%s\", %d, %s</FONT>"), Prop->fontName, Prop->fontPoint, fontStyle[Prop->fontStyle]);
		writeWithNL(buf);
	}
	color(Prop->paintColor, Prop->textColor);
	groupN(fN);
	writeEtc(fN);
	buf.Format(_T("<ALPHA>%d</ALPHA>"), Prop->alpha);
	writeWithNL(buf);
	buf.Format(_T("<MARGIN>'%s'</MARGIN>"), Prop->margin);
	writeWithNL(buf);
	SetTab(false);
	writeWithNL(_T("</LABEL>"));
}

void CBuild::srcBox(int fN)
{
	CString	buf;

	writeWithNL(_T("<BOX>"));
	SetTab(true);
	symbolRect(fN);

	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(fN);
	CformProp*	Prop = (CformProp*)ObjLoad->getProp();

	if (!Prop->objClass.IsEmpty())
	{
		buf.Format(_T("<CLASS>'%s'</CLASS>"), Prop->objClass);
		writeWithNL(buf);
	}
	color(Prop->paintColor, Prop->textColor);
	buf.Format(_T("<LINEW>%d</LINEW>"), Prop->size);
	writeWithNL(buf);
	writeFile(_T("<TYPE>"));
	switch (Prop->charType)
	{
	//case BX_2CLR:
	//	buf = _T("BX_2CLR");	break;
	case bxBORDER:
		buf = _T("BX_BORDER");	break;
	case bxLINE:
	default:
		buf = _T("BX_LINE");	break;
	}
	writeFile(buf, false);
	attributes(Prop->attributes);
	writeFile(_T("</TYPE>"), false);
	writeFile(_T("\r\n"), false);
	groupN(fN);
	writeEtc(fN);
	buf.Format(_T("<ALPHA>%d</ALPHA>"), Prop->alpha);
	writeWithNL(buf);
	SetTab(false);
	writeWithNL(_T("</BOX>"));
}

void CBuild::srcGroup(int fN)
{
	CString	buf;

	writeWithNL(_T("<GROUP>"));
	SetTab(true);
	symbolRect(fN);

	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(fN);
	CformProp*	Prop = (CformProp*)ObjLoad->getProp();

	if (!Prop->objClass.IsEmpty())
	{
		buf.Format(_T("<CLASS>'%s'</CLASS>"), Prop->objClass);
		writeWithNL(buf);
	}
	if (!Prop->strings.IsEmpty())
	{
		buf.Format(_T("<STRING>'%s'</STRING>"), Prop->strings);
		writeWithNL(buf);
	}

	if (Prop->fontStyle)
	{
		buf.Format(_T("<FONT>\"%s\", %d, %s</FONT>"), Prop->fontName, Prop->fontPoint, fontStyle[Prop->fontStyle]);
		writeWithNL(buf);
	}
	writeFile(_T("<TYPE>"));
	attributes(Prop->attributes);
	writeFile(_T("</TYPE>"), false);
	writeFile(_T("\r\n"), false);
	Prop->paintColor = -1;
	color(Prop->paintColor, Prop->textColor);
	groupN(fN);
	writeEtc(fN);
	SetTab(false);
	writeWithNL(_T("</GROUP>"));
}

void CBuild::srcTab(int fN)
{
	CString	buf;

	writeWithNL(_T("<USRTAB>"));
	SetTab(true);
	symbolRect(fN);

	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(fN);
	CformProp*	Prop = ObjLoad->getProp();

	if (Prop->styles & stCHECKED)
	{
		buf.Format(_T("<INDEX>%d</INDEX>"), Prop->checked);
		writeWithNL(buf);
	}

	buf.Format(_T("<COUNT>%d</COUNT>"), Prop->keyValues);
	writeWithNL(buf);
	writeFile(_T("<TYPE>"));
	buf = _T("");
	if (Prop->styles & stCHECKED)
	{
		writeFile(_T("FOCUS"));
	}
	alignment(Prop->alignment, buf);
	if(Prop->images.IsEmpty())
	{
		writeFile(_T("TAB_3D"), false);
	}
	else
	{
		writeFile(_T("TAB_IMAGE"), false);
	}
	attributes(Prop->attributes);
	writeFile(_T("</TYPE>"), false);
	writeFile(_T("\r\n"), false);
	if (Prop->fontStyle)
	{
		buf.Format(_T("<FONT>\"%s\", %d, %s</FONT>"), Prop->fontName, Prop->fontPoint, fontStyle[Prop->fontStyle]);
		writeWithNL(buf);
	}
	color(Prop->paintColor, Prop->textColor);

	if(!Prop->images.IsEmpty())
	{
		CString	tmps = Prop->images;
		int	offs = tmps.ReverseFind(_T('\\'));
		if (offs != -1)
			tmps = tmps.Mid(offs+1);

		buf.Format(_T("<TABIMAGE>\"%s\"</TABIMAGE>"), tmps);
		writeWithNL(buf);
	}

	//if (Prop->paintColor2 == 0 && Prop->paintColor1 == 0)
	//{
	//	Prop->paintColor2 = (Prop->charType == TAB_NONE) ? 193 : Prop->paintColor;
	//	Prop->paintColor1 = (Prop->charType == TAB_NONE) ? 203 : Prop->textColor;
	//}
	buf.Format(_T("<FCOLOR>%d, %d</FCOLOR>"), Prop->paintColor2, Prop->paintColor1);
	writeWithNL(buf);

	if (Prop->keyValues != 0)
	{
		CString	tmps;

		buf  = _T("<CAPTION>");

		char*	sDsp = (char *)Prop->displays.operator LPCTSTR();
		for (int ii = 0; ii < (int)Prop->keyValues; ii++)
		{
			tmps.Format(_T("\"%s\""), sDsp);
			if (ii < (int)Prop->keyValues - 1)
				tmps += _T(", ");
			buf += tmps;

			sDsp += strlen(sDsp);
			sDsp++;
		}
		buf += _T("</CAPTION>");
		writeWithNL(buf);

		buf = _T("<TABINFO>");

		char*	sData = (char *)Prop->strings.operator LPCTSTR();
		for (int ii = 0; ii < (int)Prop->keyValues; ii++)
		{
			tmps.Format(_T("\"%s\""), sData);
			if (ii < (int)Prop->keyValues - 1)
				tmps += _T(", ");
			buf += tmps;

			sData += strlen(sData);
			sData++;
		}
		buf += _T("</TABINFO>");
		writeWithNL(buf);
	}
	groupN(fN);
	writeEtc(fN);
	SetTab(false);
	writeWithNL(_T("</USRTAB>"));
}

void CBuild::srcPanel(int fN)
{
	CString	buf;

	writeWithNL(_T("<PANEL>"));
	SetTab(true);
	symbolRect(fN);

	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(fN);
	CformProp*	Prop = (CformProp*)ObjLoad->getProp();

	if (!Prop->objClass.IsEmpty())
	{
		buf.Format(_T("<CLASS>'%s'</CLASS>"), Prop->objClass);
		writeWithNL(buf);
	}

	if (!(Prop->styles & stIMAGE) && !Prop->strings.IsEmpty())
	{
		buf.Format(_T("<STRING>'%s'</STRING>"), Prop->strings);
	}

	writeFile(_T("<TYPE>"));
	switch (Prop->charType)
	{
	case bxBORDER:
		switch (Prop->borderStyle) 
		{
		case bsNONE:
			buf = _T("TP_NONE");	break;
		case bsRAISED:
			buf = _T("TP_RAISED");	break;
		case bsSUNKEN:
			buf = _T("TP_SUNKEN");	break;
		case bsBUMP:
			buf = _T("TP_BUMP");	break;
		case bsETCHED:
			buf = _T("TP_ETCHED");	break;
		case bsLINE:
			buf = _T("TP_LINE");	break;
		default:
			buf = _T("");		break;
		}
		writeFile(buf, false);
		break;
	}
	writeAuxiliary(fN);
	attributes(Prop->attributes);
	writeFile(_T("</TYPE>"), false);
	writeFile(_T("\r\n"), false);

	switch (Prop->charType)
	{
	case bxLINE:
		buf.Format(_T("<LINECOLOR>%d</LINECOLOR>"), Prop->borderColor);
		writeWithNL(buf);
		break;
	}

	
	color(Prop->paintColor, Prop->textColor);
	if ((Prop->styles & stIMAGE) || (Prop->styles & stIMAGETEXT))
	{
		CString	tmps = Prop->images;
		int	offs = tmps.ReverseFind(_T('\\'));
		if (offs != -1)
			tmps = tmps.Mid(offs+1);

		buf.Format(_T("<IMAGES>\"%s\"</IMAGES>"), tmps);
		writeWithNL(buf);
	}
	if (!(Prop->styles & stIMAGE))
	{
		buf = _T("");
		//if (!(Prop->styles & stIMAGETEXT))
		//{
		//	alignment(Prop->alignment, buf);
		//	space(2);
		//}
		if (Prop->fontStyle != 0)
		{
			buf.Format(_T("<FONT>\"%s\", %d, %s</FONT>"), Prop->fontName, Prop->fontPoint, fontStyle[Prop->fontStyle]);
			writeWithNL(buf);
		}
	}
	groupN(fN);
	writeEtc(fN);
	buf.Format(_T("<ALPHA>%d</ALPHA>"), Prop->alpha);
	writeWithNL(buf);
	SetTab(false);
	writeWithNL(_T("</PANEL>"));
}

void CBuild::srcButton(int fN)
{
	CString	buf;

	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(fN);
	CformProp*	Prop = ObjLoad->getProp();

	writeWithNL(_T("<BUTTON>"));
	SetTab(true);
	symbolRect(fN);

	if (!Prop->objClass.IsEmpty())
	{
		buf.Format(_T("<CLASS>'%s'</CLASS>"), Prop->objClass);
		writeWithNL(buf);
	}
	if (!(Prop->styles & stIMAGE) && !Prop->strings.IsEmpty()) 
	{
		buf.Format(_T("<STRING>'%s'</STRING>"), Prop->strings);
		writeWithNL(buf);
	}

	switch (Prop->kind)
	{
	case fmBUTTON:
		buf.Format(_T("<KEYMAP>%s</KEYMAP>"), keyMap[Prop->keyParam]);
		writeWithNL(buf);
		break;
	case fmRADIO:
		if (!Prop->keyNames.IsEmpty())
		{
			buf.Format(_T("<GROUP>%s</GROUP>"), Prop->keyNames);
			writeWithNL(buf);
		}
		break;
	}
	writeFile(_T("<TYPE>"));
	switch (Prop->kind)
	{
	case fmBUTTON:
		writeFile(_T("TP_PUSH"), false);
		writeAuxiliary(fN);

		if (Prop->styles & stCHECKED)
		{
			buf = _T("");
			addOR(buf);
			buf += _T("FOCUS");
			writeFile(buf, false);
		}
		break;

	case fmRADIO:
	case fmCHECK:
		if(Prop->kind == fmRADIO)
			buf = _T("TP_RADIO");
		else
			buf = _T("TP_CHECK");
		addOR(buf);
		switch (Prop->ioType)
		{
		case ioINPUT:
			buf += _T("EDIT_INPUT");	break;
		case ioBOTH:
			buf += _T("EDIT_INOUT");	break;
		case ioFREE:
		default:
			buf += _T("EDIT_NOP");	break;
		}
		writeFile(buf, false);
		writeAuxiliary(fN);
		//space(2);
		//attributes(Prop->attributes);
		
		break;

	default:
		break;
	}
	if (!(Prop->styles & stIMAGE))
	{
		if (Prop->kind == fmBUTTON)
		{
			if (!(Prop->styles & stIMAGETEXT))
			{
				alignment(Prop->alignment, buf);
			}
		}
	}
	attributes(Prop->attributes);
	writeFile(_T("</TYPE>"), false);
	writeFile(_T("\r\n"), false);
	//space(2);
	buf = _T("");

	if (!(Prop->styles & stIMAGE))
	{
		if (Prop->fontStyle != 0)
		{
			buf.Format(_T("<FONT>\"%s\", %d, %s</FONT>"), Prop->fontName, Prop->fontPoint, fontStyle[Prop->fontStyle]);
			writeWithNL(buf);
		}
	}

	color(Prop->paintColor, Prop->textColor);
	if (Prop->styles & stIMAGE || Prop->styles & stIMAGETEXT)
	{
		CString	tmps = Prop->images;
		int	offs = tmps.ReverseFind(_T('\\'));
		if (offs != -1)
			tmps = tmps.Mid(offs+1);

		buf.Format(_T("<IMAGES>\"%s\"</IMAGES>"), tmps);
		writeWithNL(buf);
	}
	buf.Format(_T("<FCOLOR>%d, %d</FCOLOR>"), Prop->paintColor2, Prop->paintColor1);
	writeWithNL(buf);
	tabOrder(Prop->index);
	if ((Prop->kind == fmRADIO || Prop->kind == fmCHECK) && Prop->ioType != ioFREE)
		ioOrder(Prop->iorder);
	groupN(fN);
	writeEtc(fN);
	buf.Format(_T("<AUXIMAGES>%d</AUXIMAGES>"), Prop->auxImages);
	writeWithNL(buf);
	buf.Format(_T("<ALPHA>%d</ALPHA>"), Prop->alpha);
	writeWithNL(buf);
	buf.Format(_T("<MARGIN>'%s'</MARGIN>"), Prop->margin);
	writeWithNL(buf);
	SetTab(false);
	writeWithNL(_T("</BUTTON>"));
}

void CBuild::srcForm(int fN)
{
	CString	buf;

	writeWithNL(_T("<OBJECT>"));
	SetTab(true);
	symbolRect(fN);

	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(fN);
	CformProp*	Prop = (CformProp*)ObjLoad->getProp();

	writeFile(_T("<TYPE>"));
	attributes(Prop->attributes);
	if (Prop->auxAttributes & gaFIX)
	{
		writeFile(_T("RESIZABLE"), false);
	}
	if (Prop->bFixedSize)
		writeFile(_T("FIXEDSIZE"), false);
	writeFile(_T("</TYPE>"), false);
	writeFile(_T("\r\n"), false);
	buf.Format(_T("<STRING>'%s'</STRING>"), Prop->strings);
	writeWithNL(buf);
	tabOrder(Prop->index);
	ioOrder(Prop->iorder);
	groupN(fN);
	writeEtc(fN);
	SetTab(false);
	writeWithNL(_T("</BUTTON>"));
}

void CBuild::srcEdit(int fN)
{
	CString	buf;

	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(fN);
	CformProp*	Prop = (CformProp*)ObjLoad->getProp();

	switch (Prop->kind) 
	{
	case fmEDIT:
		writeWithNL(_T("<EDIT>"));
		break;
	case fmOUTPUT:
		writeWithNL(_T("<OUT>"));
		break;
	default:
		return;
	}
	SetTab(true);
	symbolRect(fN);

	if (!Prop->objClass.IsEmpty())
	{
		buf.Format(_T("<CLASS>'%s'</CLASS>"), Prop->objClass);
		writeWithNL(buf);
	}

	buf.Format(_T("<HINTTEXTCOLR>%d</HINTTEXTCOLR>"), Prop->paintColor1);
	writeWithNL(buf);

	buf.Format(_T("<LEN>%d</LEN>"), Prop->size);
	writeWithNL(buf);
	buf.Format(_T("<RTS>%d</RTS>"), Prop->flashKeys);
	writeWithNL(buf);
	writeFile(_T("<TYPE>"));
	dataType(Prop->charType);
	buf = _T("");
	if (Prop->kind == fmEDIT)
	{
		addOR(buf);
		if (Prop->ioType == ioINPUT)
			buf += _T("EDIT_INPUT");
		else
			buf += _T("EDIT_INOUT");
	}
	//else
	//{
	//	addOR(buf);
		alignment(Prop->alignment, buf);
		addOR(buf);
	//}
	writeFile(buf, false);
	buf = _T("");

	if (Prop->charType == ctDIGIT)
		Prop->attributes |= faXFLOAT;
	else
		Prop->attributes &= ~faXFLOAT;

	if (Prop->attributes & faXFLOAT)
		Prop->offsets = 0;

	if (Prop->offsets)
		Prop->attributes |= faFLOAT;
	else
		Prop->attributes &= ~faFLOAT;

	attributes(Prop->attributes);
	subattributes(Prop->attributes, Prop->auxAttributes);
	if (Prop->styles & stCODE)
	{
		addOR(buf);
		buf += _T("CODE");
		writeFile(buf);
		buf = _T("");
	}
	if (Prop->styles & stSUM)
	{
		addOR(buf);
		buf += _T("SUM");
		writeFile(buf);
		buf = _T("");
	}
	if (Prop->kind == fmEDIT && Prop->styles & stLIST)
	{
		addOR(buf);
		buf += _T("LIST");
		writeFile(buf);
		buf = _T("");
	}
	if (Prop->kind == fmEDIT && Prop->styles & stEDIT)
	{
		addOR(buf);
		buf += _T("SHOWFORM");
		writeFile(buf);
		buf = _T("");
	}
	if (Prop->kind == fmOUTPUT)
	{
		switch (Prop->ioType)
		{
		case bxBORDER:
			switch (Prop->borderStyle)
			{
			case bsNONE:
				buf = _T("TP_NONE");	break;
			case bsRAISED:
				buf = _T("TP_RAISED");	break;
			case bsSUNKEN:
				buf = _T("TP_SUNKEN");	break;
			case bsBUMP:
				buf = _T("TP_BUMP");	break;
			case bsETCHED:
				buf = _T("TP_ETCHED");	break;
			default:
				buf = _T("");		break;
			}
			writeFile(buf, false);
			buf = _T("");
			break;
		}
	}
	else if (Prop->kind == fmEDIT)
	{
		switch (Prop->borderStyle)
		{
		case bsNONE:	// sunken <--> none
			buf = _T("TP_NONE");	break;
		case bsSUNKEN:	// none <--> sunken
			buf = _T("TP_SUNKEN");	break;
		default:
			buf = _T("");		break;
		}
		writeFile(buf, false);
		buf = _T("");
	}
	writeAuxiliary(fN);
	writeFile(_T("</TYPE>"), false);
	writeFile(_T("\r\n"), false);

	if (Prop->keyParam > 0)
	{
		buf.Format(_T("<SPIN>%d</SPIN>"), Prop->keyValues);
		writeWithNL(buf);
		buf = _T("");
		if (Prop->keyParam > 1)
		{
			buf.Format(_T("<SPINTYPE>%d</SPINTYPE>"), Prop->keyParam);
			writeWithNL(buf);
			buf = _T("");
		}
	}
	else
		Prop->styles &= ~stSPIN;

	buf = _T("");
	if (Prop->fontStyle)
	{
		buf.Format(_T("<FONT>\"%s\", %d, %s</FONT>"), Prop->fontName, Prop->fontPoint, fontStyle[Prop->fontStyle]);
		writeWithNL(buf);
		buf = _T("");
	}
	color(Prop->paintColor, Prop->textColor);
	
	//if (Prop->kind == fmOUTPUT)
	//{
	//	switch (Prop->ioType)
	//	{
	//	case bxLINE:
			buf.Format(_T("<LINECOLOR>%d</LINECOLOR>"), Prop->borderColor);
			writeWithNL(buf);
			buf = _T("");
	//		break;
	//	}
	//}

	buf.Format(_T("<OPTICOLOR>%d</OPTICOLOR>"), Prop->auxColor);
	writeWithNL(buf);
	
	if (Prop->kind == fmEDIT)
	{
		if (!Prop->strings.IsEmpty())
		{
			buf.Format(_T("<INIT>\"%s\"</INIT>"), Prop->strings);
			writeWithNL(buf);
			buf = _T("");
		}
	}
	if (Prop->kind == fmEDIT)
	{
		if (!Prop->keyNames.IsEmpty() && (Prop->attributes & faENUM))
		{
			buf.Format(_T("<ENCRYPT>\"%s\"</ENCRYPT>"), Prop->keyNames);
			writeWithNL(buf);
		}
	}
	if (Prop->kind == fmOUTPUT || Prop->kind == fmEDIT)
	{
		if (Prop->offsets)
		{
			buf.Format(_T("<FLOAT>%d</FLOAT>"), Prop->offsets);
			writeWithNL(buf);
		}
	}
	if (Prop->kind == fmOUTPUT)
	{
		if (!Prop->keyNames.IsEmpty())
		{
			buf.Format(_T("<DOM>%s</DOM>"), Prop->keyNames);
			writeWithNL(buf);
		}

		if (!Prop->referenceName.IsEmpty())
		{
			buf.Format(_T("<REFERENCE>%s</REFERENCE>"), Prop->referenceName);
			writeWithNL(buf);
		}
	}

	switch (Prop->whatEdit)
	{
	case weIT:
		buf.Format(_T("<EDITFORMAT>\"%s\"</EDITFORMAT>"), Prop->editFormat);
		writeWithNL(buf);
		buf = _T("");
		break;
	case weCORGB:
		buf.Format(_T("<IFRGB>%d, %d</IFRGB>"), HIWORD(Prop->auxValues), LOWORD(Prop->auxValues));
		writeWithNL(buf);
		buf = _T("");
		break;
	case weNOP:
	default:
		break;
	}
	buf.Format(_T("<IMAGES>\"%s\"</IMAGES>"), Prop->images);
	writeWithNL(buf);
	buf.Format(_T("<ALPHA>%d</ALPHA>"), Prop->alpha);
	writeWithNL(buf);
	buf.Format(_T("<MARGIN>'%s'</MARGIN>"), Prop->margin);
	writeWithNL(buf);
	buf.Format(_T("<RTSSHAPE>%d</RTSSHAPE>"), Prop->rtsShape);
	writeWithNL(buf);
	tabOrder(Prop->index);
	ioOrder(Prop->iorder);
	groupN(fN);
	writeEtc(fN);
	SetTab(false);
	switch (Prop->kind)
	{
	case fmEDIT:
		writeWithNL(_T("</EDIT>"));
		break;
	case fmOUTPUT:
		writeWithNL(_T("</OUT>"));
		break;
	default:
		return;
	}
}

void CBuild::srcCombo(int fN)
{
	CString	buf;

	writeWithNL(_T("<COMBO>"));
	SetTab(true);
	symbolRect(fN);

	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(fN);
	CformProp*	Prop = (CformProp*)ObjLoad->getProp();

	writeFile(_T("<TYPE>"));
	switch (Prop->borderStyle)
	{
	case bsNONE:	// 3D
		buf = _T("TP_NONE");	break;
	case bsSUNKEN:	// line
		buf = _T("TP_SUNKEN");	break;
	default:
		buf = _T("");		break;
	}
	if (!buf.IsEmpty())
	{
		writeFile(buf, false);
	}
	dataType(Prop->charType);
	buf = _T("");
	addOR(buf);
	if (Prop->ioType == ioINPUT)
		buf += _T("EDIT_INPUT");
	else
		buf += _T("EDIT_INOUT");
	writeFile(buf, false);
	buf = _T("");
	attributes(Prop->attributes);
	subattributes(Prop->attributes, Prop->auxAttributes);
	if (Prop->styles & stCODE)
	{
		addOR(buf);
		buf += _T("CODE");
		writeFile(buf, false);
		buf = _T("");
	}
	writeFile(_T("</TYPE>"), false);
	writeFile(_T("\r\n"), false);

	buf.Format(_T("<LEN>%d</LEN>"), Prop->size);
	writeWithNL(buf);
	buf.Format(_T("<COUNT>%d</COUNT>"), LOWORD(Prop->keyValues));
	writeWithNL(buf);
	buf.Format(_T("<VCOUNT>%d</VCOUNT>"), HIWORD(Prop->keyValues));
	writeWithNL(buf);
	buf.Format(_T("<INDEX>%d</INDEX>"), Prop->checked);
	writeWithNL(buf);
	if (!Prop->images.IsEmpty())
	{
		buf.Format(_T("<CVAR>%s</CVAR>"), Prop->images);
		writeWithNL(buf);
	}

	buf.Format(_T("<MATCHKEY>%s</MATCHKEY>"), keyMap[Prop->auxValues]);
	writeWithNL(buf);

	if (Prop->fontStyle)
	{
		buf.Format(_T("<FONT>\"%s\", %d, %s</FONT>"), Prop->fontName, Prop->fontPoint, fontStyle[Prop->fontStyle]);
		writeWithNL(buf);
	}
	color(Prop->paintColor, Prop->textColor);
	if (LOWORD(Prop->keyValues) != 0)
	{
		CString	tmps, data;
		int	count = LOWORD(Prop->keyValues);

		buf = _T("<DISPLAY>");
		char*	sDsp = (char *)Prop->displays.operator LPCTSTR();
		for (int ii = 0; ii < count; ii++)
		{
			tmps.Format(_T("\"%s\""), sDsp);
			if (ii < count - 1)
				tmps += _T(", ");
			buf += tmps;

			sDsp += strlen(sDsp);
			sDsp++;
		}
		buf += _T("</DISPLAY>");
		writeFile(buf);
		writeFile(_T("\r\n"), false);

		buf = _T("<DATA>");
		char*	sData = (char *)Prop->strings.operator LPCTSTR();
		for (int ii = 0; ii < count; ii++)
		{
			tmps.Format(_T("\"%s\""), sData);
			if (ii < count - 1)
				tmps += _T(", ");
			buf += tmps;

			sData += strlen(sData);
			sData++;
		}
		buf += _T("</DATA>");
		writeFile(buf);
		writeFile(_T("\r\n"), false);
	}

	switch (Prop->whatEdit)
	{
	case weCORGB:
		break;
	case weIT:
	case weNOP:
	default:
		if (!Prop->editFormat.IsEmpty())
		{
			buf.Format(_T("<EDITFORMAT>\"%s\"</EDITFORMAT>"), Prop->editFormat);
			writeWithNL(buf);
			buf = _T("");
		}
		break;
	}
	tabOrder(Prop->index);
	ioOrder(Prop->iorder);
	groupN(fN);
	writeEtc(fN);
	SetTab(false);
	writeWithNL(_T("</COMBO>"));
}

void CBuild::srcGrid(int fN, char kind)
{
	CString	buf;

	if (kind == fmGRID)
		writeWithNL(_T("<GRID>"));
	else if (kind == fmGRIDEX)
		writeWithNL(_T("<GRIDEX>"));
	SetTab(true);
	symbolRect(fN);

	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(fN);
	CformProp*	Prop = (CformProp*)ObjLoad->getProp();

	buf.Format(_T("<VISIBLEROW>%d</VISIBLEROW>"), HIWORD(Prop->keyValues));
	writeWithNL(buf);
	buf.Format(_T("<CELLH>%d</CELLH>"), LOWORD(Prop->keyValues));
	writeWithNL(buf);
	buf.Format(_T("<MATRIX>%d, %d</MATRIX>"), Prop->size, Prop->cellProperties->GetCount());
	writeWithNL(buf);
	buf.Format(_T("<FIX>%d</FIX>"), (Prop->auxAttributes & gaMULTI) ? 0 : Prop->fixColumn);
	writeWithNL(buf);
	buf.Format(_T("<LINECOLOR>%d</LINECOLOR>"), Prop->borderColor);
	writeWithNL(buf);
	buf.Format(_T("<FOCUSCOLOR>%d</FOCUSCOLOR>"), Prop->auxValues);
	writeWithNL(buf);
	buf.Format(_T("<FOCUSKIND>%d</FOCUSKIND>"), Prop->whatEdit);
	writeWithNL(buf);

	if (Prop->fontStyle)
	{
		buf.Format(_T("<FONT>\"%s\", %d, %s</FONT>"), Prop->fontName, Prop->fontPoint, fontStyle[Prop->fontStyle]);
		writeWithNL(buf);
	}

	if (!Prop->strings.IsEmpty())
	{
		buf.Format(_T("<VARIANT>\"%s\"</VARIANT>"), Prop->strings);
		writeWithNL(buf);
	}

	if (!Prop->images.IsEmpty())
	{
		buf.Format(_T("<GRSHARE>\"%s\"</GRSHARE>"), Prop->images);
		writeWithNL(buf);
	}

	if (Prop->auxAttributes & gaROW)
	{
		buf.Format(_T("<ROWC>%d, %d</ROWC>"), Prop->paintColor2, Prop->paintColor1);
		writeWithNL(buf);
		buf.Format(_T("<ROWCC>%d</ROWCC>"), Prop->rowLines);
		writeWithNL(buf);
	}

	writeFile(_T("<TYPE>"));
	buf = _T("");
	if (Prop->auxAttributes & gaAUTOSIZE)
	{
		addOR(buf);
		buf += _T("AUTOSIZE");
	}
	if (Prop->auxAttributes & gaMULTI)
	{
		addOR(buf);
		buf += _T("MULTI");
	}
	if (Prop->auxAttributes & gaMERGE)
	{
		addOR(buf);
		buf += _T("MERGE");
	}
	if (Prop->auxAttributes & gaALWAYS)
	{
		addOR(buf);
		buf += _T("SELALWAYS");
	}
	if (Prop->auxAttributes & gaFLEX)
	{
		addOR(buf);
		buf += _T("FLEXIBLE");
	}
	if (Prop->auxAttributes & gaVSCR)
	{
		addOR(buf);
		buf += _T("VSCROLL");
	}
	if (Prop->auxAttributes & gaHSCR)
	{
		addOR(buf);
		buf += _T("HSCROLL");
	}
	if (Prop->auxAttributes & gaTOP)
	{
		addOR(buf);
		buf += _T("INS_TOP");
	}
	if (Prop->auxAttributes & gaXHEAD)
	{
		addOR(buf);
		buf += _T("NOHEAD");
	}
	if (Prop->auxAttributes & gaFIX)
	{
		addOR(buf);
		buf += _T("GRFIX");
	}
	if (Prop->auxAttributes & gaX3D)
	{
		addOR(buf);
		buf += _T("NO3D");
	}
	if (Prop->auxAttributes & ga3DHEAD)
	{
		addOR(buf);
		buf += _T("ONLYHEAD3D");
	}
	if (Prop->auxAttributes & gaROW)
	{
		addOR(buf);
		buf += _T("RROW");
	}
	if (Prop->auxAttributes & gaXVLINE)
	{
		addOR(buf);
		buf += _T("XVLINE");
	}
	if (Prop->auxAttributes & gaXHLINE)
	{
		addOR(buf);
		buf += _T("XHLINE");
	}
	if (Prop->auxAttributes & gaPAGE)
	{
		addOR(buf);
		buf += _T("PAGE");
	}
	if (Prop->auxAttributes & gaHEADER)
	{
		addOR(buf);
		buf += _T("HEADER");
	}
	if (Prop->auxAttributes & gaTABLE)
	{
		addOR(buf);
		buf += _T("TABLE");
	}
	if (Prop->auxAttributes & gaTHUMB)
	{
		addOR(buf);
		buf += _T("THUMB");
	}
	if (Prop->auxAttributes & gaCUD)
	{
		addOR(buf);
		buf += _T("CUD");
	}
	if (Prop->auxAttributes & gaKEY)
	{
		addOR(buf);
		buf += _T("KEYFOCUS");
	}
	if (Prop->auxAttributes & gaEXPAND)
	{
		addOR(buf);
		buf += _T("GRFLEXEXPAND");
	}
	if (Prop->auxAttributes & gaVALID)
	{
		addOR(buf);
		buf += _T("GRVALID");
	}
	if (Prop->auxAttributes & gaFTEXT)
	{
		addOR(buf);
		buf += _T("GRFTEXT");
	}
	if (Prop->auxAttributes & gaXXLS)
	{
		addOR(buf);
		buf += _T("GRXXSL");
	}
	if (Prop->attributes & faWRAP)
	{
		addOR(buf);
		buf += _T("LEAVE");
	}
	if (Prop->attributes & faENUM)
	{
		addOR(buf);
		buf += _T("CELLEDIT");
	}
	if (Prop->attributes & faXDELETE)
	{
		addOR(buf);
		buf += _T("CXDELETE");
	}
	if (Prop->attributes & faDROP)
	{
		addOR(buf);
		buf += _T("FADROP");
	}
	if (Prop->auxAttributes & gaXSIZE)
	{
		addOR(buf);
		buf += _T("GRXSIZE");
	}
	if (Prop->auxAttributes & gaAUTO)
	{
		addOR(buf);
		buf += _T("GRAUTOWIDTH");
	}
	if (buf.GetAt(0) == _T('|'))
		buf.Delete(0);

	writeFile(buf, false);
	writeFile(_T("</TYPE>"), false);
	writeFile(_T("\r\n"), false);

	buf = _T("");
	Prop->styles |= stENABLE;
	tabOrder(Prop->index);
	ioOrder(Prop->iorder);
	groupN(fN);
	writeEtc(fN);
	writeColumns(Prop->cellProperties);
	SetTab(false);
	if (kind == fmGRID)
		writeWithNL(_T("</GRID>"));
	else if (kind == fmGRIDEX)
		writeWithNL(_T("</GRIDEX>"));
}

void CBuild::srcMemo(int fN)
{
	CString	buf;

	writeWithNL(_T("<MEMO>"));
	SetTab(true);
	symbolRect(fN);

	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(fN);
	CformProp*	Prop = (CformProp*)ObjLoad->getProp();

	writeFile(_T("<TYPE>"));
	switch (Prop->borderStyle)
	{
	case bsNONE:
		buf += _T("TP_NONE");	break;
	case bsRAISED:
	default:
		buf += _T("TP_RAISED");	break;
	}
	addOR(buf);
	writeFile(buf, false);
	dataType(ctMIXED);

	buf = _T("");
	switch (Prop->ioType)
	{
	case ioINPUT:
		addOR(buf);
		buf += _T("EDIT_INPUT");	break;
	case ioBOTH:
		addOR(buf);
		buf += _T("EDIT_INOUT");	break;
	case ioOUTPUT:
		addOR(buf);
		buf += _T("EDIT_OUTPUT");	break;
	}
	addOR(buf);
	buf += _T("FA_HAN");
	writeFile(buf, false);
	writeFile(_T("</TYPE>"), false);
	writeFile(_T("\r\n"), false);

	buf = _T("");
	if (Prop->fontStyle)
	{
		buf.Format(_T("<FONT>\"%s\", %d, %s</FONT>"), Prop->fontName, Prop->fontPoint, fontStyle[Prop->fontStyle]);
		writeWithNL(buf);
	}
	color(Prop->paintColor, Prop->textColor);
	attributes(Prop->attributes);

	tabOrder(Prop->index);
	ioOrder(Prop->iorder);
	groupN(fN);
	writeEtc(fN);
	SetTab(false);
	writeWithNL(_T("</MEMO>"));
}

void CBuild::srcTable(int fN)
{
	CString	buf;

	writeWithNL(_T("<TABLE>"));
	SetTab(true);
	symbolRect(fN);

	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(fN);
	CformProp*	Prop = (CformProp*)ObjLoad->getProp();

	buf.Format(_T("<MATRIX>%d, %d</MATRIX>"), Prop->cellProperties->GetCount(), 1);
	writeWithNL(buf);
	buf.Format(_T("<LINECOLOR>%d</LINECOLOR>"), Prop->borderColor);
	writeWithNL(buf);
	buf.Format(_T("<CELLH>%d</CELLH>"), Prop->iAutoSize); //Prop->keyValues
	writeWithNL(buf);
	writeFile(_T("<TYPE>"));
	buf = _T("");
	if (Prop->auxAttributes & gaAUTOSIZE)
	{
		addOR(buf);
		buf += _T("AUTOSIZE");
	}
	if (Prop->auxAttributes & gaMULTI)
	{
		addOR(buf);
		buf += _T("MULTI");
	}
	if (Prop->auxAttributes & gaXHEAD)
	{
		addOR(buf);
		buf += _T("NOHEAD");
	}
	if (Prop->auxAttributes & gaXVLINE)
	{
		addOR(buf);
		buf += _T("XVLINE");
	}
	if (Prop->auxAttributes & gaXHLINE)
	{
		addOR(buf);
		buf += _T("XHLINE");
	}
	if (Prop->attributes & faXDELETE)
	{
		addOR(buf);
		buf += _T("CXDELETE");
	}
	if (Prop->styles & stCHECKED)
	{
		addOR(buf);
		buf += _T("ISFOCUS");
	}
	switch (Prop->ioType)
	{
	case ioOUTPUT:
		addOR(buf);
		buf += _T("EDIT_OUTPUT"); break;
	case ioFREE:
	default:
		addOR(buf);
		buf += _T("EDIT_NOP"); break;
	}
	if (buf.GetAt(0) == _T('|'))
		buf.Delete(0);
	writeFile(buf, false);
	writeFile(_T("</TYPE>"), false);
	writeFile(_T("\r\n"), false);
	buf = _T("");

	if (Prop->fontStyle)
	{
		buf.Format(_T("<FONT>\"%s\", %d, %s</FONT>"), Prop->fontName, Prop->fontPoint, fontStyle[Prop->fontStyle]);
		writeWithNL(buf);
	}
	ioOrder(Prop->iorder);
	groupN(fN);
	writeEtc(fN);
	writeRows(Prop->cellProperties);
	SetTab(false);
	writeWithNL(_T("</TABLE>"));
}

void CBuild::srcTreeView(int fN)
{
	CString	buf;

	writeWithNL(_T("<TREEVIEW>"));
	SetTab(true);
	symbolRect(fN);

	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(fN);
	CformProp*	Prop = (CformProp*)ObjLoad->getProp();

	buf.Format(_T("<LEN>%d</LEN>"), Prop->size);
	writeWithNL(buf);
	writeFile(_T("<TYPE>"));
	dataType(ctALPHA);
	attributes(Prop->attributes);
	if (Prop->styles & stCHECKED)
	{
		buf = _T("");
		addOR(buf);
		buf += _T("MULTISEL");
		writeFile(buf, false);
	}
	writeFile(_T("</TYPE>"), false);
	writeFile(_T("\r\n"), false);

	if (Prop->fontStyle)
	{
		buf.Format(_T("<FONT>\"%s\", %d, %s</FONT>"), Prop->fontName, Prop->fontPoint, fontStyle[Prop->fontStyle]);
		writeWithNL(buf);
	}

	color(Prop->paintColor, Prop->textColor);
	ioOrder(Prop->iorder);
	groupN(fN);
	writeEtc(fN);
	SetTab(false);
	writeWithNL(_T("</TREEVIEW>"));
}

void CBuild::srcBrowser(int fN)
{
	CString	buf;

	writeWithNL(_T("<BROWSER>"));
	SetTab(true);
	symbolRect(fN);

	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(fN);
	CformProp*	Prop = (CformProp*)ObjLoad->getProp();

	writeFile(_T("<TYPE>"));
	buf = _T("");
	switch (Prop->ioType)
	{
	case ioFREE:
	default:
		addOR(buf);
		buf += _T("EDIT_NOP");	break;
	case ioOUTPUT:
		addOR(buf);
		buf += _T("EDIT_OUTPUT");	break;
	}

	if (Prop->styles & stCHECKED)
	{
		addOR(buf);
		buf += _T("HASTOOL");
	}
	if (buf.GetAt(0) == _T('|'))
		buf.Delete(0);
	writeFile(buf, false);
	attributes(Prop->attributes);
	writeFile(_T("</TYPE>"), false);
	writeFile(_T("\r\n"), false);
	if (!Prop->strings.IsEmpty())
	{
		buf.Format(_T("<URL>\"%s\"</URL>"), Prop->strings);
		writeWithNL(buf);
	}
	if (Prop->ioType == ioOUTPUT)
		ioOrder(Prop->iorder);
	groupN(fN);
	writeEtc(fN);
	SetTab(false);
	writeWithNL(_T("</BROWSER>"));
}

void CBuild::srcObject(int fN)
{
	CRect	fmRc;
	CString	buf;

	writeWithNL(_T("<CONTROL>"));
	SetTab(true);
	symbolRect(fN);

	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(fN);
	CformProp*	Prop = (CformProp*)ObjLoad->getProp();

	writeFile(_T("<TYPE>"));
	buf = _T("");
	switch (Prop->ioType)
	{
	case ioINPUT:
		buf += _T("EDIT_INPUT");	break;
	case ioBOTH:
		buf += _T("EDIT_INOUT");	break;
	case ioOUTPUT:
		buf += _T("EDIT_OUTPUT");	break;
	case ioFREE:
	default:
		buf += _T("EDIT_NOP");		break;
	}
	writeFile(buf, false);
	attributes(Prop->attributes);
	writeFile(_T("</TYPE>"), false);
	writeFile(_T("\r\n"), false);
	buf = _T("");
	buf.Format(_T("<STRING>'%s'</STRING>"), Prop->strings);
	writeWithNL(buf);
	buf.Format(_T("<IMAGES>\"%s\"</IMAGES>"), Prop->images);
	writeWithNL(buf);

	buf.Format(_T("<LEN>%d</LEN>"), Prop->size);
	writeWithNL(buf);
	buf.Format(_T("<IDX>%d</IDX>"), Prop->iCtmIdx);
	writeWithNL(buf);

	if (Prop->fontStyle)
	{
		buf.Format(_T("<FONT>\"%s\", %d, %s</FONT>"), Prop->fontName, Prop->fontPoint, fontStyle[Prop->fontStyle]);
		writeWithNL(buf);
	}

	color(Prop->paintColor, Prop->textColor);

	tabOrder(Prop->index);
	if (Prop->ioType != ioFREE)
		ioOrder(Prop->iorder);
	groupN(fN);
	writeEtc(fN);
	SetTab(false);
	writeWithNL(_T("</CONTROL>"));
}

void CBuild::symbolRect(int fN)
{
	CString	buf;

	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(fN);
	CformProp*	Prop = ObjLoad->getProp();

	buf.Format(_T("<NAME>%s</NAME>"), Prop->name);
	writeWithNL(buf);
	buf.Format(_T("<RECT>%d, %d, %d, %d</RECT>"), Prop->rect.left, Prop->rect.top,
		Prop->rect.right, Prop->rect.bottom);
	writeWithNL(buf);
}

void CBuild::SetTab(bool bTab)
{
	if (bTab)
	{
		m_iTab++;
	}
	else
	{
		m_iTab--;
		if (m_iTab < 0) m_iTab = 0;
	}
}

void CBuild::InitTab()
{
	m_iTab = 0;
}

void CBuild::attributes(DWORD attributes)
{
	CString	buf;

	buf = _T("");
	if (attributes & faCOMMA)
	{
		addOR(buf);
		buf += _T("FA_COMMA");
	}
	if (attributes & faRIGHT)
	{
		addOR(buf);
		buf += _T("FA_RIGHT");
	}
	if (attributes & faPASSWD)
	{
		addOR(buf);
		buf += _T("FA_PASSWD");
	}
	if (attributes & faZEROSUP)
	{
		addOR(buf);
		buf += _T("FA_ZEROSUP");
	}
	if (attributes & faSEND)
	{
		addOR(buf);
		buf += _T("FA_SEND");
	}
	if (attributes & faPROTECT)
	{
		addOR(buf);
		buf += _T("FA_PROTECT");
	}
	if (attributes & faWRAP)
	{
		addOR(buf);
		buf += _T("FA_WRAP");
	}
	if (attributes & faMUST)
	{
		addOR(buf);
		buf += _T("FA_MAND");
	}
	if (attributes & faMULTI)
	{
		addOR(buf);
		buf += _T("FA_HAN");
	}
	if (attributes & faSKIP)
	{
		addOR(buf);
		buf += _T("FA_SKIP");
	}
	if (attributes & faCORGB)
	{
		addOR(buf);
		buf += _T("FA_CORGB");
	}
	if (attributes & faDOMINO)
	{
		addOR(buf);
		buf += _T("FA_DOMINO");
	}
	if (attributes & faREF)
	{
		addOR(buf);
		buf += _T("FA_REF");
	}
	if (attributes & faDATE)
	{
		addOR(buf);
		buf += _T("FA_DATE");
	}
	if (attributes & faZERO)
	{
		addOR(buf);
		buf += _T("FA_ZERO");
	}
	if (attributes & faFLASH)
	{
		addOR(buf);
		buf += _T("FA_FLASH");
	}
	if (attributes & faHORS)
	{
		addOR(buf);
		buf += _T("FA_HORS");
	}
	if (attributes & faFLOAT)
	{
		addOR(buf);
		buf += _T("FA_FLOAT");
	}
	if (attributes & faFLOATx)
	{
		addOR(buf);
		buf += _T("FA_FLOATx");
	}
	if (attributes & faXFLOAT)
	{
		addOR(buf);
		buf += _T("FA_XFLOAT");
	}
	if (attributes & faUPPER)
	{
		addOR(buf);
		buf += _T("FA_UPPER");
	}
	if (attributes & faDRAG)
	{
		addOR(buf);
		buf += _T("FA_DRAG");
	}
	if (attributes & faDROP)
	{
		addOR(buf);
		buf += _T("FA_DROP");
	}
	if (attributes & faNOR)
	{
		addOR(buf);
		buf += _T("FA_NOR");
	}
	if (attributes & faENUM)
	{
		addOR(buf);
		buf += _T("FA_ENUM");
	}
	if (attributes & faXDELETE)
	{
		addOR(buf);
		buf += _T("FA_XDELETE");
	}
	if (attributes & faCONTRAST)
	{
		addOR(buf);
		buf += _T("FA_CONTRAST");
	}
	if (attributes & faPASS)
	{
		addOR(buf);
		buf += _T("FA_PASS");
	}
	writeFile(buf,false);
}

void CBuild::subattributes(DWORD attributes, DWORD auxAttributes)
{
	CString	buf;

	buf = _T("");
	if (attributes & faZEROSUP)
	{
		switch (getTypeOfZeroSupress(auxAttributes))
		{
		case spSPACE:
			addOR(buf);
			buf += _T("SA_SPACE");
			break;
		case spASTER:
			addOR(buf);
			buf += _T("SA_ASTER");
			break;
		case spZERO:
			addOR(buf);
			buf += _T("SA_ZERO");
			break;
		default:
			break;
		}
	}

	if (attributes & faCORGB)
	{
		switch (getTypeOfConditionColor(auxAttributes))
		{
		case ccRAW:
			addOR(buf);
			buf += _T("SA_CORAW");
			break;
		case ccSUP:
			addOR(buf);
			buf += _T("SA_COSUP");
			break;
		case ccSIGN:
			addOR(buf);
			buf += _T("SA_COSIGN");
			break;
		default:
			break;
		}
	}

	if (attributes & faSEND)
	{
		switch (getTypeOfSendMethod(auxAttributes))
		{
		case smALL:
			addOR(buf);
			buf += _T("SA_ALL");
			break;
		case smTHIS:
			addOR(buf);
			buf += _T("SA_THIS");
			break;
		default:
			break;
		}
	}

	if (attributes & faDATE)
	{
		switch (getTypeOfDateFormat(auxAttributes))
		{
		case dfAUTO:
			addOR(buf);
			buf += _T("SA_AUTO");
			break;
		case dfYYYYMM:
			addOR(buf);
			buf += _T("SA_YYYYMM");
			break;
		case dfYYMM:
			addOR(buf);
			buf += _T("SA_YYMM");
			break;
		case dfMMDD:
			addOR(buf);
			buf += _T("SA_MMDD");
			break;
		default:
			break;
		}
	}

	if (attributes & faCONTRAST)
	{
		switch (getTypeOfConditionColor(auxAttributes))
		{
		case ccTEXT:
			addOR(buf);
			buf += _T("SA_TEXT");
			break;
		case ccPAINT:
			addOR(buf);
			buf += _T("SA_PAINT");
			break;
		default:
			break;
		}
	}

	writeFile(buf, false);
}

void CBuild::color(DWORD paintColor, DWORD textColor)
{
	CString	buf;

	buf.Format(_T("<COLOR>%d, %d</COLOR>"), paintColor, textColor);
	writeWithNL(buf);
}

void CBuild::dataType(BYTE type)
{
	CString	buf;

	switch (type)
	{
	case ctNUMERIC:
		buf = _T("IO_NUM");	break;
	case ctALPHA:
		buf = _T("IO_ALPHA");	break;
	case ctMULTI:
		buf = _T("IO_HAN");	break;
	case ctMIXED:
		buf = _T("IO_MIXED");	break;
	case ctDIGIT:
		buf = _T("IO_DIGIT");	break;
	default:
		buf = _T("");		break;
	}
	writeFile(buf, false);
}

void CBuild::alignment(BYTE align, CString buf)
{
	switch (align)
	{
	case atLEFT:
		buf += _T("AL_LEFT");		break;
	case atRIGHT:
		buf += _T("AL_RIGHT");		break;
	case atTOP:
		buf += _T("AL_TOP");		break;
	case atBOTTOM:
		buf += _T("AL_BOTTOM");		break;
	case atTILE:
		buf += _T("AL_TILE");		break;
	case atSTRETCH:
		buf += _T("AL_STRETCH");	break;
	case atCENTER:
	default:
		buf += _T("AL_CENTER");		break;
	}
	writeFile(buf,false);
}

void CBuild::hintType(BYTE isHint, CString buf)
{
	CString strEle = buf;
	switch (isHint)
	{
	case htNOP:
		buf += _T("IH_NONE");		break;
	case htSTRING:
		buf += _T("IH_STRING");	break;
	case htREF:
		buf += _T("IH_REF");		break;
	case htFORM:
		buf += _T("IH_MAP");		break;
	case htDESC:
		buf += _T("IH_DESC");		break;
	}

	strEle.Replace("<","</");
	buf += strEle;
	writeWithNL(buf);
}

CString CBuild::parse(CString& src, CString subs)
{
	CString	tmps;
	int	idx = src.Find(subs);
	
	if (idx == -1)
	{
		tmps = src;
		src  = _T("");
	}
	else
	{
		tmps = src.Left(idx);
		src  = src.Mid(idx + subs.GetLength());
	}
	return tmps;
}

void CBuild::tabOrder(WORD idx)
{
	CString	buf;

	buf.Format(_T("<TABORDER>%d</TABORDER>"), idx);
	writeWithNL(buf);
}

void CBuild::ioOrder(WORD idx)
{
	CString	buf;

	buf.Format(_T("<IOORDER>%d</IOORDER>"), idx);
	writeWithNL(buf);
}

void CBuild::groupN(WORD idx)
{
	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(idx);
	CformProp*	Prop = (CformProp*)ObjLoad->getProp();
	if (Prop->group)
	{
		CString	buf;

		CformProp* groupProp = NULL;
		int idx = 0;
		for (int ii = 0; ii < m_mapH->forms.GetCount(); ii++)
		{
			groupProp = ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getProp();
			if (groupProp == Prop->group)
			{
				idx = ii;
			}
		}
		buf.Format(_T("<GROUPN>%d</GROUPN>"), idx);
		writeWithNL(buf);
	}
}

void CBuild::writeEtc(int fN)
{
	bool	etc = false;

	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(fN);
	CformProp*	Prop = ObjLoad->getProp();

	writeFile(_T("<STYLES>"));
	if (Prop->styles & stENABLE)
	{
		if (etc) writeFile(_T("|"), false);
		else etc = true;
		writeFile(_T("ENABLE"), false);
	}

	if (Prop->styles & stVISIBLE)
	{
		if (etc) writeFile(_T("|"), false);
		else etc = true;
		writeFile(_T("VISIBLE"), false);
	}

	if ((Prop->styles & stCHECKED) && (Prop->kind == fmCHECK || Prop->kind == fmRADIO))
	{
		if (etc) writeFile(_T("|"), false);
		else etc = true;
		writeFile(_T("CHECKED"), false);
	}

	if ((Prop->styles & stVERT) && (Prop->kind == fmLABEL || Prop->kind == fmBUTTON || Prop->kind == fmIMVIEW))
	{
		if (etc) writeFile(_T("|"), false);
		else etc = true;
		writeFile(_T("VERTICAL"), false);
	}

	if (Prop->styles & stACCN && Prop->kind == fmCOMBO)
	{
		if (etc) writeFile(_T("|"), false);
		else etc = true;
		writeFile(_T("ACCCBO"), false);
	}

	if (Prop->styles & stTRANSPARENT)
	{
		if (etc) writeFile(_T("|"), false);
		else etc = true;
		writeFile(_T("TRANSPARENT"), false);
	}

	if ((Prop->styles & stIMAGE) && (Prop->kind == fmIMVIEW || Prop->kind == fmBUTTON))
	{
		if (etc) writeFile(_T("|"), false);
		else etc = true;
		writeFile(_T("IMAGE"), false);
	}

	if ((Prop->styles & stIMAGETEXT) && (Prop->kind == fmIMVIEW || Prop->kind == fmBUTTON))
	{
		if (etc) writeFile(_T("|"), false);
		else etc = true;
		writeFile(_T("IMAGETEXT"), false);
	}

	if ((Prop->styles & stLINES) && Prop->kind == fmTREE)
	{
		if (etc) writeFile(_T("|"), false);
		else etc = true;
		writeFile(_T("HASLINE"), false);
	}

	if (Prop->styles & stHOVER && Prop->kind != fmEDIT)
	{
		if (etc) writeFile(_T("|"), false);
		else etc = true;
		writeFile(_T("HOVER"), false);
	}

	if (Prop->styles & stSORT && ( Prop->kind == fmGRID || Prop->kind == fmGRIDEX) )
	{
		if (etc) writeFile(_T("|"), false);
		else etc = true;
		writeFile(_T("SORT"), false);
	}
	writeFile(_T("</STYLES>"), false);
	writeFile(_T("\r\n"), false);
	if (Prop->kind != fmGRID && Prop->kind != fmGRIDEX && Prop->kind != fmTABLE && Prop->kind != fmMEMO)
	{	
		CString	buf;

		buf = _T("<HINTTYPE>");
		hintType(Prop->typeHint, buf);
	}
	
	if (!Prop->hintText.IsEmpty())
	{
		CString	buf;
		buf.Format(_T("<HELPTEXT>\"%s\"</HELPTEXT>"), Prop->hintText);
		writeWithNL(buf);
	}
	
}

void CBuild::writeAuxiliary(int fN)
{
	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(fN);
	CformProp*	Prop = ObjLoad->getProp();

	if ((Prop->styles & stIMAGE) || (Prop->styles & stIMAGETEXT))
	{
		CString	buf;

		addOR(buf);
		if (Prop->styles & stIMAGETEXT)
			buf += _T("IMAGETEXT");
		else
			buf += _T("IMAGE");
		addOR(buf);
		alignment(Prop->alignmentImage, buf);
	}
}

void CBuild::writeColumns(CObArray* arryCellProp)
{
	CString	tmps, buf;
	for (int ii = 0; ii < arryCellProp->GetCount(); ii++)
	{
		writeWithNL("<COLINFO>");
		SetTab(true);
		cellOb* cellProp = (cellOb*)arryCellProp->GetAt(ii);
		buf.Format(_T("<NAME>\"%s\"</NAME>"), cellProp->name);
		writeWithNL(buf);
		buf.Format(_T("<LEN>%d</LEN>"), cellProp->size);
		writeWithNL(buf);
		buf.Format(_T("<WIDTH>%d</WIDTH>"), cellProp->width);
		writeWithNL(buf);
		buf.Format(_T("<RTS>%d</RTS>"), cellProp->flashKeys);
		writeWithNL(buf);
		if (cellProp->kind == ckCOMBO)
		{
			buf.Format(_T("<MATCHKEY>%s</MATCHKEY>"), keyMap[cellProp->keyParam]);
			writeWithNL(buf);
		}
		if (!cellProp->headText.IsEmpty())
		{
			buf.Format(_T("<HEAD>\"%s\"</HEAD>"), cellProp->headText);
			writeWithNL(buf);
		}

		buf = _T("<COLALIGN>");
		switch (cellProp->alignmentHead)
		{
		case atLEFT:
			buf += _T("AL_LEFT");	break;
		case atRIGHT:
			buf += _T("AL_RIGHT");	break;
		case atCENTER:
		default:
			buf += _T("AL_CENTER");	break;
		}
		buf += _T(", ");
		if (cellProp->kind == ckBUTTON && (cellProp->styles & stIMAGE || cellProp->styles & stIMAGETEXT))
		{
			switch (cellProp->alignmentImage)
			{
			case atCENTER:
				buf += _T("AL_CENTER");	break;
			case atTILE:
				buf += _T("AL_TILE");	break;
			case atSTRETCH:
			default:
				buf += _T("AL_STRETCH");break;
			}
		}
		else
		{
			switch (cellProp->alignmentText)
			{
			case atCENTER:
				buf += _T("AL_CENTER");	break;
			case atLEFT:
				buf += _T("AL_LEFT");	break;
			case atRIGHT:
			default:
				buf += _T("AL_RIGHT");	break;
			}
		}
		buf += _T("</COLALIGN>");
		writeWithNL(buf);
		
		buf = _T("<COLHINTTYPE>");
		hintType(cellProp->typeHint, buf);

		if (strlen(cellProp->hintText))
		{
			buf.Format(_T("<COLHELPTEXT>\"%s\"</COLHELPTEXT>"), cellProp->hintText);
			writeWithNL(buf);
		}

		buf.Format(_T("<HEADCOLOR>%d, %d</HEADCOLOR>"), cellProp->headPaintColor, cellProp->headTextColor);
		writeWithNL(buf);

		if (cellProp->offsets)
		{
			cellProp->attributes |= faFLOAT;
			buf.Format(_T("<FLOAT>%d</FLOAT>"), cellProp->offsets);
			writeWithNL(buf);
		}
		else
			cellProp->attributes &= ~faFLOAT;

		writeFile(_T("<TYPE>"));
		dataType(cellProp->charType);
		buf = _T("");
		addOR(buf);
		switch (cellProp->ioType)
		{
		case ioINPUT:
			buf += _T("EDIT_INPUT");	break;
		case ioBOTH:
			buf += _T("EDIT_INOUT");	break;
		case ioOUTPUT:
			buf += _T("EDIT_OUTPUT");	break;
		case ioFREE:
		default:
			buf += _T("EDIT_NOP");		break;
			break;
		}
		if (buf.GetAt(0) == _T('|'))
			buf.Delete(0);
		writeFile(buf, false);

		attributes(cellProp->attributes);
		subattributes(cellProp->attributes, cellProp->auxAttributes);

		buf = _T("");
		if (cellProp->styles & stIMAGE)
		{
			addOR(buf);
			buf += _T("COLUMN_IMAGE");
			writeFile(buf, false);
		}

		if (cellProp->styles & stIMAGETEXT)
		{
			addOR(buf);
			buf += _T("COLUMN_IMGTEXT");
			writeFile(buf, false);
		}

		if (cellProp->styles & stVISIBLE)
		{
			addOR(buf);
			buf += _T("COLUMN_VISIBLE");
			writeFile(buf, false);
		}

		if (cellProp->styles & stSUPRESS)
		{
			addOR(buf);
			buf += _T("SUPRESS");
			writeFile(buf, false);
		}

		if (cellProp->styles & stEDIT)
		{
			addOR(buf);
			buf += _T("COLUMN_SHOWFORM");
			writeFile(buf, false);
		}

		if (cellProp->styles & stSORT)
		{
			addOR(buf);
			buf += _T("COLUMN_SORT");
			writeFile(buf, false);
		}

		if (cellProp->styles & stCHECKED)
		{
			addOR(buf);
			buf += _T("COLUMN_EXCHECKE");
			writeFile(buf, false);
		}

		if (cellProp->styles & stBOLD)
		{
			addOR(buf);
			buf += _T("COLUMN_BOLD");
			writeFile(buf, false);
		}

		switch (cellProp->kind)
		{
		case ckBUTTON:	
			addOR(buf);
			buf += _T("IBUTTON");
			writeFile(buf, false); break;
		case ckCOMBO:
			addOR(buf);
			buf += _T("ICOMBO");
			writeFile(buf, false); break;
		case ckCHECK:
			addOR(buf);
			buf += _T("ICHECK");
			writeFile(buf, false); break;
		case ckCHART:
			addOR(buf);
			buf += _T("IGRAPH");
			writeFile(buf, false); break;
		case ckNORMAL:
			addOR(buf);
			buf += _T("INORMAL");
			writeFile(buf, false); break;
		default:
			break;
		}
		writeFile(_T("</TYPE>"), false);
		writeFile(_T("\r\n"), false);

		color(cellProp->paintColor, cellProp->textColor);
		if (!cellProp->dominoName.IsEmpty())
		{
			buf.Format(_T("<DOM>%s</DOM>"), cellProp->dominoName);
			writeWithNL(buf);
		}

		if (!cellProp->referenceName.IsEmpty())
		{
			buf.Format(_T("<REFERENCE>%s</REFERENCE>"), cellProp->referenceName);
			writeWithNL(buf);
		}

		switch (cellProp->whatEdit)
		{
		case weIT:
			buf.Format(_T("<EDITFORMAT>\"%s\"</EDITFORMAT>"), cellProp->editFormat);
			writeWithNL(buf);
			break;
		case weCORGB:
			buf.Format(_T("<IFRGB>%d, %d</IFRGB>"), HIWORD(cellProp->auxValues), LOWORD(cellProp->auxValues));
			writeWithNL(buf);
			break;
		case weNOP:
		default:
			break;
		}
		
		if (cellProp->kind == ckCOMBO && cellProp->entryCombo != 0)
		{
			buf.Format(_T("<COUNT>%d</COUNT>"), cellProp->entryCombo);
			writeWithNL(buf);
			if (cellProp->entryCombo != 0)
			{
				CString	tmps;
				int	count = cellProp->entryCombo;
				
				buf = _T("<DISPLAY>");
				char*	sDsp = (char *)cellProp->displays.operator LPCTSTR();
				for (int jj = 0; jj < count; jj++)
				{
					tmps.Format(_T("\"%s\""), sDsp);
					if (jj < count - 1)
						tmps += _T(", ");
					buf += tmps;

					sDsp += strlen(sDsp);
					sDsp++;
				}
				buf += _T("</DISPLAY>");
				writeWithNL(buf);
				
				buf = _T("<DATA>");
				char*	sData = (char *)cellProp->strings.operator LPCTSTR();
				for (int jj = 0; jj < count; jj++)
				{
					tmps.Format(_T("\"%s\""), sData);
					if (jj < count - 1)
						tmps += _T(", ");
					buf += tmps;

					sData += strlen(sData);
					sData++;
				}
				buf += _T("</DATA>");
				writeWithNL(buf);
			}
		}

		if (cellProp->kind == ckCHECK && !cellProp->strings.IsEmpty())
		{
			buf.Format(_T("<CHECKSTR>\"%s\"</CHECKSTR>"), cellProp->strings);
			writeWithNL(buf);
		}
		
		if (cellProp->kind == ckBUTTON)
		{
			if (!(cellProp->styles & stIMAGE))
			{
				buf.Format(_T("<BUTTONSTR>\"%s\"</BUTTONSTR>"), cellProp->displays);
				writeWithNL(buf);
			}
			if (cellProp->styles & stIMAGE || cellProp->styles && stIMAGETEXT)
			{
				CString tmps = cellProp->images;
				int	offs = tmps.ReverseFind(_T('\\'));
				if (offs != -1)
					tmps = tmps.Mid(offs+1);
				buf.Format(_T("<BUTTONIMG>\"%s\"</BUTTONIMG>"), tmps);
				writeWithNL(buf);
			}
		}
		SetTab(false);
		writeWithNL("</COLINFO>");
	}
}

void CBuild::writeRows(CObArray* arryCellProp)
{
	CString	tmps, buf;

	//CCellItem *cellItem = new CCellItem(repR);
	for (int ii = 0; ii < arryCellProp->GetCount(); ii++)
	{
		writeWithNL("<COLINFO>");
		SetTab(true);
		cellOb* cellProp = (cellOb*)arryCellProp->GetAt(ii);

		buf.Format(_T("<NAME>\"%s\"</NAME>"), cellProp->name);
		writeWithNL(buf);
		buf.Format(_T("<LEN>%d</LEN>"), cellProp->size);
		writeWithNL(buf);
		buf.Format(_T("<WIDTH>%d</WIDTH>"), cellProp->width);
		writeWithNL(buf);
		buf.Format(_T("<RTS>%d</RTS>"), cellProp->flashKeys);
		writeWithNL(buf);
		if (!cellProp->headText.IsEmpty())
		{
			buf.Format(_T("<HEAD>\"%s\"</HEAD>"), cellProp->headText);
			writeWithNL(buf);
		}

		buf = _T("<COLALIGN>");
		switch (cellProp->alignmentHead)
		{
		case atCENTER:
			buf += _T("AL_CENTER");	break;
		case atRIGHT:
			buf += _T("AL_RIGHT");	break;
		case atLEFT:
		default:
			buf += _T("AL_LEFT");	break;
		}
		buf += _T(", ");
		switch (cellProp->alignmentText)
		{
		case atCENTER:
			buf += _T("AL_CENTER");	break;
		case atLEFT:
			buf += _T("AL_LEFT");	break;
		case atRIGHT:
		default:
			buf += _T("AL_RIGHT");	break;
		}
		buf += _T("</COLALIGN>");
		writeWithNL(buf);

		buf.Format(_T("<HEADCOLOR>%d, %d</HEADCOLOR>"), cellProp->headPaintColor, cellProp->headTextColor);
		writeWithNL(buf);

		if (cellProp->offsets)
		{
			cellProp->attributes |= faFLOAT;
			buf.Format(_T("<FLOAT>%d</FLOAT>"), cellProp->offsets);
			writeWithNL(buf);
		}
		else
			cellProp->attributes &= ~faFLOAT;

		writeFile(_T("<TYPE>"));
		dataType(cellProp->charType);
		buf = _T("");
		addOR(buf);
		attributes(cellProp->attributes);
		subattributes(cellProp->attributes, cellProp->auxAttributes);
		if (cellProp->styles & stVISIBLE)
		{
			addOR(buf);
			buf += _T("COLUMN_VISIBLE");
		}
		writeFile(buf, false);
		writeFile(_T("</TYPE>"), false);
		writeFile(_T("\r\n"), false);
		color(cellProp->paintColor, cellProp->textColor);

		buf = _T("<COLHINTTYPE>");
		hintType(cellProp->typeHint, buf);

		if (strlen(cellProp->hintText))
		{
			buf.Format(_T("<COLHELPTEXT>\"%s\"</COLHELPTEXT>"), cellProp->hintText);
			writeWithNL(buf);
		}

		if (!cellProp->dominoName.IsEmpty())
		{
			buf.Format(_T("<DOM>%s</DOM>"), cellProp->dominoName);
			writeWithNL(buf);
		}

		if (!cellProp->referenceName.IsEmpty())
		{
			buf.Format(_T("<REFERENCE>%s</REFERENCE>"), cellProp->referenceName);
			writeWithNL(buf);
		}

		switch (cellProp->whatEdit)
		{
		case weIT:
			buf.Format(_T("<EDITFORMAT>\"%s\"</EDITFORMAT>"), cellProp->editFormat);
			writeWithNL(buf);
			break;
		case weCORGB:
			buf.Format(_T("<IFRGB>%d, %d</IFRGB>"), HIWORD(cellProp->auxValues), LOWORD(cellProp->auxValues));
			writeWithNL(buf);
			break;
		case weNOP:
		default:
			break;
		}
		SetTab(false);
		writeWithNL("</COLINFO>");
	}
}

void CBuild::writeWithNL(CString buf, bool iscontinue)	// write with newline
{
	if (!buf.IsEmpty())
		writeFile(buf);

	newline(iscontinue);
}

void CBuild::writeFile(CString buf, bool bTab)
{
	if (m_getsource)
	{
		m_source += buf;
		return;
	}

	if (!m_open || buf.IsEmpty())
	{
		if (!m_open)
			AfxMessageBox(_T("file is not opened."));
		return;
	}

	CString	msg;
	TRY
	{
		if (bTab)
		{
			for (int ii = 0; ii < m_iTab; ii++)
			{
				m_file.Write("\t", 1);
			}
		}
		m_file.Write((char *)buf.operator LPCTSTR(), buf.GetLength());
	}
	CATCH (CFileException, e)
	{
		msg.Format(_T("error : write file. [%d]"), e->m_cause);
		AfxMessageBox(msg);
	}
	END_CATCH
}

void CBuild::writeDataToFile(char* buf)
{
	CString text, tmps = CString(buf, (int)strlen(buf));
	while (!tmps.IsEmpty())
	{
		text = parse(tmps, _T("\n"));
		text.Replace(_T("\r"), _T(""));
		if (!text.IsEmpty())
			writeFile(text, false);
		//else
		//	newline(false);
	}
}

void CBuild::writeScriptData(char* dir)
{
	CFile	file;
	DWORD	scpSize = 0;
	DWORD	readLen = 0;
	char*	wb;

	if (!file.Open(dir, CFile::modeRead|CFile::typeBinary))
		return;

	scpSize = (DWORD)file.GetLength();
	if (scpSize < 0)
	{
		AfxMessageBox(_T("error : get script file size"));
		file.Close();
		return;
	}

	wb = new char[scpSize+1];
	readLen = file.Read(wb, scpSize);
	file.Close();

	if (scpSize != readLen)
	{
		AfxMessageBox(_T("error : read script file"));
		delete[] wb;
		return;
	}
	wb[scpSize] = '\0';
	writeDataToFile(wb);
	delete[] wb;
}

void CBuild::addOR(CString& buf)
{
	buf += _T('|');
}

void CBuild::brace(bool begin)
{
	if (begin)
		writeFile(_T("{"));
	else
		writeFile(_T("}"));

	newline(false);
}

void CBuild::space(int count)
{
	CString	sp = CString(_T(' '), count);
	writeFile(sp);
}

void CBuild::newline(bool iscontinue)
{
	CString	buf;

	if (iscontinue)
		writeFile(_T("  &"));

	buf  = (TCHAR)0x0d;
	buf += (TCHAR)0x0a;
	writeFile(buf, false);
}