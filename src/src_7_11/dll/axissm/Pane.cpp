// Pane.cpp : implementation file
//

#include "stdafx.h"
#include "Pane.h"
#include "smdefine.h"
#include "resource.h"
#include "PaneList.h"

#include "../../axis/axMsg.hxx"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TM_INDEX	7001
#define TM_NEWS		7002
#define TM_UPDOWN	7003
#define TM_INTER	7004

#define TMI_INDEX	3000
#define TMI_NEWS	5000
#define TMI_UPDOWN	10000
#define TMI_INTER	3000

#define PANETITLEWIDTH		"   머니투데이"

#define MAX_NEWS	10
/////////////////////////////////////////////////////////////////////////////
// CPane

CPane::CPane(CString home, CString userN, bool (*axiscall)(int, WPARAM, LPARAM), CBitmap* bkImage)
{
	m_home		= home;
	m_userN		= userN;
	m_axiscall	= axiscall;
	m_bkIMG		= bkImage;

	m_pen.CreatePen(PS_SOLID, 0, GetColor(COLOR_TICK_LINE));
	m_bitmap.LoadBitmap(IDB_TICKLIST);
	
	BITMAP	bm;
	m_bitmap.GetBitmap(&bm);
	m_wBitmap = bm.bmWidth / 2;

	m_down = false;
	m_bReset = false;
	m_kind = TKIND_INDEX;
	m_select = 0;

	m_index = -1;
	m_aux = -1;
	m_interval = TMI_INDEX;

	CString	fontName;
	fontName.LoadString(STSM_FONTNAME_DEFAULT);
	m_font.CreatePointFont(90, "굴림체");

	m_bkColor  = GetColor(COLOR_TICK_INSIDE);
	m_color[0] = GetColor(COLOR_TICK_SH);
	m_color[1] = GetColor(COLOR_TICK_SS);
	m_color[2] = GetColor(COLOR_TICK_BH);
	m_color[3] = GetColor(COLOR_TICK_HL);
	m_color[4] = GetColor(COLOR_TICK_HH);

	m_panelist = NULL;
	MakeExpectSymbolTable();
}

CPane::~CPane()
{
	m_font.DeleteObject();
	m_pen.DeleteObject();
	m_bitmap.DeleteObject();

	m_arItem.RemoveAll();
	m_arNewsKey.RemoveAll();
	m_mapData.RemoveAll();
	m_arSymData.RemoveAll();
	m_arIndexbySym.RemoveAll();
	m_arSymbyIndex.RemoveAll();
	
	for (int ii = 0; ii < m_arInter.GetSize(); ii++)
		delete m_arInter.GetAt(ii);

	m_arInter.RemoveAll();
	m_arInterByCode.RemoveAll();

	if (m_panelist)		delete m_panelist;
}


BEGIN_MESSAGE_MAP(CPane, CWnd)
	//{{AFX_MSG_MAP(CPane)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PANELIST, OnPanelist)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPane message handlers
void CPane::MakeExpectSymbolTable()
{
//	char*	sym[] = {"XNI01", "X0001", "X2001", "XQSTR", "XQ001", "XQ501", "XBD40"};
	char*	sym[] = {"UNI01", "K0001", "K2001", "KQSTR", "KQ001", "KQ501", "KBD40"};
	char*	name[] = {"KRX-예상", "KP-예상", "K200-예상", "스타-예상", "KQ-예상", "KQ50-예상", "배당-예상"};

	int	ncnt = sizeof(sym)/sizeof(char*);
	
	for (int ii = 0 ; ii < ncnt ; ii++ )
	{
		m_mapExpectSymbol.SetAt(sym[ii], name[ii]);
	}
}

CString CPane::GetExpectSymbolName(CString sym)
{
	CString	ret = _T("");
	if (sym.IsEmpty())
		return _T("");

	m_mapExpectSymbol.Lookup(sym, ret);
	return ret;
}

void CPane::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	cRc;
	GetClientRect(cRc);
	
	CDC	memDC;
	if (!memDC.CreateCompatibleDC(&dc))
	{
		drawBackground(&dc);
		DrawData(&dc);
	}
	else
	{
		CBitmap	bmp, *pbmp;
		bmp.CreateCompatibleBitmap(&dc, cRc.Width(), cRc.Height());
		pbmp = (CBitmap *) memDC.SelectObject(&bmp);

		drawBackground(&memDC);
		DrawData(&memDC);

		dc.BitBlt(0, 0, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pbmp);
		memDC.DeleteDC();
	}
}

void CPane::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CString		text, tmps, data;
	
	if (m_arRect.PtInRect(point))
	{
		SetCapture();
		CClientDC	dc(this);
		DrawAr(&dc, true);
	}
	else
	{
		PopUpScreen();
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CPane::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();

	if (!m_arRect.PtInRect(point))
	{
		InvalidateRect(m_arRect);

		CWnd::OnLButtonUp(nFlags, point);
		return;
	}

	switch (m_kind)
	{
	case TKIND_INTER:
		if (!m_panelist)
		{
			int count = (int)m_arInter.GetSize();
			if (count > MAX_PANELIST)	count = MAX_PANELIST;
			CRect	pRc, wRc;
			GetWindowRect(pRc);
			wRc.SetRect(pRc.left, pRc.top - ((pRc.Height() + 2) * count), pRc.right, pRc.top);
			if (wRc.top < 0)	wRc.OffsetRect(0, pRc.bottom + abs(wRc.top) + 2);
			
			m_panelist = new CPaneList(this, m_id, m_kind, count, m_bkColor);
			m_panelist->CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, AfxRegisterWndClass(0), NULL, 
				WS_VISIBLE|WS_POPUP, wRc, NULL, NULL, NULL);
			m_panelist->SetInters(&m_arInter);
		}
		else
		{
			m_panelist->DestroyWindow();
			delete m_panelist;
			m_panelist = NULL;
		}
		break;		break;
	case TKIND_INDEX:
		if (!m_panelist)
		{
			int count = (int)m_arItem.GetSize();
			if (count > MAX_PANELIST)	count = MAX_PANELIST;
			CRect	pRc, wRc;
			GetWindowRect(pRc);
			wRc.SetRect(pRc.left, pRc.top - ((pRc.Height() + 2) * count), pRc.right, pRc.top);
			if (wRc.top < 0)	wRc.OffsetRect(0, pRc.bottom + abs(wRc.top) + 2);
			
			m_panelist = new CPaneList(this, m_id, m_kind, count, m_bkColor);
			m_panelist->CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, AfxRegisterWndClass(0), NULL, 
				WS_VISIBLE|WS_POPUP, wRc, NULL, NULL, NULL);
		}
		else
		{
			m_panelist->DestroyWindow();
			delete m_panelist;
			m_panelist = NULL;
		}
		break;
	case TKIND_UPDOWN:
		if (!m_panelist)
		{
			int count = (int)m_arItem.GetSize();
			if (count > MAX_PANELIST)	count = MAX_PANELIST;
			CRect	pRc, wRc;
			GetWindowRect(pRc);
			wRc.SetRect(pRc.left, pRc.top - ((pRc.Height() + 2) * count), pRc.right, pRc.top);
			if (wRc.top < 0)	wRc.OffsetRect(0, pRc.bottom + abs(wRc.top) + 2);
			
			m_panelist = new CPaneList(this, m_id, m_kind, count, m_bkColor);
			m_panelist->CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, AfxRegisterWndClass(0), NULL, 
				WS_VISIBLE|WS_POPUP, wRc, NULL, NULL, NULL);
		}
		else
		{
			m_panelist->DestroyWindow();
			delete m_panelist;
			m_panelist = NULL;
		}
		break;
	case TKIND_NEWS:
		if (!m_panelist)
		{
			int count = (int)m_arHistoryNews.GetSize();
			if (count > MAX_PANELIST)	count = MAX_PANELIST;
			CRect	pRc, wRc;
			GetWindowRect(pRc);
			wRc.SetRect(pRc.left, pRc.top - ((pRc.Height() + 2) * count), pRc.right, pRc.top);
			if (wRc.top < 0)	wRc.OffsetRect(0, pRc.bottom + abs(wRc.top) + 2);
			
			m_panelist = new CPaneList(this, m_id, m_kind, count, m_bkColor);
			m_panelist->CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, AfxRegisterWndClass(0), NULL, 
				WS_VISIBLE|WS_POPUP, wRc, NULL, NULL, NULL);
			m_panelist->SetNews(&m_arHistoryNews);
		}
		else
		{
			m_panelist->DestroyWindow();
			delete m_panelist;
			m_panelist = NULL;
		}
		break;
	case TKIND_CONCLUSION:
		(*m_axiscall)(AXI_SHOWDEAL, 0, 0);
		break;
	}

	InvalidateRect(m_arRect);

	CWnd::OnLButtonUp(nFlags, point);
}

