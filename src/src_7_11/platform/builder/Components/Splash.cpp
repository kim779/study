// CG: This file was added by the Splash Screen component.
// Splash.cpp : implementation file
//

#include "stdafx.h"  
#include "Splash.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//   Splash Screen class

BOOL CSplashWnd::c_bShowSplashWnd;
CSplashWnd* CSplashWnd::c_pSplashWnd;
CSplashWnd::CSplashWnd()
{
	m_str         = "Connecting...";
	cnt           = 0;
	m_nPercentage = 0;
	m_Bitmap.LoadBitmap(IDB_UPDATEBMP);
}

CSplashWnd::~CSplashWnd()
{
	// Clear the static window pointer.
	ASSERT(c_pSplashWnd == this);
	c_pSplashWnd = NULL;
}

BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	//{{AFX_MSG_MAP(CSplashWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSplashWnd::EnableSplashScreen(BOOL bEnable /*= TRUE*/)
{
	c_bShowSplashWnd = bEnable;
}

void CSplashWnd::ShowSplashScreen(CWnd* pParentWnd /*= NULL*/)
{
	if (!c_bShowSplashWnd || c_pSplashWnd != NULL)
		return;

	// Allocate a new splash screen, and create the window.
	c_pSplashWnd = new CSplashWnd;
	if (!c_pSplashWnd->Create(pParentWnd))
		delete c_pSplashWnd;
	else
		c_pSplashWnd->UpdateWindow();
}

BOOL CSplashWnd::Create(CWnd* pParentWnd /*= NULL*/)
{
	return CreateEx(WS_EX_TOPMOST,
		AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
		NULL, WS_POPUP | WS_VISIBLE, 0, 0, 250/*bm.bmWidth*/, 30/*bm.bmHeight*/, pParentWnd->GetSafeHwnd(), NULL);
}

void CSplashWnd::HideSplashScreen()
{
	// Destroy the window, and update the mainframe.
	if (c_pSplashWnd)
	{
		c_pSplashWnd->DestroyWindow();
		c_pSplashWnd = NULL;
		AfxGetMainWnd()->UpdateWindow();
	}
}

void CSplashWnd::NextScreen(int nPercentage, int nType)
{
	if (c_pSplashWnd)
	{
		if (nType == 0)
		{
			c_pSplashWnd->m_str = "Initializing...";
			c_pSplashWnd->m_nPercentage = nPercentage;
			c_pSplashWnd->RedrawWindow();
		}
		else
		{
			switch(nType)
			{
			case 1:	// download
				c_pSplashWnd->m_str = "Downloading...";
				break;
			case 2:	// connect0
				c_pSplashWnd->m_str = "Connecting...";
				break;
			case 3:	// Disconnected
				c_pSplashWnd->m_str = "Disconnected.";
				break;
			case 4:	// Backuping
				c_pSplashWnd->m_str = "Backuping...";
				break;
			case 10:
				c_pSplashWnd->m_str = "Sock Initial 1";	break;
			case 11:
				c_pSplashWnd->m_str = "Sock Initial complete";	break;
			case 12:
				c_pSplashWnd->m_str = "Sock Open complete";	break;
			case 13:
				c_pSplashWnd->m_str = "Initialized";	break;
			default:

				//c_pSplashWnd->m_str = "No Information.";
				c_pSplashWnd->m_str.Format("Debug %s", (char*)nType);
				break;
			}
			c_pSplashWnd->m_nPercentage = nPercentage;
			c_pSplashWnd->RedrawWindow();
		}
	}
}

bool CSplashWnd::IsCreated()
{
	return c_pSplashWnd?true:false;
}

void CSplashWnd::PostNcDestroy()
{
	// Free the C++ class.
	delete this;
}

int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Center the window.
	CenterWindow();

	setSize(m_Bitmap.operator HBITMAP());
	return 0;
}

