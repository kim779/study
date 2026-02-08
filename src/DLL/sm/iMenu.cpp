// iMenu.cpp : implementation file
//

#include "stdafx.h"
#include "iMenu.h"
#include "resource.h"
#include "../../h/axisvar.h"
#include "IhUtils.h"
#include "NButton.h"
#include <afx.h>

#include "../../axis/axMsg.hxx"
#include <windef.h>
#define COMPILE_MULTIMON_STUBS
#include "multimon.h"

#define HNEOMENU	// for xp menu , don't call DestroyMenu()

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TPM_FLAG	TPM_LEFTALIGN | TPM_RIGHTBUTTON
#define DT_FLAG		DT_CENTER|DT_VCENTER|DT_SINGLELINE

#define WM_CHANGEPOPUP	WM_USER+600
#define WM_SELECTMENU	WM_USER+601

//#define EXTEXT		"≫"
#define	SPLIT		'\t'
#define	SPLIT2		'\n'
#define	SIGN_SEP	'-'
#define	SIGN_MAP	'#'
#define SIGN_NEW	'@'
#define SIGN_REC	'$'
#define	LINKSPLIT	';'
#define SIGN_EDIT	'^'
#define L_LINK		4
#define MGAP		14


#define SELCOLOR	RGB(169, 205, 246)
#define	BKCOLOR		RGB(237, 243, 249)

#define	EXPANDWIDTH	13
#define	ITEMEXPAND	9999

//#define LINKMAP_BY_DISPNUMBER

#define clrTL	RGB(255, 255, 255)
#define clrBR	RGB(128, 128, 128)

CIMenu*		iMenu = NULL;
static CRect	g_monRc(0, 0, 0, 0);
static HHOOK	gHook = NULL;

#define IDC_MENUMODE	1500

BOOL CALLBACK MyInfoEnumProc(HMONITOR hMonitor,  HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	MONITORINFO lpmi ;
	memset(&lpmi,0,sizeof(MONITORINFO));
	lpmi.cbSize = sizeof(MONITORINFO);
	if(!GetMonitorInfo(hMonitor,  &lpmi ))
		return FALSE;

	if (g_monRc.left  > lpmi.rcWork.left)	g_monRc.left   = lpmi.rcWork.left;
	if (g_monRc.top   > lpmi.rcWork.top)	g_monRc.top    = lpmi.rcWork.top;
	if (g_monRc.right < lpmi.rcWork.right)	g_monRc.right  = lpmi.rcWork.right;
	if (g_monRc.bottom< lpmi.rcWork.bottom)	g_monRc.bottom = lpmi.rcWork.bottom;
	return TRUE;
}

LRESULT CALLBACK getMsgProc(int code, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK getMsgProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (iMenu && code == HC_ACTION)
	{
		const MSG	*msg = (MSG *) lParam;
		switch (msg->message)
		{
		case WM_MOUSEMOVE:
			iMenu->HookProcess(CPoint(msg->pt.x, msg->pt.y));
			break;
		case WM_NCLBUTTONDOWN:
		case WM_LBUTTONDOWN:
			//iMenu->HookLbutton(CPoint(msg->pt.x, msg->pt.y));
			break;
		case WM_SYSKEYDOWN:
			break;
#if 0
		case WM_SYSKEYUP:
			if (wParam != PM_REMOVE)
				break;
			if (msg->wParam == VK_F10 || msg->wParam == 18 /* ALT key */)
			{
				iMenu->HookSyskey();
				return TRUE;
			}
			break;
#endif
		case WM_KEYDOWN:
			switch (msg->wParam)
			{
			case VK_LEFT:case VK_RIGHT:
			case VK_DOWN:case VK_UP:
				if (wParam != PM_REMOVE)	break;
				iMenu->HookKeyboard(msg->wParam, msg->lParam);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}

	
	return CallNextHookEx(gHook, code, wParam, lParam);
}

UINT create_LinkFile(LPVOID lpvoid);
UINT create_LinkFile(LPVOID lpvoid)
{
	CIMenu*	iMenu = (CIMenu *) lpvoid;
	iMenu->makeLinkFile();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CIMenu

#pragma warning (disable : 26409)
#pragma warning (disable : 26400)
#pragma warning (disable : 26401)
#pragma warning (disable : 6011)

CIMenu::CIMenu(CWnd* parent, int commandBase)
{
	m_parent = parent;
	m_commandBase = commandBase;
	m_commandCnt  = 0;
	m_hover = -1;
	m_hideMenuN = -1;
	m_showMenuN = -1;
	m_moveMenuN = -1;
	m_mouseIN = false;
	m_showPopup = false;
	m_menuKeyOn = false;
	m_menuMode = MM_NORMAL;
	m_menuModeHover = FALSE;

	NONCLIENTMETRICS info; info.cbSize = sizeof(info);
	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
	m_font.CreateFontIndirect(&info.lfMenuFont);

	for (int ii = 0; ii < MAX; ii++)
	{
		m_popM[ii] = NULL;
		m_menuN[ii].Empty();
	}
	m_pMenu    = NULL;
	m_pNewMenu = NULL;

	m_arItem.RemoveAll();
	m_arMap.RemoveAll();
	m_arMenu.RemoveAll();
	m_arFrameMenu.RemoveAll();
	m_maplist.RemoveAll();
	m_mapedlist.RemoveAll();
	m_arMenu.RemoveAll();
	m_arsMenuT.RemoveAll();
	m_arMenuT.RemoveAll();
	m_arItemRect.RemoveAll();

	m_expandRc.SetRectEmpty();
	m_expandSYM.LoadString(ST_SYMBOL_MENUEXSYM);

	/*CFileBitmap fileBmp(Axis::home, "MenuBack");
	if (fileBmp.m_hObject)
	{
		m_back.Attach(fileBmp.Detach());
	}
	else
	{
		m_back.LoadBitmap(IDB_MENUBK);
	}*/

	//m_back = Axis::GetBitmap( IDB_MENUBK );
	m_back = Axis::GetBitmap( "MENUBACK" );

	/*CFileBitmap fileTerm(Axis::home, "MenuTerm");
	if (fileTerm.m_hObject)
	{
		m_bitmapMenuTerminator.Attach(fileTerm.Detach());
	}*/

	m_bmpMenumode[BMS_UP][BMS_NORMAL] = Axis::GetBitmap("TOOL_BTN_UP");
	m_bmpMenumode[BMS_UP][BMS_HOVER] = Axis::GetBitmap("TOOL_BTN_UP_EN");
	m_bmpMenumode[BMS_DOWN][BMS_NORMAL] = Axis::GetBitmap("TOOL_BTN_DOWN");
	m_bmpMenumode[BMS_DOWN][BMS_HOVER] = Axis::GetBitmap("TOOL_BTN_DOWN_EN");

	if (m_bmpMenumode[BMS_UP][BMS_NORMAL])
	{
		BITMAP bm;
		m_bmpMenumode[BMS_UP][BMS_NORMAL]->GetBitmap(&bm);
		m_szMenuMode.cx = bm.bmWidth;
		m_szMenuMode.cy = bm.bmHeight;
	}
	else
		m_szMenuMode = CSize(0, 0);

	m_bmpExpand = Axis::GetBitmap("MENU_MORE");
	if (m_bmpExpand)
	{
		BITMAP bm;
		m_bmpExpand->GetBitmap(&bm);
		m_szMenuExpand.cx = bm.bmWidth / 3;
		m_szMenuExpand.cy = bm.bmHeight;
	}
	else {
		m_szMenuExpand.cx = EXPANDWIDTH;
		m_szMenuExpand.cy = 10;
	}

/////////////////////////////////////////////////////////////////////////////
//	 특정사용자에게만 메뉴 오픈
// 2006 11 08
	CWinApp* app = AfxGetApp();
	m_id = app->GetProfileString(WORKSTATION, SIGNONID);
	m_id = IH::Parser(m_id, "|");
/////////////////////////////////////////////////////////////////////////////

	m_hNewIcon = AfxGetApp()->LoadIcon(IDI_NEW);
	m_hMedalIcon = AfxGetApp()->LoadIcon(IDI_MEDAL);
	m_hWebIcon = AfxGetApp()->LoadIcon(IDI_WEB);
	m_hEditIcon = AfxGetApp()->LoadIcon(IDI_EDIT);
}

CIMenu::~CIMenu()
{
	iMenu = NULL;
	if (gHook) ::UnhookWindowsHookEx(gHook);
#ifndef HNEOMENU
	for (int ii = 0; ii < m_arMenu.GetSize(); ii++)
		delete m_arMenu.GetAt(ii);
	for (ii = 0; ii < m_arFrameMenu.GetSize(); ii++)
		delete m_arFrameMenu.GetAt(ii);
#else
	for (int ii = 0; ii < m_arMenuT.GetSize(); ii++)
		delete m_arMenuT.GetAt(ii);
	for (int ii = 0; ii < m_arFrameMenu.GetSize(); ii++)
		delete m_arFrameMenu.GetAt(ii);
#endif
	_itemM* item{};
	CString	key;
	for (POSITION pos = m_arItem.GetStartPosition(); pos; )
	{
		m_arItem.GetNextAssoc(pos, key, (CObject *&) item);
		delete item;
	}

	if (m_back->m_hObject)
		::DeleteObject(m_back->Detach());
	if (m_bitmapMenuTerminator.m_hObject)
		::DeleteObject(m_bitmapMenuTerminator.Detach());

	m_arItem.RemoveAll();
	m_arMap.RemoveAll();
	m_arMenu.RemoveAll();
	m_arFrameMenu.RemoveAll();
	m_maplist.RemoveAll();
	m_mapedlist.RemoveAll();
	m_arsMenuT.RemoveAll();
	m_arMenuT.RemoveAll();
	m_arItemRect.RemoveAll();
	m_arIndex.RemoveAll();

	m_font.DeleteObject();

	if (m_pNewMenu)
		delete m_pNewMenu;

	if (m_hMedalIcon)
		DestroyIcon(m_hMedalIcon);
	if (m_hNewIcon)
		DestroyIcon(m_hNewIcon);
	if (m_hWebIcon)
		DestroyIcon(m_hWebIcon);
	if (m_hEditIcon)
		DestroyIcon(m_hEditIcon);
}

BEGIN_MESSAGE_MAP(CIMenu, CWnd)
	//{{AFX_MSG_MAP(CIMenu)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_WM_MEASUREITEM()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_CHANGEPOPUP, OnChangePopup)
	ON_MESSAGE(WM_SELECTMENU, OnSelectMenu)
	ON_MESSAGE(WM_XMSG, OnXMSG)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CIMenu message handlers

void CIMenu::MeasureMenuItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	HMENU hMenu = AfxGetThreadState()->m_hTrackingMenu;
	CMenu	*pMenu = CMenu::FromHandle(hMenu);
	pMenu->MeasureItem(lpMeasureItemStruct);
}

BOOL CIMenu::PreTranslateMsg(MSG* pMsg)
{
#if 0
	CPoint	point;
	switch (pMsg->message)
	{
	case WM_SYSKEYDOWN:
// No use F10 for hotkey
		if (pMsg->wParam == VK_F10)
			return TRUE;
		break;
	}
#endif
	return FALSE;
}

void CIMenu::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	drawMenu(&dc);
	DrawMenuMode(&dc);
}

void CIMenu::OnLButtonDown(UINT nFlags, CPoint point) 
{
	processButtonDown(point);
	CWnd::OnLButtonDown(nFlags, point);
}

void CIMenu::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	processButtonDown(point);
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CIMenu::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);

	const CPoint	lpoint = point;

	CheckMenuModeStatus(point);

	if (m_menuMode == MM_SMALL)
	{
		return;
	}

	CWnd*			wndUnderMouse = NULL;
	const CWnd*			wndActive = this;
	TRACKMOUSEEVENT		csTME{};

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
		hoverMenu(lpoint);
	}
	else 	
		m_mouseIN = false;
}

