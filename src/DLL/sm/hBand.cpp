// hBand.cpp : implementation file
// 유저툴바

#include "stdafx.h"
#include "hBand.h"
#include "resource.h"
#include "MenuXP.h"

#include "../../axis/axMsg.hxx"
#include "../../h/axisvar.h"
#include "RenameDlg.h"
#include "NPalette.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
#define	XGAP		1
#define	YGAP		1
#define	EXPANDWIDTH	13
#define	IEXPAND	9999

#define LMENU_BASE	1000

#define	MN_REGISTER	1
#define	MN_RENAME	2
#define	MN_DELETE	3
#define	MN_RUN		4

#define	SIGN_MAP	'#'
#define SIGN_NEW	'@'
#define SIGN_REC	'$'
#define SIGN_EDIT	'^'
/////////////////////////////////////////////////////////////////////////////
// CHBand

CHBand::CHBand(bool (*axiscall)(int, WPARAM, LPARAM))
{
	m_axiscall = axiscall;
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

	m_pDragImage = NULL;
	m_bDragEnter = FALSE;
	m_nDragItem = -99;

	m_expandRc = CRect(0, 0, 0, 0);

	m_itcolorCool = COLOR_ITCOOL;
	m_itcolorOver = COLOR_ITOVER;
	m_itcolorDown = COLOR_ITDOWN;

	m_ToolTip = NULL;
	m_arItem.RemoveAll();
	m_arsubItem.RemoveAll();

	m_img[0] = Axis::GetSkinBitmap( "BAND" );
	m_img[1] = Axis::GetSkinBitmap( "TOOLBOX_MENU_MORE" );

	BITMAP	bm;
	m_img[1]->GetBitmap(&bm);
	m_expandW = bm.bmWidth/3;

	m_hNewIcon = AfxGetApp()->LoadIcon(IDI_NEW);
	m_hMedalIcon = AfxGetApp()->LoadIcon(IDI_MEDAL);
	m_hWebIcon = AfxGetApp()->LoadIcon(IDI_WEB);
	m_hEditIcon = AfxGetApp()->LoadIcon(IDI_EDIT);

	// 툴바 버튼
	LoadToolBox();
	
}

CHBand::~CHBand()
{
	m_font.DeleteObject();
	CBandItem* item{};
	for (int ii = 0; ii < m_arItem.GetSize(); ii++)
	{
#ifdef DF_USESTL_HBAND
		item = m_arItem.GetAt(ii).get();
#else
		item = m_arItem.GetAt(ii);
#endif
		
		delete item;
	}
	m_arItem.RemoveAll();

	for (int ii = 0; ii < m_arsubItem.GetSize(); ii++)
	{
#ifdef DF_USESTL_HBAND
		item = m_arsubItem.GetAt(ii).get();
#else
		item = m_arsubItem.GetAt(ii);
#endif
	
		delete item;
	}
	m_arsubItem.RemoveAll();

	m_img[0]->DeleteObject();
	m_img[1]->DeleteObject();

	if (m_hMedalIcon)
		DestroyIcon(m_hMedalIcon);
	if (m_hNewIcon)
		DestroyIcon(m_hNewIcon);
	if (m_hWebIcon)
		DestroyIcon(m_hWebIcon);
	if (m_hEditIcon)
		DestroyIcon(m_hEditIcon);
}


BEGIN_MESSAGE_MAP(CHBand, CWnd)
	//{{AFX_MSG_MAP(CHBand)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_MEASUREITEM()
	ON_WM_NCCALCSIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHBand message handlers

void CHBand::SetToolTipText(CString text, BOOL bActivate)
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

void CHBand::InitToolTip()
{
	if (!m_ToolTip)
	{
#ifdef DF_USESTL_HBAND
		m_ToolTip = std::make_unique<CToolTipCtrl>();
#else
		m_ToolTip = new CToolTipCtrl();
#endif
		
		m_ToolTip->Create(this, TTS_ALWAYSTIP);
		m_ToolTip->Activate(TRUE);
	}
}

BOOL CHBand::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	const int	comm = LOWORD(wParam) - LMENU_BASE;
	if (comm >= 0 && comm < m_commandList.GetSize())
	{
		CString mapN = m_commandList.GetAt(comm);
		(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(const char *)mapN);
	}
	return CWnd::OnCommand(wParam, lParam);
}

LRESULT CHBand::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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
			if (!m_ToolTip || !IsWindow(m_ToolTip->GetSafeHwnd()))
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

void CHBand::OnDestroy() 
{
	if (m_ToolTip)
	{
		if (m_ToolTip->m_hWnd)
		{
			m_ToolTip->Activate(FALSE);
			m_ToolTip->DelTool(this);
			//m_ToolTip->RelayEvent(NULL);
		}
#ifndef DF_USESTL_HBAND
		delete m_ToolTip;
#endif
		
	}

	CWnd::OnDestroy();
}

