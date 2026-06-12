// mapload.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "awBuild.h"
#include "mapload.h"
#include "maptbl.hxx"

#include "../../h/axisvar.h"
#include "../awTool/awObjectLoad.h"

// CMapLoad

CMapLoad::CMapLoad()
{
}

CMapLoad::~CMapLoad()
{
}

void CMapLoad::Load(class mapForm* mapForm, CString curdir)
{
	m_mapH  = mapForm;

	m_rtype  = 0;
	m_script = _T("");
	m_tabPageArr.RemoveAll();
	m_groupArr.RemoveAll();
}

void CMapLoad::BuildXmlHeader(CNode* pElementNode)
{
	CMap<CString, LPCSTR, CString, LPCSTR> mapData;
	for (int ii = 0; ii < pElementNode->GetCount(); ii++)
	{
		ElementNode* pNode = pElementNode->GetAt(ii);
		mapData.SetAt(pNode->strElement, pNode->strValue);
	}

	CString strData, strTmp;
	int pos, ii;

	if (mapData.Lookup("MAPNAME", strData))
	{
		strData.Trim();
		memcpy(m_mapH->mapN, strData.GetBuffer(0), strData.GetLength());
	}

	if (mapData.Lookup("MEDIA", strData))
	{
		strData.Trim();
		m_mapH->media = atoi(strData);
	}

	if (mapData.Lookup("MAPSIZE", strData))
	{
		pos = strData.Find(",");
		m_mapH->height = atoi(strData.Left(pos));
		m_mapH->width = atoi(strData.Mid(pos + 1));
	}
	if (mapData.Lookup("TITLE", strData))
	{
		strData.Replace("\"", "");
		strData.Trim();
		strcpy_s(m_mapH->caption, sizeof(m_mapH->caption), strData.GetBuffer(0));
	}
	if (mapData.Lookup("FONT", strData))
	{
		pos = strData.Find(",");
		strTmp = strData.Left(pos);
		strData = strData.Mid(pos + 1);
		strTmp.Trim();
		strTmp.Replace("\"", "");
		strcpy_s(m_mapH->fName, sizeof(m_mapH->fName), strTmp.GetBuffer(0));
		pos = strData.Find(",");
		m_mapH->fPoint = (BYTE)(atoi(strData.Left(pos)));
		strTmp = strData.Mid(pos + 1);
		strTmp.Trim();
		for (ii = 0; ii < FSTYLE; ii++)
		{
			if (!strTmp.CompareNoCase(fStyle[ii].strName))
			{
				m_mapH->fStyle = BYTE(fStyle[ii].defValue);
				break;
			}
		}
	}
	if (mapData.Lookup("OPTIONS", strData))
	{
		strData.Trim();
		m_mapH->options = 0;
		while (!strData.IsEmpty())
		{
			if ((pos = strData.Find('|')) != -1)
			{
				strTmp = strData.Left(pos);
				strData = strData.Mid(pos + 1);
			}
			else
			{
				strTmp = strData;
				strData = _T("");
			}

			int jj;
			for (jj = 0; jj < OPTIONS; jj++)
			{
				if (!strTmp.CompareNoCase(options[jj].strName))
					break;
			}
			if (jj >= OPTIONS)
				continue;

			m_mapH->options |= options[jj].defValue;
		}
	}
	if (mapData.Lookup("REPEATTIME", strData))
	{
		strData.Trim();
		m_mapH->repeatV = atoi(strData);
	}
	if (mapData.Lookup("HELPFILE", strData))
	{
		strData.Trim();
		strcpy_s(m_mapH->helpN, sizeof(m_mapH->helpN), strData.GetBuffer(0));
	}
	if (mapData.Lookup("VERSION", strData))
	{
		strData.Trim();
		m_mapH->vers = (BYTE)atoi(strData);
	}
	if (mapData.Lookup("MAPDESC", strData))
	{
		strData.Replace("\"", "");
		strData.Trim();
		strcpy_s(m_mapH->mapdesc, sizeof(m_mapH->mapdesc), strData.GetBuffer(0));
	}
	if (mapData.Lookup("MAPKIND", strData))
	{
		strData.Trim();
		for (ii = 0; ii < MAPKIND; ii++)
		{
			if (!strData.CompareNoCase(mapKind[ii].strName))
			{
				m_mapH->mapK = mapKind[ii].defValue;
				break;
			}
		}
	}
	if (mapData.Lookup("BGROUND", strData))
	{
		strData.Trim();
		pos = strData.Find(",");
		strTmp = strData.Left(pos);
		strData = strData.Mid(pos + 1);
		for (ii = 0; ii < BGROUND; ii++)
		{
			if (!strTmp.CompareNoCase(bground[ii].strName))
				break;
		}
		if (ii < BGROUND)
		{
			m_mapH->imageK = bground[ii].defValue;
			if (m_mapH->imageK == IK_BMP)
			{
				strData.Replace("\"", "");
				strData.Trim();
				if (strData.GetLength() < sizeof(m_mapH->imageV))
				{
					sprintf_s(m_mapH->imageV, sizeof(m_mapH->imageV), "%s", strData);
				}
			}
			else if (m_mapH->imageK == IK_BRUSH)
				m_mapH->bkClr = atoi(strData);
		}
	}
	if (mapData.Lookup("MAPTYPE", strData))
	{
		strData.Trim();
		m_mapH->type = 0;
		for (int ii = 0; ii < MAPTYPE; ii++)
		{
			if (!strData.CompareNoCase(mapType[ii].strName))
			{
				m_mapH->type = mapType[ii].defValue;
				break;
			}
		}
	}
	if (mapData.Lookup("MAPPUBLIC", strData))
	{
		if (strData = _T(";"))
			m_mapH->publicR[0] = '\0';
		else
		{
			strcpy_s(m_mapH->publicR, sizeof(m_mapH->publicR), strData.GetBuffer(0));
		}
	}
}

