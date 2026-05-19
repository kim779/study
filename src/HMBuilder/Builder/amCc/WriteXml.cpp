// WriteXml.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "comutil.h"
#include "amCc.h"
#include "WriteXml.h"
//#include "../h/mainvar.h"

//#include "../amCrypt/libCrypt.h"

const char toPath[10][6]	= {_T("AXIS"), _T("BP1"), _T("BP2"), _T("BP3"), _T("BP4"), _T("BP5"), _T("BP6"), _T("BP7"), _T("FEP"), _T("MAXBP")};
const char mapMethod[3][5]	= {_T("FORM"), _T("MENU"), _T("SEND")};
const char fontStyle[5][12]	= {_T(""), _T("NORMAL"), _T("ITALIC"), _T("BOLD"), _T("IBOLD")};
const char keyMap[21][12]	= {_T("NOTMAPPING"), _T("PAGEUP"), _T("PAGEDN"), _T("HOME"), _T("END"), _T("F2"), _T("F3"), _T("F4"), _T("F5"), _T("F6"), _T("F7"), _T("F8"), _T("F9"), _T("F11"), _T("F12"), _T("NUMPAD+"), _T("NUMPAD-"), _T("NUMPAD/"), _T("NUMPAD*"), _T("KEYUP"), _T("KEYDOWN")};
const char mapKind[3][16]	= {_T("NORMAL"), _T("TEMPLATE"), _T("PROCEDURES")};
const char mapBgk[3][6]		= {_T(""), _T("BRUSH"), _T("IMAGE")};

#define DELIMETER	','
// CWriteXml
CWriteXml::CWriteXml()
{
	m_pWriteDoc	= NULL;
	m_bOpen		= false;
	m_pFormItem	= NULL;
	m_mapH		= (struct _mapH*)0;
	m_Xmlpath	= _T("");
	m_Binpath	= _T("");
	m_FormElement	= NULL;
	m_TrLayoutElement = NULL;
	m_RtsSymbolElement = NULL;
	m_LayoutElement	= NULL;
	m_writeType	= NONE;
	m_status	= NULL;
	m_inputidx	= 0;
	m_outputidx	= 0;

	m_trlayoutxml.RemoveAll();
	m_rtssymbolxml.RemoveAll();

	m_control = NULL;
}

CWriteXml::~CWriteXml()
{
	if (m_bOpen)
		m_file.Close();

	if (m_pWriteDoc)
	{
		m_pWriteDoc->Release();
		m_pWriteDoc = NULL;
	}

	if (m_pFormItem)
		delete m_pFormItem;

	if (m_status)
		delete m_status;

	CString key, dat;

	removeIXMLDOMElement(m_FormElement); 
	removeIXMLDOMElement(m_TrLayoutElement);
	removeIXMLDOMElement(m_RtsSymbolElement);
	removeIXMLDOMElement(m_LayoutElement);

	m_trlayoutxml.RemoveAll();
	m_rtssymbolxml.RemoveAll();
}

CString CWriteXml::GetSource(bool bNotResource, struct _mapH* mapH)
{
	CString	tmps = _T("");
	
	return m_sSrc;
}

int CWriteXml::GenerateSource(bool alarm, struct _mapH* mapH, bool getsource)
{
	HRESULT				hr;
	CString				tmp, spath;
	IXMLDOMProcessingInstruction*	pProcess = NULL;
	IXMLDOMElement*			pRootElement = NULL;
	IXMLDOMElement*			pChildElement = NULL;

	m_Xmlpath = _T("");
	m_Binpath = _T("");

	if (mapH->mapN[0] == NULL)
	{
		TRACE("GenerateSource::mapN is empty!!\n");
		return -1;
	}

	((CamCcApp *)AfxGetApp())->SetTempPath(spath, mapH->mapN);
	m_Xmlpath.Format(_T("%s%s"), spath, _T(".xml"));

	CFileFind file;
	if (file.FindFile(m_Xmlpath))
		DeleteFile(m_Xmlpath);
//TRACE("----------m_Xmlpath---------------%s\n", m_Xmlpath);

	CoInitialize(NULL);
	CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument2, (void**)&m_pWriteDoc);

	hr = m_pWriteDoc->createProcessingInstruction(_bstr_t(_T("xml")), _bstr_t(_T("version=\"1.0\" encoding=\"utf-8\"")), &pProcess);
	if (hr != S_OK)	
	{
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("\nCan't create xml.\n\nMapName Error!"), _T("Message"), MB_ICONEXCLAMATION|MB_OK);
		return -1;
	}

	((CamCcApp *)AfxGetApp())->SetTempPath(spath, mapH->mapN);
	m_Binpath = spath;

//TRACE("----------m_Binpath---------------%s\n", m_Binpath);

	m_bOpen = (m_file.Open(m_Binpath, CFile::modeCreate|CFile::modeWrite)) ? true : false;
	if (!m_bOpen)
	{
		pProcess->Release(); pProcess = NULL;
		::MessageBox(AfxGetMainWnd()->m_hWnd, "\nCan't create Source.\n\nMapName Error!", "Message", MB_ICONEXCLAMATION|MB_OK);
		return -1;
	}

	m_bSrc = getsource;
	m_bOpen = !getsource;

	hr = m_pWriteDoc->appendChild(pProcess, NULL);
	if (hr != S_OK)
	{
		pProcess->Release(); pProcess = NULL; m_file.Close();CoUninitialize();
		return -1;
	}

	hr = m_pWriteDoc->createElement(_bstr_t(_T("FORM")), &pRootElement);
	if (hr != S_OK)
	{
		pProcess->Release(); pProcess = NULL; m_file.Close();CoUninitialize();
		return -1;
	}

	m_mapH  = mapH;

	hr = m_pWriteDoc->appendChild(pRootElement, NULL);
	if (hr == S_OK)
	{
		tmp.Format(_T("%s"), m_mapH->mapN);
		SetElementToAttribute(pRootElement, _T("NAME"), tmp);
	}

	if (m_pFormItem)
		delete m_pFormItem;
	m_pFormItem = new CFormItem(mapH);

	char* ElementName[] = {_T("GLOBAL"), _T("CONTROL"), _T("TRLAYOUT"), _T("RTSSYMBOL"), _T("LAYOUT"), NULL};

	for (int ii = 0; ElementName[ii] != NULL; ii++)
	{
		hr = m_pWriteDoc->createElement(_bstr_t(CString(ElementName[ii])), &pChildElement);

		if (hr != S_OK) continue;

		pRootElement->appendChild(pChildElement, NULL);

		if (!CString(ElementName[ii]).CompareNoCase(_T("GLOBAL")))
		{
			GenerateHeader(pChildElement);
			generateFormScript(pChildElement);
		}
		else if (!CString(ElementName[ii]).CompareNoCase(_T("CONTROL")))
		{
			m_FormElement = pChildElement;
			GenerateForm();
		}
		else if (!CString(ElementName[ii]).CompareNoCase(_T("TRLAYOUT")))
		{
			m_TrLayoutElement = pChildElement;
			writeTrlayout();
		}
		else if (!CString(ElementName[ii]).CompareNoCase(_T("RTSSYMBOL")))
		{
			m_RtsSymbolElement = pChildElement;
			writeRtsSymbol();
		}
		else if (!CString(ElementName[ii]).CompareNoCase(_T("LAYOUT")))
		{
			m_LayoutElement = pChildElement;
			writeLayout();
		}
	}
	m_file.Close();	m_bOpen	= false;

//Encrypt(m_Binpath);

	hr = m_pWriteDoc->save(CComVariant((LPCTSTR)m_Xmlpath));
	if (hr == S_OK)
	{
		mapH->source = true;
		mapH->modified = false;

		if (alarm)
		{
			tmp.Format(_T("\nXML file is created.\n\nMapName : %s"), m_mapH->mapN);
			::MessageBox(AfxGetMainWnd()->m_hWnd, tmp, _T("Message"), MB_ICONEXCLAMATION|MB_OK);
		}
	}
	else
		AfxMessageBox(_T("Create XMLfile false!"));

	pProcess->Release(); pProcess = NULL;

	removeIXMLDOMElement(pChildElement);
	removeIXMLDOMElement(pRootElement);

	CoUninitialize();

	return 0;
}

void CWriteXml::GenerateHeader(IXMLDOMElement* pElement)
{
	CString tmp;
	bool	first;

	m_writeType = PROPERT;
	writeFile(_T("axForm"));

	if (m_mapH->mapN[0] != NULL)
	{
		tmp.Format(_T("%s"), m_mapH->mapN);
		MakeElement(pElement, _T("MAPNAME"), tmp);
	}
	else
	{
		writeFile();
	}

	tmp.Format(_T("%d"), m_mapH->height);		MakeElement(pElement, _T("HEIGHT"), tmp);
	tmp.Format(_T("%d"), m_mapH->width);		MakeElement(pElement, _T("WIDTH"), tmp);
	tmp.Format(_T("%s"), mapKind[m_mapH->mapK]);	MakeElement(pElement, _T("MAPKIND"), tmp, false);
	tmp.Format(_T("%d"), m_mapH->mapK);		writeFile(tmp);
	
	if (m_mapH->mapdesc[0] != NULL)
	{
		tmp.Format(_T("%s"), m_mapH->mapdesc);	MakeElement(pElement, _T("MAPDESC"), tmp);
	}
	else
	{
		writeFile();
	}

	if (m_mapH->fName[0] != NULL)
	{
		tmp.Format(_T("%s"), m_mapH->fName);		MakeElement(pElement, _T("FONTNAME"), tmp);
		tmp.Format(_T("%d"), m_mapH->fPoint);		MakeElement(pElement, _T("FONTPOINT"), tmp);
		tmp.Format(_T("%s"), fontStyle[m_mapH->fStyle]);MakeElement(pElement, _T("FONTSTYLE"), tmp, false);
		tmp.Format(_T("%d"), m_mapH->fStyle);		writeFile(tmp);
	}
	else
	{
		writeFile();
		writeFile();
		writeFile();
	}

	first = true;
	if (m_mapH->options)
	{
		tmp = _T("");
		if (m_mapH->options & OP_LEDGER)
		{
			if (!first) tmp += _T(","); tmp += _T("LEDGER"); first = false;
		}
		if (m_mapH->options & OP_OOP)
		{
			if (!first) tmp += _T(","); tmp += _T("FMS");	first = false;
		}
		if (m_mapH->options & OP_FLASH)
		{
			if (!first) tmp += _T(","); tmp += _T("FLASH");	first = false;
		}
		if (m_mapH->options & OP_DOMINO)
		{
			if (!first) tmp += _T(","); tmp += _T("DOMINO");first = false;
		}
		if (m_mapH->options & OP_TABS)
		{
			if (!first) tmp += _T(","); tmp += _T("TABS");	first = false;
		}
		if (m_mapH->options & OP_CR2TB)
		{
			if (!first) tmp += _T(","); tmp += _T("CR2TB");	first = false;
		}
		if (m_mapH->options & OP_ENC)
		{
			if (!first) tmp += _T(","); tmp += _T("ENC");	first = false;
		}
		if (m_mapH->options & OP_CERTIFY)
		{
			if (!first) tmp += _T(","); tmp += _T("CERTIFY");first = false;
		}
		if (m_mapH->options & OP_SAVE)
		{
			if (!first) tmp += _T(","); tmp += _T("SAVE");	first = false;
		}
		MakeElement(pElement, _T("OPTIONS"), tmp, false);

		tmp.Format(_T("%d"), m_mapH->options);
		writeFile(tmp);
	}
	else
		writeFile();

	if (m_mapH->caption[0] != NULL)
	{
		tmp.Format(_T("%s"), m_mapH->caption); MakeElement(pElement, _T("CAPTION"), tmp);
	}
	else
	{
		writeFile();
	}

	tmp.Format(_T("%s"), toPath[m_mapH->toPath]);	MakeElement(pElement, _T("TOPATH"), tmp, false);
	tmp.Format(_T("%d"), m_mapH->toPath);		writeFile(tmp);

	tmp.Format(_T("%s"), mapMethod[m_mapH->mapM]);	MakeElement(pElement, _T("METHOD"), tmp, false);
	tmp.Format(_T("%d"), m_mapH->mapM);		writeFile(tmp);

	if (m_mapH->trxC[0] != NULL)
	{
		tmp.Format(_T("%s"), m_mapH->trxC);	MakeElement(pElement, _T("TRXCODE"), tmp);
	}
	else
	{
		writeFile();
	}

	if (m_mapH->repeatV != 0)
	{
		tmp.Format(_T("%d"), m_mapH->repeatV);	MakeElement(pElement, _T("REPEATTIME"), tmp);
	}
	else
	{
		writeFile();
	}

	tmp.Format(_T("%d"), m_mapH->flashClr);		MakeElement(pElement, _T("FLASHCOLOR"), tmp);

	if (m_mapH->flashSym[0] != NULL)
	{
		tmp.Format(_T("%s"), m_mapH->flashSym);	MakeElement(pElement, _T("FLASHSYM"), tmp);
	}
	else
	{
		writeFile();
	}

	tmp.Format(_T("%d"), m_mapH->upClr);		MakeElement(pElement, _T("UPCOLOR"), tmp);
	tmp.Format(_T("%d"), m_mapH->dnClr);		MakeElement(pElement, _T("DNCOLOR"), tmp);

	if (m_mapH->contrastN[0] != NULL)
	{
		tmp.Format(_T("%s"), m_mapH->contrastN);MakeElement(pElement, _T("CONTRAST"), tmp);
	}
	else
	{
		writeFile();
	}

	tmp.Format(_T("%s"), _T("1"));	MakeElement(pElement, _T("BGKIND"), tmp, false);
	tmp.Format(_T("%d"), 1);	writeFile(tmp);

	if (m_mapH->bgFileN[0] != NULL)
	{
		tmp.Format(_T("%s"), m_mapH->bgFileN);	MakeElement(pElement, _T("BGFILEN"), tmp);
	}
	else
	{
		writeFile();
	}

	tmp.Format(_T("%d"), m_mapH->bgClr);	MakeElement(pElement, _T("BGCOLOR"), tmp);	
	tmp.Format(_T("%d"), m_mapH->formtype);	MakeElement(pElement, _T("FORMTYPE"), tmp);

	tmp.Format(_T("%d"), m_mapH->alpha);	MakeElement(pElement, _T("MAPALPHA"), tmp);

	m_writeType = PROPERTLAST;
	tmp.Format(_T("%s"), m_mapH->formCtrl);

	if (tmp.GetLength() > 0)
		MakeElement(pElement, _T("FORMCTRL"), tmp);
// 	else
// 		writeFile();
}

void CWriteXml::GenerateForm()
{
	CString name, aValue;

	for (int ii = 0; ii < m_pFormItem->GetCount(); ii++)
	{
		m_writeType = PROPERT;

		switch (m_pFormItem->GetForm(ii)->kind)
		{
		case FM_LABEL:
			writeFile(_T("axLabel"));	srcLabel(ii, m_FormElement);	break;
		case FM_BOX:
			writeFile(_T("axBox"));		srcBox(ii, m_FormElement);	break;
		case FM_GROUP:
			break;
			//writeFile(_T("AXGROUP"));	srcGroup(ii, m_FormElement);	break;
		case FM_IMAGEVIEW:
			writeFile(_T("axImageView"));	srcImageVW(ii, m_FormElement);	break;
		case FM_BUTTON:
			writeFile(_T("axButton"));	srcButton(ii, m_FormElement);	break;
		case FM_CHECK:
			writeFile(_T("axCheck"));	srcCheck(ii, m_FormElement);	break;
		case FM_RADIO:
			writeFile(_T("axRadio"));	srcRadio(ii, m_FormElement);	break;
		case FM_EDIT:
			writeFile(_T("axEdit"));	srcEdit(ii, m_FormElement);	break;
		case FM_COMBO:
			writeFile(_T("axCombo"));	srcCombo(ii, m_FormElement);	break;
		case FM_OUT:
			writeFile(_T("axOutput"));	srcEdit(ii, m_FormElement);	break;
		case FM_GRID:
			writeFile(_T("axGrid"));	srcGrid(FM_GRID, ii, m_FormElement);	break;			
			break;
		case FM_GRIDEX:
			writeFile(_T("axGridEx"));	srcGrid(FM_GRIDEX, ii, m_FormElement);	break;
		case FM_TABLE:
			writeFile(_T("axTable"));	srcTable(ii, m_FormElement);	break;
		case FM_OBJECT:
			writeFile(_T("axObject"));	srcObject(ii, m_FormElement);	break;
		case FM_TAB:
			writeFile(_T("axTab"));		srcTab(ii, m_FormElement);	break;
		case FM_BROWSER:
			writeFile(_T("axWebView"));	srcBrowser(ii, m_FormElement);	break;
		case FM_CONTROL:
			writeFile(_T("axControl"));	srcControl(ii, m_FormElement);	break;
		default:
			break;
		}
	}
}

