// XListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AxisCode.h"
#include "XListCtrl.h"

UINT NEAR WM_XLISTCTRL_EDITBOX_CHANGE = ::RegisterWindowMessage(_T("WM_XLISTCTRL_EDITBOX_CHANGE"));
UINT NEAR WM_XLISTCTRL_DATEBOX_CHANGE = ::RegisterWindowMessage(_T("WM_XLISTCTRL_DATEBOX_CHANGE"));
UINT NEAR WM_XLISTCTRL_COMBOBOX_CHANGE = ::RegisterWindowMessage(_T("WM_XLISTCTRL_COMBOBOX_CHANGE"));
UINT NEAR WM_XLISTCTRL_CHECKBOX_CLICKING = ::RegisterWindowMessage(_T("WM_XLISTCTRL_CHECKBOX_CLICKING"));
// CXListCtrl

IMPLEMENT_DYNAMIC(CXListCtrl, CListCtrl)

CXListCtrl::CXListCtrl()
{
	m_nEditItem				= -1;
	m_nEditSubItem				= -1;

	m_nDateItem				= -1;
	m_nDateSubItem			= -1;

	m_dwExtendedStyleX			 = 0;
	m_bHeaderIsSubclassed		 = FALSE;

	m_cr3DFace              = ::GetSysColor(COLOR_3DFACE);
	m_cr3DHighLight         = ::GetSysColor(COLOR_3DHIGHLIGHT);
	m_cr3DShadow            = ::GetSysColor(COLOR_3DSHADOW);
	m_crBtnFace             = RGB(240,240,240);//::GetSysColor(COLOR_BTNFACE);
	m_crBtnShadow           = ::GetSysColor(COLOR_BTNSHADOW);
	m_crBtnText             = ::GetSysColor(COLOR_BTNTEXT);
	m_crGrayText            = ::GetSysColor(COLOR_GRAYTEXT);
	m_crHighLight           = RGB(245,245,245);//::GetSysColor(COLOR_HIGHLIGHT);//RGB(220,220,250);//
	m_crHighLightText       = RGB(140,140,140);//::GetSysColor(COLOR_HIGHLIGHTTEXT);//RGB(0,0,0);//
	m_crWindow              = RGB(255,255,255);//::GetSysColor(COLOR_WINDOW);
	m_crWindowText          = ::GetSysColor(COLOR_WINDOWTEXT);

	m_bFocusRect			= TRUE;
	m_bResize				= TRUE;
	m_strNoItemMsg			= _T("There are no items to show in this view.");
	m_nStatusColumn			= -1;
	m_crBgProgress			= ::GetSysColor(COLOR_WINDOW);/*::GetSysColor(COLOR_BTNFACE);*/
	m_bUnderLine			= TRUE;
	m_crUnderLine			= RGB(0,0,0);
	m_bHeaderTracking		= FALSE;
	m_bHeaderDraging		= FALSE;
	
	m_bTracking				= FALSE;
	m_nTrackingItem			= -1;
	m_crTracking			= RGB(230,250,230);

	m_pTextFont				= NULL;
}

CXListCtrl::~CXListCtrl()
{
	#ifndef DO_NOT_INCLUDE_XCOMBOLIST
//	CXComboBox::DeleteInstance();
#endif
//	CXEditBox::DeleteInstance();
//	CXDateBox::DeleteInstance();
}


BEGIN_MESSAGE_MAP(CXListCtrl, CListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CXListCtrl::OnCustomDraw)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
//	ON_WM_MOUSEMOVE()
ON_WM_MOUSELEAVE()
ON_WM_NCPAINT()
ON_WM_PAINT()
END_MESSAGE_MAP()



// CXListCtrl message handlers


void CXListCtrl::Sort(int nSubItem, BOOL bSort)
{
}

void CXListCtrl::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	CListCtrl::PreSubclassWindow();

	SubclassHeaderControl();
}


BOOL CXListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class

	return CListCtrl::OnNotify(wParam, lParam, pResult);
}


BOOL CXListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CListCtrl::PreTranslateMessage(pMsg);
}

int CXListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	SubclassHeaderControl();
	return 0;
}

void CXListCtrl::SubclassHeaderControl()
{
	if (m_bHeaderIsSubclassed)
		return;

	CHeaderCtrl* pHeader = GetHeaderCtrl();
	if (pHeader)
	{
		VERIFY(m_HeaderCtrl.SubclassWindow(pHeader->m_hWnd));
		m_bHeaderIsSubclassed = TRUE;
	}
}

void CXListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	int nItem = static_cast<int> (pLVCD->nmcd.dwItemSpec);
	int nSubItem = pLVCD->iSubItem;

	*pResult = CDRF_DODEFAULT;

	if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		// This is the notification message for an item.  We'll request
		// notifications before each subitem's prepaint stage.

		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
	{
		*pResult = CDRF_SKIPDEFAULT;	// We've painted everything.

		XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) pLVCD->nmcd.lItemlParam;
		if( !pXLCD )
			return;

		COLORREF crText  = pXLCD[nSubItem].crText;
		COLORREF crBkgnd = pXLCD[nSubItem].crBackground;

		if( !pXLCD[0].bEnabled )
			crText = m_crGrayText;

		// store the colors back in the NMLVCUSTOMDRAW struct
		pLVCD->clrText = crText;
		pLVCD->clrTextBk = crBkgnd;

		CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
		CRect rect;
	//	GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);

		// Progress
		if( pXLCD && pXLCD[nSubItem].bShowProgress )
			TRACE(_T("progress\n"));
	//		DrawProgress(nItem, nSubItem, pDC, crText, crBkgnd, rect, pXLCD);
		// CheckBox
	//	else if( pXLCD && (pXLCD[nSubItem].nCheckedState!=-1) )
	//		DrawCheckbox(nItem, nSubItem, pDC, crText, crBkgnd, rect, pXLCD);
		// ComboBox
#ifndef DO_NOT_INCLUDE_XCOMBOLIST
	//	else if( pXLCD && pXLCD[nSubItem].bCombo )
	//		DrawText(nItem, nSubItem, pDC, crText, crBkgnd, rect, pXLCD);
#endif
		// EditBox
		else if( pXLCD && pXLCD[nSubItem].bEditBox )
			DrawText(nItem, nSubItem, pDC, crText, crBkgnd, rect, pXLCD);
		// DateBox
	//	else if( pXLCD && pXLCD[nSubItem].bDateBox )
	//		DrawText(nItem, nSubItem, pDC, crText, crBkgnd, rect, pXLCD);		
		// Default
	//	else
		{
	//		rect.left += DrawImage(nItem, nSubItem, pDC, crText, crBkgnd, rect, pXLCD);
	//		DrawText(nItem, nSubItem, pDC, crText, crBkgnd, rect, pXLCD);
		}
	}
}

void CXListCtrl::DrawText(int nItem, 
						  int nSubItem,
						  CDC *pDC,
						  COLORREF crText,
						  COLORREF crBkgnd,
						  CRect& rect,
						  XLISTCTRLDATA *pXLCD)
{
	ASSERT(pDC);
	ASSERT(pXLCD);

	CRect rcNew;
	rcNew = rect;

	CFont *pOldFont = pDC->SelectObject(m_pTextFont);
	CPen newPen(PS_SOLID, 1, m_crUnderLine);
	CPen *pOldPen = pDC->SelectObject(&newPen);


	if( nSubItem == m_nStatusColumn )
	{
		COLORREF crTemp = m_crWindow;
		GetDrawColors(nItem, nSubItem, crText, crTemp);

		pDC->FillSolidRect(&rect, crTemp);
		rcNew.top	+= 1;
		rcNew.left	+= 2;
		rcNew.right	-= 2;
		rcNew.bottom-= 2;
		pDC->FillSolidRect(&rcNew, crBkgnd);
	}
	else
	{
		GetDrawColors(nItem, nSubItem, crText, crBkgnd);
		pDC->FillSolidRect(&rect, crBkgnd);
	}

	CString str(_T(""));
	str = GetItemText(nItem, nSubItem);	
	if (!str.IsEmpty())
	{
		// get text justification
		HDITEM hditem;
		hditem.mask = HDI_FORMAT;
		m_HeaderCtrl.GetItem(nSubItem, &hditem);
		int nFmt = hditem.fmt & HDF_JUSTIFYMASK;
		UINT nFormat = DT_VCENTER | DT_SINGLELINE;
		if (nFmt == HDF_CENTER)
		{
			nFormat |= DT_CENTER;
		}
		else if (nFmt == HDF_LEFT)
		{
			nFormat |= DT_LEFT;
			rcNew.left += 4;
		}
		else
		{
			nFormat |= DT_RIGHT;
		}

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(crText);
		pDC->SetBkColor(crBkgnd);
		rcNew.OffsetRect(0, 1);
		pDC->DrawText(str, &rcNew, nFormat);
	}

	if( m_bUnderLine )
	{
		pDC->MoveTo( rect.left-1, rect.bottom );
		pDC->LineTo( rect.right+1, rect.bottom );
	}

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();

}

