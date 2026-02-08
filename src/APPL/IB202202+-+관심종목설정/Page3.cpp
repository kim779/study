// Page3.cpp : implementation file
//

#include "stdafx.h"
#include "IB202201.h"
#include "Page3.h"

#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "../../control/fx_misc/misctype.h"

#include <MMSYSTEM.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	saveFILE	"portfolio.ini"
#define	tempFILE	"portfolio.ini.tmp"

#define	saveFILEx	"codeinfo.dat"
#define	tempFILEx	"codeinfo.dat.tmp"

const char	gnCHAR	= 0x7f;
const char	tabCHAR	= 0x09;
const char	lfCHAR	= 0x0A;

const UINT kindID[6] = {IDC_CHK_CURR,  IDC_CHK_DIFF,  IDC_CHK_ASK,  IDC_CHK_BID,  IDC_CHK_RATE,  IDC_CHK_VOL};
const UINT avalID[6] = {IDC_EDIT_CURR, IDC_EDIT_DIFF, IDC_EDIT_ASK, IDC_EDIT_BID, IDC_EDIT_RATE, IDC_EDIT_VOL};
const UINT condID[6] = {IDC_RUP_CURR,  IDC_RUP_DIFF,  IDC_RUP_ASK,  IDC_RUP_BID,  IDC_RUP_RATE,  IDC_RUP_VOL};
const UINT condXD[6] = {IDC_RDN_CURR,  IDC_RDN_DIFF,  IDC_RDN_ASK,  IDC_RDN_BID,  IDC_RDN_RATE,  IDC_RDN_VOL};
const UINT operID[6] = {0,             IDC_AND_DIFF,  IDC_AND_ASK,  IDC_AND_BID,  IDC_AND_RATE,  IDC_AND_VOL};
const UINT operXD[6] = {0,             IDC_OR_DIFF,   IDC_OR_ASK,   IDC_OR_BID,   IDC_OR_RATE,   IDC_OR_VOL};

/////////////////////////////////////////////////////////////////////////////
// CPage3 property page

IMPLEMENT_DYNCREATE(CPage3, CPropertyPage)

CPage3::CPage3(CWnd* parent) : CPropertyPage(CPage3::IDD)
{
	//{{AFX_DATA_INIT(CPage3)
	//}}AFX_DATA_INIT

	m_parent = parent;
	m_activegno = -1;

	m_inters.RemoveAll();

	m_activePage = false;
}

CPage3::~CPage3()
{
	ClearInterest(); ClearCondition();
}

void CPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3)
	DDX_Control(pDX, IDC_BTN_SAVEALL, m_saveall);
	DDX_Control(pDX, IDC_EDIT_JPRC, m_editjprc);
	DDX_Control(pDX, IDC_EDIT_VOL, m_editvol);
	DDX_Control(pDX, IDC_EDIT_RATE, m_editrate);
	DDX_Control(pDX, IDC_EDIT_JNUM, m_editjnum);
	DDX_Control(pDX, IDC_EDIT_DIFF, m_editdiff);
	DDX_Control(pDX, IDC_EDIT_BID, m_editbid);
	DDX_Control(pDX, IDC_EDIT_ASK, m_editask);
	DDX_Control(pDX, IDC_EDIT_CURR, m_editcurr);
	DDX_Control(pDX, IDC_COMBO_MMGB, m_mmgb);
	DDX_Control(pDX, IDC_COMBO_JMGB, m_jmgb);
	DDX_Control(pDX, IDC_SISE_VOL, m_ssVol);
	DDX_Control(pDX, IDC_SISE_RATE, m_ssRate);
	DDX_Control(pDX, IDC_SISE_DIFF, m_ssDiff);
	DDX_Control(pDX, IDC_SISE_CURR, m_ssCurr);
	DDX_Control(pDX, IDC_SISE_BID, m_ssBid);
	DDX_Control(pDX, IDC_SISE_ASK, m_ssAsk);
	DDX_Control(pDX, IDC_CODENAME, m_jnam);
	DDX_Control(pDX, IDC_CODE, m_code);
	DDX_Control(pDX, IDC_GNAME, m_gname);
	DDX_Control(pDX, IDC_LIST2, m_list2);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage3, CPropertyPage)
	//{{AFX_MSG_MAP(CPage3)
	ON_CBN_SELCHANGE(IDC_GNAME, OnSelchangeGname)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, OnClickList2)
	ON_CBN_SELCHANGE(IDC_COMBO_MMGB, OnSelchangeComboMmgb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3 message handlers

BOOL CPage3::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_root = Variant(homeCC, "");
	m_name = Variant(nameCC, "");
	m_font = getAxFont(idTOstring(IDS_DEFAULTFONT));
	

	CString userD; userD.Format("%s/%s/%s", m_root, USRDIR, m_name);
	CString saveN; saveN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, saveFILEx);
	CString tempN; tempN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILEx);

	CreateDirectory(userD, nullptr); CopyFile(saveN, tempN, FALSE);

	initList(); initCombo(); initStatic(); initEdit(); initCondition();

	if (m_activegno != -1)
	{
		int count = loadingInterest(m_activegno);

		int nItem; _inters* pintes;
		for (int ii = 0; ii < count; ii++)
		{
			pintes = m_inters.GetAt(ii);
			nItem = AddItem(&m_list1, pintes->code, ii);
			AddItem(&m_list1, pintes->name, nItem, 1);
		}

		_codeinfo* pinfo = nullptr;
		count = loadingCondition();

		for (ii = 0; ii < count; ii++)
		{
			pinfo = m_cinfos.GetAt(ii);
			nItem = AddItem(&m_list2, pinfo->code, ii);
			AddItem(&m_list2, pinfo->name, nItem, 1);

			if (pinfo->capture[0] == '0')
				AddItem(&m_list2, idTOstring(IDS_CAPTURENO), nItem, 2);
			else
				AddItem(&m_list2, idTOstring(IDS_CAPTUREOK), nItem, 2);
		}
	}

	m_fontBT = getAxFont("굴림체", 12, 3);

	m_saveall.SetFont(m_fontBT); m_saveall.Init(bsGRAD|bsGENERAL|bsTEXT|bsCOLOR,   "저장",      "", RGB(255, 255, 255), RGB(0, 0, 255), RGB(128, 128, 255));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPage3::OnSetActive() 
{
	m_activePage = true;	
	return CPropertyPage::OnSetActive();
}

