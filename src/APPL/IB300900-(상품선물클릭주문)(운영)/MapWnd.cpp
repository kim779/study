// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB300900.h"
#include "MapWnd.h"
#include "math.h"
#include "AccountConfirmWnd.h"
#include "../../h/MistakeStopOrder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMapWnd::CMapWnd(CWnd *pParent) 
: CAxisExt(pParent)
{
	ZeroMemory(m_hRndBmp, sizeof(m_hRndBmp));

	m_home = Variant(homeCC, _T(""));
	m_user = Variant(userCC, _T(""));
	m_name = Variant(nameCC, _T(""));
#ifdef _DEF_IB300900_
	Variant(titleCC, _T("[3009] 상품선물 클릭주문"));
#else
	Variant(titleCC, _T("[3007] 선물옵션 그룹 클릭주문"));
#endif

	SetLogMode( (GetLogConfig("IB300600.MapWnd")==0) ? FALSE : TRUE );
	WriteLog("MapWnd Start!! -------------------------------------------------------------");

	m_profile.Format(CONFIG_INI, Variant(homeCC,""), Variant(nameCC,""));

	for(int n=TK_NOTIFY_CTRL; n<=0xFF; ++n)
		m_NotifyKeys.push_back(n);

	m_bFoldCtrl1 = FALSE;
	m_bFoldCtrl2 = FALSE;

#ifdef _DEF_IB300900_
	m_AcntMode = AM_ACCOUNT;
#else
	m_AcntMode = AM_GROUP;
#endif
	m_pHogaWnd = new CFOHogaWnd(m_pParent, &m_AcntData, &m_RsvdData);
	m_pSise = new CSiseWnd(m_pParent);
	m_pControl = new CControlWnd(m_pParent,this, &m_AcntData, &m_SiseData);
	m_pControl2 = new CControl2Wnd(m_pParent, &m_AcntData, &m_SiseData);
	m_pGroup = new CComboBox();
	m_pStAccn = new CfxStatic(m_pFont);
	m_pStCode = new CfxStatic(m_pFont);
	m_pStAcnm = new CfxStaticEx(m_pFont);
	m_pPswd = new CfxEditEx(m_pFont);

	m_pAcno = new CAccountCtrl(m_pParent, "AN20", TK_ACCOUNT_CTRL1);
	m_pCode = new CCodeCtrl(m_pParent, "", TK_CODE_CTRL);
	m_pStCodeNm = new CfxStaticEx(m_pFont);

	m_pJngoTick = new CJngoTicker(m_pParent);

	m_hiSha256 = NULL;

	m_RsvdData.reserve(100);

	m_hBrush1 = (HBRUSH)CreateSolidBrush( m_crBodyBk );
	m_hBrush2 = (HBRUSH)CreateSolidBrush( RGB(255,0,0) );

	LoadConfig();
	LoadImage();
}

CMapWnd::~CMapWnd()
{
	if (m_hiSha256)
	{
		AfxFreeLibrary(m_hiSha256);
		m_hiSha256 = NULL;
	}
}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
	ON_MESSAGE(WM_APP_SIG, OnAppSignal)
	ON_MESSAGE(WM_CODE,OnGetCode)
	ON_BN_CLICKED(IDC_TEST_BTN, OnTest)
	ON_EN_SETFOCUS(IDC_EDIT_PSWD, OnPswdFocus)
	ON_BN_CLICKED(IDC_BTN_SEND, OnSendBtn)
	ON_BN_CLICKED(IDC_BTN_CONF, OnConfBtn)
	ON_BN_CLICKED(IDC_BTN_GROUP, OnGroupEdit)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnCodeSearch)
	ON_BN_CLICKED(IDC_BTN_LIMIT, OnLimit)
	ON_BN_CLICKED(IDC_BTN_FOLD1, OnFold)
	ON_CBN_SELCHANGE(IDC_CB_GROUP, OnGroupChange)
END_MESSAGE_MAP()

void CMapWnd::Init()
{
}

LRESULT CMapWnd::OnUser( WPARAM wParam, LPARAM lParam )
{
	int msg = LOBYTE(LOWORD(wParam));
	int key = HIBYTE(LOWORD(wParam));
	int len = HIWORD(wParam);
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
		case 250:
			if (m_pAcno) m_pAcno->m_pCtrl->SendMessage(WM_USER, wParam, lParam); 
			break;
		case TK_SDLDQ003:
			res_SDLDQ003(dat, len);
			break;
		case TK_SACMT238:
			res_SACMT238(dat, len);
			break;
		case TK_SONBQ101:
			if (m_pControl2 && m_AcntMode==AM_GROUP)
				m_pControl2->SendMessage(WM_USER, wParam, lParam);
			else if (m_pControl && m_AcntMode==AM_ACCOUNT)
				m_pControl->SendMessage(WM_USER, wParam, lParam);
			break;
		case TK_PIBOFODR:
			res_pibofodr(dat, len);
			break;
		}
		break;
// 	case DLL_ALERT:
// 		proc_alert(dat, strlen(dat));
// 		break;
	case DLL_ALERTx:
		struct _alertR* alertR;
		alertR = (struct _alertR*)lParam;
		proc_alert(alertR);
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
		proc_guide((LPCSTR)lParam);
		return true;
	case DLL_DOMINO:
		break;
	}
	return 0;
}

void CMapWnd::LoadImage()
{
	TCHAR buff[1024];
	CString path, img_path;

	path.Format(_T("%s\\tab\\PALETTE.INI"), (LPCTSTR)m_home);
	int readL = GetPrivateProfileString(_T("General"), _T("Palette"), _T("Blue"), buff, sizeof(buff), path);
	if (readL==0) return;

	img_path.Format(_T("%s\\image\\axpanel%s1_"), (LPCSTR)m_home, buff);

	m_hRndBmp[0] = GetAxBitmap((LPCSTR)( img_path + _T("lt.bmp")));
	m_hRndBmp[1] = GetAxBitmap((LPCSTR)( img_path + _T("rt.bmp")));
	m_hRndBmp[2] = GetAxBitmap((LPCSTR)( img_path + _T("lb.bmp")));
	m_hRndBmp[3] = GetAxBitmap((LPCSTR)( img_path + _T("rb.bmp")));
	m_hGuide     = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\알림_ic.bmp")));
	m_hCombo[0] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\CONF_COMBO.BMP")));
	m_hCombo[1] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\CONF_COMBO_N.BMP")));

	m_hBtn2[0] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\2btn.bmp")));
	m_hBtn2[1] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\2btn_dn.bmp")));
	m_hBtn2[2] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\2btn_en.bmp")));
	
	m_hBtn4[0] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\4btn.bmp")));
	m_hBtn4[1] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\4btn_dn.bmp")));
	m_hBtn4[2] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\4btn_en.bmp")));

	m_hSearch[0] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\검색.bmp")));
	m_hSearch[1] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\검색_dn.bmp")));
	m_hSearch[2] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\검색_en.bmp")));
}

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this);
	Draw(&dc);
	m_pStJcnt->Invalidate(TRUE);
	m_pPswd->Invalidate(TRUE);
	m_pStCodeNm->Invalidate(TRUE);
	m_pStAcnm->Invalidate(TRUE);
}

void CMapWnd::Draw(CDC *pdc)
{
	CDC mdc;
	CRect rc;

	GetClientRect(rc);
	mdc.CreateCompatibleDC(pdc);	
	CBitmap *oldBmp = mdc.SelectObject(&m_bgBmp);
	pdc->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &mdc, 0, 0, SRCCOPY);
	mdc.SelectObject(oldBmp);
	
	mdc.DeleteDC();	
}

void CMapWnd::DrawRoundRectangle(CDC *pDC, CRect rc)
{
	CSize size(2,2);
	pDC->FillSolidRect((LPRECT)rc, m_crBk);
	rc.DeflateRect(size);
	pDC->FillSolidRect((LPRECT)rc, m_crBodyBk);
	DrawBitmap(pDC, rc.left,    rc.top,      5, 5, m_hRndBmp[0]->operator HBITMAP());
	DrawBitmap(pDC, rc.right-5, rc.top,      5, 5, m_hRndBmp[1]->operator HBITMAP());
	DrawBitmap(pDC, rc.left,    rc.bottom-5, 5, 5, m_hRndBmp[2]->operator HBITMAP());
	DrawBitmap(pDC, rc.right-5, rc.bottom-5, 5, 5, m_hRndBmp[3]->operator HBITMAP());
}

void CMapWnd::DrawBitmap(CDC *pDC, int x, int y, int w, int h, HBITMAP hBitmap, COLORREF tCol)
{
	HDC hDC = ::CreateCompatibleDC(pDC->m_hDC);
	HBITMAP tBmp = (HBITMAP)::SelectObject(hDC, hBitmap);
	::TransparentBlt(pDC->m_hDC, x, y, w, h, hDC, 0, 0, w, h, tCol);
	::SelectObject(hDC, tBmp);
	::DeleteObject(hDC);
}

void CMapWnd::DrawBitmap(CDC *pDC, CRect *pRC, HBITMAP hBitmap, COLORREF tCol)
{
	DrawBitmap(pDC, pRC->left, pRC->top, pRC->Width(), pRC->Height(), hBitmap, tCol);
}

void CMapWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	CalcSize();
}

void CMapWnd::MakeBackground( int width, int height )
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

		m_bgBmp.DeleteObject();
		m_bgBmp.CreateCompatibleBitmap(pdc, width, height);
		//TRACE("MakeBackground %d %d\n", width, height);

		mdc.CreateCompatibleDC(pdc);
		oldBmp = mdc.SelectObject(&m_bgBmp);
		
		// Drawing 
		DrawRoundRectangle(&mdc, m_rcAccn);		// 상단 계좌/종목 선택 윈도우
		DrawRoundRectangle(&mdc, m_rcHoga);		// 호가그리드 윈도우
		if (!m_bFoldCtrl1)
		{
			DrawRoundRectangle(&mdc, m_rcCtrl2);	// 우우측 실시간잔고 윈도우
			DrawRoundRectangle(&mdc, m_rcCtrl);		// 우측 주문설정/잔고 윈도우
		}
		DrawRoundRectangle(&mdc, m_rcGuide);	// 하단 GUIDE메세지 윈도우

		//TRACE("m_rcCtrl2 -> %d %d, %d %d\n", m_rcCtrl2.left, m_rcCtrl2.top, m_rcCtrl2.Width(), m_rcCtrl2.Height());
		// guide
		CRect rc(m_rcGuide);
		rc.top += (rc.Height()-14)/2;
		rc.left = PADDING_VALUE;
		rc.bottom = rc.top + 14;
		rc.right = rc.left + 14;
		DrawBitmap(&mdc, &rc, m_hGuide->operator HBITMAP(), RGB(0xDD,0xDD,0xDD));

		mdc.SelectObject(oldBmp);
		mdc.DeleteDC();
	}
	ReleaseDC(pdc);
}

