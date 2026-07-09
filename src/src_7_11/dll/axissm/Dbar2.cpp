// Dbar2.cpp : implementation file
//

#include "stdafx.h"
#include "Dbar2.h"
#include "SVband.h"
#include "tmenu.h"
#include "resource.h"
#include "MenuXP.h"
#include "smdefine.h"
#include "nbutton.h"
#include "../../h/axisvar.h"
#include "../../axis/axMsg.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	ID_TOOLTIP	1
#define	GAP		3
#define	COLOR_CTRLBARBK	RGB(240, 240, 240)


#define LT_COLOR	RGB(171, 168, 153)
#define RB_COLOR	RGB(115, 113, 100)

#define	COLOR_INACTIVETEXT	RGB(45, 54, 139)	//RGB(116, 116, 116)
#define	COLOR_ACTIVETEXT	RGB(254, 254, 252)

#define	EXPANDWIDTH	13

#define	BUTTONMAX	4
#define SHOW_ALL	0x00
#define HIDE_FONT	0x01
#define HIDE_HELP	0x02
#define HIDE_COPY	0x04
#define HIDE_GROUP	0x08

#define RBUTTON_CLOSE		2000
#define RBUTTON_CLOSEALL	2001

/////////////////////////////////////////////////////////////////////////////
// CDbar2

CDbar2::CDbar2(bool (*callback)(int, WPARAM, LPARAM), CString home)
{
	m_axiscall	= callback;
	m_home		= home;
	m_bk		= COLOR_CTRLBARBK;
	m_height	= LISTBAR_HEIGHT;
	
	CString	fontName;
	fontName.LoadString(STSM_FONTNAME_DEFAULT);
	m_font.CreatePointFont(90, fontName);
	m_bFont.CreateFont(	   12,                        // nHeight
				   0,                         // nWidth
				   0,                         // nEscapement
				   0,                         // nOrientation
				   FW_NORMAL,	              // nWeight
				   FALSE,                     // bItalic
				   FALSE,                     // bUnderline
				   0,                         // cStrikeOut
				   DEFAULT_CHARSET,              // nCharSet
				   OUT_DEFAULT_PRECIS,        // nOutPrecision
				   CLIP_DEFAULT_PRECIS,       // nClipPrecision
				   DEFAULT_QUALITY,           // nQuality
				   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				   "굴림");                 // lpszFacename

	m_vsN = V_SCREEN1;
	m_tipN = -1;
	m_hindex = -1;
	m_actionN = -1;
	m_mouseIN = false;
	m_defaultColor = false;
	m_close = false;
	m_hide = NULL;
	m_ToolTip = NULL;
	m_band = NULL;
	m_bandRc.SetRectEmpty();
	m_expandRc.SetRectEmpty();

	m_img[0].LoadBitmap(IDB_BAR2BUTTON);
	m_img[1].LoadBitmap(IDB_EXPAND2);
	m_img[2].LoadBitmap(IDB_BAR2BACK);

	BITMAP	bm;
	m_img[0].GetBitmap(&bm);
	m_buttonW = bm.bmWidth/2;
	m_buttonH = bm.bmHeight;

	m_deleteBtn = NULL;
}

CDbar2::~CDbar2()
{
	for (int vsN = V_SCREEN1; vsN <= V_SCREEN4; vsN++)
	{
		for (int ii = 0; ii < m_arButton[vsN].GetSize(); ii++)
			delete m_arButton[vsN].GetAt(ii);
		m_arButton[vsN].RemoveAll();
	}
	if (m_hide)	delete m_hide;
	if (m_band)	delete m_band;

	m_font.DeleteObject();
	m_bFont.DeleteObject();
	m_img[0].DeleteObject();
	m_img[1].DeleteObject();
	m_img[2].DeleteObject();
}


BEGIN_MESSAGE_MAP(CDbar2, CDialogBar)
	//{{AFX_MSG_MAP(CDbar2)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MEASUREITEM()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_XMSG, OnXMSG)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDbar2 message handlers

void CDbar2::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	draw_Back(&dc);
//	draw_Gripper(&dc);
	draw_Button();
}

