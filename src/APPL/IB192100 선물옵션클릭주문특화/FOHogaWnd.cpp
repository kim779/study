// FOHogaWnd.cpp : implementation file
//

#include "stdafx.h"
#include "FOHogaWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFOHogaWnd

CFOHogaWnd::CFOHogaWnd(CWnd *pParent, AccountMap *pAcntMap, RsvdDataArray *pRsvdData)
: CAxisExt(pParent), m_pAccounts(pAcntMap), m_pRsvdData(pRsvdData)
{
	m_bExtOdrColumn = FALSE;
	m_bOneClickOrder = FALSE;
	m_bSpaceOrder = FALSE;
	m_bEnterOrder = FALSE;
	m_bRClickCancel = FALSE;
	m_MouseMode = MM_NONE;
	m_pGrid = std::make_unique<CFOGridCtrl>(m_pParent);
	m_pBGrid = std::make_unique < CGridCtrl>();
	m_pMadoRsvd = std::make_unique <CfxButton>(m_pFont);
	m_pMadoJumn = std::make_unique <CfxButton>(m_pFont);
	m_pMasuRsvd = std::make_unique <CfxButton>(m_pFont);
	m_pMasuJumn = std::make_unique <CfxButton>(m_pFont);
	m_pHogaFix  = std::make_unique <CfxButton>(m_pFont);
	m_pSiseData = NULL;
	m_csArrow = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_csHand  = AfxGetApp()->LoadStandardCursor(IDC_HAND);
}

CFOHogaWnd::~CFOHogaWnd()
{
}

BEGIN_MESSAGE_MAP(CFOHogaWnd, CWnd)
	//{{AFX_MSG_MAP(CFOHogaWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_APP_REFRESH, OnRefresh)
	ON_MESSAGE(WM_APP_SIG,     OnAppSignal)
	ON_BN_CLICKED(IDC_BTN_MADO_JUMN, OnAllMadoCancel)
	ON_BN_CLICKED(IDC_BTN_MASU_JUMN, OnAllMasuCancel)
	ON_BN_CLICKED(IDC_BTN_MADO_RSVD, OnAllMadoRsvdCancel)
	ON_BN_CLICKED(IDC_BTN_MASU_RSVD, OnAllMasuRsvdCancel)
	ON_BN_CLICKED(IDC_BTN_HOGA_FIX, OnHogaFix)
END_MESSAGE_MAP()

int CFOHogaWnd::GetRequireWidth()
{
	return m_pGrid->GetRequireWidth();
}

void CFOHogaWnd::ShowRate(bool bShowRate)
{
	m_pGrid->ShowRate(bShowRate);
}

void CFOHogaWnd::ShowBefore( bool bShowBefore )
{
	m_pGrid->ShowBefore(bShowBefore);
}

void CFOHogaWnd::ShowRsvd( bool bShowRsvd )
{
	m_pGrid->ShowRsvd(bShowRsvd);
}

void CFOHogaWnd::SetBGridData( int dcnt, int dvol, int sdch, int svol, int scnt )
{
	// 총 잔량/건수/차
	char buff[32];

	buff[0] = NULL;
	if (dvol>0) sprintf(buff, "%4d", dvol);
	m_pBGrid->SetItemText(0, m_pGrid->IDX_ASK_SIZE, buff);

	buff[0] = NULL;
	if (dcnt>0) sprintf(buff, "%3d", dcnt);
	m_pBGrid->SetItemText(0, m_pGrid->IDX_ASK_BEFORE, buff);

	buff[0] = NULL;
	if (svol>0) sprintf(buff, "%4d", svol);
	m_pBGrid->SetItemText(0, m_pGrid->IDX_BID_SIZE, buff);

	buff[0] = NULL;
	if (scnt>0) sprintf(buff, "%4d", scnt);
	m_pBGrid->SetItemText(0, m_pGrid->IDX_BID_BEFORE, buff);

	m_pBGrid->RedrawRow(0);	
}

void CFOHogaWnd::SetData(SiseData *psd)
{
	m_pSiseData = psd;
	m_pGrid->SetData(psd);
	SetBottomGrid();
	SetBGridData(psd->t_dcnt, psd->t_dvol, psd->t_sdch, psd->t_svol, psd->t_scnt);
}

void CFOHogaWnd::SetRealData(RealData *rp)
{
	m_pGrid->SetRealData(rp);

	if (rp->find(101) != rp->end())
	{
		SetBGridData(
			Str2Int(rp->operator [](103)),
			Str2Int(rp->operator [](101)),
			Str2Int(rp->operator [](120)),
			Str2Int(rp->operator [](106)),
			Str2Int(rp->operator [](108)) 
		);
	}
}

void CFOHogaWnd::SetRealData(DWORD* data)
{
	m_pGrid->SetRealData(data);

	if (data[101])
	{
		SetBGridData(
			atoi((char*)data[103]),
			atoi((char*)data[101]),
			atoi((char*)data[120]),
			atoi((char*)data[106]),
			atoi((char*)data[108])
		);
	}

}

int CFOHogaWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;	

	SetFont(m_pFont);

	const CRect rc(0, 0, 0, 0);

	m_TitleTip.Create(this);
	
	m_pGrid->Create(rc, this, IDC_GRID_HOGA, WS_CHILD|WS_BORDER|WS_TABSTOP|WS_VISIBLE|WS_CLIPSIBLINGS);
	m_pBGrid->Create(rc, this, IDC_GRID_BOTTOM, WS_CHILD|WS_BORDER|WS_VISIBLE|WS_CLIPCHILDREN);
	SetBottomGrid();
	m_pGrid->SetFont(m_pFont, FALSE);
	m_pBGrid->SetFont(m_pFont, FALSE);

	m_pGrid->SetDoubleBuffering();

	m_pMadoRsvd->Create("", WS_CHILD/*|WS_VISIBLE*/, CRect(0, 0, 0, 0), this, IDC_BTN_MADO_RSVD);
	m_pMadoJumn->Create("", WS_CHILD/*|WS_VISIBLE*/, CRect(0, 0, 0, 0), this, IDC_BTN_MADO_JUMN);
	m_pMasuJumn->Create("", WS_CHILD/*|WS_VISIBLE*/, CRect(0, 0, 0, 0), this, IDC_BTN_MASU_JUMN);
	m_pMasuRsvd->Create("", WS_CHILD/*|WS_VISIBLE*/, CRect(0, 0, 0, 0), this, IDC_BTN_MASU_RSVD);
	m_pHogaFix->Create("", WS_CHILD/*|WS_VISIBLE*/, CRect(0, 0, 0, 0), this, IDC_BTN_HOGA_FIX);

	m_pMadoRsvd->Init(bsGENERAL|bsCOLOR|bsTEXT, "취소", "취소", RGB(0xBC, 0xE6, 0xC5));
	m_pMadoJumn->Init(bsGENERAL|bsCOLOR|bsTEXT, "취소", "취소", m_crMadoBk);
	m_pHogaFix->Init(bsTOGGLE|bsTEXT, "호가고정", "호가고정", m_crBodyBk);
	m_pMasuJumn->Init(bsGENERAL|bsCOLOR|bsTEXT, "취소", "취소", m_crMasuBk);
	m_pMasuRsvd->Init(bsGENERAL|bsCOLOR|bsTEXT, "취소", "취소", RGB(0xBC, 0xE6, 0xC5));
	
	m_HandCursorWnd.insert( m_pMadoRsvd->GetSafeHwnd() );
	m_HandCursorWnd.insert( m_pMadoJumn->GetSafeHwnd() );
	m_HandCursorWnd.insert( m_pMasuJumn->GetSafeHwnd() );
	m_HandCursorWnd.insert( m_pMasuRsvd->GetSafeHwnd() );

	return 0;
}

void CFOHogaWnd::CalcSize()
{
	CRect rc, crc;
	GetClientRect(rc);
	int pos_y=0, bottom_space = GRID_CELL_HEIGHT*2+PADDING_VALUE;

	// spec
	bottom_space = GRID_CELL_HEIGHT + PADDING_VALUE/2;

	pos_y = rc.top;
	m_pGrid->MoveWindow(rc.left, pos_y, rc.Width(), rc.Height()-bottom_space, FALSE);

	pos_y += rc.Height()-bottom_space;
	m_pBGrid->MoveWindow(rc.left, rc.bottom-bottom_space-1, rc.Width(), GRID_CELL_HEIGHT, FALSE);
	SetBottomGrid();

	pos_y += GRID_CELL_HEIGHT;
	m_pBGrid->GetCellRect(0, m_pGrid->IDX_ASK_RSVD, crc);
	m_pMadoRsvd->MoveWindow(crc.left, pos_y, crc.Width()+2, GRID_CELL_HEIGHT, FALSE);
	m_pBGrid->GetCellRect(0, m_pGrid->IDX_ASK_JUMUN, crc);
	m_pMadoJumn->MoveWindow(crc.left, pos_y, crc.Width()+2, GRID_CELL_HEIGHT, FALSE);

	m_pBGrid->GetCellRect(0, m_pGrid->IDX_HOGA, crc);
	m_pHogaFix->MoveWindow(crc.left, pos_y, crc.Width()+2, GRID_CELL_HEIGHT, FALSE);

	m_pBGrid->GetCellRect(0, m_pGrid->IDX_BID_JUMUN, crc);
	m_pMasuJumn->MoveWindow(crc.left, pos_y, crc.Width()+2, GRID_CELL_HEIGHT, FALSE);
	m_pBGrid->GetCellRect(0, m_pGrid->IDX_BID_RSVD, crc);
	m_pMasuRsvd->MoveWindow(crc.left, pos_y, crc.Width()+2, GRID_CELL_HEIGHT, FALSE);

}

void CFOHogaWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	CalcSize();
}

