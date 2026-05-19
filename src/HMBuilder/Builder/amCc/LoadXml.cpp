// LoadXml.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "comutil.h"
#include "amCc.h"
#include "LoadXml.h"
#include "maptbl.hxx"
#include "afxcoll.h"

#include "../amCom/CellItem.h"
// CLoadXml

CLoadXml::CLoadXml()
{
//	m_pReadDoc = NULL;
	m_pFormItem = NULL;
	m_cMapHeader.RemoveAll();
	m_cControlForm.RemoveAll();
	m_cScript.RemoveAll();
	m_groupArr.RemoveAll();
	m_fN = -1;
	m_readKind = NONE;
	m_ParentNodename = NULL;	//BSTR(_T(""));
	m_bfirst = false;
	m_colcnt = -1;
	m_cellItem = NULL;
	m_multi = 0;

	m_control = NULL;
}

CLoadXml::~CLoadXml()
{
	if (m_pFormItem)
		delete m_pFormItem;
}

void CLoadXml::RemoveCMap()
{
	m_cMapHeader.RemoveAll();
	m_cControlForm.RemoveAll();
	m_cScript.RemoveAll();
	m_groupArr.RemoveAll();
}

void CLoadXml::Load(struct _mapH* mapH, CString curdir)
{
	m_mapH = mapH;

	if (m_pFormItem)
	{
		delete m_pFormItem;
	}

	m_pFormItem = new CFormItem(mapH);
	m_rtype  = 0;
	m_groupArr.RemoveAll();

	CString tmp;
	for (int ii = 0; MapHeader[ii] != NULL; ii++)
	{
		tmp.Format(_T("%d"), ii);
		m_cMapHeader.SetAt(MapHeader[ii], tmp);
	}

	for (int ii = 0; Control[ii] != NULL; ii++)
	{
		tmp.Format(_T("%d"), ii);
		m_cControlForm.SetAt(Control[ii], tmp);
	}

	for (int ii = 0; Script[ii] != NULL; ii++)
	{
		tmp.Format(_T("%d"), ii);
		m_cScript.SetAt(Script[ii], tmp);
	}

	LoadXml(curdir);
	SetTrRtsInfo();

	struct	_formR*	formR;
	for (int ii = 0; ii < m_pFormItem->GetCount(); ii++)
	{
		formR = m_pFormItem->GetForm(ii);
		if (formR->kind == FM_CONTROL)
		{
			if (formR->str == NULL || strlen(formR->str) == 0)
			{
				if (m_control && formR->dat != NULL && strlen(formR->dat) > 0)
				{
					CString	desc, name, ctrl = formR->dat;
					for (POSITION pos = m_control->GetStartPosition(); pos; )
					{
						m_control->GetNextAssoc(pos, name, desc);
						if (!desc.CompareNoCase(ctrl))
						{
							m_pFormItem->SetFormStr(ii, _T("str"), name);
							break;
						}
					}
				}
			}
		}
	}
}

void CLoadXml::LoadXml(CString sPath)
{
	HRESULT			hr;
	VARIANT_BOOL		bOLoad = 0;
	IXMLDOMDocument*	pDoc = NULL;
	IXMLDOMElement*		pRootElement = NULL;
	IXMLDOMParseError*	pDocError = NULL;

	if (sPath.IsEmpty())
		return;

	CoInitialize(NULL);
	CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument2, (void**)&pDoc);

	hr = pDoc->load(CComVariant((LPCTSTR)sPath), &bOLoad);
	pDoc->put_async(VARIANT_FALSE);

	if (hr != S_OK)	
	{
		pDoc->Release();
		pDoc = NULL;
		return;
	}

	if (bOLoad)
	{
		HRESULT		hr;
		IXMLDOMNode*	pNode = NULL;
		BSTR		bstr, nodename;
		CString		strtmp;
		VARIANT		mapname;
		m_fN = -1;

		/*
		if (m_pReadDoc)
		{
			m_pReadDoc->Release();
			m_pReadDoc = NULL;
		}
		*/

		//m_pReadDoc = pDoc;

		hr = pDoc->get_documentElement(&pRootElement);

		if (hr != S_OK || pRootElement == NULL)
		{
			pDoc->Release();
			pDoc = NULL;
			return;
		}

		// FORM Attribute Read
		hr = pRootElement->get_nodeName(&nodename);
		if (hr != S_OK)	
		{
			pDoc->Release();
			pDoc = NULL;
			return;
		}
		strtmp = CString(nodename);

		hr = pRootElement->getAttribute(_bstr_t(_T("NAME")), &mapname);
		if (hr != S_OK)
		{
			pDoc->Release();
			pDoc = NULL;
			return;
		}

		strtmp = CString(mapname);
		// FORM
		hr = pRootElement->get_firstChild(&pNode);
		if (hr != S_OK)
		{
			pDoc->Release();
			pDoc = NULL;
			return;
		}

		((CamCcApp *)AfxGetApp())->m_trlayoutxml.RemoveAll();
		((CamCcApp *)AfxGetApp())->m_rtssymbolxml.RemoveAll();
		((CamCcApp *)AfxGetApp())->removeAllCMap();

		while (pNode)
		{
			GetXmlAttribute(pNode);
			hr = pNode->get_nodeName(&nodename);

			if (!CString(nodename).CompareNoCase(_T("GLOBAL")))
				m_readKind = GLOBAL;
			else if (!CString(nodename).CompareNoCase(_T("CONTROL")))
				m_readKind = CONTROL;
			else if (!CString(nodename).CompareNoCase(_T("TRLAYOUT")))
				m_readKind = TRLAYOUT;
			else if (!CString(nodename).CompareNoCase(_T("RTSSYMBOL")))
				m_readKind = RTSSYMBOL;
			else if (!CString(nodename).CompareNoCase(_T("LAYOUT")))
				m_readKind = LAYOUT;

			if (hr == S_OK)
			{
				XmlParser(pNode);
				hr = pNode->get_nextSibling(&pNode);
				if (hr != S_OK)	
					break;
			}
		}

		hr = pDoc->get_xml(&bstr);
		if (hr != S_OK)
		{
			pDoc->Release();
			pDoc = NULL;
			return;
		}

		removeIXMLDOMNode(pNode);
		removeIXMLDOMElement(pRootElement);

		pDoc->Release();
		pDoc = NULL;

		SysFreeString(bstr);
		SysFreeString(nodename);
	}
	else
	{
		CString	msg;
		long	line, linePos;
		BSTR	reason = NULL;

		pDoc->get_parseError(&pDocError);
		pDocError->get_errorCode(&hr);
		pDocError->get_line(&line);
		pDocError->get_linepos(&linePos);
		pDocError->get_reason(&reason);

		msg.Format(_T("Error 0x%.8X on line %d, position %d\r\nReason: %s"), hr, line, linePos, CString(reason));

		pDoc->Release();
		pDoc = NULL;

		pDocError->Release();
		pDocError = NULL;
		SysFreeString(reason);
	}

	CoUninitialize();
}

int CLoadXml::formcount()
{
	return m_fN;
}

void CLoadXml::XmlParser(IXMLDOMNode* pNode)
{
	IXMLDOMNode*	pNodes = NULL;
	IXMLDOMNode*	pChildNodes = NULL;
	IXMLDOMNode*	pParentNode = NULL;
	HRESULT		hr;
	BSTR		node, nodename, text;
	int		formN = 0;
	DOMNodeType	type;

	m_multi		= 0;

	if (pNode != NULL) 
	{	
		hr = pNode->get_firstChild(&pNodes);
		pNode->get_nodeName(&node);
		if (hr != S_OK) return;

		while (pNode)
		{
			GetXmlAttribute(pNode);

			hr = pNodes->get_firstChild(&pChildNodes);
			pNodes->get_nodeName(&nodename);

			if (CString(nodename).Find(_T("-SYMBOL_")) >= 0)
			{
				hr = pNodes->get_parentNode(&pParentNode);

				if (hr == S_OK)
					pParentNode->get_nodeName(&m_ParentNodename);

				m_fN++;

				if (m_fN >= m_pFormItem->GetCount())
				{
					m_bfirst = true;
					m_bgridtable = false;
					m_colcnt = -1;
					m_pFormItem->AddForm();
				}
			}

			if (hr != S_OK)
			{
//TRACE(_T("----------upvalue---------[%15s][%15s][%20s]\n"), CString(m_ParentNodename), CString(node), CString(nodename));
				SetFormData(CString(m_ParentNodename), CString(node), CString(nodename), _T(""));
				hr = pNodes->get_nextSibling(&pNodes);
				if (hr == S_OK)
					continue;
			}

			if (hr == S_OK)
			{	
				pChildNodes->get_nodeType(&type);

				switch (type)
				{
				case NODE_ELEMENT:
					XmlParser(pNodes);
					break;
				case NODE_TEXT:
					pChildNodes->get_text(&text);
					SetFormData(CString(m_ParentNodename), CString(node), CString(nodename), CString(text));						
//TRACE(_T("----------dnvalue---------[%15s][%15s][%20s][%10s]\n"), CString(m_ParentNodename), CString(node), CString(nodename), CString(text));
					SysFreeString(text);
					break;
				}

				SysFreeString(nodename);
				hr = pNodes->get_nextSibling(&pNodes);
			}

			if (hr != S_OK)
			{
				SysFreeString(node);
				return;
			}
		}

		SysFreeString(node);
		removeIXMLDOMNode(pNodes);
		removeIXMLDOMNode(pChildNodes);
		removeIXMLDOMNode(pParentNode);
	}
}

