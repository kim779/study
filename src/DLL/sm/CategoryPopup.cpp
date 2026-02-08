// CategoryPopup.cpp : implementation file
//

#include "stdafx.h"
#include "CategoryPopup.h"

#include "../../h/interSt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable:4800)

#define IDC_STATIC_ORDER 90
#define IDC_COMBO_ORDER 91
#define IDC_STATIC_QC	92
#define IDC_STATIC_NT   93

#define IDC_CHECK_BGJ 100
#define IDC_CHECK_SSJ 101
#define IDC_CHECK_KOJ 102
#define IDC_CHECK_WSJ 103
#define IDC_CHECK_KOD 104
#define IDC_CHECK_KLJ 105
#define IDC_CHECK_GLJ 106
#define IDC_CHECK_KJJ 107
#define IDC_CHECK_TUJ 108
#define IDC_CHECK_KOP 109

#define IDC_BTN_PLAY  110
#define IDC_BTN_CLOSE 111
#define IDC_BTN_SEARCH 112
#define IDC_BTN_PLAY2 113

#define IDC_LIST_OUTPUT 120

/////////////////////////////////////////////////////////////////////////////
// CCategoryPopup

#define PT_TREE_H 244
#define PT_TREE_V 155

#define PT_STATIC_Y 13
#define PT_TREE_Y	33
#define PT_CHECK_Y	42

#define PT_TREE_X 11


#define CL_EDGE_POP1 RGB(91,102,132)
#define CL_EDGE_POP2 RGB(229,229,231)
#define CL_EDGE_POP3 RGB(170,186,212)
#define CL_EDGE_POP4 RGB(116,116,116)
#define CL_BK		RGB(211,211,211)
#define CL_BK_GROUP RGB(235,235,235)
#define CL_TEXT		RGB(100,100,100)
#define CL_EDGE		RGB(152,152,152)
#define CL_TREE_HT RGB(0,144,171)
#define CL_TREE_HC RGB(27,178,207)
#define CL_BK_QR	RGB(106,106,106)
#define CL_BK_HD	RGB(182,182,182)

CCategoryPopup::CCategoryPopup(CWnd* parent)
{
	m_parent = parent;
	m_pTree = NULL;
	m_nKind = 0;
	for(int i = 0 ; i < CHECKER_COUNT ; i++)
	{
		m_pCheckBtn[i] = NULL;
		if( i == 0)
			m_rcCheck[i] = CRect( PT_TREE_V + 17 , PT_CHECK_Y  , PT_TREE_V + 17 + 130 , PT_CHECK_Y + 4+ 15);
		else
			m_rcCheck[i] = CRect( PT_TREE_V + 17 , m_rcCheck[i-1].bottom + 4, PT_TREE_V + 17 + 125 , m_rcCheck[i-1].bottom +4 +15);
	}
	
	for(  int i = 0 ; i < CHECKER_COUNT ; i++ )
	{
		if( i==2 ||i==3 ||i==5 ||i==6 ||i==7 ||i==8)
			m_bCateChecked[i] = true;
		else
			m_bCateChecked[i] = false;
	}

	m_szCheckName[0] = "불성실공시종목 제외";
	m_szCheckName[1] = "신규상장종목 제외";
	m_szCheckName[2] = "권리락종목 제외";
	m_szCheckName[3] = "우선주 제외";
	m_szCheckName[4] = "코스닥종목 제외";
	m_szCheckName[5] = "관리종목 제외";
	m_szCheckName[6] = "감리종목 제외";
	m_szCheckName[7] = "거래정지종목 제외";
	m_szCheckName[8] = "투자유의종목 제외";
	m_szCheckName[9] = "코스피종목 제외";

	m_pCloseBtn = NULL;
	m_pPlayBtn = NULL;

	m_pStatic = NULL;
	m_pComboOrder = NULL;
	m_pSearchBtn = NULL;
	m_pQCStatic = NULL;
	m_pList = NULL;
	m_brBk.CreateSolidBrush( CL_BK);//m_pMainWnd->getAxColor(64 ) );
	m_brSt.CreateSolidBrush( RGB(255,255,255));//m_pMainWnd->getAxColor(66 ) );
	m_brBk1.CreateSolidBrush( CL_BK_GROUP);

	m_elwbase.RemoveAll();
}

CCategoryPopup::~CCategoryPopup()
{
#ifdef DF_USESTL_CATEGORY

#else
	for(int i = 0 ; i < 10 ; i++)
	{
		if( m_pCheckBtn[i] )
			delete m_pCheckBtn[i];
		m_pCheckBtn[i] = NULL;
	}

	if( m_pStatic )
		delete m_pStatic;
	if( m_pComboOrder)
		delete m_pComboOrder;
	
	if( m_pQCStatic )
		delete m_pQCStatic;
	if( m_pSearchBtn)
		delete m_pSearchBtn;
	if( m_pPlayBtn)
		delete m_pPlayBtn;
	if( m_pCloseBtn )
		delete m_pCloseBtn;

	if( m_pList )
		delete m_pList;

	for(int i = 0; i < m_hjCodes.GetSize(); i++){
		_mhjcode* pcode = m_hjCodes.GetAt(i);
		delete pcode;
	}
	
	for (int ii = 0; ii < m_codeArr.GetSize(); ii++)
	{
		m_codeArr.GetAt(ii)->RemoveAll();
		delete m_codeArr.GetAt(ii);
	}
	m_codeArr.RemoveAll();	
#endif
}

/** **/
#define ID_CX_CategoryTREE	1001

BEGIN_MESSAGE_MAP(CCategoryPopup, CWnd)
	//{{AFX_MSG_MAP(CCategoryPopup)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_NOTIFY(NM_DBLCLK, ID_CX_CategoryTREE, OnTreeDblClk)
	ON_WM_DESTROY()
	ON_CONTROL_RANGE(BN_CLICKED,IDC_CHECK_BGJ,IDC_CHECK_KOP,OnChecked)
	ON_BN_CLICKED(IDC_BTN_PLAY,OnPlayClk)
	ON_BN_CLICKED(IDC_BTN_CLOSE,OnCloseClk)
	ON_BN_CLICKED(IDC_BTN_SEARCH,OnSearchClk)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_SIZING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCategoryPopup message handlers

HBRUSH CCategoryPopup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	
	if( nCtlColor == CTLCOLOR_STATIC  ) 
	{
		if( pWnd->GetSafeHwnd() == m_pQCStatic->GetSafeHwnd()){
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(CL_TEXT);
			return (HBRUSH)m_brSt;
		}
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor( CL_TEXT );
		return (HBRUSH)m_brBk1;
		
	}
	return hbr;
}
BOOL CCategoryPopup::OnEraseBkgnd(CDC* pDC) 
{
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(rc,CL_BK);//m_pMainWnd->getAxColor(64 ));	
	return CWnd::OnEraseBkgnd(pDC);
}

int CCategoryPopup::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//**loadHjCode(); 처음엔 안띠움.

	m_font.CreatePointFont(90, "굴림");

	InitControls();
	InitListCtrl();

	HTREEITEM hItem = m_pTree->GetFirstVisibleItem();

	m_pTree->SelectItem(hItem);

	/**
	CString temp = m_pMainWnd->Variant(historyCC, "1301");
	**/
	CString temp("");

	while(!temp.IsEmpty()){
		
		CString temp1 = Parser( temp,"\t").Left(6);
		temp1.TrimRight();
		m_history.Add(temp1);

	}
	return 0;
}

