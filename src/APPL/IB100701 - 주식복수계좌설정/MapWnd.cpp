// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB100701.h"
#include "MapWnd.h"
#include "../../control/fx_misc/misctype.h"
#include "../../control/fx_GridEx/UGCell.h"
#include "../../h/grid.h"
//#include "../../control/fx_grid/fxgrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapWnd

CMapWnd::CMapWnd(CWnd* pWizard)
{
	m_pWizard = pWizard;
}

CMapWnd::~CMapWnd()
{
/*
//	_SAFEDESTROY(m_pAccTitle);
//	_SAFEDESTROY(m_pGroupTitle);
//	_SAFEDESTROY(m_pSelTitle);
//	_SAFEDESTROY(m_pAccGrid);
//	_SAFEDESTROY(m_pGroupGrid);
//	_SAFEDESTROY(m_pSelGrid);
	_SAFEDESTROY(m_pLtoR);
//	_SAFEDESTROY(m_pIDStatic);
	_SAFEDESTROY(m_pID);
//	_SAFEDESTROY(m_pNameStatic);
	_SAFEDESTROY(m_pName);
	_SAFEDESTROY(m_pAddModify);
	_SAFEDESTROY(m_pGroupDelete);
//	_SAFEDESTROY(m_pGroupUp);
//	_SAFEDESTROY(m_pGroupDown);
//	_SAFEDESTROY(m_pAccStatic);
//	_SAFEDESTROY(m_pAccount);
//	_SAFEDESTROY(m_pRateStatic);
	_SAFEDESTROY(m_pRate);
//	_SAFEDESTROY(m_pRateSpin);
	_SAFEDESTROY(m_pSelModify);
	_SAFEDESTROY(m_pSelDelete);
//	_SAFEDESTROY(m_pSelUp);
//	_SAFEDESTROY(m_pSelDown);
	_SAFEDESTROY(m_pDefault);
	_SAFEDESTROY(m_pOk);
	_SAFEDESTROY(m_pCancel);
	_SAFEDESTROY(m_pApply);
*/
	if (m_pLinePen)
		m_pLinePen->DeleteObject();
	

}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_LTOR, OnLtoR)
	ON_BN_CLICKED(IDC_ADDMODIFY, OnAddModify)
	ON_BN_CLICKED(IDC_GROUPDELETE, OnGroupDelete)
	ON_BN_CLICKED(IDC_SELMODIFY, OnSelModify)
	ON_BN_CLICKED(IDC_SELDELETE, OnSelDelete)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_MESSAGE(WM_USER, OnMessage)