void CWriteXml::writeTrlayout()
{
	CString tmp;
	CString sKey, str;

	/*for (POSITION pos = ((CamCcApp *)AfxGetApp())->m_trlayout.GetStartPosition(); pos != NULL; )
	{
		((CamCcApp *)AfxGetApp())->m_trlayout.GetNextAssoc(pos, sKey, str);
		m_writeType = TRLAYOUT;
		tmp = Parser(str, _T("\t"));
TRACE("-----------writeTrlayout--------------%s\n", str);
		writeFile(str);

		bwriteruntime = true;
	}*/

	if (m_mapH->trinfo == NULL)
		return;

	CString trinfo = CString(m_mapH->trinfo, strlen(m_mapH->trinfo));
	while (!trinfo.IsEmpty())
	{
		int pos = trinfo.Find(_T("^"));
		if (pos != -1)
		{
			str = Parser(trinfo, _T("^"));
			tmp = Parser(str, _T("\t"));
//TRACE("-----------writeTrlayout222222--------------%s\t%s\n", tmp, str);
			convertRuntimexml(tmp, str);
			MakeElement(m_TrLayoutElement, tmp, str, false);
		}
		else
			trinfo = _T("");
	}

	for (POSITION pos = ((CamCcApp *)AfxGetApp())->m_trlayoutxml.GetStartPosition(); pos != NULL; )
	{
		((CamCcApp *)AfxGetApp())->m_trlayoutxml.GetNextAssoc(pos, sKey, str);
		tmp = Parser(str, _T("\t"));
//TRACE("-----------writeTrlayout111111--------------%s\t%s\n", tmp, str);
		//convertRuntimexml(tmp, str);

		//MakeElement(m_TrLayoutElement, tmp, str, false);
	}

	/*for (POSITION pos = m_trlayoutxml.GetStartPosition(); pos != NULL; )
	{
		m_trlayoutxml.GetNextAssoc(pos, sKey, str);
		tmp = Parser(str, _T("\t"));
TRACE("-----------writeTrlayout2--------------%s\n", str);
		convertRuntimexml(tmp, str);

		MakeElement(m_TrLayoutElement, tmp, str, false);
	}*/
}

void CWriteXml::convertRuntimexml(CString  key, CString data)
{
	CString tmp, tmps, trlay, gubn;

	tmps.Format(_T("%s%c%s%c"), _T("TRINFO"), GB_PROPERT, key, GB_PROPERT);
	trlay = Parser(data, _T("*"));

	while (!trlay.IsEmpty())
	{
		tmp = Parser(trlay, _T("/"));
		if (!tmp.IsEmpty())
		{
			// RuntimeXML에는 TR Description이 저장되지 않아야한다.
			ParserDescription(tmp, "=");
			tmps += tmp;
			if (!trlay.IsEmpty())
			{
				gubn.Format(_T("%c"), GB_PROPERT);
				tmps += gubn;
			}
		}
	}

	gubn.Format(_T("%c"), GB_SCRIPT);
	tmps += gubn;

	while (!data.IsEmpty())
	{
		tmp = Parser(data, _T("/"));
		if (!tmp.IsEmpty())
		{
			// RuntimeXML에는 TR Description이 저장되지 않아야한다.
			ParserDescription(tmp, "=");
			tmps += tmp;
			gubn.Format(_T("%c"), GB_PROPERT);
			tmps += gubn;
		}
	}

	gubn.Format(_T("%c"), GB_CLASS);
	tmps += gubn;

	m_writeType = TRLAYOUT;
	writeFile(tmps);

//TRACE(_T("convertRuntimexml[%s]\n"), tmps);
}

CString CWriteXml::ParserDescription(CString &srcstr, CString substr)
{
	int findidx = 0, indexcnt = 0;
	// "=" 두개면 Description 존재
	for (;;)
	{
		findidx = srcstr.Find(substr, findidx);
		if (findidx > 0)
		{
			findidx++;
			
			if (++indexcnt > 1)
			{
				CString temp = srcstr;
				srcstr = srcstr.Left(findidx-1);
				return temp.Mid(findidx);
			}
		}
		else
			break;
	}
	return _T("");
}


void CWriteXml::writeRtsSymbol()
{
	CString tmp;
	CString sKey, str;

	/*for (POSITION pos = ((CamCcApp *)AfxGetApp())->m_rtssymbol.GetStartPosition(); pos != NULL; )
	{
		((CamCcApp *)AfxGetApp())->m_rtssymbol.GetNextAssoc(pos, sKey, str);
		m_writeType = TRLAYOUT;
		tmp = Parser(str, _T("\t"));
		writeFile(str);
	}*/

	if (m_mapH->rtsinfo == NULL)
		return;

	CString rtsinfo = CString(m_mapH->rtsinfo, strlen(m_mapH->rtsinfo));
	CString rtsinfo2 = CString(m_mapH->rtsinfo, strlen(m_mapH->rtsinfo));
	while (!rtsinfo.IsEmpty())
	{
		int pos = rtsinfo.Find(_T("^"));
		if (pos != -1)
		{
			str = Parser(rtsinfo, _T("^"));
			tmp = Parser(str, _T("\t"));			
//TRACE("-----------writeTrlayout33333--------------%s\t%s\n", tmp, str);
			convertRuntimerts(tmp, str);
			//tmp.Replace(_T("("), _T("::")); tmp.Replace(_T(")"), _T(":."));
			//MakeElement(m_RtsSymbolElement, tmp, str, false);
			//tmp += _T("\t") + str;
			//MakeElement(m_RtsSymbolElement, _T("AAAA :fdf@"), tmp, false);
		}
		else
			rtsinfo = _T("");
	}
	MakeElement(m_RtsSymbolElement, _T("RTSINFO"), rtsinfo2, false);

	for (POSITION pos = ((CamCcApp *)AfxGetApp())->m_rtssymbolxml.GetStartPosition(); pos != NULL; )
	{
		((CamCcApp *)AfxGetApp())->m_rtssymbolxml.GetNextAssoc(pos, sKey, str);
		tmp = Parser(str, _T("\t"));
//TRACE("-----------writeTrlayout22222--------------%s\t%s\n", tmp, str);
		//convertRuntimerts(tmp, str);
		//MakeElement(m_RtsSymbolElement, tmp, str, false);
	}
}

//LAYOUT0x0cCURRLAYOUT0x0cCODE,0,0,80,40,TRUE0x0cDATE,0,80,80,40,TRUE0x0cNAME,0,160,80,40,FALSE0x08
void CWriteXml::writeLayout()
{	
	struct _formR* formR;
	struct _Layout* layout;
	CString tmps, sKey, sIndex, gubn;
	CString xml, rxml;

	if (!m_pFormItem->m_mapH->maplayout)
		return;

	int nCnt = m_pFormItem->m_mapH->maplayout->GetCount();

	CString *pTmp, *pTmps, *pKey, *pIndex;
	pTmp = new CString[nCnt-1];
	pTmps = new CString[nCnt-1];
	pKey = new CString[nCnt-1];
	pIndex = new CString[nCnt-1];

	int idx = 0;
	
	for (int ii = 0; ii < m_pFormItem->GetCount(); ii++)
	{			
		formR = m_pFormItem->GetForm(ii);
		
		idx = 0;
		for (POSITION pos = m_pFormItem->m_mapH->maplayout->GetStartPosition(); pos;)
		{
			m_pFormItem->m_mapH->maplayout->GetNextAssoc(pos, sIndex, sKey);

			formR->layout->Lookup(sKey, (void*&)layout);

			if (!sKey.CompareNoCase("Original"))
				continue;

			pKey[idx] = sKey;
			pIndex[idx] = sIndex;
			pTmp[idx].Format("%s,%d,%d,%d,%d,%d%c", 
				formR->name, layout->left, layout->top, layout->right - layout->left,
				layout->bottom - layout->top, layout->properties , GB_PROPERT);
			pTmps[idx] += pTmp[idx];
			idx++;
		}
	}

	xml.Format("LAYOUT%c", GB_PROPERT);
	//rxml = xml;
	for (int ii = 0; ii < nCnt - 1; ii++)
	{
		tmps.Format("LAYOUT%c%s%c%s", GB_PROPERT, pKey[ii], GB_PROPERT, pTmps[ii]);
		rxml += tmps;
		tmps.Format("%s::%s%c%s", pIndex[ii], pKey[ii], GB_PROPERT, pTmps[ii]);
		xml += tmps;
		
		xml = xml.Left(xml.ReverseFind(GB_PROPERT));
		rxml = rxml.Left(rxml.ReverseFind(GB_PROPERT));
		
		tmps.Format("%c", GB_CLASS);
		xml += tmps;
		rxml += tmps;
	}

	gubn.Format("%c", GB_PROPERT);
	xml.Replace(gubn, "::");
	gubn.Format("%c", GB_CLASS);
	xml.Replace(gubn, ":.");

	MakeElement(m_LayoutElement, _T("LAYOUT"), xml, false);
	m_writeType = TRLAYOUT;
	writeFile(rxml);

	delete[] pTmp;
	delete[] pTmps;
	delete[] pKey;
	delete[] pIndex;
}

void CWriteXml::convertRuntimerts(CString  key, CString data)
{
	CString tmp, tmps, skey, type;
	bool isGrid = false;

	if (data.IsEmpty())
		return;

	skey = Parser(data, _T("/"));
	tmp += skey;
	tmps.Format(_T("%c"), GB_PROPERT);
	tmp += tmps;
	tmp += Parser(data, _T("|"));
	tmp += _T("|");

	if (key.Find(_T("(")) != -1)
	{
		Parser(key, _T("("));
		type = Parser(key, _T(")"));
		if (type.Find(_T("GRID")) == 0)	// GRID or GRIDEX
			isGrid = true;
	}

	tmp += makertsinfo(data, isGrid);//data;
	tmps.Format(_T("%c"), GB_CLASS);
	tmp += tmps;

//TRACE("convertRuntimerts[%s]\n", tmp);
	m_writeType = TRLAYOUT;
	writeFile(tmp);
}

CString CWriteXml::makertsinfo(CString data, bool isGrid)
{
	CString tmp, tmps, retvalue;
	int	pos = -1;

	while (!data.IsEmpty())
	{
		pos = -1;
		tmp = Parser(data, _T(","));

		// Grid명.Column 일경우 그리드명 제거
		if (isGrid)
			Parser(tmp, _T("."));

		if (tmp.Find(_T("$")) >= 0)
		{
			pos = tmp.Find(_T("."));
			tmps = tmp.Mid(pos + 1);
			retvalue += tmps;
		}
		else
			retvalue += tmp;

		retvalue += _T(",");
	}
	return retvalue;
}

void CWriteXml::srcLabel(int fN, IXMLDOMElement* pElement)
{
	CString		tmp;
	IXMLDOMElement*	pChildElement;

	pChildElement = BaseWrite(_T("FM_LABEL"), fN, pElement);
	writeEtc(fN, pChildElement);

	if (!m_pFormItem->GetFormStr(fN, _T("str")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("str")));
		MakeElement(pChildElement, _T("STR"), tmp);
	}
	else
		writeFile();

	writeAuxiliary(m_pFormItem->GetForm(fN)->properties, pChildElement);
	alignmentImage(m_pFormItem->GetForm(fN)->alignImage, pChildElement);

	if (!m_pFormItem->GetFormStr(fN, _T("str2")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("str2")));
		MakeElement(pChildElement, _T("STR2"), tmp);
	}
	else
		writeFile();

	if (!m_pFormItem->GetFormStr(fN, _T("desc")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("desc")));
		MakeElement(pChildElement, _T("DESC"), tmp);
	}
	else
		writeFile();

	alignment(m_pFormItem->GetForm(fN)->alignment, pChildElement);
	formFont(fN, pChildElement);
	attributes(m_pFormItem->GetForm(fN)->attr, pChildElement);
	formcolor(m_pFormItem->GetForm(fN)->pRGB, m_pFormItem->GetForm(fN)->tRGB, pChildElement);

	writeBoders(fN, pChildElement);

	tmp.Format(_T("%ld"), RGBtoARGB(m_pFormItem->GetForm(fN)->bRGB));
	MakeElement(pChildElement, _T("LINECOLOR"), tmp);

	SetpixFont(fN);

	// alpha
	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->alpha);
	MakeElement(pChildElement, _T("ALPHA"), tmp);

	// margin
	m_writeType = PROPERTLAST;
	if (m_pFormItem->GetForm(fN)->margin)
	{
		tmp = _T("");
		for (int ii = 0; ii < m_pFormItem->GetForm(fN)->margin->GetCount(); ii++)
		{
			tmp += m_pFormItem->GetForm(fN)->margin->GetAt(ii);
			if (ii < m_pFormItem->GetForm(fN)->margin->GetUpperBound())
				tmp += _T(',');
		}
		MakeElement(pChildElement, _T("MARGIN"), tmp);
	}
	else
		writeFile();

	generateControlScript(fN, pChildElement);
	removeIXMLDOMElement(pChildElement);
}

void CWriteXml::srcBox(int fN, IXMLDOMElement* pElement)
{
	CString		tmp;
	IXMLDOMElement*	pChildElement;

	pChildElement = BaseWrite(_T("FM_BOX"), fN, pElement);
	writeEtc(fN, pChildElement);

	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->size);
	MakeElement(pChildElement, _T("LEN"), tmp);

	writeAuxiliary(m_pFormItem->GetForm(fN)->properties, pChildElement);
	attributes(m_pFormItem->GetForm(fN)->attr, pChildElement);

	tmp.Format(_T("%ld"), m_pFormItem->GetForm(fN)->pRGB);
	MakeElement(pChildElement, _T("PRGB"), tmp);	
	
	tmp.Format(_T("%ld"), m_pFormItem->GetForm(fN)->tRGB);
	MakeElement(pChildElement, _T("TRGB"), tmp);

	m_writeType = PROPERTLAST;
	// alpha
	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->alpha);
	MakeElement(pChildElement, _T("ALPHA"), tmp);

	generateControlScript(fN, pChildElement);

	removeIXMLDOMElement(pChildElement);
}

void CWriteXml::srcGroup(int fN, IXMLDOMElement* pElement)
{
	CString		tmp;
	IXMLDOMElement*	pChildElement;

	pChildElement = BaseWrite(_T("FM_GROUP"), fN, pElement);
	writeEtc(fN, pChildElement);

	if (!m_pFormItem->GetFormStr(fN, _T("str")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("str")));
		MakeElement(pChildElement, _T("STR"), tmp);
	}
	else
		writeFile();

	alignment(m_pFormItem->GetForm(fN)->alignment, pChildElement);
	formFont(fN, pChildElement);
	writeAuxiliary(m_pFormItem->GetForm(fN)->properties, pChildElement);
	attributes(m_pFormItem->GetForm(fN)->attr, pChildElement);
	m_pFormItem->GetForm(fN)->pRGB = -1;

	formcolor(m_pFormItem->GetForm(fN)->pRGB, m_pFormItem->GetForm(fN)->tRGB, pChildElement);

	m_writeType = PROPERTLAST;
	SetpixFont(fN);

	generateControlScript(fN, pChildElement);

	removeIXMLDOMElement(pChildElement);
}

void CWriteXml::srcImageVW(int fN, IXMLDOMElement* pElement)
{
	CString		tmp;
	IXMLDOMElement*	pChildElement;

	pChildElement = BaseWrite(_T("FM_IMAGEVIEW"), fN, pElement);
	writeEtc(fN, pChildElement);

	//if (!m_pFormItem->GetFormStr(fN, _T("str")).IsEmpty())
	//{
	//	tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("str")));
	//	MakeElement(pChildElement, _T("STR"), tmp);
	//}
	//else
		writeFile();

	if (!m_pFormItem->GetFormStr(fN, _T("str2")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("str2")));
		MakeElement(pChildElement, _T("STR2"), tmp);
	}
	else
		writeFile();

	if (!m_pFormItem->GetFormStr(fN, _T("desc")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("desc")));
		MakeElement(pChildElement, _T("DESC"), tmp);
	}
	else
		writeFile();

	writeAuxiliary(m_pFormItem->GetForm(fN)->properties, pChildElement);
	alignmentImage(m_pFormItem->GetForm(fN)->alignImage, pChildElement);

	tmp.Format(_T("%ld"), m_pFormItem->GetForm(fN)->pRGB);
	MakeElement(pChildElement, _T("PRGB"), tmp, false);
	
	tmp.Format(_T("%ld"), RGBtoARGB(m_pFormItem->GetForm(fN)->pRGB));
	writeFile(tmp);

	writeBoders(fN, pChildElement);
		
	tmp.Format(_T("%ld"), RGBtoARGB(m_pFormItem->GetForm(fN)->bRGB));
	MakeElement(pChildElement, _T("LINECOLOR"), tmp);

	m_writeType = PROPERTLAST;
	// alpha
	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->alpha);
	MakeElement(pChildElement, _T("ALPHA"), tmp);

	generateControlScript(fN, pChildElement);

	removeIXMLDOMElement(pChildElement);
}