void CCategoryPopup::InitControls(){
#ifdef DF_USESTL_CATEGORY
	m_pTree = std::make_unique<CTreeCtrl>();
	m_pStatic = std::make_unique<CStatic>();
	m_pComboOrder = std::make_unique<CSComboBox>();
#else
	m_pTree = new CTreeCtrl();
	m_pStatic = new CStatic();
	m_pComboOrder = new CSComboBox();
#endif
	
	m_pTree->Create(WS_VISIBLE |WS_HSCROLL| WS_TABSTOP | WS_CHILD /*| WS_BORDER */| TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_DISABLEDRAGDROP |TVS_SHOWSELALWAYS,
		CRect(PT_TREE_X, PT_TREE_Y, PT_TREE_V, PT_TREE_H ), this, ID_CX_CategoryTREE);
//	m_pTree->ModifyStyleEx(NULL, WS_EX_CLIENTEDGE);
	baseCX_Category();
	m_pTree->SetFont(GetFont());
	
	m_pStatic->Create("조회순서", WS_VISIBLE|WS_CHILD , 
		CRect(PT_TREE_V+10, PT_STATIC_Y + 8,PT_TREE_V+10+50,PT_STATIC_Y  + 8 + 11),this, IDC_STATIC_ORDER );
	m_pStatic->SetFont(&m_font);

	m_pComboOrder->Create(WS_VISIBLE|WS_CHILD|WS_TABSTOP
			|WS_VSCROLL|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBS_DROPDOWNLIST|CBS_HASSTRINGS    , 
		CRect( PT_TREE_V+10+50, PT_STATIC_Y + 4, /*PT_TREE_V+10+50 + 130*/m_rcCheck[1].right+50 , PT_STATIC_Y  + 4 + 15 ), this, IDC_COMBO_ORDER);
	m_pComboOrder->SetFont(&m_font);
	
	m_pComboOrder->InsertString(0,"종목코드순");
	m_pComboOrder->InsertString(1,"종목코드순(코스피->코스닥)");
	m_pComboOrder->InsertString(2,"종목명순");
	m_pComboOrder->InsertString(3,"종목명순(코스피->코스닥)");
	
	m_pComboOrder->SetCurSel(0);
	CRect wRc;
	m_pComboOrder->GetWindowRect(&wRc);
	m_pComboOrder->SetWindowPos(NULL, 0, 0, wRc.Width(), 20*4, SWP_NOMOVE|SWP_SHOWWINDOW);
		
	for( int i = 0 ; i < CHECKER_COUNT ; i++ )
	{
#ifdef DF_USESTL_CATEGORY
	m_pCheckBtn[i] = std::make_unique<CButton>();
#else
	m_pCheckBtn[i] = new CButton();
#endif	
		m_pCheckBtn[i]->Create( m_szCheckName[i],WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			m_rcCheck[i], this, IDC_CHECK_BGJ + i  );
		
		m_pCheckBtn[i]->SetFont(&m_font);
		m_pCheckBtn[i]->SetCheck(m_bCateChecked[i]);
	}

	CString str2n,str2d,str2u;
	
	str2n = Axis::home + "\\image\\" + "2BTN.BMP";
	str2d = Axis::home + "\\image\\" + "2BTN_DN.BMP";
	str2u = Axis::home + "\\image\\" + "2BTN_EN.BMP";

	CBitmap* bmp{};
	int cx{}, cy{};

	bmp = Axis::GetBitmap("2BTN");
	if (bmp)
	{
		BITMAP bm;
		bmp->GetBitmap(&bm);
		cx = bm.bmWidth;
		cy = bm.bmHeight;
	}
	else
	{
		cx = cy = 0;
	}
#ifdef DF_USESTL_CATEGORY
	m_pSearchBtn = std::make_unique<CxSkinButton>();
#else 
	m_pSearchBtn = new CxSkinButton();
#endif
	m_pSearchBtn->Create("조회",  WS_VISIBLE | WS_CHILD , 
		CRect(PT_CX-15-cx,m_rcCheck[9].bottom-18 ,PT_CX-15,m_rcCheck[9].bottom+2),	this, IDC_BTN_SEARCH );
	m_pSearchBtn->SetFont( &m_font);

	m_pSearchBtn->SetSkin( str2n,str2d,str2u);

#ifdef DF_USESTL_CATEGORY
	m_pQCStatic = std::make_unique<CSStatic>();
#else 
	m_pQCStatic = new CSStatic();
#endif
	m_pQCStatic->Create("",WS_VISIBLE | WS_CHILD |SS_CENTER  , 
		CRect(PT_TREE_X, PT_TREE_H  + 26 , PT_TREE_V , PT_TREE_H + 10 + 70 ), this, IDC_STATIC_QC);
	m_pQCStatic->SetFont( &m_font);

#ifdef DF_USESTL_CATEGORY
	m_pList = std::make_unique<CExListCtrl>();
#else 
	m_pList = new CExListCtrl();
#endif	
	m_pList->Create(WS_CHILD|WS_VISIBLE/*|WS_BORDER*/|LVS_REPORT|WS_HSCROLL|WS_VSCROLL, 
		CRect(PT_TREE_V, PT_TREE_H  + 10 , PT_CX-10/*m_rcCheck[1].right+50*/ , PT_TREE_H + 10 + 70),this, IDC_LIST_OUTPUT );

	m_pList->SetFont(&m_font);
	
	const int buttonTop = 95;

#ifdef DF_USESTL_CATEGORY
	m_pPlayBtn = std::make_unique<CxSkinButton>();
#else 
	m_pPlayBtn = new CxSkinButton();
#endif	
	m_pPlayBtn->Create("추가", WS_VISIBLE | WS_CHILD , 
		//**CRect(PT_TREE_X ,PT_TREE_H + buttonTop ,PT_TREE_X + cx ,PT_TREE_H + buttonTop + cy),
		CRect(PT_CX - (10 + cx) * 2, PT_TREE_H + buttonTop, PT_CX - (10 + cx) * 2 + cx, PT_TREE_H + buttonTop + cy),
		this, IDC_BTN_PLAY );
	m_pPlayBtn->SetFont( &m_font);
	m_pPlayBtn->SetSkin(str2n,str2d,str2u);

#ifdef DF_USESTL_CATEGORY
	m_pCloseBtn = std::make_unique<CxSkinButton>();
#else 
	m_pCloseBtn = new CxSkinButton();
#endif	
	m_pCloseBtn->Create("취소", WS_VISIBLE | WS_CHILD , 
		CRect(PT_CX - 10 - cx, PT_TREE_H + buttonTop, PT_CX - 10, PT_TREE_H + buttonTop + cy),
		this, IDC_BTN_CLOSE );
	m_pCloseBtn->SetFont( &m_font);
	m_pCloseBtn->SetSkin(str2n,str2d,str2u);

	m_bmp1 = Axis::GetBitmap("분류아이콘");
	m_bmp2 = Axis::GetBitmap("경고아이콘");
}

void CCategoryPopup::InitListCtrl()
{
 	CString hData[] = {
		"코드",
		"종목명",
	};
	const int col = 2;
	int	width[col]{};
	CRect rect;
	m_pList->GetClientRect(&rect);
	const int tot = (int)(rect.Width() / col);

	const int swidth = GetSystemMetrics(SM_CXHSCROLL);
	width[0] = tot - 10;// - swidth;
	width[1] = tot + 10;

	m_pList->InitHeaderData(hData, width, col, LVCFMT_LEFT);
	m_pList->SetFullRowSelect();
}

void CCategoryPopup::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
	
	if (nState == WA_INACTIVE)
		ShowWindow(SW_HIDE);
}

void CCategoryPopup::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
}

void CCategoryPopup::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rc(0,0,PT_CX,PT_CY);
	CBrush br;
	br.CreateSolidBrush(CL_EDGE_POP1);
	dc.FrameRect( rc, &br);
	
	rc.DeflateRect(1,1,1,1);
	br.DeleteObject();
	br.CreateSolidBrush(CL_EDGE_POP2);
	dc.FrameRect( rc, &br);
	
	rc.DeflateRect(1,1,1,1);
	br.DeleteObject();
	br.CreateSolidBrush(CL_EDGE_POP3);
	dc.FrameRect( rc, &br);
	
	rc.DeflateRect(1,1,1,1);
	br.DeleteObject();
	br.CreateSolidBrush(CL_EDGE_POP4);
	dc.FrameRect( rc, &br);

	CFont* pOld = NULL;	

	rc = CRect(PT_TREE_X,PT_STATIC_Y,PT_TREE_V,PT_TREE_H);	
	rc.InflateRect(1,1,1,1);

	br.DeleteObject();
	br.CreateSolidBrush(CL_EDGE);
	dc.FrameRect( rc, &br);
	
	rc = CRect (PT_TREE_V+6 ,PT_STATIC_Y,PT_CX-10,PT_TREE_H);
	rc.InflateRect(1,1,1,1);
	br.DeleteObject();
	br.CreateSolidBrush(CL_EDGE);
	dc.FrameRect( rc, &br);
	rc.DeflateRect(1,1,1,1);
	dc.FillSolidRect(rc, CL_BK_GROUP);
	
	rc = CRect(PT_TREE_X,PT_STATIC_Y,PT_TREE_V,PT_STATIC_Y+2);
	dc.FillSolidRect(rc, CL_TREE_HC);
	rc = CRect(PT_TREE_X,PT_STATIC_Y+2,PT_TREE_V,PT_TREE_Y);
	dc.FillSolidRect(rc, CL_TREE_HT);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(253,253,255));
	pOld = dc.SelectObject(&m_font);
	rc.DeflateRect(30,0,0,0);
	dc.DrawText("분류",rc, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
	dc.SelectObject(pOld);
	if( m_bmp1 && m_bmp1->GetSafeHandle() ){
		
		BITMAP	bm;

		m_bmp1->GetBitmap(&bm);
		const int bx = bm.bmWidth;
		const int by = bm.bmHeight;
			
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		memDC.SelectObject(m_bmp1);
		rc.InflateRect(25,0,0,0);
			
		dc.BitBlt(rc.left, rc.top , bx,by,&memDC,0,0,SRCCOPY);
		memDC.DeleteDC();
	}

	rc = CRect( PT_TREE_X , PT_TREE_H + 10 , PT_TREE_V ,PT_TREE_H +26 );
	dc.Draw3dRect( rc, RGB(156,156,156),RGB(80,80,80));
	rc.DeflateRect(1,1,1,1);
	dc.FillSolidRect(rc, CL_BK_QR);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(253,253,244));
	pOld = dc.SelectObject(&m_font);
	dc.DrawText("!조회개수(최대1000개)",rc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	dc.SelectObject(pOld);

	rc = CRect( PT_TREE_X , PT_TREE_H + 10 , PT_CX-10 , PT_TREE_H + 80 );
	rc.InflateRect(1,1,1,1);
	dc.Draw3dRect( rc, CL_BK_QR, RGB(222,222,222));

	rc = CRect( PT_TREE_X , PT_TREE_H + 85 , PT_CX-10 , PT_TREE_H + 87 );
	dc.Draw3dRect( rc, CL_EDGE,CL_BK_GROUP);	
}

void CCategoryPopup::baseCX_Category()
{
	if (!m_pTree)
		return;

	CString dir;
	HTREEITEM hItem1{};
	dir.Format("%s\\tab\\", Axis::home);

	switch (m_nKind)
	{
	case 0:		// 현재가 분류
		//**hItem1 = m_pTree->InsertItem("History", 0, 1);
		//**m_pTree->SetItemData(hItem1, MAKELONG(0, 4));
		
		hItem1 = m_pTree->InsertItem("전종목", 0, 1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 5));
		//거래소 전종목
		hItem1 = m_pTree->InsertItem("거래소",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 7));
		
		//코스닥 전종목
		hItem1 = m_pTree->InsertItem("코스닥",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 8));
		
		//코스피,코스탁 세분류..
		loadUCode(dir + "upcode.dat", TVI_ROOT );
		
		//KOSPI200
		hItem1 = m_pTree->InsertItem("KOSPI200",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 9));

		//KOSDAQ50
		hItem1 = m_pTree->InsertItem("KOSDAQ50",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 10));