LRESULT CIMenu::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_mouseIN = false;
	drawSelectMenu(false);
	m_hover = -1;
	Invalidate();
	return 0;
}

LRESULT CIMenu::OnChangePopup(WPARAM wParam, LPARAM lParam)
{
	CMenuXP* menu{};
	CPoint	point;
	
	if (wParam)
		m_showMenuN = m_moveMenuN;
	m_showPopup = true;
	m_menuKeyOn = true;

	if (m_showMenuN != -1)
	{
		drawSelectMenu();
		if (m_showMenuN == ITEMEXPAND)
		{
			int	menuN = 0;
			const int itemCnt = m_arItemRect.GetSize();
			const UINT	nFlags = MF_POPUP|MF_BYPOSITION|MF_STRING;
			CString	str;

			CMenuXP*	popup = new CMenuXP();
			popup->CreatePopupMenu();
			for (int ii = m_hideMenuN; ii < itemCnt; ii++)
			{
				menu = m_arMenuT.GetAt(ii);
				str  = m_arsMenuT.GetAt(ii);
				popup->InsertMenuX(menuN++, nFlags, (UINT)menu->m_hMenu, str);
			}
			point = CPoint(m_expandRc.left, m_expandRc.bottom+1);
			ClientToScreen(&point);
			m_pMenu = popup;
			//if (point.x <= 0 && GetSystemMetrics(80) < 2)	point.x = 0;
			const int	lpos = GetScreenPos();
			if (point.x < lpos) point.x = lpos;
			popup->TrackPopupMenu(TPM_FLAG, point.x, point.y, m_parent);
#ifndef HNEOMENU
			while (popup->GetMenuItemCount())
				popup->RemoveMenu(0, MF_BYPOSITION);
			popup->DestroyMenu();
#else
			popup->Clear(false);
			while (popup->GetMenuItemCount())
				popup->RemoveMenu(0, MF_BYPOSITION);
			delete popup;
#endif
		}
		else
		{
			menu = m_arMenuT.GetAt(m_showMenuN);
			const CRect iRc = m_arItemRect.GetAt(m_showMenuN);
			point = CPoint(iRc.left, iRc.bottom+1);
			ClientToScreen(&point);
			m_pMenu = menu;
			//if (point.x <= 0 && GetSystemMetrics(80) < 2)	point.x = 0;
			const int	lpos = GetScreenPos();
			if (point.x < lpos) point.x = lpos;
			menu->TrackPopupMenu(TPM_FLAG, point.x, point.y, m_parent);
		}
	}

	drawSelectMenu(false);
	m_showPopup = false;
	m_menuKeyOn = false;
	gHook = NULL;
	return 0;
}

LRESULT CIMenu::OnSelectMenu(WPARAM wParam, LPARAM lParam)
{
	const CRect	iRc = m_arItemRect.GetAt(m_showMenuN);
	CPoint	point(iRc.left, iRc.bottom+1);

	drawSelectMenu();
	ClientToScreen(&point);

	m_showPopup = true;
	m_menuKeyOn = true;
	CMenuXP*	menu = m_arMenu.GetAt(m_showMenuN);
	m_pMenu = menu;
	menu->TrackPopupMenu(TPM_FLAG, point.x, point.y, m_parent);
	drawSelectMenu(false);
	m_showPopup = false;
	m_menuKeyOn = false;
	return 0;
}

void CIMenu::drawMenu(CDC* pDC)
{
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;

	drawBk(pDC);

	if (m_menuMode == MM_SMALL)
		return;
	
	const int itemCnt = m_arsMenuT.GetSize();
	if (itemCnt <= 0)	return;

	CSize	size;
	CString	text;
	CRect	cRc, iRc;
	CFont*	font = pDC->SelectObject(&m_font);
	
	GetClientRect(cRc);
	cRc.DeflateRect(0, 1);
	cRc.OffsetRect(80,0);
	cRc.right -=130;
	

	if (m_arItemRect.GetSize() <= 0)
	{
		iRc.CopyRect(&cRc);
		iRc.InflateRect(0, -2);
		for (int ii = 0; ii < itemCnt; ii++)
		{
			text = m_arsMenuT.GetAt(ii);
			size = pDC->GetTextExtent(text);

			iRc.right = iRc.left + MGAP + size.cx;
			m_arItemRect.Add(iRc);
			iRc.OffsetRect(iRc.Width(), 0);
		}
	}

	pDC->SetBkMode(TRANSPARENT);
	for (int ii = 0; ii < m_arItemRect.GetSize(); ii++)
	{
		text = m_arsMenuT.GetAt(ii);
		iRc = m_arItemRect.GetAt(ii);
		if (ii == itemCnt-1)
		{
			m_expandRc.SetRectEmpty();
			if (iRc.right + m_szMenuExpand.cx >= cRc.right)
			{
				m_hideMenuN = ii;
				drawExpand(pDC);
				break;
			}
			m_hideMenuN = -1;
		}
		else
		{
			if (iRc.right + m_szMenuExpand.cx >= cRc.right)
			{
				m_hideMenuN = ii;
				drawExpand(pDC);
				break;
			}
			else
				drawTerm(pDC, iRc);
		}
		pDC->SetTextColor(RGB(255,255,255));	
		pDC->DrawText(text, iRc, DT_FLAG);
		iRc.OffsetRect(iRc.Width(), 0);
	}
	
	pDC->SelectObject(font);
	
	if (m_showPopup)
		drawSelectMenu();
	else
	{
		drawSelectMenu(false);
		m_showMenuN = -1;
	}
}

void CIMenu::drawBk(CDC* pDC)
{
	CRect	cRc;
	CDC	memDC;
	BITMAP	bm{};

	GetClientRect(cRc);
	//**cRc.DeflateRect(0, 1);

	if (memDC.CreateCompatibleDC(pDC) && m_back)
	{
		m_back->GetBitmap(&bm);
		CBitmap* pbmp = (CBitmap *) memDC.SelectObject(m_back);
		pDC->StretchBlt(0, 0, cRc.Width(), cRc.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		memDC.SelectObject(pbmp);
		memDC.DeleteDC();
	}
	else
	{
		DrawGradient(pDC, cRc, RGB(255, 255, 255), RGB(223, 223, 225), false);
		/*pDC->SetBkColor(BKCOLOR);
		pDC->ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, &cRc, "", 0, NULL);*/
	}
	DrawMenuMode(pDC);
}

void CIMenu::drawExpand(CDC* pDC, int mode)
{
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;
	CRect		cRc, eRc;

	int	x{};

	GetClientRect(cRc);
	cRc.DeflateRect(0, 1);
	eRc.SetRect(cRc.right - m_szMenuExpand.cx-50, cRc.top, cRc.right-50, cRc.bottom-1);
	m_expandRc.CopyRect(&eRc);

	if (!m_bmpExpand)
	{
		switch (mode)
		{
		default:
		case M_COOL:
			pDC->FillSolidRect(m_expandRc, BKCOLOR);
			break;
		case M_OVER:
			pDC->Draw3dRect(m_expandRc, clrTL, clrBR);
			break;
		case M_DOWN:
			pDC->FillSolidRect(m_expandRc, BKCOLOR);
			break;
		}
		pDC->SetTextColor(RGB(255,255,255));
		pDC->DrawText(m_expandSYM, m_expandRc, DT_CENTER|DT_TOP|DT_SINGLELINE);
	}
	else
	{
		//eRc.OffsetRect(-5, 0);
		//eRc.OffsetRect(-50, 0);
		switch (mode)
		{
		default:
		case M_COOL:	x = 0;		break;
		case M_OVER:	x = m_szMenuExpand.cx;	break;
		case M_DOWN:	x = m_szMenuExpand.cx * 2;	break;
		}

		CDC	memDC;
		if (memDC.CreateCompatibleDC(pDC))
		{
			CBitmap* oldbuffBitmap = memDC.SelectObject(m_bmpExpand);
			if (oldbuffBitmap != (CBitmap *) 0)
			{
				pDC->BitBlt(eRc.left, eRc.top+1, m_szMenuExpand.cx, m_szMenuExpand.cy, 
					&memDC, x, 0, SRCCOPY);
				memDC.SelectObject(oldbuffBitmap);
			}
			else	
				memDC.DeleteDC();
		}
	}

	DrawMenuMode(pDC);
}

void CIMenu::drawSelectMenu(bool select)
{
	int	nItem = m_showMenuN;

	if (m_showMenuN == -1)
	{
		if (m_hover == -1)	
			return;

		nItem = m_hover;
	}

	CClientDC	dc(this);
	CString		str  = nItem == ITEMEXPAND ? m_expandSYM : m_arsMenuT.GetAt(nItem);
	CFont*		font = dc.SelectObject(&m_font);
	CRect		iRc  = nItem == ITEMEXPAND ? m_expandRc : m_arItemRect.GetAt(nItem);

	CRect	cRc;
	CRgn	clipRgn;

	GetClientRect(cRc);
	cRc.DeflateRect(0, 1);

	
	clipRgn.CreateRectRgn(iRc.left, cRc.top, iRc.right, cRc.bottom);
	dc.SelectClipRgn(&clipRgn, RGN_COPY);

	drawBk(&dc);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(255,255,255));
	if (select)
	{
		if (!str.Compare(m_expandSYM)) 
		{
			dc.SelectClipRgn(NULL);
			drawExpand(&dc, M_DOWN); //dc.DrawText(str, iRc, DT_CENTER|DT_TOP|DT_SINGLELINE);
		}
		else	
		{
			dc.Draw3dRect(iRc.left, cRc.top, iRc.right, cRc.bottom-1, clrBR, clrTL);
			iRc.OffsetRect(1, 1);

			dc.DrawText(str, iRc, DT_FLAG);
		}
	}
	else
	{
		if (!str.Compare(m_expandSYM))
		{
			dc.SelectClipRgn(NULL);
			drawExpand(&dc, M_COOL);//dc.DrawText(str, iRc, DT_CENTER|DT_TOP|DT_SINGLELINE);
		}
		else
		{
			dc.DrawText(str, iRc, DT_FLAG);
		}
	}


	dc.SelectClipRgn(NULL);
	dc.SelectObject(font);

	clipRgn.DeleteObject();

	DrawMenuMode(&dc);
}

