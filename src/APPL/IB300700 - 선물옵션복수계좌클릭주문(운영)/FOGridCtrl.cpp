// FOGridCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "IB300700.h"
#include "FOGridCtrl.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//int head_width[] = { 60, 60, 35, 45, 55, 40, 45, 35, 60, 60 };
//int head_width[] = { 60, 62, 40, 45, 55, 40, 45, 40, 62, 60 };
//char *head_text[] = { "예약", "매도주문", "건수", "잔량", "호가", "등락%", "잔량", "건수", "매수주문", "예약", NULL};

int   DEF_HEAD_WIDTH[] = { 60, 62, 40, 45, 55, 40, 45, 40, 62, 60 };
char* DEF_HEAD_TEXT[]  = { "예약", "매도주문", "건수", "잔량", "호가", "등락%", "잔량", "건수", "매수주문", "예약", NULL};

bool operator<(const CCellID &lsh, const CCellID &rsh)
{
	return memcmp(&lsh, &rsh, sizeof(CCellID))<0;
}

bool operator<(CCellID &lsh, CCellID &rsh)
{
	return memcmp(&lsh, &rsh, sizeof(CCellID))<0;
}

CFOGridCtrl::CFOGridCtrl(CWnd *pParent)
:	CGridCtrlExt(), CAxisExt(pParent)
{
	for(int n=0; n<COLUMN_SIZE; ++n)
	{
		m_HeadText[n] = DEF_HEAD_TEXT[n];
		m_HeadWidth[n] = DEF_HEAD_WIDTH[n];
	}

	m_pdir = 0;
	m_pid = CCellID(-1, -1);

	m_bExpandHoga = false;
	m_bShowRate = m_bShowBefore = m_bShowRsvd = true;
	m_bOutMouse = TRUE;
	m_bHogaFix = FALSE;

	m_crNormalBk = RGB(0xFF, 0xFF, 0xFF);
	m_crYellowBk = RGB(0xFF, 0xFF, 0xB4);

	m_pSiseData = NULL;

	m_pFontB->GetLogFont(&m_BLogFont);

	ZeroMemory(m_MdgaCell, sizeof(m_MdgaCell));
	ZeroMemory(m_MsgaCell, sizeof(m_MsgaCell));
}

CFOGridCtrl::~CFOGridCtrl()
{
}

BOOL CFOGridCtrl::Create(const RECT& rect, CWnd* parent, UINT nID, DWORD dwStyle , DWORD dwExStyle)
{
	//SetNoScrollBar(TRUE);
	BOOL bRet = CGridCtrl::Create(rect, parent, nID, dwStyle, dwExStyle);
	
	EnableTitleTips(FALSE);
	SetColumnResize(FALSE);
	EnableSelection(FALSE);
	SetDoubleBuffering(TRUE);
	SetEditable(FALSE);
	SetFrameFocusCell(FALSE);
	SetDefCellWidth(50);
	SetDefCellHeight(GRID_CELL_HEIGHT);
	SetDefCellMargin(1);

	SetColumnCount(COLUMN_SIZE);
	SetRowCount(DEF_ROW_SIZE);
	SetFixedColumnCount(0);
	SetFixedRowCount(1);
	SetGridHeader();
	SetGridBody();

	return bRet;
}


BEGIN_MESSAGE_MAP(CFOGridCtrl, CGridCtrlExt)
	//{{AFX_MSG_MAP(CFOGridCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CFOGridCtrl::SetGridHeader()
{
	if (!GetSafeHwnd()) return;

	CGridCellBase *phead = GetDefaultCell(TRUE, FALSE);
	phead->SetBackClr(GetIndexColor(COLOR_GRIDHEAD));
	phead->SetTextClr(GetIndexColor(COLOR_GRIDHEADTXT));
	phead->SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);

	for(int n=0; m_HeadText[n]!=NULL; ++n)
	{
		if (!m_bShowRate && n==IDX_RATE)
			SetColumnWidth(n, 0);
		else if (!m_bShowBefore && (n==IDX_BID_BEFORE || n==IDX_ASK_BEFORE))
			SetColumnWidth(n, 0);
		else if (!m_bShowRsvd && (n==IDX_BID_RSVD || n==IDX_ASK_RSVD))
			SetColumnWidth(n, 0);
		else if (m_bExpandHoga && (n==IDX_HOGA || n==IDX_BID_SIZE || n==IDX_ASK_SIZE))
			SetColumnWidth(n, m_HeadWidth[n]+EXPAND_HOGA_WIDTH);
		else
			SetColumnWidth(n, m_HeadWidth[n]);
		CGridCell *pCell = (CGridCell*)(GetCell(0, n));
		if (pCell)
		{
			GV_ITEM item;
			item.row = 0;
			item.col = n;
			item.mask = GVIF_TEXT|GVIF_FORMAT;
			item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
			item.strText = m_HeadText[n];
			SetItem(&item);
		}
	}	
}

