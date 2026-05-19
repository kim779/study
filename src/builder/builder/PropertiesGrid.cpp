// PropertiesGrid.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisBuilder.h"
#include "PropertiesGrid.h"

#include "h/mapform.h"
#include "h/mainvar.h"
#include "awDlg/PropertyGridButtonProperty.h"
#include "awDlg/PropertyGridEditProperty.h"
#include "awTool/awObjectLoad.h"
#include "h/fmObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// data field information
struct _field {
	int	num;
	char*	name;
} fieldInfo[] = {
	// Form
	{0,	"name"},
	{1,	"size"},
	{2,	"charType"},
	{3,	"ioType"},
	{4,	"HkeyValues"},
	{5,	"LkeyValues"},
	{6,	"AkeyValues"},
	{7,	"left"},
	{8,	"top"},
	{9,	"width"},
	{10,	"height"},
	{11,	"alignment"},
	{12,	"borderStyle"},
	{13,	"index"},
	{14,	"fontStyle"},
	{15,	"fontPoint"},
	{16,	"fontName"},
	{17,	"textColor"},
	{18,	"paintColor"},
	{19,	"borderColor"},
	{20,	"attributes"},
	{21,	"auxAttributes"},
	{22,	"styles"},
	{23,	"checked"},
	{24,	"typeHint"},
	{25,	"hintText"},
	{26,	"whatEdit"},
	{27,	"auxValues"},
	{28,	"HcoRGB"},	//not use
	{29,	"LcoRGB"},	//not use
	{30,	"keyParam"},
	{31,	"keyValues"},
	{32,	"rowLines"},	//not use
	{33,	"vals3"},	//not use
	{34,	"keyNames"},
	{35,	"strings"},
	{36,	"initC"},	//not use
	{37,	"referenceName"},
	{38,	"offsets"},
	{39,	"table"},
	{40,	"rows"},
	{41,	"cols"},
	{42,	"images"},
	{43,	"paintColor1"},
	{44,	"paintColor2"},
	{45,	"tabdisplays"},
	{46,	"tabstrings"},
	{47,	"tabVisible"},
	{48,	"tableHead"},
	{49,	"editS"},
	{50,	"alignmentImage"},
	{51,	"fixSize"},
	{52,	"fixedColumn" },
	{53,	"rts" },
	{54,	"eLayoutLeft" },
	{55,	"eLayoutTop" },
	{56,	"eLayoutWidth" },
	{57,	"eLayoutHeight" },
	{58,	"eLayoutAvailable" },
	{59,	"objClass" },
	{60,	"alpha" },
	{61,	"margin" },
	{62,	"auxImages" },
	{63,	"auxColor" },
	{64,	"rtsShape" },
	// Cell (grid column, table row)
	{100,	"cellname"},
	{101,	"cellcharType"},
	{102,	"cellioType"},
	{103,	"cellkind"},
	{104,	"cellsize"},
	{105,	"cellwidth"},
	{106,	"cellheadText"},
	{107,	"cellalignmentHead"},
	{108,	"cellalignmentText"},
	{109,	"cellheadPaintColor"},
	{110,	"cellheadTextColor"},
	{111,	"cellpaintColor"},
	{112,	"celltextColor"},
	{113,	"celloffsets"},
	{114,	"cellstrings"},
	{115,	"celldominoName"},
	{116,	"cellreferenceName"},
	{117,	"cellstyles"},
	{118,	"cellattributes"},
	{119,	"allcellwidth"},	// not use
	{120,	"cellstr2"},		// not use
	{121,	"celldat"},			// not use
	{122,	"entryCombo"},
	{123,	"cellkeyParam"},
	{124,	"cellalignmentImage"},
	{125,	"cellimages"},
	{126,	"celldisplays"},
	{127,	"celltypeHint"},
	{128,	"cellhintText"},
	{129,	"cellauxAttributes"},
	{130,	"cellExFormula"},
	{131,	"cellrts"},
	// map 
	{200,	"mapcaption"},
	{201,	"mapheight"},
	{202,	"mapwidth"},
	{203,	"mapfname"},
	{204,	"mapfpoint"},
	{205,	"mapfstyle"},
	{206,	"mapvers"},
	{207,	"mapname"},
	{208,	"mapdesc"},
	{213,	"maphelpN"},
	{214,	"maprepeatV"},
	{216,	"mapimageK"},
	{217,	"mapimageV"},
	{219,	"maptype"},
	{220,	"repeatV"},
	{221,	"publicR"},
	{222,	"mapBkClr"},
	{223,	"controller"},
	{224,	"alpha"}
};

// CPropertiesGrid

IMPLEMENT_DYNAMIC(CPropertiesGrid, CMFCPropertyGridCtrl)

CPropertiesGrid::CPropertiesGrid()
{
	m_mapH = NULL;

	m_fN = -1;
	m_propMap.RemoveAll();
	m_propCtmObj.RemoveAll();
	m_fieldMap.RemoveAll();
	for (int ii = 0; ii < sizeof(fieldInfo)/sizeof(_field); ii++)
		m_fieldMap.SetAt(fieldInfo[ii].name, (void *)fieldInfo[ii].num);
}

CPropertiesGrid::~CPropertiesGrid()
{
	int	key;
	CPtrArray* arr;

	RemoveAll();
	for (POSITION pos = m_propMap.GetStartPosition(); pos;)
	{
		m_propMap.GetNextAssoc(pos, key, arr);
		for (int ii = 0; ii < arr->GetCount(); ii++)
			delete (Cproperty *)arr->GetAt(ii);
		arr->RemoveAll();
		delete arr;
	}
	m_propMap.RemoveAll();
	for (POSITION pos = m_propCtmObj.GetStartPosition(); pos;)
	{
		m_propCtmObj.GetNextAssoc(pos, key, arr);
		for (int ii = 0; ii < arr->GetCount(); ii++)
			delete (Cproperty *)arr->GetAt(ii);
		arr->RemoveAll();
		delete arr;
	}
	m_propCtmObj.RemoveAll();
	m_fieldMap.RemoveAll();
}


BEGIN_MESSAGE_MAP(CPropertiesGrid, CMFCPropertyGridCtrl)
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


// CPropertiesGrid 메시지 처리기입니다.

void CPropertiesGrid::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
{
	SendMessage(WM_USER, MAKEWPARAM(100, 0), (LPARAM)pProp);

	return CMFCPropertyGridCtrl::OnPropertyChanged(pProp);
}

LRESULT CPropertiesGrid::OnMessage(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))
	{
	case 1:		// checkGridMultiLine
		checkGridMultiLine(HIWORD(wp));
		break;

	case 2:		// load data
		SetProperties(m_kind, m_fN);
		break;

	case 100:	// OnPropertyChanged
		{
			CString	data;
			CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lp;
			Cproperty* prop = (Cproperty *)pProp->GetData();
			_variant_t var = pProp->GetValue();
			if (var.vt == VT_BSTR)
			{
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, -1);
				data = var.bstrVal;
				if (m_fN == -2)
				{
					for (int ii = 0; ii < m_mapH->forms.GetCount(); ii++)
					{
						if (((CObjectLoad*)m_mapH->forms.GetAt(ii))->getProp()->bCommon)
							setValue(prop, ii, -1, data);
					}
				}
				else
					setValue(prop, m_fN, -1, data);
			}
		}
		break;
	}
	return 0;
}

void CPropertiesGrid::Initialize(class mapForm* mapForm)
{
	m_mapH = mapForm;
}

bool CPropertiesGrid::SetIndex(int fN)
{
	if (m_fN == fN)
		return false;

	m_fN = fN;
	return true;
}

void CPropertiesGrid::SetPropInfo(int kind, CString info, bool bCtmObj)
{
	CString	tmps;
	Cproperty*	prop;
	CPtrArray*	arr;
	CMap	< int, int, CPtrArray*, CPtrArray* >* pPropMap;
	if (bCtmObj)
		pPropMap = &m_propCtmObj;
	else
		pPropMap = &m_propMap;

	if (!pPropMap->Lookup(kind, arr))
		arr = new CPtrArray();
	else
	{
		for (int ii = 0; ii < arr->GetSize(); ii++)
			delete (Cproperty *)arr->GetAt(ii);
	}
	arr->RemoveAll();

	for ( ; true; )
	{
		tmps = parse(info, _T("\t"));
		if (tmps.IsEmpty())
			break;

		prop = new Cproperty();
		prop->m_head = parse(tmps, _T("|"));
		prop->m_type = atoi(parse(tmps, _T("|")));
		prop->m_val1 = parse(tmps, _T("|"));
		prop->m_val2 = parse(tmps, _T("|"));
		prop->m_keys = parse(tmps, _T("|"));
		prop->m_desc = parse(tmps, _T("|"));

		arr->Add(prop);
	}
	pPropMap->SetAt(kind, arr);
}