BOOL CIMenu::HookKeyboard(WPARAM wParam, LPARAM lParam)
{
	CPoint	point;
	int	newhover{};
	switch(wParam)
	{
	case VK_LEFT:
		if (m_showPopup)
		{
			processArrow(VK_LEFT);
		}
		else if (m_menuKeyOn)
		{
			if (m_hover == ITEMEXPAND)
				newhover = m_hideMenuN - 1;
			else
			{
				newhover = m_hover - 1;
				if (m_hideMenuN == -1)
				{
					if (newhover < 0)
						newhover = m_arItemRect.GetSize() - 1;
				}
				else
				{
					if (newhover < 0)
						newhover = ITEMEXPAND;
				}
			}

			if (newhover == ITEMEXPAND)
				point = m_expandRc.CenterPoint();
			else	
				point = m_arItemRect.GetAt(newhover).CenterPoint();
			hoverMenu(point);
		}
		break;
	case VK_RIGHT:
		if (m_showPopup)
		{
			processArrow(VK_RIGHT);
			
		}
		else if (m_menuKeyOn)
		{
			if (m_hover == ITEMEXPAND)
				newhover = 0;
			else
			{
				newhover = m_hover + 1;
				if (m_hideMenuN == -1)
				{
					if (newhover >= m_arItemRect.GetSize())
						newhover = 0;
				}
				else
				{
					if (newhover >= m_hideMenuN)
						newhover = ITEMEXPAND;
				}
			}

			if (newhover == ITEMEXPAND)
				point = m_expandRc.CenterPoint();
			else	
				point = m_arItemRect.GetAt(newhover).CenterPoint();
			hoverMenu(point);
		}
		break;
	case VK_DOWN:
	case VK_UP:
		if (m_menuKeyOn && !m_showPopup)
		{
			if (m_arItemRect.GetSize())
			{
				m_showMenuN = m_hover;
				PostMessage(WM_CHANGEPOPUP, 0, 0);
		//		PostMessage(WM_KEYDOWN, VK_DOWN, 0);
			}
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

void CIMenu::HookProcess(CPoint point)
{
	if (!(GetParent()->GetStyle() & WS_VISIBLE))
		return;
	if (m_showMenuN == -1)	return;

	if (m_curPoint.x == point.x && m_curPoint.y == point.y)
		return;
	const CWnd* wndUnderMouse = WindowFromPoint(point);
	if (wndUnderMouse != this)	return;

	m_curPoint = point;
	ScreenToClient(&point);

	CRect	iRc, cRc;
	int	newshowMenuN = -1;
	const int	itemCnt = m_arItemRect.GetSize();

	GetClientRect(cRc);
	cRc.DeflateRect(0, 1);
	cRc.right-=50;
	
	for (int ii = 0; ii < itemCnt; ii++)
	{
		iRc = m_arItemRect.GetAt(ii);
		if (ii == itemCnt-1)
		{
			if (iRc.right + m_szMenuExpand.cx >= cRc.right)
				break;
		}
		else
		{
			if (iRc.right + m_szMenuExpand.cx >= cRc.right)
				break;
		}

		if (iRc.PtInRect(point))
		{
			newshowMenuN = ii;
			break;
		}
	}

	if (newshowMenuN == -1 && m_expandRc.PtInRect(point))
		newshowMenuN = ITEMEXPAND;
	if (newshowMenuN == m_showMenuN || newshowMenuN == -1)
		return;

	m_parent->PostMessage(WM_CANCELMODE);
	m_moveMenuN = -1;
	PostMessage(WM_CHANGEPOPUP);
	
	drawSelectMenu(false);
	m_showMenuN = newshowMenuN;
}

void CIMenu::LoadFramemenu(UINT nIDResource)
{
	CMenuXP	frameMenu, * menu{}, * subMenu{};
	if (!frameMenu.LoadMenu(MAKEINTRESOURCE(nIDResource)))
	{
		TRACE(_T("Failed to load menu\n"));
		return;
	}

	int	menuCnt{}, subCnt{}, nID{};
	const UINT	nFlags = MF_POPUP|MF_BYPOSITION|MF_STRING;
	CString	str;

	menuCnt = frameMenu.GetMenuItemCount();
	for(int ii = 0; ii < menuCnt; ii++)
	{
		frameMenu.GetMenuString(ii, str, MF_BYPOSITION);

		menu = new CMenuXP();
		menu->CreatePopupMenu();

		m_arMenu.Add(menu);
		m_arMenuT.Add(menu);
		m_arsMenuT.Add(str);

		subMenu = (CMenuXP *) frameMenu.GetSubMenu(ii);
		subCnt = subMenu->GetMenuItemCount();
		for (int jj = 0; jj < subCnt; jj++)
		{
			nID = subMenu->GetMenuItemID(jj);

			switch (nID)
			{
			case 0:		// SEPARATOR
				str = "-";
				break;
			case -1:	// pop-up menu
			{
				CMenuXP* subpopup{}, * popup = new CMenuXP();
					popup->CreateMenu();

					m_arMenu.Add(popup);
					subMenu->GetMenuString(jj, str, MF_BYPOSITION);
					menu->InsertMenuX(jj, nFlags, (UINT) popup->m_hMenu, str);
					subpopup = (CMenuXP *) subMenu->GetSubMenu(jj);
					for (int kk = 0; kk < (int) subpopup->GetMenuItemCount(); kk++)
					{
						nID = subpopup->GetMenuItemID(kk);
						switch (nID)
						{
						case -1:	continue;
						case 0:		// separator
							str = "-";
							break;
						default:
							subpopup->GetMenuString(kk, str, MF_BYPOSITION);
							break;
						}

						if (!str.Compare("-"))
							popup->AppendMenuX(MF_SEPARATOR);
						else	popup->AppendMenuX(MF_STRING, nID, str);
					}
				}
				// popup menu process
				continue;
			default:
				subMenu->GetMenuString(jj, str, MF_BYPOSITION);
				break;
			}
			if (!str.Compare("-"))
				menu->AppendMenuX(MF_SEPARATOR);
			else	menu->AppendMenuX(MF_STRING, nID, str);
		}
	}
#ifndef HNEOMENU
	frameMenu.DestroyMenu();
#endif
	iMenu = this;
	Invalidate();

	gHook = ::SetWindowsHookEx(WH_GETMESSAGE, getMsgProc, 
		AfxGetInstanceHandle(), GetCurrentThreadId());

}

void CIMenu::LoadMenufromfile()
{
	CString	file;
	char	buf[1024 * 2]{};

	file = GetProfileFileName(pkAxisMenu);
	m_commandCnt = 0;

	CFileFind pFind;

	const BOOL bRet = pFind.FindFile(file);

	if(!bRet)
	{
		CString err = "메뉴 파일이 존재하지 않습니다.";

		WriteFile((char *)(const char*)err, err.GetLength());
	}

	FILE *fp = fopen(file, "r");
	if (!fp)
	{
		CString err = "메뉴 파일을 읽을 수 없습니다.";

		WriteFile((char *)(const char*)err, err.GetLength());

		Invalidate();
		return;
	}

	m_maplist.RemoveAll();
	m_mapedlist.RemoveAll();
	while (fgets(buf, sizeof(buf), fp) != NULL)
		makeMenu(buf);
	fclose(fp);

//	gHook = ::SetWindowsHookEx(WH_GETMESSAGE, getMsgProc, 
//		AfxGetInstanceHandle(), GetCurrentThreadId());

	Invalidate();
#if 1
	makeLinkFile();
#else
	Sleep(0);
	AfxBeginThread(create_LinkFile, this);
#endif
}

void CIMenu::WriteFile(char* pBytes, int nBytes)
{
#if 0
	CFile		Dfile;
	DWORD		pos;
	CString		filename, time;
	
	filename.Format("%s\\menulog", Axis::home);
	BOOL bExist = Dfile.Open(filename, CFile::modeReadWrite);//CFile::modeCreate|CFile::modeReadWrite);
	if (!bExist)
	{
		Dfile.Open(filename, CFile::modeCreate|CFile::modeReadWrite);
		Dfile.Write(pBytes, nBytes);
	}
	else
	{
		pos = Dfile.SeekToEnd();
		Dfile.Seek((long) pos, CFile::begin);
		Dfile.Write(pBytes, nBytes);
	}
	Dfile.Close();
#endif
}

void CIMenu::makeMenu(CString dat)
{
	int	tmpI{}, menuN{}, step = 0;
	BOOL	map{}, sep{};
	const UINT	nFlags = MF_POPUP|MF_BYPOSITION|MF_STRING;
	CString	str, dispN, tmpS, menu1, menu2;
	
	if (!m_pNewMenu)
	{
		m_pNewMenu = new CMenuXP();
		m_pNewMenu->CreatePopupMenu();
	}

	CMenuXP* popM{};
	map = sep = FALSE;
	menu1 = menu2 = "";

	_itemM* item{};
	BOOL isWeb{}, isNew{}, isRec{}, isEdit{};

	while (!dat.IsEmpty())
	{
		isWeb = isNew = isRec = isEdit= FALSE;
		int pos = dat.Find(SPLIT);
		if (pos == -1)	
			break;

		str = dat.Left(pos++);
		dat = dat.Mid(pos);
		if (str.IsEmpty())	
			continue;

		if (menu1.IsEmpty())
			menu1 = str;
		else if (menu2.IsEmpty() && !str.IsEmpty() && str[0] != '#' && str[0] != '$' && str[0] != '@' && str[0] != '^')
			menu2 = str;

		const char	ch = str.GetAt(0);

		switch (ch)
		{
		case SIGN_SEP:	
			sep = TRUE;	
			break;
		case SIGN_MAP:	
			map = TRUE;	
			break;
		case SIGN_NEW:
			map = TRUE;
			isNew = TRUE;
			break;
		case SIGN_REC:
			map = TRUE;
			isRec = TRUE;
			break;
		case SIGN_EDIT:
			map = TRUE;
			isEdit = TRUE;
			break;
		default:			
			break;
		}

		if (sep || map || isRec || isEdit)	
			str = str.Mid(1);

		if (step > MAX)	
			return;
			
		switch (step)
		{
		case 0:
			if (!m_menuN[step].Compare(str))
			{
				step++;
				continue;
			}
			popM = new CMenuXP();
			popM->CreatePopupMenu();

			m_arMenu.Add(popM);
			tmpI = m_arMenuT.GetSize();
			/*if (tmpI >= 1)
			{
				m_arMenuT.InsertAt(tmpI -1, popM);
				m_arsMenuT.InsertAt(tmpI-1, str);
			}
			else
			{*/
				m_arMenuT.Add(popM);
				m_arsMenuT.Add(str);
			//} 

			m_popM[step]  = popM;
			m_menuN[step++] = str;
			for (menuN = step; menuN < MAX; menuN++)
			{
				m_popM[menuN] = NULL;
				m_menuN[menuN].Empty();
			}
			break;
		default:
			if (!m_popM[step-1])	return;
			menuN = m_popM[step-1]->GetMenuItemCount();
			if (sep)
			{
				m_popM[step-1]->AppendMenuX(MF_SEPARATOR);
				return;
			}

			if (map)
			{
/////////////////////////////////////////////////////////////////////////////
//	 특정사용자에게만 메뉴 오픈
// 2006 11 08
				CMapStringToString	mapList;
				const int	ncnt = IH::GetUnitInList(mapList, str, ",", "{", "}");
				if (ncnt)
				{
					CString	sztmp = _T("");
					if (!mapList.Lookup(m_id, sztmp))
						return;

					const int	nFind = str.Find("}");
					if (nFind >= 0)
						str = str.Mid(nFind + 1);
				}
/////////////////////////////////////////////////////////////////////////////
//	 아이콘 삽입
				HICON hIcon = NULL;

				/**
				if (!str.IsEmpty() && str.GetAt(0) == '@')
				{
					int nFind = str.Find("@", 1);
					if (nFind >= 0)
					{
						int nIcon = atoi(str.Mid(1, nFind - 1));
						switch (nIcon)
						{
						case 1:
							hIcon = m_hNewIcon;
							break;
						case 2:
							hIcon = m_hMedalIcon;
							break;
						}
						str = str.Mid(nFind + 1);
					}
				}
				**/
				if (isNew)
					hIcon = m_hNewIcon;
				else if (isRec)
					hIcon = m_hMedalIcon;
				/*
				else if (isEdit)
					hIcon = m_hEditIcon;
				*/
/////////////////////////////////////////////////////////////////////////////
				
				item = saveinfo(str, dat, menu1, menu2);

// 				CString s;
// 				s.Format("MENU CREATE : [%s]	[%s]	[%s]	[%s]\n",str,dat,menu1,menu2);
// 				OutputDebugString(s);
				if (!item) 
					return;

				tmpS = item->disN;
				if (tmpS.IsEmpty())
					return;
				

				isWeb = item->isWeb;

				if (isWeb)
					hIcon = m_hWebIcon;
				
				dispN.Format("%s %s", tmpS, str);

				if (hIcon == m_hNewIcon)
					m_pNewMenu->AppendMenuX(MF_STRING|MF_ENABLED,
						m_commandBase + m_commandCnt, dispN, NULL);

				m_popM[step-1]->AppendMenuX(MF_STRING|MF_ENABLED,
					m_commandBase + m_commandCnt++, dispN, hIcon);

				return;
			}

			if (!m_menuN[step].Compare(str))
			{
				step++;
				continue;
			}

			HICON hIcon = NULL;
			if (!str.IsEmpty() && str.GetAt(0) == '@')
			{
				const int nFind = str.Find("@", 1);
				if (nFind >= 0)
				{
					int nIcon = atoi(str.Mid(1, nFind - 1));
					switch (nIcon)
					{
					case 1:
						hIcon = m_hNewIcon;
						break;
					case 2:
						hIcon = m_hMedalIcon;
						break;
					}
					str = str.Mid(nFind + 1);
				}
			}

			popM = new CMenuXP();
			popM->CreatePopupMenu();
			m_popM[step-1]->InsertMenuX(menuN, nFlags, (UINT) popM->m_hMenu, str, hIcon);
			m_arMenu.Add(popM);

			m_popM[step] = popM;
			m_menuN[step++] = str;
			for (menuN = step; menuN < MAX; menuN++)
			{
				m_popM[menuN] = NULL;
				m_menuN[menuN].Empty();
			}
			break;
		}
	}
}

_itemM* CIMenu::saveinfo(CString desc, CString dat)
{
	CString	key, str;
	_itemM*	item = new _itemM();

	int	idx = 0;
	for (int ii = 0; ii < 4; ii++)
	{
		if (!dat.IsEmpty())
		{
			int pos = dat.Find(SPLIT);
			const int pos2 = dat.Find(SPLIT2);
			if (pos == -1 && pos2 == -1)
				str.Empty();
			else
			{
				if (pos == -1)	pos = pos2;
				str = dat.Left(pos++);
				dat = dat.Mid(pos);
			}
		}
		else	str.Empty();

		str.TrimRight();
		switch (idx++)
		{
		case 0:	// 화면명
			item->mapN = str;	break;
		case 1:	// 단축명
			item->sName = str;	break;
		case 2:	// 화면번호
			item->disN  = str;	break;
		case 3:	// 부가정보
			item->info  = str;	break;
		}
	}

	item->disp = desc;
	item->mapN.MakeUpper();
	if (item->mapN.GetLength() < L_MAPN)
	{
		delete item;
		return NULL;
	}

	item->isWeb = IsWebMenu(item->mapN);
	item->disN = item->mapN.Mid(2, 4);
	item->sName = item->disN;

	_itemM* xitem = NULL;
	if (m_arItem.Lookup(item->mapN, (CObject *&) xitem))
	{	//	중복된 메뉴...........
		TRACE("[%s][%s]\n", item->mapN, item->disp);
		m_arMap.Add(xitem->mapN);
		delete item;
		return xitem;
	}
	
	m_arItem.SetAt(item->mapN, (CObject *&) item);
	m_arMap.Add(item->mapN);

	m_maplist.SetAt(item->mapN, item->disN);
	m_mapedlist.SetAt(item->disN, item->mapN);
	return item;
}

_itemM* CIMenu::saveinfo(CString desc, CString dat, CString menu1, CString menu2)
{
	CString	key, str;
	_itemM*	item = new _itemM();

	int	idx = 0;
	for (int ii = 0; ii < 4; ii++)
	{
		if (!dat.IsEmpty())
		{
			int pos = dat.Find(SPLIT);
			const int pos2 = dat.Find(SPLIT2);
			if (pos == -1 && pos2 == -1)
				str.Empty();
			else
			{
				if (pos == -1)	pos = pos2;
				str = dat.Left(pos++);
				dat = dat.Mid(pos);
			}
		}
		else	str.Empty();

		str.TrimRight();
		switch (idx++)
		{
		case 0:	// 화면명
			item->mapN = str;	break;
		case 1:	// 단축명
			item->sName = str;	break;
		case 2:	// 화면번호
			item->disN  = str;	break;
		case 3:	// 부가정보
			item->info  = str;	break;
		}
	}
	//AfxMessageBox(item->mapN+")"+desc);
	
	item->disp = desc;
	item->mapN.MakeUpper();
	item->menu1 = menu1;
	item->menu2 = menu2;

	if (item->mapN.GetLength() < L_MAPN)
	{
		delete item;
		return NULL;
	}

	/**
	if (!item->mapN.Left(4).Compare(WEB))
		item->isWeb = TRUE;
	if (item->info.Find("http") >= 0)
		item->isWeb = TRUE;
	**/
	

	/**
	if (item->disN.IsEmpty())
	{
		if (item->isWeb)
			item->disN = item->mapN.Mid(4, 4);
		else	
			item->disN = item->mapN.Mid(2, 4);
	}

	if (item->sName.IsEmpty())
	{
		if (item->isWeb)
			item->sName = item->mapN.Mid(4, 4);
		else	
			item->sName = item->disN;
	}
	**/
	item->isWeb = IsWebMenu(item->mapN);
	if (item->disN.IsEmpty())
		item->disN = item->mapN.Mid(2, 4);
	if (item->sName.IsEmpty())
		item->sName = item->disN;

	_itemM* xitem = NULL;
	if (m_arItem.Lookup(item->mapN, (CObject *&) xitem))
	{	//	중복된 메뉴...........
#ifdef _DEBUG
	TRACE("[%s][%s]\n", item->mapN, item->disp);
#endif
		m_arMap.Add(xitem->mapN);
		delete item;
		return xitem;
	}
	
	m_arItem.SetAt(item->mapN, (CObject *&) item);
	m_arMap.Add(item->mapN);

	m_maplist.SetAt(item->mapN, item->disN);
	m_mapedlist.SetAt(item->disN, item->mapN);
	return item;
}

void CIMenu::hoverMenu(CPoint point)
{
	const int	itemCnt = m_arsMenuT.GetSize();
	if (!itemCnt)		
		return;

	if (GetMenuModeRect().PtInRect(point))
	{
		if (m_hover != -1)
		{
			m_hover = -1;
			Invalidate();
		}
		return;
	}

	int	newhover = -1;
	CRect	cRc, iRc;
	GetClientRect(cRc);

	cRc.DeflateRect(0, 1);
	cRc.right -=50;
	for (int ii = 0; ii < m_arItemRect.GetSize(); ii++)
	{
		iRc = m_arItemRect.GetAt(ii);
		if (ii == itemCnt-1)
		{
			if (iRc.right + m_szMenuExpand.cx >= cRc.right)
				break;
		}
		else
		{
			if (iRc.right + m_szMenuExpand.cx >= cRc.right)
				break;
		}

		if (iRc.PtInRect(point))
		{
			newhover = ii;
			break;
		}
	}

	if (newhover == -1 && m_expandRc.PtInRect(point))
		newhover = ITEMEXPAND;

	if (newhover == m_hover)	
		return;

	CString	str;
	CDC*	pDC = GetDC();
	CFont*	font = pDC->SelectObject(&m_font);

	pDC->SetBkMode(TRANSPARENT);
	if (m_hover != -1)		// clear pre hover
	{
		if (m_hover == ITEMEXPAND)
			drawExpand(pDC, M_COOL);
		else
		{
			iRc = m_arItemRect.GetAt(m_hover);
			str = m_arsMenuT.GetAt(m_hover);
		
			CRgn	clipRgn;
			clipRgn.CreateRectRgn(iRc.left, cRc.top, iRc.right, cRc.bottom);
			pDC->SelectClipRgn(&clipRgn, RGN_COPY);

			drawBk(pDC);
			drawTerm(pDC, iRc);
			pDC->SetTextColor(RGB(255,255,255));
			pDC->DrawText(str, iRc, DT_FLAG);
			pDC->SelectClipRgn(NULL);
			clipRgn.DeleteObject();
		}
	}	

	m_hover = newhover;
	if (m_hover != -1)
	{
		if (m_hover == ITEMEXPAND)
			drawExpand(pDC, M_OVER);
		else
		{
			iRc = m_arItemRect.GetAt(m_hover);
			str = m_arsMenuT.GetAt(m_hover);

			iRc.top = cRc.top;
			iRc.bottom = cRc.bottom-1;
			if (m_menuKeyOn)
				pDC->Draw3dRect(iRc, clrBR, clrTL);
			else	pDC->Draw3dRect(iRc, clrTL, clrBR);
		}
	}

	pDC->SelectObject(font);
	ReleaseDC(pDC);
}

LONG CIMenu::OnXMSG(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
#ifdef DF_MK_CAPTION
	case CTRL_MARKET:
	{
		ToggleMarketMenu();
	}
	break;
#endif
	case CTRL_UPDN:
		m_nShow = !m_nShow;

		if(!m_nShow)
			m_moveBar->set_Image("tool_down");
		else
			m_moveBar->set_Image("tool_up");

		AfxGetMainWnd()->SendMessage(WM_AXIS, MAKEWPARAM(axToolbarHide, 0), (long)(const char*)"HIDE");
		break;
	}

	return 0;
}
void CIMenu::processButtonDown(CPoint point)
{
	if (GetMenuModeRect().PtInRect(point))
	{
#ifdef DF_MK_CAPTION
		ToggleMenuMode();
#else
		
#endif
		return;
	}

	if (m_menuMode != MM_NORMAL)
		return;
	
	CRect	iRc, cRc;
	const int	selectMenuN = -1;
	const int	itemCnt = m_arItemRect.GetSize();

	GetClientRect(cRc);
	cRc.DeflateRect(0, 1);
	for (int ii = 0; ii < itemCnt; ii++)
	{
		iRc = m_arItemRect.GetAt(ii);
		if (ii == itemCnt-1)
		{
			if (iRc.right + m_szMenuExpand.cx > cRc.right)
				break;
		}
		else
		{
			if (iRc.right + m_szMenuExpand.cx >= cRc.right)
				break;
		}

		if (iRc.PtInRect((point)))
		{
			CPoint	point(iRc.left, iRc.bottom+1);
			ClientToScreen(&point);

			if (m_showMenuN == ii)
			{
				m_showMenuN = -1;
				m_parent->PostMessage(WM_CANCELMODE);
				break;
			}
			m_showMenuN = ii;

			m_showPopup = true;
			m_menuKeyOn = true;
			CMenuXP*	menu = m_arMenuT.GetAt(ii);
			m_pMenu = menu;
			//if (point.x <= 0 && GetSystemMetrics(80) < 2)	point.x = 0;
			const int	lpos = GetScreenPos();
			if (point.x < lpos) point.x = lpos;

			menu->TrackPopupMenu(TPM_FLAG, point.x, point.y, m_parent);
			drawSelectMenu(false);

			m_showPopup = false;
			m_menuKeyOn = false;
			return;
		}
	}

	if (selectMenuN == -1 && m_expandRc.PtInRect(point))
	{
		if (m_showMenuN == ITEMEXPAND)
		{
			m_showMenuN = -1;
			m_parent->PostMessage(WM_CANCELMODE);
			return;
		}
		m_showMenuN = ITEMEXPAND;

		CString	str;
		int	menuN = 0;
		CMenuXP* menu{}, * popup = new CMenuXP();
		popup->CreatePopupMenu();
		for (int ii = m_hideMenuN; ii < itemCnt; ii++)
		{
			menu = m_arMenuT.GetAt(ii);
			str  = m_arsMenuT.GetAt(ii);
			popup->InsertMenuX(menuN++, MF_POPUP|MF_BYPOSITION|MF_STRING, (UINT)menu->m_hMenu, str);
		}

		point = CPoint(m_expandRc.left, m_expandRc.bottom+1);
		ClientToScreen(&point);

		m_showPopup = true;
		m_menuKeyOn = true;
		m_pMenu = popup;
		//if (point.x <= 0 && GetSystemMetrics(80) < 2)	point.x = 0;
		const int	lpos = GetScreenPos();
		if (point.x < lpos) point.x = lpos;
		popup->TrackPopupMenu(TPM_FLAG, point.x, point.y, m_parent);

#ifndef HNEOMENU
		while (popup->GetMenuItemCount())
			popup->RemoveMenu(0, MF_BYPOSITION);
		popup->DestroyMenu();
#else
		popup->Clear(false);
		while (popup->GetMenuItemCount())
			popup->RemoveMenu(0, MF_BYPOSITION);
		delete popup;
#endif
		drawSelectMenu(false);
		m_showPopup = false;
		m_menuKeyOn = false;
	}
}

void CIMenu::HookLbutton(CPoint point)
{
	CRect	cRc;
	GetClientRect(cRc);
	cRc.DeflateRect(0, 1);
	ScreenToClient(&point);
	if (!cRc.PtInRect(point))
	{
		if (m_menuKeyOn)
			m_menuKeyOn = false;
		m_showMenuN = -1;
		m_hover = -1;
		Invalidate(FALSE);
	}
}

void CIMenu::processArrow(TCHAR key)
{
	if (!m_menuKeyOn && !(m_pMenu && m_pMenu->GetSafeHmenu()))
		return;

	CMenuXP		*menu = NULL;
	int		itemCnt{};
	MENUITEMINFO	info;
	CString		str;
	
	info.cbSize = sizeof (MENUITEMINFO);
	info.fMask = MIIM_STATE|MIIM_SUBMENU;

	if (key == VK_LEFT)
	{
		itemCnt = m_arIndex.GetSize();
		if (itemCnt)
		{
			for (int ii = 0; ii < itemCnt; ii++)
			{
				const int index = m_arIndex.GetAt(ii);
				if (!ii)
					menu = (CMenuXP *) m_pMenu->GetSubMenu(index);
				else	menu = (CMenuXP *) menu->GetSubMenu(index);
			}
		}
		else	menu = m_pMenu;

		if (!menu)	menu = m_pMenu;
		itemCnt = menu->GetMenuItemCount();
		for (int ii = 0; ii < itemCnt; ii++)
		{
			menu->GetMenuItemInfo(ii, &info, TRUE);
			menu->GetMenuString(ii, str, MF_BYPOSITION);
			if (!(info.fState & MFS_HILITE))
				continue;

			if (menu != m_pMenu)
			{
				m_arIndex.RemoveAll();
				return;
			//	m_arIndex.RemoveAt(m_arIndex.GetSize() - 1);
			//	break;
			}
			if (m_showMenuN == ITEMEXPAND)
			{
				m_moveMenuN = m_hideMenuN - 1;
			}
			else
			{
				m_moveMenuN = m_showMenuN - 1;
				if (m_moveMenuN < 0)
				{
					if (m_hideMenuN == -1)
						m_moveMenuN = m_arItemRect.GetSize() - 1;
					else	m_moveMenuN = ITEMEXPAND;
				}
			}
			m_arIndex.RemoveAll();
			m_parent->PostMessage(WM_CANCELMODE);

			PostMessage(WM_CHANGEPOPUP, 1, 0);
			PostMessage(WM_KEYDOWN, VK_DOWN, 0);
			return;
		}

		if (m_showMenuN == ITEMEXPAND)
		{
			m_moveMenuN = m_hideMenuN - 1;
		}
		else
		{
			m_moveMenuN = m_showMenuN - 1;
			if (m_moveMenuN < 0)
			{
				if (m_hideMenuN == -1)
					m_moveMenuN = m_arItemRect.GetSize() - 1;
				else	m_moveMenuN = ITEMEXPAND;
			}
		}
		m_arIndex.RemoveAll();
		m_parent->PostMessage(WM_CANCELMODE);
		PostMessage(WM_CHANGEPOPUP, 1, 0);
		PostMessage(WM_KEYDOWN, VK_DOWN, 0);
	}
	else
	{
		itemCnt = m_arIndex.GetSize();
		if (itemCnt)
		{
			for (int ii = 0; ii < itemCnt; ii++)
			{
				const int index = m_arIndex.GetAt(ii);
				if (!ii)
					menu = (CMenuXP *) m_pMenu->GetSubMenu(index);
				else	menu = (CMenuXP *) menu->GetSubMenu(index);
			}
		}
		else	menu = m_pMenu;

		if (!menu)	menu = m_pMenu;
		itemCnt = menu->GetMenuItemCount();
		for (int ii = 0; ii < itemCnt; ii++)
		{
			menu->GetMenuItemInfo(ii, &info, TRUE);
			menu->GetMenuString(ii, str, MF_BYPOSITION);
			if (!(info.fState & MFS_HILITE))
				continue;
			if (info.hSubMenu)
				m_arIndex.Add(ii);
			else
			{
				if (m_showMenuN == ITEMEXPAND)
					m_moveMenuN = 0;
				else
				{
					m_moveMenuN = m_showMenuN + 1;
					if (m_hideMenuN == -1)
					{
						if (m_moveMenuN >= m_arItemRect.GetSize())
							m_moveMenuN = 0;
					}
					else
					{
						if (m_moveMenuN >= m_hideMenuN)
							m_moveMenuN = ITEMEXPAND;
					}
				}
				m_arIndex.RemoveAll();
				m_parent->PostMessage(WM_CANCELMODE);
				PostMessage(WM_CHANGEPOPUP, 1, 0);
				PostMessage(WM_KEYDOWN, VK_DOWN, 0);
			}
			return;
		}

		if (m_showMenuN == ITEMEXPAND)
			m_moveMenuN = 0;
		else
		{
			m_moveMenuN = m_showMenuN + 1;
			if (m_hideMenuN == -1)
			{
				if (m_moveMenuN >= m_arItemRect.GetSize())
					m_moveMenuN = 0;
			}
			else
			{
				if (m_moveMenuN >= m_hideMenuN)
					m_moveMenuN = ITEMEXPAND;
			}
		}
		m_arIndex.RemoveAll();
		m_parent->PostMessage(WM_CANCELMODE);
		PostMessage(WM_CHANGEPOPUP, 1, 0);
		PostMessage(WM_KEYDOWN, VK_DOWN, 0);
	}
}

void CIMenu::HookSyskey()
{
	m_menuKeyOn = !m_menuKeyOn;
	if (m_menuKeyOn && m_arItemRect.GetSize())
	{
		SetFocus();
		const CPoint point = m_arItemRect.GetAt(0).CenterPoint();
		hoverMenu(point);
	}
	else	
		hoverMenu(CPoint(-100, -100));
}

void CIMenu::makeLinkFile()
{
	removeLinkFile();

	_itemM* item{};
	int	po{}, len{};
	const int datL = 1024 * 60;
	CString	file, mapN, linkmap, dat, str, desc, key;
	char*	buffer = new char [datL];

	for (POSITION pos = m_arItem.GetStartPosition(); pos;)
	{
		m_arItem.GetNextAssoc(pos, mapN, (CObject *&) item);
		dat = item->info;
		dat.TrimLeft();
#if 0		// all link map
		if (item->isWeb)	continue;
		if (dat.IsEmpty())
		{
			file.Format("%s\\%s\\%s", m_home, MTBLDIR, mapN.Left(4));
			WritePrivateProfileSection(mapN, "", file);
			Sleep(0);
			continue;
		}
#else
		if (dat.IsEmpty() || item->isWeb)
			continue;
#endif

		len = 0;
		ZeroMemory(buffer, datL);
		file.Format("%s\\%s\\%s", Axis::home, MTBLDIR, mapN.Left(4));
		while (!dat.IsEmpty())
		{
			po = dat.Find(LINKSPLIT);
			if (po == -1)
			{
				str = dat;
				dat.Empty();
			}
			else
			{
				str = dat.Left(po++);
				dat = dat.Mid(po);
			}

			str.TrimLeft();	str.TrimRight();
			if (str.GetLength() != L_LINK)
				continue;

			key = GetMap(str);
			if (!ExistMenu(key))
				continue;
#ifdef LINKMAP_BY_DISPNUMBER
			linkmap = GetMap(str);
			desc.Format("%s=%s %s", key, str, GetDesc(linkmap));
#else // linkmap_by_mapsource_number
			linkmap.Format("%s%s00", mapN.Left(2), str);
			desc.Format("%s=%s %s", key, GetMapping(linkmap).Mid(2, 4), GetDesc(linkmap));
#endif
			CopyMemory(&buffer[len], desc, desc.GetLength());
			len += desc.GetLength() + 1;
		}
		WritePrivateProfileSection(mapN, buffer, file);
		Sleep(0);
	}
	delete [] buffer;
}

void CIMenu::removeLinkFile()
{
	CString		str, file, userN;
	HANDLE		hFind{};
	WIN32_FIND_DATA FindFileData{};

	CWinApp* app = AfxGetApp();
	userN = app->GetProfileString(WORKSTATION, SIGNONID);
	const int pos = userN.Find('|');
	if (pos < 0)	userN = _T("Anonymous");
	else		userN = userN.Mid(pos+1);
	if (userN.IsEmpty())	userN = _T("Anonymous");


	file.Format("%s\\%s\\*", Axis::home, MTBLDIR);
	hFind = FindFirstFile(file, &FindFileData);
	while (hFind != INVALID_HANDLE_VALUE)
	{
		str.Format("%s", FindFileData.cFileName);
		if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (str.Find('.') == -1 && str.GetLength() == 4 && str.Find("IB") == -1)
			{
				file.Format("%s\\%s\\%s", Axis::home, MTBLDIR, str);
				DeleteFile(file);
			}
		}
		if (!FindNextFile(hFind, &FindFileData))
			break;
	}
}

BOOL CIMenu::ExistMenu(CString mapN)
{
#ifdef _DEBUG
	return TRUE;
#endif	

	_itemM*	item;
	if (m_arItem.Lookup(mapN, (CObject *&) item))
		return TRUE;
	if (m_arItem.Lookup(mapN.Left(6)+"00", (CObject *&) item))
		return TRUE;

	return FALSE;
}

BOOL CIMenu::IsWeb(CString mapN)
{
	CString	key;
	_itemM*	item;

	/**
	if (mapN.Find("WWW#") == -1)
		key.Format("WWW#%s", mapN.Mid(2, 4));
	else	
		key = mapN;
	**/
	key = mapN;
	
	if (m_arItem.Lookup(key, (CObject *&) item))
	{
		if (item->isWeb)
			return TRUE;
	}
	return FALSE;
}

BOOL CIMenu::IsMappinged(CString mapN)
{
	CString	value;
	if (m_maplist.Lookup(mapN, value))
		return TRUE;
	return FALSE;
}

BOOL CIMenu::GetSelectList(CString input, CStringArray& array)
{
	array.RemoveAll();
	if (m_arItem.GetCount() <= 0)	return FALSE;

	_itemM* item{};
	CString	key;

	CMapStringToPtr mapArray;

	for (POSITION pos = m_arItem.GetStartPosition(); pos; )
	{
		m_arItem.GetNextAssoc(pos, key, (CObject *&) item);
		const int	po = item->disp.Find(input);
		if (po == -1)
		{
			if(item->menu2 != "")
			{
				if(item->menu2.Find(input) == -1)
				{
					if(item->menu1.Find(input) == -1)
						continue;
				}
			}
			else
			{
				continue;
			}
		}

		CString strTitle;
		
		strTitle.Format("%s > ",item->menu1);

		if(item->menu2 != "")
			strTitle.Format("%s%s > ",strTitle,item->menu2);

		strTitle += item->disp;

		//array.Add(item->disN + item->mapN + strTitle);
		CStringArray* arr;
		
		if(mapArray.Lookup(item->menu1,(void*&)arr))
		{
			arr->Add(item->disN + item->mapN + strTitle);
		}
		else
		{
			arr = new CStringArray;
			arr->Add(item->disN + item->mapN + strTitle);
			mapArray.SetAt(item->menu1,arr);
		}

// 		CString s;
// 		s.Format("VIEW TR : [%s] [%s] [%s]\n",item->menu1,item->menu2,item->disN);
// 		OutputDebugString(s);
	}

	CString tmp;
	CStringArray* arr;

	if(mapArray.Lookup("주식주문",(void *&)arr))
	{
		for(int i=0;i<arr->GetSize();i++)
		{
			array.Add(arr->GetAt(i));
		}
	}

	mapArray.RemoveKey("주식주문");

	for (POSITION pos1 = mapArray.GetStartPosition(); pos1; )
	{
		mapArray.GetNextAssoc(pos1, tmp, (void *&) arr);

		for(int i=0;i<arr->GetSize();i++)
		{
			array.Add(arr->GetAt(i));
		}
	}

	if (array.GetSize() > 0)
		return TRUE;
	return FALSE;
}

CString CIMenu::GetURL(CString mapN)
{
	CString	key;
	_itemM*	item;

	/**
	if (mapN.Find("WWW#") == -1)
		key.Format("WWW#%s", mapN.Mid(2, 4));
	else	
		key = mapN;
	**/
	key = mapN;

	if (m_arItem.Lookup(key, (CObject *&) item))
	{
		if (!item->isWeb)	
			return _T("");
		return item->info;
	}
	return _T("");
}

// return short name
CString CIMenu::GetSName(CString mapN)
{
	mapN = mapN.Mid(0, 6)+"00";

	_itemM*	item;
	if (m_arItem.Lookup(mapN, (CObject *&) item))
		return item->sName;
	
	if (mapN.GetLength() < L_MAPN)	return mapN;

	return mapN.Mid(2, 4);
}

CString CIMenu::GetDesc(CString mapN)
{
	if (mapN.GetLength() < L_MAPN)
		return "";

	mapN = mapN.Left(L_MAPN - 2) + "00";
	
	_itemM*	item;
	if (!m_arItem.Lookup(mapN, (CObject *&) item))
		return mapN;
	if (item->menu2.IsEmpty())
		return item->disp;
	else
		return item->menu2 + "::" + item->disp;

}

CString CIMenu::GetMenuName(CString mapN)
{
	if (mapN.GetLength() < L_MAPN)
		return "";

	mapN = mapN.Left(L_MAPN - 2) + "00";

	_itemM*  item;
	if (!m_arItem.Lookup(mapN, (CObject *&)item))
		return mapN;

	return item->disp;
}

// return etc infomation(link map infomation)
CString CIMenu::GetInfo(CString mapN)
{
	_itemM*	item;
	if (m_arItem.Lookup(mapN, (CObject *&) item))
		return item->info;
	return mapN;
}

CString CIMenu::GetMap(int index)
{
	if (m_arMap.GetSize() < index)
		return "";
	return m_arMap.GetAt(index);
}

// return fullmapname
CString CIMenu::GetMap(CString dispN)
{
	CString	mapN;
	if (!m_mapedlist.Lookup(dispN, mapN))
		mapN.Format("%s%s00", "00"/*m_hKey*/, dispN);
	return mapN;
}

// return mapping map
CString CIMenu::GetMapping(CString mapN)
{
	CString	value, mapname;
	if (m_maplist.Lookup(mapN, value))
	{
		if (value.GetLength() != 4)	return mapN;
		mapname.Format("%s%s00", mapN.Left(2), value);
		return mapname;
	}
	return mapN;
}

CString CIMenu::GetDispN(CString mapN)
{
	CString	dispN = mapN.Mid(2, 4);
	m_maplist.Lookup(mapN, dispN);
	
	return dispN;
}

CMenu* CIMenu::GetPopupMenu(int index)
{
	int	idx = index;
	if (index == -1)	// lastMenu
		idx = m_arMenuT.GetSize() - 1;
	return m_arMenuT.GetAt(idx);
}

void CIMenu::OnKillFocus(CWnd* pNewWnd) 
{
	if (m_menuKeyOn)	HookSyskey();
	CWnd::OnKillFocus(pNewWnd);
}

void CIMenu::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
#ifndef HNEOMENU
	HMENU hMenu = AfxGetThreadState()->m_hTrackingMenu;
	CMenu	*pMenu = CMenu::FromHandle(hMenu);
	pMenu->MeasureItem(lpMeasureItemStruct);
#endif
	CWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CIMenu::ShowFullMenu(CPoint pt)
{
	CString	str;
	int	menuN   = 0;
	const int	itemCnt = m_arMenuT.GetSize();
	if (!itemCnt)	return;
	
	CMenuXP* menu{}, * popup = new CMenuXP();
	popup->CreatePopupMenu();
	for (int ii = 0; ii < itemCnt; ii++)
	{
		menu = m_arMenuT.GetAt(ii);
		str  = m_arsMenuT.GetAt(ii);
		popup->InsertMenuX(menuN++, MF_POPUP|MF_BYPOSITION|MF_STRING, (UINT)menu->m_hMenu, str);
	}

	m_showPopup = true;
	m_menuKeyOn = true;
	m_pMenu = popup;
	if (pt.x <= 0 && GetSystemMetrics(80) < 2)	pt.x = 0;
	const int	lpos = GetScreenPos();
	if (pt.x < lpos) pt.x = lpos;
	popup->TrackPopupMenu(TPM_FLAG, pt.x, pt.y, m_parent);
#ifndef HNEOMENU
	while (popup->GetMenuItemCount())
		popup->RemoveMenu(0, MF_BYPOSITION);
	popup->DestroyMenu();
#else
	popup->Clear(false);
	while (popup->GetMenuItemCount())
		popup->RemoveMenu(0, MF_BYPOSITION);
	delete popup;
#endif
	drawSelectMenu(false);
	m_showPopup = false;
	m_menuKeyOn = false;
}

/*
 *	idx
 *	0 - left
 *	1 - top
 *	2 - right
 *	3 - bottom
 */
int CIMenu::GetScreenPos(int idx)
{
	int	nPos = 0;
	CRect	dualRc(0, 0, 0, 0);

	const int moniterN = GetSystemMetrics(SM_CMONITORS);
	if (moniterN <= 1)
	{
		GetDesktopWindow()->GetWindowRect(&dualRc);
		switch (idx)
		{
		case 0:	nPos = dualRc.left;	break;
		case 1:	nPos = dualRc.top;	break;
		case 2:	nPos = dualRc.right;	break;
		case 3:	nPos = dualRc.bottom;	break;
		}
		return nPos;
	}
#if 1
	g_monRc.SetRectEmpty();
	EnumDisplayMonitors(NULL, NULL, MyInfoEnumProc, 0);
	switch (idx)
	{
	case 0:	nPos = g_monRc.left;	break;
	case 1:	nPos = g_monRc.top;	break;
	case 2:	nPos = g_monRc.right;	break;
	case 3:	nPos = g_monRc.bottom;	break;
	}
#else
	RECT		mRc;
	MONITORINFO	lpmi;

	memset(&lpmi,0,sizeof(MONITORINFO));
	lpmi.cbSize = sizeof(MONITORINFO);
	mRc.left = mRc.top = mRc.right = mRc.bottom = 0;
	
	HMONITOR hMonitor = MonitorFromRect(&mRc, MONITOR_DEFAULTTOPRIMARY);
	GetMonitorInfo(hMonitor,  &lpmi);
	dualRc.left = lpmi.rcWork.left;		dualRc.top = lpmi.rcWork.top;
	dualRc.right = lpmi.rcWork.right;	dualRc.bottom= lpmi.rcWork.bottom;

	hMonitor = MonitorFromRect(&mRc, MONITOR_DEFAULTTONEAREST);
	GetMonitorInfo(hMonitor,  &lpmi);
	if (dualRc.left  >= lpmi.rcWork.left)	dualRc.left   = lpmi.rcWork.left;
	if (dualRc.top   >= lpmi.rcWork.top)	dualRc.top    = lpmi.rcWork.top;
	if (dualRc.right  < lpmi.rcWork.right)	dualRc.right  = lpmi.rcWork.right;
	if (dualRc.bottom < lpmi.rcWork.bottom)	dualRc.bottom = lpmi.rcWork.bottom;

	switch (idx)
	{
	case 0:	nPos = dualRc.left;	break;
	case 1:	nPos = dualRc.top;	break;
	case 2:	nPos = dualRc.right;	break;
	case 3:	nPos = dualRc.bottom;	break;
	}
#endif
	return nPos;
}

void CIMenu::DrawGradient(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight)
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
			pDC->SelectObject(sPen);
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
				pDC->SelectObject(sPen);
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
			pDC->SelectObject(sPen);
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
				pDC->SelectObject(sPen);
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

void CIMenu::drawTerm(CDC* dc, CRect rc)
{
	return;
	if (m_bitmapMenuTerminator.m_hObject)
	{
		BITMAP bm;
		m_bitmapMenuTerminator.GetBitmap(&bm);

		rc.left = rc.right - bm.bmWidth;

		CDC mdc;
		mdc.CreateCompatibleDC(dc);
		CBitmap* oldBmp = mdc.SelectObject(&m_bitmapMenuTerminator);

		dc->BitBlt(rc.left, 0, bm.bmWidth, bm.bmHeight, &mdc, 0, 0, SRCCOPY);
		mdc.SelectObject(oldBmp);
	}
}

int CIMenu::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	/**
	m_szMenuMode = CSize(0, 0);
	CFileBitmap menuMode(m_home, "MenuMode");

	if (menuMode.m_hObject)
	{
		m_menuMode = new CNButton(IDC_MENUMODE);
		const DWORD	dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

		if (m_menuMode->Create(NULL, "Menu", dwStyle, CRect(0, 0, 0, 0), this, IDC_MENUMODE))
		{
			//m_menuMode->set_ToolTipText("메뉴를 잠시 숨기거나 보이게 합니다.");
			CAxButtonName bn(m_home, "MenuMode");
			bn.m_down = bn.m_hover = bn.m_normal;
			m_menuMode->set_Image(bn);
			m_szMenuMode = m_menuMode->get_ImgSize();

			m_menuMode->BringWindowToTop();
		}
		else
		{
			delete m_menuMode;
			m_menuMode = NULL;
		}
	}
	else
	{
		m_menuMode = NULL;
	}
	**/
	//	툴바 숨기기
	CProfile profile(pkAxisTicker);

	m_nShow = profile.GetInt("General", "view1", 1);

	if(!m_nShow)
		m_moveBar = CreateNButton(CTRL_UPDN, "툴바 보이기/숨기기", "tool_down", FALSE);
	else
		m_moveBar = CreateNButton(CTRL_UPDN, "툴바 보이기/숨기기", "tool_up", FALSE);

#ifdef DF_MK_CAPTION
	char redB[32]{};
	int redL;
	CString file, stmp;
	file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, "userconf.ini");
	WritePrivateProfileString("MKgubn", "Market", "통합", file);
	stmp.Format("TMENU_MARKET_%s", "통합");
	stmp.TrimRight();

	m_btSelectMarket = CreateNButton(CTRL_MARKET, "거래소 선택", stmp, FALSE);
	//m_btSelectMarket->ShowWindow(SW_HIDE);
