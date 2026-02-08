/************************************************************************/
/* FILENAME:    AxMTabCtrl.cpp                                          */
/* DESCRIPTION: CAxMTabCtrl class implementation file.                  */
/* WRITTEN:     macho@2006.11.06                                        */
/*                                                                      */
/************************************************************************/

#include "stdafx.h"
#include "AxMCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseTabCtrl

CBaseTabCtrl::CBaseTabCtrl(int nType)
{
	m_crBack = (COLORREF)-1; // use default color
	m_nDrawType = BTC_NONE;

	EnableDraw(nType);
}

CBaseTabCtrl::~CBaseTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CBaseTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CBaseTabCtrl)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseTabCtrl message handlers

void CBaseTabCtrl::SetBkgndColor(COLORREF color)
{
	// set new color
	m_crBack = color;

	// redraw
	if (GetSafeHwnd())
		Invalidate();
}

BOOL CBaseTabCtrl::EnableDraw(int nType)
{
	ASSERT (nType >= BTC_NONE && nType <= BTC_ALL);

	if (nType < BTC_NONE || nType > BTC_ALL)
		return FALSE;

	m_nDrawType = nType;

	if (GetSafeHwnd())
	{
		if (m_nDrawType != BTC_NONE) 
			ModifyStyle(0, TCS_OWNERDRAWFIXED);
		else
			ModifyStyle(TCS_OWNERDRAWFIXED, 0);

		Invalidate();
	}

	return TRUE;
}

void CBaseTabCtrl::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
	ASSERT (0);
}

void CBaseTabCtrl::DrawItemBorder(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
	ASSERT (0);
}

void CBaseTabCtrl::DrawMainBorder(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
	ASSERT (0);
}

COLORREF CBaseTabCtrl::GetTabColor(BOOL /*bSelected*/)
{
	return ::GetSysColor(COLOR_3DFACE);
}

COLORREF CBaseTabCtrl::GetTabTextColor(BOOL /*bSelected*/)
{
	return ::GetSysColor(COLOR_WINDOWTEXT);
}

void CBaseTabCtrl::PreSubclassWindow() 
{
	CTabCtrl::PreSubclassWindow();

	if (m_nDrawType != BTC_NONE) 
		ModifyStyle(0, TCS_OWNERDRAWFIXED);
}

BOOL CBaseTabCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
	CRect rClient, rTab, rTotalTab, rBkgnd, rEdge;
	COLORREF crBack;
	int nTab, nTabHeight = 0;

	CTabCtrl::OnEraseBkgnd(pDC);

	// calc total tab width
	GetClientRect(rClient);
	nTab = GetItemCount();
	rTotalTab.SetRectEmpty();

	while (nTab--)
	{
		GetItemRect(nTab, rTab);
		rTotalTab.UnionRect(rTab, rTotalTab);
	}

	nTabHeight = rTotalTab.Height();

	// add a bit
	rTotalTab.InflateRect(2, 3);
	rEdge = rTotalTab;

	// then if background color is set, paint the visible background
	// area of the tabs in the bkgnd color
	// note: the mfc code for drawing the tabs makes all sorts of assumptions
	// about the background color of the tab control being the same as the page
	// color - in some places the background color shows thru' the pages!!
	// so we must only paint the background color where we need to, which is that
	// portion of the tab area not excluded by the tabs themselves
	crBack = (m_crBack == -1) ? ::GetSysColor(COLOR_3DFACE) : m_crBack;
	
	// full width of tab ctrl above top of tabs
	rBkgnd = rClient;
	rBkgnd.bottom = rTotalTab.top + 3;
	pDC->SetBkColor(crBack);
	pDC->ExtTextOut(rBkgnd.left, rBkgnd.top, ETO_CLIPPED | ETO_OPAQUE, rBkgnd, "", NULL);
	
	// width of tab ctrl visible bkgnd including bottom pixel of tabs to left of tabs
	rBkgnd = rClient;
	rBkgnd.right = 2;
	rBkgnd.bottom = rBkgnd.top + (nTabHeight + 2);
	pDC->ExtTextOut(rBkgnd.left, rBkgnd.top, ETO_CLIPPED | ETO_OPAQUE, rBkgnd, "", NULL);
	
	// to right of tabs
	rBkgnd = rClient;
	rBkgnd.left += rTotalTab.Width() - 2;
	rBkgnd.bottom = rBkgnd.top + (nTabHeight + 2);
	pDC->ExtTextOut(rBkgnd.left, rBkgnd.top, ETO_CLIPPED | ETO_OPAQUE, rBkgnd, "", NULL);

	return TRUE;
}