bool CLoadXml::GetXmlAttribute(IXMLDOMNode* pNode)
{
	HRESULT			hr;
	long			nLen;
	BSTR			name;
	VARIANT			value;
	CString			stmp;
	bool			bret = false;
	IXMLDOMNamedNodeMap*	pNodemap = NULL;
	IXMLDOMNode*		pNodes = NULL;

	if (pNode != NULL) 
	{
		hr = pNode->get_attributes(&pNodemap);
		if (hr != S_OK)	return bret;

		hr = pNodemap->get_length(&nLen);
		if (hr != S_OK)	return bret;

		if (nLen > 0)
			bret = true;

		for (long ii = 0; ii < nLen; ii++)
		{
			pNodemap->get_item(ii, &pNodes);
			pNodes->get_nodeName(&name);
			pNodes->get_nodeValue(&value);

//TRACE(_T("--------Attribute-------[%15s][%20s][%10s]\n"), _T(""), CString(name), CString(stmp));
			SysFreeString(name);
		}
	}

	removeIXMLDOMNode(pNodes);
	removeIXMLDOMNamedNodeMap(pNodemap);

	return bret;
}

void CLoadXml::SetFormData(CString parentnode, CString node, CString sKey, CString dat)
{
	CString tmp;
	bool	bscript = false;
	
	if (m_pFormItem->GetCount() > 0 || m_readKind == GLOBAL)
	{
		if (m_cScript.GetSize() > 0)
		{
			m_cScript.Lookup(sKey, tmp);
			if (!tmp.IsEmpty())
			{
				bscript = true;;
				m_bgridtable = false;
			}
		}
	}

	if (!node.CompareNoCase(_T("MULTICOL")))
	{
		if (!sKey.CompareNoCase(_T("COLINFO")))
		{
			m_bgridtable = true;
			m_bfirst = true;
			m_colcnt++;
		}
	}

	switch (m_readKind)
	{
	case GLOBAL:
		if (bscript)
			SetScript(sKey, dat);		// Script Load
		else
			SetGlobal(sKey, dat);		// Map Load
		break;
	case CONTROL:						
		if (bscript)
			SetScript(sKey, dat, m_fN);
		else if (m_bgridtable)
			SetMulticolControl(parentnode, node, sKey, dat, m_fN);	// Grid / GridEX / Table Load
		else
			SetControl(parentnode, node, sKey, dat, m_fN);	// Control Load
		break;
	case TRLAYOUT:
		setTrLayout(sKey, dat);
		break;
	case RTSSYMBOL:
		setRtsSymbol(sKey, dat);
		break;
	case LAYOUT:
		setLayout(sKey, dat);
		break;
	case NONE:
	default:
		break;
	}
}

void CLoadXml::setTrLayout(CString sKey, CString dat)
{
	CString tmp;
	tmp.Format(_T("%s\t%s"), sKey, dat);
	((CamCcApp *)AfxGetApp())->setTrLayOut(1, tmp);
	//((CamCcApp *)AfxGetApp())->m_trlayoutxml.SetAt(sKey, tmp);
}

void CLoadXml::setRtsSymbol(CString sKey, CString dat)
{
	CString tmp;
	tmp.Format(_T("%s\t%s"), sKey, dat);
//TRACE("-----------readTrlayout55555--------------%s\n", tmp);
	((CamCcApp *)AfxGetApp())->setRtsSymbol(1, tmp);
	//((CamCcApp *)AfxGetApp())->m_rtssymbolxml.SetAt(sKey, tmp);
}

//LAYOUT0x0cCURRLAYOUT0x0cCODE,0,0,80,40,TRUE0x0cDATE,0,80,80,40,TRUE0x0cNAME,0,160,80,40,FALSE0x08
void CLoadXml::setLayout(CString sKey, CString dat)
{
	struct _formR* formR;
	struct _Layout* layout;
	CString tmp, sIndex, sName, sData, sInfo, sForm;

	CMapStringToString* smap = new CMapStringToString;
	CMapStringToPtr map;

		
	sName = dat.Left(dat.Find("::"));
	if (!sName.CompareNoCase("LAYOUT"))
		Parser(dat, "::");

	// original Setting
	smap->SetAt("-1", "Original");
	for (int ii = 0; ii < m_pFormItem->GetCount(); ii++)
	{
		layout = new _Layout;
		formR = m_pFormItem->GetForm(ii);

		layout->left = formR->left;
		layout->right = formR->right;
		layout->top = formR->top;
		layout->bottom = formR->bottom;
		layout->properties = PR_VISIBLE;
		layout->bfirst = false;
				
		formR->layout->SetAt("Original", (void*&)layout);		
	}

	for (;;)
	{	
		sData = Parser(dat, ":.");

		if (sData.IsEmpty())
			break;

		TRACE("--------------------------------------\n");
		sIndex = Parser(sData, "::");	// Layout Index
		TRACE("Index = %s\n", sIndex);
		sName = Parser(sData, "::");	// Layout Name
		TRACE("Layout Name = %s\n", sName);

		smap->SetAt(sIndex, sName);
		for (;;)
		{
			tmp = Parser(sData, "::");
			if (tmp.IsEmpty())
				break;

			layout = new _Layout;			
			sForm = Parser(tmp, ",");	// Form Name
			TRACE("Form Name = %s\n", sForm);
			sInfo = Parser(tmp, ",");	// Left
			layout->left = atoi(sInfo);
			TRACE("Left = %s\n", sInfo);
			sInfo = Parser(tmp, ",");	// Top
			layout->top = atoi(sInfo);
			TRACE("Top = %s\n", sInfo);
			sInfo = Parser(tmp, ",");	// Width
			layout->right = layout->left + atoi(sInfo);
			TRACE("Width = %s\n", sInfo);
			sInfo = Parser(tmp, ",");	// Height
			layout->bottom = layout->top + atoi(sInfo);			
			TRACE("Height = %s\n", sInfo);
			sInfo = Parser(tmp, ",");	// Visible
			layout->properties = atoi(sInfo);
			TRACE("Visible = %s\n", sInfo);
			layout->bfirst = false;

			// save
			map.SetAt(sForm, (void*&)layout);			
		}

		for (int ii = 0; ii < m_pFormItem->GetCount(); ii++)
		{
			layout = NULL;
			formR = m_pFormItem->GetForm(ii);
			map.Lookup(formR->name, (void*&)layout);
			formR->layout->SetAt(sName, (void*&)layout);
		}
	}
	m_pFormItem->SetCurrLayoutName("Original");
	m_pFormItem->AddLayout(smap);	
}

