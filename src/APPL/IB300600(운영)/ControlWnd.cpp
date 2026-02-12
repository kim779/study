// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB300600.h"
#include "ControlWnd.h"
#include "MarketConfirm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CControlWnd::CControlWnd(CWnd *pParent, AccountMap *pAccount, SiseData *pSiseData)
: CAxisExt(pParent), m_pAccount(pAccount), m_pSiseData(pSiseData)
{
	CString path;
	path.Format("%s\\%s\\", Variant(homeCC,""), "image");

	m_hBtn2[0] = GetAxBitmap(path + "2btn.bmp");
	m_hBtn2[1] = GetAxBitmap(path + "2btn_dn_DLL.bmp");	//2015.11.26 KSJ 색상 변경
	m_hBtn2[2] = GetAxBitmap(path + "2btn_en.bmp");

	m_hBtn3[0] = GetAxBitmap(path + "3btn.bmp");
	m_hBtn3[1] = GetAxBitmap(path + "3btn_dn.bmp");
	m_hBtn3[2] = GetAxBitmap(path + "3btn_en.bmp");

	m_hBtnLink[0] = GetAxBitmap(path + "LINK.bmp");
	m_hBtnLink[1] = GetAxBitmap(path + "LINK_dn.bmp");
	m_hBtnLink[2] = GetAxBitmap(path + "LINK_en.bmp");

	m_hBtnLong[0] = GetAxBitmap(path + "6BTN.bmp");
	m_hBtnLong[1] = GetAxBitmap(path + "6BTN_DN_DLL.bmp");	//2015.11.26 KSJ 색상 변경
	m_hBtnLong[2] = GetAxBitmap(path + "6BTN_en.bmp");

	m_pJcntGrid = m_pOptGrid = m_pJvolGrid = m_pChegGrid = NULL;
	m_pBtnCheg = m_pBtnChart = m_pBtnJango = m_pBtnFold = m_pBtnJvol = m_pBtnChegLink = NULL;
	m_pStJumun = m_pStJcnt = m_pStRsvd = m_pStMstr = m_pStJvol = NULL;
//	m_pSpJcnt = m_pSpMulti = m_pSpRsvd = NULL;
	m_pSpMulti = m_pSpRsvd = NULL;
	m_pSpJcnt = NULL;
	m_pEdJprc = NULL;
	m_pTab = NULL;

	m_dtOptionIdx = -1;

	nSizeMode = SIZE_GENERAL;

	m_profile.Format(CONFIG_INI, Variant(homeCC,""), Variant(nameCC,""));

	m_bChegUpdate = FALSE;

	m_bMarketConfirm = TRUE;

	m_bFocusCheg = TRUE;

	m_pScreenWnd = NULL;

	m_bChegView = FALSE;

	m_bKind = FALSE;

	m_nCurrTab = 0;

	m_nCurFSelect = 3;	//2015.12.01 KSJ 

	m_nCurOSelect = 3;	

	m_hBodyBrush = (HBRUSH)CreateSolidBrush(m_crBodyBk);

	m_iKind = MINIBTN;

	m_bClickInWeek = false;
}

CControlWnd::~CControlWnd()
{
	DeleteObject(m_hBodyBrush);
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHK_AUTO, OnChkAuto)
	ON_BN_CLICKED(IDC_CHK_LOSS, OnChkLoss)
	ON_BN_CLICKED(IDC_BTN_SIZE, OnBtnSize)
	ON_BN_CLICKED(IDC_BTN_CHEG , OnBtnCheg )
	ON_BN_CLICKED(IDC_BTN_CHART, OnBtnChart)
	ON_BN_CLICKED(IDC_BTN_JANGO, OnBtnJango)
	ON_BN_CLICKED(IDC_CHK_MULTI, OnChkMulti)
	ON_BN_CLICKED(IDC_BTN_FOLD,  OnBtnFold)
	ON_BN_CLICKED(IDC_CHK_OCLICK, OnChkOneClick)
	ON_BN_CLICKED(IDC_BTN_JVOL, OnBtnJvol)
	ON_BN_CLICKED(IDC_BTN_CHEGLINK, OnChegLink)
	ON_BN_CLICKED(IDC_CHK_MARKET, OnChkMarket)
	ON_BN_CLICKED(IDC_BTN_GEN, OnBtnGen)  
	ON_BN_CLICKED(IDC_BTN_MINI, OnBtnMini)
	ON_BN_CLICKED(IDC_BTN_WEEK, OnBtnWeek)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_FUT1, IDC_BTN_FUT5, OnFutBtnClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_OPT1, IDC_BTN_OPT5, OnPutBtnClick)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CTL_TAB1, OnTabChange)
	ON_MESSAGE(WM_APP_REFRESH, OnRefresh)
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers

void CControlWnd::OnDestroy() 
{
	if (m_pBtnChart) { m_pBtnChart->DestroyWindow(); delete m_pBtnChart; }
	if (m_pBtnCheg)  { m_pBtnCheg->DestroyWindow();  delete m_pBtnCheg;  }
	if (m_pBtnJango) { m_pBtnJango->DestroyWindow(); delete m_pBtnJango; }
	if (m_pStJumun)  { m_pStJumun->DestroyWindow();  delete m_pStJumun;  }
	if (m_pStJvol)   { m_pStJvol->DestroyWindow();   delete m_pStJvol;   }
	if (m_pEdJprc)   { m_pEdJprc->DestroyWindow();   delete m_pEdJprc;   }
	if (m_pJvolGrid) { m_pJvolGrid->DestroyWindow(); delete m_pJvolGrid; }
	if (m_pBtnJvol)  { m_pBtnJvol->DestroyWindow();  delete m_pBtnJvol;  }
	if (m_pBtnChegLink) { m_pBtnChegLink->DestroyWindow(); delete m_pBtnChegLink; }

	for(int n=0; n<4; ++n)
	{
		if (m_pBtnFuture[n]) { m_pBtnFuture[n]->DestroyWindow(); delete m_pBtnFuture[n]; }
		if (m_pBtnOption[n]) { m_pBtnOption[n]->DestroyWindow(); delete m_pBtnOption[n]; }
		if (m_pStFuture[n]) { m_pStFuture[n]->DestroyWindow(); delete m_pStFuture[n]; }
	}
	if (m_pStJcnt)  { m_pStJcnt->DestroyWindow();  delete m_pStJcnt;  }
	if (m_pStRsvd)  { m_pStRsvd->DestroyWindow();  delete m_pStRsvd;  }
	if (m_pStMstr)  { m_pStMstr->DestroyWindow();  delete m_pStMstr;  }
	if (m_pSpJcnt)  { m_pSpJcnt->DestroyWindow();  delete m_pSpJcnt;  }
	if (m_pSpMulti) { m_pSpMulti->DestroyWindow(); delete m_pSpMulti; }
	if (m_pSpRsvd)  { m_pSpRsvd->DestroyWindow();  delete m_pSpRsvd;  }
	
	if (m_pJcntGrid) { m_pJcntGrid->DestroyWindow(); delete m_pJcntGrid; }
	if (m_pOptGrid)  { m_pOptGrid->DestroyWindow();  delete m_pOptGrid;  }
	if (m_pChegGrid) { m_pChegGrid->DestroyWindow(); delete m_pChegGrid; }

	if (m_pTab)	     { m_pTab->DestroyWindow(); delete m_pTab; }
	
	if (m_pSise) { m_pSise->DestroyWindow(); delete m_pSise; }

	if (m_pScreenWnd)
	{
		if (m_pScreenWnd->m_hWnd && ::IsWindow(m_pScreenWnd->m_hWnd))
			m_pScreenWnd->DestroyWindow();
		delete m_pScreenWnd;	m_pScreenWnd = NULL;
	}

// 	if (m_pTooltip)
// 	{
// 		m_pTooltip->Activate(TRUE);
// 		delete m_pTooltip;
// 		m_pTooltip = NULL;
// 	}

	CWnd::OnDestroy();	
}

void CControlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if(nSizeMode == SIZE_GENERAL)
	{
		CalcSize();
	}
	else
	{
		CalcSizeJango();
	}
}

void CControlWnd::CalcSizeJango()
{
	int n;
	CRect rc;
	GetClientRect(rc);
	int width = rc.Width(), height = rc.Height();
	if (width<=0 || height<=0) return;

	int pos_x, pos_y, col, dx;
	int btn_w = 40;
	int btn_h = 18, tab_h = 24;

	// Line 1
	pos_x = 0;
	pos_y = 0;
	m_pBtnCheg->MoveWindow(pos_x, pos_y, btn_w, btn_h, TRUE); pos_x += btn_w + PADDING_VALUE/2;
	m_pBtnChart->MoveWindow(pos_x, pos_y, btn_w, btn_h, TRUE); pos_x += btn_w + PADDING_VALUE/2;
	m_pBtnJango->MoveWindow(pos_x, pos_y, btn_w, btn_h, TRUE); pos_x = width - 20;
	m_pBtnFold->MoveWindow(pos_x, pos_y, 20, btn_h, TRUE);
	
	// Line 2	-- 주문설정 라벨
// 	pos_x = 0;
// 	pos_y += btn_h + PADDING_VALUE;

	// Line 8.0 -- 탭
	pos_x = 0;
	pos_y += tab_h;
	m_pTab->MoveWindow(pos_x, pos_y, width, tab_h, FALSE);
	m_pBtnChegLink->MoveWindow(width-41, pos_y, 20, 20, FALSE);

	m_pBtnSize->MoveWindow(width - 20, pos_y, 20, 20, TRUE);

	// Line 8	-- 종목 - 잔고/미체결 라벨
// 	pos_x = 0;
// 	pos_y += 20;
// 	m_pStMstr->MoveWindow(pos_x, pos_y, width, btn_h, FALSE);

	// 체결그리드
	{
		int pos_x2 = 0;
		int pos_y2 = pos_y + tab_h + PADDING_VALUE/2;
		m_pChegGrid->MoveWindow(pos_x2, pos_y2, width, height-pos_y2);
	}

	// 현재가 화면
	{
		int pos_x2 = 0;
		int pos_y2 = pos_y + tab_h + PADDING_VALUE/2;
		m_pScreenWnd->MoveWindow(pos_x2, pos_y2, width, height - pos_y2 - 65);	
	}
	
	// 시세그리드
	{
		int pos_x2 = 0;
		int pos_y2 = pos_y + tab_h + PADDING_VALUE/2;
		m_pSise->MoveWindow(pos_x2, height - 54, width, 54);
	}

	//미니선옵 전환 버튼
	pos_x = 0;
	pos_y += tab_h + PADDING_VALUE/2;
	dx = (width-9)/3;
	
	m_pBtnGen->MoveWindow(pos_x, pos_y, dx, btn_h);
	
	pos_x += (dx+4);
	
	m_pBtnMini->MoveWindow(pos_x, pos_y, dx, btn_h);

	pos_x += (dx+4);
	
	m_pBtnWeek->MoveWindow(pos_x, pos_y, dx, btn_h);

	// Line 9	-- 선물종목 버튼
	pos_x = 0;
	pos_y += tab_h + PADDING_VALUE/2;
	dx = (width-9)/4;
	for(n=0; n<4; ++n, pos_x+=(dx+4))
	{
		if(n<3)
			m_pBtnFuture[n]->MoveWindow(pos_x, pos_y, dx, btn_h);
		else if(n==3)
		{
			m_pBtnFuture[n]->MoveWindow((int)(pos_x+dx*0.3), pos_y, (int)(dx*0.7), btn_h);
			m_pBtnFuture[n+1]->MoveWindow(pos_x, pos_y, (int)(dx*0.3), btn_h);
		}

		m_pStFuture[n]->MoveWindow(pos_x, pos_y+btn_h+PADDING_VALUE/3, dx, btn_h);
	}

	// Line 10	-- 옵션종목 버튼
	pos_x = 0;
	pos_y += btn_h*2 + PADDING_VALUE;
	dx = (width-9)/4;
	for(n=0; n<4; ++n, pos_x+=(dx+4))
	{
		if(n<3)
			m_pBtnOption[n]->MoveWindow(pos_x, pos_y, dx, btn_h);
		else if(n==3)
		{
			m_pBtnOption[n]->MoveWindow(pos_x+ (int)(dx*0.3), pos_y, (int)(dx*0.7), btn_h);
			m_pBtnOption[n+1]->MoveWindow(pos_x, pos_y, (int)(dx*0.3), btn_h);
		}
	}

	// Line 11	-- 옵션그리드
	pos_x = 0;
	pos_y += btn_h + PADDING_VALUE/2;
	m_pOptGrid->MoveWindow(pos_x, pos_y, width, height-pos_y);
	col = m_pOptGrid->GetColumnCount();
	dx = (width-SCROLLBAR_WIDTH)/col;
	m_pOptGrid->SetColumnWidth(0, dx);
	m_pOptGrid->SetColumnWidth(1, width-SCROLLBAR_WIDTH-(dx*2) );
	m_pOptGrid->SetColumnWidth(2, dx);
	
	MakeBackground();
}