void CWriteXml::srcButton(int fN, IXMLDOMElement* pElement)
{
	CString		tmp;
	IXMLDOMElement*	pChildElement;

	pChildElement = BaseWrite(_T("FM_BUTTON"), fN, pElement);
	writeEtc(fN, pChildElement);

	if (!m_pFormItem->GetFormStr(fN, _T("str")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("str")));
		MakeElement(pChildElement, _T("STR"), tmp);
	}
	else
		writeFile();

	if (!m_pFormItem->GetFormStr(fN, _T("str2")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("str2")));
		MakeElement(pChildElement, _T("STR2"), tmp);
	}
	else
		writeFile();

	if (!m_pFormItem->GetFormStr(fN, _T("desc")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("desc")));
		MakeElement(pChildElement, _T("DESC"), tmp);
	}
	else
		writeFile();

	if (!(m_pFormItem->GetForm(fN)->properties & PR_IMAGE))
	{
		if (!(m_pFormItem->GetForm(fN)->properties & PR_IMAGETEXT))
			alignment(m_pFormItem->GetForm(fN)->alignment, pChildElement);
		else
			writeFile();
	}
	else
		writeFile();

	formFont(fN, pChildElement);
	writeAuxiliary(m_pFormItem->GetForm(fN)->properties, pChildElement);
	alignmentImage(m_pFormItem->GetForm(fN)->alignImage, pChildElement);
	attributes(m_pFormItem->GetForm(fN)->attr, pChildElement);
	formcolor(m_pFormItem->GetForm(fN)->pRGB, m_pFormItem->GetForm(fN)->tRGB, pChildElement);

	taborder(m_pFormItem->GetForm(fN)->index, pChildElement);

	tmp.Format(_T("%ld"), RGBtoARGB(m_pFormItem->GetForm(fN)->spRGB));
	MakeElement(pChildElement, _T("SPRGB"), tmp);
		
	SetpixFont(fN);

	// alpha
	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->alpha);
	MakeElement(pChildElement, _T("ALPHA"), tmp);

	if (!m_pFormItem->GetFormStr(fN, _T("optionimg")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("optionimg")));
		MakeElement(pChildElement, _T("OPTIONIMG"), tmp);
	}
	else
		writeFile();

	if (m_pFormItem->GetForm(fN)->margin)
	{
		tmp = _T("");
		for (int ii = 0; ii < m_pFormItem->GetForm(fN)->margin->GetCount(); ii++)
		{
			tmp += m_pFormItem->GetForm(fN)->margin->GetAt(ii);
			if (ii < m_pFormItem->GetForm(fN)->margin->GetUpperBound())
				tmp += _T(',');
		}
		MakeElement(pChildElement, _T("MARGIN"), tmp);
	}
	else
		writeFile();

	m_writeType = PROPERTLAST;
	tmp.Format(_T("%ld"), RGBtoARGB(m_pFormItem->GetForm(fN)->stRGB));
	MakeElement(pChildElement, _T("STRGB"), tmp);

	generateControlScript(fN, pChildElement);

	removeIXMLDOMElement(pChildElement);
}

void CWriteXml::srcCheck(int fN, IXMLDOMElement* pElement)
{
	CString		tmp;
	IXMLDOMElement*	pChildElement;

	pChildElement = BaseWrite(_T("FM_CHECK"), fN, pElement);
	writeEtc(fN, pChildElement);

	if (!m_pFormItem->GetFormStr(fN, _T("str")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("str")));
		MakeElement(pChildElement, _T("STR"), tmp);
	}
	else
		writeFile();

	if (!m_pFormItem->GetFormStr(fN, _T("str2")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("str2")));
		MakeElement(pChildElement, _T("STR2"), tmp);
	}
	else
		writeFile();

	if (!m_pFormItem->GetFormStr(fN, _T("desc")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("desc")));
		MakeElement(pChildElement, _T("DESC"), tmp);
	}
	else
		writeFile();

	if (!(m_pFormItem->GetForm(fN)->properties & PR_IMAGE))
	{
		if (!(m_pFormItem->GetForm(fN)->properties & PR_IMAGETEXT))
			alignment(m_pFormItem->GetForm(fN)->alignment, pChildElement);
		else
			writeFile();
	}
	else
		writeFile();

	formFont(fN, pChildElement);
	writeIOK(fN, pChildElement);
	writeAuxiliary(m_pFormItem->GetForm(fN)->properties, pChildElement);
	alignmentImage(m_pFormItem->GetForm(fN)->alignImage, pChildElement);
	attributes(m_pFormItem->GetForm(fN)->attr, pChildElement);
	formcolor(m_pFormItem->GetForm(fN)->pRGB, m_pFormItem->GetForm(fN)->tRGB, pChildElement);

	taborder(m_pFormItem->GetForm(fN)->index, pChildElement);
	SetpixFont(fN);

	// alpha
	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->alpha);
	MakeElement(pChildElement, _T("ALPHA"), tmp);

	if (!m_pFormItem->GetFormStr(fN, _T("optionimg")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("optionimg")));
		MakeElement(pChildElement, _T("OPTIONIMG"), tmp);
	}
	else
		writeFile();

	if (m_pFormItem->GetForm(fN)->margin)
	{
		tmp = _T("");
		for (int ii = 0; ii < m_pFormItem->GetForm(fN)->margin->GetCount(); ii++)
		{
			tmp += m_pFormItem->GetForm(fN)->margin->GetAt(ii);
			if (ii < m_pFormItem->GetForm(fN)->margin->GetUpperBound())
				tmp += _T(',');
		}
		MakeElement(pChildElement, _T("MARGIN"), tmp);
	}
	else
		writeFile();

	m_writeType = PROPERTLAST;

	tmp.Format(_T("%ld"), RGBtoARGB(m_pFormItem->GetForm(fN)->sfpRGB));
	MakeElement(pChildElement, _T("SFPRGB"), tmp);

	generateControlScript(fN, pChildElement);
	removeIXMLDOMElement(pChildElement);
}

void CWriteXml::srcRadio(int fN, IXMLDOMElement* pElement)
{
	CString		tmp;
	IXMLDOMElement*	pChildElement;

	pChildElement = BaseWrite(_T("FM_RADIO"), fN, pElement);
	writeEtc(fN, pChildElement);

	if (!m_pFormItem->GetFormStr(fN, _T("str")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("str")));
		MakeElement(pChildElement, _T("STR"), tmp);
	}
	else
		writeFile();

	if (!m_pFormItem->GetFormStr(fN, _T("str2")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("str2")));
		MakeElement(pChildElement, _T("STR2"), tmp);
	}
	else
		writeFile();

	if (!m_pFormItem->GetFormStr(fN, _T("desc")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("desc")));
		MakeElement(pChildElement, _T("DESC"), tmp);
	}
	else
		writeFile();

	if (!m_pFormItem->GetFormStr(fN, _T("auxS")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("auxS")));
		MakeElement(pChildElement, _T("AUXS"), tmp);
	}
	else
		writeFile();

	if (!(m_pFormItem->GetForm(fN)->properties & PR_IMAGE))
	{
		if (!(m_pFormItem->GetForm(fN)->properties & PR_IMAGETEXT))
			alignment(m_pFormItem->GetForm(fN)->alignment, pChildElement);
		else
			writeFile();
	}
	else
		writeFile();

	formFont(fN, pChildElement);
	writeIOK(fN, pChildElement);
	writeAuxiliary(m_pFormItem->GetForm(fN)->properties, pChildElement);
	alignmentImage(m_pFormItem->GetForm(fN)->alignImage, pChildElement);
	attributes(m_pFormItem->GetForm(fN)->attr, pChildElement);
	formcolor(m_pFormItem->GetForm(fN)->pRGB, m_pFormItem->GetForm(fN)->tRGB, pChildElement);
	
	taborder(m_pFormItem->GetForm(fN)->index, pChildElement);		
	SetpixFont(fN);

	// alpha
	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->alpha);
	MakeElement(pChildElement, _T("ALPHA"), tmp);

	if (!m_pFormItem->GetFormStr(fN, _T("optionimg")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("optionimg")));
		MakeElement(pChildElement, _T("OPTIONIMG"), tmp);
	}
	else
		writeFile();

	
	if (m_pFormItem->GetForm(fN)->margin)
	{
		tmp = _T("");
		for (int ii = 0; ii < m_pFormItem->GetForm(fN)->margin->GetCount(); ii++)
		{
			tmp += m_pFormItem->GetForm(fN)->margin->GetAt(ii);
			if (ii < m_pFormItem->GetForm(fN)->margin->GetUpperBound())
				tmp += _T(',');
		}
		MakeElement(pChildElement, _T("MARGIN"), tmp);
	}
	else
		writeFile();

	m_writeType = PROPERTLAST;

	tmp.Format(_T("%ld"), RGBtoARGB(m_pFormItem->GetForm(fN)->sfpRGB));
	MakeElement(pChildElement, _T("SFPRGB"), tmp);

	generateControlScript(fN, pChildElement);
	removeIXMLDOMElement(pChildElement);
}

void CWriteXml::srcEdit(int fN, IXMLDOMElement* pElement)
{
	CString		tmp;
	IXMLDOMElement*	pChildElement;
	CString		kind;

	if (m_pFormItem->GetForm(fN)->kind == FM_EDIT)
		pChildElement = BaseWrite(_T("FM_EDIT"), fN, pElement);
	else
		pChildElement = BaseWrite(_T("FM_OUT"), fN, pElement);

	writeEtc(fN, pChildElement);

	tmp.Format("%d", m_pFormItem->GetForm(fN)->size);
	MakeElement(pChildElement, _T("LEN"), tmp);

	dataType(m_pFormItem->GetForm(fN)->type, pChildElement);

	if (m_pFormItem->GetForm(fN)->offs)
	{
		tmp.Format("%d", m_pFormItem->GetForm(fN)->offs);
		MakeElement(pChildElement, _T("FLOAT"), tmp);
	}
	else
		writeFile();

	writeAuxiliary(m_pFormItem->GetForm(fN)->properties, pChildElement);
	
	if (!m_pFormItem->GetFormStr(fN, _T("str")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("str")));
		MakeElement(pChildElement, _T("STR"), tmp);
	}
	else
		writeFile();

	if (m_pFormItem->GetForm(fN)->kind == FM_EDIT)
	{
		if (!m_pFormItem->GetFormStr(fN, _T("str2")).IsEmpty())
		{
			tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("str2")));
			MakeElement(pChildElement, _T("STR2"), tmp);
		}
		else
			writeFile();

		if (!m_pFormItem->GetFormStr(fN, _T("dat")).IsEmpty())
		{
			tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("dat")));
			MakeElement(pChildElement, _T("DAT"), tmp);
		}
		else
			writeFile();

		writeIOK(fN, pChildElement);
		alignmentImage(m_pFormItem->GetForm(fN)->alignImage, pChildElement);
	}
	else
	{
		alignment(m_pFormItem->GetForm(fN)->alignment, pChildElement);
		writeBoders(fN, pChildElement);
		tmp.Format(_T("%ld"), RGBtoARGB(m_pFormItem->GetForm(fN)->bRGB));
		MakeElement(pChildElement, _T("LINECOLOR"), tmp);
	}

	if (!m_pFormItem->GetFormStr(fN, _T("auxS")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("auxS")));
		MakeElement(pChildElement, _T("AUXS"), tmp);
	}
	else
		writeFile();

	if (!m_pFormItem->GetFormStr(fN, _T("editS")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetForm(fN)->editS);
		MakeElement(pChildElement, _T("EDITFORMAT"), tmp, false);

		tmp.Format(_T("%s"), m_pFormItem->GetForm(fN)->editS);
		writeFile(tmp);
	}
	else
		writeFile();

	tmp.Format(_T("%d%c%d"), HIWORD(m_pFormItem->GetForm(fN)->onEdit), DELIMETER, LOWORD(m_pFormItem->GetForm(fN)->onEdit));
	MakeElement(pChildElement, _T("IFRGB"), tmp, false);
	tmp.Format(_T("%d%c%d"), HIWORD(m_pFormItem->GetForm(fN)->onEdit), GB_PROPERT, LOWORD(m_pFormItem->GetForm(fN)->onEdit));
	writeFile(tmp);

	formFont(fN, pChildElement);

	if (m_pFormItem->GetForm(fN)->type == IO_DIGIT)
		m_pFormItem->GetForm(fN)->attr |= FA_FLOATx;

	if (m_pFormItem->GetForm(fN)->attr & FA_FLOATx)
		m_pFormItem->GetForm(fN)->offs = 0;

	attributes(m_pFormItem->GetForm(fN)->attr, pChildElement);
	subattributes(m_pFormItem->GetForm(fN)->attr, m_pFormItem->GetForm(fN)->attr2, pChildElement);

	formcolor(m_pFormItem->GetForm(fN)->pRGB, m_pFormItem->GetForm(fN)->tRGB, pChildElement);
	taborder(m_pFormItem->GetForm(fN)->index, pChildElement);	// Property Grid에서만 제거

	ioorder(fN, pChildElement);

	writeFile();
	writeFile();

	if (m_pFormItem->GetForm(fN)->kind == FM_EDIT)
	{
		writeBoders(fN, pChildElement);
		tmp.Format(_T("%ld"), RGBtoARGB(m_pFormItem->GetForm(fN)->bRGB));
		MakeElement(pChildElement, _T("LINECOLOR"), tmp);
	}

	tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("rts")));
	MakeElement(pChildElement, _T("FORMRTSSYMBOL"), tmp);

	if (m_pFormItem->GetForm(fN)->kind == FM_OUT)
	{
		if (!m_pFormItem->GetFormStr(fN, _T("str2")).IsEmpty())
		{
			tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("str2")));
			MakeElement(pChildElement, _T("STR2"), tmp);
		}
		else
			writeFile();
	}
	SetpixFont(fN);
		
	// alpha
	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->alpha);
	MakeElement(pChildElement, _T("ALPHA"), tmp);
	
	// add edit align text
	if (m_pFormItem->GetForm(fN)->kind == FM_EDIT)
	{
		alignment(m_pFormItem->GetForm(fN)->alignment, pChildElement);

		if (!m_pFormItem->GetFormStr(fN, _T("refs")).IsEmpty())
		{
			tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("refs")));
			MakeElement(pChildElement, _T("REFS"), tmp);
		}
		else
			writeFile();
	}

	
	if (m_pFormItem->GetForm(fN)->margin)
	{
		tmp = _T("");
		for (int ii = 0; ii < m_pFormItem->GetForm(fN)->margin->GetCount(); ii++)
		{
			tmp += m_pFormItem->GetForm(fN)->margin->GetAt(ii);
			if (ii < m_pFormItem->GetForm(fN)->margin->GetUpperBound())
				tmp += _T(',');
		}
		MakeElement(pChildElement, _T("MARGIN"), tmp);
	}
	else
		writeFile();

	tmp.Format(_T("%ld"), m_pFormItem->GetForm(fN)->opRGB);
	MakeElement(pChildElement, _T("OPRGB"), tmp);

	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->blinkShape);
	MakeElement(pChildElement, _T("BLINKKIND"), tmp);

	m_writeType = PROPERTLAST;
	tmp.Format(_T("%ld"), RGBtoARGB(m_pFormItem->GetForm(fN)->sfpRGB));
	MakeElement(pChildElement, _T("SFPRGB"), tmp);

	generateControlScript(fN, pChildElement);
	removeIXMLDOMElement(pChildElement);
}

