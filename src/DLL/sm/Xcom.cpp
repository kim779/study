// Xcom.cpp : implementation file
//

#include "stdafx.h"
#include "Xcom.h"
#include "xedit.h"

#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	COLOR_BUTTON	::GetSysColor(COLOR_BTNFACE)
/////////////////////////////////////////////////////////////////////////////
// CXcom

CXcom::CXcom(UINT id)
{
	m_id = id;
	m_edit = NULL;
	m_bRc.SetRectEmpty();
	m_push = false;

	m_arr = Axis::GetBitmap("xcombo");
	m_arr_dn = Axis::GetBitmap("xcombo_dn");
	m_bmpXCombBack = Axis::GetSkinBitmap("XCOMB_BACK");
}

CXcom::~CXcom()
{
#ifndef DF_USESTL_XCOM
	if (m_edit)	delete m_edit;
#endif

	m_font.DeleteObject();

	if (m_pen.GetSafeHandle())		m_pen.DeleteObject();
	if (m_brush.GetSafeHandle())	m_brush.DeleteObject();
}


BEGIN_MESSAGE_MAP(CXcom, CWnd)
	//{{AFX_MSG_MAP(CXcom)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SELECT, OnSelect)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXcom message handlers

void CXcom::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	client;
	GetClientRect(client);

	CBrush* pbr{};
	CPen*	pen = dc.SelectObject(&m_pen);
	if (m_edit)
		pbr = dc.SelectObject(&(m_edit->m_brush));
	//dc.Rectangle(client);
	dc.SelectObject(pen);
	if (m_edit)
		dc.SelectObject(pbr);

	draw_Button(&dc);
}

void CXcom::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	m_push = true;

	Invalidate(FALSE);

	CWnd::OnLButtonDown(nFlags, point);
}

void CXcom::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	m_push = false;

	if (m_bRc.PtInRect(point))
	{
		if (m_edit)	m_edit->show_History();
	}

	Invalidate(FALSE);
	//m_edit->SetSel(0, -1);
	//m_edit->SetFocus();

	CWnd::OnLButtonUp(nFlags, point);
}

