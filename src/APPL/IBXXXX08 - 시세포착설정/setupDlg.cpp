#include "stdafx.h"
#include "setupDlg.h"
#include "libDefine.h"
#include "../../h/fxCommonx.h"
#include "../../h/axisfire.h"
#include <MMSYSTEM.H>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct _grInfo grInfoSetupTitle[] = {
	{ "종목코드",	70,	"1301",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	"",	0},
	{ "",		90,	"2024",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONTRAST,	"",	0},
	{ "종목명",	70,	"1022",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	"",	0},
	{ "",		180,	"2026",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	"",	0},
};
enum { ITCOL_CODET = 0, ITCOL_CODE, ITCOL_NAMET, ITCOL_NAME };
// sise
struct _grInfo grInfoSetupSise[] = {
	{ "현재가",	66,	"2023",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	"",	0},
	{ "전일대비",	66,	"2024",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONTRAST,	"",	0},
	{ "매도호가",	66,	"2025",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	"",	0},
	{ "매수호가",	66,	"2026",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	"",	0},
	{ "등락률",	63,	"2033",	GVFM_RIGHT,	GVAT_CONDITION,			"",	0},
	{ "거래량",	79,	"2027",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	"",	0},
};
enum { IUCOL_CURR = 0, IUCOL_DIFF, IUCOL_MDGA, IUCOL_MSGA, IUCOL_RATE, IUCOL_GVOL };

struct	_enableW {
	UINT	chkidc;
	UINT	ids[5];
} enableW[] = {
	{ IDC_CHK_CURR, IDC_SPINCURR, IDC_RUP_CURR, IDC_RDN_CURR, 0, 0}, 
	{ IDC_CHK_DIFF, IDC_SPINDIFF, IDC_RUP_DIFF, IDC_RDN_DIFF, IDC_AND_DIFF, IDC_OR_DIFF },
	{ IDC_CHK_MDGA, IDC_SPINMDGA, IDC_RUP_MDGA, IDC_RDN_MDGA, IDC_AND_MDGA, IDC_OR_MDGA },
	{ IDC_CHK_MSGA, IDC_SPINMSGA, IDC_RUP_MSGA, IDC_RDN_MSGA, IDC_AND_MSGA, IDC_OR_MSGA },
	{ IDC_CHK_RATE, IDC_SPINRATE, IDC_RUP_RATE, IDC_RDN_RATE, IDC_AND_RATE, IDC_OR_RATE },
	{ IDC_CHK_GVOL, IDC_SPINGVOL, IDC_RUP_GVOL, IDC_RDN_GVOL, IDC_AND_GVOL, IDC_OR_GVOL },
	{ IDC_CHK_CONFIRM, 0, 0, 0, 0, 0 },
	{ IDC_CHK_SIGNAL, 0, 0, 0, 0, 0 },
	{ IDC_CHK_WAVE, IDC_EDIT_WAVE, IDC_BTN_SEARCH, IDC_BTN_LISTEN, 0, 0 },
	{ IDC_CHK_JUMUN, IDC_CBJMGB, IDC_CBMMGB, IDC_SPINJMSU, IDC_SPINJMGA, 0 },
};
enum { W_SPIN = 0, W_UP, W_DN, W_AND, W_OR };

struct	_radioUD {
	UINT	idc[2];
} radioUD[] = {
	{ IDC_RUP_CURR, IDC_RDN_CURR },
	{ IDC_RUP_DIFF, IDC_RDN_DIFF },
	{ IDC_RUP_MDGA, IDC_RDN_MDGA },
	{ IDC_RUP_MSGA, IDC_RDN_MSGA },
	{ IDC_RUP_RATE, IDC_RDN_RATE },
	{ IDC_RUP_GVOL, IDC_RDN_GVOL }
};

struct	_radioAO {
	UINT	idc[2];
} radioAO[] = {
	{ 0,		0 },
	{ IDC_AND_DIFF, IDC_OR_DIFF },
	{ IDC_AND_MDGA, IDC_OR_MDGA },
	{ IDC_AND_MSGA, IDC_OR_MSGA },
	{ IDC_AND_RATE, IDC_OR_RATE },
	{ IDC_AND_GVOL, IDC_OR_GVOL }
};
enum { IDX_CURR = 0, IDX_DIFF, IDX_MDGA, IDX_MSGA, IDX_RATE, IDX_GVOL, IDX_CONFIRM, IDX_SIGNAL, IDX_WAVE, IDX_JUMUN };

enum { MMGB_SIJANG = 0, MMGB_JIJUNG };		// m_cbMmgb : 0:시장가, 1:지정가
enum { JMGB_MD = 0, JMGB_MS };			// m_cbJmgb : 0:매도, 1:매수

CSetupDlg::CSetupDlg(CWnd* pParent, CWnd* pView, CString strCodeName, CString strPriceValue)
	: CDialog(CSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetupDlg)
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_pView = pView;
	
	m_sCodeName = strCodeName;
	m_sPriceValue = strPriceValue;

	initConstruct();
}

void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupDlg)
	DDX_Control(pDX, IDC_BTN_LISTEN, m_ListenBtn);
	DDX_Control(pDX, IDC_BTN_SEARCH, m_SearchBtn);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_CancelBtn);
	DDX_Control(pDX, IDC_BTN_OK, m_OkBtn);
	DDX_Control(pDX, IDC_SPINJMGA, m_spinJmga);
	DDX_Control(pDX, IDC_SPINJMSU, m_spinJmsu);
	DDX_Control(pDX, IDC_SPINRATE, m_spinRate);
	DDX_Control(pDX, IDC_SPINMSGA, m_spinMsga);
	DDX_Control(pDX, IDC_SPINMDGA, m_spinMdga);
	DDX_Control(pDX, IDC_SPINGVOL, m_spinGvol);
	DDX_Control(pDX, IDC_SPINDIFF, m_spinDiff);
	DDX_Control(pDX, IDC_SPINCURR, m_spinCurr);
	DDX_Control(pDX, IDC_CHK_ORDEREXC, m_OrderExcBtn);
	DDX_Control(pDX, IDC_CBMMGB, m_cbMmgb);
	DDX_Control(pDX, IDC_CBJMGB, m_cbJmgb);
	DDX_Control(pDX, IDC_CBORDER_TYPE, m_cbOrderType);
	DDX_Control(pDX, IDC_CBDEALING_TYPE, m_cbDealingType);
	//}}AFX_DATA_MAP
}

void CSetupDlg::initConstruct()
{
	m_gubn = CG_NOT;
	m_bAdd = true;
	m_pSetupSise = nullptr;
	//m_pPrice = nullptr;
	memset(&m_codeinfo, 0x00, sz_codeinfo);
}

void CSetupDlg::destroy()
{
	//if (m_pPrice)		LIB_DELETE(m_pPrice);
}

BEGIN_MESSAGE_MAP(CSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CSetupDlg)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_CBJMGB, OnSelchangeCbjmgb)
	ON_CBN_SELCHANGE(IDC_CBMMGB, OnSelchangeCbmmgb)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	ON_BN_CLICKED(IDC_BTN_LISTEN, OnBtnListen)
	ON_BN_CLICKED(IDC_CXPRICE, OnCxprice)
	ON_BN_CLICKED(IDC_CHK_ORDEREXC, OnChkOrderexc)
	ON_CBN_SELCHANGE(IDC_CBDEALING_TYPE, OnSelchangeCbdealingType)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
	ON_WM_CREATE()
	ON_EN_CHANGE(IDC_ORDERCOST_EDIT, OnChangeOrdercostEdit)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVNM_LMOUSEDOWN, IDC_GRSETUPSISE, OnGrSiseLMousedown)
