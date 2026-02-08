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

/////////////////////////////////////////////////////////////////////////////
// CAmountPopup

CAmountPopup::CAmountPopup()
{
	m_bDrag = false;
	m_nStatus = 1;
	m_selRt = CRect(0, 0, 0, 0);

	m_crBlack = RGB(0, 0, 0);
	
	m_crWhite = RGB(255, 255, 255);
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

	m_profile.Format(CONFIG_INI, Variant(homeCC,""), Variant(nameCC,""));

	m_pFont = getAxFont("굴립체",9);

	CRect rc;

	this->GetClientRect(&rc);

	rc.DeflateRect(0,16,0,0);

	//rc.right = rc.left + 400;

	m_pJcntGrid = new CGridCtrl();
	m_pJcntGrid->SetNoScrollBar(TRUE);
	m_pJcntGrid->Create(rc, this, 0);
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

	LoadConfig();
	
	//m_bInit = TRUE;

	//m_pJcntGrid->SetFocus();
	
	return 0;
}

void CAmountPopup::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	CRect rc;

	GetClientRect(&rc);

	rc.right = rc.left + 400;
	rc.top = rc.top + 16;
	
	m_pJcntGrid->MoveWindow(rc);

	int row = m_pJcntGrid->GetRowCount();
	int col = m_pJcntGrid->GetColumnCount();

	int n=0;

	m_pJcntGrid->SetNoScrollBar(TRUE);

	for (n=0; n<col; ++n)
		m_pJcntGrid->SetColumnWidth(n, rc.Width()/col);
	for (n=0; n<row; ++n)
		m_pJcntGrid->SetRowHeight(n, rc.Height()/3);
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
	if (wParam==0)
	{
		*pResult = 1;
        GV_DISPINFO *pi = (GV_DISPINFO*)lParam;
		if (pi->hdr.code == GVN_BEGINLABELEDIT)
		{
			// 편집 시작하면 현재 값을 저장
			oldVal = m_pJcntGrid->GetItemText(pi->item.row, pi->item.col);
		}
        else if (pi->hdr.code == GVN_ENDLABELEDIT || pi->hdr.code == GVN_SELCHANGED)
		{
			CCellID cId(pi->item.row, pi->item.col);
			CString key, val = m_pJcntGrid->GetItemText(cId.row, cId.col);
			val.Trim();
			
			if (pi->hdr.code==GVN_ENDLABELEDIT)
			{
				// 공백이면 원래값으로 되돌린다.
				if (val.GetLength()>0)
				{
					key.Format("MA_%d_%d", cId.row, cId.col);
					WritePrivateProfileString("JQTY_GRID", (LPCSTR)key, (LPCSTR)val, (LPCSTR)m_profile);
				} 
				else 
				{
					val = oldVal;
					m_pJcntGrid->SetItemText(cId.row, cId.col, oldVal);
				}
			}

			m_pParent->SendMessage(WM_USER + 1, 1, atoi(val));
			
			for (int row=0; row<m_pJcntGrid->GetRowCount(); ++row)
			{
				for (int col=0; col<m_pJcntGrid->GetColumnCount(); ++col)
				{
					if (pi->item.row==row && pi->item.col==col)
					{
						m_pJcntGrid->SetItemBkColour(row, col, ::GetSysColor(COLOR_HIGHLIGHT));
						m_pJcntGrid->SetItemFgColour(row, col, ::GetSysColor(COLOR_HIGHLIGHTTEXT));
					}
					else
					{
						m_pJcntGrid->SetItemBkColour(row, col, m_crWhite);
						m_pJcntGrid->SetItemFgColour(row, col, m_crBlack);
					}
				}
			}
			m_pJcntGrid->Invalidate(FALSE);
		}
	}
	
	return CWnd::OnNotify(wParam, lParam, pResult);
}

void CAmountPopup::SetJcnt( int jcnt )
{
	if (jcnt<=0) return;
	
	for (int row=0; row<m_pJcntGrid->GetRowCount(); ++row)
	{
		for (int col=0; col<m_pJcntGrid->GetColumnCount(); ++col)
		{
			m_pJcntGrid->SetItemBkColour(row, col, m_crWhite);
			m_pJcntGrid->SetItemFgColour(row, col, m_crBlack);
		}
	}
	m_pJcntGrid->Invalidate(FALSE);
}