//		LoadHjCode(dir + "hjcode.dat" ,TVI_ROOT , "KOSDAQ50" );
		//KOSDAQ50
//		loadUCode(dir + "upcode.dat", TVI_ROOT , "KOSDAQ50");
		
		//KRX100
		hItem1 = m_pTree->InsertItem("KRX100",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 11));
		
		//프리보드
		hItem1 = m_pTree->InsertItem("프리보드",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 12));
		
		//ETF
		hItem1 = m_pTree->InsertItem("ETF",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 13));
		
		//ELW
		hItem1 = m_pTree->InsertItem("ELW",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 14));

		//ELW기초자산
		hItem1 = m_pTree->InsertItem("ELW기초자산",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 15));

		//관심종목
		hItem1 = m_pTree->InsertItem("관심종목", 0, 1);
		m_InterITEM = hItem1;
		m_pTree->SetItemData(hItem1, MAKELONG(0, -1));
		LoadGroup(Axis::home + "\\user\\" + Axis::user + "\\portFolio.ini", hItem1);
		
		//테마종목
		hItem1 = m_pTree->InsertItem("테마종목", 0, 1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, -1));
		LoadFolder(dir + "themcd.ini", hItem1, "TMCODE", 6);

		//그룹사별
		hItem1 = m_pTree->InsertItem("그룹사별", 0, 1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, -1));
		LoadFolder(dir + "gropcd.ini", hItem1, "GRCODE", 0);

		break;
	case 1:		// 관심종목 분류
		//거래소 전종목
		hItem1 = m_pTree->InsertItem("거래소",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 7));
		
		//코스닥 전종목
		hItem1 = m_pTree->InsertItem("코스닥",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 8));
		
		//코스피,코스탁 세분류..
		loadUCode(dir + "upcode.dat", TVI_ROOT );
		
		//KOSPI200
		hItem1 = m_pTree->InsertItem("KOSPI200",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 9));

		//KOSDAQ50
		hItem1 = m_pTree->InsertItem("KOSDAQ50",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 10));

//		LoadHjCode(dir + "hjcode.dat" ,TVI_ROOT , "KOSDAQ50" );
		//KOSDAQ50
