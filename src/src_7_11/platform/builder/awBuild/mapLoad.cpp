// mapLoad.cpp: implementation of the CMapLoad class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "awBuild.h"
#include "mapLoad.h"
#include "maptbl.hxx"

#include "../mapvar.h"
#include "../../h/axisvar.h"
#include "../awCommon/CellItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapLoad::CMapLoad()
{
	m_pFormItem = NULL;
}

CMapLoad::~CMapLoad()
{
	if (m_pFormItem)
		delete m_pFormItem;
}

void CMapLoad::Load(struct _mapH* mapH, CString curdir)
{
	m_mapH  = mapH;
	
	if (m_pFormItem)
		delete m_pFormItem;
	m_pFormItem = new CFormItem(mapH);

	m_rtype  = 0;
	m_script = _T("");
	m_curdir = curdir;
	m_tabPageArr.RemoveAll();
	m_groupArr.RemoveAll();
}

void CMapLoad::BuildHeader(CMapToken* token, int count)
{
	int	tokN = 0;
	CString tmps;

	tmps = CString(token[tokN].name, token[tokN].len);
	int ii = 0;
	for (ii = 0; ii < GLOBALSYM; ii++)
	{
		if (!tmps.CompareNoCase(globalSym[ii].strName))
			break;
	}

	if (ii >= GLOBALSYM)
		return;

	tokN++;
	if (!strlen(token[tokN].name))
		return;

	switch (globalSym[ii].flow)
	{
	case 1:					// MAPNAME
		CopyMemory(m_mapH->mapN, token[tokN].name, token[tokN].len);
		break;
	case 2:					// MAPSIZE
		m_mapH->height = atoi(token[tokN].name);	tokN++;
		m_mapH->width  = atoi(token[tokN].name);
		break;
	case 3:					// CAPTION
		CopyMemory(m_mapH->caption, token[tokN].name,token[tokN].len);
		m_mapH->caption[token[tokN].len] = '\0';
		break;
	case 4:					// FONT
		CopyMemory(m_mapH->fName, token[tokN].name,token[tokN].len);
		m_mapH->fName[token[tokN].len] = '\0';	tokN++;
		m_mapH->fPoint = (BYTE)(atoi(token[tokN].name));	tokN++;
		tmps = CString(token[tokN].name, token[tokN].len);
		for (ii = 0; ii < FSTYLE; ii++)
		{
			if (!tmps.CompareNoCase(fStyle[ii].strName))
				break;
		}
		if (ii >= FSTYLE)
			break;
		m_mapH->fStyle = BYTE(fStyle[ii].defValue);
		break;
	case 5:					// TRXC
		CopyMemory(m_mapH->trxC, token[tokN].name, token[tokN].len);	
		break;
	case 6:					// TOPATH
		tmps = CString(token[tokN].name, token[tokN].len);
		for (ii = 0; ii < TOPATH; ii++)
		{
			if (!tmps.CompareNoCase(toPath[ii].strName))
				break;
		}
		if (ii >= TOPATH)
			break;
		m_mapH->toPath = toPath[ii].defValue;
		break;
	case 7:					// METHOD
		tmps = CString(token[tokN].name, token[tokN].len);
		for (ii = 0; ii < MAPMETHOD; ii++)
		{
			if (!tmps.CompareNoCase(mapMethod[ii].strName))
				break;
		}
		if (ii >= MAPMETHOD)
			break;
		m_mapH->mapM = mapMethod[ii].defValue;
		break;
	case 8:					// HEADER
		m_mapH->typeH = 0;
		tmps = CString(token[tokN].name, token[tokN].len);
		for (ii = 0; ii < HEADERS; ii++)
		{
			if (!tmps.CompareNoCase(headers[ii].strName))
				break;
		}
		if (ii >= HEADERS)
			break;
		m_mapH->typeH = headers[ii].defValue;
		break;
	case 9:					// OPTIONS
		m_mapH->options = 0;
		for (ii = tokN; ii < count; ii++)
		{
			tmps = CString(token[ii].name, token[ii].len);
			int jj = 0;
			for (jj = 0; jj < OPTIONS; jj++)
			{
				if (!tmps.CompareNoCase(options[jj].strName))
					break;
			}
			if (jj >= OPTIONS)
				continue;

			m_mapH->options |= options[jj].defValue;
		}
		tokN = ii;
		break;
	case 10:					// REPEATTIME
		m_mapH->repeatV = atoi(token[tokN].name);
		break;
	case 11:					// WAITTIMEOUT
		m_mapH->waitV = atoi(token[tokN].name);
		break;
	case 12:					// HELPN
		CopyMemory(m_mapH->helpN, token[tokN].name, min(token[tokN].len, 128));
		m_mapH->helpN[min(token[tokN].len, 128)] = 0x00;
		break;
	case 13:					// VERS
		m_mapH->vers = (BYTE)atoi(token[tokN].name);
		break;
	case 14:					// MAPDESC
		CopyMemory(m_mapH->mapdesc, token[tokN].name, min(token[tokN].len, 19));
		if (token[tokN].len > 19)
			m_mapH->mapdesc[20] = NULL;
		break;
	case 15:					// TRXH
		CopyMemory(m_mapH->trxH, token[tokN].name, token[tokN].len);	
		break;
	case 16:					// MAPKIND
		tmps = CString(token[tokN].name, token[tokN].len);
		for (ii = 0; ii < MAPKIND; ii++)
		{
			if (!tmps.CompareNoCase(mapKind[ii].strName))
				break;
		}
		if (ii >= MAPKIND)
			break;
		m_mapH->mapK = mapKind[ii].defValue;
		break;
	case 17:					// BGROUND
		tmps = CString(token[tokN].name, token[tokN].len);
		for (ii = 0; ii < BGROUND; ii++)
		{
			if (!tmps.CompareNoCase(bground[ii].strName))
				break;
		}
		if (ii >= BGROUND)
			break;
		m_mapH->imageK = bground[ii].defValue;
		tokN++;
		if (m_mapH->imageK == IK_BMP)
		{
			if (token[tokN].name.GetLength() < sizeof(m_mapH->imageV))
				CopyMemory(m_mapH->imageV, token[tokN].name, 64);
		}
		else if (m_mapH->imageK == IK_BRUSH)
			m_mapH->bkClr = atoi(token[tokN].name);
		break;
	case 18:					// MAPGROUP
		m_mapH->mapGroup = (BYTE)atoi(token[tokN].name);
		break;
	case 19:					// MAPTYPE
		m_mapH->type = 0;
		tmps = CString(token[tokN].name, token[tokN].len);
		for (ii = 0; ii < MAPTYPE; ii++)
		{
			if (!tmps.CompareNoCase(mapType[ii].strName))
				break;
		}
		if (ii >= MAPTYPE)
			break;
		m_mapH->type = mapType[ii].defValue;
		break;
	case 20:					// MAPPUBLIC
		if (token[tokN].len == 1 && strncmp(token[tokN].name, ";", 1) == 0)
			strcpy_s(m_mapH->publicR, 2, "");
		else
			strcpy_s(m_mapH->publicR, 512, token[tokN].name);
		break;
	}
}