void CFOHogaWnd::OnDestroy() 
{
	m_TitleTip.DestroyWindow();

	//if (m_pGrid) { m_pGrid->DestroyWindow(); delete m_pGrid; }
	//if (m_pBGrid) { m_pBGrid->DestroyWindow(); delete m_pBGrid; }
	//if (m_pMadoRsvd) { m_pMadoRsvd->DestroyWindow(); delete m_pMadoRsvd; }
	//if (m_pMadoJumn) { m_pMadoJumn->DestroyWindow(); delete m_pMadoJumn; }
	//if (m_pMasuRsvd) { m_pMasuRsvd->DestroyWindow(); delete m_pMasuRsvd; }
	//if (m_pMasuJumn) { m_pMasuJumn->DestroyWindow(); delete m_pMasuJumn; }
	//if (m_pHogaFix)  { m_pHogaFix->DestroyWindow(); delete m_pHogaFix; }

	CWnd::OnDestroy();	
}

void CFOHogaWnd::SetBottomGrid()
{
	const int cols = m_pGrid->GetColumnCount();
	const int rows = m_pGrid->GetRowCount();

	m_pBGrid->SetBkColor(m_crBodyBk);
	m_pBGrid->SetNoScrollBar(TRUE);
	m_pBGrid->SetColumnResize(FALSE);
	m_pBGrid->EnableSelection(FALSE);
	m_pBGrid->SetDoubleBuffering(TRUE);
	m_pBGrid->SetEditable(FALSE);
	m_pBGrid->SetFrameFocusCell(FALSE);
	m_pBGrid->SetDefCellWidth(50);
	m_pBGrid->SetDefCellHeight(GRID_CELL_HEIGHT);
	m_pBGrid->SetDefCellMargin(1);
	
	m_pBGrid->SetDefCellHeight( m_pBGrid->GetDefCellHeight() );
	m_pBGrid->SetColumnCount( m_pGrid->GetColumnCount() );

	m_pBGrid->SetRowCount(1);
	
	GV_ITEM item;
	for(int n=0; n<cols; ++n)
	{
		CString val = m_pBGrid->GetItemText(0, n);
		item.row = 0;
		item.col = n;
		item.crFgClr = m_crBlack;
		item.nMargin = 5;
		item.nState = 0;
		item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_BKCLR|GVIF_FGCLR|GVIF_MARGIN|GVIF_STATE;
		item.nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;		
		item.strText = (LPCSTR)val;
		item.lParam = -1;

		if      (n==m_pGrid->IDX_ASK_JUMUN || n==m_pGrid->IDX_ASK_RSVD) item.crBkClr = m_crMadoBk;
		else if (n==m_pGrid->IDX_BID_JUMUN || n==m_pGrid->IDX_BID_RSVD) item.crBkClr = m_crMasuBk;
		else                                          item.crBkClr = CLR_DEFAULT;
		m_pBGrid->SetItem(&item);
		m_pBGrid->SetColumnWidth( n, m_pGrid->GetColumnWidth(n) );
	}

	m_pBGrid->SetItemBkColour(0, m_pGrid->IDX_HOGA, m_crBodyBk);
	m_pBGrid->SetItemFormat(0, m_pGrid->IDX_HOGA, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	m_pBGrid->SetItemText(0, m_pGrid->IDX_HOGA, "▼");
	//m_pBGrid->Invalidate(TRUE);
}

void CFOHogaWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(rc);
	dc.FillSolidRect(rc, m_crBodyBk);
}

LRESULT CFOHogaWnd::OnRefresh( WPARAM wParam, LPARAM lParam )
{
	if (wParam==WP_MICHEG)
	{
		PriceMichegMap tmp;
		PriceMichegMap::iterator pst, ped;
		int tot_dcnt=0, tot_scnt=0;
		int tot_dwqty=0, tot_swqty=0;

		string code = (LPCSTR)lParam;
		AccountMap::iterator ast, aed;
		ast = m_pAccounts->begin();
		aed = m_pAccounts->end();
		for(; ast!=aed; ++ast)
		{
			PriceMichegMap &pmm = ast->second.micheg[code];
			for(pst=pmm.begin(), ped=pmm.end(); pst!=ped; ++pst)
			{
				Micheg &m = tmp[pst->first];
				m.d_cnt += pst->second.d_cnt;
				m.s_cnt += pst->second.s_cnt;
				m.d_wqty += pst->second.d_wqty;
				m.s_wqty += pst->second.s_wqty;

				// total - for bottom grid
				tot_dcnt += pst->second.d_cnt;
				tot_scnt += pst->second.s_cnt;
				tot_dwqty += pst->second.d_wqty;
				tot_swqty += pst->second.s_wqty;
			}
		}
		
		m_pGrid->SetMicheg(&tmp);
		SetBGridMichegData(tot_dcnt, tot_scnt, tot_dwqty, tot_swqty);
	} 
	else if (wParam==WP_RESERVE) 
	{
		AccountMap::iterator ast = m_pAccounts->begin();
		const AccountMap::iterator aed = m_pAccounts->end();
		int oqty=0, ocnt=0;
		const int mult_cnt = m_pAccounts->size();
		int mult_qty = 0;
		int tot_dcnt=0, tot_scnt=0;
		int tot_dwqty=0, tot_swqty=0;

		for(; ast!=aed; ++ast) 
			mult_qty += ast->second.mult;

		ReserveData *prd = (ReserveData*)lParam;
		RsvdDataArray::iterator st, ed;
		st = m_pRsvdData->begin();
		ed = m_pRsvdData->end();
		for(; st!=ed; ++st)
		{
			switch(st->odgb)
			{
			case 1: 
				tot_dcnt += mult_cnt;
				tot_dwqty += st->oqty * mult_qty;
				break;
			case 2:
				tot_scnt += mult_cnt;
				tot_swqty += st->oqty * mult_qty;
				break;
			}

			if (st->cprc==prd->cprc && st->odgb==prd->odgb)
			{
				oqty += st->oqty;
				ocnt += 1;
				AddArrow( CCellID(st->row, st->col), st->ptic );
			}
		}
		CString text;
		if (ocnt>0 && mult_cnt>0 && mult_qty>0)	
		{
			text.Format("%d (%d)", ocnt*mult_cnt, oqty*mult_qty);
		}
		else
		{
			DelArrow(CCellID(prd->row, prd->col), prd->ptic);
			text.Format("");
		}
		m_pGrid->SetItemText(prd->row, prd->col, text);
		m_pGrid->RedrawCell(prd->row, prd->col);
		SetBGridRsvdData(tot_dcnt, tot_scnt, tot_dwqty, tot_swqty);
	}
	return 0;
}

