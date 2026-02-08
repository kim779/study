// CX_PricePopup.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Price.h"
#include "PricePopup.h"
#include "ControlWnd.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TP_NONE		0
#define	TP_DBLCLK	1
/////////////////////////////////////////////////////////////////////////////
// CPricePopup

CPricePopup::CPricePopup()
{
	m_pParent = nullptr;
	m_pWizard = nullptr;

	m_nStatus = 1;
	m_selRc.SetRectEmpty();
	m_pVScroll = nullptr; 
	m_startPos = 0;
	m_nType = TP_NONE;

	m_upClr = RGB(226, 0, 0);
	m_dnClr = RGB(0, 31, 146);

	m_pDoubleTmp = 0;
	m_bNoMaxMin = false;

	m_fDone = false;
	m_bDrag = false;
	m_bLhGd = false;
	m_bKospi = false;

	m_dCurr = m_dMax = m_dMin = m_dGjga = 0;
	m_fStart = m_fEnd = m_fSel = 0;
}

CPricePopup::~CPricePopup()
{
}


BEGIN_MESSAGE_MAP(CPricePopup, CWnd)
	//{{AFX_MSG_MAP(CPricePopup)
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_ACTIVATE()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_NCPAINT()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPricePopup message handlers

BOOL CPricePopup::OnEraseBkgnd(CDC* pDC) 
{
	CRect	rc;

	GetClientRect(&rc);
	pDC->FillSolidRect(rc, RGB(255, 255, 255));
	
	return CWnd::OnEraseBkgnd(pDC);
}

void CPricePopup::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	MSG	msg{};

	for (m_fDone = false; !m_fDone; WaitMessage())
	{
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if ((msg.message == WM_KILLFOCUS && msg.hwnd == this->m_hWnd) || 
				((msg.message == WM_LBUTTONDOWN || msg.message == WM_NCLBUTTONDOWN) && 
				(msg.hwnd != this->m_hWnd && (!m_pVScroll || msg.hwnd != m_pVScroll->m_hWnd))) ||
				(msg.message == WM_USER && msg.hwnd == this->m_hWnd))
			{
				if (msg.message == WM_LBUTTONDOWN || msg.message == WM_NCLBUTTONDOWN)
					::PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
				m_fDone = true;
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
	ShowWindow(SW_HIDE);
}

void CPricePopup::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
	
	if (nState == WA_INACTIVE)
		PostMessage(WM_USER, 0, 0);
}

void CPricePopup::drawCaption(CDC *pDC)
{
	CRect	rc;
	
	GetClientRect(&rc);
	rc.bottom = rc.top + 16;
	pDC->FillSolidRect(rc, GetSysColor(COLOR_HIGHLIGHT));

	rc.DeflateRect(2, 0, 16, 0);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	pDC->DrawText(LoadStringTb(IDS_SELECTDEPTH), rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}

void CPricePopup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	rc;
	CFont*	pOldFont = (CFont*)dc.SelectObject(getAxFont(LoadStringTb(IDS_BASEFONTNAME3), 9));

	drawCaption(&dc);
	drawData(&dc);

	CPen*	pOldPen = (CPen*)dc.SelectObject(getAxPen(GetSysColor(COLOR_HIGHLIGHTTEXT), 1, PS_SOLID));

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
}

LRESULT CPricePopup::OnNcHitTest(CPoint point) 
{
	UINT nHitTest = CWnd::OnNcHitTest(point);

	CRect	rc;

	GetWindowRect(&rc);
	rc.bottom = rc.top + 16;
	if (nHitTest == HTCLIENT && rc.PtInRect(point))
		nHitTest = HTCAPTION;
	
	return nHitTest;
}

void CPricePopup::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	CRect	rc;

	GetWindowRect(&rc);
	rc.left = rc.right - 14;
	rc.top = rc.top + 2;
	rc.bottom = rc.top + 10;
	rc.right = rc.left + 10;
	if (rc.PtInRect(point))
		PostMessage(WM_USER, 0, 0);
	
	CWnd::OnNcLButtonDown(nHitTest, point);
}