void CMapLoad::BuildForm(CMapToken* token, int count, int& fN)
{
	int	tokN, kk, ll, mm;
	int	repN, multi, offs;
	CRect	tRc;
	CString	tmps;
	struct	_repR*	repR;
	CCellItem *cellItem = NULL;

	tokN = 0;
	tmps = CString(token[tokN].name, token[tokN].len);

	int ii = 0;
	for (ii = 0; ii < FORMTBL; ii++)
	{
		if (!tmps.CompareNoCase(formTbl[ii].strName))
			break;
	}
	if (ii >= FORMTBL)
		return;

	tokN++;
	while(fN >= m_pFormItem->getFormCount())
		m_pFormItem->addForm();
	m_pFormItem->getFormPoint(fN)->kind = formTbl[ii].kind;
	if (m_pFormItem->getFormPoint(fN)->kind == FM_GRID || m_pFormItem->getFormPoint(fN)->kind == FM_TABLE)
	{
		repN = -1;
		multi = 0;
		m_pFormItem->getFormPoint(fN)->auxR = new char[sizeof(_repR)];
		ZeroMemory(m_pFormItem->getFormPoint(fN)->auxR, sizeof(_repR));
		repR = (struct _repR *)m_pFormItem->getFormPoint(fN)->auxR;
		repR->selected = -1;
		cellItem = new CCellItem(repR);
		m_pFormItem->getFormPoint(fN)->pRGB = 7;
	}

	if (m_pFormItem->getFormPoint(fN)->kind == FM_PANEL) 
		m_pFormItem->getFormPoint(fN)->type = PN_NONE;
	if (m_pFormItem->getFormPoint(fN)->kind == FM_OUT) 
		m_pFormItem->getFormPoint(fN)->iok = PN_NONE;
	if (m_pFormItem->getFormPoint(fN)->kind == FM_PANEL || m_pFormItem->getFormPoint(fN)->kind == FM_BUTTON) 
		m_pFormItem->getFormPoint(fN)->alignment = AL_CENTER;
	
	// NAME
	m_pFormItem->setFormStr(fN, "name", CString(token[tokN].name, token[tokN].len));
	tokN++;

	for (int jj = 0; jj < 4; jj++)			// RECT
	{
		if (token[tokN].type != TOK_NUM)
			return;

		switch (jj)
		{
		case 0:
			tRc.left   = atoi(token[tokN].name);	break;
		case 1:
			tRc.top    = atoi(token[tokN].name);	break;
		case 2:
			tRc.right  = atoi(token[tokN].name);	break;
		case 3:
			tRc.bottom = atoi(token[tokN].name);	break;
		}
		tokN++;
	}

	tRc.NormalizeRect();
	m_pFormItem->getFormPoint(fN)->left   = tRc.left;
	m_pFormItem->getFormPoint(fN)->top    = tRc.top;
	m_pFormItem->getFormPoint(fN)->right  = tRc.right;
	m_pFormItem->getFormPoint(fN)->bottom = tRc.bottom;

	switch (m_pFormItem->getFormPoint(fN)->kind)
	{
	case FM_SIZE:
		break;
	default:
		if (m_pFormItem->getFormPoint(fN)->left == m_pFormItem->getFormPoint(fN)->right)
			m_pFormItem->getFormPoint(fN)->right = m_pFormItem->getFormPoint(fN)->left + 120;
		if (m_pFormItem->getFormPoint(fN)->top == m_pFormItem->getFormPoint(fN)->bottom)
			m_pFormItem->getFormPoint(fN)->bottom = m_pFormItem->getFormPoint(fN)->top + 40;
		break;
	}


	int jj = 0;
	switch (m_pFormItem->getFormPoint(fN)->kind)
	{
	case FM_EDIT:
	case FM_COMBO:
	case FM_OUT:
	case FM_MEMO:
	case FM_SHEET:
	case FM_TREE:
		for (jj = tokN; jj < count; jj++)
		{
			tmps = CString(token[jj].name, token[jj].len);
			for (kk = 0; kk < IOTBL; kk++)
			{
				if (!tmps.CompareNoCase(ioTbl[kk].strName))
				{
					m_pFormItem->getFormPoint(fN)->type = ioTbl[kk].type;
					token[jj].type = TOK_USE;
					break;
				}
			}

			if (kk >= IOTBL)
				continue;
			break;
		}
		if (jj >= count)
			return;
		break;
	default:
		break;
	}

	for (jj = tokN; jj < count; jj++)
	{
		if (token[jj].type == TOK_USE)
			continue;

		if (token[jj].type == TOK_TEXT)
		{
			m_pFormItem->setFormStr(fN, "str", CString(token[jj].name, token[jj].len));
			m_pFormItem->getFormPoint(fN)->size = token[jj].len;
			continue;
		}

		if (token[jj].type == TOK_FILE)
		{
			switch(m_pFormItem->getFormPoint(fN)->kind)
			{
			case FM_PANEL:
			case FM_BUTTON:
			case FM_AVI:
				m_pFormItem->setFormStr(fN, "str2", CString(token[jj].name, token[jj].len));
				m_pFormItem->getFormPoint(fN)->size = token[jj].len;
				continue;
			case FM_OBJECT:
			case FM_GRAPH:
				m_pFormItem->setFormStr(fN, "str", CString(token[jj].name, token[jj].len));
				continue;
			case FM_CONTROL:
				m_pFormItem->setFormStr(fN, "str", CString(token[jj].name, token[jj].len));
				jj++;
				m_pFormItem->setFormStr(fN, "str2", CString(token[jj].name, token[jj].len));
				continue;
			default:
				return;
			}
		}

		tmps = CString(token[jj].name, token[jj].len);
		switch(m_pFormItem->getFormPoint(fN)->kind)
		{
		case FM_COMBO:
			for (kk = 0; kk < PANELTBL; kk++)
			{
				if (!tmps.CompareNoCase(panelTbl[kk].strName))
				{
					m_pFormItem->getFormPoint(fN)->borders = panelTbl[kk].borders;
					break;
				}
			}
			if (kk < PANELTBL)
				continue;

			for (kk = 0; kk < EMODETBL; kk++)
			{
				if (!tmps.CompareNoCase(emodeTbl[kk].strName))
				{
					m_pFormItem->getFormPoint(fN)->iok = emodeTbl[kk].iok;
					break;
				}
			}
			if (kk < EMODETBL)
				continue;

			for (kk = 0; kk < ASENDTBL; kk++)
			{
				if (!tmps.CompareNoCase(asendTbl[kk].strName))
				{
					m_pFormItem->getFormPoint(fN)->attr2 |= asendTbl[kk].attr2;
					break;
				}
			}
			if (kk < ASENDTBL)
				continue;

			for (kk = 0; kk < IETBL; kk++)
			{
				if (!tmps.CompareNoCase(ieTbl[kk].strName))
				{
					m_pFormItem->getFormPoint(fN)->isEdit = ieTbl[kk].isEdit;
					jj++;
					switch (ieTbl[kk].isEdit)
					{
					case IE_CORGB:
						break;
					case IE_EDIT:
					default:
						m_pFormItem->setFormStr(fN, "editS", CString(token[jj].name, token[jj].len));
						break;
					}
					break;
				}
			}
			if (kk < IETBL)
				continue;
			break;

		case FM_EDIT:
		case FM_OUT:
			for (kk = 0; kk < ZEROSUPTBL; kk++)
			{
				if (!tmps.CompareNoCase(zerosupTbl[kk].strName))
				{
					m_pFormItem->getFormPoint(fN)->attr2 |= zerosupTbl[kk].attr2;
					break;
				}
			}
			if (kk < ZEROSUPTBL)
				continue;

			for (kk = 0; kk < CORGBTBL; kk++)
			{
				if (!tmps.CompareNoCase(coRgbTbl[kk].strName))
				{
					m_pFormItem->getFormPoint(fN)->attr2 |= coRgbTbl[kk].attr2;
					break;
				}
			}
			if (kk < CORGBTBL)
				continue;

			for (kk = 0; kk < ASENDTBL; kk++)
			{
				if (!tmps.CompareNoCase(asendTbl[kk].strName))
				{
					m_pFormItem->getFormPoint(fN)->attr2 |= asendTbl[kk].attr2;
					break;
				}
			}
			if (kk < ASENDTBL)
				continue;

			for (kk = 0; kk < CONTRASTTBL; kk++)
			{
				if (!tmps.CompareNoCase(contrastTbl[kk].strName))
				{
					m_pFormItem->getFormPoint(fN)->attr2 |= contrastTbl[kk].attr2;
					break;
				}
			}
			if (kk < CONTRASTTBL)
				continue;

			for (kk = 0; kk < DATETBL; kk++)
			{
				if (!tmps.CompareNoCase(dateTbl[kk].strName))
				{
					m_pFormItem->getFormPoint(fN)->attr2 |= dateTbl[kk].attr2;
					break;
				}
			}
			if (kk < DATETBL)
				continue;

			for (kk = 0; kk < PANELTBL; kk++)
			{
				if (!tmps.CompareNoCase(panelTbl[kk].strName))
				{
					m_pFormItem->getFormPoint(fN)->borders = panelTbl[kk].borders;
					if (panelTbl[kk].borders != BD_NONE && 
						m_pFormItem->getFormPoint(fN)->kind != FM_EDIT)
						m_pFormItem->getFormPoint(fN)->iok     = PN_BORDER;
					break;
				}
			}
			if (kk < PANELTBL)
				continue;

			for (kk = 0; kk < IETBL; kk++)
			{
				if (!tmps.CompareNoCase(ieTbl[kk].strName))
				{
					m_pFormItem->getFormPoint(fN)->isEdit = ieTbl[kk].isEdit;
					jj++;
					WORD	upRGB,  dnRGB;
					switch (ieTbl[kk].isEdit)
					{
					case IE_EDIT:
						m_pFormItem->setFormStr(fN, "editS", CString(token[jj].name, token[jj].len));
						break;
					case IE_CORGB:
						upRGB = atoi(token[jj].name);	jj++;
						dnRGB = atoi(token[jj].name);
						m_pFormItem->getFormPoint(fN)->onEdit = MAKELONG(dnRGB, upRGB);
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
					m_pFormItem->getFormPoint(fN)->iok = emodeTbl[kk].iok;
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
					m_pFormItem->getFormPoint(fN)->iok = emodeTbl[kk].iok;
					break;
				}
			}
			if (kk < EMODETBL)
				continue;
			
			for (kk = 0; kk < PANELTBL; kk++)
			{
				if (!tmps.CompareNoCase(panelTbl[kk].strName))
				{
					m_pFormItem->getFormPoint(fN)->borders = panelTbl[kk].borders;
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
					m_pFormItem->getFormPoint(fN)->type = sheetStyleTbl[kk].type;
					break;
				}
			}
			if (kk < SHEETSTYLETBL)
				continue;

			for (kk = 0; kk < EMODETBL; kk++)
			{
				if (!tmps.CompareNoCase(emodeTbl[kk].strName))
				{
					m_pFormItem->getFormPoint(fN)->iok = emodeTbl[kk].iok;
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
					m_pFormItem->getFormPoint(fN)->type = sysTbl[kk].type;
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
					m_pFormItem->getFormPoint(fN)->type = boxTbl[kk].type;
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
					m_pFormItem->getFormPoint(fN)->borders = panelTbl[kk].borders;
					m_pFormItem->getFormPoint(fN)->type    = PN_BORDER;
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
					m_pFormItem->getFormPoint(fN)->iok = emodeTbl[kk].iok;
					break;
				}
			}
			if (kk < EMODETBL)
				continue;

			for (kk = 0; kk < BUTTONTBL; kk++)
			{
				if (!tmps.CompareNoCase(buttonTbl[kk].strName))
				{
					m_pFormItem->getFormPoint(fN)->type = buttonTbl[kk].type;
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
					m_pFormItem->getFormPoint(fN)->type = tabStyleTbl[kk].type;
					break;
				}
			}
			if (kk < TABSTYLETBL)
				continue;
			break;

		default:
			break;
		}

		if (!(m_pFormItem->getFormPoint(fN)->kind == FM_GRID || m_pFormItem->getFormPoint(fN)->kind == FM_TABLE))
		{
			for (kk = 0; kk < ALIGNTBL; kk++)
			{
				if (!tmps.CompareNoCase(alignTbl[kk].strName))
				{
					if (alignTbl[kk].alignment <= AL_BOTTOM)
						m_pFormItem->getFormPoint(fN)->alignment = alignTbl[kk].alignment;
					else
						m_pFormItem->getFormPoint(fN)->alignImage = alignTbl[kk].alignment;
					continue;
				}
			}
			if (kk < ALIGNTBL)
				continue;

			for (kk = 0; kk < ATTRTBL; kk++)
			{
				if (!tmps.CompareNoCase(attrTbl[kk].strName))
				{
					m_pFormItem->getFormPoint(fN)->attr |= attrTbl[kk].attr;
					continue;
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
				m_pFormItem->setFormStr(fN, "fName", CString(token[jj].name, token[jj].len));
				jj++;
				m_pFormItem->getFormPoint(fN)->fPoint = atoi(token[jj].name);	jj++;

				tmps = CString(token[jj].name, token[jj].len);
				for (ll = 0; ll < FSTYLE; ll++)
				{
					if (!tmps.CompareNoCase(fStyle[ll].strName))
						break;
				}
				if (ll >= FSTYLE)
					break;
				m_pFormItem->getFormPoint(fN)->fStyle = fStyle[ll].defValue;
				continue;	

			case 2:		// ENABLE
				m_pFormItem->getFormPoint(fN)->properties |= PR_ENABLE;
				continue;

			case 3:		// VISIBLE
				m_pFormItem->getFormPoint(fN)->properties |= PR_VISIBLE;
				continue;

			case 4:		// CHEKCED
				m_pFormItem->getFormPoint(fN)->properties |= PR_CHECKED;
				continue;

			case 5:		// HINTTYPE
				jj++;
				tmps = CString(token[jj].name, token[jj].len);
				for (ll = 0; ll < HINTTBL; ll++)
				{
					if (!tmps.CompareNoCase(hintTbl[ll].strName))
						break;
				}
				if (ll >= HINTTBL)
					break;
				m_pFormItem->getFormPoint(fN)->isHint = hintTbl[ll].type;
				continue;

			case 6:		// FIXEDSIZE
				m_pFormItem->getFormPoint(fN)->bFixedSize = true;
				continue;

			case 7:		// TRANSPARENT
				m_pFormItem->getFormPoint(fN)->properties |= PR_TRANSPARENT;
				continue;

			case 8:		// IMAGE
				m_pFormItem->getFormPoint(fN)->properties |= PR_IMAGE;
				continue;

			case 9:		// HOVER
				m_pFormItem->getFormPoint(fN)->properties |= PR_HOVER;
				continue;

			case 10:	// SORT
				m_pFormItem->getFormPoint(fN)->properties |= PR_SORT;
				continue;

			case 11:	// HASLINES
				m_pFormItem->getFormPoint(fN)->properties |= PR_LINES;
				continue;

			case 12:	// HELPTEXT
				jj++;
				m_pFormItem->setFormStr(fN, "help", CString(token[jj].name, token[jj].len));
				continue;

			case 13:	// VERTICAL
				m_pFormItem->getFormPoint(fN)->properties |= PR_VERT;
				continue;

			case 14:	// IMAGETEXT
				m_pFormItem->getFormPoint(fN)->properties |= PR_IMAGETEXT;
				continue;

			case 15:	// SHOWFORM
				m_pFormItem->getFormPoint(fN)->properties |= PR_EDIT;
				continue;

			case 16:	// ACCCBO
				m_pFormItem->getFormPoint(fN)->properties |= PR_ACCN;
				continue;

			case 17:	// RESIZABLE
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_FIX;
				continue;

			case 18:	// GROUPN
				jj++;
				m_groupArr.SetAt(fN, atoi(token[jj].name));
				continue;

			case 19:	// TABIMAGE
				jj++;
				m_pFormItem->setFormStr(fN, "str2", CString(token[jj].name, token[jj].len));
				continue;

			}
		}

		switch (m_pFormItem->getFormPoint(fN)->kind)
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

			// jj �� ���������ؾ��Ѵ�.
			switch (ioExt[kk].flow)
			{
			case 1:		// LEN
				if (token[++jj].type != TOK_NUM)
					break;
				m_pFormItem->getFormPoint(fN)->size = (WORD)atoi(token[jj].name);
				continue;

			case 2:		// COLOR
				if (token[++jj].type != TOK_NUM)
					break;
				m_pFormItem->getFormPoint(fN)->pRGB = atoi(token[jj].name);
				if (token[++jj].type != TOK_NUM)
					break;
				m_pFormItem->getFormPoint(fN)->tRGB = atoi(token[jj].name);
				continue;

			case 3:		// CURSOR
				m_pFormItem->getFormPoint(fN)->initC = true;
				continue;

			case 4:		// DOM
				jj++;
				m_pFormItem->setFormStr(fN, "auxS", CString(token[jj].name, token[jj].len));
				continue;

			case 5:		// COUNT
				if (token[++jj].type != TOK_NUM)
					break;
				m_pFormItem->getFormPoint(fN)->keys = (BYTE)atoi(token[jj].name);
				continue;

			case 6:		// DISPLAY
				if (LOWORD(m_pFormItem->getFormPoint(fN)->keys) <= 0)
					continue;

				{
					CString data = _T("");
					for (kk = 0; kk < LOWORD(m_pFormItem->getFormPoint(fN)->keys); kk++)
					{
						jj++;
						data += CString(token[jj].name, token[jj].len);
						data += "\t";
					}
					m_pFormItem->setFormStr(fN, "dat", data);
				}
				continue;

			case 10:	// CAPTION
				{
					if (m_pFormItem->getFormPoint(fN)->keys <= 0)
						continue;
					
					CString temp = _T("");
					for (kk = 0; kk < (int)m_pFormItem->getFormPoint(fN)->keys; kk++)
					{
						jj++;
						temp += CString(token[jj].name, token[jj].len);
						temp += '\t';
					}
					m_pFormItem->setFormStr(fN, "dat", temp);
				}
				continue;

			case 7:		// DATA
				{
					CString data = _T("");
					for (kk = 0; kk < LOWORD(m_pFormItem->getFormPoint(fN)->keys); kk++)
					{
						jj++;
						data += CString(token[jj].name, token[jj].len);
						data += "\t";
					}
					m_pFormItem->setFormStr(fN, "str", data);
				}
				continue;

			case 8:		// INIT
			case 21:	// URL
				jj++;
				m_pFormItem->setFormStr(fN, "str", CString(token[jj].name, token[jj].len));
				continue;

			case 9:		// FLOAT
				if (token[++jj].type != TOK_NUM)
					break;
				m_pFormItem->getFormPoint(fN)->offs = (BYTE)atoi(token[jj].name);
				continue;

			case 11:	// UNIT
				{
					CString temp = _T("");
					for (kk = 0; kk < (int)m_pFormItem->getFormPoint(fN)->keys; kk++)
					{
						jj++;
						temp += CString(token[jj].name, token[jj].len);
						temp += '\t';
					}
					m_pFormItem->setFormStr(fN, "str", temp);
				}
				continue;

			case 12:	// INDEX
				if (token[++jj].type != TOK_NUM)
					break;
				m_pFormItem->getFormPoint(fN)->checked = (BYTE)atoi(token[jj].name);
				continue;

			case 13:	// OBJECT
				jj++;
				m_pFormItem->setFormStr(fN, "auxS", CString(token[jj].name, token[jj].len));
				continue;

			case 14:	// TABINFO
				{
					CString temp = _T("");
					for (kk = 0; kk < (int)m_pFormItem->getFormPoint(fN)->keys; kk++)
					{
						jj++;
						temp += CString(token[jj].name, token[jj].len);
						temp += '\t';
					}
					m_pFormItem->setFormStr(fN, "str", temp);
				}
				continue;

			case 15:	// FOCUS
			case 18:	// MULTISEL
				m_pFormItem->getFormPoint(fN)->properties |= PR_CHECKED;
				continue;

			case 16:	// CHARS
				jj++;
				if (m_pFormItem->getFormPoint(fN)->kind != FM_MEMO)
				{
					jj++;
					continue;
				}
				m_pFormItem->getFormPoint(fN)->keys = atoi(token[jj].name);
				continue;

			case 17:	// REFERENCE
				jj++;
				m_pFormItem->setFormStr(fN, "refS", CString(token[jj].name, token[jj].len));
				continue;

			case 19:	// DEFAULT
				jj++;
				m_pFormItem->setFormStr(fN, "dat", CString(token[jj].name, token[jj].len));
				jj++;
				m_pFormItem->setFormStr(fN, "str", CString(token[jj].name, token[jj].len));
				continue;

			case 20:	// TABH
				if (token[++jj].type != TOK_NUM)
					break;
				m_pFormItem->getFormPoint(fN)->size = (BYTE)atoi(token[jj].name);
				continue;

			case 22:	// SPIN
				//m_pFormItem->getFormPoint(fN)->properties |= PR_SPIN;
				if (m_pFormItem->getFormPoint(fN)->vals0 == 0)
					m_pFormItem->getFormPoint(fN)->vals0 = 1;
				jj++;
				m_pFormItem->getFormPoint(fN)->keys = atoi(token[jj].name);
				continue;

			case 23:	// CODE
				m_pFormItem->getFormPoint(fN)->properties |= PR_CODE;
				continue;

			case 24:	// HASTOOL
				m_pFormItem->getFormPoint(fN)->properties |= PR_CHECKED;
				continue;

			case 25:	// SUM
				m_pFormItem->getFormPoint(fN)->properties |= PR_SUM;
				continue;

			case 26:	// FCOLOR
				if (token[++jj].type != TOK_NUM)
					break;
				m_pFormItem->getFormPoint(fN)->spRGB = atoi(token[jj].name);
				if (token[++jj].type != TOK_NUM)
					break;
				m_pFormItem->getFormPoint(fN)->stRGB = atoi(token[jj].name);
				continue;

			case 27:	// VCOUNT
				jj++;
				m_pFormItem->getFormPoint(fN)->keys = MAKELONG((WORD)m_pFormItem->getFormPoint(fN)->keys, atoi(token[jj].name));
				continue;

			case 28:	// CVAR
				jj++;
				m_pFormItem->setFormStr(fN, "editS", CString(token[jj].name, token[jj].len));
				continue;

			case 29:	// TABORDER
				jj++;
				m_pFormItem->getFormPoint(fN)->index = atoi(token[jj].name);
				continue;

			case 30:	// PAGE
				{
					jj++;
					int pageN = atoi(token[jj++].name), formCnt = atoi(token[jj].name);
					for (int nLoop = 0; nLoop < formCnt; nLoop++)
					{
						struct _tabPage tmp;
						tmp.tabN = fN;
						tmp.pageN = pageN;
						m_tabPageArr.SetAt(atoi(token[++jj].name), tmp);
					}
				}
				continue;

			case 31:	// GROUPN
				/*jj++;
				m_groupArr.SetAt(fN, atoi(token[jj].name));*/
				continue;

			case 32:	// IOORDER
				jj++;
				m_pFormItem->getFormPoint(fN)->iorder = atoi(token[jj].name);
				continue;

			case 33:	// MATCHKEY
				jj++;
				tmps = CString(token[jj].name, token[jj].len);
				for (kk = 0; kk < VKTBL; kk++)
				{
					if (!tmps.CompareNoCase(vkTbl[kk].strName))
					{
						m_pFormItem->getFormPoint(fN)->onEdit = kk;
						break;
					}
				}
				continue;

			case 34:	// SPINTYPE
				jj++;
				m_pFormItem->getFormPoint(fN)->vals0 = atoi(token[jj].name);
				continue;

			case 35:	// ENCRYPT
				jj++;
				m_pFormItem->setFormStr(fN, "str2", CString(token[jj].name, token[jj].len));
				continue;

			case 36:	// LINECOLOR
				jj++;
				m_pFormItem->getFormPoint(fN)->bRGB = atoi(token[jj].name);
				m_pFormItem->getFormPoint(fN)->iok = PN_LINE;
				continue;

			case 38:	// LIST
				m_pFormItem->getFormPoint(fN)->properties |= PR_LIST;
				continue;
			}
			break;

		case FM_GRID:
			for (kk = 0; kk < EMODETBL; kk++)
			{
				if (!tmps.CompareNoCase(emodeTbl[kk].strName))
				{
					cellItem->getCellPoint(repN)->iok = emodeTbl[kk].iok;
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
					cellItem->getCellPoint(repN)->type = ioTbl[kk].type;
					break;
				}
			}
			if (kk < IOTBL)
				continue;

			for (kk = 0; kk < ATTRTBL; kk++)
			{
				if (!tmps.CompareNoCase(attrTbl[kk].strName))
				{
					cellItem->getCellPoint(repN)->attr |= attrTbl[kk].attr;
					if (attrTbl[kk].attr == FA_WRAP)
						multi++;
					break;
				}
			}
			if (kk < ATTRTBL)
				continue;

			for (kk = 0; kk < ZEROSUPTBL; kk++)
			{
				if (!tmps.CompareNoCase(zerosupTbl[kk].strName))
				{
					cellItem->getCellPoint(repN)->attr2 |= zerosupTbl[kk].attr2;
					break;
				}
			}
			if (kk < ZEROSUPTBL)
				continue;

			for (kk = 0; kk < CORGBTBL; kk++)
			{
				if (!tmps.CompareNoCase(coRgbTbl[kk].strName))
				{
					cellItem->getCellPoint(repN)->attr2 |= coRgbTbl[kk].attr2;
					break;
				}
			}
			if (kk < CORGBTBL)
				continue;

			for (kk = 0; kk < DATETBL; kk++)
			{
				if (!tmps.CompareNoCase(dateTbl[kk].strName))
				{
					cellItem->getCellPoint(repN)->attr2 |= dateTbl[kk].attr2;
					break;
				}
			}
			if (kk < DATETBL)
				continue;

			for (kk = 0; kk < CONTRASTTBL; kk++)
			{
				if (!tmps.CompareNoCase(contrastTbl[kk].strName))
				{
					cellItem->getCellPoint(repN)->attr2 |= contrastTbl[kk].attr2;
					break;
				}
			}
			if (kk < CONTRASTTBL)
				continue;

			for (kk = 0; kk < COLUMNTBL; kk++)
			{
				if (!tmps.Compare(columnTbl[kk].strName))
					break;
			}
			if (kk >= COLUMNTBL)
				break;

			switch (columnTbl[kk].flow)
			{
			case 1:		// COLUMNS
				{
					if (token[++jj].type != TOK_NUM)
						break;
					repR->cols = (BYTE)atoi(token[jj].name);
					for (int jj = 0; jj < repR->cols; jj++)
						cellItem->addCell();
				}
				continue;

			case 2:		// MATRIX
				if (token[++jj].type != TOK_NUM)
					break;
				repR->rows = (WORD)atoi(token[jj].name);
				if (token[++jj].type != TOK_NUM)
					break;
				repR->cols = (BYTE)atoi(token[jj].name);
				if (m_pFormItem->getFormPoint(fN)->kind == FM_GRID)
					for (int jj = 0; jj < repR->cols; jj++)
						cellItem->addCell();
				else if (m_pFormItem->getFormPoint(fN)->kind == FM_TABLE)
					for (int jj = 0; jj < repR->rows; jj++)
						cellItem->addCell();

				continue;

			case 3:		// COLINFO
				repN++; jj++;
				CopyMemory(cellItem->getCellPoint(repN)->name, token[jj].name, token[jj].len); 
				cellItem->getCellPoint(repN)->name[token[jj].len]= '\0';
				cellItem->getCellPoint(repN)->lineN = (BYTE)multi;
				cellItem->getCellPoint(repN)->iok   = EIO_OUTPUT;
				continue;

			case 4:		// LEN
				if (token[++jj].type != TOK_NUM)
					break;
				cellItem->getCellPoint(repN)->size = (WORD)atoi(token[jj].name);
				continue;

			case 5:		// COLOR
				if (token[++jj].type != TOK_NUM)
					break;
				cellItem->getCellPoint(repN)->pRGB = atoi(token[jj].name);
				if (token[++jj].type != TOK_NUM)
					break;
				cellItem->getCellPoint(repN)->tRGB = atoi(token[jj].name);
				continue;

			case 6:		// DOM
				jj++;
				CopyMemory(cellItem->getCellPoint(repN)->auxS, token[jj].name, token[jj].len);
				continue;

			case 7:		// VALID
				if (token[++jj].type != TOK_NUM)
					break;
				m_pFormItem->getFormPoint(fN)->keys = MAKELONG(LOWORD(m_pFormItem->getFormPoint(fN)->keys), (WORD)atoi(token[jj].name));
				continue;

			case 8:		// HEAD
				jj++;
				CopyMemory(cellItem->getCellPoint(repN)->head, token[jj].name, token[jj].len);
				tmps = cellItem->getCellPoint(repN)->head;
				//cellItem->getCellPoint(repN)->headN = 1;
				/*for ( ;; )
				{
					offs = tmps.Find('~');
					if (offs == -1)
						break;
					else
						tmps = tmps.Mid(offs+1);
					cellItem->getCellPoint(repN)->headN++;
				}*/
				continue;

			case 9:		// WIDTH
				if (token[++jj].type != TOK_NUM)
					break;
				cellItem->getCellPoint(repN)->width = (WORD)atoi(token[jj].name);
				continue;

			case 10:		// HEADCOLOR
				if (token[++jj].type != TOK_NUM)
					break;
				cellItem->getCellPoint(repN)->hpRGB = atoi(token[jj].name);
				if (token[++jj].type != TOK_NUM)
					break;
				cellItem->getCellPoint(repN)->htRGB = atoi(token[jj].name);
				continue;

			case 11:		// COLUMN_VISIBLE
				cellItem->getCellPoint(repN)->properties |= PR_VISIBLE;
				continue;

			case 12:		// MULTI
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_MULTI;
				continue;

			case 13:		// FLOAT
				if (token[++jj].type != TOK_NUM)
					break;
				cellItem->getCellPoint(repN)->vals = (BYTE)atoi(token[jj].name);
				continue;

			case 14:		// FLEXIBLE
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_FLEX;
				continue;

			case 15:		// VSCROLL
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_VSCR;
				continue;

			case 16:		// HSCROLL
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_HSCR;
				continue;

			case 17:		// FOCUSKIND
				if (token[++jj].type != TOK_NUM)
					break;
				m_pFormItem->getFormPoint(fN)->isEdit = atoi(token[jj].name);
				continue;

			case 18:		// COLALIGN
				for (mm = 0; mm < 2; mm++)
				{
					jj++;
					tmps = CString(token[jj].name, token[jj].len);
					for (ll = 0; ll < ALIGNTBL; ll++)
					{
						if (!tmps.Compare(alignTbl[ll].strName))
						{
							if (!mm)
								cellItem->getCellPoint(repN)->alignH = alignTbl[ll].alignment;
							else
								cellItem->getCellPoint(repN)->alignC = alignTbl[ll].alignment;
							break;
						}
					}
				}
				continue;

			case 19:		// INS_TOP
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_TOP;
				continue;

			case 20:		// NOHEAD
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_XHEAD;
				continue;

			case 21:		// NO3D
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_X3D;
				continue;

			case 22:		// RROW
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_ROW;
				continue;

			case 23:		// XVLINE
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_XVLINE;
				continue;

			case 24:		// XHLINE
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_XHLINE;
				continue;

			case 25:		// LINECOLOR
				if (token[++jj].type != TOK_NUM)
					break;
				m_pFormItem->getFormPoint(fN)->bRGB = atoi(token[jj].name);
				continue;

			//case 26:		// LINEW
			case 27:		// CELLH
				if (token[++jj].type != TOK_NUM)
					break;
				m_pFormItem->getFormPoint(fN)->keys = MAKELONG(atoi(token[jj].name), HIWORD(m_pFormItem->getFormPoint(fN)->keys));
				continue;

			case 28:		// COLUMN_SORT
				cellItem->getCellPoint(repN)->properties |= PR_SORT;
				continue;

			case 29:		// REFERENCE
				jj++;
				CopyMemory(cellItem->getCellPoint(repN)->refS, token[jj].name, token[jj].len);
				continue;

			case 30:		// ICOMBO
				cellItem->getCellPoint(repN)->kind = CK_COMBO;
				continue;

			case 31:		// ICHECK
				cellItem->getCellPoint(repN)->kind = CK_CHECK;
				continue;

			case 32:		// COUNT
				if (token[++jj].type != TOK_NUM)
					break;
				cellItem->getCellPoint(repN)->keys = (BYTE)atoi(token[jj].name);
				continue;

			case 33:		// DISPLAY
				offs = 0;
				if (cellItem->getCellPoint(repN)->keys <= 0)
					continue;
				for (kk = 0; kk < cellItem->getCellPoint(repN)->keys; kk++)
				{
					jj++;
					CopyMemory(&cellItem->getCellPoint(repN)->dat[offs], token[jj].name, token[jj].len);
					offs += token[jj].len;
					cellItem->getCellPoint(repN)->dat[offs++] = '\t';
				}
				continue;

			case 34:		// DATA
				offs = 0;
				for (kk = 0; kk < cellItem->getCellPoint(repN)->keys; kk++)
				{
					jj++;
					CopyMemory(&cellItem->getCellPoint(repN)->str[offs], token[jj].name, token[jj].len);
					offs += cellItem->getCellPoint(repN)->size;
					cellItem->getCellPoint(repN)->str[offs++] = '\t';
				}
				continue;

			case 35:		// CHECKSTR
				jj++;
				CopyMemory(&cellItem->getCellPoint(repN)->str2, token[jj].name, token[jj].len);
				cellItem->getCellPoint(repN)->str[token[jj].len] = '\0';
				continue;

			case 36:		// LEAVE
				m_pFormItem->getFormPoint(fN)->attr |= FA_WRAP;
				continue;

			case 38:		// FIX
				if (token[++jj].type != TOK_NUM)
					break;
				m_pFormItem->getFormPoint(fN)->type = atoi(token[jj].name);
				continue;

			case 39:		// CELLEDIT
				m_pFormItem->getFormPoint(fN)->attr |= FA_ENUM;
				continue;

			case 40:		// VARIANT
				jj++;
				m_pFormItem->setFormStr(fN, "str", CString(token[jj].name, token[jj].len));
				continue;

			case 41:		// ROWC
				if (token[++jj].type != TOK_NUM)
					break;
				m_pFormItem->getFormPoint(fN)->spRGB = atoi(token[jj].name);
				if (token[++jj].type != TOK_NUM)
					break;
				m_pFormItem->getFormPoint(fN)->stRGB = atoi(token[jj].name);
				continue;

			case 42:		// PAGE
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_PAGE;
				continue;

			case 43:		// HEADER
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_HEADER;
				continue;

			case 44:		// TABLE
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_TABLE;
				continue;

			case 45:		// THUMB
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_THUMB;
				continue;

			case 46:		// CXDELETE
				m_pFormItem->getFormPoint(fN)->attr |= FA_XDELETE;
				continue;

			case 47:		// TABORDER
				jj++;
				if (token[jj].type != TOK_NUM)
					break;
				m_pFormItem->getFormPoint(fN)->index = atoi(token[jj].name);
				continue;

			case 48:		// EDITFORMAT
				jj++;
				CopyMemory(&cellItem->getCellPoint(repN)->editS, token[jj].name, token[jj].len);
				cellItem->getCellPoint(repN)->editS[token[jj].len] = '\0';
				cellItem->getCellPoint(repN)->isEdit = IE_EDIT;
				continue;

			case 49:		// IFRGB
				{
					WORD upRGB = atoi(token[++jj].name);
					WORD dnRGB = atoi(token[++jj].name);
					cellItem->getCellPoint(repN)->onEdit = MAKELONG(dnRGB, upRGB);
					cellItem->getCellPoint(repN)->isEdit = IE_CORGB;
					cellItem->getCellPoint(repN)->attr |= FA_CORGB;
				}
				continue;

			case 50:		// IOORDER
				jj++;
				m_pFormItem->getFormPoint(fN)->iorder = atoi(token[jj].name);
				continue;
			
			case 51:		// INORMAL
				cellItem->getCellPoint(repN)->kind = CK_NORMAL;
				continue;

			case 52:		// IGRAPH
				cellItem->getCellPoint(repN)->kind = CK_GRAPH;
				continue;
			
			case 54:		// AUTOSIZE
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_AUTOSIZE;
				continue;
			
			case 55:		// SUPRESS
				cellItem->getCellPoint(repN)->properties |= PR_SUPRESS;
				continue;

			case 56:		// CUD
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_CUD;
				continue;

			case 57:		// ISFOCUS
				m_pFormItem->getFormPoint(fN)->properties |= PR_CHECKED;
				continue;

			case 58:		// MATCHKEY
				jj++;
				tmps = CString(token[jj].name, token[jj].len);
				for (kk = 0; kk < VKTBL; kk++)
				{
					if (!tmps.CompareNoCase(vkTbl[kk].strName))
					{
						cellItem->getCellPoint(repN)->vals2 = kk;
						break;
					}
				}
				continue;

			case 59:		// MERGE
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_MERGE;
				continue;

			case 60:		// IBUTTON
				cellItem->getCellPoint(repN)->kind = CK_BUTTON;
				continue;

			case 61:		// BUTTONSTR
				jj++;
				CopyMemory(&cellItem->getCellPoint(repN)->txt, token[jj].name, token[jj].len);
				cellItem->getCellPoint(repN)->str[token[jj].len] = '\0';
				continue;

			case 62:		// BUTTONIMG
				jj++;
				CopyMemory(&cellItem->getCellPoint(repN)->str3, token[jj].name, token[jj].len);
				cellItem->getCellPoint(repN)->str[token[jj].len] = '\0';
				continue;

			case 63:		// COLUMN_IMAGE
				cellItem->getCellPoint(repN)->properties |= PR_IMAGE;
				continue;

			case 64:		// COLUMN_IMGTEXT
				cellItem->getCellPoint(repN)->properties |= PR_IMAGETEXT;
				continue;
			
			case 65:
				if (token[++jj].type != TOK_NUM)
					break;
				m_pFormItem->getFormPoint(fN)->onEdit = atoi(token[jj].name);
				continue;

			case 66:		// KEYFOCUS
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_KEY;
				continue;

			case 67:		// SELALWAYS
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_ALWAYS;
				continue;

			case 68:		// COLUMN_SHOWFORM
				cellItem->getCellPoint(repN)->properties |= PR_EDIT;
				continue;

			case 69:		// COLHINTTYPE
				jj++;
				tmps = CString(token[jj].name, token[jj].len);
				for (kk = 0; kk < HINTTBL; kk++)
				{
					if (!tmps.CompareNoCase(hintTbl[kk].strName))
					{
						cellItem->getCellPoint(repN)->isHint = hintTbl[kk].type;
						break;
					}
				}
				continue;

			case 70:		// COLHELPTEXT
				jj++;
				CopyMemory(cellItem->getCellPoint(repN)->onHint, token[jj].name, token[jj].len);
				continue;

			case 71:		// GRFIX
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_FIX;
				continue;

			case 72:		// GRFLEXEXPAND
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_EXPAND;
				continue;

			case 73:		// COLUMN_EXCHECKE
				cellItem->getCellPoint(repN)->properties |= PR_CHECKED;
				continue;

			case 74:		// GRXSIZE
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_XSIZE;
				continue;

			case 75:		// GRAUTOWIDTH
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_AUTO;
				continue;

			case 79:		// GRVALID
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_VALID;
				continue;

			case 80:		// GRFTEXT
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_FTEXT;
				continue;

			case 81:		// GRXXSL
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_XXLS;
				continue;

			case 82:		// GRSHARE
				jj++;
				m_pFormItem->setFormStr(fN, "str2", CString(token[jj].name, token[jj].len));
				continue;

			case 83:		// COLUMN_BOLD
				cellItem->getCellPoint(repN)->properties |= PR_BOLD;
				continue;

			case 84:		// ONLYHEAD3D
				m_pFormItem->getFormPoint(fN)->attr2 |= GO_3DHEAD;
				continue;

			case 99:		// INIT
				jj++;
				m_pFormItem->setFormStr(fN, "str", CString(token[jj].name, token[jj].len));
				continue;
			}
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
				m_pFormItem->getFormPoint(fN)->pRGB = atoi(token[jj].name);
				if (token[++jj].type != TOK_NUM)
					break;
				m_pFormItem->getFormPoint(fN)->tRGB = atoi(token[jj].name);
				continue;

			case 2:		// GROUP
				jj++;
				m_pFormItem->setFormStr(fN, "auxS", CString(token[jj].name, token[jj].len));
				continue;

			case 3:		// FOCUS
				m_pFormItem->getFormPoint(fN)->properties |= PR_CHECKED;
				continue;

			case 4:		// DATE
			case 7:		// LINEW
				jj++;
				m_pFormItem->getFormPoint(fN)->size = (WORD)atoi(token[jj].name);
				continue;

			case 5:		// KEYMAP
				jj++;
				tmps = CString(token[jj].name, token[jj].len);
				for (kk = 0; kk < VKTBL; kk++)
				{
					if (!tmps.CompareNoCase(vkTbl[kk].strName))
					{
						m_pFormItem->getFormPoint(fN)->keys = kk;
						break;
					}
				}
				continue;

			case 6:		// REF
				jj++;
				m_pFormItem->setFormStr(fN, "auxS", CString(token[jj].name, token[jj].len));
				continue;

			case 8:			// LINECOLOR
				jj++;
				m_pFormItem->getFormPoint(fN)->bRGB = atoi(token[jj].name);
				m_pFormItem->getFormPoint(fN)->type = PN_LINE;
				continue;
			case 9:			// TABORDER
				jj++;
				if (token[jj].type != TOK_NUM)
					break;
				m_pFormItem->getFormPoint(fN)->index = atoi(token[jj].name);
				continue;
			case 10:		// GROUPN
				/*jj++;
				m_groupArr.SetAt(fN, atoi(token[jj].name));*/
				continue;
			case 11:		// IOORDER
				jj++;
				if (token[jj].type != TOK_NUM)
					break;
				m_pFormItem->getFormPoint(fN)->iorder = atoi(token[jj].name);
				continue;
			}
			break;
		}
	}
	if (cellItem)
		delete cellItem;
	fN++;
}