void CFOGridCtrl::SetGridBody()
{
	GV_ITEM item;

	// 잔량/건수
	item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_MARGIN;
	item.nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
	item.strText = "";
	item.nMargin = 5;
	for(item.row=1; item.row<m_nRows; ++item.row)
	{
		item.col = IDX_ASK_BEFORE; SetItem(&item);
		item.col = IDX_ASK_SIZE;   SetItem(&item);
		item.col = IDX_BID_BEFORE; SetItem(&item);
		item.col = IDX_BID_SIZE;   SetItem(&item);
	}

	// 예약/주문
	item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_MARGIN|GVIF_BKCLR|GVIF_PARAM;
	item.strText = "";
	for(item.row=1; item.row<m_nRows; ++item.row)
	{
		item.nMargin = 5;	// 화살표 막대기때문에 약간의 공백이 더 필요하다.
		item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
		item.crBkClr = m_crMadoRsvdBk;
		item.col = IDX_ASK_RSVD;  SetItem(&item);

		item.nMargin = 5;
		item.crBkClr = m_crMadoBk;
		item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
		item.col = IDX_ASK_JUMUN; SetItem(&item);

		item.nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
		item.crBkClr = m_crMasuRsvdBk;
		item.col = IDX_BID_RSVD;  SetItem(&item);
		item.crBkClr = m_crMasuBk;
		item.col = IDX_BID_JUMUN; SetItem(&item);
	}

	// 호가/등락
	item.crFgClr = m_crNormalBk;
	item.crBkClr = CLR_DEFAULT;
	item.nMargin = 5;
	item.nState = 0;
	item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_BKCLR|GVIF_FGCLR|GVIF_MARGIN|GVIF_STATE;
	item.nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;		
	item.strText = "";
	for(item.row=1; item.row<m_nRows; ++item.row)
	{
		item.col = IDX_HOGA; SetItem(&item);
		item.col = IDX_RATE; SetItem(&item);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFOGridCtrl message handlers

int CFOGridCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CGridCtrlExt::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CFOGridCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CGridCtrlExt::OnSize(nType, cx, cy);
	EnableScrollBars(SB_VERT, TRUE);
	EnableScrollBars(SB_HORZ, FALSE);
	GetClientRect(m_rcClient);
}

int CFOGridCtrl::GetRequireWidth()
{
	int width = SCROLLBAR_WIDTH;
	for(int n=0; n<COLUMN_SIZE; ++n)
		width += m_HeadWidth[n];

	if (!m_bShowBefore)
		width -= (m_HeadWidth[IDX_ASK_BEFORE] + m_HeadWidth[IDX_BID_BEFORE] );
	if (!m_bShowRate)
		width -= m_HeadWidth[IDX_RATE];
	if (!m_bShowRsvd)
		width -= (m_HeadWidth[IDX_ASK_RSVD] + m_HeadWidth[IDX_BID_RSVD] );
	if (m_bExpandHoga)
		width += EXPAND_HOGA_WIDTH*3;	// 호가, 매도/매수 잔량
	return width;
}

void CFOGridCtrl::ShowRate( bool bShowRate )
{
	if (m_bShowRate == bShowRate) return;

	m_bShowRate = bShowRate;
	SetColumnWidth(IDX_RATE, (m_bShowRate) ? m_HeadWidth[IDX_RATE] : 0);
	CheckExpand();
	Invalidate(TRUE);
	GetParent()->SendMessage(WM_SIZE, 0, 0);
}

void CFOGridCtrl::ShowBefore( bool bShowBefore )
{
	if (m_bShowBefore == bShowBefore) return;
	
	m_bShowBefore = bShowBefore;
	SetColumnWidth(IDX_BID_BEFORE, (m_bShowBefore) ? m_HeadWidth[IDX_BID_BEFORE] : 0);
	SetColumnWidth(IDX_ASK_BEFORE, (m_bShowBefore) ? m_HeadWidth[IDX_ASK_BEFORE] : 0);
	CheckExpand();
	Invalidate(TRUE);
	GetParent()->SendMessage(WM_SIZE, 0, 0);
}

void CFOGridCtrl::ShowRsvd( bool bShowRsvd )
{
	if (m_bShowRsvd == bShowRsvd) return;
	
	m_bShowRsvd = bShowRsvd;
	SetColumnWidth(IDX_BID_RSVD, (m_bShowRsvd) ? m_HeadWidth[IDX_BID_RSVD] : 0);
	SetColumnWidth(IDX_ASK_RSVD, (m_bShowRsvd) ? m_HeadWidth[IDX_ASK_RSVD] : 0);
	CheckExpand();
	Invalidate(TRUE);
	GetParent()->SendMessage(WM_SIZE, 0, 0);
}

void CFOGridCtrl::SetData(SiseData *psd)
{
	m_pSiseData = psd;

	int n, row;
	GV_ITEM item;
	COLORREF fgCol, bgCol;
	int stat, hcnt = psd->fhoga.size();
	CString strCurr;

	SetRowCount(hcnt+1);
	Clear();
	
	m_mapHogaRow.clear();

	// 호가/등락율 Column 세팅
	vector<int>::iterator st = psd->fhoga.begin();
	vector<int>::iterator ed = psd->fhoga.end();
	for(row=1; st!=ed; ++st, ++row)
	{
		if      (*st>psd->gjga) fgCol = GetIndexColor(94);
		else if (*st<psd->gjga) fgCol = GetIndexColor(95);
		else               fgCol = RGB(0, 0, 0);

		if (*st>=psd->jega && *st<=psd->koga) bgCol = m_crYellowBk;
		else                                  bgCol = CLR_DEFAULT/*RGB(0xFF, 0xFF, 0xFF)*/;

		if (*st!=psd->curr) stat = 0;
		else {
			stat = GVIS_SELECTED;
			m_CurCell.col = IDX_HOGA;
			m_CurCell.row = row;
		}

		if (m_pSiseData->code.GetAt(0)=='4' || m_pSiseData->code.GetAt(0) == 'D')  //파생상품 코드개편
		{
			if (*st>0)
				strCurr = "+" + Int2Str(*st, 2);
			else
				strCurr = Int2Str(*st, 2);
			
			if(*st == psd->jega)
			{
				strCurr = "L" + strCurr;
				
				m_nJegaRow = row;
			}
			else if(*st == psd->koga)
			{
				strCurr = "H" + strCurr;
				
				m_nKogaRow = row;
			}
		}
		else
		{
			strCurr = Int2Str(*st, 2);
			
			if(*st == psd->jega)
			{
				strCurr = "L" + strCurr;
				
				m_nJegaRow = row;
			}
			else if(*st == psd->koga)
			{
				strCurr = "H" + strCurr;
				
				m_nKogaRow = row;
			}
		}

		SetItemFgColour(row, IDX_HOGA, fgCol);
		SetItemBkColour(row, IDX_HOGA, bgCol);
		SetItemState(row, IDX_HOGA, stat);
		SetItemText(row, IDX_HOGA, strCurr);
		SetItemFont(row, IDX_HOGA, &m_BLogFont);
		SetItemData(row, IDX_HOGA, *st);

		SetItemFgColour(row, IDX_RATE, fgCol);

		double rate = (*st-psd->gjga) / (double)psd->gjga;
		if (rate>0) rate += DOUBLE_PREFIX;
		else if (rate<0) rate -= DOUBLE_PREFIX;
		rate *= 10000.0;
		SetItemText(row, IDX_RATE, Int2Str((int)rate) );

		m_mapHogaRow[*st] = row;
	}

	// 매도/매수 호가잔량/건수 세팅
	item.nMargin = 5;
	item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_MARGIN;
	item.nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
	for(n=0; n<MAX_FHOGA; ++n)
	{
		// 매도
		row = m_mapHogaRow[ psd->hoga[n].mdga ];
		if (row)
		{
			item.row = row;
			item.col = IDX_ASK_SIZE;
			item.strText = (psd->hoga[n].dvol>0) ? Int2CommaStr( psd->hoga[n].dvol ) : "";
			SetItem(&item);
			item.col = IDX_ASK_BEFORE;
			item.strText = (psd->hoga[n].dcnt>0) ? Int2CommaStr( psd->hoga[n].dcnt ) : "";
			SetItem(&item);
			m_MdgaCell[n] = row;
		}

		// 매수 
		row = m_mapHogaRow[ psd->hoga[n].msga ];
		if (row)
		{
			item.row = row;
			item.col = IDX_BID_SIZE;
			item.strText = (psd->hoga[n].svol>0) ? Int2CommaStr( psd->hoga[n].svol ) : "";
			SetItem(&item);
			item.col = IDX_BID_BEFORE;
			item.strText = (psd->hoga[n].scnt>0) ? Int2CommaStr( psd->hoga[n].scnt ) : "";
			SetItem(&item);
			m_MsgaCell[n] = row;
		}
	}

	SetCurrToCenter();
}


void CFOGridCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	CGridCtrlExt::OnMouseMove(nFlags, point);

	BOOL bOut = (m_rcClient.top>point.y || m_rcClient.bottom<point.y || m_rcClient.left>point.x || m_rcClient.right<point.x);	
	//TRACE("Mouse is %s\n", bOut ? "Out" : "In");

	CCellID id = GetCellFromPt(point);

	//if (id.row>=m_nRows || id.col>=m_nCols) return;

// 	vector<CArrow>::iterator st = m_Arrows.begin();
// 	vector<CArrow>::iterator ed = m_Arrows.end();
// 	for(;st!=ed; ++st)
// 	{
// 		// 		s.Format("4.m_st x : [%d]	m_st y : [%d]	m_sid row : [%d]\n",st->m_st.x,st->m_st.y,st->m_sid.row);
// 		// 		OutputDebugString(s);
// 		if(st->m_sid.row == id.row && st->m_sid.col == id.col)
// 		{
// 			CRect rc;
// 			GetCellRect(st->m_sid,rc);
// 
// 			m_pTooltip->AddTool(this,"사각형 표시는 시장가 주문을 뜻합니다.",rc,GetDlgCtrlID());
// 			m_pTooltip->Activate(TRUE);
// 		}
// 	}

	if (m_bOutMouse==FALSE && bOut==TRUE)
		GetParent()->SendMessage(WM_APP_SIG, WP_MOUSE_OUT, 0);
	else if (m_bOutMouse==TRUE && bOut==FALSE)
		GetParent()->SendMessage(WM_APP_SIG, WP_MOUSE_IN, 0);
	
	if (m_pid!=id)
		GetParent()->SendMessage(WM_APP_SIG, WP_CURRCELL_CHANGE, (LPARAM)&id);

	m_bOutMouse = bOut;

	int dir = IDX_HOGA	- id.col;
	BOOL bChange = ((m_pdir>0 && dir<0) || (m_pdir<0 && dir>0));
	if ( ((m_pid.row!=id.row)||bChange) &&  (id.row>0) )
	{
		if (m_pid.row>0)
		{
			SetItemBkColour(m_pid.row, IDX_ASK_BEFORE);
			SetItemBkColour(m_pid.row, IDX_ASK_SIZE);
			SetItemBkColour(m_pid.row, IDX_BID_BEFORE);
			SetItemBkColour(m_pid.row, IDX_BID_SIZE);
			SetItemState(m_pid.row, IDX_HOGA, GetItemState(m_pid.row, IDX_HOGA) & ~GVIS_FOCUSED );
			RedrawCell(m_pid.row, IDX_ASK_BEFORE);
			RedrawCell(m_pid.row, IDX_ASK_SIZE);
			RedrawCell(m_pid.row, IDX_BID_BEFORE);
			RedrawCell(m_pid.row, IDX_BID_SIZE);
		}

		SetItemState(id.row, IDX_HOGA, GetItemState(id.row, IDX_HOGA)|GVIS_FOCUSED );
		if (id.col<IDX_HOGA)		// 매도
		{	
			SetItemBkColour(id.row, IDX_ASK_BEFORE, m_crMadoBk);
			SetItemBkColour(id.row, IDX_ASK_SIZE  , m_crMadoBk);
			RedrawCell(id.row, IDX_ASK_BEFORE);
			RedrawCell(id.row, IDX_ASK_SIZE);
		}
		else if (id.col>IDX_HOGA)	// 매수
		{
			SetItemBkColour(id.row, IDX_BID_BEFORE, m_crMasuBk);
			SetItemBkColour(id.row, IDX_BID_SIZE  , m_crMasuBk);		
			RedrawCell(id.row, IDX_BID_BEFORE);
			RedrawCell(id.row, IDX_BID_SIZE);
		}

		m_pid = id;
		m_pdir = dir;
	}
}

