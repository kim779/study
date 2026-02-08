#include "stdafx.h"
#include "IB401300.h"
#include "MapWnd.h"
#include "time.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 26409)
#pragma warning(disable : 26400)
#pragma warning(disable : 26401)

struct _OGRID_HEADER {
	LPCSTR name;
	DWORD format;
	DWORD width;
	COLORREF clBk;
};

#define CL_ATMG		RGB(0x94,0x94,0xD2)
#define CL_GRAY		RGB(0xE0,0xE0,0xE0)
#define CL_RATM		RGB(0xFF,0xFF,0x00)
#define DF_HEADER	(DT_SINGLELINE|DT_CENTER|DT_VCENTER)
#define DF_NUMBER	(DT_SINGLELINE|DT_RIGHT|DT_VCENTER)
struct _OGRID_HEADER OGRID_HEADER[SCR_TYPES + 5][30] = {
	{	// SCR_TYPE1 (옵션종합현재가)
		{"IV",		DF_NUMBER, 40, 0},
		{"증감",	DF_NUMBER, 50, 0},
		{"미결제",	DF_NUMBER, 54, 0},
		{"거래량",	DF_NUMBER, 63, 0},
		{"매수",	DF_NUMBER, 40, 0},
		{"매도",	DF_NUMBER, 40, 0},
		{"전일비",	DF_NUMBER, 60, 0},
		{"현재가",	DF_NUMBER, 57, 0},
		{"행사가",	DF_NUMBER, 88, CL_GRAY},	// sum 445
		{"지수환산",DF_NUMBER, 88, CL_GRAY},
		{"현재가",	DF_NUMBER, 57, 0},
		{"전일비",	DF_NUMBER, 60, 0},
		{"매도",	DF_NUMBER, 40, 0},
		{"매수",	DF_NUMBER, 40, 0},
		{"거래량",	DF_NUMBER, 63, 0},
		{"미결제",	DF_NUMBER, 54, 0},
		{"증감",	DF_NUMBER, 50, 0},
		{"IV",		DF_NUMBER, 40, 0},	// sum 445
		{NULL, NULL, NULL, NULL}
	},
	{
		{"거래량",	DF_NUMBER, 63, 0},
		{"괴리도",	DF_NUMBER, 54, 0},
		{"이론가",	DF_NUMBER, 54, 0},
		{"시간가치",DF_NUMBER, 54, 0},
		{"등락율",	DF_NUMBER, 54, 0},
		{"전일비",	DF_NUMBER, 64, 0},
		{"현재가",	DF_NUMBER, 61, 0},
		{"행사가",	DF_NUMBER, 88, CL_GRAY},
		{"지수환산",DF_NUMBER, 88, CL_GRAY},
		{"현재가",	DF_NUMBER, 61, 0},
		{"전일비",	DF_NUMBER, 64, 0},
		{"등락율",	DF_NUMBER, 54, 0},
		{"시간가치",DF_NUMBER, 54, 0},
		{"이론가",	DF_NUMBER, 54, 0},
		{"괴리도",	DF_NUMBER, 54, 0},
		{"거래량",	DF_NUMBER, 63, 0},
		{NULL, NULL, NULL, NULL}
	},
	{
		{"로우",	DF_NUMBER, 55, 0},
		{"베가",	DF_NUMBER, 55, 0},
		{"쎄타",	DF_NUMBER, 55, 0},
		{"감마",	DF_NUMBER, 55, 0},
		{"델타",	DF_NUMBER, 55, 0},
		{"전일비",	DF_NUMBER, 65, 0},
		{"현재가",	DF_NUMBER, 64, 0},
		{"행사가",	DF_NUMBER, 88, CL_GRAY},
		{"지수환산",DF_NUMBER, 88, CL_GRAY},
		{"현재가",	DF_NUMBER, 64, 0},
		{"전일비",	DF_NUMBER, 65, 0},
		{"델타",	DF_NUMBER, 55, 0},
		{"감마",	DF_NUMBER, 55, 0},
		{"쎄타",	DF_NUMBER, 55, 0},
		{"베가",	DF_NUMBER, 55, 0},
		{"로우",	DF_NUMBER, 55, 0},
		{NULL, NULL, NULL, NULL}
	},
	{
		{"로우",	DF_NUMBER, 55, 0},
		{"베가",	DF_NUMBER, 55, 0},
		{"쎄타",	DF_NUMBER, 55, 0},
		{"감마",	DF_NUMBER, 55, 0},
		{"델타",	DF_NUMBER, 55, 0},
		{"전일비",	DF_NUMBER, 65, 0},
		{"현재가",	DF_NUMBER, 64, 0},
		{"행사가",	DF_NUMBER, 88, CL_GRAY},
		{"지수환산",DF_NUMBER, 88, CL_GRAY},
		{"현재가",	DF_NUMBER, 64, 0},
		{"전일비",	DF_NUMBER, 65, 0},
		{"델타",	DF_NUMBER, 55, 0},
		{"감마",	DF_NUMBER, 55, 0},
		{"쎄타",	DF_NUMBER, 55, 0},
		{"베가",	DF_NUMBER, 55, 0},
		{"로우",	DF_NUMBER, 55, 0},
		{NULL, NULL, NULL, NULL}
	},
	{	// 옵션시고저
		{"저가",	DF_NUMBER, 55, 0},
		{"고가",	DF_NUMBER, 55, 0},
		{"시가",	DF_NUMBER, 55, 0},
		{"미결재",	DF_NUMBER, 55, 0},
		{"거래량",	DF_NUMBER, 55, 0},
		{"전일비",	DF_NUMBER, 65, 0},
		{"현재가",	DF_NUMBER, 65, 0},
		{"행사가",	DF_NUMBER, 88, CL_GRAY},	// sum 445
		{"지수환산",DF_NUMBER, 88, CL_GRAY},
		{"현재가",	DF_NUMBER, 65, 0},
		{"전일비",	DF_NUMBER, 65, 0},
		{"거래량",	DF_NUMBER, 55, 0},
		{"미결재",	DF_NUMBER, 55, 0},
		{"시가",	DF_NUMBER, 55, 0},
		{"고가",	DF_NUMBER, 55, 0},
		{"저가",	DF_NUMBER, 55, 0},
		{NULL, NULL, NULL, NULL}
	},
	{	// (옵션종합예상가)
		{"IV",		DF_NUMBER, 40, 0},
		{"증감",	DF_NUMBER, 50, 0},
		{"미결제",	DF_NUMBER, 54, 0},
		{"예상량",	DF_NUMBER, 63, 0},
		{"매수",	DF_NUMBER, 40, 0},
		{"매도",	DF_NUMBER, 40, 0},
		{"전일비",	DF_NUMBER, 60, 0},
		{"예상가",	DF_NUMBER, 57, 0},
		{"행사가",	DF_NUMBER, 88, CL_GRAY},	// sum 445
		{"지수환산",DF_NUMBER, 88, CL_GRAY},
		{"예상가",	DF_NUMBER, 57, 0},
		{"전일비",	DF_NUMBER, 60, 0},
		{"매도",	DF_NUMBER, 40, 0},
		{"매수",	DF_NUMBER, 40, 0},
		{"예상량",	DF_NUMBER, 63, 0},
		{"미결제",	DF_NUMBER, 54, 0},
		{"증감",	DF_NUMBER, 50, 0},
		{"IV",		DF_NUMBER, 40, 0},	// sum 445
		{NULL, NULL, NULL, NULL}
	},
	{
		{"예상량",	DF_NUMBER, 63, 0},
		{"괴리도",	DF_NUMBER, 54, 0},
		{"이론가",	DF_NUMBER, 54, 0},
		{"시간가치",DF_NUMBER, 54, 0},
		{"등락율",	DF_NUMBER, 54, 0},
		{"전일비",	DF_NUMBER, 64, 0},
		{"예상가",	DF_NUMBER, 61, 0},
		{"행사가",	DF_NUMBER, 88, CL_GRAY},
		{"지수환산",DF_NUMBER, 88, CL_GRAY},
		{"예상가",	DF_NUMBER, 61, 0},
		{"전일비",	DF_NUMBER, 64, 0},
		{"등락율",	DF_NUMBER, 54, 0},
		{"시간가치",DF_NUMBER, 54, 0},
		{"이론가",	DF_NUMBER, 54, 0},
		{"괴리도",	DF_NUMBER, 54, 0},
		{"예상량",	DF_NUMBER, 63, 0},
		{NULL, NULL, NULL, NULL}
	},
	{
		{"로우",	DF_NUMBER, 55, 0},
		{"베가",	DF_NUMBER, 55, 0},
		{"쎄타",	DF_NUMBER, 55, 0},
		{"감마",	DF_NUMBER, 55, 0},
		{"델타",	DF_NUMBER, 55, 0},
		{"전일비",	DF_NUMBER, 65, 0},
		{"예상가",	DF_NUMBER, 64, 0},
		{"행사가",	DF_NUMBER, 88, CL_GRAY},
		{"지수환산",DF_NUMBER, 88, CL_GRAY},
		{"예상가",	DF_NUMBER, 64, 0},
		{"전일비",	DF_NUMBER, 65, 0},
		{"델타",	DF_NUMBER, 55, 0},
		{"감마",	DF_NUMBER, 55, 0},
		{"쎄타",	DF_NUMBER, 55, 0},
		{"베가",	DF_NUMBER, 55, 0},
		{"로우",	DF_NUMBER, 55, 0},
		{NULL, NULL, NULL, NULL}
	}
	,
	{
		{"로우",	DF_NUMBER, 55, 0},
		{"베가",	DF_NUMBER, 55, 0},
		{"쎄타",	DF_NUMBER, 55, 0},
		{"감마",	DF_NUMBER, 55, 0},
		{"델타",	DF_NUMBER, 55, 0},
		{"전일비",	DF_NUMBER, 65, 0},
		{"예상가",	DF_NUMBER, 64, 0},
		{"행사가",	DF_NUMBER, 88, CL_GRAY},
		{"지수환산",DF_NUMBER, 88, CL_GRAY},
		{"예상가",	DF_NUMBER, 64, 0},
		{"전일비",	DF_NUMBER, 65, 0},
		{"델타",	DF_NUMBER, 55, 0},
		{"감마",	DF_NUMBER, 55, 0},
		{"쎄타",	DF_NUMBER, 55, 0},
		{"베가",	DF_NUMBER, 55, 0},
		{"로우",	DF_NUMBER, 55, 0},
		{NULL, NULL, NULL, NULL}
	},
	{	// 옵션시고저
		{"저가",	DF_NUMBER, 55, 0},
		{"고가",	DF_NUMBER, 55, 0},
		{"시가",	DF_NUMBER, 55, 0},
		{"미결재",	DF_NUMBER, 55, 0},
		{"예상량",	DF_NUMBER, 55, 0},
		{"전일비",	DF_NUMBER, 65, 0},
		{"예상가",	DF_NUMBER, 65, 0},
		{"행사가",	DF_NUMBER, 88, CL_GRAY},	// sum 445
		{"지수환산",DF_NUMBER, 88, CL_GRAY},
		{"예상가",	DF_NUMBER, 65, 0},
		{"전일비",	DF_NUMBER, 65, 0},
		{"예상량",	DF_NUMBER, 55, 0},
		{"미결재",	DF_NUMBER, 55, 0},
		{"시가",	DF_NUMBER, 55, 0},
		{"고가",	DF_NUMBER, 55, 0},
		{"저가",	DF_NUMBER, 55, 0},
		{NULL, NULL, NULL, NULL}
	}
};

struct _OGRID_HEADER FGRID_HEADER[2][30] = {
	{	// SCR_TYPE1 (옵션종합현재가)
		{"선물",		DF_HEADER, 75, 0},
		{"종목명",		DF_HEADER, 125, 0},
		{"현재가",		DF_NUMBER, 75, 0},
		{"대비",		DF_NUMBER, 75, 0},
		{"등락율",		DF_NUMBER, 70, 0},
		{"거래량",		DF_NUMBER, 75, 0},
		{"이론가",		DF_NUMBER, 75, 0},
		{"괴리율",		DF_NUMBER, 75, 0},
		{"BASIS",		DF_NUMBER, 70, 0},
		{"이론BASIS",	DF_NUMBER, 70, 0},
		{"미결제약정",	DF_NUMBER, 75, 0},
		{"미결증감",	DF_NUMBER, 75, 0},
		{NULL, NULL, NULL, NULL}
	},
	{	// SCR_TYPE1 (옵션종합현재가)
		{"선물",		DF_HEADER, 75, 0},
		{"종목명",		DF_HEADER, 95, 0},
		{"예상가",		DF_NUMBER, 75, 0},
		{"대비",		DF_NUMBER, 70, 0},
		{"등락률",		DF_NUMBER, 75, 0},
		{"예상량",		DF_NUMBER, 75, 0},
		{"이론가",		DF_NUMBER, 75, 0},
		{"괴리율",		DF_NUMBER, 75, 0},
		{"BASIS",		DF_NUMBER, 70, 0},
		{"이론BASIS",	DF_NUMBER, 70, 0},
		{"미결제약정",	DF_NUMBER, 75, 0},
		{"미결증감",	DF_NUMBER, 75, 0},
		{NULL, NULL, NULL, NULL}
	}
};

CMapWnd::CMapWnd(CWnd *pParent) : CAxisExt(pParent)
{
	m_stMonth = NULL;
	m_cbMonth = NULL;
	for(int n=0; n<SCR_TYPES; ++n)
		m_rdType[n] = NULL;
	m_btnConf = NULL;
	m_btnGreek = NULL;
	m_stCall = m_stPut = NULL;
	m_gdFuture = m_gdOption = NULL;

	m_style = SCR_TYPE1;
	m_AtmRow = m_RAtmRow = m_OptionCurRow = m_FutureCurRow = -1;

	m_cdkr = m_bjis = m_clday = m_bzday = m_clday2 = m_bzday2 = m_basic = 0.0;

	m_crEven = GetIndexColor(68);
	m_crOdd = GetIndexColor(77);

	m_home = Variant(homeCC, _T(""));
	m_user = Variant(userCC, _T(""));
	m_name = Variant(nameCC, _T(""));

	m_cCX = m_cCY = m_cStyle = 0;

	m_nIndex = 0;

	m_bClose = FALSE;

	strPrevHour = "0";
	strPrevMin = "0";
	strPrevSec = "0";

	Variant(titleCC, _T("[4013] 옵션 월물별 종합시세표"));
	
	m_pBrBk = GetAxBrush(m_crCtrlBk);

	LoadImage();
	LoadScreenConfig();
}

CMapWnd::~CMapWnd()
{
}

BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SCRTYPE+0, OnScreenType1)
	ON_BN_CLICKED(IDC_SCRTYPE+1, OnScreenType2)
	ON_BN_CLICKED(IDC_SCRTYPE+2, OnScreenType3)
	ON_BN_CLICKED(IDC_SCRTYPE+3, OnScreenType4)
	ON_BN_CLICKED(IDC_SCRTYPE+4, OnScreenType5)
	ON_BN_CLICKED(IDC_BTN_GREEK, OnGreek)
	ON_BN_CLICKED(IDC_BTN_CONF, OnConf)
	ON_BN_CLICKED(IDC_CHECK_PREDICT, OnCheckPredict)
	ON_CBN_SELCHANGE(IDC_CBMONTH, OnMonthChange)
	ON_CBN_SELCHANGE(IDC_CBMARKET, OnMarketChange)
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()

CfxStatic* CMapWnd::CreateStatic( LPCTSTR lpszCaption, COLORREF colBk, COLORREF colFg, COLORREF colBorder )
{
//	CfxStatic* pSt = new CfxStatic(m_pFont);
	std::unique_ptr<CfxStatic> pSt = std::make_unique<CfxStatic>(m_pFont);
	if (!pSt) return NULL;

	if (pSt->Create(lpszCaption, WS_VISIBLE|WS_CHILD|SS_CENTER, CRect(0,0,0,0), this))
	{
		pSt->Init(ssCOLOR|(colBorder ? ssBORDER : 0), 0, lpszCaption, "", colBk, colBk, colFg);
		pSt->SetBorderColor(colBorder);
	}
	else 
	{
//		delete pSt;
		pSt = NULL;
	}
	return pSt.release();
}

CfxImgButton* CMapWnd::CreateImgButton( LPCSTR caption, int nID, CBitmap **bmp )
{
	std::unique_ptr<CfxImgButton> pBtn = std::make_unique<CfxImgButton>(m_pFont);
	
	if (pBtn->Create(caption, CRect(0, 0, 0, 0), this, nID, FALSE, TRUE))
	{
		pBtn->SetImgBitmap(	bmp[0]->operator HBITMAP(), bmp[1]->operator HBITMAP(), bmp[2]->operator HBITMAP() );
		return pBtn.release();
	}
	else return NULL;
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

	m_hSend[0] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\조회BTN.bmp")));
	m_hSend[1] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\조회BTN_DN.bmp")));
	m_hSend[2] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\조회BTN_EN.bmp")));

	m_hHist[0] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\ARROW.bmp")));
	m_hHist[1] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\ARROW_DN.bmp")));
	m_hHist[2] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\ARROW_EN.bmp")));
	
	m_hGuide     = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\알림_ic.bmp")));
}

void CMapWnd::DrawRoundRectangle( CDC *pDC, CRect rc, COLORREF colBack )
{
	const CSize size(2,2);
	pDC->FillSolidRect((LPRECT)rc, m_crBk);
	rc.DeflateRect(size);
	pDC->FillSolidRect((LPRECT)rc, colBack);
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
	
	DrawRoundRectangle(&mdc, m_rcCtrl, m_crCtrlBk);		// Control window
	DrawRoundRectangle(&mdc, m_rcBody, m_crBodyBk);		// Body window
		
	mdc.SelectObject(oldBmp);
	mdc.DeleteDC();
	
	ReleaseDC(pdc);
}

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this);
	Draw(&dc);
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

void CMapWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect rc;
	GetClientRect(&rc);
	
	if (m_cCX>0 && m_cCY>0)
	{
		m_pParent->PostMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(m_cCX, m_cCY));
		m_cCX = m_cCY = 0;
	}

	int pos_x = 0, pos_y = 0;
	const int line_h = 20;
	m_rcCtrl.SetRect(rc.left, rc.top, rc.right, rc.top+CONTROL_WND_HEIGHT);
	m_rcBody.SetRect(rc.left, rc.top+CONTROL_WND_HEIGHT, rc.right, rc.bottom);

	// Control
	{
		rc = m_rcCtrl;
		rc.DeflateRect(PADDING_VALUE, PADDING_VALUE);
		pos_x = rc.left;
		pos_y = rc.top;

		if (m_cbMarket)
		{
			m_cbMarket->MoveWindow(pos_x,pos_y, 80, line_h*3, FALSE);
			pos_x += 80 + PADDING_VALUE;
		}
		
		if (m_stMonth) {
			m_stMonth->MoveWindow(pos_x, pos_y, 60, line_h, FALSE); pos_x += 60 + PADDING_VALUE;
		}
		if (m_cbMonth) {
			m_cbMonth->MoveWindow(pos_x, pos_y, 80, line_h*11, FALSE); pos_x += 100 + PADDING_VALUE;
		}

		for(int n=0; n<SCR_TYPES; ++n)
		{
			if (!m_rdType[n]) continue;

			if(n < 3)
			{
				m_rdType[n]->MoveWindow(pos_x, pos_y, 110, line_h, FALSE);

				pos_x += 110 + PADDING_VALUE;
			}
			else
			{
				m_rdType[n]->MoveWindow(pos_x, pos_y, 80, line_h, FALSE);

				pos_x += 80 + PADDING_VALUE;
			}
		}
		
		pos_x = rc.right - 80;
		if (m_btnGreek) {
			m_btnGreek->MoveWindow(pos_x, pos_y, 80, line_h, FALSE); pos_x -= 40 + PADDING_VALUE;
		}
		if (m_btnConf) {
			m_btnConf->MoveWindow(pos_x, pos_y, 40, line_h, FALSE); pos_x -= 50 + PADDING_VALUE;
		}
		
		if (m_chkPrd) {
			m_chkPrd->MoveWindow(pos_x, pos_y, 50, line_h, FALSE);
		}
	}

	// Body 
	{
		rc = m_rcBody;
		rc.DeflateRect(PADDING_VALUE, PADDING_VALUE);
		//int pos_c = rc.left + 445;
		const int pos_c = rc.left + rc.Width() / 2;
		pos_x = rc.left;
		pos_y = rc.top;
		
		if (m_stCall) m_stCall->MoveWindow(pos_x, pos_y, pos_c-pos_x, line_h, FALSE);
		if (m_stPut ) m_stPut->MoveWindow(pos_c-1, pos_y, rc.right-pos_c+1, line_h, FALSE);

		CString s;
		s.Format("WIDTH [%d] [%d]\n",pos_c-pos_x,rc.right-pos_c+1);
		OutputDebugString(s);

		pos_y += line_h;
		if (m_gdOption) m_gdOption->MoveWindow(rc.left, pos_y, rc.Width(), rc.Height()-line_h-FUTURE_WND_HEIGHT-PADDING_VALUE);

		pos_y = rc.bottom - FUTURE_WND_HEIGHT;
		if (m_gdFuture) m_gdFuture->MoveWindow(rc.left, pos_y, rc.Width(), FUTURE_WND_HEIGHT);

	}

	MakeBackground();
}

int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	int n=0;

	m_cbMarket = std::make_unique<CComboBox>();
	m_cbMarket->Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|CBS_SIMPLE, CRect(0,0,0,0), this, IDC_CBMARKET);
	m_cbMarket->SetFont(m_pFont, FALSE);


	m_cbMarket->ResetContent();

	m_cbMarket->AddString("일반");
	m_cbMarket->AddString("미니");
	m_cbMarket->AddString("위클리");  //ㅅㄷㄴㅅ
 
	if (m_cKind < 2)
		m_cbMarket->SetCurSel(m_cKind);
	else
		m_cbMarket->SetCurSel(0);

	m_stMonth = CreateStatic("옵션월물", m_crCtrlBk, m_crBlack, m_crCtrlBk);
	m_cbMonth = std::make_unique<CComboBox>();
	m_cbMonth->Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|CBS_SIMPLE, CRect(0,0,0,0), this, IDC_CBMONTH);
	m_cbMonth->SetFont(m_pFont, FALSE);


	SetWindowTheme(m_cbMonth->GetSafeHwnd(), L"", L"");
	SetWindowTheme(m_cbMarket->GetSafeHwnd(), L"", L"");


	const CString scr_names[] = {"옵션종합현재가", "옵션종합이론가", "옵션종합민감도", "KRX민감도", "옵션시고저"};
	for(n=0; n<SCR_TYPES; ++n)
	{
		std::unique_ptr<CButton> pBtn = std::make_unique<CButton>();

		pBtn->Create(scr_names[n], WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON|(n ? 0 : WS_GROUP), CRect(), this, IDC_SCRTYPE+n);
		pBtn->SetFont(m_pFont);
		m_rdType[n] = pBtn.release();
	}

	m_chkPrd = std::make_unique<CButton>();
	m_chkPrd->Create("예상",WS_VISIBLE|WS_CHILD|BS_AUTOCHECKBOX,CRect(),this,IDC_CHECK_PREDICT);
	m_chkPrd->SetFont(m_pFont);

	m_btnConf = CreateImgButton("설정", IDC_BTN_CONF, m_hBtn2);
	m_btnGreek = CreateImgButton("민감도설정", IDC_BTN_GREEK, m_hBtn4);

	m_stCall = CreateStatic("콜옵션", m_crMasuBk, m_crBlack, m_crLine);	m_stCall->SetFont(m_pFontB);
	m_stPut  = CreateStatic("풋옵션", m_crMadoBk, m_crBlack, m_crLine); m_stPut->SetFont(m_pFontB);

	InitFutureGrid();
 	InitOptionGrid();
	SelectOptionGrid(m_cStyle);
	m_rdType[m_cStyle]->SetCheck(TRUE);

	m_cfg.CreateCfg(m_pParent,  m_home);
	m_opcal.CreateOpSensCal(m_pParent, m_home);

	LoadCfg();
	LoadOptionData();
	LoadFutureData();
	MakeOptionWnd();
	Send();

	if(m_cStyle == SCR_TYPE4)
	{
		SetTimer(1000,10000,NULL);
	}

	return 0;
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
		case TK_PIBO4013: ParsePIBO4013(dat, len); break;
		case TK_JISU    : ParseJISU(dat, len); break;
		case TK_OPTION  : ParseOption(dat, len); break;
		}
		if (key>=TK_FUTURE && key<(TK_FUTURE+m_fcodes.GetSize())) ParseFUTURE(dat, len);
		break;

// 	case DLL_ALERT:
// 		ParseAlert(dat, strlen(dat));
// 		OutputDebugString(CString(dat,strlen(dat)) + "\n");
// 		break;
	case DLL_ALERTx:
		struct _alertR* alertR;
		alertR = (struct _alertR*)lParam;
		ParseAlert(alertR);
		break;
	case DLL_TRIGGER:
		break;

	case DLL_NOTICE:
		break;

	case DLL_SETPAL:
	case DLL_SETFONT:
	case DLL_SETFONTx:
		ChangeTheme();
		break;

	case DLL_GUIDE:
		return true;

	case DLL_DOMINO:
		break;
	}
	return 0;
}

void CMapWnd::LoadOptionData()
{
	FILE *fp = NULL;
	OPTION_HEADER ohead{};

	//미니선옵 기능 추가 -dkkim 2015.06.30
	CString strFile;
/*  //20190923 weekly option
	if(m_cbMarket->GetCurSel() == 0)
	{
		strFile = "\\tab\\opcode.dat";
	}
	else
	{
		strFile = "\\tab\\mocode.dat";
	}
*/
	if(m_cbMarket->GetCurSel() == 0)
	{
		strFile = "\\tab\\opcode.dat";
	}
	else if(m_cbMarket->GetCurSel() == 1)
	{
		strFile = "\\tab\\mocode.dat";
	}
	else    //weekly option
	{
		strFile = "\\tab\\wocode.dat";   
	}
	/////////////////////////////////////

	do {
		fp = fopen(m_home + strFile, "rb");
		if (!fp) break;

		const size_t len = fread(&ohead, 1, sizeof(ohead), fp);
		if (len!=sizeof(ohead)) break;

		m_cbMonth->ResetContent();
		for(int n=0; n<11; ++n)
		{
			CString strMonth;
			strMonth.Format("%.4s.%.2s", &ohead.pjym[n][0], &ohead.pjym[n][4]);

			if(strMonth != "0000.00")
				m_cbMonth->AddString( strMonth );
		}
		m_cbMonth->SetCurSel(0);
	} while(FALSE);

	if (fp) fclose(fp);
}

void CMapWnd::OnDestroy() 
{
	SaveScreenConfig();	

	if (m_stCall) { m_stCall->DestroyWindow(); delete m_stCall; m_stCall=NULL; }
	if (m_stPut) { m_stPut->DestroyWindow(); delete m_stPut; m_stPut=NULL; }
	if (m_cbMonth) { m_cbMonth->DestroyWindow(); m_cbMonth=nullptr; }
	if (m_btnConf) { m_btnConf->DestroyWindow(); delete m_btnConf; m_btnConf=NULL; }
	if (m_btnGreek) { m_btnGreek->DestroyWindow(); delete m_btnGreek; m_btnGreek=NULL; }
	if (m_gdFuture) { m_gdFuture->DestroyWindow(); delete m_gdFuture; m_gdFuture=NULL; }
	if (m_gdOption) { m_gdOption->DestroyWindow(); delete m_gdOption; m_gdOption=NULL; }
	for(int n=0; n<SCR_TYPES; ++n)
	{
		m_rdType[n]->DestroyWindow();
		delete m_rdType[n];
		m_rdType[n] = NULL;
	}

 	CWnd::OnDestroy();

	ClearData();
}

HBRUSH CMapWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	const int nID = pWnd->GetDlgCtrlID();
	if (nID>=IDC_SCRTYPE || nID == IDC_CHECK_PREDICT)
	{
		pDC->SetBkMode(TRANSPARENT);
		return m_pBrBk->operator HBRUSH();
	}
	else if (nID == IDC_CBMARKET || nID == IDC_CBMONTH)
	{
		pDC->SetBkColor(RGB(255, 255, 255));
	}
	
	return hbr;
}

