// Linkbar.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "Linkbar.h"
#include "axMsg.hxx"
#include "../h/axisvar.h"
#include "../dll/axissm/NButton.h"
#include "../dll/axissm/MenuXP.h"
#include "../dll/axissm/smheader.h"

#include <algorithm>
#include <afxpriv.h>

#define ITEM_COOL	0
#define ITEM_OVER	1
#define ITEM_DOWN	2
#define LINKEXPAND	999

#define COLOR_ITEMTEXT	RGB(  50, 50, 50)
#define SRCMASK		0x00220326    // mask

#define	ID_TOOLTIP	1
#define HGAP		2
#define BTNW		24
#define BTNH		20
#define BTNWGAP		10

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CStaticCmdUI : public CCmdUI      
{
public: 
	virtual void SetTextX(LPCTSTR lpszText) {}
};

/////////////////////////////////////////////////////////////////////////////
// CLinkbar

CLinkbar::CLinkbar(class CTMenu* menu, CString mapN, bool (*axiscall)(int, WPARAM, LPARAM), CString home, CString userName, CString skinName)
{
	m_tmenu		= menu;
	m_mapname	= mapN.Left(L_MAPN);
	m_axiscall	= axiscall;
	m_home		= home;
	m_userName	= userName;
	m_itemW		= 0;
	m_itemH		= 0;
	m_expandW	= 0;
	m_hover		= -1;
	m_push		= -1;
	m_pushON	= false;
	m_mouseOver	= false;
	m_ToolTip	= NULL;
	m_linkEdit	= NULL;

	CString	file;

	file.Format("%s\\image\\linkm.bmp", m_home);
	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	if (hBitmap)	m_bitmap.Attach(hBitmap);

	file.Format("%s\\image\\linkitem.bmp", m_home);
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)
	{
		BITMAP	bm;

		m_imgB.Attach(hBitmap);
		m_imgB.GetBitmap(&bm);
		m_itemW = bm.bmWidth/3;
		m_itemH = bm.bmHeight;
	}

	file.Format("%s\\image\\linkexpand.bmp", m_home);
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)
	{
		BITMAP	bm;

		m_exImg.Attach(hBitmap);
		m_exImg.GetBitmap(&bm);
		m_expandW = bm.bmWidth/3;
	}
	m_patbrush.CreatePatternBrush(&m_bitmap);

	m_font.CreatePointFont(90, "굴림");
}

CLinkbar::~CLinkbar()
{
	removeAry();

	m_font.DeleteObject();
	if (m_linkEdit)			delete m_linkEdit;
	if (m_imgB.GetSafeHandle())	m_imgB.DeleteObject();
	if (m_exImg.GetSafeHandle())	m_exImg.DeleteObject();
	if (m_bitmap.GetSafeHandle())	m_bitmap.DeleteObject();
	if (m_patbrush.GetSafeHandle())	m_patbrush.DeleteObject();
}


BEGIN_MESSAGE_MAP(CLinkbar, CControlBar)
	//{{AFX_MSG_MAP(CLinkbar)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SIZEPARENT, OnSizeParent)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_XMSG, OnXMSG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLinkbar message handlers
LRESULT CLinkbar::OnSizeParent(WPARAM, LPARAM lParam)
{
	AFX_SIZEPARENTPARAMS* lpLayout = (AFX_SIZEPARENTPARAMS*)lParam;
	return CControlBar::OnSizeParent(0, lParam);
}

void CLinkbar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CStaticCmdUI state;
	state.m_pOther = this;
	state.m_nIndexMax = 1;		// there's only one thing to update
	state.m_nID = AFX_IDW_STATUS_BAR;

	// allow the statusbar itself to have update handlers
	if (CWnd::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
		return;

	// allow target (owner) to handle the remaining updates
	state.DoUpdate(pTarget, FALSE);
}

CSize CLinkbar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	// The Y value is the sum of the calculated height from DrawText,
	// plus the top and bottom border.
	CSize size;
	size.cx = 32767;
	size.cy = LBAR_HEIGHT;

	return size;
}

void CLinkbar::InitToolTip()
{
	if (!m_ToolTip)
	{
		m_ToolTip = new CToolTipCtrl();
		m_ToolTip->Create(this, TTS_ALWAYSTIP);
		m_ToolTip->Activate(TRUE);
	}
}

