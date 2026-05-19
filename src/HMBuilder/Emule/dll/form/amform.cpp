// axform.cpp : implementation file
//

#include "stdafx.h"
#include "amform.h"
//#include "../lib/axislib.h"
#include "../amLib/AmDraw.h"

//#include "fmAvi.h"
#include "amBox.h"
#include "amBrowser.h"
#include "amButton.h"
#include "amCheck.h"
#include "amCombo.h"
#include "amCtrl.h"
#include "amEdit.h"
#include "amEditEx.h"
#include "amGrid.h"
#include "amGroup.h"
#include "amLabel.h"
//#include "amMemo.h"
#include "amObject.h"
#include "amOut.h"
#include "amPanel.h"
#include "amRadio.h"
//#include "fmSheet.h"
//#include "fmSysm.h"
//#include "amTab.h"
#include "amTable.h"
//#include "fmTreeView.h"
#include "amUserTab.h"
#include "amInfo.h"


using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	N_CODE			6		// 5 -> 6

/////////////////////////////////////////////////////////////////////////////
// CAmForm

char*	LCcontrast[] = {"  ", "¡é", "¡å", "¡ã", "¡è"};

void SAFEDELETEFORM(struct _formR* t)
{
	SAFE_DELETEA(t->name);
	SAFE_DELETEA(t->fName);
	SAFE_DELETEA(t->desc);
	SAFE_DELETEA(t->auxS);
	SAFE_DELETEA(t->scpN);
	SAFE_DELETEA(t->editS);
	SAFE_DELETEA(t->str);
	SAFE_DELETEA(t->str2);
	SAFE_DELETEA(t->dat);
	SAFE_DELETEA(t->rtssy);
//	SAFE_DELETEA(t->optionimg);
//	SAFE_DELETEA(t->refS);
//	SAFE_DELETEA(t->formR->help);
//	SAFE_DELETEA(t->optionimg2);
//	SAFE_DELETEA(t->optionimg3);
	SAFE_DELETE(t);		
}

void SAFEDELETECELL(struct _cellR* t)
{
	SAFE_DELETE(t);
}


IMPLEMENT_DYNCREATE(CAmForm, CCmdTarget)

CAmForm::CAmForm()
{
	EnableAutomation();

	m_mapB = (char *) 0;
	m_mapL = 0;

	m_avi    = -1;
	m_rect.SetRectEmpty();
	m_hRatio = 1.0;
	m_vRatio = 1.0;
	m_cursor = CPoint(0, 0);

	m_mapH    = (struct _mapH  *) 0;
//	m_pageR   = (struct _pageR *) 0;
	m_valueR  = (DWORD *) 0;
	m_scriptR = (char  *) 0;
	m_strR    = (char  *) 0;

	m_drawOnly = false;
	m_tabs.RemoveAll();

	m_hCode = AfxLoadLibrary("AxisCodx.dll");
	if (m_hCode == NULL)
	{
		axGetCategory = NULL;
		axGetSpinData = NULL;
		axGetDelta    = NULL;
	}
	else
	{
		axGetCategory = (bool   (APIENTRY*)(char*, int, int, char*, bool))GetProcAddress(m_hCode, _T("axGetCategory")); 
		axGetSpinData = (int    (APIENTRY*)(DWORD, int, int, char*, double, bool))GetProcAddress(m_hCode, _T("axGetSpinData"));
		axGetDelta    = (double	(APIENTRY*)(DWORD, double, bool))GetProcAddress(m_hCode, _T("axGetDelta"));
	}

	m_LCcontrast.SetSize(5);
	m_LCtype = PRIMARYLANGID(GetSystemDefaultLangID());
	switch (m_LCtype)
	{
	case LANG_CHINESE:
		m_LCcontrast[ct_uLimit]  = (TCHAR)0xa1;
		m_LCcontrast[ct_uLimit] += (TCHAR)0xb6;
		m_LCcontrast[ct_Rise]    = (TCHAR)0xa1;
		m_LCcontrast[ct_Rise]   += (TCHAR)0xb6;
		m_LCcontrast[ct_Fall]    = (TCHAR)0xa1;
		m_LCcontrast[ct_Fall]   += (TCHAR)0xbf;
		m_LCcontrast[ct_lLimit]  = (TCHAR)0xa1;
		m_LCcontrast[ct_lLimit] += (TCHAR)0xbf;
		m_LCcontrast.SetAt(ct_Flat, _T("  "));
		break;
	default:
		m_LCcontrast.SetAt(ct_uLimit, LCcontrast[ct_uLimit]);
		m_LCcontrast.SetAt(ct_Rise, LCcontrast[ct_Rise]);
		m_LCcontrast.SetAt(ct_Fall, LCcontrast[ct_Fall]);
		m_LCcontrast.SetAt(ct_lLimit, LCcontrast[ct_lLimit]);
		m_LCcontrast.SetAt(ct_Flat, LCcontrast[ct_Flat]);
		break;
	}
}

CAmForm::~CAmForm()
{
	if (m_fmObs.GetSize())
		removeForm();

	if (m_hCode)
		AfxFreeLibrary(m_hCode);

	m_tabs.RemoveAll();

	SAFE_DELETEA(m_mapB)
	SAFE_DELETE(m_mapH);
	for_each(m_vFORM.begin(), m_vFORM.end(), SAFEDELETEFORM);	
	for each( std::pair<string, char*> t in m_mSCRIPT )
		SAFE_DELETEA(t.second);						  
	for each( std::pair<string, vector <struct _cellR*>*> p in m_MTABLE)
	{
		for_each(p.second->begin(), p.second->end(), SAFEDELETECELL);
		SAFE_DELETE(p.second);
	}
	
	for each( std::pair<string, vector <struct _cellR*>*> p in m_MGRID)
	{
		for_each(p.second->begin(), p.second->end(), SAFEDELETECELL);
		SAFE_DELETE(p.second);
	}

	m_vFORM.clear();
}


void CAmForm::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CAmForm, CCmdTarget)
	//{{AFX_MSG_MAP(CAmForm)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CAmForm, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CAmForm)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IAmform to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {1AED3C4B-5A8A-48F3-821D-B0E0C6C82D3F}
