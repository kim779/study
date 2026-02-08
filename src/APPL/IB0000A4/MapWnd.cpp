// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB0000A4.h"
#include "MapWnd.h"
#include "../../control/fx_misc/misctype.h"
#include "../../dll/axiscm/AxCommon.h"
#include "../../h/group.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#define GAP_TOP			3
#define GAP_SIDE		1
#define GAP_BOTTOM		1
#define GAP_BLOCK		5
#define GAP_PANEL		4
#define PANAL_HEIGHT	26
//판넬
#define clPanel	66
#define clPanelLine 165

//contents 외곽
#define clContents 181
#define clGuide	26
#define	clBack	64
#define clBox	65
#define	clText	97//69
#define GID_CLIENT "999"

const COLORREF clrGridHead		= 74;
const COLORREF clrGridLine		= 75;
const COLORREF clrGridBG		= 68;
const COLORREF clrGridFG		= 69;
const COLORREF clrGridFocusBG	= 78;
const COLORREF clrGrid1stRow	= clrGridBG;

const int MAX_SEND = 14*1024;			// 최대 send size(14K넘는경우)
#define SC_SAVEACC 103 
#define SC_ACCLISTBYGROUP 102 

#define IMAGEDIR	"image"
#define LIB_DELETE(XXX) { delete XXX; XXX = NULL; }
#define _SAFEDESTROY(window) if (window) {delete window; window = NULL; }

/////////////////////////////////////////////////////////////////////////////
// CMapWnd

CMapWnd::CMapWnd()
{
	m_panel = NULL;
	m_static1 = NULL;
	m_static2 = NULL;
	m_static3 = NULL;
	m_static4 = NULL;
	m_sendB	  = NULL;

	m_btnRDAccn = NULL;
	m_btnRDName = NULL;
	
}

CMapWnd::~CMapWnd()
{
	_SAFEDESTROY(m_static1);
	_SAFEDESTROY(m_static2);
	_SAFEDESTROY(m_static3);
	if (m_grid1)
	{
		m_grid1->DestroyWindow();
		m_grid1 = NULL;
	}	
	if (m_grid2)
	{
		m_grid2->DestroyWindow();
		m_grid2 = NULL;
	}
	
	_SAFEDESTROY(m_panel);
	
	
	_SAFEDESTROY(m_btnAdd);
	_SAFEDESTROY(m_btnInsert);
	_SAFEDESTROY(m_btnDelete);
	_SAFEDESTROY(m_btnTop);
	_SAFEDESTROY(m_btnUp);
	_SAFEDESTROY(m_btnDown);
	_SAFEDESTROY(m_btnBottom);
	_SAFEDESTROY(m_btnAllAdd);
	_SAFEDESTROY(m_btnAllDelete);
	_SAFEDESTROY(m_btnOK);
	_SAFEDESTROY(m_btnCancel);

	_SAFEDESTROY(m_static4);
	_SAFEDESTROY(m_btnRDAccn);
	_SAFEDESTROY(m_btnRDName);
}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTNADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTNINS, OnBtnInsert)
	ON_BN_CLICKED(IDC_BTNDEL, OnBtnDelete)
	ON_BN_CLICKED(IDC_BTNTOP, OnBtnTop)
	ON_BN_CLICKED(IDC_BTNUP , OnBtnUp)
	ON_BN_CLICKED(IDC_BTNDN , OnBtnDown)
	ON_BN_CLICKED(IDC_BTNBOT, OnBtnBottom)
	ON_BN_CLICKED(IDC_BTNALA, OnBtnAllAdd)
	ON_BN_CLICKED(IDC_BTNALD, OnBtnAllDelete)
	ON_BN_CLICKED(IDC_BTNOK, OnBtnOK)
	ON_BN_CLICKED(IDC_BTNCANC, OnBtnCancel)

	//}}AFX_MSG_MAP
	ON_NOTIFY(GVNM_LMOUSEDOWN, IDC_GRID1, OnGrid1Click)
	ON_NOTIFY(GVNM_DBLCLICKED, IDC_GRID1, OnGrid1DblClick)
	ON_NOTIFY(GVNM_DBLCLICKED, IDC_GRID2, OnGrid2DblClick)
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMapWnd message handlers

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(rc);
	dc.Rectangle(rc);
	// Do not call CWnd::OnPaint() for painting messages
}