void CMapWnd::InitFutureGrid()
{
	if (m_gdFuture) 
	{
		if (m_gdFuture->GetSafeHwnd()) m_gdFuture->DestroyWindow();
		delete m_gdFuture;
	}

	//CGridCtrl *pGrid = new CGridCtrl();
	std::unique_ptr<CGridCtrl> pGrid = std::make_unique<CGridCtrl>();
	
	if (pGrid->Create(CRect(), this, IDC_GRID_FUTURE, WS_CHILD|WS_TABSTOP|WS_VISIBLE|WS_BORDER))
	{
		pGrid->SetFont(m_pFont);
		pGrid->SetDoubleBuffering(TRUE);
		pGrid->EnableSelection(FALSE);
		pGrid->SetFrameFocusCell(FALSE);
		pGrid->SetTrackFocusCell(TRUE);
		pGrid->SetColumnResize(FALSE);
		pGrid->SetRowResize(FALSE);
		pGrid->SetBkColor(m_crBodyBk);
		pGrid->SetEditable(FALSE);
	}
	else
	{
	//	delete pGrid;
		pGrid = NULL;
	}
	if (pGrid==NULL) return;

	//m_gdFuture = pGrid.release();

	struct _OGRID_HEADER *ph = FGRID_HEADER[0];
	
	int col=0, cols = 0;
	for(cols=0; ph[cols].name!=NULL; ++cols);
	pGrid->SetFixedRowCount(1);
	pGrid->SetFixedColumnCount(0);
	pGrid->SetColumnCount(cols);
	pGrid->SetRowCount(8);

	LOGFONT lf, lfb;
	m_pFont->GetLogFont(&lf);
	m_pFontB->GetLogFont(&lfb);

	const COLORREF crHeadBk = GetIndexColor(74);
	const COLORREF crHeadFg = GetIndexColor(76);

	for(col=0; col<cols; ++col)
	{
		pGrid->SetItemBkColour(0, col, crHeadBk);
		pGrid->SetItemFgColour(0, col, crHeadFg);
		pGrid->SetItemText(0, col, ph[col].name);
		pGrid->SetItemFormat(0, col, DF_HEADER);
		pGrid->SetColumnWidth(col, ph[col].width);
		
		for(int row=1; row<pGrid->GetRowCount(); ++row)
		{
			pGrid->SetItemFormat(row, col, ph[col].format);
			pGrid->SetItemBkColour(row, col, (row%2) ? m_crEven : m_crOdd);
			pGrid->SetItemFgColour(row, col, m_crNormal);
			if (ph[col].clBk)
				pGrid->SetItemBkColour(row, col, ph[col].clBk);
			pGrid->SetItemText(row, col, "");
		}
	}
	pGrid->Refresh();

	m_gdFuture = pGrid.release();
}

void CMapWnd::SelectFutureGrid(int nIndex)
{
	if(!m_gdFuture) return;

	struct _OGRID_HEADER *ph = FGRID_HEADER[nIndex];
	
	m_gdFuture->SetItemText(0, 2, ph[2].name);
	m_gdFuture->SetItemText(0, 3, ph[3].name);
	m_gdFuture->SetItemText(0, 4, ph[4].name);
	m_gdFuture->SetItemText(0, 5, ph[5].name);

	m_gdFuture->Refresh();
}

void CMapWnd::InitOptionGrid()
{
	if (m_gdOption) 
	{
		if (m_gdOption->GetSafeHwnd()) m_gdOption->DestroyWindow();
		delete m_gdOption;
	}
	
	std::unique_ptr<CGridCtrl>pGrid = std::make_unique<CGridCtrl>();

	if (pGrid->Create(CRect(), this, IDC_GRID_OPTION, WS_CHILD|WS_TABSTOP|WS_VISIBLE|WS_BORDER))
	{
		pGrid->SetFont(m_pFont);
		pGrid->SetDoubleBuffering(TRUE);
		pGrid->EnableSelection(FALSE);
		pGrid->SetFrameFocusCell(FALSE);
		pGrid->SetTrackFocusCell(TRUE);
		pGrid->SetColumnResize(FALSE);
		pGrid->SetRowResize(FALSE);
		pGrid->SetBkColor(m_crBodyBk);
		pGrid->SetEditable(FALSE);
		pGrid->EnableToolTips(TRUE);
		pGrid->SetDefCellMargin(5);
	}
	else
	{
	//	delete pGrid;
		pGrid = NULL;
	}
	m_gdOption = pGrid.release();
}

void CMapWnd::SelectOptionGrid( int type )
{
	if (!m_gdOption) return;

	m_style = type;

	CGridCtrl *pGrid = m_gdOption;
	struct _OGRID_HEADER *ph = OGRID_HEADER[type + m_nIndex];

	int col=0, cols = 0;
	for(cols=0; ph[cols].name!=NULL; ++cols);
	pGrid->SetFixedRowCount(1);
	pGrid->SetFixedColumnCount(0);
	pGrid->SetColumnCount(cols);
	//pGrid->SetRowCount(100);

	LOGFONT lf, lfb;
	m_pFont->GetLogFont(&lf);
	m_pFontB->GetLogFont(&lfb);

	const COLORREF crHeadBk = GetIndexColor(74);
	const COLORREF crHeadFg = GetIndexColor(76);
	for(col=0; col<cols; ++col)
	{
		pGrid->SetItemBkColour(0, col, crHeadBk);
		pGrid->SetItemFgColour(0, col, crHeadFg);
		pGrid->SetItemText(0, col, ph[col].name);
		pGrid->SetItemFormat(0, col, DF_HEADER);
		pGrid->SetColumnWidth(col, ph[col].width);

		for(int row=1; row<m_gdOption->GetRowCount(); ++row)
		{
			pGrid->SetItemFormat(row, col, ph[col].format);
			pGrid->SetItemBkColour(row, col, (row%2) ? m_crEven : m_crOdd);
			pGrid->SetItemFgColour(row, col, m_crNormal);
			if (ph[col].clBk)
				pGrid->SetItemBkColour(row, col, ph[col].clBk);
			pGrid->SetItemText(row, col, "");

			// 현재가 컬럼 볼드처리
			if (m_style==SCR_TYPE1)
			{
				if (col==7 || col==10)	pGrid->SetItemFont(row, col, &lfb);
				else                    pGrid->SetItemFont(row, col, &lf);
			}
			else
			{
				if (col==6 || col==9)   pGrid->SetItemFont(row, col, &lfb);
				else                    pGrid->SetItemFont(row, col, &lf);
			}
		}
	}
	pGrid->Refresh();

	CString key;
	OPTION_DATA* pod{};
	POSITION pos = m_odat.GetStartPosition();
	while(pos!=NULL)
	{
		m_odat.GetNextAssoc(pos, key, pod);
		pod->set_all(TRUE);
	}
	DrawOptionData(NULL);
}

BOOL CMapWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if      (wParam==IDC_GRID_OPTION) OnOptionGridNotify((GV_DISPINFO*)lParam);
	else if (wParam==IDC_GRID_FUTURE) OnFutureGridNotify((GV_DISPINFO*)lParam);

	*pResult = 0;
	return CWnd::OnNotify(wParam, lParam, pResult);
}


void CMapWnd::OnFutureGridNotify( GV_DISPINFO *pDI )
{
	if (pDI->item.row==0) return;

	CString key;
	FUTURE_DATA *pfd = NULL;
	POSITION pos = m_fdat.GetStartPosition();
	while(pos!=NULL)
	{
		m_fdat.GetNextAssoc(pos, key, pfd);
		if (pfd->grid_row==pDI->item.row)
		{
			CString tdat;
			CString tmpCode = pfd->code;
			
			if(tmpCode.GetAt(0) == 'X')
			{
				tmpCode = tmpCode.Mid(1);
			}

			tdat.Format("30301\t%s", tmpCode);
			SetTrigger(tdat);
			break;
		}
	}

	if ( (pDI->hdr.code==NM_CLICK && m_cEvent=="1") || (pDI->hdr.code==NM_DBLCLK  && m_cEvent=="2") )
	{
		CString data;
		CString tmpCode;
		
		if(pfd)
		{
			tmpCode = pfd->code;

			if(tmpCode.GetAt(0) == 'X')
			{
				tmpCode = tmpCode.Mid(1);
			}
		}
		else
		{
			tmpCode = "";
		}

		data.Format("%s /S/t0/d30301\t%s", m_cFScreen, tmpCode);
		SetView(data);
	}

	if (m_FutureCurRow != pDI->item.row || pDI->hdr.code==GVN_SELCHANGING)
	{
		if (m_FutureCurRow>0) 
		{
			const COLORREF colBk = (m_OptionCurRow%2) ? m_crEven : m_crOdd;
			for(int col=0; col<m_gdFuture->GetColumnCount(); ++col)
				m_gdFuture->SetItemBkColour(m_FutureCurRow, col, colBk);
			m_gdFuture->RedrawRow(m_FutureCurRow);
		}
		{
			const COLORREF colBk = GetIndexColor(78);
			for(int col=0; col<m_gdFuture->GetColumnCount(); ++col)
				m_gdFuture->SetItemBkColour(pDI->item.row, col, colBk);
			m_gdFuture->RedrawRow(pDI->item.row);
		}
		m_FutureCurRow = pDI->item.row;
	}
}

void CMapWnd::OnOptionGridNotify( GV_DISPINFO *pDI )
{
	if (pDI->item.row==0) return;

	CString key, tdat;
	OPTION_DATA *pod = NULL;
	POSITION pos = m_odat.GetStartPosition();
	while(pos!=NULL)
	{
		m_odat.GetNextAssoc(pos, key, pod);
		if (pod->grid_row==pDI->item.row)
		{
			if (m_style==SCR_TYPE1)
			{
				if ( (pDI->item.col<=8 && (pod->code[1]=='2' || pod->code[1] == 'B')) || (pDI->item.col>=9 && ( pod->code[1]=='3' || pod->code[1] == 'C')) )  //파상생품 코드개편
				{
					CString tmpCode = pod->code;

					if(tmpCode.GetAt(0) == 'X')
					{
						tmpCode = tmpCode.Mid(1);
					}

					tdat.Format("40301\t%s", tmpCode);
					SetTrigger(tdat);
					break;
				}
			}
			else
			{
				if ( (pDI->item.col<=7 &&( pod->code[1]=='2' || pod->code[1] == 'B')) || (pDI->item.col>=8 && ( pod->code[1]=='3' || pod->code[1] == 'C')) )  //파상생품 코드개편
				{
					CString tmpCode = pod->code;

					if(tmpCode.GetAt(0) == 'X')
					{
						tmpCode = tmpCode.Mid(1);
					}

					tdat.Format("40301\t%s", tmpCode);
					SetTrigger(tdat);
					break;
				}
			}
		}
	}

	if ( (pDI->hdr.code==NM_CLICK && m_cEvent=="1") || (pDI->hdr.code==NM_DBLCLK  && m_cEvent=="2") )
	{
		CString data;
		CString tmpCode;
		
		if(pod)
		{
			tmpCode = pod->code;
			
			if(tmpCode.GetAt(0) == 'X')
			{
				tmpCode = tmpCode.Mid(1);
			}
		}
		else
		{
			tmpCode = "";
		}

		data.Format("%s /S/t0/d40301\t%s", m_cOScreen, tmpCode);
		SetView(data);
	}

	if (m_OptionCurRow != pDI->item.row || pDI->hdr.code==GVN_SELCHANGING)
	{
		struct _OGRID_HEADER *ph = OGRID_HEADER[m_style];
		{
			const COLORREF colBk = GetIndexColor(78);
		
			for(int col=0; col<m_gdOption->GetColumnCount(); ++col)
				m_gdOption->SetItemBkColour(pDI->item.row, col, colBk);

			const int col = (m_style==SCR_TYPE1) ? 8 : 7;
			if (pDI->item.row==m_AtmRow)
			{
				m_gdOption->SetItemBkColour(pDI->item.row, col+0, CL_ATMG);
				m_gdOption->SetItemBkColour(pDI->item.row, col+1, CL_ATMG);
			}
			else if (pDI->item.row==m_RAtmRow)
			{
				m_gdOption->SetItemBkColour(pDI->item.row, col+0, CL_RATM);
				m_gdOption->SetItemBkColour(pDI->item.row, col+1, CL_RATM);
			}
			m_gdOption->RedrawRow(pDI->item.row);
		}
		{
			const COLORREF colBk = (m_OptionCurRow%2) ? m_crEven : m_crOdd;
			for(int col=0; col<m_gdOption->GetColumnCount(); ++col)
			{
				if (ph[col].clBk) m_gdOption->SetItemBkColour(m_OptionCurRow, col, ph[col].clBk);
				else              m_gdOption->SetItemBkColour(m_OptionCurRow, col, colBk);
			}

			const int col = (m_style==SCR_TYPE1) ? 8 : 7;
			if (m_OptionCurRow==m_AtmRow)
			{
				m_gdOption->SetItemBkColour(m_OptionCurRow, col+0, CL_ATMG);
				m_gdOption->SetItemBkColour(m_OptionCurRow, col+1, CL_ATMG);				
			}
			else if (m_OptionCurRow==m_RAtmRow)
			{
				m_gdOption->SetItemBkColour(m_OptionCurRow, col+0, CL_RATM);
				m_gdOption->SetItemBkColour(m_OptionCurRow, col+1, CL_RATM);				
			}
			m_gdOption->RedrawRow(m_OptionCurRow);
		}		
		m_OptionCurRow = pDI->item.row;
	}
}

void CMapWnd::Send()
{
	ClearData();
	SelectOptionGrid(m_style);

	// pibo4013
	{
		CString yymm;
		m_cbMonth->GetWindowText(yymm);
		yymm.Replace(".", "");

		if (yymm.GetLength()!=6) return;

		struct pibo4013_mid mid;
		ZeroMemory(&mid, sizeof(mid));

		/* weekly option 20190923
		if(m_cbMarket->GetCurSel() == 0)
		{
			memcpy(mid.type,"0",1);
		}
		else
		{
			memcpy(mid.type,"1",1);
		}
		*/
		if(m_cbMarket->GetCurSel() == 0)
		{
			memcpy(mid.type,"0",1);
		}
		else if(m_cbMarket->GetCurSel() == 1)
		{
			memcpy(mid.type,"1",1);
		}
		else if(m_cbMarket->GetCurSel() == 2)  //weekly option
		{
			memcpy(mid.type,"2",1);
		}


		CString strFlag;

		if(m_chkPrd->GetCheck())
		{
			strFlag = "1";
		}
		else
		{
			strFlag = "0";
		}

		memcpy(mid.prd, (LPCSTR)strFlag,1);
		memcpy(mid.mymd, (LPCSTR)yymm, sizeof(mid.mymd));

	//	SendTR("pibo401A", 0, (LPCSTR)&mid, sizeof(mid), TK_PIBO4013);
		SendTR("pibo401B", 0, (LPCSTR)&mid, sizeof(mid), TK_PIBO4013);
	//	int major = m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC));
		CString send;
		send.Format("%s%c%s", "sendprocdll4013", 0x09, yymm);  
		//bool ret = m_pParent->SendMessage(WM_USER, MAKEWPARAM(procDLL, MAKEWORD(major, m_key[0]) ), (LPARAM)(LPCTSTR)send);	
		const bool ret = m_pParent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0 ), (LPARAM)(LPCTSTR)send);	
		send.Format("%d %d", m_key[0], m_key[1]);
		SendTR("pibo401b", 0, (LPCSTR)&mid, sizeof(mid), TK_PIBO4013);   //확인필요  안날려도 됨
	}

	// KOSPI
	{
		char data[1024];
		const int dlen = sprintf(data, "20301%c%s\t20021\t20023\t20306\t20034\t", 0x7f, "001");
		SendTR("pooppoop", US_OOP, data, dlen, TK_JISU);
	}

	// KOSPI
	{
		char data[1024];
		const int dlen = sprintf(data, "20301%c%s\t20021\t20023\t20306\t20034\t", 0x7f, "101");
		SendTR("pooppoop", US_OOP, data, dlen, TK_JISU);
	}
	
	// future
	{
		char data[1024]{};

		if(m_chkPrd->GetCheck())
		{
			for(int n=0; n<m_fcodes.GetSize(); ++n)
			{
				const int dlen = sprintf(data, "30301%c%s\t30021\t30022\t31111\t31023\t31115\t31204\t31112\t31201\t31206\t31116\t31203\t31205\t31202\t", 0x7f, m_fcodes.GetAt(n).GetString());
				SendTR("pooppoop", US_OOP, data, dlen, TK_FUTURE+n);
			}
		}
		else
		{
			for(int n=0; n<m_fcodes.GetSize(); ++n)
			{
				const int dlen = sprintf(data, "30301%c%s\t30021\t30022\t31111\t31023\t31024\t31204\t31027\t31201\t31206\t31033\t31203\t31205\t31202\t", 0x7f, m_fcodes.GetAt(n).GetString());
				SendTR("pooppoop", US_OOP, data, dlen, TK_FUTURE+n);
			}
		}
	}

}