void CMapLoad::BuildXmlForm(CNode* pElementNode, CNode* pCellPropNode)
{
	CString strTmp;
	int pos;
	int	repN, multi, offs;
	CMap<CString, LPCSTR, CString, LPCSTR> mapData;
	ElementNode* pNode;
	for (int ii = 0; ii < pElementNode->GetCount(); ii++)
	{
		pNode = pElementNode->GetAt(ii);
		mapData.SetAt(pNode->strElement, pNode->strValue);
	}
	CArray<CMap<CString, LPCSTR, CString, LPCSTR>, CMap<CString, LPCSTR, CString, LPCSTR>> aryCell;
	CformProp* Prop = (CformProp*)((CObjectLoad*)m_mapH->forms.GetAt(m_mapH->forms.GetCount() - 1))->getProp();
	if (Prop->kind == fmGRID || Prop->kind == fmGRIDEX || Prop->kind == fmTABLE)
	{
		repN = -1;
		multi = 0;
		Prop->CellSelected = -1;
	}

	if (Prop->kind == fmIMVIEW)
		Prop->charType = bxLINE;
	if (Prop->kind == fmOUTPUT)
		Prop->ioType = ioOUTPUT;
	if (Prop->kind == fmIMVIEW || Prop->kind == fmBUTTON)
		Prop->alignment = atCENTER;

	Prop->styles &= ~(stENABLE | stVISIBLE | stCHECKED); // �⺻ ���ð� ����
	// NAME
	CString strData;
	if (mapData.Lookup("NAME", strData))
	{
		strData.Trim();
		Prop->name = strData;
	}

	if (mapData.Lookup("RECT", strData))
	{
		strData.Trim();
		pos = strData.Find(",");
		strTmp = strData.Left(pos);
		strData = strData.Mid(pos + 1);
		Prop->rect.left = atoi(strTmp);

		pos = strData.Find(",");
		strTmp = strData.Left(pos);
		strData = strData.Mid(pos + 1);
		Prop->rect.top = atoi(strTmp);

		pos = strData.Find(",");
		strTmp = strData.Left(pos);
		strData = strData.Mid(pos + 1);
		Prop->rect.right = atoi(strTmp);

		Prop->rect.bottom = atoi(strData);
	}

	if (Prop->rect.left == Prop->rect.right)
		Prop->rect.right = Prop->rect.left + 120;
	if (Prop->rect.top == Prop->rect.bottom)
		Prop->rect.bottom = Prop->rect.top + 40;

	switch (Prop->kind)
	{
	case fmEDIT:
	case fmCOMBO:
	case fmOUTPUT:
	case fmMEMO:
	case fmTREE:
	case fmOBJECT:
		if (mapData.Lookup("TYPE", strData))
		{
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find('|')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}

				for (int kk = 0; kk < IOTBL; kk++)
				{
					if (!strTmp.CompareNoCase(ioTbl[kk].strName))
					{
						Prop->charType = ioTbl[kk].charType;
						//token[jj].type = TOK_USE;
						break;
					}
				}
			}
		}
	}

	if (mapData.Lookup("CLASS", strData))
	{
		strData.Trim();
		Prop->objClass = strData;
	}

	if (mapData.Lookup("ALPHA", strData))
	{
		strData.Trim();
		Prop->alpha = atoi(strData);
	}

	if (mapData.Lookup("MARGIN", strData))
	{
		strData.Trim();
		Prop->objClass = strData;
	}

	if (mapData.Lookup("HINTTEXTCOLR", strData))
	{
		strData.Trim();
		Prop->paintColor1 = atoi(strData);
	}

	if (mapData.Lookup("LINECOLOR", strData))
	{
		strData.Trim();
		Prop->borderColor = atoi(strData);
	}

	if (mapData.Lookup("OPTICOLOR", strData))
	{
		strData.Trim();
		Prop->auxColor = atoi(strData);
	}

	if (mapData.Lookup("AUXIMAGES", strData))
	{
		strData.Trim();
		Prop->auxImages = strData;
	}

	if (mapData.Lookup("RTSSHAPE", strData))
	{
		strData.Trim();
		Prop->rtsShape = atoi(strData);
	}

	if (mapData.Lookup("RTS", strData))
	{
		Prop->flashKeys = strData;
	}

	if (mapData.Lookup("STRING", strData))
	{
		strData.Replace("\'", "");
		Prop->strings = strData;
		Prop->size = strData.GetLength();
	}

	if (mapData.Lookup("IMAGES", strData))
	{
		strData.Replace("\"", "");
		Prop->images = strData;
		Prop->size = strData.GetLength();
	}
	int kk;
	switch (Prop->kind)
	{
	case fmOBJECT:
		if (mapData.Lookup("BXSTYLE", strData))
		{
			for (kk = 0; kk < BOXTBL; kk++)
			{
				if (!strData.CompareNoCase(boxTbl[kk].strName))
				{
					Prop->charType = boxTbl[kk].type;
					break;
				}
			}
		}
		if (mapData.Lookup("TYPE", strData))
		{
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find('|')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}

				for (kk = 0; kk < EMODETBL; kk++)
				{
					if (!strTmp.CompareNoCase(emodeTbl[kk].strName))
					{
						Prop->ioType = emodeTbl[kk].ioType;
						break;
					}
				}
				if (kk < EMODETBL)
					continue;

				for (kk = 0; kk < ASENDTBL; kk++)
				{
					if (!strTmp.CompareNoCase(asendTbl[kk].strName))
					{
						Prop->auxAttributes |= asendTbl[kk].auxAttributes;
						break;
					}
				}
				if (kk < ASENDTBL)
					continue;
			}

		}

		if (mapData.Lookup("IFRGB", strData))
		{
			Prop->whatEdit = weCORGB;
		}
		if (mapData.Lookup("EDITFORMAT", strData))
		{
			Prop->whatEdit = weIT;
			strData.Replace("\"", "");
			Prop->editFormat = strData;
		}

		if (mapData.Lookup("BORDERSTYLE", strData))
		{
			for (kk = 0; kk < PANELTBL; kk++)
			{
				if (!strTmp.CompareNoCase(panelTbl[kk].strName))
				{
					Prop->borderStyle = panelTbl[kk].borders;
					break;
				}
			}
		}
		if (mapData.Lookup("TYPE", strData))
		{
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find('|')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}

				for (kk = 0; kk < ZEROSUPTBL; kk++)
				{
					if (!strTmp.CompareNoCase(zerosupTbl[kk].strName))
					{
						Prop->auxAttributes |= zerosupTbl[kk].auxAttributes;
						break;
					}
				}
				if (kk < ZEROSUPTBL)
					continue;

				for (kk = 0; kk < CORGBTBL; kk++)
				{
					if (!strTmp.CompareNoCase(coRgbTbl[kk].strName))
					{
						Prop->auxAttributes |= coRgbTbl[kk].auxAttributes;
						break;
					}
				}
				if (kk < CORGBTBL)
					continue;

				for (kk = 0; kk < ASENDTBL; kk++)
				{
					if (!strTmp.CompareNoCase(asendTbl[kk].strName))
					{
						Prop->auxAttributes |= asendTbl[kk].auxAttributes;
						break;
					}
				}
				if (kk < ASENDTBL)
					continue;

				for (kk = 0; kk < CONTRASTTBL; kk++)
				{
					if (!strTmp.CompareNoCase(contrastTbl[kk].strName))
					{
						Prop->auxAttributes |= contrastTbl[kk].auxAttributes;
						break;
					}
				}
				if (kk < CONTRASTTBL)
					continue;

				for (kk = 0; kk < DATETBL; kk++)
				{
					if (!strTmp.CompareNoCase(dateTbl[kk].strName))
					{
						Prop->auxAttributes |= dateTbl[kk].auxAttributes;
						break;
					}
				}
				if (kk < DATETBL)
					continue;
			}
		}

		if (mapData.Lookup("BORDERSTYLE", strData))
		{
			for (kk = 0; kk < PANELTBL; kk++)
			{
				if (!strData.CompareNoCase(panelTbl[kk].strName))
				{
					Prop->borderStyle = panelTbl[kk].borders;
					if (panelTbl[kk].borders != bsNONE && Prop->kind != fmEDIT)
						Prop->ioType = bxBORDER;
					break;
				}
			}
		}

		if (mapData.Lookup("IFRGB", strData))
		{
			Prop->whatEdit = weCORGB;
			if ((pos = strData.Find(',')) != -1)
			{
				strTmp = strData.Left(pos);
				strData = strData.Mid(pos + 1);

				WORD	upRGB, dnRGB;
				upRGB = atoi(strTmp);
				dnRGB = atoi(strData);
				Prop->auxValues = MAKELONG(dnRGB, upRGB);
			}
		}
		if (mapData.Lookup("EDITFORMAT", strData))
		{
			Prop->whatEdit = weIT;
			strData.Replace("\"", "");
			Prop->editFormat = strData;
		}

		if (mapData.Lookup("TYPE", strData))
		{
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find('|')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}

				for (kk = 0; kk < EMODETBL; kk++)
				{
					if (!strTmp.CompareNoCase(emodeTbl[kk].strName))
					{
						Prop->ioType = emodeTbl[kk].ioType;
						break;
					}
				}
			}
		}
		if (mapData.Lookup("TYPE", strData))
		{
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find('|')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}

				for (kk = 0; kk < EMODETBL; kk++)
				{
					if (!strTmp.CompareNoCase(emodeTbl[kk].strName))
					{
						Prop->ioType = emodeTbl[kk].ioType;
						break;
					}
				}
			}
		}

		if (mapData.Lookup("BORDERSTYLE", strData))
		{
			for (kk = 0; kk < PANELTBL; kk++)
			{
				if (!strData.CompareNoCase(panelTbl[kk].strName))
				{
					Prop->borderStyle = panelTbl[kk].borders;
					break;
				}
			}
		}
		if (mapData.Lookup("BXSTYLE", strData))
		{
			for (kk = 0; kk < BOXTBL; kk++)
			{
				if (!strData.CompareNoCase(boxTbl[kk].strName))
				{
					Prop->charType = boxTbl[kk].type;
					break;
				}
			}
		}
		if (mapData.Lookup("MULTISEL", strData))
		{
			Prop->styles |= stCHECKED;
		}
		if (mapData.Lookup("TYPE", strData))
		{
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find('|')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}

				for (kk = 0; kk < PANELTBL; kk++)
				{
					if (!strTmp.CompareNoCase(panelTbl[kk].strName))
					{
						Prop->borderStyle = panelTbl[kk].borders;
						Prop->charType = bxBORDER;
						break;
					}
				}
			}
		}
		if (mapData.Lookup("TYPE", strData))
		{
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find('|')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}

				for (kk = 0; kk < EMODETBL; kk++)
				{
					if (!strTmp.CompareNoCase(emodeTbl[kk].strName))
					{
						Prop->ioType = emodeTbl[kk].ioType;
						break;
					}
				}
			}
		}
		break;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case fmCOMBO:
		if (mapData.Lookup("TYPE", strData))
		{
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find('|')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}

				for (kk = 0; kk < EMODETBL; kk++)
				{
					if (!strTmp.CompareNoCase(emodeTbl[kk].strName))
					{
						Prop->ioType = emodeTbl[kk].ioType;
						break;
					}
				}
				if (kk < EMODETBL)
					continue;

				for (kk = 0; kk < ASENDTBL; kk++)
				{
					if (!strTmp.CompareNoCase(asendTbl[kk].strName))
					{
						Prop->auxAttributes |= asendTbl[kk].auxAttributes;
						break;
					}
				}
				if (kk < ASENDTBL)
					continue;
			}

		}

		if (mapData.Lookup("IFRGB", strData))
		{
			Prop->whatEdit = weCORGB;
		}
		if (mapData.Lookup("EDITFORMAT", strData))
		{
			Prop->whatEdit = weIT;
			strData.Replace("\"", "");
			Prop->editFormat = strData;
		}

		if (mapData.Lookup("BORDERSTYLE", strData))
		{
			for (kk = 0; kk < PANELTBL; kk++)
			{
				if (!strTmp.CompareNoCase(panelTbl[kk].strName))
				{
					Prop->borderStyle = panelTbl[kk].borders;
					break;
				}
			}
		}
		break;
	case fmEDIT:
	case fmOUTPUT:
		if (mapData.Lookup("TYPE", strData))
		{
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find('|')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}

				for (kk = 0; kk < ZEROSUPTBL; kk++)
				{
					if (!strTmp.CompareNoCase(zerosupTbl[kk].strName))
					{
						Prop->auxAttributes |= zerosupTbl[kk].auxAttributes;
						break;
					}
				}
				if (kk < ZEROSUPTBL)
					continue;

				for (kk = 0; kk < CORGBTBL; kk++)
				{
					if (!strTmp.CompareNoCase(coRgbTbl[kk].strName))
					{
						Prop->auxAttributes |= coRgbTbl[kk].auxAttributes;
						break;
					}
				}
				if (kk < CORGBTBL)
					continue;

				for (kk = 0; kk < ASENDTBL; kk++)
				{
					if (!strTmp.CompareNoCase(asendTbl[kk].strName))
					{
						Prop->auxAttributes |= asendTbl[kk].auxAttributes;
						break;
					}
				}
				if (kk < ASENDTBL)
					continue;

				for (kk = 0; kk < CONTRASTTBL; kk++)
				{
					if (!strTmp.CompareNoCase(contrastTbl[kk].strName))
					{
						Prop->auxAttributes |= contrastTbl[kk].auxAttributes;
						break;
					}
				}
				if (kk < CONTRASTTBL)
					continue;

				for (kk = 0; kk < DATETBL; kk++)
				{
					if (!strTmp.CompareNoCase(dateTbl[kk].strName))
					{
						Prop->auxAttributes |= dateTbl[kk].auxAttributes;
						break;
					}
				}
				if (kk < DATETBL)
					continue;

				
			}
		}

		if (mapData.Lookup("BORDERSTYLE", strData))
		{
			for (kk = 0; kk < PANELTBL; kk++)
			{
				if (!strData.CompareNoCase(panelTbl[kk].strName))
				{
					Prop->borderStyle = panelTbl[kk].borders;
					if (panelTbl[kk].borders != bsNONE && Prop->kind != fmEDIT)
						Prop->ioType = bxBORDER;
					break;
				}
			}
		}

		if (mapData.Lookup("IFRGB", strData))
		{
			Prop->whatEdit = weCORGB;
			if ((pos = strData.Find(',')) != -1)
			{
				strTmp = strData.Left(pos);
				strData = strData.Mid(pos + 1);

				WORD	upRGB, dnRGB;
				upRGB = atoi(strTmp);
				dnRGB = atoi(strData);
				Prop->auxValues = MAKELONG(dnRGB, upRGB);
			}
		}
		if (mapData.Lookup("EDITFORMAT", strData))
		{
			Prop->whatEdit = weIT;
			strData.Replace("\"", "");
			Prop->editFormat = strData;
		}
	case fmTABLE:
	case fmBROWSER:
	//case fmOBJECT:
		if (mapData.Lookup("TYPE", strData))
		{
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find('|')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}

				for (kk = 0; kk < EMODETBL; kk++)
				{
					if (!strTmp.CompareNoCase(emodeTbl[kk].strName))
					{
						Prop->ioType = emodeTbl[kk].ioType;
						break;
					}
				}
			}
		}
		
		break;
	case fmMEMO:
		if (mapData.Lookup("TYPE", strData))
		{
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find('|')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}

				for (kk = 0; kk < EMODETBL; kk++)
				{
					if (!strTmp.CompareNoCase(emodeTbl[kk].strName))
					{
						Prop->ioType = emodeTbl[kk].ioType;
						break;
					}
				}
			}
		}

		if (mapData.Lookup("BORDERSTYLE", strData))
		{
			for (kk = 0; kk < PANELTBL; kk++)
			{
				if (!strData.CompareNoCase(panelTbl[kk].strName))
				{
					Prop->borderStyle = panelTbl[kk].borders;
					break;
				}
			}
		}
		break;
	case fmBOX:
		if (mapData.Lookup("BXSTYLE", strData))
		{
			for (kk = 0; kk < BOXTBL; kk++)
			{
				if (!strData.CompareNoCase(boxTbl[kk].strName))
				{
					Prop->charType = boxTbl[kk].type;
					break;
				}
			}
		}
		break;
	case fmIMVIEW:
		if (mapData.Lookup("MULTISEL", strData))
		{
			Prop->styles |= stCHECKED;
		}
		if (mapData.Lookup("TYPE", strData))
		{
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find('|')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}

				for (kk = 0; kk < PANELTBL; kk++)
				{
					if (!strTmp.CompareNoCase(panelTbl[kk].strName))
					{
						Prop->borderStyle = panelTbl[kk].borders;
						Prop->charType = bxBORDER;
						break;
					}
				}
			}
		}
		break;
	case fmBUTTON:
	case fmRADIO:
	case fmCHECK:
		if (mapData.Lookup("TYPE", strData))
		{
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find('|')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}

				for (kk = 0; kk < EMODETBL; kk++)
				{
					if (!strTmp.CompareNoCase(emodeTbl[kk].strName))
					{
						Prop->ioType = emodeTbl[kk].ioType;
						break;
					}
				}
			}
		}
		
		break;
	}

	if (!(Prop->kind == fmGRID || Prop->kind == fmGRIDEX || Prop->kind == fmTABLE))
	{
		if (mapData.Lookup("ALIGNMENT", strData))
		{
			for (kk = 0; kk < ALIGNTBL; kk++)
			{
				if (!strData.CompareNoCase(alignTbl[kk].strName))
				{
					if (alignTbl[kk].alignment <= atBOTTOM)
						Prop->alignment = alignTbl[kk].alignment;
					else
						Prop->alignmentImage = alignTbl[kk].alignment;
					continue;
				}
			}
		}
		
		if (mapData.Lookup("TYPE", strData))
		{
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find('|')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}

				for (kk = 0; kk < ATTRTBL; kk++)
				{
					if (!strTmp.CompareNoCase(attrTbl[kk].strName))
					{
						Prop->attributes |= attrTbl[kk].attributes;
						continue;
					}
				}
			}
		}
	}

	if (mapData.Lookup("FONT", strData))
	{
		pos = strData.Find(",");
		strTmp = strData.Left(pos);
		strData = strData.Mid(pos + 1);
		strTmp.Replace("\"","");
		Prop->fontName = strTmp;
		pos = strData.Find(",");
		strTmp = strData.Left(pos);
		strData = strData.Mid(pos + 1);
		strData.Trim();
		Prop->fontPoint = atoi(strTmp);
		int ii = 0;
		for (ii = 0; ii < FSTYLE; ii++)
		{
			if (!strData.CompareNoCase(fStyle[ii].strName))
				break;
		}
		Prop->fontStyle = fStyle[ii].defValue;
	}
	if (mapData.Lookup("STYLES", strData))
	{
		while (!strData.IsEmpty())
		{
			if ((pos = strData.Find('|')) != -1)
			{
				strTmp = strData.Left(pos);
				strData = strData.Mid(pos + 1);
			}
			else
			{
				strTmp = strData;
				strData = _T("");
			}
			if (strTmp == "ENABLE")
			{
				Prop->styles |= stENABLE;
			}
			else if (strTmp == "VISIBLE")
			{
				Prop->styles |= stVISIBLE;
			}
			else if (strTmp == "CHECKED")
			{
				Prop->styles |= stCHECKED;
			}
			else if (strTmp == "TRANSPARENT")
			{
				Prop->styles |= stTRANSPARENT;
			}
			else if (strTmp == "IMAGE")
			{
				Prop->styles |= stIMAGE;
			}
			else if (strTmp == "HOVER")
			{
				Prop->styles |= stHOVER;
			}
			else if (strTmp == "SORT")
			{
				Prop->styles |= stSORT;
			}
			else if (strTmp == "HASLINES")
			{
				Prop->styles |= stLINES;
			}
			else if (strTmp == "VERTICAL")
			{
				Prop->styles |= stVERT;
			}
			else if (strTmp == "IMAGETEXT")
			{
				Prop->styles |= stIMAGETEXT;
			}
			else if (strTmp == "SHOWFORM")
			{
				Prop->styles |= stEDIT;
			}
			else if (strTmp == "ACCCBO")
			{
				Prop->styles |= stACCN;
			}
			else if (strTmp == "RESIZABLE")
			{
				Prop->auxAttributes |= gaFIX;
			}
		}
	}
	if (mapData.Lookup("HINTTYPE", strData))
	{
		int ii;
		for (ii = 0; ii < HINTTBL; ii++)
		{
			if (!strData.CompareNoCase(hintTbl[ii].strName))
			{
				Prop->typeHint = hintTbl[ii].type;
				break;
			}
		}
	}
	if (mapData.Lookup("FIXEDSIZE", strData))
	{
		Prop->bFixedSize = true;
	}
	if (mapData.Lookup("FIXEDSIZE", strData))
	{
		Prop->bFixedSize = true;
	}
	if (mapData.Lookup("HELPTEXT", strData))
	{
		strData.Replace("\"", "");
		Prop->hintText = strData;
	}
	if (mapData.Lookup("GROUPN", strData))
	{
		m_groupArr.SetAt(m_groupArr.GetCount() - 1, atoi(strData));
	}
	if (mapData.Lookup("TABIMAGE", strData))
	{
		strData.Replace("\"", "");
		Prop->images = strData;
	}
	switch (Prop->kind)
	{
	case fmOUTPUT:
	case fmEDIT:
	case fmCOMBO:
	case fmTAB:
	case fmMEMO:
	case fmTREE:
	case fmBROWSER:
	case fmOBJECT:
		if (mapData.Lookup("LEN", strData))
		{
			Prop->size = (WORD)atoi(strData);
		}
		if (mapData.Lookup("IDX", strData))
		{
			Prop->iCtmIdx = (WORD)atoi(strData);
		}
		if (mapData.Lookup("COLOR", strData))
		{
			pos = strData.Find(",");
			strTmp = strData.Left(pos);
			strData = strData.Mid(pos + 1);

			Prop->paintColor = atoi(strTmp);
			Prop->textColor = atoi(strData);
		}
		if (mapData.Lookup("DOM", strData))
		{
			Prop->keyNames = strData;
		}
		if (mapData.Lookup("COUNT", strData))
		{
			Prop->keyValues = MAKELONG(LOWORD((BYTE)atoi(strData)), HIWORD(Prop->keyValues));
		}
		if (mapData.Lookup("DISPLAY", strData))
		{
			strData.Replace("\"", "");
			CString data = _T("");
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find(',')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}
				data += strTmp;
				data += _T('\0');
			}
			Prop->displays = data;
		}
		if (mapData.Lookup("CAPTION", strData))
		{
			strData.Replace("\"", "");
			CString data = _T("");
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find(',')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}
				strTmp.Trim();
				data += strTmp;
				data += _T('\0');
			}
			Prop->displays = data;
		}
		if (mapData.Lookup("DATA", strData))
		{
			strData.Replace("\"", "");
			CString data = _T("");
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find(',')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}
				strTmp.Trim();
				data += strTmp;
				data += _T('\0');
			}
			Prop->strings = data;
		}
		if (mapData.Lookup("INIT", strData))
		{
			strData.Replace("\"", "");
			Prop->strings = strData;
		}
		if (mapData.Lookup("URL", strData))
		{
			strData.Replace("\"", "");
			Prop->strings = strData;
		}
		if (mapData.Lookup("FLOAT", strData))
		{
			Prop->offsets = (BYTE)atoi(strData);
		}
		if (mapData.Lookup("UNIT", strData))
		{
			strData.Replace("\"", "");
			CString data = _T("");
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find(',')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}
				data += strTmp;
				data += '\t';
			}
			Prop->strings = data;
		}
		if (mapData.Lookup("INDEX", strData))
		{
			Prop->checked = (BYTE)atoi(strData);
		}
		if (mapData.Lookup("OBJECT", strData))
		{
			Prop->keyNames = strData;
		}
		if (mapData.Lookup("TABINFO", strData))
		{
			strData.Replace("\"", "");
			CString data = _T("");
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find(',')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}
				strTmp.Trim();
				data += strTmp;
				data += _T('\0');
			}
			Prop->strings = data;
		}
		if (mapData.Lookup("MULTISEL", strData))
		{
			Prop->styles |= stCHECKED;
		}
		if (mapData.Lookup("CHARS", strData))
		{
			if (Prop->kind == fmMEMO)
			{
				Prop->keyValues = atoi(strData);
			}
		}
		if (mapData.Lookup("REFERENCE", strData))
		{
			Prop->referenceName = strData;
		}
		if (mapData.Lookup("DEFAULT", strData))
		{
			//Prop->displays = CString(token[jj].name, token[jj].len);
			//Prop->strings = CString(token[jj].name, token[jj].len);
		}
		if (mapData.Lookup("TABH", strData))
		{
			Prop->size = (BYTE)atoi(strData);
		}
		if (mapData.Lookup("SPIN", strData))
		{
			//Prop->styles |= stSPIN;
			//if (Prop->keyParam == 0)
			//	Prop->keyParam = 1;
			Prop->keyValues = atoi(strData);
		}
		if (mapData.Lookup("CODE", strData))
		{
			Prop->styles |= stCODE;
		}
		if (mapData.Lookup("SUM", strData))
		{
			Prop->styles |= stSUM;
		}
		if (mapData.Lookup("FCOLOR", strData))
		{
			pos = strData.Find(",");
			strTmp = strData.Left(pos);
			strData = strData.Mid(pos + 1);

			Prop->paintColor2 = atoi(strTmp);
			Prop->paintColor1 = atoi(strData);
		}
		if (mapData.Lookup("VCOUNT", strData))
		{
			Prop->keyValues = MAKELONG((WORD)Prop->keyValues, atoi(strData));
		}
		if (mapData.Lookup("CVAR", strData))
		{
			Prop->editFormat = strData;
		}
		if (mapData.Lookup("TABORDER", strData))
		{
			Prop->index = atoi(strData);
		}
		//int pageN = atoi(token[jj++].name), formCnt = atoi(token[jj].name);
		//for (int nLoop = 0; nLoop < formCnt; nLoop++)
		//{
		//	struct _tabPage pageR;
		//	pageR.tabN = fN;
		//	pageR.pageN = pageN;
		//	m_tabPageArr.SetAt(atoi(token[++jj].name), pageR);
		//}
		if (mapData.Lookup("IOORDER", strData))
		{
			Prop->iorder = atoi(strData);
		}
		if (mapData.Lookup("MATCHKEY", strData))
		{
			for (kk = 0; kk < VKTBL; kk++)
			{
				if (!strData.CompareNoCase(vkTbl[kk].strName))
				{
					Prop->auxValues = kk;
					//formR->onEdit = kk;
					break;
				}
			}
		}
		if (mapData.Lookup("SPINTYPE", strData))
		{
			Prop->keyParam = atoi(strData);
			if (Prop->kind == fmEDIT)
			{
				if (Prop->keyParam == 0) Prop->styles &= ~stSPIN;
				else Prop->styles |= stSPIN;
			}
		}
		if (mapData.Lookup("ENCRYPT", strData))
		{
			strData.Trim();
			Prop->keyNames = strData;
		}
		if (mapData.Lookup("LINECOLOR", strData))
		{
			Prop->borderColor = atoi(strData);
			Prop->ioType = ioINPUT;
		}
		if (mapData.Lookup("TYPE", strData))
		{
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find('|')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}

				if (strTmp == "LIST")
				{
					Prop->styles |= stLIST;
				}

				if (strTmp == "HASTOOL")
				{
					Prop->styles |= stCHECKED;
				}

				if (strTmp == "FOCUS")
				{
					Prop->styles |= stCHECKED;
				}
			}
		}
		break;
	case fmGRID:
	case fmGRIDEX:
		if (pNode->strElement == _T("TYPE"))
		{
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find('|')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}

				for (kk = 0; kk < EMODETBL; kk++)
				{
					if (!strData.CompareNoCase(emodeTbl[kk].strName))
					{
						((cellOb*)Prop->cellProperties->GetAt(repN))->ioType = emodeTbl[kk].ioType;
						break;
					}
				}
			}
		}
	case fmTABLE:
		for (int ii = 0; ii < pCellPropNode->GetCount(); ii++)
		{
			pNode = pCellPropNode->GetAt(ii);
			strData = pNode->strValue;
			if (pNode->strElement == _T("COLINFO"))
			{
				repN++;
			}
			else if (pNode->strElement == _T("NAME"))
			{
				strData.Replace("\"", "");
				((cellOb*)Prop->cellProperties->GetAt(repN))->name = strData;
				((cellOb*)Prop->cellProperties->GetAt(repN))->lineN = (BYTE)multi;
				((cellOb*)Prop->cellProperties->GetAt(repN))->ioType = ioOUTPUT;
			}
			else if (pNode->strElement == _T("RTS"))
			{
				((cellOb*)Prop->cellProperties->GetAt(repN))->flashKeys = strData;
			}
			else if (pNode->strElement == _T("TYPE"))
			{
				while (!strData.IsEmpty())
				{
					if ((pos = strData.Find('|')) != -1)
					{
						strTmp = strData.Left(pos);
						strData = strData.Mid(pos + 1);
					}
					else
					{
						strTmp = strData;
						strData = _T("");
					}

					for (kk = 0; kk < IOTBL; kk++)
					{
						if (!strTmp.CompareNoCase(ioTbl[kk].strName))
						{
							((cellOb*)Prop->cellProperties->GetAt(repN))->charType = ioTbl[kk].charType;
							break;
						}
					}

					for (kk = 0; kk < ATTRTBL; kk++)
					{
						if (!strTmp.CompareNoCase(attrTbl[kk].strName))
						{
							((cellOb*)Prop->cellProperties->GetAt(repN))->attributes |= attrTbl[kk].attributes;
							if (attrTbl[kk].attributes == faWRAP)
								multi++;
							break;
						}
					}

					for (kk = 0; kk < ZEROSUPTBL; kk++)
					{
						if (!strTmp.CompareNoCase(zerosupTbl[kk].strName))
						{
							((cellOb*)Prop->cellProperties->GetAt(repN))->auxAttributes |= zerosupTbl[kk].auxAttributes;
							break;
						}
					}

					for (kk = 0; kk < CORGBTBL; kk++)
					{
						if (!strTmp.CompareNoCase(coRgbTbl[kk].strName))
						{
							((cellOb*)Prop->cellProperties->GetAt(repN))->auxAttributes |= coRgbTbl[kk].auxAttributes;
							break;
						}
					}

					for (kk = 0; kk < CONTRASTTBL; kk++)
					{
						if (!strTmp.CompareNoCase(contrastTbl[kk].strName))
						{
							((cellOb*)Prop->cellProperties->GetAt(repN))->auxAttributes |= contrastTbl[kk].auxAttributes;
							break;
						}
					}

					if (strTmp == "COLUMN_VISIBLE")
					{
						((cellOb*)Prop->cellProperties->GetAt(repN))->styles |= stVISIBLE;
					}

					else if (strTmp == "COLUMN_SORT")
					{
						((cellOb*)Prop->cellProperties->GetAt(repN))->styles |= stSORT;
					}
					else if (strTmp == "ICOMBO")
					{
						((cellOb*)Prop->cellProperties->GetAt(repN))->kind = ckCOMBO;
					}
					else if (strTmp == "ICHECK")
					{
						((cellOb*)Prop->cellProperties->GetAt(repN))->kind = ckCHECK;
					}
					else if (strTmp == "IGRAPH")
					{
						((cellOb*)Prop->cellProperties->GetAt(repN))->kind = ckCHART;
					}
					else if (strTmp == "SUPRESS")
					{
						((cellOb*)Prop->cellProperties->GetAt(repN))->styles |= stSUPRESS;
					}
					else if (strTmp == "IBUTTON")
					{
						((cellOb*)Prop->cellProperties->GetAt(repN))->kind = ckBUTTON;
					}
					else if (strTmp == "COLUMN_IMAGE")
					{
						((cellOb*)Prop->cellProperties->GetAt(repN))->styles |= stIMAGE;
					}
					else if (strTmp == "COLUMN_IMGTEXT")
					{
						((cellOb*)Prop->cellProperties->GetAt(repN))->styles |= stIMAGETEXT;
					}
					else if (strTmp == "COLUMN_SHOWFORM")
					{
						((cellOb*)Prop->cellProperties->GetAt(repN))->styles |= stEDIT;
					}
					else if (strTmp == "COLUMN_EXCHECKE")
					{
						((cellOb*)Prop->cellProperties->GetAt(repN))->styles |= stCHECKED;
					}
					else if (strTmp == "COLUMN_BOLD")
					{
						((cellOb*)Prop->cellProperties->GetAt(repN))->styles |= stBOLD;
					}
				}
			}
			if (pNode->strElement == "LEN")
			{
				((cellOb*)Prop->cellProperties->GetAt(repN))->size = (WORD)atoi(strData);
			}
			if (pNode->strElement == "COLOR")
			{
				strData.Trim();
				pos = strData.Find(",");
				strTmp = strData.Left(pos);
				strData = strData.Mid(pos + 1);

				((cellOb*)Prop->cellProperties->GetAt(repN))->paintColor = atoi(strTmp);
				((cellOb*)Prop->cellProperties->GetAt(repN))->textColor = atoi(strData);
			}
			if (pNode->strElement == "DOM")
			{
				strData.Replace("\"", "");
				strData.Trim();
				((cellOb*)Prop->cellProperties->GetAt(repN))->dominoName = strData;
			}
			if (pNode->strElement == "HEAD")
			{
				strData.Replace("\"","");
				((cellOb*)Prop->cellProperties->GetAt(repN))->headText = strData;
				strTmp = ((cellOb*)Prop->cellProperties->GetAt(repN))->headText;
			}
			if (pNode->strElement == "WIDTH")
			{
				((cellOb*)Prop->cellProperties->GetAt(repN))->width = (WORD)atoi(strData);
			}
			if (pNode->strElement == "HEADCOLOR")
			{
				strData.Trim();
				pos = strData.Find(",");
				strTmp = strData.Left(pos);
				strData = strData.Mid(pos + 1);
				((cellOb*)Prop->cellProperties->GetAt(repN))->headPaintColor = atoi(strTmp);
				((cellOb*)Prop->cellProperties->GetAt(repN))->headTextColor = atoi(strData);
			}
			if (pNode->strElement == "COLUMN_VISIBLE")
			{
				((cellOb*)Prop->cellProperties->GetAt(repN))->width = (WORD)atoi(strData);
			}
			if (pNode->strElement == "FLOAT")
			{
				((cellOb*)Prop->cellProperties->GetAt(repN))->offsets = (BYTE)atoi(strData);
			}
			if (pNode->strElement == "COLALIGN")
			{
				strData.Trim();
				pos = strData.Find(",");
				strTmp = strData.Left(pos);
				strData = strData.Mid(pos + 1);

				for (int ii = 0; ii < ALIGNTBL; ii++)
				{
					if (!strTmp.Compare(alignTbl[ii].strName))
					{
						((cellOb*)Prop->cellProperties->GetAt(repN))->alignmentHead = alignTbl[ii].alignment;
						break;
					}
				}
				for (int ii = 0; ii < ALIGNTBL; ii++)
				{
					if (!strData.Compare(alignTbl[ii].strName))
					{
						((cellOb*)Prop->cellProperties->GetAt(repN))->alignmentText = alignTbl[ii].alignment;
						break;
					}
				}
			}
			if (pNode->strElement == "REFERENCE")
			{
				strData.Replace("\"", "");
				strData.Trim();
				((cellOb*)Prop->cellProperties->GetAt(repN))->referenceName = strData;
			}
			if (pNode->strElement == "COUNT")
			{
				((cellOb*)Prop->cellProperties->GetAt(repN))->entryCombo = (BYTE)atoi(strData);
			}
			if (pNode->strElement == "DISPLAY")
			{
				while (!strData.IsEmpty())
				{
					if ((pos = strData.Find(',')) != -1)
					{
						strTmp = strData.Left(pos);
						strData = strData.Mid(pos + 1);
					}
					else
					{
						strTmp = strData;
						strData = _T("");
					}
					strTmp.Replace("\"", "");

					offs = 0;
					for (kk = 0; kk < ((cellOb*)Prop->cellProperties->GetAt(repN))->entryCombo; kk++)
					{
						((cellOb*)Prop->cellProperties->GetAt(repN))->displays += strTmp;
						((cellOb*)Prop->cellProperties->GetAt(repN))->displays += '\0';
					}
				}
			}

			if (pNode->strElement == "DATA")
			{
				while (!strData.IsEmpty())
				{
					if ((pos = strData.Find(',')) != -1)
					{
						strTmp = strData.Left(pos);
						strData = strData.Mid(pos + 1);
					}
					else
					{
						strTmp = strData;
						strData = _T("");
					}
					strTmp.Replace("\"", "");

					offs = 0;
					for (kk = 0; kk < ((cellOb*)Prop->cellProperties->GetAt(repN))->entryCombo; kk++)
					{
						((cellOb*)Prop->cellProperties->GetAt(repN))->strings += strTmp;
						((cellOb*)Prop->cellProperties->GetAt(repN))->strings += '\0';
					}
				}
			}
			if (pNode->strElement == "CHECKSTR")
			{
				strData.Replace("\"", "");
				strData.Trim();
				((cellOb*)Prop->cellProperties->GetAt(repN))->strings = strData;
			}
			if (pNode->strElement == "EDITFORMAT")
			{
				strData.Replace("\"", "");
				strData.Trim();
				((cellOb*)Prop->cellProperties->GetAt(repN))->editFormat = strData;
				((cellOb*)Prop->cellProperties->GetAt(repN))->whatEdit = weIT;
			}
			if (pNode->strElement == "IFRGB")
			{
				strData.Trim();
				pos = strData.Find(",");
				strTmp = strData.Left(pos);
				strData = strData.Mid(pos + 1);

				strData = strData.Mid(pos + 1);
				WORD upRGB = atoi(strTmp);
				WORD dnRGB = atoi(strData);
				((cellOb*)Prop->cellProperties->GetAt(repN))->auxValues = MAKELONG(dnRGB, upRGB);
				((cellOb*)Prop->cellProperties->GetAt(repN))->whatEdit = weCORGB;
				((cellOb*)Prop->cellProperties->GetAt(repN))->attributes = faCORGB;
			}
			if (pNode->strElement == "INORMAL")
			{
				((cellOb*)Prop->cellProperties->GetAt(repN))->kind = ckNORMAL;
			}
			if (pNode->strElement == "MATCHKEY")
			{
				for (kk = 0; kk < VKTBL; kk++)
				{
					if (!strData.CompareNoCase(vkTbl[kk].strName))
					{
						((cellOb*)Prop->cellProperties->GetAt(repN))->keyParam = kk;
						break;
					}
				}
			}
			if (pNode->strElement == "BUTTONSTR")
			{
				strData.Replace("\"", "");
				strData.Trim();
				((cellOb*)Prop->cellProperties->GetAt(repN))->displays = strData;
			}
			if (pNode->strElement == "BUTTONIMG")
			{
				strData.Replace("\"", "");
				strData.Trim();
				((cellOb*)Prop->cellProperties->GetAt(repN))->images = strData;
			}
			if (pNode->strElement == "COLHELPTEXT")
			{
				strData.Replace("\"", "");
				strData.Trim();
				((cellOb*)Prop->cellProperties->GetAt(repN))->hintText = strData;
			}
		}
		if (mapData.Lookup("VALID", strData))
		{
			Prop->keyValues = MAKELONG(LOWORD(Prop->keyValues), (WORD)atoi(strData));
		}
		if (mapData.Lookup("CELLH", strData))
		{
			Prop->keyValues = MAKELONG(atoi(strData), HIWORD(Prop->keyValues));
			Prop->iAutoSize = atoi(strData);
		}
		if (mapData.Lookup("TYPE", strData))
		{
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find('|')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}
				if (strTmp == "LEAVE")
				{
					Prop->attributes |= faWRAP;
				}
				else if (strTmp == "CELLEDIT")
				{
					Prop->attributes |= faENUM;
				}
				else if (strTmp == "PAGE")
				{
					Prop->auxAttributes |= gaPAGE;
				}
				else if (strTmp == "HEADER")
				{
					Prop->auxAttributes |= gaHEADER;
				}
				else if (strTmp == "TABLE")
				{
					Prop->auxAttributes |= gaTABLE;
				}
				else if (strTmp == "THUMB")
				{
					Prop->auxAttributes |= gaTHUMB;
				}
				else if (strTmp == "CXDELETE")
				{
					Prop->attributes |= faXDELETE;
				}
				else if (strTmp == "AUTOSIZE")
				{
					Prop->auxAttributes |= gaAUTOSIZE;
					Prop->keyValues = 0;
				}
				else if (strTmp == "CUD")
				{
					Prop->auxAttributes |= gaCUD;
				}
				else if (strTmp == "ISFOCUS")
				{
					Prop->styles |= stCHECKED;
				}
				else if (strTmp == "MERGE")
				{
					Prop->auxAttributes |= gaMERGE;
				}
				else if (strTmp == "KEYFOCUS")
				{
					Prop->auxAttributes |= gaKEY;
				}
				else if (strTmp == "SELALWAYS")
				{
					Prop->auxAttributes |= gaALWAYS;
				}
				else if (strTmp == "GRFIX")
				{
					Prop->auxAttributes |= gaFIX;
				}
				else if (strTmp == "GRFLEXEXPAND")
				{
					Prop->auxAttributes |= gaEXPAND;
				}
				else if (strTmp == "GRXSIZE")
				{
					Prop->auxAttributes |= gaXSIZE;
				}
				else if (strTmp == "GRAUTOWIDTH")
				{
					Prop->auxAttributes |= gaAUTO;
				}
				else if (strTmp == "GRVALID")
				{
					Prop->auxAttributes |= gaVALID;
				}
				else if (strTmp == "GRFTEXT")
				{
					Prop->auxAttributes |= gaFTEXT;
				}
				else if (strTmp == "GRXXSL")
				{
					Prop->auxAttributes |= gaXXLS;
				}
				else if (strTmp == "ONLYHEAD3D")
				{
					Prop->auxAttributes |= ga3DHEAD;
				}
				else if (strTmp == "FADROP")
				{
					Prop->attributes |= faDROP;
				}
				else if (strTmp == "MULTI")
				{
					Prop->auxAttributes |= gaMULTI;
				}
				else if (strTmp == "FLEXIBLE")
				{
					Prop->auxAttributes |= gaFLEX;
				}
				else if (strTmp == "VSCROLL")
				{
					Prop->auxAttributes |= gaVSCR;
				}
				else if (strTmp == "HSCROLL")
				{
					Prop->auxAttributes |= gaHSCR;
				}
				else if (strTmp == "INS_TOP")
				{
					Prop->auxAttributes |= gaTOP;
				}
				else if (strTmp == "NOHEAD")
				{
					Prop->auxAttributes |= gaXHEAD;
				}
				else if (strTmp == "NO3D")
				{
					Prop->auxAttributes |= gaX3D;
				}
				else if (strTmp == "RROW")
				{
					Prop->auxAttributes |= gaROW;
				}
				else if (strTmp == "XVLINE")
				{
					Prop->auxAttributes |= gaXVLINE;
				}
				else if (strTmp == "XHLINE")
				{
					Prop->auxAttributes |= gaXHLINE;
				}
			}
		}
		if (mapData.Lookup("COLUMNS", strData))
		{
		}
		if (mapData.Lookup("MATRIX", strData))
		{
			strData.Trim();
			pos = strData.Find(",");
			strTmp = strData.Left(pos);
			strData = strData.Mid(pos + 1);
			Prop->size = (WORD)atoi(strTmp);
		}
		if (mapData.Lookup("FOCUSKIND", strData))
		{
			Prop->whatEdit = atoi(strData);
		}
		if (mapData.Lookup("LINECOLOR", strData))
		{
			Prop->borderColor = atoi(strData);
		}
		//if (mapData.Lookup("LINEW", strData))
		//{
		//}
		if (mapData.Lookup("FIX", strData))
		{
			Prop->fixColumn = atoi(strData);
		}
		if (mapData.Lookup("VARIANT", strData))
		{
			strData.Replace("\"","");
			strData.Trim();
			Prop->strings = strData;
		}
		if (mapData.Lookup("ROWC", strData))
		{
			strData.Trim();
			pos = strData.Find(",");
			strTmp = strData.Left(pos);
			strData = strData.Mid(pos + 1);

			Prop->paintColor2 = atoi(strTmp);
			Prop->paintColor1 = atoi(strData);
		}
		if (mapData.Lookup("TABORDER", strData))
		{
			Prop->index = atoi(strData);
		}
		if (mapData.Lookup("IOORDER", strData))
		{
			Prop->iorder = atoi(strData);
		}
		if (mapData.Lookup("FOCUSCOLOR", strData))
		{
			Prop->auxValues = MAKELONG(LOWORD(atoi(strData)), HIWORD(Prop->auxValues));
		}
		if (mapData.Lookup("COLHINTTYPE", strData))
		{
			for (kk = 0; kk < HINTTBL; kk++)
			{
				if (!strData.CompareNoCase(hintTbl[kk].strName))
				{
					((cellOb*)Prop->cellProperties->GetAt(repN))->typeHint = hintTbl[kk].type;
					break;
				}
			}
		}
		if (mapData.Lookup("FOCUSCOLOR", strData))
		{
			Prop->auxValues = MAKELONG(LOWORD(atoi(strData)), HIWORD(Prop->auxValues));
		}
		//if (mapData.Lookup("GRSHARE", strData))
		//{
		//}
		if (mapData.Lookup("ROWCC", strData))
		{
			Prop->rowLines = atoi(strData);
		}
		if (mapData.Lookup("INIT", strData))
		{
			strData.Replace("\"", "");
			Prop->strings = strData;
		}
		break;
	default:
		if (mapData.Lookup("TYPE", strData))
		{
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find('|')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}

				if (strTmp == "FOCUS")
				{
					Prop->styles |= stCHECKED;
				}
			}
		}
		if (mapData.Lookup("COLOR", strData))
		{
			strData.Trim();
			pos = strData.Find(",");
			strTmp = strData.Left(pos);
			strData = strData.Mid(pos + 1);
			Prop->paintColor = atoi(strTmp);
			Prop->textColor = atoi(strData);
		}
		if (mapData.Lookup("GROUP", strData))
		{
			Prop->keyNames = strData;
		}
		if (mapData.Lookup("DATE", strData))
		{
			Prop->size = (WORD)atoi(strData);
		}
		if (mapData.Lookup("LINEW", strData))
		{
			Prop->size = (WORD)atoi(strData);
		}
		if (mapData.Lookup("KEYMAP", strData))
		{
			for (kk = 0; kk < VKTBL; kk++)
			{
				if (!strData.CompareNoCase(vkTbl[kk].strName))
				{
					Prop->keyParam = kk;
					break;
				}
			}
		}
		if (mapData.Lookup("REF", strData))
		{
			Prop->keyNames = CString(strData);
		}
		if (mapData.Lookup("LINECOLOR", strData))
		{
			Prop->borderColor = atoi(strData);
			Prop->charType = bxLINE;
		}
		if (mapData.Lookup("TABORDER", strData))
		{
			Prop->index = atoi(strData);
		}
		//if (mapData.Lookup("GROUPN", strData))
		//{
		//}
		if (mapData.Lookup("IOORDER", strData))
		{
			Prop->iorder = atoi(strData);
		}
		break;
	}
	switch (Prop->kind)
	{
	case fmOBJECT:
		if (mapData.Lookup("LEN", strData))
		{
			Prop->size = (WORD)atoi(strData);
		}
		if (mapData.Lookup("IDX", strData))
		{
			Prop->iCtmIdx = (WORD)atoi(strData);
		}
		if (mapData.Lookup("COLOR", strData))
		{
			pos = strData.Find(",");
			strTmp = strData.Left(pos);
			strData = strData.Mid(pos + 1);

			Prop->paintColor = atoi(strTmp);
			Prop->textColor = atoi(strData);
		}
		if (mapData.Lookup("DOM", strData))
		{
			Prop->keyNames = strData;
		}
		if (mapData.Lookup("COUNT", strData))
		{
			Prop->keyValues = MAKELONG(LOWORD((BYTE)atoi(strData)), HIWORD(Prop->keyValues));
		}
		if (mapData.Lookup("DISPLAY", strData))
		{
			strData.Replace("\"", "");
			CString data = _T("");
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find(',')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}
				data += strTmp;
				data += _T('\0');
			}
			Prop->displays = data;
		}
		if (mapData.Lookup("CAPTION", strData))
		{
			strData.Replace("\"", "");
			CString data = _T("");
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find(',')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}
				strTmp.Trim();
				data += strTmp;
				data += _T('\0');
			}
			Prop->displays = data;
		}
		if (mapData.Lookup("DATA", strData))
		{
			strData.Replace("\"", "");
			CString data = _T("");
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find(',')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}
				strTmp.Trim();
				data += strTmp;
				data += _T('\0');
			}
			Prop->strings = data;
		}
		if (mapData.Lookup("INIT", strData))
		{
			strData.Replace("\"", "");
			Prop->strings = strData;
		}
		if (mapData.Lookup("URL", strData))
		{
			strData.Replace("\"", "");
			Prop->strings = strData;
		}
		if (mapData.Lookup("FLOAT", strData))
		{
			Prop->offsets = (BYTE)atoi(strData);
		}
		if (mapData.Lookup("UNIT", strData))
		{
			strData.Replace("\"", "");
			CString data = _T("");
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find(',')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}
				data += strTmp;
				data += '\t';
			}
			Prop->strings = data;
		}
		if (mapData.Lookup("INDEX", strData))
		{
			Prop->checked = (BYTE)atoi(strData);
		}
		if (mapData.Lookup("OBJECT", strData))
		{
			Prop->keyNames = strData;
		}
		if (mapData.Lookup("TABINFO", strData))
		{
			strData.Replace("\"", "");
			CString data = _T("");
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find(',')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}
				strTmp.Trim();
				data += strTmp;
				data += _T('\0');
			}
			Prop->strings = data;
		}
		if (mapData.Lookup("MULTISEL", strData))
		{
			Prop->styles |= stCHECKED;
		}
		if (mapData.Lookup("CHARS", strData))
		{
			if (Prop->kind == fmMEMO)
			{
				Prop->keyValues = atoi(strData);
			}
		}
		if (mapData.Lookup("REFERENCE", strData))
		{
			Prop->referenceName = strData;
		}
		if (mapData.Lookup("DEFAULT", strData))
		{
			//Prop->displays = CString(token[jj].name, token[jj].len);
			//Prop->strings = CString(token[jj].name, token[jj].len);
		}
		if (mapData.Lookup("TABH", strData))
		{
			Prop->size = (BYTE)atoi(strData);
		}
		if (mapData.Lookup("SPIN", strData))
		{
			//Prop->styles |= stSPIN;
			//if (Prop->keyParam == 0)
			//	Prop->keyParam = 1;
			Prop->keyValues = atoi(strData);
		}
		if (mapData.Lookup("CODE", strData))
		{
			Prop->styles |= stCODE;
		}
		if (mapData.Lookup("SUM", strData))
		{
			Prop->styles |= stSUM;
		}
		if (mapData.Lookup("FCOLOR", strData))
		{
			pos = strData.Find(",");
			strTmp = strData.Left(pos);
			strData = strData.Mid(pos + 1);

			Prop->paintColor2 = atoi(strTmp);
			Prop->paintColor1 = atoi(strData);
		}
		if (mapData.Lookup("VCOUNT", strData))
		{
			Prop->keyValues = MAKELONG((WORD)Prop->keyValues, atoi(strData));
		}
		if (mapData.Lookup("CVAR", strData))
		{
			Prop->editFormat = strData;
		}
		if (mapData.Lookup("TABORDER", strData))
		{
			Prop->index = atoi(strData);
		}
		//int pageN = atoi(token[jj++].name), formCnt = atoi(token[jj].name);
		//for (int nLoop = 0; nLoop < formCnt; nLoop++)
		//{
		//	struct _tabPage pageR;
		//	pageR.tabN = fN;
		//	pageR.pageN = pageN;
		//	m_tabPageArr.SetAt(atoi(token[++jj].name), pageR);
		//}
		if (mapData.Lookup("IOORDER", strData))
		{
			Prop->iorder = atoi(strData);
		}
		if (mapData.Lookup("MATCHKEY", strData))
		{
			for (kk = 0; kk < VKTBL; kk++)
			{
				if (!strData.CompareNoCase(vkTbl[kk].strName))
				{
					Prop->auxValues = kk;
					//formR->onEdit = kk;
					break;
				}
			}
		}
		if (mapData.Lookup("SPINTYPE", strData))
		{
			Prop->keyParam = atoi(strData);
			if (Prop->kind == fmEDIT)
			{
				if (Prop->keyParam == 0) Prop->styles &= ~stSPIN;
				else Prop->styles |= stSPIN;
			}
		}
		if (mapData.Lookup("ENCRYPT", strData))
		{
			strData.Trim();
			Prop->keyNames = strData;
		}
		if (mapData.Lookup("LINECOLOR", strData))
		{
			Prop->borderColor = atoi(strData);
			Prop->ioType = ioINPUT;
		}
		if (mapData.Lookup("TYPE", strData))
		{
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find('|')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}

				if (strTmp == "LIST")
				{
					Prop->styles |= stLIST;
				}

				if (strTmp == "HASTOOL")
				{
					Prop->styles |= stCHECKED;
				}

				if (strTmp == "FOCUS")
				{
					Prop->styles |= stCHECKED;
				}
			}
		}
		if (pNode->strElement == _T("TYPE"))
		{
			while (!strData.IsEmpty())
			{
				if ((pos = strData.Find('|')) != -1)
				{
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
				}
				else
				{
					strTmp = strData;
					strData = _T("");
				}

				for (kk = 0; kk < EMODETBL; kk++)
				{
					if (!strData.CompareNoCase(emodeTbl[kk].strName))
					{
						((cellOb*)Prop->cellProperties->GetAt(repN))->ioType = emodeTbl[kk].ioType;
						break;
					}
				}
			}
			for (int ii = 0; ii < pCellPropNode->GetCount(); ii++)
			{
				pNode = pCellPropNode->GetAt(ii);
				strData = pNode->strValue;
				if (pNode->strElement == _T("COLINFO"))
				{
					repN++;
				}
				else if (pNode->strElement == _T("NAME"))
				{
					strData.Replace("\"", "");
					((cellOb*)Prop->cellProperties->GetAt(repN))->name = strData;
					((cellOb*)Prop->cellProperties->GetAt(repN))->lineN = (BYTE)multi;
					((cellOb*)Prop->cellProperties->GetAt(repN))->ioType = ioOUTPUT;
				}
				else if (pNode->strElement == _T("RTS"))
				{
					((cellOb*)Prop->cellProperties->GetAt(repN))->flashKeys = strData;
				}
				else if (pNode->strElement == _T("TYPE"))
				{
					while (!strData.IsEmpty())
					{
						if ((pos = strData.Find('|')) != -1)
						{
							strTmp = strData.Left(pos);
							strData = strData.Mid(pos + 1);
						}
						else
						{
							strTmp = strData;
							strData = _T("");
						}

						for (kk = 0; kk < IOTBL; kk++)
						{
							if (!strTmp.CompareNoCase(ioTbl[kk].strName))
							{
								((cellOb*)Prop->cellProperties->GetAt(repN))->charType = ioTbl[kk].charType;
								break;
							}
						}

						for (kk = 0; kk < ATTRTBL; kk++)
						{
							if (!strTmp.CompareNoCase(attrTbl[kk].strName))
							{
								((cellOb*)Prop->cellProperties->GetAt(repN))->attributes |= attrTbl[kk].attributes;
								if (attrTbl[kk].attributes == faWRAP)
									multi++;
								break;
							}
						}

						for (kk = 0; kk < ZEROSUPTBL; kk++)
						{
							if (!strTmp.CompareNoCase(zerosupTbl[kk].strName))
							{
								((cellOb*)Prop->cellProperties->GetAt(repN))->auxAttributes |= zerosupTbl[kk].auxAttributes;
								break;
							}
						}

						for (kk = 0; kk < CORGBTBL; kk++)
						{
							if (!strTmp.CompareNoCase(coRgbTbl[kk].strName))
							{
								((cellOb*)Prop->cellProperties->GetAt(repN))->auxAttributes |= coRgbTbl[kk].auxAttributes;
								break;
							}
						}

						for (kk = 0; kk < CONTRASTTBL; kk++)
						{
							if (!strTmp.CompareNoCase(contrastTbl[kk].strName))
							{
								((cellOb*)Prop->cellProperties->GetAt(repN))->auxAttributes |= contrastTbl[kk].auxAttributes;
								break;
							}
						}

						if (strTmp == "COLUMN_VISIBLE")
						{
							((cellOb*)Prop->cellProperties->GetAt(repN))->styles |= stVISIBLE;
						}

						else if (strTmp == "COLUMN_SORT")
						{
							((cellOb*)Prop->cellProperties->GetAt(repN))->styles |= stSORT;
						}
						else if (strTmp == "ICOMBO")
						{
							((cellOb*)Prop->cellProperties->GetAt(repN))->kind = ckCOMBO;
						}
						else if (strTmp == "ICHECK")
						{
							((cellOb*)Prop->cellProperties->GetAt(repN))->kind = ckCHECK;
						}
						else if (strTmp == "IGRAPH")
						{
							((cellOb*)Prop->cellProperties->GetAt(repN))->kind = ckCHART;
						}
						else if (strTmp == "SUPRESS")
						{
							((cellOb*)Prop->cellProperties->GetAt(repN))->styles |= stSUPRESS;
						}
						else if (strTmp == "IBUTTON")
						{
							((cellOb*)Prop->cellProperties->GetAt(repN))->kind = ckBUTTON;
						}
						else if (strTmp == "COLUMN_IMAGE")
						{
							((cellOb*)Prop->cellProperties->GetAt(repN))->styles |= stIMAGE;
						}
						else if (strTmp == "COLUMN_IMGTEXT")
						{
							((cellOb*)Prop->cellProperties->GetAt(repN))->styles |= stIMAGETEXT;
						}
						else if (strTmp == "COLUMN_SHOWFORM")
						{
							((cellOb*)Prop->cellProperties->GetAt(repN))->styles |= stEDIT;
						}
						else if (strTmp == "COLUMN_EXCHECKE")
						{
							((cellOb*)Prop->cellProperties->GetAt(repN))->styles |= stCHECKED;
						}
						else if (strTmp == "COLUMN_BOLD")
						{
							((cellOb*)Prop->cellProperties->GetAt(repN))->styles |= stBOLD;
						}
					}
				}
				if (pNode->strElement == "LEN")
				{
					((cellOb*)Prop->cellProperties->GetAt(repN))->size = (WORD)atoi(strData);
				}
				if (pNode->strElement == "COLOR")
				{
					strData.Trim();
					pos = strData.Find(",");
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);

					((cellOb*)Prop->cellProperties->GetAt(repN))->paintColor = atoi(strTmp);
					((cellOb*)Prop->cellProperties->GetAt(repN))->textColor = atoi(strData);
				}
				if (pNode->strElement == "DOM")
				{
					strData.Replace("\"", "");
					strData.Trim();
					((cellOb*)Prop->cellProperties->GetAt(repN))->dominoName = strData;
				}
				if (pNode->strElement == "HEAD")
				{
					strData.Replace("\"", "");
					((cellOb*)Prop->cellProperties->GetAt(repN))->headText = strData;
					strTmp = ((cellOb*)Prop->cellProperties->GetAt(repN))->headText;
				}
				if (pNode->strElement == "WIDTH")
				{
					((cellOb*)Prop->cellProperties->GetAt(repN))->width = (WORD)atoi(strData);
				}
				if (pNode->strElement == "HEADCOLOR")
				{
					strData.Trim();
					pos = strData.Find(",");
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);
					((cellOb*)Prop->cellProperties->GetAt(repN))->headPaintColor = atoi(strTmp);
					((cellOb*)Prop->cellProperties->GetAt(repN))->headTextColor = atoi(strData);
				}
				if (pNode->strElement == "COLUMN_VISIBLE")
				{
					((cellOb*)Prop->cellProperties->GetAt(repN))->width = (WORD)atoi(strData);
				}
				if (pNode->strElement == "FLOAT")
				{
					((cellOb*)Prop->cellProperties->GetAt(repN))->offsets = (BYTE)atoi(strData);
				}
				if (pNode->strElement == "COLALIGN")
				{
					strData.Trim();
					pos = strData.Find(",");
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);

					for (int ii = 0; ii < ALIGNTBL; ii++)
					{
						if (!strTmp.Compare(alignTbl[ii].strName))
						{
							((cellOb*)Prop->cellProperties->GetAt(repN))->alignmentHead = alignTbl[ii].alignment;
							break;
						}
					}
					for (int ii = 0; ii < ALIGNTBL; ii++)
					{
						if (!strData.Compare(alignTbl[ii].strName))
						{
							((cellOb*)Prop->cellProperties->GetAt(repN))->alignmentText = alignTbl[ii].alignment;
							break;
						}
					}
				}
				if (pNode->strElement == "REFERENCE")
				{
					strData.Replace("\"", "");
					strData.Trim();
					((cellOb*)Prop->cellProperties->GetAt(repN))->referenceName = strData;
				}
				if (pNode->strElement == "COUNT")
				{
					((cellOb*)Prop->cellProperties->GetAt(repN))->entryCombo = (BYTE)atoi(strData);
				}
				if (pNode->strElement == "DISPLAY")
				{
					while (!strData.IsEmpty())
					{
						if ((pos = strData.Find(',')) != -1)
						{
							strTmp = strData.Left(pos);
							strData = strData.Mid(pos + 1);
						}
						else
						{
							strTmp = strData;
							strData = _T("");
						}
						strTmp.Replace("\"", "");

						offs = 0;
						for (kk = 0; kk < ((cellOb*)Prop->cellProperties->GetAt(repN))->entryCombo; kk++)
						{
							((cellOb*)Prop->cellProperties->GetAt(repN))->displays += strTmp;
							((cellOb*)Prop->cellProperties->GetAt(repN))->displays += '\0';
						}
					}
				}

				if (pNode->strElement == "DATA")
				{
					while (!strData.IsEmpty())
					{
						if ((pos = strData.Find(',')) != -1)
						{
							strTmp = strData.Left(pos);
							strData = strData.Mid(pos + 1);
						}
						else
						{
							strTmp = strData;
							strData = _T("");
						}
						strTmp.Replace("\"", "");

						offs = 0;
						for (kk = 0; kk < ((cellOb*)Prop->cellProperties->GetAt(repN))->entryCombo; kk++)
						{
							((cellOb*)Prop->cellProperties->GetAt(repN))->strings += strTmp;
							((cellOb*)Prop->cellProperties->GetAt(repN))->strings += '\0';
						}
					}
				}
				if (pNode->strElement == "CHECKSTR")
				{
					strData.Replace("\"", "");
					strData.Trim();
					((cellOb*)Prop->cellProperties->GetAt(repN))->strings = strData;
				}
				if (pNode->strElement == "EDITFORMAT")
				{
					strData.Replace("\"", "");
					strData.Trim();
					((cellOb*)Prop->cellProperties->GetAt(repN))->editFormat = strData;
					((cellOb*)Prop->cellProperties->GetAt(repN))->whatEdit = weIT;
				}
				if (pNode->strElement == "IFRGB")
				{
					strData.Trim();
					pos = strData.Find(",");
					strTmp = strData.Left(pos);
					strData = strData.Mid(pos + 1);

					strData = strData.Mid(pos + 1);
					WORD upRGB = atoi(strTmp);
					WORD dnRGB = atoi(strData);
					((cellOb*)Prop->cellProperties->GetAt(repN))->auxValues = MAKELONG(dnRGB, upRGB);
					((cellOb*)Prop->cellProperties->GetAt(repN))->whatEdit = weCORGB;
					((cellOb*)Prop->cellProperties->GetAt(repN))->attributes = faCORGB;
				}
				if (pNode->strElement == "INORMAL")
				{
					((cellOb*)Prop->cellProperties->GetAt(repN))->kind = ckNORMAL;
				}
				if (pNode->strElement == "MATCHKEY")
				{
					for (kk = 0; kk < VKTBL; kk++)
					{
						if (!strData.CompareNoCase(vkTbl[kk].strName))
						{
							((cellOb*)Prop->cellProperties->GetAt(repN))->keyParam = kk;
							break;
						}
					}
				}
				if (pNode->strElement == "BUTTONSTR")
				{
					strData.Replace("\"", "");
					strData.Trim();
					((cellOb*)Prop->cellProperties->GetAt(repN))->displays = strData;
				}
				if (pNode->strElement == "BUTTONIMG")
				{
					strData.Replace("\"", "");
					strData.Trim();
					((cellOb*)Prop->cellProperties->GetAt(repN))->images = strData;
				}
				if (pNode->strElement == "COLHELPTEXT")
				{
					strData.Replace("\"", "");
					strData.Trim();
					((cellOb*)Prop->cellProperties->GetAt(repN))->hintText = strData;
				}
			}
			if (mapData.Lookup("VALID", strData))
			{
				Prop->keyValues = MAKELONG(LOWORD(Prop->keyValues), (WORD)atoi(strData));
			}
			if (mapData.Lookup("CELLH", strData))
			{
				Prop->keyValues = MAKELONG(atoi(strData), HIWORD(Prop->keyValues));
				Prop->iAutoSize = atoi(strData);
			}
			if (mapData.Lookup("TYPE", strData))
			{
				while (!strData.IsEmpty())
				{
					if ((pos = strData.Find('|')) != -1)
					{
						strTmp = strData.Left(pos);
						strData = strData.Mid(pos + 1);
					}
					else
					{
						strTmp = strData;
						strData = _T("");
					}
					if (strTmp == "LEAVE")
					{
						Prop->attributes |= faWRAP;
					}
					else if (strTmp == "CELLEDIT")
					{
						Prop->attributes |= faENUM;
					}
					else if (strTmp == "PAGE")
					{
						Prop->auxAttributes |= gaPAGE;
					}
					else if (strTmp == "HEADER")
					{
						Prop->auxAttributes |= gaHEADER;
					}
					else if (strTmp == "TABLE")
					{
						Prop->auxAttributes |= gaTABLE;
					}
					else if (strTmp == "THUMB")
					{
						Prop->auxAttributes |= gaTHUMB;
					}
					else if (strTmp == "CXDELETE")
					{
						Prop->attributes |= faXDELETE;
					}
					else if (strTmp == "AUTOSIZE")
					{
						Prop->auxAttributes |= gaAUTOSIZE;
						Prop->keyValues = 0;
					}
					else if (strTmp == "CUD")
					{
						Prop->auxAttributes |= gaCUD;
					}
					else if (strTmp == "ISFOCUS")
					{
						Prop->styles |= stCHECKED;
					}
					else if (strTmp == "MERGE")
					{
						Prop->auxAttributes |= gaMERGE;
					}
					else if (strTmp == "KEYFOCUS")
					{
						Prop->auxAttributes |= gaKEY;
					}
					else if (strTmp == "SELALWAYS")
					{
						Prop->auxAttributes |= gaALWAYS;
					}
					else if (strTmp == "GRFIX")
					{
						Prop->auxAttributes |= gaFIX;
					}
					else if (strTmp == "GRFLEXEXPAND")
					{
						Prop->auxAttributes |= gaEXPAND;
					}
					else if (strTmp == "GRXSIZE")
					{
						Prop->auxAttributes |= gaXSIZE;
					}
					else if (strTmp == "GRAUTOWIDTH")
					{
						Prop->auxAttributes |= gaAUTO;
					}
					else if (strTmp == "GRVALID")
					{
						Prop->auxAttributes |= gaVALID;
					}
					else if (strTmp == "GRFTEXT")
					{
						Prop->auxAttributes |= gaFTEXT;
					}
					else if (strTmp == "GRXXSL")
					{
						Prop->auxAttributes |= gaXXLS;
					}
					else if (strTmp == "ONLYHEAD3D")
					{
						Prop->auxAttributes |= ga3DHEAD;
					}
					else if (strTmp == "FADROP")
					{
						Prop->attributes |= faDROP;
					}
					else if (strTmp == "MULTI")
					{
						Prop->auxAttributes |= gaMULTI;
					}
					else if (strTmp == "FLEXIBLE")
					{
						Prop->auxAttributes |= gaFLEX;
					}
					else if (strTmp == "VSCROLL")
					{
						Prop->auxAttributes |= gaVSCR;
					}
					else if (strTmp == "HSCROLL")
					{
						Prop->auxAttributes |= gaHSCR;
					}
					else if (strTmp == "INS_TOP")
					{
						Prop->auxAttributes |= gaTOP;
					}
					else if (strTmp == "NOHEAD")
					{
						Prop->auxAttributes |= gaXHEAD;
					}
					else if (strTmp == "NO3D")
					{
						Prop->auxAttributes |= gaX3D;
					}
					else if (strTmp == "RROW")
					{
						Prop->auxAttributes |= gaROW;
					}
					else if (strTmp == "XVLINE")
					{
						Prop->auxAttributes |= gaXVLINE;
					}
					else if (strTmp == "XHLINE")
					{
						Prop->auxAttributes |= gaXHLINE;
					}
				}
			}
			if (mapData.Lookup("COLUMNS", strData))
			{
			}
			if (mapData.Lookup("MATRIX", strData))
			{
				strData.Trim();
				pos = strData.Find(",");
				strTmp = strData.Left(pos);
				strData = strData.Mid(pos + 1);
				Prop->size = (WORD)atoi(strTmp);
			}
			if (mapData.Lookup("FOCUSKIND", strData))
			{
				Prop->whatEdit = atoi(strData);
			}
			if (mapData.Lookup("LINECOLOR", strData))
			{
				Prop->borderColor = atoi(strData);
			}
			//if (mapData.Lookup("LINEW", strData))
			//{
			//}
			if (mapData.Lookup("FIX", strData))
			{
				Prop->fixColumn = atoi(strData);
			}
			if (mapData.Lookup("VARIANT", strData))
			{
				strData.Replace("\"", "");
				strData.Trim();
				Prop->strings = strData;
			}
			if (mapData.Lookup("ROWC", strData))
			{
				strData.Trim();
				pos = strData.Find(",");
				strTmp = strData.Left(pos);
				strData = strData.Mid(pos + 1);

				Prop->paintColor2 = atoi(strTmp);
				Prop->paintColor1 = atoi(strData);
			}
			if (mapData.Lookup("TABORDER", strData))
			{
				Prop->index = atoi(strData);
			}
			if (mapData.Lookup("IOORDER", strData))
			{
				Prop->iorder = atoi(strData);
			}
			if (mapData.Lookup("FOCUSCOLOR", strData))
			{
				Prop->auxValues = MAKELONG(LOWORD(atoi(strData)), HIWORD(Prop->auxValues));
			}
			if (mapData.Lookup("COLHINTTYPE", strData))
			{
				for (kk = 0; kk < HINTTBL; kk++)
				{
					if (!strData.CompareNoCase(hintTbl[kk].strName))
					{
						((cellOb*)Prop->cellProperties->GetAt(repN))->typeHint = hintTbl[kk].type;
						break;
					}
				}
			}
			if (mapData.Lookup("FOCUSCOLOR", strData))
			{
				Prop->auxValues = MAKELONG(LOWORD(atoi(strData)), HIWORD(Prop->auxValues));
			}
			//if (mapData.Lookup("GRSHARE", strData))
			//{
			//}
			if (mapData.Lookup("ROWCC", strData))
			{
				Prop->rowLines = atoi(strData);
			}
			if (mapData.Lookup("INIT", strData))
			{
				strData.Replace("\"", "");
				Prop->strings = strData;
			}
			break;
		}
	}
}