void CHBand::OnPaint() 
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

void CHBand::OnSize(UINT nType, int cx, int cy) 
{
	ReCalc();
	CWnd::OnSize(nType, cx, cy);
}

void CHBand::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CClientDC	dc(this);
	const int		nItem = GetSelectItem(point);

	SetCapture();
	m_pushON = true;
	m_nDragItem = -99;
	m_bDragEnter = FALSE;
	switch (nItem)
	{
	case -1:
		m_push   = -1;
		m_pushON = false;
		break;
	case IEXPAND:
		m_push = IEXPAND;
		draw_Expand(&dc, MODE_DOWN);
		break;
	default:
		drawItem(&dc, nItem, MODE_DOWN);
		m_push = nItem;
		m_nDragItem = nItem;
		break;
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CHBand::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();

	if (m_pDragImage)
	{
		m_pDragImage->DragLeave(NULL);
		m_pDragImage->EndDrag();
		m_pDragImage->DeleteImageList();
#ifndef DF_USESTL_HBAND
		delete m_pDragImage;
#endif
		m_pDragImage = NULL;
	}
	
	if (m_bDragEnter)	// OnEndDrag
	{
		m_bDragEnter = FALSE;

		if (GetSelectItem(point) == m_nDragItem || GetSelectItem(point) == -1)
		{
			if (GetSelectItem(point) == -1)
			{
				if (MessageBox("툴바에서 " + m_arItem.GetAt(m_nDragItem)->tooltip + " 버튼을 삭제하시겠습니까?", "툴바 버튼 삭제 확인창", MB_YESNO) == IDYES)
				{
					ItemDEL(m_nDragItem);
					SaveItems();
				}
			}
			m_nDragItem = -99;
			m_push = -1; 
			m_pushON = false;
			return;
		}

		CRect	cRc;
		//const CRect iRc;

		GetClientRect(cRc);
		for (int nItem = 0; nItem < m_arItem.GetSize(); nItem++)
		{
			const CRect	iRc = GetToolRect(nItem);

			if (iRc.right > cRc.right)
				break;
			if (iRc.PtInRect(point))
			{
				if (point.x > iRc.left + iRc.Width() / 2)	// right
					nItem++;
				m_arItem.InsertAt(nItem, m_arItem.GetAt(m_nDragItem));
				if (nItem < m_nDragItem)
					m_nDragItem++;
				m_arItem.RemoveAt(m_nDragItem);
				ReCalc();
				Invalidate();
				SaveItems();
				break;
			}
		}
		
		m_nDragItem = -99;
		m_push = -1; 
		m_pushON = false;
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
#ifdef DF_USESTL_HBAND
			const CBandItem* item = m_arItem.GetAt(nItem).get();
#else
			const CBandItem* item = m_arItem.GetAt(nItem);
#endif
			
			if (item->kind == BAND_ITEM)
				(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(const char *)item->mapN);
			else
			{
				const CRect	iRc = GetToolRect(nItem);
#if 0
				int dx = iRc.right - iRc.Width()/3;
				int dy = iRc.bottom - iRc.Height()/3;
				if ( (point.x>dx) && (point.y>dy) )
				{
					ShowItemList(item->mapN, CPoint(iRc.left, iRc.bottom));
				}
				else
				{
					CString mapN = GetSubItemData(item->mapN, 0);
					if (!mapN.IsEmpty())
						(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(LPCSTR)mapN);
				}
#else
				ShowItemList(item->mapN, CPoint(iRc.left, iRc.bottom));
#endif
			}
		}
		break;
	}

	m_bDragEnter = FALSE;
	m_nDragItem = -99;
	m_push = -1; 
	m_pushON = false;

	CWnd::OnLButtonUp(nFlags, point);
}

