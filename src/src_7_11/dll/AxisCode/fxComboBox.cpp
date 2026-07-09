// fxComboBox1.cpp : implementation file
//

#include "stdafx.h"
#include "fxComboBox.h"
//#include "memdc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLOR_WHITE	RGB(255, 255, 255)
#define COLOR_DISABLE	RGB(212, 208, 201)
#define COLOR_PAL_BG	PALETTERGB(27, 79, 129);
#define COLOR_PAL_FG	PALETTERGB(255, 145, 2);

/////////////////////////////////////////////////////////////////////////////
// CfxComboBox

CfxComboBox::CfxComboBox()
{
	m_pFont = NULL;
	m_pParent = NULL;
	m_bEnable = TRUE;
	m_bImgBtn = false;
	m_bCursorOnButton = false;
	m_bDropdownClick = false;
}

CfxComboBox::~CfxComboBox()
{
}


BEGIN_MESSAGE_MAP(CfxComboBox, CComboBox)
	//{{AFX_MSG_MAP(CfxComboBox)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ENABLE()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	//ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	//ON_CONTROL_REFLECT(CBN_CLOSEUP, &CfxComboBox::OnCbnCloseup)
	ON_CONTROL_REFLECT_EX(CBN_DROPDOWN, OnDropdown)
	ON_CONTROL_REFLECT_EX(CBN_CLOSEUP, &CfxComboBox::OnCbnCloseup)
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
//	ON_WM_SIZING()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfxComboBox message handlers

void CfxComboBox::OnDestroy() 
{
	CWnd::OnDestroy();
}

BOOL CfxComboBox::Create(int dwStyle, CRect rect, CWnd* parent, UINT nID)
{
	m_pParent = parent;
	return CComboBox::Create(dwStyle, rect, parent, nID);
}

void CfxComboBox::SetFont(CFont* pFont, bool redraw)
{
	m_pFont = pFont;
	CComboBox::SetFont(pFont, FALSE);
	
	if (redraw) RedrawWindow();
}