int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	int hoga_width, total_width, total_height;

	// Calculate window size
	hoga_width = m_pHogaWnd->GetRequireWidth() + (PADDING_VALUE*2);
	total_width  = hoga_width + CTRL_WND_WIDTH;
	total_height = MAIN_WND_HEIGHT;

	lpCreateStruct->cx = total_width;
	lpCreateStruct->cy = total_height;

	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rc(0, 0, 0, 0);

	m_pHogaWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rc, this, -1);

	m_pSise->Create(rc, this, -1, WS_CHILD|WS_BORDER|WS_VISIBLE);
	m_pJngoTick->Create(rc, this, IDC_GRID_JANGO, WS_CHILD|WS_BORDER|WS_VISIBLE);
	m_pControl->Create(NULL, "CTRLWND", WS_CHILD|WS_VISIBLE, rc, this, -1);
	m_pControl2->Create(NULL, "CTRL2WND", WS_CHILD|WS_VISIBLE, rc, this, -1);

	m_pStAccn->Create("그 룹", WS_CHILD|WS_VISIBLE, rc, this, -1);
	m_pStAccn->SetBkColor(m_crBodyBk);
	m_pStCode->Create("종 목", WS_CHILD|WS_VISIBLE, rc, this, -1);
	m_pStCode->SetBkColor(m_crBodyBk);
	m_pStAcnm->Create("", WS_CHILD|WS_VISIBLE|WS_BORDER|SS_LEFT, rc, this, IDC_ST_ACNM);
	m_pStAcnm->Init(WS_CHILD|WS_VISIBLE|WS_BORDER, ssTEXT|ssBORDER, "", "", RGB(0xE8, 0xE0, 0xED), RGB(0xE8, 0xE0, 0xED), RGB(0xBF, 0xBF, 0xBF), alLEFT, 0);
	m_pStAcnm->SetBkColor(RGB(0xEB, 0xEB, 0xEB));
	m_pStCodeNm->Create("", WS_CHILD|WS_VISIBLE|WS_BORDER|SS_LEFT, rc, this, -1);
	m_pStCodeNm->Init(WS_CHILD|WS_VISIBLE|WS_BORDER|ssTEXT, 0, "", "", RGB(0xEB, 0xEB, 0xEB), RGB(0xEB, 0xEB, 0xEB), RGB(0xBF, 0xBF, 0xBF), alLEFT, 0);
	m_pStCodeNm->SetBkColor(RGB(0xEB, 0xEB, 0xEB));

	m_pAcno->Create(NULL, "AN20", WS_CHILD|WS_TABSTOP, rc, this, -1);
	m_pGroup->Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST, rc, this, IDC_CB_GROUP);
	m_pGroup->SetFont(m_pFont);
	m_pGrpEdit = CreateImgButton("편집", IDC_BTN_GROUP, m_hBtn2);
	m_pPswd->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_PASSWORD|ES_AUTOHSCROLL, rc, this, IDC_EDIT_PSWD);
	m_pPswd->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, 0);
	m_pPswd->SetLimitText(10);
	
	m_pSend = CreateImgButton("조회", IDC_BTN_SEND, m_hBtn2);
	m_pConf = CreateImgButton("설정", IDC_BTN_CONF, m_hBtn2);
	m_pLimit = CreateImgButton("한도", IDC_BTN_LIMIT, m_hBtn2);

	// Fold기능 추가 2010.09.28 by warship
	m_pFold = CreateImgButton("<<", IDC_BTN_FOLD1, m_hBtn2);
	m_pStJcnt = new CfxStatic(m_pFont);
	m_pStJcnt->Create("수량", WS_CHILD|WS_VISIBLE, rc, this);
	m_pStJcnt->SetBkColor( m_crBodyBk );
	m_pSpJcnt = new CfxSpin(m_pFont);
	m_pSpJcnt->Create(attrNUMERIC|attrCOMMA, spNOT, rc, this, -1);
	m_pSpJcnt->SetRangeEx(0, 5000);
	// ------------------------------------

	if (m_stGuide.Create(" ", WS_CHILD|WS_VISIBLE, rc, this, IDC_GUIDECTRL))
		m_stGuide.SetFont(m_pFont);

	m_pCode->Create(NULL, "CODE", WS_CHILD|WS_VISIBLE|WS_TABSTOP, CRect(0, 0, CODECTRL_WIDTH, 20), this, -1);
	m_pSearch = CreateImgButton("", IDC_BTN_SEARCH, m_hSearch);
	
	m_btn.Create("테스트", WS_CHILD|WS_VISIBLE|WS_TABSTOP, rc, this, IDC_TEST_BTN);

	LoadConfig();

	if (m_AcntMode==AM_GROUP) 
	{
		req_SDLDQ003_grp();
		m_pControl->SendMessage(WM_APP_REFRESH, WP_CTRL2_FOLD_DISABLE, (LPARAM)FALSE);
	}
	else if (m_AcntMode==AM_ACCOUNT)
	{
		m_pControl->SendMessage(WM_APP_REFRESH, WP_CTRL2_FOLD_DISABLE, (LPARAM)TRUE);
	}
	
	ChangeCode( m_pCode->GetCode() );
	return 0;
}

void CMapWnd::OnClose() 
{
	CWnd::OnClose();
}

void CMapWnd::OnDestroy() 
{
	SaveConfig();

	if (m_pHogaWnd) { m_pHogaWnd->DestroyWindow(); delete m_pHogaWnd; }
	if (m_pAcno) { m_pAcno->DestroyWindow(); delete m_pAcno; }
	if (m_pSise) { m_pSise->DestroyWindow(); delete m_pSise; }
	if (m_pControl) { m_pControl->DestroyWindow(); delete m_pControl; }
	if (m_pControl2) { m_pControl2->DestroyWindow(); delete m_pControl2; }
	if (m_pCode) { m_pCode->DestroyWindow(); delete m_pCode; }
	if (m_pGroup) { m_pGroup->DestroyWindow(); delete m_pGroup; }
	if (m_pGrpEdit) { m_pGrpEdit->DestroyWindow(); delete m_pGrpEdit; }
	if (m_pPswd) { m_pPswd->DestroyWindow(); delete m_pPswd; }
	if (m_pSend) { m_pSend->DestroyWindow(); delete m_pSend; }
	if (m_pConf) { m_pConf->DestroyWindow(); delete m_pConf; }
	if (m_pStAcnm) { m_pStAcnm->DestroyWindow(); delete m_pStAcnm; }
	if (m_pStCode) { m_pStCode->DestroyWindow(); delete m_pStCode; }
	if (m_pStAccn) { m_pStAccn->DestroyWindow(); delete m_pStAccn; }
	if (m_pSearch) { m_pSearch->DestroyWindow(); delete m_pSearch; }
	//if (m_tooltip.GetSafeHwnd()) m_tooltip.DestroyWindow();
	if (m_pLimit) { m_pLimit->DestroyWindow(); delete m_pLimit; }
	if (m_pJngoTick) {  m_pJngoTick->DestroyWindow(); delete m_pJngoTick; }
	if (m_pFold) { m_pFold->DestroyWindow(); delete m_pFold; }
	if (m_pStJcnt) { m_pStJcnt->DestroyWindow(); delete m_pStJcnt; }
	if (m_pSpJcnt) { m_pSpJcnt->DestroyWindow(); delete m_pSpJcnt; }

	CWnd::OnDestroy();
}

void CMapWnd::SetSize( int width, int height )
{
	SetWindowPos(NULL, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
	m_pParent->PostMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(width, height));
}

void CMapWnd::CalcSize()
{
	CRect rc, rcTemp;
	GetClientRect(&rc);

	int total_width, total_height, hoga_width;
	int cx = rc.Width(), cy = rc.Height(), btn_h = 20, pos_x, pos_y;
	
	total_height = rc.Height();
	hoga_width = m_pHogaWnd->GetRequireWidth() + (PADDING_VALUE*2);

	total_width = hoga_width;
	if (!m_bFoldCtrl1) 
	{
		total_width = hoga_width + CTRL_WND_WIDTH;
		if (!m_bFoldCtrl2 && (m_AcntMode==AM_GROUP))
			total_width += CTRL2_WND_WIDTH;
	}

// 	CString emsg;
// 	emsg.Format("%d / %d / %d\n", cx, total_width, m_pHogaWnd->GetRequireWidth());
// 	OutputDebugString(emsg);

// 	if (rc.Width() != total_width)
// 	{
// 		SetWindowPos(NULL, 0, 0, total_width, total_height, SWP_NOZORDER | SWP_NOMOVE);
// 		m_pParent->PostMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(total_width, total_height));
// 		return;
// 	}

	// 각 윈도우 영역별 Rect 설정
	//m_rcCtrl.SetRect(0, 0, hoga_width, ACCN_WND_HEIGHT);
	int rcHeight = GROUP_WND_HEIGHT;
	if (m_AcntMode==AM_ACCOUNT)
		rcHeight = ACCN_WND_HEIGHT;
	m_rcAccn.SetRect(0, 0, hoga_width, rcHeight);

	pos_y = m_rcAccn.top + PADDING_VALUE + (btn_h*2) + PADDING_VALUE/2;
	m_rcSise.SetRect(0, pos_y, hoga_width, pos_y + SISE_WND_HEIGHT);


	m_rcHoga.SetRect(0, rcHeight, hoga_width, cy-GUID_WND_HEIGHT);
	m_rcCtrl.SetRect(hoga_width, 0, hoga_width+CTRL_WND_WIDTH, cy-GUID_WND_HEIGHT);
	m_rcCtrl2.SetRect(m_rcCtrl.right, 0, total_width, cy-GUID_WND_HEIGHT);
	m_rcGuide.SetRect(0, cy-GUID_WND_HEIGHT, total_width, cy);
	
	// 계좌 윈도우
	{
		CRect rc(m_rcAccn);
		rc.DeflateRect(CSize(PADDING_VALUE, 0));

		pos_x = rc.left;
		pos_y = rc.top + PADDING_VALUE;
		m_pStAccn->MoveWindow(pos_x, pos_y, 40, btn_h, FALSE);

		if (m_AcntMode==AM_GROUP)
		{
			m_pStAccn->SetText("그 룹");
			m_pGroup->ShowWindow(SW_SHOW);
			m_pGrpEdit->ShowWindow(SW_SHOW);
			m_pAcno->ShowWindow(SW_HIDE);
			m_pStAcnm->ShowWindow(SW_HIDE);
			m_pJngoTick->ShowWindow(SW_HIDE);
			m_pControl2->ShowWindow(SW_SHOW);
			m_pFold->ShowWindow(SW_SHOW);

			m_pGroup->MoveWindow(pos_x + 40, pos_y, 140, 100, FALSE); 
			m_pPswd->MoveWindow(pos_x+ 185, pos_y, 50, btn_h, FALSE);
			m_pGrpEdit->MoveWindow(pos_x + 240, pos_y, 40, btn_h, FALSE);
		}
		else if (m_AcntMode==AM_ACCOUNT)
		{
			m_pStAccn->SetText("계 좌");
			m_pGroup->ShowWindow(SW_HIDE);
			m_pGrpEdit->ShowWindow(SW_HIDE);
			m_pAcno->ShowWindow(SW_SHOW);
			m_pStAcnm->ShowWindow(SW_SHOW);
			m_pJngoTick->ShowWindow(SW_SHOW);
			m_pControl2->ShowWindow(SW_HIDE);
			m_pFold->ShowWindow(SW_SHOW);

			m_pAcno->MoveWindow(pos_x + 40, pos_y, 140, btn_h, FALSE);
			m_pPswd->MoveWindow(pos_x+ 185, pos_y, 50, btn_h, FALSE);
			m_pStAcnm->MoveWindow(pos_x+240, pos_y, rc.Width()-240, btn_h, FALSE);
		}

		if ((m_bShowBefore || m_bShowRate) && m_bShowRsvd)
		{
			m_pSend->MoveWindow(rc.right-122, pos_y, 40, btn_h, FALSE);
			m_pConf->MoveWindow(rc.right-81, pos_y, 40, btn_h, FALSE);
			m_pLimit->MoveWindow(rc.right-40, pos_y, 40, btn_h, FALSE);
			m_pLimit->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pSend->MoveWindow(rc.right-81, pos_y, 40, btn_h, FALSE);
			m_pConf->MoveWindow(rc.right-40, pos_y, 40, btn_h, FALSE);
			m_pLimit->ShowWindow(SW_HIDE);
		}

		// Line 2
		pos_x = rc.left; 
		pos_y += btn_h + (PADDING_VALUE/2);

		m_pStCode->MoveWindow(pos_x, pos_y, 40, btn_h, FALSE);	pos_x += 40;
		m_pCode->MoveWindow(pos_x, pos_y, CODECTRL_WIDTH, btn_h, FALSE); pos_x += CODECTRL_WIDTH;
		m_pSearch->MoveWindow(pos_x, pos_y, 19, btn_h, FALSE); pos_x += 20 + PADDING_VALUE/2;

		//2017.01.24 KSJ 150 -> 148
		m_pStCodeNm->MoveWindow(pos_x, pos_y, 148, btn_h, FALSE);
		rcTemp.left = pos_x;

		if (m_AcntMode==AM_ACCOUNT)
		{
// 			emsg.Format("WIDTH = %d\n", rc.Width());
// 			OutputDebugString(emsg);

			pos_x = rc.right-20;
			m_pFold->MoveWindow(pos_x, pos_y, 20, btn_h, FALSE); 
			
			if (m_bFoldCtrl1)
			{
				if (rc.Width()>=487)
				{
					pos_x -= 30+1; 
					m_pLimit->MoveWindow(pos_x, pos_y, 30, btn_h, FALSE);
					m_pLimit->ShowWindow(SW_SHOW);
				}
				else
				{
					m_pLimit->ShowWindow(SW_HIDE);
				}
				//m_pStCodeNm->ShowWindow( (rc.Width()>=447) ? SW_SHOW : SW_HIDE );
				
			}
			else
			{
				pos_x -= 30+1; 
				m_pLimit->MoveWindow(pos_x, pos_y, 30, btn_h, FALSE);
				m_pLimit->ShowWindow(SW_SHOW);
				m_pStCodeNm->ShowWindow(SW_SHOW);
			}
			
			pos_x -= 30+1;
			m_pConf->MoveWindow(pos_x, pos_y, 30, btn_h, FALSE);
			
			pos_x -= 30+1;
			m_pSend->MoveWindow(pos_x, pos_y, 30, btn_h, FALSE);

			if( m_bFoldCtrl1 )
			{
				pos_x -= 61;
				m_pSpJcnt->MoveWindow(pos_x, pos_y, 60, btn_h, FALSE);

				pos_x -= 31;
				m_pStJcnt->MoveWindow(pos_x, pos_y, 30, btn_h, FALSE);

				//2017.01.24 KSJ 사이즈 조절
				rcTemp.right = pos_x;
				rcTemp.top = pos_y;
				rcTemp.bottom = pos_y + btn_h;
				
				m_pStCodeNm->MoveWindow(rcTemp, FALSE);

/*
				CRect tmpRc;
				m_pStCodeNm->GetClientRect(&tmpRc);
				if (tmpRc.right > pos_x)
				{
					tmpRc.right = pos_x - 1;
					m_pStCodeNm->MoveWindow(&tmpRc);
				}
*/
			}
		}
		else
		{
			m_pFold->MoveWindow(rc.right-20, pos_y, 20, btn_h, FALSE);
			if( m_bFoldCtrl1 )
			{
				m_pSpJcnt->MoveWindow(rc.right-85, pos_y, 60, btn_h, FALSE);
				if ((pos_x+92)>(rc.right-145))
				{
					m_pStJcnt->SetText("수량");
					m_pStJcnt->MoveWindow(rc.right-115, pos_y, 30, btn_h, FALSE);
				}
				else
				{
					m_pStJcnt->SetText("주문수량");
					m_pStJcnt->MoveWindow(rc.right-145, pos_y, 60, btn_h, FALSE);
				}
			}
		}
	
		m_pSpJcnt->ShowWindow( m_bFoldCtrl1 ? SW_SHOW : SW_HIDE);
		m_pStJcnt->ShowWindow( m_bFoldCtrl1 ? SW_SHOW : SW_HIDE);
		m_pFold->SetWindowText((m_bFoldCtrl1) ? ">>" : "<<");
		m_pControl->PostMessage(WM_APP_REFRESH, WP_FOLDSTAT, (LPARAM)m_bFoldCtrl2);
	}

	// 시세 윈도우
	{
		CRect rc(m_rcSise);
		rc.DeflateRect(CSize(PADDING_VALUE, 0));
		rc.bottom -= PADDING_VALUE;
		rc.top += PADDING_VALUE/2;
		m_pSise->MoveWindow(rc, FALSE);
	}

	// 잔고 티커 ( 계좌모드일 경우에만 )
	if (m_AcntMode==AM_ACCOUNT)
	{
		CRect rc(m_rcSise);
		rc.OffsetRect(CSize(0, m_rcSise.Height()));
		rc.top -= PADDING_VALUE/2;
		rc.bottom = rc.top + GRID_CELL_HEIGHT*2;
		rc.DeflateRect(CSize(PADDING_VALUE, 0));
		m_pJngoTick->MoveWindow(rc, FALSE);
	}

	// 그리드윈도우 위치 지정
	if (m_pHogaWnd) {
		if (m_pHogaWnd->GetSafeHwnd()) {
			CRect rc = m_rcHoga;
			rc.DeflateRect(CSize(PADDING_VALUE, PADDING_VALUE));
			m_pHogaWnd->MoveWindow(rc.left, rc.top, rc.Width(), rc.Height(), FALSE);
		}
	}

	// 컨트롤윈도우 위치 지정
	{
		CRect rc(m_rcCtrl);
		rc.DeflateRect(CSize(PADDING_VALUE,PADDING_VALUE));
		m_pControl->MoveWindow(&rc, TRUE);
	}

	// 컨트롤2윈도우 위치 지정
	if (m_AcntMode==AM_GROUP)
	{
		CRect rc(m_rcCtrl2);
		rc.DeflateRect(CSize(PADDING_VALUE,PADDING_VALUE));
		m_pControl2->MoveWindow(&rc, TRUE);
	}

	// 가이드 윈도우 위치 지정
	{
		CRect rc(m_rcGuide);
		rc.DeflateRect(CSize(25, 5));
		m_stGuide.MoveWindow(&rc, TRUE);
	}

	MakeBackground(total_width, cy);

// 	emsg.Format("[LAST]	%d / %d / %d\n", cx, total_width, m_pHogaWnd->GetRequireWidth());
// 	OutputDebugString(emsg);

	if (cx!=total_width)
	{
		SetSize(total_width, cy);
		m_nWidth = total_width;
		m_nHeight = cy;
		return;
	}
}

