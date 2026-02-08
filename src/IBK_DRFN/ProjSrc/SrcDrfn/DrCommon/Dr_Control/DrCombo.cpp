// DrCombo.cpp : implementation file
//

#include "stdafx.h"
#include "DrCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrCombo
#define CLR_DISABLE		RGB(215,216,218)
#define CLR_BORDER		RGB(106,106,106)
#define CLR_BTN_NORMAL	RGB(255,255,255)
#define CLR_BTN_CLICK   RGB(212,212,212)
#define CLR_BTN_DISABLE RGB(212,212,212)

CDrCombo::CDrCombo()
{
	m_bSetFocus = FALSE;
	m_bLButtonDown = FALSE;
}

CDrCombo::~CDrCombo()
{
}


BEGIN_MESSAGE_MAP(CDrCombo, CComboBox)
	//{{AFX_MSG_MAP(CDrCombo)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_CONTROL_REFLECT(CBN_SETFOCUS, OnSetfocus)
	ON_CONTROL_REFLECT(CBN_KILLFOCUS, OnKillfocus)
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
void CDrCombo::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect r;
	GetClientRect(&r);
	
	CDC memDC;
    memDC.CreateCompatibleDC(&dc);
	
	CBitmap m_bitmap, *m_pOldBitmap;

    m_bitmap.CreateCompatibleBitmap(&dc, r.Width(), r.Height());
	m_pOldBitmap = memDC.SelectObject(&m_bitmap);

	memDC.FillSolidRect(r, IsWindowEnabled() ? RGB(255,255,255): CLR_DISABLE);

	if(IsWindowEnabled())
 		CComboBox::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
	else
	{
		CFont* oldfont, * pFont = GetFont();
		memDC.SetTextColor(RGB(0,0,0));
		CString str;
		if(GetCurSel()>=0)
			GetLBText(GetCurSel(), str);

		oldfont = memDC.SelectObject(pFont);
		CRect rcText = r;
		rcText.DeflateRect(4, 0, 17, 0);
		memDC.DrawText(str, rcText, DT_LEFT |DT_SINGLELINE | DT_VCENTER); 
		memDC.SelectObject(oldfont);
	}

	DrawComboBox(&memDC);

	dc.BitBlt(r.left, r.top, r.Width(), r.Height(), &memDC, r.left, r.top, SRCCOPY);

    memDC.SelectObject(m_pOldBitmap);
	m_bitmap.DeleteObject();

	memDC.DeleteDC();

	// CComboBox::OnPaint() 호출하지 말것
}

void CDrCombo::DrawDownArrow(CDC* pDC, CRect rcArrow)
{
	BOOL bEnable =  IsWindowEnabled();

	CPen penLine(PS_SOLID, 1, RGB(179,179,179)), *pOldPen;
	CPen penCLine(PS_SOLID, 1, RGB(138,138,138));
	CPen penArrow(PS_SOLID, 1, RGB(91,91,91));
	CPen penDArrow(PS_SOLID, 1, RGB(179,179,179));

	pOldPen = pDC->SelectObject(&penLine);

	pDC->MoveTo(rcArrow.TopLeft());
	pDC->LineTo(rcArrow.left, rcArrow.bottom);

	if(bEnable)
	{
		if(m_bLButtonDown)
		{
			pDC->SelectObject(&penCLine);
			pDC->MoveTo(rcArrow.right-1, rcArrow.top);
			pDC->LineTo(rcArrow.left+1, rcArrow.top);
			pDC->LineTo(rcArrow.left+1, rcArrow.bottom);
		}
		
		pDC->SelectObject(&penArrow);
	}
	else
		pDC->SelectObject(&penDArrow);


	int x = (rcArrow.left + rcArrow.right - 7) / 2 + 1;
	int xx = x + 7;
	int y = (rcArrow.top + rcArrow.bottom - 5) / 2;
	if(m_bLButtonDown) y++;

	for(int i = 0; i < 5; i++)
	{
		pDC->MoveTo(x, y);
		pDC->LineTo(xx, y);
		y++;

		if(i == 0) continue;
		x++;
		xx--;
	}
	pDC->SelectObject(&pOldPen);
}

void CDrCombo::DrawComboBox(CDC* pDC)
{
	CRect rcItem;
	GetClientRect( &rcItem );

	// 테두리 그리기
	pDC->Draw3dRect(rcItem, CLR_BORDER, CLR_BORDER);
	rcItem.DeflateRect( 1,1 );

	COLORREF clrLine =  IsWindowEnabled() ? RGB(255,255,255): CLR_DISABLE;

	pDC->Draw3dRect( rcItem,clrLine,clrLine);

	// 콤보의 버튼 그리기
	int nXScrollBox = GetSystemMetrics(SM_CXHSCROLL);
	rcItem.DeflateRect(1,1);
	rcItem.left = rcItem.right - nXScrollBox;
	pDC->FillSolidRect(rcItem, clrLine);
	rcItem.left++;
	pDC->FillSolidRect(rcItem, m_bLButtonDown ? CLR_BTN_CLICK:CLR_BTN_NORMAL);
	
	DrawDownArrow(pDC, rcItem);
}


BOOL CDrCombo::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;//CComboBox::OnEraseBkgnd(pDC);
}

void CDrCombo::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLButtonDown = TRUE;
	Invalidate();

	CComboBox::OnLButtonDown(nFlags, point);
}

void CDrCombo::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLButtonDown = FALSE;
	CComboBox::OnLButtonUp(nFlags, point);
	Invalidate();

}

void CDrCombo::OnSetfocus() 
{
	m_bSetFocus = TRUE;
	Invalidate();
}

void CDrCombo::OnKillfocus() 
{
	m_bSetFocus = FALSE;
	Invalidate();
}

void CDrCombo::OnCloseup() 
{
	// TODO: Add your control notification handler code here
	m_bLButtonDown = FALSE;
	Invalidate();
}