//		loadUCode(dir + "upcode.dat", TVI_ROOT , "KOSDAQ50");
		
		//KRX100
		hItem1 = m_pTree->InsertItem("KRX100",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 11));
		
		//프리보드
		hItem1 = m_pTree->InsertItem("프리보드",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 12));
		
		//ETF
		hItem1 = m_pTree->InsertItem("ETF",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 13));
		
		//ELW
		hItem1 = m_pTree->InsertItem("ELW",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 14));

		//ELW기초자산
		hItem1 = m_pTree->InsertItem("ELW기초자산",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 15));

		//관심종목
		hItem1 = m_pTree->InsertItem("관심종목", 0, 1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, -1));
		LoadGroup(Axis::home + "\\user\\" + Axis::user + "\\portFolio.ini", hItem1);
		
		//테마종목
		hItem1 = m_pTree->InsertItem("테마종목", 0, 1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, -1));
		LoadFolder(dir + "themcd.ini", hItem1, "TMCODE", 6);

		//그룹사별
		hItem1 = m_pTree->InsertItem("그룹사별", 0, 1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, -1));
		LoadFolder(dir + "gropcd.ini", hItem1, "GRCODE", 0);
		
		break;
	}

}
//필터링 해서 조건에 맞는 경우만 참 리턴
bool CCategoryPopup::IsFilteredHj(_mhjcode* hj)
{
	if( m_bCateChecked[0] &&	//불성실공시종목 
		(hj->jchk & 0x01) )
		return false;
	if( m_bCateChecked[1] &&//신규상장종목
		(hj->jchk & 0x02) )
		return false;
	if( m_bCateChecked[2] &&//권리락종목
		(hj->jchk & 0x04) )
		return false;
	if( m_bCateChecked[3] &&//우선주종목
		(hj->jchk & 0x08) )
		return false;
	if( m_bCateChecked[4] &&//코스닥 종목 제외
	    ( hj->kosd == jmKOSDAQ))
		return false;
	if( m_bCateChecked[5] &&//관리종목
		(hj->jchk & 0x10) )
		return false;
	if( m_bCateChecked[6] &&//감리종목
		(hj->jchk & 0x20) )
		return false;
	if( m_bCateChecked[7] &&//거래정지종목
		(hj->jchk & 0x40) )
		return false;
	if( m_bCateChecked[8] &&//투자유의종목
		(hj->jchk & 0x80) )
		return false;
	if( m_bCateChecked[9] &&//코스닥 종목 제외
	    ( hj->kosd == jmKOSPI))
		return false;

	return true;
}
void TrimLeftRight(char* pstr){
#ifdef DF_USESTL_CATEGORY
	int len = lstrlen(pstr);
	std::unique_ptr<char[]>temp = std::make_unique<char[]>(len);
	memset(temp.get(),0x00, len);
	//TrimLeft
	for( int i = 0 ; i < len-1 ;i++){
		
		if( pstr[i] == 0x20 && pstr[i+1] != 0x20){
			lstrcpy( temp.get(), pstr+i+1 );
			lstrcpy( pstr,temp.get());
			break;
		}
	}
	len = lstrlen(pstr);
	//TrimRight
	for( int i = len-1 ; i >= 0 ; i--){
		if( pstr[i] == 0x20) 
			pstr[i] = 0;
		else
			break;
	}
#else
	int len = lstrlen(pstr);
	char* temp = new char[len];
	memset(temp,0x00, len);
	//TrimLeft
	for( int i = 0 ; i < len-1 ;i++){
		
		if( pstr[i] == 0x20 && pstr[i+1] != 0x20){
			lstrcpy( temp, pstr+i+1 );
			lstrcpy( pstr,temp );
			break;
		}
	}
	len = lstrlen(pstr);
	//TrimRight
	for( int i = len-1 ; i >= 0 ; i--){
		if( pstr[i] == 0x20) 
			pstr[i] = 0;
		else
			break;
	}

	delete []temp;
#endif
}
#ifdef DF_USESTL_CATEGORY
void CCategoryPopup::SearchCode()
{
	loadHjCode();

	HTREEITEM hItem = m_pTree->GetSelectedItem();

	if( hItem == NULL )
		return ;
	
	if (HIWORD(m_pTree->GetItemData(hItem)) == 3)
	{
		const int nGroup = LOWORD(m_pTree->GetItemData(hItem));
		loadCodeList(nGroup);
		return;
	}

	int i{}, j = 0;
	m_shjCodes.RemoveAll();
	CString szCode = "", szName = "", szName1 = "";

	// 분류별 로드
	CArray< std::shared_ptr<_mhjcode>, std::shared_ptr<_mhjcode>> tempCodes;
	CString hnama="",hnamb="";
	
	tempCodes.Copy( m_hjCodes );

	//CStringArray*pArr = NULL;
	const int ntype = HIWORD(m_pTree->GetItemData(hItem));

	if( ntype == 0)//그룹사
	{
		auto pArr = m_codeArr.GetAt(LOWORD(m_pTree->GetItemData(hItem)));
		for( i = 0 ; i < pArr->GetSize(); i++)
		{
			szName1= pArr->GetAt(i);szName1.TrimLeft();szName1.TrimRight();
			szName1 = "A"+szName1;
			
			for( j = 0; j < tempCodes.GetSize() ; j++)
			{
				auto hj = tempCodes.GetAt(j);
				if( !IsFilteredHj(hj.get()))
					continue;
				if( hj->code == szName1)
				{
					m_shjCodes.Add(hj);
					break;	
				}
			}
			
		}
	}
	else if( ntype == 6 ) //테마종목
	{
		auto pArr = m_codeArr.GetAt(LOWORD(m_pTree->GetItemData(hItem)));
		for( i = 0 ; i < pArr->GetSize(); i++)
		{
			for( j = 0; j < tempCodes.GetSize() ; j++)
			{
				auto hj = tempCodes.GetAt(j);
				if( !IsFilteredHj(hj.get()))
					continue;

				szName1= pArr->GetAt(i);szName1.TrimLeft();szName1.TrimRight();
				szName1 = "A"+szName1;
				if( hj->code == szName1)
				{
					m_shjCodes.Add(hj);
					break;	
				}
			}
			
		}
	}
	else if( ntype == 2 ) //코스피 업종
	{
		for(i = 0; i < tempCodes.GetSize() ; i++)
		{
			auto hj = tempCodes.GetAt(i);
			if( !IsFilteredHj(hj.get()))
				continue;
			ListUpjongSort(LOWORD(m_pTree->GetItemData(hItem)), hj);
		}
	}
	else if( ntype == 3)	
	{
		const int nGroup = LOWORD(m_pTree->GetItemData(hItem));
	
	}
	else if( ntype == 4)	//히스토리
	{
		CString code = "";
		for(i = 0 ; i < m_history.GetSize(); i++)
		{
		
			code = m_history.GetAt(i);

			for( j = 0; j < tempCodes.GetSize() ; j++)
			{
				
				auto hj = tempCodes.GetAt(j);
				
				if( hj->code.Mid(1) == code)
				{
					m_shjCodes.Add(hj);
					break;	
				}
			}		
		}

	}
	else if( ntype == 5 ) //전종목
	{
		for(i = 0; i < tempCodes.GetSize() ; i++)
		{
			auto hj = tempCodes.GetAt(i);
			if( !IsFilteredHj(hj.get()))
				continue;
			
			m_shjCodes.Add(hj);
		}
	}
	else if( ntype == 7 )	//코스피 전종목
	{
		for(i = 0; i < tempCodes.GetSize() ; i++)
		{
			auto hj = tempCodes.GetAt(i);
			if( !IsFilteredHj(hj.get()))
				continue;
			if( hj->kosd == jmKOSPI)
			{
					
				m_shjCodes.Add(hj);
			}
		}
	}
	else if( ntype == 8 )	//코스닥 전종목
	{
		for(i = 0; i < tempCodes.GetSize() ; i++)
		{
			auto hj = tempCodes.GetAt(i);
			if( !IsFilteredHj(hj.get()))
				continue;
			if( hj->kosd == jmKOSDAQ)
			{
						
				m_shjCodes.Add(hj);
			}
		}
	}
	else if( ntype == 9 )	//KOSPI200
	{

		for(i = 0; i < tempCodes.GetSize() ; i++)
		{
			auto hj = tempCodes.GetAt(i);
			if( !IsFilteredHj(hj.get()))
				continue;

			if( hj->kosd == jmKOSPI)
			{
				if(hj->kpgb == 1 || hj->kpgb == 2 || hj->kpgb == 3 ) 
					m_shjCodes.Add(hj);
			}			
		}

	}
	else if( ntype == 10 )	//KOSDAQ50
	{
		for(i = 0; i < tempCodes.GetSize() ; i++)
		{
			auto hj = tempCodes.GetAt(i);
			if( !IsFilteredHj(hj.get()))
				continue;
			if( hj->kosd == jmKOSDAQ&& hj->kpgb == 1 )
			{
					
				m_shjCodes.Add(hj);
			}			
		}
	}
	else if( ntype == 11 )	//KRX100
	{
		for(i = 0; i < tempCodes.GetSize() ; i++)
		{
			auto hj = tempCodes.GetAt(i);
			if( !IsFilteredHj(hj.get()))
				continue;
			if( hj->unio == 1)
			{		
				m_shjCodes.Add(hj);
			}
		}
		
	}
	else if( ntype == 12 )	//프리보드
	{
		for(i = 0; i < tempCodes.GetSize() ; i++)
		{
			auto hj = tempCodes.GetAt(i);
			if( !IsFilteredHj(hj.get()))
				continue;
			if( hj->kosd == jm3RD)
			{		
				m_shjCodes.Add(hj);
			}
		}
	}
	else if( ntype == 13 )	//ETF
	{
		for(i = 0; i < tempCodes.GetSize() ; i++)
		{
			auto hj = tempCodes.GetAt(i);
			if( !IsFilteredHj(hj.get()))
				continue;
			if( hj->ssgb == jmETF)
			{		
				m_shjCodes.Add(hj);
			}
		}
	}
	else if( ntype == 14 )	//ELW
	{
		for(i = 0; i < tempCodes.GetSize() ; i++)
		{
			auto hj = tempCodes.GetAt(i);
			if( !IsFilteredHj(hj.get()))
				continue;
			if( hj->ssgb == jmELW)
			{
				m_shjCodes.Add(hj);
			}
		}
	}
	else if ( ntype == 15)	// ELW기초자산
	{
		szName.Format("%s\\tab\\", Axis::home);
		LoadELWBase(szName + "elwcod.dat");
		for(i = 0; i < m_elwbase.GetSize() ; i++)
		{
			auto hj = m_elwbase.GetAt(i);
			if( !IsFilteredHj(hj.get()))
				continue;

			m_shjCodes.Add(hj);
		}
	}
	else if( ntype == 16 )	//코스닥 업종
	{
		for(i = 0; i < tempCodes.GetSize() ; i++)
		{
			auto hj = tempCodes.GetAt(i);
			if( !IsFilteredHj(hj.get()))
				continue;
			ListUpjongSort(LOWORD(m_pTree->GetItemData(hItem)),hj,1);
		}
	}
	
	tempCodes.RemoveAll();
	tempCodes.Copy(m_shjCodes);
	//Sorting
	int ncmpcnt =0;
	_mhjcode *hja = NULL,
			 *hjb = NULL;
	if( m_pComboOrder->GetCurSel() == 1 )
	{
		ncmpcnt = min(tempCodes.GetSize() - 1, MAX_LIST_SIZE-1 );
		for(int i = 0 ; i < ncmpcnt ; i++)
		{
			auto hja = tempCodes.GetAt(i);
			if( hja->kosd != jmKOSPI)
			{				
				for(int j = i+1 ; j < tempCodes.GetSize() ; j++)
				{				
					auto hjb = tempCodes.GetAt(j);
					if( hjb->kosd == jmKOSPI )
					{
						tempCodes.SetAt( i,hjb );
						tempCodes.SetAt( j,hja);
						break;
					}
				}
			}
			
		}
	}
	else if(m_pComboOrder->GetCurSel() == 2 )
	{
		ncmpcnt = min(tempCodes.GetSize() - 1, MAX_LIST_SIZE-1 );

		for(int i = 0 ; i < ncmpcnt ; i++)
		{
			for(int j = i+1 ; j < tempCodes.GetSize() ; j++)
			{
				auto hja = tempCodes.GetAt(i);
				auto hjb = tempCodes.GetAt(j);
				if( hjb->hnam.CompareNoCase(hja->hnam) < 0)
				{
					tempCodes.SetAt( i, hjb);
					tempCodes.SetAt( j,hja);
				}
			}
		}
	}else if(m_pComboOrder->GetCurSel() == 3 )
	{
		ncmpcnt = min(tempCodes.GetSize() - 1, MAX_LIST_SIZE-1 );
		for(int i = 0 ; i < ncmpcnt ; i++)
		{
			for(int j = i+1 ; j < tempCodes.GetSize() ; j++)
			{
				auto hja = tempCodes.GetAt(i);
				auto hjb = tempCodes.GetAt(j);
				if( hjb->hnam.CompareNoCase(hja->hnam) < 0)
				{
					tempCodes.SetAt( i,hjb );
					tempCodes.SetAt( j,hja);
				}
			}
		}
		for( i = 0 ; i < ncmpcnt ; i++)
		{
			auto hja = tempCodes.GetAt(i); 
			if( hja->kosd != jmKOSPI)
			{				
				for(int j = i+1 ; j < tempCodes.GetSize() ; j++)
				{
				
					auto hjb = tempCodes.GetAt(j);
					if( hjb->kosd == jmKOSPI )
					{
						tempCodes.SetAt( i,hjb );
						tempCodes.SetAt( j,hja);
						break;
					}
				}
			}
			
		}
	}

	m_shjCodes.RemoveAll();
	m_shjCodes.Copy(tempCodes);
	if( m_shjCodes.GetSize() > MAX_LIST_SIZE )
	{	
		const int rmvcnt = m_shjCodes.GetSize()- MAX_LIST_SIZE ;
		m_shjCodes.RemoveAt( MAX_LIST_SIZE ,rmvcnt );
	}
	
	m_pList->DeleteAllItems();
	m_sCodeList = "";

	if( m_shjCodes.GetSize() > 4)
	{
		const int col = 2;
		int	width[col]{};
		CRect rect;
		m_pList->GetClientRect(&rect);
		const int tot = (int)(rect.Width() / col);

		const int swidth = GetSystemMetrics(SM_CXHSCROLL);
		width[0] = tot - 10 - swidth;
		width[1] = tot + 10;
		m_pList->SetColumnWidth(0,width[0]);
		m_pList->SetColumnWidth(1,width[1]);
	}
	else
	{
		const int col = 2;
		int	width[col]{};
		CRect rect;
		m_pList->GetClientRect(&rect);
		const int tot = (int)(rect.Width() / col);

		const int swidth = GetSystemMetrics(SM_CXHSCROLL);
		width[0] = tot - 10 ;
		width[1] = tot + 10;
		m_pList->SetColumnWidth(0,width[0]);
		m_pList->SetColumnWidth(1,width[1]);
	}

	for( i = 0 ; i < m_shjCodes.GetSize() ; i ++)
	{
		if( i >= MAX_LIST_SIZE )
			break;

		auto hj = m_shjCodes.GetAt(i);
		m_pList->InsertItem(i,hj->code.Mid(1));
		m_pList->SetItem(i,1,LVIF_TEXT, hj->hnam,0,0,0,0);
		m_sCodeList += hj->code.Mid(1);
		m_sCodeList += '\t';
	}
}  
#else
void CCategoryPopup::SearchCode()
{
	loadHjCode();

	HTREEITEM hItem = m_pTree->GetSelectedItem();

	if (hItem == NULL)
		return;

	int i{}, j = 0;
	m_shjCodes.RemoveAll();
	CString szCode = "", szName = "", szName1 = "";

	// 분류별 로드
	_mhjcode* hj = NULL;
	CArray< _mhjcode*, _mhjcode*> tempCodes;
	CString hnama = "", hnamb = "";

	tempCodes.Copy(m_hjCodes);

	CStringArray* pArr = NULL;
	const int ntype = HIWORD(m_pTree->GetItemData(hItem));

	if (ntype == 0)//그룹사
	{
		pArr = m_codeArr.GetAt(LOWORD(m_pTree->GetItemData(hItem)));
		for (i = 0; i < pArr->GetSize(); i++)
		{
			szName1 = pArr->GetAt(i); szName1.TrimLeft(); szName1.TrimRight();
			szName1 = "A" + szName1;

			for (j = 0; j < tempCodes.GetSize(); j++)
			{
				hj = tempCodes.GetAt(j);
				if (!IsFilteredHj(hj))
					continue;
				if (hj->code == szName1)
				{
					m_shjCodes.Add(hj);
					break;
				}
			}

		}
	}
	else if (ntype == 6) //테마종목
	{
		pArr = m_codeArr.GetAt(LOWORD(m_pTree->GetItemData(hItem)));
		for (i = 0; i < pArr->GetSize(); i++)
		{

			for (j = 0; j < tempCodes.GetSize(); j++)
			{
				hj = tempCodes.GetAt(j);
				if (!IsFilteredHj(hj))
					continue;

				szName1 = pArr->GetAt(i); szName1.TrimLeft(); szName1.TrimRight();
				szName1 = "A" + szName1;
				if (hj->code == szName1)
				{
					m_shjCodes.Add(hj);
					break;
				}
			}

		}
	}
	else if (ntype == 2) //코스피 업종
	{
		for (i = 0; i < tempCodes.GetSize(); i++)
		{
			hj = tempCodes.GetAt(i);
			if (!IsFilteredHj(hj))
				continue;
			ListUpjongSort(LOWORD(m_pTree->GetItemData(hItem)), hj);
		}
	}
	else if (ntype == 3)
	{
		const int nGroup = LOWORD(m_pTree->GetItemData(hItem));
		CString tmp = loadCodeList(nGroup);
		CString code = "";
		while (!tmp.IsEmpty())
		{
			code = Parser(tmp, "\t"); code.TrimLeft(); code.TrimRight();

			for (j = 0; j < tempCodes.GetSize(); j++)
			{
				hj = tempCodes.GetAt(j);
				if (!IsFilteredHj(hj))
					continue;

				if (hj->code.Mid(1) == code)
				{
					m_shjCodes.Add(hj);
					break;
				}
			}
		}


	}
	else if (ntype == 4)	//히스토리
	{
		CString code = "";
		for (i = 0; i < m_history.GetSize(); i++)
		{

			code = m_history.GetAt(i);

			for (j = 0; j < tempCodes.GetSize(); j++)
			{

				hj = tempCodes.GetAt(j);

				if (hj->code.Mid(1) == code)
				{
					m_shjCodes.Add(hj);
					break;
				}
			}
		}

	}
	else if (ntype == 5) //전종목
	{
		for (i = 0; i < tempCodes.GetSize(); i++)
		{
			hj = tempCodes.GetAt(i);
			if (!IsFilteredHj(hj))
				continue;

			m_shjCodes.Add(hj);
		}
	}
	else if (ntype == 7)	//코스피 전종목
	{
		for (i = 0; i < tempCodes.GetSize(); i++)
		{
			hj = tempCodes.GetAt(i);
			if (!IsFilteredHj(hj))
				continue;
			if (hj->kosd == jmKOSPI)
			{

				m_shjCodes.Add(hj);
			}
		}
	}
	else if (ntype == 8)	//코스닥 전종목
	{
		for (i = 0; i < tempCodes.GetSize(); i++)
		{
			hj = tempCodes.GetAt(i);
			if (!IsFilteredHj(hj))
				continue;
			if (hj->kosd == jmKOSDAQ)
			{

				m_shjCodes.Add(hj);
			}
		}
	}
	else if (ntype == 9)	//KOSPI200
	{

		for (i = 0; i < tempCodes.GetSize(); i++)
		{
			hj = tempCodes.GetAt(i);
			if (!IsFilteredHj(hj))
				continue;

			if (hj->kosd == jmKOSPI)
			{
				if (hj->kpgb == 1 || hj->kpgb == 2 || hj->kpgb == 3)
					m_shjCodes.Add(hj);
			}
		}

	}
	else if (ntype == 10)	//KOSDAQ50
	{
		for (i = 0; i < tempCodes.GetSize(); i++)
		{
			hj = tempCodes.GetAt(i);
			if (!IsFilteredHj(hj))
				continue;
			if (hj->kosd == jmKOSDAQ && hj->kpgb == 1)
			{

				m_shjCodes.Add(hj);
			}
		}
	}
	else if (ntype == 11)	//KRX100
	{
		for (i = 0; i < tempCodes.GetSize(); i++)
		{
			hj = tempCodes.GetAt(i);
			if (!IsFilteredHj(hj))
				continue;
			if (hj->unio == 1)
			{
				m_shjCodes.Add(hj);
			}
		}

	}
	else if (ntype == 12)	//프리보드
	{
		for (i = 0; i < tempCodes.GetSize(); i++)
		{
			hj = tempCodes.GetAt(i);
			if (!IsFilteredHj(hj))
				continue;
			if (hj->kosd == jm3RD)
			{
				m_shjCodes.Add(hj);
			}
		}
	}
	else if (ntype == 13)	//ETF
	{
		for (i = 0; i < tempCodes.GetSize(); i++)
		{
			hj = tempCodes.GetAt(i);
			if (!IsFilteredHj(hj))
				continue;
			if (hj->ssgb == jmETF)
			{
				m_shjCodes.Add(hj);
			}
		}
	}
	else if (ntype == 14)	//ELW
	{
		for (i = 0; i < tempCodes.GetSize(); i++)
		{
			hj = tempCodes.GetAt(i);
			if (!IsFilteredHj(hj))
				continue;
			if (hj->ssgb == jmELW)
			{
				m_shjCodes.Add(hj);
			}
		}
	}
	else if (ntype == 15)	// ELW기초자산
	{
		szName.Format("%s\\tab\\", Axis::home);
		LoadELWBase(szName + "elwcod.dat");
		for (i = 0; i < m_elwbase.GetSize(); i++)
		{
			hj = m_elwbase.GetAt(i);
			if (!IsFilteredHj(hj))
				continue;

			m_shjCodes.Add(hj);
		}
	}
	else if (ntype == 16)	//코스닥 업종
	{
		for (i = 0; i < tempCodes.GetSize(); i++)
		{
			hj = tempCodes.GetAt(i);
			if (!IsFilteredHj(hj))
				continue;
			ListUpjongSort(LOWORD(m_pTree->GetItemData(hItem)), hj, 1);
		}
	}

	tempCodes.RemoveAll();
	tempCodes.Copy(m_shjCodes);
	//Sorting
	int ncmpcnt = 0;
	_mhjcode* hja = NULL,
		* hjb = NULL;
	if (m_pComboOrder->GetCurSel() == 1)
	{
		ncmpcnt = min(tempCodes.GetSize() - 1, MAX_LIST_SIZE - 1);
		for (int i = 0; i < ncmpcnt; i++)
		{
			hja = tempCodes.GetAt(i);
			if (hja->kosd != jmKOSPI)
			{
				for (int j = i + 1; j < tempCodes.GetSize(); j++)
				{
					hjb = tempCodes.GetAt(j);
					if (hjb->kosd == jmKOSPI)
					{
						tempCodes.SetAt(i, hjb);
						tempCodes.SetAt(j, hja);
						break;
					}
				}
			}

		}
	}
	else if (m_pComboOrder->GetCurSel() == 2)
	{
		ncmpcnt = min(tempCodes.GetSize() - 1, MAX_LIST_SIZE - 1);

		for (int i = 0; i < ncmpcnt; i++)
		{
			for (int j = i + 1; j < tempCodes.GetSize(); j++)
			{
				hja = tempCodes.GetAt(i);
				hjb = tempCodes.GetAt(j);
				if (hjb->hnam.CompareNoCase(hja->hnam) < 0)
				{
					tempCodes.SetAt(i, hjb);
					tempCodes.SetAt(j, hja);
				}
			}
		}
	}
	else if (m_pComboOrder->GetCurSel() == 3)
	{
		ncmpcnt = min(tempCodes.GetSize() - 1, MAX_LIST_SIZE - 1);
		for (int i = 0; i < ncmpcnt; i++)
		{
			for (int j = i + 1; j < tempCodes.GetSize(); j++)
			{
				hja = tempCodes.GetAt(i);
				hjb = tempCodes.GetAt(j);
				if (hjb->hnam.CompareNoCase(hja->hnam) < 0)
				{
					tempCodes.SetAt(i, hjb);
					tempCodes.SetAt(j, hja);
				}
			}
		}
		for (i = 0; i < ncmpcnt; i++)
		{
			hja = tempCodes.GetAt(i);
			if (hja->kosd != jmKOSPI)
			{
				for (int j = i + 1; j < tempCodes.GetSize(); j++)
				{

					hjb = tempCodes.GetAt(j);
					if (hjb->kosd == jmKOSPI)
					{
						tempCodes.SetAt(i, hjb);
						tempCodes.SetAt(j, hja);
						break;
					}
				}
			}

		}
	}

	m_shjCodes.RemoveAll();
	m_shjCodes.Copy(tempCodes);
	if (m_shjCodes.GetSize() > MAX_LIST_SIZE)
	{
		const int rmvcnt = m_shjCodes.GetSize() - MAX_LIST_SIZE;
		m_shjCodes.RemoveAt(MAX_LIST_SIZE, rmvcnt);
	}

	m_pList->DeleteAllItems();
	m_sCodeList = "";

	if (m_shjCodes.GetSize() > 4)
	{
		const int col = 2;
		int	width[col]{};
		CRect rect;
		m_pList->GetClientRect(&rect);
		const int tot = (int)(rect.Width() / col);

		const int swidth = GetSystemMetrics(SM_CXHSCROLL);
		width[0] = tot - 10 - swidth;
		width[1] = tot + 10;
		m_pList->SetColumnWidth(0, width[0]);
		m_pList->SetColumnWidth(1, width[1]);
	}
	else
	{
		const int col = 2;
		int	width[col]{};
		CRect rect;
		m_pList->GetClientRect(&rect);
		const int tot = (int)(rect.Width() / col);

		const int swidth = GetSystemMetrics(SM_CXHSCROLL);
		width[0] = tot - 10;
		width[1] = tot + 10;
		m_pList->SetColumnWidth(0, width[0]);
		m_pList->SetColumnWidth(1, width[1]);
	}

	for (i = 0; i < m_shjCodes.GetSize(); i++)
	{
		if (i >= MAX_LIST_SIZE)
			break;

		hj = m_shjCodes.GetAt(i);
		m_pList->InsertItem(i, hj->code.Mid(1));
		m_pList->SetItem(i, 1, LVIF_TEXT, hj->hnam, 0, 0, 0, 0);
		m_sCodeList += hj->code.Mid(1);
		m_sCodeList += '\t';
	}
}

