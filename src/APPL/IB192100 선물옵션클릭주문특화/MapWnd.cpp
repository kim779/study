#include "stdafx.h"
#include "IB192100.h"
#include "MapWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 26400)
#pragma warning(disable : 26401)
#pragma warning(disable : 26409)

CMapWnd::CMapWnd(CWnd *pParent)
: CAxisExt(pParent)
{
	ZeroMemory(m_hRndBmp, sizeof(m_hRndBmp));

	m_pAcno = NULL;
	m_pCode = NULL;
	m_pHogaWnd = NULL;	
	m_pJango = NULL;
	m_pJqty = NULL;
	m_pStAccn = NULL;
	m_pStCode = NULL;
	m_pStGuide = NULL;
	m_pStAcnm = NULL;
	m_pStCodeNm = NULL;
	m_pPswd = NULL;
	m_pBtnSearch = NULL;
	m_pStJango = NULL;
	m_pBtnSend = NULL;
	m_pBtnCheg = NULL;
	m_pBtnJngo = NULL;
	m_pBtnConf = NULL;
	m_pBtnAlign = NULL;
	m_pBtnCancel = NULL;
	
	m_home = Variant(homeCC, _T(""));
	m_user = Variant(userCC, _T(""));
	m_name = Variant(nameCC, _T(""));
	Variant(titleCC, _T("[1921] 선옵클릭주문(특)"));
	
	for(int n=TK_NOTIFY_CTRL_S; n<=TK_NOTIFY_CTRL_E; ++n)
		m_NotifyKeys.push_back(n);

	m_bShowConfirm = TRUE;
	m_bHogaReverse = TRUE;
	m_JqtyIndex = -1;
	m_profile.Format(CONFIG_INI, Variant(homeCC,""), Variant(nameCC,""));

	LoadConfig();
	LoadImage();
	Init();
}

CMapWnd::~CMapWnd()
{
}

BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
	ON_MESSAGE(WM_APP_SIG, OnAppSignal)
	ON_BN_CLICKED(IDC_SEARCH_BTN, OnCodeSearch)
	ON_BN_CLICKED(IDC_SEND_BTN, OnSendBtn)
	ON_BN_CLICKED(IDC_CHEG_BTN, OnChegBtn)
	ON_BN_CLICKED(IDC_JNGO_BTN, OnJngoBtn)
	ON_BN_CLICKED(IDC_CONF_BTN, OnConfBtn)
	ON_BN_CLICKED(IDC_ALIGN_BTN, OnAlignBtn)
	ON_BN_CLICKED(IDC_CANCEL_BTN, OnCancelBtn)
END_MESSAGE_MAP()

void CMapWnd::Init()
{
}

void CMapWnd::LoadConfig()
{
	int ival=0;
	CString key, sval;
	if (m_pJqty)
	{
		const int def_val[] = { 1, 3, 5, 10, 20, -1 };
		for(int row=0; def_val[row]>0; ++row)
		{
			key.Format("MA_%d", row);
			ival = GetPrivateProfileInt("JQTY_GRID", (LPCSTR)key, def_val[row], (LPCSTR)m_profile);
			sval.Format("%d", ival);

			if (row<m_pJqty->GetRowCount())
				m_pJqty->SetItemText(row, 0, (LPCSTR)sval );
		}
	}

	ival = GetPrivateProfileInt("ORDER", "PUTREVERSE", 0, (LPCSTR)m_profile);
	m_bHogaReverse = (ival) ? TRUE : FALSE;
	
	ival = GetPrivateProfileInt("ORDER", "CONFIRM", 0, (LPCSTR)m_profile);
	m_bShowConfirm = (ival) ? TRUE : FALSE;

	if (m_pHogaWnd)
	{
		ival = GetPrivateProfileInt("SHOW", "BEFORE", 0, (LPCSTR)m_profile);
		m_pHogaWnd->ShowBefore((ival) ? true : false);
		m_pHogaWnd->ShowRate(false);
		m_pHogaWnd->ShowRsvd(false);

		ival = GetPrivateProfileInt("ORDER", "ENTER", 0, (LPCSTR)m_profile);
		m_pHogaWnd->SetEnterOrder((ival) ? TRUE : FALSE);
		ival = GetPrivateProfileInt("ORDER", "SPACE", 1, (LPCSTR)m_profile);
		m_pHogaWnd->SetSpaceOrder((ival) ? TRUE : FALSE);
		ival = GetPrivateProfileInt("ORDER", "ONECLICK", 0, (LPCSTR)m_profile);
		m_pHogaWnd->SetOneClickMode((ival) ? TRUE : FALSE);
		
		int rsvd, jumun, cnt, vol, hoga, rate;
		m_pHogaWnd->GetWidths(&rsvd, &jumun, &cnt, &vol, &hoga, &rate);
		rsvd = GetPrivateProfileInt("WIDTH", "RSVD", rsvd, (LPCSTR)m_profile);
		jumun = GetPrivateProfileInt("WIDTH", "JUMUN", jumun, (LPCSTR)m_profile);
		cnt = GetPrivateProfileInt("WIDTH", "CNT", cnt, (LPCSTR)m_profile);
		vol = GetPrivateProfileInt("WIDTH", "VOL", vol, (LPCSTR)m_profile);
		hoga = GetPrivateProfileInt("WIDTH", "HOGA", hoga, (LPCSTR)m_profile);
		rate = GetPrivateProfileInt("WIDTH", "RATE", rate, (LPCSTR)m_profile);
		m_pHogaWnd->SetWidths(rsvd, jumun, cnt, vol, hoga, rate);
	}
}

void CMapWnd::SaveConfig()
{
	int row=0;
	CString key, val;

	if (m_pJqty)
	{
		for(row=0; row<m_pJqty->GetRowCount(); ++row)
		{
			key.Format("MA_%d", row);
			WritePrivateProfileString("JQTY_GRID", key, m_pJqty->GetItemText(row, 0), m_profile);
		}
	}

	WritePrivateProfileString("ORDER", "PUTREVERSE", (m_bHogaReverse) ? "1" : "0", m_profile);
	WritePrivateProfileString("ORDER", "CONFIRM", (m_bShowConfirm) ? "1" : "0", m_profile);

	if (m_pHogaWnd)
	{
		int rsvd=0, jumun=0, cnt=0, vol=0, hoga=0, rate=0;

		WritePrivateProfileString("SHOW", "BEFORE", m_pHogaWnd->IsShowBefore() ? "1" : "0", m_profile);
		WritePrivateProfileString("ORDER", "ENTER", m_pHogaWnd->GetEnterOrder() ? "1" : "0", m_profile);
		WritePrivateProfileString("ORDER", "SPACE", m_pHogaWnd->GetSpaceOrder() ? "1" : "0", m_profile);
		WritePrivateProfileString("ORDER", "ONECLICK", m_pHogaWnd->IsOneClickMode() ? "1" : "0", m_profile);
		
		m_pHogaWnd->GetWidths(&rsvd, &jumun, &cnt, &vol, &hoga, &rate);
		val.Format("%d", rsvd);  WritePrivateProfileString("WIDTH", "RSVD", val, m_profile);
		val.Format("%d", jumun); WritePrivateProfileString("WIDTH", "JUMUN", val, m_profile);
		val.Format("%d", cnt);   WritePrivateProfileString("WIDTH", "CNT", val, m_profile);
		val.Format("%d", vol);   WritePrivateProfileString("WIDTH", "VOL", val, m_profile);
		val.Format("%d", hoga);  WritePrivateProfileString("WIDTH", "HOGA", val, m_profile);
		val.Format("%d", rate);  WritePrivateProfileString("WIDTH", "RATE", val, m_profile);
	}
}

void CMapWnd::LoadImage()
{
	char buff[1024];
	CString path, img_path;
	
	path.Format(_T("%s\\tab\\PALETTE.INI"), (LPCSTR)m_home);
	const int readL = GetPrivateProfileString("General", "Palette", "Blue", buff, sizeof(buff), path);
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
	
	m_hSearch[0] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\검색.bmp")));
	m_hSearch[1] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\검색_dn.bmp")));
	m_hSearch[2] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\검색_en.bmp")));

	m_hGuide     = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\알림_ic.bmp")));
}

void CMapWnd::DrawRoundRectangle( CDC *pDC, CRect rc )
{
	const CSize size(2,2);
	pDC->FillSolidRect((LPRECT)rc, m_crBk);
	rc.DeflateRect(size);
	pDC->FillSolidRect((LPRECT)rc, m_crBodyBk);
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
	CalcSize();
}

void CMapWnd::MakeBackground()
{
	CRect tRc;
	GetClientRect(&tRc);

	const int width = tRc.Width(), height = tRc.Height();

	CBitmap* oldBmp{};
	CDC *pdc = GetDC(), mdc;
	
	m_hBackground.DeleteObject();
	m_hBackground.CreateCompatibleBitmap(pdc, width, height);
	
	mdc.CreateCompatibleDC(pdc);
	oldBmp = mdc.SelectObject(&m_hBackground);
	
	// Drawing 
	DrawRoundRectangle(&mdc, m_rcBody);		// Body window
	DrawRoundRectangle(&mdc, m_rcGuide);	// Guide window
	
	// guide
	CRect rc(m_rcGuide);
	rc.top += (rc.Height()-14)/2;
	rc.left = PADDING_VALUE;
	rc.bottom = rc.top + 14;
	rc.right = rc.left + 14;
	DrawBitmap(&mdc, &rc, m_hGuide->operator HBITMAP(), RGB(0xDD,0xDD,0xDD));
	
	mdc.SelectObject(oldBmp);
	mdc.DeleteDC();

	ReleaseDC(pdc);
}

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this);
	Draw(&dc);
	m_pStAcnm->Invalidate(FALSE);
	m_pPswd->Invalidate(FALSE);
	m_pStCodeNm->Invalidate(FALSE);
	m_pStGuide->Invalidate(FALSE);
	m_pStJango->Invalidate(FALSE);
}

void CMapWnd::Draw( CDC *pdc )
{
	CDC mdc;
	CRect rc;
	
	GetClientRect(rc);
	mdc.CreateCompatibleDC(pdc);	
	CBitmap *oldBmp = mdc.SelectObject(&m_hBackground);
	pdc->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &mdc, 0, 0, SRCCOPY);
	mdc.SelectObject(oldBmp);
	
	mdc.DeleteDC();	
}

void CMapWnd::MoveWindow( CWnd *pWnd, int pos_x, int pos_y, int width, int height )
{
	if (!pWnd) return;
	if (!pWnd->m_hWnd) return;
	pWnd->MoveWindow(pos_x, pos_y, width, height, TRUE);                                                                                                         
}

