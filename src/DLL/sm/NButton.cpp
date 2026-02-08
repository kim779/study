// NButton.cpp : implementation file
//

#include "stdafx.h"
#include "NButton.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	ID_TOOLTIP	1
/////////////////////////////////////////////////////////////////////////////
// CNButton

CNButton::CNButton(UINT id, COLORREF color, int mode, bool defaultColor)
{
	m_id = id;
	m_over = false;
	m_push = false;
	m_expand = false;
	m_imgMode = false;
	m_defaultColor = defaultColor;
	m_mode = mode;
	m_bk = m_overbk = color;
	m_ToolTip = NULL;
	m_imageType = IT_EACH;
	m_skin = false;

}

CNButton::~CNButton()
{
	XMSG();
	/**
	if (m_Icool.GetSafeHandle())	m_Icool.DeleteObject();
	if (m_Iover.GetSafeHandle())	m_Iover.DeleteObject();
	if (m_Idown.GetSafeHandle())	m_Idown.DeleteObject();
	**/

	if (CRgn::FromHandle(m_hRgn))	DeleteObject(m_hRgn);
}


BEGIN_MESSAGE_MAP(CNButton, CWnd)
	//{{AFX_MSG_MAP(CNButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNButton message handlers

void CNButton::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	const CFont*		font = NULL;

	if (m_imgMode)
	{
		if (DrawImg(&dc))
			return;
	}

	CRect	cRc;
	GetClientRect(cRc);

	if (m_over)
	{
		if (m_push)
			dc.SetBkColor(m_defaultColor ? ::GetSysColor(COLOR_BTNFACE):m_bk);
		else	
			dc.SetBkColor(m_defaultColor ? ::GetSysColor(COLOR_BTNFACE):m_overbk);
	}
	else	dc.SetBkColor(m_defaultColor ? ::GetSysColor(COLOR_BTNFACE):m_bk);
	dc.ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, cRc, "", 0, NULL);

	if (m_push)
	{
		CPoint	point;		GetCursorPos(&point);
		const CWnd* wndUnderMouse = WindowFromPoint(point);
		if (wndUnderMouse == this)
		{
			cRc.right--;	cRc.bottom--;
			dc.Draw3dRect(cRc, COLOR_RB, RGB(255, 255, 255));
			cRc.right++;	cRc.bottom++;
			dc.Draw3dRect(cRc, COLOR_RB, COLOR_LT);
		}
		else	dc.Draw3dRect(cRc, COLOR_LT, COLOR_LT);
	}
	else if (m_mode == MODE_3D || m_over)
	{
		dc.Draw3dRect(cRc, COLOR_LT, COLOR_RB);
		cRc.left++;	cRc.top++;
		dc.Draw3dRect(cRc, RGB(255, 255, 255), COLOR_RB);
	}
	else	dc.Draw3dRect(cRc, COLOR_LT, COLOR_LT);

// 	if(m_sText != "")
// 	{
// 		COLORREF	color;
// 		CRect rc;
// 		rc.top = 5;
// 		rc.left = 30;
// 
// 		dc.SetBkMode(TRANSPARENT);
// 		
// 		//font = dc.SelectObject(&m_font);
// 		
// 		color = dc.SetTextColor(14);
// 		dc.DrawText(m_sText, cRc, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
// 		dc.SetTextColor(color);
// 		dc.SelectObject(&font);
// 		font->DeleteObject();
// 		font = NULL;
// 	}
}

void CNButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	m_push = true;
	Invalidate(FALSE);

	CWnd::OnLButtonDown(nFlags, point);
}

void CNButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	m_push = false;
	Invalidate(FALSE);

	if(m_id == (CTRL_FUNC + 16))
	{
// 		CString s;
// 		s.Format("TOP10 SM [%d] [%d] [%d] [%d]\n",r.top,r.left,r.bottom,r.right);
// 		OutputDebugString(s);

		if (m_over)	GetParent()->PostMessage(WM_XMSG, MAKEWPARAM(m_id, 0), 0);
	}
	else
	if (m_over)	GetParent()->PostMessage(WM_XMSG, MAKEWPARAM(m_id, 0), 0);

	CWnd::OnLButtonUp(nFlags, point);
}

void CNButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd*			wndUnderMouse = NULL;
	const CWnd*			wndActive = this;
	TRACKMOUSEEVENT		csTME{};

	CWnd::OnMouseMove(nFlags, point);
	const CPoint	lpoint = point;
	ClientToScreen(&point);
	wndUnderMouse = WindowFromPoint(point);
	if (wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd && wndActive)
	{
		if (!m_over)
		{
			m_over = true;
			csTME.cbSize = sizeof(csTME);
			csTME.dwFlags = TME_LEAVE;
			csTME.hwndTrack = m_hWnd;
			::_TrackMouseEvent(&csTME);

// 			if(m_id == (CTRL_FUNC + 16))
// 			{
// 				if (m_over)	GetParent()->PostMessage(WM_XMSG, MAKEWPARAM(m_id, 0), 0);
// 			}
		}
	}
	else 
		m_over = false;

	if(m_id == CTRL_FUNC + 14 || m_id == CTRL_FUNC + 15 || m_id == CTRL_FUNC + 16)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE(IDC_HAND)));
	}

	Invalidate(FALSE);
}

