// Xcomex.cpp : implementation file
//

#include "stdafx.h"
#include "Xcomex.h"
#include "xedit.h"
#include "MenuXP.h"

#include "../../h/axisvar.h"

#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	COLOR_BUTTON	::GetSysColor(COLOR_BTNFACE)
/////////////////////////////////////////////////////////////////////////////
// CXcomex

CXcomex::CXcomex(CWnd* parent)
{
	m_bTracking  = FALSE;
	m_bHover     = FALSE;
	m_bDown		 = FALSE;
	m_parent = parent;
	m_bRc = CRect(0,0,0,0);
	CString	fontName;
	fontName.LoadString(STSM_FONTNAME_DEFAULT);
	m_font.CreatePointFont(90, fontName);

	m_arr = Axis::GetBitmap("axcombox");
	m_arr_dn = Axis::GetBitmap("axcombox_dn");
	m_back = Axis::GetBitmap("XCOMBX_BACK");
	m_band_back = Axis::GetSkinBitmap( "BAND" );
}

CXcomex::~CXcomex()
{
	if (m_pen.GetSafeHandle())	m_pen.DeleteObject();
	if (m_brush.GetSafeHandle())	m_brush.DeleteObject();

	m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CXcomex, CWnd)
	//{{AFX_MSG_MAP(CXcomex)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXcomex message handlers

void CXcomex::OnPaint() 
{
	CPaintDC dc(this);

	CDC memDC;
	memDC.CreateCompatibleDC( &dc );
	memDC.SelectObject( m_band_back );

	BITMAP	bm;
	m_back->GetBitmap(&bm);

	dc.StretchBlt( 0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0,7,3,22, SRCCOPY );
	
	DrawMaskedBitmap( &dc, 0, 0, m_back, RGB(255,0,255), bm.bmWidth, bm.bmHeight );		

	CRect	cRc;
	GetClientRect(cRc);

	CPen*	pen  = dc.SelectObject(&m_pen);
	CFont*	font = dc.SelectObject(&m_font);
	CBrush* pbr  = dc.SelectObject(&m_brush);
	
	/*if (m_bHover)
	{
		cRc.DeflateRect(1, 1, 1, 1);
		dc.FillSolidRect(cRc, RGB(245, 245, 255));
		cRc.InflateRect(1, 1, 1, 1);
		dc.Draw3dRect(cRc, RGB(0, 0, 0), RGB(0, 0, 0));
	}
	else
		dc.Rectangle(cRc);*/
	
	dc.SelectObject(pen);	
	
	draw_Button(&dc);

	cRc.left += 10;
	cRc.right -= (COMBTN_WIDTH-1);	
	const int mode = dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(COLOR_XCOMBX);	
	dc.DrawText(m_gName, cRc, DT_VCENTER|DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);
	dc.SetBkMode(mode);


	dc.SelectObject(font);
	dc.SelectObject(pbr);	
}

void CXcomex::init()
{
	m_pen.CreatePen(PS_SOLID, 0, COLOR_COMLINE);
	//m_brush.CreateSolidBrush(COLOR_BUTTON);
	m_brush.CreateSolidBrush(COLOR_BARBACKGROUND);
}

void CXcomex::changepos()
{
	CRect	cRc;
	
	GetClientRect(cRc);
	m_bRc.CopyRect(&cRc);
#if 0	// old style
	m_bRc.left = cRc.right - (COMBTN_WIDTH+1);
	m_bRc.DeflateRect(0, 1, 1, 1);
#else	// new style
	m_bRc.left = cRc.right - COMBTN_WIDTH;
	m_bRc.DeflateRect(0, 2);
	m_bRc.OffsetRect(-2, 0);
#endif
}

#define DSx     0x00660046L
#define DSna    0x00220326L


BOOL CXcomex::DrawMaskedBitmap(CDC* pDC, int x, int y, CBitmap* bitmap, UINT maskcolor, int width, int height)
{
	if (!bitmap || !bitmap->m_hObject)
		return FALSE;

	CDC			buffDC, maskDC, memoryDC, copyDC;
	CBitmap		buffBitmap, maskBitmap, copyBitmap;
	CBitmap* oldbuffBitmap{}, * oldmaskBitmap{}, * oldmemoryBitmap{}, * oldcopyBitmap{};

	buffDC.CreateCompatibleDC(pDC);
	buffBitmap.CreateCompatibleBitmap(pDC, width, height);
	oldbuffBitmap = buffDC.SelectObject(&buffBitmap);

	maskDC.CreateCompatibleDC(pDC);
	maskBitmap.CreateBitmap(width, height, 1, 1, NULL);
	oldmaskBitmap = maskDC.SelectObject(&maskBitmap);

	const CRect		maskRc( 0, 0, width, height);
	buffDC.FillSolidRect(&maskRc, maskcolor);

	memoryDC.CreateCompatibleDC(pDC);
	oldmemoryBitmap = memoryDC.SelectObject(bitmap);
	buffDC.BitBlt( 0, 0, width, height, &memoryDC, 0, 0, SRCCOPY);
	maskDC.BitBlt( 0, 0, width, height, &buffDC, 0, 0, SRCCOPY);

	copyDC.CreateCompatibleDC(pDC);
	copyBitmap.CreateCompatibleBitmap(pDC, width, height);
	oldcopyBitmap = copyDC.SelectObject(&copyBitmap);

	copyDC.BitBlt( 0, 0, width, height, pDC,  x, y, SRCCOPY);
	copyDC.BitBlt( 0, 0, width, height, &maskDC, 0, 0, SRCAND);
	buffDC.BitBlt( 0, 0, width, height, &maskDC, 0, 0, SRCMASK);
	copyDC.BitBlt( 0, 0, width, height, &buffDC, 0, 0, SRCPAINT);
	pDC->BitBlt(x, y, width, height, &copyDC, 0, 0, SRCCOPY);

	copyDC.SelectObject(&oldcopyBitmap);
	memoryDC.SelectObject(&oldmemoryBitmap);
	maskDC.SelectObject(&oldmaskBitmap);
	buffDC.SelectObject(&oldbuffBitmap);

	buffDC.DeleteDC();
	maskDC.DeleteDC();
	memoryDC.DeleteDC();
	copyDC.DeleteDC();
	
	return TRUE;
}