void CFOGridCtrl::SetCurrToCenter()
{
	if (m_CurCell.row)
	{
		CRect rc;
		GetClientRect(&rc);
		int pos = (m_CurCell.row*GetDefCellHeight()) - rc.Height()/2;

//			CString msg;
//  		SCROLLINFO info;
//  		GetScrollInfo(SB_VERT, &info, SIF_ALL);
//  		msg.Format("SetCurrToCenter min(%d) max(%d) pos(%d)\n", info.nMin, info.nMax, info.nPos);
//  		OutputDebugString(msg);

		SetScrollPos32(SB_VERT, pos, TRUE);
		SendMessage(WM_VSCROLL, 0, 0);
	}
}


void CFOGridCtrl::SetMdgaToCenter()
{
	if (m_MdgaCell[0]==0) 
	{
		SetCurrToCenter();
	}
	else 
	{
		int pos = ((m_MdgaCell[0]+1)*GRID_CELL_HEIGHT) - m_rcClient.Height()/2;

//  		CString msg;
//  		SCROLLINFO info;
//  		GetScrollInfo(SB_VERT, &info, SIF_ALL);
//  		msg.Format("SetMdgaToCenter min(%d) max(%d) pos(%d) mdga[%d]\n", info.nMin, info.nMax, info.nPos, m_MdgaCell[0]);
//  		OutputDebugString(msg);

		SetScrollPos32(SB_VERT, pos, TRUE);
		SendMessage(WM_VSCROLL, 0, 0);
	}
}