END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMapWnd message handlers
void CMapWnd::LoadRoundBitmap()
{
	CString sFile;
	sFile.Format("%s\\tab\\PALETTE.INI", m_strRoot);

	CString sPalette = _T("");
	int readL = 0; 
	char readB[1024]; 
	readL = GetPrivateProfileString(_T("General"), _T("Palette"), _T("Blue"), readB, sizeof(readB), sFile);
	if (readL > 0)	sPalette.Format("%s", readB);

	CString sImage;
	sImage.Format("%s\\Image\\axpanel%s1_",  m_strRoot, sPalette);
	for(int i=0; i<4; i++)
	{
		if(m_hRoundBitmap[i])
			DeleteObject(m_hRoundBitmap[i]);
	}

	CIB100701App* pApp = (CIB100701App*)AfxGetApp(); 
	m_hRoundBitmap[0] = (HBITMAP)::LoadImage( pApp->m_hInstance , sImage+"lt.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );
	m_hRoundBitmap[1] = (HBITMAP)::LoadImage( pApp->m_hInstance, sImage+"rt.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );
	m_hRoundBitmap[2] = (HBITMAP)::LoadImage( pApp->m_hInstance, sImage+"lb.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );
	m_hRoundBitmap[3] = (HBITMAP)::LoadImage(pApp->m_hInstance, sImage+"rb.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );

}
int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	Variant(titleCC, "[1007] 주식복수계좌설정");
	m_strRoot = Variant(homeCC);
	m_pNormalFont = GetAxFont("굴림체", 9, false, 0);
	m_pBoldFont = GetAxFont("굴림체", 9, false, 1);

	CRect rcClient;
	CRect rcCtrl;
	GetClientRect(&rcClient);

	LoadRoundBitmap();
	int nCenterLeft = 0;

	rcCtrl.SetRect(SZ_FORM_MARGIN, SZ_FORM_MARGIN, SZ_FORM_MARGIN + SZ_CTRL_ACCWIDTH, SZ_FORM_MARGIN + SZ_CTRL_HEIGHT);

	m_pAccTitle = std::make_unique<CfxStatic>();
	m_pAccTitle->Create(_T("계좌리스트"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rcCtrl, this, IDC_ACCTITLE);

	rcCtrl.top = rcCtrl.bottom + SZ_CTRL_VGAP;
	rcCtrl.bottom = rcClient.Height() - SZ_FORM_MARGIN - SZ_CTRL_HEIGHT - SZ_CTRL_VGAP - 10;

	m_pAccGrid = std::make_unique<CGridEx>(this);
	m_pAccGrid->CreateGrid(WS_CHILD | WS_VISIBLE, rcCtrl, this, IDC_ACCGRID);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_1BUTTON;
	rcCtrl.top = 220;
	rcCtrl.bottom = rcCtrl.top + SZ_CTRL_HEIGHT;

	m_pLtoR = std::make_unique <CfxImgButton>();
	m_pLtoR->Create("\n적용", rcCtrl, this, IDC_LTOR);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_RIGHTWIDTH;
	rcCtrl.top = SZ_FORM_MARGIN;
	rcCtrl.bottom = rcCtrl.top + SZ_CTRL_HEIGHT;

	nCenterLeft = rcCtrl.left;

	m_pGroupTitle = std::make_unique<CfxStatic>();
	m_pGroupTitle->Create(_T("그룹리스트"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rcCtrl, this, IDC_GROUPTITLE);

	rcCtrl.top = rcCtrl.bottom + SZ_CTRL_VGAP;
	rcCtrl.bottom = rcCtrl.top + SZ_CTRL_HEIGHT;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_3STATIC;

	m_pIDStatic = std::make_unique<CfxStatic>();
	m_pIDStatic->Create(_T("그룹ID"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
					rcCtrl, this, IDC_IDSTATIC);

	rcCtrl.left = rcCtrl.right + 3;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2EDIT;
	
	m_pID = std::make_unique <CfxEdit>();
	m_pID->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, rcCtrl, this, IDC_ID);
	m_pID->SetPasswordChar(0);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2STATIC;

	m_pNameStatic = std::make_unique<CfxStatic>();
	m_pNameStatic->Create(_T("설명"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
					rcCtrl, this, IDC_NAMESTATIC);

	rcCtrl.left = rcCtrl.right + 3;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_5EDIT;

	m_pName = std::make_unique <CfxEdit>();
	m_pName->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, rcCtrl, this, IDC_NAME);
	m_pName->SetPasswordChar(0);

	rcCtrl.left = rcClient.right - SZ_FORM_MARGIN - SZ_CTRL_5BUTTON - SZ_CTRL_3BUTTON - 2;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_5BUTTON;

	m_pAddModify = std::make_unique < CfxImgButton>();
	m_pAddModify->Create(_T("\n추가/수정"), rcCtrl, this, IDC_ADDMODIFY);

	rcCtrl.left = rcCtrl.right + 2;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_3BUTTON;

	m_pGroupDelete = std::make_unique < CfxImgButton>();
	m_pGroupDelete->Create(_T("\n삭제Χ"), rcCtrl, this, IDC_GROUPDELETE);

	rcCtrl.top = rcCtrl.bottom + SZ_CTRL_VGAP;
	rcCtrl.bottom = rcCtrl.top + SZ_CTRL_GROUPHEIGHT;
	rcCtrl.left = nCenterLeft;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_RIGHTWIDTH;

	m_pGroupGrid = std::make_unique<CGridEx>(this);
	m_pGroupGrid->CreateGrid(WS_CHILD | WS_VISIBLE, rcCtrl, this, IDC_GROUPGRID);
/*
	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_5BUTTON;

	m_pGroupUp = new CfxImgButton;
	m_pGroupUp->Create(_T("\n위로↑"), rcCtrl, this, IDC_GROUPUP);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_5BUTTON;

	m_pGroupDown = new CfxImgButton;
	m_pGroupDown->Create(_T("\n아래로↓"), rcCtrl, this, IDC_GROUPDOWN);
*/
	rcCtrl.top = rcCtrl.bottom + SZ_CTRL_VGAP * 3;
	rcCtrl.bottom = rcCtrl.top + SZ_CTRL_HEIGHT;
	rcCtrl.left = nCenterLeft;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_RIGHTWIDTH;

	m_pSelTitle = std::make_unique<CfxStatic>();
	m_pSelTitle->Create(_T("선택그룹 계좌리스트"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rcCtrl, this, IDC_SELTITLE);

	rcCtrl.top = rcCtrl.bottom + SZ_CTRL_VGAP;
	rcCtrl.bottom = rcCtrl.top + SZ_CTRL_HEIGHT;
	rcCtrl.left = nCenterLeft;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2STATIC;

	m_pAccStatic = std::make_unique<CfxStatic>();
	m_pAccStatic->Create(_T("계좌"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
					rcCtrl, this, IDC_ACCSTATIC);

	rcCtrl.left = rcCtrl.right + 3;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_8STATIC;

	m_pAccount = std::make_unique<CfxStatic>();
	m_pAccount->Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
					rcCtrl, this, IDC_ACCOUNT);
	
	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2STATIC;
	
	m_pRateStatic = std::make_unique<CfxStatic>();
	m_pRateStatic->Create(_T("배수"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
					rcCtrl, this, IDC_RATESTATIC);

	rcCtrl.left = rcCtrl.right + 1;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4EDIT;

	m_pRate = std::make_unique <CfxEdit>();
	m_pRate->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | ES_RIGHT, rcCtrl, this, IDC_RATE);
	m_pRate->SetPasswordChar(0);

	rcCtrl.left = rcCtrl.right - 10;
	rcCtrl.right = rcCtrl.left + 20;
	
	m_pRateSpin = std::make_unique <CNumSpinCtrl>();
	m_pRateSpin->Create(WS_CHILD | WS_VISIBLE | UDS_ALIGNRIGHT | UDS_AUTOBUDDY | UDS_ARROWKEYS, rcCtrl, this, IDC_RATESPIN);

	rcCtrl.left = rcClient.right - SZ_FORM_MARGIN - SZ_CTRL_3BUTTON - SZ_CTRL_2BUTTON  - 2 ;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2BUTTON;

	m_pSelModify = std::make_unique < CfxImgButton>();
	m_pSelModify->Create(_T("\n수정"), rcCtrl, this, IDC_SELMODIFY);

	rcCtrl.left = rcCtrl.right + 2;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_3BUTTON;

	m_pSelDelete = std::make_unique <CfxImgButton>();
	m_pSelDelete->Create(_T("\n삭제Χ"), rcCtrl, this, IDC_SELDELETE);

	rcCtrl.top = rcCtrl.bottom + SZ_CTRL_VGAP;
	rcCtrl.bottom = rcCtrl.top + SZ_CTRL_SELHEIGHT;
	rcCtrl.left = nCenterLeft;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_RIGHTWIDTH;

	m_pSelGrid = std::make_unique<CGridEx>(this);
	m_pSelGrid->CreateGrid(WS_CHILD | WS_VISIBLE, rcCtrl, this, IDC_SELGRID);

	rcCtrl.top = rcCtrl.bottom + SZ_CTRL_VGAP;
	rcCtrl.bottom = rcCtrl.top + SZ_CTRL_HEIGHT;
	rcCtrl.left = rcCtrl.right - SZ_CTRL_2BUTTON;

	m_pOk = std::make_unique <CfxImgButton>();
	m_pOk->Create(_T("\n확인"), rcCtrl, this, IDOK);


	return 0;
}

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcClient, rcbox;
	GetClientRect(&rcClient);
	rcClient.bottom -= 26;
	dc.FillSolidRect(&rcClient, GetIndexColor(64));

	rcbox = rcClient;
	rcbox.DeflateRect(2,2,2,2);
	dc.FillSolidRect(&rcbox, GetIndexColor(CLR_FORM_BACK));

	DrawCornerRoundBitmap(&dc, rcbox, 0);

}

void CMapWnd::Init()
{
	m_pAccTitle->SetStyle(m_pAccTitle->GetStyle() | ssBORDER);
	m_pAccTitle->SetBkColor(GetIndexColor(CLR_GUIDE_BACK));
	m_pAccTitle->SetFgColor(GetIndexColor(CLR_GUIDE_FORE));
	m_pAccTitle->SetBorderColor(GetIndexColor(CLR_GUIDE_LINE));
	m_pAccTitle->SetFont(m_pNormalFont);
	m_pAccTitle->SetAlignment(alCENTER);

	m_pGroupTitle->SetStyle(m_pAccTitle->GetStyle() | ssBORDER);
	m_pGroupTitle->SetBkColor(GetIndexColor(CLR_GUIDE_BACK));
	m_pGroupTitle->SetFgColor(GetIndexColor(CLR_GUIDE_FORE));
	m_pGroupTitle->SetBorderColor(GetIndexColor(CLR_GUIDE_LINE));
	m_pGroupTitle->SetFont(m_pNormalFont);
	m_pGroupTitle->SetAlignment(alCENTER);

	m_pSelTitle->SetStyle(m_pAccTitle->GetStyle() | ssBORDER);
	m_pSelTitle->SetBkColor(GetIndexColor(CLR_GUIDE_BACK));
	m_pSelTitle->SetFgColor(GetIndexColor(CLR_GUIDE_FORE));
	m_pSelTitle->SetBorderColor(GetIndexColor(CLR_GUIDE_LINE));
	m_pSelTitle->SetFont(m_pNormalFont);
	m_pSelTitle->SetAlignment(alCENTER);

	CString strPath = "";
	strPath.Format("%s\\Image\\", m_strRoot);

	HBITMAP hbmp	= GetAxBitmap(strPath + "다음.bmp")->operator HBITMAP();
	HBITMAP hbmp_dn	= GetAxBitmap(strPath + "다음_dn.bmp")->operator HBITMAP();
	HBITMAP hbmp_hv = GetAxBitmap(strPath + "다음_en.bmp")->operator HBITMAP();

	m_pLtoR->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pLtoR->SetFont(m_pNormalFont);

	m_pIDStatic->SetBkColor(GetIndexColor(CLR_FORM_BACK));
	m_pIDStatic->SetFgColor(GetIndexColor(CLR_STATIC_FORE));
	m_pIDStatic->SetFont(m_pNormalFont);

	m_pID->SetFont(m_pNormalFont);
	m_pID->ModifyAttribute(0, attrNUMERIC);
	m_pID->LimitText(3);

	m_pNameStatic->SetBkColor(GetIndexColor(CLR_FORM_BACK));
	m_pNameStatic->SetFgColor(GetIndexColor(CLR_STATIC_FORE));
	m_pNameStatic->SetFont(m_pNormalFont);

	m_pName->SetFont(m_pNormalFont);

	m_pAccStatic->SetBkColor(GetIndexColor(CLR_FORM_BACK));
	m_pAccStatic->SetFgColor(GetIndexColor(CLR_STATIC_FORE));
	m_pAccStatic->SetFont(m_pNormalFont);

	m_pAccount->SetBkColor(GetIndexColor(CLR_FORM_BACK));
	m_pAccount->SetBorderColor(GetIndexColor(CLR_GRID_LINE));
	m_pAccount->SetStyle(m_pAccount->GetStyle() | ssBORDER);
	m_pAccount->SetFont(m_pNormalFont);

	m_pRateStatic->SetBkColor(GetIndexColor(CLR_FORM_BACK));
	m_pRateStatic->SetFgColor(GetIndexColor(CLR_STATIC_FORE));
	m_pRateStatic->SetFont(m_pNormalFont);

	m_pRate->SetFont(m_pNormalFont);
	m_pRate->ModifyAttribute(0, attrNUMERIC);
	m_pRate->SetWindowText(_T("0.00"));
	
	m_pRateSpin->SetDecimalPlaces(2);
	m_pRateSpin->SetTrimTrailingZeros(FALSE);
	m_pRateSpin->SetRangeAndDelta(0.01, 1000, 0.01);
	m_pRateSpin->SetPos(0.01);

	hbmp	= GetAxBitmap(strPath + "5btn.bmp")->operator HBITMAP();
	hbmp_dn	= GetAxBitmap(strPath + "5btn_dn.bmp")->operator HBITMAP();
	hbmp_hv = GetAxBitmap(strPath + "5btn_en.bmp")->operator HBITMAP();

	m_pAddModify->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pAddModify->SetWindowText(_T("추가/수정"));
	m_pAddModify->SetFont(m_pNormalFont);

	hbmp	= GetAxBitmap(strPath + "3btn.bmp")->operator HBITMAP();
	hbmp_dn	= GetAxBitmap(strPath + "3btn_dn.bmp")->operator HBITMAP();
	hbmp_hv = GetAxBitmap(strPath + "3btn_en.bmp")->operator HBITMAP();

	m_pGroupDelete->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pGroupDelete->SetWindowText(_T("삭제Χ"));
	m_pGroupDelete->SetFont(m_pNormalFont);

	m_pSelDelete->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pSelDelete->SetWindowText(_T("삭제Χ"));
	m_pSelDelete->SetFont(m_pNormalFont);

//	m_pGroupUp->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
//	m_pGroupUp->SetWindowText(_T("위로↑"));
//	m_pGroupUp->SetFont(m_pNormalFont);

//	m_pGroupDown->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
//	m_pGroupDown->SetWindowText(_T("아래로↓"));
//	m_pGroupDown->SetFont(m_pNormalFont);

//	m_pSelUp->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
//	m_pSelUp->SetWindowText(_T("위로↑"));
//	m_pSelUp->SetFont(m_pNormalFont);

//	m_pSelDown->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
//	m_pSelDown->SetWindowText(_T("아래로↓"));
//	m_pSelDown->SetFont(m_pNormalFont);

	
	hbmp	= GetAxBitmap(strPath + "2btn.bmp")->operator HBITMAP();
	hbmp_dn	= GetAxBitmap(strPath + "2btn_dn.bmp")->operator HBITMAP();
	hbmp_hv = GetAxBitmap(strPath + "2btn_en.bmp")->operator HBITMAP();

	m_pSelModify->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pSelModify->SetWindowText(_T("수정"));
	m_pSelModify->SetFont(m_pNormalFont);

	m_pOk->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pOk->SetWindowText(_T("확인"));
	m_pOk->SetFont(m_pNormalFont);

	CUGCell cell;
	m_pLinePen = std::make_unique<CPen>(PS_SOLID, 1, GetIndexColor(CLR_GRID_LINE));

	m_pAccGrid->SetNumberCols(2);
	m_pAccGrid->SetNumberRows(0);

	m_pAccGrid->SetColWidth(0, SZ_COL_ACCOUNT);
	m_pAccGrid->SetColWidth(1, SZ_COL_ACCNAME);

	m_pAccGrid->GetCell(0, -1, &cell);
	cell.SetBackColor(GetIndexColor(CLR_GRID_HBACK));
	cell.SetTextColor(GetIndexColor(CLR_GRID_HTEXT));
	cell.SetText(_T("계좌번호"));
	cell.SetBorder(UG_BDR_LTHIN | UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	cell.SetBorderColor(m_pLinePen.get());
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	m_pAccGrid->SetCell(0, -1, &cell);

	m_pAccGrid->GetCell(1, -1, &cell);
	cell.SetBackColor(GetIndexColor(CLR_GRID_HBACK));
	cell.SetTextColor(GetIndexColor(CLR_GRID_HTEXT));
	cell.SetText(_T("표시계좌명"));
	cell.SetBorder(UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	cell.SetBorderColor(m_pLinePen.get());
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	m_pAccGrid->SetCell(1, -1, &cell);

	m_pAccGrid->GetColDefault(0, &cell);
	cell.SetHBackColor(GetIndexColor(CLR_GRID_SBACK));
	cell.SetHTextColor(GetIndexColor(CLR_GRID_STEXT));
	cell.SetBackColor(GetIndexColor(CLR_GRID_BACK));
	cell.SetTextColor(GetIndexColor(CLR_GRID_TEXT));
	cell.SetBorder(UG_BDR_LTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	cell.SetBorderColor(m_pLinePen.get());
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	m_pAccGrid->SetColDefault(0, &cell);

	cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
	m_pAccGrid->SetColDefault(1, &cell);

	m_pSelGrid->SetNumberCols(3);
	m_pSelGrid->SetNumberRows(0);

	m_pSelGrid->SetColWidth(0, SZ_COL_ACCOUNT);
	m_pSelGrid->SetColWidth(1, SZ_COL_ACCNAME);
	m_pSelGrid->SetColWidth(2, SZ_COL_RATE);

	m_pSelGrid->GetCell(0, -1, &cell);
	cell.SetBackColor(GetIndexColor(CLR_GRID_HBACK));
	cell.SetTextColor(GetIndexColor(CLR_GRID_HTEXT));
	cell.SetText(_T("계좌번호"));
	cell.SetBorder(UG_BDR_LTHIN | UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	cell.SetBorderColor(m_pLinePen.get());
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	m_pSelGrid->SetCell(0, -1, &cell);

	cell.SetText(_T("표시계좌명"));
	cell.SetBorder(UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	m_pSelGrid->SetCell(1, -1, &cell);

	cell.SetText(_T("배수"));
	cell.SetBorder(UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	m_pSelGrid->SetCell(2, -1, &cell);

	m_pSelGrid->GetColDefault(0, &cell);
	cell.SetHBackColor(GetIndexColor(CLR_GRID_SBACK));
	cell.SetHTextColor(GetIndexColor(CLR_GRID_STEXT));
	cell.SetBackColor(GetIndexColor(CLR_GRID_BACK));
	cell.SetTextColor(GetIndexColor(CLR_GRID_TEXT));
	cell.SetBorder(UG_BDR_LTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	cell.SetBorderColor(m_pLinePen.get());
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	m_pSelGrid->SetColDefault(0, &cell);

	cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
	m_pSelGrid->SetColDefault(1, &cell);

	cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
	m_pSelGrid->SetColDefault(2, &cell);


	m_pGroupGrid->SetNumberCols(4);
	m_pGroupGrid->SetNumberRows(0);

	m_pGroupGrid->SetColWidth(0, SZ_COL_GROUPID);
	m_pGroupGrid->SetColWidth(1, SZ_COL_GROUPNAME);
	m_pGroupGrid->SetColWidth(2, SZ_COL_TOTRATE);
	m_pGroupGrid->SetColWidth(3, 0);

	m_pGroupGrid->GetCell(0, -1, &cell);
	cell.SetBackColor(GetIndexColor(CLR_GRID_HBACK));
	cell.SetTextColor(GetIndexColor(CLR_GRID_HTEXT));
	cell.SetText(_T("그룹ID"));
	cell.SetBorder(UG_BDR_LTHIN | UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	cell.SetBorderColor(m_pLinePen.get());
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	m_pGroupGrid->SetCell(0, -1, &cell);

	cell.SetText(_T("그룹설명"));
	cell.SetBorder(UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	m_pGroupGrid->SetCell(1, -1, &cell);

	cell.SetText(_T("배수합"));
	cell.SetBorder(UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	m_pGroupGrid->SetCell(2, -1, &cell);

	cell.SetText(_T("계좌저장여부"));
	cell.SetBorder(UG_BDR_TTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	m_pGroupGrid->SetCell(3, -1, &cell);

	m_pGroupGrid->GetColDefault(0, &cell);
	cell.SetHBackColor(GetIndexColor(CLR_GRID_SBACK));
	cell.SetHTextColor(GetIndexColor(CLR_GRID_STEXT));
	cell.SetBackColor(GetIndexColor(CLR_GRID_BACK));
	cell.SetTextColor(GetIndexColor(CLR_GRID_TEXT));
	cell.SetBorder(UG_BDR_LTHIN | UG_BDR_RTHIN | UG_BDR_BTHIN);
	cell.SetBorderColor(m_pLinePen.get());
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	m_pGroupGrid->SetColDefault(0, &cell);

	cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
	m_pGroupGrid->SetColDefault(1, &cell);

	cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
	m_pGroupGrid->SetColDefault(2, &cell);
	
	cell.SetBorder(UG_BDR_RTHIN | UG_BDR_BTHIN);
	m_pGroupGrid->SetColDefault(3, &cell);
}

COLORREF CMapWnd::GetIndexColor(UINT nIndex)
{
	if (!m_pWizard)
		return 0;

	return m_pWizard->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)nIndex);
}


CString CMapWnd::Variant(int nComm, CString strData /* = _T("") */)
{
	CString strRet(_T(""));

	char* pData = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nComm), (LPARAM)(LPCTSTR)strData);

	if ((long)pData > 1)
		strRet = pData;

	return strRet;
}

CFont* CMapWnd::GetAxFont(CString strName, int nPt, bool bItalic, int nBold)
{
	struct _fontR fontR;
	fontR.name = (LPSTR)(LPCTSTR)strName;
	fontR.point = nPt;
	fontR.italic = bItalic;
	fontR.bold = nBold;

	return (CFont*)m_pWizard->SendMessage(WM_USER, getFONT, (long)&fontR);
}

void CMapWnd::SetAccountList()
{
	CString strData = Variant(accountCC);
	int nCurRow = 0;
	BOOL bUseAlias = FALSE;

	// 주문대리인 관련 로직 추가 - 2011.01.28 -------------------------------
	BOOL bStaff = FALSE, bTuja = FALSE;
	CString strUsid = Variant(userCC);
	
	if (strUsid.GetLength()==6 && atoi(strUsid)>0)
		bStaff = TRUE;

	LPCSTR szOTPF = (LPCSTR)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, popCC), (LPARAM)"LOGIN.Otpf");
	if (szOTPF)
		if (*szOTPF=='1')	// 투자일임업자
			bTuja = TRUE;
	// ----------------------------------------------------------------------

	CString strUserPath;
	strUserPath.Format("%s\\user\\%s\\%s.ini", m_strRoot, Variant(nameCC), Variant(nameCC));

	if (GetPrivateProfileInt(_T("Account"), _T("UseAlias"), 0, (LPCTSTR)strUserPath) == 1)
		bUseAlias = TRUE;

	while (!strData.IsEmpty())
	{
		CString strAccInfo(_T(""));
		CString strAccount;
		CString strAccNameInfo;
		CString strAccName;
		CString strAccNick;
		CString strAccOwnf;
		
		strAccInfo = Parser(strData, _T("\n"));

		strAccount = Parser(strAccInfo, _T("\t"));

		CString strMid = strAccount.Mid(3, 2);

		if (strMid != _T("10") && strMid != _T("11") && strMid != _T("30") && strMid != _T("31"))
			continue;

/*
		if (strAccount.Mid(3, 2) == _T("20") || strAccount.Mid(3, 2) == _T("21") ||
			strAccount.Mid(3, 2) == _T("53") || strAccount.Mid(3, 2) == _T("00"))
			continue;
*/
		strAccount.Insert(3, _T("-"));
		strAccount.Insert(6, _T("-"));
		Parser(strAccInfo, _T("\t"));

		strAccNameInfo = Parser(strAccInfo, _T("\t"));

		strAccName = Parser(strAccNameInfo, _T("|"));
		strAccNick = Parser(strAccNameInfo, _T("|"));
		strAccOwnf = Parser(strAccNameInfo, _T("|"));

		const int iAccOwnf = atoi(strAccOwnf);

		// Wrap 계좌는 무조건 안보여준다.
		if ((iAccOwnf&0x02) != 0) continue;

		if (!bStaff)
		{
			// 고객중 투자자문사가 아닌경우, 대리인계좌는 편집할 수 없다.
			if (!bTuja && ((iAccOwnf & 0x01) == 0) ) continue;
		}

// 		CString emsg;
// 		emsg.Format("acno(%s) acnm(%s) nick(%s) ownf(%s)\n", strAccount, strAccName, strAccNick, strAccOwnf);
// 		OutputDebugString(emsg);

		if (m_pAccGrid->GetNumberRows() == nCurRow)
			m_pAccGrid->InsertRow(nCurRow);

		m_pAccGrid->QuickSetText(0, nCurRow, strAccount);
		
		if (bUseAlias)
		{
			if (strAccNick.IsEmpty())
				strAccNick = strAccName;

			m_pAccGrid->QuickSetText(1, nCurRow, strAccNick);
		}
		else
			m_pAccGrid->QuickSetText(1, nCurRow, strAccName);

		m_pAccGrid->QuickSetText(1, nCurRow, bUseAlias ? strAccNick : strAccName);

		nCurRow++;
	}
}

CString CMapWnd::Parser(CString &srcstr, CString substr)
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

void CMapWnd::OnLtoR()
{
	const int nGroupRow = m_pGroupGrid->GetCurrentRow();
	
	if (nGroupRow < 0)
		return;

	CString strGroupID = m_pGroupGrid->QuickGetText(0, nGroupRow);

	_TRIMDATA(strGroupID);

	if (strGroupID.IsEmpty())
		return;

	const int nCurRow = m_pAccGrid->GetCurrentRow();
	const int nTargetRow = m_pSelGrid->GetNumberRows();

	CString strAccount = m_pAccGrid->QuickGetText(0, nCurRow);
	CString strRawAcnt = strAccount;
	strRawAcnt.Remove('-');
	CString strAccName = m_pAccGrid->QuickGetText(1, nCurRow);
	CString strPassword = Variant(passCC, strRawAcnt);

	_TRIMDATA(strAccount);
	_TRIMDATA(strAccName);
	_TRIMDATA(strPassword);

	if (strAccount.IsEmpty())
		return;

	if (strPassword.IsEmpty())
	{
		MessageBox("계좌비밀번호 저장 후 사용하십시오.");
		return;
	}

	for (int i = 0; i < nTargetRow; i++)
	{
		if (strAccount == m_pSelGrid->QuickGetText(0, i))
			return;
	}

	m_pSelGrid->InsertRow(m_pSelGrid->GetNumberRows());
	m_pSelGrid->QuickSetText(0, nTargetRow, strAccount);
	m_pSelGrid->QuickSetText(1, nTargetRow, strAccName);
	m_pSelGrid->QuickSetText(2, nTargetRow, _T("1.00"));
	m_pSelGrid->RedrawRow(nTargetRow);
	m_pSelGrid->GotoRow(nTargetRow);

	QueryGroupAMD(2);
}

long CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INIT:
		QueryGroupList();
		SetAccountList();
		break;

	case DLL_OUB:
		{
			const int key = HIBYTE(LOWORD(wParam));
			const int len = HIWORD(wParam);
			struct _ledgerH ledger;
			CString strLedger(_T(""));
			CString strErrCode(_T(""));
			CString strErrText(_T(""));
			CString strMsg(_T(""));
			int nNextKey = 0;

			CopyMemory(&ledger, (void*)lParam, L_ledgerH);
			strLedger = CString((char*)&ledger, L_ledgerH);
			strErrCode = CString((char*)&ledger.emsg, 4);
			strErrText = CString((char*)&ledger.emsg, 98);
			nNextKey = ledger.next[0];
			
			lParam += L_ledgerH;
			strMsg = "ERR\t" + strErrText;
			if (strErrCode.GetAt(0) != _T('0'))	// 오류
			{
				strMsg.Format("[%s]%s", strErrCode, strErrText.Right(strErrText.GetLength() - 4));
				return 0;
			}

			switch (key)
			{
			case KEY_GROUPLIST:
				ParsingGroupList((char*)lParam, HIWORD(wParam), strMsg);
				break;

			case KEY_GROUPAMD:
				ParsingGroupAMD((char*)lParam, HIWORD(wParam), strMsg);
				break;

			case KEY_GROUPACCOUNT:
				ParsingGroupAccount((char*)lParam, HIWORD(wParam), strMsg);
				break;
			}
		}
		break;
	}

	return 0;
}

BOOL CMapWnd::SendTR(CString strTR, CString strData, int nKey, int nStat)
{
	CString strTRData = "";
	
	struct _userTH user;
	strncpy(&user.trc[0], strTR, 8);
	user.key  = nKey;
	user.stat = nStat;
	
	strTRData = CString((char*)&user, sizeof(_userTH));
	strTRData += strData;
	
	return m_pWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strData.GetLength()), 
		(LPARAM)(const char*)strTRData); 
}

void CMapWnd::QueryGroupList()
{
	CString strUser(_T(""));

	strUser = Variant(userCC);

	_TRIMDATA(strUser);

	struct gqry_mid mid;
	FillMemory(&mid, L_gqry_mid, ' ');

	CopyMemory(&mid.In, _T("00001"), sizeof(mid.In));
	
	if ((long)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0L) == 0)
		mid.zUserTp[0] = _T('1');
	else
		mid.zUserTp[0] = _T('2');

	CopyMemory(&mid.zCreUserId, (LPCTSTR)strUser, strUser.GetLength());
	
	struct _ledgerH ledger;

	FillMemory(&ledger, L_ledgerH, ' ');
	m_pWizard->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);

	CopyMemory(&ledger.svcd, _T("SONAQ038"), sizeof(ledger.svcd));
	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '1';

	CString strSendData = CString((char*)&ledger, L_ledgerH);

	strSendData += CString((char*)&mid, L_gqry_mid);
	SendTR("PIBOPBXQ", strSendData, KEY_GROUPLIST, 0);
}

void CMapWnd::ParsingGroupList(CString strData, int nLen, CString strMsg)
{
	int nRec = atoi((LPCTSTR)strData.Mid(L_gqry_mid, 5));

	strData.Delete(0, L_gqry_mid + 5);

	int nCount = 0;

	for (int i = 0; i < nRec; i++)
	{
		struct gqry_rec rec;

		CopyMemory(&rec, (LPCTSTR)strData.Left(L_gqry_rec), L_gqry_rec);

		if (rec.zFundKindTp[0] == _T('2'))
		{
			CString strGroupID(rec.zFundNm, sizeof(rec.zFundNm));
			CString strGroupName(rec.zCampDescr, sizeof(rec.zCampDescr));

			_TRIMDATA(strGroupID);
			_TRIMDATA(strGroupName);

			m_pGroupGrid->InsertRow(i - nCount);
			m_pGroupGrid->QuickSetText(0, i - nCount, (LPCTSTR)strGroupID);
			m_pGroupGrid->QuickSetText(1, i - nCount, (LPCTSTR)strGroupName);
			m_pGroupGrid->QuickSetText(3, i - nCount, _T("Y"));
			m_pGroupGrid->RedrawRow(i - nCount);
		}
		else
			nCount++;

		strData.Delete(0, L_gqry_rec);
	}

	if (nRec > 0)
		OnRowChange(-1, 0, m_pGroupGrid.get());
}

void CMapWnd::QueryGroupAMD(int nMode)
{
	CString strUser(_T(""));
	CString strPass(_T(""));
	CString strGroupID(_T(""));
	CString strGroupName(_T(""));
	CString strAccount(_T(""));
	CString strPassword(_T(""));
	CString strRate(_T(""));

	strUser = Variant(userCC);
	_TRIMDATA(strUser);

	strPass = Variant(passCCx);//(char*)AfxGetMainWnd()->SendMessage(WM_USER, 0x01, 0L);
	_TRIMDATA(strPass);

	const int nRow = m_pGroupGrid->GetCurrentRow();

	strGroupID = m_pGroupGrid->QuickGetText(0, nRow);
	strGroupName = m_pGroupGrid->QuickGetText(1, nRow);

	strAccount = m_pSelGrid->QuickGetText(0, m_pSelGrid->GetCurrentRow());
	strAccount.Remove(_T('-'));
	_TRIMDATA(strAccount);

	if (strAccount.IsEmpty())
		return;

	strPassword = Variant(passCC, strAccount);
	_TRIMDATA(strPassword);

	// 그룹삭제, 계좌삭제일 경우에는 계좌비밀번호 저장 여부를 검사하지 않는다. 
	// by warship 2010.10.26
	if (nMode!=4 && nMode!=6)
	{
		if (strPassword.IsEmpty())
		{
			MessageBox("계좌비밀번호 저장 후 사용하십시오.");
			return;
		}
	}

	strRate = m_pSelGrid->QuickGetText(2, m_pSelGrid->GetCurrentRow());
	_TRIMDATA(strRate);
	const int nRate = atoi((LPCTSTR)strRate);
	const double dRate = atof((LPCTSTR)strRate) - nRate;

	CString strTemp;
	strRate.Format("+%.9d", nRate);
	strTemp.Format("%.8f", dRate);
	strTemp.Delete(0);

	strRate += strTemp;

	struct gamd_mid mid;
	FillMemory(&mid, L_gamd_mid, ' ');

	CopyMemory(&mid.In, _T("00001"), sizeof(mid.In));

	mid.zTrxTp[0] = nMode + 0x30;

	if ((long)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0L) == 0)
		mid.zUserTp[0] = _T('1');
	else
		mid.zUserTp[0] = _T('2');


	CopyMemory(&mid.zUserId, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&mid.zEmpPwd, (LPCTSTR)strPass, strPass.GetLength());
	CopyMemory(&mid.zFundNm, (LPCTSTR)strGroupID, strGroupID.GetLength());
	CopyMemory(&mid.zFundPtnCode, _T("2 "), sizeof(mid.zFundPtnCode));
	CopyMemory(&mid.zAcntNo, (LPCTSTR)strAccount, strAccount.GetLength());
	CopyMemory(&mid.zPwd, (LPCTSTR)strPassword, strPassword.GetLength());
	CopyMemory(&mid.zIsuNo, _T("A000660     "), sizeof(mid.zIsuNo));
	CopyMemory(&mid.lOrdAbleQty, _T("+000000000000000"), sizeof(mid.lOrdAbleQty));
	CopyMemory(&mid.dRatVal, (LPCTSTR)strRate, strRate.GetLength());
	CopyMemory(&mid.dWghtval, _T("+000000000.00000000"), sizeof(mid.dWghtval));
	mid.zPrcPtnTp[0] = _T('1');
	mid.zOrdMthdTp[0] = _T('1');
	CopyMemory(&mid.lOrdQty, _T("+000000000000000"), sizeof(mid.lOrdQty));
	CopyMemory(&mid.dOrdPrc, _T("+000000000.00"), sizeof(mid.dOrdPrc));
	mid.zBnsTp[0] = _T('1');
	CopyMemory(&mid.zFundDescr40, (LPCTSTR)strGroupName, strGroupName.GetLength());
	mid.zMktTp[0] = _T('1');

	struct _ledgerH ledger;

	FillMemory(&ledger, L_ledgerH, ' ');
	m_pWizard->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);

	CopyMemory(&ledger.svcd, _T("SONAT626"), sizeof(ledger.svcd));
	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '3';
	ledger.odrf[0] = '1';

	CString strSendData = CString((char*)&ledger, L_ledgerH);


	strSendData += CString((char*)&mid, L_gamd_mid);
	SendTR("PIBOPBXQ", strSendData, KEY_GROUPAMD, 0);
}

void CMapWnd::ParsingGroupAMD(CString strData, int nLen, CString strMsg)
{
	struct gamd_mid mid;

	CopyMemory(&mid, (LPCTSTR)strData, L_gamd_mid);

	CString strID(_T(""));
	CString strName(_T(""));
	CString strGroupID = CString(mid.zFundNm, sizeof(mid.zFundNm));
	CString strGroupName = CString(mid.zFundDescr40, sizeof(mid.zFundDescr40));

	_TRIMDATA(strGroupID);
	_TRIMDATA(strGroupName);
	m_pID->GetWindowText(strID);
	m_pName->GetWindowText(strName);

	ASSERT(strID == strGroupID);
	ASSERT(strName == strGroupName);

	const int nRow = FindGroup(strID);
	
	if (nRow == -1)
		return;

	if (mid.zTrxTp[0] == _T('3'))
	{
		m_pGroupGrid->QuickSetText(1, nRow, (LPCTSTR)strName);
		m_pGroupGrid->RedrawCell(1, nRow);	
	}
	else if (mid.zTrxTp[0] == _T('4'))
	{
		QueryGroupAccount();
	}
	else if (mid.zTrxTp[0] == _T('6'))
	{
		m_pGroupGrid->DeleteRow(nRow);
		QueryGroupAccount();
	}
	else if (mid.zTrxTp[0] == _T('2'))
	{
		const int nRow = m_pGroupGrid->GetCurrentRow();
		m_pGroupGrid->QuickSetText(3, nRow, _T("Y"));

		QueryGroupAccount();
	}
}

void CMapWnd::QueryGroupAccount()
{
	ClearSelGrid();

	CString strUser(_T(""));
	CString strPass(_T(""));
	CString strGroupID(_T(""));
	CString strGroupName(_T(""));

	strUser = Variant(userCC);
	_TRIMDATA(strUser);

	strPass = Variant(passCCx);
	_TRIMDATA(strPass);

	const int nRow = m_pGroupGrid->GetCurrentRow();

	if (nRow < 0)
		return;

	strGroupID = m_pGroupGrid->QuickGetText(0, nRow);
	strGroupName = m_pGroupGrid->QuickGetText(1, nRow);

	struct aqry_mid mid;

	FillMemory(&mid, L_aqry_mid, ' ');
	CopyMemory(&mid.In, _T("00001"), sizeof(mid.In));
	
	if ((long)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0L) == 0)
		mid.zUserTp[0] = _T('1');
	else
		mid.zUserTp[0] = _T('2');

	CopyMemory(&mid.zCreUserId, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&mid.zPwd, (LPCTSTR)strPass, strPass.GetLength());
	CopyMemory(&mid.zFundNm, (LPCTSTR)strGroupID, strGroupID.GetLength());
	mid.zFundKindTp[0] = _T('2');

	struct _ledgerH ledger;

	FillMemory(&ledger, L_ledgerH, ' ');
	m_pWizard->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);

	CopyMemory(&ledger.svcd, _T("SONAQ039"), sizeof(ledger.svcd));
	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '3';
	ledger.odrf[0] = '1';

	CString strSendData = CString((char*)&ledger, L_ledgerH);


	strSendData += CString((char*)&mid, L_aqry_mid);
	SendTR("PIBOPBXQ", strSendData, KEY_GROUPACCOUNT, 0);
}

