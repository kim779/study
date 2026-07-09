// iMenu.cpp : implementation file
//

#include "stdafx.h"
#include "iMenu.h"
#include "resource.h"
#include "../../h/axisvar.h"

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
#define	SEP		'-'
#define	MAP		'#'
#define	LINKSPLIT	';'
#define L_LINK		4
#define MGAP		14

//#define	BKCOLOR		RGB(220, 223, 232)
#define	BKCOLOR		RGB(76, 81, 90)   //메뉴의바탕색
//#define SELCOLOR	RGB(184, 192, 230) //RGB(190, 204, 240)
#define SELCOLOR	RGB(122, 130, 143) //RGB(190, 204, 240)  //메뉴에 마우스오버일때
#define	EXPANDWIDTH	13
#define	ITEMEXPAND	9999

//#define LINKMAP_BY_DISPNUMBER

#define clrTL	RGB(255, 255, 255)
//#define clrBR	RGB(148, 169, 227)
#define clrBR	RGB(76, 81, 90)

CIMenu*		iMenu = NULL;
static CRect	g_monRc(0, 0, 0, 0);
static HHOOK	gHook = NULL;

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
		MSG	*msg = (MSG *) lParam;
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

CIMenu::CIMenu(CWnd* parent, CString home, int commandBase)
{
	m_parent = parent;
	m_home   = home;
	m_commandBase = commandBase;
	m_commandCnt  = 0;
	m_hover = -1;
	m_hideMenuN = -1;
	m_showMenuN = -1;
	m_moveMenuN = -1;
	m_mouseIN = false;
	m_showPopup = false;
	m_menuKeyOn = false;

	NONCLIENTMETRICS info; info.cbSize = sizeof(info);
	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
//	m_font.CreateFontIndirect(&info.lfMenuFont);		// vista
	CString	fontName;
	fontName.LoadString(STSM_FONTNAME_DEFAULT);
//	m_font.CreatePointFont(90, fontName);
	m_font.CreateFont(12, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, fontName);

	for (int ii = 0; ii < MAX; ii++)
	{
		m_popM[ii] = NULL;
		m_menuN[ii].Empty();
	}
	m_pMenu = NULL;

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

	m_back.LoadBitmap(IDB_MENUBK);
}

CIMenu::~CIMenu()
{
	int ii = 0;
	iMenu = NULL;
	if (gHook) ::UnhookWindowsHookEx(gHook);
#ifndef HNEOMENU
	for (ii = 0; ii < m_arMenu.GetSize(); ii++)
		delete m_arMenu.GetAt(ii);
	for (ii = 0; ii < m_arFrameMenu.GetSize(); ii++)
		delete m_arFrameMenu.GetAt(ii);
#else
	for (ii = 0; ii < m_arMenuT.GetSize(); ii++)
		delete m_arMenuT.GetAt(ii);
	for (ii = 0; ii < m_arFrameMenu.GetSize(); ii++)
		delete m_arFrameMenu.GetAt(ii);
#endif
	_itemM*	item;
	CString	key;
	for (POSITION pos = m_arItem.GetStartPosition(); pos; )
	{
		m_arItem.GetNextAssoc(pos, key, (CObject *&) item);
		delete item;
	}

	m_back.DeleteObject();
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
}

