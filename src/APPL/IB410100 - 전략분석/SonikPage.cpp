#include "stdafx.h"
#include <math.h>
#include "IB410100.h"
#include "SonikPage.h"
#include "GridWnd.h"
#include "GrpWnd.h"
#include "MapWnd.h"
#include "../../h/axisfire.h"
#include "../../h/ledger.h"
#include "../../control/fx_misc/miscType.h"
#include "../../control/fx_misc/fxtab.h"
#include "../../h/axisvar.h"

#include "Cfg.h"
#include "OpSensCal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAP_HOGA	"DH310110"

#define	TM_GETIVID	1001
#define	TM_GETIVGAP	100
#define GRID_WIDTH	100
#define GRP_WIDTH	520
#define	ROW_HEIGHT	22
#define CTRLBAR_HEIGHT  32

struct _tblSonikCol{
	int	width;
} tblSonikCol[3] = {37, 153, 80};

struct _tblSonikBunKiCol{
	int	width;
} tblSonikBunKiCol[2] = {70, 200};

struct _tblDepositCol{
	int	width;
} tblDepositCol[2] = { 135, 135 };

[[gsl::suppress(26409)]]
IMPLEMENT_DYNCREATE(CSonikPage, CPropertyPage)

CSonikPage::CSonikPage(CWnd* pView, CWnd* pParent) : CPropertyPage(CSonikPage::IDD)
{
	//{{AFX_DATA_INIT(CSonikPage)
	//}}AFX_DATA_INIT
	m_pApp = (CIB410100App *)AfxGetApp();
	m_pView = pView;
	m_pParent = pParent;

	m_pTab = NULL;
	m_pGrpWnd = NULL;
	m_pGridWnd = NULL;

	m_sSonikTbl[0][0] = "";
	m_sSonikTbl[0][1] = "지수범위";
	m_sSonikTbl[0][2] = "만기손익(천)";
	m_sSonikTbl[1][0] = "최대";
	m_sSonikTbl[1][2] = "";
	m_sSonikTbl[1][2] = "";
	m_sSonikTbl[2][0] = "최소";
	m_sSonikTbl[2][2] = "";
	m_sSonikTbl[2][2] = "";

	m_sSonikBunKiTbl[0][0] = "손익분기";
	m_sSonikBunKiTbl[0][1] = "기초자산가격";
	m_sSonikBunKiTbl[1][0] = "만기손익";
	m_sSonikBunKiTbl[1][1] = "";
	m_sSonikBunKiTbl[2][0] = "이론손익";
	m_sSonikBunKiTbl[2][1] = "";

	m_sDepositTbl[0][0] = "위탁증거금";
	m_sDepositTbl[1][0] = "";
	m_sDepositTbl[0][1] = "유지증거금";
	m_sDepositTbl[1][1] = "";
	
	
	m_sDepGuessTbl[0][0] = "위탁증거금";
	m_sDepGuessTbl[1][0] = "";
	m_sDepGuessTbl[0][1] = "유지증거금";
	m_sDepGuessTbl[1][1] = "";
	
	m_nKind = 0;
	m_nInfoKind = 0;

	m_pathRange = "";
	
}

CSonikPage::~CSonikPage()
{
}

void CSonikPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSonikPage)
	DDX_Control(pDX, IDC_CHKRANGE, m_chkRange);
	DDX_Control(pDX, IDC_BTN_APPLY, m_btnApply);
	DDX_Control(pDX, IDC_RDO_CONFIGVOL, m_rdoConfigVol);
	DDX_Control(pDX, IDC_SPN_ITEMPUTCHG, m_spnItemPutChg);
	DDX_Control(pDX, IDC_SPN_ITEMCALLCHG, m_spnItemCallChg);
	DDX_Control(pDX, IDC_SPN_CFGPUTCHG, m_spnCfgPutChg);
	DDX_Control(pDX, IDC_SPN_CFGCALLCHG, m_spnCfgCallChg);
	DDX_Control(pDX, IDC_SPN_REMAINDAY, m_spnRemainDay);
	DDX_Control(pDX, IDC_SPN_JISURANGE2, m_spnJisuRange2);
	DDX_Control(pDX, IDC_SPN_JISURANGE1, m_spnJisuRange1);
	DDX_Control(pDX, IDC_MGCHK1, m_ChkSonik);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSonikPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSonikPage)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_MGCHK1, OnMgchk1)
	ON_BN_CLICKED(IDC_MGCHK2, OnMgchk2)
	ON_BN_CLICKED(IDC_MGCHK3, OnMgchk3)
	ON_BN_CLICKED(IDC_MGCHK4, OnMgchk4)
	ON_BN_CLICKED(IDC_MGCHK5, OnMgchk5)
	ON_BN_CLICKED(IDC_MGCHK6, OnMgchk6)
	ON_BN_CLICKED(IDC_MGRADIOX1, OnMgradiox1)
	ON_BN_CLICKED(IDC_MGRADIOX2, OnMgradiox2)
	ON_BN_CLICKED(IDC_MGRADIOX3, OnMgradiox3)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHK_APPLY, OnChkApply)
	ON_BN_CLICKED(IDC_CHKRANGE, OnChkrange)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_POSInitData, OnInitData)
	ON_MESSAGE(WM_POSMSVAL, OnMsgMouseValue)
	ON_MESSAGE(WM_POSGetHV, OnMsgGetHV)
	ON_MESSAGE(WM_POSGetFee, OnMsgGetFee)
	ON_MESSAGE(WM_POSInitParam, OnMsgInitParam)
	ON_MESSAGE(WM_POSDEPOSITSEL, OnMsgDepositSel)
	ON_MESSAGE(WM_POSCLEAR, OnMsgClear)
	
		
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSonikPage message handlers

int CSonikPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_bkColor = m_pApp->GetIndexColor(m_pView, 66);
	m_bkBr.CreateSolidBrush(m_bkColor);

	m_pFont = m_pApp->GetFont(m_pView, 9);

	m_gubn = variant(systemCC, "");
	
	
	return 0;
}

void CSonikPage::OnDestroy() 
{
	//DELWND(m_pTab);
	//DELWND(m_pGrpWnd);
	//DELWND(m_pGridWnd);

	CPropertyPage::OnDestroy();
}

BOOL CSonikPage::OnSetActive() 
{
	//::SetWindowPos( m_hWnd, HWND_TOP, 1, 1, 0, 0, SWP_NOSIZE); 
	CRect rcParent;
	GetParent()->GetClientRect(&rcParent);
	::SetWindowPos( m_hWnd, HWND_TOP, rcParent.left, rcParent.top, rcParent.right, rcParent.bottom, 0); 
	
	return CPropertyPage::OnSetActive();
}

void CSonikPage::OnMgchk1() 
{
	const int chk = ((CButton *)GetDlgItem(IDC_MGCHK1))->GetCheck();
	((CButton *)GetDlgItem(IDC_MGCHK6))->EnableWindow(chk);
	CheckMinGam();
	m_pGrpWnd->SendMessage(WM_GRPRefresh, ResetSensitive+chk, SONIK);
}

void CSonikPage::OnMgchk2() 
{
	const int chk = ((CButton *)GetDlgItem(IDC_MGCHK2))->GetCheck();
	CheckMinGam();
	m_pGrpWnd->SendMessage(WM_GRPRefresh, ResetSensitive+chk, DELTA);
}

void CSonikPage::OnMgchk3() 
{
	const int chk = ((CButton *)GetDlgItem(IDC_MGCHK3))->GetCheck();
	CheckMinGam();
	m_pGrpWnd->SendMessage(WM_GRPRefresh, ResetSensitive+chk, GAMMA);
}