void CBaseTabCtrl::OnPaint() 
{
	if (m_nDrawType == BTC_NONE)
	{
		Default();
	}
	else if (m_nDrawType == BTC_TABS)
	{
		ASSERT (GetStyle() & TCS_OWNERDRAWFIXED);
		Default();
	}
	else // all
	{
		CPaintDC dc(this); // device context for painting

		// prepare dc
		dc.SelectObject(GetFont());

		DRAWITEMSTRUCT dis;
		dis.CtlType = ODT_TAB;
		dis.CtlID = GetDlgCtrlID();
		dis.hwndItem = GetSafeHwnd();
		dis.hDC = dc.GetSafeHdc();
		dis.itemAction = ODA_DRAWENTIRE;

		// draw the rest of the border
		CRect rClient, rPage;
		GetClientRect(&dis.rcItem);
		rPage = dis.rcItem;
		AdjustRect(FALSE, rPage);
//		dis.rcItem.top = rPage.top - 2;

//		DrawMainBorder(&dis);

		// paint the tabs first and then the borders
		int nTab = GetItemCount();
		int nSel = GetCurSel();

		if (!nTab) // no pages added
			return;

		while (nTab--)
		{
			if (nTab != nSel)
			{
				dis.itemID = nTab;
				dis.itemState = 0;

				VERIFY(GetItemRect(nTab, &dis.rcItem));

				//dis.rcItem.bottom -= 2;
				DrawItem(&dis);
				//DrawItemBorder(&dis);
			}
		}

		// now selected tab
		dis.itemID = nSel;
		dis.itemState = ODS_SELECTED;

		VERIFY(GetItemRect(nSel, &dis.rcItem));

//		dis.rcItem.bottom += 2;
//		dis.rcItem.top -= 2;
		DrawItem(&dis);
//		DrawItemBorder(&dis);
	}
}


DWORD CAxMTabCtrl::s_dwCustomLook = 0;

const int PADDING = 0;//3;
const int EDGE    = 0;//20;

//////////////////////////////////////////////////////////////////////////////
// helpers
COLORREF Darker(COLORREF crBase, float fFactor)
{
	ASSERT (fFactor < 1.0f && fFactor > 0.0f);

	fFactor = min(fFactor, 1.0f);
	fFactor = max(fFactor, 0.0f);

	BYTE bRed, bBlue, bGreen;
	BYTE bRedShadow, bBlueShadow, bGreenShadow;

	bRed = GetRValue(crBase);
	bBlue = GetBValue(crBase);
	bGreen = GetGValue(crBase);

	bRedShadow = (BYTE)(bRed * fFactor);
	bBlueShadow = (BYTE)(bBlue * fFactor);
	bGreenShadow = (BYTE)(bGreen * fFactor);

	return RGB(bRedShadow, bGreenShadow, bBlueShadow);
}

