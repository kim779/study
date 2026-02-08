// MDIClient.cpp : implementation file
//

#include "stdafx.h"
#include "MDIClient.h"
#include "Windows.h"

#include "../../axis/axmsg.hxx"
#include "../../h/axisvar.h"


#define	BTN_GAP		5
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	BKMODE_NORMAL	0
#define	BKMODE_CHANGE	1

/////////////////////////////////////////////////////////////////////////////
// CMDIClient

CMDIClient::CMDIClient(bool (*axiscall)(int, WPARAM, LPARAM), COLORREF color)
{
	m_axiscall	= axiscall;
	m_bk		= color;
	m_pBkMapWnd	= NULL;
	m_nBkMode	= BKMODE_NORMAL;

	m_ctrlKey = 0;
	m_shiftKey = 0;
	m_edgeBmp[0].LoadBitmap(IDB_BK_LT);
	m_edgeBmp[1].LoadBitmap(IDB_BK_RT);
	m_edgeBmp[2].LoadBitmap(IDB_BK_LB);
	m_edgeBmp[3].LoadBitmap(IDB_BK_RB);

	LoadConfig();
}

CMDIClient::~CMDIClient()
{
	for (int ii = 0; ii < 4; ii++)
		m_edgeBmp[ii].DeleteObject();
}


BEGIN_MESSAGE_MAP(CMDIClient, CWnd)
	//{{AFX_MSG_MAP(CMDIClient)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_NCCALCSIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMDIClient message handlers

void CMDIClient::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC	 memDC;
	CRect	 cRc;
	GetClientRect(cRc);

	if (memDC.CreateCompatibleDC(&dc))
	{
		CBitmap	bitmap;
		bitmap.CreateCompatibleBitmap(&dc, cRc.Width(), cRc.Height());
		CBitmap* pbmp = (CBitmap *) memDC.SelectObject(&bitmap);
	

		if (!m_drawBackColor && !m_drawBackImage)
		{
			draw_background(&memDC);
		}
		else
		{
			if (m_drawBackColor)
				memDC.FillSolidRect(cRc, m_colorBackground);

			if (m_drawBackImage)
				m_picture.Draw(memDC.m_hDC, cRc, m_backImagePos);
		}


		dc.BitBlt(0, 0, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pbmp);

		memDC.DeleteDC();
		bitmap.DeleteObject();
	}
	else
	{
		dc.SetBkColor(m_bk);
		dc.ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, &cRc, "", 0, NULL);
	}
}

void CMDIClient::draw_background(CDC *pDC)
{
	CBitmap* bmpBackground = Axis::GetSkinBitmap("BG");
	CBitmap* bmpBgPattern  = Axis::GetSkinBitmap("BG_PATERN");

	CRect rc;
	GetClientRect(rc);

	if (!bmpBackground || !bmpBgPattern)
	{
		pDC->SetBkColor(m_bk);
		pDC->ExtTextOut(rc.left, rc.top, ETO_OPAQUE, &rc, "", 0, NULL);
		return;
	}

	BITMAP bm;
	bmpBackground->GetBitmap(&bm);

	CDC mdc;
	mdc.CreateCompatibleDC(pDC);

	CBitmap* oldBmp = mdc.SelectObject(bmpBgPattern);
	BITMAP bmPattern;
	bmpBgPattern->GetBitmap(&bmPattern);

	pDC->StretchBlt(rc.left, rc.bottom - bmPattern.bmHeight, rc.Width(), bmPattern.bmHeight,
		&mdc, 0, 0, bmPattern.bmWidth, bmPattern.bmHeight, SRCCOPY);
	
	if( m_iDrawStartScreen )
	{	
		mdc.SelectObject(bmpBackground);
		pDC->BitBlt(rc.right - bm.bmWidth, rc.bottom - bm.bmHeight, bm.bmWidth, bm.bmHeight, 
				&mdc, 0, 0, SRCCOPY);
	}

	if (bmPattern.bmHeight < rc.Height())
	{
		mdc.SelectObject(bmpBgPattern);
		CBrush br(mdc.GetPixel(0, 0));
		pDC->FillRect(CRect(rc.left, rc.top, rc.right, rc.bottom - rc.top - bmPattern.bmHeight),
			&br);
	}
	mdc.SelectObject(oldBmp);
	/*
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, (COLORREF)Axis::GetColor(4));
	CPen* oldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(rc.left, rc.top);
	pDC->LineTo(rc.right, rc.top);
	pDC->MoveTo(rc.left, rc.bottom - 1);
	pDC->LineTo(rc.right, rc.bottom - 1);
	pDC->SelectObject(oldPen);
	*/
}

