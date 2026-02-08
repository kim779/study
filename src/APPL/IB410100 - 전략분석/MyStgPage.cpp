// MyStg.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "IB410100.h"
#include "MyStgPage.h"
#include "GridWnd.h"
#include "GrpWnd.h"
#include "MapWnd.h"
#include "Cfg.h"
#include "OpSensCal.h"
#include "../../h/axisfire.h"
#include "../../control/fx_misc/miscType.h"
#include "../../control/fx_grid/fxgrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define STG_WIDTH	200
#define GRID_WIDTH	231
#define	GRID_HEIGHT	95
#define GRID_ROWH	19
#define	TM_GETIVID	1001
#define	TM_GETIVGAP	100
#define GRP_WIDTH	480
#define CTRLBAR_HEIGHT  35
#define	GAP1		3

struct	_mygridHdr {
	char*	text;		// fixed header
	UINT	width;		// fixed width
	UINT	attr;		// fixde attr

	char	symbol[8];	// nonfixed symbol
	UINT	format;		// nonfixed format
	UINT	attrx;		// nonfixed attr
	char*	defaultVal;
} mygridHdr[] = {
	{ "종목코드",	0,	0,	"",		0,		0},			// 0	
	{ "종목명",	103,	0,	"",		GVFM_LEFT,	0},			// 1
	{ "구분",	33,	0,	"",		GVFM_CENTER,	GVAT_CONDITIONx},	// 2
	{ "수량",	39,	0,	"",		GVFM_RIGHT,	GVAT_COMMA},		// 3
	{ "주문가",	40,	0,	"",		GVFM_RIGHT,	GVAT_CONDITIONx},	// 4
};

const int gCode = 0, gName = 1, gGB = 2, gQty = 3, gPrc = 4;

/////////////////////////////////////////////////////////////////////////////
// CMyStgPage property page
[[gsl::suppress(26409)]]
IMPLEMENT_DYNCREATE(CMyStgPage, CPropertyPage)

CMyStgPage::CMyStgPage(CWnd* pView, CWnd* pParent) : CPropertyPage(CMyStgPage::IDD)
{
	//{{AFX_DATA_INIT(CMyStgPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pApp = (CIB410100App *)AfxGetApp();
	m_pView = pView;
	m_pParent = pParent;
	m_pPFGrid = NULL;
	m_pGrpWnd = NULL;
	m_pGridWnd = NULL;
	m_bRts    = false;
}

CMyStgPage::~CMyStgPage()
{
}

void CMyStgPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyStgPage)
	DDX_Control(pDX, IDC_BTN_SETMYSTG, m_btnSetStg);
	DDX_Control(pDX, IDC_BTN_DELMYSTG, m_btnDelStg);
	DDX_Control(pDX, IDC_BTN_ADDMYSTG, m_btnAddStg);
	DDX_Control(pDX, IDC_MGCHK1, m_ChkSonik);
	DDX_Control(pDX, IDC_LIST_STRATEGY, m_ListStrategy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyStgPage, CPropertyPage)
	//{{AFX_MSG_MAP(CMyStgPage)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SETMYSTG, OnBtnStg)
	ON_BN_CLICKED(IDC_BTN_ADDMYSTG, OnBtnAddstg)
	ON_BN_CLICKED(IDC_BTN_DELMYSTG, OnBtnDel)
	ON_BN_CLICKED(IDC_MGCHK1, OnMgchk1)
	ON_BN_CLICKED(IDC_MGCHK2, OnMgchk2)
	ON_BN_CLICKED(IDC_MGCHK3, OnMgchk3)
	ON_BN_CLICKED(IDC_MGCHK4, OnMgchk4)
	ON_BN_CLICKED(IDC_MGCHK5, OnMgchk5)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_LIST_STRATEGY, OnClickListStrategy)
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_POSInitData, OnInitData)
	ON_MESSAGE(WM_POSMSVAL, OnMsgMouseValue)
	ON_MESSAGE(WM_POSGetHV, OnMsgGetHV)
	ON_MESSAGE(WM_POSGetFee, OnMsgGetFee)
	ON_MESSAGE(WM_MAPRefresh, OnMsgRefreshList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyStgPage message handlers

int CMyStgPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CMyStgPage::OnDestroy() 
{
	m_bRts = false;
	RemoveProject();

	//DELWND(m_pPFGrid);
	//DELWND(m_pGrpWnd);
	//DELWND(m_pGridWnd);

	CPropertyPage::OnDestroy();
}

BOOL CMyStgPage::OnSetActive() 
{
	//::SetWindowPos( m_hWnd, HWND_TOP, 1, 1, 0, 0, SWP_NOSIZE); 
	CRect rcParent;
	const int nGuideBarHeight = 20;
	GetParent()->GetClientRect(&rcParent);
	::SetWindowPos( m_hWnd, HWND_TOP, rcParent.left, rcParent.top, rcParent.right, rcParent.bottom, 0); 
	setList();
	
	return CPropertyPage::OnSetActive();
}

void CMyStgPage::initGrid()
{
	const int count = sizeof(mygridHdr) / sizeof(_mygridHdr);
	m_pPFGrid->Initial(12, count, 1, 0);


	LOGFONT lf; m_pFont->GetLogFont(&lf); lf.lfHeight = 90;
	GVITEM	gvitem;
	CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));

 	// column header setting
	for (int ii = 0; ii < count; ii++)
	{
		gvitem.row   = 0;
		gvitem.col   = ii;
		gvitem.state = 0;
		gvitem.mask  = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_MISC;
		gvitem.format= GVFM_HEADER;
		gvitem.fgcol = m_pApp->GetIndexColor(m_pView, 76);
		gvitem.bkcol = m_pApp->GetIndexColor(m_pView, 74);

		gvitem.text  = mygridHdr[ii].text;
		gvitem.attr  = mygridHdr[ii].attr;
		m_pPFGrid->SetItem(&gvitem);
	};

	for (int ii = 0; ii < count; ii++)
		m_pPFGrid->SetColumnWidth(ii, mygridHdr[ii].width);

	for (int ii = 0; ii < count; ii++)
	{
		gvitem.state = 0;
		gvitem.mask  = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_SYMBOL;

		gvitem.format= mygridHdr[ii].format;
		gvitem.symbol= mygridHdr[ii].symbol; 
		gvitem.attr  = mygridHdr[ii].attrx;
		gvitem.fgcol = m_pApp->GetIndexColor(m_pView, 69);
		gvitem.bkcol = m_pApp->GetIndexColor(m_pView, 68);

		m_pPFGrid->SetColumnItems(ii, &gvitem);
	}

	for (int ii = 0; ii < m_pPFGrid->GetRowCount(); ii++)
		m_pPFGrid->SetRowHeight(ii, GRID_ROWH);

	//m_pPFGrid->Setm_pPFGridDIScroll(false);
	//m_pPFGrid->Setm_pPFGridScroll(false);
	m_pPFGrid->SetFocusCellSensitive(false);
	m_pPFGrid->SetStepColor(1, m_pApp->GetIndexColor(m_pView, 68), m_pApp->GetIndexColor(m_pView, 77));

	m_pPFGrid->SetGridLine(GVLN_BOTH);
	m_pPFGrid->SetGridScroll(GVSC_VERT);
	m_pPFGrid->SetRowColResize(FALSE, FALSE);
	m_pPFGrid->SetGridFocusLine(FALSE, TRUE);
	m_pPFGrid->SetGridColor(m_pApp->GetIndexColor(m_pView, 65));
	m_pPFGrid->SetBKSelColor(m_pApp->GetIndexColor(m_pView, 78));
	//m_pPFGrid->SetFGSelColor(m_pApp->GetIndexColor(m_pView, 65));
	m_pPFGrid->Adjust();
}

