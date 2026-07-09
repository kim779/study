// hBand.cpp : implementation file
//

#include "stdafx.h"
#include "hBand.h"
#include "resource.h"
#include "MenuXP.h"
#include "dbar1.h"
#include "ItemRename.h"

#include "../../axis/axMsg.hxx"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
#define BANDGAP		4
#define	XGAP		1
#define	YGAP		1
#define	EXPANDWIDTH	13
#define	IEXPAND	9999
#define	ICOMMON	9990

#define LMENU_BASE	1000
/////////////////////////////////////////////////////////////////////////////
// CHBand

CHBand::CHBand(bool (*axiscall)(int, WPARAM, LPARAM), CString home)
{
	m_axiscall = axiscall;
	m_home = home;
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

	m_itcolorCool = COLOR_ITCOOL;
	m_itcolorOver = COLOR_ITOVER;
	m_itcolorDown = COLOR_ITDOWN;

	m_ToolTip = NULL;
	m_arItem.RemoveAll();
	m_arsubItem.RemoveAll();
	m_arCommonItem.RemoveAll();

	m_img[0].LoadBitmap(IDB_BAR1BACK);
	m_img[1].LoadBitmap(IDB_EXPAND);

	BITMAP	bm;
	m_img[1].GetBitmap(&bm);
	m_expandW = bm.bmWidth/3;

	m_bDrag = false;
	m_pDragImage = NULL;
}

CHBand::~CHBand()
{
	m_font.DeleteObject();
	CBandItem*	item;
	int ii = 0;

	for (ii = 0; ii < m_arItem.GetSize(); ii++)
	{
		item = m_arItem.GetAt(ii);
		delete item;
	}
	m_arItem.RemoveAll();

	for (ii = 0; ii < m_arsubItem.GetSize(); ii++)
	{
		item = m_arsubItem.GetAt(ii);
		delete item;
	}
	m_arsubItem.RemoveAll();

	m_img[0].DeleteObject();
	m_img[1].DeleteObject();
}


BEGIN_MESSAGE_MAP(CHBand, CWnd)
	//{{AFX_MSG_MAP(CHBand)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MEASUREITEM()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_CREATE()
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
		m_ToolTip = new CToolTipCtrl();
		m_ToolTip->Create(this, TTS_ALWAYSTIP);
		m_ToolTip->Activate(TRUE);
	}
}