void CLoadXml::SetGlobal(CString sKey, CString dat)
{
	int	ii, jj, pos;
	CString tmp, tmps;

	if (m_cMapHeader.GetSize() > 0)
	{
		m_cMapHeader.Lookup(sKey, tmp);
		if (dat.IsEmpty() || tmp.IsEmpty())
			return;

		switch (atoi(tmp))
		{
		case 0:
			CopyMemory(m_mapH->mapN, dat, dat.GetLength());
			break;
		case 1:
			m_mapH->height =  atoi(dat);
			break;
		case 2:
			m_mapH->width = atoi(dat);
			break;
		case 3:
			for (ii = 0; ii < MAPKIND; ii++)
			{
				if (!dat.CompareNoCase(mapKind[ii].strName))
					break;
			}
			if (ii >= MAPKIND)
				break;
			m_mapH->mapK = mapKind[ii].defValue;
			break;
		case 4:
			CopyMemory(m_mapH->mapdesc, dat, dat.GetLength());
			if (dat.GetLength() > 19)
				m_mapH->mapdesc[20] = NULL;
			break;
		case 5:
			CopyMemory(m_mapH->fName, dat,dat.GetLength());
			break;
		case 6:
			m_mapH->fPoint = (BYTE)(atoi(dat));
			break;
		case 7:
			for (ii = 0; ii < FSTYLE; ii++)
			{
				if (!dat.CompareNoCase(fStyle[ii].strName))
					break;
			}
			if (ii >= FSTYLE)
				break;
			m_mapH->fStyle = BYTE(fStyle[ii].defValue);
			break;
		case 8:
			m_mapH->options = 0;
			while (dat)
			{
				pos = dat.Find(_T(","));
				if (pos >= 0)
				{
					tmps = dat.Left(pos);
					dat  = dat.Mid(pos + 1);

					for (jj = 0; jj < OPTIONS; jj++)
					{
						if (!tmps.CompareNoCase(options[jj].strName))
							break;
					}
					if (jj >= OPTIONS)
						continue;

					m_mapH->options |= options[jj].defValue;
				}
				else
				{
					tmps = dat;
					for (jj = 0; jj < OPTIONS; jj++)
					{
						if (!tmps.CompareNoCase(options[jj].strName))
							m_mapH->options |= options[jj].defValue;
					}
					dat = _T("");
					break;
				}
			}
			break;
		case 9:
			CopyMemory(m_mapH->caption, dat, dat.GetLength());
			break;
		case 10:
			for (ii = 0; ii < TOPATH; ii++)
			{
				if (!dat.CompareNoCase(toPath[ii].strName))
					break;
			}
			if (ii >= TOPATH)
				break;
			m_mapH->toPath = toPath[ii].defValue;
			break;
		case 11:
			for (ii = 0; ii < MAPMETHOD; ii++)
			{
				if (!dat.CompareNoCase(mapMethod[ii].strName))
					break;
			}
			if (ii >= MAPMETHOD)
				break;
			m_mapH->mapM = mapMethod[ii].defValue;
			break;
		case 12:
			CopyMemory(m_mapH->trxC, dat, dat.GetLength());
			break;
		case 13:
			m_mapH->repeatV = atoi(dat);
			break;
		case 14:
			m_mapH->flashClr = atoi(dat);
			break;
		case 15:
			CopyMemory(m_mapH->flashSym, dat, dat.GetLength());
			break;
		case 16:
			m_mapH->upClr = atoi(dat);
			break;
		case 17:
			m_mapH->dnClr = atoi(dat);
			break;
		case 18:
			CopyMemory(m_mapH->contrastN, dat, dat.GetLength());
			break;
		case 19:
			m_mapH->bgK = 1;//atoi(dat); ljh
			break;
		case 20:
			CopyMemory(m_mapH->bgFileN, dat, dat.GetLength());
			break;
		case 21:
			m_mapH->bgClr = atoi(dat);
			break;
		case 22:
			m_mapH->formtype = atoi(dat);
			break;
		case 23:
			CopyMemory(m_mapH->onLoadN, dat, dat.GetLength());
			break;
		case 24:
			CopyMemory(m_mapH->onSendN, dat, dat.GetLength());
			break;
		case 25:
			CopyMemory(m_mapH->onReceiveN, dat, dat.GetLength());
			break;
		case 26:
			CopyMemory(m_mapH->onAlertN, dat, dat.GetLength());
			break;
		case 27:
			CopyMemory(m_mapH->onTimerN, dat, dat.GetLength());
			break;
		case 28:
			CopyMemory(m_mapH->onCloseN, dat, dat.GetLength());
			break;
		case 29:
			CopyMemory(m_mapH->onDeviceN, dat, dat.GetLength());
			break;
		case 30:
			CopyMemory(m_mapH->onErrorN, dat, dat.GetLength());
			break;
		case 31:
			CopyMemory(m_mapH->onFlickingN, dat, dat.GetLength());
			break;
		case 32:
			CopyMemory(m_mapH->onChangeLayoutN, dat, dat.GetLength());
			break;
		case 33:
			CopyMemory(m_mapH->onDeclarationN, dat, dat.GetLength());
			break;
		case 34:
			CopyMemory(m_mapH->onInDeclarationN, dat, dat.GetLength());
			break;
		case 35:
			m_mapH->alpha = atoi(dat);
			break;
		case 36:
			CopyMemory(m_mapH->formCtrl, dat, dat.GetLength());
			break;

		}
	}
}

CString CLoadXml::FmKind(CString tmp)
{
	int pos = -1;
	CString kind = _T("");

	if (!tmp.IsEmpty())
	{
		pos = tmp.Find(_T("-"));
		kind = tmp.Left(pos);
	}

	return kind;
}

