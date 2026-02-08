#include "stdafx.h"
#include "IB877000.h"
#include "MapWnd.h"

#include "dataio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMapWnd::CMapWnd(CWnd *pParent) : CAxisExt(pParent)
{
	m_home = Variant(homeCC, "");
	Variant(titleCC, "시장부 데이터 입력");
	LoadImage();

	m_pTabWnd = NULL;
}

void CMapWnd::OnDestroy() 
{
	//SafeDelete(m_pTabWnd);
	//SafeDelete(m_pSiHwangWnd);
	//SafeDelete(m_pTujajaWnd);

	CWnd::OnDestroy();
}

CMapWnd::~CMapWnd()
{
}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabChange)
END_MESSAGE_MAP()

void CMapWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	CRect rc;
	GetClientRect(&rc);

	m_rcBody.SetRect(0, 0, rc.right, rc.bottom-GUID_WND_HEIGHT);
	m_rcGuide.SetRect(0, rc.bottom-GUID_WND_HEIGHT, rc.right, rc.bottom);

	{
		CRect trc = m_rcBody;
		trc.DeflateRect(PADDING_VALUE, PADDING_VALUE);
		m_pTabWnd->MoveWindow( trc.left, trc.top, trc.Width(), TAB_WND_HEIGHT );
		
		if (m_pSiHwangWnd->GetSafeHwnd())
			m_pSiHwangWnd->MoveWindow( trc.left, trc.top+TAB_WND_HEIGHT, trc.Width(), trc.Height()-TAB_WND_HEIGHT);
		if (m_pTujajaWnd->GetSafeHwnd())
			m_pTujajaWnd->MoveWindow( trc.left, trc.top+TAB_WND_HEIGHT, trc.Width(), trc.Height()-TAB_WND_HEIGHT);
	}
	

	MakeBackground(rc.Width(), rc.Height());
}

LRESULT CMapWnd::OnUser( WPARAM wParam, LPARAM lParam )
{
	const int msg = LOBYTE(LOWORD(wParam));
	const int key = HIBYTE(LOWORD(wParam));
	const int len = HIWORD(wParam);
	const LPCSTR dat = (LPCSTR)lParam;

	
	switch(msg)
	{
	case DLL_INB:
		break;
	case DLL_OUBx:
		{
			struct _extTHx* extTHx = (struct _extTHx*)lParam;
			//CString slog;
			//slog.Format("[%d]   [%d]", extTHx->key, extTHx->size);
			
			if (extTHx->key == TK_PIBONEWS)
			{
				char* pBytes = extTHx->data;
				pBytes += L_ledgerH;
				
				::SendMessage(m_pSiHwangWnd->GetSafeHwnd(), WM_USER, wParam, (LPARAM)lParam);
				break;
			}
		}
		break;
	case DLL_OUB:
		switch(key)
		{
		case TK_PIBONEWS:
		case TK_PIBO7710:
		case TK_PIDOSIHW:
			::SendMessage(m_pSiHwangWnd->GetSafeHwnd(), WM_USER, wParam, lParam); 
			break;
		case TK_PIDOTUJA:
			::SendMessage(m_pTujajaWnd->GetSafeHwnd(), WM_USER, wParam, lParam); 
			break;
		}
		break;
		
	case DLL_ALERT:
		break;
		
	case DLL_TRIGGER:
		break;
		
	case DLL_NOTICE:
		break;
		
	case DLL_SETPAL:
	case DLL_SETFONT:
	case DLL_SETFONTx:
		break;
		
	case DLL_GUIDE:
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
	const int readL = GetPrivateProfileString(_T("General"), _T("Palette"), _T("Blue"), buff, sizeof(buff), path);
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

void CMapWnd::SetSize(int width, int height)
{
	SetWindowPos(NULL, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
	m_pParent->PostMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(width, height));
}

void CMapWnd::MakeBackground(int width, int height)
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
		TRACE("MakeBackground %d %d\n", width, height);
		
		mdc.CreateCompatibleDC(pdc);
		oldBmp = mdc.SelectObject(&m_bgBmp);
		
		// Drawing 
		DrawRoundRectangle(&mdc, m_rcBody);		// Body Window
		DrawRoundRectangle(&mdc, m_rcGuide);	// Guide Window
		
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

void CMapWnd::DrawRoundRectangle(CDC *pDC, CRect rc)
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

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this);
	Draw(&dc);
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

int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pTabWnd = std::make_unique<CfxTab>();
	m_pTabWnd->Create(CRect(0,0,0,TAB_WND_HEIGHT), this, IDC_TAB);
	m_pTabWnd->InsertItem(0, "투자자별 추정치");
	m_pTabWnd->InsertItem(1, "시황속보");
	m_pTabWnd->LoadTabImage(Variant(homeCC, ""), false);
	m_pTabWnd->SetStyle(0x00000001, false);
	m_pTabWnd->SetBorderColor(GetIndexColor(TAB_BORDER_INDEX));
	m_pTabWnd->SetColor(GetIndexColor(CLR_TAB_FORE), GetIndexColor(CONTENTS_BACK_INDEX), 
		GetIndexColor(CLR_TAB_SELFORE), GetIndexColor(CONTENTS_BACK_INDEX), GetIndexColor(CONTENTS_BACK_INDEX));
	m_pTabWnd->SetFont(m_pFont);

	m_pSiHwangWnd = std::make_unique<CSiHwang>(m_pParent);
	m_pSiHwangWnd->Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), this, IDC_SIHWANG);
	m_pSiHwangWnd->RequestPIBO7710();

	m_pTujajaWnd = std::make_unique<CTujaja>(m_pParent);
	m_pTujajaWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, IDC_TUJAJA);

	return 0;
}

void CMapWnd::SafeDelete( CWnd *pWnd )
{
	if (pWnd)
	{
		pWnd->DestroyWindow();
		delete pWnd;
	}
}

void CMapWnd::OnTabChange( NMHDR *pHeader, LRESULT *pResult )
{
	if (pHeader->idFrom==IDC_TAB && m_pTabWnd)
	{
		const int idx = m_pTabWnd->GetCurSel();
		switch(idx)
		{
		case 0:
			if (m_pTujajaWnd->GetSafeHwnd())  m_pTujajaWnd->ShowWindow(SW_SHOW);
			if (m_pSiHwangWnd->GetSafeHwnd()) m_pSiHwangWnd->ShowWindow(SW_HIDE);
			break;
		case 1:
			if (m_pTujajaWnd->GetSafeHwnd())  m_pTujajaWnd->ShowWindow(SW_HIDE);
			if (m_pSiHwangWnd->GetSafeHwnd()) m_pSiHwangWnd->ShowWindow(SW_SHOW);
			break;
		}
	}
}