void CfxComboBox::LoadImg(CString path)
{
	m_bImgBtn = true;
	m_hBmp[BTN_DF] = (HBITMAP)::LoadImage (NULL, path + "AXEDIT.BMP",
		IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_hBmp[BTN_DN] = (HBITMAP)::LoadImage (NULL, path + "AXEDIT_DN.BMP",
		IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_hBmp[BTN_EN] = (HBITMAP)::LoadImage (NULL, path + "AXEDIT_EN.BMP",
		IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_hBmp[BTN_XN] = (HBITMAP)::LoadImage (NULL, path + "AXEDIT_XN.BMP",
		IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

	m_hBmpLoad = m_hBmp[BTN_DF];
}

void CfxComboBox::OnPaint()
{
	if(m_bImgBtn)
	{
		CPaintDC	dc(this);
		CDC		*pDC = &dc;
		//CRect rc, rcBtn, rcText;
		GetClientRect(&m_rc);
		CString strItem("");
		int iItemSel = -1;

		pDC->FillSolidRect(m_rc, COLOR_WHITE);
		CPen *OldPen, NewPen(PS_SOLID, 1, RGB(116,116,116));
		CPen pPen(PS_SOLID, 2, RGB(255,0, 0));
		OldPen = pDC->SelectObject(&NewPen);
		pDC->Rectangle(m_rc);
		pDC->SelectObject(m_pFont);
		pDC->SelectObject(&pPen);

		switch(GetStyle() & CBS_DROPDOWNLIST)
		{
		  case CBS_SIMPLE:
			// do stuff
			break;

		  case CBS_DROPDOWN:
		  case CBS_DROPDOWNLIST:
			  iItemSel = this->GetCurSel();
			  if(iItemSel != CB_ERR)
			  {
				  m_rcText.CopyRect(&m_rc);
				  m_rcText.left += 3;
				  this->GetLBText(this->GetCurSel(),strItem);
				  pDC->DrawText(strItem,m_rcText,DT_SINGLELINE |DT_VCENTER);
			  }
			break;
		}
		/////////////////////////////////////////////////////////////////////////////////////
		CDC  MemDC;
		CBitmap Bitmap, *pOldBitmap;
		BITMAP bm;

		MemDC.CreateCompatibleDC(pDC);
		Bitmap.Attach(m_hBmpLoad);
		Bitmap.GetObject(sizeof(bm), &bm);

		m_rcBtn.CopyRect(&m_rc);
		m_rcBtn.top = m_rc.top + 1;
		m_rcBtn.left = m_rc.right - bm.bmWidth - 1 - ((m_rc.bottom-2) - bm.bmHeight);
		m_rcBtn.right -= 1;
		m_rcBtn.bottom = m_rcBtn.top + bm.bmHeight + ((m_rc.bottom-2) - bm.bmHeight);

		pOldBitmap = MemDC.SelectObject(&Bitmap);
		//pDC->BitBlt(m_rcBtn.left, m_rcBtn.top, bm.bmWidth, bm.bmHeight, &MemDC, 0, 0, SRCCOPY);
		//pDC->BitBlt(m_rcBtn.left, m_rcBtn.top, m_rcBtn.Width(), m_rcBtn.Height(), &MemDC, 0, 0, SRCCOPY);
		pDC->SetStretchBltMode(COLORONCOLOR);
		pDC->StretchBlt(m_rcBtn.left, m_rcBtn.top, m_rcBtn.Width(), m_rcBtn.Height(), &MemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		MemDC.SelectObject(pOldBitmap);
		Bitmap.Detach();
		Bitmap.DeleteObject();
		//////////////////////////////////////////////////////////////////////////////////////
		pDC->SelectObject(OldPen);
		NewPen.DeleteObject();
	}
	else
	{
		CComboBox::OnPaint();
		return;
	//	CPaintDC	dc(this); 
	//	
	//	CDC		*pDC = &dc;
	//	
	//	COLORREF bg, fg;
	//	if (m_pParent != NULL)
	//	{
	//		bg = m_cbRGB;
	//		fg = m_csRGB;
	//	}
	//	else
	//	{
	//		bg = COLOR_PAL_BG;
	//		fg = COLOR_PAL_FG;
	//	}
	// 
	//	CRect rc, rcBtn, tRc;
	//	CPoint	pts[3];
	//	GetClientRect(&rc);
	//	if (m_bEnable == TRUE)
	//		pDC->FillSolidRect(rc, COLOR_WHITE);
	//	else
	//		pDC->FillSolidRect(rc, COLOR_DISABLE);
	//	pDC->FillSolidRect(rc, COLOR_WHITE);
	//	pDC->DrawEdge(&rc, EDGE_SUNKEN, BF_RECT);   
	//	
	//	rcBtn = rc;
	//	rcBtn.DeflateRect(0, 3, 2, 3);
	//	rcBtn.left = rcBtn.right - rcBtn.Height();
	//	tRc = rcBtn;
	//	tRc.DeflateRect(1, 0);
	//	
	//	CPen *open, pen(PS_SOLID, 1, bg);
	//	CBrush *obr, brush(bg);
	//	open = pDC->SelectObject(&pen);
	//	obr = pDC->SelectObject(&brush);
	//	pDC->FillRect(tRc, &brush);
	//
	//	pDC->MoveTo(rcBtn.left, rcBtn.top+1);
	//	pDC->LineTo(rcBtn.left, rcBtn.bottom-1);
	//	pDC->MoveTo(rcBtn.right-1, rcBtn.top+1);
	//	pDC->LineTo(rcBtn.right-1, rcBtn.bottom-1);
	//
	//	CPen pen1(PS_SOLID, 1, fg);
	//	CBrush brush1(fg);
	//	pDC->SelectObject(&pen1);
	//	pDC->SelectObject(&brush1);
	//
	//	tRc = rcBtn;
	//	tRc.DeflateRect(3, tRc.Height()*2/5);
	//	tRc.OffsetRect(1, 0);
	//	tRc.right = tRc.left + (tRc.Width()/2)*2-1;
	//	tRc.bottom = tRc.top + tRc.Width()/2;
	//	pts[0].y = tRc.bottom;
	//	pts[1].y = tRc.top;
	//	pts[2].y = tRc.top;
	//	pts[0].x = tRc.left + (tRc.Width()/2);
	//	pts[1].x = tRc.right-1;
	//	pts[2].x = tRc.left;
	//	pDC->Polygon(pts, 3);
	//
	//	if (m_pFont)
	//	{
	//		CString str = "";
	//		CFont *ofont;
	//		ofont = pDC->SelectObject(m_pFont);
	//		int index = this->GetCurSel();
	//		if (index != -1)
	//			this->GetLBText(index, str);
	//		rc.right = rcBtn.left;
	//		rc.DeflateRect(2, 2, 1, 2);
	//		if (m_bEnable == TRUE)
	//			pDC->FillSolidRect(rc, COLOR_WHITE);
	//		else
	//			pDC->FillSolidRect(rc, COLOR_DISABLE);
	//
	//		rc.DeflateRect(2, 0, 0, 0);
	//		pDC->DrawText(str, rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	//
	//		pDC->SelectObject(ofont);
	//	}
	//
	//	pDC->SelectObject(open);
	//	pDC->SelectObject(obr);
	//
	//// 2008. 3. 24. Kim Jung-tae GDI Leak Check Release Code
	//	pen.DeleteObject();
	//	pen1.DeleteObject();
	//	brush.DeleteObject();
	//	brush1.DeleteObject();
	}
}

void CfxComboBox::SetPalette(COLORREF clrFg, COLORREF clrBg)
{
	m_cbRGB = clrFg;
	m_csRGB = clrBg;
	Invalidate();
}

int CfxComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetHorizontalExtent(160);
	SetDroppedWidth(160);
	return 0;
}

BOOL CfxComboBox::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
	return TRUE;	
	return CComboBox::OnEraseBkgnd(pDC);
}

void CfxComboBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//CRect rc;
	//GetClientRect(&rc);

	////Invalidate();
	
	CComboBox::OnLButtonDown(nFlags, point);
}

void CfxComboBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//Invalidate();
	
	CComboBox::OnLButtonUp(nFlags, point);
}

void CfxComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	UINT action = lpDrawItemStruct->itemAction;
	UINT state = lpDrawItemStruct->itemState;
	CRect rcItem = lpDrawItemStruct->rcItem;
	CString strItem("");
	pDC->SelectObject(m_pFont);
	// Dropdown Item List 에서 마우스가 특정 아이템에 위치한다면 
	if((state & ODS_SELECTED) && (action & (ODA_SELECT | ODA_DRAWENTIRE))) {
		CBrush brHighlight(::GetSysColor(COLOR_HIGHLIGHT));
		CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&brHighlight);

		CPen penHighlight(PS_SOLID, 1, ::GetSysColor(COLOR_HIGHLIGHT));
		CPen* pOldPen = (CPen*)pDC->SelectObject(&penHighlight);

		pDC->Rectangle(&rcItem);
		pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));

		// SetItemData 함수를 사용하여 설정한 데이터를 얻는다 
		//_IconComboBoxData* pData = (_IconComboBoxData*)lpDrawItemStruct->itemData;
		//ASSERT(pData != NULL);
		
		GetLBText(lpDrawItemStruct->itemID,strItem);
		pDC->DrawText(strItem, rcItem, DT_SINGLELINE |DT_VCENTER ); 
		//if(pData && lpDrawItemStruct->itemData != 0xFFFFFFFF) {
		//	// 아이콘 그리기 
		//	DrawIconEx(pDC->m_hDC, rcItem.left + 1, rcItem.top + 1, pData->m_hIcon, 16, 16, 0, NULL, DI_NORMAL);

		//	// 문자열 그리기 
		//	rcItem.left += 19;
		//	pDC->DrawText(pData->m_strItem, rcItem, DT_SINGLELINE |DT_VCENTER ); 
		//}

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);

	// 2008. 3. 24. Kim Jung-tae GDI Leak Check Release Code
		brHighlight.DeleteObject();
		penHighlight.DeleteObject();
	}

	// Dropdown Item List 에서 마우스가 특정 아이템에 위치했던 예전 위치이면 
	if(!(state & ODS_SELECTED) && (action & (ODA_SELECT | ODA_DRAWENTIRE))) {
		CBrush brWindow(::GetSysColor(COLOR_WINDOW));
		CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&brWindow);

		CPen penHighlight(PS_SOLID, 1, ::GetSysColor(COLOR_WINDOW));
		CPen* pOldPen = (CPen*)pDC->SelectObject(&penHighlight);

		pDC->Rectangle(&rcItem);
		pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
		pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));

		pDC->Rectangle(&rcItem);
		pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
		pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));

		// SetItemData 함수를 사용하여 설정한 데이터를 얻는다 
		//_IconComboBoxData* pData = (_IconComboBoxData*)lpDrawItemStruct->itemData;
		//ASSERT(pData != NULL);
		GetLBText(lpDrawItemStruct->itemID,strItem);
		pDC->DrawText(strItem, rcItem, DT_SINGLELINE |DT_VCENTER ); 
		//if(pData && lpDrawItemStruct->itemData != 0xFFFFFFFF) {
		//	// 아이콘 그리기 
		//	DrawIconEx(pDC->m_hDC, rcItem.left + 1, rcItem.top + 1, pData->m_hIcon, 16, 16, 0, NULL, DI_NORMAL);

		//	// 문자열 그리기 
		//	rcItem.left += 19;
		//	pDC->DrawText(pData->m_strItem, rcItem, DT_SINGLELINE |DT_VCENTER ); 
		//}

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);

	// 2008. 3. 24. Kim Jung-tae GDI Leak Check Release Code
		brWindow.DeleteObject();
		penHighlight.DeleteObject();
	}	
} 