void CMapWnd::CalcSize()
{
	CRect tRc;
	GetClientRect(&tRc);
	
	const int total_width = tRc.Width(), total_height = tRc.Height();
	if ((total_width==0) || (total_height==0)) return;
	const int new_width = m_pHogaWnd->GetRequireWidth()+60;
	if (total_width!=new_width)
	{
		SetWindowPos(NULL, 0, 0, new_width, total_height, SWP_NOZORDER | SWP_NOMOVE);
		m_pParent->PostMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(new_width, total_height));
		return;
	}
	
	m_rcBody.SetRect(0, 0, total_width, total_height-GUIDE_WND_HEIGHT); 
	m_rcGuide.SetRect(0, total_height-GUIDE_WND_HEIGHT, total_width, total_height);
	
	const int width  = total_width - (PADDING_VALUE*2);
	const int height = total_height - (PADDING_VALUE*2);
	int pos_x = 0, pos_y = 0;
	const int line_h = 20, line_h2 = 55;
	
	// 1 line
	pos_x = PADDING_VALUE; pos_y = PADDING_VALUE;
	MoveWindow(m_pStAccn.get(), pos_x, pos_y,  40, line_h);	pos_x += 40;
	MoveWindow(m_pAcno.get(),   pos_x, pos_y, 140, line_h);	pos_x += ADDTC(140);
	MoveWindow(m_pPswd.get(),   pos_x, pos_y,  50, line_h);	pos_x += ADDTC(50);
	MoveWindow(m_pStAcnm.get(), pos_x, pos_y, width-pos_x+PADDING_VALUE, line_h);

	// 2 line
	pos_x = PADDING_VALUE; pos_y += ADDTC(line_h+1);
	MoveWindow(m_pStCode.get(), pos_x, pos_y,  40, line_h);				pos_x += 40;
	MoveWindow(m_pCode.get(),   pos_x, pos_y, CODECTRL_WIDTH, line_h);	pos_x += CODECTRL_WIDTH;
	MoveWindow(m_pBtnSearch.get(), pos_x, pos_y, 19, line_h);				pos_x += ADDTC(19);
	MoveWindow(m_pStCodeNm.get(), pos_x, pos_y, width-pos_x+PADDING_VALUE-ADDTC(40), line_h);
	MoveWindow(m_pBtnSend.get(), width+PADDING_VALUE-ADDTC(40)+ADDTC(0), pos_y, 40, line_h);

	// 3 line
	pos_x = PADDING_VALUE; pos_y += ADDTC(line_h+1);
	MoveWindow(m_pJango.get(), pos_x, pos_y, 175, GRID_CELL_HEIGHT*2); pos_x += ADDTC1(175);
	MoveWindow(m_pBtnCheg.get(), pos_x, pos_y+(line_h/2), 40, line_h); pos_x += ADDTC1(40);
	MoveWindow(m_pBtnJngo.get(), pos_x, pos_y+(line_h/2), 40, line_h); pos_x += ADDTC1(40);
	MoveWindow(m_pBtnConf.get(), pos_x, pos_y+(line_h/2), 40, line_h);

	// 4 line
	pos_x = PADDING_VALUE; pos_y += ADDTC(GRID_CELL_HEIGHT*2+1);
	//MoveWindow(m_pHogaWnd, pos_x, pos_y, HOGA_WND_WIDTH, m_rcBody.bottom-pos_y-2);
	MoveWindow(m_pHogaWnd.get(), pos_x, pos_y, m_pHogaWnd->GetRequireWidth(), m_rcBody.bottom-pos_y-2);
	pos_x += m_pHogaWnd->GetRequireWidth() + PADDING_VALUE;

	const int jqty_width = width-pos_x+PADDING_VALUE-1;
	int tpos_y = pos_y + m_rcBody.Height()/2 - (GRID_CELL_HEIGHT*5)-100;

	MoveWindow(m_pStJango.get(), pos_x, tpos_y-line_h-2, jqty_width, line_h);
	MoveWindow(m_pJqty.get(), pos_x, tpos_y, jqty_width, GRID_CELL_HEIGHT*5);
 	if (m_pJqty && jqty_width>0)
 		if (m_pJqty->GetColumnCount()>0)
 			m_pJqty->SetColumnWidth(0, jqty_width-1);

	tpos_y += ADDTC(GRID_CELL_HEIGHT*6);
	MoveWindow(m_pBtnCancel.get(), pos_x, tpos_y, jqty_width, line_h2);

	tpos_y += ADDTC(line_h2+5);
	MoveWindow(m_pBtnAlign.get(), pos_x, tpos_y, jqty_width, line_h2);

	MoveWindow(m_pStGuide.get(), m_rcGuide.left+25, m_rcGuide.top+3, width-50, line_h-3);

	MakeBackground();
}

int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	const CRect cRc(0, 0, 0, 0);

	// contructor
	m_pStAccn = std::make_unique<CfxStatic>(m_pFont);
	m_pAcno = std::make_unique<CAccountCtrl>(m_pParent, "AN20", TK_ACCOUNT_CTRL1);
	m_pPswd = std::make_unique<CfxEditEx>(m_pFont);
	m_pStAcnm = std::make_unique<CfxStaticEx>(m_pFont);
	m_pStCode = std::make_unique<CfxStatic>(m_pFont);
	m_pCode = std::make_unique<CCodeCtrl>(m_pParent, "", TK_CODE_CTRL);
	m_pBtnSearch = std::make_unique<CfxImgButton>(m_pFont);
	m_pStCodeNm = std::make_unique<CfxStaticEx>(m_pFont);
	m_pStJango = std::make_unique<CfxStaticEx>(m_pFont);
	m_pHogaWnd = std::make_unique<CFOHogaWnd>(m_pParent, &m_AcntData, &m_RsvdData);
	m_pStGuide = std::make_unique<CfxStatic>(m_pFont);
	m_pJango = std::make_unique<CGridCtrl>();
	m_pJqty = std::make_unique<CGridCtrl>();
	m_pBtnSend = std::make_unique<CfxImgButton>(m_pFont);
	m_pBtnCheg = std::make_unique<CfxImgButton>(m_pFont);
	m_pBtnJngo = std::make_unique<CfxImgButton>(m_pFont);
	m_pBtnConf = std::make_unique<CfxImgButton>(m_pFont);
	m_pBtnAlign = std::make_unique<CfxImgButton>(m_pFont);
	m_pBtnCancel = std::make_unique<CfxImgButton>(m_pFont);
	
	// initialize
	m_pStAccn->Create("계 좌", WS_CHILD|WS_VISIBLE, cRc, this);
	m_pStAccn->SetBkColor(m_crBodyBk);

	m_pAcno->Create(NULL, "AN20", WS_CHILD|WS_TABSTOP|WS_VISIBLE, cRc, this, IDC_ACNO_CTRL);

	m_pPswd->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_PASSWORD|ES_AUTOHSCROLL, cRc, this, IDC_PSWD_EDIT);
	m_pPswd->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, 0);
	m_pPswd->SetLimitText(10);

	m_pStAcnm->Create("", WS_CHILD|WS_VISIBLE|WS_BORDER|SS_LEFT, cRc, this, IDC_ACNM_STATIC);
	m_pStAcnm->Init(WS_CHILD|WS_VISIBLE|WS_BORDER, ssTEXT|ssBORDER, "", "", RGB(0xE8, 0xE0, 0xED), RGB(0xE8, 0xE0, 0xED), RGB(0xBF, 0xBF, 0xBF), alLEFT, 0);
	
	m_pStCode->Create("종 목", WS_CHILD|WS_VISIBLE, cRc, this);
	m_pStCode->SetBkColor(m_crBodyBk);

	m_pCode->Create(NULL, "CODE", WS_CHILD|WS_VISIBLE|WS_TABSTOP, CRect(0, 0, CODECTRL_WIDTH, 20), this, -1);

	m_pBtnSearch = CreateImgButton("", IDC_SEARCH_BTN, m_hSearch);
	m_pBtnSend = CreateImgButton("조회", IDC_SEND_BTN, m_hBtn2);
	m_pBtnCheg = CreateImgButton("체결", IDC_CHEG_BTN, m_hBtn2);
	m_pBtnJngo = CreateImgButton("잔고", IDC_JNGO_BTN, m_hBtn2);
	m_pBtnConf = CreateImgButton("설정", IDC_CONF_BTN, m_hBtn2);
	m_pBtnAlign = CreateImgButton("정렬", IDC_ALIGN_BTN, m_hBtn2);
	m_pBtnCancel = CreateImgButton("취소", IDC_CANCEL_BTN, m_hBtn2);

	m_pStCodeNm->Create("", WS_CHILD|WS_VISIBLE|WS_BORDER|SS_LEFT, cRc, this, -1);
	m_pStCodeNm->Init(WS_CHILD|WS_VISIBLE|WS_BORDER|ssTEXT, 0, "", "", RGB(0xEB, 0xEB, 0xEB), RGB(0xEB, 0xEB, 0xEB), RGB(0xBF, 0xBF, 0xBF), alLEFT, 0);

	m_pStJango->Create("", WS_CHILD|WS_VISIBLE|WS_BORDER|SS_CENTER, cRc, this, -1);
	m_pStJango->Init(WS_CHILD|WS_VISIBLE|WS_BORDER, ssTEXT|ssBORDER, "", "", m_crBodyBk, m_crBodyBk, ::GetSysColor(COLOR_BTNTEXT), alCENTER, 0);

	m_pHogaWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, cRc, this, IDC_HOGA_WND);
	m_pHogaWnd->ShowRate(false);
	m_pHogaWnd->ShowBefore(false);
	m_pHogaWnd->ShowRsvd(false);
	m_pHogaWnd->SetRClickCancel(TRUE);
	m_pHogaWnd->SetExtOrderColumn(TRUE);
	
	m_pStGuide->Create("", WS_CHILD|WS_VISIBLE, cRc, this);
	m_pStGuide->SetAlignment(alLEFT, false);
	m_pStGuide->SetBkColor(m_crBodyBk);

	m_pJango->Create(cRc, this, IDC_JANGO_GRID);
	SetJangoGrid();
	
	m_pJqty->Create(cRc, this, IDC_JQTY_GRID);
	SetJqtyGrid();

	LoadConfig();

	Send();

	return 0;
}

HBRUSH CMapWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);	
	const UINT nID = pWnd->GetDlgCtrlID();
	return hbr;
}

std::shared_ptr <CfxImgButton> CMapWnd::CreateImgButton( LPCSTR caption, int nID, CBitmap **bmp )
{
	std::shared_ptr <CfxImgButton> pBtn = std::make_shared<CfxImgButton>(m_pFont);
	if (pBtn->Create(caption, CRect(0, 0, 0, 0), this, nID, FALSE, TRUE))
	{
		pBtn->SetImgBitmap(	bmp[0]->operator HBITMAP(), bmp[1]->operator HBITMAP(), bmp[2]->operator HBITMAP() );
		return pBtn;
	}
	else return NULL;
}

LRESULT CMapWnd::OnUser( WPARAM wParam, LPARAM lParam )
{
	const int msg = LOBYTE(LOWORD(wParam));
	const int key = HIBYTE(LOWORD(wParam));
	const int len = HIWORD(wParam);
	LPCSTR dat = (LPCSTR)lParam;

	switch(msg)
	{
	case DLL_INB:
		break;
	case DLL_OUB:
		switch(key)
		{
		case TK_PIBO3002:
			res_pibo3002(dat, len);
			break;
		case TK_PIBOFCHG:
			res_pibofchg(dat, len);
			break;
		case TK_CODE_CTRL:
			if (m_pCode) m_pCode->m_pCtrl->SendMessage(WM_USER, wParam, lParam);
			break;
		case TK_ACCOUNT_CTRL1:
		case TK_ACCOUNT_CTRL2:
			if (m_pAcno) m_pAcno->m_pCtrl->SendMessage(WM_USER, wParam, lParam); 
			break;
		case TK_SACMT238:
			res_SACMT238(dat, len);
			break;
		case TK_SONBQ101:
			break;
		case TK_PIBOFODR:
			res_pibofodr(dat, len);
			break;
		}
		break;
	case DLL_ALERT:
//		proc_alert(dat, strlen(dat));
		break;
	case DLL_ALERTx:
		proc_alertx((struct _alertR*)lParam);
		break;
	case DLL_TRIGGER:
		proc_trigger(dat, strlen(dat));
		break;
	case DLL_NOTICE:
		proc_notice(dat, strlen(dat));
		break;
	case DLL_SETPAL:
	case DLL_SETFONT:
	case DLL_SETFONTx:
		ChangeTheme();
		break;
	case DLL_GUIDE:
		proc_guide(dat);
		return true;
	case DLL_DOMINO:
		proc_domino(dat, strlen(dat));
		break;
	}
	return 0;
}