//
//	syntax
//
struct _lineSyntax {
	char    token[32];
	BYTE    flag;
} lineSyntax[]={
		{"ONSTART",		LTYPE_ONSTART},
		{"ONSEND",		LTYPE_ONSEND},
		{"ONRECEIVE",		LTYPE_ONRECEIVE},
		{"ONALERT",		LTYPE_ONALERT},
		{"ONSERVICE",		LTYPE_ONSERVICE},
		{"ONFILE",		LTYPE_ONFILE},
		{"ONSELECT",		LTYPE_ONSELECT},
		{"ONTIMER",		LTYPE_ONTIMER},
		{"ONFOCUS",		LTYPE_ONFOCUS},
		{"ONCLOSE",		LTYPE_ONCLOSE},
		{"ONDEVICE",		LTYPE_ONDEVICE},
		{"ONAPPROVE",		LTYPE_ONAPPROVE},
		{"ONKEY",		LTYPE_ONKEY},
		{"ONTRIGGER",		LTYPE_ONTRIGGER},
		{"ONDECLARE",		LTYPE_ONDECLARE},
		{"ONINDECLARE",		LTYPE_ONINDECLARE},
		{"SCRIPT",		LTYPE_SCPBEGIN},
		{"ENDSCRIPT",		LTYPE_SCPEND},
		{"ONEDIT",		LTYPE_ONEDIT},
		{"ONCLICK",		LTYPE_ONCLICK},
		{"ONDBLCLK",		LTYPE_ONDBLCLK},
		{"ONCHANGE",		LTYPE_ONCHANGE},
		{"ONCHARCHANGE",	LTYPE_ONCHAR},
		{"ONKEYCHANGE",		LTYPE_ONKEYCHANGE},
		{"PROCEDURE",		LTYPE_PROC},
		{"ENDPROC",		LTYPE_PROCEND},
		{"ONENDSUB",		LTYPE_ONENDSUB},
		{"ONENDFUNC",		LTYPE_ONENDFUNC},
		{"ONEND",		LTYPE_ONENDSUB},
		{"}",			LTYPE_DIVEND}
};
#define LINESYNTAX      sizeof(lineSyntax) / sizeof(struct _lineSyntax)

