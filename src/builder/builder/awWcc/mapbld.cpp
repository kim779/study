// mapbld.cpp : CCompile class의 구현 파일입니다.
//

#include "stdafx.h"
#include "awWcc.h"
#include "Compile.h"
//#include "../../h/mapform.h"
#include "maptbl.hxx"

int CCompile::parseGlobalToken(CMapToken* token, int line, WORD *type)
{
	CString	text;
	CString	tmp;
	int	tN;		

	tN = 0;
	text = m_text[line].text;

	m_tokFlag = 0;
	for (int ii = 0; ii < text.GetLength(); ii++)
	{
		switch (text.GetAt(ii))
		{
		case ' ':
		case '\t':
			if ((m_tokFlag & TOKF_QUS) || (m_tokFlag & TOKF_DQS))
			{
				tmp += text.GetAt(ii);
				break;
			}

			if (m_tokFlag & TOKF_HOS)
			{
				if (tmp.GetLength())
					tmp += text.GetAt(ii);
			}
			break;

		case '(':
			if ((m_tokFlag & TOKF_QUS) || (m_tokFlag & TOKF_DQS))
			{
				tmp += text.GetAt(ii);
				break;
			}

			if (m_tokFlag & TOKF_HOS)
			{
				setMessage(line, 3);
				return -1;
			}
			else
				m_tokFlag |= TOKF_HOS;

			if (tmp.GetLength())
			{
				token[tN].name = tmp;
				token[tN].len  = tmp.GetLength();
				tmp.Empty();
				tN++;
			}
			break;

		case ')':
			if ((m_tokFlag & TOKF_QUS) || (m_tokFlag & TOKF_DQS))
			{
				tmp  += text.GetAt(ii);
				break;
			}

			if (m_tokFlag & TOKF_HOE)
			{
				setMessage(line, 4);
				return -1;
			}
			else if (m_tokFlag & TOKF_HOS)
			{
				m_tokFlag &= ~TOKF_HOS;
				m_tokFlag &= ~TOKF_HOE;

				if (tmp.GetLength() > 0)
				{
					token[tN].name = tmp;
					token[tN].len = tmp.GetLength();
					tmp.Empty();
					tN++;
				}
			}
			else
				m_tokFlag |= TOKF_HOE;
			break;

		case '\"':
			if (!(m_tokFlag & TOKF_DQS))
			{
				m_tokFlag |= TOKF_DQS;
				break;
			}

			m_tokFlag &= ~TOKF_DQS;

			token[tN].name = tmp;
			token[tN].len  = tmp.GetLength();
			token[tN].type = TOK_FILE;
			tmp.Empty();
			tN++;
			break;

		case ',':
		case '|':
			if ((m_tokFlag & TOKF_QUS) || (m_tokFlag & TOKF_DQS))
			{
				tmp += text.GetAt(ii);
				break;
			}

			if ((m_tokFlag & TOKF_HOS) && tmp.GetLength())
			{
				token[tN].name = tmp;
				token[tN].len = tmp.GetLength();
				tmp.Empty(); tN++;
			}
			break;

		default:
			tmp += text.GetAt(ii);
			break;
		}
	}

	if (m_tokFlag & TOKF_MSK)
	{
		setMessage(line, 2);
		return -1;
	}
	*type = TYPE_GLOBAL;
	return tN;
}

int CCompile::parseFormToken(CMapToken* token, int line, WORD *type)
{
	CString	text;
	CString tmp = "";
	int	tN;

	tN = 0;
	text = m_text[line].text;

	m_tokFlag = 0;
	for (int ii = 0; ii < text.GetLength(); ii++)
	{
		switch (text.GetAt(ii))
		{
		case ' ':
		case '\t':
			if ((m_tokFlag & TOKF_QUS) || (m_tokFlag & TOKF_DQS))
			{
				tmp += text.GetAt(ii);
				break;
			}

			if (!tmp.IsEmpty())
			{
				token[tN].name = tmp;
				token[tN].len  = tmp.GetLength();
				token[tN].type = TOK_NONE;
				tmp.Empty();
				tN++;
			}
			break;

		case '(':
			if ((m_tokFlag & TOKF_QUS) || (m_tokFlag & TOKF_DQS))
			{
				tmp += text.GetAt(ii);
				break;
			}

			if (m_tokFlag & TOKF_HOS)
			{
				setMessage(line, 3);
				return -1 ;
			}
			else
				m_tokFlag |= TOKF_HOS;

			if (!tmp.IsEmpty())
			{
				token[tN].name = tmp;
				token[tN].len  = tmp.GetLength();
				token[tN].type = TOK_NONE;
				tmp.Empty();
				tN++;
			}
			break;

		case ')':
			if ((m_tokFlag & TOKF_QUS) || (m_tokFlag & TOKF_DQS))
			{
				tmp += text.GetAt(ii);
				break;
			}

			if (m_tokFlag & TOKF_HOE)
			{
				setMessage(line, 4);
				return -1;
			}
			else if (m_tokFlag & TOKF_HOS)
			{
				m_tokFlag &= ~TOKF_HOS;
				m_tokFlag &= ~TOKF_HOE;

				if (!tmp.IsEmpty())
				{
					token[tN].name = tmp;
					token[tN].len  = tmp.GetLength();
					token[tN].type = TOK_NUM;
					tmp.Empty();
					tN++;
				}
			}
			else
				m_tokFlag |= TOKF_HOE;
			break;

		case '|':
			if ((m_tokFlag & TOKF_QUS) || (m_tokFlag & TOKF_DQS))
			{
				tmp += text.GetAt(ii);
				break;
			}

			token[tN].type = (m_tokFlag & TOKF_HOS) ? TOK_NUM : TOK_NONE;
			if (!tmp.IsEmpty())
			{
				token[tN].name = tmp;
				token[tN].len  = tmp.GetLength();
				tmp.Empty();
				tN++;
			}
			break;

		case ',':
			if ((m_tokFlag & TOKF_QUS) || (m_tokFlag & TOKF_DQS))
			{
				tmp += text.GetAt(ii);
				break;
			}

			token[tN].type = (m_tokFlag & TOKF_HOS) ? TOK_NUM : TOK_NONE;
			if (!tmp.IsEmpty())
			{
				token[tN].name = tmp;
				token[tN].len = tmp.GetLength();
				tmp.Empty();
				tN++;
			}
			break;

		case '\'':				// 0x27
			if (!(m_tokFlag & TOKF_QUS))
			{
				m_tokFlag |= TOKF_QUS;
				break;
			}

			m_tokFlag &= ~TOKF_QUS;
			if (!tmp.IsEmpty())
			{
				token[tN].name = tmp;
				token[tN].len  = tmp.GetLength();
				token[tN].type = TOK_TEXT;
				tmp.Empty();
				tN++;
			}
			else
				setMessage(line, 5);
			break;

		case '\"':				// 0x22
			if (!(m_tokFlag & TOKF_DQS))
			{
				m_tokFlag |= TOKF_DQS;
				break;
			}

			m_tokFlag &= ~TOKF_DQS;
			token[tN].name = tmp;
			token[tN].len  = tmp.GetLength();
			token[tN].type = TOK_FILE;
			tmp.Empty();
			tN++;
			break;

		default:
			tmp += text.GetAt(ii);
			break;
		}
	}

	if (!tmp.IsEmpty())
	{
		token[tN].name = tmp;
		token[tN].len  = tmp.GetLength();
		token[tN].type = TOK_NONE;
		tN++;
	}

	*type = m_text[line].type;
	return tN;
}