void CMapWnd::req_pibo3002(LPCSTR code)
{
	if (strlen(code)!=FCODE_LEN) return;

	m_SiseData.code = code;
	m_SiseData.type = GetCodeType(code);
	SendTR("PIHO3002", 0, m_SiseData.code, 8, TK_PIBO3002);
}

void CMapWnd::res_pibo3002( LPCSTR data, int size )
{
	if (size!=sizeof(pibo3002_mod)) return;

// 	CString tmp;
// 	tmp = CString(data,size);

	CString s;
// 	s.Format("PIBO3002 : [%s] [%d]\n",tmp,size);
// 	OutputDebugString(s);

	pibo3002_mod *mod = (pibo3002_mod *)data;

	m_SiseData.fhoga.clear();
	m_SiseData.hhga = (int)(fabs(Str2Double(mod->hhga, sizeof(mod->hhga))*100) + DOUBLE_PREFIX);
	if (m_SiseData.code.GetAt(0)=='4' || m_SiseData.code.GetAt(0) == 'D') //파생상품 코드개편
	{
// 		OutputDebugString("[KSJ] IN");
		// 하한가는 반대부호로 오게 되어 있다(색상) 고로 스프레드의 경우 다시 역방향으로 처리해주는 센스가 필요하다.
		double hhga = Str2Double(mod->hhga, sizeof(mod->hhga))*100 + DOUBLE_PREFIX;
		//hhga *= -1.0;
		m_SiseData.hhga = (int)(hhga);
	}

	if(m_SiseData.code.GetAt(0)!='4' && m_SiseData.code.GetAt(0) != 'D')  //파생상품 코드개편
	{
		m_SiseData.shga = (int)(fabs(Str2Double(mod->shga, sizeof(mod->shga))*100) + DOUBLE_PREFIX);
		m_SiseData.gjga = (int)(fabs(Str2Double(mod->gjga, sizeof(mod->gjga))*100) + DOUBLE_PREFIX);
		m_SiseData.siga = (int)(fabs(Str2Double(mod->siga, sizeof(mod->siga))*100) + DOUBLE_PREFIX);
		m_SiseData.koga = (int)(fabs(Str2Double(mod->koga, sizeof(mod->koga))*100) + DOUBLE_PREFIX);
		m_SiseData.jega = (int)(fabs(Str2Double(mod->jega, sizeof(mod->jega))*100) + DOUBLE_PREFIX);
		m_SiseData.curr = (int)(fabs(Str2Double(mod->curr, sizeof(mod->curr))*100) + DOUBLE_PREFIX);
	}
	else
	{
		if(Str2Double(mod->shga, sizeof(mod->shga))*100 >= 0)
			m_SiseData.shga = (int)(fabs(Str2Double(mod->shga, sizeof(mod->shga))*100) + DOUBLE_PREFIX);
		else
			m_SiseData.shga = (int)(Str2Double(mod->shga, sizeof(mod->shga))*100 - DOUBLE_PREFIX);
		
		double test = Str2Double(mod->gjga, sizeof(mod->gjga))*100;
		if(Str2Double(mod->gjga, sizeof(mod->gjga))*100 >= 0)
			m_SiseData.gjga = (int)(fabs(Str2Double(mod->gjga, sizeof(mod->gjga))*100) + DOUBLE_PREFIX);
		else
			m_SiseData.gjga = (int)(Str2Double(mod->gjga, sizeof(mod->gjga))*100 - DOUBLE_PREFIX);
		
		if(Str2Double(mod->siga, sizeof(mod->siga))*100 >= 0)
			m_SiseData.siga = (int)(fabs(Str2Double(mod->siga, sizeof(mod->siga))*100) + DOUBLE_PREFIX);
		else
			m_SiseData.siga = (int)(Str2Double(mod->siga, sizeof(mod->siga))*100 - DOUBLE_PREFIX);
		
		if(Str2Double(mod->koga, sizeof(mod->koga))*100 >= 0)
			m_SiseData.koga = (int)(fabs(Str2Double(mod->koga, sizeof(mod->koga))*100) + DOUBLE_PREFIX);
		else
			m_SiseData.koga = (int)(Str2Double(mod->koga, sizeof(mod->koga))*100 - DOUBLE_PREFIX);
		
		if(Str2Double(mod->jega, sizeof(mod->jega))*100 >= 0)
			m_SiseData.jega = (int)(fabs(Str2Double(mod->jega, sizeof(mod->jega))*100) + DOUBLE_PREFIX);
		else
			m_SiseData.jega = (int)(Str2Double(mod->jega, sizeof(mod->jega))*100 - DOUBLE_PREFIX);
		
		if(Str2Double(mod->curr, sizeof(mod->curr))*100 >= 0)
			m_SiseData.curr = (int)(fabs(Str2Double(mod->curr, sizeof(mod->curr))*100) + DOUBLE_PREFIX);
		else
			m_SiseData.curr = (int)(Str2Double(mod->curr, sizeof(mod->curr))*100 - DOUBLE_PREFIX);
	}
	
	//2016.11.14 KSJ 조회할때 초기값 세팅을 해준다.
	m_SiseData.pcurr = FIRST_PCURR;

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
		if(Str2Double(mod->hoga[n].msga, sizeof(mod->hoga[n].msga))*100 < 0)
		{
			m_SiseData.hoga[n].msga = (int)(fabs(Str2Double(mod->hoga[n].msga, sizeof(mod->hoga[n].msga))*100) + DOUBLE_PREFIX);

			m_SiseData.hoga[n].msga = m_SiseData.hoga[n].msga * -1;
		}
		else
		{
			m_SiseData.hoga[n].msga = (int)(Str2Double(mod->hoga[n].msga, sizeof(mod->hoga[n].msga))*100 + DOUBLE_PREFIX);
		}

		if(Str2Double(mod->hoga[n].mdga, sizeof(mod->hoga[n].mdga))*100 < 0)
		{
			m_SiseData.hoga[n].mdga = (int)(fabs(Str2Double(mod->hoga[n].mdga, sizeof(mod->hoga[n].mdga))*100) + DOUBLE_PREFIX);
			
			m_SiseData.hoga[n].mdga = m_SiseData.hoga[n].mdga * -1;
		}
		else
		{
			m_SiseData.hoga[n].mdga = (int)(Str2Double(mod->hoga[n].mdga, sizeof(mod->hoga[n].mdga))*100 + DOUBLE_PREFIX);
		}

		m_SiseData.hoga[n].dvol = Str2Int(mod->hoga[n].dvol, sizeof(mod->hoga[n].dvol));
		m_SiseData.hoga[n].svol = Str2Int(mod->hoga[n].svol, sizeof(mod->hoga[n].svol));
		m_SiseData.hoga[n].dcnt = Str2Int(mod->hoga[n].dcnt, sizeof(mod->hoga[n].dcnt));
		m_SiseData.hoga[n].scnt = Str2Int(mod->hoga[n].scnt, sizeof(mod->hoga[n].scnt));

// 		s.Format("[%d] [%d] [%d] [%d] [%d] [%d]\n",m_SiseData.hoga[n].msga,m_SiseData.hoga[n].mdga,m_SiseData.hoga[n].dvol,m_SiseData.hoga[n].svol,m_SiseData.hoga[n].dcnt,m_SiseData.hoga[n].scnt);
// 		OutputDebugString(s);
	}

	int price;
	m_SiseData.fhoga.push_back(m_SiseData.shga);
	do {
		price = GetNextPrice(m_SiseData.fhoga.back(), m_SiseData.type, false);
		m_SiseData.fhoga.push_back( price );	
	} while( price > m_SiseData.hhga );