void CLoadXml::SetControl(CString parentnode, CString node, CString sKey, CString dat, int& fN)
{
	int	ii = 0;
	int	pos = -1;
	CString tmp, tmps;
	CRect	tRc;
	
	if (m_bfirst)
	{
		for (ii = 0; ii < FORMTBL; ii++)
		{
			if (!FmKind(node).CompareNoCase(formTbl[ii].strName))
				break;
		}

		m_pFormItem->GetForm(fN)->kind = formTbl[ii].kind;

		if (m_pFormItem->GetForm(fN)->kind == FM_IMAGEVIEW) 
			m_pFormItem->GetForm(fN)->alignment = AL_CENTER;
	}

	m_bfirst = false;

	if (m_cControlForm.GetSize() > 0)
	{
		m_cControlForm.Lookup(sKey, tmp);

		if (dat.IsEmpty() || tmp.IsEmpty())
			return;

		switch (atoi(tmp))
		{
		case 0:				// SYMBOLNAME
			m_pFormItem->SetFormStr(fN, _T("name"), dat);
			break;
		case 1:				// LEFT
			m_pFormItem->GetForm(fN)->left = atoi(dat);
			break;
		case 2:				// TOP
			m_pFormItem->GetForm(fN)->top = atoi(dat);
			break;
		case 3:				// RIGHT
			m_pFormItem->GetForm(fN)->right = atoi(dat);
			break;
		case 4:				// BOTTOM
			m_pFormItem->GetForm(fN)->bottom = atoi(dat);
			break;
		case 5:				// RADIOKIND
			break;
		case 6:				// BORDERS
			for (ii = 0; ii < PANELTBL; ii++)
			{
				if (!dat.CompareNoCase(panelTbl[ii].strName))
				{
					m_pFormItem->GetForm(fN)->borders = panelTbl[ii].borders;
					break;
				}
			}
			break;
		case 7:				// STR
			tmp = _T("");
			if (m_pFormItem->GetForm(fN)->kind == FM_COMBO || m_pFormItem->GetForm(fN)->kind == FM_TAB)
			{
				for (ii = 0; ii < LOWORD(m_pFormItem->GetForm(fN)->keys); ii++)
				{
					pos = dat.Find(",");
					if (pos >= 0)
					{
						tmps = dat.Left(pos);
						dat  = dat.Mid(pos + 1);
					}
					else
						tmps = dat;
					tmp += CString(tmps, tmps.GetLength());
					tmp += '\t';
				}
				m_pFormItem->SetFormStr(fN, _T("str"), tmp);
			}
			else
				m_pFormItem->SetFormStr(fN, _T("str"), dat);
			break;
		case 8:				// STR2
			m_pFormItem->SetFormStr(fN, _T("str2"), dat);
			break;
		case 9:				// DAT
			tmp = _T("");
			if (m_pFormItem->GetForm(fN)->kind == FM_COMBO || m_pFormItem->GetForm(fN)->kind == FM_TAB)
			{
				for (ii = 0; ii < LOWORD(m_pFormItem->GetForm(fN)->keys); ii++)
				{
					pos = dat.Find(",");
					if (pos >= 0)
					{
						tmps = dat.Left(pos);
						dat  = dat.Mid(pos + 1);
					}
					else
						tmps = dat;

					tmp += CString(tmps, tmps.GetLength());
					tmp += '\t';
				}
				m_pFormItem->SetFormStr(fN, _T("dat"), tmp);
			}
			else
				m_pFormItem->SetFormStr(fN, _T("dat"), dat);
			break;
		case 10:			// FLOAT
			m_pFormItem->GetForm(fN)->offs = (BYTE)atoi(dat);
			break;
		case 11:			// DESC
			m_pFormItem->SetFormStr(fN, _T("desc"), dat);
			break;
		case 12:			// EDITFORMAT : editS
			m_pFormItem->SetFormStr(fN, _T("editS"), dat);
			break;
		case 13:			// IFRGB : onEdit
			{
				WORD	upRGB,  dnRGB;
				upRGB = dnRGB = 0;
				int	pos = -1;

				pos = dat.Find(_T(","));
				if (pos >= 0)
				{
					upRGB = ARGBtoRGB(atoi(dat.Left(pos)));
					dnRGB = ARGBtoRGB(atoi(dat.Mid(pos + 1)));
				}

				m_pFormItem->GetForm(fN)->attr |= FA_CORGB;
				m_pFormItem->GetForm(fN)->onEdit = MAKELONG(dnRGB, upRGB);
			}
			break;
		case 14:			// IFRGB2
			break;
		case 15:			// AUXS
			m_pFormItem->SetFormStr(fN, _T("auxS"), dat);
			break;
		case 16:			// IMAGETYPE
			for (ii = 0; ii < PROPERTTBL; ii++)
			{
				if (!dat.CompareNoCase(propertiesTbl[ii].strName))
				{
					m_pFormItem->GetForm(fN)->properties |= propertiesTbl[ii].properties;
					break;
				}
			}
			break;
		case 17:			// EIOIOK
			for (ii = 0; ii < EMODETBL; ii++)
			{
				if (!dat.CompareNoCase(emodeTbl[ii].strName))
				{
					m_pFormItem->GetForm(fN)->iok = emodeTbl[ii].iok;
					break;
				}
			}
			break;
		case 18:			// ALIGNMENT
			if (m_pFormItem->GetForm(fN)->kind == FM_IMAGEVIEW) 
			{
				m_pFormItem->GetForm(fN)->alignment = AL_CENTER;
				break;
			}

			for (ii = 0; ii < ALIGNTBL; ii++)
			{
				if (!dat.CompareNoCase(alignTbl[ii].strName))
				{
					m_pFormItem->GetForm(fN)->alignment = alignTbl[ii].alignment;
					break;
				}

			}
			break;
		case 19:			// ALIGNMENTIMAGE
			for (ii = 0; ii < ALIGNTBLIMAGE; ii++)
			{
				if (!dat.CompareNoCase(alignTblImage[ii].strName))
				{
					m_pFormItem->GetForm(fN)->alignImage = alignTblImage[ii].alignment;
					break;
				}
			}
			break;
		case 20:			// FONTNAME
			m_pFormItem->SetFormStr(fN, _T("fName"), dat);
			break;
		case 21:			// FONTPOINT
			m_pFormItem->GetForm(fN)->fPoint = atoi(dat);
			break;
		case 22:			// FONTSTYLE
			for (ii = 0; ii < FSTYLE; ii++)
			{
				if (!dat.CompareNoCase(fStyle[ii].strName))
				{
					m_pFormItem->GetForm(fN)->fStyle = fStyle[ii].defValue;
					break;
				}
			}
			break;
		case 23:			// LEN
			m_pFormItem->GetForm(fN)->size = (WORD)atoi(dat);
			break;
		case 24:			// COUNT
			{
				WORD	count, nvisible;
				int	pos = -1;

				pos = dat.Find(_T(","));
				if (pos < 0)
					break;

				count = atoi(dat.Left(pos));
				nvisible = atoi(dat.Mid(pos + 1));
				m_pFormItem->GetForm(fN)->keys = MAKELONG(count, nvisible);
			}
			break;
		case 25:			// VCOUNT	//
			break;
		case 26:			// INDEX
			//m_pFormItem->GetForm(fN)->index = atoi(dat);
			m_pFormItem->GetForm(fN)->checked = atoi(dat);
			break;
		case 27:			// MATCHKEY
			for (ii = 0; ii < VKTBL; ii++)
			{
				if (!dat.CompareNoCase(vkTbl[ii].strName))
				{
					m_pFormItem->GetForm(fN)->onEdit = ii;
					break;
				}
			}
			break;
		case 28:			// CHARS
			m_pFormItem->GetForm(fN)->keys = atoi(dat);
			break;
		case 29:			// DATATYPE
			if (m_pFormItem->GetForm(fN)->kind == FM_TAB)
				m_pFormItem->GetForm(fN)->type = atoi(dat);
			else
			{
				for (ii = 0; ii < IOTBL; ii++)
				{
					if (!dat.CompareNoCase(ioTbl[ii].strName))
					{
						m_pFormItem->GetForm(fN)->type = ioTbl[ii].type;
						break;
					}
				}
			}
			break;
		case 30:			// ATTRIBUTE
			while (dat)
			{
				pos = dat.Find(_T(","));
				if (pos >= 0)
				{
					tmps = dat.Left(pos);
					dat  = dat.Mid(pos + 1);

					for (ii = 0; ii < ATTRTBL; ii++)
					{
						if (!tmps.CompareNoCase(attrTbl[ii].strName))
							m_pFormItem->GetForm(fN)->attr |= attrTbl[ii].attr;
					}
				}
				else
				{
					tmps = dat;
					for (ii = 0; ii < ATTRTBL; ii++)
					{
						if (!tmps.CompareNoCase(attrTbl[ii].strName))
							m_pFormItem->GetForm(fN)->attr |= attrTbl[ii].attr;
					}
					dat = _T("");
					break;
				}
			}
			break;
		case 31:			// SUBATTRIBUTE
			break;
		case 32:			// PRGB
			m_pFormItem->GetForm(fN)->pRGB = ARGBtoRGB(atoi(dat));
			break;
		case 33:			// TRGB
			m_pFormItem->GetForm(fN)->tRGB = ARGBtoRGB(atoi(dat));
			break;
		case 34:			// SPRGB
			m_pFormItem->GetForm(fN)->spRGB = ARGBtoRGB(atoi(dat));
			break;
		case 35:			// TABORDER
			m_pFormItem->GetForm(fN)->index = atoi(dat);
			break;
		case 36:			// STRGB
			m_pFormItem->GetForm(fN)->stRGB = ARGBtoRGB(atoi(dat));
			break;
		case 37:			// IOORDER
			m_pFormItem->GetForm(fN)->iorder = atoi(dat);
			break;
		case 38:			// GROUPN
			m_groupArr.SetAt(fN, atoi(dat));
			break;
		case 39:			// RESIZABLE
			m_pFormItem->GetForm(fN)->attr2 |= GO_FIX;
			break;
		case 40:			// ENABLE
			m_pFormItem->GetForm(fN)->properties |= PR_ENABLE;
			break;
		case 41:			// VISIBLE
			m_pFormItem->GetForm(fN)->properties |= PR_VISIBLE;
			break;
		case 42:			// CHECKED
			m_pFormItem->GetForm(fN)->properties |= PR_CHECKED;
			break;
		case 43:			// IMAGE
			m_pFormItem->GetForm(fN)->properties |= PR_IMAGE;
			break;
		case 44:			// IMGTEXT
			m_pFormItem->GetForm(fN)->properties |= PR_IMAGETEXT;
			break;
		case 45:			// HOVER
			m_pFormItem->GetForm(fN)->properties |= PR_HOVER;
			break;
		case 46:			// SORT
			m_pFormItem->GetForm(fN)->properties |= PR_SORT;
			break;
		case 47:			// SHOWFORM
			m_pFormItem->GetForm(fN)->properties |= PR_EDIT;
			break;
		case 48:			// ONTAB
			m_pFormItem->GetForm(fN)->onTap = atoi(dat);
			break;
		case 49:			// ONTABL
			m_pFormItem->GetForm(fN)->onTapL = atoi(dat);
			break;
		case 50:			// ONLONGTAB
			m_pFormItem->GetForm(fN)->onLongTap = atoi(dat);
			break;
		case 51:			// ONLONGTABL
			m_pFormItem->GetForm(fN)->onLongTapL = atoi(dat);
			break;
		case 52:			// ONCHANGE
			m_pFormItem->GetForm(fN)->onChange = atoi(dat);
			break;
		case 53:			// ONCHANGEL
			m_pFormItem->GetForm(fN)->onChangeL = atoi(dat);
			break;
		case 54:			// ONCHAR
			m_pFormItem->GetForm(fN)->onChar = atoi(dat);
			break;
		case 55:			// ONCHARL
			m_pFormItem->GetForm(fN)->onCharL = atoi(dat);
			break;
		case 56:			// BLOCK
			m_pFormItem->GetForm(fN)->block = atoi(dat) ? true : false;
			break;
		case 57:			// DRAWN
			m_pFormItem->GetForm(fN)->drawn = atoi(dat) ? true : false;
			break;
		case 58:			// BCOMMON
			m_pFormItem->GetForm(fN)->bCommon = atoi(dat) ? true : false;
			break;
		case 59:			// SCROLLPOS
			m_pFormItem->GetForm(fN)->scrollPos = atoi(dat);
			break;
		case 60:			// VISIBLEROW	*/
			m_pFormItem->GetForm(fN)->keys = MAKELONG(LOWORD(m_pFormItem->GetForm(fN)->keys), atoi(dat));
			break;
		case 61:			// CELLH
			m_pFormItem->GetForm(fN)->keys = MAKELONG(atoi(dat), HIWORD(m_pFormItem->GetForm(fN)->keys));
			break;
		case 62:			// MATRIX
			{
				int	pos = -1;

				pos = dat.Find(_T(","));
				if (pos >= 0)
				{
					m_row = atoi(dat.Left(pos));
					m_col = atoi(dat.Mid(pos + 1));
				}
				
				newCellitem();

				if (m_pFormItem->GetForm(fN)->kind == FM_GRID || m_pFormItem->GetForm(fN)->kind == FM_GRIDEX)
					for (int jj = 0; jj < m_col; jj++)
						m_cellItem->AddCell();
				else if (m_pFormItem->GetForm(fN)->kind == FM_TABLE)
					for (int jj = 0; jj < m_row; jj++)
						m_cellItem->AddCell();
			}
			break;
		case 63:			// FIX
			m_pFormItem->GetForm(fN)->attr2 |= GO_MULTI;
			break;
		case 64:			// MAINCOLOR
			{
				int	prgb, trgb;
				prgb = trgb = 0;
				int	pos = -1;

				pos = dat.Find(_T(","));
				if (pos >= 0)
				{
					prgb = atoi(dat.Left(pos));
					trgb = atoi(dat.Mid(pos + 1));
				}

				m_pFormItem->GetForm(fN)->pRGB = ARGBtoRGB(prgb);
				m_pFormItem->GetForm(fN)->tRGB = ARGBtoRGB(trgb);
			}
			break;
		case 65:			// LINECOLOR
			m_pFormItem->GetForm(fN)->bRGB = ARGBtoRGB(atoi(dat));
			break;
		case 66:			// FOCUSCOLOR
			m_pFormItem->GetForm(fN)->onEdit = atoi(dat);
			break;
		case 67:			// FOCUSKIND
			m_pFormItem->GetForm(fN)->isEdit = atoi(dat);
			break;
		case 68:			// AUTOSIZE
			m_pFormItem->GetForm(fN)->attr2 |= GO_AUTOSIZE;
			break;
		case 69:			// GRIDOPTION
			while (dat)
			{
				pos = dat.Find(_T(","));
				if (pos >= 0)
				{
					tmps = dat.Left(pos);
					dat  = dat.Mid(pos + 1);

					for (ii = 0; ii < GRIDOPTION; ii++)
					{
						if (!tmps.CompareNoCase(gridoptiontbl[ii].strName))
							m_pFormItem->GetForm(fN)->attr2 |= gridoptiontbl[ii].attr2;
					}
				}
				else
				{
					tmps = dat;
					for (ii = 0; ii < GRIDOPTION; ii++)
					{
						if (!tmps.CompareNoCase(gridoptiontbl[ii].strName))
							m_pFormItem->GetForm(fN)->attr2 |= gridoptiontbl[ii].attr2;
					}
					dat = _T("");
					break;
				}
			}
			break;
		case 70:			// COLS		*/
			break;
		case 71:			// BRGB		*/
			break;
		case 72:			// COLINFO
			break;
		case 73:			// CELLLEN
			break;
		case 74:			// WIDTH
			break;
		case 75:			// HEAD
			break;
		case 76:			// COLALIGNH
			break;
		case 77:			// COLALIGNC
			break;
		case 78:			// HEADCOLOR
			break;
		case 79:			// HPRGB grid 1st row
			m_pFormItem->GetForm(fN)->spRGB = atoi(dat);
			break;
		case 80:			// HTRGB grid 2st row
			m_pFormItem->GetForm(fN)->stRGB = atoi(dat);
			break;
		case 81:			// DOM
			break;
		case 82:			// COLUMN_VISIBLE	*/
			break;
		case 83:			// DISPLAY
			break;
		case 84:			// DATA
			break;
		case 85:			// CHECKSTR
			break;
		case 86:			// BUTTONSTR
			break;
		case 87:			// BUTTONIMG
			break;
		case 88:			//ZEROSUPATTR2
			while (dat)
			{
				pos = dat.Find(_T(","));
				if (pos >= 0)
				{
					tmps = dat.Left(pos);
					dat  = dat.Mid(pos + 1);

					for (ii = 0; ii < ZEROSUPTBL; ii++)
					{
						if (!tmps.CompareNoCase(zerosupTbl[ii].strName))
							m_pFormItem->GetForm(fN)->attr2 |= zerosupTbl[ii].attr2;
					}
				}
				else
				{
					tmps = dat;
					for (ii = 0; ii < ZEROSUPTBL; ii++)
					{
						if (!tmps.CompareNoCase(zerosupTbl[ii].strName))
							m_pFormItem->GetForm(fN)->attr2 |= zerosupTbl[ii].attr2;
					}
					dat = _T("");
					break;
				}
			}
			break;
		case 89:			// CORGBATTR2
			while (dat)
			{
				pos = dat.Find(_T(","));
				if (pos >= 0)
				{
					tmps = dat.Left(pos);
					dat  = dat.Mid(pos + 1);

					for (ii = 0; ii < CORGBTBL; ii++)
					{
						if (!tmps.CompareNoCase(coRgbTbl[ii].strName))
							m_pFormItem->GetForm(fN)->attr2 |= coRgbTbl[ii].attr2;
					}
				}
				else
				{
					tmps = dat;
					for (ii = 0; ii < CORGBTBL; ii++)
					{
						if (!tmps.CompareNoCase(coRgbTbl[ii].strName))
							m_pFormItem->GetForm(fN)->attr2 |= coRgbTbl[ii].attr2;
					}
					dat = _T("");
					break;
				}
			}
			break;
		case 90:			// CONTRASTATTR2
			break;
		case 91:
			m_pFormItem->GetForm(fN)->fixrow = atoi(dat);
			break;
		case 92:
			m_pFormItem->GetForm(fN)->fixcol = atoi(dat);			
			break;
		case 93:
			m_pFormItem->GetForm(fN)->headH = atoi(dat);
			break;
		case 94:
			m_pFormItem->GetForm(fN)->rowof = atoi(dat);
			break;
		case 95:
			m_pFormItem->SetFormStr(fN, _T("rts"), dat);
			break;
		case 97:	// Alpha
			m_pFormItem->GetForm(fN)->alpha = atoi(dat);
			break;
		case 100:		// Check : Overlay Image Setting & Tab : Option Image Setting.
			m_pFormItem->GetForm(fN)->properties |= PR_OVERLAYIMG;
			break;
		case 101:		// Tab : Option Image & Button, Check, Radio : icon image & combobox : dropdown button image
			m_pFormItem->SetFormStr(fN, _T("optionimg"), dat);
			break;
		case 102:		// Grid : Option Pos (0: None, 1: Left, 2: Right)
			m_pFormItem->GetForm(fN)->optionPos = atoi(dat);
			break;
		case 103:		// Grid Select Style
			m_pFormItem->GetForm(fN)->selectShape = atoi(dat);
			break;
		case 104:		// Edit Domino Key
			m_pFormItem->SetFormStr(fN, _T("refs"), dat);
			break;
		case 106:		// onscroll
			m_pFormItem->GetForm(fN)->onScroll = atoi(dat);
			break;
		case 107:		// onscrollL
			m_pFormItem->GetForm(fN)->onScrollL = atoi(dat);
			break;
		case 111:		// Tab : Setting button
			m_pFormItem->GetForm(fN)->properties |= PR_TABSET;
			break;
		case 112:		// Output : text resize in rect
			m_pFormItem->GetForm(fN)->properties |= PR_RESIZE;
			break;
		case 113:		// combobox : dropdownlist bg image
			m_pFormItem->SetFormStr(fN, _T("optionimg2"), dat);
			break;
		case 114:		// combobox : dropdownlist select image
			m_pFormItem->SetFormStr(fN, _T("optionimg3"), dat);
			break;
		case 115:		// margin
			dat.Trim();
			if (!dat.IsEmpty())
			{
				struct _formR* formR = m_pFormItem->GetForm(fN);

				if (formR->margin == NULL)
				{
					formR->margin = new CStringArray;
					formR->margin->RemoveAll();
				}
				while (!dat.IsEmpty())
				{
					pos = dat.Find(_T(","));
					if (pos >= 0)
					{
						tmps = dat.Left(pos);
						dat  = dat.Mid(pos + 1);
					}
					else
					{
						tmps = dat;
						dat  = _T("");
					}
					formR->margin->Add(tmps);
				}
			}
			break;
		case 116:
			m_pFormItem->GetForm(fN)->properties |= PR_MARGIN;
			break;
		case 117:
			m_pFormItem->GetForm(fN)->opRGB = atoi(dat);
			break;
		case 119:
			m_pFormItem->GetForm(fN)->blinkShape = atoi(dat);
			break;
		case 121:
			m_pFormItem->GetForm(fN)->sfpRGB = atoi(dat);
			break;
		case 122:
			m_pFormItem->GetForm(fN)->properties |= PR_SCREFECT;
			break;
		case 123:
			m_pFormItem->GetForm(fN)->properties |= PR_REFRESH;
			break;
		}
	}
}