void CMapWnd::proc_trigger( LPCSTR data, int dlen )
{
	vector<StringProxy> fields;
	ParseString(data, data+dlen, '\t', fields);

	if (fields.size()==2)
	{
		CString sym = fields[0].ToCStr();
		CString val = fields[1].ToCStr();
		if (sym=="ed_focod"||sym=="30301"||sym=="40301")
			ChangeCode(val, FALSE);
		else if (sym=="AN20")
			m_pAcno->Trigger(val);
	}
}

LRESULT CMapWnd::OnAppSignal( WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case WP_ACCN_CHANGE:
		ChangeAccount((LPCSTR)lParam);
		break;
	case WP_CODE_CHANGE:
		ChangeCode((LPCSTR)lParam);
		break;
	case WP_JNGO_CHANGE:
		proc_jango((Jango*)lParam);
		break;
	case WP_MADO_ALL_CANCEL:
		if (CheckAccount()) cancel_order_all(1);
		break;
	case WP_MASU_ALL_CANCEL:
		if (CheckAccount()) cancel_order_all(2);
		break;
	case WP_MADO_RSVD_ALL_CANCEL:
		if (CheckAccount()) cancel_rsvd_all(1);
		break;
	case WP_MASU_RSVD_ALL_CANCEL:
		if (CheckAccount()) cancel_rsvd_all(2);
		break;
	case WP_ORDER:
		if (CheckAccount()) click_order((Order*)lParam);
		break;
	case WP_RSVD_ORDER:
		if (CheckAccount()) rsvd_order((ReserveData*)lParam);
		break;
	case WP_MODIFY_ORDER:
		if (CheckAccount()) modify_order((ModifyData*)lParam);
		break;
	case WP_CANCEL_ORDER:
		if (CheckAccount()) cancel_order((CancelData*)lParam);
		break;
	case WP_MODIFY_RSVD:
		if (CheckAccount()) modify_rsvd((ModifyData*)lParam);
		break;
	case WP_CANCEL_RSVD:
		if (CheckAccount()) cancel_rsvd((CancelData*)lParam);
		break;
	case WP_HOGA_CLICK:
		break;
	case WP_ONECLICK_MODE:
		m_pHogaWnd->SetOneClickMode((BOOL)lParam);
		break;
		/*
	case WP_CTRL2_FOLD:
		FoldControl2((BOOL)lParam);
		break;
	case WP_SET_JCNT:
		m_pControl->SetJcnt( (int)lParam );
		break;
		*/
	}
	return 0;
}

void CMapWnd::ChangeAccount( LPCSTR data )
{
	vector<StringProxy> fields;
	ParseString(data, data+strlen(data), '\t', fields);
	CString acno, acnm, pswd;
	acno = fields[0].ToCStr();
	acnm = fields[1].ToCStr();
	if (fields.size()>2)
		pswd = fields[2].ToCStr();

	if (m_AcntData.size()==1)
	{
		if (m_AcntData.begin()->first == (LPCSTR)acno)
			return;
	}

	ClearAccount();	
	m_pAcno->SetWindowText(acno);
	m_pStAcnm->SetText(acnm);

	if (pswd.IsEmpty()) 
	{
		m_pPswd->SetText("");
		m_pPswd->EnableWindow(TRUE);
		m_pPswd->SetFocus();
	}
	else
	{
		m_pPswd->SetText(pswd);
		m_pPswd->EnableWindow(FALSE);
		req_SACMT238(acno, pswd);
	}
}

void CMapWnd::ClearAccount()
{
	m_AcntData.clear();
	
	NotifyMap::iterator st = m_NotifyMap.begin();
	const NotifyMap::iterator ed = m_NotifyMap.end();
	for(; st!=ed; ++st)
	{
		CNotifyCtrl *pNotify = st->second;
		pNotify->DestroyWindow();
		m_NotifyKeys.push_back(pNotify->GetKey());
		delete pNotify;
	}
	m_NotifyMap.clear();

	if (m_pJango) 
	{
		for(int col=0; col<m_pJango->GetColumnCount(); ++col)
			m_pJango->SetItemText(1, col, "");
		m_pJango->RedrawRow(1);
	}
	if (m_pStJango)
	{
		m_pStJango->SetText("");
	}
}

void CMapWnd::ChangeCode( LPCSTR code, BOOL bTrigger /*= TRUE*/ )
{
	vector<StringProxy> fields;
	ParseString(code, code+strlen(code), '\t', fields);
	
	if (fields.empty()) return;
	
	char ch{};
	CString _code, _name, param;
	if (fields.size()==1) 
	{
		_code = fields[0].ToCStr();
		m_pCode->SetCode(_code);
		_name = m_pCode->GetName();
	}
	else if (fields.size()==2)
	{
		_code = fields[0].ToCStr();
		_name = fields[1].ToCStr();
	}
	else return;
	
	_code.TrimRight();
	_name.TrimRight();
	
	if (_code.GetLength()!=FCODE_LEN) return;
	/*
	if (_code.GetAt(0)=='4')
	{
		MessageBox("스프레드종목은 지원하지 않습니다.", "IBK투자증권", MB_OK|MB_ICONINFORMATION);
		return;
	}
	*/
	
	if (bTrigger)
	{
		ch = _code.GetAt(0);
		param.Format("ed_focod\t%s", _code); Variant(codeCC, param); Variant(triggerCC, param);
		if      (ch=='1' || ch=='4') { param.Format("30301\t%s", _code); Variant(codeCC, param); Variant(triggerCC, param); }
		else if (ch=='2' || ch=='3') { param.Format("40301\t%s", _code); Variant(codeCC, param); Variant(triggerCC, param); }
	}
	
	if (m_SiseData.code!=_code)
	{
		req_pibo3002(_code);
		m_pStCodeNm->SetText(_name);

		if (m_RsvdCode!=_code)
		{
			cancel_rsvd_all(1);
			cancel_rsvd_all(2);
		}
	}
}

void CMapWnd::req_pibo3002( LPCSTR code )
{
	if (strlen(code)!=FCODE_LEN) return;
	
	m_SiseData.code = code;
	m_SiseData.type = GetCodeType(code);
	SendTR("PIBO3002", 0, m_SiseData.code, 8, TK_PIBO3002);
}

void CMapWnd::res_pibo3002( LPCSTR data, int size )
{
	if (size!=sizeof(pibo3002_mod)) return;
	
	pibo3002_mod *mod = (pibo3002_mod *)data;
	
	m_SiseData.fhoga.clear();
	m_SiseData.hhga = (int)(fabs(Str2Double(mod->hhga, sizeof(mod->hhga))*100) + DOUBLE_PREFIX);
	if (m_SiseData.code.GetAt(0)=='4')
	{
		// 하한가는 반대부호로 오게 되어 있다(색상) 고로 스프레드의 경우 다시 역방향으로 처리해주는 센스가 필요하다.
		double hhga = Str2Double(mod->hhga, sizeof(mod->hhga))*100 + DOUBLE_PREFIX;
		hhga *= -1.0;
		m_SiseData.hhga = (int)hhga;
	}

	m_SiseData.shga = (int)(fabs(Str2Double(mod->shga, sizeof(mod->shga))*100) + DOUBLE_PREFIX);
	m_SiseData.gjga = (int)(fabs(Str2Double(mod->gjga, sizeof(mod->gjga))*100) + DOUBLE_PREFIX);
	m_SiseData.siga = (int)(fabs(Str2Double(mod->siga, sizeof(mod->siga))*100) + DOUBLE_PREFIX);
	m_SiseData.koga = (int)(fabs(Str2Double(mod->koga, sizeof(mod->koga))*100) + DOUBLE_PREFIX);
	m_SiseData.jega = (int)(fabs(Str2Double(mod->jega, sizeof(mod->jega))*100) + DOUBLE_PREFIX);
	m_SiseData.curr = (int)(fabs(Str2Double(mod->curr, sizeof(mod->curr))*100) + DOUBLE_PREFIX);
	m_SiseData.rate = (int)(Str2Double(mod->rate, sizeof(mod->rate))*100);
	m_SiseData.gvol = Str2Int(mod->gvol, sizeof(mod->gvol));
	m_SiseData.mgjv = Str2Int(mod->mgjv, sizeof(mod->mgjv));
	m_SiseData.t_dcnt = Str2Int(mod->dcnt, sizeof(mod->dcnt));
	m_SiseData.t_scnt = Str2Int(mod->scnt, sizeof(mod->scnt));
	m_SiseData.t_dvol = Str2Int(mod->dvol, sizeof(mod->dvol));
	m_SiseData.t_svol = Str2Int(mod->svol, sizeof(mod->svol));
	m_SiseData.t_sdch = Str2Int(mod->sdch, sizeof(mod->sdch));
	CString diff(mod->diff, sizeof(mod->diff));
	diff.TrimLeft();
	m_SiseData.udgb = diff.GetAt(0);
	m_SiseData.diff = (int)(fabs(atof((LPCSTR)diff.Mid(1))*100) + DOUBLE_PREFIX);
	
	for(int n=0; n<MAX_FHOGA; ++n)
	{
		m_SiseData.hoga[n].msga = (int)(Str2Double(mod->hoga[n].msga, sizeof(mod->hoga[n].msga))*100 + DOUBLE_PREFIX);
		m_SiseData.hoga[n].mdga = (int)(Str2Double(mod->hoga[n].mdga, sizeof(mod->hoga[n].mdga))*100 + DOUBLE_PREFIX);
		m_SiseData.hoga[n].dvol = Str2Int(mod->hoga[n].dvol, sizeof(mod->hoga[n].dvol));
		m_SiseData.hoga[n].svol = Str2Int(mod->hoga[n].svol, sizeof(mod->hoga[n].svol));
		m_SiseData.hoga[n].dcnt = Str2Int(mod->hoga[n].dcnt, sizeof(mod->hoga[n].dcnt));
		m_SiseData.hoga[n].scnt = Str2Int(mod->hoga[n].scnt, sizeof(mod->hoga[n].scnt));
	}
	
	int price;
	m_SiseData.fhoga.push_back(m_SiseData.shga);
	do {
		price = GetNextPrice(m_SiseData.fhoga.back(), m_SiseData.type, false);
		m_SiseData.fhoga.push_back( price );	
	} while( price > m_SiseData.hhga );
	
	if (m_bHogaReverse && m_SiseData.code.GetAt(0)=='3')
	{
		m_pHogaWnd->SetReverseMode(TRUE);
		std::reverse(m_SiseData.fhoga.begin(), m_SiseData.fhoga.end());
	}
	else
	{
		m_pHogaWnd->SetReverseMode(FALSE);
	}

	// 2011.01.14 코드가 변경되지 않으면 예약주문데이터를 초기화 하지 않으므로, 그리드 갱신이 필요함.
	if (m_RsvdCode==m_SiseData.code)
		RefreshRsvd();
	m_RsvdCode = m_SiseData.code;

	m_pHogaWnd->SetData(&m_SiseData);
	RefreshJango();
	calc_micheg(NULL);

	m_pHogaWnd->SetFocusGrid();
}

CODE_TYPE CMapWnd::GetCodeType( LPCSTR code )
{
	switch(*code)
	{
	case '1': return CT_FUTURE;
	case '2':
	case '3': return CT_OPTION;
	case '4': return CT_SPREAD;
	default : return CT_NONE;
	}
}