#endif

#ifdef DF_USESTL_CATEGORY
void CCategoryPopup::ListUpjongSort(int nidx, std::shared_ptr<_mhjcode>  hjc, int ntype){
#else
void CCategoryPopup::ListUpjongSort(int nidx,_mhjcode* hjc,int ntype){
#endif
	//코스피
	if (ntype == 0 && hjc->kosd == jmKOSPI)
	{
		if (hjc->ssgb == jmSINJU || hjc->ssgb == jmSINJS || hjc->ssgb == jmHYFND)	// 신주인수권 수익증권 제외
			return;

		if (nidx == 1)
		{
			m_shjCodes.Add(hjc);
		}
		else if (nidx < 5)
		{
			// 2 : 대형주, 3 : 중형주, 4 : 소형주
			if (hjc->size == nidx)
				m_shjCodes.Add(hjc);
		}
		else if (nidx < 27)
		{
			CString	ucdm = acd_KOSPI[nidx];
			if (!ucdm.CompareNoCase(hjc->acdm) || !ucdm.CompareNoCase(hjc->acds))
				m_shjCodes.Add(hjc);
		}
		else if (nidx == 27)	// 제조업
		{
			if (hjc->jjug == nidx)
				m_shjCodes.Add(hjc);
		}
		else if (nidx < 41)
		{
			if (hjc->jsiz == nidx)
				m_shjCodes.Add(hjc);
		}
		else if (nidx == 41)	// 배당지수
		{
			if (hjc->jsiz >= 38 + nidx && hjc->jsiz <= 41 + nidx)
				m_shjCodes.Add(hjc);
		}
		else if (nidx == 42)	// 지배구조우수
		{
			if (hjc->wsgb == 1)
				m_shjCodes.Add(hjc);
		}
		else if (nidx == 101)	// 코스피 200
		{
			if (hjc->kpgb == 1 || hjc->kpgb == 2 || hjc->kpgb == 3)
				m_shjCodes.Add(hjc);
		}
		else if (nidx == 401)	// 코스피 100
		{
			if (hjc->kpgb == 3 || hjc->kpgb == 2)
				m_shjCodes.Add(hjc);
		}
		else if (nidx == 402)	// 코스피 50
		{
			if (hjc->kpgb == 3)
				m_shjCodes.Add(hjc);
		}
		else if (nidx == 403)	// 코스피 IT
		{
			if (hjc->itgb == 1)
				m_shjCodes.Add(hjc);
		}
	}
	else if (ntype == 1 && hjc->kosd == jmKOSDAQ) //코스닥
	{
		nidx = nidx % 100;
		if (nidx == 1)
		{
			m_shjCodes.Add(hjc);
		}
		else if (nidx == 2)	// 벤처지수
		{
			if (hjc->jjug == 2)
				m_shjCodes.Add(hjc);
		}
		else if (nidx < 5)
		{
			// 3 : 기타 서비스, 4 : 코스닥 IT 종합
			if (hjc->size == nidx)
				m_shjCodes.Add(hjc);
		}
		else if (nidx == 5)	// 코스닥 IT 벤처
		{
			if (hjc->size == 4 && hjc->jjug == 2)
				m_shjCodes.Add(hjc);
		}
		else if (nidx < 38 || nidx == 41)
		{
			CString	ucdm = acd_KOSDAQ[nidx];
			if (!ucdm.CompareNoCase(hjc->acdm) || !ucdm.CompareNoCase(hjc->acds))
				m_shjCodes.Add(hjc);
		}
		else if (nidx < 41)
		{
			// 38 : KOSDAQ 100, 39 : KOSDAQ MID 300, 40 : KOSDAQ SMALL
			if (hjc->jsiz == nidx - 37)
				m_shjCodes.Add(hjc);
		}
		else if (nidx == 42)	// KOSDAQ 우량기업부
		{
			if (hjc->kpgb == 1)
				m_shjCodes.Add(hjc);
		}
		else if (nidx > 42 && nidx <= 45)
		{
			CString ucdm = acd_KOSDAQ[nidx];
			if (!ucdm.CompareNoCase(hjc->acdl))
				m_shjCodes.Add(hjc);
		}
	}
}

BOOL CCategoryPopup::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CCategoryPopup::LoadFolder(CString path, HTREEITEM hItem, CString sSection, int nKey)
{
	char	wb[2048]{};
	CString	tmps;
#ifdef DF_USESTL_CATEGORY
	std::shared_ptr<CStringArray> pStrArr;
#else
	CStringArray* pStrArr{};
#endif
	for (int ii = 0; ii < 100; ii++)
	{
		HTREEITEM	hItem1{};
		tmps.Format("%02d", ii);
		GetPrivateProfileString("FOLDER", tmps, "", wb, sizeof(wb), path);
		tmps = wb;
		
		if (tmps.IsEmpty())
			break;
		
		hItem1 = m_pTree->InsertItem(Parser(tmps, ";"), 0, 1, hItem);
#ifdef DF_USESTL_CATEGORY		
		pStrArr = std::make_shared<CStringArray>();
#else
		pStrArr = new CStringArray();	
#endif

		//tmps.Format("%02d", ii);
		GetPrivateProfileString(sSection, tmps, "", wb, sizeof(wb), path);
		tmps = wb;

		if (tmps.IsEmpty())
		{
			m_pTree->DeleteItem(hItem1);
#ifndef DF_USESTL_CATEGORY			
			delete pStrArr;
#endif
			continue;
		}

		while(!tmps.IsEmpty())
		{
			CString sCode = Parser(tmps, ";");

			if (!sCode.IsEmpty())
				pStrArr->Add(sCode);

			if (tmps.IsEmpty())
			{
				m_pTree->SetItemData(hItem1, MAKELONG(m_codeArr.Add(pStrArr), nKey));
				break;
			}
		}
	}
}

void CCategoryPopup::LoadELWBase(CString path)
{
	if (m_elwbase.GetSize() > 0)
		return;

	int	codeN{};
	CFile	file;
	struct  elwcode  ELWCode {};

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))	return;

	codeN = gsl::narrow_cast<int>(file.GetLength() / sizeof(struct elwcode));

	bool	bFound{};
	CArray	<CString , CString>	tmpBase;
	tmpBase.RemoveAll();
	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&ELWCode, sizeof(struct elwcode));

		for (int jj = 0; jj < 5; jj ++)
		{
			CString bcode = CString((char*)ELWCode.gcjs[jj], 12);
			bcode.TrimRight();
			if (bcode == "") break;

			bFound = false;
			for (int kk = 0; kk < tmpBase.GetSize(); kk++)
			{
				if (bcode.CompareNoCase(tmpBase.GetAt(kk)) == 0)
				{
					bFound = true;
					break;
				}
			}

			if (!bFound)
			{
				if (bcode.CompareNoCase("KOSPI200") == 0)
					continue;
				else if (bcode.CompareNoCase("BASKET") == 0)
					break;

				tmpBase.Add(bcode);
			}
		}
	}

	file.Close();