void CAmountPopup::LoadConfig()
{
	int def_val[][10] = {{10, 20, 30, 40, 50, 60, 70, 80, 90, 100},{100, 200, 300, 400, 500, 600, 700, 800, 900, 1000},{1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000}};
	
	int ival;
	CString key, sval;
	for (int row=0; row<3; ++row)
	{
		for (int col=0; col<10; ++col)
		{
			key.Format("MA_%d_%d", row, col);
			ival = GetPrivateProfileInt("JQTY_GRID", (LPCSTR)key, def_val[row][col], (LPCSTR)m_profile);
			sval.Format("%d", ival);
			m_pJcntGrid->SetItemText(row, col, (LPCSTR)sval );
		}
	}
}

void CAmountPopup::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);

	MSG msg;
	bool bSendChangeEvent = false;
	for (m_fDone = FALSE; !m_fDone; WaitMessage())
	{
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if ((msg.message == WM_KILLFOCUS && msg.hwnd == this->m_hWnd) || 
				((msg.message == WM_LBUTTONDOWN || msg.message == WM_NCLBUTTONDOWN) && msg.hwnd != this->m_hWnd && msg.hwnd != m_pJcntGrid->m_hWnd) ||
				(msg.message == WM_USER && msg.hwnd == this->m_hWnd && msg.hwnd != m_pJcntGrid->m_hWnd))
			{
				if ((msg.message == WM_LBUTTONDOWN) && msg.hwnd != this->m_hWnd)
				{
					m_pParent->SendMessage(WM_USER + 1, 2, NULL);
					bSendChangeEvent = true;
				}
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

	ShowWindow(SW_HIDE);
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

LOGFONT CAmountPopup::setFont(CString fName, int size, bool isItalic, bool isBold)
{
	LOGFONT	lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = size;
	if (fName.IsEmpty())
		fName = LoadStringTb(IDS_BASEFONTNAME5);
	
	strcpy(lf.lfFaceName, fName);   
	
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;

	lf.lfWeight = (isBold? FW_BOLD : FW_NORMAL);
	lf.lfItalic = (isItalic? TRUE : FALSE);
	return lf;
}

void CAmountPopup::drawCaption(CDC *pDC)
{
	CRect rc;
	
	GetClientRect(&rc);
	rc.bottom = rc.top + 16;

	pDC->FillSolidRect(rc, GetSysColor(COLOR_HIGHLIGHT));

	rc.DeflateRect(2, 0, 16, 0);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	pDC->DrawText(LoadStringTb(IDS_SELECTAMTSTR), rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}

void CAmountPopup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	CPen *pOldPen;
	CFont	*pOldFont;

	pOldFont = (CFont*)dc.SelectObject(getAxFont("굴림", 9));

	drawCaption(&dc);
	//drawData(&dc);

	pOldPen = (CPen*)dc.SelectObject(getAxPen(GetSysColor(COLOR_HIGHLIGHTTEXT), 1, PS_SOLID));

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

void CAmountPopup::setPathInfo(CString sRoot, CString sUser)
{
	m_sRoot = sRoot;
	m_sUserID = sUser;
}

LRESULT CAmountPopup::OnNcHitTest(CPoint point)
{
	UINT nHitTest = CWnd::OnNcHitTest(point);
	
	CRect rc;
	GetWindowRect(&rc);
	rc.bottom = rc.top + 16;
	if (nHitTest == HTCLIENT && rc.PtInRect(point))
		nHitTest = HTCAPTION;
	
	return nHitTest;
}

void CAmountPopup::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	CRect rc;
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

void CAmountPopup::drawData(CDC *pDC)
{
// 	CRect rc;
// 	GetClientRect(&rc);
// 	rc.DeflateRect(0, 16, 0, 0);
// 	pDC->SetTextColor(RGB(0, 0, 0));
// 
// 	switch (m_nStatus)
// 	{
// 	case 1:		// 현물
// 	case 4:		// ECN
// 		{
// 			int nBase = 10;
// 			for (int yy = 0; yy < 3; yy++)
// 			{
// 				for (int xx = 0; xx < 10; xx++)
// 				{
// 					CRect tRc = rc;
// 					CString sTmp;
// 					tRc.left = xx * rc.Width() / 10;
// 					tRc.top = rc.top + yy * rc.Height() / 3;
// 					tRc.right = (xx + 1) * rc.Width() / 10;
// 					tRc.bottom = rc.top + (yy + 1) * rc.Height() / 3;
// 					
// 					pDC->FillSolidRect(tRc, RGB(255, 255, 255));
// 					tRc.InflateRect(1, 1);
// 					pDC->Draw3dRect(tRc, RGB(0, 0, 0), RGB(0, 0, 0));
// 					tRc.DeflateRect(4, 4, 4, 4);
// 					
// 					sTmp.Format("%d", nBase * (xx + 1));
// 					pDC->DrawText(sTmp, tRc, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
// 				}
// 				nBase *= 10;
// 			}
// 		}
// 		break;
// 	case 2:		// 선물
// 	case 6:		// KOFEX
// 		{
// 			int nBase = 1, nGap = 0;
// 			for (int yy = 0; yy < 4; yy++)
// 			{
// 				for (int xx = 0; xx < 10; xx++)
// 				{
// 					CRect tRc = rc;
// 					CString sTmp;
// 					tRc.left = xx * rc.Width() / 10;
// 					tRc.top = rc.top + yy * rc.Height() / 4;
// 					tRc.right = (xx + 1) * rc.Width() / 10;
// 					tRc.bottom = rc.top + (yy + 1) * rc.Height() / 4;
// 					
// 					pDC->FillSolidRect(tRc, RGB(255, 255, 255));
// 					tRc.InflateRect(1, 1);
// 					pDC->Draw3dRect(tRc, RGB(0, 0, 0), RGB(0, 0, 0));
// 					tRc.DeflateRect(4, 4, 4, 4);
// 					
// 					//if (nBase * (xx + 1) + nGap >= 1000)
// 					//	sTmp = "999";
// 					//else
// 						sTmp.Format("%d", nBase * (xx + 1) + nGap);
// 					pDC->DrawText(sTmp, tRc, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
// 				}
// 				if (yy == 0){nBase = 10;	nGap = 0;}
// 				else if (yy == 1){nBase = 10;	nGap = 5;}
// 				else if (yy == 2){nBase = 100;	nGap = 0;}
// 			}
// 		}
// 		break;
// 	case 3:		// 옵션
// 	case 5:		// 주식옵션
// 		{
// 			int nBase = 1, nGap = 0;
// 			for (int yy = 0; yy < 5; yy++)
// 			{
// 				CString sTmp;
// 				for (int xx = 0; xx < 10; xx++)
// 				{
// 					CRect tRc = rc;
// 					tRc.left = xx * rc.Width() / 10;
// 					tRc.top = rc.top + yy * rc.Height() / 5;
// 					tRc.right = (xx + 1) * rc.Width() / 10;
// 					tRc.bottom = rc.top + (yy + 1) * rc.Height() / 5;
// 					
// 					pDC->FillSolidRect(tRc, RGB(255, 255, 255));
// 					tRc.InflateRect(1, 1);
// 					pDC->Draw3dRect(tRc, RGB(0, 0, 0), RGB(0, 0, 0));
// 					tRc.DeflateRect(4, 4, 4, 4);
// 					
// 					if (sTmp == "4999")
// 						sTmp = "5000";
// 					else if (nBase * (xx + 1) + nGap >= 5000)
// 						sTmp = "4999";
// 					else
// 						sTmp.Format("%d", nBase * (xx + 1) + nGap);
// 					pDC->DrawText(sTmp, tRc, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
// 				}
// 				if (yy == 0){nBase = 10;	nGap = 0;}
// 				else if (yy == 1){nBase = 10;	nGap = 5;}
// 				else if (yy == 2){nBase = 100;	nGap = 0;}
// 				else if (yy == 3){nBase = 500;	nGap = 500;}
// 			}
// 		}
// 		break;
// 	}
}

void CAmountPopup::drawRectangle(CRect rt, COLORREF clr)
{
	CDC	*pDC = GetDC();
	pDC->Draw3dRect(rt, clr, clr);
	ReleaseDC(pDC);
}

CRect CAmountPopup::getPosition(CPoint point, int &val)
{
// 	CRect rc;
// 	GetClientRect(&rc);
// 	rc.DeflateRect(0, 16, 0, 0);
// 
// 	switch (m_nStatus)
// 	{
// 	case 1:		// 주식
// 	case 4:		// ECN
// 		{
// 			int nBase = 10;
// 			for (int yy = 0; yy < 3; yy++)
// 			{
// 				for (int xx = 0; xx < 10; xx++)
// 				{
// 					CRect tRc = rc;
// 					CString sTmp;
// 					tRc.left = xx * rc.Width() / 10;
// 					tRc.top = rc.top + yy * rc.Height() / 3;
// 					tRc.right = (xx + 1) * rc.Width() / 10;
// 					if (xx == 9)
// 						tRc.right++;	// because line span
// 					tRc.bottom = rc.top + (yy + 1) * rc.Height() / 3;
// 					if (yy == 2)
// 						tRc.bottom++;	// because line span
// 					if (tRc.PtInRect(point))
// 					{
// 						val = nBase * (xx + 1);
// 						return tRc;
// 					}
// 				}
// 				nBase *= 10;
// 			}
// 		}
// 		break;
// 	case 2:		// 선물
// 	case 6:		// KOFEX
// 		{
// 			int nBase = 1, nGap = 0;
// 			for (int yy = 0; yy < 4; yy++)
// 			{
// 				for (int xx = 0; xx < 10; xx++)
// 				{
// 					CRect tRc = rc;
// 					CString sTmp;
// 					tRc.left = xx * rc.Width() / 10;
// 					tRc.top = rc.top + yy * rc.Height() / 4;
// 					tRc.right = ((xx + 1) * rc.Width()) / 10;
// 					if (xx == 9)
// 						tRc.right++;	// because line span
// 					tRc.bottom = rc.top + (yy + 1) * rc.Height() / 4;
// 					if (yy == 3)
// 						tRc.bottom++;	// because line span
// 					
// 					if (tRc.PtInRect(point))
// 					{
// 						val = nBase * (xx + 1) + nGap;
// 						return tRc;
// 					}
// 				}
// 				if (yy == 0){nBase = 10;	nGap = 0;}
// 				else if (yy == 1){nBase = 10;	nGap = 5;}
// 				else if (yy == 2){nBase = 100;	nGap = 0;}
// 			}
// 		}
// 		break;
// 	case 3:		// 옵션
// 	case 5:		// 주식옵션
// 		{
// 			int nBase = 1, nGap = 0;
// 			for (int yy = 0; yy < 5; yy++)
// 			{
// 				for (int xx = 0; xx < 10; xx++)
// 				{
// 					CRect tRc = rc;
// 					CString sTmp;
// 					tRc.left = xx * rc.Width() / 10;
// 					tRc.top = rc.top + yy * rc.Height() / 5;
// 					tRc.right = (xx + 1) * rc.Width() / 10;
// 					if (xx == 9)
// 						tRc.right++;	// because line span
// 					tRc.bottom = rc.top + (yy + 1) * rc.Height() / 5;
// 					if (yy == 4)
// 						tRc.bottom++;	// because line span
// 					
// 					if (tRc.PtInRect(point))
// 					{
// 						if (nBase * (xx + 1) + nGap >= 5000)
// 						{
// 							if (xx == 9)
// 								val = 5000;
// 							else
// 								val = 4999;
// 						}else
// 							val = nBase * (xx + 1) + nGap;
// 						return tRc;
// 					}
// 				}
// 				if (yy == 0){nBase = 10;	nGap = 0;}
// 				else if (yy == 1){nBase = 10;	nGap = 5;}
// 				else if (yy == 2){nBase = 100;	nGap = 0;}
// 				else if (yy == 3){nBase = 500;	nGap = 500;}
// 			}
// 		}
// 		break;
// 	}
// 	val = 0;
 	return CRect(0, 0, 0, 0);
}

void CAmountPopup::setParent(CWnd *pWnd)
{
	m_pParent = pWnd;
}

CFont* CAmountPopup::getAxFont(CString fName, int point)
{
	struct _fontR fontR;
	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point;
	fontR.italic = false;
	fontR.bold = 0;
	return (CFont*)m_pWizard->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CPen* CAmountPopup::getAxPen(COLORREF clr, int nWidth, int nStyle)
{
	struct _penR penR;
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