static const IID IID_IAmform =
{ 0x1aed3c4b, 0x5a8a, 0x48f3, { 0x82, 0x1d, 0xb0, 0xe0, 0xc6, 0xc8, 0x2d, 0x3f } };

BEGIN_INTERFACE_MAP(CAmForm, CCmdTarget)
	INTERFACE_PART(CAmForm, IID_IAmform, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAmForm message handlers

bool CAmForm::LoadMAP(CString mapN)
{
	TRIM(mapN);

	CString ss;
	CString sRoot;

	CString sData;

	sRoot.Format("%s\\exe\\workshop.ini", m_root);
	GetPrivateProfileString("Environment", "Emul_Path", "", ss.GetBuffer(256), 256, sRoot); 
	ss.ReleaseBuffer();
	int nPos = ss.ReverseFind('\\');
	if (nPos > 0)
		ss += "\\";
	
	CString sFile = FORMAT("%s\\Form\\%s", ss, mapN);

/*
#ifdef _last
	CString sFile = FORMAT("%sexe\\tmp\\%s", m_root, mapN);
#else
	CString sFile = mapN;
#endif
*/

	HANDLE hFile = CreateFile(sFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	DWORD dwMapL = GetFileSize(hFile, NULL);
/*
	if (dwMapL == 0xFFFFFFFF || dwMapL < L_MAPH)
	{
		CloseHandle(hFile);
		return false;
	}
*/
	if (dwMapL > m_mapL)
	{
		if (m_mapB != NULL)
			delete[] m_mapB;
		m_mapL = dwMapL+2;
		m_mapB = new char[m_mapL];
	}

	DWORD dwReadLen = 0;
	BOOL  bRc = ReadFile(hFile, m_mapB, dwMapL, &dwReadLen, NULL);
	sData = UTF8ToAnsi(m_mapB);
	sData = sData.Left(sData.ReverseFind(mCLASS)+1);
	char* pch = (LPSTR)(LPCSTR)sData;
	CloseHandle(hFile);
	if (!bRc || dwMapL != dwReadLen)
		return false;
		
	char* pData = {""};
	//***************************************************************************************
	vector<string> sCLASS; 
	vector<string> sSCRIPT;
	vector<string> sITEM;
	CString sMsg = FORMAT(//"TRINFO%cPIBOPOOP%cCODE=1301%cDATE=1302%cNAME=1022%cGUBN=1376%cCURR=2023%cDIFF=2024%cDAILYGRID.DATE=11310.11034%cDAILYGRID.CURR=11310.2023%cDAILYGRID.DIFF=11310.2024%cDAILYGRID.VOL=11310.2027%c\
						  // TRINFO%cPIBO0199%cA=CODE%cK=DATE%cDAILYGRID.B=DAILYGRID.NAME%cDAILYGRID.C=DAILYGRID.CURR%c",
					      "TRINFO%cPTBO0101%cA=I1301%cB=NEXTG%cC=NAME%cD=RCOD%cE=CURR%c",
						  mPRO, mPRO, mPRO, mSCRIPT, mPRO, mPRO, mCLASS);
						  //TRINFO%cPIBO0198%cA=CODE%cB=NAME%cC=CURR%cD=DIFF%c",
						  /*11310.11034=DAILYGRID.DATE%c11310.2023=DAILYGRID.CURR%c11310.2024=DAILYGRID.DIFF%c11310.2027=DAILYGRID.VOL%c\*/
						  //mPRO, mPRO, mPRO, mSCRIPT, mPRO, mPRO, mPRO, mPRO, mPRO, mPRO, mPRO, mCLASS,
						  //mPRO, mPRO, mSCRIPT, mPRO, mPRO, mCLASS);
	

	//11301.1301=DAILYGRID.DATE%c
	CString cClass  = FORMAT("%c", mCLASS);
	CString cScript = FORMAT("%c", mSCRIPT);
	CString cItem   = FORMAT("%c", mITEM);
	CString cGubn   = FORMAT("%c", mPRO);

	string MyString;
	//CString ss = CString(MyString.data(), MyString.length());
	MyString.append(pch, strlen(pch));
	//MyString.append(m_mapB, dwReadLen);	
	
//	MyString.append(sMsg);

int nFind = 0;
//	int nCount = Split(sCLASS, MyString, mCLASS);
	int nCount = 
	boost::regex_split(std::back_inserter(sCLASS), MyString, boost::regex(cClass.GetBuffer(0)));
	for each (string s in  sCLASS)
	{
		nFind = s.find(mPRO, 0);
		if (s.empty() || s.compare(" ") < 0 || nFind < 0)
			break;

		int n = 0; 

		vector<string> v;		
//		split(v, s, is_any_of(""), token_compress_off);
		Split(v, s, '');
		trim(v[0]);

		// layout
		if (v[0].compare("LAYOUT") == 0)
			continue;

		if (v[0].compare("RTSINFO") == 0)
			continue;

		if (v[0].compare("TRINFO") != 0)
		{
			n = boost::regex_split(std::back_inserter(sSCRIPT), s, boost::regex(cScript.GetBuffer(0))); 
		//	n = Split(sSCRIPT, s, mSCRIPT);
			if (n > 1)
			{
				/// ¼­ºê¾ÆÀÌÅÛ 
				n = boost::regex_split(std::back_inserter(sITEM), sSCRIPT[0], boost::regex(cItem.GetBuffer(0))); 
				//n = Split(sITEM, sSCRIPT[0], mITEM);
				{
					sSCRIPT[0] = sITEM[0];	
					sITEM.erase(sITEM.begin());
				}	

				DoInfo( CSTRING(sSCRIPT[0]), &sSCRIPT,  n >= 1 ? &sITEM : NULL );
				sITEM.clear();
				sSCRIPT.clear();
			}
			else
			{
				n = boost::regex_split(std::back_inserter(sITEM), sSCRIPT[0], boost::regex(cItem.GetBuffer(0))); 
				//n = Split(sITEM, sSCRIPT[0], mITEM);
				sSCRIPT[0] = sITEM[0];	
				sITEM.erase(sITEM.begin());

				DoInfo( CSTRING(sSCRIPT[0]), &sSCRIPT,  n >= 1 ? &sITEM : NULL );
				sITEM.clear();
				sSCRIPT.clear();
			}
		}
		else 
		{
			//LOADTRLAYOUT
			LoadtrLayout(v[1].data(), CSTRING(s));
			v.clear();
		}
		//**********************************************************************************************
	}
	// Emulator Resolution
	int resolution = GetPrivateProfileInt(_T("Environment"), _T("Emul_Resolution"), 100, sRoot); 
	m_mapH->resolution = resolution;

	m_mapH->formN = m_vFORM.size();
	makeString();

	return true;
}

void CAmForm::LoadForm(CRect mRect, bool drawOnly)
{
	CString	path = _T("");
	m_drawOnly = drawOnly;
	if (m_fmObs.GetSize())
		removeForm();

	m_point = m_mapH->fPoint;
	m_fonts = CString(m_mapH->fName);
	
	vector<struct _formR*>::iterator it; 
	struct _formR* pform;
	vector<CamBase*> vGrid;


	int ii = 0;
	for(it = m_vFORM.begin();it != m_vFORM.end(); it++)
	{
		ii++;

		pform = *it;
		switch ((*it)->kind)
		{
		case FM_LABEL:
			CamLabel* fmLabel;
			fmLabel = new CamLabel(this, (*it));//&m_formR[ii]);
			ASSERT_VALID(fmLabel);
			m_fmObs.Add(fmLabel);
			break;
		
		case FM_BOX:
			CamBox* amBox;
			amBox = new CamBox(this, (*it)); ///&m_formR[ii]);
			ASSERT_VALID(amBox);
			m_fmObs.Add(amBox);
			break;

		case FM_GROUP:
			CamGroup* fmGroup;
			fmGroup = new CamGroup(this, (*it));//&m_formR[ii]);
			ASSERT_VALID(fmGroup);
			m_fmObs.Add(fmGroup);
			break;

		case FM_IMAGEVW:
			CamPanel* fmPanel;
			fmPanel = new CamPanel(this,(*it));// &m_formR[ii]);
			ASSERT_VALID(fmPanel);
			m_fmObs.Add(fmPanel);
			break;

		case FM_BUTTON:	
			CAmButton* amButton;
			amButton = new CAmButton(this, (*it)); //&m_formR[ii]);
			ASSERT_VALID(amButton);
			m_fmObs.Add(amButton);		
			break;
		case FM_RADIO:
			CamRadio* amRadio;
			amRadio = new CamRadio(this, (*it)); ////&m_formR[ii]);
			ASSERT_VALID(amRadio);
			m_fmObs.Add(amRadio);
			SetRadioGroup(amRadio->GetSymbolName(), (*it)->properties & PR_CHECKED ? true : false, ii);
			break;
		case FM_CHECK:
			CamCheck* amCheck;
			amCheck = new CamCheck(this, (*it)); //..&m_formR[ii]);
			ASSERT_VALID(amCheck);
			m_fmObs.Add(amCheck);
			break;

		case FM_EDIT:
			switch ((*it)->type)
			{
			case IO_NUM:
			case IO_DIGIT:
				if (!( (*it)->attr & FA_COMMA))
				{
//					switch (m_mapH->typeH)
//					{
//					case TH_4702:
//					case TH_KOSCOM:
//					case TH_SCUP:
//						if (m_formR[ii].properties & PR_VISIBLE || m_formR[ii].properties & PR_ENABLE)
//							m_formR[ii].attr |= FA_ZERO;
//						break;
//					}
				}
				break;

			case IO_MIXED:
				if (!(m_mapH->options & OP_CNV))
					 (*it)->type = IO_ANY;
				break;

			default:
				break;
			}

			if ( (*it)->type == IO_MIXED)
			{
				CamEditEx* amEditEx;
				amEditEx = new CamEditEx(this, (*it)); //&m_formR[ii]);
				ASSERT_VALID(amEditEx);
				m_fmObs.Add(amEditEx);
				break;
			}

			CamEdit* amEdit;
			amEdit = new CamEdit(this,  (*it), m_drawOnly);
			ASSERT_VALID(amEdit);
			m_fmObs.Add(amEdit);
			break;

		case FM_COMBO:
			CamCombo* fmCombo;
			fmCombo = new CamCombo(this, (*it));//&m_formR[ii]);
			ASSERT_VALID(fmCombo);
			m_fmObs.Add(fmCombo);
			break;

		case FM_OUT:
			CamOut* fmOut;
			fmOut = new CamOut(this, (*it)); //&m_formR[ii]);
			ASSERT_VALID(fmOut);
			m_fmObs.Add(fmOut);
			break;
		case FM_GRID:
		case FM_GRIDEX:
			CamGrid* fmGrid;

			//if ( (*it)->vals[0] != NOVALUE)

	//		if (m_vCELL.size() > 0)
			if (m_MGRID[string((*it)->name)]->size() > 0)
				fmGrid = new CamGrid(this, (*it)  /*&m_formR[ii]*/, /*(struct _cellR*)&m_cellR[(*it)->vals[0]]*/m_MGRID[string((*it)->name)]);
			else
				fmGrid = new CamGrid(this, (*it)); ////(&m_formR[ii]);
			ASSERT_VALID(fmGrid);
			m_fmObs.Add(fmGrid);
			break;

		case FM_TABLE:
			CamTable* fmTable;
			//if ((*it)->vals[0] != NOVALUE)
			if (m_MTABLE[string((*it)->name)]->size() > 0)
				fmTable = new CamTable(this, (*it)/*&m_formR[ii]*/, /*(struct _cellR*)&m_cellR[(*it)->vals[0]]*/ m_MTABLE[string((*it)->name)]);
			else
				fmTable = new CamTable(this, (*it)); //&m_formR[ii]);
			ASSERT_VALID(fmTable);
			m_fmObs.Add(fmTable);
			break;
/*
		case FM_TREE:
			CfmTreeView* fmTree;
			fmTree = new CfmTreeView(this, &m_formR[ii]);
			ASSERT_VALID(fmTree);
			m_fmObs.Add(fmTree);
			break;

		case FM_AVI:
			CfmAvi* fmAvi;
			fmAvi = new CfmAvi(this, &m_formR[ii]);
			ASSERT_VALID(fmAvi);
			m_fmObs.Add(fmAvi);
			m_avi = ii;
			break;
*/
		case FM_OBJECT:
			CamObject* fmObject;
			fmObject = new CamObject(this, (*it)); //&m_formR[ii]);
			ASSERT_VALID(fmObject);
			m_fmObs.Add(fmObject);
			break;

		case FM_TAB:
			CamUserTab* fmUserTab;
			fmUserTab = new CamUserTab(this, (*it)); //&m_formR[ii]);
			ASSERT_VALID(fmUserTab);
			m_fmObs.Add(fmUserTab);
			break;

		case FM_BROWSER:
			CamBrowser* amBrowser;
			amBrowser = new CamBrowser(this, (*it)/*&m_formR[ii]*/, m_drawOnly);
			ASSERT_VALID(amBrowser);
			m_fmObs.Add(amBrowser);
			break;

		case FM_CONTROL:
			CamCtrl* fmCtrl;
			fmCtrl = new CamCtrl(this, (*it)/*&m_formR[ii]*/, m_drawOnly);
			ASSERT_VALID(fmCtrl);
			m_fmObs.Add(fmCtrl);
			break;

		default:
			CamBase* fmBase;
			fmBase = new CamBase(this, (*it)/*&m_formR[ii]*/);
			ASSERT_VALID(fmBase);
			m_fmObs.Add(fmBase);
			break;
		}
	}


	CPoint	pt;
	pt.x = mRect.left;
	pt.y = mRect.top;

	float ratio = 1.0;
	float hRatio = 1.0;
	float vRatio = 1.0;

	ratio = (float)m_mapH->resolution / 100.0;
	hRatio = ((float)mRect.Width() / (float)m_mapH->width) * ratio;
	vRatio = ((float)mRect.Height() / (float)m_mapH->height) * ratio;
	
	////////////////////////////////////////////////////
	// Object Å©±âº¯È¯
	/*
	if (hRatio != ratio)
		hRatio *= 100.0 / (float)m_mapH->resolution;
	if (vRatio != ratio)
		vRatio *= 100.0 / (float)m_mapH->resolution;
	*/
	if (hRatio != ratio)
		hRatio = ((float)mRect.Width() / (float)m_mapH->width);
	if (vRatio != ratio)
		vRatio = ((float)mRect.Height() / (float)m_mapH->height);

	m_rect = mRect;
	for (int ii = 0; ii < m_fmObs.GetSize(); ii++)
	{
		CamBase* form = (CamBase *)m_fmObs.GetAt(ii);
		ASSERT_VALID(form);
		form->Adjust(pt, hRatio, vRatio);
	}
}

CamBase* CAmForm::GetAtForm(int key)
{
	if (key < 0 || key > m_fmObs.GetUpperBound())
		return (CamBase *) NULL;

	CamBase* form = (CamBase *)m_fmObs.GetAt(key);
	ASSERT_VALID(form);
	return	form;
}

bool CAmForm::FindForm(CString symbol, int& key)
{
	for (key = 0; key < m_mapH->formN; key++)
	{
		CamBase* form = (CamBase *)m_fmObs.GetAt(key);
		ASSERT_VALID(form);
		if (!symbol.CompareNoCase(form->GetSymbolName()))
			return true;
	}

	key = -1;
	return false;
}

bool CAmForm::FindForm(CString symbol, CamBase*& form)
{
	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		form = (CamBase *)m_fmObs.GetAt(ii);
		ASSERT_VALID(form);
		if (!symbol.CompareNoCase(form->GetSymbolName()))
			return true;
	}
	
	form = (CamBase *)NULL;
	return false;
}


int CAmForm::FindForm(CamBase* form)
{
	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		if (form == (CamBase *)m_fmObs.GetAt(ii))
			return ii;
	}
	return -1;
}