BOOL CPage3::OnKillActive() 
{
	savingCondition();
	return CPropertyPage::OnKillActive();
}

void CPage3::OnSelchangeGname()
{
	m_list1.DeleteAllItems();

	m_activegno = m_gname.GetItemData(m_gname.GetCurSel());
	int count = loadingInterest(m_activegno);

	int nItem; _inters* pintes;
	for (int ii = 0; ii < count; ii++)
	{
		pintes = m_inters.GetAt(ii);
		nItem = AddItem(&m_list1, pintes->code, ii);
		AddItem(&m_list1, pintes->name, nItem, 1);
	}

	initialDialog();
}

void CPage3::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int	nitem;
	CString	code, name;
	POSITION pos = m_list1.GetFirstSelectedItemPosition();

	if (pos)
	{
		nitem = m_list1.GetNextSelectedItem(pos);

		code = m_list1.GetItemText(nitem, 0);
		name = m_list1.GetItemText(nitem, 1);

		m_code.SetText(code); m_jnam.SetText(name);

		initialDialog(); 
		sendTransaction(code);
	}

	*pResult = 0;
}

void CPage3::OnClickList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int	nitem;
	CString	code, name, string;
	POSITION pos = m_list2.GetFirstSelectedItemPosition();
	
	*pResult = 0;

	if (pos)
	{
		nitem = m_list2.GetNextSelectedItem(pos);
		if (nitem >= m_cinfos.GetSize())
			return;

		initialDialog();

		CString	string;
		_codeinfo* pinfo = m_cinfos.GetAt(nitem);

		string.Format("%s", chTOstring(pinfo->code, sizeof(pinfo->code))); m_code.SetText(string);
		string.Format("%s", chTOstring(pinfo->name, sizeof(pinfo->name))); m_jnam.SetText(string);
		sendTransaction(chTOstring(pinfo->code, sizeof(pinfo->code)));

		for (int ii = 0; ii < sizeof(kindID) / sizeof(UINT); ii++)
		{
			if (pinfo->alarm[ii].kind[0] == '0')
				continue;

			((CButton *)GetDlgItem(kindID[ii]))->SetCheck(1);
			idTOwnd(avalID[ii])->SetText(chTOstring(pinfo->alarm[ii].aval, 10));
			
			if (pinfo->alarm[ii].cond[0] == '1')
			{
				if (condID[ii] != 0) ((CButton *)GetDlgItem(condID[ii]))->SetCheck(0);
				if (condXD[ii] != 0) ((CButton *)GetDlgItem(condXD[ii]))->SetCheck(1);
			}

			if (pinfo->alarm[ii].oper[0] == '1')
			{
				if (operID[ii] != 0) ((CButton *)GetDlgItem(operID[ii]))->SetCheck(0);
				if (operXD[ii] != 0) ((CButton *)GetDlgItem(operXD[ii]))->SetCheck(1);
			}
		}

		if (pinfo->o_run[0] == '1')
		{
			((CButton *)GetDlgItem(IDC_CHK_JUMUN))->SetCheck(1);

			m_jmgb.SetCurSel(pinfo->o_kind[0] - '0');
			m_mmgb.SetCurSel(pinfo->o_mmgb[0] - '0');

			string = chTOstring(pinfo->o_num, sizeof(pinfo->o_num)); string.TrimLeft();
			idTOwnd(IDC_EDIT_JNUM)->SetText(string);

			string = chTOstring(pinfo->o_prc, sizeof(pinfo->o_prc)); string.TrimLeft();
			idTOwnd(IDC_EDIT_JPRC)->SetText(string);
		}

		if (pinfo->a_kind[0] & 0x01)
			((CButton *)GetDlgItem(IDC_CHK_CONFIRM))->SetCheck(1);
		if (pinfo->a_kind[0] & 0x02)
			((CButton *)GetDlgItem(IDC_CHK_SIGNAL))->SetCheck(1);
		if (pinfo->a_kind[0] & 0x04)
			((CButton *)GetDlgItem(IDC_CHK_WAVE))->SetCheck(1);

		((CWnd *)GetDlgItem(IDC_EDIT_WAVE))->SetWindowText(chTOstring(pinfo->a_wave, sizeof(pinfo->a_wave)));

		initCondition(true);
	}
}

