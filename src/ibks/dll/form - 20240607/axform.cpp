// axform.cpp : implementation file
//

#include "stdafx.h"
#include "axform.h"
#include "../lib/axislib.h"

#include "fmAvi.h"
#include "fmBox.h"
#include "fmBrowser.h"
#include "fmButton.h"
#include "fmCheck.h"
#include "fmCombo.h"
#include "fmCtrl.h"
#include "fmEdit.h"
#include "fmEditEx.h"
#include "fmGrid.h"
#include "fmGroup.h"
#include "fmLabel.h"
#include "fmMemo.h"
#include "fmObject.h"
#include "fmOut.h"
#include "fmPanel.h"
#include "fmRadio.h"
#include "fmSheet.h"
#include "fmSysm.h"
#include "fmTab.h"
#include "fmTable.h"
#include "fmTreeView.h"
#include "fmUserTab.h"
#include <versionhelpers.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	N_CODE			6		// 5 -> 6

/////////////////////////////////////////////////////////////////////////////
// CAxisForm

char*	LCcontrast[] = {"  ", "¡é", "¡å", "¡ã", "¡è"};

IMPLEMENT_DYNCREATE(CAxisForm, CCmdTarget)

CAxisForm::CAxisForm()
{
	EnableAutomation();

	m_mapB = (char *) 0;
	m_mapL = 0;

	m_avi    = -1;
	m_point = 0;
	m_rect.SetRectEmpty();
	m_hRatio = 1.0;
	m_vRatio = 1.0;
	m_cursor = CPoint(0, 0);

	m_mapH    = (struct _mapH  *) 0;
	m_formR   = (struct _formR *) 0;
	m_cellR   = (struct _cellR *) 0;
	m_pageR   = (struct _pageR *) 0;
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

	m_LCtype = PRIMARYLANGID(GetSystemDefaultLangID());
	switch (m_LCtype)
	{
	case LANG_CHINESE:
		LCcontrast[ct_uLimit][0] = (unsigned char)0xa1;
		LCcontrast[ct_uLimit][1] = (unsigned char)0xb6;
		LCcontrast[ct_Rise][0]   = (unsigned char)0xa1;
		LCcontrast[ct_Rise][1]   = (unsigned char)0xb6;
		LCcontrast[ct_Fall][0]   = (unsigned char)0xa1;
		LCcontrast[ct_Fall][1]   = (unsigned char)0xbf;
		LCcontrast[ct_lLimit][0] = (unsigned char)0xa1;
		LCcontrast[ct_lLimit][1] = (unsigned char)0xbf;
		break;
	default:
		break;
	}

	m_oldVers = false;
	OSVERSIONINFO osvi;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
//	if (GetVersionEx(&osvi))
//	{
//		if (osvi.dwPlatformId == VER_PLATFORM_WIN32s || osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
//			m_oldVers = true;
//	}
	if (!IsWindows8OrGreater())
		m_oldVers = true;
}

CAxisForm::~CAxisForm()
{
	if (m_fmObs.GetSize())
	{
		m_n2Obs.RemoveAll();
		removeForm();
	}

	if (m_mapB)
		delete[] m_mapB;

	m_tabs.RemoveAll();
	if (m_hCode)
		AfxFreeLibrary(m_hCode);
}


void CAxisForm::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CAxisForm, CCmdTarget)
	//{{AFX_MSG_MAP(CAxisForm)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CAxisForm, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CAxisForm)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IAxisForm to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {1AED3C4B-5A8A-48F3-821D-B0E0C6C82D3F}
static const IID IID_IAxisForm =
{ 0x1aed3c4b, 0x5a8a, 0x48f3, { 0x82, 0x1d, 0xb0, 0xe0, 0xc6, 0xc8, 0x2d, 0x3f } };