void CAmForm::SetFont(bool resize, int fpoint, int fstyle, CString fname)
{
	if (!resize && m_mapH->options & OP_XFONT)
		return;

	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		CamBase* form = (CamBase *)m_fmObs.GetAt(ii);
		ASSERT_VALID(form);
		form->SetFont(fpoint, fstyle, fname, resize);
	}
}

void CAmForm::DrawForm(CDC* dc)
{
	CRect	fmRc;
//	CamTab*	tab;
	CamBase* form;

	if (dc == NULL)
	{
		for (int ii = 0; ii < m_fmObs.GetSize(); ii++)
		{
			form = (CamBase *)m_fmObs.GetAt(ii);
			ASSERT_VALID(form);
			form->ShowControl(FALSE);
		}
		return;
	}

	for (int ii = 0; ii < m_fmObs.GetSize(); ii++)
	{
		/*
		if (m_tabs.Lookup(ii, (CObject*&)tab))
		{
			if (!tab->IsFormVisible(ii))
			{
				form = (CamBase *)m_fmObs.GetAt(ii);
				ASSERT_VALID(form);
				form->ShowControl(FALSE);
				continue;
			}
		}
		*/

		form = (CamBase *)m_fmObs.GetAt(ii);
		ASSERT_VALID(form);
		fmRc = form->GetRect();
		if (dc->RectVisible(&fmRc))
		{
			form->Draw(dc);
			form->ShowControl(form->m_form->properties & PR_VISIBLE);
		}
	}
}