void CPage3::OnSelchangeComboMmgb() 
{
	int idx = m_mmgb.GetCurSel();
	if (idx == 0) m_editjprc.SetText("0");
}

BOOL CPage3::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	switch (wParam)
	{
	case IDC_CHK_CURR:
	case IDC_CHK_DIFF:
	case IDC_CHK_BID:
	case IDC_CHK_ASK:
	case IDC_CHK_RATE:
	case IDC_CHK_VOL:
		enableWindow(wParam);
		updateEditData(wParam);
		break;
	case IDC_CHK_JUMUN:
	case IDC_CHK_WAVE:
		enableWindow(wParam);
		break;
	case IDC_BTN_ADD:
		addCondition();
		break;
	case IDC_BTN_DELETE:
		delCondition();
		break;
	case IDC_BTN_DELETEALL:
		deleteAllCondition();
		break;
	case IDC_BTN_SAVEALL:
		addCondition(); savingCondition();
		break;
	case IDC_BTN_SEARCH:
		findWave();
		break;
	case IDC_BTN_LISTEN:
		listenWave();
		break;
	}
	return CPropertyPage::OnCommand(wParam, lParam);
}

void CPage3::OKaction()
{
	CString saveN; saveN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, saveFILEx);
	CString tempN; tempN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILEx);

	DeleteFile(saveN); CopyFile(tempN, saveN, FALSE);

	CCaction();

}

void CPage3::CCaction()
{
	CString tempN; tempN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILEx);
	DeleteFile(tempN);
}

void CPage3::initList()
{
	AddColumn(&m_list1, "", 0, 0);
	AddColumn(&m_list1, idTOstring(IDS_GH_NAME), 1, 90);
	m_list1.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	
	AddColumn(&m_list2, "", 0, 0);
	AddColumn(&m_list2, idTOstring(IDS_GH_NAME), 1, 80);
	AddColumn(&m_list2, idTOstring(IDS_CAPTURE), 2, 20);
	m_list2.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}

void CPage3::initCombo()
{
	int	idx;
	int	readL = 0;
	char	readB[512]; //관심그룹 많을때 256->512
	CString	string, gno, gname, filePath;

	m_gname.ResetContent();

	filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);

	readL = GetPrivateProfileString(_T("GROUPORDER"), "00", "", readB, sizeof(readB), filePath);
	if (readL <= 0)	return;

	string = CString(readB, readL);
	while (!string.IsEmpty())
	{
		gno = parseX(string, ";");
		if (gno.IsEmpty() || gno.GetLength() != 2)
			break;

		readL = GetPrivateProfileString(_T("GROUPNAME"), gno, "", readB, sizeof(readB), filePath);
		gname = CString(readB, readL);

		idx = m_gname.AddString(gname); 
		m_gname.SetItemData(idx, atoi(gno));
	}

	if (m_activegno == -1 && m_gname.GetCount() > 0)
	{
		idx = m_gname.SetCurSel(0);
		m_activegno = m_gname.GetItemData(idx);
	}

	m_jmgb.AddString(idTOstring(IDS_JMGB1)); m_jmgb.AddString(idTOstring(IDS_JMGB2));	// 매도,   매수
	m_mmgb.AddString(idTOstring(IDS_MMGB1)); m_mmgb.AddString(idTOstring(IDS_MMGB2));	// 지정가, 시장가

	m_jmgb.SetCurSel(0); m_mmgb.SetCurSel(0);
}

