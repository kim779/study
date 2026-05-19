// splashWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisMBuilder.h"
#include "splashWnd.h"
#include "resource.h"

// CmemDC
class CmemDC : public CDC
{
public:

	// constructor sets up the memory DC
	CmemDC(CDC* pDC) : CDC()
	{
		ASSERT(pDC != NULL);

		m_pDC = pDC;
		m_oldBitmap = NULL;
		m_bMemDC = !pDC->IsPrinting();
              
		if (m_bMemDC)    // Create a Memory DC
		{
			pDC->GetClipBox(&m_rect);
			CreateCompatibleDC(pDC);
			pDC->LPtoDP(&m_rect);

			m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
			m_oldBitmap = SelectObject(&m_bitmap);

			SetMapMode(pDC->GetMapMode());
			SetWindowExt(pDC->GetWindowExt());
			SetViewportExt(pDC->GetViewportExt());

			pDC->DPtoLP(&m_rect);
			SetWindowOrg(m_rect.left, m_rect.top);
		}
		else        // Make a copy of the relevent parts of the current DC for printing
		{
			m_bPrinting = pDC->m_bPrinting;
			m_hDC       = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}
	}
    
	// Destructor copies the contents of the mem DC to the original DC
	~CmemDC()
	{
		if (m_bMemDC) 
		{    
			m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
						this, m_rect.left, m_rect.top, SRCCOPY);
			SelectObject(m_oldBitmap);
		}
		else
		{
			m_hDC = m_hAttribDC = NULL;
		}
	}

	// Allow usage as a pointer
	CmemDC* operator->() { return this; }

	// Allow usage as a pointer
	operator CmemDC*() { return this; }

private:
	CBitmap  m_bitmap;
	CBitmap* m_oldBitmap;
	CDC*     m_pDC;
	CRect    m_rect;
	BOOL     m_bMemDC;
};



// CSplashWnd

IMPLEMENT_DYNAMIC(CSplashWnd, CWnd)

BOOL CSplashWnd::c_showSplashWnd;
CSplashWnd* CSplashWnd::c_splashWnd;
CSplashWnd::CSplashWnd()
{
	m_str = _T("Connecting...");
	m_percentage = 0;
	m_bitmap.LoadBitmap(IDB_UPDATE);
}

CSplashWnd::~CSplashWnd()
{
	ASSERT(c_splashWnd == this);
	c_splashWnd = NULL;
}


BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

// CSplashWnd 메시지 처리기입니다.

int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CenterWindow();
	setSize(m_bitmap.operator HBITMAP());
	return 0;
}


void CSplashWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CmemDC	memdc(&dc);
	CRect	rc, rc_text;
	CFont	font, *pOldFont;
	LOGFONT	lf;

	GetClientRect(&rc);
	setFont(lf, _T(""), 90, false, false);
	font.CreatePointFontIndirect(&lf, &memdc);
	pOldFont = memdc.SelectObject(&font);

	memdc.FillSolidRect(rc, RGB(192, 192, 192));
	memdc.Draw3dRect(rc, RGB(255, 255, 255), RGB(0, 0, 0));
	memdc.SetBkMode(TRANSPARENT);

	if (m_bitmap.operator HBITMAP())
	{
		drawBitmap(&memdc);

		rc.SetRect(40, 54, rc.right - 40, 66);
		memdc.Draw3dRect(rc, RGB(192, 192, 192), RGB(192, 192, 192));
		rc.DeflateRect(1, 1, 1, 1);
		rc_text.SetRect(85, 25, rc.right - 40, 43);

		if (m_percentage)
			rc.right = rc.left + (rc.Width() * m_percentage) / 100;
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
		if (m_percentage)
			rc.right = rc.left + (rc.Width() * m_percentage) / 100;
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


LRESULT CSplashWnd::OnNcHitTest(CPoint point)
{
	LRESULT nHitTest = CWnd::OnNcHitTest(point);
	if (nHitTest == HTCLIENT)
		nHitTest = HTCAPTION;
	return nHitTest;
}


void CSplashWnd::PostNcDestroy()
{
	delete this;
}

void CSplashWnd::EnableSplashScreen(BOOL bEnable)
{
	c_showSplashWnd = bEnable;
}

void CSplashWnd::ShowSplashScreen(CWnd* pParent)
{
	if (!c_showSplashWnd || c_splashWnd != NULL)
		return;

	c_splashWnd = new CSplashWnd;
	if (!c_splashWnd->create(pParent))
		delete c_splashWnd;
	else
		c_splashWnd->UpdateWindow();
}

void CSplashWnd::HideSplashScreen()
{
	if (c_splashWnd)
	{
		c_splashWnd->DestroyWindow();
		c_splashWnd = NULL;
		AfxGetMainWnd()->UpdateWindow();
	}
}

void CSplashWnd::NextScreen(int nPercentage, int nType)
{
	if (c_splashWnd)
	{
		if (nType == 0)
		{
			c_splashWnd->m_str = _T("Initializing...");
			c_splashWnd->m_percentage = nPercentage;
			c_splashWnd->RedrawWindow();
		}
		else
		{
			switch(nType)
			{
			case 1:	// download
				c_splashWnd->m_str = _T("Downloading...");
				break;
			case 2:	// connect0
				c_splashWnd->m_str = _T("Connecting...");
				break;
			case 3:	// Disconnected
				c_splashWnd->m_str = _T("Disconnected.");
				break;
			case 4:	// Backuping
				c_splashWnd->m_str = _T("Backuping...");
				break;
			case 10:
				c_splashWnd->m_str = _T("Sock Initial 1");
				break;
			case 11:
				c_splashWnd->m_str = _T("Sock Initial complete");
				break;
			case 12:
				c_splashWnd->m_str = _T("Sock Open complete");
				break;
			case 13:
				c_splashWnd->m_str = _T("Initialized");
				break;
			default:

				c_splashWnd->m_str.Format(_T("Debug %s"), (char*)nType);
				break;
			}
			c_splashWnd->m_percentage = nPercentage;
			c_splashWnd->RedrawWindow();
		}
	}
}

bool CSplashWnd::IsCreated()
{
	return c_splashWnd? true : false;
}

BOOL CSplashWnd::create(CWnd* pParent)
{
	return CreateEx(WS_EX_TOPMOST, AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
			NULL, WS_POPUP | WS_VISIBLE, 0, 0, 250, 30, pParent->GetSafeHwnd(), NULL);
}

void CSplashWnd::drawBitmap(CDC *pDC)
{
	CRect	rc;

	GetClientRect(&rc);
	if (m_bitmap.operator HBITMAP())
	{
		CDC	memDC;
		
		memDC.CreateCompatibleDC(pDC);
		CBitmap *pOldBitmap = memDC.SelectObject(&m_bitmap);
		pDC->BitBlt(rc.left, rc.top, rc.right, rc.bottom, &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
	}
}

void CSplashWnd::setSize(HBITMAP hBitmap)
{
	if (hBitmap)
	{
		CSize	sz = bitmapSize(hBitmap);
		SetWindowPos(NULL, 0, 0, sz.cx, sz.cy, SWP_NOZORDER|SWP_NOMOVE);
	}
	else
		SetWindowPos(NULL, 0, 0, 250, 30, SWP_NOZORDER|SWP_NOMOVE);

	CenterWindow();
	Invalidate();
}

CSize CSplashWnd::bitmapSize(HBITMAP hBitmap)
{
	if (!hBitmap)
		return CSize(0, 0);

	BITMAP	BM;

	GetObject(hBitmap, sizeof(BITMAP), &BM);
	return CSize((int)BM.bmWidth, (int)BM.bmHeight);
}

void CSplashWnd::setFont(LOGFONT& lf, CString fName, int size, bool isItalic, bool isBold)
{
	ZeroMemory(&lf, sizeof(LOGFONT));

	lf.lfHeight         = size;
	lf.lfCharSet        = DEFAULT_CHARSET;
	lf.lfOutPrecision   = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
	lf.lfQuality        = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	lf.lfWeight         = isBold ? FW_BOLD : FW_NORMAL;
	lf.lfItalic         = isItalic ? TRUE : FALSE;
	//strcpy_s(lf.lfFaceName, LF_FACENAME, fName);   
}