void CWriteXml::srcCombo(int fN, IXMLDOMElement* pElement)
{
	CString		tmp;
	IXMLDOMElement*	pChildElement;

	pChildElement = BaseWrite(_T("FM_COMBO"), fN, pElement);
	writeEtc(fN, pChildElement);

	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->size);
	MakeElement(pChildElement, _T("LEN"), tmp);
	tmp.Format(_T("%d%c%d"), LOWORD(m_pFormItem->GetForm(fN)->keys), DELIMETER, HIWORD(m_pFormItem->GetForm(fN)->keys));
	MakeElement(pChildElement, _T("COUNT"), tmp, false);
	tmp.Format(_T("%d%c%d"), LOWORD(m_pFormItem->GetForm(fN)->keys), GB_PROPERT, HIWORD(m_pFormItem->GetForm(fN)->keys));
	writeFile(tmp);

	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->checked);
	MakeElement(pChildElement, _T("INDEX"), tmp);

	tmp.Format(_T("%s"), keyMap[m_pFormItem->GetForm(fN)->onEdit]);
	MakeElement(pChildElement, _T("MATCHKEY"), tmp, false);
	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->onEdit);		// ljh
	writeFile(tmp);

	dataType(m_pFormItem->GetForm(fN)->type, pChildElement);

	if (LOWORD(m_pFormItem->GetForm(fN)->keys) != 0)
	{
		CString data, tmps;
		int	count = LOWORD(m_pFormItem->GetForm(fN)->keys);
		data = m_pFormItem->GetFormStr(fN, _T("dat"));
		tmp = _T("");
		for (int ii = 0; ii < count; ii++)
		{
			tmps.Format(_T("%s"), Parser(data, _T("\t")));
			if (count - 1)
				tmps += _T(",");

			tmp += tmps;
		}
		MakeElement(pChildElement, _T("DAT"), tmp);
	}
	else
		writeFile();

	if (LOWORD(m_pFormItem->GetForm(fN)->keys) != 0)
	{
		CString data, tmps;
		int	count = LOWORD(m_pFormItem->GetForm(fN)->keys);

		data = m_pFormItem->GetFormStr(fN, _T("str"));
		tmp = _T("");
		for (int ii = 0; ii < count; ii++)
		{
			tmps.Format(_T("%s"), Parser(data, _T("\t")));
			if (count - 1)
				tmps += _T(",");

			tmp += tmps;
		}
		MakeElement(pChildElement, _T("STR"), tmp);
	}
	else
		writeFile();

	if (!m_pFormItem->GetFormStr(fN, _T("str2")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("str2")));
		MakeElement(pChildElement, _T(_T("STR2")), tmp);
	}
	else
		writeFile();

	if (!m_pFormItem->GetFormStr(fN, _T("editS")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetForm(fN)->editS);
		MakeElement(pChildElement, _T("EDITFORMAT"), tmp);
	}
	else
		writeFile();

	writeIOK(fN, pChildElement);
	formFont(fN, pChildElement);
	writeAuxiliary(m_pFormItem->GetForm(fN)->properties, pChildElement);
	attributes(m_pFormItem->GetForm(fN)->attr, pChildElement);
	subattributes(m_pFormItem->GetForm(fN)->attr, m_pFormItem->GetForm(fN)->attr2, pChildElement);
	formcolor(m_pFormItem->GetForm(fN)->pRGB, m_pFormItem->GetForm(fN)->tRGB, pChildElement);
	taborder(m_pFormItem->GetForm(fN)->index, pChildElement);

	ioorder(fN, pChildElement);
	writeFile();
	writeFile();

	tmp.Format(_T("%ld"), RGBtoARGB(m_pFormItem->GetForm(fN)->spRGB));
	MakeElement(pChildElement, _T("SPRGB"), tmp);
	SetpixFont(fN);

	// alpha
	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->alpha);
	MakeElement(pChildElement, _T("ALPHA"), tmp);

	// text alignment
	alignment(m_pFormItem->GetForm(fN)->alignment, pChildElement);

	// dropdown button image
	if (!m_pFormItem->GetFormStr(fN, _T("optionimg")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("optionimg")));
		MakeElement(pChildElement, _T("OPTIONIMG"), tmp);
	}
	else
		writeFile();

	// dropdownlist bg image
	if (!m_pFormItem->GetFormStr(fN, _T("optionimg2")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("optionimg2")));
		MakeElement(pChildElement, _T("OPTIONIMG2"), tmp);
	}
	else
		writeFile();

	// dropdownlist select image
	if (!m_pFormItem->GetFormStr(fN, _T("optionimg3")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("optionimg3")));
		MakeElement(pChildElement, _T("OPTIONIMG3"), tmp);
	}
	else
		writeFile();

	// dropdownlist line color
	tmp.Format(_T("%ld"), m_pFormItem->GetForm(fN)->stRGB);
	MakeElement(pChildElement, _T("STRGB"), tmp);

	// dropdownlist text color
	tmp.Format(_T("%ld"), m_pFormItem->GetForm(fN)->opRGB);
	MakeElement(pChildElement, _T("OPRGB"), tmp);

	if (m_pFormItem->GetForm(fN)->margin)
	{
		tmp = _T("");
		for (int ii = 0; ii < m_pFormItem->GetForm(fN)->margin->GetCount(); ii++)
		{
			tmp += m_pFormItem->GetForm(fN)->margin->GetAt(ii);
			if (ii < m_pFormItem->GetForm(fN)->margin->GetUpperBound())
				tmp += _T(',');
		}
		MakeElement(pChildElement, _T("MARGIN"), tmp);
	}
	else
		writeFile();

	m_writeType = PROPERTLAST;

	tmp.Format(_T("%ld"), RGBtoARGB(m_pFormItem->GetForm(fN)->sfpRGB));
	MakeElement(pChildElement, _T("SFPRGB"), tmp);

	generateControlScript(fN, pChildElement);
	removeIXMLDOMElement(pChildElement);
}

void CWriteXml::srcGrid(int kind, int fN, IXMLDOMElement* pElement)
{
	CString		tmp;
	IXMLDOMElement*	pChildElement;

	if (kind == FM_GRID)
		pChildElement = BaseWrite(_T("FM_GRID"), fN, pElement);
	else if (kind == FM_GRIDEX)
		pChildElement = BaseWrite(_T("FM_GRIDEX"), fN, pElement);
	writeEtc(fN, pChildElement);

	struct	_formR	*formR = m_pFormItem->GetForm(fN);
	struct	_repR	*repR = (struct _repR*)formR->auxR;

	tmp.Format(_T("%d"), HIWORD(formR->keys));
	MakeElement(pChildElement, _T("VISIBLEROW"), tmp);
	tmp.Format(_T("%d"), LOWORD(formR->keys));
	MakeElement(pChildElement, _T("CELLH"), tmp);
	tmp.Format(_T("%d%c%d"), repR->rows, DELIMETER, repR->cols);
	MakeElement(pChildElement, _T("MATRIX"), tmp, false);
	tmp.Format(_T("%d%c%d"), repR->rows, GB_PROPERT, repR->cols);
	writeFile(tmp);
	tmp.Format(_T("%ld%c%ld"), RGBtoARGB(formR->pRGB), DELIMETER, RGBtoARGB(formR->tRGB));
	MakeElement(pChildElement, _T("MAINCOLOR"), tmp, false);
	tmp.Format(_T("%ld%c%ld"), RGBtoARGB(formR->pRGB), GB_PROPERT, RGBtoARGB(formR->tRGB));
	writeFile(tmp);

	tmp.Format(_T("%ld"), RGBtoARGB(formR->bRGB));
	MakeElement(pChildElement, _T("LINECOLOR"), tmp);
	tmp.Format(_T("%ld"), formR->onEdit);
	MakeElement(pChildElement, _T("FOCUSCOLOR"), tmp);
	//tmp.Format(_T("%d"), formR->isEdit);
	tmp.Format(_T("%d"), formR->selectShape);
	MakeElement(pChildElement, _T("FOCUSKIND"), tmp);
	//tmp.Format(_T("%d"), formR->isEdit);

	formFont(fN, pChildElement);

	if (!m_pFormItem->GetFormStr(fN, _T("str")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("str")));
		MakeElement(pChildElement, _T("STR"), tmp);
	}
	else
		writeFile();

	writeAuxiliary(formR->properties, pChildElement);

	writeGridOption(fN, pChildElement);
	taborder(formR->index, pChildElement);

	ioorder(fN, pChildElement);
	writeFile();
	writeFile();

	tmp.Format(_T("%d"), formR->fixrow);
	MakeElement(pChildElement, _T("FIXEDROW"), tmp);
	tmp.Format(_T("%d"), formR->fixcol);
	MakeElement(pChildElement, _T("FIXEDCOL"), tmp);
	tmp.Format(_T("%d"), formR->headH);
	MakeElement(pChildElement, _T("HEADHEIGHT"), tmp);

	tmp.Format(_T("%d"), formR->rowof);
	MakeElement(pChildElement, _T("ROWOF"), tmp);
	tmp.Format(_T("%ld"), RGBtoARGB(formR->spRGB));
	MakeElement(pChildElement, _T("HPRGB"), tmp);
	tmp.Format(_T("%ld"), RGBtoARGB(formR->stRGB));
	MakeElement(pChildElement, _T("HTRGB"), tmp);
	SetpixFont(fN);
		
	// alpha
	tmp.Format(_T("%d"), formR->alpha);
	MakeElement(pChildElement, _T("ALPHA"), tmp);

	// BG Image
	if (!m_pFormItem->GetFormStr(fN, _T("str2")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("str2")));
		MakeElement(pChildElement, _T("STR2"), tmp);
	}
	else
		writeFile();

	// Select Shape
	tmp.Format(_T("%d"),formR->selectShape);
	MakeElement(pChildElement, _T("SHAPE"), tmp);

	// Edit Option
	tmp.Format(_T("%d"), formR->optionPos);
	MakeElement(pChildElement, _T("OPTIONPOS"), tmp);

	// Edit Option Image
	if (!m_pFormItem->GetFormStr(fN, _T("optionimg")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("optionimg")));
		MakeElement(pChildElement, _T("OPTIONIMG"), tmp);
	}
	else
		writeFile();

	// blink color
	tmp.Format(_T("%ld"), m_pFormItem->GetForm(fN)->opRGB);
	MakeElement(pChildElement, _T("OPRGB"), tmp);

	if (formR->margin)
	{
		tmp = _T("");
		for (int ii = 0; ii < formR->margin->GetCount(); ii++)
		{
			tmp += formR->margin->GetAt(ii);
			if (ii < formR->margin->GetUpperBound())
				tmp += _T(',');
		}
		MakeElement(pChildElement, _T("MARGIN"), tmp);
	}
	else
		writeFile();


	m_writeType = PROPERTLAST;

	tmp.Format(_T("%d"), formR->blinkShape);
	MakeElement(pChildElement, _T("BLINKKIND"), tmp);

	writeColumns(kind, repR, pChildElement);
	generateControlScript(fN, pChildElement);
	removeIXMLDOMElement(pChildElement);
}

void CWriteXml::srcTable(int fN, IXMLDOMElement* pElement)
{
	CString		tmp;
	IXMLDOMElement*	pChildElement;
	struct	_repR*	repR;

	pChildElement = BaseWrite(_T("FM_TABLE"), fN, pElement);
	writeEtc(fN, pChildElement);

	repR = (struct _repR*)m_pFormItem->GetForm(fN)->auxR;
	repR->cols = 1;
	
	tmp.Format(_T("%d%c%d"), repR->rows, DELIMETER, repR->cols);
	MakeElement(pChildElement, _T("MATRIX"), tmp, false);
	tmp.Format(_T("%d%c%d"), repR->rows, GB_PROPERT, repR->cols);
	writeFile(tmp);
	tmp.Format(_T("%ld"), RGBtoARGB(m_pFormItem->GetForm(fN)->bRGB));
	MakeElement(pChildElement, _T("LINECOLOR"), tmp);
	tmp.Format(_T("%ld"), m_pFormItem->GetForm(fN)->keys);
	MakeElement(pChildElement, _T("CELLH"), tmp);

	formFont(fN, pChildElement);
	writeAuxiliary(m_pFormItem->GetForm(fN)->properties, pChildElement);
	writeGridOption(fN, pChildElement);
	writeIOK(fN, pChildElement);
	ioorder(fN, pChildElement);

	writeFile();
	writeFile();
	SetpixFont(fN);

	// alpha
	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->alpha);
	MakeElement(pChildElement, _T("ALPHA"), tmp);

	if (m_pFormItem->GetForm(fN)->margin)
	{
		tmp = _T("");
		for (int ii = 0; ii < m_pFormItem->GetForm(fN)->margin->GetCount(); ii++)
		{
			tmp += m_pFormItem->GetForm(fN)->margin->GetAt(ii);
			if (ii < m_pFormItem->GetForm(fN)->margin->GetUpperBound())
				tmp += _T(',');
		}
		MakeElement(pChildElement, _T("MARGIN"), tmp);
	}
	else
		writeFile();

	tmp.Format(_T("%ld"), m_pFormItem->GetForm(fN)->opRGB);
	MakeElement(pChildElement, _T("OPRGB"), tmp);

	m_writeType = PROPERTLAST;
	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->blinkShape);
	MakeElement(pChildElement, _T("BLINKKIND"), tmp);

	writeRows(repR, pChildElement);
	generateControlScript(fN, pChildElement);
	removeIXMLDOMElement(pChildElement);
}

void CWriteXml::srcObject(int fN, IXMLDOMElement* pElement)
{
	CString		tmp;
	IXMLDOMElement*	pChildElement;
	
	pChildElement = BaseWrite(_T("FM_OBJECT"), fN, pElement);
	writeEtc(fN, pChildElement);

	if (!m_pFormItem->GetFormStr(fN, _T("str")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("str")));
		MakeElement(pChildElement, _T("STR"), tmp);
	}
	else
		writeFile();

	writeAuxiliary(m_pFormItem->GetForm(fN)->properties, pChildElement);
	attributes(m_pFormItem->GetForm(fN)->attr, pChildElement);

	if (m_pFormItem->GetForm(fN)->attr2 & GO_FIX)
	{
		tmp.Format(_T("%ld"), m_pFormItem->GetForm(fN)->attr2);
		MakeElement(pChildElement, _T("RESIZABLE"), tmp);
	}
	else
		writeFile();

	taborder(m_pFormItem->GetForm(fN)->index, pChildElement);
	ioorder(fN, pChildElement);
	writeFile();
	m_writeType = PROPERTLAST;
	writeFile();

	generateControlScript(fN, pChildElement);

	removeIXMLDOMElement(pChildElement);
}

void CWriteXml::srcTab(int fN, IXMLDOMElement* pElement)
{
	CString		tmp;
	IXMLDOMElement*	pChildElement;
	
	pChildElement = BaseWrite(_T("FM_TAB"), fN, pElement);
	writeEtc(fN, pChildElement);

	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->checked);
	MakeElement(pChildElement, _T("INDEX"), tmp);
	tmp.Format(_T("%d%c%d"), LOWORD(m_pFormItem->GetForm(fN)->keys), DELIMETER, HIWORD(m_pFormItem->GetForm(fN)->keys));
	MakeElement(pChildElement, _T("COUNT"), tmp, false);
	tmp.Format(_T("%d%c%d"), LOWORD(m_pFormItem->GetForm(fN)->keys), GB_PROPERT, HIWORD(m_pFormItem->GetForm(fN)->keys));
	writeFile(tmp);

	attributes(m_pFormItem->GetForm(fN)->attr, pChildElement);
	formFont(fN, pChildElement);
	formcolor(m_pFormItem->GetForm(fN)->pRGB, m_pFormItem->GetForm(fN)->tRGB, pChildElement);

	writeAuxiliary(m_pFormItem->GetForm(fN)->properties, pChildElement);
	alignmentImage(m_pFormItem->GetForm(fN)->alignImage, pChildElement);

	if (!m_pFormItem->GetFormStr(fN, _T("str2")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("str2")));
		MakeElement(pChildElement, _T("STR2"), tmp);
	}
	else
		writeFile();

//	TRACE ("spRGB : %d\nstRGB : %d\n", m_pFormItem->GetForm(fN)->spRGB, m_pFormItem->GetForm(fN)->stRGB);
	if (m_pFormItem->GetForm(fN)->spRGB == 0 && m_pFormItem->GetForm(fN)->stRGB == 0)
	{
		m_pFormItem->GetForm(fN)->spRGB = m_pFormItem->GetForm(fN)->pRGB;
		m_pFormItem->GetForm(fN)->stRGB = m_pFormItem->GetForm(fN)->tRGB;
	}

	//tmp.Format(_T("%ld"), RGBtoARGB(m_pFormItem->GetForm(fN)->spRGB));
	//MakeElement(pChildElement, _T("SPRGB"), tmp);
	//tmp.Format(_T("%ld"), RGBtoARGB(m_pFormItem->GetForm(fN)->stRGB));
	//MakeElement(pChildElement, _T("STRGB"), tmp);
	formselcolor(m_pFormItem->GetForm(fN)->spRGB, m_pFormItem->GetForm(fN)->stRGB, pChildElement);
	

	if (LOWORD(m_pFormItem->GetForm(fN)->keys) > 0)
	{
		CString tmps, data;
		int ii, count = LOWORD(m_pFormItem->GetForm(fN)->keys);

		tmp = _T("");
		data  = m_pFormItem->GetFormStr(fN, _T("dat"));
		for (ii = 0; ii < count; ii++)
		{
			tmps.Format(_T("%s"), Parser(data, _T("\t")));
			if (ii < (int)m_pFormItem->GetForm(fN)->keys - 1)
				tmps += _T(",");
			tmp += tmps;
		}
		MakeElement(pChildElement, _T("DAT"), tmp);
	}
	else
		writeFile();

	if (LOWORD(m_pFormItem->GetForm(fN)->keys) != 0)
	{
		CString tmps, data;
		int ii, count = LOWORD(m_pFormItem->GetForm(fN)->keys);

		tmp = _T("");
		if (!m_pFormItem->GetFormStr(fN, _T("str")).IsEmpty())
		{
			data = m_pFormItem->GetFormStr(fN, _T("str"));
			for (ii = 0; ii < count; ii++)
			{
				tmps.Format(_T("%s"), Parser(data, _T("\t")));
				if (ii < (int)m_pFormItem->GetForm(fN)->keys - 1)
					tmps += _T(",");
				tmp += tmps;
			}
		}
		MakeElement(pChildElement, _T("STR"), tmp);
	}
	else
		writeFile();

	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->type);
	MakeElement(pChildElement, _T("DATATYPE"), tmp);

	tmp = _T("");
	if (!m_pFormItem->GetFormStr(fN, _T("desc")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("desc")));
		MakeElement(pChildElement, _T("DESC"), tmp);
	}
	else
		writeFile();

	SetpixFont(fN);

	// alpha
	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->alpha);
	MakeElement(pChildElement, _T("ALPHA"), tmp);

	// Option Image Path
	if (!m_pFormItem->GetFormStr(fN, _T("optionimg")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("optionimg")));
		MakeElement(pChildElement, _T("OPTIONIMG"), tmp);
	}
	else
		writeFile();
	
	if (m_pFormItem->GetForm(fN)->margin)
	{
		tmp = _T("");
		for (int ii = 0; ii < m_pFormItem->GetForm(fN)->margin->GetCount(); ii++)
		{
			tmp += m_pFormItem->GetForm(fN)->margin->GetAt(ii);
			if (ii < m_pFormItem->GetForm(fN)->margin->GetUpperBound())
				tmp += _T(',');
		}
		MakeElement(pChildElement, _T("MARGIN"), tmp);
	}
	else
		writeFile();

	m_writeType = PROPERTLAST;
	if (!m_pFormItem->GetFormStr(fN, _T("optionimg2")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("optionimg2")));
		MakeElement(pChildElement, _T("OPTIONIMG2"), tmp);
	}
	else
		writeFile();

	generateControlScript(fN, pChildElement);
	removeIXMLDOMElement(pChildElement);
}

