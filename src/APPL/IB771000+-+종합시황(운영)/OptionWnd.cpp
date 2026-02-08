// OptionWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB771000.h"
#include "OptionWnd.h"
#include "MapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionWnd
#pragma warning (disable : 26409)
COptionWnd::COptionWnd(CMapDlg *pParent)
{
	m_bDaetoo = TRUE;
	m_pMapDlg = pParent;

	m_pFont		= NULL;
	
	m_sRoot.Empty();
	m_sUser.Empty();
	m_sFile.Empty();
	m_sSection.Empty();
	
	// 색상
	m_clrHanNews		= COLOR_BLACK;			
	m_clrEDaily		= COLOR_BLACK;
	m_clrInfoStock		= COLOR_BLACK;
	m_clrMoneyToday		= COLOR_BLACK;
	m_clrYounhap		= COLOR_BLACK;			
	m_clrMaekyoung		= COLOR_BLACK;
	m_clrEToday		= COLOR_BLACK;
	m_clrNewsp		= COLOR_BLACK;

	// 본문조회
	m_bContextSearch	= FALSE;	// TRUE : 본문조회 포함, FALSE : 제목만 조회
	
	// Color Picker
	m_pHanNews_Picker	= NULL;
	m_pEDaily_Picker	= NULL;
	m_pInfoStock_Picker	= NULL;
	m_pMoneytoday_Picker	= NULL;
	m_pYounhap_Picker	= NULL;
	m_pMaekyoung_Picker	= NULL;
	m_pEToday_Picker	= NULL;
	m_pNewsp_Picker	= NULL;

	m_pHanNew_Static	= NULL;
	m_pEDaily_Static	= NULL;
	m_pInfoStock_Static	= NULL;
	m_pMoneytoday_Static	= NULL;
	m_pYounhap_Static	= NULL;
	m_pMaekyoung_Static	= NULL;
	m_pEToday_Static	= NULL;
	m_pNewsp_Static		= NULL;

	m_rcHanNesw_Letter.SetRectEmpty();
	m_rcEDaily_Letter.SetRectEmpty();
	m_rcInfoStock_Letter.SetRectEmpty();
	m_rcMoneyStock_Letter.SetRectEmpty();
	m_rcYounhap_Letter.SetRectEmpty();
	m_rcMaekyoung_Letter.SetRectEmpty();
	m_rcEToday_Letter.SetRectEmpty();
	m_rcNewsp_Letter.SetRectEmpty();


	m_pContext_Check	= NULL;
}

COptionWnd::~COptionWnd()
{
}


BEGIN_MESSAGE_MAP(COptionWnd, CWnd)
	//{{AFX_MSG_MAP(COptionWnd)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(CPN_SELENDOK, OnSelEndOK)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COptionWnd message handlers

void COptionWnd::Init(CString root, CString user)
{
	m_sRoot		= root;
	m_sUser		= user;
	m_sSection	= SECTION_OPTION;
	m_sFile.Format("%s/%s/%s/%s", m_sRoot, USRDIR, m_sUser, NEWS_CHECK_FILE);

	// 뉴스별 색상 초기화
	m_pHanNews_Picker->Color		= GetColorData(idHanNews);
	m_pEDaily_Picker->Color			= GetColorData(idEDaily);
	m_pInfoStock_Picker->Color		= GetColorData(idInfoStock);
	if (m_bDaetoo)	// 20071122
	{
		m_pMoneytoday_Picker->Color	= GetColorData(idMoneyToday);

		m_pYounhap_Picker->Color	= GetColorData(idYounhap);
		m_pMaekyoung_Picker->Color	= GetColorData(idMaekyoung);
		m_pEToday_Picker->Color		= GetColorData(idEToday);
		m_pNewsp_Picker->Color		= GetColorData(idNewsp);
	}

	// 본문조회 체크 초기화
	LoadCheckContext();
}

