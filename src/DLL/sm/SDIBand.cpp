// SDIBand.cpp : implementation file
//

#include "stdafx.h"
#include "SDIBand.h"
#include "resource.h"
#include "MenuXP.h"
#include "Smain.h"
#include "RenameDlg.h"
#include "NPalette.h"
#include "Dbar1.h"

#include "../../axis/axMsg.hxx"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
#define	XGAP		1
#define	YGAP		1
#define	EXPANDWIDTH	13
#define	IEXPAND		9999
#define BANDGAP		4

#define LMENU_BASE	1000

#define	MN_REGISTER	1
#define	MN_RENAME	2
#define	MN_DELETE	3
#define	MN_RUN		4
/////////////////////////////////////////////////////////////////////////////
// CSDIBand

CSDIBand::CSDIBand(bool (*axiscall)(int, WPARAM, LPARAM), bool bSDI)
{
	m_axiscall = axiscall;
	m_bSDI = bSDI;
	m_defaultColor = false;
	m_hindex = -1;
	m_itemW = 0;
	m_dispN = 1;
	CString	fontName;
	fontName.LoadString(STSM_FONTNAME_DEFAULT);
	m_font.CreatePointFont(90, fontName);
	
	m_mouseIN = false;
	m_pushON = false;
	m_hover = -1;
	m_push = -1;

	m_expandRc = CRect(0, 0, 0, 0);

	m_itcolorCool = SCOLOR_ITCOOL;
	m_itcolorOver = SCOLOR_ITOVER;
	m_itcolorDown = SCOLOR_ITDOWN;

	m_ToolTip = NULL;

#ifdef DF_USESTL_SDIBAND
	m_arItem.clear();
	m_arsubItem.clear();
#else
	m_arItem.RemoveAll();
	m_arsubItem.RemoveAll();
#endif

	/**
	CString file;
	file.Format("%s\\image\\mb_red_expand.bmp", Axis::home);
	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	if (hBitmap)
	{	
		m_bmExpand.Attach(hBitmap);
		BITMAP	bm;
		m_bmExpand.GetBitmap(&bm);
		m_expandW = bm.bmWidth/3;
	}
	**/

	m_bDrag = false;
	m_pDragImage = NULL;
}

CSDIBand::~CSDIBand()
{
	XMSG();
	m_font.DeleteObject();

#ifdef DF_USESTL_SDIBAND
	m_arItem.clear();
	m_arsubItem.clear();
#else
	CBandItem* item{};
	for (int ii = 0; ii < m_arItem.GetSize(); ii++)
	{
		item = m_arItem.GetAt(ii);
		delete item;
	}
	m_arItem.RemoveAll();

	for (int ii = 0; ii < m_arsubItem.GetSize(); ii++)
	{
		item = m_arsubItem.GetAt(ii);
		delete item;
	}
	m_arsubItem.RemoveAll();
#endif
}

BEGIN_MESSAGE_MAP(CSDIBand, CWnd)
	//{{AFX_MSG_MAP(CSDIBand)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MEASUREITEM()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSDIBand message handlers

void CSDIBand::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC	 memDC;
	CRect	 cRc;
	
	GetClientRect(cRc);
	if (memDC.CreateCompatibleDC(&dc))
	{
		CBitmap	bitmap;
		bitmap.CreateCompatibleBitmap(&dc, cRc.Width(), cRc.Height());
		CBitmap* oldbuffBitmap = memDC.SelectObject(&bitmap);
		if (oldbuffBitmap != (CBitmap *) 0)
		{
			drawBackGround(&memDC);
			drawBandItem(&memDC);
			
			dc.BitBlt(cRc.left, cRc.top, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);
			memDC.SelectObject(oldbuffBitmap);
		}
		memDC.DeleteDC();
	}
	
}

void CSDIBand::OnDestroy() 
{
	XMSG();
	if (m_ToolTip)
	{
		if (m_ToolTip->m_hWnd)
		{
			m_ToolTip->Activate(FALSE);
			m_ToolTip->DelTool(this);
			//m_ToolTip->RelayEvent(NULL);
		}

#ifndef DF_USESTL_SDIBAND
		delete m_ToolTip;
#endif
		
	}
	CWnd::OnDestroy();
}

void CSDIBand::OnSize(UINT nType, int cx, int cy) 
{
	ReCalc();
	CWnd::OnSize(nType, cx, cy);
}

