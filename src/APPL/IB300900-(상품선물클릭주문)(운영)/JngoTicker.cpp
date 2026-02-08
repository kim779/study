// JngoTicker.cpp : implementation file
//

#include "stdafx.h"
#include "IB300900.h"
#include "JngoTicker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJngoTicker

CJngoTicker::CJngoTicker(CWnd *pParent)
: CGridCtrl(), CAxisExt(pParent)
{
}

CJngoTicker::~CJngoTicker()
{
}


BEGIN_MESSAGE_MAP(CJngoTicker, CGridCtrl)
	//{{AFX_MSG_MAP(CJngoTicker)
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
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

LRESULT CJngoTicker::OnRefresh( WPARAM wParam, LPARAM lParam )
{
//	OutputDebugString("JANGO TICKER REFRESH================================\n");
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
	if (id.row==1 && m_LeftClickDownCell==id)
	{
		if (id.col==1 || id.col==2) 
		{
			CString data = GetItemText(1, id.col);
			int jcnt = atoi((LPCSTR)data);
			GetParent()->SendMessage(WM_APP_SIG, WP_SET_JCNT, (LPARAM)jcnt);
		}
	}
	CGridCtrl::OnLButtonUp(nFlags, point);
}
