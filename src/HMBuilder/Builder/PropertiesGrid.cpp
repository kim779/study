// PropertiesGrid.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisMBuilder.h"
#include "PropertiesGrid.h"

#include "MainFrm.h"
#include "h/mapform.h"
#include "h/mainvar.h"
#include "amCom/formItem.h"
#include "amDlg/PropertyGridButtonProperty.h"
#include "amDlg/PropertyGridEditProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CUSTOMCOUNT		100

enum {
	eNAME,			// 1
	eSIZE,	
	eTYPE,	
	eIOK,	
	eHKEYS,	
	eLKEYS,	
	eAKEYS,	
	eLEFT,	
	eTOP,
	eWIDTH,			// 10
	eHEIGHT,
	eALIGN,
	eBORDERS,
	eINDEX,
	eFSTYLE,
	eFPOINT,
	eFNAME,
	eTRGB,
	ePRGB,
	eBRGB,			// 20
	eATTR,
	eATTR2,
	ePROPERTIES,
	eCHECKED,
	eISHINT,
	eONHINT,
	eISEDIT,
	eONEDIT,
	eHCORGB,
	eLCORGB,		// 30
	eVALS0,
	eVALS1,
	eVALS2,
	eVALS3,
	eAUXS,
	eSTR,
	eINITC,
	eREFS,
	eOFFS,
	eTABLE,			// 40
	eROWS,
	eCOLS,
	eSTR2,
	eSTRGB,
	eSPRGB,
	eTABHEAD,	
	eTABDATA,	
	eTABVISIBLE,	
	eTABLEHEAD,	
	eEDITS,			// 50
	eALIGNIMG,	
	eFIXSIZE,	
	eDESC,		
	eFIXEDROW,	
	eFIXEDCOL,	
	eHEADH,		
	eROWSOF,		
	eFORMOB_01,	
	eFORMOB_02,	
	eFORMOB_03,		// 60
	eFORMOB_04,	
	eFORMOB_05,	
	eFORMOB_06,	
	eFORMOB_07,	
	eFORMOB_08,	
	eFORMOB_09,	
	eFORMOB_10,
	eFORMOB_11,
	eFORMOB_12,
	eFORMOB_13,		// 70
	eFORMOB_14,
	eFORMOB_15,
	eFORMOB_16,
	eFORMOB_17,
	eFORMOB_18,
	eFORMOB_19,
	eFORMOB_20,
	eFORMOB_21,
	eFORMOB_22,
	eFORMOB_23,		// 80
	eFORMOB_24,
	eFORMOB_25,
	eFORMOB_26,
	eFORMOB_27,
	eFORMOB_28,
	eFORMOB_29,
	eFORMOB_30,
	eFORMOB_31,
	eFORMOB_32,
	eFORMOB_33,		// 90
	eFORMOB_34,
	eFORMOB_35,
	eRTS,	  
	eAlpha,	  
	eOptionImg,		
	eOptionPos,
	eSelectShape,		
	eLayoutLeft,		
	eLayoutTop,		
	eLayoutWidth,		// 100
	eLayoutHeight,
	eLayoutAvailable,
	eOptionImg2,		
	eOptionImg3,
	eOptionRGB,
	eBlinkShape,
	eSelectFontRGB,

	// margin
	eMargin0 = 130,		// 130
	eMargin1,
	eMargin2,
	eMargin3,
	
	// Cell (grid column, table row)
	eCELLNAME = 150,		
	eCELLTYPE,			
	eCELLIOK,		
	eCELLKIND,		
	eCELLSIZE,		
	eCELLWIDTH,		
	eCELLHEAD,		
	eCELLALIGNH,		
	eCELLALIGNC,			
	eCELLHPRGB,			
	eCELLHTRGB,			// 160
	eCELLPRGB,			
	eCELLTRGB,			
	eCELLVALS,			
	eCELLSTR,			
	eCELLAUXS,			
	eCELLREFS,			
	eCELLPROPERTIES,		
	eCELLATTR,			
	eALLCELLWIDTH,			
	eCELLSTR2,			// 170
	eCELLDAT,			
	eCELLKEYS,			
	eCELLVALS2,			
	eCELLALIGNI,			
	eCELLSTR3,			
	eCELLTXT,			
	eCELLISHINT,			
	eCELLONHINT,			
	eCELLATTR2,			
	eCELLRTS,			// 180
	eCELLEDITS,
	eCELLDRAGROW,
	eCELLFONTNAME,
	eCELLFONTSIZE,
	eCELLFONTSTYLE,
	eCELLMERGENAME,
	eCELLHEADIMG,
	eCELLHEIGHT,
	eCELLHEADH,
	eCELLCUSTOMNAME,
	eCELLCHARTLIMIT,
	// cell margin
	eCellMargin0 = 200,		// 200
	eCellMargin1,
	eCellMargin2,
	eCellMargin3,

	// map 
	eMAPCAPTION = 300,
	eMAPHEIGHT,
	eMAPWIDTH,	
	eMAPFNAME,	
	eMAPFPOINT,	
	eMAPFSTYLE,	
	eMAPVERS,	
	eMAPNAME,	
	eMAPDESC,	
	eMAPTRXC,	
	eMAPTRXH,	
	eMAPTOPATH,	
	eMAPMAPM,	
	eMAPHELPN,	
	eMAPREPEATV,	
	eMAPWAITV,	
	eMAPIMAGEK,	
	eMAPIMAGEV,	
	eMAPGROUP,	
	eFORMTYPE,	
	eREPEATV,	
	ePUBLICR,	
	eMAPBKCLR,
	eMAPALPHA,
	eFORMCTRL,
};

// data field information
struct _field {
	int	num;
	char*	name;
} fieldInfo[] = {
	// Form
	{eNAME,		"name"},
	{eSIZE,		"size"},
	{eTYPE,		"type"},
	{eIOK,		"iok"},
	{eHKEYS,	"Hkeys"},
	{eLKEYS,	"Lkeys"},
	{eAKEYS,	"Akeys"},
	{eLEFT,		"left"},
	{eTOP,		"top"},
	{eWIDTH,	"width"},
	{eHEIGHT,	"height"},
	{eALIGN,	"align"},
	{eBORDERS,	"borders"},
	{eINDEX,	"index"},
	{eFSTYLE,	"fstyle"},
	{eFPOINT,	"fpoint"},
	{eFNAME,	"fname"},
	{eTRGB	,	"tRGB"},
	{ePRGB,		"pRGB"},
	{eBRGB,		"bRGB"},
	{eATTR,		"attr"},
	{eATTR2,	"attr2"},
	{ePROPERTIES,	"properties"},
	{eCHECKED,	"checked"},
	{eISHINT,	"isHint"},
	{eONHINT,	"onHint"},
	{eISEDIT,	"isEdit"},
	{eONEDIT,	"onEdit"},
	{eHCORGB,	"HcoRGB"},
	{eLCORGB,	"LcoRGB"},
	{eVALS0,	"vals0"},
	{eVALS1,	"vals1"},
	{eVALS2,	"vals2"},
	{eVALS3,	"vals3"},
	{eAUXS,		"auxs"},
	{eSTR,		"str"},
	{eINITC,	"initC"},
	{eREFS,		"refs"},
	{eOFFS,		"offs"},
	{eTABLE,	"table"},
	{eROWS,		"rows"},
	{eCOLS,		"cols"},
	{eSTR2,		"str2"},
	{eSTRGB,	"stRGB"},
	{eSPRGB,	"spRGB"},
	{eTABHEAD,	"tabHead"},
	{eTABDATA,	"tabData"},
	{eTABVISIBLE,	"tabVisible"},
	{eTABLEHEAD,	"tableHead"},
	{eEDITS,	"editS"},
	{eALIGNIMG,	"alignImg"},
	{eFIXSIZE,	"fixSize"},
	{eDESC,		"desc"},
	{eFIXEDROW,	"fixedrow"},
	{eFIXEDCOL,	"fixedcol"},
	{eHEADH,	"HeadH"},
	{eROWSOF,	"rowsof"},
	{eFORMOB_01,	"formOB_01"},
	{eFORMOB_02,	"formOB_02"},
	{eFORMOB_03,	"formOB_03"},
	{eFORMOB_04,	"formOB_04"},
	{eFORMOB_05,	"formOB_05"},
	{eFORMOB_06,	"formOB_06"},
	{eFORMOB_07,	"formOB_07"},
	{eFORMOB_08,	"formOB_08"},
	{eFORMOB_09,	"formOB_09"},
	{eFORMOB_10,	"formOB_10"},
	{eFORMOB_11,	"formOB_11"},
	{eFORMOB_12,	"formOB_12"},
	{eFORMOB_13,	"formOB_13"},
	{eFORMOB_14,	"formOB_14"},
	{eFORMOB_15,	"formOB_15"},
	{eFORMOB_16,	"formOB_16"},
	{eFORMOB_17,	"formOB_17"},
	{eFORMOB_18,	"formOB_18"},
	{eFORMOB_19,	"formOB_19"},
	{eFORMOB_20,	"formOB_20"},
	{eFORMOB_21,	"formOB_21"},
	{eFORMOB_22,	"formOB_22"},
	{eFORMOB_23,	"formOB_23"},
	{eFORMOB_24,	"formOB_24"},
	{eFORMOB_25,	"formOB_25"},
	{eFORMOB_26,	"formOB_26"},
	{eFORMOB_27,	"formOB_27"},
	{eFORMOB_28,	"formOB_28"},
	{eFORMOB_29,	"formOB_29"},
	{eFORMOB_30,	"formOB_30"},
	{eFORMOB_31,	"formOB_31"},
	{eFORMOB_32,	"formOB_32"},
	{eFORMOB_33,	"formOB_33"},
	{eFORMOB_34,	"formOB_34"},
	{eFORMOB_35,	"formOB_35"},
	{eRTS,		"rts"},
	{eAlpha,	"alpha"},
	{eOptionImg,	"OptionImg"},
	{eOptionPos,	"OptionPos"},
	{eSelectShape,	"SelectShape"},	
	{eLayoutLeft,	"LLeft"},
	{eLayoutTop,	"LTop"},
	{eLayoutWidth,	"LWidth"},	
	{eLayoutHeight,	"LHeight"},
	{eLayoutAvailable,"Available"},
	{eOptionImg2,	"OptionImg2"},
	{eOptionImg3,	"OptionImg3"},
	{eOptionRGB,	"opRGB"},
	{eBlinkShape,	"BlinkShape"},
	{eSelectFontRGB,"sfpRGB"},

	// margin
	{eMargin0,	"margin0"},
	{eMargin1,	"margin1"},
	{eMargin2,	"margin2"},
	{eMargin3,	"margin3"},

	// Cell (grid column, table row)
	{eCELLNAME,	"cellname"},
	{eCELLTYPE,	"celltype"},
	{eCELLIOK,	"celliok"},
	{eCELLKIND,	"cellkind"},
	{eCELLSIZE,	"cellsize"},
	{eCELLWIDTH,	"cellwidth"},
	{eCELLHEAD,	"cellhead"},
	{eCELLALIGNH,	"cellalignH"},
	{eCELLALIGNC,	"cellalignC"},
	{eCELLHPRGB,	"cellhpRGB"},
	{eCELLHTRGB,	"cellhtRGB"},
	{eCELLPRGB,	"cellpRGB"},
	{eCELLTRGB,	"celltRGB"},
	{eCELLVALS,	"cellvals"},
	{eCELLSTR,	"cellstr"},
	{eCELLAUXS,	"cellauxs"},
	{eCELLREFS,	"cellrefs"},
	{eCELLPROPERTIES,	"cellproperties"},
	{eCELLATTR,	"cellattr"},
	{eALLCELLWIDTH,	"allcellwidth"},
	{eCELLSTR2,	"cellstr2"},
	{eCELLDAT,	"celldat"},
	{eCELLKEYS,	"cellkeys"},
	{eCELLVALS2,	"cellvals2"},
	{eCELLALIGNI,	"cellalignI"},
	{eCELLSTR3,	"cellstr3"},
	{eCELLTXT,	"celltxt"},
	{eCELLISHINT,	"cellisHint"},
	{eCELLONHINT,	"cellonHint"},
	{eCELLATTR2,	"cellattr2"},
	{eCELLRTS,	"cellrts"},
	{eCELLEDITS,	"celleditS"},
	{eCELLFONTNAME,	"cellfname"},
	{eCELLFONTSIZE,	"cellfsize"},
	{eCELLFONTSTYLE,"cellfstyle"},
	{eCELLMERGENAME,"cellmerge"},
	{eCELLHEADIMG,	"cellstr4"},
	{eCELLHEIGHT,	"cellheight"},
	{eCELLHEADH,	"cellheadH"},
	{eCELLCHARTLIMIT, "chartLimit"},

	// cell margin
	{eCellMargin0,	"cellmargin0"},
	{eCellMargin1,	"cellmargin1"},
	{eCellMargin2,	"cellmargin2"},
	{eCellMargin3,	"cellmargin3"},

	// map 
	{eMAPCAPTION,	"mapcaption"},
	{eMAPHEIGHT,	"mapheight"},
	{eMAPWIDTH,	"mapwidth"},
	{eMAPFNAME,	"mapfname"},
	{eMAPFPOINT,	"mapfpoint"},
	{eMAPFSTYLE,	"mapfstyle"},
	{eMAPVERS,	"mapvers"},
	{eMAPNAME,	"mapname"},
	{eMAPDESC,	"mapdesc"},
	{eMAPTRXC,	"maptrxC"},
	{eMAPTRXH,	"maptrxH"},
	{eMAPTOPATH,	"maptoPath"},
	{eMAPMAPM,	"mapmapM"},
	{eMAPHELPN,	"maphelpN"},
	{eMAPREPEATV,	"maprepeatV"},
	{eMAPWAITV,	"mapwaitV"},
	{eMAPIMAGEK,	"mapimageK"},
	{eMAPIMAGEV,	"mapimageV"},
	{eMAPGROUP,	"mapGroup"},
	{eFORMTYPE,	"formtype"},
	{eREPEATV,	"repeatV"},
	{ePUBLICR,	"publicR"},
	{eMAPBKCLR,	"mapBkClr"},
	{eMAPALPHA,	"mapalpha"},
	{eFORMCTRL,	"formCtrl"},
};

