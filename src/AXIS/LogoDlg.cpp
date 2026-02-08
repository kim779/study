// LogoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "axMsg.hxx"
#include "LogoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define ITEM_HEIGHT 20
#define COLOR_ITEM			RGB(255, 255, 255)
#define COLOR_SELITEM		RGB(  0, 101, 178)
#define COLOR_ITEM_FONT     RGB(103, 103, 103)
#define COLOR_SELITEM_FONT	RGB(255, 255, 255)
#define COLOR_INLINE		RGB( 75, 147, 192)
#define ICO_GAP		2
/////////////////////////////////////////////////////////////////////////////
// CLogoDlg dialog
#define OUT_BG	 1
#define OUT_LINE 2
#define SEL_BG	3

CLogoDlg::CLogoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLogoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogoDlg, CDialog)
	//{{AFX_MSG_MAP(CLogoDlg)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ACTIVATE()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogoDlg message handlers

BOOL CLogoDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_activeItem = -1;
	m_parent     = pParentWnd;
	m_font.CreatePointFont(90,"굴림체");
	m_BG_top = Axis::GetSkinBitmap("LOGO_BG_TOP",true);
	m_ico1 = Axis::GetSkinBitmap("logo_ico1");
	m_ico2 = Axis::GetBitmap(IDB_LOGO_ICO2);
	m_outbg = ChangeSkinColor(OUT_BG);
	m_outline = ChangeSkinColor(OUT_LINE);
	m_selbg = ChangeSkinColor(SEL_BG);	
	return CDialog::Create(IDD, pParentWnd);
}

void CLogoDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	/*
	CRect rectdlg;
	HRGN hRgn;
	GetClientRect(&rectdlg);
	hRgn = CreateRoundRectRgn(rectdlg.left,rectdlg.top,rectdlg.right,rectdlg.bottom,5,5);
	SetWindowRgn(hRgn,TRUE);	
	*/
	MakeShape();
	// TODO: Add your message handler code here
	
}

void CLogoDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	CRect rc;
	GetClientRect(rc);
	rc.right --;
	rc.bottom --;
	CPen pn; 
	//pn.CreatePen(PS_SOLID, 1, m_outline);
	
	
	//dc.SelectObject(&pn);
	dc.RoundRect(rc,CPoint(6,6));
	if (m_BG_top)
	{
		CBrush br_inbg, databg, itembg;

		br_inbg.CreateSolidBrush(m_outbg);//Axis::GetColor(192));
		databg.CreateSolidBrush(RGB(255,255,255));
		itembg.CreateSolidBrush(m_outbg);		
		pn.CreatePen(PS_SOLID, 1, m_outbg);

		dc.SelectObject(&pn);
		dc.SelectObject(br_inbg);
		dc.RoundRect(CRect(2, 10, rc.right-2, rc.bottom-2), CPoint(7, 7));

		CDC tdc;
		
		tdc.CreateCompatibleDC(&dc);
		tdc.SelectObject(m_BG_top);
		dc.BitBlt(0,0,207,71, &tdc, 0,0, SRCCOPY);
		
		dc.SelectObject(databg);
		dc.RoundRect(CRect(4, 75, rc.right-4, rc.bottom-2-3), CPoint(7, 7));
		CPen inpn;
		inpn.CreatePen(PS_SOLID, 1, Axis::GetColor(192));
		dc.SelectObject(&inpn);
		dc.RoundRect(CRect(6, 77, rc.right-6, rc.bottom-4-3), CPoint(7, 7));
		//데이터 영역 그리기
		CBrush item, sitem;
		
		item.CreateSolidBrush(COLOR_ITEM);

		sitem.CreateSolidBrush(m_selbg);
		CRect rt;
		CString data, fdata;
		dc.SetBkMode(TRANSPARENT);
		dc.SelectObject(m_font);
		CStringArray		ary;
		for(int dd=0; dd<m_ListCount ; dd++)
		{
			rt = DataRect(dd);
			data = slist.GetAt(dd);
			parseAry(data, ary, "|");
			fdata.Format("[%s]%s", ary.GetAt(0), ary.GetAt(1));
			if (dd+1 == m_activeItem)
			{
				dc.FillRect(rt,&sitem);
				dc.SetTextColor(COLOR_SELITEM_FONT);
				dc.TextOut(rt.left+22, rt.top+3, fdata);
				CDC idc;
				idc.CreateCompatibleDC(&dc);
				idc.SelectObject(m_ico1);
				dc.BitBlt(rt.left+ICO_GAP,rt.top+ICO_GAP, 18, 15, &idc, 0, 0, SRCCOPY);
				idc.DeleteDC();
			}
			else
			{
				dc.FillRect(rt,&item);
				dc.SetTextColor(COLOR_ITEM_FONT);
				dc.TextOut(rt.left+22, rt.top+3, fdata);
				CDC idc;
				idc.CreateCompatibleDC(&dc);
				idc.SelectObject(m_ico2);
				dc.BitBlt(rt.left+ICO_GAP,rt.top+ICO_GAP, 18, 15, &idc, 0, 0, SRCCOPY);
				idc.DeleteDC();
			}
			
			
		}
	}
	/*
	CString tmp; tmp.Format("Active: %d", m_activeItem);
	dc.TextOut(10, 68, tmp);
	*/
	
	


	//m_ListCount*ITEM_HEIGHT
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CLogoDlg::OnMainWndMoved( )
{
	CRect rc;
	AfxGetMainWnd()->GetWindowRect(&rc);
	/*
	int cx = rc.left - m_rcMain.left;
	int cy = rc.top - m_rcMain.top;

	CRect r;
	GetWindowRect(&r);
	*/

	rc.OffsetRect(7, 3);

	SetWindowPos(NULL, rc.left, rc.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

	
}

void CLogoDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ShowWindow(SW_HIDE);
	if (m_activeItem != -1)
	{
		CString data;
		data = slist.GetAt(m_activeItem-1);
		CStringArray		ary;
		parseAry(data, ary, "|");
		data = ary.GetAt(0);
		
		
		::SendMessage(m_parent->m_hWnd, WM_USER, MMSG_SCREENOPEN, (LPARAM)data.operator LPCTSTR());


	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CLogoDlg::MakeShape()
{
	/*
	return;
	CRect crc;
	GetClientRect(&crc);
	BITMAP bm;
	m_BG_top->GetBitmap(&bm);
	CBitmap* back = new CBitmap();
	back->CreateBitmap(198,600, bm.bmPlanes, bm.bmBitsPixel, bm.bmBits );
	SetWindowPos(NULL, 0, 0, crc.Width(), crc.Height(), SWP_NOMOVE | SWP_NOZORDER);
	CDC dc = GetDC();
	dc.MaskBlt(
	*/
	
	CRect crc;
	GetClientRect(&crc);
	if (m_BG_top && m_BG_top->m_hObject)
	{
		HRGN hWndRgn = DIBToRgn((HBITMAP)m_BG_top->m_hObject, Axis::maskColor, FALSE);
		
		CRgn roundrgn; roundrgn.CreateRoundRectRgn(0,45,199,crc.bottom,5,5);
		
		CombineRgn((HRGN)roundrgn.m_hObject, (HRGN)roundrgn.m_hObject, hWndRgn,  RGN_OR);
		
		if(hWndRgn)
		{
			SetWindowRgn((HRGN)roundrgn.m_hObject,TRUE);
			::DeleteObject((HRGN)roundrgn.m_hObject);
		}
	}
	
}


void CLogoDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	
	switch( nState )
	{
	case WA_INACTIVE:
		//OnNcActivate(FALSE);
		//AfxMessageBox("WA_INACTIVE");
		//ShowWindow(SW_HIDE);
		break;
	case WA_ACTIVE:
	case WA_CLICKACTIVE:
		
		//OnNcActivate(TRUE);
		break;
	}
}


LRESULT CLogoDlg::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	//m_bHover = TRUE; 
	//Invalidate();
	//AfxMessageBox("OnMouseHover");
	return 0;
}