void CMyStgPage::initGridRow(int row)
{
	LOGFONT lf; m_pFont->GetLogFont(&lf); lf.lfHeight = 90;
	GVITEM	gvitem;

	gvitem.row   = row;
	gvitem.mask  = /*GVMK_TEXT|*/GVMK_FORMAT|GVMK_FONT|GVMK_FGCOLOR|GVMK_BKCOLOR;
	gvitem.mask |= GVMK_ATTR;
	gvitem.font  = lf;
	gvitem.fgcol = m_pApp->GetIndexColor(m_pView, 69);
	gvitem.bkcol = m_pApp->GetIndexColor(m_pView, 68);
	CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));

	for (int ii = 0; ii < m_pPFGrid->GetColumnCount(); ii++)
	{
		gvitem.state = 0;
		gvitem.col    = ii;
		//gvitem.text   = "";
		gvitem.format= mygridHdr[ii].format;			

		gvitem.attr  = mygridHdr[ii].attrx;
		m_pPFGrid->SetItem(&gvitem);
	}
	m_pPFGrid->SetRowHeight(row, GRID_ROWH);
}

void CMyStgPage::setList()
{
	CFileFind finder;
	CString	file, strNo;
	CHAR inBuf[80]{};

	m_ListStrategy.DeleteAllItems();
	
	bool bContinue = TRUE;
	int ii = 0;
	while (bContinue)
	{
		strNo.Format("%02d", ii);
		file.Format("%s\\%s%s", m_sPath, STRATEGY_FILE, strNo);

		if (finder.FindFile(file))
		{
			ZeroMemory(inBuf, sizeof(inBuf));
			GetPrivateProfileString (SN_NAME, SN_PROJ, NULL, inBuf, 80, file);

			m_ListStrategy.InsertItem(LVIF_TEXT, ii, CString(inBuf), NULL, NULL, 0, 0);
		}
		else
			bContinue = FALSE;

		ii++;
	}
}

BOOL CMyStgPage::addColumn(CListCtrl *LC, LPCTSTR str, int item, int width)
{
	LV_COLUMN lvc;

	lvc.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt     = LVCFMT_LEFT;
	lvc.pszText = (LPTSTR) str;
	lvc.cx      = LC->GetStringWidth(lvc.pszText) + width;
//	lvc.iSubItem = -1;

	return LC->InsertColumn(item, &lvc);
}


