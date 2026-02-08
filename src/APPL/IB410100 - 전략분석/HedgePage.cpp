// HedgePage.cpp : implementation file
//

#include "stdafx.h"
#include "IB410100.h"
#include "HedgePage.h"
#include "message.h"
#include <math.h>
#include "OpSensCal.h"
#include "MapWnd.h"
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "../../control/fx_misc/miscType.h"
#include "../../control/fx_grid/griditem.h"
#include "../../control/fx_grid/fxgrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GUARANTEE_RATE	0.05		// 증거금율

#define	HG_MAINID	2003
#define	HG_MAXCOL	13

#define HG_CODE		0	// 코드
#define HG_JNAM		1	// 종목
#define HG_JQTY		2	// 수량
#define HG_JMGA		3	// 가격
#define HG_PGSI		4	// 소요자금, 헤지자금
#define HG_DELT		5	// 델타
#define HG_DELTR	6	// 델타차
#define HG_GAMA		7	// 감마
#define HG_GAMAR	8	// 감마차
#define HG_CETA		9	// 세타
#define HG_CETAR	10	// 세타차
#define HG_VEGA		11	// 베가
#define HG_VEGAR	12	// 베가차

#define	HG_COLW0	115
#define	HG_COLW1	60
#define	HG_COLW2	83
#define	HG_COLW3	120
#define	HG_COLW4	65
#define	HG_COLW5	65
#define	HG_COLW6	65
#define	HG_COLW7	65

struct _HgridINFO	{
	char	*text;
	int	width;
	UINT	format;
	DWORD	attr;
	char	*defaultVal;
	int	param;
	int	bkClr;
public:
} HgridINFO[HG_MAXCOL] = {
	{"코드",	0,		0,		0,		"",	0,	74},
	{"종목",	HG_COLW0,	GVFM_LEFT,	0,		"",	0,	74},
	{"수량",	HG_COLW1,	GVFM_RIGHT,	0,		"",	0,	74},
	{"가격",	HG_COLW2,	GVFM_RIGHT,	0,		"",	0,	74},
	{"소요자금",	HG_COLW3,	GVFM_RIGHT,	GVAT_COMMA,	"",	0,	74},
	{"델타",	HG_COLW4,	GVFM_RIGHT,	0,		"",	0,	74},
	{"델타결과",	HG_COLW4,	GVFM_RIGHT,	0,		"",	0,	75},
	{"감마",	HG_COLW5,	GVFM_RIGHT,	0,		"",	0,	74},
	{"감마결과",	HG_COLW5,	GVFM_RIGHT,	0,		"",	0,	75},
	{"세타",	HG_COLW6,	GVFM_RIGHT,	0,		"",	0,	74},
	{"세타결과",	HG_COLW6,	GVFM_RIGHT,	0,		"",	0,	75},
	{"베가",	HG_COLW7,	GVFM_RIGHT,	0,		"",	0,	74},
	{"베가결과",	HG_COLW7,	GVFM_RIGHT,	0,		"",	0,	75}
};

/////////////////////////////////////////////////////////////////////////////
// CHedgePage property page

[[gsl::suppress(26409)]]
IMPLEMENT_DYNCREATE(CHedgePage, CPropertyPage)

CHedgePage::CHedgePage(CWnd* pView, CWnd* pParent) : CPropertyPage(CHedgePage::IDD)
{
	//{{AFX_DATA_INIT(CHedgePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pApp = (CIB410100App *)AfxGetApp();
	m_pView = pView;
	m_pParent = pParent;

	m_gridArray.RemoveAll();
	m_codeArray.RemoveAll();

	m_RadioIdx = 0;
}

CHedgePage::~CHedgePage()
{
}

void CHedgePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHedgePage)
	DDX_Control(pDX, IDC_HBTN2, m_btnOrder);
	DDX_Control(pDX, IDC_HBTN1, m_btnCalc);
	DDX_Control(pDX, IDC_HEDIT2, m_EEdit);
	DDX_Control(pDX, IDC_HEDIT1, m_SEdit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHedgePage, CPropertyPage)
	//{{AFX_MSG_MAP(CHedgePage)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_HRADIO1, OnHradio1)
	ON_BN_CLICKED(IDC_HRADIO2, OnHradio2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_HSPIN1, OnDeltaposHspin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_HSPIN2, OnDeltaposHspin2)
	ON_BN_CLICKED(IDC_HBTN1, OnHbtn1)
	ON_BN_CLICKED(IDC_HBTN2, OnHbtn2)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_SP22Data, OnHedgeData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHedgePage message handlers

