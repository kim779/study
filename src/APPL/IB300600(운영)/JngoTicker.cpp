// JngoTicker.cpp : implementation file
//

#include "stdafx.h"
#include "IB300600.h"
#include "JngoTicker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJngoTicker

#define TM_SETCURSOR 9898

CJngoTicker::CJngoTicker(CWnd *pParent)
: CGridCtrl(), CAxisExt(pParent)
{
	m_iDragJango = 0;
	m_hHandCursor = ::LoadCursor(NULL, IDC_HAND);
	m_hOriCursor = ::LoadCursor(NULL, IDC_ARROW);
	m_mouseDragmode = false;
}

CJngoTicker::~CJngoTicker()
{
}


BEGIN_MESSAGE_MAP(CJngoTicker, CGridCtrl)
	//{{AFX_MSG_MAP(CJngoTicker)
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_APP_REFRESH, OnRefresh)
END_MESSAGE_MAP()

BOOL CJngoTicker::Create( const RECT& rect, CWnd* parent, UINT nID, DWORD dwStyle /*= WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE */, DWORD dwExStyle /*= 0*/ )
{
	BOOL bRet = CGridCtrl::Create(rect, parent, nID, dwStyle, dwExStyle);
	SetFont(m_pFont, FALSE);
	SetGrid();
	return bRet;
}

void CJngoTicker::ChangeTheme()
{
	SetGrid();
	Invalidate(TRUE);
}

void CJngoTicker::SetGrid()
{
	AxInit();

	CRect rc;
	GetClientRect(rc);
	SetNoScrollBar(TRUE);
	SetColumnResize(FALSE);
	EnableSelection(FALSE);
	SetDoubleBuffering(TRUE);
	SetEditable(FALSE);
	SetFrameFocusCell(FALSE);
	
	SetGridBorderColor(RGB(0x6B,0x6B,0x6B));
	SetGridLineColor(RGB(0x6B,0x6B,0x6B));
	SetDefCellMargin(5);
	SetDefCellHeight(GRID_CELL_HEIGHT);
	SetColumnCount(7);
	SetRowCount(2);
	SetFixedColumnCount(0);
	SetFixedRowCount(0);

	UINT format[] = { DT_CENTER, DT_RIGHT, DT_RIGHT, DT_RIGHT, DT_RIGHT, DT_RIGHT, DT_RIGHT};
	char *head_txt[] = { "구분", "잔고", "청산", "평균가", "평가금액", "평가손익", "수익률" };
	for(int x=0; x<GetColumnCount(); ++x)
	{
		GV_ITEM item;
		item.row = 0;
		item.col = x;
		item.crBkClr = GetIndexColor(COLOR_GRIDHEAD);
		item.crFgClr = GetIndexColor(COLOR_GRIDHEADTXT);	
		item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_BKCLR|GVIF_FGCLR;
		item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
		item.strText = head_txt[x];
		SetItem(&item);

		SetItemFormat(1, x, DT_VCENTER|DT_SINGLELINE|format[x]);
	}
}

void CJngoTicker::OnSize(UINT nType, int cx, int cy) 
{
	CGridCtrl::OnSize(nType, cx, cy);
	EnableScrollBars(SB_BOTH, FALSE);

	if (nType==SIZE_RESTORED)
	{
		CRect rc;
		GetClientRect(&rc);
		int width = rc.Width();
		int height = rc.Height();
		
		if (width && GetColumnCount()>0)
		{
			int dr[] = { 1, 1, 1, 1, 2, 2, 1 };
			int dx = width / 9;
			int x = 0;
			for(x=0; x<(GetColumnCount()-1); ++x)
			{
				m_arColWidths[x] = dx * dr[x];
				width -= dx * dr[x];
			}
			m_arColWidths[x] = width+1;
		}
		
		if (height && GetRowCount()>0)
		{
			int y, dy = height / GetRowCount();
			for(y=0; y<(GetRowCount()-1); ++y)
			{
				m_arRowHeights[y] = dy;
				height -= dy;
			}
			m_arRowHeights[y] = height+1;
		}
	} 
}

void CJngoTicker::SetTitleChange(int nColumn,CString text)
{
	if(nColumn > 6)
		return;
	
	GV_ITEM item;
	item.row = 0;
	item.col = nColumn;
	item.crBkClr = GetIndexColor(COLOR_GRIDHEAD);
	item.crFgClr = GetIndexColor(COLOR_GRIDHEADTXT);	
	item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_BKCLR|GVIF_FGCLR;
	item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	item.strText = text;

	SetItem(&item);

	RedrawRow(0);
}

LRESULT CJngoTicker::OnRefresh( WPARAM wParam, LPARAM lParam )
{
	COLORREF col;
	CString tamt, srat;
	Jango *pj = (Jango*)lParam;
	if (pj->flag!='D')
	{
		if      (pj->gubn[0]=='-') SetItemFgColour(1, 0, m_crMinus);
		else if (pj->gubn[0]=='+') SetItemFgColour(1, 0, m_crPlus);

		SetItemText(1, 0, &pj->gubn[1]);
		SetItemText(1, 1, Int2CommaStr(pj->jqty));
		SetItemText(1, 2, Int2CommaStr(pj->xqty));
		SetItemText(1, 3, Int2Str(pj->pprc, 2));
		SetItemText(1, 4, Int2CommaStr((int)pj->camt));

		if      (pj->tamt>0.0) { col = m_crPlus;  tamt = Int2CommaStr((int)pj->tamt); srat.Format("%.2f", pj->srat); }
		else if (pj->tamt<0.0) { col = m_crMinus; tamt = Int2CommaStr((int)pj->tamt); srat.Format("%.2f", pj->srat); }
		else                   { col = m_crBlack; tamt = "";                     srat = "";                     }
		
		SetItemFgColour(1, 5, col);
		SetItemText(1, 5, tamt);
		SetItemFgColour(1, 6, col);
		SetItemText(1, 6, srat);
		
		RedrawRow(1);
	}
	else
	{
		Clear();
	}
	return 0;
}