BEGIN_MESSAGE_MAP(CIMenu, CWnd)
	//{{AFX_MSG_MAP(CIMenu)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_WM_MEASUREITEM()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_CHANGEPOPUP, OnChangePopup)
	ON_MESSAGE(WM_SELECTMENU, OnSelectMenu)
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
	CWnd*			wndUnderMouse = NULL;
	CWnd*			wndActive = this;
	TRACKMOUSEEVENT		csTME;

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
		hoverMenu(lpoint);
	}
	else 	m_mouseIN = false;
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
	CMenuXP*	menu;
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
			int	menuN = 0, itemCnt = (int)m_arItemRect.GetSize();
			UINT	nFlags = MF_POPUP|MF_BYPOSITION|MF_STRING;
			CString	str;

			CMenuXP*	popup = new CMenuXP();
			popup->CreatePopupMenu();
			for (int ii = m_hideMenuN; ii < itemCnt; ii++)
			{
				menu = m_arMenuT.GetAt(ii);
				str  = m_arsMenuT.GetAt(ii);
				popup->InsertMenuX(menuN++, nFlags, (UINT)menu->m_hMenu, str);
			}
			point = CPoint(m_expandRc.left - 1, m_expandRc.bottom + 1);
			ClientToScreen(&point);
			m_pMenu = popup;
			//if (point.x <= 0 && GetSystemMetrics(80) < 2)	point.x = 0;
			int	lpos = GetScreenPos();
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
			CRect iRc = m_arItemRect.GetAt(m_showMenuN);
			point = CPoint(iRc.left - 1, iRc.bottom + 1);
			ClientToScreen(&point);
			m_pMenu = menu;
			//if (point.x <= 0 && GetSystemMetrics(80) < 2)	point.x = 0;
			int	lpos = GetScreenPos();
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
	CRect	iRc = m_arItemRect.GetAt(m_showMenuN);
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

	int itemCnt = (int)m_arsMenuT.GetSize();
	if (itemCnt <= 0)	return;

	CSize	size;
	CString	text;
	CRect	cRc, iRc;
	CFont*	font = pDC->SelectObject(&m_font);
	GetClientRect(cRc);
	cRc.DeflateRect(0, 1);

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
			iRc.OffsetRect(iRc.Width() + 1, 0);
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
			if (iRc.right > cRc.right)
			{
				m_hideMenuN = ii;
				drawExpand(pDC);
				break;
			}
			m_hideMenuN = -1;
		}
		else
		{
			if (iRc.right+EXPANDWIDTH >= cRc.right)
			{
				m_hideMenuN = ii;
				drawExpand(pDC);
				break;
			}
		}

		pDC->SetTextColor(RGB(255, 255, 255));
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
//	BITMAP	bm;
	GetClientRect(cRc);
//	cRc.DeflateRect(0, 1);

/*	if (memDC.CreateCompatibleDC(pDC))
	{
		m_back.GetBitmap(&bm);
		CBitmap* pbmp = (CBitmap *) memDC.SelectObject(&m_back);
		pDC->StretchBlt(0, 0, cRc.Width(), cRc.Height(), &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		
		memDC.SelectObject(pbmp);
		memDC.DeleteDC();
	}
	else
*/	{
		pDC->SetBkColor(BKCOLOR);
		pDC->ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, &cRc, "", 0, NULL);
	}
}

void CIMenu::drawExpand(CDC* pDC, int mode)
{
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;
	CRect		cRc, eRc;

//	BITMAP	bm;
//	m_img[1].GetBitmap(&bm);
//	int	x, width = bm.bmWidth/3;

	GetClientRect(cRc);
	cRc.DeflateRect(0, 1);
	eRc.SetRect(cRc.right - EXPANDWIDTH, cRc.top, cRc.right, cRc.bottom-1);
	m_expandRc.CopyRect(&eRc);
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
	pDC->DrawText(m_expandSYM, m_expandRc, DT_CENTER|DT_TOP|DT_SINGLELINE);
/*	
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
	}*/
}

void CIMenu::drawSelectMenu(bool select)
{
	int	nItem = m_showMenuN;
	if (m_showMenuN == -1)
	{
		if (m_hover == -1)	return;
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
	dc.SetTextColor(RGB(255, 255, 255));

	if (select)
	{
		dc.FillSolidRect(iRc.left, cRc.top, iRc.right, cRc.bottom-1, RGB(122, 130, 143) );  //마우스클릭시
		dc.Draw3dRect(iRc.left, cRc.top, iRc.right, cRc.bottom-1, clrBR, clrTL);

		iRc.OffsetRect(1, 1);
		if (!str.Compare(m_expandSYM))
			dc.DrawText(str, iRc, DT_CENTER|DT_TOP|DT_SINGLELINE);
		else	dc.DrawText(str, iRc, DT_FLAG);
	}
	else
	{
		dc.FillSolidRect(iRc, BKCOLOR);
		if (!str.Compare(m_expandSYM))
			dc.DrawText(str, iRc, DT_CENTER|DT_TOP|DT_SINGLELINE);
		else	dc.DrawText(str, iRc, DT_FLAG);
	}

	dc.SelectClipRgn(NULL);
	dc.SelectObject(font);

	clipRgn.DeleteObject();
}

BOOL CIMenu::HookKeyboard(WPARAM wParam, LPARAM lParam)
{
	CPoint	point;
	int	newhover;
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
						newhover = (int)m_arItemRect.GetSize() - 1;
				}
				else
				{
					if (newhover < 0)
						newhover = ITEMEXPAND;
				}
			}

			if (newhover == ITEMEXPAND)
				point = m_expandRc.CenterPoint();
			else	point = m_arItemRect.GetAt(newhover).CenterPoint();
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
			else	point = m_arItemRect.GetAt(newhover).CenterPoint();
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