void CPane::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case TM_INDEX:
	case TM_UPDOWN:
		m_index++;
		if (m_index >= m_arItem.GetSize())
			m_index = 0;
		if (m_index >= 0 && m_index < m_arSym.GetSize())
		{
			CString	sym, dat, keys;

			keys.Format("%d", m_arItem.GetAt(m_index));
			m_arSymbyIndex.Lookup(keys, sym);
			m_arSymData.Lookup(sym, dat);
			SetData(dat);
		}
		else	SetData("");
		Invalidate();
		break;
	case TM_INTER:
		m_index++;
		if (m_index >= m_arInter.GetSize())
			m_index = 0;

		Invalidate();
		break;
	default:
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

void CPane::drawBackground(CDC* pDC)
{
	CDC	 memDC;
	CRect	 cRc;

	GetClientRect(cRc);

	pDC->SetBkColor(m_bkColor);
	pDC->ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, cRc, "", 0, NULL);

	CPen	*pOldPen, pen(PS_SOLID, 1, GetColor(COLOR_TICK_LINE));
	CBrush	*pOldBrush, brush(m_bkColor);
	pOldPen = pDC->SelectObject(&pen);
	pOldBrush = pDC->SelectObject(&brush);

	pDC->Rectangle(cRc);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

void CPane::DrawData(CDC* pDC/*= NULL*/)
{
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;

	DrawHead(pDC);

	switch (m_kind)
	{
	case TKIND_INDEX:	Draw_Index(pDC);	break;
	case TKIND_NEWS:	Draw_News(pDC);		break;
	case TKIND_UPDOWN:	Draw_Updn(pDC);		break;
	case TKIND_INTER:	Draw_Inters(pDC);	break;
	case TKIND_CONCLUSION:	Draw_Conclusion(pDC);	break;
	default:return;
	}
}

void CPane::DrawAr(CDC* pDC, bool bDown)
{
	CRect	cRc;
	BITMAP	bm;

	GetClientRect(cRc);
	m_bitmap.GetBitmap(&bm);

	cRc.left = cRc.right - bm.bmWidth/3 - PGAP;
	cRc.DeflateRect(1, 1);
	pDC->FillSolidRect(cRc, m_bkColor);

	GetClientRect(cRc);
	cRc.left = cRc.right - bm.bmWidth/3 - PGAP;
	cRc.top    = (cRc.Height() - bm.bmHeight) / 2;
	cRc.bottom = cRc.top + bm.bmHeight;
	m_arRect.CopyRect(&cRc);

	CDC	memDC;
	if (memDC.CreateCompatibleDC(pDC))
	{
		CBitmap* bitmap = memDC.SelectObject(&m_bitmap);
		pDC->BitBlt(cRc.left, cRc.top, bm.bmWidth/3, cRc.Height(), &memDC, bDown ? bm.bmWidth/3 : 0, 0, SRCCOPY);
		memDC.SelectObject(bitmap);
		memDC.DeleteDC();
	}
}

void CPane::Draw_Index(CDC* pDC)
{
	if (m_index < 0 ||m_index >= m_arItem.GetSize())//m_mapData.GetCount() <= 0)
		return;
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;

	int bkMode = pDC->SetBkMode(TRANSPARENT);
	int	key = m_arItem.GetAt(m_index);
	switch (key)
	{
	default:
	case 1:case 2:case 3:case 4:case 5:case 6:case 7:	// kindex
	case 8:case 9:case 10:					// future index
	case 11:case 12:case 13:				// foreign
	case 17:case 18:
		Draw_KIndex(pDC, key);
		break;
	}

	pDC->SetBkMode(bkMode);
}