END_MESSAGE_MAP()

void CSetupDlg::OnGrSiseLMousedown(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GVITEM *pgvitem = (NM_GVITEM*)pNMHDR;

	const int col = pgvitem->col;
	CString tmpS = m_pSetupSise->GetItemText(SISEROW, col);
	tmpS = getColString(col, tmpS);
	if (((CButton *)GetDlgItem(enableW[col].chkidc))->GetCheck())
		((CExSpin *)GetDlgItem(enableW[col].ids[W_SPIN]))->SetText(tmpS);

	if (col == IDX_CURR || col == IDX_MDGA || col == IDX_MSGA)
	{
		if (((CButton *)GetDlgItem(IDC_CHK_JUMUN))->GetCheck() && m_cbMmgb.GetCurSel() == MMGB_JIJUNG)
		{
			// LKM
			//((CExSpin *)GetDlgItem(IDC_SPINJMGA))->SetText(tmpS);
			GetDlgItem(IDC_SPINJMGA)->SetWindowText(tmpS);
		}
	}
}


BOOL CSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (m_bAdd)
		SetWindowText("시세포착설정[추가]");
	else
		SetWindowText("시세포착설정[수정]");
	

	m_sImgDir.Format("%s\\image\\", m_root);
	m_clrBk = GetIndexColor(CLRBG);
	m_clrWriteBk = GetIndexColor(181);
	m_clrRoundBk = GetIndexColor(66);
	m_br.CreateSolidBrush(m_clrRoundBk);

	initControl();
	m_spinMsga.SetRoot(m_root);
	m_spinMdga.SetRoot(m_root);
	m_spinCurr.SetRoot(m_root);
	return TRUE; 
}


void CSetupDlg::OnDestroy() 
{
	destroy();
	CDialog::OnDestroy();
}


void CSetupDlg::OnOK() 
{
	if (!checkInfo())
		return;
	saveInfo();
	CDialog::OnOK();
}

COLORREF CSetupDlg::GetIndexColor(int index)
{
	return m_pParent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

BOOL CSetupDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	const UINT idc = (UINT)wParam;
	switch (idc)
	{
	case IDC_CHK_CURR:
		enableItems(IDX_CURR, ((CButton *)GetDlgItem(idc))->GetCheck(), false);	break;
	case IDC_CHK_DIFF:
		enableItems(IDX_DIFF, ((CButton *)GetDlgItem(idc))->GetCheck(), false);	break;
	case IDC_CHK_MDGA:
		enableItems(IDX_MDGA, ((CButton *)GetDlgItem(idc))->GetCheck(), false);	break;
	case IDC_CHK_MSGA:
		enableItems(IDX_MSGA, ((CButton *)GetDlgItem(idc))->GetCheck(), false);	break;
	case IDC_CHK_RATE:
		enableItems(IDX_RATE, ((CButton *)GetDlgItem(idc))->GetCheck(), false);	break;
	case IDC_CHK_GVOL:
		enableItems(IDX_GVOL, ((CButton *)GetDlgItem(idc))->GetCheck(), false);	break;
		break;

	case IDC_RUP_CURR:	case IDC_RDN_CURR:
		radioUDSel(IDX_CURR, idc);	break;
	case IDC_RUP_DIFF:	case IDC_RDN_DIFF:
		radioUDSel(IDX_DIFF, idc);	break;
	case IDC_RUP_MDGA:	case IDC_RDN_MDGA:
		radioUDSel(IDX_MDGA, idc);	break;
	case IDC_RUP_MSGA:	case IDC_RDN_MSGA:
		radioUDSel(IDX_MSGA, idc);	break;
	case IDC_RUP_RATE:	case IDC_RDN_RATE:
		radioUDSel(IDX_RATE, idc);	break;
	case IDC_RUP_GVOL:	case IDC_RDN_GVOL:
		radioUDSel(IDX_GVOL, idc);	break;

	case IDC_AND_DIFF:	case IDC_OR_DIFF:
		radioAOSel(IDX_DIFF, idc);	break;
	case IDC_AND_MDGA:	case IDC_OR_MDGA:
		radioAOSel(IDX_MDGA, idc);	break;
	case IDC_AND_MSGA:	case IDC_OR_MSGA:
		radioAOSel(IDX_MSGA, idc);	break;
	case IDC_AND_RATE:	case IDC_OR_RATE:
		radioAOSel(IDX_RATE, idc);	break;
	case IDC_AND_GVOL:	case IDC_OR_GVOL:
		radioAOSel(IDX_GVOL, idc);	break;

	case IDC_CHK_CONFIRM:
		enableItems(IDX_CONFIRM, ((CButton *)GetDlgItem(idc))->GetCheck(), false);	break;
	case IDC_CHK_SIGNAL:
		enableItems(IDX_SIGNAL, ((CButton *)GetDlgItem(idc))->GetCheck(), false);	break;
	case IDC_CHK_WAVE:
		enableItems(IDX_WAVE, ((CButton *)GetDlgItem(idc))->GetCheck(), false);	break;
	case IDC_CHK_JUMUN:
		enableItems(IDX_JUMUN, ((CButton *)GetDlgItem(idc))->GetCheck(), false);	break;

	}
	return CDialog::OnCommand(wParam, lParam);
}

void CSetupDlg::OnSelchangeCbjmgb() 
{	
}

void CSetupDlg::OnSelchangeCbmmgb() 
{
	const int sel = m_cbMmgb.GetCurSel();
	if (sel == MMGB_SIJANG)
	{
		m_spinJmga.EnableWindow(FALSE);
		m_spinJmga.SetText("");
	}
	else
	{
		m_spinJmga.EnableWindow(TRUE);
		m_spinJmga.SetFocus();
	}
}

void CSetupDlg::OnBtnSearch() 
{
	CString	pathS;
	GetDlgItem(IDC_EDIT_WAVE)->GetWindowText(pathS);

	CFileDialog fileDlg(true, nullptr, pathS, 0, "wave file(*.wav)|*.wav||");
	if (fileDlg.DoModal() == IDOK)
		GetDlgItem(IDC_EDIT_WAVE)->SetWindowText(fileDlg.GetPathName());
}

void CSetupDlg::OnBtnListen() 
{
	CString	pathS;
	GetDlgItem(IDC_EDIT_WAVE)->GetWindowText(pathS);
	if (!pathS.IsEmpty()) sndPlaySound(pathS, SND_ASYNC);
	
}