#ifndef DF_USESTL_CATEGORY
	_mhjcode* mhjcode{};
#endif
	for (int ii = 0; ii < tmpBase.GetSize(); ii++)
	{
#ifdef DF_USESTL_CATEGORY		
		auto mhjcode = FindCode(tmpBase.GetAt(ii));
#else
		mhjcode = FindCode(tmpBase.GetAt(ii));
#endif

		if (mhjcode)	m_elwbase.Add(mhjcode);
	}

	tmpBase.RemoveAll();
}

#ifdef DF_USESTL_CATEGORY
	std::shared_ptr<_mhjcode> CCategoryPopup::FindCode(CString sCode)
{
	//_mhjcode* mhjcode{};
	//CArray< _mhjcode* , _mhjcode*> tempCodes ;
	CArray<  std::shared_ptr< _mhjcode>, std::shared_ptr< _mhjcode>> tempCodes;
	tempCodes.Copy( m_hjCodes );
	for (int ii = 0; ii < tempCodes.GetSize(); ii++)
	{
		auto mhjcode = tempCodes.GetAt(ii);
		if (mhjcode->code == sCode)
			return mhjcode;
	}

	return NULL;
}
#else
	_mhjcode* CCategoryPopup::FindCode(CString sCode)
{
	_mhjcode* mhjcode{};
	CArray< _mhjcode* , _mhjcode*> tempCodes ;
	tempCodes.Copy( m_hjCodes );
	for (int ii = 0; ii < tempCodes.GetSize(); ii++)
	{
		mhjcode = tempCodes.GetAt(ii);
		if (mhjcode->code == sCode)
			return mhjcode;
	}

	return NULL;
}
#endif

void CCategoryPopup::setParent(CWnd *pWnd)
{
	m_pParent = pWnd;
}

void CCategoryPopup::OnTreeDblClk(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM hItem = m_pTree->GetSelectedItem();
	// 분류별 로드
	const int nType =HIWORD(m_pTree->GetItemData(hItem));
	if( nType < 0 || nType == 0xffff)
		return;

	SearchCode();	
	CString str;
	str.Format("%d개 조회",m_pList->GetItemCount());
	m_pQCStatic->SetWindowText(str);
	m_pQCStatic->Invalidate();
}

CString CCategoryPopup::Parser(CString &srcstr, CString substr)
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

void CCategoryPopup::OnDestroy() 
{
	CWnd::OnDestroy();
	
	if (m_pTree)
	{
		m_pTree->DestroyWindow();
#ifndef DF_USESTL_CATEGORY		
		delete m_pTree;
#endif
	}

}