// 	CString strTemp;
// 	strTemp.Format("[KSJ] siga[%s] koga[%s] jega[%s] shga[%d] hhga[%d]",CString(mod->siga, sizeof(mod->siga)), CString(mod->koga, sizeof(mod->koga)), CString(mod->jega, sizeof(mod->jega)), m_SiseData.shga, m_SiseData.hhga);
// 	OutputDebugString(strTemp);

	m_pHogaWnd->SetData(&m_SiseData);
	m_pSise->SetData(&m_SiseData);

	// 체결탭
	RealData crp;
	m_pControl->ClearChegGrid();
	vector<CString> c_curr, c_time, c_cvol;
	int n = 0;
	for(n=0; n<MAX_FC; ++n)
	{
		if (mod->cheg[n].time[0]<'0' || mod->cheg[n].time[0]>'9') break;
		
		crp[34] = StringProxy(&mod->cheg[n].time[0], &mod->cheg[n].time[0] + sizeof(mod->cheg[n].time));	// ChegTime Symbol [34]
		crp[23] = StringProxy(&mod->cheg[n].curr[0], &mod->cheg[n].curr[0] + sizeof(mod->cheg[n].curr));	// ChegCurr Symbol [23]
		crp[32] = StringProxy(&mod->cheg[n].cvol[0], &mod->cheg[n].cvol[0] + sizeof(mod->cheg[n].cvol));	// ChegVolu Symbol [32]
		m_pControl->SetRealData(&crp);
	}
	
	if (m_AcntMode==AM_GROUP) 
	{
		m_pControl2->SetJvolPrice(m_SiseData.curr);
		m_pControl2->SendMessage(WM_APP_REFRESH, WP_JANGO, NULL);
		m_pControl2->SendMessage(WM_APP_REFRESH, WP_MICHEG, (LPARAM)(LPCSTR)m_SiseData.code);
	}
	else if (m_AcntMode==AM_ACCOUNT)
	{
		m_pControl->SetJvolPrice(m_SiseData.curr);
		AccountMap::iterator apos = m_AcntData.find( (LPCSTR)m_pAcno->GetNumber() );
		//OutputDebugString("ACCOUNT DATA FIND==========================================>\n");
		if (apos!=m_AcntData.end())
		{
			//OutputDebugString("ACCOUNT DATA OK==========================================>\n");
			CodeJangoMap::iterator cpos = apos->second.jango.find( (LPCSTR)m_SiseData.code );

			if (cpos!=apos->second.jango.end())
			{
				//OutputDebugString("ACCOUNT DATA CODE OK==========================================>\n");
				m_pJngoTick->SendMessage(WM_APP_REFRESH, WP_JANGO, (LPARAM)&cpos->second);
			}
			else
			{
				m_pJngoTick->Clear();
			}
		}
	}

	// 2011.01.14 코드가 변경되지 않으면 예약주문데이터를 초기화 하지 않으므로, 그리드 갱신이 필요함.
	if (m_RsvdCode==m_SiseData.code)
		RefreshRsvd();

	m_RsvdCode = m_SiseData.code;
	calc_micheg(NULL);
	
	//m_pHogaWnd->SetFocusGrid();
}

void CMapWnd::proc_alert( LPCSTR data, int size )
{
	int sym, pack_cnt, n;
	CString key;
	LPCSTR st = data;
	LPCSTR ed = st + size;
	LPCSTR pos;
	vector<StringProxy> lines;

	// Key check
	pos = find(st, ed, '\t');
	key = CString(st, pos-st);
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
		RealData::iterator pCurr = rdat.find(23);
		if (pCurr!=rdat.end())
		{
			m_SiseData.pcurr = m_SiseData.curr;
			m_SiseData.curr = (int)(fabs(Str2Double( pCurr->second ) * 100.0) + DOUBLE_PREFIX);
			if (m_SiseData.pcurr!=m_SiseData.curr)
				CheckRsvd();
		}

		m_pSise->SetRealData(&rdat);
		m_pHogaWnd->SetRealData(&rdat);
		m_pControl->SetRealData(&rdat);
	}
}

void CMapWnd::proc_alert( struct _alertR* alertR )
{
	if (alertR->code!=m_SiseData.code) return;

	DWORD* data;
	
	for(int i=alertR->size-1;i>-1;--i)
	{
		data = (DWORD*)alertR->ptr[i];

		if (data[23])
		{
// 			CString s;
// 			s.Format("ALERT CURR : [%s]\n",(char*)data[23]);
// 			OutputDebugString(s);

			m_SiseData.pcurr = m_SiseData.curr;
			m_SiseData.curr = (int)(fabs(atof((char*)data[23]) * 100.0) + DOUBLE_PREFIX);
			if (m_SiseData.pcurr!=m_SiseData.curr)
				CheckRsvd();

// 			s.Format("SISE CURR : [%d]\n",m_SiseData.curr);
// 			OutputDebugString(s);
		}
		
		m_pSise->SetRealData(data);
		m_pHogaWnd->SetRealData(data);
		m_pControl->SetRealData(data);
	}
}

CODE_TYPE CMapWnd::GetCodeType( LPCSTR code )
{
	CString strCode;
	strCode.Format("%s",code);
	
	if(strCode.GetLength() < 8)
		return CT_NONE;
	
	CString type = strCode.Mid(1,2);
	if (strCode.Left(3) == "170" || strCode.Left(3) == "470" || strCode.Left(3) == "A70" || strCode.Left(3) == "D70")
	{
		return CT_30GUMRI;
	}
	else if(atoi(type) >= 60 && atoi(type) <= 74)
	{
		return CT_GUMRI;
	}
	else if(atoi(type) >= 75 && atoi(type) <= 84)
	{
		if(atoi(type) == 78)	return CT_YUAN;	//2015.11.20 KSJ 위안화 추가
		else					return CT_MONEY;
	}

	return CT_NONE;

// 	switch(*code)
// 	{
// 	case '1': return CT_FUTURE;
// 	case '2':
// 	case '3': return CT_OPTION;
// 	case '4': return CT_SPREAD;
// 	default : return CT_NONE;
// 	}
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
	int nrec = Str2Int(mod->nrec, sizeof(mod->nrec));

	string acno = string(mod->accn, sizeof(mod->accn));
	AccountMap::iterator pid = m_AcntData.find( acno );
	if (pid==m_AcntData.end())	// 모르는 계좌면 나가야지~
		return;

	string code;
	Account *pAcnt = &(pid->second);
	for(int n=0; n<nrec; ++n)
	{
		pibofchg_grid *grid = &mod->grid[n];

		code = string(grid->cod2, sizeof(grid->cod2));
		
		int juno = Str2Int(grid->juno, sizeof(grid->juno) );
		int ojno = Str2Int(grid->ojno, sizeof(grid->ojno) );
		int oprc = (int)(fabs(Str2Double(grid->oprc, sizeof(grid->oprc))*100.0) + DOUBLE_PREFIX);
		int oqty = Str2Int(grid->oqty, sizeof(grid->oqty) );
		int dprc = (int)(fabs(Str2Double(grid->dprc, sizeof(grid->dprc))*100.0) + DOUBLE_PREFIX);
		int dqty = Str2Int(grid->dqty, sizeof(grid->dqty) );
		int wqty = Str2Int(grid->wqty, sizeof(grid->wqty) );
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

		//TRACE("\nMICHEG[%02d] accn(%s) code(%s) juno(%d) ojno(%d) oprc(%d) oqty(%d) dprc(%d) dqty(%d) wqty(%d) stat(%s)",
		//	n+1, acno.c_str(), code.c_str(), juno, ojno, oprc, oqty, dprc, dqty, wqty, (LPCSTR)stat);

	}

	if (ledger->next[0]=='Y') 
	{
		req_pibofchg(acno.c_str(), GetPassword(acno.c_str()), TRUE, ledger->nkey);
	} else 
	{
		calc_micheg(acno.c_str());
		if (m_AcntMode==AM_GROUP)
			m_pControl2->SendMessage(WM_APP_REFRESH, WP_MICHEG, (LPARAM)(LPCSTR)m_SiseData.code);
	}
}


void CMapWnd::OnTest()
{
	ClearAccount();
	AddAccount("00120000035", "0423");
	AddAccount("00120000026", "0423");
	req_SDLDQ003_grp();
}

LPCSTR CMapWnd::GetPassword( LPCSTR acno )
{
	AccountMap::iterator pos = m_AcntData.find(acno);
	if (pos==m_AcntData.end()) return "";
	else return (LPCSTR)pos->second.pswd;
}

void CMapWnd::proc_notice( LPCSTR data, int size )
{
	int sym;
	CString val;
	RealData rdat;
	LPCSTR st, ed, p;
	st = data;
	ed = data+size;
	
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
	int juno = Str2Int(rdat[904]);
	int ojno = Str2Int(rdat[905]);
	int oqty = Str2Int(rdat[909]);
	int oprc = Str2Int(rdat[910]);
	int dqty = Str2Int(rdat[931]);
	int dprc = Str2Int(rdat[916]);
	int wqty = Str2Int(rdat[921]);
	int dtcd = Str2Int(rdat[996]);
	int csym = Str2Int(rdat[993]);
	CString dsgb = rdat[912].ToCStr();

	WriteLog("RDDATA data_cd(%2d) dsgb(%s) acno(%s) code(%s) juno(%d) ojno(%d) oprc(%d) oqty(%d) dprc(%d) dqty(%d) wqty(%d) csym(%d)",
			dtcd, (LPCSTR)dsgb, acno.c_str(), code.c_str(), juno, ojno, oprc, oqty, dprc, dqty, wqty,csym);

	CString s;
// 	s.Format("RDDATA data_cd(%2d) dsgb(%s) acno(%s) code(%s) juno(%d) ojno(%d) oprc(%d) oqty(%d) dprc(%d) dqty(%d) wqty(%d) csym(%d)",
// 			dtcd, (LPCSTR)dsgb, acno.c_str(), code.c_str(), juno, ojno, oprc, oqty, dprc, dqty, wqty,csym);
//	OutputDebugString(s+"\n");

	AccountMap::iterator pid = m_AcntData.find( acno );
	if (pid==m_AcntData.end()) return;

	Order *pOdr;
	Account *pAcnt = &(pid->second);
	OrderMap *pOdrMap = &(pAcnt->order[code]);
	OrderMap::iterator pos, end = pOdrMap->end();
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

		if (m_pControl2 && m_AcntMode==AM_GROUP)
			m_pControl2->SendMessage(WM_APP_REFRESH, WP_MICHEG_INSERT, (LPARAM)pOdr);
		
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
				if (m_pControl2 && m_AcntMode==AM_GROUP)
					m_pControl2->SendMessage(WM_APP_REFRESH, WP_MICHEG_DELETE, (LPARAM)&pos->second);
				pOdrMap->erase(pos);
			}
		} else 
		{
			// 남아있으면 미체결수량만 변경~
			if (pos != end)
			{
				pos->second.wqty = wqty;
				if (m_pControl2 && m_AcntMode==AM_GROUP)
					m_pControl2->SendMessage(WM_APP_REFRESH, WP_MICHEG_UPDATE, (LPARAM)&pos->second);
			}
		}
		if (code[0]=='4' || code[0] == 'D')  //파생상품 코드개편
		{
			if (m_pControl)
			{
				m_pControl->Clear();
				NotifyMap::iterator pos = m_NotifyMap.find(acno);
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
				if (m_pControl2 && m_AcntMode==AM_GROUP)
					m_pControl2->SendMessage(WM_APP_REFRESH, WP_MICHEG_DELETE, (LPARAM)&pos->second);
				pOdrMap->erase(pos);
			}
			else
			{
				if (m_pControl2 && m_AcntMode==AM_GROUP)
					m_pControl2->SendMessage(WM_APP_REFRESH, WP_MICHEG_UPDATE, (LPARAM)&pos->second);
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
		if (m_pControl2 && m_AcntMode==AM_GROUP)
			m_pControl2->SendMessage(WM_APP_REFRESH, WP_MICHEG_INSERT, (LPARAM)pOdr);
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
			if (m_pControl2 && m_AcntMode==AM_GROUP)
				m_pControl2->SendMessage(WM_APP_REFRESH, WP_MICHEG_DELETE, (LPARAM)&pos->second);
			pOdrMap->erase(pos);
		}
		else
		{
			if (m_pControl2 && m_AcntMode==AM_GROUP)
				m_pControl2->SendMessage(WM_APP_REFRESH, WP_MICHEG_UPDATE, (LPARAM)&pos->second);
		}
		break;
	case 14: case 15: case 16:	// 거부
		if (ojno>0)
			pos = pOdrMap->find(ojno);
		else
		{
			pos = pOdrMap->find(juno);		// IOC/FOK일 경우 주문번호로 취소됨

			if(dtcd == 14)	pos->second.wqty -= oqty;	//2015.09.23 warship 신규주문에 대해서만 미체결 차감 처리한다.
		}
		if (pos==end) break;
				
		if (pos->second.wqty<=0) 
		{
			if (m_pControl2 && m_AcntMode==AM_GROUP)
				m_pControl2->SendMessage(WM_APP_REFRESH, WP_MICHEG_DELETE, (LPARAM)&pos->second);
			pOdrMap->erase(pos);
		}
		else
		{
			if (m_pControl2 && m_AcntMode==AM_GROUP)
				m_pControl2->SendMessage(WM_APP_REFRESH, WP_MICHEG_UPDATE, (LPARAM)&pos->second);
		}
		break;
	default:
		break;
	}
	_calc_micheg(pAcnt, code);
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
			PriceMichegMap::iterator ed = pmm.end();
			for(; st!=ed; ++st)
				wqty += st->second.d_wqty;
			j->xqty = max(j->jqty-wqty, 0);
		}
		else if (strcmp(j->gubn, "-매도")==0)
		{
			PriceMichegMap &pmm = m_AcntData[j->acno].micheg[j->code];
			PriceMichegMap::iterator st = pmm.begin();
			PriceMichegMap::iterator ed = pmm.end();
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
	if (bSend)
		m_pControl->SendMessage(WM_APP_REFRESH, WP_JANGO, (LPARAM)j);

	/* m_pJngoTick 및 m_pControl의 경우 현재종목이 아닌 잔고에 대해서는 갱신할 필요가 없다 */
	if (m_SiseData.code!=j->code) return;

	if (m_AcntMode==AM_GROUP)
		m_pControl2->SendMessage(WM_APP_REFRESH, WP_JANGO, (LPARAM)j);

	if (m_AcntMode==AM_ACCOUNT)
		m_pJngoTick->SendMessage(WM_APP_REFRESH, WP_JANGO, (LPARAM)j);

	// 2012.08.08 김덕기 - 평균가 표시 기능 추가
// 	if (strcmp(j->gubn, "+매수")==0)
// 	{
// 		int nTmp = j->pprc * 100;
// 
// 		if(nTmp)
// 	}
// 	vector<int>::iterator st = m_SiseData.fhoga.begin();
// 	vector<int>::iterator ed = m_SiseData.fhoga.end();
// 	for(int row=1; st!=ed; ++st, ++row)
// 	{
// 		
// 	}
// 
// 	m_pHogaWnd->AddArrow();
}