void CAmForm::ClearForm(int type)
{
	for (int ii = 0; ii < m_fmObs.GetSize(); ii++)
	{
		
	//	switch (m_formR[ii].kind)
		switch (m_vFORM[ii]->kind)
		{
		case FM_OUT:
		case FM_TABLE:
			if (type == CLR_INPUT)
				continue;
			break;
		case FM_GRID:
		case FM_GRIDEX:
		case FM_EDIT:
		case FM_COMBO:
		//case FM_MEMO:
			switch (type)
			{
			case CLR_INPUT:
				if (m_vFORM[ii]->iok == EIO_OUTPUT)
					continue;
				break;
			case CLR_OUTPUT:
				if (m_vFORM[ii]->iok == EIO_INPUT)
					continue;
			case CLR_ALL:
			default:
				break;
			}
			break;
		default:
			continue;
		}

		CamBase* form = (CamBase *)m_fmObs.GetAt(ii);
		ASSERT_VALID(form);
		form->ClearData();
	}
}

void CAmForm::ResizeForm(float hRatio, float vRatio, bool redraw)
{
	CPoint cp;
	
	m_hRatio = hRatio;
	m_vRatio = vRatio;
	
	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		CamBase* form = (CamBase *)m_fmObs.GetAt(ii);
		ASSERT_VALID(form);
		form->Resize(hRatio, vRatio, true, redraw);
	}
}

