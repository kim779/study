// Dbar2.cpp : implementation file
//

#include "stdafx.h"
#include "Dbar3.h"
//#include "SVband.h"
#include "tmenu.h"
#include "resource.h"
#include "MenuXP.h"
#include "smdefine.h"
#include "nbutton.h"
#include "../../h/axisvar.h"
#include "../../h/stopmsg.h"
#include "../../axis/axMsg.hxx"

#include "../../h/interMSG.h"
#include "ioformat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define HILIGHT		::GetSysColor(COLOR_BTNHILIGHT)
#define	SHADOW		RGB(155, 155, 155)

#define	ID_TOOLTIP	1
#define	GAP		2
#define	COLOR_CTRLBARBK	RGB(240, 240, 240)

#define LT_COLOR	RGB(171, 168, 153)
#define RB_COLOR	RGB(115, 113, 100)

#define	COLOR_INACTIVETEXT	RGB(116, 116, 116)
#define	COLOR_ACTIVETEXT	RGB(254, 254, 252)
#define COLOR_BACKLINE		RGB( 25,  82, 130)

#define	BUTTONMAX	4
#define SHOW_ALL	0x00
#define HIDE_FONT	0x01
#define HIDE_HELP	0x02
#define HIDE_COPY	0x04
#define HIDE_GROUP	0x08

#define	RBUTTONBASE	2000		// cej
#define RBUTTONBASE_1	2001		//** macho
#define	RBUTTONBASE2	3000		// cej
#define	RBUTTONBASE2_1	3001		// cej
#define	RBUTTONBASE3	3100		// lwj
#define	RBUTTONBASE3_1	3101		// lwj
#define	RBUTTONBASE3_2	3102		// lwj
#define	RBUTTONBASE_INTERGROUP		4000		
#define	RBUTTONBASE_INTER			4001		

#define WM_TIPLINK	WM_USER + 10

#define TIP_STOP	0x0001		// stoploss tip

/////////////////////////////////////////////////////////////////////////////
// CDbar3

CDbar3::CDbar3(bool (*callback)(int, WPARAM, LPARAM))
{
	m_axiscall	= callback;
	m_bk		= COLOR_CTRLBARBK;
	m_height	= LISTBAR_HEIGHT;
	
	CString	fontName;
	fontName.LoadString(STSM_FONTNAME_DEFAULT);
	m_font.CreatePointFont(90, fontName);

	m_bFont.CreateFont(	   12,                // nHeight
				   0,                         // nWidth
				   0,                         // nEscapement
				   0,                         // nOrientation
				   FW_SEMIBOLD,	              // nWeight
				   FALSE,                     // bItalic
				   FALSE,                     // bUnderline
				   0,                         // cStrikeOut
				   DEFAULT_CHARSET,           // nCharSet
				   OUT_DEFAULT_PRECIS,        // nOutPrecision
				   CLIP_DEFAULT_PRECIS,       // nClipPrecision
				   DEFAULT_QUALITY,           // nQuality
				   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				   "굴림");                   // lpszFacename

	m_vsN = V_SCREEN1;
	m_tipN = -1;
	m_hindex = -1;
	//m_actionN = -1;
	m_mouseIN = false;
	m_defaultColor = false;
	m_close = false;
	m_ToolTip = NULL;
	//m_band = NULL;
	//m_bandRc = CRect(0);
	m_bandRc.SetRectEmpty();
	m_expandRc.SetRectEmpty();

	m_img[0] = LoadFileBitmap("GRAY_BAR_BUTTON");
	m_img[1] = LoadFileBitmap("GRAY_expand_ticklist");
	//m_img[2] = Axis::GetSkinBitmap("BAR_BG");
	m_img[2] = LoadFileBitmap("GRAY_BAR_BG");
	m_img[3] = LoadFileBitmap("GRAY_BAR_CLOSE");
	m_img[4] = LoadFileBitmap("GRAY_BAR_CLOSE_DN");
	m_img[5] = LoadFileBitmap("GRAY_하단_BAR_BUTTON");
	m_img[6] = LoadFileBitmap("GRAY_하단_BAR_BG");

	BITMAP	bm{};
	if (m_img[0] && m_img[0]->m_hObject)
	{
		m_img[0]->GetBitmap(&bm);
		m_buttonW = bm.bmWidth / 2;
		m_buttonH = bm.bmHeight;
	}
	else
	{
		m_buttonW = 35;
		m_buttonH = 16;
	}

	if (m_img[1] && m_img[1]->m_hObject)
	{
		m_img[1]->GetBitmap(&bm);
		m_expandSize.cx = bm.bmWidth / 3;
		m_expandSize.cy = bm.bmHeight;
	}else
	{
		m_expandSize.cx = 13;
		m_expandSize.cy = 23;
	}
	
	// cej
	m_xRc.SetRectEmpty();
	m_xBtn = NULL;
	m_deleteBtn = NULL;
	m_menuID = -1;

	m_pStopPane = NULL;
	m_pClock = NULL;
	m_bMDIMode = TRUE;
	SetMDIMode(TRUE);

	//m_skin = NULL;
	m_closeAll = NULL;
	m_moveBar = NULL;
	m_expanded = FALSE;
	m_bPopup = TRUE;
}

CBitmap* CDbar3::LoadFileBitmap(const char* bmpName )
{
	HBITMAP hBitmap{};
	CString fileName;
	
	CString path(bmpName);
	path.MakeUpper();
	if (path.Find(".BMP") < 0)
		fileName.Format("%s\\image\\%s.bmp", Axis::home, bmpName);
	else
		fileName = path;
	CString s;
// 	s.Format("BITMAP LOAD : %s\n",fileName);
// 	OutputDebugString(s);
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), fileName, 
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)
	{
		//TRACE("Loaded File Bitmap NAME = %s\n", bmpName);
		TRACE("SUCCESS !!!!! = %s\n", bmpName);
		
#ifdef DF_USESTL_DBAR3
		std::unique_ptr<CBitmap> bmp = std::make_unique<CBitmap>();
#else 
		CBitmap* bmp = new CBitmap;
#endif

		bmp->Attach(hBitmap);
		
#ifdef DF_USESTL_DBAR3
		return bmp.release();
#else 
		return bmp;
#endif
		
	}
	else
	{
		//TRACE("Load Failed Bitmap NAME = %s\n", bmpName);
		TRACE("FAIL!!!!! = %s\n", bmpName);
	}
	return NULL;
}

CDbar3::~CDbar3()
{
	XMSG();
#ifndef DF_USESTL_DBAR3
	DELETE_WND(m_pStopPane);
#endif
	DELETE_WND(m_pClock);
	//DELETE_WND(m_skin);
	DELETE_WND(m_closeAll);
	DELETE_WND(m_moveBar);

	for (int vsN = V_SCREEN1; vsN <= V_SCREEN6; vsN++)
	{
		for (int ii = 0; ii < m_arButton.GetSize(); ii++)
			delete m_arButton.GetAt(ii);
		m_arButton.RemoveAll();
	}
	/*if (m_band)	
	{
		delete m_band;
		m_band = NULL;
	}*/

	m_font.DeleteObject();
	m_bFont.DeleteObject();

}


BEGIN_MESSAGE_MAP(CDbar3, CDialogBar)
	//{{AFX_MSG_MAP(CDbar3)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MEASUREITEM()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_TIPLINK, OnTipLink)
	ON_MESSAGE(WM_XMSG, OnXMSG)
	ON_MESSAGE(WM_MSG_MAINTOMAP, OnMsgFromMain)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDbar3 message handlers

void CDbar3::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	draw_Back(&dc);
	//draw_Gripper(&dc);
	draw_Button();
}

CSize CDbar3::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CRect	rect;
	rect.SetRectEmpty();
	CalcInsideRect(rect, bHorz);

	CRect	client;
	GetClientRect(client);

	CSize	size;
	CRect	fRc;

	/*size.cy = m_height;
	size.cy += (rect.Height());*/
	size.cy = 26;

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

void CDbar3::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CCmdUI	state;

	state.m_pOther = this;
	state.m_nIndexMax = 1;
	state.m_nID = AFX_IDW_STATUS_BAR;

	if (CControlBar::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
		return;
	state.DoUpdate(pTarget, FALSE);
}

