// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB650001.h"
#include "MapWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define WRITELOG	//2014.02.19 로그를 저장할지 여부

/////////////////////////////////////////////////////////////////////////////
// CMapWnd

CMapWnd::CMapWnd(CWnd *pParent)
: CAxisExt(pParent), CWnd()
{
	m_bLoading = FALSE;
	m_bNewGroup = FALSE;
	m_pCurGroup = NULL;
	m_pCurAccount = NULL;
	m_pGridSelectIdx[0] = m_pGridSelectIdx[1] = m_pGridSelectIdx[2] = -1;
}

CMapWnd::~CMapWnd()
{
}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
	ON_BN_CLICKED(IDC_BTN_GROUPUPDATE, UpdateGroup)
	ON_BN_CLICKED(IDC_BTN_GROUPADD, OnBtnNewGroup)
	ON_BN_CLICKED(IDC_BTN_GROUPDELETE, OnBtnDelGroup)
	ON_BN_CLICKED(IDC_BTN_RELOAD, OnBtnReload)
	ON_BN_CLICKED(IDC_BTN_LIMITSTOCK, OnBtnLimit)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMapWnd message handlers

int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetFont(m_pFont);
	Init();	
	InitGrid();
	InitControl();
	
	m_pass.clear();	//2014.02.19 KSJ 처음에만 지우고 나중에는 계속 관리한다.

	// request group/account data
	LoadData();
	
	return 0;
}

void CMapWnd::OnDestroy() 
{
	for(int n=0; n<3; ++n)
	{
		if (m_pGrid[n]->GetSafeHwnd())
		{
			m_pGrid[n]->DestroyWindow();
			delete m_pGrid[n];
			m_pGrid[n] = NULL;
		}
	}
	CWnd::OnDestroy();	
}

void CMapWnd::LoadData()
{
	m_bLoading = TRUE;
	EnableWindow(FALSE);

	m_gdata.clear();
	m_adata.clear();
	m_owns.clear();
//	m_pass.clear();	//2014.02.19 KSJ 처음에만 지우고 나중에는 계속 관리한다.
	m_pCurAccount = NULL;
	m_pCurGroup = NULL;
	m_bNewGroup = FALSE;

	for(int n=0; n<3; ++n)
		ClearGrid( m_pGrid[n] );

	LoadOwnAccount();

	vector<char> data( L_ledgerH + sizeof(sdldq003_mid) , ' ');

	struct _ledgerH *ledger = (struct _ledgerH*)&data[0];
	GetLedger(ledger);
	CopyMemory(ledger->svcd, "SDLDQ003", 8);
	CopyMemory(ledger->rcnt, "0030", 4);
	CopyMemory(ledger->brno, "000", 3);
	CopyMemory(ledger->odrf, "2", 1);
	CopyMemory(ledger->mkty, "1", 1);	


	sdldq003_mid *mid = (sdldq003_mid*)&data[L_ledgerH];
	FillMemory(mid, sizeof(mid), ' ');
	CopyMemory(mid->irec, "00001", 5);
	CopyMemory(mid->gubn, "1", 1);
	CopyMemory(mid->type, "10", 2);

	WriteLog("LoadData", data.size(), &data[0]);
	SendTR("piboPBxQ", US_ENC, &data[0], data.size(), TK_SDLDQ003_GROUP);
}

LRESULT CMapWnd::OnUser( WPARAM wParam, LPARAM lParam )
{
	int msg = LOBYTE(LOWORD(wParam));
	int key = HIBYTE(LOWORD(wParam));
	int len = HIWORD(wParam);
	LPCSTR dat = (LPCSTR)lParam;

	CString strDomino = dat;
	strDomino.TrimLeft(); strDomino.TrimRight();

	switch(key)
	{
	case TK_SDLDQ003_GROUP:
		WriteLog("OnGroup SDLDQ003", len, dat);
		OnGroupList(dat, len);

		//2013.12.02 KSJ 서비스 태우고 enable
		((CButton*)GetDlgItem(IDC_BTN_GROUPUPDATE))->EnableWindow(TRUE);

		break;
	case TK_SDLDQ003_ACCOUNT:
		WriteLog("OnAccount SDLDQ003", len, dat);
		OnGroupAccount(dat, len);
		break;
	case TK_SDLDT003:
		WriteLog("OnUpdate SDLDQ003", len, dat);
		OnGroupUpdate(dat, len);
	}
	return 0;
}

void CMapWnd::OnGroupList( LPCSTR data, int dlen )
{
	struct _ledgerH *ledger = (struct _ledgerH*)data;
	sdldq003_mod *mod = (sdldq003_mod*)(data+L_ledgerH);
	
	m_pGrid[0]->ClearCells(CCellRange(1, -1, -1, -1));

	int gcnt = S2I(mod->orec);
	for(int i=0; i<gcnt; ++i)
	{
		CString gpno = F2S(mod->grid[i].gpno);	gpno.TrimRight();
		CString gpnm = F2S(mod->grid[i].gpnm);	gpnm.TrimRight();
		
		grop_info gi;
		sprintf(gi.gpno, (LPCSTR)gpno);
		sprintf(gi.gpnm, (LPCSTR)gpnm);
		m_gdata[gpno] = gi;

		m_pGrid[0]->SetItemText(i+1, 0, gpno.Right(3));
		m_pGrid[0]->SetItemText(i+1, 1, gpnm);
		m_pGrid[0]->SetItemFgColour(i+1, 0);
		m_pGrid[0]->SetItemFgColour(i+1, 1);
	}

	WritePLog("OnGroupList - 그룹목록 %d", gcnt);

	m_pGrid[0]->Refresh();

	p_grop_data st = m_gdata.begin(), ed = m_gdata.end();
	for(; st!=ed; ++st)
		LoadGroupAccount(st->second.gpno, st->second.gpnm);

	if (m_gdata.empty())
	{
		RefreshRemainAccount();		
		EnableWindow(TRUE);
	}
}

void CMapWnd::LoadGroupAccount( LPCSTR gpno, LPCSTR gpnm, struct _ledgerH *nledger)
{
	vector<char> data( L_ledgerH + sizeof(sdldq003_mid), ' ' );
	
	struct _ledgerH *ledger = (struct _ledgerH*)&data[0];
	if (nledger==NULL)
	{
		GetLedger(ledger);
		CopyMemory(ledger->svcd, "SDLDQ003", 8);
		CopyMemory(ledger->brno, "000", 3);
		CopyMemory(ledger->rcnt, "0030", 4);
		CopyMemory(ledger->odrf, "1", 1);
		CopyMemory(ledger->mkty, "1", 1);
	}
	else
	{
		CopyMemory(ledger, nledger, L_ledgerH);
	}
	
	
	sdldq003_mid *mid = (sdldq003_mid*)&data[L_ledgerH];
	FillMemory(mid, sizeof(mid), ' ');
	CopyMemory(mid->irec, "00001", 5);
	CopyMemory(mid->gubn, "2", 1);
	CopyMemory(mid->type, "10", 2);
	CopyMemory(mid->gpno, gpno, strlen(gpno));
	CopyMemory(mid->gpnm, gpnm, strlen(gpnm));
	
	WriteLog("LoadGroupAccount", data.size(), &data[0]);
	SendTR("piboPBxQ", US_ENC, &data[0], data.size(), TK_SDLDQ003_ACCOUNT);
}