void CLoadXml::newCellitem()
{
	int	repN, multi;
	struct	_formR*	formR;
	struct	_repR*	repR;


	if (m_cellItem)
	{
		delete m_cellItem;
		m_cellItem = NULL;
	}

	formR = m_pFormItem->GetForm(m_fN);

	if (formR->kind == FM_GRID || formR->kind == FM_GRIDEX || formR->kind == FM_TABLE)
	{
		repN = -1;
		multi = 0;
		formR->auxR = new char[sizeof(_repR)];
		ZeroMemory(formR->auxR, sizeof(_repR));
		repR = (struct _repR *)formR->auxR;
		repR->selected = -1;
		repR->rows = m_row;
		repR->cols = m_col;

		m_cellItem = new CCellItem(repR);
		formR->pRGB = ARGBtoRGB(7);
	}
}

void CLoadXml::SetMulticolControl(CString parentnode, CString node, CString sKey, CString dat, int& fN)
{
	int	ii = 0;
	int	pos = -1;
	CString tmp, tmps;
	int	repN, multi, offs;
	CRect	tRc;

	repN = m_colcnt;
	multi = repN + 1;

	if (m_cControlForm.GetSize() > 0)
	{
		m_cControlForm.Lookup(sKey, tmp);

		if (dat.IsEmpty())
			return;

		switch (atoi(tmp))
		{
		case 0:				// SYMBOLNAME
			break;
		case 1:				// LEFT
			break;
		case 2:				// TOP
			break;
		case 3:				// RIGHT
			break;
		case 4:				// BOTTOM
			break;
		case 5:				// RADIOKIND
			break;
		case 6:				// BORDERS
			break;
		case 7:				// STR
			break;
		case 8:				// STR2
			break;
		case 9:				// DAT
			break;
		case 10:			// FLOAT
			m_cellItem->GetCell(repN)->vals = (BYTE)atoi(dat);
			break;
		case 11:			// DESC
			break;
		case 12:			// EDITFORMAT : editS
			CopyMemory(&m_cellItem->GetCell(repN)->editS, dat, dat.GetLength());
			m_cellItem->GetCell(repN)->editS[dat.GetLength()] = '\0';
			break;
		case 13:			// IFRGB : onEdit
			{
				WORD	upRGB,  dnRGB;
				upRGB =  dnRGB = 0;
				int	pos = -1;

				pos = dat.Find(_T(","));
				if (pos >= 0)
				{
					upRGB = ARGBtoRGB(atoi(dat.Left(pos)));
					dnRGB = ARGBtoRGB(atoi(dat.Mid(pos + 1)));
				}

				m_cellItem->GetCell(repN)->onEdit = MAKELONG(dnRGB, upRGB);
				m_cellItem->GetCell(repN)->attr |= FA_CORGB;
			}
			break;
		case 14:			// IFRGB2
			break;
		case 15:			// AUXS
			break;
		case 16:			// IMAGETYPE
			break;
		case 17:			// EIOIOK
			for (ii = 0; ii < EMODETBL; ii++)
			{
				if (!dat.CompareNoCase(emodeTbl[ii].strName))
				{
					if (m_pFormItem->GetForm(fN)->kind == FM_GRIDEX || m_pFormItem->GetForm(fN)->kind == FM_GRID)
						m_cellItem->GetCell(repN)->iok = emodeTbl[ii].iok;
					break;
				}
			}
			break;
		case 18:			// ALIGNMENT
			break;
		case 19:			// ALIGNMENTIMAGE
			break;
		case 20:			// FONTNAME
			m_cellItem->GetCell(repN)->fName = m_pFormItem->SetStr(m_cellItem->GetCell(repN)->fName, dat);
			//CopyMemory(m_cellItem->GetCell(repN)->fName, dat, dat.GetLength()); 
			break;
		case 21:			// FONTPOINT
			m_cellItem->GetCell(repN)->fPoint = atoi(dat);
			break;
		case 22:			// FONTSTYLE
			for (ii = 0; ii < FSTYLE; ii++)
			{
				if (!dat.CompareNoCase(fStyle[ii].strName))
				{
					m_cellItem->GetCell(repN)->fStyle = fStyle[ii].defValue;
					break;
				}
			}
			break;
		case 23:			// LEN
			break;
		case 24:			// COUNT
			m_cellItem->GetCell(repN)->keys = atoi(dat);
			break;
		case 25:			// VCOUNT	//
			break;
		case 26:			// INDEX
			break;
		case 27:			// MATCHKEY
			for (ii = 0; ii < VKTBL; ii++)
			{
				if (!dat.CompareNoCase(vkTbl[ii].strName))
				{
					m_cellItem->GetCell(repN)->vals2 = ii;
					break;
				}
			}
			break;
		case 28:			// CHARS
			m_pFormItem->GetForm(fN)->keys = atoi(dat);
			break;
		case 29:			// DATATYPE
			for (ii = 0; ii < IOTBL; ii++)
			{
				if (!dat.CompareNoCase(ioTbl[ii].strName))
				{
					m_cellItem->GetCell(repN)->type = ioTbl[ii].type;
					break;
				}
			}
			break;
		case 30:			// ATTRIBUTE
			while (dat)
			{
				pos = dat.Find(_T(","));
				if (pos >= 0)
				{
					tmps = dat.Left(pos);
					dat  = dat.Mid(pos + 1);

					for (ii = 0; ii < ATTRTBL; ii++)
					{
						if (!tmps.CompareNoCase(attrTbl[ii].strName))
							m_cellItem->GetCell(repN)->attr |= attrTbl[ii].attr;						
					}
				}
				else
				{
					tmps = dat;
					for (ii = 0; ii < ATTRTBL; ii++)
					{
						if (!tmps.CompareNoCase(attrTbl[ii].strName))
							m_cellItem->GetCell(repN)->attr |= attrTbl[ii].attr;
					}
					dat = _T("");
					break;
				}
			}

			if (m_cellItem->GetCell(repN)->attr & FA_WRAP)
				m_multi++;

			break;
		case 31:			// SUBATTRIBUTE
			break;
		case 32:			// PRGB
			m_cellItem->GetCell(repN)->pRGB = ARGBtoRGB(atoi(dat));
			break;
		case 33:			// TRGB
			m_cellItem->GetCell(repN)->tRGB = ARGBtoRGB(atoi(dat));
			break;
		case 34:			// SPRGB
			break;
		case 35:			// TABORDER
			break;
		case 36:			// STRGB
			break;
		case 37:			// IOORDER
			break;
		case 38:			// GROUPN
			break;
		case 39:			// RESIZABLE
			break;
		case 40:			// ENABLE
			m_cellItem->GetCell(repN)->properties |= PR_ENABLE;
			break;
		case 41:			// VISIBLE
			m_cellItem->GetCell(repN)->properties |= PR_VISIBLE;
			break;
		case 42:			// CHECKED
			m_cellItem->GetCell(repN)->properties |= PR_CHECKED;
			break;
		case 43:			// IMAGE
			m_cellItem->GetCell(repN)->properties |= PR_IMAGE;
			break;
		case 44:			// IMGTEXT
			m_cellItem->GetCell(repN)->properties |= PR_IMAGETEXT;
			break;
		case 45:			// HOVER
			m_cellItem->GetCell(repN)->properties |= PR_HOVER;
			break;
		case 46:			// SORT
			m_cellItem->GetCell(repN)->properties |= PR_SORT;
			break;
		case 47:			// SHOWFORM
			m_cellItem->GetCell(repN)->properties |= PR_EDIT;
			break;
		case 56:			// BLOCK
			break;
		case 57:			// DRAWN
			break;
		case 58:			// BCOMMON
			break;
		case 59:			// SCROLLPOS
			break;
		case 60:			// VISIBLEROW	*/
			break;
		case 61:			// CELLH
			break;
		case 62:			// MATRIX
			break;
		case 63:			// FIX
			break;
		case 64:			// MAINCOLOR
			break;
		case 65:			// LINECOLOR
			break;
		case 66:			// FOCUSCOLOR
			break;
		case 67:			// FOCUSKIND
			break;
		case 68:			// AUTOSIZE
			break;
		case 69:			// GRIDOPTION
			while (dat)
			{
				pos = dat.Find(_T(","));
				if (pos >= 0)
				{
					tmps = dat.Left(pos);
					dat  = dat.Mid(pos + 1);

					for (ii = 0; ii < GRIDOPTION; ii++)
					{
						if (!tmps.CompareNoCase(gridoptiontbl[ii].strName))
							m_cellItem->GetCell(repN)->attr2 |= gridoptiontbl[ii].attr2;
					}
				}
				else
				{
					tmps = dat;
					for (ii = 0; ii < GRIDOPTION; ii++)
					{
						if (!tmps.CompareNoCase(gridoptiontbl[ii].strName))
							m_cellItem->GetCell(repN)->attr2 |= gridoptiontbl[ii].attr2;
					}
					dat = _T("");
					break;
				}
			}
			break;
		case 70:			// COLS		*/
			break;
		case 71:			// BRGB		*/
			break;
		case 72:			// COLINFO
			CopyMemory(m_cellItem->GetCell(repN)->name, dat, dat.GetLength()); 
			m_cellItem->GetCell(repN)->name[dat.GetLength()]= '\0';
			m_cellItem->GetCell(repN)->lineN = (BYTE)m_multi;			
			m_cellItem->GetCell(repN)->iok   = EIO_OUTPUT;			
			break;
		case 73:			// CELLLEN
			m_cellItem->GetCell(repN)->size = (WORD)atoi(dat);
			break;
		case 74:			// WIDTH
			m_cellItem->GetCell(repN)->width = (WORD)atoi(dat);
			break;
		case 75:			// HEAD
			CopyMemory(m_cellItem->GetCell(repN)->head, dat, dat.GetLength());
			break;
		case 76:			// COLALIGNH
			for (ii = 0; ii < ALIGNTBL; ii++)
			{	
				if (!dat.Compare(alignTbl[ii].strName))
				{
					m_cellItem->GetCell(repN)->alignH = alignTbl[ii].alignment;
					break;
				}
			}
			break;
		case 77:			// COLALIGNC
			for (ii = 0; ii < ALIGNTBL; ii++)
			{	
				if (!dat.Compare(alignTbl[ii].strName))
				{
					m_cellItem->GetCell(repN)->alignC = alignTbl[ii].alignment;
					break;
				}
			}
			break;
		case 78:			// HEADCOLOR
			break;
		case 79:			// HPRGB
			m_cellItem->GetCell(repN)->hpRGB = ARGBtoRGB(atoi(dat));
			break;
		case 80:			// HTRGB
			m_cellItem->GetCell(repN)->htRGB = ARGBtoRGB(atoi(dat));
			break;
		case 81:			// DOM
			CopyMemory(m_cellItem->GetCell(repN)->auxS, dat, dat.GetLength());
			break;
		case 82:			// COLUMN_VISIBLE	*/
			break;
		case 83:			// DISPLAY
			offs = 0;			
			for (ii = 0; ii < m_cellItem->GetCell(repN)->keys; ii++)
			{
				pos = dat.Find(_T(","));
				if (pos >= 0)
				{
					tmps = dat.Left(pos);
					dat  = dat.Mid(pos + 1);
				}

				CopyMemory(&m_cellItem->GetCell(repN)->dat[offs], tmps, tmps.GetLength());
				offs += m_cellItem->GetCell(repN)->size;
				m_cellItem->GetCell(repN)->dat[offs++] = '\t';
			}
			break;
		case 84:			// DATA
			offs = 0;			
			for (ii = 0; ii < m_cellItem->GetCell(repN)->keys; ii++)
			{
				pos = dat.Find(_T(","));
				if (pos >= 0)
				{
					tmps = dat.Left(pos);
					dat  = dat.Mid(pos + 1);
				}

				CopyMemory(&m_cellItem->GetCell(repN)->str[offs], tmps, tmps.GetLength());
				offs += m_cellItem->GetCell(repN)->size;
				m_cellItem->GetCell(repN)->str[offs++] = '\t';
			}
			break;
		case 85:			// CHECKSTR
			CopyMemory(&m_cellItem->GetCell(repN)->str2, dat, dat.GetLength());
			m_cellItem->GetCell(repN)->str2[dat.GetLength()] = '\0';
			break;
		case 86:			// BUTTONSTR
			CopyMemory(&m_cellItem->GetCell(repN)->txt, dat, dat.GetLength());
			m_cellItem->GetCell(repN)->txt[dat.GetLength()] = '\0';
			break;
		case 87:			// BUTTONIMG
			CopyMemory(&m_cellItem->GetCell(repN)->str3, dat, dat.GetLength());
			m_cellItem->GetCell(repN)->str3[dat.GetLength()] = '\0';
			break;
		case 88:			//ZEROSUPATTR2
			while (dat)
			{
				pos = dat.Find(_T(","));
				if (pos >= 0)
				{
					tmps = dat.Left(pos);
					dat  = dat.Mid(pos + 1);

					for (ii = 0; ii < ZEROSUPTBL; ii++)
					{
						if (!tmps.CompareNoCase(zerosupTbl[ii].strName))
							m_cellItem->GetCell(repN)->attr2 |= zerosupTbl[ii].attr2;
					}
				}
				else
				{
					tmps = dat;
					for (ii = 0; ii < ZEROSUPTBL; ii++)
					{
						if (!tmps.CompareNoCase(zerosupTbl[ii].strName))
							m_cellItem->GetCell(repN)->attr2 |= zerosupTbl[ii].attr2;
					}
					dat = _T("");
					break;
				}
			}
			break;
		case 89:			// CORGBATTR2
			while (dat)
			{
				pos = dat.Find(_T(","));
				if (pos >= 0)
				{
					tmps = dat.Left(pos);
					dat  = dat.Mid(pos + 1);

					for (ii = 0; ii < CORGBTBL; ii++)
					{
						if (!tmps.CompareNoCase(coRgbTbl[ii].strName))
							m_cellItem->GetCell(repN)->attr2 |= coRgbTbl[ii].attr2;
					}
				}
				else
				{
					tmps = dat;
					for (ii = 0; ii < CORGBTBL; ii++)
					{
						if (!tmps.CompareNoCase(coRgbTbl[ii].strName))
							m_cellItem->GetCell(repN)->attr2 |= coRgbTbl[ii].attr2;
					}
					dat = _T("");
					break;
				}	
			}
			break;
		case 90:			// CONTRASTATTR2
			break;
		case 95:
			CopyMemory(m_cellItem->GetCell(repN)->rtssy, dat, dat.GetLength());
			break;
		case 96:
			m_cellItem->GetCell(repN)->properties |= PR_DRAGROW;
			break;
		case 98:			// Cell Kind
			m_cellItem->GetCell(repN)->kind = atoi(dat);
			break;
		case 105:
			CopyMemory(m_cellItem->GetCell(repN)->merge, dat, dat.GetLength());
			break;
		case 108:
			m_cellItem->GetCell(repN)->height = atoi(dat);
			break;
		case 109:
			m_cellItem->GetCell(repN)->headH = atoi(dat);
			break;
		case 110:
			CopyMemory(m_cellItem->GetCell(repN)->str4, dat, dat.GetLength());
			break;
		case 112:			// Grid/GridEx/Table : text resize in rect
			m_cellItem->GetCell(repN)->properties |= PR_RESIZE;
			break;
		case 115:			// cell margin
			dat.Trim();
			if (!dat.IsEmpty())
			{
				if (m_cellItem->GetCell(repN)->margin == NULL)
				{
					m_cellItem->GetCell(repN)->margin = new CStringArray;
					m_cellItem->GetCell(repN)->margin->RemoveAll();
				}
				while (!dat.IsEmpty())
				{
					pos = dat.Find(_T(","));
					if (pos >= 0)
					{
						tmps = dat.Left(pos);
						dat  = dat.Mid(pos + 1);
					}
					else
					{
						tmps = dat;
						dat  = _T("");
					}
					m_cellItem->GetCell(repN)->margin->Add(tmps);
				}
			}
			break;
		case 116:
			m_cellItem->GetCell(repN)->properties |= PR_MARGIN;
			break;
		case 118:	// cell custom name, property
			dat.Trim();
			if (!dat.IsEmpty())
			{
				pos = dat.Find(_T(","));
				if (pos >= 0)
				{
					tmps = dat.Left(pos);
					dat  = dat.Mid(pos + 1);
				}
				else
				{
					tmps = dat;
					dat  = _T("");
				}
				CopyMemory(m_cellItem->GetCell(repN)->dat, tmps, tmps.GetLength());
				m_cellItem->GetCell(repN)->dat[tmps.GetLength()] = '\0';
				CopyMemory(m_cellItem->GetCell(repN)->str, dat, dat.GetLength());
				m_cellItem->GetCell(repN)->str[dat.GetLength()] = '\0';
			}
			break;

		case 120:
			m_cellItem->GetCell(repN)->chartLimit = (WORD)atoi(dat);
			break;

		}
	}
}