void CPage3::initStatic()
{
	COLORREF bkColor = RGB(255, 255, 255);
	COLORREF fgColor = RGB(  0,   0,   0);

	m_code.Init(ssTEXT|ssCOLOR|ssBORDER, 0, "", "", bkColor, fgColor, alLEFT);
	m_code.SetFont(m_font, false);
	m_jnam.Init(ssTEXT|ssCOLOR|ssBORDER, 0, "", "", bkColor, fgColor, alLEFT);
	m_jnam.SetFont(m_font, false);

	m_ssCurr.Init(ssTEXT|ssCOLOR|ssBORDER, attrNUMERIC|attrCOMMA|attrCORGBx, "", "", bkColor, fgColor, alRIGHT);
	m_ssCurr.SetFont(m_font, false);
	m_ssDiff.Init(ssTEXT|ssCOLOR|ssBORDER, attrNUMERIC|attrNUMERIC|attrCONTRAST, "", "", bkColor, fgColor, alRIGHT);
	m_ssDiff.SetFont(m_font, false);
	m_ssRate.Init(ssTEXT|ssCOLOR|ssBORDER, attrNUMERIC|attrCORGB, "", "", bkColor, fgColor, alRIGHT);
	m_ssRate.SetFont(m_font, false);
	m_ssAsk.Init(ssTEXT|ssCOLOR|ssBORDER, attrNUMERIC|attrCOMMA|attrCORGBx, "", "", bkColor, fgColor, alRIGHT);
	m_ssAsk.SetFont(m_font, false);
	m_ssBid.Init(ssTEXT|ssCOLOR|ssBORDER, attrNUMERIC|attrCOMMA|attrCORGBx, "", "", bkColor, fgColor, alRIGHT);
	m_ssBid.SetFont(m_font, false);
	m_ssVol.Init(ssTEXT|ssCOLOR|ssBORDER, attrNUMERIC|attrCOMMA, "", "", bkColor, fgColor, alRIGHT);
	m_ssVol.SetFont(m_font, false);
}

void CPage3::initEdit()
{
	COLORREF bkColor = RGB(255, 255, 255);
	COLORREF fgColor = RGB(  0,   0,   0);

	m_editcurr.Init(esNORMAL, attrNUMERIC|attrCOMMA, "", bkColor, fgColor, alRIGHT); m_editcurr.SetFont(m_font, false);
	m_editdiff.Init(esNORMAL, attrNUMERIC|attrCOMMA, "", bkColor, fgColor, alRIGHT); m_editdiff.SetFont(m_font, false);
	m_editask.Init(esNORMAL,  attrNUMERIC|attrCOMMA, "", bkColor, fgColor, alRIGHT); m_editask.SetFont(m_font, false);
	m_editbid.Init(esNORMAL,  attrNUMERIC|attrCOMMA, "", bkColor, fgColor, alRIGHT); m_editbid.SetFont(m_font, false);
	m_editrate.Init(esNORMAL, attrNUMERIC|attrFLOAT, "", bkColor, fgColor, alRIGHT); m_editrate.SetFont(m_font, false);
	m_editvol.Init(esNORMAL,  attrNUMERIC|attrCOMMA, "", bkColor, fgColor, alRIGHT); m_editvol.SetFont(m_font, false);

	m_editjprc.Init(esNORMAL,  attrNUMERIC|attrCOMMA, "", bkColor, fgColor, alRIGHT); m_editjprc.SetFont(m_font, false);
	m_editjnum.Init(esNORMAL,  attrNUMERIC|attrCOMMA, "", bkColor, fgColor, alRIGHT); m_editjnum.SetFont(m_font, false);
}

void CPage3::initCondition(bool enableOnly/*false*/)
{
	for (int ii = 0; !enableOnly && ii < sizeof(condID) / sizeof(UINT); ii++)
	{
		if (condID[ii] != 0) ((CButton *)GetDlgItem(condID[ii]))->SetCheck(1);
		if (operID[ii] != 0) ((CButton *)GetDlgItem(operID[ii]))->SetCheck(1);
	}

	for (ii = 0; ii < sizeof(kindID) / sizeof(UINT); ii++)
		enableWindow(kindID[ii]);

	enableWindow(IDC_CHK_JUMUN); enableWindow(IDC_CHK_WAVE); UpdateData(TRUE);
}

void CPage3::initialDialog()
{
	for (int ii = 0; ii < sizeof(kindID) / sizeof(UINT); ii++)
	{
		if (kindID[ii] != 0) ((CButton *)GetDlgItem(kindID[ii]))->SetCheck(0);
		if (avalID[ii] != 0) idTOwnd(avalID[ii])->SetText("");

		if (condID[ii] != 0) ((CButton *)GetDlgItem(condID[ii]))->SetCheck(1);
		if (condXD[ii] != 0) ((CButton *)GetDlgItem(condXD[ii]))->SetCheck(0);
		if (operID[ii] != 0) ((CButton *)GetDlgItem(operID[ii]))->SetCheck(1);
		if (operXD[ii] != 0) ((CButton *)GetDlgItem(operXD[ii]))->SetCheck(0);
	}

	((CButton *)GetDlgItem(IDC_CHK_JUMUN))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_CHK_CONFIRM))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_CHK_SIGNAL))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_CHK_WAVE))->SetCheck(0);

	initCondition(true);
}