int CHedgePage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CHedgePage::OnDestroy() 
{
	DeleteGridArray();
//	DELWND(m_pGrid);

	CPropertyPage::OnDestroy();
}

BOOL CHedgePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_bkColor = m_pApp->GetIndexColor(m_pView, 66);
	m_bkBr.CreateSolidBrush(m_bkColor);
	InitProperties();
	m_pFont = m_pApp->GetFont(m_pView, 9, 0, "굴림");
	

	m_btnCalc.SetImgBitmap(((CMapWnd*)m_pParent)->m_bmp2BTN, ((CMapWnd*)m_pParent)->m_bmp2BTN_DN, ((CMapWnd*)m_pParent)->m_bmp2BTN_EN);
	m_btnCalc.SetFont(m_pFont);
	m_btnOrder.SetImgBitmap(((CMapWnd*)m_pParent)->m_bmp4BTN, ((CMapWnd*)m_pParent)->m_bmp4BTN_DN, ((CMapWnd*)m_pParent)->m_bmp4BTN_EN);
	m_btnOrder.SetFont(m_pFont);
	m_btnOrder.ShowWindow(false);

	CRect	rc;
	GetClientRect(&rc);
	rc.top = 35;
	rc.bottom = rc.bottom - 1;
	rc.left = 2;
	int nWidth = 16;
	for (int ii = 0 ; ii < HG_MAXCOL; ii++)
		nWidth += HgridINFO[ii].width;
	rc.right = rc.left + nWidth;

	m_pGrid = std::make_unique<CfxGrid>();
	m_pGrid->Create(rc, this, HG_MAINID, GVSC_VERT, GVDD_FULL, WS_CHILD|WS_VISIBLE);
	m_pGrid->SetFont(m_pFont);
	m_pGrid->SetGridFocusLine(FALSE, TRUE);
	m_pGrid->SetGridColor(RGB(192,192,192));
	m_pGrid->SetStepColor(1, m_pApp->GetIndexColor(m_pView, 30), m_pApp->GetIndexColor(m_pView, 31));
	m_pGrid->SetBKSelColor(m_pApp->GetIndexColor(m_pView, 78));
	InitGrid();

	m_SEdit.SetWindowText("0.00");
	m_EEdit.SetWindowText("0.00");

	for (int ii = 0; ii < 18; ii++)
		AddRow();
	m_pGrid->SetGridDIScroll(true);
	m_pGrid->Adjust();

	((CButton*)GetDlgItem(IDC_HRADIO1))->SetCheck(1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHedgePage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcClient;
	GetClientRect(&rcClient);
	dc.FillSolidRect(rcClient, m_bkColor);
}

long CHedgePage::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
		case DLL_ALERT:
			break;
		case DLL_SETPAL:
			InitProperties();
			InitGrid();
			Invalidate();
			break;
		case DLL_SETFONTx:
			break;
	}
	
	return 0;
}

long CHedgePage::OnHedgeData(WPARAM wParam, LPARAM lParam)
{
	CString stmp;
	char* pcBuffer = (char*) lParam;
	struct _modo* modo = (_modo*) pcBuffer;
	int nrec = atoi(CString(modo->grec, sizeof(modo->grec)));
	if (nrec <= 0)
		return 0;

stmp.Format("IB410100 m_gridArray size=[%d] grid row size = [%d]\n", m_gridArray.GetSize(), m_pGrid->GetRowCount());
OutputDebugString(stmp);


	CString val;
	CString code;
	gsl::owner<_HgridData*> gData = new _HgridData;

	code.Format("%s", ((CMapWnd*)m_pParent)->m_fcode);
	gData->jcod.Format("%s", code);
	gData->jnam.Format("%s", (char*)m_pView->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (long)(LPCTSTR)code));
	gData->jnam.TrimLeft(); gData->jnam.TrimRight();
	gData->curr = fabs(((CMapWnd*)m_pParent)->m_bData->fcur);
	gData->delt = 5.;
	gData->gama = 0.;
	gData->ceta = 0.;
	gData->vega = 0.;
	gData->gdan = 250000;	//KSJ 
	m_gridArray.Add(gData);