void CXcomex::draw_Button(CDC* pDC)
{
	if (m_bRc.IsRectEmpty())	
		return;
	if (!m_arr) 
		return;
	if (!m_back)
		return;
	BITMAP	bm;
	CDC	memDC;

	m_back->GetBitmap(&bm);
	if (m_back && m_back->m_hObject)
	{
		CBitmap* oldBmp = NULL;
		m_back->GetBitmap(&bm);
		
		if (!memDC.m_hDC)
			memDC.CreateCompatibleDC(pDC);
		oldBmp = memDC.SelectObject(m_back);
		//pDC->TransparentBlt( 0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255,0,255)  );
		//pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);		
		//memDC.SelectObject(oldBmp);
		//DrawTransparentBitmap( pDC->GetSafeHdc(), (HBITMAP)m_back, 0, 0, RGB(0,0,255) );
	}

	if( memDC.GetSafeHdc() )
		memDC.DeleteDC();
	
	m_arr->GetBitmap(&bm);	
	if (memDC.CreateCompatibleDC(pDC))
	{
		CBitmap* oldbuffBitmap{};
		if( m_bDown )
			oldbuffBitmap = memDC.SelectObject(m_arr_dn);
		else
			oldbuffBitmap = memDC.SelectObject(m_arr);

		if (oldbuffBitmap != (CBitmap *) 0)
		{
			/*pDC->StretchBlt(m_bRc.left, m_bRc.top, m_bRc.Width(), m_bRc.Height(), 
					&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);*/
			pDC->BitBlt( m_bRc.left, m_bRc.top, m_bRc.Width(), m_bRc.Height(), 
				&memDC, 0, 0, SRCCOPY );
			memDC.SelectObject(oldbuffBitmap);
		}
		else	
			memDC.DeleteDC();
	}
}

void CXcomex::set_gName(CString name)
{
	m_gName = name;
	Invalidate(FALSE);
}

void CXcomex::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bRc.PtInRect(point))
	{
		// 다운이미지 그리기 
		m_bDown = TRUE;
		Invalidate();		
		//
	
	}
	CWnd::OnLButtonDown(nFlags, point);
}


void CXcomex::OnMouseMove(UINT nFlags, CPoint point) 
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
	CWnd::OnMouseMove(nFlags, point);
}

LRESULT CXcomex::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	if (m_bHover)
		Invalidate();
	m_bTracking = FALSE;
	m_bHover = FALSE;
	m_bDown = FALSE;

	return 0;
}

LRESULT CXcomex::OnMouseHover(WPARAM wparam, LPARAM lparam)
{
	if (!m_bHover)
		Invalidate();
	m_bHover = TRUE;
	
	return 0;
}

void CXcomex::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_bDown )
	{
		const int	bufSize = 1024*4;
		CMenuXP		menu;
		CPoint		xpoint = m_bRc.BottomRight();
		CStringArray	ary;

		xpoint += CPoint(3, 3);
		ClientToScreen(&xpoint);
		menu.CreatePopupMenu();

		int		idx{}, uindex{}, tot{};
		CString text;

		CProfile profile(pkUserTool);
		text = profile.GetString(szUserSN, USEREN, "0, 0");
		idx  = text.Find(',');
		if (idx >= 0)	uindex = atoi(text.Left(idx++));
		tot = atoi(text.Mid(idx));

		CString code = _T("");
		CString gName = _T("");
		for (int ii = 0; ii < tot; ii++)
		{
			code.Format("%02d", ii);
			text = profile.GetString(szUserSN, code, "99=");
			if (text.IsEmpty() || text == "99=")
				break;
			menu.AppendMenuX(MF_STRING, COMM_BASE + ii+1, text);
		}
		
		menu.AppendMenuX(MF_SEPARATOR);
		menu.AppendMenuX(MF_STRING, COMM_BASE, "사용자 툴바편집");
		

		menu.TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON, xpoint.x, xpoint.y, m_parent);
	}

	m_bDown = FALSE;
	Invalidate();
	
	CWnd::OnLButtonUp(nFlags, point);
}