struct _ObjectSyntax {
	char    token[32];
	BYTE    flag;
} ObjectSyntax[] = {
		{"_ONCLICK",		OBJECT_ONCLICK},
		{"_ONDBLCLK",		OBJECT_ONDBLCLK},
		{"_ONCHANGE",		OBJECT_ONCHANGE},
		{"_ONCHARCHANGE",	OBJECT_ONCHAR},
		{"_ONKEYCHANGE",	OBJECT_ONKEYCHANGE},
};
#define OBJECTSYNTAX      sizeof(ObjectSyntax) / sizeof(struct _ObjectSyntax)


void CMapLoad::BuildXmlResource(CNode* pElementNode)
{
	CMap<CString, LPCSTR, CString, LPCSTR> mapData;
	CMap<CString, LPCSTR, CString, LPCSTR> mapObject;
	ElementNode* pNode;
	DWORD offset;
	CFile file;
	CString strTmp;
	int jj = 0;
	for (int ii = 0; ii < pElementNode->GetCount(); ii++)
	{
		pNode = pElementNode->GetAt(ii);
		for (jj = 0; jj < OBJECTSYNTAX; jj++)
		{
			if (pNode->strElement.Find(ObjectSyntax[jj].token) != -1)
				break;
		}

		if (jj == OBJECTSYNTAX)
			mapData.SetAt(pNode->strElement, pNode->strValue);
		else
			mapObject.SetAt(pNode->strElement, pNode->strValue);
	}

	CString strData;
	char	prefix[L_SGID + 1];
	CopyMemory(prefix, m_mapH->mapN, L_SGID);
	prefix[L_SGID] = '\0';
	
	if (mapData.Lookup("ONSTART", strData))
	{
		m_mapH->onStart = true;
		m_mapH->onStartN[0] = '\0';
		wccGetTempName(m_mapH->onStartN, prefix);
		saveScriptFile(CString(m_mapH->onStartN), strData);
	}
	if (mapData.Lookup("ONSEND", strData))
	{
		m_mapH->onSend = true;
		m_mapH->onSendN[0] = '\0';
		wccGetTempName(m_mapH->onSendN, prefix);
		saveScriptFile(CString(m_mapH->onSendN), strData);
	}
	if (mapData.Lookup("ONRECEIVE", strData))
	{
		m_mapH->onReceive = true;
		m_mapH->onReceiveN[0] = '\0';
		wccGetTempName(m_mapH->onReceiveN, prefix);
		saveScriptFile(CString(m_mapH->onReceiveN), strData);
	}
	if (mapData.Lookup("ONALERT", strData))
	{
		m_mapH->onAlert = true;
		m_mapH->onAlertN[0] = '\0';
		wccGetTempName(m_mapH->onAlertN, prefix);
		saveScriptFile(CString(m_mapH->onAlertN), strData);
	}
	if (mapData.Lookup("ONSERVICE", strData))
	{
		m_mapH->onService = true;
		m_mapH->onServiceN[0] = '\0';
		wccGetTempName(m_mapH->onServiceN, prefix);
		saveScriptFile(CString(m_mapH->onServiceN), strData);
	}
	if (mapData.Lookup("ONFILE", strData))
	{
		m_mapH->onFile = true;
		m_mapH->onFileN[0] = '\0';
		wccGetTempName(m_mapH->onFileN, prefix);
		saveScriptFile(CString(m_mapH->onFileN), strData);
	}
	if (mapData.Lookup("ONSELECT", strData))
	{
		m_mapH->onSelect = true;
		m_mapH->onSelectN[0] = '\0';
		wccGetTempName(m_mapH->onSelectN, prefix);
		saveScriptFile(CString(m_mapH->onSelectN), strData);
	}
	if (mapData.Lookup("ONTIMER", strData))
	{
		m_mapH->onTimer = true;
		m_mapH->onTimerN[0] = '\0';
		wccGetTempName(m_mapH->onTimerN, prefix);
		saveScriptFile(CString(m_mapH->onTimerN), strData);
	}
	if (mapData.Lookup("ONFOCUS", strData))
	{
		m_mapH->onFocus = true;
		m_mapH->onFocusN[0] = '\0';
		wccGetTempName(m_mapH->onFocusN, prefix);
		saveScriptFile(CString(m_mapH->onFocusN), strData);
	}
	if (mapData.Lookup("ONCLOSE", strData))
	{
		m_mapH->onClose = true;
		m_mapH->onCloseN[0] = '\0';
		wccGetTempName(m_mapH->onCloseN, prefix);
		saveScriptFile(CString(m_mapH->onCloseN), strData);
	}
	if (mapData.Lookup("ONDEVICE", strData))
	{
		m_mapH->onDevice = true;
		m_mapH->onDeviceN[0] = '\0';
		wccGetTempName(m_mapH->onDeviceN, prefix);
		saveScriptFile(CString(m_mapH->onDeviceN), strData);
	}
	if (mapData.Lookup("ONAPPROVE", strData))
	{
		m_mapH->onApprove = true;
		m_mapH->onApproveN[0] = '\0';
		wccGetTempName(m_mapH->onApproveN, prefix);
		saveScriptFile(CString(m_mapH->onApproveN), strData);
	}
	if (mapData.Lookup("ONKEY", strData))
	{
		m_mapH->onKey = true;
		m_mapH->onKeyN[0] = '\0';
		wccGetTempName(m_mapH->onKeyN, prefix);
		saveScriptFile(CString(m_mapH->onKeyN), strData);
	}
	if (mapData.Lookup("ONTRIGGER", strData))
	{
		m_mapH->onTrigger = true;
		m_mapH->onTriggerN[0] = '\0';
		wccGetTempName(m_mapH->onTriggerN, prefix);
		saveScriptFile(CString(m_mapH->onTriggerN), strData);
	}
	if (mapData.Lookup("ONDECLARE", strData))
	{
		m_mapH->onDeclaration = true;
		m_mapH->onDeclarationN[0] = '\0';
		wccGetTempName(m_mapH->onDeclarationN, prefix);
		saveScriptFile(CString(m_mapH->onDeclarationN), strData);
	}
	if (mapData.Lookup("ONINDECLARE", strData))
	{
		m_mapH->onInDeclaration = true;
		m_mapH->onInDeclarationN[0] = '\0';
		wccGetTempName(m_mapH->onInDeclarationN, prefix);
		saveScriptFile(CString(m_mapH->onInDeclarationN), strData);
	}

	if (!m_mapH->pythonMode) {
		POSITION p = mapData.GetStartPosition();
		while (p) {
			CString key, val;
			mapData.GetNextAssoc(p, key, val);
			if (val.Find("def ") >= 0 || val.Find("import ") >= 0) {
				m_mapH->pythonMode = true;
				break;
			}
		}
	}
	if (!m_mapH->pythonMode) {
		POSITION p = mapObject.GetStartPosition();
		while (p) {
			CString key, val;
			mapObject.GetNextAssoc(p, key, val);
			if (val.Find("def ") >= 0 || val.Find("import ") >= 0) {
				m_mapH->pythonMode = true;
				break;
			}
		}
	}

	POSITION pos;
	CString strValue;
	

	CformProp* Prop;
	char fileName[1024];
	for (int ii = 0; ii < m_mapH->forms.GetCount(); ii++)
	{
		Prop = ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getProp();

		CopyMemory(prefix, m_mapH->mapN, L_SGID);
		prefix[L_SGID] = '\0';

		wccGetTempName(fileName, prefix);
		Prop->scpN = fileName;

		pos = mapObject.GetStartPosition();
		while (pos != NULL)
		{
			mapObject.GetNextAssoc(pos, strData, strValue);
			if (strData.Find(Prop->name) > -1)
			{
				if (strData.Find("_ONCLICK") > -1)
				{
					Prop->onClick = 0;
					Prop->onClickL = strValue.GetLength();
					if (file.Open(Prop->scpN, CFile::modeRead | CFile::typeBinary))
					{
						offset = (DWORD)file.GetLength();
						if (offset > 0)
							Prop->onClick = offset;
						file.Close();
					}
					saveScriptFile(Prop->scpN, strValue);
				}
				else if (strData.Find("_ONDBLCLK") > -1)
				{
					Prop->onDblClk = 0;
					Prop->onDblClkL = strValue.GetLength();
					if (file.Open(Prop->scpN, CFile::modeRead | CFile::typeBinary))
					{
						offset = (DWORD)file.GetLength();
						if (offset > 0)
							Prop->onDblClk = offset;
						file.Close();
					}
					saveScriptFile(Prop->scpN, strValue);
				}
				else if (strData.Find("_ONCHANGE") > -1)
				{
					Prop->onChange = 0;
					Prop->onChangeL = strValue.GetLength();
					if (file.Open(Prop->scpN, CFile::modeRead | CFile::typeBinary))
					{
						offset = (DWORD)file.GetLength();
						if (offset > 0)
							Prop->onChange = offset;
						file.Close();
					}
					saveScriptFile(Prop->scpN, strValue);
				}
				else if (strData.Find("_ONCHARCHANGE") > -1)
				{
					Prop->onChar = 0;
					Prop->onCharL = strValue.GetLength();
					if (file.Open(Prop->scpN, CFile::modeRead | CFile::typeBinary))
					{
						offset = (DWORD)file.GetLength();
						if (offset > 0)
							Prop->onChar = offset;
						file.Close();
					}
					saveScriptFile(Prop->scpN, strValue);
				}
				else if (strData.Find("_ONKEYCHANGE") > -1)
				{
					Prop->onKey = 0;
					Prop->onKeyL = strValue.GetLength();
					if (file.Open(Prop->scpN, CFile::modeRead | CFile::typeBinary))
					{
						offset = (DWORD)file.GetLength();
						if (offset > 0)
							Prop->onKey = offset;
						file.Close();
					}
					saveScriptFile(Prop->scpN, strValue);
				}
			}
		}
	}
	//else
	//{
	//	if (m_rtype == LTYPE_ONEDIT)
	//	{
	//		int	tokCnt, tokN = 0;
	//		WORD	Type, upRGB, dnRGB;

	//		tokCnt = wccParseToken(token, line, &Type);

	//		tmps = CString(token[tokN].name, token[tokN].len);
	//		for (kk = 0; kk < IETBL; kk++)
	//		{
	//			if (!tmps.CompareNoCase(ieTbl[kk].strName))
	//			{
	//				CformProp* Prop = (CformProp*)((CObjectLoad*)m_mapH->forms.GetAt(m_formP))->getProp();
	//				if (!m_column)
	//					Prop->whatEdit = ieTbl[kk].whatEdit;
	//				else
	//				{
	//					((cellOb*)Prop->cellProperties->GetAt(m_colP))->whatEdit = ieTbl[kk].whatEdit;
	//				}
	//				break;
	//			}
	//		}

	//		if (kk >= IETBL)
	//		{
	//			return;
	//		}

	//		tokN++;
	//		switch (ieTbl[kk].whatEdit)
	//		{
	//		case weIT:
	//		{
	//			CformProp* Prop = (CformProp*)((CObjectLoad*)m_mapH->forms.GetAt(m_formP))->getProp();
	//			if (m_column)
	//			{
	//				((cellOb*)Prop->cellProperties->GetAt(m_colP))->editFormat = token[tokN].name;
	//				break;
	//			}
	//			Prop->editFormat = CString(token[tokN].name, token[tokN].len);
	//		}
	//		break;

	//		case weCORGB:
	//		{
	//			CformProp* Prop = (CformProp*)((CObjectLoad*)m_mapH->forms.GetAt(m_formP))->getProp();
	//			upRGB = atoi(token[tokN].name);	tokN++;
	//			dnRGB = atoi(token[tokN].name);
	//			if (m_column)
	//			{
	//				((cellOb*)Prop->cellProperties->GetAt(m_colP))->auxValues = MAKELONG(dnRGB, upRGB);
	//				break;
	//			}
	//			Prop->auxValues = MAKELONG(dnRGB, upRGB);
	//		}
	//		break;

	//		default:
	//			break;
	//		}
	//		for (ll = 0; ll < tokCnt; ll++)
	//			token[ll].Clear();

	//		m_rtype = 0;
	//		return;
	//	}

	//	tmps = CString(tmp);
	//	tmps.TrimRight();
	//	tmps += _T("\n");
	//	switch (m_rtype)
	//	{
	//	case LTYPE_ONSTART:
	//	case LTYPE_ONSEND:
	//	case LTYPE_ONRECEIVE:
	//	case LTYPE_ONALERT:
	//	case LTYPE_ONSERVICE:
	//	case LTYPE_ONFILE:
	//	case LTYPE_ONSELECT:
	//	case LTYPE_ONTIMER:
	//	case LTYPE_ONFOCUS:
	//	case LTYPE_ONCLOSE:
	//	case LTYPE_ONDEVICE:
	//	case LTYPE_ONAPPROVE:
	//	case LTYPE_ONKEY:
	//	case LTYPE_ONTRIGGER:
	//	case LTYPE_ONDECLARE:
	//	case LTYPE_ONINDECLARE:
	//	case LTYPE_ONCLICK:
	//	case LTYPE_ONDBLCLK:
	//	case LTYPE_ONCHANGE:
	//	case LTYPE_ONCHAR:
	//	case LTYPE_ONKEYCHANGE:
	//		m_script += tmps;
	//	default:
	//		break;
	//	}
	//}
}