void CAmForm::AdjustForm(CSize size, bool move)
{
	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		CamBase* form = (CamBase *)m_fmObs.GetAt(ii);
		ASSERT_VALID(form);
		if (move)
			form->Adjust(size, 1.0, 1.0);
		else
			form->Adjust(size);
	}
}

void CAmForm::OffsetForm(CSize offs)
{
	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		CamBase* form = (CamBase *)m_fmObs.GetAt(ii);
		ASSERT_VALID(form);
		form->Offset(offs);
	}
}

void CAmForm::DominoForm(bool all)
{
	int	rgb;//, auxS;
//	DWORD	offs;
	CString symbol;
	CamBase *form,  *domino;

	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
//		switch (m_formR[ii].kind)
		switch (m_vFORM[ii]->kind)
		{
		case FM_EDIT:
		case FM_OUT:
			if (!(m_vFORM[ii]->attr & FA_DOMINO))
				continue;

			symbol = m_vFORM[ii]->desc;
			if (symbol.IsEmpty())
				continue;

			form = (CamBase *)m_fmObs.GetAt(ii);
			ASSERT_VALID(form);

			//auxS = m_vFORM[ii]->desc;
			FindForm(symbol, domino);
//			domino = (CamBase *)m_fmObs.GetAt(auxS);
			ASSERT_VALID(domino);
			form->m_itRGB = domino->m_itRGB;
			rgb = domino->GetFgColor();

//			rgb = _ARGB(255, 255, 0, 0);
			form->SetFgColor(rgb);
			form->SetFgColor(form->m_itRGB);		
			
			break;

		case FM_GRID:
		case FM_GRIDEX:
		case FM_TABLE:
			if (!all)
				continue;
			//offs = m_vFORM[ii]->vals[0];
			//if (offs == NOVALUE)
			//	break;
			if ( m_vFORM[ii]->row == 0 || m_vFORM[ii]->col == 0)
				break;

			form = (CamBase *)m_fmObs.GetAt(ii);
			ASSERT_VALID(form);
			form->SetDomino();
			break;
		default:
			continue;
		}
	}
}

void CAmForm::ReloadForm(CRect mRect)
{
	CPoint	pt;

	pt.x = mRect.left;
	pt.y = mRect.top;

	//float hRatio = (float)mRect.Width() / (float)m_mapH->width;
	//float vRatio = (float)mRect.Height() / (float)m_mapH->height;
	float hRatio = (float)m_mapH->resolution / 100.0;
	float vRatio = (float)m_mapH->resolution / 100.0;

	m_rect = mRect;
	for (int ii = 0; ii < m_fmObs.GetSize(); ii++)
	{
		CamBase* form = (CamBase *)m_fmObs.GetAt(ii);
		ASSERT_VALID(form);
		form->Reload();
		form->Adjust(pt, hRatio, vRatio);
	}
	m_view->InvalidateRect(&m_rect, FALSE);
}

void CAmForm::ResetSystemForm()
{
	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
	//	if (m_formR[ii].kind != FM_SYS)
	//		continue;		

		CamBase* form = (CamBase *)m_fmObs.GetAt(ii);
		ASSERT_VALID(form);
		form->WriteData(_T(""));
	}
}

void CAmForm::SetRadioGroup(CString symbol, bool checked, int n)
{
	int	key;
	if (!FindForm(symbol, key))
		return;

	int	index;
	CString ss = CString(m_vFORM[key]->auxS);

	if (ss.IsEmpty())
		return;

	if (m_radio.Lookup(m_vFORM[key]->auxS, index))
	{
		CamBase* form = GetAtForm(key);	
		if (index != -1)
		{
			((CamRadio *)form)->SetChecked(false);
			return;
		}
		if (!checked)
			return;
	}

	index = (m_vFORM[key]->properties & PR_CHECKED) ? key : -1;
	m_radio.SetAt(m_vFORM[key]->auxS, index);
}

CString CAmForm::GetRadioGroupName(CString symbol)
{
	int	key;
	if (!FindForm(symbol, key))
		return "";

	return CString(m_vFORM[key]->auxS);
}

void CAmForm::UnCheckRadioGroup(CString symbol)
{
	int	key;
	if (!FindForm(symbol, key))
		return;
	
	int	index;
	if (m_radio.Lookup(m_vFORM[key]->auxS, index))
	{
		if (index != -1 && key != index)
		{
			CamBase* form = GetAtForm(index);
			((CamRadio *)form)->SetChecked(false);
		}			
		m_radio.SetAt(m_vFORM[key]->auxS, key);
	}
}

void CAmForm::SetTabPage(int index, CamBase* tab)
{
	ASSERT_VALID(tab);
	
	if (index >= 0 && index < m_mapH->formN)
		m_tabs.SetAt(index, tab);
}

bool CAmForm::IsFormVisible(CamBase* form)
{
	int	key = FindForm(form);
	if (key < 0)
		return false;
/*
	CamTab*	tab;
	if (m_tabs.Lookup(key, (CObject*&)tab))
	{
		if (!tab->IsFormVisible(key))
			return false;
	}
*/
	return true;
}

bool CAmForm::PlayAVI(bool replay)
{
	return false;
/*
	if (m_avi < 0)
		return false;

	CfmAvi*	fmAvi = (CfmAvi *)GetAtForm(m_avi);
	if (replay)
		return fmAvi->Replay();
	return fmAvi->Play();
*/
}