int CPage3::loadingInterest(int gno)
{
	ClearInterest();

	CString	filePath;
	if (ExistFile(gno, true))
		filePath.Format("%s/%s/%s/portfolio.i%02d.tmp", m_root, USRDIR, m_name, gno);
	else if (ExistFile(gno))
		filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_name, gno);
	else
		return 0;

	UINT	readL;
	struct	_inters* pintes;

	CFile	fileH(filePath, CFile::modeRead);
	if (fileH.m_hFile == CFile::hFileNull)
		return 0;

	for (int ii = 0; ii < maxJONGMOK; ii++)
	{
		pintes = (_inters *) new char[sz_inters];

		readL = fileH.Read(pintes, sz_inters);
		if (readL < sz_inters)
		{
			delete pintes;
			break;
		}
		m_inters.Add(pintes);
	}

	fileH.Close();
	return m_inters.GetSize();
}

void CPage3::ClearInterest()
{
	for (int ii = 0; ii < m_inters.GetSize(); ii++)
		delete m_inters.GetAt(ii);
	m_inters.RemoveAll();
}

void CPage3::enableWindow(UINT chkid)
{
	struct	_enableW {
		UINT	ids[6];
	} enableW[] = {
		{IDC_CHK_CURR,	IDC_EDIT_CURR,	IDC_RUP_CURR,	IDC_RDN_CURR,		0,	0},
		{IDC_CHK_DIFF,	IDC_EDIT_DIFF,	IDC_RUP_DIFF,	IDC_RDN_DIFF,	IDC_AND_DIFF,	IDC_OR_DIFF},
		{IDC_CHK_ASK,	IDC_EDIT_ASK,	IDC_RUP_ASK,	IDC_RDN_ASK,	IDC_AND_ASK,	IDC_OR_ASK},
		{IDC_CHK_BID,	IDC_EDIT_BID,	IDC_RUP_BID,	IDC_RDN_BID,	IDC_AND_BID,	IDC_OR_BID},
		{IDC_CHK_RATE,	IDC_EDIT_RATE,	IDC_RUP_RATE,	IDC_RDN_RATE,	IDC_AND_RATE,	IDC_OR_RATE},
		{IDC_CHK_VOL,	IDC_EDIT_VOL,	IDC_RUP_VOL,	IDC_RDN_VOL,	IDC_AND_VOL,	IDC_OR_VOL},
		{IDC_CHK_JUMUN,	IDC_COMBO_JMGB,	IDC_COMBO_MMGB,	IDC_EDIT_JNUM,	IDC_EDIT_JPRC,	0},
		{IDC_CHK_WAVE,	IDC_EDIT_WAVE,	IDC_BTN_SEARCH, IDC_BTN_LISTEN,		0,	0},
	};

	BOOL	enable;
	CWnd*	pWnd = nullptr; CButton* button = nullptr;

	for (int ii = 0; ii < sizeof(enableW)/sizeof(_enableW); ii++)
	{
		if (enableW[ii].ids[0] != chkid || enableW[ii].ids[0] == 0)
			continue;

		button = (CButton*) GetDlgItem(enableW[ii].ids[0]);
		enable = button->GetCheck() ? TRUE : FALSE;
		for (int jj = 1; jj < sizeof(enableW[ii].ids) / sizeof(UINT); jj++)
		{
			if (!enableW[ii].ids[jj])
				continue;

			pWnd = (CWnd*) GetDlgItem(enableW[ii].ids[jj]);
			pWnd->EnableWindow(enable);
		}
		break;
	}
}

void CPage3::updateEditData(UINT chkid)
{
	struct _updateW {
		UINT		ids[2];
		CfxStatic*	fxStatic;
	} updateW[] = {
		{IDC_CHK_CURR, IDC_EDIT_CURR, &m_ssCurr}, {IDC_CHK_DIFF, IDC_EDIT_DIFF, &m_ssDiff}, {IDC_CHK_ASK, IDC_EDIT_ASK, &m_ssAsk},
		{IDC_CHK_BID,  IDC_EDIT_BID,  &m_ssBid},  {IDC_CHK_RATE, IDC_EDIT_RATE, &m_ssRate}, {IDC_CHK_VOL, IDC_EDIT_VOL, &m_ssVol},
	};

	BOOL	enable; CString string;
	CfxEdit* pWnd = nullptr; CButton* button = nullptr;

	for (int ii = 0; ii < sizeof(updateW) / sizeof(_updateW); ii++)
	{
		if (updateW[ii].ids[0] != chkid || updateW[ii].ids[0] == 0)
			continue;

		button = (CButton*) GetDlgItem(updateW[ii].ids[0]);
		enable = button->GetCheck() ? TRUE : FALSE;

		pWnd = (CfxEdit*) GetDlgItem(updateW[ii].ids[1]);
		pWnd->SetText("");
		if (enable)
		{
			bool	minus = false;

			string = updateW[ii].fxStatic->GetText();
			if (updateW[ii].ids[1] == IDC_EDIT_DIFF && string.GetLength() > 0)
			{
				switch (string.GetAt(0))
				{
				case '4':
				case '5':
				case '-':
					minus = true;
					break;
				}
				string = string.Mid(1);
				if (minus) string.Insert(0, '-');
			}
				
			pWnd->SetText(string);
		}
		break;
	}
}