#endif
	return 0;
}

void CIMenu::OnSize(UINT nType, int cx, int cy) 
{
	/**
	if (m_menuMode)
	{
		CRect rc;
		GetClientRect(&rc);

		rc.left = cx - m_szMenuMode.cx - 5;
		rc.right = cx - 5;

		rc.top = cy - m_szMenuMode.cy;
		rc.bottom = cy;

		m_menuMode->MoveWindow(rc);
	}
	**/
#ifdef DF_MK_CAPTION
	CRect rcmovebar;
	if(m_moveBar)
	{
		rcmovebar = GetToolbarHideRect();
		m_moveBar->MoveWindow(rcmovebar, FALSE);
	}

	if (m_btSelectMarket)
	{
		m_rcSelectMarket = rcmovebar;
		m_rcSelectMarket.top = rcmovebar.top - 1 ;
		m_rcSelectMarket.right = m_rcSelectMarket.left; // 왼쪽 끝에 맞춤
		m_rcSelectMarket.left -=45;                   // 버튼 폭만큼 왼쪽으로
		m_rcSelectMarket.bottom = m_rcSelectMarket.top + 15; // 버튼 높이 지정

		m_rcSelectMarket.OffsetRect(-5, 0);
		m_btSelectMarket->MoveWindow(m_rcSelectMarket, FALSE);
	}
#else
	if (m_moveBar)
	{
		m_moveBar->MoveWindow(GetToolbarHideRect(), FALSE);
	}
#endif
	CWnd::OnSize(nType, cx, cy);
}