void CXListCtrl::GetDrawColors(int nItem,
							   int nSubItem,
							   COLORREF& colorText,
							   COLORREF& colorBkgnd)
{
	DWORD dwStyle    = GetStyle();
	DWORD dwExStyle  = GetExtendedStyle();

	COLORREF crText  = colorText;
	COLORREF crBkgnd = colorBkgnd;

	if (GetItemState(nItem, LVIS_SELECTED) && m_bFocusRect)
	{
		if (dwExStyle & LVS_EX_FULLROWSELECT)
		{
			// selected?  if so, draw highlight background
			crText  = m_crHighLightText;
			crBkgnd = m_crHighLight;

			// has focus?  if not, draw gray background
			if (m_hWnd != ::GetFocus())
			{
				if (dwStyle & LVS_SHOWSELALWAYS)
				{
					crText  = m_crWindowText;
					crBkgnd = m_crBtnFace;
				}
				else
				{
					crText  = colorText;
					crBkgnd = colorBkgnd;
				}
			}
		}
	}

	// 해당 Item이 Tracking Item과 동일하면 배경 색을 Tracking Color로 저장한다.
	if( m_nTrackingItem >= 0 )
	{
		if( nItem == m_nTrackingItem )
			crBkgnd = m_crTracking;
	}

	colorText = crText;
	colorBkgnd = crBkgnd;
}


BOOL CXListCtrl::GetSubItemRect(int nItem,  int nSubItem, int nArea, CRect& rect)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	BOOL bRC = CListCtrl::GetSubItemRect(nItem, nSubItem, nArea, rect);
	// if nSubItem == 0, the rect returned by CListCtrl::GetSubItemRect
	// is the entire row, so use left edge of second subitem
	if (nSubItem == 0)
	{
		if (GetColumns() > 1)
		{
			CRect rect1;
			bRC = GetSubItemRect(nItem, 1, LVIR_BOUNDS, rect1);
			rect.right = rect1.left;
		}
	}

	// 마우스가 올라왔을때 Grid line이 지워지는 문제를 보완
	rect.bottom -= 1;

	return bRC;

}

int CXListCtrl::GetColumns()
{
	return GetHeaderCtrl()->GetItemCount();
}

INT_PTR CXListCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	// TODO: Add your specialized code here and/or call the base class

	return CListCtrl::OnToolHitTest(point, pTI);
}





void CXListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CXListCtrl::OnDestroy()
{
	CListCtrl::OnDestroy();

	// TODO: Add your message handler code here
}


BOOL CXListCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	#if 1
		CRect clip;
		m_HeaderCtrl.GetWindowRect(&clip);		// get rect of the control
		ScreenToClient(&clip);
		pDC->ExcludeClipRect(&clip);
		pDC->GetClipBox(&clip);
		pDC->FillSolidRect(clip, m_crWindow);
	return TRUE;
#else
	return TRUE;	
#endif
	return CListCtrl::OnEraseBkgnd(pDC);
}


void CXListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	int nItem = -1;
	CRect rect;
	for(int i=0; i<GetItemCount(); i++)
	{
		if (CListCtrl::GetItemRect(i, &rect, LVIR_BOUNDS))
		{
			if (rect.PtInRect(point))
			{
				nItem = i;
				break;
			}
		}
	}

	CListCtrl::OnLButtonDown(nFlags, point);
}


//void CXListCtrl::OnMouseMove(UINT nFlags, CPoint point)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	CListCtrl::OnMouseMove(nFlags, point);
//}


void CXListCtrl::OnMouseLeave()
{
	// TODO: Add your message handler code here and/or call default

	CListCtrl::OnMouseLeave();
}


void CXListCtrl::OnNcPaint()
{
	// TODO: Add your message handler code here
	// Do not call CListCtrl::OnNcPaint() for painting messages
	Default();

	DWORD dwExStyle = GetExStyle();
	if( dwExStyle&WS_EX_STATICEDGE ||
		dwExStyle&WS_EX_CLIENTEDGE )
	{
		CRect rc;
		GetWindowRect(&rc);

		COLORREF cr = Global.GetRGB(IDX_RGB_CONTROL_BORDER);
	
		//Non-Client 영역의 DC를 얻어온다.
		CWindowDC dc(this);
		dc.Draw3dRect(0, 0, rc.Width(), rc.Height(), cr, cr);
	}
}


void CXListCtrl::OnPaint()
{
	Default();

	if(GetItemCount() <= 0)
	{
		CDC* pDC = GetDC();
		int nSavedDC = pDC->SaveDC();

		CRect rc;
		GetWindowRect(&rc);
		ScreenToClient(&rc);
		CHeaderCtrl* pHC = GetHeaderCtrl();
		if (pHC != NULL)
		{
			CRect rcH;
			pHC->GetItemRect(0, &rcH);
			rc.top += rcH.bottom;
		}

		CString strText = m_strNoItemMsg;
		COLORREF crText = m_crWindowText;
		COLORREF crBkgnd = m_crWindow;		
		pDC->SetTextColor(crText);
		pDC->SetBkColor(crBkgnd);
		pDC->SelectStockObject(ANSI_VAR_FONT);
		pDC->FillSolidRect(&rc, crBkgnd);

		rc.top += 10;
		pDC->DrawText(strText, -1, rc, DT_CENTER | DT_WORDBREAK | DT_NOPREFIX | DT_NOCLIP);

		pDC->RestoreDC(nSavedDC);
		ReleaseDC(pDC);
	}
	m_HeaderCtrl.RedrawWindow();
}