// CPropertiesGrid

IMPLEMENT_DYNAMIC(CPropertiesGrid, CMFCPropertyGridCtrl)

CPropertiesGrid::CPropertiesGrid()
{
	m_formItem = NULL;
	m_mapH = NULL;

	m_fN = -1;
	m_propMap.RemoveAll();
	m_fieldMap.RemoveAll();
	for (int ii = 0; ii < sizeof(fieldInfo)/sizeof(_field); ii++)
		m_fieldMap.SetAt(fieldInfo[ii].name, (void *)fieldInfo[ii].num);
	m_control.RemoveAll();

	m_controlcnt = 0;
	m_mapFOB.RemoveAll();
}

CPropertiesGrid::~CPropertiesGrid()
{
	int	key;
	CString skey, str;
	CPtrArray* arr;

	RemoveAll();
	for (POSITION pos = m_propMap.GetStartPosition(); pos; )
	{
		m_propMap.GetNextAssoc(pos, key, arr);
		for (int ii = 0; ii < arr->GetCount(); ii++)
			delete (Cproperty *)arr->GetAt(ii);
		arr->RemoveAll();
		delete arr;
	}
	m_propMap.RemoveAll();
	m_fieldMap.RemoveAll();
	m_control.RemoveAll();
	m_mapFOB.RemoveAll();
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
	CString tmp;

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
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
				data = var.bstrVal;
				if (m_fN == -2)
				{
					for (int ii = 0; ii < m_formItem->GetCount(); ii++)
					{
						if (m_formItem->GetForm(ii)->bCommon)
							setValue(prop, ii, -1, data);
					}
				}
				else
				{
					tmp = getValue(prop, m_fN, -1);
					setValue(prop, m_fN, -1, data);


					if (prop->m_head == _T("Name")) // ljh
					{	
						CWnd *pWnd = NULL;
						while(pWnd == NULL)
							pWnd = AfxGetMainWnd();

						pWnd->PostMessage(WM_USER, ID_USR_REFRESHCTRL, MAKEWORD(1, 0));
					}
				}
			}
		}
		break;
	}
	return 0;
}

void CPropertiesGrid::addProperties(CPtrArray* arr, CString ctrl)
{
	CString path, text, str;
	CString tmps, sym, tmp;
	char	wb[1024];

	if (!m_control.Lookup(ctrl, sym))
		return;

	parse(sym, _T(";"));


	while (arr->GetCount() > m_controlcnt)
	{
		delete (Cproperty *)arr->GetAt(m_controlcnt);
		arr->RemoveAt(m_controlcnt);
	}

	path.Format(_T("%s\\%s\\control.ini"), (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0), TABDIR);
	AddCMapOb(_T("-1"));

	//for (int ii = 1; ii < 20; ii++)
	for (int ii = 1; ii < CUSTOMCOUNT; ii++)
	{
		tmps.Format(_T("%02d"), ii);
		GetPrivateProfileString(sym, tmps, _T(""), wb, sizeof(wb), path);

		text = wb;
		tmp.Format(_T("formOB_%02d"), ii);
		if (text.IsEmpty())
			break;

		Cproperty*	prop;
		prop = new Cproperty();

		prop->m_head = text;
		prop->m_type = 1;
		prop->m_val1 = _T("");
		prop->m_val2 = _T("");
		prop->m_keys = tmp;
		prop->m_desc = _T("");

		arr->Add(prop);
	}	
}

void CPropertiesGrid::Initialize(struct _mapH* mapH, CFormItem* formItem)
{
	m_mapH = mapH;
	m_formItem = formItem;
	loadFile();
}

bool CPropertiesGrid::SetIndex(int fN)
{
	if (m_fN == fN)
		return false;

	m_fN = fN;
	return true;
}

void CPropertiesGrid::SetPropInfo(int kind, CString info)
{
	CString		tmps;
	Cproperty*	prop;
	CPtrArray*	arr;

	if (!m_propMap.Lookup(kind, arr))
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
	m_propMap.SetAt(kind, arr);

	if (kind == FM_CONTROL)
		m_controlcnt = arr->GetSize();
}

void CPropertiesGrid::SetProperties(int kind, int fN, int cellN)
{
	CPtrArray*	arr;

	Clear();
	if (!m_propMap.Lookup(kind, arr))
		return;

	Cproperty*	prop;
	CString		tmps, tmp;
	int		limit;
	DWORD		style;

	m_kind = kind;
	m_fN = fN;
	if (m_fN == -2)
	{
		for (int ii = 0; ii < m_formItem->GetCount(); ii++)
		{
			if (m_formItem->GetForm(ii)->bCommon)
			{
				fN = ii;
				break;
			}
		}
	}

	if (kind == FM_CONTROL)
	{
		tmps = m_formItem->GetFormStr(fN, _T("str"));
		if (!tmps.IsEmpty())
			addProperties(arr, tmps);
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

			pEdit = new CPropertyGridEditProperty(limit, style, prop->m_head, (_variant_t)getValue(prop, fN, cellN), prop->m_desc, (DWORD)prop);
			if (prop->m_keys.CompareNoCase(_T("str")) == 0)
			{
				CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();				
				tmps = m_formItem->GetFormStr(fN, _T("str"));
				struct _formR* formR = (struct _formR*)m_formItem->GetForm(fN);

				if (!tmps.IsEmpty() && !pFrame->IsLanguage(tmps) && formR->properties & PR_LANGUAGE)
					pEdit->SetColor(TRUE);
			}

			if (!prop->m_val1.IsEmpty() && prop->m_val1.GetAt(0) == _T('1'))
				pEdit->AllowEdit(FALSE);
			else
				pEdit->AllowEdit(TRUE);
			AddProperty(pEdit);
			break;

		case 2:	// edit + button
			CPropertyGridButtonProperty*	pButton;

			pButton = new CPropertyGridButtonProperty(false, atoi(prop->m_val2), prop->m_head, (_variant_t)getValue(prop, fN, cellN), prop->m_desc, (DWORD)prop);
			pButton->SetInfo(this, m_fN, m_mapH, m_formItem);
			if (!prop->m_val1.IsEmpty() && prop->m_val1.GetAt(0) == _T('1'))
				pButton->AllowEdit(FALSE);
			else
				pButton->AllowEdit(TRUE);
			AddProperty(pButton);
			break;
		
		case 3:	// combo
			CMFCPropertyGridProperty*	pCombo;

			tmps = getValue(prop, fN, cellN);
			pCombo = new CMFCPropertyGridProperty(prop->m_head, (_variant_t)_T(""), prop->m_desc, (DWORD)prop);
			fillCombo(pCombo, prop->m_val1, tmps);
			pCombo->AllowEdit(FALSE);
			AddProperty(pCombo);
			break;

		case 4:	// button
			CPropertyGridButtonProperty*	pfButton;

			pfButton = new CPropertyGridButtonProperty(true, atoi(prop->m_val2), prop->m_head, (_variant_t)getValue(prop, fN, cellN), prop->m_desc, (DWORD)prop);
			pfButton->SetInfo(this, m_fN, m_mapH, m_formItem);
			pfButton->AllowEdit(FALSE);
			AddProperty(pfButton);
			break;
		}
	}
}