void CControlWnd::CalcSize()
{
	int n;
	CRect rc;
	GetClientRect(rc);
	int width = rc.Width(), height = rc.Height();
	if (width<=0 || height<=0) return;

	int pos_x, pos_y, row, col, dx;
	int btn_w = 40;
	int btn_h = 18, tab_h = 24;

	// Line 1
	pos_x = 0;
	pos_y = 0;
	m_pBtnCheg->MoveWindow(pos_x, pos_y, btn_w, btn_h, TRUE); pos_x += btn_w + PADDING_VALUE/2;
	m_pBtnChart->MoveWindow(pos_x, pos_y, btn_w, btn_h, TRUE); pos_x += btn_w + PADDING_VALUE/2;
	m_pBtnJango->MoveWindow(pos_x, pos_y, btn_w, btn_h, TRUE); pos_x = width - 20;
	m_pBtnFold->MoveWindow(pos_x, pos_y, 20, btn_h, TRUE);
	
	// Line 2	-- 주문설정 라벨
	pos_x = 0;
	pos_y += btn_h + PADDING_VALUE;
	m_pStJumun->MoveWindow(pos_x, pos_y, width, btn_h, FALSE);

	// Line 2<->3 -- 주문가능수량 ( 계좌모드일경우에만 )
	if (!m_pBtnFold->IsWindowEnabled())
	{
		pos_x = 0;
		pos_y += btn_h + PADDING_VALUE;

		m_pStJvol->MoveWindow(pos_x, pos_y, 45, btn_h, FALSE); pos_x += 45;
		m_pEdJprc->MoveWindow(pos_x, pos_y, 50, btn_h, FALSE); pos_x += 55;
		m_chkAutoJvol.MoveWindow(pos_x, pos_y, 80, btn_h, FALSE);

		pos_x = 0;
		pos_y += btn_h + PADDING_VALUE/2;
		dx = width/3;
		m_pJvolGrid->MoveWindow(pos_x, pos_y, width, GRID_CELL_HEIGHT*3, FALSE);
		for(int n=0; n<3; ++n)
			m_pJvolGrid->SetColumnWidth(n, dx);

		CRect rc;
		m_pJvolGrid->GetCellRect(0, 0, &rc);
		m_pBtnJvol->MoveWindow(pos_x, pos_y, rc.Width()+2, rc.Height()+2);

		pos_y += GRID_CELL_HEIGHT*3 + PADDING_VALUE;
	}
	else
	{
		pos_y += btn_h + PADDING_VALUE;
	}

	// Line 3	-- 주문수량 설정 그리드
	pos_x = 0;
	//pos_y += GRID_CELL_HEIGHT*3 + PADDING_VALUE; 위에서 한다~
	row = m_pJcntGrid->GetRowCount();
	col = m_pJcntGrid->GetColumnCount();
	m_pJcntGrid->SetNoScrollBar(TRUE);
	m_pJcntGrid->MoveWindow(0, pos_y, width, btn_h*3 );
	for(n=0; n<col; ++n)
		m_pJcntGrid->SetColumnWidth(n, width/col);
	for(n=0; n<row; ++n)
		m_pJcntGrid->SetRowHeight(n, btn_h);
	
	// Line 4	-- 주문수량 SpinButton
	pos_y += btn_h*3 + PADDING_VALUE/2;
	m_pStJcnt->MoveWindow( 0, pos_y,       75, btn_h, FALSE);
	m_pSpJcnt->MoveWindow(75, pos_y, width-75, btn_h, FALSE);

	// Line 5	-- 예약주문
	pos_y += btn_h + PADDING_VALUE/2;
	m_pStRsvd->MoveWindow( 0, pos_y,       43, btn_h, FALSE);
	m_pSpRsvd->MoveWindow(43, pos_y, 50, btn_h, FALSE);
	m_chkMarket.MoveWindow(96,pos_y, 85, btn_h, FALSE);

	// Line 6	-- 주문조건
	pos_x = 5;
	pos_y += btn_h + PADDING_VALUE/2;
	dx = (width-pos_x)/3;
	m_rdNone.MoveWindow(pos_x     , pos_y, dx, btn_h, FALSE); pos_x += 5;
	m_rdIOC.MoveWindow( pos_x+dx  , pos_y, dx, btn_h, FALSE); pos_x += 2;
	m_rdFOK.MoveWindow( pos_x+dx*2, pos_y, dx, btn_h, FALSE);

	// Line 7	-- 복수주문
	pos_x = 5;
	pos_y += btn_h + PADDING_VALUE/2;
	m_chkMulti.MoveWindow(pos_x, pos_y, 70, btn_h, FALSE);
	m_pSpMulti->MoveWindow(pos_x+70, pos_y, width-(pos_x+70), btn_h);

	// Line 7-1 -- 원클릭주문
	pos_x = 5;
	pos_y += btn_h + PADDING_VALUE/2;
	m_chkOneClick.MoveWindow(pos_x, pos_y, 90, btn_h, FALSE);

	//if(m_dept == "813" || m_dept == m_sAccnDept)
	if(m_dept == m_sAccnDept)
	{
		pos_x = 5;
		pos_y += btn_h + PADDING_VALUE/2;
		m_chkChaik.MoveWindow(pos_x, pos_y, 90, btn_h, FALSE);
	}
	else
	{
		m_chkChaik.ShowWindow(SW_HIDE);
	}

// 	pos_x = 5;
// 	pos_y += btn_h +PADDING_VALUE/2;
// 	m_chkAuto.MoveWindow( pos_x, pos_y,       70, btn_h, FALSE);
// 	m_pSpAuto->MoveWindow(75, pos_y, width-75, btn_h, FALSE);
// 
// 	pos_x = 5;
// 	pos_y += btn_h +PADDING_VALUE/2;
// 	m_chkLoss.MoveWindow( pos_x, pos_y,       70, btn_h, FALSE);
// 	m_pSpLoss->MoveWindow(75, pos_y, width-75, btn_h, FALSE);

	// Line 8.0 -- 탭
	pos_x = 0;
	pos_y += tab_h;
	m_pTab->MoveWindow(pos_x, pos_y, width, tab_h, FALSE);
	m_pBtnChegLink->MoveWindow(width-41, pos_y, 20, 20, FALSE);

	m_pBtnSize->MoveWindow(width - 20, pos_y, 20, 20, TRUE);

	// Line 8	-- 종목 - 잔고/미체결 라벨
// 	pos_x = 0;
// 	pos_y += 20;
// 	m_pStMstr->MoveWindow(pos_x, pos_y, width, btn_h, FALSE);

	// 체결그리드
	{
		int pos_x2 = 0;
		int pos_y2 = pos_y + tab_h + PADDING_VALUE/2;
		m_pChegGrid->MoveWindow(pos_x2, pos_y2, width, height-pos_y2);
	}

	// 현재가 화면
	{
		int pos_x2 = 1;
		int pos_y2 = pos_y + tab_h + PADDING_VALUE/2;
		m_pScreenWnd->MoveWindow(pos_x2, pos_y2, width, height - pos_y2 - 65);	
	}

	// 시세그리드
	{
		int pos_x2 = 0;
		int pos_y2 = pos_y + tab_h + PADDING_VALUE/2;
		m_pSise->MoveWindow(pos_x2, height - 54, width, 54);
	}

	//미니선옵 전환 버튼
	pos_x = 0;
	pos_y += tab_h + PADDING_VALUE/2;
	dx = (width-9)/3;

	m_pBtnGen->MoveWindow(pos_x, pos_y, dx, btn_h);

	pos_x += (dx+4);

	m_pBtnMini->MoveWindow(pos_x, pos_y, dx, btn_h);

	pos_x += (dx+4);
	
	m_pBtnWeek->MoveWindow(pos_x, pos_y, dx, btn_h);

	// Line 9	-- 선물종목 버튼
	pos_x = 0;
	pos_y += btn_h + PADDING_VALUE;
	dx = (width-9)/4;
	for(n=0; n<4; ++n, pos_x+=(dx+4))
	{
		if(n<3)
			m_pBtnFuture[n]->MoveWindow(pos_x, pos_y, dx, btn_h);
		else if(n==3)
		{
			m_pBtnFuture[n]->MoveWindow(pos_x+ (int)(dx*0.3), pos_y, (int)(dx*0.7), btn_h);
			m_pBtnFuture[n+1]->MoveWindow(pos_x, pos_y, (int)(dx*0.3), btn_h);
		}
		//m_pBtnFuture[n]->MoveWindow(pos_x, pos_y, dx, btn_h);
		m_pStFuture[n]->MoveWindow(pos_x, pos_y+btn_h+PADDING_VALUE/3, dx, btn_h);
	}

	// Line 10	-- 옵션종목 버튼
	pos_x = 0;
	pos_y += btn_h*2 + PADDING_VALUE;
	dx = (width-9)/4;
	for(n=0; n<4; ++n, pos_x+=(dx+4))
	{
		if(n<3)
			m_pBtnOption[n]->MoveWindow(pos_x, pos_y, dx, btn_h);
		else if(n==3)
		{
			m_pBtnOption[n]->MoveWindow(pos_x+ (int)(dx*0.3), pos_y, (int)(dx*0.7), btn_h);
			m_pBtnOption[n+1]->MoveWindow(pos_x, pos_y, (int)(dx*0.3), btn_h);
		}
	}

	// Line 11	-- 옵션그리드
	pos_x = 0;
	pos_y += btn_h + PADDING_VALUE/2;
	m_pOptGrid->MoveWindow(pos_x, pos_y, width, height-pos_y);
	col = m_pOptGrid->GetColumnCount();
	dx = (width-SCROLLBAR_WIDTH)/col;
	m_pOptGrid->SetColumnWidth(0, dx);
	m_pOptGrid->SetColumnWidth(1, width-SCROLLBAR_WIDTH-(dx*2) );
	m_pOptGrid->SetColumnWidth(2, dx);
	
	MakeBackground();
}

int CControlWnd::GetChaik()
{
	return m_chkChaik.GetCheck();
}

int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pBtnCheg  = CreateImgButton("체결", IDC_BTN_CHEG , m_hBtn2);
	m_pBtnChart = CreateImgButton("차트", IDC_BTN_CHART, m_hBtn2);
	m_pBtnJango = CreateImgButton("잔고", IDC_BTN_JANGO, m_hBtn2);
	m_pBtnFold   = CreateImgButton("<<" , IDC_BTN_FOLD,  m_hBtn2);
	m_pBtnChegLink = CreateImgButton("" , IDC_BTN_CHEGLINK,  m_hBtnLink);

	CRect rc(0, 0, 0, 0);
	m_pStJumun = new CfxStatic(m_pFont);
	m_pStJumun->Create("주문 설정", WS_CHILD|WS_VISIBLE|WS_BORDER, rc, this);
	m_pStJumun->SetBkColor(GetIndexColor(COLOR_GRIDHEAD), false);
	m_pStJumun->SetFgColor(GetIndexColor(COLOR_GRIDHEADTXT), false);

	m_pStJvol = new CfxStatic(m_pFont);
	m_pStJvol->Create("주문가", WS_VISIBLE|WS_CHILD|SS_LEFT, rc, this);
	m_pStJvol->SetBkColor(m_crBodyBk );
	m_pStJvol->SetAlignment(alLEFT);
	
	m_pEdJprc = new CfxEdit(m_pFont);
	m_pEdJprc->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_RIGHT|ES_READONLY, rc, this, -1);
	m_pEdJprc->SetPasswordChar(0x0);
	m_pEdJprc->SetLimitText(8);

	m_chkAutoJvol.Create("자동조회", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, rc, this, IDC_CHK_AUTOJVOL);
	m_chkAutoJvol.SetFont(m_pFont);
	m_chkAutoJvol.SetCheck(FALSE);

	m_pJvolGrid = new CGridCtrl();
	m_pJvolGrid->SetNoScrollBar(TRUE);
	m_pJvolGrid->Create(rc, this, IDC_GRID_JVOL, WS_CHILD|WS_BORDER|WS_TABSTOP|WS_VISIBLE|WS_CLIPSIBLINGS);
	SetJvolGrid();

	m_pBtnJvol = CreateImgButton("조회", IDC_BTN_JVOL, m_hBtn3);
	m_pBtnJvol->BringWindowToTop();

	m_pJcntGrid = new CGridCtrl();
	m_pJcntGrid->SetNoScrollBar(TRUE);
	m_pJcntGrid->Create(rc, this, IDC_GRID_JCNT);
	m_pJcntGrid->EnableSelection(FALSE);
	m_pJcntGrid->SetFrameFocusCell(FALSE);
	m_pJcntGrid->EnableTitleTips(FALSE);
	m_pJcntGrid->SetFont(m_pFont);
	CGridCellBase *pc = m_pJcntGrid->GetDefaultCell(FALSE, FALSE);
	if (pc)
	{
		pc->SetFormat( DT_VCENTER|DT_SINGLELINE|DT_RIGHT );
		pc->SetMargin( 5 );
		m_pJcntGrid->SetRowCount(3);
		m_pJcntGrid->SetColumnCount(4);
	}
	m_pJcntGrid->ShowScrollBar(SB_BOTH, FALSE);

	m_pStJcnt = new CfxStatic(m_pFont);
	m_pStJcnt->Create(" 주문수량 ", WS_VISIBLE|WS_CHILD, rc, this);
	m_pStJcnt->SetBkColor( m_crBodyBk );

//	m_pSpJcnt = new CfxSpin(m_pFont);    
	m_pSpJcnt = new CSpEdit(m_pFont);    
//	m_pSpJcnt->Create(attrNUMERIC|attrCOMMA, spNOT, rc, this, -1);
	m_pSpJcnt->Create(attrNUMERIC|attrCOMMA, spNOT, rc, this, IDC_EDIT_JMVAL);
	m_pSpJcnt->SetRangeEx(0, 10000);

	m_rdNone.Create("없음", WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON, rc, this, IDC_RD_NONE);
	m_rdIOC.Create("IOC", WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON, rc, this, IDC_RD_IOC);
	m_rdFOK.Create("FOK", WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON, rc, this, IDC_RD_FOK);
	m_rdNone.SetFont(m_pFont); m_rdIOC.SetFont(m_pFont); m_rdFOK.SetFont(m_pFont);
	m_rdNone.SetCheck(TRUE);

	
	m_chkMulti.Create("다중매매", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, rc, this, IDC_CHK_MULTI);
	m_chkMulti.SetFont(m_pFont);
//	m_chkMulti.SetCheck(FALSE);
	char buff[128];
	GetPrivateProfileString("ORDER", "MULTI_TRADE", "FALSE", buff, sizeof(buff), m_profile);
	if(CString(buff,1) == "1")
		m_chkMulti.SetCheck(TRUE);
	else
		m_chkMulti.SetCheck(FALSE);

	m_chkOneClick.Create("원클릭 주문", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, rc, this, IDC_CHK_OCLICK);
	m_chkOneClick.SetFont(m_pFont);
	
	GetPrivateProfileString("ORDER", "ONECLICK", "FALSE", buff, sizeof(buff), m_profile);
	if(CString(buff,1) == "1")
		m_chkOneClick.SetCheck(TRUE);
	else
		m_chkOneClick.SetCheck(FALSE);

	GetParent()->SendMessage(WM_APP_SIG, WP_ONECLICK_MODE, (LPARAM)m_chkOneClick.GetCheck());
	m_chkChaik.Create("차익거래", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, rc, this, IDC_COMBO_CHAIK);
	m_chkChaik.SetFont(m_pFont);
	m_chkChaik.SetCheck(TRUE);

	m_pSpMulti = new CfxSpin(m_pFont); 
	m_pSpMulti->Create(attrNUMERIC|attrCOMMA, spNOT, rc, this, IDC_SPIN_MULTITRADE);
	m_pSpMulti->SetRangeEx(2, 10);

	GetPrivateProfileString("ORDER", "MULTI_TRADE_VOL", "2", buff, sizeof(buff), m_profile);
	CString sdata;
	sdata.Format("%s", buff);
	m_pSpMulti->SetText(sdata);

	if(m_chkMulti.GetCheck())
		m_pSpMulti->EnableWindow(TRUE);
	else
		m_pSpMulti->EnableWindow(FALSE);

	m_pStRsvd = new CfxStatic(m_pFont);
	m_pStRsvd->Create("예약틱", WS_VISIBLE|WS_CHILD, rc, this);
	m_pStRsvd->SetBkColor( m_crBodyBk );

	m_chkMarket.Create("시장가", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, rc, this, IDC_CHK_MARKET);
	m_chkMarket.SetFont(m_pFont);
	m_chkMarket.SetCheck(FALSE);
	
	m_pSpRsvd = new CfxSpin(m_pFont);
	m_pSpRsvd->Create(attrNUMERIC|attrCOMMA, spNOT, rc, this, -1);
	m_pSpRsvd->SetText("1");
	m_pSpRsvd->SetRangeEx(-100, 100);

	m_pSise = new CSiseWnd(m_pParent);
	m_pSise->Create(rc, this, -1, WS_CHILD|WS_BORDER|WS_VISIBLE);

	m_pScreenWnd = new CScreenWnd(m_pParent);
	m_pScreenWnd->Create(NULL, NULL, WS_CHILD, rc, this, IDC_MAPVIEW);
// 	m_pTooltip = new CToolTipCtrl;
// 	m_pTooltip->Create(this);
// 	m_pTooltip->Activate(TRUE);
// 	m_pTooltip->AddTool(GetDlgItem(IDC_CHK_MARKET),"사각형 표시는 시장가 주문을 뜻합니다.");