BEGIN_INTERFACE_MAP(CAxisForm, CCmdTarget)
	INTERFACE_PART(CAxisForm, IID_IAxisForm, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxisForm message handlers

bool CAxisForm::LoadMAP(CString mapN)
{
	HANDLE hFile = CreateFile(mapN, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	DWORD dwMapL = GetFileSize(hFile, NULL);
	if (dwMapL == 0xFFFFFFFF || dwMapL < L_MAPH)
	{
		CloseHandle(hFile);
		return false;
	}

	char* mapB = new char[dwMapL + 2];
	DWORD dwReadLen = 0;
	BOOL  bRc = ReadFile(hFile, mapB, dwMapL, &dwReadLen, NULL);
	CloseHandle(hFile);
	if (!bRc || dwMapL != dwReadLen)
	{
		delete[] mapB;
		return false;
	}
	if (m_mapB != NULL)
		delete[] m_mapB;

	if (!strncmp(&mapB[iAXIS], sKEY, iKEY))
	{
		m_mapB = new char[dwMapL * 4];
		m_mapL = DoSomething(doDECOMPRESS, NULL, (WPARAM)&mapB[iAXIS + iKEY], (LPARAM)m_mapB, (int)(dwMapL - iAXIS - iKEY), dwMapL * 4);
		if (m_mapL <= 0 || m_mapL < L_MAPH)
		{
			delete[] mapB;
			return false;
		}
		CopyMemory(m_mapB, mapB, iAXIS);
		delete[] mapB;
	}
	else
	{
		m_mapB = mapB;
		m_mapL = dwMapL + 2;
	}

	m_mapH = (struct _mapH *)m_mapB;
	if (m_mapH->formR != NOVALUE)
		m_formR = (struct _formR *)&m_mapB[m_mapH->formR];

	if (m_mapH->cellR < dwMapL || m_mapH->cellR != NOVALUE)
		m_cellR = (struct _cellR *)&m_mapB[m_mapH->cellR];

	if (m_mapH->pageR < dwMapL || m_mapH->pageR != NOVALUE)
		m_pageR = (struct _pageR *)&m_mapB[m_mapH->pageR];

	if (m_mapH->valueR < dwMapL || m_mapH->valueR != NOVALUE)
		m_valueR = (DWORD *)&m_mapB[m_mapH->valueR];

	if (m_mapH->scriptR < dwMapL || m_mapH->scriptR != NOVALUE)
		m_scriptR = (char *)&m_mapB[m_mapH->scriptR];

	if (m_mapH->strR < dwMapL || m_mapH->strR != NOVALUE)
		m_strR = (char *)&m_mapB[m_mapH->strR];

	if (GetSystemMetrics(SM_CXSCREEN) < 1024)
	{
		if (m_mapH->fStyle == FS_BOLD || m_mapH->fStyle == FS_IBOLD)
			m_mapH->fStyle = FS_NORM;

		for (int ii = 0; ii < m_mapH->formN; ii++)
		{
			if (!(m_formR[ii].fStyle == FS_BOLD || m_formR[ii].fStyle == FS_IBOLD))
				continue;

			m_formR[ii].fStyle = FS_NORM;
		}
	}

	makeString();
	return true;
}

void CAxisForm::LoadForm(CRect mRect, bool drawOnly)
{
	CString	path = _T("");

	m_drawOnly = drawOnly;
	if (m_fmObs.GetSize())
	{
		m_n2Obs.RemoveAll();
		removeForm();
	}

	m_point = m_mapH->fPoint;
	m_fonts = (char *)m_mapH->fName;
	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		if (m_formR[ii].attr & FA_COMMA)
			m_formR[ii].attr |= FA_RIGHT;

		switch (m_formR[ii].kind)
		{
		case FM_SYS:
			CfmSysm* fmSysm;
			fmSysm = new CfmSysm(this, &m_formR[ii], ii);
			ASSERT_VALID(fmSysm);
			m_fmObs.Add(fmSysm);
			addForm((char *)m_formR[ii].name, fmSysm);
			break;

		case FM_LABEL:
			CfmLabel* fmLabel;
			fmLabel = new CfmLabel(this, &m_formR[ii], ii);
			ASSERT_VALID(fmLabel);
			m_fmObs.Add(fmLabel);
			addForm((char *)m_formR[ii].name, fmLabel);
			break;

		case FM_BOX:
			CfmBox* fmBox;
			fmBox = new CfmBox(this, &m_formR[ii], ii);
			ASSERT_VALID(fmBox);
			m_fmObs.Add(fmBox);
			addForm((char *)m_formR[ii].name, fmBox);
			break;

		case FM_GROUP:
			CfmGroup* fmGroup;
			fmGroup = new CfmGroup(this, &m_formR[ii], ii);
			ASSERT_VALID(fmGroup);
			m_fmObs.Add(fmGroup);
			addForm((char *)m_formR[ii].name, fmGroup);
			break;

		case FM_PANEL:
			CfmPanel* fmPanel;
			fmPanel = new CfmPanel(this, &m_formR[ii], ii);
			ASSERT_VALID(fmPanel);
			m_fmObs.Add(fmPanel);
			addForm((char *)m_formR[ii].name, fmPanel);
			break;

		case FM_BUTTON:
			switch(m_formR[ii].type)
			{	
			case BTN_RADIO:
				CfmRadio* fmRadio;
				fmRadio = new CfmRadio(this, &m_formR[ii], ii);
				ASSERT_VALID(fmRadio);
				m_fmObs.Add(fmRadio);
				addForm((char *)m_formR[ii].name, fmRadio);
				SetRadioGroup((char *)m_formR[ii].name, m_formR[ii].properties & PR_CHECKED ? true : false);
				break;

			case BTN_CHECK:
				CfmCheck* fmCheck;
				fmCheck = new CfmCheck(this, &m_formR[ii], ii);
				ASSERT_VALID(fmCheck);
				m_fmObs.Add(fmCheck);
				addForm((char *)m_formR[ii].name, fmCheck);
				break;

			case BTN_PUSH:
			case BTN_USER:
			default:
				CfmButton* fmButton;
				fmButton = new CfmButton(this, &m_formR[ii], ii);
				ASSERT_VALID(fmButton);
				m_fmObs.Add(fmButton);
				addForm((char *)m_formR[ii].name, fmButton);
				break;
			}
			break;

		case FM_EDIT:
			switch (m_formR[ii].type)
			{
			case IO_NUM:
			case IO_DIGIT:
				if (!(m_formR[ii].attr & FA_COMMA))
				{
					switch (m_mapH->typeH)
					{
					case TH_4702:
					case TH_KOSCOM:
					case TH_SCUP:
						if (m_formR[ii].properties & PR_VISIBLE || m_formR[ii].properties & PR_ENABLE)
							m_formR[ii].attr |= FA_ZERO;
						break;
					}
				}
				break;

			case IO_MIXED:
				if (!(m_mapH->options & OP_CNV))
					m_formR[ii].type = IO_ANY;
				break;

			default:
				break;
			}

			if (m_formR[ii].type == IO_MIXED)
			{
				CfmEditEx* fmEditEx;
				fmEditEx = new CfmEditEx(this, &m_formR[ii], ii);
				ASSERT_VALID(fmEditEx);
				m_fmObs.Add(fmEditEx);
				addForm((char *)m_formR[ii].name, fmEditEx);
				break;
			}

			CfmEdit* fmEdit;
			fmEdit = new CfmEdit(this, &m_formR[ii], ii, m_drawOnly);
			ASSERT_VALID(fmEdit);
			m_fmObs.Add(fmEdit);
			addForm((char *)m_formR[ii].name, fmEdit);
			break;

		case FM_COMBO:
			CfmCombo* fmCombo;
			fmCombo = new CfmCombo(this, &m_formR[ii], ii, m_drawOnly);
			ASSERT_VALID(fmCombo);
			m_fmObs.Add(fmCombo);
			addForm((char *)m_formR[ii].name, fmCombo);
			break;

		case FM_OUT:
			CfmOut* fmOut;
			fmOut = new CfmOut(this, &m_formR[ii], ii);
			ASSERT_VALID(fmOut);
			m_fmObs.Add(fmOut);
			addForm((char *)m_formR[ii].name, fmOut);
			break;

		case FM_MEMO:
			CfmMemo* fmMemo;
			fmMemo = new CfmMemo(this, &m_formR[ii], ii, m_drawOnly);
			ASSERT_VALID(fmMemo);
			m_fmObs.Add(fmMemo);
			addForm((char *)m_formR[ii].name, fmMemo);
			break;

		case FM_GRID:
			CfmGrid* fmGrid;
			if (m_formR[ii].vals[0] != NOVALUE)
				fmGrid = new CfmGrid(this, &m_formR[ii], ii, &m_cellR[m_formR[ii].vals[0]]);
			else
				fmGrid = new CfmGrid(this, &m_formR[ii], ii);
			ASSERT_VALID(fmGrid);
			m_fmObs.Add(fmGrid);
			addForm((char *)m_formR[ii].name, fmGrid);
			break;

		case FM_TABLE:
			CfmTable* fmTable;
			if (m_formR[ii].vals[0] != NOVALUE)
				fmTable = new CfmTable(this, &m_formR[ii], ii, &m_cellR[m_formR[ii].vals[0]]);
			else
				fmTable = new CfmTable(this, &m_formR[ii], ii);
			ASSERT_VALID(fmTable);
			m_fmObs.Add(fmTable);
			addForm((char *)m_formR[ii].name, fmTable);
			break;

		case FM_TREE:
			CfmTreeView* fmTree;
			fmTree = new CfmTreeView(this, &m_formR[ii], ii);
			ASSERT_VALID(fmTree);
			m_fmObs.Add(fmTree);
			addForm((char *)m_formR[ii].name, fmTree);
			break;

		case FM_AVI:
			CfmAvi* fmAvi;
			fmAvi = new CfmAvi(this, &m_formR[ii], ii);
			ASSERT_VALID(fmAvi);
			m_fmObs.Add(fmAvi);
			addForm((char *)m_formR[ii].name, fmAvi);
			m_avi = ii;
			break;

		case FM_OBJECT:
			CfmObject* fmObject;
			fmObject = new CfmObject(this, &m_formR[ii], ii);
			ASSERT_VALID(fmObject);
			m_fmObs.Add(fmObject);
			addForm((char *)m_formR[ii].name, fmObject);
			break;

		case FM_TAB:
			CfmTab* fmTab;
			fmTab = new CfmTab(this, &m_formR[ii], ii);
			ASSERT_VALID(fmTab);
			m_fmObs.Add(fmTab);
			addForm((char *)m_formR[ii].name, fmTab);
			break;

		case FM_UTAB:
			CfmUserTab* fmUserTab;
			fmUserTab = new CfmUserTab(this, &m_formR[ii], ii);
			ASSERT_VALID(fmUserTab);
			m_fmObs.Add(fmUserTab);
			addForm((char *)m_formR[ii].name, fmUserTab);
			break;

		case FM_SHEET:
			CfmSheet* fmSheet;
			fmSheet = new CfmSheet(this, &m_formR[ii], ii);
			ASSERT_VALID(fmSheet);
			m_fmObs.Add(fmSheet);
			addForm((char *)m_formR[ii].name, fmSheet);
			break;

		case FM_BROWSER:
			CfmBrowser* fmBrowser;
			fmBrowser = new CfmBrowser(this, &m_formR[ii], ii, m_drawOnly);
			ASSERT_VALID(fmBrowser);
			m_fmObs.Add(fmBrowser);
			addForm((char *)m_formR[ii].name, fmBrowser);
			break;

		case FM_CONTROL:
			CfmCtrl* fmCtrl;
			fmCtrl = new CfmCtrl(this, &m_formR[ii], ii, m_drawOnly);
			ASSERT_VALID(fmCtrl);
			m_fmObs.Add(fmCtrl);
			addForm((char *)m_formR[ii].name, fmCtrl);
			fmCtrl->SetEnum(_T(""), 0, 255);
			break;

		case FM_GRID2:
		default:
			CfmBase* fmBase;
			fmBase = new CfmBase(this, &m_formR[ii], ii);
			ASSERT_VALID(fmBase);
			m_fmObs.Add(fmBase);
			addForm((char *)m_formR[ii].name, fmBase);
			break;
		}
	}

	CPoint	pt;
	pt.x = mRect.left;
	pt.y = mRect.top;

	float hRatio = (float)mRect.Width() / (float)m_mapH->width;
	float vRatio = (float)mRect.Height() / (float)m_mapH->height;
	if (IsResizable())
	{
		vRatio = 1.0;
		hRatio = (float)max(1.0, hRatio);
	}
	m_rect = mRect;
	for (int ii = 0; ii < m_fmObs.GetSize(); ii++)
	{
		CfmBase* form = (CfmBase *)m_fmObs.GetAt(ii);
		ASSERT_VALID(form);
		form->Adjust(pt, hRatio, vRatio);
	}
}

CfmBase* CAxisForm::GetAtForm(int key)
{
	if (key < 0 || key > m_fmObs.GetUpperBound())
		return (CfmBase *) NULL;

	CfmBase* form = (CfmBase *)m_fmObs.GetAt(key);
	ASSERT_VALID(form);
	return form;
}

bool CAxisForm::FindForm(CString symbol, int& key)
{
	CfmBase* form = (CfmBase*) 0;

	symbol.MakeUpper();
	if (m_n2Obs.Lookup(symbol, (CObject *&)form))
	{
		ASSERT_VALID(form);
		key = form->m_iform;
		return true;
	}

	key = -1;
	return false;
}

bool CAxisForm::FindForm(CString symbol, CfmBase*& form)
{
	symbol.MakeUpper();
	if (m_n2Obs.Lookup(symbol, (CObject *&)form))
	{
		ASSERT_VALID(form);
		return true;
	}
	
	form = (CfmBase *)NULL;
	return false;
}

int CAxisForm::FindForm(CfmBase* form)
{
	if (form)
		return form->m_iform;
	return -1;
}

void CAxisForm::SetFont(bool resize, int fpoint, int fstyle, CString fname)
{
	if (!resize && m_mapH->options & OP_XFONT)
		return;

	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		CfmBase* form = (CfmBase *)m_fmObs.GetAt(ii);
		ASSERT_VALID(form);
		form->SetFont(fpoint, fstyle, fname, resize);
	}
}