void CMapWnd::Send()
{
	CString code = m_pCode->GetCode();

// 	if (code.GetLength()==8)
// 	{
// 		CString name = m_pCode->GetName();
// 		ChangeCode(code+"\t"+name, FALSE);
// 	}

	if (!m_pPswd->IsWindowEnabled()) 
		m_pPswd->EnableWindow(TRUE);
	
	m_SiseData.code = "";

	ChangeCode(m_pCode->GetCode(), FALSE);
	
	CString acno;
	acno = m_pAcno->GetNumber();
	CString pswd = m_pPswd->GetText();
	m_pPswd->EnableWindow(FALSE);
	
	acno.TrimRight();
	pswd.TrimRight();
	
	if (acno.IsEmpty() || acno.GetLength()==0)
	{
		SetGuide("계좌번호를 입력하시기 바랍니다.");
		m_pPswd->EnableWindow(TRUE);
	}
	else if (pswd.IsEmpty())
	{
		SetGuide("계좌비밀번호를 입력하시기 바랍니다.");
		m_pPswd->EnableWindow(TRUE);
	}
	else
	{
		ClearAccount();
		req_SACMT238(acno, pswd);
	}
}

void CMapWnd::req_SACMT238( LPCSTR acno, LPCSTR pswd )
{
	vector<char> buff(L_ledgerH+sizeof(SACMT238_mid), ' ');
	struct _ledgerH *ledger = (struct _ledgerH *)&buff[0];
	SACMT238_mid *mid = (SACMT238_mid*)&buff[L_ledgerH];

	GetLedger(ledger);
	CopyMemory(ledger->svcd, "SACMT238", 8);
	CopyMemory(ledger->brno, "000", 3);
	CopyMemory(ledger->odrf, "1", 1);
	CopyMemory(ledger->mkty, "1", 1);
	
	CopyMemory(mid->In, "00001", 5);
	CopyMemory(mid->zAcntNo, acno, strlen(acno));
	// 2011.07.06 로그인계좌소유주 점검 결과 개별계좌 송신으로 변경
	//mid->zAcntNo[3] = mid->zAcntNo[4] = '0';
	CopyMemory(mid->zPwd, pswd, strlen(pswd));

	SendTR("piboPBxQ", US_ENC, &buff[0], buff.size(), TK_SACMT238);
}

void CMapWnd::res_SACMT238( LPCSTR data, int size )
{
	struct _ledgerH *ledger = (struct _ledgerH *)data;
	SACMT238_mod *mod = (SACMT238_mod*)(data+L_ledgerH);

	CString ecod = CString(ledger->ecod, sizeof(ledger->ecod));
	CString emsg = CString(ledger->emsg, sizeof(ledger->emsg));
	emsg.TrimRight();
	SetGuide(emsg);

	if (ecod.GetAt(0) == '0')
	{
		CString acno, pswd, acnm;
		m_pAcno->GetWindowText(acno);
		m_pPswd->GetWindowText(pswd);
		acnm = m_pStAcnm->GetText();
		AddAccount(acno, pswd, acnm, 1);
	}
	else
	{
		MessageBox(emsg, "IBK투자증권", MB_OK|MB_ICONERROR);
		m_pPswd->SetText("");
		m_pPswd->EnableWindow(TRUE);
		m_pPswd->SetFocus();
	}
}

void CMapWnd::AddAccount( LPCSTR acno, LPCSTR pswd, LPCSTR acnm /*= ""*/, int mult /*= 1*/ )
{
	const AccountMap::iterator pos = m_AcntData.find(acno);
	if (pos==m_AcntData.end()) 
	{
		Account account = Account();
		account.IsValid = TRUE;
		account.acnm = acnm;
		account.mult = mult;
		account.pswd = pswd;
		m_AcntData[acno] = account;
		
		req_pibofchg(acno, pswd, FALSE, NULL);
	}
	
	const NotifyMap::iterator pos2 = m_NotifyMap.find(acno);
	if (pos2==m_NotifyMap.end())
	{
		CNotifyCtrl *pNotify = new CNotifyCtrl(m_pParent, "", m_NotifyKeys.back());
		pNotify->Create(NULL, "NOTIFY", WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, -1);
		m_NotifyKeys.pop_back();
		m_NotifyMap[acno] = pNotify;
		pNotify->Send(acno, pswd, "");
	}
	RefreshRsvd();
}

void CMapWnd::DelAccount(LPCSTR acno)
{
	const AccountMap::iterator pos = m_AcntData.find(acno);
	if (pos!=m_AcntData.end()) 
		m_AcntData.erase(pos);
	
	const NotifyMap::iterator pos2 = m_NotifyMap.find(acno);
	if (pos2!=m_NotifyMap.end())
	{
		CNotifyCtrl *pNotify = pos2->second;
		pNotify->DestroyWindow();
		m_NotifyKeys.push_back(pNotify->GetKey());
		delete pNotify;
		m_NotifyMap.erase(pos2);
	}
}

void CMapWnd::req_pibofchg(LPCSTR acno, LPCSTR pswd, BOOL bNext, LPCSTR nkey)
{
	if (acno==NULL || pswd==NULL) return;
	vector<char> data( L_ledgerH + sizeof(pibofchg_mid), ' ');
	
	struct _ledgerH *ledger = (struct _ledgerH *)&data[0];
	pibofchg_mid    *mid    = (pibofchg_mid*)&data[L_ledgerH];
	
	GetLedger(ledger);
	
	if (bNext)
	{
		ledger->fkey[0] = '7';
		CopyMemory(ledger->nkey, nkey, sizeof(ledger->nkey));
	}
	else
	{
		ledger->fkey[0] = 'C';
		FillMemory(ledger->nkey, sizeof(ledger->nkey), ' ');
	}
	
	CopyMemory(mid->accn, acno, 11);
	CopyMemory(mid->pswd, pswd, strlen(pswd));
	mid->dlgb[0] = '2';
	mid->sort[0] = '1';
	mid->cpgb[0] = '0';
	mid->dsgb[0] = '0';
	
	SendTR("PIBOFCHG", US_ENC, &data[0], data.size(), TK_PIBOFCHG);
}

void CMapWnd::res_pibofchg( LPCSTR data, int size )
{
	struct _ledgerH *ledger = (struct _ledgerH *)data;
	pibofchg_mod *mod = (pibofchg_mod*)(data+L_ledgerH);
	const int nrec = Str2Int(mod->nrec, sizeof(mod->nrec));
	
	string acno = string(mod->accn, sizeof(mod->accn));
	const AccountMap::iterator pid = m_AcntData.find( acno );
	if (pid==m_AcntData.end())	// 모르는 계좌면 나가야지~
		return;
	
	string code;
	Account *pAcnt = &(pid->second);
	for(int n=0; n<nrec; ++n)
	{
		pibofchg_grid *grid = &mod->grid[n];
		
		code = string(grid->cod2, sizeof(grid->cod2));
		
		const int juno = Str2Int(grid->juno, sizeof(grid->juno) );
		const int ojno = Str2Int(grid->ojno, sizeof(grid->ojno) );
		const int oprc = (int)(fabs(Str2Double(grid->oprc, sizeof(grid->oprc))*100.0) + DOUBLE_PREFIX);
		const int oqty = Str2Int(grid->oqty, sizeof(grid->oqty) );
		const int dprc = (int)(fabs(Str2Double(grid->dprc, sizeof(grid->dprc))*100.0) + DOUBLE_PREFIX);
		const int dqty = Str2Int(grid->dqty, sizeof(grid->dqty) );
		const int wqty = Str2Int(grid->wqty, sizeof(grid->wqty) );
		CString odgb(grid->odgb, sizeof(grid->odgb)); odgb.TrimRight();
		CString stat = CString(grid->stat, sizeof(grid->stat));
		
		Order *pOdr = &(pAcnt->order[code][juno]);
		sprintf(pOdr->acno, acno.c_str());
		sprintf(pOdr->code, code.c_str());
		pOdr->juno = juno;
		pOdr->ojno = ojno;
		pOdr->oprc = oprc;
		pOdr->oqty = oqty;
		pOdr->dprc = dprc;
		pOdr->dqty = dqty;
		pOdr->wqty = wqty;
		if      (odgb=="+매수") pOdr->odgb = 2;
		else if (odgb=="-매도") pOdr->odgb = 1;
		else                    pOdr->odgb = 0;
		
		TRACE("\nMICHEG[%02d] accn(%s) code(%s) juno(%d) ojno(%d) oprc(%d) oqty(%d) dprc(%d) dqty(%d) wqty(%d) stat(%s)",
			n+1, acno.c_str(), code.c_str(), juno, ojno, oprc, oqty, dprc, dqty, wqty, (LPCSTR)stat);
		
	}
	
	if (ledger->next[0]=='Y') 
	{
		req_pibofchg(acno.c_str(), GetPassword(acno.c_str()), TRUE, ledger->nkey);
	} else 
	{
		calc_micheg(acno.c_str());
	}
}

LPCSTR CMapWnd::GetPassword( LPCSTR acno )
{
	const AccountMap::iterator pos = m_AcntData.find(acno);
	if (pos==m_AcntData.end()) return "";
	else return (LPCSTR)pos->second.pswd;
}

void CMapWnd::SetGuide( LPCSTR msg )
{
	m_pStGuide->SetText(msg);
}

void CMapWnd::proc_alertx(struct _alertR* alertR)
{
	TRY
	{
		if (alertR->code != m_SiseData.code) return;

		DWORD* data=nullptr;


		for (int i = alertR->size - 1; i > -1; --i)
		{
			data = (DWORD*)alertR->ptr[i];
			RealData rdat;
			for (int j=0; j < 255; j++) {
				if (data[j]) {
					CString sym;
					sym.Format("%03d", j);
					rdat[j] = StringProxy(sym, (char*)data[j]);
				}
			}


			if (data[23])
			{
				m_SiseData.pcurr = m_SiseData.curr;

				if (m_SiseData.code.GetAt(0) != '4')
				{
					m_SiseData.curr = (int)(fabs(atof((char*)data[23]) * 100.0) + DOUBLE_PREFIX);
				}
				else
				{
					if (atof((char*)data[23]) * 100.0 >= 0)
						m_SiseData.curr = (int)(fabs(atof((char*)data[23]) * 100.0) + DOUBLE_PREFIX);
					else
						m_SiseData.curr = (int)(atof((char*)data[23]) * 100.0 - DOUBLE_PREFIX);
				}

				if (m_SiseData.pcurr != m_SiseData.curr)
					CheckRsvd();
				// 			s.Format("SISE CURR : [%d]\n",m_SiseData.curr);
				// 			OutputDebugString(s);
			}

			m_pHogaWnd->SetRealData(data);
		}
	}
		
	CATCH(CMemoryException, e)
	{
		char emsg[1024];
		e->GetErrorMessage(emsg, 1024);
//		testSaveFile3("m_lpszClassName", e->GetRuntimeClass()->m_lpszClassName);
//		testSaveFile3("ErrorMessage", emsg);

		e->ReportError();
	}
	END_CATCH
}