BOOL CMyStgPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_pFont = m_pApp->GetFont(m_pView, 9);
	m_bkColor = m_pApp->GetIndexColor(m_pView, 66);
	m_bkBr.CreateSolidBrush(m_bkColor);
	
	CRect rcClient, rc;
	GetClientRect(&rcClient);
	m_sPath.Format("%s", ((CMapWnd*)m_pParent)->m_sSavePath);

	m_btnAddStg.SetImgBitmap(((CMapWnd*)m_pParent)->m_bmp4BTN, ((CMapWnd*)m_pParent)->m_bmp4BTN_DN, ((CMapWnd*)m_pParent)->m_bmp4BTN_EN);
	m_btnAddStg.SetFont(m_pFont);
	m_btnSetStg.SetImgBitmap(((CMapWnd*)m_pParent)->m_bmp4BTN, ((CMapWnd*)m_pParent)->m_bmp4BTN_DN, ((CMapWnd*)m_pParent)->m_bmp4BTN_EN);
	m_btnSetStg.SetFont(m_pFont);
	m_btnDelStg.SetImgBitmap(((CMapWnd*)m_pParent)->m_bmp2BTN, ((CMapWnd*)m_pParent)->m_bmp2BTN_DN, ((CMapWnd*)m_pParent)->m_bmp2BTN_EN);
	m_btnDelStg.SetFont(m_pFont);

	rc = CRect(STG_WIDTH, GAP1, STG_WIDTH + GRID_WIDTH, 3 + GRID_HEIGHT);
	m_pPFGrid = std::make_unique<CfxGrid>();
	m_pPFGrid->Create(rc, this, ID_JANGOGRID, GVSC_VERT, GVDD_FULL, WS_CHILD|WS_TABSTOP|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);
	m_pPFGrid->SetFont(m_pFont);
	initGrid();

	rcClient.DeflateRect(2, 2);

	rc.top = GAP1 + GRID_HEIGHT + GAP1;
	rc.bottom = rcClient.bottom;
	m_pGridWnd = std::make_unique<CGridWnd>(m_pView, m_pParent);
	m_pGridWnd->m_nRow = 8;
	m_pGridWnd->m_fcolWidth = 80;
	m_pGridWnd->Create(NULL, "", WS_VISIBLE|WS_CHILD, rc, this, -1);
	m_pGridWnd->SetGridItemText(0, 1, "만기/이론");

	rc.top = CTRLBAR_HEIGHT + GAP1;
	rc.left = rc.right + GAP1;
	rc.right = rc.left + GRP_WIDTH;
	m_pGrpWnd = std::make_unique<CGrpWnd>(m_pView, m_pParent);
	m_pGrpWnd->Create(NULL, "", WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, rc, this, -1);
	m_pGrpWnd->m_bMoveStg = false;

	addColumn(&m_ListStrategy, "전략명", 0, 128);
	m_ListStrategy.SetExtendedStyle(LVS_EX_FULLROWSELECT);	

	((CMapWnd*)m_pParent)->m_idxX = 0;	// 만기손익체크
	((CMapWnd*)m_pParent)->m_idxVar = 0;	// H.V

	m_ChkSonik.SetCheck(1);	// 손익체크
	m_pGrpWnd->SendMessage(WM_GRPRefresh, SetSensitive, SONIK);
	m_pGridWnd->SetGridItemText(0, 1, "만기/이론");

	SetTimer(TM_GETIVID, TM_GETIVGAP, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMyStgPage::OnClickListStrategy(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int	nItem=0;
	CHAR	inBuf[80]{};
	CString	file, strNo, val;
	CFileFind finder;

	RemoveProject();
	m_pPFGrid->Clear();

	POSITION pos = m_ListStrategy.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		nItem = m_ListStrategy.GetNextSelectedItem(pos);
		strNo.Format("%02d", nItem);
		file.Format("%s\\%s%s", m_sPath, STRATEGY_FILE, strNo);

		if (finder.FindFile(file))
		{
			nItem = 0;
			bool bContinue = true;
			while (bContinue)
			{
				strNo.Format("%02d", nItem);
				ZeroMemory(inBuf, sizeof(inBuf));
				GetPrivateProfileString (SN_CODE, strNo, NULL, inBuf, 80, file);
				val.Format("%s", inBuf); val.TrimLeft();
			
				if (val.IsEmpty()) {
					bContinue = false;
					break;
				}

				setGridItem(nItem+1, val);
				nItem++;
			}
			queryMingam();
		}
	}
			
	*pResult = 0;
}

CString CMyStgPage::Parse(CString &dat, CString delimiter)
{
	int pos;
	CString val;
	pos = dat.Find(delimiter);
	if (pos >= 0)
	{
		val = dat.Left(pos++);
		dat = dat.Mid(pos);
	}
	else
	{
		val = dat;
		dat.Empty();
	}
	return val;
}

void CMyStgPage::setGridItem(int row, CString dat)
{
	if (row >= m_pPFGrid->GetRowCount())
		m_pPFGrid->SetRowCount(row + 1);
	initGridRow(row);

	CString val, tmp;
	CString code;
	int num=0, mmgb=0;
	double prc=0;
	val = Parse(dat);
	code.Format("%s", val.GetString());
	m_pPFGrid->SetItemText(row, gCode, val);
	
	CString cname;
	cname.Format("%s", (char*)m_pView->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (long)(LPSTR)(LPCTSTR)val));
	cname.TrimRight(); 
	if (cname.IsEmpty()) 
		cname.Format("%s(만기)", code);
	m_pPFGrid->SetItemText(row, gName, cname);

	val = Parse(dat);	mmgb = atoi(val);
	if (atoi(val) == 1)	m_pPFGrid->SetItemText(row, gGB, "+매수");
	else			m_pPFGrid->SetItemText(row, gGB, "-매도");
	val = Parse(dat);	num = atoi(val);
	m_pPFGrid->SetItemText(row, gQty, val);
	val = Parse(dat);	prc = atof(val);
	m_pPFGrid->SetItemText(row, gPrc, val);
	AddPCode(code, num, mmgb, prc);
}