void CSDIBand::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CClientDC	dc(this);
	const int		nItem = GetSelectItem(point);

	SetCapture();
	m_pushON = true;
	switch (nItem)
	{
	case -1:
		m_push   = -1;
		m_pushON = false;
		break;
	case IEXPAND:
		m_push = IEXPAND;
		draw_Expand(&dc, SMODE_DOWN);
		break;
	default:
		m_rcSelected = GetToolRect(nItem);
		drawItem(&dc, nItem, SMODE_DOWN);
		m_push = nItem;
		break;
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CSDIBand::OnLButtonUp(UINT nFlags, CPoint point) 
{
#ifdef DF_USESTL_SDIBAND
	ReleaseCapture();

	if (m_bDrag)
	{
		CImageList::DragLeave(this);
		CImageList::EndDrag();

		if (m_pDragImage)
		{
			delete m_pDragImage;
			m_pDragImage = NULL;
		}
		auto pItem = m_arItem.at(m_push);

		CRect cRc;
		GetClientRect(&cRc);
		cRc.InflateRect(100, 0);
		if (cRc.PtInRect(point))
		{
			const int index = GetDropIndex(cRc, point);

			if (index >= 0 && index != m_push)
			{
				m_arItem.insert(m_arItem.begin() + index, pItem);

				if (index < m_push)
					m_arItem.erase(m_arItem.begin() + m_push + 1);
				else
					m_arItem.erase(m_arItem.begin() + m_push);

				WriteToolItem();
				if (m_bSDI)	((class CSmain*)GetParent())->Refresh();
				else		((class CDbar1*)GetParent())->Refresh();
			}
		}
		else
		{
			if (MessageBox(pItem->text + "를 툴바그룹에서 삭제하시겠습니까?", "사용자 툴바 아이템 삭제", MB_YESNO) == IDYES)
			{
				ItemDEL(m_push);

				WriteToolItem();
				if (m_bSDI)	((class CSmain*)GetParent())->Refresh();
				else		((class CDbar1*)GetParent())->Refresh();
			}
		}

		m_push = -1;
		m_pushON = false;
		m_bDrag = false;

		CWnd::OnLButtonUp(nFlags, point);
		return;
	}

	CClientDC	dc(this);
	const int		nItem = GetSelectItem(point);

	switch (m_push)
	{
	case -1:	break;
	case IEXPAND:
		draw_Expand(&dc);
		if (nItem == m_push)
			showMenu();
		break;
	default:
		drawItem(&dc, m_push);
		if (nItem == m_push)
		{
			auto item = m_arItem.at(nItem);
			if (item->kind == SBAND_ITEM)
				(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(const char*)item->mapN);
			else
			{
				const CRect	iRc = GetToolRect(nItem);
				ShowItemList(item->mapN, CPoint(iRc.left, iRc.bottom));
			}
		}
		break;
	}

	m_push = -1;
	m_pushON = false;
#else
	ReleaseCapture();

	if (m_bDrag)
	{
		CImageList::DragLeave(this);
		CImageList::EndDrag();

		if (m_pDragImage)
		{
			delete m_pDragImage;
			m_pDragImage = NULL;
		}
		CBandItem* pItem = m_arItem.GetAt(m_push);

		CRect cRc;
		GetClientRect(&cRc);
		cRc.InflateRect(100, 0);
		if (cRc.PtInRect(point))
		{
			const int index = GetDropIndex(cRc, point);

			if (index >= 0 && index != m_push)
			{
				m_arItem.InsertAt(index, pItem);

				if (index < m_push)
					m_arItem.RemoveAt(m_push + 1);
				else
					m_arItem.RemoveAt(m_push);

				WriteToolItem();
				if (m_bSDI)	((class CSmain*)GetParent())->Refresh();
				else		((class CDbar1*)GetParent())->Refresh();
			}
		}
		else
		{
			if (MessageBox(pItem->text + "를 툴바그룹에서 삭제하시겠습니까?", "사용자 툴바 아이템 삭제", MB_YESNO) == IDYES)
			{
				ItemDEL(m_push);

				WriteToolItem();
				if (m_bSDI)	((class CSmain*)GetParent())->Refresh();
				else		((class CDbar1*)GetParent())->Refresh();
			}
		}

		m_push = -1;
		m_pushON = false;
		m_bDrag = false;

		CWnd::OnLButtonUp(nFlags, point);
		return;
	}

	CClientDC	dc(this);
	const int		nItem = GetSelectItem(point);

	switch (m_push)
	{
	case -1:	break;
	case IEXPAND:
		draw_Expand(&dc);
		if (nItem == m_push)
			showMenu();
		break;
	default:
		drawItem(&dc, m_push);
		if (nItem == m_push)
		{
			const CBandItem* item = m_arItem.GetAt(nItem);
			if (item->kind == SBAND_ITEM)
				(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(const char*)item->mapN);
			else
			{
				const CRect	iRc = GetToolRect(nItem);
				ShowItemList(item->mapN, CPoint(iRc.left, iRc.bottom));
			}
		}
		break;
	}

	m_push = -1;
	m_pushON = false;
#endif

	CWnd::OnLButtonUp(nFlags, point);
}

void CSDIBand::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd*			wndUnderMouse = NULL;
	const CWnd*			wndActive = this;
	TRACKMOUSEEVENT		csTME{};

	CWnd::OnMouseMove(nFlags, point);
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
		HoverItem(lpoint);
	}
	else 	m_mouseIN = false;

	if (m_bDrag)
	{
		POINT pt = lpoint;
		ClientToScreen( &pt );
		CImageList::DragMove(pt);
		CImageList::DragShowNolock(TRUE);
		return;
	}
	else if(nFlags == MK_LBUTTON)
	{
		if (!PtInRect(&m_rcSelected, lpoint) && m_push != -1)
		{
#ifdef DF_USESTL_SDIBAND
			if (m_push >= (int)m_arItem.size())	return;

			auto item = m_arItem.at(m_push);
#else
			if (m_push >= m_arItem.GetSize())	return;

			const CBandItem* item = m_arItem.GetAt(m_push);
#endif
		
			if (m_pDragImage)
			{
				delete m_pDragImage;
				m_pDragImage = NULL;
			}
			m_pDragImage = MakeDragImage(item->text, this);
			
			if (!m_pDragImage) return;
			
			CPoint	pt;	
			IMAGEINFO ii;
			m_pDragImage->GetImageInfo(0, &ii);
			pt.x = (ii.rcImage.right - ii.rcImage.left) / 2;
			pt.y = (ii.rcImage.bottom - ii.rcImage.top) / 2;
			
			m_pDragImage->BeginDrag(0, pt);
			pt = lpoint;
			ClientToScreen(&pt);
			m_pDragImage->DragEnter(NULL, pt);
			
			m_bDrag = true;
			m_rcSelected.SetRectEmpty();
			return;
		}
	}
}