/*
void CMapWnd::proc_alertx(struct _alertR* alertR)
{
	DWORD* data{};
	CString sCode = alertR->code;// , sSym, sData, sCurr, sDiff = "", sRate = "", sAccn;
	if (sCode != m_SiseData.code)
		return;

	// rts proc
	for (int n = 0; n < alertR->size; ++n)
	{
		data = (DWORD*)alertR->ptr[n];

		RealData rdat;
//		st = lines[n].first;
//		ed = lines[n].second;

		//TRACE("\n----------------------------\n");
		//TRACE("%s\n", data);
		while (st < ed)
		{
			pos = find(st, ed, '\t');
			sym = Str2Int(st, pos - st);
			st = pos + 1;
			if (st > ed) break;
			pos = find(st, ed, '\t');
			rdat[sym] = StringProxy(st, pos);
			st = pos + 1;
			//TRACE("[%03d/%s]", sym, (LPCSTR)val);
		}

		// 예약주문을 위해 현재가는 Sync한다
		const RealData::iterator pCurr = rdat.find(23);
		if (pCurr != rdat.end())
		{
			m_SiseData.pcurr = m_SiseData.curr;
			m_SiseData.curr = (int)(fabs(Str2Double(pCurr->second) * 100.0) + DOUBLE_PREFIX);
			if (m_SiseData.pcurr != m_SiseData.curr)
				CheckRsvd();
		}
		m_pHogaWnd->SetRealData(&rdat);
	}




	if (!data) return;
}
*/
void CMapWnd::proc_alert( LPCSTR data, int dlen )
{
	int sym=0, pack_cnt=0, n=0;
	CString key;
	LPCSTR st = data;
	LPCSTR ed = st + dlen;
	LPCSTR pos;
	vector<StringProxy> lines;
	
	// Key check
	pos = find(st, ed, '\t');
	key = CString(st, pos-st);

//	OutputDebugString(data);

	if (key!=m_SiseData.code) return;
	st = ++pos;
	
	// rts packet count
	while(st<ed)
	{
		pos = find(st, ed, '\n');
		lines.push_back( StringProxy(st, pos) );
		st = pos + 1;
	}
	
	// rts proc
	pack_cnt = (int)lines.size();
	for(n=0; n<pack_cnt; ++n)
	{
		RealData rdat;
		st = lines[n].first;
		ed = lines[n].second;
		
		//TRACE("\n----------------------------\n");
		//TRACE("%s\n", data);
		while(st<ed)
		{
			pos = find(st, ed, '\t');
			sym = Str2Int(st, pos-st);
			st = pos + 1;
			if (st>ed) break;
			pos = find(st, ed, '\t');
			rdat[sym] = StringProxy(st, pos);
			st = pos + 1;
			//TRACE("[%03d/%s]", sym, (LPCSTR)val);
		}
		
		// 예약주문을 위해 현재가는 Sync한다
		const RealData::iterator pCurr = rdat.find(23);
		if (pCurr!=rdat.end())
		{
			m_SiseData.pcurr = m_SiseData.curr;
			m_SiseData.curr = (int)(fabs(Str2Double( pCurr->second ) * 100.0) + DOUBLE_PREFIX);
 			if (m_SiseData.pcurr!=m_SiseData.curr)
 				CheckRsvd();
		}
		m_pHogaWnd->SetRealData(&rdat);
	}
}

BOOL CMapWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CMapWnd::proc_guide( LPCSTR msg )
{
	if (strcmp(msg, "그룹계좌가 존재하지 않습니다.")==0) return;
	SetGuide(msg);
}

void CMapWnd::calc_micheg( LPCSTR acno )
{
	if (acno)
	{
		const AccountMap::iterator pos = m_AcntData.find(acno);
		if (pos!=m_AcntData.end())
			_calc_micheg(&(pos->second), FALSE);
	}
	else
	{
		AccountMap::iterator st, ed;
		st = m_AcntData.begin();
		ed = m_AcntData.end();
		for(; st!=ed; ++st)
			_calc_micheg(&(st->second), FALSE);
	}
	
	RefreshMicheg();
}

void CMapWnd::_calc_micheg( Account *pAcnt, BOOL bNotify)
{
	set<string> codes;
	CodeOrderMap::iterator st, ed;
	st = pAcnt->order.begin();
	ed = pAcnt->order.end();
	for(; st!=ed; ++st)
		codes.insert(st->first);
	
	set<string>::iterator sst, sed;
	sst = codes.begin();
	sed = codes.end();
	for(; sst!=sed; ++sst)
		_calc_micheg(pAcnt, *sst, bNotify);
	
	RefreshMicheg();
}

void CMapWnd::_calc_micheg( Account *pAcnt, string code, BOOL bNotify)
{
	pAcnt->micheg[code].clear();
	OrderMap *om = &(pAcnt->order[code]);
	OrderMap::iterator st = om->begin();
	const OrderMap::iterator ed = om->end();
	for(; st!=ed; ++st)
	{
		Micheg &m = pAcnt->micheg[code][st->second.oprc];
		switch(st->second.odgb)
		{
		case 1: 
			m.d_cnt  += 1;
			m.d_wqty += st->second.wqty; 
			break;
		case 2: 
			m.s_cnt  += 1;
			m.s_wqty += st->second.wqty; 
			break;
		}
	}
	
	// 디스플레이를 위한 계좌/종목/가격정보를 입력한다.
	CodeMichegMap::iterator mst, med;
	mst = pAcnt->micheg.begin();
	med = pAcnt->micheg.end();
	for(; mst!=med; ++mst)
	{
		PriceMichegMap::iterator pst, ped;
		pst = mst->second.begin();
		ped = mst->second.end();
		for(; pst!=ped; ++pst)
		{
			sprintf(pst->second.acno, pAcnt->acno);
			sprintf(pst->second.code, mst->first.c_str());
			pst->second.oprc = pst->first;
			
			TRACE(
				"\n미체결정보 - 종목(%s) 가격(%d) 매도(%d/%d) 매수(%d/%d)\n",
				code.c_str(), pst->second.oprc, pst->second.d_cnt, pst->second.d_wqty, 
				pst->second.s_cnt, pst->second.s_wqty );
		}
	}
	if (bNotify) RefreshMicheg();
}

void CMapWnd::RefreshMicheg()
{
	m_pHogaWnd->SendMessage(WM_APP_REFRESH, WP_MICHEG, (LPARAM)(LPCSTR)m_SiseData.code);

	int tot_dcnt=0, tot_scnt=0;
	int tot_dwqty=0, tot_swqty=0;
	string code = (LPCSTR)m_SiseData.code;
	
	AccountMap::iterator ast, aed;
	ast = m_AcntData.begin();
	aed = m_AcntData.end();
	for(; ast!=aed; ++ast)
	{
		PriceMichegMap &pmm = ast->second.micheg[code];
		PriceMichegMap::iterator pst = pmm.begin();
		const PriceMichegMap::iterator ped = pmm.end();
		for(; pst!=ped; ++pst)
		{
			tot_dcnt += pst->second.d_cnt;
			tot_scnt += pst->second.s_cnt;
			tot_dwqty += pst->second.d_wqty;
			tot_swqty += pst->second.s_wqty;
		}
	}
}

void CMapWnd::res_pibofodr( LPCSTR data, int size )
{
	struct _ledgerH *ledger = (struct _ledgerH*)data;
	pibofodr_mod *mod = (pibofodr_mod *)(data+L_ledgerH);
	
	const UINT oidx = Str2Int(&ledger->fil2[ORDER_INFO_IDX], 6);
	const OrderAcnoMap::iterator pos = m_OdrAcnoMap.find(oidx);
	
	SetGuide( CString(ledger->emsg, sizeof(ledger->emsg)) );
	
	if (pos!=m_OdrAcnoMap.end())
	{
		const int nrec = Str2Int(mod->nrec, sizeof(mod->nrec));
		if (pos->second.size()!=nrec) return;
		for(int n=0; n<nrec; ++n)
		{
			int juno=0, ojno=0;
			OrderResult or;
			pibofodr_grid *grid = &mod->grid[n];
			or.acno = pos->second[n];
			or.emsg = CString(grid->emsg, sizeof(grid->emsg));
			juno = atoi(CString(grid->jmno, sizeof(grid->jmno)));
			ojno = atoi(CString(grid->ojno, sizeof(grid->ojno)));
			
			or.emsg.TrimRight();
			or.juno = (juno==0) ? "" : Int2CommaStr(juno);
			or.ojno = (ojno==0) ? "" : Int2CommaStr(ojno);
		}
	}
}

BOOL CMapWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	static CString oldVal;
	static CCellID oldId(-1, -1);

	if (wParam==IDC_PSWD_EDIT)
	{
		NMHDR *phdr = (NMHDR*)lParam;
		if (phdr->code==ENM_VKRETURN)
		{
			Send();
		}
	}    
    else if (wParam==IDC_JANGO_GRID)
	{
		m_pHogaWnd->SetFocusGrid();
	}
	else if (wParam==IDC_JQTY_GRID)
	{
		*pResult = 1;
        GV_DISPINFO *pi = (GV_DISPINFO*)lParam;
		if (pi->hdr.code == GVN_BEGINLABELEDIT)
		{
			// 편집 시작하면 현재 값을 저장
			oldVal = m_pJqty->GetItemText(pi->item.row, pi->item.col);
		}
        else if (pi->hdr.code == GVN_ENDLABELEDIT || pi->hdr.code == GVN_SELCHANGED)
		{
			const CCellID cId(pi->item.row, pi->item.col);
			CString key, val = m_pJqty->GetItemText(cId.row, cId.col);
			val.TrimLeft(); val.TrimRight();
			
			if (pi->hdr.code==GVN_ENDLABELEDIT)
			{
				// 공백이면 원래값으로 되돌린다.
				if (val.GetLength()>0)
				{
					key.Format("MA_%d", cId.row);
					WritePrivateProfileString("JQTY_GRID", (LPCSTR)key, (LPCSTR)val, (LPCSTR)m_profile);
				} 
				else 
				{
					val = oldVal;
					m_pJqty->SetItemText(cId.row, cId.col, oldVal);
				}
			}
			
			for(int row=0; row<m_pJqty->GetRowCount(); ++row)
			{
				for(int col=0; col<m_pJqty->GetColumnCount(); ++col)
				{
					if (pi->item.row==row && pi->item.col==col)
					{
						m_pJqty->SetItemBkColour(row, col, ::GetSysColor(COLOR_HIGHLIGHT));
						m_pJqty->SetItemFgColour(row, col, ::GetSysColor(COLOR_HIGHLIGHTTEXT));
					}
					else
					{
						m_pJqty->SetItemBkColour(row, col, m_crWhite);
						m_pJqty->SetItemFgColour(row, col, m_crBlack);
					}
				}
			}

			m_JqtyIndex = pi->item.row;

			m_pJqty->Refresh();
			m_pHogaWnd->SetFocusGrid();
		}
	}
	return CWnd::OnNotify(wParam, lParam, pResult);
}