void CFOHogaWnd::OnAllMadoCancel()
{
	GetParent()->SendMessage(WM_APP_SIG, WP_MADO_ALL_CANCEL, 0);
	if (m_pGrid) m_pGrid->SetFocus();
}

void CFOHogaWnd::OnAllMasuCancel()
{
	GetParent()->SendMessage(WM_APP_SIG, WP_MASU_ALL_CANCEL, 0);
	if (m_pGrid) m_pGrid->SetFocus();
}

BOOL CFOHogaWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	
	//if (view->hdr.code==NM_CLICK && view->iRow==0 && view->iColumn==IDX_HOGA)
	if (wParam==IDC_GRID_HOGA || wParam==IDC_GRID_BOTTOM)
	{
		NM_GRIDVIEW *view = (NM_GRIDVIEW*)lParam;
		if (view->hdr.code==NM_CLICK && view->iRow==0 && view->iColumn==m_pGrid->IDX_HOGA)
		{
			switch(wParam)
			{
			case IDC_GRID_HOGA:
				m_pGrid->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEUP, 0), 0);
				break;
			case IDC_GRID_BOTTOM:
				m_pGrid->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEDOWN, 0), 0);
				break;
			}
		}
		if (wParam==IDC_GRID_BOTTOM)
			m_pGrid->SetFocus();
	}
	
	if (wParam==IDC_GRID_HOGA)
	{
		*pResult = 1;

		BOOL bOneClick = FALSE;
		GV_DISPINFO *pi = (GV_DISPINFO*)lParam;

		const CCellID id(pi->item.row, pi->item.col);
		if (pi->hdr.code == GVN_SELCHANGING)
		{
			const int curr = m_pGrid->GetItemData(pi->item.row, m_pGrid->IDX_HOGA);
			if (curr && pi->item.col==m_pGrid->IDX_HOGA)
				GetParent()->PostMessage(WM_APP_SIG, WP_HOGA_CLICK, curr);

			if (IsJumnCell(id))
			{
				m_DragStartId = id;
				m_MouseMode = MM_DRAG_JUMN;
				SetCursor(m_csHand);
			}
			else if (IsRsvdCell(id))
			{
				m_DragStartId = id;
				m_MouseMode = MM_DRAG_RSVD;
				SetCursor(m_csHand);
			}
			else 
			{
				m_MouseMode = MM_NONE;
			}
		}
		else if (pi->hdr.code == GVN_SELCHANGED)
		{
			if (m_MouseMode==MM_DRAG_JUMN)
			{
				if (m_bMouseOut)	// 취소네
				{
					CancelData cd;
					if      (m_DragStartId.col==m_pGrid->IDX_ASK_JUMUN) cd.odgb = 1;
					else if (m_DragStartId.col==m_pGrid->IDX_BID_JUMUN) cd.odgb = 2;
					else                                       cd.odgb = 0;
					if (cd.odgb>0)
					{
						cd.cprc = m_pGrid->GetItemData(m_DragStartId.row, m_pGrid->IDX_HOGA);
						GetParent()->SendMessage(WM_APP_SIG, WP_CANCEL_ORDER, (LPARAM)&cd);
					}
				}
				else if (m_DragStartId.col==id.col && m_DragStartId.row!=id.row && id.row>0)	// 정정이네
				{
					ModifyData md;
					if      (m_DragStartId.col==m_pGrid->IDX_ASK_JUMUN) md.odgb = 1;
					else if (m_DragStartId.col==m_pGrid->IDX_BID_JUMUN) md.odgb = 2;
					else                                       md.odgb = 0;
					if (md.odgb>0)
					{
						md.fprc = m_pGrid->GetItemData(m_DragStartId.row, m_pGrid->IDX_HOGA);
						md.tprc = m_pGrid->GetItemData(id.row, m_pGrid->IDX_HOGA);
						GetParent()->SendMessage(WM_APP_SIG, WP_MODIFY_ORDER, (LPARAM)&md);
					}
				}
			}
			else if (m_MouseMode==MM_DRAG_RSVD)
			{
				if (m_bMouseOut)	// 취소네
				{
					CancelData cd;
					if      (m_DragStartId.col==m_pGrid->IDX_ASK_JUMUN) cd.odgb = 1;
					else if (m_DragStartId.col==m_pGrid->IDX_BID_JUMUN) cd.odgb = 2;
					else                                       cd.odgb = 0;
					if (cd.odgb>0)
					{
						cd.cprc = m_pGrid->GetItemData(m_DragStartId.row, m_pGrid->IDX_HOGA);
						GetParent()->SendMessage(WM_APP_SIG, WP_CANCEL_RSVD, (LPARAM)&cd);
					}
				}
				else if (m_DragStartId.col==id.col && m_DragStartId.row!=id.row && id.row>0)	// 정정이네
				{
					ModifyData md;
					if      (m_DragStartId.col==m_pGrid->IDX_ASK_JUMUN) md.odgb = 1;
					else if (m_DragStartId.col==m_pGrid->IDX_BID_JUMUN) md.odgb = 2;
					else                                       md.odgb = 0;
					if (md.odgb>0)
					{
						md.fprc = m_pGrid->GetItemData(m_DragStartId.row, m_pGrid->IDX_HOGA);
						md.tprc = m_pGrid->GetItemData(id.row, m_pGrid->IDX_HOGA);
						md.rtic = m_DragStartId.row - id.row;
						GetParent()->SendMessage(WM_APP_SIG, WP_MODIFY_RSVD, (LPARAM)&md);
					}
				}
			}

			// 주문 정정/취소가 아니고, 놓고 뗀 위치가 같고, 마우스가 컨트롤 안에 있으면 원클릭주문~
			bOneClick = m_bOneClickOrder && (m_pGrid->GetFocusCell()==m_pGrid->GetLeftClickDownCell()) && (!m_bMouseOut);

			m_MouseMode = MM_NONE;
			SetCursor(m_csArrow);
		}

		NM_GRIDVIEW *view = (NM_GRIDVIEW*)lParam;
	 	if ((view->hdr.code==NM_DBLCLK && !m_bOneClickOrder) || 
			(bOneClick) ||
			(view->hdr.code==NM_CHAR && (UINT)view->hdr.hwndFrom==VK_SPACE && m_bSpaceOrder) ||
			(view->hdr.code==NM_CHAR && (UINT)view->hdr.hwndFrom==VK_RETURN && m_bEnterOrder))
		{
			BOOL bJumn = (view->iColumn==m_pGrid->IDX_ASK_JUMUN || view->iColumn==m_pGrid->IDX_BID_JUMUN);
			const BOOL bRsvd = (view->iColumn==m_pGrid->IDX_ASK_RSVD || view->iColumn==m_pGrid->IDX_BID_RSVD);

			// 2010.10.05 잔량이나 건수에서도 주문내시겠단다.
			if (!bJumn && m_bExtOdrColumn)
			{
				bJumn = view->iColumn==m_pGrid->IDX_ASK_BEFORE || view->iColumn==m_pGrid->IDX_ASK_SIZE ||	
					    view->iColumn==m_pGrid->IDX_BID_BEFORE || view->iColumn==m_pGrid->IDX_BID_SIZE;
			}
			
			if (view->iRow>0 && bJumn)
			{
				Order odr;
				odr.oprc = m_pGrid->GetItemData(view->iRow, m_pGrid->IDX_HOGA);
				if      (view->iColumn==m_pGrid->IDX_ASK_JUMUN || view->iColumn==m_pGrid->IDX_ASK_BEFORE || view->iColumn==m_pGrid->IDX_ASK_SIZE) odr.odgb = 1;
				else if (view->iColumn==m_pGrid->IDX_BID_JUMUN || view->iColumn==m_pGrid->IDX_BID_BEFORE || view->iColumn==m_pGrid->IDX_BID_SIZE) odr.odgb = 2;
				GetParent()->SendMessage(WM_APP_SIG, WP_ORDER, (LPARAM)&odr);
			}
			else if (view->iRow>0 && bRsvd)
			{
				ReserveData rd;
				rd.row = view->iRow;
				rd.col = view->iColumn;
				rd.cprc = m_pGrid->GetItemData(view->iRow, m_pGrid->IDX_HOGA);
				if      (view->iColumn==m_pGrid->IDX_ASK_RSVD) rd.odgb = 1;
				else if (view->iColumn==m_pGrid->IDX_BID_RSVD) rd.odgb = 2;
				GetParent()->SendMessage(WM_APP_SIG, WP_RSVD_ORDER, (LPARAM)&rd);
			}
		}
		else if (view->hdr.code==NM_RCLICK)
		{
			BOOL bAccept = (view->iColumn==m_pGrid->IDX_ASK_JUMUN || view->iColumn==m_pGrid->IDX_BID_JUMUN);

			// 2010.10.05 잔량이나 건수에서도 취소합니다~
			if (!bAccept && m_bExtOdrColumn)
			{
				bAccept = view->iColumn==m_pGrid->IDX_ASK_BEFORE || view->iColumn==m_pGrid->IDX_ASK_SIZE ||	
					    view->iColumn==m_pGrid->IDX_BID_BEFORE || view->iColumn==m_pGrid->IDX_BID_SIZE;
			}

			if (bAccept)
				ShowOrderContextMenu(view->iRow, view->iColumn);
		}
		m_bMouseOut = FALSE;
	}
	return CWnd::OnNotify(wParam, lParam, pResult);
}

