// fxButton.cpp : implementation file
//

#include "stdafx.h"
#include "fxButton.h"
//#include "misctype.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CfxButton

CfxButton::CfxButton(CFont* pFont/*=NULL*/)
{
	//m_style   = bsTEXT|bsCOLOR;
	m_style   = NULL;
	m_text    = _T("");
	m_bPress = false;
	m_bitmap  = (CBitmap *)0;
	m_clrBACK = GetSysColor(COLOR_3DFACE);
	m_clrFORE = GetSysColor(COLOR_BTNTEXT);
	m_clrDISBACK = GetSysColor(COLOR_3DFACE);
	m_clrDISFORE = GetSysColor(COLOR_GRAYTEXT);
	m_clrTOGGLE = m_clrBACK;
	
	m_bPushed  = false;
	m_parent  = (CWnd *)0;

	m_pFont = pFont;
	m_bUser = false;
	m_bImgBtn = false;
	m_bTracking = FALSE;
	m_bToogleSel = false;
	m_hCursor = NULL;
}

CfxButton::~CfxButton()
{
	if (m_bitmap)
		m_bitmap->DeleteObject();
	if (m_hCursor) ::DestroyCursor(m_hCursor);
}

BEGIN_MESSAGE_MAP(CfxButton, CButton)
	//{{AFX_MSG_MAP(CfxButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfxButton message handlers

BOOL CfxButton::Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	if (m_bUser)
		dwStyle |= BS_OWNERDRAW; // Enforce
	m_parent = pParentWnd;

	return CButton::Create(lpszCaption, dwStyle, rect, pParentWnd, nID);
}

void CfxButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	if (!m_bUser)
		return;

	if (!IsWindowVisible())
		return;

	CDC	dc;
	CRect	rect, textRc, focusRc;

	dc.Attach(lpDIS->hDC);
	rect.CopyRect(&lpDIS->rcItem);
	textRc = rect;
	
	
	if(m_bImgBtn)
	{
		if(m_bToogleSel)
		{
			m_AxBmp[BMP_CN].Draw(&dc,rect);
		}
		//else if ((lpDIS->itemAction & ODA_FOCUS || lpDIS->itemState & ODS_FOCUS))
		//{
			else if(m_bPushed)
				m_AxBmp[BMP_DN].Draw(&dc,rect);
			else if(m_bTracking)
				m_AxBmp[BMP_EN].Draw(&dc,rect);
			else
				m_AxBmp[BMP_DF].Draw(&dc,rect);
		//}
		//else if(m_bTracking)
		//{
		//	m_AxBmp[BMP_EN].Draw(&dc,rect);
		//}
		//else
		//{
		//	m_AxBmp[BMP_DF].Draw(&dc,rect);
		//}
		GetWindowText(m_text);
	}
	else
	{
		//if (m_style & bsCOLOR)
		//{
		//	if ((m_style & bsTOGGLE) && m_bPress)
		//		dc.FillSolidRect(rect, m_clrTOGGLE);
		//	else
		//	{
		//		if (m_style & bsGRAD)
		//			drawGradRect(&dc, rect, m_clrBACK, m_clrTOGGLE);
		//		else	// 일반
		//		{
		//			if (ODS_DISABLED & lpDIS->itemState)
		//				dc.FillSolidRect(rect, m_clrDISBACK);
		//			else
		//				dc.FillSolidRect(rect, m_clrBACK);
		//		}
		//	}
		//}

		//if (m_style & bsTOGGLE)
		//{
		//	if (m_bPress)
		//		dc.Draw3dRect(rect.left, rect.top, rect.Width(), rect.Height(), 
		//			GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DHILIGHT));
		//	else
		//		dc.Draw3dRect(rect.left, rect.top, rect.Width(), rect.Height(), 
		//			GetSysColor(COLOR_3DHILIGHT), GetSysColor(COLOR_3DSHADOW));
		//}
		//if (m_style & bsPUSH)
		//{
		//	if (m_bPushed)
		//		dc.Draw3dRect(rect.left, rect.top, rect.Width(), rect.Height(), 
		//			GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DHILIGHT));
		//	else
		//		dc.Draw3dRect(rect.left, rect.top, rect.Width(), rect.Height(), 
		//			GetSysColor(COLOR_3DHILIGHT), GetSysColor(COLOR_3DSHADOW));
		//}
		//else if (m_style & bsGENERAL)
		//{
		//	if (m_bPushed)
		//		dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
		//	else
		//		dc.DrawEdge(&rect, EDGE_RAISED, BF_RECT);
		//}
		//else if (m_style & bsRECT)
		//{
		//	dc.DrawEdge(&rect, EDGE_BUMP, BF_RECT);
		//}
		//else	// bsFLATE/DEFAULT
		//{
		//	if (m_bPushed)
		//		dc.FrameRect(&lpDIS->rcItem, CBrush::FromHandle((HBRUSH)GetStockObject(BLACK_BRUSH)));
		//	else
		//		dc.DrawEdge(&rect, EDGE_BUMP, BF_RECT);
		//}

		//if (m_style & bsIMAGE)
		//{
		//	if (m_bitmap)
		//	{
		//		BITMAP	bm;
		//		UINT	flag = DST_BITMAP;
		//		CPoint	pt   = CPoint(2, 2);
		//		m_bitmap->GetBitmap(&bm);
		//		flag |= DSS_NORMAL;
		//		dc.DrawState(pt, CSize(bm.bmWidth, bm.bmHeight), m_bitmap, flag);
		//		textRc.left += (pt.x + bm.bmWidth + 2);
		//	}
		//}

		//if ((lpDIS->itemAction & ODA_FOCUS || lpDIS->itemState & ODS_FOCUS)) 
		//{
		//	focusRc.CopyRect(rect);
		//	focusRc.DeflateRect(3, 3);

		//	if (!(m_style & bsTOGGLE))
		//		dc.DrawFocusRect(&focusRc);
		//}
	}

	if (!m_text.IsEmpty())
	{
		//if ((m_style & bsTEXT) || ((m_style & bsIMAGE) && !m_bitmap))
		//{
			COLORREF clr;
			int	mode = dc.SetBkMode(TRANSPARENT);

			CGdiObject* font;
			if (m_pFont != NULL) font = dc.SelectObject(m_pFont);
			else
			{
				if (GetParent()->GetFont())
				{
					m_pFont = GetParent()->GetFont();
					font = dc.SelectObject(m_pFont);
				}
				else
				{
					font = dc.SelectStockObject(SYSTEM_FONT);
				}
			}

			if (ODS_DISABLED & lpDIS->itemState)
			{
				clr = dc.SetTextColor(m_clrDISFORE);
				dc.DrawText(m_text, textRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
			else
			{
				clr = dc.SetTextColor(m_clrFORE);
				dc.DrawText(m_text, textRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
			dc.SetTextColor(clr);
			dc.SelectObject(font);
			dc.SetBkMode(mode);
		//}
	}

	dc.Detach();
}

void CfxButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!m_bUser)
	{
		CButton::OnLButtonDown(nFlags, point);
		return;
	}

	//if (!(m_style & (bsRECT | bsTOGGLE)))
		m_bPushed = true;
	
	Invalidate(); CButton::OnLButtonDown(nFlags, point);
}

void CfxButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (!m_bUser)
	{
		CButton::OnLButtonUp(nFlags, point);
		return;
	}
	if (m_bPushed)
		m_bPushed = false;

	Invalidate();
	CButton::OnLButtonUp(nFlags, point);
}


void CfxButton::OnMouseLeave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bTracking = false;
	Invalidate();
	CButton::OnMouseLeave();
}

void CfxButton::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	Invalidate();
	CButton::OnMouseHover(nFlags, point);
}

void CfxButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bUser)
	{
		CButton::OnMouseMove(nFlags, point);
		return;
	}

	if(!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}

	if (m_bPushed)
	{
		ClientToScreen(&point);
		if (WindowFromPoint(point) != this)
		{
			m_bPushed = false;
			Invalidate();
		}
	}
	CButton::OnMouseMove(nFlags, point);
}


void CfxButton::OnSetFocus(CWnd* pOldWnd) 
{
	CButton::OnSetFocus(pOldWnd);
	Invalidate();	
}

void CfxButton::Init(DWORD style, CString text, CString images, 
		     COLORREF clrBACK, COLORREF clrFORE, COLORREF clrTOGGLE)
{
	//if (m_pFont != NULL)
	//{
	//	SetFont(m_pFont);
	//}
	if (!m_bUser)
	{
		SetText(text);
		return;
	}
	
	//bool	redraw = false;

	//if (m_style != style)
	//{
	//	m_style = style;
	//	redraw  = true;
	//}
	//if (m_style & bsTEXT)
	//{
	//	m_text = text;
	//	redraw = true;
	//}
	//if (m_style & bsIMAGE)
	//{
	//	SetBitmap(images, false);
	//	m_text = text;
	//	redraw = true;
	//	ModifyStyle(0, BS_OWNERDRAW);
	//}
	//if (m_style & bsCOLOR)
	//{
	//	m_clrBACK = clrBACK;
	//	m_clrFORE = clrFORE;
	//	m_clrDISBACK = clrBACK;
	//	m_clrTOGGLE = clrTOGGLE;
	//	redraw    = true;
	//	ModifyStyle(0, BS_OWNERDRAW);
	//}

	//if (redraw)
	//	Invalidate();
		
}