void CCategoryPopup::loadUCode(CString path, HTREEITEM	hItem)
{
	char	buf[256]{};

	int	res{};
	DWORD	dw{};
	int	bufsize{};
	struct	upcode* pUpCode{};

	HANDLE fh = CreateFile(path.operator LPCTSTR(), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("File not found.");
		CloseHandle(fh);
		return ;
	}
	HTREEITEM	hItem1{}, hItem2{};
	hItem1 = m_pTree->InsertItem("KOSPI", 0, 1, hItem);
	m_pTree->SetItemData(hItem1, MAKELONG(0, -1));
	hItem2 = m_pTree->InsertItem("KOSDAQ", 0, 1, hItem);
	m_pTree->SetItemData(hItem2, MAKELONG(0, -1));

	bufsize = sizeof(struct upcode);
	
	for (int ii = 0; ; ii++)
	{
		CString		name, dta;

		res = ReadFile(fh, buf, bufsize, &dw, NULL);

		if (bufsize != (int)dw)
			break;

		pUpCode = (struct upcode *)buf;

		name = CString(pUpCode->hnam, UNameLen);
		res = name.Find('\0');
		if (res != -1)
			name = name.Left(res);

		name.TrimRight();

		dta.Format("%d%02d", (int)pUpCode->jgub, (int)pUpCode->ucod);

		if (pUpCode->jgub == upKOSPI || pUpCode->jgub == upINTEGRATE ||
			(pUpCode->jgub == upKOSPI200 && pUpCode->ucod == 1) || 
			pUpCode->jgub == upKOSPIx)
			m_pTree->SetItemData(m_pTree->InsertItem(name, 0, 1, hItem1), MAKELONG(atoi(dta), 2));
		else if (pUpCode->jgub == upKOSDAQ || 
			pUpCode->jgub == upKOSDAQ50)
			m_pTree->SetItemData(m_pTree->InsertItem(name, 0, 1, hItem2), MAKELONG(atoi(dta), 16));
	}
	CloseHandle(fh);
}

void CCategoryPopup::InsertCodeInGroup(CString sRet)
{
#ifdef DF_USESTL_CATEGORY
	m_shjCodes.RemoveAll();
	CString code = "", tmp;
	tmp = sRet;
	const int isize = m_hjCodes.GetSize();
	CArray< std::shared_ptr<_mhjcode>, std::shared_ptr<_mhjcode>> tempCodes;
	tempCodes.Copy(m_hjCodes);
	while (!tmp.IsEmpty())
	{
		code = Parser(tmp, "\t"); code.TrimLeft(); code.TrimRight();

		for (int ii = 0; ii < tempCodes.GetSize(); ii++)
		{
			auto hj = tempCodes.GetAt(ii);
			if (!IsFilteredHj(hj.get()))
				continue;

			if (hj->code.Mid(1) == code)
			{
				m_shjCodes.Add(hj);
				break;
			}
		}
	}



	tempCodes.RemoveAll();
	tempCodes.Copy(m_shjCodes);
	//Sorting
	int ncmpcnt = 0;
	_mhjcode* hja = NULL,
		* hjb = NULL;
	if (m_pComboOrder->GetCurSel() == 1)
	{
		ncmpcnt = min(tempCodes.GetSize() - 1, MAX_LIST_SIZE - 1);
		for (int i = 0; i < ncmpcnt; i++)
		{
			auto hja = tempCodes.GetAt(i);
			if (hja->kosd != jmKOSPI)
			{
				for (int j = i + 1; j < tempCodes.GetSize(); j++)
				{
					auto hjb = tempCodes.GetAt(j);
					if (hjb->kosd == jmKOSPI)
					{
						tempCodes.SetAt(i, hjb);
						tempCodes.SetAt(j, hja);
						break;
					}
				}
			}

		}
	}
	else if (m_pComboOrder->GetCurSel() == 2)
	{
		ncmpcnt = min(tempCodes.GetSize() - 1, MAX_LIST_SIZE - 1);

		for (int i = 0; i < ncmpcnt; i++)
		{
			for (int j = i + 1; j < tempCodes.GetSize(); j++)
			{
				auto hja = tempCodes.GetAt(i);
				auto hjb = tempCodes.GetAt(j);
				if (hjb->hnam.CompareNoCase(hja->hnam) < 0)
				{
					tempCodes.SetAt(i, hjb);
					tempCodes.SetAt(j, hja);
				}
			}
		}
	}
	else if (m_pComboOrder->GetCurSel() == 3)
	{
		ncmpcnt = min(tempCodes.GetSize() - 1, MAX_LIST_SIZE - 1);
		for (int i = 0; i < ncmpcnt; i++)
		{
			for (int j = i + 1; j < tempCodes.GetSize(); j++)
			{
				auto hja = tempCodes.GetAt(i);
				auto hjb = tempCodes.GetAt(j);
				if (hjb->hnam.CompareNoCase(hja->hnam) < 0)
				{
					tempCodes.SetAt(i, hjb);
					tempCodes.SetAt(j, hja);
				}
			}
		}
		for (int i = 0; i < ncmpcnt; i++)
		{
			auto hja = tempCodes.GetAt(i);
			if (hja->kosd != jmKOSPI)
			{
				for (int j = i + 1; j < tempCodes.GetSize(); j++)
				{

					auto hjb = tempCodes.GetAt(j);
					if (hjb->kosd == jmKOSPI)
					{
						tempCodes.SetAt(i, hjb);
						tempCodes.SetAt(j, hja);
						break;
					}
				}
			}

		}
	}

	m_shjCodes.RemoveAll();
	m_shjCodes.Copy(tempCodes);
	if (m_shjCodes.GetSize() > MAX_LIST_SIZE)
	{
		const int rmvcnt = m_shjCodes.GetSize() - MAX_LIST_SIZE;
		m_shjCodes.RemoveAt(MAX_LIST_SIZE, rmvcnt);
	}

	m_pList->DeleteAllItems();
	m_sCodeList = "";

	if (m_shjCodes.GetSize() > 4)
	{
		const int col = 2;
		int	width[col]{};
		CRect rect;
		m_pList->GetClientRect(&rect);
		const int tot = (int)(rect.Width() / col);

		const int swidth = GetSystemMetrics(SM_CXHSCROLL);
		width[0] = tot - 10 - swidth;
		width[1] = tot + 10;
		m_pList->SetColumnWidth(0, width[0]);
		m_pList->SetColumnWidth(1, width[1]);
	}
	else
	{
		const int col = 2;
		int	width[col]{};
		CRect rect;
		m_pList->GetClientRect(&rect);
		const int tot = (int)(rect.Width() / col);

		const int swidth = GetSystemMetrics(SM_CXHSCROLL);
		width[0] = tot - 10;
		width[1] = tot + 10;
		m_pList->SetColumnWidth(0, width[0]);
		m_pList->SetColumnWidth(1, width[1]);
	}

	for (int i = 0; i < m_shjCodes.GetSize(); i++)
	{
		if (i >= MAX_LIST_SIZE)
			break;

		auto hj = m_shjCodes.GetAt(i);
		m_pList->InsertItem(i, hj->code.Mid(1));
		m_pList->SetItem(i, 1, LVIF_TEXT, hj->hnam, 0, 0, 0, 0);
		m_sCodeList += hj->code.Mid(1);
		m_sCodeList += '\t';
	}

	CString str;
	str.Format("%d개 조회", m_pList->GetItemCount());
	m_pQCStatic->SetWindowText(str);
	m_pQCStatic->Invalidate();
#endif
}

void CCategoryPopup::InsertGroupList(CString sRet)
{
	CString stmp;
	int ii = 0;
	while (!sRet.IsEmpty())
	{
		stmp = Parser(sRet, "\n");
		HTREEITEM hItem1;
		stmp.TrimRight();
		hItem1 = m_pTree->InsertItem(stmp, 0, 1, m_InterITEM);
		m_pTree->SetItemData(hItem1, MAKELONG(ii, 3));
		ii++;
	}

}

void CCategoryPopup::LoadGroup(CString path, HTREEITEM hItem)
{
	if ((m_InterITEM != NULL) && m_pTree->ItemHasChildren(m_InterITEM))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = m_pTree->GetChildItem(m_InterITEM);
		while (hChildItem != nullptr)
		{
			hNextItem = m_pTree->GetNextItem(hChildItem, TVGN_NEXT);
			m_pTree->DeleteItem(hChildItem);
			hChildItem = hNextItem;
		}
	}
	m_parent->SendMessage(WM_XMSG, MAKEWPARAM(1006, 0), 0);
	//char twb[1024]{};
	//CString sTmp;

	//for (int ii = 0; ii < 100; ii++)
	//{
	//	sTmp.Format("%02d", ii);
	//	GetPrivateProfileString("GROUPNAME", sTmp, "", twb, sizeof(twb), path);
	//	sTmp = twb;

	//	if (!sTmp.IsEmpty())
	//	{
	//		HTREEITEM hItem1;
	//		hItem1 = m_pTree->InsertItem(sTmp, 0, 1, hItem);
	//		m_pTree->SetItemData(hItem1, MAKELONG(ii, 3));
	//	}
	//}
}


void CCategoryPopup::OnSizing(UINT fwSide, LPRECT pRect) 
{/*
	
//	if( fwSide == )
	CRect rc;
	m_pMainWnd->GetWindowRect(&rc);
	if( m_pMainWnd->m_bShowPlay )
		rc.SetRect(rc.left  , rc.bottom  , rc.left + PT_CX, rc.bottom + PT_CY);
	else
		rc.SetRect(rc.left  , rc.bottom  , rc.left + PT_CX, rc.bottom + PT_CX-30);

	pRect->left = rc.left;
	pRect->top = rc.top;
	pRect->right = rc.right;
	pRect->bottom = rc.bottom;
		
	CWnd::OnSizing(fwSide, pRect);
*/	
}