void CMDIClient::change_Color(CString img, COLORREF color)
{
	m_bk = color;
	m_img = img;

	LoadConfig();

	SetFocus();
	Invalidate(FALSE);
}

BOOL CMDIClient::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
	case WM_KEYUP:
		m_ctrlKey = m_shiftKey = 0;
		break;
	case WM_KEYDOWN:
		{
		//**case WM_SYSKEYDOWN:	// for F10
			if (GetKeyState(VK_CONTROL) & 0x8000)
				m_ctrlKey = 1;
			if (GetKeyState(VK_SHIFT) & 0x8000)
				m_shiftKey = 1;
			
			/**
			switch (pMsg->wParam)
			{
			case VK_F1:case VK_F2:case VK_F3:case VK_F4:case VK_F5:case VK_F6:
			case VK_F7:case VK_F8:case VK_F9:case VK_F10:case VK_F11:case VK_F12:
				if (m_ctrlKey ^ m_shiftKey)
				{
					if ((*m_axiscall)(AXI_HOTKEY, pMsg->wParam, MAKELONG(m_shiftKey, m_ctrlKey)))
						return TRUE;
				}
				break;
			case VK_TAB:
				if (m_ctrlKey)
				{
					(*m_axiscall)(AXI_MDINEXT, 0, 0);
					return TRUE;
				}
				break;
			default:
				break;
			}
			break;
			**/
			if (m_shiftKey)			break;
			if (m_ctrlKey && m_shiftKey)	break;
			if (!m_ctrlKey)			break;
// 			CString s;
// 			s.Format("KEY WPARAM : %X\n",pMsg->wParam);
// 			OutputDebugString(s);
			if ((pMsg->wParam >= 0x3a && pMsg->wParam <= 0x5a) || pMsg->wParam == VK_ADD || pMsg->wParam == VK_MULTIPLY || pMsg->wParam == VK_DIVIDE || pMsg->wParam == VK_SUBTRACT)
			{
				CString map;

				if(pMsg->wParam == VK_ADD)
				{
					map = getHotCode(0x2b);
				}
				else if(pMsg->wParam == VK_MULTIPLY)
				{
					map = getHotCode(0x2a);
				}
				else if(pMsg->wParam == VK_DIVIDE)
				{
					map = getHotCode(0x2f);
				}
				else if(pMsg->wParam == VK_SUBTRACT)
				{
					map = getHotCode(0x2d);
				}
				else
				{
					map = getHotCode(pMsg->wParam);
				}

				map.TrimLeft();		map.TrimRight();
				if (!map.IsEmpty())    
					(*m_axiscall)(AXI_HOTKEY, pMsg->wParam, (LPARAM)(const char*)map);

				m_ctrlKey = m_shiftKey = 0;
				return TRUE;
			}
		}
		break;		
	case WM_CHAR:
		if (GetKeyState(VK_MENU) & 0x8000)
			TRACE("ALT KEY");
		break;
	default:
		break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}

CString CMDIClient::getHotCode(int key)
{
	CProfile profile(pkHotKey);
	const int state = profile.GetInt(szHKey, "state");
	if (!state)
		return "";

	CString keys, code;
	keys.Format("%c", key);
	keys.MakeUpper();

	code = profile.GetString(szHKey, keys);
	if  (code.GetLength() > 4)
		code = code.Left(4);
	return code;
}

void CMDIClient::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	Invalidate(FALSE);	
	if (m_pBkMapWnd)
	{
		CRect rc;
		m_pBkMapWnd->GetClientRect(&rc);
		if (m_nBkMode == BKMODE_NORMAL)
		{
			if (rc.Width() < cx)
				rc.OffsetRect((cx - rc.Width()) / 2, 0);
			
			if (rc.Height() < cy)
				rc.OffsetRect(0, (cy - rc.Height()) / 2);
		}
		else if (m_nBkMode == BKMODE_CHANGE)
		{
			rc.OffsetRect(cx - rc.Width() - 5, 5);
		}

		m_pBkMapWnd->MoveWindow(rc);
	}
}