void CfxButton::SetStyle(DWORD style, bool redraw)
{
	if  (m_style == style)
		return;

	m_style = style;
	if (redraw)
		Invalidate();
}

void CfxButton::SetSelected(bool bFlag)
{
	m_bToogleSel = bFlag;
	Invalidate();
}

void CfxButton::SetBold(bool bFlag)
{

}

DWORD CfxButton::SetBtnCursor(int nCursorId, BOOL bRepaint)
{
	HINSTANCE	hInstResource = NULL;
	// Destroy any previous cursor
	if (m_hCursor)
	{
		::DestroyCursor(m_hCursor);
		m_hCursor = NULL;
	} // if

	// Load cursor
	if (nCursorId)
	{
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nCursorId), RT_GROUP_CURSOR);
		// Load cursor resource
		m_hCursor = (HCURSOR)::LoadImage(hInstResource, MAKEINTRESOURCE(nCursorId), IMAGE_CURSOR, 0, 0, 0);
		// Repaint the button
		if (bRepaint) Invalidate();
		// If something wrong
		if (m_hCursor == NULL) return BTNST_INVALIDRESOURCE;
	} // if

	return BTNST_OK;
} // End of SetBtnCursor

DWORD CfxButton::SetColor(BYTE byColorIndex, COLORREF crColor, BOOL bRepaint)
{
	return 0;
}

void CfxButton::SetText(CString text, bool redraw)
{
	if (!m_text.CompareNoCase(text))
		return;

	SetWindowText(text);
	m_text = text;
	if (redraw)
		Invalidate();
}

void CfxButton::SetBitmap(CString path, bool redraw)
{
	if (m_bitmap)
	{
		m_bitmap->DeleteObject();
		m_bitmap = (CBitmap *) 0;
	}

	HBITMAP hbitmap = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (hbitmap)
		m_bitmap = CBitmap::FromHandle(hbitmap);

	if (redraw)
		Invalidate();
}

void CfxButton::SetBkColor(COLORREF clrBACK, bool redraw)
{
	if (m_clrBACK == clrBACK)	return;
	m_clrBACK = clrBACK;
	if (redraw)	Invalidate();
}

void CfxButton::SetFgColor(COLORREF clrFORE, bool redraw)
{
	if (m_clrFORE == clrFORE)	return;
	m_clrFORE = clrFORE;
	if (redraw)	Invalidate();
}

void CfxButton::SetDisableBkColor(COLORREF clrDISBACK, bool redraw)
{
	if (m_clrDISBACK == clrDISBACK)	return;
	m_clrDISBACK = clrDISBACK;
	if (redraw)	Invalidate();
}

void CfxButton::SetDisableFgColor(COLORREF clrDISFORE, bool redraw)
{
	if (m_clrDISFORE == clrDISFORE)	return;
	m_clrDISFORE = clrDISFORE;
	if (redraw)	Invalidate();
}

void CfxButton::SetToggleColor(COLORREF clrTOGGLE, bool redraw)
{
	if (m_clrTOGGLE == clrTOGGLE)
		return;
	m_clrTOGGLE = clrTOGGLE;
	if (redraw)
		Invalidate();
}

void CfxButton::SetToggleStatus(bool bPress, bool redraw)
{
	if (m_bPress == bPress)
		return;
	m_bPress = bPress;
	if (redraw)
		Invalidate();
}

void CfxButton::SetFont(CFont* pFont, bool redraw)
{
	if (!m_bUser)
	{
		CButton::SetFont(pFont);
		return;
	}

	m_pFont = pFont;
	//this->SetFont(m_pFont);
	if (redraw) Invalidate();
}

void CfxButton::ModifyBtnStyle(DWORD remove, DWORD add, bool redraw)
{
	m_style &= ~remove;
	m_style |= add;
	if (redraw)
		Invalidate();
}