void CFOGridCtrl::SetRealData( RealData *rp )
{	
	RealData::iterator p, ed = rp->end();
	if (!m_pSiseData) return;
	if (m_pSiseData->code.IsEmpty()) return;

	BOOL bCurrChange = FALSE;
	// 현재가 변경
	if ( (p=rp->find(23)) != ed )
	{
		bCurrChange = TRUE;
		int curr = (int)(fabs(Str2Double(p->second)*100.0) + DOUBLE_PREFIX);
		int row = m_mapHogaRow[curr];
		if (row>0 && row!=m_CurCell.row)
		{
			if (abs(row-m_CurCell.row)>1)
			{
				CDC *pdc = GetDC();
				if (row>m_CurCell.row)
				{
					for(int jj=m_CurCell.row; jj<row; ++jj)
					{
						SetItemBkColour( jj, IDX_HOGA, m_crYellowBk);
						RedrawCell(jj, IDX_HOGA, pdc);
					}
				}
				else
				{
					for(int jj=m_CurCell.row; jj<row; ++jj)
					{
						SetItemBkColour( jj, IDX_HOGA, m_crYellowBk);
						RedrawCell(jj, IDX_HOGA, pdc);
					}
				}
				ReleaseDC(pdc);
			}

			SetItemState( m_CurCell.row, IDX_HOGA, GetItemState(m_CurCell.row, IDX_HOGA) & ~GVIS_SELECTED );
			SetItemState( row, IDX_HOGA, GetItemState(row, IDX_HOGA) | GVIS_SELECTED );
			SetItemBkColour( row, IDX_HOGA, m_crYellowBk);
			RedrawCell(m_CurCell);
			m_CurCell.row = row;
			RedrawCell(m_CurCell);
		}
	}

	// 호가 변경
	if ( (p=rp->find(40)) != ed )
	{
		int n, row, hoga;
		set<int> setMado, setMasu;
		for(n=0; n<MAX_FHOGA; ++n)
		{
			setMado.insert(m_MdgaCell[n]);
			setMasu.insert(m_MsgaCell[n]);
		}
		setMado.erase(0); setMasu.erase(0);

		if (m_bHogaFix)
		{
			int val = (int)(fabs(Str2Double(rp->find(51)->second)*100.0) + DOUBLE_PREFIX);
			map<int,int>::iterator pos = m_mapHogaRow.find(val);
			if (pos!=m_mapHogaRow.end())
			{
				if (Str2Int(rp->find(41)->second)>0)	// 0이라면 아무래도 장전~
				{
					if (m_MdgaCell[0] != pos->second)
					{
						m_MdgaCell[0] = pos->second;
						SetMdgaToCenter();
					}
				}
				else if (bCurrChange)
				{
					SetCurrToCenter();
				}
			}
			else if (bCurrChange)
			{
				SetCurrToCenter();
			}
		}

		map<int,int>::iterator pos;
		for(n=0; n<MAX_FHOGA; ++n)
		{
			if (m_pSiseData->code.GetAt(0)!='4' && m_pSiseData->code.GetAt(0) != 'D')  //파생상품 코드개편
			{
				hoga = (int)(fabs(Str2Double(rp->find(51+n)->second)*100.0) + DOUBLE_PREFIX);
			}
			else
			{
				CString strHoga = rp->find(51+n)->second.ToCStr();
				strHoga.Replace("++", "+");
				strHoga.Replace("-+", "+");
				strHoga.Replace("0+", "+");
				strHoga.Replace("--", "-");
				strHoga.Replace("+-", "-");
				strHoga.Replace("0-", "-");
				hoga = (int)((Str2Double(strHoga)*100.0) + DOUBLE_PREFIX);
			}
			
			pos = m_mapHogaRow.find(hoga);
			if (pos!=m_mapHogaRow.end())
			{
				if (pos->second>0)
				{
					row = pos->second;
					int dvol = Str2Int(rp->find( 41+n)->second);
					int dcnt = Str2Int(rp->find(211+n)->second);
					SetItemText(row, IDX_ASK_SIZE  , (dvol) ? Int2CommaStr(dvol) : "");	// 매도잔량
					SetItemText(row, IDX_ASK_BEFORE, (dcnt) ? Int2CommaStr(dcnt) : "");	// 매도건수
					RedrawCell(row, IDX_ASK_SIZE); RedrawCell(row, IDX_ASK_BEFORE);
					m_MdgaCell[n] = row;
					setMado.erase(row);
				}
			}

			if (m_pSiseData->code.GetAt(0)!='4' && m_pSiseData->code.GetAt(0) != 'D')  //파생상품 코드개편
			{
				hoga = (int)(fabs(Str2Double(rp->find(71+n)->second)*100.0) + DOUBLE_PREFIX);
			}
			else
			{
				CString strHoga = rp->find(71+n)->second.ToCStr();
				strHoga.Replace("++", "+");
				strHoga.Replace("-+", "+");
				strHoga.Replace("0+", "+");
				strHoga.Replace("--", "-");
				strHoga.Replace("+-", "-");
				strHoga.Replace("0-", "-");
				hoga = (int)((Str2Double(strHoga)*100.0) + DOUBLE_PREFIX);
			}
			
			pos = m_mapHogaRow.find(hoga);
			if (pos!=m_mapHogaRow.end())
			{
				if (pos->second>0)
				{
					row = pos->second;
					int svol = Str2Int(rp->find( 61+n)->second);
					int scnt = Str2Int(rp->find(221+n)->second);
					SetItemText(row, IDX_BID_SIZE  , (svol) ? Int2CommaStr(svol) : "");	// 매수잔량
					SetItemText(row, IDX_BID_BEFORE, (scnt) ? Int2CommaStr(scnt) : "");	// 매수건수
					RedrawCell(row, IDX_BID_SIZE); RedrawCell(row, IDX_BID_BEFORE);
					m_MsgaCell[n] = row;
					setMasu.erase(row);
				}
			}
		}
	
		set<int>::iterator st, ed;
		for(st=setMado.begin(), ed=setMado.end(); st!=ed; ++st)
		{
			SetItemText(*st, IDX_ASK_SIZE, "");
			SetItemText(*st, IDX_ASK_BEFORE, "");
			RedrawCell(*st, IDX_ASK_SIZE);
			RedrawCell(*st, IDX_ASK_BEFORE);
		}
		for(st=setMasu.begin(), ed=setMasu.end(); st!=ed; ++st)
		{
			SetItemText(*st, IDX_BID_SIZE, "");
			SetItemText(*st, IDX_BID_BEFORE, "");
			RedrawCell(*st, IDX_BID_SIZE);
			RedrawCell(*st, IDX_BID_BEFORE);
		}
	}
}