//=================================================================================================
void CSetupDlg::initControl()
{
	m_strPal   = GetPalette();
	m_clrBlink = GetColorA(m_pView, CLRBLINK);

	// spin edit
	const struct _spininfo {
		UINT	idc;
		DWORD	attr;
		double	minV;
		double	maxV;
	} spininfo[] = {
		{ IDC_SPINCURR, attrNUMERIC|attrCOMMA, 0,	9999999},
		{ IDC_SPINDIFF, attrNUMERIC|attrCOMMA, -999999,	999999 },
		{ IDC_SPINMDGA, attrNUMERIC|attrCOMMA, 0,	9999999},
		{ IDC_SPINMSGA, attrNUMERIC|attrCOMMA, 0,	9999999},
		{ IDC_SPINRATE, attrFLOAT,		-999,	999  },
		{ IDC_SPINGVOL, attrNUMERIC|attrCOMMA, 0,	99999999 },
		{ IDC_SPINJMSU, attrNUMERIC|attrCOMMA, 0,	999999 },
		{ IDC_SPINJMGA, attrNUMERIC|attrCOMMA, 0,	99999999 },
	};
	const struct _spininfoFuture {
		UINT	idc;
		DWORD	attr;
		double	minV;
		double	maxV;
	} spininfoFuture[] = {
		{ IDC_SPINCURR, attrFLOAT|attrCOMMA, 0,	9999999},
		{ IDC_SPINDIFF, attrFLOAT|attrCOMMA, -999999,	999999 },
		{ IDC_SPINMDGA, attrFLOAT|attrCOMMA, 0,	9999999},
		{ IDC_SPINMSGA, attrFLOAT|attrCOMMA, 0,	9999999},
		{ IDC_SPINRATE, attrFLOAT,		-999,	999  },
		{ IDC_SPINGVOL, attrFLOAT|attrCOMMA, 0,	99999999 },
		{ IDC_SPINJMSU, attrFLOAT|attrCOMMA, 0,	999999 },
		{ IDC_SPINJMGA, attrFLOAT|attrCOMMA, 0,	99999999 },
	};
	const struct _spininfoOption {
		UINT	idc;
		DWORD	attr;
		double	minV;
		double	maxV;
	} spininfoOption[] = {
		{ IDC_SPINCURR, attrFLOAT|attrCOMMA, 0,	9999999},
		{ IDC_SPINDIFF, attrFLOAT|attrCOMMA, -999999,	999999 },
		{ IDC_SPINMDGA, attrFLOAT|attrCOMMA, 0,	9999999},
		{ IDC_SPINMSGA, attrFLOAT|attrCOMMA, 0,	9999999},
		{ IDC_SPINRATE, attrFLOAT,		-999,	999  },
		{ IDC_SPINGVOL, attrFLOAT|attrCOMMA, 0,	99999999 },
		{ IDC_SPINJMSU, attrFLOAT|attrCOMMA, 0,	999999 },
		{ IDC_SPINJMGA, attrFLOAT|attrCOMMA, 0,	99999999 },
	};

	DWORD sptype = spNOT;
	switch (m_gubn)
	{
	case CG_KOSPI:
		sptype = spKOSPI;	break;
	case CG_KOSDAQ:
		sptype = spKOSDAQ;	break;
	case CG_FUTURE:
		sptype = spFUTURE;	break;
	case CG_CALL:
	case CG_PUT:
		sptype = spOPTION;	break;
	case etfType:
		sptype = spETF;	break;
	case etnType:
		sptype = spETN;	break;
	}

	for (int ii = 0; ii < sizeof(spininfo)/sizeof(_spininfo); ii++)
	{
		switch (spininfo[ii].idc)
		{
		case IDC_SPINRATE:
			((CExSpin *)GetDlgItem(spininfo[ii].idc))->Init(spininfoOption[ii].attr, spRATE, this);
			((CExSpin *)GetDlgItem(spininfo[ii].idc))->SetDecimalPlaces(2);
			break;
		case IDC_SPINGVOL:
		case IDC_SPINJMSU:
			((CExSpin *)GetDlgItem(spininfo[ii].idc))->Init(spininfoOption[ii].attr, spNOT, this);
			break;
		default:
			((CExSpin *)GetDlgItem(spininfo[ii].idc))->Init(spininfoOption[ii].attr, sptype, this);
			break;
		}
		
		((CExSpin *)GetDlgItem(spininfo[ii].idc))->SetText("");
		((CExSpin *)GetDlgItem(spininfo[ii].idc))->SetRangeEx(spininfo[ii].minV, spininfo[ii].maxV);
		((CExSpin *)GetDlgItem(spininfo[ii].idc))->SetOwner(this);
	}

	// 초기화(disable)
	const UINT idcs[] = {
		IDC_SPINCURR, IDC_RUP_CURR, IDC_RDN_CURR, 
		IDC_SPINDIFF, IDC_RUP_DIFF, IDC_RDN_DIFF, IDC_AND_DIFF, IDC_OR_DIFF,
		IDC_SPINMDGA, IDC_RUP_MDGA, IDC_RDN_MDGA, IDC_AND_MDGA, IDC_OR_MDGA,
		IDC_SPINMSGA, IDC_RUP_MSGA, IDC_RDN_MSGA, IDC_AND_MSGA, IDC_OR_MSGA,
		IDC_SPINRATE, IDC_RUP_RATE, IDC_RDN_RATE, IDC_AND_RATE, IDC_OR_RATE,
		IDC_SPINGVOL, IDC_RUP_GVOL, IDC_RDN_GVOL, IDC_AND_GVOL, IDC_OR_GVOL,
		IDC_BTN_SEARCH, IDC_BTN_LISTEN,
		IDC_CBJMGB, IDC_CBMMGB, IDC_SPINJMSU, IDC_SPINJMGA,
	};
	for (int ii = 0; ii < sizeof(idcs)/sizeof(UINT); ii++)
	{
		GetDlgItem(idcs[ii])->EnableWindow(FALSE);
	}

	// 코드grid
	CRect rc;
	GetDlgItem(IDC_POSSETUPTITLE)->GetWindowRect(rc);
	ScreenToClient(rc);
	m_pSetupTitle = std::make_unique<CExGrid>();
	m_pSetupTitle->Create(rc, this, IDC_GRSETUPTITLE, GVSC_NONE, 0, WS_CHILD|WS_VISIBLE);
	initGrid(m_pSetupTitle.get(), IDC_GRSETUPTITLE);
	
	// 시세grid
	GetDlgItem(IDC_POSSETUPSISE)->GetWindowRect(rc);
	ScreenToClient(rc);
	m_pSetupSise = std::make_unique<CExGrid>();
	m_pSetupSise->Create(rc, this, IDC_GRSETUPSISE, GVSC_NONE, 0, WS_CHILD|WS_VISIBLE);
	initGrid(m_pSetupSise.get(), IDC_GRSETUPSISE);
	((CEdit*)GetDlgItem(IDC_EDIT_WAVE))->GetRect(rc);
	rc.top += 15;
	((CEdit*)GetDlgItem(IDC_EDIT_WAVE))->SetRect(rc);

	setImgBtn(&m_SearchBtn, 1, TRUE);
	setImgBtn(&m_ListenBtn, 2, TRUE);
	setImgBtn(&m_OkBtn, 4, TRUE);
	setImgBtn(&m_CancelBtn, 4, TRUE);

	// combo
	m_cbJmgb.AddString("매도");	m_cbJmgb.AddString("매수");	m_cbJmgb.SetCurSel(JMGB_MD); 
	m_cbMmgb.AddString("시장가");	m_cbMmgb.AddString("지정가");	m_cbMmgb.SetCurSel(MMGB_SIJANG); 
	
	//+ 요건 번호 : 0014 [20090513:kbkim]
	m_cbOrderType.AddString("매도");	m_cbOrderType.AddString("매수");	  m_cbOrderType.SetCurSel(JMGB_MD); 
	m_cbDealingType.AddString("보통");	m_cbDealingType.AddString("시장가");	m_cbDealingType.SetCurSel(MMGB_SIJANG); 
	//- 

	// cx_price
	/*
	GetDlgItem(IDC_CXPRICE)->GetWindowRect(rc);
	ScreenToClient(rc);
	
	m_pPrice = new CCxPrice();
	m_pPrice->m_sRoot = m_root;
	m_pPrice->m_pParent = m_pView;
	m_pPrice->Create(nullptr, nullptr, WS_CHILD|WS_VISIBLE, rc, this, -1);
	m_pPrice->CreatePrice();
	//m_pPrice->SetWindowPos(nullptr, rc.right, rc.top, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	*/

	updateSise();
	updateInfo();

	OnChkOrderexc();
}