void CMapWnd::init(CWnd* pParent)
{
	m_pWizard = pParent;
	m_font = getAxFont("굴림체", 9, false, 0);
	Variant(titleCC, "설정::조회계좌설정");
	m_sStaffNum = Variant(userCC, "");
	if (m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0))
		m_bCustomer = true;
	m_root = Variant(homeCC);
	m_user = Variant(nameCC);
	m_calBufL = -1;
	CRect rc;	GetClientRect(rc);
	CRect drc;	drc = rc;
	rc.bottom += 4;
	m_panel = new CfxStatic;
	m_panel->Create(_T(""), WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, rc, this, IDC_PANEL);
	m_panel->Init(ssGRADIENT, 0, _T(""), _T(""), GetIndexColor(clPanel), GetIndexColor(clPanel), GetIndexColor(clPanel), 0);
		
	HBITMAP BitMap[4];
	CStringArray strImage;
	//panel
	strImage.Add("AXPANEL1_LT.bmp");
	strImage.Add("AXPANEL1_RT.bmp");
	strImage.Add("AXPANEL1_LB.bmp");
	strImage.Add("AXPANEL1_RB.bmp");
	
	int nLineTick[4];
	nLineTick[0] = GAP_SIDE;
	nLineTick[1] = GAP_TOP;
	nLineTick[2] = GAP_SIDE;
	nLineTick[3] = GAP_BLOCK;
	
	for(int i = 0; i < strImage.GetSize(); i++)
	{	
		BitMap[i] = getBitmap2(strImage.GetAt(i));
	}
	
	m_panel->SetCornerRound(BitMap, 5, &nLineTick[0]);
	m_panel->SetBorderColor(GetIndexColor(clBack));
	
	m_static1 = new CfxStatic();
	drc.top = 6; drc.left = 20; drc.right = 200; drc.bottom = 24;
	m_static1->Create("보유계좌",WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,drc,m_panel,IDC_STATIC1);
	m_static1->Init(ssCOLOR, 0, _T(""), _T(""), GetIndexColor(clPanel), GetIndexColor(clPanel), GetIndexColor(clText), alLEFT);
	m_static1->SetFont(m_font);

	m_static2 = new CfxStatic();
	drc.OffsetRect(310,0);
	m_static2->Create("조회계좌",WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,drc,m_panel,IDC_STATIC2);
	m_static2->Init(ssCOLOR, 0, _T(""), _T(""), GetIndexColor(clPanel), GetIndexColor(clPanel), GetIndexColor(clText), alLEFT);
	m_static2->SetFont(m_font);

	m_static3 = new CfxStatic();
	drc.OffsetRect(-320,rc.Height()-30);
	drc.right = rc.right-160;
	m_static3->Create("※ 조회하고자 하는 계좌를 오른쪽으로 이동후 확인하여 주십시오.",WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,drc,m_panel,IDC_STATIC3);
	m_static3->Init(ssCOLOR, 0, _T(""), _T(""), GetIndexColor(clPanel), GetIndexColor(clPanel), GetIndexColor(clText), alLEFT);
	m_static3->SetFont(m_font);
	drc = rc;
	drc.left = 10;  drc.right = 240; drc.top = 25; drc.bottom -= 60;
	m_grid1 = new CfxGrid;
	m_grid1->Create(drc, this, IDC_GRID1, GVSC_BOTH, GVDD_NONE);
	
	initGrid(m_grid1,IDC_GRID1);

	drc.OffsetRect(310,0);
	m_grid2 = new CfxGrid;
	m_grid2->Create(drc, this, IDC_GRID2, GVSC_BOTH, GVDD_NONE);
	initGrid(m_grid2,IDC_GRID2);
	CString path; path.Format("%s\\image\\%s",m_root,"6BTN.BMP");
	//버튼 활성화
	drc.left -= 70; drc.right = drc.left+65; drc.top += 10; drc.bottom = drc.top+24;
	
	path.Format("%s\\%s\\%s\\%s.ini", m_root, "user", m_user, m_user);
	CProfile profile(path);
	m_useAlias = profile.GetInt("Account", "UseAlias");
	
	setImgBtn(m_btnAdd,5,true,drc, IDC_BTNADD, "추가 -->", IDB_BTNADD);
	drc.OffsetRect(0, 25);	setImgBtn(m_btnInsert,5,true,drc, IDC_BTNINS, "삽입 -->", IDB_BTNINS);
	drc.OffsetRect(0, 25);	setImgBtn(m_btnDelete,5,true,drc, IDC_BTNDEL, "<-- 삭제", IDB_BTNDEL);
	drc.OffsetRect(0, 25);
	drc.OffsetRect(0, 25);	setImgBtn(m_btnTop,5,true,drc, IDC_BTNTOP, "↑", IDB_BTNTOP);
	drc.OffsetRect(0, 25);	setImgBtn(m_btnUp,5,true,drc, IDC_BTNUP, "▲", IDB_BTNUP);
	drc.OffsetRect(0, 25);	setImgBtn(m_btnDown,5,true,drc, IDC_BTNDN, "▼", IDB_BTNDN);
	drc.OffsetRect(0, 25);	setImgBtn(m_btnBottom,5,true,drc, IDC_BTNBOT, "↓", IDB_BTNBOT);
	drc.OffsetRect(0, 25);
	drc.OffsetRect(0, 25);	setImgBtn(m_btnAllAdd,5,true,drc, IDC_BTNALA, "전체추가", IDB_BTNALA);
	drc.OffsetRect(0, 25);	setImgBtn(m_btnAllDelete,5,true,drc, IDC_BTNALD, "전체삭제", IDB_BTNALD);

	//2013.05.16 KSJ 정렬순서 라디오버튼 추가
	drc.left = 15; drc.right = drc.left+110; drc.top += 10; drc.bottom = drc.top+24;

	drc.OffsetRect(0, 25);
	m_static4 = new CfxStatic();
	m_static4->Create("계좌목록 정렬순서",WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,drc,m_panel,IDC_STATIC4);
	m_static4->Init(ssCOLOR, 0, _T(""), _T(""), GetIndexColor(clPanel), GetIndexColor(clPanel), GetIndexColor(clText), alLEFT);
	m_static4->SetFont(m_font);
	
	//2013.05.16 KSJ
	int nSort = 0;
	path.Format("%s\\%s\\%s\\%s", m_root, "user", m_user, "userconf.ini");
	
	profile.SetFileName(path);
	nSort = profile.GetInt("IB0000A4", "SORT");	//2013.05.16 KSJ 계좌명에 체크되었으면 1 아니면 0
	//2013.05.16 KSJ end

	drc.OffsetRect(120, 0);
	drc.right = drc.left + 70;
	m_btnRDAccn = new CButton();
	m_btnRDAccn->Create(_T("계좌번호"), WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON, drc, this, IDC_BTNRDACCN);
	m_btnRDAccn->SetFont(m_font, TRUE);
	m_btnRDAccn->SetCheck(!nSort);

	drc.OffsetRect(80, 0);
	m_btnRDName = new CButton();
	m_btnRDName->Create(_T("계좌명"), WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON, drc, this, IDC_BTNRDNAME);
	m_btnRDName->SetFont(m_font, TRUE);
	m_btnRDName->SetCheck(nSort);
	//2013.05.16 KSJ

	drc.top = rc.bottom-26; drc.bottom = drc.top+20; drc.left = rc.right-160; drc.right = drc.left+50;
	setImgBtn(m_btnOK,5,true,drc, IDC_BTNOK, "확인", IDB_BTNOK);
	drc.OffsetRect(80, 0);	setImgBtn(m_btnCancel,5,true,drc, IDC_BTNCANC, "취소", IDB_BTNCAN);
	
	/*
	m_btnOK = new CfxButton();
	m_btnOK->Create("", WS_VISIBLE, drc, this, IDC_BTNOK);
	
	m_btnOK->Init(bsIMAGE|bsTOGGLE, "\n확인", path);
	*/
	
	path.Format("%s\\image\\%s",m_root,"5BTN.BMP");
	//m_img.LoadBitmap(path);
	
	LoadAllAcnt();
	UpdateAllAcc();

	queryGroup();
}