void CSonikPage::OnMgchk4() 
{
	const int chk = ((CButton *)GetDlgItem(IDC_MGCHK4))->GetCheck();
	CheckMinGam();
	m_pGrpWnd->SendMessage(WM_GRPRefresh, ResetSensitive+chk, CETA);	
}

void CSonikPage::OnMgchk5() 
{
	const int chk = ((CButton *)GetDlgItem(IDC_MGCHK5))->GetCheck();
	CheckMinGam();
	m_pGrpWnd->SendMessage(WM_GRPRefresh, ResetSensitive+chk, VEGA);	
}

void CSonikPage::OnMgchk6() 
{
	const int chk = ((CButton *)GetDlgItem(IDC_MGCHK6))->GetCheck();
	CheckMinGam();
	m_pGrpWnd->SendMessage(WM_GRPRefresh, ResetSensitive+chk, IRON);	
}

void CSonikPage::OnMgradiox1() 
{
	m_nKind = 0;
	SetSonikPage();
	m_pGridWnd->SetGridItemText(0, 0, "지수");
	GetDlgItem(IDC_MGCHK1)->SetWindowText("만기");
	GetDlgItem(IDC_MGCHK6)->ShowWindow(SW_SHOW);

	const int chk = ((CButton *)GetDlgItem(IDC_MGCHK1))->GetCheck();
	if (chk)	m_pGridWnd->SetGridItemText(0, 1, "만기");

	m_pGrpWnd->SendMessage(WM_GRPRefresh, SetPayOff, 0);
}

void CSonikPage::OnMgradiox2() 
{
	m_nKind = 1;
	SetSonikPage();
	m_pGridWnd->SetGridItemText(0, 0, "잔존일수");
	GetDlgItem(IDC_MGCHK1)->SetWindowText("이론");
	GetDlgItem(IDC_MGCHK6)->ShowWindow(SW_HIDE);
	
	const int chk = ((CButton *)GetDlgItem(IDC_MGCHK1))->GetCheck();
	if (chk)	m_pGridWnd->SetGridItemText(0, 1, "이론손익");

	m_pGrpWnd->SendMessage(WM_GRPRefresh, SetPayOff, 2);
}

void CSonikPage::OnMgradiox3() 
{
	m_nKind = 2;
	SetSonikPage();
	m_pGridWnd->SetGridItemText(0, 0, "변동성");
	GetDlgItem(IDC_MGCHK1)->SetWindowText("이론");
	GetDlgItem(IDC_MGCHK6)->ShowWindow(SW_HIDE);
	
	const int chk = ((CButton *)GetDlgItem(IDC_MGCHK1))->GetCheck();
	if (chk)	m_pGridWnd->SetGridItemText(0, 1, "이론손익");

	m_pGrpWnd->SendMessage(WM_GRPRefresh, SetPayOff, 3);
}

void CSonikPage::CheckMinGam()
{
	int	chknum = 0;
	CString	names = "";
	const int pID[4] = {IDC_MGCHK2, IDC_MGCHK3, IDC_MGCHK4, IDC_MGCHK5};
	for (int ii = 0 ; ii < 4 ; ii++)
	{
		if (((CButton*)GetDlgItem(pID[ii]))->GetCheck())
		{
			switch (pID[ii])
			{
			case IDC_MGCHK2:
				m_pGrpWnd->m_chkdelta = true;
				names += "델타|";
				break;
			case IDC_MGCHK3:
				m_pGrpWnd->m_chkgamma = true;
				names += "감마|";
				break;
			case IDC_MGCHK4:
				m_pGrpWnd->m_chkseta = true;
				names += "세타|";
				break;
			case IDC_MGCHK5:
				m_pGrpWnd->m_chkvega = true;
				names += "베가|";
				break;
			}
			chknum++;
		}
		else
		{
			switch (pID[ii])
			{
			case IDC_MGCHK2:
				m_pGrpWnd->m_chkdelta = false;
				break;
			case IDC_MGCHK3:
				m_pGrpWnd->m_chkgamma = false;
				break;
			case IDC_MGCHK4:
				m_pGrpWnd->m_chkseta = false;
				break;
			case IDC_MGCHK5:
				m_pGrpWnd->m_chkvega = false;
				break;
			}
		}
	}

	const int chkSonik = ((CButton *)GetDlgItem(IDC_MGCHK1))->GetCheck();
	if (chkSonik)
	{
		const int chk = ((CButton *)GetDlgItem(IDC_MGRADIOX1))->GetCheck();
		if (chk)	
		{
			if (((CButton *)GetDlgItem(IDC_MGCHK6))->GetCheck())
				m_pGridWnd->SetGridItemText(0, 1, "이론손익");
			else
				m_pGridWnd->SetGridItemText(0, 1, "만기손익");

		}
		else		
			m_pGridWnd->SetGridItemText(0, 1, "이론손익");
	}
	else
	{
		int		no = 0;
		CString text[4] = {"델타", "감마", "세타", "베가"};
		for (int ii = 0 ; ii < 4; ii++)
		{
			if (((CButton*)GetDlgItem(pID[ii]))->GetCheck())
			{
				m_pGridWnd->SetGridItemText(0, 1, text[no]);
				break;
			}
			no++;
		}
	}

	// MinGam Set
	m_pGrpWnd->m_names  = names;
	m_pGrpWnd->m_chknum = chknum;
}