void CfxButton::drawGradRect(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor)
{
	CRect	rect;
	int	nWidth, nHeight;

	drawRC.right += 1;
	drawRC.bottom += 1;
	nWidth = drawRC.Width(); 
	nHeight = drawRC.Height();

	int rs,gs,bs;
	rs = GetRValue(sColor);
	gs = GetGValue(sColor);
	bs = GetBValue(sColor);

	int re,ge,be;
	re = GetRValue(eColor);
	ge = GetGValue(eColor);
	be = GetBValue(eColor);

	float	rStep, gStep, bStep;
	CPen	*sPen;

	if (!nHeight)
	{
		CPen	cPen(PS_SOLID, 1, sColor);
		sPen = pDC->SelectObject(&cPen);
		
		pDC->MoveTo(drawRC.left, drawRC.top);
		pDC->LineTo(drawRC.right, drawRC.bottom);
		pDC->SelectObject(sPen);
		cPen.DeleteObject();
		return;
	}


	rStep = float(rs - re)/float(nHeight);
	gStep = float(gs - ge)/float(nHeight);
	bStep = float(bs - be)/float(nHeight);

	if (!nWidth)
	{
		for (int ii = 0; ii < nHeight; ++ii)
		{
			CPen	cPen(PS_SOLID, 1, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
			sPen = pDC->SelectObject(&cPen);

			pDC->MoveTo(drawRC.left, drawRC.top + ii);
			pDC->LineTo(drawRC.left, drawRC.top + ii + 1);
			pDC->SelectObject(sPen);
			cPen.DeleteObject();
		}
	}
	else
	{
		for (int ii = 0; ii < nHeight; ++ii)
		{
			rect.SetRect(drawRC.left, drawRC.top + ii, drawRC.right, drawRC.top + ii + 1);
			
			CBrush brush;
			brush.CreateSolidBrush(RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
			pDC->FillRect(&rect, &brush);
			brush.DeleteObject();
		}
	}
}

void CfxButton::SetUser()
{
	m_bUser = true;
	//ModifyStyle(0,BS_OWNERDRAW);
	//CButton::ModifyStyle(0,BS_OWNERDRAW);
}

void CfxButton::LoadImg(CString path)
{
	m_bImgBtn = true;
	//m_hBmp[L_STAB] = (HBITMAP)::LoadImage (NULL, path + "GRAY_L_STAB.BMP",
	//	IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	//m_hBmp[C_STAB] = (HBITMAP)::LoadImage (NULL, path + "GRAY_STAB.BMP",
	//	IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	//m_hBmp[R_STAB] = (HBITMAP)::LoadImage (NULL, path + "GRAY_R_STAB.BMP",
	//	IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	//m_hBmp[L_TAB] = (HBITMAP)::LoadImage (NULL, path + "GRAY_L_TAB.BMP",
	//	IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	//m_hBmp[C_TAB] = (HBITMAP)::LoadImage (NULL, path + "GRAY_TAB.BMP",
	//	IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	//m_hBmp[R_TAB] = (HBITMAP)::LoadImage (NULL, path + "GRAY_R_TAB.BMP",
	//	IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	SetUser();
	CRect rc;
	GetClientRect(rc);
	char* sBtnName = (char*)malloc(10);
	CString strBtnName("");
	int nBtn = (int)(rc.right/10) - 1;
	itoa(nBtn,sBtnName,10);
	strBtnName.Format("%s",sBtnName);

	if(nBtn >= 11)
	{
		strBtnName = "11";
	}
	else if(nBtn > 7)
	{
		strBtnName = "7";
	}
	else if(nBtn < 1)
	{
		strBtnName = "1";
	}

	m_AxBmp[BMP_DF].LoadBitmap(path + strBtnName + "BTN.BMP");
	m_AxBmp[BMP_EN].LoadBitmap(path + strBtnName + "BTN_EN.BMP");
	m_AxBmp[BMP_DN].LoadBitmap(path + strBtnName + "BTN_DN.BMP");
	m_AxBmp[BMP_CN].LoadBitmap(path + strBtnName + "BTN_CN.BMP");
	free(sBtnName);
	CButton::ModifyStyle(0,BS_OWNERDRAW);
}

LOGFONT CfxButton::getLogFont(int fsize, DWORD weight)
{
	LOGFONT	lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = fsize * 10; 
	
	strcpy(lf.lfFaceName, "굴림체");
	
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	
	lf.lfWeight = weight;
	lf.lfItalic = FALSE;
	return lf;
}

void CfxButton::OnDestroy()
{
	CButton::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//for(int i=0; i< BMP_MAX; i++)
	//{
	//	m_AxBmp[i].DeleteObject();
	//}
}


BOOL CfxButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// If a cursor was specified then use it!
	if (m_hCursor != NULL)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	} // if
	return CButton::OnSetCursor(pWnd, nHitTest, message);
}