stmp.Format("IB410100 nrect=[%d] \n", nrec);
OutputDebugString(stmp);
//	const int nItem = 0;
	for (int ii = 0 ; ii < nrec; ii++)
	{
		CString code;
		gData = new _HgridData;

		_gmodo nGrid = modo->gmod[ii];

stmp.Format("----IB410100 call=[%.10s] put=[%.10s]\n", nGrid.c_rtcd, nGrid.p_rtcd);
OutputDebugString(stmp);
		
		gData->jcod = CString(nGrid.c_rtcd, sizeof(nGrid.c_rtcd)); gData->jcod.TrimLeft();
		code.Format("%s", gData->jcod);
		gData->jnam.Format("%s", (char*)m_pView->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (long)(LPCTSTR)code));
		gData->jnam.TrimLeft(); gData->jnam.TrimRight();

if(gData->jnam.IsEmpty())
	OutputDebugString("##########  " + stmp);

		gData->curr = fabs(atof(CString(nGrid.c_curr, sizeof(nGrid.c_curr))));
		const double strike = m_pApp->getOptionStrike(gData->jcod);

// 		gData->gdan = fabs(((CMapWnd*)m_pParent)->m_PFCodes.GetAt(ii)->gdan);

		((CMapWnd*)m_pParent)->m_opsenscal->SetJCode(gData->jcod);
		val.Format("%.2f", gData->curr);	((CMapWnd*)m_pParent)->m_opsenscal->SetCurrValue(val);
		val.Format("%.1f", strike);		((CMapWnd*)m_pParent)->m_opsenscal->SetStrike(val);
		val.Format("%.2f", ((CMapWnd*)m_pParent)->m_bData->cdgl);	((CMapWnd*)m_pParent)->m_opsenscal->SetCd(val);
		//val.Format("%.2f", fabs(((CMapWnd*)m_pParent)->m_bData->k200cur));	m_opsenscal->SetK200Cur(val);
		//val.Format("%.2f", fabs(((CMapWnd*)m_pParent)->m_bData->fcur));	m_opsenscal->SetFutureCur(val); 
		//val.Format("%.6f", ((CMapWnd*)m_pParent)->m_bData->bdjs);	m_opsenscal->SetBedangJisu(val);
		//val.Format("%.2f", mdat->iiil[0]);	m_opsenscal->SetBzRemainDays(val);
		//val.Format("%.2f", mdat->iiil[1]);	m_opsenscal->SetCalRemainDays(val);
		//m_opsenscal->SetTime(((CMapWnd*)m_pParent)->m_bData->time);
		((CMapWnd*)m_pParent)->m_opsenscal->Calc(0);
//CString sTest;
//sTest.Format("IC [%s] strike(%f) bprc(%f) bdjs(%f), 거래일(%f), 달력일(%f) Cdgl(%f)\n", 
//	     code, strike, m_bData->basicprc, m_bData->bdjs, mdat->iiil[0], mdat->iiil[1], m_bData->cdgl);
//OutputDebugString(sTest);

		if(code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D')	//2012.10.12 KSJ 선물일때 //파생상품 코드개편
			gData->delt = 1.0;
		else
			gData->delt = atof(((CMapWnd*)m_pParent)->m_opsenscal->GetDelta());

		gData->gama = atof(((CMapWnd*)m_pParent)->m_opsenscal->GetGamma());
		gData->ceta = atof(((CMapWnd*)m_pParent)->m_opsenscal->GetTheta());
		gData->vega = atof(((CMapWnd*)m_pParent)->m_opsenscal->GetVegga());
			
//TRACE("[%s] [%.4f][%.4f][%.4f][%.4f]\n", gData->jcod, gData->delt, gData->gama, gData->ceta, gData->vega);
		m_gridArray.Add(gData);
	}