void CHBand::OnRButtonDown(UINT nFlags, CPoint point) 
{
	const int nItem = GetSelectItem(point);
	CPoint curPos;
	GetCursorPos(&curPos);

	if (nItem != -1)
	{

		CMenuXP		menu;
		menu.CreatePopupMenu();
		if (nItem >= 0 && nItem < m_arItem.GetSize())
		{
			menu.AppendMenuX(MF_STRING, MN_RUN, m_arItem.GetAt(nItem)->tooltip);
			menu.AppendMenuX(MF_SEPARATOR);
		}
		menu.AppendMenuX(MF_STRING, MN_REGISTER, "현재화면등록");
		if (nItem >= 0 && nItem < m_arItem.GetSize())
		{
			menu.AppendMenuX(MF_STRING, MN_RENAME, "이름바꾸기");
			menu.AppendMenuX(MF_STRING, MN_DELETE, "삭제");
		}

		const int nRtn = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, curPos.x, curPos.y, this);

		switch (nRtn)
		{
		case MN_REGISTER:
			GetParent()->SendMessage(WM_XMSG, MAKEWPARAM(CTRL_REGISTER, 0));
			break;
		case MN_RENAME:	// rename
			{
				if (nItem < 0 || nItem >= m_arItem.GetSize())
					break;
				CRenameDlg dlg(this, m_arItem.GetAt(nItem)->text);
				dlg.m_point = curPos;
				if (dlg.DoModal() == IDOK)
				{
					m_arItem.GetAt(nItem)->text = dlg.m_sName;
					Invalidate();
					SaveItems();
				}
			}
			break;
		case MN_DELETE: // delete
			ItemDEL(nItem);
			SaveItems();
			break;
		case MN_RUN:
			{
#ifdef DF_USESTL_HBAND
			const CBandItem* item = m_arItem.GetAt(nItem).get();
#else
			const CBandItem* item = m_arItem.GetAt(nItem);
#endif
			
				if (item->kind == BAND_ITEM)
					(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(const char *)item->mapN);
				else
				{
					const CRect	iRc = GetToolRect(nItem);
					ShowItemList(item->mapN, CPoint(iRc.left, iRc.bottom));
				}
			}
			break;
		}
	}

	CWnd::OnRButtonDown(nFlags, point);
}

void CHBand::OnMouseMove(UINT nFlags, CPoint point) 
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

	if (m_nDragItem >= 0 && GetSelectItem(lpoint) != m_nDragItem && !m_bDragEnter)
		m_bDragEnter = TRUE;

	if (m_bDragEnter && !m_pDragImage)
	{
		CDC *pDC = GetDC();
		CDC	memDC;
		CBitmap bitmap;
		CRect	itemRc = GetToolRect(m_nDragItem);
		itemRc.right -= 4;
		
		if (!memDC.CreateCompatibleDC(pDC))
		{
			ReleaseDC(pDC);
			return;
		}
		
		if (!bitmap.CreateCompatibleBitmap(pDC, itemRc.Width(), itemRc.Height()))
		{
			ReleaseDC(pDC);
			memDC.DeleteDC();
			return;
		}
		
		CBitmap* pOldMemDCBitmap = memDC.SelectObject(&bitmap);
		memDC.FillSolidRect(0, 0, itemRc.Width(), itemRc.Height(), RGB(255, 255,255));
		drawItem(&memDC, m_nDragItem, MODE_DRAG);

		memDC.SelectObject(pOldMemDCBitmap);
#ifdef DF_USESTL_HBAND
		m_pDragImage = std::make_unique<CImageList>();
#else
		m_pDragImage = new CImageList;
#endif
		
		m_pDragImage->Create(itemRc.Width(), itemRc.Height(), ILC_COLOR32 | ILC_MASK, 0, 1);
		m_pDragImage->Add(&bitmap, RGB(0, 255, 0)); 
		bitmap.DeleteObject();
		memDC.DeleteDC();
		ReleaseDC(pDC);

		m_pDragImage->BeginDrag(0, CPoint(-10, -20));
		POINT pt = point;
		ClientToScreen( &pt );
		m_pDragImage->DragEnter(NULL, pt);
	}

	if (m_bDragEnter && m_pDragImage)
	{
		const POINT pt = point;
		m_pDragImage->DragMove(pt);
	}
	
}

LRESULT CHBand::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_mouseIN = false;
	Invalidate(FALSE);
	m_hover = -1;
	return 0;
}