void CJngoTicker::Clear()
{
	for(int x=0; x<GetColumnCount(); ++x)
		SetItemText(1, x, "");
	RedrawRow(1);
}

void CJngoTicker::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CCellID id = GetCellFromPt(point);

CString slog;
slog.Format("[3006]_cursor -->OnLButtonUp<--  point x=[%d] point y=[%d] id.row=[%d] id.col=[%d] m_mouseDragmode=[%d]", point.x, point.y, id.row, id.col, m_mouseDragmode);
//OutputDebugString(slog);

	
	KillTimer(TM_SETCURSOR);
	if(m_mouseDragmode)
	{
		m_mouseDragmode = false; 
		ReleaseCapture();
		ClientToScreen(&point);
		if (WindowFromPoint(point) == this)
		{
			CGridCtrl::OnLButtonUp(nFlags, point);
			return;
		}

		GetParent()->SendMessage(WM_APP_SIG, WP_DRAG_JANGO, (LPARAM)m_iDragJango);
	}
	m_iDragJango = 0;
	m_mouseDragmode = false;  
slog.Format("[3006]_cursor -->OnLButtonUp<--  point x=[%d] point y=[%d] id.row=[%d] id.col=[%d] m_mouseDragmode=[%d]", point.x, point.y, id.row, id.col, m_mouseDragmode);
//OutputDebugString(slog);
slog.Format("[3006] jngoTicker onlbuttonup");
OutputDebugString(slog);
GetParent()->SetFocus();
	CGridCtrl::OnLButtonUp(nFlags, point);
}

CString CJngoTicker::GetpprcData()
{
	CString sResult = "";
	sResult = GetItemText(1, 3);
	return  sResult;
}

void CJngoTicker::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CString slog;
	CCellID id = GetCellFromPt(point);

slog.Format("[3006]_cursor -->OnLButtonDown<--  point x=[%d] point y=[%d] id.row=[%d] id.col=[%d] m_mouseDragmode=[%d]", point.x, point.y, id.row, id.col, m_mouseDragmode);
OutputDebugString(slog);

	if (id.row==1 && id.col==1)
	{	
		CString data = GetItemText(1, id.col);
		int jcnt = atoi((LPCSTR)data);

		if(jcnt <= 0)
		{
			
slog.Format("[3006]_cursor -->OnLButtonDown<-- jcnt = [%d]return", jcnt);
OutputDebugString(slog);

	//		CGridCtrl::OnLButtonDown(nFlags, point);
			return; 
		}

		SetCursor(m_hHandCursor);
		SetCapture();
		m_iDragJango = jcnt;
		m_mouseDragmode = true;

slog.Format("[3006]_cursor -->OnLButtonDown<--  point x=[%d] point y=[%d] id.row=[%d] id.col=[%d] m_mouseDragmode=[%d]", point.x, point.y, id.row, id.col, m_mouseDragmode);
OutputDebugString(slog);
		

	//	GetParent()->SendMessage(WM_APP_SIG, WP_SET_JCNT, (LPARAM)jcnt);  //주문수량에 드래그주문 수량 세팅

#ifndef _DEBUG
		SetTimer(TM_SETCURSOR, 100, NULL);
#endif

		return;
	}
	CGridCtrl::OnLButtonDown(nFlags, point);
}

void CJngoTicker::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CString slog;
	switch(nIDEvent)
	{
	case TM_SETCURSOR:
		{
			if(!m_mouseDragmode)
			{
slog.Format("[3006]_cursor -->OnTimer<-- m_mouseDragmode=[%d]",  m_mouseDragmode);
OutputDebugString(slog);
				SetCursor(m_hOriCursor);
				KillTimer(TM_SETCURSOR);
			}
			else
			{
//slog.Format("[3006]_cursor -->OnTimer<-- m_mouseDragmode=[%d]",  m_mouseDragmode);
//OutputDebugString(slog);
				SetCursor(m_hHandCursor);
			}
		}
		break;
	}
	CGridCtrl::OnTimer(nIDEvent);
}

BOOL CJngoTicker::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
/*
	if(m_mouseDragmode == true)
	{
CString slog;
slog.Format("[3006] OnSetCursor m_mouseDragmode true", m_mouseDragmode);
//OutputDebugString(slog);
		m_hOriCursor = SetCursor(m_hHandCursor);
		return TRUE;
	}else
	{
CString slog;
slog.Format("[3006] OnSetCursor m_mouseDragmode false", m_mouseDragmode);
//OutputDebugString(slog);
		SetCursor(m_hOriCursor);
		return TRUE;
	}
*/
	return CGridCtrl::OnSetCursor(pWnd, nHitTest, message);
}



void CJngoTicker::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	if(m_mouseDragmode)
//		SetCursor(m_hHandCursor);

	CGridCtrl::OnMouseMove(nFlags, point);
}