void CMapWnd::ParsePIBO4013( LPCSTR dat, int len )
{
	struct pibo4013_mod *mod = (struct pibo4013_mod *)dat;
	CString stmp;
	int ii{}, row{};
	const int nrec = Str2Int(mod->grec, sizeof(mod->grec));
	if (nrec<=0) return;

	CGridCtrl *pg = m_gdOption;
	pg->SetRowCount(nrec + 1);	// + header

	for(ii=0, row=1; ii<nrec; ++ii, ++row)
	{
		struct pibo4013_grid *dg = (struct pibo4013_grid *)&mod->grid[ii];

		const double atmg = Str2Double(mod->atmg, sizeof(mod->atmg));

		m_hsga.Add(Str2Double(dg->hang, sizeof(dg->hang)));
		CString s;

		// 콜
		{
			CString code(dg->c_rtcd, sizeof(dg->c_rtcd));
			
			OPTION_DATA *od;
			if (!m_odat.Lookup(code, od))
				od = new OPTION_DATA();

			sprintf(od->code, (LPCSTR)code);
			od->set_mdif(Str2Int(dg->c_mdif, sizeof(dg->c_mdif)));
			od->set_mgjv(Str2Int(dg->c_mgjv, sizeof(dg->c_mgjv)));
			od->set_gvol(Str2Int(dg->c_gvol, sizeof(dg->c_gvol)));

			if(m_chkPrd->GetCheck())
			{
				od->set_msga(0.0);
				od->set_mdga(0.0);
			}
			else
			{
				od->set_msga(fabs(Str2Double(dg->c_msga, sizeof(dg->c_msga))));
				od->set_mdga(fabs(Str2Double(dg->c_mdga, sizeof(dg->c_mdga))));
			}

			od->set_rate(Str2Double(dg->c_rate, sizeof(dg->c_rate)));
			od->set_diff(Str2Double(&dg->c_diff[1], sizeof(dg->c_diff)-1));
			if (dg->c_diff[0]=='5' || dg->c_diff[0]=='4' || dg->c_diff[0]=='-')
				od->set_diff(-od->diff);
			od->set_curr(fabs(Str2Double(dg->c_curr, sizeof(dg->c_curr))));
			od->set_ycurr(fabs(Str2Double(dg->c_ycur, sizeof(dg->c_ycur))));

			stmp.Format("%s", dg->c_ycvl);
			stmp = stmp.Left(sizeof(dg->c_ycvl));
			stmp.Trim();
			od->set_ycuvl(Str2Int(stmp));  //0422 oubc 콜

			if(m_chkPrd->GetCheck())
			{
				od->set_gjga(fabs(od->ycurr - od->diff));
			}
			else
			{
				od->set_gjga(fabs(od->curr - od->diff));
			}
			od->set_hsga(Str2Double(dg->hang, sizeof(dg->hang)));
			od->set_jisu(Str2Double(dg->jihs, sizeof(dg->jihs)));
			s.Format("HANG [%.f]\n",Str2Double(dg->hang, sizeof(dg->hang)));
			OutputDebugString(s);
			//KRX민감도 추가
			//2013.07.17 김덕기
			od->set_krxrho(Str2Double(dg->c_raw, sizeof(dg->c_raw)));
			od->set_krxvega(Str2Double(dg->c_bega, sizeof(dg->c_bega)));
			od->set_krxteta(Str2Double(dg->c_theta, sizeof(dg->c_theta)));
			od->set_krxgmma(Str2Double(dg->c_gamma, sizeof(dg->c_gamma)));
			od->set_krxdelt(Str2Double(dg->c_delta, sizeof(dg->c_delta)));

			//시고저
			od->set_siga(Str2Double(dg->c_siga,sizeof(dg->c_siga)));
			od->set_koga(Str2Double(dg->c_koga,sizeof(dg->c_koga)));
			od->set_jega(Str2Double(dg->c_jega,sizeof(dg->c_jega)));

// 			CString s;
// 			s.Format("KRX RHO [%s] [%lf]\n",dg->c_raw,Str2Double(dg->c_raw, sizeof(dg->c_raw)));
// 			OutputDebugString(s);

			if (atmg == od->hsga) m_AtmRow = row;
			od->grid_row = row;
			
			m_odat.SetAt(code, od);
		}

		// 풋
		{
			CString code(dg->p_rtcd, sizeof(dg->p_rtcd));

			OPTION_DATA *od;
			if (!m_odat.Lookup(code, od))
				od = new OPTION_DATA();
			sprintf(od->code, (LPCSTR)code);
			od->set_mdif(Str2Int(dg->p_mdif, sizeof(dg->p_mdif)));
			od->set_mgjv(Str2Int(dg->p_mgjv, sizeof(dg->p_mgjv)));
			od->set_gvol(Str2Int(dg->p_gvol, sizeof(dg->p_gvol)));

			if(m_chkPrd->GetCheck())
			{
				od->set_msga(0.0);
				od->set_mdga(0.0);
			}
			else
			{
				od->set_msga(fabs(Str2Double(dg->p_msga, sizeof(dg->p_msga))));
				od->set_mdga(fabs(Str2Double(dg->p_mdga, sizeof(dg->p_mdga))));
			}

			od->set_rate(Str2Double(dg->p_rate, sizeof(dg->p_rate)));
			od->set_diff(Str2Double(&dg->p_diff[1], sizeof(dg->p_diff)-1));
			if (dg->p_diff[0]=='5' || dg->p_diff[0]=='4' || dg->p_diff[0]=='-')
				od->set_diff(-od->diff);
			od->set_curr(fabs(Str2Double(dg->p_curr, sizeof(dg->p_curr))));
			od->set_ycurr(fabs(Str2Double(dg->p_ycur, sizeof(dg->p_ycur))));

			stmp.Format("%s", dg->p_ycvl);
			stmp = stmp.Left(sizeof(dg->p_ycvl));
			stmp.Trim();
			od->set_ycuvl(Str2Int(stmp));  //0422oubp 풋

			if(m_chkPrd->GetCheck())
			{
				od->set_gjga(fabs(od->ycurr - od->diff));
			}
			else
			{
				od->set_gjga(fabs(od->curr - od->diff));
			}
			od->set_hsga(Str2Double(dg->hang, sizeof(dg->hang)));
			od->set_jisu(Str2Double(dg->jihs, sizeof(dg->jihs)));
			s.Format("HANG [%.f]\n",Str2Double(dg->hang, sizeof(dg->hang)));
			OutputDebugString(s);
			//KRX민감도 추가
			//2013.07.17 김덕기
			od->set_krxrho(Str2Double(dg->p_raw, sizeof(dg->p_raw)));
			od->set_krxvega(Str2Double(dg->p_bega, sizeof(dg->p_bega)));
			od->set_krxteta(Str2Double(dg->p_theta, sizeof(dg->p_theta)));
			od->set_krxgmma(Str2Double(dg->p_gamma, sizeof(dg->p_gamma)));
			od->set_krxdelt(Str2Double(dg->p_delta, sizeof(dg->p_delta)));

			od->set_siga(Str2Double(dg->p_siga,sizeof(dg->p_siga)));
			od->set_koga(Str2Double(dg->p_koga,sizeof(dg->p_koga)));
			od->set_jega(Str2Double(dg->p_jega,sizeof(dg->p_jega)));

			if (atmg == od->hsga) m_AtmRow = row;

			od->grid_row = row;
			
			m_odat.SetAt(code, od);
		}
	}

	POSITION pos = m_odat.GetStartPosition();
	CString code;
	OPTION_DATA* pod{};
	if (pos!=NULL)
	{
		m_odat.GetNextAssoc(pos, code, pod);

		CString sendCode(pod->code,sizeof(pod->code));
		sendCode = sendCode.Mid(1);

		CString s;
		s.Format("CODE [%s] SENDCODE [%s]\n",code,sendCode);
		OutputDebugString(s);
		
		char data[1024];
		const int dlen = sprintf(data, "40301%c%s\t40332\t40323\t40314\t40370\t40361\t40369\t40368\t40367\t", 0x7f, sendCode.GetString());
		SendTR("pooppoop", US_OOP, data, dlen, TK_OPTION);
	}

	if (m_AtmRow>0)
	{
		CRect rc;
		m_gdOption->GetClientRect(&rc);
		const int pos = (int)(((m_AtmRow+0.5)*m_gdOption->GetDefCellHeight()) - rc.Height()/2);
		m_gdOption->SetScrollPos(SB_VERT, pos, TRUE);
		m_gdOption->SendMessage(WM_VSCROLL, 0, 0);
	}

	CalcRAtm();
	GenFutureData();
	GenOptionData();
	SelectOptionGrid(m_style);
	DrawOptionData(NULL);
}


void CMapWnd::DrawFutureData( FUTURE_DATA *pfd /*= NULL*/, CDC *pDC /*= NULL*/ )
{
	if (pfd==NULL)
	{
		CString key;
		FUTURE_DATA* dat{};
		POSITION pos = m_fdat.GetStartPosition();
		
		const BOOL bRelease = (pDC==NULL);
		if (pDC==NULL) pDC = m_gdFuture->GetDC();
		while(pos!=NULL)
		{	
			m_fdat.GetNextAssoc(pos, key, dat);
			DrawFutureData(dat, pDC);
		}
		if (bRelease) m_gdFuture->ReleaseDC(pDC);
		return;
	}
	
	char buff[32]{};
	const int row = pfd->grid_row;
	
	CGridCtrl *pg = m_gdFuture;
	const BOOL bRelease = (pDC==NULL);
	if (pDC==NULL) pDC = pg->GetDC();

	if (pfd->is_code())
	{
		DWORD      colFormat{};   //fixs_s
		colFormat = pg->GetItemFormat(row, 0);
		colFormat = colFormat ^ DT_LEFT;
		colFormat = colFormat | DT_CENTER;
		pg->SetItemFormat(row, 0, colFormat);  //fixe_e
		pg->SetItemText(row, 0, pfd->code);
		pg->RedrawCell(row, 0, pDC);
	}

	if (pfd->is_name())
	{
		DWORD      colFormat{};   //fixs_s
		colFormat = pg->GetItemFormat(row, 0);
		colFormat = colFormat ^ DT_LEFT;
		colFormat = colFormat | DT_CENTER;
		pg->SetItemFormat(row, 1, colFormat);  //fixe_e
		pg->SetItemText(row, 1, pfd->hnam);
		pg->RedrawCell(row, 1, pDC);
	}

	if(m_chkPrd->GetCheck())
	{
		if (pfd->is_ycurr())
		{
			pg->SetItemFgColour(row, 2, GetColor(pfd->diff));
			sprintf(buff, "%.2f", pfd->ycurr);
			pg->SetItemText(row, 2, (pfd->ycurr!=0.0) ? buff : "");
			pg->RedrawCell(row, 2, pDC);
		}

		if (pfd->is_ycuvl())
		{
			pg->SetItemText(row, 5, (pfd->ycuvl) ? Int2CommaStr(pfd->ycuvl) : "");
			pg->RedrawCell(row, 5, pDC);
		}
	}
	else
	{
		if (pfd->is_curr())
		{
			pg->SetItemFgColour(row, 2, GetColor(pfd->diff));
			sprintf(buff, "%.2f", pfd->curr);
			pg->SetItemText(row, 2, (pfd->curr!=0.0) ? buff : "");
			pg->RedrawCell(row, 2, pDC);
		}

		if (pfd->is_gvol())
		{
			pg->SetItemText(row, 5, (pfd->gvol) ? Int2CommaStr(pfd->gvol) : "");
			pg->RedrawCell(row, 5, pDC);
		}
	}

// 	CString s;
// 	s.Format("FUTURE CURR : [%f]\n",pfd->curr);
// 	OutputDebugString(s);
	if (pfd->is_diff())
	{
		//OutputDebugString("FUTURE DIFF \n");
		pg->SetItemFgColour(row, 3, GetColor(pfd->diff) );
		sprintf(buff, "%s %.2f", GetDiffGiho(pfd->diff), fabs(pfd->diff));
		pg->SetItemText(row, 3, (pfd->diff!=0.0) ? buff : "");
		pg->RedrawCell(row, 3, pDC);
	}

	if (pfd->is_rate())
	{
		//OutputDebugString("FUTURE RATE \n");
		pg->SetItemFgColour(row, 4, GetColor(pfd->diff) );
		sprintf(buff, "%.2f", pfd->rate);
		pg->SetItemText(row, 4, (pfd->rate!=0.0) ? buff : "");
		pg->RedrawCell(row, 4, pDC);
	}

	if (pfd->is_irga())
	{
		sprintf(buff, "%.2f", fabs(pfd->irga));
		pg->SetItemText(row, 6, buff);
		pg->RedrawCell(row, 6, pDC);
	}

	if (pfd->is_gryl())
	{
		pg->SetItemFgColour(row, 7, GetColor(pfd->gryl) );
		sprintf(buff, "%.2f", pfd->gryl);
		pg->SetItemText(row, 7, (pfd->gryl!=0.0) ? buff : "");
		pg->RedrawCell(row, 7, pDC);
	}

	if (pfd->is_bsis())
	{
		pg->SetItemFgColour(row, 8, GetColor(pfd->bsis) );
		sprintf(buff, "%.2f", pfd->bsis);
		pg->SetItemText(row, 8, (pfd->bsis!=0.0) ? buff : "");
		pg->RedrawCell(row, 8, pDC);
	}

	if (pfd->is_isis())
	{
		pg->SetItemFgColour(row, 9, GetColor(pfd->isis) );
		sprintf(buff, "%.2f", pfd->isis);
		pg->SetItemText(row, 9, (pfd->isis!=0.0) ? buff : "");
		pg->RedrawCell(row, 9, pDC);
	}

	if (pfd->is_mgjv())
	{
		pg->SetItemText(row, 10, (pfd->mgjv) ? Int2CommaStr(pfd->mgjv) : "");
		pg->RedrawCell(row, 10, pDC);
	}

	if (pfd->is_mdif())
	{
		pg->SetItemFgColour(row, 11, GetColor(pfd->mdif) );
		pg->SetItemText(row, 11, (pfd->mdif) ? Int2CommaStr(pfd->mdif) : "");
		pg->RedrawCell(row, 11, pDC);
	}

	if (bRelease) pg->ReleaseDC(pDC);
}