/*
	TCHAR* pbuf = new TCHAR[128];
	memset(pbuf, 0, sizeof(TCHAR) * 128);
	__try  
	{
		CopyMemory(pbuf, (TCHAR*)pdata, sizeof(TCHAR) * 128);
		delete pbuf;
	}
	 __except(EXCEPTION_EXECUTE_HANDLER)  
	{
		return FALSE;
	}

	return TRUE;
	*/
void CIMenu::HookProcess(CPoint point)
{
	 if (!(GetParent()->GetStyle() & WS_VISIBLE))
			return;

	if (m_showMenuN == -1)	return;

	if (m_curPoint.x == point.x && m_curPoint.y == point.y)
		return;
	CWnd* wndUnderMouse = WindowFromPoint(point);
	if (wndUnderMouse != this)	return;

	m_curPoint = point;
	ScreenToClient(&point);

	CRect	iRc, cRc;
	int	newshowMenuN = -1;
	int	itemCnt = (int)m_arItemRect.GetSize();

	GetClientRect(cRc);
	cRc.DeflateRect(0, 1);
	for (int ii = 0; ii < itemCnt; ii++)
	{
		iRc = m_arItemRect.GetAt(ii);
		if (ii == itemCnt-1)
		{
			if (iRc.right > cRc.right)
				break;
		}
		else
		{
			if (iRc.right+EXPANDWIDTH >= cRc.right)
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
	CMenuXP	frameMenu, *menu, *subMenu;
	if (!frameMenu.LoadMenu(MAKEINTRESOURCE(nIDResource)))
	{
		TRACE(_T("Failed to load menu\n"));
		return;
	}

	int	menuCnt, subCnt, nID;
	UINT	nFlags = MF_POPUP|MF_BYPOSITION|MF_STRING;
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
					CMenuXP	*subpopup, *popup = new CMenuXP();
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
	CString	file, dat;
	char	buf[1024*2];
	file.Format("%s\\%s\\%s", m_home, TABDIR, AXISMENU);

	CFileFind find;
	if (!find.FindFile(file))
	{
		file.Format("%s\\%s\\%s", m_home, MTBLDIR, AXISMENU);
	}

	m_commandCnt = 0;

	FILE *fp = NULL;
	errno_t err = fopen_s(&fp, file, "r");
	
	if (!fp)
	{
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

void CIMenu::makeMenu(CString dat)
{
	CString strdata;
	int	tmpI, menuN, step = 0;
	BOOL	map, sep;
	UINT	nFlags = MF_POPUP|MF_BYPOSITION|MF_STRING;
	CString	str, dispN, tmpS;

	CMenuXP*	popM;
	map = sep = FALSE;
	while (!dat.IsEmpty())
	{
		int pos = dat.Find(SPLIT);
		if (pos == -1)	break;

		str = dat.Left(pos++);
		dat = dat.Mid(pos);
		if (str.IsEmpty())	continue;

		char	ch = str.GetAt(0);
		switch (ch)
		{
		case SEP:	
			sep = TRUE;	
			break;
		case MAP:	
			map = TRUE;	
			break;
		default:			
			break;
		}
		if (sep || map)	
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
			tmpI = (int)m_arMenuT.GetSize();

TRACE(str + _T("\n"));
			/*
			if (tmpI >= 2)
			{
				m_arMenuT.InsertAt(tmpI -1, popM);
				m_arsMenuT.InsertAt(tmpI-1, str);
			}
			else
			{
				m_arMenuT.Add(popM);
				m_arsMenuT.Add(str);
			}
			*/
			m_arMenuT.Add(popM);
			m_arsMenuT.Add(str);


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
				tmpS = saveinfo(str, dat);
				if (tmpS.IsEmpty())	return;
				dispN.Format("%s %s", tmpS, str);

				m_popM[step-1]->AppendMenuX(MF_STRING|MF_ENABLED,
					m_commandBase + m_commandCnt++, dispN);
				return;
			}

			if (!m_menuN[step].Compare(str))
			{
				step++;
				continue;
			}

			popM = new CMenuXP();
			popM->CreatePopupMenu();
			m_popM[step-1]->InsertMenuX(menuN, nFlags, (UINT) popM->m_hMenu, str);
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

CString CIMenu::saveinfo(CString desc, CString dat)
{
	CString	key, str;
	_itemM*	item = new _itemM();
TRACE(desc + _T("\n"));
	int	idx = 0;
	for (int ii = 0; ii < 4; ii++)
	{
		if (!dat.IsEmpty())
		{
			int pos = dat.Find(SPLIT);
			int pos2 = dat.Find(SPLIT2);
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
		return _T("");
	}

	if (!item->mapN.Left(4).Compare(WEB))
		item->isWeb = TRUE;
	
	if (item->disN.IsEmpty())
	{
		if (item->isWeb)
			item->disN = item->mapN.Mid(4, 4);
		else	item->disN = item->mapN.Mid(2, 4);
	}

	if (item->sName.IsEmpty())
	{
		if (item->isWeb)
			item->sName = item->mapN.Mid(4, 4);
		else	item->sName = item->disN;
	}

	_itemM* xitem = NULL;
	if (m_arItem.Lookup(item->mapN, (CObject *&) xitem))
	{	//	중복된 메뉴...........
		m_arMap.Add(xitem->mapN);
		delete item;
		return xitem->disN;
	}

	m_arItem.SetAt(item->mapN, (CObject *&) item);
	m_arMap.Add(item->mapN);

	m_maplist.SetAt(item->mapN, item->disN);
	m_mapedlist.SetAt(item->disN, item->mapN);
	return item->disN;
}

void CIMenu::hoverMenu(CPoint point)
{
	int	itemCnt = (int)m_arsMenuT.GetSize();
	if (!itemCnt)		return;
	int	newhover = -1;
	CRect	cRc, iRc;
	GetClientRect(cRc);

	cRc.DeflateRect(0, 1);
	for (int ii = 0; ii < m_arItemRect.GetSize(); ii++)
	{
		iRc = m_arItemRect.GetAt(ii);
		if (ii == itemCnt-1)
		{
			if (iRc.right > cRc.right)
				break;
		}
		else
		{
			if (iRc.right+EXPANDWIDTH >= cRc.right)
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

	if (newhover == m_hover)	return;

	CString	str;
	CDC*	pDC = GetDC();
	CFont*	font = pDC->SelectObject(&m_font);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255, 255, 255));

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
			{
				pDC->FillSolidRect(iRc, SELCOLOR);
				pDC->Draw3dRect(iRc, clrBR, clrTL);
				pDC->DrawText(str, iRc, DT_FLAG);
			}
			else
			{
				pDC->FillSolidRect(iRc, SELCOLOR);
				pDC->Draw3dRect(iRc, clrTL, clrBR);
				pDC->DrawText(str, iRc, DT_FLAG);
			}
		}
	}

	pDC->SelectObject(font);
	ReleaseDC(pDC);
}

void CIMenu::processButtonDown(CPoint point)
{
	CRect	iRc, cRc;
	int	selectMenuN = -1;
	int	itemCnt = (int)m_arItemRect.GetSize();

	GetClientRect(cRc);
	cRc.DeflateRect(0, 1);
	for (int ii = 0; ii < itemCnt; ii++)
	{
		iRc = m_arItemRect.GetAt(ii);
		if (ii == itemCnt-1)
		{
			if (iRc.right > cRc.right)
				break;
		}
		else
		{
			if (iRc.right+EXPANDWIDTH >= cRc.right)
				break;
		}

		if (iRc.PtInRect((point)))
		{
			CPoint	point(iRc.left - 1, iRc.bottom + 1);
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
			int	lpos = GetScreenPos();
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
		CMenuXP	*menu, *popup = new CMenuXP();
		popup->CreatePopupMenu();
		for (int ii = m_hideMenuN; ii < itemCnt; ii++)
		{
			menu = m_arMenuT.GetAt(ii);
			str  = m_arsMenuT.GetAt(ii);
			popup->InsertMenuX(menuN++, MF_POPUP|MF_BYPOSITION|MF_STRING, (UINT)menu->m_hMenu, str);
		}

		point = CPoint(m_expandRc.left - 1, m_expandRc.bottom + 1);
		ClientToScreen(&point);

		m_showPopup = true;
		m_menuKeyOn = true;
		m_pMenu = popup;
		//if (point.x <= 0 && GetSystemMetrics(80) < 2)	point.x = 0;
		int	lpos = GetScreenPos();
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
	int		itemCnt;
	MENUITEMINFO	info;
	CString		str;
	
	info.cbSize = sizeof (MENUITEMINFO);
	info.fMask = MIIM_STATE|MIIM_SUBMENU;

	if (key == VK_LEFT)
	{
		itemCnt = (int)m_arIndex.GetSize();
		if (itemCnt)
		{
			for (int ii = 0; ii < itemCnt; ii++)
			{
				int index = m_arIndex.GetAt(ii);
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
						m_moveMenuN = (int)m_arItemRect.GetSize() - 1;
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
					m_moveMenuN = (int)m_arItemRect.GetSize() - 1;
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
		itemCnt = (int)m_arIndex.GetSize();
		if (itemCnt)
		{
			for (int ii = 0; ii < itemCnt; ii++)
			{
				int index = m_arIndex.GetAt(ii);
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
		CPoint point = m_arItemRect.GetAt(0).CenterPoint();
		hoverMenu(point);
	}
	else	hoverMenu(CPoint(-100, -100));
}

void CIMenu::makeLinkFile()
{
	removeLinkFile();

	_itemM*	item;
	int	po, len, datL = 1024*60;
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
		file.Format("%s\\%s\\%s", m_home, MTBLDIR, mapN.Left(4));
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
	HANDLE		hFind;
	WIN32_FIND_DATA FindFileData;

	CWinApp* app = AfxGetApp();
	userN = app->GetProfileString(WORKSTATION, SIGNONID);
	int pos = userN.Find('|');
	if (pos < 0)	
		userN = _T("Anonymous");
	else		
		userN = userN.Mid(pos+1);

	if (userN.IsEmpty())	
		userN = _T("Anonymous");


	file.Format("%s\\%s\\*", m_home, MTBLDIR);
	hFind = FindFirstFile(file, &FindFileData);
	while (hFind != INVALID_HANDLE_VALUE)
	{
		str.Format("%s", FindFileData.cFileName);
		if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (str.Find('.') == -1 && str.GetLength() == 4)
			{
				file.Format("%s\\%s\\%s", m_home, MTBLDIR, str);
				DeleteFile(file);
			}
		}
		if (!FindNextFile(hFind, &FindFileData))
			break;
	}
}

BOOL CIMenu::ExistMenu(CString mapN)
{
/*
#ifdef _DEBUG
	return TRUE;
#endif	
*/
	_itemM*	item;
	if (m_arItem.Lookup(mapN, (CObject *&) item))
		return TRUE;
	return FALSE;
}

BOOL CIMenu::IsWeb(CString mapN)
{
	CString	key;
	_itemM*	item;

	if (mapN.Find("WWW#") == -1)
		key.Format("WWW#%s", mapN.Mid(2, 4));
	else	key = mapN;
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

	_itemM*	item;
	CString	key;
	for (POSITION pos = m_arItem.GetStartPosition(); pos; )
	{
		m_arItem.GetNextAssoc(pos, key, (CObject *&) item);
		int	po = item->disp.Find(input);
		if (po == -1)	continue;
		array.Add(item->disN + item->mapN + item->disp);
	}

	if (array.GetSize() > 0)
		return TRUE;
	return FALSE;
}

CString CIMenu::GetURL(CString mapN)
{
	CString	key;
	_itemM*	item;

	if (mapN.Find("WWW#") == -1)
		key.Format("WWW#%s", mapN.Mid(2, 4));
	else	key = mapN;
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
	_itemM*	item;
	if (m_arItem.Lookup(mapN, (CObject *&) item))
		return item->sName;
	if (mapN.GetLength() < L_MAPN)	return mapN;
	return mapN.Mid(2, 4);
}

CString CIMenu::GetDesc(CString mapN)
{
	_itemM*	item;
	if (m_arItem.Lookup(mapN, (CObject *&) item))
		return item->disp;
	return mapN;
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
		mapN.Format("%s%s00", "AX"/*m_hKey*/, dispN);
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
		idx = (int)m_arMenuT.GetSize() - 1;
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

void CIMenu::ShowFullMenu(CPoint pt, bool bTray)
{
	CString	str;
	int	menuN   = 0;
	int	itemCnt = (int)m_arMenuT.GetSize();
	if (!itemCnt)	return;
	
	CMenuXP	*menu, *popup = new CMenuXP();
	popup->CreatePopupMenu();
	for (int ii = 0; ii < itemCnt; ii++)
	{
		menu = m_arMenuT.GetAt(ii);
		str  = m_arsMenuT.GetAt(ii);
		popup->InsertMenuX(menuN++, MF_POPUP|MF_BYPOSITION|MF_STRING, (UINT)menu->m_hMenu, str);
	}

	if (bTray)
		popup->InsertMenuX(menuN++, MF_BYPOSITION|MF_STRING, NULL, "TRAY메뉴닫기");

	m_showPopup = true;
	m_menuKeyOn = true;
	m_pMenu = popup;
	if (pt.x <= 0 && GetSystemMetrics(80) < 2)	pt.x = 0;
	int	lpos = GetScreenPos();
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

	int moniterN = GetSystemMetrics(SM_CMONITORS);
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