void CPropertiesGrid::Refresh(int index)
{
	if (index < 0 || index >= GetPropertyCount() || m_fN < 0)
		return;

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
	LONG	key;

	if (m_fieldMap.Lookup(keys, (void *&)key))
		return (int)key;

	return -1;		
}

CString CPropertiesGrid::getValue(Cproperty* prop, int fN, int col)
{
	if (fN >= m_formItem->GetCount())
		return _T("");

	CString tmps = _T("");
	int	id = getKey(prop->m_keys);

	switch (id)
	{
	case eNAME:		// name
		tmps = m_formItem->GetFormStr(fN, _T("name"));
		break;
	case eSIZE:		// size
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->size);
		break;
	case eTYPE:		// type
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->type);
		break;
	case eIOK:		// iok
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->iok);
		break;
	case eHKEYS:		// Hkeys
		tmps.Format(_T("%d"), HIWORD(m_formItem->GetForm(fN)->keys));
		break;
	case eLKEYS:		// Lkeys
		tmps.Format(_T("%d"), LOWORD(m_formItem->GetForm(fN)->keys));
		break;
	case eAKEYS:		// Akeys
		if (m_formItem->GetForm(fN)->kind == FM_TABLE && m_formItem->GetForm(fN)->keys == 0)
		{
			struct _repR *repR   = (struct _repR *)m_formItem->GetForm(fN)->auxR;
			if (repR)
				m_formItem->GetForm(fN)->keys = (m_formItem->GetForm(fN)->bottom - m_formItem->GetForm(fN)->top) / repR->rows;
		}
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->keys);
		break;
	case eLEFT:		// left
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->left);
		break;
	case eTOP:		// top
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->top);
		break;
	case eWIDTH:		// width
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->right - m_formItem->GetForm(fN)->left);
		break;
	case eHEIGHT:	// height
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->bottom - m_formItem->GetForm(fN)->top);
		break;
	case eALIGN:	// align
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->alignment);
		break;
	case eBORDERS:	// borders
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->borders);
		break;
	case eINDEX:	// index
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->index);
		break;
	case eFSTYLE:	// fstyle
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->fStyle);
		break;
	case eFPOINT:	// fpoint
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->fPoint);
		if (atoi(tmps) == 0 && m_formItem->GetForm(fN)->fStyle == FS_NONE)
			tmps.Format(_T("%d"), m_mapH->fPoint);
		break;
	case eFNAME:	// fname
		tmps = m_formItem->GetFormStr(fN, _T("fName"));
		if (tmps.IsEmpty() && m_formItem->GetForm(fN)->fStyle == FS_NONE)
			tmps = CString(m_mapH->fName);
		break;
	case eTRGB:	// tRGB
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->tRGB);
		break;
	case ePRGB:	// pRGB
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->pRGB);
		break;
	case eBRGB:	// bRGB
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->bRGB);
		break;
	case eATTR:	// attr
		if (hex2Int(prop->m_val2) & m_formItem->GetForm(fN)->attr)
			tmps = _T("1");
		else
			tmps = _T("0");
		break;
	case eATTR2:	// attr2
		if (m_formItem->GetForm(fN)->kind == FM_EDIT)
		{
			if (hex2Int(prop->m_val2) & GO_BLINK)
			{
				if (hex2Int(prop->m_val2) & m_formItem->GetForm(fN)->attr2)
					tmps = _T("1");
				else
					tmps = _T("0");
			}
			break;

		}

		// Grid Operation 변경
		if (prop->m_val2.IsEmpty())
		{
			if (m_formItem->GetForm(fN)->attr2 & GO_MERGE)
				tmps = _T("1");
			else if (m_formItem->GetForm(fN)->attr2 & GO_MULTI)
				tmps = _T("2");
			else
				tmps = _T("0");
			break;
		}
		
		if (hex2Int(prop->m_val2) & m_formItem->GetForm(fN)->attr2)
			tmps = _T("1");
		else
			tmps = _T("0");
		break;
	case ePROPERTIES:	// properties
		{
			CString str = prop->m_val2;
			int	nCnt = 1;

			tmps = "0";
			while (!str.IsEmpty())
			{
				CString str2 = parse(str, _T(";"));
				if (hex2Int(str2) & m_formItem->GetForm(fN)->properties)
					tmps.Format(_T("%d"), nCnt);
				nCnt++;
			}
		}
		break;
	case eCHECKED:	// checked
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->checked);
		break;
	case eISHINT:	// isHint
	//	tmps.Format(_T("%d"), m_formItem->GetForm(fN)->isHint);
		break;
	case eONHINT:	// onHint
		tmps = m_formItem->GetFormStr(fN, _T("dat"));
		break;
	case eISEDIT:	// isEidt
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->isEdit);
		break;
	case eONEDIT:	// onEdit
		tmps.Format("%ld", m_formItem->GetForm(fN)->onEdit);
		break;
	case eHCORGB:	// HcoRGB
		tmps.Format(_T("%d"), HIWORD(m_formItem->GetForm(fN)->onEdit));
		break;
	case eLCORGB:	// LcoRGB
		tmps.Format(_T("%d"), LOWORD(m_formItem->GetForm(fN)->onEdit));
		break;
	case eVALS0:	// vals0
		break;
	case eAUXS:	// auxs
		tmps = m_formItem->GetFormStr(fN, _T("auxS"));
		break;
	case eSTR:	// str
		/*if (m_formItem->GetForm(fN)->kind == FM_CONTROL)
		{
			CString str, idx;
			int	nidx;
			tmps = m_formItem->GetFormStr(fN, _T("str"));
			if (m_control.Lookup(tmps, idx))
			{
				nidx = atoi(parse(idx, _T(";")));
				tmps.Format("%d", nidx);
			}
			else
				tmps.Format("%d", 0);
		}
		else*/
			tmps = m_formItem->GetFormStr(fN, _T("str"));

			if (!m_formItem->GetForm(fN))
				break;

			if (m_formItem->GetForm(fN)->kind == FM_CONTROL)
			{
				CString str, controlkey;

				if (m_control.Lookup(tmps, str))
				{
					controlkey = parse(str, _T(";"));
					m_formItem->SetFormStr(fN, _T("dat"), str);
				}
			}
		break;
	case eINITC:	// initC
		break;
	case eREFS:	// refs
		tmps = m_formItem->GetFormStr(fN, _T("refs"));
		break;
	case eOFFS:	// offs
		tmps.Format(_T("%d"), min(m_formItem->GetForm(fN)->size, m_formItem->GetForm(fN)->offs));
		break;
	case eTABLE:	// table
		break;
	case eROWS:	// rows
		{
			struct _repR* repR = (struct _repR *)m_formItem->GetForm(fN)->auxR;
			if (repR)
				tmps.Format(_T("%d"), repR->rows);
		}
		break;
	case eCOLS:	// cols
		{
			struct _repR* repR = (struct _repR *)m_formItem->GetForm(fN)->auxR;
			if (repR)
				tmps.Format(_T("%d"), repR->cols);
		}
		break;
	case eSTR2:	// str2
		tmps = m_formItem->GetFormStr(fN, _T("str2"));
		break;
	case eSTRGB:	// stRGB
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->stRGB);
		break;
	case eSPRGB:	// spRGB
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->spRGB);
		break;
	case eAlpha:	// alpha

		if (fN == -1)
			tmps = _T("0");
		else
			tmps.Format(_T("%d"), m_formItem->GetForm(fN)->alpha);
		break;
	case eSelectShape:
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->selectShape);
		break;
	case eOptionImg:	// Option Image Path
		tmps = m_formItem->GetFormStr(fN, _T("optionimg"));
		break;
	case eOptionImg2:	// option image path2
		tmps = m_formItem->GetFormStr(fN, _T("optionimg2"));
		break;
	case eOptionImg3:	// pption image path3
		tmps = m_formItem->GetFormStr(fN, _T("optionimg3"));
		break;
	case eOptionPos:
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->optionPos);
		break;
	case eOptionRGB:
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->opRGB);
		break;
	case eBlinkShape:
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->blinkShape);
		break;
	case eSelectFontRGB:
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->sfpRGB);
		break;
	// Layout
	case eLayoutLeft:
		{
			struct _Layout* layout;
			CString sName = m_formItem->GetCurrLayoutName();
			m_formItem->GetForm(fN)->layout->Lookup(sName, (void*&)layout);
			tmps.Format(_T("%d"), layout->left);
		}
		break;
	case eLayoutTop:
		{
			struct _Layout* layout;
			CString sName = m_formItem->GetCurrLayoutName();
			m_formItem->GetForm(fN)->layout->Lookup(sName, (void*&)layout);
			tmps.Format(_T("%d"), layout->top);
		}
		break;
	case eLayoutWidth:
		{
			struct _Layout* layout;
			CString sName = m_formItem->GetCurrLayoutName();
			m_formItem->GetForm(fN)->layout->Lookup(sName, (void*&)layout);
			tmps.Format(_T("%d"), (layout->right - layout->left));
		}
		break;
	case eLayoutHeight:
		{
			struct _Layout* layout;
			CString sName = m_formItem->GetCurrLayoutName();
			m_formItem->GetForm(fN)->layout->Lookup(sName, (void*&)layout);
			tmps.Format(_T("%d"), (layout->bottom - layout->top));
		}
		break;
	case eLayoutAvailable:
		{
			struct _Layout* layout;
			CString sName = m_formItem->GetCurrLayoutName();
			m_formItem->GetForm(fN)->layout->Lookup(sName, (void*&)layout);
			tmps = _T("0");
			if (layout->properties & PR_VISIBLE)
				tmps = _T("1");
		}
		break;

	// margin
	case eMargin0:
	case eMargin1:
	case eMargin2:
	case eMargin3:
		if (m_formItem->GetForm(fN)->margin != NULL && (m_formItem->GetForm(fN)->margin->GetUpperBound() >= id - eMargin0))
			tmps = m_formItem->GetForm(fN)->margin->GetAt(id - eMargin0);
		break;

	case eTABHEAD:	// tabHead
		{
			if (m_formItem->GetForm(fN)->keys)
			{
				CString str = m_formItem->GetFormStr(fN, _T("dat"));
				for (int ii = 0; ii < m_formItem->GetForm(fN)->checked + 1; ii++)
					tmps = parse(str, _T("\t"));
			}
		}
		break;
	case eTABDATA:	// tabData
		{
			CString tmp;
			if (m_formItem->GetForm(fN)->keys)
			{
				CString str = m_formItem->GetFormStr(fN, _T("str"));
				for (int ii = 0; ii < m_formItem->GetForm(fN)->checked + 1; ii++)
					tmp = parse(str, _T("\t"));
				parse(tmp, _T(":"));
				tmps = parse(tmp, _T(":"));
			}
		}
		break;
	case eTABVISIBLE:	// tabVisible
		{
			if (m_formItem->GetForm(fN)->keys)
			{
				CString str = m_formItem->GetFormStr(fN, _T("str"));
				for (int jj = 0; jj < m_formItem->GetForm(fN)->checked + 1; jj++)
					tmps = parse(str, _T("\t"));
				tmps = parse(tmps, _T(":"));
			}
		}
		break;
	case eTABLEHEAD:	// tableHead
		{
			if (m_formItem->GetForm(fN)->attr2 & GO_XHEAD)
				tmps = _T('2');
			else if (m_formItem->GetForm(fN)->attr2 & GO_MULTI)
				tmps = _T('0');
			else
				tmps = _T('1');
		}
		break;
	case eEDITS:	// editS
		tmps = m_formItem->GetFormStr(fN, _T("editS"));
		break;
	case eALIGNIMG:	// alignImg
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->alignImage);
		break;
	case eFIXSIZE:	// fixSize
		break;
	case eDESC:	// desc
		tmps = m_formItem->GetFormStr(fN, _T("desc"));
		break;
	case eFIXEDROW:
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->fixrow);
		break;
	case eFIXEDCOL:
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->fixcol);
		break;
	case eHEADH:
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->headH);
		break;
	case eROWSOF:	// rowsof
		tmps.Format(_T("%d"), m_formItem->GetForm(fN)->rowof);
		break;
	case eFORMOB_01:
	case eFORMOB_02:
	case eFORMOB_03:
	case eFORMOB_04:
	case eFORMOB_05:
	case eFORMOB_06:
	case eFORMOB_07:
	case eFORMOB_08:
	case eFORMOB_09:
	case eFORMOB_10:	// formOB_01 init parse
	case eFORMOB_11:
	case eFORMOB_12:
	case eFORMOB_13:
	case eFORMOB_14:
	case eFORMOB_15:
	case eFORMOB_16:
	case eFORMOB_17:
	case eFORMOB_18:
	case eFORMOB_19:
	case eFORMOB_20:
	case eFORMOB_21:
	case eFORMOB_22:
	case eFORMOB_23:
	case eFORMOB_24:
	case eFORMOB_25:
	case eFORMOB_26:
	case eFORMOB_27:
	case eFORMOB_28:
	case eFORMOB_29:
	case eFORMOB_30:
	case eFORMOB_31:
	case eFORMOB_32:
	case eFORMOB_33:
	case eFORMOB_34:
	case eFORMOB_35:
		{
			CString tmp, idx;
			CString orign, rowdata, head;
			orign = m_formItem->GetFormStr(fN, _T("str2"));
			while (!orign.IsEmpty())
			{
				rowdata = parse(orign, _T(":"));
				head = parse(rowdata, _T("="));
				if (head.CompareNoCase(prop->m_head))
					continue;
				tmp.Format(_T("%s=%s:"), head, rowdata);
				idx.Format(_T("%02d"), id);
				AddCMapOb(idx, tmp);
				tmps = rowdata;
				break;
			}
		}
		break;
	case eRTS:	// rts
		tmps = m_formItem->GetFormStr(fN, _T("rts"));
		break;
	case eALLCELLWIDTH:	// allcellwidth
		{
			struct _repR* repR = (struct _repR *)m_formItem->GetForm(fN)->auxR;
			if (repR->rows > 0)
			{
				CCellItem* cellItem = new CCellItem(repR);
				tmps.Format(_T("%d"), cellItem->GetCell(0)->width);
				delete cellItem;
			}
			else
				tmps = _T("80");
		}
		break;

	}

	if (id >= eCELLNAME && id < eMAPCAPTION)
	{
		struct _repR* repR = (fN >= 0 ? (struct _repR *)m_formItem->GetForm(fN)->auxR : NULL);
		if (repR != NULL && (repR->selected >= 0 || col >= 0))
		{
			if (col < 0)
				col = repR->selected;
			
			CCellItem* cellItem = new CCellItem(repR);
			struct _cellR* cellR = cellItem->GetCell(col);
			switch (id)
			{
			case eCELLNAME:	// cellname
				tmps = cellR->name;
				break;
			case eCELLTYPE:	// celltype
				tmps.Format(_T("%d"), cellR->type);
				break;
			case eCELLIOK:	// celliok
				if (cellR->iok == EIO_OUTPUT && cellR->kind == CK_CHECK)
					cellR->iok = EIO_INOUT;
				if ((cellR->iok == EIO_OUTPUT || cellR->iok == EIO_INOUT || cellR->iok == EIO_INPUT) && cellR->kind == CK_BUTTON)
					cellR->iok = EIO_NOP;
				tmps.Format(_T("%d"), cellR->iok);
				break;
			case eCELLKIND:	// cellkind
				if (cellR->iok == EIO_OUTPUT && cellR->kind == CK_CHECK)
					cellR->kind = CK_NORMAL;
				if ((cellR->iok == EIO_OUTPUT || cellR->iok == EIO_INOUT || cellR->iok == EIO_INPUT) && cellR->kind == CK_BUTTON)
					cellR->kind = CK_NORMAL;
				tmps.Format(_T("%d"), cellR->kind);
				break;
			case eCELLSIZE:	// cellsize
				tmps.Format(_T("%d"), cellR->size);
				break;
			case eCELLWIDTH:	// cellwidth
				tmps.Format(_T("%d"), cellR->width);
				break;
			case eCELLHEAD:	// cellhead
				tmps = cellR->head;
				break;
			case eCELLALIGNH:	// cellalignH
				tmps.Format(_T("%d"), cellR->alignH);
				break;
			case eCELLALIGNC:	// cellalignC
				tmps.Format(_T("%d"), cellR->alignC);
				break;
			case eCELLHPRGB:	// cellhpRGB
				tmps.Format(_T("%d"), cellR->hpRGB);
				break;
			case eCELLHTRGB:	// cellhtRGB
				tmps.Format(_T("%d"), cellR->htRGB);
				break;
			case eCELLPRGB:	// cellpRGB
				tmps.Format(_T("%d"), cellR->pRGB);
				break;
			case eCELLTRGB:	// celltRGB
				tmps.Format(_T("%d"), cellR->tRGB);
				break;
			case eCELLVALS:	// cellvals
				tmps.Format(_T("%d"), cellR->vals);
				break;
			case eCELLSTR:	// cellstr
				tmps = cellR->str;
				break;
			case eCELLAUXS:	// cellauxs
				tmps = cellR->auxS;
				break;
			case eCELLREFS:		// cellrefs
				break;
			case eCELLPROPERTIES:	// cellproperties
				{
					CString str = prop->m_val2;
					int	nCnt = 1;

					tmps = _T("0");
					while (!str.IsEmpty())
					{
						CString str2 = parse(str, _T(";"));
						if (hex2Int(str2) & cellR->properties)
							tmps.Format(_T("%d"), nCnt);
						nCnt++;
					}
				}
				break;
			case eCELLATTR:	// cellattr
				if (hex2Int(prop->m_val2) & cellR->attr)
					tmps = _T("1");
				else
					tmps = _T("0");
				break;
			case eCELLSTR2:	// cellstr2
				tmps = cellR->str2;
				break;
			case eCELLDAT:	// celldat
				tmps = cellR->dat;
				break;
			case eCELLKEYS:	// cellkeys
				tmps.Format(_T("%d"), cellR->keys);
				break;
			case eCELLVALS2:	// cellvals2
				tmps.Format(_T("%ld"), cellR->vals2);
				break;
			case eCELLALIGNI:	// cellalignI
				break;
			case eCELLSTR3:	// cellstr3
				tmps = cellR->str3;
				break;
			case eCELLTXT:	// celltxt
				tmps = cellR->txt;
				break;
			case eCELLISHINT:	// cellisHint
				break;
			case eCELLONHINT:	// cellonHint
				break;
			case eCELLATTR2:	// cellattr2
				if (hex2Int(prop->m_val2) & cellR->attr2)
					tmps = _T("1");
				else
					tmps = _T("0");
				break;
				break;
			case eCELLRTS:	// cellrts
				tmps = cellR->rtssy;
				break;
			case eCELLEDITS:	// edit format
				tmps = cellR->editS;
				break;
			case eCELLFONTNAME:
				tmps = cellR->fName;
				break;
			case eCELLFONTSIZE:
				tmps.Format(_T("%d"), cellR->fPoint);
				break;
			case eCELLFONTSTYLE:
				tmps.Format(_T("%d"), cellR->fStyle);
				break;
			case eCELLMERGENAME:	// Merge Column
				tmps = cellR->merge;
				break;
			case eCELLHEADIMG:	// columnhead image
				tmps = cellR->str4;
				break;
			case eCELLHEIGHT:	// cell height in multiline
				tmps.Format(_T("%d"), cellR->height);
				break;
			case eCELLHEADH:	// head height in multiline
				tmps.Format(_T("%d"), cellR->headH);
				break;
			case eCELLCHARTLIMIT:
				tmps.Format(_T("%d"), cellR->chartLimit);
				break;
			case eCellMargin0:	// cell margin
			case eCellMargin1:
			case eCellMargin2:
			case eCellMargin3:
				if (cellR->margin != NULL && (cellR->margin->GetUpperBound() >= id - eCellMargin0))
					tmps = cellR->margin->GetAt(id - eCellMargin0);
				break;
			}
			delete cellItem;
		}
	}
	
	if (id >= eMAPCAPTION)
	{
		switch (id) // form
		{
		case eMAPCAPTION:	// mapcaption
			tmps = m_mapH->caption;
			break;
		case eMAPHEIGHT:	// mapheight
			tmps.Format(_T("%d"), m_mapH->height);
			break;
		case eMAPWIDTH:	// mapwidth
			tmps.Format(_T("%d"), m_mapH->width);
			break;
		case eMAPFNAME:	// mapfname
			tmps = m_mapH->fName;
			break;
		case eMAPFPOINT:	// mapfpoint
			tmps.Format(_T("%d"), m_mapH->fPoint);
			break;
		case eMAPFSTYLE:	// mapfstyle
			tmps.Format(_T("%d"), m_mapH->fStyle);
			break;
		case eMAPVERS:	// mapvers
			break;
		case eMAPNAME:	// mapname
			tmps = m_mapH->mapN;
			break;
		case eMAPDESC:	// mapdesc
			if (strlen(m_mapH->mapdesc) == 0)
			{
				tmps = m_mapH->caption;
				CopyMemory(m_mapH->mapdesc, m_mapH->caption, min(strlen(m_mapH->caption), sizeof(m_mapH->mapdesc)-1));
			}
			else
				tmps = m_mapH->mapdesc;
			break;
		case eMAPTRXC:	// maptrxC
			tmps = m_mapH->trxC;
			break;
		case eMAPTRXH:	// maptrxH
			break;
		case eMAPTOPATH:	// maptoPath
			tmps.Format(_T("%d"), m_mapH->toPath);
			break;
		case eMAPMAPM:	// mapmapM
			tmps.Format(_T("%d"), m_mapH->mapM);
			break;
		case eMAPHELPN:	// maphelpN
			break;
		case eMAPREPEATV:	// maprepeatV
			tmps.Format(_T("%d"), m_mapH->repeatV);
			break;
		case eMAPWAITV:	// mapwaitV
		//	tmps.Format(_T("%d"), m_mapH->waitV);
			break;
		case eMAPIMAGEK:	// mapimageK
			tmps.Format(_T("%d"), 1/*m_mapH->bgK*/);//ljh
			break;
		case eMAPIMAGEV:	// mapimageV
			tmps = m_mapH->bgFileN;
			break;
		case eMAPGROUP:	// mapGroup
		//	tmps.Format(_T("%d"), m_mapH->mapGroup);
			break;
		case eFORMTYPE:	// formtype
			tmps.Format(_T("%d"), m_mapH->formtype);
			break;
		case eREPEATV:	// repeatV
			tmps.Format(_T("%d"), m_mapH->repeatV);
			break;
		case ePUBLICR:	// publicR
		//	tmps = m_mapH->publicR;
			break;
		case eMAPBKCLR:	// mapBkClr
			tmps.Format(_T("%d"), m_mapH->bgClr);
			break;
		case eMAPALPHA:
			tmps.Format(_T("%d"), m_mapH->alpha);
			break;
		case eFORMCTRL:
			tmps = m_mapH->formCtrl;
			break;
		}
	}
	
	return tmps;
}