void CDbar3::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	RepositionCtrls();
	Invalidate();
}

void CDbar3::OnLButtonDown(UINT nFlags, CPoint point) 
{
	_buttonHist* button{};
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
			for (int ii = m_hindex; ii < m_arButton.GetSize(); ii++)
			{
				button = m_arButton.GetAt(ii);
				str.Format("%s", button->dispN);
				menu.AppendMenuX(MF_STRING, ii, str);
				//if (button->active)	menu.CheckMenuItem(ii, MF_CHECKED);
			}
			menu.TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON, xpoint.x, xpoint.y, this);
#if 0	// for xpmenu
			menu.DestroyMenu();
#endif
			draw_Expand();
		}
	}

// 	if(!expand)
// 	{
// 		SetCapture();
// 		m_bMouseDown = TRUE;
// 	}

	CString dfind;
	for (int ii = 0; !expand && ii < m_arButton.GetSize(); ii++)
	{
		const CRect	bRc = GetButtonRect(ii);
		const CRect xRc = getCloseRect(bRc);			// cej
		if (ii+1 >= m_arButton.GetSize())	//  next item no
		{
			if (bRc.right > m_bandRc.left)
				break;
		}
		else
		{
			if (bRc.right + m_expandSize.cx > m_bandRc.left)
				break;
		}

		button = m_arButton.GetAt(ii);
// 		if (xRc.PtInRect(point))
// 		{
// 			button->state = MS_DOWN;
// 			InvalidateRect(xRc);
// 			(*m_axiscall)(AXI_DELHISTORY, (long)(const char*)button->code, 0);		// cej
// 			m_xBtn = NULL;
// 			m_xRc.SetRectEmpty();
// 			break;
// 		}
		if (!bRc.PtInRect(point))	continue;
		dfind = button->dispN;

		(*m_axiscall)(AXI_CHANGETRIGGER, (long)(const char*)button->code, 0);
		break;
	}

	CDialogBar::OnLButtonDown(nFlags, point);
}

void CDbar3::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_expandRc.PtInRect(point))	return;

	for (int ii = 0; ii < m_arButton.GetSize(); ii++)
	{
		const CRect	bRc = GetButtonRect(ii);
		if (ii+1 >= m_arButton.GetSize())	//  next item no
		{
			if (bRc.right > m_bandRc.left)
				break;
		}
		else
		{
			if (bRc.right + m_expandSize.cx > m_bandRc.left)
				break;
		}

		if (!bRc.PtInRect(point))	continue;
		
		return;
	}
	
	CDialogBar::OnLButtonDblClk(nFlags, point);
}

void CDbar3::SetPopupUse(BOOL bflag)
{
	m_bPopup = bflag;
}

void CDbar3::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bPopup == FALSE)
	{
		return;
	}

	CWnd*			wndUnderMouse = NULL;
	const CWnd*			wndActive = this;
	TRACKMOUSEEVENT		csTME{};
	_buttonHist* button{};

	CDialogBar::OnMouseMove(nFlags, point);
	const CPoint	lpoint = point;
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
		changeOver(lpoint);		
	}
	else 	
	{
		draw_Expand();
		m_mouseIN = false;
	}

	bool bflag =  false;

	for (int ii = 0;ii < m_arButton.GetSize(); ii++)
	{
		const CRect	bRc = GetButtonRect(ii);
		const CRect xRc = getCloseRect(bRc);			// cej
		if (ii+1 >= m_arButton.GetSize())	//  next item no
		{
			if (bRc.right > m_bandRc.left)
				break;
		}
		else
		{
			if (bRc.right + m_expandSize.cx > m_bandRc.left)
				break;
		}
		
		button = m_arButton.GetAt(ii);
// 		if (xRc.PtInRect(point))
// 		{
// 			m_ToolTip->SetToolRect(this, ID_TOOLTIP, bRc);
// 	 		SetToolTipText("종목 삭제");
// 		}
		
		//OutputDebugString("SM DBAR3 MOVE\n");

		if (!bRc.PtInRect(lpoint))	continue;

		CClientDC	dc(this);
		//OutputDebugString("SM DBAR3 MOVE RUN\n");

		if(m_currButton != ii)
		{
			bflag = true;
			m_currButton = ii;
		}
		else
		{
			break;
		}

		if(bflag)
		{
			Invalidate();
		}
		//drawCloseButton(&dc,xRc,button);

		RECT rc;
		rc.top = bRc.top;
		rc.left = bRc.left;
		rc.right = bRc.right;
		rc.bottom = bRc.bottom;

		if(button != NULL)
			(*m_axiscall)(AXI_SHOWMAP, (long)(const char*)button->code, (LPARAM)&rc);

		break;
	} 
}

// cej
void CDbar3::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_deleteBtn = NULL;
	for (int ii = 0; ii < m_arButton.GetSize(); ii++)	// false로 되어 있었음
	{
		CRect	bRc = GetButtonRect(ii);
		if (ii+1 >= m_arButton.GetSize())	//  next item no
		{
			if (bRc.right > m_bandRc.left)
				break;
		}
		else
		{
			if (bRc.right + m_expandSize.cx > m_bandRc.left)
				break;
		}

		if (!bRc.PtInRect(point))
		{
			bRc.InflateRect(1, 0);
			bRc.OffsetRect(m_buttonW, 0);
			continue;
		}
		//m_actionN = ii;
		m_deleteBtn = m_arButton.GetAt(ii);
		break;
	}
	
	HWND m_hMain = AfxGetMainWnd()->m_hWnd;
	CString stemp;
	CString strHwnd;
	strHwnd.Format("%d", this->m_hWnd);
	::SendMessage(m_hMain, WM_INTERMSG, MAKEWPARAM(MMSG_SEARCH_INTERGROUP, 0), (LPARAM)strHwnd.operator LPCSTR());

	CDialogBar::OnRButtonDown(nFlags, point);
}


LRESULT CDbar3::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
// 	if(m_bPopup == FALSE)
// 	{
// 		return 0;
// 	}
	m_currButton = -1;

	m_mouseIN = false;
	draw_Expand();

	(*m_axiscall)(AXI_CLOSEMAP, 0, 0); //test
	return 0;
}

void CDbar3::init()
{
}

void CDbar3::make_Ctrl()
{
	InitToolTip();
	SetToolTipText("");

	/**
	m_pClock = new CPClock();
	if (!m_pClock->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 90, 20), this, 100))
	{
		delete m_pClock;
		m_pClock = NULL;
	}
	else
	{
		m_pClock->Init();
		if (AfxGetApp()->GetProfileInt("INFORMATION", "bar_clock_visible", 1))
			m_pClock->ShowWindow(SW_SHOW);
		else
			m_pClock->ShowWindow(SW_HIDE);
		RepositionCtrls();	
	}
	**/

	//m_skin		= CreateNButton(CTRL_SKIN, "스킨선택", "THEME", FALSE);
	m_closeAll	= CreateNButton(CTRL_CLOSEALL, "종목툴바 닫기", "tk_close", FALSE);
	if (GetBarStyle() & CBRS_ALIGN_BOTTOM)
		m_moveBar   = CreateNButton(CTRL_UPDN, "상/하단으로 이동", "tk_up", FALSE);
	else
		m_moveBar   = CreateNButton(CTRL_UPDN, "상/하단으로 이동", "tk_down", FALSE);

	//AddNewTerminator(m_tlist, this);

#ifdef DF_USESTL_DBAR3
	m_pStopPane = std::make_unique<CUseStopPane>(m_axiscall);
#else 
	m_pStopPane = new CUseStopPane(m_axiscall);
#endif
	
	if (!m_pStopPane->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 100, 20), this, 100))
	{
#ifndef DF_USESTL_DBAR3
		delete m_pStopPane;
#endif
		m_pStopPane = NULL;
	}
	else
	{
		m_pStopPane->ShowWindow(SW_HIDE);
		RepositionCtrls();
	}
}

void CDbar3::Change_Skin(CString skinName)
{
	Invalidate();
	return;
}