COLORREF Lighter(COLORREF crBase, float fFactor) 
{
	ASSERT (fFactor > 1.0f);

	fFactor = max(fFactor, 1.0f);

	BYTE bRed, bBlue, bGreen;
	BYTE bRedHilite, bBlueHilite, bGreenHilite;

	bRed = GetRValue(crBase);
	bBlue = GetBValue(crBase);
	bGreen = GetGValue(crBase);

	bRedHilite = (BYTE)min((int)(bRed * fFactor), 255);
	bBlueHilite = (BYTE)min((int)(bBlue * fFactor), 255);
	bGreenHilite = (BYTE)min((int)(bGreen * fFactor), 255);

	return RGB(bRedHilite, bGreenHilite, bBlueHilite);
}

CSize FormatText(CString& sText, CDC* pDC, int nWidth)
{
	CRect rect(0, 0, nWidth, 20);
	UINT uFlags = DT_CALCRECT | DT_SINGLELINE | DT_VCENTER | DT_MODIFYSTRING | DT_END_ELLIPSIS;


	::DrawText(pDC->GetSafeHdc(), sText.GetBuffer(sText.GetLength() + 4), -1, rect, uFlags);
	sText.ReleaseBuffer();

	return pDC->GetTextExtent(sText);
}

////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CAxMTabCtrl, CTabCtrl)
//{{AFX_MSG_MAP(CAxMTabCtrl)
	ON_WM_PAINT()
	ON_MESSAGE(AXM_SETMSGHANDLER, OnSetMessageHandler)
	ON_MESSAGE(AXM_GETMSGHANDLER, OnGetMessageHandler)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CAxMTabCtrl::CAxMTabCtrl(CWnd* parent)
{
	m_parent = parent;
	m_font.CreatePointFont(90, "±¼¸²");
	m_oldTabIndex = -1;
	m_bkColor = clBACK;

	for (int ii = 0; ii < ARRAY_SIZE(m_TabBMP); ii++) 
	{
		m_TabBMP[ii] = NULL;
	}
}

CAxMTabCtrl::~CAxMTabCtrl()
{
	ClearImageObject();
}

void CAxMTabCtrl::SetFont(int pt, LPCTSTR lpszFontName)
{
	if (m_font.m_hObject) 
		m_font.DeleteObject();
	m_font.CreatePointFont(pt * 10, lpszFontName);
}


/////////////////////////////////////////////////////////////////////////////
// CAxMTabCtrl message handlers

int CAxMTabCtrl::getBmpWidth(HBITMAP hBitmap)
{
	BITMAP bm;
	GetObject(hBitmap, sizeof(bm), &bm);
	return bm.bmWidth;
}

int CAxMTabCtrl::drawBmp(CDC *dc, CDC *memDC, CRect rc, HBITMAP hBitmap, int posX, int posTo)
{
	HBITMAP pbmp = NULL;
	BITMAP	bm;

	pbmp = (HBITMAP)memDC->SelectObject(hBitmap);

	GetObject(hBitmap, sizeof(bm), &bm);

	::TransparentBlt(dc->m_hDC, posX, 0, rc.Width(), rc.Height(), memDC->m_hDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255, 0, 255));
	memDC->SelectObject(pbmp);

	return posX;
}

void CAxMTabCtrl::EnableCustomLook(BOOL bEnable, DWORD dwStyle)
{
	if (!bEnable)
		dwStyle = 0;

	s_dwCustomLook = dwStyle;
}

void CAxMTabCtrl::PreSubclassWindow() 
{
	CTabCtrl::PreSubclassWindow();

	if (s_dwCustomLook)
		ModifyStyle(0, TCS_OWNERDRAWFIXED);
}

int CAxMTabCtrl::GetItemFromPoint(CPoint pt, CRect& rc)
{
	for (int i = 0; i < GetItemCount(); i++) 
	{
		if (GetItemRect(i, rc)) 
		{
			if (rc.PtInRect(pt))
				return i;
		}
	}
	return -1;
}


LRESULT CAxMTabCtrl::Notify(UINT msg, WPARAM wParam, LPARAM lParam)
{
	NOTIFY(msg, wParam, lParam);
}