void CMapWnd::proc_notice(LPCSTR data, int dlen)
{
	TRACE(data);

	int sym=0;
	CString val;
	RealData rdat;
	LPCSTR st{}, ed{}, p{};
	st = data;
	ed = data+dlen;
	
	op_notice_spliter op_split;
	while(st<ed)
	{
		p = find_if(st, ed, op_split);
		sym = Str2Int(st, p-st);
		st = p + 1;
		if (st>ed) break;
		p = find_if(st, ed, op_split);
		rdat[sym] = StringProxy(st, p);
		st = p + 1;
	} 

	string acno = rdat[901].ToStr();
	string code = rdat[907].ToStr();
	const int juno = Str2Int(rdat[904]);
	const int ojno = Str2Int(rdat[905]);
	const int oqty = Str2Int(rdat[909]);
	const int oprc = Str2Int(rdat[910]);
	const int dqty = Str2Int(rdat[931]);
	const int dprc = Str2Int(rdat[916]);
	const int wqty = Str2Int(rdat[921]);
	const int dtcd = Str2Int(rdat[996]);
	CString dsgb = rdat[912].ToCStr();

	TRACE("\nNOTICE data_cd(%d) dsgb(%s) acno(%s) code(%s) juno(%d) ojno(%d) oprc(%d) oqty(%d) dprc(%d) dqty(%d) wqty(%d)",
			dtcd, (LPCSTR)dsgb, acno.c_str(), code.c_str(), juno, ojno, oprc, oqty, dprc, dqty, wqty);

	const AccountMap::iterator pid = m_AcntData.find( acno );
	if (pid==m_AcntData.end()) return;

	Order* pOdr{};
	Account *pAcnt = &(pid->second);
	OrderMap *pOdrMap = &(pAcnt->order[code]);
	OrderMap::iterator pos;
	const OrderMap::iterator end = pOdrMap->end();
	switch(dtcd)
	{
	case 1:				// 접수
		pOdr = &(pOdrMap->operator [](juno));
		sprintf(pOdr->acno, acno.c_str());
		sprintf(pOdr->code, code.c_str());
		pOdr->juno = juno;
		pOdr->ojno = ojno;
		pOdr->oqty = oqty;
		pOdr->oprc = oprc;
		pOdr->dqty = dqty;
		pOdr->dprc = dprc;
		pOdr->wqty = wqty;
		if      (dsgb=="매수") pOdr->odgb = 2;
		else if (dsgb=="매도") pOdr->odgb = 1;
		else                   pOdr->odgb = 0;
		break;
	case 2:				// 정정접수
		break;
	case 3:				// 취소접수
		break;
	case 11:			// 체결
		pos = pOdrMap->find(juno);
		if (wqty<=0) 
		{
			// 전량 체결됬으면 삭제~
			if (pos != end)
			{
				pOdrMap->erase(pos);
			}
		} else 
		{
			// 남아있으면 미체결수량만 변경~
			if (pos != end)
			{
				pos->second.wqty = wqty;
			}
		}
		if (code[0]=='4')
		{
			if (m_pJango)
			{
				m_pJango->ClearCells(CCellRange(1, 0, 1, 4));
				m_pJango->RedrawRow(1);
				const NotifyMap::iterator pos = m_NotifyMap.find(acno);
				if (m_NotifyMap.end()!=pos)
					pos->second->Send(acno.c_str(), (LPCSTR)(m_AcntData[acno].pswd), "");
			}
		}
		break;
	case 12:			// 정정확인
		// 주문수량만큼 원주문에서 빼주고~, 원주문의 미체결수량이 0이면 삭제~
		pos = pOdrMap->find(ojno);
		if (pos != end)
		{
			pos->second.wqty -= oqty;
			if (pos->second.wqty<=0)
			{
				pOdrMap->erase(pos);
			}
		}
	
		// 정정으로 인한 신규주문 입력~
		pOdr = &(pOdrMap->operator [](juno));
		sprintf(pOdr->acno, acno.c_str());
		sprintf(pOdr->code, code.c_str());
		pOdr->juno = juno;
		pOdr->ojno = ojno;
		pOdr->oqty = oqty;
		pOdr->oprc = oprc;
		pOdr->dqty = dqty;
		pOdr->dprc = dprc;
		pOdr->wqty = wqty;
		if      (dsgb=="매수") pOdr->odgb = 2;
		else if (dsgb=="매도") pOdr->odgb = 1;
		else                   pOdr->odgb = 0;
		break;
	case 13:			// 취소확인
		// 취소수량만큼 원주문에서 빼주고, 미체결수량이 0이면 삭제~
		if (ojno>0)
			pos = pOdrMap->find(ojno);
		else
			pos = pOdrMap->find(juno);		// IOC/FOK일 경우 주문번호로 취소됨
		if (pos==end) break;

		pos->second.wqty -= oqty;
		if (pos->second.wqty<=0) 
		{
			pOdrMap->erase(pos);
		}
		break;
	case 14: case 15: case 16:	// 거부
		break;
	default:
		break;
	}
	_calc_micheg(pAcnt, code);
}

void CMapWnd::ChangeTheme()
{
	AxInit();
	LoadImage();
	MakeBackground();
	
	m_pHogaWnd->ChangeTheme();

	SetJangoGrid(TRUE);

	CalcSize();
	Invalidate(TRUE);
}

void CMapWnd::cancel_order( CancelData *pcd )
{
	vector< vector<char> > orders;
	AccountMap::iterator st = m_AcntData.begin();
	const AccountMap::iterator ed = m_AcntData.end();
	
	const LPCSTR code = m_SiseData.code;
	const CODE_TYPE type = m_SiseData.type;
	const int odgb = pcd->odgb;
	int d_oqty = 0, d_ojno = 0, d_jqty = 0;
	set<string> acno_qty;

	for(; st!=ed; ++st)
	{
		LPCSTR acno = st->first.c_str();
		LPCSTR pswd = GetPassword(acno);
		const CodeOrderMap::iterator pos = st->second.order.find(code);
		if (pos!=st->second.order.end())
		{
			OrderMap::iterator ost, oed;
			ost = pos->second.begin();
			oed = pos->second.end();
			for(; ost!=oed; ++ost)
			{
				if (ost->second.oprc == pcd->cprc && ost->second.odgb==pcd->odgb)
				{
					orders.push_back(vector<char>());
					make_fodr(orders.back(), 4, acno, pswd, ost->second.juno, code, ost->second.wqty, 0, 0);

					acno_qty.insert(acno);
					d_jqty += 1;
					d_oqty += ost->second.wqty;
					d_ojno = ost->second.juno;
				}
			}
		}
	}
	if (orders.empty()) return;
	
	vector<char> data;
	merge_order(data, orders);

	if (m_bShowConfirm)
	{
		CConfirmDlg dlg(this, m_pParent);
		dlg.m_cdnm = m_pStCodeNm->GetText();
		dlg.m_odgb = 4;
		dlg.m_ojno = d_ojno;
		dlg.m_acno_qty = acno_qty.size();
		dlg.m_oqty = d_oqty;
		dlg.m_jqty = d_jqty;
		const int ret = dlg.DoModal();
		m_pHogaWnd->SetFocusGrid();
		if (ret!=IDOK) return;
	}

	SendTR("pibofodr", US_ENC|US_CA, &data[0], data.size(), TK_PIBOFODR);
}

void CMapWnd::modify_order( ModifyData *pmd )
{
	vector< vector<char> > orders;
	AccountMap::iterator st = m_AcntData.begin();
	const AccountMap::iterator ed = m_AcntData.end();
	
	LPCSTR code = m_SiseData.code;
	const CODE_TYPE type = m_SiseData.type;
	const int odgb = pmd->odgb;
	const int cdgb = GetCdgb();
	int d_oqty = 0, d_ojno = 0, d_jqty = 0;
	set<string> acno_qty;
	
	for(; st!=ed; ++st)
	{
		LPCSTR acno = st->first.c_str();
		LPCSTR pswd = GetPassword(acno);
		const CodeOrderMap::iterator pos = st->second.order.find(code);
		if (pos!=st->second.order.end())
		{
			OrderMap::iterator ost, oed;
			ost = pos->second.begin();
			oed = pos->second.end();
			for(; ost!=oed; ++ost)
			{
				if (ost->second.oprc == pmd->fprc && ost->second.odgb == odgb)
				{
					orders.push_back(vector<char>());
					make_fodr(orders.back(), 3, acno, pswd, ost->second.juno, code, ost->second.wqty, pmd->tprc, cdgb);

					acno_qty.insert( acno );
					d_jqty += 1;
					d_oqty += ost->second.wqty;
					d_ojno = ost->second.juno;
				}
			}
		}
	}
	if (orders.empty()) return;

	vector<char> data;
	merge_order(data, orders);

	if (m_bShowConfirm)
	{
		CConfirmDlg dlg(this, m_pParent);
		dlg.m_cdnm = m_pStCodeNm->GetText();
		dlg.m_odgb = 3;
		dlg.m_ojno = d_ojno;
		dlg.m_old_oprc = pmd->fprc;
		dlg.m_oprc = pmd->tprc;
		dlg.m_acno_qty = acno_qty.size();
		dlg.m_oqty = d_oqty;
		dlg.m_jqty = d_jqty;
		const int ret = dlg.DoModal();
		m_pHogaWnd->SetFocusGrid();
		if (ret!=IDOK) return;
	}

	SendTR("pibofodr", US_ENC|US_CA, &data[0], data.size(), TK_PIBOFODR);
}

void CMapWnd::rsvd_order( ReserveData *prd )
{
	if (m_SiseData.code.GetLength()!=FCODE_LEN) return;

	if (prd->cprc == m_SiseData.curr)
	{
		LPCSTR msg = "현재가에는 예약주문을 할 수 없습니다.";
		SetGuide(msg);
		MessageBox(msg, "IBK투자증권", MB_OK|MB_ICONERROR);
		return;
	}

	int n=0, jqty=0;
	const int rsvd = GetRsvd();
	const CODE_TYPE type = m_SiseData.type;

	jqty = GetJcnt();

	prd->oprc = prd->cprc;
	prd->oqty = jqty;

	if (prd->oqty==0)
	{
		MessageBox("주문수량을 입력하십시오.", "IBK투자증권", MB_OK|MB_ICONWARNING);
		return;
	}
	
	if (prd->odgb==1)	// 매도
	{
		if      (rsvd>0) for(n=0; n<rsvd; ++n) prd->oprc = GetNextPrice(prd->oprc, type, false);
		else if (rsvd<0) for(n=0; n>rsvd; --n) prd->oprc = GetNextPrice(prd->oprc, type, true);
		prd->ptic -= rsvd;
	}
	else if (prd->odgb==2)
	{
		if      (rsvd>0) for(n=0; n<rsvd; ++n) prd->oprc = GetNextPrice(prd->oprc, type, true);
		else if (rsvd<0) for(n=0; n>rsvd; --n) prd->oprc = GetNextPrice(prd->oprc, type, false);
		prd->ptic += rsvd;
	}
	else return;

	m_RsvdData.push_back(*prd);
	m_pHogaWnd->SendMessage(WM_APP_REFRESH, WP_RESERVE, (LPARAM)prd);
}

void CMapWnd::CheckRsvd()
{
	vector<RsvdDataArray::iterator> delptr;

	ReserveData mado_rd, masu_rd;
	RsvdDataArray::iterator st, ed;
	st = m_RsvdData.begin();
	ed = m_RsvdData.end();
	for(; st!=ed; ++st)
	{
		BOOL bOrder = FALSE;
		if      (m_SiseData.pcurr <= st->cprc && m_SiseData.curr >= st->cprc)	bOrder = TRUE;    // 현재가가 아래서 위로 지나갈때 
		else if (m_SiseData.pcurr >= st->cprc && m_SiseData.curr <= st->cprc)	bOrder = TRUE;    // 현재가가 위에서 아래로 지나갈때
		if (bOrder)
		{
			Order *pOdr = new Order();
			pOdr->stop = true;
			pOdr->oprc = st->oprc;
			pOdr->oqty = st->oqty;
			pOdr->odgb = st->odgb;
			if      (pOdr->odgb==1) mado_rd = *st;
			else if (pOdr->odgb==2) masu_rd = *st;
			PostMessage(WM_APP_SIG, WP_ORDER, (LPARAM)pOdr);

			delptr.push_back(st);
		}
	}

	if (delptr.empty()) return;

	vector<RsvdDataArray::iterator>::reverse_iterator rst, red;
	rst = delptr.rbegin();
	red = delptr.rend();
	for(; rst!=red; ++rst)
		m_RsvdData.erase(*rst);

	if (mado_rd.row>0)
		m_pHogaWnd->SendMessage(WM_APP_REFRESH, WP_RESERVE, (LPARAM)&mado_rd);
	if (masu_rd.row>0)
		m_pHogaWnd->SendMessage(WM_APP_REFRESH, WP_RESERVE, (LPARAM)&masu_rd);
}

void CMapWnd::modify_rsvd(ModifyData *pmd)
{
	if (pmd->tprc == m_SiseData.curr)
	{
		LPCSTR msg = "현재가로 예약주문을 정정할 수 없습니다.";
		SetGuide(msg);
		MessageBox(msg, "IBK투자증권", MB_OK|MB_ICONERROR);
		return;
	}

	ReserveData mado_rd[2]{}, masu_rd[2]{};
	RsvdDataArray::iterator st, ed;

	st = m_RsvdData.begin();
	ed = m_RsvdData.end();
	for(; st!=ed; ++st)
	{
		if (st->cprc==pmd->fprc && st->odgb==pmd->odgb)
		{
			mado_rd[0] = *st;		// org
			st->cprc = pmd->tprc;
			st->oprc = GetPrice(pmd->tprc, m_SiseData.type, st->ptic);
			st->row  -= pmd->rtic;
			mado_rd[1] = *st;		// new
		}
	}

	for(int n=0; n<2; ++n)
	{		
		if (mado_rd[n].row>0)
			m_pHogaWnd->SendMessage(WM_APP_REFRESH, WP_RESERVE, (LPARAM)&mado_rd[n]);
		if (masu_rd[n].row>0)
			m_pHogaWnd->SendMessage(WM_APP_REFRESH, WP_RESERVE, (LPARAM)&masu_rd[n]);
	}
}