void CAxisForm::DrawForm(CDC* dc)
{
	CRect	fmRc(0, 0, 0, 0);
	CfmTab* tab = (CfmTab*)0;
	CfmBase* form = (CfmBase*)0;

	if (dc == NULL)
	{
		for (int ii = 0; ii < m_fmObs.GetSize(); ii++)
		{
			form = (CfmBase*)m_fmObs.GetAt(ii);
			ASSERT_VALID(form);
			form->ShowControl(FALSE);
		}
		return;
	}

	for (int ii = 0; ii < m_fmObs.GetSize(); ii++)
	{
		if (m_tabs.Lookup(ii, (CObject*&)tab))
		{
			if (!tab->IsFormVisible(ii))
			{
				form = (CfmBase *)m_fmObs.GetAt(ii);
				ASSERT_VALID(form);
				form->ShowControl(FALSE);
				continue;
			}
		}

		form = (CfmBase *)m_fmObs.GetAt(ii);
		ASSERT_VALID(form);
		fmRc = form->GetRect();
		if (dc->RectVisible(&fmRc))
		{
			form->Draw(dc);
			form->ShowControl(form->m_form->properties & PR_VISIBLE);
		}
	}
}

void CAxisForm::ClearForm(int type)
{
	for (int ii = 0; ii < m_fmObs.GetSize(); ii++)
	{
		switch (m_formR[ii].kind)
		{
		case FM_OUT:
		case FM_TABLE:
			if (type == CLR_INPUT)
				continue;
			break;
		case FM_GRID:
		case FM_EDIT:
		case FM_COMBO:
		case FM_MEMO:
			switch (type)
			{
			case CLR_INPUT:
				if (m_formR[ii].iok == EIO_OUTPUT)
					continue;
				break;
			case CLR_OUTPUT:
				if (m_formR[ii].iok == EIO_INPUT)
					continue;
				[[fallthrough]];
			case CLR_ALL:
				[[fallthrough]];
			default:
				break;
			}
			break;
		default:
			continue;
		}

		CfmBase* form = (CfmBase *)m_fmObs.GetAt(ii);
		ASSERT_VALID(form);
		form->ClearData();
	}
}