BOOL CHBand::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int	comm = LOWORD(wParam) - LMENU_BASE;
	if (comm >= 0 && comm < m_commandList.GetSize())
	{
		CString mapN = m_commandList.GetAt(comm);
		(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(char *)mapN.operator LPCTSTR());
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
			m_ToolTip->RelayEvent(NULL);
		}
		delete m_ToolTip;
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
			drawCommonItem(&memDC);
			drawBandItem(&memDC);
			
			//dc.FillSolidRect(cRc, RGB(255,0,0));
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
	int		nItem = GetSelectItem(point);

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
		draw_Expand(&dc, MODE_DOWN);
		break;
	default:
		m_rcSelected = GetToolRect(nItem);
		drawItem(&dc, nItem, MODE_DOWN);
		m_push = nItem;
		break;
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CHBand::OnLButtonUp(UINT nFlags, CPoint point) 
{
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
		CBandItem *pItem = m_arItem.GetAt(m_push);

		CRect cRc;
		GetClientRect(&cRc);
		cRc.InflateRect(100, 0);
		if (cRc.PtInRect(point))
		{
			int index = GetDropIndex(cRc, point);

			if (index >= 0 && index != m_push)
			{
				m_arItem.InsertAt(index, pItem);

				if (index < m_push)
					m_arItem.RemoveAt(m_push + 1);
				else
					m_arItem.RemoveAt(m_push);

				WriteToolItem();
				((class CDbar1 *)GetParent())->Refresh();
			}
		}
		else
		{
			if (MessageBox(pItem->text + "를 툴바그룹에서 삭제하시겠습니까?", "사용자 툴바 아이템 삭제", MB_YESNO) == IDYES)
			{
				ItemDEL(m_push);
				
				WriteToolItem();
				((class CDbar1 *)GetParent())->Refresh();
			}
		}

		m_push = -1;
		m_pushON = false;
		m_bDrag = false;

		CWnd::OnLButtonUp(nFlags, point);
		
		return;
	}

	CClientDC	dc(this);
	int		nItem = GetSelectItem(point);
	
	switch (m_push)
	{
	case -1:	break;
	case IEXPAND:
		draw_Expand(&dc);
		if (nItem == m_push)
			showMenu();
		break;
	case ICOMMON:
	case ICOMMON + 1:
	case ICOMMON + 2:
	case ICOMMON + 3:
		{
			drawCommon(&dc, m_push);
			if (nItem == m_push)
			{
				CBandItem*	item = m_arCommonItem.GetAt(nItem - ICOMMON);
				if (item->kind == BAND_ITEM)
				{
					if(m_push == ICOMMON)
						(*m_axiscall)(AXI_FUNCKEY, 29, 0);
					else if(m_push == ICOMMON + 1)
						(*m_axiscall)(AXI_FUNCKEY, 36, 0);
					else if(m_push == ICOMMON + 2)
						(*m_axiscall)(AXI_FUNCKEY, 37, 0);
					else if(m_push == ICOMMON + 3)
						(*m_axiscall)(AXI_FUNCKEY, 38, 0);		
				}
					//(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(char *)item->mapN.operator LPCTSTR());
				else
				{
					CRect	iRc = GetCommonRect(nItem - ICOMMON);
					ShowItemList(item->mapN, CPoint(iRc.left, iRc.bottom));
				}
			}
		}
		break;
	default:
		drawItem(&dc, m_push);
		if (nItem == m_push)
		{
			CBandItem*	item = m_arItem.GetAt(nItem);
			if (item->kind == BAND_ITEM)
				(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(char *)item->mapN.operator LPCTSTR());
			else
			{
				CRect	iRc = GetToolRect(nItem);
				ShowItemList(item->mapN, CPoint(iRc.left, iRc.bottom));
			}
		}
		break;
	}

	m_push = -1; 
	m_pushON = false;

	CWnd::OnLButtonUp(nFlags, point);
}


void CHBand::OnMouseMove(UINT nFlags, CPoint point) 
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
			if (m_push >= m_arItem.GetSize())	return;

			CBandItem *item = m_arItem.GetAt(m_push);

			if (m_pDragImage)
			{
				delete m_pDragImage;
				m_pDragImage = NULL;
			}
			//m_pDragImage = MakeDragImage(item->text, this);  //마우스 드래그 기능 없음
			
			if (!m_pDragImage) 
				return;
			
			CPoint	pt;	
			IMAGEINFO ii;
			m_pDragImage->GetImageInfo(0, &ii);
			pt.x = (ii.rcImage.right - ii.rcImage.left) / 2;
			pt.y = (ii.rcImage.bottom - ii.rcImage.top) / 2;
			
			m_pDragImage->BeginDrag(0, pt);
			pt = lpoint;
			ClientToScreen(&pt);
			m_pDragImage->DragEnter(NULL, pt);
			
			//m_bDrag = true;
			m_bDrag = false ; //마우스 드래그 기능 없음
			m_rcSelected.SetRectEmpty();
			return;
		}
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
	//	if (iRc.right + m_expandW > cRc.right)
		if (iRc.right + m_expandW > cRc.right - m_expandW - (m_itemW * 4))
		{
			expand = true;
			m_hindex = ii;
			m_expandRc.SetRect(cRc.right-m_expandW , iRc.top, cRc.right, iRc.bottom);
			m_expandRc.OffsetRect(-(m_itemW * 4), 0);
		}
	}

	if (!expand)
	{
		m_hindex = -1;
		m_expandRc.SetRectEmpty();
	}

	Invalidate(FALSE);
}