int CCompile::parseResourceToken(CMapToken* token, int line, WORD *type)
{
	CString tmp;
	CString	str;
	int	tN;
	int	lineL;

	tN = 0;
	str = CString(m_text[line].text);
	str.TrimRight();
	lineL = (int)strlen(m_text[line].text);

	m_tokFlag = 0;
	if (str.Find(_T("_AW_SCRIPT_AW_(")) == 0)
	{
		token[tN].name = _T("_AW_SCRIPT_AW_");
		token[tN].len  = token[tN].name.GetLength();	// 14
		token[tN].type = TOK_OPER;
		tN++;
		
		str = str.Mid(15, str.Find(")") - 15);
		token[tN].name = str;
		token[tN].len  = str.GetLength();
		token[tN].type = TOK_NONE;
		m_tokFlag = 0;
		return tN;
	}
	
	token[tN].name = str;
	token[tN].len  = str.GetLength();
	token[tN++].type = TOK_NONE;

	*type = TYPE_RESOURCE;
	return tN;
}

void CCompile::buildFormStr(CMapToken* token, int tokCnt, int line)
{
	int	icell, nTok = 0;
	int	ii, jj, kk, ll, mm, rc;
	CString	tmps;
	struct	_domR	colDom[MAX_COLUMN];

	tmps = CString(token[nTok].name, token[nTok].len);
	for (ii = 0; ii < FORMTBL; ii++)
	{
		if (!tmps.CompareNoCase(formTbl[ii].strName))
			break;
	}

	if (ii >= FORMTBL)
	{
		setMessage(line, 16, tmps);
		return;
	}

	nTok++;
	m_formR[m_fN].kind = formTbl[ii].kind;
	m_formR[m_fN].index = 0xFFFF;
	m_formR[m_fN].iorder = 0xFFFF;
	
	if (m_formR[m_fN].kind == FM_PANEL)
		m_formR[m_fN].type = PN_NONE;
	if (m_formR[m_fN].kind == FM_OUT)
		m_formR[m_fN].iok = PN_NONE;

	if (m_formR[m_fN].kind == FM_GRID || m_formR[m_fN].kind == FM_GRIDEX || m_formR[m_fN].kind == FM_TABLE)
	{
		icell = m_formR[m_fN].vals[0] = m_repN+1;
		m_formR[m_fN].vals[1] = 0;
		for (int jj = 0; jj < MAX_COLUMN; jj++)
			colDom[jj].dSym[0] = '\0';
	}
								//Symbol
	if (!compareSymbol(CString(token[nTok].name, token[nTok].len)))
	{
		setMessage(line, 13, CString(token[nTok].name, token[nTok].len), m_fN);
		return;
	}
	m_formR[m_fN].name = writeString(token[nTok].name);
	nTok++;
	for (jj = 0; jj < 4; jj++)			// RECT
	{
		if (token[nTok].type != TOK_NUM)
		{
			setMessage(line, 12, CString(token[nTok].name, token[nTok].len), m_fN);
			return;
		}
		switch (jj)
		{
		case 0:
			m_formR[m_fN].left   = max(0, atoi(token[nTok].name));
			break;
		case 1:
			m_formR[m_fN].top    = max(0, atoi(token[nTok].name));
			break;
		case 2:
			m_formR[m_fN].right  = max(0, atoi(token[nTok].name));
			break;
		case 3:
			m_formR[m_fN].bottom = max(0, atoi(token[nTok].name));
			break;
		}
		nTok++;
	}

	switch (m_formR[m_fN].kind)
	{
	case FM_EDIT:
	case FM_COMBO:
	case FM_OUT:
	case FM_MEMO:
	case FM_TREE:
	case FM_SHEET:
		for (jj = nTok; jj < tokCnt; jj++)
		{
			tmps = CString(token[jj].name, token[jj].len);
			for (kk = 0; kk < IOTBL; kk++)
			{
				if (!tmps.CompareNoCase(ioTbl[kk].strName))
				{ 
					m_formR[m_fN].type = ioTbl[kk].charType;
					token[jj].type     = TOK_USE;
					break;
				}
			}

			if (kk >= IOTBL)
				continue;
			break;
		}
		if (jj >= tokCnt)
		{
			setMessage(line, 9, tmps, m_fN);
			return;
		}
		break;

	default:
		break;
	}
							// Varible
	for (jj = nTok; jj < tokCnt; jj++)
	{
		if (token[jj].type == TOK_USE)
			continue;

		if (token[jj].type == TOK_TEXT)
		{
			m_formR[m_fN].vals[0] = writeString(token[jj].name);
			m_formR[m_fN].size = token[jj].len;
			continue;
		}

		if (token[jj].type == TOK_FILE)
		{
			switch (m_formR[m_fN].kind)
			{
			case FM_AVI:
				m_formR[m_fN].vals[0] = writeString(token[jj].name);
				m_formR[m_fN].size = token[jj].len;
				continue;
			case FM_PANEL:
			case FM_BUTTON:
				m_formR[m_fN].vals[1] = writeString(token[jj].name);
				m_formR[m_fN].size = token[jj].len;
				continue;
			case FM_OBJECT:
				m_formR[m_fN].vals[0] = writeString(token[jj].name);
				continue;
			case FM_CONTROL:
				m_formR[m_fN].vals[0] = writeString(token[jj].name);
				jj++;
				m_formR[m_fN].vals[1] = writeString(token[jj].name);
				continue;
			default:
				setMessage(line, 8, CString(token[jj].name, token[jj].len), m_fN);
				return;
			}
		}

		tmps = CString(token[jj].name, token[jj].len);
		switch (m_formR[m_fN].kind)
		{
		case FM_COMBO:
			for (kk = 0; kk < PANELTBL; kk++)
			{
				if (!tmps.CompareNoCase(panelTbl[kk].strName))
				{
					m_formR[m_fN].borders = panelTbl[kk].borders;
					break;
				}
			}
			if (kk < PANELTBL)
				continue;

			for (kk = 0; kk < IETBL; kk++)
			{
				if (!tmps.CompareNoCase(ieTbl[kk].strName))
				{
					m_formR[m_fN].isEdit = ieTbl[kk].whatEdit;
					jj++;
					switch (ieTbl[kk].whatEdit)
					{
					case IE_CORGB:
						break;
					case IE_EDIT:
					default:
						m_formR[m_fN].onEdit = writeString(token[jj].name);
						break;
					}
					break;
				}
			}
			
			if (kk < IETBL)
				continue;

		case FM_EDIT:
		case FM_OUT:
			for (kk = 0; kk < ZEROSUPTBL; kk++)
			{
				if (!tmps.CompareNoCase(zerosupTbl[kk].strName))
				{
					m_formR[m_fN].attr2 |= zerosupTbl[kk].auxAttributes;
					break;
				}
			}
			if (kk < ZEROSUPTBL)
				continue;

			for (kk = 0; kk < CORGBTBL; kk++)
			{
				if (!tmps.CompareNoCase(coRgbTbl[kk].strName))
				{
					m_formR[m_fN].attr2 |= coRgbTbl[kk].auxAttributes;
					break;
				}
			}
			if (kk < CORGBTBL)
				continue;

			for (kk = 0; kk < ASENDTBL; kk++)
			{
				if (!tmps.CompareNoCase(asendTbl[kk].strName))
				{
					m_formR[m_fN].attr2 |= asendTbl[kk].auxAttributes;
					break;
				}
			}
			if (kk < ASENDTBL)
				continue;

			for (kk = 0; kk < CONTRASTTBL; kk++)
			{
				if (!tmps.CompareNoCase(contrastTbl[kk].strName))
				{
					m_formR[m_fN].attr2 |= contrastTbl[kk].auxAttributes;
					break;
				}
			}
			if (kk < CONTRASTTBL)
				continue;

			for (kk = 0; kk < DATETBL; kk++)
			{
				if (!tmps.CompareNoCase(dateTbl[kk].strName))
				{
					m_formR[m_fN].attr2 |= dateTbl[kk].auxAttributes;
					break;
				}
			}
			if (kk < DATETBL)
				continue;

			for (kk = 0; kk < PANELTBL; kk++)
			{
				if (!tmps.CompareNoCase(panelTbl[kk].strName))
				{
					m_formR[m_fN].borders = panelTbl[kk].borders;
					if (panelTbl[kk].borders != BD_NONE &&
						m_formR[m_fN].kind != FM_EDIT)
						m_formR[m_fN].iok = PN_BORDER;
					break;
				}
			}
			if (kk < PANELTBL)
				continue;
			
			for (kk = 0; kk < IETBL; kk++)
			{
				if (!tmps.CompareNoCase(ieTbl[kk].strName))
				{
					m_formR[m_fN].isEdit = ieTbl[kk].whatEdit;
					jj++;
					WORD	upRGB,  dnRGB;
					switch (ieTbl[kk].whatEdit)
					{
					case IE_EDIT:
						m_formR[m_fN].onEdit = writeString(token[jj].name);
						break;
					case IE_CORGB:
						upRGB = atoi(token[jj].name);	jj++;
						dnRGB = atoi(token[jj].name);
						//m_formR[m_fN].onEdit = MAKELONG(dnRGB, upRGB);
						m_formR[m_fN].onEdit = writeValue(upRGB);
						writeValue(dnRGB);
						break;
					default:
						break;
					}
					break;
				}
			}
			
			if (kk < IETBL)
				continue;

		case FM_TABLE:
		case FM_BROWSER:
		case FM_CONTROL:
			for (kk = 0; kk < EMODETBL; kk++)
			{
				if (!tmps.CompareNoCase(emodeTbl[kk].strName))
				{
					m_formR[m_fN].iok = emodeTbl[kk].ioType;
					break;
				}
			}
			if (kk < EMODETBL)
				continue;
			break;

		case FM_MEMO:
			for (kk = 0; kk < EMODETBL; kk++)
			{
				if (!tmps.CompareNoCase(emodeTbl[kk].strName))
				{
					m_formR[m_fN].iok = emodeTbl[kk].ioType;
					break;
				}
			}
			if (kk < EMODETBL)
				continue;

			for (kk = 0; kk < PANELTBL; kk++)
			{
				if (!tmps.CompareNoCase(panelTbl[kk].strName))
				{
					m_formR[m_fN].borders = panelTbl[kk].borders;
					break;
				}
			}
			if (kk < PANELTBL)
				continue;
			break;

		case FM_SHEET:
			for (kk = 0; kk < SHEETSTYLETBL; kk++)
			{
				if (!tmps.CompareNoCase(sheetStyleTbl[kk].strName))
				{
					m_formR[m_fN].type = sheetStyleTbl[kk].type;
					break;
				}
			}
			if (kk < SHEETSTYLETBL)
				continue;

			for (kk = 0; kk < EMODETBL; kk++)
			{
				if (!tmps.CompareNoCase(emodeTbl[kk].strName))
				{
					m_formR[m_fN].iok = emodeTbl[kk].ioType;
					break;
				}
			}
			if (kk < EMODETBL)
				continue;
			break;

		case FM_SYS:
			for (kk = 0; kk < SYSTBL; kk++)
			{
				if (!tmps.CompareNoCase(sysTbl[kk].strName))
				{
					m_formR[m_fN].type = sysTbl[kk].type;
					break;
				}
			}
			if (kk < SYSTBL)
				continue;

		case FM_BOX:
			for (kk = 0; kk < BOXTBL; kk++)
			{
				if (!tmps.CompareNoCase(boxTbl[kk].strName))
				{
					m_formR[m_fN].type = boxTbl[kk].type;
					break;
				}
			}
			if (kk < BOXTBL)
				continue;
			break;

		case FM_PANEL:
			for (kk = 0; kk < PANELTBL; kk++)
			{
				if (!tmps.CompareNoCase(panelTbl[kk].strName))
				{
					m_formR[m_fN].borders = panelTbl[kk].borders;
					m_formR[m_fN].type = PN_BORDER;
					break;
				}
			}
			if (kk < PANELTBL)
				continue;
			break;

		case FM_BUTTON:
			for (kk = 0; kk < EMODETBL; kk++)
			{
				if (!tmps.CompareNoCase(emodeTbl[kk].strName))
				{
					m_formR[m_fN].iok = emodeTbl[kk].ioType;
					break;
				}
			}
			if (kk < EMODETBL)
				continue;

			for (kk = 0; kk < BUTTONTBL; kk++)
			{
				if (!tmps.CompareNoCase(buttonTbl[kk].strName))
				{
					m_formR[m_fN].type = buttonTbl[kk].type;
					break;
				}
			}
			if (kk < BUTTONTBL)
				continue;
			break;

		case FM_UTAB:
		case FM_TAB:
			for (kk = 0; kk < TABSTYLETBL; kk++)
			{
				if (!tmps.CompareNoCase(tabStyleTbl[kk].strName))
				{
					m_formR[m_fN].type = tabStyleTbl[kk].type;
					break;
				}
			}
			if (kk < TABSTYLETBL)
				continue;
			break;

		default:
			break;
		}

		if (!(m_formR[m_fN].kind == FM_GRID || m_formR[m_fN].kind == FM_GRIDEX || m_formR[m_fN].kind == FM_TABLE))
		{
			for (kk = 0; kk < ALIGNTBL; kk++)
			{
				if (!tmps.CompareNoCase(alignTbl[kk].strName))
				{
					m_formR[m_fN].align = alignTbl[kk].alignment;
					break;
				}
			}
			if (kk < ALIGNTBL)
				continue;

			for (kk = 0; kk < ATTRTBL; kk++)
			{
				if (!tmps.CompareNoCase(attrTbl[kk].strName))
				{
					m_formR[m_fN].attr |= attrTbl[kk].attributes;
					break;
				}
			}
			if (kk < ATTRTBL)
				continue;
		}

		for (kk = 0; kk < FORMETCTBL; kk++)
		{
			if (!tmps.CompareNoCase(formEtcTbl[kk].strName))
				break;
		}

		if (kk < FORMETCTBL)
		{
			switch (formEtcTbl[kk].flow)
			{
			case 1:		// FONT
				jj++;
				m_formR[m_fN].fName = writeString(token[jj].name);jj++;
				m_formR[m_fN].fPoint = atoi(token[jj].name);	jj++;

				tmps = CString(token[jj].name, token[jj].len);
				for (ll = 0; ll < FSTYLE; ll++)
				{
					if (!tmps.CompareNoCase(fStyle[ll].strName))
						break;
				}
				if (ll >= FSTYLE)
				{
					setMessage(line, 6, tmps, m_fN);
					break;
				}
				m_formR[m_fN].fStyle = fStyle[ll].defValue;
				continue;

			case 2:		// ENABLE
				m_formR[m_fN].properties |= PR_ENABLE;
				continue;

			case 3:		// VISIBLE
				m_formR[m_fN].properties |= PR_VISIBLE;
				continue;

			case 4:		// CHEKCED
				m_formR[m_fN].properties |= PR_CHECKED;
				continue;

			case 5:		// HINTTYPE
				jj++;
				tmps = CString(token[jj].name, token[jj].len);
				for (ll = 0; ll < HINTTBL; ll++)
				{
					if (!tmps.CompareNoCase(hintTbl[ll].strName))
						break;
				}
				if (ll >= FSTYLE)
				{
					setMessage(line, 6, tmps, m_fN);
					break;
				}
				m_formR[m_fN].isHint = hintTbl[ll].type;
				continue;

			case 6:		// FIXEDSIZE
				continue;

			case 7:		// TRANSPARENT
				m_formR[m_fN].properties |= PR_TRANSPARENT;
				continue;

			case 8:		// IMAGE
				m_formR[m_fN].properties |= PR_IMAGE;
				continue;

			case 9:		// HOVER
				m_formR[m_fN].properties |= PR_HOVER;
				continue;

			case 10:	// SORT
				m_formR[m_fN].properties |= PR_SORT;
				continue;

			case 11:	// HAlineS
				m_formR[m_fN].properties |= PR_LINES;
				continue;

			case 12:	// HELPTEXT
				jj++;
				m_formR[m_fN].onHint = writeString(token[jj].name);
				continue;

			case 13:	// VERTICAL
				m_formR[m_fN].properties |= PR_VERT;
				continue;

			case 14:	// IMAGETEXT
				m_formR[m_fN].properties |= PR_IMAGETEXT;
				continue;

			case 15:	// SHOWFORM
				m_formR[m_fN].properties |= PR_EDIT;
				continue;

			case 16:	// ACCCBO
				m_formR[m_fN].properties |= PR_ACCN;
				continue;

			case 17:	// RESIZABLE
				m_formR[m_fN].attr2 |= GO_FIX;
				continue;
			
			case 18:	// GROUPN
				jj++;
				continue;

			case 19:	// TABIMAGE
				jj++;
				m_formR[m_fN].vals[2] = writeString(token[jj].name);
				continue;
			}
		}
		switch(m_formR[m_fN].kind)
		{
		case FM_OUT:		
		case FM_EDIT:
		case FM_COMBO:
		case FM_TAB:
		case FM_UTAB:
		case FM_MEMO:
		case FM_TREE:
		case FM_SHEET:
		case FM_BROWSER:
		case FM_CONTROL:
			for (kk = 0; kk < IOEXT; kk++)
			{
				if (!tmps.CompareNoCase(ioExt[kk].strName))
					break;
			}

			if (kk >= IOEXT)
				break;

// jj 를 먼저증가해야한다.
			switch (ioExt[kk].flow)
			{
			case 1:		// LEN
				if (token[++jj].type != TOK_NUM)
					break;
				m_formR[m_fN].size = atoi(token[jj].name);
				continue;

			case 2:		// COLOR
				if (token[++jj].type != TOK_NUM)
					break;
				m_formR[m_fN].pRGB = atoi(token[jj].name);
				if (token[++jj].type != TOK_NUM)
					break;
				m_formR[m_fN].tRGB = atoi(token[jj].name);
				continue;

			case 3:		// CURSOR
				/*m_mapH.curPos = m_fN;*/
				continue;

			case 4:		// DOM
				jj++;
				CopyMemory(m_domR[m_fN].dSym, token[jj].name, token[jj].len);
				m_domR[m_fN].dSym[token[jj].len] = '\0';
				continue;

			case 5:		// COUNT
				if (token[++jj].type != TOK_NUM)
					break;
				m_formR[m_fN].keys = atoi(token[jj].name);
				continue;

			case 6:		// DISPLAY
				for (kk = 0; kk < LOWORD(m_formR[m_fN].keys); kk++)
				{
					jj++;
					if (kk == 0)
						m_formR[m_fN].vals[1] = writeString(token[jj].name);
					else
						writeString(token[jj].name);
				}
				continue;

			case 10:	// CAPTION
				for (kk = 0; kk < (int)m_formR[m_fN].keys; kk++)
				{
					jj++;
					if (kk == 0)
						m_formR[m_fN].vals[1] = writeString(token[jj].name);
					else
						writeString(token[jj].name);
				}
				continue;

			case 7:		// DATA
				for (kk = 0; kk < LOWORD(m_formR[m_fN].keys); kk++)
				{
					jj++;
					if (kk == 0)
						m_formR[m_fN].vals[0] = writeString(token[jj].name);
					else
						writeString(token[jj].name);
				}
				continue;

			case 11:	// UNIT
			case 14:	// TABINFO
				for (kk = 0; kk < (int)m_formR[m_fN].keys; kk++)
				{
					jj++;
					if (kk == 0)
						m_formR[m_fN].vals[0] = writeString(token[jj].name);
					else
						writeString(token[jj].name);
				}
				continue;

			case 8:		// INIT
			case 21:	// URL
				jj++;
				m_formR[m_fN].vals[0] = writeString(token[jj].name);
				continue;

			case 9:		// FLOAT
				if (token[++jj].type != TOK_NUM)
					break;
				m_formR[m_fN].vals[1] = atoi(token[jj].name);
				continue;

			case 12:	// INDEX
				if (token[++jj].type != TOK_NUM)
					break;
				m_formR[m_fN].checked = atoi(token[jj].name);
				continue;

			case 13:	// OBJECT
				jj++;
				CopyMemory(m_domR[m_fN].rSym, token[jj].name, token[jj].len);
				m_domR[m_fN].rSym[token[jj].len] = '\0';
				continue;

			case 15:	// FOCUS
			case 18:	// MULTISEL
				m_formR[m_fN].properties |= PR_CHECKED;
				continue;

			case 16:	// CHARS
				jj++;
				if (m_formR[m_fN].kind != FM_MEMO)
				{
					jj++;
					continue;
				}
				m_formR[m_fN].keys = atoi(token[jj].name);
				continue;

			case 17:	// REFERENCE
				jj++;
				CopyMemory(m_domR[m_fN].rSym, token[jj].name, token[jj].len);
				m_domR[m_fN].rSym[token[jj].len] = '\0';
				continue;

			case 19:	// DEFAULT
				jj++;
				m_formR[m_fN].vals[1] = writeString(token[jj].name);	jj++;
				m_formR[m_fN].vals[0] = writeString(token[jj].name);
				continue;

			case 20:	// TABH
				if (token[++jj].type != TOK_NUM)
					break;
				m_formR[m_fN].size = atoi(token[jj].name);
				continue;

			case 22:	// SPIN
				m_formR[m_fN].properties |= PR_SPIN;
				jj++;
				m_formR[m_fN].keys = atoi(token[jj].name);
				continue;

			case 23:	// CODE
				m_formR[m_fN].properties |= PR_CODE;
				continue;

			case 24:	// HASTOOL
				m_formR[m_fN].properties |= PR_CHECKED;
				continue;

			case 25:	// SUM
				m_formR[m_fN].properties |= PR_SUM;
				continue;

			case 26:	// FCOLOR
				if (token[++jj].type != TOK_NUM)
					break;
				m_formR[m_fN].vals[3] = writeValue(atoi(token[jj].name));
				if (token[++jj].type != TOK_NUM)
					break;
				writeValue(atoi(token[jj].name));
				continue;

			case 27:	// VCOUNT
				jj++;
				m_formR[m_fN].keys
					= MAKELONG((WORD)m_formR[m_fN].keys, atoi(token[jj].name));
				continue;

			case 28:	// CVAR
				jj++;
				m_formR[m_fN].vals[2] = writeString(token[jj].name);
				continue;
			case 29:	// TABORDER
				jj++;
				if (atoi(token[jj].name))
					m_formR[m_fN].index = atoi(token[jj].name) - 1;
				else
					m_formR[m_fN].index = 0xFFFF;
				continue;
			case 30:	// PAGE
				{
					jj++;
					int pageN = atoi(token[jj++].name);
					_pageR tPage;
					tPage.formN = atoi(token[jj].name);
					for (int nLoop = 0; nLoop < tPage.formN; nLoop++)
					{
						if (nLoop == 0)
							tPage.formR = writeValue(atoi(token[++jj].name));
						else
							writeValue(atoi(token[++jj].name));
					}
					if (pageN == 0)
						m_formR[m_fN].vals[0] = writePage(tPage);
					else
						writePage(tPage);
				}
				continue;
			case 31:	// GROUPN
				jj++;
				continue;
			case 32:	// IOORDER
				jj++;
				if (atoi(token[jj].name))
					m_formR[m_fN].iorder = atoi(token[jj].name) - 1;
				else
					m_formR[m_fN].iorder = 0xFFFF;
				continue;
			case 33:	// MATCHKEY
				jj++;
				tmps = CString(token[jj].name, token[jj].len);
				for (kk = 0; kk < VKTBL; kk++)
				{
					if (!tmps.CompareNoCase(vkTbl[kk].strName))
					{
						if (vkTbl[kk].vkKey == 0)
							m_formR[m_fN].vals[2] = NOVALUE;
						else
							m_formR[m_fN].vals[2] = vkTbl[kk].vkKey;
						break;
					}
				}
				if (kk < VKTBL)
					continue;
				break;
			case 34:	// SPINTYPE
				jj++;
				m_formR[m_fN].vals[2] = atoi(token[jj].name);
				continue;
			case 35:	// ENCRYPT
				jj++;
				m_formR[m_fN].keys = writeString(token[jj].name);
				continue;

			case 36:	// LINECOLOR
				jj++;
				m_formR[m_fN].bRGB = atoi(token[jj].name);
				m_formR[m_fN].iok = PN_LINE;
				continue;
			case 38:	// LIST
				m_formR[m_fN].properties |= PR_LIST;
				continue;
			}
			break;

		case FM_GRID:
		case FM_GRIDEX:
			for (kk = 0; kk < EMODETBL; kk++)
			{
				if (!tmps.CompareNoCase(emodeTbl[kk].strName))
				{
					m_cellR[m_repN].iok = emodeTbl[kk].ioType;
					break;
				}
			}
			if (kk < EMODETBL)
				continue;

		case FM_TABLE:
			for (kk = 0; kk < IOTBL; kk++)
			{
				if (!tmps.CompareNoCase(ioTbl[kk].strName))
				{
					m_cellR[m_repN].type = ioTbl[kk].charType;
					break;
				}
			}
			if (kk < IOTBL)
				continue;

			for (kk = 0; kk < ATTRTBL; kk++)
			{
				if (!tmps.CompareNoCase(attrTbl[kk].strName))
				{
					m_cellR[m_repN].attr |= attrTbl[kk].attributes;
					break;
				}
			}
			if (kk < ATTRTBL)
				continue;

			for (kk = 0; kk < CORGBTBL; kk++)
			{
				if (!tmps.CompareNoCase(coRgbTbl[kk].strName))
				{
					m_cellR[m_repN].attr2 |= coRgbTbl[kk].auxAttributes;
					break;
				}
			}
			if (kk < CORGBTBL)
				continue;
	
			for (kk = 0; kk < ZEROSUPTBL; kk++)
			{
				if (!tmps.CompareNoCase(zerosupTbl[kk].strName))
				{
					m_cellR[m_repN].attr2 |= zerosupTbl[kk].auxAttributes;
					break;
				}
			}

			if (kk < ZEROSUPTBL)
				continue;

			for (kk = 0; kk < DATETBL; kk++)
			{
				if (!tmps.CompareNoCase(dateTbl[kk].strName))
				{
					m_cellR[m_repN].attr2 |= dateTbl[kk].auxAttributes;
					break;
				}
			}
			if (kk < DATETBL)
				continue;

			for (kk = 0; kk < CONTRASTTBL; kk++)
			{
				if (!tmps.CompareNoCase(contrastTbl[kk].strName))
				{
					m_cellR[m_repN].attr2 |= contrastTbl[kk].auxAttributes;
					break;
				}
			}
			if (kk < CONTRASTTBL)
				continue;

			for (kk = 0; kk < COLUMNTBL; kk++)
			{
				if (!tmps.CompareNoCase(columnTbl[kk].strName))
					break;
			}

			switch (columnTbl[kk].flow)
			{
			case 1:		// COLUMN
				if (token[++jj].type != TOK_NUM)
					break;
				m_formR[m_fN].vals[1] = atoi(token[jj].name);
				m_formR[m_fN].vals[1] = m_formR[m_fN].vals[1] << 16;
				continue;

			case 2:		// MATRIX
				if (token[++jj].type != TOK_NUM)
					break;
				rc = atoi(token[jj].name);	jj++;
				if (token[jj].type != TOK_NUM)
					break;
				m_formR[m_fN].vals[1] = atoi(token[jj].name);
				m_formR[m_fN].vals[1] = m_formR[m_fN].vals[1] << 16;
				m_formR[m_fN].vals[1] |= rc;
				continue;

			case 3:		// COLINFO
				m_repN++; jj++;
				if (!compareColumnSymbol(CString(token[jj].name, token[jj].len), icell, m_repN))
				{
					setMessage(line, 14, CString(token[jj].name, token[jj].len), m_fN);
					return;
				}
				m_cellR[m_repN].name = writeString(token[jj].name);
				m_cellR[m_repN].iok  = EIO_OUTPUT;
				continue;

			case 4:		// LEN
				if (token[++jj].type != TOK_NUM)
					break;
				m_cellR[m_repN].size = atoi(token[jj].name);
				continue;

			case 5:		// COLOR
				if (token[++jj].type != TOK_NUM)
					break;
				m_cellR[m_repN].pRGB = atoi(token[jj].name);
				if (token[++jj].type != TOK_NUM)
					break;
				m_cellR[m_repN].tRGB = atoi(token[jj].name);
				continue;

			case 6:		// DOM
				jj++;
				CopyMemory(colDom[m_repN-icell].dSym, token[jj].name, token[jj].len);
				colDom[m_repN-icell].dSym[token[jj].len] = '\0';
				m_cellR[m_repN].keys = 1;
				continue;

			case 7:		// VALID
				if (token[++jj].type != TOK_NUM)
					break;
				m_formR[m_fN].keys = MAKELONG(LOWORD(m_formR[m_fN].keys), atoi(token[jj].name));
				continue;

			case 8:		// HEAD
				jj++;
				m_cellR[m_repN].head = writeString(token[jj].name);
				continue;

			case 9:		// WIDTH
				if (token[++jj].type != TOK_NUM)
					break;
				m_cellR[m_repN].width = atoi(token[jj].name);
				continue;

			case 10:		// HEADCOLOR
				if (token[++jj].type != TOK_NUM)
					break;
				m_cellR[m_repN].hpRGB = atoi(token[jj].name);
				if (token[++jj].type != TOK_NUM)
					break;
				m_cellR[m_repN].htRGB = atoi(token[jj].name);
				continue;

			case 11:		// COLUMN_VISIBLE
				m_cellR[m_repN].properties |= PR_VISIBLE;
				continue;

			case 12:		// MULTI
				m_formR[m_fN].attr2 |= GO_MULTI;
				continue;

			case 13:		// FLOAT
				if (token[++jj].type != TOK_NUM)
					break;
				m_cellR[m_repN].vals[1] = atoi(token[jj].name);
				continue;

			case 14:		// FLEXIBLE
				m_formR[m_fN].attr2 |= GO_FLEX;
				continue;

			case 15:		// VSCROLL
				m_formR[m_fN].attr2 |= GO_VSCR;
				continue;

			case 16:		// HSCROLL
				m_formR[m_fN].attr2 |= GO_HSCR;
				continue;

			case 17:		// FOCUSKIND
				if (token[++jj].type != TOK_NUM)
					break;
				m_formR[m_fN].isEdit = atoi(token[jj].name);
				continue;

			case 18:		// COLALIGN
				for (mm = 0; mm < 2; mm++)
				{
					jj++;
					tmps = CString(token[jj].name, token[jj].len);
					for (ll = 0; ll < ALIGNTBL; ll++)
					{
						if (!tmps.CompareNoCase(alignTbl[ll].strName))
						{
							if (!mm)
								m_cellR[m_repN].alignH = alignTbl[ll].alignment;
							else
								m_cellR[m_repN].alignC = alignTbl[ll].alignment;
							break;
						}
					}
				}
				continue;

			case 19:		// INS_TOP
				m_formR[m_fN].attr2 |= GO_TOP;
				continue;

			case 20:		// NOHEAD
				m_formR[m_fN].attr2 |= GO_XHEAD;
				continue;

			case 21:		// NO3D
				m_formR[m_fN].attr2 |= GO_X3D;
				continue;

			case 22:		// RROW
				m_formR[m_fN].attr2 |= GO_ROW;
				continue;

			case 23:		// XVLINE
				m_formR[m_fN].attr2 |= GO_XVLINE;
				continue;

			case 24:		// XHLINE
				m_formR[m_fN].attr2 |= GO_XHLINE;
				continue;

			case 25:		// LINECOLOR
				if (token[++jj].type != TOK_NUM)
					break;
				m_formR[m_fN].bRGB = atoi(token[jj].name);
				continue;

			case 26:		// LINEW
				if (token[++jj].type != TOK_NUM)
					break;
				m_formR[m_fN].keys = atoi(token[jj].name);
				continue;

			case 27:		// CELLH
				if (token[++jj].type != TOK_NUM)
					break;
				m_formR[m_fN].keys = MAKELONG(atoi(token[jj].name), HIWORD(m_formR[m_fN].keys));
				continue;

			case 28:		// COLUMN_SORT
				m_cellR[m_repN].properties |= PR_SORT;
				continue;

			case 29:		// REFERENCE
				jj++;
				CopyMemory(colDom[m_repN-icell].rSym, token[jj].name, token[jj].len);
				colDom[m_repN-icell].rSym[token[jj].len] = '\0';
				m_cellR[m_repN].vals[0] = 1;
				continue;

			case 30:		// ICOMBO
				m_cellR[m_repN].kind = CK_COMBO;
				continue;

			case 31:		// ICHECK
				m_cellR[m_repN].kind = CK_CHECK;
				continue;

			case 32:		// COUNT
				if (token[++jj].type != TOK_NUM)
					break;
				m_cellR[m_repN].keys = atoi(token[jj].name);
				continue;

			case 33:		// DISPLAY
				for (mm = 0; mm < (int)m_cellR[m_repN].keys; mm++)
				{
					jj++;
					if (mm == 0)
						m_cellR[m_repN].vals[1] = writeString(token[jj].name);
					else
						writeString(token[jj].name);
				}
				continue;

			case 34:		// DATA
				for (mm = 0; mm < (int)m_cellR[m_repN].keys; mm++)
				{
					jj++;
					if (mm == 0)
						m_cellR[m_repN].vals[0] = writeString(token[jj].name);
					else
						writeString(token[jj].name);
				}
				continue;

			case 35:		// CHECKSTR
				jj++;
				m_cellR[m_repN].vals[0] = writeString(token[jj].name);
				continue;

			case 36:		// LEAVE
				m_formR[m_fN].attr |= FA_WRAP;
				continue;

			case 38:		// FIX
				if (token[++jj].type != TOK_NUM)
					break;
				m_formR[m_fN].type = atoi(token[jj].name);
				continue;

			case 39:		// CELLEDIT
				m_formR[m_fN].attr |= FA_ENUM;
				continue;

			case 40:		// VARIANT
				jj++;
				m_formR[m_fN].vals[2] = writeString(token[jj].name);
				continue;

			case 41:		// ROWC
				if (token[++jj].type != TOK_NUM)
					break;
				m_formR[m_fN].vals[3] = writeValue(atoi(token[jj].name));
				if (token[++jj].type != TOK_NUM)
					break;
				writeValue(atoi(token[jj].name));
				continue;

			case 42:		// PAGE
				m_formR[m_fN].attr2 |= GO_PAGE;
				continue;

			case 43:		// HEADER
				m_formR[m_fN].attr2 |= GO_HEADER;
				continue;

			case 44:		// TABLE
				m_formR[m_fN].attr2 |= GO_TABLE;
				continue;

			case 45:		// THUMB
				m_formR[m_fN].attr2 |= GO_THUMB;
				continue;

			case 46:		// CXDELETE
				m_formR[m_fN].attr |= FA_XDELETE;
				continue;
			case 47:		//	TABORDER
				jj++;
				if (atoi(token[jj].name))
					m_formR[m_fN].index = atoi(token[jj].name) - 1;
				else
					m_formR[m_fN].index = 0xFFFF;
				continue;
			case 48:		// EDITFORMAT
				jj++;
				m_cellR[m_repN].onEdit = writeString(token[jj].name);
				m_cellR[m_repN].isEdit = IE_EDIT;
				continue;
			case 49:		// IFRGB
				{
					DWORD upRGB = atoi(token[++jj].name);
					DWORD dnRGB = atoi(token[++jj].name);
					m_cellR[m_repN].onEdit = writeValue(upRGB);
					writeValue(dnRGB);
					m_cellR[m_repN].isEdit = IE_CORGB;
					m_cellR[m_repN].attr |= FA_CORGB;
				}
				continue;
			case 50:		// IOORDER
				jj++;
				if (atoi(token[jj].name))
					m_formR[m_fN].iorder = atoi(token[jj].name) - 1;
				else
					m_formR[m_fN].iorder = 0xFFFF;
				continue;
			
			case 51:		// INORMAL
				m_cellR[m_repN].kind = CK_NORMAL;
				continue;

			case 52:		// IGRAPH
				m_cellR[m_repN].kind = CK_GRAPH;
				continue;
			case 53:		// GROUPN
				//jj++;
				continue;
			case 54:		// AUTOSIZE
				m_formR[m_fN].keys = MAKELONG(0, HIWORD(m_formR[m_fN].keys));
				continue;

			case 55:		// SUPRESS
				m_cellR[m_repN].properties |= PR_SUPRESS;
				continue;
			case 56:		// CUD
				m_formR[m_fN].attr2 |= GO_CUD;
				continue;
			case 57:		// ISFOCUS
				m_formR[m_fN].properties |= PR_CHECKED;
				continue;
			case 58:		// MATCHKEY
				jj++;
				tmps = CString(token[jj].name, token[jj].len);
				for (kk = 0; kk < VKTBL; kk++)
				{
					if (!tmps.CompareNoCase(vkTbl[kk].strName))
					{
						if (vkTbl[kk].vkKey == 0)
							m_cellR[m_repN].vals[2] = NOVALUE;
						else
							m_cellR[m_repN].vals[2] = vkTbl[kk].vkKey;
						break;
					}
				}
				if (kk < VKTBL)
					continue;
				break;
			case 59:		// MERGE
				m_formR[m_fN].attr2 |= GO_MERGE;
				continue;

			case 60:		// IBUTTON
				m_cellR[m_repN].kind = CK_BUTTON;
				continue;

			case 61:		// BUTTONSTR
				jj++;
				m_cellR[m_repN].vals[0] = writeString(token[jj].name);
				continue;

			case 62:		// BUTTONIMG
				jj++;
				m_cellR[m_repN].vals[1] = writeString(token[jj].name);
				continue;

			case 63:		// COLUMN_IMAGE
				m_cellR[m_repN].properties |= PR_IMAGE;
				continue;

			case 64:		// COLUMN_VIMGTEXT
				m_cellR[m_repN].properties |= PR_IMAGETEXT;
				continue;

			case 65:		// FOCUSCOLOR
				if (token[++jj].type != TOK_NUM)
					break;
				m_formR[m_fN].onEdit = atoi(token[jj].name);
				continue;

			case 66:		// KEYFOCUS
				m_formR[m_fN].attr2 |= GO_KEY;
				continue;

			case 67:		// SELALWAYS
				m_formR[m_fN].attr2 |= GO_ALWAYS;
				continue;
			case 68:		// COLUMN_SHOWFORM
				m_cellR[m_repN].properties |= PR_EDIT;
				continue;

			case 69:		// HINTTYPE
				jj++;
				tmps = CString(token[jj].name, token[jj].len);
				for (kk = 0; kk < HINTTBL; kk++)
				{
					if (!tmps.CompareNoCase(hintTbl[kk].strName))
					{
						m_cellR[m_repN].isHint = hintTbl[kk].type;
						break;
					}
				}
				continue;
			case 70:		// COLHELPTEXT
				jj++;
				m_cellR[m_repN].onHint = writeString(token[jj].name);
				continue;
			case 71:		// GRFIX
				m_formR[m_fN].attr2 |= GO_FIX;
				continue;

			case 72:		// GRFLEXEXPAND
				m_formR[m_fN].attr2 |= GO_EXPAND;
				continue;

			case 73:		// COLUMN_EXCHECKE
				m_cellR[m_repN].properties |= PR_CHECKED;
				continue;

			case 74:		// GRXSIZE
				m_formR[m_fN].attr2 |= GO_XSIZE;
				continue;

			case 75:		// GRAUTOWIDTH
				m_formR[m_fN].attr2 |= GO_AUTO;
				continue;

			case 79:		// GRVALID
				m_formR[m_fN].attr2 |= GO_VALID;
				continue;

			case 80:		// GRFTEXT
				m_formR[m_fN].attr2 |= GO_FTEXT;
				continue;

			case 81:		// GRXXSL
				m_formR[m_fN].attr2 |= GO_XXLS;
				continue;

			case 82:		// GRSHARE
				jj++;
				if (token[jj].len)
					m_formR[m_fN].onHint = writeString(token[jj].name);
				continue;

			case 83:		// COLUMN_BOLD
			//	m_cellR[m_repN].properties |= PR_BOLD;
				continue;

			case 84:		// ONLYHEAD3D
			//	m_formR[m_fN].attr2 |= GO_3DHEAD;
				continue;
			case 85:		// FADROP
				m_formR[m_fN].attr |= FA_DROP;
				continue;
			case 86:		// ROWCC
				jj++;
				m_formR[m_fN].vals[0] = writeString(token[jj].name);
				continue;
			}

			if (kk < COLUMNTBL)
				continue;
			break;

		default:
			for (kk = 0; kk < FORMEXT; kk++)
			{
				if (!tmps.CompareNoCase(formExt[kk].strName))
					break;
			}
			if (kk >= FORMEXT)
				break;

			switch (formExt[kk].flow)
			{
			case 1:		// COLOR
				if (token[++jj].type != TOK_NUM)
					break;
				m_formR[m_fN].pRGB = atoi(token[jj].name);
				if (token[++jj].type != TOK_NUM)
					break;
				m_formR[m_fN].tRGB = atoi(token[jj].name);
				continue;

			case 2:		// GROUP
				jj++;
				CopyMemory(m_domR[m_fN].dSym, token[jj].name, token[jj].len);
				m_domR[m_fN].dSym[token[jj].len] = '\0';
				continue;

			case 3:		// FOCUS
				m_formR[m_fN].properties |= PR_CHECKED;
				continue;

			case 4:		// DATE
			case 7:		// LINEW
				jj++;
				m_formR[m_fN].size = atoi(token[jj].name);
				continue;

			case 5:		// KEYMAP
				jj++;
				tmps = CString(token[jj].name, token[jj].len);
				for (kk = 0; kk < VKTBL; kk++)
				{
					if (!tmps.CompareNoCase(vkTbl[kk].strName))
					{
						m_formR[m_fN].keys = vkTbl[kk].vkKey;
						break;
					}
				}
				continue;

			case 6:		// REF
				jj++;
				CopyMemory(m_domR[m_fN].dSym, token[jj].name, token[jj].len);
				m_domR[m_fN].dSym[token[jj].len] = '\0';
				continue;

			case 8:			// LINECOLOR
				jj++;
				m_formR[m_fN].bRGB = atoi(token[jj].name);
				m_formR[m_fN].type = PN_LINE;
				continue;
			case 9:			// TABORDER
				jj++;
				if (token[jj].type != TOK_NUM)
					break;
				if (atoi(token[jj].name))
					m_formR[m_fN].index = atoi(token[jj].name) - 1;
				else
					m_formR[m_fN].index = 0xFFFF;
				continue;
			case 10:		// GROUPN
				//jj++;
				continue;
			case 11:		// IOORDER
				jj++;
				if (token[jj].type != TOK_NUM)
					break;
				if (atoi(token[jj].name))
					m_formR[m_fN].iorder = atoi(token[jj].name) - 1;
				else
					m_formR[m_fN].iorder = 0xFFFF;
				continue;
			}
			break;
		}
		setMessage(line, 6, CString(token[jj].name, token[jj].len), m_fN);
		return;
	}

	if (m_formR[m_fN].kind == FM_GRID || m_formR[m_fN].kind == FM_GRIDEX || m_formR[m_fN].kind == FM_TABLE) // TABLE 추가.
	{
		int	count = (m_formR[m_fN].kind == FM_GRID || m_formR[m_fN].kind == FM_GRIDEX) ? HIWORD(m_formR[m_fN].vals[1])
							: LOWORD(m_formR[m_fN].vals[1]);
		if (count == 0)
		{
			m_formR[m_fN].vals[0] = NOVALUE;
			return;
		}

		int	idx = m_formR[m_fN].vals[0];
		for (jj = 0; jj < count; jj++)
		{
			if (m_cellR[idx+jj].kind == CK_COMBO)
				continue;

			if (m_cellR[idx+jj].keys == 1)
				m_cellR[idx+jj].keys = getColumnIndex(colDom[jj].dSym, (int)strlen(colDom[jj].dSym), m_fN);
			else
				m_cellR[idx+jj].keys = NOVALUE;
		}
		for (jj = 0; jj < count; jj++)
		{
			if (m_cellR[idx+jj].attr & FA_FLOAT
				|| m_cellR[idx+jj].kind == CK_COMBO || m_cellR[idx+jj].kind == CK_CHECK || m_cellR[idx+jj].kind == CK_BUTTON)
				continue;

			if ((m_cellR[idx+jj].attr & FA_REF) && (m_cellR[idx+jj].vals[0] == 1))
				m_cellR[idx+jj].vals[0] = getColumnIndex(colDom[jj].rSym, (int)strlen(colDom[jj].rSym), m_fN);
			else
				m_cellR[idx+jj].vals[0] = NOVALUE;
		}
	}

}