void CAmForm::StopAVI()
{
	/*
	if (m_avi < 0)
		return;

	CfmAvi* fmAvi = (CfmAvi *)GetAtForm(m_avi);
	fmAvi->Stop();
	*/
}

bool CAmForm::GetCode(int kind, int index, CString& code, bool prev)
{
	if (axGetCategory)
	{
		char	wb[128];

		strcpy_s(wb, sizeof(wb), code);
		if ((*axGetCategory)((char *)m_root.operator LPCTSTR(), kind, index, wb, prev))
		{
			code = wb;
			return true;
		}
	}

	return false;
}

int CAmForm::GetSpinData(DWORD type, int nfloat, int xfloat, CString& data, double value, bool prev)
{
	if (axGetSpinData)
	{
		char	wb[128];

		strcpy_s(wb, sizeof(wb), data);
		xfloat = (*axGetSpinData)(type, nfloat, xfloat, wb, value, prev);
		data = wb;
		return xfloat;
	}

	return 0;
}

double CAmForm::GetDelta(DWORD type, double value, bool prev)
{
	if (axGetDelta)
		return (*axGetDelta)(type, value, prev);

	return 0;
}

bool CAmForm::IsContrast(int& value)
{
	switch (m_LCtype)
	{
	default:
	case LANG_KOREAN:
		switch (value)
		{
		case 0xa1e3:				// TriUp
		case 0xa1e8:				// Up
		case 0xa3ab:				// '+'
			value = '+';
			return true;
		case 0xa1e5:				// TriDown
		case 0xa1e9:				// Down
		case 0xa3ad:				// '-'
			value = '-';
			return true;
		}
		break;
	case LANG_CHINESE:
		switch (value)
		{
		case 0xa1b6:				// TriUp
		case 0xa1cf:				// '+'
			value = '+';
			return true;
		case 0xa1bf:				// TriDown
		case 0xa1d0:				// '-'
			value = '-';
			return true;
		}
		break;
	}
	return false;
}

int CAmForm::GetContrast(int key, CString& text)
{
	switch (key)
	{
	case '1':
		text = m_LCcontrast[ct_uLimit];
		return ct_uLimit;
	case '+':
	case '2':
		text = m_LCcontrast[ct_Rise];
		return ct_Rise;
	case '4':
		text = m_LCcontrast[ct_lLimit];
		return ct_lLimit;
	case '-':
	case '5':
		text = m_LCcontrast[ct_Fall];
		return ct_Fall;
	}
	text  = _T("  ");
	return ct_Flat;
}

CString CAmForm::TrimSign(CString& data)
{
	int	idx;
	CString	signs;

	signs.Empty();

	idx = 0;
	switch (data.FindOneOf("+-"))
	{
	case -1:
		if (data.Find(m_LCcontrast[ct_uLimit]) && data.Find(m_LCcontrast[ct_lLimit]) &&
				data.Find(m_LCcontrast[ct_Rise]) && data.Find(m_LCcontrast[ct_Fall]))
			break;
		signs += data.GetAt(idx++);
	case 0:
		signs += data.GetAt(idx++);
		data = data.Mid(idx);
		break;
	}
	return signs;
}

int CAmForm::GetDouble(int value)
{
	if (value < 0x80)
	{
		switch (m_LCtype)
		{
		default:
		case LANG_KOREAN:
			if (value == ' ')
				return 0xa1a1;
			return (value + 0x80) + 0xa300;
		case LANG_CHINESE:
			break;
		}
	}
	return value;
}

CString CAmForm::GetString(DWORD offset)
{
	if (offset != NOVALUE)
		return (char *)&m_strR[offset];
	return _T("");
}

void CAmForm::makeString()
{

}

void CAmForm::removeForm()
{
	for (int ii = 0; ii < m_fmObs.GetSize(); ii++)
	{
		CamBase* form = (CamBase *)m_fmObs.GetAt(ii);
		ASSERT_VALID(form);
		delete	form;
	}
	m_fmObs.RemoveAll();
}

void CAmForm::getExternalScript(CString& scripts, CString& pubs)
{
	CString	text, str;
	CString	tmps, tmpx;
	int	pos;

	pubs = _T("");
	text = _T("AX_SUB_include_AX_");
	if (!getRoutine(text, scripts, true))
		return;

	pos = text.Find('\n');
	if (pos == -1)
		return;

	text = text.Mid(pos+1);
	for ( ; !text.IsEmpty(); )
	{
		pos = text.Find('\n');
		if (pos != -1)
		{
			tmps = text.Left(pos);
			text = text.Mid(pos+1);
		}
		else
		{
			tmps = text;
			text = _T("");
		}

		str  = _T("#LOAD ");
		tmpx = tmps;
		tmpx.MakeUpper();
		pos = tmpx.Find(str);
		if (pos != -1 && !isComment(tmps.Left(pos)))
		{
			tmps = tmps.Mid(pos+str.GetLength());
			tmps.TrimLeft();
			tmps.TrimRight();
			pos = tmps.Find('.');
			if (pos != -1)
			{
				str  = tmps.Mid(pos+1);
				tmps = tmps.Left(pos);

				tmps = getScripts(tmps, pubs);
				if (!tmps.IsEmpty())
				{
					if (getRoutine(str, tmps))
						scripts += str;
				}
			}
			else
				scripts += getScripts(tmps, pubs);
			scripts += '\n';
		}
	}

	if (findOption(scripts))
	{
		while (findOption(scripts))
			;
		text = _T("Option Explicit\n");
		scripts.Insert(0, text);
	}
}

CString CAmForm::getScripts(CString name, CString& pubs)
{
	return _T("");
}