LRESULT CSDIBand::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_mouseIN = false;
	Invalidate(FALSE);
	m_hover = -1;
	return 0;
}

BOOL CSDIBand::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	const int	comm = LOWORD(wParam) - LMENU_BASE;
	if (comm >= 0 && comm < m_commandList.GetSize())
	{
		CString mapN = m_commandList.GetAt(comm);
		(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(const char *)mapN);
	}
	return CWnd::OnCommand(wParam, lParam);
}

LRESULT CSDIBand::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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
			MSG	msg{};
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
	return CWnd::WindowProc(message, wParam, lParam);
}

void CSDIBand::SetToolTipText(CString text, BOOL bActivate)
{
	if (text.IsEmpty())	return;

	InitToolTip();
	if (m_ToolTip->GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip->AddTool(this, text, CRect(0, 0, 100, 100), SID_TOOLTIP);
	}

	m_ToolTip->UpdateTipText(text, this, SID_TOOLTIP);
	m_ToolTip->Activate(bActivate);
}

void CSDIBand::InitToolTip()
{
	if (!m_ToolTip)
	{
#ifdef DF_USESTL_SDIBAND
		m_ToolTip = std::make_unique<CToolTipCtrl>();
#else
		m_ToolTip = new CToolTipCtrl();
#endif
		m_ToolTip->Create(this, TTS_ALWAYSTIP);
		m_ToolTip->Activate(TRUE);
	}
}

void CSDIBand::ReCalc()
{
	CRect	cRc, iRc;
	bool	expand = false;
	
	GetClientRect(cRc);
#ifdef DF_USESTL_SDIBAND
	for (int ii = 0; ii < (int)m_arItem.size() && !expand; ii++)
#else
	for (int ii = 0; ii < m_arItem.GetSize() && !expand; ii++)
#endif
	{
		iRc = GetToolRect(ii);
		if (iRc.right + m_expandW > cRc.right)
		{
			expand = true;
			m_hindex = ii;
			m_expandRc.SetRect(cRc.right-m_expandW, iRc.top, cRc.right, iRc.bottom);
		}
	}

	if (!expand)
	{
		m_hindex = -1;
		m_expandRc.SetRectEmpty();
	}

	Invalidate(FALSE);
}

// dat format : mapN=툴팁;단축명
bool CSDIBand::ItemADD(CString dat, bool list)
{
	int	pos = dat.Find('=');
	if (pos == -1)	return false;

#ifdef DF_USESTL_SDIBAND
	const auto item = m_arItem.emplace_back(std::make_shared<CBandItem>());
#else
	CBandItem* item = new CBandItem;
#endif

	item->mapN = dat.Left(pos++);
	dat = dat.Mid(pos);

	if (list)	item->kind = SBAND_LIST;
	else		item->kind = SBAND_ITEM;

	pos = dat.Find(';');
	if (pos == -1)
	{
		if (list)
			item->text = item->tooltip = item->mapN;
		else	item->text = item->tooltip = item->mapN.Mid(2, 4);
#ifndef DF_USESTL_SDIBAND
		m_arItem.Add(item);
#endif
		ReCalc();
		return true;
	}
	
	item->tooltip = dat.Left(pos++);
	dat = dat.Mid(pos);
	item->text = dat;

#ifndef DF_USESTL_SDIBAND
	m_arItem.Add(item);
#endif
	ReCalc();
	return true;
}