void CMapWnd::OnGroupAccount( LPCSTR data, int dlen )
{
	struct _ledgerH *ledger = (struct _ledgerH*)data;
	sdldq003_mod *mod = (sdldq003_mod*)(data+L_ledgerH);
	
	CString gpno, gpnm, acno, acnm, acpw;
	int mult;
	double rate;
	int gcnt = S2I(mod->orec);
	for(int i=0; i<gcnt; ++i)
	{
		gpno = F2S(mod->grid[i].gpno);	gpno.TrimRight();
		gpnm = F2S(mod->grid[i].gpnm);	gpnm.TrimRight();
		acno = F2S(mod->grid[i].acno);	acno.TrimRight();
		acnm = F2S(mod->grid[i].acnm);	acnm.TrimRight();
		acpw = F2S(mod->grid[i].acpw);	acpw.TrimRight();
		mult = S2I(mod->grid[i].mult);
		rate = S2D(mod->grid[i].rate);
		//m_pass[acno] = acpw; //2014.02.19 KSJ 비밀번호입력했다고 저장하면 안되고 진짜 맞으면 저장

		TRACE("[%s(%s)] %s %s %s %d %.2f\n", gpno, gpnm, acno, acnm, acpw, mult, rate);

		accn_info ai;
		sprintf(ai.acno, (LPCSTR)acno);
		sprintf(ai.acno2, "%.3s-%.2s-%.6s", ((LPCSTR)acno)+0, ((LPCSTR)acno)+3, ((LPCSTR)acno)+5);
		sprintf(ai.acnm, (LPCSTR)acnm);
		sprintf(ai.acpw, (LPCSTR)acpw);
		ai.mult = mult;
		ai.rate = rate;

		m_adata[gpno].push_back(ai);

		vector<accn_info>::iterator pos = find_if(m_owns.begin(), m_owns.end(), op_eq_acno(ai.acno));
		if (pos!=m_owns.end())
		{
			TRACE("Filter - %s %s\n", pos->acno, pos->acnm);
			pos->used = TRUE;
		}
	}

	WritePLog("OnGroupAccount - 그룹[%s] 계좌[%d] 로드", gpno, gcnt);

	if (ledger->next[0]=='Y')
	{
		LoadGroupAccount( (LPCSTR)gpno, (LPCSTR)gpnm, ledger );
	}
	else
	{
		m_gdata[gpno].load = TRUE;

		WritePLog("OnGroupAccount - 그룹[%s] 총계좌[%d] 로드 완료", gpno, m_adata[gpno].size());

		if (IsLoadingComplete() && !m_gdata.empty())
		{
			SelectGroup(1);
			RefreshRemainAccount();
			EnableWindow(TRUE);
		}
	}
}

BOOL CMapWnd::IsLoadingComplete()
{
	p_grop_data st = m_gdata.begin();
	p_grop_data ed = m_gdata.end();
	for(; st!=ed; ++st)
	{
		if (st->second.load==FALSE)
			return FALSE;
	}
	return TRUE;
}

void CMapWnd::Init()
{
	m_home = Variant(homeCC, _T(""));
	m_user = Variant(userCC, _T(""));
	m_name = Variant(nameCC, _T(""));
	Variant(titleCC, _T("[6500] 복수계좌 그룹등록"));

	// Loading images
	{
		char buff[1024];
		CString path, img_path;
		
		path.Format(_T("%s\\tab\\PALETTE.INI"), (LPCSTR)m_home);
		int readL = GetPrivateProfileString("General", "Palette", "Blue", buff, sizeof(buff), path);
		if (readL==0) return;
		
		img_path.Format(_T("%s\\image\\axpanel%s1_"), (LPCSTR)m_home, buff);
		
		m_hRndBmp[0] = GetAxBitmap((LPCSTR)( img_path + _T("lt.bmp")));
		m_hRndBmp[1] = GetAxBitmap((LPCSTR)( img_path + _T("rt.bmp")));
		m_hRndBmp[2] = GetAxBitmap((LPCSTR)( img_path + _T("lb.bmp")));
		m_hRndBmp[3] = GetAxBitmap((LPCSTR)( img_path + _T("rb.bmp")));
		
		m_hBtn2[0] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\2btn.bmp")));
		m_hBtn2[1] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\2btn_dn.bmp")));
		m_hBtn2[2] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\2btn_en.bmp")));
		
		m_hBtn4[0] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\4btn.bmp")));
		m_hBtn4[1] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\4btn_dn.bmp")));
		m_hBtn4[2] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\4btn_en.bmp")));
	}

	m_crOdd = GetIndexColor(COLOR_ODD_ROW);
	m_crEven = GetIndexColor(COLOR_EVEN_ROW);
	m_crHeader = GetIndexColor(COLOR_HEADER);
	m_crSelect = GetIndexColor(COLOR_SELECT_ROW);

	m_rcGroupList[0].SetRect(0, 0, 280, 25);
	m_rcGroupList[1].SetRect(0, 25, 280, 390);

	m_rcGroupAccount[0].SetRect(285, 0, 560, 25);
	m_rcGroupAccount[1].SetRect(285, 25, 560, 390);

	m_rcRemainAccount[0].SetRect(565, 0, 775, 25);
	m_rcRemainAccount[1].SetRect(565, 25, 775, 390);

	MakeBackground();
}

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this);

	CDC mdc;	
	CRect rc;

	GetClientRect(rc);
	mdc.CreateCompatibleDC(&dc);	
	CBitmap *oldBmp = mdc.SelectObject(&m_hBackground);
	dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &mdc, 0, 0, SRCCOPY);
	mdc.SelectObject(oldBmp);
	mdc.DeleteDC();	
}

void CMapWnd::MakeBackground()
{
	CRect tRc, rc;
	GetClientRect(&tRc);
	
	int width = tRc.Width(), height = tRc.Height();
	
	CFont *oldFont;
	CBitmap *oldBmp;
	CDC *pdc = GetDC(), mdc;
	
	m_hBackground.DeleteObject();
	m_hBackground.CreateCompatibleBitmap(pdc, width, height);
	
	mdc.CreateCompatibleDC(pdc);

	oldFont = mdc.SelectObject(m_pFont);
	oldBmp = mdc.SelectObject(&m_hBackground);
	
	mdc.FillSolidRect(&tRc, m_crBk);

	// Drawing 
	DrawRoundRectangle(&mdc, m_rcGroupList[0], m_crHeader);
	DrawRoundRectangle(&mdc, m_rcGroupList[1], m_crBodyBk);
	DrawRoundRectangle(&mdc, m_rcGroupAccount[0], m_crHeader);
	DrawRoundRectangle(&mdc, m_rcGroupAccount[1], m_crBodyBk);
	DrawRoundRectangle(&mdc, m_rcRemainAccount[0], m_crHeader);
	DrawRoundRectangle(&mdc, m_rcRemainAccount[1], m_crBodyBk);

	CSize size;
	
	mdc.SetBkMode(TRANSPARENT);

	rc = m_rcGroupList[0];
	size = mdc.GetOutputTextExtent("그룹계좌");
	mdc.TextOut( rc.left+(rc.Width()-size.cx)/2, rc.top+(rc.Height()-size.cy)/2, "그룹계좌" );

	rc = m_rcGroupAccount[0];
	size = mdc.GetOutputTextExtent("계좌목록");
	mdc.TextOut( rc.left+(rc.Width()-size.cx)/2, rc.top+(rc.Height()-size.cy)/2, "계좌목록" );

	rc = m_rcRemainAccount[0];
	size = mdc.GetOutputTextExtent("미등록계좌");
	mdc.TextOut( rc.left+(rc.Width()-size.cx)/2, rc.top+(rc.Height()-size.cy)/2, "미등록계좌" );

	mdc.SelectObject(oldBmp);
	mdc.SelectObject(m_pFont);
	mdc.DeleteDC();
	
	ReleaseDC(pdc);
}