bool CAmForm::getRoutine(CString& str, CString& src, bool remove)
{
	CString	text, tmps, string;
	int	pos, index, sum;
	bool	func;

	sum    = 0;
	string = src;
	for ( ; true; )
	{
		pos = string.Find(str);
		if (pos == -1)
			return false;

		text  = string.Mid(pos);
		index = text.FindOneOf("( ");
		if (index == -1)
		{
			string = string.Mid(pos+str.GetLength());
			continue;
		}

		sum += pos;
		text = text.Left(index);
		if (str.CompareNoCase(text))
		{
			string = string.Mid(pos+text.GetLength());
			continue;
		}

	//	if (isComment(string.Left(text.GetLength())))
	//	{
	//		string = string.Mid(text.GetLength());
	//		continue;
	//	}

		text   = string.Left(pos);
		string = string.Mid(pos+str.GetLength());
		pos  = text.ReverseFind('\n');
		if (pos != -1)
			text = text.Mid(pos+1);
		sum -= text.GetLength();
		text.TrimRight();

		if (isComment(text))
			continue;

		pos = text.ReverseFind(' ');
		if (pos != -1)
			text = text.Mid(pos+1);

		if (!text.CompareNoCase("sub"))
		{
			func = false;
			break;
		}
		else if (!text.CompareNoCase("function"))
		{
			func = true;
			break;
		}
	}

	sum  = max(0, sum);
	text = (sum == 0) ? src : src.Mid(sum);
	tmps = func ? _T("End Function") : _T("End Sub");
	pos  = text.Find(tmps);
	if (pos == -1)
		return false;

	str = text.Left(pos+tmps.GetLength()+1);		// get routine OK!!
	if (remove)
		src.Delete(sum, str.GetLength());
	return true;
}

bool CAmForm::findOption(CString& scripts)
{
	CString	text, str;
	CString	tmps, tmpx;
	int	pos, spos;

	text = _T("OPTION ");
	str  = scripts;
	str.MakeUpper();

	spos = str.Find(text);
	if (spos == -1)
		return false;

	str = str.Mid(spos+text.GetLength());
	pos = str.Find('\n');
	if (pos != -1)
		tmps = str.Left(pos);
	else
	{
		tmps = str;
		pos  = str.GetLength();
	}

	tmps.TrimLeft();
	tmps.TrimRight();
	if (!tmps.CompareNoCase("explicit"))
	{
		scripts.Delete(spos, text.GetLength()+pos);
		return true;
	}

	return false;
}

bool CAmForm::isComment(CString str)
{
	if (str.IsEmpty())
		return false;

	int	pos;
	CString	tmps;

	pos = str.ReverseFind('\n');
	if (pos != -1)
		str = str.Mid(pos+1);
	str.TrimLeft();
	str.TrimRight();
	if (!str.IsEmpty())
	{
		pos = str.Find('\'');
		if (pos != -1)
			return true;
	}

	return false;
}


bool CAmForm::DoInfo(CString sControl, vector<string>* pSCRIPT, vector<string>* pSUB)
{
	sControl.Replace(12, 0x08);
	if (sControl.IsEmpty())
		return false;

	char *pMapString[] = {	
			    "axForm",
				"axLabel",
				"axBox",
				"AXGROUP",
				"axImageView",
				"axButton",
				"axCheck",
				"axRadio",
				"axEdit",
				"axCombo",
				"axOutput",
				//"AXMEMO",	
				"axGrid",
				"axGridEx",
				"axTable",
				"axObject",
				"axTab",
				"axWebView",
				"axControl",
				"TRINFO",
				"LAYOUT",
		};

	
	map<string, int> sMAP;
	string scontrol;
	scontrol.append(sControl);
	
	int ii = 0;
	for (ii = 0 ; ii < _countof(pMapString); ii++)
		sMAP[pMapString[ii]] = ii;

	vector<string> sCLASS;
	CString cPRO = FORMAT("%c", mCLASS);
	string str = " ";

//	int nCount = /// Object count
//		boost::regex_split(std::back_inserter(sCLASS), scontrol, boost::regex(cPRO.GetBuffer(0))); 

	int nCount = Split(sCLASS, scontrol, mCLASS);
	struct _formR* pform = NULL;

	algorithm::trim(sCLASS[0]);

	switch (sMAP[sCLASS[0]])
	{
	case 0:
		{
			CiMap *pMap = new CiMap(&sCLASS, pSCRIPT, pSUB, &m_mSCRIPT);
			m_mapH = pMap->GetMapH();
			SAFE_DELETE(pMap);
			return true;
		}
		break;
	case FM_LABEL:		
		{
			CiLabel *pLable = new CiLabel(&sCLASS, pSCRIPT, pSUB, &m_mSCRIPT);
			pform = pLable->GetForm();
			SAFE_DELETE(pLable);
		}
		break;
	case FM_BOX:
		{
			CiBox *pBox = new CiBox(&sCLASS, pSCRIPT, pSUB, &m_mSCRIPT);
			pform = pBox->GetForm();
			SAFE_DELETE(pBox);
		}
		break;
	case FM_GROUP:
		{
			CiGroup *pGroup = new CiGroup(&sCLASS, pSCRIPT, pSUB, &m_mSCRIPT);
			pform = pGroup->GetForm();
			SAFE_DELETE(pGroup);
		}
		break;
	case FM_IMAGEVW:		
		{
			CiImageView *pImageView = new CiImageView(&sCLASS, pSCRIPT, pSUB, &m_mSCRIPT);
			pform = pImageView->GetForm();
			SAFE_DELETE(pImageView);
		}
		break;
	case FM_BUTTON:
		{
			CiButton *pButton = new CiButton(&sCLASS, pSCRIPT, pSUB, &m_mSCRIPT);
			pform = pButton->GetForm();
			SAFE_DELETE(pButton);
		}
		break;
	case FM_CHECK:
		{
			CiCheck *pCheck = new CiCheck(&sCLASS, pSCRIPT, pSUB, &m_mSCRIPT);
			pform = pCheck->GetForm();
			SAFE_DELETE(pCheck);
		}
		break;
	case FM_RADIO:		
		{
			CiRadio *pRadio = new CiRadio(&sCLASS, pSCRIPT, pSUB, &m_mSCRIPT);
			pform = pRadio->GetForm();
			SAFE_DELETE(pRadio);
		}
		break;
	case FM_EDIT:
		{
			CiEdit * pEdit = new CiEdit(&sCLASS, pSCRIPT, pSUB, &m_mSCRIPT);
			pform = pEdit->GetForm();
			SAFE_DELETE(pEdit);
		}
		break;
	case FM_COMBO:
		{
			CiCombo *pCombo = new CiCombo(&sCLASS, pSCRIPT, pSUB, &m_mSCRIPT);
			pform = pCombo->GetForm();
			SAFE_DELETE(pCombo);
		}
		break;
	case FM_OUT:			// output
		{
			CiOut	*pOut = new CiOut(&sCLASS, pSCRIPT, pSUB, &m_mSCRIPT);
			pform = pOut->GetForm();
			SAFE_DELETE(pOut);
		}
		break;
/*
	case FM_MEMO:				// memo
		{
			CiMemo	*pMemo = new CiMemo(&sCLASS, pSCRIPT, pSUB, &m_mSCRIPT);
			pform = pMemo->GetForm();
			SAFE_DELETE(pMemo);
		}
		break;
*/
	case FM_GRID:
	case FM_GRIDEX:				// grid
		{
			int kind = FM_GRID;
			if (sMAP[sCLASS[0]] == FM_GRIDEX)
				kind = FM_GRIDEX;

			vector <struct _cellR*>* pvCell = new vector <struct _cellR*>;
			CiGrid	*pGrid = new CiGrid(&sCLASS, pSCRIPT, pSUB, &m_mSCRIPT, pvCell, kind);
			pform = pGrid->GetForm();

			m_MGRID[string(pform->name)] = pvCell;
			SAFE_DELETE(pGrid);
		}
		break;
	case FM_TABLE:			// table
		{
			vector <struct _cellR*>* pvCell = new vector <struct _cellR*>;
			CiTable	*pTable = new CiTable(&sCLASS, pSCRIPT, pSUB, &m_mSCRIPT, pvCell);
			pform = pTable->GetForm();

			m_MTABLE[string(pform->name)] = pvCell;
			SAFE_DELETE(pTable);
		}
		break;
	case FM_OBJECT:			// unit MAP object
		{
			CiObject *pObject = new CiObject(&sCLASS, pSCRIPT, pSUB, &m_mSCRIPT);
			pform = pObject->GetForm();
			SAFE_DELETE(pObject);
		}
		break;
	case FM_TAB:				// tab
		{
			CiTab	*pTab	= new CiTab(&sCLASS, pSCRIPT, pSUB, &m_mSCRIPT);
			pform = pTab->GetForm();
			SAFE_DELETE(pTab);
		}
		break;
	case FM_BROWSER:			// browser (file name, url, ... + '\0')
		{
			CiBrowser	*pBrowser = new CiBrowser(&sCLASS, pSCRIPT, pSUB, &m_mSCRIPT);
			pform = pBrowser->GetForm();
			SAFE_DELETE(pBrowser);
		}
		break;
	case FM_CONTROL:			// control
		{
			CiControl  *pControl = new CiControl(&sCLASS, pSCRIPT, pSUB, &m_mSCRIPT);
			pform = pControl->GetForm();
			SAFE_DELETE(pControl);
		}
		break;

	default:
		break;
	}

	m_vFORM.push_back(pform);	

	return true;
}