CSize CDbar2::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CRect	rect;
	rect.SetRectEmpty();
	CalcInsideRect(rect, bHorz);

	CRect	client;
	GetClientRect(client);

	CSize	size;
	CRect	fRc;

	size.cy = m_height;
	size.cy += (rect.Height());

	if (IsFloating())
	{
		GetDockingFrame()->GetParent()->GetWindowRect(fRc);
		size.cx = fRc.Width();
	}
	else
	{
		GetDockingFrame()->GetWindowRect(fRc);
		size.cx = fRc.Width();
	}

	return size;
}

void CDbar2::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CCmdUI	state;

	state.m_pOther = this;
	state.m_nIndexMax = 1;
	state.m_nID = AFX_IDW_STATUS_BAR;

	if (CControlBar::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
		return;
	state.DoUpdate(pTarget, FALSE);
}

void CDbar2::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	Invalidate();
}

void CDbar2::OnLButtonDown(UINT nFlags, CPoint point) 
{
	_button* button;
	bool	 expand = false;
	if (m_expandRc.PtInRect(point))
	{
		CString		str;
		CMenuXP		menu;
		CPoint		xpoint = m_expandRc.BottomRight();

		expand = true;
		if (menu.CreatePopupMenu())
		{
			draw_Expand(NULL, MODE_DOWN);
			ClientToScreen(&xpoint);
			for (int ii = m_hindex; ii < m_arButton[m_vsN].GetSize(); ii++)
			{
				button = m_arButton[m_vsN].GetAt(ii);
				str.Format("%s", button->dispN);
				menu.AppendMenuX(MF_STRING, ii, str);
				if (button->active)	menu.CheckMenuItem(ii, MF_CHECKED);
			}
			menu.TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON, xpoint.x, xpoint.y, this);
#if 0	// for xpmenu
			menu.DestroyMenu();
#endif
			draw_Expand();
		}
	}
	
	for (int ii = 0; !expand && ii < m_arButton[m_vsN].GetSize(); ii++)
	{
		CRect	bRc = GetButtonRect(ii);
		if (ii+1 >= m_arButton[m_vsN].GetSize())	//  next item no
		{
			if (bRc.right > m_bandRc.left)
				break;
		}
		else
		{
			if (bRc.right+ EXPANDWIDTH > m_bandRc.left)
				break;
		}

		if (!bRc.PtInRect(point))	continue;
		button = m_arButton[m_vsN].GetAt(ii);
		(*m_axiscall)(AXI_CHANGEFOCUS, button->key, 0);
		break;
	}

	CDialogBar::OnLButtonDown(nFlags, point);
}

void CDbar2::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_expandRc.PtInRect(point))	return;

	for (int ii = 0; ii < m_arButton[m_vsN].GetSize(); ii++)
	{
		CRect	bRc = GetButtonRect(ii);
		if (ii+1 >= m_arButton[m_vsN].GetSize())	//  next item no
		{
			if (bRc.right > m_bandRc.left)
				break;
		}
		else
		{
			if (bRc.right+ EXPANDWIDTH > m_bandRc.left)
				break;
		}

		if (!bRc.PtInRect(point))	continue;
		
		return;
	}
	
	CDialogBar::OnLButtonDblClk(nFlags, point);
}

void CDbar2::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd*			wndUnderMouse = NULL;
	CWnd*			wndActive = this;
	TRACKMOUSEEVENT		csTME;

	CDialogBar::OnMouseMove(nFlags, point);
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
		SetInfo(lpoint);
	}
	else 	m_mouseIN = false;

}

void CDbar2::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_deleteBtn = NULL;
	for (int ii = 0; ii < m_arButton[m_vsN].GetSize(); ii++)	// false로 되어 있었음
	{
		CRect	bRc = GetButtonRect(ii);
		if (ii+1 >= m_arButton[m_vsN].GetSize())	//  next item no
		{
			if (bRc.right > m_bandRc.left)
				break;
		}
		else
		{
			if (bRc.right+ EXPANDWIDTH > m_bandRc.left)
				break;
		}

		if (!bRc.PtInRect(point))
		{
			bRc.InflateRect(1, 0);
			bRc.OffsetRect(m_buttonW, 0);
			continue;
		}

		m_deleteBtn = m_arButton[m_vsN].GetAt(ii);
		break;
	}

	CString		str, tmps;
	CMenuXP		menu;
	menu.CreatePopupMenu();
	int	index = 0;
	
	if (m_deleteBtn)
	{
		tmps.Format("%s 닫기", m_deleteBtn->tooltip);
		menu.AppendMenuX(MF_STRING, RBUTTON_CLOSE, tmps);
	}
	menu.AppendMenuX(MF_STRING, RBUTTON_CLOSEALL, "전체화면 닫기");
	CPoint	curPt;
	GetCursorPos(&curPt);
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, curPt.x, curPt.y, this);

	CDialogBar::OnRButtonDown(nFlags, point);
}