void CPropertiesGrid::setValue(Cproperty* prop, int fN, int col, CString data)
{
	CString sdata;
	struct	_formR*	formR = nullptr;
	if (fN >= m_formItem->GetCount())
		return;

	int	index = fN, id = getKey(prop->m_keys);
	if (prop->m_type == 3)	// combo
		data = getComboData(prop->m_val1, data);

	m_mapH->modified = true;
	switch (id)
	{
	case eNAME:		// name
		m_formItem->SetFormStr(fN, _T("name"), data);
		break;
	case eSIZE:		// size
		m_formItem->GetForm(fN)->size = atoi(data);
		break;
	case eTYPE:		// type
		m_formItem->GetForm(fN)->type = atoi(data);
		break;
	case eIOK:		// iok
		m_formItem->GetForm(fN)->iok = atoi(data);
		break;
	case eHKEYS:		// Hkeys
		m_formItem->GetForm(fN)->keys = MAKELONG(LOWORD(m_formItem->GetForm(fN)->keys), atoi(data));
		break;
	case eLKEYS:		// Lkeys
		m_formItem->GetForm(fN)->keys = MAKELONG(atoi(data), HIWORD(m_formItem->GetForm(fN)->keys));
		break;
	case eAKEYS:		// keys
		if (m_formItem->GetForm(fN)->kind == FM_TAB)
		{
			if (atoi(data) <= 0)
				return;

			CString tmps;
			struct _formR* formR = m_formItem->GetForm(fN);
			for (int ii = formR->keys; ii < atoi(data); ii++)
			{
				tmps.Format(_T("Tab%d\t"), ii);
				m_formItem->SetFormStr(fN, _T("dat"), m_formItem->GetFormStr(fN, _T("dat")) + tmps);
				tmps.Format(_T("1:%d\t"), m_formItem->GetForm(fN)->keys);
				m_formItem->SetFormStr(fN, _T("str"), m_formItem->GetFormStr(fN, _T("str")) + tmps);
			}

			if (atoi(data) < (int)formR->keys)
			{
				CString	tmp2, rtmp, rtmp2;

				tmps = m_formItem->GetFormStr(fN, "dat");
				tmp2 = m_formItem->GetFormStr(fN, "str");
				rtmp = _T("");
				rtmp2 = _T("");
				for (int ii = 0; ii < atoi(data); ii++)
				{
					rtmp += parse(tmps, _T("\t"));
					rtmp += _T('\t');
					rtmp2 += parse(tmp2, _T("\t"));
					rtmp2 += _T('\t');
				}
				m_formItem->SetFormStr(fN, _T("dat"), rtmp);
				m_formItem->SetFormStr(fN, _T("str"), rtmp2);
				formR->checked = 0;
			}
			GetParent()->PostMessage(WM_USER, 1, fN);	// Change Property
		}
		m_formItem->GetForm(fN)->keys = atoi(data);
		break;
	case eLEFT:		// left
		m_formItem->GetForm(fN)->right += atoi(data) - m_formItem->GetForm(fN)->left;
		m_formItem->GetForm(fN)->left = atoi(data);
		index = -1;
		break;
	case eTOP:		// top
		m_formItem->GetForm(fN)->bottom += atoi(data) - m_formItem->GetForm(fN)->top;
		m_formItem->GetForm(fN)->top = atoi(data);
		index = -1;
		break;
	case eWIDTH:		// width
		m_formItem->GetForm(fN)->right = atoi(data) + m_formItem->GetForm(fN)->left;
		index = -1;
		break;
	case eHEIGHT:	// height
		m_formItem->GetForm(fN)->bottom = atoi(data) + m_formItem->GetForm(fN)->top;
		index = -1;
		break;
	case eALIGN:	// align
		m_formItem->GetForm(fN)->alignment = atoi(data);
		break;
	case eBORDERS:	// borders
		m_formItem->GetForm(fN)->borders = atoi(data);
		break;
	case eINDEX:	// index
		m_formItem->GetForm(fN)->index = atoi(data);
		break;
	case eFSTYLE:	// fstyle
		if (m_formItem->GetForm(fN)->fStyle == FS_NONE && atoi(data) != FS_NONE)
		{
			m_formItem->SetFormStr(fN, _T("fName"), m_mapH->fName);
			m_formItem->GetForm(fN)->fPoint = m_mapH->fPoint;
			AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_REFRESHPRO, -1);
		}
		if (m_formItem->GetForm(fN)->fStyle != FS_NONE && atoi(data) == FS_NONE)
		{
			m_formItem->SetFormStr(fN, _T("fName"), _T(""));
			m_formItem->GetForm(fN)->fPoint = 0;
			AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_REFRESHPRO, -1);
		}
		m_formItem->GetForm(fN)->fStyle = atoi(data);
		break;
	case eFPOINT:	// fpoint
		m_formItem->GetForm(fN)->fPoint = atoi(data);
		//	m_formItem->GetForm(fN)->fStyle = FS_NORM;
		break;
	case eFNAME:	// fname
		m_formItem->SetFormStr(fN, _T("fName"), data);
		break;
	case eTRGB:	// tRGB
		m_formItem->GetForm(fN)->tRGB = atoi(data);
		break;
	case ePRGB:	// pRGB
		m_formItem->GetForm(fN)->pRGB = atoi(data);
		break;
	case eBRGB:	// bRGB
		m_formItem->GetForm(fN)->bRGB = atoi(data);
		break;
	case eATTR:	// attr
		if (data.GetAt(0) == '0')
			m_formItem->GetForm(fN)->attr &= ~hex2Int(prop->m_val2);
		else
			m_formItem->GetForm(fN)->attr |= hex2Int(prop->m_val2);
		break;

	case eATTR2:	// attr2
		// 		if (m_formItem->GetForm(fN)->kind == FM_EDIT)
		// 			break;

		if (prop->m_val2.IsEmpty())
		{
			switch (atoi(data))
			{
			case 0:
				m_formItem->GetForm(fN)->attr2 &= ~GO_MERGE;
				m_formItem->GetForm(fN)->attr2 &= ~GO_MULTI;
				break;
			case 1:
				m_formItem->GetForm(fN)->attr2 &= ~GO_MULTI;
				m_formItem->GetForm(fN)->attr2 |= GO_MERGE;
				break;
			case 2:
				m_formItem->GetForm(fN)->attr2 &= ~GO_MERGE;
				m_formItem->GetForm(fN)->attr2 |= GO_MULTI;
				break;
			}
			checkGridMultiLine(fN);
			break;
		}

		if (data.GetAt(0) == _T('0'))
		{
			m_formItem->GetForm(fN)->attr2 &= ~hex2Int(prop->m_val2);
			if (hex2Int(prop->m_val2) & GO_FLEX)
				GetParent()->PostMessage(WM_USER, 1, fN);	// Change Property
		}
		else
			m_formItem->GetForm(fN)->attr2 |= hex2Int(prop->m_val2);

		if (m_formItem->GetForm(fN)->kind == FM_EDIT)
			break;

		checkGridMultiLine(fN);
		break;
	case ePROPERTIES:	// properties
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
				m_formItem->GetForm(fN)->properties |= hex2Int(str2);
			else
				m_formItem->GetForm(fN)->properties &= ~hex2Int(str2);
			nCnt++;
		}

		if (m_formItem->GetForm(fN)->kind == FM_TAB)
			m_formItem->GetForm(fN)->scrollPos = 0;
		
		sdata = m_formItem->GetFormStr(fN, _T("str"));
		ChangeColor(sdata, m_formItem->GetForm(fN)->properties & PR_LANGUAGE ? true : false);
	}
	break;
	case eCHECKED:	// checked
		m_formItem->GetForm(fN)->checked = atoi(data);
		if (m_formItem->GetForm(fN)->kind == FM_TAB)
			GetParent()->PostMessage(WM_USER, 1, fN);	// Change Property
		break;
	case eISHINT:	// isHint
		//	m_formItem->GetForm(fN)->isHint = atoi(data);
		break;
	case eONHINT:	// onHint
		m_formItem->SetFormStr(fN, _T("dat"), data);
		break;
	case eISEDIT:	// isEdit
		m_formItem->GetForm(fN)->isEdit = atoi(data);
		break;
	case eONEDIT:	// onEdit
		m_formItem->GetForm(fN)->onEdit = atoi(data);
		break;
	case eHCORGB:	// HcoRGB
		m_formItem->GetForm(fN)->onEdit = MAKELONG(atoi(data), HIWORD(m_formItem->GetForm(fN)->onEdit));
		break;
	case eLCORGB:	// LcoRGB
		m_formItem->GetForm(fN)->onEdit = MAKELONG(LOWORD(m_formItem->GetForm(fN)->onEdit), atoi(data));
		break;
	case eVALS0:	// vals0
		break;
	case eAUXS:	// auxS
		m_formItem->SetFormStr(fN, _T("auxS"), data);
		break;
	case eSTR:	// str
		if (!m_formItem->GetForm(fN))
			break;

		if (m_formItem->GetForm(fN)->kind == FM_OBJECT && !data.IsEmpty())
		{
			if (!data.CompareNoCase(m_mapH->mapN))
			{
				CString	tmps;

				GetParent()->PostMessage(WM_USER, 1, fN);	// Change Property
				tmps = _T("Object Name must different from Map Name!!");
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_STATUSERR, (LPARAM)tmps.operator LPCTSTR());
				break;
			}
		}

		m_formItem->SetFormStr(fN, _T("str"), data);	
		formR = m_formItem->GetForm(fN);

		if (formR->properties & PR_LANGUAGE)
			ChangeColor(data, true);
		else 
			ChangeColor(data, false);

		if (m_formItem->GetForm(fN)->kind == FM_CONTROL)
		{
			CString str, controlkey;
			if (m_control.Lookup(data, str))
			{
				controlkey = parse(str, _T(";"));
				m_formItem->SetFormStr(fN, _T("dat"), str);
			}
		}
		break;
	case eINITC:	// initC
		break;
	case eREFS:	// refs
		m_formItem->SetFormStr(fN, _T("refs"), data);
		break;
	case eOFFS:	// offs
		if (!atoi(data))
		{
			m_formItem->GetForm(fN)->attr &= ~FA_FLOAT;
			m_formItem->GetForm(fN)->offs = 0;
		}
		else
		{
			m_formItem->GetForm(fN)->offs = min(m_formItem->GetForm(fN)->size, atoi(data));
			if (m_formItem->GetForm(fN)->size < atoi(data))
			{
				CString	tmps;

				GetParent()->PostMessage(WM_USER, 1, fN);	// Change Property
				tmps = _T("Data Length less then float digit!");
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_STATUSERR, (LPARAM)tmps.operator LPCTSTR());
			}
		}
		break;
	case eTABLE:	// table
		break;
	case eROWS:	// rows
		{
			struct _cellR *cellR;

			if (atoi(data) < 0)
				data = _T("0");

			struct _repR* repR = (struct _repR *)m_formItem->GetForm(fN)->auxR;
			CCellItem* cellItem = new CCellItem(repR);
			if (m_formItem->GetForm(fN)->kind == FM_TABLE && cellItem->GetCount() < atoi(data))
			{
				for (int ii = cellItem->GetCount(); ii < atoi(data); ii++)
				{
					cellR = cellItem->GetCell(cellItem->AddCell());
					cellR->pRGB = basINPUT;
					cellR->tRGB = 232;
					if (repR->rows)
					{
						cellR->hpRGB = cellItem->GetCell(repR->rows - 1)->hpRGB;
						cellR->htRGB = cellItem->GetCell(repR->rows - 1)->htRGB;
						cellR->width = cellItem->GetCell(repR->rows - 1)->width;
					}
					else
					{
						cellR->hpRGB = basPRGB;
						cellR->htRGB = 232;
						cellR->width = 80;
					}
					cellR->size = 0;
					//cellR->headN = 1;
					cellR->type = IO_NUM;
					strcpy_s(cellR->name, sizeof(cellR->name), getInitSymbol(fN, _T("Row")));
					cellR->alignC = AL_RIGHT;
					cellR->properties |= PR_VISIBLE;
				}
			}
			if (m_formItem->GetForm(fN)->kind == FM_TABLE)
			{
				while (atoi(data) < cellItem->GetCount())
					cellItem->RemoveLastCell(atoi(data));
			}
			repR->rows = atoi(data);
			delete cellItem;
		}
		break;
	case eCOLS:	// cols
		{
			struct _cellR* cellR;
			if (atoi(data) < 0)
				data = _T("0");

			struct _repR* repR = (struct _repR *)m_formItem->GetForm(fN)->auxR;
			CCellItem* cellItem = new CCellItem(repR);
			if ((m_formItem->GetForm(fN)->kind == FM_GRIDEX || m_formItem->GetForm(fN)->kind == FM_GRID ) && cellItem->GetCount() < atoi(data))
			{
				for (int ii = cellItem->GetCount(); ii < atoi(data); ii++)
				{
					cellR = cellItem->GetCell(cellItem->AddCell());
					cellR->pRGB = basINPUT;
					cellR->tRGB = 232;
					if (repR->cols)
					{
						cellR->hpRGB = cellItem->GetCell(repR->cols - 1)->hpRGB;
						cellR->htRGB = cellItem->GetCell(repR->cols - 1)->htRGB;
						cellR->width = cellItem->GetCell(repR->cols - 1)->width;
					}
					else
					{
						cellR->hpRGB = basPRGB;
						cellR->htRGB = 232;
						cellR->width = 80;
					}
					cellR->size = 0;
					//cellR->headN = 1;
					cellR->type = IO_NUM;
					cellR->iok = EIO_OUTPUT;
					cellR->kind = CK_NORMAL;
					strcpy_s(cellR->name, sizeof(cellR->name), getInitSymbol(fN, _T("Col")));
					cellR->alignC = AL_RIGHT;
					cellR->properties |= PR_VISIBLE;
				}
			}
			if (m_formItem->GetForm(fN)->kind == FM_GRIDEX || m_formItem->GetForm(fN)->kind == FM_GRID )
			{
				while(atoi(data) < cellItem->GetCount())
					cellItem->RemoveLastCell(atoi(data));
			}
			repR->cols = atoi(data);
			delete cellItem;
			checkGridMultiLine(fN);
		}
		break;
	case eSTR2:	// str2
		m_formItem->SetFormStr(fN, _T("str2"), data);
		break;
	case eSTRGB:	// stRGB
		m_formItem->GetForm(fN)->stRGB = atoi(data);
		break;
	case eSPRGB:	// spRGB
		m_formItem->GetForm(fN)->spRGB = atoi(data);
		break;
	case eAlpha:	// alpha
		m_formItem->GetForm(fN)->alpha = atoi(data);
		break;
	case eSelectShape:
		m_formItem->GetForm(fN)->selectShape = atoi(data);
		break;
	case eOptionImg:	// Option Image Path
		m_formItem->SetFormStr(fN, _T("optionimg"), data);
		break;
	case eOptionImg2:	// option image path2
		m_formItem->SetFormStr(fN, _T("optionimg2"), data);
		break;
	case eOptionImg3:	// option image path3yy
		m_formItem->SetFormStr(fN, _T("optionimg3"), data);
		break;
	case eOptionPos:	// Option Image Position
		m_formItem->GetForm(fN)->optionPos = atoi(data);
		break;
	case eOptionRGB:
		m_formItem->GetForm(fN)->opRGB = atoi(data);
		break;
	case eBlinkShape:
		m_formItem->GetForm(fN)->blinkShape = atoi(data);
		break;
	case eSelectFontRGB:
		m_formItem->GetForm(fN)->sfpRGB = atoi(data);
		break;
	// Layout
	case eLayoutLeft:
		{
			struct _Layout* layout;
			CString sName = m_formItem->GetCurrLayoutName();
			m_formItem->GetForm(fN)->layout->Lookup(sName, (void*&)layout);
			layout->left = atoi(data);
			m_formItem->GetForm(fN)->layout->SetAt(sName, (void*&)layout);

			m_formItem->GetForm(fN)->right += atoi(data) - m_formItem->GetForm(fN)->left;
			m_formItem->GetForm(fN)->left = atoi(data);
			index = -1;
		}
		break;
	case eLayoutTop:
		{
			struct _Layout* layout;
			CString sName = m_formItem->GetCurrLayoutName();
			m_formItem->GetForm(fN)->layout->Lookup(sName, (void*&)layout);
			layout->top = atoi(data);
			m_formItem->GetForm(fN)->layout->SetAt(sName, (void*&)layout);

			m_formItem->GetForm(fN)->bottom += atoi(data) - m_formItem->GetForm(fN)->top;
			m_formItem->GetForm(fN)->top = atoi(data);
			index = -1;
		}
		break;
	case eLayoutWidth:
		{
			struct _Layout* layout;
			CString sName = m_formItem->GetCurrLayoutName();
			m_formItem->GetForm(fN)->layout->Lookup(sName, (void*&)layout);
			layout->right = layout->left + atoi(data);
			m_formItem->GetForm(fN)->layout->SetAt(sName, (void*&)layout);

			m_formItem->GetForm(fN)->right = atoi(data) + m_formItem->GetForm(fN)->left;
			index = -1;
		}
		break;
	case eLayoutHeight:
		{
			struct _Layout* layout;
			CString sName = m_formItem->GetCurrLayoutName();
			m_formItem->GetForm(fN)->layout->Lookup(sName, (void*&)layout);
			layout->bottom = layout->top + atoi(data);
			m_formItem->GetForm(fN)->layout->SetAt(sName, (void*&)layout);

			m_formItem->GetForm(fN)->bottom = atoi(data) + m_formItem->GetForm(fN)->top;
			index = -1;
		}
		break;
	case eLayoutAvailable:
		{
			struct _Layout* layout;
			CString sName = m_formItem->GetCurrLayoutName();
			m_formItem->GetForm(fN)->layout->Lookup(sName, (void*&)layout);
			layout->properties = 0;
			if (atoi(data))
				layout->properties = PR_VISIBLE;			
			m_formItem->GetForm(fN)->layout->SetAt(sName, (void*&)layout);
		}
		break;

	case eMargin0:
	case eMargin1:
	case eMargin2:
	case eMargin3:
		{
			struct _formR* formR = m_formItem->GetForm(fN);
			if (formR->margin == NULL)
			{
				formR->margin = new CStringArray;
				formR->margin->RemoveAll();
			}
			if (id-eMargin0 > formR->margin->GetUpperBound())
			{
				for (int ii = formR->margin->GetUpperBound(); ii <= id - eMargin0; ii++)
					formR->margin->Add(_T(""));
			}
			formR->margin->SetAt(id-eMargin0, data);
		}
		break;

	case eTABHEAD:	// tabHead
		{
			if (m_formItem->GetForm(fN)->checked >= m_formItem->GetForm(fN)->keys)
				break;

			CString	tmps, str, retv;

			tmps = retv = _T("");
			str = m_formItem->GetFormStr(fN, _T("dat"));
			for (int ii = 0; ii < m_formItem->GetForm(fN)->checked; ii++)
			{
				tmps = parse(str, _T("\t"));
				retv += tmps;
				retv += _T("\t");
			}
			retv += data;
			retv += _T("\t");
			parse(str, _T("\t"));
			retv += str;
			m_formItem->SetFormStr(fN, _T("dat"), retv);
		}
		break;
	case eTABDATA:	// tabData
		{
			if (m_formItem->GetForm(fN)->checked >= m_formItem->GetForm(fN)->keys)
				break;

			CString	tmps, str, retv;

			tmps = retv = _T("");
			str = m_formItem->GetFormStr(fN, _T("str"));
			for (int ii = 0; ii < m_formItem->GetForm(fN)->checked; ii++)
			{
				tmps = parse(str, _T("\t"));
				retv += tmps;
				retv += _T("\t");
			}
			tmps = parse(str, _T("\t"));
			if (tmps.GetLength() && (tmps.GetAt(0) == _T('0') ||  tmps.GetAt(0) == _T('1')))
			{
				retv += tmps.GetAt(0);
				retv += _T(':');
				retv += data;
				retv += _T(':');
				parse(tmps, _T(":"));
				parse(tmps, _T(":"));
				retv += tmps;
				retv += _T('\t');
			}
			retv += str;
			m_formItem->SetFormStr(fN, _T("str"), retv);
		}
		break;
	case eTABVISIBLE:	// tabVisible
		{
			if (m_formItem->GetForm(fN)->checked >= m_formItem->GetForm(fN)->keys)
				break;

			CString	tmps, str, retv;

			tmps = retv = _T("");
			str = m_formItem->GetFormStr(fN, _T("str"));
			for (int ii = 0; ii < m_formItem->GetForm(fN)->checked; ii++)
			{
				tmps = parse(str, _T("\t"));
				retv += tmps;
				retv += _T("\t");
			}
			tmps = parse(str, _T("\t"));
			parse(tmps, _T(":"));
			if (data.GetAt(0) == _T('0'))
				retv += _T("0:");
			else
				retv += _T("1:");
			retv += tmps;
			retv += _T("\t");
			
			retv += str;
			m_formItem->SetFormStr(fN, _T("str"), retv);
		}
		break;
	case eTABLEHEAD:	// tableHead
		{
			m_formItem->GetForm(fN)->attr2 &= ~(GO_MULTI | GO_XHEAD);
			m_formItem->GetForm(fN)->iok = EIO_NOP;
			switch (atoi(data))
			{
			case 0:
				m_formItem->GetForm(fN)->attr2 |= GO_MULTI;
				m_formItem->GetForm(fN)->iok = EIO_OUTPUT;
				break;
			case 1:	break;
			case 2:
				m_formItem->GetForm(fN)->attr2 |= GO_XHEAD;
				m_formItem->GetForm(fN)->iok = EIO_OUTPUT;
				break;
			}
		}
		break;
	case eEDITS:	// editS
		if (!data.IsEmpty())
		{
			data.TrimLeft();
			data.TrimRight();
		}
		m_formItem->SetFormStr(fN, _T("editS"), data);
		break;
	case eALIGNIMG:	// alignImg
		m_formItem->GetForm(fN)->alignImage = atoi(data);
		break;
	case eFIXSIZE:	// fixSize
		break;
	case eDESC:	// desc
		m_formItem->SetFormStr(fN, _T("desc"), data);
		break;
	case eFIXEDROW:	//53:	
		m_formItem->GetForm(fN)->fixrow = atoi(data);
		break;
	case eFIXEDCOL:
		m_formItem->GetForm(fN)->fixcol = atoi(data);
		break;
	case eHEADH:
		m_formItem->GetForm(fN)->headH = atoi(data);
		break;
	case eROWSOF:	// rowsof
		m_formItem->GetForm(fN)->rowof = atoi(data);
		break;
	//case 57:case 58:case 59:case 60:case 61:case 62:case 63:case 64:case 65:case 66:
	case eFORMOB_01:
	case eFORMOB_02:
	case eFORMOB_03:
	case eFORMOB_04:
	case eFORMOB_05:
	case eFORMOB_06:
	case eFORMOB_07:
	case eFORMOB_08:
	case eFORMOB_09:
	case eFORMOB_10:
	case eFORMOB_11:
	case eFORMOB_12:
	case eFORMOB_13:
	case eFORMOB_14:
	case eFORMOB_15:
	case eFORMOB_16:
	case eFORMOB_17:
	case eFORMOB_18:
	case eFORMOB_19:
	case eFORMOB_20:
	case eFORMOB_21:
	case eFORMOB_22:
	case eFORMOB_23:
	case eFORMOB_24:
	case eFORMOB_25:
	case eFORMOB_26:
	case eFORMOB_27:
	case eFORMOB_28:
	case eFORMOB_29:
	case eFORMOB_30:
	case eFORMOB_31:
	case eFORMOB_32:
	case eFORMOB_33:
	case eFORMOB_34:
	case eFORMOB_35:		
		{
			CString stmp = _T("");
			CString	tmp, str, rtval;

			tmp.Format(_T("%02d"), id);
			str.Format(_T("%s=%s:"), prop->m_head, data);
			AddCMapOb(tmp, str);

			m_formItem->SetFormStr(fN, _T("str2"), AddCMapOb(_T("0")));
		}
		break;
	case eRTS:	// rts
		m_formItem->SetFormStr(fN, _T("rts"), data);
		break;
	case eALLCELLWIDTH:	// allcellwidth
		{
			struct _repR* repR = (struct _repR *)m_formItem->GetForm(fN)->auxR;
			CCellItem* cellItem = new CCellItem(repR);
			for (int ii = 0; ii < cellItem->GetCount(); ii++)
				cellItem->GetCell(ii)->width = atoi(data);
			delete cellItem;
		}
		break;
	}
		
	struct _repR* repR   = (fN >= 0 ? (struct _repR *)m_formItem->GetForm(fN)->auxR : NULL);
	if (repR != NULL && (repR->selected >= 0 || col >= 0))
	{
		if (col < 0)
			col = repR->selected;
		if (col == -1)
			return;

		CCellItem* cellItem = new CCellItem(repR);
		struct _cellR* cellR = cellItem->GetCell(col);
		switch (id)
		{
		case eCELLNAME:	// cellname
			strcpy_s(cellR->name, sizeof(cellR->name), data);
			break;
		case eCELLTYPE:	// celltype
			cellR->type = atoi(data);
			if (cellR->type != IO_NUM)
				cellR->attr &= ~FA_COMMA;

			break;
		case eCELLIOK:	// celliok
			if (atoi(data) == EIO_OUTPUT && cellR->kind == CK_CHECK)
			{
				cellR->iok = EIO_INOUT;
			//	LoadData(m_index, m_InputPnt.x, m_InputPnt.y);
			//	UpdateCombo();
			}
			else if ((atoi(data) == EIO_OUTPUT || atoi(data) == EIO_INOUT || atoi(data) == EIO_INPUT) && cellR->kind == CK_BUTTON)
			{
				cellR->iok = EIO_NOP;
			//	LoadData(m_index, m_InputPnt.x, m_InputPnt.y);
			//	UpdateCombo();
			}
			else
				cellR->iok = atoi(data);
			break;
		case eCELLKIND:	// cellkind
			if (cellR->iok == EIO_OUTPUT && atoi(data) == CK_CHECK)
			{
				cellR->kind = CK_NORMAL;
			//	LoadData(m_index, m_InputPnt.x, m_InputPnt.y);
			//	UpdateCombo();
			}
			else if ((cellR->iok == EIO_OUTPUT || cellR->iok == EIO_INOUT || cellR->iok == EIO_INPUT) && atoi(data) == CK_BUTTON)
			{
				cellR->kind = CK_NORMAL;
			//	LoadData(m_index, m_InputPnt.x, m_InputPnt.y);
			//	UpdateCombo();
			}
			else
				cellR->kind = atoi(data);
			break;
		case eCELLSIZE:	// cellsize
			cellR->size = atoi(data);
			break;
		case eCELLWIDTH:	// cellwidth
			cellR->width = atoi(data);
			break;
		case eCELLHEAD:	// cellhead
			strcpy_s(cellR->head, sizeof(cellR->head), data);
			break;
		case eCELLALIGNH:	// cellalignH
			cellR->alignH = atoi(data);
			break;
		case eCELLALIGNC:	// cellalignC
			cellR->alignC = atoi(data);
			break;
		case eCELLHPRGB:	// cellhpRGB
			cellR->hpRGB = atoi(data);
			break;
		case eCELLHTRGB:	// cellhtRGB
			cellR->htRGB = atoi(data);
			break;
		case eCELLPRGB:	// cellpRGB
			cellR->pRGB = atoi(data);
			break;
		case eCELLTRGB:	// celltRGB
			cellR->tRGB = atoi(data);
			break;
		case eCELLVALS:	// cellvals
			cellR->vals = atoi(data);
			break;
		case eCELLSTR:	// cellstr
			strcpy_s(cellR->str, sizeof(cellR->str), data);
			break;
		case eCELLAUXS:	// cellauxs
			strcpy_s(cellR->auxS, sizeof(cellR->auxS), data);
			break;
		case eCELLREFS:	// cellrefs
			break;
		case eCELLPROPERTIES:	// cellproperties
			{
				CString	str;
				int	nCnt = 1;

				str = prop->m_val2;
				data = parse(data, _T("-"));
				while(!str.IsEmpty())
				{
					CString str2 = parse(str, _T(";"));
					if (nCnt == atoi(data))
						cellR->properties |= hex2Int(str2);
					else
						cellR->properties &= ~hex2Int(str2);
					nCnt++;
				}
			}
			break;
		case eCELLATTR:	// cellattr
			if (data.GetAt(0) == _T('0'))
				cellR->attr &= ~hex2Int(prop->m_val2);
			else
				cellR->attr |= hex2Int(prop->m_val2);
			break;
		case eCELLSTR2:	// cellstr2
			strcpy_s(cellR->str2, sizeof(cellR->str2), data);
			break;
		case eCELLDAT:	// celldat
			strcpy_s(cellR->dat, sizeof(cellR->dat), data);
			break;
		case eCELLKEYS:	// cellkeys
			cellR->keys = atoi(data);
			break;
		case eCELLVALS2:	// cellvals2
			cellR->vals2 = atoi(data);
			break;
		case eCELLALIGNI:	// cellalignI
			break;
		case eCELLSTR3:	// cellstr3
			strcpy_s(cellR->str3, sizeof(cellR->str3), data);
			break;
		case eCELLTXT:	// celltxt
			strcpy_s(cellR->txt, sizeof(cellR->txt), data);
			break;
		case eCELLISHINT:	// cellisHint
			break;
		case eCELLONHINT:	// cellonHint
			break;
		case eCELLATTR2:	// cellattr2
			if (data.GetAt(0) == _T('0'))
				cellR->attr2 &= ~hex2Int(prop->m_val2);
			else
				cellR->attr2 |= hex2Int(prop->m_val2);
			break;
		case eCELLRTS:	// cellrts
			strcpy_s(cellR->rtssy, sizeof(cellR->rtssy), data);
			break;
		case eCELLEDITS:
			strcpy_s(cellR->editS, sizeof(cellR->editS), data);
			break;
		case eCELLFONTNAME:
			strcpy_s(cellR->fName, sizeof(cellR->fName), data);
			break;
		case eCELLFONTSIZE:
			cellR->fPoint = atoi(data);			
			break;
		case eCELLFONTSTYLE:
			cellR->fStyle = atoi(data);
			break;
		case eCELLMERGENAME:
			strcpy_s(cellR->merge, sizeof(cellR->merge), data);
			break;
		case eCELLHEADIMG:
			strcpy_s(cellR->str4, sizeof(cellR->str4), data);
			break;
		case eCELLHEIGHT:
			cellR->height = atoi(data);
			break;
		case eCELLHEADH:
			cellR->headH = atoi(data);
			break;
		case eCELLCHARTLIMIT:
			cellR->chartLimit = atoi(data);
			break;
		case eCellMargin0:
		case eCellMargin1:
		case eCellMargin2:
		case eCellMargin3:
			if (cellR->margin == NULL)
			{
				cellR->margin = new CStringArray;
				cellR->margin->RemoveAll();
			}
			if (id-eCellMargin0 > cellR->margin->GetUpperBound())
			{
				for (int ii = cellR->margin->GetUpperBound(); ii <= id - eCellMargin0; ii++)
					cellR->margin->Add(_T(""));
			}
			cellR->margin->SetAt(id-eCellMargin0, data);
			break;
		}
		delete cellItem;
	}

	switch (id) // form
	{
	case eMAPCAPTION:	// mapcaption
		strcpy_s(m_mapH->caption, sizeof(m_mapH->caption), data);
		if (m_mapH->mapK == MK_NORM)
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETCAPTION, 0);
		break;
	case eMAPHEIGHT:	// mapheight
		m_mapH->height = atoi(data);
		index = -1;
		break;
	case eMAPWIDTH:	// mapwidth
		m_mapH->width = atoi(data);
		index = -1;
		break;
	case eMAPFNAME:	// mapfname
		strcpy_s(m_mapH->fName, sizeof(m_mapH->fName), data);
		break;
	case eMAPFPOINT:	// mapfpoint
		m_mapH->fPoint = atoi(data);
		break;
	case eMAPFSTYLE:	// mapfstyle
		m_mapH->fStyle = atoi(data);
		break;
	case eMAPVERS:	// mapvers
	//	m_mapH->vers = atoi(data);
		break;
	case eMAPNAME:	// mapname
		strcpy_s(m_mapH->mapN, sizeof(m_mapH->mapN), data);
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETMAPNAME);
		break;
	case eMAPDESC:	// mapdesc
		strcpy_s(m_mapH->mapdesc, sizeof(m_mapH->mapdesc), data);
		break;
	case eMAPTRXC:	// maptrxC
		strcpy_s(m_mapH->trxC, sizeof(m_mapH->trxC), data);
		break;
	case eMAPTRXH:	// maptrxH
		break;
	case eMAPTOPATH:	// maptoPath
		m_mapH->toPath = atoi(data);
		break;
	case eMAPMAPM:	// mapmapM
		m_mapH->mapM = atoi(data);
		break;
	case eMAPHELPN:	// maphelpN
		break;
	case eMAPREPEATV:	// maprepeatV
		m_mapH->repeatV = atoi(data);
		break;
	case eMAPWAITV:	// mapwaitV
		break;
	case eMAPIMAGEK:	// mapimageK
		m_mapH->bgK = 1;//atoi(data);//ljh
		break;
	case eMAPIMAGEV:	// mapimageV
		strcpy_s(m_mapH->bgFileN, sizeof(m_mapH->bgFileN), data);
		break;
	case eMAPGROUP:	// mapGroup
	//	m_mapH->mapGroup = atoi(data);
		break;
	case eFORMTYPE:	// formtype
		m_mapH->formtype = atoi(data);
		break;
	case eREPEATV:	// repeatV
		m_mapH->repeatV = atoi(data);
		break;
	case ePUBLICR:	// publicR
		break;
	case eMAPBKCLR:	// mapBkClr
		m_mapH->bgClr = atoi(data);
		break;
	case eMAPALPHA:
		m_mapH->alpha = atoi(data);
		break;
	case eFORMCTRL:
		strcpy_s(m_mapH->formCtrl, sizeof(m_mapH->formCtrl), data);
		break;
	}
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REDRAWCTRL, index);
}