BOOL CfxComboBox::OnDropdown() 
{
	if(m_bImgBtn)
	{
		m_bDropdownClick = true;
		if(m_hBmpLoad != m_hBmp[BTN_DN])
		{
			m_hBmpLoad = m_hBmp[BTN_DN];
			InvalidateRect(m_rcBtn);
		}
	}
	return FALSE;
//return;
//	CString str;
//	int nWidth = 0;
//
//	CClientDC dc(this);
//	
//	CFont* pFont = m_pFont;
//	CFont* pOldFont = dc.SelectObject(pFont);
//
//	for(int index = 0; index < GetCount(); index++) {
//		// 문자열을 얻는다 
//		GetLBText(index, str);
//
//		// 리소스에 설정된 폰트를 기준으로 문자열의 사이즈를 얻는다 
//		CSize sz = dc.GetTextExtent(str);
//		
//		if(sz.cx > nWidth)	// 가장 사이즈 큰 값을 구한다 
//			nWidth = sz.cx;
//	}
//
//	nWidth += GetSystemMetrics(SM_CXVSCROLL);	// VScrroll 바의 크기를 더 한다 
//
//	SetDroppedWidth(nWidth);	// Dropdown List Width를 설정한다 
}

BOOL CfxComboBox::OnCbnCloseup()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_bImgBtn)
	{
		m_bDropdownClick = false;
		if(m_hBmpLoad != m_hBmp[BTN_DF])
		{
			m_hBmpLoad = m_hBmp[BTN_DF];
			InvalidateRect(m_rcBtn);
		}
	}
	
	return FALSE;
}