int CPage3::loadingCondition()
{
	ClearCondition();

	CString	filePath; filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILEx);

	CFileFind finder;
	if (!finder.FindFile(filePath))
		return 0;

	UINT	readL;
	struct	_codeinfo* pinfo;

	CFile	fileH(filePath, CFile::modeRead);
	if (fileH.m_hFile == CFile::hFileNull)
		return 0;

	for (int ii = 0; ii < maxJONGMOK; ii++)
	{
		pinfo = (_codeinfo *) new char[sz_codeinfo];

		readL = fileH.Read(pinfo, sz_codeinfo);
		if (readL < sz_codeinfo)
		{
			delete pinfo;
			break;
		}
		m_cinfos.Add(pinfo);
	}
	fileH.Close();
	return m_cinfos.GetSize();
}

void CPage3::savingCondition()
{
	CString	filePath;
	filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILEx);

	struct	_codeinfo* pinfo;

	CFile	fileH(filePath, CFile::modeWrite|CFile::modeCreate);
	if (fileH.m_hFile == CFile::hFileNull) return;

	for (int ii = 0; ii < m_cinfos.GetSize(); ii++)
	{
		pinfo = m_cinfos.GetAt(ii);
		fileH.Write(pinfo, sizeof(_codeinfo));
	}
	fileH.Close();
}

void CPage3::ClearCondition()
{
	for (int ii = 0; ii < m_cinfos.GetSize(); ii++)
		delete m_cinfos.GetAt(ii);
	m_cinfos.RemoveAll();
}

void CPage3::addCondition()
{
	CString code = m_code.GetText();
	CString	name = m_jnam.GetText();

	if (code.IsEmpty() || name.IsEmpty())
	{
		GuideMsg(idTOstring(IDS_EM_SELECTION));
		return;
	}

	if (m_cinfos.GetSize() >= maxJONGMOK)
	{
		GuideMsg(idTOstring(IDS_GUIDE1));

		return;
	}

	struct	_codeinfo* pinfo = nullptr;
	for (int xidx = 0; xidx < m_cinfos.GetSize(); xidx++)
	{
		pinfo = m_cinfos.GetAt(xidx);
		if (!code.CompareNoCase(pinfo->code))
			break;
	}

	if (xidx >= m_cinfos.GetSize())
	{
		xidx = -1;
		pinfo = (_codeinfo*) new char[sz_codeinfo];
		ZeroMemory(pinfo, sizeof(_codeinfo));
	}

	int index;
	CString string;
	CButton* button; CWnd* pWnd;

	CopyMemory(pinfo->code, code.operator LPCTSTR(), min(code.GetLength(), sizeof(pinfo->code)));
	CopyMemory(pinfo->name, name.operator LPCTSTR(), min(name.GetLength(), sizefo(pinfo->name)));
	pinfo->capture[0] = '0';

	for (int ii = 0; ii < sizeof(kindID) / sizeof(UINT); ii++)
	{
		// initial value
		pinfo->alarm[ii].kind[0] = '0';
		FillMemory(pinfo->alarm[ii].aval, sizeof(pinfo->alarm[ii].aval), ' ');
		pinfo->alarm[ii].cond[0] = '0';
		pinfo->alarm[ii].oper[0] = '0';

		button = (CButton *) GetDlgItem(kindID[ii]);
		if (!button->GetCheck()) continue;

		pinfo->alarm[ii].kind[0] = ii + '1';

		if (avalID[ii] != 0)
		{
			string = idTOwnd(avalID[ii])->GetText();
//			pWnd = (CWnd *) GetDlgItem(avalID[ii]); pWnd->GetWindowText(string);
			CopyMemory(pinfo->alarm[ii].aval, string.operator LPCTSTR(), min(string.GetLength(), 10));
		}
		if (condID[ii] != 0)
		{
			button = (CButton *) GetDlgItem(condID[ii]);
			pinfo->alarm[ii].cond[0] = button->GetCheck() ? '0' : '1';
		}
		if (operID[ii] != 0)
		{
			button = (CButton *) GetDlgItem(operID[ii]);
			pinfo->alarm[ii].oper[0] = button->GetCheck() ? '0' : '1';
		}
	}

	pinfo->o_run[0] = '0';

	button = (CButton *) GetDlgItem(IDC_CHK_JUMUN);
	if (button->GetCheck())
	{
		pinfo->o_run[0] = '1';

		index = m_jmgb.GetCurSel();
		if (index != LB_ERR) pinfo->o_kind[0] = index + '0';

		index = m_mmgb.GetCurSel();
		if (index != LB_ERR) pinfo->o_mmgb[0] = index + '0';

		string.Format("%10s", idTOwnd(IDC_EDIT_JNUM)->GetText());
		CopyMemory(pinfo->o_num, string.operator LPCTSTR(), min(string.GetLength(), 10));

		string.Format("%10s", idTOwnd(IDC_EDIT_JPRC)->GetText());
		CopyMemory(pinfo->o_prc, string.operator LPCTSTR(), min(string.GetLength(), 10));
	}

	pinfo->a_kind[0] = 0x00;

	button = (CButton *) GetDlgItem(IDC_CHK_CONFIRM); if (button->GetCheck()) pinfo->a_kind[0] |= 0x01;
	button = (CButton *) GetDlgItem(IDC_CHK_SIGNAL);  if (button->GetCheck()) pinfo->a_kind[0] |= 0x02;
	button = (CButton *) GetDlgItem(IDC_CHK_WAVE);    if (button->GetCheck()) pinfo->a_kind[0] |= 0x04;

	pWnd = (CWnd *) GetDlgItem(IDC_EDIT_WAVE); pWnd->GetWindowText(string);
	CopyMemory(pinfo->a_wave, string.operator LPCTSTR(), min(string.GetLength(), sizeof(pinfo->a_wave)));

	if (xidx != -1)
	{
		m_cinfos.SetAt(xidx, pinfo);
		AddItem(&m_list2, idTOstring(IDS_CAPTURENO), xidx, 2);
		return;
	}

	index = m_cinfos.Add(pinfo);
	int nItem = AddItem(&m_list2, code, index);
	AddItem(&m_list2, name, nItem, 1); AddItem(&m_list2, idTOstring(IDS_CAPTURENO), nItem, 2);
}