COLORREF CMapWnd::GetIndexColor(int index)
{
	return m_pWizard->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

HBITMAP CMapWnd::getBitmap2(CString bmps)
{
	CString	path;
	path.Format("%s\\%s\\%s", m_root, IMAGEDIR, bmps);
	
	return ((CBitmap *)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(getBITMAP, 0), (LPARAM)(LPCTSTR)path))->operator HBITMAP();
}

CString CMapWnd::Variant(int cmd, CString data)
{
	CString retvalue;
	char* dta = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, cmd), (LPARAM)(LPCTSTR)data);
	
	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";
	
	return retvalue;
}

HBRUSH CMapWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	UINT nID = pWnd->GetDlgCtrlID();
    if (nID == IDC_STATIC1 || nID == IDC_STATIC2 || nID == IDC_STATIC3 || nID == IDC_STATIC4 || nID == IDC_BTNRDACCN || nID == IDC_BTNRDNAME)
	{
		pDC->SetBkColor(GetIndexColor(clPanel));
		return CreateSolidBrush(GetIndexColor(clPanel));
	}

	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

CFont* CMapWnd::getAxFont(CString fName, int point, bool bItalic, int nBold)
{
	struct _fontR fontR;
	fontR.name   = (LPSTR)fName.operator LPCTSTR();
	fontR.point  = point;
	fontR.italic = bItalic;
	fontR.bold   = nBold;
	return (CFont*)m_pWizard->SendMessage(WM_USER, getFONT, (long)&fontR);
}

void CMapWnd::initGrid(CfxGrid* &pGrid, UINT idc)
{
	_GVITEM gvitem;
	LOGFONT lf;
	COLORREF gridHead, gridLine;
	int ii = 0;
	CString	sectionS;
	int colCnt = 0;
	
	gridHead = GetIndexColor(clrGridHead);
	gridLine = GetIndexColor(clrGridLine);
	
	//pGrid->SetBkColor(GetIndexColor(clrGrid1stRow));
	
	pGrid->SetGridColor(gridLine);
	pGrid->SetGridFocusLine(TRUE, TRUE);
	
	if (m_font)	m_font->GetLogFont(&lf);
	lf.lfHeight = 9 * 10;
	
	gvitem.row = 0;
	gvitem.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_FGCOLOR|GVMK_BKCOLOR;
	gvitem.mask |= GVMK_ATTR|GVMK_MISC|GVMK_PARAM;
	
	gvitem.font = lf;
	gvitem.format = GVFM_HEADER;
	gvitem.fgcol = GetIndexColor(76);
	gvitem.bkcol = gridHead;
	
	switch (idc)
	{
	case IDC_GRID1:
	case IDC_GRID2:
		colCnt = sizeof(grInfoAcc)/sizeof(_grInfo);
		pGrid->Initial(1, colCnt, 1, 0);
		for (ii = 0; ii < colCnt; ii++)
		{
			gvitem.symbol = grInfoAcc[ii].symbols;
			gvitem.text = grInfoAcc[ii].heads;
			gvitem.attr = grInfoAcc[ii].attr;
			gvitem.col = ii;
			pGrid->SetItem(&gvitem);
			pGrid->SetColumnWidth(ii, grInfoAcc[ii].width);
		}
		//sectionS = GRACC;
		break;
	
	}
	
	pGrid->SetRowHeight(0, GRIDROW_HEIGHT);
	pGrid->SetRowColResize(FALSE, FALSE);
	pGrid->SetBKSelColor(GetIndexColor(clrGridFocusBG));
	//	pGrid->SetGridColor(GetIndexColor(65));
	//	pGrid->SetBkColor(GetIndexColor(CLRBG));
	pGrid->Adjust();
}

void CMapWnd::setImgBtn(CBitmapButton* &pButton, int nLength, BOOL bImageFit, CRect drc, UINT idc, CString title, UINT ibc)
{
	/*
	pButton = new CImgBtn();
	pButton->Create(title, WS_VISIBLE|WS_CHILD, drc, this, idc);
	pButton->SetFont(m_font, false);
	CString sImageDir;
	sImageDir.Format("%dbtn", nLength);
	*/
	pButton = new CBitmapButton();
	pButton->Create("123123", WS_VISIBLE|WS_CHILD|BS_OWNERDRAW, drc, this, idc);
	pButton->LoadBitmaps(ibc, IDB_BTNCLK);
	pButton->SizeToContent();
	
	
}
/*
void CMapWnd::setImgBtn(CImgBtn *pButton, int nLength, BOOL bImageFit, CRect drc, UINT idc, CString title)
{
	pButton = new CImgBtn();
	pButton->Create(title, WS_VISIBLE|WS_CHILD, drc, this, idc);
	pButton->SetFont(m_font, false);
	CString sImageDir;
	sImageDir.Format("%dbtn", nLength);
	
	pButton->m_hBitmap    = getBitmap2(sImageDir + ".bmp");
	pButton->m_hBitmap_dn = getBitmap2(sImageDir + "_dn.bmp");
	pButton->m_hBitmap_hv = getBitmap2(sImageDir + "_en.bmp");
	
	if (bImageFit)
		pButton->FitImageSize();
}
*/

void CMapWnd::OnBtnAdd()
{
	CIdCell idcell;
	idcell = m_grid1->GetFocusCell();
	if (idcell == NULL)
	{
		MessageBox("계좌를 선택하여주십시오.","IBK투자증권");
		return;
	}

		
	CString str_acnt = m_grid1->GetItemText(idcell.row, 0);
	CString str_acnm = m_grid1->GetItemText(idcell.row, 1);
	if (str_acnt.IsEmpty()) return;	
	str_acnt.Remove('-');
	AddAcnt(str_acnt, str_acnm);
	UpdateGpSave();

}

void CMapWnd::DoDataExchange(CDataExchange* pDX) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CWnd::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapWnd)
	//DDX_Control(pDX, IDC_BTNADD, m_btnAdd);
	//}}AFX_DATA_MAP
}