stmp.Format("IB410100 [2] m_gridArray size=[%d] grid row size = [%d]\n", m_gridArray.GetSize(), m_pGrid->GetRowCount());
OutputDebugString(stmp);

	for (int ii = 0 ; ii < nrec; ii++)
	{
		CString code;
		gsl::owner<_HgridData*> gData = new _HgridData;
		const _gmodo nGrid = modo->gmod[ii];

		gData->jcod = CString(nGrid.p_rtcd, sizeof(nGrid.p_rtcd)); gData->jcod.TrimLeft();
		code.Format("%s", gData->jcod);
		gData->jnam.Format("%s", (char*)m_pView->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (long)(LPCTSTR)code));
		gData->jnam.TrimLeft(); gData->jnam.TrimRight();
		gData->curr = fabs(atof(CString(nGrid.p_curr, sizeof(nGrid.p_curr))));
		const double strike = m_pApp->getOptionStrike(gData->jcod);

		((CMapWnd*)m_pParent)->m_opsenscal->SetJCode(gData->jcod);
		val.Format("%.2f", gData->curr);	((CMapWnd*)m_pParent)->m_opsenscal->SetCurrValue(val);
		val.Format("%.1f", strike);		((CMapWnd*)m_pParent)->m_opsenscal->SetStrike(val);
		val.Format("%.2f", ((CMapWnd*)m_pParent)->m_bData->cdgl);	((CMapWnd*)m_pParent)->m_opsenscal->SetCd(val);
		((CMapWnd*)m_pParent)->m_opsenscal->Calc(0);

		if(code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D')	//2012.10.12 KSJ 선물일때//파생상품 코드개편
			gData->delt = 1.0;
		else
			gData->delt = atof(((CMapWnd*)m_pParent)->m_opsenscal->GetDelta());

		gData->gama = atof(((CMapWnd*)m_pParent)->m_opsenscal->GetGamma());
		gData->ceta = atof(((CMapWnd*)m_pParent)->m_opsenscal->GetTheta());
		gData->vega = atof(((CMapWnd*)m_pParent)->m_opsenscal->GetVegga());
			
		m_gridArray.Add(gData);
	}

stmp.Format("IB410100 [3] m_gridArray size=[%d] grid row size = [%d]\n", m_gridArray.GetSize(), m_pGrid->GetRowCount());
OutputDebugString(stmp);

	CalcHedge();
	return 0;
}

BOOL CHedgePage::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if (wParam == HG_MAINID)
	{
		NM_GVITEM* pnmgvitem = (NM_GVITEM*) lParam;
	}
	
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

BOOL CHedgePage::OnSetActive() 
{
	//::SetWindowPos( m_hWnd, HWND_TOP, 1, 1, 0, 0, SWP_NOSIZE); 
	CRect rcParent;
	GetParent()->GetClientRect(&rcParent);
	::SetWindowPos( m_hWnd, HWND_TOP, rcParent.left, rcParent.top, rcParent.right, rcParent.bottom, 0); 
		
	OnRadio(m_RadioIdx);
	
	return CPropertyPage::OnSetActive();
}

void CHedgePage::OnHradio1() 
{
	OnRadio(0);	
}

void CHedgePage::OnHradio2() 
{
	OnRadio(1);
}

void CHedgePage::OnDeltaposHspin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString	str;
	m_SEdit.GetWindowText(str);

	const double delta = atof(str) - pNMUpDown->iDelta;

	str.Format("%.2f", delta);
	m_SEdit.SetWindowText(str);
	
	*pResult = 0;
}

void CHedgePage::OnDeltaposHspin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString	str;
	m_EEdit.GetWindowText(str);

	const double delta = atof(str) - pNMUpDown->iDelta;

	str.Format("%.2f", delta);
	m_EEdit.SetWindowText(str);
	
	*pResult = 0;
}

void CHedgePage::OnHbtn1() 
{
	OnRadio(m_RadioIdx);	
}

void CHedgePage::OnHbtn2() 
{
	CString sCodes;
	const CIdCell	cell = m_pGrid->GetFocusCell();
	if (cell.row <= 0 || cell.row > m_gridArray.GetSize())
	{
		MessageBox("선택된 헤지코드가 없습니다.", "전략주문", MB_OK|MB_ICONEXCLAMATION );
		return;
	}

	m_tData = "";
	CString sGubn, sCode, sCount, sPrice;

	sCode = m_pGrid->GetItemText(cell.row, HG_CODE);
	sCount = m_pGrid->GetItemText(cell.row, HG_JQTY);
	sPrice = m_pGrid->GetItemText(cell.row, HG_JMGA);
	if (atoi(sCount) > 0)	// 매수
		sGubn = "2";		// 매수
	else
		sGubn = "1";		// 매도

	m_tData.Format("%s|%s|%s|%s|\t", sCode, sGubn, sCount, sPrice);
	if (m_tData.IsEmpty())
		return;

	m_pParent->SendMessage(WM_MAPHedgeData, MAKEWPARAM(ReqOrder, 0), long(m_tData.operator LPCTSTR()));
}

void CHedgePage::CalcHedge()
{
	CString	sData;
	sData.Format("%s", (char*)m_pParent->SendMessage(WM_MAPHedgeData, GetHedgeTarget, 0));
	for (int ii = 0; ii < 5; ii++)
		m_pHedgeTarget[ii] = atof(m_pApp->Parser(sData, "\t"));

	switch (m_RadioIdx)
	{
	case 0:	CalcHedgeD();	break;
	case 1:	CalcHedgeP();	break;
	}
}