void CPane::Draw_KIndex(CDC* pDC, int key)
{
	CRect	cRc, tRc;
	CString	text, keys, value, sym, tmps;
	CFont*	font = pDC->SelectObject(&m_font);
	int	cx = pDC->GetTextExtent(PANETITLEWIDTH).cx;
	COLORREF color;

	GetClientRect(cRc);
	cRc.SetRect(cRc.left + cx + PGAP, cRc.top, cRc.right - PGAP - m_wBitmap, cRc.bottom);
	tRc.SetRect(cRc.left, cRc.top, cRc.left, cRc.bottom);

	if (m_mapData.Lookup(S_CUR, value))	// 현재가
	{
		switch (GetSign(value))
		{
		case '+':
			color = pDC->SetTextColor(GetColor(COLOR_TICK_UP));
			sym.LoadString(ST_SYMBOL_UP2);
			break;
		case '-':
			color = pDC->SetTextColor(GetColor(COLOR_TICK_DOWN));
			sym.LoadString(ST_SYMBOL_DOWN2);
			break;
		default:
			color = pDC->SetTextColor(GetColor(COLOR_TICK_TEXT));
			break;
		}
		
		text.Format("%s", commaE(value));
		cx = pDC->GetTextExtent("99,999.99").cx;
		tRc.left = tRc.right;
		tRc.right = tRc.left + cx;

		pDC->DrawText(text, tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
	}
	else
	{
		DrawAr(pDC);

		pDC->SelectObject(font);
		return;
	}
	
	switch (key)
	{
	case 8:
		if (m_mapData.Lookup(S_KOSPI, value))	// K200
		{
			tRc.left = tRc.right;
			tRc.right = tRc.left + 20;
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->DrawText("/", tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
			
			switch (GetSign(value))
			{
			case '+':
				color = pDC->SetTextColor(GetColor(COLOR_TICK_UP));
				sym.LoadString(ST_SYMBOL_UP2);
				break;
			case '-':
				color = pDC->SetTextColor(GetColor(COLOR_TICK_DOWN));
				sym.LoadString(ST_SYMBOL_DOWN2);
				break;
			default:
				color = pDC->SetTextColor(GetColor(COLOR_TICK_TEXT));
				break;
			}
			
			text.Format("%s", commaE(value));
			cx = pDC->GetTextExtent("9,999.99").cx;
			tRc.left = tRc.right;
			tRc.right = tRc.left + cx;
			
			pDC->DrawText(text, tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
		}
		break;
	default:
		if (m_mapData.Lookup(S_DB, value))	// 대비
		{
			text.Format("%s", getDBSign(value));
			cx = pDC->GetTextExtent("  ▲").cx;
			tRc.left = tRc.right;
			tRc.right = tRc.left + cx;
			
			pDC->DrawText(text, tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
			
			cx = pDC->GetTextExtent(" 999.99").cx;
			tRc.left = tRc.right;
			tRc.right = tRc.left + cx;
			
			pDC->DrawText(commaE(value), tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
		}
		break;
	}

	switch (key)
	{
	case 8:case 9:case 10:
		if (m_mapData.Lookup(BASIS, value))	// BASIS
		{
			switch (GetSign(value))
			{
			case '+':	pDC->SetTextColor(GetColor(COLOR_TICK_UP));	break;
			case '-':	pDC->SetTextColor(GetColor(COLOR_TICK_DOWN));	break;
			default:	pDC->SetTextColor(GetColor(COLOR_TICK_TEXT));	break;
			}

			text.Format("%sB", commaE(value, false));
			cx = pDC->GetTextExtent(" +99.99%").cx;
			tRc.left = tRc.right;
			tRc.right = tRc.left + cx;
			
			pDC->DrawText(text, tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
		}
		break;
	default:
		if (m_mapData.Lookup(S_RATE, value))	// 등락률
		{
			text.Format("%s%%", value);
			cx = pDC->GetTextExtent(" +99.99%").cx;
			tRc.left = tRc.right;
			tRc.right = tRc.left + cx;

			pDC->DrawText(text, tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
		}
		break;
	}
	
	switch (key)
	{
	case 8:
		if (m_mapData.Lookup(S_THEORY, value))		// 이론가
		{
			tRc.left = tRc.right;
			tRc.right = tRc.left + 20;
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->DrawText("/", tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

			switch (GetSign(value))
			{
			case '+':
				color = pDC->SetTextColor(GetColor(COLOR_TICK_UP));
				sym.LoadString(ST_SYMBOL_UP2);
				break;
			case '-':
				color = pDC->SetTextColor(GetColor(COLOR_TICK_DOWN));
				sym.LoadString(ST_SYMBOL_DOWN2);
				break;
			default:
				color = pDC->SetTextColor(GetColor(COLOR_TICK_TEXT));
				break;
			}
			
			text.Format("%s", commaE(value));
			cx = pDC->GetTextExtent("9,999.99").cx;
			tRc.left = tRc.right;
			tRc.right = tRc.left + cx;
			
			pDC->DrawText(text, tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
		}
		break;
	case 11:	case 12:				// 해외지수 시간
	case 17:	case 18:
		if (m_mapData.Lookup(S_FTIME, value))
		{
			int pos = value.Find(' ');
			if (pos < 0)	break;
			value = value.Mid(++pos);
			if (value.GetLength() > 5)	value = value.Left(5);
			
			text = _T(" ") + value + _T(" (한국시간기준)");
			cx = pDC->GetTextExtent(text).cx;
			tRc.left = tRc.right;
			tRc.right = tRc.left + cx;
			pDC->SetTextColor(GetColor(COLOR_TICK_TEXT));
			pDC->DrawText(text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		}
		break;
	default:
		tRc.right += 4;
		if (m_aux)
		{
			if (m_mapData.Lookup(S_NVOL, value))	// 거래량
			{
				text.Format("%s", commaE(value));
				cx = pDC->GetTextExtent(text).cx;
				tRc.left = tRc.right;
				tRc.right = tRc.left + cx;
				
				pDC->SetTextColor(GetColor(COLOR_TICK_TEXT));
				pDC->DrawText(text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			}
		}
		else
		{
			for (int ii = 0; ii < m_arSign[key].GetSize(); ii++)
			{
				switch (atoi(m_arSign[key].GetAt(ii)))
				{
				case 3:
					text = S_NONE;
					pDC->SetTextColor(GetColor(COLOR_TICK_TEXT));
					break;
				case 1:
					text = S_SKofDay;
					pDC->SetTextColor(GetColor(COLOR_TICK_UP));
					break;
				case 5:
					text = S_SJofDay;
					pDC->SetTextColor(GetColor(COLOR_TICK_DOWN));
					break;
				case 2:
					pDC->SetTextColor(GetColor(COLOR_TICK_UP));
					text = S_SSofPrev;
					break;
				case 4:
					text = S_HLofPrev;
					pDC->SetTextColor(GetColor(COLOR_TICK_DOWN));
					break;
				default:
					continue;
				}
				
				cx = pDC->GetTextExtent(text).cx;
				tRc.left = tRc.right + 1;
				tRc.right = tRc.left + cx;
				
				pDC->DrawText(text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			}
		}
		break;
	}

	DrawAr(pDC);

	pDC->SetTextColor(color);
	pDC->SelectObject(font);
}

void CPane::Draw_News(CDC* pDC)
{
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;

	CString	text;
	CRect	cRc, tRc;

	GetClientRect(cRc);
	CFont* font = pDC->SelectObject(&m_font);
	int cx = pDC->GetTextExtent(PANETITLEWIDTH).cx;

	tRc.SetRect(cRc.left + cx + PGAP, cRc.top, cRc.right, cRc.bottom);
	if (m_mapData.Lookup(SNEW_TITLE, text))
	{
		tRc.right -= m_wBitmap;
		pDC->DrawText(" " + text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
	}

	DrawAr(pDC);
	pDC->SelectObject(font);
}

void CPane::Draw_Updn(CDC* pDC)
{
	if (m_index < 0)
		return;

	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;
	pDC->SetBkMode(TRANSPARENT);

	int		cx;
	int		ii = 0;
	CRect		cRc, tRc, dRc[RMAX];
	CString		text;
	COLORREF	color;

	GetClientRect(cRc);
	CPen*  pen  = pDC->SelectObject(&m_pen);
	CFont* font = pDC->SelectObject(&m_font);
	cx = pDC->GetTextExtent(PANETITLEWIDTH).cx;
	cRc.left = cRc.left + cx + PGAP;
	cRc.right = cRc.right - PGAP - m_wBitmap;
	
	switch (m_aux)
	{
	case AUX_TEXT:
		{
			double width = cRc.Width() / 5.0;
			for (ii = 0; ii < 5; ii++)
			{
				if (ii == 4)
					dRc[ii].SetRect(cRc.left + (int)(width*ii), cRc.top + PGAP, cRc.right, cRc.bottom - PGAP);
				else
					dRc[ii].SetRect(cRc.left + (int)(width*ii), cRc.top + PGAP,
							cRc.left + (int)(width*(ii + 1)) - 1, cRc.bottom - PGAP);
			}

			color = pDC->SetTextColor(COLOR_PANETEXT);
			UINT nFlag = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
			for (ii = 0; ii < 5; ii++)
			{
				pDC->FillSolidRect(dRc[ii], m_color[ii]);

				switch (ii)
				{
				case 0:	m_mapData.Lookup(S_SH, text);
					pDC->SetTextColor(RGB(255, 255, 255));	break;
				case 1:	m_mapData.Lookup(S_SS, text);
					pDC->SetTextColor(RGB(255, 255, 255));	break;
				case 2:	m_mapData.Lookup(S_BH, text);
					pDC->SetTextColor(GetColor(COLOR_TICK_TEXT));	break;
				case 3:	m_mapData.Lookup(S_HL, text);
					pDC->SetTextColor(RGB(255, 255, 255));	break;
				case 4:	m_mapData.Lookup(S_HH, text);
					pDC->SetTextColor(RGB(255, 255, 255));	break;
				}
				pDC->DrawText(text, dRc[ii], nFlag);
			}
			pDC->SetTextColor(color);
		}
		break;
	case AUX_GRAPH:
		{
			if (m_mapData.GetCount() <= 0)
				break;
			
			int	total = 0, value, w;
			CArray < int, int > ary, aryW;
			color = pDC->SetTextColor(COLOR_PANETEXT);

			if (!m_mapData.Lookup(S_SH, text))
			{
				pDC->SetTextColor(color);
				break;
			}

			value = atoi(text);	ary.Add(value);	total += value;
			m_mapData.Lookup(S_SS, text);	value = atoi(text);	ary.Add(value);	total += value;
			m_mapData.Lookup(S_BH, text);	value = atoi(text);	ary.Add(value);	total += value;
			m_mapData.Lookup(S_HL, text);	value = atoi(text);	ary.Add(value);	total += value;
			m_mapData.Lookup(S_HH, text);	value = atoi(text);	ary.Add(value);	total += value;

			int width = cRc.Width() - 4;	// 4 : 1 pixel between each column
			for (ii = (int)ary.GetSize() - 1; ii >= 0; ii--)
			{
				value = ary.GetAt(ii);
				if (value)
				{
					w = (value * width) / total;
					if (!w)	w = 1;	// fit
				}
				else	w = 0;
				aryW.InsertAt(0, w);
			}

			total = 0;
			for (ii = 0; ii < ary.GetSize(); ii++)
			{
				value = aryW.GetAt(ii);

				if (ii == ary.GetSize() - 1)
				{
					if (value)
						dRc[ii].SetRect(cRc.left + total, cRc.top, cRc.right, cRc.bottom);
					else
					{
						dRc[ii].SetRectEmpty();
						
						for (int jj = 3; jj >= 0; jj--)
						{
							if (!dRc[jj].IsRectEmpty())
							{
								dRc[jj].right = cRc.right;
								break;
							}
						}
					}
				}
				else
				{
					if (value)
					{
						dRc[ii].SetRect(cRc.left + total, cRc.top, cRc.left + total + value, cRc.bottom);
						value++;
					}
					else 
						dRc[ii].SetRectEmpty();
				}
				total += value;
			}

			for (ii = 0; ii < 5; ii++)
			{
				dRc[ii].DeflateRect(0, 2);
				pDC->FillSolidRect(dRc[ii], m_color[ii]);
			}

			pDC->SetTextColor(color);
		}
		break;
	}

	DrawAr(pDC);

	pDC->SelectObject(font);
	pDC->SelectObject(pen);
}

void CPane::Draw_Inters(CDC* pDC)
{
	if (m_arInterByCode.GetCount() < 1)
		return;
	if (m_index < 0)	m_index = 0;

	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;
	pDC->SetBkMode(TRANSPARENT);

	CString	text, keys, sym;
	CRect	cRc, tRc;

	GetClientRect(cRc);
	pDC->SetBkMode(TRANSPARENT);

// draw data
	_inter* inter = m_arInter.GetAt(m_index);
	text.Format("%s", inter->Name);
	if (inter->Name.IsEmpty())	return;

	CFont*	font = pDC->SelectObject(&m_font);

	GetClientRect(cRc);
	int	cx = pDC->GetTextExtent(PANETITLEWIDTH).cx;
	cRc.left += cx + PGAP;
	cRc.right -= (PGAP + m_wBitmap);

	COLORREF color;
	switch (GetSign(inter->curr))
	{
	case '+':
		color = pDC->SetTextColor(GetColor(COLOR_TICK_UP));
		sym.LoadString(ST_SYMBOL_UP2);
		break;
	case '-':
		color = pDC->SetTextColor(GetColor(COLOR_TICK_DOWN));
		sym.LoadString(ST_SYMBOL_DOWN2);
		break;
	default:
		color = pDC->SetTextColor(GetColor(COLOR_TICK_TEXT));
		break;
	}
	
	text = commaE(inter->curr);
	cx = pDC->GetTextExtent(" " + text).cx;
	tRc.SetRect(cRc.left, cRc.top, cRc.left + cx, cRc.bottom);
	pDC->DrawText(" " + text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	text.Format("  %s", dbE(inter->diff));
	cx = pDC->GetTextExtent(text).cx;
	tRc.left = tRc.right;
	tRc.right = tRc.left + cx;

	pDC->DrawText(text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(color);

	text.Format("  %s", commaE(inter->gvol));
	cx = pDC->GetTextExtent(text).cx;
	tRc.left = tRc.right;
	tRc.right = tRc.left + cx;

	pDC->DrawText(text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	DrawAr(pDC);
	pDC->SelectObject(font);
}

void CPane::Draw_Conclusion(CDC *pDC)
{
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;

	CString	text;
	CRect	cRc, tRc;

	GetClientRect(cRc);
	CFont* font = pDC->SelectObject(&m_font);
	int cx = pDC->GetTextExtent(PANETITLEWIDTH).cx;

	tRc.SetRect(cRc.left + cx + PGAP, cRc.top, cRc.right, cRc.bottom);
	if (m_arConclusion.GetSize())
	{
		text = m_arConclusion.GetAt(0);
		tRc.right -= m_wBitmap;

		pDC->DrawText(" " + text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);
	}

	DrawAr(pDC);

	pDC->SelectObject(font);
}

void CPane::SetPaneInfo(int index)
{
	int	ii, idx, fontsize, bold;
	char	buf[64];
	CString	file, section, keys, value, fontname;

	m_arSymData.RemoveAll();
	file.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_userN, TICKSETUP);

	fontsize = GetPrivateProfileInt("GENERAL", "fontsize", 9, file);
	bold = GetPrivateProfileInt("GENERAL", "bold", 0, file);
	GetPrivateProfileString("GENERAL", "fontname", "굴림체", buf, sizeof(buf), file);
	fontname = buf;

	m_font.DeleteObject();
	LOGFONT	lf;
	m_font.CreatePointFont(fontsize*10, fontname);
	m_font.GetLogFont(&lf);
	m_font.DeleteObject();
	m_font.CreateFont(lf.lfHeight, lf.lfWidth, lf.lfEscapement, lf.lfOrientation, bold ? FW_BOLD : FW_NORMAL,	
				lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet, lf.lfOutPrecision,		
				lf.lfClipPrecision, lf.lfQuality, lf.lfPitchAndFamily,		
				(char *) fontname.operator LPCTSTR());
	m_bkColor = GetPrivateProfileInt("GENERAL", "color", m_bkColor, file);
	m_interval = GetPrivateProfileInt("GENERAL", "period", 5, file) * 1000;
	section.Format("BAR_INFO_%02d", index);

	m_id = index;
	m_kind = GetPrivateProfileInt(section, "INDEXK", TKIND_INDEX, file);

	switch (m_kind)
	{
	default:
	case TKIND_INDEX:
		for (ii = 0; ; ii++)
		{
			keys.Format("%03d", ii);
			DWORD dw = GetPrivateProfileString(section, keys, "", buf, sizeof(buf), file);
			if (dw <= 0)	break;
			idx = GetTicKey(atoi(buf));
			m_arItem.Add(idx);
		}
		if (m_arItem.GetSize() <= 0)	m_arItem.Add(2);
		m_aux = GetPrivateProfileInt(section, "AUX", 0, file);
		break;
	case TKIND_NEWS:
		m_arItem.Add(NEWS_BASE);
		for (ii = 0; ; ii++)
		{
			keys.Format("%02d", ii);
			DWORD dw = GetPrivateProfileString(section, keys, "", buf, sizeof(buf), file);
			if (dw <= 0)	break;
			idx = GetNewsKey(atoi(buf));
			m_arNewsKey.Add(idx);
		}
		if (m_arNewsKey.GetSize() <= 0)	m_arNewsKey.Add(2);/*공시*/
		m_aux = GetPrivateProfileInt(section, "AUX", 0, file);
		break;
	case TKIND_UPDOWN:
		ii = GetPrivateProfileInt(section, "SELECT", 0, file);
		switch (ii)
		{
		case 0:case 1:
			m_arItem.Add(UPDN_KOSPI+ii);	break;
		case 2:	
			m_arItem.Add(UPDN_KOSPI);
			m_arItem.Add(UPDN_KOSDAQ);	break;
		default:m_arItem.Add(UPDN_KOSPI);	break;
		}
		m_aux = GetPrivateProfileInt(section, "AUX", 1, file);
		break;
	case TKIND_INTER:
		loadInter(section);
		break;
		{
			m_aux = GetPrivateProfileInt(section, "SELECT", -1, file);
			if (m_aux == -1)	break;
			value.Format("%02d", m_aux);
			loadInter(value);
		}
		break;
	}

	if (m_arItem.GetSize())
		m_index = 0;
	else	m_index = -1;
}

void CPane::SetFmx(bool setting, int kind, int key, CString symbol, CString dat)
{
	if (!setting)
	{
		if (m_index >= m_arItem.GetSize())
			m_index = 0;
		if (m_index >= 0 && m_index < m_arSym.GetSize())
		{
			CString	sym, dat;
			sym = m_arSym.GetAt(m_index);
			m_arSymData.Lookup(sym, dat);
			SetData(dat);
		}
		Invalidate();

		switch (m_kind)
		{
		case TKIND_INDEX:
			if (m_arSym.GetSize() > 1)
				SetTimer(TM_INDEX, m_interval, NULL);
			break;
		case TKIND_UPDOWN:
			if (m_arSym.GetSize() > 1)
				SetTimer(TM_UPDOWN, m_interval, NULL);
			break;
		case TKIND_INTER:
			if (m_arSym.GetSize() > 1)
				SetTimer(TM_INTER, m_interval, NULL);
			break;
		}

		return;
	}

	if (m_kind != kind)	return;

	POSITION pos;
	for (int ii = 0; ii < m_arItem.GetSize(); ii++)
	{
		int	sKey = m_arItem.GetAt(ii);
		if (sKey != key)	continue;

		CString	oldDat, keys, value, tmps;
		
		CMapStringToString	newfms, oldfms;
		if (m_arSymData.Lookup(symbol, oldDat))
		{

			parseDat(dat, newfms);
			parseDat(oldDat, oldfms);

			if (oldfms.GetCount())
			{
				for (pos = newfms.GetStartPosition(); pos; )
				{
					newfms.GetNextAssoc(pos, keys, value);
					oldfms.SetAt(keys, value);
				}

				dat.Empty();
				for (pos = oldfms.GetStartPosition(); pos; )
				{
					oldfms.GetNextAssoc(pos, keys, value);
					if (dat.IsEmpty())
						dat.Format("%s\t%s", keys, value);
					else
					{
						tmps = dat;
						dat.Format("%s\t%s\t%s", tmps, keys, value);
					}
				}
			}

			newfms.RemoveAll();
			oldfms.RemoveAll();
		}

		if (m_kind == TKIND_NEWS)
		{
			m_arSymData.SetAt(symbol, dat);
			parseDat(dat, newfms);
			if (newfms.Lookup(SNEW_INFO, value))
			{
				int	newsKey;
				for (int jj = 0; jj < m_arNewsKey.GetSize(); jj++)
				{
					newsKey = m_arNewsKey.GetAt(jj);
					if (newsKey == atoi(value))
					{
						SetData(dat);
						m_arSymData.SetAt(symbol, dat);
						m_arHistoryNews.InsertAt(0, dat);

						if (m_arHistoryNews.GetSize() > MAX_NEWS)
							m_arHistoryNews.RemoveAt(MAX_NEWS);
						break;
					}
				}
			}
	
			newfms.RemoveAll();
		}
		else
		{
			setSign(sKey, dat);
			m_arSymData.SetAt(symbol, dat);
			tmps.Format("%d", key);
			m_arIndexbySym.SetAt(symbol, tmps);
			m_arSymbyIndex.SetAt(tmps, symbol);
			m_arSym.Add(symbol);
		}
		break;
	}
}

void CPane::SetInter(char* dat, bool bRotate/*=true*/)
{
	if (m_kind != TKIND_INTER)
		return;
	
	struct	_iGrid {
		char	code[12];		 /* 종목코드			 */
		char    hnam[20];                /* 종목명                       */
		char    curr[8];                 /* 현재가                       */
		char    diff[6];                 /* 전일대비      (9999V99)      */
		char    gvol[12];                /* 거래량                       */
	};
	CString	code, name, curr, diff, gvol;

	_iGrid*	grid = (_iGrid *) dat;
	code = CString(grid->code, sizeof(grid->code));	code.TrimRight();
	name = CString(grid->hnam, sizeof(grid->hnam));	name.TrimRight();
	curr = CString(grid->curr, sizeof(grid->curr));	curr.TrimRight();
	diff = CString(grid->diff, sizeof(grid->diff));	diff.TrimLeft();
	gvol = CString(grid->gvol, sizeof(grid->gvol));	gvol.TrimLeft();

	if (code.GetLength() == 7 && code.GetAt(0) == 'J')	code.SetAt(0, 'A');
	_inter*	inter = NULL;
	m_arInterByCode.Lookup(code, (CObject *&) inter);
	if (!inter)	return;
	inter->Name = name;
	inter->curr = curr;
	inter->diff = diff;
	inter->gvol = gvol;

	Invalidate(FALSE);
	if (bRotate && m_arInterByCode.GetCount() > 1)
	{
		m_index = 0;
		SetTimer(TM_INTER, m_interval, NULL);
	}
}

void CPane::UpdatePaneData(int kind, CString dat)
{
	if (kind != m_kind)
		return;
	
	SetData(dat);
	Invalidate();
}

void CPane::ProcessRTS(CString symbol, CString dat)
{
	CString		keys, value, tmps;
	CMapStringToString	newfms, oldfms;

	if (symbol.GetLength() == 7 && symbol.GetAt(0) == 'J')	symbol.SetAt(0, 'A');
	if (!m_arSymData.Lookup(symbol, value))	return;
	if (m_index == -1)	return;
	
	parseDat(dat, newfms);
	parseDat(value, oldfms);

	if (m_arIndexbySym.Lookup(symbol, tmps))
		setSign(atoi(tmps), dat);
	POSITION pos;
	if (m_kind != TKIND_NEWS && oldfms.GetCount())
	{
		for (pos = newfms.GetStartPosition(); pos; )
		{
			newfms.GetNextAssoc(pos, keys, value);
			oldfms.SetAt(keys, value);
		}

		dat.Empty();
		for (pos = oldfms.GetStartPosition(); pos; )
		{
			oldfms.GetNextAssoc(pos, keys, value);
			if (dat.IsEmpty())
				dat.Format("%s\t%s", keys, value);
			else
			{
				tmps = dat;
				dat.Format("%s\t%s\t%s", tmps, keys, value);
			}
		}

		if (m_panelist)		m_panelist->Invalidate(FALSE);
	}

	m_arSymData.SetAt(symbol, dat);
	if (m_kind == TKIND_INTER)
	{
		_inter* inter;

		m_arInterByCode.Lookup(symbol, (CObject *&) inter);
		newfms.Lookup(S_CUR, inter->curr);	// 현재가
		newfms.Lookup(S_DB, inter->diff);	// 대비
		newfms.Lookup(S_NVOL, inter->gvol);	// 거래량
		if (!m_arSym.GetAt(m_index).Compare(symbol))
			Invalidate();

		if (m_panelist)		m_panelist->Invalidate(FALSE);
	}
	else if (m_kind == TKIND_NEWS)
	{
		if (newfms.Lookup(SNEW_INFO, value))
		{
			int	newsKey;
			for (int jj = 0; jj < m_arNewsKey.GetSize(); jj++)
			{
				newsKey = m_arNewsKey.GetAt(jj);
				if (newsKey == atoi(value))
				{
					SetData(dat);
					Invalidate();
					m_arHistoryNews.InsertAt(0, dat);
					break;
				}
			}
		}

		if (m_panelist)		m_panelist->Invalidate(FALSE);
	}
	else if (m_index >= 0 && m_index < m_arSym.GetSize())
	{
		value = m_arSym.GetAt(m_index);
		if (!value.Compare(symbol))
		{
			SetData(dat);
			Invalidate();
		}

		if (m_panelist)		m_panelist->Invalidate(FALSE);
	}

	newfms.RemoveAll();
	oldfms.RemoveAll();
}

void CPane::ProcessNotice(CString str)
{
	if (m_kind != TKIND_CONCLUSION)
		return;

	m_arConclusion.Add(str);

	Invalidate(FALSE);
}

CString CPane::commaE(CString value, bool remove)
{
	CString		tmps, gb, tail;
	int		pos;

	value.TrimLeft();	value.TrimRight();
	if (value.IsEmpty())	return _T("");

	if (value.Left(1) == "+" || value.Left(1) == "-")
	{
		gb = value.Left(1);
		value = value.Mid(1);
	}
	
	pos = value.Find('.');
	if (pos != -1)
	{
		tail = value.Mid(pos);
		value = value.Left(pos);
	}

	if (remove)	gb = _T("");
	if (value.GetLength() < 3)
		return gb + value + tail;

	for (int ii = value.GetLength()-1, jj = 0; ii >= 0; jj++)
	{
		if (jj != 0 && jj % 3 == 0)
			tmps.Insert(0, ',');
		tmps.Insert(0, value.GetAt(ii));
		ii--;
	}
	return gb + tmps + tail;
}

COLORREF CPane::GetColor(int index)
{
	int		pos;
	char		buffer[128];
	CString		file, tmpS, dat, section, key;
	CStringArray	ary;

	file.Format("%s\\%s\\%s", m_home, TABDIR, PALETTE);
	GetPrivateProfileString(GENERALSN, "Palette", "Palette", buffer, sizeof(buffer), file);

	section = buffer;
	key.Format("RGB%03d", index);
	DWORD dw = GetPrivateProfileString(section, key, "", buffer, sizeof(buffer), file);
	if (dw <= 0)
	{
		if (!section.Compare("Palette"))
			return RGB(215, 215, 215);
		section = _T("Palette");
		dw = GetPrivateProfileString(section, key, "", buffer, sizeof(buffer), file);
		if (dw <= 0)	return RGB(215, 215, 215);
	}

	dat = buffer;
	for (; !dat.IsEmpty();)
	{
		pos = dat.Find(',');
		if (pos == -1)
		{
			ary.Add(dat);
			break;
		}
		tmpS = dat.Left(pos++);
		dat = dat.Mid(pos);
		ary.Add(tmpS);
	}

	if (ary.GetSize() < 3)
		return RGB(215, 215, 215);
	return RGB(atoi(ary.GetAt(0)), atoi(ary.GetAt(1)), atoi(ary.GetAt(2)));
}

void CPane::SetData(CString dat)
{
	CString sym, value;
	int	pos;

	m_mapData.RemoveAll();
	while (!dat.IsEmpty())
	{
		pos = dat.Find('\t');
		if (pos == -1)	break;
		sym = dat.Left(pos++);
		dat = dat.Mid(pos);

		pos = dat.Find('\t');
		if (pos == -1)
		{
			value = dat;
			dat.Empty();
		}
		else
		{
			value = dat.Left(pos++);
			dat = dat.Mid(pos);
		}

		value.TrimRight();
		m_mapData.SetAt(sym, value);
	}
}

void CPane::SetData(CString dat, CMapStringToString &mapData)
{
	CString sym, value;
	int	pos;

	mapData.RemoveAll();
	while (!dat.IsEmpty())
	{
		pos = dat.Find('\t');
		if (pos == -1)	break;
		sym = dat.Left(pos++);
		dat = dat.Mid(pos);

		pos = dat.Find('\t');
		if (pos == -1)
		{
			value = dat;
			dat.Empty();
		}
		else
		{
			value = dat.Left(pos++);
			dat = dat.Mid(pos);
		}

		value.TrimRight();
		mapData.SetAt(sym, value);
	}
}

void CPane::parseDat(CString dat, CMapStringToString& ary)
{
	CString sym, value;
	int	pos;
	while (!dat.IsEmpty())
	{
		pos = dat.Find('\t');
		if (pos == -1)	break;
		sym = dat.Left(pos++);
		dat = dat.Mid(pos);

		pos = dat.Find('\t');
		if (pos == -1)
		{
			value = dat;
			dat.Empty();
		}
		else
		{
			value = dat.Left(pos++);
			dat = dat.Mid(pos);
		}

		ary.SetAt(sym, value);
	}
}

CString CPane::dbE(CString value)
{
	CString		retS, sign;

	if (value.IsEmpty())	return _T("");

	char ch = value.GetAt(0);
	value = value.Mid(1);
	value.TrimLeft();

	switch (ch)
	{
	case '1':	// 상한
		sign.LoadString(ST_SYMBOL_UP1);	break;// = "↑";	break;
	case '2':	// 상승
	case '+':
		sign.LoadString(ST_SYMBOL_UP2);	break;//sign = "▲";	break;
	default:
	case '3':	// 보합
		sign = "";	break;
	case '4':	// 하한
		sign.LoadString(ST_SYMBOL_DOWN1);	break;//sign = "↓";	break;
	case '5':	// 하락
	case '-':
		sign.LoadString(ST_SYMBOL_DOWN2);	break;//sign = "▼";	break;
	}

	value = commaE(value);
	retS.Format("%s %6s", sign, value);
	return retS;
}

CString CPane::getDBSign(CString value)
{
	CString		retS, sign;

	if (value.IsEmpty())	return _T("");

	char ch = value.GetAt(0);
	value = value.Mid(1);
	value.TrimLeft();

	switch (ch)
	{
	case '1':	// 상한
		sign.LoadString(ST_SYMBOL_UP1);	break;// = "↑";	break;
	case '2':	// 상승
	case '+':
		sign.LoadString(ST_SYMBOL_UP2);	break;//sign = "▲";	break;
	default:
	case '3':	// 보합
		sign = "";	break;
	case '4':	// 하한
		sign.LoadString(ST_SYMBOL_DOWN1);	break;//sign = "↓";	break;
	case '5':	// 하락
	case '-':
		sign.LoadString(ST_SYMBOL_DOWN2);	break;//sign = "▼";	break;
	}

	return sign;
}

// 0 - 보합, 1 - 상승, 2 - 하락
char CPane::GetSign(CString dat)
{
	if (dat.Find('+') != -1)return '+';
	if (dat.Find('-') != -1)return '-';
	return ' ';
}

void CPane::loadInter(CString section)
{
	int ii = 0;

	for (ii = 0; ii < m_arInter.GetSize(); ii++)
		delete m_arInter.GetAt(ii);
	m_arInter.RemoveAll();
	m_arInterByCode.RemoveAll();

	DWORD	dw;
	char	wb[16];
	CString	file, keys, value;

	file.Format("%s\\user\\%s\\%s", m_home, m_userN, TICKSETUP);
	for (ii = 0; ; ii++)
	{
		keys.Format("%02d", ii);
		dw = GetPrivateProfileString(section, keys, "", wb, sizeof(wb), file);
		if (dw <= 0)	break;

		_inter* inter = new _inter;
		inter->Code = wb;

		m_arInter.Add(inter);
		m_arInterByCode.SetAt(inter->Code, (CObject *) inter);
		m_arSymData.SetAt(inter->Code, "x");
		m_arSym.Add(inter->Code);
	}
}

int CPane::GetPosY(int value, int max)
{
	CRect	cRc;
	
	GetClientRect(cRc);
	cRc.DeflateRect(0, 2);
	int	posy = cRc.bottom;
	double	inc = 0.0, valueByPixel = (double)max/(double)cRc.Height();
	if (!value)	return posy;

	for (;;)
	{
		if (max <= value)
		{
			posy = cRc.top;
			break;
		}

		if (inc >= value)
			break;
		posy--;
		inc += valueByPixel;
	}

	return posy;
}

CString CPane::GetIndexSName(int index)
{
	CString	SName;
	
	switch (index)
	{
	case 1:		SName = _T("KRX100");		break;
	case 2:		SName = _T("KOSPI");		break;
	case 3:		SName = _T("KOSDAQ");		break;
	case 4:		SName = _T("KOSPI200");		break;
	case 5:		SName = _T("KOSPI100");		break;

	case 6:		SName = _T("배당지수");		break;
	case 7:		SName = _T("KOSTAR");		break;
	case 8:		SName = _T("BASIS");		break;
	case 9:		SName = _T("선물");		break;
	case 10:	SName = _T("스타선물");		break;

	case 11:	SName = _T("S&&P500");		break;
	case 12:	SName = _T("NASDAQ100");	break;
	case 13:	SName = _T("환율");		break;
	case 14:	SName = _T("KOSPI");		break;
	case 15:	SName = _T("KOSDAQ");		break;
	case 17:	SName = _T("NIKKEI225");	break;
	case 18:	SName = _T("항셍H지수");	break;
	default:	SName = _T("지수");		break;
	}
	return SName;
}

void CPane::saveinfo(int selectN)
{
	CString	file, section, value;

	file.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_userN, TICKSETUP);
	section.Format("BAR_INFO_%02d", m_id);
	value.Format("%d", selectN);
	WritePrivateProfileString(section, "SELECT", value, file);
}

BOOL CPane::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
}

void CPane::OnLButtonDblClk(UINT nFlags, CPoint point) 
{

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CPane::setSign(int key, CString dat)
{
	if (m_kind != TKIND_INDEX)
		return;

	CMapStringToString	fms;
	CString			value;

	parseDat(dat, fms);
	if (fms.Lookup(NEWKJ, value))
	{
		m_arSign[key].InsertAt(0, value);
		while (m_arSign[key].GetSize() > 5)
			m_arSign[key].RemoveAt(m_arSign[key].GetSize()-1);
	}
}

void CPane::OnRButtonDown(UINT nFlags, CPoint point) 
{
	(*m_axiscall)(AXI_TICKSETUP, (WPARAM)m_id, 0);
//	CWnd::OnRButtonDown(nFlags, point);
}

int CPane::GetTicKey(int index)
{
	int	key = 0;
	switch (index)
	{
	default:
	case 0:		key = 1;	break;
	case 1:		key = 2;	break;
	case 2:		key = 3;	break;
	case 3:		key = 4;	break;
	case 4:		key = 5;	break;	
	case 5:		key = 6;	break;
	case 6:		key = 7;	break;
	case 7:		key = 8; 	break;
	case 8:		key = 9; 	break;
	case 9:		key = 10;	break;
	case 10:	key = 11;	break;
	case 11:	key = 12;	break;
	case 12:	key = 13;	break;
	case 13:	key = 17;	break;
	case 14:	key = 18;	break;
	}
	return key;
}

int CPane::GetNewsKey(int index)
{
	int	key = 0;
	switch (index)
	{
	default:
	case 1:		key = 2;	break;		// 증전공시
	case 2:		key = 4;	break;		// 이데일리
	case 3:		key = 3;	break;		// 머니투데이
	case 4:		key = 6;	break;		// 한국경제
	case 5:		key = 7;	break;		// 연합뉴스
	case 6:		key = 8;	break;		// 인포스탁
	case 7:		key = 9; 	break;		// FnGuide
	case 8:		key = 5; 	break;		// 해외뉴스
	}
	return key;
}

CString CPane::GetdataUPDN()
{
	if (m_kind != TKIND_UPDOWN)
		return "";

	CString	dats, kSym = "001";
	if (m_arItem.GetAt(m_index) == 15)	kSym = "201";

	dats.Format("SE71010020301\t%s\n", kSym);
	return dats;
}

void CPane::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

CString CPane::GetNewsTitle(int index)
{
	CString title;

	switch (index)
	{
	case 1:		title = _T("시황");		break;
	case 2:		title = _T("공시");		break;
	case 3:		title = _T("머니투데이");	break;
	case 4:		title = _T("이데일리");		break;
	case 5:		title = _T("해외뉴스");		break;
	case 6:		title = _T("한국경제");		break;
	case 7:		title = _T("연합뉴스");		break;
	case 8:		title = _T("인포스탁");		break;
	case 9:		title = _T("FnGuide");		break;
	}

	return title;
}

void CPane::DrawHead(CDC *pDC)
{
	CRect	cRc, tRc;
	CString	text, keys, value, sym, tmps;
	CFont*	font = pDC->SelectObject(&m_font);
	m_wHead = pDC->GetTextExtent(PANETITLEWIDTH).cx;

	GetClientRect(cRc);
	tRc.SetRect(cRc.left, cRc.top, cRc.left + m_wHead, cRc.bottom);

	pDC->MoveTo(tRc.right, tRc.top + 2);
	pDC->LineTo(tRc.right, tRc.bottom - 2);
	tRc.DeflateRect(1, 0);

	switch (m_kind)
	{
	case TKIND_INDEX:	
		{
			BOOL	bExpect = FALSE;

			if (m_index < 0 ||m_index >= m_arItem.GetSize())//m_mapData.GetCount() <= 0)
				return;
			
			int bkMode = pDC->SetBkMode(TRANSPARENT);
			int key = m_arItem.GetAt(m_index);
		
			if (m_mapData.Lookup("600", text))
			{
				if (atoi(text) == 0)
					bExpect = TRUE;
			}
			
			if (bExpect)
			{
				keys.Format("%d", key);
				m_arSymbyIndex.Lookup(keys, sym);
				text.Format("%s", GetExpectSymbolName(sym));
				if (text.IsEmpty())
					text.Format("%s", GetIndexSName(key));
			}
			else
				text.Format("%s", GetIndexSName(key));
			
			pDC->SetBkMode(bkMode);
		}
		break;
	case TKIND_NEWS:	
		if (m_mapData.Lookup(SNEW_INFO, keys))
			text = GetNewsTitle(atoi(keys));
		break;
	case TKIND_UPDOWN:	
		text.Format("%s", GetIndexSName(m_arItem.GetAt(m_index)));
		break;
	case TKIND_INTER:	
		{
			if (m_arInterByCode.GetCount() < 1)	break;
			if (m_index < 0)	m_index = 0;
			
			_inter* inter = m_arInter.GetAt(m_index);
			text.Format("%s", inter->Name);
			if (inter->Name.IsEmpty())	break;
			text.Replace("&", "&&");
			text.TrimLeft();	text.TrimRight();

			int cx2 = pDC->GetTextExtent(text).cx;

			if (cx2 > tRc.Width())
				pDC->DrawText(text, tRc, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
			else
				pDC->DrawText(text, tRc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			text = "";
		}
		break;
	case TKIND_CONCLUSION:
		text = "체결";
		break;
	}

	if (!text.IsEmpty())
		pDC->DrawText(text, tRc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	pDC->SelectObject(font);
}

BOOL CPane::GetPaneData(int index, CString &keys, CString &sym, CMapStringToString &mapData)
{
	switch (m_kind)
	{
	case TKIND_INDEX:
	case TKIND_UPDOWN:
		{
			if (index >= m_arItem.GetSize())
				return FALSE;
			CString	dat;
			
			keys.Format("%d", m_arItem.GetAt(index));
			m_arSymbyIndex.Lookup(keys, sym);
			m_arSymData.Lookup(sym, dat);
			SetData(dat, mapData);
		}
		break;
	}

	return TRUE;
}

LRESULT CPane::OnPanelist(WPARAM wParam, LPARAM lParam)
{
	switch ((int)LOWORD(wParam))
	{
	case 0:		// destroy
		if ((CPaneList *)lParam == m_panelist && m_panelist)
		{
			m_panelist->DestroyWindow();
			delete m_panelist;
			m_panelist = NULL;
		}
		break;
	case 1:		// click
		if ((CPaneList *)lParam == m_panelist && m_panelist)
		{
			PopUpScreen(HIWORD(wParam));

			m_panelist->DestroyWindow();
			delete m_panelist;
			m_panelist = NULL;
		}
		break;
	}

	return 0;
}

void CPane::PopUpScreen(int index)
{
	if (index < 0)	index = m_index;

	int id;
	CString		text, tmps, data;

	switch (m_kind)
	{
	case TKIND_INDEX:
		id = m_arItem.GetAt(index);
		switch (id)
		{
		case 1:	if (!GetExpect(index))	data = "SE71030020301\t999";
			else			data = "SE710700upgb\t999";	break;		// krx100
		case 2:	if (!GetExpect(index))	data = "SE71030020301\t001";
			else			data = "SE710700upgb\t001";	break;		// kospi
		case 3: if (!GetExpect(index))	data = "SE71030020301\t201";	
			else			data = "SE710700upgb\t201";	break;		// kosdaq
		case 4:	if (!GetExpect(index))	data = "SE71030020301\t101";	
			else			data = "SE710700upgb\t101";	break;		// kospi200
		case 5: if (!GetExpect(index))	data = "SE71030020301\t401";
			else			data = "SE71030020301\t401";	break;		// kospi100
		case 6:	if (!GetExpect(index))	data = "SE71030020301\t041";
			else			data = "SE710700upgb\t041";	break;		// 배당지수
		case 7: if (!GetExpect(index))	data = "SE71030020301\t303";	
			else			data = "SE710700upgb\t303";	break;		// kostar

		// future index
		case 8:							// basis
		case 9:							// kospi future
		case 10:						// star future
			tmps.Format("%d", id);
			m_arSymbyIndex.Lookup(tmps, text);
			data.Format("SE73000030301\t%s", text);
			break;
		case 11:						// S&P500
		case 12:						// NASDAQ100
			data = "SE740300";			break;
		case 13:						// EXCHANGE
			data = "SE740400symb2\tUSD";		break;
		case 17:						// nikkei
			data = "SE740400symb2\tNII@NI225";	break;
		case 18:						// hangseng
			data = "SE740400symb2\tHSI@HSCE";	break;
		}
		if (!data.IsEmpty())
			(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(char *) data.operator LPCTSTR());
		break;
	case TKIND_NEWS:
		{
			if (index >= m_arHistoryNews.GetSize())
				return;

			CMapStringToString	fms;
			CString value;
			
			fms.RemoveAll();
			value = m_arHistoryNews.GetAt(index);
			parseDat(value, fms);
			
			if (fms.Lookup(SNEW_INFO, text))
			{
				text.TrimRight();
				if (!text.IsEmpty())
				{
					tmps.Format("gubn\t%s", text);
					data += tmps;
				}
			}
			
			if (fms.Lookup(SNEW_ISAMNO, text))
			{
				text.TrimRight();
				if (!text.IsEmpty())
				{
					tmps.Format("\nisrn\t%s", text);
					data += tmps;
				}
			}
			
			fms.RemoveAll();
			(*m_axiscall)(AXI_NEWSLIST, 0, (LPARAM)(char *) data.operator LPCTSTR());
		}
		break;
	case TKIND_UPDOWN:
		data = GetdataUPDN();
		if (!data.IsEmpty())
			(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(char *) data.operator LPCTSTR());
		break;
	case TKIND_INTER:
		{
			if (index < 0 || index >= m_arInter.GetSize())
				break;
			_inter* inter = m_arInter.GetAt(index);
			if (inter->Code.GetLength() == 8)
				data.Format("SE730100focod\t%s", inter->Code);
			else
				data.Format("SE7000001301\t%s", inter->Code.Mid(1));
			(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(char *) data.operator LPCTSTR());
		}
		break;
	case TKIND_CONCLUSION:
		(*m_axiscall)(AXI_SHOWDEAL, 0, 0);
		break;
	}
}

bool CPane::GetExpect(int index)
{
	if (index < 0 || index >= m_arItem.GetSize())
		return false;

	CMapStringToString	mapData;
	CString key, sym, text;
	GetPaneData(index, key, sym, mapData);	

	if (mapData.Lookup("600", text))
	{
		if (atoi(text) == 0)
			return true;
	}
	
	return false;
}