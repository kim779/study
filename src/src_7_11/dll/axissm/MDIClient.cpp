// MDIClient.cpp : implementation file
//

#include "stdafx.h"
#include "MDIClient.h"
#include "bb.h"
#include "bctrl1.h"
#include "bctrl2.h"
#include "bctrl3.h"
#include "../../axis/axmsg.hxx"
#include "../../h/axisvar.h"

#define	BTN_GAP		5
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDIClient

CMDIClient::CMDIClient(bool (*axiscall)(int, WPARAM, LPARAM), CString home, COLORREF color)
{
	m_axiscall = axiscall;
	m_home = home;
	m_bk = color;
	m_bb = NULL;

	m_ctrlKey = 0;
	m_shiftKey = false;
}

CMDIClient::~CMDIClient()
{
	if (m_bb)	delete m_bb;
}


BEGIN_MESSAGE_MAP(CMDIClient, CWnd)
	//{{AFX_MSG_MAP(CMDIClient)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMDIClient message handlers

void CMDIClient::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	 cRc;

	GetClientRect(cRc);
	dc.SetBkColor(m_bk);
	dc.ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, &cRc, "", 0, NULL);

	if (m_bb && !(m_bb->GetStyle() & WS_VISIBLE))
	{
		draw_Img(&dc);
		draw_Button(&dc);
	}
	else
		draw_Button(&dc);
}

void CMDIClient::draw_Img(CDC* pDC)
{
	CString		file;
	HBITMAP		hBitmap;
	
	if (m_bUseImage && !m_userImg.IsEmpty())
	{
		hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_userImg,
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		
		if (hBitmap)
		{
			CRect	cRc;
			BITMAP  bm;	CDC     buffDC ;
			(CBitmap::FromHandle(hBitmap))->GetBitmap(&bm);
			
			GetClientRect(cRc);
			buffDC.CreateCompatibleDC(pDC) ;
			CBitmap* oldbuffBitmap = buffDC.SelectObject(CBitmap::FromHandle(hBitmap));
			
			switch (m_pos)
			{
			case 0:
				pDC->StretchBlt((cRc.Width() - bm.bmWidth) / 2, (cRc.Height() - bm.bmHeight) / 2, 
					bm.bmWidth, bm.bmHeight, &buffDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
				break;
			case 1:
				{
					CRect rc;
					rc.SetRect(cRc.left, cRc.top, bm.bmWidth, bm.bmHeight);
					for (int ii = 0; ; ii++)
					{
						for (int jj = 0; ; jj++)
						{
							pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &buffDC, 0, 0, SRCCOPY);

							if (rc.right > cRc.right)
								break;
							else
								rc.OffsetRect(bm.bmWidth, 0);
						}

						if (rc.bottom > cRc.bottom)
							break;
						else
							rc.SetRect(cRc.left, rc.bottom, bm.bmWidth, rc.bottom + bm.bmHeight);
					}
				}
				break;
			case 2:
				pDC->StretchBlt(0, 0, cRc.Width(), cRc.Height(),
					&buffDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY); 
				break;
			}

			buffDC.SelectObject(oldbuffBitmap);
			DeleteObject(hBitmap);
		}
	}
	else
	{
		file.Format("%s\\image\\%s.bmp", m_home, m_img);
		hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); 
		
		if (hBitmap)
		{
			CRect	cRc;
			BITMAP  bm;	CDC     buffDC ;
			(CBitmap::FromHandle(hBitmap))->GetBitmap(&bm);
			
			GetClientRect(cRc);
			buffDC.CreateCompatibleDC(pDC) ;
			CBitmap* oldbuffBitmap = buffDC.SelectObject(CBitmap::FromHandle(hBitmap));
			pDC->StretchBlt(0, cRc.bottom - bm.bmHeight, bm.bmWidth, bm.bmHeight,
				&buffDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY); 
			buffDC.SelectObject(oldbuffBitmap);
			DeleteObject(hBitmap);
		}
	}
}