BOOL CSonikPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CRect	rc, rcClient;
	GetClientRect(&rcClient);
	rcClient.DeflateRect(2, 2);

	rc = rcClient;
	rc.top = CTRLBAR_HEIGHT;
	rc.right = rc.left + GRID_WIDTH;
	m_pGridWnd = std::make_unique<CGridWnd>(m_pView, m_pParent);
	m_pGridWnd->m_fcolWidth = 40;
	m_pGridWnd->Create(NULL, "", WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS, rc, this, -1);
	m_pGridWnd->SetGridItemText(0, 1, "만기");

	rc.left = rc.right + 1;
	rc.right = rc.left + GRP_WIDTH;
	m_pGrpWnd = std::make_unique <CGrpWnd>(m_pView, m_pParent);
	m_pGrpWnd->Create(NULL, "", WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS, rc, this, -1);

	((CMapWnd*)m_pParent)->m_idxX = 0;	// 만기손익체크
	((CButton *)GetDlgItem(IDC_MGRADIOX1))->SetCheck(1);
	//((CMapWnd*)m_pParent)->m_idxVar = 0;	// H.V

	m_ChkSonik.SetCheck(1);	// 손익체크
	m_pGrpWnd->SendMessage(WM_GRPRefresh, SetSensitive, SONIK);

	//((CMapWnd*)m_pParent)->m_jjIdx = 0;	// 잔존일수 - 달력일수

	GetDlgItem(IDC_BOX_INFO)->GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.top = 5; 
	rc.bottom = rc.top + TAB_HEIGHT;//35;
	

	m_pTab = std::make_unique <CfxTab>();
	m_pTab->Create(rc, this, ID_TAB1);
	m_pTab->LoadTabImage(((CMapWnd*)m_pParent)->m_sRoot, false);

	m_pTab->SetColor(m_pApp->GetIndexColor(m_pView, 70), m_pApp->GetIndexColor(m_pView, 181), m_pApp->GetIndexColor(m_pView, 71), m_pApp->GetIndexColor(m_pView, 181), m_pApp->GetIndexColor(m_pView, 181));
	m_pTab->SetBorderColor(m_pApp->GetIndexColor(m_pView, 44)); 

	m_pTab->InsertItem(0, "손익분기");
	//m_pTab->SetColor(RGB(0, 0, 0), NULL, RGB(0, 0, 0), NULL, m_bkColor);
	m_pTab->InsertItem(1, " Simulation ");
	//m_pTab->InsertItem(2, " 증거금 ");
	m_pTab->SetStyle(tsIMAGE, true);

	m_btnApply.SetImgBitmap(((CMapWnd*)m_pParent)->m_bmp2BTN, ((CMapWnd*)m_pParent)->m_bmp2BTN_DN, ((CMapWnd*)m_pParent)->m_bmp2BTN_EN);
	m_btnApply.SetFont(m_pFont);

	m_spnJisuRange1.Init(attrNUMERIC, spNUMBER, this);
	m_spnJisuRange2.Init(attrNUMERIC, spNUMBER, this);
	m_spnRemainDay.Init(attrNUMERIC, spNUMBER, this);
	m_spnCfgCallChg.Init(attrNUMERIC, spNUMBER, this);
	m_spnCfgPutChg.Init(attrNUMERIC, spNUMBER, this);
	m_spnItemCallChg.Init(attrNUMERIC, spNUMBER, this);
	m_spnItemPutChg.Init(attrNUMERIC, spNUMBER, this);
	m_spnJisuRange1.m_pEdit->SetFont(m_pFont);
	m_spnJisuRange2.m_pEdit->SetFont(m_pFont);
	m_spnRemainDay.m_pEdit->SetFont(m_pFont);
	m_spnCfgCallChg.m_pEdit->SetFont(m_pFont);
	m_spnCfgPutChg.m_pEdit->SetFont(m_pFont);
	m_spnItemCallChg.m_pEdit->SetFont(m_pFont);
	m_spnItemPutChg.m_pEdit->SetFont(m_pFont);
	m_rdoConfigVol.SetCheck(true);

	SetSonikPage();
	SetTimer(TM_GETIVID, TM_GETIVGAP, NULL);

	((CButton*)GetDlgItem(IDC_CHK_APPLY))->SetCheck(0);
	SetApplyMode();

	((CEdit *)GetDlgItem(IDC_EDT_K200GUESS))->SetLimitText(6);
	m_pTab->BringWindowToTop();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSonikPage::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TM_GETIVID)
	{
		KillTimer(nIDEvent);
		//m_pParent->SendMessage(WM_MAPUser, MAKEWPARAM(RequestIV, 0), 0);
	}
	
	CPropertyPage::OnTimer(nIDEvent);
}

void CSonikPage::InitData(struct _bdat *bdat)
{
	CString fcod = bdat->frts;
	CString cdkr; cdkr.Format("%.2f", bdat->cdgl);
	CString yjbs_21; yjbs_21.Format("%.2f", bdat->hv[0]);
	CString yjbs_30; yjbs_30.Format("%.2f", bdat->hv[1]);
	CString yjbs_60; yjbs_60.Format("%.2f", bdat->hv[2]);
	CString yjbs_90; yjbs_90.Format("%.2f", bdat->hv[3]);

	m_pGrpWnd->m_cdkr = bdat->cdgl / 100;
	m_pGrpWnd->m_k200 = bdat->basicprc;
	//m_pGrpWnd->m_k200Jisu = bdat->k200cur;	// cej
	m_pGrpWnd->m_k200Jisu = bdat->basicprc;	// cej	20071214
	if (m_pGrpWnd->m_k200Jisu < 0.0)
		m_pGrpWnd->m_k200Jisu *= (-1.0);

	m_sHval[0] = yjbs_21;
	m_sHval[1] = yjbs_30;
	m_sHval[2] = yjbs_60;
	m_sHval[3] = yjbs_90;

	WriteCfgCallPutIV();

	m_pGrpWnd->SendMessage(WM_GRPRefresh, DeleteGrpData, 0);

	//m_spnJisuRange1.m_pEdit->SetWindowText("");
	//m_rtnStr = "-1";
	//m_pGrpWnd->SendMessage(WM_GRPSetParam, SetJisuStart, (long)(LPTSTR)(LPCTSTR)m_rtnStr);

	//m_spnJisuRange2.m_pEdit->SetWindowText("");
	//m_rtnStr = "-1";
	//m_pGrpWnd->SendMessage(WM_GRPSetParam, SetJisuEnd, (long)(LPTSTR)(LPCTSTR)m_rtnStr);

	CString rootS = variant(homeCC, "");
	m_pathRange.Format("%s\\%s\\%s\\%s", variant(homeCC, ""), USRDIR, variant(nameCC, ""), FILE3101);

	loadSonikRange();

	m_pGrpWnd->SendMessage(WM_GRPRefresh, RedrawGraph);
}

LRESULT CSonikPage::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		switch (HIBYTE(LOWORD(wParam)))
		{
		case KEY_CODEMINGAM:	
			break;
		case KEY_DEPOSIT:
			oubDeposit((char *)lParam, HIWORD(wParam));
			break;
		case KEY_DEPGUESS:
			oubDepGuess((char *)lParam, HIWORD(wParam));
			break;
		}
		break;
	//case DLL_ALERT:
	case DLL_ALERTx:
		m_pGrpWnd->SendMessage(WM_GRPAlert, wParam, lParam);
		break;
	case DLL_SETPAL:
		m_pGridWnd->SetColor();
		m_pGridWnd->InitFormat();
		Invalidate();
		break;
	}
	return 0;
}


LRESULT CSonikPage::OnInitData(WPARAM wParam, LPARAM lParam)
{
	InitData((struct _bdat *)wParam);
	return 0;
}

long CSonikPage::OnMsgMouseValue(WPARAM wParam, LPARAM lParam)
{
	CString sValue;

	_MouseValue* MouseValue = (_MouseValue*) wParam;
	for (int ii = 0 ; ii < 11 ; ii++)
	{
		if (ii != 0)
			MouseValue = MouseValue + 1;

		sValue.Format("%s", MouseValue->sValue);
		m_pGridWnd->SetGridItemText(ii + 1, 0, MouseValue->kpt);
		m_pGridWnd->SetGridItemText(ii + 1, 1, sValue);
	}
	return 0;
}