void CPropertiesGrid::SetProperties(int kind, int fN, int cellN)
{
	CPtrArray*	arr;

	Clear();

	CMap	< int, int, CPtrArray*, CPtrArray* >* pPropMap;
	if (kind == fmOBJECT)
	{
		pPropMap = &m_propCtmObj;
		if (!pPropMap->Lookup(((CObjectLoad*)m_mapH->forms.GetAt(fN))->getProp()->iCtmIdx, arr))
			return;
	}
	else
	{
		pPropMap = &m_propMap;
		if (!pPropMap->Lookup(kind, arr))
			return;
	}

	

	Cproperty*	prop;
	CString		tmps;
	int		limit;
	DWORD		style;

	m_kind = kind;
	m_fN = fN;
	if (m_fN == -2)
	{
		int idx = AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_OBJSEL, 0);
		if (idx >= 0)
		{
			fN = idx;
		}
		else
		{
			for (int ii = 0; ii < m_mapH->forms.GetCount(); ii++)
			{
				if (((CObjectLoad*)m_mapH->forms.GetAt(ii))->getProp()->bCommon)
				{
					fN = ii;
					break;
				}
			}
		}
	}

	for (int ii = 0; ii < arr->GetSize(); ii++)
	{
		prop = (Cproperty *)arr->GetAt(ii);

		switch (prop->m_type)
		{
		case 1:	// edit
			CPropertyGridEditProperty*	pEdit;

			if (prop->m_val1.GetLength() >= 3)
				limit = atoi(prop->m_val1.Mid(1, 3));
			else
				limit = 0;

			style = 0;
			if (!prop->m_keys.CompareNoCase(_T("mapname")) || !prop->m_head.CompareNoCase(_T("MapName")))
				style |= ES_UPPERCASE;
			if (!prop->m_val1.IsEmpty() && prop->m_val1.GetAt(0) == _T('2'))
				style |= ES_NUMBER;

			pEdit = new CPropertyGridEditProperty(limit, style, prop->m_head, (_variant_t)getValue(prop, fN, cellN), prop->m_desc, (DWORD_PTR)prop);
			if (!prop->m_val1.IsEmpty() && prop->m_val1.GetAt(0) == _T('1'))
				pEdit->AllowEdit(FALSE);
			else
				pEdit->AllowEdit(TRUE);
			AddProperty(pEdit);
			break;

		case 2:	// edit + button
			CPropertyGridButtonProperty*	pButton;

			pButton = new CPropertyGridButtonProperty(false, atoi(prop->m_val2), prop->m_head, (_variant_t)getValue(prop, fN, cellN), prop->m_desc, (DWORD_PTR)prop);
			pButton->SetInfo(this, m_fN, m_mapH);
			if (!prop->m_val1.IsEmpty() && prop->m_val1.GetAt(0) == _T('1'))
				pButton->AllowEdit(FALSE);
			else
				pButton->AllowEdit(TRUE);
			AddProperty(pButton);
			break;
		
		case 3:	// combo
			CMFCPropertyGridProperty*	pCombo;

			tmps = getValue(prop, fN, cellN);
			pCombo = new CMFCPropertyGridProperty(prop->m_head, (_variant_t)_T(""), prop->m_desc, (DWORD_PTR)prop);
			fillCombo(pCombo, prop->m_val1, tmps);
			pCombo->AllowEdit(FALSE);
			AddProperty(pCombo);
			break;

		case 4:	// button
			CPropertyGridButtonProperty*	pfButton;

			pfButton = new CPropertyGridButtonProperty(true, atoi(prop->m_val2), prop->m_head, (_variant_t)getValue(prop, fN, cellN), prop->m_desc, (DWORD_PTR)prop);
			pfButton->SetInfo(this, m_fN, m_mapH);
			pfButton->AllowEdit(FALSE);
			AddProperty(pfButton);
			break;
		}
	}
}

void CPropertiesGrid::Refresh(int index)
{
	if (index < 0 || index >= GetPropertyCount())
		return;

	CString	tmps;
	Cproperty*	prop;
	CMFCPropertyGridProperty* pProp = GetProperty(index);
	prop = (Cproperty *)pProp->GetData();
	pProp->SetValue((_variant_t)getValue(prop, m_fN, index));
}

void CPropertiesGrid::Clear()
{
	RemoveAll();
	RedrawWindow();
}

int CPropertiesGrid::getKey(CString keys)
{
	LONG_PTR	key;

	if (m_fieldMap.Lookup(keys, (void *&)key))
		return (int)key;

	return -1;		
}