void CMyStgPage::AddPCode(CString Code, int Num, int mmgb, double prc)
{
	_ProjectCode	ProjectCode;

	ProjectCode.Code = Code;
	ProjectCode.Num = Num;
	ProjectCode.mmgb = mmgb;
	ProjectCode.jprc = prc;

	m_ProjectCode.Add(ProjectCode);
}

void CMyStgPage::OnBtnStg()
{
	if (m_ProjectCode.GetSize() <= 0)
		return;

	_userMsg userMsg;
	userMsg.kind = TR_StgApply;
	userMsg.nSize = m_ProjectCode.GetSize();

	m_pParent->SendMessage(WM_MAPStgApply, (WPARAM) &m_ProjectCode[0], (LPARAM) (char*) &userMsg);	
}

void CMyStgPage::OnBtnAddstg() 
{
	if (m_ProjectCode.GetSize() <= 0)
		return;

	_userMsg userMsg;
	userMsg.kind = TR_StgApply;
	userMsg.nSize = m_ProjectCode.GetSize();

	m_pParent->SendMessage(WM_MAPStgAdd, (WPARAM) &m_ProjectCode[0], (LPARAM) (char*) &userMsg);	
}

void CMyStgPage::OnBtnDel() 
{
	int	nItem=0;
	CString	file, strNo;
	CFileFind finder;

	RemoveProject();
	m_pPFGrid->Clear();

	POSITION pos = m_ListStrategy.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		nItem = m_ListStrategy.GetNextSelectedItem(pos);
		m_ListStrategy.DeleteItem(nItem);

		strNo.Format("%02d", nItem);
		file.Format("%s\\%s%s", m_sPath, STRATEGY_FILE, strNo);
		if (finder.FindFile(file))
		{
			DeleteFile(file);	
		}
	}	
}

void CMyStgPage::OnMgchk1() 
{
	const int chk = ((CButton *)GetDlgItem(IDC_MGCHK1))->GetCheck();
	CheckMinGam();
	m_pGrpWnd->SendMessage(WM_GRPRefresh, ResetSensitive+chk, SONIK);
	m_pGridWnd->SetGridItemText(0, 1, "만기/이론");
}

void CMyStgPage::OnMgchk2() 
{
	const int chk = ((CButton *)GetDlgItem(IDC_MGCHK2))->GetCheck();
	CheckMinGam();
	m_pGrpWnd->SendMessage(WM_GRPRefresh, ResetSensitive+chk, DELTA);
}

void CMyStgPage::OnMgchk3() 
{
	const int chk = ((CButton *)GetDlgItem(IDC_MGCHK3))->GetCheck();
	CheckMinGam();
	m_pGrpWnd->SendMessage(WM_GRPRefresh, ResetSensitive+chk, GAMMA);
}

void CMyStgPage::OnMgchk4() 
{
	const int chk = ((CButton *)GetDlgItem(IDC_MGCHK4))->GetCheck();
	CheckMinGam();
	m_pGrpWnd->SendMessage(WM_GRPRefresh, ResetSensitive+chk, CETA);	
}

void CMyStgPage::OnMgchk5() 
{
	const int chk = ((CButton *)GetDlgItem(IDC_MGCHK5))->GetCheck();
	CheckMinGam();
	m_pGrpWnd->SendMessage(WM_GRPRefresh, ResetSensitive+chk, VEGA);	
}

void CMyStgPage::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TM_GETIVID)
	{
		KillTimer(nIDEvent);
		//m_pParent->SendMessage(WM_MAPUser, MAKEWPARAM(RequestIV, 0), 0);
	}
	
	CPropertyPage::OnTimer(nIDEvent);
}