void CSplashWnd::OnPaint()
{
	CPaintDC dc(this);
	CAxDC memdc(&dc);
	CRect rc, rc_text;
	CFont	font, *pOldFont;
	GetClientRect(&rc);

	LOGFONT lf = setFont("", 90, false, false);
	font.CreatePointFontIndirect(&lf, &memdc);
	pOldFont = memdc.SelectObject(&font);

	memdc.FillSolidRect(rc, RGB(192, 192, 192));
	memdc.Draw3dRect(rc, RGB(255, 255, 255), RGB(0, 0, 0));

	memdc.SetBkMode(TRANSPARENT);

	if (m_Bitmap.operator HBITMAP())
	{
		drawBitmap(&memdc);

		rc.SetRect(40, 54, rc.right - 40, 66);
		memdc.Draw3dRect(rc, RGB(192, 192, 192), RGB(192, 192, 192));
		rc.DeflateRect(1, 1, 1, 1);
		rc_text.SetRect(85, 25, rc.right - 40, 43);

		if (m_nPercentage)
			rc.right = rc.left + (rc.Width() * m_nPercentage) / 100;
		else
			rc.right = rc.left;
		memdc.FillSolidRect(rc, GetSysColor(COLOR_HIGHLIGHT));

		memdc.SetTextColor(RGB(0, 0, 0));
		memdc.DrawText(m_str, rc_text, DT_VCENTER|DT_SINGLELINE|DT_LEFT);
	}
	else
	{
		rc.DeflateRect(5, 5, 5, 5);
		memdc.Draw3dRect(rc, RGB(0, 0, 0), RGB(255, 255, 255));
		rc.DeflateRect(1, 1, 1, 1);
		rc_text = rc;
		if (m_nPercentage)
			rc.right = rc.left + (rc.Width() * m_nPercentage) / 100;
		else
			rc.right = rc.left;
		memdc.FillSolidRect(rc, GetSysColor(COLOR_HIGHLIGHT));
		memdc.SetTextColor(RGB(0, 0, 0));
		memdc.DrawText(m_str, rc_text, DT_VCENTER|DT_SINGLELINE|DT_CENTER);
		memdc.IntersectClipRect(rc);
		memdc.SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
		memdc.DrawText(m_str, rc_text, DT_VCENTER|DT_SINGLELINE|DT_CENTER);
	}
	memdc.SelectObject(pOldFont);
	font.DeleteObject();
}

LOGFONT CSplashWnd::setFont(CString fName, int size, bool isItalic, bool isBold)
{
	LOGFONT	lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = size;
	
	//strcpy(lf.lfFaceName, fName);   
	
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;

	lf.lfWeight = (isBold? FW_BOLD : FW_NORMAL);
	lf.lfItalic = (isItalic? TRUE : FALSE);
	return lf;
}

CSize CSplashWnd::BitmapSize(HBITMAP hBitmap)
{
	if (!hBitmap)
		return CSize(0, 0);
	BITMAP BM;
	GetObject(hBitmap, sizeof(BITMAP), &BM);
	return CSize((int)BM.bmWidth, (int)BM.bmHeight);
}

void CSplashWnd::setSize(HBITMAP hBitmap)
{
	if (hBitmap)
	{
		CSize sz = BitmapSize(hBitmap);
		SetWindowPos(NULL, 0, 0, sz.cx, sz.cy, SWP_NOZORDER|SWP_NOMOVE);
	}
	else
		SetWindowPos(NULL, 0, 0, 250, 30, SWP_NOZORDER|SWP_NOMOVE);
	CenterWindow();
	Invalidate();
}

void CSplashWnd::drawBitmap(CDC *pDC)
{
	CRect rc;
	GetClientRect(&rc);

	if (m_Bitmap.operator HBITMAP())
	{
		CDC		memDC;
		
		memDC.CreateCompatibleDC(pDC);
		CBitmap *pOldBitmap = memDC.SelectObject(&m_Bitmap);
		
		pDC->BitBlt(rc.left, rc.top, rc.right, rc.bottom, &memDC, 0, 0, SRCCOPY);
		
		memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
	}
}

LRESULT CSplashWnd::OnNcHitTest(CPoint point) 
{
	UINT nHitTest = CWnd::OnNcHitTest(point);
	if (nHitTest == HTCLIENT)
		nHitTest = HTCAPTION;

	return nHitTest;
}