LRESULT CFOHogaWnd::OnAppSignal( WPARAM wParam, LPARAM lParam )
{
	CCellID* pid{};

	switch(wParam)	
	{
	case WP_MOUSE_IN:
		m_bMouseOut = FALSE;
		break;
	case WP_MOUSE_OUT:
		m_bMouseOut = TRUE;
		if (m_MouseMode==MM_NONE)
		{
			SetCursor(m_csArrow);
			TRACE("SetCursor IDC_ARROW\n");
		}
		break;
	case WP_CURRCELL_CHANGE:
		 pid = (CCellID *)lParam;
		if (m_MouseMode==MM_DRAG_JUMN && !m_bMouseOut)
		{
			if (m_DragStartId.col!=pid->col)
			{
				SetCursor(m_csHand);
				TRACE("SetCursor IDC_HAND\n");
			}
			else
			{
				SetCursor(m_csHand);
				TRACE("SetCursor IDC_HAND\n");
			}
		}
	}
	return 0;
}

void CFOHogaWnd::ShowOrderInfo( int row, int col )
{
	if (m_pSiseData==NULL) return;

	int line=0;
	CString tmp, text = "\r\n"
		" 주문#       계좌        미체결 \r\n";
	//        1 | 001-20-000035 |        ";

	AccountMap::iterator st = m_pAccounts->begin();
	const AccountMap::iterator ed = m_pAccounts->end();
	for(; st!=ed; ++st)
	{
		LPCSTR acno = st->first.c_str();
		LPCSTR code = m_pSiseData->code;
		const int oprc = m_pGrid->GetItemData(row, m_pGrid->IDX_HOGA);

		OrderMap *pom = &(st->second.order[code]);
		OrderMap::iterator ost = pom->begin();
		const OrderMap::iterator oed = pom->end();
		for(line=0; ost!=oed; ++ost)
		{
			if (oprc==ost->second.oprc)
			{
				tmp.Format(" %5d   %.3s-%.2s-%.6s   %6d \r\n",
					ost->first, acno, acno+3, acno+5, ost->second.wqty);
				text += tmp;
				 ++line;
			}
		}
	}
	if (line==0) return;

	CRect rc;
	m_pGrid->GetCellRect(row, col, &rc);
	rc.right = rc.left + 205;
	rc.bottom = rc.top + (GRID_CELL_HEIGHT*(line+2)) + 10;

	LOGFONT lfont;
	m_pFont->GetLogFont(&lfont);
	m_TitleTip.Show(rc, text, 0, NULL, &lfont);
}

