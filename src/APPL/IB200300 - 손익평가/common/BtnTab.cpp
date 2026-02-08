// BtnTab.cpp : implementation file
//

#include "stdafx.h"
#include "BtnTab.h"

#include "../../../h/memDc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBtnTab

CBtnTab::CBtnTab()
{
	m_bkColor   = RGB(255, 255, 255);
	m_backColor = RGB(255, 255, 255);
	m_frColor   = RGB(0, 0, 0);
	m_selectedbColor = RGB(190, 190, 190);
	m_selectedfColor = RGB(255, 255, 255);
	m_pFont      = NULL;
	m_bitmapT_nm = NULL;
	m_bitmapT_dn = NULL;
	m_bitmapT_hv = NULL;

	m_bHover    = FALSE;
	m_bTracking = FALSE;
	m_nHover = -1;
}

CBtnTab::~CBtnTab()
{
}


BEGIN_MESSAGE_MAP(CBtnTab, CTabCtrl)
	//{{AFX_MSG_MAP(CBtnTab)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBtnTab message handlers
void CBtnTab::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMemDC MemDC(&dc);
	OnDraw(&MemDC);	
}

void CBtnTab::OnDraw(CDC *pDC)
{
	TC_ITEM tci;
	char	szTabText[128];
	CRect	rt, tRc1, tBRc;
	CFont	font, *pOldFont;
	bool	bCheck = false;
	int	tabHeight = 0;
	const	int marginL = 15, marginR = 2;
 
	GetClientRect(&rt); pDC->FillSolidRect(&rt, m_backColor);

	for (int ii = 0; ii < GetItemCount(); ii++)
	{
		tci.mask        = TCIF_TEXT;
		tci.pszText     = szTabText;
		tci.cchTextMax  = sizeof(szTabText) - 1;
		GetItem(ii, &tci);
		
		bCheck = GetCurSel() == ii;

		// set font
		LOGFONT lf;
		if (m_pFont)
		{
			m_pFont->GetLogFont(&lf);
		}
		else
		{ 
			if (GetParent()->GetFont())
				GetParent()->GetFont()->GetLogFont(&lf);
			else
			{
				lf = GetLogFont(9, NULL);
				lf.lfHeight = -12;
			}
		}

		lf.lfWeight = FW_NORMAL;//(bCheck ? FW_SEMIBOLD : FW_NORMAL);
		font.CreateFontIndirect(&lf);//, &dc);
		pOldFont = (CFont*)pDC->SelectObject(&font);
		
		GetItemRect(ii, &rt);
		tRc1 = rt; tRc1.DeflateRect(0, 0, -marginR, 0);

		pDC->SetBkMode(TRANSPARENT);

		if (bCheck)
		{
			if (m_bitmapT_dn)
			{
				CDC memDC;
				CBitmap *pBitmap = CBitmap::FromHandle(m_bitmapT_dn), *pOldBitmap;
				BITMAP stBitmapInfo ;
				pBitmap->GetBitmap(&stBitmapInfo);
				memDC.CreateCompatibleDC(pDC);
				pOldBitmap = (CBitmap*)memDC.SelectObject(pBitmap);
				tRc1.OffsetRect(-1, -1);
				pDC->StretchBlt(tRc1.left, tRc1.top, tRc1.Width(), tRc1.Height(),
						&memDC, marginL, 0, stBitmapInfo.bmWidth - (marginL+marginR), stBitmapInfo.bmHeight, SRCCOPY);
				pDC->BitBlt(tRc1.left, tRc1.top, marginL, tRc1.Height(), &memDC, 0, 0, SRCCOPY);
				pDC->BitBlt(tRc1.right - marginR, tRc1.top, marginR, tRc1.Height(),
						&memDC, stBitmapInfo.bmWidth - marginR, 0, SRCCOPY);
				memDC.SelectObject(pOldBitmap); memDC.DeleteDC();
			}
			pDC->SetTextColor(m_selectedfColor);
		}
		else
		{
			//pDC->DrawFrameControl(tRc1, DFC_BUTTON, DFCS_BUTTONPUSH);
			if (m_bitmapT_nm)
			{
				CPoint pt;
				TCHITTESTINFO tcHitTestInfo;
				GetCursorPos(&pt);
				ScreenToClient(&pt);
				tcHitTestInfo.pt    = pt;
				tcHitTestInfo.flags = TCHT_ONITEM;
				HBITMAP hBitmap = m_bitmapT_nm;
				if (m_bHover && HitTest(&tcHitTestInfo) == ii && m_bitmapT_hv)
					hBitmap = m_bitmapT_hv;

				CDC memDC;
				CBitmap *pBitmap = CBitmap::FromHandle(hBitmap), *pOldBitmap;
				BITMAP stBitmapInfo ;
				pBitmap->GetBitmap(&stBitmapInfo);
				memDC.CreateCompatibleDC(pDC);
				pOldBitmap = (CBitmap*)memDC.SelectObject(pBitmap);
				pDC->StretchBlt(tRc1.left, tRc1.top, tRc1.Width(), tRc1.Height(),
						&memDC, marginL, 0, stBitmapInfo.bmWidth - (marginL+marginR), stBitmapInfo.bmHeight, SRCCOPY);
				pDC->BitBlt(tRc1.left, tRc1.top, marginL, tRc1.Height(), &memDC, 0, 0, SRCCOPY);
				pDC->BitBlt(tRc1.right - marginR, tRc1.top, marginR, tRc1.Height(),
						&memDC, stBitmapInfo.bmWidth - marginR, 0, SRCCOPY);
				memDC.SelectObject(pOldBitmap); memDC.DeleteDC();
			}
			pDC->SetTextColor(m_frColor);
		}

		tRc1 = rt;
		
		pDC->DrawText(tci.pszText, lstrlen(tci.pszText), tRc1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		pDC->SelectObject(pOldFont);
		font.DeleteObject();
	}
}

LOGFONT CBtnTab::GetLogFont(int fsize, DWORD weight)
{
	LOGFONT	lf;
	memset(&lf, 0, sizeof(LOGFONT));      
	lf.lfHeight = fsize * 10; 
	
	CString	sfont; sfont = "±¼¸²Ã¼";
	strcpy(lf.lfFaceName, sfont);   
	
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	
	lf.lfWeight = weight;
	lf.lfItalic = FALSE;
	return lf;
}

void CBtnTab::SetFont(CFont* pFont, bool redraw)
{
	m_pFont = pFont;
	if (redraw) RedrawWindow();
}

void CBtnTab::SetColor(COLORREF fcolor, COLORREF bcolor, COLORREF sfcolor, COLORREF sbcolor, COLORREF backColor)
{
	m_selectedfColor = sfcolor;
	m_selectedbColor = sbcolor;
	m_frColor = fcolor;
	m_bkColor = bcolor;
	m_backColor = backColor;
}

void CBtnTab::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	else if (m_bHover)
	{
		TCHITTESTINFO tcHitTestInfo;
		tcHitTestInfo.pt    = point;
		tcHitTestInfo.flags = TCHT_ONITEM;
		if (HitTest(&tcHitTestInfo) != m_nHover)
		{
			m_nHover = HitTest(&tcHitTestInfo);
			RedrawWindow();
		}
	}
	CTabCtrl::OnMouseMove(nFlags, point);
}

void CBtnTab::OnMouseHover(WPARAM wparam, LPARAM lparam)
{
	m_bHover = TRUE; RedrawWindow();
}

LRESULT CBtnTab::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_bHover=FALSE;

	RedrawWindow(); return 0;
}

BOOL CBtnTab::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
	return CTabCtrl::OnEraseBkgnd(pDC);
}