void CMapWnd::OnGrid1Click(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GVITEM *pgvitem = (NM_GVITEM*)pNMHDR;
	if (pResult)	*pResult = 0;
	//AfxMessageBox("Grid1Click");
}


void CMapWnd::OnGrid1DblClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GVITEM *pgvitem = (NM_GVITEM*)pNMHDR;
	CString str_acnt = m_grid1->GetItemText(pgvitem->row, 0);
	CString str_acnm = m_grid1->GetItemText(pgvitem->row, 1);
	
	str_acnt.Remove('-');
	AddAcnt(str_acnt, str_acnm);
	
}

void CMapWnd::LoadAllAcnt()
{
	CString str = Variant(accountCC, "1");
	CString buf, account, name, password, alias;
	int row = 1;
	deleteAllAcSave();
	m_mapPass.RemoveAll();
	CString tmp, mkgb;

	while (TRUE)
	{
		buf = parser(str, "\n");
		if (buf.IsEmpty())
			break;
		
		account = parser(buf);
		if (account.IsEmpty())
			break;
		
		password = parser(buf);
		tmp = parser(buf);
		name = parser(tmp, "|");
		alias = tmp;
		//DecodeAcctName(parser(buf), name, alias);
		/*
		switch (m_selType)
		{
		case SEL_ALL:
			break;
		case SEL_FU:
			if (!IsFOptionAcc(account))
				continue;
			break;
		default:
			if (!IsStockAcc(account))
				continue;
			break;
		}
		*/
		
		if (name.IsEmpty() || account.IsEmpty())
			continue;

		mkgb = "";
		if (account.GetLength()>=5)
			mkgb = account.Mid(3, 2);
		if (mkgb=="25")	// 해외선물 계좌
			continue;
		if (mkgb=="27") // FX마진 계좌
			continue;

		m_mapPass.SetAt(account, password);
		AddAcctSave(account, name, alias);
	}
}

void CMapWnd::AddAcctSave(const char *no, const char *nm, const char *al)
{
	CString name, alias;
	//DecodeAcctName(nm, name, alias);
	
	_acSave* p = new _acSave;
	p->sAccntName = nm;
	p->sAccntNum = no;
	p->sAccntNick = al;
	m_arAcSave.Add(p);
}

void CMapWnd::deleteAllAcSave()
{
	_acSave *acSave = NULL;
	for (int ii = 0; ii < m_arAcSave.GetSize(); ii++)
	{
		acSave = m_arAcSave.GetAt(ii);
		if (acSave)	LIB_DELETE(acSave);
	}
	m_arAcSave.RemoveAll();
}

void CMapWnd::UpdateAllAcc(CfxGrid* s_grid)
{
	CfxGrid* m_grid;
	if (s_grid == NULL)
		m_grid = m_grid1;
	else
		m_grid = s_grid;

	if (!m_grid)	return;
	
	m_grid->Clear();
	m_grid->SetRowCount(1);
	
	_acSave	*acSave = NULL;
	_GVITEM gvitem;
	LOGFONT lf;
	CString	accS;
	
	int nRow = m_grid->GetRowCount();
	
	if (m_font)
		m_font->GetLogFont(&lf);
	lf.lfHeight = 9*10;
	gvitem.font   = lf;
	gvitem.mask   = GVMK_FONT|GVMK_TEXT|GVMK_FORMAT|GVMK_PARAM|GVMK_ATTR|GVMK_BKCOLOR|GVMK_FGCOLOR;
	gvitem.bkcol  = GetIndexColor(clrGridBG);
	gvitem.fgcol  = GetIndexColor(clrGridFG);
	
	for (int ii = 0; ii < m_arAcSave.GetSize(); ii++)
	{
		acSave = m_arAcSave.GetAt(ii);
		
		nRow = m_grid->GetRowCount();
		m_grid->InsertRow(" ", nRow);
		m_grid->SetRowHeight(nRow, GRIDROW_HEIGHT);
		gvitem.row    = nRow;
		
		// 계좌번호(편집)
		gvitem.text = EncodeAcctNo(acSave->sAccntNum);
		gvitem.col = IACOL_ACC;
		gvitem.format = grInfoAcc[IACOL_ACC].format;
		gvitem.attr   = grInfoAcc[IACOL_ACC].attr;
		gvitem.attr &= ~GVAT_SORTSTR;
		m_grid->SetItem(&gvitem);
		
		// 계좌이름
		gvitem.text = (m_useAlias && !acSave->sAccntNick.IsEmpty()) ? acSave->sAccntNick: acSave->sAccntName;
		gvitem.col = IACOL_NAME;
		gvitem.format = grInfoAcc[IACOL_NAME].format;
		gvitem.attr   = grInfoAcc[IACOL_NAME].attr;
		gvitem.attr &= ~GVAT_SORTSTR;
		m_grid->SetItem(&gvitem);
		
		// Index
		gvitem.text.Format("%d", ii);
		gvitem.col = IACOL_IDX;
		gvitem.format = grInfoAcc[IACOL_IDX].format;
		gvitem.attr   = grInfoAcc[IACOL_IDX].attr;
		gvitem.attr &= ~GVAT_SORTSTR;
		m_grid->SetItem(&gvitem);
	}
	m_grid->Sort(0, true);
	if (m_grid==m_grid2)
		UpdateGpSave();
	
}