void CDbar3::draw_Back(CDC* pDC)
{
	if (!m_img[2] || !m_img[2]->m_hObject)
		return;

	//BITMAP	bm;
	CRect cRc;
	GetWindowRect(cRc);
	cRc -= cRc.TopLeft();

// 	m_img[2]->GetBitmap(&bm);
// 
// 	CDC	memDC;
// 	if (memDC.CreateCompatibleDC(pDC))
// 	{
// 		CBitmap* oldbuffBitmap;
// 		if( GetBarStyle() & CBRS_ALIGN_TOP )
// 			oldbuffBitmap = memDC.SelectObject(m_img[2]);
// 		else
// 			oldbuffBitmap = memDC.SelectObject(m_img[6]);
// 
// 		if (oldbuffBitmap != (CBitmap *) 0)
// 		{
// 			pDC->StretchBlt(cRc.left, cRc.top, cRc.Width(), cRc.Height()/*bm.bmHeight*/, 
// 				&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
// 		}
// 		memDC.DeleteDC();
// 	}

// 	CDC	memDC;
// 
// 	if(memDC.CreateCompatibleDC(pDC))
// 	{
// 		CBrush brush;
// 		brush.CreateSolidBrush(RGB(0,0,0));
// 		memDC.FillRect(&cRc, &brush);
// 		brush.DeleteObject();
// 		memDC.DeleteDC();
// 	}

	CBrush brush;
	brush.CreateSolidBrush(RGB(250,250,251));
	pDC->FillRect(&cRc, &brush);

	brush.DeleteObject();
}

void CDbar3::draw_Gripper(CDC* pDC)
{
	const CRect	gRc(get_GripperRect());

	pDC->FillSolidRect(gRc.left, gRc.top + 1, 1, gRc.Height() - 2, GRIP_LT);
	pDC->FillSolidRect(gRc.left + 1, gRc.top, 1, 1, GRIP_LT);
	pDC->FillSolidRect(gRc.left + 2, gRc.top + 1, 1, gRc.Height() - 2, GRIP_RB);
	pDC->FillSolidRect(gRc.left + 1, gRc.bottom - 1, 1, 1, GRIP_RB);
}

CRect CDbar3::get_GripperRect()
{
	CRect pRect;
	GetClientRect(pRect);
	pRect.OffsetRect(-pRect.left,-pRect.top);

	pRect.DeflateRect(4, 4, 0, 3);
	pRect.right = pRect.left + 3;
	return pRect;
}

void CDbar3::set_Background(COLORREF color)
{
	m_bk = color;
	m_defaultColor = false;
	/*if (m_band)
		m_band->set_Background(color);*/
	Invalidate();
}

void CDbar3::set_Barheight(int height)
{
	m_height = height;
	GetDockingFrame()->RecalcLayout();
	GetDockingFrame()->Invalidate();
	//Invalidate();
}

void CDbar3::ToggleImage(bool start)
{
//	if (m_band)	m_band->ToggleImage(start);
}

// info (맵이름=툴팁<[화면번호] 화면설명>;단축명)
// void CDbar3::add_Button(int key, CString mapN, class CTMenu* menu)
// {
// 	_button*	button;
// 	int		matchN = -1;
// 	for (int ii = 0; ii < m_arButton[m_vsN].GetSize(); ii++)
// 	{
// 		button = m_arButton[m_vsN].GetAt(ii);
// 		if (button->key != ii)	continue;
// 		matchN = ii;		break;
// 	}
// 
// 	Inactive_Button();
// 	if (matchN == -1)
// 		button = new _button;
// 	
// 	button->state   = MS_NOT;
// 	button->active  = true;
// 	button->key     = key;
// 	button->mapN    = mapN;
// 	
// 	if (button->mapN != "IB820850")
// 	m_arButton[m_vsN].Add(button);
// 	Invalidate(FALSE);
// }

// void CDbar3::add_Button(int key, CString mapN, class CTMenu* menu, int vsN)
// {
// 	_button*	button;
// 	int		matchN = -1;
// 	for (int ii = 0; ii < m_arButton[vsN].GetSize(); ii++)
// 	{
// 		button = m_arButton[vsN].GetAt(ii);
// 		if (button->key != ii)	continue;
// 		matchN = ii;		break;
// 	}
// 
// 	Inactive_Button(vsN);
// 	if (matchN == -1)	button = new _button;
// 	
// 	button->state   = MS_NOT;
// 	button->active  = true;
// 	button->key     = key;
// 	button->mapN    = mapN;
// 
// 	m_arButton[vsN].Add(button);
// 	Invalidate(FALSE);
// }

void CDbar3::add_History(CString code,CString name)
{
	_buttonHist*	button;

//	Inactive_Button(vsN);
	button = new _buttonHist;
	button->state   = MS_NOT;
	button->active  = true;
	button->code     = code;
	button->dispN	= name;
	
	m_arButton.Add(button);
	Invalidate(FALSE);
}

void CDbar3::del_Button(CString code)
{
	for (int ii = 0; ii < m_arButton.GetSize(); ii++)
	{
		_buttonHist* button = m_arButton.GetAt(ii);
		if (button->code != code)
			continue;
		m_arButton.RemoveAt(ii);
		delete button;
		Invalidate();
		break;
	}
}

void CDbar3::del_AllButton()
{
	for (int ii = 0; ii < m_arButton.GetSize(); ii++)
		delete m_arButton.GetAt(ii);
	m_arButton.RemoveAll();
	Invalidate();
}

// void CDbar3::change_Button(int key)
// {
// //	Inactive_Button();
// 	for (int ii = 0; ii < m_arButton.GetSize(); ii++)
// 	{
// 		_buttonHist* button = m_arButton.GetAt(ii);
// 		if (button->key != key)
// 			continue;
// //TRACE("......match change_Button....[%d]\n", m_vsN);
// 		button->active = true;
// 		draw_Button();
// 		break;
// 	}
// }

// void CDbar3::change_ButtonText(int key, CString text, int vsN)
// {
// 	if (vsN == -1)	vsN = m_vsN;
// 	for (int ii = 0; ii < m_arButton.GetSize(); ii++)
// 	{
// 		_button* button = m_arButton.GetAt(ii);
// 		if (button->key != key)
// 			continue;
// 		int pos = text.Find(']');
// 		if (pos != -1)	text = text.Mid(++pos);
// 
// 		text.TrimLeft();
// 		text.TrimRight();
// 		if (text.GetLength() > 8)
// 			button->dispN = text.Left(8);
// 		else	button->dispN = text;
// 		button->tooltip = text;
// 		
// 		if (button->mapN.Find("IB7700")!=-1 && button->mapN != "IB770000")
// 		{
// 			button->dispN   = "바탕화면";
// 			button->tooltip = "바탕화면";
// 		}
// 		
// 		draw_Button();
// 		break;
// 	}
// }

// void CDbar3::Inactive_Button(int vsN)
// {
// 	if (vsN == -1)	vsN = m_vsN;
// 	for (int ii = 0; ii < m_arButton.GetSize(); ii++)
// 	{
// 		_button* button = m_arButton.GetAt(ii);
// 		button->active = false;
// 	}
// }