void CHedgePage::CalcHedgeD()
{
	CString slog;
	CString	str;
	m_SEdit.GetWindowText(str);
	const double	dMin = atof(str);
	m_EEdit.GetWindowText(str);
	const double	dMax = atof(str);

	const double	deltaSum = m_pHedgeTarget[1];
	const double	gamaSum = m_pHedgeTarget[2];
	const double	thetaSum = m_pHedgeTarget[3];
	const double	vegaSum = m_pHedgeTarget[4];
	const double	pivot = deltaSum;

	str.Format("헤지대상 Delta : %.4f", pivot);
	GetDlgItem(IDC_STC_HEDGETARGET)->SetWindowText(str);

	_HgridData	*gData = nullptr;
	int	iCnt1=0, iCnt2=0;
	const int	nSize = m_gridArray.GetSize();

slog.Format("IB410100 gridrow=[%d]\n", m_pGrid->GetRowCount());
OutputDebugString(slog);

	for (int ii = 0; ii < nSize; ii++)
	{
		gData = m_gridArray.GetAt(ii);
		if (dMin != 0 || dMax != 0)
		{
			if (dMin > gData->curr || dMax < gData->curr)
				continue;
		}

		if (gData->delt == 0)
			continue;

		iCnt1 = Round(pivot / gData->delt);
		if (gData->delt * iCnt1 - pivot >= 0)
			iCnt2 = iCnt1 - 1;
		else
			iCnt2 = iCnt1 + 1;

		if (fabs(fabs(iCnt1*gData->delt) - fabs(pivot)) <= fabs(fabs(iCnt2*gData->delt) - fabs(pivot)))
			gData->jqty = iCnt1;
		else
			gData->jqty = iCnt2;

		if (gData->jqty == 0)	
			gData->jqty = 1;
		if (pivot*gData->delt*gData->jqty > 0)
			gData->jqty = gData->jqty*(-1);

		AddRow();
		m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_CODE, gData->jcod);
		m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_JNAM, gData->jnam);
		m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_JQTY, m_pApp->StringModify(gData->jqty, 0));
		m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_JMGA, m_pApp->StringModify(gData->curr, 2));

slog.Format("IB410100 [%s][%s][%d][%f]\n", gData->jcod, gData->jnam, gData->jqty, gData->curr);
OutputDebugString(slog);

		if (gData->jcod.GetAt(0) == '1' || gData->jcod.GetAt(0) == 'A') //파생상품 코드개편
		{
			m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_PGSI, m_pApp->StringModify(gData->curr*fabs(gData->jqty)*DBASE*GUARANTEE_RATE, 0));
			m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_DELT, m_pApp->StringModify(gData->delt*gData->jqty, 4));
			m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_GAMA, m_pApp->StringModify(0, 4));
			m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_CETA, m_pApp->StringModify(0, 4));
			m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_VEGA, m_pApp->StringModify(0, 4));
			m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_DELTR, m_pApp->StringModify(deltaSum + gData->delt*gData->jqty, 4));
			m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_GAMAR, m_pApp->StringModify(gamaSum, 4));
			m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_CETAR, m_pApp->StringModify(thetaSum, 4));
			m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_VEGAR, m_pApp->StringModify(vegaSum, 4));
		}
		else
		{
			m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_PGSI, m_pApp->StringModify(gData->curr*fabs(gData->jqty)*gData->gdan, 0));
			m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_DELT, m_pApp->StringModify(gData->delt*gData->jqty, 4));
			m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_GAMA, m_pApp->StringModify(gData->gama*gData->jqty, 4));
			m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_CETA, m_pApp->StringModify(gData->ceta*gData->jqty, 4));
			m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_VEGA, m_pApp->StringModify(gData->vega*gData->jqty, 4));
			m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_DELTR, m_pApp->StringModify(deltaSum + gData->delt*gData->jqty, 4));
			m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_GAMAR, m_pApp->StringModify(gamaSum + gData->gama*gData->jqty, 4));
			m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_CETAR, m_pApp->StringModify(thetaSum + gData->ceta*gData->jqty, 4));
			m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_VEGAR, m_pApp->StringModify(vegaSum + gData->vega*gData->jqty, 4));
		}
	}
	m_pGrid->Adjust();
}