LRESULT CNButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_over = false;
	Invalidate(FALSE);
	return 0;
}

void CNButton::OnDestroy() 
{
	XMSG();
	if (m_ToolTip)
	{
		if (m_ToolTip->m_hWnd)
		{
			//m_ToolTip->RelayEvent(NULL);
			m_ToolTip->Activate(FALSE);
			m_ToolTip->DelTool(this);
		}

#ifndef DF_USESTL_NBUTTON
	delete m_ToolTip;
#endif
		
		m_ToolTip = NULL;
	}

	CWnd::OnDestroy();
}

LRESULT CNButton::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		{
			if (!m_ToolTip || !IsWindow(m_ToolTip->GetSafeHwnd()))
				break;
			MSG	msg{};
			POINT	point;
			
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);

			msg.hwnd = m_hWnd;
			msg.message = message;
			msg.wParam = wParam;
			msg.lParam = lParam;
			msg.pt = point;
		
			m_ToolTip->RelayEvent(&msg);
		}
		break;
	default:
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

void CNButton::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	CRect	cRc;
	GetClientRect(cRc);

	if (m_ToolTip)			m_ToolTip->SetToolRect(this, ID_TOOLTIP, cRc);
	if (CRgn::FromHandle(m_hRgn))	DeleteObject(m_hRgn);

	cRc.right++;	cRc.bottom++;
	m_hRgn = CreateRoundRectRgn(cRc.left, cRc.top, cRc.right, cRc.bottom, 2, 2);
	if (CRgn::FromHandle(m_hRgn))	SetWindowRgn(m_hRgn, TRUE);
}

void CNButton::set_ToolTipText(CString text, BOOL bActivate)
{
	if (text.IsEmpty())	return;

	InitToolTip();
	if (m_ToolTip->GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip->AddTool(this, text, rectBtn, ID_TOOLTIP);
	}

	m_ToolTip->UpdateTipText(text, this, ID_TOOLTIP);
	m_ToolTip->Activate(bActivate);
}

void CNButton::InitToolTip()
{
	if (!m_ToolTip)
	{
#ifdef DF_USESTL_NBUTTON
		m_ToolTip = std::make_unique<CToolTipCtrl>();
#else
		m_ToolTip = new CToolTipCtrl();
#endif
		m_ToolTip->Create(GetDesktopWindow(), TTS_ALWAYSTIP);
		m_ToolTip->Activate(TRUE);
	}
}

void CNButton::change_img(CAxButtonName& bn)//CString cool, CString over, CString down)
{
	//set_Image(cool, over, down);
	set_Image(bn);
	Invalidate();
}

void CNButton::set_Image(UINT nID_cool, UINT nID_over, UINT nID_down)
{
}

//void CNButton::set_Image(CString img_cool, CString img_over, CString img_down)
void CNButton::set_Image(CAxButtonName& bn)
{
	m_imgMode = true;
	m_normal = bn.Normal();
	m_down   = bn.Down();
	m_hover   = bn.Hover();

	m_skin = bn.m_skin;

	m_imageType = IT_EACH;
}

void CNButton::set_Image(const char* buttonName)
{
	m_imgMode = true;
	m_normal = m_down = m_hover = buttonName;

	m_imageType = IT_INDEX;
}