void CDbar3::draw_Button(CDC* pDC)
{
	if (m_vsN >= 6 || m_vsN < 0)
		return;
	
	_buttonHist* button{};
	CClientDC	dc(this);
	CFont*		font = NULL;
	
	if (!pDC)	
		pDC = &dc;
	
	move_Band();
	pDC->SetBkMode(TRANSPARENT);
	
	m_expanded = FALSE;
	
	
	for (int ii = 0; ii < m_arButton.GetSize(); ii++)
	{
		CRect bRc = GetButtonRect(ii);
		CRect xRc = getCloseRect(bRc);	
		
		if (ii+1 >= m_arButton.GetSize())	//  next item no
		{
			m_expandRc.SetRectEmpty();
			if (bRc.right > m_bandRc.left)
			{
				m_hindex = ii;
				m_expanded = TRUE;
				draw_Expand();
				
				break;
			}
			m_hindex = -1;
		}
		else
		{
			
			if (bRc.right + m_expandSize.cx > m_bandRc.left)
			{
				m_hindex = ii;
				m_expanded = TRUE;
				draw_Expand();
				
				break;
			}
			
		}
		
		button = m_arButton.GetAt(ii);
		
		if (!button)
			continue;
	
		draw_rect(&dc,bRc,button->active);
		
		COLORREF	color{};
		
		if (button->active)
			font = pDC->SelectObject(&m_font);
		else
			font = pDC->SelectObject(&m_font);
		color = pDC->SetTextColor(Axis::GetColor(button->active ? 13: 14));
		if (GetBarStyle() & CBRS_ALIGN_TOP)
		{
			//bRc.OffsetRect(8, 1+3); 
			bRc.OffsetRect(8, 1); 
			xRc.OffsetRect(0, 0);
		}
		else
		{
			//bRc.OffsetRect(8, -3+3); 
			bRc.OffsetRect(8, -3 + 5); 
			xRc.OffsetRect(0, 1);
			
		}

		CString name = button->dispN;

		//name = name.Left(4);

		bRc.right -= 20;
		bRc.left -= 10;

// 		if(name.Find("&",0) > -1)
// 		{
// 			int nindex = name.Find("&",0);
// 			name.Insert(nindex,"\");
// 		}
		
//		pDC->DrawText(name, bRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		pDC->DrawText(name, bRc, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
		pDC->SetTextColor(color);
		pDC->SelectObject(&font);
		
// 		if(m_currButton == ii)
// 		{
// 			xRc.left = xRc.left - 4;
// 			xRc.right = xRc.right - 4;
// 			drawCloseButton(pDC, xRc, button);	
// 		}
	}
}

void CDbar3::draw_rect(CDC *pDC, CRect iRc, bool bActive)
{
	iRc.top = iRc.top + 4;
	
//	CPen	cPen(PS_SOLID, 1, RGB(128,128,128));
	
//	const CPen *sPen = pDC->SelectObject(&cPen);
// 	pDC->MoveTo(iRc.left - 2, iRc.top);
// 	pDC->LineTo(iRc.left - 2 + m_buttonW - 4, iRc.top);
// 	pDC->MoveTo(iRc.left - 2  + m_buttonW - 4, iRc.top);
// 	pDC->LineTo(iRc.left - 2  + m_buttonW - 4, iRc.top + m_buttonH - 7);
// 	pDC->MoveTo(iRc.left - 2  + m_buttonW - 4, iRc.top + m_buttonH - 7);
// 	pDC->LineTo(iRc.left - 2 ,iRc.top + m_buttonH -7);
// 	pDC->MoveTo(iRc.left - 2 ,iRc.top + m_buttonH - 7);
// 	pDC->LineTo(iRc.left - 2 , iRc.top);

	CRect rect;
	rect.left = iRc.left - 1;
	rect.top = iRc.top - 2;
	rect.right = iRc.left - 1  + m_buttonW - 2;
	rect.bottom = iRc.top + m_buttonH - 5;

	POINT pt;
	pt.x = 5;
	pt.y = 5;

	pDC->RoundRect(rect,pt);
	
	//cPen.DeleteObject();
}

CRect CDbar3::GetButtonRect(int index)
{
	CRect	cRc, bRc;
	const int buttonW(m_buttonW);

	GetClientRect(cRc);
	bRc.SetRect(cRc.left, cRc.top, cRc.left + buttonW, cRc.top + m_buttonH);
	bRc.OffsetRect(5 + buttonW * index, 0);

	return bRc;
}

void CDbar3::draw_Img(CDC* pDC, CRect iRc, bool bActive)
{
	CDC	memDC;
	const int	x = bActive ? 0 : m_buttonW;
	if (memDC.CreateCompatibleDC(pDC))
	{
		CBitmap* oldbuffBitmap{};// = memDC.SelectObject(m_img[0]);
		if (GetBarStyle() & CBRS_ALIGN_BOTTOM)
		{
			oldbuffBitmap = memDC.SelectObject(m_img[5]);
		}else{
			oldbuffBitmap = memDC.SelectObject(m_img[0]);
		}
		if (oldbuffBitmap != (CBitmap *) 0)
		{
			if (GetBarStyle() & CBRS_ALIGN_BOTTOM)
			{
				pDC->StretchBlt(iRc.left, iRc.top, m_buttonW, m_buttonH, 
					&memDC, x, 0, m_buttonW, m_buttonH, SRCCOPY);
			}else{
				pDC->StretchBlt(iRc.left, iRc.top+1, m_buttonW, m_buttonH, 
					&memDC, x, 0, m_buttonW, m_buttonH, SRCCOPY);
			}	
			memDC.SelectObject(&oldbuffBitmap);
		}
		memDC.DeleteDC();
	}
}

void CDbar3::draw_Expand(CDC* pDC, int mode)
{
	
	if (!m_img[1] || !m_img[1]->GetSafeHandle())
	{
		return;
	}

	if (!m_expanded)
		return;
	

	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;
	CRect		eRc, cRc;

	int	x{};

	GetClientRect(cRc);
	eRc.SetRect(m_bandRc.left - m_expandSize.cx, cRc.top , m_bandRc.left, cRc.bottom);

	m_expandRc.CopyRect(&eRc);
	
	switch (mode)
	{
	default:
	case MODE_COOL:	
		x = 0;		break;
	case MODE_OVER:	
		x = m_expandSize.cx;	break;
	case MODE_DOWN:	
		x = m_expandSize.cx * 2;	break;
	}

// 	CDC	memDC;
// 	if (memDC.CreateCompatibleDC(pDC))
// 	{
// 		CBitmap* oldbuffBitmap = memDC.SelectObject(m_img[1]);
// 		if (oldbuffBitmap != (CBitmap *) 0)
// 		{
// 			if (GetBarStyle() & CBRS_ALIGN_BOTTOM)
// 			{
// 				pDC->BitBlt(eRc.left, eRc.top+2, m_expandSize.cx, m_expandSize.cy, 
// 					&memDC, x, 0, SRCCOPY);
// 			}
// 			else
// 				pDC->BitBlt(eRc.left, eRc.top+1, m_expandSize.cx, m_expandSize.cy, 
// 					&memDC, x, 0, SRCCOPY);
// 
// 			memDC.SelectObject(&oldbuffBitmap);
// 		}
// 		memDC.DeleteDC();
// 	}

	CFont fontBold;
	CFont* font{};

	LOGFONT	logFont;
	m_font.GetLogFont(&logFont);
	logFont.lfHeight = 11;
	logFont.lfWeight = FW_BOLD;
	fontBold.CreateFontIndirect(&logFont);

	font = pDC->SelectObject(&fontBold);

	pDC->SetTextColor(RGB(128,128,128));

	eRc.top += 8;
	pDC->DrawText(">>",eRc,DT_VCENTER | DT_CENTER);

	pDC->SelectObject(&font);
}

void CDbar3::SetInfo(CPoint point)
{
	if (!m_arButton.GetSize())	
		return;

	if (m_expandRc.PtInRect(point))
	{
		if (m_tipN == 9999)	
			return;

		m_ToolTip->SetToolRect(this, ID_TOOLTIP, m_expandRc);
		SetToolTipText("추가 히스토리 보기");
		m_tipN = 9999;		
		return;
	}

	int		tmpI = -1;
	//_buttonHist*	button;
	
	for (int ii = 0; ii < m_arButton.GetSize(); ii++)
	{
		const CRect	bRc = GetButtonRect(ii);
		if (ii+1 >= m_arButton.GetSize())	//  next item no
		{
			if (bRc.right > m_bandRc.left)
				break;
		}
		else
		{
			if (bRc.right + m_expandSize.cx > m_bandRc.left)
				break;
		}

		if (!bRc.PtInRect(point))	continue;
		if (ii == m_tipN)
		{
			tmpI = ii;
			break;
		}

//		button = m_arButton.GetAt(ii);
// 		m_ToolTip->SetToolRect(this, ID_TOOLTIP, bRc);
// 		SetToolTipText("종목 삭제");
		tmpI = ii;	break;
	}

	m_tipN = tmpI;
}