void CMapWnd::DrawRoundRectangle( CDC *pDC, CRect rc, COLORREF crBk )
{
	CSize size(2,2);
	pDC->FillSolidRect((LPRECT)rc, m_crBk);
	rc.DeflateRect(size);
	pDC->FillSolidRect((LPRECT)rc, crBk);
	DrawBitmap(pDC, rc.left,    rc.top,      5, 5, m_hRndBmp[0]->operator HBITMAP());
	DrawBitmap(pDC, rc.right-5, rc.top,      5, 5, m_hRndBmp[1]->operator HBITMAP());
	DrawBitmap(pDC, rc.left,    rc.bottom-5, 5, 5, m_hRndBmp[2]->operator HBITMAP());
	DrawBitmap(pDC, rc.right-5, rc.bottom-5, 5, 5, m_hRndBmp[3]->operator HBITMAP());
}

void CMapWnd::DrawBitmap( CDC *pDC, int x, int y, int w, int h, HBITMAP hBitmap, COLORREF tCol/*=0xFF00FF*/ )
{
	HDC hDC = ::CreateCompatibleDC(pDC->m_hDC);
	HBITMAP tBmp = (HBITMAP)::SelectObject(hDC, hBitmap);
	::TransparentBlt(pDC->m_hDC, x, y, w, h, hDC, 0, 0, w, h, tCol);
	::SelectObject(hDC, tBmp);
	::DeleteObject(hDC);
}

void CMapWnd::DrawBitmap( CDC *pDC, CRect *pRC, HBITMAP hBitmap, COLORREF tCol/*=0xFF00FF*/ )
{
	DrawBitmap(pDC, pRC->left, pRC->top, pRC->Width(), pRC->Height(), hBitmap, tCol);
}

void CMapWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	CRect rc;
	
	rc = m_rcGroupList[1];
	rc.DeflateRect(PADDING_VALUE, PADDING_VALUE); rc.top += PADDING_VALUE/2+DEF_STATIC_HEIGHT;
	m_pGrid[0]->MoveWindow(&rc, FALSE);

	rc = m_rcGroupAccount[1];
	rc.DeflateRect(PADDING_VALUE, PADDING_VALUE); rc.top += PADDING_VALUE/2+DEF_STATIC_HEIGHT;
	m_pGrid[1]->MoveWindow(&rc, FALSE);

	rc = m_rcRemainAccount[1];
	rc.DeflateRect(PADDING_VALUE, PADDING_VALUE); rc.top += PADDING_VALUE/2+DEF_STATIC_HEIGHT;
	m_pGrid[2]->MoveWindow(&rc, FALSE);

	MakeBackground();
}

HBRUSH CMapWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr;
	UINT nID = pWnd->GetDlgCtrlID();

	if (nID==IDC_STATIC_CONTROL)
		hbr = ::CreateSolidBrush(m_crBodyBk);
	else
		hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);	

	return hbr; 
}