// 	m_chkAuto.Create("자동청산", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, rc, this, IDC_CHK_AUTO);
// 	m_chkAuto.SetFont(m_pFont);
// 	m_chkAuto.SetCheck(FALSE);
// 	
// 	m_pSpAuto = new CfxSpin(m_pFont);
// 	m_pSpAuto->Create(attrNUMERIC|attrCOMMA, spNOT, rc, this, -1);
// 	m_pSpAuto->SetText("1");
// 	m_pSpAuto->SetRangeEx(-100, 100);
// 	m_pSpAuto->EnableWindow(FALSE);
// 
// 	m_chkLoss.Create("자동손절", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, rc, this, IDC_CHK_LOSS);
// 	m_chkLoss.SetFont(m_pFont);
// 	m_chkLoss.SetCheck(FALSE);
// 	
// 	m_pSpLoss = new CfxSpin(m_pFont);
// 	m_pSpLoss->Create(attrNUMERIC|attrCOMMA, spNOT, rc, this, -1);
// 	m_pSpLoss->SetText("1");
// 	m_pSpLoss->SetRangeEx(-100, 100);
// 	m_pSpLoss->EnableWindow(FALSE);

	m_pBtnGen = CreateImgButton("일 반", IDC_BTN_GEN, m_hBtnLong);
	m_pBtnMini = CreateImgButton("미 니", IDC_BTN_MINI, m_hBtnLong);
	m_pBtnWeek = CreateImgButton("위클리", IDC_BTN_WEEK, m_hBtnLong);  //week

	CString home = Variant(homeCC, "");

	CString szn, szu, szd;
	
	szn = home + "\\image\\" + "창조절상하_확대.BMP";
	szu = home + "\\image\\" + "창조절상하_확대_en.BMP";
	szd = home + "\\image\\" + "창조절상하_확대_dn.BMP";
	
	m_hBtnSize[0] = GetAxBitmap(szn);
	m_hBtnSize[1] = GetAxBitmap(szu);
	m_hBtnSize[2] = GetAxBitmap(szd);

 	m_pBtnSize = CreateImgButton("", IDC_BTN_SIZE , m_hBtnSize);

	m_pTab = new CfxTab();
	m_pTab->Create(CRect(0,0,0,24), this, IDC_CTL_TAB1);
	
	m_pTab->InsertItem(0, "잔(미)");
	//m_pTab->InsertItem(1, "현재");
	//m_pTab->InsertItem(2, "체");
	m_pTab->LoadTabImage(Variant(homeCC, ""), false);
	m_pTab->SetStyle(0x00000001, false);
	m_pTab->SetBorderColor(GetIndexColor(TAB_BORDER_INDEX));
	m_pTab->SetColor(GetIndexColor(CLR_TAB_FORE), GetIndexColor(CONTENTS_BACK_INDEX), 
			GetIndexColor(CLR_TAB_SELFORE), GetIndexColor(CONTENTS_BACK_INDEX), GetIndexColor(CONTENTS_BACK_INDEX));
	m_pTab->SetFont(m_pFont);

	m_pChegGrid = new CGridCtrl();
	m_pChegGrid->Create(rc, this, IDC_GRID_CHEG);
	m_pChegGrid->ShowWindow(SW_HIDE);
	m_pChegGrid->SetFont(m_pFont);
	SetChegGrid();

	m_pStMstr = new CfxStatic(m_pFont);
	m_pStMstr->Create("종목 - 잔고(미체결)", WS_CHILD|WS_VISIBLE|WS_BORDER, rc, this);
	m_pStMstr->SetBkColor(GetIndexColor(COLOR_GRIDHEAD), false);
	m_pStMstr->SetFgColor(GetIndexColor(COLOR_GRIDHEADTXT), false);

	for(size_t n=0; n<4; ++n)
	{
		m_pBtnFuture[n] = CreateImgButton("", IDC_BTN_FUT1+n , m_hBtn2);
		if (n<m_fjcode.size())
		{
			CString str(m_fjcode[n].hnam, sizeof(m_fjcode[n].hnam));
			str.TrimRight();
			m_pBtnFuture[n]->SetWindowText(str.Right(4));
		}

		if(n==3)
		{
			m_pBtnFuture[4] = CreateImgButton("", IDC_BTN_FUT1+n+1, m_hBtn2);

			m_pBtnFuture[4]->SetWindowText("▼");
		}

		m_pStFuture[n] = new CfxStatic(m_pFont);
		m_pStFuture[n]->Create("123", WS_VISIBLE|WS_CHILD, rc, this);
		m_pStFuture[n]->Init(ssBORDER|ssTEXT, 0, "", "", 
			GetSysColor(COLOR_3DFACE), GetSysColor(COLOR_3DFACE), GetSysColor(COLOR_BTNTEXT), alRIGHT);
		m_pStFuture[n]->SetBkColor(m_crWhite);
		m_pStFuture[n]->SetBorderColor( m_crLine, false);

// 		m_pBtnOption[n] = CreateImgButton("", IDC_BTN_OPT1+n , m_hBtn2);
// 		{
// 			CString str(m_ophead.pjym[n], sizeof(m_ophead.pjym[n]));
// 			str.TrimRight();
// 			m_pBtnOption[n]->SetWindowText(str.Right(2) + "월");
// 		}
	}

	for(int n=0;n<4;++n)
	{
		m_pBtnOption[n] = CreateImgButton("", IDC_BTN_OPT1+n , m_hBtn2);
		{
			CString str(m_ophead.pjym[n], sizeof(m_ophead.pjym[n]));
			str.TrimRight();
			m_pBtnOption[n]->SetWindowText(str.Right(4));
		}

		if(n==3)
		{
			m_pBtnOption[4] = CreateImgButton("", IDC_BTN_OPT1+n+1, m_hBtn2);
			
			m_pBtnOption[4]->SetWindowText("▼");
		}
	}

	m_pOptGrid = new CGridCtrl();
	m_pOptGrid->Create(rc, this, IDC_GRID_OPT);
	m_pOptGrid->SetFont(m_pFont);
	SetOptionGrid();
	//SetOptionData(0);
	//PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_OPT1, 0), (LPARAM)m_pBtnOption[0]->GetSafeHwnd());
	//PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_FUT1, 0), (LPARAM)m_pBtnFuture[0]->GetSafeHwnd());	//2015.10.12 KSJ 첫번째 월물이 선택되도록
	m_nCurFSelect = 3;
	m_nCurOSelect = 3;

	LoadConfig();

	//OnBtnGen();
	OnBtn_Gen();

	m_dept = Variant(deptCC,"");

	return 0;
}

void CControlWnd::SetControlWindowWEEK(bool bInit)
{
	CString stmp;
	for(size_t n=0; n<4; ++n)
	{
		if (n<m_fjcode.size())
		{
			CString str(m_fjcode[n].hnam, sizeof(m_fjcode[n].hnam));
			str.TrimRight();
			//stmp.Format("IB300600 !!! %s\n", str);
			//OutputDebugString(stmp);
			m_pBtnFuture[n]->SetWindowText(str.Right(4));
			
			m_pBtnFuture[n]->Invalidate();
		}
	}
	
	SetWeelyBtnSHOW();
	
	m_nCurFSelect = 3;
	m_nCurOSelect = 3;

//CString slog;
//slog.Format("IB300600 <<<SetControlWindowWEEK>>>  ");
//OutputDebugString(slog);
	SetWOptionData(0, true);
}

void CControlWnd::SetControlWindow()
{
	CString stmp;
	for(size_t n=0; n<4; ++n)
	{
		//m_pBtnFuture[n] = CreateImgButton("", IDC_BTN_FUT1+n , m_hBtn2);
		if (n<m_fjcode.size())
		{
			CString str(m_fjcode[n].hnam, sizeof(m_fjcode[n].hnam));
			str.TrimRight();
//stmp.Format("IB300600 !!! %s\n", str);
//OutputDebugString(stmp);
			m_pBtnFuture[n]->SetWindowText(str.Right(4));

			m_pBtnFuture[n]->Invalidate();
		}
		
// 		if(n==3)
// 		{			
// 			m_pBtnFuture[4]->SetWindowText("▼");
// 		}
		
// 		m_pStFuture[n] = new CfxStatic(m_pFont);
// 		m_pStFuture[n]->Create("123", WS_VISIBLE|WS_CHILD, rc, this);
// 		m_pStFuture[n]->Init(ssBORDER|ssTEXT, 0, "", "", 
// 			GetSysColor(COLOR_3DFACE), GetSysColor(COLOR_3DFACE), GetSysColor(COLOR_BTNTEXT), alRIGHT);
// 		m_pStFuture[n]->SetBkColor(m_crWhite);
// 		m_pStFuture[n]->SetBorderColor( m_crLine, false);
		
		// 		m_pBtnOption[n] = CreateImgButton("", IDC_BTN_OPT1+n , m_hBtn2);
		// 		{
		// 			CString str(m_ophead.pjym[n], sizeof(m_ophead.pjym[n]));
		// 			str.TrimRight();
		// 			m_pBtnOption[n]->SetWindowText(str.Right(2) + "월");
		// 		}
	}
	
	for(int n=0;n<4;++n)
	{
		//m_pBtnOption[n] = CreateImgButton("", IDC_BTN_OPT1+n , m_hBtn2);
		{
			CString str(m_ophead.pjym[n], sizeof(m_ophead.pjym[n]));
			str.TrimRight();

//slog.Format("IB300600 @@@ %s\n", str);
//OutputDebugString(slog);

			m_pBtnOption[n]->SetWindowText(str.Right(4));
			m_pBtnOption[n]->Invalidate();
		}

// 		if(n==3)
// 		{
// 			m_pBtnOption[4] = CreateImgButton("", IDC_BTN_OPT1+n+1, m_hBtn2);
// 			
// 			m_pBtnOption[4]->SetWindowText("▼");
// 		}
	}

	//SetOptionData(0);
//	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_OPT1, 0), (LPARAM)m_pBtnOption[0]->GetSafeHwnd());
//	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_FUT1, 0), (LPARAM)m_pBtnFuture[0]->GetSafeHwnd());	//2015.10.12 KSJ 첫번째 월물이 선택되도록  //test  첫번째 월물이 선택되지 않도록....
	m_nCurFSelect = 3;
	m_nCurOSelect = 3;

	if(m_pTab->GetCurSel() == 0)
	{
		for(int n=0; n<5; ++n)
		{
			if (m_pBtnOption[n])
				m_pBtnOption[n]->ShowWindow(SW_SHOW);
		}
	}
	
}

void CControlWnd::SetWeelyBtnSHOW()
{
	if (m_pTab->GetCurSel() == 1 || m_pTab->GetCurSel() == 2)
		return;

	m_pBtnOption[0]->ShowWindow(SW_HIDE);
	m_pBtnOption[1]->ShowWindow(SW_HIDE);
	m_pBtnOption[2]->ShowWindow(SW_HIDE);
	m_pBtnOption[3]->ShowWindow(SW_HIDE);
	m_pBtnOption[4]->ShowWindow(SW_HIDE);

	int icntbtn = 0;
	for (int ii = 0; ii < 11; ii++)
	{
		CString str(m_ophead.pjym[ii], sizeof(m_ophead.pjym[ii]));
		if (str != "000000")
		{
			m_pBtnOption[icntbtn]->SetWindowText(str.Right(4));
			m_pBtnOption[icntbtn]->ShowWindow(SW_SHOW);
			m_pBtnOption[icntbtn]->Invalidate();
			icntbtn++;
		}

		if (icntbtn == 5)
			break;
	}

	/*
	CString str(m_ophead.pjym[0], sizeof(m_ophead.pjym[0]));
	m_pBtnOption[0]->SetWindowText(str.Right(4));
	if(str == "000000")
		m_pBtnOption[0]->ShowWindow(SW_HIDE);
	m_pBtnOption[0]->Invalidate();

	CString str1(m_ophead.pjym[1], sizeof(m_ophead.pjym[1]));
	m_pBtnOption[1]->SetWindowText(str1.Right(4));
	if(str1 == "000000")
		m_pBtnOption[1]->ShowWindow(SW_HIDE);
	m_pBtnOption[1]->Invalidate();

	m_pBtnOption[2]->ShowWindow(FALSE);   //위클리인경우 월물이2개라 남은 두개는 안보여준다
	m_pBtnOption[3]->ShowWindow(FALSE);
	m_pBtnOption[4]->ShowWindow(FALSE);
	*/
}

CfxImgButton* CControlWnd::CreateImgButton(LPCSTR caption, int nID, CBitmap **bmp)
{
	CfxImgButton *pBtn = new CfxImgButton(m_pFont);
	if (pBtn->Create(caption, CRect(0, 0, 0, 0), this, nID, FALSE, TRUE))
	{
		pBtn->SetImgBitmap(	bmp[0]->operator HBITMAP(), bmp[1]->operator HBITMAP(), bmp[2]->operator HBITMAP() );
		return pBtn;
	}
	else return NULL;
}

void CControlWnd::OnBtnCheg()
{
	CString opt;
	opt.Format(MAP_CHEG);
	SetView(opt);
}

void CControlWnd::OnBtnChart()
{
	CString opt;
	opt.Format(MAP_CHART);
	SetView(opt);
}

void CControlWnd::OnBtnJango()
{
	CString opt;
	opt.Format(MAP_JANGO);
	SetView(opt);	
}

void CControlWnd::MakeBackground( int width, int height )
{
	if (width<0 || height<0)
	{
		CRect rc;
		GetClientRect(rc);
		width = rc.Width();
		height = rc.Height();
	}

	CDC *pdc = GetDC();
	{
		CDC mdc; 
		CBitmap *oldBmp;

		m_hBk.DeleteObject();
		m_hBk.CreateCompatibleBitmap(pdc, width, height);
		
		mdc.CreateCompatibleDC(pdc);
		oldBmp = mdc.SelectObject(&m_hBk);
		
		mdc.FillSolidRect(0, 0, width, height, m_crBodyBk);
		
		mdc.SelectObject(oldBmp);
		mdc.DeleteDC();
	}
	ReleaseDC(pdc);
}

void CControlWnd::OnPaint() 
{
	CPaintDC dc(this);
	Draw(&dc);
}

void CControlWnd::Draw(CDC *pdc)
{
	CDC mdc;
	CRect rc;
	
	GetClientRect(rc);
	mdc.CreateCompatibleDC(pdc);	
	CBitmap *oldBmp = mdc.SelectObject(&m_hBk);
	pdc->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &mdc, 0, 0, SRCCOPY);
	mdc.SelectObject(oldBmp);
	
	mdc.DeleteDC();	
}

