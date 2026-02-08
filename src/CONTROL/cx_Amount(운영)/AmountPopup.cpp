// AmountPopup.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Amount.h"
#include "AmountPopup.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GRID_JCNT	1000
#define GRID_JMON	2000
#define GRID_OCNT	3000


/////////////////////////////////////////////////////////////////////////////
// CAmountPopup

CAmountPopup::CAmountPopup()
{
	m_nStatus = 1;
	m_pParent = nullptr;
	m_pWizard = nullptr;

	m_crBlack = RGB(0, 0, 0);
	m_crWhite = RGB(255, 255, 255);

	m_bAlways = FALSE;

	m_pJcntGrid = nullptr;
	m_pJmonGrid = nullptr;
	m_pFont = nullptr;

	m_fDone = false;
}

CAmountPopup::~CAmountPopup()
{
}


BEGIN_MESSAGE_MAP(CAmountPopup, CWnd)
	//{{AFX_MSG_MAP(CAmountPopup)
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_ACTIVATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAmountPopup message handlers
int CAmountPopup::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect	rc;

	m_profile.Format(CONFIG_INI, Variant(homeCC,""), Variant(nameCC,""));
	m_pFont = getAxFont("굴림체",9);
	this->GetClientRect(&rc);
	if (m_nStatus == 1 || m_nStatus == 7)
	{
		rc.DeflateRect(SPACE_LEFT,SPACE_TOP,0,SPACE_BOTTOM);	//2013.10.08 KSJ
		m_rcJcntGrid = CRect(rc.left, rc.top, rc.Width()+SPACE_LEFT, rc.Height()*3/5);
		m_rcJmonGrid = CRect(rc.left, rc.Height()*3/5, rc.Width()+SPACE_LEFT, rc.Height());

		// 수량
		m_pJcntGrid = std::make_shared<CGridCtrl>();
		m_pJcntGrid->SetNoScrollBar(TRUE);
		m_pJcntGrid->Create(m_rcJcntGrid, this, GRID_JCNT);
		m_pJcntGrid->EnableSelection(FALSE);
		m_pJcntGrid->SetFrameFocusCell(FALSE);
		m_pJcntGrid->EnableTitleTips(FALSE);
		m_pJcntGrid->SetFont(m_pFont);
		
		CGridCellBase *pc = m_pJcntGrid->GetDefaultCell(FALSE, FALSE);
		if (pc)
		{
			pc->SetFormat( DT_VCENTER|DT_SINGLELINE|DT_RIGHT );
			pc->SetMargin( 5 );
			m_pJcntGrid->SetRowCount(AMT_GRIDROWS);		//3
			m_pJcntGrid->SetColumnCount(AMT_GRIDCOLS);	//10
		}
		m_pJcntGrid->ShowScrollBar(SB_BOTH, FALSE);
		
		m_pJcntGrid->ShowWindow(SW_SHOW);
		// 수량 끝
		
		// 금액
		m_pJmonGrid = std::make_shared<CGridCtrl>();
		m_pJmonGrid->SetNoScrollBar(TRUE);
		m_pJmonGrid->Create(m_rcJmonGrid, this, GRID_JMON);
		m_pJmonGrid->EnableSelection(FALSE);
		m_pJmonGrid->SetFrameFocusCell(FALSE);
		m_pJmonGrid->EnableTitleTips(FALSE);
		m_pJmonGrid->SetFont(m_pFont);
		
		pc = m_pJmonGrid->GetDefaultCell(FALSE, FALSE);
		if (pc)
		{
			pc->SetFormat( DT_VCENTER|DT_SINGLELINE|DT_RIGHT );
			pc->SetMargin( 5 );
			m_pJmonGrid->SetRowCount(MON_GRIDROWS);			//2
			m_pJmonGrid->SetColumnCount(MON_GRIDCOLS);		//5
		}
		m_pJmonGrid->ShowScrollBar(SB_BOTH, FALSE);
		
		m_pJmonGrid->ShowWindow(SW_SHOW);
		// 금액 끝
		
		LoadConfig();
	}
	else
	{
		rc.DeflateRect(0,16,0,0);
		m_pJcntGrid = std::make_shared<CGridCtrl>();
		m_pJcntGrid->SetNoScrollBar(TRUE);
		m_pJcntGrid->Create(rc, this, GRID_OCNT);
		m_pJcntGrid->EnableSelection(FALSE);
		m_pJcntGrid->SetFrameFocusCell(FALSE);
		m_pJcntGrid->EnableTitleTips(FALSE);
		m_pJcntGrid->SetFont(m_pFont);
		
		CGridCellBase *pc = m_pJcntGrid->GetDefaultCell(FALSE, FALSE);
		if (pc)
		{
			pc->SetFormat( DT_VCENTER|DT_SINGLELINE|DT_RIGHT );
			pc->SetMargin( 5 );
			m_pJcntGrid->SetRowCount(3);
			m_pJcntGrid->SetColumnCount(10);
		}
		m_pJcntGrid->ShowScrollBar(SB_BOTH, FALSE);
		
		m_pJcntGrid->ShowWindow(SW_SHOW);

		m_pJmonGrid = m_pJcntGrid;	//핸들없으면 setFocus에서 에러남
		//m_pJmonGrid = std::unique_ptr<CGridCtrl>(m_pJcntGrid.get());	//핸들없으면 setFocus에서 에러남
		LoadConfig();
	}

	return 0;
}