void CPage3::delCondition()
{
	int nItem = -1;
	POSITION pos;
	int count = m_list2.GetItemCount();
	for (int ii = 0; ii < count; ii++)
	{
		pos = m_list2.GetFirstSelectedItemPosition();
		if (pos != nullptr)
		{
			nItem = m_list2.GetNextSelectedItem(pos);
			DeleteItemToList2(nItem);
		}
	}

	count = m_list2.GetItemCount();
	if (count > nItem)
		selcMark(&m_list2, nItem, false);
	else
		selcMark(&m_list2, nItem -1, false);
}

void CPage3::deleteAllCondition()
{
	ClearCondition();
	m_list2.DeleteAllItems();
}

void CPage3::DeleteItemToList2(int ipos)
{
	if (m_list2.GetItemCount() > ipos)
		m_list2.DeleteItem(ipos);

	if (m_cinfos.GetSize() > ipos)
	{
		delete m_cinfos.GetAt(ipos);
		m_cinfos.RemoveAt(ipos);
	}
}

void CPage3::selcMark(CListCtrl* listctrl, int nItem, bool scroll /*=true*/)
{
	CRect	rc; CSize size;

	listctrl->SetItemState(nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

	if (scroll)
	{
		listctrl->GetItemRect(0, rc, LVIR_LABEL);
		size.cx = 0;
		size.cy = rc.Height() * (nItem - listctrl->GetTopIndex());
		listctrl->Scroll(size);
	}
}

void CPage3::deselcMark(CListCtrl* listctrl)
{
	int nItem;
	POSITION pos = listctrl->GetFirstSelectedItemPosition();
	if (pos != nullptr)
	{
		while (pos)
		{
			nItem = listctrl->GetNextSelectedItem(pos);
			listctrl->SetItemState(nItem, nullptr, LVIS_SELECTED|LVIS_FOCUSED);
		}
	}
}

void CPage3::findWave()
{
	CString	string;
	CWnd* pWnd = (CWnd *) GetDlgItem(IDC_EDIT_WAVE);
	pWnd->GetWindowText(string);

	CFileDialog fileDlg(true, nullptr, string, 0, "wave file(*.wav)|*.wav||");
	if (fileDlg.DoModal() == IDOK)
		pWnd->SetWindowText(fileDlg.GetPathName());
}

void CPage3::listenWave()
{
	CString	string;
	CWnd* pWnd = (CWnd *) GetDlgItem(IDC_EDIT_WAVE);
	pWnd->GetWindowText(string);

	if (!string.IsEmpty()) sndPlaySound(string, SND_ASYNC);
}

CfxEdit* CPage3::idTOwnd(UINT id)
{
	switch (id)
	{
	case IDC_EDIT_CURR:	return &m_editcurr;
	case IDC_EDIT_DIFF:	return &m_editdiff;
	case IDC_EDIT_ASK:	return &m_editask;
	case IDC_EDIT_BID:	return &m_editbid;
	case IDC_EDIT_RATE:	return &m_editrate;
	case IDC_EDIT_VOL:	return &m_editvol;
	case IDC_EDIT_JNUM:	return &m_editjnum;
	case IDC_EDIT_JPRC:	return &m_editjprc;
	}

	return (CfxEdit *) 0;
}

void CPage3::sendTransaction(CString code)
{
	int	sendL = 0;
	char	sendB[512];
	char	tempB[32];

	char*	symbols[] = { "1021", "2023", "2024", "2033", "3051", "3071", "2027" };		//2013.02.19 KSJ 2025 --> 3051, 2026 --> 3071 심볼변경 

	struct	_userTH* uTH;
	uTH = (struct _userTH *) sendB;

	sprintf(tempB, "%s%c%s%c", "1301", gnCHAR, code, tabCHAR);		// 종목코드
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	for (int ii = 0; ii < sizeof(symbols) / sizeof(symbols[0]); ii++)
	{
		sprintf(tempB, "%s%c", symbols[ii], tabCHAR);
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	}
	sendB[sendL] = 0x00;
	
	CWnd* wnd = GetParent()->GetParent();
	wnd->SendMessage(TOmapWnd, MAKEWPARAM(ssACTION, sendL), (LPARAM)sendB);
}

void CPage3::parsingData(char* datB, int datL)
{
	CString	string, stringx;

	string = CString(datB, datL);
	for (int ii = 0; !string.IsEmpty(); ii++)
	{
		stringx = parseX(string, tabCHAR);

		switch (ii)
		{
		case 0: continue;
		case 1: m_ssCurr.SetText(stringx);	break;
		case 2:	m_ssDiff.SetText(stringx);	break;
		case 3: m_ssRate.SetText(stringx);	break;
		case 4: m_ssAsk.SetText(stringx);	break;
		case 5: m_ssBid.SetText(stringx);	break;
		case 6: m_ssVol.SetText(stringx);	break;
		}
	}

	return;
}

///////////////////////////////////////////////////////////////////////////////
// 
BOOL CPage3::AddColumn(CListCtrl *ctrl, LPCTSTR string, int item, int width)
{
	LV_COLUMN lvc;

	lvc.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt     = LVCFMT_LEFT;
	lvc.pszText = (LPTSTR) string;
	lvc.cx      = ctrl->GetStringWidth(lvc.pszText) + width;

	return ctrl->InsertColumn(item, &lvc);
}

int CPage3::AddItem(CListCtrl *ctrl, LPCSTR string, int nItem, int nSubItem)
{
	if (nSubItem < 0) // code
		return ctrl->InsertItem(LVIF_TEXT, nItem, string, nullptr, nullptr, 0, 0);
	else		  // name
		return ctrl->SetItemText(nItem, nSubItem, string);
}

bool CPage3::ExistFile(int gno, bool temp /*=false*/)
{
	CString	filePath;

	if (temp)
		filePath.Format("%s/%s/%s/portfolio.i%02d.tmp", m_root, USRDIR, m_name, gno);
	else
		filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_name, gno);

	DWORD	fLen = 0;
	CFileFind finder;
	if (!finder.FindFile(filePath))
		return false;

	return true;
}