void CMapWnd::DrawOptionData( OPTION_DATA *pod, CDC *pDC )
{
	if (pod==NULL)
	{
		CString key;
		OPTION_DATA* dat{};
		POSITION pos = m_odat.GetStartPosition();
		
		const BOOL bRelease = (pDC==NULL);
		if (pDC==NULL) pDC = m_gdOption->GetDC();
		while(pos!=NULL)
		{	
			m_odat.GetNextAssoc(pos, key, dat);
			DrawOptionData(dat, pDC);
		}
		if (bRelease) m_gdOption->ReleaseDC(pDC);
		return;
	}

	const int row = pod->grid_row;
	char buff[32]{};
	CGridCtrl *pg = m_gdOption;

	const BOOL bRelease = (pDC==NULL);
	if (pDC==NULL) pDC = pg->GetDC();
	if (m_style==SCR_TYPE1)
	{
		int *idx = NULL;
		int col_idx[] = {0,1,2,3,4,5,6,7,8,9};
		int put_idx[] = {17,16,15,14,13,12,11,10,8,9};
		if      (pod->code[1]=='2' || pod->code[1] == 'B')	idx = col_idx;     //파생상품 코드개편
		else if (pod->code[1]=='3' || pod->code[1] == 'C') idx = put_idx;  //파생상품 코드개편
		if (idx==NULL) return;

		if (pod->is_iv())
		{
			//OutputDebugString("IV\n");
			sprintf(buff, "%.2f", fabs(pod->iv));
			pg->SetItemText(row, *(idx+0), (pod->iv==0.0) ? "" : buff);
			pg->RedrawCell(row, *(idx+0), pDC);
			CString ss;
			ss.Format("[IV DATA : %s]\n",buff);
			//OutputDebugString(ss);

		}

		if (pod->is_mdif())
		{
			pg->SetItemFgColour(row, *(idx+1), GetColor(pod->mdif));
			pg->SetItemText(row, *(idx+1), (pod->mdif) ? Int2CommaStr(pod->mdif) : "");
			pg->RedrawCell(row, *(idx+1), pDC);
		}
		
		if (pod->is_mgjv())
		{
			pg->SetItemText(row, *(idx+2), (pod->mgjv) ? Int2CommaStr(pod->mgjv) : "");
			pg->RedrawCell(row, *(idx+2), pDC);
		}

		if (pod->is_gvol())
		{
			pg->SetItemText(row, *(idx+3), (pod->gvol) ? Int2CommaStr(pod->gvol) : "");
			pg->RedrawCell(row, *(idx+3), pDC);
		}
		
		if(!m_chkPrd->GetCheck())
		{
			if (pod->is_msga())
			{
				pg->SetItemFgColour(row, *(idx+4),  GetColor(pod->gjga, pod->msga) );
				sprintf(buff, "%.2f", fabs(pod->msga));
				pg->SetItemText(row, *(idx+4), (pod->msga==0.0) ? "" : buff);
				pg->RedrawCell(row, *(idx+4), pDC);
			}
			
			if (pod->is_mdga())
			{
				pg->SetItemFgColour(row, *(idx+5), GetColor(pod->gjga, pod->mdga) );
				sprintf(buff, "%.2f", fabs(pod->mdga));
				pg->SetItemText(row, *(idx+5), (pod->mdga==0.0) ? "" : buff);
				pg->RedrawCell(row, *(idx+5), pDC);
			}
		}
		
		if (pod->is_diff())
		{
			pg->SetItemFgColour(row, *(idx+6), GetColor(pod->diff) );
			sprintf(buff, "%s %.2f", GetDiffGiho(pod->diff), fabs(pod->diff));
			pg->SetItemText(row, *(idx+6), (pod->diff!=0.0) ? buff : "");
			pg->RedrawCell(row, *(idx+6), pDC);
		}

		if(m_chkPrd->GetCheck())
		{
			if (pod->is_ycurr())
			{
				pg->SetItemFgColour(row, *(idx+7), GetColor(pod->diff) );
				sprintf(buff, "%.2f", fabs(pod->ycurr));
				pg->SetItemText(row, *(idx+7), (pod->ycurr != 0.0) ? buff : "");
				pg->RedrawCell(row, *(idx+7), pDC);
			}

			if (pod->is_ycuvl())  //0422 draw 콜 
			{
				sprintf(buff, "%d", pod->ycuvl);
				pg->SetItemText(row, *(idx + 3), (pod->ycuvl != 0) ? buff : "");
				pg->RedrawCell(row, *(idx + 3), pDC);
			}
		}
		else
		{
			if (pod->is_curr())
			{
				pg->SetItemFgColour(row, *(idx+7), GetColor(pod->diff) );
				sprintf(buff, "%.2f", fabs(pod->curr));
				pg->SetItemText(row, *(idx+7), buff);
				pg->RedrawCell(row, *(idx+7), pDC);
			}
		}
		
		if (pod->is_hsga())
		{
			sprintf(buff, "%.2f", fabs(pod->hsga));
			pg->SetItemText(row, *(idx+8), buff);
			if (row==m_AtmRow) pg->SetItemBkColour(row, *(idx+8), CL_ATMG);
			else if (row==m_RAtmRow) pg->SetItemBkColour(row, *(idx+8), CL_RATM);
			pg->RedrawCell(row, *(idx+8), pDC);
		}
		
		if (pod->is_jisu())
		{
			sprintf(buff, "%.2f", floor(pod->jisu*100.0)/100.0);
			pg->SetItemText(row, *(idx+9), buff);
			if (row==m_AtmRow) pg->SetItemBkColour(row, *(idx+9), CL_ATMG);
			else if (row==m_RAtmRow) pg->SetItemBkColour(row, *(idx+9), CL_RATM);
			pg->RedrawCell(row, *(idx+9), pDC);
		}
	}
	else if (m_style==SCR_TYPE2)
	{
		int *idx = NULL;
		int col_idx[] = {0,1,2,3,4,5,6,7,8};
		int put_idx[] = {15,14,13,12,11,10,9,7,8};
		if      (pod->code[1]=='2' || pod->code[1] == 'B')	idx = col_idx;    //파생상품 코드개편
		else if (pod->code[1]=='3' || pod->code[1] == 'C') idx = put_idx;
		if (idx==NULL) return;

		if (pod->is_gvol())
		{
			pg->SetItemText(row, *(idx+0), (pod->gvol) ? Int2CommaStr(pod->gvol) : "");
			pg->RedrawCell(row, *(idx+0), pDC);
		}

		if (pod->is_grdo())
		{
			sprintf(buff, "%.2f", pod->grdo);
			pg->SetItemText(row, *(idx+1), (pod->grdo!=0.0) ? buff : "");
			pg->RedrawCell(row, *(idx+1), pDC);
		}
		
		if (pod->is_irga())
		{
			sprintf(buff, "%.2f", pod->irga);
			pg->SetItemText(row, *(idx+2), (pod->irga!=0.0) ? buff : "");
			pg->RedrawCell(row, *(idx+2), pDC);
		}

		if (pod->is_sggc())
		{
			sprintf(buff, "%.2f", pod->sggc);
			pg->SetItemText(row, *(idx+3), (pod->sggc!=0.0) ? buff : "");
			pg->RedrawCell(row, *(idx+3), pDC);
		}

		if(m_chkPrd->GetCheck())
		{
			if (pod->is_rate())
			{
				pg->SetItemFgColour(row, *(idx+4), GetColor(pod->rate) );
				sprintf(buff, "%.2f", pod->rate);
				pg->SetItemText(row, *(idx+4), (pod->rate!=0.0) ? buff : "");
				pg->RedrawCell(row, *(idx+4), pDC);
			}
		}
		else
		{
			if (pod->is_rate())
			{
				pg->SetItemFgColour(row, *(idx+4), GetColor(pod->gjga, pod->curr) );
				sprintf(buff, "%.2f", pod->rate);
				pg->SetItemText(row, *(idx+4), (pod->rate!=0.0) ? buff : "");
				pg->RedrawCell(row, *(idx+4), pDC);
			}
		}

		if (pod->is_diff())
		{
			pg->SetItemFgColour(row, *(idx+5), GetColor(pod->diff) );
			sprintf(buff, "%s %.2f", GetDiffGiho(pod->diff), fabs(pod->diff));
			pg->SetItemText(row, *(idx+5), (pod->diff!=0.0) ? buff : "");
			pg->RedrawCell(row, *(idx+5), pDC);
		}

		if(m_chkPrd->GetCheck())
		{
			if (pod->is_ycurr())
			{
				pg->SetItemFgColour(row, *(idx+6), GetColor(pod->diff) );
				sprintf(buff, "%.2f", fabs(pod->ycurr));
				pg->SetItemText(row, *(idx+6), (pod->ycurr != 0.0) ? buff : "");
				pg->RedrawCell(row, *(idx+6), pDC);
			}

			if (pod->is_ycuvl())  //0422 draw 풋 
			{
				sprintf(buff, "%d", pod->ycuvl);
				pg->SetItemText(row, *(idx + 0), (pod->ycuvl != 0) ? buff : "");
				pg->RedrawCell(row, *(idx + 0), pDC);
			}
		}
		else
		{
			if (pod->is_curr())
			{
				pg->SetItemFgColour(row, *(idx+6), GetColor(pod->diff) );
				sprintf(buff, "%.2f", fabs(pod->curr));
				pg->SetItemText(row, *(idx+6), (pod->curr != 0.0) ? buff : "");
				pg->RedrawCell(row, *(idx+6), pDC);
			}
		}
		
		if (pod->is_hsga())
		{
			sprintf(buff, "%.2f", fabs(pod->hsga));
			pg->SetItemText(row, *(idx+7), buff);
			if (row==m_AtmRow) pg->SetItemBkColour(row, *(idx+7), CL_ATMG);
			else if (row==m_RAtmRow) pg->SetItemBkColour(row, *(idx+7), CL_RATM);
			pg->RedrawCell(row, *(idx+7), pDC);
		}
		
		if (pod->is_jisu())
		{
			sprintf(buff, "%.2f", floor(pod->jisu*100.0)/100.0);
			pg->SetItemText(row, *(idx+8), buff);
			if (row==m_AtmRow) pg->SetItemBkColour(row, *(idx+8), CL_ATMG);
			else if (row==m_RAtmRow) pg->SetItemBkColour(row, *(idx+8), CL_RATM);
			pg->RedrawCell(row, *(idx+8), pDC);
		}
	}
	else if (m_style==SCR_TYPE3)
	{
		int *idx = NULL;
		int col_idx[] = {0,1,2,3,4,5,6,7,8};
		int put_idx[] = {15,14,13,12,11,10,9,7,8};
		if      (pod->code[1]=='2' || pod->code[1] == 'B')	idx = col_idx;   //파상생품 코드개편
		else if (pod->code[1]=='3' || pod->code[1] == 'C') idx = put_idx;
		if (idx==NULL) return;
		
		if (pod->is_rho())
		{
			sprintf(buff, "%.4f", pod->rho);
			pg->SetItemText(row, *(idx+0), (pod->rho!=0.0) ? buff : "");
			pg->RedrawCell(row, *(idx+0), pDC);
		}
		
		if (pod->is_vega())
		{
			sprintf(buff, "%.4f", pod->vega);
			pg->SetItemText(row, *(idx+1), (pod->vega!=0.0) ? buff : "");
			pg->RedrawCell(row, *(idx+1), pDC);
		}
		
		if (pod->is_teta())
		{
			sprintf(buff, "%.4f", pod->teta);
			pg->SetItemText(row, *(idx+2), (pod->teta!=0.0) ? buff : "");
			pg->RedrawCell(row, *(idx+2), pDC);
		}

		if (pod->is_gmma())
		{
			sprintf(buff, "%.4f", pod->gmma);
			pg->SetItemText(row, *(idx+3), (pod->gmma!=0.0) ? buff : "");
			pg->RedrawCell(row, *(idx+3), pDC);
		}

		if (pod->is_delt())
		{
			sprintf(buff, "%.4f", pod->delt);
			pg->SetItemText(row, *(idx+4), (pod->delt!=0.0) ? buff : "");
			pg->RedrawCell(row, *(idx+4), pDC);
		}
		
		if (pod->is_diff())
		{
			pg->SetItemFgColour(row, *(idx+5), GetColor(pod->diff) );
			sprintf(buff, "%s %.2f", GetDiffGiho(pod->diff), fabs(pod->diff));
			pg->SetItemText(row, *(idx+5), (pod->diff!=0.0) ? buff : "");
			pg->RedrawCell(row, *(idx+5), pDC);
		}

		if(m_chkPrd->GetCheck())
		{
			if (pod->is_ycurr())
			{
				pg->SetItemFgColour(row, *(idx+6), GetColor(pod->diff) );
				sprintf(buff, "%.2f", fabs(pod->ycurr));
				pg->SetItemText(row, *(idx+6), (pod->ycurr != 0.0) ? buff : "");
				pg->RedrawCell(row, *(idx+6), pDC);
			}
		}
		else
		{
			if (pod->is_curr())
			{
				pg->SetItemFgColour(row, *(idx+6), GetColor(pod->diff) );
				sprintf(buff, "%.2f", fabs(pod->curr));
				pg->SetItemText(row, *(idx+6), (pod->curr != 0.0) ? buff : "");
				pg->RedrawCell(row, *(idx+6), pDC);
			}
		}
		
		if (pod->is_hsga())
		{
			sprintf(buff, "%.2f", fabs(pod->hsga));
			pg->SetItemText(row, *(idx+7), buff);
			if (row==m_AtmRow) pg->SetItemBkColour(row, *(idx+7), CL_ATMG);
			else if (row==m_RAtmRow) pg->SetItemBkColour(row, *(idx+7), CL_RATM);
			pg->RedrawCell(row, *(idx+7), pDC);
		}
		
		if (pod->is_jisu())
		{
			sprintf(buff, "%.2f", floor(pod->jisu*100.0)/100.0);
			pg->SetItemText(row, *(idx+8), buff);
			if (row==m_AtmRow) pg->SetItemBkColour(row, *(idx+8), CL_ATMG);
			else if (row==m_RAtmRow) pg->SetItemBkColour(row, *(idx+8), CL_RATM);
			pg->RedrawCell(row, *(idx+8), pDC);
		}
	}
	else if (m_style==SCR_TYPE4)
	{
		//OutputDebugString("SCR_TYPE4 DRAW\n");
		int *idx = NULL;
		int col_idx[] = {0,1,2,3,4,5,6,7,8};
		int put_idx[] = {15,14,13,12,11,10,9,7,8};
		if      (pod->code[1]=='2' || pod->code[1] == 'B')	idx = col_idx;    //파생상품 코드개편
		else if (pod->code[1]=='3' || pod->code[1] == 'C') idx = put_idx;
		if (idx==NULL) return;
		CString s;

		if (pod->is_krxrho())
		{
			sprintf(buff, "%.4f", pod->krxrho);
			pg->SetItemText(row, *(idx+0), (pod->krxrho!=0.0) ? buff : "");
			pg->RedrawCell(row, *(idx+0), pDC);
			s.Format("KRX RAW [%s]\n",buff);
			OutputDebugString(s);
		}
		
		if (pod->is_krxvega())
		{
			sprintf(buff, "%.4f", pod->krxvega);
			pg->SetItemText(row, *(idx+1), (pod->krxvega!=0.0) ? buff : "");
			pg->RedrawCell(row, *(idx+1), pDC);
		}
		
		if (pod->is_krxteta())
		{
			sprintf(buff, "%.4f", pod->krxteta);
			pg->SetItemText(row, *(idx+2), (pod->krxteta!=0.0) ? buff : "");
			pg->RedrawCell(row, *(idx+2), pDC);
		}
		
		if (pod->is_krxgmma())
		{
			sprintf(buff, "%.4f", pod->krxgmma);
			pg->SetItemText(row, *(idx+3), (pod->krxgmma!=0.0) ? buff : "");
			pg->RedrawCell(row, *(idx+3), pDC);
		}
		
		if (pod->is_krxdelt())
		{
			sprintf(buff, "%.4f", pod->krxdelt);
			pg->SetItemText(row, *(idx+4), (pod->krxdelt!=0.0) ? buff : "");
			pg->RedrawCell(row, *(idx+4), pDC);
		}
		
		if (pod->is_diff())
		{
			pg->SetItemFgColour(row, *(idx+5), GetColor(pod->diff) );
			sprintf(buff, "%s %.2f", GetDiffGiho(pod->diff), fabs(pod->diff));
			pg->SetItemText(row, *(idx+5), (pod->diff!=0.0) ? buff : "");
			pg->RedrawCell(row, *(idx+5), pDC);
		}
		
		if(m_chkPrd->GetCheck())
		{
			if (pod->is_ycurr())
			{
				pg->SetItemFgColour(row, *(idx+6), GetColor(pod->diff) );
				sprintf(buff, "%.2f", fabs(pod->ycurr));
				pg->SetItemText(row, *(idx+6), (pod->ycurr != 0.0) ? buff : "");
				pg->RedrawCell(row, *(idx+6), pDC);
			}
		}
		else
		{
			if (pod->is_curr())
			{
				pg->SetItemFgColour(row, *(idx+6), GetColor(pod->diff) );
				sprintf(buff, "%.2f", fabs(pod->curr));
				pg->SetItemText(row, *(idx+6), (pod->curr != 0.0) ? buff : "");
				pg->RedrawCell(row, *(idx+6), pDC);
			}
		}
		
		if (pod->is_hsga())
		{
			sprintf(buff, "%.2f", fabs(pod->hsga));
			pg->SetItemText(row, *(idx+7), buff);
			if (row==m_AtmRow) pg->SetItemBkColour(row, *(idx+7), CL_ATMG);
			else if (row==m_RAtmRow) pg->SetItemBkColour(row, *(idx+7), CL_RATM);
			pg->RedrawCell(row, *(idx+7), pDC);
		}
		
		if (pod->is_jisu())
		{
			sprintf(buff, "%.2f", floor(pod->jisu*100.0)/100.0);
			pg->SetItemText(row, *(idx+8), buff);
			if (row==m_AtmRow) pg->SetItemBkColour(row, *(idx+8), CL_ATMG);
			else if (row==m_RAtmRow) pg->SetItemBkColour(row, *(idx+8), CL_RATM);
			pg->RedrawCell(row, *(idx+8), pDC);
		}
	}
	else if (m_style==SCR_TYPE5)
	{
		int *idx = NULL;
		int col_idx[] = {0,1,2,3,4,5,6,7,8};
		int put_idx[] = {15,14,13,12,11,10,9,7,8};
		if      (pod->code[1]=='2' || pod->code[1] == 'B' )	idx = col_idx;    //파생상품 코드개편
		else if (pod->code[1]=='3' || pod->code[1] == 'C') idx = put_idx;
		if (idx==NULL) return;

		if (pod->is_jega())
		{
			pg->SetItemFgColour(row, *(idx+0), GetColor(pod->gjga,pod->jega) );
			sprintf(buff, "%.2f", fabs(pod->jega));
			pg->SetItemText(row, *(idx+0), (pod->jega != 0.0) ? buff : "");
			pg->RedrawCell(row, *(idx+0), pDC);
		}

		if (pod->is_koga())
		{
			pg->SetItemFgColour(row, *(idx+1), GetColor(pod->gjga,pod->koga) );
			sprintf(buff, "%.2f", fabs(pod->koga));
			pg->SetItemText(row, *(idx+1), (pod->koga != 0.0) ? buff : "");
			pg->RedrawCell(row, *(idx+1), pDC);
		}
		
		if (pod->is_siga())
		{
			pg->SetItemFgColour(row, *(idx+2), GetColor(pod->gjga,pod->siga) );
			sprintf(buff, "%.2f", fabs(pod->siga));
			pg->SetItemText(row, *(idx+2), (pod->siga != 0.0) ? buff : "");
			pg->RedrawCell(row, *(idx+2), pDC);
		}

		if (pod->is_mgjv())
		{
			pg->SetItemText(row, *(idx+3), (pod->mgjv) ? Int2CommaStr(pod->mgjv) : "");
			pg->RedrawCell(row, *(idx+3), pDC);
		}

		if (pod->is_diff())
		{
			pg->SetItemFgColour(row, *(idx+5), GetColor(pod->diff) );
			sprintf(buff, "%s %.2f", GetDiffGiho(pod->diff), fabs(pod->diff));
			pg->SetItemText(row, *(idx+5), (pod->diff!=0.0) ? buff : "");
			pg->RedrawCell(row, *(idx+5), pDC);
		}

		if(m_chkPrd->GetCheck())
		{
			if (pod->is_ycurr())
			{
				pg->SetItemFgColour(row, *(idx+6), GetColor(pod->diff) );
				sprintf(buff, "%.2f", fabs(pod->ycurr));
				pg->SetItemText(row, *(idx+6), (pod->ycurr != 0.0) ? buff : "");
				pg->RedrawCell(row, *(idx+6), pDC);
			}

			if (pod->is_ycuvl())
			{
				pg->SetItemText(row, *(idx + 4), (pod->ycuvl) ? Int2CommaStr(pod->ycuvl) : "");
				pg->RedrawCell(row, *(idx + 4), pDC);
			}

		}
		else
		{
			if (pod->is_curr())
			{
				pg->SetItemFgColour(row, *(idx+6), GetColor(pod->diff) );
				sprintf(buff, "%.2f", fabs(pod->curr));
				pg->SetItemText(row, *(idx+6), buff);
				pg->RedrawCell(row, *(idx+6), pDC);
			}

			if (pod->is_gvol())
			{
				pg->SetItemText(row, *(idx + 4), (pod->gvol) ? Int2CommaStr(pod->gvol) : "");
				pg->RedrawCell(row, *(idx + 4), pDC);
			}
		}
		
		if (pod->is_hsga())
		{
			sprintf(buff, "%.2f", fabs(pod->hsga));
			pg->SetItemText(row, *(idx+7), buff);
			if (row==m_AtmRow) pg->SetItemBkColour(row, *(idx+7), CL_ATMG);
			else if (row==m_RAtmRow) pg->SetItemBkColour(row, *(idx+7), CL_RATM);
			pg->RedrawCell(row, *(idx+7), pDC);
		}
		
		if (pod->is_jisu())
		{
			sprintf(buff, "%.2f", floor(pod->jisu*100.0)/100.0);
			pg->SetItemText(row, *(idx+8), buff);
			if (row==m_AtmRow) pg->SetItemBkColour(row, *(idx+8), CL_ATMG);
			else if (row==m_RAtmRow) pg->SetItemBkColour(row, *(idx+8), CL_RATM);
			pg->RedrawCell(row, *(idx+8), pDC);
		}
	}

	if (bRelease) pg->ReleaseDC(pDC);
}