// dat format : mapN=툴팁;단축명
bool CSDIBand::ItemSubADD(CString dat)
{
	int	pos = dat.Find('=');
	if (pos == -1)	return false;

#ifdef DF_USESTL_SDIBAND
	const auto item = m_arsubItem.emplace_back(std::make_shared<CBandItem>());
#else
	CBandItem* item = new CBandItem;
#endif
	
	item->mapN = dat.Left(pos++);
	dat = dat.Mid(pos);

	item->kind = SBAND_ITEM;

	pos = dat.Find(';');
	if (pos == -1)
	{
		item->text = item->tooltip = item->mapN.Mid(2, 4);
#ifndef DF_USESTL_SDIBAND
		m_arsubItem.Add(item);
#endif
		return true;
	}
	
	item->tooltip = dat.Left(pos++);
	dat = dat.Mid(pos);
	item->text = dat;

#ifndef DF_USESTL_SDIBAND
	m_arsubItem.Add(item);
#endif
	return true;
}

bool CSDIBand::ItemDEL(int index)
{
#ifdef DF_USESTL_SDIBAND
	if (index >= (int)m_arItem.size())	return false;

	m_arItem.erase(m_arItem.begin() + index);
	ReCalc();
#else
	if (index >= m_arItem.GetSize())	return false;

	delete m_arItem.GetAt(index);
	m_arItem.RemoveAt(index);
	ReCalc();
#endif

	return true;
}

void CSDIBand::removeALL()
{
#ifdef DF_USESTL_SDIBAND
	m_arItem.clear();
#else
	CBandItem* item{};
	for (int ii = 0; ii < m_arItem.GetSize(); ii++)
	{
		item = m_arItem.GetAt(ii);
		delete item;
	}
	m_arItem.RemoveAll();
#endif

}

void CSDIBand::HoverItem(CPoint point)
{
#ifdef DF_USESTL_SDIBAND
	if (!m_arItem.size())
		return;
#else
	if (!m_arItem.GetSize())
		return;
#endif

	const int	nItem = GetSelectItem(point);
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
			default:	drawItem(&dc, m_push, SMODE_DOWN);	break;
			case IEXPAND:	draw_Expand(&dc, SMODE_DOWN);		break;
			}
		}
		else
		{
			switch (nItem)
			{
			case -1:	break;
			default:	drawItem(&dc, m_push);	break;
			case IEXPAND:	draw_Expand(&dc);	break;
			}
		}
	}
	else
	{
#ifdef DF_USESTL_SDIBAND
		switch (m_hover)
		{
		case -1:	break;
		default:	drawItem(&dc, m_hover);	break;
		case IEXPAND:	draw_Expand(&dc);	break;
		}

		switch (nItem)
		{
		case -1:	break;
		default:
		{
			auto item = m_arItem.at(nItem);
			drawItem(&dc, nItem, SMODE_OVER);
			m_ToolTip->SetToolRect(this, SID_TOOLTIP, GetToolRect(nItem));
			SetToolTipText(item->tooltip);		break;
		}
		case IEXPAND:
			draw_Expand(&dc, SMODE_OVER);
			m_ToolTip->SetToolRect(this, SID_TOOLTIP, m_expandRc);
			SetToolTipText("More");			break;
		}
#else
		CBandItem* item{};
		switch (m_hover)
		{
		case -1:	break;
		default:	drawItem(&dc, m_hover);	break;
		case IEXPAND:	draw_Expand(&dc);	break;
		}

		switch (nItem)
		{
		case -1:	break;
		default:
			item = m_arItem.GetAt(nItem);
			drawItem(&dc, nItem, SMODE_OVER);
			m_ToolTip->SetToolRect(this, SID_TOOLTIP, GetToolRect(nItem));
			SetToolTipText(item->tooltip);		break;
		case IEXPAND:
			draw_Expand(&dc, SMODE_OVER);
			m_ToolTip->SetToolRect(this, SID_TOOLTIP, m_expandRc);
			SetToolTipText("More");			break;
		}
#endif

		
	}

	m_hover = nItem;
}

void CSDIBand::drawBackGround(CDC* pDC)
{
	CRect	cRc;
	GetClientRect(cRc);
	pDC->FillSolidRect(cRc, CNPalette::GetIndexColor(CLR_TOOL_BK));

	if (!m_bSDI)
	{
		CPen	pen(PS_SOLID, 1, RGB(255, 255, 255));
		
		CPen	*pOldPen = pDC->SelectObject(&pen);
		pDC->MoveTo(0, 0);
		pDC->LineTo(cRc.right, 0);
		
		pDC->SelectObject(pOldPen);
	}
}

void CSDIBand::drawBandItem(CDC* pDC)
{
#ifdef DF_USESTL_SDIBAND
	for (int ii = 0; ii < (int)m_arItem.size(); ii++)
	{
		if (ii == m_hindex)	break;
		drawItem(pDC, ii);
}
#else
	for (int ii = 0; ii < m_arItem.GetSize(); ii++)
	{
		if (ii == m_hindex)	break;
		drawItem(pDC, ii);
	}
#endif
	

	if (!m_expandRc.IsRectEmpty())	draw_Expand(pDC);
}