void CPricePopup::drawData(CDC *pDC)
{
	CRect	rc, tRc;
	CString sTmp;

	GetClientRect(&rc);
	rc.DeflateRect(0, 36, 0, 20);
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SetBkMode(TRANSPARENT);

	// 상한가
	tRc = rc;
	tRc.top -= 20;
	tRc.bottom = tRc.top + 20;

	if (!m_bNoMaxMin)
		tRc.right = 75;

	pDC->FillSolidRect(tRc, RGB(255, 255, 255));
	tRc.InflateRect(1, 1);
// 	pDC->Draw3dRect(tRc, RGB(0, 0, 0), RGB(0, 0, 0));
// 	tRc.DeflateRect(1, 1, 1, 1);
	pDC->Draw3dRect(tRc, RGB(0, 0, 0), RGB(0, 0, 0));
	tRc.DeflateRect(4, 4, 4, 4);

	if ((m_nStatus == 2 || m_nStatus == 6 || m_nStatus == 3) && !m_bLhGd)
		sTmp.Format("%.2f", m_dMax);
	else
		sTmp.Format("%.f", m_dMax);
	pDC->SetTextColor(m_upClr);
	sTmp = SetComma(sTmp);
	tRc.DeflateRect(0, 0, 14, 0);
	pDC->DrawText(sTmp, tRc, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	pDC->DrawText("↑", tRc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	// 하한가
	tRc = rc;
	tRc.top = rc.bottom;
	tRc.bottom += 20;

	if (!m_bNoMaxMin)
		tRc.right = 75;

	pDC->FillSolidRect(tRc, RGB(255, 255, 255));
	tRc.InflateRect(1, 1);
// 	pDC->Draw3dRect(tRc, RGB(0, 0, 0), RGB(0, 0, 0));
// 	tRc.DeflateRect(1, 1, 1, 1);
	pDC->Draw3dRect(tRc, RGB(0, 0, 0), RGB(0, 0, 0));
	tRc.DeflateRect(4, 4, 4, 4);

	if ((m_nStatus == 2 || m_nStatus == 6 || m_nStatus == 3) && !m_bLhGd)
		sTmp.Format("%.2f", m_dMin);
	else
		sTmp.Format("%.f", m_dMin);
	pDC->SetTextColor(m_dnClr);
	sTmp = SetComma(sTmp);
	tRc.DeflateRect(0, 0, 14, 0);
	pDC->DrawText(sTmp, tRc, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	pDC->DrawText("↓", tRc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	int	nCnt = min(m_arPrice.GetSize() - 2, 11);
	for (int ii = 0; ii < nCnt; ii++)
	{
		tRc = rc;
		tRc.top = rc.top + ii * rc.Height() / nCnt;
		//tRc.right = (m_pVScroll?rc.Width() - 15:rc.Width());
		if (!m_bNoMaxMin)
			tRc.right = 75;
		else
			tRc.right = (m_pVScroll?rc.Width() - 15:rc.Width());

		tRc.bottom = rc.top + (ii + 1) * rc.Height() / nCnt;
		
		if (m_dCurr != m_arPrice.GetAt(ii + m_startPos))
			pDC->FillSolidRect(tRc, RGB(255, 255, 255));
		else
			pDC->FillSolidRect(tRc, RGB(178, 178, 178));
		//pDC->FillSolidRect(tRc, RGB(255, 255, 255));
		tRc.InflateRect(1, 1);
		pDC->Draw3dRect(tRc, RGB(0, 0, 0), RGB(0, 0, 0));
		tRc.DeflateRect(4, 4, 4, 4);
		
		if (ii + m_startPos < m_arPrice.GetSize() && m_dGjga > m_arPrice.GetAt(ii + m_startPos))
			pDC->SetTextColor(m_dnClr);
		else if (ii + m_startPos < m_arPrice.GetSize() && m_dGjga < m_arPrice.GetAt(ii + m_startPos))
			pDC->SetTextColor(m_upClr);
		else
			pDC->SetTextColor(RGB(0, 0, 0));
		if (ii + m_startPos < m_arPrice.GetSize())
		{
			if ((m_nStatus == 2 || m_nStatus == 6 || m_nStatus == 3) && !m_bLhGd)
				sTmp.Format("%.2f", m_arPrice.GetAt(ii + m_startPos));
			else
				sTmp.Format("%.f", m_arPrice.GetAt(ii + m_startPos));
		}
		else
			sTmp = "0";
		sTmp = SetComma(sTmp);
		pDC->DrawText(sTmp, tRc, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	}

	//2열
	// 상한가
	if (!m_bNoMaxMin)
	{
		tRc = rc;
		tRc.top -= 20;
		tRc.left = 75;
		tRc.bottom = tRc.top + 20;
		tRc.right = 140;
		
		pDC->FillSolidRect(tRc, RGB(255, 255, 255));
		tRc.InflateRect(1, 1);
	// 	pDC->Draw3dRect(tRc, RGB(0, 0, 0), RGB(0, 0, 0));
	// 	tRc.DeflateRect(1, 1, 1, 1);
		pDC->Draw3dRect(tRc, RGB(0, 0, 0), RGB(0, 0, 0));
		tRc.DeflateRect(4, 4, 4, 4);
		
		double	percent = CalculateRate(m_dMax);
		if ((m_nStatus == 2 || m_nStatus == 6 || m_nStatus == 3) && !m_bLhGd)
			sTmp.Format("%.2lf", CeilEX(percent,2));
		else
			sTmp.Format("%.2lf", CeilEX(percent,2));
		pDC->SetTextColor(m_upClr);
		sTmp = SetComma(sTmp);
		sTmp += "%";
		sTmp.Remove('-');
		tRc.DeflateRect(0, 0, 14, 0);
		pDC->DrawText(sTmp, tRc, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		//pDC->DrawText("↑", tRc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		
		// 하한가
		tRc = rc;
		tRc.top = rc.bottom;
		tRc.left = 75;
		tRc.bottom += 20;
		tRc.right = 140;
		
		pDC->FillSolidRect(tRc, RGB(255, 255, 255));
		tRc.InflateRect(1, 1);
	// 	pDC->Draw3dRect(tRc, RGB(0, 0, 0), RGB(0, 0, 0));
	// 	tRc.DeflateRect(1, 1, 1, 1);
		pDC->Draw3dRect(tRc, RGB(0, 0, 0), RGB(0, 0, 0));
		tRc.DeflateRect(4, 4, 4, 4);

		percent = CalculateRate(m_dMin);
		if ((m_nStatus == 2 || m_nStatus == 6 || m_nStatus == 3) && !m_bLhGd)
			sTmp.Format("%.2lf", CeilEX(percent,2));
		else
			sTmp.Format("%.2lf", CeilEX(percent,2));
		pDC->SetTextColor(m_dnClr);
		sTmp = SetComma(sTmp);
		sTmp += "%";
		sTmp.Remove('-');
		tRc.DeflateRect(0, 0, 14, 0);
		pDC->DrawText(sTmp, tRc, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		//pDC->DrawText("↓", tRc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		
		nCnt = min(m_arPrice.GetSize() - 2, 11);
		for (int ii = 0; ii < nCnt; ii++)
		{
			tRc = rc;
			tRc.top = rc.top + ii * rc.Height() / nCnt;
			tRc.left = 75;
			tRc.right = (m_pVScroll?rc.Width() - 15:rc.Width());
			//tRc.right = 150;
			tRc.bottom = rc.top + (ii + 1) * rc.Height() / nCnt;
			
			if (m_dCurr != m_arPrice.GetAt(ii + m_startPos))
				pDC->FillSolidRect(tRc, RGB(255, 255, 255));
			else
				pDC->FillSolidRect(tRc, RGB(178, 178, 178));
			//pDC->FillSolidRect(tRc, RGB(255, 255, 255));
			tRc.InflateRect(1, 1);
			pDC->Draw3dRect(tRc, RGB(0, 0, 0), RGB(0, 0, 0));
			tRc.DeflateRect(4, 4, 4, 4);
			
			if (ii + m_startPos < m_arPrice.GetSize() && m_dGjga > m_arPrice.GetAt(ii + m_startPos))
				pDC->SetTextColor(m_dnClr);
			else if (ii + m_startPos < m_arPrice.GetSize() && m_dGjga < m_arPrice.GetAt(ii + m_startPos))
				pDC->SetTextColor(m_upClr);
			else
				pDC->SetTextColor(RGB(0, 0, 0));
			if (ii + m_startPos < m_arPrice.GetSize())
			{
				if ((m_nStatus == 2 || m_nStatus == 6 || m_nStatus == 3) && !m_bLhGd)
					sTmp.Format("%.2lf", CeilEX(m_arRate.GetAt(ii + m_startPos),2));
				else
					sTmp.Format("%.2lf", CeilEX(m_arRate.GetAt(ii + m_startPos),2));
			}
			else
				sTmp = "0";
			sTmp = SetComma(sTmp);
			sTmp += "%";
			sTmp.Remove('-');
			pDC->DrawText(sTmp, tRc, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		}
	}
}

void CPricePopup::makeList()
{
	CString	sprice;
	double	price = 0;
	double	rate = 0;

	if (m_bNoMaxMin)
	{
		// 상한, 하한이 없는 종목군은 nScope 만큼 상/하로 표시한다 ('ELW')
		CString	sprice;
		const	int	nScope = 10;

		m_arPrice.Add(m_dCurr);
		price = m_dCurr;
		for (int ii = 0; ii < nScope -1; ii++)
		{
			price += CalculateUnit(price);

			sprice.Format("%6.2f", price);
			price = atof(sprice);
			m_arPrice.InsertAt(0, price);

// 			rate = CalculateRate(price);
// 			m_arRate.InsertAt(0,rate);
		}
		m_dMax = price + CalculateUnit(price);
		
		price = m_dCurr;
		for (int ii = 0; ii < nScope -1; ii++)
		{
			price -= CalculateUnit(price, false);

			sprice.Format("%6.2f", price);
			price = atof(sprice);
			m_arPrice.Add(price);

// 			rate = CalculateRate(price);
// 			m_arRate.Add(rate);
		}
		m_dMin = price - CalculateUnit(price, false);
	}
	else
	{
		price = m_dMin;
		for ( ;; )
		{
			price += CalculateUnit(price);
			if (price >= m_dMax)
				break;

			sprice.Format("%6.2f", price);
			price = atof(sprice);
			m_arPrice.InsertAt(0, price);

			rate = CalculateRate(price);

			m_arRate.InsertAt(0,rate);
		}
	}

	setCurrPosition();

	if (m_arPrice.GetSize() > 11)
	{
		if (m_pVScroll)
		{
			if (::IsWindow(m_pVScroll->m_hWnd))
				m_pVScroll->DestroyWindow();
			delete m_pVScroll;
			m_pVScroll = NULL;
		}

		CRect	rc;
		SCROLLINFO info{};

		GetClientRect(&rc);

		m_pVScroll = new CScrollBar;
		m_pVScroll->Create(WS_CHILD | WS_VISIBLE | SBS_VERT, CRect(rc.Width() - 15, 36, rc.Width(), rc.Height() - 20), this, 101);
		
		info.cbSize = sizeof(SCROLLINFO);
		m_pVScroll->GetScrollInfo(&info);
		info.fMask = SIF_ALL;     

		info.nMin = 0;     
		info.nMax = m_arPrice.GetSize() - 10;
		info.nPage = 2;
		info.nPos = m_startPos;    
		info.nTrackPos = 0; 
		m_pVScroll->SetScrollInfo(&info);
	}
	else
	{
		CRect	rc;

		GetClientRect(&rc);
		SetWindowPos(NULL, 0, 0, rc.Width() - 20, m_arPrice.GetSize() * 17 + 56, SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);
	}

}

double CPricePopup::CalculateRate(double price)
{
	double	rate = 0;

	if (m_dGjga != 0)
	{
		if (price > m_dGjga)
			rate = ((price - m_dGjga) * 100) / m_dGjga;
		else if (price < m_dGjga)
			rate = ((price - m_dGjga) * 100) / m_dGjga;
	}

	return rate;
}

double CPricePopup::CalculateUnit(double price, bool bInc)
{
	CString spcd;
	double	unit = 0;

	if (m_sCode.GetLength()>2)
		spcd = m_sCode.Mid(1,2);
	switch (m_nStatus)
	{
	case 10: //주식선물
		if (!m_bNewUnit)	// asis
		{
			if (price < 5000)
			{
				unit = 2;
				if (!bInc && price <= 5) unit = 0;
			}
			else if (price < 10000)
			{
				unit = 5;
				if (!bInc && price == 5000) unit = 2;
			}
			else if (price < 50000)
			{
				unit = 25;
				if (!bInc && price == 10000) unit = 5;
			}
			else if (price < 100000)
			{
				unit = 50;
				if (!bInc && price == 50000) unit = 25;
			}
			else if (price < 500000)
			{
				unit = 250;
				if (!bInc && price == 100000) unit = 50;
			}
			else if (price >= 500000)
			{
				unit = 500;
				if (!bInc && price == 500000) unit = 250;
			}
			break;
		}
		else
		{
			// 2023 주식 호가가격단위 개선의 경우
			// 주식과 동일한 호가가격단위 사용하므로 아래 case문으로 fallthrough
		}
	case 1:	// 주식
	case 4:	// ECN
		if (m_bNewUnit)		// 2023 주식 호가가격단위 개선
		{
			if (price < 2000)
			{
				unit = 1;
				if (!bInc && price <= 1) unit = 0;
			}
			else if (price < 5000)
			{
				unit = 5;
				if (!bInc && price == 2000) unit = 1;
			}
			else if (price < 20000)
			{
				unit = 10;
				if (!bInc && price == 5000) unit = 5;
			}
			else if (price < 50000)
			{
				unit = 50;
				if (!bInc && price == 20000) unit = 10;
			}
			else if (price < 200000)
			{
				unit = 100;
				if (!bInc && price == 50000) unit = 50;
			}
			else if (price < 500000)
			{
				unit = 500;
				if (!bInc && price == 100000) unit = 100;
			}
			else if (price >= 500000)
			{
				unit = 1000;
				if (!bInc && price == 500000) unit = 500;
			}

		}
		else	// asis
		{
			if (m_bKospi)	// KOSPI 주식
			{
				if (price < 1000)	// 2010.08.02 호가단위세분화 제도변경에 의한 수정
				{
					unit = 1;
					if (!bInc && price <= 1) unit = 0;
				}
				else if (price < 5000)
				{
					unit = 5;
					if (!bInc && price <= 5) unit = 0;
				}
				else if (price < 10000)
				{
					unit = 10;
					if (!bInc && price == 5000) unit = 5;
				}
				else if (price < 50000)
				{
					unit = 50;
					if (!bInc && price == 10000) unit = 10;
				}
				else if (price < 100000)
				{
					unit = 100;
					if (!bInc && price == 50000) unit = 50;
				}
				else if (price < 500000)
				{
					unit = 500;
					if (!bInc && price == 100000) unit = 100;
				}
				else if (price >= 500000)
				{
					unit = 1000;
					if (!bInc && price == 500000) unit = 500;
				}
			}
			else	// KOSDAQ 주식
			{
				if (price < 1000)	// 2010.08.02 호가단위세분화 제도변경에 의한 수정
				{
					unit = 1;
					if (!bInc && price <= 1) unit = 0;
				}
				else if (price < 5000)
				{
					unit = 5;
					if (!bInc && price <= 5) unit = 0;
				}
				else if (price < 10000)
				{
					unit = 10;
					if (!bInc && price == 5000) unit = 5;
				}
				else if (price < 50000)
				{
					unit = 50;
					if (!bInc && price == 10000) unit = 10;
				}
				else if (price >= 50000)
				{
					unit = 100;
					if (!bInc && price == 50000) unit = 50;
				}
				break;
			}
		}
		break;

	case 2:	// 선물
		if		(spcd=="61") unit=0.01;	// KTB
		else if	(spcd=="63") unit=0.01;	// 5TB
		else if (spcd=="62") unit=0.01; // MSB
		else if (spcd=="64") unit=0.02;	// LKTB
		else if	(spcd=="65") unit=0.01;	// NKTB
		else if (spcd=="66") unit=0.01; // N5TB
		else if (spcd=="67") unit=0.01;	// NLKTB
		else if (spcd=="75") unit=0.1;	// USD
		else if (spcd=="76") unit=0.1;	// JPY
		else if (spcd=="77") unit=0.1;	// EUR
		else if (spcd=="85") unit=10;	// GOLD
		else if (spcd=="86") unit=5;	// LH
		else if (spcd=="87") unit=10;	// MiniGOLD
		else if (m_sCode.GetLength() > 3 && m_sCode.GetAt(2) == '3') unit = 0.5; // KOSTAR
		else unit = 0.05;
		break;
	case 6:	// KOFEX
		unit = 0.05;
		if (!bInc && price <= 0.05) unit = 0;
		break;
	case 3:	// 옵션
		if (m_bKospi)	// 옵션
		{
			if (price < 3)
			{
				unit = 0.01;
				if (!bInc && price <= 0.01) unit = 0;
			}
			else
			{
				unit = 0.05;
				if (!bInc && price == 3) unit = 0.01;
			}
		}
		else // KOSDAQ 옵션
		{
			if (price < 5)
			{
				unit = 0.01;
				if (!bInc && price <= 0.01) unit = 0;
			}
			else
			{
				unit = 0.05;
				if (!bInc && price == 5) unit = 0.01;
			}
		}
		break;
	case 5:	// 주식옵션
		if (price < 1000)
		{
			unit = 10;
			if (!bInc && price <= 10) unit = 0;
		}
		else if (price < 2000)
		{
			unit = 20;
			if (!bInc && price == 1000) unit = 10;
		}
		else if (price < 5000)
		{
			unit = 50;
			if (!bInc && price == 2000) unit = 20;
		}
		else if (price < 10000)
		{
			unit = 100;
			if (!bInc && price == 5000) unit = 50;
		}
		else if (price >= 10000)
		{
			unit = 200;
			if (!bInc && price == 10000) unit = 100;
		}
		break;
	case 7://KOSDAQ ETF 200740824
		if (price < 2000)
		{
			unit = 1;
			if (!bInc && price <= 1)
				unit = 0;
		}
		else
			unit = 5;
		break;
	case 8:	// kostar
		unit = 0.5;
		break;
	case 9:	// ELW
		unit = 5;	// 2010.08.02 일 호가단위제도 변경에 의해 ELW종목은 무조건 5원단위 호가 제공
		break;
	}
	return unit;
}

void CPricePopup::OnDestroy() 
{
	if (m_pVScroll)
	{
		if (::IsWindow(m_pVScroll->m_hWnd))
			m_pVScroll->DestroyWindow();
		delete m_pVScroll;
		m_pVScroll = NULL;
	}
	CWnd::OnDestroy();
}

void CPricePopup::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	SCROLLINFO info{};

	info.cbSize = sizeof(SCROLLINFO);
	m_pVScroll->GetScrollInfo(&info);

	if (nSBCode != SB_THUMBTRACK)
		nPos = info.nPos;
	switch (nSBCode)
	{
	case SB_LINEUP:		// move 1
		if (nPos == 0)
			nPos = 0;
		else
			nPos--;
		break;
	case SB_LINEDOWN:	// move 1
		nPos++;
		break;
	case SB_PAGEUP:
		if (nPos <= 1)
			nPos = 0;
		else
			nPos -= 2;
		break;
	case SB_PAGEDOWN:	// move nPage
		nPos += 2;
		break;
	case SB_THUMBTRACK:
		break;
	default:
		return;
		break;		
	}

	if (nPos >= (UINT)info.nMax)
		nPos = info.nMax - 1;
	info.nPos = nPos;
	m_pVScroll->SetScrollInfo(&info);
	m_startPos = nPos;
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);

	CRect	rc;

	GetClientRect(&rc);
	InvalidateRect(CRect(0, 36, rc.Width() - 15, rc.Height() - 20));
}

int CPricePopup::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CString spcd = m_sCode.Mid(1,2);
	switch (atoi(spcd))
	{
	case 85:
	case 86:
	case 87:
		m_bLhGd = true;
		break;
	default:
		m_bLhGd = false;
	}
	
	makeList();

	return 0;
}

CString CPricePopup::SetComma(CString src)
{
	CString temp, floatStr;

	if (src.Find('.') >= 0)
	{
		floatStr = src.Mid(src.Find('.'));
		src = src.Left(src.Find('.'));
	}
	else
		floatStr.Empty();
	
	temp = src;
	src.Empty();
	
	for (int ii = temp.GetLength() - 1, jj = -1; ii >= 0; ii--)
	{
		jj++;
		if (jj == 3)
		{
			if (temp.GetAt(0) != '-' && temp.GetAt(0) != '+')
				src = ',' + src;
			else if (ii != 0)
				src = ',' + src;
			
			jj = 0;
		}
		src = temp.GetAt(ii) + src;
	}
	src += floatStr;
	
	return src;
}

void CPricePopup::OnLButtonDown(UINT nFlags, CPoint point) 
{
	double tmp = 0;
	const	CRect	tRc = getPosition(point, &tmp);

	if (!tRc.IsRectEmpty() && m_selRc != tRc)
	{
		COLORREF clr = RGB(255,255,255);

		if (m_bDrag)
		{
			clr = RGB(128,255,255);
		}

		m_selRc.DeflateRect(0, 0, 1, 1);
		drawRectangle(m_selRc, clr);
		m_selRc.DeflateRect(1, 1, 1, 1);
		drawRectangle(m_selRc, clr);
		m_selRc = tRc;
		m_fSel = tmp;
		m_selRc.DeflateRect(0, 0, 1, 1);
		drawRectangle(m_selRc, GetSysColor(COLOR_HIGHLIGHT));
		m_selRc.DeflateRect(1, 1, 1, 1);
		drawRectangle(m_selRc, GetSysColor(COLOR_HIGHLIGHT));
		m_selRc.InflateRect(1, 1, 2, 2);

		if (m_bDrag)
		{
			m_fStart = tmp;
		}
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CPricePopup::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_pDoubleTmp = 0;
	if (getPosition(point, &m_pDoubleTmp) != CRect(0, 0, 0, 0))
	{
		getPosition(point, &m_pDoubleTmp);
		if (m_bDrag)
		{
			CString str;

			m_fEnd = m_pDoubleTmp;
			str.Format("%lf,%lf",m_fStart,m_fEnd);
			m_pParent->SendMessage(WM_USER + 1, 5, (LPARAM)str.GetString());
		}
		else
		{
			m_pParent->SendMessage(WM_USER + 1, 1, (LPARAM)&m_pDoubleTmp);
		}

		if (m_nType != TP_DBLCLK)
			PostMessage(WM_USER, 0, 0);	// Close
	}
	CWnd::OnLButtonUp(nFlags, point);
}

void CPricePopup::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_nType == TP_DBLCLK)
	{
		m_pParent->PostMessage(WM_USER + 1, 3, MAKELONG(point.x, point.y));
		PostMessage(WM_USER, 0, 0);	// Close
	}
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CPricePopup::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (nFlags & MK_LBUTTON)
	{
		double	tmp = 0;
		const	CRect	tRc = getPosition(point, &tmp);

		if (!tRc.IsRectEmpty() && m_selRc != tRc)
		{
			COLORREF clr = RGB(255,255,255);

			if (m_bDrag)
			{
				if (m_fSel > m_fStart && tmp > m_fStart)
				{
					if (m_fSel < tmp)
					{
						clr = RGB(128,255,255);
					}
				}
				else if (m_fSel < m_fStart && tmp < m_fStart)
				{
					if (m_fSel > tmp)
					{
						clr = RGB(128,255,255);
					}
				}
				else
				{
					if (m_fSel > m_fStart && tmp < m_fStart)
					{
						clr = RGB(128,255,255);
					}
					else if (m_fSel < m_fStart && tmp > m_fStart)
					{
						clr = RGB(128,255,255);
					}
				}

				if (m_fSel == m_fStart)
				{
					clr = RGB(128,255,255);
				}
			}

			m_selRc.DeflateRect(0, 0, 1, 1);
			drawRectangle(m_selRc, clr);
			m_selRc.DeflateRect(1, 1, 1, 1);
			drawRectangle(m_selRc, clr);
			m_selRc = tRc;
			m_fSel = tmp;
			m_selRc.DeflateRect(0, 0, 1, 1);
			drawRectangle(m_selRc, GetSysColor(COLOR_HIGHLIGHT));
			m_selRc.DeflateRect(1, 1, 1, 1);
			drawRectangle(m_selRc, GetSysColor(COLOR_HIGHLIGHT));
			m_selRc.InflateRect(1, 1, 2, 2);
		}

		CRect	rc;

		GetClientRect (&rc);
		rc.DeflateRect(0, 36, 0, 20);
		
		if (point.y >= rc.top && point.y <= rc.top + 10)
			if (m_pVScroll)
				SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), 0);
		else if (point.y >= rc.bottom - 10 && point.y <= rc.bottom)
			if (m_pVScroll)
				SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), 0);
	}

	if (GetFocus()->m_hWnd != m_hWnd)
		SetFocus();

	CWnd::OnMouseMove(nFlags, point);
}