void CAmountPopup::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	CRect rc;

	GetClientRect(&rc);

	if (m_nStatus == 1 || m_nStatus == 7)
	{
		rc.left = rc.left + SPACE_LEFT;			//2013.10.08 KSJ
		rc.bottom = rc.bottom - SPACE_BOTTOM;		//2013.10.08 KSJ

		rc.right = rc.left + 400;
		rc.top = rc.top + SPACE_TOP;
		
		m_rcJcntGrid = CRect(rc.left, rc.top, rc.Width()+SPACE_LEFT, rc.Height()*AMT_GRIDROWS/5 + SPACE_BOTTOM);
		m_rcJmonGrid = CRect(rc.left, rc.Height()*AMT_GRIDROWS/5 + SPACE_BOTTOM -1, rc.Width()+SPACE_LEFT, rc.Height()+ SPACE_BOTTOM);

		//수량
		m_pJcntGrid->MoveWindow(m_rcJcntGrid);

		int row = m_pJcntGrid->GetRowCount();
		int col = m_pJcntGrid->GetColumnCount();

		int n=0;

		m_pJcntGrid->SetNoScrollBar(TRUE);

		for (n=0; n<col; ++n)
			m_pJcntGrid->SetColumnWidth(n, m_rcJcntGrid.Width()/col);
		for (n=0; n<row; ++n)
			m_pJcntGrid->SetRowHeight(n, m_rcJcntGrid.Height()/AMT_GRIDROWS);
		//수량 끝

		//금액
		m_pJmonGrid->MoveWindow(m_rcJmonGrid);
		
		row = m_pJmonGrid->GetRowCount();
		col = m_pJmonGrid->GetColumnCount();
		
		n=0;
		
		m_pJmonGrid->SetNoScrollBar(TRUE);
		
		for (n=0; n<col; ++n)
			m_pJmonGrid->SetColumnWidth(n, m_rcJmonGrid.Width()/col);
		for (n=0; n<row; ++n)
			m_pJmonGrid->SetRowHeight(n, m_rcJmonGrid.Height()/MON_GRIDROWS);
		//금액 끝
	}
	else
	{
		rc.left -= 1;
		rc.right = rc.left + 400;
		rc.top = rc.top + 16;
		rc.bottom += 2;
		
		m_pJcntGrid->MoveWindow(rc);
		
		const	int row = m_pJcntGrid->GetRowCount();
		const	int col = m_pJcntGrid->GetColumnCount();
		
		m_pJcntGrid->SetNoScrollBar(TRUE);
		
		for (int n=0; n<col; ++n)
			m_pJcntGrid->SetColumnWidth(n, rc.Width()/col);
		for (int n=0; n<row; ++n)
			m_pJcntGrid->SetRowHeight(n, rc.Height()/AMT_GRIDROWS);
	}
}