CString CPropertiesGrid::getValue(Cproperty* prop, int fN, int col)
{
	if (fN >= m_mapH->forms.GetCount())
		return _T("");

	CString tmps = _T("");
	int	id = getKey(prop->m_keys);

	CformProp* form = NULL;
	if (fN > -1)
	{
		form = ((CObjectLoad*)m_mapH->forms.GetAt(fN))->getProp();
		switch (id)
		{
		case 0:		// name
			tmps = form->name;
			break;
		case 1:		// size
			tmps.Format(_T("%d"), form->size);
			break;
		case 2:		// type
			tmps.Format(_T("%d"), form->charType);
			break;
		case 3:		// iok
			tmps.Format(_T("%d"), form->ioType);
			break;
		case 4:		// Hkeys
			tmps.Format(_T("%d"), HIWORD(form->keyValues));
			break;
		case 5:		// Lkeys
			if (form->kind == fmGRID || form->kind == fmGRIDEX)
			{
				tmps.Format(_T("%d"), form->iAutoSize);
			}
			else
			{
				tmps.Format(_T("%d"), LOWORD(form->keyValues));
			}
			break;
		case 6:		// Akeys
			//if (form->kind == fmTABLE && m_iAutoSize == 0)
			//{
			//	//struct _repR *repR = (struct _repR *)form->auxR;
			//	if (form->cellProperties != NULL)
			//		form->keyValues = (form->rect.bottom - form->rect.top) / form->size;
			//}
			//tmps.Format(_T("%d"), form->keyValues);
			if (form->kind == fmTABLE)
			{
				if (form->auxAttributes & gaAUTOSIZE)
				{
					form->keyValues = MAKELONG(0, HIWORD(form->keyValues));
				}
				else
				{
					form->keyValues = MAKELONG(form->iAutoSize, HIWORD(form->keyValues));
					form->rect.bottom = form->rect.top + (form->iAutoSize * form->cellProperties->GetCount()) + form->cellProperties->GetCount() + 1;
				}
				tmps.Format(_T("%d"), form->iAutoSize);
			}
			else
			{
				tmps.Format(_T("%d"), LOWORD(form->keyValues));
			}
			break;
		case 7:		// left
			tmps.Format(_T("%d"), form->rect.left);
			//if (form->tabN)
			//	tmps.Format(_T("%d"), form->rect.left - form->tabN->left);
			break;
		case 8:		// top
			tmps.Format(_T("%d"), form->rect.top);
			//if (form->tabN)
			//	tmps.Format(_T("%d"), form->top - form->tabN->top);
			break;
		case 9:		// width
			tmps.Format(_T("%d"), form->rect.right - form->rect.left);
			break;
		case 10:	// height
			tmps.Format(_T("%d"), form->rect.bottom - form->rect.top);
			break;
		case 11:	// align
			tmps.Format(_T("%d"), form->alignment);
			break;
		case 12:	// borders
			tmps.Format(_T("%d"), form->borderStyle);
			break;
		case 13:	// index
			tmps.Format(_T("%d"), form->index);
			break;
		case 14:	// fstyle
			tmps.Format(_T("%d"), form->fontStyle);
			break;
		case 15:	// fpoint
			tmps.Format(_T("%d"), form->fontPoint);
			if (atoi(tmps) == 0 && form->fontStyle == fsNONE)
				tmps.Format(_T("%d"), m_mapH->fPoint);
			break;
		case 16:	// fname
			tmps = form->fontName;
			if (tmps.IsEmpty() && form->fontStyle == fsNONE)
				tmps = CString(m_mapH->fName);
			break;
		case 17:	// tRGB
			tmps.Format(_T("%d"), form->textColor);
			break;
		case 18:	// pRGB
			tmps.Format(_T("%d"), form->paintColor);
			break;
		case 19:	// bRGB
			tmps.Format(_T("%d"), form->borderColor);
			break;
		case 20:	// attr
			if (hex2Int(prop->m_val2) & form->attributes)
				tmps = _T("1");
			else
				tmps = _T("0");
			break;
		case 21:	// attr2
			if (form->kind == fmEDIT)
			{
				if (form->attributes & faDATE)
					tmps.Format(_T("%d"), (getTypeOfDateFormat(form->auxAttributes)));
				else
					tmps.Format(_T("%d"), getTypeOfDateFormat(0xffffffff));
				break;
			}
			if (hex2Int(prop->m_val2) & form->auxAttributes)
				tmps = _T("1");
			else
				tmps = _T("0");
			break;
		case 22:	// properties
		{
			CString str = prop->m_val2;
			int	nCnt = 1;

			tmps = "0";
			while (!str.IsEmpty())
			{
				CString str2 = parse(str, _T(";"));
				if (hex2Int(str2) & form->styles)
					tmps.Format(_T("%d"), nCnt);
				nCnt++;
			}
		}
			break;
		case 23:	// checked
			tmps.Format(_T("%d"), form->checked);
			break;
		case 24:	// isHint
			tmps.Format(_T("%d"), form->typeHint);
			break;
		case 25:	// onHint
			tmps = form->hintText;
			break;
		case 26:	// whatEdit
			tmps.Format(_T("%d"), form->whatEdit);
			break;
		case 27:	// auxValues
			tmps.Format("%ld", form->auxValues);
			break;
		case 28:	// HcoRGB
			tmps.Format(_T("%d"), HIWORD(form->auxValues));
			break;
		case 29:	// LcoRGB
			tmps.Format(_T("%d"), LOWORD(form->auxValues));
			break;
		case 30:	// vals0
			tmps.Format(_T("%d"), LOWORD(form->keyParam));
			break;
		case 31:
			tmps.Format(_T("%d"), form->keyValues);
			break;
		case 32:
			tmps.Format(_T("%d"), form->rowLines);
			break;
		case 34:	// auxs
			tmps = form->keyNames;
			break;
		case 35:	// str
			tmps = form->strings;
			break;
		case 37:	// refs
			if (form->attributes & faREF)
				tmps = form->referenceName;
			break;
		case 38:	// offs
			tmps.Format(_T("%d"), min(form->size, form->offsets));
			break;
		case 39:	// table
			if (form->auxAttributes & gaTABLE)
				tmps = _T("1");
			else
				tmps = _T("0");
			break;
		case 40:	// rows
			if (form->kind == fmTABLE)
			{
				if (form->auxAttributes & gaAUTOSIZE)
				{
					form->keyValues = 0;
				}
				else
				{
					form->keyValues = MAKELONG(form->iAutoSize, HIWORD(form->keyValues));
					form->rect.bottom = form->rect.top + (form->iAutoSize * form->cellProperties->GetCount()) + form->cellProperties->GetCount() + 1;
				}
				tmps.Format(_T("%d"), form->cellProperties->GetCount());
			}
			else
			{
				tmps.Format(_T("%d"), form->size);
			}
			break;
		case 41:	// cols
			if (form->cellProperties != NULL) tmps.Format(_T("%d"), form->cellProperties->GetCount());
			else tmps.Format(_T("0"));
			//{
			//	struct _repR* repR = (struct _repR *)form->auxR;
			//	if (repR)
			//		tmps.Format(_T("%d"), repR->cols);
			//}
			break;
		case 42:	// str2
			tmps = form->images;
			break;
		case 43:	// stRGB
			tmps.Format(_T("%d"), form->paintColor1);
			break;
		case 44:	// spRGB
			tmps.Format(_T("%d"), form->paintColor2);
			break;
		case 45:	// tabHead
		{
			if (form->keyValues)
			{
				if (form->checked >= (int)form->keyValues)
					break;

				//CString strData("");
				char*	sDsp = (char *)form->displays.operator LPCTSTR();
				for (int ii = 0; ii < (int)form->keyValues; ii++)
				{
					if (form->checked == ii)
					{
						tmps = sDsp;
						break;
					}
					sDsp += strlen(sDsp);
					sDsp++;
				}
			}
		}
			break;
		case 46:	// tabData
		{
			if (form->checked >= (int)form->keyValues)
				break;

			CString strData(""), strTmp("");
			char*	sData = (char *)form->strings.operator LPCTSTR();
			for (int ii = 0; ii < (int)form->keyValues; ii++)
			{
				if (form->checked == ii)
				{
					tmps.Format("%c", sData[2]);
					break;
				}
				sData += strlen(sData);
				sData++;
			}
		}
			break;
		case 47:	// tabVisible
		{
			if (form->keyValues)
			{
				if (form->checked >= (int)form->keyValues)
					break;

				//CString strData("");
				char*	sData = (char *)form->strings.operator LPCTSTR();
				for (int ii = 0; ii < (int)form->keyValues; ii++)
				{
					if (form->checked == ii)
					{
						tmps.Format("%c", sData[0]);
						break;
					}
					sData += strlen(sData);
					sData++;
				}
			}
		}
			break;
		case 48:	// tableHead
		{
			if (form->auxAttributes & gaXHEAD)
				tmps = _T('2');
			else if (form->auxAttributes & gaMULTI)
				tmps = _T('0');
			else
				tmps = _T('1');
		}
			break;
		case 49:	// editS
			tmps = form->editFormat;
			break;
		case 50:	// alignImg
			tmps.Format(_T("%d"), form->alignmentImage);
			break;
		case 51:	// fixSize
			if (form->bFixedSize)
				tmps = _T('1');
			else
				tmps = _T('0');
			break;
		case 52:	// fixedColumn
			tmps.Format(_T("%d"), form->fixColumn);
			break;
		case 53:	// rts
			tmps.Format(_T("%s"), form->flashKeys);
			break;
		case 54:	// eLayoutLeft
			{
				struct _Layout* layout;
				CString sName = m_mapH->LayoutName;
				form->layout->Lookup(sName, (void*&)layout);
				tmps.Format(_T("%d"), layout->left);
			}
			break;
		case 55:	// eLayoutTop
			{
				struct _Layout* layout;
				CString sName = m_mapH->LayoutName;
				form->layout->Lookup(sName, (void*&)layout);
				tmps.Format(_T("%d"), layout->top);
			}
			break;
		case 56:	// eLayoutWidth
			{
				struct _Layout* layout;
				CString sName = m_mapH->LayoutName;
				form->layout->Lookup(sName, (void*&)layout);
				tmps.Format(_T("%d"), (layout->right - layout->left));
			}
			break;
		case 57:	// eLayoutHeight
			{
				struct _Layout* layout;
				CString sName = m_mapH->LayoutName;
				form->layout->Lookup(sName, (void*&)layout);
				tmps.Format(_T("%d"), (layout->bottom - layout->top));
			}
			break;
		case 58:	// eLayoutAvailable
			{
				struct _Layout* layout;
				CString sName = m_mapH->LayoutName;
				form->layout->Lookup(sName, (void*&)layout);
				tmps = _T("0");
				if (layout->properties & stVISIBLE)
					tmps = _T("1");
			}
			break;
		case 59:	//objClass
			tmps.Format(_T("%s"), form->objClass);
			break;
		case 60:	//alpha
			tmps.Format(_T("%d"), form->alpha);
			break;
		case 61:	//margin
			tmps.Format(_T("%s"), form->margin);
			break;
		case 62:	//auxImages
			tmps.Format(_T("%s"), form->auxImages);
			break;
		case 63:	//auxColor
			tmps.Format(_T("%d"), form->auxColor);
			break;
		case 64:	//rtsShape
			tmps.Format(_T("%s"), form->auxImages);
			break;
		case 119:	// allcellwidth
		{
			//if (form->cellProperties == NULL) break;
			if (form->size > 0)
			{
				cellOb* cellProp = (cellOb*)form->cellProperties->GetAt(0);
				tmps.Format(_T("%d"), cellProp->width);
			}
			else
				tmps = _T("80");
		}
			break;
		}

		if (id >= 100 && id < 200)
		{
			if (form->cellProperties != NULL && !form->cellProperties->IsEmpty() && (form->CellSelected >= 0 || col >= 0))
			{
				if (col < 0)
					col = form->CellSelected;

				cellOb* cellProp = (cellOb*)form->cellProperties->GetAt(col);
				switch (id)
				{
				case 100:	// cellname
					tmps = cellProp->name;
					break;
				case 101:	// celltype
					tmps.Format(_T("%d"), cellProp->charType);
					break;
				case 102:	// celliok
					if (cellProp->ioType == ioOUTPUT && cellProp->kind == ckCHECK)
						cellProp->ioType = ioBOTH;
					if ((cellProp->ioType == ioOUTPUT || cellProp->ioType == ioBOTH || cellProp->ioType == ioINPUT) && cellProp->kind == ckBUTTON)
						cellProp->ioType = ioFREE;
					tmps.Format(_T("%d"), cellProp->ioType);
					break;
				case 103:	// cellkind
					if (cellProp->ioType == ioOUTPUT && cellProp->kind == ckCHECK)
						cellProp->kind = ckNORMAL;
					if ((cellProp->ioType == ioOUTPUT || cellProp->ioType == ioBOTH || cellProp->ioType == ioINPUT) && cellProp->kind == ckBUTTON)
						cellProp->kind = ckNORMAL;
					tmps.Format(_T("%d"), cellProp->kind);
					break;
				case 104:	// cellsize
					tmps.Format(_T("%d"), cellProp->size);
					break;
				case 105:	// cellwidth
					tmps.Format(_T("%d"), cellProp->width);
					break;
				case 106:	// cellhead
					tmps = cellProp->headText;
					break;
				case 107:	// cellalignH
					tmps.Format(_T("%d"), cellProp->alignmentHead);
					break;
				case 108:	// cellalignC
					tmps.Format(_T("%d"), cellProp->alignmentText);
					break;
				case 109:	// cellhpRGB
					tmps.Format(_T("%d"), cellProp->headPaintColor);
					break;
				case 110:	// cellhtRGB
					tmps.Format(_T("%d"), cellProp->headTextColor);
					break;
				case 111:	// cellpRGB
					tmps.Format(_T("%d"), cellProp->paintColor);
					break;
				case 112:	// celltRGB
					tmps.Format(_T("%d"), cellProp->textColor);
					break;
				case 113:	// cellvals
					tmps.Format(_T("%d"), cellProp->offsets);
					break;
				case 114:	// cellstr
					tmps = cellProp->strings;
					break;
				case 115:	// cellauxs
					tmps = cellProp->dominoName;
					break;
				case 116:	// cellrefs
					tmps = cellProp->referenceName;
					break;
				case 117:	// cellproperties
				{
					CString str = prop->m_val2;
					int	nCnt = 1;

					tmps = _T("0");
					while (!str.IsEmpty())
					{
						CString str2 = parse(str, _T(";"));
						if (hex2Int(str2) & cellProp->styles)
							tmps.Format(_T("%d"), nCnt);
						nCnt++;
					}
				}
					break;
				case 118:	// cellattr
					if (hex2Int(prop->m_val2) & cellProp->attributes)
						tmps = _T("1");
					else
						tmps = _T("0");
					break;
					//case 120:	// cellstr2
					//	tmps = cellProp->strings;
					//	break;
					//case 121:	// celldat
					//	tmps = cellProp->displays;
					//	break;
				case 122:	// cellkeys
					tmps.Format(_T("%d"), cellProp->entryCombo);
					break;
				case 123:	// cellvals2
					tmps.Format(_T("%ld"), cellProp->keyParam);
					break;
				case 124:	// cellalignI
					tmps.Format(_T("%d"), cellProp->alignmentImage);
					break;
				case 125:	// cellstr3
					tmps = cellProp->images;
					break;
				case 126:	// celltxt
					tmps = cellProp->displays;
					break;
				case 127:	// cellisHint
					tmps.Format(_T("%d"), cellProp->typeHint);
					break;
				case 128:	// cellonHint
					tmps = cellProp->hintText;
					break;
				case 129:	// cellattr2
					if ((cellProp->ioType == ioBOTH || cellProp->ioType == ioINPUT) && cellProp->kind == ckNORMAL)	// edit
					{
						if (cellProp->attributes & faDATE)
							tmps.Format(_T("%d"), getTypeOfDateFormat(cellProp->auxAttributes));
						else
							tmps.Format(_T("%d"), getTypeOfDateFormat(0xffffffff));
						break;
					}
					tmps.Format(_T("%d"), getTypeOfDateFormat(0xffffffff));
					break;
				case 130:	// formula
					tmps = cellProp->formula;
					break;
				case 131:	// rts
					tmps = cellProp->flashKeys;
					break;
				}
			}
		}
	}
	if (id >= 200)
	{
		switch (id) // form
		{
		case 200:	// mapcaption
			tmps = m_mapH->caption;
			break;
		case 201:	// mapheight
			tmps.Format(_T("%d"), m_mapH->height);
			break;
		case 202:	// mapwidth
			tmps.Format(_T("%d"), m_mapH->width);
			break;
		case 203:	// mapfname
			tmps = m_mapH->fName;
			break;
		case 204:	// mapfpoint
			tmps.Format(_T("%d"), m_mapH->fPoint);
			break;
		case 205:	// mapfstyle
			tmps.Format(_T("%d"), m_mapH->fStyle);
			break;
		case 206:	// mapvers
			tmps.Format(_T("%d"), m_mapH->vers);
			break;
		case 207:	// mapname
			tmps = m_mapH->mapN;
			break;
		case 208:	// mapdesc
			if (strlen(m_mapH->mapdesc) == 0)
			{
				tmps = m_mapH->caption;
				CopyMemory(m_mapH->mapdesc, m_mapH->caption, min(strlen(m_mapH->caption), sizeof(m_mapH->mapdesc)-1));
			}
			else
				tmps = m_mapH->mapdesc;
			break;
		case 213:	// maphelpN
			tmps = m_mapH->helpN;
			break;
		case 214:	// maprepeatV
			tmps.Format(_T("%d"), m_mapH->repeatV);
			break;
		case 216:	// mapimageK
			tmps.Format(_T("%d"), m_mapH->imageK);
			break;
		case 217:	// mapimageV
			tmps = m_mapH->imageV;
			break;
		case 219:	// maptype
			tmps.Format(_T("%d"), m_mapH->type);
			break;
		case 220:	// repeatV
			tmps.Format(_T("%d"), m_mapH->repeatV);
			break;
		case 221:	// publicR
			tmps = m_mapH->publicR;
			break;
		case 222:	// mapBkClr
			tmps.Format(_T("%d"), m_mapH->bkClr);
			break;
		case 223:	// controller
			tmps.Format(_T("%s"), m_mapH->controller);
			break;
		case 224:	// alpha
			tmps.Format(_T("%d"), m_mapH->alpha);
			break;
		}
	}
	
	return tmps;
}