void CMapWnd::AddAcnt(CString acnt,CString acnm, int idx)
{
	if (!m_grid2)	return;
	
	//m_grid2->Clear();
	//m_grid2->SetRowCount(1);
	
	_acSave	*acSave = NULL;
	_GVITEM gvitem;
	LOGFONT lf;
	CString	accS;
	
	int nRow = m_grid2->GetRowCount();
	//if (idx != -1) nRow = idx;   //삽입인 경우
	//CString tmp; tmp.Format("%d[%d]", idx, nRow);
	//AfxMessageBox(tmp);

	for (int ii = 0; ii < nRow; ii++)
	{
		accS = m_grid2->GetItemText(ii,0);
		accS.Remove('-');
		if (accS == acnt)
		{
			MessageBox("동일한계좌가 존재합니다.", "IBK투자증권");
			return;	
		}	
	}

	if (m_font)
		m_font->GetLogFont(&lf);
	lf.lfHeight = 9*10;
	gvitem.font   = lf;
	gvitem.mask   = GVMK_FONT|GVMK_TEXT|GVMK_FORMAT|GVMK_PARAM|GVMK_ATTR|GVMK_BKCOLOR|GVMK_FGCOLOR;
	gvitem.bkcol  = GetIndexColor(clrGridBG);
	gvitem.fgcol  = GetIndexColor(clrGridFG);
	
	//for (int ii = 0; ii < m_arAcSave.GetSize(); ii++)
	{
		
		
		nRow = m_grid2->GetRowCount();
		if (idx != -1) nRow = idx;   //삽입인 경우
		m_grid2->InsertRow(" ", nRow);
		m_grid2->SetRowHeight(nRow, GRIDROW_HEIGHT);
		gvitem.row    = nRow;
		
		// 계좌번호(편집)
		gvitem.text = EncodeAcctNo(acnt);
		gvitem.col = IACOL_ACC;
		gvitem.format = grInfoAcc[IACOL_ACC].format;
		gvitem.attr   = grInfoAcc[IACOL_ACC].attr;
		gvitem.attr &= ~GVAT_SORTSTR;
		m_grid2->SetItem(&gvitem);
		
		// 계좌이름
		gvitem.text = acnm;
		gvitem.col = IACOL_NAME;
		gvitem.format = grInfoAcc[IACOL_NAME].format;
		gvitem.attr   = grInfoAcc[IACOL_NAME].attr;
		gvitem.attr &= ~GVAT_SORTSTR;
		m_grid2->SetItem(&gvitem);
		
		// Index
		gvitem.text.Format("%d", nRow);
		gvitem.col = IACOL_IDX;
		gvitem.format = grInfoAcc[IACOL_IDX].format;
		gvitem.attr   = grInfoAcc[IACOL_IDX].attr;
		gvitem.attr &= ~GVAT_SORTSTR;
		m_grid2->SetItem(&gvitem);
	}
	//m_grid1->Sort(0, true);
}

void CMapWnd::OnBtnInsert()
{
	CIdCell idcell;
	idcell = m_grid1->GetFocusCell();
	if (idcell == NULL)
	{
		MessageBox("계좌를 선택하여주십시오.","IBK투자증권");
		return;
	}
	
	
	CString str_acnt = m_grid1->GetItemText(idcell.row, 0);
	CString str_acnm = m_grid1->GetItemText(idcell.row, 1);
	if (str_acnt.IsEmpty()) return;
	str_acnt.Remove('-');
	CIdCell idcell2 = m_grid2->GetFocusCell();
	int ins_row = -1;
	if (idcell2 != NULL) ins_row = idcell2.row;
	AddAcnt(str_acnt, str_acnm, ins_row);
	UpdateGpSave();
}

void CMapWnd::OnBtnDelete()
{
	CIdCell idcell2 = m_grid2->GetFocusCell();
	int ins_row = -1;
	if (idcell2 != NULL) ins_row = idcell2.row;
	m_grid2->DeleteRow(ins_row);
	UpdateGpSave();
	
}

void CMapWnd::OnBtnTop()
{
	UpdateGrid2(IDC_BTNTOP);
}

void CMapWnd::OnBtnUp()
{
	UpdateGrid2(IDC_BTNUP);
}

void CMapWnd::OnBtnDown()
{
	UpdateGrid2(IDC_BTNDN);
}

void CMapWnd::OnBtnBottom()
{
	UpdateGrid2(IDC_BTNBOT);
}

void CMapWnd::OnBtnAllAdd()
{
	//
	
	UpdateAllAcc(m_grid2);
}

void CMapWnd::OnBtnAllDelete()
{
	//
	//m_grid2->Clear();	

	for(int ii=m_grid2->GetRowCount();ii>0 ; ii--) 
		m_grid2->DeleteRow(ii);

	UpdateGpSave();
}

void CMapWnd::OnGrid2DblClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GVITEM *pgvitem = (NM_GVITEM*)pNMHDR;

	OnBtnDelete();
	
}

void CMapWnd::UpdateGrid2(int tp)
{
	CIdCell idcell2 = m_grid2->GetFocusCell();
	int ins_row = -1;
	if (idcell2 == NULL) return; ins_row = idcell2.row;
	CString str_acnt = m_grid2->GetItemText(ins_row, 0);
	CString str_acnm = m_grid2->GetItemText(ins_row, 1);
	if (str_acnt.IsEmpty()) return;
	switch(tp)
	{
	case	IDC_BTNTOP:
		if (ins_row==1) return;
		m_grid2->DeleteRow(ins_row);
		ins_row = 1;
		break;
	case	IDC_BTNUP:
		if (ins_row==1) return;
		m_grid2->DeleteRow(ins_row);
		ins_row--;
		break;
	case	IDC_BTNDN:
		if (ins_row==(m_grid2->GetRowCount()-1)) return;
		m_grid2->DeleteRow(ins_row);
		ins_row++;
		break;
	case	IDC_BTNBOT:
		if (ins_row==(m_grid2->GetRowCount()-1)) return;
		m_grid2->DeleteRow(ins_row);
		ins_row = m_grid2->GetRowCount();		
		break;
		
	}
	
	
	str_acnt.Remove('-');
	AddAcnt(str_acnt, str_acnm, ins_row);
	m_grid2->SetSelectRow(ins_row);
	m_grid2->SetFocusCellEdit(ins_row,1, false);
	UpdateGpSave();
}

void CMapWnd::OnBtnOK()
{
	m_btnOK->EnableWindow(false);
	queryGroupSave(SC_SAVEACC, 3);	// 저장 OK되면 화면 닫음

	//2013.05.16 KSJ
	CString path;
	path.Format("%s\\%s\\%s\\%s", m_root, "user", m_user, "userconf.ini");

	CProfile profile(path);
	profile.Write("IB0000A4", "SORT", m_btnRDName->GetCheck());	//2013.05.16 KSJ 계좌명에 체크되었으면 1 아니면 0
	//2013.05.16 KSJ end
}

void CMapWnd::OnBtnCancel()
{
	CString m_rtnStr = "OnGroupChange\tok";			// 스크립트로 변경되었다고 알려줌
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)m_rtnStr);
	m_pWizard->PostMessage(WM_USER, closeDLL);
}