void CAxisForm::ResizeForm(float hRatio, float vRatio, bool font, bool redraw)
{
	m_hRatio = hRatio;
	m_vRatio = vRatio;
	
	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		CfmBase* form = (CfmBase *)m_fmObs.GetAt(ii);
		ASSERT_VALID(form);
		form->Resize(hRatio, vRatio, font, redraw);
	}
}

void CAxisForm::ResizeForm(bool redraw)
{
	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		CfmBase* form = (CfmBase *)m_fmObs.GetAt(ii);
		ASSERT_VALID(form);
		form->Resize(m_hRatio, m_vRatio, true, redraw);
	}
}

void CAxisForm::AdjustForm(CSize size, bool move)
{
	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		CfmBase* form = (CfmBase *)m_fmObs.GetAt(ii);
		ASSERT_VALID(form);
		if (move)
			form->Adjust(size, 1.0, 1.0);
		else
			form->Adjust(size);
	}
}

void CAxisForm::OffsetForm(CSize offs)
{
	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		CfmBase* form = (CfmBase *)m_fmObs.GetAt(ii);
		ASSERT_VALID(form);
		form->Offset(offs);
	}
}

void CAxisForm::DominoForm(bool all)
{
	int	rgb = 0, auxS = 0;
	DWORD	offs = 0;
	CfmBase *form, *domino;

	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		switch (m_formR[ii].kind)
		{
		case FM_EDIT:
		case FM_OUT:
			if (!(m_formR[ii].attr & FA_DOMINO))
				continue;

			if (m_formR[ii].keys == NOVALUE)
				continue;

			form = (CfmBase *)m_fmObs.GetAt(ii);
			ASSERT_VALID(form);

			auxS = m_formR[ii].keys;
			domino = (CfmBase *)m_fmObs.GetAt(auxS);
			ASSERT_VALID(domino);

			rgb = domino->GetFgColor();
			form->SetFgColor(rgb);
			form->m_itRGB = domino->m_itRGB;
			break;

		case FM_GRID:
		case FM_TABLE:
			if (!all)
				continue;

			offs = m_formR[ii].vals[0];
			if (offs == NOVALUE)
				break;

			form = (CfmBase *)m_fmObs.GetAt(ii);
			ASSERT_VALID(form);
			form->SetDomino();
			break;
		default:
			continue;
		}
	}
}