void CIMenu::OnDestroy() 
{
	XMSG();
	CWnd::OnDestroy();
	
	//**DELETE_WND(m_menuMode);	
}

CRect CIMenu::GetMenuModeRect()
{
	CRect rc;
	GetClientRect (&rc);

	CRect result(0, 0, 0, 0);

	const int RIGHT_MARGIN = 3;
	if (m_bmpMenumode[BMS_UP][BMS_NORMAL])
	{
		result.left = rc.Width() - RIGHT_MARGIN - m_szMenuMode.cx;
		result.top = (rc.Height() - m_szMenuMode.cy)/2;

		result.right = result.left + m_szMenuMode.cx;
		result.bottom = rc.Height()-result.top;
	}
	return result;
}

CRect CIMenu::GetToolbarHideRect()
{
	CRect rc;
	GetClientRect (&rc);
	
	CRect result(0, 0, 0, 0);
	
	const int RIGHT_MARGIN = 3;
	result.left = rc.Width() - RIGHT_MARGIN - m_szMenuMode.cx - RIGHT_MARGIN - 15;
	result.top = (rc.Height() - m_szMenuMode.cy)/2;
	
	result.right = result.left + 15;
	result.bottom = rc.Height()-result.top;
#ifdef DF_MK_CAPTION
	//result.OffsetRect(20, 0);
#endif

	return result;
}