void CCompile::buildResourceStr(CMapToken* token, int tokCnt, int line)
{
	int	ii, rc,  nTok, fmPos;
	CString	tmps, symbol;

	nTok = 0;
	tmps = CString(token[nTok].name, token[nTok].len);
	
	for (ii = 0; ii < SCRIPTTBL; ii++)
	{
		if (!tmps.CompareNoCase(scriptTbl[ii].strName))
			break;
	}

	if (ii < SCRIPTTBL)
	{
		switch (scriptTbl[ii].flow)
		{
		case 1:		// SCRIPT Symbol Check
			nTok++; tmps = _T("");
			m_symbol = CString(token[nTok].name, token[nTok].len);
			rc = m_symbol.Find('.');
			if (rc != -1)
			{
				tmps = m_symbol.Mid(rc+1);
				m_symbol = m_symbol.Left(rc);
			}
			fmPos = getFormIndex((char *)m_symbol.operator LPCTSTR(), m_symbol.GetLength());
			if (fmPos == -1)
			{
				setMessage(line, 10, CString(token[nTok].name, token[nTok].len));
				return;
			}

			if (!tmps.IsEmpty() && (m_formR[fmPos].kind == FM_GRID || m_formR[fmPos].kind == FM_GRIDEX || m_formR[fmPos].kind == FM_TABLE))
			{
				if (getColumnIndex((char *)tmps.operator LPCTSTR(), tmps.GetLength(), fmPos) == -1)
				{
					setMessage(line, 10, CString(token[nTok].name, token[nTok].len), fmPos);
					return;
				}
			}
			break;

		case 2:		// ENDSCRIPT
			break;
		
		case 3:		// ONSTART
			writeScript(_T("Sub AX_SUB_OnStart_AX_()"));
			break;
		
		case 4:		// ONSEND
			writeScript(_T("Sub AX_SUB_OnSend_AX_()"));
			break;
		
		case 5:		// ONRECEIVE
			writeScript(_T("Sub AX_SUB_OnReceive_AX_()"));
			break;

		case 6:		// ONALERT
			writeScript(_T("Sub AX_SUB_OnAlert_AX_(code, data, state)"));
			break;

		case 7:		// ONSERVICE
			writeScript(_T("Sub AX_SUB_OnService_AX_(data, length)"));
			break;

		case 8:		// ONFILE
			writeScript(_T("Sub AX_SUB_OnFile_AX_(upload, data, length)"));
			break;

		case 9:		// ONSELECT
			writeScript(_T("Sub AX_SUB_OnSelect_AX_(data)"));
			break;

		case 10:		// ONTIMER
			writeScript(_T("Sub AX_SUB_OnTimer_AX_()"));
			break;

		case 11:		// ONFOCUS
			writeScript(_T("Sub AX_SUB_OnFocus_AX_()"));
			break;

		case 12:		// ONCLOSE
			writeScript(_T("Sub AX_SUB_OnClose_AX_()"));
			break;

		case 13:		// ONDEVICE
			writeScript(_T("Sub AX_SUB_OnDevice_AX_()"));
			break;

		case 14:		// ONAPPROVE
			writeScript(_T("Sub AX_SUB_OnApprove_AX_(key, data, length)"));
			break;
		
		case 15:		// ONKEY
			writeScript(_T("Sub AX_SUB_OnKey_AX_()"));
			break;

		case 16:	// ONTRIGGER
			writeScript(_T("Sub AX_SUB_OnTrigger_AX_()"));
			break;
		case 17:	// ONENDSUB
			writeScript(_T("End Sub"));
			break;
		case 18:	// ONDECLARE
			break;
		case 19:	// ONINDECLARE
			writeScript(_T("Sub AX_SUB_include_AX_()"));
			break;
		case 20:	// ONENDFUNC
			writeScript(_T("End Function"));
			break;
		}
		return;
	}
		
	for (ii = 0; ii < FEVENTTBL; ii++)
	{
		if (!tmps.CompareNoCase(fEventTbl[ii].strName))
			break;
	}
	
	if (ii < FEVENTTBL)
	{
		switch (fEventTbl[ii].flow)
		{
		case 1: 		// OnEdit : MANIE
			return;
		case 2:			// onClick
			symbol = "Sub AX_" + m_symbol + "_OnClick_AX_()";
			writeScript(symbol);
			return;
			
		case 3:			// onDblClk
			symbol = "Sub AX_" + m_symbol + "_OnDblClick_AX_()";
			writeScript(symbol);
			return;
			
		case 4:			// onChange
			symbol = "Sub AX_" + m_symbol + "_OnChange_AX_()";
			writeScript(symbol);
			return;
			
		case 5:			// onChar
			symbol = "Sub AX_" + m_symbol + "_OnCharChange_AX_()";
			writeScript(symbol);
			return;

		case 6:			// onKey
			symbol = "Sub AX_" + m_symbol + "_OnKey_AX_()";
			writeScript(symbol);
			return;
			
		default:
			break;
		}
	}

	writeScript(tmps);
}