void CAxisForm::ReloadForm(CRect mRect)
{
	CPoint	pt;

	pt.x = mRect.left;
	pt.y = mRect.top;

	float hRatio = (float)mRect.Width() / (float)m_mapH->width;
	float vRatio = (float)mRect.Height() / (float)m_mapH->height;

	m_rect = mRect;
	for (int ii = 0; ii < m_fmObs.GetSize(); ii++)
	{
		CfmBase* form = (CfmBase *)m_fmObs.GetAt(ii);
		ASSERT_VALID(form);
		form->Reload();
		form->Adjust(pt, hRatio, vRatio);
	}
	m_view->InvalidateRect(&m_rect, FALSE);
}

void CAxisForm::ResetSystemForm()
{
	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		if (m_formR[ii].kind != FM_SYS)
			continue;

		CfmBase* form = (CfmBase *)m_fmObs.GetAt(ii);
		ASSERT_VALID(form);
		form->WriteData(_T(""));
	}
}

void CAxisForm::SetRadioGroup(CString symbol, bool checked)
{
	int key = 0;
	if (!FindForm(symbol, key))
		return;

	if (m_formR[key].keys == NOVALUE)
		return;

	int	index;
	if (m_radio.Lookup(m_formR[key].keys, index))
	{
		if (!checked)
			return;
	}

	index = (m_formR[key].properties & PR_CHECKED) ? key : -1;
	m_radio.SetAt(m_formR[key].keys, index);
}