LRESULT CAxMTabCtrl::OnSetMessageHandler(WPARAM wParam, LPARAM lParam)
{
	SetMessageHandler((CWnd*)wParam);
	return 0L;
}

LRESULT CAxMTabCtrl::OnGetMessageHandler(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)GetMessageHandler();
}


void CAxMTabCtrl::SetDrawMode(int mode)
{
	if (mode == DM_IMAGE) {
		if (!IsImageReady())
			return;
	}
	m_drawMode = mode;
	Invalidate();
}

void CAxMTabCtrl::ClearImageObject()
{
	for (int ii = 0; ii < 2; ii++)
	{
		if (m_TabBMP[ii])
		{
			::DeleteObject(m_TabBMP[ii]);
			m_TabBMP[ii] = NULL;
		}
	}
}

void CAxMTabCtrl::SetTabImage(LPCTSTR sel, LPCTSTR back)
{
	if (IsImageReady())
		return;

	ClearImageObject();

	m_TabBMP[0] = (HBITMAP)LoadImage(NULL, sel, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_TabBMP[1] = (HBITMAP)LoadImage(NULL, back, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

BOOL CAxMTabCtrl::IsImageReady()
{
	for (int ii = 0; ii < 2; ii++)
	{
		if (m_TabBMP[ii] == NULL)
			return FALSE;
	}
	return TRUE;
}

void CAxMTabCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	drawImage(&dc);
}

void CAxMTabCtrl::drawImage(CDC* pDC)
{
	char        szTabText[100];
	TC_ITEM     tci;
	CRect rt, rtItem;
	CFont	font, *pOldFont;
	bool	bSelect = false;
	int	nCheck;
	CDC	memDC;

	GetClientRect(&rt);
	rt.left -= 1;
	pDC->FillSolidRect(&rt, m_bkColor);

	int	sMode = pDC->SetBkMode(TRANSPARENT);

	LOGFONT lf;
	m_font.GetLogFont(&lf);
	int	color = RGB(70, 82, 102);
	CPen	cPen(PS_SOLID, 1, RGB(149, 159, 171));
	CPen	*sPen = pDC->SelectObject(&cPen);
/*
	pDC->MoveTo(rt.right-1, rt.top);
	pDC->LineTo(rt.right-1, rt.bottom);
	pDC->MoveTo(rt.left, rt.top);
	pDC->LineTo(rt.right, rt.top);
*/
	nCheck = GetCurSel();
	memDC.CreateCompatibleDC(pDC);
	for (int ii = 0; ii < GetItemCount(); ii++)
	{
		tci.mask        = TCIF_TEXT;
		tci.pszText     = szTabText;
		tci.cchTextMax  = sizeof(szTabText) - 1;
		GetItem(ii, &tci);
		
		bSelect = (nCheck == ii)? true:false;

		// Set font
		if (bSelect)
		{
			lf.lfWeight |= FW_BOLD;
			color = clBlack;
		}
		else
		{
			lf.lfWeight &= ~FW_BOLD;
			color = RGB(70, 82, 102);
		}

		lf.lfWeight = (bSelect ? FW_SEMIBOLD : FW_NORMAL);
		font.CreateFontIndirect(&lf);
		pOldFont = (CFont*)pDC->SelectObject(&font);
		
		GetItemRect(ii, &rtItem);
		rtItem.bottom += 3;

		// Draw Image
		if (bSelect)
		{
			drawBmp(pDC, &memDC, rtItem, m_TabBMP[0], rtItem.left);
			pDC->SetTextColor(color);
		} 
		else
		{
			drawBmp(pDC, &memDC, rtItem, m_TabBMP[1], rtItem.left);
			pDC->SetTextColor(color);
		}
		pDC->DrawText(tci.pszText, lstrlen(tci.pszText), rtItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		pDC->SelectObject(pOldFont);
		font.DeleteObject();
	}

	memDC.DeleteDC();
	pDC->SelectObject(sPen);
	pDC->SetBkMode(sMode);
}