int CDbar3::GetMarketByCode(CString strCode)
{
	strCode.TrimLeft();
	strCode.TrimRight();
	
	int nMarketType = MarketStock;
	
	switch(strCode.GetLength()) {
	case CODELEN_INDEX:
		{
			nMarketType = MarketIndex;			    // 업종
		}
		break;
	case CODELEN_STOCK:
		{
			if ((strCode[0] == '5') || (strCode[0] == '6'))
			{
				nMarketType = MarketELW;		    // ELW
			}
			else
			{
				char *tmp;
				strtol(strCode.operator LPCTSTR(), &tmp, 10);
				
				if (NULL == *tmp)
				{
					nMarketType = MarketStock;		// 주식
				}
				else
				{
					nMarketType = MarketForeign;	// 해외
				}
			}
		}
		break;
	case CODELEN_FUTOPT:
		{
			if(strCode.Left(1) == "1" || strCode.Left(1) == "4" || strCode.Left(3) == "999")	//선물, 스프래드, 연결 선물
			{
				nMarketType = MarketFuture;
			}
			else if(strCode.Left(2) == "20" || strCode.Left(2) == "30")	//옵션(콜,풋)
			{
				nMarketType = MarketOption;
			}
			else if(strCode.Left(2) == "21" || strCode.Left(2) == "31")	//개별주식옵션(콜,풋)
			{
				nMarketType = MarketStockOption;
			}
			else 
			{
				nMarketType = MarketForeign;
			}
		}
		break;
	default:  // 해외
		{
			nMarketType = MarketForeign;
		}
		break;
	}
	
	return nMarketType;
}

void CDbar3::SetToolTipText(CString text, BOOL bActivate)
{
	if (text.IsEmpty())	return;

	InitToolTip();
	if (m_ToolTip->GetToolCount() == 0)
		m_ToolTip->AddTool(this, text, CRect(0, 0, 0, 0) , ID_TOOLTIP);

	m_ToolTip->UpdateTipText(text, this, ID_TOOLTIP);
	m_ToolTip->Activate(bActivate);
}

void CDbar3::InitToolTip()
{
	if (!m_ToolTip)
	{
#ifdef DF_USESTL_DBAR3
		m_ToolTip = std::make_unique<CToolTipCtrl>();
#else 
		m_ToolTip = new CToolTipCtrl();
#endif
		
		m_ToolTip->Create(this, TTS_ALWAYSTIP);
		m_ToolTip->Activate(TRUE);
	}
}

void CDbar3::move_Band()
{
	GetClientRect(m_bandRc);

	if (m_closeAll)
	{
		CRect rc;
		m_closeAll->GetWindowRect(&rc);
		ScreenToClient(&rc);
		m_bandRc.left = rc.left - 25;
	}
	else
		m_bandRc.left = m_bandRc.right - 80;

}

_buttonHist* CDbar3::GetActiveButton()
{
	_buttonHist* button{};
	for (int ii = 0; ii < m_arButton.GetSize(); ii++)
	{
		button = m_arButton.GetAt(ii);
		if (!button->active)	continue;
		return button;
	}
	return NULL;
}

// LRESULT CDbar3::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
// {
// 	switch (message)
// 	{
// 	case WM_LBUTTONDOWN:
// 	case WM_MOUSEMOVE:
// 	case WM_LBUTTONUP:
// 	case WM_RBUTTONDOWN:
// 	case WM_MBUTTONDOWN:
// 	case WM_RBUTTONUP:
// 	case WM_MBUTTONUP:
// 		{
// // 			if (!m_ToolTip)
// // 				break;
// // 
// // 			if(!m_ToolTip->GetSafeHwnd())
// // 			{
// // 				break;
// // 			}
// // 
// // 			MSG	msg;
// // 			POINT	point;
// // 			
// // 			point.x = LOWORD(lParam);
// // 			point.y = HIWORD(lParam);
// // 			
// // 			if(!m_hWnd)
// // 			{
// // 				break;
// // 			}
// // 			
// // 			msg.hwnd = m_hWnd;
// // 			msg.message = message;
// // 			msg.wParam = wParam;
// // 			msg.lParam = lParam;
// // 			msg.pt = point;
// // 			//OutputDebugString("WINPROC 1\n");
// // 			m_ToolTip->RelayEvent(&msg);
// 			//OutputDebugString("WINPROC 2\n");
// 		}
// 		break;
// 	default:
// 		break;
// 	}
// 	
// 	return CDialogBar::WindowProc(message, wParam, lParam);
// }

void CDbar3::OnDestroy() 
{
	XMSG();
	ClearTerminator(m_tlist);
	
	if (m_ToolTip)
	{
		if (m_ToolTip->m_hWnd)
		{
			m_ToolTip->Activate(FALSE);
			m_ToolTip->DelTool(this);
			//m_ToolTip->RelayEvent(NULL);
		}
#ifndef DF_USESTL_DBAR3
		delete m_ToolTip;
#endif
		m_ToolTip = NULL;
	}

	CDialogBar::OnDestroy();
}

BOOL CDbar3::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// cej
	const int	comm = LOWORD(wParam);

	if (comm >= 0 && comm < m_arButton.GetSize())
	{
		const _buttonHist* button = m_arButton.GetAt(comm);
		if (button)
		{
			(*m_axiscall)(AXI_CHANGETRIGGER, (long)(const char*)button->code, 0);
		}
	}
	else
	{
		switch (comm)
		{
		case RBUTTONBASE:	// 닫기
			if (m_deleteBtn)
				(*m_axiscall)(AXI_DELHISTORY, (long)(const char*)m_deleteBtn->code, 0);
			break;
// 		case RBUTTONBASE_1: // 선택창 제외하고 모두닫기
// 			if (m_deleteBtn)
// 				(*m_axiscall)(AXI_CLOSECHILD, m_deleteBtn->key, 1);
// 			break;
		case RBUTTONBASE2:	// 현재스크린 모두닫기
			//OutputDebugString("히스토리 모두 삭제\n");
			(*m_axiscall)(AXI_DELHISTORYALL, 0, 0);
			break;
// 		case RBUTTONBASE2_1: // 모든 창 최소화
// 			(*m_axiscall)(AXI_MINIMIZESCREEN, 0, 0);
// 			break;
// 		case RBUTTONBASE3:	// 시계 보이기/감추기
// 			if (m_pClock)
// 			{
// 				m_pClock->ShowWindow(m_pClock->IsWindowVisible()?SW_HIDE:SW_SHOW);
// 				RepositionCtrls();
// 				AfxGetApp()->WriteProfileInt("INFORMATION", "bar_clock_visible", m_pClock->IsWindowVisible());
// 			}
// 			break;
// 		case RBUTTONBASE3_1:	// 스탑로스 보이기/감추기
// 			if (m_pStopPane)
// 			{
// 				m_pStopPane->ShowWindow(SW_HIDE);
// 				
// 				m_pStopPane->ShowWindow(m_pStopPane->IsWindowVisible()?SW_HIDE:SW_SHOW);
// 				RepositionCtrls();
// 				AfxGetApp()->WriteProfileInt("INFORMATION", "bar_stop_visible", m_pStopPane->IsWindowVisible());
// 				
// 			}
// 			break;
		case RBUTTONBASE3_2:	// 상단이동/ 하단이동
			{
				MoveBar(GetBarStyle() & CBRS_ALIGN_TOP);
			}
			break;
		}
	}
	return CDialogBar::OnCommand(wParam, lParam);
}

void CDbar3::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	HMENU hMenu = AfxGetThreadState()->m_hTrackingMenu;
	CMenu	*pMenu = CMenu::FromHandle(hMenu);
	pMenu->MeasureItem(lpMeasureItemStruct);
	
	//CDialogBar::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

CString CDbar3::getActiveKey()
{
	const int		key = 0;
	_buttonHist* button{};
	for (int ii = 0; ii < m_arButton.GetSize(); ii++)
	{
		button = m_arButton.GetAt(ii);
		if (!button->active)	continue;
		return button->code;
	}
	return "";  //vc2019
}