void CAxisForm::UnCheckRadioGroup(CString symbol)
{
	int key = 0;
	if (!FindForm(symbol, key))
		return;

	if (m_formR[key].keys == NOVALUE)
		return;

	int index = 0;
	if (m_radio.Lookup(m_formR[key].keys, index))
	{
		if (index != -1 && key != index)
		{
			CfmBase* form = GetAtForm(index);
			if (form) ((CfmRadio *)form)->SetChecked(false);
		}
		m_radio.SetAt(m_formR[key].keys, key);
	}
}

void CAxisForm::SetTabPage(int index, CfmBase* tab)
{
	ASSERT_VALID(tab);
	
	if (index >= 0 && index < m_mapH->formN)
		m_tabs.SetAt(index, tab);
}

bool CAxisForm::IsFormVisible(CfmBase* form)
{
	int	key = FindForm(form);
	if (key < 0)
		return false;

	CfmTab*	tab = (CfmTab*) 0;
	if (m_tabs.Lookup(key, (CObject*&)tab))
	{
		if (!tab->IsFormVisible(key))
			return false;
	}
	return true;
}

bool CAxisForm::PlayAVI(bool replay)
{
	if (m_avi < 0)
		return false;

	CfmAvi*	fmAvi = (CfmAvi *)GetAtForm(m_avi);
	if (fmAvi && replay)
		return fmAvi->Replay();
	return fmAvi->Play();
}

void CAxisForm::StopAVI()
{
	if (m_avi < 0)
		return;

	CfmAvi* fmAvi = (CfmAvi *)GetAtForm(m_avi);
	if (fmAvi) fmAvi->Stop();
}

bool CAxisForm::GetCode(int kind, int index, CString& code, bool prev)
{
	if (axGetCategory)
	{
		char	wb[128] = { 0, };

		strcpy_s(wb, sizeof(wb), code);
		if ((*axGetCategory)((char *)m_root.operator LPCTSTR(), kind, index, wb, prev))
		{
			code = wb;
			return true;
		}
	}

	return false;
}

int CAxisForm::GetSpinData(DWORD type, int nfloat, int xfloat, CString& data, double value, bool prev)
{
	if (axGetSpinData)
	{
		char	wb[128] = { 0, };

		strcpy_s(wb, sizeof(wb), data);
		xfloat = (*axGetSpinData)(type, nfloat, xfloat, wb, value, prev);
		data = wb;
		return xfloat;
	}

	return 0;
}

double CAxisForm::GetDelta(DWORD type, double value, bool prev)
{
	if (axGetDelta)
		return (*axGetDelta)(type, value, prev);

	return 0;
}

bool CAxisForm::IsContrast(int& value)
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

int CAxisForm::GetContrast(int key, CString& text)
{
	switch (key)
	{
	case '1':
		text = LCcontrast[ct_uLimit];
		return ct_uLimit;
	case '+':
	case '2':
		text = LCcontrast[ct_Rise];
		return ct_Rise;
	case '4':
		text = LCcontrast[ct_lLimit];
		return ct_lLimit;
	case '-':
	case '5':
		text = LCcontrast[ct_Fall];
		return ct_Fall;
	}
	text  = _T("  ");
	return ct_Flat;
}