void COptionWnd::OnDestroy() 
{
	CWnd::OnDestroy();

	// Save Check/Color
	SaveCheckContext();
	SetColorData(m_clrHanNews, idHanNews);
	SetColorData(m_clrEDaily, idEDaily);
	SetColorData(m_clrInfoStock, idInfoStock);
	SetColorData(m_clrMoneyToday, idMoneyToday);

	SetColorData(m_clrYounhap, idYounhap);
	SetColorData(m_clrMaekyoung, idMaekyoung);
	SetColorData(m_clrEToday, idEToday);
	SetColorData(m_clrNewsp, idNewsp);

	// Color Picker
	if (m_pHanNews_Picker)		{ m_pHanNews_Picker->DestroyWindow();		delete m_pHanNews_Picker; }
	if (m_pEDaily_Picker)		{ m_pEDaily_Picker->DestroyWindow();		delete m_pEDaily_Picker; }
	if (m_pInfoStock_Picker)	{ m_pInfoStock_Picker->DestroyWindow();		delete m_pInfoStock_Picker; }
	if (m_pMoneytoday_Picker)	{ m_pMoneytoday_Picker->DestroyWindow();	delete m_pMoneytoday_Picker; }
	if (m_pYounhap_Picker)		{ m_pYounhap_Picker->DestroyWindow();		delete m_pYounhap_Picker; }
	if (m_pMaekyoung_Picker)	{ m_pMaekyoung_Picker->DestroyWindow();		delete m_pMaekyoung_Picker; }
	if (m_pEToday_Picker)		{ m_pEToday_Picker->DestroyWindow();		delete m_pEToday_Picker; }
	if (m_pNewsp_Picker)		{ m_pNewsp_Picker->DestroyWindow();		delete m_pNewsp_Picker; }

	if (m_pContext_Check)		{ m_pContext_Check->DestroyWindow();		delete m_pContext_Check; }

	if (m_pHanNew_Static)		{ m_pHanNew_Static->DestroyWindow();		delete m_pHanNew_Static; }
	if (m_pEDaily_Static)		{ m_pEDaily_Static->DestroyWindow();		delete m_pEDaily_Static; }
	if (m_pInfoStock_Static)	{ m_pInfoStock_Static->DestroyWindow();		delete m_pInfoStock_Static; }
	if (m_pMoneytoday_Static)	{ m_pMoneytoday_Static->DestroyWindow();	delete m_pMoneytoday_Static; }
	if (m_pYounhap_Static)		{ m_pYounhap_Static->DestroyWindow();		delete m_pYounhap_Static; }
	if (m_pMaekyoung_Static)	{ m_pMaekyoung_Static->DestroyWindow();		delete m_pMaekyoung_Static; }
	if (m_pEToday_Static)		{ m_pEToday_Static->DestroyWindow();		delete m_pEToday_Static; }
	if (m_pNewsp_Static)		{ m_pNewsp_Static->DestroyWindow();		delete m_pNewsp_Static; }
}


int COptionWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	int	sx = 0, sy = 0, sy2 = 0, ex = 0, ey = 0, ey2 = 0;
	CRect	rc; const CRect rcItem;
	GetClientRect(&rc);

	SetBrush(PALETTERGB(230, 230, 230));
	m_pFont = m_pMapDlg->GetAxFont(9,false,"굴림체");
	
	sx = GAP5;		sy = GAP3;		ex = sx + STATIC_WIDTH2;		ey = sy + STATIC_HEIGHT;
	m_pContext_Check = new CButton();
	m_pContext_Check->Create(NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, CRect(sx+3, sy, ex, ey), this, IDC_CHECK_CONTEXT);
	m_pContext_Check->SetWindowText("본문자동조회");
	m_pContext_Check->SetCheck(FALSE);
	m_bContextSearch = FALSE;
	m_pContext_Check->SetFont(m_pFont);

	// 한경뉴스
	sx = GAP3;	sy = ey + GAP5+4;		ex = sx + STATIC_WIDTH1;		ey = sy + STATIC_HEIGHT;
	m_pHanNew_Static = new CStatic();
	m_pHanNew_Static->Create(" 한경뉴스", WS_CHILD|WS_VISIBLE|SS_LEFT|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(sx, sy+2, ex, ey-3), this);
	m_pHanNew_Static->SetFont(m_pFont);

	sx = rc.right - PICKER_WIDTH - GAP10;	sy2 = sy - GAP5;	ex = sx + PICKER_WIDTH;		ey2 = sy2 + PICKER_HEIGHT;
	m_pHanNews_Picker = new CColorButton;
	m_pHanNews_Picker->Create("", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect(sx, sy2, ex, ey2), this, IDC_PICKER_HANNEWS);
	m_pHanNews_Picker->DefaultColor		= ::GetSysColor(COLOR_APPWORKSPACE);
	m_pHanNews_Picker->TrackSelection	= TRUE;
	m_pHanNews_Picker->CustomText		= _T("사용자 정의");
	m_pHanNews_Picker->DefaultText		= _T("자동");
	
	// 이데일리
	sx = GAP3;	sy = ey;		ex = sx + STATIC_WIDTH1;		ey = sy + STATIC_HEIGHT;
	m_pEDaily_Static = new CStatic();
	m_pEDaily_Static->Create(" 이데일리", WS_CHILD|WS_VISIBLE|SS_LEFT|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(sx, sy+2, ex, ey-3), this);
	m_pEDaily_Static->SetFont(m_pFont);

	sx = rc.right - PICKER_WIDTH - GAP10;	sy2 = ey2 + GAP5;	ex = sx + PICKER_WIDTH;		ey2 = sy2 + PICKER_HEIGHT;
	m_pEDaily_Picker = new CColorButton;
	m_pEDaily_Picker->Create("", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect(sx, sy2, ex, ey2), this, IDC_PICKER_EDAILY);
	m_pEDaily_Picker->DefaultColor		= ::GetSysColor(COLOR_APPWORKSPACE);
	m_pEDaily_Picker->TrackSelection	= TRUE;
	m_pEDaily_Picker->CustomText		= _T("사용자 정의");
	m_pEDaily_Picker->DefaultText		= _T("자동");

	// 인포스탁
	sx = GAP3;	sy = ey;		ex = sx + STATIC_WIDTH1;		ey = sy + STATIC_HEIGHT;
	m_pInfoStock_Static = new CStatic();
	m_pInfoStock_Static->Create(" 인포스탁", WS_CHILD|WS_VISIBLE|SS_LEFT|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(sx, sy+2, ex, ey-3), this);
	m_pInfoStock_Static->SetFont(m_pFont);

	sx = rc.right - PICKER_WIDTH - GAP10;	sy2 = ey2 + GAP5;	ex = sx + PICKER_WIDTH;		ey2 = sy2 + PICKER_HEIGHT;
	m_pInfoStock_Picker = new CColorButton;
	m_pInfoStock_Picker->Create("", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect(sx, sy2, ex, ey2), this, IDC_PICKER_INFOSTOCK);
	m_pInfoStock_Picker->DefaultColor		= ::GetSysColor(COLOR_APPWORKSPACE);
	m_pInfoStock_Picker->TrackSelection	= TRUE;
	m_pInfoStock_Picker->CustomText		= _T("사용자 정의");
	m_pInfoStock_Picker->DefaultText		= _T("자동");

	if (m_bDaetoo)	// 20071122
	{
		// 머니투데이
		sx = GAP3;	sy = ey;			ex = sx + STATIC_WIDTH1;		ey = sy + STATIC_HEIGHT;
		m_pMoneytoday_Static = new CStatic();
		m_pMoneytoday_Static->Create("머니투데이", WS_CHILD|WS_VISIBLE|SS_LEFT|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(sx, sy+2, ex, ey-3), this);
		m_pMoneytoday_Static->SetFont(m_pFont);
		
		sx = rc.right - PICKER_WIDTH - GAP10;	sy2 = ey2 + GAP5;	ex = sx + PICKER_WIDTH;		ey2 = sy2 + PICKER_HEIGHT;
		m_pMoneytoday_Picker = new CColorButton;
		m_pMoneytoday_Picker->Create("", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect(sx, sy2, ex, ey2), this, IDC_PICKER_MONEYTODAY);
		m_pMoneytoday_Picker->DefaultColor		= ::GetSysColor(COLOR_APPWORKSPACE);
		m_pMoneytoday_Picker->TrackSelection	= TRUE;
		m_pMoneytoday_Picker->CustomText		= _T("사용자 정의");
		m_pMoneytoday_Picker->DefaultText		= _T("자동");

		// 연합
		sx = GAP3;	sy = ey;		ex = sx + STATIC_WIDTH1;		ey = sy + STATIC_HEIGHT;
		m_pYounhap_Static = new CStatic();
		m_pYounhap_Static->Create(" 연합뉴스", WS_CHILD|WS_VISIBLE|SS_LEFT|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(sx, sy+2, ex, ey-3), this);
		m_pYounhap_Static->SetFont(m_pFont);
		
		sx = rc.right - PICKER_WIDTH - GAP10;	sy2 = ey2 + GAP5;	ex = sx + PICKER_WIDTH;		ey2 = sy2 + PICKER_HEIGHT;
		m_pYounhap_Picker = new CColorButton;
		m_pYounhap_Picker->Create("", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect(sx, sy2, ex, ey2), this, IDC_PICKER_YOUNHAP);
		m_pYounhap_Picker->DefaultColor		= ::GetSysColor(COLOR_APPWORKSPACE);
		m_pYounhap_Picker->TrackSelection	= TRUE;
		m_pYounhap_Picker->CustomText		= _T("사용자 정의");
		m_pMoneytoday_Picker->DefaultText		= _T("자동");

		// 매일경제
		sx = GAP3;	sy = ey;		ex = sx + STATIC_WIDTH1;		ey = sy + STATIC_HEIGHT;
		m_pMaekyoung_Static = new CStatic();
		m_pMaekyoung_Static->Create(" 매일경제", WS_CHILD|WS_VISIBLE|SS_LEFT|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(sx, sy+2, ex, ey-3), this);
		m_pMaekyoung_Static->SetFont(m_pFont);
		
		sx = rc.right - PICKER_WIDTH - GAP10;	sy2 = ey2 + GAP5;	ex = sx + PICKER_WIDTH;		ey2 = sy2 + PICKER_HEIGHT;
		m_pMaekyoung_Picker = new CColorButton;
		m_pMaekyoung_Picker->Create("", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect(sx, sy2, ex, ey2), this, IDC_PICKER_MAEKYOUNG);
		m_pMaekyoung_Picker->DefaultColor		= ::GetSysColor(COLOR_APPWORKSPACE);
		m_pMaekyoung_Picker->TrackSelection	= TRUE;
		m_pMaekyoung_Picker->CustomText		= _T("사용자 정의");
		m_pMaekyoung_Picker->DefaultText		= _T("자동");

		// 이투데이
		sx = GAP3;	sy = ey;		ex = sx + STATIC_WIDTH1;		ey = sy + STATIC_HEIGHT;
		m_pEToday_Static = new CStatic();
		m_pEToday_Static->Create(" 이투데이", WS_CHILD|WS_VISIBLE|SS_LEFT|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(sx, sy+2, ex, ey-3), this);
		m_pEToday_Static->SetFont(m_pFont);
		
		sx = rc.right - PICKER_WIDTH - GAP10;	sy2 = ey2 + GAP5;	ex = sx + PICKER_WIDTH;		ey2 = sy2 + PICKER_HEIGHT;
		m_pEToday_Picker = new CColorButton;
		m_pEToday_Picker->Create("", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect(sx, sy2, ex, ey2), this, IDC_PICKER_ETODAY);
		m_pEToday_Picker->DefaultColor		= ::GetSysColor(COLOR_APPWORKSPACE);
		m_pEToday_Picker->TrackSelection	= TRUE;
		m_pEToday_Picker->CustomText		= _T("사용자 정의");
		m_pEToday_Picker->DefaultText		= _T("자동");

		// 뉴스핌
		sx = GAP3;	sy = ey;		ex = sx + STATIC_WIDTH1;		ey = sy + STATIC_HEIGHT;
		m_pNewsp_Static = new CStatic();
		m_pNewsp_Static->Create(" 뉴스핌", WS_CHILD|WS_VISIBLE|SS_LEFT|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(sx, sy+2, ex, ey-3), this);
		m_pNewsp_Static->SetFont(m_pFont);
		
		sx = rc.right - PICKER_WIDTH - GAP10;	sy2 = ey2 + GAP5;	ex = sx + PICKER_WIDTH;		ey2 = sy2 + PICKER_HEIGHT;
		m_pNewsp_Picker = new CColorButton;
		m_pNewsp_Picker->Create("", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect(sx, sy2, ex, ey2), this, IDC_PICKER_NEWSP);
		m_pNewsp_Picker->DefaultColor		= ::GetSysColor(COLOR_APPWORKSPACE);
		m_pNewsp_Picker->TrackSelection	= TRUE;
		m_pNewsp_Picker->CustomText		= _T("사용자 정의");
		m_pNewsp_Picker->DefaultText		= _T("자동");
	}

	return 0;
}