BOOL CFOHogaWnd::IsJumnCell( CCellID id )
{
	if (id.col==m_pGrid->IDX_ASK_JUMUN || id.col==m_pGrid->IDX_BID_JUMUN)
		return (!m_pGrid->GetItemText(id.row, id.col).IsEmpty() || !m_pGrid->GetItemText(id.row, id.col).IsEmpty());
	else
		return FALSE;
}

BOOL CFOHogaWnd::IsRsvdCell( CCellID id )
{
	if (id.col==m_pGrid->IDX_ASK_RSVD || id.col==m_pGrid->IDX_BID_RSVD)
		return (!m_pGrid->GetItemText(id.row, id.col).IsEmpty() || !m_pGrid->GetItemText(id.row, id.col).IsEmpty());
	else
		return FALSE;
}

void CFOHogaWnd::AddArrow( CCellID id, int tick )
{
	CArrow ar;
	CRect crc;

	ar.m_sid = id;

	if (!m_pGrid->GetAbsCellRect(id, crc)) return;

	if (id.col==m_pGrid->IDX_ASK_RSVD)
	{
		ar.m_st.x = crc.right - 5;
		ar.m_st.y = crc.top + GRID_CELL_HEIGHT/2;
		ar.m_ed.x = crc.right + (LONG)(m_pGrid->GetColumnWidth(m_pGrid->IDX_ASK_JUMUN)*0.8);
		ar.m_ed.y = ar.m_st.y - (tick * GRID_CELL_HEIGHT);
		ar.m_eid.row = id.row - tick;
		ar.m_eid.col = id.col + 1;
	}
	else if (id.col==m_pGrid->IDX_BID_RSVD)
	{
		ar.m_st.x = crc.left + 5;
		ar.m_st.y = crc.top + GRID_CELL_HEIGHT/2;
		ar.m_ed.x = crc.left - (LONG)(m_pGrid->GetColumnWidth(m_pGrid->IDX_BID_JUMUN)*0.8);
		ar.m_ed.y = ar.m_st.y - (tick * GRID_CELL_HEIGHT);
		ar.m_eid.row = id.row - tick;
		ar.m_eid.col = id.col - 1;
	}	
	else return;

	m_pGrid->AddArrow(ar);
}