long CSonikPage::OnMsgGetHV(WPARAM wParam, LPARAM lParam)
{
	CString str, val;
	double dCallMove=0, dPutMove=0;
	_GraphData *pData = (_GraphData *)lParam;

	if (((CButton*)GetDlgItem(IDC_CHK_APPLY))->GetCheck())
	{
		if (m_rdoConfigVol.GetCheck())
		{
			if (pData->code[0] == '2' || pData->code[0] == 'B')  //파생상품 코드개편
				GetDlgItem(IDC_EDT_CFGCALLVOL)->GetWindowText(val);
			else
				GetDlgItem(IDC_EDT_CFGPUTVOL)->GetWindowText(val);

			m_spnCfgCallChg.m_pEdit->GetWindowText(str);	dCallMove = atof(str);
			m_spnCfgPutChg.m_pEdit->GetWindowText(str);	dPutMove = atof(str);
		}
		else
		{
			((CMapWnd*)m_pParent)->m_opsenscal->SetJCode(pData->code);
			val.Format("%.2f", fabs(pData->curr));		((CMapWnd*)m_pParent)->m_opsenscal->SetCurrValue(val);
			val.Format("%.1f", pData->hsga);		((CMapWnd*)m_pParent)->m_opsenscal->SetStrike(val);
			val.Format("%.2f", ((CMapWnd*)m_pParent)->m_bData->cdgl);	((CMapWnd*)m_pParent)->m_opsenscal->SetCd(val);
			val.Format("%.2f", ((CMapWnd*)m_pParent)->m_bData->k200cur);	((CMapWnd*)m_pParent)->m_opsenscal->SetK200Cur(val);
			val.Format("%.2f", ((CMapWnd*)m_pParent)->m_bData->fcur);	((CMapWnd*)m_pParent)->m_opsenscal->SetFutureCur(val);
			val.Format("%.6f", ((CMapWnd*)m_pParent)->m_bData->bdjs);	((CMapWnd*)m_pParent)->m_opsenscal->SetBedangJisu(val);
			val.Format("%.2f", pData->jjis[0]);		((CMapWnd*)m_pParent)->m_opsenscal->SetBzRemainDays(val);
			val.Format("%.2f", pData->jjis[1]);		((CMapWnd*)m_pParent)->m_opsenscal->SetCalRemainDays(val);
			((CMapWnd*)m_pParent)->m_opsenscal->Calc(1);	// IV 만 계산

			val = ((CMapWnd*)m_pParent)->m_opsenscal->GetIv();

			m_spnItemCallChg.m_pEdit->GetWindowText(str);	dCallMove = atof(str);
			m_spnItemPutChg.m_pEdit->GetWindowText(str);	dPutMove = atof(str);
		}

		if (pData->code[0] == '2' || pData->code[0] == 'B')  //파생상품 코드개편
		{
			val.Format("%.2f", atof(val) + dCallMove);
		}
		else
		{
			val.Format("%.2f", atof(val) + dPutMove);
		}
		m_sVal.Format("%s", val);
	}
	else
	{
		switch (atoi(((CMapWnd*)m_pParent)->m_cfg->ReadCfg("VolType")))
		{
		/*
		case 0: // 종목별내재변동성
		case 1: // 콜풋 ATM 내재변동성
			{
				CString val;

				((CMapWnd*)m_pParent)->m_opsenscal->SetJCode(pData->code);
				val.Format("%.2f", fabs(pData->curr));
				((CMapWnd*)m_pParent)->m_opsenscal->SetCurrValue(val);
				val.Format("%.1f", pData->hsga);
				((CMapWnd*)m_pParent)->m_opsenscal->SetStrike(val);
				val.Format("%.2f", fabs(m_pGrpWnd->m_k200));
				((CMapWnd*)m_pParent)->m_opsenscal->SetUnderlying(val);
				val.Format("%.2f", ((CMapWnd*)m_pParent)->m_bData->bdjs);
				((CMapWnd*)m_pParent)->m_opsenscal->SetBedangJisu(val);
				if (atoi(((CMapWnd*)m_pParent)->m_cfg->ReadCfg("RDType")) == 0)
					val.Format("%.2f", pData->jjis[0]);	// 거래일수
				else
					val.Format("%.2f", pData->jjis[0]);	// 달력일수
				((CMapWnd*)m_pParent)->m_opsenscal->SetRemainDays(val);

				val = ((CMapWnd*)m_pParent)->m_opsenscal->GetIv();
	//CString sTest;
	//sTest.Format("IC [%s] prc(%f) strike(%f) k200(%f) bdjs(%f), 거래일(%f) iv(%s)\n", 
	//	     pData->code, fabs(pData->curr), pData->hsga, fabs(m_pGrpWnd->m_k200), (((CMapWnd*)m_pParent)->m_bData->bdjs), pData->jjis[0], val);
	//OutputDebugString(sTest);
				double	iv = atof(val);
				m_sVal.Format("%2f", iv);
			}
			break;
		*/
		case 2: // 사용자정의
			if (pData->code[0] == '2' || pData->code[0] == 'B')  //파생상품 코드개편
				m_sVal = ((CMapWnd*)m_pParent)->m_cfg->ReadCfg("UserCallIV");
			else
				m_sVal = ((CMapWnd*)m_pParent)->m_cfg->ReadCfg("UserPutIV");
			break;
		default: // 역사적 변동성 적용
			val = ((CMapWnd*)m_pParent)->m_cfg->ReadCfg("HVDay");
			switch (atoi(val))
			{
			case 90:
				m_sVal.Format("%.2f",  ((CMapWnd*)m_pParent)->m_bData->hv[0]);
				break;
			case 60:
				m_sVal.Format("%.2f",  ((CMapWnd*)m_pParent)->m_bData->hv[1]);
				break;
			case 30:
				m_sVal.Format("%.2f",  ((CMapWnd*)m_pParent)->m_bData->hv[2]);
				break;
			case 21:
				m_sVal.Format("%.2f",  ((CMapWnd*)m_pParent)->m_bData->hv[3]);
				break;
			}
			break;
		}
	}

	return (LONG) m_sVal.operator LPCTSTR();
}

long CSonikPage::OnMsgGetFee(WPARAM wParam, LPARAM lParam)
{
	switch ((int) wParam)
	{
	case 0: // 선물 수수료
//		m_pEdit_sFee->GetWindowText(m_valFee);
		break;
	case 1: // 옵션 수수료
//		m_pEdit_oFee->GetWindowText(m_valFee);
		break;
	}

//	return (LONG) m_valFee.operator LPCTSTR();
	return 0;
}

long CSonikPage::OnMsgInitParam(WPARAM wParam, LPARAM lParam)
{
	ReadGrpParam();
	return 0;
}


LRESULT CSonikPage::OnMsgClear(WPARAM wParam, LPARAM lParam)
{
	m_sDepositTbl[1][0] = "";
	m_sDepositTbl[1][1] = "";

	m_sDepGuessTbl[1][0] = "";
	m_sDepGuessTbl[1][1] = "";

	CRect rcInfo;
	GetDlgItem(IDC_BOX_INFO)->GetWindowRect(&rcInfo); 
	ScreenToClient(&rcInfo);
	InvalidateRect(rcInfo);
	return 0;
}

LRESULT CSonikPage::OnMsgDepositSel(WPARAM wParam, LPARAM lParam)
{
	if (m_nInfoKind == 2)	// 증거금 선택시
	{
		CString k200Guess;
		GetDlgItem(IDC_EDT_K200GUESS)->GetWindowText(k200Guess);
		char *k200 = (char *)lParam;
		memcpy(k200, k200Guess, k200Guess.GetLength());
		return TRUE;
	}
	else
		return FALSE;
}

HBRUSH CSonikPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	pDC->SetBkMode(TRANSPARENT);
	
	if (pWnd->GetDlgCtrlID() == IDC_X_STATIC	|| 
		pWnd->GetDlgCtrlID() == IDC_MGRADIOX1	||
		pWnd->GetDlgCtrlID() == IDC_MGRADIOX2	||
		pWnd->GetDlgCtrlID() == IDC_MGRADIOX3	||
		pWnd->GetDlgCtrlID() == IDC_Y_STATIC	||
		pWnd->GetDlgCtrlID() == IDC_MGCHK1		||
		pWnd->GetDlgCtrlID() == IDC_MGCHK2		||
		pWnd->GetDlgCtrlID() == IDC_MGCHK3		||
		pWnd->GetDlgCtrlID() == IDC_MGCHK4		||
		pWnd->GetDlgCtrlID() == IDC_MGCHK5		||
		pWnd->GetDlgCtrlID() == IDC_MGCHK6		||
		pWnd->GetDlgCtrlID() == IDC_CHK_APPLY		||
		pWnd->GetDlgCtrlID() == IDC_STC_VOLCHG		||
		pWnd->GetDlgCtrlID() == IDC_RDO_CONFIGVOL	||
		pWnd->GetDlgCtrlID() == IDC_RDO_ITEMIV		||
		pWnd->GetDlgCtrlID() == IDC_CHKRANGE )
	{
		pDC->SetBkColor(m_bkColor);
		return CreateSolidBrush(m_bkColor);
	}