BOOL COptionWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	switch (LOWORD(wParam))
	{
	case IDC_CHECK_CONTEXT:
		m_bContextSearch = !m_bContextSearch;
		SaveCheckContext();
		m_pMapDlg->SendMessage(WM_MANAGE, (WPARAM)MSG_OPTION_CHECK, 0);
		break;
	}

	return CWnd::OnCommand(wParam, lParam);
}

BOOL COptionWnd::GetCheckContext()
{
	return m_bContextSearch;
}

void COptionWnd::SaveCheckContext()
{
	CString sData;

	if (m_bContextSearch)
		sData = "TRUE";
	else
		sData = "FALSE";

	WritePrivateProfileString(m_sSection, KEY_CONTEXT, sData, m_sFile);
}

void COptionWnd::LoadCheckContext()
{
	char	wb[32];
	CString sData;

	GetPrivateProfileString(m_sSection, KEY_CONTEXT, DEFAULT_CHECK, wb, sizeof(wb), m_sFile);
	sData = CString(wb, sizeof(wb));
	
	if (sData == "TRUE")
	{
		m_bContextSearch = TRUE;
		m_pContext_Check->SetCheck(TRUE);
	}
	else
	{
		m_bContextSearch = FALSE;
		m_pContext_Check->SetCheck(FALSE);
	}
}