void CSetupDlg::SetCodeInfo(bool bAdd, CString siseS, _codeinfo *codeinfo, int gubn)
{
	m_bAdd = bAdd;
	m_siseS = siseS;
	m_gubn = gubn;
	memcpy(&m_codeinfo, codeinfo, sz_codeinfo);

	// default 관심종목으로 신호보내기 선택
	if (m_bAdd)
	{
		m_codeinfo.a_kind[0] |= 0x01;		// check confirm
		m_codeinfo.a_kind[0] |= 0x02;		// check signal
		m_codeinfo.o_mmgb[0] = '0';		// 지정가
	}
}

void CSetupDlg::GetCodeInfo(_codeinfo *codeinfo)
{
	memcpy(codeinfo, &m_codeinfo, sz_codeinfo);
}

void CSetupDlg::Alert(const CMapStringToString &rtmStore)
{
	if (!m_pSetupSise)	return;
	alertSise(rtmStore, "023", IUCOL_CURR);	// 현재가
	alertSise(rtmStore, "024", IUCOL_DIFF);	// 대비
	alertSise(rtmStore, "025", IUCOL_MDGA);	// 매도호가
	alertSise(rtmStore, "026", IUCOL_MSGA);	// 매수호가
	alertSise(rtmStore, "027", IUCOL_GVOL);	// 거래량
	alertSise(rtmStore, "033", IUCOL_RATE);	// 등락률
}

void CSetupDlg::alertSise(const CMapStringToString &rtmStore, CString symS, int col)
{
	if (m_pSetupSise && m_pSetupSise->GetSafeHwnd())
	{
		CString dataS;
		if (rtmStore.Lookup(symS, dataS) && m_pSetupSise->GetItemText(SISEROW, col) != dataS)
		{
			m_pSetupSise->SetItemText(SISEROW, col, dataS);
			m_pSetupSise->Blink(SISEROW, col, m_clrBlink);
		}
	}
}

void CSetupDlg::SetFont(LOGFONT lf, CFont *pFont)
{
	m_lf = lf;
	m_pFont = pFont;
}

void CSetupDlg::SetBFont(LOGFONT blf, CFont *pBFont)
{
	m_blf = blf;
	m_pBFont = pBFont;
}


void CSetupDlg::initGrid(CExGrid* pGrid, UINT idc)
{
	_GVITEM gvitem;
	COLORREF gridCodeHead{};
	COLORREF gridHead{};
	CFont* pFont = nullptr;
	int ii = 0;
	int colCnt = 0;
	
	gridCodeHead = GetColorA(m_pView, 96);
	gridHead = GetColorA(m_pView, CLRGRHB);

	gvitem.row = 0;
	gvitem.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_FGCOLOR|GVMK_BKCOLOR;
	gvitem.mask |= GVMK_ATTR|GVMK_MISC|GVMK_PARAM;
	
	gvitem.font = m_lf;
	gvitem.format = GVFM_HEADER;
	gvitem.fgcol = GetColorA(m_pView, CLRGRHF);
	
	pGrid->SetGridFocusLine(FALSE, FALSE);
	switch (idc)
	{
	case IDC_GRSETUPTITLE:
		colCnt = sizeof(grInfoSetupTitle)/sizeof(_grInfo);
		pGrid->Initial(1, colCnt, 0, 0);
		for (ii = 0; ii < colCnt; ii++)
		{
			if (ii % 2 == 0)
			{
				gvitem.font = m_lf;
				gvitem.bkcol = gridCodeHead;
			}
			else
			{
				gvitem.font = m_blf;
				gvitem.bkcol = GetColorA(m_pView, CLRGRROWB1);
			}
			gvitem.symbol = grInfoSetupTitle[ii].symbols;
			gvitem.text = grInfoSetupTitle[ii].heads;
			gvitem.attr = 0;
			gvitem.col = ii;
			if(ii == 1)
			{
				gvitem.text = m_sCodeName;
				gvitem.fgcol = RGB(0, 0, 0);
			}
			else if(ii == 3)
			{
				gvitem.text = m_sPriceValue;
				gvitem.fgcol = RGB(0, 0, 0);
			}
			else
				gvitem.fgcol = RGB(0, 0, 0);

			pGrid->SetItem(&gvitem);
			pGrid->SetColumnWidth(ii, grInfoSetupTitle[ii].width);
		}
		pGrid->SetRowHeight(0, GRIDROW_HEIGHT+4);
		break;
	case IDC_GRSETUPSISE:
		colCnt = sizeof(grInfoSetupSise)/sizeof(_grInfo);
		pGrid->Initial(1, colCnt, 1, 0);
		for (ii = 0; ii < colCnt; ii++)
		{
			gvitem.bkcol = gridHead;
			gvitem.symbol = grInfoSetupSise[ii].symbols;
			gvitem.text = grInfoSetupSise[ii].heads;
			gvitem.attr = 0;
			gvitem.col = ii;
			pGrid->SetItem(&gvitem);
			pGrid->SetColumnWidth(ii, grInfoSetupSise[ii].width);
		}
		pGrid->SetRowHeight(0, GRIDROW_HEIGHT);
		insertGrid(pGrid, idc, SISEROW);
		break;
	}
	
	
	pGrid->SetRowColResize(FALSE, FALSE);
	pGrid->SetBKSelColor(GetColorA(m_pView, CLRGRSEL));
	pGrid->SetGridColor(GetColorA(m_pView, CLRGRLINE));
	pGrid->SetBkColor(GetColorA(m_pView, CLRGRROWB1));
	pGrid->Adjust();
}

int CSetupDlg::insertGrid(CExGrid* pGrid, UINT idc, int row)
{
	_GVITEM gvitem;
	CString	 tmpS, statS;

	gvitem.font   = m_lf;
	gvitem.mask   = GVMK_FONT|GVMK_TEXT|GVMK_FORMAT|GVMK_PARAM|GVMK_ATTR|GVMK_BKCOLOR|GVMK_FGCOLOR;
	gvitem.bkcol = GetColorA(m_pView, CLRGRROWB1);
	gvitem.fgcol  = GetColorA(m_pView, CLRGRROWF);

	row = pGrid->InsertRow(" ", row);
	pGrid->SetRowHeight(row, GRIDROW_HEIGHT);
	gvitem.row    = row;

	int col = 0;
	switch (idc)
	{
	case IDC_GRSETUPSISE:
		for (col = 0; col < sizeof(grInfoSetupSise)/sizeof(_grInfo); col++)
		{
			gvitem.text = "";
			gvitem.col = col;
			gvitem.format = grInfoSetupSise[col].format;
			gvitem.attr = grInfoSetupSise[col].attr;
			pGrid->SetItem(&gvitem);
		}
		break;
	}
	return row;
}