void CPropertiesGrid::setValue(Cproperty* prop, int fN, int col, CString data)
{
	if (fN >= m_mapH->forms.GetCount())
		return;

	int	index = fN, id = getKey(prop->m_keys);
	if (prop->m_type == 3)	// combo
		data = getComboData(prop->m_val1, data);

	m_mapH->modified = true;
	int iFormReload = 0;
	CformProp* form = NULL;
	CfmObject* Obj = NULL;
	CObjectLoad* ObjLoad = NULL;
	if (fN > -1)
	{
		ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(fN);
		form = ObjLoad->getProp();
		Obj = ObjLoad->getObject();
		switch (id)
		{
		case 0:		// name
			form->name = data;
			if (!prop->m_keys.CompareNoCase(_T("name")) || !prop->m_head.CompareNoCase(_T("name")))
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHCTRL, MAKEWORD(0, fN));
			break;
		case 1:		// size
			form->size = atoi(data);
			break;
		case 2:		// type
			form->charType = atoi(data);
			break;
		case 3:		// iok
			form->ioType = atoi(data);
			SortIoOrder(form);
			break;
		case 4:		// Hkeys
			form->keyValues = MAKELONG(LOWORD(form->keyValues), atoi(data));
			if (form->kind == fmGRID || form->kind == fmGRIDEX)
				iFormReload = GRIDSCROLL;
			break;
		case 5:		// Lkeys
			if (form->kind == fmGRID || form->kind == fmGRIDEX)
			{
				form->iAutoSize = atoi(data);
				if (form->auxAttributes & gaAUTOSIZE)
				{
				}
				else
				{
					form->keyValues = MAKELONG(form->iAutoSize, HIWORD(form->keyValues));
				}
			}
			else
			{
				form->keyValues = MAKELONG(atoi(data), HIWORD(form->keyValues));
			}
			break;
		case 6:		// keys

			if (form->kind == fmTAB)
			{
				if (atoi(data) <= 0)
					return;

				CString tmps;
				if (LOWORD(form->keyValues) <= (DWORD)atoi(data))
				{
					for (int ii = LOWORD(form->keyValues); ii < atoi(data); ii++)
					{
						tmps.Format(_T("Tab%d"), ii);
						form->displays += tmps;
						form->displays += _T('\0');

						tmps.Format(_T("1:%d"), LOWORD(form->keyValues));
						form->strings += tmps;
						form->strings += _T('\0');
					}
				}
				else
				{
					CString strDsp(""), strData("");
					char*	sDsp = (char *)form->displays.operator LPCTSTR();
					char*	sData = (char *)form->strings.operator LPCTSTR();
					for (int ii = 0; ii < atoi(data); ii++)
					{
						strDsp += sDsp;
						strDsp += '\0';
						sDsp += strlen(sDsp);
						sDsp++;

						strData += sData;
						strData += '\0';
						sData += strlen(sData);
						sData++;
					}
					form->displays.IsEmpty();
					form->displays = strDsp;
					form->strings.IsEmpty();
					form->strings = strData;
				}
				form->keyValues = MAKELONG(atoi(data), HIWORD(form->keyValues));
				GetParent()->PostMessage(WM_USER, 1, fN);	// Change Property
			}
			else if (form->kind == fmTABLE)
			{
				form->iAutoSize = atoi(data);
				if (form->auxAttributes & gaAUTOSIZE)
				{
					form->keyValues = 0;
				}
				else
				{
					form->keyValues = form->iAutoSize;
					form->rect.bottom = form->rect.top + (form->iAutoSize * form->cellProperties->GetCount()) + form->cellProperties->GetCount() + 1;
				}
			}
			else
			{
				form->keyValues = MAKELONG(atoi(data), HIWORD(form->keyValues));
			}
			break;
		case 7:		// left
			form->rect.right += atoi(data) - form->rect.left;
			form->rect.left = atoi(data);
			//Obj->SetRect(form->rect, form->rect);
			//if (form->tabN)
			//{
			//	form->rect.right += form->tabN->left;
			//	form->left += form->tabN->left;
			//}
			//index = -1;
			break;
		case 8:		// top
			form->rect.bottom += atoi(data) - form->rect.top;
			form->rect.top = atoi(data);
			//Obj->SetRect(form->rect, form->rect);
			//if (form->tabN)
			//{
			//	form->rect.bottom += form->tabN->top;
			//	form->rect.top += form->tabN->top;
			//}
			//index = -1;
			break;
		case 9:		// width
			form->rect.right = atoi(data) + form->rect.left;
			//Obj->SetRect(form->rect, form->rect);
			//index = -1;
			if (form->kind == fmGRID || form->kind == fmGRIDEX)
				iFormReload = GRIDSCROLL;
			else if (form->kind == fmFORM)
				iFormReload = FORMOBJECT;
			break;
		case 10:	// height
			form->rect.bottom = atoi(data) + form->rect.top;
			//Obj->SetRect(form->rect, form->rect);
			//index = -1;
			if (form->kind == fmGRID || form->kind == fmGRIDEX)
				iFormReload = GRIDSCROLL;
			else if (form->kind == fmFORM)
				iFormReload = FORMOBJECT;
			break;
		case 11:	// align
			form->alignment = atoi(data);
			break;
		case 12:	// borders
			form->borderStyle = atoi(data);
			break;
		case 13:	// index
			form->index = atoi(data);
			break;
		case 14:	// fstyle
			//if (form->fontStyle == fsNORMAL && atoi(data) != fsNORMAL)
			//{
			//	form->fontName = m_mapH->fName;
			//	form->fontPoint = m_mapH->fPoint;
			//	AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_REFRESHPRO, -1);
			//}
			//if (form->fontStyle != fsNORMAL && atoi(data) == fsNORMAL)
			//{
			//	form->fontName = _T("");
			//	form->fontPoint = 0;
			//	AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_REFRESHPRO, -1);
			//}
			form->fontStyle = atoi(data);
			//Obj->Reload();
			//Obj->SetFont(form->fontPoint, form->fontStyle, form->fontName, 0);
			break;
		case 15:	// fpoint
			form->fontPoint = atoi(data);
			//Obj->Reload();
			//Obj->SetFont(form->fontPoint, form->fontStyle, form->fontName, 0);
			break;
		case 16:	// fname
			//form->fontName = data;
			////Obj->SetFont(form->fontPoint, form->fontStyle, form->fontName, 0);
			break;
		case 17:	// tRGB
			form->textColor = atoi(data);
			//Obj->SetFgColor(form->textColor);
			break;
		case 18:	// pRGB
			form->paintColor = atoi(data);
			//Obj->SetBkColor(atoi(data));
			break;
		case 19:	// bRGB
			form->borderColor = atoi(data);
			break;

		case 20:	// attr
			if (data.GetAt(0) == '0')
				form->attributes &= ~hex2Int(prop->m_val2);
			else
			{
				form->attributes |= hex2Int(prop->m_val2);
				if (hex2Int(prop->m_val2) & faXFLOAT)
				{
					form->offsets = 0;
					GetParent()->PostMessage(WM_USER, 1, fN);	// Change Property
				}
			}
			break;
		case 21:	// auxAttributes
			if (form->kind == fmEDIT)
			{
				if (getTypeOfDateFormat(atoi(data)) == getTypeOfDateFormat(0xffffffff))	// None
				{
					form->attributes &= ~faDATE;
					form->auxAttributes &= ~getTypeOfDateFormat(form->auxAttributes);
				}
				else
				{
					form->attributes |= faDATE;
					form->auxAttributes &= ~getTypeOfDateFormat(form->auxAttributes);
					form->auxAttributes |= getTypeOfDateFormat(atoi(data));
				}
				break;
			}
			if (data.GetAt(0) == _T('0'))
			{
				form->auxAttributes &= ~hex2Int(prop->m_val2);
				if (hex2Int(prop->m_val2) & gaFLEX)
				{
					form->auxAttributes &= ~gaEXPAND;
					GetParent()->PostMessage(WM_USER, 1, fN);	// Change Property
				}
			}
			else
			{
				form->auxAttributes |= hex2Int(prop->m_val2);
				if ((hex2Int(prop->m_val2) & gaEXPAND) && !(form->auxAttributes & gaFLEX))
				{
					form->auxAttributes &= ~gaEXPAND;
					GetParent()->PostMessage(WM_USER, 1, fN);	// Change Property
				}
			}
			if (form->kind == fmTABLE)
			{
				if (form->auxAttributes & gaAUTOSIZE)
				{
					form->keyValues = 0;
				}
				else
				{
					form->keyValues = form->iAutoSize;
					form->rect.bottom = form->rect.top + (form->iAutoSize * form->cellProperties->GetCount()) + form->cellProperties->GetCount() + 1;
					GetParent()->PostMessage(WM_USER, 1, fN);	// Change Property
				}
			}
			else if (form->kind == fmGRID || form->kind == fmGRIDEX)
			{
				if (form->auxAttributes & gaAUTOSIZE)
				{
					form->keyValues = MAKELONG(0, HIWORD(form->keyValues));
					iFormReload = GRIDSCROLL;
				}
				else
				{
					form->keyValues = MAKELONG(form->iAutoSize, HIWORD(form->keyValues));
				}
			}
			checkGridMultiLine(fN);
			break;
		case 22:	// properties
		{
			CString	tmps, str;
			int	nCnt = 1;
			str = prop->m_val2;
			tmps = data;
			tmps = parse(tmps, _T("-"));
			while (!str.IsEmpty())
			{
				CString str2 = parse(str, _T(";"));
				if (nCnt == atoi(tmps))
					form->styles |= hex2Int(str2);
				else
					form->styles &= ~hex2Int(str2);
				nCnt++;
			}
		}
			break;

		case 23:	// checked
			form->checked = atoi(data);
			if (form->kind == fmTAB)
				GetParent()->PostMessage(WM_USER, 1, fN);	// Change Property
			break;
		case 24:	// typeHint
			form->typeHint = atoi(data);
			break;
		case 25:	// onHint
			form->hintText = data;
			break;
		case 26:	// whatEdit
			form->whatEdit = atoi(data);
			break;
		case 27:	// grid focus color  // onEdit(old ver)
			form->auxValues = atoi(data);
			break;

		case 28:	// HcoRGB
			form->auxValues = MAKELONG(atoi(data), HIWORD(form->auxValues));
			break;
		case 29:	// LcoRGB
			form->auxValues = MAKELONG(LOWORD(form->auxValues), atoi(data));
			break;

		case 30:	// vals0
			form->keyParam = atoi(data);
			if (form->kind == fmEDIT)
			{
				if (form->keyParam == 0) form->styles &= ~stSPIN;
				else form->styles |= stSPIN;
			}
			break;
		case 31:
			form->keyValues = MAKELONG(LOWORD(atoi(data)), HIWORD(form->keyValues));
			break;
		case 32:
			form->rowLines = (atoi(data) == 0) ? 1 : atoi(data);
			break;
		case 34:	// auxS
			form->keyNames = data;
			break;
		case 35:	// str
			if (form->kind == fmFORM && !data.IsEmpty())
			{
				if (!data.CompareNoCase(m_mapH->mapN))
				{
					CString	tmps;

					GetParent()->PostMessage(WM_USER, 1, fN);	// Change Property
					tmps = _T("Form Name must different from Map Name!!");
					AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_STATUSERR, (LPARAM)tmps.operator LPCTSTR());
					break;
				}
				else
				{
					form->strings = data;
					iFormReload = FORMOBJECT;
				}
			}
			else
			{
				form->strings = data;
			}
			break;
		case 37:	// referenceName
			if (data.IsEmpty())
			{
				form->attributes &= ~faREF;
				form->referenceName = _T("");
			}
			else
			{
				form->attributes |= faREF;
				form->referenceName = data;
			}
			break;
		case 38:	// offsets
			if (!atoi(data))
			{
				form->attributes &= ~faFLOAT;
				form->offsets = 0;
			}
			else
			{
				form->offsets = min(form->size, atoi(data));
				if (form->offsets != 0)
				{
					form->attributes |= faFLOAT;
				}
				if (form->size < atoi(data))
				{
					CString	tmps;

					GetParent()->PostMessage(WM_USER, 1, fN);	// Change Property
					tmps = _T("Data Length less then float digit!");
					AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_STATUSERR, (LPARAM)tmps.operator LPCTSTR());
				}
			}
			break;
		case 39:	// table
			if (data.GetAt(0) == _T('0'))
			{
				form->auxAttributes &= ~gaTABLE;
			}
			else
			{
				form->auxAttributes |= gaTABLE;
				form->attributes &= ~faENUM;
				form->auxAttributes &= ~gaHEADER;
			}
			break;
		case 40:	// rows
		{
			if (atoi(data) < 0)
				data = _T("0");

			if (form->kind == fmTABLE)
			{
				if (form->cellProperties->GetCount() < atoi(data))
				{
					cellOb* lstCellProp = NULL;
					for (int ii = form->cellProperties->GetCount(); ii < atoi(data); ii++)
					{
						cellOb* cellProp = new cellOb();
						cellProp->paintColor = basINPUT;
						cellProp->textColor = 232;
						if (form->cellProperties->GetCount() > 0)
						{
							lstCellProp = (cellOb*)form->cellProperties->GetAt(form->cellProperties->GetCount() - 1);
							cellProp->headPaintColor = lstCellProp->headPaintColor;
							cellProp->headTextColor = lstCellProp->headTextColor;
							cellProp->width = lstCellProp->width;
						}
						else
						{
							cellProp->headPaintColor = basPRGB;
							cellProp->headTextColor = 232;
							cellProp->width = 80;
						}
						cellProp->size = 0;
						cellProp->charType = ctNUMERIC;
						cellProp->name = getInitSymbol(fN, _T("Row"));
						cellProp->alignmentText = atRIGHT;
						cellProp->styles |= stVISIBLE;
						form->cellProperties->Add(cellProp);
					}
				}
				else
				{
					int cnt = form->cellProperties->GetCount() - atoi(data);
					cellOb* cellProp = NULL;
					for (int ii = 0; ii < cnt; ii++)
					{
						cellProp = (cellOb*)form->cellProperties->GetAt(form->cellProperties->GetCount() - 1);
						form->cellProperties->RemoveAt(form->cellProperties->GetCount() - 1);
						delete cellProp;
					}
				}

				if (form->auxAttributes & gaAUTOSIZE)
				{
					form->keyValues = 0;
				}
				else
				{
					int Height = (form->iAutoSize * form->cellProperties->GetCount()) + form->cellProperties->GetCount() + 1;
					form->keyValues = form->iAutoSize;
					form->rect.bottom = form->rect.top + Height;
				}
			}
			else
			{
				form->size = atoi(data);
				if (form->kind == fmGRID || form->kind == fmGRIDEX)
					iFormReload = GRIDSCROLL;
			}
		}
			break;
		case 41:	// cols
		{
			if (atoi(data) < 0)
				data = _T("0");

			if (form->kind == fmGRID || form->kind == fmGRIDEX)
			{
				if (form->cellProperties->GetCount() < atoi(data))
				{
					cellOb* lstCellProp = NULL;
					for (int ii = form->cellProperties->GetCount(); ii < atoi(data); ii++)
					{
						cellOb* cellProp = new cellOb();
						cellProp->paintColor = basINPUT;
						cellProp->textColor = 232;
						if (form->cellProperties->GetCount() > 0)
						{
							lstCellProp = (cellOb*)form->cellProperties->GetAt(form->cellProperties->GetCount() - 1);
							cellProp->headPaintColor = lstCellProp->headPaintColor;
							cellProp->headTextColor = lstCellProp->headTextColor;
							cellProp->width = lstCellProp->width;
						}
						else
						{
							cellProp->headPaintColor = basPRGB;
							cellProp->headTextColor = 232;
							cellProp->width = 80;
						}

						cellProp->size = 0;
						cellProp->charType = ctNUMERIC;
						cellProp->ioType = ioOUTPUT;
						cellProp->kind = ckNORMAL;
						cellProp->name = getInitSymbol(fN, _T("Col"));
						cellProp->alignmentText = atRIGHT;
						cellProp->styles |= stVISIBLE;
						form->cellProperties->Add(cellProp);
					}
				}
				else
				{
					int cnt = form->cellProperties->GetCount() - atoi(data);
					cellOb* cellProp = NULL;
					for (int ii = 0; ii < cnt; ii++)
					{
						cellProp = (cellOb*)form->cellProperties->GetAt(form->cellProperties->GetCount() - 1);
						form->cellProperties->RemoveAt(form->cellProperties->GetCount() - 1);
						delete cellProp;
					}
				}
				iFormReload = GRIDSCROLL;
			}
			checkGridMultiLine(fN);
		}
			break;
		case 42:	// str2
			form->images = data;
			break;
		case 43:	// stRGB
			form->paintColor1 = atoi(data);
			break;
		case 44:	// spRGB
			form->paintColor2 = atoi(data);
			break;

		case 45:	// tabHead
		{
			if (form->checked >= (int)form->keyValues)
				break;

			CString strDsp("");
			char*	sDsp = (char *)form->displays.operator LPCTSTR();
			for (int ii = 0; ii < (int)form->keyValues; ii++)
			{
				if (form->checked == ii) strDsp += data;
				else strDsp += sDsp;
				strDsp += '\0';
				sDsp += strlen(sDsp);
				sDsp++;
			}
			form->displays.IsEmpty();
			form->displays = strDsp;
		}
			break;

		case 46:	// tabData
		{
			if (form->checked >= (int)form->keyValues)
				break;

			CString strData(""), strTmp("");
			char*	sData = (char *)form->strings.operator LPCTSTR();
			for (int ii = 0; ii < (int)form->keyValues; ii++)
			{
				if (form->checked == ii)
				{
					strTmp.Format("%c:%d", sData[0], atoi(data));
					strData += strTmp;
				}
				else strData += sData;
				strData += '\0';
				sData += strlen(sData);
				sData++;
			}
			form->strings.IsEmpty();
			form->strings = strData;
		}
			break;
		case 47:	// tabVisible
		{
			if (form->checked >= (int)form->keyValues)
				break;

			CString strData(""), strTmp("");
			char*	sData = (char *)form->strings.operator LPCTSTR();
			for (int ii = 0; ii < (int)form->keyValues; ii++)
			{
				if (form->checked == ii)
				{
					strTmp.Format("%s:%s", data, sData + 2);
					strData += strTmp;
				}
				else strData += sData;
				strData += '\0';
				sData += strlen(sData);
				sData++;
			}
			form->strings.IsEmpty();
			form->strings = strData;
		}
			break;
		case 48:	// tableHead
		{
			form->auxAttributes &= ~(gaMULTI | gaXHEAD);
			form->ioType = ioFREE;
			switch (atoi(data))
			{
			case 0:
				form->auxAttributes |= gaMULTI;
				form->ioType = ioOUTPUT;
				break;
			case 1:	break;
			case 2:
				form->auxAttributes |= gaXHEAD;
				form->ioType = ioOUTPUT;
				break;
			}
			SortIoOrder(form);
		}
			break;
		case 49:	// editS
			form->editFormat = data;
			break;
		case 50:	// alignImg
			form->alignmentImage = atoi(data);
			break;
		case 51:	// fixSize
			if (data.GetAt(0) == _T('0'))
				form->bFixedSize = false;
			else
				form->bFixedSize = true;
			iFormReload = FORMOBJECT;
			break;
		case 52:	// fixedColumn
			form->fixColumn = atoi(data);
			break;
		case 53:	// rts
			form->flashKeys = data;
			break;
		case 54:	// eLayoutLeft
			{
				struct _Layout* layout;
				CString sName = m_mapH->LayoutName;
				form->layout->Lookup(sName, (void*&)layout);
				layout->left = atoi(data);
				form->layout->SetAt(sName, (void*&)layout);

				form->rect.right += atoi(data) - form->rect.left;
				form->rect.left = atoi(data);
				index = -1;
			}
			break;
		case 55:	// eLayoutTop
			{
				struct _Layout* layout;
				CString sName = m_mapH->LayoutName;
				form->layout->Lookup(sName, (void*&)layout);
				layout->top = atoi(data);
				form->layout->SetAt(sName, (void*&)layout);

				form->rect.bottom += atoi(data) - form->rect.top;
				form->rect.top = atoi(data);
				index = -1;
			}
			break;
		case 56:	// eLayoutWidth
			{
				struct _Layout* layout;
				CString sName = m_mapH->LayoutName;
				form->layout->Lookup(sName, (void*&)layout);
				layout->right = layout->left + atoi(data);
				form->layout->SetAt(sName, (void*&)layout);

				form->rect.right = atoi(data) + form->rect.left;
				index = -1;
			}
			break;
		case 57:	// eLayoutHeight
			{
				struct _Layout* layout;
				CString sName = m_mapH->LayoutName;
				form->layout->Lookup(sName, (void*&)layout);
				layout->bottom = layout->top + atoi(data);
				form->layout->SetAt(sName, (void*&)layout);

				form->rect.bottom = atoi(data) + form->rect.top;
				index = -1;
			}
			break;
		case 58:	// eLayoutAvailable
			{
				struct _Layout* layout;
				CString sName = m_mapH->LayoutName;
				form->layout->Lookup(sName, (void*&)layout);
				layout->properties = 0;
				if (atoi(data))
					layout->properties = stVISIBLE;
				form->layout->SetAt(sName, (void*&)layout);
			}
			break;
		case 59:	//objClass
			form->objClass = data;
			break;
		case 60:	//alpha
			form->alpha = atoi(data);
			break;
		case 61:	//margin
			form->margin = data;
			break;
		case 62:	//auxImages
			form->auxImages = data;
			break;
		case 63:	//auxColor
			form->auxColor = atoi(data);
			break;
		case 64:	//rtsShape
			form->auxImages = data;
			break;
		case 119:	// allcellwidth
		{
			for (int ii = 0; ii < form->cellProperties->GetCount(); ii++)
				((cellOb*)form->cellProperties->GetAt(ii))->width = atoi(data);;
		}
			break;
		}


		//struct _repR* repR   = (fN >= 0 ? (struct _repR *)form->auxR : NULL);
		if (form != NULL && (form->CellSelected >= 0 || col >= 0))
		{
			if (col < 0)
				col = form->CellSelected;
			if (col == -1)
				return;

			cellOb* cellProp = (cellOb*)form->cellProperties->GetAt(col);
			switch (id)
			{
			case 100:	// cellname
				cellProp->name = data;
				break;
			case 101:	// celltype
				cellProp->charType = atoi(data);
				if (cellProp->charType != ctNUMERIC)
					cellProp->attributes &= ~(faCOMMA | faRIGHT);

				break;
			case 102:	// celliok
				if (atoi(data) == ioOUTPUT && cellProp->kind == ckCHECK)
				{
					cellProp->ioType = ioBOTH;
					//	LoadData(m_index, m_InputPnt.x, m_InputPnt.y);
					//	UpdateCombo();
				}
				else if ((atoi(data) == ioOUTPUT || atoi(data) == ioBOTH || atoi(data) == ioINPUT) && cellProp->kind == ckBUTTON)
				{
					cellProp->ioType = ioFREE;
					//	LoadData(m_index, m_InputPnt.x, m_InputPnt.y);
					//	UpdateCombo();
				}
				else
					cellProp->ioType = atoi(data);

				if (!((cellProp->ioType == ioBOTH || cellProp->ioType == ioINPUT) && cellProp->kind == ckNORMAL))	// No edit
				{
					cellProp->attributes &= ~faDATE;
					cellProp->auxAttributes &= ~getTypeOfDateFormat(cellProp->auxAttributes);
				}
				break;
			case 103:	// cellkind
				if (cellProp->ioType == ioOUTPUT && atoi(data) == ckCHECK)
				{
					cellProp->kind = ckNORMAL;
					//	LoadData(m_index, m_InputPnt.x, m_InputPnt.y);
					//	UpdateCombo();
				}
				else if ((cellProp->ioType == ioOUTPUT || cellProp->ioType == ioBOTH || cellProp->ioType == ioINPUT) && atoi(data) == ckBUTTON)
				{
					cellProp->kind = ckNORMAL;
					//	LoadData(m_index, m_InputPnt.x, m_InputPnt.y);
					//	UpdateCombo();
				}
				else
					cellProp->kind = atoi(data);
				if (!((cellProp->ioType == ioBOTH || cellProp->ioType == ioINPUT) && cellProp->kind == ckNORMAL))	// No edit
				{
					cellProp->attributes &= ~faDATE;
					cellProp->auxAttributes &= ~getTypeOfDateFormat(cellProp->auxAttributes);
				}

				break;
			case 104:	// cellsize
				cellProp->size = atoi(data);
				break;
			case 105:	// cellwidth
				cellProp->width = atoi(data);
				break;
			case 106:	// cellhead
				cellProp->headText = data;
				break;
			case 107:	// cellalignH
				cellProp->alignmentHead = atoi(data);
				break;
			case 108:	// cellalignC
				cellProp->alignmentText = atoi(data);
				break;
			case 109:	// cellhpRGB
				cellProp->headPaintColor = atoi(data);
				break;
			case 110:	// cellhtRGB
				cellProp->headTextColor = atoi(data);
				break;
			case 111:	// cellpRGB
				cellProp->paintColor = atoi(data);
				break;
			case 112:	// celltRGB
				cellProp->textColor = atoi(data);
				break;
			case 113:	// cellvals
				cellProp->offsets = atoi(data);
				if (atoi(data) == 0)
				{
					cellProp->attributes &= ~faRIGHT;
				}
				else
				{
					cellProp->attributes |= faRIGHT;
				}
				break;
			case 114:	// cellstr
				cellProp->strings = data;
				break;
			case 115:	// cellauxs
				cellProp->dominoName = data;
				break;
			case 116:	// cellrefs
				cellProp->referenceName = data;
				break;
			case 117:	// cellproperties
			{
				CString	str;
				int	nCnt = 1;

				str = prop->m_val2;
				data = parse(data, _T("-"));
				while (!str.IsEmpty())
				{
					CString str2 = parse(str, _T(";"));
					if (nCnt == atoi(data))
						cellProp->styles |= hex2Int(str2);
					else
						cellProp->styles &= ~hex2Int(str2);
					nCnt++;
				}
			}
				break;
			case 118:	// cellattr
				if (data.GetAt(0) == _T('0'))
					cellProp->attributes &= ~hex2Int(prop->m_val2);
				else
					cellProp->attributes |= hex2Int(prop->m_val2);
				break;
				//case 120:	// cellstr2
				//	cellProp->strings = data;
				//	break;
				//case 121:	// celldat
				//	cellProp->displays = data;
				//	break;
			case 122:	// cellkeys
				cellProp->entryCombo = atoi(data);
				break;
			case 123:	// cellvals2
				cellProp->keyParam = atoi(data);
				break;
			case 124:	// cellalignI
				cellProp->alignmentImage = atoi(data);
				break;
			case 125:	// cellstr3
				cellProp->images = data;
				break;
			case 126:	// celltxt
				cellProp->displays = data;
				break;
			case 127:	// cellisHint
				cellProp->typeHint = atoi(data);
				break;
			case 128:	// cellonHint
				cellProp->hintText = data;
				break;
			case 129:	// cellattr2
				if ((cellProp->ioType == ioBOTH || cellProp->ioType == ioINPUT) && cellProp->kind == ckNORMAL)	// edit
				{
					if (getTypeOfDateFormat(atoi(data)) == getTypeOfDateFormat(0xffffffff))
					{
						cellProp->attributes &= ~faDATE;
						cellProp->auxAttributes &= ~getTypeOfDateFormat(cellProp->auxAttributes);
					}
					else
					{
						cellProp->attributes |= faDATE;
						cellProp->auxAttributes &= ~getTypeOfDateFormat(cellProp->auxAttributes);
						cellProp->auxAttributes |= getTypeOfDateFormat(atoi(data));
					}
					break;
				}

				cellProp->attributes &= ~faDATE;
				cellProp->auxAttributes &= ~getTypeOfDateFormat(cellProp->auxAttributes);
				//	LoadData(m_index, m_InputPnt.x, m_InputPnt.y);
				//	UpdateCombo();
				break;
			case 130:	// formula
				cellProp->formula = data;
				break;
			case 131:	// rts
				cellProp->flashKeys = data;
				break;
			}
		}
	}
	switch (id) // form
	{
	case 200:	// mapcaption
		strcpy_s(m_mapH->caption, sizeof(m_mapH->caption), data);
		if (m_mapH->mapK == MK_NORM)
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETCAPTION, 0);
		break;
	case 201:	// mapheight
		m_mapH->height = atoi(data);
		index = -1;
		break;
	case 202:	// mapwidth
		m_mapH->width = atoi(data);
		index = -1;
		break;
	case 203:	// mapfname
		strcpy_s(m_mapH->fName, sizeof(m_mapH->fName), data);
		break;
	case 204:	// mapfpoint
		m_mapH->fPoint = atoi(data);
		break;
	case 205:	// mapfstyle
		m_mapH->fStyle = atoi(data);
		break;
	case 206:	// mapvers
		m_mapH->vers = atoi(data);
		break;
	case 207:	// mapname
		strcpy_s(m_mapH->mapN, sizeof(m_mapH->mapN), data);
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETMAPNAME);
		break;
	case 208:	// mapdesc
		strcpy_s(m_mapH->mapdesc, sizeof(m_mapH->mapdesc), data);
		if (m_mapH->mapK == MK_TEMPLATE)
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETCAPTION, 0);
		break;
	case 213:	// maphelpN
		strcpy_s(m_mapH->helpN, sizeof(m_mapH->helpN), data);
		break;
	case 214:	// maprepeatV
		m_mapH->repeatV = atoi(data);
		break;
	case 216:	// mapimageK
		m_mapH->imageK = atoi(data);
		iFormReload = FORMVIEW;
		break;
	case 217:	// mapimageV
		strcpy_s(m_mapH->imageV, sizeof(m_mapH->imageV), data);
		break;
	case 219:	// maptype
		m_mapH->type = atoi(data);
		break;
	case 220:	// repeatV
		m_mapH->repeatV = atoi(data);
		break;
	case 221:	// publicR
		strcpy_s(m_mapH->publicR, sizeof(m_mapH->publicR), data);
		break;
	case 222:	// mapBkClr
		m_mapH->bkClr = atoi(data);
		iFormReload = FORMVIEW;
		break;
	case 223:	// controller
		strcpy_s(m_mapH->controller, sizeof(m_mapH->controller), data);
		break;
	case 224:	// alpha
		m_mapH->alpha = atoi(data);
		break;
	}
	AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_REDRAWCTRL, iFormReload), index);
}