void CDbar3::changeOver(CPoint pt)
{
	if (!m_arButton.GetSize())	return;

	if (m_expandRc.PtInRect(pt)) {
		draw_Expand(NULL, MODE_OVER);
		return;
	}
	draw_Expand();

	_buttonHist* button{};
	CRect		xRc;

	for (int ii = 0; ii < m_arButton.GetSize(); ii++)
	{
		const CRect	bRc = GetButtonRect(ii);
		if (ii+1 >= m_arButton.GetSize())	//  next item no
		{
			if (bRc.right > m_bandRc.left)
				break;
		}
		else
		{
			if (bRc.right + m_expandSize.cx > m_bandRc.left)
			{
				break;
			}
		}

		button = m_arButton.GetAt(ii);
		xRc = getCloseRect(bRc);
		if (xRc.PtInRect(pt))
		{
			button->state = MS_OVER;	
			if (xRc != m_xRc)
			{
				if (m_xBtn)
				{
					m_xBtn->state = MS_NOT;
					InvalidateRect(m_xRc);
				}

				m_xBtn = button;
				m_xRc = xRc;
				InvalidateRect(xRc);
			}
			break;
		}
		else
		{
			if (m_xBtn && !m_xRc.IsRectEmpty())
			{
				if (AfxIsValidAddress(m_xBtn, sizeof(_button)))
					m_xBtn->state = MS_NOT;
				InvalidateRect(m_xRc);

				m_xRc.SetRectEmpty();
				m_xBtn = NULL;
			}

		}
	}
}

CRect CDbar3::getCloseRect(CRect bRc)
{
	if (!m_img[3] || !m_img[3]->m_hObject)
		return CRect(0, 0, 0, 0);

	CRect xRc;
	BITMAP bm;
	m_img[3]->GetBitmap(&bm);

	xRc.SetRect(bRc.right - bm.bmWidth, bRc.top, bRc.right, bRc.top + bm.bmHeight);
	xRc.OffsetRect(-7, 7);
	return xRc;
}


void CDbar3::drawCloseButton(CDC* pDC, CRect xRc, _buttonHist *button)
{
	if (!m_img[3] || !m_img[3]->m_hObject)
		return;
	if (!m_img[4] || !m_img[4]->m_hObject)
		return;

	//if (!button->active)
	//	return;

	CBitmap* bmp{};

	if (button->state == MS_OVER || button->state == MS_DOWN)
		bmp = m_img[4];
	else
		bmp = m_img[3];

	BITMAP bm;
	bmp->GetBitmap(&bm);

	DrawMaskedBitmap(pDC, xRc.left, xRc.top, bmp, RGB(247,247,247), bm.bmWidth, bm.bmHeight);
}

void CDbar3::SetMDIMode(BOOL bMDI)
{
	if (m_bMDIMode != bMDI)
	{
		m_bMDIMode = bMDI;
		ImageToggle();
		RepositionCtrls();
	}
}