void CWriteXml::srcBrowser(int fN, IXMLDOMElement* pElement)
{
	CString		tmp;
	IXMLDOMElement*	pChildElement;

	pChildElement = BaseWrite(_T("FM_BROWSER"), fN, pElement);
	writeEtc(fN, pChildElement);

	writeIOK(fN, pChildElement);

	if (!m_pFormItem->GetFormStr(fN, "str").IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("str")));
		MakeElement(pChildElement, _T("STR"), tmp);
	}
	else
		writeFile();

	writeAuxiliary(m_pFormItem->GetForm(fN)->properties, pChildElement);
	attributes(m_pFormItem->GetForm(fN)->attr, pChildElement);

	ioorder(fN, pChildElement);
	writeFile();	
	writeFile();

	m_writeType = PROPERTLAST;
	// alpha
	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->alpha);
	MakeElement(pChildElement, _T("ALPHA"), tmp);

	generateControlScript(fN, pChildElement);

	removeIXMLDOMElement(pChildElement);
}

void CWriteXml::srcControl(int fN, IXMLDOMElement* pElement)
{
	CString		tmp, dat;
	IXMLDOMElement*	pChildElement;

	pChildElement = BaseWrite(_T("FM_CONTROL"), fN, pElement);
	writeEtc(fN, pChildElement);

	writeAuxiliary(m_pFormItem->GetForm(fN)->properties, pChildElement);
	attributes(m_pFormItem->GetForm(fN)->attr, pChildElement);
	if (m_pFormItem->GetForm(fN)->attr2 & GO_FIX)
	{
		tmp.Format(_T("%ld"), m_pFormItem->GetForm(fN)->attr2);
		MakeElement(pChildElement, _T("RESIZABLE"), tmp);
	}
	else
		writeFile();

	if (!m_pFormItem->GetFormStr(fN, _T("str")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("str")));
		if (m_control && m_control->Lookup(tmp, dat))
		{
			MakeElement(pChildElement, _T("STR"), tmp, false);
			writeFile(dat);
		}
		else
			writeFile();
	}
	else
		writeFile();

	if (!m_pFormItem->GetFormStr(fN, _T("str2")).IsEmpty())
	{
		tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("str2")));
		MakeElement(pChildElement, _T("STR2"), tmp);
	}
	else
		writeFile();

	writeIOK(fN, pChildElement);
	formFont(fN, pChildElement);
	formcolor(m_pFormItem->GetForm(fN)->pRGB, m_pFormItem->GetForm(fN)->tRGB, pChildElement);
	taborder(m_pFormItem->GetForm(fN)->index, pChildElement);;

	ioorder(fN, pChildElement);
	writeFile();
	writeFile();

	SetpixFont(fN);

	tmp.Format(_T("%d%c%d"), HIWORD(m_pFormItem->GetForm(fN)->onEdit), DELIMETER, LOWORD(m_pFormItem->GetForm(fN)->onEdit));
	MakeElement(pChildElement, _T("IFRGB"), tmp, false);
	tmp.Format(_T("%d%c%d"), HIWORD(m_pFormItem->GetForm(fN)->onEdit), GB_PROPERT, LOWORD(m_pFormItem->GetForm(fN)->onEdit));
	writeFile(tmp);
		
	tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("rts")));
	MakeElement(pChildElement, _T("FORMRTSSYMBOL"), tmp);
		
	tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("dat")));
	MakeElement(pChildElement, _T("DAT"), tmp, false);

	m_writeType = PROPERTLAST;
	// alpha
	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->alpha);
	MakeElement(pChildElement, _T("ALPHA"), tmp);

	generateControlScript(fN, pChildElement);

	removeIXMLDOMElement(pChildElement);
}

void CWriteXml::writeEtc(int fN, IXMLDOMElement* pElement)
{
	CString tmp = _T("");
	struct _formR *pFormR = m_pFormItem->GetForm(fN);

	if (	pFormR->kind == FM_EDIT		||
		pFormR->kind == FM_OUT		||
		pFormR->kind == FM_IMAGEVIEW    ||
		pFormR->kind == FM_LABEL	|| 
		pFormR->kind == FM_BUTTON	||
		pFormR->kind == FM_TAB		||
		pFormR->kind == FM_CHECK	||
		pFormR->kind == FM_RADIO	||
		pFormR->kind == FM_TABLE	||
		pFormR->kind == FM_CONTROL	||
		pFormR->kind == FM_GRID		||
		pFormR->kind == FM_GRIDEX)
	{
		tmp.Format(_T("%d"), pFormR->onTap);
		MakeElement(pElement, _T("onclick"), tmp, false);
		tmp.Format(_T("%d"), pFormR->onTapL);
		MakeElement(pElement, _T("onclickl"), tmp, false);
	}

	tmp = _T("");
	if (	pFormR->kind == FM_LABEL	||
		pFormR->kind == FM_OUT		||
		pFormR->kind == FM_IMAGEVIEW	|| 
		pFormR->kind == FM_BUTTON	||
		pFormR->kind == FM_TAB		||
		pFormR->kind == FM_CHECK	||
		pFormR->kind == FM_RADIO	||
		pFormR->kind == FM_TABLE	||
		pFormR->kind == FM_CONTROL	||
		pFormR->kind == FM_GRID		||
		pFormR->kind == FM_GRIDEX)
	{
		tmp.Format(_T("%d"), pFormR->onLongTap);
		MakeElement(pElement, _T("ONLONGTAB"), tmp, false);
		tmp.Format(_T("%d"), pFormR->onLongTapL);
		MakeElement(pElement, _T("ONLONGTABL"), tmp, false);
	}

	tmp = _T("");
	if (	pFormR->kind == FM_EDIT    ||
		pFormR->kind == FM_GRID    ||
		pFormR->kind == FM_GRIDEX  ||
		pFormR->kind == FM_CONTROL ||
		pFormR->kind == FM_COMBO)
	{
		tmp.Format(_T("%d"), pFormR->onChange);
		MakeElement(pElement, _T("ONCHANGE"), tmp, false);
		tmp.Format(_T("%d"), pFormR->onChangeL);
		MakeElement(pElement, _T("ONCHANGEL"), tmp, false);
	}

	tmp = _T("");
	if (	pFormR->kind == FM_EDIT || 
		pFormR->kind == FM_GRID ||
		pFormR->kind == FM_GRIDEX)	
	{
		tmp.Format(_T("%d"), pFormR->onChar);
		MakeElement(pElement, _T("ONCHAR"), tmp, false);
		tmp.Format(_T("%d"), pFormR->onCharL);
		MakeElement(pElement, _T("ONCHARL"), tmp, false);
	}
	

	if (pFormR->kind == FM_GRID || pFormR->kind == FM_GRIDEX)	
	{
		tmp.Format(_T("%d"), pFormR->onDrag);
		MakeElement(pElement, _T("ONDRAG"), tmp, false);
		tmp.Format(_T("%d"), pFormR->onDragL);
		MakeElement(pElement, _T("ONDRAGL"), tmp, false);
	}

	if (pFormR->kind == FM_GRID || pFormR->kind == FM_GRIDEX)	
	{
		tmp.Format(_T("%d"), pFormR->onScroll);
		MakeElement(pElement, _T("ONSCROLL"), tmp, false);
		tmp.Format(_T("%d"), pFormR->onScrollL);
		MakeElement(pElement, _T("ONSCROLLL"), tmp, false);
	}

	tmp.Format(_T("%d"), pFormR->block ? 1 : 0);
	MakeElement(pElement, _T("BLOCK"), tmp, false);

	tmp.Format(_T("%d"), pFormR->drawn ? 1 : 0);
	MakeElement(pElement, _T("DRAWN"), tmp, false);

	tmp.Format(_T("%d"), pFormR->bCommon ? 1 : 0);
	MakeElement(pElement, _T("BCOMMON"), tmp, false);

	tmp.Format(_T("%d"), pFormR->scrollPos);
	MakeElement(pElement, _T("SCROLLPOS"), tmp, false);

	groupN(fN, pElement);
}

void CWriteXml::symbolRect(int fN, IXMLDOMElement* pElement)
{
	CString tmp = _T("");

	tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("name")));
	MakeElement(pElement, _T("SYMBOLNAME"), tmp);
	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->left);
	MakeElement(pElement, _T("LEFT"), tmp);
	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->top);
	MakeElement(pElement, _T("TOP"), tmp);
	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->right);
	MakeElement(pElement, _T("RIGHT"), tmp);
	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->bottom);
	MakeElement(pElement, _T("BOTTOM"), tmp);
}

void CWriteXml::formFont(int fN, IXMLDOMElement* pElement)
{
	CString tmp = _T("");

	tmp.Format(_T("%s"), m_pFormItem->GetFormStr(fN, _T("fName")));
	if (!tmp.IsEmpty())
	{
		MakeElement(pElement, _T("FONTNAME"), tmp);
		tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->fPoint);
		MakeElement(pElement, _T("FONTPOINT"), tmp);
		tmp.Format(_T("%s"), fontStyle[m_pFormItem->GetForm(fN)->fStyle]);
		MakeElement(pElement, _T("FONTSTYLE"), tmp, false);
		tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->fStyle);
		writeFile(tmp);
	}
	else
	{
		if (m_mapH->fName[0] != NULL)
		{
			tmp.Format(_T("%s"), m_mapH->fName);
			MakeElement(pElement, _T("FONTNAME"), tmp);
			tmp.Format(_T("%d"), m_mapH->fPoint);
			MakeElement(pElement, _T("FONTPOINT"), tmp);
			tmp.Format(_T("%s"), fontStyle[m_mapH->fStyle]);
			MakeElement(pElement, _T("FONTSTYLE"), tmp, false);
			tmp.Format(_T("%d"), m_mapH->fStyle);
			writeFile(tmp);
		}
	}
}

void CWriteXml::cellFont(struct _cellR *cellR, IXMLDOMElement* pElement)
{
	CString tmp = _T("");

	tmp.Format(_T("%s"), cellR->fName);
	if (!tmp.IsEmpty())
	{
		MakeElement(pElement, _T("FONTNAME"), tmp);
		tmp.Format(_T("%d"), cellR->fPoint);
		MakeElement(pElement, _T("FONTPOINT"), tmp, false);
		SetCellpixFont(cellR);
		tmp.Format(_T("%s"), fontStyle[cellR->fStyle]);
		MakeElement(pElement, _T("FONTSTYLE"), tmp, false);
		tmp.Format(_T("%d"), cellR->fStyle);
		writeFile(tmp);
	}
	else
	{
		tmp.Format(_T("%s"), _T("굴림"));
		MakeElement(pElement, _T("FONTNAME"), tmp);
		tmp.Format(_T("9"));
		MakeElement(pElement, _T("FONTPOINT"), tmp);
		tmp.Format(_T("NORMAL"));
		MakeElement(pElement, _T("FONTSTYLE"), tmp, false);
		tmp.Format(_T("0"));
		writeFile(tmp);
	}
}


int CWriteXml::FontHeight(char* fontN, int point, bool italic, int bold)
{
	CString	keys;
	CFont*	font;
	LOGFONT	logfont;
	int	fontH = point;

	font = new CFont();

	logfont.lfHeight         = point * 10 + (point < 9 ? 5 : 0);
	logfont.lfWidth          = 0;
	logfont.lfEscapement     = 0;
	logfont.lfOrientation    = 0;
	logfont.lfWeight         = bold;
	logfont.lfItalic         = italic;
	logfont.lfUnderline      = false;
	logfont.lfStrikeOut      = false;
	logfont.lfCharSet        = DEFAULT_CHARSET;
	logfont.lfOutPrecision   = OUT_DEFAULT_PRECIS;
	logfont.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality        = DEFAULT_QUALITY;
	logfont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy_s(logfont.lfFaceName, LF_FACESIZE, fontN);

	font->CreatePointFontIndirect(&logfont);

	TEXTMETRIC tm;
	HDC hdc = GetDC(NULL);
	HFONT	hFontOld = (HFONT)SelectObject(hdc, font->GetSafeHandle());
	GetTextMetrics(hdc, &tm);
	fontH = tm.tmHeight + tm.tmExternalLeading;
	SelectObject(hdc, hFontOld);
	ReleaseDC(NULL, hdc);

	font->DeleteObject();
	delete font;

	return fontH;
}

void CWriteXml::alignment(BYTE align, IXMLDOMElement* pElement)
{
	CString tmp = _T("");

	switch (align)
	{
	case AL_LEFT:
		tmp = _T("AL_LEFT");	break;
	case AL_RIGHT:
		tmp = _T("AL_RIGHT");	break;
	case AL_TOP:
		tmp = _T("AL_TOP");	break;
	case AL_BOTTOM:
		tmp = _T("AL_BOTTOM");	break;
	case AL_CENTER:
	default:
		tmp = _T("AL_CENTER");	break;
	}

	MakeElement(pElement, _T("ALIGNMENT"), tmp, false);
	tmp.Format(_T("%d"), align);
	writeFile(tmp);
}

void CWriteXml::alignmentImage(BYTE align, IXMLDOMElement* pElement)
{
	CString tmp = _T("");

	switch (align)
	{
	case IA_STRETCH:
		tmp = _T("IA_STRETCH");break;
	case IA_TILE:
		tmp = _T("IA_TILE");	break;
	case IA_CENTER:
	default:
		tmp = _T("IA_CENTER");	break;
	}
	MakeElement(pElement, _T("ALIGNMENTIMAGE"), _T("IA_STRETCH"), false);
	tmp.Format(_T("%d"), IA_STRETCH);
	writeFile(tmp);
}

void CWriteXml::attributes(DWORD attr, IXMLDOMElement* pElement)
{
	CString tmp = _T("");

	if (attr & FA_COMMA)
	{
		tmp += _T("FA_COMMA");
		attr |= FA_COMMA;
		tmp += _T(",");
	}
	if (attr & FA_PASSWD)
	{
		tmp += _T("FA_PASSWD");
		attr |= FA_PASSWD;
		tmp += _T(",");
	}
	if (attr & FA_ZEROSUP)
	{
		tmp += _T("FA_ZEROSUP");
		attr |= FA_ZEROSUP;
		tmp += _T(",");
	}
	if (attr & FA_PROTECT)
	{
		tmp += _T("FA_PROTECT");
		attr |= FA_PROTECT;
		tmp += _T(",");
	}
	if (attr & FA_WRAP)
	{
		tmp += _T("FA_WRAP");
		attr |= FA_WRAP;
		tmp += _T(",");
	}
	if (attr & FA_SKIP)
	{
		tmp += _T("FA_SKIP");
		attr |= FA_SKIP;
		tmp += _T(",");
	}
	if (attr & FA_CORGB)
	{
		tmp += _T("FA_CORGB");
		attr |= FA_CORGB;
		tmp += _T(",");
	}
	if (attr & FA_DOMINO)
	{
		tmp += _T("FA_DOMINO");
		attr |= FA_DOMINO;
		tmp += _T(",");
	}
	if (attr & FA_FLOAT)
	{
		tmp += _T("FA_FLOAT");
		attr |= FA_FLOAT;
		tmp += _T(",");
	}
	if (attr & FA_FLOATx)
	{
		tmp += _T("FA_FLOATx");
		attr |= FA_FLOATx;
		tmp += _T(",");
	}
	if (attr & FA_UPPER)
	{
		tmp += _T("FA_UPPER");
		attr |= FA_UPPER;
		tmp += _T(",");
	}
	if (attr & FA_NOR)
	{
		tmp += _T("FA_NOR");
		attr |= FA_NOR;
		tmp += _T(",");
	}
	if (attr & FA_PASS)
	{
		tmp += _T("FA_PASS");
		attr |= FA_PASS;
		tmp += _T(",");
	}

	MakeElement(pElement, _T("ATTRIBUTE"), tmp, false);

	tmp.Format(_T("%d"), attr);
	writeFile(tmp);
}

void CWriteXml::formcolor(DWORD pRGB, DWORD tRGB, IXMLDOMElement* pElement)
{
	CString	tmp;
	
	tmp.Format(_T("%ld"), pRGB);
	MakeElement(pElement, _T("PRGB"), tmp, false);	
	tmp.Format(_T("%ld"), tRGB);
	MakeElement(pElement, _T("TRGB"), tmp, false);

	tmp.Format(_T("%ld%c%ld"), RGBtoARGB(pRGB), GB_PROPERT,  RGBtoARGB(tRGB));
	writeFile(tmp);
}