//
//	syntax
//
struct _lineSyntax {
	char    token[21];
	BYTE    flag;
} lineSyntax[]={
		{"_AW_ONSTART_AW_",		LTYPE_ONSTART},
		{"_AW_ONSEND_AW_",		LTYPE_ONSEND},
		{"_AW_ONRECEIVE_AW_",		LTYPE_ONRECEIVE},
		{"_AW_ONALERT_AW_",		LTYPE_ONALERT},
		{"_AW_ONSERVICE_AW_",		LTYPE_ONSERVICE},
		{"_AW_ONFILE_AW_",		LTYPE_ONFILE},
		{"_AW_ONSELECT_AW_",		LTYPE_ONSELECT},
		{"_AW_ONTIMER_AW_",		LTYPE_ONTIMER},
		{"_AW_ONFOCUS_AW_",		LTYPE_ONFOCUS},
		{"_AW_ONCLOSE_AW_",		LTYPE_ONCLOSE},
		{"_AW_ONDEVICE_AW_",		LTYPE_ONDEVICE},
		{"_AW_ONAPPROVE_AW_",		LTYPE_ONAPPROVE},
		{"_AW_ONKEY_AW_",		LTYPE_ONKEY},
		{"_AW_ONTRIGGER_AW_",		LTYPE_ONTRIGGER},
		{"_AW_ONDECLARE_AW_",		LTYPE_ONDECLARE},
		{"_AW_ONINDECLARE_AW_",		LTYPE_ONINDECLARE},
		{"_AW_SCRIPT_AW_",		LTYPE_SCPBEGIN},
		{"_AW_ENDSCRIPT_AW_",		LTYPE_SCPEND},
		{"_AW_ONEDIT_AW_",		LTYPE_ONEDIT},
		{"_AW_ONCLICK_AW_",		LTYPE_ONCLICK},
		{"_AW_ONDBLCLK_AW_",		LTYPE_ONDBLCLK},
		{"_AW_ONCHANGE_AW_",		LTYPE_ONCHANGE},
		{"_AW_ONCHARCHANGE_AW_",	LTYPE_ONCHAR},
		{"_AW_ONKEYCHANGE_AW_",		LTYPE_ONKEYCHANGE},
		{"PROCEDURE",			LTYPE_PROC},
		{"ENDPROC",			LTYPE_PROCEND},
		{"_AW_ONENDSUB_AW_",		LTYPE_ONENDSUB},
		{"_AW_ONENDFUNC_AW_",		LTYPE_ONENDFUNC},
		{"_AW_ONEND_AW_",		LTYPE_ONENDSUB},
		{"}",				LTYPE_DIVEND}
};
#define LINESYNTAX      sizeof(lineSyntax) / sizeof(struct _lineSyntax)