void CHBand::ReCalc()
{
	CRect	cRc, iRc;
	bool	expand = false;
	
	GetClientRect(cRc);
	for (int ii = 0; ii < m_arItem.GetSize() && !expand; ii++)
	{
		iRc = GetToolRect(ii);
		if (iRc.right + m_expandW > cRc.right)
		{
			expand = true;
			m_hindex = ii;
			m_expandRc.SetRect(cRc.right-m_expandW-XGAP*2, iRc.top, cRc.right-XGAP*2, iRc.bottom);
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
bool CHBand::ItemADD(CString dat, bool list)
{
	int	pos = dat.Find('=');
	if (pos == -1)	return false;

#ifdef DF_USESTL_HBAND
	auto item = std::make_shared<CBandItem>();
#else
	CBandItem* item = new CBandItem;
#endif
	
	item->mapN = dat.Left(pos++);
	/*
	if (atoi(item->mapN.Mid(2, 4)) == 0)
	{
		delete item;
		return false;
	}
	*/
	if (dat.IsEmpty())
	{	
#ifndef DF_USESTL_HBAND
		delete item;
#endif
		return false;
	}
	dat = dat.Mid(pos);

	if (list)	item->kind = BAND_LIST;
	else		item->kind = BAND_ITEM;

	pos = dat.Find(';');
	if (pos == -1)
	{
		if (list)
			item->text = item->tooltip = item->mapN;
		else	item->text = item->tooltip = item->mapN.Mid(2, 4);
		m_arItem.Add(item);
		ReCalc();
		return true;
	}
	
	item->tooltip = dat.Left(pos++);
	dat = dat.Mid(pos);
	item->text = dat;

	m_arItem.Add(item);
	ReCalc();
	return true;
}

// dat format : mapN=툴팁;단축명
bool CHBand::ItemINSERT(int nIdx, CString dat, bool list)
{
	int	pos = dat.Find('=');
	if (pos == -1)	return false;

#ifdef DF_USESTL_HBAND
	auto item = std::shared_ptr<CBandItem>();
#else
	CBandItem* item = new CBandItem;
#endif


	item->mapN = dat.Left(pos++);
	dat = dat.Mid(pos);

	if (list)	item->kind = BAND_LIST;
	else		item->kind = BAND_ITEM;

	pos = dat.Find(';');
	if (pos == -1)
	{
		if (list)
			item->text = item->tooltip = item->mapN;
		else	item->text = item->tooltip = item->mapN.Mid(2, 4);

#ifdef DF_USESTL_HBAND
		m_arItem.InsertAt(nIdx, item);
#else
		m_arItem.InsertAt(nIdx, item);
#endif
		
		ReCalc();
		return true;
	}
	
	item->tooltip = dat.Left(pos++);
	dat = dat.Mid(pos);
	item->text = dat;

	m_arItem.InsertAt(nIdx, item);
	ReCalc();
	return true;
}

// dat format : mapN=툴팁;단축명
bool CHBand::ItemSubADD(CString dat)
{
	int	pos = dat.Find('=');
	if (pos == -1)	return false;

#ifdef DF_USESTL_HBAND
	auto item = std::make_shared<CBandItem>();
#else
	CBandItem* item = new CBandItem;
#endif
	

	item->mapN = dat.Left(pos++);
	dat = dat.Mid(pos);

	item->kind = BAND_ITEM;

	pos = dat.Find(';');
	if (pos == -1)
	{
		item->text = item->tooltip = item->mapN.Mid(2, 4);
		m_arsubItem.Add(item);
		return true;
	}
	
	item->tooltip = dat.Left(pos++);
	dat = dat.Mid(pos);
	item->text = dat;

	m_arsubItem.Add(item);
	return true;
}

bool CHBand::ItemDEL(int index)
{
	if (index >= m_arItem.GetSize())	return false;
#ifndef DF_USESTL_HBAND
	delete m_arItem.GetAt(index);
#endif
	m_arItem.RemoveAt(index);
	ReCalc();
	return true;
}

void CHBand::removeALL()
{
	CBandItem* item{};

#ifndef DF_USESTL_HBAND
	for (int ii = 0; ii < m_arItem.GetSize(); ii++)
	{
		item = m_arItem.GetAt(ii);
		delete item;
	}
#endif

	m_arItem.RemoveAll();
}

void CHBand::HoverItem(CPoint point)
{
	if (!m_arItem.GetSize())
		return;

	const int nItem = GetSelectItem(point);
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
			default:	drawItem(&dc, m_push, MODE_DOWN);	break;
			case IEXPAND:	draw_Expand(&dc, MODE_DOWN);		break;
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
#ifdef DF_USESTL_HBAND
			item = m_arItem.GetAt(nItem).get();
#else
			item = m_arItem.GetAt(nItem);
#endif
			drawItem(&dc, nItem, MODE_OVER);
			m_ToolTip->SetToolRect(this, ID_TOOLTIP, GetToolRect(nItem));
			SetToolTipText(item->tooltip);		break;
		case IEXPAND:	
			draw_Expand(&dc, MODE_OVER);
			m_ToolTip->SetToolRect(this, ID_TOOLTIP, m_expandRc);
			SetToolTipText("More");			break;
		}
	}
	m_hover = nItem;
}

void CHBand::drawBackGround(CDC* pDC)
{
	CRect	cRc;
	BITMAP	bm;
	m_img[0]->GetBitmap(&bm);

	CDC	memDC;
	GetClientRect(cRc);
	if (memDC.CreateCompatibleDC(pDC))
	{
		CBitmap* oldbuffBitmap = memDC.SelectObject(m_img[0]);
		if (oldbuffBitmap != (CBitmap *) 0)
		{
			pDC->StretchBlt(cRc.left, cRc.top,  cRc.Width(), bm.bmHeight, 
					&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			/**
			pDC->FillSolidRect(cRc.right - 3, cRc.top, 3, 1, RGB(226, 227, 229));
			pDC->FillSolidRect(cRc.right - 3, cRc.top + 1, 3, 1, RGB(225, 235, 237));
			pDC->FillSolidRect(cRc.right - 3, cRc.top + 2, 3, 1, RGB(235, 235, 237));
			**/
			memDC.SelectObject(oldbuffBitmap);
		}
		else	
			memDC.DeleteDC();
	}
}

void CHBand::drawBandItem(CDC* pDC)
{
	for (int ii = 0; ii < m_arItem.GetSize(); ii++)
	{
		if (ii == m_hindex)	
			break;

		drawItem(pDC, ii);
	}

	if (!m_expandRc.IsRectEmpty())	
		draw_Expand(pDC);
}

void CHBand::drawItem(CDC* pDC, int idx, int mode)
{
	if (idx == -1 || idx >= m_arItem.GetSize())	
	{
		return;
	}
	
	CRect		iRc   = GetToolRect(idx);
	CFont*		pfont = pDC->SelectObject(&m_font);
	
#ifdef DF_USESTL_HBAND
	const CBandItem* item = m_arItem.GetAt(idx).get();
#else
	const CBandItem* item = m_arItem.GetAt(idx);
#endif
	COLORREF	color{};

	if (item->draw==1)
		mode = MODE_OVER;

	switch (mode)
	{
	case MODE_OVER:	color = pDC->SetTextColor(m_itcolorOver);	break;
	case MODE_DOWN:	color = pDC->SetTextColor(m_itcolorDown);	break;
	case MODE_DRAG:	
		color = pDC->SetTextColor(m_itcolorCool);	
		iRc.OffsetRect(-iRc.left, -iRc.top);
		mode = MODE_COOL;
		break;
	default:	
		color = pDC->SetTextColor(m_itcolorCool);	break;
	}

	draw_Img(pDC, iRc, item->kind, mode);

	FitTextRect(iRc);

	pDC->SetBkMode(TRANSPARENT);

	const UINT	nFlags = DT_CENTER |DT_WORDBREAK|DT_EDITCONTROL;
	pDC->DrawText(item->text, iRc, nFlags);

	pDC->SetTextColor(color);
	pDC->SelectObject(pfont);
}


void CHBand::draw_Img(CDC* pDC, CRect iRc, int kind, int mode)
{
	if (!m_bmpToolbox)
	{
		LoadToolBox();
		if (!m_bmpToolbox)
		{
			return;
		}
	}

	int	sx{};

	switch (mode)
	{
	case MODE_COOL:	sx = 0;		break;
	case MODE_OVER:	sx = m_itemW;	break;
	case MODE_DOWN:	sx = m_itemW*2;	break;
	default:	return;
	}

	CDC	memDC;

	if (kind == BAND_LIST)	
		sx += m_itemW*3;

	if (memDC.CreateCompatibleDC(pDC))
	{
		CBitmap* oldbuffBitmap = memDC.SelectObject(m_bmpToolbox);
		if (oldbuffBitmap != (CBitmap *) 0)
		{
			pDC->BitBlt(iRc.left, 1, m_itemW, m_itemH, &memDC, sx, 0, SRCCOPY);
			memDC.SelectObject(oldbuffBitmap);
		}
		memDC.DeleteDC();
	}
}

void CHBand::draw_Expand(CDC* pDC, int mode)
{
	int	sx = 0;
	switch (mode)
	{
	case MODE_OVER:	sx = m_expandW;		break;
	case MODE_DOWN:	sx = m_expandW*2;	break;
	}

	CDC	memDC;
	if (memDC.CreateCompatibleDC(pDC))
	{
		CBitmap* oldbuffBitmap = memDC.SelectObject(m_img[1]);
		if (oldbuffBitmap != (CBitmap *) 0)
		{
			BITMAP bm;
			m_img[1]->GetBitmap(&bm);
			pDC->BitBlt(m_expandRc.left, 1+9, bm.bmWidth / 3, bm.bmHeight, &memDC, sx, 0, SRCCOPY);
			memDC.SelectObject(oldbuffBitmap);
		}
		memDC.DeleteDC();
	}
}

void CHBand::change_Skin(CString skinName)
{

	Invalidate();
	return;

	/************************************************************************/
	/* 만일 툴박스 이미지가 스킨의 영향을 받는 경우 아래에서 이미지를 다시  */
	/* 로딩하고 각종 사이즈 계산을 다시 하도록 하면 됨.                     */
	/************************************************************************/
	/**
	CString	file;

	file.Format("%s\\image\\toolbox.bmp", Axis::home);

	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	if (m_bitmap.GetSafeHandle())
		m_bitmap.DeleteObject();
	if (hBitmap)	m_bitmap.Attach(hBitmap);

	if (!m_itemW && m_bitmap.GetSafeHandle())
	{
		BITMAP	bm;
		m_bitmap.GetBitmap(&bm);
		m_itemW = bm.bmWidth/6;
	}

	ReCalc();
	**/
}

void CHBand::FitTextRect(CRect& tRc)
{
	tRc.right -= 2;
	tRc.DeflateRect(5, 6, 3, 2);
}

void CHBand::ShowItemList(CString key, CPoint point)
{
	const char* const oldchart[] = {"IB700000","IB710000","IB720000","IB701000","IB702000","IB702100","IB740100","IB703000","IB704000","IB730000","IB731000"};

	char		scrN[16]{};
	CMenuXP		menu;
	CString		str;
#ifdef DF_USESTL_HBAND
	std::shared_ptr<CBandItem> item;
#else
	CBandItem* item{};
#endif
	
	ClientToScreen(&point);
	menu.CreatePopupMenu();

	if (key.Find('#') == -1)
		return;
	LoadSubItem(key);
	m_commandList.RemoveAll();

	char		buf[512];
	
	CString file;
	file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, "axisensetup.ini");
	
	const DWORD ret = GetPrivateProfileString("OLDCHARTCHANGE","CHANGE","NO",buf,sizeof(buf),file);
	
	CString strChange(buf);

	for (int ii = 0; ii < m_arsubItem.GetSize(); ii++)
	{
		item = m_arsubItem.GetAt(ii);

		if(strChange == "NO")
		{
			for(int i=0;i<ArraySize(oldchart);i++)
			{
				CString strOld(oldchart[i]);
				
				// 			CString s;
				// 			s.Format("BANK OLD NEW [%s] [%s]\n",mapN,strOld);
				// 			OutputDebugString(s);
				
				if(item->mapN == strOld)
				{
					item->mapN = GetNewChart(strOld);
				}
			}
		}

		sprintf(scrN, "%s", item->mapN.GetString());
		(*m_axiscall)(AXI_GETMAPNUM, 0, (LPARAM)scrN);
		str.Format("%s %s", scrN, item->tooltip);
		menu.AppendMenuX(MF_STRING, LMENU_BASE+ii, str);
		m_commandList.Add(item->mapN);
	}

	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
}

CRect CHBand::GetToolRect(int index)
{
	CRect	cRc, rc;

	GetClientRect(cRc);
	rc.SetRect(cRc.left+m_itemW*index, 0,
		cRc.left+m_itemW*(index+1), cRc.bottom);
	return rc;
}

void CHBand::LoadSubItem(CString section)
{
#ifdef DF_USESTL_HBAND
	std::shared_ptr<CBandItem> item;
#else
	CBandItem* item{};
#endif

	for (int ii = 0; ii < m_arsubItem.GetSize(); ii++)
	{
		item = m_arsubItem.GetAt(ii);
#ifndef DF_USESTL_HBAND
		delete item;
#endif
	}
	m_arsubItem.RemoveAll();

	CProfile profile(pkUserTool);
	CString buffer(profile.GetSectionLF(section));

	const int len = buffer.GetLength();

	if (len <= 0)
		return;

	CString subitem;
	for (; !buffer.IsEmpty(); )
	{
		int idx = buffer.Find('\n');
		if (idx == -1)	break;

		subitem  = buffer.Left(idx++);
		buffer = buffer.Mid(idx);

		idx = subitem.Find('=');
		if (idx == -1)	continue;

		ItemSubADD(subitem);
	}
}

CString CHBand::GetSubItemData( CString section, int idx )
{
	CProfile profile(pkUserTool);
	CString buffer(profile.GetSectionLF(section));
	
	if (buffer.IsEmpty()) return "";

	LPCSTR st = (LPCSTR)buffer;
	LPCSTR ed = st + buffer.GetLength();
	LPCSTR pos = st;

	int idx2 = 0;
	while(pos<ed)
	{
		pos = std::find(st, ed, '\n');
		{
			LPCSTR pos2 = std::find(st, pos, '=');
			if (idx==idx2) return CString(st, pos2-st);
		}
		st = pos + 1;
		++idx2;
	}
	return "";
}


void CHBand::SaveItems()
{
/*
	CProfile profile(pkUserTool);
	CString buffer(profile.GetString(szUserSN, USEREN, "0, 0"));
	int idx = buffer.Find(',');
	if (idx == -1) idx = 0;
	else
	{
		buffer = buffer.Left(idx);
		idx = atoi(buffer);
	}

	CString tmpS(Format("%02d", idx));
	profile.WriteSection(tmpS, "");

	for (int ii = 0; ii < m_arItem.GetSize(); ii++)
	{
		CBandItem *pItem = m_arItem.GetAt(ii);
		profile.Write(tmpS, pItem->mapN, pItem->tooltip + ";" + pItem->text +char(13)+char(10));
		
	}
*/
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

	CBandItem *pItem = NULL;
	for (int ii = 0, jj=0; ii < m_arItem.GetSize(); ii++)
	{
#ifdef DF_USESTL_HBAND
		pItem = m_arItem.GetAt(ii).get();
#else
		pItem = m_arItem.GetAt(ii);
#endif
		if (pItem->draw!=0)
		{
			++jj; 
			continue;
		}

		if ((ii-jj) == 0)
			tmpS.Format("%s=%s;%s", pItem->mapN, pItem->tooltip, pItem->text);
		else
			tmpS.Format("%c%s=%s;%s", '\n', pItem->mapN, pItem->tooltip, pItem->text);
		data += tmpS;
	}
	data += '\0';
	profile.WriteSection(section, data);

}

void CHBand::InsertSubMenu(CMenu* menu, CString section, int& index)
{
	CProfile profile(pkUserTool);

	CString buffer(profile.GetSectionLF(section));
	const int len = buffer.GetLength();
	if (len < 1)
		return;

	char	scrN[16]{};
	CString mapN, desc, str, subitem, string(buffer);

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

		sprintf(scrN, "%s", mapN.GetString());
		(*m_axiscall)(AXI_GETMAPNUM, 0, (LPARAM)scrN);
		str.Format("%s %s", scrN, desc);

		((CMenuXP*)menu)->AppendMenuX(MF_STRING, index++, str, GetScreenIcon(desc, mapN));
		m_commandList.Add(mapN);
	}
}