void CfxComboBox::OnEnable(BOOL bEnable) 
{
	CComboBox::OnEnable(bEnable);
	m_bEnable = bEnable;
	Invalidate();
}

void CfxComboBox::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(m_bImgBtn&&!m_bDropdownClick)
	{
		if(m_rcBtn.PtInRect(point))
		{
			if(m_hBmpLoad != m_hBmp[BTN_EN])
			{
				m_hBmpLoad = m_hBmp[BTN_EN];
				InvalidateRect(m_rcBtn);
			}
		}
		else
		{
			if(m_hBmpLoad != m_hBmp[BTN_DF])
			{
				m_hBmpLoad = m_hBmp[BTN_DF];
				InvalidateRect(m_rcBtn);
			}
		}
	}
	
	CComboBox::OnMouseMove(nFlags, point);
}

void CfxComboBox::OnMouseLeave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(m_bImgBtn&&!m_bDropdownClick)
	{
		if(m_hBmpLoad != m_hBmp[BTN_DF])
		{
			m_hBmpLoad = m_hBmp[BTN_DF];
			InvalidateRect(m_rcBtn);
		}
	}
	CComboBox::OnMouseLeave();
}

void CfxComboBox::OnSize(UINT nType, int cx, int cy)
{
	CComboBox::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