void CLoadXml::checkGridMultiLine(int fN)
{
	struct	_formR*	formR = m_pFormItem->GetForm(fN);

	if ((formR->kind == FM_GRIDEX || formR->kind == FM_GRID) && (formR->attr2 & GO_MULTI))
	{
		if (formR->attr2 & GO_MERGE)
		{
			formR->attr2 &= ~GO_MULTI;
			return;
		}

		struct _repR* repR = (struct _repR *)formR->auxR;
		if (repR)
		{
			int	line, colN;
			CCellItem* cellItem = new CCellItem(repR);

			line = colN = 0;
			for (int ii = 0; ii < repR->cols; ii ++)
			{
				colN++;
				cellItem->GetCell(ii)->lineN = (BYTE)line;
				if (cellItem->GetCell(ii)->attr & FA_WRAP)
					line++;
			}
			delete cellItem;
		}
	}
}

void CLoadXml::SetScript(CString sKey, CString dat, int formN)
{
	CString tmp;
	char	fileName[128];
	char	prefix[L_SGID+1];

	if (m_cScript.GetSize() > 0)
	{
		if (!m_cScript.Lookup(sKey, tmp))
			return;

		dat.TrimRight();
			
		CopyMemory(prefix, m_mapH->mapN, L_SGID); prefix[L_SGID] = '\0';

		// global script
		if (atoi(tmp) >= 0 && atoi(tmp) <= 11)
		{
			char* onname = NULL;

			switch (atoi(tmp))
			{
			case 0:
				m_mapH->onLoad = true; onname = &m_mapH->onLoadN[0];		break;
			case 1:
				m_mapH->onSend = true; onname = &m_mapH->onSendN[0];		break;
			case 2:
				m_mapH->onReceive = true; onname = &m_mapH->onReceiveN[0];	break;
			case 3:
				m_mapH->onAlert = true; onname = &m_mapH->onAlertN[0];		break;
			case 4:
				m_mapH->onTimer = true; onname = &m_mapH->onTimerN[0];		break;
			case 5:
				m_mapH->onClose = true; onname = &m_mapH->onCloseN[0];		break;
			case 6:
				m_mapH->onDevice = true; onname = &m_mapH->onDeviceN[0];	break;
			case 7:
				m_mapH->onError = true; onname = &m_mapH->onErrorN[0];		break;
			case 8:
				m_mapH->onFlicking = true; onname = &m_mapH->onFlickingN[0];	break;
			case 9:
				m_mapH->onChangeLayout = true; onname = &m_mapH->onChangeLayoutN[0];break;
			case 10:
				m_mapH->onDeclaration = true; onname = &m_mapH->onDeclarationN[0];break;
			case 11:
				m_mapH->onInDeclaration = true; onname = &m_mapH->onInDeclarationN[0];break;

			}

			if (onname != NULL)
			{
				((CamCcApp*)AfxGetApp())->GetTempName(onname, prefix);
				SaveScriptFile(CString(onname), dat, dat.GetLength());
			}
		}
		else
		{
			// form object
			if (formN >= 0 && m_pFormItem->GetFormStr(formN, _T("scpN")).IsEmpty())
			{
				((CamCcApp*)AfxGetApp())->GetTempName(fileName, prefix);
				m_pFormItem->SetFormStr(formN, _T("scpN"), fileName);
			}

			switch (atoi(tmp))
			{
			case 12:
				if (dat.IsEmpty())
				{
					m_pFormItem->GetForm(formN)->onTap = -1;
					break;
				}
				SaveScriptFile(m_pFormItem->GetFormStr(formN, "scpN"), dat, m_pFormItem->GetForm(formN)->onTapL);
				break;
			case 13:
				if (dat.IsEmpty())
				{
					m_pFormItem->GetForm(formN)->onLongTap = -1;
					break;
				}
				SaveScriptFile(m_pFormItem->GetFormStr(formN, "scpN"), dat, m_pFormItem->GetForm(formN)->onLongTapL);
				break;
			case 14:
				if (dat.IsEmpty())
				{
					m_pFormItem->GetForm(formN)->onChange = -1;
					break;
				}
				SaveScriptFile(m_pFormItem->GetFormStr(formN, "scpN"), dat, m_pFormItem->GetForm(formN)->onChangeL);
				break;
			case 15:
				if (dat.IsEmpty())
				{
					m_pFormItem->GetForm(formN)->onChar = -1;
					break;
				}
				SaveScriptFile(m_pFormItem->GetFormStr(formN, "scpN"), dat, m_pFormItem->GetForm(formN)->onCharL);
				break;
		//	case 16:	// ondrag
			case 17:		// onScroll
				if (dat.IsEmpty())
				{
					m_pFormItem->GetForm(formN)->onScroll = -1;
					break;
				}
				SaveScriptFile(m_pFormItem->GetFormStr(formN, "scpN"), dat, m_pFormItem->GetForm(formN)->onScrollL);
				break;
			}
		}
	}
}