void CMapWnd::queryGroupSave(int sendKey, int selType)
{
	CString slog;
	UpdateGpSave();
	
	m_nextTRPos = -1;
	m_totTRCnt = -1;
	m_subTRCnt = -1;
	
	
	int ii = 0, jj = 0;
	int pos = L_gmid;

	_gpSave *gpSave = NULL;
	_acSave *acSave = NULL;
	
	int gpCnt = 0;
	CString tmpS;
	
	//_group	group;
	_group*	group;
	group = new _group;
	memset(group, ' ', L_group);

	//_gmid	gmid;
	_gmid* gmid = new _gmid;
	memset(gmid, ' ', L_gmid);

	//_accn	accn;
	_accn*	accn = new _accn;
	memset(accn, ' ', L_accn);

	if (m_sendB)
	{
		delete[] m_sendB;
		m_sendB = NULL;
	}
	
	
	m_calBufL = L_gmid + L_group * 0;
	//CString tmp; tmp.Format("m_calBufL : %d", m_calBufL);
	//AfxMessageBox(tmp);
	for (ii = 0; ii < m_arGpSave.GetSize(); ii++)
	{
		gpSave = m_arGpSave.GetAt(ii);

		if (!gpSave)	
			continue;
		if (gpSave->sGroupID != GID_CLIENT)
			continue;

		for (jj = 0; jj < gpSave->arAcEdit.GetSize(); jj++)
		{
			m_calBufL += L_accn;
		}

		m_calBufL += L_group;
		
	}
	//tmp.Format("m_calBufL: %d", m_calBufL);	
	//AfxMessageBox(tmp);
	m_sendB = new char[m_calBufL+1];
	
	memset(m_sendB, 0x00, m_calBufL+1);
	

	memset(gmid, 0x00, L_gmid);
	gmid->xflg = '2';				// 저장
	sprintf(gmid->usrid, "%-12s", m_sStaffNum);	// 사원번호
	gmid->sflag = '1';				// 모두
	//memset(&gmid, 0x00, L_gmid);
	//gmid.xflg  = '2';				// 저장
	//sprintf(gmid.usrid, "%-12s", m_sStaffNum);	// 사원번호
	//gmid.sflag = '1';				// 모두
	
	
	bool bAccPri = false;
	int seqn = 0;
	int arAcEditCnt = 0;
	CString passS;
	CString fstr;
	
	for (ii = 0, seqn = 0; ii < m_arGpSave.GetSize(); ii++)
	{
		gpSave = m_arGpSave.GetAt(ii);
		
		//memset(&group, 0x00 , L_group);
		memset(group, 0x00, L_group);
		if (gpSave->sGroupID.IsEmpty())
			group->xflg = 'I';	// Insert
		//	group.xflg = 'I';	// Insert
		else
			group->xflg = 'U';	// update
		//	group.xflg = 'U';	// update
		
		arAcEditCnt = gpSave->arAcEdit.GetSize();
		
		if (arAcEditCnt == 0)
		{
			//group.xflg = 'D';	// delete : 등록계좌 없으면 delete, 새로 등록한 그룹인 경우는 서버에 저장안함
			group->xflg = 'D';	// delete : 등록계좌 없으면 delete, 새로 등록한 그룹인 경우는 서버에 저장안함
			if (gpSave->sGroupID.IsEmpty())
				continue;
		}

		SetString(group->gpid, sizeof(group->gpid), gpSave->sGroupID);
		FormatCopy(group->seqn, "%03d", ++seqn);
		SetString(group->gnam, sizeof(group->gnam), gpSave->sGrName);
		FormatCopy(group->nrec, "%04d", arAcEditCnt);
		memcpy(&m_sendB[pos], group, L_group);
	/*	SetString(group.gpid, sizeof(group.gpid), gpSave->sGroupID);
		FormatCopy(&group.seqn, "%03d", ++seqn);
		SetString(group.gnam, sizeof(group.gnam), gpSave->sGrName);
		FormatCopy(&group.nrec, "%04d", arAcEditCnt);
		//memcpy(&m_sendB[pos], &group, L_group);*/

		pos += L_group;

		gpCnt++;

		// 대표계좌등록
		bAccPri = false;
		for (jj = 0; jj < arAcEditCnt; jj++)
		{
			acSave = gpSave->arAcEdit.GetAt(jj);
			if (acSave->sPriAcc == "1")
			{
				bAccPri = true;
				break;
			}
		}
		if (arAcEditCnt > 0 && !bAccPri)
			acSave->sPriAcc = "1";

//		slog.Format("[IB0000A4] arAcEditCnt = [%d]\n", arAcEditCnt);
//		OutputDebugString(slog);
		// 계좌저장
		for (jj = 0; jj < arAcEditCnt; jj++)
		{
			acSave = gpSave->arAcEdit.GetAt(jj);
			/*memset(&accn, 0x00, L_accn);
			sprintf(accn.seqn, "%03d", jj + 1);
			SetString(accn.accn, sizeof(accn.accn), acSave->sAccntNum);*/
			memset(accn, 0x00, L_accn);
			sprintf(accn->seqn, "%03d", jj + 1);
			SetString(accn->accn, sizeof(accn->accn), acSave->sAccntNum);

//slog.Format("--- [IB0000A4] [%s][%s]n", accn.accn, acSave->sAccntNum);
//OutputDebugString(slog);

			if (!m_bCustomer)
			{
				passS = getMapPass(acSave->sAccntNum);
				if (!passS.IsEmpty())
					SetString(accn->pass, sizeof(accn->pass), passS);
				else
					SetString(accn->pass, sizeof(accn->pass), "          ");
			}
			else
				SetString(accn->pass, sizeof(accn->pass), "          ");

			SetString(accn->acnm, sizeof(accn->acnm), acSave->sAccntName);
			if (acSave->sAllocRate.IsEmpty()) fstr = "0";
			else	fstr = Format("%.f", atof(acSave->sAllocRate));
			SetString(accn->rate, sizeof(accn->rate), fstr);
			SetString(accn->multi, sizeof(accn->multi), acSave->sAllocMulti);
			SetString(accn->alis, sizeof(accn->alis), acSave->sAccntNick);
			if (!acSave->sPriAcc.IsEmpty())
				accn->prea = acSave->sPriAcc.GetAt(0);
			else
				accn->prea = ' ';
			/*if (!m_bCustomer)
			{
				passS = getMapPass(acSave->sAccntNum);
				if (!passS.IsEmpty())
					SetString(accn.pass, sizeof(accn.pass), passS);
				else
					SetString(accn.pass, sizeof(accn.pass), "          ");
			}
			else
				SetString(accn.pass, sizeof(accn.pass), "          ");

			SetString(accn.acnm, sizeof(accn.acnm), acSave->sAccntName);
			if (acSave->sAllocRate.IsEmpty()) fstr = "0";
			else	fstr = Format("%.f", atof(acSave->sAllocRate));
			SetString(accn.rate, sizeof(accn.rate), fstr);
			SetString(accn.multi, sizeof(accn.multi), acSave->sAllocMulti);
			SetString(accn.alis, sizeof(accn.alis), acSave->sAccntNick);
			if (!acSave->sPriAcc.IsEmpty())
				accn.prea = acSave->sPriAcc.GetAt(0);
			else
				accn.prea = ' ';*/

			//memcpy(&m_sendB[pos], &accn, L_accn);
			memcpy(&m_sendB[pos], accn, L_accn);
			pos += L_accn;
		}
	}
	
	//sprintf(gmid.grec, "%04d", gpCnt);		// 총 그룹개수
	sprintf(gmid->grec, "%04d", gpCnt);		// 총 그룹개수
	
	memcpy(&m_sendB[0], gmid, L_gmid);
	//SetFocus();
	//TRACE("m_calBufL[%d] MAX_SEND[%d]\n", m_calBufL, MAX_SEND);
	//CString tmp; tmp.Format("%d [%d]", MAX_SEND, m_calBufL);
	//AfxMessageBox(tmp);
	
	//Sleep(1000); //무슨이유에선지... 텀을 줘야만..정상작동을 함..ㅡ,.ㅡ; 윈도우 7에서..
	if (m_calBufL > MAX_SEND-1)
	{
		m_nextTRPos = 0;
		m_subTRCnt = 1;
		m_totTRCnt = int(m_calBufL / (MAX_SEND + L_ihead)) + 1;
		//TRACE("연속 m_totTRCnt1[%d]\n", m_totTRCnt);
		int val = m_totTRCnt * (MAX_SEND - L_ihead);
		if (val < m_calBufL)
			m_totTRCnt++;
	//	m_totTRCnt = m_calBufL / (MAX_SEND + L_ihead) + 1;

		nextSend('F', sendKey);

	}
	else
	{
		//CString tmp; tmp.Format("pos: %d [%s]", pos, m_sendB);
		//AfxMessageBox(tmp);
		sendTR(TR_GROUP, m_sendB, pos, sendKey);

	}
}