void CMapWnd::ParsingGroupAccount(CString strData, int nLen, CString strMsg)
{
	int nRec = atoi((LPCTSTR)strData.Mid(L_aqry_mid, 5));

	strData.Delete(0, L_aqry_mid + 5);

	for (int i = 0; i < nRec; i++)
	{
		struct aqry_rec rec;

		CopyMemory(&rec, (LPCTSTR)strData.Left(L_aqry_rec), L_aqry_rec);

		CString strAccount(rec.zAcntNo, sizeof(rec.zAcntNo));
		CString strAccName(rec.zAcntNm, sizeof(rec.zAcntNm));
		CString strRat(rec.dRatVal, sizeof(rec.dRatVal));
		
		_TRIMDATA(strAccount);
		_TRIMDATA(strAccName);
		_TRIMDATA(strRat);

		strAccount.Insert(3, _T('-'));
		strAccount.Insert(6, _T('-'));

		const double dRat = atof((LPCTSTR)strRat);

		strRat.Format("%.2f", dRat);

		m_pSelGrid->InsertRow(i);
		m_pSelGrid->QuickSetText(0, i, (LPCTSTR)strAccount);
		m_pSelGrid->QuickSetText(1, i, (LPCTSTR)strAccName);
		m_pSelGrid->QuickSetText(2, i, (LPCTSTR)strRat);
		
		m_pSelGrid->RedrawRow(i);

		strData.Delete(0, L_aqry_rec);
	}

	if (nRec > 0)
		OnRowChange(-1, 0, m_pSelGrid.get());
}

