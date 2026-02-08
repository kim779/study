// ToolTip.cpp : implementation file
//

#include "stdafx.h"
#include "axisgmain.h"
#include "ToolTip.h"
#include "memDC.h"
#include "ObjEvent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PATTERN_WIDTH	360
#define PATTERN_HEIGHT  145

#define PATTERNIMAGE_WIDTH	70
#define PATTERNIMAGE_HEIGHT	70

#define TITLE_HEIGHT	20

const int MARGIN = 8, GAB = 2;

/////////////////////////////////////////////////////////////////////////////
// CToolTip

CToolTip::CToolTip(CWnd *pView, class CObjEvent *pObjEvent)
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();
	if(!(::GetClassInfo(hInst, MAIN_TOOLTIP_CLASS, &wndcls)))
	{
		wndcls.style		= CS_SAVEBITS;
		wndcls.lpfnWndProc	= ::DefWindowProc;
		wndcls.cbClsExtra	= wndcls.cbWndExtra = 0;
		wndcls.hInstance	= hInst;
		wndcls.hIcon		= NULL;
		wndcls.hCursor		= LoadCursor(hInst, IDC_ARROW);
		wndcls.hbrBackground	= (HBRUSH)(COLOR_INFOBK + 1); 
		wndcls.lpszMenuName	= NULL;
		wndcls.lpszClassName	= MAIN_TOOLTIP_CLASS;

		if (!AfxRegisterClass(&wndcls))	AfxThrowResourceException();
	}

	m_iCellHeight = 0;
	m_iMaxWidthTitle = 0;

	m_pwndView = pView;
	m_pObjEvent = pObjEvent;
	CAxisGMainApp	*pApp = (CAxisGMainApp *)AfxGetApp();
	m_pfont = pApp->GetFont(m_pwndView, 9, "굴림");

	m_patternInfo.bmp = NULL;
	m_mode = MODE_DATA;
}

CToolTip::~CToolTip()
{
	if (m_patternInfo.bmp)
		::DeleteObject(m_patternInfo.bmp);
}


BEGIN_MESSAGE_MAP(CToolTip, CWnd)
	//{{AFX_MSG_MAP(CToolTip)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CToolTip message handlers

int CToolTip::Create()
{
	DWORD dwStyle = WS_BORDER | WS_POPUP; 
	DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;

	return CreateEx(dwExStyle, MAIN_TOOLTIP_CLASS, NULL, dwStyle, 
                    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
		            NULL, NULL, NULL );
}

int CToolTip::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_crBack = GetSysColor(COLOR_INFOBK);
	
	return 0;
}