CRect CIMenu::GetXComRect()
{
	CRect rc;
	GetClientRect (&rc);
	
	CRect result(0, 0, 0, 0);
	
	const int RIGHT_MARGIN = 3;
	result.left = rc.Width() - RIGHT_MARGIN - m_szMenuMode.cx - RIGHT_MARGIN - 15 - RIGHT_MARGIN - 100;
	result.top = (rc.Height() - m_szMenuMode.cy)/2;
	
	result.right = result.left + 100;
	result.bottom = rc.Height()-result.top;
	
	return result;
}

CNButton* CIMenu::CreateNButton(unsigned int id, const char* title, const char* button, BOOL eachButton)
{
	CNButton*	btn = new CNButton(id);
	const DWORD	dwStyle = WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	
	if (!btn->Create(NULL, title, dwStyle, CRect(0,0,0,0), this, -1))
	{
		delete btn;
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
		
		return btn;
	}
}

void CIMenu::DrawMenuMode(CDC* dc)
{
#ifdef DF_MK_CAPTION
	//return; 
#endif
	const CRect rc(GetMenuModeRect());
	if (!rc.left)
		return;

	int updown{}, hover{};
	if (m_menuMode)
		updown = BMS_DOWN;
	else
		updown = BMS_UP;
	if (m_menuModeHover)
		hover = BMS_HOVER;
	else
		hover = BMS_NORMAL;

	if (m_bmpMenumode[updown][hover] == FALSE)
		return;

	CDC mdc;
	mdc.CreateCompatibleDC(dc);

	CBitmap* oldBmp = mdc.SelectObject(m_bmpMenumode[updown][hover]);
	dc->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &mdc, 0, 0, SRCCOPY);
	mdc.SelectObject(oldBmp);
}