void CMyStgPage::CheckMinGam()
{
	int	chknum = 0;
	CString	names = "";
	const int		pID[4] = {IDC_MGCHK2, IDC_MGCHK3, IDC_MGCHK4, IDC_MGCHK5};
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
		const int chk = 1; //((CButton *)GetDlgItem(IDC_MGRADIOX1))->GetCheck();
		if (chk)	m_pGridWnd->SetGridItemText(0, 1, "만기/이론");
		else		m_pGridWnd->SetGridItemText(0, 1, "이론손익");
	}
	else
	{
		int		no = 0;
		CString	text[4] = {"델타", "감마", "세타", "베가"};
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

void CMyStgPage::InitData(struct _bdat *bdat)
{
	CString fcod = bdat->frts;
	CString cdkr; cdkr.Format("%.2f", bdat->cdgl);
	CString yjbs_21; yjbs_21.Format("%.2f", bdat->hv[0]);
	CString yjbs_30; yjbs_30.Format("%.2f", bdat->hv[1]);
	CString yjbs_60; yjbs_60.Format("%.2f", bdat->hv[2]);
	CString yjbs_90; yjbs_90.Format("%.2f", bdat->hv[3]);

	m_pGrpWnd->m_cdkr = bdat->cdgl / 100;
	m_pGrpWnd->m_k200 = bdat->basicprc;

	m_sHval[0] = yjbs_21;
	m_sHval[1] = yjbs_30;
	m_sHval[2] = yjbs_60;
	m_sHval[3] = yjbs_90;
}

LRESULT CMyStgPage::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		switch (HIBYTE(LOWORD(wParam)))
		{
		case KEY_CODEMINGAM2: parseMingam((char*)lParam);
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

void CMyStgPage::parseMingam(char *dat)
{
	struct _mod *mod;
	mod = (struct _mod *)dat;
	
	m_ListStrategy.EnableWindow(TRUE);
	/*
	m_bData->frts = CString(mod->frts, sizeof(mod->frts));
	m_bData->fnam = CString(mod->fnam, sizeof(mod->fnam));
	m_bData->fcur = atof(CString(mod->fcur, sizeof(mod->fcur)));
	m_bData->krts = CString(mod->krts, sizeof(mod->krts));
	m_bData->kcur = atof(CString(mod->kcur, sizeof(mod->kcur)));
	m_bData->k200rts = CString(mod->k200rts, sizeof(mod->k200rts));
	m_bData->k200cur = atof(CString(mod->k200cur, sizeof(mod->k200cur)));
	for (int ii =0; ii < 4; ii++)
		m_bData->hv[ii] = atof(CString(mod->hv[ii], sizeof(mod->hv[ii])));
	m_bData->cdgl = atof(CString(mod->cdgl, sizeof(mod->cdgl)));
	m_bData->bdjs = atof(CString(mod->bdjs, sizeof(mod->bdjs)));
// Test********
m_bData->cdgl = 0;
	generateBasicPrice();
	generateFData();
	*/

	int pos=0, idx=0;
	CString code, firstcode("");
	gsl::owner<struct _mdat*> mdat = nullptr;
	int nRec = atoi(CString(mod->grec, sizeof(mod->grec)));
	for (int ii = 0; ii < nRec; ii++)
	{
		struct  _gmod *gmod = &mod->gmod[ii];
		code.Format("%s", CString(gmod->ccod, sizeof(gmod->ccod))); code.TrimRight();

		//2012.04.06 KSJ X 붙어있음
		if(code.GetAt(0) == 'x' || code.GetAt(0) == 'X')
		{
			code = code.Mid(1);
// 			OutputDebugString(code);
		}
		// KSJ

		if (firstcode.IsEmpty()) firstcode = code;
		pos = findMData(code);
		if (pos >= 0)
		{
			mdat = m_mData.GetAt(pos);
		}
		else
		{
			mdat = new _mdat;
			mdat->ccod.Format("%s", code);
			idx = findGData(code);
			//if (idx >= 0 && m_ProjectCode.GetAt(idx).jprc == 0)
			//	m_ProjectCode.GetAt(idx)->prc = fabs(atof(CString(gmod->ccur, sizeof(gmodccur))));
		}
		mdat->ccur = fabs(atof(CString(gmod->ccur, sizeof(gmod->ccur))));
		mdat->shga = atof(CString(gmod->shga, sizeof(gmod->shga)));
		mdat->hhga = atof(CString(gmod->hhga, sizeof(gmod->hhga)));
		for (int jj = 0; jj < 5; jj++)
		{
			mdat->mdga[jj] = atof(CString(gmod->mdga[jj], sizeof(gmod->mdga[0])));
			mdat->msga[jj] = atof(CString(gmod->msga[jj], sizeof(gmod->msga[0])));
		}
		mdat->jjil[0] = atof(CString(gmod->jjil[0], sizeof(gmod->jjil[0])));
		mdat->jjil[1] = atof(CString(gmod->jjil[1], sizeof(gmod->jjil[1])));

		if (pos < 0) m_mData.Add(mdat);
		generateMingam(m_mData.GetSize() -1);
	}
	SetGraph();
	m_bRts = true;
}

LRESULT CMyStgPage::OnInitData(WPARAM wParam, LPARAM lParam)
{
	InitData((struct _bdat *)wParam);

	return 0;
}

long CMyStgPage::OnMsgMouseValue(WPARAM wParam, LPARAM lParam)
{
	CString sValue;

	_MouseValue* MouseValue = (_MouseValue*) wParam;
	for (int ii = 0 ; ii < m_pGridWnd->m_nRow ; ii++)
	{
		if (ii != 0)
			MouseValue = MouseValue + 1;

		sValue.Format("%s", MouseValue->sValue);
		m_pGridWnd->SetGridItemText(ii + 1, 0, MouseValue->kpt);
		m_pGridWnd->SetGridItemText(ii + 1, 1, sValue);
	}
	return 0;
}

long CMyStgPage::OnMsgGetHV(WPARAM wParam, LPARAM lParam)
{
	CString str, val;
	_GraphData *pData = (_GraphData *)lParam;

	((CMapWnd*)m_pParent)->m_opsenscal->SetJCode(pData->code);
	val.Format("%.2f", fabs(pData->curr));		((CMapWnd*)m_pParent)->m_opsenscal->SetCurrValue(val);
	val.Format("%.1f", pData->hsga);		((CMapWnd*)m_pParent)->m_opsenscal->SetStrike(val);
	val.Format("%.2f", ((CMapWnd*)m_pParent)->m_bData->cdgl);	((CMapWnd*)m_pParent)->m_opsenscal->SetCd(val);
	val.Format("%.2f", m_pGrpWnd->m_k200);		((CMapWnd*)m_pParent)->m_opsenscal->SetK200Cur(val);
	val.Format("%.2f", m_pGrpWnd->m_k200);		((CMapWnd*)m_pParent)->m_opsenscal->SetFutureCur(val);
	val.Format("%.2f", ((CMapWnd*)m_pParent)->m_bData->bdjs);	((CMapWnd*)m_pParent)->m_opsenscal->SetBedangJisu(val);
	val.Format("%.2f", pData->jjis[0]);		((CMapWnd*)m_pParent)->m_opsenscal->SetBzRemainDays(val);
	val.Format("%.2f", pData->jjis[1]);		((CMapWnd*)m_pParent)->m_opsenscal->SetCalRemainDays(val);
	((CMapWnd*)m_pParent)->m_opsenscal->Calc(1);	// IV 만 계산
	val = ((CMapWnd*)m_pParent)->m_opsenscal->GetIv();

	if (pData->code[0] == '2' || pData->code[0] == 'B') //파생상품 코드개편
	{
		val.Format("%.2f", atof(val));
	}
	else
	{
		val.Format("%.2f", atof(val));
	}
	m_sVal.Format("%s", val);

	/*
	CString val;
	_GraphData *pData = (_GraphData *)lParam;

	switch (atoi(((CMapWnd*)m_pParent)->m_cfg->ReadCfg("VolType")))
	{
	case 0: // 종목별내재변동성
	case 1: // 콜풋 ATM 내재변동성
		{
			CString val;

			((CMapWnd*)m_pParent)->m_opsenscal->SetJCode(pData->code);
			val.Format("%.2f", fabs(pData->curr));		((CMapWnd*)m_pParent)->m_opsenscal->SetCurrValue(val);
			val.Format("%.1f", pData->hsga);		((CMapWnd*)m_pParent)->m_opsenscal->SetStrike(val);
			val.Format("%.2f", ((CMapWnd*)m_pParent)->m_bData->cdgl);	((CMapWnd*)m_pParent)->m_opsenscal->SetCd(val);
			val.Format("%.2f", m_pGrpWnd->m_k200);		((CMapWnd*)m_pParent)->m_opsenscal->SetK200Cur(val);
			val.Format("%.2f", m_pGrpWnd->m_k200);		((CMapWnd*)m_pParent)->m_opsenscal->SetFutureCur(val);
			val.Format("%.2f", ((CMapWnd*)m_pParent)->m_bData->bdjs);	((CMapWnd*)m_pParent)->m_opsenscal->SetBedangJisu(val);
			val.Format("%.2f", pData->jjis[0]);		((CMapWnd*)m_pParent)->SetBzRemainDays(val);
			val.Format("%.2f", pData->jjis[1]);		((CMapWnd*)m_pParent)->m_opsenscal->SetCalRemainDays(val);
			((CMapWnd*)m_pParent)->m_opsenscal->Calc(1);	// IV 만 계산
			val = ((CMapWnd*)m_pParent)->m_opsenscal->GetIv();
//	CString sTest;
//	sTest.Format("IC [%s] prc(%f) strike(%f) k200(%f) bdjs(%f), 거래일(%f) iv(%s)\n", 
//		     pData->code, fabs(pData->curr), pData->hsga, fabs(m_pGrpWnd->m_k200), (((CMapWnd*)m_pParent)->m_bData->bdjs), pData->jjis[0], val);
//	OutputDebugString(sTest);
			double	iv = atof(val);
			m_sVal.Format("%2f", iv);
		}
		break;
	case 2: // 사용자정의
		if (pData->code[0] == '2')
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
	*/
	return (LONG) m_sVal.operator LPCTSTR();
}

long CMyStgPage::OnMsgGetFee(WPARAM wParam, LPARAM lParam)
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

long CMyStgPage::OnMsgRefreshList(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case UpdateUserList:
		setList();
		break;
	}
	return 0;
}

void CMyStgPage::sendTR(CString trCode, char* datB, int datL, int nKey, int nStat)
{
	std::unique_ptr<char[]> sendB = std::make_unique<char[]>(L_userTH + datL + 1);

	struct	_userTH* uTH;
	uTH = (struct _userTH *) sendB.get();

	strcpy(uTH->trc, trCode);
	uTH->key = nKey;
	uTH->stat = nStat;

	CopyMemory(&sendB.get()[L_userTH], datB, datL);
	m_pView->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datL), (LPARAM) sendB.get());

}

