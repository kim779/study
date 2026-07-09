// Bb.cpp : implementation file
//

#include "stdafx.h"
#include "Bb.h"
#include "bctrl1.h"
#include "bctrl2.h"
#include "bctrl3.h"
#include "../../axis/axmsg.hxx"
#include "../../h/axisvar.h"

#include "resource.h"

#define	DEFAULT_CTRL1	471
#define	DEFAULT_CTRL2	255
#define	CLOSEW		13
#define	TM_DELAY	100

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBb

CBb::CBb(bool (*axiscall)(int, WPARAM, LPARAM), CString home, COLORREF color)
{
	int ii = 0;
	m_axiscall = axiscall;
	m_home = home;
	m_bk = color;
	m_bList = false;

	for (ii = 0; ii < IMAX; ii++)
		m_imgRc[ii].SetRect(0, 0, 0, 0);

	m_ctrl1 = NULL;
	m_ctrl2 = NULL;
	m_ctrl3 = NULL;

	m_hover  = -1;
	m_cursor = 0;
	m_bIndex = 0;
	m_bCnt   = 0;
	m_delay  = 0;
	m_mouseIN = false;
	m_closeRc = CRect(0, 0, CLOSEW, CLOSEW);

	CString bottoms[5] = { "", "  장애시대처방법  ", "  위험안내고지  ", "  이용약관  ", "  고객지원센터 1588-4588  " };
	for (ii = 0; ii < 5; ii++)
		m_bottoms[ii] = bottoms[ii];
}

CBb::~CBb()
{
	if (m_ctrl1)	
	{
		m_ctrl1->DestroyWindow();
		delete m_ctrl1;
	}
	if (m_ctrl2)	
	{
		m_ctrl2->DestroyWindow();
		delete m_ctrl2;
	}
	if (m_ctrl3)	
	{
		m_ctrl3->DestroyWindow();
		delete m_ctrl3;
	}
}


BEGIN_MESSAGE_MAP(CBb, CWnd)
	//{{AFX_MSG_MAP(CBb)
	ON_WM_CHILDACTIVATE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBb message handlers

void CBb::OnChildActivate() 
{
	CWnd::OnChildActivate();
	SetWindowPos(&wndBottom, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOREDRAW);
}

void CBb::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	 cRc, rc;

	GetClientRect(cRc);
//	dc.SetBkColor(m_bk);

	dc.ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, &cRc, "", 0, NULL);

	rc = cRc;
	dc.FillSolidRect(rc, RGB(22, 43, 83));
	rc.DeflateRect(1, 1);
	dc.FillSolidRect(rc, RGB(158, 160, 202));
	rc.DeflateRect(2, 2);
	dc.FillSolidRect(rc, RGB(103, 128, 175));

	draw_Img(&dc);
	draw_Benner(&dc);
	draw_Etc(&dc);
}

void CBb::draw_Img(CDC* pDC)
{
	CString		file;
	HBITMAP		hBitmap;
	
	file.Format("%s\\image\\intro.bmp", m_home);
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); 
	
	if (hBitmap == NULL)	return;
	BITMAP  bm;	CDC     buffDC ;
	(CBitmap::FromHandle(hBitmap))->GetBitmap(&bm);
	
	buffDC.CreateCompatibleDC(pDC) ;
	CBitmap* oldbuffBitmap = buffDC.SelectObject(CBitmap::FromHandle(hBitmap));
	
	pDC->BitBlt(8, 8, bm.bmWidth, bm.bmHeight, &buffDC, 0, 0, SRCCOPY);
	buffDC.SelectObject(oldbuffBitmap);
	buffDC.DeleteDC();
	DeleteObject(hBitmap);
}