void CCategoryPopup::loadRank(HTREEITEM hItem)
{
	struct _rankStruct
	{
		char *name;
		long value;
	}const  rankStruct[] = {
		{"상승률상위(거래소)",			111},
		{"상승률상위(코스닥)",			112},
		{"하락율상위(거래소)",			121},
		{"하락율상위(코스닥)",			122},
		{"보통주/우선주이격율(거래소)",		131},
		{"보통주/우선주이격율(코스닥)",		132},
		{"거래량상위(거래소)",			141},
		{"거래량상위(코스닥)",			142},
		{"거래대금상위(거래소)",		151},
		{"거래대금상위(코스닥)",		152},
		{"시가총액순위(거래소)",		161},
		{"시가총액순위(코스닥)",		162},
		{"상한가종목(거래소)",			11},
		{"상한가종목(코스닥)",			12},
		{"상승종목(거래소)",			21},
		{"상승종목(코스닥)",			22},
		{"보합종목(거래소)",			31},
		{"보합종목(코스닥)",			32},
		{"하락종목(거래소)",			51},
		{"하락종목(코스닥)",			52},
		{"하한가종목(거래소)",			41},
		{"하한가종목(코스닥)",			42},
		{"52주고가종목(거래소)",		171},
		{"52주고가종목(코스닥)",		172},
		{"52주저가종목(거래소)",		181},
		{"52주저가종목(코스닥)",		182},
		{"매수체결비율상위(거래소)",		211},
		{"매수체결비율상위(코스닥)",		212},
		{"매도체결비율상위(거래소)",		221},
		{"매도체결비율상위(코스닥)",		222},
		{"5일-20일 골든크로스종목(거래소)",	311},
		{"5일-20일 골든크로스종목(코스닥)",	312},
		{"5일-60일 골든크로스종목(거래소)",	321},
		{"5일-60일 골든크로스종목(코스닥)",	322},
		{"20일-60일 골든크로스종목(거래소)",	331},
		{"20일-60일 골든크로스종목(코스닥)",	332},
		{"5일-20일 데드크로스종목(거래소)",	351},
		{"5일-20일 데드크로스종목(코스닥)",	352},
		{"5일-60일 데드크로스종목(거래소)",	361},
		{"5일-60일 데드크로스종목(코스닥)",	362},
		{"20일-60일 데드크로스종목(거래소)",	371},
		{"20일-60일 데드크로스종목(코스닥)",	372}
	};
	
 
	for (int ii = 0; ii < (sizeof(rankStruct) / sizeof(_rankStruct)); ii++)
	{
		m_pTree->SetItemData(
			m_pTree->InsertItem(LoadStringTb(IDS_RISERATETOP_KP + ii), 0, 1, hItem), 
			MAKELONG(rankStruct[ii].value, 1));
	}
}

CString CCategoryPopup::LoadStringTb(UINT nIdx)
{
	CString str;
	str.LoadString(nIdx);
	return str;
}

void CCategoryPopup::OnChecked(UINT nID)
{
	if( nID>= IDC_CHECK_BGJ && nID <= IDC_CHECK_KOP){
		const int nCheck = ((CButton*)GetDlgItem(nID))->GetCheck();
		m_bCateChecked[nID - IDC_CHECK_BGJ] = (bool)nCheck;
	}
}

void CCategoryPopup::OnPlayClk(){

	const int nCnt = min(m_shjCodes.GetSize() , MAX_LIST_SIZE );
	CString data;
	data.Empty();
	
	for( int i = 0 ; i < nCnt ; i++ ){
#ifdef DF_USESTL_CATEGORY
		const auto hj = m_shjCodes.GetAt(i);
#else
		const _mhjcode *hj = m_shjCodes.GetAt(i);	
#endif
		data+=hj->code.Mid(1);
		data+="\t";
		data+=hj->hnam;
		data+="\n";
	}
	
	m_parent->SendMessage(WM_XMSG, MAKEWPARAM(1005, 0), (LPARAM)(LPCTSTR)data);
	ShowWindow(SW_HIDE);
	/**
	if( nCnt > 0)
		m_pMainWnd->m_pParent->SendMessage(WM_USER , MAKEWPARAM(autoDLL, nCnt ), (LPARAM)(LPCTSTR)data );
	**/

}

void CCategoryPopup::OnCloseClk()
{
	ShowWindow(SW_HIDE);
}

void CCategoryPopup::OnSearchClk(){
	SearchCode();
	CString str;
	str.Format("%d개 조회",m_pList->GetItemCount());
	m_pQCStatic->SetWindowText(str);
	m_pQCStatic->Invalidate();
	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString getMidString(char *pStr, int nFirst, int nLen)
{
	if ((int)strlen(pStr + nFirst) < nLen)
		nLen = strlen(pStr + nFirst);
	return CString(pStr + nFirst, nLen);
}

CString CCategoryPopup::loadCodeList(int nGroup)
{
	CString sgroup;
	sgroup.Format("%02d", nGroup + 1);

	m_parent->SendMessage(WM_XMSG, MAKEWPARAM(1007, 0),  (LPARAM)(LPSTR)(LPCTSTR)sgroup);
	return "";
	/*const int	cnt = 0;
	struct _inters	inter;

	CString dir, sRtn;
	dir.Format("%s\\user\\%s\\portfolio.i%02d", Axis::home, Axis::user, nGroup);
	TRY
	{
		CFile file(dir, CFile::modeRead);
		UINT nBytesRead;
		do
		{ 
			nBytesRead = file.Read(&inter, sizeof(_inters));
			if (nBytesRead == sizeof(inter))
			{
				CString sCode = getMidString(inter.code, 0, 12);
				sCode.TrimLeft();
				if (!sCode.IsEmpty())
				{
					sRtn += sCode;
					sRtn += '\t';
				}
			}
		}while((int)nBytesRead);
		file.Close();
		return sRtn;
	}CATCH(CFileException, e)
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
		return "";
	}
	END_CATCH
		return "";*/
}

void CCategoryPopup::loadHjCode()
{
	if (m_hjCodes.GetSize() > 0)
		return;

	int	codeN{};
	CFile	file;
	CString path; 

	path = Axis::home + "\\tab\\hjcode3.dat";
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
		return;

	codeN =gsl::narrow_cast<int>( file.GetLength() / sizeof(struct hjcodex));

	hjcodex hj{};
	for (int ii = 0; ii < codeN; ii++)
	{
#ifdef DF_USESTL_CATEGORY
		std::shared_ptr<_mhjcode> mhj = std::make_shared<_mhjcode>();
#else
		_mhjcode *mhj = new _mhjcode;
#endif
//		memset(mhj,0x00, sizeof(_mhjcode));
		if( file.Read(&hj, sizeof(hjcodex)) != sizeof(hjcodex)){
			TRACE("파일읽기 오류\n");
			break;
		}
		
		mhj->code = CString(hj.code,HCodeLen);mhj->code.TrimLeft();mhj->code.TrimRight();mhj->code.Remove(0);
		mhj->hnam = CString(hj.hnam,HNameLen);mhj->hnam.TrimLeft();mhj->hnam.TrimRight();mhj->code.Remove(0);
		mhj->enam = CString(hj.enam,ENameLen);mhj->enam.TrimLeft();mhj->enam.TrimRight();mhj->code.Remove(0);
		mhj->symb = CString(hj.symb,HSymbolLen);mhj->symb.TrimLeft();mhj->symb.TrimRight();mhj->code.Remove(0);
		mhj->acdl = CString(hj.acdl, 6); mhj->acdl.TrimLeft(); mhj->acdl.TrimRight();
		mhj->acdm = CString(hj.acdm, 6); mhj->acdm.TrimLeft(); mhj->acdm.TrimRight();
		mhj->acds = CString(hj.acds, 6); mhj->acds.TrimLeft(); mhj->acds.TrimRight();

		mhj->ecng = hj.ecng;		
		mhj->jsiz = hj.jsiz;	
		mhj->size = hj.size;
		mhj->jjug = hj.jjug;
		mhj->kpgb = hj.kpgb;
		mhj->kosd = hj.kosd;
		mhj->ssgb = hj.ssgb;
		mhj->ucmd = hj.ucmd;
		mhj->itgb = hj.itgb;
		mhj->wsgb = hj.wsgb;
		mhj->jqty = hj.jqty;
		mhj->star = hj.star;
		mhj->unio = hj.unio;
		mhj->jchk = hj.jchk;
		//memcpy( &mhj->size ,&hj.size , 16 );
	
		m_hjCodes.Add(mhj);
		m_codeMap.SetAt(mhj->code, mhj->hnam);

		if( (mhj->jsiz >= 38 + 41 && mhj->jsiz <= 41 + 41)){
			TRACE("배당지수 %d 있음",mhj->jsiz);
		}
	 }
#ifndef DF_USESTL_CATEGORY
		_mhjcode *hja = NULL,
			 *hjb = NULL;
#endif
	for(int i = 0 ; i < m_hjCodes.GetSize() - 1 ; i++){
		for(int j = i+1 ; j < m_hjCodes.GetSize() ; j++){
#ifdef DF_USESTL_CATEGORY
			auto hja = m_hjCodes.GetAt(i);
			auto hjb = m_hjCodes.GetAt(j);
#else
			hja = m_hjCodes.GetAt(i);
			hjb = m_hjCodes.GetAt(j);
#endif
			if( hjb->code.CompareNoCase(hja->code) < 0){
				m_hjCodes.SetAt( i,hjb );
				m_hjCodes.SetAt( j,hja);
			}
		}
	}

	file.Close();
}

void CCategoryPopup::Initialize()
{
	
}