void CMapWnd::ParseAlert( LPCSTR dat, int len )
{

	int sym=0, pack_cnt=0, n=0;
	CString key;
	LPCSTR st = dat;
	LPCSTR ed = st + len;
	LPCSTR pos;
	vector<StringProxy> lines;

	// Key check
	pos = find(st, ed, '\t');
	key = CString(st, pos-st);

	OPTION_DATA *pod = NULL;
	FUTURE_DATA *pfd = NULL;
	JISU_DATA   *pjd = NULL;
	m_odat.Lookup(key, pod);
	m_fdat.Lookup(key, pfd);
	m_jdat.Lookup(key, pjd);

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
		st = lines[n].first;
		ed = lines[n].second;
		
		CMap<int,int,StringProxy, StringProxy&> rdat;
		while(st<ed)
		{
			pos = find(st, ed, '\t');
			sym = Str2Int(st, pos-st);
			st = pos + 1;
			if (st>ed) break;
			pos = find(st, ed, '\t');
			rdat.SetAt(sym, StringProxy(st, pos));
			st = pos + 1;
		}
		
		StringProxy sp;
		if (pod)
		{
			if (rdat.Lookup(206, sp)) pod->set_mdif(Str2Int(sp));
			if (rdat.Lookup(201, sp)) pod->set_mgjv(Str2Int(sp));
			if (rdat.Lookup( 27, sp)) pod->set_gvol(Str2Int(sp));
			if (rdat.Lookup( 26, sp)) pod->set_msga(fabs(Str2Double(sp)));
			if (rdat.Lookup( 25, sp)) pod->set_mdga(fabs(Str2Double(sp)));
			if (rdat.Lookup( 33, sp)) pod->set_rate(Str2Double(sp));
			if (rdat.Lookup( 24, sp)) pod->set_diff(Str2Double(sp));
			if (rdat.Lookup( 23, sp)) pod->set_curr(fabs(Str2Double(sp)));
			//if (pod->b_curr) GenOptionData(pod);
			DrawOptionData(pod);
		}
		else if (pfd)
		{
			BOOL bFlag = FALSE;
			CString strTime;

			const BOOL bNearCode = (m_fcodes.GetAt(0)==key);
			if (rdat.Lookup( 23, sp)) 
			{
				pfd->set_curr(fabs(Str2Double(sp)));
				if (bNearCode) m_opcal.SetFutureCur(sp.ToCStr());
			}
			if (rdat.Lookup( 24, sp)) pfd->set_diff(Str2Double(sp));
			if (rdat.Lookup( 33, sp)) pfd->set_rate(Str2Double(sp));
			if (rdat.Lookup(202, sp)) pfd->set_irga(Str2Double(sp));
			if (rdat.Lookup(205, sp)) pfd->set_gryl(Str2Double(sp));
			if (rdat.Lookup(204, sp)) pfd->set_bsis(Str2Double(sp));
			if (rdat.Lookup(203, sp)) pfd->set_isis(Str2Double(sp));
			if (rdat.Lookup( 27, sp)) pfd->set_gvol(Str2Int(sp));
			if (rdat.Lookup(201, sp)) pfd->set_mgjv(Str2Int(sp));
			if (rdat.Lookup(206, sp)) pfd->set_mdif(Str2Int(sp));

			if (bNearCode && pfd->b_curr)
			{
				if (rdat.Lookup(34, sp)) 
				{	
					strTime = sp.ToCStr();
					
					strTime.Replace(":","");

					strTime.TrimLeft();
					strTime.TrimRight();
				}

				if(m_bClose)
				{					
					CString s;
					//s.Format("[CODE : %s TIME : %s]\n",key,strTime);
					//OutputDebugString(s);
					
					const CTime t = CTime::GetCurrentTime();
					
					const int year = t.GetYear();
					const int month = t.GetMonth();
					const int day = t.GetDay();

					if(strTime.GetLength() == 6)
					{
						CString hour = strTime.Mid(0,2);
						CString minute = strTime.Mid(2,2);
						CString sec = strTime.Mid(4,2);
						
						const time_t interval = ConvertSecondTime(year,month,day,atoi(hour),atoi(minute),atoi(sec)) - ConvertSecondTime(year,month,day,atoi(strPrevHour),atoi(strPrevMin),atoi(strPrevSec));
// 						s.Format("[INTERVAL : %d]\n",interval);
// 						OutputDebugString(s);
						if(interval >= 2 || interval < -2)
						{
							strPrevHour = hour;
							strPrevMin = minute;
							strPrevSec = sec;
							bFlag = TRUE;
						}
					}
				}
				
// 				if(sp.ToCStr().Find("장") > -1)
// 				{
// 					m_bClose = TRUE;
// 				}
				if(strTime != "" && atoi(strTime) >= 145000)
				{
					m_bClose = TRUE;
				}

				GenFutureData();

				if(bFlag)
					GenOptionData();
				DrawOptionData(NULL);
			}
			DrawFutureData(pfd);
		}
		else if (pjd)
		{
			BOOL K200Flag = FALSE;

			const BOOL bK200 = (key=="K2001");
			if (rdat.Lookup(23, sp)) 
			{
				pjd->set_curr(fabs(Str2Double(sp)));
				if (bK200) m_opcal.SetK200Cur(sp.ToCStr());
			}
			if (rdat.Lookup(34, sp))
			{
				CString time = sp.ToCStr();
				time.Replace(":", "");
				sprintf(pjd->time, time);
				if (bK200) m_opcal.SetTime(time);

				if(bK200 && strPrevK200Time != time)
				{
					//CString s;
					//s.Format("[TIME : %s]\n",time);
					//OutputDebugString(s);
	
					strPrevK200Time = time;
					K200Flag = TRUE;
				}
			}
			if (bK200 && K200Flag)
			{
				CalcRAtm();
				GenFutureData();
				GenOptionData();
				DrawOptionData(NULL);
			}
		}
	}
}