BOOL CAmountPopup::OnEraseBkgnd(CDC* pDC) 
{
// 	CRect rc;
// 	GetClientRect(&rc);
// 	pDC->FillSolidRect(rc, RGB(255, 255, 255));
// 	
 	return CWnd::OnEraseBkgnd(pDC);
}

BOOL CAmountPopup::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	static CString oldVal;
	static CCellID oldId(-1, -1);

	CGridCtrl* pGrid = NULL, * pOldGrid = NULL;
	CString sSection, sPrice, sUnit;
	int	nParam = 0;

	switch (wParam)
	{
	case GRID_JCNT:
		pGrid = m_pJcntGrid.get();
		pOldGrid = m_pJmonGrid.get();
		sSection = "JQTY_GRID";
		sPrice = "";
		nParam = 1; 
		break;
	case GRID_JMON:
		pGrid = m_pJmonGrid.get();
		pOldGrid = m_pJcntGrid.get();
		sSection = "JMON_GRID";
		sPrice = Variant(popCC, "OSBJPrc");
		sUnit = Variant(popCC, "OSBJ1377");
		sPrice.Trim();
		sUnit.Trim();
		nParam = 3;
		break;
	case GRID_OCNT:
		pGrid = m_pJcntGrid.get();
		sSection = "OQTY_GRID";
		sPrice = "";
		nParam = 1; 
		break;
	default:break;
	}

	if (pGrid != NULL)
	{
		*pResult = 1;
		
		const	GV_DISPINFO *pi = (GV_DISPINFO*)lParam;

		if (pi->hdr.code == GVN_BEGINLABELEDIT)
		{
			// 편집 시작하면 현재 값을 저장
			oldVal = pGrid->GetItemText(pi->item.row, pi->item.col);
		}
		else if (pi->hdr.code == GVN_ENDLABELEDIT || pi->hdr.code == GVN_SELCHANGED)
		{
			if (pOldGrid != NULL)
			{
				//이전에 선택된 그리드의 색깔 초기화.
				for (int row=0; row<pOldGrid->GetRowCount(); ++row)
				{
					for (int col=0; col<pOldGrid->GetColumnCount(); ++col)
					{
						pOldGrid->SetItemBkColour(row, col, m_crWhite);
						pOldGrid->SetItemFgColour(row, col, m_crBlack);
					}
				}
				
				pOldGrid->Invalidate(FALSE);
				pOldGrid->SetFocusCell(-1, -1);	//선택을 초기화 시킴
			}

			const	CCellID cId(pi->item.row, pi->item.col);
			CString key, val = pGrid->GetItemText(cId.row, cId.col);
			val.Trim();
			val.Replace(",", "");

			if (pi->hdr.code==GVN_ENDLABELEDIT)
			{
				// 공백이면 원래값으로 되돌린다.
				if (val.GetLength()>0)
				{
					if (atof(val) > 2000000000)	//2013.10.10 20억이 넘어가면 atoi에서 깨짐
					{
						val = oldVal;
						pGrid->SetItemText(cId.row, cId.col, oldVal);
					}
					else
					{
						key.Format("MA_%d_%d", cId.row, cId.col);
						WritePrivateProfileString(sSection, (LPCSTR)key, (LPCSTR)val, (LPCSTR)m_profile);
					}
				} 
				else 
				{
					val = oldVal;
					pGrid->SetItemText(cId.row, cId.col, oldVal);
				}
			}

			if (wParam == GRID_JMON)
			{
				int nValue = 0;

				if (atoi(sPrice) > 0 && atoi(sUnit) > 0)
					nValue = atoi(val) / atoi(sPrice) / atoi(sUnit) * atoi(sUnit);

				val.Format("%d", nValue);
			}

			m_pParent->SendMessage(WM_USER + 1, nParam, atoi(val));
// 			PostMessage(WM_USER, 0, 0);
			
			for (int row=0; row<pGrid->GetRowCount(); ++row)
			{
				for (int col=0; col<pGrid->GetColumnCount(); ++col)
				{
					if (pi->item.row==row && pi->item.col==col)
					{
						pGrid->SetItemBkColour(row, col, ::GetSysColor(COLOR_HIGHLIGHT));
						pGrid->SetItemFgColour(row, col, ::GetSysColor(COLOR_HIGHLIGHTTEXT));
					}
					else
					{
						pGrid->SetItemBkColour(row, col, m_crWhite);
						pGrid->SetItemFgColour(row, col, m_crBlack);
					}
				}
			}
			pGrid->Invalidate(FALSE);
		}
		else if (pi->hdr.code == NM_DBLCLK)	//2013.10.24 KSJ 더블클릭이 들어왔을때는 종료한다
		{
			if (!m_bAlways)
				PostMessage(WM_USER, 0, 0);
		}
	}

	return CWnd::OnNotify(wParam, lParam, pResult);
}

