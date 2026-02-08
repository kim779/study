#include "stdafx.h"
#include "ExDialog.h"
#include "imm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CExDialog, CDialog)

CExDialog::CExDialog(UINT nIDTemplate, CWnd* pParentWnd) 
			:CDialog(nIDTemplate, pParentWnd)
{
	m_bFile = true;
	m_bColor = false;
	m_crBack = GetSysColor(COLOR_3DFACE);
	m_bkBrush.CreateSolidBrush(m_crBack); 
}

CExDialog::~CExDialog()
{
	if (m_bkBrush.GetSafeHandle())
		m_bkBrush.DeleteObject();
	ChangeEnglishMode(this->m_hWnd);
}

void CExDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExDialog)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExDialog, CDialog)
	//{{AFX_MSG_MAP(CExDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CExDialog::ChangeHangulMode(HWND hWnd)
{
	DWORD	dwConversion{}, dwSentence{};

	HIMC hImc = ImmGetContext(hWnd);
	if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
		ImmSetConversionStatus(hImc, dwConversion | IME_CMODE_HANGEUL, dwSentence);
}

void CExDialog::ChangeEnglishMode(HWND hWnd)
{
	DWORD	dwConversion{}, dwSentence{};

	HIMC hImc = ImmGetContext(hWnd);
	if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
	{
		if (dwConversion & IME_CMODE_HANGEUL)
			dwConversion -= IME_CMODE_HANGEUL;
		ImmSetConversionStatus(hImc, IME_CMODE_ALPHANUMERIC, IME_SMODE_NONE);
		ImmReleaseContext(hWnd, hImc);
	}
}

void CExDialog::SetBackground(COLORREF rgb)
{
	m_bColor = true;
	m_crBack = rgb;
	
	if(m_bkBrush.GetSafeHandle())
		m_bkBrush.DeleteObject();

	m_bkBrush.CreateSolidBrush(m_crBack); 
}

BOOL CExDialog::SetBackground(CString Path)
{
	if (!m_Background.Load(Path))
	{
		m_bFile = true;
		return FALSE;
	}
	m_bFile = false;
	return TRUE;
}

BOOL CExDialog::SetBackground(HBITMAP& hBitmap)
{
	if (!m_Background.Load(hBitmap))
	{
		m_bFile = true;
		return FALSE;
	}
	m_bFile = false;
	return TRUE;
}

void CExDialog::SetWinPos(CRect wRect)
{
	CPoint pos = CPoint(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	pos.x -= wRect.Width();
	pos.y -= wRect.Height();
	pos.x /= 2;
	pos.y /= 2;
	SetWinPos(pos, wRect);
}

void CExDialog::SetWinPos(CPoint pos, CRect wRect)
{
	SetWinPos(pos.x, pos.y, wRect);
}

void CExDialog::SetWinPos(int wx, int wy, CRect wRect)
{
	SetWindowPos(NULL, wx, wy, wRect.Width(), wRect.Height(), SWP_SHOWWINDOW);
}

void CExDialog::GetBmpSize(int& width, int& height)
{
	m_Background.GetSize(width, height);
}

void CExDialog::ReSizeWnd(UINT *idcX, int cnt, double hRatio, double vRatio)
{
	CWnd* item{};
	CRect	wRc;
	for (int ii = 0; ii < cnt; ii++)
	{
		item = GetDlgItem(idcX[ii]);
		item->GetWindowRect(&wRc);
		this->ScreenToClient(&wRc);

		wRc.left   = (int)(wRc.left   * hRatio);
		wRc.right  = (int)(wRc.right  * hRatio);
		wRc.top    = (int)(wRc.top    * vRatio);
		wRc.bottom = (int)(wRc.bottom * vRatio);

		item->SetWindowPos(NULL, wRc.left, wRc.top, wRc.Width(), wRc.Height(), SWP_NOZORDER);
	}
}

BOOL CExDialog::OnEraseBkgnd(CDC* pDC) 
{
	if (m_bFile)
		return CDialog::OnEraseBkgnd(pDC);

	if (m_Background.GetPixelPtr() != 0) 
	{
		CRect rc;
		GetWindowRect(rc);
		m_Background.DrawDIB(pDC, 0, 0, rc.Width(), rc.Height());
	}

	return TRUE;
}

HBRUSH CExDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if (m_bColor)
	{
		if (nCtlColor == CTLCOLOR_DLG) 
		return (HBRUSH) m_bkBrush.GetSafeHandle(); 
	}

	if (m_bFile)
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (m_Background.GetPixelPtr() != 0) 
	{
		switch(nCtlColor)
		{
			case CTLCOLOR_STATIC:
				TCHAR lpszClassName[255];
				GetClassName(pWnd->m_hWnd, lpszClassName, 255);
				if (_tcscmp(lpszClassName, TRACKBAR_CLASS) == 0)
					return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

			case CTLCOLOR_BTN:
				pDC->SetBkMode(TRANSPARENT);
				return HBRUSH(m_HollowBrush);
			default:
				break;
		}
	}
	
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CExDialog::OnQueryNewPalette() 
{
	if (m_bFile)
		return FALSE;

	CPalette * pPal = m_Background.GetPalette();
	if (pPal != 0 && GetSafeHwnd() != 0)
	{
		CClientDC dc(this);
		CPalette* pOldPalette = dc.SelectPalette(pPal, FALSE);
		const UINT nChanged = dc.RealizePalette();
		dc.SelectPalette(pOldPalette, TRUE);

		if (nChanged == 0)
			return FALSE;

		Invalidate();
		return TRUE;
	}
	return CDialog::OnQueryNewPalette();
	
}

void CExDialog::OnPaletteChanged(CWnd* pFocusWnd) 
{
	if (m_bFile)
		return;

	CPalette * pPal = m_Background.GetPalette();
	if (pPal != 0 && GetSafeHwnd() != 0 && pFocusWnd != this && ! IsChild(pFocusWnd))
	{
		CClientDC dc(this);
		CPalette* pOldPalette = dc.SelectPalette(pPal, TRUE);
		const UINT nChanged = dc.RealizePalette();
		dc.SelectPalette(pOldPalette, TRUE);

		if (nChanged)
			Invalidate();
	}
	else
		CDialog::OnPaletteChanged(pFocusWnd);
}