void CBb::draw_Benner(CDC* pDC)
{
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;

	CString		file, section, key;
	HBITMAP		hBitmap;
	char		buf[256];

	file.Format("%s\\%s\\axisbb.ini", m_home, TABDIR);
	GetPrivateProfileString("BENNER", "imgCount", "0", buf, sizeof(buf), file);
	m_bCnt = atoi(buf);
	if (m_bCnt < 1)	return;

	file.Format("%s\\image\\b%d.bmp", m_home, m_bIndex);
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			  IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); 

	if (hBitmap == NULL) return;

	BITMAP  bm;
	CDC     buffDC;
	(CBitmap::FromHandle(hBitmap))->GetBitmap(&bm);
	
	buffDC.CreateCompatibleDC(pDC);
	CBitmap* oldbuffBitmap = buffDC.SelectObject(CBitmap::FromHandle(hBitmap));

	pDC->StretchBlt(m_imgRc[IMAX].left, m_imgRc[IMAX].top, bm.bmWidth, bm.bmHeight,
			&buffDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
	buffDC.SelectObject(oldbuffBitmap);
	buffDC.DeleteDC();
	DeleteObject(hBitmap);

	bool	top = false;
	int	x, y, gap = 2;
	file.Format("%s\\%s\\axisbb.ini", m_home, TABDIR);
	section.Format("B%d", m_bIndex);
	GetPrivateProfileString(section, "pos", "0", buf, sizeof(buf), file);
	if (atoi(buf) == 1)	top = true;

	for (int ii = 1; ii <= 2 && m_bCnt > 1; ii++)
	{
		file.Format("%s\\image\\arr%d.bmp", m_home, ii);
		hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
				  IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); 

		if (hBitmap == NULL)	continue;

		BITMAP  bm;	
		CDC     buffDC ;
		(CBitmap::FromHandle(hBitmap))->GetBitmap(&bm);
		
		buffDC.CreateCompatibleDC(pDC) ;
		CBitmap* oldbuffBitmap = buffDC.SelectObject(CBitmap::FromHandle(hBitmap));
		
		x = m_imgRc[IMAX].right - (gap + bm.bmWidth)*ii;
		if (top)
		{
			y = m_imgRc[IMAX].top + gap;
			pDC->StretchBlt(x, y, bm.bmWidth, bm.bmHeight,
					&buffDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY); 
		}
		else
		{
			y = m_imgRc[IMAX].bottom - (gap + bm.bmWidth);
			pDC->StretchBlt(x, y, bm.bmWidth, bm.bmHeight,
					&buffDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY); 
		}
		buffDC.SelectObject(oldbuffBitmap);
		buffDC.DeleteDC();
		DeleteObject(hBitmap);
	}
}