void CFOGridCtrl::SetRealData( DWORD* data )
{   
    if (!m_pSiseData) return;

    if (m_pSiseData->code.IsEmpty()) return;

    BOOL bCurrChange = FALSE;

    CString strVal;

    CString s;

    // 현재가 변경
    if ( data[23] )
    {
        bCurrChange = TRUE;

        strVal = (char*)data[23];
		strVal.Replace("++", "+");
		strVal.Replace("-+", "+");
		strVal.Replace("0+", "+");
		strVal.Replace("--", "-");
		strVal.Replace("+-", "-");
		strVal.Replace("0-", "-");

        int curr;

		if(m_pSiseData->code.GetAt(0)!='4' && m_pSiseData->code.GetAt(0) != 'D')  //파생상품 코드개편
        {
            curr = (int)(fabs(atof(strVal)*100.0) + DOUBLE_PREFIX);
        }
        else
        {
            if(atof(strVal)*100.0 >= 0)
                curr = (int)(fabs(atof(strVal)*100.0) + DOUBLE_PREFIX);
            else
                curr = (int)((atof(strVal)*100.0) - DOUBLE_PREFIX);
        }

        int row = m_mapHogaRow[curr];

        if (row>0 && row!=m_CurCell.row)
        {
            if (abs(row-m_CurCell.row)>1)
            {
                CDC *pdc = GetDC();

                if (row>m_CurCell.row)
                {
                    for(int jj=m_CurCell.row; jj<row; ++jj)
                    {
                        SetItemBkColour( jj, IDX_HOGA, m_crYellowBk);
                        RedrawCell(jj, IDX_HOGA, pdc);
                    }
                }
                else
                {
                    for(int jj=m_CurCell.row; jj<row; ++jj)
                    {
                        SetItemBkColour( jj, IDX_HOGA, m_crYellowBk);
                        RedrawCell(jj, IDX_HOGA, pdc);
                    }
                }
                ReleaseDC(pdc);
            }
 
            SetItemState( m_CurCell.row, IDX_HOGA, GetItemState(m_CurCell.row, IDX_HOGA) & ~GVIS_SELECTED );
            SetItemState( row, IDX_HOGA, GetItemState(row, IDX_HOGA) | GVIS_SELECTED );
            SetItemBkColour( row, IDX_HOGA, m_crYellowBk);

			CString strCurr;
			CString s;
			if(m_nJegaRow < row)
			{
				strCurr = GetItemText(m_nJegaRow,IDX_HOGA);
				
				strCurr.Remove('L');
				
				strCurr.TrimLeft();
				strCurr.TrimRight();
				
				SetItemText(m_nJegaRow, IDX_HOGA, strCurr);
				
				strCurr = GetItemText(row,IDX_HOGA);
				
				strCurr = "L" + strCurr;
				
				SetItemText(row, IDX_HOGA, strCurr);
				
				m_nJegaRow = row;
			}
			else if(m_nKogaRow > row)
			{
				strCurr = GetItemText(m_nKogaRow,IDX_HOGA);
				
				strCurr.Remove('H');
				
				strCurr.TrimLeft();
				strCurr.TrimRight();
				
				SetItemText(m_nKogaRow, IDX_HOGA, strCurr);
				
				strCurr = GetItemText(row,IDX_HOGA);
				
				strCurr = "H" + strCurr;
				
				SetItemText(row, IDX_HOGA, strCurr);
				
				m_nKogaRow = row;
			}

            RedrawCell(m_CurCell);
            m_CurCell.row = row;
            RedrawCell(m_CurCell);
        }
 
//      s.Format("3007 REAL DATA CURR [%d]\n",curr);
//      OutputDebugString(s);

    }

    // 호가 변경
    if ( data[40] )
    {
        int n, row, hoga;

        set<int> setMado, setMasu;

        for(n=0; n<MAX_FHOGA; ++n)
        {
            setMado.insert(m_MdgaCell[n]);
            setMasu.insert(m_MsgaCell[n]);
        }
        setMado.erase(0); setMasu.erase(0);

        if (m_bHogaFix)
        {
            strVal = (char*)data[51];

            int val;

			if (m_pSiseData->code.GetAt(0)!='4' && m_pSiseData->code.GetAt(0) != 'D')  //파생상품 코드개편
            {
                val = (int)(fabs(atof(strVal)*100.0) + DOUBLE_PREFIX);
            }
            else

            {
                if(atof(strVal)*100.0 >= 0)
                    val = (int)((atof(strVal)*100.0) + DOUBLE_PREFIX);
                else
                    val = (int)((atof(strVal)*100.0) - DOUBLE_PREFIX);
            }

            map<int,int>::iterator pos = m_mapHogaRow.find(val);

            if (pos!=m_mapHogaRow.end())
            {
                if (atoi((char*)data[41])>0)    // 0이라면 아무래도 장전~
                {
                    if (m_MdgaCell[0] != pos->second)
                    {
                        m_MdgaCell[0] = pos->second;
                        SetMdgaToCenter();
                    }
                }
                else if (bCurrChange)
                {
                    SetCurrToCenter();
                }
            }
            else if (bCurrChange)
            {
                SetCurrToCenter();
            }
        }

        map<int,int>::iterator pos;

        for(n=0; n<MAX_FHOGA; ++n)
        {
            CString strHoga;

			if (m_pSiseData->code.GetAt(0)!='4' && m_pSiseData->code.GetAt(0) != 'D')  //파생상품 코드개편
            {
                strHoga = (char*)data[51+n];
				hoga = (int)(fabs(atof(strHoga) * 100.0) + DOUBLE_PREFIX);
            }
            else
            {
                strHoga = (char*)data[51+n];
				strHoga.Replace("++", "+");
				strHoga.Replace("-+", "+");
				strHoga.Replace("0+", "+");
				strHoga.Replace("--", "-");
				strHoga.Replace("+-", "-");
				strHoga.Replace("0-", "-");

                if(atof(strHoga)*100.0 >= 0)
                    hoga = (int)((atof(strHoga)*100.0) + DOUBLE_PREFIX);
                else
                    hoga = (int)((atof(strHoga)*100.0) - DOUBLE_PREFIX);
            }


//          s.Format("3007 REAL DATA HOGA [%d]\n",hoga);

//          OutputDebugString(s);
            

            pos = m_mapHogaRow.find(hoga);
            if (pos!=m_mapHogaRow.end())
            {
                if (pos->second>0)
                {
                    row = pos->second;
                    int dvol = atoi((char*)data[41+n]);
                    int dcnt = atoi((char*)data[211+n]);

                    SetItemText(row, IDX_ASK_SIZE  , (dvol) ? Int2CommaStr(dvol) : ""); // 매도잔량
                    SetItemText(row, IDX_ASK_BEFORE, (dcnt) ? Int2CommaStr(dcnt) : ""); // 매도건수
                    RedrawCell(row, IDX_ASK_SIZE); RedrawCell(row, IDX_ASK_BEFORE);
                    m_MdgaCell[n] = row;
                    setMado.erase(row);
                }
            }
 

			if (m_pSiseData->code.GetAt(0)!='4' && m_pSiseData->code.GetAt(0) != 'D')  //파생상품 코드개편
            {
                strHoga = (char*)data[71+n];
                hoga = (int)(fabs(atof(strHoga)*100.0) + DOUBLE_PREFIX);
            }
            else
            {
                strHoga = (char*)data[71+n];
				strHoga.Replace("++", "+");
				strHoga.Replace("-+", "+");
				strHoga.Replace("0+", "+");
				strHoga.Replace("--", "-");
				strHoga.Replace("+-", "-");
				strHoga.Replace("0-", "-");
                
                if(atof(strHoga)*100.0 >= 0)
                    hoga = (int)((atof(strHoga)*100.0) + DOUBLE_PREFIX);
                else
                    hoga = (int)((atof(strHoga)*100.0) - DOUBLE_PREFIX);
            }

//          s.Format("3007 REAL DATA HOGA [%d]\n",hoga);

//          OutputDebugString(s);

            pos = m_mapHogaRow.find(hoga);

            if (pos!=m_mapHogaRow.end())
            {
                if (pos->second>0)
                {
                    row = pos->second;
                    int svol = atoi((char*)data[61+n]);
                    int scnt = atoi((char*)data[221+n]);
                    SetItemText(row, IDX_BID_SIZE  , (svol) ? Int2CommaStr(svol) : ""); // 매수잔량
                    SetItemText(row, IDX_BID_BEFORE, (scnt) ? Int2CommaStr(scnt) : ""); // 매수건수
                    RedrawCell(row, IDX_BID_SIZE); RedrawCell(row, IDX_BID_BEFORE);
                    m_MsgaCell[n] = row;
                    setMasu.erase(row);
                }
            }
        }


        set<int>::iterator st, ed;

        for(st=setMado.begin(), ed=setMado.end(); st!=ed; ++st)
        {
            SetItemText(*st, IDX_ASK_SIZE, "");
            SetItemText(*st, IDX_ASK_BEFORE, "");
            RedrawCell(*st, IDX_ASK_SIZE);
            RedrawCell(*st, IDX_ASK_BEFORE);
        }

        for(st=setMasu.begin(), ed=setMasu.end(); st!=ed; ++st)
        {
            SetItemText(*st, IDX_BID_SIZE, "");
            SetItemText(*st, IDX_BID_BEFORE, "");
            RedrawCell(*st, IDX_BID_SIZE);
            RedrawCell(*st, IDX_BID_BEFORE);
        }
    }
}