void CMapWnd::ParseAlert( struct _alertR* alertR)
{
	DWORD* data = (DWORD*)alertR->ptr[0];

	CString sGubn = (char*)data[0];

	if (sGubn.FindOneOf("BD") != -1)
		return;

//AxStd::_Msg("[%s][%s]", sGubn, alertR->code);

	OPTION_DATA *pod = nullptr;
	FUTURE_DATA *pfd = nullptr;
	JISU_DATA   *pjd = nullptr;
	m_odat.Lookup(alertR->code, pod);
	m_fdat.Lookup(alertR->code, pfd);
	m_jdat.Lookup(alertR->code, pjd);
		
	CString sp;
	if (pod)
	{
		if (data[206]) 
		{
			sp = (char*)data[206];
			pod->set_mdif(Str2Int(sp));
		}
		if (data[201]) 
		{
			sp = (char*)data[201];
			pod->set_mgjv(Str2Int(sp));
		}

		if(!m_chkPrd->GetCheck())
		{
			if (data[27]) 
			{
				sp = (char*)data[27];
				pod->set_gvol(Str2Int(sp));
			}
		}

		if (data[26]) 
		{
			sp = (char*)data[26];
			pod->set_msga(fabs(Str2Double(sp)));
		}
		if (data[25]) 
		{
			sp = (char*)data[25];
			pod->set_mdga(fabs(Str2Double(sp)));
		}

		if(data[29])
		{
			sp = (char*)data[29];
			pod->set_siga(Str2Double(sp));
		}

		if(data[30])
		{
			sp = (char*)data[30];
			pod->set_koga(Str2Double(sp));
		}

		if(data[31])
		{
			sp = (char*)data[31];
			pod->set_jega(Str2Double(sp));
		}

		if(m_chkPrd->GetCheck())
		{
			if (data[116]) 
			{
				sp = (char*)data[116];
				pod->set_rate(Str2Double(sp));
			}
		}
		else
		{
			if (data[33]) 
			{
				sp = (char*)data[33];
				pod->set_rate(Str2Double(sp));
			}
		}

		if(m_chkPrd->GetCheck())
		{
			if (data[115]) 
			{
				sp = (char*)data[115];
				pod->set_diff(Str2Double(sp));

// 					if(alertR->code == "X301GC262" || alertR->code == "X301GC265")
// 					{
// 						CString ss;
// 						ss.Format("4013 DIFF : [%s] [%s] [%s]\n",alertR->code,(char*)data[111],sp);
// 						OutputDebugString(ss);
// 					}
			}
		}
		else
		{
			if (data[24]) 
			{
				sp = (char*)data[24];
				pod->set_diff(Str2Double(sp));
			}
		}

		if(m_chkPrd->GetCheck())
		{
			if (data[111]) 
			{
				sp = (char*)data[111];
				pod->set_ycurr(fabs(Str2Double(sp)));
			}

			if (data[112])  //0422
			{
				sp = (char*)data[112];
				sp.Trim();
				pod->set_ycuvl(fabs(Str2Int(sp)));
			}
		}
		else
		{
			if (data[23]) 
			{
				sp = (char*)data[23];
				pod->set_curr(fabs(Str2Double(sp)));
			}
		}
		//if (pod->b_curr) GenOptionData(pod);
		DrawOptionData(pod);
	}
	else if (pfd)
	{
		BOOL bFlag = FALSE;
		CString strTime;

		const BOOL bNearCode = (m_fcodes.GetAt(0)==alertR->code);

		if(m_chkPrd->GetCheck())
		{
			if (data[111]) 
			{
				sp = (char*)data[111];
				pfd->set_ycurr(fabs(Str2Double(sp)));
				if (bNearCode)
				{
					if(data[23])
					{
						sp = (char*)data[23];
						m_opcal.SetFutureCur(sp);
					}
				}
			}
		}
		else
		{
			if (data[23]) 
			{
				sp = (char*)data[23];
				pfd->set_curr(fabs(Str2Double(sp)));
				if (bNearCode) m_opcal.SetFutureCur(sp);
			}
		}

		if(m_chkPrd->GetCheck())
		{
			if (data[115]) 
			{
				sp = (char*)data[115];
				pfd->set_diff(Str2Double(sp));
			}
		}
		else
		{
			if (data[24])
			{
				sp = (char*)data[24];
				pfd->set_diff(Str2Double(sp));
			}
		}
			
		if(m_chkPrd->GetCheck())
		{
			if (data[116]) 
			{
				sp = (char*)data[116];
				pfd->set_rate(Str2Double(sp));
			}
		}
		else
		{
			if (data[33])
			{
				sp = (char*)data[33];
				pfd->set_rate(Str2Double(sp));
			}
		}
			
		if (data[202])
		{
			sp = (char*)data[202];
			pfd->set_irga(Str2Double(sp));
		}
		if (data[205])
		{
			sp = (char*)data[205];
			pfd->set_gryl(Str2Double(sp));
		}
		if (data[204])
		{
			sp = (char*)data[204];
			pfd->set_bsis(Str2Double(sp));
		}
		if (data[203])
		{
			sp = (char*)data[203];
			pfd->set_isis(Str2Double(sp));
		}
			
		if(!m_chkPrd->GetCheck())
		{
			if (data[27])
			{
				sp = (char*)data[27];
				pfd->set_gvol(Str2Int(sp));
			}
		}

		if (data[201])
		{
			sp = (char*)data[201];
			pfd->set_mgjv(Str2Int(sp));
		}
		if (data[206])
		{
			sp = (char*)data[206];
			pfd->set_mdif(Str2Int(sp));
		}

		if (bNearCode && pfd->b_curr)
		{
			if (data[34]) 
			{	
				sp = (char*)data[34];
				strTime = sp;
					
				strTime.Replace(":","");

				strTime.TrimLeft();
				strTime.TrimRight();
			}

			if(m_bClose)
			{					
				CString s;
					
				const CTime t = CTime::GetCurrentTime();
					
				const int year = t.GetYear();
				const int month = t.GetMonth();
				const int day = t.GetDay();

				if(strTime.GetLength() == 6)
				{
					CString hour = strTime.Mid(0,2);
					CString minute = strTime.Mid(2,2);
					CString sec = strTime.Mid(4,2);
						
					const time_t interval = (int)ConvertSecondTime(year,month,day,atoi(hour),atoi(minute),atoi(sec)) - ConvertSecondTime(year,month,day,atoi(strPrevHour),atoi(strPrevMin),atoi(strPrevSec));
					if(interval >= 2 || interval < -2)
					{
						strPrevHour = hour;
						strPrevMin = minute;
						strPrevSec = sec;
						bFlag = TRUE;
					}
				}
			}
				
			if(strTime != "" && atoi(strTime) >= 145000)
			{
				m_bClose = TRUE;
			}

			GenFutureData();

			if(bFlag)
				GenOptionData();
			DrawOptionData(NULL);
		}
		DrawFutureData(pfd);
	}
	else if (pjd)
	{
		BOOL K200Flag = FALSE;

		const BOOL bK200 = (alertR->code=="K2001");
		if (data[23]) 
		{
			sp = (char*)data[23];
			pjd->set_curr(fabs(Str2Double(sp)));
			if (bK200) m_opcal.SetK200Cur(sp);
		}
		if (data[34])
		{
			sp = (char*)data[34];
			CString time = sp;
			time.Replace(":", "");
			sprintf(pjd->time, time);
			if (bK200) m_opcal.SetTime(time);

			if(bK200 && strPrevK200Time != time)
			{	
				strPrevK200Time = time;
				K200Flag = TRUE;
			}
		}
		if (bK200 && K200Flag)
		{
			CalcRAtm();
			GenFutureData();
			GenOptionData();
			DrawOptionData(NULL);
		}
	}
}

time_t CMapWnd::ConvertSecondTime(int parm_year, int parm_month, int parm_day,int parm_hour, int parm_minute, int parm_second)
{
    struct tm base_time;
	
    base_time.tm_sec = parm_second;
    base_time.tm_min = parm_minute;
    base_time.tm_hour = parm_hour;
    base_time.tm_mday = parm_day;
    base_time.tm_mon = parm_month - 1;
	
    // 1900년 부터 지정시간까지의 초단위로 계산하므로..
    base_time.tm_year = parm_year - 1900;
    base_time.tm_isdst = 0;
	
    // 초단위로 변경하여 리턴한다.
    return mktime(&base_time);
}

void CMapWnd::LoadFutureData()
{
	FILE *fp = NULL;

	CString strFile;

	m_fcodes.RemoveAll();

	if(m_cbMarket->GetCurSel() == 0)
	{
		strFile = "\\tab\\fjcode.dat";
	}
	else if(m_cbMarket->GetCurSel() == 1)
	{
		strFile = "\\tab\\mfcode.dat";
	}
	else if(m_cbMarket->GetCurSel() == 2)  //weekly
	{
		strFile = "\\tab\\fjcode.dat";
	}
	
	FUTURE_HEADER fhead{};
	do {
		fp = fopen(m_home + strFile, "rb");
		if (!fp) break;
		
		while(TRUE)
		{
			const size_t len = fread(&fhead, 1, sizeof(fhead), fp);
			if (len!=sizeof(fhead)) break;
			
			CString code(fhead.codx, sizeof(fhead.codx));
			if (code.GetAt(0)=='1')
			{
				m_fcodes.Add(code);
			}
		}
	} while(FALSE);
	
	if (fp) fclose(fp);
}

void CMapWnd::ParseJISU( LPCSTR dat, int len )
{
	vector<StringProxy> fds;
	ParseString(dat, dat+len, 0x09, fds);

	if (fds.size()!=4) return;

	// 20021\t20023\t20306\t20034
	// 코드, 지수, 전일지수, 시간

	CString code = fds[0].ToCStr();
	JISU_DATA *pjd;
	
	if (!m_jdat.Lookup(code, pjd))
		pjd = new JISU_DATA();	

	sprintf(pjd->code, code);
	pjd->set_curr( fabs(Str2Double(fds[1])) );
	pjd->set_pcur( fabs(Str2Double(fds[2])) );
	sprintf(pjd->time, fds[3].ToCStr());
	
	m_jdat.SetAt(code, pjd);

	if (code=="K2001")
	{
		m_opcal.SetK200Cur(fds[1].ToCStr());
		CString time = fds[3].ToCStr();
		time.Replace(":", "");
		m_opcal.SetTime(time);

		if(time.Find("장") > -1)
		{
			//OutputDebugString(time);
			//OutputDebugString("[JISU CLOSE]\n");
			m_bClose = TRUE;
		}
	}

	CalcRAtm();
	GenFutureData();
	GenOptionData();
	DrawOptionData(NULL);
}

void CMapWnd::ParseFUTURE( LPCSTR dat, int len )
{
	vector<StringProxy> fds;


	ParseString(dat, dat+len, 0x09, fds);

	if (fds.size()!=13) return;
	// 30021\t30022\t31111\t31023\t31024\t31204\t31027\t31201\t31206\t31033\t31203\t31205\t31202\t
	// 코드, 종목명, 예상가, 현재가, 대비, 시장Basis, 거래량, 미결제약정, 미결제증감, 등락률, 이론Basis, 괴리율, 이론가

	FUTURE_DATA *pfd;
	CString code = fds[0].ToCStr();
	if (!m_fdat.Lookup(code, pfd))
		pfd = new FUTURE_DATA();

	CString name = fds[1].ToCStr();
	name.TrimRight();
	
	pfd->set_code( code );
	pfd->set_name( name );

	pfd->b_ycurr = TRUE;
	pfd->set_ycurr( fabs(Str2Double(fds[2])) );
	pfd->set_curr( fabs(Str2Double(fds[3])) );

	//pfd->set_ycurr( fabs(Str2Double(fds[2])));

	pfd->b_diff = TRUE;

	pfd->set_diff( Str2Double(fds[4]) );
	pfd->set_bsis( fabs(Str2Double(fds[5])) );

	pfd->b_gvol = TRUE;

	if (m_chkPrd->GetCheck())
	{
		pfd->set_ycuvl(Str2Int(fds[6]));
	}
	else
	{
		pfd->set_gvol(Str2Int(fds[6]));
	}
	
	pfd->set_mgjv( Str2Int(fds[7]) );
	pfd->set_mdif( Str2Int(fds[8]) );

	pfd->b_rate = TRUE;

	pfd->set_rate( Str2Double(fds[9]) );
	pfd->set_isis( Str2Double(fds[10]) );
	pfd->set_gryl( Str2Double(fds[11]) );
	pfd->set_irga( Str2Double(fds[12]) );
	
	for(int n=0; n<m_fcodes.GetSize(); ++n)
	{
		if (m_fcodes.GetAt(n)==code)
		{
			pfd->grid_row = n+1;
			break;
		}
	}

	if (m_fcodes.GetAt(0)==code)
	{
		char buff[16];
		sprintf(buff, "%.2f", pfd->curr);
		m_opcal.SetFutureCur(buff);
	}

	m_fdat.SetAt(code, pfd);

	CalcRAtm();
	GenFutureData();
	GenOptionData();
	DrawFutureData(pfd);
	DrawOptionData(NULL);
}

void CMapWnd::ClearData()
{
	m_cdkr = m_bjis = m_clday = m_bzday = m_clday2 = m_bzday2 = m_basic = 0.0;
	m_AtmRow = m_RAtmRow = -1;

	m_hsga.RemoveAll();
	{
		CString key;
		JISU_DATA* pjd{};
		POSITION pos = m_jdat.GetStartPosition();
		while(pos!=NULL)
		{
			m_jdat.GetNextAssoc(pos, key, pjd);
			delete pjd;
		}
		m_jdat.RemoveAll();
	}
	{
		CString key;
		FUTURE_DATA* pfd{};
		POSITION pos = m_fdat.GetStartPosition();
		while(pos!=NULL)
		{
			m_fdat.GetNextAssoc(pos, key, pfd);
			delete pfd;
		}
		m_fdat.RemoveAll();
	}
	{
		CString key;
		OPTION_DATA* pod{};
		POSITION pos = m_odat.GetStartPosition();
		while(pos!=NULL)
		{
			m_odat.GetNextAssoc(pos, key, pod);
			delete pod;
		}
		m_odat.RemoveAll();
	}
}

void CMapWnd::ParseOption( LPCSTR dat, int len )
{
	vector<StringProxy> fds;
	ParseString(dat, dat+len, 0x09, fds);

	if (fds.size()!=8) return;

	// 40332\t40323\t40314\t40370\t40361\t40369\t40368\t40367
	// CD금리, 배당지수, 잔존일수(달력), 잔존일수(영업), 역사적변동성 90, 60, 30, 21
	
	char buff[64]{};
	
	m_opcal.SetHv(fds[4].ToCStr(), fds[5].ToCStr(), fds[6].ToCStr(), fds[7].ToCStr());
	m_cdkr = Str2Double(fds[0]);  sprintf(buff, "%.2f", m_cdkr);  m_opcal.SetCd(buff);
	m_bjis = Str2Double(fds[1])/1000000.0;  sprintf(buff, "%.6f", m_bjis);  m_opcal.SetBedangJisu(buff);
	m_clday = Str2Double(fds[2]); sprintf(buff, "%.2f", m_clday); m_opcal.SetCalRemainDays(buff);
	m_bzday = Str2Double(fds[3]); sprintf(buff, "%.2f", m_bzday); m_opcal.SetBzRemainDays(buff);

	CalcRAtm();
	GenFutureData();
	GenOptionData();
	DrawOptionData(NULL);
}