void CSetupDlg::updateSise()
{
	if (!m_pSetupTitle || !m_pSetupSise)	return;

	CString dataS = m_siseS;
	CString tmpS;
	int pos = 0;
	while (!dataS.IsEmpty())
	{
		tmpS = parseX(dataS, "\t");
		switch (pos)
		{
		case 0:	// 종목코드
				m_pSetupTitle->SetItemText(0, ITCOL_CODE, tmpS);		break;
		case 1:	// 종목명
				m_pSetupTitle->SetItemText(0, ITCOL_NAME, tmpS);		break;
		case 2:	// 현재가
			m_pSetupSise->SetItemText(SISEROW, IUCOL_CURR, tmpS);		break;
		case 3:	// 대비
			m_pSetupSise->SetItemText(SISEROW, IUCOL_DIFF, tmpS);		break;
		case 4:	// 매도호가
			m_pSetupSise->SetItemText(SISEROW, IUCOL_MDGA, tmpS);		break;
		case 5:	// 매수호가
			m_pSetupSise->SetItemText(SISEROW, IUCOL_MSGA, tmpS);		break;
		case 6:	// 등락률
			m_pSetupSise->SetItemText(SISEROW, IUCOL_RATE, tmpS);		break;
		case 7:	// 거래량
			m_pSetupSise->SetItemText(SISEROW, IUCOL_GVOL, tmpS);		break;
		
		}
		pos++;
	}
}

void CSetupDlg::updateInfo()
{
	for (int ii = 0; ii < sizeof(m_codeinfo.alarm)/sizeof(m_codeinfo.alarm[0]); ii ++)
	{
		if (m_codeinfo.alarm[ii].kind[0] >= '1')
			enableItems(ii, TRUE, true);
		else
			enableItems(ii, FALSE, true);
	}

	if (m_codeinfo.a_kind[0] & 0x01)	// 확인창
		enableItems(IDX_CONFIRM, TRUE, true);
	else
		enableItems(IDX_CONFIRM, FALSE, true);

	if (m_codeinfo.a_kind[0] & 0x02)	// 관심종목신호적용
		enableItems(IDX_SIGNAL, TRUE, true);
	else
		enableItems(IDX_SIGNAL, FALSE, true);

	if (m_codeinfo.a_kind[0] & 0x04)	// 소리적용
		enableItems(IDX_WAVE, TRUE, true);
	else
		enableItems(IDX_WAVE, FALSE, true);

	if (m_codeinfo.o_run[0] == '1')
	{
		enableItems(IDX_JUMUN, TRUE, true);
	}
	else
	{
		enableItems(IDX_JUMUN, FALSE, true);
	}	
}

void CSetupDlg::enableItems(int idx, BOOL bEnable, bool bInit)
{
	if (idx < 0 || idx >= sizeof(enableW)/sizeof(struct _enableW))	return;
	CString code = CString(m_codeinfo.code, sizeof(m_codeinfo.code));
	code.TrimRight();
	CString tmpS;
	for (int ii = 0; ii < sizeof(enableW[idx].ids)/sizeof(UINT); ii++)
	{
		if (enableW[idx].ids[ii] == 0)	continue;
		GetDlgItem(enableW[idx].ids[ii])->EnableWindow(bEnable);
	}
	if (bInit)
	{
		if (idx >= IDX_CURR && idx <= IDX_GVOL)
		{
			// check여부
			if (m_codeinfo.alarm[idx].kind[0] >= '1')
				((CButton *)GetDlgItem(enableW[idx].chkidc))->SetCheck(TRUE);
			else
				((CButton *)GetDlgItem(enableW[idx].chkidc))->SetCheck(FALSE);

			// 조건값
			tmpS = getString(m_codeinfo.alarm[idx].aval, sizeof(m_codeinfo.alarm[idx].aval));
			((CExSpin *)GetDlgItem(enableW[idx].ids[W_SPIN]))->SetText(tmpS);
				
			// 이상/이하
			if (m_codeinfo.alarm[idx].cond[0] == '1')	// 이하
			{
				((CButton *)GetDlgItem(enableW[idx].ids[W_DN]))->SetCheck(TRUE);
				((CButton *)GetDlgItem(enableW[idx].ids[W_UP]))->SetCheck(FALSE);
			}
			else
			{
				((CButton *)GetDlgItem(enableW[idx].ids[W_DN]))->SetCheck(FALSE);
				((CButton *)GetDlgItem(enableW[idx].ids[W_UP]))->SetCheck(TRUE);
			}
			
			// AND/OR
			if (idx != 0)		// 현재가는 AND/OR없음
			{
				if (m_codeinfo.alarm[idx].oper[0] == '1')	// OR
				{
					((CButton *)GetDlgItem(enableW[idx].ids[W_OR]))->SetCheck(TRUE);
					((CButton *)GetDlgItem(enableW[idx].ids[W_AND]))->SetCheck(FALSE);
				}
				else
				{
					((CButton *)GetDlgItem(enableW[idx].ids[W_OR]))->SetCheck(FALSE);
					((CButton *)GetDlgItem(enableW[idx].ids[W_AND]))->SetCheck(TRUE);
				}
			}
		}
		else
		{
			((CButton *)GetDlgItem(enableW[idx].chkidc))->SetCheck(bEnable);

			if (idx == IDX_WAVE)
			{
				// 소리 file
				tmpS = getString(m_codeinfo.a_wave, sizeof(m_codeinfo.a_wave));
				if (tmpS.IsEmpty() && m_bAdd)
				{
					CString soundPath;
					soundPath.Format("%s\\image\\시세포착.wav", m_root);
					tmpS = soundPath;
				}
				GetDlgItem(IDC_EDIT_WAVE)->SetWindowText(tmpS);
			}
			if (idx == IDX_JUMUN)
			{
				// 주문구분
				if (m_codeinfo.o_kind[0] == '1')	// 매수
					m_cbJmgb.SetCurSel(JMGB_MS);
				else					// 매도
					m_cbJmgb.SetCurSel(JMGB_MD);

				// 매매구분
				if (m_codeinfo.o_mmgb[0] == '1')	// 시장가
				{
					m_cbMmgb.SetCurSel(MMGB_SIJANG);
					GetDlgItem(IDC_ORDERCOST_EDIT)->EnableWindow(FALSE);
				}
				else					// 지정가
				{
					m_cbMmgb.SetCurSel(MMGB_JIJUNG);
					GetDlgItem(IDC_ORDERCOST_EDIT)->EnableWindow(TRUE);
				}
				
				// 주문수량
				tmpS = getString(m_codeinfo.o_num, sizeof(m_codeinfo.o_num));
				((CExSpin *)GetDlgItem(IDC_SPINJMSU))->SetText(tmpS);

				// 주문단가
				tmpS = getString(m_codeinfo.o_prc, sizeof(m_codeinfo.o_prc));
				if (code.GetLength()==8 && !tmpS.IsEmpty())
					tmpS.Insert(tmpS.GetLength()-2, ".");
				GetDlgItem(IDC_ORDERCOST_EDIT)->SetWindowText(tmpS);
			}
		}
	}
	else
	{
		if (!m_pSetupSise)	return;
		if (idx >= IDX_CURR && idx <= IDX_GVOL)
		{
			if (((CButton *)GetDlgItem(enableW[idx].chkidc))->GetCheck())
			{
				tmpS = ((CExSpin *)GetDlgItem(enableW[idx].ids[W_SPIN]))->GetText();
				if (tmpS.IsEmpty() || atof(tmpS) == 0.0)
				{
					tmpS = m_pSetupSise->GetItemText(SISEROW, idx);
					tmpS = getColString(idx, tmpS);
					((CExSpin *)GetDlgItem(enableW[idx].ids[W_SPIN]))->SetText(tmpS);
				}
			}
			else
				((CExSpin *)GetDlgItem(enableW[idx].ids[W_SPIN]))->SetText("");

		}
		if (idx == IDX_JUMUN)
		{
			if ( ((CButton *)GetDlgItem(IDC_CHK_JUMUN))->GetCheck())
			{
				const int sel = m_cbMmgb.GetCurSel();
				if (sel == MMGB_SIJANG)
				{
					m_spinJmga.EnableWindow(FALSE);
				}
				else
				{
					m_spinJmga.EnableWindow(TRUE);
				}	
			}
		}

	}

	//+ 요건 번호 : 0014 [20090513:kbkim]
	// 주문구분
	if(m_codeinfo.o_run[0] == '1')
	{
		m_OrderExcBtn.SetCheck(BST_CHECKED);

		if (m_codeinfo.o_kind[0] == '1')	// 매수
			m_cbOrderType.SetCurSel(JMGB_MS+1);
		else if(m_codeinfo.o_kind[0] == '0') // 매도
			m_cbOrderType.SetCurSel(JMGB_MD+1);
		else
			m_cbOrderType.SetCurSel(-1);

		// 매매구분
		if (m_codeinfo.o_mmgb[0] == '1')	// 시장가
			m_cbDealingType.SetCurSel(MMGB_JIJUNG+1);
		else if(m_codeinfo.o_mmgb[0] == '0')	// 지정가
			m_cbDealingType.SetCurSel(MMGB_SIJANG+1);
		else
			m_cbDealingType.SetCurSel(-1);
		
		// 주문수량
		tmpS = getString(m_codeinfo.o_num, sizeof(m_codeinfo.o_num));
		GetDlgItem(IDC_ORDERCOUNT_EDIT)->SetWindowText(tmpS);

		// 주문단가
		tmpS = getString(m_codeinfo.o_prc, sizeof(m_codeinfo.o_prc));
		if (code.GetLength()==8 && tmpS.GetLength()>=2)
			tmpS.Insert(tmpS.GetLength()-2, ".");
		GetDlgItem(IDC_ORDERCOST_EDIT)->SetWindowText(tmpS);
	}

	OnChkOrderexc();
	//- 
}