void CMapWnd::OnAddModify()
{
	CString strID(_T(""));
	CString strName(_T(""));

	m_pID->GetWindowText(strID);
	m_pName->GetWindowText(strName);
	/*
	if (strID.GetLength()<3)
	{
		MessageBox("그룹ID는 3자리로 입력 해주시기 바랍니다.","IBK투자증권");
		m_pID->SetSel(0,-1);
		m_pID->SetFocus();
		return;
	}
	*/

	if (strID.IsEmpty() || strName.IsEmpty())
		return;

	if (FindGroup(strID) == -1)
	{
		const int nCurRow = m_pGroupGrid->GetNumberRows();
		m_pGroupGrid->InsertRow(nCurRow);
		m_pGroupGrid->GotoRow(nCurRow);

		m_pGroupGrid->QuickSetText(0, nCurRow, (LPCTSTR)strID);
		m_pGroupGrid->QuickSetText(1, nCurRow, (LPCTSTR)strName);
		m_pGroupGrid->QuickSetText(3, nCurRow, _T("N"));

		m_pGroupGrid->RedrawRow(nCurRow);

		ClearSelGrid();
	}
	else
	{
		if (m_pSelGrid->GetNumberRows() > 0)
			QueryGroupAMD(3);	// 수정
		else
		{
			const int nFindRow = FindGroup(strID);
			m_pGroupGrid->QuickSetText(1, nFindRow, (LPCTSTR)strName);
			m_pGroupGrid->RedrawCell(1, nFindRow);
		}
	}
}

