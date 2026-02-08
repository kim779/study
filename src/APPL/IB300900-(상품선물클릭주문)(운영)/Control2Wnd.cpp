// Control2Wnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB300900.h"
#include "Control2Wnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControl2Wnd

CControl2Wnd::CControl2Wnd(CWnd *pParent, AccountMap *pAccount, SiseData *pSise)
: CAxisExt(pParent), m_pAccount(pAccount), m_pSiseData(pSise)
{
	CString path;
	path.Format("%s\\%s\\", Variant(homeCC,""), "image");
	m_hBtn2[0] = GetAxBitmap(path + "2btn.bmp");
	m_hBtn2[1] = GetAxBitmap(path + "2btn_dn.bmp");
	m_hBtn2[2] = GetAxBitmap(path + "2btn_en.bmp");

	for(int n=0; n<3; ++n)
	{
		m_pStJvol[n] = new CfxStatic(m_pFont);
	}
	m_pStJngo = new CfxStatic(m_pFont);
	m_pStMicheg = new CfxStatic(m_pFont);
	m_pStRet = new CfxStatic(m_pFont);
	m_pJvolGrid = new CGridCtrl();
	m_pJngoGrid = new CGridCtrl();
	m_pMichGrid = new CGridCtrl();
	m_pRetGrid = new CGridCtrl();
}

CControl2Wnd::~CControl2Wnd()
{
}

void CControl2Wnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	for(int n=0; n<3; ++n)
	{
		if (m_pStJvol[n]) { m_pStJvol[n]->DestroyWindow(); delete m_pStJvol[n]; }
	}
	if (m_pStJngo)   { m_pStJngo->DestroyWindow(); delete m_pStJngo; }
	if (m_pStMicheg) { m_pStMicheg->DestroyWindow(); delete m_pStMicheg; }
	if (m_pJvolGrid) { m_pJvolGrid->DestroyWindow(); delete m_pJvolGrid; }
	if (m_pJngoGrid) { m_pJngoGrid->DestroyWindow(); delete m_pJngoGrid; }
	if (m_pMichGrid) { m_pMichGrid->DestroyWindow(); delete m_pMichGrid; }
	if (m_pStRet)    { m_pStRet->DestroyWindow(); delete m_pStRet; }
	if (m_pRetGrid)  { m_pRetGrid->DestroyWindow(); delete m_pRetGrid; }
}

int CControl2Wnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rc;

	m_pStJvol[0]->Create("※ 주문가능수량", WS_CHILD|WS_VISIBLE, rc, this, -1);
	m_pStJvol[0]->SetBkColor( m_crBodyBk );
	m_pStJvol[0]->SetAlignment(alLEFT);

	m_pStJvol[1]->Create("조회단가", WS_CHILD|WS_VISIBLE|WS_BORDER, rc, this);
	m_pStJvol[1]->SetBkColor(GetIndexColor(COLOR_GRIDHEAD), false);
	m_pStJvol[1]->SetFgColor(GetIndexColor(COLOR_GRIDHEADTXT), false);

	m_pStJvol[2]->Create("", WS_CHILD|WS_VISIBLE|WS_BORDER, rc, this);
	m_pStJvol[2]->SetBkColor(m_crWhite, false);
	m_pStJvol[2]->SetFgColor(m_crNormal, false);

	m_pJvolBtn[0] = CreateImgButton("매도", IDC_JVOL_MADO, m_hBtn2);
	m_pJvolBtn[1] = CreateImgButton("매수", IDC_JVOL_MASU, m_hBtn2);

	m_pJvolGrid->Create(rc, this, -1);
	SetJvolGrid();

	m_pStJngo->Create("※ 잔고", WS_CHILD|WS_VISIBLE|SS_LEFT, rc, this, -1);
	m_pStJngo->SetAlignment(alLEFT);
	m_pStJngo->SetBkColor( m_crBodyBk );

	m_pJngoGrid->Create(rc, this, -1);
	SetJngoGrid();

	m_pStMicheg->Create("※ 미체결", WS_CHILD|WS_VISIBLE|SS_LEFT, rc, this, -1);
	m_pStMicheg->SetBkColor( m_crBodyBk );
	m_pStMicheg->SetAlignment(alLEFT);

	m_pMichGrid->Create(rc, this, -1);
	SetMichGrid();

	m_pStRet->Create("※ 주문처리결과", WS_CHILD|WS_VISIBLE|SS_LEFT, rc, this, -1);
	m_pStRet->SetAlignment(alLEFT);
	m_pStRet->SetBkColor( m_crBodyBk );

	m_pRetGrid->Create(rc, this, -1);
	SetRetGrid();

	return 0;
}