void CMapWnd::cancel_rsvd(CancelData *pcd)
{
	vector<RsvdDataArray::iterator> delptr;

	ReserveData mado_rd, masu_rd;
	RsvdDataArray::iterator st, ed;

	st = m_RsvdData.begin();
	ed = m_RsvdData.end();
	for(; st!=ed; ++st)
	{
		if ( pcd->cprc==st->cprc && pcd->odgb==st->odgb )
		{
			if      (st->odgb==1) mado_rd = *st;
			else if (st->odgb==2) masu_rd = *st;
			delptr.push_back(st);
		}
	}

	if (delptr.empty()) return;

	vector<RsvdDataArray::iterator>::reverse_iterator rst, red;
	rst = delptr.rbegin();
	red = delptr.rend();
	for(; rst!=red; ++rst)
		m_RsvdData.erase(*rst);

	if (mado_rd.row>0)
		m_pHogaWnd->SendMessage(WM_APP_REFRESH, WP_RESERVE, (LPARAM)&mado_rd);
	if (masu_rd.row>0)
		m_pHogaWnd->SendMessage(WM_APP_REFRESH, WP_RESERVE, (LPARAM)&masu_rd);
}

void CMapWnd::cancel_rsvd_all( int odgb )
{
	RsvdDataArray tmp;
	RsvdDataArray::iterator st, ed;
	vector<RsvdDataArray::iterator> delptr;
	
	st = m_RsvdData.begin();
	ed = m_RsvdData.end();
	for(; st!=ed; ++st)
	{
		if ( odgb == st->odgb )
		{
			delptr.push_back(st);
			tmp.push_back(*st);
		}
	}
	
	if (delptr.empty()) return;
	
	vector<RsvdDataArray::iterator>::reverse_iterator rst, red;
	rst = delptr.rbegin();
	red = delptr.rend();
	for(; rst!=red; ++rst)
		m_RsvdData.erase(*rst);
	
	st=tmp.begin(), ed=tmp.end();
	for(; st!=ed; ++st)
		m_pHogaWnd->SendMessage(WM_APP_REFRESH, WP_RESERVE, (LPARAM)&*st);
}

void CMapWnd::merge_order( vector<char> &buff, vector< vector<char> > &orders )
{
	buff.clear();
	if (orders.empty()) return;
	
	const int cnt = orders.size();
	const int offset = L_ledgerH + offsetof(pibofodr_mid, odgb);
	const int size = L_ledgerH + sizeof(pibofodr_mid);
	
	char tmp[32]{};
	buff.reserve(1024*16);
	
	// Ledger 삽입
	if (orders[0].size()<L_ledgerH) return;
	buff.insert(buff.end(), orders[0].begin(), orders[0].begin() + L_ledgerH);
	
	// 레코드갯수 조정
	sprintf(tmp, "%04d", cnt);
	buff.insert(buff.end(), tmp, tmp+4);
	
	// 주문데이터 연결
	for(int n=0; n<cnt; ++n)
	{
		if (orders[n].size()==size)
			buff.insert(buff.end(), orders[n].begin() + offset, orders[n].end());
	}
	
	reg_order((struct _ledgerH*)&buff[0], orders);
}

void CMapWnd::make_fodr( vector<char> &buff, int mmgb, LPCSTR acno, LPCSTR pswd, int ojno, LPCSTR code, int jqty, int jprc, int cdgb )
{
	if (mmgb<1 || mmgb>4) return;
	if (!acno || !pswd || !code) return;
	if (mmgb>2 && ojno<1) return;
	
	const int lsize = L_ledgerH;
	const int dsize = sizeof(pibofodr_mid);
	buff.resize(L_ledgerH + sizeof(pibofodr_mid));
	FillMemory(&buff[0], buff.size(), ' ');
	
	struct _ledgerH *ledger = (struct _ledgerH *)&buff[0];
	pibofodr_mid *mid = (pibofodr_mid*)&buff[L_ledgerH];
	
	// Ledger 세팅
	GetLedger(ledger);
	CopyMemory(ledger->odrf, "1", 1);
	CopyMemory(ledger->rcnt, "0000", 4);
	ledger->mkty[0] = '2';
	ledger->odrf[0] = '1';
	
	// 데이터 세팅
	CopyMemory(mid->nrec, "0001", 4);
	mid->odgb[0] = '2';
	mid->mmgb[0] = '0' + mmgb;
	sprintf(mid->accn, "%11s", acno);
	sprintf(mid->pswd, "%-8s", pswd);
	sprintf(mid->ojno, "%12d", ojno);
	sprintf(mid->cod2, "%s", code);
	sprintf(mid->jqty, "%8d", jqty);
	sprintf(mid->jprc, "%10d", jprc);
	CopyMemory(mid->hogb, "00", 2);
	sprintf(mid->cdgb, "%d", cdgb);
	mid->mdgb[0] = '2';
	mid->prgb[0] = 'X';
	FillMemory(mid->usrf, sizeof(mid->usrf), ' ');
}

void CMapWnd::reg_order( struct _ledgerH *ledger, vector< vector<char> > &orders)
{
	static UINT regid = 0;
	
	vector< vector<char> >::iterator st, ed;
	vector<CString> *pam = &(m_OdrAcnoMap[regid]);
	
	for(st=orders.begin(), ed=orders.end(); st!=ed; ++st)
	{
		pibofodr_mid *mid = (pibofodr_mid*)( &(*st)[L_ledgerH] );
		pam->push_back( CString(mid->accn, 11) );
	}
	
	sprintf(&ledger->fil2[ORDER_INFO_IDX], "%06d", regid++);
}

BOOL CMapWnd::CheckAccount()
{
	if (m_AcntData.empty())
	{
		SetGuide("계좌번호 혹은 그룹을 확인하시기 바랍니다.");
		return FALSE;
	} else return TRUE;
}

void CMapWnd::click_order( Order *pOdr )
{
	if (m_SiseData.code.GetLength()!=FCODE_LEN) return;
	
	vector< vector<char> > orders;
	AccountMap::iterator st = m_AcntData.begin();
	const AccountMap::iterator ed = m_AcntData.end();
	
	LPCSTR code = m_SiseData.code;
	const CODE_TYPE type = m_SiseData.type;
	const int odgb = pOdr->odgb;
	
	const int jqty = (pOdr->stop) ? pOdr->oqty : GetJcnt();
	const int cdgb = GetCdgb();
	const int mult = GetMulti();
	int oprc = pOdr->oprc;
	int d_acno_qty = 0, d_oqty = 0;
	if (jqty==0)
	{
		MessageBox("주문수량을 입력하십시오.", "IBK투자증권", MB_OK|MB_ICONWARNING);
		return;
	}
	
	for(; st!=ed; ++st)
	{		
		LPCSTR acno = st->first.c_str();
		LPCSTR pswd = GetPassword(acno);
		oprc = pOdr->oprc;	// 여기서 주문가격 초기화 안하면 계좌별로 계속 가격 누적된다!
		
		orders.push_back(vector<char>());
		make_fodr(orders.back(), odgb, acno, pswd, 0, code, jqty * st->second.mult, oprc, cdgb);
		
		d_acno_qty += 1;
		d_oqty += jqty * st->second.mult;
		
		if (!pOdr->stop)	// stop일 경우 다중매매를 적용하지 않는다.
		{
			for(int n=1; n<mult; ++n)
			{
				orders.push_back(vector<char>());
				if      (odgb==1) oprc = GetNextPrice(oprc, type, true);
				else if (odgb==2) oprc = GetNextPrice(oprc, type, false );
				else continue;
				
				make_fodr(orders.back(), odgb, acno, pswd, 0, code, jqty * st->second.mult, oprc, cdgb);
			}
		}
	}
	if (orders.empty()) return;
	
	vector<char> data;
	merge_order(data, orders);
	
	if (pOdr->stop) delete pOdr;	// stop일 경0우 동적생성된 것이다! PostMessage
	
	if (m_bShowConfirm)
	{
		CConfirmDlg dlg(this, m_pParent);
		dlg.m_cdnm = m_pStCodeNm->GetText();
		dlg.m_odgb = odgb;
		dlg.m_cdgb = cdgb;
		dlg.m_oprc = oprc;
		dlg.m_acno_qty = d_acno_qty;
		dlg.m_oqty = d_oqty;
		const int ret = dlg.DoModal();
		m_pHogaWnd->SetFocusGrid();
		if (ret!=IDOK) return;
		
	}
	SendTR("pibofodr", US_ENC|US_CA, &data[0], data.size(), TK_PIBOFODR);
}

void CMapWnd::cancel_order_all(int odgb)
{
	vector< vector<char> > orders;
	AccountMap::iterator ast = m_AcntData.begin();
	const AccountMap::iterator aed = m_AcntData.end();
	
	for(; ast!=aed; ++ast)
	{
		OrderMap *om = &(ast->second.order[(LPCSTR)m_SiseData.code]);
		OrderMap::iterator ost = om->begin();
		const OrderMap::iterator oed = om->end();
		for(; ost!=oed; ++ost)
		{
			Order *pOdr = &(ost->second);
			if ((pOdr->odgb==odgb||odgb==-1) && pOdr->wqty>0)
			{
				orders.push_back(vector<char>());
				make_fodr(orders.back(), 4, pOdr->acno, GetPassword(pOdr->acno), pOdr->juno, pOdr->code, pOdr->wqty, 0, 0);
			}
		}
	}
	if (orders.empty()) return;
	
	vector<char> data;
	merge_order(data, orders);
	SendTR("pibofodr", US_ENC|US_CA, &data[0], data.size(), TK_PIBOFODR);
}

void CMapWnd::proc_jango( Jango *j )
{
	BOOL bSend = TRUE;

	/* 실시간잔고컨트롤의 버그를 일단 여기서 Fix */
	if ( j->flag!='D')
	{
		int wqty = 0;
		if (strcmp(j->gubn, "+매수")==0)
		{
			PriceMichegMap &pmm = m_AcntData[j->acno].micheg[j->code];
			PriceMichegMap::iterator st = pmm.begin();
			const PriceMichegMap::iterator ed = pmm.end();
			for(; st!=ed; ++st)
				wqty += st->second.d_wqty;
			j->xqty = max(j->jqty-wqty, 0);
		}
		else if (strcmp(j->gubn, "-매도")==0)
		{
			PriceMichegMap &pmm = m_AcntData[j->acno].micheg[j->code];
			PriceMichegMap::iterator st = pmm.begin();
			const PriceMichegMap::iterator ed = pmm.end();
			for(; st!=ed; ++st)
				wqty += st->second.s_wqty;
			j->xqty = max(j->jqty-wqty, 0);
		}
	}
	/* -------- */

	if ( j->flag=='I')
	{
		m_AcntData[j->acno].jango[j->code] = *j;
	}
	else if (j->flag=='D')
	{
		m_AcntData[j->acno].jango.erase(j->code);
	}
	else if (j->flag=='U')
	{
		Jango *org_j = &(m_AcntData[j->acno].jango[j->code]);
		bSend = (j->jqty != org_j->jqty);
		CopyMemory(org_j, j, sizeof(Jango));
	}

	/* m_pJngoTick 및 m_pControl의 경우 현재종목이 아닌 잔고에 대해서는 갱신할 필요가 없다 */
	if (m_SiseData.code!=j->code) return;

	RefreshJango();
}

