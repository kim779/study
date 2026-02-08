// fxSheet.cpp : implementation file
//

#include "stdafx.h"
#include "fxSheet.h"
#include "resource.h"
#include "misctype.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int timerID = 100;
/////////////////////////////////////////////////////////////////////////////
// CwkButton

CwkButton::CwkButton(int buttonID, bool repeatLBDN)
{
	m_mouse = false;
	m_flate = true;

	m_buttonID   = buttonID;
	m_repeatLBDN = repeatLBDN;
}

CwkButton::~CwkButton()
{
}

BEGIN_MESSAGE_MAP(CwkButton, CButton)
	//{{AFX_MSG_MAP(CwkButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CwkButton message handlers

void CwkButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	SetTimer (timerID, 10, NULL);
	CButton::OnMouseMove(nFlags, point);
}

void CwkButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_mouse = true;
	CButton::OnLButtonDown(nFlags, point);

	if (m_repeatLBDN)
	{
		HWND hWnd = GetSafeHwnd();
		HWND hWndParent = GetParent()->GetSafeHwnd();

		MSG msg;
		while (!PeekMessage(&msg, hWnd, WM_LBUTTONUP, WM_LBUTTONUP, PM_REMOVE))
		{
			Sleep(100);
			::SendMessage(hWndParent, WM_USER+1004, m_buttonID, 0);
		}

		SendMessage(WM_LBUTTONUP);
	}
}

void CwkButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_mouse = false;
	CButton::OnLButtonUp(nFlags, point);
}

void CwkButton::OnTimer(UINT nIDEvent) 
{
	CRect	btnRC;
	GetWindowRect(&btnRC);

	CPoint	curPT;
	GetCursorPos(&curPT);

	static bool drawing = false;

	if (m_mouse || !btnRC.PtInRect(curPT))
	{
		KillTimer (timerID);
		if (drawing) InvalidateRect (NULL);
		drawing = false;
		return;
	}

	if (m_flate)
	{
		CDC *pDC = GetDC();
		if (!drawing)
		{
			GetClientRect(&btnRC);
			pDC->FillSolidRect(btnRC, ::GetSysColor(COLOR_BTNFACE));

			if (GetIcon())
			{
				setIconRect();
				DrawIconEx(pDC->GetSafeHdc(), m_rcIcon.left,
					m_rcIcon.top, GetIcon(),
					m_rcIcon.Width(), m_rcIcon.Height(),
					NULL, (HBRUSH)NULL, DI_NORMAL); 
			}
			
			// draw the button rect.
			pDC->Draw3dRect(btnRC, GetSysColor(COLOR_BTNHIGHLIGHT), GetSysColor(COLOR_BTNSHADOW));
			drawing = true;
		}

		ReleaseDC (pDC);
	}

	CButton::OnTimer(nIDEvent);
}

void CwkButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	ASSERT(lpDrawItemStruct != NULL);	

	CDC* pDC = GetDC();
	UINT itemS = lpDrawItemStruct->itemState;

	// copy the rect, and fill the background.
	m_rcButton.CopyRect(&lpDrawItemStruct->rcItem);	
	pDC->FillSolidRect(m_rcButton, ::GetSysColor(COLOR_BTNFACE));

	// define rect to be used for left button down.
	setIconRect();

	if (m_flate)
	{
		if (itemS & ODS_SELECTED)
			pDC->Draw3dRect(m_rcButton, GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNHIGHLIGHT));
		else
			pDC->Draw3dRect(m_rcButton, GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNFACE));
	}
	else
	{
		CRect rc(m_rcButton);
		rc.DeflateRect(1,1);
		
		if (itemS & ODS_SELECTED)
		{	// Draw button pressed.
			pDC->Draw3dRect(m_rcButton, GetSysColor(COLOR_3DDKSHADOW), GetSysColor(COLOR_BTNHIGHLIGHT));
			pDC->Draw3dRect(rc, GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNFACE));
		}
		else
		{	// Draw button raised.
			pDC->Draw3dRect(m_rcButton, GetSysColor(COLOR_BTNHIGHLIGHT), GetSysColor(COLOR_3DDKSHADOW));
			pDC->Draw3dRect(rc, GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNSHADOW));
		}
	}

	// Save the item state, set background to transparent.
	pDC->SetBkMode(TRANSPARENT);

	// if an icon is associated with button, draw it.
	if(GetIcon())
	{
		DrawIconEx (pDC->GetSafeHdc(), m_rcIcon.left,
				m_rcIcon.top, GetIcon(), m_rcIcon.Width(),
				m_rcIcon.Height(), NULL, (HBRUSH)NULL, DI_NORMAL);
	}

	ReleaseDC (pDC);
}