void CMapLoad::BuildTab(int idx)
{
	CformProp* Prop = NULL;
	CformProp* groupProp = NULL;
	for (int ii = idx; ii < m_mapH->forms.GetCount(); ii++)
	{
		//struct	_tabPage pageR;
		int	value;
		//if (m_tabPageArr.Lookup(ii - idx, pageR))
		//{
		//	m_formItem->GetForm(ii)->tabN  = m_formItem->GetForm(pageR.tabN);
		//	m_formItem->GetForm(ii)->pageN = pageR.pageN;
		//}
		if (m_groupArr.Lookup(ii - idx, value))
		{
			Prop = (CformProp*)((CObjectLoad*)m_mapH->forms.GetAt(value))->getProp();
			groupProp = (CformProp*)((CObjectLoad*)m_mapH->forms.GetAt(ii - idx))->getProp();
			groupProp->group = Prop;
		}
	}
}

void CMapLoad::saveScriptFile(CString path, CString script)
{
	CFile	file;

	if (file.Open(path, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
	{
		file.SeekToEnd();
		file.Write(script, script.GetLength());
		file.Close();
	}
}

int CMapLoad::getFormIndex(char* symbol, int len, int count)
{
	CString	str = CString(symbol, len);
	for (int ii = 0; ii < count; ii++)
	{
		if (!str.CompareNoCase(((CObjectLoad*)m_mapH->forms.GetAt(ii))->getProp()->name))
			return ii;
	}

	return -1;
}

int CMapLoad::getColumnIndex(char* symbol, int len, int pos)
{
	int	count;
	CString	tmps;
	CformProp* Prop = (CformProp*)((CObjectLoad*)m_mapH->forms.GetAt(pos))->getProp();

	count = Prop->cellProperties->GetCount();

	tmps = CString(symbol, len);
	for (int ii = 0; ii < count; ii++)  // Column Symbol Check
	{
		if (!tmps.Compare(((cellOb*)Prop->cellProperties->GetAt(ii))->name))
		{
			return ii;
		}
	}
	return -1;
}

void CMapLoad::BuildXmlTRlayout(CNode* pElementNode)
{
	ElementNode* pNode;
	CString strTmp;
	for (int ii = 0; ii < pElementNode->GetCount(); ii++)
	{
		pNode = pElementNode->GetAt(ii);
		strTmp += pNode->strElement;
		strTmp += '\t';
		strTmp += pNode->strValue;
		strTmp += '^';
	}
	m_mapH->trinfo = strTmp;
}

void CMapLoad::BuildXmlLayoutEdit(CNode* pElementNode)
{
	CformProp* Prop = NULL;
	CformProp* groupProp = NULL;
	ElementNode* pNode;
	CString strTmp, strData, strName;

	for (int ii = 0; ii < m_mapH->forms.GetCount(); ii++)
	{
		Prop = (CformProp*)((CObjectLoad*)m_mapH->forms.GetAt(ii))->getProp();
		for (int ii = 0; ii < pElementNode->GetCount(); ii++)
		{
			pNode = pElementNode->GetAt(ii);
			if (pNode->strElement == Prop->name)
			{
				strTmp = pNode->strValue;
				while (!strTmp.IsEmpty())
				{
					strData = Parser(strTmp, "^");

					_Layout *playout;
					playout = new _Layout;
					ZeroMemory(playout, sizeof(_Layout));

					strName = Parser(strData, ",");
					playout->left = atoi(Parser(strData, ","));
					playout->top = atoi(Parser(strData, ","));
					playout->right = atoi(Parser(strData, ","));
					playout->bottom = atoi(Parser(strData, ","));
					playout->bfirst = true;
					playout->properties = 0x00;
					if (atoi(strData))
						playout->properties = stVISIBLE;

					Prop->layout->SetAt(strName, playout);
				}
			}
		}
	}

	CString sIndex;
	if (m_mapH->maplayout == nullptr)
		m_mapH->maplayout = new CMapStringToString;
	else
		m_mapH->maplayout->RemoveAll();

	int iIndex = -1;
	if( pElementNode->GetCount() > 0)
	{
		pNode = pElementNode->GetAt(0);
		strTmp = pNode->strValue;

		int iFind = -1;
		while ((iFind = strTmp.Find("^")) != -1)
		{
			strTmp.Delete(iFind);
			iIndex++;
		}
		strTmp = pNode->strValue;
		while (!strTmp.IsEmpty())
		{
			m_mapH->maplayout;
			strData = Parser(strTmp, "^");
			strData = Parser(strData, ",");
			if (strData == _T("Original"))
				sIndex.Format("-1");
			else
				sIndex.Format("%d", iIndex--);
			m_mapH->maplayout->SetAt(sIndex, strData);
		}
	}
}

void CMapLoad::BuildXmlRTS(CNode* pElementNode)
{
	ElementNode* pNode;
	CString strTmp;
	for (int ii = 0; ii < pElementNode->GetCount(); ii++)
	{
		pNode = pElementNode->GetAt(ii);
		strTmp += pNode->strValue;
		strTmp += '^';
	}
	m_mapH->rtsinfo = strTmp;
}

CString CMapLoad::Parser(CString &srcstr, CString substr)
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