void CHedgePage::CalcHedgeP()
{	
	CString slog;
	CString	str, val;
	m_SEdit.GetWindowText(str);
	const double	dMin = atof(str);
	m_EEdit.GetWindowText(str);
	const double	dMax = atof(str);

	const double	deltaSum = m_pHedgeTarget[1];
	const double	gamaSum = m_pHedgeTarget[2];
	const double	thetaSum = m_pHedgeTarget[3];
	const double	vegaSum = m_pHedgeTarget[4];
	const double	pivot = m_pHedgeTarget[0];

	val.Format("%.0f", pivot);
	str.Format("헤지대상 소요자금 : %s", AddComma(val));
	GetDlgItem(IDC_STC_HEDGETARGET)->SetWindowText(str);

	_HgridData	*gData = nullptr;
	int	iCnt=0;
	const int	nSize = m_gridArray.GetSize();
	for (int ii = 0; ii < nSize; ii++)
	{
		gData = m_gridArray.GetAt(ii);
		if (dMin != 0 || dMax != 0)
		{
			if (dMin > gData->curr || dMax < gData->curr)
				continue;
		}

		if (gData->jcod.GetAt(0) == '1' || gData->jcod.GetAt(0) == 'A')  //파생상품 코드개편
		{
			iCnt = Round(fabs(pivot) / (gData->curr * DBASE * GUARANTEE_RATE));
			if (iCnt == 0)
			{
				if (fabs(pivot) / (gData->curr * DBASE * GUARANTEE_RATE) > 0)
					iCnt = 1;
			}
			if (iCnt == 0)
				continue;

			if (pivot > 0)
				iCnt = iCnt*(-1);

			if (iCnt > 0)
			{
				if (fabs(iCnt*gData->curr * DBASE * GUARANTEE_RATE + pivot) > fabs((iCnt+1)*gData->curr * DBASE * GUARANTEE_RATE + pivot))
					iCnt = iCnt + 1;
			}
			else
			{
				if (fabs(iCnt*gData->curr * DBASE * GUARANTEE_RATE + pivot) > fabs((iCnt-1)*gData->curr * DBASE * GUARANTEE_RATE + pivot))
					iCnt = iCnt - 1;
			}
			gData->jqty = iCnt;

			if (iCnt != 0)
			{
slog.Format("IB410100 [%s][%s][%d][%f]\n", gData->jcod, gData->jnam, gData->jqty, gData->curr);
OutputDebugString(slog);
				AddRow();
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_CODE, gData->jcod);
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_JNAM, gData->jnam);
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_JQTY, m_pApp->StringModify(gData->jqty, 0));
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_JMGA, m_pApp->StringModify(gData->curr, 2));

				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_PGSI, m_pApp->StringModify(gData->curr*fabs(gData->jqty)*DBASE*GUARANTEE_RATE, 0));
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_DELT, m_pApp->StringModify(gData->delt*gData->jqty, 4));
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_GAMA, m_pApp->StringModify(0, 4));
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_CETA, m_pApp->StringModify(0, 4));
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_VEGA, m_pApp->StringModify(0, 4));
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_DELTR, m_pApp->StringModify(deltaSum + gData->delt*gData->jqty, 4));
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_GAMAR, m_pApp->StringModify(gamaSum, 4));
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_CETAR, m_pApp->StringModify(thetaSum, 4));
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_VEGAR, m_pApp->StringModify(vegaSum, 4));
			}
		}
		else
		{
			iCnt = Round(fabs(pivot) / (gData->curr * gData->gdan));
			if (iCnt == 0)
				continue;

			if (gData->jcod[0] == '2' || gData->jcod[0] == 'B')  //파생상품 코드개편
			{ 
				if (pivot > 0)
					iCnt = iCnt*(-1);
			} 
			else
			{
				if (pivot < 0)
					iCnt = iCnt*(-1);
			}

			if (iCnt > 0)
			{
				if (fabs(iCnt*gData->curr * gData->gdan + pivot) > fabs((iCnt+1)*gData->curr * gData->gdan + pivot))
					iCnt = iCnt + 1;
			}
			else
			{
				if (fabs(iCnt*gData->curr * gData->gdan + pivot) > fabs((iCnt-1)*gData->curr * gData->gdan + pivot))
					iCnt = iCnt - 1;
			}
			gData->jqty = iCnt;

			if (iCnt != 0)
			{
slog.Format("IB410100 [%s][%s][%d][%f]\n", gData->jcod, gData->jnam, gData->jqty, gData->curr);
OutputDebugString(slog);
				AddRow();
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_CODE, gData->jcod);
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_JNAM, gData->jnam);
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_JQTY, m_pApp->StringModify(gData->jqty, 0));
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_JMGA, m_pApp->StringModify(gData->curr, 2));

				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_PGSI, m_pApp->StringModify(gData->curr*fabs(gData->jqty)*gData->gdan, 0));
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_DELT, m_pApp->StringModify(gData->delt*gData->jqty, 4));
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_GAMA, m_pApp->StringModify(gData->gama*gData->jqty, 4));
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_CETA, m_pApp->StringModify(gData->ceta*gData->jqty, 4));
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_VEGA, m_pApp->StringModify(gData->vega*gData->jqty, 4));
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_DELTR, m_pApp->StringModify(deltaSum + gData->delt*gData->jqty, 4));
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_GAMAR, m_pApp->StringModify(gamaSum + gData->gama*gData->jqty, 4));
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_CETAR, m_pApp->StringModify(thetaSum + gData->ceta*gData->jqty, 4));
				m_pGrid->SetItemText(m_pGrid->GetRowCount()-1, HG_VEGAR, m_pApp->StringModify(vegaSum + gData->vega*gData->jqty, 4));
			}
		}
	}