CRect CPricePopup::getPosition(CPoint point, double *val)
{
	CRect	rc, tRc;
	CString sTmp;

	GetClientRect(&rc);
	rc.DeflateRect(0, 36, 0, 20);

	// 상한가
	tRc = rc;
	tRc.top -= 20;
	tRc.bottom = tRc.top + 20;

	if (tRc.PtInRect(point))
	{
		*val = m_dMax;
		return tRc;
	}

	// 하한가
	tRc = rc;
	tRc.top = rc.bottom;
	tRc.bottom += 20;

	if (tRc.PtInRect(point))
	{
		*val = m_dMin;
		return tRc;
	}

	const	int	nCnt = min(m_arPrice.GetSize() - 2, 11);
	for (int ii = 0; ii < nCnt; ii++)
	{
		tRc = rc;
		tRc.top = rc.top + ii * rc.Height() / nCnt;
		tRc.right = (m_pVScroll?rc.Width() - 15:rc.Width());
		tRc.bottom = rc.top + (ii + 1) * rc.Height() / nCnt;
				
		if (tRc.PtInRect(point))
		{
			*val = m_arPrice.GetAt(ii + m_startPos);
			return tRc;
		}
	}
	*val = 0;
	return CRect(0, 0, 0, 0);
}

void CPricePopup::drawRectangle(CRect rt, COLORREF clr)
{
	CDC	*pDC = GetDC();
	pDC->Draw3dRect(rt, clr, clr);
	ReleaseDC(pDC);
}