void CFOGridCtrl::Clear()
{
	SetGridBody();
	ClearCells(CCellRange(1, 0, GetRowCount()-1, GetColumnCount()-1));	
}

void CFOGridCtrl::SetMicheg( PriceMichegMap *pmm )
{
	CCellID id;
	CString txt;
	set<CCellID> tmp;
	PriceMichegMap::iterator st, ed;

	st = pmm->begin();
	ed = pmm->end();
	for(; st!=ed; ++st)
	{
		id.row = m_mapHogaRow[st->first];
		if (!id.row) continue;
	
		if (st->second.d_cnt)
		{
			id.col = IDX_ASK_JUMUN;
			txt.Format("%d (%d)", st->second.d_cnt, st->second.d_wqty);
			if (txt!=GetItemText(id.row, id.col))
			{
				SetItemText(id.row, id.col, txt);			
				RedrawCell(id.row, id.col);
			}
// 			OutputDebugString(txt);

			m_JumnCells.erase(id);
			tmp.insert(id);
			//TRACE("\nSet to Mado string [%d/%d]", id.row, id.col);
		}

		if (st->second.s_cnt)
		{
			id.col = IDX_BID_JUMUN;
			txt.Format("%d (%d)", st->second.s_cnt, st->second.s_wqty);
			if (txt!=GetItemText(id.row, id.col))
			{
				SetItemText(id.row, id.col, txt);
				RedrawCell(id.row, id.col);
			}
// 			OutputDebugString(txt);
				
			m_JumnCells.erase(id);
			tmp.insert(id);
			//TRACE("\nSet to Masu string [%d/%d]", id.row, id.col);
		}
		
		/*
		TRACE("호가 미체결정보 - 가격(%d) 매도(%d/%d) 매수(%d/%d)\n",
			st->second.oprc, st->second.d_cnt, st->second.d_wqty, st->second.s_cnt, st->second.s_wqty );
		*/
	}
	set<CCellID>::iterator cst = m_JumnCells.begin();
	set<CCellID>::iterator ced = m_JumnCells.end();
	for(; cst!=ced; ++cst)
	{	
		//TRACE("\nSet to empty string [%d/%d]", cst->row, cst->col);
		SetItemText(cst->row, cst->col, "");
		RedrawCell(cst->row, cst->col);
	}
	m_JumnCells = tmp;
}