slog.Format("IB410100 ------[%d]------\n", m_pGrid->GetRowCount());
//OutputDebugString(slog);
	m_pGrid->Adjust();
}

void CHedgePage::AddRow()
{
	m_pGrid->SetRowCount(m_pGrid->GetRowCount() + 1);
	SetRow(m_pGrid->GetRowCount() - 1);
}

void CHedgePage::SetRow(int nRow)
{
	LOGFONT	lf; lf = m_pApp->GetLogFont(m_Grid_FSize); lf.lfHeight = 90;

	GVITEM	gvitem;
	gvitem.row   = nRow;
	gvitem.mask  = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_FGCOLOR;
	gvitem.mask |= GVMK_ATTR|GVMK_MISC|GVMK_PARAM;
	gvitem.font  = lf;
	gvitem.fgcol = m_pApp->GetIndexColor(m_pView, 69);
	gvitem.bkcol = m_pApp->GetIndexColor(m_pView, 68);

	for (int ii = 0; ii < HG_MAXCOL; ii++)
	{
		gvitem.col    = ii;
		gvitem.text   = "";
		gvitem.format = HgridINFO[ii].format;
		gvitem.attr   = HgridINFO[ii].attr;
		gvitem.param  = HgridINFO[ii].param;

		CString	temp = HgridINFO[ii].defaultVal; 
		gvitem.misc = temp;

		for (int jj = 0; jj <= HgridINFO[ii].param; jj++) 
			gvitem.text = m_pApp->Parser(temp, "|");

		m_pGrid->SetItem(&gvitem);
	}
	m_pGrid->SetRowHeight(nRow, m_Grid_Height);
}

void CHedgePage::OnRadio(int index)
{
	m_RadioIdx = index;
	switch (index)
	{
	case 0:
		m_pGrid->SetItemText(0, HG_PGSI, "소요자금");
		break;
	case 1:
		m_pGrid->SetItemText(0, HG_PGSI, "헤지소요자금");
		break;
	}
	GetDlgItem(IDC_STC_HEDGETARGET)->SetWindowText("");

	DeleteGridArray();
	if (!m_pParent->SendMessage(WM_MAPHedgeData, IsHedgeable, 0))
	{
		for (int ii = 0; ii < 18; ii++)	AddRow();
		m_pGrid->Adjust();
		return;
	}

	m_pParent->SendMessage(WM_MAPHedgeData, GetHedgeData, 0);
}

void CHedgePage::InitProperties()
{
	m_Grid_Height = 20;
	m_Grid_FSize = 9;

	m_FontColor = m_pApp->GetIndexColor(m_pView, 69);
	m_cFgColor = m_pApp->GetIndexColor(m_pView, 69);
	m_cBgColor = m_pApp->GetIndexColor(m_pView, 68);
	m_sFgColor = m_pApp->GetIndexColor(m_pView, 78);
	m_hFgColor = m_pApp->GetIndexColor(m_pView, 76);
	m_hBgColor = m_pApp->GetIndexColor(m_pView, 74);
}