LRESULT CLogoDlg::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	//m_bTracking = FALSE;
	//m_bHover=FALSE;

	//Invalidate(); 
	//AfxMessageBox("OnMouseLeave");
	ShowWindow(SW_HIDE);
	return 0;
}

void CLogoDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(tme);
    tme.dwFlags = TME_LEAVE;
    tme.hwndTrack = m_hWnd;
    _TrackMouseEvent(&tme);
	if (CalcDataRect(point)!=-1)
	{
		if (changeIndex)	draw_DataArea();
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CLogoDlg::Initialize(BOOL bSmall)
{
	CRect rc;
	AfxGetMainWnd()->GetWindowRect(&rc);

	if( bSmall )
		rc.OffsetRect(9, 3);
	else
		rc.OffsetRect(7, 3);
	
	
	m_outbg = ChangeSkinColor(OUT_BG);
	m_outline = ChangeSkinColor(OUT_LINE);
	m_selbg   = ChangeSkinColor(SEL_BG);

	//SetWindowPos(NULL, rc.left, rc.top, 199, DataCnt*ITEM_HEIGHT+200, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);


	m_ListCount = LoadMenu();

	/*
	{
		AfxMessageBox("추천메뉴를 로딩하지 못하였습니다.");
		return;
	}
	*/
	MoveWindow(rc.left, rc.top, 199, m_ListCount*ITEM_HEIGHT+90);
	MakeShape();
}

int CLogoDlg::LoadMenu()
{
	int res = 0;
	CString path, tmp, tmp_val;
	path.Format("%s\\%s\\%s", Axis::home, TABDIR, "FAV_MENU.INI");
	//AfxMessageBox(path);
	if(!IsFileExist(path)) return 0;

	CProfile profile(path);
	CStringArray		ary;
	slist.RemoveAll();
	for (int ii = 0 ; ii < 99 ; ii++ )
	{
		tmp_val = "";
		tmp.Format("%02d", ii);
		
		tmp_val = profile.GetString("FAV_MENU", tmp);
		if (!tmp_val.IsEmpty())
		{
			ary.RemoveAll();
			slist.Add(tmp_val);
			res++;
			/*
			parseAry(tmp_val, ary, "|");
			AfxMessageBox(ary.GetAt(0));
			*/

		}
	}
	

	return res;
}

void CLogoDlg::parseAry(CString data, CStringArray &ary, CString separate)
{
	CString	value;
	ary.RemoveAll();
	while (!data.IsEmpty())
	{
		int pos = data.Find('|');
		if (pos == -1)
		{
			data.TrimLeft();
			data.TrimRight();
			ary.Add(data);
			break;
		}
		value = data.Left(pos++);
		data = data.Mid(pos);
		value.TrimLeft();
		value.TrimRight();
		ary.Add(value);
	}
}

int CLogoDlg::CalcDataRect(CPoint pt)
{
	CRect rt;
	//m_activeItem = -1;
	for(int dd=0; dd<m_ListCount ; dd++)
	{
		rt = DataRect(dd);
		if(rt.PtInRect(pt))
		{
			m_before_activeItem = m_activeItem;
			changeIndex = (m_activeItem != (dd+1));
			m_activeItem = dd+1;
			return m_activeItem;
			break;
		}
	}
	m_activeItem = -1;
	return m_activeItem;
}

CRect CLogoDlg::DataRect(int idx)
{
	
	CRect rt;
	rt.SetRectEmpty();
	if (idx>m_ListCount) return rt;
	rt.top    = 80+ITEM_HEIGHT*idx;
	rt.bottom = rt.top + ITEM_HEIGHT;
	rt.left   = 8;
	rt.right  = 190;

	return rt;
}

BOOL CLogoDlg::OnEraseBkgnd(CDC* pDC) 
{
	/*
	CRect clientRC; GetClientRect(&clientRC);
	COLORREF clrBack = GetSysColor(COLOR_BTNFACE);
	
	pDC->FillSolidRect(clientRC, clrBack);
	*/
	//OnPaint();
	return true;
	
	return CDialog::OnEraseBkgnd(pDC);
}

void CLogoDlg::draw_DataArea()
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	CRect rc;
	GetClientRect(rc);
	rc.right --;
	rc.bottom --;
	CPen pn; pn.CreatePen(PS_SOLID, 1, RGB(74, 147, 190));
	
	
	dc.SelectObject(&pn);
	dc.RoundRect(rc,CPoint(6,6));
	//데이터 영역 그리기
	CBrush item, sitem;
		
	item.CreateSolidBrush(COLOR_ITEM);
	sitem.CreateSolidBrush(COLOR_SELITEM);
	CRect rt;
	CString data, fdata;
	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(m_font);
	CStringArray		ary;
	for(int dd=0; dd<m_ListCount ; dd++)
	{
		//if ((dd == m_activeItem) || (dd == m_before_activeItem))
		{
			rt = DataRect(dd);
			data = slist.GetAt(dd);
			parseAry(data, ary, "|");
			fdata.Format("[%s]%s", ary.GetAt(0), ary.GetAt(1));
			if (dd+1 == m_activeItem)
			{
				dc.FillRect(rt,&sitem);
				dc.SetTextColor(COLOR_SELITEM_FONT);
				dc.TextOut(rt.left+22, rt.top+3, fdata);
				CDC idc;
				idc.CreateCompatibleDC(&dc);
				idc.SelectObject(m_ico1);
				dc.BitBlt(rt.left+ICO_GAP,rt.top+ICO_GAP, 18, 15, &idc, 0, 0, SRCCOPY);
				idc.DeleteDC();
			}
			else
			{
				dc.FillRect(rt,&item);
				dc.SetTextColor(COLOR_ITEM_FONT);
				dc.TextOut(rt.left+22, rt.top+3, fdata);
				CDC idc;
				idc.CreateCompatibleDC(&dc);
				idc.SelectObject(m_ico2);
				dc.BitBlt(rt.left+ICO_GAP,rt.top+ICO_GAP, 18, 15, &idc, 0, 0, SRCCOPY);
				idc.DeleteDC();
			}
		}
		
	}
	CRect rt1,rt2;
	
	rt1 = DataRect(m_before_activeItem-1); 
	rt2 = DataRect(m_activeItem-1);
	
	InvalidateRect(rt1, false);
	InvalidateRect(rt2, false);

}