void CSDIBand::drawItem(CDC* pDC, int idx, int mode)
{
	if (idx == -1)	return;
	
	CRect		iRc   = GetToolRect(idx);
	CFont*		pfont = pDC->SelectObject(&m_font);
	
#ifdef DF_USESTL_SDIBAND
	const auto item = m_arItem.at(idx);
#else
	const CBandItem* item = m_arItem.GetAt(idx);
#endif
	COLORREF	color{};


	switch (mode)
	{
	case SMODE_OVER:	color = pDC->SetTextColor(m_itcolorOver);	break;
	case SMODE_DOWN:	color = pDC->SetTextColor(m_itcolorDown);	break;
	default:		color = pDC->SetTextColor(m_itcolorCool);	break;
	}

	draw_Img(pDC, iRc, item->kind, mode);
	FitTextRect(iRc);
	pDC->SetBkMode(TRANSPARENT);

	const UINT	nFlags = DT_VCENTER|DT_CENTER |DT_SINGLELINE|DT_END_ELLIPSIS;
	pDC->DrawText(item->text, iRc, nFlags);

	pDC->SetTextColor(color);
	pDC->SelectObject(pfont);
}

void CSDIBand::draw_Img(CDC* pDC, CRect iRc, int kind, int mode)
{
	CDC	memDC;
	int	sx{};

	switch (mode)
	{
	case SMODE_COOL:	sx = 0;		break;
	case SMODE_DOWN:	sx = m_itemW;	break;
	case SMODE_OVER:	sx = m_itemW*2;	break;
	default:	return;
	}

	if (kind == SBAND_LIST)	sx += m_itemW*3;
	if (memDC.CreateCompatibleDC(pDC))
	{
		CBitmap* oldbuffBitmap = memDC.SelectObject(m_bmToolbox);
		if (oldbuffBitmap != (CBitmap *) 0)
		{
			pDC->BitBlt(iRc.left, iRc.top, m_itemW, iRc.Height(), 
					&memDC, sx, 0, SRCCOPY);
			memDC.SelectObject(oldbuffBitmap);
		}
		memDC.DeleteDC();
	}
}

void CSDIBand::draw_Expand(CDC* pDC, int mode)
{
	int	sx = 0;
	switch (mode)
	{
	case SMODE_OVER:	sx = m_expandW;		break;
	case SMODE_DOWN:	sx = m_expandW*2;	break;
	}

	CDC	memDC;
	if (memDC.CreateCompatibleDC(pDC))
	{
		CBitmap* oldbuffBitmap = memDC.SelectObject(m_bmExpand);
		if (oldbuffBitmap != (CBitmap *) 0)
		{
			pDC->BitBlt(m_expandRc.left, m_expandRc.top, m_expandRc.Width(),
					m_expandRc.Height(), &memDC, sx, 0, SRCCOPY);
			memDC.SelectObject(oldbuffBitmap);
		}
		memDC.DeleteDC();
	}
}

void CSDIBand::change_Skin(CString skinName)
{
	m_bmToolbox = Axis::GetBitmap(Format("%s_TOOLBOX", skinName));//Format("MB_%s_TOOLBOX", skinName));
	if (!m_itemW && m_bmToolbox)
	{
		BITMAP	bm;
		m_bmToolbox->GetBitmap(&bm);
		m_itemW = bm.bmWidth / 6;
		m_itemH = bm.bmHeight;
	}

	m_bmExpand = Axis::GetBitmap(Format("%s_EXPAND", skinName));//Format("MB_%s_EXPAND", skinName));
	if (m_bmExpand)
	{
		BITMAP bm;
		m_bmExpand->GetBitmap(&bm);
		m_expandW = bm.bmWidth / 3;
	}

	ReCalc();
}

void CSDIBand::FitTextRect(CRect& tRc)
{
	tRc.right -= 4;
	tRc.DeflateRect(2, 2);
}

void CSDIBand::ShowItemList(CString key, CPoint point)
{
	char		scrN[16]{};
	CMenuXP		menu;
	CString		str;
	
	ClientToScreen(&point);
	menu.CreatePopupMenu();

	if (key.Find('#') == -1)
		return;
	LoadSubItem(key);
	m_commandList.RemoveAll();

#ifdef DF_USESTL_SDIBAND
	for (int ii = 0; ii <(int)m_arsubItem.size(); ii++)
	{
		auto item = m_arsubItem.at(ii);
		sprintf(scrN, "%s", item->mapN);
		(*m_axiscall)(AXI_GETMAPNUM, 0, (LPARAM)scrN);
		str.Format("%s %s", scrN, item->tooltip);
		menu.AppendMenuX(MF_STRING, LMENU_BASE + ii, str);
		m_commandList.Add(item->mapN);
}
#else
	CBandItem* item{};
	for (int ii = 0; ii < m_arsubItem.GetSize(); ii++)
	{
		item = m_arsubItem.GetAt(ii);
		sprintf(scrN, "%s", item->mapN);
		(*m_axiscall)(AXI_GETMAPNUM, 0, (LPARAM)scrN);
		str.Format("%s %s", scrN, item->tooltip);
		menu.AppendMenuX(MF_STRING, LMENU_BASE + ii, str);
		m_commandList.Add(item->mapN);
	}
#endif
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
}