CString CPropertiesGrid::getInitSymbol(int fN, CString syms)
{
	CString tmps, slist;
	int	initN;
	CformProp* form = ((CObjectLoad*)m_mapH->forms.GetAt(fN))->getProp();

	if (form->cellProperties == NULL)
		return syms;

	cellOb* cellProp = NULL;
	for (int kk = 0; kk < form->cellProperties->GetCount(); kk++)
	{
		cellProp = (cellOb*)form->cellProperties->GetAt(kk);
		if (!cellProp->name.IsEmpty())
		{
			slist += cellProp->name;
			slist += _T('\t');
		}
	}

	initN = 0;
	slist.MakeUpper();
	for ( ;; )
	{
		tmps.Format(_T("%s%d"), syms, initN);
		tmps.MakeUpper();
		if (slist.Find(tmps + _T("\t")) == -1)
			break;
		else
			initN++;
	}
	
	tmps.Format(_T("%s%d"), syms, initN);
	return tmps;
}

int CPropertiesGrid::fillCombo(CMFCPropertyGridProperty* combo, CString str, CString data)
{
	if (str.GetLength() <= 0)
		return 0;

	int	pos;
	if (str.GetAt(0) == _T('$'))	// reserved items
	{
		CString field = "00$bool\t01$fontStyle\t02$alignment\t03$typeHint\t";

		pos = field.Find(str);
		if (pos == -1)
			return 0;

		switch (atoi(field.Mid(pos - 2, 2)))
		{
		case 0:		// $bool
			str = _T("0-False;1-True;");
			break;
		case 1:		// $fontStyle
			str = _T("0-None;1-Normal;2-Italic;3-Bold;4-Bold/Italic;");
			break;
		case 2:		// $alignment
			str = _T("0-Center;1-Left;2-Right;");
			break;
		case 3:		// $typeHint
			str = _T("0-None;1-String;2-Reference;");
			break;
		}		
	}

	CString	item, tmps = str;
	for ( ; !tmps.IsEmpty(); )
	{
		item = parse(tmps, _T(";"));
		parse(item, _T("-"));
		combo->AddOption(item);
	}

	str.Insert(0, _T(';'));
	data.Insert(0, _T(';'));
	data += _T('-');
	pos = str.Find(data);
	if (pos != -1)
	{
		pos += data.GetLength();
		str = str.Mid(pos);
		pos = str.Find(_T(';'));
		if (pos != -1)
		{
			str = str.Left(pos);
			combo->SetValue((_variant_t)str);
		}
	}
	return combo->GetOptionCount();
}