void CMapWnd::InitGrid()
{
	int n, i, rows, cols;
	CGridCtrl *pGrid;
	CRect rc(0, 0, 0, 0);
	
	// GroupList grid setting (0, 25, 280, 390);
	pGrid = m_pGrid[0] = new CGridCtrl();
	if (pGrid->Create(rc, this, IDC_GRID_GROUPLIST))
	{
		pGrid->SetDoubleBuffering(TRUE);
		pGrid->SetDefCellHeight(GRID_CELL_HEIGHT);
		pGrid->GetDefaultCell(TRUE, FALSE)->SetBackClr( GetIndexColor(COLOR_GRIDHEAD) );
		pGrid->GetDefaultCell(TRUE, FALSE)->SetTextClr( GetIndexColor(COLOR_GRIDHEADTXT) );
		pGrid->SetSingleColSelection(FALSE);
		pGrid->SetEditable(FALSE);
		pGrid->SetSingleRowSelection(TRUE);
		pGrid->SetFrameFocusCell(FALSE);
		pGrid->EnableSelection(FALSE);

		pGrid->SetColumnCount(2);
		pGrid->SetRowCount(MAX_GROUPLIST+1);
		pGrid->SetFixedRowCount(1);

		rows = pGrid->GetRowCount();
		cols = pGrid->GetColumnCount();
		LPCSTR header[] = {"번호", "그룹명"};
		int widths[] = {60, 189};
		for(n=0; n<cols; ++n)
		{
			pGrid->SetColumnWidth(n, widths[n]);
			pGrid->SetItemText(0, n, header[n]);
			pGrid->SetItemFormat(0, n, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
		for(n=1; n<rows; ++n)
		{
			COLORREF col = (n%2) ? m_crEven : m_crOdd;
			for(i=0; i<cols; ++i)
			{
				if (i==0) pGrid->SetItemFormat(n, i, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				pGrid->SetItemBkColour(n, i, col);
			}
		}
		
	}

	// GroupAccount grid setting (285, 25, 560, 390);
	pGrid = m_pGrid[1] = new CGridCtrl();
	if (pGrid->Create(rc, this, IDC_GRID_GROUPACCOUNT))
	{

		pGrid->SetDoubleBuffering(TRUE);
		pGrid->SetDefCellHeight(GRID_CELL_HEIGHT);
		pGrid->GetDefaultCell(TRUE, FALSE)->SetBackClr( GetIndexColor(COLOR_GRIDHEAD) );
		pGrid->GetDefaultCell(TRUE, FALSE)->SetTextClr( GetIndexColor(COLOR_GRIDHEADTXT) );
		pGrid->SetSingleColSelection(FALSE);
		pGrid->SetSingleRowSelection(TRUE);
		pGrid->SetFrameFocusCell(FALSE);
		pGrid->EnableSelection(FALSE);

		pGrid->SetColumnCount(5);
		pGrid->SetRowCount(MAX_GROUPACCOUNT+1);
		pGrid->SetFixedRowCount(1);

		rows = pGrid->GetRowCount();
		cols = pGrid->GetColumnCount();
		LPCSTR header[] = {"계좌번호", "계좌명", "비밀번호", "비율", "배수"};
		int widths[] = {90, 99, 55, 0, 0};
		for(n=0; n<cols; ++n)
		{
			pGrid->SetColumnWidth(n, widths[n]);
			pGrid->SetItemText(0, n, header[n]);
			pGrid->SetItemFormat(0, n, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
		for(n=1; n<rows; ++n)
		{
			COLORREF col = (n%2) ? m_crEven : m_crOdd;
			for(i=0; i<cols; ++i)
			{
				pGrid->SetItemBkColour(n, i, col);
				switch(i)
				{
				case 0:
					pGrid->SetItemFormat(n, i, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
					pGrid->SetItemState(n, i, pGrid->GetItemState(n,i)|GVIS_READONLY);
					break;
				case 1:
					pGrid->SetItemFormat(n, i, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
					pGrid->SetItemState(n, i, pGrid->GetItemState(n,i)|GVIS_READONLY);
					break;
				case 2:
					pGrid->SetItemFormat(n, i, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
					pGrid->SetCellType(n, i, RUNTIME_CLASS(CGridCellPassword));
					break;
				case 3:
					pGrid->SetItemFormat(n, i, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
					pGrid->SetCellType(n, i, RUNTIME_CLASS(CGridCellNumeric));
					break;
				case 4:
					pGrid->SetItemFormat(n, i, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
					pGrid->SetCellType(n, i, RUNTIME_CLASS(CGridCellNumeric));
					break;
				}
			}
		}
	}

	// RemainAccount grid setting (560, 25, 775, 390);
	pGrid = m_pGrid[2] = new CGridCtrl();
	if (pGrid->Create(rc, this, IDC_GRID_REMAINACCOUNT))
	{
		pGrid->SetDoubleBuffering(TRUE);
		pGrid->SetDefCellHeight(GRID_CELL_HEIGHT);
		pGrid->GetDefaultCell(TRUE, FALSE)->SetBackClr( GetIndexColor(COLOR_GRIDHEAD) );
		pGrid->GetDefaultCell(TRUE, FALSE)->SetTextClr( GetIndexColor(COLOR_GRIDHEADTXT) );
		pGrid->SetSingleColSelection(FALSE);
		pGrid->SetEditable(FALSE);
		pGrid->SetSingleRowSelection(TRUE);
		pGrid->SetFrameFocusCell(FALSE);
		pGrid->EnableSelection(FALSE);

		pGrid->SetColumnCount(2);
		pGrid->SetRowCount(1000);
		pGrid->SetFixedRowCount(1);

		rows = pGrid->GetRowCount();
		cols = pGrid->GetColumnCount();
		LPCSTR header[] = {"계좌번호", "계좌명"};
		int widths[] = {89, 90};
		for(n=0; n<cols; ++n)
		{
			pGrid->SetColumnWidth(n, widths[n]);
			pGrid->SetItemText(0, n, header[n]);
			pGrid->SetItemFormat(0, n, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
		for(n=1; n<rows; ++n)
		{
			COLORREF col = (n%2) ? m_crEven : m_crOdd;
			for(i=0; i<cols; ++i)
			{
				pGrid->SetItemBkColour(n, i, col);
				if (i==0) pGrid->SetItemFormat(n, i, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				else      pGrid->SetItemFormat(n, i, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			}
		}

	}
}

void CMapWnd::InitControl()
{
	CRect rc;

	rc = m_rcGroupList[1];
	rc.top += PADDING_VALUE;
	rc.left += PADDING_VALUE;
	rc.right = rc.left + 60;
	rc.bottom = rc.top + DEF_STATIC_HEIGHT;
	
	m_btnGroupAdd = CreateImgButton("새 그룹", IDC_BTN_GROUPADD, m_hBtn4);
	m_btnGroupAdd->MoveWindow(&rc, FALSE);
	
	rc.OffsetRect(60+PADDING_VALUE, 0);
	m_btnGroupDelete = CreateImgButton("그룹삭제", IDC_BTN_GROUPDELETE, m_hBtn4);
	m_btnGroupDelete->MoveWindow(&rc, FALSE);
	
	rc.OffsetRect(60+PADDING_VALUE, 0);
	m_btnReload = CreateImgButton("조  회", IDC_BTN_RELOAD, m_hBtn4);
	m_btnReload->MoveWindow(&rc, FALSE);
	
	//2014.06.09 KSJ 채권은 제한종목 등록이 없다.
// 	rc.OffsetRect(60+PADDING_VALUE, 0);
// 	m_btnLimit = CreateImgButton("제한종목", IDC_BTN_LIMITSTOCK, m_hBtn4);
// 	m_btnLimit->MoveWindow(&rc, FALSE);
	
	rc = m_rcGroupAccount[1];
	rc.top += PADDING_VALUE;
	rc.left += PADDING_VALUE;
	rc.bottom = rc.top + DEF_STATIC_HEIGHT;
	rc.right = rc.left + 40;
	if (m_stGroupName.Create("그룹명", WS_CHILD|WS_VISIBLE|SS_CENTER|SS_CENTERIMAGE, rc, this, IDC_STATIC_CONTROL))
	{
		m_stGroupName.SetFont(m_pFont, FALSE);
	}
	
	rc.OffsetRect(40, 0);
	rc.top += 1; rc.bottom -= 1;
	rc.right = rc.left + 155;
	if (m_edGroupName.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT, rc, this, IDC_EDIT_GROUPNAME))
	{
		m_edGroupName.SetFont(m_pFont);
		m_edGroupName.SetLimitText(40);
	}
	
	rc.OffsetRect(155+PADDING_VALUE, 0);
	rc.right = rc.left + 60;
	m_btnGroupUpdate = CreateImgButton("그룹저장", IDC_BTN_GROUPUPDATE, m_hBtn4);
	m_btnGroupUpdate->MoveWindow(&rc, FALSE);
}

void CMapWnd::SelectGroup( int grid_idx )
{
	ClearUsedStatus();
	RefreshRemainAccount();

	if (grid_idx<1) return;

	grop_info *pgi;
	vector<accn_info> *pai;

	CString no = m_pGrid[0]->GetItemText(grid_idx, 0);
	if (no.IsEmpty()) return;

	CString gpno = m_usid + "-" + no;
	CString gpnm = m_pGrid[0]->GetItemText(grid_idx, 1);

	WritePLog("---------------------------------------------------------------");
	if ( (m_gdata.find(gpno)==m_gdata.end()) || (m_adata.find(gpno)==m_adata.end()) )
	{
		pgi = m_pCurGroup = &m_TempGroup;
		pai = m_pCurAccount = &m_TempAccount;
		sprintf(pgi->gpno, (LPCSTR)gpno);
		sprintf(pgi->gpnm, (LPCSTR)gpnm);
		WritePLog("SelectGroup %s[%s] 계좌[%d] 신규상태!!!!", pgi->gpno, pgi->gpnm, pai->size());
	}
	else
	{
		pgi = m_pCurGroup = &m_gdata[gpno];
		pai = m_pCurAccount = &m_adata[gpno];
		WritePLog("SelectGroup %s[%s] 계좌[%d]", pgi->gpno, pgi->gpnm, pai->size());
	}

	TRACE("SelectGroup %d\n", grid_idx);

	char buff[32];

	SelectGridRow(m_pGrid[0], grid_idx);
	SelectGridRow(m_pGrid[1], -1);
	SelectGridRow(m_pGrid[2], -1);
	ClearGrid(m_pGrid[1], FALSE);

//	m_pass.clear();	//2014.02.19 KSJ 처음에만 지우고 나중에는 계속 관리한다.
	for(int idx=0; idx<pai->size(); ++idx)
	{
		m_pGrid[1]->SetItemText(idx+1, 0, (*pai)[idx].acno2);
		m_pGrid[1]->SetItemText(idx+1, 1, (*pai)[idx].acnm);
		m_pGrid[1]->SetItemText(idx+1, 2, "****");
		CString strAcno, strAcpw;

		strAcno = F2S((*pai)[idx].acno);
		strAcpw = F2S((*pai)[idx].acpw);

		m_pass[ (*pai)[idx].acno ] = (*pai)[idx].acpw;

		sprintf(buff, "%.02f", (*pai)[idx].rate);
		m_pGrid[1]->SetItemText(idx+1, 3, buff);
		
		sprintf(buff, "%d", (*pai)[idx].mult);
		m_pGrid[1]->SetItemText(idx+1, 4, buff);

		TRACE("ROW[%2d] %s %s\n", idx+1, (*pai)[idx].acno2, (*pai)[idx].acnm);
	}

	m_pGrid[0]->Refresh();
	m_pGrid[1]->Refresh();
	m_pGrid[2]->Refresh();
	

	m_edGroupName.SetWindowText(m_pCurGroup->gpnm);
}

BOOL CMapWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NM_GRIDVIEW *gv = (NM_GRIDVIEW*)lParam;
	GV_DISPINFO *pi = (GV_DISPINFO*)lParam;
	if (wParam==IDC_GRID_GROUPLIST)
	{
		if (pi->hdr.code==GVN_SELCHANGED && pi->item.row>0)
		{
			SelectGroup(pi->item.row);
		}

		//2013.08.08 KSJ 헤더 더블클릭시 정렬
		else if (gv->hdr.code==NM_DBLCLK && gv->iRow == 0 && gv->iColumn == 1)
		{
			m_pGrid[0]->SortTextItems(gv->iColumn, !m_pGrid[0]->GetSortAscending(), !m_pGrid[0]->GetSortAscending());
			m_pGrid[0]->Refresh();
		}

	}
	else if (wParam==IDC_GRID_GROUPACCOUNT)
	{
		if (pi->hdr.code==GVN_ENDLABELEDIT)	// 패스워드 
		{
			
			if (pi->item.col==2)
			{
				CString acno = m_pGrid[1]->GetItemText(pi->item.row, 0); acno.Replace("-", "");
				CString pass = m_pGrid[1]->GetItemText(pi->item.row, pi->item.col);
				char buff[32] = {0, };
				FillMemory(buff, pass.GetLength(), '*');

				m_pass[acno] = pass; //2014.02.19 KSJ 비밀번호입력했다고 저장하면 안되고 진짜 맞으면 저장
				/*
				vector<accn_info>::iterator pos = find_if(m_pCurAccount->begin(), m_pCurAccount->end(), op_eq_acno(acno));
				if (pos!=m_pCurAccount->end())
					sprintf(pos->acpw, (LPCSTR)pass);
				*/

				m_pGrid[1]->SetItemText(pi->item.row, pi->item.col, buff);
			}
			else if (pi->item.col==3)	// 비율
			{
				CString acno = m_pGrid[1]->GetItemText(pi->item.row, 0); acno.Replace("-", "");
				double rate = atof(m_pGrid[1]->GetItemText(pi->item.row, pi->item.col));
				
				vector<accn_info>::iterator pos = find_if(m_pCurAccount->begin(), m_pCurAccount->end(), op_eq_acno(acno));
				if (pos!=m_pCurAccount->end())
				{
					char buff[32];
					if (rate<=0.0)
					{
						sprintf(buff, "%.2f", (pos->rate<=0.0) ? 1.0 : pos->rate);
						m_pGrid[1]->SetItemText(pi->item.row, pi->item.col, buff);
					}
					else
					{
						sprintf(buff, "%.2f", rate);
						m_pGrid[1]->SetItemText(pi->item.row, pi->item.col, buff);
						pos->rate = atof(buff);
					}
				}
			}
			else if (pi->item.col==4)	// 배수
			{
				CString acno = m_pGrid[1]->GetItemText(pi->item.row, 0); acno.Replace("-", "");
				int mult = atoi(m_pGrid[1]->GetItemText(pi->item.row, pi->item.col));
				
				vector<accn_info>::iterator pos = find_if(m_pCurAccount->begin(), m_pCurAccount->end(), op_eq_acno(acno));
				if (pos!=m_pCurAccount->end())
				{
					char buff[32];
					if (mult<=0)
					{
						sprintf(buff, "%d", (pos->mult<=0) ? 1 : pos->mult);
						m_pGrid[1]->SetItemText(pi->item.row, pi->item.col, buff);
					}
					else
					{
						sprintf(buff, "%d", mult);
						m_pGrid[1]->SetItemText(pi->item.row, pi->item.col, buff);
						pos->mult = atoi(buff);
					}
				}
			}
		}
		else if (pi->hdr.code==GVN_SELCHANGED && pi->item.row>0)
		{
			SelectGridRow(m_pGrid[1], pi->item.row);
		}
		else if (gv->hdr.code==NM_DBLCLK && gv->iRow>0 && gv->iColumn<2)
		{
			CString acnm = m_pGrid[1]->GetItemText(gv->iRow, 1);
			CString acno2 = m_pGrid[1]->GetItemText(gv->iRow, 0);
			CString acno = acno2;
			acno.TrimRight();
			acno.Replace("-","");

			if (!acno.IsEmpty())
			{
				accn_info ai;
				sprintf(ai.acno, (LPCSTR)acno);
				sprintf(ai.acno2, (LPCSTR)acno2);
				sprintf(ai.acnm, (LPCSTR)acnm);

				DelGroupAccount(&ai);
			}
		}
		//2013.08.08 KSJ 헤더 더블클릭시 정렬
		else if (gv->hdr.code==NM_DBLCLK && gv->iRow == 0 && gv->iColumn<2)
		{
			m_pGrid[1]->SortTextItems(gv->iColumn, !m_pGrid[1]->GetSortAscending(), !m_pGrid[1]->GetSortAscending());
			m_pGrid[1]->Refresh();
		}
	}
	else if (wParam==IDC_GRID_REMAINACCOUNT)
	{
		if (pi->hdr.code==GVN_SELCHANGED && pi->item.row>0)
		{
			SelectGridRow(m_pGrid[2], pi->item.row);
		}
		else if (gv->hdr.code==NM_DBLCLK && gv->iRow != 0)
		{
			CString acno = m_pGrid[2]->GetItemText(gv->iRow, 0);
			acno.Replace("-", "");
			
			vector<accn_info>::iterator pos = find_if(m_owns.begin(), m_owns.end(), op_eq_acno(acno));
			if (pos!=m_owns.end())
				AddGroupAccount( &*pos );
		}
		//2013.08.08 KSJ 헤더 더블클릭시 정렬
		else if (gv->hdr.code==NM_DBLCLK && gv->iRow == 0)
		{
			m_pGrid[2]->SortTextItems(gv->iColumn, !m_pGrid[2]->GetSortAscending(), !m_pGrid[2]->GetSortAscending());
			m_pGrid[2]->Refresh();
		}
	}
	return CWnd::OnNotify(wParam, lParam, pResult);
}

void CMapWnd::ClearGrid( CGridCtrl *pGrid, BOOL bRepaint )
{
	int col=1, row=0;
	int rows = pGrid->GetRowCount();
	int cols = pGrid->GetColumnCount();

	for(row=1; row<rows; ++row)
	{
		for(col=0; col<cols; ++col)
			pGrid->SetItemText(row, col, "");
	}
	if (bRepaint) pGrid->Refresh();
}

void CMapWnd::RefreshRemainAccount()
{	
	ClearGrid(m_pGrid[2]);

	vector<accn_info>::iterator st, ed;
	st = m_owns.begin();
	ed = m_owns.end();
	int row = 0;
	for(row=1; st!=ed; ++st)
	{
		if (st->used) continue;

		m_pGrid[2]->SetItemText(row, 0, st->acno2);
		m_pGrid[2]->SetItemText(row, 1, st->acnm);

		++row;
	}
	m_pGrid[2]->SetRowCount(row > 20 ? row:20);	//2014.06.30 KSJ 
	m_pGrid[2]->Refresh();
}

void CMapWnd::SelectGridRow( CGridCtrl *pGrid, int idx )
{
	for(int n=0; n<3; ++n)
	{
		if (m_pGrid[n]==pGrid)
		{
			m_pGridSelectIdx[n] = idx;
			break;
		}
	}

	CDC *pDC = pGrid->GetDC();
	int cols = pGrid->GetColumnCount();
	for(int row=1; row<pGrid->GetRowCount(); ++row)
	{
		COLORREF col_bg;
		if (row==idx)	col_bg = m_crSelect;
		else			col_bg = (row%2) ? m_crEven : m_crOdd;
				
		if (pGrid->GetItemBkColour(row, 0)!=col_bg)
		{
			for(int col=0; col<cols; ++col)
			{
				pGrid->SetItemBkColour(row, col, col_bg);
				pGrid->RedrawCell(row, col, pDC);
			}
		}
	}
	pGrid->ReleaseDC(pDC);

	((CButton*)GetDlgItem(IDC_BTN_GROUPUPDATE))->EnableWindow(TRUE);
}

void CMapWnd::AddGroupAccount( accn_info *ai )
{
	if (!m_pCurGroup)
	{
		MessageBox("그룹을 생성 혹은 선택하신후 계좌를 추가하시기 바랍니다.", "IBK투자증권", MB_OK);
		return;
	}

	int row;
	for(row=1; row<m_pGrid[1]->GetRowCount(); ++row)
	{
		CString acno2 = m_pGrid[1]->GetItemText(row, 0);
		if ( acno2 == ai->acno2 )
		{
			MessageBox("이미 등록된 계좌번호 입니다.", "IBK투자증권");
			return;
		}
		if ( acno2.IsEmpty() ) break;
	}

	if (row>=m_pGrid[1]->GetRowCount())
	{
		CString msg;
		msg.Format("그룹당 최대 %d개 까지만 추가할 수 있습니다.", m_pGrid[1]->GetRowCount()-1);
		MessageBox(msg, "IBK투자증권", MB_OK|MB_ICONWARNING);
		return;
	}

	WritePLog("AddGroupAccount - group[%s][%s] accn[%s][%s]", m_pCurGroup->gpno, m_pCurGroup->gpnm, ai->acno2, ai->acnm );

	m_pGrid[1]->SetItemText(row, 0, ai->acno2);
	m_pGrid[1]->SetItemText(row, 1, ai->acnm);

	//2014.02.19 KSJ
	bool bExist = false;

	CString strAcpw, strAcno, strOrgAcno = F2S(ai->acno);

	map<CString,CString>::iterator pos = m_pass.begin();
	for(; pos!=m_pass.end(); ++pos)
	{
		strAcno.Format("%s", (LPCSTR)pos->first);
		strAcpw.Format("%s", (LPCSTR)pos->second);
		if(!strAcno.Compare(strOrgAcno) && !strAcpw.IsEmpty())
		{
			bExist = true;
			break;
		}
	}

	if(bExist)	m_pGrid[1]->SetItemText(row, 2, "****");	//2014.02.19 KSJ 비번이 있는계좌이면 별표
	else		m_pGrid[1]->SetItemText(row, 2, "");		//2014.02.19 KSJ 비번이 없는계좌이면 빈값
	//2014.02.19 KSJ END

	m_pGrid[1]->SetItemText(row, 3, "1.00");
	m_pGrid[1]->SetItemText(row, 4, "1");
	m_pGrid[1]->RedrawRow(row);
}

void CMapWnd::UpdateGroup()
{
	//2013.12.02 KSJ 저장버튼 두번 안누르게 disable
	((CButton*)GetDlgItem(IDC_BTN_GROUPUPDATE))->EnableWindow(FALSE);

	CGridCtrl *pGrid = m_pGrid[1];
	int row, rows = pGrid->GetRowCount();
	vector<accn_info> updata;
	
	//2014.05.26 KSJ 그룹의 계좌가 40개를 넘길수 없다. 넘기면 에러가나고 FEP가 끊긴다..
	CString msg;

	for(row=1; row<rows; ++row)
	{
		CString acno;
		acno = pGrid->GetItemText(row, 0);

		if (acno.IsEmpty()) break;
	}

	if(row -1 > 40)
	{
		msg.Format("한 그룹의 계좌가 40개를 넘을 수 없습니다.\n");
		MessageBox(msg, "IBK투자증권");
		((CButton*)GetDlgItem(IDC_BTN_GROUPUPDATE))->EnableWindow(TRUE);
		return;
	}

	//2014.05.26 KSJ END

#if 0
	CString stmp, stmp2;
	map<CString,CString>::iterator pos = m_pass.begin();
	for(; pos!=m_pass.end(); ++pos)
	{
		stmp2.Format("%s %s\r\n", (LPCSTR)pos->first, (LPCSTR)pos->second);
		stmp += stmp2;
	}
	MessageBox(stmp);
#endif

	// validation
	for(row=1; row<rows; ++row)
	{
		CString acno2 = pGrid->GetItemText(row, 0); 
		CString acno = acno2;
		acno.TrimRight();
		acno.Replace("-", "");
		if (acno.IsEmpty()) break;
		
		CString acnm, pass, rate, mult, msg;
		acnm = pGrid->GetItemText(row, 1); acnm.TrimRight();
		rate = pGrid->GetItemText(row, 3); rate.TrimRight();
		mult = pGrid->GetItemText(row, 4); mult.TrimRight();
 		pass = m_pass[acno];

		accn_info ai;
		sprintf(ai.acno, (LPCSTR)acno);
		sprintf(ai.acnm, (LPCSTR)acnm);
		sprintf(ai.acpw, (LPCSTR)pass);
		ai.rate = atof(rate);
		ai.mult = atoi(mult);
		updata.push_back(ai);
		
		if (pass.IsEmpty())
		{
			msg.Format("%s 계좌의 비밀번호를 입력하십시오", (LPCSTR)acno2);
			MessageBox(msg, "IBK투자증권");
			pGrid->SetSelectedRange(row, 2, row, 2);
			SelectGridRow(pGrid, row);
			return;
		}

		if (rate.IsEmpty())
		{
			msg.Format("%s 계좌의 비율을 입력하십시오", (LPCSTR)acno2);
			MessageBox(msg, "IBK투자증권");
			pGrid->SetSelectedRange(row, 2, row, 3);
			SelectGridRow(pGrid, row);
			return;
		}

		if (mult.IsEmpty())
		{
			msg.Format("%s 계좌의 배수를 입력하십시오", (LPCSTR)acno2);
			MessageBox(msg, "IBK투자증권");
			pGrid->SetSelectedRange(row, 2, row, 4);
			SelectGridRow(pGrid, row);
			return;
		}
	}

	if(m_pCurAccount == NULL)
	{
		((CButton*)GetDlgItem(IDC_BTN_GROUPUPDATE))->EnableWindow(TRUE);
		return;
	}

	CString imsg;
	vector<accn_info> tmp(m_pCurAccount->begin(), m_pCurAccount->end());
	for(row=1; row<rows; ++row)
	{
		CString acno, acno2, acnm, str;
		acno2 = pGrid->GetItemText(row, 0); acno2.TrimRight();
		acno = acno2; acno.Replace("-", "");
		acnm = pGrid->GetItemText(row, 1); acnm.TrimRight();

		if (acno.IsEmpty()) break;

		vector<accn_info>::iterator pos = find_if(tmp.begin(), tmp.end(), op_eq_acno(acno));
		if (pos==tmp.end()) // 추가
		{
			str.Format("    %s %s\r\n", (LPCSTR)acno2, (LPCSTR)acnm);
			imsg += str;
		}
		else tmp.erase(pos);
	}
	if (!imsg.IsEmpty())
		 imsg = "다음과 같이 변경하시겠습니까?\r\n\r\n[추가]\r\n" + imsg;

	// update 가 0이면 그룹삭제임.
	if (!tmp.empty() && !updata.empty())
	{
		imsg += "[삭제]\r\n";
		for(int n=0; n<tmp.size(); ++n)
		{
			CString str;
			str.Format("    %s %s\r\n", tmp[n].acno2, tmp[n].acnm);
			imsg += str;
		}
	}

	if (imsg.IsEmpty())
	{
		if (updata.empty())
		{
			if (m_pCurAccount==&m_TempAccount)
			{
				MessageBox("[신규그룹] 계좌를 추가하신 후 저장하시기 바랍니다.", "IBK투자증권", MB_OK);
				//2013.12.02 KSJ 아니오 눌렀을때 다시 enable
				((CButton*)GetDlgItem(IDC_BTN_GROUPUPDATE))->EnableWindow(TRUE);
				return;
			}	
			else
			{
				imsg = "계좌가 존재하지 않습니다. 그룹을 삭제하시겠습니까?";
			}
		}
		else
		{
			imsg = "그룹설정을 변경하시겠습니까?";
		}
	}
		
	//2014.04.07 KSJ update를 m_updateAcnn에 복사
	m_updateAcnn = updata;
	
	if (IDYES==MessageBox(imsg, "IBK투자증권", MB_YESNO|MB_ICONWARNING))
	{
		char field[32];
		CString gpnm, gpno = m_pCurGroup->gpno;
		m_edGroupName.GetWindowText(gpnm);
		gpnm.TrimLeft(); gpnm.TrimRight();

		vector<char> buff(1024*64, ' ');
		
		struct _ledgerH *ledger = (struct _ledgerH*)&buff[0];
		GetLedger(ledger);
		CopyMemory(ledger->svcd, "SDLDT003", 8);
		CopyMemory(ledger->brno, "000", 3);
		CopyMemory(ledger->rcnt, "0000", 4);
		CopyMemory(ledger->odrf, "1", 1);
		CopyMemory(ledger->mkty, "1", 1);
	
		sdldt003_mid *mid = (sdldt003_mid*)&buff[L_ledgerH];
		FillMemory(mid, sizeof(sdldt003_mid), ' ');
		CopyMemory(mid->irec, "00001", 5);
		if (m_pCurGroup==&m_TempGroup)
			CopyMemory(mid->gubn, "I", 1);
		else
			CopyMemory(mid->gubn, updata.empty() ? "D" : "U", 1);
		
		CopyMemory(mid->type, "10", 2);
		CopyMemory(mid->gpno, (LPCSTR)gpno, gpno.GetLength());
		CopyMemory(mid->gpnm, (LPCSTR)gpnm, gpnm.GetLength());
		sprintf(field, "%05d", updata.size());
		CopyMemory(mid->orec, field, strlen(field));

		for(int n=0; n<updata.size(); ++n)
		{
			accn_info *ai = &updata[n];
			sdldt003_grid *grid = (sdldt003_grid*)&buff[ L_ledgerH + sizeof(sdldt003_mid) + (n*sizeof(sdldt003_grid)) ];
			CopyMemory(grid->acno, ai->acno, strlen(ai->acno));
			CopyMemory(grid->acnm, ai->acnm, strlen(ai->acnm));
			CopyMemory(grid->acpw, ai->acpw, strlen(ai->acpw));
			sprintf(field, "+%016.010f", ai->rate);
			CopyMemory(grid->rate, field, strlen(field));
			sprintf(field, "+%05d.%010d", ai->mult, 0);
			CopyMemory(grid->mult, field, strlen(field));
		}

		WritePLog("UpdateGroup %s[%s] - 업데이트 %d 계좌", (LPCSTR)gpno, (LPCSTR)gpnm, updata.size());

		buff.resize( L_ledgerH + sizeof(sdldt003_mid) + (updata.size()*sizeof(sdldt003_grid)) );
		WriteLog("UpdateGroup", buff.size(), &buff[0]);
		
// 		CString strTemp(&buff[0], buff.size());
// 		OutputDebugString("[KSJ] send[" + strTemp + "]" );
		SendTR("piboPBxQ", US_ENC, &buff[0], buff.size(), TK_SDLDT003);
	}
	else
	{
		//2013.12.02 KSJ 아니오 눌렀을때 다시 enable
		((CButton*)GetDlgItem(IDC_BTN_GROUPUPDATE))->EnableWindow(TRUE);
	}
}

CfxImgButton* CMapWnd::CreateImgButton( LPCSTR caption, int nID, CBitmap **bmp )
{
	CfxImgButton *pBtn = new CfxImgButton(m_pFont);
	if (pBtn->Create(caption, CRect(0, 0, 0, 0), this, nID, FALSE, TRUE))
	{
		pBtn->SetImgBitmap(	bmp[0]->operator HBITMAP(), bmp[1]->operator HBITMAP(), bmp[2]->operator HBITMAP() );
		return pBtn;
	}
	else return NULL;
}

void CMapWnd::DelGroupAccount( accn_info *ai )
{
	CGridCtrl *pGrid = m_pGrid[1];

	int row, rows = pGrid->GetRowCount();
	for(row=1; row<rows; ++row)
	{
		CString acno;
		acno = pGrid->GetItemText(row, 0); 
		acno.TrimRight();
		acno.Replace("-", "");
		
		if (acno==ai->acno) break;
	}

	if (row>=rows) return;

	WritePLog("DelGroupAccount - group[%s][%s] accn[%s][%s]", m_pCurGroup->gpno, m_pCurGroup->gpnm, ai->acno2, ai->acnm);

	vector<accn_info>::iterator pos = find_if(m_owns.begin(), m_owns.end(), op_eq_acno(ai->acno));
	if (pos!=m_owns.end())
	{
		pos->used = FALSE;
		RefreshRemainAccount();
	}
// 	m_pass.erase(ai->acno);	//2014.02.19 KSJ 비밀번호를 가지고 있어야 하므로 지우지 않는다.

	CDC *pDC = pGrid->GetDC();
	for(; row<rows; ++row)
	{
		for(int col=0; col<pGrid->GetColumnCount(); ++col)
		{
			if ((row+1)>MAX_GROUPACCOUNT)
				pGrid->SetItemText(row, col, "");
			else 
				pGrid->SetItemText(row, col, pGrid->GetItemText(row+1, col));
			pGrid->RedrawCell(row, col, pDC);
		}
	}
	pGrid->ReleaseDC(pDC);
}

void CMapWnd::OnGroupUpdate( LPCSTR data, int dlen )
{
	struct _ledgerH *ledger = (struct _ledgerH*)data;
	sdldt003_mod *mod = (sdldt003_mod*)(data + L_ledgerH + sizeof(sdldt003_mid));

	CString emsg(ledger->emsg, sizeof(ledger->emsg));
	CString ecod(ledger->ecod, sizeof(ledger->ecod));
	emsg.TrimRight();
	MessageBox(emsg, "IBK투자증권");

	//2014.04.07 KSJ 에러날 경우는 계좌번호가 저장되면 안된다.
	if(ecod.Compare("1801") == 0 || ecod.Compare("1796") == 0)	//1801 비밀번호틀림, 1796 비밀번호 5회 오류
	{
		bool bEqual = false;	//메세지의 계좌번호와 동일할때
		CString strAccn, strErrAccn;
		int nPos;

		nPos = emsg.Find("계좌");
		strErrAccn = emsg.Mid(nPos-12, 11);

		for(int ii = 0; ii <m_updateAcnn.size(); ii++)
		{
			strAccn = m_updateAcnn[ii].acno;
			
			if(bEqual)
			{
				m_pass[strAccn] = "";
				continue;
			}

			if(strAccn.Compare(strErrAccn) == 0)
			{
				bEqual = true;
				m_pass[strAccn] = "";
			}
		}
	}

	CString strLedger(ledger->size, L_ledgerH);
//	OutputDebugString("[KSJ] msg[" + strLedger + "]");

	//2014.02.19 KSJ sdldt003_mod가 없으면 딱 _ledgerH 사이즈만큼 온다.
	if(dlen > L_ledgerH)
	{
		CString strSize(mod->orec, sizeof(mod->orec));
		int nSize = atoi(strSize);
		
		for(int ii = 0; ii < nSize; ii ++)
		{
			sdldt003_grid *grid = (sdldt003_grid*)(data + L_ledgerH + sizeof(sdldt003_mid) + (ii*sizeof(sdldt003_grid)));
			m_pass[grid->acno] = grid->acpw;
		}
	}
	LoadData();
}

void CMapWnd::LoadOwnAccount()
{
	LPCSTR accn = Variant(accountCC, "1");
	vector<StringProxy> lines, fields, fields2;
	
	m_owns.clear();

	ParseString(accn, accn+strlen(accn), '\n', lines);
	for(int row=0; row<lines.size(); ++row)
	{
		ParseString(lines[row], '\t', fields);

		if (fields.size()==3)
		{
			ParseString(fields[2], '|', fields2);				
			if (fields2.size()!=3) continue;
			
			// Wrap 계좌는 제외한다.
			int ownf = atoi(fields2[2].ToCStr());
			if (ownf & 0x02) continue;

			CString acno = fields[0].ToCStr(); acno.TrimRight();
			CString pass = fields[1].ToCStr(); pass.TrimRight();
			CString acnm = fields2[0].ToCStr();	acnm.TrimRight();
			CString type = acno.Mid(3, 2);
			
			if ( (type=="10") || (type=="11") )
			{
				accn_info ai;
				sprintf(ai.acno, (LPCSTR)acno);
				sprintf(ai.acno2, "%.3s-%.2s-%.6s", ((LPCSTR)acno)+0, ((LPCSTR)acno)+3, ((LPCSTR)acno)+5);
				sprintf(ai.acpw, (LPCSTR)pass);
				sprintf(ai.acnm, (LPCSTR)acnm);
				ai.mult = 1;
				ai.rate = 1.0;

				ai.used = FALSE;
				m_owns.push_back(ai);
				
				TRACE("accountCC - %s %s\n", (LPCSTR)acno, (LPCSTR)acnm);
			}
		}
	}
}

void CMapWnd::ClearUsedStatus()
{
	vector<accn_info>::iterator st, ed;
	st = m_owns.begin();
	ed = m_owns.end();
	for(; st!=ed; ++st)
	{
		BOOL bFind = FALSE;
		p_accn_data pst = m_adata.begin();
		p_accn_data ped = m_adata.end();
		for(; pst!=ped; ++pst)
		{
			vector<accn_info>::iterator pos = find_if(pst->second.begin(), pst->second.end(), op_eq_acno(st->acno));
			if (pos!=pst->second.end())
			{
				bFind = TRUE;
				break;
			}
		}
		st->used = (bFind) ? TRUE : FALSE;
	}
}

void CMapWnd::OnBtnNewGroup()
{
	if (m_bNewGroup)
	{
		MessageBox("이미 신규 그룹을 편집중입니다.", "IBK투자증권", MB_OK);
		return;
	}

	m_bNewGroup = TRUE;
	int gidx, row;
	CString gpno, gpnm, no;
	m_pCurGroup = &m_TempGroup;
	m_pCurAccount = &m_TempAccount;
	for(gidx=1; gidx<=MAX_GROUPLIST; ++gidx)
	{
		no.Format("%03d", gidx);
		gpno.Format("%s-%03d", (LPCSTR)m_usid, gidx);
		gpnm.Format("신규 %03d", gidx);
		
		p_grop_data pos = m_gdata.find(gpno);
		if (pos==m_gdata.end()) break;
	}
	if (gidx==MAX_GROUPLIST)
	{
		CString msg;
		msg.Format("그룹은 최대 %d개 까지 만들 수 있습니다.", MAX_GROUPLIST);
		MessageBox(msg, "IBK투자증권", MB_OK|MB_ICONINFORMATION);
		return;
	}

	sprintf(m_pCurGroup->gpno, (LPCSTR)gpno);
	sprintf(m_pCurGroup->gpnm, (LPCSTR)gpnm);

	WritePLog("OnBtnNewGroup - %s[%s]", (LPCSTR)gpno, (LPCSTR)gpnm);

	for(row=1; row<m_pGrid[0]->GetRowCount(); ++row)
	{
		CString tmp = m_pGrid[0]->GetItemText(row, 0);
		tmp.TrimRight();
		if (tmp.IsEmpty()) break;
	}

	m_pGrid[0]->SetItemText(row, 0, no);
	m_pGrid[0]->SetItemText(row, 1, gpnm);
	m_pGrid[0]->SetItemFgColour(row, 0, m_crPlus);
	m_pGrid[0]->SetItemFgColour(row, 1, m_crPlus);
	m_pGrid[0]->RedrawRow(row);

	m_edGroupName.SetWindowText(gpnm);
	SelectGridRow(m_pGrid[0], row);
	ClearGrid(m_pGrid[1]);
	ClearUsedStatus();
	RefreshRemainAccount();
}

void CMapWnd::OnBtnDelGroup()
{
	if (!m_pCurGroup) return;
	
	if (m_pCurGroup==&m_TempGroup)
	{
		LoadData();
		return;
	}

	CString gpno = m_pCurGroup->gpno;
	CString gpnm = m_pCurGroup->gpnm;
	gpnm.TrimLeft(); gpnm.TrimRight();
	
	CString msg;
	msg.Format("[%s] 그룹을 삭제하시겠습니까?", gpnm);
	if (MessageBox(msg, "IBK투자증권", MB_YESNO|MB_ICONWARNING)!=IDYES) return;

	vector<char> buff(1024*64, ' ');
	
	struct _ledgerH *ledger = (struct _ledgerH*)&buff[0];
	GetLedger(ledger);
	CopyMemory(ledger->svcd, "SDLDT003", 8);
	CopyMemory(ledger->brno, "000", 3);
	CopyMemory(ledger->rcnt, "0000", 4);
	CopyMemory(ledger->odrf, "1", 1);
	CopyMemory(ledger->mkty, "1", 1);
	
	sdldt003_mid *mid = (sdldt003_mid*)&buff[L_ledgerH];
	FillMemory(mid, sizeof(sdldt003_mid), ' ');
	CopyMemory(mid->irec, "00001", 5);
	CopyMemory(mid->gubn, "D", 1);
	CopyMemory(mid->type, "10", 2);
	CopyMemory(mid->gpno, (LPCSTR)gpno, gpno.GetLength());
	CopyMemory(mid->gpnm, (LPCSTR)gpnm, gpnm.GetLength());
	CopyMemory(mid->orec, "00000", 5);
	
	buff.resize( L_ledgerH + sizeof(sdldt003_mid));
	WriteLog("OnBtnDelGroup", buff.size(), &buff[0]);
	SendTR("piboPBxQ", US_ENC, &buff[0], buff.size(), TK_SDLDT003);
}

void CMapWnd::OnBtnReload()
{
	LoadData();	
}

void CMapWnd::OnBtnLimit()
{
	SetModal("IB190010 /S/s/p5/t");
}

void CMapWnd::WriteLog(LPCSTR name, int dlen, LPCSTR data)
{
#ifdef WRITELOG

	CString logpath = m_home + "\\IB650001.log";
	FILE *fp = fopen((LPCSTR)logpath, "a+");
	if (!fp) return;
	
	SYSTEMTIME st;
	GetCurrentTime(&st);
	CTime strTime;
	strTime.Format("[%04d-%02d-%02d %02d:%02d:%02d]", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	fprintf(fp, (LPCSTR)strTime);
	fprintf(fp, "%-20s : [", name);
	fwrite(data, 1, dlen, fp);
	fwrite("]\n", 1, 2, fp);

	fclose(fp);

#endif
}

void CMapWnd::WritePLog( LPCSTR fmt, ... )
{
#ifdef WRITELOG

	CString logpath = m_home + "\\IB650001.log";
	FILE *fp = fopen((LPCSTR)logpath, "a+");
	if (!fp) return;
	
	SYSTEMTIME st;
	GetCurrentTime(&st);
	CTime strTime;
	strTime.Format("[%04d-%02d-%02d %02d:%02d:%02d]", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	fprintf(fp, (LPCSTR)strTime);
	
	va_list argptr;
	va_start(argptr, fmt);
	vfprintf(fp, fmt, argptr);
	va_end(argptr);
	fprintf(fp, "\n");
	
	fclose(fp);	

#endif
}