// selected color
void CWriteXml::formselcolor(DWORD spRGB, DWORD stRGB, IXMLDOMElement* pElement)
{	
	CString	tmp;
	
	tmp.Format(_T("%ld"), spRGB);
	MakeElement(pElement, _T("SPRGB"), tmp, false);	
	tmp.Format(_T("%ld"), stRGB);
	MakeElement(pElement, _T("STRGB"), tmp, false);

	tmp.Format(_T("%ld%c%ld"), RGBtoARGB(spRGB), GB_PROPERT,  RGBtoARGB(stRGB));
	writeFile(tmp);
}


DWORD CWriteXml::RGBtoARGB(DWORD color)
{
	DWORD	value;
	value = color;
	
	if (value & 0x02000000)
	{
		int r = (color >> 16) & 0xFF;
		int g = (color >> 8) & 0xFF;
		int b = (color >> 0) & 0xFF;

		return _ARGB(255, r, g, b);
	}
	return value;
}

void CWriteXml::taborder(WORD idx, IXMLDOMElement* pElement)
{
	CString	tmp;

	tmp.Format(_T("%d"), idx);
	MakeElement(pElement, _T("TABORDER"), tmp);
}

void CWriteXml::ioorder(int fN, IXMLDOMElement* pElement)
{
	CString	tmp;
	
	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->iorder);
	MakeElement(pElement, _T("IOORDER"), tmp);
}

void CWriteXml::dataType(BYTE type, IXMLDOMElement* pElement)
{
	CString tmp;

	switch (type)
	{
	case IO_NUM:
		tmp = _T("IO_NUM");	break;
	case IO_ALPHA:
		tmp = _T("IO_ALPHA");	break;
	case IO_HAN:
		tmp = _T("IO_HAN");	break;
	case IO_MIXED:
		tmp = _T("IO_MIXED");	break;
	case IO_DIGIT:
		tmp = _T("IO_DIGIT");	break;
	case IO_ANY:
		tmp = _T("IO_ANY");	break;
	default:
		tmp.Empty();		break;
	}

	MakeElement(pElement, _T("DATATYPE"), tmp, false);
	tmp.Format(_T("%d"), type);
	writeFile(tmp);
}

void CWriteXml::subattributes(DWORD attr, DWORD attr2, IXMLDOMElement* pElement)
{
	CString tmp;

	if (attr & FA_ZEROSUP)
	{
		switch (attr2 & SA_ZEROSUP)
		{
		case SA_SPACE:
			tmp += _T("SA_SPACE");
			attr2 |= SA_SPACE;
			tmp += _T(",");
			break;
		case SA_ASTER:
			tmp += _T("SA_ASTER");
			attr2 |= SA_ASTER;
			tmp += _T(",");
			break;
		case SA_ZERO:
			tmp += _T("SA_ZERO");
			attr2 |= SA_ZERO;
			tmp += _T(",");
			break;
		default:
			tmp += _T("SA_ZEROSUP");
			tmp += _T(",");
			break;
		}
		MakeElement(pElement, _T("ZEROSUPATTR2"), tmp, false);
	}

	if (attr & FA_CORGB)
	{
		switch (attr2 & SA_CORGB)
		{
		case SA_CORAW:
			tmp += _T("SA_CORAW");
			attr2 |= SA_CORAW;
			tmp += _T(",");
			break;
		case SA_COSUP:
			tmp += _T("SA_COSUP");
			attr2 |= SA_COSUP;
			tmp += _T(",");
			break;
		case SA_COSIGN:
			tmp += _T("SA_COSIGN");
			attr2 |= SA_COSIGN;
			tmp += _T(",");
			break;
		case SA_TEXT:
			tmp += _T("SA_TEXT");
			attr2 |= SA_TEXT;
			tmp += _T(",");
			break;
		case SA_PAINT:
			tmp += _T("SA_PAINT");
			attr2 |= SA_PAINT;
			tmp += _T(",");
			break;
		case SA_TEXTSUP:	// add 2014. 01. 29.
			tmp += _T("SA_TEXTSUP");
			attr2 |= SA_TEXTSUP;
			tmp += _T(",");
			break;
		case SA_PAINTSUP:	// add 2014. 01. 29.
			tmp += _T("SA_PAINTSUP");
			attr2 |= SA_PAINTSUP;
			tmp += _T(",");
			break;
		default:
			break;
		}

		MakeElement(pElement, _T("CORGBATTR2"), tmp, false);
	}

	if (attr2 & GO_BLINK)
	{
		tmp += _T("BLINK,");
		MakeElement(pElement, _T("GRIDOPTION"), tmp, false);
	}

	tmp.Format(_T("%ld"), attr2);
	writeFile(tmp);
}

void CWriteXml::writeAuxiliary(DWORD properties, IXMLDOMElement* pElement)
{
	CString	tmp;

	if (properties & PR_ENABLE)
	{
		tmp = _T("ENABLE");
		MakeElement(pElement, _T("ENABLE"), tmp, false);
	}
	if (properties & PR_VISIBLE)
	{
		tmp = _T("VISIBLE");
		MakeElement(pElement, _T("VISIBLE"), tmp, false);
	}
	if (properties & PR_CHECKED)
	{
		tmp = _T("CHECKED");
		MakeElement(pElement, _T("CHECKED"), tmp, false);
	}
	if (properties & PR_IMAGE)
	{
		tmp = _T("IMAGE");
		MakeElement(pElement, _T("IMAGE"), tmp, false);
	}
	if (properties & PR_IMAGETEXT)
	{
		tmp = _T("IMAGETEXT");
		MakeElement(pElement, _T("IMGTEXT"), tmp, false);
	}
	if (properties & PR_HOVER)
	{
		tmp = _T("IMAGEHOVER");
		MakeElement(pElement, _T("IMAGETYPE"), tmp, false);
	}
	if (properties & PR_SORT)
	{
		tmp = _T("SORT");
		MakeElement(pElement, _T("SORT"), tmp, false);
	}
	if (properties & PR_EDIT)
	{
		tmp = _T("EDIT");
		MakeElement(pElement, _T("SHOWFORM"), tmp, false);
	}
	if (properties & PR_FIXED)
	{
		tmp = _T("FIXED");
		MakeElement(pElement, _T("FIXED"), tmp, false);
	}
	if (properties & PR_DRAGROW)
	{
		tmp = _T("DRAGROW");
		MakeElement(pElement, _T("DRAGROW"), tmp, false);
	}
	if (properties & PR_OVERLAYIMG)
	{
		tmp = _T("OVERLAYIMG");
		MakeElement(pElement, _T("OVERLAYIMG"), tmp, false);
	}
	if (properties & PR_TABSET)
	{
		tmp = _T("TABIMG");
		MakeElement(pElement, _T("TABIMG"), tmp, false);
	}
	if (properties & PR_RESIZE)
	{
		tmp = _T("TEXTRESIZE");
		MakeElement(pElement, _T("TEXTRESIZE"), tmp, false);
	}
	if (properties & PR_MARGIN)
	{
		tmp = _T("USEMARGIN");
		MakeElement(pElement, _T("USEMARGIN"), tmp, false);
	}
	if (properties & PR_SCREFECT)
	{
		tmp = _T("SCREFECT");
		MakeElement(pElement, _T("SCREFECT"), tmp, false);
	}
	if (properties & PR_REFRESH)
	{
		tmp = _T("REFRESH");
		MakeElement(pElement, _T("REFRESH"), tmp, false);
	}

	if (properties & PR_LANGUAGE)
	{
		tmp = _T("LANGUAGE");
		MakeElement(pElement, _T("LANGUAGE"), tmp, false);
	}

	tmp.Format(_T("%ld"), properties);
	writeFile(tmp);
}

void CWriteXml::writeIOK(int fN, IXMLDOMElement* pElement)
{
	CString tmp;

	switch (m_pFormItem->GetForm(fN)->iok)
	{
	case EIO_INPUT:
		tmp = _T("EIO_INPUT");	break;
	case EIO_INOUT:
		tmp = _T("EIO_INOUT");	break;
	case EIO_OUTPUT:
		tmp = _T("EIO_OUTPUT");	break;
	case EIO_NOP:
	default:
		tmp = _T("EIO_NOP");	break;
	}
	MakeElement(pElement, _T("EIOIOK"), tmp, false);
	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->iok);
	writeFile(tmp);
}

void CWriteXml::writeBoders(int fN, IXMLDOMElement* pElement)
{
	CString tmp;

	switch (m_pFormItem->GetForm(fN)->borders) 
	{
	case BD_NONE:
		tmp = _T("TP_NONE");	break;
	case BD_RAISED:
		tmp = _T("TP_RAISED");	break;
	case BD_SUNKEN:
		tmp = _T("TP_SUNKEN");	break;
	case BD_BUMP:
		tmp = _T("TP_BUMP");	break;
	case BD_ETCHED:
		tmp = _T("TP_ETCHED");	break;
	case BD_LINE:
		tmp = _T("TP_LINE");	break;
	default:
		tmp.Empty();		break;
	}
	MakeElement(pElement, _T("BORDERS"), tmp, false);
	tmp.Format(_T("%d"), m_pFormItem->GetForm(fN)->borders);
	writeFile(tmp);
}

void CWriteXml::writeGridOption(int fN, IXMLDOMElement* pElement)
{
	CString tmp;

	if (m_pFormItem->GetForm(fN)->attr2 & GO_MULTI)
	{
		tmp += _T("MULTI");
		m_pFormItem->GetForm(fN)->attr2 |= GO_MULTI;
		tmp += _T(",");
	}
	if (m_pFormItem->GetForm(fN)->attr2 & GO_FLEX)
	{
		m_pFormItem->GetForm(fN)->attr2 |= GO_FLEX;
		tmp += _T("FLEX");
		tmp += _T(",");
	}
	if (m_pFormItem->GetForm(fN)->attr2 & GO_VSCR)
	{
		m_pFormItem->GetForm(fN)->attr2 |= GO_VSCR;
		tmp += _T("VSCR");
		tmp += _T(",");
	}
	if (m_pFormItem->GetForm(fN)->attr2 & GO_HSCR)
	{
		m_pFormItem->GetForm(fN)->attr2 |= GO_HSCR;
		tmp += _T("HSCR");
		tmp += _T(",");
	}
	if (m_pFormItem->GetForm(fN)->attr2 & GO_TOP)
	{
		m_pFormItem->GetForm(fN)->attr2 |= GO_TOP;
		tmp += _T("TOP");
		tmp += _T(",");
	}
	if (m_pFormItem->GetForm(fN)->attr2 & GO_XHEAD)
	{
		m_pFormItem->GetForm(fN)->attr2 |= GO_XHEAD;
		tmp += _T("XHEAD");
		tmp += _T(",");
	}
	if (m_pFormItem->GetForm(fN)->attr2 & GO_ROW)	
	{
		m_pFormItem->GetForm(fN)->attr2 |= GO_ROW;
		tmp += _T("ROW");
		tmp += _T(",");
	}
	if (m_pFormItem->GetForm(fN)->attr2 & GO_XVLINE)	
	{
		m_pFormItem->GetForm(fN)->attr2 |= GO_XVLINE;
		tmp += _T("XVLINE");
		tmp += _T(",");
	}
	if (m_pFormItem->GetForm(fN)->attr2 & GO_XHLINE)	
	{
		m_pFormItem->GetForm(fN)->attr2 |= GO_XHLINE;
		tmp += _T("XHLINE");
		tmp += _T(",");
	}
	if (m_pFormItem->GetForm(fN)->attr2 & GO_HEADER)
	{	
		m_pFormItem->GetForm(fN)->attr2 |= GO_HEADER;
		tmp += _T("HEADER");
		tmp += _T(",");
	}
	if (m_pFormItem->GetForm(fN)->attr2 & GO_FIX)	
	{
		m_pFormItem->GetForm(fN)->attr2 |= GO_FIX;
		tmp += _T("FIX");
		tmp += _T(",");
	}
	if (m_pFormItem->GetForm(fN)->attr2 & GO_MERGE)	
	{
		m_pFormItem->GetForm(fN)->attr2 |= GO_MERGE;
		tmp += _T("MERGE");
		tmp += _T(",");
	}
	if (m_pFormItem->GetForm(fN)->attr2 & GO_VALID)	
	{
		m_pFormItem->GetForm(fN)->attr2 |= GO_VALID;
		tmp += _T("VALID");
		tmp += _T(",");
	}
	if (m_pFormItem->GetForm(fN)->attr2 & GO_AUTOSIZE)	
	{
		m_pFormItem->GetForm(fN)->attr2 |= GO_AUTOSIZE;
		tmp += _T("AUTOSIZE");
		tmp += _T(",");
	}
	if (m_pFormItem->GetForm(fN)->attr2 & GO_DRAGCOL)	
	{
		m_pFormItem->GetForm(fN)->attr2 |= GO_DRAGCOL;
		tmp += _T("DRAGCOL");
		tmp += _T(",");
	}
	if (m_pFormItem->GetForm(fN)->attr2 & GO_XINLINE)	
	{
		m_pFormItem->GetForm(fN)->attr2 |= GO_XINLINE;
		tmp += _T("INLINE");
		tmp += _T(",");
	}

	if (m_pFormItem->GetForm(fN)->attr2 & GO_BLINK)
	{
		tmp += _T("BLINK,");
	}

	MakeElement(pElement, _T("GRIDOPTION"), tmp, false);
	tmp.Format(_T("%ld"), m_pFormItem->GetForm(fN)->attr2);
	writeFile(tmp);
}