void CBb::draw_Etc(CDC* pDC)
{
	COLORREF color = pDC->SetTextColor(RGB(255, 255, 255));

	CFont	font;
	font.CreatePointFont(90, "굴림");
	CFont *pOldFont = pDC->SelectObject(&font);

	for (int ii = 0; ii < 5; ii++)
	{
		if (!ii)
			pDC->DrawText(m_bottoms[ii], m_imgRc2[ii], DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		else
		{
			pDC->DrawText(m_bottoms[ii], m_imgRc2[ii], DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			if (ii != 4)
			{
				CRect tRc;
				tRc.SetRect(m_imgRc2[ii].right - 5, m_imgRc2[ii].top, m_imgRc2[ii].right + 5, m_imgRc2[ii].bottom);
				pDC->DrawText("|", tRc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
		}
	}

	pDC->SelectObject(pOldFont);
	pDC->SetTextColor(color);
}

void CBb::draw_CoolImg(CDC* pDC)
{
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;

	CString		file, file__, file2, file2__;
	file.Format("%s\\image\\intro.bmp", m_home);
	file__.Format("%s\\image\\intro__.bmp", m_home);
	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
				  IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	HBITMAP hBitmap__ = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file__,
			  IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	file2.Format("%s\\image\\intro2.bmp", m_home);
	file2__.Format("%s\\image\\intro2__.bmp", m_home);
	HBITMAP hBitmap2 = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file2,
				  IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	HBITMAP hBitmap2__ = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file2__,
			  IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	for (int ii = 0; ii < IMAX; ii++)
	{
		CDC     buffDC ;
		CBitmap* oldbuffBitmap = NULL;
		buffDC.CreateCompatibleDC(pDC) ;

		if (m_bList && ii == IMAX - 1)
		{
			if (ii == m_hover)
				oldbuffBitmap = buffDC.SelectObject(CBitmap::FromHandle(hBitmap2__));
			else
				oldbuffBitmap = buffDC.SelectObject(CBitmap::FromHandle(hBitmap2));
			
			pDC->StretchBlt(m_imgRc[ii].left, m_imgRc[ii].top, m_imgRc[ii].Width(), m_imgRc[ii].Height(),
				&buffDC, 0, 0, m_imgRc[ii].Width(), m_imgRc[ii].Height(), SRCCOPY); 
		}
		else
		{
			if (ii == m_hover)
				oldbuffBitmap = buffDC.SelectObject(CBitmap::FromHandle(hBitmap__));
			else
				oldbuffBitmap = buffDC.SelectObject(CBitmap::FromHandle(hBitmap));
			
			pDC->StretchBlt(m_imgRc[ii].left, m_imgRc[ii].top, m_imgRc[ii].Width(), m_imgRc[ii].Height(),
				&buffDC, 0, m_imgRc[ii].top - IMG_GAP, m_imgRc[ii].Width(), m_imgRc[ii].Height(), SRCCOPY); 
		}

		buffDC.SelectObject(oldbuffBitmap);
		buffDC.DeleteDC();
	}
	DeleteObject(hBitmap);
	DeleteObject(hBitmap__);
}

void CBb::draw_HotImg(int index, CDC* pDC)
{
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;
	
	CString		file;
	HBITMAP		hBitmap;
	
	if (m_bList && index == IMAX-1) 
		file.Format("%s\\image\\%d_.bmp", m_home, index+1);
	else
		file.Format("%s\\image\\%d_.bmp", m_home, index);
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			  IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); 

	if( hBitmap)
	{
		BITMAP  bm;	
		CDC     buffDC ;
		(CBitmap::FromHandle(hBitmap))->GetBitmap(&bm);
		
		buffDC.CreateCompatibleDC( pDC ) ;
		CBitmap* oldbuffBitmap = buffDC.SelectObject(CBitmap::FromHandle(hBitmap));
		pDC->StretchBlt(m_imgRc[index].left, m_imgRc[index].top, 
				m_imgRc[index].Width(), m_imgRc[index].Height(),
				&buffDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY); 
		buffDC.SelectObject(oldbuffBitmap);
		buffDC.DeleteDC();
		DeleteObject(hBitmap);
	}
}

void CBb::init()
{
	CString		file;
	BITMAP		bm;
	HBITMAP		hBitmap;

	file.Format("%s\\image\\intro.bmp", m_home);
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	(CBitmap::FromHandle(hBitmap))->GetBitmap(&bm);
	int	top[IMAX+1]	= { 69,  91, 137, 156, 175, 210, 235, 260, 285, 310, 335, 361 };
	int	bottom[IMAX+1]	= { 89, 111, 155, 174, 193, 233, 258, 283, 308, 333, 358, 492 };
	for (int ii = 0; ii <= IMAX; ii++)
	{
		m_imgRc[ii].SetRect(0, top[ii], bm.bmWidth - 1, bottom[ii]);
		m_imgRc[ii].OffsetRect(IMG_GAP, IMG_GAP);
	}
	DeleteObject(hBitmap);

	make_Ctrl();
	fit();

	char	buf[32];
	file.Format("%s\\%s\\axisbb.ini", m_home, TABDIR);
	GetPrivateProfileString("BENNER", "delaytime", "0", buf, sizeof(buf), file);

	m_delay = atoi(buf);
	if (m_delay > 0)
		SetTimer(TM_DELAY, m_delay*1000*60, NULL);
}

void CBb::fit()
{
	int	cx = 0, cy = 0, cy2 = 0;

	SetWindowPos(&wndTop, 0, 0, 890, 522, SWP_NOMOVE);

	CString		file;
	CRect		cRc;
	int		move;

	GetClientRect(cRc);

	cx = DEFAULT_CTRL1;
	cy = 490;
	m_ctrl1->SetWindowPos(NULL, m_imgRc[0].right + 4, IMG_GAP, cx, cy, SWP_SHOWWINDOW);

	move = m_imgRc[0].right + cx + 4;
	cx = DEFAULT_CTRL2;
	
	cy2 = 117;
	m_ctrl3->SetWindowPos(NULL, move, IMG_GAP, cx, cy2, SWP_SHOWWINDOW);
	m_ctrl2->SetWindowPos(NULL, move, IMG_GAP +  cy2, cx, cy - cy2, SWP_SHOWWINDOW);

	CDC *pDC = GetDC();
	CFont	font;
	font.CreatePointFont(90, "굴림");
	CFont *pOldFont = pDC->SelectObject(&font);

	m_imgRc2[0].SetRect(IMG_GAP, m_imgRc[IMAX].bottom + 4, IMG_GAP + 300, m_imgRc[IMAX].bottom + 16);
	move = cx + move;
	for (int ii = 4; ii > 0; ii--)
	{
		CSize sz = pDC->GetTextExtent(m_bottoms[ii]);
		m_imgRc2[ii].SetRect(move - sz.cx, m_imgRc2[0].top, move, m_imgRc2[0].bottom);

		move -= sz.cx;
	}
	pDC->SelectObject(pOldFont);
}

void CBb::make_Ctrl()
{
	DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	m_ctrl1 = new CBCtrl1(m_home);
	if (!m_ctrl1->Create(AfxRegisterWndClass(0), "ctrl1", dwStyle, CRect(0), this, -1))
	{
		delete m_ctrl1;
		m_ctrl1 = NULL;
	}

	m_ctrl2 = new CBCtrl2(m_home);
	if (!m_ctrl2->Create(AfxRegisterWndClass(0), "ctrl2", dwStyle, CRect(0), this, -1))
	{
		delete m_ctrl2;
		m_ctrl2 = NULL;
	}

	m_ctrl3 = new CBCtrl3(m_home);
	if (!m_ctrl3->Create(AfxRegisterWndClass(0), "ctrl3", dwStyle, CRect(0), this, -1))
	{
		delete m_ctrl3;
		m_ctrl3 = NULL;
	}
}

LRESULT CBb::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_mouseIN = false;

	m_hover = -1;
	m_cursor = 0;

	draw_CoolImg();

	return 0;
}

void CBb::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd*			wndUnderMouse = NULL;
	CWnd*			wndActive = this;
	TRACKMOUSEEVENT		csTME;

	ChangeCursor(point);
	CWnd::OnMouseMove(nFlags, point);
	CPoint	lpoint = point;
	ClientToScreen(&point);
	wndUnderMouse = WindowFromPoint(point);
	if (wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd && wndActive)
	{
		if (!m_mouseIN)
		{
			m_mouseIN = true;

			csTME.cbSize = sizeof(csTME);
			csTME.dwFlags = TME_LEAVE;
			csTME.hwndTrack = m_hWnd;
			::_TrackMouseEvent(&csTME);
		}
		HoverItem(lpoint);
	}
	else 	m_mouseIN = false;
}

void CBb::HoverItem(CPoint point)
{
	int ii = 0;
	for (ii = 0; ii < IMAX; ii++)
	{
		if (!m_imgRc[ii].PtInRect(point))
			continue;

		if (ii == m_hover)	return;

		m_hover = ii;
		break;
	}
	if (ii == IMAX)	m_hover = -1;
	draw_CoolImg();
}

void CBb::Toggle()
{
	m_bList = !m_bList;

	draw_CoolImg();
//	draw_HotImg(IMAX-1);
}

void CBb::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int ii = 0;
	int	inc = NextBenner(point);
	if (inc != 0)
	{
		KillTimer(TM_DELAY);
		m_bIndex += inc;
		if (m_bIndex < 0)
			m_bIndex = m_bCnt - 1;
		if (m_bIndex >= m_bCnt)
			m_bIndex = 0;
		
		draw_Benner();
		if (m_delay >= 1)
		{
			SetTimer(TM_DELAY, m_delay*1000*60, NULL);
		}
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	if (m_closeRc.PtInRect(point))
	{
		ShowWindow(SW_HIDE);
		GetParent()->Invalidate(FALSE);
	}
	else
	{
		char		buf[1024];
		bool		match = false;
		CString		file, key, link = _T("");
		file.Format("%s\\%s\\axisbb.ini", m_home, TABDIR);
		
		for (ii = 0; ii < IMAX; ii++)
		{
			if (!m_imgRc[ii].PtInRect(point))
				continue;
			match = true;

			switch (ii)
			{
			case 0:		// Daily News
			case 1:		// Global market Review
			case 2:		// 주가 예측
			case 3:		// 인공지능검색
			case 4:		// 종목검색종합
			case 5:		// 개인일정
			case 6:		// HTS 도움말
			case 7:		// 신규상장종목
			case 8:		// 영업점 안내
			case 9:		// 공인인증서
				key.Format("%d", ii);
				GetPrivateProfileString("LINK", key, "", buf, sizeof(buf), file);
				link = buf;
				link.TrimRight();
				if (!link.IsEmpty()){
					if (link.Mid(0,4) == "http")
						(*m_axiscall)(AXI_BOARD, ii, (long) link.operator LPCTSTR());
					else
						(!(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)buf));
				}
				break;
			case IMAX - 1:	// 화면목록 or 공지사항.
				if (m_bList)
					(*m_axiscall)(AXI_BOARD, ii, (long) _T(""));
				else	
					(*m_axiscall)(AXI_BOARD, ii, (long) _T("SE000102"));
				break;
			}
			break;
		}

		for (ii = 1; ii <= 4 && !match; ii++)
		{
			if (!m_imgRc2[ii].PtInRect(point))
				continue;
			match = true;
			(*m_axiscall)(AXI_BOARD, ii + 20, 0);
		}

		if (!match)
		{
			link = ChangeCursor(point);
			if (!link.IsEmpty())
				(*m_axiscall)(AXI_BOARD, 100, (long) link.operator LPCTSTR());
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

BOOL CBb::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	switch (m_cursor)
	{
	case 1:
		SetCursor(AfxGetApp()->LoadCursor(IDC_HANDCURSOR));
		return TRUE;
	default:
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		break;
	}	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

CString CBb::ChangeCursor(CPoint point)
{
	if (!m_imgRc[IMAX].PtInRect(point))
	{
		m_cursor = 0;
		return _T("");
	}

	CString	file, section, key, dat, tmpS;
	char	buf[1024];

	file.Format("%s\\%s\\axisbb.ini", m_home, TABDIR);
	section.Format("B%d", m_bIndex);
	key = _T("linkN");
	GetPrivateProfileString(section, key, "0", buf, sizeof(buf), file);
	int	cnt = atoi(buf);
	if (cnt < 1)	return _T("");

	CRgn	rgn;
	int	x = 0, y = 0, cx = 0, cy = 0, roundx = 0, roundy = 0, pos;
	for (int ii = 1; ii <= cnt; ii++)
	{
		key.Format("region%d", ii);
		GetPrivateProfileString(section, key, "0, 0, 0, 0", buf, sizeof(buf), file);
		dat = buf;

		for (int jj = 0; jj < 6; jj++)
		{
			pos = dat.Find(',');
			if (pos == -1)
			{
				switch (jj)
				{
				case 0:	x = atoi(dat);		break;
				case 1:	y = atoi(dat);		break;
				case 2: cx = atoi(dat);		break;
				case 3:	cy = atoi(dat);		break;
				case 4: roundx = atoi(dat);	break;
				case 5:	roundy = atoi(dat);	break;
					break;
				}
				break;
			}
			tmpS = dat.Left(pos++);
			dat = dat.Mid(pos);
			switch (jj)
			{
			case 0:	x = atoi(tmpS);		break;
			case 1:	y = atoi(tmpS);		break;
			case 2: cx = atoi(tmpS);	break;
			case 3:	cy = atoi(tmpS);	break;
			case 4: roundx = atoi(tmpS);	break;
			case 5:	roundy = atoi(tmpS);	break;
			}
		}

		key.Format("shape%d", ii);
		GetPrivateProfileString(section, key, "0", buf, sizeof(buf), file);
		switch (atoi(buf))
		{
		case 0:	// rectangle
			rgn.CreateRectRgn(x, y, x + cx, y + cy);			break;
		case 1:	// circle
			rgn.CreateEllipticRgn(x, y, x + cx, y + cy);			break;
		case 2:	// round rectangle
			rgn.CreateRoundRectRgn(x, y, x + cx, y + cy, roundx, roundy);	break;
		default:continue;
		}

		rgn.OffsetRgn(m_imgRc[IMAX].left, m_imgRc[IMAX].top);
		CRect cRc;
		cRc.SetRect(x, y, x + cx, y + cy);
		cRc.OffsetRect(m_imgRc[IMAX].left, m_imgRc[IMAX].top);

		if (rgn.PtInRegion(point))
		{
			key.Format("url%d", ii);
			GetPrivateProfileString(section, key, "www.pru.co.kr", buf, sizeof(buf), file);
			rgn.DeleteObject();

			m_cursor = 1;
			SetCursor(AfxGetApp()->LoadCursor(IDC_HANDCURSOR));
			return buf;
		}
		else
		{
			m_cursor = 0;
			rgn.DeleteObject();
		}
	}

	if (rgn.GetSafeHandle())	rgn.DeleteObject();

	return _T("");
}

void CBb::OnTimer(UINT_PTR nIDEvent) 
{
	if (nIDEvent == TM_DELAY)
	{
		m_bIndex++;
		if (m_bIndex >= m_bCnt)
			m_bIndex = 0;
		draw_Benner();
	}
	CWnd::OnTimer(nIDEvent);
}

int CBb::NextBenner(CPoint point)
{
	if (m_bCnt < 1)	return 0;

	CString		file, section, key;
	HBITMAP		hBitmap;
	char		buf[256];

	file.Format("%s\\image\\b%d.bmp", m_home, m_bIndex);
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			  IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); 

	if( hBitmap == NULL) return 0;

	BITMAP  bm;
	(CBitmap::FromHandle(hBitmap))->GetBitmap(&bm);
	DeleteObject(hBitmap);

	bool	top = false;
	int	x, y, gap = 2;
	file.Format("%s\\%s\\axisbb.ini", m_home, TABDIR);
	section.Format("B%d", m_bIndex);
	GetPrivateProfileString(section, "pos", "0", buf, sizeof(buf), file);
	if (atoi(buf) == 1)	top = true;

	for (int ii = 1; ii <= 2; ii++)
	{
		file.Format("%s\\image\\arr%d.bmp", m_home, ii);
		hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
				  IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); 

		if( hBitmap == NULL)	continue;
		(CBitmap::FromHandle(hBitmap))->GetBitmap(&bm);
		DeleteObject(hBitmap);

		x = m_imgRc[IMAX].right - (gap + bm.bmWidth)*ii;
		if (top)
			y = m_imgRc[IMAX].top + gap;
		else
			y = m_imgRc[IMAX].bottom - (gap + bm.bmWidth);

		CRect	aRc;
		aRc.SetRect(x, y, x + bm.bmWidth, y + bm.bmHeight);
		if (aRc.PtInRect(point))
		{
			if (ii == 1)	return 1;
			return -1;
		}
	}
	return 0;
}

BOOL CBb::GetReload(int id)
{
	BOOL	reload = FALSE;
	switch (id)
	{
	case 1:	if (!m_bList)
			reload = TRUE;	break;
	case 2:	reload = TRUE;		break;
	case 3: if (!m_bList)
			reload = TRUE;	break;
	default:			break;
	}
	return reload;
}

void CBb::SetLastTime(CString time)
{
	m_bottoms[0] = time;
}