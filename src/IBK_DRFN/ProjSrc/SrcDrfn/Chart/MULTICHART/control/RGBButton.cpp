// RGBButton.cpp: implementation of the CRGBButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGBButton.h"
#include "../LeftBarDlg.h"

#define DISCOLOR_BORDER			RGB(128, 128, 128)
#define DISCOLOR_BACKGROUND		RGB(115, 115, 115)
#define DISCOLOR_TEXT			RGB(128, 128, 128)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CRGBButton

CRGBButton::CRGBButton()
{
	m_bTracking = FALSE;		// Botton_RGB 영역 내 존재시 마우스 Mouse Position Tracking 시작 (Now Tracking - TRUE / End Tracking - FALSE)
	m_bHover	= FALSE;		// Hovering 상태 (In - TRUE / Out - FALSE)

	m_bHasBorder = TRUE;
	m_bIsRoundRect = FALSE;
	m_pFont = NULL;
	m_szText.Empty();

	m_pSideBar = NULL;
	m_bSideBarRunning = FALSE;
}

CRGBButton::~CRGBButton()
{
}

BEGIN_MESSAGE_MAP(CRGBButton, CButton)
	//{{AFX_MSG_MAP(CRGBButton)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

void CRGBButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcClient = lpDrawItemStruct->rcItem;
	pDC->SetBkMode(TRANSPARENT);

	DrawButtonRect(pDC, rcClient, lpDrawItemStruct->itemState);
	DrawButtonText(pDC, rcClient, lpDrawItemStruct->itemState);
}

void CRGBButton::DrawButtonRect(CDC* pDC, CRect rcClient, UINT nItemState)
{
	CPen pen(PS_SOLID, 1, nItemState & ODS_DISABLED ? DISCOLOR_BORDER : m_clrBorder);
	CPen* pOldPen = pDC->SelectObject(&pen);

	if(m_bHasBorder) {

		if(m_bIsRoundRect) {				// 둥근 Border
			POINT pt;
			pt.x = 2; pt.y = 2;

			pDC->RoundRect(&rcClient, pt);
		}
		else
			pDC->Rectangle(&rcClient);
	}
		
	pen.DeleteObject();
	pDC->SelectObject(pOldPen);


	rcClient.DeflateRect(1, 1);

	COLORREF clrBackGround = m_clrBackGround;
	if(nItemState & ODS_DISABLED)
		clrBackGround = DISCOLOR_BACKGROUND;
	else if(nItemState & ODS_SELECTED)
		clrBackGround = m_clrBackGround_Sel;

	if(m_bIsRoundRect)
		rcClient.DeflateRect(1, 1);

	pDC->FillSolidRect(rcClient, clrBackGround);
}

void CRGBButton::DrawButtonText(CDC* pDC, CRect rcClient, UINT nItemState)
{
    COLORREF clrText = nItemState & ODS_DISABLED ? DISCOLOR_TEXT : m_clrText;
	COLORREF clrOldText = pDC->SetTextColor(clrText);	
	int nOldMode = pDC->SetBkMode(TRANSPARENT);

	CFont * pOldFont;
	if ( m_pFont ) 
		pOldFont = pDC->SelectObject(m_pFont);

//	if(nItemState & ODS_SELECTED)
//		rcClient.OffsetRect(1, 1);

	pDC->DrawText(m_szText, m_szText.GetLength(), rcClient, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	if ( m_pFont ) 
		pDC->SelectObject (pOldFont);
	pDC->SetTextColor(clrOldText);
	pDC->SetBkMode(nOldMode);
}

void CRGBButton::SetBorderColor(COLORREF clrBorder)
{
	m_clrBorder = clrBorder;
}

void CRGBButton::SetBackGroundColor(COLORREF clrBackGround, COLORREF clrBackGround_Sel)
{
	m_clrBackGround = clrBackGround;
	m_clrBackGround_Sel = clrBackGround_Sel;
}

void CRGBButton::SetTextColor(COLORREF clrText)
{
	m_clrText = clrText;
}

void CRGBButton::SetButtonText(LPCTSTR szText)
{
	m_szText = szText;
}

void CRGBButton::SetSideBar(CLeftBarDlg* pSideBar)
{
	m_pSideBar = pSideBar;
}

void CRGBButton::StartSideBar()
{
	if(m_bSideBarRunning == FALSE)
	{
		m_bSideBarRunning = TRUE;
		m_pSideBar->StartSideBar();
	}
}

void CRGBButton::EndSideBar()
{
	m_bSideBarRunning = FALSE;
}

BOOL CRGBButton::PreTranslateMessage(MSG* pMsg) 
{
//	InitToolTip();
//	m_ToolTip.RelayEvent(pMsg);
	
	return CButton::PreTranslateMessage(pMsg);
}

void CRGBButton::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}

//-------------------------------------------------------------
// 툴팁 생성

void CRGBButton::InitToolTip()
{
	if(m_ToolTip.m_hWnd == NULL)
	{
		m_ToolTip.Create(this);									// Create ToolTip Control
		m_ToolTip.Activate(FALSE);								// Inactivate
		m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 400);		// Enable Multiline
	}
}


//-------------------------------------------------------------
// 툴팁 텍스트


void CRGBButton::SetTooltipText(LPCTSTR lpszText, BOOL bActivate)
{
	// We cannot accept NULL pointer
	if (lpszText == NULL) return;

	// Initialize ToolTip
	InitToolTip();

	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, lpszText, rectBtn, 1);
	}

	// Set text for tooltip
	m_ToolTip.UpdateTipText(lpszText, this, 1);
	m_ToolTip.Activate(bActivate);
} // End of SetTooltipText


//-------------------------------------------------------------
// 툴팁 텍스트


void CRGBButton::SetTooltipText(int nId, BOOL bActivate)
{
	CString sText;

	// load string resource
	sText.LoadString(nId);
	// If string resource is not empty
	if (sText.IsEmpty() == FALSE) SetTooltipText((LPCTSTR)sText, bActivate);
} // End of SetTooltipText

/////////////////////////////////////////////////////////////////////////////
// CRGBButton message handlers

void CRGBButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( m_bTracking == FALSE )
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;		// Botton_Bitmap 영역 감지 (In / Out flag)
		tme.dwHoverTime = 1;					// Hovering 감지 시간차
		m_bTracking = _TrackMouseEvent(&tme);
	}

	CButton::OnMouseMove(nFlags, point);
}

LRESULT CRGBButton::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	if(!m_bHover && m_pSideBar && m_pSideBar->GetSafeHwnd())
	{
		m_bHover = TRUE;

		//@Solomon-alzioyes:슬라이드 기능을 빼달라는 요청반영.
		//StartSideBar();
	}
	Invalidate();
	return 0;
}

LRESULT CRGBButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if(m_bHover && m_pSideBar && m_pSideBar->GetSafeHwnd())
	{
		m_bHover = FALSE;
	}
	m_bTracking = FALSE;
	Invalidate();
	return 0;
}