void CWriteXml::writeColumns(int kind, struct _repR* repR, IXMLDOMElement* pElement)
{
	CString tmp;
	CCellItem *cellItem = new CCellItem(repR);
	IXMLDOMElement*	pChildElement;

	pChildElement = MakeElement(pElement, _T("MULTICOL"), _T(""), false);

	for (int ii = 0; ii < repR->cols; ii++)
	{
		m_writeType = SUBITEM;
		writeFile();
		m_writeType = PROPERT;

		struct _cellR *cellR = cellItem->GetCell(ii);

		tmp.Format(_T("%s"), cellR->name);
		MakeElement(pChildElement, _T("COLINFO"), tmp);
		tmp.Format(_T("%d"), cellR->size);
		MakeElement(pChildElement, _T("CELLLEN"), tmp);
		tmp.Format(_T("%d"), cellR->width);
		MakeElement(pChildElement, _T("WIDTH"), tmp);

		if (cellR->kind == CK_COMBO)
		{
			tmp.Format(_T("%s"), keyMap[cellR->vals2]);
			MakeElement(pChildElement, _T("MATCHKEY"), tmp, false);
			tmp.Format(_T("%d"), cellR->vals2);
			writeFile(tmp);
		}
		else
			writeFile();
		if (cellR->head[0] != NULL)
		{
			tmp.Format(_T("%s"), cellR->head);
			MakeElement(pChildElement, _T("HEAD"), tmp);
		}
		else
			writeFile();

		switch (cellR->alignH)
		{
		case AL_LEFT:
			tmp = _T("AL_LEFT");         break;
		case AL_RIGHT:
			tmp = _T("AL_RIGHT");         break;
		case AL_CENTER:
		default:
			tmp = _T("AL_CENTER");        break;
		}
		MakeElement(pChildElement, _T("COLALIGNH"), tmp, false);

		tmp.Format(_T("%d"), cellR->alignH);
		writeFile(tmp);

		switch (cellR->alignC)
		{
		case AL_CENTER:
			tmp = _T("AL_CENTER");        break;
		case AL_LEFT:
			tmp = _T("AL_LEFT");          break;
		case AL_RIGHT:
		default:
			tmp = _T("AL_RIGHT");         break;
		}
		MakeElement(pChildElement, _T("COLALIGNC"), tmp, false);
		tmp.Format(_T("%d"), cellR->alignC);
		writeFile(tmp);

		tmp.Format(_T("%ld"), RGBtoARGB(cellR->hpRGB));
		MakeElement(pChildElement, _T("HPRGB"), tmp);
		tmp.Format(_T("%ld"), RGBtoARGB(cellR->htRGB));
		MakeElement(pChildElement, _T("HTRGB"), tmp);

		if (cellR->vals)
		{
			cellR->attr |= FA_FLOAT;
			tmp.Format(_T("%ld"), cellR->vals);
			MakeElement(pChildElement, _T("FLOAT"), tmp);
		}
		else
		{
			cellR->attr &= ~FA_FLOAT;
			writeFile();
		}

		dataType(cellR->type, pChildElement);
		switch (cellR->iok)
		{
		case EIO_INPUT:
			tmp = _T("EIO_INPUT");	break;
		case EIO_INOUT:
			tmp = _T("EIO_INOUT");	break;
		case EIO_OUTPUT:
			tmp = _T("EIO_OUTPUT");	break;
		case EIO_NOP:
		default:
			tmp = _T("EIO_NOP");	break;
		}
		MakeElement(pChildElement, _T("EIOIOK"), tmp, false);
		tmp.Format(_T("%d"), cellR->iok);
		writeFile(tmp);

		attributes(cellR->attr, pChildElement);
		subattributes(cellR->attr, cellR->attr2, pChildElement);
		formcolor(cellR->pRGB, cellR->tRGB, pChildElement);

		if (cellR->auxS[0] != NULL)
		{
			tmp.Format(_T("%s"), cellR->auxS);
			MakeElement(pChildElement, _T("DOM"), tmp);
		}
		else
			writeFile();

		writeAuxiliary(cellR->properties, pChildElement);
		
		tmp.Format(_T("%s"), cellR->editS);
		if (!tmp.IsEmpty())
		{
			tmp.TrimLeft();	tmp.TrimRight();
		}
		MakeElement(pChildElement, _T("EDITFORMAT"), tmp, false);
		tmp.Format(_T("%d%c%d"), HIWORD(cellR->onEdit), DELIMETER, LOWORD(cellR->onEdit));
		MakeElement(pChildElement, _T("IFRGB"), tmp, false);

		tmp.Format(_T("%s"), cellR->editS);
		writeFile(tmp);
		tmp.Format(_T("%d%c%d"), HIWORD(cellR->onEdit), GB_PROPERT, LOWORD(cellR->onEdit));
		writeFile(tmp);

		if (cellR->kind == CK_COMBO && cellR->keys != 0)
		{
			tmp.Format(_T("%d"), cellR->keys);
			MakeElement(pChildElement, _T("COUNT"), tmp);

			if (cellR->keys != 0)
			{
				CString tmps, data;
				int ii, count = cellR->keys;

				tmp = _T("");
				data = cellR->dat;
				for (ii = 0; ii < count; ii++)
				{
					tmps.Format(_T("%s"), Parser(data, "\t"));
					if (ii < count - 1)
						tmps += _T(",");

					tmp += tmps;
				}
				MakeElement(pChildElement, _T("DISPLAY"), tmp);

				tmp = _T("");
				data = cellR->str;
				for (ii = 0; ii < count; ii++)
				{
					tmps.Format(_T("%s"), Parser(data, "\t"));
					if (ii < count - 1)
						tmps += _T(",");

					tmp += tmps;
				}
				MakeElement(pChildElement, _T("DATA"), tmp);
			}
			else
			{
				writeFile();
				writeFile();
			}
		}
		else
		{
			writeFile();
			writeFile();
			writeFile();
		}

		if (cellR->kind == CK_CHECK)// && cellR->str2[0] != '\0')
		{
			if (!(cellR->properties & PR_IMAGE))
			{
				tmp.Format(_T("%s"), cellR->str2);
				MakeElement(pChildElement, _T("CHECKSTR"), tmp);
			}
			else
				writeFile();
		}
		else
			writeFile();

		if (cellR->kind == CK_BUTTON)
		{
			if (!(cellR->properties & PR_IMAGE))
			{
				tmp.Format(_T("%s"), cellR->txt);
				MakeElement(pChildElement, _T("BUTTONSTR"), tmp);
			}
			else
				writeFile();
		}
		else
			writeFile();
			
		if ((cellR->properties & PR_IMAGE) || (cellR->properties & PR_IMAGETEXT))
		{
			CString tmps = cellR->str3;
			int	offs = tmps.ReverseFind('\\');
			if (offs != -1)
				tmps = tmps.Mid(offs + 1);
			tmp.Format(_T("%s"), tmps);
			MakeElement(pChildElement, _T("BUTTONIMG"), tmp);
		}
		else
			writeFile();

		tmp.Format(_T("%s"), cellR->rtssy);
		MakeElement(pChildElement, _T("FORMRTSSYMBOL"), tmp);

		// 
		writeCellKind(cellR->kind, pChildElement);

		// font info		
		cellFont(cellR, pChildElement);

		// Merge Column Name
		tmp = cellR->merge;
		MakeElement(pChildElement, _T("MERGECOLUMN"), tmp);

		// head image
		tmp = cellR->str4;
		MakeElement(pChildElement, _T("COLUMNHEADIMG"), tmp);

		// column height in multiline grid
		tmp.Format(_T("%d"), cellR->height);
		MakeElement(pChildElement, _T("COLUMNHEIGHT"), tmp);

		// column head height in multiline grid
		tmp.Format(_T("%d"), cellR->headH);
		MakeElement(pChildElement, _T("COLUMNHEADHEIGHT"), tmp);

		if (cellR->kind == CK_CUSTOM)
		{
			tmp.Format(_T("%s,%s"), cellR->dat, cellR->str);
			MakeElement(pChildElement, _T("CELLCUSTOM"), tmp, false);
			writeFile(cellR->dat);
			writeFile(cellR->str);
		}
		else
		{
			writeFile();
			writeFile();
		}

		if (cellR->margin)
		{
			tmp = _T("");
			for (int ii = 0; ii < cellR->margin->GetCount(); ii++)
			{
				tmp += cellR->margin->GetAt(ii);
				if (ii < cellR->margin->GetUpperBound())
					tmp += _T(',');
			}
			MakeElement(pChildElement, _T("MARGIN"), tmp);
		}
		else
			writeFile();

		m_writeType = PROPERTLAST;

		switch (cellR->chartLimit)
		{
		case 1:
			tmp = _T("1");         break;
		case 0:
		default:
			tmp = _T("0");         break;
		}
		MakeElement(pChildElement, _T("CHARTLIMIT"), tmp, false);
		tmp.Format(_T("%d"), cellR->chartLimit);
		writeFile(tmp);
	}

	delete cellItem;
}

void CWriteXml::writeCellKind(BYTE kind, IXMLDOMElement* pElement)
{
	switch (kind)
	{
		case CK_NORMAL:
			MakeElement(pElement, _T("CELLKIND"), _T("0"));	break;
		case CK_CHECK:
			MakeElement(pElement, _T("CELLKIND"), _T("1"));	break;
		case CK_COMBO:
			MakeElement(pElement, _T("CELLKIND"), _T("2"));	break;
		case CK_BUTTON:
			MakeElement(pElement, _T("CELLKIND"), _T("3"));	break;
		case CK_CUSTOM:
			MakeElement(pElement, _T("CELLKIND"), _T("4"));	break;
		case CK_CHART:
			MakeElement(pElement, _T("CELLKIND"), _T("9"));	break;
		case CK_C_CHART:
			MakeElement(pElement, _T("CELLKIND"), _T("10"));	break;
	}
}

void CWriteXml::writeRows(struct _repR* repR, IXMLDOMElement* pElement)
{
	CString tmp;

	CCellItem *cellItem = new CCellItem(repR);
	IXMLDOMElement* pChildElement;

	pChildElement = MakeElement(pElement, _T("MULTICOL"), _T(""), false);

	for (int ii = 0; ii < repR->rows; ii++)
	{
		m_writeType = SUBITEM;
		writeFile();
		m_writeType = PROPERT;

		struct _cellR *cellR = cellItem->GetCell(ii);
		tmp.Format(_T("%s"), cellR->name);
		MakeElement(pChildElement, _T("COLINFO"), tmp);
		tmp.Format(_T("%d"), cellR->size);
		MakeElement(pChildElement, _T("CELLLEN"), tmp);
		tmp.Format(_T("%d"), cellR->width);
		MakeElement(pChildElement, _T("WIDTH"), tmp);

		if (cellR->head[0] != NULL)
		{
			tmp.Format(_T("%s"), cellR->head);
			MakeElement(pChildElement, _T("HEAD"), tmp);
		}
		else
			writeFile();

		switch (cellR->alignH)
		{
		case AL_CENTER:
			tmp = _T("AL_CENTER");        break;
		case AL_RIGHT:
			tmp = _T("AL_RIGHT");         break;
		case AL_LEFT:
		default:
			tmp = _T("AL_LEFT");          break;
		}
		MakeElement(pChildElement, _T("COLALIGNH"), tmp, false);
		tmp.Format(_T("%d"), cellR->alignH);
		writeFile(tmp);

		switch (cellR->alignC)
		{
		case AL_CENTER:
			tmp = _T("AL_CENTER");        break;
		case AL_LEFT:
			tmp = _T("AL_LEFT");          break;
		case AL_RIGHT:
		default:
			tmp = _T("AL_RIGHT");         break;
		}
		MakeElement(pChildElement, _T("COLALIGNC"), tmp, false);
		tmp.Format(_T("%d"), cellR->alignC);
		writeFile(tmp);

		tmp.Format(_T("%ld"), RGBtoARGB(cellR->hpRGB));
		MakeElement(pChildElement, _T("HPRGB"), tmp);
		tmp.Format(_T("%ld"), RGBtoARGB(cellR->htRGB));
		MakeElement(pChildElement, _T("HTRGB"), tmp);

		if (cellR->vals)
		{
			cellR->attr |= FA_FLOAT;
			tmp.Format(_T("%ld"), cellR->vals);
			MakeElement(pChildElement, _T("FLOAT"), tmp);
		}
		else 
		{
			cellR->attr &= ~FA_FLOAT;
			writeFile();
		}

		dataType(cellR->type, pChildElement);

		switch (cellR->iok)
		{
		case EIO_INPUT:
			tmp = _T("EIO_INPUT");	break;
		case EIO_INOUT:
			tmp = _T("EIO_INOUT");	break;
		case EIO_OUTPUT:
			tmp = _T("EIO_OUTPUT");	break;
		case EIO_NOP:
		default:
			tmp = _T("EIO_NOP");	break;
		}
		MakeElement(pChildElement, _T("EIOIOK"), tmp, false);
		tmp.Format(_T("%d"), cellR->iok);
		writeFile(tmp);

		attributes(cellR->attr, pChildElement);
		subattributes(cellR->attr, cellR->attr2, pChildElement);
		formcolor(cellR->pRGB, cellR->tRGB, pChildElement);

		if (cellR->auxS[0] != NULL)
		{
			tmp.Format(_T("%s"), cellR->auxS);
			MakeElement(pChildElement, _T("DOM"), tmp);
		}
		else
			writeFile();

		writeAuxiliary(cellR->properties, pChildElement);
		//writeCellKind(cellR->kind, pChildElement);

		tmp.Format(_T("%s"), cellR->editS);
		if (!tmp.IsEmpty())
		{
			tmp.TrimLeft();	tmp.TrimRight();
		}
		MakeElement(pChildElement, _T("EDITFORMAT"), tmp, false);

		tmp.Format(_T("%d%c%d"), HIWORD(cellR->onEdit), DELIMETER, LOWORD(cellR->onEdit));
		MakeElement(pChildElement, _T("IFRGB"), tmp, false);

		tmp.Format(_T("%s"), cellR->editS);
		writeFile(tmp);
		tmp.Format(_T("%d"), HIWORD(cellR->onEdit));
		writeFile(tmp);
		tmp.Format(_T("%d"), LOWORD(cellR->onEdit));
		writeFile(tmp);

		m_writeType = PROPERTLAST;
		tmp.Format(_T("%s"), cellR->rtssy);
		MakeElement(pChildElement, _T("FORMRTSSYMBOL"), tmp);
	}

	delete cellItem;
}

void CWriteXml::generateFormScript(IXMLDOMElement* pElement)
{
	CString tmp, script;
	IXMLDOMElement* pChildElement;

	MakeElement(pElement, _T("PATH_ONLOAD"), CString(m_mapH->onLoadN), false);
	MakeElement(pElement, _T("PATH_ONSEND"), CString(m_mapH->onSendN), false);
	MakeElement(pElement, _T("PATH_ONRECIVE"), CString(m_mapH->onReceiveN), false);
	MakeElement(pElement, _T("PATH_ONALERT"), CString(m_mapH->onAlertN), false);
	MakeElement(pElement, _T("PATH_ONTIMMER"), CString(m_mapH->onTimerN), false);
	MakeElement(pElement, _T("PATH_ONCLOSE"), CString(m_mapH->onCloseN), false);
	MakeElement(pElement, _T("PATH_ONDEVICE"), CString(m_mapH->onDeviceN), false);
	MakeElement(pElement, _T("PATH_ONERROR"), CString(m_mapH->onErrorN), false);
	MakeElement(pElement, _T("PATH_ONFILCKING"), CString(m_mapH->onFlickingN), false);
	MakeElement(pElement, _T("PATH_ONCHANGELAYOUT"), CString(m_mapH->onChangeLayoutN), false);
	MakeElement(pElement, _T("PATH_ONDECLARATION"), CString(m_mapH->onDeclarationN), false);
	MakeElement(pElement, _T("PATH_ONINDECLARATION"), CString(m_mapH->onInDeclarationN), false);

	m_writeType = SCRIPT;
	writeFile();

	pChildElement = MakeElement(pElement, _T("SCRIPT"), _T(""), false);

	if (m_mapH->onLoad && m_mapH->onLoadN[0] != NULL)
		writeScriptData(m_mapH->onLoadN, _T("_am_onload_am_"), pChildElement);

	if (m_mapH->onSend && m_mapH->onSendN[0] != NULL)
		writeScriptData(m_mapH->onSendN, _T("_am_onsend_am_"), pChildElement);

	if (m_mapH->onReceive && m_mapH->onReceiveN[0] != NULL)
		writeScriptData( m_mapH->onReceiveN, _T("_am_onrecv_am_"), pChildElement);

	if (m_mapH->onAlert && m_mapH->onAlertN[0] != NULL)
		writeScriptData(m_mapH->onAlertN, _T("_am_onalert_am_"), pChildElement);

	if (m_mapH->onTimer && m_mapH->onTimerN[0] != NULL)
		writeScriptData( m_mapH->onTimerN, _T("_am_ontimer_am_"), pChildElement);

	if (m_mapH->onClose && m_mapH->onCloseN[0] != NULL)
		writeScriptData(m_mapH->onCloseN, _T("_am_onclose_am_"), pChildElement);

	if (m_mapH->onDevice && m_mapH->onDeviceN[0] != NULL)
		writeScriptData(m_mapH->onDeviceN, _T("_am_ondevice_am_"), pChildElement);

	if (m_mapH->onError && m_mapH->onErrorN[0] != NULL)
		writeScriptData(m_mapH->onErrorN, _T("_am_onerror_am_"), pChildElement);

	if (m_mapH->onChangeLayout && m_mapH->onChangeLayoutN[0] != NULL)
		writeScriptData(m_mapH->onChangeLayoutN, _T("_am_onchangelayout_am_"), pChildElement);

	if (m_mapH->onFlicking && m_mapH->onFlickingN[0] != NULL)
		writeScriptData(m_mapH->onFlickingN, _T("_am_onflicking_am_"), pChildElement);

	if (m_mapH->onDeclaration && m_mapH->onDeclarationN[0] != NULL)
		writeScriptData(m_mapH->onDeclarationN, _T("_am_ondeclaration_am_"), pChildElement);

	if (m_mapH->onInDeclaration && m_mapH->onInDeclarationN[0] != NULL)
		writeScriptData(m_mapH->onInDeclarationN, _T("_am_onindeclaration_am_"), pChildElement);

	m_writeType = NEWFORM;
	writeFile();

	removeIXMLDOMElement(pChildElement);
}