COLORREF COptionWnd::GetColorData(idPicker idContent)
{
	char		wb[32]{};
	CString		sRed, sGreen, sBlue, sData, sKey;
	COLORREF	selColor = COLOR_BLACK;

	switch (idContent)
	{
	case idHanNews		: sKey = KEY_HANNEWS;		break;
	case idEDaily		: sKey = KEY_EDAILY;		break;
	case idInfoStock	: sKey = KEY_INFOSTOCK;		break;
	case idMoneyToday	: sKey = KEY_MONEYTODAY;	break;
	case idYounhap		: sKey = KEY_YOUNHAP;		break;
	case idMaekyoung	: sKey = KEY_MAEKYOUNG;		break;
	case idEToday		: sKey = KEY_ETODAY;		break;
	case idNewsp		: sKey = KEY_NEWSP;		break;

	}

	GetPrivateProfileString(m_sSection, sKey, DEFAULT_COLOR, wb, sizeof(wb), m_sFile);
	sData = CString(wb, sizeof(wb));
	
	sRed = Parser(sData, SIGN_SEPER);
	sGreen = Parser(sData, SIGN_SEPER);
	sBlue = sData;
	selColor = PALETTERGB(atoi(sRed), atoi(sGreen), atoi(sBlue));

	switch (idContent)
	{
	case idHanNews		: m_clrHanNews = selColor;		break;
	case idEDaily		: m_clrEDaily = selColor;		break;
	case idInfoStock	: m_clrInfoStock = selColor;		break;
	case idMoneyToday	: m_clrMoneyToday = selColor;		break;
	case idYounhap		: m_clrYounhap = selColor;		break;
	case idMaekyoung	: m_clrMaekyoung = selColor;		break;
	case idEToday		: m_clrEToday = selColor;		break;
	case idNewsp		: m_clrNewsp = selColor;		break;
	}

	return selColor;
}