void CMDIClient::draw_Button(CDC* pDC)
{
	if (!m_bb)	return;

	CString		file;
	HBITMAP		hBitmap;

	if (m_bb->GetStyle() & WS_VISIBLE)
		file.Format("%s\\image\\%s.bmp", m_home, "intro_show");
	else
		file.Format("%s\\image\\%s.bmp", m_home, "intro_hide");
	
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); 
	
	if (hBitmap)
	{
		CRect	cRc, rc;
		BITMAP  bm;	CDC     buffDC;
		(CBitmap::FromHandle(hBitmap))->GetBitmap(&bm);
		
		if (m_bb->GetStyle() & WS_VISIBLE)
		{
			m_bb->GetWindowRect(cRc);
			ScreenToClient(cRc);

			rc.SetRect(cRc.right - (bm.bmWidth+BTN_GAP), cRc.top - bm.bmHeight, 
				cRc.right - bm.bmWidth, cRc.top);
		}
		else
		{
			GetClientRect(cRc);
			rc.SetRect(cRc.right - (bm.bmWidth + BTN_GAP), cRc.top + BTN_GAP,
				cRc.right - BTN_GAP, cRc.top + BTN_GAP + bm.bmHeight);
		}
		
		buffDC.CreateCompatibleDC(pDC);
		CBitmap* oldbuffBitmap = buffDC.SelectObject(CBitmap::FromHandle(hBitmap));
		pDC->StretchBlt(rc.left, rc.top, bm.bmWidth, bm.bmHeight,
			&buffDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY); 
		buffDC.SelectObject(oldbuffBitmap);
		DeleteObject(hBitmap);
	}
}

void CMDIClient::change_Color(CString img, COLORREF color, CString user)
{
	m_bk = color;
	m_img = img;

	char	buf[256];
	CString file, value, section = "Image";
	file.Format("%s\\%s\\%s\\%s", m_home, USRDIR, user, SETUPFILE);

	m_bUseImage = GetPrivateProfileInt(section, "UseImage", 0, file);
	m_bUseColor = GetPrivateProfileInt(section, "UseColor", 0, file);
	DWORD dw = GetPrivateProfileString(section, "bkFile", "", buf, sizeof(buf), file);
	if (dw > 0)
		m_userImg = buf;
	if (m_bUseColor)
		m_bk = GetPrivateProfileInt(section, "bkColor", RGB(0, 0, 0), file);
	m_pos = GetPrivateProfileInt(section, "posImage", 0, file);

	Invalidate(FALSE);
}

void CMDIClient::make_board(COLORREF bk, bool visible)
{
	ShowWindow(SW_HIDE);
	m_bb = new CBb(m_axiscall, m_home);
	CString strWndClass = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW,
		              ::LoadCursor(NULL, IDC_ARROW),
			      (HBRUSH) ::GetStockObject(WHITE_BRUSH), 0);
	DWORD	dwStyle = WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
	if (visible)	dwStyle |= WS_VISIBLE;

	if (!m_bb->Create(strWndClass, NULL, dwStyle, CRect(0), this, -1))
	{
		delete m_bb;
		m_bb = NULL;
		return;
	}

	m_bb->init();
	m_bb->SetWindowPos(&wndBottom, 0, 0, 0, 0, SWP_NOACTIVATE);
}

void CMDIClient::set_Key(int id, int key)
{
	if (!m_bb)	return;
	switch (id)
	{
	case 1:	m_bb->m_ctrl1->set_Key(key);	break;
	case 2:	m_bb->m_ctrl2->set_Key(key);	break;
	case 3:	m_bb->m_ctrl3->set_Key(key);	break;
	default:	break;
	}
}

int CMDIClient::get_Key(int id)
{
	if (!m_bb)	return 0;
	switch (id)
	{
	case 1:	return m_bb->m_ctrl1->get_Key();
	case 2:	return m_bb->m_ctrl2->get_Key();
	case 3:	return m_bb->m_ctrl3->get_Key();
	default:	break;
	}
	return 0;
}

CWnd* CMDIClient::get_Ctrl(int index)
{
	if (!m_bb)	return NULL;
	switch (index)
	{
	case 1:	return (CWnd *) m_bb->m_ctrl1;
	case 2:	return (CWnd *) m_bb->m_ctrl2;
	case 3:	return (CWnd *) m_bb->m_ctrl3;
	default:return NULL;
	}
	return NULL;
}