//	hbr = m_bkBr;
	return hbr;
}

void CSonikPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcClient, rcGrp;
	GetClientRect(&rcClient);
	m_pGrpWnd->GetWindowRect(rcGrp); ScreenToClient(rcGrp);
	dc.ExcludeClipRect(&rcGrp);
	dc.FillSolidRect(rcClient, m_bkColor);

	CRect rc;
	GetDlgItem(IDC_BOX_INFO)->GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.top = 2; 
	rc.bottom = 5;
	dc.FillSolidRect(&rc, m_pApp->GetIndexColor(m_pView, 181));

	m_pTab->LoadTabImage(((CMapWnd*)m_pParent)->m_sRoot, false); // 스킨 변경시 적용될 수 있도록

	DrawInfo(&dc);
}

void CSonikPage::SetSonikPage()
{
	/*
	switch (m_nKind)
	{
	case 0: break; // 만기지수
	case 1: break; // 잔존일수
	case 2: break; // 변동성
	}
	*/
	GetDlgItem(IDC_SPN_JISURANGE1)->ShowWindow(m_nInfoKind == 0 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_SPN_JISURANGE2)->ShowWindow(m_nInfoKind == 0 ? SW_SHOW : SW_HIDE);

	GetDlgItem(IDC_CHK_APPLY)->ShowWindow(m_nInfoKind == 1 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_SPN_REMAINDAY)->ShowWindow(m_nInfoKind == 1 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_STC_VOLCHG)->ShowWindow(m_nInfoKind == 1 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_RDO_CONFIGVOL)->ShowWindow(m_nInfoKind == 1 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_RDO_ITEMIV)->ShowWindow(m_nInfoKind == 1 ? SW_SHOW : SW_HIDE);

	GetDlgItem(IDC_EDT_CFGCALLVOL)->ShowWindow(m_nInfoKind == 1 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_EDT_CFGPUTVOL)->ShowWindow(m_nInfoKind == 1 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_SPN_CFGCALLCHG)->ShowWindow(m_nInfoKind == 1 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_SPN_CFGPUTCHG)->ShowWindow(m_nInfoKind == 1 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_SPN_ITEMCALLCHG)->ShowWindow(m_nInfoKind == 1 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_SPN_ITEMPUTCHG)->ShowWindow(m_nInfoKind == 1 ? SW_SHOW : SW_HIDE);
	
	CRect rcInfo;
	GetDlgItem(IDC_BOX_INFO)->GetWindowRect(&rcInfo); 
	ScreenToClient(&rcInfo);
	InvalidateRect(rcInfo);

	switch (m_nInfoKind)
	{
	case 0:	// 만기지수
	case 1:	// 잔존일수
		GetDlgItem(IDC_BTN_APPLY)->SetWindowText("적용");
		GetDlgItem(IDC_EDT_K200GUESS)->ShowWindow(SW_HIDE);
		
		break;
	case 2:	// 변동성
		GetDlgItem(IDC_BTN_APPLY)->SetWindowText("조회");
		GetDlgItem(IDC_EDT_K200GUESS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDT_K200GUESS)->SetFocus();
		
		searchDeposit(false);
		break;
	}

	switch (m_nInfoKind)
	{
	case 0:	// 만기지수
		m_chkRange.ShowWindow(SW_SHOW);
		break;
	case 1:	// 잔존일수
	case 2:	// 변동성
		m_chkRange.ShowWindow(SW_HIDE);
		break;
	}
}

void CSonikPage::ReadGrpParam()
{
	int nVal;
	CString sText, sJisuStart, sJisuEnd;
	
	m_spnJisuRange1.m_pEdit->GetWindowText(sJisuStart);
	m_spnJisuRange2.m_pEdit->GetWindowText(sJisuEnd);

	sText.Format("%s", (char *)m_pGrpWnd->SendMessage(WM_GRPSetParam, GetJisuStart, 0));
	nVal = atoi(sText); if (nVal == -1) sText = "0";
	sText.Format("%d", nVal);
	if (sJisuStart != sText) m_spnJisuRange1.m_pEdit->SetWindowText(sText);

	sText.Format("%s", (char*)m_pGrpWnd->SendMessage(WM_GRPSetParam, GetJisuEnd, 0));
	nVal = atoi(sText); if (nVal == -1) sText = "0";
	sText.Format("%d", nVal);
	if (sJisuEnd != sText) m_spnJisuRange2.m_pEdit->SetWindowText(sText);


	m_sSonikTbl[1][1] = m_pGrpWnd->m_allmax;
	m_sSonikTbl[2][1] = m_pGrpWnd->m_allmin;
	m_sSonikTbl[1][2] = m_pGrpWnd->m_Maxiik;
	m_sSonikTbl[2][2] = m_pGrpWnd->m_Miniik;
	
	CString sSonikBunki("");
	for (int flag = 0; flag < 2; flag++)
	{
		sSonikBunki.Empty();
		for (int ii = 0; ii < m_pGrpWnd->m_bunkiCount[flag]; ii++)
		{
			if (ii == m_pGrpWnd->m_bunkiCount[flag] -1)
				sSonikBunki += m_pGrpWnd->m_bunkiValue[flag][ii];
			else
				sSonikBunki += m_pGrpWnd->m_bunkiValue[flag][ii] + " / ";
		}
		m_sSonikBunKiTbl[flag+1][1] = sSonikBunki;
	}

	CRect rcInfo;
	GetDlgItem(IDC_BOX_INFO)->GetWindowRect(&rcInfo); 
	ScreenToClient(&rcInfo);
	InvalidateRect(rcInfo);
}