void CPricePopup::setColor(COLORREF upClr, COLORREF dnClr)
{
	m_upClr = upClr;
	m_dnClr = dnClr;
}

void CPricePopup::setCurrPosition()
{
	const	int	nCnt = min(m_arPrice.GetSize() - 2, 11);
	if (m_dCurr == m_dMax)
	{
		m_startPos = 0;
		return;
	}
	if (m_dCurr == m_dMin)
	{
		m_startPos = m_arPrice.GetSize() - nCnt;
		if (m_arPrice.GetSize() < 11)
			m_startPos = 0;
		return;
	}
	for (int ii = 0; ii < m_arPrice.GetSize(); ii++)
	{
		if (m_dCurr == m_arPrice.GetAt(ii))
		{
			m_startPos = ii - 5;
			if (m_startPos + nCnt > m_arPrice.GetSize())
				m_startPos = m_arPrice.GetSize() - nCnt;
			if (m_startPos < 0)
				m_startPos = 0;
		}
	}
}

BOOL CPricePopup::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (m_pVScroll)
	{
		SCROLLINFO info;
		m_pVScroll->GetScrollInfo(&info);
		if (info.nMax >= info.nPos)
		{
			if (zDelta < 0)
				SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEDOWN, 0), 0);
			else
				SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEUP, 0), 0);
		}
	}
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CPricePopup::setParent(CWnd *pWnd)
{
	m_pParent = pWnd;

	m_bDrag = (BOOL)((CControlWnd*)m_pParent)->GetDragMode();
}

CFont* CPricePopup::getAxFont(CString fName, int point)
{
	struct	_fontR	fontR{};

	fontR.name = (LPSTR)fName.GetString();
	fontR.point = point;
	fontR.italic = false;
	fontR.bold = 0;
	return (CFont*)m_pWizard->SendMessage(WM_USER, getFONT, (LPARAM)&fontR);
}

CPen* CPricePopup::getAxPen(COLORREF clr, int nWidth, int nStyle)
{
	struct	_penR	penR{};

	penR.clr = clr;
	penR.width = nWidth;
	penR.style = nStyle;
	return (CPen*)m_pWizard->SendMessage(WM_USER, getPEN, (LPARAM)&penR);
}

CString CPricePopup::LoadStringTb(UINT nIdx)
{
	CString str;
	str.LoadString(nIdx);
	return str;
}
