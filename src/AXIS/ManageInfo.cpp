// ManageInfo.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "ManageInfo.h"
#include "../dll/ctrl/MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CManageInfo dialog
#define	TM_SLIDE	100
#define	TMI_SLIDE	100
#define	SLIDEGAP	10
#define	GAPX		4
#define SIZE_MARGIN	20

CManageInfo::CManageInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CManageInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManageInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nKind = 0;
	m_nRowHeight = 0;
	m_height = m_width = 0;
	
	m_font.CreateFont(	12,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_BOLD,	           // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				DEFAULT_CHARSET,           // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				_T("돋움체"));     

	m_bmp = Axis::GetBitmap("MESSAGE02");
	m_stretch = FALSE;
}


void CManageInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManageInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CManageInfo, CDialog)
	//{{AFX_MSG_MAP(CManageInfo)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManageInfo message handlers
long CManageInfo::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch ((int)wParam)
	{
		case 9898:
		{
			const UINT uFlags = SWP_NOACTIVATE | SWP_SHOWWINDOW;
			SetWindowPos(&wndTop, m_x, m_y, m_width, m_height, uFlags);

			if (m_height <= 10 || (int)lParam == 1)
			{
				KillTimer(9898);
				ShowWindow(SW_HIDE);
				return 0;
			}
			
			Invalidate();
		}
		break;
	}
	
	return 0;
}

void CManageInfo::OnDestroy() 
{
	CDialog::OnDestroy();	
	m_font.DeleteObject();
}

void CManageInfo::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case TM_SLIDE:	Slide();break;
	default:		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CManageInfo::ChangePalette()
{
}

void CManageInfo::ChangeData()
{
}

void CManageInfo::ShowSlide(CRect baseRC)
{
	m_baseRc.CopyRect(&baseRC);
	int x = m_baseRc.right - m_width;
	int y = m_baseRc.top;

	if (x < 0)	x = 0;	if (y < 0)	y = 0;

	if (m_baseRc.top < 0)
		y = 0;

	SetWindowPos(&wndTop, x, y, m_width, m_height, SWP_NOACTIVATE|SWP_SHOWWINDOW);
}

void CManageInfo::HideSlide()
{
#ifdef DF_MNG_THREAD
	std::thread([this]()
	{
			for (;;)
			{
				CRect	wRc;

				GetWindowRect(wRc);
				m_height = wRc.Height() - SLIDEGAP;

				if (m_height < 10)
				{
					PostMessage(WM_USER, 9898, 1);
					break;
				}

				m_x = m_baseRc.right - m_width;
				m_y = m_baseRc.top;

				if (m_x < 0)	m_x = 0;	if (m_y < 0)	m_y = 0;

				if (m_baseRc.top < 0)
					m_y = 0;

				PostMessage(WM_USER, 9898, 0);
				Sleep(50);
			}
	}).detach();
#else
	SetTimer(TM_SLIDE, TMI_SLIDE, NULL);
#endif
}

void CManageInfo::SetData(CString dat, int kind)
{
	m_nKind = kind;
	CString	tok = "\n";
	dat.TrimLeft(), dat.TrimRight();
	CString	tmpstr;
	dat = "\n" + dat + "\n";
	tmpstr.Format("%s", dat);	
	const int	ncnt = tmpstr.Replace(tok, tok);
	const bool	bSingleline = (ncnt) ? false : true;
	
	
	CDC*	pDC = GetDC();
	CFont*	font = pDC->SelectObject(&m_font);
	CSize	size;
	m_arData.RemoveAll();
	
	int	pos = 0;
	int	nMax = 0, nMaxY = 0;
	CString	sztmp = _T("");

	for ( int ii = 0 ; ii < ncnt + 1; ii++ )
	{
		pos = tmpstr.Find(tok);
		if (pos < 0)
		{
			tmpstr.TrimLeft(), tmpstr.TrimRight();
			size = pDC->GetTextExtent(tmpstr);
			m_arData.Add(tmpstr);
		}
		else
		{
			sztmp = tmpstr.Left(pos);
			sztmp.TrimLeft(), sztmp.TrimRight();
			size = pDC->GetTextExtent(sztmp);	
			m_arData.Add(sztmp);
			tmpstr = tmpstr.Mid(pos + 1);
		}

		if (size.cx > nMax)
			nMax = size.cx;	
		if (size.cy > nMaxY)
			nMaxY = size.cy;
	}

	size.cx = nMax;
	size.cy = nMaxY;
	pDC->SelectObject(font);	
	ReleaseDC(pDC);

	CRect	dispRc;
	m_nRowHeight = size.cy + 2;

	size.cy = m_nRowHeight * (ncnt + 1);
	dispRc.SetRect(0, 0, size.cx, size.cy);
	dispRc.InflateRect(SIZE_MARGIN, 0);
	dispRc.InflateRect(4, 4, 2, 2);
	m_width = dispRc.Width();
	m_height = dispRc.Height();

	if (m_bmp && m_bmp->m_hObject)
	{
		BITMAP bm;
		m_bmp->GetBitmap(&bm);
		if (m_width < bm.bmWidth)
		{
			m_stretch = FALSE;
			m_width = bm.bmWidth;
		}
		else
			m_stretch = TRUE;

		m_height = bm.bmHeight;
		
		m_bmpSize.cx = bm.bmWidth;
		m_bmpSize.cy = bm.bmHeight;
	}

	SetWindowPos(NULL, 0, 0, m_width, m_height, SWP_NOACTIVATE|SWP_HIDEWINDOW);
	m_rect = CRect(0, 0, m_width, m_height);	
}