void CSonikPage::DrawInfo(CDC *dc)
{
	CString str;
	CBrush br(m_pApp->GetIndexColor(m_pView, 75));
	dc->SetBkMode(TRANSPARENT);

	CFont *ofont;
	CRect rcClient, rcCell;
	GetDlgItem(IDC_BOX_INFO)->GetWindowRect(&rcClient); ScreenToClient(&rcClient);
	rcClient.DeflateRect(10, 10);

	ofont = dc->SelectObject(m_pFont);
	switch (m_nInfoKind)
	{
	case 2:	// 증거금
		{
			rcCell = rcClient;
			rcCell.bottom = rcCell.top + 40;

			CRect rcTitle = rcCell;
			rcTitle.top = rcTitle.bottom - 19;
			dc->DrawText("* 주문체결 후 증거금", -1, rcTitle, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

			for (int ii = 0; ii < 2; ii++)
			{
				rcCell.top = rcCell.bottom - 1;
				rcCell.bottom = rcCell.top + ROW_HEIGHT;
				rcCell.right = rcClient.left -1;

				for (int jj = 0; jj < 2; jj++)
				{
					rcCell.left = rcCell.right -1;
					rcCell.right = rcCell.left + tblDepositCol[jj].width;
					dc->FrameRect(rcCell, &br);
					
					dc->DrawText(m_sDepositTbl[ii][jj], -1, rcCell, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				}
			}

			rcTitle = rcClient;
			rcTitle.top = rcTitle.top + 100;
			rcTitle.bottom = rcTitle.top + 20;
			dc->DrawText("* 주문체결 후 익일 추정증거금", -1, rcTitle, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

			rcTitle.top = rcTitle.bottom + 4;
			rcTitle.bottom = rcTitle.top + 20;
			dc->DrawText("예상 KOSPI200 지수              (\"조회\" 클릭)", -1, rcTitle, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

			rcCell = rcTitle;
			rcCell.top = rcTitle.bottom + 30;
			rcCell.bottom = rcTitle.top + ROW_HEIGHT;
			for (int ii = 0; ii < 2; ii++)
			{
				rcCell.top = rcCell.bottom - 1;
				rcCell.bottom = rcCell.top + ROW_HEIGHT;
				rcCell.right = rcClient.left -1;

				for (int jj = 0; jj < 2; jj++)
				{
					rcCell.left = rcCell.right -1;
					rcCell.right = rcCell.left + tblDepositCol[jj].width;
					dc->FrameRect(rcCell, &br);
					
					dc->DrawText(m_sDepGuessTbl[ii][jj], -1, rcCell, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
					
				}
			}
		}
		break;
	case 0:	// 손익분기
		switch (m_nKind)
		{
		case 0: // 만기지수
		case 1: // 잔존일수
		case 2: // 변동성
			{
				// 만기손익 & 이론손익 table
				rcCell = rcClient;
				rcCell.bottom = rcCell.top + 60;
				for (int ii = 0; ii < 3; ii++)
				{
					rcCell.top = rcCell.bottom - 1;
					rcCell.bottom = rcCell.top + ROW_HEIGHT;
					rcCell.right = rcClient.left -1;

					for (int jj = 0; jj < 3; jj++)
					{
						rcCell.left = rcCell.right -1;
						rcCell.right = rcCell.left + tblSonikCol[jj].width;
						dc->FrameRect(rcCell, &br);
						
						if (m_sSonikTbl[ii][jj].GetLength() > 0)
						{
							dc->DrawText(m_sSonikTbl[ii][jj], -1, rcCell, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

// 							OutputDebugString("[KSJ]" +m_sSonikTbl[ii][jj] );
						}
					}
				}

				// 손익분기 table
				rcCell.bottom = rcCell.bottom + 20;
				for (int ii = 0; ii < 3; ii++)
				{
					rcCell.top = rcCell.bottom - 1;
					rcCell.bottom = rcCell.top + ROW_HEIGHT;
					rcCell.right = rcClient.left -1;

					for (int jj = 0; jj < 2; jj++)
					{
						rcCell.left = rcCell.right -1;
						rcCell.right = rcCell.left + tblSonikBunKiCol[jj].width;
						dc->FrameRect(rcCell, &br);
						
						if (m_sSonikBunKiTbl[ii][jj].GetLength() > 0)
							dc->DrawText(m_sSonikBunKiTbl[ii][jj], -1, rcCell, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
					}
				}
				DrawItemText(dc, IDC_SPN_JISURANGE1, true, &rcClient, "기초자산범위");
				DrawItemText(dc, IDC_SPN_JISURANGE1, false, &rcClient, "∼", 5);
			}
		}
		break;
	case 1:	// simulation
		{
			DrawItemText(dc, IDC_SPN_REMAINDAY, true, &rcClient, "잔존일수 조정");
			DrawItemText(dc, IDC_SPN_REMAINDAY, false, &rcClient, "(+,-)");

			DrawItemText(dc, IDC_EDT_CFGCALLVOL, true, &rcClient, "콜");
			DrawItemText(dc, IDC_EDT_CFGCALLVOL, false, &rcClient, "+");
			DrawItemText(dc, IDC_SPN_CFGCALLCHG, false, &rcClient, "%P", 5);

			DrawItemText(dc, IDC_EDT_CFGPUTVOL, true, &rcClient, "풋");
			DrawItemText(dc, IDC_EDT_CFGPUTVOL, false, &rcClient, "+");
			DrawItemText(dc, IDC_SPN_CFGPUTCHG, false, &rcClient, "%P", 5);

			DrawItemText(dc, IDC_SPN_ITEMCALLCHG, true, &rcClient, "콜 종목별IV  +");
			DrawItemText(dc, IDC_SPN_ITEMCALLCHG, false, &rcClient, "%P", 5);
			DrawItemText(dc, IDC_SPN_ITEMPUTCHG, true, &rcClient, "풋 종목별IV  +");
			DrawItemText(dc, IDC_SPN_ITEMPUTCHG, false, &rcClient, "%P", 5);
		}
		break;
	}
	dc->SelectObject(ofont);
}

void CSonikPage::DrawItemText(CDC* dc, long dlgItem, bool left, CRect* rcLimit, CString text, int gap)
{
	const int nLimitGap = 7;
	CRect rcCell;
	GetDlgItem(dlgItem)->GetWindowRect(&rcCell); ScreenToClient(&rcCell);
	if (left)
	{
		rcCell.right = rcCell.left - gap; rcCell.left = rcLimit->left + nLimitGap;
		dc->DrawText(text, -1, rcCell, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
	}
	else
	{
		rcCell.left = rcCell.right + gap; rcCell.right = rcLimit->right - nLimitGap;
		dc->DrawText(text, -1, rcCell, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	}
}

BOOL CSonikPage::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	//double dval;
	CString str, str2;
	NMHDR *pNMHDR = (NMHDR*) lParam;
	switch(wParam)
	{
	case ID_TAB1:
		if (m_pTab->GetCurSel() == m_nInfoKind) break;

		m_nInfoKind = m_pTab->GetCurSel();
		/*
		switch(m_nInfoKind)
		{
		case 0: // 챠트정보
			break;
		case 1: // 민감설정
			break;
		case 2:	// 증거금
			break;
		}
		*/
		SetSonikPage();
		break;
	/*
	case IDC_SPN_JISURANGE1:
		if (pNMHDR->code == SPNM_ONCHANGE)
		{
			m_spnJisuRange1.m_pEdit->GetWindowText(str);
			if (atoi(str) == 0) break;
			dval = fabs(atof(str));	m_rtnStr.Format("%.2f", dval);
			if (dval < 50 || dval < fabs(((CMapWnd*)m_pParent)->m_bData->fcur) - 100 || dval > fabs(((CMapWnd*)m_pParent)->m_bData->fcur) + 300) break;
				
			m_spnJisuRange2.m_pEdit->GetWindowText(str);
			if (dval >= atof(str)) break;

			m_pGrpWnd->SendMessage(WM_GRPSetParam, SetJisuStart, (long)(LPTSTR)(LPCTSTR)m_rtnStr);
			m_pGrpWnd->PostMessage(WM_GRPRefresh, RedrawGraph);
		}
	case IDC_SPN_JISURANGE2:
		if (pNMHDR->code == SPNM_ONCHANGE)
		{
			m_spnJisuRange2.m_pEdit->GetWindowText(str);
			if (atoi(str) == 0) break;
			dval = fabs(atof(str));	m_rtnStr.Format("%.2f", dval);
			if (dval < 50 || dval < fabs(((CMapWnd*)m_pParent)->m_bData->fcur) - 100 || dval > fabs(((CMapWnd*)m_pParent)->m_bData->fcur) + 300) break;

			m_spnJisuRange1.m_pEdit->GetWindowText(str);
			if (dval <= atof(str)) break;

			m_pGrpWnd->SendMessage(WM_GRPSetParam, SetJisuEnd, (long)(LPTSTR)(LPCTSTR)m_rtnStr);
			m_pGrpWnd->PostMessage(WM_GRPRefresh, RedrawGraph);
		}
		break;
	case IDC_SPN_REMAINDAY:
		if (pNMHDR->code == SPNM_ONCHANGE)
		{
			m_spnRemainDay.m_pEdit->GetWindowText(str);
			dval = fabs(atof(str));	m_rtnStr.Format("%.2f", dval);
			m_pGrpWnd->SendMessage(WM_GRPSetParam, SetJjilMove, (long)(LPTSTR)(LPCTSTR)m_rtnStr);
			m_pGrpWnd->PostMessage(WM_GRPRefresh, RedrawGraph);
		}
		break;
	case IDC_RDO_CONFIGVOL:
	case IDC_RDO_ITEMIV:
		m_pGrpWnd->PostMessage(WM_GRPRefresh, RedrawGraph);
		break;
	*/
	}
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

void CSonikPage::WriteCfgCallPutIV()
{
	double dval = 0;
	CString str, strCall, strPut, val;
	switch (atoi(((CMapWnd*)m_pParent)->m_cfg->ReadCfg("VolType")))
	{
	case 2: // 사용자정의
		strCall = ((CMapWnd*)m_pParent)->m_cfg->ReadCfg("UserCallIV");
		strPut = ((CMapWnd*)m_pParent)->m_cfg->ReadCfg("UserPutIV");
		GetDlgItem(IDC_EDT_CFGCALLVOL)->SetWindowText(strCall);
		GetDlgItem(IDC_EDT_CFGPUTVOL)->SetWindowText(strPut);
		break;
	default: // 역사적 변동성 적용
		val = ((CMapWnd*)m_pParent)->m_cfg->ReadCfg("HVDay");
		
		switch (atoi(val))
		{
		case 90:
			dval = ((CMapWnd*)m_pParent)->m_bData->hv[0];
			break;
		case 60:
			dval = ((CMapWnd*)m_pParent)->m_bData->hv[1];
			break;
		case 30:
			dval = ((CMapWnd*)m_pParent)->m_bData->hv[2];
			break;
		case 21:
			dval = ((CMapWnd*)m_pParent)->m_bData->hv[3];
			break;
		}
		if (dval < 0 || dval > 100) dval = 0;
		str.Format("%.2f",  dval);
		GetDlgItem(IDC_EDT_CFGCALLVOL)->SetWindowText(str);
		GetDlgItem(IDC_EDT_CFGPUTVOL)->SetWindowText(str);
		break;
	}
}


void CSonikPage::OnBtnApply() 
{
	switch (m_nInfoKind)
	{
	case 2:	// 증거금
		searchDeposit(true);
		break;
	default:
		{
		double dval;
		CString str;

		m_spnJisuRange1.m_pEdit->GetWindowText(str);
		dval = fabs(atof(str));	m_rtnStr.Format("%.2f", dval);
		if (dval > 50 && dval > fabs(((CMapWnd*)m_pParent)->m_bData->fcur) - 100 && dval < fabs(((CMapWnd*)m_pParent)->m_bData->fcur) + 300)
		{
			m_spnJisuRange2.m_pEdit->GetWindowText(str);
			if (dval < atof(str))
				m_pGrpWnd->SendMessage(WM_GRPSetParam, SetJisuStart, (long)(LPTSTR)(LPCTSTR)m_rtnStr);
		}

		m_spnJisuRange2.m_pEdit->GetWindowText(str);
		dval = fabs(atof(str));	m_rtnStr.Format("%.2f", dval);
		if (dval > 50 && dval > fabs(((CMapWnd*)m_pParent)->m_bData->fcur) - 100 && dval < fabs(((CMapWnd*)m_pParent)->m_bData->fcur) + 300)
		{
			m_spnJisuRange1.m_pEdit->GetWindowText(str);
			if (dval > atof(str))
				m_pGrpWnd->SendMessage(WM_GRPSetParam, SetJisuEnd, (long)(LPTSTR)(LPCTSTR)m_rtnStr);
		}
		
		saveSonikRange();

		m_spnRemainDay.m_pEdit->GetWindowText(str);
		dval = atof(str); m_rtnStr.Format("%.2f", dval);
		m_pGrpWnd->SendMessage(WM_GRPSetParam, SetJjilMove, (long)(LPTSTR)(LPCTSTR)m_rtnStr);

		m_pGrpWnd->PostMessage(WM_GRPRefresh, RedrawGraph);
		}
		break;
	}

}

void CSonikPage::OnSize(UINT nType, int cx, int cy) 
{
	CPropertyPage::OnSize(nType, cx, cy);
	
	if (m_pGrpWnd == NULL || !m_pGrpWnd->GetSafeHwnd())
		return;

	CRect rcClient, rcGrp;
	GetClientRect(rcClient);
	m_pGrpWnd->GetWindowRect(rcGrp); ScreenToClient(rcGrp);
	rcGrp.bottom = rcClient.bottom -2;
	m_pGrpWnd->MoveWindow(&rcGrp, TRUE);
}

void CSonikPage::SetApplyMode()
{
	const BOOL bEnable = ((CButton*)GetDlgItem(IDC_CHK_APPLY))->GetCheck();

	GetDlgItem(IDC_RDO_CONFIGVOL)->EnableWindow(bEnable);
	GetDlgItem(IDC_RDO_ITEMIV)->EnableWindow(bEnable);
	m_spnItemPutChg.EnableWindow(bEnable);
	m_spnItemCallChg.EnableWindow(bEnable);
	m_spnCfgPutChg.EnableWindow(bEnable);
	m_spnCfgCallChg.EnableWindow(bEnable);
	m_spnRemainDay.EnableWindow(bEnable);
}

void CSonikPage::OnChkApply() 
{
	SetApplyMode();
}

// cej
void CSonikPage::SetBasicJisu(double basicprc)
{
	if (m_pGrpWnd)
	{
		m_pGrpWnd->m_k200Jisu = basicprc;	// cej	20071214
		m_pGrpWnd->Invalidate();
	}
}

void CSonikPage::oubDeposit(char *buf, int len)
{
	CString sErrCode;
	CString sErrText;

	struct _ledgerH ledger;
	CopyMemory(&ledger, (void*)buf, L_ledgerH);
	sErrCode = CString((char*)&ledger.ecod, sizeof(ledger.ecod));
	sErrText = CString((char*)&ledger.emsg, sizeof(ledger.emsg));

	if (sErrCode.Compare("000000"))	// 오류
	{
		AfxMessageBox(sErrText);
		return;
	}
	buf += L_ledgerH;

	CString dataS = CString(buf, len-L_ledgerH);
	parse(dataS, "\t");					// 현잔고기준 : 총액 : 예탁금액
	parse(dataS, "\t");					// 현잔고기준 : 총액 : 위탁증거금
	parse(dataS, "\t");					// 현잔고기준 : 총액 : 주문가능금액
	parse(dataS, "\t");					// 현잔고기준 : 총액 : 유지증거금
	parse(dataS, "\t");					// 현잔고기준 : 총액 : 추가증거금
	parse(dataS, "\t");					// 현잔고기준 : 현금 : 예탁금액
	parse(dataS, "\t");					// 현잔고기준 : 현금 : 위탁증거금
	parse(dataS, "\t");					// 현잔고기준 : 현금 : 주문가능금액
	parse(dataS, "\t");					// 현잔고기준 : 현금 : 유지증거금
	parse(dataS, "\t");					// 현잔고기준 : 현금 : 추가증거금

	parse(dataS, "\t");					// 주문체결가정 : 총액 : 예탁금액
	m_sDepositTbl[1][0] = setComma(parse(dataS, "\t"));	// 주문체결가정 : 총액 : 위탁증거금 ★
	parse(dataS, "\t");					// 주문체결가정 : 총액 : 주문가능금액
	m_sDepositTbl[1][1] = setComma(parse(dataS, "\t"));	// 주문체결가정 : 총액 : 유지증거금 ★
	parse(dataS, "\t");					// 주문체결가정 : 총액 : 추가증거금
	parse(dataS, "\t");					// 주문체결가정 : 현금 : 예탁금액
	parse(dataS, "\t");					// 주문체결가정 : 현금 : 위탁증거금
	parse(dataS, "\t");					// 주문체결가정 : 현금 : 주문가능금액
	parse(dataS, "\t");					// 주문체결가정 : 현금 : 유지증거금
	parse(dataS, "\t");					// 주문체결가정 : 현금 : 추가증거금
	
	CRect rcInfo;
	GetDlgItem(IDC_BOX_INFO)->GetWindowRect(&rcInfo); 
	ScreenToClient(&rcInfo);
	InvalidateRect(rcInfo);
	
}

void CSonikPage::oubDepGuess(char *buf, int len)
{
	CString sErrCode;
	CString sErrText;

	struct _ledgerH ledger;
	CopyMemory(&ledger, (void*)buf, L_ledgerH);
	sErrCode = CString((char*)&ledger.ecod, sizeof(ledger.ecod));
	sErrText = CString((char*)&ledger.emsg, sizeof(ledger.emsg));

	if (sErrCode.Compare("000000"))	// 오류
	{
		AfxMessageBox(sErrText);
		return;
	}
	buf += L_ledgerH;

	CString dataS = CString(buf, len-L_ledgerH);
	parse(dataS, "\t");					// 현잔고기준 : 총액 : 예탁금액
	parse(dataS, "\t");					// 현잔고기준 : 총액 : 위탁증거금
	parse(dataS, "\t");					// 현잔고기준 : 총액 : 주문가능금액
	parse(dataS, "\t");					// 현잔고기준 : 총액 : 유지증거금
	parse(dataS, "\t");					// 현잔고기준 : 총액 : 추가증거금
	parse(dataS, "\t");					// 현잔고기준 : 현금 : 예탁금액
	parse(dataS, "\t");					// 현잔고기준 : 현금 : 위탁증거금
	parse(dataS, "\t");					// 현잔고기준 : 현금 : 주문가능금액
	parse(dataS, "\t");					// 현잔고기준 : 현금 : 유지증거금
	parse(dataS, "\t");					// 현잔고기준 : 현금 : 추가증거금

	parse(dataS, "\t");					// 주문체결가정 : 총액 : 예탁금액
	m_sDepGuessTbl[1][0] = setComma(parse(dataS, "\t"));	// 주문체결가정 : 총액 : 위탁증거금 ★
	parse(dataS, "\t");					// 주문체결가정 : 총액 : 주문가능금액
	m_sDepGuessTbl[1][1] = setComma(parse(dataS, "\t"));	// 주문체결가정 : 총액 : 유지증거금 ★
	parse(dataS, "\t");					// 주문체결가정 : 총액 : 추가증거금
	parse(dataS, "\t");					// 주문체결가정 : 현금 : 예탁금액
	parse(dataS, "\t");					// 주문체결가정 : 현금 : 위탁증거금
	parse(dataS, "\t");					// 주문체결가정 : 현금 : 주문가능금액
	parse(dataS, "\t");					// 주문체결가정 : 현금 : 유지증거금
	parse(dataS, "\t");					// 주문체결가정 : 현금 : 추가증거금
	
	CRect rcInfo;
	GetDlgItem(IDC_BOX_INFO)->GetWindowRect(&rcInfo); 
	ScreenToClient(&rcInfo);
	InvalidateRect(rcInfo);
	
}


CString CSonikPage::parse(CString &srcstr, CString substr)
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

CString CSonikPage::variant(int cmd, CString data)
{
	CString retvalue;
	char* dta = (char*)m_pView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, cmd), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";

	return retvalue;
}

CString CSonikPage::setComma(CString src)
{
	CString temp, floatStr;
	int	ii=0, jj = -1;

	src.TrimLeft();
	if (src.Find('.') >= 0)
	{
		floatStr = src.Mid(src.Find('.'));
		src = src.Left(src.Find('.'));
	}
	else
		floatStr.Empty();
	
	temp = src;
	src.Empty();
	
	for (ii = temp.GetLength() - 1; ii >= 0; ii--)
	{
		jj++;
		if (jj == 3)
		{
			if (temp.GetAt(0) != '-' && temp.GetAt(0) != '+')
				src = ',' + src;
			else if (ii != 0)
				src = ',' + src;
			
			jj = 0;
		}
		src = temp.GetAt(ii) + src;
	}
	src += floatStr;
	
	return src;
}




void CSonikPage::searchDeposit(bool bAll) 
{
	CString k200Guess;
	GetDlgItem(IDC_EDT_K200GUESS)->GetWindowText(k200Guess);

	m_pParent->SendMessage(WM_USER + 1, MAKEWPARAM(MSG_DEPOSIT, bAll ? TRUE : FALSE), (LPARAM)(LPCTSTR)k200Guess);
}

void CSonikPage::loadSonikRange()
{
	const int chk = GetPrivateProfileInt(RANGESECTION, CHKRANGE, 0, m_pathRange);
	m_chkRange.SetCheck(chk ? TRUE : FALSE);
	if (!chk)	return;

	const int start = GetPrivateProfileInt(RANGESECTION, STARTRANGE, -1, m_pathRange);
	const int end = GetPrivateProfileInt(RANGESECTION, ENDRANGE, -1, m_pathRange);

	if (start == -1 || end == -1)
		return;

	m_rtnStr.Format("%d", start);
	m_spnJisuRange1.m_pEdit->SetWindowText(m_rtnStr);
	m_pGrpWnd->SendMessage(WM_GRPSetParam, SetJisuStart, (long)(LPTSTR)(LPCTSTR)m_rtnStr);

	m_rtnStr.Format("%d", end);
	m_spnJisuRange2.m_pEdit->SetWindowText(m_rtnStr);
	m_pGrpWnd->SendMessage(WM_GRPSetParam, SetJisuEnd, (long)(LPTSTR)(LPCTSTR)m_rtnStr);

}

void CSonikPage::saveSonikRange()
{
	CString tmpS;
	CString sJisuStart, sJisuEnd;

	const BOOL chk = m_chkRange.GetCheck();
	if (chk)
	{
		m_spnJisuRange1.m_pEdit->GetWindowText(sJisuStart);
		tmpS.Format("%d", atoi(sJisuStart));
		WritePrivateProfileString(RANGESECTION, STARTRANGE, tmpS, m_pathRange);

		m_spnJisuRange2.m_pEdit->GetWindowText(sJisuEnd);
		tmpS.Format("%d", atoi(sJisuEnd));
		WritePrivateProfileString(RANGESECTION, ENDRANGE, tmpS, m_pathRange);
	}
	else
	{
		WritePrivateProfileString(RANGESECTION, STARTRANGE, "-1", m_pathRange);
		WritePrivateProfileString(RANGESECTION, ENDRANGE, "-1", m_pathRange);
	}

	
	
}

void CSonikPage::OnChkrange() 
{
	CString tmpS;
	const BOOL chk = m_chkRange.GetCheck();
	tmpS.Format("%d", chk);
	WritePrivateProfileString(RANGESECTION, CHKRANGE, tmpS, m_pathRange);
	
}