void CSetupDlg::radioUDSel(int idx, UINT selidc)
{
	for (int ii = 0; ii < 2; ii++)
	{
		if (radioUD[idx].idc[ii] == 0)	return;

		if (radioUD[idx].idc[ii] == selidc)
			((CButton *)GetDlgItem(radioUD[idx].idc[ii]))->SetCheck(TRUE);
		else
			((CButton *)GetDlgItem(radioUD[idx].idc[ii]))->SetCheck(FALSE);
	}
}

void CSetupDlg::radioAOSel(int idx, UINT selidc)
{
	for (int ii = 0; ii < 2; ii++)
	{
		if (radioAO[idx].idc[ii] == 0)	return;

		if (radioAO[idx].idc[ii] == selidc)
			((CButton *)GetDlgItem(radioAO[idx].idc[ii]))->SetCheck(TRUE);
		else
			((CButton *)GetDlgItem(radioAO[idx].idc[ii]))->SetCheck(FALSE);
	}
}

bool CSetupDlg::checkInfo()
{
	CString tmpS;
	bool bCheck = false;
	for (int ii = IDX_CURR; ii <= IDX_GVOL; ii++)
	{
		if (((CButton *)GetDlgItem(enableW[ii].chkidc))->GetCheck())
			bCheck = true;
	}
	if (!bCheck)
	{
		MessageBox("설정된 조건이 없습니다. 조건을 설정하세요", "시세포착설정", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
		return false;
	}

	for (int ii = IDX_CURR; ii <= IDX_GVOL; ii++)
	{
		if (((CButton *)GetDlgItem(enableW[ii].chkidc))->GetCheck())
		{
			tmpS = ((CExSpin *)GetDlgItem(enableW[ii].ids[W_SPIN]))->GetText();
			if (tmpS.IsEmpty())
			{
				MessageBox("조건값을 설정하세요.", "시세포착설정", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
				GetDlgItem(enableW[ii].ids[W_SPIN])->SetFocus();
				return false;
			}
		}
	}

	if (((CButton *)GetDlgItem(IDC_CHK_WAVE))->GetCheck())
	{
		GetDlgItem(IDC_EDIT_WAVE)->GetWindowText(tmpS);
		if (tmpS.IsEmpty())
		{
			MessageBox("소리file이 지정되어 있지 않습니다.", "시세포착설정", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
			return false;
		}
	}

	if (((CButton *)GetDlgItem(IDC_CHK_JUMUN))->GetCheck())
	{
		// LKM
 		//tmpS = ((CExSpin *)GetDlgItem(IDC_SPINJMGA))->GetText();	// 주문가격
		GetDlgItem(IDC_ORDERCOST_EDIT)->GetWindowText(tmpS);	// 주문가격
		tmpS.Replace(".", "");

		const int nPrc = atoi(tmpS);

		if (m_cbMmgb.GetCurSel() == MMGB_JIJUNG)
		{
			if (tmpS.IsEmpty())
			{
				MessageBox("주문가격을 설정하세요.", "시세포착설정", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
				GetDlgItem(IDC_ORDERCOST_EDIT)->SetFocus();
				return false;
			}
			else if(nPrc <= 0)	//2017.01.20 KSJ 0이거나 0보다 작으면 선옵이라도 소숫점을 떼고 저장함.
			{
				MessageBox("주문가격을 다시 설청하세요(0 이하)", "시세포착설정", MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
				GetDlgItem(IDC_ORDERCOST_EDIT)->SetFocus();
				return false;				
			}
		}
	}
	return true;
}

void CSetupDlg::saveInfo()
{
	CString tmpS, code;
	_codeinfo codeinfo;
	memset(&codeinfo, ' ', sz_codeinfo);

	memcpy(&codeinfo.code, &m_codeinfo.code, sizeof(codeinfo.code));
	memcpy(&codeinfo.name, &m_codeinfo.name, sizeof(codeinfo.name));
	codeinfo.capture[0] = '0';
	
	code = CString(codeinfo.code, sizeof(codeinfo.code));
	code.TrimRight();

	for (int ii = IDX_CURR; ii <=  IDX_GVOL; ii++)
	{
		if (((CButton *)GetDlgItem(enableW[ii].chkidc))->GetCheck())
			codeinfo.alarm[ii].kind[0] = '1' + ii;
		else
			codeinfo.alarm[ii].kind[0] = '0';
		
		if (enableW[ii].ids[W_SPIN] != 0)
		{
			tmpS = ((CExSpin *)GetDlgItem(enableW[ii].ids[W_SPIN]))->GetText();
			memcpy(&codeinfo.alarm[ii].aval, tmpS, min(tmpS.GetLength(), sizeof(codeinfo.alarm[ii].aval)));
		}
		if (enableW[ii].ids[W_UP] != 0 && enableW[ii].ids[W_DN] != 0)
		{
			if (((CButton *)GetDlgItem(enableW[ii].ids[W_DN]))->GetCheck())	// 이하
				codeinfo.alarm[ii].cond[0] = '1';
			else
				codeinfo.alarm[ii].cond[0] = '0';
		}
		if (enableW[ii].ids[W_OR] != 0 && enableW[ii].ids[W_AND] != 0)
		{
			if (((CButton *)GetDlgItem(enableW[ii].ids[W_OR]))->GetCheck())// OR
				codeinfo.alarm[ii].oper[0] = '1';
			else
				codeinfo.alarm[ii].oper[0] = '0';
		}
	}

	// 알람설정
	codeinfo.a_kind[0] = 0x00;
	if (((CButton *)GetDlgItem(IDC_CHK_CONFIRM))->GetCheck())
		codeinfo.a_kind[0] |= 0x01;
	if (((CButton *)GetDlgItem(IDC_CHK_SIGNAL))->GetCheck())
		codeinfo.a_kind[0] |= 0x02;
	if (((CButton *)GetDlgItem(IDC_CHK_WAVE))->GetCheck())
		codeinfo.a_kind[0] |= 0x04;
	

	// 주문확인
	int sel = 0;
	codeinfo.o_run[0] = '0';
	
	((CExSpin *)GetDlgItem(IDC_EDIT_WAVE))->GetWindowText(tmpS);	// 소리파일
	memcpy(&codeinfo.a_wave, tmpS, min(tmpS.GetLength(), sizeof(codeinfo.a_wave)));

	if (((CButton *)GetDlgItem(IDC_CHK_JUMUN))->GetCheck())
		codeinfo.o_run[0] = '1';		// 주문확인

	//+ 요건 번호 : 0014 [20090513:kbkim]
	if(m_OrderExcBtn.GetCheck() == BST_CHECKED) // 주문실행이 체크되어 있는 경우에 값 설정
	{
		codeinfo.o_run[0] = '1';

		sel = m_cbOrderType.GetCurSel();	// 주문구분
		if (sel == JMGB_MS)
			codeinfo.o_kind[0] = '0';
		else
			codeinfo.o_kind[0] = '1';
		
		sel = m_cbDealingType.GetCurSel();	// 매매구분
		if (sel == MMGB_JIJUNG)
			codeinfo.o_mmgb[0] = '0';
		else
			codeinfo.o_mmgb[0] = '1'; 

		GetDlgItem(IDC_ORDERCOUNT_EDIT)->GetWindowText(tmpS);	// 주문수량
		memcpy(&codeinfo.o_num, tmpS, min(tmpS.GetLength(), sizeof(codeinfo.o_num)));

		GetDlgItem(IDC_ORDERCOST_EDIT)->GetWindowText(tmpS); 	// 주문단가
		if (code.GetLength()==8)
			tmpS.Format("%d", (int)(atof((LPCSTR)tmpS)*100 + 0.0000000001));
		memcpy(&codeinfo.o_prc, tmpS, min(tmpS.GetLength(), sizeof(codeinfo.o_prc)));
	}
	else
		codeinfo.o_run[0] = '0';

	//- 

	memcpy(&m_codeinfo, &codeinfo, sz_codeinfo);
}

CString	CSetupDlg::getString(char* datB, int datL)
{

	CString	string = CString(datB, datL);
	string.TrimLeft(); string.TrimRight();
	return string;
}

CString CSetupDlg::getColString(int col, CString tmpS)
{
	if (tmpS.IsEmpty())	
		return tmpS;
	
	switch (col)
	{
	case IDX_DIFF:
		{
			bool bMinus = false;
			if (tmpS.GetAt(0) == '4' || tmpS.GetAt(0) == '5')
				bMinus = true;
			tmpS = tmpS.Mid(1);
			if (bMinus)	tmpS = "-" + tmpS;
		}			
		break;
	case IDX_RATE:
		if (tmpS.GetAt(0) == '+')
			tmpS = tmpS.Mid(1);
		break;
	default:
		tmpS.Remove('+');	tmpS.Remove('-');
		break;
	}
	return tmpS;
}


void CSetupDlg::OnCxprice() 
{
	//if (!m_pPrice || !m_pSetupSise)	return;

	//CString currS = m_pSetupSise->GetItemText(SISEROW, IUCOL_CURR);
	//CString str;
	//str.Format("%s%-12s%-8.2f%-8.2f%-8.2f", '1', "", currS.GetString(), 100000.0, 100.0);
	//m_pPrice->SetValues(str);
	////m_pPrice->SetWindowPos(nullptr, rc.right, rc.top, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	//m_pPrice->BtnClick();	
}

void CSetupDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

//////////////////////////////////////////////////////////////////////////
// 요건 번호   : 0014
// 요건 제목   : 시세포착
// 요건 요청자 : 김진아
// 요건 내용   : 주문 실행 컨트롤들 삽입
//               포착시 데이터(주문구분, 수량, 단가) 부모 윈도우에 전달
// 수정 일자   : 20090513
// 수정자      : kbkim
//////////////////////////////////////////////////////////////////////////
//+
void CSetupDlg::OnChkOrderexc() 
{
	// TODO: Add your control notification handler code here
	BOOL bOrderExc{};
	if(m_OrderExcBtn.GetCheck() == BST_CHECKED)
		bOrderExc = TRUE;
	else
		bOrderExc = FALSE;

	GetDlgItem(IDC_CBORDER_TYPE)->EnableWindow(bOrderExc);
	GetDlgItem(IDC_CBDEALING_TYPE)->EnableWindow(bOrderExc);
	GetDlgItem(IDC_ORDERCOUNT_EDIT)->EnableWindow(bOrderExc);
	GetDlgItem(IDC_ORDERCOST_EDIT)->EnableWindow(bOrderExc);
}
//-

void CSetupDlg::OnSelchangeCbdealingType() 
{
	// TODO: Add your control notification handler code here
	CString sCtrlDate = "";
	GetDlgItem(IDC_CBDEALING_TYPE)->GetWindowText(sCtrlDate);

	if(sCtrlDate == "시장가")
	{
		GetDlgItem(IDC_ORDERCOST_EDIT)->SetWindowText("0");
		GetDlgItem(IDC_ORDERCOST_EDIT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_ORDERCOST_EDIT)->EnableWindow(TRUE);
	}
}

void CSetupDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	const int nGap = 5;
	CRect rc;
	CRect rcTopGroupCtrl, rcBottomGroupCtrl;
	GetClientRect(&rc);
	dc.FillSolidRect(&rc, m_clrBk);
	
	rc.InflateRect(-3,-3);
	dc.FillSolidRect(&rc, m_clrWriteBk);
	DrawRoundBitmap(&dc, rc, ROUND_PANEL);

	GetDlgItem(IDC_STATIC1)->GetWindowRect(rcTopGroupCtrl);
	ScreenToClient(rcTopGroupCtrl);
	GetDlgItem(IDC_STATIC3)->GetWindowRect(rcBottomGroupCtrl);
	ScreenToClient(rcBottomGroupCtrl);

	rc.left = 2*nGap; 
	rc.top = rcTopGroupCtrl.top - nGap;
	rc.right = rc.right - nGap - 2; 
	rc.bottom = rcBottomGroupCtrl.bottom + nGap + 2;

	dc.FillSolidRect(&rc, m_clrRoundBk);
	// Do not call CDialog::OnPaint() for painting messages

	draw_groupBox(&dc, IDC_STATIC1);
	draw_groupBox(&dc, IDC_STATIC2);
	draw_groupBox(&dc, IDC_STATIC3);
}

CString CSetupDlg::GetPalette()
{
	char buff[10];
	CString strPal;
	CString file;
	file.Format("%s\\%s\\%s", m_root, "tab", "palette.ini");
	
	GetPrivateProfileString("General", "Palette", "", buff, sizeof(buff), file);
	strPal = buff;
	
	return strPal;
}

void CSetupDlg::DrawRoundBitmap(CDC *pDC, CRect rc, int nKind)
{
	CRect bitRc;
	
	switch(nKind)
	{
	case ROUND_PANEL:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rt.bmp");
		
		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lb.bmp");
		
		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rb.bmp");
		break;
	case ROUND_CONTENTS:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rt.bmp");
		
		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lb.bmp");
		
		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rb.bmp");
		break;
	}
}

void CSetupDlg::DrawBitmap(CDC *pDC, CRect rc, CString strImage)
{
	CString strPath = Variant(homeCC, "");
	CString sImageDir = strPath + "\\image\\" + strImage;
	
	HBITMAP BitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), sImageDir, IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	CDC		memDC;
	
	memDC.CreateCompatibleDC(pDC);
	
	if (memDC.m_hDC != nullptr) 
		memDC.SelectObject(BitMap);
	
	if (memDC.m_hDC)
		::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);
	
	DeleteObject(BitMap);
	memDC.DeleteDC();
}

CString CSetupDlg::Variant(int comm, CString data)
{
	CString retvalue;
	char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);
	
	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";
	
	return retvalue;
}

void CSetupDlg::setImgBtn(CImgBtn *pButton, int nLength, BOOL bImageFit)
{
	CString sImgDir = "";
	sImgDir.Format("%s%dbtn", m_sImgDir, nLength);
	
	pButton->m_hBitmap    = getAxBitmap(sImgDir + ".bmp")->operator HBITMAP();
	pButton->m_hBitmap_dn = getAxBitmap(sImgDir + "_dn.bmp")->operator HBITMAP();
	pButton->m_hBitmap_hv = getAxBitmap(sImgDir + "_en.bmp")->operator HBITMAP();
	if (bImageFit)
		pButton->FitImageSize();
}

CBitmap* CSetupDlg::getAxBitmap(CString path)
{
	return (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}

HBRUSH CSetupDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO: Change any attributes of the DC here
	pDC->SetBkMode(TRANSPARENT);

	const int nID = pWnd->GetDlgCtrlID();
	switch(nID)
	{
	case IDC_CHK_CURR	:
	case IDC_CHK_DIFF	:
	case IDC_CHK_MDGA	:
	case IDC_CHK_MSGA	:
	case IDC_CHK_RATE	:
	case IDC_CHK_GVOL	:
	case IDC_CHK_CONFIRM:
	case IDC_CHK_SIGNAL	:
	case IDC_CHK_WAVE	:
	case IDC_CHK_JUMUN	: 
	case IDC_RUP_CURR	:
	case IDC_RDN_DIFF	:
	case IDC_RDN_CURR	:
	case IDC_RUP_DIFF	:
	case IDC_RUP_MDGA	:
	case IDC_RDN_MDGA	:
	case IDC_RUP_MSGA	:
	case IDC_RDN_MSGA	:
	case IDC_RUP_RATE	:
	case IDC_RDN_RATE	:
	case IDC_RUP_GVOL	:
	case IDC_RDN_GVOL	:
	case IDC_AND_DIFF	:
	case IDC_OR_DIFF	:
	case IDC_AND_MDGA	:
	case IDC_OR_MDGA	:
	case IDC_AND_MSGA	:
	case IDC_OR_MSGA	:
	case IDC_AND_RATE	:
	case IDC_OR_RATE	:
	case IDC_AND_GVOL	:
	case IDC_OR_GVOL	:		
	case IDC_STATIC		:
	case IDC_STATIC1	:		
	case IDC_STATIC2	:		
	case IDC_CHK_ORDEREXC:
		return m_br;
	}	

	return hbr;
}

void CSetupDlg::OnBtnOk() 
{
	// TODO: Add your control notification handler code here
	if (!checkInfo())
		return;
	saveInfo();
	CDialog::OnOK();	
}

void CSetupDlg::OnBtnCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CSetupDlg::draw_groupBox(CPaintDC *pdc, int ids)
{
	pdc->SelectObject(m_pFont);
	CRect rc, out;
	GetDlgItem(ids)->GetClientRect(rc);
	GetDlgItem(ids)->GetWindowRect(out);
	ScreenToClient(out);
	
	rc.top+=6;
	CPen pn_in, pn_out;
	pn_in.CreatePen(PS_SOLID, 1, GetAxColor(38));
	pn_out.CreatePen(PS_SOLID, 1, GetAxColor(76));
	
	rc.OffsetRect(out.left, out.top); 
	rc.OffsetRect(1,1);
	
	pdc->SelectObject(pn_out);
	pdc->MoveTo(rc.left, rc.top); pdc->LineTo(rc.right, rc.top); pdc->LineTo(rc.right, rc.bottom);
	pdc->LineTo(rc.left, rc.bottom); pdc->LineTo(rc.left, rc.top);
	rc.OffsetRect(-1,-1);
	pdc->SelectObject(pn_in);
	pdc->MoveTo(rc.left, rc.top); pdc->LineTo(rc.right, rc.top); pdc->LineTo(rc.right, rc.bottom);
	pdc->LineTo(rc.left, rc.bottom); pdc->LineTo(rc.left, rc.top);
	
	CString data;
	GetDlgItem(ids)->GetWindowText(data);
	pdc->TextOut(rc.left+7, rc.top-5, data);
} 

COLORREF CSetupDlg::GetAxColor(int iColor)
{
	if (iColor & 0x02000000)
		return iColor;
	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)iColor);
}

void CSetupDlg::OnChangeOrdercostEdit() 
{
	//2017.01.20 KSJ WIN7에서 무한루프탄다.
	return;

/*
	CEdit *pWnd = (CEdit*)GetDlgItem(IDC_ORDERCOST_EDIT);
	if (pWnd)
	{
		int sp,ep;
		pWnd->GetSel(sp, ep);
		BOOL bDot = FALSE;
		CString txt, ntxt;
		pWnd->GetWindowText(txt);
		for(int n=0; n<txt.GetLength(); ++n)
		{
			char ch = txt.GetAt(n);
			if (ch>='0' && ch<='9')
			{
				ntxt += ch;
			}
			else if (ch=='.' && !bDot)
			{
				bDot = TRUE;
				ntxt += ch;
			}
		}
		pWnd->SetWindowText(ntxt);
		if (ntxt!=txt)
			pWnd->SetSel(sp-1, ep-1, FALSE);
		else
			pWnd->SetSel(sp, ep, FALSE);
		
		CString log;
		log.Format("pos(%d/%d) (%s)->(%s)\n", sp, ep, (LPCSTR)txt, (LPCSTR)ntxt);
		OutputDebugString((LPCSTR)log);

	}*/
}