void CFOHogaWnd::DelArrow( CCellID id, int tick )
{
	CArrow ar;
	ar.m_sid = id;
	if (id.col==m_pGrid->IDX_ASK_RSVD)
	{
		ar.m_eid.row = id.row - tick;
		ar.m_eid.col = id.col + 1;
	}
	else if (id.col==m_pGrid->IDX_BID_RSVD)
	{
		ar.m_eid.row = id.row - tick;
		ar.m_eid.col = id.col - 1;
	}	
	else return;

	m_pGrid->DelArrow(ar);

	CRect src, erc, trc;
	m_pGrid->GetCellRect(ar.m_sid, src);
	m_pGrid->GetCellRect(ar.m_eid, erc);
	trc.SetRect( min(src.left, erc.left), min(src.top, erc.top), max(src.right, erc.right), max(src.bottom, erc.bottom) );
	m_pGrid->InvalidateRect(trc, FALSE);
}

void CFOHogaWnd::OnAllMadoRsvdCancel()
{
	GetParent()->SendMessage(WM_APP_SIG, WP_MADO_RSVD_ALL_CANCEL, 0);
	if (m_pGrid) m_pGrid->SetFocus();
}

void CFOHogaWnd::OnAllMasuRsvdCancel()
{
	GetParent()->SendMessage(WM_APP_SIG, WP_MASU_RSVD_ALL_CANCEL, 0);
	if (m_pGrid) m_pGrid->SetFocus();
}

void CFOHogaWnd::ChangeTheme()
{
	m_pGrid->SetGridHeader();
	Invalidate(TRUE);
}

BOOL CFOHogaWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CFOHogaWnd::PreTranslateMessage(MSG* pMsg) 
{
	if (m_HandCursorWnd.find(pMsg->hwnd) != m_HandCursorWnd.end())
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));

	return CWnd::PreTranslateMessage(pMsg);
}

void CFOHogaWnd::SetHogaFix( BOOL bFixed )
{
	m_pGrid->SetHogaFix(bFixed);
	if (m_pGrid) m_pGrid->SetFocus();
}

void CFOHogaWnd::OnHogaFix()
{
	m_pGrid->SetHogaFix( m_pHogaFix->GetToggleStatus() ? TRUE : FALSE );
	if (m_pGrid) m_pGrid->SetFocus();
}

void CFOHogaWnd::SetOneClickMode( BOOL bOneClickOrder )
{
	m_bOneClickOrder = bOneClickOrder;	
}

void CFOHogaWnd::SetBGridMichegData( int t_dcnt, int t_scnt, int t_dwqty, int t_swqty )
{
	CString text;

	if (t_dcnt) text.Format("%d (%d)", t_dcnt, t_dwqty);
	else        text = "";
	if (m_pBGrid->GetItemText(0, m_pGrid->IDX_ASK_JUMUN)!=text)
	{
		m_pBGrid->SetItemText(0, m_pGrid->IDX_ASK_JUMUN, text);
		m_pBGrid->RedrawCell(0, m_pGrid->IDX_ASK_JUMUN);
	}

	if (t_scnt) text.Format("%d (%d)", t_scnt, t_swqty);
	else        text = "";
	if (m_pBGrid->GetItemText(0, m_pGrid->IDX_BID_JUMUN)!=text)
	{
		m_pBGrid->SetItemText(0, m_pGrid->IDX_BID_JUMUN, text);
		m_pBGrid->RedrawCell(0, m_pGrid->IDX_BID_JUMUN);
	}
}

void CFOHogaWnd::SetBGridRsvdData( int t_dcnt, int t_scnt, int t_dwqty, int t_swqty )
{
	CString text;
	
	if (t_dcnt) text.Format("%d (%d)", t_dcnt, t_dwqty);
	else        text = "";
	if (m_pBGrid->GetItemText(0, m_pGrid->IDX_ASK_RSVD)!=text)
	{
		m_pBGrid->SetItemText(0, m_pGrid->IDX_ASK_RSVD, text);
		m_pBGrid->RedrawCell(0, m_pGrid->IDX_ASK_RSVD);
	}
	
	if (t_scnt) text.Format("%d (%d)", t_scnt, t_swqty);
	else        text = "";
	if (m_pBGrid->GetItemText(0, m_pGrid->IDX_BID_RSVD)!=text)
	{
		m_pBGrid->SetItemText(0, m_pGrid->IDX_BID_RSVD, text);
		m_pBGrid->RedrawCell(0, m_pGrid->IDX_BID_RSVD);
	}
}