bool CHBand::CommonItemADD()
{
	CBandItem*	item = new CBandItem;
	/*
	item->mapN = _T("");
	item->tooltip = _T("메뉴툴바");
	item->text = _T("메뉴툴바");
	item->kind = BAND_ITEM;
	m_arCommonItem.Add(item);		
	*/
	for(int ii = 0 ; ii < 4 ; ii++)
	{
		CBandItem*	item = new CBandItem;
		switch(ii)
		{
			case 0:
				item->mapN = _T("");
				item->tooltip = _T("종목검색");
				item->text = _T("종목검색");
				break;
			case 1:
				item->mapN = _T("");
				item->tooltip = _T("관심종목");
				item->text = _T("관심종목");
				break;
			case 2:
				item->mapN = _T("");
				item->tooltip = _T("메뉴툴바");
				item->text = _T("메뉴툴바");
				break;
			case 3:
				item->mapN = _T("");
				item->tooltip = _T("메뉴탭바");
				item->text = _T("메뉴탭바");
				break;
		}
		item->kind = BAND_ITEM;
		m_arCommonItem.Add(item);
	}
	
	
	ReCalc();
	return true;
}
// dat format : mapN=툴팁;단축명
bool CHBand::ItemADD(CString dat, bool list)
{
	int	pos = dat.Find('=');
	if (pos == -1)	return false;

	CBandItem*	item = new CBandItem;

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
		m_arItem.Add(item);
		ReCalc();
		return true;
	}
	
	item->tooltip = dat.Left(pos++);
	item->tooltip.Replace(_T("#"), _T(""));
	dat = dat.Mid(pos);
	item->text = dat;

	m_arItem.Add(item);
	ReCalc();
	return true;
}

// dat format : mapN=툴팁;단축명
bool CHBand::ItemSubADD(CString dat)
{
	int	pos = dat.Find('=');
	if (pos == -1)	return false;

	CBandItem*	item = new CBandItem;

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

	delete m_arItem.GetAt(index);
	m_arItem.RemoveAt(index);
	ReCalc();
	return true;
}

void CHBand::removeALL()
{
	CBandItem*	item;
	for (int ii = 0; ii < m_arItem.GetSize(); ii++)
	{
		item = m_arItem.GetAt(ii);
		delete item;
	}
	m_arItem.RemoveAll();
}