void CMapWnd::LoadCfg()
{
	m_opcal.SetUserName(m_name);
	m_opcal.ReadCfg();

	m_opt_tval = atoi(m_cfg.ReadCfg("TimeValue"));
	m_opt_bptype = atoi(m_cfg.ReadCfg("BPType"));
	m_opt_k200rate = atof(m_cfg.ReadCfg("K200Rate"));
	m_opt_rdtype = atoi(m_cfg.ReadCfg("RDType"));
	m_opt_stim = GetMinTime(m_cfg.ReadCfg("TimeStart"));
	m_opt_etim = GetMinTime(m_cfg.ReadCfg("TimeEnd"));
}

int CMapWnd::GetMinTime(LPCSTR val)
{
	CString sval = val;
	if (sval.GetLength() != 4)
		return 0;
	return atoi(sval.Left(2))*60 + atoi(sval.Right(2));
}

void CMapWnd::GenFutureData()
{
	JISU_DATA *pjd = NULL;
	FUTURE_DATA *pfd = NULL;

	if (!m_jdat.Lookup("K2001", pjd)) return;
	if (!m_fdat.Lookup(m_fcodes.GetAt(0), pfd)) return;

	if (m_opt_tval==1)
	{
		char buff[64]{};
		double ctime=0.0, tloss = 0.0;

		if      (strcmp("장마감", pjd->time)==0) ctime = GetMinTime("1500");
		else if (strcmp("장종료", pjd->time)==0) ctime = GetMinTime("1600");
		else                                     ctime = GetMinTime(pjd->time);

		if (ctime > m_opt_stim)
		{
			if (ctime > m_opt_etim) ctime = m_opt_etim;
			if (m_opt_stim < m_opt_etim)  tloss = (ctime - m_opt_stim) / (m_opt_etim - m_opt_stim);
		}
		
		sprintf(buff, "%.2f", m_clday - tloss); m_clday2 = atof(buff);
		sprintf(buff, "%.2f", m_bzday - tloss); m_bzday2 = atof(buff);
	}
	else
	{
		m_clday2 = m_clday;
		m_bzday2 = m_bzday;
	}

	switch(m_opt_bptype)
	{
	case 1: 
		m_basic = pfd->curr;
		break;
	case 2:
		m_basic = (fabs(pjd->curr - m_bjis) * m_opt_k200rate) + (fabs(pfd->curr) * (100.0-m_opt_k200rate)) / 100.0;
		break;
	case 3:
		if (m_opt_rdtype) m_basic = fabs(pfd->curr) * exp(-m_cdkr / 100.0 * fabs(m_clday2) / 365.0);
		else              m_basic = fabs(pfd->curr) * exp(-m_cdkr / 100.0 * fabs(m_bzday2) / 250.0);
		break;
	default:
		m_basic = fabs(pjd->curr) - m_bjis;
		break;
	}
}

void CMapWnd::GenOptionData(OPTION_DATA *pod)
{

	JISU_DATA* pK200{}, * pKOSPI{};
	if (m_cdkr==0.0) return;
	if (!m_jdat.Lookup("K2001", pK200))  return;
	if (!m_jdat.Lookup("K0001", pKOSPI)) return;

	if (pod==NULL)
	{
		CString key;
		POSITION pos = m_odat.GetStartPosition();
		while(pos!=NULL)
		{
			m_odat.GetNextAssoc(pos, key, pod);
			GenOptionData(pod);
		}
		return;
	}
	else
	{
		char buff[64]{};
		const double strk = GetOptionStrike(pod->code);
		const double jcnv = fabs(pKOSPI->curr) * strk / fabs(pK200->curr);
		pod->set_jisu(jcnv);

		double sggc = fabs(pod->curr);
		sggc -= (pod->code[1]=='2' || pod->code[1] == 'B') ? max(pK200->curr - strk, 0.0) : max(strk - pK200->curr, 0.0);
		pod->set_sggc(sggc);

		
		m_opcal.SetJCode(pod->code);
		sprintf(buff, "%.2f", pod->curr); m_opcal.SetCurrValue(buff);
		sprintf(buff, "%.1f", strk);      m_opcal.SetStrike(buff);
		m_opcal.Calc(0);
		
		double dIV =0.0;
		double dDelta = 0.0;
		double dGamma = 0.0;
		double dTheta = 0.0;
		double dVegga = 0.0;
		double dRho = 0.0;
		double dTheoryValue = 0.0;
		// 미니선옵 추가 부분
		dIV = atof(m_opcal.m_IOpSensCal->GetIv());
		dDelta = atof(m_opcal.m_IOpSensCal->GetDelta());
		dGamma = atof(m_opcal.m_IOpSensCal->GetGamma());
		dTheta = atof(m_opcal.m_IOpSensCal->GetTheta());
		dVegga = atof(m_opcal.m_IOpSensCal->GetVegga());
		dRho = atof(m_opcal.m_IOpSensCal->GetRho());
		dTheoryValue = atof(m_opcal.m_IOpSensCal->GetTheoryValue());

		pod->set_iv  ( dIV );
		pod->set_delt( dDelta );
		pod->set_gmma( dGamma );
		pod->set_teta( dTheta );
		pod->set_vega( dVegga );
		pod->set_rho ( dRho );
		pod->set_irga( dTheoryValue );
		pod->set_grdo( pod->curr - pod->irga );
	}
}

double CMapWnd::GetOptionStrike( LPCSTR code )
{
	double dval = atof(code+6);
	if (code[8]=='2' || code[8]=='7') dval += 0.5;
	return dval==0 ? 1.0 : dval;
}

void CMapWnd::OnScreenType1()
{
	SelectOptionGrid(SCR_TYPE1);

	KillTimer(1000);
}

void CMapWnd::OnScreenType2()
{
	SelectOptionGrid(SCR_TYPE2);

	KillTimer(1000);
}

void CMapWnd::OnScreenType3()
{
	SelectOptionGrid(SCR_TYPE3);
	
	KillTimer(1000);
}

void CMapWnd::OnScreenType4()
{
	SelectOptionGrid(SCR_TYPE4);	

	if(m_style == SCR_TYPE4)
	{
		SetTimer(1000,10000,NULL);
	}
}

void CMapWnd::OnScreenType5()
{
	SelectOptionGrid(SCR_TYPE5);	
	
	KillTimer(1000);
}

void CMapWnd::OnMonthChange()
{
	Send();
}

void CMapWnd::OnMarketChange()
{
	LoadOptionData();

	if(m_cbMonth->GetCount() == 0)
	{
		if(m_cbMarket->GetCurSel() == 2)  //20190923 weekly
			MessageBox("선택하신 기간에 상장된 위클리 옵션종목이 없습니다.","IBK투자증권", MB_OK|MB_ICONERROR);
		else
			MessageBox("옵션데이타가 존재하지 않습니다.","IBK투자증권", MB_OK|MB_ICONERROR);
		
		m_cbMarket->SetCurSel(0);

		OnMarketChange();
		return;
	}

	LoadFutureData();

	if(m_fcodes.GetSize() == 0)
	{
		MessageBox("선물데이타가 존재하지 않습니다.","IBK투자증권", MB_OK|MB_ICONERROR);

		m_cbMarket->SetCurSel(0);

		OnMarketChange();
		return;
	}

	for(int i=m_gdFuture->GetRowCount()-1;i>0;i--)
	{
		m_gdFuture->DeleteRow(i);
	}

	m_gdFuture->SetRowCount(8);

	Send();
}

void CMapWnd::OnGreek()
{
	if (m_cfg.ShowDlg())
	{
		m_opcal.ReadCfg();
		Send();
	}
}

void CMapWnd::OnConf()
{
	CScreenSetDlg dlg(this, m_home, m_name, m_pParent);
	if (dlg.DoModal()==IDOK)
	{
		LoadScreenConfig();
	}
}

void CMapWnd::LoadScreenConfig()
{
	CString sFile;
	sFile.Format("%s/%s/%s/%s", m_home, USDIR, m_name, USER_CONFIG);

	int	readL = 0;
	char	readB[1024];
	
	// 클릭 이벤트
	readL = GetPrivateProfileString(USER_SECTION, USER_EVENT_KEY, "2", readB, sizeof(readB), sFile);
	if (readL > 0)
	{
		m_cEvent = CString(readB, readL);
	}
	
	// 옵션 연결 화면 번호
	readL = GetPrivateProfileString(USER_SECTION, USER_SCREEN_NO_KEY, DEFAULT_SCREEN, readB, sizeof(readB), sFile);
	if (readL > 0)
	{
		m_cOScreen = CString(readB, readL);
	}
	
	// 선물 연결 화면 번호
	readL = GetPrivateProfileString(USER_SECTION, USER_FSCREEN_NO_KEY, DEFAULT_FSCREEN, readB, sizeof(readB), sFile);
	if (readL > 0)
	{
		m_cFScreen = CString(readB, readL);
	}

	readL = GetPrivateProfileString(USER_SECTION, USER_CX, "0", readB, sizeof(readB), sFile);
	if (readL > 0)
	{
		m_cCX = atoi(CString(readB, readL));
	}

	if(m_cCX < 1000)
	{
		m_cCX = 1000;
	}

	readL = GetPrivateProfileString(USER_SECTION, USER_CY, "0", readB, sizeof(readB), sFile);
	if (readL > 0)
	{
		m_cCY = atoi(CString(readB, readL));
	}

	readL = GetPrivateProfileString(USER_SECTION, USER_STYLE, "0", readB, sizeof(readB), sFile);
	if (readL > 0)
	{
		m_cStyle = atoi(CString(readB, readL));
		if (m_cStyle<0 || m_cStyle>=SCR_TYPES)
			m_cStyle = SCR_TYPE1;
	}

	readL = GetPrivateProfileString(USER_SECTION, USER_KIND, "0", readB, sizeof(readB), sFile);
	if (readL > 0)
	{
		m_cKind = atoi(CString(readB, readL));
	}
}

void CMapWnd::SaveScreenConfig()
{
	CString sFile, dat;
	sFile.Format("%s/%s/%s/%s", m_home, USDIR, m_name, USER_CONFIG);

	CRect rc;
	GetClientRect(&rc);

	dat.Format("%d", rc.Width());
	WritePrivateProfileString(USER_SECTION, USER_CX, dat, sFile);

	dat.Format("%d", rc.Height());
	WritePrivateProfileString(USER_SECTION, USER_CY, dat, sFile);

	dat.Format("%d", m_style);
	WritePrivateProfileString(USER_SECTION, USER_STYLE, dat, sFile);

	dat.Format("%d", m_cbMarket->GetCurSel());
	WritePrivateProfileString(USER_SECTION, USER_KIND, dat, sFile);
}

void CMapWnd::CalcRAtm()
{
	JISU_DATA *pjd;
	if (!m_jdat.Lookup("K2001", pjd)) return;
	if (!pjd->b_curr) return;

	const int hsiz = m_hsga.GetSize();
	for(int n=0; n<hsiz; ++n)
	{
		if (pjd->curr > m_hsga.GetAt(n))
		{
			const double val1 = fabs( m_hsga.GetAt(n) - pjd->curr );
			const double val2 = fabs( m_hsga.GetAt(n-1) - pjd->curr );

			const int row = (val1>val2) ? (n) : (n+1);
			if (m_RAtmRow!=row && m_AtmRow!=row)
			{
				const int col = (m_style==SCR_TYPE1) ? 8 : 7;
				m_gdOption->SetItemBkColour(m_RAtmRow, col, CL_GRAY);
				m_gdOption->SetItemBkColour(row, col, CL_RATM);
				m_gdOption->RedrawCell(row, col);
				m_RAtmRow = row;
			}
			break;
		}
	}
}

void CMapWnd::ChangeTheme()
{
	m_crCtrlBk = GetIndexColor(COLOR_CONTROLBACK);

	int col=0;
	const COLORREF crHeadBk = GetIndexColor(74);
	const COLORREF crHeadFg = GetIndexColor(76);
	
	MakeBackground();

	for(col=0; col<m_gdOption->GetColumnCount(); ++col)
	{
		m_gdOption->SetItemBkColour(0, col, crHeadBk);
		m_gdOption->SetItemFgColour(0, col, crHeadFg);
	}
	m_gdOption->RedrawRow(0);

	for(col=0; col<m_gdFuture->GetColumnCount(); ++col)
	{
		m_gdFuture->SetItemBkColour(0, col, crHeadBk);
		m_gdFuture->SetItemFgColour(0, col, crHeadFg);
	}
	m_gdFuture->RedrawRow(0);

	m_pBrBk = GetAxBrush(m_crCtrlBk);

	for(int n=0; n<SCR_TYPES; ++n)
		m_rdType[n]->Invalidate(TRUE);

	m_stMonth->SetBkColor(m_crCtrlBk);
	m_stMonth->SetBorderColor(m_crCtrlBk);

	Invalidate(TRUE);
}

void CMapWnd::OnCheckPredict()
{
	if(m_chkPrd->GetCheck())
	{
		m_nIndex = 5;

		SelectFutureGrid(1);
	}
	else
	{
		SelectFutureGrid(0);

		m_nIndex = 0;
	}

	SelectOptionGrid(m_style);

	Send();
}

void CMapWnd::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == 1000)
	{
		Send();
	}
	
	CWnd::OnTimer(nIDEvent);
}

void CMapWnd::MakeOptionWnd()
{
	CString mapN;
	mapN = "IB999987";
	const CString pdata[2] = {"IB401301", "IB401302"};
	CString strPush ;
	for(int ii = 0 ; ii < 2; ii++)
	{
//		m_pOptionwnd[ii] = new COptionWnd;
		m_pOptionwnd[ii] = std::make_shared<COptionWnd>();
		if (!m_pOptionwnd[ii]->Create(NULL, "SCREEN", WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_VSCROLL, CRect(0,0,1,1), this, (int)m_pOptionwnd[ii].get()))
			return;
		//int type = m_pParent->SendMessage(WM_USER, MAKEWPARAM(typeDLL, 0), (long) mapN.operator LPCTSTR());
		int type = m_pParent->SendMessage(WM_USER, MAKEWPARAM(typeDLL, 0), (long) pdata[ii].GetString());
		if( !(type & vtypeNRM))
			type = type | vtypeNRM;	
		strPush.Format("4013hwnd\t%d", (int)m_pOptionwnd[ii]->m_hWnd);
		Variant(pushCC, strPush);
		m_key[ii] = m_pParent->SendMessage(WM_USER, MAKEWPARAM(attachMAP, type), (long) m_pOptionwnd[ii].get());
		//BOOL ret = m_pParent->SendMessage(WM_USER, MAKEWPARAM(loadMAP, m_key[ii]), (long) mapN.operator LPCTSTR());
		const BOOL ret = m_pParent->SendMessage(WM_USER, MAKEWPARAM(loadMAP, m_key[ii]), (long)  pdata[ii].GetString());
	}
}