void CPropertiesGrid::ChangeColor(CString sData, bool bflag)
{
	Cproperty*	prop = nullptr;
	CPropertyGridEditProperty* pEdit = nullptr;
	CMFCPropertyGridProperty* pProp  = nullptr;
	int	pos;
	CString tmps, text = _T("");

	for (int ii = 0; ii < GetPropertyCount(); ii++)
	{
		pProp = GetProperty(ii);
		prop = (Cproperty *)pProp->GetData();
		if (prop->m_keys.CompareNoCase(_T("str")) == 0)
		{
			pEdit = (CPropertyGridEditProperty*)pProp;
				break;
		}
		pProp->GetData();
	}

	if (pEdit == nullptr)
		return;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	
	if (!bflag || sData.IsEmpty())
	{
		pEdit->SetColor(false);
		pEdit->Redraw();
		return;
	}

	if (pFrame->IsLanguage(sData))
		pEdit->SetColor(false);
	else 
		pEdit->SetColor(true);

	if (pEdit)
		pEdit->Redraw();
}


CString CPropertiesGrid::AddCMapOb(CString key, CString data)
{
	CString str, tmp = _T("");

	switch (atoi(key))
	{
	case -1:
		m_mapFOB.RemoveAll();
		break;
	case 0:
		{
			CString tmps;
			for (int ii = eFORMOB_01; ii < eFORMOB_35; ii++)
			{
				tmps.Format(_T("%02d"), ii);
				if (m_mapFOB.Lookup(tmps, str))
					tmp += str;
			}
		}
		break;
	default:
		m_mapFOB.SetAt(key, data);
		break;
	}

	return tmp;
}