void CMapWnd::OnRowChange(int oldRow, int newRow, CWnd* pWnd)
{
	if (pWnd == m_pGroupGrid.get())
	{
		CString strGroupID = m_pGroupGrid->QuickGetText(0, newRow);
		CString strGroupName = m_pGroupGrid->QuickGetText(1, newRow);

		if (!strGroupID.IsEmpty())
			m_pID->SetWindowText(strGroupID);

		if (!strGroupName.IsEmpty())
			m_pName->SetWindowText(strGroupName);

		QueryGroupAccount();
	}
	else if (pWnd == m_pSelGrid.get())
	{
		CString strAccount = m_pSelGrid->QuickGetText(0, newRow);
		CString strRate = m_pSelGrid->QuickGetText(2, newRow);

		m_pAccount->SetText(strAccount);
		m_pRate->SetWindowText(strRate);
	}
//	else if (pWnd == m_pAccGrid)
//		;
}

int CMapWnd::FindGroup(CString strID)
{
	const int nCount = m_pGroupGrid->GetNumberRows();

	for (int i = 0; i < nCount; i++)
	{
		if (strID == m_pGroupGrid->QuickGetText(0, i))
			return i;
	}

	return -1;
}

void CMapWnd::OnGroupDelete()
{
	QueryGroupAMD(6);
}