CRect CSDIBand::GetToolRect(int index)
{
	CRect	cRc, rc;

	GetClientRect(cRc);
	const int hGap = (int)((cRc.Height() - m_itemH) / 2);
	rc.SetRect(cRc.left+m_itemW*index, hGap,
		cRc.left+m_itemW*(index+1), hGap + m_itemH);
	return rc;
}

void CSDIBand::LoadSubItem(CString section)
{
#ifdef DF_USESTL_SDIBAND
	m_arsubItem.clear();
#else
	CBandItem* item{};
	for (int ii = 0; ii < m_arsubItem.GetSize(); ii++)
	{
		item = m_arsubItem.GetAt(ii);
		delete item;
	}
	m_arsubItem.RemoveAll();
#endif



	CProfile profile(pkUserTool);
	CString buffer(profile.GetSectionLF(section));
	const int len = buffer.GetLength();
	if (len < 1)
		return;

	CString subitem, string(buffer);
	for (; !string.IsEmpty(); )
	{
		int idx = string.Find('\n');
		if (idx == -1)	break;

		subitem  = string.Left(idx++);
		string = string.Mid(idx);

		idx = subitem.Find('=');
		if (idx == -1)	continue;

		ItemSubADD(subitem);
	}
}

CString CSDIBand::GetUserName()
{
	CString userN = AfxGetApp()->GetProfileString(WORKSTATION, SIGNONID);
	const int pos = userN.Find('|');
	if (pos < 0)	userN = _T("Anonymous");
	else		userN = userN.Mid(pos+1);
	if (userN.IsEmpty())	userN = _T("Anonymous");
	return userN;
}

void CSDIBand::InsertSubMenu(CMenu* menu, CString section, int& index)
{
	CProfile profile(pkUserTool);

	CString string(profile.GetSectionLF(section));

	if (string.IsEmpty())	
		return;

	CString	tmpS, mapN, desc, str, subitem;
	char	scrN[16]{};

	for (; !string.IsEmpty(); )
	{
		int idx = string.Find('\n');
		if (idx == -1)	break;

		subitem  = string.Left(idx++);
		string = string.Mid(idx);

		idx = subitem.Find('=');
		if (idx == -1)	continue;

		mapN = subitem.Left(idx++);
		subitem = subitem.Mid(idx);

		idx = subitem.Find(';');
		if (idx == -1)	desc = subitem;
		else		desc = subitem.Left(idx);

		sprintf(scrN, "%s", mapN);
		(*m_axiscall)(AXI_GETMAPNUM, 0, (LPARAM)scrN);
		str.Format("%s %s", scrN, desc);

		((CMenuXP*)menu)->AppendMenuX(MF_STRING, index++, str);
		m_commandList.Add(mapN);
	}
}

int CSDIBand::GetSelectItem(CPoint pt)
{
	int	nItem = -1;
	CRect	iRc;
	
#ifdef DF_USESTL_SDIBAND
	for (int ii = 0; ii < (int)m_arItem.size(); ii++)
	{
		if (ii == m_hindex)	break;
		iRc = GetToolRect(ii);
		if (iRc.PtInRect(pt))
		{
			nItem = ii;
			break;
		}
}
#else
	for (int ii = 0; ii < m_arItem.GetSize(); ii++)
	{
		if (ii == m_hindex)	break;
		iRc = GetToolRect(ii);
		if (iRc.PtInRect(pt))
		{
			nItem = ii;
			break;
		}
	}
#endif

	if (m_hindex != -1 && m_expandRc.PtInRect(pt))
		nItem = IEXPAND;

	return nItem;
}

void CSDIBand::showMenu()
{
#ifdef DF_USESTL_SDIBAND
	CString		str;
	CMenuXP		menu, * subMenu = NULL;
	CPoint		xpoint = m_expandRc.BottomRight();

	ClientToScreen(&xpoint);
	menu.CreatePopupMenu();

	char	scrN[16]{};
	CArray	< CMenuXP*, CMenuXP* > menulist;
	const UINT	nFlags = MF_POPUP | MF_BYPOSITION | MF_STRING;

	m_commandList.RemoveAll();
	for (int ii = m_hindex, index = LMENU_BASE; ii < (int)m_arItem.size(); ii++)
	{
		auto item = m_arItem.at(ii);
		if (item->kind == SBAND_LIST)
		{
#pragma warning (disable : 26400)
#pragma warning (disable : 26409)
			subMenu = new CMenuXP();
#pragma warning (default : 26400)
#pragma warning (default : 26409)
			subMenu->CreatePopupMenu();
			menulist.Add(subMenu);
			InsertSubMenu(subMenu, item->mapN, index);
			menu.InsertMenuX(ii, nFlags, (UINT)subMenu->m_hMenu, item->tooltip);
		}
		else
		{
			sprintf(scrN, "%s", item->mapN);
			(*m_axiscall)(AXI_GETMAPNUM, 0, (LPARAM)scrN);
			str.Format("%s %s", scrN, item->tooltip);
			menu.AppendMenuX(MF_STRING, index++, str);
			m_commandList.Add(item->mapN);
		}
	}

	menu.TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON, xpoint.x, xpoint.y, this);
	menulist.RemoveAll();