void CMapLoad::BuildResource(char* text, int line, int formN)
{
	CMapToken  tok[2];
	int	ii,  jj,  kk,  ll;
	int	offset, tokN,  len;
	bool	me;
	CFile	ffile;
	HANDLE	hFile;
	char	prefix[L_SGID+1];
	CString	tmps, tmp;

	len   = strlen(text);
	tmp = text;
	me    = false;
	tmp.TrimLeft();

	for (jj = 0; jj < LINESYNTAX; jj++)
	{
		if (!tmp.IsEmpty() && tmp.Find(lineSyntax[jj].token) == 0)
		{
			break;
		}
	}
	tmp = text;

	switch (m_rtype)
	{
	case LTYPE_SCPEND:
	case LTYPE_ONCLICK:
	case LTYPE_ONDBLCLK:
	case LTYPE_ONCHANGE:
	case LTYPE_ONCHAR:
	case LTYPE_ONKEYCHANGE:
		if (m_formP == -1)
		{
			return;
		}
		break;
	case LTYPE_ONEDIT:
		if (m_formP == -1 && m_colP == -1)
		{
			return;
		}
	/*case LTYPE_ONENDSUB:
	case LTYPE_ONENDFUNC:
		if (m_formP == -1)
		{
			//return;
		}
		break;*/
	default:
		break;
	}

	if (jj < LINESYNTAX)
	{
		switch (m_rtype)
		{
		case LTYPE_ONSTART:
			m_script.TrimRight();
			if (m_script.IsEmpty())
			{
				m_mapH->onStart     = false;
				m_mapH->onStartN[0] = '\0';
				break;
			}
			SaveScriptFile(CString(m_mapH->onStartN), m_script);
			break;

		case LTYPE_ONSEND:
			m_script.TrimRight();
			if (m_script.IsEmpty())
			{
				m_mapH->onSend     = false;
				m_mapH->onSendN[0] = '\0';
				break;
			}
			SaveScriptFile(CString(m_mapH->onSendN), m_script);
			break;

		case LTYPE_ONRECEIVE:
			m_script.TrimRight();
			if (m_script.IsEmpty())
			{
				m_mapH->onReceive     = false;
				m_mapH->onReceiveN[0] = '\0';
				break;
			}
			SaveScriptFile(CString(m_mapH->onReceiveN), m_script);
			break;

		case LTYPE_ONALERT:
			m_script.TrimRight();
			if (m_script.IsEmpty())
			{
				m_mapH->onAlert     = false;
				m_mapH->onAlertN[0] = '\0';
				break;
			}
			SaveScriptFile(CString(m_mapH->onAlertN), m_script);
			break;

		case LTYPE_ONSERVICE:
			m_script.TrimRight();
			if (m_script.IsEmpty())
			{
				m_mapH->onService     = false;
				m_mapH->onServiceN[0] = '\0';
				break;
			}
			SaveScriptFile(CString(m_mapH->onServiceN), m_script);
			break;

		case LTYPE_ONFILE:
			m_script.TrimRight();
			if (m_script.IsEmpty())
			{
				m_mapH->onFile     = false;
				m_mapH->onFileN[0] = '\0';
				break;
			}
			SaveScriptFile(CString(m_mapH->onFileN), m_script);
			break;

		case LTYPE_ONSELECT:
			m_script.TrimRight();
			if (m_script.IsEmpty())
			{
				m_mapH->onSelect     = false;
				m_mapH->onSelectN[0] = '\0';
				break;
			}
			SaveScriptFile(CString(m_mapH->onSelectN), m_script);
			break;

		case LTYPE_ONTIMER:
			m_script.TrimRight();
			if (m_script.IsEmpty())
			{
				m_mapH->onTimer     = false;
				m_mapH->onTimerN[0] = '\0';
				break;
			}
			SaveScriptFile(CString(m_mapH->onTimerN), m_script);
			break;

		case LTYPE_ONFOCUS:
			m_script.TrimRight();
			if (m_script.IsEmpty())
			{
				m_mapH->onFocus     = false;
				m_mapH->onFocusN[0] = '\0';
				break;
			}
			SaveScriptFile(CString(m_mapH->onFocusN), m_script);
			break;

		case LTYPE_ONCLOSE:
			m_script.TrimRight();
			if (m_script.IsEmpty())
			{
				m_mapH->onClose     = false;
				m_mapH->onCloseN[0] = '\0';
				break;
			}
			SaveScriptFile(CString(m_mapH->onCloseN), m_script);
			break;

		case LTYPE_ONDEVICE:
			m_script.TrimRight();
			if (m_script.IsEmpty())
			{
				m_mapH->onDevice     = false;
				m_mapH->onDeviceN[0] = '\0';
				break;
			}
			SaveScriptFile(CString(m_mapH->onDeviceN), m_script);
			break;

		case LTYPE_ONAPPROVE:
			m_script.TrimRight();
			if (m_script.IsEmpty())
			{
				m_mapH->onApprove     = false;
				m_mapH->onApproveN[0] = '\0';
				break;
			}
			SaveScriptFile(CString(m_mapH->onApproveN), m_script);
			break;

		case LTYPE_ONKEY:
			m_script.TrimRight();
			if (m_script.IsEmpty())
			{
				m_mapH->onKey     = false;
				m_mapH->onKeyN[0] = '\0';
				break;
			}
			SaveScriptFile(CString(m_mapH->onKeyN), m_script);
			break;

		case LTYPE_ONTRIGGER:
			m_script.TrimRight();
			if (m_script.IsEmpty())
			{
				m_mapH->onTrigger     = false;
				m_mapH->onTriggerN[0] = '\0';
				break;
			}
			SaveScriptFile(CString(m_mapH->onTriggerN), m_script);
			break;


		case LTYPE_ONDECLARE:
			m_script.TrimRight();
			if (m_script.IsEmpty())
			{
				m_mapH->onDeclaration     = false;
				m_mapH->onDeclarationN[0] = '\0';
				break;
			}
			SaveScriptFile(CString(m_mapH->onDeclarationN), m_script);
			break;
		
		case LTYPE_ONINDECLARE:
			m_script.TrimRight();
			if (m_script.IsEmpty())
			{
				m_mapH->onInDeclaration     = false;
				m_mapH->onInDeclarationN[0] = '\0';
				break;
			}
			SaveScriptFile(CString(m_mapH->onInDeclarationN), m_script);
			break;

		case LTYPE_ONCLICK:
			m_script.TrimRight();
			if (m_script.IsEmpty())
			{
				m_pFormItem->getFormPoint(m_formP)->onClick = -1;
				break;
			}
			SaveScriptFile(m_pFormItem->getFormStr(m_formP, "scpN"), m_script);
			break;

		case LTYPE_ONDBLCLK:
			m_script.TrimRight();
			if (m_script.IsEmpty())
			{
				m_pFormItem->getFormPoint(m_formP)->onDblClk = -1;
				break;
			}
			SaveScriptFile(m_pFormItem->getFormStr(m_formP, "scpN"), m_script);
			break;

		case LTYPE_ONCHANGE:
			m_script.TrimRight();
			if (m_script.IsEmpty())
			{
				m_pFormItem->getFormPoint(m_formP)->onChange = -1;
				break;
			}
			SaveScriptFile(m_pFormItem->getFormStr(m_formP, "scpN"), m_script);
			break;

		case LTYPE_ONCHAR:
			m_script.TrimRight();
			if (m_script.IsEmpty())
			{
				m_pFormItem->getFormPoint(m_formP)->onChar = -1;
				break;
			}
			SaveScriptFile(m_pFormItem->getFormStr(m_formP, "scpN"), m_script);
			break;

		case LTYPE_ONKEYCHANGE:
			m_script.TrimRight();
			if (m_script.IsEmpty())
			{
				m_pFormItem->getFormPoint(m_formP)->onKey = -1;
				break;
			}
			SaveScriptFile(m_pFormItem->getFormStr(m_formP, "scpN"), m_script);
			break;

		default:
			break;
		}

		m_rtype  = lineSyntax[jj].flag;
		m_script = _T("");
		switch (m_rtype)
		{
		case LTYPE_SCPEND:
		case LTYPE_ONCLICK:
		case LTYPE_ONDBLCLK:
		case LTYPE_ONCHANGE:
		case LTYPE_ONCHAR:
		case LTYPE_ONKEYCHANGE:
			if (m_formP == -1)
			{
				return;
			}
			break;
		case LTYPE_ONEDIT:
			if (m_formP == -1 && m_colP == -1)
			{
				return;
			}
		default:
			break;
		}

		switch (m_rtype)
		{
		case LTYPE_SCPBEGIN:
			for (ii = 0; ii < 2; ii++)
				tok[ii].Clear();

			tokN = 0;
			for (ii = strlen(lineSyntax[jj].token); ii < (int)strlen(tmp); ii++)
			{
				switch (tmp.GetAt(ii))
				{
				case '(':
					tok[tokN].type |= TOKF_HOS;
					break;
				case '.':
					if (!tok[tokN].name.IsEmpty())
					{
						tok[tokN].len = (WORD)tok[tokN].name.GetLength();
						tokN++;
					}
					tok[tokN].type |= TOK_PRP;
					break;
				case ')':
					if (tok[tokN].type & TOKF_HOS)
						tok[tokN].type = TYPE_NOTDEF;

					if (!tok[tokN].name.IsEmpty())
					{
						tok[tokN].len = (WORD)tok[tokN].name.GetLength();
						ii = lstrlen(tmp);
					}
					break;
				default :
					tok[tokN].name += tmp.GetAt(ii);
					break;
				}
			}

			m_formP = getFormIndex((LPSTR)tok[0].name.operator LPCTSTR(), tok[0].len, formN);
			
			if (m_formP == -1)
			{
				return;
			}

			m_column = false;
			if (tok[1].type == TOK_PRP)
			{
				m_colP = getColumnIndex((LPSTR)tok[1].name.operator LPCTSTR(), tok[1].len, m_formP);
				if (m_colP == -1)
				{
					return;
				}

				m_column = true;
			}

			if (!m_column)
			{
				m_pFormItem->getFormPoint(m_formP)->onClick   = -1;
				m_pFormItem->getFormPoint(m_formP)->onDblClk  = -1;
				m_pFormItem->getFormPoint(m_formP)->onChange  = -1;
				m_pFormItem->getFormPoint(m_formP)->onChar    = -1;
				m_pFormItem->getFormPoint(m_formP)->onKey     = -1;

				m_pFormItem->getFormPoint(m_formP)->onClickL   = 0;
				m_pFormItem->getFormPoint(m_formP)->onDblClkL  = 0;
				m_pFormItem->getFormPoint(m_formP)->onChangeL  = 0;
				m_pFormItem->getFormPoint(m_formP)->onCharL    = 0;
				m_pFormItem->getFormPoint(m_formP)->onKeyL     = 0;

				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = '\0'; 
				m_pFormItem->setFormStr(m_formP, "scpN", "");
				char fileName[1024];
				wccGetTempName(fileName, prefix);
				m_pFormItem->setFormStr(m_formP, "scpN", fileName);
			}
			break;

		case LTYPE_SCPEND:
			m_rtype = 0;
			if (!m_column)
			{
				if ((m_pFormItem->getFormPoint(m_formP)->onClickL  == 0 && m_pFormItem->getFormPoint(m_formP)->onClick   == -1)
				&& (m_pFormItem->getFormPoint(m_formP)->onDblClkL  == 0 && m_pFormItem->getFormPoint(m_formP)->onDblClk  == -1)
				&& (m_pFormItem->getFormPoint(m_formP)->onChangeL  == 0 && m_pFormItem->getFormPoint(m_formP)->onChange  == -1)
				&& (m_pFormItem->getFormPoint(m_formP)->onCharL    == 0 && m_pFormItem->getFormPoint(m_formP)->onChar    == -1)
				&& (m_pFormItem->getFormPoint(m_formP)->onKeyL     == 0 && m_pFormItem->getFormPoint(m_formP)->onKey     == -1))
				{
					DeleteFile(m_pFormItem->getFormStr(m_formP, "scpN"));
					break;
				}

				hFile = CreateFile(m_pFormItem->getFormStr(m_formP, "scpN"), GENERIC_READ, FILE_SHARE_READ,
						NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (hFile == INVALID_HANDLE_VALUE)
					break;

				offset = GetFileSize(hFile, NULL);
				if (offset == 0xFFFFFFFF)
				{
					CloseHandle(hFile);
					break;
				}
				// order importent onKey->onChar->onChnage->onDblClk->OnClick
				if (m_pFormItem->getFormPoint(m_formP)->onKey != -1)
				{
					m_pFormItem->getFormPoint(m_formP)->onKeyL = offset - m_pFormItem->getFormPoint(m_formP)->onKey;
					offset = m_pFormItem->getFormPoint(m_formP)->onKey;
				}				
				if (m_pFormItem->getFormPoint(m_formP)->onChar != -1)
				{
					m_pFormItem->getFormPoint(m_formP)->onCharL = offset - m_pFormItem->getFormPoint(m_formP)->onChar;
					offset = m_pFormItem->getFormPoint(m_formP)->onChar;
				}
				if (m_pFormItem->getFormPoint(m_formP)->onChange != -1)
				{
					m_pFormItem->getFormPoint(m_formP)->onChangeL = offset - m_pFormItem->getFormPoint(m_formP)->onChange;
					offset = m_pFormItem->getFormPoint(m_formP)->onChange;
				}
				if (m_pFormItem->getFormPoint(m_formP)->onDblClk != -1)
				{
					m_pFormItem->getFormPoint(m_formP)->onDblClkL = offset - m_pFormItem->getFormPoint(m_formP)->onDblClk;
					offset = m_pFormItem->getFormPoint(m_formP)->onDblClk;
				}
				if (m_pFormItem->getFormPoint(m_formP)->onClick != -1)
				{
					m_pFormItem->getFormPoint(m_formP)->onClickL = offset - m_pFormItem->getFormPoint(m_formP)->onClick;
					offset = m_pFormItem->getFormPoint(m_formP)->onClick;
				}
				
				CloseHandle(hFile);
			}
			m_column = false;
			break;

		case LTYPE_ONSTART:
			m_mapH->onStart = true;
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = '\0';
			m_mapH->onStartN[0] = '\0';
			wccGetTempName(m_mapH->onStartN, prefix);
			break;

		case LTYPE_ONSEND:
			m_mapH->onSend = true;
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = '\0'; 
			m_mapH->onSendN[0] = '\0';
			wccGetTempName(m_mapH->onSendN, prefix);
			break;

		case LTYPE_ONRECEIVE:
			m_mapH->onReceive = true;
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = '\0'; 
			m_mapH->onReceiveN[0] = '\0';
			wccGetTempName(m_mapH->onReceiveN, prefix);
			break;

		case LTYPE_ONALERT:
			m_mapH->onAlert = true;
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = '\0'; 
			m_mapH->onAlertN[0] = '\0';
			wccGetTempName(m_mapH->onAlertN, prefix);
			break;

		case LTYPE_ONSERVICE:
			m_mapH->onService = true;
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = '\0'; 
			m_mapH->onServiceN[0] = '\0';
			wccGetTempName(m_mapH->onServiceN, prefix);
			break;

		case LTYPE_ONFILE:
			m_mapH->onFile = true;
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = '\0'; 
			m_mapH->onFileN[0] = '\0';
			wccGetTempName(m_mapH->onFileN, prefix);
			break;

		case LTYPE_ONSELECT:
			m_mapH->onSelect = true;
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = '\0'; 
			m_mapH->onSelectN[0] = '\0';
			wccGetTempName(m_mapH->onSelectN, prefix);
			break;

		case LTYPE_ONTIMER:
			m_mapH->onTimer = true;
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = '\0'; 
			m_mapH->onTimerN[0] = '\0';
			wccGetTempName(m_mapH->onTimerN, prefix);
			break;

		case LTYPE_ONFOCUS:
			m_mapH->onFocus = true;
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = '\0'; 
			m_mapH->onFocusN[0] = '\0';
			wccGetTempName(m_mapH->onFocusN, prefix);
			break;

		case LTYPE_ONCLOSE:
			m_mapH->onClose = true;
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = '\0'; 
			m_mapH->onCloseN[0] = '\0';
			wccGetTempName(m_mapH->onCloseN, prefix);
			break;

		case LTYPE_ONDEVICE:
			m_mapH->onDevice = true;
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = '\0'; 
			m_mapH->onDeviceN[0] = '\0';
			wccGetTempName(m_mapH->onDeviceN, prefix);
			break;

		case LTYPE_ONAPPROVE:
			m_mapH->onApprove = true;
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = '\0'; 
			m_mapH->onApproveN[0] = '\0';
			wccGetTempName(m_mapH->onApproveN, prefix);
			break;

		case LTYPE_ONKEY:
			m_mapH->onKey = true;
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = '\0'; 
			m_mapH->onKeyN[0] = '\0';
			wccGetTempName(m_mapH->onKeyN, prefix);
			break;

		case LTYPE_ONTRIGGER:
			m_mapH->onTrigger = true;
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = '\0';
			m_mapH->onTriggerN[0] = '\0';
			wccGetTempName(m_mapH->onTriggerN, prefix);
			break;

		case LTYPE_ONDECLARE:
			m_mapH->onDeclaration = true;
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = '\0';
			m_mapH->onDeclarationN[0] = '\0';
			wccGetTempName(m_mapH->onDeclarationN, prefix);
			break;

		case LTYPE_ONINDECLARE:
			m_mapH->onInDeclaration = true;
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = '\0';
			m_mapH->onInDeclarationN[0] = '\0';
			wccGetTempName(m_mapH->onInDeclarationN, prefix);
			break;

		case LTYPE_ONCLICK:
			m_pFormItem->getFormPoint(m_formP)->onClick = 0;
			hFile = CreateFile(m_pFormItem->getFormStr(m_formP, "scpN"), GENERIC_READ, FILE_SHARE_READ,
						NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
				break;
			offset = GetFileSize(hFile, NULL);
			if (offset != 0xFFFFFFFF)
				m_pFormItem->getFormPoint(m_formP)->onClick = offset;
			CloseHandle(hFile);
			break;

		case LTYPE_ONDBLCLK:
			m_pFormItem->getFormPoint(m_formP)->onDblClk = 0;
			hFile = CreateFile(m_pFormItem->getFormStr(m_formP, "scpN"), GENERIC_READ, FILE_SHARE_READ,
						NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
				break;
			offset = GetFileSize(hFile, NULL);
			if (offset != 0xFFFFFFFF)
				m_pFormItem->getFormPoint(m_formP)->onDblClk = offset;
			CloseHandle(hFile);
			break;

		case LTYPE_ONCHANGE:
			m_pFormItem->getFormPoint(m_formP)->onChange = 0;
			hFile = CreateFile(m_pFormItem->getFormStr(m_formP, "scpN"), GENERIC_READ, FILE_SHARE_READ,
						NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
				break;
			offset = GetFileSize(hFile, NULL);
			if (offset != 0xFFFFFFFF)
				m_pFormItem->getFormPoint(m_formP)->onChange = offset;
			CloseHandle(hFile);
			break;

		case LTYPE_ONCHAR:
			m_pFormItem->getFormPoint(m_formP)->onChar = 0;
			hFile = CreateFile(m_pFormItem->getFormStr(m_formP, "scpN"), GENERIC_READ, FILE_SHARE_READ,
						NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
				break;
			offset = GetFileSize(hFile, NULL);
			if (offset != 0xFFFFFFFF)
				m_pFormItem->getFormPoint(m_formP)->onChar = offset;
			CloseHandle(hFile);
			break;

		case LTYPE_ONKEYCHANGE:
			m_pFormItem->getFormPoint(m_formP)->onKey = 0;
			hFile = CreateFile(m_pFormItem->getFormStr(m_formP, "scpN"), GENERIC_READ, FILE_SHARE_READ,
						NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
				break;
			offset = GetFileSize(hFile, NULL);
			if (offset != 0xFFFFFFFF)
				m_pFormItem->getFormPoint(m_formP)->onKey = offset;
			CloseHandle(hFile);
			break;

		default:
			break;
		}
	}
	else
	{
		if (m_rtype == LTYPE_ONEDIT)
		{
			int	tokCnt, tokN = 0;
			WORD	Type, upRGB, dnRGB;
			struct	_repR*	repR;
			CCellItem *cellItem = NULL;

			tokCnt = wccParseToken(token, line, &Type);

			tmps = CString(token[tokN].name, token[tokN].len);
			for (kk = 0; kk < IETBL; kk++)
			{
				if (!tmps.CompareNoCase(ieTbl[kk].strName))
				{
					if (!m_column)
						m_pFormItem->getFormPoint(m_formP)->isEdit = ieTbl[kk].isEdit;
					else
					{
						repR = (struct _repR *)m_pFormItem->getFormPoint(m_formP)->auxR;
						cellItem = new CCellItem(repR);
						cellItem->getCellPoint(m_colP)->isEdit = ieTbl[kk].isEdit;
					}
					break;
				}
			}

			if (kk >= IETBL)
			{
				if (cellItem)
					delete cellItem;
				return;
			}

			tokN++;
			switch (ieTbl[kk].isEdit)
			{
			case IE_EDIT:
				if (m_column)
				{
					CopyMemory(cellItem->getCellPoint(m_colP)->editS, token[tokN].name, token[tokN].len);
					break;
				}
				m_pFormItem->setFormStr(m_formP, "editS", CString(token[tokN].name, token[tokN].len));
				break;

			case IE_CORGB:
				upRGB = atoi(token[tokN].name);	tokN++;
				dnRGB = atoi(token[tokN].name);
				if (m_column)
				{
					cellItem->getCellPoint(m_colP)->onEdit = MAKELONG(dnRGB, upRGB);
					break;
				}
				m_pFormItem->getFormPoint(m_formP)->onEdit = MAKELONG(dnRGB, upRGB);
				break;

			default:
				break;
			}
			for (ll = 0; ll < tokCnt; ll++)
				token[ll].Clear();

			m_rtype = 0;
			delete cellItem;
			return;
		}

		tmps = CString(tmp);
		tmps.TrimRight();
		tmps += "\n";
		switch (m_rtype)
		{
		case LTYPE_ONSTART:
		case LTYPE_ONSEND:
		case LTYPE_ONRECEIVE:
		case LTYPE_ONALERT:
		case LTYPE_ONSERVICE:
		case LTYPE_ONFILE:
		case LTYPE_ONSELECT:
		case LTYPE_ONTIMER:
		case LTYPE_ONFOCUS:
		case LTYPE_ONCLOSE:
		case LTYPE_ONDEVICE:
		case LTYPE_ONAPPROVE:
		case LTYPE_ONKEY:
		case LTYPE_ONTRIGGER:
		case LTYPE_ONDECLARE:
		case LTYPE_ONINDECLARE:
		case LTYPE_ONCLICK:
		case LTYPE_ONDBLCLK:
		case LTYPE_ONCHANGE:
		case LTYPE_ONCHAR:
		case LTYPE_ONKEYCHANGE:
			m_script += tmps;
		default:
			break;
		}
	}
}

int CMapLoad::getFormIndex(char* symbol, int len, int count)
{
	CString	str = CString(symbol, len);
	int ii = 0;
	for (ii = 0; ii < count; ii++)
	{
		if (!str.CompareNoCase(m_pFormItem->getFormStr(ii, "name")))
			break;
	}

	if (ii >= count)
		return (-1);

	return (ii);
}

int CMapLoad::getColumnIndex(char* symbol, int len, int pos)
{
	int	count;
	CString	tmps;
	struct	_repR*	repR;

	repR = (struct _repR *)m_pFormItem->getFormPoint(pos)->auxR;
	CCellItem *cellItem = new CCellItem(repR);
	if (m_pFormItem->getFormPoint(pos)->kind == FM_TABLE)
		count = repR->rows;
	else
		count = repR->cols;

	tmps = CString(symbol, len);
	int ii = 0;
	for (ii = 0; ii < count; ii++)  // Column Symbol Check
	{
		if (!tmps.Compare(cellItem->getCellPoint(ii)->name))
			break;
	}

	delete cellItem;
	if (ii >= count)
		return (-1);

	return (ii);
}

void CMapLoad::BuildTab(int idx)
{
	for (int ii = idx; ii < m_pFormItem->getFormCount(); ii++)
	{
		struct _tabPage tmp;
		int tmp2;
		if (m_tabPageArr.Lookup(ii - idx, tmp))
		{
			m_pFormItem->getFormPoint(ii)->tabN = m_pFormItem->getFormPoint(tmp.tabN);
			m_pFormItem->getFormPoint(ii)->pageN = tmp.pageN;
		}
		if (m_groupArr.Lookup(ii - idx, tmp2))
			m_pFormItem->getFormPoint(ii)->group = m_pFormItem->getFormPoint(tmp2);
	}
}

void CMapLoad::SaveScriptFile(CString sPath, CString sScript)
{
	CFile	ffile;

	if (!m_mapH->pythonMode && (sScript.Find("def ") >= 0 || sScript.Find("import ") >= 0))
		m_mapH->pythonMode = true;

	if (ffile.Open(sPath, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
	{
		ffile.SeekToEnd();
		ffile.Write(sScript, sScript.GetLength());
		ffile.Close();
	}
}