void CMapWnd::ClearSelGrid()
{
	while (m_pSelGrid->GetNumberRows() != 0)
		m_pSelGrid->DeleteRow(0);
}

void CMapWnd::OnSelModify()
{
	CString strRate(_T(""));
	const int nRow = m_pSelGrid->GetCurrentRow();

	if (nRow < 0)
		return;

	m_pRate->GetWindowText(strRate);

	if (atof((LPCTSTR)strRate) <= 0)
	{
		MessageBox("배수는 0.00보다 큰 값을 입력 해야 합니다.");
		return;
	}

	m_pSelGrid->QuickSetText(2, nRow, (LPCTSTR)strRate);
	m_pSelGrid->RedrawCell(2, nRow);

	QueryGroupAMD(3);
}

void CMapWnd::OnSelDelete()
{
	if (m_pSelGrid->GetCurrentRow() < 0)
		return;

	QueryGroupAMD(4);
}

void CMapWnd::OnDestroy() 
{
	CString strGroup(_T("OnGroupAcc\t"));
	
	const int nRow = m_pGroupGrid->GetNumberRows();

	for (int i = 0; i < nRow; i++)
	{
		strGroup += m_pGroupGrid->QuickGetText(0, i);
		strGroup += _T("\t");
		strGroup += m_pGroupGrid->QuickGetText(1, i);
		strGroup += _T("\n");
	}

	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)strGroup);

	CWnd::OnDestroy();
}