#else
	CString		str;
	CBandItem* item{};
	CMenuXP		menu, * subMenu = NULL;
	CPoint		xpoint = m_expandRc.BottomRight();

	ClientToScreen(&xpoint);
	menu.CreatePopupMenu();

	char	scrN[16]{};
	CArray	< CMenuXP*, CMenuXP* > menulist;
	const UINT	nFlags = MF_POPUP | MF_BYPOSITION | MF_STRING;

	m_commandList.RemoveAll();
	for (int ii = m_hindex, index = LMENU_BASE; ii < m_arItem.GetSize(); ii++)
	{
		item = m_arItem.GetAt(ii);
		if (item->kind == SBAND_LIST)
		{
			subMenu = new CMenuXP();
			subMenu->CreatePopupMenu();
			menulist.Add(subMenu);
			InsertSubMenu(subMenu, item->mapN, index);
			menu.InsertMenuX(ii, nFlags, (UINT)subMenu->m_hMenu, item->tooltip);
		}
		else
		{
			sprintf(scrN, "%s", item->mapN);
			(*m_axiscall)(AXI_GETMAPNUM, 0, (LPARAM)scrN);
			str.Format("%s %s", scrN, item->tooltip);
			menu.AppendMenuX(MF_STRING, index++, str);
			m_commandList.Add(item->mapN);
		}
	}

	menu.TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON, xpoint.x, xpoint.y, this);
	menulist.RemoveAll();
#endif
	
}
void CSDIBand::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	HMENU hMenu = AfxGetThreadState()->m_hTrackingMenu;
	CMenu	*pMenu = CMenu::FromHandle(hMenu);
	pMenu->MeasureItem(lpMeasureItemStruct);
	
//	CWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CSDIBand::WriteToolItem()
{
	CProfile profile(pkUserTool);
	CString section(profile.GetString(szUserSN, USEREN, "0, 0"));

	CString	data, tmpS, id;
	int	idx = section.Find(',');

	if (idx == -1)	
		idx = 0;
	else
	{
		section = section.Left(idx);
		idx = atoi(section);
	}

	section.Format("%02d", idx);

	//** ??? ssL = GetPrivateProfileSection(section, ssb, sizeof(ssb), file);

#ifdef DF_USESTL_SDIBAND
	for (int ii = 0; ii < (int)m_arItem.size(); ii++)
	{
		auto pItem = m_arItem.at(ii);

		if (ii == 0)
			tmpS.Format("%s=%s;%s", pItem->mapN, pItem->tooltip, pItem->text);
		else
			tmpS.Format("%c%s=%s;%s", '\n', pItem->mapN, pItem->tooltip, pItem->text);
		data += tmpS;
}
#else
	CBandItem* pItem = NULL;
	for (int ii = 0; ii < m_arItem.GetSize(); ii++)
	{
		pItem = m_arItem.GetAt(ii);

		if (ii == 0)
			tmpS.Format("%s=%s;%s", pItem->mapN, pItem->tooltip, pItem->text);
		else
			tmpS.Format("%c%s=%s;%s", '\n', pItem->mapN, pItem->tooltip, pItem->text);
		data += tmpS;
	}
#endif

	data += '\0';
	profile.WriteSection(section, data);
}

int CSDIBand::GetDropIndex(CRect rc, CPoint point)
{
#ifdef DF_USESTL_SDIBAND
	CRect tRc, lRc;

	for (int ii = 0; ii < (int)m_arItem.size(); ii++)
	{
		tRc = GetToolRect(ii);

		if (ii == 0)
			lRc.SetRect(rc.left, rc.top, tRc.right, rc.bottom);
		else
			lRc.SetRect(tRc.left - BANDGAP, rc.top, tRc.right, rc.bottom);

		if (lRc.PtInRect(point))
			return ii;
	}

	return m_arItem.size();
#else
	CRect tRc, lRc;

	for (int ii = 0; ii < m_arItem.GetSize(); ii++)
	{
		tRc = GetToolRect(ii);

		if (ii == 0)
			lRc.SetRect(rc.left, rc.top, tRc.right, rc.bottom);
		else
			lRc.SetRect(tRc.left - BANDGAP, rc.top, tRc.right, rc.bottom);

		if (lRc.PtInRect(point))
			return ii;
	}

	return m_arItem.GetSize();
#endif
}