void CControl2Wnd::CalcSize( int width/*=-1*/, int height/*=-1*/ )
{
	if (width<=0 || height<=0)
	{
		CRect rc;
		GetClientRect(rc);
		width = rc.Width();
		height = rc.Height();
	}

	int pos_x = 0, pos_y = 0, line_h = 20;

	// Line 1
	m_pStJvol[0]->MoveWindow(pos_x, pos_y, 100, line_h, FALSE); pos_x += 120;
	m_pStJvol[1]->MoveWindow(pos_x, pos_y, 60, line_h, FALSE); pos_x += 59;
	m_pStJvol[2]->MoveWindow(pos_x, pos_y, 60, line_h, FALSE); pos_x += 80;

	pos_x = width - (40*2) - PADDING_VALUE/2 - 3;
	m_pJvolBtn[0]->MoveWindow(pos_x, pos_y, 40, line_h, FALSE); pos_x += 40 + PADDING_VALUE;
	m_pJvolBtn[1]->MoveWindow(pos_x, pos_y, 40, line_h, FALSE);

	// Line 2
	pos_x = 0;
	pos_y += line_h + PADDING_VALUE/2;
	m_pJvolGrid->MoveWindow(pos_x, pos_y, width, GRID_CELL_HEIGHT*6, FALSE);

	// Line 3
	pos_x = 0;
	pos_y += (GRID_CELL_HEIGHT*6) + PADDING_VALUE/2;
	m_pStJngo->MoveWindow(pos_x, pos_y, 100, line_h);

	// Line 4
	pos_x = 0;
	pos_y += line_h;
	m_pJngoGrid->MoveWindow(pos_x, pos_y, width, GRID_CELL_HEIGHT*6, FALSE);

	// Line 5
	pos_x = 0;
	pos_y += (GRID_CELL_HEIGHT*6) + PADDING_VALUE/2;
	m_pStMicheg->MoveWindow(pos_x, pos_y, 100, line_h);

	// Line 6
	pos_x = 0;
	pos_y += line_h;
	m_pMichGrid->MoveWindow(pos_x, pos_y, width, GRID_CELL_HEIGHT*9, FALSE);

	pos_x = 0;
	pos_y += (GRID_CELL_HEIGHT*9) + PADDING_VALUE/2;
	m_pStRet->MoveWindow(pos_x, pos_y, 100, line_h);

	pos_x = 0;
	pos_y += line_h;
	m_pRetGrid->MoveWindow(pos_x, pos_y, width, height-pos_y, FALSE);

	MakeBackground();	
}

void CControl2Wnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	CalcSize();
}

BEGIN_MESSAGE_MAP(CControl2Wnd, CWnd)
	//{{AFX_MSG_MAP(CControl2Wnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, IDC_JVOL_MADO, IDC_JVOL_MASU, OnJvolBtnClick)
	ON_MESSAGE(WM_APP_REFRESH, OnRefresh)
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()

void CControl2Wnd::MakeBackground( int width, int height )
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

void CControl2Wnd::Draw(CDC *pdc)
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

void CControl2Wnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	Draw(&dc);
}