void CMapWnd::SetString(char *pChar, int nMinLen, CString sStr)
{
	if (nMinLen > sStr.GetLength())
		CopyMemory(pChar, sStr.operator LPCTSTR(), min(nMinLen, sStr.GetLength()));
	else	
		CopyMemory(pChar, sStr.operator LPCTSTR(), min(nMinLen, sStr.GetLength()+1));
}

CString CMapWnd::getMapPass(CString acc)
{
	CString pass = _T("");
	m_mapPass.Lookup(acc, pass);
	return pass;
}


void CMapWnd::sendTR(CString trC, char *pBytes, int nBytes, char key)
{
	if (!m_pWizard)	
		return;
	
	int	len = L_userTH + nBytes;
	char*	sndB = new char[len+1];
	struct _userTH* udat = (struct _userTH*) sndB;
	
	ZeroMemory(sndB, sizeof(sndB));
	
	memcpy(udat->trc, trC, trC.GetLength());
	
	udat->key = key;	
	udat->stat = US_OOP;
	//udat->stat = US_ENC;  //test

	CopyMemory(&sndB[L_userTH], pBytes, nBytes);

	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, nBytes), (LPARAM)sndB);
	delete []sndB;
}

void CMapWnd::nextSend(char next, int sendKey)
{
	if (m_sendB2)
	{
		delete[] m_sendB2;	m_sendB2 = NULL;
	}
	
	int sendL = MAX_SEND-L_ihead;
	if (m_subTRCnt == m_totTRCnt)
	{
		sendL = m_calBufL - m_nextTRPos;
		next = 'E';
	}
	
	CString tmpS;
	m_sendB2 = new char[MAX_SEND+1];
	memset(m_sendB2, 0x00, MAX_SEND+1);
	
	_ihead*	ihead = (struct _ihead *)m_sendB2;
	ihead->xflg[0] = next;
	
	FormatCopy(&ihead->csiz, "%06d", sendL);
	FormatCopy(&ihead->fsiz, "%06d", m_calBufL);
	
	memcpy(&m_sendB2[L_ihead], &m_sendB[m_nextTRPos], sendL);
	m_nextTRPos += sendL;
	m_subTRCnt++;
	
	sendTR(TR_GROUP, m_sendB2, sendL+L_ihead, sendKey);
}

void CMapWnd::UpdateGpSave()
{
	deleteAllGpSave();
	_gpSave* m_gpSave = new _gpSave();
	m_gpSave->sAccntCnt.Format("%04d",m_grid2->GetRowCount());
	m_gpSave->sGroupID  = GID_CLIENT;
	m_gpSave->sGrName	= GID_CLIENT;
	CString acnt;
	for(int ii=1; ii<m_grid2->GetRowCount();ii++)
	{
		_acSave* m_acSave = new _acSave();
		acnt = m_grid2->GetItemText(ii, 0);
		acnt.Remove('-');
		m_acSave->sAccntNum  = acnt;
		m_acSave->sAccntName = m_grid2->GetItemText(ii, 1);
		m_acSave->sAccntNick = m_grid2->GetItemText(ii, 1);
		m_acSave->sPassword	 = getMapPass(acnt);
		m_gpSave->arAcEdit.Add(m_acSave);
	}
	if (m_grid2->GetRowCount()!=0)
		m_arGpSave.Add(m_gpSave);
	
}