void CFOGridCtrl::CheckExpand()
{
	//bool bExpandHoga = (!m_bShowRate && !m_bShowRsvd && !m_bShowBefore);
	bool bExpandHoga = (!m_bShowRsvd && !m_bShowBefore);
	if (m_bExpandHoga!=bExpandHoga)
	{
		m_bExpandHoga = bExpandHoga;
		int add_width = (m_bExpandHoga ? EXPAND_HOGA_WIDTH : -EXPAND_HOGA_WIDTH);
		SetColumnWidth(IDX_HOGA, GetColumnWidth(IDX_HOGA) + add_width);
		SetColumnWidth(IDX_BID_SIZE, GetColumnWidth(IDX_BID_SIZE) + add_width);
		SetColumnWidth(IDX_ASK_SIZE, GetColumnWidth(IDX_ASK_SIZE) + add_width);
	}
}

BOOL CFOGridCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (m_bHogaFix)
		return TRUE;
	else
		return CGridCtrlExt::OnMouseWheel(nFlags, zDelta, pt);
	//return CGridCtrlExt::OnMouseWheel(nFlags, zDelta, pt);
}

void CFOGridCtrl::SetHogaFix( BOOL bFixed )
{
	if (m_bHogaFix==bFixed) return;
	m_bHogaFix = bFixed;
	SetMdgaToCenter();
	//ShowScrollBar(SB_VERT, !bFixed);
}

void CFOGridCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar==VK_SPACE)
	{
		CWnd *pParent = GetParent();
		if (pParent)
		{
			CRect rc;
			POINT pos;
			GetCursorPos(&pos);
			GetWindowRect(&rc);
			HWND hWnd = ::WindowFromPoint(pos);

			if (GetSafeHwnd()==hWnd)
			{
				pos.x -= rc.left;
				pos.y -= rc.top;
				CCellID cid = GetCellFromPt(pos);
//				TRACE("Output -> %d/%d\n", cid.row, cid.col);

 				NM_GRIDVIEW nmgv;
 				nmgv.iRow         = cid.row;
 				nmgv.iColumn      = cid.col;
 				nmgv.hdr.hwndFrom = m_hWnd;
 				nmgv.hdr.idFrom   = GetDlgCtrlID();
 				nmgv.hdr.code     = NM_CHAR;
				
				pParent->SendMessage(WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);
			}
		}
	}
	CGridCtrlExt::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CFOGridCtrl::SetWidths( int rsvd, int jumun, int cnt, int vol, int hoga, int rate )
{
	m_HeadWidth[IDX_ASK_RSVD] = m_HeadWidth[IDX_BID_RSVD] = rsvd;
	m_HeadWidth[IDX_ASK_JUMUN] = m_HeadWidth[IDX_BID_JUMUN] = jumun;
	m_HeadWidth[IDX_ASK_BEFORE] = m_HeadWidth[IDX_BID_BEFORE] = cnt;
	m_HeadWidth[IDX_ASK_SIZE] = m_HeadWidth[IDX_BID_SIZE] = vol;
	m_HeadWidth[IDX_HOGA] = hoga;
	m_HeadWidth[IDX_RATE] = rate;
	SetGridHeader();
	Refresh();
}

void CFOGridCtrl::GetWidths(int *rsvd, int *jumun, int *cnt, int *vol, int *hoga, int *rate)
{
	*rsvd = m_HeadWidth[IDX_ASK_RSVD];
	*jumun = m_HeadWidth[IDX_ASK_JUMUN];
	*cnt = m_HeadWidth[IDX_ASK_BEFORE];
	*vol = m_HeadWidth[IDX_ASK_SIZE];
	*hoga = m_HeadWidth[IDX_HOGA];
	*rate = m_HeadWidth[IDX_RATE];
}

BOOL CFOGridCtrl::IsMarketOrder( CCellID id )
{
	vector<CArrow>::iterator st = m_Arrows.begin();
	vector<CArrow>::iterator ed = m_Arrows.end();
	for(;st!=ed; ++st)
	{
		// 		s.Format("4.m_st x : [%d]	m_st y : [%d]	m_sid row : [%d]\n",st->m_st.x,st->m_st.y,st->m_sid.row);
		// 		OutputDebugString(s);
		if(st->m_sid.row == id.row && st->m_sid.col == id.col)
		{
			if(st->m_bmarket)
				return TRUE;
			else
				return FALSE;
		}
	}

	return FALSE;
}