void CMapWnd::OnOk()
{
	const int nRows = m_pGroupGrid->GetNumberRows();
	CString strMsg(_T(""));
	CString strGroup(_T(""));
	CString strVal(_T(""));
	BOOL bEmpty = FALSE;
	
	for (int i = 0; i < nRows; i++)
	{
		strVal = m_pGroupGrid->QuickGetText(3, i);
		if (strVal == _T("N"))
		{
			strGroup.Format("[%s]", m_pGroupGrid->QuickGetText(0, i));
			
			if (!strMsg.IsEmpty())
				strMsg += _T(", ");

			strMsg += strGroup;
			bEmpty = TRUE;
		}
	}

	if (bEmpty)
	{
		strMsg += _T(" 그룹에 포함된 계좌가 없으므로 저장되지 않습니다.");	
		MessageBox(strMsg);
	}

	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(closeDLL, 0), 0L);
}


void CMapWnd::DrawBitmap(CDC* pDC, CRect rc, HBITMAP hBitmap)
{
	HDC hDC = ::CreateCompatibleDC (NULL);
	SelectObject (hDC, hBitmap); //icon
//	::BitBlt( pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(),	hDC, 0, 0, SRCCOPY);
	::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);

	::DeleteDC (hDC);
}

void CMapWnd::DrawCornerRoundBitmap(CDC* pDC, CRect rc, int iType)
{
	const int iPoint = 5;
	int idx = 0;
	DrawBitmap(pDC, CRect(rc.left, rc.top, rc.left+iPoint, rc.top +iPoint), m_hRoundBitmap[idx++]);
	DrawBitmap(pDC, CRect(rc.right-iPoint, rc.top, rc.right, rc.top +iPoint), m_hRoundBitmap[idx++]);
	DrawBitmap(pDC, CRect(rc.left, rc.bottom-iPoint, rc.left+iPoint, rc.bottom), m_hRoundBitmap[idx++]);
	DrawBitmap(pDC, CRect(rc.right-iPoint, rc.bottom-iPoint, rc.right, rc.bottom), m_hRoundBitmap[idx++]);
}

void CMapWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	m_iHeight = cy;
	// TODO: Add your message handler code here
	
}

BOOL CMapWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( wParam != IDC_GROUPGRID) return 0;
/*
	NM_GVITEM* nmgv = (NM_GVITEM*) lParam;
	switch (nmgv->hdr.code)
	{
		case case GVNM_LMOUSEDOWN:
			break; 
	}	
	*/
	return CWnd::OnNotify(wParam, lParam, pResult);
}