CString CHBand::GetNewChart(CString old)
{
	CString strReturn;
	
	if(old == "IB700000")
	{
		strReturn = "IB711100";
	}
	else if(old == "IB710000")
	{
		strReturn = "IB711300";
	}
	else if(old == "IB720000")
	{
		strReturn = "IB711400";
	}
	else if(old == "IB701000")
	{
		strReturn = "IB711100";
	}
	else if(old == "IB702000")
	{
		strReturn = "IB711100";
	}
	else if(old == "IB702100")
	{
		strReturn = "IB711100";
	}
	else if(old == "IB740100")
	{
		strReturn = "IB712300";
	}
	else if(old == "IB703000")
	{
		strReturn = "IB711100";
	}
	else if(old == "IB704000")
	{
		strReturn = "IB712000";
	}
	else if(old == "IB730000")
	{
		strReturn = "IB711600";
	}
	else if(old == "IB731000")
	{
		strReturn = "IB711100";
	}
	
	return strReturn;
}

int CHBand::GetSelectItem(CPoint pt)
{
	int	nItem = -1;
	CRect	iRc;
	
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

	if (m_hindex != -1 && m_expandRc.PtInRect(pt))
		nItem = IEXPAND;

	return nItem;
}

void CHBand::showMenu()
{
	CString		str;
	CBandItem* item{};
	CMenuXP		menu ;
#ifdef DF_USESTL_HBAND
	std::shared_ptr<CMenuXP> subMenu;
#else
	CMenuXP* subMenu;
#endif
	CPoint		xpoint = m_expandRc.BottomRight();
			
	CString theme = CNPalette::GetTheme();
	theme.MakeUpper();
	ClientToScreen(&xpoint);
	menu.CreatePopupMenu();
	if (theme.Compare("BLUE")==0)
	{
		menu.SetBorderColor(RGB(17,123,183));
		menu.SetSelectedBarColor(RGB(29,126,202));
		menu.SetSelectedTextColor(RGB(255,255,255));
	}else if (theme.Compare("GREEN")==0)
	{
		menu.SetBorderColor(RGB(55, 126, 58));
		menu.SetSelectedBarColor(RGB(63, 140, 62));
		menu.SetSelectedTextColor(RGB(255,255,255));
	}else if (theme.Compare("BROWN")==0)
	{
		menu.SetBorderColor(RGB(111,61,24));
		menu.SetSelectedBarColor(RGB(174, 115, 15));
		menu.SetSelectedTextColor(RGB(255,255,255));
	}else if (theme.Compare("VIOLET")==0)
	{
		menu.SetBorderColor(RGB(71, 18, 135));
		menu.SetSelectedBarColor(RGB(118, 60, 196));
		menu.SetSelectedTextColor(RGB(255,255,255));
	}else if (theme.Compare("GRAY")==0)
	{
		menu.SetBorderColor(RGB(55,72,98));
		menu.SetSelectedBarColor(RGB(73, 90, 116));
		menu.SetSelectedTextColor(RGB(255,255,255));
	}


	char	scrN[16]{};
	CArray	< CMenuXP*, CMenuXP* > menulist;
	const UINT	nFlags = MF_POPUP|MF_BYPOSITION|MF_STRING;

	m_commandList.RemoveAll();
	for (int ii = m_hindex, index = LMENU_BASE; ii < m_arItem.GetSize(); ii++)
	{
#ifdef DF_USESTL_HBAND
		item = m_arItem.GetAt(ii).get();
#else
		item = m_arItem.GetAt(ii);
#endif
		if (item->kind == BAND_LIST)
		{
#ifdef DF_USESTL_HBAND
			subMenu = std::make_shared<CMenuXP>();
#else
			subMenu = new CMenuXP();
#endif
			
			subMenu->CreatePopupMenu();

#ifdef DF_USESTL_HBAND
			menulist.Add(subMenu.get());
			InsertSubMenu(subMenu.get(), item->mapN, index);
#else
			menulist.Add(subMenu);
			InsertSubMenu(subMenu, item->mapN, index);
#endif
		
			
			menu.InsertMenuX(ii, nFlags, (UINT) subMenu->m_hMenu, item->tooltip);
		}
		else
		{
			
			sprintf(scrN, "%s", item->mapN.GetString());
			(*m_axiscall)(AXI_GETMAPNUM, 0, (LPARAM)scrN);
			str.Format("%s %s", scrN, item->tooltip);
			
			menu.AppendMenuX(MF_STRING, index++, str, GetScreenIcon(item->tooltip, item->mapN));
			m_commandList.Add(item->mapN);
		}
	}

	menu.TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON, xpoint.x, xpoint.y, this);
	menulist.RemoveAll();
}