void CwkButton::SetIcon(HICON hIcon, CSize cSize)
{
	CButton::SetIcon(hIcon);

	m_szIcon.cx = cSize.cx;
	m_szIcon.cy = cSize.cy;
}

void CwkButton::setIconRect()
{
	CRect rcClient, rcWindow;
	GetClientRect(&rcClient);
	GetWindowRect(&rcWindow);

	CPoint cPos;
	GetCursorPos(&cPos);

	CPoint ptCenter = rcClient.CenterPoint();

	int x1 = (rcClient.Width() - ptCenter.x) / 2 - 1;
	int x2 = x1 + rcClient.Width() - 1;
	int y1 = (rcClient.Height()-ptCenter.y) / 2 - 1;
	int y2 = y1 + rcClient.Height() - 1;

	m_rcIcon.CopyRect(rcClient);
	if (m_mouse)
	{
		if (rcWindow.PtInRect(cPos))
			m_rcIcon.OffsetRect(1,1);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CfxSheet

const int angleV = 8;
const int buttonW = 17;
const int buttonH = 17;

#define	idBUTTON	1001
#define	sheetIMG	"sheet.bmp"

CfxSheet::CfxSheet(CString imageDir/*_T("")*/, CFont* defaultFont/*=NULL*/, CFont* activeFont/*=NULL*/)
{
	m_defaultFont = defaultFont;
	m_activeFont  = activeFont;

	m_drawRC.SetRectEmpty();
	m_scrollV = 0;
	m_scrollMAX = 0;
	m_imageDir = _T("");
	if (imageDir != _T(""))	m_imageDir.Format("%s\\%s", imageDir, sheetIMG);

	m_active = 0;
	m_activeFG = PALETTERGB(  0,   0,   0);
	m_activeBK = PALETTERGB(255, 255, 255);		// 액티브 백그라운드
	m_colorLN  = PALETTERGB(128, 128, 128);		// 테두리 라인

	m_array.RemoveAll();

	m_mouse = false;
	m_move  = -1;

	m_parent = (CWnd *) 0;
	m_first = true;
}

CfxSheet::~CfxSheet()
{
	for (int ii = 0; ii < scrollBTN; ii++)
	{
		delete m_button[ii];
		m_button[ii] = NULL;
	}
	if (m_image)
	{
		m_image->DeleteImageList();
		delete m_image;
	}

	for (ii = 0; ii < m_array.GetSize(); ii++)
		delete m_array.GetAt(ii);
	m_array.RemoveAll();
}


BEGIN_MESSAGE_MAP(CfxSheet, CWnd)
	//{{AFX_MSG_MAP(CfxSheet)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(idBUTTON, idBUTTON+scrollBTN, OnCommandButton)
	ON_MESSAGE(WM_USER+1004, OnRepeatLButtonDown)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CfxSheet message handlers
CWnd* CfxSheet::Initial(CWnd* parent, UINT parentID, COLORREF bk, bool bottom)
{
	ASSERT(parent && IsWindow(parent->m_hWnd));

	CRect	clientRC; parent->GetClientRect(&clientRC);

	if (bottom)
		clientRC.top = clientRC.bottom - GetSystemMetrics(SM_CYVTHUMB);
	else
		clientRC.bottom = clientRC.top + GetSystemMetrics(SM_CYVTHUMB);

	m_colorBK  = bk; if (m_colorBK == (COLORREF) 0) m_colorBK = GetSysColor(COLOR_BTNFACE);
	m_parent   = parent;
	m_parentID = parentID;
	m_bottomPos= bottom;

	m_brushBK.DeleteObject();
	m_brushBK.CreateSolidBrush(m_colorBK);
	parent->ModifyStyle(0, WS_CLIPCHILDREN);

	if (m_first)
	{
		m_first = false;

		CString	classname = AfxRegisterWndClass(0,
			AfxGetApp()->LoadStandardCursor(IDC_ARROW),(HBRUSH)m_brushBK, 0);

		UINT	style = WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
		if (Create(classname, _T(""), style, clientRC, parent, parentID))
			return this;
	}
	return this;
}

int CfxSheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	HINSTANCE hinst = AfxGetResourceHandle();
	AfxSetResourceHandle(::GetModuleHandle("fx_misc.dll"));

	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_image = new CImageList();
	m_image->Create(IDB_SHEETIMAGE, buttonW, 1, RGB(255, 0, 255));
	m_image->SetBkColor(CLR_NONE);

	AfxSetResourceHandle(hinst);

	CBitmap	cBitmap;
	HBITMAP	hBitmap = (HBITMAP) LoadImage(NULL, m_imageDir, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); 
	if (hBitmap)
	{
		m_image->DeleteImageList();

		cBitmap.Attach(hBitmap);
		m_image->Create(buttonW, buttonH, ILC_COLORDDB|ILC_MASK, 1, 1);
		m_image->Add(&cBitmap, RGB(255, 0, 255));
		m_image->SetBkColor(CLR_NONE);

		cBitmap.DeleteObject();
		DeleteObject(hBitmap);
	}

	GetClientRect(&m_drawRC);
	m_buttonRC = m_drawRC;

	m_drawRC.left += buttonW * scrollBTN;
	m_buttonRC.right = m_buttonRC.left + buttonW * scrollBTN;

	m_image->SetBkColor(m_colorBK);

	CRect	btnRC = m_buttonRC;
	for (int ii = 0; ii < scrollBTN; ii++)
	{
		m_button[ii] = new CwkButton(idBUTTON+ii, false);

		if (!m_button[ii]->Create(NULL,
			WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_NOTIFY|BS_OWNERDRAW|BS_ICON,
			CRect(0, 0, 0, 0), this, idBUTTON+ii))
		{
			TRACE("button create error.....\n");
			return -1;
		}

		m_button[ii]->SetIcon(m_image->ExtractIcon(ii), CSize(buttonW, buttonH));

		btnRC.right = btnRC.left + buttonW;
		m_button[ii]->MoveWindow(btnRC);
		btnRC.left += buttonW;
	}

	OnCommandButton(idBUTTON);
	return 0;
}

bool CfxSheet::SetItem(const SVITEM* item)
{
	if (!item) return false;

	SVITEM* sheet;
	for (int ii = 0; ii < m_array.GetSize(); ii++)
	{
		sheet = m_array.GetAt(ii);
		if (sheet->key == item->key)
		{
			CopyMemory(sheet, item, sizeof(SVITEM));
			m_array.SetAt(ii, sheet);
			return true;
		}
	}

	sheet = (SVITEM *) new char[sizeof(SVITEM)];

	CopyMemory(sheet, item, sizeof(SVITEM));
	sheet->rect.SetRectEmpty();
	sheet->angle = 0;

	m_array.Add(sheet);

	return true;
}

bool CfxSheet::GetItem(SVITEM *item)
{
	if (!item) return false;

	SVITEM* sheet;
	for (int ii = 0; ii < m_array.GetSize(); ii++)
	{
		sheet = m_array.GetAt(ii);
		if (sheet->key == item->key)
		{
			CopyMemory(item, sheet, sizeof(SVITEM));
			return true;
		}
	}

	return false;
}

int CfxSheet::GetActiveKey()
{
	if (m_active >= m_array.GetSize())
		return -1;

	SVITEM* sheet = m_array.GetAt(m_active);
	return sheet->key;
}

void CfxSheet::SetActiveKey(int key)
{
	SVITEM* sheet;
	for (int ii = 0; ii < m_array.GetSize(); ii++)
	{
		sheet = m_array.GetAt(ii);
		if (sheet->key == key)
		{
			CSize	sz;
			CGdiObject* oldFont;

			CDC* dc = GetDC();

			if (sheet->font != NULL) oldFont = dc->SelectObject(sheet->font);
			else oldFont = dc->SelectStockObject(SYSTEM_FONT);
			sz = dc->GetOutputTextExtent(sheet->desc);
			dc->SelectObject(oldFont);

			ReleaseDC(dc);

			m_scrollMAX += (sz.cx + angleV + 4);

			m_active = ii;
			Invalidate(); break;
		}
	}

	m_scrollV = ((m_scrollMAX+angleV) - m_drawRC.Width()) * -1;
	if (m_scrollV > 0) m_scrollV = 0;
}

bool CfxSheet::SetFgColor(int key, COLORREF fgcol)
{
	SVITEM* sheet;
	for (int ii = 0; ii < m_array.GetSize(); ii++)
	{
		sheet = m_array.GetAt(ii);
		if (sheet->key == key)
		{
			sheet->mask |= SVMK_FGCOLOR;
			if (fgcol == (COLORREF) 0)
				sheet->mask &= ~SVMK_FGCOLOR;

			sheet->fgcol = fgcol;
			Invalidate(); break;
		}
	}

	return true;
}

bool CfxSheet::SetBkColor(int key, COLORREF bkcol)
{
	SVITEM* sheet;
	for (int ii = 0; ii < m_array.GetSize(); ii++)
	{
		sheet = m_array.GetAt(ii);
		if (sheet->key == key)
		{
			sheet->mask |= SVMK_BKCOLOR;
			if (bkcol == (COLORREF) 0)
				sheet->mask &= ~SVMK_BKCOLOR;

			sheet->bkcol = bkcol;
			Invalidate(); break;
		}
	}

	return true;
}

bool CfxSheet::SetItemFont(int key, CFont* font)
{
	SVITEM* sheet;
	for (int ii = 0; ii < m_array.GetSize(); ii++)
	{
		sheet = m_array.GetAt(ii);
		if (sheet->key == key)
		{
			sheet->mask |= SVMK_FONT;
			sheet->font = font;
			Invalidate(); break;
		}
	}

	return true;
}

void CfxSheet::Adjust()
{
	if (!m_parent || !GetParent()) return;

	CRect	clientRC;
	m_parent->GetClientRect(&clientRC);

	if (m_bottomPos)
		clientRC.top = clientRC.bottom - GetSystemMetrics(SM_CYVTHUMB);
	else
		clientRC.bottom = clientRC.top + GetSystemMetrics(SM_CYVTHUMB);
	
	m_drawRC.left = clientRC.left + (buttonW * scrollBTN);
	m_drawRC.right = clientRC.right;

	SetWindowPos(NULL, clientRC.left, clientRC.top, clientRC.right, clientRC.bottom, SWP_SHOWWINDOW);
//	MoveWindow(clientRC);
	Invalidate();
}

void CfxSheet::Refresh()
{
	CDC* dc = GetDC();
	drawingSheet(dc);
	ReleaseDC(dc);
}

bool CfxSheet::Delete(int key)
{
	SVITEM* sheet;
	for (int ii = 0; ii < m_array.GetSize(); ii++)
	{
		sheet = m_array.GetAt(ii);
		if (sheet->key == key)
		{
			delete sheet;
			m_array.RemoveAt(ii);
			Refresh();
			return true;
		}
	}

	return false;
}

void CfxSheet::DeleteAll(bool refresh)
{
	for (int ii = 0; ii < m_array.GetSize(); ii++)
		delete m_array.GetAt(ii);
	m_array.RemoveAll();

	if (refresh) Refresh();
}

bool CfxSheet::Append(SVITEM* item)
{
	if (!item) return false;

	SVITEM* sheet;
	for (int ii = 0; ii < m_array.GetSize(); ii++)
	{
		sheet = m_array.GetAt(ii);
		if (sheet->key == item->key)
			return false;
	}

	sheet = (SVITEM*) new char[sizeof(SVITEM)];
	CopyMemory(sheet, item, sizeof(SVITEM));
	sheet->rect.SetRectEmpty();
	sheet->angle = 0;

	m_array.Add(sheet); Refresh();
	return true;
}

void CfxSheet::drawingSheet(CDC * pDC)
{
	if (m_drawRC.IsRectNull())
		return;

	CDC	dc;
	CBitmap	bitmap;
	if (!dc.CreateCompatibleDC(pDC)) return;
	if (!bitmap.CreateCompatibleBitmap(pDC, m_drawRC.Width(), m_drawRC.Height())) return;

	int saveid = dc.SaveDC();

	CBitmap* pOldBitmap = dc.SelectObject(&bitmap);

	CBrush* pOldBrush = dc.SelectObject(&m_brushBK);
	dc.PatBlt(0, 0, m_drawRC.Width(), m_drawRC.Height(), PATCOPY);

	static const int ptCount = 4;

	CRect	activeRC, dtRC;
	CPoint	dtPT[ptCount], activePT[ptCount];
	CRgn	activeRgn;
	CString	activeSTR;
	CFont*	activeFont = NULL;

	m_scrollMAX = 0;

	CString	string;
	int cx = m_scrollV + angleV;

	SVITEM* sheet;
	for (int ii = 0; ii < m_array.GetSize(); ii++)
	{
		sheet = m_array.GetAt(ii);
		string = sheet->desc;

		if (m_defaultFont != NULL)
			dc.SelectObject(m_defaultFont);
		else
			dc.SelectStockObject(SYSTEM_FONT);

		dc.SetTextColor(RGB(0, 0, 0));
		dc.SetBkMode(TRANSPARENT);

		if (sheet->mask & SVMK_FONT)
			dc.SelectObject(sheet->font);
		if (sheet->mask & SVMK_FGCOLOR)
			dc.SetTextColor(sheet->fgcol);
		if (sheet->mask & SVMK_BKCOLOR)
			dc.SetBkColor(sheet->bkcol);

		dtRC.SetRect(cx, 0, 0, 0);
		dc.DrawText(string, dtRC, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_CALCRECT);

		dtRC.left  -= 1;
		dtRC.top    = 0;
		dtRC.right += 10;
		dtRC.bottom = m_drawRC.Height();

		sheet->rect = dtRC;

		dtPT[0] = CPoint(dtRC.left - angleV, 0);
		dtPT[1] = CPoint(dtRC.left, dtRC.bottom);
		dtPT[2] = CPoint(dtRC.BottomRight());
		dtPT[3] = CPoint(dtRC.right + angleV, 0);

		CRgn dtRgn;
		dtRgn.CreatePolygonRgn(dtPT, ptCount, WINDING);
		dc.FillRgn (&dtRgn, &CBrush(m_colorBK));
		if (sheet->mask & SVMK_BKCOLOR) dc.FillRgn (&dtRgn, &CBrush(sheet->bkcol));
		dc.FrameRgn(&dtRgn, &CBrush(m_colorLN), 1, 1);
		dc.DrawText(string, dtRC, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

		if (m_active == ii)		// active index
		{
			activeRgn.CreatePolygonRgn(dtPT, ptCount, ALTERNATE);
			activeRC = dtRC;
			activeSTR = string;

			if (sheet->mask & SVMK_FONT)
				activeFont = sheet->font;
			for (int jj = 0; jj < ptCount; jj++)
				activePT[jj] = dtPT[jj];
		}

		cx  += dtRC.Width()+angleV;
		m_scrollMAX += dtRC.Width()+angleV;
	}

	if (m_active >= 0)	// active index
	{
		dc.FillRgn (&activeRgn, &CBrush(m_activeBK));
		dc.FrameRgn(&activeRgn, &CBrush(m_colorLN), 1, 1);
		dc.SetTextColor(m_activeFG);

		if (m_activeFont != NULL)
			dc.SelectObject(m_activeFont);
		else if (activeFont != NULL)
			dc.SelectObject(activeFont);
		else
			dc.SelectStockObject(SYSTEM_FONT);

		dc.DrawText(activeSTR, activeRC, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}

	if (m_move != -1)
	{
		sheet = m_array.GetAt(m_move);
		dtRC = sheet->rect;

		CPen	pen(PS_SOLID, 1, RGB(0, 0, 255));
		CBrush	brush(RGB(0, 255, 0));

		dc.SelectObject(&pen);
		dc.SelectObject(&brush);

		if (sheet->angle == 1)	// left
		{
			dtPT[0] = CPoint(dtRC.left-angleV, dtRC.top);
			dtPT[1] = CPoint(dtRC.left-angleV/2, dtRC.top+angleV);
			dtPT[2] = CPoint(dtRC.left, dtRC.top);
			dtPT[3] = CPoint(dtRC.left-angleV, dtRC.top);

			dc.Polygon(dtPT, ptCount);
		}
		else if (sheet->angle == 2)
		{
			dtPT[0] = CPoint(dtRC.right+angleV, dtRC.top);
			dtPT[1] = CPoint(dtRC.right+angleV/2, dtRC.top+angleV);
			dtPT[2] = CPoint(dtRC.right, dtRC.top);
			dtPT[3] = CPoint(dtRC.right+angleV, dtRC.top);

			dc.Polygon(dtPT, ptCount);
		}
	}

	pDC->BitBlt(m_drawRC.left, m_drawRC.top, m_drawRC.Width(), m_drawRC.Height(), &dc, 0,0, SRCCOPY);

	dc.RestoreDC(saveid);
}

BOOL CfxSheet::OnEraseBkgnd(CDC* pDC) 
{
	drawingSheet(pDC);
	for (int ii = 0; ii < scrollBTN; ii++)
	{
		if (m_button[ii] == NULL)
			continue;
		m_button[ii]->Invalidate();
	}

	pDC->ExcludeClipRect(m_drawRC);
	CBrush* pOldBrush = pDC->SelectObject(&m_brushBK);
	CRect rect;
	pDC->GetClipBox(&rect);
	pDC->SelectObject(pOldBrush);
	
	return CWnd::OnEraseBkgnd(pDC);
}

void CfxSheet::OnCommandButton(UINT nID)
{
	int shiftCount = 0;

	switch (nID)
	{
	case idBUTTON:			// left shift all
		m_scrollV = 0;
		Invalidate();
		return;
	case idBUTTON+1:		// left shift
		shiftCount = getScrollWidth(false);
		break;
	case idBUTTON+2:		// right shift
		shiftCount = getScrollWidth(true) * (-1);
		break;
	case idBUTTON+3:		// right shift all
		m_scrollV = ((m_scrollMAX+angleV) - m_drawRC.Width()) * -1;
		if (m_scrollV > 0) m_scrollV = 0;
		Invalidate();
		return;
	default:
		return;
	}

	m_scrollV += shiftCount;
	if (abs(m_scrollV) + m_drawRC.Width() - m_scrollMAX > 0)
		m_scrollV = ((m_scrollMAX+angleV) - m_drawRC.Width()) * -1;
	if (m_scrollV > 0) m_scrollV = 0;
	Invalidate();
}

LONG CfxSheet::OnRepeatLButtonDown(WPARAM wParam, LPARAM lParam)
{
	int shiftCount;

	switch ((int)wParam)
	{
	case idBUTTON+1:		// left shift
		shiftCount = getScrollWidth(false);
		break;
	case idBUTTON+2:		// right shift
		shiftCount = getScrollWidth(true) * (-1);
		break;
	default:
		return 0;
	}

	m_scrollV += shiftCount;
	if (abs(m_scrollV) + m_drawRC.Width() - m_scrollMAX > 0)
		m_scrollV = ((m_scrollMAX+angleV) - m_drawRC.Width()) * -1;
	if (m_scrollV > 0) m_scrollV = 0;

	CDC* dc = GetDC();
	drawingSheet(dc);
	ReleaseDC(dc);

	return 1;
}

void CfxSheet::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SVITEM* sheet;
	point.x -= m_buttonRC.Width();

	for (int ii = 0; ii < m_array.GetSize(); ii++)
	{
		sheet = m_array.GetAt(ii);
		if (!sheet->rect.PtInRect(point))
			continue;
		
		if (messageToParent(sheet->key, SVNM_LBTNDOWN, point) == -1)
			break;

		if (m_active != ii)
		{
			m_active = ii;
			Invalidate();
		}

		m_mouse = true; SetTimer(timerID, 500, NULL);
		break;
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CfxSheet::OnTimer(UINT nIDEvent) 
{
	KillTimer(timerID);

	CPoint	curpt; GetCursorPos(&curpt);
	ScreenToClient(&curpt);
	curpt.x -= m_buttonRC.Width(); curpt.y = 1;

	SVITEM* sheet = m_array.GetAt(m_active);
	CRect rect = sheet->rect;
	if (m_mouse && rect.PtInRect(curpt))
	{
		sheet->angle = 2;
		if (rect.left + rect.Width() / 2 >= curpt.x)
			sheet->angle = 1;

		m_move = m_active;
		Invalidate();

		SetCapture();
		messageToParent(sheet->key, SVNM_BEGINDRAG, CPoint(0, 0));
	}

	CWnd::OnTimer(nIDEvent);
}

void CfxSheet::OnMouseMove(UINT nFlags, CPoint point) 
{
	point.x -= m_buttonRC.Width(); point.y = 1;

	if (m_mouse && m_move != -1)
	{
		CRect	rect; SVITEM* sheet;
		for (int ii = 0; ii < m_array.GetSize(); ii++)
		{
			sheet = m_array.GetAt(ii);
			rect = sheet->rect; rect.InflateRect(angleV-1, 0);
			if (rect.PtInRect(point))
			{
				sheet->angle = 2;
				if (rect.left + rect.Width() / 2 >= point.x)
					sheet->angle = 1;

				if (m_move != ii)
				{
					sheet = m_array.GetAt(m_move);
					sheet->angle = 0;
					m_move = ii;
				}
				Refresh();
				break;
			}
		}

		if (m_move != ii)
		{
			if (m_buttonRC.right >= point.x)
			{
				sheet = m_array.GetAt(m_move);
				sheet->angle = 1;
				m_move = 0;
			}
			else if (m_scrollMAX <= point.x)
			{
				sheet = m_array.GetAt(m_move);
				sheet->angle = 2;
				m_move = m_array.GetUpperBound();
			}
			Refresh();
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CfxSheet::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_mouse = false;
	::ReleaseCapture();

	if (m_move != -1)
	{
		SVITEM *sheet, *sheetx;
		sheet = m_array.GetAt(m_move);
		sheet->angle = 0;

		if (m_active != m_move)
		{
			sheet = (SVITEM*) new char[sizeof(SVITEM)];
			sheetx = m_array.GetAt(m_active);

			CopyMemory(sheet, sheetx, sizeof(SVITEM));
			delete sheetx; m_array.RemoveAt(m_active);
			if (m_move >= m_array.GetSize())
			{
				m_array.Add(sheet);
				m_active = m_array.GetUpperBound();
			}
			else
			{
				m_array.InsertAt(m_move, sheet);
				m_active = m_move;
			}
		}
		messageToParent(sheet->key, SVNM_ENDDRAG, CPoint(0, 0));
		m_move = -1; Refresh();
	}
	CWnd::OnLButtonUp(nFlags, point);
}

void CfxSheet::OnRButtonDown(UINT nFlags, CPoint point) 
{
	point.x -= m_buttonRC.Width();

	CRect	rect; SVITEM* sheet;
	for (int ii = 0; ii < m_array.GetSize(); ii++)
	{
		sheet = m_array.GetAt(ii);
		rect = sheet->rect; rect.InflateRect(angleV-1, 0);
		if (rect.PtInRect(point))
		{
			messageToParent(sheet->key, SVNM_RBTNDOWN, point);
			break;
		}
	}
	
	CWnd::OnRButtonDown(nFlags, point);
}

int CfxSheet::getScrollWidth(bool bNext)
{
	int nWidth = 0;

	SVITEM* sheet;
	for (int ii = 0; ii < m_array.GetSize(); ii++)
	{
		sheet = m_array.GetAt(ii);
		if (sheet->rect.right <= 0)
			continue;

		nWidth = sheet->rect.Width() + angleV;
		if (sheet->rect.left < 0)
		{
			nWidth = abs(sheet->rect.left) + angleV;
		}
		else if (!bNext && ii > 0)
		{
			sheet = m_array.GetAt(ii -1);
			nWidth = sheet->rect.Width() + angleV;
		}
		break;
	}

	return nWidth;
}

LRESULT CfxSheet::messageToParent(int key, int msg, CPoint pt)
{
	if (!IsWindow(m_hWnd)) return 0;

	NM_SVITEM nmsheet;

	nmsheet.key = key;
	nmsheet.pt  = pt;

	nmsheet.hdr.hwndFrom = m_hWnd;
	nmsheet.hdr.idFrom = m_parentID;
	nmsheet.hdr.code = msg;

	CWnd* owner = GetOwner();
	if (owner && IsWindow(owner->m_hWnd))
		return owner->SendMessage(WM_NOTIFY, m_parentID, (LPARAM)&nmsheet);
	return 0;
}