void CDbar3::DrawGradient(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight)
{
	CRect	rect;
	int	nWidth{}, nHeight{};

	drawRC.right += 1;
	drawRC.bottom += 1;
	nWidth = drawRC.Width(); 
	nHeight = drawRC.Height();

	int rs{}, gs{}, bs{};
	rs = GetRValue(sColor);
	gs = GetGValue(sColor);
	bs = GetBValue(sColor);

	int re{}, ge{}, be{};
	re = GetRValue(eColor);
	ge = GetGValue(eColor);
	be = GetBValue(eColor);

	float	rStep{}, gStep{}, bStep{};
	CPen* sPen{};
	if (bRight)
	{
		if (!nWidth)
		{
			CPen	cPen(PS_SOLID, 1, sColor);
			sPen = pDC->SelectObject(&cPen);
			
			pDC->MoveTo(drawRC.left, drawRC.top);
			pDC->LineTo(drawRC.right, drawRC.bottom);
			pDC->SelectObject(&sPen);
			return;
		}

		rStep = float(rs - re)/float(nWidth);
		gStep = float(gs - ge)/float(nWidth);
		bStep = float(bs - be)/float(nWidth);

		if (!nHeight)
		{
			for (int ii = 0; ii < nWidth; ++ii)
			{
				CPen	cPen(PS_SOLID, 1, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				sPen = pDC->SelectObject(&cPen);

				pDC->MoveTo(drawRC.left + ii, drawRC.top);
				pDC->LineTo(drawRC.left + ii + 1, drawRC.top);
				pDC->SelectObject(&sPen);
			}
		}
		else
		{
			for (int ii = 0; ii < nWidth; ++ii)
			{
				rect.SetRect(drawRC.left + ii, drawRC.top, drawRC.left + ii + 1, drawRC.bottom);
				
				CBrush brush;
				brush.CreateSolidBrush(RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				pDC->FillRect(&rect, &brush);
				brush.DeleteObject();
			}
		}
	}
	else
	{
		if (!nHeight)
		{
			CPen	cPen(PS_SOLID, 1, sColor);
			sPen = pDC->SelectObject(&cPen);
			
			pDC->MoveTo(drawRC.left, drawRC.top);
			pDC->LineTo(drawRC.right, drawRC.bottom);
			pDC->SelectObject(&sPen);
			return;
		}

		rStep = float(rs - re)/float(nHeight);
		gStep = float(gs - ge)/float(nHeight);
		bStep = float(bs - be)/float(nHeight);

		if (!nWidth)
		{
			for (int ii = 0; ii < nHeight; ++ii)
			{
				CPen	cPen(PS_SOLID, 1, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				sPen = pDC->SelectObject(&cPen);

				pDC->MoveTo(drawRC.left, drawRC.top + ii);
				pDC->LineTo(drawRC.left, drawRC.top + ii + 1);
				pDC->SelectObject(&sPen);
			}
		}
		else
		{
			for (int ii = 0; ii < nHeight; ++ii)
			{
				rect.SetRect(drawRC.left, drawRC.top + ii, drawRC.right, drawRC.top + ii + 1);
				
				CBrush brush;
				brush.CreateSolidBrush(RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				pDC->FillRect(&rect, &brush);
				brush.DeleteObject();
			}
		}
	}
}

void CDbar3::RepositionCtrls()
{
	CRect cRc;
	GetClientRect(&cRc);
	cRc.DeflateRect(0, 3, 5, 2);	//gap

	CRect rc;
	if (m_pClock/* && m_pClock->IsWindowVisible()*/)
	{
		CRect itemRc;
		m_pClock->GetClientRect(&itemRc);
		
		cRc.left = cRc.right - itemRc.Width();
		m_pClock->MoveWindow(cRc);
	}

	if (m_pClock && m_pClock->IsWindowVisible())
	{
		cRc.right = cRc.left - 3;
	}

	if (m_pStopPane)
	{
		CRect itemRc;
		m_pStopPane->GetClientRect(&itemRc);
		
		cRc.left = cRc.right - itemRc.Width();
		m_pStopPane->MoveWindow(cRc);
		
	}

	int xright = cRc.right + 2;
	/*
	if (m_skin)
	{
		m_skin->GetClientRect(rc);
		if (!rc.Width())
		{
			rc.right = 42;
			rc.bottom = 13;
		}
		m_skin->MoveWindow(xright - rc.Width()+1 , cRc.top +1, rc.Width(), rc.Height());
		xright -= rc.Width();
	}
	*/

	if (m_closeAll)
	{
		xright --;
		m_closeAll->GetClientRect(rc);
		if (!rc.Width())
		{
			rc.right = rc.bottom = 15;
		}
		if( GetBarStyle() & CBRS_ALIGN_BOTTOM )		
			m_closeAll->MoveWindow(xright - rc.Width(), cRc.top+4, rc.Width(), rc.Height());
		else
			m_closeAll->MoveWindow(xright - rc.Width(), cRc.top+1, rc.Width(), rc.Height());
		xright -= rc.Width();
	}

	if (m_moveBar)
	{
		xright--;
		m_moveBar->GetClientRect(rc);
		if (!rc.Width())
		{
			rc.right = rc.bottom = 15;
		}

		if( GetBarStyle() & CBRS_ALIGN_BOTTOM )		
			m_moveBar->MoveWindow(xright - rc.Width(), cRc.top+4, rc.Width(), rc.Height());
		else
			m_moveBar->MoveWindow(xright - rc.Width(), cRc.top+1, rc.Width(), rc.Height());
		
		
		xright -= rc.Width();
	}
	/*
	if (m_skin)
	{
		CTerminator* t = m_tlist.GetHead();

		if (t)
		{
			xright -= 3;
			t->MoveWindow(xright - default_width, cRc.top, xright, default_height);
			xright -= default_width;
		}
	}
	*/
	draw_Expand();
}

void CDbar3::ImageToggle()
{
	if (((GetBarStyle() & CBRS_ALIGN_BOTTOM) && !m_bBottom) || 
		((GetBarStyle() & CBRS_ALIGN_TOP) && m_bBottom))
	{
		/**
		if (!m_img[0]->FlipImage(FALSE, TRUE))
			TRACE("Image Rotate Error\n");
		if (!m_img[2]->FlipImage(FALSE, TRUE))
			TRACE("Image Rotate Error\n");
		**/
		m_bBottom = !m_bBottom;
		Invalidate();
	}
}

int CDbar3::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_bBottom = TRUE;
	if (!(GetBarStyle() & CBRS_ALIGN_BOTTOM))
		ImageToggle();

	for (int ii = 0; ii < 2; ii++)
	{
	//	m_tip[ii].Create(this, TRUE);
	//	m_tip[ii].SetCallbackHyperlink(GetSafeHwnd(), WM_TIPLINK);
	}
	
	return 0;
}

void CDbar3::ShowInformation()
{
	return;
	POINT pos;
	CRect rc;
	GetWindowRect(&rc);
	pos.x = rc.left;
	pos.y = rc.top;

	if (!AfxGetApp()->GetProfileInt("INFORMATION", "listbartip_msg", 0))
	{
/*
		if (GetBarStyle() & CBRS_ALIGN_BOTTOM)
			m_tip[0].ShowHelpTooltip(&pos, "<b>화면목록바</b>를 상단 혹은 하단으로 이동하실 수 있습니다.\n<b>마우스 우클릭메뉴</b>로도 변경가능합니다.\n<a msg=\"top\">[상단으로 이동하기]</a>&nbsp;<a msg=\"flag\">[팁 그만 보기]</a>");
		else
			m_tip[0].ShowHelpTooltip(&pos, "<b>화면목록바</b>를 상단 혹은 하단으로 이동하실 수 있습니다.\n<b>마우스 우클릭메뉴</b>로도 변경가능합니다.\n<a msg=\"bottom\">[하단으로 이동하기]</a>&nbsp;<a msg=\"flag\">[팁 그만 보기]</a>");
*/
	}

	/*if (m_pStopPane && m_pStopPane->IsWindowVisible() && 
		!(AfxGetApp()->GetProfileInt("INFORMATION", "tip_msg", 0) & TIP_
		STOP))
	{
		pos.x = rc.right;
		CRect itemRc;
		if (m_pStopPane && m_pStopPane->IsWindowVisible())
		{
			m_pStopPane->GetClientRect(&itemRc);
			pos.x -= itemRc.Width();
		}
		
		if (m_pClock && m_pClock->IsWindowVisible())
		{
			
			m_pClock->GetClientRect(&itemRc);
			pos.x -= itemRc.Width();
		}

		m_tip[1].ShowHelpTooltip(&pos, "<b>스탑로스</b> 화면을 띄우거나 사용 여부를 확인할 수 있습니다.\n※ 시세전용에서는 활성화되지 않습니다.\n<a msg=\"stop\">[사용안함/감추기]</a>&nbsp;<a msg=\"tip_flag0001\">[팁 그만보기]</a>");
	}*/
}

LRESULT CDbar3::OnTipLink(WPARAM wParam, LPARAM lParam)
{
	if (wParam)
	{
		CString sKey = (LPSTR)wParam;
		if (!sKey.CompareNoCase("top"))
		{
			MoveBar(MB_TOP);
		}
		else if (!sKey.CompareNoCase("bottom"))
		{
			MoveBar(MB_BOTTOM);
		}
		else if (!sKey.CompareNoCase("stop"))
		{
			if (m_pStopPane)
			{
				m_pStopPane->ShowWindow(SW_HIDE);
				RepositionCtrls();
				AfxGetApp()->WriteProfileInt("INFORMATION", "bar_stop_visible", FALSE);
			}
		}
		else if (!sKey.CompareNoCase("flag"))
			AfxGetApp()->WriteProfileInt("INFORMATION", "listbartip_msg", 1);

		if (sKey.GetLength() > 8 && sKey.Left(8) == "tip_flag")
		{
			DWORD dwFlag = atol(sKey.Mid(8));
			AfxGetApp()->WriteProfileInt("INFORMATION", "tip_msg", AfxGetApp()->GetProfileInt("INFORMATION", "tip_msg", 0) | dwFlag);
		}
	}
	return 0;
}

void CDbar3::SetUseStop(int nKind, BOOL bUse)
{
	if (m_pStopPane)
	{
		switch (nKind)
		{
		case KS_STOCK:
			m_pStopPane->SetUseStockStop(bUse);	break;
		case KS_FUTURE:
			m_pStopPane->SetUseFOStop(bUse);	break;
		}
	}
}

void CDbar3::SetRunningStop(int nKind, BOOL bRunning)
{
	if (m_pStopPane)
	{
		switch (nKind)
		{
		case KS_STOCK:
			m_pStopPane->SetRunningStockStop(bRunning);	break;
		case KS_FUTURE:
			m_pStopPane->SetRunningFOStop(bRunning);	break;
		}
	}
	if (m_pStopPane->m_bStockRunning && m_pStopPane->m_bFORunning)
		m_pStopPane->SetWindowPos(NULL, 0, 0, 220, 20, SWP_NOZORDER|SWP_NOMOVE);
	else
		m_pStopPane->SetWindowPos(NULL, 0, 0, 110, 20, SWP_NOZORDER|SWP_NOMOVE);
	RepositionCtrls();
	if (bRunning)
	{
			m_pStopPane->ShowWindow(SW_SHOW);	// 실행중이 아니면 StopPane에서 비교하여 Hide시킴
	}
}


void CDbar3::change_Palette(CString skinN)
{
	CProfile profile(pkPalette);
	profile.Write(GENERALSN, szPalette, skinN);
}

CString CDbar3::GetCurrentSkinName()
{
	CProfile profile(pkPalette);
	CString skin(profile.GetString(GENERALSN, szPalette));
	return skin;
}

LONG CDbar3::OnXMSG(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case CTRL_CLOSEALL:
		CloseAllChildFrames();
		break;
	case CTRL_UPDN:
		MoveBar(GetBarStyle() & CBRS_ALIGN_TOP);
		break;
	case CTRL_SKIN:		
		{
			/*
			CMenuXP	menu;
			CRect rc;
			CString skinName;
			m_skin->GetWindowRect(&rc);
			
			menu.CreatePopupMenu();

			const char* skins[] = {
				"Blue", "Green", "Brown", "Violet", "Gray", NULL
			};

			skinName = GetCurrentSkinName();

			int count = 0;
			for (int i = 0; skins[i]; i++)
			{
				menu.AppendMenuX(MF_STRING, i + 1, skins[i]);
				if (!skinName.CompareNoCase(skins[i]))
					menu.CheckMenuItem(i + 1, MF_CHECKED);
				count++;
			}

			int nRtn = menu.TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, rc.left, rc.top, this);

			if (nRtn > 0 && nRtn <= count)
				skinName = skins[nRtn - 1];
			else
				break;

			change_Palette(skinName);
			(*m_axiscall)(AXI_CHANGEBAR2, 0, (long)(const char*)skinName);
			break;
			*/
		}
	}
	return 0;
}

CNButton* CDbar3::CreateNButton(unsigned int id, const char* title, const char* button, BOOL eachButton)
{
	CString slog;
	slog.Format("[axissm][CDbar3][CreateNButton] title=[%s] button=[%s]\n", title, button);
	//OutputDebugString(slog);
#ifdef DF_USESTL_DBAR3
	std::unique_ptr<CNButton> btn = std::make_unique<CNButton>(id);
#else 
	CNButton* btn = new CNButton(id);
#endif
	
	const DWORD	dwStyle = WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	if (!btn->Create(NULL, title, dwStyle, CRect(0,0,0,0), this, -1))
	{
#ifndef DF_USESTL_DBAR3
		delete btn;
#endif
		return NULL;
	}
	else
	{
		btn->set_ToolTipText(title);
		btn->set_ImageType(eachButton? IT_EACH : IT_INDEX);
		if (eachButton)
			btn->set_Image(CAxButtonName(button));
		else
			btn->set_Image(button);
		
		btn->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

#ifdef DF_USESTL_DBAR3
		return btn.release();
#else 
		return btn;
#endif
	
	}
}

void CDbar3::CloseAllChildFrames()
{
	(*m_axiscall)(AXI_DELHISTORYALL, 0, 0);
}


CRect CDbar3::GetScrButtonPos(CString code)
{
	_buttonHist* button{};
	CRect rc(0,0,0,0);
	for (int i = 0; i < m_arButton.GetSize(); i++)
	{
		button = m_arButton.GetAt(i);
		if (button->code == code)
		{
			CRect rc = GetButtonRect(i);
			//ClientToScreen(&rc);
			return rc;
		}
	}
	return rc;						
}

BOOL CDbar3::DrawMaskedBitmap(CDC* pDC, int x, int y, CBitmap* bitmap, int maskcolor, int width, int height)
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

int CDbar3::GetHeight()
{
	return m_height;
}

void CDbar3::MoveBar(int mb)
{

	if (mb == MB_TOP)
	{
		SetBarStyle((GetBarStyle() | CBRS_ALIGN_TOP) & ~CBRS_ALIGN_BOTTOM);
		m_moveBar->set_Image("tk_down");
	}
	else
	{
		SetBarStyle((GetBarStyle() | CBRS_ALIGN_BOTTOM) & ~CBRS_ALIGN_TOP);
		m_moveBar->set_Image("tk_up");
	}

	(*m_axiscall)(AXI_RECALCLAYOUT, 0, 0);		

	CProfile p(pkUserSetup);
	p.Write("INFORMATION", "listbar3_pos", (double)GetBarStyle());
	AfxGetApp()->WriteProfileInt("INFORMATION", "listbar3_pos", 0);

	ImageToggle();

	RepositionCtrls();
}


CString CDbar3::parseX(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

LRESULT CDbar3::OnMsgFromMain(WPARAM wParam, LPARAM lParam)
{

	switch (LOWORD(wParam))
	{
		/*	case MMSG_SEARCH_SISE:
			{
				char* pdata = (char*)lParam;

				struct	_grid {
					char	code[12];					   //종목코드
					char    hnam[20];                // 종목명
					char    curr[8];					  // 현재가
					char    diff[6];					  // 전일대비      (9999V99)
					char    gvol[12];				  // 거래량
				};

				struct  _interMod {
					char	keyf[2];		// ticker id
					char	nrec[3];		// count jcode
					struct	_grid	grid[1];	// data
				};

				CString		code, name, price, diff, gvol;
				_interMod* interMod = (struct _interMod*)pdata;
				int	cnt = atoi(CString(interMod->nrec, sizeof(interMod->nrec)));
				for (int ii = 0; ii < cnt; ii++)
				{
					code = CString(interMod->grid[ii].code, sizeof(interMod->grid[ii].code));	code.TrimRight();
					name = CString(interMod->grid[ii].hnam, sizeof(interMod->grid[ii].hnam));	name.TrimRight();
					price = CString(interMod->grid[ii].curr, sizeof(interMod->grid[ii].curr));	price.TrimRight();
					diff = CString(interMod->grid[ii].diff, sizeof(interMod->grid[ii].diff));	diff.TrimLeft();
					gvol = CString(interMod->grid[ii].gvol, sizeof(interMod->grid[ii].gvol));	gvol.TrimLeft();
					//SetInter((char*)&interMod->grid[ii]);
				}
			}
			break;
			case MMSG_SEARCH_GROUPCODE:
			{
				char* pdata = (char*)lParam;

				CString stmp, sRet, code;
				CString strseq = CString(pdata, 2);
				CString strgname = CString(&pdata[2], 20);
				int cnt = atoi(CString(&pdata[22], 4));
				int parseL = 26;

				struct _jinfo* jinfo{};
				char* pCode;
				pCode = new char[sizeof(jinfo->code) + 1];
				sRet.Empty();

				if (cnt <= 0)
					return 0;

				for (int ii = 0; ii < cnt; ii++)
				{
					jinfo = (struct _jinfo*)&pdata[parseL];

					memset(pCode, 0x00, sizeof(jinfo->code) + 1);
					memcpy(pCode, jinfo->code, sizeof(jinfo->code));  // 종목코드[12]

					code.Format("%s", pCode);
					code.TrimRight();


					auto inter = std::make_unique<_inter>();
					inter->Code = code;
					m_arInter.Add(inter.get());
					m_arInterByCode.SetAt(inter->Code, (CObject*)inter.get());

					m_arSymData.SetAt(inter->Code, "x");
					m_arSym.Add(inter->Code);

					parseL += sizeof(struct _jinfo);
				}
				SendPiboSise();
			}
			break;
			*/
	case MMSG_SEARCH_INTERGROUP:
	{
		CStringArray m_arrInterGroup;
		m_arrInterGroup.RemoveAll();

		CString stmp, seq, sgname, keys;
		char* pdata = (char*)lParam;
		int cnt = atoi(CString(pdata, 4));
		if (cnt == 0) return 0;

		int parseL = 4;
		struct _grSearch
		{
			char ngrs[2];
			char grseq[2];
			char gname[30];
		};

		struct _grSearch* stgr;
		char* pSeq, * pgame;
		pSeq = new char[sizeof(stgr->grseq) + 1];
		pgame = new char[sizeof(stgr->gname) + 1];

		for (int ii = 0; ii < cnt; ii++)
		{
			stgr = (struct _grSearch*)&pdata[parseL];
			memset(pSeq, 0x00, sizeof(stgr->grseq) + 1);
			memcpy(pSeq, stgr->grseq, sizeof(stgr->grseq));
			seq.Format("%s", pSeq);

			memset(pgame, 0x00, sizeof(stgr->gname) + 1);
			memcpy(pgame, stgr->gname, sizeof(stgr->gname));
			sgname.Format("%s", pgame);
			sgname.TrimRight();

			parseL += sizeof(struct _grSearch);

			//	m_arrInterGroup.SetAt(ii, sgname);
			m_arrInterGroup.Add(sgname);
		}

		delete[] pSeq; pSeq = nullptr;
		delete[] pgame; pgame = nullptr;

		CString		str, tmps;
		CMenuXP		menu;
		menu.CreatePopupMenu();
		int	index = 0;

		if (m_deleteBtn)
		{
			tmps = "종목 삭제";
			menu.AppendMenuX(MF_STRING, RBUTTONBASE, tmps);
			tmps = "관심 등록";
			menu.AppendMenuX(MF_STRING, RBUTTONBASE_INTERGROUP, tmps);
			menu.AppendMenuX(MF_SEPARATOR);

			for (int ii = 0; ii < m_arrInterGroup.GetSize(); ii++)
			{
				menu.AppendMenuX(MF_STRING, RBUTTONBASE_INTER + index, m_arrInterGroup.GetAt(ii));
				index++;
			}
		}

		CPoint	curPt;
		GetCursorPos(&curPt);
		const int nRet = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, curPt.x, curPt.y, this);

		CString slog;
		slog.Format("[code] nRet = %d", nRet);

		if (nRet == RBUTTONBASE)
		{
			if (m_deleteBtn)
				(*m_axiscall)(AXI_DELHISTORY, (long)(const char*)m_deleteBtn->code, 0);
		}
		else if (nRet == RBUTTONBASE_INTERGROUP)
		{
			(*m_axiscall)(AXI_ADDINTERHISTORY, -1, 0);
		}
		else if (nRet >= RBUTTONBASE_INTER && nRet <= RBUTTONBASE_INTER + index)
		{
			if (m_deleteBtn)
				(*m_axiscall)(AXI_ADDINTERHISTORY, nRet - 4000, (long)(const char*)m_deleteBtn->code);
		}
	}
	break;
	}

	return 0;
}