void CAxisForm::GetContrastData(CString& text)
{
	int	pos = 0;
	CString	tmps = text;

	pos = tmps.Find(LCcontrast[ct_uLimit]);
	if (pos != -1)
	{
		text = tmps.Left(pos) + _T('+') + tmps.Mid(pos+2);
		return;
	}
	pos = tmps.Find(LCcontrast[ct_Rise]);
	if (pos != -1)
	{
		text = tmps.Left(pos) + _T('+') + tmps.Mid(pos+2);
		return;
	}
	pos = tmps.Find(LCcontrast[ct_lLimit]);
	if (pos != -1)
	{
		text = tmps.Left(pos) + _T('-') + tmps.Mid(pos+2);
		return;
	}
	pos = tmps.Find(LCcontrast[ct_Fall]);
	if (pos != -1)
	{
		text = tmps.Left(pos) + _T('-') + tmps.Mid(pos+2);
		return;
	}
}


CString CAxisForm::TrimSign(CString& data)
{
	int	idx = 0;
	CString	signs = _T("");

	switch (data.FindOneOf("+-"))
	{
	case -1:
		if (data.Find(LCcontrast[ct_uLimit]) && data.Find(LCcontrast[ct_lLimit]) &&
				data.Find(LCcontrast[ct_Rise]) && data.Find(LCcontrast[ct_Fall]))
			break;
		signs += data.GetAt(idx++);
		[[fallthrough]];
	case 0:
		signs += data.GetAt(idx++);
		data = data.Mid(idx);
		break;
	}
	return signs;
}

int CAxisForm::GetDouble(int value)
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

void CAxisForm::addForm(CString symbol, CfmBase* form)
{
	symbol.MakeUpper();
	m_n2Obs.SetAt(symbol, form);
}

void CAxisForm::makeString()
{
	int	count = 0;

	if (m_mapH->fName != NOVALUE)
		m_mapH->fName   = (DWORD)&m_strR[m_mapH->fName];
	if (m_mapH->caption != NOVALUE)
		m_mapH->caption = (DWORD)&m_strR[m_mapH->caption];
	if (m_mapH->helpN != NOVALUE)
		m_mapH->helpN   = (DWORD)&m_strR[m_mapH->helpN];
	if (m_mapH->publicR != NOVALUE)
		m_mapH->publicR = (DWORD)&m_strR[m_mapH->publicR];

	for (int ii = 0; ii < m_mapH->formN; ii++)
	{
		if (m_formR[ii].name != NOVALUE)
			m_formR[ii].name  = (DWORD)&m_strR[m_formR[ii].name];
		if (m_formR[ii].fStyle != FS_NONE && m_formR[ii].fName != NOVALUE)
			m_formR[ii].fName = (DWORD)&m_strR[m_formR[ii].fName];
		if (m_formR[ii].isHint != IH_NOP && m_formR[ii].onHint != NOVALUE)
			m_formR[ii].onHint = (DWORD)&m_strR[m_formR[ii].onHint];
	}

	if (m_mapH->cellR != NOVALUE)
	{
		if (m_mapH->pageR != NOVALUE)
			count = m_mapH->pageR - m_mapH->cellR;
		else if (m_mapH->valueR != NOVALUE)
			count = m_mapH->valueR - m_mapH->cellR;
		else if (m_mapH->scriptR != NOVALUE)
			count = m_mapH->scriptR - m_mapH->cellR;
		else if (m_mapH->strR != NOVALUE)
			count = m_mapH->strR - m_mapH->cellR;
		else
			return;

		count /= sizeof(struct _cellR);
		for (int ii = 0; ii < count; ii++)
		{
			if (m_cellR[ii].name != NOVALUE)
				m_cellR[ii].name = (DWORD)&m_strR[m_cellR[ii].name];
			if (m_cellR[ii].head != NOVALUE)
				m_cellR[ii].head = (DWORD)&m_strR[m_cellR[ii].head];
			if (m_cellR[ii].isHint != IH_NOP && m_cellR[ii].onHint != NOVALUE)
				m_cellR[ii].onHint = (DWORD)&m_strR[m_cellR[ii].onHint];
		}
	}
}

void CAxisForm::removeForm()
{
	for (int ii = 0; ii < m_fmObs.GetSize(); ii++)
	{
		CfmBase* form = (CfmBase *)m_fmObs.GetAt(ii);
		ASSERT_VALID(form);
		delete	form;
	}
	m_fmObs.RemoveAll();
}