void CMyStgPage::queryMingam()
{
	if (m_ProjectCode.GetSize() == 0)
		return;

	m_bRts = false;
	m_ListStrategy.EnableWindow(FALSE);

	struct _mid mid;
	FillMemory(&mid, sizeof(struct _mid), ' ');

	CString val, param;
	param.Format("%%.%dd", sizeof(mid.crec));
	val.Format(param, m_ProjectCode.GetSize());
	CopyMemory(&mid.crec, val.GetBuffer(val.GetLength()), sizeof(mid.crec));
	val.ReleaseBuffer();

	for (int ii = 0; ii < m_ProjectCode.GetSize(); ii++)
	{
		val = m_ProjectCode.GetAt(ii).Code;
		CopyMemory(&mid.codes[ii], val.GetBuffer(val.GetLength()), sizeof(mid.codes[0]));
	}

	sendTR(TRN_MINGAM, (char*)&mid, 3 + 8 * m_ProjectCode.GetSize()/*sizeof(struct _mid)*/, KEY_CODEMINGAM2, 0);
}

void CMyStgPage::SetGraph()
{
	// Delete
	m_pGrpWnd->SendMessage(WM_GRPRefresh, DeleteGrpData, 0);
	
	int nMData{};
	_GraphData GData{};
	struct _ProjectCode pjcode;
	struct _mdat* mdat = nullptr;
	CString fistcode("");
	for (int ii = 0; ii < m_ProjectCode.GetSize(); ii++)
	{
		pjcode = m_ProjectCode.GetAt(ii);
		nMData = findMData(pjcode.Code);
		if (nMData == -1) return;
		mdat = m_mData.GetAt(nMData);
		
		sprintf(GData.code, "%s", pjcode.Code.GetString());	// 종목코드
		GData.curr = mdat->ccur;		// 현재가
		GData.prc  = pjcode.jprc;		// 주문가
		GData.numb = pjcode.Num;		// 주문수량
		GData.mmgb = pjcode.mmgb;		// 매매구분(-1. 매도, 1. 매수)
		GData.jjis[0] = mdat->jjil[0];		// 잔존만기일(거래일수)
		GData.jjis[1] = mdat->jjil[1];		// 잔존만기일(달력일수)
//		GData.hvbd = ((CMapWnd*)m_pParent)->m_bData->hv[4];	// 역사적 변동성
		GData.hvbd = ((CMapWnd*)m_pParent)->m_bData->hv[3];	// 역사적 변동성

		if (fistcode.IsEmpty())
			fistcode = GData.code;

		m_pGrpWnd->SendMessage(WM_GRPAddData, 0, (long)(LPTSTR)(LPCTSTR)&GData);
	}
	// ReDraw
	m_pGrpWnd->SendMessage(WM_GRPRefresh, RedrawGraph, 0);
}