void CLinkbar::SetToolTipText(CString text, BOOL bActivate)
{
	if (text.IsEmpty())	return;

	InitToolTip();
	if (m_ToolTip->GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip->AddTool(this, text, CRect(0, 0, 100, 100), ID_TOOLTIP);
	}

	m_ToolTip->UpdateTipText(text, this, ID_TOOLTIP);
	m_ToolTip->Activate(bActivate);
}

LRESULT CLinkbar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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
	return CControlBar::WindowProc(message, wParam, lParam);
}

void CLinkbar::OnPaint() 
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

		memDC.FillRect(cRc, &m_patbrush);

		drawLinkinfo(&memDC);
		dc.BitBlt(0, 0, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);

		memDC.SelectObject(pbmp);
		memDC.DeleteDC();
		bitmap.DeleteObject();
	}
}

void CLinkbar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CClientDC	dc(this);
	int		nItem = GetSelectItem(point);

	SetCapture();
	m_pushON = true;
	switch (nItem)
	{
	case -1:
		m_push   = -1;
		m_pushON = false;
		break;
	case LINKEXPAND:
		m_push = LINKEXPAND;
		draw_Expand(&dc, ITEM_DOWN);
		break;
	default:
		drawItem(&dc, nItem, ITEM_DOWN);
		m_push = nItem;
		break;
	}
	CControlBar::OnLButtonDown(nFlags, point);
}

void CLinkbar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();

	CClientDC	dc(this);
	int		nItem = GetSelectItem(point);
	
	switch (m_push)
	{
	case -1:	break;
	case LINKEXPAND:
		draw_Expand(&dc);
		if (nItem == m_push)
			showMenu();
	break;
	default:
		drawItem(&dc, m_push);
		if (nItem == m_push)
		{
			linkinfo*	item = m_arInfo.GetAt(nItem);
			GetParent()->SendMessage(WM_LINKOPEN, 0, (LPARAM)item->mapN.operator LPCTSTR());
		}
		break;
	}

	m_push = -1; 
	m_pushON = false;
	CControlBar::OnLButtonUp(nFlags, point);
}

void CLinkbar::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd*			wndUnderMouse = NULL;
	CWnd*			wndActive = this;
	TRACKMOUSEEVENT		csTME;

	CControlBar::OnMouseMove(nFlags, point);

	CPoint	lpoint = point;
	ClientToScreen(&point);
	wndUnderMouse = WindowFromPoint(point);
	if (wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd && wndActive)
	{
		if (!m_mouseOver)
		{
			m_mouseOver = true;

			csTME.cbSize = sizeof(csTME);
			csTME.dwFlags = TME_LEAVE;
			csTME.hwndTrack = m_hWnd;
			::_TrackMouseEvent(&csTME);
		}

		HoverItem(lpoint);
	}
	else 
		m_mouseOver = false;
}

LRESULT CLinkbar::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_mouseOver	= false;
	m_hover		= -1;
	Invalidate(FALSE);
	return 0;
}

LRESULT CLinkbar::OnXMSG(WPARAM wParam, LPARAM lParam)
{
	CString imgC, imgO, imgD;
	switch (LOWORD(wParam))
	{
	case CTRL_LINKEDIT:
		GetParent()->SendMessage(WM_LINKOPEN, 1, (LPARAM)m_mapname.operator LPCTSTR());
		break;
	default:
		break;
	}
	return 0;
}

void CLinkbar::OnDestroy() 
{
	if (m_ToolTip)
	{
		if (m_ToolTip->m_hWnd)
		{
			m_ToolTip->RelayEvent(NULL);
			m_ToolTip->Activate(FALSE);
			m_ToolTip->DelTool(this);
		}
		delete m_ToolTip;
		m_ToolTip = NULL;
	}
	
	CControlBar::OnDestroy();	
}

void CLinkbar::OnSize(UINT nType, int cx, int cy) 
{
	if (m_linkEdit)
	{
		CRect	cRc, bRc;
		GetClientRect(cRc);

		bRc.right  = cRc.right - HGAP;
		bRc.left   = bRc.right - BTNW;
		bRc.top    = cRc.top;
		bRc.bottom = bRc.top + BTNH;

		int gapH = (cRc.Height() - BTNH)/2;
		bRc.OffsetRect(0, gapH);

		m_linkEdit->MoveWindow(bRc);
	}

	CControlBar::OnSize(nType, cx, cy);
}

