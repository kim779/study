#include "stdafx.h"
#include "axis.h"
#include "dtInfo.h"
#include "../dll/ctrl/MemDC.h"
#include "axmsg.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	DT_SLIDE	100
#define	DT_SLIDE	100
#define	DT_SLIDEGAP	10
#define	DT_GAPX		4
#define DT_MARGIN	20

CDtInfo::CDtInfo(CWnd* pParent)
	: CDialog(CDtInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDtInfo)
	//}}AFX_DATA_INIT

	m_mainframe = pParent;
	m_nRowHeight = 0;
	m_height = m_width = 0;
	m_bDoctor = true;
	
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
				_T("µ¸¿òÃ¼"));     
}


void CDtInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDtInfo)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDtInfo, CDialog)
	//{{AFX_MSG_MAP(CDtInfo)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDtInfo::OnDestroy() 
{
	CDialog::OnDestroy();	
	m_font.DeleteObject();
	
}

void CDtInfo::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case DT_SLIDE:	Slide();break;
	default:		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CDtInfo::OnPaint() 
{
	CPaintDC dc(this);
	
	CMemDC	mdc(&dc);
	OnDraw(&mdc);	
}

void CDtInfo::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_mainframe)	
		m_mainframe->SendMessage(WM_AXIS, MAKEWPARAM(axDOCTOR, 0), 0);
	CDialog::OnLButtonDown(nFlags, point);
}
void CDtInfo::ChangePalette()
{
}

void CDtInfo::ChangeData()
{
}

void CDtInfo::ShowSlide(CRect baseRC)
{
	m_baseRc.CopyRect(&baseRC);
	int x = m_baseRc.right - m_width;
	int y = m_baseRc.top;

	if (x < 0)	x = 0;	if (y < 0)	y = 0;
	if (m_baseRc.right > GetSystemMetrics(SM_CXSCREEN))
		x = GetSystemMetrics(SM_CXSCREEN) - m_width;
	if (m_baseRc.top < 0)
		y = 0;

	SetWindowPos(&wndTop, x, y, m_width, m_height, SWP_NOACTIVATE|SWP_SHOWWINDOW);
}

void CDtInfo::HideSlide()
{
	SetTimer(DT_SLIDE, DT_SLIDE, NULL);
}

void CDtInfo::SetData(CString dat)
{
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
	dispRc.InflateRect(DT_MARGIN, 0);
	dispRc.InflateRect(4, 4, 2, 2);
	m_width = dispRc.Width();
	m_height = dispRc.Height();

	SetWindowPos(NULL, 0, 0, m_width, m_height, SWP_NOACTIVATE|SWP_HIDEWINDOW);
	m_rect = CRect(0, 0, m_width, m_height);	
}

void CDtInfo::SetDataColor(CString dat, COLORREF clrHeader, COLORREF clrCell)
{
	
}

void CDtInfo::Slide()
{
	CRect	wRc;
	int height{}, x{}, y{};
	const UINT uFlags = SWP_NOACTIVATE|SWP_SHOWWINDOW;
	
	GetWindowRect(wRc);
	height  = wRc.Height() - DT_GAPX;
	if (height <= 10)
	{
		KillTimer(DT_SLIDE);
		ShowWindow(SW_HIDE);
		return; 
	}

	x = m_baseRc.right - m_width;
	y = m_baseRc.top;

	if (x < 0)	x = 0;	if (y < 0)	y = 0;
	if (m_baseRc.right > GetSystemMetrics(SM_CXSCREEN))
		x = GetSystemMetrics(SM_CXSCREEN) - m_width;
	if (m_baseRc.top < 0)
		y = 0;
	
	SetWindowPos(&wndTop, x, y, m_width, height, uFlags);
}

void CDtInfo::StopSlide()
{
	KillTimer(DT_SLIDE);
}
void CDtInfo::OnDraw(CDC* pDC)
{
	CFont*	old = pDC->SelectObject(&m_font);
	DrawBack(pDC);
	DrawData(pDC);
	pDC->SelectObject(old);
}

void CDtInfo::DrawBack(CDC* pDC)
{
	COLORREF	bkcolor{};
	if (m_bDoctor)
		bkcolor = RGB(219, 222, 252);
	else
		bkcolor = RGB(255, 225, 240);

	pDC->FillSolidRect(m_rect, bkcolor);
	DrawFrame(pDC);
}

void CDtInfo::DrawFrame(CDC* pDC)
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

void CDtInfo::DrawData(CDC* pDC)
{		
	const int	ncnt = m_arData.GetSize();
	CRect	cRc = m_rect;
	cRc.DeflateRect(DT_MARGIN, 0);
	cRc.DeflateRect(3, 3);
	CRect	rcText;
	CString	sztmp;
	rcText = cRc;
	rcText.bottom = rcText.top + m_nRowHeight;
	const int	nMode = pDC->SetBkMode(TRANSPARENT);
	const COLORREF	color = pDC->SetTextColor(RGB(0, 0, 0));

	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		sztmp = m_arData.GetAt(ii);
		pDC->DrawText(sztmp, rcText, DT_VCENTER|DT_LEFT|DT_SINGLELINE);
		rcText.OffsetRect(0, m_nRowHeight);
	}

	pDC->SetTextColor(color);
	pDC->SetBkMode(nMode);
}