int CMyStgPage::findGrid(class CfxGrid* pGrid, CString code)
{
	CString val;
	int idx = -1;
	if (code.IsEmpty()) return idx;

	for (int ii = 1; ii < pGrid->GetRowCount(); ii++)
	{
		val = pGrid->GetItemText(ii, gCode);
		if (val.CompareNoCase(code) == 0)
		{
			idx = ii;
			break;
		}
	}
	return idx;
}

int CMyStgPage::findMData(CString code)
{
	int idx = -1;
	struct _mdat* mDat = nullptr;
	for (int ii = 0; ii < m_mData.GetSize(); ii++)
	{
		mDat = (struct _mdat*)m_mData.GetAt(ii);
		if (code.CompareNoCase(mDat->ccod) == 0)
		{
			idx = ii;
			break;
		}
	}
	return idx;
}

int CMyStgPage::findGData(CString code)
{
	int idx = -1;
	struct _ProjectCode gdat;
	for (int ii = 0; ii < m_ProjectCode.GetSize(); ii++)
	{
		gdat = m_ProjectCode.GetAt(ii);
		if (code.CompareNoCase(gdat.Code) == 0)
		{
			idx = ii;
			break;
		}
	}
	return idx;
}

void CMyStgPage::deleteMData(CString code)
{
	for (int ii = m_mData.GetSize() -1; ii >= 0; ii--)
	{
		struct _mdat* mdat = (struct _mdat*)m_mData.GetAt(ii);
		if (code.CompareNoCase(mdat->ccod) == 0)
		{
			m_mData.RemoveAt(ii);

			mdat->ccod.Empty();
			mdat->iv.Empty();
			//mdat->theoryamt.Empty();
			delete mdat;
			break;
		}
	}
}