int CCompile::writeString(CString str)
{
	int	offset = m_strN;

	CopyMemory(&m_strR[offset], str.operator LPCTSTR(), str.GetLength());
	m_strN += str.GetLength();
	m_strR[m_strN++] = 0x00;

	return offset;
}
 
int CCompile::writeValue(DWORD value)
{
	DWORD*	buf = new DWORD[m_valueN + 1];
	if (m_valueN)
	{
		CopyMemory(buf, m_valueR, sizeof(DWORD) * m_valueN);
		delete[] m_valueR;
	}
	buf[m_valueN] = value;
	m_valueR = buf;
	//m_valueN++;
	return m_valueN++;
}

int CCompile::writePage(_pageR pageR)
{
	_pageR*	buf = new _pageR[m_pageN + 1];
	if (m_pageN)
	{
		CopyMemory(buf, m_pageR, m_pageN * sizeof(_pageR));
		delete[] m_pageR;
	}
	buf[m_pageN] = pageR;
	m_pageR = buf;
	//m_pageN++;
	return m_pageN++;
}

int CCompile::writeScript(CString str)
{
	int	offset = m_scriptN;

	CopyMemory(&m_scriptR[offset], str.operator LPCTSTR(), str.GetLength());
	m_scriptN += str.GetLength();
	m_scriptR[m_scriptN++] = '\n';

	return offset;
}

int CCompile::getFormIndex(char* symbol, int len)
{
	CString	string;

	string = CString(symbol, len);
	for (int ii = 0; ii < m_fN; ii++)		// form symbol check
	{
		if (string.CompareNoCase(&m_strR[m_formR[ii].name]))
			continue;
		else
			return ii;
	}
	return -1;
}