void CHBand::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	HMENU hMenu = AfxGetThreadState()->m_hTrackingMenu;
	CMenu	*pMenu = CMenu::FromHandle(hMenu);
	pMenu->MeasureItem(lpMeasureItemStruct);
//	CWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CHBand::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	//lpncsp->rgrc[0].top--;
	//lpncsp->rgrc[0].bottom++;
	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CHBand::LoadToolBox()
{	
	m_bmpToolbox = Axis::GetSkinBitmap("toolbox");

	if (m_bmpToolbox)
	{
		BITMAP bm;
		m_bmpToolbox->GetBitmap(&bm);
		m_itemW = bm.bmWidth / 6;
		m_itemH = bm.bmHeight;
	}
	else {
		m_itemW = m_itemH = 0;
	}
}

HICON CHBand::GetScreenIcon(CString Desc, CString scrNo)
{
	CString	file;
	char	buf[1024 * 2]{};

	file = GetProfileFileName(pkAxisMenu);
	//m_commandCnt = 0;
	FILE *fp = fopen(file, "r");
	if (!fp)
	{
		Invalidate();
		return NULL;
	}
	CString data(_T(""));
	CString find(_T(""));
	char kind{};
	int pos = -1;
	pos = Desc.Find("::");
	if (pos>-1)	find = Desc.Mid(pos+2);
	else find = Desc;
	/*
	pos = -1;
	pos = Desc.Find("::");
	if (pos>-1)	Desc = Desc.Mid(pos+1);
	*/

	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		//makeMenu(buf);
		data = CString(buf);
		
		pos = data.Find(find);
		if (pos>-1)
		{
			//AfxMessageBox(Desc);
			kind = data.GetAt(pos-1);
			//AfxMessageBox(kind);
			switch(kind)
			{
			case SIGN_NEW:
				fclose(fp);
				return m_hNewIcon;
				break;
			case SIGN_REC:
				fclose(fp);
				return m_hMedalIcon;
				break;

			case SIGN_EDIT:
				fclose(fp);
				return m_hEditIcon;
				break;
			/*
			case SIGN_MAP:
				fclose(fp);
				return NULL;
				break;
			*/
			}
			if (IsWebMenu(scrNo))
			{
				fclose(fp);
				return m_hWebIcon;
				break;
			}

			break;
		}
	}
	fclose(fp);
	return NULL;
}

BOOL CHBand::IsWebMenu(const char *MapName)
{
	
	CProfile profile(Format("%s\\%s\\WEBINFO.INI", Axis::home, TABDIR));
	//AfxMessageBox("["+CString(MapName).Mid(2, 4)+"]"+profile.GetString(CString(MapName).Mid(2, 4), "Type"));
	return !profile.GetString(CString(MapName).Mid(2, 4), "Type").IsEmpty();
}