CString CPage3::parseX(CString &srcstr, CString substr)
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

void CPage3::sendTR(CString trCode, char* datB, int datL, int key)
{
	char* sendB = new char [L_userTH + datL + 1];

	struct	_userTH* uTH;
	uTH = (struct _userTH *) sendB;

	strcpy(uTH->trc, trCode);
	uTH->key = key;
	uTH->stat = US_PASS;

	CopyMemory(&sendB[L_userTH], datB, datL);
	m_parent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datL), (LPARAM) sendB);

	delete [] sendB;
}

CFont* CPage3::getAxFont(CString fName, int point, int style)
{
	struct _fontR fontR;

	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point; fontR.italic = false; fontR.bold = 0;

	switch (style)
	{
	case 0: case 1: // none
		break;
	case 2: // italic
		fontR.italic = true;
		break;
	case 3: // bold
		fontR.bold = FW_BOLD;
		break;
	case 4: // both
		fontR.italic = true;
		fontR.bold = FW_BOLD;
		break;
	}
	return (CFont*)m_parent->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CString CPage3::Variant(int comm, CString data)
{
	CString retvalue;
	char* dta = (char*)m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";

	return retvalue;
}

bool CPage3::GuideMsg(CString msg, bool question)
{
	UINT nType = MB_OK|MB_ICONEXCLAMATION;
	if (question)
		nType = MB_OKCANCEL|MB_ICONEXCLAMATION;

	if (MessageBox(msg, idTOstring(IDS_REGISTERINTER), nType) == IDCANCEL)
		return false;

	return true;
}

CString CPage3::idTOstring(UINT id)
{
	CString	msg;
	if (!msg.LoadString(id))
		msg = _T("");
	return msg;
}

CString CPage3::chTOstring(char* chB, int chL)
{
	CString string = CString(chB, chL);
	string.TrimRight(); return string;
}