LRESULT CMapWnd::OnAppSignal( WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case WP_JNGO_CHANGE:
		proc_jango((Jango*)lParam);
		break;
	case WP_CODE_CHANGE:
		ChangeCode((LPCSTR)lParam);
		break;
	case WP_ACCN_CHANGE:
		ChangeAccount((LPCSTR)lParam);
		break;
	case WP_MADO_ALL_CANCEL:
		WriteLog("AppSignal - 매도주문 모두 취소");
		if (chck_account()) cancel_order_all(1);
		break;
	case WP_MASU_ALL_CANCEL:
		WriteLog("AppSignal - 매수주문 모두 취소");
		if (chck_account()) cancel_order_all(2);
		break;
	case WP_MADO_RSVD_ALL_CANCEL:
		WriteLog("AppSignal - 예약매도주문 모두 취소");
		if (chck_account()) cancel_rsvd_all(1);
		break;
	case WP_MASU_RSVD_ALL_CANCEL:
		WriteLog("AppSignal - 예약매수주문 모두 취소");
		if (chck_account()) cancel_rsvd_all(2);
		break;
	case WP_ORDER:
		{
			WriteLog("AppSignal - 주문");
			Order* ord = (Order*)lParam;

			if(m_utype == 0)
			{
				if(m_AcntMode==AM_GROUP && m_otpf == 0)
				{
					CAccountConfirmWnd accDlg(m_pParent,&m_AcntData);
					
					int ret = accDlg.DoModal();
					
					if(ret == IDCANCEL)
					{
						return 0;
					}
				}
			}
			if (chck_account()) click_order((Order*)lParam);
		}
		break;
	case WP_RSVD_REALORDER:
		if (chck_account()) click_order((Order*)lParam);
		break;
	case WP_RSVD_ORDER:
		WriteLog("AppSignal - 예약주문");
		if (chck_account()) rsvd_order((ReserveData*)lParam);
		break;
	case WP_MODIFY_ORDER:
		WriteLog("AppSignal - 정정주문");
		if (chck_account()) modify_order((ModifyData*)lParam);
		break;
	case WP_CANCEL_ORDER:
		WriteLog("AppSignal - 취소주문");
		if (chck_account()) cancel_order((CancelData*)lParam);
		break;
	case WP_MODIFY_RSVD:
		WriteLog("AppSignal - 예약 정정주문");
		if (chck_account()) modify_rsvd((ModifyData*)lParam);
		break;
	case WP_CANCEL_RSVD:
		WriteLog("AppSignal - 예약 취소주문");
		if (chck_account()) cancel_rsvd((CancelData*)lParam);
		break;
	case WP_HOGA_CLICK:
		if      (m_pControl2 && m_AcntMode==AM_GROUP)   m_pControl2->SetJvolPrice(lParam);
		else if (m_pControl  && m_AcntMode==AM_ACCOUNT) m_pControl->SetJvolPrice(lParam);
		break;
	case WP_CTRL2_FOLD:
		FoldControl2((BOOL)lParam);
		break;
	case WP_ONECLICK_MODE:
		m_pHogaWnd->SetOneClickMode((BOOL)lParam);
		break;
	case WP_SET_JCNT:
		m_pControl->SetJcnt( (int)lParam );
		break;
	}
	return 0;
}

HBRUSH CMapWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

	UINT nID = pWnd->GetDlgCtrlID();

	if (nID == IDC_GUIDECTRL)
	{
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor( m_crBodyBk );
		pDC->SetTextColor( m_crNormal );
		hbr = m_hBrush1;
	}
	else if (nID == IDC_ST_ACNM)
	{
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor( m_crBodyBk );
		pDC->SetTextColor( m_crNormal );
		hbr = m_hBrush2;
	}
	else if (nID == IDC_CB_GROUP)
	{
		CDC dc; 
		dc.CreateCompatibleDC(pDC);
		dc.SelectObject(m_hCombo[0]);
		CRect rc;
		CPen pn; pn.CreatePen(PS_SOLID, 1, RGB(125,125,125));
		pDC->SelectObject(&pn);
		m_pGroup->GetClientRect(rc);
		pDC->Rectangle(rc);
		pDC->StretchBlt(rc.right-20,0,19,rc.bottom,&dc,0,0,19,20,SRCCOPY);
		dc.DeleteDC();		
	}
	return hbr;
}

void CMapWnd::proc_guide( LPCSTR msg )
{
	if (strcmp(msg, "그룹계좌가 존재하지 않습니다.")==0) return;
	m_stGuide.SetWindowText(msg);
}

void CMapWnd::ChangeCode( LPCSTR code , BOOL bTrigger /*=TRUE*/)
{
	vector<StringProxy> fields;
	ParseString(code, code+strlen(code), '\t', fields);
	
	if (fields.empty()) return;

	CHAR ch;
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

	if (_code.IsEmpty()) return;
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
		param.Format("ed_fcod\t%s", _code); Variant(codeCC, param); Variant(triggerCC, param);

		//2017.01.24 KSJ 3010번과 트리거 연동되므로 주석처리
		//param.Format("100301\t%s", _code); Variant(codeCC, param); Variant(triggerCC, param);
// 		if      (ch=='1' || ch=='4') { param.Format("30301\t%s", _code); Variant(codeCC, param); Variant(triggerCC, param); }
// 		else if (ch=='2' || ch=='3') { param.Format("40301\t%s", _code); Variant(codeCC, param); Variant(triggerCC, param); }
	}

	//WriteLog("ChangeCode - %s", _code);
	CString s;
	//s.Format("CHANGECODE : [%s]\n",_code);
//	OutputDebugString(s);

	if (m_SiseData.code!=_code)
	{
		req_pibo3002(_code);
		m_pStCodeNm->SetText(_name);

		m_strCode = _code;

		if (m_RsvdCode!=_code)
		{
			cancel_rsvd_all(1);
			cancel_rsvd_all(2);
		}
	}
}

BOOL CMapWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if (wParam==IDC_EDIT_PSWD)
	{
		NMHDR *phdr = (NMHDR*)lParam;
		if (phdr->code==ENM_VKRETURN)
		{
			Send();
		}
	}
	return CWnd::OnNotify(wParam, lParam, pResult);
}

void CMapWnd::calc_micheg( LPCSTR acno )
{
	if (acno)
	{
		AccountMap::iterator pos = m_AcntData.find(acno);
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
	OrderMap::iterator ed = om->end();
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
			
// 			TRACE(
// 				"\n미체결정보 - 종목(%s) 가격(%d) 매도(%d/%d) 매수(%d/%d)\n",
// 				code.c_str(), pst->second.oprc, pst->second.d_cnt, pst->second.d_wqty, 
// 				pst->second.s_cnt, pst->second.s_wqty );
		}
	}
	if (bNotify) RefreshMicheg();
}

void CMapWnd::cancel_order_all(int odgb)
{
	vector< vector<char> > orders;
	AccountMap::iterator ast = m_AcntData.begin(), aed = m_AcntData.end();

	vector<CString> logmsg;

	for(; ast!=aed; ++ast)
	{
		OrderMap *om = &(ast->second.order[(LPCSTR)m_SiseData.code]);
		OrderMap::iterator ost = om->begin(), oed = om->end();
		for(; ost!=oed; ++ost)
		{
			Order *pOdr = &(ost->second);
			if (pOdr->odgb==odgb && pOdr->wqty>0)
			{
				orders.push_back(vector<char>());
				make_fodr(orders.back(), 4, pOdr->acno, GetPassword(pOdr->acno), pOdr->juno, pOdr->code, pOdr->wqty, 0, 0);
				
				logmsg.push_back("");
				logmsg.back().Format("주문취소 - 주문번호(%d)", pOdr->juno);
			}
		}
	}
	if (orders.empty()) return;

	vector<char> data;
	merge_order(data, orders);
	SendTR("pibofodr", US_ENC|US_CA, &data[0], data.size(), TK_PIBOFODR);
	
	for(size_t n=0; n<logmsg.size(); ++n)
		WriteLog(logmsg[n]);

}

void CMapWnd::make_fodr( vector<char> &buff, int mmgb, LPCSTR acno, LPCSTR pswd, int ojno, LPCSTR code, int jqty, int jprc, int cdgb )
{
	if (mmgb<1 || mmgb>4) return;
	if (!acno || !pswd || !code) return;
	if (mmgb>2 && ojno<1) return;

	int lsize = L_ledgerH;
	int dsize = sizeof(pibofodr_mid);
	buff.resize(L_ledgerH + sizeof(pibofodr_mid));
	FillMemory(&buff[0], buff.size(), ' ');
	
	struct _ledgerH *ledger = (struct _ledgerH *)&buff[0];
	pibofodr_mid *mid = (pibofodr_mid*)&buff[L_ledgerH];

	// Ledger 세팅
	GetLedger(ledger);
	CopyMemory(ledger->odrf, "1", 1);
	CopyMemory(ledger->rcnt, "0000", 4);

	//2013.12.23 KSJ 일방향암호화 추가
	if(strlen(pswd) > 0)
	{
		CString sPswd;
		sPswd = GetEncPassword(pswd);
		CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
		CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	}

	ledger->mkty[0] = '2';
	ledger->odrf[0] = '1';

	// 데이터 세팅
	CopyMemory(mid->nrec, "0001", 4);
	mid->odgb[0] = '2';
	mid->mmgb[0] = '0' + mmgb;
	sprintf(mid->accn, "%11s", acno);
// 	sprintf(mid->pswd, "%-8s", pswd);
	sprintf(mid->pswd, "%-8s", "HEAD");	//2013.12.23 KSJ 일방향암호화 추가
	sprintf(mid->ojno, "%12d", ojno);
	sprintf(mid->cod2, "%s", code);
	sprintf(mid->jqty, "%8d", jqty);
	
	if (jprc==MARKET_JPRC)
	{
		sprintf(mid->jprc, "%10d", 0);
		CopyMemory(mid->hogb, "03", 2);
		sprintf(mid->cdgb, "%d", 0);
	}
	else
	{
		sprintf(mid->jprc, "%10d", jprc);
		CopyMemory(mid->hogb, "00", 2);
		sprintf(mid->cdgb, "%d", cdgb);
	}
	
	mid->mdgb[0] = '2';
	mid->prgb[0] = 'X';
	FillMemory(mid->usrf, sizeof(mid->usrf), ' ');
}