void CAmountPopup::LoadConfig()
{
	int	ival = 0;
	CString key, sval;
	
	if (m_nStatus == 1 || m_nStatus == 7)
	{
		const	int def_val[][10] = {{10, 20, 30, 40, 50, 60, 70, 80, 90, 100},
						{100, 200, 300, 400, 500, 600, 700, 800, 900, 1000},
						{1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000}};
		
		for (int row=0; row<AMT_GRIDROWS; ++row)
		{
			for (int col=0; col<AMT_GRIDCOLS; ++col)
			{
				key.Format("MA_%d_%d", row, col);
				ival = GetPrivateProfileInt("JQTY_GRID", (LPCSTR)key, def_val[row][col], (LPCSTR)m_profile);
				sval.Format("%d", ival);
				m_pJcntGrid->SetItemText(row, col, (LPCSTR)sval );
			}
		}

		const	int	def_val1[][5] = {{100000, 200000, 300000, 500000, 1000000},
					{1000000, 2000000, 3000000, 5000000, 10000000}};
		
		for (int row=0; row<MON_GRIDROWS; ++row)
		{
			for (int col=0; col<MON_GRIDCOLS; ++col)
			{
				key.Format("MA_%d_%d", row, col);
				ival = GetPrivateProfileInt("JMON_GRID", (LPCSTR)key, def_val1[row][col], (LPCSTR)m_profile);
				sval.Format("%d", ival);
				addComma(sval);
				m_pJmonGrid->SetItemText(row, col, (LPCSTR)sval );
			}
		}
	}
	else
	{
		const	int	def_val[][10] = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
						{10, 20, 30, 40, 50, 60, 70, 80, 90, 100},
			{100, 200, 300, 400, 500, 600, 700, 800, 900, 1000}};
		
		for (int row=0; row<AMT_GRIDROWS; ++row)
		{
			for (int col=0; col<AMT_GRIDCOLS; ++col)
			{
				key.Format("MA_%d_%d", row, col);
				ival = GetPrivateProfileInt("OQTY_GRID", (LPCSTR)key, def_val[row][col], (LPCSTR)m_profile);
				sval.Format("%d", ival);
				m_pJcntGrid->SetItemText(row, col, (LPCSTR)sval );
			}
		}
	}
}

void CAmountPopup::addComma(CString& data)
{
	CString	tmps;
	int	pos = 0, length = 0;
	
	tmps = data;
//	tmps.Trim();
	length = tmps.GetLength();
	pos = tmps.Find('.') + 1;
	length -= pos;
	
	if (length < 4)
		return;
	
	data.Empty();
	for (int ii = 0; ii < length; ii++)
	{
		if ((length-ii) % 3 == 0 && ii != 0)
			data += ',';
		data += tmps.GetAt(ii);
	}
}