int CLinkbar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwStyle = WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	m_linkEdit = new CNButton(CTRL_LINKEDIT);
	m_linkEdit->set_Color(RGB(247, 251, 196));
	if (!m_linkEdit->Create(NULL, "search screen", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_linkEdit;
		m_linkEdit = NULL;
	}
	else
	{
		CString		imgC, imgO, imgD;
	//	m_linkEdit->set_ToolTipText("연결화면 편집..");
		imgC.Format("%s\\image\\%s.bmp", m_home, "linkedit");
		imgO.Format("%s\\image\\%s.bmp", m_home, "linkedit_en");
		imgD.Format("%s\\image\\%s.bmp", m_home, "linkedit_dn");
		m_linkEdit->set_Image(imgC, imgO, imgD);
	}
	
	SetToolTipText("연결화면정보");
	loadinfo(m_mapname);
	return 0;
}

void CLinkbar::ChangeSkin(CString skinName)
{
	CString	file;

	file.Format("%s\\image\\linkm.bmp", m_home);
	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	if (hBitmap)
	{
		if (m_bitmap.GetSafeHandle())
			m_bitmap.DeleteObject();
		m_bitmap.Attach(hBitmap);

		if (m_patbrush.GetSafeHandle())	m_patbrush.DeleteObject();
		m_patbrush.CreatePatternBrush(&m_bitmap);
	}

	file.Format("%s\\image\\linkitem.bmp", m_home);
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	if (hBitmap)
	{
		if (m_imgB.GetSafeHandle())
			m_imgB.DeleteObject();
		m_imgB.Attach(hBitmap);
	}

	Invalidate(FALSE);
}

void CLinkbar::loadinfo(CString mapN)
{
	CString	file, str;
	char	buf[1024*16];
	linkinfo*	item;

	removeAry();
	file.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_userName, mapN.Left(4));
	DWORD	dw = GetPrivateProfileSection(mapN, buf, sizeof(buf), file);
	if (dw > 0)
	{
		for (int ii = 0; ii < (int) dw; ii++)
		{
			if (buf[ii] != '\0')
			{
				str += buf[ii];
				continue;
			}
			
			int pos = str.Find("=");
			if (pos == -1)
			{
				str.Empty();
				continue;
			}
			mapN = str.Left(pos++);
			if (mapN.GetLength() == L_MAPN)
			{
				item = new linkinfo;
				item->mapN = mapN;
				item->desc = m_tmenu->GetDesc(mapN);
				item->name = GetSName(mapN);

				m_arInfo.Add(item);
			}
			str.Empty();
		}
	}
	else
	{
		file.Format("%s\\%s\\%s", m_home, MTBLDIR, mapN.Left(4));
		dw = GetPrivateProfileSection(mapN, buf, sizeof(buf), file);
		if (dw <= 0)	return;
		for (int ii = 0; ii < (int) dw; ii++)
		{
			if (buf[ii] != '\0')
			{
				str += buf[ii];
				continue;
			}
			
			int pos = str.Find("=");
			if (pos == -1)
			{
				str.Empty();
				continue;
			}
			mapN = str.Left(pos++);
			if (mapN.GetLength() == L_MAPN)
			{
				item = new linkinfo;
				item->mapN = mapN;
				item->desc = m_tmenu->GetDesc(mapN);
				item->name = GetSName(mapN);

				m_arInfo.Add(item);
			}

			str.Empty();
		}
	}
	Invalidate(FALSE);
}

void CLinkbar::removeAry()
{
	linkinfo*	item;
	
	for (int ii = 0; ii < m_arInfo.GetSize(); ii++)
	{
		item = m_arInfo.GetAt(ii);
		delete item;
	}
	m_arInfo.RemoveAll();
}

void CLinkbar::drawLinkinfo(CDC* pDC)
{
	int	dispCnt = displayCnt();
	for (int ii = 0; ii < m_arInfo.GetSize() && m_itemW; ii++)
	{
		if (ii >= dispCnt)
			break;
		drawItem(pDC, ii);
	}

	CRect	eRc = GetItemRect(LINKEXPAND);
	if (!eRc.IsRectEmpty())
		draw_Expand(pDC);
}