bool CNButton::DrawImg(CDC* pDC)
{
	CRect	bRC;
	GetClientRect(bRC);
	CFont*		font = NULL;

	CDC	memDC;
	if (!memDC.CreateCompatibleDC(pDC))
		return false;

	CBitmap* bmp{};
	if( m_skin )
		bmp = Axis::GetSkinBitmap(m_normal);
	else
		bmp = Axis::GetBitmap(m_normal);
	int index = 0;

	if (m_imageType == IT_EACH)
	{
		if (m_over)
			if (m_push)
				if( m_skin )
					bmp = Axis::GetSkinBitmap(m_down);
				else
					bmp = Axis::GetBitmap(m_down);
			else
				if( m_skin )
					bmp = Axis::GetSkinBitmap(m_down);
				else
					bmp = Axis::GetBitmap(m_hover);
	}
	else if (m_imageType == IT_INDEX)
	{
		if (m_over)
			if (m_push)
				index = 2;
			else
				index = 1;
	}

	if (!bmp || bmp->m_hObject == NULL)
		return false;

	BITMAP	bm{};
	int	bx = bRC.Width();
	int	by = bRC.Height();


	CBitmap* oldbuffBitmap = memDC.SelectObject(bmp);

	bmp->GetBitmap(&bm);
	bx = bm.bmWidth;
	by = bm.bmHeight;

	if (m_imageType == IT_INDEX)
		bx = bx / 3;

	//pDC->StretchBlt(bRC.left, bRC.top, bRC.Width(), bRC.Height(), &memDC, index * bx, 0, bx, by, SRCCOPY);
	pDC->BitBlt(bRC.left, bRC.top, bRC.Width(), bRC.Height(), &memDC, index * bx, 0, SRCCOPY);
	if (!IsWindowEnabled())
	{
		pDC->FillSolidRect(0, 0, bx, by, RGB(140, 140, 140));
		pDC->StretchBlt(bRC.left, bRC.top, bRC.Width(), bRC.Height(), &memDC, index * bx, 0, bx, by, SRCAND);
	}
	memDC.SelectObject(oldbuffBitmap);
	//매수상위 배너를 위해서 만든 부분
	if(m_sText != "")
	{
//		OutputDebugString("TOP TEXT DRAW\n");
		const COLORREF	color{};
		const CRect rc;

		CString number,sText;
		number = m_sText.Mid(0,2);
		sText = m_sText.Mid(3);

		CString bmpName;
		bmpName.Format("buy_no%s",number);
// 		CString s;
// 		s.Format("TOP10 [%s] [%s]\n",bmpName,m_sText);
// 		OutputDebugString(s);

		HBITMAP hBitmap{};
		CString fileName;
		
		CString path(bmpName);
		path.MakeUpper();
		if (path.Find(".BMP") < 0)
			fileName.Format("%s\\image\\%s.bmp", Axis::home, bmpName);
		else
			fileName = path;

		hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), fileName, 
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);


#ifdef DF_USESTL_NBUTTON
		std::unique_ptr<CBitmap>bmpS = std::make_unique<CBitmap>();
#else
		CBitmap* bmpS = new CBitmap;
#endif
		

		if (hBitmap)
		{
			//TRACE("Loaded File Bitmap NAME = %s\n", bmpName);
			//TRACE("SUCCESS !!!!! = %s\n", bmpName);
			bmpS->Attach(hBitmap);
		}

		BITMAP	bm2{};
		if (bmpS && bmpS->m_hObject)
		{
			bmpS->GetBitmap(&bm2);
		}

#ifdef DF_USESTL_NBUTTON
		oldbuffBitmap = memDC.SelectObject(bmpS.get());
#else
		oldbuffBitmap = memDC.SelectObject(bmpS);
#endif

		bx = bm2.bmWidth;
		by = bm2.bmHeight;
		
		//pDC->StretchBlt(bRC.left, bRC.top, bRC.Width(), bRC.Height(), &memDC, index * bx, 0, bx, by, SRCCOPY);
		pDC->BitBlt(bRC.left+5, bRC.top+6, bm2.bmWidth, bm2.bmHeight, &memDC, 0, 0, SRCCOPY);

		memDC.SelectObject(oldbuffBitmap);
		
		pDC->SetBkMode(TRANSPARENT);
		
		font = pDC->SelectObject(&m_font);

		//color = pDC->SetTextColor(RGB(0,0,0));

		bRC.left = bm2.bmWidth + 10;
		bRC.right = bRC.right - 5;

		pDC->DrawText(sText, bRC, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
		//pDC->SetTextColor(color);
		pDC->SelectObject(&font);

		if(hBitmap)
			::DeleteObject(hBitmap);
	}

	return true;
}

CSize CNButton::get_ImgSize()
{
	BITMAP	bm{};
	CBitmap* bmp = Axis::GetBitmap(m_normal);
	if (!bmp)
		return CSize(0, 0);
	if (!bmp->GetSafeHandle())
		return CSize(0, 0);
	bmp->GetBitmap(&bm);

	return CSize(bm.bmWidth, bm.bmHeight);
}

void CNButton::set_ImageType(int imageType)
{
	TRY 
	{
		if (m_imageType != imageType)
		{
			m_imageType = imageType;
		}
	}
	CATCH (CMemoryException, e)
	{
		TCHAR szCause[500];

		e->GetErrorMessage(szCause,500);

		CString s;
		s.Format("NBUTTON MEMORY EXCEPTION [%s]\n",szCause);
		
		MessageBox(s,"Report",MB_OK);
	}
	END_CATCH
}

void CNButton::set_Text( CString text )
{
	m_sText = text;
	Invalidate();
}


int CNButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_font.CreateFont(	   12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_SEMIBOLD,	              // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"굴림");                 // lpszFacename
	
	return 0;
}