void CAmountPopup::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);

	MSG	msg{};
	bool	bSendChangeEvent = false;

	for (m_fDone = FALSE; !m_fDone; WaitMessage())
	{
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if ((msg.message == WM_KILLFOCUS && msg.hwnd == this->m_hWnd) || 
				((msg.message == WM_LBUTTONDOWN || msg.message == WM_NCLBUTTONDOWN) && msg.hwnd != this->m_hWnd && msg.hwnd != m_pJcntGrid->m_hWnd && msg.hwnd != m_pJmonGrid->m_hWnd) ||
				(msg.message == WM_USER && msg.hwnd == this->m_hWnd && msg.hwnd != m_pJcntGrid->m_hWnd && msg.hwnd != m_pJmonGrid->m_hWnd))
			{
				if ((msg.message == WM_LBUTTONDOWN) && msg.hwnd != this->m_hWnd)
				{
					if (!m_bAlways)
					{
						m_pParent->SendMessage(WM_USER + 1, 2, NULL);
						bSendChangeEvent = true;
					}
				}
				//if (msg.message == WM_LBUTTONDOWN || msg.message == WM_NCLBUTTONDOWN)
				//	::PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
				m_fDone = TRUE;
				break;
			}
			
			if (!IsDialogMessage(&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if (m_fDone)
			break;
	}
}

void CAmountPopup::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
	
	if (nState == WA_INACTIVE)
		PostMessage(WM_USER, 0, 0);	
}