#ifdef DF_MK_CAPTION
void CIMenu::ToggleMarketMenu()
{
	char redB[32]{};
	int redL;
	CString file, stmp;
	file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, "userconf.ini");
	redL = GetPrivateProfileString("MKgubn", "Market", "KRX", redB, sizeof(redB), file);
	stmp.Format("%s", redB);
	stmp.TrimRight();

	CMenu cMenu;

	cMenu.CreatePopupMenu();
	//const	char* const pDisplay[3] = { "KRX", "NXT", "통합" };
	const	char* const pDisplay[3] = { "통합","KRX", "NXT" };
	for (int ii = 1; ii < 4; ii++)
	{
		if (stmp == pDisplay[ii - 1])
			cMenu.AppendMenu(MFT_STRING | MF_CHECKED, ii, pDisplay[ii - 1]);
		else
			cMenu.AppendMenu(MFT_STRING, ii, pDisplay[ii - 1]);
		//	cMenu.AppendMenu(MFT_STRING, ii + 1, pDisplay[ii]);
	}

	CRect rec;
	m_btSelectMarket->GetWindowRect(rec);
	rec.OffsetRect(0, -10);

	int	ret = cMenu.TrackPopupMenu(TPM_RIGHTBUTTON | TPM_RETURNCMD, rec.left, rec.bottom + m_rcSelectMarket.Height(), this, NULL);

	if (ret < 1)
		return;

	switch (ret)
	{
	case 1:
		m_btSelectMarket->set_Image("TMENU_MARKET_통합");
		m_btSelectMarket->Invalidate();
		WritePrivateProfileString("MKgubn", "Market", "통합", file);
		break;
	case 2:
		m_btSelectMarket->set_Image("TMENU_MARKET_KRX");
		m_btSelectMarket->Invalidate();
		WritePrivateProfileString("MKgubn", "Market", "KRX", file);
		break;
	case 3:
		m_btSelectMarket->set_Image("TMENU_MARKET_NXT");
		m_btSelectMarket->Invalidate();
		WritePrivateProfileString("MKgubn", "Market", "NXT", file);
		break;
	}


	if (ret == 1)
		ret = 3;
	else if (ret == 2)
		ret = 1;
	else if (ret == 3)
		ret = 2;

	CWnd* pMain = AfxGetMainWnd();
	if (pMain)
		pMain->SendMessage(WM_USER, MAKEWPARAM(MMSG_MKMSG_FROM_SM, ret), ret); //lparam 은 딱히 필요없어서..
}
#endif