CImageList* CSDIBand::MakeDragImage(CString strImage, CWnd *pWnd)
{
	CRect	rectImage, rect;
	CDC	*pDC = pWnd->GetDC();

	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap{};

#ifdef DF_USESTL_SDIBAND
	std::unique_ptr<CBitmap> pDragBitmap = std::make_unique<CBitmap>();
	std::unique_ptr<CImageList> pDragImage = std::make_unique<CImageList>();
#else
	CBitmap* pDragBitmap = new CBitmap();
	CImageList* pDragImage = new CImageList();
#endif

	CFont font, *pOldFont;
	font.CreatePointFont(90, "굴림");
	pOldFont = MemDC.SelectObject(&font);

	MemDC.DrawText(strImage, rectImage, DT_CALCRECT);
	const int nBitCount = pDC->GetDeviceCaps(BITSPIXEL);
	pDragBitmap->CreateBitmap(rectImage.Width() + 4, rectImage.Height() + 4, 1, nBitCount, NULL);

#ifdef DF_USESTL_SDIBAND
	pOldBitmap = MemDC.SelectObject(pDragBitmap.get());
#else
	pOldBitmap = MemDC.SelectObject(pDragBitmap);
#endif
	
	CBrush	brush;
	brush.CreateSolidBrush(RGB(102,109,128));
	MemDC.SetBkColor(RGB(102,109,128));
	MemDC.SetTextColor(RGB(255, 255, 255));
	rect.SetRect(0, 0, rectImage.Width() + 4, rectImage.Height() + 4);
	MemDC.FillRect(&rect,&brush);
	rect.SetRect(2, 2, rectImage.Width() + 2, rectImage.Height() + 2);
	MemDC.DrawText(strImage, rect,  DT_LEFT | DT_SINGLELINE);
	MemDC.SelectObject(pOldBitmap);
	pWnd->ReleaseDC(pDC);

	pDragImage->Create(rectImage.Width() + 4, rectImage.Height() + 4, ILC_COLORDDB | ILC_MASK, 4, 4);
#ifdef DF_USESTL_SDIBAND
	pDragImage->Add(pDragBitmap.get(), RGB(192, 192, 192));
#else
	pDragImage->Add(pDragBitmap, RGB(192, 192, 192));
	delete pDragBitmap;
#endif

	MemDC.SelectObject(pOldFont);
	MemDC.DeleteDC();	
	brush.DeleteObject();	

#ifdef DF_USESTL_SDIBAND
	return pDragImage.release();
#else
	return pDragImage;
#endif

}

void CSDIBand::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CRect	cRc;
	const CRect rc;
	GetClientRect(cRc);
	
	const int		nItem = GetSelectItem(point);
	if (nItem != -1 && nItem != IEXPAND && nItem == m_hover)
	{

#ifdef DF_USESTL_SDIBAND
		auto item = m_arItem.at(nItem);
#else
		CBandItem* item = m_arItem.GetAt(nItem);
#endif

		
		CMenuXP menu;
		menu.CreatePopupMenu();
		CString strMenu;
		strMenu.Format("%s 삭제", item->tooltip);
		menu.AppendMenuX(MF_STRING, 1, strMenu);
		
		strMenu.Format("%s 이름수정", item->tooltip);
		menu.AppendMenuX(MF_STRING, 2, strMenu);
		
		CPoint xpoint = point;
		ClientToScreen(&xpoint);
		const int iChoice = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, xpoint.x, xpoint.y, this);
//		menu.DestroyMenu();
		
		if (iChoice == 1)		// delete item
		{
			ItemDEL(nItem);
			
			WriteToolItem();
			if (m_bSDI)	((class CSmain *)GetParent())->Refresh();
			else		((class CDbar1 *)GetParent())->Refresh();
		}
		else if (iChoice == 2)	// rename item
		{
#if 0
			CItemRename dlg;
			dlg.SetName(item->text);
			if (dlg.DoModal() == IDOK)
			{
				item->text = dlg.m_edit;
				
				WriteToolItem();
				if (m_bSDI)	((class CSmain *)GetParent())->Refresh();
				else		((class CDbar1 *)GetParent())->Refresh();
			}
#else
			CPoint curPos;
			GetCursorPos(&curPos);
			CRenameDlg dlg(this, item->text);
			dlg.m_point = curPos;
			if (dlg.DoModal() == IDOK)
			{
				item->text = dlg.m_sName;
				WriteToolItem();
				if (m_bSDI)	((class CSmain *)GetParent())->Refresh();
				else		((class CDbar1 *)GetParent())->Refresh();
			}
#endif
		}
	}
	else if (!m_bSDI)
	{
		CMenuXP menu;
		menu.CreatePopupMenu();
		menu.AppendMenuX(MF_STRING, 1, "2줄 툴바로 보기");
		
		CPoint xpoint = point;
		ClientToScreen(&xpoint);
		const int iChoice = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, xpoint.x, xpoint.y, this);
//		menu.DestroyMenu();
		
		if (iChoice == 1)		// delete item
			((class CDbar1 *)GetParent())->SetBandType(2);
	}
	
	CWnd::OnRButtonUp(nFlags, point);
}