void CMyStgPage::generateMingam(int idx)
{
	if (idx == -1) return;

	struct  _mdat *mdat = m_mData.GetAt(idx);
	if (mdat->ccod.IsEmpty()) return;

	// 민감도 계산(iv, delta, gamma, theta, vegga, rho)
	if (mdat->ccod[0] == '1' || mdat->ccod[0] == '4' || mdat->ccod[0] == 'A' || mdat->ccod[0] == 'D') //파생상품 코드개편
	{
		// 선물 민감 계산
		mdat->iv = "0";
// 		mdat->delta = 5 * 100;
		mdat->delta = 100;	//2012.10.12 KSJ 선물과 옵션이 동일하게 50만원이므로.. *5해주지 않음
		mdat->gamma = 0;
		mdat->theta = 0;
		mdat->vegga = 0;
	}
	else
	{
		CString code, val, opt;
		code.Format("%s", mdat->ccod);
		const double strike = m_pApp->getOptionStrike(code);

		((CMapWnd*)m_pParent)->m_opsenscal->SetJCode(code);
		val.Format("%.2f", mdat->ccur);		((CMapWnd*)m_pParent)->m_opsenscal->SetCurrValue(val);
//CString sTest;
//sTest.Format("IC [%s] strike(%f) bprc(%f) bdjs(%f), 거래일(%f), 달력일(%f) Cdgl(%f)\n", 
//	     code, strike, m_bData->basicprc, m_bData->bdjs, mdat->jjil[0], mdat->jjil[1], m_bData->cdgl);
//OutputDebugString(sTest);
		val.Format("%.1f", strike);		((CMapWnd*)m_pParent)->m_opsenscal->SetStrike(val);
		val.Format("%.2f", ((CMapWnd*)m_pParent)->m_bData->cdgl);	((CMapWnd*)m_pParent)->m_opsenscal->SetCd(val);
		val.Format("%.2f", ((CMapWnd*)m_pParent)->m_bData->k200cur);	((CMapWnd*)m_pParent)->m_opsenscal->SetK200Cur(val);
		val.Format("%.2f", ((CMapWnd*)m_pParent)->m_bData->fcur);	((CMapWnd*)m_pParent)->m_opsenscal->SetFutureCur(val); 

		val.Format("%.2f", ((CMapWnd*)m_pParent)->m_bData->bdjs);	((CMapWnd*)m_pParent)->m_opsenscal->SetBedangJisu(val);
		val.Format("%.2f", mdat->jjil[0]);	((CMapWnd*)m_pParent)->m_opsenscal->SetBzRemainDays(val);	// 영업잔존일,
		val.Format("%.2f", mdat->jjil[1]);	((CMapWnd*)m_pParent)->m_opsenscal->SetCalRemainDays(val);	// 달력잔존일
		((CMapWnd*)m_pParent)->m_opsenscal->Calc(0);

		/*
		((CMapWnd*)m_pParent)->m_opsenscal->SetJCode(code);
		val.Format("%.2f", mdat->ccur);
//CString sTest;
//sTest.Format("IC [%s] strike(%f) bprc(%f) bdjs(%f), 거래일(%f), 달력일(%f) Cdgl(%f)\n", 
//	     code, strike, m_bData->basicprc, m_bData->bdjs, mdat->jjil[0], mdat->jjil[1], m_bData->cdgl);
//OutputDebugString(sTest);
		((CMapWnd*)m_pParent)->m_opsenscal->SetCurrValue(val);
		val.Format("%.1f", strike);
		((CMapWnd*)m_pParent)->m_opsenscal->SetStrike(val);
		val.Format("%.2f", ((CMapWnd*)m_pParent)->m_bData->cdgl);
		((CMapWnd*)m_pParent)->m_opsenscal->SetCd(val);
		val.Format("%.2f", ((CMapWnd*)m_pParent)->m_bData->basicprc);
		((CMapWnd*)m_pParent)->m_opsenscal->SetUnderlying(val);
		val.Format("%.2f", ((CMapWnd*)m_pParent)->m_bData->bdjs);
		((CMapWnd*)m_pParent)->m_opsenscal->SetBedangJisu(val);
		if (atoi(((CMapWnd*)m_pParent)->m_cfg->ReadCfg("RDType")) == 0)
			val.Format("%.2f", mdat->jjil[0]);	// 거래일수
		else
			val.Format("%.2f", mdat->jjil[1]);	// 달력일수
		((CMapWnd*)m_pParent)->m_opsenscal->SetRemainDays(val);

		opt = ((CMapWnd*)m_pParent)->m_cfg->ReadCfg("VolType");
		switch (atoi(opt))
		{
		case 0: // 종목별내재변동성
			val = ((CMapWnd*)m_pParent)->m_opsenscal->GetIv();
			break;
		case 1: // 콜풋 ATM 내재변동성
			val = ((CMapWnd*)m_pParent)->m_opsenscal->GetIv();
			break;
		case 2: // 사용자정의
			if (mdat->ccod[0] == '2')
				val = ((CMapWnd*)m_pParent)->m_cfg->ReadCfg("UserCallIV");
			else
				val = ((CMapWnd*)m_pParent)->m_cfg->ReadCfg("UserPutIV");
			break;
		default: // 역사적 변동성 적용
			val = ((CMapWnd*)m_pParent)->m_cfg->ReadCfg("HVDay");
			switch (atoi(val))
			{
			case 90:
				val.Format("%.2f",  ((CMapWnd*)m_pParent)->m_bData->hv[0]);
				break;
			case 60:
				val.Format("%.2f",  ((CMapWnd*)m_pParent)->m_bData->hv[1]);
				break;
			case 30:
				val.Format("%.2f",  ((CMapWnd*)m_pParent)->m_bData->hv[2]);
				break;
			case 21:
				val.Format("%.2f",  ((CMapWnd*)m_pParent)->m_bData->hv[3]);
				break;
			}
		}
		((CMapWnd*)m_pParent)->m_opsenscal->SetVolatility(val);
		*/

		mdat->iv = ((CMapWnd*)m_pParent)->m_opsenscal->GetIv();
		//mdat->theoryamt = ((CMapWnd*)m_pParent)->m_opsenscal->GetTheoryValue();
		mdat->delta = atof(((CMapWnd*)m_pParent)->m_opsenscal->GetDelta()) * 100;
		mdat->gamma = atof(((CMapWnd*)m_pParent)->m_opsenscal->GetGamma()) * 100;
		mdat->theta = atof(((CMapWnd*)m_pParent)->m_opsenscal->GetTheta());
		mdat->vegga = atof(((CMapWnd*)m_pParent)->m_opsenscal->GetVegga());
	}
}

void CMyStgPage::RemoveProject()
{
	for (int ii = m_ProjectCode.GetSize() -1; ii >= 0; ii--)
		deleteMData(m_ProjectCode.GetAt(ii).Code);
	m_ProjectCode.RemoveAll();
}



void CMyStgPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcGrp, rcClient;
	GetClientRect(&rcClient);
	m_pGrpWnd->GetWindowRect(rcGrp); ScreenToClient(rcGrp);
	dc.ExcludeClipRect(&rcGrp);
	dc.FillSolidRect(rcClient, m_bkColor);
}

HBRUSH CMyStgPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (pWnd->GetDlgCtrlID() == IDC_GBX_LIST  || 
		pWnd->GetDlgCtrlID() == IDC_MY_XSTATIC ||
		pWnd->GetDlgCtrlID() == IDC_MGCHK1 ||
		pWnd->GetDlgCtrlID() == IDC_MGCHK2 ||
		pWnd->GetDlgCtrlID() == IDC_MGCHK3 ||
		pWnd->GetDlgCtrlID() == IDC_MGCHK4 ||
		pWnd->GetDlgCtrlID() == IDC_MGCHK5 )
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

void CMyStgPage::OnSize(UINT nType, int cx, int cy) 
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