void CControl2Wnd::SetJvolPrice( int jprc )
{
	CString sdat;
	sdat.Format("%d.%02d", jprc/100, jprc%100);
	if (m_pStJvol[2])
		m_pStJvol[2]->SetText(sdat);
}

void CControl2Wnd::SetJvolGrid()
{
	CGridCellBase *pc;
	m_pJvolGrid->SetEditable(FALSE);
	//m_pJvolGrid->EnableSelection(FALSE);
	m_pJvolGrid->SetFrameFocusCell(FALSE);
	
	pc = m_pJvolGrid->GetDefaultCell(FALSE, FALSE);
	if (pc)	{
		pc->SetFormat( DT_VCENTER|DT_SINGLELINE|DT_RIGHT );
		pc->SetMargin( 5 );
	}
	pc = m_pJvolGrid->GetDefaultCell(TRUE, FALSE);
	if (pc) {
		
		pc->SetBackClr( GetIndexColor(COLOR_GRIDHEAD) );
		pc->SetTextClr( GetIndexColor(COLOR_GRIDHEADTXT) );
	}
	m_pJvolGrid->SetBkColor(m_crWhite);
	m_pJvolGrid->SetRowCount( max(m_pJvolGrid->GetRowCount(),10) );
	m_pJvolGrid->SetColumnCount(7);
	m_pJvolGrid->SetFixedRowCount(1);
	
	//400
	int col_width[] = { 0, 90, 70, 40, 58, 55, 55};
	char *head[] = { "RTS", "계좌번호", "계좌명", "배수", "총가능", "신규", "청산"};
	for(int n=0; n<7; ++n)
	{
		m_pJvolGrid->SetColumnWidth(n, col_width[n]);
		m_pJvolGrid->SetItemText(0, n, head[n]);
		m_pJvolGrid->SetItemFormat(0, n, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
}

CfxImgButton* CControl2Wnd::CreateImgButton(LPCSTR caption, int nID, CBitmap **bmp)
{
	CfxImgButton *pBtn = new CfxImgButton(m_pFont);
	if (pBtn->Create(caption, CRect(0, 0, 0, 0), this, nID, FALSE, TRUE))
	{
		pBtn->SetImgBitmap(	bmp[0]->operator HBITMAP(), bmp[1]->operator HBITMAP(), bmp[2]->operator HBITMAP() );
		return pBtn;
	}
	else return NULL;
}

LRESULT CControl2Wnd::OnRefresh( WPARAM wParam, LPARAM lParam )
{
	if (wParam==WP_ACCOUNT)
	{
		int cnt = (int)m_pAccount->size();

		m_pJvolGrid->SetRowCount( max(cnt+1, 10) );
		m_pJngoGrid->SetRowCount( max(cnt+1, 10) );
		
		AccountMap::iterator st = m_pAccount->begin();
		AccountMap::iterator ed = m_pAccount->end();
		for(int row=1; st!=ed; ++row, ++st)
		{
			CString acno_txt, mult;

			LPCSTR acno = st->first.c_str();
			acno_txt.Format("%.3s-%.2s-%.6s", acno, acno+3, acno+5);
			mult.Format("%d", st->second.mult);

			m_pJvolGrid->SetItemText(row, 0, acno);
			m_pJvolGrid->SetItemFormat(row, 1, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pJvolGrid->SetItemText(row, 1, acno_txt);
			m_pJvolGrid->SetItemFormat(row, 2, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
			m_pJvolGrid->SetItemText(row, 2, st->second.acnm);
			m_pJvolGrid->SetItemFormat(row, 3, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
			m_pJvolGrid->SetItemText(row, 3, mult);
			m_pJvolGrid->RedrawRow(row);

			m_pJngoGrid->SetItemText(row, 0, acno);
			m_pJngoGrid->SetItemFormat(row, 1, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pJngoGrid->SetItemText(row, 1, acno_txt);
			m_pJngoGrid->RedrawRow(row);
		}
	}
	else if (wParam==WP_JANGO)
	{
		Jango *pj = (Jango*)lParam;
		if (pj!=NULL) proc_jngo(pj);
		else proc_jngo_all();
	}
	else if (wParam==WP_MICHEG)
	{
		proc_micheg_all();
	}
	else if (wParam==WP_MICHEG_INSERT || wParam==WP_MICHEG_UPDATE || wParam==WP_MICHEG_DELETE)
	{
		proc_micheg(wParam, (Order*)lParam);
	}
	else if (wParam==WP_ORDER_RESULT)
	{
		proc_order((OrderResult*)lParam);
	}

	return 0;
}

void CControl2Wnd::OnJvolBtnClick( UINT nID )
{
	int rcnt = m_pJvolGrid->GetRowCount();
	if (rcnt>0)
	{
		for(int row=1; row<rcnt; ++row)
		{
			for(int col=4; col<7; ++col)
			{
				m_pJvolGrid->SetItemText(row, col, "");
				m_pJvolGrid->RedrawCell(row, col);
			}
		}
	}

	vector<char> data( L_ledgerH + sizeof(SONBQ101_mid), ' ' );

	CString OdrPrc = m_pStJvol[2]->GetText();
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
	if      (nID==IDC_JVOL_MADO) CopyMemory(mid->zBnsTp, "1", 1);
	else if (nID==IDC_JVOL_MASU) CopyMemory(mid->zBnsTp, "2", 1);
	else return;
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

		SendTR("piboPBxQ", US_ENC, &data[0], data.size(), TK_SONBQ101);
	}
}

LRESULT CControl2Wnd::OnUser(WPARAM wParam, LPARAM lParam)
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

void CControl2Wnd::res_SONBQ101( LPCSTR data, int dlen )
{
	struct _ledgerH *ledger = (struct _ledgerH*)data;
	SONBQ101_mod *mod = (SONBQ101_mod*)(data+L_ledgerH);

	CString emsg(ledger->emsg, sizeof(ledger->emsg));
	GetParent()->SendMessage(WM_USER, DLL_GUIDE, (LPARAM)(LPCSTR)emsg);

	int tmp;
	CString acno(mod->mid.zAcntNo, sizeof(mod->mid.zAcntNo));
	CString OrdAbleQty(mod->lOrdAbleQty, sizeof(mod->lOrdAbleQty));
	CString NewAbleQty(mod->lNewOrdAbleQty, sizeof(mod->lNewOrdAbleQty));
	CString LqdtAbleQty(mod->lLqdtOrdAbleQty, sizeof(mod->lLqdtOrdAbleQty));

	acno.TrimRight();
	tmp = atoi(OrdAbleQty); OrdAbleQty = Int2CommaStr(tmp);
	tmp = atoi(NewAbleQty); NewAbleQty = Int2CommaStr(tmp);
	tmp = atoi(LqdtAbleQty); LqdtAbleQty = Int2CommaStr(tmp);

	tmp = m_pJvolGrid->GetRowCount();
	if (tmp<=0) return;
	for(int row=1; row<tmp; ++row)
	{
		if (m_pJvolGrid->GetItemText(row, 0)==acno)
		{
			m_pJvolGrid->SetItemText(row, 4, OrdAbleQty);
			m_pJvolGrid->SetItemText(row, 5, NewAbleQty);
			m_pJvolGrid->SetItemText(row, 6, LqdtAbleQty);
			m_pJvolGrid->RedrawRow(row);
			break;
		}
	}
}

void CControl2Wnd::SetJngoGrid()
{
	CGridCellBase *pc;
	m_pJngoGrid->SetEditable(FALSE);
	//m_pJngoGrid->EnableSelection(FALSE);
	m_pJngoGrid->SetFrameFocusCell(FALSE);
	
	pc = m_pJngoGrid->GetDefaultCell(FALSE, FALSE);
	if (pc)	{
		pc->SetFormat( DT_VCENTER|DT_SINGLELINE|DT_RIGHT );
		pc->SetMargin( 5 );
	}
	pc = m_pJngoGrid->GetDefaultCell(TRUE, FALSE);
	if (pc) {
		
		pc->SetBackClr( GetIndexColor(COLOR_GRIDHEAD) );
		pc->SetTextClr( GetIndexColor(COLOR_GRIDHEADTXT) );
	}
	m_pJngoGrid->SetBkColor(m_crWhite);
	m_pJngoGrid->SetRowCount( max(m_pJngoGrid->GetRowCount(),10) );
	m_pJngoGrid->SetColumnCount(7);
	m_pJngoGrid->SetFixedRowCount(1);
	
	//400
	int col_width[] = { 0, 90, 35, 35, 55, 100, 55};
	char *head[] = { "RTS", "계좌번호", "구분", "잔고", "평균가", "평가손익", "수익률"};
	for(int n=0; n<7; ++n)
	{
		m_pJngoGrid->SetColumnWidth(n, col_width[n]);
		m_pJngoGrid->SetItemText(0, n, head[n]);
		m_pJngoGrid->SetItemFormat(0, n, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
}

void CControl2Wnd::SetMichGrid()
{
	CGridCellBase *pc;
	m_pMichGrid->SetEditable(FALSE);
	//m_pMichGrid->EnableSelection(FALSE);
	m_pMichGrid->SetFrameFocusCell(FALSE);
	
	pc = m_pMichGrid->GetDefaultCell(FALSE, FALSE);
	if (pc)	{
		pc->SetFormat( DT_VCENTER|DT_SINGLELINE|DT_RIGHT );
		pc->SetMargin( 5 );
	}
	pc = m_pMichGrid->GetDefaultCell(TRUE, FALSE);
	if (pc) {
		
		pc->SetBackClr( GetIndexColor(COLOR_GRIDHEAD) );
		pc->SetTextClr( GetIndexColor(COLOR_GRIDHEADTXT) );
	}
	m_pMichGrid->SetBkColor(m_crWhite);
	m_pMichGrid->SetRowCount( max(m_pMichGrid->GetRowCount(),12) );
	m_pMichGrid->SetColumnCount(7);
	m_pMichGrid->SetFixedRowCount(1);
	
	//400
	int col_width[] = { 0, 90, 50, 50, 60, 60, 60};
	char *head[] = { "RTS", "계좌번호", "주문#", "구분", "주문단가", "주문수량", "미체결량"};
	for(int n=0; n<7; ++n)
	{
		m_pMichGrid->SetColumnWidth(n, col_width[n]);
		m_pMichGrid->SetItemText(0, n, head[n]);
		m_pMichGrid->SetItemFormat(0, n, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
}

void CControl2Wnd::SetRetGrid()
{
	CGridCellBase *pc;
	m_pRetGrid->SetEditable(FALSE);
	//m_pRetGrid->EnableSelection(FALSE);
	m_pRetGrid->SetFrameFocusCell(FALSE);
	
	pc = m_pRetGrid->GetDefaultCell(FALSE, FALSE);
	if (pc)	{
		pc->SetFormat( DT_VCENTER|DT_SINGLELINE|DT_RIGHT );
		pc->SetMargin( 5 );
	}
	pc = m_pRetGrid->GetDefaultCell(TRUE, FALSE);
	if (pc) {
		
		pc->SetBackClr( GetIndexColor(COLOR_GRIDHEAD) );
		pc->SetTextClr( GetIndexColor(COLOR_GRIDHEADTXT) );
	}
	m_pRetGrid->SetBkColor(m_crWhite);
	m_pRetGrid->SetRowCount(max(m_pRetGrid->GetRowCount(),10) );
	m_pRetGrid->SetColumnCount(4);
	m_pRetGrid->SetFixedRowCount(1);
	
	//400
	int col_width[] = { 0, 90, 50, 230};
	char *head[] = { "RTS", "계좌번호", "주문#", "처리결과"};
	for(int n=0; n<4; ++n)
	{
		m_pRetGrid->SetColumnWidth(n, col_width[n]);
		m_pRetGrid->SetItemText(0, n, head[n]);
		m_pRetGrid->SetItemFormat(0, n, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
}

void CControl2Wnd::proc_jngo( Jango *pj )
{
	if (pj->code != m_pSiseData->code) return;
	
	CString srat, tamt;
	COLORREF col;
	int rcnt = m_pJngoGrid->GetRowCount();
	for(int row=1; row<rcnt; ++row)
	{
		if ( m_pJngoGrid->GetItemText(row, 0) == pj->acno )
		{
			if (pj->flag=='D')
			{
				for(int n=2; n<7; ++n) m_pJngoGrid->SetItemText(row, n, "");
				m_pJngoGrid->RedrawRow(row);
			}
			else
			{
				if      (pj->gubn[0]=='+') m_pJngoGrid->SetItemFgColour(row, 2, m_crPlus);
				else if (pj->gubn[0]=='-') m_pJngoGrid->SetItemFgColour(row, 2, m_crMinus);
				else                       m_pJngoGrid->SetItemFgColour(row, 2, m_crBlack);
				m_pJngoGrid->SetItemText(row, 2, &pj->gubn[1]);
				
				m_pJngoGrid->SetItemText(row, 3, Int2CommaStr(pj->jqty));
				m_pJngoGrid->SetItemText(row, 4, Int2Str(pj->pprc, 2));
				
				if      (pj->tamt>0.0) { col = m_crPlus;  tamt = Int2CommaStr((int)pj->tamt); srat.Format("%.2f", pj->srat); }
				else if (pj->tamt<0.0) { col = m_crMinus; tamt = Int2CommaStr((int)pj->tamt); srat.Format("%.2f", pj->srat); }
				else                   { col = m_crBlack; tamt = "";                     srat = "";                     }
				
				m_pJngoGrid->SetItemFgColour(row, 5, col);
				m_pJngoGrid->SetItemText(row, 5, tamt);
				
				m_pJngoGrid->SetItemFgColour(row, 6, col);
				m_pJngoGrid->SetItemText(row, 6, srat);
				
				m_pJngoGrid->RedrawRow(row);
			}
			break;
		}
	}
}

void CControl2Wnd::proc_jngo_all()
{
	COLORREF col;
	CString srat, tamt, code = m_pSiseData->code;
	int rcnt = m_pJngoGrid->GetRowCount();
	for(int row=1; row<rcnt; ++row)
	{
		CString acno = m_pJngoGrid->GetItemText(row, 0);
		AccountMap::iterator pos = m_pAccount->find( (LPCSTR)acno );
		if (pos==m_pAccount->end()) continue;

		CodeJangoMap::iterator jpos = pos->second.jango.find( (LPCSTR)code );
		if (jpos==pos->second.jango.end()) 
		{
			for(int n=2; n<7; ++n) m_pJngoGrid->SetItemText(row, n, "");
			m_pJngoGrid->RedrawRow(row);
			continue;
		}

		Jango *pj = &jpos->second;

		if      (pj->gubn[0]=='+') m_pJngoGrid->SetItemFgColour(row, 2, m_crPlus);
		else if (pj->gubn[0]=='-') m_pJngoGrid->SetItemFgColour(row, 2, m_crMinus);
		else                       m_pJngoGrid->SetItemFgColour(row, 2, m_crBlack);
		m_pJngoGrid->SetItemText(row, 2, &pj->gubn[1]);
		
		m_pJngoGrid->SetItemText(row, 3, Int2CommaStr(pj->jqty));
		m_pJngoGrid->SetItemText(row, 4, Int2Str(pj->pprc, 2));
		
		if      (pj->tamt>0.0) { col = m_crPlus;  tamt = Int2CommaStr((int)pj->tamt); srat.Format("%.2f", pj->srat); }
		else if (pj->tamt<0.0) { col = m_crMinus; tamt = Int2CommaStr((int)pj->tamt); srat.Format("%.2f", pj->srat); }
		else                   { col = m_crBlack; tamt = "";                     srat = "";                     }
		
		m_pJngoGrid->SetItemFgColour(row, 5, col);
		m_pJngoGrid->SetItemText(row, 5, tamt);
		
		m_pJngoGrid->SetItemFgColour(row, 6, col);
		m_pJngoGrid->SetItemText(row, 6, srat);
		
		m_pJngoGrid->RedrawRow(row);
	}
}

void CControl2Wnd::proc_micheg_all()
{
	int nrec, row;
	CString acno_txt;
	AccountMap::iterator st = m_pAccount->begin();
	AccountMap::iterator ed = m_pAccount->end();
	
	for(nrec=0; st!=ed; ++st)
	{
		CodeOrderMap::iterator pos = st->second.order.find( (LPCSTR)m_pSiseData->code );
		if (pos==st->second.order.end()) continue;
		nrec += pos->second.size();
	}
	m_pMichGrid->SetRowCount( 1 );
	m_pMichGrid->SetRowCount( nrec + 1 );

	st = m_pAccount->begin();
	for(row=1; st!=ed; ++st)
	{
		LPCSTR acno = (LPCSTR)st->first.c_str();
		acno_txt.Format("%.3s-%.2s-%.6s", acno, acno+3, acno+5);	

		CodeOrderMap::iterator pos = st->second.order.find( (LPCSTR)m_pSiseData->code );
		if (pos==st->second.order.end()) continue;

		OrderMap::iterator ost = pos->second.begin();
		OrderMap::iterator oed = pos->second.end();
		for(; ost!=oed; ++ost, ++row)
		{
			Order *po = &(ost->second);
			// "RTS", "계좌번호", "주문#", "구분", "주문단가", "주문수량", "미체결량"
			m_pMichGrid->SetItemText(row, 0, acno);
			m_pMichGrid->SetItemText(row, 1, acno_txt);
			m_pMichGrid->SetItemText(row, 2, Int2Str(po->juno,0));
			if (po->odgb==1) { 
				m_pMichGrid->SetItemFgColour(row, 3, m_crMinus); 
				m_pMichGrid->SetItemText(row, 3, "매도");
			} else if (po->odgb==2) { 
				m_pMichGrid->SetItemFgColour(row, 3, m_crPlus);
				m_pMichGrid->SetItemText(row, 3, "매수");
			} else {
				--row; 
				continue;
			}
			m_pMichGrid->SetItemText(row, 4, Int2Str(po->oprc));
			m_pMichGrid->SetItemText(row, 5, Int2CommaStr(po->oqty));
			m_pMichGrid->SetItemText(row, 6, Int2CommaStr(po->wqty));
			m_pMichGrid->RedrawRow(row);
		}
	}
	m_pMichGrid->SetRowCount(max(10, row));
}

void CControl2Wnd::proc_micheg( int sig, Order *pOdr )
{
	if (pOdr->code != m_pSiseData->code) return;

	int nrec = m_pMichGrid->GetRowCount();
	if (sig!=WP_MICHEG_INSERT)
	{
		for(int row=1; row<nrec; ++row)
		{
			CString acno = m_pMichGrid->GetItemText(row, 0);
			int juno = atoi(m_pMichGrid->GetItemText(row, 2));
			if (acno==pOdr->acno && juno==pOdr->juno)
			{
				if (sig==WP_MICHEG_UPDATE)
				{
					m_pMichGrid->SetItemText(row, 0, acno);
					m_pMichGrid->SetItemText(row, 6, Int2CommaStr(pOdr->wqty));
				}
				else if (sig==WP_MICHEG_DELETE)
				{
					m_pMichGrid->DeleteRow(row);
					m_pMichGrid->InsertRow("");
				}
				m_pMichGrid->Invalidate(FALSE);
				break;
			}
		}
	}
	else
	{
		CString acno_txt;
		acno_txt.Format("%.3s-%.2s-%.6s", pOdr->acno, pOdr->acno+3, pOdr->acno+5);

		int row = m_pMichGrid->InsertRow("", 1);
		m_pMichGrid->SetItemText(row, 0, pOdr->acno);
		m_pMichGrid->SetItemText(row, 1, acno_txt);
		m_pMichGrid->SetItemText(row, 2, Int2Str(pOdr->juno,0));
		if (pOdr->odgb==1) { 
			m_pMichGrid->SetItemFgColour(row, 3, m_crMinus); 
			m_pMichGrid->SetItemText(row, 3, "매도");
		} else if (pOdr->odgb==2) { 
			m_pMichGrid->SetItemFgColour(row, 3, m_crPlus);
			m_pMichGrid->SetItemText(row, 3, "매수");
		} else {
			m_pMichGrid->SetItemFgColour(row, 3, m_crNormal);
			m_pMichGrid->SetItemText(row, 3, "");
		}
		m_pMichGrid->SetItemText(row, 4, Int2Str(pOdr->oprc));
		m_pMichGrid->SetItemText(row, 5, Int2CommaStr(pOdr->oqty));
		m_pMichGrid->SetItemText(row, 6, Int2CommaStr(pOdr->wqty));
		m_pMichGrid->Invalidate(FALSE);
	}
	m_pMichGrid->SetScrollPos(SB_VERT, 0, TRUE);
}

void CControl2Wnd::proc_order( OrderResult* pOr )
{
	int row = m_pRetGrid->InsertRow("", 1);

	LPCSTR acno = pOr->acno;
	CString acno_txt;
	acno_txt.Format("%.3s-%.2s-%.6s", acno, acno+3, acno+5);

	m_pRetGrid->SetItemText(row, 0, acno);

	m_pRetGrid->SetItemFormat(row, 1, DT_VCENTER|DT_SINGLELINE|DT_CENTER);
	m_pRetGrid->SetItemText(row, 1, acno_txt);

	m_pRetGrid->SetItemFormat(row, 2, DT_VCENTER|DT_SINGLELINE|DT_RIGHT);
	m_pRetGrid->SetItemText(row, 2, pOr->juno);

	m_pRetGrid->SetItemFormat(row, 3, DT_VCENTER|DT_SINGLELINE|DT_LEFT);
	m_pRetGrid->SetItemText(row, 3, pOr->emsg);

	m_pRetGrid->SetScrollPos(SB_VERT, 0, TRUE);

	m_pRetGrid->Invalidate(FALSE);
}

void CControl2Wnd::ChangeTheme()
{
	AxInit();
	SetJvolGrid();
	SetJngoGrid();
	SetMichGrid();
	SetRetGrid();
	m_pStJvol[1]->SetBkColor(GetIndexColor(COLOR_GRIDHEAD), false);
	Invalidate(TRUE);
}

void CControl2Wnd::Clear()
{
	int rows, cols, row, col;

	rows = m_pJvolGrid->GetRowCount();
	cols = m_pJvolGrid->GetColumnCount();
	for(row=1; row<rows; ++row)
		for(col=0; col<cols; ++col)
			m_pJvolGrid->SetItemText(row, col, "");
	m_pJvolGrid->Invalidate(TRUE);

	rows = m_pJngoGrid->GetRowCount();
	cols = m_pJngoGrid->GetColumnCount();
	for(row=1; row<rows; ++row)
		for(col=0; col<cols; ++col)
			m_pJngoGrid->SetItemText(row, col, "");
	m_pJngoGrid->Invalidate(TRUE);

	rows = m_pMichGrid->GetRowCount();
	cols = m_pMichGrid->GetColumnCount();
	for(row=1; row<rows; ++row)
		for(col=0; col<cols; ++col)
			m_pMichGrid->SetItemText(row, col, "");
	m_pMichGrid->Invalidate(TRUE);

}