void CAmountPopup::OnLButtonDown(UINT nFlags, CPoint point) 
{
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CAmountPopup::OnMouseMove(UINT nFlags, CPoint point) 
{
	
	CWnd::OnMouseMove(nFlags, point);
}

void CAmountPopup::OnLButtonUp(UINT nFlags, CPoint point) 
{
// 	int tmp = 0;
// 	if (getPosition(point, tmp) != CRect(0, 0, 0, 0))
// 	{
// 		int tmp = 0;
// 		getPosition(point, tmp);
// 		m_pParent->SendMessage(WM_USER + 1, 1, tmp);
// 		PostMessage(WM_USER, 0, 0);
// 	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CAmountPopup::drawCaption(CDC *pDC)
{
	CRect	rcTop, rcBottom, rcLeftTop, rcLeftBottom;
	
	GetClientRect(&rcTop);

	if (m_nStatus == 1 || m_nStatus == 7)
	{
		//2011.12.15 KSJ
		//우선 hFont에 대입하고 해제해준다.
		CFont	*pOldFont = (CFont*)pDC->SelectObject(m_pFont);
		// KSJ
		
		CPen	*pOldPen = (CPen*)pDC->SelectObject(getAxPen(m_crBlack, 1, PS_SOLID));

		rcTop.top -= 1;
		rcTop.left -= 1;
 		rcTop.right += 2;
		rcTop.bottom += 1;

		rcBottom = rcLeftTop = rcLeftBottom = rcTop;

		// 상단 타이틀(수량/금액 선택)
		rcTop.bottom = rcTop.top + SPACE_TOP + GAP;
		pDC->FillSolidRect(rcTop, GetSysColor(COLOR_HIGHLIGHT));
		pDC->Draw3dRect(rcTop, m_crBlack, m_crBlack);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
		pDC->DrawText(LoadStringTb(IDS_SELECTAMTSTR), rcTop, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		// 하단 유의사항(※ 금액은 수수료 미반영)
		rcBottom.top = rcBottom.bottom - SPACE_BOTTOM - GAP;
		pDC->FillSolidRect(rcBottom, m_crWhite);
		pDC->Draw3dRect(rcBottom, m_crBlack, m_crBlack);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->DrawText(LoadStringTb(IDS_CAPTIONSTR), rcBottom, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		// 좌상단 타이틀(수량)
		rcLeftTop.top = rcLeftTop.top + SPACE_TOP + 1;
		rcLeftTop.bottom = rcLeftTop.top + 57;
		rcLeftTop.right = rcLeftTop.left + SPACE_LEFT + GAP;

		pDC->FillSolidRect(rcLeftTop, GetSysColor(COLOR_HIGHLIGHT));
		pDC->Draw3dRect(rcLeftTop, m_crBlack, m_crBlack);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
		rcLeftTop.top += 14;
		pDC->DrawText(LoadStringTb(IDS_AMTSTR), rcLeftTop, DT_CENTER | DT_VCENTER);
		
		// 좌하단 타이틀(금액)
		rcLeftBottom.top = rcLeftTop.bottom -1;
		rcLeftBottom.bottom = rcLeftBottom.top + 40;
		rcLeftBottom.right = rcLeftBottom.left + SPACE_LEFT + GAP;

		pDC->FillSolidRect(rcLeftBottom, GetSysColor(COLOR_HIGHLIGHT));
		pDC->Draw3dRect(rcLeftBottom, m_crBlack, m_crBlack);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
		rcLeftBottom.top += 8;
		pDC->DrawText(LoadStringTb(IDS_MONSTR), rcLeftBottom, DT_CENTER);

		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldFont);
	}
	else
	{
		CRect	rc;
		
		GetClientRect(&rc);
		rc.bottom = rc.top + 16;
		pDC->FillSolidRect(rc, GetSysColor(COLOR_HIGHLIGHT));
		rc.DeflateRect(2, 0, 16, 0);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
		pDC->DrawText(LoadStringTb(IDS_SELECTAMTSTR1), rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
}

void CAmountPopup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	rc;

	//2011.12.15 KSJ
	//우선 hFont에 대입하고 해제해준다.
	CFont	*pOldFont = (CFont*)dc.SelectObject(m_pFont);
	CPen	*pOldPen = (CPen*)dc.SelectObject(getAxPen(GetSysColor(COLOR_HIGHLIGHTTEXT), 1, PS_SOLID));
	// KSJ

	drawCaption(&dc);

	// exit button
	GetClientRect(&rc);
	rc.left = rc.right - 14;
	rc.top = rc.top + 2;
	rc.bottom = rc.top + 10;
	rc.right = rc.left + 10;
	dc.Rectangle(rc);
	rc.DeflateRect(1, 1);
	dc.FillSolidRect(rc, GetSysColor(COLOR_HIGHLIGHT));
	rc.InflateRect(1, 1);
	dc.MoveTo(rc.left, rc.top);
	dc.LineTo(rc.right - 1, rc.bottom - 1);
	dc.MoveTo(rc.right - 1, rc.top);
	dc.LineTo(rc.left, rc.bottom - 1);

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldFont);
	
	// Do not call CWnd::OnPaint() for painting messages
}

LRESULT CAmountPopup::OnNcHitTest(CPoint point)
{
	LRESULT nHitTest = CWnd::OnNcHitTest(point);
	CRect	rc;

	GetWindowRect(&rc);
	rc.bottom = rc.top + 16;
	if (nHitTest == HTCLIENT && rc.PtInRect(point))
		nHitTest = HTCAPTION;
	
	return nHitTest;
}

void CAmountPopup::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	CRect	rc;

	GetWindowRect(&rc);
	rc.left = rc.right - 14;
	rc.top = rc.top + 2;
	rc.bottom = rc.top + 10;
	rc.right = rc.left + 10;
	if (rc.PtInRect(point))
	{
		m_pParent->SendMessage(WM_USER + 1, 2, NULL);
		PostMessage(WM_USER, 0, 0);
	}

	CWnd::OnNcLButtonDown(nHitTest, point);
}

void CAmountPopup::setParent(CWnd *pWnd)
{
	m_pParent = pWnd;
}

CFont* CAmountPopup::getAxFont(CString fName, int point)
{
	struct _fontR fontR {};

	fontR.name = (LPSTR)fName.GetString();
	fontR.point = point;
	fontR.italic = false;
	fontR.bold = 0;
	return (CFont*)m_pWizard->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CPen* CAmountPopup::getAxPen(COLORREF clr, int nWidth, int nStyle)
{
	struct _penR penR {};

	penR.clr = clr;
	penR.width = nWidth;
	penR.style = nStyle;
	return (CPen*)m_pWizard->SendMessage(WM_USER, getPEN, (long)&penR);
}

CString CAmountPopup::LoadStringTb(UINT nIdx)
{
	CString str;
	str.LoadString(nIdx);
	return str;
}

int CAmountPopup::Variant(int cmd, int data)
{
	return m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, cmd), (LPARAM)data);
}

LPCTSTR CAmountPopup::Variant( int cmd, LPCTSTR data )
{
	return (LPCTSTR)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, cmd), (LPARAM)data);
}

void CAmountPopup::SetPopupAlways( BOOL bflag )
{
	m_bAlways = bflag;
}