void CWriteXml::generateControlScript(int fN, IXMLDOMElement* pElement)
{
	CString tmp, script, removeComment;
	IXMLDOMElement*	pChildElement = NULL;

	m_writeType = SCRIPT;
	writeFile();
	pChildElement = MakeElement(pElement, _T("SCRIPT"), _T(""), false);

	int	scpSize = 0;
	char*	sbuff;
	bool	edit;
	CFile	file;

	char*	cTap		= new char[1024*32];
	char*	cLongTab	= new char[1024*32];
	char*	cChange		= new char[1024*32];
	char*	cChar		= new char[1024*32];
	char*	cSCroll		= new char[1024*32];


	if (m_pFormItem->GetFormStr(fN, _T("name")).IsEmpty())
	{
		delete[] cTap;
		delete[] cLongTab;
		delete[] cChange;	
		delete[] cChar;
		delete[] cSCroll;
		return;
	}
	edit = false;
	//if (m_pFormItem->GetForm(fN)->isEdit != IE_NOP)
	//	edit = true;

	if (file.Open(m_pFormItem->GetFormStr(fN, _T("scpN")), CFile::modeRead))
	{
		scpSize = (UINT)file.GetLength();

		//if (scpSize == 0xFFFFFFFF)
		//{
		//	if (!edit)
		//		return;
		//}

		sbuff = new char[scpSize+1];
		ZeroMemory(sbuff, scpSize+1);
		UINT rc = file.Read(sbuff, scpSize);
		if (rc != scpSize)
		{
			delete[] sbuff;
			file.Close();
			if (!edit)
			{
				delete[] cTap;
				delete[] cLongTab;
				delete[] cChange;	
				delete[] cChar;
				delete[] cSCroll;
				return;
			}
		}

		cTap[0]		= NULL;
		cLongTab[0]	= NULL;
		cChange[0]	= NULL;
		cChar[0]	= NULL;
		cSCroll[0]	= NULL;

		if (m_pFormItem->GetForm(fN)->onTap != -1)
		{
			CopyMemory(cTap, &sbuff[m_pFormItem->GetForm(fN)->onTap], m_pFormItem->GetForm(fN)->onTapL);
			cTap[m_pFormItem->GetForm(fN)->onTapL] = NULL;
		}
		if (m_pFormItem->GetForm(fN)->onLongTap != -1)
		{
			CopyMemory(cLongTab, &sbuff[m_pFormItem->GetForm(fN)->onLongTap], m_pFormItem->GetForm(fN)->onLongTapL);
			cLongTab[m_pFormItem->GetForm(fN)->onLongTapL] = NULL;
		}
		if (m_pFormItem->GetForm(fN)->onChange != -1)
		{
			CopyMemory(cChange, &sbuff[m_pFormItem->GetForm(fN)->onChange], m_pFormItem->GetForm(fN)->onChangeL);
			cChange[m_pFormItem->GetForm(fN)->onChangeL] = NULL;
		}
		if (m_pFormItem->GetForm(fN)->onChar != -1)
		{
			CopyMemory(cChar, &sbuff[m_pFormItem->GetForm(fN)->onChar], m_pFormItem->GetForm(fN)->onCharL);
			cChar[m_pFormItem->GetForm(fN)->onCharL] = NULL;
		}
		/*if (m_pFormItem->GetForm(fN)->onDrag != -1)
		{
			CopyMemory(cChar, &sbuff[m_pFormItem->GetForm(fN)->onDrag], m_pFormItem->GetForm(fN)->onDragL);
			cChar[m_pFormItem->GetForm(fN)->onDragL] = NULL;
		}*/
		if (m_pFormItem->GetForm(fN)->onScroll != -1)
		{
			CopyMemory(cSCroll, &sbuff[m_pFormItem->GetForm(fN)->onScroll], m_pFormItem->GetForm(fN)->onScrollL);
			cSCroll[m_pFormItem->GetForm(fN)->onScrollL] = NULL;
		}

		delete[] sbuff;
		sbuff = NULL;

		tmp = m_pFormItem->GetFormStr(fN, _T("name"));

		int nKind = m_pFormItem->GetForm(fN)->kind;

		if (cTap[0] != NULL && m_pFormItem->GetForm(fN)->onTapL != 0 && 
			(nKind == FM_IMAGEVIEW  || 
			 nKind == FM_EDIT  ||
			 nKind == FM_OUT	||
			 nKind == FM_LABEL ||
			 nKind == FM_BUTTON ||
			 nKind == FM_TAB ||
			 nKind == FM_CHECK ||
			 nKind == FM_RADIO ||
			 nKind == FM_GRID ||
			 nKind == FM_GRIDEX ||
			 nKind == FM_CONTROL ||
			 nKind == FM_TABLE ))
		{
			script = CString(cTap, strlen(cTap));
			removeComment = RemoveComment(script);			
			MakeElementDiff(pChildElement, _T("_am_onclick_am_"), script, removeComment);
			//MakeElement(pChildElement, _T("_am_onclick_am_"), removeComment);
		}

		if (cLongTab[0] != NULL && m_pFormItem->GetForm(fN)->onLongTapL != 0 && 
			(nKind == FM_IMAGEVIEW  || 
			 nKind == FM_LABEL ||
			 nKind == FM_OUT	||
			 nKind == FM_BUTTON ||
			 nKind == FM_TAB ||
			 nKind == FM_CHECK ||
			 nKind == FM_RADIO ||
			 nKind == FM_TABLE  ||
			 nKind == FM_CONTROL ||
			 nKind == FM_GRID ||
			 nKind == FM_GRIDEX ))
		{
			script = CString(cLongTab, strlen(cLongTab));
			removeComment = RemoveComment(script);			
			MakeElementDiff(pChildElement, _T("_am_onlongtap_am_"), script, removeComment);
			//MakeElement(pChildElement, _T("_am_onlongtap_am_"), script);
		}

		if (cChange[0] != NULL && m_pFormItem->GetForm(fN)->onChangeL != 0 &&
			(nKind == FM_EDIT ||
			 nKind == FM_GRID ||
			 nKind == FM_GRIDEX ||
			 nKind == FM_CONTROL ||
			 nKind == FM_COMBO))
		{
			script = CString(cChange, strlen(cChange));
			removeComment = RemoveComment(script);			
			MakeElementDiff(pChildElement, _T("_am_onchange_am_"), script, removeComment);
			//MakeElement(pChildElement, _T("_am_onchange_am_"), script);
		}

		if (cChar[0] != NULL && m_pFormItem->GetForm(fN)->onCharL != 0 
			&& (nKind == FM_EDIT || nKind == FM_GRID || nKind == FM_GRIDEX))
		{
			script = CString(cChar, strlen(cChar));
			removeComment = RemoveComment(script);			
			MakeElementDiff(pChildElement, _T("_am_onchar_am_"), script, removeComment);
			//MakeElement(pChildElement, _T("_am_onchar_am_"), script);
		}

		if (cChar[0] != NULL && m_pFormItem->GetForm(fN)->onDragL != 0 
			&& (nKind == FM_GRID || nKind == FM_GRIDEX))	// complex grid
		{
			script = CString(cChar, strlen(cChar));
			removeComment = RemoveComment(script);
			MakeElementDiff(pChildElement, _T("_am_ondrag_am_"), script, removeComment);
			//MakeElement(pChildElement, _T("_am_ondrag_am_"), script);
		}

		if (cSCroll[0] != NULL && m_pFormItem->GetForm(fN)->onScrollL != 0 
			&& (nKind == FM_GRID || nKind == FM_GRIDEX))	// complex grid
		{
			script = CString(cSCroll, strlen(cSCroll));
			removeComment = RemoveComment(script);
			MakeElementDiff(pChildElement, _T("_am_onscroll_am_"), script, removeComment);			
		}

	}

	delete[] cTap;
	delete[] cLongTab;
	delete[] cChange;	
	delete[] cChar;
	delete[] cSCroll;

	m_writeType = NEWFORM;
	writeFile();

	removeIXMLDOMElement(pChildElement);
}

void CWriteXml::writeScriptData(char* dir, CString name, IXMLDOMElement* pElement)
{
	CFile	file;
	UINT	nFilesize = 0;
	char*	szData = NULL;
	CString removeComment;

	if (file.Open(dir, CFile::modeRead))
	{
		nFilesize = (UINT)file.GetLength();
		szData = new char[nFilesize+1];
		ZeroMemory(szData, nFilesize+1);
		UINT rc = file.Read(szData, nFilesize);
		if (rc != nFilesize)
		{
			delete[] szData;
			file.Close();
			return;
		}
	}

	removeComment = RemoveComment(CString(szData));			
	MakeElementDiff(pElement, name, CString(szData), removeComment);
	//MakeElement(pElement, name, CString(szData));

	delete[] szData;
	file.Close();
	writeFile();
}

void CWriteXml::groupN(WORD idx, IXMLDOMElement* pElement)
{
	CString	tmp;

	if (m_pFormItem->GetForm(idx)->group)
	{
		tmp.Format(_T("%d"), m_pFormItem->GetIdxOfPtr(m_pFormItem->GetForm(idx)->group));
		MakeElement(pElement, _T("GROUPN"), tmp, false);
	}
}

CString CWriteXml::Parser(CString &srcstr, CString substr)
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
	return _T("");
}

IXMLDOMElement* CWriteXml::BaseWrite(CString kind, int fN, IXMLDOMElement* pElement)
{
	CString		tmp, tmps;
	IXMLDOMElement*	pChildElement;

	tmp.Format(_T("%s-SYMBOL_%s"), kind, m_pFormItem->GetFormStr(fN, _T("name")));
	pChildElement = MakeElement(pElement, tmp, _T(""), false);
	symbolRect(fN, pChildElement);

	return pChildElement;
}

IXMLDOMElement* CWriteXml::MakeElement(IXMLDOMElement* pElement, CString name, CString sValue, bool bresource)
{
	HRESULT		hr;
	IXMLDOMNode*	pNode = NULL;
	IXMLDOMElement*	pChildElement = NULL;

	hr = m_pWriteDoc->createElement(_bstr_t(name), &pChildElement);

	if (hr == S_OK && !name.IsEmpty())
	{
		if (!sValue.IsEmpty())
		{
			//if (bArribute)
			//	SetElementToAttribute(pElement, name, sValue);
			//else
				SetElementToText(pChildElement, sValue);
		}

		if (bresource)
			writeFile(sValue, name);

		hr = pElement->appendChild(pChildElement, &pNode);
	}

	removeIXMLDOMNode(pNode);
	return pChildElement;
}

// true : XML만 , false : Runtime XML만
IXMLDOMElement* CWriteXml::MakeElementDiff(IXMLDOMElement* pElement, CString name, CString sXML, CString sRXML)
{
	HRESULT		hr;
	IXMLDOMNode*	pNode = NULL;
	IXMLDOMElement*	pChildElement = NULL;

	hr = m_pWriteDoc->createElement(_bstr_t(name), &pChildElement);

	if (hr == S_OK && !name.IsEmpty())
	{
		if (!sXML.IsEmpty())
			SetElementToText(pChildElement, sXML);

		if (!sRXML.IsEmpty())
			writeFile(sRXML, name);

		hr = pElement->appendChild(pChildElement, &pNode);
	}

	removeIXMLDOMNode(pNode);
	return pChildElement;
}


void CWriteXml::writeFile(CString buf, CString name)
{
//Msg("Write %s : %s", buf, name);

	if (m_bSrc)
	{
		m_sSrc += buf;
		return;
	}

	if (!m_bOpen)
	{
		AfxMessageBox(_T("XError [b4]"));
		return;
	}

	CString	tmp;

	if (buf.Find(_T("_SYMBOL_")) > 0)
		buf.Replace(_T("_SYMBOL_"), _T(""));

	if (buf.IsEmpty())
		buf = _T(" ");

	TRY
	{
		switch (m_writeType)
		{
		case PROPERT:
			tmp.Format(_T("%s%c"), buf, GB_PROPERT);
			break;
		case PROPERTLAST:
			tmp.Format(_T("%s"), buf);
			break;
		case SCRIPT:
			tmp.Format(_T("%c%s%c%s"), GB_SCRIPT, name, GB_PROPERT, buf);
			break;
		case NEWFORM:
			tmp.Format(_T("%c"), GB_CLASS);
			break;
		case SUBITEM:
			tmp.Format(_T("%c"), GB_SUITEM);
			break;
		case TRLAYOUT:
			tmp.Format(_T("%s"), buf);
			break;
		case NONE:
		default:
			break;
		}

		CString str = AnsiToUTF8(tmp);	
		m_file.Write(str, str.GetLength());
	}
	CATCH (CFileException, e)
	{
		tmp.Format(_T("XError [b6] [%d]"), e->m_cause);
		AfxMessageBox(tmp);
	}
	END_CATCH
}

/*void CWriteXml::Encrypt(CString path)
{
	CFile file;
	if (!file.Open(path, CFile::modeReadWrite|CFile::typeBinary))
		return;
	
	char	*src, *des;
	int mapL = (int)file.GetLength();		
	
	src = new char[mapL];
	file.Read(src, mapL);
		
	des = new char[mapL + 4];
	ZeroMemory(des, mapL + 4);
	mapL = axEncrypt(src, mapL, des);
	file.SeekToBegin();
	file.Write(des, mapL);
	file.Close();

	delete[] src;
	delete[] des;
}*/



void CWriteXml::SetElementToAttribute(IXMLDOMElement* pElement, CString name, CString value)
{
	pElement->setAttribute(_bstr_t(name), CComVariant((LPCTSTR)value));
}

void CWriteXml::SetElementToText(IXMLDOMElement* pElement, CString text)
{
	pElement->put_text(_bstr_t(text));
}

void CWriteXml::removeIXMLDOMNode(IXMLDOMNode* pNode)
{
	if (pNode)
	{
		pNode->Release();
		pNode = NULL;
	}
}

void CWriteXml::removeIXMLDOMElement(IXMLDOMElement* pElement)
{
	if (pElement)
	{
		pElement->Release();
		pElement = NULL;
	}
}

void CWriteXml::removeIXMLDOMNamedNodeMap(IXMLDOMNamedNodeMap* pNodename)
{
	if (pNodename)
	{
		pNodename->Release();
		pNodename = NULL;
	}
}

void CWriteXml::removeIXMLDOMAttribute(IXMLDOMAttribute* pAttribute)
{
	if(pAttribute)
	{
		pAttribute->Release();
		pAttribute = NULL;
	}
}
// CWriteXml 메시지 처리기입니다.

void CWriteXml::SetpixFont(int fN)
{
	bool	fstyle, bold;
	DWORD	fheight, point;
	DWORD	dwcase;
	CString tmp;

	tmp    = m_pFormItem->GetFormStr(fN, _T("fName"));
	dwcase = m_pFormItem->GetForm(fN)->fStyle;
	point  = m_pFormItem->GetForm(fN)->fPoint;

	if (tmp.IsEmpty())
	{
		tmp.Format(_T("%s"), m_mapH->fName);
		point = m_mapH->fPoint;
	}

	switch (dwcase)
	{
	case 1:
		fstyle = true;
		bold = false;
		break;
	case 2:
		fstyle = false;
		bold = true;
		break;
	case 3:
		fstyle = true;
		bold = true;
		break;
	default:
		fstyle = false;
		bold = false;
		break;
	}

	fheight = FontHeight((char*)tmp.operator LPCTSTR(), point, fstyle, bold);
	tmp.Format(_T("%d"), fheight);
	writeFile(tmp);
}

void CWriteXml::SetCellpixFont(struct _cellR* cellR)
{
	bool	fstyle, bold;
	DWORD	fheight, point;
	DWORD	dwcase;
	CString tmp;

	tmp    = cellR->fName;
	dwcase = cellR->fStyle;
	point  = cellR->fPoint;

	if (tmp.IsEmpty())
	{
		tmp.Format(_T("%s"), m_mapH->fName);
		point = m_mapH->fPoint;
	}

	switch (dwcase)
	{
	case 1:
		fstyle = true;
		bold = false;
		break;
	case 2:
		fstyle = false;
		bold = true;
		break;
	case 3:
		fstyle = true;
		bold = true;
		break;
	default:
		fstyle = false;
		bold = false;
		break;
	}

	fheight = FontHeight((char*)tmp.operator LPCTSTR(), point, fstyle, bold);
	tmp.Format(_T("%d"), fheight);
	writeFile(tmp);
}
/*
void CWriteXml::loadFile()
{
	CString path, desc;
	CString tmps, text, sym, tmp, idx, retval, str, han;
	char	wb[1024];
	DWORD	value;

	path.Format(_T("%s\\%s\\control.ini"), (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0), TABDIR);
	for (int ii = 1; ii < 100; ii++)
	{
		tmps.Format(_T("%02d"), ii);
		idx = tmps;

		GetPrivateProfileString(_T("Custom Objects"), tmps, _T(""), wb, sizeof(wb), path);
		desc = wb;
		tmps = Parser(desc, _T(";"));
		han  = Parser(desc, _T(";"));

		if (tmps.IsEmpty())
			break;

		m_control.SetAt(han, tmps);

		value = GetPrivateProfileSection(tmps, wb, sizeof(wb), path);
	}
}
*/
void CWriteXml::SetTrlayoutxml(int kind, CString data)
{
	CString key, redata, tmp;

	tmp = data;
	key = Parser(tmp, _T("\t"));

	if (kind == 0)
		return;

	if (m_trlayoutxml.Lookup(key, redata))
		m_trlayoutxml.RemoveKey(key);
	if (!tmp.IsEmpty())
		m_trlayoutxml.SetAt(key, data);
}

CString CWriteXml::GetTrLayOut()
{
	CString tmp = _T("");
	CString sKey, str;

	for (POSITION pos = m_trlayoutxml.GetStartPosition(); pos != NULL; )
	{
		m_trlayoutxml.GetNextAssoc(pos, sKey, str);
		tmp += str;
		tmp += _T("^");
	}

	return tmp; 
}

CString CWriteXml::RemoveComment(CString strScript)
{
	int pos, pos2;
	CString strTmp, removeStr;
	strTmp = removeStr = strScript;

	for (int ii = 0; ; ii++)
	{
		pos = strTmp.Find(_T("--"));
		if (pos < 0)
			break;
		else
		{
			pos2 = strTmp.Find(_T("\n"), pos);
			strTmp = strTmp.Left(pos);
			if (strTmp.Mid(pos-1, pos) != _T("\n"))
				strTmp += _T("\n");
			if (pos2 > 0)
				strTmp += removeStr.Mid(pos2 + 1);
			removeStr = strTmp;
		}
	}
	return removeStr;
}

CString CWriteXml::AnsiToUTF8(CString InputStr)
{
	WCHAR szUnicode[1024*64];
	char szUTF8char[1024*64];

	CString strConver;
	char* szSrc = (LPSTR)(LPCTSTR)InputStr;

	char szRetVale[1024*64] = "";

	// EUC-KR -> Unicode
	int UnicodeSize = MultiByteToWideChar(CP_ACP, 0, szSrc, (int)strlen(szSrc), szUnicode, sizeof(szUnicode));
	// Unicode -> UTF-8
	int UTF8CodeSize = WideCharToMultiByte(CP_UTF8, 0, szUnicode, UnicodeSize, szUTF8char, sizeof(szUTF8char), NULL, NULL);

	memcpy(szRetVale, szUTF8char, UTF8CodeSize);
	strConver = szRetVale;

	return strConver;
}