void CIMenu::ToggleMenuMode()
{
	CMenuXP	menu;
	CString skinName;
	const CRect rc(GetMenuModeRect());
	CPoint m_point;
	::GetCursorPos(&m_point);
	menu.CreatePopupMenu();

// 	const char* skins[] = {
// 		"Blue", "Green", "Brown", "Violet", "Gray", NULL
// 	};
	const char* skins[] = {
		"Blue", "Brown", "Gray", NULL
	};

	skinName = GetCurrentSkinName();

	if (!skinName.CompareNoCase("Green") || !skinName.CompareNoCase("Violet"))
	{
		skinName = "Blue";
	}

	int count = 0;
	for (int i = 0; skins[i]; i++)
	{
		menu.AppendMenuX(MF_STRING, i + 1, skins[i]);
		if (!skinName.CompareNoCase(skins[i]))
			menu.CheckMenuItem(i + 1, MF_CHECKED);
		count++;
	}

	const int nRtn = menu.TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, m_point.x, m_point.y, this);

	if (nRtn > 0 && nRtn <= count)
		skinName = skins[nRtn - 1];
	else
		return;

	change_Palette(skinName);
	//::SendMessage(m_parent->GetParent()->GetParent()->GetParent()->m_hWnd, WM_USER, 0, (long)(const char*)skinName);
	AfxGetMainWnd()->SendMessage(WM_AXIS, MAKEWPARAM(axSkinChange, 0), (long)(const char*)skinName);
	//(*m_axiscall)(AXI_CHANGEBAR2, 0, (long)(const char*)skinName);
	/*
	if (m_menuMode == MM_NORMAL)
		m_menuMode = MM_SMALL;
	else
		m_menuMode = MM_NORMAL;
	m_menuModeHover = FALSE;

	m_hover = -1;
	m_hideMenuN = -1;
	m_showMenuN = -1;
	m_moveMenuN = -1;
	m_mouseIN = false;
	m_showPopup = false;
	m_menuKeyOn = false;

	AfxGetMainWnd()->SendMessage(WM_AXIS, MAKEWPARAM(axMenuMode, m_menuMode), 0);
	*/
}

int CIMenu::GetSmallHeight()
{
	return m_szMenuMode.cy + 1;
}

void CIMenu::CheckMenuModeStatus(CPoint point)
{
	const CRect rc(GetMenuModeRect());

	if ((!m_menuModeHover && rc.PtInRect(point)) ||
		(m_menuModeHover && !rc.PtInRect(point)))
	{
		m_menuModeHover = !m_menuModeHover;
		CWindowDC dc(this);
		DrawMenuMode(&dc);
	}	
}

BOOL CIMenu::IsWebMenu(const char* menuName)
{
	CProfile profile(Format("%s\\%s\\WEBINFO.INI", Axis::home, TABDIR));
CString slog;
slog.Format("[axis][axissm][CIMenu] IsWebMenu menuName=[%s]\n", menuName);
//OutputDebugString(slog);
	return !profile.GetString(CString(menuName).Mid(2, 4), "Type").IsEmpty();
}


CString CIMenu::GetCurrentSkinName()
{
	CProfile profile(pkPalette);
	CString skin(profile.GetString(GENERALSN, szPalette));
	return skin;
}

void CIMenu::change_Palette(CString skinN)
{
	CProfile profile(pkPalette);
	profile.Write(GENERALSN, szPalette, skinN);
}

#pragma warning (default : 26409)
#pragma warning (default : 26401)
#pragma warning (default : 26400)
#pragma warning (default : 6011)