BOOL CControlWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	static CString oldVal;
	static CCellID oldId(-1, -1);

	if (wParam == IDC_SPIN_MULTITRADE)
	{
		if (m_chkMulti.GetCheck() && m_pSpMulti->IsWindowEnabled())
			WritePrivateProfileString("ORDER", "MULTI_TRADE_VOL", m_pSpMulti->GetText(), (LPCSTR)m_profile);
	}
	else if (wParam==IDC_GRID_JCNT)
	{
		*pResult = 1;
        GV_DISPINFO *pi = (GV_DISPINFO*)lParam;
		if (pi->hdr.code == GVN_BEGINLABELEDIT)
		{
			// 편집 시작하면 현재 값을 저장
			oldVal = m_pJcntGrid->GetItemText(pi->item.row, pi->item.col);
		}
        else if (pi->hdr.code == GVN_ENDLABELEDIT || pi->hdr.code == GVN_SELCHANGED)
		{
			CCellID cId(pi->item.row, pi->item.col);
			CString key, val = m_pJcntGrid->GetItemText(cId.row, cId.col);
			val.TrimLeft(); val.TrimRight();

			if (pi->hdr.code==GVN_ENDLABELEDIT)
			{
				// 공백이면 원래값으로 되돌린다.
				if (val.GetLength()>0)
				{
					key.Format("MA_%d_%d", cId.row, cId.col);
					WritePrivateProfileString("JQTY_GRID", (LPCSTR)key, (LPCSTR)val, (LPCSTR)m_profile);
				} 
				else 
				{
					val = oldVal;
					m_pJcntGrid->SetItemText(cId.row, cId.col, oldVal);
				}
			}
			
			// 주문수량 세팅
			m_pSpJcnt->SetText(val);

			for(int row=0; row<m_pJcntGrid->GetRowCount(); ++row)
			{
				for(int col=0; col<m_pJcntGrid->GetColumnCount(); ++col)
				{
					if (pi->item.row==row && pi->item.col==col)
					{
						m_pJcntGrid->SetItemBkColour(row, col, ::GetSysColor(COLOR_HIGHLIGHT));
						m_pJcntGrid->SetItemFgColour(row, col, ::GetSysColor(COLOR_HIGHLIGHTTEXT));
					}
					else
					{
						m_pJcntGrid->SetItemBkColour(row, col, m_crWhite);
						m_pJcntGrid->SetItemFgColour(row, col, m_crBlack);
					}
				}
			}
			m_pJcntGrid->Invalidate(FALSE);
		}
	}
	else if (wParam==IDC_GRID_JVOL)
	{
		NM_GRIDVIEW *view = (NM_GRIDVIEW *)lParam;
		if (view->hdr.code==NM_CLICK || view->hdr.code==NM_DBLCLK)
		{
			if (view->iRow>0 && view->iColumn>0)
			{
				CString val = m_pJvolGrid->GetItemText(view->iRow, view->iColumn);
				val.Replace(",", "");
				val.TrimRight();
				if (!val.IsEmpty())
					SetJcnt( atoi((LPCSTR)val) );
			}
		}
	}
	else if (wParam==IDC_GRID_OPT)
	{
		*pResult = 1;
		NM_GRIDVIEW *view = (NM_GRIDVIEW *)lParam;
		if (view->hdr.code == NM_DBLCLK || view->hdr.code == NM_CLICK)
		{
			if (view->iRow>0)
			{
				OptionRows &rows = m_dtOption[m_dtOptionIdx];
				if (view->iColumn==0)       // Call
					GetParent()->SendMessage(WM_APP_SIG, WP_CODE_CHANGE, (LPARAM)rows[ view->iRow-1 ].call_code);
				else if (view->iColumn==2)	// Put
					GetParent()->SendMessage(WM_APP_SIG, WP_CODE_CHANGE, (LPARAM)rows[ view->iRow-1 ].put_code);
			}
		}
	}
	return CWnd::OnNotify(wParam, lParam, pResult);
}

HBRUSH CControlWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	int nID = pWnd->GetDlgCtrlID();
	if (nID==IDC_RD_NONE || nID==IDC_RD_IOC || nID==IDC_RD_FOK || nID==IDC_CHK_MULTI || nID==IDC_CHK_OCLICK || nID==IDC_CHK_AUTOJVOL || nID==IDC_CHK_AUTO || nID==IDC_CHK_LOSS || nID == IDC_CHK_MARKET || nID == IDC_COMBO_CHAIK)
	{
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(m_crBodyBk);

		//return ::CreateSolidBrush(m_crBodyBk);
		return m_hBodyBrush;
	}

	return hbr;
}

void CControlWnd::OnChkMulti()
{
	BOOL bCheck = m_chkMulti.GetCheck();
	m_chkMulti.SetCheck(FALSE);
	if (bCheck)
	{
		CDialog dlg(IDD_MULTI_DLG, this);
		if (dlg.DoModal()==IDOK)
		{
			m_chkMulti.SetCheck(TRUE);
			m_pSpMulti->EnableWindow(TRUE);
		}
	}
	else
	{
		m_pSpMulti->EnableWindow(FALSE);
	}

	bCheck = m_chkMulti.GetCheck();
	if(bCheck)
		WritePrivateProfileString("ORDER", "MULTI_TRADE", (LPCSTR)"1", (LPCSTR)m_profile);
	else
		WritePrivateProfileString("ORDER", "MULTI_TRADE", (LPCSTR)"0", (LPCSTR)m_profile);

	WritePrivateProfileString("ORDER", "MULTI_TRADE_VOL", m_pSpMulti->GetText(), (LPCSTR)m_profile);
}

void CControlWnd::LoadMaster(int igubn)
{
	CString slog, stmp;
//	slog.Format("IB300600 [CControlWnd:LoadMaster]  [%d]\n", igubn);
//	OutputDebugString(slog);

	CFile file;
	CString path;
	path.Format("%s\\tab\\", Variant(homeCC, ""));	
	
	CString strFuture;
	CString strOption;
	
	if(igubn == NORMALBTN)
	{
		strFuture = "fjcode.dat";
		strOption = "opcode2.dat";
	}
	else if(igubn == MINIBTN)
	{
		strFuture = "mfcode.dat";
		strOption = "mocode2.dat";
	}
	else if(igubn == WEEKBTN)
	{
		strFuture = "fjcode.dat";
		strOption = "wocode.dat";
	}
	
	// 선물종목코드
	if (file.Open(path+strFuture, CFile::modeRead))
	{
		m_fjcode.resize( (size_t)(file.GetLength()/sizeof(struct fjcode)) );
		file.Read(&m_fjcode[0], m_fjcode.size() * sizeof(struct fjcode) );
		file.Close();
	}
	else
	{
		if(igubn == MINIBTN)
			MessageBox("미니선물종목 데이타가 없읍니다.","IBK투자증권",MB_OK|MB_ICONERROR);
		else if(igubn == WEEKBTN)
			MessageBox("위클리옵션 종목 데이타가 없읍니다.","IBK투자증권",MB_OK|MB_ICONERROR);
	}
	
	if (file.Open(path+strOption, CFile::modeRead))
	{
		m_opcode.resize((size_t)((file.GetLength()-sizeof(struct ojcodh)) / sizeof(struct ojcode)) );
		file.Read(&m_ophead, sizeof(struct ojcodh));
		file.Read(&m_opcode[0], m_opcode.size() * sizeof(struct ojcode));
		file.Close();
		
		m_dtOption.clear();
		vector<struct ojcode>::iterator st, ed;
		st = m_opcode.begin();
		ed = m_opcode.end();
		int isize =m_opcode.size();
		int icnt = 0;

		m_sArrTWeek.RemoveAll();
		m_sArrMWeek.RemoveAll();
		//CString str;
		for(int n=0;st!=ed; ++st)
		{
			for(int i=0; i<11; ++i)
			{
				if (st->put[i].yorn=='1')
				{
					OptionRow row;
					row.atmg = (st->atmg==1);
					
					sprintf(row.call_code, "%.8s", st->call[10-i].cod2);
					sprintf(row.put_code, "%.8s", st->put[i].cod2);
					if (strlen(st->price) > 5)
						sprintf(row.hsga, "%.4s.%.2s", &st->price[0], &st->price[3]);
					else
						sprintf(row.hsga, "%.3s.%.2s", &st->price[0], &st->price[3]);
					
					//str.Format("[opcode] %s", st->price);
					//OutputDebugString(str);

					m_dtOption[i].push_back(row);
					isize = m_dtOption[i].size();

					if (row.atmg && igubn == WEEKBTN)
					{
						if (row.call_code[1] == 'A' && row.call_code[2] == 'F')
						{
							stmp.Format("%s", row.call_code);
							stmp.Trim();
							m_sArrMWeek.Add(stmp);
						}
						else if (row.call_code[1] == '0' && row.call_code[2] == '9')
						{
							stmp.Format("%s", row.call_code);
							stmp.Trim();
							m_sArrTWeek.Add(stmp);
						}

					}
				}
			}

		//slog.Format("IB300600 [CControlWnd:LoadMaster]  [%d]\n", isize);
		//OutputDebugString(slog);
			icnt++;
		}

	//slog.Format("------IB300600 [CControlWnd:LoadMaster]  [%d]\n", m_dtOption.size());
	//OutputDebugString(slog);
		
		m_OptionMap.clear();
		for(int n=0; n<11; ++n)
		{
			reverse(m_dtOption[n].begin(), m_dtOption[n].end());
			for(size_t j=0; j<m_dtOption[n].size(); ++j)
			{
				m_OptionMap[ m_dtOption[n][j].call_code ] = OptionIdxRow(n, j+1);
				m_OptionMap[ m_dtOption[n][j].put_code  ] = OptionIdxRow(n, j+1);
				//m_OptionMap[string(st->call[3-i].cod2, 8)] = OptionIdxRow(i, m_dtOption[i].size()-1);
			}
		}
	}
	else
	{
		if(igubn == MINIBTN)
			MessageBox("미니옵션종목 데이타가 없읍니다.","IBK투자증권",MB_OK|MB_ICONERROR);
		else if(igubn == WEEKBTN)
			MessageBox("위클리옵션 종목 데이타가 없읍니다.","IBK투자증권",MB_OK|MB_ICONERROR);
	}
}

void CControlWnd::LoadMaster(bool bFlag)
{
	CFile file;
	CString path;
	path.Format("%s\\tab\\", Variant(homeCC, ""));	

	CString strFuture;
	CString strOption;

	if(bFlag)
	{
		strFuture = "fjcode.dat";
		strOption = "opcode2.dat";
	}
	else
	{
		strFuture = "mfcode.dat";
		strOption = "mocode.dat";
	}

	// 선물종목코드
	if (file.Open(path+strFuture, CFile::modeRead))
	{
		m_fjcode.resize((size_t)(file.GetLength()/sizeof(struct fjcode)) );
		file.Read(&m_fjcode[0], m_fjcode.size() * sizeof(struct fjcode) );
		file.Close();
	}
	else
	{
		if(!bFlag)
			MessageBox("미니선물종목 데이타가 없읍니다.","IBK투자증권",MB_OK|MB_ICONERROR);
	}
	
	if (file.Open(path+strOption, CFile::modeRead))
	{
		m_opcode.resize((size_t)((file.GetLength()-sizeof(struct ojcodh)) / sizeof(struct ojcode)) );
		file.Read(&m_ophead, sizeof(struct ojcodh));
		file.Read(&m_opcode[0], m_opcode.size() * sizeof(struct ojcode));
		file.Close();

		m_dtOption.clear();
		vector<struct ojcode>::iterator st, ed;
		st = m_opcode.begin();
		ed = m_opcode.end();
		for(int n=0;st!=ed; ++st)
		{
			for(int i=0; i<11; ++i)
			{
				if (st->put[i].yorn=='1')
				{
					OptionRow row;
					row.atmg = (st->atmg==1);
					sprintf(row.call_code, "%.8s", st->call[10-i].cod2);
					sprintf(row.put_code, "%.8s", st->put[i].cod2);
					sprintf(row.hsga, "%.3s.%.2s", &st->price[0], &st->price[3]);
				
					m_dtOption[i].push_back(row);
				}
			}
		}
		m_OptionMap.clear();
		for(int n=0; n<11; ++n)
		{
			reverse(m_dtOption[n].begin(), m_dtOption[n].end());
			for(size_t j=0; j<m_dtOption[n].size(); ++j)
			{
				m_OptionMap[ m_dtOption[n][j].call_code ] = OptionIdxRow(n, j+1);
				m_OptionMap[ m_dtOption[n][j].put_code  ] = OptionIdxRow(n, j+1);
				//m_OptionMap[string(st->call[3-i].cod2, 8)] = OptionIdxRow(i, m_dtOption[i].size()-1);
			}
		}
	}
	else
	{
		if(!bFlag)
			MessageBox("미니옵션종목 데이타가 없읍니다.","IBK투자증권",MB_OK|MB_ICONERROR);
	}
}

void CControlWnd::LoadMiniMaster()
{
	CFile file;
	CString path;
	path.Format("%s\\tab\\", Variant(homeCC, ""));
	
	// 선물종목코드
	if (file.Open(path+"mfcode.dat", CFile::modeRead))
	{
		m_mfcode.resize((size_t)(file.GetLength()/sizeof(struct fjcode)) );
		file.Read(&m_fjcode[0], m_fjcode.size() * sizeof(struct fjcode) );
		file.Close();
	}
	
	if (file.Open(path+"mocode.dat", CFile::modeRead))
	{
		m_mocode.resize((size_t)((file.GetLength()-sizeof(struct ojcodh)) / sizeof(struct ojcode)) );
		file.Read(&m_mohead, sizeof(struct ojcodh));
		file.Read(&m_opcode[0], m_opcode.size() * sizeof(struct ojcode));
		file.Close();
		
		m_dtMOption.clear();
		vector<struct ojcode>::iterator st, ed;
		st = m_mocode.begin();
		ed = m_mocode.end();
		for(int n=0;st!=ed; ++st)
		{
			for(int i=0; i<11; ++i)
			{
				if (st->put[i].yorn=='1')
				{
					OptionRow row;
					row.atmg = (st->atmg==1);
					sprintf(row.call_code, "%.8s", st->call[10-i].cod2);
					sprintf(row.put_code, "%.8s", st->put[i].cod2);
					sprintf(row.hsga, "%.3s.%.2s", &st->price[0], &st->price[3]);
					
					m_dtMOption[i].push_back(row);
				}
			}
		}
		m_dtMOption.clear();
		for(int n=0; n<11; ++n)
		{
			reverse(m_dtMOption[n].begin(), m_dtMOption[n].end());
			for(size_t j=0; j<m_dtMOption[n].size(); ++j)
			{
				m_MOptionMap[ m_dtMOption[n][j].call_code ] = OptionIdxRow(n, j+1);
				m_MOptionMap[ m_dtMOption[n][j].put_code  ] = OptionIdxRow(n, j+1);
				//m_OptionMap[string(st->call[3-i].cod2, 8)] = OptionIdxRow(i, m_dtOption[i].size()-1);
			}
		}
	}
}