CString CPropertiesGrid::getInitSymbol(int fN, CString syms)
{
	CString tmps, slist;
	int	initN;
	struct _repR* repR = (struct _repR *)m_formItem->GetForm(fN)->auxR;

	if (!repR)
		return syms;

	CCellItem cellItem(repR);

	for (int kk = 0; kk < cellItem.GetCount(); kk++)
	{
		if (cellItem.GetCell(kk)->name[0] != NULL)
		{
			slist += cellItem.GetCell(kk)->name;
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
	CString tmp, idx;

	if (str.GetAt(0) == _T('$'))	// reserved items
	{
		CString field = "00$bool\t01$fstyle\t02$alignment\t03$hint\t04$fixSize\t05$control\t";

		pos = field.Find(str);
		if (pos == -1)
			return 0;

		switch (atoi(field.Mid(pos - 2, 2)))
		{
		case 0:		// $bool
			str = _T("0-False;1-True;");
			break;
		case 1:		// $fstyle
			str = _T("0-None;1-Normal;2-Italic;3-Bold;4-Bold/Italic;");
			break;
		case 2:		// $alignment
			str = _T("0-Center;1-Left;2-Right;");
			break;
		case 3:		// $hint
			str = _T("0-None;1-String;2-Reference;");
			break;
		case 4:
			str = _T("0-Automatic;1-Fixed;");
			break;
		case 5:
			str = _T("0-None;") + loadFile();
			if (m_control.Lookup(data, tmp))
			{
				idx = parse(tmp, _T(";"));
				data.Format(_T("%d"), atoi(idx));
			}
			else
				data = _T("0");
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

CString CPropertiesGrid::loadFile()
{
	CString path, desc;
	CString tmps, text, sym, tmp, idx, retval, str, han;
	char	wb[1024];
	DWORD	value;

	path.Format(_T("%s\\%s\\control.ini"), (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0), TABDIR);
	for (int ii = 1; ii < CUSTOMCOUNT; ii++)
	{
		tmps.Format(_T("%02d"), ii);
		idx = tmps;

		GetPrivateProfileString(_T("Custom Objects"), tmps, _T(""), wb, sizeof(wb), path);
		desc = wb;
		tmps = parse(desc, _T(";"));
		han  = parse(desc, _T(";"));

		if (tmps.IsEmpty())
			break;

		tmp.Format("%d-%s;", atoi(idx), han);
		retval += tmp;
		str.Format(_T("%s;%s"), idx, tmps);
		m_control.SetAt(han, str);

		value = GetPrivateProfileSection(tmps, wb, sizeof(wb), path);
	}
	return retval;
}

CString CPropertiesGrid::getComboData(CString items, CString data)
{
	if (data.GetLength() <= 0)
		return _T("");

	bool	bcustom = false;
	int	pos;
	if (items.GetAt(0) == _T('$'))	// reserved items
	{
		CString field = "00$bool\t01$fstyle\t02$alignment\t03$hint\t04$fixSize\t05$control\t";

		pos = field.Find(items);
		if (pos == -1)
			return 0;

		switch (atoi(field.Mid(pos - 2, 2)))
		{
		case 0:		// $bool
			items = _T("0-False;1-True;");
			break;
		case 1:		// $fstyle
			items = _T("0-None;1-Normal;2-Italic;3-Bold;4-Bold/Italic;");
			break;
		case 2:		// $alignment
			items = _T("0-Center;1-Left;2-Right;");
			break;
		case 3:		// $hint
			items = _T("0-None;1-String;2-Reference;");
			break;
		case 4:
			items = _T("0-Automatic;1-Fixed;");
			break;
		case 5:
			items = _T("0-None;") + loadFile();
			bcustom = true;
			break;
		}
	}

	items.Insert(0, _T(';'));
	data += _T(';');

	if (bcustom)
	{
		data.Replace(_T(";"), _T(""));
		return data;
	}

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
	struct	_formR*	formR = m_formItem->GetForm(fN);

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