void CHedgePage::InitGrid()
{
	m_pGrid->Initial(20, HG_MAXCOL, 1, 0);
	LOGFONT	lf; lf = m_pApp->GetLogFont(m_Grid_FSize); lf.lfHeight = 90;

	GVITEM	gvitem;
	CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));

	for (int ii = 0 ; ii < HG_MAXCOL; ii++)
	{
		gvitem.row = 0;
		gvitem.state = 0;
		gvitem.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_ATTR;
		gvitem.format = GVFM_HEADER;
		gvitem.fgcol = m_pApp->GetIndexColor(m_pView, 76);
		gvitem.bkcol = m_pApp->GetIndexColor(m_pView, HgridINFO[ii].bkClr);

		gvitem.text = HgridINFO[ii].text;
		if (ii == 1)
			gvitem.attr = GVAT_SORTSTR;
		else
			gvitem.attr = GVAT_SORTVAL;
		gvitem.col = ii;
		m_pGrid->SetItem(&gvitem);
		m_pGrid->SetColumnWidth(ii, HgridINFO[ii].width);
	}
	m_pGrid->SetRowHeight(0, m_Grid_Height);

	m_pGrid->SetFocusCellSensitive(false);
	m_pGrid->SetStepColor(1, m_pApp->GetIndexColor(m_pView, 68), m_pApp->GetIndexColor(m_pView, 77));

	m_pGrid->SetGridLine(GVLN_BOTH);
	m_pGrid->SetGridScroll(GVSC_VERT);
	m_pGrid->SetRowColResize(FALSE, FALSE);
	m_pGrid->SetGridFocusLine(FALSE, TRUE);
	m_pGrid->SetGridColor(m_pApp->GetIndexColor(m_pView, 65));
	m_pGrid->SetBKSelColor(m_pApp->GetIndexColor(m_pView, 78));
	//m_pGrid->SetFGSelColor(m_pApp->GetIndexColor(m_pView, 65));
	m_pGrid->Adjust();
}

HBRUSH CHedgePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_HRADIO1				|| 
		pWnd->GetDlgCtrlID() == IDC_HRADIO2				||
		pWnd->GetDlgCtrlID() == IDC_HEDGE_INTERSTATIC   ||
		pWnd->GetDlgCtrlID() == IDC_HEDGE_STATIC		||
		pWnd->GetDlgCtrlID() == IDC_STC_HEDGETARGET)
	{
		pDC->SetBkColor(m_bkColor);
		return CreateSolidBrush(m_bkColor);
	}
	
	if (nCtlColor == CTLCOLOR_BTN || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(m_bkColor);
		hbr = m_bkBr;
	}

	return hbr;
}

void CHedgePage::DeleteGridArray(bool bData, bool bGrid)
{
	if (bData)
	{
		_HgridData	*gData = nullptr;
		const int nSize = m_gridArray.GetSize();
		for (int ii = 0 ; ii < nSize ; ii++)
		{
			gData = m_gridArray.GetAt(ii);
			delete gData;
		}
		m_gridArray.RemoveAll();
	}

	if (bGrid)
	{
		if (bGrid)
		{
			const int	nSize = m_pGrid->GetRowCount();
			for (int ii = 0; ii < nSize-1; ii++)
			{
				m_pGrid->DeleteRow(1);
			}
		}
	}
}

int CHedgePage::Round(double dVal)
{
	if (dVal >= 0)
		return int(dVal + 0.5);

	return int(dVal - 0.5);
}

CString CHedgePage::AddComma(CString strData)
{
	CString strBuffer = strData;
	strBuffer.TrimLeft();
	strBuffer.TrimRight();

	if (strBuffer.IsEmpty())
		return "";

	CString strSign;
	if (strBuffer[0] == '-')
	{
		strSign = "-";
		strBuffer = strBuffer.Mid(1);
	}

	const int iLength = strBuffer.GetLength();
	const int iPosPoint = strBuffer.Find('.');

	int iNaturalLength=0;

	if (iPosPoint < 0)
		iNaturalLength = iLength;
	else
		iNaturalLength = iLength - iPosPoint - 1;

	if (iNaturalLength < 4)
		return strData.GetBuffer(24);

	int iCommaCount = (iNaturalLength - 1) / 3;

	int iDest = 0;
	char acData[24]{};
	for (int ii = 0; ii < iNaturalLength; ii++)
	{
		if ((iNaturalLength - ii) % 3 == 0 && ii != 0) 
			acData[iDest++] = ',';
		acData[iDest++] = strBuffer[ii];
	}

	acData[iNaturalLength + iCommaCount] = 0;
	return strSign + CString(acData);
}