void CXcom::init(bool (*callback)(int, WPARAM, LPARAM))
{
	m_pen.CreatePen(PS_SOLID, 0, COLOR_COMLINE);
	m_brush.CreateSolidBrush(COLOR_BUTTON);
#ifdef DF_USESTL_XCOM
	m_edit = std::make_unique<CXedit>();
	const DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | ES_LEFT |
		ES_NOHIDESEL | ES_UPPERCASE | ES_AUTOHSCROLL;
	if (!m_edit->Create(dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		m_edit = NULL;
		return;
	}
#else
	m_edit = new CXedit;
	const DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | ES_LEFT |
		ES_NOHIDESEL | ES_UPPERCASE | ES_AUTOHSCROLL;
	if (!m_edit->Create(dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_edit;
		m_edit = NULL;
		return;
	}
#endif
	

	m_edit->set_ToolTipText("메뉴명을 검색 할 수 있습니다.");

	switch (m_id)
	{
	case CTRL_SCREEN:
		if (m_edit)	{
			m_edit->setlen(32);
			m_edit->SetLimitText(32);
			//m_edit->setlen(4);
			//m_edit->SetLimitText(4);
		}
		break;
	}

// 	CString	fontName;
// 	fontName.LoadString(STSM_FONTNAME_DEFAULT);
// 	m_font.CreatePointFont(100, fontName);
// 
// 	m_edit->SetFont(&m_font);

	m_edit->setaxiscall(callback);
}

void CXcom::set_Owner()
{
	m_edit->set_Owner();
}

void CXcom::changepos()
{
	CRect	cRc;
	
	if (m_edit)
	{
		m_edit->MoveWindow(CRect(0, 0, 0, 0));
		GetClientRect(cRc);
		cRc.right -= COMBTN_WIDTH;
		cRc.DeflateRect(3, 3);
		m_edit->MoveWindow(cRc);
	}

	GetClientRect(cRc);
	m_bRc.CopyRect(&cRc);

	m_bRc.left = cRc.right - COMBTN_WIDTH;
}

void CXcom::draw_Button(CDC* pDC)
{
	if (m_bRc.IsRectEmpty())	return;

	BITMAP	bm{};
	CDC	memDC;
	

	if (m_bmpXCombBack && m_bmpXCombBack->m_hObject)
	{
		CBitmap* oldBmp = NULL;
		m_bmpXCombBack->GetBitmap(&bm);
		
		if (!memDC.m_hDC)
			memDC.CreateCompatibleDC(pDC);
		oldBmp = memDC.SelectObject(m_bmpXCombBack);
		pDC->BitBlt(-1, -6, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(oldBmp);
	}

	if (m_arr && m_arr->m_hObject)
	{
		m_arr->GetBitmap(&bm);

		if( memDC.GetSafeHdc() )
			memDC.DeleteDC();
		
		if (memDC.CreateCompatibleDC(pDC))
		{
			CBitmap* oldbuffBitmap{};
			if (m_push)	
				oldbuffBitmap = memDC.SelectObject(m_arr_dn);
			else		
				oldbuffBitmap = memDC.SelectObject(m_arr);

			CRect rc(m_bRc);
			rc.DeflateRect(0, 2, 0, 2);
			rc.OffsetRect(-2, 0);
			if (oldbuffBitmap != (CBitmap *) 0)
			{
				pDC->StretchBlt(rc.left, rc.top, rc.Width(), rc.Height(), 
						&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
				memDC.SelectObject(oldbuffBitmap);
			}
			else	
				memDC.DeleteDC();
		}
	}
}

#define SRCMASK	0x00220326    // mask

BOOL CXcom::ShowBmp(CDC* pDC, int x, int y, UINT nResID, int maskcolor)
{
	CBitmap	bitmap;
	if (!bitmap.LoadBitmap(nResID))
		return FALSE;
	BITMAP  bm;   
	bitmap.GetBitmap(&bm);
	const CRect  r(0,0,bm.bmWidth,bm.bmHeight);

	CDC		 buffDC ;
	buffDC.CreateCompatibleDC( pDC ) ;
	CBitmap  buffBitmap ;
	buffBitmap.CreateCompatibleBitmap( pDC, bm.bmWidth, bm.bmHeight ) ;
	CBitmap* oldbuffBitmap = buffDC.SelectObject( &buffBitmap ) ;

	CDC		 maskDC ;
	maskDC.CreateCompatibleDC( pDC ) ;
	CBitmap  maskBitmap ;  
	maskBitmap.CreateBitmap( bm.bmWidth, bm.bmHeight, 1, 1, NULL ) ;
	CBitmap* oldmaskBitmap = maskDC.SelectObject( &maskBitmap ) ;

	buffDC.FillSolidRect( &r, maskcolor) ;

	CDC		 memoryDC ;
	memoryDC.CreateCompatibleDC( pDC ) ;
	CBitmap* oldmemoryBitmap = memoryDC.SelectObject(&bitmap/* CBitmap::FromHandle( hBitmap ) */) ;
	buffDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &memoryDC, 0, 0, SRCCOPY ) ;
	maskDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &buffDC, 0, 0, SRCCOPY ) ;

	CDC		 copyDC ;
	copyDC.CreateCompatibleDC( pDC ) ;
	CBitmap  copyBitmap ;
	copyBitmap.CreateCompatibleBitmap( pDC, bm.bmWidth, bm.bmHeight ) ;
	CBitmap* oldcopyBitmap = copyDC.SelectObject( &copyBitmap ) ;

	copyDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, pDC,  x, y, SRCCOPY) ;
	copyDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &maskDC,  0, 0, SRCAND) ;
	buffDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &maskDC,  0, 0, SRCMASK) ;
	copyDC.BitBlt( 0, 0, bm.bmWidth, bm.bmHeight, &buffDC,  0, 0, SRCPAINT) ;

	pDC->BitBlt(x,y, bm.bmWidth, bm.bmHeight, &copyDC, 0, 0, SRCCOPY  );

	copyDC.SelectObject( oldcopyBitmap ) ;
	memoryDC.SelectObject( oldmemoryBitmap ) ;
	maskDC.SelectObject( oldmaskBitmap ) ;
	buffDC.SelectObject( oldbuffBitmap ) ;
	return TRUE;
}

void CXcom::set_Home()
{
	if (!m_edit)	return;

	switch (m_id)
	{
	case CTRL_SCREEN:
		//m_edit->SetWindowText("");
		m_edit->SetSel(0, -1);
		m_edit->SetFocus();
		break;
	default:
		break;
	}
}

void CXcom::set_Maps(CString mapname)
{
	if (!m_edit)	return;
	if (mapname.IsEmpty())
	{
		m_edit->SetWindowText(_T(""));
		m_edit->SetFocus();
		return;
	}

	if (mapname.Right(2) == "00")
	{
		CString	str = mapname.Mid(2, 4);
		m_edit->SetWindowText(str);
	}
}

void CXcom::set_Code(CString code)
{
	m_edit->set_Code(code);
}

void CXcom::set_List(const CStringArray& array)
{
	if (m_edit)	m_edit->set_List(array);
}

void CXcom::add_History(CString dat)
{
	if (m_edit)	m_edit->add_History(dat);
}

LONG CXcom::OnSelect(WPARAM wParam, LPARAM lParam)
{
	if (m_edit)
	{
		m_edit->m_bSelect = true;
		m_edit->SetSel(0, -1);
		m_edit->SetFocus();
		m_edit->m_bSelect = false;
	}

	return 0;
}

bool CXcom::IsInputFocus()
{
#ifdef DF_USESTL_XCOM
	const CWnd* wnd = GetFocus();
	if (wnd == m_edit.get())
		return true;
	return false;
#else
	const CWnd* wnd = GetFocus();
	if (wnd == m_edit)
		return true;
	return false;
#endif
	
}


void CXcom::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	m_edit->SetSel(0, -1);
	m_edit->SetFocus();
}