void CLoadXml::SaveScriptFile(CString sPath, CString sScript, int size)
{
	if (size <= 0) return;

	CFile	sfile;
	if (!sfile.Open(sPath, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
		return;

	sfile.SeekToEnd();
	sfile.Write(sScript, size);
	sfile.Close();

// 	if (size > 0)
// 	{
// 		CFile	ffile;
// 		int	fsize;
// 		char	*buff;
// 
// 		buff = new char[size + 1];
// 		ZeroMemory(buff, size + 1);
// 		FillMemory(buff, size + 1, ' ');
// 		CopyMemory(buff, (char*)sScript.operator LPCTSTR(), size);
// 
// 		if (size > 0)
// 			fsize = size;
// 		else
// 			fsize = sScript.GetLength();
// 
// 		if (ffile.Open(sPath, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
// 		{
// 			ffile.SeekToEnd();
// 			//ffile.Write(sScript, fsize);
// 			ffile.Write(buff, fsize);
// 			ffile.Close();
// 		}
// 
// 		delete [] buff;
// 	}
}

int CLoadXml::getFormIndex(char* symbol, int len, int count)
{
	int ii;
	CString	str = CString(symbol, len);

	for (ii = 0; ii < count; ii++)
	{
		if (!str.CompareNoCase(m_pFormItem->GetFormStr(ii, "name")))
			break;
	}

	if (ii >= count)
		return (-1);

	return (ii);
}

int CLoadXml::getColumnIndex(char* symbol, int len, int pos)
{
	int	count, ii;
	CString	tmps;
	struct	_repR*	repR;

	repR = (struct _repR *)m_pFormItem->GetForm(pos)->auxR;
	CCellItem *cellItem = new CCellItem(repR);
	if (m_pFormItem->GetForm(pos)->kind == FM_TABLE)
		count = repR->rows;
	else
		count = repR->cols;

	tmps = CString(symbol, len);
	for (ii = 0; ii < count; ii++)  // Column Symbol Check
	{
		if (!tmps.Compare(cellItem->GetCell(ii)->name))
			break;
	}

	delete cellItem;
	if (ii >= count)
		return (-1);

	return (ii);
}

char* CLoadXml::SetStr(char *map, CString data)
{
	if (map)
		delete[] map;
	map = new char[data.GetLength() + 1];
	ZeroMemory(map, data.GetLength() + 1);
	CopyMemory(map, (LPCTSTR)data, data.GetLength());
	return map;
}

void CLoadXml::removeIXMLDOMNode(IXMLDOMNode* pNode)
{
	if (pNode)
	{
		pNode->Release();
		pNode = NULL;
	}
}

void CLoadXml::removeIXMLDOMElement(IXMLDOMElement* pElement)
{
	if (pElement)
	{
		pElement->Release();
		pElement = NULL;
	}
}

void CLoadXml::removeIXMLDOMNamedNodeMap(IXMLDOMNamedNodeMap* pNodename)
{
	if (pNodename)
	{
		pNodename->Release();
		pNodename = NULL;
	}
}

void CLoadXml::removeIXMLDOMAttribute(IXMLDOMAttribute* pAttribute)
{
	if(pAttribute)
	{
		pAttribute->Release();
		pAttribute = NULL;
	}
}

CString CLoadXml::Parser(CString &srcstr, CString substr)
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

int CLoadXml::ARGBtoRGB(int color)
{
	DWORD	value;
	value = color;

	if (value & 0x02000000)
	{
		int a = (color & 0xFF000000) >> 24;
		int r = (color & 0x00FF0000) >> 16;
		int g = (color & 0x0000FF00) >> 8;
		int b = (color & 0x000000FF);
		return PALETTERGB(b, g, r);
	}

	return value;
}

void CLoadXml::SetTrRtsInfo()
{
	CString trinfo = _T("");
	CString rtsinfo = _T("");
	CString tmp, sKey, str;
	int len;

	for (POSITION pos = ((CamCcApp *)AfxGetApp())->m_trlayoutxml.GetStartPosition(); pos != NULL; )
	{
		((CamCcApp *)AfxGetApp())->m_trlayoutxml.GetNextAssoc(pos, sKey, str);
		trinfo += str; trinfo += _T("^");
	}
	len = strlen(trinfo);
	m_mapH->trinfo = new char[len+1];
	CopyMemory(m_mapH->trinfo, trinfo, len);
	m_mapH->trinfo[len] = 0x00;

//TRACE("-----------readTrlayout1--------------%s\n", trinfo);

	for (POSITION pos = ((CamCcApp *)AfxGetApp())->m_rtssymbolxml.GetStartPosition(); pos != NULL; )
	{
		((CamCcApp *)AfxGetApp())->m_rtssymbolxml.GetNextAssoc(pos, sKey, str);

		if (sKey.CompareNoCase("RTSINFO"))
			continue;

		Parser(str, _T("\t"));
		rtsinfo += str; //rtsinfo += _T("^");
	}
	len = rtsinfo.GetLength();
	m_mapH->rtsinfo = new char[len + 1];
	CopyMemory(m_mapH->rtsinfo, rtsinfo, len);
	m_mapH->rtsinfo[len] = 0x00;
//TRACE("-----------readTrlayout11111--------------%s\n", rtsinfo);
}

// CLoadXml 메시지 처리기입니다.