void CLinkbar::drawItem(CDC* pDC, int idx, int mode)
{
	if (m_arInfo.GetSize() <= idx)
		return;
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(COLOR_ITEMTEXT);

	CRect	  iRc	= GetItemRect(idx);
	CFont*	  font	= pDC->SelectObject(&m_font);
	linkinfo* item	= m_arInfo.GetAt(idx);

	drawImg(pDC, iRc, mode);
	iRc.OffsetRect(0, 1);
	pDC->DrawText(item->name, iRc, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
	pDC->SelectObject(font);
}

int CLinkbar::displayCnt()
{
	int		itemW, itemAW = 0, cnt = 0;
	int		ii = 0;
	CRect		cRc;
	
	GetClientRect(cRc);
	cRc.right -= BTNW + HGAP;
	
	if (m_itemW)
	{
		CClientDC	dc(this);
		int	aryCnt  = (int)m_arInfo.GetSize();
		CFont*	font	= dc.SelectObject(&m_font);

		for (ii = 0; ii < aryCnt; ii++)
		{
			linkinfo* item = m_arInfo.GetAt(ii);
			itemW = dc.GetTextExtent(item->name).cx + BTNWGAP;
			itemAW += itemW;

			if (cRc.Width() > itemAW)
			{
				cnt++;
				continue;
			}
			break;
		}
		
		if (aryCnt > cnt)
		{
			cRc.right -= m_expandW;
			itemAW = cnt = 0;
			for (ii = 0; ii < aryCnt; ii++)
			{
				linkinfo* item = m_arInfo.GetAt(ii);
				itemW = dc.GetTextExtent(item->name).cx + BTNWGAP;
				itemAW += itemW;

				if (cRc.Width() > itemAW)
				{
					cnt++;
					continue;
				}
				break;
			}
		}
		dc.SelectObject(font);
	}
	return cnt;
}

CRect CLinkbar::GetItemRect(int idx)
{
	CRect	cRc, iRc(0, 0, 0, 0);
	int	dispCnt = displayCnt();
	int	aryCnt  = (int)m_arInfo.GetSize();
	if (idx != LINKEXPAND)
		idx = dispCnt - idx - 1;

	GetClientRect(cRc);
	cRc.right -= BTNW + HGAP;

	int	gapH = (cRc.Height() - m_itemH)/2;

	CClientDC	dc(this);
	linkinfo*	item;

	CFont*	font = dc.SelectObject(&m_font);
	if (dispCnt > idx)
	{
		iRc.top    = cRc.top;
		iRc.bottom = cRc.bottom;

		if (dispCnt >= aryCnt)
			iRc.right  = cRc.right;
		else	iRc.right  = cRc.right - m_expandW;
		
		for (int ii = 0; ii <= idx; ii++)
		{
			item = m_arInfo.GetAt(ii);
			iRc.left = iRc.right - (dc.GetTextExtent(item->name).cx + BTNWGAP);
			if (ii != idx)	iRc.right = iRc.left;
		}

		iRc.DeflateRect(0, gapH);
	}
	else	// expand rect
	{
		if (dispCnt >= aryCnt)
			iRc.SetRectEmpty();
		else
		{
			iRc.right  = cRc.right;
			iRc.top    = cRc.top;
			iRc.left   = iRc.right - m_expandW;
			iRc.bottom = cRc.bottom;
		}
		iRc.DeflateRect(0, gapH);
	}
	dc.SelectObject(font);
	return iRc;
}

void CLinkbar::drawImg(CDC* pDC, CRect bRC, int mode, int maskcolor)
{
	if (bRC.IsRectEmpty())	return;

	CBitmap*	bitmap;
	int		bx, by, sx;

	bx = m_itemW;
	by = m_itemH;
	switch (mode)
	{
	default:
	case ITEM_COOL:	sx = 0;		break;
	case ITEM_OVER:	sx = bx;	break;
	case ITEM_DOWN:	sx = bx*2;	break;
	}
	
	bitmap = &m_imgB;

	CDC		buffDC, maskDC, memoryDC, copyDC;
	CBitmap		buffBitmap, maskBitmap, copyBitmap;
	CBitmap		*oldbuffBitmap, *oldmaskBitmap, *oldmemoryBitmap, *oldcopyBitmap;

	buffDC.CreateCompatibleDC(pDC);
	buffBitmap.CreateCompatibleBitmap(pDC, bx, by);
	oldbuffBitmap = buffDC.SelectObject(&buffBitmap);

	maskDC.CreateCompatibleDC(pDC);
	maskBitmap.CreateBitmap(bx, by, 1, 1, NULL);
	oldmaskBitmap = maskDC.SelectObject(&maskBitmap);

	CRect		maskRc( 0, 0, bx, by);
	buffDC.FillSolidRect(&maskRc, maskcolor);

	memoryDC.CreateCompatibleDC(pDC);
	oldmemoryBitmap = memoryDC.SelectObject(bitmap);
	buffDC.BitBlt( 0, 0, bx, by, &memoryDC, sx/*0*/, 0, SRCCOPY);
	maskDC.BitBlt( 0, 0, bx, by, &buffDC, 0, 0, SRCCOPY);

	copyDC.CreateCompatibleDC(pDC);
	copyBitmap.CreateCompatibleBitmap(pDC, bx, by);
	oldcopyBitmap = copyDC.SelectObject(&copyBitmap);

	copyDC.BitBlt( 0, 0, bx, by, pDC,  bRC.left, bRC.top, SRCCOPY);
	copyDC.BitBlt( 0, 0, bx, by, &maskDC, 0, 0, SRCAND);
	buffDC.BitBlt( 0, 0, bx, by, &maskDC, 0, 0, SRCMASK);
	copyDC.BitBlt( 0, 0, bx, by, &buffDC, 0, 0, SRCPAINT);
	pDC->StretchBlt(bRC.left, bRC.top, bRC.Width(), bRC.Height(), &copyDC, 0, 0, bx, by, SRCCOPY);

	copyDC.SelectObject(oldcopyBitmap);
	memoryDC.SelectObject(oldmemoryBitmap);
	maskDC.SelectObject(oldmaskBitmap);
	buffDC.SelectObject(oldbuffBitmap);

	buffDC.DeleteDC();
	maskDC.DeleteDC();
	memoryDC.DeleteDC();
	copyDC.DeleteDC();
}

void CLinkbar::HoverItem(CPoint point)
{
	if (!m_arInfo.GetSize())
		return;
	
	int nItem = GetSelectItem(point);
	if (nItem == m_hover && !m_pushON)
		return;

	CClientDC	dc(this);
	if (m_pushON)
	{
		if (nItem == m_push)
		{
			switch (nItem)
			{
			case -1:	break;
			default:	drawItem(&dc, m_push,  ITEM_DOWN);	break;
			case LINKEXPAND:draw_Expand(&dc, ITEM_DOWN);		break;
			}
		}
		else
		{
			switch (nItem)
			{
			case -1:	break;
			default:	drawItem(&dc, m_push);	break;
			case LINKEXPAND:draw_Expand(&dc);	break;
			}
		}
	}
	else
	{
		linkinfo*	item;
		switch (m_hover)
		{
		case -1:	break;
		default:	drawItem(&dc, m_hover);	break;
		case LINKEXPAND:draw_Expand(&dc);	break;
		}

		switch (nItem)
		{
		case -1:	break;
		default:
			item = m_arInfo.GetAt(nItem);
			drawItem(&dc, nItem, ITEM_OVER);
			m_ToolTip->SetToolRect(this, ID_TOOLTIP, GetItemRect(nItem));
			SetToolTipText(item->desc);		break;
		case LINKEXPAND:	
			draw_Expand(&dc, ITEM_OVER);
			m_ToolTip->SetToolRect(this, ID_TOOLTIP, GetItemRect(LINKEXPAND));
			SetToolTipText("More");			break;
		}
	}
	m_hover = nItem;;
}

int CLinkbar::GetSelectItem(CPoint pt)
{
	CRect	iRc;
	int	dispCnt = displayCnt();
	for (int ii = 0; ii < m_arInfo.GetSize(); ii++)
	{
		if (ii >= dispCnt)
		{
			iRc = GetItemRect(LINKEXPAND);
			if (iRc.PtInRect(pt))
				return LINKEXPAND;
			break;
		}
		iRc = GetItemRect(ii);
		if (iRc.PtInRect(pt))
			return ii;
	}
	return -1;
}

void CLinkbar::draw_Expand(CDC* pDC, int mode, int maskcolor)
{
	int	sx = 0, bx, by;

	switch (mode)
	{
	case ITEM_OVER:	sx = m_expandW;		break;
	case ITEM_DOWN:	sx = m_expandW*2;	break;
	}

	CRect	eRc = GetItemRect(LINKEXPAND);
	if (eRc.IsRectEmpty())	return;

	CBitmap*	bitmap;

	bx = eRc.Width();
	by = eRc.Height();
	
	
	bitmap = &m_exImg;

	CDC		buffDC, maskDC, memoryDC, copyDC;
	CBitmap		buffBitmap, maskBitmap, copyBitmap;
	CBitmap		*oldbuffBitmap, *oldmaskBitmap, *oldmemoryBitmap, *oldcopyBitmap;

	buffDC.CreateCompatibleDC(pDC);
	buffBitmap.CreateCompatibleBitmap(pDC, bx, by);
	oldbuffBitmap = buffDC.SelectObject(&buffBitmap);

	maskDC.CreateCompatibleDC(pDC);
	maskBitmap.CreateBitmap(bx, by, 1, 1, NULL);
	oldmaskBitmap = maskDC.SelectObject(&maskBitmap);

	CRect		maskRc( 0, 0, bx, by);
	buffDC.FillSolidRect(&maskRc, maskcolor);

	memoryDC.CreateCompatibleDC(pDC);
	oldmemoryBitmap = memoryDC.SelectObject(bitmap);
	buffDC.BitBlt( 0, 0, bx, by, &memoryDC, sx/*0*/, 0, SRCCOPY);
	maskDC.BitBlt( 0, 0, bx, by, &buffDC, 0, 0, SRCCOPY);

	copyDC.CreateCompatibleDC(pDC);
	copyBitmap.CreateCompatibleBitmap(pDC, bx, by);
	oldcopyBitmap = copyDC.SelectObject(&copyBitmap);

	copyDC.BitBlt( 0, 0, bx, by, pDC,  eRc.left, eRc.top, SRCCOPY);
	copyDC.BitBlt( 0, 0, bx, by, &maskDC, 0, 0, SRCAND);
	buffDC.BitBlt( 0, 0, bx, by, &maskDC, 0, 0, SRCMASK);
	copyDC.BitBlt( 0, 0, bx, by, &buffDC, 0, 0, SRCPAINT);
	pDC->BitBlt(eRc.left, eRc.top, bx, by, &copyDC, 0/*sx*/, 0, SRCCOPY);

	copyDC.SelectObject(oldcopyBitmap);
	memoryDC.SelectObject(oldmemoryBitmap);
	maskDC.SelectObject(oldmaskBitmap);
	buffDC.SelectObject(oldbuffBitmap);

	buffDC.DeleteDC();
	maskDC.DeleteDC();
	memoryDC.DeleteDC();
	copyDC.DeleteDC();
}

void CLinkbar::showMenu()
{
	CString		str;
	linkinfo*	item;
	CMenuXP		menu;
	CPoint		xpoint = GetItemRect(LINKEXPAND).BottomRight();
			
	ClientToScreen(&xpoint);
	menu.CreatePopupMenu();

	UINT	nFlags = MF_POPUP|MF_BYPOSITION|MF_STRING;
	int	menuID = 1000, dispCnt = displayCnt();

	for (int ii = dispCnt; ii < m_arInfo.GetSize(); ii++)
	{
		item = m_arInfo.GetAt(ii);
		str.Format("%s %s", m_tmenu->GetDispN(item->mapN), item->desc);
		menu.AppendMenuX(MF_STRING, menuID + ii, str);
	}

	int cmd = menu.TrackPopupMenu(TPM_RETURNCMD|TPM_RIGHTALIGN|TPM_RIGHTBUTTON, xpoint.x, xpoint.y, this);
	cmd -= menuID;
	if (cmd > 0)
	{
		item = m_arInfo.GetAt(cmd);
		GetParent()->SendMessage(WM_LINKOPEN, 0, (LPARAM)item->mapN.operator LPCTSTR());
	}
}

void CLinkbar::ChangeInfo(CString mapname)
{
	if (!mapname.IsEmpty())
		m_mapname = mapname;
	loadinfo(m_mapname);
}

CString CLinkbar::GetSName(CString mapN)
{
	CString	sName, file;

	file.Format("%s\\%s\\linkname.ini", m_home, TABDIR);
	UINT dispN = GetPrivateProfileInt("SNAME", "DISPN", 1, file);
	if (dispN)	sName = m_tmenu->GetDispN(mapN.Left(L_MAPN));
	else
	{
		char	name[24];
		
		DWORD dw = GetPrivateProfileString("SNAME", mapN.Left(L_MAPN), "", name, sizeof(name), file);
		if (dw <= 0)	
			sName = m_tmenu->GetSName(mapN.Left(L_MAPN));
		else
		{
			sName = name;
			sName.TrimLeft();
			sName.TrimRight();
			if (sName.IsEmpty())
				sName = m_tmenu->GetSName(mapN.Left(L_MAPN));
		}

	}
	return sName;
}