bool CAmForm::LoadtrLayout(CString sPath, CString trinfo)
{
	CString sFile = FORMAT("%s\\exe\\tmp\\%s", m_root, sPath);
	CString sBuffer;
	vector<string>  vBuffer;

	HANDLE hFile = CreateFile(sFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		sFile.Format("%s\\trlayout\\%s", m_root, sPath);
		hFile = CreateFile(sFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return false;
	}

	DWORD dwMapL = GetFileSize(hFile, NULL);

	if (dwMapL == 0xFFFFFFFF || dwMapL < 10)
	{
		CloseHandle(hFile);
		return false;
	}

	DWORD dwReadLen = 0;
	BOOL  bRc = ReadFile(hFile, sBuffer.GetBuffer(dwMapL), dwMapL, &dwReadLen, NULL);
	sBuffer.ReleaseBuffer();
	CloseHandle(hFile);


	m_MTRINFO[string(sPath)]  = sBuffer;
	m_LINKINFO[string(sPath)] = trinfo;

	/*
	m_pvInfo = new vector<string>;
	split(*m_pvInfo, string(sBuffer), is_any_of("\n"), token_compress_on);

	
	int ii = 0;
	for each (string s in (*m_pvInfo))
	{
		split(vBuffer, s, is_any_of("="), token_compress_on);
		if (!vBuffer[1].empty())
			(*m_pvInfo)[ii] = vBuffer[1];

		vBuffer.clear();
		ii++;
	}

	vBuffer.clear();
	CString s = trinfo.Mid(trinfo.Find(sPath) + sPath.GetLength() + 1, trinfo.GetLength());
	m_pvInfo->push_back(string(s));


	string ss = (*m_pvInfo)[0];
	m_MTRINFO[ss] = m_pvInfo;
	
	//m_MTRINFO.SetAt(ss.data(),  m_pvInfo);	

	//pInfo = m_MTRINFO[ss];
	//	pInfo = TrInfo(ss);
	*/
	return true;
}

CString CAmForm::UTF8ToAnsi(char* pszUTF8)
{	
	int nU8StrLen, nBStrLen, nAStrLen;
	BSTR bstr;
	char* pszANSI;
 
	nU8StrLen = lstrlen(pszUTF8);
	nBStrLen = MultiByteToWideChar(CP_UTF8, 0, pszUTF8, nU8StrLen + 1, NULL, NULL);
	bstr = SysAllocStringLen(NULL, nBStrLen);
	MultiByteToWideChar(CP_UTF8, 0, pszUTF8, nU8StrLen + 1, bstr, nBStrLen);
 
	nAStrLen = WideCharToMultiByte(CP_ACP, 0, bstr, -1, NULL, 0, NULL, NULL);
	pszANSI = new char[nAStrLen];
	WideCharToMultiByte(CP_ACP, 0, bstr, -1, pszANSI, nAStrLen, NULL, NULL);
	SysFreeString(bstr);
 
	delete [] pszANSI;

	return (CString)bstr;
}