CString CPropertiesGrid::getComboData(CString items, CString data)
{
	if (data.GetLength() <= 0)
		return _T("");

	int	pos;
	if (items.GetAt(0) == _T('$'))	// reserved items
	{
		CString field = "00$bool\t01$fontStyle\t02$alignment\t03$typeHint\t";

		pos = field.Find(items);
		if (pos == -1)
			return 0;

		switch (atoi(field.Mid(pos - 2, 2)))
		{
		case 0:		// $bool
			items = _T("0-False;1-True;");
			break;
		case 1:		// $fontStyle
			items = _T("0-None;1-Normal;2-Italic;3-Bold;4-Bold/Italic;");
			break;
		case 2:		// $alignment
			items = _T("0-Center;1-Left;2-Right;");
			break;
		case 3:		// $typeHint
			items = _T("0-None;1-String;2-Reference;");
			break;
		}
	}

	items.Insert(0, _T(';'));
	data += _T(';');
	pos = items.Find(data);
	if (pos != -1)
	{
		data = items.Left(pos);
		pos = data.ReverseFind(_T(';'));
		if (pos != -1)
			data = data.Mid(pos+1);
		pos = data.Find(_T('-'));
		if (pos != -1)
		{
			data = data.Left(pos);
			data.Trim();
			return data;
		}
	}
	return _T("");
}