void CMapWnd::SetJangoGrid(BOOL bOnlyHeader)
{
	m_pJango->SetFont(m_pFont, FALSE);

	m_pJango->SetDefCellHeight(GRID_CELL_HEIGHT);
	m_pJango->SetDefCellWidth(50);

	m_pJango->SetNoScrollBar(TRUE);
	m_pJango->SetColumnResize(FALSE);
	m_pJango->EnableSelection(FALSE);
	m_pJango->SetDoubleBuffering(TRUE);
	m_pJango->SetEditable(FALSE);
	m_pJango->SetFrameFocusCell(FALSE);

	m_pJango->SetColumnCount(4);
	m_pJango->SetRowCount(2);
	m_pJango->SetFixedRowCount(1);
	m_pJango->SetFixedColumnCount(0);

	m_pJango->SetGridBorderColor(RGB(0x6B,0x6B,0x6B));
	m_pJango->SetGridLineColor(RGB(0x6B,0x6B,0x6B));
	m_pJango->SetDefCellMargin(1);

	int n=0;
	const int widths[] = { 35, 30, 45, 65 };
	const LPCSTR header[] = { "구분", "수량", "평균가", "평가손익" };
	for(n=0; n<m_pJango->GetColumnCount(); ++n)
	{
		m_pJango->SetColumnWidth(n, widths[n]);
		GV_ITEM item;
		item.row = 0;
		item.col = n;
		item.crBkClr = GetIndexColor(COLOR_GRIDHEAD);
		item.crFgClr = GetIndexColor(COLOR_GRIDHEADTXT);	
		item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_BKCLR|GVIF_FGCLR;
		item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
		item.strText = header[n];
		m_pJango->SetItem(&item);
	}

	if (!bOnlyHeader)
	{
		for(n=0; n<m_pJango->GetColumnCount(); ++n)
		{
			GV_ITEM item;
			item.row = 1;
			item.col = n;
			item.lParam = 0;
			item.nMargin = 5;
			item.crFgClr = GetIndexColor(COLOR_TEXT);
			item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_FGCLR|GVIF_MARGIN;
			item.nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			item.strText = "";
			m_pJango->SetItem(&item);
		}
	}
}

BOOL CMapWnd::DestroyWindow() 
{
	return CWnd::DestroyWindow();
}

void CMapWnd::OnDestroy() 
{
	ClearAccount();
/*
	if (m_pStAccn) { m_pStAccn->DestroyWindow();     delete m_pStAccn; }
	if (m_pAcno)			{ m_pAcno->DestroyWindow();       delete m_pAcno;       }
	if (m_pPswd)			{ m_pPswd->DestroyWindow();       delete m_pPswd;       }
	if (m_pStAcnm)			{ m_pStAcnm->DestroyWindow();     delete m_pStAcnm;     }
	if (m_pStCode)			{ m_pStCode->DestroyWindow();     delete m_pStCode;     }
	if (m_pCode)			{ m_pCode->DestroyWindow();       delete m_pCode;       }
	if (m_pBtnSearch)		{ m_pBtnSearch->DestroyWindow();  delete m_pBtnSearch;  }
	if (m_pStCodeNm)		{ m_pStCodeNm->DestroyWindow();   delete m_pStCodeNm;   }
	if (m_pHogaWnd)			{ m_pHogaWnd->DestroyWindow();    delete m_pHogaWnd;    }
	if (m_pStGuide)			{ m_pStGuide->DestroyWindow();    delete m_pStGuide;    }
	if (m_pJango)			{ m_pJango->DestroyWindow();      delete m_pJango;      }
	if (m_pJqty)			{ m_pJqty->DestroyWindow();       delete m_pJqty;       }
	if (m_pStJango)			{ m_pStJango->DestroyWindow();    delete m_pStJango;    }
	if (m_pBtnSend)			{ m_pBtnSend->DestroyWindow();    delete m_pBtnSend;    }
	if (m_pBtnCheg)			{ m_pBtnCheg->DestroyWindow();    delete m_pBtnCheg;    }
	if (m_pBtnJngo)			{ m_pBtnJngo->DestroyWindow();    delete m_pBtnJngo;    }
	if (m_pBtnConf)			{ m_pBtnConf->DestroyWindow();    delete m_pBtnConf;    }
	if (m_pBtnAlign)		{ m_pBtnAlign->DestroyWindow();	  delete m_pBtnAlign;   }
	if (m_pBtnCancel)       { m_pBtnCancel->DestroyWindow();  delete m_pBtnCancel;  }
*/
	CWnd::OnDestroy();	
}

void CMapWnd::RefreshJango()
{
	const AccountMap::iterator apos = m_AcntData.find( (LPCSTR)m_pAcno->GetNumber() );
	if (apos!=m_AcntData.end())
	{
		const CodeJangoMap::iterator cpos = apos->second.jango.find( (LPCSTR)m_SiseData.code );
		if (cpos!=apos->second.jango.end())
		{
			Jango *j = &cpos->second;

			char buff[128]{};
			switch (j->gubn[0])
			{
			case '+':
				m_pJango->SetItemFgColour(1, 0, m_crPlus);
				m_pStJango->SetFgColor(m_crPlus, false);
				break;				
			case '-': 
				m_pJango->SetItemFgColour(1, 0, m_crMinus);
				m_pStJango->SetFgColor(m_crMinus, false);
				break;
			default :
				m_pJango->SetItemFgColour(1, 0, m_crBlack);
				m_pStJango->SetFgColor(m_crBlack, false);
				break;
			}

			sprintf(buff, "%c%d", j->gubn[0], j->jqty);
			m_pStJango->SetText((j->jqty==0) ? "" : buff);
			m_pJango->SetItemText(1, 0, &(j->gubn[1]));
			m_pJango->SetItemText(1, 1, (j->jqty==0) ? "" : Int2CommaStr(j->jqty));
			m_pJango->SetItemText(1, 2, (j->pprc==0) ? "" : Int2Str(j->pprc, 2));
			m_pJango->SetItemFgColour(1, 3, (j->tamt>0) ? m_crPlus : m_crMinus);
			m_pJango->SetItemText(1, 3, (j->tamt==0) ? "" : Double2CommaStr(j->tamt));
			m_pJango->RedrawRow(1);
			return;
		}
	}
	m_pJango->SetItemText(1, 0, "");
	m_pJango->SetItemText(1, 1, "");
	m_pJango->SetItemText(1, 2, "");
	m_pJango->SetItemText(1, 3, "");
	m_pJango->RedrawRow(1);
	m_pStJango->SetText("");
}

void CMapWnd::SetJqtyGrid()
{
	m_pJqty->SetFont(m_pFont, FALSE);
	
	m_pJqty->SetDefCellHeight(GRID_CELL_HEIGHT);
	m_pJqty->SetDefCellWidth(25);
	
	m_pJqty->SetNoScrollBar(TRUE);
	m_pJqty->SetColumnResize(FALSE);
	m_pJqty->EnableSelection(FALSE);
	m_pJqty->SetDoubleBuffering(TRUE);
	m_pJqty->SetEditable(TRUE);
	m_pJqty->SetFrameFocusCell(FALSE);
	
	m_pJqty->SetColumnCount(1);
	m_pJqty->SetRowCount(5);
	m_pJqty->SetFixedRowCount(0);
	m_pJqty->SetFixedColumnCount(0);
	
	m_pJqty->SetGridBorderColor(RGB(0x6B,0x6B,0x6B));
	m_pJqty->SetGridLineColor(RGB(0x6B,0x6B,0x6B));
	m_pJqty->SetDefCellMargin(1);

	const int jqtys[] = { 1, 3, 5, 10, 20 };
	for(int n=0; n<m_pJqty->GetRowCount(); ++n)
	{
		m_pJqty->SetItemFormat(n, 0, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
		m_pJqty->SetItemText(n, 0, (LPCSTR)Int2CommaStr(jqtys[n]));
	}
}

int CMapWnd::GetJcnt()
{
	if (m_JqtyIndex<0 || m_JqtyIndex>=m_pJqty->GetRowCount()) return 0;
	CString jqty = m_pJqty->GetItemText(m_JqtyIndex, 0);
	return atoi(jqty);
}

void CMapWnd::OnCodeSearch()
{
	CRect rect;
	m_pCode->GetWindowRect(&rect);
	LPCSTR data = (LPCSTR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(foptionCODE, 2)), MAKELPARAM(rect.left, rect.bottom + 2));
	
	vector<StringProxy> fields;
	ParseString(data, data+strlen(data), '\t', fields);
	
	if (fields.empty()) return;
	
	CString code = fields[0].ToCStr();
	if (code.GetLength()==5)
		code += "000";
	
	ChangeCode(code);
}

void CMapWnd::OnSendBtn()
{
	Send();
}

void CMapWnd::OnChegBtn()
{
	CString opt;
	opt.Format(MAP_CHEG);
	SetView(opt);
}

void CMapWnd::OnJngoBtn()
{
	CString opt;
	opt.Format(MAP_JANGO);
	SetView(opt);	
}

void CMapWnd::OnConfBtn()
{
	int tmp=0;
	CConfigDlg dlg(this, m_profile);

	dlg.m_bEnterOrder = m_pHogaWnd->GetEnterOrder();
	dlg.m_bSpaceOrder = m_pHogaWnd->GetSpaceOrder();
	dlg.m_bShowBefore = m_pHogaWnd->IsShowBefore();
	m_pHogaWnd->GetWidths(&tmp, (int*)&dlg.m_iOdrWidth, (int*)&dlg.m_iCntWidth, (int*)&dlg.m_iVolWidth, (int*)&dlg.m_iHogaWidth, &tmp);
	dlg.m_bPutReverse =  m_bHogaReverse;
	dlg.m_bOrderConfirm = m_bShowConfirm;

	if (dlg.DoModal()==IDOK)
	{
		m_pHogaWnd->SetEnterOrder(dlg.m_bEnterOrder);
		m_pHogaWnd->SetSpaceOrder(dlg.m_bSpaceOrder);
		
		m_pHogaWnd->ShowBefore(dlg.m_bShowBefore ? true : false);
		m_pHogaWnd->SetWidths(0, dlg.m_iOdrWidth, dlg.m_iCntWidth, dlg.m_iVolWidth, dlg.m_iHogaWidth, 0);
		m_pHogaWnd->ShowRate(FALSE);
		m_pHogaWnd->ShowRsvd(FALSE);

		m_bHogaReverse = dlg.m_bPutReverse;
		m_bShowConfirm = dlg.m_bOrderConfirm;

		switch(dlg.m_iClickMode)
		{
		case 0: m_pHogaWnd->SetOneClickMode(TRUE); break;
		case 1: 
		default: m_pHogaWnd->SetOneClickMode(FALSE); break;
		}

		CalcSize();
	}
	SaveConfig();
	m_pHogaWnd->SetFocusGrid();
}

void CMapWnd::OnAlignBtn()
{
	m_pHogaWnd->SetCurrToCenter();
	m_pHogaWnd->SetFocusGrid();
}

void CMapWnd::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	m_pHogaWnd->SetFocusGrid();
}

void CMapWnd::OnCancelBtn()
{
	cancel_order_all(-1);
	m_pHogaWnd->SetFocusGrid();
}

void CMapWnd::proc_domino( LPCSTR data, int size )
{
	
}

void CMapWnd::RefreshRsvd()
{
	RsvdDataArray::iterator pos = m_RsvdData.begin();
	const RsvdDataArray::iterator ed = m_RsvdData.end();
	for(; pos!=ed; ++pos)
		m_pHogaWnd->SendMessage(WM_APP_REFRESH, WP_RESERVE, (LPARAM)&*pos);
}