void CAxisForm::getExternalScript(CString& scripts, CString& pubs)
{
	CString	text;
	CString	tmps = _T(""), tmpx = _T("");
	int	pos = 0;

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

		CString str  = _T("#LOAD ");
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

CString CAxisForm::getScripts(CString name, CString& pubs)
{
	CString	str = _T("");
	char *mapB = nullptr, *wb = nullptr;


	str.Format(_T("%s/%s/%s/%s/%s"), LPCTSTR(m_root), MAPDIR, LPCTSTR(name.Left(L_SGID)), LPCTSTR(name.Left(L_TGID)), LPCTSTR(name));
	HANDLE hFile = CreateFile(str, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return _T("");

	DWORD dwMapL = GetFileSize(hFile, NULL);
	if (dwMapL == 0xFFFFFFFF || dwMapL < L_MAPH)
	{
		CloseHandle(hFile);
		return _T("");
	}

	wb = new char[dwMapL+2];
	DWORD dwReadLen = 0;
	BOOL  bRc = ReadFile(hFile, wb, dwMapL, &dwReadLen, NULL);
	CloseHandle(hFile);
	if (!bRc || dwMapL != dwReadLen)
	{
		delete[] wb;
		return _T("");
	}

	if (!strncmp(&wb[iAXIS], sKEY, iKEY))
	{
		mapB = new char[dwMapL * 4];
		int mapL = DoSomething(doDECOMPRESS, NULL, (WPARAM)&wb[iAXIS + iKEY], (LPARAM)mapB, (int)(dwMapL - iAXIS - iKEY), dwMapL * 4);
		if (mapL <= 0 || mapL < L_MAPH)
		{
			delete[] wb;
			delete[] mapB;
			return _T("");
		}
		CopyMemory(mapB, wb, iAXIS);
		delete[] wb;
	}
	else
		mapB = wb;

	str = _T("");
	struct	_mapH* mapH = (struct _mapH *)mapB;
	if (mapH->type == VW_PROC && mapH->scriptR < dwMapL || mapH->scriptR != NOVALUE)
	{
		if (m_mapH->publicR != NOVALUE)
			pubs = (char *)m_mapH->publicR;

		if (m_mapH->strR < dwMapL || m_mapH->strR != NOVALUE)
			str = CString((char *)&mapB[mapH->scriptR], mapH->strR - mapH->scriptR);
		else
			str = CString((char *)&mapB[mapH->scriptR]);

		CString	text = _T("AX_SUB_include_AX_");
		getRoutine(text, str, true);
	}

	delete[] mapB;
	return str;
}

bool CAxisForm::getRoutine(CString& str, CString& src, bool remove)
{
	CString	text, tmps, string;
	int	pos, index, sum;
	bool	func = false;

	pos = index = sum = 0;
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

bool CAxisForm::findOption(CString& scripts)
{
	CString	text, str;
	CString	tmps = _T("");
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

	tmps.Trim();
	if (!tmps.CompareNoCase(_T("explicit")))
	{
		scripts.Delete(spos, text.GetLength()+pos);
		return true;
	}

	return false;
}

bool CAxisForm::isComment(CString str)
{
	if (str.IsEmpty())
		return false;

	int pos = str.ReverseFind('\n');
	if (pos != -1)
		str = str.Mid(pos+1);

	str.Trim();
	if (!str.IsEmpty())
	{
		pos = str.Find('\'');
		if (pos != -1)
			return true;
	}

	return false;
}

BOOL CAxisForm::IsRectVisible(CDC* dc)
{
	CRect	clipRc;

	dc->GetClipBox(&clipRc);
	return isRectVisible(dc, 0, clipRc);
}

BOOL CAxisForm::isRectVisible(CDC* dc, int index, CRect rect)
{
	CRgn	rgn;
	CRect	fmRc;
	CfmTab*	tab = (CfmTab*) 0;
	CfmBase* form;

	for (int ii = index; ii < m_fmObs.GetSize(); ii++)
	{
		if (m_tabs.Lookup(ii, (CObject*&)tab))
		{
			if (!tab->IsFormVisible(ii))
				continue;
		}

		form = (CfmBase *)m_fmObs.GetAt(ii);
		ASSERT_VALID(form);
		switch (form->m_form->kind)
		{
		case FM_OBJECT:
			break;
		case FM_OUT:
		case FM_GRID:
		case FM_TABLE:
			break;
		case FM_EDIT:
		case FM_COMBO:
			if (form->m_form->iok == EIO_INOUT)
				break;
			[[fallthrough]];
		default:
			continue;
		}
		fmRc = form->GetRect();
		if (dc->RectVisible(&fmRc))
		{
			rgn.CreateRectRgnIndirect(fmRc);
			if (rgn.RectInRegion(rect))
			{
				rgn.DeleteObject();
				if (form->m_form->kind == FM_OBJECT)
				{
					if (DoSomething(doBACK, form, (WPARAM)dc))
						continue;
				}
if (form->m_form->kind == FM_GRID)
TRACE("GRID [%s]\n", (char *)form->m_form->name);
				return FALSE;
			}
			rgn.DeleteObject();
		}
	}

	return TRUE;
}