void CMapWnd::click_order( Order *pOdr )
{
	if (m_SiseData.code.GetLength()!=FCODE_LEN) return;

	vector< vector<char> > orders;
	AccountMap::iterator st = m_AcntData.begin();
	AccountMap::iterator ed = m_AcntData.end();

	LPCSTR code = m_SiseData.code;
	CODE_TYPE type = m_SiseData.type;
	int odgb = pOdr->odgb;

	int jqty = (pOdr->stop) ? pOdr->oqty : m_pControl->GetJqty();
	if (!pOdr->stop && m_bFoldCtrl1)
		jqty = Str2Int(m_pSpJcnt->GetText());

	int cdgb = m_pControl->GetCdgb();
	int mult = m_pControl->GetMulti();
	int oprc = pOdr->oprc;
	int d_acno_qty = 0, d_oqty = 0;
	if (jqty==0)
	{
		MessageBox("주문수량을 입력하십시오.", "IBK투자증권", MB_OK|MB_ICONWARNING);
		return;
	}

	for(; st!=ed; ++st)
	{	
		if(m_utype == 0)
		{
			if(m_AcntMode==AM_GROUP)
			{
				if(m_otpf != 1 && !st->second.IsOwn)
				{
					continue;
				}
			}
		}

		LPCSTR acno = st->first.c_str();
// 		CString s;
// 		s.Format("[ORDER ACCOUNT : %s]\n",acno);
// 		OutputDebugString(s);
		LPCSTR pswd = GetPassword(acno);
		oprc = pOdr->oprc;	// 여기서 주문가격 초기화 안하면 계좌별로 계속 가격 누적된다!
		if (oprc==MARKET_JPRC)
			cdgb = 3;

// 		orders.push_back(vector<char>());
// 		make_fodr(orders.back(), odgb, acno, pswd, 0, code, jqty * st->second.mult, oprc, cdgb);
		
		//2014.05.22 KSJ 주문착오방지 적용
		MistakeOrder misOdr;
		
		misOdr.strAccn.Format("%.3s-%.2s-%.6s", acno, acno+3, acno+5);
		misOdr.strCode = code;
		misOdr.lPrice = oprc * 0.01;
		misOdr.lQuantity = jqty * st->second.mult;
		misOdr.nMmgb = odgb;
		misOdr.strHogb.Format("%s", pOdr->bmarket ? "03":"00");
		misOdr.strCurPrc.Format("%f", m_pHogaWnd->m_pSiseData->curr * 0.01);
		misOdr.strStdPrc.Format("%f", m_pHogaWnd->m_pSiseData->gjga * 0.01);
		
		bool bRet = CheckupMistakeOrder(this, misOdr);
		
		if(bRet)
		{
			orders.push_back(vector<char>());
			make_fodr(orders.back(), odgb, acno, pswd, 0, code, jqty * st->second.mult, oprc, cdgb);
			
			d_acno_qty += 1;
			d_oqty += jqty * st->second.mult;
		}
		//End

		if (!pOdr->stop && oprc!=MARKET_JPRC)	// stop, 시장가일 경우 다중매매를 적용하지 않는다.
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

		CString s;
		s.Format("OPRC : [%d]\n",oprc);
//		OutputDebugString(s);
		
		int ret = dlg.DoModal();
		m_pHogaWnd->SetFocusGrid();
		if (ret!=IDOK) return;
		
	}
	SendTR("pibofodr", US_ENC|US_CA, &data[0], data.size(), TK_PIBOFODR);

	WriteLog("주문실행 - 종목(%s) 구분(%s) 수량(%d) 가격(%d)", code, (odgb==1) ? "매도":"매수", jqty, oprc);
}