BOOL CMDIClient::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
	return CWnd::OnEraseBkgnd(pDC);
}

void CMDIClient::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);

	const int revise = 2;
	lpncsp->rgrc[0].top  -= revise;
	lpncsp->rgrc[0].left -= revise;
	lpncsp->rgrc[0].right += revise;
	lpncsp->rgrc[0].bottom += revise;
}

void CMDIClient::DrawBitmapByMask(CDC *pDC, CBitmap *pBitmap, CPoint pos, COLORREF maskcolor)
{
	BITMAP		bm;
	pBitmap->GetBitmap(&bm);
	CDC		buffDC, maskDC, memoryDC, copyDC;
	CBitmap		buffBitmap, maskBitmap, copyBitmap;
	CBitmap* oldbuffBitmap{}, * oldmaskBitmap{}, * oldmemoryBitmap{}, * oldcopyBitmap{};

	buffDC.CreateCompatibleDC(pDC);
	buffBitmap.CreateCompatibleBitmap(pDC, bm.bmWidth, bm.bmHeight);
	oldbuffBitmap = buffDC.SelectObject(&buffBitmap);

	maskDC.CreateCompatibleDC(pDC);
	maskBitmap.CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);
	oldmaskBitmap = maskDC.SelectObject(&maskBitmap);

	const CRect		maskRc( 0, 0, bm.bmWidth, bm.bmHeight);
	buffDC.FillSolidRect(&maskRc, maskcolor);

	memoryDC.CreateCompatibleDC(pDC);
	oldmemoryBitmap = memoryDC.SelectObject(pBitmap);
	buffDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &memoryDC, 0, 0, SRCCOPY);
	maskDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &buffDC, 0, 0, SRCCOPY);

	copyDC.CreateCompatibleDC(pDC);
	copyBitmap.CreateCompatibleBitmap(pDC, bm.bmWidth, bm.bmHeight);
	oldcopyBitmap = copyDC.SelectObject(&copyBitmap);

	copyDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, pDC,  pos.x, pos.y, SRCCOPY);
	copyDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &maskDC, 0, 0, SRCAND);
	buffDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &maskDC, 0, 0, SRCMASK);
	copyDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &buffDC, 0, 0, SRCPAINT);
	pDC->BitBlt(pos.x, pos.y, bm.bmWidth, bm.bmHeight, &copyDC, 0, 0, SRCCOPY);

	copyDC.SelectObject(oldcopyBitmap);
	memoryDC.SelectObject(oldmemoryBitmap);
	maskDC.SelectObject(oldmaskBitmap);
	buffDC.SelectObject(oldbuffBitmap);

	buffDC.DeleteDC();
	maskDC.DeleteDC();
	memoryDC.DeleteDC();
	copyDC.DeleteDC();
}

void CMDIClient::LoadConfig()
{
	CProfile profile(pkEnvironment);

	const char* section = "Image";

	m_bitmapFileBackground = profile.GetString(section, "bkFile");
	Trim(m_bitmapFileBackground);

	m_drawBackImage = profile.GetInt(section, "UseImage");
	m_drawBackColor = profile.GetInt(section, "UseColor");
	m_backImagePos  = profile.GetInt(section, "posImage");
	m_colorBackground = profile.GetInt(section, "bkColor");

	if (!m_bitmapFileBackground.IsEmpty() && m_drawBackImage)
	{
		if (!m_picture.Load(m_bitmapFileBackground))
			m_drawBackImage = FALSE;
	}

	CString file;
	file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, "axisensetup.ini");
	int iRet = GetPrivateProfileInt("SCREEN", "STARTSCREEN", 1, file ) ;	

	//바닥에 초기공지 그리는거 제외.다시 구현시 axiscp,axis도 함꼐 수정해야함.
	iRet = 0;

	if( iRet > 0 )
	{
		m_iDrawStartScreen = 0;
	}
	else
		m_iDrawStartScreen = 1;
	
}