void CToolTip::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMemDC	mdc(&dc);
	DrawTip(&mdc);
}
// modified by schbang 2006.03.15, Content 세로정렬
void CToolTip::DrawTip(CDC *pDC)
{
	CRect rc, rectC;
	GetClientRect(rc);
	pDC->FillSolidRect(rc, m_crBack);	
	CFont	*sFont = pDC->SelectObject(m_pfont);
	rectC = m_rcDraw;

	if (m_mode == MODE_DATA) 
	{
		CRect rectT = rectC;
		CString strText;
		int iCount = m_arTitle.GetSize();
		for (int ii = 0 ; ii < iCount ; ii++ )
		{
			rectT.bottom = rectT.top + m_iCellHeight;
			rectT.left = rectC.left;
			rectT.right = rectT.left + m_iMaxWidthTitle;
			strText = m_arTitle.GetAt(ii);
			pDC->DrawText(strText, rectT, DT_SINGLELINE|DT_VCENTER|DT_LEFT);

			rectT.left = rectT.right;
			rectT.right = rectC.right;
			strText = m_arContent.GetAt(ii);
			pDC->DrawText(strText, rectT, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
			rectT.top = rectT.bottom;
		}
	}
	else if (m_mode == MODE_TEXT)
	{
		pDC->DrawText(m_text, rc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	}
	else if (m_mode == MODE_PATTERN)
	{
		if (m_patternInfo.bmp) 
		{
			CString str;
			CRect rect(rc);
			
			rect.bottom = TITLE_HEIGHT;

			LOGFONT lf;
			m_pfont->GetLogFont(&lf);
			lf.lfWeight |= FW_BOLD;
			CFont boldFont;
			boldFont.CreateFontIndirect(&lf);

			CBrush brTitle, brComment;
			brTitle.CreateSolidBrush(RGB(219, 216, 213));
			brComment.CreateSolidBrush(RGB(249, 244, 241));

			pDC->FillRect(rect, &brTitle);
			pDC->SetBkMode(TRANSPARENT);

			rect.left += MARGIN;
			pDC->SetTextColor(RGB(72, 72, 72));
			CSize sz = pDC->GetTextExtent(m_patternInfo.datetime);
			str.Format("[%s] %s", m_patternInfo.datetime, m_patternInfo.alias);
			pDC->DrawText(m_patternInfo.datetime, rect, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
			rect.left += sz.cx + 5;
			pDC->SelectObject(&boldFont);
			pDC->DrawText(m_patternInfo.alias, rect, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
			pDC->SelectObject(m_pfont);

			rect = rc;
			rect.top += TITLE_HEIGHT;
			pDC->FillRect(rect, &brComment);

			CRect rcImage(rc);
			rcImage.left += MARGIN;
			rcImage.top = TITLE_HEIGHT + (rc.Height() - TITLE_HEIGHT - PATTERNIMAGE_HEIGHT) / 2;
			rcImage.right = rcImage.left + PATTERNIMAGE_WIDTH;
			rcImage.bottom = rcImage.top + PATTERNIMAGE_HEIGHT;

			CDC mdc;
			mdc.CreateCompatibleDC(pDC);
			HBITMAP oldBmp = (HBITMAP)mdc.SelectObject(m_patternInfo.bmp);

			pDC->BitBlt(rcImage.left, rcImage.top, rcImage.Width(), rcImage.Height(),
				&mdc, 0, 0, SRCCOPY);

			mdc.SelectObject(oldBmp);

			pDC->SelectStockObject(NULL_BRUSH);
			CPen penImage;
			penImage.CreatePen(PS_SOLID, 1, RGB(84, 84, 84));
			CPen	*sPen = pDC->SelectObject(&penImage);
			pDC->Rectangle(rcImage);
			pDC->SelectObject(sPen);

			rect = rcImage;
			rect.top  = TITLE_HEIGHT + MARGIN - 2;
			rect.left = MARGIN * 2 + PATTERNIMAGE_WIDTH;
			rect.right = rc.right - MARGIN;
			rect.bottom = rc.bottom - MARGIN;

			int y = rect.bottom;
			pDC->SetTextColor(0);
			pDC->DrawText(m_patternInfo.description, rect, DT_VCENTER | DT_LEFT | DT_WORDBREAK);
		}
		else 
		{
			pDC->DrawText(m_patternInfo.alias, rc, DT_CENTER);
		}
	}
	
	pDC->SelectObject(sFont);
}

LRESULT CToolTip::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_ERASEBKGND)
		return TRUE;
	
	return CWnd::WindowProc(message, wParam, lParam);
}


//title + \t + content + \n + title + \t + content + \n...
CRect CToolTip::SetData(CString sData)
{	
	m_mode = MODE_DATA;
	m_arTitle.RemoveAll();
	m_arContent.RemoveAll();

	int	iPos;
	CString	strTitle;
	CString	strCntnt;	// content;
	for (;!sData.IsEmpty();)
	{
		iPos = sData.Find('\t');
		if (iPos < 0)
			break;
		strTitle = sData.Left(iPos++);
		sData = sData.Mid(iPos);

		iPos = sData.Find('\t');
		if (iPos < 0)
			iPos = sData.GetLength();
		strCntnt = sData.Left(iPos++);
		strCntnt =  " : " + strCntnt;	// added by schbang 2006.03.15
		
		m_arTitle.Add(strTitle);
		m_arContent.Add(strCntnt);

		if (sData.GetLength() <= iPos)
			break;
		sData = sData.Mid(iPos);
	}

	return MakeSizeWnd();
}

CSize CToolTip::SetText(CString sText)
{
	m_mode = MODE_TEXT;
	m_text = sText;

	CWindowDC dc(this);

	CSize sz = dc.GetTextExtent(m_text);

	sz.cx += GAB * 2;
	sz.cy += GAB * 2;

	return sz;
}

CSize CToolTip::SetPattern(LPCTSTR datetime, LPCTSTR alias, LPCTSTR imageName, LPCTSTR description)
{
	if (m_patternInfo.bmp)
		::DeleteObject(m_patternInfo.bmp);

	m_patternInfo.bmp = (HBITMAP)LoadImage(NULL, imageName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (m_patternInfo.bmp == NULL) 
	{
		return CSize(0, 0);
	}

	m_patternInfo.datetime = datetime;
	m_patternInfo.alias    = alias;
	m_patternInfo.description = description;

	m_mode = MODE_PATTERN;

	CWindowDC dc(this);
	CFont	*sFont = dc.SelectObject(m_pfont);
	CRect rc(0, 0, PATTERN_WIDTH, PATTERN_HEIGHT);

	CRect rect;
	rect.top  = TITLE_HEIGHT;
	rect.left = MARGIN * 2 + PATTERNIMAGE_WIDTH;
	rect.right = rc.right - MARGIN;
	rect.bottom = rc.bottom - MARGIN;

	dc.DrawText(m_patternInfo.description, rect, DT_VCENTER | DT_LEFT | DT_WORDBREAK | DT_CALCRECT);
	dc.SelectObject(sFont);

	rect.bottom += MARGIN * 2;

	if (rect.bottom < TITLE_HEIGHT + PATTERNIMAGE_HEIGHT + MARGIN * 2)
		rect.bottom = TITLE_HEIGHT + PATTERNIMAGE_HEIGHT + MARGIN * 2;
	return CSize(PATTERN_WIDTH, rect.bottom);
}

CRect CToolTip::MakeSizeWnd() 
{
	CRect rcWnd;
	GetWindowRect(rcWnd);
	CRect rcDraw = rcWnd;
	rcDraw.DeflateRect(GAB, GAB, GAB * 2, GAB);
	
	CString	strTemp;
	CSize size = CSize(0, 0);
	int iCount = m_arTitle.GetSize();
	int iMaxWidthT = 0;	// title
	int iMaxWidthC = 0;	// contelnt
	for (int ii = 0 ; ii < iCount ; ii++ )
	{
		//strTemp.Format("%s %s", m_arTitle.GetAt(ii), m_arContent.GetAt(ii));
		size = CalWndSize(m_arTitle.GetAt(ii));
//		size.cx += 4;
		if (size.cx > iMaxWidthT)
			iMaxWidthT = size.cx;

		size = CalWndSize(m_arContent.GetAt(ii));
//		size.cx += 4;
		if (size.cx > iMaxWidthC)
			iMaxWidthC = size.cx;
	}
	m_iMaxWidthTitle = iMaxWidthT;

	m_iCellHeight = size.cy + 2;
	rcDraw.right = rcDraw.left + iMaxWidthT + iMaxWidthC;
	rcDraw.bottom = rcDraw.top + (m_iCellHeight * iCount);

	rcWnd = rcDraw;
	rcWnd.InflateRect(GAB, GAB, GAB * 2, GAB);
	m_rcDraw = rcDraw;
	ScreenToClient(m_rcDraw);
//	m_rcBase = rcWnd;	

	return rcWnd;
}

CSize CToolTip::CalWndSize(CString strExam)
{
	CDC* pDC = GetDC();
	CFont	*sFont = pDC->SelectObject(m_pfont);
	CSize size = pDC->GetTextExtent(strExam);
	pDC->SelectObject(sFont);
	ReleaseDC(pDC);		
	return size;
}

void CToolTip::OnMouseMove(UINT nFlags, CPoint point) 
{
	ClientToScreen(&point);

	if (m_pObjEvent)
		m_pObjEvent->TipMouseMove(point);

	CWnd::OnMouseMove(nFlags, point);
}

void CToolTip::WindowMove(int x, int y, int cx, int cy)
{
	SetWindowPos(&wndTop, x, y, cx, cy, SWP_NOACTIVATE);
	Invalidate();
}

void CToolTip::OnTimer(UINT nIDEvent) 
{
	KillTimer(nIDEvent);
	switch (nIDEvent) 
	{
	case 1: ShowWindow(SW_SHOW); break;
	}
		
	CWnd::OnTimer(nIDEvent);
}