void COptionWnd::SetColorData(COLORREF color, idPicker idContent)
{
	BYTE	r{}, g{}, b{};
	CString	sData, sKey;
	
	switch (idContent)
	{
	case idHanNews		: sKey = KEY_HANNEWS;		break;
	case idEDaily		: sKey = KEY_EDAILY;		break;
	case idInfoStock	: sKey = KEY_INFOSTOCK;		break;
	case idMoneyToday	: sKey = KEY_MONEYTODAY;	break;
	case idYounhap		: sKey = KEY_YOUNHAP;		break;
	case idMaekyoung	: sKey = KEY_MAEKYOUNG;		break;
	case idEToday		: sKey = KEY_ETODAY;		break;
	case idNewsp		: sKey = KEY_NEWSP;		break;
	}	

	r = (BYTE) (color);
	g = (BYTE) (color >> 8);
	b = (BYTE) (color >> 16);
	
	sData.Format("%d, %d, %d", r, g, b);		
	
	WritePrivateProfileString(m_sSection, sKey, sData, m_sFile);
}

LONG COptionWnd::OnSelEndOK(UINT lParam, LONG wParam)
{
	COLORREF SelColor;
	SelColor = (DWORD)lParam;
	
	switch (wParam)
	{
	case IDC_PICKER_HANNEWS: 
		m_clrHanNews = SelColor;
		SetColorData(m_clrHanNews, idHanNews);
		break;
	case IDC_PICKER_EDAILY: 
		m_clrEDaily = SelColor;		
		SetColorData(m_clrEDaily, idEDaily);
		break;
	case IDC_PICKER_INFOSTOCK: 
		m_clrInfoStock = SelColor;
		SetColorData(m_clrInfoStock, idInfoStock);
		break;
	case IDC_PICKER_MONEYTODAY: 
		m_clrMoneyToday = SelColor;	
		SetColorData(m_clrMoneyToday, idMoneyToday);
		break;
	case IDC_PICKER_YOUNHAP: 
		m_clrYounhap = SelColor;		
		SetColorData(m_clrYounhap, idYounhap);
		break;
	case IDC_PICKER_MAEKYOUNG: 
		m_clrMaekyoung = SelColor;	
		SetColorData(m_clrMaekyoung, idMaekyoung);
		break;
	case IDC_PICKER_ETODAY: 
		m_clrEToday = SelColor;		
		SetColorData(m_clrEToday, idEToday);
		break;
	case IDC_PICKER_NEWSP: 
		m_clrNewsp = SelColor;		
		SetColorData(m_clrNewsp, idNewsp);
		break;
	}
	
	m_pMapDlg->SendMessage(WM_MANAGE, (WPARAM)MSG_COLOR_CHANGE, 0);
	
	return TRUE;
}


HBRUSH COptionWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_BTN || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)m_brBk;
	}

	if (pWnd->GetSafeHwnd() == m_pContext_Check->GetSafeHwnd())
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)m_brBk;
	}
	
	return hbr;
}


BOOL COptionWnd::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
}

void COptionWnd::OnPaint() 
{
	CPaintDC dc(this);

	CRect rc;
	GetClientRect(&rc);
	DrawBkgrd(&dc,rc);
}

COLORREF COptionWnd::GetNewsColor(idPicker idContent)
{
	COLORREF color = COLOR_BLACK;

	switch (idContent)
	{
	case idHanNews		: color = m_clrHanNews;		break;
	case idEDaily		: color = m_clrEDaily;		break;
	case idInfoStock	: color = m_clrInfoStock;	break;
	case idMoneyToday	: color = m_clrMoneyToday;	break;
	case idYounhap		: color = m_clrYounhap;		break;
	case idMaekyoung	: color = m_clrMaekyoung;	break;
	case idEToday		: color = m_clrEToday;		break;
	case idNewsp		: color = m_clrNewsp;		break;
	}
	
	return color;
}
#pragma warning (default : 26409)
	