void CHBand::HoverItem(CPoint point)
{
	if (!m_arItem.GetSize())
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
		CBandItem*	item;
		switch (m_hover)
		{
		case -1:	break;
		default:	drawItem(&dc, m_hover);	break;
		case IEXPAND:	draw_Expand(&dc);	break;
		}

		switch (nItem)
		{
			case -1:	break;
			case ICOMMON:
			case ICOMMON + 1:
			case ICOMMON + 2:
			case ICOMMON + 3:
			{
				item = m_arCommonItem.GetAt(nItem - ICOMMON);
				drawCommon(&dc, nItem, MODE_OVER);
				m_ToolTip->SetToolRect(this, ID_TOOLTIP, GetCommonRect(nItem - ICOMMON));
				SetToolTipText(item->tooltip);
			}
			break;
			default:
				item = m_arItem.GetAt(nItem);
				drawItem(&dc, nItem, MODE_OVER);
				m_ToolTip->SetToolRect(this, ID_TOOLTIP, GetToolRect(nItem));
				SetToolTipText(item->tooltip);		
			break;
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
	GetClientRect(cRc);
	pDC->FillSolidRect(cRc, COLOR_FRAME);

	//CPen	pen1(PS_SOLID, 1, RGB(128, 128, 128));
	CPen	pen1(PS_SOLID, 1, RGB(76, 81, 90));
	CPen	pen2(PS_SOLID, 1, RGB(255, 255, 255));


	CPen	*pOldPen = pDC->SelectObject(&pen1);
	pDC->MoveTo(0, 0);
	pDC->LineTo(cRc.right, 0);
	
	pDC->SelectObject(&pen2);
	pDC->MoveTo(0, 1);
	pDC->LineTo(cRc.right, 1);

	pDC->SelectObject(pOldPen);
}


void CHBand::drawCommonItem(CDC* pDC)
{
	for (int ii = 0; ii < m_arCommonItem.GetSize(); ii++)
	{
		drawCommon(pDC, ii + ICOMMON);
	}

}

void CHBand::drawCommon(CDC* pDC, int idx, int mode)
{
//	if (idx == -1 || idx >= m_arItem.GetSize())	
//		return;

	CRect		iRc   = GetCommonRect(idx - ICOMMON);
	CFont*		pfont = pDC->SelectObject(&m_font);
	CBandItem*	item  = m_arCommonItem.GetAt(idx - ICOMMON);
	COLORREF	color;

	switch (mode)
	{
	case MODE_OVER:	
		color = pDC->SetTextColor(m_itcolorOver);	break;
	case MODE_DOWN:	
		color = pDC->SetTextColor(m_itcolorDown);	break;
	default:	color = pDC->SetTextColor(m_itcolorCool);	break;
	}

	iRc.left -= 1;
	draw_Img(pDC, iRc, item->kind, mode);
	FitTextRect(iRc);
	pDC->SetBkMode(TRANSPARENT);

	if (item->text.GetLength() > 4)
	{
		UINT	nFlags = DT_CENTER |DT_WORDBREAK|DT_EDITCONTROL;
		pDC->DrawText(item->text, iRc, nFlags);
	}
	else
	{
		UINT	nFlags = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
		pDC->DrawText(item->text, iRc, nFlags);
	}
	pDC->SetTextColor(color);
	pDC->SelectObject(pfont);
}

void CHBand::drawBandItem(CDC* pDC)
{
	for (int ii = 0; ii < m_arItem.GetSize(); ii++)
	{
		if (ii == m_hindex)	
			break;
		drawItem(pDC, ii);
	}

	if (!m_expandRc.IsRectEmpty())	draw_Expand(pDC);
}

void CHBand::drawItem(CDC* pDC, int idx, int mode)
{

	if (idx == -1 || idx >= m_arItem.GetSize())	return;
	
	CRect		iRc   = GetToolRect(idx);
	CFont*		pfont = pDC->SelectObject(&m_font);
	CBandItem*	item  = m_arItem.GetAt(idx);
	COLORREF	color;


	switch (mode)
	{
	case MODE_OVER:	
		color = pDC->SetTextColor(m_itcolorOver);	break;
	case MODE_DOWN:	
		color = pDC->SetTextColor(m_itcolorDown);	break;
	default:	color = pDC->SetTextColor(m_itcolorCool);	break;
	}

	//if(idx != 0)
	//	iRc.OffsetRect(-1, 0);
	draw_Img(pDC, iRc, item->kind, mode);
	FitTextRect(iRc);
	pDC->SetBkMode(TRANSPARENT);

	if (item->text.GetLength() > 4)
	{
		UINT	nFlags = DT_CENTER |DT_WORDBREAK|DT_EDITCONTROL;
		pDC->DrawText(item->text, iRc, nFlags);
	}
	else
	{
		UINT	nFlags = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
		pDC->DrawText(item->text, iRc, nFlags);
	}
	pDC->SetTextColor(color);
	pDC->SelectObject(pfont);
}


void CHBand::draw_Img(CDC* pDC, CRect iRc, int kind, int mode)
{
	CDC	memDC;
	int	sx;

	switch (mode)
	{
	case MODE_COOL:	sx = 0;		break;
	case MODE_DOWN:	sx = m_itemW;	break;
	case MODE_OVER:	sx = m_itemW*2;	break;
	default:	return;
	}

	if (kind == BAND_LIST)	sx += m_itemW*3;
	if (memDC.CreateCompatibleDC(pDC))
	{
		CBitmap* oldbuffBitmap = memDC.SelectObject(&m_bitmap);
		if (oldbuffBitmap != (CBitmap *) 0)
		{
			
			pDC->BitBlt(iRc.left, iRc.top, m_itemW, iRc.Height(), 
					&memDC, sx, 0, SRCCOPY);
					
			/*
			pDC->StretchBlt(iRc.left, iRc.top,  m_itemW + 1, iRc.Height(), &memDC,
			sx, 0, m_itemW , iRc.Height(), SRCCOPY);
			*/
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
		CBitmap* oldbuffBitmap = memDC.SelectObject(&m_img[1]);
		if (oldbuffBitmap != (CBitmap *) 0)
		{
			pDC->BitBlt(m_expandRc.left, m_expandRc.top, m_expandRc.Width(),
					m_expandRc.Height(), &memDC, sx, 0, SRCCOPY);
			memDC.SelectObject(oldbuffBitmap);
		}
		memDC.DeleteDC();
	}
}

void CHBand::change_Skin(CString skinName)
{
	CString	file;

	file.Format("%s\\image\\mb_toolbox2.bmp", m_home);
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
}

void CHBand::FitTextRect(CRect& tRc)
{
	//tRc.right -= 4;
	tRc.DeflateRect(3, 3);
	tRc.OffsetRect(0,4);
}

void CHBand::ShowItemList(CString key, CPoint point)
{
	char		scrN[16];
	CMenuXP		menu;
	CString		str;
	CBandItem*	item;
	ClientToScreen(&point);
	menu.CreatePopupMenu();

	if (key.Find('#') == -1)
		return;
	LoadSubItem(key);
	m_commandList.RemoveAll();
	for (int ii = 0; ii < m_arsubItem.GetSize(); ii++)
	{
		item = m_arsubItem.GetAt(ii);
		sprintf_s(scrN, "%s", item->mapN);
		(*m_axiscall)(AXI_GETMAPNUM, 0, (LPARAM)scrN);
		str.Format("%s %s", scrN, item->tooltip);
		menu.AppendMenuX(MF_STRING, LMENU_BASE+ii, str);
		m_commandList.Add(item->mapN);
	}

	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
}

CRect CHBand::GetCommonRect(int index)
{
	CRect	cRc, rc;

	GetClientRect(cRc);
	rc.SetRect(cRc.right - m_itemW*(4 - index), 1,
			cRc.right - m_itemW*(3 - index), cRc.bottom );
	//rc.SetRect(cRc.right - m_itemW*(1 -index), 1,
	//		cRc.right - m_itemW*(index), cRc.bottom );
	if(index != 0)
		rc.OffsetRect(index * -1, 0);
	return rc;
}

CRect CHBand::GetToolRect(int index)
{
	CRect	cRc, rc;

	GetClientRect(cRc);
	rc.SetRect(cRc.left+m_itemW*index, 1,
		cRc.left+m_itemW*(index+1), cRc.bottom);
	if(index != 0)
		rc.OffsetRect(index * -1, 0);
	return rc;
}

void CHBand::LoadSubItem(CString section)
{
	CBandItem*	item;
	for (int ii = 0; ii < m_arsubItem.GetSize(); ii++)
	{
		item = m_arsubItem.GetAt(ii);
		delete item;
	}
	m_arsubItem.RemoveAll();

	char	ssb[1024*4];
	CString	file, tmpS;
	CString user = GetUserName();

	file.Format("%s\\%s\\%s\\usertool.ini", m_home, USRDIR, user);
	DWORD ssL = GetPrivateProfileSection(section, ssb, sizeof(ssb), file);
	if (ssL <= 0)	return;

	CString subitem, string = CString(ssb, ssL);
	for (; !string.IsEmpty(); )
	{
		int idx = string.Find('\0');
		if (idx == -1)	break;

		subitem  = string.Left(idx++);
		string = string.Mid(idx);

		idx = subitem.Find('=');
		if (idx == -1)	continue;

		ItemSubADD(subitem);
	}
}

CString CHBand::GetUserName()
{
	CString userN = AfxGetApp()->GetProfileString(WORKSTATION, SIGNONID);
	int pos = userN.Find('|');
	if (pos < 0)	
		userN = _T("Anonymous");
	else	
		userN = userN.Mid(pos+1);

	if (userN.IsEmpty())	
		userN = _T("Anonymous");
	return userN;
}

void CHBand::InsertSubMenu(CMenu* menu, CString section, int& index)
{
	char	ssb[1024*4];
	CString	file, tmpS, mapN, desc, str;
	CString user = GetUserName();

	file.Format("%s\\%s\\%s\\usertool.ini", m_home, USRDIR, user);
	DWORD ssL = GetPrivateProfileSection(section, ssb, sizeof(ssb), file);
	if (ssL <= 0)	return;

	char	scrN[16];
	CString subitem, string = CString(ssb, ssL);
	for (; !string.IsEmpty(); )
	{
		int idx = string.Find('\0');
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

		sprintf_s(scrN, "%s", mapN);
		(*m_axiscall)(AXI_GETMAPNUM, 0, (LPARAM)scrN);
		str.Format("%s %s", scrN, desc);

		((CMenuXP*)menu)->AppendMenuX(MF_STRING, index++, str);
		m_commandList.Add(mapN);
	}
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


	if(nItem == -1) //공통툴버튼인지 체크
	{
		for (int ii = 0; ii < m_arCommonItem.GetSize(); ii++)
		{
			if (ii == m_hindex)	
				break;
			iRc = GetCommonRect(ii);
			if (iRc.PtInRect(pt))
			{
				nItem = ii + ICOMMON;
				break;
			}
		}
	}


	return nItem;
}

void CHBand::showMenu()
{
	CString		str;
	CBandItem*	item;
	CMenuXP		menu, *subMenu = NULL;
	CPoint		xpoint = m_expandRc.BottomRight();
			
	ClientToScreen(&xpoint);
	menu.CreatePopupMenu();

	char	scrN[16];
	CArray	< CMenuXP*, CMenuXP* > menulist;
	UINT	nFlags = MF_POPUP|MF_BYPOSITION|MF_STRING;

	m_commandList.RemoveAll();
	for (int ii = m_hindex, index = LMENU_BASE; ii < m_arItem.GetSize(); ii++)
	{
		item = m_arItem.GetAt(ii);
		if (item->kind == BAND_LIST)
		{
			subMenu = new CMenuXP();
			subMenu->CreatePopupMenu();
			menulist.Add(subMenu);
			InsertSubMenu(subMenu, item->mapN, index);
			menu.InsertMenuX(ii, nFlags, (UINT) subMenu->m_hMenu, item->tooltip);
		}
		else
		{
			sprintf_s(scrN, "%s", item->mapN);
			(*m_axiscall)(AXI_GETMAPNUM, 0, (LPARAM)scrN);
			str.Format("%s %s", scrN, item->tooltip);
			menu.AppendMenuX(MF_STRING, index++, str);
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

void CHBand::WriteToolItem()
{
	char	ssb[1024*4];
	CString	file, data, section, tmpS;
	CString id, user = GetUserName();
	file.Format("%s\\%s\\%s\\usertool.ini", m_home, USRDIR, user);
	DWORD ssL = GetPrivateProfileString(USERSN, USEREN, "0, 0", ssb, sizeof(ssb), file);
	section = CString(ssb, ssL);
	int	idx = section.Find(',');
	if (idx == -1)	idx = 0;
	else
	{
		section = section.Left(idx);
		idx = atoi(section);
	}

	section.Format("%02d", idx);
	ssL = GetPrivateProfileSection(section, ssb, sizeof(ssb), file);

	CBandItem *pItem = NULL;
	for (int ii = 0; ii < m_arItem.GetSize(); ii++)
	{
		pItem = m_arItem.GetAt(ii);

		if (ii == 0)
			tmpS.Format("%s=%s;%s", pItem->mapN, pItem->tooltip, pItem->text);
		else
			tmpS.Format("%c%s=%s;%s", '\n', pItem->mapN, pItem->tooltip, pItem->text);
		data += tmpS;
	}
	data += '\0';
	WritePrivateProfileSection(section, data, file);
}

int CHBand::GetDropIndex(CRect rc, CPoint point)
{
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

	return (int)m_arItem.GetSize();
}

CImageList* CHBand::MakeDragImage(CString strImage, CWnd *pWnd)
{
	CRect	rectImage, rect;
	CDC	*pDC = pWnd->GetDC();

	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap;
	CBitmap *pDragBitmap = new CBitmap();
	CImageList *pDragImage = new CImageList();

	CFont font, *pOldFont;
	font.CreatePointFont(90, "굴림");
	pOldFont = MemDC.SelectObject(&font);

	MemDC.DrawText(strImage, rectImage, DT_CALCRECT);
	int nBitCount = pDC->GetDeviceCaps(BITSPIXEL);
	pDragBitmap->CreateBitmap(rectImage.Width() + 4, rectImage.Height() + 4, 1, nBitCount, NULL);
	pOldBitmap = MemDC.SelectObject(pDragBitmap);
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
	pDragImage->Add(pDragBitmap, RGB(192, 192, 192));
	delete pDragBitmap;

	MemDC.SelectObject(pOldFont);
	MemDC.DeleteDC();	
	brush.DeleteObject();	

	return pDragImage;
}

int CHBand::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CommonItemADD();
	change_Skin("");
	return 0;
}