void CMDIClient::change_Board()
{
	if (!m_bb)	return;
	m_bb->fit();

	CRect	wRc, cRc;
	CPoint	point;

	GetClientRect(cRc);
	m_bb->GetWindowRect(wRc);

	if (cRc.Width() < wRc.Width())		point.x = 0;
	else	point.x = (cRc.Width() - wRc.Width())/2;

	if (cRc.Height() < wRc.Height())	point.y = 0;
	else	point.y = (cRc.Height() - wRc.Height())/2;

	int	nFormat = SWP_NOACTIVATE|SWP_NOSIZE;
	if (wRc.Width() <= 100 || wRc.Height() <= 100)
		m_bb->SetWindowPos(&wndBottom, point.x, point.y, 0, 0, SWP_HIDEWINDOW);
	else	m_bb->SetWindowPos(&wndBottom, point.x, point.y, 0, 0, nFormat);
	
	Invalidate(FALSE);
}

void CMDIClient::Toggle()
{
	if (!m_bb)	return;
	m_bb->Toggle();
}

BOOL CMDIClient::GetReload(int id)
{
	return m_bb->GetReload(id);
}

void CMDIClient::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	change_Board();
}

void CMDIClient::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!m_bb)
	{
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	CString		file;
	HBITMAP		hBitmap;
	file.Format("%s\\image\\%s.bmp", m_home, "intro_hide");
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			  IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); 

	if (hBitmap)
	{
		CRect	cRc, bRc;
		BITMAP  bm;
		(CBitmap::FromHandle(hBitmap))->GetBitmap(&bm);
		DeleteObject(hBitmap);
		
		if (m_bb->GetStyle() & WS_VISIBLE)
		{
			m_bb->GetWindowRect(cRc);
			ScreenToClient(cRc);

			bRc.SetRect(cRc.right - (bm.bmWidth+BTN_GAP), cRc.top - bm.bmHeight, 
				cRc.right - BTN_GAP, cRc.top);
		}
		else
		{
			GetClientRect(cRc);
			bRc.SetRect(cRc.right - (bm.bmWidth + BTN_GAP), cRc.top + BTN_GAP,
				cRc.right - BTN_GAP, cRc.top + BTN_GAP + bm.bmHeight);
		}
		
		if (bRc.PtInRect(point))
		{
			if (m_bb->GetStyle() & WS_VISIBLE)
			{
				m_bb->ShowWindow(SW_HIDE);
				Invalidate(FALSE);
			}
			else
			{
				(*m_axiscall)(AXI_RELOADBB, 0, 0);
				m_bb->ShowWindow(SW_SHOW);
				Invalidate(FALSE);
			}
		}
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CMDIClient::ShowBB()
{
	if (m_bb->GetStyle() & WS_VISIBLE)
	{
		m_bb->ShowWindow(SW_HIDE);
		Invalidate(FALSE);
	}
	else
	{
		(*m_axiscall)(AXI_RELOADBB, 0, 0);
		m_bb->ShowWindow(SW_SHOW);
		Invalidate(FALSE);
	}
}

BOOL CMDIClient::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		if (GetKeyState(VK_SHIFT) & 0x8000)
			m_shiftKey = true;
		if (GetKeyState(VK_CONTROL) & 0x8000)
		{
			switch (pMsg->wParam)
			{
			case VK_TAB:
			case VK_F6:
				(*m_axiscall)(AXI_HOOKTAB, m_ctrlKey, m_shiftKey ? 1 : 0);
				m_ctrlKey++;
				return TRUE;
			default:
				break;
			}
		}
		break;
	case WM_KEYUP:
		if (!(GetKeyState(VK_CONTROL) & 0x8000) && m_ctrlKey)
			m_ctrlKey = 0;
		if (!(GetKeyState(VK_SHIFT) & 0x8000) && m_shiftKey)
			m_shiftKey = false;
		break;
	default:
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}

void CMDIClient::SetBZ()
{
	if (m_bb)
		m_bb->SetWindowPos(&wndBottom, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
}

void CMDIClient::SetLastTime(CString time)
{
	if (m_bb)
		m_bb->SetLastTime(time);
}