COLORREF CLogoDlg::ChangeSkinColor(int tp)
{
	CString pal = Axis::skinName;
		
	pal.MakeUpper();
	if (pal == "BLUE")
	{
		switch(tp)
		{
		case OUT_BG :
			return RGB(168,218,244);
			break;
		case OUT_LINE :
			return RGB(75,147,191);
			break;
		case SEL_BG :
			return RGB(0,101,178);
			break;
		}
	}
	else if (pal == "GREEN")
	{
		switch(tp)
		{
		case OUT_BG :
			return RGB(173,226,184);
			break;
		case OUT_LINE :
			return RGB(96,180,105);
			break;
		case SEL_BG :
			return RGB(67,105,90);
			break;
		}
	}
	else if (pal == "BROWN")
	{
		switch(tp)
		{
		case OUT_BG :
			return RGB(236,225,161);
			break;
		case OUT_LINE :
			return RGB(145,128,84);
			break;
		case SEL_BG :
			return RGB(119,69,20);
			break;
		}
	}
	else if (pal == "VIOLET")
	{
		switch(tp)
		{
		case OUT_BG :
			return RGB(227,216,248);
			break;
		case OUT_LINE :
			return RGB(179,132,234);
			break;
		case SEL_BG :
			return RGB(52,4,107);
			break;
		}
	}
	else if (pal == "GRAY")
	{
		switch(tp)
		{
		case OUT_BG :
			return RGB(199,199,199);
			break;
		case OUT_LINE :
			return RGB(137,137,137);
			break;
		case SEL_BG :
			return RGB(58,74,107);
			break;
		}
	}
	else
	{
		switch(tp)
		{
		case OUT_BG :
			return RGB(168,218,244);
			break;
		case OUT_LINE :
			return RGB(75,147,191);
			break;
		case SEL_BG :
			return RGB(0,101,178);
			break;
		}
	}
	return RGB(125,125,125);
}