void CFOHogaWnd::SetExtOrderColumn( BOOL bExtend /*= FALSE*/ )
{
	m_bExtOdrColumn = bExtend;
}

void CFOHogaWnd::SetSpaceOrder( BOOL bEnable /*= FALSE*/ )
{
	m_bSpaceOrder = bEnable;
}

void CFOHogaWnd::SetEnterOrder(BOOL bEnable /*= FALSE*/)
{
	m_bEnterOrder = bEnable;
}

void CFOHogaWnd::SetRClickCancel(BOOL bEnable)
{
	m_bRClickCancel = bEnable;
}

void CFOHogaWnd::ShowOrderContextMenu( int nRow, int nCol )
{
	const int ocnt=0, oprc = m_pGrid->GetItemData(nRow, m_pGrid->IDX_HOGA);
	vector<Order*> odr_list;

	if (m_pAccounts->empty()) return;
	AccountMap::iterator st = m_pAccounts->begin();
	const AccountMap::iterator ed = m_pAccounts->end();
	for(; st!=ed; ++st)
	{
		LPCSTR acno = st->first.c_str();
		LPCSTR code = m_pSiseData->code;
		
		OrderMap *pom = &(st->second.order[code]);
		if (pom->empty()) continue;

		OrderMap::iterator ost = pom->begin();
		const OrderMap::iterator oed = pom->end();
		for(; ost!=oed; ++ost)
		{
			if (oprc!=ost->second.oprc) continue;
			
			if (m_pGrid->IDX_ASK_JUMUN < m_pGrid->IDX_BID_JUMUN)
			{
				if ( (nCol<m_pGrid->IDX_HOGA && ost->second.odgb==1) || (nCol>m_pGrid->IDX_RATE && ost->second.odgb==2) )
					odr_list.push_back(&(ost->second));	
			}
			else
			{
				if ( (nCol>m_pGrid->IDX_HOGA && ost->second.odgb==1) || (nCol<m_pGrid->IDX_RATE && ost->second.odgb==2) )
					odr_list.push_back(&(ost->second));	
			}
		}
	}

	if (odr_list.empty()) return;

	CPoint pt;
	CMenu menu;
	CString pmsg;
	vector<Order*>::iterator ost = odr_list.begin();
	const vector<Order*>::iterator oed = odr_list.end();
	int jqty=0, jcnt=0, nRet=0;

	if (!m_bRClickCancel)
	{
		for(; ost!=oed; ++ost)
		{
			jcnt += 1;
			jqty += (*ost)->wqty;
		}
		pmsg.Format("[취소] - 가격(%3d.%02d)   건수(%3d)   수량(%3d)",
			odr_list.front()->oprc/100, odr_list.front()->oprc%100, jcnt, jqty);

		menu.CreatePopupMenu();
		menu.AppendMenu(MF_STRING, 0xFFFF, pmsg);

		GetCursorPos(&pt);
		nRet = menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RETURNCMD, pt.x, pt.y, this);
	}

	if (nRet==0xFFFF || m_bRClickCancel)
	{
		CancelData cd;

		if (m_pGrid->IDX_ASK_JUMUN < m_pGrid->IDX_BID_JUMUN)		// 일반
		{
			if      (nCol<m_pGrid->IDX_HOGA) cd.odgb = 1;
			else if (nCol>m_pGrid->IDX_RATE) cd.odgb = 2;
			else    return;
		}
		else													// Reverse
		{
			if      (nCol<m_pGrid->IDX_HOGA) cd.odgb = 2;
			else if (nCol>m_pGrid->IDX_RATE) cd.odgb = 1;
			else    return;
		}
		
		cd.cprc = oprc;
		GetParent()->SendMessage(WM_APP_SIG, WP_CANCEL_ORDER, (LPARAM)&cd);
	}
}

void CFOHogaWnd::SetFocusGrid()
{
	if (m_pGrid)
		if (m_pGrid->GetSafeHwnd())
			m_pGrid->SetFocus();
}

void CFOHogaWnd::SetWidths( int rsvd, int jumun, int cnt, int vol, int hoga, int rate )
{
	m_pGrid->SetWidths(rsvd, jumun, cnt, vol, hoga, rate);
}

void CFOHogaWnd::GetWidths( int *rsvd, int *jumun, int *cnt, int *vol, int *hoga, int *rate )
{
	m_pGrid->GetWidths(rsvd, jumun, cnt, vol, hoga, rate);
}

void CFOHogaWnd::SetReverseMode( BOOL bReverse /*= FALSE*/ )
{
	m_pGrid->SetReverseMode(bReverse);
}