void CPropertiesGrid::checkGridMultiLine(int fN)
{
	CformProp* form = ((CObjectLoad*)m_mapH->forms.GetAt(fN))->getProp();

	if (form->kind == fmGRID && (form->auxAttributes & gaMULTI))
	{
		if (form->auxAttributes & gaMERGE)
		{
			form->auxAttributes &= ~gaMULTI;
			return;
		}

		CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(fN);
		CformProp*	Prop = (CformProp*)ObjLoad->getProp();
		if (Prop->cellProperties != NULL)
		{
			int	line, colN;
			line = colN = 0;
			cellOb* cellProp = NULL;
			for (int ii = 0; ii < Prop->cellProperties->GetCount(); ii++)
			{
				colN++;
				cellProp = (cellOb*)Prop->cellProperties->GetAt(ii);
				cellProp->lineN = (BYTE)line;
				if (cellProp->attributes & faWRAP)
					line++;
			}
		}
	}
}

int CPropertiesGrid::hex2Int(CString value)
{
	if (value.IsEmpty())
		return 0;

	struct _hexMap
	{
		TCHAR	chr;
		int	value;
	};
	const int hexMapL = 16;
	_hexMap hexMap[hexMapL] =
	{
		{_T('0'), 0},	{_T('1'), 1},
		{_T('2'), 2},	{_T('3'), 3},
		{_T('4'), 4},	{_T('5'), 5},
		{_T('6'), 6},	{_T('7'), 7},
		{_T('8'), 8},	{_T('9'), 9},
		{_T('A'), 10},	{_T('B'), 11},
		{_T('C'), 12},	{_T('D'), 13},
		{_T('E'), 14},	{_T('F'), 15}
	};
	TCHAR	*mstr, *s;
	int	result;
	bool	firsttime = true;

	value.MakeUpper();
	mstr = (TCHAR *)value.operator LPCSTR();
	s = mstr;
	
	result = 0;
	if (*s == _T('0') && *(s + 1) == _T('X'))
		s += 2;
	while (*s != _T('\0'))
	{
		bool	found = false;
		for (int ii = 0; ii < hexMapL; ii++)
		{
			if (*s == hexMap[ii].chr)
			{
				if (!firsttime)
					result <<= 4;
				result |= hexMap[ii].value;
				found = true;
				break;
			}
		}
		if (!found)
			break;
		s++;
		firsttime = false;
	}
//	free(mstr);
	return result;
}

CString CPropertiesGrid::parse(CString& src, CString sub)
{
	CString	tmps;
	int	pos = src.Find(sub);
	
	if (pos == -1)
	{
		tmps = src;
		src  = _T("");
		return tmps;
	}

	tmps = src.Left(pos);
	src  = src.Mid(pos + sub.GetLength());
	return tmps;
}

void CPropertiesGrid::SortIoOrder(CformProp* Prop)
{
	int iCnt = 0;
	CformProp* pProp = NULL;
	for (int ii = 0; ii < m_mapH->forms.GetCount(); ii++)
	{
		pProp = ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getProp();
		if(pProp->iorder != 0)
		{
			iCnt++;
		}
	}
	if(ioFREE != Prop->ioType && Prop->iorder == 0)
	{
		Prop->iorder = iCnt+1;
	}
	else if(ioFREE == Prop->ioType && Prop->iorder != 0)
	{
		CformProp* pProp = NULL;
		for (int ii = 0; ii < m_mapH->forms.GetCount(); ii++)
		{
			pProp = ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getProp();
			if(Prop == pProp) continue;
			if(Prop->iorder < pProp->iorder)
				pProp->iorder--;
		}
		Prop->iorder = 0;
	}
}