void CMapWnd::deleteAllGpSave()
{
	for (int ii = 0; ii < m_arGpSave.GetSize(); ii++)
	{
		_gpSave *gpSave = m_arGpSave.GetAt(ii);
		if (gpSave)	LIB_DELETE(gpSave);
		
	}
	m_arGpSave.RemoveAll();
}

LRESULT CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	int len = 0;
	switch(LOBYTE(LOWORD(wParam))) 
	{
	case DLL_OUB:
		len = HIWORD(wParam);
		if (HIWORD(wParam))
		{
			switch(HIBYTE(LOWORD(wParam)))
			{
			case SC_ACCLISTBYGROUP:	// 그룹계좌조회
				oubGroup((char *)lParam);
				break;
			case SC_SAVEACC:
				{
					
					CString m_rtnStr = "OnGroupChange\tok";			// 스크립트로 변경되었다고 알려줌
					m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)m_rtnStr);
					m_pWizard->PostMessage(WM_USER, closeDLL);
					
					
				}	
				break;
			}
		}
		break;
	case DLL_GUIDE:
		return true;
		break;
	}
	return false;
}

void CMapWnd::queryGroup()
{
	deleteAllGpSave();
	m_mapPass.RemoveAll();
	
	//_gmid	gmid;
	//gmid.xflg  = '1';				// 조회
	//sprintf(gmid.usrid, "%-12s", m_sStaffNum);
	//sprintf(gmid.grec, "0000");

	_gmid*	gmid = new _gmid;
	memset(gmid, ' ', L_gmid);
	gmid->xflg = '1';				// 조회
	sprintf(gmid->usrid, "%-12s", m_sStaffNum);
	sprintf(gmid->grec, "0000");

	/*
	switch (m_selType)
	{
	case SEL_ALL:
		gmid.sflag = '3';				// 전체 조회
		break;
	case SEL_FU:
		gmid.sflag = '2';				// 선물옵션 조회
		break;
	default:
		gmid.sflag = '1';				// 현물 조회
		break;
	}
	*/
	//gmid.sflag = '1';
	gmid->sflag = '1';				// 현물 조회
	
	//CString sData = CString((char*)&gmid, L_gmid);
	CString sData = CString((char*)gmid, L_gmid);
	//sendTR(TR_GROUP, (char*)&gmid, L_gmid, SC_ACCLISTBYGROUP);
	sendTR(TR_GROUP, (char*)gmid, L_gmid, SC_ACCLISTBYGROUP);
	//sendTR(TR_GROUP, m_sendB, pos, sendKey);
}

void CMapWnd::oubGroup(char *buf)
{
	m_mapPass.RemoveAll();
	
	_gmod *gmod = (_gmod *)buf;
	if (gmod->okgb != 'Y')
		return;
	int grec = atoi(CString(gmod->grec, sizeof(gmod->grec)));
	if (grec == 0)
	{
		return;
	}
	//AfxMessageBox(CString(gmod->grec, sizeof(gmod->grec)));
	CString tmpS;
	int accn = 0;
	buf += L_gmod;
	for (int ii = 0; ii < grec; ii++)
	{
		_group *group = (_group *)buf;
		
		_gpSave *gpSave = new _gpSave();
		gpSave->sGroupID = GetString(group->gpid, sizeof(group->gpid));
		gpSave->sGrSortNum = GetString(group->seqn, sizeof(group->seqn));
		gpSave->sGrName = GetString(group->gnam, sizeof(group->gnam));
		gpSave->sAccntCnt = GetString(group->nrec, sizeof(group->nrec));
		accn = atoi(gpSave->sAccntCnt);
		
			
		buf += L_group;
		for (int jj = 0; jj < accn; jj++)
		{
			_accn *accn = (_accn *)buf;
			
			_acSave *acSave = new _acSave;
			acSave->sSortNum = GetString(accn->seqn, sizeof(accn->seqn));
			acSave->sAccntNum = GetString(accn->accn, sizeof(accn->accn));
			acSave->sAccntName = GetAcctNameFromAllAccount(acSave->sAccntNum, GetString(accn->acnm, sizeof(accn->acnm)));//(GetString(accn->acnm, sizeof(accn->acnm)));
			tmpS = GetString(accn->rate, sizeof(accn->rate));
			acSave->sAllocRate.Format("%.f", atof(tmpS));
			acSave->sAccntNick = GetString(accn->alis, sizeof(accn->alis));
			
			if (m_bCustomer)
			{
				acSave->sPassword = Variant(passCC, acSave->sAccntNum);
			}
			else
			{
				acSave->sPassword = GetString(accn->pass, sizeof(accn->pass));
				setMapPass(acSave->sAccntNum, acSave->sPassword);
			}
			acSave->sPassword.TrimRight();
			acSave->sPriAcc = accn->prea;
			acSave->sAllocMulti =  GetString(accn->multi, sizeof(accn->multi));
			gpSave->arAcEdit.Add(acSave);
			buf += L_accn;
			if (gpSave->sGroupID == GID_CLIENT)
			{
				AddAcnt(acSave->sAccntNum, acSave->sAccntName);
			}
		}
		m_arGpSave.Add(gpSave);
		
	}	
}

CString CMapWnd::GetString(char *pChar, int nMinLen)
{
	CString sRtn = CString(pChar, min(nMinLen, (int)strlen(pChar)));
	sRtn.TrimRight();
	return sRtn;
}

void CMapWnd::setMapPass(CString acc, CString pass)
{
	pass.TrimRight();
	acc.Remove('-');
	m_mapPass.SetAt(acc, pass);
}

CString CMapWnd::GetAcctNameFromAllAccount(const char *acct, const char *name)
{
	CString str(acct);
	str.Replace("-", "");
	
	for (int i = 0; i < m_arAcSave.GetSize(); i++)
	{
		if (m_arAcSave[i]->sAccntNum == str)
		{
			if (m_useAlias && !m_arAcSave[i]->sAccntNick.IsEmpty())
				return m_arAcSave[i]->sAccntNick;
			else
				return m_arAcSave[i]->sAccntName;
		}
	}
	return CString(name);
}

CString CMapWnd::parser(CString &srcstr, CString substr)
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