void CControlWnd::SetChegGrid()
{
	CGridCellBase *pc;
	m_pChegGrid->SetEditable(FALSE);
	m_pChegGrid->EnableSelection(FALSE);
	m_pChegGrid->SetFrameFocusCell(FALSE);
	m_pChegGrid->SetColumnResize(FALSE);
	m_pChegGrid->SetRowResize(FALSE);
	m_pChegGrid->EnableTitleTips(FALSE);
	m_pChegGrid->SetDefCellHeight(16);

	m_pChegGrid->SetRowCount(1);
	m_pChegGrid->SetColumnCount(3);

	pc = m_pChegGrid->GetDefaultCell(FALSE, FALSE);
	if (pc)	{
		pc->SetFormat( DT_VCENTER|DT_SINGLELINE|DT_RIGHT );
		pc->SetMargin( 5 );
	}

	pc = m_pChegGrid->GetDefaultCell(TRUE, FALSE);
	if (pc) {
		
		pc->SetBackClr( GetIndexColor(COLOR_GRIDHEAD) );
		pc->SetTextClr( GetIndexColor(COLOR_GRIDHEADTXT) );
	}
	m_pChegGrid->SetBkColor(m_crWhite);
	m_pChegGrid->SetRowCount(m_dtOption[0].size()+1);
	m_pChegGrid->SetColumnCount(3);
	m_pChegGrid->SetFixedRowCount(1);
	
	char *head[] = { "시간", "체결가", "수량" };
	int  width[] = { 60, 50, 40 };
	for(int n=0; n<3; ++n)
	{
		m_pChegGrid->SetItemText(0, n, head[n]);
		m_pChegGrid->SetItemFormat(0, n, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		m_pChegGrid->SetColumnWidth(n, width[n]);
	}
}


void CControlWnd::SetOptionGrid()
{
	CGridCellBase *pc;
	m_pOptGrid->SetEditable(FALSE);
	m_pOptGrid->EnableSelection(FALSE);
	m_pOptGrid->SetFrameFocusCell(FALSE);
	m_pOptGrid->SetColumnResize(FALSE);
	m_pOptGrid->SetRowResize(FALSE);
	m_pOptGrid->EnableTitleTips(FALSE);
	
	pc = m_pOptGrid->GetDefaultCell(FALSE, FALSE);
	if (pc)	{
		pc->SetFormat( DT_VCENTER|DT_SINGLELINE|DT_RIGHT );
		pc->SetMargin( 5 );
	}
	pc = m_pOptGrid->GetDefaultCell(TRUE, FALSE);
	if (pc) {
		
		pc->SetBackClr( GetIndexColor(COLOR_GRIDHEAD) );
		pc->SetTextClr( GetIndexColor(COLOR_GRIDHEADTXT) );
	}
	m_pOptGrid->SetBkColor(m_crWhite);
	m_pOptGrid->SetRowCount(m_dtOption[0].size()+1);
	m_pOptGrid->SetColumnCount(3);
	m_pOptGrid->SetFixedRowCount(1);

	char *head[] = { "Call", "행사가", "Put" };
	for(int n=0; n<3; ++n)
	{
		m_pOptGrid->SetItemText(0, n, head[n]);
		m_pOptGrid->SetItemFormat(0, n, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
}

void CControlWnd::SetWOptionData(int idx, bool bWeekClick)
{

	OptionRows &rows = m_dtOption[idx];
	CString slog;
//slog.Format("IB300600 SetOptionData [%d]  rows size= [%d]\n",idx, rows.size());
//OutputDebugString(slog);

//옵션버튼 이미지 작업
	for(int n=0; n<4; ++n)
	{
		m_pBtnOption[n]->SetImgBitmap(m_hBtn2[0]->operator HBITMAP(), m_hBtn2[1]->operator HBITMAP(), m_hBtn2[2]->operator HBITMAP());
		m_pBtnOption[n]->Invalidate(FALSE);
	}

	for (int n = 0; n < 4; ++n)
	{
		if (n == idx)
			m_pBtnOption[n]->SetImgBitmap(m_hBtn2[1]->operator HBITMAP(), m_hBtn2[1]->operator HBITMAP(), m_hBtn2[1]->operator HBITMAP());
		else
			m_pBtnOption[n]->SetImgBitmap(m_hBtn2[0]->operator HBITMAP(), m_hBtn2[1]->operator HBITMAP(), m_hBtn2[2]->operator HBITMAP());
		m_pBtnOption[n]->Invalidate(FALSE);
	}

	m_pBtnOption[0]->Invalidate(FALSE);
	m_pBtnOption[1]->Invalidate(FALSE);

	////잔고 보여주기
	//DisplayAllJango();
	
//행사가 관련 그리드 세팅 과정 
//옵션 행사가 배열m_dtOption 참조
	if (rows.empty())
	{
		for(int i=m_pOptGrid->GetRowCount()-1 ; i>0 ; i--)
		{
			m_pOptGrid->DeleteRow(i);
		}
		m_pOptGrid->Refresh();
		return;
	}
	
	m_dtOptionIdx = idx;
	m_pOptGrid->SetRowCount(rows.size()+1);
	int atmRow = -1;
	OptionRows::iterator st, ed;
	st = rows.begin();
	ed = rows.end();
	for(size_t i=0; i<rows.size(); ++i, ++st)
	{
		m_pOptGrid->SetItemText(i+1, 0, "");
		m_pOptGrid->SetItemText(i+1, 1, st->hsga); 
		m_pOptGrid->SetItemText(i+1, 2, "");
		
		m_pOptGrid->SetItemFormat(i+1, 0, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
		m_pOptGrid->SetItemFormat(i+1, 1, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		m_pOptGrid->SetItemFormat(i+1, 2, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
		
		m_pOptGrid->SetItemBkColour(i+1, 1, (st->atmg) ? m_crLine : m_crBodyBk);
		if (st->atmg)
			atmRow = i+1;
	}
	
	if (atmRow>0)
	{
#ifdef _DEF_IB300600_
		m_pOptGrid->SetScrollPos(SB_VERT, m_pOptGrid->GetDefCellHeight()*(atmRow-4), FALSE);
#else
		m_pOptGrid->SetScrollPos(SB_VERT, m_pOptGrid->GetDefCellHeight()*(atmRow-6), FALSE);
#endif
		m_pOptGrid->PostMessage(WM_VSCROLL, 0, 0);
	}

	//잔고 보여주기
	DisplayAllJango();  //fix
	
	m_pOptGrid->Refresh();

	//if (m_iKind == WEEKBTN)
	//{
	//	if (idx == 0)  //월요일 만기 위클리
	//	{
	//		if(m_sArrMWeek.GetSize() > 0)
	//			GetParent()->PostMessage(WM_APP_SIG, WP_CODE_CHANGE, (LPARAM)(LPCSTR)m_sArrMWeek.GetAt(0));
	//	}
	//	else if (idx == 1)
	//	{
	//		if(m_sArrTWeek.GetSize() > 0)
	//			GetParent()->PostMessage(WM_APP_SIG, WP_CODE_CHANGE, (LPARAM)(LPCSTR)m_sArrTWeek.GetAt(0));
	//	}
	//	else if (idx == 2)  //월요일 만기 위클리
	//	{
	//		if (m_sArrMWeek.GetSize() > 1)
	//			GetParent()->PostMessage(WM_APP_SIG, WP_CODE_CHANGE, (LPARAM)(LPCSTR)m_sArrMWeek.GetAt(1));
	//	}
	//	else if (idx == 3)
	//	{
	//		if (m_sArrTWeek.GetSize() > 1)
	//			GetParent()->PostMessage(WM_APP_SIG, WP_CODE_CHANGE, (LPARAM)(LPCSTR)m_sArrTWeek.GetAt(1));
	//	}
	//}
}

void CControlWnd::SetOptionData( int idx)
{
CString slog;
//slog.Format("IB300600 SetOptionData");
//OutputDebugString(slog);
	if(m_iKind == WEEKBTN)
	{
		SetWOptionData(idx);
	}
	else
	{
		OptionRows &rows = m_dtOption[idx];
		
	//slog.Format("IB300600 SetOptionData [%d]  rows size= [%d]\n",idx, rows.size());
	//OutputDebugString(slog);
		for(int n=0; n<4; ++n)
		{
			if (n==idx)
				m_pBtnOption[n]->SetImgBitmap(m_hBtn2[1]->operator HBITMAP(), m_hBtn2[1]->operator HBITMAP(), m_hBtn2[1]->operator HBITMAP());
			else
				m_pBtnOption[n]->SetImgBitmap(m_hBtn2[0]->operator HBITMAP(), m_hBtn2[1]->operator HBITMAP(), m_hBtn2[2]->operator HBITMAP());
			m_pBtnOption[n]->Invalidate(FALSE);
		}
		
		if(idx >= 3)
		{
			m_pBtnOption[3]->SetImgBitmap(m_hBtn2[1]->operator HBITMAP(), m_hBtn2[1]->operator HBITMAP(), m_hBtn2[1]->operator HBITMAP());
			m_pBtnOption[3]->Invalidate(FALSE);
		}
		
		if (rows.empty())
		{
			for(int i=m_pOptGrid->GetRowCount()-1 ; i>0 ; i--)
			{
				m_pOptGrid->DeleteRow(i);
			}
			m_pOptGrid->Refresh();
			return;
		}
		
		m_dtOptionIdx = idx;
		m_pOptGrid->SetRowCount(rows.size()+1);
		int atmRow = -1;
		OptionRows::iterator st, ed;
		st = rows.begin();
		ed = rows.end();
		for(size_t i=0; i<rows.size(); ++i, ++st)
		{
			m_pOptGrid->SetItemText(i+1, 0, "");
			m_pOptGrid->SetItemText(i+1, 1, st->hsga); 
			m_pOptGrid->SetItemText(i+1, 2, "");
			
			m_pOptGrid->SetItemFormat(i+1, 0, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
			m_pOptGrid->SetItemFormat(i+1, 1, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pOptGrid->SetItemFormat(i+1, 2, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
			
			m_pOptGrid->SetItemBkColour(i+1, 1, (st->atmg) ? m_crLine : m_crBodyBk);
			if (st->atmg)
				atmRow = i+1;
		}
		
		if (atmRow>0)
		{
#ifdef _DEF_IB300600_
			m_pOptGrid->SetScrollPos(SB_VERT, m_pOptGrid->GetDefCellHeight()*(atmRow-4), FALSE);
#else
			m_pOptGrid->SetScrollPos(SB_VERT, m_pOptGrid->GetDefCellHeight()*(atmRow-6), FALSE);
#endif
			m_pOptGrid->PostMessage(WM_VSCROLL, 0, 0);
		}
		
		m_pOptGrid->Refresh();
		
		DisplayAllJango();
	}
}

//2015.10.12 KSJ 선물도 선택되면 버튼이 눌려지도록 추가
void CControlWnd::SetFutureData( int idx )
{
	// 	CString slog;
	// 	slog.Format("SetOptionData [%d]\n",idx);
	// 	OutputDebugString(slog);
	for(int n=0; n<4; ++n)
	{
		if (n==idx)
			m_pBtnFuture[n]->SetImgBitmap(m_hBtn2[1]->operator HBITMAP(), m_hBtn2[1]->operator HBITMAP(), m_hBtn2[1]->operator HBITMAP());
		else
			m_pBtnFuture[n]->SetImgBitmap(m_hBtn2[0]->operator HBITMAP(), m_hBtn2[1]->operator HBITMAP(), m_hBtn2[2]->operator HBITMAP());
		m_pBtnFuture[n]->Invalidate(FALSE);
	}
	
	if(idx >= 3)
	{
		m_pBtnFuture[3]->SetImgBitmap(m_hBtn2[1]->operator HBITMAP(), m_hBtn2[1]->operator HBITMAP(), m_hBtn2[1]->operator HBITMAP());
		m_pBtnFuture[3]->Invalidate(FALSE);
	}
}

void CControlWnd::OnFutBtnClick( UINT nID )
{
	CString slog;
//	slog.Format("OnFutBtnClick");
// 	OutputDebugString(slog);
	int idx = nID - IDC_BTN_FUT1;

//slog.Format("------IB300600 [CControlWnd::OnFutBtnClick] %d\n", idx);
//OutputDebugString(slog);

	if(m_iKind == WEEKBTN)
	{
		m_bClickInWeek = true;
	}
	else
	{
		m_bClickInWeek = false;
	}

	if(idx < 4)
	{
		if( idx == 3)	//2015.12.01 KSJ 선택된거 저장
		{
			CString code(m_fjcode[m_nCurFSelect].cod2, sizeof(m_fjcode[m_nCurFSelect].cod2));
			GetParent()->SendMessage(WM_APP_SIG, WP_CODE_CHANGE, (LPARAM)(LPCSTR)code);
			SetFutureData(m_nCurFSelect);
		}
		else
		{	
			CString code(m_fjcode[idx].cod2, sizeof(m_fjcode[idx].cod2));
			GetParent()->SendMessage(WM_APP_SIG, WP_CODE_CHANGE, (LPARAM)(LPCSTR)code);
			SetFutureData(idx);	
		}
	}
	else
	{
		CMenu cMenu;
		cMenu.CreatePopupMenu();
		
		CString strCode;
		
		m_pBtnFuture[3]->GetWindowText(strCode);
		
		int jj = 0;
		
		for (int ii = 3; ii < 7; ii++)
		{
			CString code(m_fjcode[ii].cod2, sizeof(m_fjcode[ii].cod2));
			
			if(code.GetAt(0) == '4' || code.GetAt(0) == 'D') //파생상품 코드개편
				break;
			
			CString name(m_fjcode[ii].hnam, sizeof(m_fjcode[ii].hnam));
			name.TrimRight();
			
			name = name.Right(4);
			
			if (!strCode.CompareNoCase(name))
				cMenu.AppendMenu(MFT_STRING|MF_CHECKED, jj+1, name);
			else
				cMenu.AppendMenu(MFT_STRING, jj+1, name);
			
			jj++;
		}
		
		CPoint	point;
		GetCursorPos(&point);
		int ret = cMenu.TrackPopupMenu(TPM_RIGHTBUTTON|TPM_RETURNCMD, point.x, point.y, this, NULL);
		if (ret > 0)
		{
			strCode = CString(m_fjcode[ret+2].cod2,sizeof(m_fjcode[ret+2].cod2));
			
			GetParent()->SendMessage(WM_APP_SIG, WP_CODE_CHANGE, (LPARAM)(LPCSTR)strCode);
			
			SetFutureData(ret + 2);
			
			CString strNam(m_fjcode[ret+2].hnam,sizeof(m_fjcode[ret+2].hnam));
			
			strNam.TrimRight();
			
			strNam = strNam.Right(4);
			
			m_pBtnFuture[3]->SetWindowText(strNam);
			
			m_pBtnFuture[3]->Invalidate();
			
			m_nCurFSelect = ret + 2;
		}
	}	
}

void CControlWnd::OnPutBtnClick( UINT nID )
{
	int idx = nID-IDC_BTN_OPT1;

CString slog;
//slog.Format("------IB300600 [CControlWnd::OnPutBtnClick] %d\n", idx);
//OutputDebugString(slog);

	if(idx<4)
	{
		//2015.12.01 KSJ 선택된거 저장
		if (idx == 3)	SetOptionData(m_nCurOSelect);
		else			SetOptionData(idx);
	}
	else
	{
		CMenu cMenu;
		cMenu.CreatePopupMenu();
		
		CString strCode;
		
		m_pBtnOption[3]->GetWindowText(strCode);
		
		int jj = 0;
		
		for (int ii = 3; ii < 11; ii++)
		{
			CString str(m_ophead.pjym[ii], sizeof(m_ophead.pjym[ii]));
			str.TrimRight();
			
			if( str == "000000" )
				continue;

			if (!strCode.CompareNoCase(str.Right(4)))
				cMenu.AppendMenu(MFT_STRING|MF_CHECKED, jj+1, str.Right(4));
			else
				cMenu.AppendMenu(MFT_STRING, jj+1, str.Right(4));
			
			jj++;
		}
		
		CPoint	point;
		GetCursorPos(&point);
		int ret = cMenu.TrackPopupMenu(TPM_RIGHTBUTTON|TPM_RETURNCMD, point.x, point.y, this, NULL);
		if (ret > 0)
		{
			SetOptionData(ret+2);

			CString strNam(m_ophead.pjym[ret+2],sizeof(m_ophead.pjym[ret+2]));
			
			strNam.TrimRight();
			
			strNam = strNam.Right(4);
			
			m_pBtnOption[3]->SetWindowText(strNam);
			
			m_pBtnOption[3]->Invalidate();

			m_nCurOSelect = ret + 2;	//2015.12.01 KSJ 설정 저장
		}
	}
}

void CControlWnd::LoadConfig()
{
	int def_val[][4] = {{1, 2, 3, 5},{10, 20, 30, 50},{100, 200, 300, 500}};
	
	int ival;
	CString key, sval;
	for(int row=0; row<3; ++row)
	{
		for(int col=0; col<4; ++col)
		{
			key.Format("MA_%d_%d", row, col);
			ival = GetPrivateProfileInt("JQTY_GRID", (LPCSTR)key, def_val[row][col], (LPCSTR)m_profile);
			sval.Format("%d", ival);
			m_pJcntGrid->SetItemText(row, col, (LPCSTR)sval );
		}
	}

	int iSel = GetPrivateProfileInt("TAB1", "SELECT", 0, (LPCSTR)m_profile);
	m_pTab->SetCurSel(iSel);

	NMHDR hdr;
	hdr.idFrom = IDC_CTL_TAB1;
	OnTabChange(&hdr, NULL);

	CString	Path;
	Path.Format("%s\\tab\\ACCNTDEPT.INI", Variant(homeCC,""));
	
	char readB[1024];
	int readL;
	
	readL = GetPrivateProfileString("ACCNTDEPT","DEPT"," ",readB,sizeof(readB),Path);
	
	CString tDept(readB,readL);
	tDept.TrimLeft();tDept.TrimRight();
	m_sAccnDept = tDept;
}

int CControlWnd::GetJqty()
{
	return Str2Int(m_pSpJcnt->GetText());
}

int CControlWnd::GetCdgb()
{
	if      (m_rdNone.GetCheck()) return  0;
	else if (m_rdIOC.GetCheck())  return  1;
	else if (m_rdFOK.GetCheck())  return  2;
	else                          return -1;
}

LRESULT CControlWnd::OnRefresh( WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case WP_JANGO: 
		DisplayJango((Jango*)lParam);
		break;
	case WP_MICHEG: 
		DisplayMicheg((LPCSTR)lParam);
		break;
	case WP_CTRL2_FOLD_DISABLE: 
		m_pBtnFold->ShowWindow( ((BOOL)lParam) ? SW_HIDE : SW_SHOW );
		m_pBtnFold->EnableWindow( !(BOOL)lParam );

		if(nSizeMode == SIZE_GENERAL)
		{
			CalcSize();
		}
		else
		{
			CalcSizeJango();
		}
		break;
	case WP_FOLDSTAT:
		m_pBtnFold->SetWindowText( ((BOOL)lParam) ? ">>" : "<<" );
		break;
	}

	return 0;
}

void CControlWnd::DisplayJango( Jango *pj )
{
	CString text, tmp;
	AccountMap::iterator st, ed;
	
	int wcnt = 0, jqty = 0;	// 미체결 건수, 잔고수량
	
	// 잔고수량 계산
	CString stemp;
	st = m_pAccount->begin();
	ed = m_pAccount->end();
	for(; st!=ed; ++st)
	{
		CodeJangoMap::iterator pos = st->second.jango.find(pj->code);
		if (pos != st->second.jango.end())
		{
			jqty += pos->second.jqty;
			stemp = pos->second.gubn;
		}
	}
	
	// 미체결건수 계산
	st = m_pAccount->begin();
	ed = m_pAccount->end();
	for(; st!=ed; ++st)
	{
		CodeMichegMap::iterator pos = st->second.micheg.find(pj->code);
		if (pos != st->second.micheg.end())
		{
			PriceMichegMap::iterator st2 = pos->second.begin();
			PriceMichegMap::iterator ed2 = pos->second.end();
			for(; st2!=ed2; ++st2)
				wcnt += st2->second.d_cnt + st2->second.s_cnt;
		}
	}
	//wcnt  jqty // 미체결 건수, 잔고수량
	if (jqty<=0 && wcnt<=0) text = "";
	/*else if (jqty>0 && wcnt<=0) text.Format("%d", jqty);
	else if (jqty>0 && wcnt>0)	text.Format("%d(%d)", jqty, wcnt);
	else if (jqty<=0 && wcnt>0) text.Format("(%d)", wcnt);*/
	else if (jqty > 0 && wcnt <= 0)
	{
		if(stemp.Find("매도") >= 0)
			text.Format("-%d", jqty);
		else
			text.Format("%d", jqty);
	}
	else if (jqty > 0 && wcnt > 0)
	{
		if (stemp.Find("매도") >= 0)
			text.Format("-%d(%d)", jqty, wcnt);
		else
			text.Format("%d(%d)", jqty, wcnt);
	}
	else if (jqty <= 0 && wcnt > 0)
	{
		text.Format("(%d)", wcnt);
	}
	else return;		// -_-;

 	/*CString slog;
 	slog.Format("stemp=[%s] text=[%s] JQTY [%d] WCNT [%d]  [%s]\n", stemp, text,jqty,wcnt, pj->code);
 	OutputDebugString(slog);*/
	
	//TRACE("\nControl Jango -> %d %d\n", jqty, wcnt);

	if (pj->code[0]=='1' || pj->code[0] == 'A')  //파생상품 코드개편
	{
		int cnt = (int)m_fjcode.size();
		for(int n=0; n<cnt; ++n)
		{
			if (memcmp(pj->code, m_fjcode[n].cod2, 8)==0)
			{
				if(n < 3)
				{
					if (m_pStFuture[n]->GetText()!=text)
						m_pStFuture[n]->SetText(text);
				}
				else
				{
					if (m_pStFuture[3]->GetText()!=text)
						m_pStFuture[3]->SetText(text);
				}
				break;
			}
		}
	}
	else
	{
		OptionIdxMap::iterator pos = m_OptionMap.find(pj->code) ;
		if (pos != m_OptionMap.end())
		{
			if (m_dtOptionIdx==pos->second.first)
			{
				CCellID id;
				id.row = pos->second.second;
				
				if (pj->code[0] == '2' || pj->code[0] == 'B')   //파생상품 코드개편
					id.col = 0;
				else if (pj->code[0] == '3' || pj->code[0] == 'C')
					id.col = 2;
				else 
					return;
				
				if (m_pOptGrid->GetItemText(id.row, id.col)!=text)
				{
					if (stemp.Find("매수") >= 0 )
					{
						m_pOptGrid->SetItemFgColour(id.row, id.col, RGB(255, 0, 0));
						m_pOptGrid->SetItemText(id.row, id.col, text);
						m_pOptGrid->RedrawCell(id);
					}
					else if (stemp.Find("매도") >= 0 )
					{
						m_pOptGrid->SetItemFgColour(id.row, id.col, RGB(0, 0, 255));
						m_pOptGrid->SetItemText(id.row, id.col, text);
						m_pOptGrid->RedrawCell(id);
					}
					else
					{
						m_pOptGrid->SetItemText(id.row, id.col, text);
						m_pOptGrid->RedrawCell(id);
					}
				
				}
			}
		}
		}
}

void CControlWnd::DisplayAllJango()
{
	Jango jngo;
	set<string> codes;

	AccountMap::iterator st, ed;
	st = m_pAccount->begin();
	ed = m_pAccount->end();
	for(; st!=ed; ++st)
	{
		CodeJangoMap::iterator st2, ed2;
		st2 = st->second.jango.begin();
		ed2 = st->second.jango.end();
		for(; st2!=ed2; ++st2)
			codes.insert(st2->first);
	}

	set<string>::iterator st3 = codes.begin();
	set<string>::iterator ed3 = codes.end();
	for(; st3!=ed3; ++st3)
	{
		sprintf(jngo.code, st3->c_str());
		DisplayJango(&jngo);
	}
}

void CControlWnd::DisplayMicheg( LPCSTR code )
{
// 	CString s;
// 	s.Format("DIS MCHG [%s]\n",code);
// 	OutputDebugString(s);

	Jango jngo;
	if (code && strlen(code)==8)
	{
		sprintf(jngo.code, code);
		DisplayJango(&jngo);
	}
	else
	{
		set<string> tmp;

		AccountMap::iterator st, ed;
		st = m_pAccount->begin();
		ed = m_pAccount->end();
		for(; st!=ed; ++st)
		{
			CodeMichegMap::iterator cst, ced;
			cst = st->second.micheg.begin();
			ced = st->second.micheg.end();
			for(; cst!=ced; ++cst)
			{
				//MessageBox(cst->first.c_str());
				tmp.insert(cst->first);
			}
		}

		set<string>::iterator tst, ted;
		tst = tmp.begin();
		ted = tmp.end();
		for(; tst!=ted; ++tst)
		{
			sprintf(jngo.code, tst->c_str());
			DisplayJango(&jngo);
		}
	}
}

int CControlWnd::GetMulti()
{
	if (m_chkMulti.GetCheck() && m_pSpMulti->IsWindowEnabled())
		return atoi((LPCSTR)m_pSpMulti->GetText());
	else
		return 0;
}

void CControlWnd::Clear()
{
	int n;
	for(n=0; n<4; ++n)
		m_pStFuture[n]->SetText("");
	for(n=1; n<m_pOptGrid->GetRowCount(); ++n)
	{
		m_pOptGrid->SetItemText(n, 0, ""); m_pOptGrid->RedrawCell(n, 0);
		m_pOptGrid->SetItemText(n, 2, ""); m_pOptGrid->RedrawCell(n, 2);
	}
}

int CControlWnd::GetRsvd()
{
	return atoi((LPCSTR)m_pSpRsvd->GetText());
}

void CControlWnd::ChangeTheme()
{
	AxInit();
	m_pStJumun->SetBkColor(GetIndexColor(COLOR_GRIDHEAD), false);
	m_pStJumun->SetFgColor(GetIndexColor(COLOR_GRIDHEADTXT), false);
	m_pStMstr->SetBkColor(GetIndexColor(COLOR_GRIDHEAD), false);
	m_pStMstr->SetFgColor(GetIndexColor(COLOR_GRIDHEADTXT), false);
	SetOptionGrid();
	SetJvolGrid();

	for(int n=0; n<m_pChegGrid->GetColumnCount(); ++n)
	{
		m_pChegGrid->SetItemBkColour(0, n, GetIndexColor(COLOR_GRIDHEAD));
		m_pChegGrid->SetItemFgColour(0, n, GetIndexColor(COLOR_GRIDHEADTXT));
	}

	m_pTab->LoadTabImage(Variant(homeCC, ""), false);

	Invalidate(TRUE);
}

void CControlWnd::OnBtnFold()
{
	CString txt;
	m_pBtnFold->GetWindowText(txt);
	if (txt=="<<")
	{
		m_pBtnFold->SetWindowText(">>");
		GetParent()->SendMessage(WM_APP_SIG, WP_CTRL2_FOLD, (LPARAM)TRUE);
	}
	else if (txt==">>")
	{
		m_pBtnFold->SetWindowText("<<");
		GetParent()->SendMessage(WM_APP_SIG, WP_CTRL2_FOLD, (LPARAM)FALSE);
	}

}

void CControlWnd::OnChkOneClick()
{
	if(m_chkOneClick.GetCheck())
	{
		MessageBox("원클릭주문 설정 시 한번의 클릭으로 주문이 접수되므로 이용에 유의하시기 바랍니다." , "IBK투자증권", MB_OK);
		WritePrivateProfileString("ORDER", "ONECLICK", (LPCSTR)"1", (LPCSTR)m_profile);
	}
	else
		WritePrivateProfileString("ORDER", "ONECLICK", (LPCSTR)"0", (LPCSTR)m_profile);

	GetParent()->SendMessage(WM_APP_SIG, WP_ONECLICK_MODE, (LPARAM)m_chkOneClick.GetCheck());
}

void CControlWnd::SetJvolGrid()
{
	m_pJvolGrid->SetEditable(FALSE);
	m_pJvolGrid->EnableSelection(FALSE);
	m_pJvolGrid->SetFrameFocusCell(FALSE);
	m_pJvolGrid->SetFont(m_pFont);
	m_pJvolGrid->SetColumnResize(FALSE);
	m_pJvolGrid->SetRowResize(FALSE);
	m_pJvolGrid->ShowScrollBar(SB_BOTH, FALSE);
	m_pJvolGrid->SetDefCellHeight(GRID_CELL_HEIGHT);

	m_pJvolGrid->SetRowCount(3);
	m_pJvolGrid->SetColumnCount(3);
	m_pJvolGrid->SetFixedRowCount(1);
	m_pJvolGrid->SetFixedColumnCount(1);

	COLORREF row_cols[3] = { GetIndexColor(COLOR_GRIDHEAD), m_crMadoBk, m_crMasuBk};
	for(int row=0; row<3; ++row)
	{
		for(int col=0; col<m_pJvolGrid->GetColumnCount(); ++col)
		{
			m_pJvolGrid->SetItemBkColour(row, col, row_cols[row]);
			if (row==0)
				m_pJvolGrid->SetItemFgColour(row, col, GetIndexColor(COLOR_GRIDHEADTXT));
			else
				m_pJvolGrid->SetItemFgColour(row, col, m_crNormal);

			if (row!=0 && col!=0)
				m_pJvolGrid->SetItemFormat(row, col, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
			else
				m_pJvolGrid->SetItemFormat(row, col, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
	}

	m_pJvolGrid->SetItemText(0, 1, "신 규");
	m_pJvolGrid->SetItemText(0, 2, "청 산");
	m_pJvolGrid->SetItemText(1, 0, "매 도");
	m_pJvolGrid->SetItemText(2, 0, "매 수");
}

void CControlWnd::SetJvolPrice( int jprc )
{
	CString sdat;
	sdat.Format("%d.%02d", jprc/100, abs(jprc%100));
	if (m_pEdJprc)
		m_pEdJprc->SetText(sdat);

	if (m_chkAutoJvol.GetCheck())
		req_SONBQ101();
}

void CControlWnd::SetJcnt( int jcnt, BOOL bPower )
{
	//2017.01.19 KSJ m_pJcntGrid에서 선택했을 때 그리드 선택된 것도 초기화
	if (!bPower &&jcnt<=0) return;

	char buff[16];
	sprintf(buff, "%d", jcnt);
	m_pSpJcnt->SetText(buff);

	for(int row=0; row<m_pJcntGrid->GetRowCount(); ++row)
	{
		for(int col=0; col<m_pJcntGrid->GetColumnCount(); ++col)
		{
			m_pJcntGrid->SetItemBkColour(row, col, m_crWhite);
			m_pJcntGrid->SetItemFgColour(row, col, m_crBlack);
		}
	}

	m_pJcntGrid->SetFocusCell(-1, -1);

	m_pJcntGrid->Invalidate(FALSE);
}

void CControlWnd::req_SONBQ101()
{
	if (m_pAccount->empty()) return;

	for(int row=1; row<m_pJvolGrid->GetRowCount(); ++row)
	{
		for(int col=1; col<m_pJvolGrid->GetColumnCount(); ++col)
		{
			m_pJvolGrid->SetItemText(row, col, "");
			m_pJvolGrid->RedrawCell(row, col);
		}
	}

	vector<char> data( L_ledgerH + sizeof(SONBQ101_mid), ' ' );
	
	CString OdrPrc = m_pEdJprc->GetText();
	OdrPrc.Replace(".", "");	
	int dOdrPrc = atoi(OdrPrc);
	OdrPrc.Format("+%09d.%02d", dOdrPrc/100, dOdrPrc%100);
	
	struct _ledgerH *ledger = (struct _ledgerH*)&data[0];
	SONBQ101_mid *mid = (SONBQ101_mid*)&data[L_ledgerH];
	
	GetLedger(ledger);
	CopyMemory(ledger->svcd, "SONBQ101", 8);
	CopyMemory(ledger->rcnt, "0000", 4);
	CopyMemory(ledger->mkty, "2", 1);
	CopyMemory(ledger->odrf, "2", 1);
	
	CopyMemory(mid->In, "00001", 5);
	CopyMemory(mid->zQryTp, "1", 1);
	CopyMemory(mid->lOrdAmt, "+000000000000000", 16);
	CopyMemory(mid->dRatVal, "+000000000.00000000", 19);
	CopyMemory(mid->zFnoIsuNo, m_pSiseData->code, m_pSiseData->code.GetLength());
	CopyMemory(mid->dOrdPrc, (LPCSTR)OdrPrc, 13);
	CopyMemory(mid->zFnoOrdprcPtnCode, "00", 2);
	
	AccountMap::iterator st = m_pAccount->begin();
	AccountMap::iterator ed = m_pAccount->end();
	for(;st!=ed;++st)
	{
		string acno = st->first;
		string pswd = (LPCSTR)st->second.pswd;
		FillMemory(mid->zAcntNo, sizeof(mid->zAcntNo), ' ');
		CopyMemory(mid->zAcntNo, acno.c_str(), acno.size());
		FillMemory(mid->zPwd, sizeof(mid->zPwd), ' ');
		CopyMemory(mid->zPwd, pswd.c_str(), pswd.size());
	
		// 매도
		CopyMemory(mid->zBnsTp, "1", 1);
		SendTR("piboPBxQ", US_ENC, &data[0], data.size(), TK_SONBQ101);

		// 매수
		CopyMemory(mid->zBnsTp, "2", 1);
		SendTR("piboPBxQ", US_ENC, &data[0], data.size(), TK_SONBQ101);
	}
}

LRESULT CControlWnd::OnUser( WPARAM wParam, LPARAM lParam )
{
	int msg = LOBYTE(LOWORD(wParam));
	int key = HIBYTE(LOWORD(wParam));
	int len = HIWORD(wParam);
	LPCSTR dat = (LPCSTR)lParam;
	switch(msg)
	{
	case DLL_OUB:
		switch(key)
		{
		case TK_SONBQ101:
			res_SONBQ101(dat, len);
			break;
		}
	}
	return 0;	
}

void CControlWnd::res_SONBQ101( LPCSTR data, int dlen )
{
	struct _ledgerH *ledger = (struct _ledgerH*)data;
	SONBQ101_mod *mod = (SONBQ101_mod*)(data+L_ledgerH);

	int tmp, row;

	if      (mod->mid.zBnsTp[0]=='1') row = 1;
	else if (mod->mid.zBnsTp[0]=='2') row = 2;
	else return;

	CString OrdAbleQty(mod->lOrdAbleQty, sizeof(mod->lOrdAbleQty));
	CString NewAbleQty(mod->lNewOrdAbleQty, sizeof(mod->lNewOrdAbleQty));
	CString LqdtAbleQty(mod->lLqdtOrdAbleQty, sizeof(mod->lLqdtOrdAbleQty));
	
	tmp = atoi(OrdAbleQty); OrdAbleQty = Int2CommaStr(tmp);
	tmp = atoi(NewAbleQty); NewAbleQty = Int2CommaStr(tmp);
	tmp = atoi(LqdtAbleQty); LqdtAbleQty = Int2CommaStr(tmp);

	//MessageBox(OrdAbleQty + "|" + NewAbleQty + "|" + LqdtAbleQty);
	CString emsg(ledger->emsg, sizeof(ledger->emsg));
	GetParent()->SendMessage(WM_USER, DLL_GUIDE, (LPARAM)(LPCSTR)emsg);

	m_pJvolGrid->SetItemText(row, 1, NewAbleQty);
	m_pJvolGrid->SetItemText(row, 2, LqdtAbleQty);
	m_pJvolGrid->RedrawRow(row);
}

void CControlWnd::OnBtnJvol()
{
	req_SONBQ101();
}

void CControlWnd::OnTabChange( NMHDR *pHeader, LRESULT *pResult )
{
	if (pHeader->idFrom==IDC_CTL_TAB1 && m_pTab)
	{
		m_nCurrTab = m_pTab->GetCurSel();

		int nCmd1 = SW_SHOW, nCmd2 = SW_SHOW, nCmd3 = SW_SHOW;
		if (m_pTab->GetCurSel()==0)
		{
			nCmd1 = SW_SHOW;
			nCmd2 = SW_HIDE;
			nCmd3 = SW_HIDE;
			m_bChegUpdate = FALSE;

			m_pScreenWnd->closeMap();
		}
		else if (m_pTab->GetCurSel()==2) 
		{
			nCmd1 = SW_HIDE;
			nCmd2 = SW_SHOW;
			nCmd3 = SW_HIDE;
			m_bChegUpdate = TRUE;

			m_pScreenWnd->closeMap();
		}
		else if (m_pTab->GetCurSel() == 1)
		{
			if(m_bCurrView)
			{
				nCmd1 = SW_HIDE;
				nCmd2 = SW_HIDE;
				nCmd3 = SW_SHOW;
				m_bChegUpdate = FALSE;

				m_pScreenWnd->ChangeMap("IB401399", "");
			}
			else
			{
				nCmd1 = SW_HIDE;
				nCmd2 = SW_SHOW;
				nCmd3 = SW_HIDE;
				m_bChegUpdate = TRUE;
				
				m_pScreenWnd->closeMap();
			}
			//m_pScreenWnd->ShowWindow(SW_SHOW);
		}
		else
		{
			return;
		}

		{
			CString strVal;
			strVal.Format("%d", m_pTab->GetCurSel());
			WritePrivateProfileString("TAB1", "SELECT", (LPCSTR)strVal, (LPCSTR)m_profile);
		}

		for(int n=0; n<4; ++n)
		{
			if (m_pStFuture[n])  m_pStFuture[n]->ShowWindow(nCmd1);
			if (m_pBtnFuture[n])
			{
				m_pBtnFuture[n]->ShowWindow(nCmd1);
			}
			if (m_pBtnOption[n]) m_pBtnOption[n]->ShowWindow(nCmd1);
			if (m_pOptGrid) m_pOptGrid->ShowWindow(nCmd1);

			if(n==3)
			{
				m_pBtnFuture[4]->ShowWindow(nCmd1);
				m_pBtnOption[4]->ShowWindow(nCmd1);
			}
		}

		m_pBtnGen->ShowWindow(nCmd1);
		m_pBtnMini->ShowWindow(nCmd1);
		m_pBtnWeek->ShowWindow(nCmd1);

		m_pChegGrid->ShowWindow(nCmd2);
		m_pBtnChegLink->ShowWindow(nCmd2);

		m_pSise->ShowWindow(nCmd3);
		m_pScreenWnd->ShowWindow(nCmd3);

		//m_pBtnFuture[4]->ShowWindow(nCmd1);
		if(m_iKind == WEEKBTN)
			SetWeelyBtnSHOW();
	}
}

void CControlWnd::SetRealData( RealData *rp )
{
	RealData::iterator p = rp->find(34);
	if (p!=rp->end())
	{
		CString time;
		time.Format("%.2s:%.2s:%.2s", p->second.first, p->second.first+2, p->second.first+4);
		CString curr = rp->find(23)->second.ToCStr();
		CString cvol = rp->find(32)->second.ToCStr();
		
		if (m_pChegGrid)
		{
			m_pChegGrid->InsertRow(NULL, 1);
			
			m_pChegGrid->SetItemFormat(1, 0, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
			m_pChegGrid->SetItemText(1, 0, time);
			
			switch(curr.GetAt(0))
			{
			case '+': m_pChegGrid->SetItemFgColour(1, 1, m_crPlus); break;
			case '-': m_pChegGrid->SetItemFgColour(1, 1, m_crMinus); break;
			default : m_pChegGrid->SetItemFgColour(1, 1, m_crBlack); break;
			}
			m_pChegGrid->SetItemText(1, 1, curr.Mid(1));

			switch(cvol.GetAt(0))
			{

			case '+':
				m_pChegGrid->SetItemFgColour(1, 2, m_crPlus);
				m_pChegGrid->SetItemBkColour(1, 2, m_crWhite);
				break;
			case '-':
				m_pChegGrid->SetItemFgColour(1, 2, m_crMinus);
				m_pChegGrid->SetItemBkColour(1, 2, m_crWhite);
				break;
			case '1': 
				if(m_bFocusCheg)
				{
					m_pChegGrid->SetItemFgColour(1, 2, m_crWhite);
					m_pChegGrid->SetItemBkColour(1, 2, m_crPlus);
				}
				break;
			case '4':
				if(m_bFocusCheg)
				{
					m_pChegGrid->SetItemFgColour(1, 2, m_crWhite);
					m_pChegGrid->SetItemBkColour(1, 2, m_crMinus);
				}
				break;
			}
			m_pChegGrid->SetItemText(1, 2, cvol.Mid(1));
			if (m_bChegUpdate)
				m_pChegGrid->Refresh();

			if (m_pChegGrid->GetRowCount()>200)
				m_pChegGrid->SetRowCount(200);
		}
	}
}

void CControlWnd::SetRealData( DWORD* data )
{
	if (data[34])
	{
		CString time = (char*)data[34];

		if(time.GetLength() > 5)
		{
			time.Insert(2,':');
			time.Insert(5,':');
		}

		CString curr = (char*)data[23];
		CString cvol = (char*)data[32];
		
		if(m_bChegView)
		{
			if (m_pChegGrid)
			{
				m_pChegGrid->InsertRow(NULL, 1);
				
				m_pChegGrid->SetItemFormat(1, 0, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
				m_pChegGrid->SetItemText(1, 0, time);
				
				switch(curr.GetAt(0))
				{
				case '+': m_pChegGrid->SetItemFgColour(1, 1, m_crPlus); break;
				case '-': m_pChegGrid->SetItemFgColour(1, 1, m_crMinus); break;
				default : m_pChegGrid->SetItemFgColour(1, 1, m_crBlack); break;
				}
				m_pChegGrid->SetItemText(1, 1, curr.Mid(1));
				
				switch(cvol.GetAt(0))
				{
					
				case '+':
					m_pChegGrid->SetItemFgColour(1, 2, m_crPlus);
					m_pChegGrid->SetItemBkColour(1, 2, m_crWhite);
					break;
				case '-':
					m_pChegGrid->SetItemFgColour(1, 2, m_crMinus);
					m_pChegGrid->SetItemBkColour(1, 2, m_crWhite);
					break;
				case '1': 
					if(m_bFocusCheg)
					{
						m_pChegGrid->SetItemFgColour(1, 2, m_crWhite);
						m_pChegGrid->SetItemBkColour(1, 2, m_crPlus);
					}
					break;
				case '4':
					if(m_bFocusCheg)
					{
						m_pChegGrid->SetItemFgColour(1, 2, m_crWhite);
						m_pChegGrid->SetItemBkColour(1, 2, m_crMinus);
					}
					break;
				}
				m_pChegGrid->SetItemText(1, 2, cvol.Mid(1));

				if (m_bChegUpdate)
					m_pChegGrid->Refresh();
				
				if (m_pChegGrid->GetRowCount()>200)
					m_pChegGrid->SetRowCount(200);
			}
		}

		m_pSise->SetRealData(data);
	}
}

void CControlWnd::SetData(SiseData* data)
{
	m_pSise->SetData(data);
}

void CControlWnd::ClearChegGrid()
{
	SetChegGrid();
}

void CControlWnd::OnChegLink()
{
	CString opt = MAP_SISECHEG;
	SetView(opt);
}

void CControlWnd::OnChkAuto()
{
// 	if(m_chkAuto.GetCheck() == TRUE)
// 	{
// 		m_pSpAuto->EnableWindow(TRUE);
// 	}
// 	else
// 	{
// 		m_pSpAuto->EnableWindow(FALSE);
// 	}
}

void CControlWnd::OnChkLoss()
{
// 	if(m_chkLoss.GetCheck() == TRUE)
// 	{
// 		m_pSpLoss->EnableWindow(TRUE);
// 	}
// 	else
// 	{
// 		m_pSpLoss->EnableWindow(FALSE);
// 	}
}

void CControlWnd::OnBtnSize()
{
	if(nSizeMode == SIZE_GENERAL)
	{
		nSizeMode = SIZE_JANGO;
		
		m_pStJumun->ShowWindow(SW_HIDE);
		m_pStJvol->ShowWindow(SW_HIDE);
		m_pEdJprc->ShowWindow(SW_HIDE);
		m_chkAutoJvol.ShowWindow(SW_HIDE);
		m_pJvolGrid->ShowWindow(SW_HIDE);
		m_pBtnJvol->ShowWindow(SW_HIDE);
		m_pJcntGrid->ShowWindow(SW_HIDE);
		m_pStJcnt->ShowWindow(SW_HIDE);
		m_pSpJcnt->ShowWindow(SW_HIDE);
		m_pStRsvd->ShowWindow(SW_HIDE);
		m_pSpRsvd->ShowWindow(SW_HIDE);
		m_rdNone.ShowWindow(SW_HIDE);
		m_rdIOC.ShowWindow(SW_HIDE);
		m_rdFOK.ShowWindow(SW_HIDE);
		m_chkMulti.ShowWindow(SW_HIDE);
		m_pSpMulti->ShowWindow(SW_HIDE);
		m_chkOneClick.ShowWindow(SW_HIDE);
		m_chkMarket.ShowWindow(SW_HIDE);

		CalcSizeJango();
	}
	else
	{
		nSizeMode = SIZE_GENERAL;

		m_pStJumun->ShowWindow(SW_SHOW);
		m_pStJvol->ShowWindow(SW_SHOW);
		m_pEdJprc->ShowWindow(SW_SHOW);
		m_chkAutoJvol.ShowWindow(SW_SHOW);
		m_pJvolGrid->ShowWindow(SW_SHOW);
		m_pBtnJvol->ShowWindow(SW_SHOW);
		m_pJcntGrid->ShowWindow(SW_SHOW);
		m_pStJcnt->ShowWindow(SW_SHOW);
		m_pSpJcnt->ShowWindow(SW_SHOW);
		m_pStRsvd->ShowWindow(SW_SHOW);
		m_pSpRsvd->ShowWindow(SW_SHOW);
		m_rdNone.ShowWindow(SW_SHOW);
		m_rdIOC.ShowWindow(SW_SHOW);
		m_rdFOK.ShowWindow(SW_SHOW);
		m_chkMulti.ShowWindow(SW_SHOW);
		m_pSpMulti->ShowWindow(SW_SHOW);
		m_chkOneClick.ShowWindow(SW_SHOW);
		m_chkMarket.ShowWindow(SW_SHOW);

		CalcSize();
	}

	Invalidate();
}

void CControlWnd::OnChkMarket()
{
	if(m_chkMarket.GetCheck())
	{
		m_pSpRsvd->EnableWindow(FALSE);

		if(m_bMarketConfirm)
		{
			CMarketConfirm dlg(this);

			dlg.DoModal();
		}
	}
	else
	{
		m_pSpRsvd->EnableWindow(TRUE);
	}
}

void CControlWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
// 	CRect rc;
// 	m_chkMarket.GetClientRect(rc);
// 
// 	if(rc.PtInRect(point))
// 	{
// 		m_pTooltip->AddTool(this, "사각형 표시는 시장가 주문을 뜻합니다.", rc, IDC_CHK_MARKET);
// 		m_pTooltip->Activate(TRUE);
// 	}
	
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CControlWnd::PreTranslateMessage(MSG* pMsg) 
{	
// 	OutputDebugString("PreTranslateMessage======================================>GO\n");
// 	if(pMsg->message == WM_MOUSEMOVE && pMsg->hwnd == m_chkMarket.m_hWnd)
// 	{
// 		OutputDebugString("RELAY EVENT-======================================>GO\n");
// 		m_pTooltip->RelayEvent(pMsg);
// 	}
	return CWnd::PreTranslateMessage(pMsg);
}

LRESULT CControlWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{	
// 	CString s;
// 	s.Format("WINDOW PROC : [%d]\n",message);
// 	OutputDebugString(s);
// 	MSG msg;
// 	msg.hwnd = m_hWnd;
// 	msg.message = message;
// 	msg.wParam = wParam;
// 	msg.lParam = lParam;
// 	if (message == WM_SETCURSOR)
// 	{
// 		OutputDebugString("RELAY EVENT-======================================>GO\n");
// 
// 		m_pTooltip->RelayEvent(&msg);
// 	}
	return CWnd::WindowProc(message, wParam, lParam);
}

void CControlWnd::SetChegView(bool bView)
{
	if(!bView)
	{
		m_bChegView = FALSE;
		
		CString strTab;

// 		if(m_mapTab.Lookup("체결",strTab))
// 			m_pTab->DeleteItem(atoi(strTab));

		m_pTab->SetCurSel(0);

		int nCmd1 = SW_SHOW, nCmd2 = SW_SHOW, nCmd3 = SW_SHOW;
		if (m_pTab->GetCurSel()==0)
		{
			nCmd1 = SW_SHOW;
			nCmd2 = SW_HIDE;
			nCmd3 = SW_HIDE;
			m_bChegUpdate = FALSE;
			
			m_pScreenWnd->closeMap();
		}
		else if (m_pTab->GetCurSel() == 1)
		{
			nCmd1 = SW_HIDE;
			nCmd2 = SW_HIDE;
			nCmd3 = SW_SHOW;
			m_bChegUpdate = FALSE;
			
			m_pScreenWnd->ChangeMap("IB401399", "");
			//m_pScreenWnd->ShowWindow(SW_SHOW);
		}
		else
		{
			return;
		}
		
		{
			CString strVal;
			strVal.Format("%d", m_pTab->GetCurSel());
			WritePrivateProfileString("TAB1", "SELECT", (LPCSTR)strVal, (LPCSTR)m_profile);
		}
		
		for(int n=0; n<4; ++n)
		{
			if (m_pStFuture[n])  m_pStFuture[n]->ShowWindow(nCmd1);
			if (m_pBtnFuture[n]) m_pBtnFuture[n]->ShowWindow(nCmd1);
			if (m_pBtnOption[n]) m_pBtnOption[n]->ShowWindow(nCmd1);
			if (m_pOptGrid) m_pOptGrid->ShowWindow(nCmd1);

			if(n==3)
			{
				m_pBtnFuture[4]->ShowWindow(nCmd1);
				m_pBtnOption[4]->ShowWindow(nCmd1);
			}
		}
		m_pChegGrid->ShowWindow(nCmd2);
		m_pBtnChegLink->ShowWindow(nCmd2);
		
		m_pSise->ShowWindow(nCmd3);
		m_pScreenWnd->ShowWindow(nCmd3);
	}
	else
	{
		m_bChegView = TRUE;
		
// 		CString strTab;
// 
// 		if(!m_mapTab.Lookup("체결",strTab))
// 		{
// 			m_pTab->InsertItem(m_pTab->GetItemCount(), "체결");
// 
// 			CString strTmp;
// 			strTmp.Format("%d",m_pTab->GetItemCount());
// 
// 			m_mapTab.SetAt("체결",strTmp);
// 		}
	}
}

void CControlWnd::SetShowCurrTab(bool bCurr)
{
	CString strTab;

	if(!bCurr)
	{
		m_bCurrView = FALSE;

// 		if(m_mapTab.Lookup("현재",strTab))
// 			m_pTab->DeleteItem(atoi(strTab));
		
		m_pScreenWnd->ShowWindow(SW_HIDE);
		m_pScreenWnd->closeMap();

		m_pTab->SetCurSel(0);

		NMHDR hdr;
		hdr.idFrom = IDC_CTL_TAB1;
		OnTabChange(&hdr, NULL);
	}
	else
	{
		m_bCurrView = TRUE;

// 		if(!m_mapTab.Lookup("현재",strTab))
// 		{
// 			m_pTab->InsertItem(1, "현재");
// 			
// 			CString strTmp;
// 			strTmp.Format("%d",1);
// 			
// 			m_mapTab.SetAt("현재",strTmp);
// 		}
	}
}

void CControlWnd::SetTabItems()
{
	char buff[128];
	
	GetPrivateProfileString("Config", "ShowCheg", "FALSE", buff, sizeof(buff), m_profile);
	BOOL m_bShowChg = (strcmp(buff, "TRUE")==0) ? TRUE : FALSE;
	
	GetPrivateProfileString("Config", "ShowCurr", "TRUE", buff, sizeof(buff), m_profile);
	BOOL m_bShowCurr = (strcmp(buff, "TRUE")==0) ? TRUE : FALSE;
	
	m_pTab->DeleteAllItems();
	
	m_pTab->InsertItem(0, "잔(미)");
	
	if(m_bShowCurr)
		m_pTab->InsertItem(1, "현재");
	
	if(!m_bShowChg)
		m_pTab->InsertItem(2, "체결");
}

void CControlWnd::OnBtn_Gen()
{
CString slog;
//slog.Format("[IB300600] [CControlWnd::OnBtn_Gen] m_iKind = [%d]\n", m_iKind);  
//OutputDebugString(slog);
	
	if(m_iKind == NORMALBTN)
	{
//slog.Format("[IB300600] [CControlWnd::OnBtn_Gen] <<<<<<return>>>>>>>");
//OutputDebugString(slog);
		return;
	}
	
	Clear();	//2015.10.12 KSJ 잔고조회 된것 초기화
	m_iKind = NORMALBTN;
	m_bKind = TRUE;
	
	//LoadMaster(TRUE);
	LoadMaster(NORMALBTN);
	
	SetControlWindow();
	
	m_pBtnMini->SetImgBitmap(m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP());
	m_pBtnGen->SetImgBitmap(m_hBtnLong[1]->operator HBITMAP(), m_hBtnLong[1]->operator HBITMAP(), m_hBtnLong[1]->operator HBITMAP());
	m_pBtnWeek->SetImgBitmap(m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP());
	
	SetOptionData(0);

	m_pBtnMini->Invalidate();
	m_pBtnGen->Invalidate();
	m_pBtnWeek->Invalidate();
}
void CControlWnd::OnBtnGen()
{
//CString stmp;
//stmp.Format("------IB300600 [CControlWnd::OnBtnGen] \n");
//OutputDebugString(stmp);

	if(m_iKind == NORMALBTN)
	{
//		stmp.Format("------IB300600 [CControlWnd::OnBtnGen] return");
		//OutputDebugString(stmp);
		return;
	}

	Clear();	//2015.10.12 KSJ 잔고조회 된것 초기화
	m_iKind = NORMALBTN;
	m_bKind = TRUE;

	//LoadMaster(TRUE);
	LoadMaster(NORMALBTN);

	SetControlWindow();

	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_OPT1, 0), (LPARAM)m_pBtnOption[0]->GetSafeHwnd());
	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_FUT1, 0), (LPARAM)m_pBtnFuture[0]->GetSafeHwnd());
	m_pBtnMini->SetImgBitmap(m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP());
	m_pBtnGen->SetImgBitmap(m_hBtnLong[1]->operator HBITMAP(), m_hBtnLong[1]->operator HBITMAP(), m_hBtnLong[1]->operator HBITMAP());
	m_pBtnWeek->SetImgBitmap(m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP());

	m_pBtnMini->Invalidate();
	m_pBtnGen->Invalidate();
	m_pBtnWeek->Invalidate();
	DisplayAllJango();
}

void CControlWnd::OnBtn_Week()
{
	CString stmp;
	stmp.Format("------IB300600 [CControlWnd::OnBtn_Week] \n");
	//OutputDebugString(stmp);
	
	if(m_iKind == WEEKBTN)
	{
		stmp.Format("------IB300600 [CControlWnd::OnBtn_Week] <<return>> \n");
		//	OutputDebugString(stmp);
		return;
	}
	
	Clear();	//2015.10.12 KSJ 잔고조회 된것 초기화
	
	m_iKind = WEEKBTN;
	m_bKind = TRUE;
	
	LoadMaster(WEEKBTN);
	
	SetControlWindowWEEK(true);
	
	m_pBtnGen->SetImgBitmap(m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP());
	m_pBtnMini->SetImgBitmap(m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP());
	m_pBtnWeek->SetImgBitmap(m_hBtnLong[1]->operator HBITMAP(), m_hBtnLong[1]->operator HBITMAP(), m_hBtnLong[1]->operator HBITMAP());
	
	m_pBtnMini->Invalidate();
	m_pBtnGen->Invalidate();
	m_pBtnWeek->Invalidate();
	DisplayAllJango();	
}

void CControlWnd::OnBtnWeek()
{
CString slog;
//slog.Format("IB300600 [CControlWnd::OnBtnWeek] \n");
//OutputDebugString(slog);

	if(m_iKind == WEEKBTN)
	{
//slog.Format("------IB300600 [CControlWnd::OnBtnWeek] return \n");
//OutputDebugString(slog);
		return;
	}
	
	Clear();	//2015.10.12 KSJ 잔고조회 된것 초기화
	
	m_iKind = WEEKBTN;
	m_bKind = TRUE;

	LoadMaster(WEEKBTN);

	SetControlWindowWEEK();
	
	m_pBtnGen->SetImgBitmap(m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP());
	m_pBtnMini->SetImgBitmap(m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP());
	m_pBtnWeek->SetImgBitmap(m_hBtnLong[1]->operator HBITMAP(), m_hBtnLong[1]->operator HBITMAP(), m_hBtnLong[1]->operator HBITMAP());
	
	m_pBtnMini->Invalidate();
	m_pBtnGen->Invalidate();
	m_pBtnWeek->Invalidate();
	DisplayAllJango();
}

void CControlWnd::OnBtn_Mini()
{
CString slog;
//slog.Format("[IB300600] [CControlWnd::OnBtn_Mini] m_iKind = [%d] \n", m_iKind);
//OutputDebugString(slog);
	
	if(m_iKind == MINIBTN)
	{
//slog.Format("[IB300600] [CControlWnd::OnBtn_Mini] <<<<<<return>>>>>> \n");
//OutputDebugString(slog);
		return;
	}
	
	Clear();	//2015.10.12 KSJ 잔고조회 된것 초기화
	
	m_iKind = MINIBTN;
	m_bKind = FALSE;
	
	//	LoadMaster(FALSE);
	LoadMaster(MINIBTN);
	
	SetControlWindow();
	
	m_pBtnGen->SetImgBitmap(m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP());
	m_pBtnMini->SetImgBitmap(m_hBtnLong[1]->operator HBITMAP(), m_hBtnLong[1]->operator HBITMAP(), m_hBtnLong[1]->operator HBITMAP());
	m_pBtnWeek->SetImgBitmap(m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP());
	
	SetOptionData(0);

	m_pBtnMini->Invalidate();
	m_pBtnGen->Invalidate();
	m_pBtnWeek->Invalidate();
}

void CControlWnd::OnBtnMini()
{
CString slog;
//slog.Format("IB300600 [CControlWnd::OnBtnMini] \n");
//OutputDebugString(slog);
	
	if(m_iKind == MINIBTN)
	{
//slog.Format("IB300600 [CControlWnd::OnBtnMini] return \n");
//OutputDebugString(slog);
		return;
	}
	
	Clear();	//2015.10.12 KSJ 잔고조회 된것 초기화

	m_iKind = MINIBTN;
	m_bKind = FALSE;

//	LoadMaster(FALSE);
	LoadMaster(MINIBTN);
	
	SetControlWindow();

	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_OPT1, 0), (LPARAM)m_pBtnOption[0]->GetSafeHwnd());
	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BTN_FUT1, 0), (LPARAM)m_pBtnFuture[0]->GetSafeHwnd());
	m_pBtnGen->SetImgBitmap(m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP());
	m_pBtnMini->SetImgBitmap(m_hBtnLong[1]->operator HBITMAP(), m_hBtnLong[1]->operator HBITMAP(), m_hBtnLong[1]->operator HBITMAP());
	m_pBtnWeek->SetImgBitmap(m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP(), m_hBtnLong[0]->operator HBITMAP());

	m_pBtnMini->Invalidate();
	m_pBtnGen->Invalidate();
	m_pBtnWeek->Invalidate();
	DisplayAllJango();
}


void CControlWnd::SetFOType( bool bFlag )
{
CString slog;
//slog.Format("\nIB300600 [CControlWnd::SetFOType] flag=[%d]\n", bFlag);
//OutputDebugString(slog);

	if(bFlag)
	{
		OnBtnGen();
	}
	else
	{
		OnBtnMini();
	}
}


void CControlWnd::SetFOType( int igubn )
{
CString slog;
//slog.Format("IB300600 [CControlWnd::SetFOType] igubn=[%d]\n", igubn);
//OutputDebugString(slog);

	if(igubn == 0 && m_iKind == WEEKBTN && m_bClickInWeek == true )  //위클리 버튼클릭 상태에서 선물월물 버튼 클릭시 일반으로 버튼 변경 막기위해
	{
		m_bClickInWeek = false;
		OnBtnWeek();	
		return;
	}
	
	 if(igubn == 0)    //일반 
	{
		OnBtn_Gen();    
	}
	else if(igubn == 1)  //미니
	{
		OnBtn_Mini();
	}
	else if(igubn == 2)   //위클리
	{
		OnBtn_Week();	
	}

}

BOOL CControlWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CString slog;
	slog.Format("[3006] CControlWnd-OnCommand   [%d]", LOWORD(wParam));
	OutputDebugString(slog);
	GetParent()->SetFocus();
	return CWnd::OnCommand(wParam, lParam);
}