void CManageInfo::SetDataColor(CString dat, COLORREF clrHeader, COLORREF clrCell)
{
	
}

void CManageInfo::Slide()
{
	CRect	wRc;
	int height{}, x{}, y{};
	const UINT uFlags = SWP_NOACTIVATE|SWP_SHOWWINDOW;
	
	GetWindowRect(wRc);
	height  = wRc.Height() - SLIDEGAP;
	if (height <= 10)
	{
		KillTimer(TM_SLIDE);
		ShowWindow(SW_HIDE);
		return; 
	}

	x = m_baseRc.right - m_width;
	y = m_baseRc.top;

	if (x < 0)	x = 0;	if (y < 0)	y = 0;

	if (m_baseRc.top < 0)
		y = 0;
	
	SetWindowPos(&wndTop, x, y, m_width, height, uFlags);
}

void CManageInfo::StopSlide()
{
	KillTimer(TM_SLIDE);
}

void CManageInfo::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CMemDC	mdc(&dc);
	OnDraw(&mdc);	
}

void CManageInfo::OnDraw(CDC* pDC)
{
	CFont*	old = pDC->SelectObject(&m_font);

	if (m_bmp)
		DrawBitmap(pDC);
	else
		DrawBack(pDC);
	
	DrawData(pDC);

	pDC->SelectObject(old);
}

void CManageInfo::DrawBack(CDC* pDC)
{
	COLORREF	bkcolor{};
	 // 0:장운영정보,1:알림,2:알림2
	if (!m_nKind)	
	{
		bkcolor = RGB(230, 236, 248);
	}
	else
	{
		bkcolor = RGB(255, 255, 166);
	}

	pDC->FillSolidRect(m_rect, bkcolor);
	DrawFrame(pDC);
}

void CManageInfo::DrawBitmap(CDC* pDC)
{
	ASSERT(m_bmp);

	CDC mdc;
	mdc.CreateCompatibleDC(pDC);

	CBitmap* oldBmp = mdc.SelectObject(m_bmp);

	if (m_stretch)
	{
		
		pDC->BitBlt(0, 0, 2, m_height, &mdc, 0, 0, SRCCOPY);
		pDC->StretchBlt(2, 0, m_width - m_bmpSize.cx, m_height, &mdc, 2, 0, 2, m_height, SRCCOPY);
		pDC->BitBlt(m_width - (m_bmpSize.cx - 2), 0, m_width - 2, m_height, &mdc, 2, 0, SRCCOPY);
	}
	else
	{
		pDC->BitBlt(0, 0, m_width, m_height, &mdc, 0, 0, SRCCOPY);
	}

	mdc.SelectObject(oldBmp);
}

void CManageInfo::DrawFrame(CDC* pDC)
{
	CBrush	brush1(RGB(55, 77, 116)), brush2(RGB(38, 109, 147)), brush3(RGB(97, 120, 164));	
	CRect	rect;
	rect = m_rect;
	pDC->FrameRect(rect, &brush1);
	rect.DeflateRect(1, 1);
	pDC->FrameRect(rect, &brush2);
	rect.DeflateRect(1, 1);
	pDC->FrameRect(rect, &brush3);
	rect.DeflateRect(1, 1);
	pDC->FrameRect(rect, &brush1);
}

void CManageInfo::DrawData(CDC* pDC)
{		
	const int	ncnt = m_arData.GetSize();
	CRect	cRc = m_rect;
	cRc.DeflateRect(SIZE_MARGIN, 0);
	cRc.DeflateRect(3, 3);
	CRect	rcText;
	CString	sztmp;
	rcText = cRc;
	rcText.bottom = rcText.top + m_nRowHeight;
	const int	nMode = pDC->SetBkMode(TRANSPARENT);

	COLORREF color{};

	if(m_nKind == 3)
	{
		color = pDC->SetTextColor(RGB(245,10,3));
	}
	else
	{
		color = pDC->SetTextColor(RGB(0, 0, 0));
	}

	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		sztmp = m_arData.GetAt(ii);
		pDC->DrawText(sztmp, rcText, DT_VCENTER|DT_LEFT|DT_SINGLELINE);
		rcText.OffsetRect(0, m_nRowHeight);
	}

	pDC->SetTextColor(color);
	pDC->SetBkMode(nMode);
}

#define WD_WARNING			(WM_USER+0x6001)
#define WD_GOODORDER		(WM_USER+0x6002)

void CManageInfo::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_nKind == 4)
	{
		m_pParentWnd->SendMessage(WD_WARNING,(WPARAM)0,(LPARAM)0);
	}
	else if(m_nKind == 5)
	{
		m_pParentWnd->SendMessage(WD_GOODORDER,(WPARAM)0,(LPARAM)0);
	}

	HideSlide();
	
	CDialog::OnLButtonDown(nFlags, point);
}