LRESULT CDbar2::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_mouseIN = false;
	return 0;
}

void CDbar2::init()
{
}

void CDbar2::make_Ctrl()
{
	InitToolTip();
	SetToolTipText("");

	CString		imgC, imgO, imgD;
	m_hide = new CNButton(CTRL_HIDE);
	if (!m_hide->Create(NULL, "숨기기", WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_hide;
		m_hide = NULL;
	}
	else
	{
		m_hide->set_ToolTipText("숨기기");
		imgC.Format("%s\\%s\\%s.bmp", m_home, "image", "mb_barhide");
		imgO.Format("%s\\%s\\%s.bmp", m_home, "image", "mb_barhide_en");
		imgD.Format("%s\\%s\\%s.bmp", m_home, "image", "mb_barhide_dn");
		m_hide->set_Image(imgC, imgO, imgD);
	}

	m_band = new CSVband(m_axiscall, m_home/*, IPOS_RIGHT*/);
	if (!m_band->Create(NULL, "SVBAND", WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE|WS_CHILD, CRect(0), this, -1))
	{
		delete m_band;
		m_band = NULL;
	}
	else
		m_band->make_Ctrl();
}

void CDbar2::Change_Skin(CString skinName)
{
	CString		file;
	file.Format("%s\\image\\%s_bar2button.bmp", m_home, skinName);
	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (m_img[0].GetSafeHandle() && hBitmap)
	{
		m_img[0].DeleteObject();
		m_img[0].Attach(hBitmap);
	}

	m_band->changeSkin(skinName);
	Invalidate(FALSE);
}

void CDbar2::draw_Back(CDC* pDC)
{
	CRect	cRc;
	GetClientRect(cRc);
	pDC->FillSolidRect(cRc, RGB(202,202,202));

	CPen	pen1(PS_SOLID, 1, RGB(76, 81, 90));
	CPen	pen2(PS_SOLID, 1, RGB(255, 255, 255));

	CPen	*pOldPen = pDC->SelectObject(&pen1);
	pDC->MoveTo(0, 0);
	pDC->LineTo(cRc.right, 0);
	
	pDC->SelectObject(&pen2);
	pDC->MoveTo(0, 1);
	pDC->LineTo(cRc.right, 1);

	pDC->SelectObject(pOldPen);
#if 0
	BITMAP	bm;
	m_img[2].GetBitmap(&bm);

	CDC	memDC;
	if (memDC.CreateCompatibleDC(pDC))
	{
		CBitmap* oldbuffBitmap = memDC.SelectObject(&m_img[2]);
		if (oldbuffBitmap != (CBitmap *) 0)
		{
			pDC->StretchBlt(cRc.left, cRc.top, cRc.Width(), bm.bmHeight, 
					&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			memDC.SelectObject(oldbuffBitmap);
		}
		else	memDC.DeleteDC();
	}
#endif
}

void CDbar2::draw_Gripper(CDC* pDC)
{
	CRect	gRc(get_GripperRect());
	pDC->Draw3dRect(gRc, HILIGHT, SHADOW);
}

CRect CDbar2::get_GripperRect()
{
	CRect pRect;
	GetClientRect(pRect);
	pRect.OffsetRect(-pRect.left,-pRect.top);

	pRect.DeflateRect(3, 2);	pRect.OffsetRect(0, 1);
	pRect.right = pRect.left + 9;
	return pRect;
}

void CDbar2::set_Background(COLORREF color)
{
	m_bk = color;
	m_defaultColor = false;
	if (m_band)
		m_band->set_Background(color);
	Invalidate();
}

void CDbar2::set_Barheight(int height)
{
	m_height = height;
	GetDockingFrame()->RecalcLayout();
	Invalidate();
}

void CDbar2::ToggleImage(bool start)
{
}

// info (맵이름=툴팁<[화면번호] 화면설명>;단축명)
void CDbar2::add_Button(int key, CString mapN, class CTMenu* menu)
{
	_button*	button;
	int		matchN = -1;
	for (int ii = 0; ii < m_arButton[m_vsN].GetSize(); ii++)
	{
		button = m_arButton[m_vsN].GetAt(ii);
		if (button->key != ii)	continue;
		matchN = ii;		break;
	}

	Inactive_Button();
	if (matchN == -1)
		button = new _button;
	
	button->active  = true;
	button->key     = key;
	button->mapN    = mapN;
	if (!ExceptMap(button))
	{
		button->dispN   = menu->GetSName(mapN);
		button->tooltip = menu->GetDesc(mapN);
	}

	m_arButton[m_vsN].Add(button);
	MakeTooltipString(m_vsN);

	Invalidate(FALSE);
}

void CDbar2::add_Button(int key, CString mapN, class CTMenu* menu, int vsN)
{
	_button*	button;
	int		matchN = -1;
	for (int ii = 0; ii < m_arButton[vsN].GetSize(); ii++)
	{
		button = m_arButton[vsN].GetAt(ii);
		if (button->key != ii)	continue;
		matchN = ii;		break;
	}

	Inactive_Button(vsN);
	if (matchN == -1)	button = new _button;
	
	button->active  = true;
	button->key     = key;
	button->mapN    = mapN;
	if (!ExceptMap(button))
	{
		CString file, tmps;
		char buf[1024];
		file.Format("%s\\%s\\FISMSG.INI", m_home, TABDIR);
		tmps = mapN.Mid(2,6);
		GetPrivateProfileString("mapname", tmps, "", buf, sizeof(buf), file);
		tmps.Format(_T("%s"), buf);
		tmps.TrimRight();

		button->dispN   = menu->GetSName(mapN);
		button->tooltip = menu->GetDesc(mapN);

		if(tmps.IsEmpty())
		{
			if(button->dispN == "2050")
			{
				button->dispN = _T("관심종목");
				button->tooltip = _T("관심종목설정");
			}
			else if(button->dispN == "6030")
			{
				button->dispN = _T("팝업공지");
				button->tooltip = _T("팝업공지");
			}
		}
		else
		{
			button->dispN = tmps;
			button->tooltip = tmps;
		}
	}

	m_arButton[vsN].Add(button);
	MakeTooltipString(vsN);

	Invalidate(FALSE);
}

void CDbar2::del_Button(int key, int vsN)
{
	if (vsN == -1)	vsN = m_vsN;
	if (key == -1)
	{
		for (int ii = 0; ii < m_arButton[vsN].GetSize(); ii++)
			delete m_arButton[vsN].GetAt(ii);
		m_arButton[vsN].RemoveAll();
		Invalidate();

		MakeTooltipString(vsN);

		return;
	}

	for (int ii = 0; ii < m_arButton[vsN].GetSize(); ii++)
	{
		_button* button = m_arButton[vsN].GetAt(ii);
		if (button->key != key)
			continue;
		m_arButton[vsN].RemoveAt(ii);
		delete button;
		Invalidate();
		break;
	}
	MakeTooltipString(m_vsN);
}

void CDbar2::change_Button(int key)
{
	Inactive_Button();
	for (int ii = 0; ii < m_arButton[m_vsN].GetSize(); ii++)
	{
		_button* button = m_arButton[m_vsN].GetAt(ii);
		if (button->key != key)
			continue;

		button->active = true;
		draw_Button();
		break;
	}
}

void CDbar2::change_ButtonText(int key, CString text, int vsN)
{
	if (vsN == -1)	vsN = m_vsN;
	for (int ii = 0; ii < m_arButton[vsN].GetSize(); ii++)
	{
		_button* button = m_arButton[vsN].GetAt(ii);
		if (button->key != key)
			continue;
		int pos = text.Find(']');
		if (pos != -1)	text = text.Mid(++pos);

		text.TrimLeft();
		text.TrimRight();
		if (text.GetLength() > 8)
			button->dispN = text.Left(8);
		else	button->dispN = text;
		button->tooltip = text;
		draw_Button();
		break;
	}
}

void CDbar2::change_Info(int key, CString mapN, class CTMenu* menu)
{
	for (int ii = 0; ii < m_arButton[m_vsN].GetSize(); ii++)
	{
		_button* button = m_arButton[m_vsN].GetAt(ii);
		if (button->key != key)	continue;

		button->mapN = mapN;
		if (!ExceptMap(button))
		{
			button->dispN = menu->GetSName(mapN);
			button->tooltip = menu->GetDesc(mapN);
		}

		draw_Button();
		break;
	}
}

void CDbar2::change_VirtualScreen(int virtualScreenN)
{
	m_vsN = virtualScreenN;

	for (int ii = 0; ii < m_arButton[m_vsN].GetSize(); ii++)
	{
		_button* button = m_arButton[m_vsN].GetAt(ii);
		if (!button->active)	continue;
		(*m_axiscall)(AXI_CHANGEFOCUS, button->key, 1);
		break;
	}
	m_band->change_VirtualScreen(m_vsN);

	Invalidate(FALSE);
}

void CDbar2::Inactive_Button(int vsN)
{
	if (vsN == -1)	vsN = m_vsN;
	for (int ii = 0; ii < m_arButton[vsN].GetSize(); ii++)
	{
		_button* button = m_arButton[vsN].GetAt(ii);
		button->active = false;
	}
}

void CDbar2::draw_Button(CDC* pDC)
{
	CString tmpS;
	_button*	button;
	CClientDC	dc(this);
	CFont*		font = NULL;
	if (!pDC)	pDC = &dc;
	
	move_Band();
	pDC->SetBkMode(TRANSPARENT);

	for (int ii = 0; ii < m_arButton[m_vsN].GetSize(); ii++)
	{
		CRect bRc = GetButtonRect(ii);
		if (ii+1 >= m_arButton[m_vsN].GetSize())	//  next item no
		{
			m_expandRc.SetRectEmpty();
			if (bRc.right > m_bandRc.left)
			{
				m_hindex = ii;
				draw_Expand();
				break;
			}
			m_hindex = -1;
		}
		else
		{
			if (bRc.right+ EXPANDWIDTH > m_bandRc.left)
			{
				m_hindex = ii;
				draw_Expand();
				break;
			}
		}

		button = m_arButton[m_vsN].GetAt(ii);		
		draw_Img(&dc, bRc, button->active);



		COLORREF	color;
		if (button->active)
		{
			font  = pDC->SelectObject(&m_bFont);
			color = pDC->SetTextColor(COLOR_ACTIVETEXT);
		}
		else
		{
			font  = pDC->SelectObject(&m_font);
			color = pDC->SetTextColor(COLOR_INACTIVETEXT);
		}
		
		bRc.DeflateRect(11, 2, 0, 1); 
		if (button->dispN.IsEmpty())
			ExceptMap(button);

		pDC->SetTextColor(RGB(94,96,105));
		if(button->dispN.GetLength() >= 12)
		{
			tmpS = button->dispN;
			tmpS.Replace(_T(" "), _T(""));
			int ifind = tmpS.Find(_T("("));
			if(ifind > 0)
			{
				if(ifind > 12)
					pDC->DrawText(tmpS.Left(12), bRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
				else
					pDC->DrawText(tmpS.Left(tmpS.Find(_T("("))), bRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
			}
			else
				pDC->DrawText(tmpS.Left(12), bRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
		}
		else
			pDC->DrawText(button->dispN + _T(" "), bRc, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);

		pDC->SetTextColor(color);
		pDC->SelectObject(font);
	}
}

CRect CDbar2::GetButtonRect(int index)
{
	CRect	cRc, bRc;

	GetClientRect(cRc);
	bRc.SetRect(cRc.left, cRc.top + 4, cRc.left+m_buttonW, cRc.top + 4 + m_buttonH);
	bRc.OffsetRect(15 + m_buttonW*index, 0);
	return bRc;
}

void CDbar2::draw_Img(CDC* pDC, CRect iRc, bool bActive)
{
	CDC	memDC;
	int	x = bActive ? 0 : m_buttonW;
	if (memDC.CreateCompatibleDC(pDC))
	{
		CBitmap* oldbuffBitmap = memDC.SelectObject(&m_img[0]);
		if (oldbuffBitmap != (CBitmap *) 0)
		{
			pDC->StretchBlt(iRc.left, iRc.top, m_buttonW, m_buttonH, 
					&memDC, x, 0, m_buttonW, m_buttonH, SRCCOPY);
			memDC.SelectObject(oldbuffBitmap);
		}
		memDC.DeleteDC();
	}
}

void CDbar2::draw_Expand(CDC* pDC, int mode)
{
	if (!m_img[1].GetSafeHandle())
		return;
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;
	CRect		eRc, cRc;

	BITMAP	bm;
	m_img[1].GetBitmap(&bm);
	int	x, width = bm.bmWidth/3;

	GetClientRect(cRc);
	eRc.SetRect(m_bandRc.left - (EXPANDWIDTH), cRc.top, m_bandRc.left, cRc.bottom);
	m_expandRc.CopyRect(&eRc);
	
	switch (mode)
	{
	default:
	case MODE_COOL:	x = 0;		break;
	case MODE_OVER:	x = width;	break;
	case MODE_DOWN:	x = width*2;	break;
	}

	CDC	memDC;
	if (memDC.CreateCompatibleDC(pDC))
	{
		CBitmap* oldbuffBitmap = memDC.SelectObject(&m_img[1]);
		if (oldbuffBitmap != (CBitmap *) 0)
		{
			pDC->StretchBlt(eRc.left, eRc.top, width, eRc.Height(), 
					&memDC, x, 0, width, bm.bmHeight, SRCCOPY);
			memDC.SelectObject(oldbuffBitmap);
		}
		else	memDC.DeleteDC();
	}
}

void CDbar2::SetInfo(CPoint point)
{
	if (!m_arButton[m_vsN].GetSize())	return;
	if (m_expandRc.PtInRect(point))
	{
		if (m_tipN == 9999)	return;

		m_ToolTip->SetToolRect(this, ID_TOOLTIP, m_expandRc);
		SetToolTipText("확장");
		m_tipN = 9999;		return;
	}

	int		tmpI = -1;
	_button*	button;
	
	for (int ii = 0; ii < m_arButton[m_vsN].GetSize(); ii++)
	{
		CRect	bRc = GetButtonRect(ii);
		if (ii+1 >= m_arButton[m_vsN].GetSize())	//  next item no
		{
			if (bRc.right > m_bandRc.left)
				break;
		}
		else
		{
			if (bRc.right+ EXPANDWIDTH > m_bandRc.left)
				break;
		}

		if (!bRc.PtInRect(point))	continue;
		if (ii == m_tipN)
		{
			tmpI = ii;
			break;
		}

		button = m_arButton[m_vsN].GetAt(ii);
		m_ToolTip->SetToolRect(this, ID_TOOLTIP, bRc);
		SetToolTipText(button->tooltip);
		tmpI = ii;	break;
	}

	m_tipN = tmpI;
}

void CDbar2::SetToolTipText(CString text, BOOL bActivate)
{
	if (text.IsEmpty())	return;

	InitToolTip();
	if (m_ToolTip->GetToolCount() == 0)
		m_ToolTip->AddTool(this, text, CRect(0) , ID_TOOLTIP);

	m_ToolTip->UpdateTipText(text, this, ID_TOOLTIP);
	m_ToolTip->Activate(bActivate);
}

void CDbar2::InitToolTip()
{
	if (!m_ToolTip)
	{
		m_ToolTip = new CToolTipCtrl();
		m_ToolTip->Create(this, TTS_ALWAYSTIP);
		m_ToolTip->Activate(TRUE);
	}
}

void CDbar2::move_Band()
{
	if (m_hide)
	{
		m_hide->MoveWindow(get_GripperRect());
	}

	if (m_band)
	{
		//CSize	size = m_band->GetBandWidth();
		CRect	cRc;

		GetClientRect(cRc);
		m_bandRc.right  = cRc.right;
		m_bandRc.left   = cRc.right;
		m_bandRc.top    = cRc.top + 2 + 1;
		m_bandRc.bottom = cRc.bottom - 1;
		m_bandRc.OffsetRect(-GAP, 0);
		//m_band->MoveWindow(m_bandRc);
	}
}

_button* CDbar2::GetActiveButton()
{
	_button*	button;
	for (int ii = 0; ii < m_arButton[m_vsN].GetSize(); ii++)
	{
		button = m_arButton[m_vsN].GetAt(ii);
		if (!button->active)	continue;
		return button;
	}
	return NULL;
}

LRESULT CDbar2::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		{
			if (!m_ToolTip)
				break;
			MSG	msg;
			POINT	point;
			
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);

			msg.hwnd = m_hWnd;
			msg.message = message;
			msg.wParam = wParam;
			msg.lParam = lParam;
			msg.pt = point;
		
			m_ToolTip->RelayEvent(&msg);
		}
		break;
	default:
		break;
	}
	
	return CDialogBar::WindowProc(message, wParam, lParam);
}

void CDbar2::OnDestroy() 
{
	if (m_ToolTip)
	{
		if (m_ToolTip->m_hWnd)
		{
			m_ToolTip->Activate(FALSE);
			m_ToolTip->DelTool(this);
			m_ToolTip->RelayEvent(NULL);
		}
		delete m_ToolTip;
	}

	CDialogBar::OnDestroy();
}

BOOL CDbar2::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int	comm = LOWORD(wParam);
	if (comm >= 0 && comm < m_arButton[m_vsN].GetSize())
	{
		_button* button = m_arButton[m_vsN].GetAt(comm);
		if (m_close)
			(*m_axiscall)(AXI_BAR2ACTION, MAKEWPARAM(button->key, comm), lParam);//(*m_axiscall)(AXI_BAR2ACTION, button->key, 0);
		else
			(*m_axiscall)(AXI_CHANGEFOCUS, button->key, 0);
	}
	else
	{
		switch (comm)
		{
		case RBUTTON_CLOSE:
			if (m_deleteBtn)
				(*m_axiscall)(AXI_CLOSESCREEN, m_deleteBtn->key, 0);
			break;
		case RBUTTON_CLOSEALL:
			(*m_axiscall)(AXI_CLOSESCREEN, 0, 0);
			break;
		}
	}
	
	return CDialogBar::OnCommand(wParam, lParam);
}

void CDbar2::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	HMENU hMenu = AfxGetThreadState()->m_hTrackingMenu;
	CMenu	*pMenu = CMenu::FromHandle(hMenu);
	pMenu->MeasureItem(lpMeasureItemStruct);
	
	//CDialogBar::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

int CDbar2::getActiveKey()
{
	int		key = 0;
	_button*	button;
	for (int ii = 0; ii < m_arButton[m_vsN].GetSize(); ii++)
	{
		button = m_arButton[m_vsN].GetAt(ii);
		if (!button->active)	continue;
		return button->key;
	}
	return key;
}

bool CDbar2::ExceptMap(_button* button)
{
	CString	mapN = button->mapN;

	if (!mapN.CompareNoCase("TB900100"))
	{
		button->dispN   = "검색";
		button->tooltip = "HTS 검색";
		return false;
	}

	return false;
}

LRESULT CDbar2::OnXMSG(WPARAM wParam, LPARAM lParam)
{
	CString imgC, imgO, imgD;
	switch (LOWORD(wParam))
	{
	case CTRL_HIDE:
		(*m_axiscall)(AXI_HIDETICKER, 0, 5);	break;
		break;
	default:
		break;  
	}
	return 0;
}

void CDbar2::draw_Seperate(CDC* pDC, CRect rc)
{
	CRect	sRc, cRc;

	GetClientRect(cRc);
	sRc.SetRect(rc.left+1, cRc.top, rc.left+3, cRc.bottom - 2);
	pDC->Draw3dRect(sRc, GRIP_RB, GRIP_LT);
}

void CDbar2::MakeTooltipString(int vsN)
{
	_button*	button;
	CString		tips;
	for (int ii = 0; ii < m_arButton[vsN].GetSize(); ii++)
	{
		button = m_arButton[vsN].GetAt(ii);
		if (button->mapN.GetLength() >= L_MAPN)
			tips += "\n[" + button->mapN.Mid(2, 4) + "] " + button->tooltip;
		else
			tips += "\n[" + button->mapN + "] " + button->tooltip;
	}

	//m_band->SetVSToolTip(vsN, tips);
}

void CDbar2::SetSDI()
{
	m_hide->ShowWindow(SW_HIDE);
}

void CDbar2::SetMDI()
{
	m_hide->ShowWindow(SW_SHOW);
}

