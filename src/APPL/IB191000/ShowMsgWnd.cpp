// ShowMsgWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ShowMsgWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FBSM_TIMERID		100
#define FBSM_CLASSNAME		"FBShowMsg"

/////////////////////////////////////////////////////////////////////////////
// CShowMsgWnd

LOGFONT	CShowMsgWnd::m_lfFont;
int		CShowMsgWnd::m_nInstanceCnt = 0;

CShowMsgWnd::CShowMsgWnd()
{
	m_nInstanceCnt++;

	m_pParentWnd = NULL;
	m_strMessage = "FB Massage Window";

	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();
	if(!(::GetClassInfo(hInst, FBSM_CLASSNAME, &wndcls)))
	{
		// otherwise we need to register a new class
		wndcls.style			= CS_SAVEBITS;
		wndcls.lpfnWndProc		= ::DefWindowProc;
		wndcls.cbClsExtra		= wndcls.cbWndExtra = 0;
		wndcls.hInstance		= hInst;
		wndcls.hIcon			= NULL;
		wndcls.hCursor			= LoadCursor( hInst, IDC_ARROW );
		wndcls.hbrBackground	= (HBRUSH)(COLOR_INFOBK + 1); 
		wndcls.lpszMenuName		= NULL;
		wndcls.lpszClassName	= FBSM_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
			AfxThrowResourceException();
	}

	memset(&m_lfFont, 0, sizeof(LOGFONT));
	m_lfFont.lfCharSet = HANGEUL_CHARSET;
	m_lfFont.lfHeight  = 90;
	m_lfFont.lfWeight  = FW_NORMAL;
	strcpy(m_lfFont.lfFaceName, "굴림체");

	/*
	CColorMap ColorMap;
	m_crBkgrnd = ColorMap.GetColor(52);	// 그룹박스 배경색
	*/
	m_crBkgrnd = RGB(239, 243, 255);
	
	m_bProgress = FALSE;
	m_nOffset	= 0;

	m_nMax		= 100;
	m_nPos		= 0;
}

CShowMsgWnd::~CShowMsgWnd()
{
	m_nInstanceCnt--;
}


BEGIN_MESSAGE_MAP(CShowMsgWnd, CWnd)
	//{{AFX_MSG_MAP(CShowMsgWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CShowMsgWnd message handlers

BOOL CShowMsgWnd::Create(CWnd* pParentWnd)
{
	ASSERT_VALID(pParentWnd);

//	DWORD dwStyle = WS_POPUP; 
//	DWORD dwExStyle = WS_EX_TOOLWINDOW |WS_EX_TOPMOST|WS_EX_STATICEDGE;
	DWORD dwStyle = WS_CHILD|WS_VISIBLE; 
	DWORD dwExStyle = WS_EX_STATICEDGE;
	m_pParentWnd = pParentWnd;


	CRect rcParent, rcThis;
	pParentWnd->GetWindowRect(&rcParent);

//	return CreateEx(dwExStyle, FBSM_CLASSNAME, NULL, dwStyle, 
//					CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
//					NULL, NULL, NULL );
	return CWnd::CreateEx(dwExStyle, NULL, NULL, dwStyle, CRect(0,0,0,0), pParentWnd, 0);
}

void CShowMsgWnd::PostNcDestroy() 
{
	if (FALSE == m_bProgress)
		delete this;
}


void CShowMsgWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CDC MemDC;
	if (!MemDC.CreateCompatibleDC(&dc))
		return;

	CRect rect;
	GetClientRect(&rect);

	CBitmap NewBitmap, *pOldBitmap = NULL;

	if (!NewBitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height()))
		return;

	pOldBitmap = MemDC.SelectObject(&NewBitmap);
	
	CFont font;
	font.CreatePointFontIndirect(&m_lfFont);
	CFont* pOldFont = (CFont*)MemDC.SelectObject(&font);

	MemDC.FillSolidRect(&rect, m_crBkgrnd);

	CRect rcWnd;

	MemDC.SetTextColor(RGB(102, 102, 102));

	if (m_bProgress)
	{
		rcWnd = rect;
		rcWnd.left   += 10;
		rcWnd.top    += 20;
		rcWnd.right  -= 10;
		rcWnd.bottom -= 5;

		m_nOffset = rcWnd.Width();

		MemDC.FillSolidRect(rcWnd, RGB(255, 255, 255));

		rcWnd.right = rcWnd.left + (m_nOffset * m_nPos / 100);
		MemDC.FillSolidRect(rcWnd, RGB(182, 182, 182));

		rcWnd = rect;
		rcWnd.left   += 10;
		rcWnd.top    += 3;
		rcWnd.right  -= 10;
		rcWnd.bottom -= 22;

		MemDC.SetBkMode(TRANSPARENT);
		MemDC.DrawText(m_strMessage, &rcWnd, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		MemDC.SetBkMode(TRANSPARENT);
		MemDC.DrawText(m_strMessage, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(pOldFont);
	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
	
	if (font.GetSafeHandle())
		font.DeleteObject();
}

CSize CShowMsgWnd::GetTextExtent(CWnd* pWnd, LPCTSTR cszText)
{
	CSize size;
	CClientDC dc(pWnd);

	CFont font;
	font.CreatePointFontIndirect(&m_lfFont);

	CFont* pOldFont = (CFont*)dc.SelectObject(&font);
	size = dc.GetTextExtent(cszText);
	dc.SelectObject(pOldFont);

	if (font.GetSafeHandle())
		font.DeleteObject();

	return size;
}

void CShowMsgWnd::SetPos(int nPos, BOOL bRedraw /* = TRUE */)
{
	m_nPos = nPos;

	if (!bRedraw)
		return;

	CRect rcWnd;
	GetClientRect(&rcWnd);
	
	rcWnd.left   += 10;
	rcWnd.top    += 20;
	rcWnd.right  -= 10;
	rcWnd.bottom -= 5;

	InvalidateRect(rcWnd);
}

void CShowMsgWnd::AddPos(int nPos, BOOL bRedraw /* = TRUE */)
{
	m_nPos = nPos;

	if (!bRedraw)
		return;

	CRect rcWnd;
	GetClientRect(&rcWnd);
	
	rcWnd.left   += 10;
	rcWnd.top    += 20;
	rcWnd.right  -= 10;
	rcWnd.bottom -= 5;

	InvalidateRect(rcWnd);
}

CWnd* CShowMsgWnd::ShowMessage(CWnd* pParentWnd, LPCTSTR cszMessage, UINT nShowTime /* = 1000 */, BOOL bProgress /* = FALSE */)
{
	CShowMsgWnd* pShowMsgWnd;
	pShowMsgWnd = new CShowMsgWnd;

	CSize size = GetTextExtent(pParentWnd, cszMessage);

	CRect rcParent, rcThis;
	pParentWnd->GetClientRect(&rcParent);

	rcThis.left		= (rcParent.right + rcParent.left)/2 - size.cx/2;
	rcThis.right	= rcThis.left + size.cx;
	rcThis.top		= (rcParent.top + rcParent.bottom)/2 - size.cy/2;
	rcThis.bottom   = rcThis.top + size.cy;	

	rcThis.InflateRect(5, 5);

	rcThis.OffsetRect(0, rcThis.Height()*(m_nInstanceCnt-1));
	
	if (bProgress)
	{
		rcThis.left   -= 10;
		rcThis.top    -= 10;
		rcThis.right  += 10;
		rcThis.bottom += 10;
	}

	pShowMsgWnd->m_strMessage = cszMessage;
	pShowMsgWnd->Create(pParentWnd);
	
	if (0 != nShowTime)
	{
		pShowMsgWnd->SetWindowPos(NULL, rcThis.left, rcThis.top, rcThis.Width(), rcThis.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
		pShowMsgWnd->CenterWindow();
		pShowMsgWnd->ShowWindow(SW_SHOW);
		pShowMsgWnd->SetTimer(FBSM_TIMERID, nShowTime, NULL);
	}
	else
	{
		pShowMsgWnd->SetWindowPos(&CWnd::wndTop, rcThis.left, rcThis.top, rcThis.Width(), rcThis.Height(), SWP_NOACTIVATE | SWP_SHOWWINDOW);
	}

	pShowMsgWnd->m_bProgress = bProgress;

	return pShowMsgWnd;
}

CWnd* CShowMsgWnd::ShowMessage(CWnd* pParentWnd, CWnd *pRectWnd, int nWidth, int nHeight, LPCTSTR cszMessage, UINT nShowTime /* = 1000 */, BOOL bProgress /* = FALSE */)
{
	CShowMsgWnd* pShowMsgWnd;
	pShowMsgWnd = new CShowMsgWnd;

	CSize size = GetTextExtent(pParentWnd, cszMessage);

	CRect rcParent, rcThis;
	pRectWnd->GetClientRect(&rcParent);

	if (0 == rcParent.Width())
	{
		rcParent.right  = nWidth;
		rcParent.bottom = nHeight;
	}

	rcThis.left		= (rcParent.right + rcParent.left)/2 - size.cx/2;
	rcThis.right	= rcThis.left + size.cx;
	rcThis.top		= (rcParent.top + rcParent.bottom)/2 - size.cy/2;
	rcThis.bottom   = rcThis.top + size.cy;	

	rcThis.InflateRect(5, 5);

	rcThis.OffsetRect(0, rcThis.Height()*(m_nInstanceCnt-1));
	
	if (bProgress)
	{
		rcThis.left   -= 10;
		rcThis.top    -= 10;
		rcThis.right  += 10;
		rcThis.bottom += 10;
	}

	pShowMsgWnd->m_strMessage = cszMessage;
	pShowMsgWnd->Create(pParentWnd);
	
	if (0 != nShowTime)
	{
		pShowMsgWnd->SetWindowPos(NULL, rcThis.left, rcThis.top, rcThis.Width(), rcThis.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
		pShowMsgWnd->CenterWindow();
		pShowMsgWnd->ShowWindow(SW_SHOW);
		pShowMsgWnd->SetTimer(FBSM_TIMERID, nShowTime, NULL);
	}
	else
	{
		pShowMsgWnd->SetWindowPos(&CWnd::wndTop, rcThis.left, rcThis.top, rcThis.Width(), rcThis.Height(), SWP_NOACTIVATE | SWP_SHOWWINDOW);
	}

	pShowMsgWnd->m_bProgress = bProgress;

	return pShowMsgWnd;
}

void CShowMsgWnd::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == FBSM_TIMERID)
	{
		KillTimer(FBSM_TIMERID);
		DestroyWindow();
	}	
}