void CMapWnd::merge_order( vector<char> &buff, vector< vector<char> > &orders )
{
	buff.clear();
	if (orders.empty()) return;

	int cnt = orders.size();
	int offset = L_ledgerH + offsetof(pibofodr_mid, odgb);
	int size = L_ledgerH + sizeof(pibofodr_mid);

	char tmp[32];
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

void CMapWnd::AddAccount(LPCSTR acno, LPCSTR pswd, LPCSTR acnm, int mult)
{
	AccountMap::iterator pos = m_AcntData.find(acno);
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

	NotifyMap::iterator pos2 = m_NotifyMap.find(acno);
	if (pos2==m_NotifyMap.end())
	{
		CNotifyCtrl *pNotify = new CNotifyCtrl(m_pParent, "", m_NotifyKeys.back());
		pNotify->Create(NULL, "NOTIFY", WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, -1);
		m_NotifyKeys.pop_back();
		m_NotifyMap[acno] = pNotify;
		pNotify->Send(acno, pswd, "");
	}

	if (m_AcntMode==AM_GROUP)
		m_pControl2->PostMessage(WM_APP_REFRESH, WP_ACCOUNT, (LPARAM)m_AcntData.size());

	RefreshRsvd();
}

void CMapWnd::DelAccount(LPCSTR acno)
{
	AccountMap::iterator pos = m_AcntData.find(acno);
	if (pos!=m_AcntData.end()) 
		m_AcntData.erase(pos);
	
	NotifyMap::iterator pos2 = m_NotifyMap.find(acno);
	if (pos2!=m_NotifyMap.end())
	{
		CNotifyCtrl *pNotify = pos2->second;
		pNotify->DestroyWindow();
		m_NotifyKeys.push_back(pNotify->GetKey());
		delete pNotify;
		m_NotifyMap.erase(pos2);
	}
}

void CMapWnd::ClearAccount()
{
	m_AcntData.clear();

	NotifyMap::iterator st = m_NotifyMap.begin();
	NotifyMap::iterator ed = m_NotifyMap.end();
	for(; st!=ed; ++st)
	{
		CNotifyCtrl *pNotify = st->second;
		pNotify->DestroyWindow();
		m_NotifyKeys.push_back(pNotify->GetKey());
		delete pNotify;
	}
	m_NotifyMap.clear();
	if (m_pControl)  m_pControl->Clear();
	if (m_pControl2) m_pControl2->Clear();
	if (m_pJngoTick) m_pJngoTick->Clear();
}

void CMapWnd::RefreshMicheg()
{
	m_pHogaWnd->SendMessage(WM_APP_REFRESH, WP_MICHEG, (LPARAM)(LPCSTR)m_SiseData.code);
	m_pControl->SendMessage(WM_APP_REFRESH, WP_MICHEG, (LPARAM)(LPCSTR)m_SiseData.code);
}

void CMapWnd::cancel_order( CancelData *pcd )
{
	vector< vector<char> > orders;
	AccountMap::iterator st = m_AcntData.begin();
	AccountMap::iterator ed = m_AcntData.end();
	
	LPCSTR code = m_SiseData.code;
	CODE_TYPE type = m_SiseData.type;
	int odgb = pcd->odgb;
	int d_oqty = 0, d_ojno = 0, d_jqty = 0;
	set<string> acno_qty;

	for(; st!=ed; ++st)
	{
		LPCSTR acno = st->first.c_str();
		LPCSTR pswd = GetPassword(acno);
		CodeOrderMap::iterator pos = st->second.order.find(code);
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
		int ret = dlg.DoModal();
		m_pHogaWnd->SetFocusGrid();
		if (ret!=IDOK) return;
	}

	SendTR("pibofodr", US_ENC|US_CA, &data[0], data.size(), TK_PIBOFODR);
}

void CMapWnd::modify_order( ModifyData *pmd )
{
	vector< vector<char> > orders;
	AccountMap::iterator st = m_AcntData.begin();
	AccountMap::iterator ed = m_AcntData.end();
	
	LPCSTR code = m_SiseData.code;
	CODE_TYPE type = m_SiseData.type;
	int odgb = pmd->odgb;
	int cdgb = m_pControl->GetCdgb();
	int d_oqty = 0, d_ojno = 0, d_jqty = 0;
	set<string> acno_qty;
	
	for(; st!=ed; ++st)
	{
		LPCSTR acno = st->first.c_str();
		LPCSTR pswd = GetPassword(acno);
		CodeOrderMap::iterator pos = st->second.order.find(code);
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
		int ret = dlg.DoModal();
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

// 	CString s;
// 	s.Format("현재가 : %d\n",m_SiseData.curr);
// 	OutputDebugString(s);

	int n, rsvd = m_pControl->GetRsvd(), jqty;
	CODE_TYPE type = m_SiseData.type;

	if (m_bFoldCtrl1)	jqty = Str2Int(m_pSpJcnt->GetText());
	else				jqty = m_pControl->GetJqty();

	if(GetMarket() == FALSE)
	{
		prd->oprc = prd->cprc;
		prd->bmarket = FALSE;
	}
	else
	{
		prd->oprc = MARKET_JPRC;
		prd->bmarket = TRUE;
	}

	prd->oqty = jqty;

	if (prd->oqty==0)
	{
		MessageBox("주문수량을 입력하십시오.", "IBK투자증권", MB_OK|MB_ICONWARNING);
		return;
	}

	if(m_utype == 0)
	{
		if(m_AcntMode==AM_GROUP && m_otpf == 0)
		{
			CAccountConfirmWnd accDlg(m_pParent,&m_AcntData);
			
			int ret = accDlg.DoModal();
			
			if(ret == IDCANCEL)
			{
				return;
			}
		}
	}
	
	if(GetMarket() == FALSE)
	{
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
	}
	else
	{
		prd->ptic = 0;
	}

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
		
		//2016.11.15 KSJ 이전현재가가 초기값이 아니고 예약주문가가 현재가랑 같을때
		if (m_SiseData.pcurr == FIRST_PCURR && st->cprc == m_SiseData.curr)
		{
			bOrder = TRUE;
		}
		else if (m_SiseData.pcurr != FIRST_PCURR)
		{
			if (m_SiseData.pcurr <= st->cprc && m_SiseData.curr >= st->cprc)	bOrder = TRUE;    // 현재가가 아래서 위로 지나갈때 
			else if (m_SiseData.pcurr >= st->cprc && m_SiseData.curr <= st->cprc)	bOrder = TRUE;    // 현재가가 위에서 아래로 지나갈때	
		}

		if (bOrder)
		{
			Order *pOdr = new Order();
			pOdr->stop = true;
			pOdr->oprc = st->oprc;
			pOdr->oqty = st->oqty;
			pOdr->odgb = st->odgb;
			pOdr->bmarket = st->bmarket;
			if      (pOdr->odgb==1) mado_rd = *st;
			else if (pOdr->odgb==2) masu_rd = *st;
			PostMessage(WM_APP_SIG, WP_RSVD_REALORDER, (LPARAM)pOdr);

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

	ReserveData mado_rd[2], masu_rd[2];
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

void CMapWnd::SetGuide( LPCSTR msg )
{
	m_stGuide.SetWindowText(msg);	
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

void CMapWnd::req_SDLDQ003_grp(BOOL bNext /*=FALSE*/, LPCSTR nkey /*=NULL*/)
{
	m_pGroup->EnableWindow(FALSE);

	vector<char> buff( L_ledgerH + sizeof(SDLDQ003_mid), ' ');

	struct _ledgerH *ledger = (struct _ledgerH*)&buff[0];
	SDLDQ003_mid *mid = (SDLDQ003_mid*)&buff[L_ledgerH];

	// Ledger settingㅡ
	GetLedger(ledger);
	CopyMemory(ledger->odrf, "1", 1);
	CopyMemory(ledger->svcd, "SDLDQ003", 8);
	CopyMemory(ledger->rcnt, "0020", 4);
	CopyMemory(ledger->mkty, "2", 1);
	CopyMemory(ledger->odrf, "2", 1);

	// Data setting
	CopyMemory(mid->In, "00001", 5);
	CopyMemory(mid->QryTp, "1", 1);
	CopyMemory(mid->GrpTp, "11", 2);

	SendTR("piboPBxQ", US_ENC, &buff[0], buff.size(), TK_SDLDQ003);
}

void CMapWnd::req_SDLDQ003_acc( LPCSTR gseq, LPCSTR gnam )
{
	m_pGroup->EnableWindow(FALSE);

	vector<char> buff( L_ledgerH + sizeof(SDLDQ003_mid), ' ');
	
	struct _ledgerH *ledger = (struct _ledgerH*)&buff[0];
	SDLDQ003_mid *mid = (SDLDQ003_mid*)&buff[L_ledgerH];
	
	// Ledger setting
	GetLedger(ledger);
	CopyMemory(ledger->odrf, "1", 1);
	CopyMemory(ledger->svcd, "SDLDQ003", 8);
	CopyMemory(ledger->rcnt, "0020", 4);
	CopyMemory(ledger->mkty, "2", 1);
	CopyMemory(ledger->odrf, "2", 1);
	
	// Data setting
	CopyMemory(mid->In, "00001", 5);
	CopyMemory(mid->QryTp, "2", 1);
	CopyMemory(mid->GrpTp, "11", 2);
	CopyMemory(mid->GrpSeqn, gseq, strlen(gseq));
	CopyMemory(mid->GrpName, gnam, strlen(gnam));
	
	SendTR("piboPBxQ", US_ENC, &buff[0], buff.size(), TK_SDLDQ003);
	
}

void CMapWnd::res_SDLDQ003( LPCSTR data, int size )
{
	m_pGroup->EnableWindow(TRUE);

	struct _ledgerH *ledger = (struct _ledgerH*)data;
	SDLDQ003_mod *mod = (SDLDQ003_mod*)(data+L_ledgerH);

	CString emsg(ledger->emsg, sizeof(ledger->emsg));
	GetParent()->SendMessage(WM_USER, DLL_GUIDE, (LPARAM)(LPCSTR)emsg);

	if (mod->mid.QryTp[0]=='1')		// 그룹조회
	{
		m_pGroup->ResetContent();
		int nrec = Str2Int(mod->In, sizeof(mod->In));
		for(int n=0; n<nrec; ++n)
		{
			CString gseq(mod->grid[n].GrpSeqn, sizeof(mod->grid[n].GrpSeqn));
			gseq.TrimRight();
			gseq = gseq.Mid(gseq.Find("-")+1);
			
			CString gnam(mod->grid[n].GrpName, sizeof(mod->grid[n].GrpName));
			gnam.TrimRight();
			m_pGroup->AddString(gseq + ". " + gnam);
		}
		if (nrec) 
		{
			m_LastGroup = "";
			m_pGroup->SetCurSel(0);
			OnGroupChange();
		}
	}
	else if (mod->mid.QryTp[0]=='2')	// 계좌조회
	{
		m_pGroup->GetLBText(m_pGroup->GetCurSel(), m_LastGroup);
		int nrec = Str2Int(mod->In, sizeof(mod->In));
		for(int n=0; n<nrec; ++n)
		{
			CString acno(mod->grid[n].Acno, sizeof(mod->grid[n].Acno)); acno.TrimRight();
			CString pswd(mod->grid[n].Pswd, sizeof(mod->grid[n].Pswd)); pswd.TrimRight();
			CString acnm(mod->grid[n].Acnm, sizeof(mod->grid[n].Acnm)); acnm.TrimRight();
			CString mult(mod->grid[n].Mult, sizeof(mod->grid[n].Mult)); mult.TrimRight();
			AddAccount(acno, pswd, acnm, atoi(mult));		
		}
		m_pPswd->EnableWindow(FALSE);
	}
	//m_pHogaWnd->SetFocusGrid();
}

CfxImgButton* CMapWnd::CreateImgButton(LPCSTR caption, int nID, CBitmap **bmp)
{
	CfxImgButton *pBtn = new CfxImgButton(m_pFont);
	if (pBtn->Create(caption, CRect(0, 0, 0, 0), this, nID, FALSE, TRUE))
	{
		pBtn->SetImgBitmap(	bmp[0]->operator HBITMAP(), bmp[1]->operator HBITMAP(), bmp[2]->operator HBITMAP() );
		return pBtn;
	}
	else return NULL;
}
 
BOOL CMapWnd::PreTranslateMessage(MSG* pMsg) 
{
//	if (pMsg->message==WM_KEYUP)
//		TRACE("PreTranslateMessage -> %d %d", pMsg->wParam, pMsg->lParam);
	return CWnd::PreTranslateMessage(pMsg);
}

void CMapWnd::OnPswdFocus()
{
}

void CMapWnd::OnGroupEdit()
{
	SetModal(MAP_GROUPEDIT);
	req_SDLDQ003_grp();
}

BOOL CMapWnd::chck_account()
{
	if (m_AcntData.empty())
	{
		SetGuide("계좌번호 혹은 그룹을 확인하시기 바랍니다.");
		return FALSE;
	} else return TRUE;
}

void CMapWnd::ChangeAccount( LPCSTR data )
{
	if (m_AcntMode==AM_GROUP)
	{
		return;
	}

	vector<StringProxy> fields;
	ParseString(data, data+strlen(data), '\t', fields);
	CString acno, acnm, pswd;
	acno = fields[0].ToCStr();
	acnm = fields[1].ToCStr();
	if (fields.size()>2)
		pswd = fields[2].ToCStr();

	/*
	if (m_AcntMode == AM_GROUP) 
	{
		m_AcntMode = AM_ACCOUNT;
		CalcSize();		// 컨트롤 Visible/Position 세팅
	}
	*/

	if (m_AcntData.size()==1)
	{
		if (m_AcntData.begin()->first == (LPCSTR)acno)
			return;
	}

	ClearAccount();	
	m_pAcno->SetWindowText(acno);
	m_pStAcnm->SetText(acnm);

	WriteLog("ChangeAccount - %s(%s)", (LPCSTR)acno, (LPCSTR)acnm);

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

	cancel_rsvd_all(1);
	cancel_rsvd_all(2);
}

void CMapWnd::req_SACMT238( LPCSTR acno, LPCSTR pswd )
{
	vector<char> buff(L_ledgerH+sizeof(SACMT238_mid), ' ');
	struct _ledgerH *ledger = (struct _ledgerH *)&buff[0];
	SACMT238_mid *mid = (SACMT238_mid*)&buff[L_ledgerH];

	GetLedger(ledger);
	CopyMemory(ledger->svcd, "SACMT238", 8);
	CopyMemory(ledger->brno, "000", 3);

	if(strlen(pswd) > 0)
	{
		CString sPswd;
		sPswd = GetEncPassword(pswd);
		CopyMemory(ledger->hsiz, "44", sizeof(ledger->hsiz));
		CopyMemory(ledger->epwd, sPswd, sPswd.GetLength());
	}

	CopyMemory(ledger->odrf, "1", 1);
	CopyMemory(ledger->mkty, "1", 1);
	
	CopyMemory(mid->In, "00001", 5);
	CopyMemory(mid->zAcntNo, acno, strlen(acno));

	// 2011.07.06 로그인계좌소유주 점검 결과 개별계좌 송신으로 변경
	//mid->zAcntNo[3] = mid->zAcntNo[4] = '0';
// 	CopyMemory(mid->zPwd, pswd, strlen(pswd));
	CopyMemory(mid->zPwd, "HEAD", 4);	//2013.12.23 KSJ 일방향암호화 추가

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

void CMapWnd::OnSendBtn()
{
	WriteLog("OnSendBtn");
	Send();
}

void CMapWnd::OnConfBtn()
{
	WriteLog("OnConfBtn");
	CConfigDlg dlg((CWnd*)this, m_profile, (CAxisExt*)this);

	m_pHogaWnd->GetWidths((int*)&dlg.m_rsvd_width, (int*)&dlg.m_jumn_width, (int*)&dlg.m_size_width, 
		(int*)&dlg.m_remain_width, (int*)&dlg.m_hoga_width, (int*)&dlg.m_rate_width);

	if (dlg.DoModal()==IDOK)
	{
		// 숨기기상태는 설정창에서 설정하는 항목이 아니다. 
		// 따라서 상태를 유지시켜줘야한다(설정버튼클릭시), 시작,종료시에만 유효하다.
		BOOL bFold1 = m_bFoldCtrl1;
		BOOL bFold2 = m_bFoldCtrl2;
		LoadConfig();
		m_bFoldCtrl1 = bFold1;
		m_bFoldCtrl2 = bFold2;

		m_pHogaWnd->SetWidths(dlg.m_rsvd_width, dlg.m_jumn_width, dlg.m_size_width, 
			dlg.m_remain_width, dlg.m_hoga_width, dlg.m_rate_width);

		CString val;
		val.Format("%d", dlg.m_rsvd_width);		WritePrivateProfileString("WIDTH", "RSVD", val, m_profile);
		val.Format("%d", dlg.m_jumn_width);		WritePrivateProfileString("WIDTH", "JUMUN", val, m_profile);
		val.Format("%d", dlg.m_size_width);		WritePrivateProfileString("WIDTH", "CNT", val, m_profile);
		val.Format("%d", dlg.m_remain_width);	WritePrivateProfileString("WIDTH", "VOL", val, m_profile);
		val.Format("%d", dlg.m_hoga_width);		WritePrivateProfileString("WIDTH", "HOGA", val, m_profile);
		val.Format("%d", dlg.m_rate_width);		WritePrivateProfileString("WIDTH", "RATE", val, m_profile);

		CalcSize();

		m_pHogaWnd->m_bSiJangDBClick = m_bSiJangDBClick;	//2017.01.24 KSJ 시장가 더블클릭
	}
}

void CMapWnd::proc_trigger( LPCSTR data, int size )
{
	vector<StringProxy> fields;
	ParseString(data, data+size, '\t', fields);

// 	CString s;
// 	s.Format("상품선물클릭 : [%s]\n",data);
//	OutputDebugString(s);

	if (fields.size()==2)
	{
		CString sym = fields[0].ToCStr();
		CString val = fields[1].ToCStr();

		CString tmp = val.Mid(0,2);
		
		//if (sym=="ed_focod" || sym=="30301" || sym=="40301")
		if (sym=="ed_fcod")
		{
			if(tmp != "16" && tmp != "17" && tmp != "46" && tmp != "47")
			return;

			ChangeCode(val, FALSE);
		}
 #ifdef _DEF_IB300900_
 		else if (sym=="AN20")
 			m_pAcno->Trigger(val);
 #endif		
	}
}

void CMapWnd::OnCodeSearch()
{
	CRect rect;
	m_pCode->GetWindowRect(&rect);
	LPCSTR data = (LPCSTR)m_pParent->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(goodFOCODE, 2)), MAKELPARAM(rect.left, rect.bottom - 100));
	
	vector<StringProxy> fields;
	ParseString(data, data+strlen(data), '\t', fields);

	if (fields.empty()) return;

	CString code = fields[0].ToCStr();
	if (code.GetLength()==5)
		code += "000";

	ChangeCode(code);
}

void CMapWnd::OnGroupChange()
{
	if (m_AcntMode == AM_ACCOUNT) 
	{
		m_AcntMode = AM_GROUP;
		CalcSize();		// 컨트롤 Visible/Position 세팅
	}
	else
	{
		CString group_nm;
		m_pGroup->GetLBText(m_pGroup->GetCurSel(), group_nm);
		if (m_LastGroup == group_nm) return;
	}
	ClearAccount();
	m_pPswd->EnableWindow(TRUE);
	m_pPswd->SetText("");
	m_pPswd->SetFocus();
}

void CMapWnd::ChangeTheme()
{
	AxInit();
	LoadImage();
	MakeBackground();

	m_pControl->ChangeTheme();
	m_pSise->ChangeTheme();
	m_pHogaWnd->ChangeTheme();
	m_pControl2->ChangeTheme();
	m_pJngoTick->ChangeTheme();

	CalcSize();
	Invalidate(TRUE);
}

void CMapWnd::Send()
{
	CString code = m_pCode->GetCode();
	if (code.GetLength()==8)
	{
		CString name = m_pCode->GetName();
		ChangeCode(code+"\t"+name, FALSE);
	}

	if (!m_pGroup->IsWindowEnabled())
	{
		MessageBox("그룹조회중입니다. 잠시 후 조회하시기 바랍니다.", "IBK투자증권", MB_OK|MB_ICONWARNING);
		return;
	}
	if (!m_pPswd->IsWindowEnabled()) 
		m_pPswd->EnableWindow(TRUE);

	m_SiseData.code = "";
	ChangeCode(m_pCode->GetCode(), FALSE);

	if (m_AcntMode==AM_ACCOUNT)
	{
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
	else
	{
		CString pswd1, pswd2;
		pswd1 = m_pPswd->GetText();
		pswd2 = Variant(passCCx, "");
		if (pswd1!=pswd2)
		{
			MessageBox("접속비밀번호가 올바르지 않습니다", "IBK투자증권", MB_OK|MB_ICONERROR);
			m_pPswd->SetSel(0, -1, TRUE);
			m_pPswd->SetFocus();
		}
		else
		{
			if (m_pGroup->GetCount()<=0)
			{
				MessageBox("그룹이 존재하지 않습니다.\r\n그룹편집후 조회하시기 바랍니다.", "IBK투자증권", MB_OK|MB_ICONERROR);
				return;
			}
			ClearAccount();
			CString data;
			m_pGroup->GetLBText(m_pGroup->GetCurSel(), data);
			CString gseq = m_usid + "-" + data.Mid(0, 3);
			CString gnam = data.Mid(5);
			req_SDLDQ003_acc(gseq, gnam);
		}
	}	
}

void CMapWnd::res_pibofodr( LPCSTR data, int size )
{
	struct _ledgerH *ledger = (struct _ledgerH*)data;
	pibofodr_mod *mod = (pibofodr_mod *)(data+L_ledgerH);
	
	UINT oidx = Str2Int(&ledger->fil2[ORDER_INFO_IDX], 6);
	OrderAcnoMap::iterator pos = m_OdrAcnoMap.find(oidx);

	CString emsg(ledger->emsg, sizeof(ledger->emsg));
	emsg.TrimRight();

	// 2011.05.19 기업은행 임직원 주문제한 처리 by warship
	if ( CString(ledger->ecod, sizeof(ledger->ecod)) == "9997" )
		MessageBox(emsg, "IBK투자증권");

	SetGuide( emsg );

	if (pos!=m_OdrAcnoMap.end())
	{
		int nrec = Str2Int(mod->nrec, sizeof(mod->nrec));
		if (pos->second.size()!=nrec) return;
		for(int n=0; n<nrec; ++n)
		{
			int juno, ojno;
			OrderResult or;
			pibofodr_grid *grid = &mod->grid[n];
			or.acno = pos->second[n];
			or.emsg = CString(grid->emsg, sizeof(grid->emsg));
			juno = atoi(CString(grid->jmno, sizeof(grid->jmno)));
			ojno = atoi(CString(grid->ojno, sizeof(grid->ojno)));
 
			or.emsg.TrimRight();
			or.juno = (juno==0) ? "" : Int2CommaStr(juno);
			or.ojno = (ojno==0) ? "" : Int2CommaStr(ojno);

			if (m_AcntMode==AM_GROUP)
				m_pControl2->SendMessage(WM_APP_REFRESH, WP_ORDER_RESULT, (LPARAM)&or);

			WriteLog("주문응답 - 주번(%d) 원주(%d) %s", juno, ojno, or.emsg);
		}
	}

	//2017.01.24 KSJ 주문후 수량 리셋 설정되어 있으면
	if(m_bJumunReset)
	{
		m_pSpJcnt->SetText("0");
		m_pControl->SetJcnt(0, TRUE);
	}
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

void CMapWnd::FoldControl2( BOOL bFold )
{
	if (m_bFoldCtrl2 != bFold)
	{
		m_bFoldCtrl2 = bFold;
		CalcSize();
	}
}

void CMapWnd::OnLimit()
{
	SetView(MAP_ORDERLIMIT);
}

void CMapWnd::SaveConfig()
{
	WritePrivateProfileString("Config", "Fold1", m_bFoldCtrl1 ? "TRUE" : "FALSE", m_profile);
	WritePrivateProfileString("Config", "Fold2", m_bFoldCtrl2 ? "TRUE" : "FALSE", m_profile);

	WritePrivateProfileString("Config", "SpaceKeyOrder" , m_pHogaWnd->GetSpaceOrder()     ? "TRUE" : "FALSE", m_profile);
	WritePrivateProfileString("Config", "ExtOrderColumn", m_pHogaWnd->GetExtOrderColumn() ? "TRUE" : "FALSE", m_profile);
}

void CMapWnd::LoadConfig()
{
	char buff[128];

	GetPrivateProfileString("Config", "ShowConfirm", "TRUE", buff, sizeof(buff), m_profile);
	m_bShowConfirm = (strcmp(buff, "FALSE")==0) ? FALSE : TRUE;
	
	GetPrivateProfileString("Config", "ShowRsvd", "TRUE", buff, sizeof(buff), m_profile);
	m_bShowRsvd = (strcmp(buff, "FALSE")==0) ? FALSE : TRUE;

	GetPrivateProfileString("Config", "ShowRate", "TRUE", buff, sizeof(buff), m_profile);
	m_bShowRate = (strcmp(buff, "FALSE")==0) ? FALSE : TRUE;

	GetPrivateProfileString("Config", "ShowBefore", "TRUE", buff, sizeof(buff), m_profile);
	m_bShowBefore = (strcmp(buff, "FALSE")==0) ? FALSE : TRUE;

	GetPrivateProfileString("Config", "Fold1", "FALSE", buff, sizeof(buff), m_profile);
	m_bFoldCtrl1 = (strcmp(buff, "TRUE")==0) ? TRUE : FALSE;

	GetPrivateProfileString("Config", "Fold2", "FALSE", buff, sizeof(buff), m_profile);
	m_bFoldCtrl2 = (strcmp(buff, "TRUE")==0) ? TRUE : FALSE;
	
	GetPrivateProfileString("Config", "SpaceKeyOrder", "FALSE", buff, sizeof(buff), m_profile);
	m_pHogaWnd->SetSpaceOrder((strcmp(buff, "TRUE")==0) ? TRUE : FALSE);

	GetPrivateProfileString("Config", "ExtOrderColumn", "FALSE", buff, sizeof(buff), m_profile);
	m_pHogaWnd->SetExtOrderColumn((strcmp(buff, "TRUE")==0) ? TRUE : FALSE);

	GetPrivateProfileString("Config", "RClickCancel", "FALSE", buff, sizeof(buff), m_profile);
	m_pHogaWnd->SetRClickCancel((strcmp(buff, "TRUE")==0) ? TRUE : FALSE);

	GetPrivateProfileString("Config", "SiJangDBClick", "FALSE", buff, sizeof(buff), m_profile);
	m_bSiJangDBClick = (strcmp(buff, "FALSE")==0) ? FALSE : TRUE;
	
	GetPrivateProfileString("Config", "JumunReset", "FALSE", buff, sizeof(buff), m_profile);
	m_bJumunReset = (strcmp(buff, "FALSE")==0) ? FALSE : TRUE;

	if (m_pHogaWnd)
	{
		if (m_pHogaWnd->GetSafeHwnd())
		{
			m_pHogaWnd->ShowRate(m_bShowRate ? true : false);
			m_pHogaWnd->ShowBefore(m_bShowBefore ? true : false);
			m_pHogaWnd->ShowRsvd(m_bShowRsvd ? true : false);
			m_pHogaWnd->m_bSiJangDBClick = m_bSiJangDBClick;	//2017.01.24 KSJ 시장가 더블클릭

			int rsvd, jumun, cnt, vol, hoga, rate;
			m_pHogaWnd->GetWidths(&rsvd, &jumun, &cnt, &vol, &hoga, &rate);
			rsvd = GetPrivateProfileInt("WIDTH", "RSVD", rsvd, (LPCSTR)m_profile);
			jumun = GetPrivateProfileInt("WIDTH", "JUMUN", jumun, (LPCSTR)m_profile);
			cnt = GetPrivateProfileInt("WIDTH", "CNT", cnt, (LPCSTR)m_profile);
			vol = GetPrivateProfileInt("WIDTH", "VOL", vol, (LPCSTR)m_profile);
			hoga = GetPrivateProfileInt("WIDTH", "HOGA", hoga, (LPCSTR)m_profile);
			rate = GetPrivateProfileInt("WIDTH", "RATE", rate, (LPCSTR)m_profile);
			m_pHogaWnd->SetWidths(rsvd, jumun, cnt, vol, hoga, rate);
			CalcSize();
		}
	}

	CString usrfile;
	usrfile.Format("%s\\%s\\%s\\%s.ini", Variant(homeCC,""), "user", Variant(nameCC,""),Variant(nameCC,""));
	
	m_otpf = GetPrivateProfileInt("LOGIN","OTPF",0,usrfile);

	m_utype = GetPrivateProfileInt("LOGIN","UTYP",0,usrfile);

	usrfile.Format("%d",m_otpf);

	if (GetSafeHwnd()) CalcSize();
}

void CMapWnd::OnFold()
{
	CString text;
	m_pFold->GetWindowText(text);
	//if (text=="<<")
	if (!m_bFoldCtrl1)
	{
		m_bFoldCtrl1 = TRUE;
		m_pFold->SetWindowText(">>");

		CString jcnt;
		jcnt.Format("%d", m_pControl->GetJqty());
		m_pSpJcnt->SetText( jcnt );
		//MessageBox("주문시, 세부설정창의 설정내용을 반드시 확인하시기 바랍니다.", "IBK투자증권", MB_ICONWARNING|MB_OK);
	}
	else
	{
		m_bFoldCtrl1 = FALSE;
		m_pFold->SetWindowText("<<");

		CString jcnt = m_pSpJcnt->GetText();
		m_pControl->SetJcnt( atoi(jcnt) );
	}
	CalcSize();
}

void CMapWnd::proc_domino( LPCSTR data, int size )
{
	//MessageBox(data);
	vector<StringProxy> fields;
	ParseString(data, data+size, '\t', fields);
}

void CMapWnd::RefreshRsvd()
{
	RsvdDataArray::iterator pos = m_RsvdData.begin();
	RsvdDataArray::iterator ed = m_RsvdData.end();
	for(; pos!=ed; ++pos)
		m_pHogaWnd->SendMessage(WM_APP_REFRESH, WP_RESERVE, (LPARAM)&*pos);
}

LRESULT CMapWnd::OnGetCode( WPARAM wParam, LPARAM lParam )
{
//	OutputDebugString("ONGETCODE================\n");
	//CString sCode = m_pCode->GetCode();
	CString s;
// 	s.Format("GETCODE : [%s]\n",m_strCode);
// 	OutputDebugString(s);

	return (LONG)(char*)m_strCode.operator LPCTSTR();
}

void CMapWnd::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	//가상화면 1번을 뺸 나머지 가상화면에서 HTS 시작시 뜰 경우 사이즈가 원래사이즈로 뜨는 현상을 막기위해 추가
	if(m_nWidth > 0 && m_nHeight > 0)
	{
		SetSize(m_nWidth,m_nHeight);
	}
}

CString CMapWnd::GetEncPassword(CString sPswd)
{
	CString dllPath;
	dllPath.Format(_T("%s%s"), m_home, _T("\\dev\\CX_SHA256.DLL"));
	CString strRetrun;
	
	if(m_hiSha256 == NULL)
	{
		m_hiSha256 = LoadLibrary(dllPath);
		
		if (!m_hiSha256)
		{
			TRACE("CX_SHA256 컨트롤 생성 실패1");
			return "";
		}
	}
	
	if(m_hiSha256)
	{
		typedef long (WINAPI *GETSHAFUNC)(char*, int);	
		GETSHAFUNC func = (GETSHAFUNC)GetProcAddress(m_hiSha256, "axEncrypt");
		
		if (func)
		{
			strRetrun =  (char*)func(sPswd.GetBuffer(sPswd.GetLength()),sPswd.GetLength());
		}
	}	
	return strRetrun;
}

